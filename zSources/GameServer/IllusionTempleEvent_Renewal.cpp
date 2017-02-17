//////////////////////////////////////////////////////////////////////
// IllusionTempleEvent_Renewal.cpp
#include "stdafx.h"
#include "IllusionTempleEvent_Renewal.h"
#include "configread.h"
#include "TLog.h"
#include "GameMain.h"

CIllusionTempleEvent_Renewal g_IT_Event;

CIllusionTempleEvent_Renewal::CIllusionTempleEvent_Renewal()
{
}

CIllusionTempleEvent_Renewal::~CIllusionTempleEvent_Renewal()
{
}

int CIllusionTempleEvent_Renewal::ITR_EventInit()
{
	if (g_ConfigRead.server.GetStateFromEventTable(g_ConfigRead.server.GetServerType(), EV_TABLE_IT) == false)
	{
		this->SetEventEnable(false);
		return FALSE;
	}

	for (int i = 0; i < 6; i++)
	{
		this->m_cITR_Proc[i].ITR_ProcessInit(i);
	}

	this->ITR_NPC_Add();
	return TRUE;
}

int CIllusionTempleEvent_Renewal::Load_ITR_EventInfo()
{
	this->Load_ITR_NPC_Position(g_ConfigRead.GetPath("\\Events\\IllusionTemple_Renewal\\IGC_IllusionTemple_Renewal_NPC.xml"));
	this->Load_ITR_Script(g_ConfigRead.GetPath("\\Events\\IllusionTemple_Renewal\\IGC_IllusionTemple_Renewal.xml"));

	return TRUE;
}

void CIllusionTempleEvent_Renewal::Load_ITR_NPC_Position(char *filename)
{
	if (g_ConfigRead.server.GetStateFromEventTable(g_ConfigRead.server.GetServerType(), EV_TABLE_IT) == false)
	{
		return;
	}

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(filename);

	if (res.status != pugi::status_ok)
	{
		g_Log.MsgBox("[ ITR ] IllusionTempleRenewalNpc.xml File Load Fail %s (%s)", filename, res.description());
		return;
	}

	pugi::xml_node main = file.child("IllusionTempleRenewal");
	int nCount = 0;

	for (pugi::xml_node npc = main.child("NPC"); npc; npc = npc.next_sibling())
	{
		this->m_ITR_NPC[nCount].wNpcType = npc.attribute("Index").as_int();
		this->m_ITR_NPC[nCount].byMapNumber = npc.attribute("MapNumber").as_int();
		this->m_ITR_NPC[nCount].byDistance = npc.attribute("Distance").as_int();
		this->m_ITR_NPC[nCount].byX = npc.attribute("StartX").as_int();
		this->m_ITR_NPC[nCount].byY = npc.attribute("StartY").as_int();
		this->m_ITR_NPC[nCount].byDir = npc.attribute("Dir").as_int();

		nCount++;
	}
}

void CIllusionTempleEvent_Renewal::Load_ITR_Script(char* filename)
{
	this->m_lstALLITR_TIME.clear();

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(filename);

	if (res.status != pugi::status_ok)
	{
		g_Log.MsgBox("[Illusion Temple] Info file Load Fail [%s] [%s]", filename, res.description());
		return;
	}

	pugi::xml_node itr = file.child("IllusionTempleRenewal");
	pugi::xml_node time = itr.child("Time");
	pugi::xml_node schedule = itr.child("EventSchedule");

	this->SetEventEnable(g_ConfigRead.server.GetStateFromEventTable(g_ConfigRead.server.GetServerType(), EV_TABLE_IT) == true ? itr.attribute("Enable").as_bool() : false);
	CIllusionTempleEvent_Renewal::m_iITR_MinUserInParty = itr.attribute("MinUserInParty").as_int();

	this->m_nITROpenTime = time.attribute("StartIn").as_int();
	this->m_nITRReadyTime = time.attribute("WaitingArea").as_int();
	this->m_nITRPlayTime = time.attribute("Duration").as_int();
	this->m_nITREndTime = time.attribute("TownSpawn").as_int();

	_ITR_STARTTIME startTime;
	
	for (pugi::xml_node start = schedule.child("Start"); start; start = start.next_sibling())
	{
		startTime.nHour = start.attribute("Hour").as_int();
		startTime.nMin = start.attribute("Minute").as_int();

		this->m_lstALLITR_TIME.push_back(startTime);
	}
}

void CIllusionTempleEvent_Renewal::ITR_NPC_Add()
{
	this->AddNpcPosNum();
}

void CIllusionTempleEvent_Renewal::ITR_Run()
{
	if (g_IT_Event.IsEventEnable() == false)
	{
		return;
	}

	int nCurTime = GetTickCount();
	bool bCheckSync = false;

	if (this->m_cITR_Proc[0].GetRealRemainTimeMin() == 0 &&
		this->m_cITR_Proc[0].GetAllCheckSync() == false)
	{
		bCheckSync = true;
		this->m_cITR_Proc[0].SetAllCheckSync();
	}

	for (int i = 0; i < 6; i++)
	{
		if (bCheckSync == TRUE)
		{
			this->m_cITR_Proc[i].CheckSync();
		}

		this->m_cITR_Proc[i].Proc_ITR(nCurTime);
	}
}

void CIllusionTempleEvent_Renewal::SetRelicsInventoryPos(BYTE btMapNumber, int aIndex, BYTE btPos)
{
	if (!IT_MAP_RANGE(btMapNumber))
	{
		return;
	}

	this->m_cITR_Proc[btMapNumber - 45].SetRelicsInventoryPos(aIndex, btPos);
}

void CIllusionTempleEvent_Renewal::SendRelicsUserInfo(OBJECTSTRUCT* lpObj)
{
	if (!IT_MAP_RANGE(lpObj->MapNumber))
	{
		return;
	}

	this->m_cITR_Proc[lpObj->MapNumber - 45].SendRelicsUserInfo(lpObj, 0);
}

BOOL CIllusionTempleEvent_Renewal::Enter_ITR(int aIndex, BYTE byTempleIndex, BYTE bySlotNum)
{
	if (g_IT_Event.IsEventEnable() == false)
	{
		return FALSE;
	}

	if (byTempleIndex < 0 || byTempleIndex > 5)
	{
		return FALSE;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (!lpObj)
	{
		return FALSE;
	}

	if (this->m_cITR_Proc[byTempleIndex].EnterUserIllusionTemple(aIndex, byTempleIndex, lpObj->PartyNumber) == TRUE)
	{
		g_Log.Add("[ ITR ] ITR index:(%d) EnterUser: (%s)(%s) class:%d",
			byTempleIndex + 1, lpObj->AccountID, lpObj->Name, lpObj->m_PlayerData->DbClass);

		this->m_cITR_Proc[byTempleIndex].SendIllusionTempleState(0, lpObj->m_Index);

		return TRUE;
	}

	else
	{
		g_Log.Add("[ ITR ] ITR index:(%d) EnterUser FAIL: (%s)(%s) class:%d",
			byTempleIndex + 1, lpObj->AccountID, lpObj->Name, lpObj->m_PlayerData->DbClass);

		return FALSE;
	}
}

int CIllusionTempleEvent_Renewal::Leave_ITR(int aIndex, BYTE btMapNumber)
{
	if (!IT_MAP_RANGE(btMapNumber))
	{
		return FALSE;
	}

	this->m_cITR_Proc[btMapNumber - 45].LeaveUserIllusionTemple(aIndex);
	return TRUE;
}

void CIllusionTempleEvent_Renewal::ActRelicsGetOrRegister(OBJECTSTRUCT *lpNpc, OBJECTSTRUCT *lpObj, BYTE byMapNumber, BYTE byPrePacket, BYTE byAct, WORD wNpcType)
{
	if (!IT_MAP_RANGE(byMapNumber))
	{
		return;
	}

	if (byPrePacket || byAct)
	{
		if (byPrePacket != 1 || byAct)
		{
			if (byPrePacket || byAct != 1)
			{
				if (byPrePacket == 1)
				{
					if (byAct == 1)
					{
						this->m_cITR_Proc[byMapNumber - 45].RegisterRelics(lpNpc, lpObj);
					}
				}
			}

			else
			{
				this->m_cITR_Proc[byMapNumber - 45].Check_RegisterRelics(lpNpc, lpObj);
			}
		}

		else
		{
			this->m_cITR_Proc[byMapNumber - 45].GetRelics(lpNpc, lpObj, wNpcType);
		}
	}

	else
	{
		this->m_cITR_Proc[byMapNumber - 45].Check_GetRelics(lpNpc, lpObj, wNpcType);
	}
}

void CIllusionTempleEvent_Renewal::CancleStoneState(WORD wNpcType, BYTE byState, BYTE mapnumber)
{
	if (!IT_MAP_RANGE(mapnumber))
	{
		return;
	}

	if (this->m_cITR_Proc[mapnumber - 45].GetIllusionTempleState() == 2)
	{
		this->m_cITR_Proc[mapnumber - 45].CancleStoneStateProcess(wNpcType, byState);
	}
}

void CIllusionTempleEvent_Renewal::DropRelicsItem(BYTE btMapNumber, int nIndex)
{
	if (!IT_MAP_RANGE(btMapNumber))
	{
		return;
	}

	this->m_cITR_Proc[btMapNumber - 45].DropRelicsItem(nIndex, 1);
}

BYTE CIllusionTempleEvent_Renewal::GetIllusionTempleState(BYTE btMapNumber)
{
	return this->m_cITR_Proc[btMapNumber - 45].GetIllusionTempleState();
}

void CIllusionTempleEvent_Renewal::AddNpcPosNum()
{
	for (int i = 0; i < 6; i++)
	{
		this->m_cITR_Proc[i].ResetITRNpcInfo();
	}

	for (int i = 0; i < 66; i++)
	{
		if (!IT_MAP_RANGE(this->m_ITR_NPC[i].byMapNumber))
		{
			g_Log.MsgBox("[ ITR ] Npc has Wrong Map Number (%d)", this->m_ITR_NPC[i].byMapNumber);
			return;
		}

		this->m_cITR_Proc[this->m_ITR_NPC[i].byMapNumber - 45].AddITRNpcInfo(
			this->m_ITR_NPC[i].wNpcType,
			this->m_ITR_NPC[i].byMapNumber,
			this->m_ITR_NPC[i].byDistance,
			this->m_ITR_NPC[i].byX,
			this->m_ITR_NPC[i].byY,
			this->m_ITR_NPC[i].byDir);
	}
}

void CIllusionTempleEvent_Renewal::ResetPosNum()
{
	for (int i = 0; i < 6; i++)
	{
		this->m_cITR_Proc[i].ResetITRPosNum();
	}
}

BYTE CIllusionTempleEvent_Renewal::GetUserTeam(BYTE btMapNumber, int nIndex)
{
	if (!IT_MAP_RANGE(btMapNumber))
	{
		return -1;
	}

	return this->m_cITR_Proc[btMapNumber - 45].GetUserTeam(nIndex);
}

void CIllusionTempleEvent_Renewal::SetStatusRegenTime(BYTE btMapNumber)
{
	if (!IT_MAP_RANGE(btMapNumber))
	{
		return;
	}

	this->m_cITR_Proc[btMapNumber - 45].SetStatusRegenTime();
}

void CIllusionTempleEvent_Renewal::ITR_USeSkill(int iIndex, WORD MagicNumber, int wTargetObjIndex, BYTE btDis)
{
	if (!ObjectMaxRange(iIndex))
	{
		g_Log.Add("ITR_UseSkill Error #1 iIndex :%d ", iIndex);
		return;
	}

	if (!IT_MAP_RANGE(gObj[iIndex].MapNumber))
	{
		g_Log.Add("ITR_UseSkill Error #2 iIndex :%d Map:%d ", iIndex, gObj[iIndex].MapNumber);
		return;
	}

	this->m_cITR_Proc[gObj[iIndex].MapNumber - 45].ITR_UseSkill(iIndex, MagicNumber, wTargetObjIndex, btDis);
}

void CIllusionTempleEvent_Renewal::EventSkillProc(OBJECTSTRUCT* lpObj)
{
	return this->m_cITR_Proc[lpObj->MapNumber - 45].ITRSkillProc(lpObj);
}

void CIllusionTempleEvent_Renewal::IllusionTempleUserDie(OBJECTSTRUCT *lpObj)
{
	this->m_cITR_Proc[lpObj->MapNumber - 45].ITRUserDie(lpObj);
}

void CIllusionTempleEvent_Renewal::IllusionTempleUserDieRegen(LPOBJ lpObj)
{
	if (!ObjectMaxRange(lpObj->m_Index))
	{
		return;
	}

	if (!IT_MAP_RANGE(lpObj->MapNumber))
	{
		return;
	}

	if (this->m_cITR_Proc[lpObj->MapNumber - 45].GetIllusionTempleState() == 2)
	{
		this->m_cITR_Proc[lpObj->MapNumber - 45].UseSkillProdection(lpObj);
	}
}

void CIllusionTempleEvent_Renewal::SetMirageIndex(int nIndex)
{
	this->m_nMirageIndex = nIndex;
}

BOOL CIllusionTempleEvent_Renewal::CheckWearingMOPH(int nUserIndex)
{
	if (!gObjIsConnected(nUserIndex))
		return FALSE;

	if (gObj[nUserIndex].pInventory[10].IsItem() == TRUE)
	{
		if (gObj[nUserIndex].pInventory[10].m_Type == ITEMGET(13,10))
			return TRUE;
		if (gObj[nUserIndex].pInventory[10].m_Type == ITEMGET(13,39))
			return TRUE;
		if (gObj[nUserIndex].pInventory[10].m_Type == ITEMGET(13,40))
			return TRUE;
		if (gObj[nUserIndex].pInventory[10].m_Type == ITEMGET(13,41))
			return TRUE;
		if (gObj[nUserIndex].pInventory[10].m_Type == ITEMGET(13,68))
			return TRUE;
		if (gObj[nUserIndex].pInventory[10].m_Type == ITEMGET(13,76))
			return TRUE;
		if (gObj[nUserIndex].pInventory[10].m_Type == ITEMGET(13,77))
			return TRUE;
		if (gObj[nUserIndex].pInventory[10].m_Type == ITEMGET(13,78))
			return TRUE;
		if (gObj[nUserIndex].pInventory[10].m_Type == ITEMGET(13,122))
			return TRUE;
		if (gObj[nUserIndex].pInventory[10].m_Type == ITEMGET(13,163))
			return TRUE;
		if (gObj[nUserIndex].pInventory[10].m_Type == ITEMGET(13,164))
			return TRUE;
		if (gObj[nUserIndex].pInventory[10].m_Type == ITEMGET(13,166))
			return TRUE;
		if (gObj[nUserIndex].pInventory[10].m_Type == ITEMGET(13,169))
			return TRUE;
		if (gObj[nUserIndex].pInventory[10].m_Type == ITEMGET(13,170))
			return TRUE;
		if (gObj[nUserIndex].pInventory[10].m_Type == ITEMGET(13,268))
			return TRUE;
	}

	if (gObj[nUserIndex].pInventory[11].IsItem() == TRUE)
	{
		if (gObj[nUserIndex].pInventory[11].m_Type == ITEMGET(13,10))
			return TRUE;
		if (gObj[nUserIndex].pInventory[11].m_Type == ITEMGET(13,39))
			return TRUE;
		if (gObj[nUserIndex].pInventory[11].m_Type == ITEMGET(13,40))
			return TRUE;
		if (gObj[nUserIndex].pInventory[11].m_Type == ITEMGET(13,41))
			return TRUE;
		if (gObj[nUserIndex].pInventory[11].m_Type == ITEMGET(13,68))
			return TRUE;
		if (gObj[nUserIndex].pInventory[11].m_Type == ITEMGET(13,76))
			return TRUE;
		if (gObj[nUserIndex].pInventory[11].m_Type == ITEMGET(13,77))
			return TRUE;
		if (gObj[nUserIndex].pInventory[11].m_Type == ITEMGET(13,78))
			return TRUE;
		if (gObj[nUserIndex].pInventory[11].m_Type == ITEMGET(13,122))
			return TRUE;
		if (gObj[nUserIndex].pInventory[11].m_Type == ITEMGET(13,163))
			return TRUE;
		if (gObj[nUserIndex].pInventory[11].m_Type == ITEMGET(13,164))
			return TRUE;
		if (gObj[nUserIndex].pInventory[11].m_Type == ITEMGET(13,166))
			return TRUE;
		if (gObj[nUserIndex].pInventory[11].m_Type == ITEMGET(13,169))
			return TRUE;
		if (gObj[nUserIndex].pInventory[11].m_Type == ITEMGET(13,170))
			return TRUE;
		if (gObj[nUserIndex].pInventory[11].m_Type == ITEMGET(13,268))
			return TRUE;
	}
	
	return FALSE;
}

BOOL CIllusionTempleEvent_Renewal::CheckChangeRing(int nType)
{
	int bRet = 0;
	switch (nType)
	{
		case ITEMGET(13,10):
		case ITEMGET(13,39):
		case ITEMGET(13,40):
		case ITEMGET(13,41):
		case ITEMGET(13,68):
		case ITEMGET(13,76):
		case ITEMGET(13,77):
		case ITEMGET(13,78):
		case ITEMGET(13,122):
		case ITEMGET(13,163):
		case ITEMGET(13,164):
		case ITEMGET(13,165):
		case ITEMGET(13,166):
		case ITEMGET(13,169):
		case ITEMGET(13,170):
		case ITEMGET(13,268):
			bRet = 1;
			break;
		default:
			return bRet;
	}
	return bRet;
}

BOOL CIllusionTempleEvent_Renewal::CheckRelics(int nIndex)
{
	if (!gObjIsConnected(nIndex))
	{
		return FALSE;
	}

	if (!ObjectMaxRange(nIndex))
	{
		return FALSE;
	}

	if (!IT_MAP_RANGE(gObj[nIndex].MapNumber))
	{
		return FALSE;
	}

	if (this->m_cITR_Proc[gObj[nIndex].MapNumber - 45].GetIllusionTempleState() != 2)
	{
		return FALSE;
	}

	if (nIndex == this->m_cITR_Proc[gObj[nIndex].MapNumber - 45].GetRelicsUserIndex(&gObj[nIndex]))
	{
		return TRUE;
	}

	return FALSE;
}

void CIllusionTempleEvent_Renewal::ReqEventReward(int nIndex)
{
	if (!IT_MAP_RANGE(gObj[nIndex].MapNumber))
	{
		return;
	}

	this->m_cITR_Proc[gObj[nIndex].MapNumber - 45].DropITR_RewardBox(nIndex);
}

int CIllusionTempleEvent_Renewal::GetRemainTime()
{
	if (g_IT_Event.IsEventEnable() == false)
	{
		return 0;
	}

	return this->m_cITR_Proc[0].GetEnterRemainTimeMin();
}

BOOL CIllusionTempleEvent_Renewal::CheckSkillProdection(int iIllusionTempleIndex, BYTE btMapNumber)
{
	if (!IT_MAP_RANGE(btMapNumber))
	{
		return FALSE;
	}

return this->m_cITR_Proc[btMapNumber - 45].CheckSkillProdection(iIllusionTempleIndex);
}

BOOL CIllusionTempleEvent_Renewal::CheckSkillRestraint(int iIllusionTempleIndex, BYTE btMapNumber)
{
	if (!IT_MAP_RANGE(btMapNumber))
	{
		return FALSE;
	}

	return this->m_cITR_Proc[btMapNumber - 45].CheckSkillRestraint(iIllusionTempleIndex);
}

bool CIllusionTempleEvent_Renewal::IsCanEnterTiming()
{
	return this->m_cITR_Proc[0].GetIllusionTempleState() == 0 && this->m_cITR_Proc[0].GetEntrance() == true;
}

void CIllusionTempleEvent_Renewal::IncreaseKillCount(int nIndex, BYTE byMapNumber, BYTE byObjectType)
{
	if (!ObjectMaxRange(nIndex))
	{
		return;
	}

	if (!IT_MAP_RANGE(byMapNumber))
	{
		return;
	}

	if (byObjectType == OBJ_USER)
	{
		this->m_cITR_Proc[byMapNumber - 45].IncUserKillCount(nIndex);
	}
}

int CIllusionTempleEvent_Renewal::CheckEnterLevel(int nIndex)
{
	int nIllusionTempleNumber;
	if (gObj[nIndex].Level >= 220)
	{
		if (gObj[nIndex].Level >= 271)
		{
			if (gObj[nIndex].Level >= 321)
			{
				if (gObj[nIndex].Level >= 351)
				{
					if (gObj[nIndex].Level >= 381)
					{
						if (gObj[nIndex].Level >= 400)
						{
							if (gObj[nIndex].Level != 400 || gObj[nIndex].m_PlayerData->ChangeUP == 2)
							{
								if (gObj[nIndex].Level == 400 && gObj[nIndex].m_PlayerData->ChangeUP)
									nIllusionTempleNumber = 5;
								else
									nIllusionTempleNumber = -1;
							}
							else
							{
								nIllusionTempleNumber = 4;
							}
						}
						else
						{
							nIllusionTempleNumber = 4;
						}
					}
					else
					{
						nIllusionTempleNumber = 3;
					}
				}
				else
				{
					nIllusionTempleNumber = 2;
				}
			}
			else
			{
				nIllusionTempleNumber = 1;
			}
		}
		else
		{
			nIllusionTempleNumber = 0;
		}
	}
	else
	{
		nIllusionTempleNumber = -1;
	}
	return nIllusionTempleNumber;
}

BYTE CIllusionTempleEvent_Renewal::Find_EmptySlot(int PartyIndex, BYTE &byEmptySlot, int nEnterIllusionIndex)
{
	BYTE bReturn = this->m_cITR_Proc[nEnterIllusionIndex].FindEmptyPartySlot(PartyIndex, byEmptySlot);

	if (byEmptySlot && byEmptySlot != 1 || bReturn)
	{
		return bReturn;
	}

	else
	{
		return nEnterIllusionIndex;
	}
}

BYTE CIllusionTempleEvent_Renewal::Update_PartyInfo(BYTE byMapNumber, BYTE bySlotNumber, int PartyIndex, int nUserIndex)
{
	return this->m_cITR_Proc[byMapNumber].SetPartyTeam(bySlotNumber, PartyIndex, nUserIndex);
}

BYTE CIllusionTempleEvent_Renewal::Delete_PartyInfo(int nPartyIndex)
{
	for (int i = 0; i < 6; i++)
	{
		if (!this->m_cITR_Proc[i].DeletePartyInfo(nPartyIndex))
		{
			return FALSE;
		}
	}

	return TRUE;
}

BYTE CIllusionTempleEvent_Renewal::Find_ExistingPartyInfo(int partyinfo, BYTE &byMapNum, BYTE &bySlotNum)
{
	BYTE bReturn1 = -1;
	BYTE bReturn2 = -1;

	for (int MapNum = 0; MapNum < 6; MapNum++)
	{
		int nTempNotUseIndex = -1;
		bReturn1 = this->m_cITR_Proc[MapNum].Find_SameParty(partyinfo, nTempNotUseIndex);

		if (bReturn1 == 0)
		{
			byMapNum = MapNum;
			bReturn2 = this->m_cITR_Proc[MapNum].Find_PartySlot(partyinfo, bySlotNum);

			if (bReturn2 == 0)
			{
				return 0;
			}
		}
	}

	return -1;
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

