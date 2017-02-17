// ------------------------------
// Decompiled by Deathway
// Date : 2007-03-09
// ------------------------------
//------------------------------------------
// GameMain.h
//------------------------------------------

#ifndef GAMEMAIN_H
#define	GAMEMAIN_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "wsJoinServerCli.h"
#include "wsGameServer.h"
#include "DirPath.h"
#include "MapClass.h"
#include "classdef.h"
#include "CLoginCount.h"
#include "DragonEvent.h"
#include "AttackEvent.h"
#include "WhisperCash.h"
#include "WzUdp.h"
#include "giocp.h"
#include "MonsterAttr.h"
#include "MonsterSetBase.h"
#include "PartyClass.h"
#include "LogToFile.h"
#include "CastleSiege.h"
#include "CastleDeepEvent.h"
#include "Crywolf.h"

struct PMSG_FRIEND_STATE
{
	PBMSG_HEAD h;	// C1:C4
	char Name[10];	// 3
	BYTE State;	// D
};

enum MU_EVENT_TYPE {
	MU_EVENT_ALL = 0x0,
	MU_EVENT_DEVILSQUARE = 0x1,
	MU_EVENT_BLOODCASTLE = 0x2,
	MU_EVENT_ATTACKEVENTTROOP = 0x3,
	MU_EVENT_GOLDENTROOP = 0x4,
	MU_EVENT_WHITEMAGETROOP = 0x5,
	MU_EVENT_LOVEPANGPANG = 0x6,
	MU_EVENT_FIRECRACKER = 0x7,
	MU_EVENT_MEDALION = 0x8,
	MU_EVENT_XMASSTAR = 0x9,
	MU_EVENT_HEARTOFLOVE = 0xa,
	MU_EVENT_SAY_HAPPYNEWYEAR = 0xb,
	MU_EVENT_SAY_MERRYXMAS = 0xc,
	MU_EVENT_CHAOSCASTLE = 0xd,
	MU_EVENT_CHRISTMAS_RIBBONBOX = 0xe,
	MU_EVENT_VALENTINESDAY_CHOCOLATEBOX = 0xf,
	MU_EVENT_WHITEDAY_CANDYBOX = 0x10,
	MU_EVENT_ILLUSION_TEMPLE = 0x11,
	MU_EVENT_CHERRY_BLOSSOM = 0x12
};

enum MU_EVENT_RELOAD 
{
	EVENT_BC = 0,
	EVENT_DS = 1,
	EVENT_CC = 2,
	EVENT_IT = 3,
	EVENT_CS = 4,
	EVENT_DEEP = 5,
	EVENT_CW = 6,
	EVENT_KANTURU = 7,
	EVENT_RAKLION = 8,
	EVENT_DG = 9,
	EVENT_IF = 10,
	EVENT_RING = 11,
	EVENT_XMAS = 12,
	EVENT_ARCA = 13,
	EVENT_ACHERON  = 14,
	EVENT_CCF = 15,
	EVENT_DSF = 16
};

enum MU_ETC_TYPE {
	MU_ETC_ALL = 0x0,
	MU_ETC_CREATECHARACTER = 0x1,
	MU_ETC_GUILD = 0x2,
	MU_ETC_TRADE = 0x3,
	MU_ETC_USECHAOSBOX = 0x4,
	MU_ETC_PERSONALSHOP = 0x5,
	MU_ETC_PKITEMDROP = 0x6,
	MU_ETC_ITEMDROPRATE = 0x7,
	MU_ETC_SPEEDHACK = 0x8,
};

enum GAMESERVER_PLAY_TYPE {
	GS_PLAY_PVP = 0,
	GS_PLAY_NONPVP = 1,
	GS_PLAY_GOLD_PVP = 2,
	GS_PLAY_GOLD_NONPVP = 3
};

extern BOOL JoinServerConnected;
extern BOOL DataServerConnected;
extern BOOL GameServerCreated;
extern BOOL IsEventChipServerConnected;
extern CDragonEvent * DragonEvent;
extern CAttackEvent * AttackEvent;

extern BOOL SpeedHackPlayerBlock;
extern int	gCheckSumSwitch;
extern int  gSpeedHackPenalty;
extern bool gSerialZeroCheck;
extern BOOL gEnableEventNPCTalk;
extern BOOL gEnableServerDivision;
extern BOOL gEvent1;
extern int  gMonsterHp;
extern BOOL gMerryXMasNpcEvent;
extern BOOL gHappyNewYearNpcEvent;
extern int  gEvent1ItemDropTodayCount;
extern char gStalkProtocolId[11];
extern BOOL gXMasEvent;
extern BOOL gFireCrackerEvent;
extern BOOL gHeartOfLoveEvent;
extern BOOL gMedalEvent;
extern BOOL gEventChipEvent;
extern BOOL gWriteSkillLog;
extern BOOL g_bStoneItemDrop;
extern int  g_iKundunMarkDropRate;
extern int  g_iMarkOfTheLord;
extern int  g_iLuckyCoin;
extern int	g_iXmasEventLuckNumber1st;
extern int	g_iXmasEventLuckNumber2nd;
extern int  g_iDarkLordHeartDropRate;
extern int  g_iDarkLordHeartOffEventRate;
extern int g_iMysteriousBeadDropRate1;
extern int g_iMysteriousBeadDropRate2;
extern int g_iHiddenTreasureBoxOfflineRate;
extern BOOL bIsIgnorePacketSpeedHackDetect;
extern BOOL gIsKickDetecHackCountLimit;
extern BOOL gIsItemDropRingOfTransform;
extern BOOL gDoPShopOpen;
extern BOOL gWriteChatLog;
extern BOOL gDisconnectHackUser;
extern BOOL GSInfoSendFlag;
//float gItemBuyPriceMultiplier;
//float gItemSellPriceMultiplier;
extern DWORD JoinServerDCTime;
extern CwsGameServer wsGServer;	// line : 213GameServer
extern wsJoinServerCli wsJServerCli;	// line : 214 Join Server
extern wsJoinServerCli wsDataCli;	// line : 215 DataServer
extern wsJoinServerCli wsExDbCli;	// line : 239 Extra DataBase Server
extern CDirPath gDirPath;	// line : 248 Directory Path
extern MapClass MapC[MAX_NUMBER_MAP];	// line 249	// Map Manager
extern CMonsterAttr gMAttr;	// line 250
extern CMonsterSetBase gMSetBase;	// line 251
extern classdef DCInfo;	// line 252:
extern CWhisperCash WhisperCash;	// line 253
extern PartyClass gParty;	// line 254
extern CGuildClass Guild;	// line 265
extern WzUdp gUdpSoc;	// line 256
extern WzUdp gLauncherUdp;
extern CLoginCount gLCount[3];	// line 329
extern UINT64 * gLevelExperience;
extern char szGameServerExeSerial[24];
extern BOOL gIsDropDarkLordItem;
extern int  gSleeveOfLordDropRate;
extern int  gSleeveOfLordDropLevel;
extern int  gSoulOfDarkHorseDropRate;
extern int  gSoulOfDarkHorseropLevel;
extern int  gSoulOfDarkSpiritDropRate;
extern int  gSoulOfDarkSpiritDropLevel;
extern float gDarkSpiritAddExperience;
extern BOOL gIsDropGemOfDefend;
extern int  gGemOfDefendDropRate;
extern int  gGemOfDefendDropLevel;
extern int  g_iUseCharacterAutoRecuperationSystem;
extern int  g_iCharacterRecuperationMaxLevel;
extern int g_iServerGroupGuildChatting;
extern int g_iServerGroupUnionChatting;
extern BOOL g_bRibbonBoxEvent;
extern int g_iRedRibbonBoxDropLevelMin;
extern int g_iRedRibbonBoxDropLevelMax;
extern int g_iRedRibbonBoxDropRate;
extern int g_iGreenRibbonBoxDropLevelMin;
extern int g_iGreenRibbonBoxDropLevelMax;
extern int g_iGreenRibbonBoxDropRate;
extern int g_iBlueRibbonBoxDropLevelMin;
extern int g_iBlueRibbonBoxDropLevelMax;
extern int g_iBlueRibbonBoxDropRate;
extern BOOL g_bChocolateBoxEvent;
extern int g_iPinkChocolateBoxDropLevelMin;
extern int g_iPinkChocolateBoxDropLevelMax;
extern int g_iPinkChocolateBoxDropRate;
extern int g_iRedChocolateBoxDropLevelMin;
extern int g_iRedChocolateBoxDropLevelMax;
extern int g_iRedChocolateBoxDropRate;
extern int g_iBlueChocolateBoxDropLevelMin;
extern int g_iBlueChocolateBoxDropLevelMax;
extern int g_iBlueChocolateBoxDropRate;
extern BOOL g_bCandyBoxEvent;
extern int g_iLightPurpleCandyBoxDropLevelMin;
extern int g_iLightPurpleCandyBoxDropLevelMax;
extern int g_iLightPurpleCandyBoxDropRate;
extern int g_iVermilionCandyBoxDropLevelMin;
extern int g_iVermilionCandyBoxDropLevelMax;
extern int g_iVermilionCandyBoxDropRate;
extern int g_iDeepBlueCandyBoxDropLevelMin;
extern int g_iDeepBlueCandyBoxDropLevelMax;
extern int g_iDeepBlueCandyBoxDropRate;
extern BOOL g_bFenrirStuffItemDrop;
extern int g_iFenrirStuff_01_DropLv_Min;
extern int g_iFenrirStuff_01_DropLv_Max;
extern int g_iFenrirStuff_01_DropMap;
extern int g_iFenrirStuff_01_DropRate;
extern int g_iFenrirStuff_02_DropLv_Min;
extern int g_iFenrirStuff_02_DropLv_Max;
extern int g_iFenrirStuff_02_DropMap;
extern int g_iFenrirStuff_02_DropRate;
extern int g_iFenrirStuff_03_DropLv_Min;
extern int g_iFenrirStuff_03_DropLv_Max;
extern int g_iFenrirStuff_03_DropMap;
extern int g_iFenrirStuff_03_DropRate;
extern int g_iFenrirRepairRate;
extern int g_iFenrirDefaultMaxDurSmall;
extern int g_iFenrirElfMaxDurSmall;
extern int g_iFenrir_01Level_MixRate;
extern int g_iFenrir_02Level_MixRate;
extern int g_iFenrir_03Level_MixRate;
extern int g_iSkillDistanceCheck;
extern int g_iSkillDistanceCheckTemp;
extern int g_iSkillDistanceKick;
extern int g_iSkillDistanceKickCount;
extern int g_iSkillDiatanceKickCheckTime;
extern BOOL g_bHallowinDayEventOn;
extern int g_iHallowinDayEventItemDropRate;
extern int g_iHallowinDayEventJOLBlessDropRate;
extern int g_iHallowinDayEventJOLAngerDropRaTe;
extern int g_iHallowinDayEventJOLScreamDropRate;
extern int g_iHallowinDayEventJOLFoodDropRate;
extern int g_iHallowinDayEventJOLDrinkDropRate;
extern int g_iHallowinDayEventJOLPolymorphRingDropRate;
extern BOOL g_bSantaPolymorphRingDropOn;
extern int g_iSantaPolymorphRingDropRate;
extern int g_iCondorFlameDropRate;
extern bool g_bBlossomBoxDropOn;
extern int g_iBlossomBoxDropRate;
extern int g_iShadowPhantomMaxLevel;

extern int g_MaxStrength;
extern int g_MaxAgility;
extern int g_MaxVitality;
extern int g_MaxEnergy;
extern int g_MaxCommand;

extern DWORD dwgCheckSum[MAX_CHECKSUM_KEY];
extern DWORD  gItemNumberCount;
extern BOOL gStalkProtocol;
extern int  gYear;
extern BOOL gOnlyFireCrackerEffectUse;
extern int  giKundunRefillHPSec;
extern int  giKundunRefillHP;
extern int  giKundunRefillHPTime;
extern int  giKundunHPLogSaveTime;
extern BOOL gUseNPGGChecksum;
extern int g_ShieldSystemOn;
extern int g_iDamageDevideToSDRate;
extern int g_iDamageDevideToHPRate;
extern float g_fSuccessAttackRateOption;
extern int g_iSDChargingOption;
extern int g_iConstNumberOfShieldPoint;
extern int g_ShieldAutoRefillOn;
extern int g_ShieldAutoRefillOnSafeZone;
extern int g_CompoundPotionDropOn;
extern int g_iCompoundPotionLv1DropRate;
extern int g_iCompoundPotionLv2DropRate;
extern int g_iCompoundPotionLv3DropRate;
extern int g_iCompoundPotionLv1DropLevel;
extern int g_iCompoundPotionLv2DropLevel;
extern int g_iCompoundPotionLv3DropLevel;
extern BOOL g_bShieldComboMissOptionOn;
extern int g_iShieldPotionLv1MixSuccessRate;
extern int g_iShieldPotionLv1MixMoney;
extern int g_iShieldPotionLv2MixSuccessRate;
extern int g_iShieldPotionLv2MixMoney;
extern int g_iShieldPotionLv3MixSuccessRate;
extern int g_iShieldPotionLv3MixMoney;
extern int g_iShieldGageConstA;
extern int g_iShieldGageConstB;
extern char szGameServerVersion[24];
extern char szClientVersion[8];
extern BOOL bCanTrade;
extern BOOL bCanChaosBox;
extern BOOL bCanWarehouseLock;
extern BOOL gEnableBattleSoccer;
extern int  gLootingTime;
extern int  gItemDropPer;
extern int gLevelUpPointNormal;
extern int gLevelUpPointMGDL;
extern int gItemDisapearTime;
extern int  IsMultiWareHouse;
extern int  g_WareHouseCount;
extern int  gEvent1ItemDropTodayMax;
extern int  gEvent1ItemDropTodayPercent;
extern int  gCharacterDeleteMinLevel;
extern BOOL gCreateCharacter;
extern BOOL gItemSerialCheck;
extern int  g_XMasEvent_StarOfXMasDropRate;
extern int  g_XMasEvent_ItemDropRateForStarOfXMas;
extern int  gFireCrackerDropRate;
extern int  g_ItemDropRateForgFireCracker;
extern int  gHeartOfLoveDropRate;
extern int  g_ItemDropRateForgHeartOfLove;
extern int  gGoldMedalDropRate;
extern int  gSilverMedalDropRate;
extern int  g_ItemDropRateForGoldMedal;
extern int  g_ItemDropRateForSilverMedal;
extern int  gBoxOfGoldDropRate;
extern int  g_ItemDropRateForBoxOfGold;
extern int  g_EventChipDropRateForBoxOfGold;
extern int  gDQChaosSuccessRateLevel1;
extern int  gDQChaosSuccessRateLevel2;
extern int  gDQChaosSuccessRateLevel3;
extern int  gDQChaosSuccessRateLevel4;
extern int  gDQChaosSuccessRateLevel5;
extern int  gDQChaosSuccessRateLevel6;
extern int	gDQChaosSuccessRateLevel7;
extern DWORD  gAttackSpeedTimeLimit;
extern DWORD  gHackCheckCount;
extern float gDecTimePerAttackSpeed;
extern int  gMinimumAttackSpeedTime;
extern int  gDetectedHackKickCount;
extern BOOL gItemDropRingOfTransform;
extern int  gQuestNPCTeleportTime;
extern int  gZenDurationTime;
extern BOOL gEnableCheckPenetrationSkill;

extern char gIsDropCmd;

extern int gSilverBoxDropRate;
extern int gGoldBoxDropRate;

extern int gSetPartyExp2;
extern int gSetPartyExp3;
extern int gSetPartyExp4;
extern int gSetPartyExp5;
extern int gPartyExp1;
extern int gPartyExp2;
extern int gPartyExp3;
extern int gPartyExp4;
extern int gPartyExp5;

extern short gLuckDropRate;
extern short gSkillDropRate;
extern short gExcLuckDropRate;
extern short gExcSkillDropRate;
extern short gExcDropRate;

extern int gWeaponDefenseDamage;
extern int gWeaponAttackDamage;

extern CCastleSiege g_CastleSiege;
extern CCastleDeepEvent g_CastleDeepEvent;
extern int gIsDropSetItemInCastleHuntZone;
extern int gSetItemInCastleHuntZoneDropRate;
extern int gSetItemInCastleHuntZoneDropLevel;

extern CLogToFile * GMLog;
extern CLogToFile * MsgLog;
extern CLogToFile * TradeLog;
extern CLogToFile * AntiHackLog;
extern CLogToFile * BotShopLog;
extern CLogToFile * SerialCheck;

//------------------------------------------
// GameMain.cpp Functions - Prototypes List - Completed
//------------------------------------------
void gSetDate();
BOOL gJoomin15Check(char* szJN);
BOOL gJoominCheck(char* szJN, int iLimitAge);
void GameMainInit(HWND hWnd);
int GetWarehouseUsedHowMuch(int UserLevel, BOOL IsLock);
void GraphPaint(HWND hWnd);
void GameMonsterAllAdd();
void GameMonsterAllCloseAndReLoad();
void AddBots();
void GameMainFree();
BOOL GMJoinServerConnect(char* ConnectServer, DWORD wMsg);
BOOL GMDataServerConnect(char* ConnectServer, WPARAM wMsg);
BOOL ExDataServerConnect(char* ConnectServer, DWORD wMsg);
BOOL GameMainServerCreate(DWORD sMsg, DWORD cMsg);
void GMServerMsgProc( WPARAM wParam, LPARAM lParam);
void GMClientMsgProc( WPARAM wParam, LPARAM lParam);
void GMJoinClientMsgProc(WPARAM wParam, LPARAM lParam);
void ExDataClientMsgProc(WPARAM wParam, LPARAM lParam);
void GMDataClientMsgProc(WPARAM wParam, LPARAM lParam);
void ReadCommonServerInfo();
void GameServerInfoSendStop();
void GameServerInfoSendStart();
void GameServerInfoSend();
void CheckSumFileLoad(char * szCheckSum);
void SetEventTable();
void LoadMapFile();
int GetEventFlag();
void ReadEventInfo(MU_EVENT_TYPE eEventType);
void ReadGameEtcInfo(MU_ETC_TYPE eGameEtcType);
void ReloadEvent(MU_EVENT_RELOAD eReload);
void AuthClientMsgProc(WPARAM wParam, LPARAM lParam);
#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

