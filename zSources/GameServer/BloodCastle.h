//////////////////////////////////////////////////////////////////////
// BloodCastle.h
// ------------------------------
// Decompiled by Deathway
// Date : 2007-05-09
// ------------------------------
#ifndef BLOODCASTLE_H
#define BLOODCASTLE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "user.h"
#include <list>

#define MAX_BLOOD_CASTLE_LEVEL 8
#define MAX_BLOOD_CASTLE_SUB_BRIDGE 10
#define MAX_BLOOD_CASTLE_BOSS_MONSTER	20
#define MAX_CLOACK_LEVEL	(MAX_BLOOD_CASTLE_LEVEL+1)

#define BC_STATE_NONE		0
#define BC_STATE_CLOSED		1
#define BC_STATE_PLAYING	2
#define BC_STATE_PLAYEND	3

#define BC_MAP_RANGE(map) (((map) < MAP_INDEX_BLOODCASTLE1)?0:((map) > MAP_INDEX_BLOODCASTLE7)?(((map) == MAP_INDEX_BLOODCASTLE8)?1:0):1)
#define BC_TIME_RANGE(x) ( (((x))<0)?FALSE:(((x))>1)?FALSE:TRUE  )
#define BC_STATUE_RANGE(x) ( (((x))<0)?FALSE:(((x))>2)?FALSE:TRUE  )
#define BC_WEAPON_LEVEL_RANGE(x) ( (((x))<0)?FALSE:(((x))>2)?FALSE:TRUE  )
#define BC_BRIDGE_RANGE(x) ( ((x)<0)?FALSE:((x)>MAX_BLOOD_CASTLE_LEVEL-1)?FALSE:TRUE  )
#define BC_SUB_BRIDGE_RANGE(x) ( ((x)<0)?FALSE:((x)>MAX_BLOOD_CASTLE_SUB_BRIDGE-1)?FALSE:TRUE  )
#define BC_CLOACK_LEVEL_RANGE(x)  (  ((x)<0)?FALSE:((x)>MAX_CLOACK_LEVEL-1)?FALSE:TRUE )

struct PMSG_SETMAPATTR_COUNT
{
	PBMSG_HEAD h;	// C1:46
	BYTE btType;	// 3
	BYTE btMapAttr;	// 4
	BYTE btMapSetType;	// 5
	BYTE btCount;	// 6
};

struct PMSG_SETMAPATTR
{
	BYTE btX;	// 0
	BYTE btY;	// 1
};

typedef struct _BLOODCASTLE_USER
{
	int m_iIndex;	// 0
	int m_iEXP;	// 4
	int m_iScore;	// 8
	int m_iUserState;	// C
	bool m_bSendQuitMsg;	// 10
	bool m_bLiveWhenDoorBreak;	// 11

} BLOODCASTLE_USER, * LPBLOODCASTLE_USER;

struct BLOODCASTLE_START_TIME
{
	int m_iHour;	// 0
	int m_iMinute;	// 4
};

struct BLOODCASTLE_MONSTER_POSITION
{
	BLOODCASTLE_MONSTER_POSITION()
	{
		this->Init();
	}

	void Init()
	{
		this->m_Type		= -1;
		this->m_MapNumber	= -1;
		this->m_Dis			= -1;
		this->m_X			= -1;
		this->m_Y			= -1;
		this->m_Dir			= -1;
	}

	WORD m_Type;
	BYTE m_MapNumber;
	BYTE m_Dis;
	BYTE m_X;
	BYTE m_Y;
	BYTE m_Dir;
};

// sizeof ( _BLOODCASTLE_DATA ) == 0x1FC
typedef struct _BLOODCASTLE_BRIDGE
{
	CRITICAL_SECTION m_critUserData;	// 0
	BLOODCASTLE_USER m_UserData[MAX_BLOOD_CASTLE_SUB_BRIDGE];	// 18

	int m_iTOTAL_EXP;	// 138
	int m_iMapNumber;	// 13C
	int m_iBridgeIndex;	// 140
	int m_iMISSION_SUCCESS;	// 144
	BOOL m_bCASTLE_DOOR_LIVE;	// 148
	int m_iBC_STATE;	// 14C
	LONGLONG m_i64_BC_REMAIN_MSEC;	// 150
	LONGLONG m_i64_BC_TICK_COUNT;	// 154
	int m_iBC_NOTIFY_COUNT;	// 158
	bool m_bBC_MONSTER_KILL_COMPLETE;	// 15C
	bool m_bBC_BOSS_MONSTER_KILL_COMPLETE;	// 15D
	bool m_bBC_DOOR_TERMINATE_COMPLETE;	// 15E
	bool m_bBC_CAN_ENTER;	// 15F
	bool m_bBC_CAN_PARTY;	// 160
	bool m_bBC_PLAY_START;	// 161
	bool m_bBC_REWARDED;	// 162
	bool m_bBC_MSG_BEFORE_ENTER;	// 163
	bool m_bBC_MSG_BEFORE_PLAY;	// 164
	bool m_bBC_MSG_BEFORE_END;	// 165
	bool m_bBC_MSG_BEFORE_QUIT;	// 166
	float m_iCastleStatueHealth;	// 168
	float m_iCastleDoorHealth;	// 16C
	int m_iBC_MONSTER_MAX_COUNT;	// 170
	int m_iBC_MONSTER_KILL_COUNT;	// 174
	int m_iBC_MONSTER_SUCCESS_MSG_COUNT;	// 178
	ULONGLONG m_i64_BC_TICK_DOOR_OPEN;	// 17C
	int m_iBC_BOSS_MONSTER_MAX_COUNT;	// 180
	int m_iBC_BOSS_MONSTER_KILL_COUNT;	// 184
	int m_iBC_BOSS_MONSTER_SUCCESS_MSG_COUNT;	// 188
	UINT64 m_nBC_QUESTITEM_SERIAL;	// 18C
	int m_iBC_QUEST_ITEM_USER_INDEX;	// 190
	BYTE m_btBC_QUEST_ITEM_NUMBER;	// 194
	int m_iExtraEXP_Kill_Door_Party;	// 19C
	int m_iExtraEXP_Kill_Door_Index;	// 1A0
	char m_szKill_Door_AccountID[11];	// 1A4
	char m_szKill_Door_CharName[11];	// 1AF
	int m_iExtraEXP_Kill_Statue_Party;	// 1BC
	int m_iExtraEXP_Kill_Statue_Index;	// 1C0
	char m_szKill_Status_CharName[11];	// 1C4
	char m_szKill_Status_AccountID[11];	// 1CF
	int m_iExtraEXP_Win_Quest_Party;	// 1DC
	int m_iExtraEXP_Win_Quest_Index;	// 1E0
	char m_szWin_Quest_CharName[11];	// 1E4
	char m_szWin_Quest_AccountID[11];	// 1EF
	int m_iBC_COMPLETE_USER_INDEX;	// 1FC
	int m_iBC_DOOR_MONSTER_INDEX; // 200
	float m_iBC_REWARD_EXP; //208
	int m_iBC_CASTLE_BLOCK_INFO;

} BLOODCASTLE_BRIDGE, * LPBLOODCASTLE_BRIDGE;	

class CBloodCastle
{

public:

	int CheckChoasMixItem(int iIndex);
	bool BloodCastleChaosMix(int iIndex, int iLEVEL);
	bool AddExperience(int iIndex, int iEXP);
	int CalcSendRewardEXP(int iIndex, int iEXP);
	int CalcSendRewardZEN(int iIndex, int iZEN);
	void DropReward(int iIndex);
	void SendRewardScore(int iIndex, int iSCORE, int iLeftTime, int iAlivePartyCount);
	void GiveReward_Win(int iIndex, int iBridgeIndex);
	void GiveReward_Fail(int iBridgeIndex);
	void SearchUserDeleteQuestItem(int iIndex);
	void SearchUserDropQuestItem(int iIndex);
	void SetUserState(int iIndex, int iState);
	void SendCastleEntranceBlockInfo(int iBridgeIndex, bool bLive);
	void SendCastleBridgeBlockInfo(int iBridgeIndex, bool bLive);
	void SendCastleDoorBlockInfo(int iBridgeIndex, bool bLive);
	void SendAllUserAnyMsg(LPBYTE lpMsg, int iSize);
	void SendBridgeAnyMsg(LPBYTE lpMsg, int iSize, int iBridgeIndex);
	void SendNoticeMessage(int iBridgeIndex, char* lpszMSG);
	void SendNoticeScore(int iBridgeIndex);
	void SendNoticeState(int iBridgeIndex, int iPlayState);
	bool CheckUserBridgeMember(int iBridgeIndex, int iIndex);
	void BlockCastleEntrance(int iBridgeIndex);
	void ReleaseCastleEntrance(int iBridgeIndex);
	void BlockCastleBridge(int iBridgeIndex);
	void ReleaseCastleBridge(int iBridgeIndex);
	void BlockCastleDoor(int iBridgeIndex);
	void ReleaseCastleDoor(int iBridgeIndex);
	void SetMonsterKillCount(int iBridgeIndex);
	bool CheckMonsterKillCount(int iBridgeIndex);
	bool CheckMonsterKillSuccess(int iBridgeIndex);
	bool CheckBossKillCount(int iBridgeIndex);
	bool CheckBossKillSuccess(int iBridgeIndex);
	bool CheckEveryUserDie(int iBridgeIndex);
	bool CheckAngelKingExist(int iBridgeIndex);
	int GetWhoGotUltimateWeapon(int iBridgeIndex);
	int GetCurrentLiveUserCount(int iBridgeIndex);
	BOOL DropItemDirectly(int iBridgeIndex, int iIndex, int iItemType, int iItemPos);
	void ClearMonster(int iBridgeIndex, bool bClearCastleDoor);
	void SetMonster(int iBridgeIndex);
	void SetBossMonster(int iBridgeIndex);
	void SetSaintStatue(int iBridgeIndex);
	//void __thiscall LoadBossMonster(int, int, int, int, int, int, int, int);
	int LeaveUserBridge(int iBridgeIndex, int iBridgeSubIndex, int iUserIndex);
	int EnterUserBridge(int iBridgeIndex, int iUserIndex);
	int LevelUp(int iIndex, int iAddExp);
	void CheckUsersOnConnect(int iBridgeIndex);
	int GetCurrentState(int iBridgeIndex);
	int GetRemainTime(int iBridgeIndex);
	LONGLONG GetCurrentRemainSec(int iBridgeIndex);
	//int __thiscall GetExcelItemDropRate();
	//int __thiscall GetNormalItemDropRate();
	int CheckEnterLevel(int iIndex, int iLevel);
	bool CheckEnterFreeTicket(int iIndex);
	int CheckEnterItem(int iIndex);
	int CheckQuestItem(int iIndex);
	bool CheckWalk(int iIndex, int iMoveX, int iMoveY);
	bool CheckCanEnter(int iBridgeIndex);
	bool CheckCanParty(int iBridgeIndex);
	bool CheckPlayStart(int iBridgeIndex);
	bool CheckQuestItemSerial(int iBridgeIndex, CMapItem* lpItem);
	bool CheckUserHaveUlimateWeapon(int iIndex);
	bool CheckWinnerExist(int iBridgeIndex);
	bool CheckWinnerValid(int iBridgeIndex);
	bool CheckUserWinnerParty(int iBridgeIndex, int iIndex);
	bool CheckPartyExist(int iIndex);
	bool CheckWinnerPartyComplete(int iBridgeIndex);
	bool SetBridgeWinner(int iBridgeIndex, int iIndex);
	int GetWinnerPartyCompleteCount(int iBridgeIndex);
	int GetWinnerPartyCompletePoint(int iBridgeIndex);
	void ChangeMonsterState(int iBridgeIndex, int iIndex);
	void FixUsersPlayStateWin(int iBridgeIndex);
	void FixUsersPlayStateFail(int iBridgeIndex);

	//Season3 add-on
	int GetBridgeMapNumber(int iBridgeIndex);
	int GetBridgeIndex(int iMAP_NUM);
	int GetItemMapNumberFirst(int iMAP_NUM);
	int GetItemMapNumberSecond(int iMAP_NUM);

	void Run();
	void Init();

	void Load(LPSTR filename);
	void LoadMonster(LPSTR filename);

	void SetState(int iBridgeIndex, int iBC_STATE);

	void SetEventEnable(bool bEnable) { this->m_bBC_EVENT_ENABLE = bEnable; }
	bool IsEventEnable() { return this->m_bBC_EVENT_ENABLE; }

	bool ChangeUserIndex(int OldIndex, int NewIndex, int iBridgeIndex);
	void SendNoticeMessageToSpecificUser(int iBridgeIndex, int aIndex, int iPlayState);
	bool SetCastleBlockInfo(int iBridgeIndex, int iCastleBlockInfo);
	int  GetCastleBlockInfo(int iBridgeIndex);
	int  GetPlayUserCountRightNow(int iBridgeIndex);

	// Fixes by Dudas

	bool SetQuestItemSerialNumber(int iBridgeIndex, UINT64 iQuestItemSerial);
	UINT64 GetQuestItemSerialNumber(int iBridgeIndex);

	void ThrowQuestItemByUser(int iBridgeIndex, UINT64 iQuestItemSerial);
	void CatchQuestItemByUser(int iBridgeIndex, int iUserIndex, int iItemLevel);

	bool NpcAngelKing(LPOBJ lpNpc, LPOBJ lpObj);

	void KillMonsterProc(int iBridgeIndex, LPOBJ lpMonsterObj);
	void DestroyCastleDoor(int iBridgeIndex, LPOBJ lpDoorObj);
	void DestroySaintStatue(int iBridgeIndex, LPOBJ lpStatueObj);
	
	bool CheckCastleDoorLive(int iBridgeIndex);

	BLOODCASTLE_MONSTER_POSITION * GetMonsterPosData(int iPosNum, int iBridgeIndex, int iMonsterType);

	int GetArchangelScrollDropRate() { return this->m_iArchangelScrollDropRate; }
	int GetBloodBoneDropRate() { return this->m_iBloodBoneDropRate; }

	void SetCheatEventOpen(LONGLONG i64RemainMsec)
	{
		for (int i = 0; i < MAX_BLOOD_CASTLE_LEVEL; i++)
		{
			this->m_BridgeData[i].m_i64_BC_REMAIN_MSEC = i64RemainMsec;
		}
	}

	bool SetPosMonster(int iBridgeIndex, int iMonsterIndex, int iPosNum, int iMonsterClass);
	bool GetPosition(BLOODCASTLE_MONSTER_POSITION * lpPos, int iMapNumber, short & x, short & y);

	CBloodCastle();
	virtual ~CBloodCastle();

protected:

	void CheckSync(int iBridgeIndex);
	void ClearBridgeData(int iBridgeIndex);
	void ProcState_None(int iBridgeIndex);
	void ProcState_Closed(int iBridgeIndex);
	void ProcState_Playing(int iBridgeIndex);
	void ProcState_PlayEnd(int iBridgeIndex);
	void SetState_None(int iBridgeIndex);
	void SetState_Closed(int iBridgeIndex);
	void SetState_Playing(int iBridgeIndex);
	void SetState_PlayEnd(int iBridgeIndex);
	void BlockSector(int iMAP_NUM, int iSTART_X, int iSTART_Y, int iEND_X, int iEND_Y);
	void ReleaseSector(int iMAP_NUM, int iSTART_X, int iSTART_Y, int iEND_X, int iEND_Y);
	int GetAliveUserTotalEXP(int iBridgeIndex);
	
private:

	BLOODCASTLE_BRIDGE m_BridgeData[MAX_BLOOD_CASTLE_LEVEL];	// 4

	BLOODCASTLE_MONSTER_POSITION m_BCMP_SaintStatue[MAX_BLOOD_CASTLE_LEVEL];
	BLOODCASTLE_MONSTER_POSITION m_BCMP_CastleGate[MAX_BLOOD_CASTLE_LEVEL];
	BLOODCASTLE_MONSTER_POSITION m_BCMP_AngelKing[MAX_BLOOD_CASTLE_LEVEL];
	BLOODCASTLE_MONSTER_POSITION m_BCMP_BossMonster[MAX_BLOOD_CASTLE_LEVEL][20];
	BLOODCASTLE_MONSTER_POSITION m_BCMP_General[MAX_BLOOD_CASTLE_LEVEL][100];

	int							 m_iBC_BOSS_SCRIPT_CNT[MAX_BLOOD_CASTLE_LEVEL];
	int							 m_iBC_GENERAL_SCRIPT_CNT[MAX_BLOOD_CASTLE_LEVEL];

	bool m_bBC_EVENT_ENABLE;	// E04
	int m_iBC_TIME_MIN_OPEN;	// E08
	int m_iBC_TIME_MIN_PLAY;	// E0C
	int m_iBC_TIME_MIN_REST;	// E10
	int m_iBC_MONSTER_REGEN;	// E1C
	bool m_bBC_RESTRICT_FINISH_ENABLE;
	int m_iBC_RESTRICT_FINISH_TIME;

	int m_iArchangelScrollDropRate;
	int m_iBloodBoneDropRate;

	std::list<BLOODCASTLE_START_TIME> m_listBloodCastleOpenTime; //season 4.5 add-on

};

static struct _BLOOODCASTLE_LEVEL
{
	int iLOWER_BOUND;	// 0
	int iUPPER_BOUND;	// 4
	int iLOWER_BOUND_MAGUMSA;	// 8
	int iUPPER_BOUND_MAGUMSA;	// C

} g_sttBLOODCASTLE_LEVEL[MAX_BLOOD_CASTLE_LEVEL] =
{
	// Blood Castle 1
	15, 80, // DK, DW, Elf
	10, 60, // MG, DL

	// Blood Castle 2
	81, 130, // DK, DW, Elf
	61, 110, // MG, DL

	// Blood Castle 3
	131, 180, // DK, DW, Elf
	111, 160, // MG, DL

	// Blood Castle 4
	181, 230, // DK, DW, Elf
	161, 210, // MG, DL

	// Blood Castle 5
	231, 280, // DK, DW, Elf
	211, 260, // MG, DL

	// Blood Castle 6
	281, 330, // DK, DW, Elf
	261, 310, // MG, DL

	// Blood Castle 7
	331, 400, // DK, DW, Elf
	311, 400, // MG, DL

	// Blood Castle 8
	400, 400,
	400, 400

};

static const int g_iBC_ChoasMixSuccessRate[MAX_BLOOD_CASTLE_LEVEL] = { 80, 80, 80, 80, 80, 80, 80, 80 };
static const int g_iBC_ChoasMixMoney[MAX_BLOOD_CASTLE_LEVEL] = { 50000 , 80000 , 150000, 250000, 400000, 600000, 850000, 1000000 };

extern CBloodCastle g_BloodCastle;

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

