#include "botpch.h"
#include "../../playerbot.h"
#include "ChangeTalentsAction.h"

using namespace ai;

bool ChangeTalentsAction::Execute(Event event)
{
	if (ai->GetMaster()) {
		string msg;
		WStrToUtf8(L"�������ûʵ��Ŷ~",msg);
		ai->TellMaster(msg);
	}
    return false;
}
