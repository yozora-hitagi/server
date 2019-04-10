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
				WStrToUtf8(L"�ҵĵȼ�ò���б仯~", msg);
				ai->TellMaster(msg);
			}

			WorldPacket data = event.getPacket();
			uint32 level;
			data >> level;

			//�ɵ�PlayerbotFactory �е�InitAvailableSpells�������Լ��ĸо�Ч�ʸ�Щ
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

				case Classes::CLASS_DRUID://��³��
					entry = 4217;
					break;
				case Classes::CLASS_HUNTER://����
					entry = 3039;
					break;
				case Classes::CLASS_MAGE://��ʦ
					entry = 3047;
					break;
				case Classes::CLASS_PALADIN://ʥ��ʿ
					entry = 928;
					break;
				case Classes::CLASS_PRIEST://��ʦ
					entry = 376;
					break;
				case Classes::CLASS_ROGUE://����
					entry = 918;
					break;
				case Classes::CLASS_SHAMAN://����
					entry = 3032;
					break;
				case Classes::CLASS_WARLOCK://��ʿ
					entry = 461;
					break;
				case Classes::CLASS_WARRIOR://սʿ
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
					WStrToUtf8(L"Զ�����ѧ���ܿ�~~", msg);
					ai->TellMaster(msg);
				}

				string msg;
				WStrToUtf8(L"���ռ���", msg);

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
					WStrToUtf8(L"ѧ�˲���ʮ��ѭ���ˣ��Ȳ�ѧ�ˣ�", msg);
					ai->TellMaster(msg);
				}
			}
	};

}
