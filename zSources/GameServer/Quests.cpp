//////////////////////////////////////////////////////////////////////
// Quests.cpp
#include "stdafx.h"
#include "Quests.h"
#include "ReadScript.h"
#include "winutil.h"
#include "TLog.h"
#include "GameMain.h"
#include "MasterSkillSystemS6.h"
#include "user.h"
#include "DSProtocol.h"
#include "BuffEffect.h"
#include "BuffEffectSlot.h"
#include "TRandomPoolMgr.h"
#include "gObjMonster.h"
#include "IllusionTempleEvent.h"
#include "BloodCastle.h"
#include "ChaosCastle.h"
#include "DevilSquare.h"
#include "AttributeSystem.h"
#include "GensSystem.h"

CQuests g_Quests;

CQuests::CQuests(void)
{
}

CQuests::~CQuests(void)
{
}

void CQuests::LoadData(LPSTR lpszFile)
{
	SMDFile = fopen(lpszFile, "r");

	if(SMDFile == NULL)
	{
		g_Log.MsgBox("%s load fail", lpszFile);
		return;
	}

	int Token;
	int iType;
	int count = 0;
	int count2 = 0;

	BYTE btGUID;
	BYTE btINDEX;
	BYTE btSTATE;
	BYTE btID;

	while ( true )
	{
		Token = GetToken();

		if ( Token == END )
			break;

		iType = TokenNumber;

		while ( true )
		{
			Token = GetToken();

			if ( Token == END )
				break;
		
			if ( iType == 1 )
			{
				if ( !strcmp("end", TokenString) )
					break;

				btGUID = TokenNumber;

				Token = GetToken();
				btINDEX = TokenNumber;

				Token = GetToken();
				btSTATE = TokenNumber;

				if(btINDEX > 0 && btINDEX < 21)
				{
					this->m_Quests[btINDEX][btSTATE].btINDEX = btINDEX;
					this->m_Quests[btINDEX][btSTATE].btSTATE = btSTATE;
				}

				else
				{
					g_Log.MsgBox("%s load fail - error in file", lpszFile);
					fclose(SMDFile);
					return;
				}

				Token = GetToken();
				this->m_Quests[btINDEX][btSTATE].btDIALOG = TokenNumber;

				Token = GetToken();
				this->m_Quests[btINDEX][btSTATE].btANSFIRST = TokenNumber;

				Token = GetToken();
				this->m_Quests[btINDEX][btSTATE].btANSSECOND = TokenNumber;

				Token = GetToken();
				this->m_Quests[btINDEX][btSTATE].btANSTHIRD = TokenNumber;

				Token = GetToken();
				this->m_Quests[btINDEX][btSTATE].wMINLEVEL = TokenNumber;

				Token = GetToken();
				this->m_Quests[btINDEX][btSTATE].wMAXLEVEL = TokenNumber;

				for(int i=0;i<7;i++)
				{
					Token = GetToken();
					this->m_Quests[btINDEX][btSTATE].btCLASS[i] = TokenNumber;
				}

				Token = GetToken();
				this->m_Quests[btINDEX][btSTATE].wNPCID = TokenNumber;

				for(int i=0;i<7;i++)
				{
					Token = GetToken();
					this->m_Quests[btINDEX][btSTATE].btDIALOGCLASS[i] = TokenNumber;
				}

				Token = GetToken();
				this->m_Quests[btINDEX][btSTATE].btRESETID = TokenNumber;

				Token = GetToken();
				this->m_Quests[btINDEX][btSTATE].btSTAGE = TokenNumber;

				Token = GetToken();
				this->m_Quests[btINDEX][btSTATE].btREQSTAGE = TokenNumber;

				Token = GetToken();
				this->m_Quests[btINDEX][btSTATE].btEVENTLEVEL = TokenNumber;

				Token = GetToken();
				this->m_Quests[btINDEX][btSTATE].btRANDOMREWARD = TokenNumber;
			}

			else if ( iType == 2 )
			{
				if ( !strcmp("end", TokenString) )
					break;

				Token = GetToken();
				btINDEX = TokenNumber;

				Token = GetToken();
				btSTATE = TokenNumber;

				if(btINDEX < 0 && btINDEX >= 21)
				{
					g_Log.MsgBox("%s load fail - error in file", lpszFile);
					fclose(SMDFile);
					return;
				}

				Token = GetToken();
				btID = TokenNumber;

				if(btID < 0 || btID > 4)
				{
					g_Log.MsgBox("%s load fail - error in file", lpszFile);
					fclose(SMDFile);
					return;
				}

				Token = GetToken();
				this->m_Quests[btINDEX][btSTATE].wREQUEST[btID].RequirementType = TokenNumber;

				Token = GetToken();
				this->m_Quests[btINDEX][btSTATE].wREQUEST[btID].DeleteItem = TokenNumber;

				Token = GetToken();
				this->m_Quests[btINDEX][btSTATE].wREQUEST[btID].RequirementID = TokenNumber;

				Token = GetToken();
				this->m_Quests[btINDEX][btSTATE].wREQUEST[btID].RequiredCount = TokenNumber;
			}

			else if ( iType == 3 )
			{
				if ( !strcmp("end", TokenString) )
					break;

				Token = GetToken();
				btINDEX = TokenNumber;

				Token = GetToken();
				btSTATE = TokenNumber;

				if(btINDEX < 0 && btINDEX >= 21)
				{
					g_Log.MsgBox("%s load fail - error in file", lpszFile);
					fclose(SMDFile);
					return;
				}

				Token = GetToken();
				btID = TokenNumber;

				if(btID < 0 || btID > 4)
				{
					g_Log.MsgBox("%s load fail - error in file", lpszFile);
					fclose(SMDFile);
					return;
				}

				Token = GetToken();
				this->m_Quests[btINDEX][btSTATE].wREWARD[btID].RewardType = TokenNumber;

				Token = GetToken();
				this->m_Quests[btINDEX][btSTATE].wREWARD[btID].RewardID = TokenNumber;

				Token = GetToken();
				this->m_Quests[btINDEX][btSTATE].wREWARD[btID].RewardCount = TokenNumber;

				Token = GetToken();
				this->m_Quests[btINDEX][btSTATE].wREWARD[btID].RewardLevel = TokenNumber;
				
				Token = GetToken();
				this->m_Quests[btINDEX][btSTATE].wREWARD[btID].RewardSkill = TokenNumber;
				
				Token = GetToken();
				this->m_Quests[btINDEX][btSTATE].wREWARD[btID].RewardLuck = TokenNumber;
				
				Token = GetToken();
				this->m_Quests[btINDEX][btSTATE].wREWARD[btID].RewardOption = TokenNumber;
				
				Token = GetToken();
				this->m_Quests[btINDEX][btSTATE].wREWARD[btID].RewardExc = TokenNumber;

				Token = GetToken();
				this->m_Quests[btINDEX][btSTATE].wREWARD[btID].RewardSet = TokenNumber;
			}

			else if ( iType == 4 )
			{
				if ( !strcmp("end", TokenString) )
					break;

				this->m_DailyQuest[count].btINDEX = TokenNumber;

				Token = GetToken();
				this->m_DailyQuest[count].btSTATE = TokenNumber;

				Token = GetToken();
				this->m_DailyQuest[count].btDAY = TokenNumber;

				Token = GetToken();
				this->m_DailyQuest[count].btREQUESTLEVEL = TokenNumber;

				count++;
			}

			else if ( iType == 5 )
			{
				if ( !strcmp("end", TokenString) )
					break;

				this->m_QuestItems[count2].btINDEX = TokenNumber;
				
				Token = GetToken();
				this->m_QuestItems[count2].btSTATE = TokenNumber;

				Token = GetToken();
				this->m_QuestItems[count2].wMonsterID = TokenNumber;

				Token = GetToken();
				this->m_QuestItems[count2].wItemID = TokenNumber;

				Token = GetToken();
				this->m_QuestItems[count2].dwRate = TokenNumber;

				Token = GetToken();
				this->m_QuestItems[count2].btItemCount = TokenNumber;

				count2++;
			}
		}
	}

	fclose(SMDFile);

	for(int i=0;i<30;i++)
	{
		if(this->m_QuestItems[i].btINDEX != 0)
		{
			LPITEM_ATTRIBUTE p = &ItemAttribute[this->m_QuestItems[i].wItemID];

			p->QuestItem = TRUE;

			g_Log.Add(0, __FILE__, __FUNCTION__, "[Quest] [SetQuestItem] %s", p->Name);
		}
	}
}

struct PBMSG_COUNT_CHAR{
	PBMSG_HEAD2 h;
	WORD npctype;
	WORD count;
};

void CQuests::CGReqQuestList(int aIndex)
{
	if(!ObjectMaxRange(aIndex))
	{
		g_Log.Add(0, __FILE__, __FUNCTION__, "error-L3 %s %s %d", __FILE__, __LINE__, aIndex);
		return;
	}

	BYTE cBUFFER[255];

	PBMSG_COUNT_CHAR * lpMsg = (PBMSG_COUNT_CHAR *)(cBUFFER);
	PMSG_QUESTLISTSENDCHAR * lpQuest = (PMSG_QUESTLISTSENDCHAR *)(cBUFFER + sizeof(PBMSG_COUNT_CHAR));

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Type != OBJ_USER)
	{
		return;
	}

	if(lpObj->m_PlayerData->ISBOT == true)
	{
		return;
	}

	lpMsg->count = 0;

	
	for(int i=0;i<50;i++)
	{
		if(lpObj->m_PlayerData->m_Quests[i].QuestIndex != 0xFFFF && lpObj->m_PlayerData->m_Quests[i].QuestIndex > 0 && lpObj->m_PlayerData->m_Quests[i].QuestIndex < 21)
		{
			if(this->CheckNPC(lpObj, this->m_Quests[lpObj->m_PlayerData->m_Quests[i].QuestIndex][lpObj->m_PlayerData->m_Quests[i].QuestState].wNPCID))
			{
				lpQuest = (PMSG_QUESTLISTSENDCHAR *)(cBUFFER + sizeof(PBMSG_COUNT_CHAR) + (lpMsg->count * sizeof(PMSG_QUESTLISTSENDCHAR)));

				lpQuest->typeL = LOBYTE(lpObj->m_PlayerData->m_Quests[i].QuestIndex);
				lpQuest->typeH = HIBYTE(lpObj->m_PlayerData->m_Quests[i].QuestIndex);
				lpQuest->indexL = LOBYTE(lpObj->m_PlayerData->m_Quests[i].QuestState);
				lpQuest->indexH = HIBYTE(lpObj->m_PlayerData->m_Quests[i].QuestState);

				lpMsg->count++;
			}
		}
	}

	bool bCheckStage = false;
	
	for(int i=0;i<MAX_QUEST_INDEX;i++)
	{
		for(int k=0;k<255;k++)
		{
			if(this->m_Quests[i][k].btDIALOG == 1 || this->m_Quests[i][k].btDIALOG == 4)
			{
				if(this->CheckLevel(lpObj, this->m_Quests[i][k].wMINLEVEL, this->m_Quests[i][k].wMAXLEVEL))
				{
					bCheckStage = this->CheckStage(lpObj, i, k);

					if(bCheckStage == true)
					{
						if(this->CheckNPC(lpObj, this->m_Quests[i][k].wNPCID))
						{
							if(this->FindQuestArrayNum(lpObj, i) == -1)
							{
								lpQuest = (PMSG_QUESTLISTSENDCHAR *)(cBUFFER + sizeof(PBMSG_COUNT_CHAR) + (lpMsg->count * sizeof(PMSG_QUESTLISTSENDCHAR)));

								lpQuest->typeL = LOBYTE(this->m_Quests[i][k].btINDEX);
								lpQuest->typeH = HIBYTE(this->m_Quests[i][k].btINDEX);
								lpQuest->indexL = LOBYTE(this->m_Quests[i][k].btSTATE);
								lpQuest->indexH = HIBYTE(this->m_Quests[i][k].btSTATE);

								lpMsg->count++;
							}
						}
					}
				}
			}
		}
	}

	lpMsg->h.c = 0xC1;
	lpMsg->h.headcode = 0xF6;
	lpMsg->h.subcode = 0x0A;
	lpMsg->npctype = lpObj->TargetNumber;
	lpMsg->h.size = sizeof(PBMSG_COUNT_CHAR) + (lpMsg->count * sizeof(PMSG_QUESTLISTSENDCHAR));

	IOCP.DataSend(aIndex, cBUFFER, lpMsg->h.size);

	g_Log.Add(0, __FILE__, __FUNCTION__, "[Quest][NPC] (%s)(%s) Sent Quest List (%d)", lpObj->AccountID, lpObj->Name, lpMsg->count);
}

void CQuests::CGReqAnswer(int aIndex, PMSG_REQ_QUESTINFO *aRecv)
{
	if(!ObjectMaxRange(aIndex))
	{
		g_Log.Add(0, __FILE__, __FUNCTION__, "error-L3 %d %s %s", aIndex, __FILE__, __LINE__);
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	QUEST_INTERNAL_DATA * lpQuest = this->GetQuestData(aRecv->QuestState, aRecv->QuestIndex);

	if(lpQuest == NULL)
	{
		g_Log.AddC(0, __FILE__, __FUNCTION__, TColor::Red,  "[Quest] ERROR: Unknown quest (%s)(%s) (%d)(%d)", lpObj->AccountID, lpObj->Name,
			aRecv->QuestIndex, aRecv->QuestState);
		return;
	}

	if(lpQuest->btDIALOG == 2 || lpQuest->btDIALOG == 3)
	{
		this->GCAnsQuestSpecificInfo(lpObj, lpQuest);
		// Request and Reward Dialog
		return;
	}

	if(lpQuest->btDIALOG == 4)
	{
		int ArrayNum = this->FindQuestArrayNum(lpObj, lpQuest->btINDEX, lpQuest->btSTATE);

		if(ArrayNum == -1)
		{
			bool bResult = this->AddQuest(lpObj, false, lpQuest->btINDEX, lpQuest->btSTATE);
			
			if(bResult == false)
			{
				g_Log.Add(0, __FILE__, __FUNCTION__, "[Quest] ERROR : AddQuest() == false (%s)(%s)", lpObj->AccountID, lpObj->Name);
				return;
			}
		}

		this->GCAnsQuestSpecificInfo(lpObj, lpQuest);
		// Request and Reward Dialog
		return;
	}
	else
	{
		if(lpQuest->btDIALOG == 1)
		{
			int ArrayNum = this->FindQuestArrayNum(lpObj, lpQuest->btINDEX, lpQuest->btSTATE);

			if(ArrayNum == -1)
			{
				bool bResult = this->AddQuest(lpObj, false, lpQuest->btINDEX, lpQuest->btSTATE);

				if(bResult == false)
				{
					g_Log.Add(0, __FILE__, __FUNCTION__, "[Quest] ERROR : AddQuest() == false (%s)(%s)", lpObj->AccountID, lpObj->Name);
					return;
				}
			}
		}
		else if(lpQuest->btDIALOG == 0)
		{
			bool bResult = this->AddQuest(lpObj, true, lpQuest->btINDEX, lpQuest->btSTATE);
		}

		PMSG_REQ_QUESTINFO pMsg;
		PHeadSubSetB((LPBYTE)&pMsg, 0xF6, 0x0B, sizeof(pMsg));

		pMsg.QuestState = lpQuest->btSTATE;
		pMsg.QuestIndex = lpQuest->btINDEX;

		IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

		g_Log.Add(0, __FILE__, __FUNCTION__, "[Quest] (%s)(%s) Started Dialog with NPC (%d/%d)", lpObj->AccountID, lpObj->Name, lpQuest->btINDEX, lpQuest->btSTATE);
	}
}

QUEST_INTERNAL_DATA * CQuests::GetQuestData(WORD State, WORD Index)
{
	if(Index < 0 || Index > 20)
		return NULL;

	if(State < 0 || State > 254)
		return NULL;

	return &this->m_Quests[Index][State];
}

bool CQuests::AddQuest(LPOBJ lpObj, bool Overwrite, WORD Index, WORD State)
{
	int arraynum = -1;

	if(Overwrite)
	{
		for(int i=0;i<50;i++)
		{
			if(lpObj->m_PlayerData->m_Quests[i].QuestIndex == Index)
			{
				arraynum = i;
				break;
			}
		}

		if(arraynum == -1)
		{
			return false;
		}

		lpObj->m_PlayerData->m_Quests[arraynum].QuestIndex = Index;
		lpObj->m_PlayerData->m_Quests[arraynum].QuestState = State;
		memset(lpObj->m_PlayerData->m_Quests[arraynum].ReqKillCount, 0x00, 5);
		
		return true;
	}
	
	else
	{
		for(int i=0;i<50;i++)
		{
			if(lpObj->m_PlayerData->m_Quests[i].QuestIndex == 0xFFFF)
			{
				lpObj->m_PlayerData->m_Quests[i].QuestIndex = Index;
				lpObj->m_PlayerData->m_Quests[i].QuestState = State;
				return true;
			}
		}
	}
	return false;
}

void CQuests::CGReqDialog(int aIndex, PMSG_REQ_QUESTANSWER *aRecv)
{
	if(!ObjectMaxRange(aIndex))
	{
		g_Log.Add(0, __FILE__, __FUNCTION__, "error-L3 %d %s %s", aIndex, __FILE__, __LINE__);
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Type != OBJ_USER)
	{
		g_Log.Add(0, __FILE__, __FUNCTION__, "error-L3 %d %s %s", aIndex, __FILE__, __LINE__);
		return;
	}

	QUEST_INTERNAL_DATA * lpQuest = this->GetQuestData(aRecv->QuestState, aRecv->QuestIndex);

	if(lpQuest == NULL)
	{
		g_Log.AddC(0, __FILE__, __FUNCTION__, TColor::Red,  "[Quest] ERROR: Unknown quest (%s)(%s) (%d)(%d)", lpObj->AccountID, lpObj->Name,
			aRecv->QuestIndex, aRecv->QuestState);
		return;
	}

	BYTE AnsType;
	bool bReqRew = false;

	if(aRecv->Answer == 1)
	{
		AnsType = lpQuest->btANSFIRST;

		if(AnsType == 0xFE)
		{
			AnsType = lpQuest->btDIALOGCLASS[lpObj->Class];
		}
	}
	else if(aRecv->Answer == 2)
	{
		AnsType = lpQuest->btANSSECOND;
	}
	else if(aRecv->Answer == 3)
	{
		AnsType = lpQuest->btANSTHIRD;
	}

	QUEST_INTERNAL_DATA * lpNewQuest = this->GetQuestData(AnsType, aRecv->QuestIndex);

	if ( lpNewQuest == NULL )
	{
		g_Log.AddC(0, __FILE__, __FUNCTION__, TColor::Red,  "[Quest] ERROR: Unknown quest continue data (%s)(%s) (%d)(%d)", lpObj->AccountID, lpObj->Name,
			aRecv->QuestIndex, aRecv->QuestState);
		return;
	}

	if(lpNewQuest->btDIALOG == 2 || lpNewQuest->btDIALOG == 3 || lpNewQuest->btDIALOG == 4)
	{
		bReqRew = true;
	}

	bool bRes = this->AddQuest(lpObj, true, aRecv->QuestIndex, AnsType);

	if(bRes == false)
	{
		g_Log.AddC(0, __FILE__, __FUNCTION__, TColor::Red,  "[Quest] ERROR: Quest not exist in PlayerData (%s)(%s)", lpObj->AccountID, lpObj->Name);
		return;
	}

	if(bReqRew == false)
	{
		PMSG_REQ_QUESTINFO pMsg;
		PHeadSubSetB((LPBYTE)&pMsg, 0xF6, 0x0B, sizeof(pMsg));

		pMsg.QuestState = lpNewQuest->btSTATE;
		pMsg.QuestIndex = lpNewQuest->btINDEX;

		IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		g_Log.Add(0, __FILE__, __FUNCTION__, "[Quest] (%s)(%s) Continue Dialog with NPC (%d/%d)", lpObj->AccountID, lpObj->Name, lpQuest->btINDEX, lpQuest->btSTATE);
	}
	else
	{
		this->GCAnsQuestSpecificInfo(lpObj, lpNewQuest);
		//Send Req/Rew
	}

	if(lpQuest->btSTAGE != 0xFF && lpQuest->btDIALOG != 2)
	{
		if(lpQuest->btSTAGE == 0xFE)
		{
			lpObj->m_PlayerData->m_Quest[24+lpQuest->btINDEX] = lpQuest->btDIALOGCLASS[lpObj->Class];
		}
		else
		{
			lpObj->m_PlayerData->m_Quest[24+lpQuest->btINDEX] = lpQuest->btSTAGE;
		}
	}
}

void CQuests::GCAnsQuestSpecificInfo(LPOBJ lpObj, QUEST_INTERNAL_DATA * lpQuest)
{
	PMSG_QUEST_DETAIL pMsg;
	memset(&pMsg, 0x00, sizeof(pMsg));
	PHeadSubSetB((LPBYTE)&pMsg, 0xF6, 0x0C, sizeof(pMsg));

	int QuestNum = this->FindQuestArrayNum(lpObj, lpQuest->btINDEX, lpQuest->btSTATE);

	if(QuestNum == -1)
	{
		return;
	}

	pMsg.QuestGroup = lpQuest->btINDEX;
	pMsg.QuestID = lpQuest->btSTATE;
	pMsg.RandRewardCount = lpQuest->btRANDOMREWARD;

	for(int i=0;i<5;i++)
	{
		if(lpQuest->wREQUEST[i].RequirementType != 0)
		{
			pMsg.Needs[i].RequirementType = lpQuest->wREQUEST[i].RequirementType;
			pMsg.Needs[i].RequirementID = lpQuest->wREQUEST[i].RequirementID;
			pMsg.Needs[i].RequiredCount = lpQuest->wREQUEST[i].RequiredCount;
			pMsg.Needs[i].Count = this->GetRequiredCount(lpObj, i, lpQuest, QuestNum);

			if(pMsg.Needs[i].RequirementType == 4) // Item
			{
				CItem Item;
				Item.m_Level = 0;
				Item.m_Type = lpQuest->wREQUEST[i].RequirementID;
				Item.m_Durability = ItemGetDurability(Item.m_Type, Item.m_Level, 0, 0);
				Item.Convert(lpQuest->wREQUEST[i].RequirementID, 0, 0,
					0, 0, 0, 0, -1, Item.m_SocketOption, 3, g_AttributeSystem.GetElement(Item.m_Type), 0);
				ItemByteConvert(pMsg.Needs[i].RequirementItemInfo, Item);
			}
			
			pMsg.NeedsCount++;
		}
	}

	for(int i=0;i<5;i++)
	{
		if(lpQuest->wREWARD[i].RewardType != 0)
		{
			pMsg.Prizes[i].RewardType = lpQuest->wREWARD[i].RewardType;
			pMsg.Prizes[i].RewardID = lpQuest->wREWARD[i].RewardID;
			pMsg.Prizes[i].RewardCount = lpQuest->wREWARD[i].RewardCount;

			if(pMsg.Prizes[i].RewardType == 1)
			{
				pMsg.Prizes[i].RewardCount = this->CalcExperience(pMsg.Prizes[i].RewardCount, lpObj);
			}

			if(pMsg.Prizes[i].RewardType == 2)
			{
				pMsg.Prizes[i].RewardCount = this->CalcMoney(pMsg.Prizes[i].RewardCount);
			}

			if(pMsg.Prizes[i].RewardType == 4) // Item
			{
				CItem Item;
				Item.m_Level = lpQuest->wREWARD[i].RewardLevel;
				Item.m_Type = lpQuest->wREWARD[i].RewardID;
				Item.m_Durability = ItemGetDurability(Item.m_Type, Item.m_Level, lpQuest->wREWARD[i].RewardExc, lpQuest->wREWARD[i].RewardSet);
				Item.Convert(lpQuest->wREWARD[i].RewardID, lpQuest->wREWARD[i].RewardSkill, lpQuest->wREWARD[i].RewardLuck,
					lpQuest->wREWARD[i].RewardOption, lpQuest->wREWARD[i].RewardExc, lpQuest->wREWARD[i].RewardSet, 0, -1, Item.m_SocketOption, 3, g_AttributeSystem.GetElement(Item.m_Type), 0);
				ItemByteConvert(pMsg.Prizes[i].RewardItemInfo, Item);
			}
			
			pMsg.PrizeCount++;

			if(lpQuest->btRANDOMREWARD != 0)
			{
				pMsg.Prizes[i].RewardType |= 0x20;
			}
		}
	}

	IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
	g_Log.Add(0, __FILE__, __FUNCTION__, "[Quest] (%s)(%s) Sent Specific Info about Quest (%d/%d) (Req:%d/Rew:%d)", lpObj->AccountID, lpObj->Name, lpQuest->btINDEX, lpQuest->btSTATE, pMsg.NeedsCount, pMsg.PrizeCount);
}

int CQuests::FindQuestArrayNum(LPOBJ lpObj, WORD Index, WORD State)
{
	for(int i=0;i<50;i++)
	{
		if(lpObj->m_PlayerData->m_Quests[i].QuestIndex == Index && lpObj->m_PlayerData->m_Quests[i].QuestState == State)
		{
			return i;
		}
	}

	return -1;
}

int CQuests::FindQuestArrayNum(LPOBJ lpObj, WORD Index)
{
	for(int i=0;i<50;i++)
	{
		if(lpObj->m_PlayerData->m_Quests[i].QuestIndex == Index)
		{
			return i;
		}
	}

	return -1;
}

QUEST_INTERNAL_DATA * CQuests::FindDailyQuest(LPOBJ lpObj, BYTE btDay, BYTE btReqLevel)
{
	for(int i=0;i<30;i++)
	{
		if(this->m_DailyQuest[i].btDAY == btDay && this->m_DailyQuest[i].btREQUESTLEVEL == btReqLevel)
		{
			QUEST_INTERNAL_DATA * lpQuest = this->GetQuestData(this->m_DailyQuest[i].btSTATE, this->m_DailyQuest[i].btINDEX);

			if(lpQuest != NULL)
			{
				return lpQuest;
			}
		}
	}

	return NULL;
}

void CQuests::ConvertByteToData(LPOBJ lpObj, unsigned char* buf)
{
	BYTE indexh;
	BYTE indexl;
	BYTE stateh;
	BYTE statel;
	BYTE kills[5];

	for(int i=0;i<50;i++)
	{
		indexh = buf[i*9];
		indexl = buf[i*9+1];
		stateh = buf[i*9+2];
		statel = buf[i*9+3];
		kills[0] = buf[i*9+4];
		kills[1] = buf[i*9+5];
		kills[2] = buf[i*9+6];
		kills[3] = buf[i*9+7];
		kills[4] = buf[i*9+8];

		lpObj->m_PlayerData->m_Quests[i].Reset();

		lpObj->m_PlayerData->m_Quests[i].QuestIndex = MAKE_NUMBERW(indexh, indexl);
		lpObj->m_PlayerData->m_Quests[i].QuestState = MAKE_NUMBERW(stateh, statel);
		memcpy(lpObj->m_PlayerData->m_Quests[i].ReqKillCount, kills, 5);
	}

	this->SetEventQuest(lpObj);
}

void CQuests::ConvertDataToByte(LPOBJ lpObj, unsigned char* buf)
{
	int n = 0;

	for(int i=0;i<50;i++)
	{
		buf[n] = HIBYTE(lpObj->m_PlayerData->m_Quests[i].QuestIndex);
		n++;

		buf[n] = LOBYTE(lpObj->m_PlayerData->m_Quests[i].QuestIndex);
		n++;

		buf[n] = HIBYTE(lpObj->m_PlayerData->m_Quests[i].QuestState);
		n++;

		buf[n] = LOBYTE(lpObj->m_PlayerData->m_Quests[i].QuestState);
		n++;

		buf[n] = lpObj->m_PlayerData->m_Quests[i].ReqKillCount[0];
		n++;

		buf[n] = lpObj->m_PlayerData->m_Quests[i].ReqKillCount[1];
		n++;

		buf[n] = lpObj->m_PlayerData->m_Quests[i].ReqKillCount[2];
		n++;

		buf[n] = lpObj->m_PlayerData->m_Quests[i].ReqKillCount[3];
		n++;

		buf[n] = lpObj->m_PlayerData->m_Quests[i].ReqKillCount[4];
		n++;
	}
}

struct PBMSG_QUESTLIST_CHAR{
	PBMSG_HEAD2 h;
	BYTE count;
};

void CQuests::CGReqQuestListLogin(int aIndex)
{
	if(!ObjectMaxRange(aIndex))
	{
		g_Log.Add(0, __FILE__, __FUNCTION__, "error-L3 %d %s %s", aIndex, __FILE__, __LINE__);
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Type != OBJ_USER)
	{
		return;
	}
	
	if(lpObj->m_PlayerData->ISBOT == true)
	{
		return;
	}

	BYTE cBUFFER[255];

	PBMSG_QUESTLIST_CHAR * lpMsg = (PBMSG_QUESTLIST_CHAR *)(cBUFFER);
	PMSG_QUESTLISTSENDCHAR * lpQuest = (PMSG_QUESTLISTSENDCHAR *)(cBUFFER + sizeof(PBMSG_QUESTLIST_CHAR));

	lpMsg->count = 0;

	for(int i=0;i<50;i++)
	{
		if(lpObj->m_PlayerData->m_Quests[i].QuestIndex != 0xFFFF)
		{
			lpQuest = (PMSG_QUESTLISTSENDCHAR *)(cBUFFER + sizeof(PBMSG_QUESTLIST_CHAR) + (lpMsg->count * sizeof(PMSG_QUESTLISTSENDCHAR)));

			lpQuest->typeL = LOBYTE(lpObj->m_PlayerData->m_Quests[i].QuestIndex);
			lpQuest->typeH = HIBYTE(lpObj->m_PlayerData->m_Quests[i].QuestIndex);
			lpQuest->indexL = LOBYTE(lpObj->m_PlayerData->m_Quests[i].QuestState);
			lpQuest->indexH = HIBYTE(lpObj->m_PlayerData->m_Quests[i].QuestState);

			lpMsg->count++;
		}
	}

	lpMsg->h.c = 0xC1;
	lpMsg->h.headcode = 0xF6;
	lpMsg->h.subcode = 0x1A;
	lpMsg->h.size = sizeof(PBMSG_QUESTLIST_CHAR) + (lpMsg->count * sizeof(PMSG_QUESTLISTSENDCHAR));

	IOCP.DataSend(lpObj->m_Index, cBUFFER, lpMsg->h.size);

	g_Log.Add(0, __FILE__, __FUNCTION__, "[Quest][Login] (%s)(%s) Sent Quest List (%d)", lpObj->AccountID, lpObj->Name, lpMsg->count);

	this->StartTutorial(aIndex);
}

void CQuests::GCInitQuest(int aIndex)
{
	PMSG_QUEST_INIT pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xF6, 0x20, sizeof(pMsg));

	IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void CQuests::CGReqQuestSpecificInfo(int aIndex, PMSG_REQ_QUEST_DETAIL *aRecv)
{
	if(!ObjectMaxRange(aIndex))
	{
		g_Log.Add(0, __FILE__, __FUNCTION__, "error-L3 %d %s %s", aIndex, __FILE__, __LINE__);
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Type != OBJ_USER)
	{
		return;
	}
	
	if(lpObj->m_PlayerData->ISBOT == true)
	{
		return;
	}

	QUEST_INTERNAL_DATA * lpQuest = this->GetQuestData(aRecv->QuestState, aRecv->QuestIndex);

	if(lpQuest == NULL)
	{
		return;
	}

	PMSG_QUEST_DETAIL pMsg;
	memset(&pMsg, 0x00, sizeof(pMsg));
	PHeadSubSetB((LPBYTE)&pMsg, 0xF6, 0x1B, sizeof(pMsg));

	int QuestNum = this->FindQuestArrayNum(lpObj, lpQuest->btINDEX, lpQuest->btSTATE);

	if(QuestNum == -1)
	{
		return;
	}

	pMsg.QuestGroup = lpQuest->btINDEX;
	pMsg.QuestID = lpQuest->btSTATE;
	pMsg.RandRewardCount = lpQuest->btRANDOMREWARD;

	for(int i=0;i<5;i++)
	{
		if(lpQuest->wREQUEST[i].RequirementType != 0)
		{
			pMsg.Needs[i].RequirementType = lpQuest->wREQUEST[i].RequirementType;
			pMsg.Needs[i].RequirementID = lpQuest->wREQUEST[i].RequirementID;
			pMsg.Needs[i].RequiredCount = lpQuest->wREQUEST[i].RequiredCount;
			pMsg.Needs[i].Count = this->GetRequiredCount(lpObj, i, lpQuest, QuestNum);

			if(pMsg.Needs[i].RequirementType == 4) // Item
			{
				CItem Item;
				Item.m_Level = 0;
				Item.m_Type = lpQuest->wREQUEST[i].RequirementID;
				Item.m_Durability = ItemGetDurability(Item.m_Type, Item.m_Level, 0, 0);
				Item.Convert(lpQuest->wREQUEST[i].RequirementID, 0, 0,
					0, 0, 0, 0, -1, Item.m_SocketOption, 3, g_AttributeSystem.GetElement(Item.m_Type), 0);
				ItemByteConvert(pMsg.Needs[i].RequirementItemInfo, Item);
			}
			
			pMsg.NeedsCount++;
		}
	}

	for(int i=0;i<5;i++)
	{
	 	if(lpQuest->wREWARD[i].RewardType != 0)
		{
			pMsg.Prizes[i].RewardType = lpQuest->wREWARD[i].RewardType;
			pMsg.Prizes[i].RewardID = lpQuest->wREWARD[i].RewardID;
			pMsg.Prizes[i].RewardCount = lpQuest->wREWARD[i].RewardCount;

			if(pMsg.Prizes[i].RewardType == 1)
			{
				pMsg.Prizes[i].RewardCount = this->CalcExperience(pMsg.Prizes[i].RewardCount, lpObj);
			}

			if(pMsg.Prizes[i].RewardType == 2)
			{
				pMsg.Prizes[i].RewardCount = this->CalcMoney(pMsg.Prizes[i].RewardCount);
			}

			if(pMsg.Prizes[i].RewardType == 4) // Item
			{
				CItem Item;
				Item.m_Level = lpQuest->wREWARD[i].RewardLevel;
				Item.m_Type = lpQuest->wREWARD[i].RewardID;
				Item.m_Durability = ItemGetDurability(Item.m_Type, Item.m_Level, lpQuest->wREWARD[i].RewardExc, lpQuest->wREWARD[i].RewardSet);
				Item.Convert(lpQuest->wREWARD[i].RewardID, lpQuest->wREWARD[i].RewardSkill, lpQuest->wREWARD[i].RewardLuck,
					lpQuest->wREWARD[i].RewardOption, lpQuest->wREWARD[i].RewardExc, lpQuest->wREWARD[i].RewardSet, 0, -1, Item.m_SocketOption, 3, g_AttributeSystem.GetElement(Item.m_Type), 0);
				ItemByteConvert(pMsg.Prizes[i].RewardItemInfo, Item);
			}

			if(lpQuest->btRANDOMREWARD != 0)
			{
				pMsg.Prizes[i].RewardType |= 0x20;
			}
			
			pMsg.PrizeCount++;
		}
	}

	IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
}

void CQuests::KillMonsterProc(LPOBJ lpMonsterObj)
{
	if(lpMonsterObj == NULL)
	{
		return;
	}

	int MaxHitUser = gObjMonsterTopHitDamageUser(lpMonsterObj);

	if(MaxHitUser == -1)
	{
		return;
	}

	LPOBJ lpObj = &gObj[MaxHitUser];

	if(lpObj->Type != OBJ_USER)
	{
		return;
	}

	if(lpObj->m_PlayerData->ISBOT == true)
	{
		return;
	}

	QUEST_INTERNAL_DATA * lpQuest;

	for(int i=0;i<50;i++)
	{
		if(lpObj->PartyNumber == -1)
		{
			if(lpObj->m_PlayerData->m_Quests[i].QuestIndex != 0xFFFF)
			{
				lpQuest = this->GetQuestData(lpObj->m_PlayerData->m_Quests[i].QuestState, lpObj->m_PlayerData->m_Quests[i].QuestIndex);

				if(lpQuest != NULL)
				{
					for(int k=0;k<5;k++)
					{
						if(lpQuest->wREQUEST[k].RequirementType == 1)
						{
							if(lpQuest->wREQUEST[k].RequirementID == lpMonsterObj->Class)
							{
								if(lpQuest->wREQUEST[k].RequiredCount > lpObj->m_PlayerData->m_Quests[i].ReqKillCount[k])
								{
									lpObj->m_PlayerData->m_Quests[i].ReqKillCount[k]++;
									g_Log.Add(0, __FILE__, __FUNCTION__, "[Quest][Single] (%s)(%s) Added KillCount: CLASS:%d (%d/%d)", lpObj->AccountID, lpObj->Name, lpMonsterObj->Class,
										lpQuest->btINDEX, lpQuest->btSTATE);
								}
							}
						}
					}
				}
			}
		}
		else
		{
			int pindex = -1;
			int dbnumber = -1;

			for(int m=0;m<5;m++)
			{
				if(gParty.GetIndexUser(lpObj->PartyNumber, m, (int &)pindex, (int &)dbnumber) == 0)
				{
					break;
				}

				if(pindex != -1 && gObjIsConnected(pindex))
				{
					LPOBJ lpPartyObj = &gObj[pindex];

					if(lpPartyObj->Type != OBJ_USER)
					{
						continue;
					}

					if(lpPartyObj->MapNumber != lpObj->MapNumber)
					{
						continue;
					}

					if(gObjCalDistance(lpObj, lpPartyObj) > 10)
					{
						continue;
					}

					if(lpPartyObj->Live == FALSE)
					{
						continue;
					}

					if(lpPartyObj->m_PlayerData->m_Quests[i].QuestIndex != 0xFFFF)
					{
						lpQuest = this->GetQuestData(lpPartyObj->m_PlayerData->m_Quests[i].QuestState, lpPartyObj->m_PlayerData->m_Quests[i].QuestIndex);

						if(lpQuest != NULL)
						{
							for(int k=0;k<5;k++)
							{
								if(lpQuest->wREQUEST[k].RequirementType == 1)
								{
									if(lpQuest->wREQUEST[k].RequirementID == lpMonsterObj->Class)
									{
										if(lpQuest->wREQUEST[k].RequiredCount > lpPartyObj->m_PlayerData->m_Quests[i].ReqKillCount[k])
										{
											lpPartyObj->m_PlayerData->m_Quests[i].ReqKillCount[k]++;
											g_Log.Add(0, __FILE__, __FUNCTION__, "[Quest][Party] (%s)(%s) Added KillCount: CLASS:%d (%d/%d)", lpPartyObj->AccountID, lpPartyObj->Name, lpMonsterObj->Class,
												lpQuest->btINDEX, lpQuest->btSTATE);
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void CQuests::CGReqQuestFinish(int aIndex, PMSG_REQ_QUESTEND * aRecv)
{
	if(!ObjectMaxRange(aIndex))
	{
		g_Log.Add(0, __FILE__, __FUNCTION__, "error-L3 %d %s %s", aIndex, __FILE__, __LINE__);
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Type != OBJ_USER)
	{
		g_Log.Add(0, __FILE__, __FUNCTION__, "error-L3 %d %s %s", aIndex, __FILE__, __LINE__);
		return;
	}
	
	if(lpObj->m_PlayerData->ISBOT == true)
	{
		return;
	}

	QUEST_INTERNAL_DATA * lpQuest = this->GetQuestData(aRecv->QuestState, aRecv->QuestIndex);

	if(lpQuest == NULL)
	{
		return;
	}

	PMSG_ANS_QUESTEND pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xF6, 0x0D, sizeof(pMsg));

	pMsg.QuestIndex = lpQuest->btINDEX;
	pMsg.QuestState = lpQuest->btSTATE;

	pMsg.btResult = this->CheckRequirements(lpObj, lpQuest);

	if(pMsg.btResult == TRUE)
	{		
		this->GiveReward(lpObj, lpQuest);
		this->DelQuest(lpObj, aRecv->QuestIndex, aRecv->QuestState);

		if(lpQuest->btINDEX == 1 && lpQuest->btSTATE == 2)
		{
			g_Log.Add(0, __FILE__, __FUNCTION__, "[Quest Tutorial] (%s)(%s) Started Tutorial (%d/%d)", lpObj->AccountID, lpObj->Name, lpQuest->btINDEX, lpQuest->btSTATE);
			lpObj->m_PlayerData->Tutorial = TUTORIAL_ONGOING;
		}
		else if(lpQuest->btINDEX == 1 && (lpQuest->btSTATE == 90 || lpQuest->btSTATE == 4) )
		{
			g_Log.Add(0, __FILE__, __FUNCTION__, "[Quest Tutorial] (%s)(%s) Finished Tutorial (%d/%d)", lpObj->AccountID, lpObj->Name, lpQuest->btINDEX, lpQuest->btSTATE);		
			lpObj->m_PlayerData->Tutorial = TUTORIAL_FINISHED;
		}

		if(lpQuest->btSTAGE == 0xFE)
		{
			lpObj->m_PlayerData->m_Quest[24+lpQuest->btINDEX] = lpQuest->btDIALOGCLASS[lpObj->Class];
		}
		else if(lpQuest->btINDEX >= 2 && lpQuest->btINDEX <= 5 || lpQuest->btINDEX == 8 || lpQuest->btINDEX == 9 || lpQuest->btINDEX == 10 || lpQuest->btINDEX == 11 || lpQuest->btINDEX == 12 || lpQuest->btINDEX == 20)
		{
			this->SetQuestStage(lpObj, lpQuest);
		}
		else
		{
			lpObj->m_PlayerData->m_Quest[24+lpQuest->btINDEX] = lpQuest->btSTAGE;
		}
	}

	g_Log.Add(0, __FILE__, __FUNCTION__, "[Quest] (%s)(%s) Finish Quest (%d/%d) Result: %s", lpObj->AccountID, lpObj->Name, lpQuest->btINDEX, lpQuest->btSTATE, pMsg.btResult == TRUE ? "OK" : "FAIL");

	IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

bool CQuests::CheckRequirements(LPOBJ lpObj, QUEST_INTERNAL_DATA * lpQuest)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return false;
	}

	bool checks[5] = { false, false, false, false, false };

	for(int i=0;i<50;i++)
	{
		if(lpObj->m_PlayerData->m_Quests[i].QuestIndex == lpQuest->btINDEX && lpObj->m_PlayerData->m_Quests[i].QuestState == lpQuest->btSTATE)
		{
			for(int k=0;k<5;k++)
			{
				if(lpQuest->wREQUEST[k].RequirementType == 1)
				{
					if(lpObj->m_PlayerData->m_Quests[i].ReqKillCount[k] >= lpQuest->wREQUEST[k].RequiredCount)
					{
						checks[k] = true;
					}
				}

				else if(lpQuest->wREQUEST[k].RequirementType == 2)
				{
					if(gObjGetMagicSearch(lpObj, lpQuest->wREQUEST[k].RequirementID) != NULL)
					{
						checks[k] = true;
					}
				}

				else if(lpQuest->wREQUEST[k].RequirementType == 4)
				{
					if(this->GetItemCount(lpObj, lpQuest->wREQUEST[k].RequirementID) >= lpQuest->wREQUEST[k].RequiredCount)
					{
						checks[k] = true;
					}
				}

				else if(lpQuest->wREQUEST[k].RequirementType == 8)
				{
					if(lpObj->Level > lpQuest->wREQUEST[k].RequiredCount)
					{
						checks[k] = true;
					}
				}

				else if(lpQuest->wREQUEST[k].RequirementType == 16)
				{
					if(lpObj->m_PlayerData->m_Quests[i].ReqKillCount[k] == 1)
					{
						checks[k] = true;
					}
				}

				else if(lpQuest->wREQUEST[k].RequirementType == 261)
				{
					if(lpObj->m_PlayerData->Money >= lpQuest->wREQUEST[k].RequiredCount)
					{
						checks[k] = true;
						lpObj->m_PlayerData->Money -= lpQuest->wREQUEST[k].RequiredCount;
						GSProtocol.GCMoneySend(lpObj->m_Index, lpObj->m_PlayerData->Money);
					}
				}

				else if(lpQuest->wREQUEST[k].RequirementType == 263)
				{
					if(lpObj->TargetNumber == lpQuest->wREQUEST[k].RequirementID)
					{
						checks[k] = true;
					}
					else if(lpQuest->wREQUEST[k].RequirementID == -2)
					{
						if(g_GensSystem.IsInfluenceNPC(lpObj))
						{
							checks[k] = true;
						}
					}
				}

				else
				{
					checks[k] = true;
				}
			}
		}
	}

	for(int l=0;l<5;l++)
	{
		if(checks[l] == false)
		{
			return false;
		}
	}

	return true;
}

bool CQuests::DelQuest(LPOBJ lpObj, WORD Index, WORD State)
{
	for(int i=0;i<50;i++)
	{
		if(lpObj->m_PlayerData->m_Quests[i].QuestIndex == Index && lpObj->m_PlayerData->m_Quests[i].QuestState == State)
		{
			lpObj->m_PlayerData->m_Quests[i].Reset();
			return true;
		}
	}

	return false;
}

int CQuests::LevelUp(LPOBJ lpObj, DWORD Experience)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return 0;
	}

	if ( MasterLevelSystemS6.CanGainMasterExp(lpObj))
	{
		MasterLevelSystemS6.LevelUp(lpObj, Experience, 0, "Quest");
		return 0;
	}

	int iLEFT_EXP = 0;

	g_Log.Add(0, __FILE__, __FUNCTION__, "[Quest] Experience : [%s][%s](%d) Experience: %d + %d",	
		lpObj->AccountID, lpObj->Name,
		lpObj->Level, lpObj->m_PlayerData->Experience,
		Experience);

	::gObjSetExpPetItem(lpObj->m_Index, Experience);

	if ( lpObj->Level >= MAX_CHAR_LEVEL )
	{
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,45), lpObj->m_Index, 1);
		return 0;
	}

	if ( (lpObj->m_PlayerData->Experience + Experience) < lpObj->m_PlayerData->NextExp )
	{
		lpObj->m_PlayerData->Experience += Experience;
		GSProtocol.GCKillPlayerMasterExpSend(lpObj->m_Index, -1, Experience, 0, 0);
	}
	else
	{
		iLEFT_EXP = lpObj->m_PlayerData->Experience + Experience - lpObj->m_PlayerData->NextExp;
		lpObj->m_PlayerData->Experience = lpObj->m_PlayerData->NextExp;
		lpObj->Level++;

		if ( lpObj->Class == CLASS_DARKLORD || lpObj->Class == CLASS_MAGUMSA || lpObj->Class == CLASS_RAGEFIGHTER )
		{
			lpObj->m_PlayerData->LevelUpPoint += gLevelUpPointMGDL;
		}
		else
		{
			lpObj->m_PlayerData->LevelUpPoint += gLevelUpPointNormal;
		}

		if ( lpObj->m_PlayerData->PlusStatQuestClear != false )
		{
			lpObj->m_PlayerData->LevelUpPoint++;

			g_Log.Add(0, __FILE__, __FUNCTION__, "[%s][%s] LevelUp PlusStatQuest Clear AddStat %d",
				lpObj->AccountID, lpObj->Name, lpObj->m_PlayerData->LevelUpPoint);
		}

		lpObj->MaxLife += DCInfo.DefClass[lpObj->Class].LevelLife;
		lpObj->MaxMana += DCInfo.DefClass[lpObj->Class].LevelMana;
		lpObj->Life = lpObj->MaxLife;
		lpObj->Mana = lpObj->MaxMana;
		gObjNextExpCal(lpObj);
		gObjSetBP(lpObj->m_Index);
		GSProtocol.GCLevelUpMsgSend(lpObj->m_Index, 1);
		gObjCalcMaxLifePower(lpObj->m_Index);
		g_Log.Add(0, __FILE__, __FUNCTION__, "Level Up [%s][%s][%d]", lpObj->AccountID, lpObj->Name, lpObj->Level);
		return 0;
	}

	return iLEFT_EXP;
}

void CQuests::CGReqQuestDelete(int aIndex, PMSG_QUESTDELETE *aRecv)
{
	if(!ObjectMaxRange(aIndex))
	{
		g_Log.Add(0, __FILE__, __FUNCTION__, "error-L3 %d %s %s", aIndex, __FILE__, __LINE__);
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Type != OBJ_USER)
	{
		return;
	}
	
	if(lpObj->m_PlayerData->ISBOT == true)
	{
		return;
	}

	if(lpObj->m_PlayerData->m_bSecurityCheck == false)
	{
		return;
	}

	QUEST_INTERNAL_DATA * lpQuest = this->GetQuestData(aRecv->QuestState, aRecv->QuestIndex);

	if(lpQuest == NULL)
	{
		return;
	}

	int ArrayNum = this->FindQuestArrayNum(lpObj, aRecv->QuestIndex, aRecv->QuestState);

	if(ArrayNum == -1)
	{
		return;
	}

	if(this->DelQuest(lpObj, aRecv->QuestIndex, aRecv->QuestState) == true)
	{
		PMSG_QUESTDELETE pMsg;
		PHeadSubSetB((LPBYTE)&pMsg, 0xF6, 0x0F, sizeof(pMsg));

		pMsg.QuestIndex = lpQuest->btINDEX;
		pMsg.QuestState = lpQuest->btRESETID;

		IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

		g_Log.Add(0, __FILE__, __FUNCTION__, "[Quest] (%s)(%s) Quest Delete - (%d/%d)", lpObj->AccountID, lpObj->Name, lpQuest->btINDEX, lpQuest->btSTATE);
	}
}

bool CQuests::CreateItem(LPOBJ lpObj, QUEST_INTERNAL_DATA * lpQuest, int RewardNum)
{
	if(lpQuest->wREWARD[RewardNum].RewardType != 4 )
	{
		return false;
	}

	for(int i=0;i<lpQuest->wREWARD[RewardNum].RewardCount;i++)
	{
		REWARD_DATA * lpReward = &lpQuest->wREWARD[RewardNum];
		ItemSerialCreateSend(lpObj->m_Index, 235, lpObj->X, lpObj->Y, lpReward->RewardID, lpReward->RewardLevel, 0, lpReward->RewardSkill, lpReward->RewardLuck, lpReward->RewardOption, lpObj->m_Index, lpReward->RewardExc, lpReward->RewardSet, g_AttributeSystem.GetElement(lpReward->RewardID), -1);
	}

	return true;
}

bool CQuests::CreateRandomItem(LPOBJ lpObj, QUEST_INTERNAL_DATA * lpQuest)
{
	bool bRewards[5] = { false, false, false, false, false };
	int count = 0;

	for(int i=0;i<50;i++)
	{
		if((lpQuest->wREWARD[i].RewardType & 0x20) == 0x20)
		{
			bRewards[i] = true;
			count++;
		}
	}

	int reward = rand()%5;

	if(bRewards[reward] == true)
	{
		this->CreateItem(lpObj, lpQuest, reward);
	}

	return true;
}

void CQuests::GCNotifyQuest(int aIndex, WORD Index, WORD State)
{
	PMSG_QUESTNOTIFY pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0xF6, 0x03, sizeof(pMsg));

	pMsg.QuestIndex = Index;
	pMsg.QuestIndex2 = Index;
	pMsg.QuestState = State;
	pMsg.QuestState2 = State;

	IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void CQuests::StartTutorial(int aIndex)
{
	if(!ObjectMaxRange(aIndex))
	{
		g_Log.Add(0, __FILE__, __FUNCTION__, "error-L3 %d %s %s", aIndex, __FILE__, __LINE__);
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Type != OBJ_USER)
	{
		return;
	}
	
	if(lpObj->m_PlayerData->ISBOT == true)
	{
		return;
	}

	if(lpObj->m_PlayerData->Tutorial == TUTORIAL_FINISHED || lpObj->m_PlayerData->Tutorial == TUTORIAL_ONGOING)
	{
		return;
	}

	int ArrayNum = this->FindQuestArrayNum(lpObj, 1);

	if(ArrayNum == -1)
	{
		this->GCNotifyQuest(aIndex, 1, 0);
		return;
	}

	else
	{
		if(lpObj->m_PlayerData->m_Quest[24+1] == 255 || lpObj->m_PlayerData->m_Quest[24+1] < 5)
		{
			this->GCNotifyQuest(aIndex, 1, lpObj->m_PlayerData->m_Quests[ArrayNum].QuestState);
		}
	}
}

void CQuests::CGReqQuestInterfaceUse(int aIndex, PMSG_REQ_QUESTINTERFACEUSE *aRecv)
{
	if(!ObjectMaxRange(aIndex))
	{
		g_Log.Add(0, __FILE__, __FUNCTION__, "error-L3 %d %s %s", aIndex, __FILE__, __LINE__);
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Type != OBJ_USER)
	{
		return;
	}
	
	if(lpObj->m_PlayerData->ISBOT == true)
	{
		return;
	}

	QUEST_INTERNAL_DATA * lpQuest = this->GetQuestData(aRecv->QuestState, aRecv->QuestIndex);

	if(lpQuest == NULL)
	{
		return;
	}

	int ArrayNum = this->FindQuestArrayNum(lpObj, aRecv->QuestIndex, aRecv->QuestState);

	if(ArrayNum == -1)
	{
		return;
	}

	for(int i=0;i<5;i++)
	{
		if(lpQuest->wREQUEST[i].RequirementType == 16) // Interface
		{
			lpObj->m_PlayerData->m_Quests[ArrayNum].ReqKillCount[i] = 1; // OK
		}
	}

	g_Log.Add(0, __FILE__, __FUNCTION__, "[Quest Tutorial] (%s)(%s) Interface used (ID:%d STATE:%d)",
		lpObj->AccountID, lpObj->Name, aRecv->QuestIndex, aRecv->QuestState);
}

int CQuests::GetItemCount(LPOBJ lpObj, int ItemNumber)
{
	int count = 0;

	for(int i=INVETORY_WEAR_SIZE;i<MAIN_INVENTORY_SIZE;i++)
	{
		if(lpObj->pInventory[i].m_Type == ItemNumber)
		{
			if(ItemNumber >= ITEMGET(14,0) && ItemNumber <= ITEMGET(14,8))
			{
				count += lpObj->pInventory[i].m_Durability;
			}
			else
			{
				count++;
			}
		}
	}

	return count;
}

int CQuests::GetRequiredCount(LPOBJ lpObj, int ReqID, QUEST_INTERNAL_DATA *lpQuest, int ArrayNum)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return 0;
	}

	switch ( lpQuest->wREQUEST[ReqID].RequirementType )
	{
		case 1:
		case 64:
		case 65:
		case 66:
		case 256:
		case 257:
		case 258:
		case 259:
		case 260:
		case 262:
			return lpObj->m_PlayerData->m_Quests[ArrayNum].ReqKillCount[ReqID];
		case 2:
			return (bool)gObjGetMagicSearch(lpObj, lpQuest->wREQUEST[ReqID].RequirementID);
		case 4:
			return this->GetItemCount(lpObj, lpQuest->wREQUEST[ReqID].RequirementID);
		case 8:
			return lpObj->Level;
		case 16:
			return (bool)lpObj->m_PlayerData->m_Quests[ArrayNum].ReqKillCount[ReqID];
		case 261:
			return lpObj->m_PlayerData->Money;
		case 263:
			if(lpQuest->wREQUEST[ReqID].RequirementID == lpObj->TargetNumber)
			{
				return true;
			}
			else if(lpQuest->wREQUEST[ReqID].RequirementID == -2)
			{
				if(g_GensSystem.IsInfluenceNPC(lpObj))
				{
					return true;
				}
			}
	}

	return 0;
}

bool CQuests::CheckGens(LPOBJ lpObj, WORD NpcID)
{
	return g_GensSystem.IsInfluenceNPC(lpObj);
}

UINT64 CQuests::CalcExperience(UINT64 i64Experience, LPOBJ lpObj)
{
	if(MasterLevelSystemS6.CanGainMasterExp(lpObj))
	{
		i64Experience *= gMLAddExperience;
	}
	else
	{
		i64Experience *= gAddExperience;
	}

	return i64Experience;
}

int CQuests::CalcMoney(int Money)
{
	return Money *= gAddZen;
}

void CQuests::CGReqShadowPhantomBuff(int aIndex)
{
	if(!ObjectMaxRange(aIndex))
	{
		g_Log.Add(0, __FILE__, __FUNCTION__, "error-L3 %d %s %s", aIndex, __FILE__, __LINE__);
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Type != OBJ_USER)
	{
		return;
	}
	
	if(lpObj->m_PlayerData->ISBOT == true)
	{
		return;
	}

	if (lpObj->Level > g_iShadowPhantomMaxLevel)
	{
		GSProtocol.GCServerCmd(lpObj->m_Index, 0x0D, 0, 0);
	}
	else
	{
		gObjAddBuffEffect(lpObj, BUFFTYPE_ATTDEF_POWER_INC, EFFECTTYPE_IMPROVE_DAMAGE, (lpObj->Level / 5 +50), EFFECTTYPE_IMPROVE_DEFENSE, (lpObj->Level / 3 +45), 1800);
		g_Log.Add(0, __FILE__, __FUNCTION__, "[Quest] (%s)(%s) Received Shadow Phantom Buff (Level:%d)", lpObj->AccountID, lpObj->Name, lpObj->Level);
	}
}

bool CQuests::IsDailyQuest(WORD Index, WORD State, BYTE & btLevel)
{
	for(int i=0;i<30;i++)
	{
		if(this->m_DailyQuest[i].btINDEX == Index &&
			this->m_DailyQuest[i].btSTATE == State)
		{
			btLevel = this->m_DailyQuest[i].btREQUESTLEVEL;
			return true;
		}
	}

	return false;
}

bool CQuests::GetDailyQuest(int aIndex, BYTE btLevel)
{
	if(!ObjectMaxRange(aIndex))
	{
		g_Log.Add(0, __FILE__, __FUNCTION__, "error-L3 %d %s %s", aIndex, __FILE__, __LINE__);
		return false;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Type != OBJ_USER)
	{
		return false;
	}
	
	if(lpObj->m_PlayerData->ISBOT == true)
	{
		return false;
	}

	SYSTEMTIME Time;
	GetLocalTime(&Time);

	if(lpObj->m_PlayerData->m_Quest[97+btLevel] == Time.wDayOfWeek)
	{
		return false;
	}

	else
	{
		lpObj->m_PlayerData->m_Quest[97+btLevel] = 0;
	}

	QUEST_INTERNAL_DATA * lpQuest = this->FindDailyQuest(lpObj, Time.wDayOfWeek, btLevel);

	if(lpQuest == NULL)
	{
		return false;
	}

	bool bResult = this->AddQuest(lpObj, false, lpQuest->btINDEX, lpQuest->btSTATE);
			
	if(bResult == false)
	{
		g_Log.Add(0, __FILE__, __FUNCTION__, "[Quest] ERROR : AddQuest() == false (%s)(%s)", lpObj->AccountID, lpObj->Name);
		return false;
	}

	lpObj->m_PlayerData->m_Quest[24+lpQuest->btINDEX] = lpQuest->btSTATE;

	PMSG_REQ_QUESTITEMUSE pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xF6, 0x04, sizeof(pMsg));
	
	pMsg.Unk = 0;
	pMsg.wResult = 1;
	pMsg.QuestState = lpQuest->btSTATE;
	pMsg.QuestIndex = lpQuest->btINDEX;

	IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

	return true;
}

void CQuests::GiveReward(LPOBJ lpObj, QUEST_INTERNAL_DATA * lpQuest)
{
	if(lpObj == NULL)
	{
		return;
	}

	if(lpObj->Type != OBJ_USER)
	{
		return;
	}
	
	if(lpObj->m_PlayerData->ISBOT == true)
	{
		return;
	}

	if(lpQuest == NULL)
	{
		return;
	}

	TRandomPoolMgr RandomReward;
	RandomReward.InitPool();
	BYTE btRandom[5] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

	if(lpQuest->btRANDOMREWARD != 0)
	{
		for(int i=0;i<5;i++)
		{
			if(lpQuest->wREWARD[i].RewardType != 0)
			{
				RandomReward.AddValue(i, 100);
			}
		}

		int cnt = 0;

		while ( cnt < lpQuest->btRANDOMREWARD )
		{
			btRandom[cnt] = RandomReward.GetRandomValue(eRANDOMPOOL_BY_RANDOM);
		
			cnt++;
		}

		for(int i=0;i<cnt;i++)
		{
			if(btRandom[i] != 0xFF)
			{
				switch ( (lpQuest->wREWARD[btRandom[i]].RewardType) )
				{
					case 1:
					{
						DWORD CalcExp = this->CalcExperience(lpQuest->wREWARD[btRandom[i]].RewardCount, lpObj);

						while ( CalcExp > 0 )
						{
							CalcExp = this->LevelUp(lpObj, CalcExp);
						}
				
						GSProtocol.GCKillPlayerMasterExpSend(lpObj->m_Index, -1, CalcExp, 0, 0);
					}
					break;		
					case 2:
						lpObj->m_PlayerData->Money += this->CalcMoney(lpQuest->wREWARD[btRandom[i]].RewardCount);
						GSProtocol.GCMoneySend(lpObj->m_Index, lpObj->m_PlayerData->Money);
						break;
					case 4:
						this->CreateItem(lpObj, lpQuest, btRandom[i]);
						break;
					case 16:
						if(g_GensSystem.IsRegGensInfluence(lpObj) == TRUE)
						{
							g_GensSystem.AddContributePoint(lpObj, lpQuest->wREWARD[btRandom[i]].RewardCount);
						}
						break;
				}
			}
		}
		
		return;
	}

	for(int i=0;i<5;i++)
	{

		switch ( lpQuest->wREWARD[i].RewardType )
		{
			case 1:
			{
				DWORD CalcExp = lpQuest->wREWARD[i].RewardCount;

				while ( CalcExp > 0 )
				{
					CalcExp = this->LevelUp(lpObj, CalcExp);
				}
				
				GSProtocol.GCKillPlayerMasterExpSend(lpObj->m_Index, -1, CalcExp, 0, 0);
			}
			break;
			case 2:
				lpObj->m_PlayerData->Money += lpQuest->wREWARD[i].RewardCount;
				GSProtocol.GCMoneySend(lpObj->m_Index, lpObj->m_PlayerData->Money);
				break;
			case 4:
				this->CreateItem(lpObj, lpQuest, i);
				break;
			case 16:
				if(g_GensSystem.IsRegGensInfluence(lpObj))
				{
					g_GensSystem.AddContributePoint(lpObj, lpQuest->wREWARD[i].RewardCount);
				}
				break;
		}
	}

	for(int i=0;i<5;i++)
	{
		if(lpQuest->wREQUEST[i].DeleteItem == true)
		{
			this->DeleteItem(lpObj, lpQuest, i);
		}
	}
}

void CQuests::DeleteItem(LPOBJ lpObj, QUEST_INTERNAL_DATA * lpQuest, int RewardNum)
{
	if(lpObj == NULL)
	{
		return;
	}

	if (lpObj->Type != OBJ_USER)
	{
		return;
	}

	if(lpQuest == NULL)
	{
		return;
	}

	if(RewardNum < 0 || RewardNum > 4)
	{
		return;
	}

	if(lpQuest->wREQUEST[RewardNum].DeleteItem == false)
	{
		return;
	}

	if(lpQuest->wREQUEST[RewardNum].RequirementType != 4 &&
		lpQuest->wREQUEST[RewardNum].RequirementType != 261)
	{
		return;
	}

	int count = lpQuest->wREQUEST[RewardNum].RequiredCount;

	if(lpQuest->wREQUEST[RewardNum].RequirementType == 4)
	{
		for(int i=INVETORY_WEAR_SIZE;i<MAIN_INVENTORY_SIZE;i++)
		{
			if(lpObj->pInventory[i].m_Type == lpQuest->wREQUEST[RewardNum].RequirementID)
			{
				count -= lpObj->pInventory[i].m_Durability;
				gObjInventoryDeleteItem(lpObj->m_Index, i);
				GSProtocol.GCInventoryItemDeleteSend(lpObj->m_Index, i, 1);
				if(count <= 0)
				{
					break;
				}
			}
		}
	}
	else if(lpQuest->wREQUEST[RewardNum].RequirementType == 261)
	{
		lpObj->m_PlayerData->Money -= count;
		GSProtocol.GCMoneySend(lpObj->m_Index, lpObj->m_PlayerData->Money);
	}
}

bool CQuests::CheckNPC(LPOBJ lpObj, WORD NPCID)
{
	if(NPCID == 0xFFFE)
	{
		if(g_GensSystem.IsInfluenceNPC(lpObj) == FALSE)
		{
			return false;
		}

		if(g_GensSystem.IsInfluenceNPC(lpObj) == TRUE)
		{
			return true;
		}

		else
		{
			return false;
		}
	}

	if(NPCID == 543)
	{
		if(g_GensSystem.IsInfluenceNPC(lpObj) == TRUE)
		{
			return true;
		}
	}

	if(NPCID == 544)
	{
		if(g_GensSystem.IsInfluenceNPC(lpObj) == TRUE)
		{
			return true;
		}
	}

	if(lpObj->TargetNumber == NPCID)
	{
		return true;
	}

	return false;
}

void CQuests::KillGensProc(LPOBJ lpObj, LPOBJ lpTargetObj)
{
	if(lpObj->Type != OBJ_USER)
	{
		return;
	}

	if(lpObj->m_PlayerData->ISBOT == true)
	{
		return;
	}

	QUEST_INTERNAL_DATA * lpQuest;

	for(int i=0;i<50;i++)
	{
		if(lpObj->PartyNumber == -1)
		{
			if(lpObj->m_PlayerData->m_Quests[i].QuestIndex != 0xFFFF)
			{
				lpQuest = this->GetQuestData(lpObj->m_PlayerData->m_Quests[i].QuestState, lpObj->m_PlayerData->m_Quests[i].QuestIndex);

				if(lpQuest != NULL)
				{
					for(int k=0;k<5;k++)
					{
						if(lpQuest->wREQUEST[k].RequirementType == 262)
						{
							if(g_GensSystem.GetGensInfluence(lpObj) != g_GensSystem.GetGensInfluence(lpTargetObj))
							{
								if(lpQuest->wREQUEST[k].RequiredCount > lpObj->m_PlayerData->m_Quests[i].ReqKillCount[k])
								{
									lpObj->m_PlayerData->m_Quests[i].ReqKillCount[k]++;
									g_Log.Add(0, __FILE__, __FUNCTION__, "[Quest][Single] (%s)(%s) Added KillCount: (GENS) (%d/%d)", lpObj->AccountID, lpObj->Name,
										lpQuest->btINDEX, lpQuest->btSTATE);
								}
							}
						}
					}
				}
			}
		}
		else
		{
			for(int m=0;m<5;m++)
			{
				int pindex = -1;
				int dbnumber = -1;

				if(gParty.GetIndexUser(lpObj->PartyNumber, m, pindex, dbnumber) == 0)
				{
					break;
				}

				if(ObjectMaxRange(pindex) && gObjIsConnected(pindex))
				{
					LPOBJ lpPartyObj = &gObj[pindex];

					if(lpPartyObj->Type != OBJ_USER)
					{
						continue;
					}

					if(lpPartyObj->MapNumber != lpObj->MapNumber)
					{
						continue;
					}

					if(gObjCalDistance(lpObj, lpPartyObj) > 10)
					{
						continue;
					}

					if(lpPartyObj->Live == FALSE)
					{
						continue;
					}

					if(lpPartyObj->m_PlayerData->m_Quests[i].QuestIndex != 0xFFFF)
					{
						lpQuest = this->GetQuestData(lpPartyObj->m_PlayerData->m_Quests[i].QuestState, lpPartyObj->m_PlayerData->m_Quests[i].QuestIndex);

						if(lpQuest != NULL)
						{
							for(int k=0;k<5;k++)
							{
								if(lpQuest->wREQUEST[k].RequirementType == 262)
								{
									if(g_GensSystem.GetGensInfluence(lpObj) != g_GensSystem.GetGensInfluence(lpTargetObj))
									{
										if(lpQuest->wREQUEST[k].RequiredCount > lpPartyObj->m_PlayerData->m_Quests[i].ReqKillCount[k])
										{
											lpPartyObj->m_PlayerData->m_Quests[i].ReqKillCount[k]++;
											g_Log.Add(0, __FILE__, __FUNCTION__, "[Quest][Party] (%s)(%s) Added KillCount: (GENS) (%d/%d)", lpPartyObj->AccountID, lpPartyObj->Name,
												lpQuest->btINDEX, lpQuest->btSTATE);
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void CQuests::SetEventQuest(LPOBJ lpObj)
{
	if(lpObj->Type != OBJ_USER)
	{
		return;
	}

	for(int i=50;i<100;i++)
	{
		if(lpObj->m_PlayerData->m_Quest[i] == 255)
		{
			lpObj->m_PlayerData->m_Quest[i] = 0;
		}
	}

	g_Log.Add(0, __FILE__, __FUNCTION__, "[Illusion Temple] [Quest] (%s)(%s) SetQuest: (C:%d, A:%d, S1:%d, S2:%d, S3:%d, S4:%d)",
		lpObj->AccountID, lpObj->Name, lpObj->m_PlayerData->m_Quest[50], lpObj->m_PlayerData->m_Quest[51], lpObj->m_PlayerData->m_Quest[52], lpObj->m_PlayerData->m_Quest[53], lpObj->m_PlayerData->m_Quest[54], lpObj->m_PlayerData->m_Quest[55]);

	g_Log.Add(0, __FILE__, __FUNCTION__, "[Blood Castle] [Quest] (%s)(%s) SetQuest: (C:%d, A:%d, P1:%d, P2:%d, P3:%d, P4:%d, P5:%d, P6:%d)",
		lpObj->AccountID, lpObj->Name, lpObj->m_PlayerData->m_Quest[56], lpObj->m_PlayerData->m_Quest[57], lpObj->m_PlayerData->m_Quest[58], lpObj->m_PlayerData->m_Quest[59], lpObj->m_PlayerData->m_Quest[60], lpObj->m_PlayerData->m_Quest[61], lpObj->m_PlayerData->m_Quest[62], lpObj->m_PlayerData->m_Quest[63]);

	g_Log.Add(0, __FILE__, __FUNCTION__, "[Chaos Castle] [Quest] (%s)(%s) SetQuest: (C:%d, A:%d, PV1:%d, PV2:%d, PV3:%d, PV4:%d, PV5:%d)",
		lpObj->AccountID, lpObj->Name, lpObj->m_PlayerData->m_Quest[64], lpObj->m_PlayerData->m_Quest[65], lpObj->m_PlayerData->m_Quest[66], lpObj->m_PlayerData->m_Quest[67], lpObj->m_PlayerData->m_Quest[68], lpObj->m_PlayerData->m_Quest[69], lpObj->m_PlayerData->m_Quest[70]);

	g_Log.Add(0, __FILE__, __FUNCTION__, "[Devil Square] [Quest] (%s)(%s) SetQuest: (C:%d, A:%d, FT1:%d, FT2:%d, FT3:%d, FT4:%d, FT5:%d)",
		lpObj->AccountID, lpObj->Name, lpObj->m_PlayerData->m_Quest[71], lpObj->m_PlayerData->m_Quest[72], lpObj->m_PlayerData->m_Quest[73], lpObj->m_PlayerData->m_Quest[74], lpObj->m_PlayerData->m_Quest[75], lpObj->m_PlayerData->m_Quest[76], lpObj->m_PlayerData->m_Quest[77]);

}

bool CQuests::CheckStage(LPOBJ lpObj, WORD Index, WORD Stage)
{
	if(lpObj->Type != OBJ_USER)
	{
		return false;
	}

	if(this->m_Quests[Index][Stage].btINDEX == 1) // Tutorial)
	{
		if(this->m_Quests[Index][Stage].btREQSTAGE == lpObj->m_PlayerData->m_Quest[24+Index])
			return true;
	}

	else if(this->m_Quests[Index][Stage].btINDEX == 2) // Chaos Castle
	{
		BYTE btCCIndex = g_ChaosCastle.GetUserLevelToEnter(lpObj->m_Index);

		if(btCCIndex == 0xFF)
		{
			return false;
		}

		if(this->m_Quests[Index][Stage].btEVENTLEVEL != btCCIndex)
		{
			return false;
		}

		if(lpObj->m_PlayerData->m_Quest[64+this->m_Quests[Index][Stage].btEVENTLEVEL] == this->m_Quests[Index][Stage].btREQSTAGE)
			return true;
	}

	else if(this->m_Quests[Index][Stage].btINDEX == 3) // Blood Castle
	{
		BYTE btBCIndex = 0xFF;

		for(int i=0;i<8;i++)
		{
			if(g_BloodCastle.CheckEnterLevel(lpObj->m_Index, i+1) == 0)
			{
				btBCIndex = i;
				break;
			}
		}

		if(btBCIndex == 0xFF)
		{
			return false;
		}

		if(this->m_Quests[Index][Stage].btEVENTLEVEL != btBCIndex)
		{
			return false;
		}

		if(lpObj->m_PlayerData->m_Quest[56+this->m_Quests[Index][Stage].btEVENTLEVEL] == this->m_Quests[Index][Stage].btREQSTAGE)
			return true;
	}

	else if(this->m_Quests[Index][Stage].btINDEX == 4) // Devil Square
	{
		WORD wGate;
		BYTE btDSIndex = g_DevilSquare.GetUserLevelToEnter(lpObj->m_Index, wGate);

		if(btDSIndex == 0xFF)
		{
			return false;
		}

		if(this->m_Quests[Index][Stage].btEVENTLEVEL != btDSIndex)
		{
			return false;
		}

		if(lpObj->m_PlayerData->m_Quest[71+this->m_Quests[Index][Stage].btEVENTLEVEL] == this->m_Quests[Index][Stage].btREQSTAGE)
			return true;
	}

	else if(this->m_Quests[Index][Stage].btINDEX == 5) // Illusion
	{
		if(this->m_Quests[Index][Stage].btEVENTLEVEL != g_IllusionTempleEvent.GetTempleLevel(lpObj->m_Index)-1)
		{
			return false;
		}

		if(lpObj->m_PlayerData->m_Quest[50+this->m_Quests[Index][Stage].btEVENTLEVEL] == this->m_Quests[Index][Stage].btREQSTAGE)
			return true;
	}

	else if(this->m_Quests[Index][Stage].btINDEX == 19) // Watchers of Kundun
	{
		if(lpObj->m_PlayerData->m_Quest[78] == this->m_Quests[Index][Stage].btREQSTAGE)
		{
			return true;
		}
	}

	else if(this->m_Quests[Index][Stage].btINDEX == 8) // Acheron
	{
		if(lpObj->m_PlayerData->m_Quest[79] == this->m_Quests[Index][Stage].btREQSTAGE)
		{
			return true;
		}
	}

	else if(this->m_Quests[Index][Stage].btINDEX == 9) // Skill Tree Part 1
	{
		if(lpObj->m_PlayerData->m_Quest[80] == this->m_Quests[Index][Stage].btREQSTAGE)
		{
			return true;
		}
	}

	else if(this->m_Quests[Index][Stage].btINDEX == 10) // Skill Tree Part 2
	{
		if(lpObj->m_PlayerData->m_Quest[81] == this->m_Quests[Index][Stage].btREQSTAGE)
		{
			return true;
		}
	}

	else if(this->m_Quests[Index][Stage].btINDEX == 11) // Skill Tree Part 3
	{
		if(lpObj->m_PlayerData->m_Quest[82] == this->m_Quests[Index][Stage].btREQSTAGE)
		{
			return true;
		}
	}

	else if(this->m_Quests[Index][Stage].btINDEX == 12) // Skill Tree Part 4
	{
		if(lpObj->m_PlayerData->m_Quest[83] == this->m_Quests[Index][Stage].btREQSTAGE)
		{
			return true;
		}
	}

	else if(this->m_Quests[Index][Stage].btINDEX == 20)
	{
		if(lpObj->m_PlayerData->m_Quest[84] == this->m_Quests[Index][Stage].btREQSTAGE)
		{
			return true;
		}
	}

	else if((this->m_Quests[Index][Stage].btREQSTAGE >= lpObj->m_PlayerData->m_Quest[24+Index] && this->m_Quests[Index][Stage].btREQSTAGE != 255) || (this->m_Quests[Index][Stage].btREQSTAGE == 0xFF && lpObj->m_PlayerData->m_Quest[24+Index] == 0xFF))
		return true;

	return false;
}

void CQuests::SetQuestStage(LPOBJ lpObj, QUEST_INTERNAL_DATA * lpQuest)
{
	if(lpObj == NULL)
	{
		return;
	}

	if(lpObj->Type != OBJ_USER)
	{
		return;
	}

	if(lpQuest == NULL)
	{
		return;
	}

	if(lpQuest->btINDEX == 2)
	{
		lpObj->m_PlayerData->m_Quest[64+lpQuest->btEVENTLEVEL] = lpQuest->btSTAGE;
	}

	else if(lpQuest->btINDEX == 3)
	{
		lpObj->m_PlayerData->m_Quest[56+lpQuest->btEVENTLEVEL] = lpQuest->btSTAGE;
	}

	else if(lpQuest->btINDEX == 4)
	{
		lpObj->m_PlayerData->m_Quest[71+lpQuest->btEVENTLEVEL] = lpQuest->btSTAGE;
	}

	else if(lpQuest->btINDEX == 5)
	{
		lpObj->m_PlayerData->m_Quest[50+lpQuest->btEVENTLEVEL] = lpQuest->btSTAGE;
	}

	else if(lpQuest->btINDEX == 8)
	{
		lpObj->m_PlayerData->m_Quest[79] = lpQuest->btSTAGE;
	}

	else if(lpQuest->btINDEX == 9)
	{
		lpObj->m_PlayerData->m_Quest[80] = lpQuest->btSTAGE;
	}

	else if(lpQuest->btINDEX == 10)
	{
		lpObj->m_PlayerData->m_Quest[81] = lpQuest->btSTAGE;
	}

	else if(lpQuest->btINDEX == 11)
	{
		lpObj->m_PlayerData->m_Quest[82] = lpQuest->btSTAGE;
	}

	else if(lpQuest->btINDEX == 12)
	{
		lpObj->m_PlayerData->m_Quest[83] = lpQuest->btSTAGE;
	}

	else if(lpQuest->btINDEX == 20)
	{
		lpObj->m_PlayerData->m_Quest[84] = lpQuest->btSTAGE;
	}
}

bool CQuests::CheckLevel(LPOBJ lpObj, WORD MinLevel, WORD MaxLevel)
{
	if (lpObj->Type != OBJ_USER)
	{
		return false;
	}

	WORD Level = lpObj->Level + lpObj->m_PlayerData->MasterLevel;

	if(MinLevel == 0xFFFE)
	{
		if(lpObj->m_PlayerData->ChangeUP == 2 && lpObj->Level == 400)
			return true;
		else return false;
	}

	else if(Level >= MinLevel && Level <= MaxLevel)
	{
		return true;
	}

	return false;
}

void CQuests::FinishEventProc(int aIndex, BYTE btEventType, BYTE btEventLevel)
{
	if(!ObjectMaxRange(aIndex))
	{
		g_Log.Add(0, __FILE__, __FUNCTION__, "error-L3 %d %s %s", aIndex, __FILE__, __LINE__);
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Type != OBJ_USER)
	{
		return;
	}
	
	if(lpObj->m_PlayerData->ISBOT == true)
	{
		return;
	}

	for (int k=0;k<50;k++)
	{

		QUEST_INTERNAL_DATA * lpQuest = this->GetQuestData(lpObj->m_PlayerData->m_Quests[k].QuestState, lpObj->m_PlayerData->m_Quests[k].QuestIndex);

		if(lpQuest == NULL)
		{
			continue;
		}

		for(int i=0;i<5;i++)
		{
			if(btEventType == 2)
			{
				if(lpQuest->wREQUEST[i].RequirementType == 257 && lpQuest->wREQUEST[i].RequirementID == btEventLevel)
				{
					lpObj->m_PlayerData->m_Quests[k].ReqKillCount[i] = 1;
						g_Log.Add(0, __FILE__, __FUNCTION__, "[Chaos Castle] (%d) [Quest] (%s)(%s) Event Cleared -> Quest State SUCCESS",
						btEventLevel, lpObj->AccountID, lpObj->Name);
					break;
				}
			}

			else if(btEventType == 3)
			{
				if(lpQuest->wREQUEST[i].RequirementType == 256 && lpQuest->wREQUEST[i].RequirementID == btEventLevel)
				{
					lpObj->m_PlayerData->m_Quests[k].ReqKillCount[i] = 1;
					g_Log.Add(0, __FILE__, __FUNCTION__, "[Blood Castle] (%d) [Quest] (%s)(%s) Event Cleared -> Quest State SUCCESS",
						btEventLevel, lpObj->AccountID, lpObj->Name);
					break;
				}
			}

			else if(btEventType == 4)
			{
				if(lpQuest->wREQUEST[i].RequirementType == 258 && lpQuest->wREQUEST[i].RequirementID == btEventLevel)
				{
					lpObj->m_PlayerData->m_Quests[k].ReqKillCount[i] = 1;
					g_Log.Add(0, __FILE__, __FUNCTION__, "[Devil Square] (%d) [Quest] (%s)(%s) Event Cleared -> Quest State SUCCESS",
						btEventLevel, lpObj->AccountID, lpObj->Name);
					break;
				}
			}

			else if(btEventType == 5)
			{
				if(lpQuest->wREQUEST[i].RequirementType == 259 && lpQuest->wREQUEST[i].RequirementID == btEventLevel)
				{
					lpObj->m_PlayerData->m_Quests[k].ReqKillCount[i] = 1;
					g_Log.Add(0, __FILE__, __FUNCTION__, "[Illusion Temple] (%d) [Quest] (%s)(%s) Event Cleared -> Quest State SUCCESS",
						btEventLevel, lpObj->AccountID, lpObj->Name);
					break;
				}
			}
		}
	}
}

void CQuests::DestroyBloodCastleGate(int aIndex, BYTE btEventLevel)
{
	if(!ObjectMaxRange(aIndex))
	{
		g_Log.Add(0, __FILE__, __FUNCTION__, "error-L3 %d %s %s", aIndex, __FILE__, __LINE__);
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Type != OBJ_USER)
	{
		return;
	}
	
	if(lpObj->m_PlayerData->ISBOT == true)
	{
		return;
	}

	for(int k=0;k<50;k++)
	{
		QUEST_INTERNAL_DATA * lpQuest = this->GetQuestData(lpObj->m_PlayerData->m_Quests[k].QuestState, lpObj->m_PlayerData->m_Quests[k].QuestIndex);

		if(lpQuest == NULL)
		{
			continue;
		}

		for(int i=0;i<5;i++)
		{
			if(lpQuest->wREQUEST[i].RequirementType == 66 && lpQuest->wREQUEST[i].RequirementID == btEventLevel &&
				lpQuest->wREQUEST[i].RequiredCount >= lpObj->m_PlayerData->m_Quests[k].ReqKillCount[i])
			{
				lpObj->m_PlayerData->m_Quests[k].ReqKillCount[i]++;
				g_Log.Add(0, __FILE__, __FUNCTION__, "[Blood Castle] (%d) [Quest] (%s)(%s) Gate Destroyed -> Quest State SUCCESS",
					btEventLevel, lpObj->AccountID, lpObj->Name);
				break;
			}
		}
	}
}

void CQuests::ChaosCastleKillProc(int aIndex, BYTE btEventLevel, BYTE btType)
{
	if(!ObjectMaxRange(aIndex))
	{
		g_Log.Add(0, __FILE__, __FUNCTION__, "error-L3 %d %s %s", aIndex, __FILE__, __LINE__);
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Type != OBJ_USER)
	{
		return;
	}
	
	if(lpObj->m_PlayerData->ISBOT == true)
	{
		return;
	}

	for(int k=0;k<50;k++)
	{
		QUEST_INTERNAL_DATA * lpQuest = this->GetQuestData(lpObj->m_PlayerData->m_Quests[k].QuestState, lpObj->m_PlayerData->m_Quests[k].QuestIndex);

		if(lpQuest == NULL)
		{
			continue;
		}

		for(int i=0;i<5;i++)
		{
			if(lpQuest->wREQUEST[i].RequirementType == 64 && lpQuest->wREQUEST[i].RequirementID == btEventLevel &&
				lpQuest->wREQUEST[i].RequiredCount >= lpObj->m_PlayerData->m_Quests[k].ReqKillCount[i] && btType == 0) // Player
			{
				lpObj->m_PlayerData->m_Quests[k].ReqKillCount[i]++;
				g_Log.Add(0, __FILE__, __FUNCTION__, "[Chaos Castle] (%d) [Quest] (%s)(%s) Player Killed -> Increase Kill Count (%d)",
					btEventLevel, lpObj->AccountID, lpObj->Name, lpObj->m_PlayerData->m_Quests[k].ReqKillCount[i]);
				break;
			}

			else if(lpQuest->wREQUEST[i].RequirementType == 65 && lpQuest->wREQUEST[i].RequirementID == btEventLevel &&
				lpQuest->wREQUEST[i].RequiredCount >= lpObj->m_PlayerData->m_Quests[k].ReqKillCount[i] && btType == 1) // Monster
			{
				lpObj->m_PlayerData->m_Quests[k].ReqKillCount[i]++;
				g_Log.Add(0, __FILE__, __FUNCTION__, "[Chaos Castle] (%d) [Quest] (%s)(%s) Monster Killed -> Increase Kill Count (%d)",
					btEventLevel, lpObj->AccountID, lpObj->Name, lpObj->m_PlayerData->m_Quests[k].ReqKillCount[i]);
				break;
			}
		}
	}
}

void CQuests::AddDevilSquarePoint(int aIndex, BYTE btEventLevel, BYTE btPoint)
{
	if(!ObjectMaxRange(aIndex))
	{
		g_Log.Add(0, __FILE__, __FUNCTION__, "error-L3 %d %s %s", aIndex, __FILE__, __LINE__);
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Type != OBJ_USER)
	{
		return;
	}
	
	if(lpObj->m_PlayerData->ISBOT == true)
	{
		return;
	}

	for(int k=0;k<50;k++)
	{
		QUEST_INTERNAL_DATA * lpQuest = this->GetQuestData(lpObj->m_PlayerData->m_Quests[k].QuestState, lpObj->m_PlayerData->m_Quests[k].QuestIndex);

		if(lpQuest == NULL)
		{
			continue;
		}

		for(int i=0;i<5;i++)
		{
			if(lpQuest->wREQUEST[i].RequirementType == 260 && lpQuest->wREQUEST[i].RequirementID == btEventLevel)
			{
				if((lpObj->m_PlayerData->m_Quests[k].ReqKillCount[i] + btPoint) > 255 || (lpObj->m_PlayerData->m_Quests[k].ReqKillCount[i] + btPoint) > lpQuest->wREQUEST[i].RequiredCount)
				{
					lpObj->m_PlayerData->m_Quests[k].ReqKillCount[i] = lpQuest->wREQUEST[i].RequiredCount;
				}

				else
				{
					lpObj->m_PlayerData->m_Quests[k].ReqKillCount[i] += btPoint;
					g_Log.Add(0, __FILE__, __FUNCTION__, "[Devil Square] (%d) [Quest] (%s)(%s) Player Points (%d)",
						btEventLevel, lpObj->AccountID, lpObj->Name, lpObj->m_PlayerData->m_Quests[k].ReqKillCount[i]);
				}
			}
		}
	}
}

bool CQuests::DropQuestItem(LPOBJ lpMonsterObj)
{
	int MaxHitUser = gObjMonsterTopHitDamageUser(lpMonsterObj);

	if ( MaxHitUser == -1 )
	{
		return false;
	}

	if ( gObj[MaxHitUser].Type != OBJ_USER )
	{
		return false;
	}

	LPOBJ lpObj = &gObj[MaxHitUser];
	
	if ( gParty.GetPartyCount(lpObj->PartyNumber) > 0 )
	{
		return this->DropQuestItemParty(lpObj, lpMonsterObj);
	}

	QUEST_INTERNAL_DATA * lpQuest;
	int ArrayNum;

	for(int i=0;i<30;i++)
	{
		lpQuest = this->GetQuestData(this->m_QuestItems[i].btSTATE, this->m_QuestItems[i].btINDEX);

		if(lpQuest == NULL)
		{
			continue;
		}

		ArrayNum = this->FindQuestArrayNum(lpObj, this->m_QuestItems[i].btINDEX, this->m_QuestItems[i].btSTATE);

		if(ArrayNum == -1)
		{
			continue;
		}

		if(gObjGetItemCountInIventory(MaxHitUser, this->m_QuestItems[i].wItemID) >= this->m_QuestItems[i].btItemCount)
		{
			continue;
		}

		if((rand()%10000) < this->m_QuestItems[i].dwRate)
		{
			ItemSerialCreateSend(lpMonsterObj->m_Index, lpMonsterObj->MapNumber, lpMonsterObj->X, lpMonsterObj->Y, this->m_QuestItems[i].wItemID, 0, 1, 0, 0, 0, MaxHitUser, 0, 0, g_AttributeSystem.GetElement(this->m_QuestItems[i].wItemID), -1);
			return true;
		}

		else
		{
			continue;
		}
	}

	return false;
}

bool CQuests::DropQuestItemParty(LPOBJ lpObj, LPOBJ lpMonsterObj)
{
	QUEST_INTERNAL_DATA * lpQuest;
	int ArrayNum;

	for (int i=0;i<MAX_USER_IN_PARTY;i++)
	{
		int aIndex = gParty.m_PartyS[lpObj->PartyNumber].Number[i];

		if ( aIndex >= 0 && gObj[aIndex].MapNumber == lpMonsterObj->MapNumber )
		{
			for(int j=0;j<30;j++)
			{
				lpQuest = this->GetQuestData(this->m_QuestItems[j].btSTATE, this->m_QuestItems[j].btINDEX);

				if(lpQuest == NULL)
				{
					continue;
				}

				ArrayNum = this->FindQuestArrayNum(lpObj, this->m_QuestItems[j].btINDEX, this->m_QuestItems[j].btSTATE);

				if(ArrayNum == -1)
				{
					continue;
				}

				if(gObjGetItemCountInIventory(aIndex, this->m_QuestItems[j].wItemID) >= this->m_QuestItems[j].btItemCount)
				{
					continue;
				}

				if((rand()%10000) < this->m_QuestItems[j].dwRate)
				{
					ItemSerialCreateSend(lpMonsterObj->m_Index, lpMonsterObj->MapNumber, lpMonsterObj->X, lpMonsterObj->Y, this->m_QuestItems[j].wItemID, 0, 1, 0, 0, 0, aIndex, 0, 0, g_AttributeSystem.GetElement(this->m_QuestItems[j].wItemID), -1);
					return true;
				}

				else
				{
					continue;
				}
			}
		}
	}

	return false;
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

