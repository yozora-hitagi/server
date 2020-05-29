#include "botpch.h"
#include "../../playerbot.h"
#include "ChangeTalentsAction.h"

using namespace ai;

bool ChangeTalentsAction::Execute(Event event)
{
	
	string arg = event.getParam();
	if (ai->GetMaster()) {
		string msg;
		WStrToUtf8(L"�ҵ��츳�ǣ�",msg);
		ostringstream out;
		out << msg << " ( " << bot->GetTalentPointCosts(0) << " / " << bot->GetTalentPointCosts(1) << " / " << bot->GetTalentPointCosts(2) << " ) ";
		ai->TellMaster(out.str());
	}
    return false;
}
