#include "../botpch.h"
#include "PlayerbotAIConfig.h"
#include "playerbot.h"
#include "RandomPlayerbotFactory.h"
#include "AccountMgr.h"
#include "SystemConfig.h"

using namespace std;

map<uint8, talent_des >  PlayerbotAIConfig::talentsDes;

INSTANTIATE_SINGLETON_1(PlayerbotAIConfig);



PlayerbotAIConfig::PlayerbotAIConfig()
{

	talentsDes[CLASS_WARRIOR] = { L"武器", L"狂怒", L"防御" };
	talentsDes[CLASS_PALADIN] = { L"神圣", L"防护", L"惩戒" };
	talentsDes[CLASS_HUNTER] = { L"野兽掌握", L"射击", L"生存" };
	talentsDes[CLASS_ROGUE] = { L"刺杀", L"战斗", L"敏锐" };
	talentsDes[CLASS_PRIEST] = { L"戒律", L"神圣", L"暗影" };
	talentsDes[CLASS_SHAMAN] = { L"元素", L"增强", L"恢复" };
	talentsDes[CLASS_MAGE] = { L"奥术", L"火焰", L"冰霜" };
	talentsDes[CLASS_WARLOCK] = { L"痛苦", L"恶魔学识", L"毁灭" };
	talentsDes[CLASS_DRUID] = { L"平衡", L"野性战斗", L"恢复" };

}

template <class T>
void LoadList(string value, T &list)
{
    vector<string> ids = split(value, ',');
    for (vector<string>::iterator i = ids.begin(); i != ids.end(); i++)
    {
        uint32 id = atoi((*i).c_str());
        if (!id)
            continue;

        list.push_back(id);
    }
}

bool PlayerbotAIConfig::Initialize()
{
    //sLog.outString("Initializing AI Playerbot by ike3, based on the original Playerbot by blueboy");

    if (!config.SetSource(SYSCONFDIR"aiplayerbot.conf"))
    {
        sLog.outString("AI Playerbot is Disabled. Unable to open configuration file aiplayerbot.conf");
        return false;
    }

    enabled = config.GetBoolDefault("AiPlayerbot.Enabled", true);
    if (!enabled)
    {
        sLog.outString("AI Playerbot is Disabled in aiplayerbot.conf");
        return false;
    }

    globalCoolDown = (uint32) config.GetIntDefault("AiPlayerbot.GlobalCooldown", 500);
    maxWaitForMove = config.GetIntDefault("AiPlayerbot.MaxWaitForMove", 3000);
    reactDelay = (uint32) config.GetIntDefault("AiPlayerbot.ReactDelay", 100);

    sightDistance = config.GetFloatDefault("AiPlayerbot.SightDistance", 50.0f);
    spellDistance = config.GetFloatDefault("AiPlayerbot.SpellDistance", 30.0f);
    reactDistance = config.GetFloatDefault("AiPlayerbot.ReactDistance", 150.0f);
    grindDistance = config.GetFloatDefault("AiPlayerbot.GrindDistance", 100.0f);
    lootDistance = config.GetFloatDefault("AiPlayerbot.LootDistance", 20.0f);
    fleeDistance = config.GetFloatDefault("AiPlayerbot.FleeDistance", 20.0f);
    tooCloseDistance = config.GetFloatDefault("AiPlayerbot.TooCloseDistance", 7.0f);
    meleeDistance = config.GetFloatDefault("AiPlayerbot.MeleeDistance", 1.5f);
    followDistance = config.GetFloatDefault("AiPlayerbot.FollowDistance", 1.5f);
    whisperDistance = config.GetFloatDefault("AiPlayerbot.WhisperDistance", 6000.0f);
    contactDistance = config.GetFloatDefault("AiPlayerbot.ContactDistance", 0.5f);

    criticalHealth = config.GetIntDefault("AiPlayerbot.CriticalHealth", 20);
    lowHealth = config.GetIntDefault("AiPlayerbot.LowHealth", 50);
    mediumHealth = config.GetIntDefault("AiPlayerbot.MediumHealth", 70);
    almostFullHealth = config.GetIntDefault("AiPlayerbot.AlmostFullHealth", 85);
    lowMana = config.GetIntDefault("AiPlayerbot.LowMana", 15);
    mediumMana = config.GetIntDefault("AiPlayerbot.MediumMana", 40);

    randomGearLoweringChance = config.GetFloatDefault("AiPlayerbot.RandomGearLoweringChance", 0.15f);
    randomBotMaxLevelChance = config.GetFloatDefault("AiPlayerbot.RandomBotMaxLevelChance", 0.02f);

    iterationsPerTick = config.GetIntDefault("AiPlayerbot.IterationsPerTick", 4);

    allowGuildBots = config.GetBoolDefault("AiPlayerbot.AllowGuildBots", true);

	//530 是外域，571 是诺森德 ， 60级是没得滴
	//randomBotMapsAsString = config.GetStringDefault("AiPlayerbot.RandomBotMaps", "0,1,530,571");
	randomBotMapsAsString = config.GetStringDefault("AiPlayerbot.RandomBotMaps", "0,1");
    LoadList<vector<uint32> >(randomBotMapsAsString, randomBotMaps);
    LoadList<list<uint32> >(config.GetStringDefault("AiPlayerbot.RandomBotQuestItems", "6948,5175,5176,5177,5178"), randomBotQuestItems);

	//54197 - cold weather flying 貌似是寒冷天气飞行，没验证
    LoadList<list<uint32> >(config.GetStringDefault("AiPlayerbot.RandomBotSpellIds", "54197"), randomBotSpellIds);

    randomBotAutologin = config.GetBoolDefault("AiPlayerbot.RandomBotAutologin", true);

	//反正在数据那里也是只随机一次，不如在配置文件就定下来。还免得每次循环查数据库
    //minRandomBots = config.GetIntDefault("AiPlayerbot.MinRandomBots", 20);
    //maxRandomBots = config.GetIntDefault("AiPlayerbot.MaxRandomBots", 45);
	randomBotsCount= config.GetIntDefault("AiPlayerbot.RandomBotsCount", 45);

	//把原来的 固定的更新间隔 改成了浮动的
	/*randomBotUpdateMinInterval = config.GetIntDefault("AiPlayerbot.RandomBotUpdateMinInterval", 60);
	randomBotUpdateMaxInterval = config.GetIntDefault("AiPlayerbot.RandomBotUpdateMaxInterval", 120);*/
	randomBotUpdateStepInterval = config.GetIntDefault("AiPlayerbot.RandomBotUpdateStepInterval", 10);

    //randomBotCountChangeMinInterval = config.GetIntDefault("AiPlayerbot.RandomBotCountChangeMinInterval", 24 * 3600);
    //randomBotCountChangeMaxInterval = config.GetIntDefault("AiPlayerbot.RandomBotCountChangeMaxInterval", 3 * 24 * 3600);

    minRandomBotInWorldTime = config.GetIntDefault("AiPlayerbot.MinRandomBotInWorldTime", 2 * 3600);
    maxRandomBotInWorldTime = config.GetIntDefault("AiPlayerbot.MaxRandomBotInWorldTime", 14 * 24 * 3600);

	//修改每次随机给点儿经验了。 这里的默认值太大了
   /* minRandomBotRandomizeTime = config.GetIntDefault("AiPlayerbot.MinRandomBotRandomizeTime", 2 * 3600);
    maxRandomBotRandomizeTime = config.GetIntDefault("AiPlayerbot.MaxRandomRandomizeTime", 14 * 24 * 3600);*/

	//更新循环变了。 这里用不上了
	//minRandomBotRandomizeTime = config.GetIntDefault("AiPlayerbot.MinRandomBotRandomizeTime", 120);
	//考虑每次给弹药1000个，所以，就算攻速1.0 这里也还好。
	//maxRandomBotRandomizeTime = config.GetIntDefault("AiPlayerbot.MaxRandomRandomizeTime", 1000);

    minRandomBotReviveTime = config.GetIntDefault("AiPlayerbot.MinRandomBotReviveTime", 60);
    maxRandomBotReviveTime = config.GetIntDefault("AiPlayerbot.MaxRandomReviveTime", 300);
    randomBotTeleportDistance = config.GetIntDefault("AiPlayerbot.RandomBotTeleportDistance", 1000);

    //minRandomBotsPerInterval = config.GetIntDefault("AiPlayerbot.MinRandomBotsPerInterval", 50);
    //maxRandomBotsPerInterval = config.GetIntDefault("AiPlayerbot.MaxRandomBotsPerInterval", 100);

    minRandomBotsPriceChangeInterval = config.GetIntDefault("AiPlayerbot.MinRandomBotsPriceChangeInterval", 2 * 3600);
    maxRandomBotsPriceChangeInterval = config.GetIntDefault("AiPlayerbot.MaxRandomBotsPriceChangeInterval", 48 * 3600);
    randomBotJoinLfg = config.GetBoolDefault("AiPlayerbot.RandomBotJoinLfg", true);
    logInGroupOnly = config.GetBoolDefault("AiPlayerbot.LogInGroupOnly", true);
    logValuesPerTick = config.GetBoolDefault("AiPlayerbot.LogValuesPerTick", false);
    fleeingEnabled = config.GetBoolDefault("AiPlayerbot.FleeingEnabled", true);
    randomBotMinLevel = config.GetIntDefault("AiPlayerbot.RandomBotMinLevel", 1);
    randomBotMaxLevel = config.GetIntDefault("AiPlayerbot.RandomBotMaxLevel", 255);
    randomBotLoginAtStartup = config.GetBoolDefault("AiPlayerbot.RandomBotLoginAtStartup", true);
    randomBotTeleLevel = config.GetIntDefault("AiPlayerbot.RandomBotTeleLevel", 3);

    randomChangeMultiplier = config.GetFloatDefault("AiPlayerbot.RandomChangeMultiplier", 1.0);

    randomBotCombatStrategies = config.GetStringDefault("AiPlayerbot.RandomBotCombatStrategies", "+dps,+attack weak");
    randomBotNonCombatStrategies = config.GetStringDefault("AiPlayerbot.RandomBotNonCombatStrategies", "+grind,+move random,+loot");

    commandPrefix = config.GetStringDefault("AiPlayerbot.CommandPrefix", "");

    commandServerPort = config.GetIntDefault("AiPlayerbot.CommandServerPort", 0);

    for (uint32 cls = 0; cls < MAX_CLASSES; ++cls)
    {
        for (uint32 spec = 0; spec < 3; ++spec)
        {
            ostringstream os; os << "AiPlayerbot.RandomClassSpecProbability." << cls << "." << spec;
            specProbability[cls][spec] = config.GetIntDefault(os.str().c_str(), 33);
        }
    }

    CreateRandomBots();
    sLog.outString("AI Playerbot configuration loaded");

    return true;
}


bool PlayerbotAIConfig::IsInRandomAccountList(uint32 id)
{
    return find(randomBotAccounts.begin(), randomBotAccounts.end(), id) != randomBotAccounts.end();
}

bool PlayerbotAIConfig::IsInRandomQuestItemList(uint32 id)
{
    return find(randomBotQuestItems.begin(), randomBotQuestItems.end(), id) != randomBotQuestItems.end();
}

string PlayerbotAIConfig::GetValue(string name)
{
    ostringstream out;

    if (name == "GlobalCooldown")
        out << globalCoolDown;
    else if (name == "ReactDelay")
        out << reactDelay;

    else if (name == "SightDistance")
        out << sightDistance;
    else if (name == "SpellDistance")
        out << spellDistance;
    else if (name == "ReactDistance")
        out << reactDistance;
    else if (name == "GrindDistance")
        out << grindDistance;
    else if (name == "LootDistance")
        out << lootDistance;
    else if (name == "FleeDistance")
        out << fleeDistance;

    else if (name == "CriticalHealth")
        out << criticalHealth;
    else if (name == "LowHealth")
        out << lowHealth;
    else if (name == "MediumHealth")
        out << mediumHealth;
    else if (name == "AlmostFullHealth")
        out << almostFullHealth;
    else if (name == "LowMana")
        out << lowMana;

    else if (name == "IterationsPerTick")
        out << iterationsPerTick;

    return out.str();
}

void PlayerbotAIConfig::SetValue(string name, string value)
{
    istringstream out(value, istringstream::in);

    if (name == "GlobalCooldown")
        out >> globalCoolDown;
    else if (name == "ReactDelay")
        out >> reactDelay;

    else if (name == "SightDistance")
        out >> sightDistance;
    else if (name == "SpellDistance")
        out >> spellDistance;
    else if (name == "ReactDistance")
        out >> reactDistance;
    else if (name == "GrindDistance")
        out >> grindDistance;
    else if (name == "LootDistance")
        out >> lootDistance;
    else if (name == "FleeDistance")
        out >> fleeDistance;

    else if (name == "CriticalHealth")
        out >> criticalHealth;
    else if (name == "LowHealth")
        out >> lowHealth;
    else if (name == "MediumHealth")
        out >> mediumHealth;
    else if (name == "AlmostFullHealth")
        out >> almostFullHealth;
    else if (name == "LowMana")
        out >> lowMana;

    else if (name == "IterationsPerTick")
        out >> iterationsPerTick;
}


//默认值改为 0xFFFFFFFF
uint32 PlayerbotAIConfig::GetEventValue(uint32 bot, string event)
{
	uint32 value = 0xFFFFFFFF;

	QueryResult* results = CharacterDatabase.PQuery(
		"select `value`, `time`, validIn from ai_playerbot_random_bots where owner = 0 and bot = '%u' and event = '%s'",
		bot, event.c_str());

	if (results)
	{
		Field* fields = results->Fetch();
		value = fields[0].GetUInt32();
		uint32 lastChangeTime = fields[1].GetUInt32();
		uint32 validIn = fields[2].GetUInt32();
		if ((time(0) - lastChangeTime) >= validIn)
			value = 0xFFFFFFFF;
		delete results;
	}

	return value;
}

//validIn 看代码 是有效期的意思 
uint32 PlayerbotAIConfig::SetEventValue(uint32 bot, string event, uint32 value, uint32 validIn)
{
	CharacterDatabase.PExecute("delete from ai_playerbot_random_bots where owner = 0 and bot = '%u' and event = '%s'",
		bot, event.c_str());
	/*if (value)
	{*/
		CharacterDatabase.PExecute(
			"insert into ai_playerbot_random_bots (owner, bot, `time`, validIn, event, `value`) values ('%u', '%u', '%u', '%u', '%s', '%u')",
			0, bot, (uint32)time(0), validIn, event.c_str(), value);
	//}

	return value;
}


void PlayerbotAIConfig::CreateRandomBots()
{
    string randomBotAccountPrefix = config.GetStringDefault("AiPlayerbot.RandomBotAccountPrefix", "rndbot");
    //int32 randomBotAccountCount = config.GetIntDefault("AiPlayerbot.RandomBotAccountCount", 5);

    if (config.GetBoolDefault("AiPlayerbot.ResetRandomBotAccountsAtStart", false))
    {
        sLog.outBasic("Deleting random bot accounts...");
        QueryResult *results = LoginDatabase.PQuery("SELECT id FROM account where username like '%s%%'", randomBotAccountPrefix.c_str());
        if (results)
        {
            do
            {
                Field* fields = results->Fetch();
                sAccountMgr.DeleteAccount(fields[0].GetUInt32());
            } while (results->NextRow());

            delete results;
        }

		CharacterDatabase.Execute("DELETE FROM ai_playerbot_random_bots");
		//CharacterDatabase.Execute("UPDATE ai_playerbot_names SET in_use=0");
        sLog.outBasic("Random bot accounts deleted");
    }

	QueryResult *results = CharacterDatabase.PQuery("SELECT COUNT(name) FROM  ai_playerbot_names;");
	uint32 maxavailbotcount = 0;
	if (results) {
		maxavailbotcount = results->Fetch()[0].GetUInt32();
		delete results;
	}
	uint32 randomBotAccountCount = (maxavailbotcount / 9) + 1;
	sLog.outDetail("Random bot maxavailbotcount=%d randomBotAccountCount=%d", maxavailbotcount, randomBotAccountCount);

    for (int accountNumber = 0; accountNumber < randomBotAccountCount; ++accountNumber)
    {
        ostringstream out; out << randomBotAccountPrefix << accountNumber;
        string accountName = out.str();
        QueryResult *results = LoginDatabase.PQuery("SELECT id FROM account where username = '%s'", accountName.c_str());
        if (results)
        {
            delete results;
            continue;
        }

        /*string password = "";
        for (int i = 0; i < 10; i++)
        {
            password += (char)urand('!', 'z');
        }*/
		//密码改成可知的，这样就可以登陆随机机器人账户了
		string password = accountName;
        sAccountMgr.CreateAccount(accountName, password);

        sLog.outDetail("Account %s created for random bots", accountName.c_str());
    }

    LoginDatabase.PExecute("UPDATE account SET expansion = '%u', playerbot = %u where username like '%s%%'", 0,true, randomBotAccountPrefix.c_str());

    int totalRandomBotChars = 0;
    for (int accountNumber = 0; accountNumber < randomBotAccountCount; ++accountNumber)
    {
        ostringstream out; out << randomBotAccountPrefix << accountNumber;
        string accountName = out.str();

        QueryResult *results = LoginDatabase.PQuery("SELECT id FROM account where username = '%s'", accountName.c_str());
        if (!results)
            continue;

        Field* fields = results->Fetch();
        uint32 accountId = fields[0].GetUInt32();
        delete results;

        randomBotAccounts.push_back(accountId);

        int count = sAccountMgr.GetCharactersCount(accountId);
		int createmore = 9 - count;

		//因为 角色最多10个 所以，为了也能登陆机器人账号
		//有9个职业， 所以，这里检查 最导致第二次创建 生成18个，导致登陆获取角色列表失败。
        //if (count >= 10)
		/*if (count > 1)
        {
            totalRandomBotChars += count;
            continue;
        }*/

		if (createmore > 0) {
			RandomPlayerbotFactory factory(accountId);
			for (int i = 0; i < createmore; i++) {
				if (!factory.CreateRandomBot()) {
					sLog.outError("Fail to create random bot for account %d", accountId);
					break;
				}
			}
		}

        
       /* for (uint8 cls = CLASS_WARRIOR; cls < MAX_CLASSES; ++cls)
        {
            if (cls != 10 && cls != 6)
                factory.CreateRandomBot(cls);
        }*/

        totalRandomBotChars += sAccountMgr.GetCharactersCount(accountId);
    }

    sLog.outBasic("%d random bot accounts with %d characters available", randomBotAccounts.size(), totalRandomBotChars);
}
