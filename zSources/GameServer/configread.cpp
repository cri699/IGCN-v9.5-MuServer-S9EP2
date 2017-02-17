//////////////////////////////////////////////////////////////////////
// configread.cpp
#include "StdAfx.h"
#include "configread.h"
#include "TLog.h"
#include "MuRummyMng.h"
#include "JewelMixSystem.h"
#include "ChangeCmd.h"
#include "UpgradeCmd.h"

CConfigread g_ConfigRead;

CConfigread::CConfigread() : m_ItemCalcLua(true)
{

}

CConfigread::~CConfigread()
{

}

void SERVER_CONFIG::SetEventTable()
{
	this->bEventTable[SERVER_NORMAL][EV_TABLE_BC] = true;
	this->bEventTable[SERVER_NORMAL][EV_TABLE_DS] = true;
	this->bEventTable[SERVER_NORMAL][EV_TABLE_CC] = true;
	this->bEventTable[SERVER_NORMAL][EV_TABLE_IT] = false;
	this->bEventTable[SERVER_NORMAL][EV_TABLE_CS] = false;
	this->bEventTable[SERVER_NORMAL][EV_TABLE_DEEP] = false;
	this->bEventTable[SERVER_NORMAL][EV_TABLE_CW] = false;
	this->bEventTable[SERVER_NORMAL][EV_TABLE_KANTURU] = true;
	this->bEventTable[SERVER_NORMAL][EV_TABLE_RAKLION] = true;
	this->bEventTable[SERVER_NORMAL][EV_TABLE_DG] = true;
	this->bEventTable[SERVER_NORMAL][EV_TABLE_IF] = true;
	this->bEventTable[SERVER_NORMAL][EV_TABLE_RING] = true;
	this->bEventTable[SERVER_NORMAL][EV_TABLE_XMAS] = true;
	this->bEventTable[SERVER_NORMAL][EV_TABLE_ARCA] = false;
	this->bEventTable[SERVER_NORMAL][EV_TABLE_ACHERON] = false;
	this->bEventTable[SERVER_NORMAL][EV_TABLE_CCF] = false;
	this->bEventTable[SERVER_NORMAL][EV_TABLE_DSF] = false;
	this->bEventTable[SERVER_NORMAL][EV_TABLE_LMS] = true;

	this->bEventTable[SERVER_CASTLE][EV_TABLE_BC] = false;
	this->bEventTable[SERVER_CASTLE][EV_TABLE_DS] = false;
	this->bEventTable[SERVER_CASTLE][EV_TABLE_CC] = false;
	this->bEventTable[SERVER_CASTLE][EV_TABLE_IT] = false;
	this->bEventTable[SERVER_CASTLE][EV_TABLE_CS] = true;
	this->bEventTable[SERVER_CASTLE][EV_TABLE_DEEP] = true;
	this->bEventTable[SERVER_CASTLE][EV_TABLE_CW] = true;
	this->bEventTable[SERVER_CASTLE][EV_TABLE_KANTURU] = false;
	this->bEventTable[SERVER_CASTLE][EV_TABLE_RAKLION] = false;
	this->bEventTable[SERVER_CASTLE][EV_TABLE_DG] = false;
	this->bEventTable[SERVER_CASTLE][EV_TABLE_IF] = false;
	this->bEventTable[SERVER_CASTLE][EV_TABLE_RING] = false;
	this->bEventTable[SERVER_CASTLE][EV_TABLE_XMAS] = false;
	this->bEventTable[SERVER_CASTLE][EV_TABLE_ARCA] = false;
	this->bEventTable[SERVER_CASTLE][EV_TABLE_ACHERON] = false;
	this->bEventTable[SERVER_CASTLE][EV_TABLE_CCF] = false;
	this->bEventTable[SERVER_CASTLE][EV_TABLE_DSF] = false;
	this->bEventTable[SERVER_CASTLE][EV_TABLE_LMS] = false;

	this->bEventTable[SERVER_MARKET][EV_TABLE_BC] = false;
	this->bEventTable[SERVER_MARKET][EV_TABLE_DS] = false;
	this->bEventTable[SERVER_MARKET][EV_TABLE_CC] = false;
	this->bEventTable[SERVER_MARKET][EV_TABLE_IT] = true;
	this->bEventTable[SERVER_MARKET][EV_TABLE_CS] = false;
	this->bEventTable[SERVER_MARKET][EV_TABLE_DEEP] = false;
	this->bEventTable[SERVER_MARKET][EV_TABLE_CW] = false;
	this->bEventTable[SERVER_MARKET][EV_TABLE_KANTURU] = false;
	this->bEventTable[SERVER_MARKET][EV_TABLE_RAKLION] = false;
	this->bEventTable[SERVER_MARKET][EV_TABLE_DG] = false;
	this->bEventTable[SERVER_MARKET][EV_TABLE_IF] = false;
	this->bEventTable[SERVER_MARKET][EV_TABLE_RING] = false;
	this->bEventTable[SERVER_MARKET][EV_TABLE_XMAS] = false;
	this->bEventTable[SERVER_MARKET][EV_TABLE_ARCA] = false;
	this->bEventTable[SERVER_MARKET][EV_TABLE_ACHERON] = false;
	this->bEventTable[SERVER_MARKET][EV_TABLE_CCF] = false;
	this->bEventTable[SERVER_MARKET][EV_TABLE_DSF] = false;
	this->bEventTable[SERVER_MARKET][EV_TABLE_LMS] = false;

	this->bEventTable[SERVER_ARCA][EV_TABLE_BC] = false;
	this->bEventTable[SERVER_ARCA][EV_TABLE_DS] = false;
	this->bEventTable[SERVER_ARCA][EV_TABLE_CC] = false;
	this->bEventTable[SERVER_ARCA][EV_TABLE_IT] = false;
	this->bEventTable[SERVER_ARCA][EV_TABLE_CS] = false;
	this->bEventTable[SERVER_ARCA][EV_TABLE_DEEP] = false;
	this->bEventTable[SERVER_ARCA][EV_TABLE_CW] = false;
	this->bEventTable[SERVER_ARCA][EV_TABLE_KANTURU] = false;
	this->bEventTable[SERVER_ARCA][EV_TABLE_RAKLION] = false;
	this->bEventTable[SERVER_ARCA][EV_TABLE_DG] = false;
	this->bEventTable[SERVER_ARCA][EV_TABLE_IF] = false;
	this->bEventTable[SERVER_ARCA][EV_TABLE_RING] = false;
	this->bEventTable[SERVER_ARCA][EV_TABLE_XMAS] = false;
	this->bEventTable[SERVER_ARCA][EV_TABLE_ARCA] = true;
	this->bEventTable[SERVER_ARCA][EV_TABLE_ACHERON] = true;
	this->bEventTable[SERVER_ARCA][EV_TABLE_CCF] = false;
	this->bEventTable[SERVER_ARCA][EV_TABLE_DSF] = false;
	this->bEventTable[SERVER_ARCA][EV_TABLE_LMS] = false;

	this->bEventTable[SERVER_BATTLECORE][EV_TABLE_BC] = false;
	this->bEventTable[SERVER_BATTLECORE][EV_TABLE_DS] = false;
	this->bEventTable[SERVER_BATTLECORE][EV_TABLE_CC] = true;
	this->bEventTable[SERVER_BATTLECORE][EV_TABLE_IT] = false;
	this->bEventTable[SERVER_BATTLECORE][EV_TABLE_CS] = false;
	this->bEventTable[SERVER_BATTLECORE][EV_TABLE_DEEP] = false;
	this->bEventTable[SERVER_BATTLECORE][EV_TABLE_CW] = false;
	this->bEventTable[SERVER_BATTLECORE][EV_TABLE_KANTURU] = false;
	this->bEventTable[SERVER_BATTLECORE][EV_TABLE_RAKLION] = false;
	this->bEventTable[SERVER_BATTLECORE][EV_TABLE_DG] = false;
	this->bEventTable[SERVER_BATTLECORE][EV_TABLE_IF] = false;
	this->bEventTable[SERVER_BATTLECORE][EV_TABLE_RING] = false;
	this->bEventTable[SERVER_BATTLECORE][EV_TABLE_XMAS] = false;
	this->bEventTable[SERVER_BATTLECORE][EV_TABLE_ARCA] = false;
	this->bEventTable[SERVER_BATTLECORE][EV_TABLE_CCF] = true;
	this->bEventTable[SERVER_BATTLECORE][EV_TABLE_DSF] = true;
	this->bEventTable[SERVER_BATTLECORE][EV_TABLE_LMS] = false;
}

void SERVER_CONFIG::ReadServerInfo(bool bObjInit)
{
	

	CIniReader ReadServerInfo(".\\GameServer.ini");

	strncpy(this->szServerName, ReadServerInfo.ReadString("GameServerInfo", "ServerName", "").c_str(), sizeof(this->szServerName));

	this->bIsTestServer			= ReadServerInfo.ReadInt("GameServerInfo", "IsTestServer", 0);
	this->iGameServerCode		= ReadServerInfo.ReadInt("GameServerInfo", "ServerCode", 0);
	this->bIsConnectMemberUse	= ReadServerInfo.ReadInt("GameServerInfo", "ConnectMemberLoad", 0);
	this->bIsNonPvP				= ReadServerInfo.ReadInt("GameServerInfo", "NonPK", 0);

	if (bObjInit == true)
	{
		this->iObjectMaxMonster = ReadServerInfo.ReadInt("GameServerInfo", "MonsterCount", 11500);
		this->iObjectMaxUser = ReadServerInfo.ReadInt("GameServerInfo", "PlayerCount", 50);
		this->iObjectMaxItem = ReadServerInfo.ReadInt("GameServerInfo", "MapItemCount", 300);
		this->iObjectMaxCallMonster = ReadServerInfo.ReadInt("GameServerInfo", "SummonMonsterCount", 500);

		this->iObjectMax = this->iObjectMaxMonster + this->iObjectMaxUser + this->iObjectMaxCallMonster;
		this->iObjectStartUserIndex = this->iObjectMax - this->iObjectMaxUser;

		this->iServerType = ReadServerInfo.ReadInt("GameServerInfo", "ServerType", SERVER_NORMAL);

		if (this->iServerType < SERVER_NORMAL || this->iServerType > SERVER_BATTLECORE)
		{
			g_Log.MsgBox("Error - wrong ServerType. Please set correct value and run GameServer again.");
			ExitProcess(0);
		}
	}

	strncpy(this->szJoinServerIP, ReadServerInfo.ReadString("GameServerInfo", "JoinServerIP", "127.0.0.1").c_str(), sizeof(this->szJoinServerIP));
	strncpy(this->szDataServerIP, ReadServerInfo.ReadString("GameServerInfo", "DataServerIP", "127.0.0.1").c_str(), sizeof(this->szDataServerIP));
	strncpy(this->szExDBIP, ReadServerInfo.ReadString("GameServerInfo", "ExDBIP", "127.0.0.1").c_str(), sizeof(this->szExDBIP));
	strncpy(this->szConnectServerIP, ReadServerInfo.ReadString("GameServerInfo", "ConnectServerIP", "").c_str(), sizeof(this->szConnectServerIP));

	this->iJoinServerPort = ReadServerInfo.ReadInt("GameServerInfo", "JoinServerPort", 55970);
	this->iDataServerPort = ReadServerInfo.ReadInt("GameServerInfo", "DataServerPort", 55960);
	this->iExDBPort = ReadServerInfo.ReadInt("GameServerInfo", "ExDBPort", 55906);
	this->iGameServerPort = ReadServerInfo.ReadInt("GameServerInfo", "GameServerPort", 55901);
	this->iConnectServerPort = ReadServerInfo.ReadInt("GameServerInfo", "ConnectServerPort", 55557);

	this->bLogCreate = ReadServerInfo.ReadInt("GameServerInfo", "LogCreate", 1);
	this->iLogFileMaxSizeMB = ReadServerInfo.ReadInt("GameServerInfo", "LogFileMaxSizeInMB", 256);

	this->iHWIDUseLimit = ReadServerInfo.ReadInt("GameServerInfo", "MachineIDConnectionLimitCount", 3);

	strncpy(this->szCashShopLogPath, ReadServerInfo.ReadString("GameServerInfo", "ItemShopLogPath", ".\\ITEM_SHOP_LOG").c_str(), sizeof(this->szCashShopLogPath));
	strncpy(this->szUserSaveLogPath, ReadServerInfo.ReadString("GameServerInfo", "CharSaveLogPath", ".\\CHAR_SAVE_LOG").c_str(), sizeof(this->szUserSaveLogPath));
	strncpy(this->szCommandLogPath, ReadServerInfo.ReadString("GameServerInfo", "CommandLogPath", ".\\COMMANDS_SAVE_LOG").c_str(), sizeof(this->szCommandLogPath));
	strncpy(this->szServerLogPath, ReadServerInfo.ReadString("GameServerInfo", "ServerStateLogPath", ".\\SERVER_CONN_STATE_LOG").c_str(), sizeof(this->szServerLogPath));
	strncpy(this->szMsgLogPath, ReadServerInfo.ReadString("GameServerInfo", "MessageLogPath", ".\\MESSAGE_LOG").c_str(), sizeof(this->szMsgLogPath));
	strncpy(this->szTradeLogPath, ReadServerInfo.ReadString("GameServerInfo", "TradeLogPath", ".\\TRADE_LOG").c_str(), sizeof(this->szTradeLogPath));
	strncpy(this->szGMLogPath, ReadServerInfo.ReadString("GameServerInfo", "GMLogPath", ".\\GM_LOG").c_str(), sizeof(this->szGMLogPath));
	strncpy(this->szAntiHackLogPath, ReadServerInfo.ReadString("GameServerInfo", "AntiHackLogPath", ".\\ANTI_HACK").c_str(), sizeof(this->szAntiHackLogPath));
	strncpy(this->szBotShopLogPath, ReadServerInfo.ReadString("GameServerInfo", "BotShopLog", ".\\BOT_SHOP_LOG").c_str(), sizeof(this->szBotShopLogPath));
	strncpy(this->szSerialCheckLogPath, ReadServerInfo.ReadString("GameServerInfo", "SerialCheckLogPath", ".\\SERIAL_CHECK_LOG").c_str(), sizeof(this->szSerialCheckLogPath));

	
}

void CConfigread::ReadConfig()
{
	CIniReader ReadCommon(this->GetPath("IGC_Common.ini"));
	CIniReader ReadChaosBox(this->GetPath("IGC_ChaosBox.ini"));
	CIniReader ReadItemPrices(this->GetPath("IGC_PriceSettings.ini"));

	this->data.common.IsJoinMu = ReadCommon.ReadInt("joinmu", "IsJoinMu", 0);

	this->m_ItemCalcLua.DoFile(this->GetPath("\\Scripts\\Misc\\ItemCalc.lua"));

	this->SeasonX = ReadCommon.ReadInt("General", "SeasonXProtocol", 0);

	this->data.common.IsGuardSay = ReadCommon.ReadInt("General", "GuardSpeak", 1);
	this->data.common.GuardSayText = ReadCommon.ReadString("General", "GuardSpeakMsg", "Hello from mars");
	this->data.common.GuardTalkChance = ReadCommon.ReadInt("General", "GuardSpeakChance", 30);

	if (this->data.common.GuardSayText.length() > 60)
	{
		g_Log.MsgBox("Error -  Guard Say text is too long");
	}

	this->data.common.WelcomeMessage = ReadCommon.ReadString("General", "WelcomeMessage", "Welcome to our Mu Online server!");
	this->data.common.EnterGameMessage = ReadCommon.ReadInt("General", "EnterGameMessageEnable", 1);

	this->data.common.AddZen = ReadCommon.ReadFloat("General", "ZenDropMultipler", 1.0);
	this->data.common.MLPointPerLevel = ReadCommon.ReadInt("General", "MasterPointPerLevel", 1);
	this->data.common.MLMonsterMinLevel = ReadCommon.ReadInt("General", "MonsterMinLevelForMLExp", 95);
	this->data.common.UserMaxLevel = ReadCommon.ReadInt("General", "MaxNormalLevel", 400);
	this->data.common.MLUserMaxLevel = ReadCommon.ReadInt("General", "MaxMasterLevel", 200);

	this->data.common.TradeExcItemLimit = ReadCommon.ReadInt("General", "CanTradeFullExcItem", 1);
	this->data.common.StoreExcItemLimit = ReadCommon.ReadInt("General", "CanSellInStoreFullExcItem", 1);
	this->data.common.ShopExcItemLimit = ReadCommon.ReadInt("General", "CanSellToShopFullExcItem", 1);
	this->data.common.TradeHarmonyBlock = ReadCommon.ReadInt("General", "CanTradeHarmonyItem", 1);

	this->antihack.EnabledAgiCheck = ReadCommon.ReadInt("AntiHack", "EnableAgilityCheck", 0);
	this->antihack.MainCRC = ReadCommon.ReadDword("AntiHack", "MainExeCRC", 0);
	this->antihack.DLLCRC = ReadCommon.ReadDword("AntiHack", "DLLCRC", 0);
	this->antihack.PlayerCRC = ReadCommon.ReadDword("AntiHack", "PlayerBmdCRC", 0);
	this->antihack.SkillCRC = ReadCommon.ReadDword("AntiHack", "SkillCRC", 0);
	this->antihack.isInvalidDLLVersionDC = ReadCommon.ReadInt("AntiHack", "DisconnectOnInvalidDLLVersion", 0);
	this->antihack.EnablePacketTimeCheck = ReadCommon.ReadInt("AntiHack", "EnablePacketTimeCheck", 1);
	this->antihack.EnableHackDetectMessage = ReadCommon.ReadInt("AntiHack", "EnableHackDetectMessage", 1);
	this->antihack.PacketTimeMinTime = ReadCommon.ReadInt("AntiHack", "PacketTimeMinTimeMsec", 19000);
	this->antihack.EnableAttackBlockInSafeZone = ReadCommon.ReadInt("AntiHack", "EnableAttackBlockInSafeZone", 0);
	this->antihack.HackDetectMessage = ReadCommon.ReadString("AntiHack", "HackDetectMessage", "WYPIERDALAJ");
	this->antihack.EnableAutoBanUserAccount = ReadCommon.ReadInt("AntiHack", "EnableAutoBanAccountForHackUser", 0);
	this->antihack.EnabledAntiRefTimeCheck = ReadCommon.ReadInt("AntiHack", "EnableAntiRefCheckTime", 0);
	this->antihack.AntiRefCheckTime = ReadCommon.ReadInt("AntiHack", "AntiRefCheckTimeMSEC", 500);
	this->antihack.AgilityDelayCheckTime = ReadCommon.ReadInt("AntiHack", "AgilityDelayCheckTime", 5000);
	this->antihack.AntiHackBreachDisconnectUser = ReadCommon.ReadInt("AntiHack", "AntiHackBreachDisconnectUser", 1);
	this->antihack.AntiHackRecvHookProtect = ReadCommon.ReadInt("AntiHack", "RecvHookProtection", 1);
	this->antihack.EnabledSkillCheckTime = ReadCommon.ReadInt("SkillTimeCheck", "IsEnabled", 0);
	this->antihack.EnabledSkillCheckDC = ReadCommon.ReadInt("SkillTimeCheck", "IsDisconnectUser", 0);
	this->antihack.SkillCheckWrongCastsToDC = ReadCommon.ReadInt("SkillTimeCheck", "NumberOfCastsToDisconnect", 10);
	this->antihack.EnabledSkillCheckDebugMode = ReadCommon.ReadInt("SkillTimeCheck", "DebugMode", 0);

	this->data.reset.enable = ReadCommon.ReadInt("ResetCMD", "Enable", 1);
	this->data.reset.iCost = ReadCommon.ReadInt("ResetCMD", "Cost", 4000000);
	this->data.reset.iMinLevel = ReadCommon.ReadInt("ResetCMD", "MinLevel", 400);
	this->data.reset.iResetLimit = ReadCommon.ReadInt("ResetCMD", "MaxReset", 100);
	this->data.reset.ResetML = ReadCommon.ReadInt("ResetCMD", "IsResetMasterLevel", 0);
	this->data.reset.ResetStats = ReadCommon.ReadInt("ResetCMD", "IsResetStats", 0);
	this->data.reset.NormalPointPerReset = ReadCommon.ReadInt("ResetCMD", "PointPerReset", 200);
	this->data.reset.SaveOldPoint = ReadCommon.ReadInt("ResetCMD", "SaveOldStatPoint", 0);
	this->data.reset.RemoveEquipment = ReadCommon.ReadInt("ResetCMD", "RemoveEquipment", 0);
	this->data.reset.MoveToCharacterSelect = ReadCommon.ReadInt("ResetCMD", "MoveToCharSelectWindow", 0);
	this->data.reset.iBlockLevelUpPointAfterResets = ReadCommon.ReadInt("ResetCMD", "BlockLevelUpPointAfterResetCount", 0);
	this->data.reset.iBlockMLPointAfterResets = ReadCommon.ReadInt("ResetCMD", "BlockMLPointAfterResetCount", 0);

	this->data.post.bPostEnable = ReadCommon.ReadInt("PostCMD", "Enable", 1);
	this->data.post.iPostPrice = ReadCommon.ReadInt("PostCMD", "Cost", 100000);
	this->data.post.wPostLevel = ReadCommon.ReadInt("PostCMD", "MinLevel", 50);
	this->data.post.btPostColorRGB[0] = ReadCommon.ReadInt("PostCMD", "ColorRed", 0x8C);
	this->data.post.btPostColorRGB[1] = ReadCommon.ReadInt("PostCMD", "ColorGreen", 0xBE);
	this->data.post.btPostColorRGB[2] = ReadCommon.ReadInt("PostCMD", "ColorBlue", 0xC3);
	this->data.post.dwPostCooldown = ReadCommon.ReadInt("PostCMD", "CoolDown", 5);

	this->data.post.bGPostEnable = ReadCommon.ReadInt("GlobalPostCMD", "Enable", 1);
	this->data.post.iGPostPrice = ReadCommon.ReadInt("GlobalPostCMD", "Cost", 500000);
	this->data.post.wGPostLevel = ReadCommon.ReadInt("GlobalPostCMD", "MinLevel", 200);
	this->data.post.btGPostColorRGB[0] = ReadCommon.ReadInt("GlobalPostCMD", "ColorRed", 0x0D);
	this->data.post.btGPostColorRGB[1] = ReadCommon.ReadInt("GlobalPostCMD", "ColorGreen", 0xD0);
	this->data.post.btGPostColorRGB[2] = ReadCommon.ReadInt("GlobalPostCMD", "ColorBlue", 0xE3);
	this->data.post.dwGPostCooldown = ReadCommon.ReadInt("GlobalPostCMD", "CoolDown", 10);

	this->data.mubot.enable = ReadCommon.ReadInt("MuBot","Enable",1);
	this->data.mubot.minlevel = ReadCommon.ReadInt("MuBot", "MinLevel", 80);
	this->data.mubot.cost = ReadCommon.ReadInt("MuBot", "Cost", 20);
	this->data.mubot.viplevel = ReadCommon.ReadInt("MuBot", "NeedVIPLevel", 0);
	this->data.mubot.autodisabletime = ReadCommon.ReadInt("MuBot", "AutoDisableTime", 0);

	g_Log.Add("Official MuBot: Enabled: %s, Minimum Level: %d, Auto Disable Time: %d minutes", this->data.mubot.enable == true ? "Yes" : "No", this->data.mubot.minlevel, this->data.mubot.autodisabletime);

	this->data.chatcolor.btInfoMsg[0] = ReadCommon.ReadInt("ChatColors", "InfoMessageRed", 0x64);
	this->data.chatcolor.btInfoMsg[1] = ReadCommon.ReadInt("ChatColors", "InfoMessageGreen", 0x96);
	this->data.chatcolor.btInfoMsg[2] = ReadCommon.ReadInt("ChatColors", "InfoMessageBlue", 0xFF);

	this->data.chatcolor.btErrorMsg[0] = ReadCommon.ReadInt("ChatColors", "ErrorMessageRed", 0xFF);
	this->data.chatcolor.btErrorMsg[1] = ReadCommon.ReadInt("ChatColors", "ErrorMessageGreen", 0x1E);
	this->data.chatcolor.btErrorMsg[2] = ReadCommon.ReadInt("ChatColors", "ErrorMessageBlue", 0x00);

	this->data.chatcolor.btChatMsg[0] = ReadCommon.ReadInt("ChatColors", "ChatMessageRed", 0xCD);
	this->data.chatcolor.btChatMsg[1] = ReadCommon.ReadInt("ChatColors", "ChatMessageGreen", 0xDC);
	this->data.chatcolor.btChatMsg[2] = ReadCommon.ReadInt("ChatColors", "ChatMessageBlue", 0xEF);

	this->data.chatcolor.btWhisperMsg[0] = ReadCommon.ReadInt("ChatColors", "WhisperMessageRed", 0xFF);
	this->data.chatcolor.btWhisperMsg[1] = ReadCommon.ReadInt("ChatColors", "WhisperMessageGreen", 0xB7);
	this->data.chatcolor.btWhisperMsg[2] = ReadCommon.ReadInt("ChatColors", "WhisperMessageBlue", 0xDB);

	this->data.chatcolor.btPartyMsg[0] = ReadCommon.ReadInt("ChatColors", "PartyMessageRed", 0x66);
	this->data.chatcolor.btPartyMsg[1] = ReadCommon.ReadInt("ChatColors", "PartyMessageGreen", 0xCC);
	this->data.chatcolor.btPartyMsg[2] = ReadCommon.ReadInt("ChatColors", "PartyMessageBlue", 0xFF);

	this->data.chatcolor.btGuildMsg[0] = ReadCommon.ReadInt("ChatColors", "GuildMessageRed", 0xFF);
	this->data.chatcolor.btGuildMsg[1] = ReadCommon.ReadInt("ChatColors", "GuildMessageGreen", 0xCC);
	this->data.chatcolor.btGuildMsg[2] = ReadCommon.ReadInt("ChatColors", "GuildMessageBlue", 0x99);

	this->data.chatcolor.btAllianceMsg[0] = ReadCommon.ReadInt("ChatColors", "AllianceMessageRed", 0xC8);
	this->data.chatcolor.btAllianceMsg[1] = ReadCommon.ReadInt("ChatColors", "AllianceMessageGreen", 0xC8);
	this->data.chatcolor.btAllianceMsg[2] = ReadCommon.ReadInt("ChatColors", "AllianceMessageBlue", 0x00);

	this->data.chatcolor.btGensMsg[0] = ReadCommon.ReadInt("ChatColors", "GensMessageRed", 0xAF);
	this->data.chatcolor.btGensMsg[1] = ReadCommon.ReadInt("ChatColors", "GensMessageGreen", 0xF4);
	this->data.chatcolor.btGensMsg[2] = ReadCommon.ReadInt("ChatColors", "GensMessageBlue", 0x7C);

	this->data.chatcolor.btGMChatMsg[0] = ReadCommon.ReadInt("ChatColors", "GMChatMessageRed", 0xFA);
	this->data.chatcolor.btGMChatMsg[1] = ReadCommon.ReadInt("ChatColors", "GMChatMessageGreen", 0xC8);
	this->data.chatcolor.btGMChatMsg[2] = ReadCommon.ReadInt("ChatColors", "GMChatMessageBlue", 0x3A);

	this->data.common.SocketExcLimit = ReadCommon.ReadInt("General", "CanUseSocketExcItem", 1); // 0 - disable, 1 - enabled without drop, 2 - enabled with drop from monsters
	this->data.common.AncHarmonyLimit = ReadCommon.ReadInt("General", "CanUseAncHarmonyItem", 1);

	this->data.common.AutoPartySet = ReadCommon.ReadInt("General", "AutoParty", 0);
	this->data.common.bReconnectSystem = ReadCommon.ReadInt("General","ReconnectSystem", 0);
	this->data.common.ThirdQuestCounter = ReadCommon.ReadInt("General", "ThirdQuestMonsterCountMsg", 0);
	this->data.common.BlockTradeFFFFFFFFItems = ReadCommon.ReadInt("General", "CanTradeFFFFFFFFSerialItem", 1);
	this->data.common.PKEnterEventAllow = ReadCommon.ReadInt("General", "CanEnterEventWithPK", 0);
	this->data.common.Item380DropMap = ReadCommon.ReadInt("General", "DropMap380Items", -1);
	this->data.common.SoulLuckRate = ReadCommon.ReadInt("General", "UseSoulWithLuckRate", 75);
	this->data.common.SoulNoLuckRate = ReadCommon.ReadInt("General", "UseSoulRate", 50);
	this->data.common.LifeRate = ReadCommon.ReadInt("General", "UseLifeRate", 50);
	this->data.common.Is28Opt = ReadCommon.ReadInt("General", "Is28Option", 1);

	this->data.common.SavePrivateChat = ReadCommon.ReadInt("General", "SavePrivateChat", 0);

	this->data.common.TradeBlock = ReadCommon.ReadInt("General", "TradeBlock", 0);
	this->data.common.EnableSellAllItems = ReadCommon.ReadInt("General", "EnableSellAllItems", 1);
	this->data.common.PotionTime = ReadCommon.ReadInt("AntiHack", "PotionDelayTime", 100);
	this->data.common.PacketLimit = ReadCommon.ReadInt("AntiHack", "PacketLimit", 50);

	this->data.common.SpiritMapDropRate = ReadCommon.ReadInt("Acheron", "SpiritMapDropRate", 100);
	this->data.common.SpiritMapDropLevel = ReadCommon.ReadInt("Acheron", "SpiritMapMonsterDropLevel", 32);

	this->data.common.AllowGetLuckyItemGround = ReadCommon.ReadInt("General", "AllowToGetLuckyItemFromGround", 0);
	this->data.common.LuckyItemDurabilityTime = ReadCommon.ReadInt("General", "LuckyItemDurabilityTime", 2400);
	this->data.common.LuckyItemAllowUseJewels = ReadCommon.ReadInt("General", "AllowEnchantLuckyItemByJewels", 0);

	this->data.common.GoldenDragonBoxCount = ReadCommon.ReadInt("GoldenMonster", "GoldenDragonBoxDropCount", 5);
	this->data.common.GreatGoldenDragonBoxCount = ReadCommon.ReadInt("GoldenMonster", "GreatGoldenDragonBoxCount", 5);

	this->data.common.CheckValidItem = ReadCommon.ReadInt("General", "CheckValidItem", 1);

	this->data.common.XMasSantaMinReset = ReadCommon.ReadInt("SantaVillage", "SantaClauseMinReset", 0);
	this->data.common.XMasSantaFirstPrizeMaxCount = ReadCommon.ReadInt("SantaVillage", "SantaClause1stPrizeMaxVisit", 1);
	this->data.common.XMasSantaSecondPrizeMaxCount = ReadCommon.ReadInt("SantaVillage", "SantaClause2ndPrizeMaxVisit", 3);

	this->mix.Wing3MaxRate = ReadChaosBox.ReadInt("Wings", "Wing3MixMaxRate", 100);
	this->mix.FeatherCondorMaxRate = ReadChaosBox.ReadInt("Wings", "FeatherOfCondorMaxRate", 100);
	this->mix.Wing2MaxRate = ReadChaosBox.ReadInt("Wings", "Wing2MixMaxRate", 100);
	this->mix.Wing25MaxRate = ReadChaosBox.ReadInt("Wings", "Wing25MixMaxRate", 60);
	this->mix.CapeLordMaxRate = ReadChaosBox.ReadInt("Wings", "CapeLordMixMaxRate", 100);
	this->mix.SocketWeaponMaxRate = ReadChaosBox.ReadInt("SocketWeapon", "SocketWeaponMixMaxRate", 40);
	this->mix.SocketWeaponRequireMoney = ReadChaosBox.ReadInt("SocketWeapon", "SocketWeaponMixRequireMoney", 1000000);

	this->mix.PlusItem10Rate[0] = ReadChaosBox.ReadInt("PlusItemLevel", "Mix10Rate_Normal", 60);
	this->mix.PlusItem11Rate[0] = ReadChaosBox.ReadInt("PlusItemLevel", "Mix11Rate_Normal", 60);
	this->mix.PlusItem12Rate[0] = ReadChaosBox.ReadInt("PlusItemLevel", "Mix12Rate_Normal", 60);
	this->mix.PlusItem13Rate[0] = ReadChaosBox.ReadInt("PlusItemLevel", "Mix13Rate_Normal", 55);
	this->mix.PlusItem14Rate[0] = ReadChaosBox.ReadInt("PlusItemLevel", "Mix14Rate_Normal", 55);
	this->mix.PlusItem15Rate[0] = ReadChaosBox.ReadInt("PlusItemLevel", "Mix15Rate_Normal", 50);
	this->mix.PlusItemAddLuckRate[0] = ReadChaosBox.ReadInt("PlusItemLevel", "MixAddLuckRate_Normal", 25);
	this->mix.PlusItemMix15ResultNotice[0] = ReadChaosBox.ReadInt("Common", "PlusItemMix15ResultNotice_Normal", 1);

	this->mix.PlusItem10Rate[1] = ReadChaosBox.ReadInt("PlusItemLevel", "Mix10Rate_Enhanced", 50);
	this->mix.PlusItem11Rate[1] = ReadChaosBox.ReadInt("PlusItemLevel", "Mix11Rate_Enhanced", 50);
	this->mix.PlusItem12Rate[1] = ReadChaosBox.ReadInt("PlusItemLevel", "Mix12Rate_Enhanced", 50);
	this->mix.PlusItem13Rate[1] = ReadChaosBox.ReadInt("PlusItemLevel", "Mix13Rate_Enhanced", 45);
	this->mix.PlusItem14Rate[1] = ReadChaosBox.ReadInt("PlusItemLevel", "Mix14Rate_Enhanced", 45);
	this->mix.PlusItem15Rate[1] = ReadChaosBox.ReadInt("PlusItemLevel", "Mix15Rate_Enhanced", 40);
	this->mix.PlusItemAddLuckRate[1] = ReadChaosBox.ReadInt("PlusItemLevel", "MixAddLuckRate_Enhanced", 25);
	this->mix.PlusItemMix15ResultNotice[1] = ReadChaosBox.ReadInt("Common", "PlusItemMix15ResultNotice_Enhanced", 1);

	this->mix.PlusItem10Rate[2] = ReadChaosBox.ReadInt("PlusItemLevel", "Mix10Rate_Socket", 40);
	this->mix.PlusItem11Rate[2] = ReadChaosBox.ReadInt("PlusItemLevel", "Mix11Rate_Socket", 40);
	this->mix.PlusItem12Rate[2] = ReadChaosBox.ReadInt("PlusItemLevel", "Mix12Rate_Socket", 40);
	this->mix.PlusItem13Rate[2] = ReadChaosBox.ReadInt("PlusItemLevel", "Mix13Rate_Socket", 35);
	this->mix.PlusItem14Rate[2] = ReadChaosBox.ReadInt("PlusItemLevel", "Mix14Rate_Socket", 35);
	this->mix.PlusItem15Rate[2] = ReadChaosBox.ReadInt("PlusItemLevel", "Mix15Rate_Socket", 30);
	this->mix.PlusItemAddLuckRate[2] = ReadChaosBox.ReadInt("PlusItemLevel", "MixAddLuckRate_Socket", 25);
	this->mix.PlusItemMix15ResultNotice[2] = ReadChaosBox.ReadInt("Common", "PlusItemMix15ResultNotice_Socket", 1);

	this->mix.PlusItem10Rate[3] = ReadChaosBox.ReadInt("PlusItemLevel", "Mix10Rate_Pentagram", 40);
	this->mix.PlusItem11Rate[3] = ReadChaosBox.ReadInt("PlusItemLevel", "Mix11Rate_Pentagram", 40);
	this->mix.PlusItem12Rate[3] = ReadChaosBox.ReadInt("PlusItemLevel", "Mix12Rate_Pentagram", 40);
	this->mix.PlusItem13Rate[3] = ReadChaosBox.ReadInt("PlusItemLevel", "Mix13Rate_Pentagram", 35);
	this->mix.PlusItem14Rate[3] = ReadChaosBox.ReadInt("PlusItemLevel", "Mix14Rate_Pentagram", 35);
	this->mix.PlusItem15Rate[3] = ReadChaosBox.ReadInt("PlusItemLevel", "Mix15Rate_Pentagram", 30);
	this->mix.PlusItemAddLuckRate[3] = ReadChaosBox.ReadInt("PlusItemLevel", "MixAddLuckRate_Pentagram", 25);
	this->mix.PlusItemMix15ResultNotice[3] = ReadChaosBox.ReadInt("Common", "PlusItemMix15ResultNotice_Pentagram", 1);

	this->mix.CherryBlossomWhiteNeedCount = ReadChaosBox.ReadInt("CherryBlossom", "CherryBlossomWhiteNeedItem", 10);
	this->mix.CherryBlossomRedNeedCount = ReadChaosBox.ReadInt("CherryBlossom", "CherryBlossomRedNeedItem", 30);
	this->mix.CherryBlossomGoldNeedCount = ReadChaosBox.ReadInt("CherryBlossom", "CherryBlossomGoldNeedItem", 255);

	this->data.common.GuildCreate = ReadCommon.ReadInt("Guilds", "GuildCreate", 1);
	this->data.common.GuildDestroy = ReadCommon.ReadInt("Guilds", "GuildDestroy", 1);
	this->data.common.GuildCreateLevel = ReadCommon.ReadInt("Guilds", "GuildCreateLevel", 150);
	this->data.common.CastleGuildDestroyLimit = ReadCommon.ReadInt("Guilds", "CastleOwnerGuildDestroyLimit", 1);
	this->data.common.AllianceMinGuildMember = ReadCommon.ReadInt("Guilds", "AllianceMinGuildMember", 20);
	this->data.common.AllianceMaxGuilds = ReadCommon.ReadInt("Guilds", "AllianceMaxGuilds", 5);
	this->data.common.MaxGuildMember = ReadCommon.ReadInt("Guilds", "MaxGuildMember", 80);

	// PRIVATE CONFIG, DO NOT ADD IT IN PUBLIC RELEASE
	this->data.common.FreeWarehouse = ReadCommon.ReadInt("joinmu", "FreeWarehouse", 0);
	this->data.common.DisableMSBEffect[MSB_DISABLE_MONSTER] = ReadCommon.ReadInt("joinmu", "DisableMSBEffect_Monster", 0);
	this->data.common.DisableMSBEffect[MSB_DISABLE_CHARACTER] = ReadCommon.ReadInt("joinmu", "DisableMSBEffect_Character", 0);
	this->data.common.DisableMSBEffect[MSB_DISABLE_CHARACTER_FENRIR] = ReadCommon.ReadInt("joinmu", "DisableMSBEffect_Character_Fenrir", 0);

	this->data.common.EventInventoryOpen = ReadCommon.ReadInt("EventInventory", "IsEventInventoryOpen", 0);
	this->data.common.EventEndDate.wYear = ReadCommon.ReadInt("EventInventory", "EventEndYear", 0);
	this->data.common.EventEndDate.wMonth = ReadCommon.ReadInt("EventInventory", "EventEndMonth", 0);
	this->data.common.EventEndDate.wDay = ReadCommon.ReadInt("EventInventory", "EventEndDay", 0);
	this->data.common.EventEndDate.wHour = ReadCommon.ReadInt("EventInventory", "EventEndHour", 0);
	this->data.common.EventEndDate.wMinute = ReadCommon.ReadInt("EventInventory", "EventEndMinute", 0);

	g_CMuRummyMng.LoadPreFixData(this->GetPath("IGC_MuRummy.ini"));
	
	this->data.common.PetEventBookMoonRabbitSpawnRate = ReadCommon.ReadInt("EggEvent", "MoonRabbitSpawnRateFromBook", 3000);
	this->data.common.PetEventBookLuckyBagSpawnRate = ReadCommon.ReadInt("EggEvent", "PouchBlessingSpawnRateFromBook", 3000);
	this->data.common.PetEventBookFireFlameSpawnRate = ReadCommon.ReadInt("EggEvent", "FireFlameSpawnRateFromBook", 3000);
	this->data.common.PetEventBookGoldGoblinSpawnRate = ReadCommon.ReadInt("EggEvent", "GoldGoblinSpawnRateFromBook", 1000);

	this->data.common.CancelItemSaleEnable = ReadCommon.ReadInt("CancelItemSale", "IsCancelItemSale", 0);
	this->data.common.CancelItemSaleMultipler = ReadCommon.ReadFloat("CancelItemSale", "PriceMultipler", 110.0);
	this->data.common.CancelItemSaleExpiryTime = ReadCommon.ReadInt("CancelItemSale", "ItemExpiryTime", 86400);

	this->data.common.BossKillMessage = ReadCommon.ReadInt("General", "BossMonsterKillNotice", 0);
	
	this->data.common.bHitHackDetection = ReadCommon.ReadInt("AntiHack","EnableHitHackDetection",0);
	this->data.common.iHitHackMaxAgility = ReadCommon.ReadInt("AntiHack", "HitHackMaxAgility", 23000);
	
	this->ReadPetSettings();
	this->ReadOffTradeConfig();
	this->ReadCalcCharacter();
	this->ReadPKSettings();

	this->itemPrices.btItemSellPriceDivisor = ReadItemPrices.ReadInt("Value", "ItemSellPriceDivisor", 3);
	this->itemPrices.iJewelOfBlessPrice = ReadItemPrices.ReadInt("Value", "JewelOfBlessPrice", 9000000);
	this->itemPrices.iJewelOfSoulPrice = ReadItemPrices.ReadInt("Value", "JewelOfSoulPrice", 6000000);
	this->itemPrices.iJewelOfChaosPrice = ReadItemPrices.ReadInt("Value", "JewelOfChaosPrice", 810000);
	this->itemPrices.iJewelOfLifePrice = ReadItemPrices.ReadInt("Value", "JewelOfLifePrice", 45000000);
	this->itemPrices.iJewelOfCreationPrice = ReadItemPrices.ReadInt("Value", "JewelOfCreationPrice", 32000000);
	this->itemPrices.iCrestOfMonarchPrice = ReadItemPrices.ReadInt("Value", "CrestOfMonarchPrice", 800000);
	this->itemPrices.iLochFeatherPrice = ReadItemPrices.ReadInt("Value", "LochFeatherPrice", 900000);
	this->itemPrices.iJewelOfGuardianPrice = ReadItemPrices.ReadInt("Value", "JewelOfGuardianPrice", 60000000);
	this->itemPrices.iWereRabbitEggPrice = ReadItemPrices.ReadInt("Value", "WereRabbitEggPrice", 3000000);
	this->itemPrices.iAngelPrice = ReadItemPrices.ReadInt("Value","GuardianAngelPrice", 200000);
	this->itemPrices.iImpPrice = ReadItemPrices.ReadInt("Value","ImpPrice", 200000);
	this->itemPrices.iUniriaPrice = ReadItemPrices.ReadInt("Value","UniriaPrice", 100000);
	this->itemPrices.iDinorantPrice = ReadItemPrices.ReadInt("Value","DinorantPrice", 1000000);
	this->itemPrices.iInvisiblityCloakPrice = ReadItemPrices.ReadInt("Value","InvisibilityCloakPrice", 800000);
	this->itemPrices.iMoonstonePendantPrice = ReadItemPrices.ReadInt("Value","MoonStonePendantPrice", 1000000);

	

	// private config
	this->data.common.joinmuIsDecreaseAllow = ReadCommon.ReadInt("joinmu", "IsDecreaseArrow", 1);
	this->data.common.joinmuFruitRequireUnWearItem = ReadCommon.ReadInt("joinmu", "IsRequireUnWearItemToFruit", 1);
	this->data.common.joinmuDropItemUnderCharacter[0] = ReadCommon.ReadInt("joinmu", "IsDropItemUnderCharacterDW", 0);
	this->data.common.joinmuDropItemUnderCharacter[1] = ReadCommon.ReadInt("joinmu", "IsDropItemUnderCharacterDK", 0);
	this->data.common.joinmuDropItemUnderCharacter[2] = ReadCommon.ReadInt("joinmu", "IsDropItemUnderCharacterElf", 0);
	this->data.common.joinmuDropItemUnderCharacter[3] = ReadCommon.ReadInt("joinmu", "IsDropItemUnderCharacterMG", 0);
	this->data.common.joinmuDropItemUnderCharacter[4] = ReadCommon.ReadInt("joinmu", "IsDropItemUnderCharacterDL", 0);
	this->data.common.joinmuDropItemUnderCharacter[5] = ReadCommon.ReadInt("joinmu", "IsDropItemUnderCharacterSU", 0);
	this->data.common.joinmuDropItemUnderCharacter[6] = ReadCommon.ReadInt("joinmu", "IsDropItemUnderCharacterRF", 0);
	this->data.common.joinmuDropItemUnderCharacter[7] = ReadCommon.ReadInt("joinmu", "IsDropItemUnderCharacterGL", 0);
	this->data.common.joinmuDurabilityDownDisable = ReadCommon.ReadInt("joinmu", "IsDisableDurabilityDec", 0);
	this->data.common.joinmuSellPriceZero = ReadCommon.ReadInt("joinmu", "IsSellPriceZero", 0);
	this->data.common.joinmuDisableWarpLevelBenefits = ReadCommon.ReadInt("joinmu","DisableWarpLevelBenefits", 0);
	this->data.common.joinmuRemoveItemSellDropRestriction = ReadCommon.ReadInt("joinmu", "RemoveItemSellDropRestriction", 0);
	this->data.common.joinmuMultiChannelWhisper = ReadCommon.ReadInt("joinmu","IsMultiChannelWhisper",0);
	CJewelMixSystem::LoadMixJewelPrice();

	g_ChangeCmd.LoadFile(this->GetPath("IGC_JoinMu_ChangeCommand.xml"));
	g_UpgradeCmd.LoadFile(this->GetPath("IGC_JoinMu_UpgradeCommand.xml"));
}

void CConfigread::ReadPetSettings()
{
	CIniReader ReadPet(this->GetPath("IGC_PetSettings.ini"));

	pet.GoldFenrirAttackRise = ReadPet.ReadInt("Pet", "GoldFenrirAttackRise", 33);
	pet.GoldFenrirDefenseRise = ReadPet.ReadInt("Pet", "GoldFenrirDefRise", 10);
	pet.GoldFenrirDurabilityRate = ReadPet.ReadInt("Pet", "GoldFenrirDurabilityRate", 2000);

	pet.RedFenrirAddExp = ReadPet.ReadInt("Pet", "RedFenrirAddExpPercent", 0);
	pet.BlueFenrirAddExp = ReadPet.ReadInt("Pet", "BlueFenrirAddExpPercent", 0);
	pet.BlackFenrirAddExp = ReadPet.ReadInt("Pet", "BlackFenrirAddExpPercent", 0);
	pet.GoldFenrirAddExp = ReadPet.ReadInt("Pet", "GoldFenrirAddExpPercent", 0);

	pet.WizardRingAddDamage = ReadPet.ReadInt("Ring", "WizardRingAddDamagePercent", 10);
	pet.WizardRingAddMagicDamage = ReadPet.ReadInt("Ring", "WizardRingAddMagicDamagePercent", 10);
	pet.WizardRingAddAttackSpeed = ReadPet.ReadInt("Ring", "WizardRingAddAttackSpeed", 10);

	pet.LethalRingAddDamage = ReadPet.ReadInt("Ring", "LethalRingAddDamagePercent", 15);
	pet.LethalRingAddMagicDamage = ReadPet.ReadInt("Ring", "LethalRingAddMagicDamagePercent", 15);
	pet.LethalRingAddAttackSpeed = ReadPet.ReadInt("Ring", "LethalRingAddAttackSpeed", 10);

	pet.UnicornAddDefense = ReadPet.ReadInt("Pet", "UnicornAddDefenseValue", 50);

	pet.EliteSkeletonPetAddDamage = ReadPet.ReadInt("Pet", "EliteSkeletonPetAddDamagePercent", 20);
	pet.EliteSkeletonPetAddAttackSpeed = ReadPet.ReadInt("Pet", "EliteSkeletonPetAddAttackSpeed", 10);

	pet.EliteSkeletonRingAddDefense = ReadPet.ReadInt("Ring", "EliteSkeletonRingAddDefensePercent", 10);

	pet.ChristmasRingAddDamage = ReadPet.ReadInt("Ring", "ChristmasRingAddDamageValue", 20);
	pet.ChristmasRingAddMagicDamage = ReadPet.ReadInt("Ring", "ChristmasRingAddMagicDamageValue", 20);

	pet.PandaRingAddDamage = ReadPet.ReadInt("Ring", "PandaRingAddDamageValue", 30);
	pet.BrownPandaRingAddDamage = ReadPet.ReadInt("Ring", "BrownPandaRingAddDamageValue", 30);
	pet.PinkPandaRingAddDamage = ReadPet.ReadInt("Ring", "PinkPandaRingAddDamageValue", 30);

	pet.SkeletonRingAddDamage = ReadPet.ReadInt("Ring", "SkeletonRingAddDamageValue", 40);

	pet.RobotKnightRingAddDefense = ReadPet.ReadInt("Ring", "RobotKnightRingAddDefenseValue", 10);
	pet.RobotKnightRingAddDamage = ReadPet.ReadInt("Ring", "RobotKnightRingAddDamageValue", 30);

	pet.MiniRobotRingAddAttackSpeed = ReadPet.ReadInt("Ring", "MiniRobotRingAddAttackSpeed", 7);
	pet.MiniRobotRingAddDamage = ReadPet.ReadInt("Ring", "MiniRobotRingAddDamageValue", 30);

	pet.MageRingAddDamage = ReadPet.ReadInt("Ring", "MageRingAddDamageValue", 40);

	pet.DecorationRingAddDamage = ReadPet.ReadInt("Ring", "DecorationRingAddDamagePercent", 10);
	pet.DecorationRingAddAttackSpeed = ReadPet.ReadInt("Ring", "DecorationRingAddAttackSpeed", 10);

	pet.BlessedDecorationRingAddDamage = ReadPet.ReadInt("Ring", "BlessedDecorationRingAddDamagePercent", 15);
	pet.BlessedDecorationRingAddAttackSpeed = ReadPet.ReadInt("Ring", "BlessedDecorationRingAddAttackSpeed", 10);

	pet.DSFRingAddDamage = ReadPet.ReadInt("Ring", "DarkTransformRingAddDamageValue", 70);
	pet.DSFRingAddAttackSpeed = ReadPet.ReadInt("Ring", "DarkTransformRingAddAttackSpeed", 10);
	pet.DSFRingAddDefense = ReadPet.ReadInt("Ring", "DarkTransformRingAddDefenseValue", 50);
	pet.DSFRingAddHP = ReadPet.ReadInt("Ring", "DarkTransformRingAddLifeValue", 50);

	pet.AngelAddHP = ReadPet.ReadInt("Pet", "AngelAddLifeValue", 50);
	pet.AngelAddDefense = ReadPet.ReadInt("Pet", "AngelReduceDamagePercent", 20);

	pet.SatanAddDamage = ReadPet.ReadInt("Pet", "SatanAddDamagePercent", 30);

	pet.DemonAddDamage = ReadPet.ReadInt("Pet", "DemonAddDamagePercent", 40);
	pet.DemonAddAttackSpeed = ReadPet.ReadInt("Pet", "DemonAddAttackSpeed", 10);

	pet.SafeGuardAddHP = ReadPet.ReadInt("Pet", "GuardianAngelAddLifeValue", 50);
	pet.SafeGuardAddDefense = ReadPet.ReadInt("Pet", "GuardianAngelReduceDamagePercent", 30);

	pet.PandaPetAddDefense = ReadPet.ReadInt("Pet", "PandaPetAddDefenseValue", 50);

	// PRIVATE CONFIG, DO NOT ADD IT IN PUBLIC RELEASE
	pet.DamageDisableForPet[DAMAGE_OFF_ANGEL] = ReadPet.ReadInt("joinmu", "DamageDisable_Angel", 0);
	pet.DamageDisableForPet[DAMAGE_OFF_SATAN] = ReadPet.ReadInt("joinmu", "DamageDisable_Satan", 0);
	pet.DamageDisableForPet[DAMAGE_OFF_UNIRIA] = ReadPet.ReadInt("joinmu", "DamageDisable_Uniria", 0);
	pet.DamageDisableForPet[DAMAGE_OFF_DINORANT] = ReadPet.ReadInt("joinmu", "DamageDisable_Dinorant", 0);
	pet.DamageDisableForPet[DAMAGE_OFF_DLHORSE] = ReadPet.ReadInt("joinmu", "DamageDisable_DLHorse", 0);
	pet.DamageDisableForPet[DAMAGE_OFF_RAVEN] = ReadPet.ReadInt("joinmu", "DamageDisable_DLRaven", 0);
	pet.DamageDisableForPet[DAMAGE_OFF_FENRIR] = ReadPet.ReadInt("joinmu", "DamageDisable_Fenrir", 0);
	pet.DamageDisableForPet[DAMAGE_OFF_DEMON] = ReadPet.ReadInt("joinmu", "DamageDisable_Demon", 0);
	pet.DamageDisableForPet[DAMAGE_OFF_SPIRIT_ANGEL] = ReadPet.ReadInt("joinmu", "DamageDisable_SpiritAngel", 0);
	pet.DamageDisableForPet[DAMAGE_OFF_RUDOLF] = ReadPet.ReadInt("joinmu", "DamageDisable_Rudolf", 0);
	pet.DamageDisableForPet[DAMAGE_OFF_PANDA] = ReadPet.ReadInt("joinmu", "DamageDisable_Panda", 0);
	pet.DamageDisableForPet[DAMAGE_OFF_UNICORN] = ReadPet.ReadInt("joinmu", "DamageDisable_Unicorn", 0);
	pet.DamageDisableForPet[DAMAGE_OFF_SKELETON] = ReadPet.ReadInt("joinmu", "DamageDisable_Skeleton", 0);
	pet.DamageDisableForPet[DAMAGE_OFF_MUUN] = ReadPet.ReadInt("joinmu", "DamageDisable_Muun", 0);
}

void CConfigread::ReadCalcCharacter()
{
	CIniReader ReadCalChar(this->GetPath("IGC_CalcCharacter.ini"));

	this->calc.DarkHorseDamageMultiplier = ReadCalChar.ReadFloat("DLPets", "DarkHorseDamageMultiplier", 100.0);
	this->calc.DarkRavenDamageMultiplier = ReadCalChar.ReadFloat("DLPets", "DarkRavenDamageMultiplier", 100.0);

	this->calc.ReflectDamage = ReadCalChar.ReadInt("ReflectDamage", "MaxDamageReflectPercent", 50);

	char * szClassPrefixes[] = { "DW", "DK", "ELF", "MG", "DL", "SU", "RF", "GL" };
	char szTemp[128];

	for (int i = 0; i < MAX_TYPE_PLAYER; i++)
	{
		for (int j = 0; j < MAX_TYPE_PLAYER; j++)
		{
			sprintf(szTemp, "%sTo%s_PvPDamageRate", szClassPrefixes[i], szClassPrefixes[j]);

			this->calc.PvPDamageRate[i][j] = ReadCalChar.ReadInt("PvPDamage", szTemp, 100);
			this->calc.ElementalPvPDamageRate[i][j] = ReadCalChar.ReadInt("Elemental", szTemp, 100);
			this->calc.PvPDamageRate_DarkSpirit[i][j] = ReadCalChar.ReadInt("DarkSpirit", szTemp, 100);
		}
	}

	this->calc.PvMDamageRate[CLASS_KNIGHT] = ReadCalChar.ReadInt("PvMDamage", "DKPvMDamageRate", 100);
	this->calc.PvMDamageRate[CLASS_WIZARD] = ReadCalChar.ReadInt("PvMDamage", "DWPvMDamageRate", 100);
	this->calc.PvMDamageRate[CLASS_ELF] = ReadCalChar.ReadInt("PvMDamage", "ELFPvMDamageRate", 100);
	this->calc.PvMDamageRate[CLASS_MAGUMSA] = ReadCalChar.ReadInt("PvMDamage", "MGPvMDamageRate", 100);
	this->calc.PvMDamageRate[CLASS_DARKLORD] = ReadCalChar.ReadInt("PvMDamage", "DLPvMDamageRate", 100);
	this->calc.PvMDamageRate[CLASS_SUMMONER] = ReadCalChar.ReadInt("PvMDamage", "SUMPvMDamageRate", 100);
	this->calc.PvMDamageRate[CLASS_RAGEFIGHTER] = ReadCalChar.ReadInt("PvMDamage", "RFPvMDamageRate", 100);
	this->calc.PvMDamageRate[CLASS_GROWLANCER] = ReadCalChar.ReadInt("PvMDamage", "GLPvMDamageRate", 100);

	this->calc.ElementalPvMDamageRate[CLASS_KNIGHT] = ReadCalChar.ReadInt("Elemental", "DKPvMDamageRate", 100);
	this->calc.ElementalPvMDamageRate[CLASS_WIZARD] = ReadCalChar.ReadInt("Elemental", "DWPvMDamageRate", 100);
	this->calc.ElementalPvMDamageRate[CLASS_ELF] = ReadCalChar.ReadInt("Elemental", "ELFPvMDamageRate", 100);
	this->calc.ElementalPvMDamageRate[CLASS_MAGUMSA] = ReadCalChar.ReadInt("Elemental", "MGPvMDamageRate", 100);
	this->calc.ElementalPvMDamageRate[CLASS_DARKLORD] = ReadCalChar.ReadInt("Elemental", "DLPvMDamageRate", 100);
	this->calc.ElementalPvMDamageRate[CLASS_SUMMONER] = ReadCalChar.ReadInt("Elemental", "SUMPvMDamageRate", 100);
	this->calc.ElementalPvMDamageRate[CLASS_RAGEFIGHTER] = ReadCalChar.ReadInt("Elemental", "RFPvMDamageRate", 100);
	this->calc.ElementalPvMDamageRate[CLASS_GROWLANCER] = ReadCalChar.ReadInt("Elemental", "GLPvMDamageRate", 100);

	this->calc.PvMDamageRate_DarkSpirit = ReadCalChar.ReadInt("DarkSpirit", "PvMDamageRate", 100);

	this->calc.MaximumDamageDec = ReadCalChar.ReadInt("PvPDamage", "MaximumDamageDec", 100);
	this->calc.ElementalDamageAffectSDOnly = ReadCalChar.ReadInt("Elemental", "ElementalDmgAffectSDOnly", 1);
}

LPSTR CConfigread::GetPath(LPSTR path)
{
	memset(this->CurrPath, 0x00, sizeof(this->CurrPath));
	strcpy(this->CurrPath, this->IGCData);
	strcat(this->CurrPath, path);

	return this->CurrPath;
}

void CConfigread::ReadOffTradeConfig()
{
	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(this->GetPath("IGC_OffTrade.xml"));

	if (res.status != pugi::status_ok)
	{
		g_Log.MsgBox("%s File Load Error (%s)", this->GetPath("IGC_OffTrade.xml"), res.description());
		return;
	}

	pugi::xml_node main = file.child("OffTrade");

	this->offtrade.enabled = main.attribute("Enable").as_int();
	this->offtrade.CoinType = main.attribute("CoinType").as_int();

	for (pugi::xml_node map = main.child("Map"); map; map = map.next_sibling())
	{
		int iMapNumber = map.attribute("Number").as_int();

		if (iMapNumber < 0 || iMapNumber >= MAX_NUMBER_MAP)
		{
			g_Log.MsgBox("%s - Wrong map number %d", this->GetPath("IGC_OffTrade.xml"), iMapNumber);
			continue;
		}

		int iState = map.attribute("Disable").as_int();

		this->offtrade.Maps[iMapNumber] = iState == 0 ? TRUE : FALSE;
	}
}

void CConfigread::ReadPKSettings()
{
	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(this->GetPath("IGC_PlayerKillSystem.xml"));

	if (res.status != pugi::status_ok)
	{
		g_Log.MsgBox("PK Config load failed (%s)", res.description());
		return;
	}

	for (int i = 0; i < 7; i++)
	{
		this->pk.m_LevelConfig[i].m_vtExpDec.clear();
	}

	pugi::xml_node main = file.child("PlayerKillSystem");

	this->pk.iPKPointDeductionDivisor = main.attribute("PointDeductionDivisor").as_int();
	this->pk.iPKPointIncForMurderer = main.attribute("MurdererPointIncrease").as_int();
	this->pk.bPkLevelIncreaseOff = main.attribute("DisablePKLevelIncrease").as_bool();
	this->pk.bPkPenaltyDisable = main.attribute("DisablePenalty").as_bool();
	this->pk.bPkCanUseShops = main.attribute("PKCanUseshops").as_bool();
	this->pk.bPkDropExpensiveItems = main.attribute("DropExpensiveItems").as_bool();
	this->pk.iPkMaxLevelItemDrop = main.attribute("MaxItemLevelDrop").as_int();
	pugi::xml_node pk_clear = main.child("PKClearCommand");

	this->pk.bPkClearEnable = pk_clear.attribute("Enable").as_bool();
	this->pk.iPkClearCost = pk_clear.attribute("Cost").as_int();
	this->pk.bIsCostMultiplyByKillCount = pk_clear.attribute("CostMultiplyByKillCount").as_bool();

	pugi::xml_node general = main.child("General");

	for (pugi::xml_node pk = general.child("PK"); pk; pk = pk.next_sibling())
	{
		int iPKLevel = pk.attribute("Level").as_int();

		if (iPKLevel < 0 || iPKLevel >= 7)
		{
			g_Log.MsgBox("PK Config - Wrong PK Level (%d)", iPKLevel);
			return;
		}

		this->pk.m_LevelConfig[iPKLevel].btPKLevel = iPKLevel;
		this->pk.m_LevelConfig[iPKLevel].iPKReqPoint = pk.attribute("ReqPoint").as_int();
		this->pk.m_LevelConfig[iPKLevel].iItemDropRate_Monster = pk.attribute("ItemLoseRateOnMonsterKill").as_int();
		this->pk.m_LevelConfig[iPKLevel].iItemDropRate_Character = pk.attribute("ItemLoseRateOnPlayerKill").as_int();
		this->pk.m_LevelConfig[iPKLevel].iZenLossRate_Monster = pk.attribute("ZenDeductionRateOnMonsterKill").as_int();
		this->pk.m_LevelConfig[iPKLevel].iZenLossRate_Character = pk.attribute("ZenDeductionRateOnPlayerKill").as_int();
		this->pk.m_LevelConfig[iPKLevel].iWarpCostMultiplier = pk.attribute("WarpChargeMultiplier").as_int();
		this->pk.m_LevelConfig[iPKLevel].bCanSummonByDarkLord = pk.attribute("CanSummonByDarkLord").as_bool();
	}

	pugi::xml_node exp_dec = main.child("ExpDeduction");

	for (pugi::xml_node pk = exp_dec.child("PK"); pk; pk = pk.next_sibling())
	{
		int iPKLevel = pk.attribute("Level").as_int();

		if (iPKLevel < 0 || iPKLevel >= 7)
		{
			g_Log.MsgBox("PK Config - Wrong PK Level (%d)", iPKLevel);
			return;
		}	

		for (pugi::xml_node level_range = pk.child("LevelRange"); level_range; level_range = level_range.next_sibling())
		{
			PK_EXP_DECREASE pkExp;

			pkExp.iLevelMin = level_range.attribute("Start").as_int();

			if (strcmp("MAX", level_range.attribute("End").as_string()) == 0)
			{
				pkExp.iLevelMax = this->data.common.UserMaxLevel + this->data.common.MLUserMaxLevel;
			}

			else
			{
				pkExp.iLevelMax = level_range.attribute("End").as_int(730);
			}

			pkExp.iDecRate = level_range.attribute("DeductionRate").as_int();

			this->pk.m_LevelConfig[iPKLevel].m_vtExpDec.push_back(pkExp);
		}
	}
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

