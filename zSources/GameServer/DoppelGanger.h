//////////////////////////////////////////////////////////////////////
// DoppelGanger.h
#ifndef DOPPELGANGER_H
#define DOPPELGANGER_H

#include "MonsterAttr.h"
#include "MonsterHerd.h"
#include "user.h"
#include "DoppelGangerPosInfo.h"
#include "DoppelGangerEventInfo.h"
#include "DoppelGangerItemBag.h"
#include "MuLua.h"

#pragma once

#define MAX_DOPPELGANGER_MONSTER_INFO			200
#define MAX_DOPPELGANGER_MONSTER_ATTR			11
#define MAX_DOPPELGANGER_BOSS_INFO				4
#define MAX_DOPPELGANGER_USER_INFO				5
#define MAX_DOPPELGANGER_TREASURE_BOX			3
#define MAX_DOPPELGANGER_ICEWORKER_INFO			5
#define MAX_GOLDEN_LARVA						3
#define MAX_DOPPELGANGER_MONSTER_IN_PORTAL		3
#define MAX_TOTAL_GOLDEN_LARVA					(MAX_GOLDEN_LARVA*MAX_DOPPELGANGER_USER_INFO)

#define DOPPELGANGER_STATE_NONE					0
#define DOPPELGANGER_STATE_READY				1
#define DOPPELGANGER_STATE_PLAY					2
#define DOPPELGANGER_STATE_END					3

enum eDoppelGanger_Gates
{
	DoppelGanger_Gate_Snow = 329,
	DoppelGanger_Gate_Volcano = 330,
	DoppelGanger_Gate_Aqua = 331,
	DoppelGanger_Gate_Ground = 332
};

class CDoppelGangerMonsterHerd : public MonsterHerd
{
public:
	CDoppelGangerMonsterHerd(void);
	virtual ~CDoppelGangerMonsterHerd(void);

	BOOL	Start();
	BOOL	MonsterHerdItemDrop(OBJECTSTRUCT* lpObj);
	void	MonsterAttackAction(OBJECTSTRUCT *lpObj, OBJECTSTRUCT *lpTargetObj);
	int		AddMonsterEX(int iMonsterType, int bAttackFirst, int nMonsterLevel, int nMonsterHp, int nMonsterAttMin, int nMonsterAttMax, int nMonsterDef);
	BOOL	GetRandomLocation(BYTE &cX, BYTE &cY);
	BOOL	SetTotalInfo(int iMapNumber, int iRadius, int nPosInfo, int bMove);
	void	MonsterBaseAct(OBJECTSTRUCT* lpObj);
	BOOL	MoveHerd();
	void	ArrangeMonsterHerd();
	BOOL	IsActive();
	void	SetActive(BOOL bActive);
	void	SetPosIndex(int nPosIndex);
	int		GetMonsterSize();
	void	SetEndPosition(int nPosIndex);
	int		GetPosIndex();
	int		GetEndPosIndex();
	int		IsMove();

private:
	BYTE	m_btEndX;
	BYTE	m_btEndY;
	int		m_nPosIndex;
	int		m_nStartPosInfo;
	int		m_nEndPosInfo;
	BOOL	m_bMove;
};

struct __tagDOPPELGANGER_USERINFO
{
	__tagDOPPELGANGER_USERINFO()
	{
		this->m_nIndex = -1;
		this->m_nLevel = 0;
	}

	BOOL IsUser()
	{
		if ( ObjectMaxRange(this->m_nIndex) )
		{
			return TRUE;
		}

		return FALSE;
	}

	int		m_nIndex;
	int		m_nLevel;
};

struct PMSG_DOPPELGANGER_ENTER_RESULT
{
	PBMSG_HEAD2 h;
	BYTE btResult;
};

struct PMSG_DOPPELGANGER_STATE
{
	PBMSG_HEAD2 h;
	BYTE btState;
};

struct PMSG_DOPPELGANGER_ICEWORKER_INFO
{
	PBMSG_HEAD2 h;
	BYTE btIceWorkerState;
	BYTE btIceWorkerPos;
};

struct PMSG_DOPPELGANGER_GOAL_COUNT
{
	PBMSG_HEAD2 h;
	BYTE btMaxGoalCount;
	BYTE btCurGoalCount;
};

struct PMSG_DOPPELGANGER_MONSTER_POS
{
	PBMSG_HEAD2 h;
	BYTE btMonsterPos;
};

struct PMSG_DOPPELGANGER_USER_POS
{
	PBMSG_HEAD2 h;
	WORD wLeftTime;
	WORD wCount;
};

struct DOPPELGANGER_USER_POS
{
	WORD wIndex;
	BYTE btMapNumber;
	BYTE btPosIndex;
};

struct PMSG_DOPPELGANGER_MISSION_RESULT
{
	PBMSG_HEAD2 h;
	BYTE btMissionResult;
	int dwRewardExp;
};

class CDoppelGanger
{
public:
	CDoppelGanger(void);
	virtual ~CDoppelGanger(void);

	void DoppelgangerProcessInit();
	void DoppelgangerInfoLoad();

	void SetDoppelgangerState(BYTE btState);
	BYTE GetDoppelgangerState();
	void SetDoppelgangerStateNone();
	void SetDoppelgangerStateReady();
	void SetDoppelgangerStatePlaying();
	void SetDoppelgangerStateEnd();

	void ProcDoppelganger(ULONGLONG i64CurTime);
	void ProcDoppelgangerState_None(ULONGLONG i64CurTime);
	void ProcDoppelgangerState_Ready(ULONGLONG i64CurTime);
	void ProcDoppelgangerState_Playing(ULONGLONG i64CurTime);
	void ProcDoppelgangerState_End(ULONGLONG i64CurTime);

	BOOL EnterDoppelgangerEvent(int aIndex, BYTE btItemPos);
	BOOL LeaveDoppelganger(int aIndex);

	BOOL AddDoppelgangerUser(int aIndex);
	void DelDoppelgangerUser(int aIndex);

	void CalUserLevel();
	void ClearUserData();

	int GetUserMaxLevel();
	int GetUserMinLevel();

	int GetUserAverageLevel();
	void SetUserAverageLevel();

	int GetUserCount();
	int GetStartUserCount();

	void SendNoticeMessage(char* lpMsg);
	void SendDoppelgangerState(BYTE btState);

	void PlatformLugardAct(OBJECTSTRUCT *lpNpc, OBJECTSTRUCT *lpObj);
	void MiddleTreasureAct(OBJECTSTRUCT *lpNpc, OBJECTSTRUCT *lpObj);
	void LastTreasureAct(OBJECTSTRUCT *lpNpc, OBJECTSTRUCT *lpObj);

	BOOL OpenTreasureBox();
	BOOL OpenLastTreasureBox();

	int GetRandomValue(int nRange);

	int GetMonsterHerdIndex();
	void IncMonsterHerdIndex();

	void SetHerdStartPosInfo(int nHerdIndex, int nPosInfo, BOOL bMove);
	void SetHerdEndPosInfo(int nHerdIndex, int nPosInfo);

	void AddMonsterHerd(int nHerdIndex, int nMonsterClass, BOOL bAttackFirst);
	void AddMonsterBoss(int nHerdIndex, int nMonsterClass, BOOL bAttackFirst);
	void AddMonsterNormal(int nMonsterClass, BYTE posX, BYTE posY);

	void SetIceWorkerRegen(int nPosInfo);
	void AddIceWorkerIndex(int nIndex);
	void DelIceWorkerIndex(int nIndex);
	BOOL CheckIceWorker();

	void AddLarvaIndex(int nIndex);
	void DelLarvaAll();

	void MonsterHerdStart(int nHerdIndex);

	void CheckDoppelgangerMonsterPos(OBJECTSTRUCT *lpObj);
	int GetDoppelgangerPosIndex(BYTE btX, BYTE btY);

	void SendMonsterGoalCount();
	void SendDoppelgangerMonsterPos();
	void SendDoppelgangerUserPos();

	void SelfExplosion(int aIndex, CMagicInf *lpMagic, int aTargetIndex);

	void AngerKillerAttack(OBJECTSTRUCT *lpObj);

	void SendDoppelgangerResult(OBJECTSTRUCT *lpObj, BYTE btResult);
	void SendDoppelgangerResultAll();

	void ArrangeMonsterHerd();

	void MoveDoppelgangerMonsterProc();

	BOOL GetRandomLocation(BYTE *cX, BYTE *cY);
	BOOL GetRandomLocation(BYTE *cX, BYTE *cY, int Seed);

	BOOL AddMiddleTreasureBoxAll(BYTE btX, BYTE btY);
	int AddMiddleTreasureBox(int nMapNumber, BYTE btX, BYTE btY);
	int AddLastTreasureBox(BYTE btX, BYTE btY);

	int AddMonsterLarva(int nMapNumber, BYTE btX, BYTE btY, int nMonsterLevel, int nMonsterHp, int nMonsterAttMin, int nMonsterAttMax, int nMonsterDef);

	void DelTreasureBox(int nIndex, int nMonsterClass);

	int CheckMapTile(int nMapNumber, BYTE btX, BYTE btY);
	void SendMapTileInfo(OBJECTSTRUCT *lpObj, BYTE btMapSetType);
	void SendMapTileInfoAll(BYTE btMapSetType);

	LPMONSTER_ATTRIBUTE GetDefaultMonsterInfo(int nClass);

	int GetDefaultMonsterLevel(int nClass);
	int GetDefaultMonsterHp(int nClass);
	int GetDefaultMonsterAttackMax(int nClass);
	int GetDefaultMonsterAttackMin(int nClass);
	int GetDefaultMonsterDefense(int nClass);

	float GetMonsterLevelWeight();
	float GetMonsterHpWeight();
	float GetMonsterAttackWeight();
	float GetMonsterDefenseWeight();

	int GetMissionResult();

	void SetMiddleBossIndex(int nClass, int nIndex);
	void SetMiddleBossKill(int nClass, int nIndex);
	int GetMiddleBossKill(int nNumber);

	void SendDoppelgangerTimerMsg(BYTE btMsg);

	DWORD CalDoppelgangerRewardExp(int nIndex);

	void SaveDoppelgangerTmpInfo();

	void SetKillerState(BYTE btState) { this->m_btKillerState = btState; }
	void SetAngerKillerState(BYTE btState) { this->m_btAngerKillerState = btState; }
	BYTE GetKillerState() { return this->m_btKillerState; }

	int GetKillerHp() { return this->m_nKillerHp; }
	void SetKillerHp(int nHp) { this->m_nKillerHp = nHp; }

	BYTE GetAngerKillerState() { return this->m_btAngerKillerState; }
	int GetAngerKillerHp() { return this->m_nAngerKillerHp; }
	void SetAngerKillerHp(int nHp) { this->m_nAngerKillerHp = nHp; }

	void SetReadyTime(ULONGLONG time) { this->m_i64ReadyTime = time; }
	void SetPlayTime(ULONGLONG time) { this->m_i64PlayTime = time; }
	void SetEndTime(ULONGLONG end_time) { this->m_i64EndTime = end_time; }

	ULONGLONG GetStateTime() const { return this->m_i64StateTime; };

	DWORD GetMapNumber() const { return this->m_nMapNumber; }

	void SetAddHerdMonsterTime(ULONGLONG time) { this->m_i64AddHerdMonsterTime = time; }
	ULONGLONG GetAddHerdMonsterTime() const { return this->m_i64AddHerdMonsterTime; }

	void SetAddBossMonsterTime(ULONGLONG time) { this->m_i64AddBossMonsterTime = time; }
	ULONGLONG GetAddBossMonsterTime() const { return this->m_i64AddBossMonsterTime; }

	int GetBossRegenOrder() const { return this->m_nBossRegenOrder; }
	void SetBossRegenOrder(int regen_order){ this->m_nBossRegenOrder = regen_order; }

	void SetEventEnable(BOOL bEnable) { this->m_bDoppelGangerEnable = bEnable; }
	bool IsEventEnable() { return this->m_bDoppelGangerEnable; }

	float GetDoppelGangerExpRate() { return this->m_fDPExpRate; }
	float GetDoppelGangerMLExpRate() { return this->m_fDPMasterLevelExpRate; }

	int GetMirrorDimensionDropRate() { return this->m_iMirrorDimensionDropRate; }
	int GetMirrorDimensionDropLevel() { return this->m_iMirrorDimensionDropLevel; }

	CDoppelGangerPosInfo			m_PosInfo;

private:
	BOOL							m_bDoppelGangerEnable;
	float							m_fDPExpRate;
	float							m_fDPMasterLevelExpRate;
	int								m_iMirrorDimensionDropRate;
	int								m_iMirrorDimensionDropLevel;
	CDoppelGangerEventInfo			m_DPEventInfo;
	CDoppelGangerItemBag			m_DPDropItemInfo;
	MONSTER_ATTRIBUTE *				m_pDoppelGangerAttr[MAX_DOPPELGANGER_MONSTER_ATTR];
	BYTE							m_btDoppelGangerState;
	bool							m_bDoppelGangerEnter;
	bool							m_bSendMsgBeforeReady;
	bool							m_bSendMsgBeforePlay;
	bool							m_bSendIceWorkerDel;
	bool							m_bSendDoppelGangerEnd;
	ULONGLONG						m_i64ReadyTime;
	ULONGLONG						m_i64PlayTime;
	ULONGLONG						m_i64EndTime;
	CRITICAL_SECTION				m_critUserData;
	CRITICAL_SECTION				m_critTreasureBox;
	bool							m_bSuccessResult;
	bool							m_bMissionResult;
	DWORD							m_nMapNumber;
	DWORD							m_nGateNumber;
	int								m_nPartyNumber;
	DWORD							m_nCurUserCount;
	DWORD							m_nStartUserCount;
	ULONGLONG						m_i64UserEnterTime;
	DWORD							m_nMaxLevel;
	DWORD							m_nMinLevel;
	DWORD							m_nAverageLevel;
	ULONGLONG						m_i64StateTime;
	ULONGLONG						m_i64CallbackTime;
	ULONGLONG						m_i64AddHerdMonsterTime;
	ULONGLONG						m_i64AddBossMonsterTime;
	ULONGLONG						m_i64MonserPosSendTime;
	DWORD							m_nGoalMonsterCnt;
	int								m_nIceWorkerIndex[MAX_DOPPELGANGER_ICEWORKER_INFO];
	ULONGLONG						m_i64IceWorkerRegenTime;
	BYTE							m_btIceWorkState;
	BYTE							m_btKillerState;
	int								m_nKillerHp;
	BYTE							m_btAngerKillerState;
	int								m_nAngerKillerHp;
	int								m_nLarvaIndex[MAX_TOTAL_GOLDEN_LARVA];
	int								m_nMiddleBossIndex[MAX_DOPPELGANGER_BOSS_INFO];
	bool							m_bMiddleBossKill[MAX_DOPPELGANGER_BOSS_INFO];
	int								m_nTeasureBoxIndex[MAX_DOPPELGANGER_TREASURE_BOX];
	int								m_nLastTreasureBoxIndex;
	bool							m_bOpenTreasureBox;
	ULONGLONG						m_i64TeasureBoxTime;
	bool							m_bTreasureBoxRegen;
	bool							m_bOpenLastTreasureBox;
	int								m_nFirstMonsterIndex;
	BYTE							m_btFirstIndex;
	CRITICAL_SECTION				m_CSMonsterHerd;
	__tagDOPPELGANGER_USERINFO		m_UserData[MAX_DOPPELGANGER_USER_INFO];
	CDoppelGangerMonsterHerd		m_MonsterHerd[MAX_DOPPELGANGER_MONSTER_INFO];
	int								m_nHerdCnt;
	int								m_nBossRegenOrder;
	MULua							m_MULua;
};

extern CDoppelGanger g_DoppelGanger;

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

