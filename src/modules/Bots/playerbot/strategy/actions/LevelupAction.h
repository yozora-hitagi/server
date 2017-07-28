#pragma once

#include "../Action.h"

#include "PlayerbotFactory.h"

namespace ai
{
	class LevelupAction : public Action
	{
	public:
		LevelupAction(PlayerbotAI* ai) : Action(ai, "level up") {}

		virtual bool Execute(Event event)
		{
			bool hashMaster = ai->GetMaster();

			if (hashMaster) {
				string msg;
				WStrToUtf8(L"我的等级貌似有变化~", msg);
				ai->TellMaster(msg);
			}

			WorldPacket data = event.getPacket();
			uint32 level;
			data >> level;

			//干掉PlayerbotFactory 中的InitAvailableSpells，还是自己的感觉效率高些
			learnSpell();

			PlayerbotFactory factory(bot, level);
			factory.CleanRandomize();

			if (!bot->GetGroup()) {
				sRandomPlayerbotMgr.RandomTeleportForLevel(bot);
			}

			return true;
		}

		
		private:

			bool learnSpell() {
				bool hashMaster = ai->GetMaster();

				uint32 entry;
				switch (bot->getClass())
				{

				case Classes::CLASS_DRUID://德鲁伊
					entry = 4217;
					break;
				case Classes::CLASS_HUNTER://猎人
					entry = 3039;
					break;
				case Classes::CLASS_MAGE://法师
					entry = 3047;
					break;
				case Classes::CLASS_PALADIN://圣骑士
					entry = 928;
					break;
				case Classes::CLASS_PRIEST://牧师
					entry = 376;
					break;
				case Classes::CLASS_ROGUE://盗贼
					entry = 918;
					break;
				case Classes::CLASS_SHAMAN://萨满
					entry = 3032;
					break;
				case Classes::CLASS_WARLOCK://术士
					entry = 461;
					break;
				case Classes::CLASS_WARRIOR://战士
					entry = 914;
					break;
				}



				TrainerSpellData const* cSpells = sObjectMgr.GetNpcTrainerSpells(entry);
				TrainerSpellData const* tSpells = sObjectMgr.GetNpcTrainerTemplateSpells(entry);

				TrainerSpellData const* trainer_spells = cSpells;
				if (!trainer_spells)
					trainer_spells = tSpells;

				if (!trainer_spells)
					return false;

				if (hashMaster) {
					string msg;
					WStrToUtf8(L"远程免费学技能咯~~", msg);
					ai->TellMaster(msg);
				}

				string msg;
				WStrToUtf8(L"掌握技能", msg);

				int c = 0;

				int count = 1;
				do {
					count = 0;
					c++;
					for (TrainerSpellMap::const_iterator itr = trainer_spells->spellList.begin(); itr != trainer_spells->spellList.end(); ++itr)
					{
						TrainerSpell const* tSpell = &itr->second;
						if (!tSpell)
							continue;
						uint32 reqLevel = 0;
						reqLevel = tSpell->isProvidedReqLevel ? tSpell->reqLevel : std::max(reqLevel, tSpell->reqLevel);
						TrainerSpellState state = bot->GetTrainerSpellState(tSpell, reqLevel);
						if (state != TRAINER_SPELL_GREEN)
							continue;

						const SpellEntry *const pSpellInfo = sSpellStore.LookupEntry(tSpell->spell);
						if (!pSpellInfo)
							continue;

						ostringstream out;
						out << msg << chat->formatSpell(pSpellInfo);
						bot->CastSpell(bot, tSpell->spell, true);
						if (hashMaster) {
							ai->TellMaster(out);
						}
						count++;
					}
					if (hashMaster) {
						ai->TellMaster("---------------------");
					}
				} while (count != 0 && c <= 10);

				if (hashMaster&&c>10) {
					string msg;
					WStrToUtf8(L"学了不下十个循环了，先不学了！", msg);
					ai->TellMaster(msg);
				}
			}
	};

}
