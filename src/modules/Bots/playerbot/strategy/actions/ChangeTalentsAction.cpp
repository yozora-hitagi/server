#include "botpch.h"
#include "../../playerbot.h"
#include "ChangeTalentsAction.h"
#include "../../PlayerbotAIConfig.h"
#include "PlayerbotFactory.h"

using namespace ai;




bool ChangeTalentsAction::Execute(Event event)
{
	string arg = event.getParam();
	//int n = arg.find('=');
	//if (n != string::npos) {
	//	string spec = arg.substr(n);
	//	uint8 specNo = 0;
	//	if (spec.c_str()[0] == '1') {
	//		specNo = 1;
	//	}else if (spec.c_str()[0] == '2') {
	//		specNo = 2;
	//	}else if (spec.c_str()[0] == '3') {
	//		specNo = 3;
	//	}
	//	setspec(specNo);
	//}

	bool reset = false;
	if (strcmp(arg.c_str(), "reset") == 0) {
		reset = true;
	}else if (strcmp(arg.c_str(), "reset=0") == 0) {
		setspec(0);
		reset = true;
	}else if (strcmp(arg.c_str(), "reset=1") == 0) {
		setspec(1);
		reset = true;
	}else if (strcmp(arg.c_str(), "reset=2") == 0) {
		setspec(2);
		reset = true;
	}

	if (reset) {
		bot->resetTalents(true);
		string msg;
		WStrToUtf8(L"我的天赋重置了！", msg);
		ostringstream out;
		out << msg;
		ai->TellMaster(out.str());

		PlayerbotFactory factory(bot, bot->getLevel());
		factory.InitTalents();
	}

	report();

    return false;
}

void ChangeTalentsAction::setspec(uint8 specNo) {
	sPlayerbotAIConfig.SetEventValue(bot->GetObjectGuid(), "talent", specNo, 5 * 365 * 24 * 3600);


	if (ai->GetMaster()) {
		ostringstream out;
		string msg;
		WStrToUtf8(L"主天赋设置为 -> ", msg);
		out << msg;

		switch (specNo)
		{
		case 0:
			 WStrToUtf8(PlayerbotAIConfig::talentsDes[bot->getClass()].tab0,msg);
			break;
		case 1:
			 WStrToUtf8(PlayerbotAIConfig::talentsDes[bot->getClass()].tab1, msg);
			break;
		case 2:
			 WStrToUtf8(PlayerbotAIConfig::talentsDes[bot->getClass()].tab2, msg);
			break;

		default:
			msg = "";
			break;
		}

		out << msg;

		ai->TellMaster(out.str());
	}
}


void ChangeTalentsAction::report() {
	if (ai->GetMaster()) {

		ostringstream out;
		string msg;
		WStrToUtf8(L"我的主天赋是 - ", msg);
		out << msg;

		uint32 specNo = sPlayerbotAIConfig.GetEventValue(bot->GetObjectGuid(), "talent");
		switch (specNo)
		{
		case 0:
			WStrToUtf8(PlayerbotAIConfig::talentsDes[bot->getClass()].tab0, msg);
			break;
		case 1:
			WStrToUtf8(PlayerbotAIConfig::talentsDes[bot->getClass()].tab1, msg);
			break;
		case 2:
			WStrToUtf8(PlayerbotAIConfig::talentsDes[bot->getClass()].tab2, msg);
			break;

		default:
			msg = "";
			break;
		}
		out << msg;

		out << " - ( " << bot->GetTalentPointCosts(0) << " / " << bot->GetTalentPointCosts(1) << " / " << bot->GetTalentPointCosts(2) << " ) ";

		out << "\n usage: ~talent [reset[=0,1,2]]";

		ai->TellMaster(out.str());
	}

	
}