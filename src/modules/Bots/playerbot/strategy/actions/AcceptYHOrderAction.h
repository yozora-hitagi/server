#pragma once

#include "../Action.h"

namespace ai
{
	class AcceptYHOrderAction : public Action
	{
	public:
		AcceptYHOrderAction(PlayerbotAI* ai) : Action(ai, "yh_order"){}

		virtual bool Execute(Event event)
		{
			if (!ai->GetMaster()) {
				return false;
			}

			string link = event.getParam();
			handlePetitionSign(link);

			ai->TellMaster("|TInterface\\AddOns\\PlayerBot\\Images\\gold:0|t |TInterface\\AddOns\\PlayerBot\\Images\\gold|t ");

			return true;
		}

	private:
		bool handlePetitionSign(string &link) {
			ItemIds itemIds = chat->parseItems(link);
			if (!itemIds.empty()) {
				uint32 itemId = *itemIds.begin();
				//5863 公会登记表
				if (itemId == 5863) {
					string msg;
					WStrToUtf8(L"直接对我要求签名就行了，别再用命令做了~", msg);
					ai->TellMaster(msg);
					/*uint32 entry = ai->GetMaster()->GetObjectGuid();
					QueryResult* result = CharacterDatabase.PQuery("SELECT petitionguid FROM petition WHERE ownerguid = '%u'", entry);
					if (result) {
					Field* fields = result->Fetch();
					ObjectGuid petitionguid = ObjectGuid(HIGHGUID_PLAYER, fields[0].GetUInt32());
					WorldPacket data;
					data << petitionguid;
					data << 0;
					bot->GetSession()->HandlePetitionSignOpcode(data);
					}*/
					return true;
				}
			}
			return false;
		}
	};

}
