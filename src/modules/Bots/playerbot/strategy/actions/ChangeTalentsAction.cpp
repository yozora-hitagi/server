#include "botpch.h"
#include "../../playerbot.h"
#include "ChangeTalentsAction.h"

using namespace ai;

bool ChangeTalentsAction::Execute(Event event)
{
	if (ai->GetMaster()) {
		string msg;
		WStrToUtf8(L"这个命令没实现哦~",msg);
		ai->TellMaster(msg);
	}
    return false;
}
