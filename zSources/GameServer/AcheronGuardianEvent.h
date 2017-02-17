//////////////////////////////////////////////////////////////////////
// AcheronGuardianEvent.h
#pragma once
#ifndef ACHERONGUARDIANEVENT_H
#define ACHEROUNGARDIANEVENT_H

#include "prodef.h"
#include "user.h"

struct _tagAE_OBELISK_INFO
{
	_tagAE_OBELISK_INFO()
	{
		this->m_iGroupNumber = 0;
		this->m_iAttrKind = 0;
		this->m_iMapIndex = 0;
		this->m_iPosX = 0;
		this->m_iPosY = 0;
		this->m_iMonsterNum = 0;
		this->m_iMonIndex = -1;
	}

	void Clear()
	{
		this->m_iMonIndex = -1;
	}

	int m_iGroupNumber;
	int m_iAttrKind;
	int m_iMapIndex;
	int m_iPosX;
	int m_iPosY;
	int m_iMonsterNum;
	int m_iMonIndex;
};

struct _stAEMonIndexInf
{
	_stAEMonIndexInf()
	{
		this->m_nMonsterIndex = -1;
	}

	int m_nMonsterIndex;
	int m_nGenSqe;
	int m_nMonAttr;
};

struct _stAEMonRegenArea
{
	_stAEMonRegenArea()
	{
		this->m_nBeginX = -1;
		this->m_nBeginY = -1;
		this->m_nEndX = -1;
		this->m_nEndY = -1;
	}

	int m_nBeginX;
	int m_nBeginY;
	int m_nEndX;
	int m_nEndY;
};

struct _stAEMonPosition
{
	_stAEMonPosition()
	{
		this->m_nGroupNumber = -1;
		this->m_nRegenAreaCnt = 0;
	}

	int m_nGroupNumber;
	int m_nRegenAreaCnt;
	int m_nMonsterKindCnt;
	_stAEMonRegenArea m_stRegenArea[50];
	_stAEMonIndexInf m_stMonsterIndexInfo[20];
};

struct _stAEMonAccountNumInfo
{
	_stAEMonAccountNumInfo()
	{
		this->m_nIndex = -1;
	}

	int m_nIndex;
};

struct _stAEMonGroupInfo
{
	_stAEMonGroupInfo()
	{
		this->m_nGroupNumber = -1;
		this->m_nMapNumber = -1;
		this->m_nCnt = 0;
		this->m_i64_RegenTimeMin = 0;
		this->m_i64_NextRegenTime = 0;
		this->m_nGenTotCnt = 0;
	}

	int m_nGroupNumber;
	int m_nMapNumber;
	int m_nCnt;
	int m_nGenTotCnt;
	ULONGLONG m_i64_RegenTimeMin;
	ULONGLONG m_i64_NextRegenTime;
	_stAEMonAccountNumInfo m_stMonsterAccountNumInfo[50];
};

struct _tagAcheronEventTime
{
	int m_iHour;
	int m_iMinute;
};

// DS

struct _tagPMSG_REQ_AE_PLAY_DS
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;
	BYTE btPlay;
};

struct _tagPMSG_ANS_AE_PLAY_DS
{
	PBMSG_HEAD2 h;
	BYTE btPlay;
};

class CAcheronGuardianEvent
{
public:
	CAcheronGuardianEvent();
	virtual ~CAcheronGuardianEvent();

	void Clear();
	bool LoadScriptAcheronEvent(char *lpszFileName);
	bool CheckSync();

	void Init();

	void SetProcState(int iState);
	void Run();
	void ProcStateChannelClose();
	void ProcStateEnd();
	void ProcStateClosed();
	void ProcStateReady();
	void ProcStatePlaying();
	void ProcStatePlayEnd();

	void GenObelisk(int iGroupNum);
	void DelObelisk();
	void SetRandomObeliskAttr();

	void ResetGenTime();
	void RegenMonsterRun();
	void RegenMonster(int nGroupNumber, int iGenSeq, int iGenMonCnt);
	int SetPosMonster(int aIndex, int nMapNumber, int nBeginX, int nBeginY, int nEndX, int nEndY);
	int GetBoxPosition(int mapnumber, int ax, int ay, int aw, int ah, short &mx, short &my);
	void DeleteMonster(int nGroupNumber);
	void DeleteAcheronEventAllMonster();

	void DestroyObelisk(OBJECTSTRUCT *lpObj, BYTE btMapNumber, BYTE cX, BYTE cY, int nMaxHitUser);

	void CGReqAcheronEventEnter(_tagPMSG_REQ_ACHERON_EVENT_ENTER *lpMsg, int iIndex);

	void GDReqAcheronEventProcMultiCast(BYTE btPlay);
	void DGAnsAcheronEventProcMultiCast(_tagPMSG_ANS_AE_PLAY_DS *lpMsg);
	void SetAcheronEventProcMultiCast(int bAEInitState);

	void SendMapServerGroupMsg(void *lpszMsg);
	void SendMapServerGroupMsg(char *szMsg);

	void SetPlayStart(BYTE btAE_PLAY_START) { this->m_bAE_PLAY_START = btAE_PLAY_START; }
	int IsPlayStart() { return this->m_bAE_PLAY_START; }
	int GetProcState() { return this->m_iState; }
	int IsInitAEProcMultiCast() { return this->m_bAEInitState; }
	int IsAcheronGuardianDay();

	void SetEventEnable(BOOL bEnable) { this->m_bAEEnable = bEnable; }

private:
	bool m_bAEEnable;
	int m_bAEInitState;
	int m_iState;
	int m_iGroupNum;
	int m_bAE_PLAY_START;
	int m_iAB_PlayTimeMin;
	int m_iAB_ChCloseMin;
	int m_iDayOfWeekCycle[7];
	std::vector<_tagAcheronEventTime> m_vtAcheronEventTime;
	LONGLONG m_i64_REMAIN_MSEC;
	LONGLONG m_i64_TICK_COUNT;
	_tagAE_OBELISK_INFO m_stObeliskInfo[5];
	_stAEMonPosition m_stMonsterPosition[5];
	_stAEMonGroupInfo m_stMonsterGroupInfo[5];
	bool m_bWindowClose;
	int m_iCheatAttr;
};

extern CAcheronGuardianEvent g_AcheronGuardianEvent;

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

