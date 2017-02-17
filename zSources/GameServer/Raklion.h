// Raklion.h: interface for the CRaklion class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RAKLION_H__E5508AFB_5EEC_464E_A811_C970F2AEE3F6__INCLUDED_)
#define AFX_RAKLION_H__E5508AFB_5EEC_464E_A811_C970F2AEE3F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RaklionBattleOfSelupan.h"

enum RAKLION_STATE
{
	RAKLION_STATE_IDLE = 0x0,
	RAKLION_STATE_NOTIFY_1 = 0x1,
	RAKLION_STATE_STANDBY = 0x2,
	RAKLION_STATE_NOTIFY_2 = 0x3,
	RAKLION_STATE_READY = 0x4,
	RAKLION_STATE_START_BATTLE = 0x5,
	RAKLION_STATE_NOTIFY_3 = 0x6,
	RAKLION_STATE_CLOSE_DOOR = 0x7,
	RAKLION_STATE_ALL_USER_DIE = 0x8,
	RAKLION_STATE_NOTIFY_4 = 0x9,
	RAKLION_STATE_END = 0xa,
	RAKLION_STATE_DETAIL_STATE = 0xb,
	RAKLION_STATE_MAX = 0xc,
};

class CRaklion  
{
	BOOL m_bRaklionBossEnable;
	RAKLION_STATE m_iRaklionState;
	int m_iRaklionStateDetail;
	BOOL m_bIsBossKillSuccess;
	ULONGLONG m_iBossAppearanceStartTime;
	ULONGLONG m_iBosszoneCloseStartTime;
	ULONGLONG m_iBosszoneOpenStartTime;
	BOOL m_bFileDataLoad;
	int m_iBossAppearanceDelay;
	int m_iBosszoneCloseDelay;
	int m_iBosszoneOpenDelay;
	int m_iBossEggHalf;
	int m_iBossEggMax;
	CRaklionBattleOfSelupan m_BattleOfSelupan;
	BOOL m_bIsNotify1;
	int m_iBossEggDieCount;
	BOOL m_bEvent;
	int m_iTodayYear;
	int m_iTodayMon;
	int m_iTodayDay;
public:
	CRaklion();
	virtual ~CRaklion();

	void Init();
	void ResetAllData();
	
	BOOL LoadData(char* lpszFileName);
	void LoadRaklionMapAttr(char* lpszFileName, BYTE btLevel);
	
	void Run();
	
	void SetState(int iRaklionState);
	void SetNextState(RAKLION_STATE iCurrentState);
	void ChangeState(int iState, int DetailState);
	
	void SetState_IDLE();
	void SetState_NOTIFY_1();
	void SetState_STANDBY();
	void SetState_NOTIFY_2();
	void SetState_READY();
	void SetState_START_BATTLE();
	void SetState_NOTIFY_3();
	void SetState_CLOSE_DOOR();
	void SetState_ALL_USER_DIE();
	void SetState_NOTIFY_4();
	void SetState_END();
	
	void ProcState_IDLE();
	void ProcState_NOTIFY_1();
	void ProcState_STANDBY();
	void ProcState_NOTIFY_2();
	void ProcState_READY();
	void ProcState_START_BATTLE();
	void ProcState_NOTIFY_3();
	void ProcState_CLOSE_DOOR();
	void ProcState_ALL_USER_DIE();
	void ProcState_NOTIFY_4();
	void ProcState_END();
	
	void SetRaklionBossEnable(BOOL bEnable);
	BOOL IsRaklionBossEnable();
	
	void SetRaklionState(RAKLION_STATE iRaklionState);
	int GetRaklionState();
	
	void SetRaklionStateDetail(int iSelupanState);
	int GetRaklionStateDetail();
	
	void SetBossKillValue(int);
	int GetBossKillValue();
	
	void CheckUserOnRaklionBossMap();
	BOOL CheckEnterRaklion(int iUserIndex);
	BOOL CheckCanEnterRaklionBattle();
	
	void BossEggDieIncrease();
	void BossEggDieDecrease();
	DWORD GetBossEggDieCount();
	void RegenBossEgg();
	void DeleteBossEgg();

	void OperateGmCommand(int iUserIndex, int iCommand);
	void UserMonsterCountCheck();

};

extern CRaklion	g_Raklion;

#endif // !defined(AFX_RAKLION_H__E5508AFB_5EEC_464E_A811_C970F2AEE3F6__INCLUDED_)


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

