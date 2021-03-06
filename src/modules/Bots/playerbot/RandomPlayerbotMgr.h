#ifndef _RandomPlayerbotMgr_H
#define _RandomPlayerbotMgr_H

#include "Common.h"
#include "PlayerbotAIBase.h"
#include "PlayerbotMgr.h"

class WorldPacket;
class Player;
class Unit;
class Object;
class Item;

using namespace std;

class MANGOS_DLL_SPEC RandomPlayerbotMgr : public PlayerbotHolder
{
    public:
        RandomPlayerbotMgr();
        virtual ~RandomPlayerbotMgr();

        virtual void UpdateAIInternal(uint32 elapsed);

    public:
        bool IsRandomBot(Player* bot);
        bool IsRandomBot(uint32 bot);
        void Randomize(Player* bot);
        void RandomizeFirst(Player* bot);
        void IncreaseLevel(Player* bot);
       // void ScheduleTeleport(uint32 bot);
		void HandleCommand(uint32 type, const string& text, Player& fromPlayer);

		void HandleChannelMessage(Channel* chn, uint32 type, const string& text, Player& fromPlayer);

        void OnPlayerLogout(Player* player);
        void OnPlayerLogin(Player* player);
        Player* GetRandomPlayer();
        void PrintStats();
        double GetBuyMultiplier(Player* bot);
        double GetSellMultiplier(Player* bot);
        uint32 GetLootAmount(Player* bot);
        void SetLootAmount(Player* bot, uint32 value);
        uint32 GetTradeDiscount(Player* bot);
        void Refresh(Player* bot);

		//考虑每次升级的时候调用下 随机，所以这里改成public方法。
		void RandomTeleportForLevel(Player* bot);
    protected:
        virtual void OnBotLoginInternal(Player * const bot) {}

    private:

        list<uint32> GetBots();
        vector<uint32> GetFreeBots(bool alliance);
        uint32 AddRandomBot(bool alliance);
        bool ProcessBot(uint32 bot, Player* player);
       // void ScheduleRandomize(uint32 bot, uint32 time);
        void RandomTeleport(Player* bot, uint32 mapId, float teleX, float teleY, float teleZ);
        
        void RandomTeleport(Player* bot, vector<WorldLocation> &locs);
        uint32 GetZoneLevel(uint32 mapId, float teleX, float teleY, float teleZ);

    private:
        vector<Player*> players;
        int processTicks;
		//PlayerBotMap::const_iterator flag_bot;
		uint32 flag_bot;
};

#define sRandomPlayerbotMgr MaNGOS::Singleton<RandomPlayerbotMgr>::Instance()

#endif
