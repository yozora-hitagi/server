#pragma once

#include "../Action.h"

namespace ai
{
	class PetitionSignAction : public Action
	{
	public:
		PetitionSignAction(PlayerbotAI* ai) : Action(ai, "petition sign") {}

		virtual bool Execute(Event event)
		{
			WorldPacket data = event.getPacket();
			ObjectGuid petitionguid;
			ObjectGuid ownerguid;
			uint32 guildguid;//就是petitionguid的getcounter
			uint8 signs;
			data >> petitionguid;
			data >> ownerguid;
			data >> guildguid;
			data >> signs;

			if (signs >= 9) {
				string msg;
				WStrToUtf8(L"已经签满啦！", msg);
				bot->Whisper(msg, Language::LANG_UNIVERSAL, ownerguid);
				return false;
			}

			for (uint8 i = 1; i <= signs; ++i)
			{
				ObjectGuid signerGuid;
				uint32 uk;
				data >> signerGuid;                     // Player GUID
				data >> uk;                                  // there 0 ...

				if (signerGuid.GetCounter() == bot->GetObjectGuid().GetCounter()) {
					string msg;
					WStrToUtf8(L"我已经帮你签过了，你再找别人吧！", msg);
					bot->Whisper(msg, Language::LANG_UNIVERSAL, ownerguid);
					return false;
				}

			}

			//第一个参数，Opcode，第二个buffer size
			WorldPacket data2(CMSG_PETITION_SIGN,8+8);
			data2 << petitionguid;
			data2 << 0;
			bot->GetSession()->HandlePetitionSignOpcode(data2);

			string msg;
			WStrToUtf8(L"我已经帮你在公会登记表上签字啦~", msg);
			bot->Whisper(msg, Language::LANG_UNIVERSAL, ownerguid);
			return true;
		}
	};

}
