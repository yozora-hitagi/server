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
			//master�뿪���������
			//��ʱ���������������������
			//��ô��һ���������ж��ڶ����оͻؼ�����ȥ����master˵bye��Ȼ����ӡ�
			//�ڶ��������˾Ͳ��ڶ��飬�Ͳ���˵bye��
			// �����뿪����� master û ����NULL
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
				//�����յ� �����뿪����Ϣ�������ж��뿪����master���͵��á�
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

				//����û������ ���Ǵ����ϣ���ĳ���ط����ܻ�����������Ȳ��������ˡ�
				//
				//��ȡ�������ʱ�����ӵ������
				string msg;
				WStrToUtf8(L"�ڴ���һ�ε�ð�գ�", msg);
				ai->TellMaster(msg);
				// ���ﵱ��������Զ����ʱ�� ȡ������ᴥ����������Ϊɶ�����������ȡ������ûɶ�����ˡ�

				//ȡ������
				/*ai->ChangeStrategy("-follow master", BOT_STATE_NON_COMBAT);
				ai->ChangeStrategy("-follow master", BOT_STATE_COMBAT);*/

				return LeaveGroupAction::Execute(event);
			}

            return false;
        }
    };

}
