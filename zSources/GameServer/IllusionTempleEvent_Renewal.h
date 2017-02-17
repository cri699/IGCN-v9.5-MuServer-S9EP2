//////////////////////////////////////////////////////////////////////
// IllusionTempleEvent_Renewal.h
#ifndef ITR_EVENT_H
#define ITR_EVENT_H

#include "IllusionTempleProcess_Renewal.h"
#include "user.h"
#include <list>

struct ITR_NPC_POSITION
{
	WORD wNpcType;
	BYTE byMapNumber;
	BYTE byDistance;
	BYTE byX;
	BYTE byY;
	BYTE byDir;
};

struct _ITR_STARTTIME
{
	BYTE nHour;
	BYTE nMin;
};

#pragma once
class CIllusionTempleEvent_Renewal
{
public:
	CIllusionTempleEvent_Renewal();
	virtual ~CIllusionTempleEvent_Renewal();

	int ITR_EventInit();
	int Load_ITR_EventInfo();
	void Load_ITR_NPC_Position(char* filename);
	void Load_ITR_Script(char* filename);
	void ITR_NPC_Add();

	void ITR_Run();

	void SetRelicsInventoryPos(BYTE btMapNumber, int aIndex, BYTE btPos);
	void SendRelicsUserInfo(OBJECTSTRUCT* lpObj);

	int Enter_ITR(int aIndex, BYTE byTempleIndex, BYTE bySlotNum);
	int Leave_ITR(int aIndex, BYTE btMapNumber);

	void ActRelicsGetOrRegister(OBJECTSTRUCT *lpNpc, OBJECTSTRUCT *lpObj, BYTE byMapNumber, BYTE byPrePacket, BYTE byAct, WORD wNpcType);
	void CancleStoneState(WORD wNpcType, BYTE byState, BYTE mapnumber);
	void DropRelicsItem(BYTE btMapNumber, int nIndex);

	BYTE GetIllusionTempleState(BYTE btMapNumber);
	void AddNpcPosNum();
	void ResetPosNum();

	BYTE GetUserTeam(BYTE btMapNumber, int nIndex);
	void SetStatusRegenTime(BYTE btMapNumber);

	void ITR_USeSkill(int iIndex, WORD MagicNumber, int wTargetObjIndex, BYTE btDis);
	void EventSkillProc(OBJECTSTRUCT* lpObj);

	void IllusionTempleUserDie(OBJECTSTRUCT *lpObj);
	void IllusionTempleUserDieRegen(OBJECTSTRUCT *lpObj);

	void SetMirageIndex(int nIndex);

	BOOL CheckWearingMOPH(int nUserIndex);
	BOOL CheckChangeRing(int nType);
	BOOL CheckRelics(int nIndex);
	void ReqEventReward(int nIndex);

	int GetRemainTime();

	BOOL CheckSkillProdection(int iIllusionTempleIndex, BYTE btMapNumber);
	BOOL CheckSkillRestraint(int iIllusionTempleIndex, BYTE btMapNumber);
	bool IsCanEnterTiming();
	void IncreaseKillCount(int nIndex, BYTE byMapNumber, BYTE byObjectType);

	int CheckEnterLevel(int nIndex);

	BYTE Find_EmptySlot(int PartyIndex, BYTE &byEmptySlot, int nEnterIllusionIndex);
	BYTE Update_PartyInfo(BYTE byMapNumber, BYTE bySlotNumber, int PartyIndex, int nUserIndex);
	BYTE Delete_PartyInfo(int nPartyIndex);
	BYTE Find_ExistingPartyInfo(int partyinfo, BYTE &byMapNum, BYTE &bySlotNum);

	void SetEventEnable(BOOL bEnable) { this->m_bITR_Event = bEnable; }
	bool IsEventEnable() { return this->m_bITR_Event; }

	int m_nITROpenTime;
	int m_nITRReadyTime;
	int m_nITRPlayTime;
	int m_nITREndTime;
	int m_iITR_MinUserInParty;
	std::list<_ITR_STARTTIME> m_lstALLITR_TIME;

private:
	CIllusionTempleProcess_Renewal m_cITR_Proc[6];
	int m_nMirageIndex;
	int m_bITR_Event;
	ITR_NPC_POSITION m_ITR_NPC[66];
};

extern CIllusionTempleEvent_Renewal g_IT_Event;

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

