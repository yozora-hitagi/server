#ifndef _RandomPlayerbotFactory_H
#define _RandomPlayerbotFactory_H

#include "Common.h"
#include "PlayerbotAIBase.h"

class WorldPacket;
class Player;
class Unit;
class Object;
class Item;

using namespace std;

class MANGOS_DLL_SPEC RandomPlayerbotFactory
{
    public:
        RandomPlayerbotFactory(uint32 accountId);
        virtual ~RandomPlayerbotFactory() {}

    public:
        bool CreateRandomBot(uint8 cls);

    private:
		/**
		���ݿⶼ�ĳɶ��������ˡ� ����Ӹ�ְҵ����
		**/
        bool CreateRandomBotName(uint8 cls,string &name,uint8 &gender);

    private:
        uint32 accountId;
        static map<uint8, vector<uint8> > availableRaces;
};

#endif
