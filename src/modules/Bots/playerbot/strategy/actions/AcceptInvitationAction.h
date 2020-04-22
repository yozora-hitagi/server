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
			WStrToUtf8(wstring(L"����~ ������һ����ֵ�ð�հɣ�"), msg);

            ai->TellMaster(msg);
			

			//��� �����˾ʹ��͵���ǰ�Ĵ���
			//orientation ����
			float x, y, z;
			inviter->GetPosition(x, y, z);
			bot->GetMotionMaster()->Clear();
			bool ok = bot->TeleportTo(inviter->GetMapId(), x, y, z, bot->GetOrientation());

			//��FollowChatShortcutAction copy ���ĸ���
			ai->ChangeStrategy("+follow master,-passive", BOT_STATE_NON_COMBAT);
			ai->ChangeStrategy("-follow master,-passive", BOT_STATE_COMBAT);

            return true;
        }
    };

}
