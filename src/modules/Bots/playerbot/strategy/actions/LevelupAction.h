#pragma once

#include "../Action.h"

namespace ai
{
	class LevelupAction : public Action
	{
	public:
		LevelupAction(PlayerbotAI* ai) : Action(ai, "level up") {}

		virtual bool Execute(Event event)
		{
			if (ai->GetMaster()) {
				string msg;
				WStrToUtf8(L"�ҵĵȼ�ò���б仯~", msg);
				ai->TellMaster(msg);
			}

			WorldPacket data = event.getPacket();

			uint32 level;
			data >> level;


			


			return true;
		}

		
	};

}
