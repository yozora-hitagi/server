#include "botpch.h"
#include "../../playerbot.h"
#include "ChangeTalentsAction.h"

using namespace ai;

bool ChangeTalentsAction::Execute(Event event)
{
	
	string arg = event.getParam();
	if (ai->GetMaster()) {
		string msg;
		WStrToUtf8(L"我的天赋是：",msg);
		ostringstream out;
		out << msg << " ( " << bot->GetTalentPointCosts(0) << " / " << bot->GetTalentPointCosts(1) << " / " << bot->GetTalentPointCosts(2) << " ) ";
		ai->TellMaster(out.str());
	}
    return false;
}
