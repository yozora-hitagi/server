#pragma once

#include "../Action.h"
#include "../../RandomPlayerbotMgr.h"

namespace ai
{
    class LeaveGroupAction : public Action {
    public:
        LeaveGroupAction(PlayerbotAI* ai, string name = "leave") : Action(ai, name) {}

        virtual bool Execute(Event event)
        {
			//master离开后会调用这里，
			//这时候队伍中至少两个机器人
			//那么第一个机器人判断在队伍中就回继续下去，对master说bye，然后离队。
			//第二个机器人就不在队伍，就不会说bye。
			// 最后个离开队伍的 master 没 设置NULL
			if (!bot->GetGroup() && !ai->GetMaster()) {
				return false;
			}

            ai->TellMaster("Goodbye!", PLAYERBOT_SECURITY_TALK);

			if (bot->GetGroup()) {
				WorldPacket p;
				string member = bot->GetName();
				p << uint32(PARTY_OP_LEAVE) << member << uint32(0);
				bot->GetSession()->HandleGroupDisbandOpcode(p);
			}

            if (sRandomPlayerbotMgr.IsRandomBot(bot))
            {
                bot->GetPlayerbotAI()->SetMaster(NULL);
                //sRandomPlayerbotMgr.ScheduleTeleport(bot->GetGUIDLow());
                sRandomPlayerbotMgr.SetLootAmount(bot, 0);

				sRandomPlayerbotMgr.RandomTeleportForLevel(bot);

            }

            ai->ResetStrategies();
            return true;
        }
    };

    class PartyCommandAction : public LeaveGroupAction {
    public:
        PartyCommandAction(PlayerbotAI* ai) : LeaveGroupAction(ai, "party command") {}

        virtual bool Execute(Event event)
        {
            WorldPacket& p = event.getPacket();
            p.rpos(0);
            uint32 operation;
            string member;

            p >> operation >> member;

            if (operation != PARTY_OP_LEAVE)
                return false;

            Player* master = GetMaster();
			if (master && member == master->GetName()) {
				//上面收到 有人离开的消息，这里判断离开的是master，就调用。
				return LeaveGroupAction::Execute(event);
			}

            return false;
        }
    };

    class UninviteAction : public LeaveGroupAction {
    public:
        UninviteAction(PlayerbotAI* ai) : LeaveGroupAction(ai,"party command" ) {}//"uninvite"

		virtual bool Execute(Event event)
		{
			WorldPacket& p = event.getPacket();
			p.rpos(0);
			ObjectGuid guid;

			p >> guid;

			if (bot->GetObjectGuid() == guid) {

				//这里没过来， 但是代码上，在某个地方可能会过来，所以先不管这里了。
				//
				//在取消邀请的时候，增加点儿处理
				string msg;
				WStrToUtf8(L"期待下一次的冒险！", msg);
				ai->TellMaster(msg);
				// 这里当机器人在远处的时候 取消邀请会触发，不晓得为啥，所以这里的取消跟随没啥意义了。

				//取消跟随
				/*ai->ChangeStrategy("-follow master", BOT_STATE_NON_COMBAT);
				ai->ChangeStrategy("-follow master", BOT_STATE_COMBAT);*/

				return LeaveGroupAction::Execute(event);
			}

            return false;
        }
    };

}
