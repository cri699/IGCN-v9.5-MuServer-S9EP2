// ------------------------------
// Decompiled by Deathway
// Date : 2007-03-09
// ------------------------------
//------------------------------------------
// GameMain.cpp
//------------------------------------------
// GS-N 0.99.60T 0x004F4490
//	GS-N	1.00.18	JPN	0x0051FE00	-	Completed
#include "stdafx.h"
#include "Lang.h"
#include "gamemain.h"
#include "winutil.h"
#include "GameServer.h"
#include "DirPath.h"
#include "TLog.h"
#include "wsJoinServerCli.h"
#include "DoppelGanger.h"
#include "DSProtocol.h"
#include "CLoginCount.h"
#include "MapClass.h"
#include "MonsterRegenSystem.h"
#include "WzUdp.h"
#include "classdef.h"
#include "prodef.h"
#include "wsGameServer.h"
#include "EledoradoEvent.h"
#include "ObjUseSkill.h"
#include "WhisperCash.h"
#include "MasterLevelSkillTreeSystem.h"
#include "SProtocol.h"
#include "ChaosCastle.h"
#include "DevilSquare.h"
#include "Gate.h"
#include "user.h"
#include "PacketCheckSum.h"
#include "DragonEvent.h"
#include "AttackEvent.h"
#include "MapServerManager.h"
#include "QuestInfo.h"
#include "SkillHitBox.h"
#include "CItemDrop.h"
#include "EventManagement.h"
#include "SetItemOption.h"
#include "ItemSocketOptionSystem.h"
#include "EDSProtocol.h"
#include "MonsterItemMng.h"
#include "ConMember.h"
#include "GMMng.h"
#include "BuffScriptLoader.h"
#include "BattleSoccerManager.h"
#include "GuildClass.h"
#include "MoveCommand.h"
#include "BloodCastle.h"
#include "Shop.h"
#include "ItemAddOption.h"
#include "Kanturu.h"
#include "KanturuMonsterMng.h"
#include "SkillAdditionInfo.h"
#include "TMonsterSkillElement.h"
#include "TMonsterSkillUnit.h"
#include "TMonsterSkillManager.h"
#include "TMonsterAIElement.h"
#include "TMonsterAIAutomata.h"
#include "TMonsterAIUnit.h"
#include "TMonsterAIRule.h"
#include "TMonsterAIGroup.h"
//Added by HermeX
#include "Crywolf.h"
#include "CrywolfAltar.h"
#include "CashShop.h"
#include "CashLotterySystem.h"
#include "ImperialGuardian.h"
#include "MonsterStatCalc.h"
#include "Raklion.h"
#include "RaklionMonsterMng.h"
#include "BonusEvent.h"
#include "AntiSwear.h"
#include "ObjCalCharacter.h"
#include "GamblingSystem.h"
#include "configread.h"
#include "VipSys.h"
#include "NewsSystem.h"
#include "SetItemDrop.h"
#include "PentagramSystem.h"
#include "PentagramMixSystem.h"
#include "LogToFile.h"
#include "MapAttribute.h"
#include "NewPVP.h"
#include "MonsterKillCount.h"
#include "PeriodItemEx.h"
#include "QuestExpInfo.h"
#include "QuestExpLuaBind.h"
#include "LuaFun.h"
#include "ExpManager.h"
#include "BagManager.h"
#include "LuckyItemManager.h"
#include "SkillUseTime.h"
#include "ProhibitedSymbols.h"
#include "TUnion.h"
#include "MuunSystem.h";
#include "ItemOptionTypeMng.h"
#include "AppointItemDrop.h"
#include "SendNPCInfo.h"
#include "ChaosCastleFinal.h"
#include "MineSystem.h"
#include "ObjAttack.h"
#include "IllusionTempleEvent_Renewal.h"
#include "AcheronGuardianEvent.h"
#include "StatSpecialize.h"
#include "SkillSpecialize.h"
#include "OfflineLevelling.h"
#include "DevilSquareFinal.h"
#include "BotSystem.h"
#include "GensSystem.h"
#include "Marry.h"
#include "EventMonSetBase.h"
#include "SkillSafeZoneUse.h"

//+-----------------------------------------
//	Castle Siege
//+-----------------------------------------
#include "CastleDeepEvent.h"
#include "LastManStanding.h"
//------------------------------------------
// GameMain.cpp Variables
//------------------------------------------

BOOL JoinServerConnected;
BOOL DataServerConnected;
BOOL GameServerCreated;
BOOL IsEventChipServerConnected;
CDragonEvent * DragonEvent;
CAttackEvent * AttackEvent;
WzUdp gUdpSoc;	// line 256
BOOL SpeedHackPlayerBlock;
int	 gCheckSumSwitch;
int  gSpeedHackPenalty;
bool gSerialZeroCheck;
BOOL gEnableEventNPCTalk;
BOOL gEnableServerDivision;
BOOL gEvent1;
int  gMonsterHp;
BOOL gMerryXMasNpcEvent;
BOOL gHappyNewYearNpcEvent;
int  gEvent1ItemDropTodayCount;
char gStalkProtocolId[11];
BOOL gXMasEvent;
BOOL gFireCrackerEvent;
BOOL gHeartOfLoveEvent;
BOOL gMedalEvent;
BOOL gEventChipEvent;
BOOL gWriteSkillLog;
BOOL g_bStoneItemDrop;
int  g_iKundunMarkDropRate;
int  g_iMarkOfTheLord;
int	 g_iLuckyCoin;
int  g_iDarkLordHeartDropRate;
int  g_iDarkLordHeartOffEventRate;
int g_iMysteriousBeadDropRate1;
int g_iMysteriousBeadDropRate2;
int g_iHiddenTreasureBoxOfflineRate;
BOOL bIsIgnorePacketSpeedHackDetect;
BOOL gIsKickDetecHackCountLimit;
BOOL gIsItemDropRingOfTransform;
BOOL gDoPShopOpen;
BOOL gWriteChatLog;
BOOL gDisconnectHackUser;
BOOL GSInfoSendFlag = FALSE;

///////////////////////////////////////////////////////////////////////////////

DWORD JoinServerDCTime = 0;

CwsGameServer wsGServer;	// line : 213GameServer
wsJoinServerCli wsJServerCli;	// line : 214 Join Server
wsJoinServerCli wsDataCli;	// line : 215 DataServer
wsJoinServerCli wsExDbCli;	// line : 239 Extra DataBase Server
CDirPath gDirPath;	// line : 248 Directory Path
MapClass MapC[MAX_NUMBER_MAP];	// line 249	// Map Manager
CMonsterAttr gMAttr;	// line 250
CMonsterSetBase gMSetBase;	// line 251
classdef DCInfo;	// line 252:
CWhisperCash WhisperCash;	// line 253
PartyClass gParty;	// line 254

CGuildClass Guild;	// line 265

CLoginCount gLCount[3];	// line 329

UINT64 * gLevelExperience;
char szGameServerExeSerial[24];

BOOL gIsDropDarkLordItem;
int  gSleeveOfLordDropRate;
int  gSleeveOfLordDropLevel;

int  gSoulOfDarkHorseDropRate;
int  gSoulOfDarkHorseropLevel;
int  gSoulOfDarkSpiritDropRate;
int  gSoulOfDarkSpiritDropLevel;
float gDarkSpiritAddExperience;
BOOL gIsDropGemOfDefend;
int  gGemOfDefendDropRate;
int  gGemOfDefendDropLevel;
int  g_iUseCharacterAutoRecuperationSystem;
int  g_iCharacterRecuperationMaxLevel;
int g_iServerGroupGuildChatting;
int g_iServerGroupUnionChatting;
BOOL g_bRibbonBoxEvent;
int g_iRedRibbonBoxDropLevelMin;
int g_iRedRibbonBoxDropLevelMax;
int g_iRedRibbonBoxDropRate;
int g_iGreenRibbonBoxDropLevelMin;
int g_iGreenRibbonBoxDropLevelMax;
int g_iGreenRibbonBoxDropRate;
int g_iBlueRibbonBoxDropLevelMin;
int g_iBlueRibbonBoxDropLevelMax;
int g_iBlueRibbonBoxDropRate;
BOOL g_bChocolateBoxEvent;
int g_iPinkChocolateBoxDropLevelMin;
int g_iPinkChocolateBoxDropLevelMax;
int g_iPinkChocolateBoxDropRate;
int g_iRedChocolateBoxDropLevelMin;
int g_iRedChocolateBoxDropLevelMax;
int g_iRedChocolateBoxDropRate;
int g_iBlueChocolateBoxDropLevelMin;
int g_iBlueChocolateBoxDropLevelMax;
int g_iBlueChocolateBoxDropRate;
BOOL g_bCandyBoxEvent;
int g_iLightPurpleCandyBoxDropLevelMin;
int g_iLightPurpleCandyBoxDropLevelMax;
int g_iLightPurpleCandyBoxDropRate;
int g_iVermilionCandyBoxDropLevelMin;
int g_iVermilionCandyBoxDropLevelMax;
int g_iVermilionCandyBoxDropRate;
int g_iDeepBlueCandyBoxDropLevelMin;
int g_iDeepBlueCandyBoxDropLevelMax;
int g_iDeepBlueCandyBoxDropRate;
BOOL g_bFenrirStuffItemDrop;
int g_iFenrirStuff_01_DropLv_Min;
int g_iFenrirStuff_01_DropLv_Max;
int g_iFenrirStuff_01_DropMap;
int g_iFenrirStuff_01_DropRate;
int g_iFenrirStuff_02_DropLv_Min;
int g_iFenrirStuff_02_DropLv_Max;
int g_iFenrirStuff_02_DropMap;
int g_iFenrirStuff_02_DropRate;
int g_iFenrirStuff_03_DropLv_Min;
int g_iFenrirStuff_03_DropLv_Max;
int g_iFenrirStuff_03_DropMap;
int g_iFenrirStuff_03_DropRate;
int g_iFenrirRepairRate;
int g_iFenrirDefaultMaxDurSmall;
int g_iFenrirElfMaxDurSmall;
int g_iFenrir_01Level_MixRate;
int g_iFenrir_02Level_MixRate;
int g_iFenrir_03Level_MixRate;
int g_iSkillDistanceCheck;
int g_iSkillDistanceCheckTemp;
int g_iSkillDistanceKick;
int g_iSkillDistanceKickCount;
int g_iSkillDiatanceKickCheckTime;
BOOL g_bHallowinDayEventOn;
int g_iHallowinDayEventItemDropRate;
int g_iHallowinDayEventJOLBlessDropRate;
int g_iHallowinDayEventJOLAngerDropRaTe;
int g_iHallowinDayEventJOLScreamDropRate;
int g_iHallowinDayEventJOLFoodDropRate;
int g_iHallowinDayEventJOLDrinkDropRate;
int g_iHallowinDayEventJOLPolymorphRingDropRate;
BOOL g_bSantaPolymorphRingDropOn;
int g_iSantaPolymorphRingDropRate;
int g_iCondorFlameDropRate;
int g_iShadowPhantomMaxLevel;
bool g_bBlossomBoxDropOn;
int g_iBlossomBoxDropRate;

int g_MaxStrength;
int g_MaxAgility;
int g_MaxVitality;
int g_MaxEnergy;
int g_MaxCommand;

DWORD dwgCheckSum[MAX_CHECKSUM_KEY];
BOOL g_bCastleGuildDestoyLimit;
DWORD  gItemNumberCount;
BOOL gStalkProtocol;
int  gYear;
BOOL gOnlyFireCrackerEffectUse;
int  giKundunRefillHPSec;
int  giKundunRefillHP;
int  giKundunRefillHPTime;
int  giKundunHPLogSaveTime;
BOOL gUseNPGGChecksum;
int g_ShieldSystemOn;
int g_iDamageDevideToSDRate;
int g_iDamageDevideToHPRate;
float g_fSuccessAttackRateOption;
int g_iSDChargingOption;
int g_iConstNumberOfShieldPoint;
int g_ShieldAutoRefillOn;
int g_ShieldAutoRefillOnSafeZone;
int g_CompoundPotionDropOn;
int g_iCompoundPotionLv1DropRate;
int g_iCompoundPotionLv2DropRate;
int g_iCompoundPotionLv3DropRate;
int g_iCompoundPotionLv1DropLevel;
int g_iCompoundPotionLv2DropLevel;
int g_iCompoundPotionLv3DropLevel;
BOOL g_bShieldComboMissOptionOn;
int g_iShieldPotionLv1MixSuccessRate;
int g_iShieldPotionLv1MixMoney;
int g_iShieldPotionLv2MixSuccessRate;
int g_iShieldPotionLv2MixMoney;
int g_iShieldPotionLv3MixSuccessRate;
int g_iShieldPotionLv3MixMoney;
int g_iShieldGageConstA;
int g_iShieldGageConstB;

////////////////////////////////////////////////////////////////////

char szClientVersion[8]="000000";
BOOL bCanTrade=1;
BOOL bCanChaosBox=1;
BOOL bCanWarehouseLock=1;

BOOL gEnableBattleSoccer=1;
int  gLootingTime=3;
int  gItemDropPer=10;
int  IsMultiWareHouse=1;
int  g_WareHouseCount=3;
int gLevelUpPointNormal=5;
int gLevelUpPointMGDL=7;
int gItemDisapearTime=120;
int  gEvent1ItemDropTodayMax=1;
int  gEvent1ItemDropTodayPercent=80;
int  gCharacterDeleteMinLevel=40;
BOOL gCreateCharacter=1;
BOOL gItemSerialCheck=1;
int  g_XMasEvent_StarOfXMasDropRate=80;
int  g_XMasEvent_ItemDropRateForStarOfXMas=2;
int  gFireCrackerDropRate=80;
int  g_ItemDropRateForgFireCracker=2;
int  gHeartOfLoveDropRate=80;
int  g_ItemDropRateForgHeartOfLove=2;
int  gGoldMedalDropRate=2;
int  gSilverMedalDropRate=2;
int  g_ItemDropRateForGoldMedal=2;
int  g_ItemDropRateForSilverMedal=2;
int  gBoxOfGoldDropRate=2;
int  g_ItemDropRateForBoxOfGold=2;
int  g_EventChipDropRateForBoxOfGold=2;
int  gDQChaosSuccessRateLevel1=75;
int  gDQChaosSuccessRateLevel2=70;
int  gDQChaosSuccessRateLevel3=65;
int  gDQChaosSuccessRateLevel4=60;
int  gDQChaosSuccessRateLevel5=55;
int  gDQChaosSuccessRateLevel6=50;
int	 gDQChaosSuccessRateLevel7=45;
DWORD  gAttackSpeedTimeLimit=130;
DWORD  gHackCheckCount=5;
float gDecTimePerAttackSpeed=5.33f;
int  gMinimumAttackSpeedTime=8;
int  gDetectedHackKickCount=10;
BOOL gItemDropRingOfTransform=1;
int  gQuestNPCTeleportTime=60;
int  gZenDurationTime=30;
BOOL gEnableCheckPenetrationSkill=TRUE;

short gPostLevel;
int gPostMoney;
char gIsDropCmd;

int gSilverBoxDropRate;
int gGoldBoxDropRate;

int gSetPartyExp2;
int gSetPartyExp3;
int gSetPartyExp4;
int gSetPartyExp5;
int gPartyExp1;
int gPartyExp2;
int gPartyExp3;
int gPartyExp4;
int gPartyExp5;

short gLuckDropRate = 4;
short gSkillDropRate = 6;
short gExcLuckDropRate = 1;
short gExcSkillDropRate = 100;
short gExcDropRate = 2000;

int gWeaponDefenseDamage;
int gWeaponAttackDamage;

int gIsDropSetItemInCastleHuntZone;
int gSetItemInCastleHuntZoneDropRate;
int gSetItemInCastleHuntZoneDropLevel;

CLogToFile * GMLog;
CLogToFile * MsgLog;
CLogToFile * TradeLog;
CLogToFile * AntiHackLog;
CLogToFile * BotShopLog;
CLogToFile * SerialCheck;

void CheckSumFileLoad(char* szCheckSum);

void gSetDate() // Good
{
	tm *today;
	time_t ltime;

	time(&ltime);
	today=localtime(&ltime);
	
	today->tm_year=today->tm_year+1900;
	gYear=today->tm_year;
}

BOOL gJoomin15Check(char* szJN) // Status : Good
{
	int tyear=1900;
	char szTyear[3]="";
	
	if ( szJN[6] == 51 || szJN[6] == 52)
	{
		tyear = 2000;
	}

	memcpy(&szTyear[0], szJN, sizeof(szTyear)-1);

	tyear=tyear+atoi(&szTyear[0]);
	if ( (gYear-15 ) < tyear )
	{
		return 0;
	}
	return 1;
}
	
BOOL gJoominCheck(char* szJN, int iLimitAge) // Good
{

	if ( iLimitAge <0 )
	{
		return 1;
	}

	int tyear=1900;
	char szTyear[3]="";

	if ( szJN[6] == 51 || szJN[6] == 52)
	{
		tyear = 2000;
	}

	memcpy(&szTyear[0], szJN, sizeof(szTyear)-1);

	tyear=tyear+atoi(&szTyear[0]);
	if ( (gYear-iLimitAge ) < tyear )
	{
		return 0;
	}
	
	return 1;
}

void GameMainInit(HWND hWnd)
{
	ItemSerialCreateSend = ItemCreate;
	srand(time(nullptr));

	g_ConfigRead.ReadServerInfo(true);
	g_ConfigRead.SetEventTable();

	g_Log.LogInit(g_ConfigRead.server.IsLogCreate(), g_ConfigRead.server.GetLogMaxFileSize());
	gSetDate();

	char Path[60];
	char IGCPath[60];
	char CommonServerPath[60];

	wsprintf(Path, "..\\data-%d\\", g_ConfigRead.server.GetGameServerCode());
	wsprintf(IGCPath, "..\\IGCData-%d\\", g_ConfigRead.server.GetGameServerCode());
	wsprintf(CommonServerPath, "..\\data-%d\\commonserver.cfg", g_ConfigRead.server.GetGameServerCode());

	WIN32_FIND_DATA wfd;

	if(FindFirstFile(CommonServerPath, &wfd) == INVALID_HANDLE_VALUE)
	{
		wsprintf(Path, "..\\data\\");
		wsprintf(IGCPath, "..\\IGCData\\");
	}

	GMLog = new CLogToFile("GM_LOG", g_ConfigRead.server.GetGMLogPath(), 256);
	TradeLog = new CLogToFile("TRADE_LOG", g_ConfigRead.server.GetTradeLogPath(), 256);
	MsgLog = new CLogToFile("MSG_LOG", g_ConfigRead.server.GetMsgLogPath(), 256);
	AntiHackLog = new CLogToFile("ANTIHACK_LOG", g_ConfigRead.server.GetAntiHackLogPath(), 256);
	BotShopLog = new CLogToFile("BOT_SHOP_LOG", g_ConfigRead.server.GetBotShopLogPath(),256);
	SerialCheck = new CLogToFile("SERIAL_CHECK_LOG", g_ConfigRead.server.GetSerialCheckLogPath(), 256);

	gDirPath.SetFirstPath(Path);
	strcpy(g_ConfigRead.IGCData, IGCPath);

	Lang.Init();
	
	gCheckSumSwitch=GetPrivateProfileInt("GameServerInfo","CheckSumCheck",1,gDirPath.GetNewPath("commonserver.cfg") );

	// Initialize classes ( gObj related )
	gObjInit();
	gMSetBase.Init();
	gParty.Init();
	g_OffLevel.Initiate();

	DragonEvent = new CDragonEvent;
//	g_Log.AddC(TColor::Red,"%s %s %s %d %s %s %d %i %s %d %d");
	if ( DragonEvent == 0 )
	{
		g_Log.MsgBox("CDragonEvent - Memory allocation error");	// Memory allocation error
		return;
	}

	AttackEvent = new CAttackEvent;
	
	if ( AttackEvent == 0 )
	{
		g_Log.MsgBox("AttackEvent - Memory allocation error");	// Memory allocation error
		return;
	}

	if ( gUdpSoc.CreateSocket() == 0)
	{
		g_Log.MsgBox("UDP Socket create error");
		return;
	}

	if (g_ConfigRead.server.GetServerType() == SERVER_BATTLECORE)
	{
		g_SendNPCInfo.LoadScript(g_ConfigRead.GetPath("IGC_MiniMap_BattleCore.xml"));
	}

	else
	{
		g_SendNPCInfo.LoadScript(g_ConfigRead.GetPath("IGC_MiniMap.xml"));
	}

	g_MapServerManager.LoadData(g_ConfigRead.GetPath("IGC_MapServerInfo.xml"));
	ReadCommonServerInfo();

	DWORD dwThreadId;
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)GameServerInfoSend, NULL, NULL, &dwThreadId);

	if (g_ConfigRead.server.GetServerType() == SERVER_CASTLE)
	{
		g_CastleSiege.Clear();

		if (g_CastleSiege.Ready(g_MapServerManager.GetMapSvrGroup()) == TRUE)
		{
			if (g_CastleSiege.LoadData(g_ConfigRead.GetPath("\\Events\\IGC_CastleSiege.xml")))
			{
				g_CastleSiege.LoadPreFixData(g_ConfigRead.GetPath("\\Events\\IGC_CastleSiege.xml"));
				g_CastleSiege.SetDataLoadState(CASTLESIEGE_DATALOAD_2);
			}
		}
	}

	InitBattleSoccer();

	gMAttr.LoadAttr(g_ConfigRead.GetPath("\\Monsters\\IGC_MonsterList.xml"));
	gMSetBase.LoadSetBase(g_ConfigRead.GetPath("\\Monsters\\IGC_MonsterSpawn.xml"));
	gEventMSetBase.LoadScript(g_ConfigRead.GetPath("\\Monsters\\IGC_MonsterSpawn_Event.xml"));

	if ( gLevelExperience != NULL )
	{
		delete [] gLevelExperience;
	}

	gLevelExperience = new UINT64[g_ConfigRead.data.common.UserMaxLevel+1];

	if ( gLevelExperience == NULL )
	{
		g_Log.MsgBox("error - memory allocation failed");
		return;
	}

	g_MonsterItemMng.Init();
	gLevelExperience[0]=0;

	MULua * TempLua = new MULua(false);
	TempLua->DoFile(g_ConfigRead.GetPath("\\Scripts\\Misc\\ExpCalc.lua"));
	double exp = 0.0;

	for (int n=1;n<=g_ConfigRead.data.common.UserMaxLevel;n++)
	{
		TempLua->Generic_Call("SetExpTable_Normal", "i>d", n, &exp);
		gLevelExperience[n] = exp;
		g_Log.Add("[NORMAL EXP] [LEVEL %d] [EXP %I64d]", n, gLevelExperience[n]);
	}

	delete TempLua;

	g_MasterLevelSkillTreeSystem.SetExpTable();

	LoadMapFile();
	g_ShopMng.LoadShopList(g_ConfigRead.GetPath("IGC_ShopList.xml"));
	wsGServer.CreateSocket(hWnd);
	GameMonsterAllAdd();
	g_LastManStanding.SpawnRegNPC();
	//g_BotSystem.Init();

	g_PeriodItemEx.Initialize();
	g_CMuunSystem.Initialize();
	Luna<QuestExpLuaBind>::Register(g_MuLuaQuestExp.GetLua());
	g_MuLuaQuestExp.DoFile(g_ConfigRead.GetPath("\\Scripts\\Quests\\Quest_Info.lua"));
	g_MuLuaQuestExp.DoFile(g_ConfigRead.GetPath("\\Scripts\\Quests\\Quest_Main.lua"));
	g_Generic_Call(g_MuLuaQuestExp.GetLua(), "SetQuestInfo", ">");
	g_QuestExpManager.QuestExpItemInit();

	if (g_OffLevel.b_Enabled == true)
	{
		DWORD ThreadId;
		CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(OffLevelThread), nullptr, 0, &ThreadId);
	}
}

int GetWarehouseUsedHowMuch(int UserLevel, BOOL IsLock)
{
	if (g_ConfigRead.data.common.FreeWarehouse == true)
	{
		return 0;
	}

	int rZen=0;
	rZen = (UserLevel * UserLevel)* 0.1 * 0.4;

	if (bCanWarehouseLock == TRUE)
	{
		if ( IsLock != 0)
		{
			rZen = rZen + ( UserLevel * 2 );
		}
	}
	if ( rZen < 1 )
	{
		rZen=1;
	}

	if ( rZen >= 1000 )
	{
		rZen = (rZen/100)*100;
	}
	
	else if ( rZen >= 100 )
	{
		rZen = (rZen/10)*10;
	}
	
	if ( rZen == 0 )
	{
		rZen=1;
	}
	
	return rZen;
}

void GameMonsterAllAdd()
{
	int result;

	for ( int n=0;n< gMSetBase.m_Count ;n++)
	{
		if ( DS_MAP_RANGE(gMSetBase.m_Mp[n].m_MapNumber) != FALSE )
		{
			continue;
		}

		result = gObjAddMonster(gMSetBase.m_Mp[n].m_MapNumber);

		if ( result >= 0 )
		{
			gObjSetPosMonster(result, n);
			gObjSetMonster(result, gMSetBase.m_Mp[n].m_Type);

			if (g_ConfigRead.server.GetServerType() == SERVER_CASTLE)
			{
				if (gObj[result].Class == 216)
				{
					g_CastleSiege.SetCrownIndex(result);
				}

				if (CRYWOLF_MAP_RANGE(gObj[result].MapNumber))
				{
					if (gObj[result].Type == OBJ_NPC)
					{
						if (gObj[result].Class == 406 || gObj[result].Class == 407)
						{

						}
						else if (CRYWOLF_ALTAR_CLASS_RANGE(gObj[result].Class)) //Altar?
						{
							g_Crywolf.m_ObjSpecialNPC.AddObj(result);
						}
						else
						{
							g_Crywolf.m_ObjCommonNPC.AddObj(result);
						}
					}
				}

				if (CRYWOLF_MAP_RANGE(gObj[result].MapNumber))
				{
					if (gObj[result].Type == OBJ_MONSTER)
					{
						g_Crywolf.m_ObjCommonMonster.AddObj(result);
					}
				}
			}

			if ( CHECK_CLASS(gObj[result].MapNumber, MAP_INDEX_CRYWOLF_FIRSTZONE) )
			{
				if ( gObj[result].Type == OBJ_MONSTER )
				{
					g_Crywolf.m_ObjCommonMonster.AddObj(result);
				}
			}
		}
	}

	g_DevilSquare.Init();
	g_BloodCastle.Init();
	g_ChaosCastle.Init();
	g_CastleDeepEvent.Init();
	g_Crywolf.Init();
	g_Kanturu.Init();
	g_Raklion.Init();
	g_DoppelGanger.DoppelgangerProcessInit();
	g_ImperialGuardian.ImperialGuardianProcessInit();
	g_RingAttackEvent.Init();
	g_XMasAttackEvent.Init();
	g_ArcaBattle.Init();
	g_AcheronGuardianEvent.Init();
	g_ChaosCastleFinal.Init();
	g_DevilSquareFinal.DSFEventInit();
	g_IT_Event.ITR_EventInit();
	g_LastManStanding.Init();

	g_ShopMng.SetShopNpcs();
	g_MineSystem.SetTwinklesInfo();
	g_BotSystem.Init();
}



void GameMonsterAllCloseAndReLoad()
{
	for ( int n=0;n<g_ConfigRead.server.GetObjectMaxMonster();n++)
	{
		if ( gObj[n].Type == OBJ_MONSTER || gObj[n].Type == OBJ_NPC )
		{
			if (g_ConfigRead.server.GetServerType() == SERVER_CASTLE)
			{
				if (gObj[n].m_btCsNpcType)
				{
					continue;
				}
			}

			if ( gObj[n].m_iCurrentAI )
			{
				gObj[n].Live = FALSE;
				gObjViewportListProtocolDestroy(&gObj[n]);
				gObjViewportClose(&gObj[n]);
			}
			
			gObjDel(n);
		}

		if ( gObj[n].Type == OBJ_USER && gObj[n].m_PlayerData->ISBOT == true )
		{
			gObj[n].Type = OBJ_EMPTY;
			delete gObj[n].m_PlayerData;
			gObj[n].m_PlayerData = nullptr;
			gObjDel(n);
		}
	}

	if (g_ConfigRead.server.GetServerType() == SERVER_CASTLE)
	{
		g_Crywolf.m_ObjCommonNPC.Reset();
		g_Crywolf.m_ObjSpecialNPC.Reset();
		g_Crywolf.m_ObjCommonMonster.Reset();
		g_Crywolf.m_ObjSpecialMonster.Reset();
	}

	gMAttr.LoadAttr(g_ConfigRead.GetPath("\\Monsters\\IGC_MonsterList.xml"));
	gMSetBase.LoadSetBase(g_ConfigRead.GetPath("\\Monsters\\IGC_MonsterSpawn.xml"));
	gEventMSetBase.LoadScript(g_ConfigRead.GetPath("\\Monsters\\IGC_MonsterSpawn_Event.xml"));

	g_IT_Event.ResetPosNum();
	g_MonsterItemMng.Init();
	gObjMonCount = 0;
	GameMonsterAllAdd();
}

void GameMainFree()
{
	g_QuestExpManager.QuestInfoDelete();
	GDGameServerInfoSave();
	gObjEnd();
	gMSetBase.Delete();
	gParty.Delete();
	ClearBattleSoccer();
	UnionManager.DelAllUnion();
	delete TradeLog;
	delete MsgLog;
	delete GMLog;
	delete AntiHackLog;
	delete [] gLevelExperience;
	delete AttackEvent;
	delete DragonEvent;
	delete BotShopLog;
	delete SerialCheck;
}

BOOL GMJoinServerConnect(LPSTR ConnectServer, DWORD wMsg)
{
	if ( wsJServerCli.GetConnect() == TRUE )
	{
		return TRUE;
	}

	wsJServerCli.CreateSocket(ghWnd);
	wsJServerCli.SetProtocolCore(SProtocolCore);
	int result = wsJServerCli.Connect(ConnectServer, g_ConfigRead.server.GetJoinServerPort(), wMsg);

	if ( result == 0 )
	{
		return FALSE;
	}

	GJServerLogin();

	g_Log.Add("JoinServer Connected (%s)", ConnectServer);
	return TRUE;

}

BOOL GMDataServerConnect(char* ConnectServer, WPARAM wMsg)
{
	if ( wsDataCli.GetConnect() == TRUE )
	{
		return TRUE;
	}

	wsDataCli.CreateSocket(ghWnd);
	wsDataCli.SetProtocolCore(DataServerProtocolCore);
	int result = wsDataCli.Connect(ConnectServer, g_ConfigRead.server.GetDataServerPort(), wMsg);

	if ( result == 0 )
	{
		g_Log.Add("DataServer connect fail IP [ %s ] PORT [ %d ]", ConnectServer, g_ConfigRead.server.GetDataServerPort());
		return FALSE;
	}

	DataServerLogin();
	g_Log.Add("DataServer connect IP [ %s ] PORT [ %d ]", ConnectServer, g_ConfigRead.server.GetDataServerPort());
	return TRUE;
}

BOOL ExDataServerConnect(char* ConnectServer, DWORD wMsg)
{
	if ( wsExDbCli.GetConnect() == TRUE )
	{
		return TRUE;
	}

	if ( g_ConfigRead.server.GetServerType() == SERVER_BATTLECORE )
	{
		return TRUE;
	}

	wsExDbCli.CreateSocket(ghWnd);
	wsExDbCli.SetProtocolCore(ExDataServerProtocolCore);
	int result = wsExDbCli.Connect(ConnectServer, g_ConfigRead.server.GetExDBPort(), wMsg);
	if ( result == 0 )
	{
		g_Log.Add("ExDataServer connect fail IP [ %s ] PORT [ %d ]", ConnectServer, g_ConfigRead.server.GetExDBPort());
		return FALSE;
	}

	ExDataServerLogin();
	g_Log.Add("ExDataServer connect IP [ %s ] PORT [ %d ]", ConnectServer, g_ConfigRead.server.GetExDBPort());
	return TRUE;
}

BOOL GameMainServerCreate(DWORD sMsg, DWORD cMsg)
{
	g_Log.Add( "GameServer create PORT [ %d ]", g_ConfigRead.server.GetGameServerPort());
	return TRUE;
}

void GMServerMsgProc( WPARAM wParam, LPARAM lParam)
{

}

void GMClientMsgProc(WPARAM wParam, LPARAM lParam)
{
	switch ( lParam & 0xFFFF & 0xFFFF)
	{
		case 1:
			wsGServer.DataRecv(wParam);
			break;

		case 2:
			wsGServer.FDWRITE_MsgDataSend(wParam);
			break;

		case 32:
			break;

	}
}



void GMJoinClientMsgProc(WPARAM wParam, LPARAM lParam)
{
	switch ( lParam & 0xFFFF & 0xFFFF )
	{
		case 1:
			wsJServerCli.DataRecv();
			break;

		case 2:
			wsJServerCli.FDWRITE_MsgDataSend();
			break;

		case 32:
			g_Log.AddC(TColor::Red,  "Game server closed because Authentication server is closed.");
			gObjAllDisconnect();
			wsJServerCli.Close();
			break;
	}
}

void ExDataClientMsgProc(WPARAM wParam, LPARAM lParam)
{
	switch ( lParam & 0xFFFF & 0xFFFF )
	{
		case 1:
			wsExDbCli.DataRecv();
			break;

		case 2:
			wsExDbCli.FDWRITE_MsgDataSend();
			break;

		case 32:
			g_Log.AddC(TColor::Red,  "Gamer server closed because Data server is closed.");
			wsExDbCli.Close();

			for ( int i =0;i<g_ConfigRead.server.GetObjectMax();i++)
			{
				if ( gObjIsConnectedGP(i) == TRUE )
				{
					PMSG_FRIEND_STATE pMsg;
					
					pMsg.h.set((LPBYTE)&pMsg, 0xC4 , sizeof(pMsg) );
					pMsg.State = -4;

					IOCP.DataSend(i, (unsigned char*)&pMsg, sizeof(pMsg) );

					gObj[i].m_FriendServerOnline = FRIEND_SERVER_STATE_OFFLINE;

					if (gObj[i].m_PlayerData->m_bUsePartyMatching == true)
					{
						if (gObj[i].PartyNumber < 0)
						{
							GSProtocol.GCPartyDelUserSendNoMessage(i);
						}

						else
						{
							GSProtocol.CGPartyList(i);
						}
					}

					gObj[i].m_PlayerData->m_nPartyMatchingIndex = -1;
					gObj[i].m_PlayerData->m_bUsePartyMatching = false;
					gObj[i].m_PlayerData->m_bPartyMatchingLeader = false;
					gObj[i].m_PlayerData->m_nPartyMatchingMemberCount = 0;
					gObj[i].m_PlayerData->m_bUseGuildMatching = false;
				}
			}

			g_Log.Add("Error-L1 : Friend Server Down (State Send Ok)");
			g_Log.AddC(TColor::Red,  "Error-L1 : Friend Server Down (State Send Ok)");
			break;
	}
}




void GMDataClientMsgProc(WPARAM wParam, LPARAM lParam)
{
	switch ( lParam & 0xFFFF & 0xFFFF )
	{
		case 1:
			wsDataCli.DataRecv();
			break;

		case 2:
			wsDataCli.FDWRITE_MsgDataSend();
			break;

		case 32:
			wsDataCli.Close();
			g_Log.AddC(TColor::Red,  "Gamer server closed because Data server is closed.");
			break;
	}
}

void ReadCommonServerInfo()
{
	char szTemp[256];

	g_ConfigRead.ReadServerInfo(false);
	gUdpSoc.SendSet(g_ConfigRead.server.GetConnectServerIP(), g_ConfigRead.server.GetConnectServerPort());

	g_VipSystem.ReadFile("./IGC_VipSettings.xml");

	gGateC.Load(g_ConfigRead.GetPath("\\Warps\\IGC_GateSettings.xml"));

	if (!IsFile(gDirPath.GetNewPath("commonserver.cfg")))
	{
		g_Log.MsgBox("[commonserver.cfg] file not found");
	}

	g_ConfigRead.ReadConfig();

	gStalkProtocol = GetPrivateProfileInt("GameServerInfo", "StalkProtocol", 0, gDirPath.GetNewPath("commonserver.cfg"));
	GetPrivateProfileString("GameServerInfo", "StalkProtocolId", "0", gStalkProtocolId, 10, gDirPath.GetNewPath("commonserver.cfg"));

	if (gStalkProtocol != 0)
	{
		g_Log.Add("Stalk Protocol ID = '%s' ", gStalkProtocolId);
	}

	g_Log.Add("[Option] Connect Server IP(%s) / PORT(%d)", g_ConfigRead.server.GetConnectServerIP(), g_ConfigRead.server.GetConnectServerPort());

	CheckSumFileLoad(gDirPath.GetNewPath("CheckSum.dat"));

	OpenItemScript(g_ConfigRead.GetPath("\\Items\\IGC_ItemList.xml"));
	g_PentagramMixSystem.LoadMixNeedSourceScript(g_ConfigRead.GetPath("\\Items\\IGC_PentagramMixNeedSource.xml"));
	g_PentagramMixSystem.LoadJewelOptionScript(g_ConfigRead.GetPath("\\Items\\IGC_PentagramJewelOptionValue.xml"));
	g_PentagramSystem.LoadDropScript(g_ConfigRead.GetPath("\\Items\\IGC_PentagramItemDropRate.xml"));
	g_PentagramSystem.LoadJewelOutRate(g_ConfigRead.GetPath("\\Items\\IGC_PentagramJewelOutRate.xml"));
	g_PentagramSystem.LoadPentagramOptionScript(g_ConfigRead.GetPath("\\Items\\IGC_PentagramOption.xml"));

	LoadResetItemList(g_ConfigRead.GetPath("\\Items\\IGC_ResetItem.xml"));

	g_MasterLevelSkillTreeSystem.Load(g_ConfigRead.GetPath("IGC_MasterSkillTree.xml"));
	MagicDamageC.LogSkillList(g_ConfigRead.GetPath("\\Skills\\IGC_SkillList.xml"));

	SkillSpearHitBox.Load(g_ConfigRead.GetPath("\\Skills\\IGC_SkillSpear.hit"));
	SkillElectricSparkHitBox.Load(g_ConfigRead.GetPath("\\Skills\\IGC_SkillElect.hit"));

	gSetItemOption.LoadOptionInfo(g_ConfigRead.GetPath("\\Items\\IGC_ItemSetOption.xml"));
	gSetItemOption.LoadTypeInfo(g_ConfigRead.GetPath("\\Items\\IGC_ItemSetType.xml"));

	g_SocketOptionSystem.LoadOptionScript(g_ConfigRead.GetPath("\\SocketSystem\\IGC_SocketOption.xml"));

	ItemDrop.LoadFile(g_ConfigRead.GetPath("IGC_DropManager.xml"));
	
	SetItemDrop.LoadFile(g_ConfigRead.GetPath("IGC_SetItemDropManager.xml"));
	ItemDrop.LoadZenDropFile(g_ConfigRead.GetPath("IGC_ZenDrop.xml"));
	gObjUseSkill.LoadSkillInfo();
	gObjCalCharacter.Init();
	gclassObjAttack.Init();

	g_kJewelOfHarmonySystem.LoadScript(g_ConfigRead.GetPath("\\Items\\IGC_HarmonyItem_Option.xml"));
	g_kJewelOfHarmonySystem.LoadScriptOfSmelt(g_ConfigRead.GetPath("\\Items\\IGC_HarmonyItem_Smelt.xml"));
	g_kItemSystemFor380.Load380ItemOptionInfo(g_ConfigRead.GetPath("\\Items\\IGC_Item380Option.xml"));
	g_ItemAddOption.Load(g_ConfigRead.GetPath("\\Items\\IGC_ItemOptionManager.xml"));

	gMoveCommand.Load(g_ConfigRead.GetPath("\\Warps\\IGC_MoveReq.xml"));
	gMoveCommand.LoadMoveLevel(g_ConfigRead.GetPath("\\Warps\\IGC_MoveLevel.xml"));

	ConMember.Load("IGC_ConnectMember.xml");

	Marry.LoadFile(g_ConfigRead.GetPath("IGC_MarriageSystem.xml"));

	g_QuestInfo.LoadQuestInfo(g_ConfigRead.GetPath("IGC_ClassQuest.xml"));

	SwearFilter.LoadFile(g_ConfigRead.GetPath("IGC_ProhibitedWords.xml"));
	g_prohibitedSymbols.LoadSymbolFile(g_ConfigRead.GetPath("IGC_ProhibitedSymbols.xml"));
	g_CashShop.Initialize();

	g_NewsSystem.LoadFile(g_ConfigRead.GetPath("IGC_NewsSystem.xml"));
	g_GensSystem.LoadData(g_ConfigRead.GetPath("IGC_GensSystem.xml"));

	TMonsterSkillElement::LoadData(g_ConfigRead.GetPath("\\Monsters\\Skills\\IGC_Monster_Skill_Element.xml"));
	TMonsterSkillUnit::LoadData(g_ConfigRead.GetPath("\\Monsters\\Skills\\IGC_Monster_Skill_Unit.xml"));
	TMonsterSkillManager::LoadData(g_ConfigRead.GetPath("\\Monsters\\Skills\\IGC_Monster_Skill.xml"));
	TMonsterAIElement::LoadData(g_ConfigRead.GetPath("\\Monsters\\AI\\IGC_Monster_AI_Element.xml"));
	TMonsterAIAutomata::LoadData(g_ConfigRead.GetPath("\\Monsters\\AI\\IGC_Monster_AI_Automata.xml"));
	TMonsterAIUnit::LoadData(g_ConfigRead.GetPath("\\Monsters\\AI\\IGC_Monster_AI_Unit.xml"));
	TMonsterAIRule::LoadData(g_ConfigRead.GetPath("\\Monsters\\AI\\IGC_Monster_AI_Rule.xml"));
	TMonsterAIGroup::LoadData(g_ConfigRead.GetPath("\\Monsters\\AI\\IGC_Monster_AI_Group.xml"));

	if (g_ConfigRead.server.GetServerType() == SERVER_CASTLE)
	{
		g_Crywolf.LoadData(g_ConfigRead.GetPath("\\Events\\IGC_CryWolf.xml"));
	}	
	
	if (g_ConfigRead.server.GetServerType() != SERVER_CASTLE)
	{
		g_Kanturu.LoadData(g_ConfigRead.GetPath("\\Events\\Kanturu\\IGC_KanturuEvent.xml"));
		g_KanturuMonsterMng.LoadData(g_ConfigRead.GetPath("\\Events\\Kanturu\\IGC_KanturuEvent_Monster.xml"));
	}

	g_SkillAdditionInfo.Load();
	g_SkillSafeZone.LoadFile(g_ConfigRead.GetPath("\\Skills\\IGC_Skill_UseArea.xml"));
	g_BuffScript.Load(g_ConfigRead.GetPath("IGC_BuffEffectManager.xml"));

	gCreateCharacter = GetPrivateProfileInt("GameServerInfo", "CreateCharacter", 1, gDirPath.GetNewPath("commonserver.cfg"));
	gCharacterDeleteMinLevel = GetPrivateProfileInt("GameServerInfo", "CharacterDeleteMinLevel", 40, gDirPath.GetNewPath("commonserver.cfg"));

	// Guild Settingss
	g_iServerGroupGuildChatting = GetPrivateProfileInt("GameServerInfo","ServerGroupGuildChatting",0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iServerGroupUnionChatting = GetPrivateProfileInt("GameServerInfo","ServerGroupUnionChatting",0, gDirPath.GetNewPath("commonserver.cfg"));

	SpeedHackPlayerBlock = GetPrivateProfileInt("GameServerInfo", "SpeedHackPlayerBlock", 0, gDirPath.GetNewPath("commonserver.cfg"));
	gItemSerialCheck = GetPrivateProfileInt("GameServerInfo", "ItemSerialCheck", 1, gDirPath.GetNewPath("commonserver.cfg"));
	gSerialZeroCheck = GetPrivateProfileInt("GameServerInfo", "SerialZeroCheck", 1, gDirPath.GetNewPath("commonserver.cfg"));
	bCanTrade = GetPrivateProfileInt("GameServerInfo", "Trade", 1, gDirPath.GetNewPath("commonserver.cfg"));

	gLevelUpPointNormal = GetPrivateProfileInt("GameServerInfo", "LevelUpPointNormal", 5, gDirPath.GetNewPath("commonserver.cfg"));
	gLevelUpPointMGDL = GetPrivateProfileInt("GameServerInfo", "LevelUpPointMGDL", 7, gDirPath.GetNewPath("commonserver.cfg"));
	
	gItemDisapearTime = GetPrivateProfileInt("GameServerInfo", "ItemDisapearTime", 120, gDirPath.GetNewPath("commonserver.cfg"));

	if(bCanTrade != 0)
	{
		g_Log.Add("[Option] Trade on");
	}
	else
	{
		g_Log.Add("[Option] Trade off");
	}

	bCanChaosBox = GetPrivateProfileInt("Common","IsChaosMachine",0, g_ConfigRead.GetPath("IGC_ChaosBox.ini"));

	gMonsterHp = GetPrivateProfileInt("GameServerInfo", "MonsterHp", 0, gDirPath.GetNewPath("commonserver.cfg"));

	if(gMonsterHp != 0)
	{
		g_Log.Add("[Option] Monster HP %d%% Down", gMonsterHp);
	}
	gLootingTime = GetPrivateProfileInt("GameServerInfo", "LootingTime", 3, gDirPath.GetNewPath("commonserver.cfg"));

	if(gLootingTime != 0)
	{
		g_Log.Add("[Option] Auto Looting Time Set : %d", gLootingTime);
	}

	gItemDropPer = GetPrivateProfileInt("GameServerInfo", "ItemDropPer", 10, gDirPath.GetNewPath("commonserver.cfg"));
	gExcDropRate = GetPrivateProfileInt("GameServerInfo", "ExcItemDropPer", 2000, gDirPath.GetNewPath("commonserver.cfg"));
	gSkillDropRate = GetPrivateProfileInt("GameServerInfo", "ItemSkillDrop", 6, gDirPath.GetNewPath("commonserver.cfg"));
	gLuckDropRate = GetPrivateProfileInt("GameServerInfo", "ItemLuckDrop", 4, gDirPath.GetNewPath("commonserver.cfg"));
	gExcSkillDropRate = GetPrivateProfileInt("GameServerInfo", "ExcItemSkillDrop", 100, gDirPath.GetNewPath("commonserver.cfg"));
	gExcLuckDropRate = GetPrivateProfileInt("GameServerInfo", "ExcItemLuckDrop", 1, gDirPath.GetNewPath("commonserver.cfg"));
	g_Log.Add("[Option] Item drop rate : %d%%", gItemDropPer);
	gZenDurationTime = GetPrivateProfileInt("GameServerInfo","ZenDurationTime",30, gDirPath.GetNewPath("commonserver.cfg"));
	gEventChipEvent = GetPrivateProfileInt("GameServerInfo","EventChipEvent",0, gDirPath.GetNewPath("commonserver.cfg"));
	gBoxOfGoldDropRate = GetPrivateProfileInt("GameServerInfo","BoxOfGoldDropRate",2, gDirPath.GetNewPath("commonserver.cfg"));
	g_ItemDropRateForBoxOfGold = GetPrivateProfileInt("GameServerInfo","ItemDropRateForBoxOfGold",2, gDirPath.GetNewPath("commonserver.cfg"));
	g_EventChipDropRateForBoxOfGold = GetPrivateProfileInt("GameServerInfo","EventChipDropRateForBoxOfGold",2, gDirPath.GetNewPath("commonserver.cfg"));
	gSilverBoxDropRate = GetPrivateProfileInt("GameServerInfo","SilverBoxDropRate",10, gDirPath.GetNewPath("commonserver.cfg"));
	gGoldBoxDropRate = GetPrivateProfileInt("GameServerInfo","GoldBoxDropRate", 10, gDirPath.GetNewPath("commonserver.cfg"));

	gSetPartyExp2 = GetPrivateProfileInt("GameServerInfo", "SetParty2ExpBonus", 120, gDirPath.GetNewPath("commonserver.cfg"));
	gSetPartyExp3 = GetPrivateProfileInt("GameServerInfo", "SetParty3ExpBonus", 230, gDirPath.GetNewPath("commonserver.cfg"));
	gSetPartyExp4 = GetPrivateProfileInt("GameServerInfo", "SetParty4ExpBonus", 270, gDirPath.GetNewPath("commonserver.cfg"));
	gSetPartyExp5 = GetPrivateProfileInt("GameServerInfo", "SetParty5ExpBonus", 300, gDirPath.GetNewPath("commonserver.cfg"));

	gPartyExp1 = GetPrivateProfileInt("GameServerInfo", "NormalParty1ExpBonus", 150, gDirPath.GetNewPath("commonserver.cfg"));
	gPartyExp2 = GetPrivateProfileInt("GameServerInfo", "NormalParty2ExpBonus", 160, gDirPath.GetNewPath("commonserver.cfg"));
	gPartyExp3 = GetPrivateProfileInt("GameServerInfo", "NormalParty3ExpBonus", 180, gDirPath.GetNewPath("commonserver.cfg"));
	gPartyExp4 = GetPrivateProfileInt("GameServerInfo", "NormalParty4ExpBonus", 200, gDirPath.GetNewPath("commonserver.cfg"));
	gPartyExp5 = GetPrivateProfileInt("GameServerInfo", "NormalParty5ExpBonus", 220, gDirPath.GetNewPath("commonserver.cfg"));

	IsMultiWareHouse = GetPrivateProfileIntA("GameServerInfo","IsMultiWareHouse",1,gDirPath.GetNewPath("commonserver.cfg"));
	g_WareHouseCount = GetPrivateProfileInt("GameServerInfo", "MultiWareHouseCount", 3, gDirPath.GetNewPath("commonserver.cfg"));

	bIsIgnorePacketSpeedHackDetect = GetPrivateProfileInt("GameServerInfo","IsIgnorePacketHackDetect",0, gDirPath.GetNewPath("commonserver.cfg"));

	// Ring of Transform
	gIsItemDropRingOfTransform = GetPrivateProfileInt("GameServerInfo","IsItemDropRingOfTransform",0, gDirPath.GetNewPath("commonserver.cfg"));
	gItemDropRingOfTransform = GetPrivateProfileInt("GameServerInfo","ItemDropRingOfTransform",1, gDirPath.GetNewPath("commonserver.cfg"));

	// HAck Log Server
	gWriteChatLog = GetPrivateProfileInt("GameServerInfo","WriteChatLog",0, gDirPath.GetNewPath("commonserver.cfg"));

	// Hack Settinhs
	gDisconnectHackUser = GetPrivateProfileInt("GameServerInfo","DisconnectHackUser",0, gDirPath.GetNewPath("commonserver.cfg"));
	GetPrivateProfileString("GameServerInfo", "DecTimePerAttackSpeed", "5.33", szTemp, 5, gDirPath.GetNewPath("commonserver.cfg"));
	gDecTimePerAttackSpeed = atof(szTemp);

	// Devil Square Event
	gDQChaosSuccessRateLevel1 = GetPrivateProfileInt("EventMixes","DQChaosSuccessRateLevel1",75, g_ConfigRead.GetPath("IGC_ChaosBox.ini"));
	gDQChaosSuccessRateLevel2 = GetPrivateProfileInt("EventMixes","DQChaosSuccessRateLevel2",70, g_ConfigRead.GetPath("IGC_ChaosBox.ini"));
	gDQChaosSuccessRateLevel3 = GetPrivateProfileInt("EventMixes","DQChaosSuccessRateLevel3",65, g_ConfigRead.GetPath("IGC_ChaosBox.ini"));
	gDQChaosSuccessRateLevel4 = GetPrivateProfileInt("EventMixes","DQChaosSuccessRateLevel4",60, g_ConfigRead.GetPath("IGC_ChaosBox.ini"));
	gDQChaosSuccessRateLevel5 = GetPrivateProfileInt("EventMixes","DQChaosSuccessRateLevel5",55, g_ConfigRead.GetPath("IGC_ChaosBox.ini"));
	gDQChaosSuccessRateLevel6 = GetPrivateProfileInt("EventMixes","DQChaosSuccessRateLevel6",50, g_ConfigRead.GetPath("IGC_ChaosBox.ini"));
	gDQChaosSuccessRateLevel7 = GetPrivateProfileInt("EventMixes","DQChaosSuccessRateLevel7",45, g_ConfigRead.GetPath("IGC_ChaosBox.ini"));

	// Skill Log
	gWriteSkillLog = GetPrivateProfileInt("GameServerInfo","WriteSkillLog",0, gDirPath.GetNewPath("commonserver.cfg"));

	// Marlon Teleport
	gQuestNPCTeleportTime = GetPrivateProfileInt("GameServerInfo","QuestNPCTeleportTime",900, gDirPath.GetNewPath("commonserver.cfg"));

	// Mark Lord
	g_iMarkOfTheLord = GetPrivateProfileInt("GameServerInfo","MarkOfTheLord",0, gDirPath.GetNewPath("commonserver.cfg"));

	// Lucky Coin
	g_iLuckyCoin = GetPrivateProfileInt("GameServerInfo","LuckyCoinDrop",0, gDirPath.GetNewPath("commonserver.cfg"));

	gIsDropCmd = GetPrivateProfileInt("GameServerInfo", "DropCommand", 1, gDirPath.GetNewPath("commonserver.cfg"));

	g_MaxStrength = GetPrivateProfileInt("GameServerInfo", "MaxStrength", 65535, gDirPath.GetNewPath("commonserver.cfg"));
	g_MaxAgility = GetPrivateProfileInt("GameServerInfo", "MaxAgility", 65535, gDirPath.GetNewPath("commonserver.cfg"));
	g_MaxVitality = GetPrivateProfileInt("GameServerInfo", "MaxVitality", 65535, gDirPath.GetNewPath("commonserver.cfg"));
	g_MaxEnergy = GetPrivateProfileInt("GameServerInfo", "MaxEnergy", 65535, gDirPath.GetNewPath("commonserver.cfg"));
	g_MaxCommand = GetPrivateProfileInt("GameServerInfo", "MaxCommand", 65535, gDirPath.GetNewPath("commonserver.cfg"));

	// Ribbon Box Event
	g_bRibbonBoxEvent = GetPrivateProfileInt("GameServerInfo","RibbonBoxEvent",0, gDirPath.GetNewPath("commonserver.cfg"));

		// Red Ribbon Box
		g_iRedRibbonBoxDropLevelMin = GetPrivateProfileInt("GameServerInfo","RedRibbonBoxDropLv_Min",0, gDirPath.GetNewPath("commonserver.cfg"));
		g_iRedRibbonBoxDropLevelMax = GetPrivateProfileInt("GameServerInfo","RedRibbonBoxDropLv_Max",0, gDirPath.GetNewPath("commonserver.cfg"));
		g_iRedRibbonBoxDropRate = GetPrivateProfileInt("GameServerInfo","RedRibbonBoxDropRate",0, gDirPath.GetNewPath("commonserver.cfg"));

		// Green Ribbon Box
		g_iGreenRibbonBoxDropLevelMin = GetPrivateProfileInt("GameServerInfo","GreenRibbonBoxDropLv_Min",0, gDirPath.GetNewPath("commonserver.cfg"));
		g_iGreenRibbonBoxDropLevelMax = GetPrivateProfileInt("GameServerInfo","GreenRibbonBoxDropLv_Max",0, gDirPath.GetNewPath("commonserver.cfg"));
		g_iGreenRibbonBoxDropRate = GetPrivateProfileInt("GameServerInfo","GreenRibbonBoxDropRate",0, gDirPath.GetNewPath("commonserver.cfg"));

		// Blue Ribbon Box
		g_iBlueRibbonBoxDropLevelMin = GetPrivateProfileInt("GameServerInfo","BlueRibbonBoxDropLv_Min",0, gDirPath.GetNewPath("commonserver.cfg"));
		g_iBlueRibbonBoxDropLevelMax = GetPrivateProfileInt("GameServerInfo","BlueRibbonBoxDropLv_Max",0, gDirPath.GetNewPath("commonserver.cfg"));
		g_iBlueRibbonBoxDropRate = GetPrivateProfileInt("GameServerInfo","BlueRibbonBoxDropRate",0, gDirPath.GetNewPath("commonserver.cfg"));


	// Chocolate Event
	g_bChocolateBoxEvent = GetPrivateProfileInt("GameServerInfo","ChocolateEvent",0, gDirPath.GetNewPath("commonserver.cfg"));

		// Pink Chocolate Box
		g_iPinkChocolateBoxDropLevelMin = GetPrivateProfileInt("GameServerInfo","PinkChocolateBoxDropLv_Min",0, gDirPath.GetNewPath("commonserver.cfg"));
		g_iPinkChocolateBoxDropLevelMax = GetPrivateProfileInt("GameServerInfo","PinkChocolateBoxDropLv_Max",0, gDirPath.GetNewPath("commonserver.cfg"));
		g_iPinkChocolateBoxDropRate = GetPrivateProfileInt("GameServerInfo","PinkChocolateBoxDropRate",0, gDirPath.GetNewPath("commonserver.cfg"));

		// Red Chocolate Box
		g_iRedChocolateBoxDropLevelMin = GetPrivateProfileInt("GameServerInfo","RedChocolateBoxDropLv_Min",0, gDirPath.GetNewPath("commonserver.cfg"));
		g_iRedChocolateBoxDropLevelMax = GetPrivateProfileInt("GameServerInfo","RedChocolateBoxDropLv_Max",0, gDirPath.GetNewPath("commonserver.cfg"));
		g_iRedChocolateBoxDropRate = GetPrivateProfileInt("GameServerInfo","RedChocolateBoxDropRate",0, gDirPath.GetNewPath("commonserver.cfg"));

		// Blue Chocolate Box
		g_iBlueChocolateBoxDropLevelMin = GetPrivateProfileInt("GameServerInfo","BlueChocolateBoxDropLv_Min",0, gDirPath.GetNewPath("commonserver.cfg"));
		g_iBlueChocolateBoxDropLevelMax = GetPrivateProfileInt("GameServerInfo","BlueChocolateBoxDropLv_Max",0, gDirPath.GetNewPath("commonserver.cfg"));
		g_iBlueChocolateBoxDropRate = GetPrivateProfileInt("GameServerInfo","BlueChocolateBoxDropRate",0, gDirPath.GetNewPath("commonserver.cfg"));

	// Candy Event
	g_bCandyBoxEvent = GetPrivateProfileInt("GameServerInfo","CandyBoxEvent",0, gDirPath.GetNewPath("commonserver.cfg"));

		// LightPurple Candy Box
		g_iLightPurpleCandyBoxDropLevelMin = GetPrivateProfileInt("GameServerInfo","LightPurpleCandyBoxDropLv_Min",0, gDirPath.GetNewPath("commonserver.cfg"));
		g_iLightPurpleCandyBoxDropLevelMax = GetPrivateProfileInt("GameServerInfo","LightPurpleCandyBoxDropLv_Max",0, gDirPath.GetNewPath("commonserver.cfg"));
		g_iLightPurpleCandyBoxDropRate = GetPrivateProfileInt("GameServerInfo","LightPurpleCandyBoxDropRate",0, gDirPath.GetNewPath("commonserver.cfg"));

		// Vermilion Candy Box
		g_iVermilionCandyBoxDropLevelMin = GetPrivateProfileInt("GameServerInfo","VermilionCandyBoxDropLv_Min",0, gDirPath.GetNewPath("commonserver.cfg"));
		g_iVermilionCandyBoxDropLevelMax = GetPrivateProfileInt("GameServerInfo","VermilionCandyBoxDropLv_Max",0, gDirPath.GetNewPath("commonserver.cfg"));
		g_iVermilionCandyBoxDropRate = GetPrivateProfileInt("GameServerInfo","VermilionCandyBoxDropRate",0, gDirPath.GetNewPath("commonserver.cfg"));

		// DeepBlue Candy Box
		g_iDeepBlueCandyBoxDropLevelMin = GetPrivateProfileInt("GameServerInfo","DeepBlueCandyBoxDropLv_Min",0, gDirPath.GetNewPath("commonserver.cfg"));
		g_iDeepBlueCandyBoxDropLevelMax = GetPrivateProfileInt("GameServerInfo","DeepBlueCandyBoxDropLv_Max",0, gDirPath.GetNewPath("commonserver.cfg"));
		g_iDeepBlueCandyBoxDropRate = GetPrivateProfileInt("GameServerInfo","DeepBlueCandyBoxDropRate",0, gDirPath.GetNewPath("commonserver.cfg"));
	
	
	// KUNDUN Settings
	giKundunRefillHPSec = GetPrivateProfileInt("GameServerInfo","KundunRefillHPSec",500, gDirPath.GetNewPath("commonserver.cfg"));
	giKundunRefillHP = GetPrivateProfileInt("GameServerInfo","KundunRefillHP",10000, gDirPath.GetNewPath("commonserver.cfg"));
	giKundunRefillHPTime = GetPrivateProfileInt("GameServerInfo","KundunRefillHPTime",600, gDirPath.GetNewPath("commonserver.cfg"));
	giKundunHPLogSaveTime = GetPrivateProfileInt("GameServerInfo","KundunHPLogSaveTime",120, gDirPath.GetNewPath("commonserver.cfg"));

	// Fenrir Settings
	g_bFenrirStuffItemDrop = GetPrivateProfileInt("GameServerInfo","FenrirStuffItemDrop", 0, gDirPath.GetNewPath("commonserver.cfg"));

		// Fenrir Stuff 1
		g_iFenrirStuff_01_DropLv_Min = GetPrivateProfileInt("GameServerInfo","FenrirStuff_01_DropLv_Min", 0, gDirPath.GetNewPath("commonserver.cfg"));
		g_iFenrirStuff_01_DropLv_Max = GetPrivateProfileInt("GameServerInfo","FenrirStuff_01_DropLv_Max", 0, gDirPath.GetNewPath("commonserver.cfg"));
		g_iFenrirStuff_01_DropMap = GetPrivateProfileInt("GameServerInfo","FenrirStuff_01_DropMap", 0, gDirPath.GetNewPath("commonserver.cfg"));
		g_iFenrirStuff_01_DropRate = GetPrivateProfileInt("GameServerInfo","FenrirStuff_01_DropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));

		// Fenrir Stuff 2
		g_iFenrirStuff_02_DropLv_Min = GetPrivateProfileInt("GameServerInfo","FenrirStuff_02_DropLv_Min", 0, gDirPath.GetNewPath("commonserver.cfg"));
		g_iFenrirStuff_02_DropLv_Max = GetPrivateProfileInt("GameServerInfo","FenrirStuff_02_DropLv_Max", 0, gDirPath.GetNewPath("commonserver.cfg"));
		g_iFenrirStuff_02_DropMap = GetPrivateProfileInt("GameServerInfo","FenrirStuff_02_DropMap", 0, gDirPath.GetNewPath("commonserver.cfg"));
		g_iFenrirStuff_02_DropRate = GetPrivateProfileInt("GameServerInfo","FenrirStuff_02_DropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));

		// Fenrir Stuff 3
		g_iFenrirStuff_03_DropLv_Min = GetPrivateProfileInt("GameServerInfo","FenrirStuff_03_DropLv_Min", 0, gDirPath.GetNewPath("commonserver.cfg"));
		g_iFenrirStuff_03_DropLv_Max = GetPrivateProfileInt("GameServerInfo","FenrirStuff_03_DropLv_Max", 0, gDirPath.GetNewPath("commonserver.cfg"));
		g_iFenrirStuff_03_DropMap = GetPrivateProfileInt("GameServerInfo","FenrirStuff_03_DropMap", 0, gDirPath.GetNewPath("commonserver.cfg"));
		g_iFenrirStuff_03_DropRate = GetPrivateProfileInt("GameServerInfo","FenrirStuff_03_DropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));

		// Fenrir Durability Settings
		g_iFenrirRepairRate = GetPrivateProfileInt("GameServerInfo","FenrirRepairRate", 4000, gDirPath.GetNewPath("commonserver.cfg"));
		g_iFenrirDefaultMaxDurSmall = GetPrivateProfileInt("GameServerInfo","FenrirDefaultMaxDurSmall", 200, gDirPath.GetNewPath("commonserver.cfg"));
		g_iFenrirElfMaxDurSmall = GetPrivateProfileInt("GameServerInfo","FenrirElfMaxDurSmall", 160, gDirPath.GetNewPath("commonserver.cfg"));

		// Fenrir Mix Rate
		g_iFenrir_01Level_MixRate = GetPrivateProfileInt("GameServerInfo","Fenrir_01Level_MixRate", 70, gDirPath.GetNewPath("commonserver.cfg"));
		g_iFenrir_02Level_MixRate = GetPrivateProfileInt("GameServerInfo","Fenrir_02Level_MixRate", 50, gDirPath.GetNewPath("commonserver.cfg"));
		g_iFenrir_03Level_MixRate = GetPrivateProfileInt("GameServerInfo","Fenrir_03Level_MixRate", 30, gDirPath.GetNewPath("commonserver.cfg"));

	// Hallowin Event
	g_bHallowinDayEventOn = GetPrivateProfileInt("GameServerInfo","HallowinEventOn",0, gDirPath.GetNewPath("commonserver.cfg"));

		g_iHallowinDayEventItemDropRate = GetPrivateProfileInt("GameServerInfo","HallowinEventPumpkinOfLuckDropRate",100, gDirPath.GetNewPath("commonserver.cfg"));
		g_iHallowinDayEventJOLBlessDropRate = GetPrivateProfileInt("GameServerInfo","HallowinEventJOLBlessDropRate",10, gDirPath.GetNewPath("commonserver.cfg"));
		g_iHallowinDayEventJOLAngerDropRaTe = GetPrivateProfileInt("GameServerInfo","HallowinEventJOLAngerDropRate",15, gDirPath.GetNewPath("commonserver.cfg"));
		g_iHallowinDayEventJOLScreamDropRate = GetPrivateProfileInt("GameServerInfo","HallowinEventJOLScreamDropRate",15, gDirPath.GetNewPath("commonserver.cfg"));
		g_iHallowinDayEventJOLFoodDropRate = GetPrivateProfileInt("GameServerInfo","HallowinEventJOLFoodDropRate",30, gDirPath.GetNewPath("commonserver.cfg"));
		g_iHallowinDayEventJOLDrinkDropRate = GetPrivateProfileInt("GameServerInfo","HallowinEventJOLDrinkDropRate",25, gDirPath.GetNewPath("commonserver.cfg"));
		g_iHallowinDayEventJOLPolymorphRingDropRate = GetPrivateProfileInt("GameServerInfo","HallowinEventJOLPolymorphRingDropRate", 5, gDirPath.GetNewPath("commonserver.cfg"));

		// SantaRing Item Drop
		g_bSantaPolymorphRingDropOn = GetPrivateProfileInt("GameServerInfo","SantaPolymorphRingDropOn",0, gDirPath.GetNewPath("commonserver.cfg"));
		g_iSantaPolymorphRingDropRate = GetPrivateProfileInt("GameServerInfo","SantaPolymorphRingDropRate",20, gDirPath.GetNewPath("commonserver.cfg"));

		// Condor Flame Drop
		g_iCondorFlameDropRate = GetPrivateProfileInt("GameServerInfo","CondorFlameDropRate",20, gDirPath.GetNewPath("commonserver.cfg"));
		
		// Shadow Phantom NPC
		g_iShadowPhantomMaxLevel = GetPrivateProfileInt("GameServerInfo","ShadowPhantomMaxLevel",220, gDirPath.GetNewPath("commonserver.cfg"));

	// Battle Soccer
	gEnableBattleSoccer = GetPrivateProfileInt("GameServerInfo","EnableBattleSoccer", 1, gDirPath.GetNewPath("commonserver.cfg"));

	// NPC Talk
	gEnableEventNPCTalk = GetPrivateProfileInt("GameServerInfo","EnableEventNPCTalk", 0, gDirPath.GetNewPath("commonserver.cfg"));

	// Server Division
	gEnableServerDivision = GetPrivateProfileInt("GameServerInfo","EnableServerDivision", 0, gDirPath.GetNewPath("commonserver.cfg"));

	// COMMANDS Init ( /make / Create /trace etc...)
	cManager.Init();

	// Penetration Skill
	gEnableCheckPenetrationSkill = GetPrivateProfileInt("GameServerInfo","EnableCheckPenetrationSkill", 1, gDirPath.GetNewPath("commonserver.cfg"));

	// Shield System
	g_ShieldSystemOn = GetPrivateProfileInt("GameServerInfo","ShieldSystemOn", 0, gDirPath.GetNewPath("commonserver.cfg"));

		g_iDamageDevideToSDRate = GetPrivateProfileInt("GameServerInfo","DamageDevideToSD", 90, gDirPath.GetNewPath("commonserver.cfg"));
		g_iDamageDevideToHPRate = GetPrivateProfileInt("GameServerInfo","DamageDevideToHP", 10, gDirPath.GetNewPath("commonserver.cfg"));
		g_fSuccessAttackRateOption = (double)(GetPrivateProfileInt("GameServerInfo","SuccessAttackRateOption", 20000, gDirPath.GetNewPath("commonserver.cfg")) / 10000 );
		g_iSDChargingOption = GetPrivateProfileInt("GameServerInfo","SDChargingOption", 1, gDirPath.GetNewPath("commonserver.cfg"));
		g_iConstNumberOfShieldPoint = GetPrivateProfileInt("GameServerInfo","ConstNumberOfShieldPoint", 20, gDirPath.GetNewPath("commonserver.cfg"));
		g_ShieldAutoRefillOn = GetPrivateProfileInt("GameServerInfo","ShieldAutoRefillOn", 0, gDirPath.GetNewPath("commonserver.cfg"));
		g_ShieldAutoRefillOnSafeZone = GetPrivateProfileInt("GameServerInfo","ShieldAutoRefilOnSafeZone", 1, gDirPath.GetNewPath("commonserver.cfg"));

	// Compund Potion
	g_CompoundPotionDropOn = GetPrivateProfileInt("GameServerInfo","CompoundPotionDropOn", 0, gDirPath.GetNewPath("commonserver.cfg"));

		g_iCompoundPotionLv1DropRate = GetPrivateProfileInt("GameServerInfo","CompoundPotionLv1DropRate", 100, gDirPath.GetNewPath("commonserver.cfg"));
		g_iCompoundPotionLv2DropRate = GetPrivateProfileInt("GameServerInfo","CompoundPotionLv2DropRate", 80, gDirPath.GetNewPath("commonserver.cfg"));
		g_iCompoundPotionLv3DropRate = GetPrivateProfileInt("GameServerInfo","CompoundPotionLv3DropRate", 50, gDirPath.GetNewPath("commonserver.cfg"));

		g_iCompoundPotionLv1DropLevel = GetPrivateProfileInt("GameServerInfo","CompoundPotionLv1DropLevel", 68, gDirPath.GetNewPath("commonserver.cfg"));
		g_iCompoundPotionLv2DropLevel = GetPrivateProfileInt("GameServerInfo","CompoundPotionLv2DropLevel", 96, gDirPath.GetNewPath("commonserver.cfg"));
		g_iCompoundPotionLv3DropLevel = GetPrivateProfileInt("GameServerInfo","CompoundPotionLv3DropLevel", 118, gDirPath.GetNewPath("commonserver.cfg"));

	// Shile System Combo
	g_bShieldComboMissOptionOn = GetPrivateProfileInt("GameServerInfo","ShieldComboMissOptionOn", 0, gDirPath.GetNewPath("commonserver.cfg"));

	// Shield Potions
	g_iShieldPotionLv1MixSuccessRate = GetPrivateProfileInt("GameServerInfo","ShieldPotionLv1MixSuccessRate", 50, gDirPath.GetNewPath("commonserver.cfg"));
	g_iShieldPotionLv1MixMoney = GetPrivateProfileInt("GameServerInfo","ShieldPotionLv1MixMoney", 100000, gDirPath.GetNewPath("commonserver.cfg"));

	g_iShieldPotionLv2MixSuccessRate = GetPrivateProfileInt("GameServerInfo","ShieldPotionLv2MixSuccessRate", 30, gDirPath.GetNewPath("commonserver.cfg"));
	g_iShieldPotionLv2MixMoney = GetPrivateProfileInt("GameServerInfo","ShieldPotionLv2MixMoney", 500000, gDirPath.GetNewPath("commonserver.cfg"));

	g_iShieldPotionLv3MixSuccessRate = GetPrivateProfileInt("GameServerInfo","ShieldPotionLv3MixSuccessRate", 30, gDirPath.GetNewPath("commonserver.cfg"));
	g_iShieldPotionLv3MixMoney = GetPrivateProfileInt("GameServerInfo","ShieldPotionLv3MixMoney", 1000000, gDirPath.GetNewPath("commonserver.cfg"));
	
	// Shield GAGE
	g_iShieldGageConstA = GetPrivateProfileInt("GameServerInfo","ShieldGageConstA", 12, gDirPath.GetNewPath("commonserver.cfg"));
	g_iShieldGageConstB = GetPrivateProfileInt("GameServerInfo","ShieldGageConstB", 30, gDirPath.GetNewPath("commonserver.cfg"));

	char szName[100];
	char szServer[100];

	strcpy(szName, "iDev.Games");
	strcpy(szServer, " Licensed: -PlatinumVIP- ");

		wsprintf(szTemp, "[%s] [%d] [%s] [%d] [%c.%c%c.%c%c] [%s] [%s][%s]", GAMESERVER_VERSION, g_ConfigRead.server.GetGameServerCode(), g_ConfigRead.server.GetServerName(), Lang.GetDefaultLangID(), ::szClientVersion[0], szClientVersion[1], szClientVersion[2], szClientVersion[3], szClientVersion[4], szGameServerExeSerial, szName, szServer);

		if (g_ConfigRead.server.GetServerType() == SERVER_CASTLE)
		{
			strcat(szTemp, " [ Siege Server]");
		}

		else if (g_ConfigRead.server.GetServerType() == SERVER_ARCA)
		{
			strcat(szTemp, " [Arca War]");
		}

		else if (g_ConfigRead.server.GetServerType() == SERVER_MARKET)
		{
			strcat(szTemp, " [Market Server]");
		}
		
		g_Log.Add("User Object Size = %d", sizeof(OBJECTSTRUCT));

		SetWindowText(ghWnd, szTemp);

	


	// Item Bag Load

	// Eledorado Event Start
	gEledoradoEvent.Init();

	gPacketCheckSum.Init();
	g_NewPVP.LoadData();

	gDoPShopOpen = GetPrivateProfileInt("GameServerInfo","PersonalShopOpen", 0, gDirPath.GetNewPath("commonserver.cfg"));

	ReadEventInfo(MU_EVENT_ALL );

	// AutoRecuperation Level
	g_iUseCharacterAutoRecuperationSystem = GetPrivateProfileInt("GameServerInfo","UseCharacterAutoRecuperationSystem", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iCharacterRecuperationMaxLevel = GetPrivateProfileInt("GameServerInfo","CharacterRecuperationMaxLevel", 100, gDirPath.GetNewPath("commonserver.cfg"));

	// Skill Check Settings
	g_iSkillDistanceCheck = GetPrivateProfileInt("GameServerInfo","SkillDistanceCheck", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iSkillDistanceCheckTemp = GetPrivateProfileInt("GameServerInfo","SkillDistanceCheckTemp", 2, gDirPath.GetNewPath("commonserver.cfg"));
	g_iSkillDistanceKick = GetPrivateProfileInt("GameServerInfo","SkillDistanceKick", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iSkillDistanceKickCount = GetPrivateProfileInt("GameServerInfo","SkillDistanceKickCount", 5, gDirPath.GetNewPath("commonserver.cfg"));
	g_iSkillDiatanceKickCheckTime = GetPrivateProfileInt("GameServerInfo","SkillDistanceKickCheckTime", 10, gDirPath.GetNewPath("commonserver.cfg"));

	gAttackSpeedTimeLimit = GetPrivateProfileInt("GameServerInfo", "AttackSpeedTimeLimit", 800, gDirPath.GetNewPath("commonserver.cfg"));
	bIsIgnorePacketSpeedHackDetect = GetPrivateProfileInt("GameServerInfo", "IsIgnorePacketHackDetect", 0, gDirPath.GetNewPath("commonserver.cfg"));
	gHackCheckCount = GetPrivateProfileInt("GameServerInfo", "HackCheckCount", 5, gDirPath.GetNewPath("commonserver.cfg"));
	gMinimumAttackSpeedTime = GetPrivateProfileInt("GameServerInfo", "MinimumAttackSpeedTime", 200, gDirPath.GetNewPath("commonserver.cfg"));
	gDetectedHackKickCount = GetPrivateProfileInt("GameServerInfo", "DetectedHackKickCount", 10, gDirPath.GetNewPath("commonserver.cfg"));
	gIsKickDetecHackCountLimit = GetPrivateProfileInt("GameServerInfo", "IsKickDetecHackCountLimit", 0, gDirPath.GetNewPath("commonserver.cfg"));
	gSpeedHackPenalty = GetPrivateProfileInt("GameServerInfo","SpeedHackPenalty",0, gDirPath.GetNewPath("commonserver.cfg"));
	g_ChaosCard.Load(g_ConfigRead.GetPath("\\ChaosCards\\IGC_ChaosCard.xml"));
	g_ChaosCardMini.Load(g_ConfigRead.GetPath("\\ChaosCards\\IGC_ChaosCard_Mini.xml"));
	g_ChaosCardRare.Load(g_ConfigRead.GetPath("\\ChaosCards\\IGC_ChaosCard_Rare.xml"));
	g_ChaosCardGold.Load(g_ConfigRead.GetPath("\\ChaosCards\\IGC_ChaosCard_Gold.xml"));

	g_GamblingItemBag.GamblingItemBagInit(g_ConfigRead.GetPath("\\Events\\IGC_GamblingLottery.xml"));
	g_LastManStanding.LoadConfig(g_ConfigRead.GetPath("\\Events\\IGC_LastManStanding.xml"));
	g_MapAttr.LoadFile(g_ConfigRead.GetPath("IGC_MapAttribute.xml"));
	g_KillCountMng.ReadConfig(g_ConfigRead.GetPath("IGC_MonsterKillCount.xml"));
	g_MonsterRegenSystem.LoadScript(g_ConfigRead.GetPath("IGC_MonsterGroupRegen.xml"));
	g_ExpManager.LoadScript(g_ConfigRead.GetPath("IGC_ExpSystem.xml"));
	g_BagManager.InitBagManager();

	g_LuckyItemManager.LoadLuckyItemInfo(g_ConfigRead.GetPath("\\Items\\IGC_LuckyItemInfo_First.xml"));
	g_LuckyItemManager.LoadLuckyItemInfo(g_ConfigRead.GetPath("\\Items\\IGC_LuckyItemInfo_Second.xml"));

	g_SkillUseTime.LoadConfig(g_ConfigRead.GetPath("IGC_Common.ini"));
	g_SkillUseTime.LoadFile(g_ConfigRead.GetPath("IGC_SkillUseTime.xml"));

	g_ItemOptionTypeMng.LoadScript(g_ConfigRead.GetPath("\\Items\\IGC_ExcellentOptions.xml"));
	g_BonusEvent.LoadFile(g_ConfigRead.GetPath("\\Events\\IGC_BonusEvent.xml"));

	g_CAppointItemDrop.LoadAppointItemDropScript();
	g_ItemOptionTypeMng.LoadAccessoryItemOptionScript(g_ConfigRead.GetPath("\\Items\\IGC_AccessoryItem.xml"));

	g_MineSystem.LoadMineingDropScript();

	g_CMuunSystem.LoadScriptMuunSystemInfo(g_ConfigRead.GetPath("\\MuunSystem\\IGC_MuunInfo.xml"));
	g_CMuunSystem.LoadScriptMuunSystemOption(g_ConfigRead.GetPath("\\MuunSystem\\IGC_MuunOption.xml"));
	g_CMuunSystem.LoadScriptMuunExchange(g_ConfigRead.GetPath("\\MuunSystem\\IGC_MuunExchange.xml"));
	
	g_StatSpec.LoadFile(g_ConfigRead.GetPath("IGC_StatSpecialize.xml"));
	g_SkillSpec.LoadScript();

	g_UnityBattleField.LoadData(g_ConfigRead.GetPath("IGC_BattleCore.ini"));
}

struct PMSG_SERVERINFO
{
	PBMSG_HEAD h;	// C1:01
	short ServerCode;	// 4F
	BYTE Percent;	// 6
	BYTE PlayType;
	short UserCount;	// 8
	short AccountCount;	// A
	short MaxUserCount;	// E
};

void GameServerInfoSend()
{
	PMSG_SERVERINFO pMsg;

	while (true)
	{
		if ( gDisconnect == TRUE )
		{
			return;
		}

		pMsg.h.c = 0xC1;
		pMsg.h.headcode = 0x01;
		pMsg.h.size = sizeof( pMsg );

		if ( GSInfoSendFlag == FALSE )
		{
			pMsg.Percent = 0x80;
		}

		else if ( gObjTotalUser == 0 )
		{
			pMsg.Percent = 0;
		}

		else
		{
			pMsg.Percent = (gObjTotalUser * 100)/ g_ConfigRead.server.GetObjectMaxUser();
		}

		pMsg.ServerCode = g_ConfigRead.server.GetGameServerCode();
		pMsg.UserCount = gObjTotalUser;
		pMsg.AccountCount = gLCount[0].Get();
		pMsg.MaxUserCount = g_ConfigRead.server.GetObjectMaxUser();

		if (g_VipSystem.IsServerVIP() > 0)
		{
			if (g_ConfigRead.server.IsNonPvP() == true)
			{
				pMsg.PlayType = GS_PLAY_GOLD_NONPVP;
			}

			else
			{
				pMsg.PlayType = GS_PLAY_GOLD_PVP;
			}
		}

		else
		{
			if (g_ConfigRead.server.IsNonPvP() == true)
			{
				pMsg.PlayType = GS_PLAY_NONPVP;
			}

			else
			{
				pMsg.PlayType = GS_PLAY_PVP;
			}
		}

		gUdpSoc.SendData((LPBYTE)&pMsg, pMsg.h.size);
		Sleep(1000);
	}
}

void CheckSumFileLoad(char * szCheckSum)
{
	if(!IsFile(szCheckSum))
	{
		g_Log.Add("%s file load fail", szCheckSum);
		return;
	}

	std::streampos fileSize;
	std::ifstream file(szCheckSum, std::ios::binary);

	file.seekg(0, std::ios::end);
	fileSize = file.tellg();
	file.seekg(0, std::ios::beg);

	file.read((char*)&dwgCheckSum[0], fileSize);

	file.close();

	g_Log.Add("%s file loaded", szCheckSum);
}

void LoadMapFile()
{
	pugi::xml_document file;
	pugi::xml_parse_result result = file.load_file(g_ConfigRead.GetPath("IGC_MapList.xml"));

	if (result.status != pugi::status_ok)
	{
		g_Log.MsgBox("Failed to load IGC_MapList.xml file!");
		ExitProcess(1);
		return;
	}

	pugi::xml_node main_section = file.child("MapList");
	pugi::xml_node map_default = main_section.child("DefaultMaps");
	pugi::xml_node cw_maps = main_section.child("CrywolfEventAttr");
	pugi::xml_node kt_maps = main_section.child("KanturuEventAttr");

	char MapAttrName[MAX_NUMBER_MAP][65];

	for (pugi::xml_node mapfile = map_default.child("Map"); mapfile; mapfile = mapfile.next_sibling())
	{
		int MapNumber = mapfile.attribute("Number").as_int(-1);

		if (MapNumberCheck(MapNumber) == FALSE)
		{
			continue;
		}

		strcpy(MapAttrName[MapNumber], mapfile.attribute("File").as_string("Null.att"));
	}

	char CrywolfEventAttr[3][65];
	char KanturuEventAttr[2][65];

	for (pugi::xml_node cwfile = cw_maps.child("Map"); cwfile; cwfile = cwfile.next_sibling())
	{
		int MapNumber = cwfile.attribute("Number").as_int(-1);

		if (MapNumberCheck(MapNumber) == FALSE)
		{
			continue;
		}

		int OccupationState = cwfile.attribute("OccupationState").as_int(-1);

		if (OccupationState < 0 || OccupationState > 2)
		{
			continue;
		}

		strcpy(CrywolfEventAttr[OccupationState], cwfile.attribute("File").as_string("Null.att"));
	}

	for (pugi::xml_node ktfile = kt_maps.child("Map"); ktfile; ktfile = ktfile.next_sibling())
	{
		int MapNumber = ktfile.attribute("Number").as_int(-1);

		if (MapNumberCheck(MapNumber) == FALSE)
		{
			continue;
		}

		int MayaState = ktfile.attribute("MayaState").as_int(-1);

		if (MayaState < 0 || MayaState > 1)
		{
			continue;
		}

		strcpy(KanturuEventAttr[MayaState], ktfile.attribute("File").as_string("Null.att"));
	}

	char szPath[128];

	for (int n = 0; n<MAX_NUMBER_MAP; n++)
	{
		sprintf(szPath, "..\\IGCData\\MapTerrains\\%s", MapAttrName[n]);
		MapC[n].ItemInit();
		MapC[n].LoadMapAttr(szPath, n);
	}

	if (g_ConfigRead.server.GetServerType() == SERVER_CASTLE)
	{
		for (int i = 0; i < 3; i++)
		{
			sprintf(szPath, "..\\IGCData\\MapTerrains\\%s", CrywolfEventAttr[i]);
			g_Crywolf.LoadCrywolfMapAttr(szPath, i);
		}
	}

	else
	{
		for (int i = 0; i < 2; i++)
		{
			sprintf(szPath, "..\\IGCData\\MapTerrains\\%s", KanturuEventAttr[i]);
			g_Kanturu.LoadKanturuMapAttr(szPath, i);
		}
	}
}

void ReadEventInfo(MU_EVENT_TYPE eEventType)
{
	char szTemp[256];

	g_DevilSquare.Load(g_ConfigRead.GetPath("\\Events\\DevilSquare\\IGC_DevilSquare_Classic.xml"));
	g_DevilSquare.LoadMonster(g_ConfigRead.GetPath("\\Events\\DevilSquare\\IGC_DevilSquare_Classic_Monsters.xml"));

	g_BloodCastle.Load(g_ConfigRead.GetPath("\\Events\\BloodCastle\\IGC_BloodCastle.xml"));
	g_BloodCastle.LoadMonster(g_ConfigRead.GetPath("\\Events\\BloodCastle\\IGC_BloodCastle_Monster.xml"));

	gIsDropDarkLordItem = GetPrivateProfileInt("GameServerInfo", "IsDropDarkLordItem", 0, gDirPath.GetNewPath("commonserver.cfg"));
	gSleeveOfLordDropRate = GetPrivateProfileInt("GameServerInfo", "SleeveOfLordDropRate", 10, gDirPath.GetNewPath("commonserver.cfg"));
	gSleeveOfLordDropLevel = GetPrivateProfileInt("GameServerInfo", "SleeveOfLordDropLevel", 86, gDirPath.GetNewPath("commonserver.cfg"));
	gSoulOfDarkHorseDropRate = GetPrivateProfileInt("GameServerInfo", "SoulOfDarkHorseDropRate", 10, gDirPath.GetNewPath("commonserver.cfg"));
	gSoulOfDarkHorseropLevel = GetPrivateProfileInt("GameServerInfo", "SoulOfDarkHorseDropLevel", 125, gDirPath.GetNewPath("commonserver.cfg"));
	gSoulOfDarkSpiritDropRate = GetPrivateProfileInt("GameServerInfo", "SoulOfDarkSpiritDropRate", 10, gDirPath.GetNewPath("commonserver.cfg"));
	gSoulOfDarkSpiritDropLevel = GetPrivateProfileInt("GameServerInfo", "SoulOfDarkSpiritDropLevel", 96, gDirPath.GetNewPath("commonserver.cfg"));
	GetPrivateProfileString("GameServerInfo", "DarkSpiritAddExperience", "1", szTemp, 10, gDirPath.GetNewPath("commonserver.cfg"));
	gDarkSpiritAddExperience = (float)atof(szTemp);
	gIsDropGemOfDefend = GetPrivateProfileInt("GameServerInfo", "IsDropGemOfDefend", 0, gDirPath.GetNewPath("commonserver.cfg"));
	gGemOfDefendDropRate = GetPrivateProfileInt("GameServerInfo", "GemOfDefendDropRate", 5, gDirPath.GetNewPath("commonserver.cfg"));
	gGemOfDefendDropLevel = GetPrivateProfileInt("GameServerInfo", "GemOfDefendDropLevel", 75, gDirPath.GetNewPath("commonserver.cfg"));

	if (g_ConfigRead.server.GetServerType() == SERVER_CASTLE)
	{
		gIsDropSetItemInCastleHuntZone = GetPrivateProfileInt("GameServerInfo", "IsDropSetItemInCastleHuntZone", 0, gDirPath.GetNewPath("commonserver.cfg"));
		gSetItemInCastleHuntZoneDropRate = GetPrivateProfileInt("GameServerInfo", "SetItemInCastleHuntZoneDropRate", 1, gDirPath.GetNewPath("commonserver.cfg"));
		gSetItemInCastleHuntZoneDropLevel = GetPrivateProfileInt("GameServerInfo", "SetItemInCastleHuntZoneDropLevel", 75, gDirPath.GetNewPath("commonserver.cfg"));
	}

	gWeaponAttackDamage = GetPrivateProfileInt("GameServerInfo", "CastleSiegeWeaponAttackDamage", 300, gDirPath.GetNewPath("commonserver.cfg"));
	gWeaponDefenseDamage = GetPrivateProfileInt("GameServerInfo", "CastleSiegeWeaponDefenseDamage", 200, gDirPath.GetNewPath("commonserver.cfg"));

	g_RingAttackEvent.Load(g_ConfigRead.GetPath("\\Events\\IGC_WhiteWizardAttack.xml"));
	g_XMasAttackEvent.Load(g_ConfigRead.GetPath("\\Events\\IGC_XMasAttack.xml"));
	g_CastleDeepEvent.Load(g_ConfigRead.GetPath("\\Events\\IGC_LorenDeep.xml"));

	gEvent1 = GetPrivateProfileInt("GameServerInfo", "EVENT1", 0, gDirPath.GetNewPath("commonserver.cfg"));
	gEvent1ItemDropTodayMax = GetPrivateProfileInt("GameServerInfo", "Event1ItemDropTodayMax", 1, gDirPath.GetNewPath("commonserver.cfg"));
	gEvent1ItemDropTodayCount = GetPrivateProfileInt("GameServerInfo", "Event1ItemDropTodayCount", 0, gDirPath.GetNewPath("commonserver.cfg"));
	gEvent1ItemDropTodayPercent = GetPrivateProfileInt("GameServerInfo", "Event1ItemDropTodayPercent", 80, gDirPath.GetNewPath("commonserver.cfg"));
	gFireCrackerEvent = GetPrivateProfileInt("GameServerInfo", "FireCrackerEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));
	gFireCrackerDropRate = GetPrivateProfileInt("GameServerInfo", "FireCrackerDropRate", 5000, gDirPath.GetNewPath("commonserver.cfg"));
	g_ItemDropRateForgFireCracker = GetPrivateProfileInt("GameServerInfo", "ItemDropRateForFireCracker", 2, gDirPath.GetNewPath("commonserver.cfg"));
	gOnlyFireCrackerEffectUse = GetPrivateProfileInt("GameServerInfo", "OnlyFireCrackerEffectUse", 0, gDirPath.GetNewPath("commonserver.cfg"));
	gMedalEvent = GetPrivateProfileInt("GameServerInfo", "MedalEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));
	gGoldMedalDropRate = GetPrivateProfileInt("GameServerInfo", "GoldMedalDropRate", 2, gDirPath.GetNewPath("commonserver.cfg"));
	gSilverMedalDropRate = GetPrivateProfileInt("GameServerInfo", "SilverMedalDropRate", 2, gDirPath.GetNewPath("commonserver.cfg"));
	gXMasEvent = GetPrivateProfileInt("GameServerInfo", "XMasEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_XMasEvent_StarOfXMasDropRate = GetPrivateProfileInt("GameServerInfo", "XMasEvent_StarOfXMasDropRate", 5000, gDirPath.GetNewPath("commonserver.cfg"));
	gHeartOfLoveEvent = GetPrivateProfileInt("GameServerInfo", "HeartOfLoveEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));
	gHeartOfLoveDropRate = GetPrivateProfileInt("GameServerInfo", "HeartOfLoveDropRate", 5000, gDirPath.GetNewPath("commonserver.cfg"));
	g_ItemDropRateForgHeartOfLove = GetPrivateProfileInt("GameServerInfo", "ItemDropRateForHeartOfLove", 2, gDirPath.GetNewPath("commonserver.cfg"));
	gHappyNewYearNpcEvent = GetPrivateProfileInt("GameServerInfo", "HappyNewYearTalkNpc", 0, gDirPath.GetNewPath("commonserver.cfg"));

	if (gHappyNewYearNpcEvent != 0)
	{
		// (Option) Happy-new-year NPC speaks
		g_Log.Add("[Option] Happy-new-year NPC speaks");
	}

	gMerryXMasNpcEvent = GetPrivateProfileInt("GameServerInfo", "MerryXMasTalkNpc", 0, gDirPath.GetNewPath("commonserver.cfg"));

	if (gMerryXMasNpcEvent != 0)
	{
		// (Option) Christmas NPC speaks
		g_Log.Add("[Option] Christmas NPC speaks");
	}

	g_ChaosCastle.Load(g_ConfigRead.GetPath("\\Events\\ChaosCastle\\IGC_ChaosCastle.xml"));
	g_ChaosCastle.LoadMonster(g_ConfigRead.GetPath("\\Events\\ChaosCastle\\IGC_ChaosCastle_Monster.xml"));

	g_iKundunMarkDropRate = GetPrivateProfileInt("GameServerInfo", "KundunMarkDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));

	g_iMysteriousBeadDropRate1 = GetPrivateProfileInt("GameServerInfo", "MysteriouseBeadDropRate1", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iMysteriousBeadDropRate2 = GetPrivateProfileInt("GameServerInfo", "MysteriouseBeadDropRate2", 0, gDirPath.GetNewPath("commonserver.cfg"));

	g_iHiddenTreasureBoxOfflineRate = GetPrivateProfileInt("GameServerInfo", "HiddenTreasureBoxOfflineRate", 0, gDirPath.GetNewPath("commonserver.cfg"));

	AttackEvent->LoadScript(g_ConfigRead.GetPath("\\Events\\IGC_AttackEvent.xml"));
	DragonEvent->LoadScript(g_ConfigRead.GetPath("\\Events\\IGC_DragonEvent.xml"));

	g_EventManager.Load(g_ConfigRead.GetPath("\\Events\\IGC_InvasionManager.xml"));
	g_EventManager.RegEvent(0, DragonEvent);
	g_EventManager.RegEvent(1, AttackEvent);
	g_EventManager.RegEvent(2, &gEledoradoEvent);
	g_EventManager.RegEvent(3, &g_RingAttackEvent);
	g_EventManager.RegEvent(4, &g_XMasAttackEvent);
	g_EventManager.Init();

	g_iDarkLordHeartDropRate = GetPrivateProfileInt("GameServerInfo", "DarkLordHeartDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iDarkLordHeartOffEventRate = GetPrivateProfileInt("GameServerInfo", "DarkLoadHeartOffEventRate", 0, gDirPath.GetNewPath("commonserver.cfg"));

	// Ribbon Box Event
	g_bRibbonBoxEvent = GetPrivateProfileInt("GameServerInfo", "RibbonBoxEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));

	// Red Ribbon Box
	g_iRedRibbonBoxDropLevelMin = GetPrivateProfileInt("GameServerInfo", "RedRibbonBoxDropLv_Min", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iRedRibbonBoxDropLevelMax = GetPrivateProfileInt("GameServerInfo", "RedRibbonBoxDropLv_Max", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iRedRibbonBoxDropRate = GetPrivateProfileInt("GameServerInfo", "RedRibbonBoxDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));

	// Green Ribbon Box
	g_iGreenRibbonBoxDropLevelMin = GetPrivateProfileInt("GameServerInfo", "GreenRibbonBoxDropLv_Min", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iGreenRibbonBoxDropLevelMax = GetPrivateProfileInt("GameServerInfo", "GreenRibbonBoxDropLv_Max", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iGreenRibbonBoxDropRate = GetPrivateProfileInt("GameServerInfo", "GreenRibbonBoxDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));

	// Blue Ribbon Box
	g_iBlueRibbonBoxDropLevelMin = GetPrivateProfileInt("GameServerInfo", "BlueRibbonBoxDropLv_Min", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iBlueRibbonBoxDropLevelMax = GetPrivateProfileInt("GameServerInfo", "BlueRibbonBoxDropLv_Max", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iBlueRibbonBoxDropRate = GetPrivateProfileInt("GameServerInfo", "BlueRibbonBoxDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));

	// Chocolate Event
	g_bChocolateBoxEvent = GetPrivateProfileInt("GameServerInfo", "ChocolateEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));

	// Pink Chocolate Box
	g_iPinkChocolateBoxDropLevelMin = GetPrivateProfileInt("GameServerInfo", "PinkChocolateBoxDropLv_Min", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iPinkChocolateBoxDropLevelMax = GetPrivateProfileInt("GameServerInfo", "PinkChocolateBoxDropLv_Max", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iPinkChocolateBoxDropRate = GetPrivateProfileInt("GameServerInfo", "PinkChocolateBoxDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));

	// Red Chocolate Box
	g_iRedChocolateBoxDropLevelMin = GetPrivateProfileInt("GameServerInfo", "RedChocolateBoxDropLv_Min", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iRedChocolateBoxDropLevelMax = GetPrivateProfileInt("GameServerInfo", "RedChocolateBoxDropLv_Max", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iRedChocolateBoxDropRate = GetPrivateProfileInt("GameServerInfo", "RedChocolateBoxDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));

	// Blue Chocolate Box
	g_iBlueChocolateBoxDropLevelMin = GetPrivateProfileInt("GameServerInfo", "BlueChocolateBoxDropLv_Min", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iBlueChocolateBoxDropLevelMax = GetPrivateProfileInt("GameServerInfo", "BlueChocolateBoxDropLv_Max", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iBlueChocolateBoxDropRate = GetPrivateProfileInt("GameServerInfo", "BlueChocolateBoxDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));

	// Candy Event
	g_bCandyBoxEvent = GetPrivateProfileInt("GameServerInfo", "CandyBoxEvent", 0, gDirPath.GetNewPath("commonserver.cfg"));

	// LightPurple Candy Box
	g_iLightPurpleCandyBoxDropLevelMin = GetPrivateProfileInt("GameServerInfo", "LightPurpleCandyBoxDropLv_Min", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iLightPurpleCandyBoxDropLevelMax = GetPrivateProfileInt("GameServerInfo", "LightPurpleCandyBoxDropLv_Max", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iLightPurpleCandyBoxDropRate = GetPrivateProfileInt("GameServerInfo", "LightPurpleCandyBoxDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));

	// Vermilion Candy Box
	g_iVermilionCandyBoxDropLevelMin = GetPrivateProfileInt("GameServerInfo", "VermilionCandyBoxDropLv_Min", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iVermilionCandyBoxDropLevelMax = GetPrivateProfileInt("GameServerInfo", "VermilionCandyBoxDropLv_Max", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iVermilionCandyBoxDropRate = GetPrivateProfileInt("GameServerInfo", "VermilionCandyBoxDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));

	// DeepBlue Candy Box
	g_iDeepBlueCandyBoxDropLevelMin = GetPrivateProfileInt("GameServerInfo", "DeepBlueCandyBoxDropLv_Min", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iDeepBlueCandyBoxDropLevelMax = GetPrivateProfileInt("GameServerInfo", "DeepBlueCandyBoxDropLv_Max", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iDeepBlueCandyBoxDropRate = GetPrivateProfileInt("GameServerInfo", "DeepBlueCandyBoxDropRate", 0, gDirPath.GetNewPath("commonserver.cfg"));

	g_Crywolf.LoadData(g_ConfigRead.GetPath("\\Events\\IGC_CryWolf.xml"));

	g_Kanturu.LoadData(g_ConfigRead.GetPath("\\Events\\Kanturu\\IGC_KanturuEvent.xml"));
	g_KanturuMonsterMng.LoadData(g_ConfigRead.GetPath("\\Events\\Kanturu\\IGC_KanturuEvent_Monster.xml"));

	g_Raklion.LoadData(g_ConfigRead.GetPath("\\Events\\Raklion\\IGC_RaklionHatchery.xml"));
	g_RaklionMonsterMng.LoadData(g_ConfigRead.GetPath("\\Events\\Raklion\\IGC_RaklionHatchery_Monsters.xml"));
		
	g_IT_Event.Load_ITR_EventInfo();

	//Cherry Blossom
	g_bBlossomBoxDropOn = GetPrivateProfileInt("GameServerInfo", "CherryBlossomEventOn", 0, gDirPath.GetNewPath("commonserver.cfg"));
	g_iBlossomBoxDropRate = GetPrivateProfileInt("GameServerInfo", "CherryBlossomEventItemDropRate", 50, gDirPath.GetNewPath("commonserver.cfg"));

	g_DoppelGanger.DoppelgangerInfoLoad();

	g_ImperialGuardian.LoadScript(g_ConfigRead.GetPath("\\Events\\ImperialGuardian\\IGC_ImperialGuardian.ini"));
	g_MonsterStatCalc.LoadScript(g_ConfigRead.GetPath("\\Events\\ImperialGuardian\\IGC_ImperialGuardian_Monster_Info.xml"));

	g_ChaosCastleFinal.Load_CCFInfo(g_ConfigRead.GetPath("\\Events\\ChaosCastle_Survival\\IGC_ChaosCastle_Survival.xml"));
	g_ChaosCastleFinal.Load_CCFMonster(g_ConfigRead.GetPath("\\Events\\ChaosCastle_Survival\\IGC_ChaosCastle_Survival_Monster.xml"));

	g_DevilSquareFinal.Load_DSFScheduleScript(g_ConfigRead.GetPath("\\Events\\Tormented Square\\IGC_TormentedSquare.xml"));
	g_DevilSquareFinal.Load_DSFMonsterScript(g_ConfigRead.GetPath("\\Events\\Tormented Square\\IGC_TormentedSquare_Monsters.xml"));

	g_ArcaBattle.LoadScriptArcaBattle(g_ConfigRead.GetPath("\\Events\\ArcaBattle\\IGC_ArcaBattle.xml"));
	g_ArcaBattle.LoadScriptBootyMix(g_ConfigRead.GetPath("\\Events\\ArcaBattle\\IGC_ArcaBattle_BootyMix.xml"));
	g_AcheronGuardianEvent.LoadScriptAcheronEvent(g_ConfigRead.GetPath("\\Events\\IGC_AcheronGuardian.xml"));

	if (g_ArcaBattle.IsArcaBattleDay() == TRUE && g_AcheronGuardianEvent.IsAcheronGuardianDay() == TRUE)
	{
		g_Log.MsgBox(MB_OK | MB_ICONERROR, "Error", "Error - ArcaBattle and AcheronGuardian cannot be active at the same day.");
		ExitProcess(1);
	}
}

void ReadGameEtcInfo(MU_ETC_TYPE eGameEtcType)
{
	switch ( eGameEtcType )
	{
		case 0:
			gCreateCharacter = GetPrivateProfileInt("GameServerInfo", "CreateCharacter", 1, gDirPath.GetNewPath("commonserver.cfg"));

			bCanTrade = GetPrivateProfileInt("GameServerInfo", "Trade", 1, gDirPath.GetNewPath("commonserver.cfg"));

			bCanChaosBox = GetPrivateProfileInt("Common","IsChaosMachine",0, g_ConfigRead.GetPath("IGC_ChaosBox.ini"));

			gItemDropPer = GetPrivateProfileInt("GameServerInfo", "ItemDropPer", 10, gDirPath.GetNewPath("commonserver.cfg"));

			gDoPShopOpen = GetPrivateProfileInt("GameServerInfo", "PersonalShopOpen", 0, gDirPath.GetNewPath("commonserver.cfg"));

			bIsIgnorePacketSpeedHackDetect = GetPrivateProfileInt("GameServerInfo", "IsIgnorePacketHackDetect", 0, gDirPath.GetNewPath("commonserver.cfg"));
			break;

		case 1:
			gCreateCharacter = GetPrivateProfileInt("GameServerInfo", "CreateCharacter", 1, gDirPath.GetNewPath("commonserver.cfg"));
			break;

		case 3:
			bCanTrade = GetPrivateProfileInt("GameServerInfo", "Trade", 1, gDirPath.GetNewPath("commonserver.cfg"));

			break;
		case 4:
			bCanChaosBox = GetPrivateProfileInt("Common","IsChaosMachine",0, g_ConfigRead.GetPath("IGC_ChaosBox.ini"));

			break;

		case 5:
			gDoPShopOpen = GetPrivateProfileInt("GameServerInfo", "PersonalShopOpen", 0, gDirPath.GetNewPath("commonserver.cfg"));

			break;

		case 7:
			gItemDropPer = GetPrivateProfileInt("GameServerInfo", "ItemDropPer", 10, gDirPath.GetNewPath("commonserver.cfg"));

			break;

		case 8:
			gAttackSpeedTimeLimit = GetPrivateProfileInt("GameServerInfo", "AttackSpeedTimeLimit", 800, gDirPath.GetNewPath("commonserver.cfg"));
			bIsIgnorePacketSpeedHackDetect = GetPrivateProfileInt("GameServerInfo", "IsIgnorePacketHackDetect", 0, gDirPath.GetNewPath("commonserver.cfg"));
			gHackCheckCount = GetPrivateProfileInt("GameServerInfo", "HackCheckCount", 5, gDirPath.GetNewPath("commonserver.cfg"));
			gMinimumAttackSpeedTime = GetPrivateProfileInt("GameServerInfo", "MinimumAttackSpeedTime", 200 , gDirPath.GetNewPath("commonserver.cfg"));
			gDetectedHackKickCount = GetPrivateProfileInt("GameServerInfo", "DetectedHackKickCount", 10, gDirPath.GetNewPath("commonserver.cfg"));
			gIsKickDetecHackCountLimit = GetPrivateProfileInt("GameServerInfo", "IsKickDetecHackCountLimit", 0, gDirPath.GetNewPath("commonserver.cfg"));

			break;
	}
}

void ReloadEvent(MU_EVENT_RELOAD eEvent)
{
	switch ( eEvent )
	{
		case 0:
			g_BloodCastle.Load(g_ConfigRead.GetPath("\\Events\\BloodCastle\\IGC_BloodCastle.xml"));
			g_BloodCastle.LoadMonster(g_ConfigRead.GetPath("\\Events\\BloodCastle\\IGC_BloodCastle_Monster.xml"));
			g_BloodCastle.Init();
			break;
		case 1:
			g_DevilSquare.Load(g_ConfigRead.GetPath("\\Events\\DevilSquare\\IGC_DevilSquare_Classic.xml"));
			g_DevilSquare.LoadMonster(g_ConfigRead.GetPath("\\Events\\DevilSquare\\IGC_DevilSquare_Classic_Monsters.xml"));
			g_DevilSquare.Init();
			break;
		case 2:
			g_ChaosCastle.Load(g_ConfigRead.GetPath("\\Events\\ChaosCastle\\IGC_ChaosCastle.xml"));
			g_ChaosCastle.LoadMonster(g_ConfigRead.GetPath("\\Events\\ChaosCastle\\IGC_ChaosCastle_Monster.xml"));
			g_ChaosCastle.Init();
			break;
		case 3:
			if (g_ConfigRead.server.GetServerType() == SERVER_MARKET)
			{
				g_IT_Event.Load_ITR_EventInfo();
			}

			g_IT_Event.ITR_EventInit();
			break;
		case 4:
			if (g_ConfigRead.server.GetServerType() == SERVER_CASTLE)
			{
				if (g_CastleSiege.Ready(g_MapServerManager.GetMapSvrGroup()) == TRUE)
				{
					if (g_CastleSiege.LoadData(g_ConfigRead.GetPath("\\Events\\IGC_CastleSiege.xml")))
					{
						g_CastleSiege.LoadPreFixData(g_ConfigRead.GetPath("\\Events\\IGC_CastleSiege.xml"));
						g_CastleSiege.SetDataLoadState(CASTLESIEGE_DATALOAD_2);
					}
				}
			}

			else
			{
				g_Log.AddC(TColor::Blue, "Function not supported");
			}
			break;
		case 5:
			if (g_ConfigRead.server.GetServerType() == SERVER_CASTLE)
			{
				g_CastleDeepEvent.Load(g_ConfigRead.GetPath("\\Events\\IGC_LorenDeep.xml"));
			}

			else
			{
				g_Log.AddC(TColor::Blue, "Function not supported");
			}

			g_CastleDeepEvent.Init();
			break;
		case 6:
			if (g_ConfigRead.server.GetServerType() == SERVER_CASTLE)
			{
				g_Crywolf.LoadData(g_ConfigRead.GetPath("\\Events\\IGC_CryWolf.xml"));
			}

			else
			{
				g_Log.AddC(TColor::Blue, "Function not supported");
			}

			g_Crywolf.Init();
			break;

		case 7:
			if (g_ConfigRead.server.GetServerType() != SERVER_CASTLE)
			{
				g_Kanturu.LoadData(g_ConfigRead.GetPath("\\Events\\Kanturu\\IGC_KanturuEvent.xml"));
				g_KanturuMonsterMng.LoadData(g_ConfigRead.GetPath("\\Events\\Kanturu\\IGC_KanturuEvent_Monster.xml"));
				g_Kanturu.Init();
			}

			else
			{
				g_Log.AddC(TColor::Blue, "Function not supported");
			}
			break;
		case 8:
			if (g_ConfigRead.server.GetServerType() == SERVER_NORMAL)
			{
				g_Raklion.LoadData(g_ConfigRead.GetPath("\\Events\\Raklion\\IGC_RaklionHatchery.xml"));
				g_RaklionMonsterMng.LoadData(g_ConfigRead.GetPath("\\Events\\Raklion\\IGC_RaklionHatchery_Monsters.xml"));
				g_Raklion.Init();
			}
			else
			{
				g_Log.AddC(TColor::Blue, "Function not supported");
			}
			break;
		case 9:
			g_DoppelGanger.DoppelgangerInfoLoad();
			g_DoppelGanger.DoppelgangerProcessInit();
			break;
		case 10:
			g_ImperialGuardian.LoadScript(g_ConfigRead.GetPath("\\Events\\ImperialGuardian\\IGC_ImperialGuardian.ini"));
			g_MonsterStatCalc.LoadScript(g_ConfigRead.GetPath("\\Events\\ImperialGuardian\\IGC_ImperialGuardian_Monster_Info.xml"));
			g_ImperialGuardian.ImperialGuardianProcessInit();
			break;
		case 13:
			g_ArcaBattle.LoadScriptArcaBattle(g_ConfigRead.GetPath("\\Events\\ArcaBattle\\IGC_ArcaBattle.xml"));
			g_ArcaBattle.LoadScriptBootyMix(g_ConfigRead.GetPath("\\Events\\ArcaBattle\\IGC_ArcaBattle_BootyMix.xml"));

			if (g_ArcaBattle.IsArcaBattleDay() == TRUE && g_AcheronGuardianEvent.IsAcheronGuardianDay() == TRUE)
			{
				g_Log.MsgBox("Error - ArcaBattle and AcheronGuardian cannot be active at the same day - ArcaBattle disabled.");
				g_ArcaBattle.SetEventEnable(false);
			}

			g_ArcaBattle.Init();
			break;
		case 14:
			g_AcheronGuardianEvent.LoadScriptAcheronEvent(g_ConfigRead.GetPath("\\Events\\IGC_AcheronGuardian.xml"));
	
			if (g_ArcaBattle.IsArcaBattleDay() == TRUE && g_AcheronGuardianEvent.IsAcheronGuardianDay() == TRUE)
			{
				g_Log.MsgBox("Error - ArcaBattle and AcheronGuardian cannot be active at the same day - Acheron Guardian disabled.");
				g_AcheronGuardianEvent.SetEventEnable(false);
			}

			g_AcheronGuardianEvent.Init();
			break;
	}
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

