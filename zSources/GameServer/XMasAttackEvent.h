//////////////////////////////////////////////////////////////////////
// XMasAttackEvent.h
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MonsterHerd.h"

#define MAX_XMASMONSTER_MAP		3

struct _tagAttackEventMapPosition
{

};

class CXMasMonsterHerd : public MonsterHerd
{

public:

	CXMasMonsterHerd();
	virtual ~CXMasMonsterHerd();

	void SendEventGiftWinner(int iIndex, int iGiftKind);
	BYTE GetX(){return this->m_iCUR_X;}
	BYTE GetY(){return this->m_iCUR_Y;}

	virtual int Start();	// 4
	virtual int MonsterHerdItemDrop(LPOBJ lpObj);	// 8
	virtual void MonsterAttackAction(LPOBJ lpObj, LPOBJ lpTargetObj);	// C

private:

	BOOL m_bEventSuccess;	// 48

};

struct XMASMONSTER_DATA
{
	int m_iType;	// 0
	BOOL m_bDoRegen;	// 4
	BOOL m_bDoAttackFirst;	// 8
};

struct XMASMONSTER_EVENT_TIME
{
	int m_iHour;	// 0
	int m_iMinute;	// 4
};

class CXMasAttackEvent
{

public:

	CXMasAttackEvent();
	virtual ~CXMasAttackEvent();

	void StartEvent();
	void StopEvent();
	void Init();
	void SetEventEnable(BOOL bEnable) { this->m_bDoEvent = bEnable; }
	void Run();
	void CheckSync();
	void SetState(int iSTATE);
	void ProcState_None();
	void ProcState_Closed();
	void ProcState_Playing();
	void SetState_None();
	void SetState_Closed();
	void SetState_Playing();
	void Move();
	void SetMenualStart(BOOL bMenualStart) {this->m_bMenualStart = bMenualStart;}
	void Start_Menual();
	void End_Menual();

	virtual int Load(char* lpszFileName);	// 4

	int GetSantaItemDropRate() { return this->m_iSantaItemDropRate; }
	int GetSantaBuffDuration() { return this->m_iSantaBuffDuration; }
	int GetRestBuffDuration() { return this->m_iRestBuffDuration; }

private:

	CXMasMonsterHerd m_XMasMonsterHerd[MAX_XMASMONSTER_MAP];	// 4
	BOOL m_bDoEvent;	// E8
	BOOL m_bHasData;	// EC
	BOOL m_bMonsterToDest;	// F0
	int m_iTIME_MSEC_REMAIN;	// F4
	int m_iTIME_MIN_OPEN;	// F8
	int m_iTIME_MIN_PLAY;	// FC
	int m_iTIME_MONSTER_TO_DEST;	// 100
	int m_iTIME_TICK_COUNT;	// 104
	int m_iTIME_NOTIFY_COUNT;	// 108
	int m_iEVENT_STATE;	// 10C
	int m_iMOVE_RAND_SIZE;	// 110
	int m_iRADIUS_MIN;	// 114
	int m_iRADIUS_MAX;	// 118
	std::vector<XMASMONSTER_DATA> m_vtMonsterAddData;	// 11C
	CRITICAL_SECTION m_critMonsterAddData;	// 12C
	std::vector<XMASMONSTER_EVENT_TIME> m_vtEventTime;	// 144
	BOOL m_bMenualStart;	// 154

	int m_iSantaItemDropRate;
	int m_iSantaBuffDuration;
	int m_iRestBuffDuration;
};

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

