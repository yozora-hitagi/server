#pragma once

#include "../Action.h"

namespace ai
{
    class AcceptInvitationAction : public Action {
    public:
        AcceptInvitationAction(PlayerbotAI* ai) : Action(ai, "accept invitation") {}

        virtual bool Execute(Event event)
        {
            Player* master = GetMaster();

            Group* grp = bot->GetGroupInvite();
            if (!grp)
            {
                return false;
            }

            Player* inviter = sObjectMgr.GetPlayer(grp->GetLeaderGuid());
            if (!inviter)
            {
                return false;
            }

            if (!ai->GetSecurity()->CheckLevelFor(PLAYERBOT_SECURITY_INVITE, false, inviter))
            {
                WorldPacket data(SMSG_GROUP_DECLINE, 10);
                data << bot->GetName();
                inviter->GetSession()->SendPacket(&data);
                bot->UninviteFromGroup();
                return false;
            }

            WorldPacket p;
            uint32 roles_mask = 0;
            p << roles_mask;
            bot->GetSession()->HandleGroupAcceptOpcode(p);

            if (sRandomPlayerbotMgr.IsRandomBot(bot))
                bot->GetPlayerbotAI()->SetMaster(inviter);

            ai->ResetStrategies();

			//"Hello, thanks for inviting me!"
			string msg;
			WStrToUtf8(wstring(L"哈哈~ 让我们一起快乐的冒险吧！"), msg);

            ai->TellMaster(msg);


			//这里添加邀请组队的时候，如果等级 1 ，就和玩家等级一样
			if (bot->getLevel() == 1) {
				bot->GiveLevel(inviter->getLevel());
			}
			

			//添加 邀请了就传送到跟前的代码
			//orientation 方向
			float x, y, z;
			inviter->GetPosition(x, y, z);
			bot->GetMotionMaster()->Clear();
			bool ok = bot->TeleportTo(inviter->GetMapId(), x, y, z, bot->GetOrientation());

			//从FollowChatShortcutAction copy 来的跟随
			ai->ChangeStrategy("+follow master,-passive", BOT_STATE_NON_COMBAT);
			ai->ChangeStrategy("-follow master,-passive", BOT_STATE_COMBAT);

            return true;
        }
    };

}
