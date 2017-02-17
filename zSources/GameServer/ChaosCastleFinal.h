//////////////////////////////////////////////////////////////////////
// ChaosCastleFinal.h
#ifndef CHAOSCASTLEFINAL_H
#define CHAOSCASTLEFINAL_H

#include "winutil.h"

#define MAX_CCF_TRAP_STEP	4
#define MAX_CCF_HOLLOW_ZONE	4
#define CCF_TRAP_STEP_RANGE(floor) ( ((floor) < 0)?FALSE:((floor) >MAX_CCF_TRAP_STEP-1 )?FALSE:TRUE )

struct _CCF_USER
{
	_CCF_USER()
	{
		this->Clear();
	}

	void Clear()
	{
		this->m_nIndex = -1;
		this->m_nEXP = 0;
		this->m_nScore = 0;
		this->m_nUserState = 0;
		this->m_byCCFType = 0;
	}

	int m_nIndex;
	int m_nEXP;
	int m_nScore;
	int m_nUserState;
	BYTE m_byCCFType;
};

struct _CCF_MONSTER_ITEM_DROP
{
	int m_nIndex;
	int m_nItemKind;
};

struct _CCF_DATA
{
	_CCF_DATA()
	{
		InitializeCriticalSection(&this->m_CCFDropCriti);
		this->Clear();
	}

	~_CCF_DATA()
	{
		DeleteCriticalSection(&this->m_CCFDropCriti);
	}

	void Clear()
	{
		this->m_byCCF_TYPE = 0;
		this->m_nMISSION_SUCCESS = -1;
		this->m_nCCF_NOTIFY_COUNT = -1;
		this->m_bCCF_MSG_BEFORE_ENTER = 0;
		this->m_bCCF_MSG_BEFORE_PLAY = 0;
		this->m_bCCF_MSG_BEFORE_END = 0;
		this->m_bCCF_MSG_BEFORE_QUIT = 0;
		this->m_bCCF_CAN_ENTER = 0;
		this->m_bCCF_CAN_PARTY = 0;
		this->m_bCCF_PLAY_START = 0;
		this->m_bCCF_MSG_ALL_NOTIFY = 0;
		this->m_nCCF_ENTERED_USER = 0;
		this->m_nCCF_FIRST_USER_COUNT = 0;
		this->m_nCCF_FIRST_MONSTER_COUNT = 0;
		this->m_lCCF_CURRENT_MONSTER_COUNT = 0;
		this->m_nCCF_TRAP_STATUS = 0;
		this->m_nCCF_TRAP_OLD_STATUS = 0;

		for (int iMON = 0; iMON < 100; iMON++)
			this->m_lCCF_MONSTER_COUNT[iMON] = -1;
		for (int iFALL = 0; iFALL < 70; iFALL++)
			this->m_lFallUser[iFALL] = -1;

		EnterCriticalSection(&this->m_CCFDropCriti);
		this->m_mapCCFMonsterItemList.clear();
		LeaveCriticalSection(&this->m_CCFDropCriti);
	}

	int m_nCCF_STATE;
	int m_nMapNumber;
	int m_nMISSION_SUCCESS;
	BYTE m_byCCF_TYPE;
	int m_nCCF_REMAIN_MSEC;
	int m_nCCF_TICK_COUNT;
	int m_nCCF_NOTIFY_COUNT;
	BYTE m_bCCF_CAN_ENTER;
	BYTE m_bCCF_CAN_PARTY;
	BYTE m_bCCF_PLAY_START;
	BYTE m_bCCF_MSG_BEFORE_ENTER;
	BYTE m_bCCF_MSG_BEFORE_PLAY;
	BYTE m_bCCF_MSG_BEFORE_END;
	BYTE m_bCCF_MSG_BEFORE_QUIT;
	BYTE m_bCCF_MSG_ALL_NOTIFY;
	int m_nCCF_ENTERED_USER;
	int m_nCCF_FIRST_USER_COUNT;
	int m_nCCF_FIRST_MONSTER_COUNT;
	long m_lCCF_CURRENT_MONSTER_COUNT;
	int m_nCCF_TRAP_STATUS;
	int m_nCCF_TRAP_OLD_STATUS;
	_CCF_USER m_UserData[70];
	long m_lCCF_MONSTER_COUNT[100];
	long m_lFallUser[70];
	std::map<int, _CCF_MONSTER_ITEM_DROP> m_mapCCFMonsterItemList;
	float m_fRewardExpRate;
	CRITICAL_SECTION m_CCFDropCriti;
};

struct CCF_START_TIME
{
	int m_nHour;
	int m_nMinute;
	int m_nTryout;
	int m_nSemiFinal;
	int m_nFinal;
};

struct CCF_MONSTER_POSITION
{
	WORD m_Type;
	BYTE m_MapNumber;
	BYTE m_Dis;
	BYTE m_X;
	BYTE m_Y;
	BYTE m_Dir;
};

struct PMSG_SEND_CCF_INFO_ALL_SVR
{
	PBMSG_HEAD2 h;
	int nMin;
	int nType;
	WORD wMapSvrNum;
};

#pragma pack(1)
struct _stPMSG_CCF_RANK_INFO
{
	PWMSG_HEAD2 h;
	BYTE btResult;
	BYTE btCnt;
};
#pragma pack()

struct _CCFRankingInfo
{
	BYTE byRank;
	int nPoint;
	char szCharName[MAX_ACCOUNT_LEN + 1];
};

struct _stCCFRankingInfo
{
	_stCCFRankingInfo()
	{
		memset(this->szCharName, 0x00, sizeof(this->szCharName));
		this->byRank = 0;
		this->nPoint = 0;
	}

	char szCharName[MAX_ACCOUNT_LEN + 1];
	BYTE byRank;
	int nPoint;
};

struct _tagPMSG_REQ_CCF_PERMISSION
{
	PBMSG_HEAD2 h;
	char szCharName[MAX_ACCOUNT_LEN + 1];
	BYTE byCCFType;
	int nIndex;
};

struct SDHP_REQ_CCF_RANKING
{
	PBMSG_HEAD2 h;
	BYTE byRankingType;
	int nServerCategory;
};

struct SDHP_RENEW_RANKING
{
	PBMSG_HEAD2 h;
	BYTE byRankingType;
};

struct _tagPMSG_REQ_SAVE_CCF_RESULT
{
	PBMSG_HEAD2 h;
	char szCharName[MAX_ACCOUNT_LEN + 1];
	int nPoint;
	int nCharClass;
	int nCharLevel;
	UINT64 nCharExp;
	BYTE byCCFType;
};

struct _tagPMSG_ANS_CCF_RANK
{
	PWMSG_HEAD h;
	BYTE byUserCnt;
	_stCCFRankingInfo RankingInfo[50];
};

struct PMSG_ANS_CCF_SCHEDULE
{
	PWMSG_HEAD2 h;
	int nCount;
	BYTE Tryout1;
	BYTE Tryout2;
	BYTE Tryout3;
	BYTE SemiFinal1;
	BYTE SemiFinal2;
	BYTE Final;
	BYTE byCCFType;
};

struct PMSG_ANS_CCF_TIME
{
	int Hour;
	int Min;
	int TryOut;
	int SemiFinal;
	int Final;
};

struct PMSG_ANS_CCF_RESULT
{
	PBMSG_HEAD2 h;
	int type;
	int result;
	int point;
	int MobKillCnt;
	int PCKillCnt;
};

static int g_iCCF_GroundAxis[4] = { 23, 75, 44, 108 };
static int g_iCCF_DamageAxis[MAX_CCF_TRAP_STEP][4] =
{
	0x17, 0x4B, 0x2C, 0x6C,
	0x19, 0x4D, 0x2A, 0x6A,
	0x1B, 0x4F, 0x28, 0x68,
	0x1D, 0x51, 0x26, 0x66
};


static int g_iCCF_MapHollowZone[MAX_CCF_TRAP_STEP][MAX_CCF_HOLLOW_ZONE][4] =
{
	0x17, 0x4B, 0x2C, 0x4C,
	0x2B, 0x4D, 0x2C, 0x6C,
	0x17, 0x6B, 0x2A, 0x6C,
	0x17, 0x4D, 0x18, 0x6A,
	0x17, 0x4B, 0x2C, 0x4C,
	0x2B, 0x4D, 0x2C, 0x6C,
	0x17, 0x6B, 0x2A, 0x6C,
	0x17, 0x4D, 0x18, 0x6A,
	0x19, 0x4D, 0x2A, 0x4E,
	0x29, 0x4F, 0x2A, 0x6A,
	0x19, 0x69, 0x28, 0x6A,
	0x19, 0x4F, 0x1A, 0x68,
	0x1B, 0x4F, 0x28, 0x50,
	0x27, 0x51, 0x28, 0x68,
	0x1B, 0x67, 0x26, 0x68,
	0x1B, 0x51, 0x1C, 0x66
};

static int g_iCCF_Jewel_Random_Drop[5][6] = 
{
	0x1D, 0x52, 0x1F, 0x66, 0x1E, 0x54,
	0x24, 0x52, 0x26, 0x65, 0x25, 0x52,
	0x20, 0x52, 0x23, 0x53, 0x21, 0x52,
	0x20, 0x58, 0x23, 0x5F, 0x22, 0x5B,
	0x20, 0x64, 0x23, 0x65, 0x22, 0x64
};

static RECT g_rt_CCF_POINT_FRAME[4];
static RECT g_rt_CCF_POINT_TRAP[4];

static int g_nCCF_BlowOutDistance[4][2] = { 3, 4, 3, 4, 2, 3, 0, 1 };
static int g_nCCF_BlowOutDamage[4] = { 15, 15, 10, 5 };
static int g_iCCF_MonsterItems[3][2] =
{
	ITEMGET(14, 254), 6,
	ITEMGET(14, 256), 6,
	ITEMGET(14, 257), 6
};

struct CCF_WING_REWARD
{
	BYTE m_btCat;
	WORD m_wIndex;
	BYTE m_btLevel;
	BYTE m_btDurability;
	BYTE m_btSkill;
	BYTE m_btLuck;
	BYTE m_btOption;
	BYTE m_btNewOption;
	DWORD m_dwExpiryTime;
};

#pragma once
class ChaosCastleFinal
{
public:
	ChaosCastleFinal();
	virtual ~ChaosCastleFinal();

	void Run();
	void Init();

	void Load_CCFMonster(char *Filename);
	void Load_CCFInfo(char *Filename);

	void ClearCCFData();
	
	void CheckSync();

	void ProcState_None();
	void ProcState_Closed();
	void ProcState_Playing();
	void ProcState_PlayEnd();

	void SendAllUserAnyMsg(LPBYTE lpMsg, int iSize);
	void SendAllLoserFailMessage(int iWinnerIndex);
	void SendCCFWinMessage(int iWinnerIndex, int nPoint, bool bFinalWinner);
	void SendFailMessage(int iLoserIndex);
	void SetUserState(int iUserIndex, int iState);
	void SendNoticeState(int iPlayState);
	void SendCastleZoneSafetyInfo(bool bDoSet);
	void UnSafetyCastleZone();
	void SafetyCastleZone();
	void SendChaosCastleAnyMsg(LPBYTE lpMsg, int iSize);

	void AddFallUser(int iUserIndex);
	void SearchNBlowObjs(int iMapNumber, int iX, int iY);
	int BlowObjsFromPoint(int iIndex, int iMapNumber, int& iX, int& iY);
	int ObjSetPosition(int iIndex, int iX, int iY);
	int CalDistance(int iX1, int iY1, int iX2, int iY2);
	void PullObjInnerPlace(int iTRAP_STEP);
	void SetMapAttrHollow(int iTRAP_STEP);
	void SetMapAttrFill();
	void ProcessTrapStatus();
	void ProcessFallUser();
	void CheckMonsterInDieTile();
	void CheckUserInDieTile();
	void GiveUserDamage(int iUserIndex, int iDamage);

	int CheckWearingMOPH(int iUserIndex);

	int GetCurrentCCFWinUser(int *nPoint);
	int GetMonsterListCount();
	int GetCurPlayUser();
	int CheckPlayEnded();
	int GetFinalRemainTime(int iUserIndex);
	int GetRemainTime(int iUserIndex);

	void SearchCCFNDropMonsterItem(int iMonsterIndex, int iMaxHitUserIndex);
	void SetItemsToMonster(int iCCFType);
	int SetMonster();
	void ClearMonster();
	void AddMonsterList(int iMonsterIndex);
	void DelMonsterList(int iMonsterIndex);

	bool CheckOpenOrNotOpenCCF_Final(BYTE bCCFType);
	
	void SetState(int iCCF_STATE, int nFlag);
	void SetState_None();
	void SetState_Closed();
	void SetState_Playing();
	void SetState_PlayEnd(int nFlag);

	int GetCurEnteredUser() { return this->m_stCCFData.m_nCCF_ENTERED_USER; }
	int GetCurrentRemainSec() { return this->m_stCCFData.m_nCCF_REMAIN_MSEC / 1000; }
	int GetCurrentState() { return this->m_stCCFData.m_nCCF_STATE; }
	bool CheckCanEnter() { return this->m_stCCFData.m_bCCF_CAN_ENTER; }
	int ReCalcUserCount();
	void GetRandomDropPos(BYTE & btPosX, BYTE & btPosY);

	void RewardUserEXP(BOOL bWinner);
	int CalcSendRewardEXP(int iUserIndex, int iEXP, int iKILLCOUNT_USER, int iKILLCOUNT_MONSTER);
	int LevelUp(int iUserIndex, int iAddExp);

	// Survival addons
	int CCF_Ranking_DayType();
	int Check_CCF_DayType();
	bool CheckCanStart_CCF();
	void CCF_Start_Fail_So_RollBack();
	void Send_CCF_Schedule(int aIndex);
	int EnterCheck_CCF_User(int iUserIndex);
	int CheckUserEnterMoney(int iUserIndex, BYTE byBattleType);
	void GD_Req_Save_CCF_Result(int index, char *Name, int Point, int nCCFtype);
	void GD_Renew_CCF_Ranking(BYTE byCCFType);
	void GD_Load_CCF_RankingList(BYTE byCCF_Type);
	void GD_Req_Get_Permission(OBJECTSTRUCT* lpObj);
	int PayUserEnterMoney(int iUserIndex, int nCCFType);
	int EnterCCF(int iUserIndex, int & nCCFUserIndex);
	int CheckRankingReqValid(BYTE byCCFType);
	void DG_Ans_CCF_Rank(_tagPMSG_ANS_CCF_RANK *lpMsg);
	void Send_RankingInfo(BYTE byCCFType, int aIndex);
	void GiveCCFWinnerItem(int iWinnerIndex, BYTE byCCFType, BYTE byRewardType);
	void SendNoticeMessage(char* lpszMSG);
	void GD_SendCCFInfoAllServer(int nMinute, int nType);
	void SendAllMemberOfCCF(int index);
	void MakeChaosCastleRewardWing(int iUserIndex);
	bool IsRealFinalDayEnd();
	void SetOpenTodaySchedule();
	void SetUBFGetReward(int iUserIndex, WORD wItemCode, UINT64 ItemSerial, BYTE btItemPos);
	void GDReqSetCCFReward_UBF(int iUserIndex, BYTE btCCFType, BYTE btRewardType);

	int m_nCCF_MinUserCount;
	int m_nCCF_Level;
	bool m_FirstRankingInfoLoad;
	bool m_bCCF_EVENT_ENABLE;
	BYTE m_bFinalDayEnd;
	CCF_MONSTER_POSITION m_CCFMP[100];

	static bool m_bCCFDayChange;
	static BYTE m_byCCFTimer;

private:
	BYTE m_byTryOut1;
	BYTE m_byTryOut2;
	BYTE m_byTryOut3;
	BYTE m_bySemiFinal1;
	BYTE m_bySemiFinal2;
	BYTE m_byFinal;

	int m_iCCF_BoxRatio1;
	int m_iCCF_BoxRatio2;
	int m_iCCF_BoxRatio3;
	int m_iCCF_BoxRatio4;
	int m_iCCF_BoxRatio5;
	int m_iCCF_BoxRatio6;
	BYTE m_byBoxDropCnt;

	int m_iCCF_TIME_MIN_ALL_NOTIFY;
	int m_iCCF_TIME_MIN_OPEN;
	int m_iCCF_TIME_MIN_PLAY;
	int m_iCCF_TIME_MIN_REST;

	bool m_bIsCCF_Ranking_Renew;

	std::vector<CCF_START_TIME> m_vtCCFOpenTime;
	std::vector<CCF_START_TIME> m_vtCCFTime;

	_CCF_DATA m_stCCFData;
	_stCCFRankingInfo m_CCFRanking[50];
	CCF_WING_REWARD m_stCCFFinalReward;

	int m_CCF_MOB_CNT_SCRIPT;

public:
	bool m_bCCF_CHEAT_TRAP;
	bool m_bCCF_CHEAT_BLOW;
};

extern ChaosCastleFinal g_ChaosCastleFinal;

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

