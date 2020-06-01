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
        bool CreateRandomBot();

    private:
		/**
		数据库都改成动漫人物了。 这里加个职业区别
		**/
        bool CreateRandomBotName(string &name,uint8 &gender, uint8 &cls, uint8 &race);

    private:
        uint32 accountId;
		static map<uint8, vector<uint8> > availableRaces;

};

#endif
