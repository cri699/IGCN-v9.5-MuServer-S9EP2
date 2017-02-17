//////////////////////////////////////////////////////////////////////
// MasterSkillSystemS6.cpp
#include "stdafx.h"
#include "MasterSkillSystemS6.h"
#include "ReadScript.h"
#include "winutil.h"
#include "TLog.h"
#include "giocp.h"
#include "GameMain.h"
#include "user.h"
#include "GameServer.h"
#include "ObjCalCharacter.h"
#include "configread.h"
#include "BuffEffect.h"
#include "BuffEffectSlot.h"
#include "ObjUseSkill.h"

CMasterSkillSystemS6 MasterLevelSystemS6;

CMasterSkillSystemS6::CMasterSkillSystemS6(void)
{
	this->ResetData();
	this->ResetInfo();
}

CMasterSkillSystemS6::~CMasterSkillSystemS6(void)
{
}

void CMasterSkillSystemS6::ResetData()
{
	for(int i=0;i<200;i++)
	{
		this->m_SkillInfo[i].Reset();
	}
}

void CMasterSkillSystemS6::ResetInfo()
{
	for(int i=0;i<200;i++)
	{
		this->m_SkillTreeData[i].Reset();
	}
}

bool CMasterSkillSystemS6::LoadDataFile(LPSTR lpszFile)
{
	SMDFile = fopen(lpszFile, "r");

	if(!SMDFile)
	{
		g_Log.MsgBox("%s file not found", lpszFile);
		return false;
	}

	this->ResetData();

	int Token;
	int n = 0;
	int index;

	while ( true )
	{
		Token = GetToken();
		if(Token == END)
			break;
		if(Token == NAME && !strcmp("end", TokenString))
			break;

		if(TokenNumber == 0)
		{
			while ( true )
			{
				 Token = GetToken();
				
				 if(strcmp("end",TokenString) == 0)
					 break;
				 index = TokenNumber;
				 Token = GetToken();
				 this->m_SkillInfo[index].btMasterSkillId = TokenNumber;
				 Token = GetToken();
				 this->m_SkillInfo[index].wType = TokenNumber;
				 Token = GetToken();
				 this->m_SkillInfo[index].btGroupType = TokenNumber;
				 Token = GetToken();
				 this->m_SkillInfo[index].btReqPts = TokenNumber;
				 Token = GetToken();
				 this->m_SkillInfo[index].btMaxLevel = TokenNumber;
				 Token = GetToken();
				 this->m_SkillInfo[index].btTmpUnk = TokenNumber;
				 Token = GetToken();
				 this->m_SkillInfo[index].ReqSkillID1 = TokenNumber;
				 Token = GetToken();
				 this->m_SkillInfo[index].ReqSkillID2 = TokenNumber;
				 Token = GetToken();
				 this->m_SkillInfo[index].SkillID = TokenNumber;

				 for(int i = 0 ; i < 20; i ++ )
				 {
					 Token = GetToken();
					 this->m_SkillInfo[index].LevelVal[i] = TokenNumber;
				 }

				 g_Log.Add(0, __FILE__, __FUNCTION__, "[Skill System Season 6] Read Skill: ID:%d Type:%d SkillID:%d",
					 this->m_SkillInfo[index].btMasterSkillId, this->m_SkillInfo[index].wType,
					 this->m_SkillInfo[index].SkillID);

				 n++;
			}
		}
	}

	fclose(SMDFile);

	g_Log.Add(0, __FILE__, __FUNCTION__, "[Skill System Season 6] Loaded %d skills", n);

	return true;
}

bool CMasterSkillSystemS6::LoadInfoFile(LPSTR lpszFile)
{
	SMDFile = fopen(lpszFile, "r");

	if(!SMDFile)
	{
		g_Log.MsgBox("%s file not found", lpszFile);
		return false;
	}

	this->ResetInfo();

	int Token;
	int n = 0;
	int index;

	while ( true )
	{
		Token = GetToken();
		if(Token == END)
			break;
		if(Token == NAME && !strcmp("end", TokenString))
			break;

		if(TokenNumber == 0)
		{
			while ( true )
			{
				 Token = GetToken();
				
				 if(strcmp("end",TokenString) == 0)
					 break;
				 index = TokenNumber;
 				 Token = GetToken();
				 this->m_SkillTreeData[index].iIndex = TokenNumber;
				 Token = GetToken();
				 this->m_SkillTreeData[index].Unk1 = TokenNumber;
				 Token = GetToken();
				 this->m_SkillTreeData[index].Unk2 = TokenNumber;
				 Token = GetToken();
				 this->m_SkillTreeData[index].Unk3 = TokenNumber;
				 Token = GetToken();
				 this->m_SkillTreeData[index].Unk4 = TokenNumber;
				 Token = GetToken();
				 this->m_SkillTreeData[index].Unk5 = TokenNumber;
				 Token = GetToken();
				 this->m_SkillTreeData[index].MaxLevel = TokenNumber;
				 Token = GetToken();
				 this->m_SkillTreeData[index].Unk6 = TokenNumber;
				 Token = GetToken();
				 this->m_SkillTreeData[index].PreviousSkillReq = TokenNumber;
				 Token = GetToken();
				 this->m_SkillTreeData[index].UNK7 = TokenNumber;
				 Token = GetToken();
				 this->m_SkillTreeData[index].SkillId = TokenNumber;
				 Token = GetToken();
				 this->m_SkillTreeData[index].UNK8 = TokenNumber;
				 Token = GetToken();
				 this->m_SkillTreeData[index].UNK9 = TokenNumber;
				 Token = GetToken();
				 this->m_SkillTreeData[index].UNK10 = TokenNumber;
				 Token = GetToken();
				 this->m_SkillTreeData[index].UNK11 = TokenNumber;

				 n++;
			}
		}
	}

	fclose(SMDFile);
	g_Log.Add(0, __FILE__, __FUNCTION__, "[Skill System Season 6] Loaded %d skill informations", n);

	return true;
}

void CMasterSkillSystemS6::AddMasterSkill(int aIndex, PMSG_MASTER_SKILL_RECV *aRecv)
{
	if(!ObjectMaxRange(aIndex))
	{
		g_Log.Add(0, __FILE__, __FUNCTION__, "error-L3 %d %s %s", aIndex, __FILE__, __LINE__);
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	EnterCriticalSection(&lpObj->m_PlayerData->m_MasterSkillCriti);
	
	int skillindex = this->FindSkillIndex(aRecv->MasterSkill, this->GetClass(lpObj->Class));
	int skillnum = this->FindSkillArrayNum(aRecv->MasterSkill);

	if(skillindex == -1)
	{
		g_Log.AddC(0, __FILE__, __FUNCTION__, TColor::Red,  "[Mastering] (%s)(%s) ERROR: Trying to add invalid/not exist skill (skillindex == -1) (%d)",
			lpObj->AccountID, lpObj->Name, aRecv->MasterSkill);
		LeaveCriticalSection(&lpObj->m_PlayerData->m_MasterSkillCriti);
		return;
	}

	if(skillnum == -1)
	{
		g_Log.AddC(0, __FILE__, __FUNCTION__, TColor::Red,  "[Mastering] (%s)(%s) ERROR: Trying to add invalid/not exist skill (skillnum == -1) (%d)",
			lpObj->AccountID, lpObj->Name, aRecv->MasterSkill);
		LeaveCriticalSection(&lpObj->m_PlayerData->m_MasterSkillCriti);
		return;
	}

	int OldSkill = 0;
	bool Result = false;
	int MasterP = this->m_SkillInfo[skillnum].btReqPts;
	int SkillType = MagicDamageC.GetMasterSkillType(aRecv->MasterSkill);
	int level =0;

	if(lpObj->m_PlayerData->MasterPoint < MasterP)
	{
		g_Log.Add(0, __FILE__, __FUNCTION__, "[Mastering] (%s)(%s) AddMasterSkill Fail - not enough MLPoints (need:%d/have:%d) (skill:%d)",
			lpObj->AccountID, lpObj->Name, lpObj->m_PlayerData->MasterPoint, MasterP, aRecv->MasterSkill);
		LeaveCriticalSection(&lpObj->m_PlayerData->m_MasterSkillCriti);
		return;
	}

	if(MagicDamageC.SkillGetRequireClass(lpObj->Class, lpObj->m_PlayerData->ChangeUP, aRecv->MasterSkill) == FALSE)
	{
		g_Log.AddC(0, __FILE__, __FUNCTION__, TColor::Red,  "[Mastering] (%s)(%s) AddMasterSkill Fail - invalid Class/ChangeUP (class:%d/CUP:%d) (skill:%d)",
			lpObj->AccountID, lpObj->Name, lpObj->Class, lpObj->m_PlayerData->ChangeUP, aRecv->MasterSkill);
		LeaveCriticalSection(&lpObj->m_PlayerData->m_MasterSkillCriti);
		return;
	}

	if(SkillType == 3) // passive
	{
		int skillid = this->SearchSkill(lpObj, aRecv->MasterSkill);

		if(skillid == -1)
		{
			if(this->AddSkillInfo(lpObj, aRecv->MasterSkill, 1) != -1)
			{
				Result = true;
				level = 1;
				g_Log.Add(0, __FILE__, __FUNCTION__, "[Mastering] (%s)(%s) Added PASSIVE skill (%d)", lpObj->AccountID, lpObj->Name, aRecv->MasterSkill);
			}
		}

		else
		{
			if(lpObj->m_PlayerData->m_MasterSkills[skillid].SkillLevel >= this->m_SkillInfo[lpObj->m_PlayerData->m_MasterSkills[skillid].SkillIndex].btMaxLevel)
			{
				this->AnsAddMasterSkill(aIndex, Result, aRecv->MasterSkill, level);
				LeaveCriticalSection(&lpObj->m_PlayerData->m_MasterSkillCriti);
				g_Log.AddC(0, __FILE__, __FUNCTION__, TColor::Red, "[Mastering] (%s)(%s) ERROR: SkillLevel >= btMaxLevel (SKILL:%d) (TYPE: PASSIVE UPGRADE)", lpObj->AccountID, lpObj->Name, aRecv->MasterSkill);
				return;
			}

			if(lpObj->m_PlayerData->m_MasterSkills[skillid].SkillLevel < this->m_SkillInfo[lpObj->m_PlayerData->m_MasterSkills[skillid].SkillIndex].btMaxLevel)
			{
				lpObj->m_PlayerData->m_MasterSkills[skillid].SkillLevel++;
			}
			else
			{
				lpObj->m_PlayerData->m_MasterSkills[skillid].SkillLevel = this->m_SkillInfo[lpObj->m_PlayerData->m_MasterSkills[skillid].SkillIndex].btMaxLevel;
			}

			Result = true;
			level = lpObj->m_PlayerData->m_MasterSkills[skillid].SkillLevel;
			g_Log.Add(0, __FILE__, __FUNCTION__, "[Mastering] (%s)(%s) Upgraded PASSIVE skill (%d) (%d) -> (%d)", lpObj->AccountID, lpObj->Name, aRecv->MasterSkill, lpObj->m_PlayerData->m_MasterSkills[skillid].SkillLevel-1, lpObj->m_PlayerData->m_MasterSkills[skillid].SkillLevel);
		}

		gObjCalCharacter(aIndex);
	}

	else if(SkillType == 4) // active
	{
		int skillid = this->SearchSkill(lpObj, aRecv->MasterSkill);
		int OldSkillID = MagicDamageC.GetBrandOfSkill(aRecv->MasterSkill);

		if(OldSkillID == 0 || aRecv->MasterSkill == 418)
		{
			int pos = this->gObjMagicAddMLS(lpObj, aRecv->MasterSkill, 10);

			if(pos == -1)
			{
				this->AnsAddMasterSkill(aIndex, Result, aRecv->MasterSkill, level);
				LeaveCriticalSection(&lpObj->m_PlayerData->m_MasterSkillCriti);
				g_Log.AddC(0, __FILE__, __FUNCTION__, TColor::Red, "[Mastering] (%s)(%s) ERROR: gObjMagicAdd() == -1 (SKILL:%d) (TYPE: ACTIVE FIRST LEVEL)", lpObj->AccountID, lpObj->Name, aRecv->MasterSkill);
				return;
			}

			skillid = this->AddSkillInfo(lpObj,  aRecv->MasterSkill, 10);
			GSProtocol.GCMagicListOneSend(aIndex, pos, aRecv->MasterSkill, 10, 0, 0);

			Result = true;
			level = lpObj->m_PlayerData->m_MasterSkills[skillid].SkillLevel;

			g_Log.Add(0, __FILE__, __FUNCTION__, "[Mastering] (%s)(%s) Added ACTIVE skill FIRST LEVEL (%d)", lpObj->AccountID, lpObj->Name, aRecv->MasterSkill);
		}

		else if(skillid == -1)
		{
			if(MagicDamageC.GetMasterSkillType(OldSkillID) < 3)
			{
				if(gObjGetMagicSearch(lpObj, OldSkillID) != 0)
				{
					if(gObjMagicDel(lpObj, OldSkillID, 0) == -1)
					{
						this->AnsAddMasterSkill(aIndex, Result, aRecv->MasterSkill, level);
						LeaveCriticalSection(&lpObj->m_PlayerData->m_MasterSkillCriti);
						g_Log.AddC(0, __FILE__, __FUNCTION__, TColor::Red, "[Mastering] (%s)(%s) ERROR: gObjMagicDel() == -1 (SKILL:%d) (TYPE: ACTIVE STRENGTHENER)", lpObj->AccountID, lpObj->Name, aRecv->MasterSkill);
						return;
					}

					int pos = this->gObjMagicAddMLS(lpObj, aRecv->MasterSkill, 1);

					if(pos == -1)
					{
						this->AnsAddMasterSkill(aIndex, Result, aRecv->MasterSkill, level);
						LeaveCriticalSection(&lpObj->m_PlayerData->m_MasterSkillCriti);
						g_Log.AddC(0, __FILE__, __FUNCTION__, TColor::Red, "[Mastering] (%s)(%s) ERROR: gObjMagicAdd() == -1 (SKILL:%d) (TYPE: ACTIVE STRENGTHENER)", lpObj->AccountID, lpObj->Name, aRecv->MasterSkill);
						return;
					}

					skillid = this->AddSkillInfo(lpObj, aRecv->MasterSkill, 1);

					Result = true;
					level = lpObj->m_PlayerData->m_MasterSkills[skillid].SkillLevel;

					g_Log.Add(0, __FILE__, __FUNCTION__, "[Mastering] (%s)(%s) Added ACTIVE skill STRENGTHENER (%d)", lpObj->AccountID, lpObj->Name, aRecv->MasterSkill);
				}
			}
			else
			{

				if(gObjGetMagicSearch(lpObj, OldSkillID) != 0)
				{
					int pos = this->gObjMagicAddMLS(lpObj, aRecv->MasterSkill, 1);

					if(pos == -1)
					{
						this->AnsAddMasterSkill(aIndex, Result, aRecv->MasterSkill, level);
						LeaveCriticalSection(&lpObj->m_PlayerData->m_MasterSkillCriti);
						g_Log.AddC(0, __FILE__, __FUNCTION__, TColor::Red, "[Mastering] (%s)(%s) ERROR: gObjMagicAdd() == -1 (SKILL:%d) (TYPE: ACTIVE MASTERY)", lpObj->AccountID, lpObj->Name, aRecv->MasterSkill);
						return;
					}

					skillid = this->AddSkillInfo(lpObj, aRecv->MasterSkill, 1);

					Result = true;
					level = 1;

					g_Log.Add(0, __FILE__, __FUNCTION__, "[Mastering] (%s)(%s) Added ACTIVE skill MASTERY (%d)", lpObj->AccountID, lpObj->Name, aRecv->MasterSkill);
				}
			}
		}
		else
		{
			CMagicInf * lpMagic = gObjGetMagicSearch(lpObj, aRecv->MasterSkill);

			if(lpMagic == NULL)
			{
				g_Log.AddC(0, __FILE__, __FUNCTION__, TColor::Red,  "[Mastering] ERROR - ACTIVE SKILL NOT EXIST IN CMagicInf (%s)(%s)(%d)", lpObj->AccountID, lpObj->Name, aRecv->MasterSkill);
				LeaveCriticalSection(&lpObj->m_PlayerData->m_MasterSkillCriti);
				return;
			}

			if(lpObj->m_PlayerData->m_MasterSkills[skillid].SkillLevel >= this->m_SkillInfo[lpObj->m_PlayerData->m_MasterSkills[skillid].SkillIndex].btMaxLevel)
			{
				this->AnsAddMasterSkill(aIndex, Result, aRecv->MasterSkill, level);
				LeaveCriticalSection(&lpObj->m_PlayerData->m_MasterSkillCriti);
				g_Log.AddC(0, __FILE__, __FUNCTION__, TColor::Red, "[Mastering] (%s)(%s) ERROR: SkillLevel >= btMaxLevel (SKILL:%d) (TYPE: ACTIVE UPGRADE)", lpObj->AccountID, lpObj->Name, aRecv->MasterSkill);
				return;
			}

			if(lpObj->m_PlayerData->m_MasterSkills[skillid].SkillLevel < this->m_SkillInfo[lpObj->m_PlayerData->m_MasterSkills[skillid].SkillIndex].btMaxLevel)
			{
				lpObj->m_PlayerData->m_MasterSkills[skillid].SkillLevel++;
				lpMagic->m_Level++;
			}
			else
			{
				lpObj->m_PlayerData->m_MasterSkills[skillid].SkillLevel = this->m_SkillInfo[lpObj->m_PlayerData->m_MasterSkills[skillid].SkillIndex].btMaxLevel;
				lpMagic->m_Level = this->m_SkillInfo[lpObj->m_PlayerData->m_MasterSkills[skillid].SkillIndex].btMaxLevel;
			}

			Result = true;
			level = lpObj->m_PlayerData->m_MasterSkills[skillid].SkillLevel;
			g_Log.Add(0, __FILE__, __FUNCTION__, "[Mastering] (%s)(%s) Upgraded ACTIVE skill (%d) (%d) -> (%d)", lpObj->AccountID, lpObj->Name, aRecv->MasterSkill, lpObj->m_PlayerData->m_MasterSkills[skillid].SkillLevel-1, lpObj->m_PlayerData->m_MasterSkills[skillid].SkillLevel);
		}
	}

	if(Result == true)
	{
		this->DecreasePoint(lpObj, MasterP);
	}

	this->AnsAddMasterSkill(aIndex, Result, aRecv->MasterSkill, level);

	LeaveCriticalSection(&lpObj->m_PlayerData->m_MasterSkillCriti);
}

void CMasterSkillSystemS6::AnsAddMasterSkill(int aIndex, BYTE btResult, int MasterSkill, int level)
{
	PMSG_ANS_ADDMASTERSKILL pMsg;
	PHeadSubSetB((LPBYTE)&pMsg,0xF3,0x52,sizeof(pMsg));

	pMsg.btResult = btResult;
	pMsg.dwSkillId = MasterSkill;
	pMsg.btLevel = level;
	pMsg.btMasterSkillId = this->FindSkillIndex(MasterSkill,this->GetClass(gObj[aIndex].Class));
	pMsg.wLevelPoint = gObj[aIndex].m_PlayerData->MasterPoint;
	
	if(level < this->m_SkillInfo[this->FindSkillArrayNum(MasterSkill)].btMaxLevel)
	{
		pMsg.nextvalue = this->m_SkillInfo[this->FindSkillArrayNum(MasterSkill)].LevelVal[level];
	}
	else
	{
		pMsg.nextvalue = this->m_SkillInfo[this->FindSkillArrayNum(MasterSkill)].LevelVal[this->m_SkillInfo[this->FindSkillArrayNum(MasterSkill)].btMaxLevel-1];
	}

	pMsg.value = this->m_SkillInfo[this->FindSkillArrayNum(MasterSkill)].LevelVal[level-1];
	IOCP.DataSend(aIndex,(LPBYTE)&pMsg,pMsg.h.size);
}

BYTE CMasterSkillSystemS6::FindSkillIndex(WORD Skillid, BYTE Class)
{
	for(int i = 0;i<512;i++)
	{
		if(this->m_SkillInfo[i].SkillID == Skillid)
		{
			if(this->m_SkillInfo[i].wType == -1 || this->m_SkillInfo[i].wType == 65535 || this->m_SkillInfo[i].wType == Class || (this->m_SkillInfo[i].wType == MST_MG+MST_DK && (Class == MST_MG || Class == MST_DK)))
			{
				return this->m_SkillInfo[i].btMasterSkillId;
			}
		}
	}
	return 0;
}

WORD CMasterSkillSystemS6::FindSkillArrayNum(WORD Skillid)
{
	for(int i = 0;i < 512; i ++)
	{
		if(this->m_SkillInfo[i].SkillID == Skillid)
		{
			return i;
		}
	}
	return -1;
}

BYTE CMasterSkillSystemS6::GetClass(BYTE Class)
{
	switch(Class)
	{
		case CLASS_KNIGHT:
			return MST_DK;
		case CLASS_ELF:
			return MST_ELF;
		case CLASS_WIZARD:
			return MST_DW;
		case CLASS_SUMMONER:
			return MST_SUM;
		case CLASS_DARKLORD:
			return MST_DL;
		case CLASS_MAGUMSA:
			return MST_MG;
		case CLASS_RAGEFIGHTER:
			return MST_RF;
		default:
			return -1;
	}
}

void CMasterSkillSystemS6::GCSendMasterSkillList(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	BYTE pBuff[4096];
	PMSG_MASTERSKILL_COUNT lpCount;
	MASTERSKILL_STRUCT lpBody;

	int sOfs = sizeof(PMSG_MASTERSKILL_COUNT);
	int lpBodySize = sizeof(lpBody);

	int count = 0;

	EnterCriticalSection(&lpObj->m_PlayerData->m_MasterSkillCriti);

	// CHANGED SINCE 7.0.4.1
	for(int i=0; i<120;i++)
	{
		if(lpObj->m_PlayerData->m_MasterSkills[i].SkillMagicId != 0xFFFF)
		{
			lpBody.btLevel = lpObj->m_PlayerData->m_MasterSkills[i].SkillLevel;
			lpBody.btMasterSkillId = this->FindSkillIndex(lpObj->m_PlayerData->m_MasterSkills[i].SkillMagicId,this->GetClass(gObj[aIndex].Class));
			lpBody.value = this->m_SkillInfo[this->FindSkillArrayNum(lpObj->m_PlayerData->m_MasterSkills[i].SkillMagicId)].LevelVal[lpObj->m_PlayerData->m_MasterSkills[i].SkillLevel-1]; // todo 
			if(lpBody.btLevel < this->m_SkillInfo[this->FindSkillArrayNum(lpObj->m_PlayerData->m_MasterSkills[i].SkillMagicId)].btMaxLevel)
			{
				lpBody.nextvalue = this->m_SkillInfo[this->FindSkillArrayNum(lpObj->m_PlayerData->m_MasterSkills[i].SkillMagicId)].LevelVal[lpObj->m_PlayerData->m_MasterSkills[i].SkillLevel]; // todo ; // todo
			}
			else
			{
				lpBody.nextvalue = this->m_SkillInfo[this->FindSkillArrayNum(lpObj->m_PlayerData->m_MasterSkills[i].SkillMagicId)].LevelVal[lpObj->m_PlayerData->m_MasterSkills[i].SkillLevel-1]; // todo ; // todo

			}
			memcpy(&pBuff[sOfs],&lpBody,sizeof(lpBody));
			sOfs += sizeof(lpBody);
			count++;
			
		}
	}

	lpCount.h.c = 0xC2;
	lpCount.h.headcode = 0xF3;
	lpCount.h.subcode = 0x53;
	lpCount.h.sizeH = SET_NUMBERH(sOfs);
	lpCount.h.sizeL = SET_NUMBERL(sOfs);
	lpCount.count = count;

	memcpy(pBuff,&lpCount,sizeof(lpCount));

	IOCP.DataSend(aIndex, pBuff, sOfs);

	LeaveCriticalSection(&lpObj->m_PlayerData->m_MasterSkillCriti);

	g_Log.Add(0, __FILE__, __FUNCTION__, "[%d][%s][%s] Master Skill List send (%d)",aIndex,lpObj->AccountID,lpObj->Name, count);
}

float CMasterSkillSystemS6::GetEffectValue(LPOBJ lpObj, int Skill)
{
	if(lpObj->Type != OBJ_USER || lpObj->m_PlayerData->ISBOT == true)
		return 0.00;

	for(int i=0;i<120;i++)
	{
		if(lpObj->m_PlayerData->m_MasterSkills[i].SkillMagicId == Skill)
		{
			return this->m_SkillInfo[lpObj->m_PlayerData->m_MasterSkills[i].SkillIndex].LevelVal[lpObj->m_PlayerData->m_MasterSkills[i].SkillLevel-1];
		}
	}

	return 0.00;
}

void CMasterSkillSystemS6::CalcEffect(OBJECTSTRUCT * lpObj)
{
	if(lpObj->Type != OBJ_USER || lpObj->m_PlayerData->ISBOT == true)
		return;

	for(int i=0;i<120;i++)
	{
		if(lpObj->m_PlayerData->m_MasterSkills[i].SkillMagicId != 0xFFFF && lpObj->m_PlayerData->m_MasterSkills[i].SkillIndex != 0xFFFF)
		{
			switch ( lpObj->m_PlayerData->m_MasterSkills[i].SkillMagicId )
			{
				case 302: // INCREASE MAXIMUM SD
				case 580: // RF VERSION
					lpObj->iAddShield += this->GetEffectValue(lpObj, 302);
					lpObj->iAddShield += this->GetEffectValue(lpObj, 580);
					GSProtocol.GCReFillSend(lpObj->m_Index, lpObj->AddLife+lpObj->MaxLife, 0xFE, 0, lpObj->iAddShield+lpObj->iMaxShield);
					GSProtocol.GCReFillSend(lpObj->m_Index, lpObj->Life, 0xFF, 0, lpObj->iShield);
					break;
				case 304:
				case 582:
					lpObj->m_Resistance[R_POISON] += this->GetEffectValue(lpObj, 304);
					lpObj->m_Resistance[R_POISON] += this->GetEffectValue(lpObj, 582);
					break;
				case 308:
				case 586:
					lpObj->m_Resistance[R_LIGHTNING] += this->GetEffectValue(lpObj, 308);
					lpObj->m_Resistance[R_LIGHTNING] += this->GetEffectValue(lpObj, 586);
					break;
				case 309:
				case 587:
					lpObj->m_Defense += this->GetEffectValue(lpObj, 309);
					lpObj->m_Defense += this->GetEffectValue(lpObj, 587);
					break;
				case 311:
				case 589:
					lpObj->m_Resistance[R_ICE] += this->GetEffectValue(lpObj, 311);
					lpObj->m_Resistance[R_ICE] += this->GetEffectValue(lpObj, 589);
					break;
				case 334:
				case 600:
					lpObj->AddLife += this->GetEffectValue(lpObj, 334);
					lpObj->AddLife += this->GetEffectValue(lpObj, 600);
					GSProtocol.GCReFillSend(lpObj->m_Index, lpObj->AddLife+lpObj->MaxLife, 0xFE, 0, lpObj->iAddShield+lpObj->iMaxShield);
					GSProtocol.GCReFillSend(lpObj->m_Index, lpObj->Life, 0xFF, 0, lpObj->iShield);
					break;
				case 335:
				case 421:
				case 557:
					lpObj->m_AttackDamageMinLeft += this->GetEffectValue(lpObj, 335);
					lpObj->m_AttackDamageMaxLeft += this->GetEffectValue(lpObj, 335);
					lpObj->m_AttackDamageMinRight += this->GetEffectValue(lpObj, 335);
					lpObj->m_AttackDamageMaxRight += this->GetEffectValue(lpObj, 335);
					lpObj->m_AttackDamageMinLeft += this->GetEffectValue(lpObj, 421);
					lpObj->m_AttackDamageMaxLeft += this->GetEffectValue(lpObj, 421);
					lpObj->m_AttackDamageMinRight += this->GetEffectValue(lpObj, 421);
					lpObj->m_AttackDamageMaxRight += this->GetEffectValue(lpObj, 421);
					lpObj->m_AttackDamageMinLeft += this->GetEffectValue(lpObj, 557);
					lpObj->m_AttackDamageMaxLeft += this->GetEffectValue(lpObj, 557);
					lpObj->m_AttackDamageMinRight += this->GetEffectValue(lpObj, 557);
					lpObj->m_AttackDamageMaxRight += this->GetEffectValue(lpObj, 557);
					break;
				case 338:
				case 601:
					lpObj->AddMana += this->GetEffectValue(lpObj, 338);
					lpObj->AddMana += this->GetEffectValue(lpObj, 601);
					GSProtocol.GCManaSend(lpObj->m_Index, lpObj->MaxMana+lpObj->AddMana, -2, 0, lpObj->MaxBP+lpObj->AddBP);
					GSProtocol.GCManaSend(lpObj->m_Index, lpObj->Mana, -1, 0, lpObj->BP);
					break;
				case 348:
					if(lpObj->pInventory[0].IsItem() == TRUE && lpObj->pInventory[0].m_TwoHand == TRUE)
					{
						lpObj->m_AttackDamageMinRight += this->GetEffectValue(lpObj, 348);
						lpObj->m_AttackDamageMaxRight += this->GetEffectValue(lpObj, 348);
					}
					break;
				case 349:
					if(lpObj->pInventory[0].IsItem() == TRUE && lpObj->pInventory[0].m_TwoHand == FALSE && lpObj->pInventory[0].m_Type >= ITEMGET(0,0) && lpObj->pInventory[0].m_Type <= ITEMGET(0,31))
					{
						lpObj->m_AttackDamageMinRight += this->GetEffectValue(lpObj, 349);
						lpObj->m_AttackDamageMaxRight += this->GetEffectValue(lpObj, 349);
					}
					else if(lpObj->pInventory[1].IsItem() == TRUE && lpObj->pInventory[1].m_TwoHand == FALSE && lpObj->pInventory[1].m_Type >= ITEMGET(0,0) && lpObj->pInventory[1].m_Type <= ITEMGET(0,31))
					{
						lpObj->m_AttackDamageMinLeft += this->GetEffectValue(lpObj, 349);
						lpObj->m_AttackDamageMaxLeft += this->GetEffectValue(lpObj, 349);
					}
					break;
				case 350:
					if(lpObj->pInventory[0].IsItem() == TRUE && lpObj->pInventory[0].m_TwoHand == TRUE && lpObj->pInventory[0].m_Type >= ITEMGET(2,0) && lpObj->pInventory[0].m_Type <= ITEMGET(2,7))
					{
						lpObj->m_AttackDamageMinRight += this->GetEffectValue(lpObj, 350);
						lpObj->m_AttackDamageMaxRight += this->GetEffectValue(lpObj, 350);
					}
					else if(lpObj->pInventory[1].IsItem() == TRUE && lpObj->pInventory[1].m_TwoHand == TRUE && lpObj->pInventory[1].m_Type >= ITEMGET(2,0) && lpObj->pInventory[1].m_Type <= ITEMGET(2,7))
					{
						lpObj->m_AttackDamageMinLeft += this->GetEffectValue(lpObj, 350);
						lpObj->m_AttackDamageMaxLeft += this->GetEffectValue(lpObj, 350);
					}
					else if(lpObj->pInventory[1].IsItem() == TRUE && lpObj->pInventory[1].m_TwoHand == TRUE && lpObj->pInventory[1].m_Type == ITEMGET(2,20))
					{
						lpObj->m_AttackDamageMinLeft += this->GetEffectValue(lpObj, 350);
						lpObj->m_AttackDamageMaxLeft += this->GetEffectValue(lpObj, 350);
					}
					else if(lpObj->pInventory[0].IsItem() == TRUE && lpObj->pInventory[1].m_TwoHand == TRUE && lpObj->pInventory[1].m_Type == ITEMGET(2,20))
					{
						lpObj->m_AttackDamageMinLeft += this->GetEffectValue(lpObj, 350);
						lpObj->m_AttackDamageMaxLeft += this->GetEffectValue(lpObj, 350);
					}
					break;
				case 351:
					if(lpObj->pInventory[0].IsItem() == TRUE && lpObj->pInventory[0].m_TwoHand == TRUE && lpObj->pInventory[0].m_Type >= ITEMGET(3,0) && lpObj->pInventory[0].m_Type <= ITEMGET(3,12))
					{
						lpObj->m_AttackDamageMinRight += this->GetEffectValue(lpObj, 351);
						lpObj->m_AttackDamageMaxRight += this->GetEffectValue(lpObj, 351);
					}
					break;
				case 353:
					if(lpObj->pInventory[0].IsItem() == TRUE && lpObj->pInventory[0].m_TwoHand == FALSE && lpObj->pInventory[0].m_Type >= ITEMGET(0,0) && lpObj->pInventory[0].m_Type <= ITEMGET(0,31))
					{
						lpObj->m_AttackSpeed += this->GetEffectValue(lpObj, 353);
						lpObj->m_MagicSpeed += this->GetEffectValue(lpObj, 353);
					}
					else if(lpObj->pInventory[1].IsItem() == TRUE && lpObj->pInventory[1].m_TwoHand == FALSE && lpObj->pInventory[1].m_Type >= ITEMGET(0,0) && lpObj->pInventory[1].m_Type <= ITEMGET(0,31))
					{
						lpObj->m_AttackSpeed += this->GetEffectValue(lpObj, 353);
						lpObj->m_MagicSpeed += this->GetEffectValue(lpObj, 353);
					}
					break;
				case 361:
				case 442:
				case 607:
					lpObj->m_AttackDamageMinLeft += this->GetEffectValue(lpObj, 361);
					lpObj->m_AttackDamageMinRight += this->GetEffectValue(lpObj, 361);
					lpObj->m_AttackDamageMinLeft += this->GetEffectValue(lpObj, 442);
					lpObj->m_AttackDamageMinRight += this->GetEffectValue(lpObj, 442);
					lpObj->m_AttackDamageMinLeft += this->GetEffectValue(lpObj, 607);
					lpObj->m_AttackDamageMinRight += this->GetEffectValue(lpObj, 607);
					break;
				case 386:
					lpObj->m_MagicDamageMin += this->GetEffectValue(lpObj, 386);
					lpObj->m_MagicDamageMax += this->GetEffectValue(lpObj, 386);
					break;
				case 397:
					if(lpObj->pInventory[0].IsItem() == TRUE && lpObj->pInventory[0].m_TwoHand == FALSE && lpObj->pInventory[0].m_Type >= ITEMGET(5,0) && lpObj->pInventory[0].m_Type < ITEMGET(6,0))
					{
						lpObj->m_MagicDamageMin += this->GetEffectValue(lpObj, 397);
						lpObj->m_MagicDamageMax += this->GetEffectValue(lpObj, 397);
					}
					break;
				case 398:
					if(lpObj->pInventory[0].IsItem() == TRUE && lpObj->pInventory[0].m_TwoHand == TRUE && lpObj->pInventory[0].m_Type >= ITEMGET(5,0) && lpObj->pInventory[0].m_Type < ITEMGET(6,0))
					{
						lpObj->m_MagicDamageMin += this->GetEffectValue(lpObj, 398);
						lpObj->m_MagicDamageMax += this->GetEffectValue(lpObj, 398);
					}
					break;
				case 399:
					if ( lpObj->pInventory[1].m_Type >= ITEMGET(6,0) && lpObj->pInventory[1].m_Type < ITEMGET(7,0) )
					{
						lpObj->m_Defense += this->GetEffectValue(lpObj, 399);
						//lpObj->m_Defense += this->GetEffectValue(lpObj, 402);
					}
					break;
				case 400:
					if(lpObj->pInventory[0].IsItem() == TRUE && lpObj->pInventory[0].m_TwoHand == FALSE && lpObj->pInventory[0].m_Type >= ITEMGET(5,0) && lpObj->pInventory[0].m_Type < ITEMGET(6,0))
					{
						lpObj->m_AttackSpeed += this->GetEffectValue(lpObj, 400);
						lpObj->m_MagicSpeed += this->GetEffectValue(lpObj, 400);
					}
					break;
				case 402:
					if ( lpObj->pInventory[1].m_Type >= ITEMGET(6,0) && lpObj->pInventory[1].m_Type < ITEMGET(7,0) )
					{
						lpObj->m_SuccessfulBlocking += this->GetEffectValue(lpObj, 402);
					}
					break;
				case 437:
					if ( lpObj->pInventory[1].m_Type >= ITEMGET(6,0) && lpObj->pInventory[1].m_Type < ITEMGET(7,0) )
					{
						lpObj->m_Defense += this->GetEffectValue(lpObj, 437);
						//lpObj->m_Defense += this->GetEffectValue(lpObj, 440);
					}
					break;
				case 440:
					if ( lpObj->pInventory[1].m_Type >= ITEMGET(6,0) && lpObj->pInventory[1].m_Type < ITEMGET(7,0) )
					{
						lpObj->m_SuccessfulBlocking += this->GetEffectValue(lpObj, 440);
					}
					break;
				case 405:
					lpObj->m_MagicDamageMin += this->GetEffectValue(lpObj, 405);
					break;
				case 435:
					if(lpObj->pInventory[1].IsItem() == TRUE && lpObj->pInventory[1].m_Type >= ITEMGET(4,0) && lpObj->pInventory[1].m_Type < ITEMGET(5,0))
					{
						lpObj->m_AttackDamageMinLeft += this->GetEffectValue(lpObj, 435);
						lpObj->m_AttackDamageMaxLeft += this->GetEffectValue(lpObj, 435);
					}
					break;
				case 436:
					if(lpObj->pInventory[0].IsItem() == TRUE && lpObj->pInventory[0].m_Type >= ITEMGET(4,0) && lpObj->pInventory[0].m_Type < ITEMGET(5,0))
					{
						lpObj->m_AttackDamageMinRight += this->GetEffectValue(lpObj, 436);
						lpObj->m_AttackDamageMaxRight += this->GetEffectValue(lpObj, 436);
					}
					break;
				case 438:
					if(lpObj->pInventory[1].IsItem() == TRUE && lpObj->pInventory[1].m_Type >= ITEMGET(4,0) && lpObj->pInventory[1].m_Type < ITEMGET(5,0))
					{
						lpObj->m_AttackSpeed += this->GetEffectValue(lpObj, 438);
						lpObj->m_MagicSpeed += this->GetEffectValue(lpObj, 438);
					}
					break;
				case 457:
					lpObj->m_MagicDamageMin += MasterLevelSystemS6.GetEffectValue(lpObj, 457);
					lpObj->m_MagicDamageMax += MasterLevelSystemS6.GetEffectValue(lpObj, 457);
					lpObj->m_CurseDamageMin += MasterLevelSystemS6.GetEffectValue(lpObj, 457);
					lpObj->m_CurseDamageMax += MasterLevelSystemS6.GetEffectValue(lpObj, 457);
					break;
				case 465:
					if(lpObj->pInventory[0].IsItem() == TRUE && lpObj->pInventory[0].m_Type >= ITEMGET(5,0) && lpObj->pInventory[0].m_Type < ITEMGET(6,0) && lpObj->pInventory[0].m_RequireClass[0] == FALSE)
					{
						lpObj->m_MagicDamageMin += this->GetEffectValue(lpObj, 465);
						lpObj->m_MagicDamageMax += this->GetEffectValue(lpObj, 465);
					}
					break;
				case 466:
					if(lpObj->pInventory[1].IsItem() == TRUE && lpObj->pInventory[1].m_Type >= ITEMGET(5,0) && lpObj->pInventory[1].m_Type < ITEMGET(6,0) && lpObj->pInventory[1].m_RequireClass[0] == FALSE)
					{
						lpObj->m_CurseDamageMin += this->GetEffectValue(lpObj, 466);
						lpObj->m_CurseDamageMax += this->GetEffectValue(lpObj, 466);
					}
					break;
				case 468:
					if(lpObj->pInventory[1].IsItem() == TRUE && lpObj->pInventory[1].m_Type >= ITEMGET(5,0) && lpObj->pInventory[1].m_Type < ITEMGET(6,0) && lpObj->pInventory[1].m_RequireClass[0] == FALSE)
					{
						lpObj->m_MagicSpeed += this->GetEffectValue(lpObj, 468);
					}
					break;
				case 471:
					lpObj->m_MagicDamageMin += this->GetEffectValue(lpObj, 471);
					lpObj->m_CurseDamageMin += this->GetEffectValue(lpObj, 471);
					break;
				case 485:
					lpObj->m_AttackDamageMinLeft += this->GetEffectValue(lpObj, 485);
					lpObj->m_AttackDamageMaxLeft += this->GetEffectValue(lpObj, 485);
					lpObj->m_AttackDamageMinRight += this->GetEffectValue(lpObj, 485);
					lpObj->m_AttackDamageMaxRight += this->GetEffectValue(lpObj, 485);
					break;
				case 488:
					lpObj->m_MagicDamageMin += this->GetEffectValue(lpObj, 488);
					lpObj->m_MagicDamageMax += this->GetEffectValue(lpObj, 488);
					break;
				case 510:
					if(lpObj->pInventory[8].m_Type == ITEMGET(13,4))
					{
						lpObj->m_Defense += MasterLevelSystemS6.GetEffectValue(lpObj, 510);
					}
					break;
				case 513:
					lpObj->m_AttackDamageMinLeft += this->GetEffectValue(lpObj, 513);
					lpObj->m_AttackDamageMaxLeft += this->GetEffectValue(lpObj, 513);
					lpObj->m_AttackDamageMinRight += this->GetEffectValue(lpObj, 513);
					lpObj->m_AttackDamageMaxRight += this->GetEffectValue(lpObj, 513);
					break;
				case 568:
					if(lpObj->pInventory[0].IsItem() == TRUE && lpObj->pInventory[0].m_Type >= ITEMGET(0,32) && lpObj->pInventory[0].m_Type <= ITEMGET(0,35))
					{
						lpObj->m_AttackDamageMinRight += this->GetEffectValue(lpObj, 568);
						lpObj->m_AttackDamageMaxRight += this->GetEffectValue(lpObj, 568);
					}
					else if(lpObj->pInventory[1].IsItem() == TRUE && lpObj->pInventory[1].m_Type >= ITEMGET(0,32) && lpObj->pInventory[1].m_Type <= ITEMGET(0,35))
					{
						lpObj->m_AttackDamageMinLeft += this->GetEffectValue(lpObj, 568);
						lpObj->m_AttackDamageMaxLeft += this->GetEffectValue(lpObj, 568);
					}
					break;
					// EX700 SKILLS
				case 315:
					{
						WORD Helm = lpObj->pInventory[2].m_Type % MAX_SUBTYPE_ITEMS;
						WORD Armor = lpObj->pInventory[3].m_Type % MAX_SUBTYPE_ITEMS;
						WORD Pants = lpObj->pInventory[4].m_Type % MAX_SUBTYPE_ITEMS;
						WORD Gloves = lpObj->pInventory[5].m_Type % MAX_SUBTYPE_ITEMS;
						WORD Boots = lpObj->pInventory[6].m_Type % MAX_SUBTYPE_ITEMS;

						if(Helm == Armor && Armor == Pants && Pants == Gloves && Gloves == Boots && Boots == Helm)
						{
							lpObj->m_Defense += this->GetEffectValue(lpObj, 315);
						}
					}
					break;
				case 316:
					lpObj->DamageReflect += this->GetEffectValue(lpObj, 316);
					break;
				case 322:
					if(lpObj->pInventory[7].m_Type == ITEMGET(12,36))
					{
						lpObj->m_Defense += this->GetEffectValue(lpObj, 322);
					}
					break;
				case 324:
					if(lpObj->pInventory[7].m_Type == ITEMGET(12,36))
					{
						lpObj->m_AttackDamageMinLeft += this->GetEffectValue(lpObj, 324);
						lpObj->m_AttackDamageMaxLeft += this->GetEffectValue(lpObj, 324);
						lpObj->m_AttackDamageMinRight += this->GetEffectValue(lpObj, 324);
						lpObj->m_AttackDamageMaxRight += this->GetEffectValue(lpObj, 324);
					}
					break;
				case 375:
					if(lpObj->pInventory[7].m_Type == ITEMGET(12,37))
					{
						lpObj->m_Defense += this->GetEffectValue(lpObj, 375);
					}
					break;
				case 377:
					if(lpObj->pInventory[7].m_Type == ITEMGET(12,37))
					{
						lpObj->m_MagicDamageMin += this->GetEffectValue(lpObj, 377);
						lpObj->m_MagicDamageMax += this->GetEffectValue(lpObj, 377);
					}
					break;
				case 341:
					lpObj->AddBP += this->GetEffectValue(lpObj, 341);
					break;
				case 364:
					lpObj->m_AttackDamageMaxRight += this->GetEffectValue(lpObj, 364);
					lpObj->m_AttackDamageMaxLeft += this->GetEffectValue(lpObj, 364);
					break;
				case 369:
					lpObj->m_ExcelentDamage += this->GetEffectValue(lpObj, 369);
					break;
				case 366:
					lpObj->m_CriticalDamage += this->GetEffectValue(lpObj, 366);
					break;
				case 407:
					lpObj->m_MagicDamageMax += this->GetEffectValue(lpObj, 407);
					break;
				case 410:
					if(lpObj->pInventory[7].m_Type == ITEMGET(12,38))
					{
						lpObj->m_Defense += this->GetEffectValue(lpObj, 410);
					}
					break;
				case 412:
					if(lpObj->pInventory[7].m_Type == ITEMGET(12,38))
					{
						lpObj->m_AttackDamageMinLeft += this->GetEffectValue(lpObj, 412);
						lpObj->m_AttackDamageMaxLeft += this->GetEffectValue(lpObj, 412);
						lpObj->m_AttackDamageMinRight += this->GetEffectValue(lpObj, 412);
						lpObj->m_AttackDamageMaxRight += this->GetEffectValue(lpObj, 412);
					}
					break;
				case 446:
					if(lpObj->pInventory[7].m_Type == ITEMGET(12,43))
					{
						lpObj->m_Defense += this->GetEffectValue(lpObj, 446);
					}
					break;
				case 447:
					if(lpObj->pInventory[7].m_Type == ITEMGET(12,43))
					{
						lpObj->m_AttackDamageMinLeft += this->GetEffectValue(lpObj, 447);
						lpObj->m_AttackDamageMaxLeft += this->GetEffectValue(lpObj, 447);
						lpObj->m_AttackDamageMinRight += this->GetEffectValue(lpObj, 447);
						lpObj->m_AttackDamageMaxRight += this->GetEffectValue(lpObj, 447);
					}
					break;
				case 473:
					lpObj->m_MagicDamageMax += this->GetEffectValue(lpObj, 473);
					lpObj->m_CurseDamageMax += this->GetEffectValue(lpObj, 473);
					break;
				case 476:
					if(lpObj->pInventory[7].m_Type == ITEMGET(12,39))
					{
						lpObj->m_Defense += this->GetEffectValue(lpObj, 476);
					}
					break;
				case 478:
					if(lpObj->pInventory[7].m_Type == ITEMGET(12,39))
					{
						lpObj->m_AttackDamageMinLeft += this->GetEffectValue(lpObj, 478);
						lpObj->m_AttackDamageMaxLeft += this->GetEffectValue(lpObj, 478);
						lpObj->m_AttackDamageMinRight += this->GetEffectValue(lpObj, 478);
						lpObj->m_AttackDamageMaxRight += this->GetEffectValue(lpObj, 478);
						lpObj->m_MagicDamageMin += this->GetEffectValue(lpObj, 478);
						lpObj->m_MagicDamageMax += this->GetEffectValue(lpObj, 478);
					}
					break;
				case 549:
					if(lpObj->pInventory[7].m_Type == ITEMGET(12,43))
					{
						lpObj->m_Defense += this->GetEffectValue(lpObj, 549);
					}
					break;
				case 550:
					if(lpObj->pInventory[7].m_Type == ITEMGET(12,43))
					{
						lpObj->m_AttackDamageMinLeft += this->GetEffectValue(lpObj, 550);
						lpObj->m_AttackDamageMaxLeft += this->GetEffectValue(lpObj, 550);
						lpObj->m_AttackDamageMinRight += this->GetEffectValue(lpObj, 550);
						lpObj->m_AttackDamageMaxRight += this->GetEffectValue(lpObj, 550);
					}
					break;
			}
		}
	}
}

bool CMasterSkillSystemS6::CheckMasterySkill(LPOBJ lpObj, int skill) // FINISHED (eX700 PLUS)
{
	switch ( skill )
	{
		case 336:
			return (bool)gObjGetMagicSearch(lpObj, 339);
		case 339:
			return (bool)gObjGetMagicSearch(lpObj, 342);
		case MST_RAGEFUL_STR:
			return (bool)gObjGetMagicSearch(lpObj, MST_RAGEFUL_MAS);
		case MST_TWISTINGSLASH_STR:
			return (bool)gObjGetMagicSearch(lpObj, MST_TWISTINGSLASH_MAS);
		case MST_SWELLLIFE_STR:
			return (bool)gObjGetMagicSearch(lpObj, MST_SWELLLIFE_PRO);
		case 337:
			return (bool)gObjGetMagicSearch(lpObj, 340);
		case 340:
			return (bool)gObjGetMagicSearch(lpObj, 343);
		case 344:
			return (bool)gObjGetMagicSearch(lpObj, 346);
		case 360:
			return (bool)gObjGetMagicSearch(lpObj, 363);
		case 380:
			return (bool)gObjGetMagicSearch(lpObj, 383);
		case 390:
			return (bool)gObjGetMagicSearch(lpObj, 394);
		case 391:
			return (bool)gObjGetMagicSearch(lpObj, 393);
		case 495:
			return (bool)gObjGetMagicSearch(lpObj, 497);
		case 403:
			return (bool)gObjGetMagicSearch(lpObj, 404);
		case 404:
			return (bool)gObjGetMagicSearch(lpObj, 406);
		case 411:
			return (bool)gObjGetMagicSearch(lpObj, 431);
		case 414:
			return (bool)gObjGetMagicSearch(lpObj, 418);
		case 417:
			return (bool)gObjGetMagicSearch(lpObj, 423);
		case 420:
			return (bool)gObjGetMagicSearch(lpObj, 422);
		case 426:
			return (bool)gObjGetMagicSearch(lpObj, 429);
		case 427:
			return (bool)gObjGetMagicSearch(lpObj, 434);
		case 430:
			return (bool)gObjGetMagicSearch(lpObj, 433);
		case 458:
			return (bool)gObjGetMagicSearch(lpObj, 462);
		case 461:
			return (bool)gObjGetMagicSearch(lpObj, 463);
		case 469:
			return (bool)gObjGetMagicSearch(lpObj, 470);
		case 470:
			return (bool)gObjGetMagicSearch(lpObj, 472);
		case 489:
			return (bool)gObjGetMagicSearch(lpObj, 491);
		case 490:
			return (bool)gObjGetMagicSearch(lpObj, 493);
		case 492:
			return (bool)gObjGetMagicSearch(lpObj, 494);
		case 511:
			return (bool)gObjGetMagicSearch(lpObj, 515);
		case 515:
			return (bool)gObjGetMagicSearch(lpObj, 517);
		case 517:
			return (bool)gObjGetMagicSearch(lpObj, 522);
		case 508:
			return (bool)gObjGetMagicSearch(lpObj, 514);
		case 512:
			return (bool)gObjGetMagicSearch(lpObj, 516);
		case 518:
			return (bool)gObjGetMagicSearch(lpObj, 520);
		case 521:
			return (bool)gObjGetMagicSearch(lpObj, 524);
		case 77:
			return (bool)gObjGetMagicSearch(lpObj ,441);
		case 260:
			return (bool)gObjGetMagicSearch(lpObj, 551);
		case 261:
			return (bool)gObjGetMagicSearch(lpObj, 552);
		case 551:
			return (bool)gObjGetMagicSearch(lpObj, 554);
		case 552:
			return (bool)gObjGetMagicSearch(lpObj, 555);
		case 262:
			return (bool)gObjGetMagicSearch(lpObj, 558);
		case 263:
			return (bool)gObjGetMagicSearch(lpObj, 559);
		case 264:
			return (bool)gObjGetMagicSearch(lpObj, 560);
		case 267:
			return (bool)gObjGetMagicSearch(lpObj, 573);
		case 268:
			return (bool)gObjGetMagicSearch(lpObj, 569);
		case 569:
			return (bool)gObjGetMagicSearch(lpObj, 572);

	}

	return false;
}

bool CMasterSkillSystemS6::SetSkillInfo(LPOBJ lpObj, unsigned char* buf)
{
	WORD type;
	BYTE level;

	int count = 0;

	for(int i=0;i<120;i++)
	{
		memcpy(&type, &buf[i*4], 2);
		memcpy(&level, &buf[i*4+2], 1);

		if(type == 0xFFFF) continue;

		if(MagicDamageC.GetMasterSkillType(type) >= 3)
		{
			if(MagicDamageC.SkillGetRequireClass(lpObj->Class, lpObj->m_PlayerData->ChangeUP, type) == FALSE)
			{
				g_Log.AddC(0, __FILE__, __FUNCTION__, TColor::Purple,  "[Mastering] WARNING: Invalid skill(%d) for class (%d)(%s)(%s)", type, lpObj->Class, lpObj->AccountID, lpObj->Name);
				continue;
			}

			if(MagicDamageC.GetMasterSkillType(type) == 4)
			{
				if(gObjGetMagicSearch(lpObj, type) == NULL)
				{
					this->gObjMagicAddMLS(lpObj, type, level);
					g_Log.AddC(0, __FILE__, __FUNCTION__, TColor::Purple,  "[Mastering] WARNING: Active skill(%d) not found in character skill -> added to MagicInfo", type);
				}
			}

			lpObj->m_PlayerData->m_MasterSkills[i].SkillMagicId = type;
			lpObj->m_PlayerData->m_MasterSkills[i].SkillLevel = level;
			lpObj->m_PlayerData->m_MasterSkills[i].SkillIndex = this->FindSkillArrayNum(type);
			count++;
		}
	}

	return true;
}

void CMasterSkillSystemS6::ConvertSkillToByte(LPOBJ lpObj, unsigned char* buf)
{
	int n = 0;

	for(int i=0;i<120;i++)
	{
		memcpy(&buf[n], &lpObj->m_PlayerData->m_MasterSkills[i].SkillMagicId, 2);
		n += 2;

		buf[n] = lpObj->m_PlayerData->m_MasterSkills[i].SkillLevel;
		n++;

		buf[n] = 0;
		n++;
	}
}

int CMasterSkillSystemS6::AddSkillInfo(LPOBJ lpObj, WORD skill, BYTE level)
{
	for(int i=0;i<120;i++)
	{
		if(lpObj->m_PlayerData->m_MasterSkills[i].SkillMagicId == 0xFFFF)
		{
			lpObj->m_PlayerData->m_MasterSkills[i].SkillMagicId = skill;
			lpObj->m_PlayerData->m_MasterSkills[i].SkillLevel = level;
			lpObj->m_PlayerData->m_MasterSkills[i].SkillIndex = this->FindSkillArrayNum(skill);
			return i;
		}
	}
	return -1;
}

int CMasterSkillSystemS6::SearchSkill(LPOBJ lpObj, WORD skill)
{
	for(int i=0;i<120;i++)
	{
		if(lpObj->m_PlayerData->m_MasterSkills[i].SkillMagicId == skill)
			return i;
	}
	
	return -1;
}

int CMasterSkillSystemS6::GetSkillLevel(LPOBJ lpObj, WORD skill)
{
	for(int i=0;i<120;i++)
	{
		if(lpObj->m_PlayerData->m_MasterSkills[i].SkillMagicId == skill)
		{
			return lpObj->m_PlayerData->m_MasterSkills[i].SkillLevel;
		}
	}

	return -1;
}

void CMasterSkillSystemS6::DecreasePoint(LPOBJ lpObj, int MasterPoint)
{
	if(lpObj == NULL)
		return;

	lpObj->m_PlayerData->MasterPoint -= MasterPoint;

	if(lpObj->m_PlayerData->MasterPoint < 0)
	{
		lpObj->m_PlayerData->MasterPoint = 0;

		g_Log.AddC(0, __FILE__, __FUNCTION__, TColor::Red,  "[Mastering] (%s)(%s) ERROR: lpObj->MasterPoint < 0 -> reset to 0",
			lpObj->AccountID, lpObj->Name);
	}
}

void CMasterSkillSystemS6::ApplyActiveSkillEffect(LPOBJ lpObj, WORD skill)
{
	if(lpObj->Type != OBJ_USER)
	{
		return;
	}

	if(skill < 300)
	{
		g_Log.AddC(0, __FILE__, __FUNCTION__, TColor::Red,  "[Mastering] (%s)(%s) ERROR: skill is not MASTER", lpObj->AccountID, lpObj->Name);
		return;
	}
}

void CMasterSkillSystemS6::SkillTreeReset(LPOBJ lpObj, BYTE Type)
{
	if(Type == 0)
	{
		return;
	}

	if(Type != 4)
	{
		int MP = 0;
		for(int i=0;i<120;i++)
		{
			if(lpObj->m_PlayerData->m_MasterSkills[i].SkillMagicId != 0xFFFF)
			{
				int skillnum = this->FindSkillArrayNum(lpObj->m_PlayerData->m_MasterSkills[i].SkillMagicId);

				if(skillnum == -1)
					continue;

				if(this->m_SkillInfo[skillnum].btGroupType == Type-1)
				{
					int SkillLevel = lpObj->m_PlayerData->m_MasterSkills[i].SkillLevel;
					if(this->m_SkillInfo[skillnum].btReqPts != 10)
					{
						MP += SkillLevel * this->m_SkillInfo[skillnum].btReqPts;
					}
					else
					{
						MP += this->m_SkillInfo[skillnum].btReqPts;
					}
				}
			}

		}

		for(int i=0;i<120;i++)
		{
			if(lpObj->m_PlayerData->m_MasterSkills[i].SkillMagicId != 0xFFFF)
			{
				int skillnum = this->FindSkillArrayNum(lpObj->m_PlayerData->m_MasterSkills[i].SkillMagicId);

				if(skillnum == -1)
					continue;

				if(this->m_SkillInfo[skillnum].btGroupType != Type-1)
				{
					continue;
				}

				int SkillType = MagicDamageC.GetMasterSkillType(lpObj->m_PlayerData->m_MasterSkills[i].SkillMagicId);

				if(SkillType == 4)
				{
					int OldSkill = MagicDamageC.GetBrandOfSkill(lpObj->m_PlayerData->m_MasterSkills[i].SkillMagicId);
					gObjMagicDel(lpObj, lpObj->m_PlayerData->m_MasterSkills[i].SkillMagicId, 0);
					
					if(OldSkill != 0)
					{
						this->gObjMagicAddMLS(lpObj, OldSkill, 1);
					}
				}
				lpObj->m_PlayerData->m_MasterSkills[i].Reset();
			}
		}

		lpObj->m_PlayerData->MasterPoint += MP;
	}

	else
	{
		for (int i=0;i<120;i++)
		{
			if(lpObj->m_PlayerData->m_MasterSkills[i].SkillMagicId != 0xFFFF)
			{
				int SkillType = MagicDamageC.GetMasterSkillType(lpObj->m_PlayerData->m_MasterSkills[i].SkillMagicId);
				if(SkillType == 4)
				{
					int OldSkill = MagicDamageC.GetBrandOfSkill(lpObj->m_PlayerData->m_MasterSkills[i].SkillMagicId);
					gObjMagicDel(lpObj, lpObj->m_PlayerData->m_MasterSkills[i].SkillMagicId, 0);

					if(OldSkill != 0)
					{
						this->gObjMagicAddMLS(lpObj, OldSkill, 1);
					}
				}
				
				lpObj->m_PlayerData->m_MasterSkills[i].Reset();
			}
		}
		lpObj->m_PlayerData->MasterPoint = lpObj->m_PlayerData->MasterLevel * g_ConfigRead.data.common.MLPointPerLevel;
	}
}

int CMasterSkillSystemS6::MLS_CheckExistSkill(LPOBJ lpObj, WORD Skill)
{
	for(int i=0;i<MAX_MAGIC;i++)
	{
		if ( lpObj->Magic[i].IsMagic() == TRUE )
		{
			if ( MagicDamageC.CheckMasterSkill(lpObj->Magic[i].m_Skill) == TRUE && MagicDamageC.GetBrandOfSkill(lpObj->Magic[i].m_Skill) == Skill )
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

int CMasterSkillSystemS6::gObjMagicAddMLS(LPOBJ lpObj, WORD Skill, WORD Level)
{
	int skillresult = -1;

	if(MagicDamageC.SkillGetRequireClass(lpObj->Class,lpObj->m_PlayerData->ChangeUP,Skill) < 1 && lpObj->Type == OBJ_USER && lpObj->m_PlayerData->ISBOT == false)
	{
		g_Log.Add(0, __FILE__, __FUNCTION__, "error-L2: His class is not allowed to use the skill [ID: %d]", skill);
		return -1;
	}

	for(int n = 0; n < MAGIC_SIZE; n++)
	{
		if(lpObj->Magic[n].IsMagic() == 1)
		{
			if(lpObj->Magic[n].m_Skill == Skill)
			{
				g_Log.Add(0, __FILE__, __FUNCTION__, "Same skill already exists. %d %d", lpObj->Magic[n].m_Skill,Skill);
				return -1;
			}
		}
	}

	for(int n = 0; n < MAGIC_SIZE; n++)
	{
		if(lpObj->Magic[n].IsMagic() == 0)
		{
			skillresult = lpObj->Magic[n].Set(Skill,Level);
			if(skillresult < 0)
			{
				g_Log.Add(0, __FILE__, __FUNCTION__, "Same magic already exists %s %d",__FILE__,__LINE__);
				return -1;
			}
			else
			{
				lpObj->MagicCount++;
				return n;
			}
		}
	}

	g_Log.Add(0, __FILE__, __FUNCTION__, "No space to add magic. %s %d",__FILE__,__LINE__);
	return -1;
}

int CMasterSkillSystemS6::CalDistance_MLS(int x, int y, int x1, int y1)
{
	if(x == x1 && y == y1)
	{
		return 0;
	}

	float tx = x - x1;
	float ty = y - y1;

	return sqrt(tx*tx + ty*ty);
}

BOOL CMasterSkillSystemS6::UseMasterSkill(int aIndex, CMagicInf * lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos, int aTargetIndex)
{
	if ( ObjectMaxRange(aIndex) == FALSE )
	{
		return FALSE;
	}

	if ( lpMagic == NULL )
	{
		return FALSE;
	}

	if ( MagicDamageC.CheckMasterSkill(lpMagic->m_Skill) == FALSE )
	{
		return FALSE;
	}

	switch ( lpMagic->m_Skill )
	{
		case MST_IRONDEFENSE:
			this->MLS_SkillIronDefense(aIndex, lpMagic, aTargetIndex);
			break;
	}
}

BOOL CMasterSkillSystemS6::MLS_SkillIronDefense(int aIndex, CMagicInf *lpMagic, int aTargetIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if ( lpObj->Type != OBJ_USER )
	{
		return FALSE;
	}

	if ( gObjIsConnected(lpObj) == FALSE )
	{
		return FALSE;
	}

	float AddStat = gObjUseSkill.m_SkillData.IronDefenseAddStat;

	gObjAddBuffEffect(lpObj, BUFFTYPE_IRON_DEFENSE, EFFECTTYPE_IMPROVE_DEFENSE, AddStat, EFFECTTYPE_HP, AddStat, 10);

	GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aIndex, TRUE);

	return TRUE;
}

BOOL CMasterSkillSystemS6::MLS_SkillCycloneStr(int aIndex, CMagicInf* lpMagic, int aTargetIndex, BOOL bCombo)
{
	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj = &gObj[aTargetIndex];

	if ( ObjectMaxRange(aTargetIndex) == FALSE )
	{
		return FALSE;
	}

	if ( lpObj->Type != OBJ_USER )
	{
		return FALSE;
	}

	if ( lpObj->Class != CLASS_KNIGHT )
	{
		return FALSE;
	}

	gObjAttack(lpObj, lpTargetObj, lpMagic, FALSE, TRUE, 0, bCombo, 0, 0);

	if ( bCombo )
	{
		GSProtocol.GCMagicAttackNumberSend(lpObj, AT_SKILL_COMBO, aTargetIndex, TRUE);
	}
}

BOOL CMasterSkillSystemS6::MLS_SkillSlashStr(int aIndex, CMagicInf* lpMagic, int aTargetIndex, BOOL bCombo)
{
	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj = &gObj[aTargetIndex];

	if ( ObjectMaxRange(aTargetIndex) == FALSE )
	{
		return FALSE;
	}

	if ( lpObj->Type != OBJ_USER )
	{
		return FALSE;
	}

	if ( lpObj->Class != CLASS_KNIGHT )
	{
		return FALSE;
	}

	gObjAttack(lpObj, lpTargetObj, lpMagic, FALSE, TRUE, 0, bCombo, 0, 0);

	if ( bCombo )
	{
		GSProtocol.GCMagicAttackNumberSend(lpObj, AT_SKILL_COMBO, aTargetIndex, TRUE);
	}
}

BOOL CMasterSkillSystemS6::MLS_SkillFallingSlashStr(int aIndex, CMagicInf* lpMagic, int aTargetIndex, BOOL bCombo)
{
	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj = &gObj[aTargetIndex];

	if ( ObjectMaxRange(aTargetIndex) == FALSE )
	{
		return FALSE;
	}

	if ( lpObj->Type != OBJ_USER )
	{
		return FALSE;
	}

	if ( lpObj->Class != CLASS_KNIGHT )
	{
		return FALSE;
	}

	gObjAttack(lpObj, lpTargetObj, lpMagic, FALSE, TRUE, 0, bCombo, 0, 0);

	if ( bCombo )
	{
		GSProtocol.GCMagicAttackNumberSend(lpObj, AT_SKILL_COMBO, aTargetIndex, TRUE);
	}
}

BOOL CMasterSkillSystemS6::MLS_SkillLungeStr(int aIndex, CMagicInf* lpMagic, int aTargetIndex, BOOL bCombo)
{
	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj = &gObj[aTargetIndex];

	if ( ObjectMaxRange(aTargetIndex) == FALSE )
	{
		return FALSE;
	}

	if ( lpObj->Type != OBJ_USER )
	{
		return FALSE;
	}

	if ( lpObj->Class != CLASS_KNIGHT )
	{
		return FALSE;
	}

	gObjAttack(lpObj, lpTargetObj, lpMagic, FALSE, TRUE, 0, bCombo, 0, 0);

	if ( bCombo )
	{
		GSProtocol.GCMagicAttackNumberSend(lpObj, AT_SKILL_COMBO, aTargetIndex, TRUE);
	}
}

BOOL CMasterSkillSystemS6::MLS_SkillWheel(int aIndex, CMagicInf* lpMagic, int aTargetIndex, BOOL bCombo)
{
	LPOBJ lpObj = &gObj[aIndex];
	int tObjNum;
	int count = 0;
	int HitCount = 0;
	int bAttack;
	int DuelIndex = lpObj->m_iDuelUser;
	int EnableAttack;

	while ( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(tObjNum >= 0)
			{
				EnableAttack = 0;

				if(lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
				{
					EnableAttack = 1;
				}
				else if(tObjNum == aTargetIndex || DuelIndex == tObjNum)
				{
					EnableAttack = 1;
				}
#ifdef GS_CASTLE
				else if(lpObj->VpPlayer2[count].type != OBJ_NPC && gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE && g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
				{
					EnableAttack = 1;
				}
#endif
				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if(gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0)
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if(gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex])==1)
					{
						EnableAttack = 1;
					}
				}

				if(EnableAttack != 0)
				{
					int Distance = MagicDamageC.GetSkillDistance(lpMagic->m_Skill);

					if ( gObjCalDistance(lpObj, &gObj[tObjNum]) <= Distance )
					{
						gObjAttack(lpObj,&gObj[tObjNum],lpMagic,0,1,0,bCombo,0,0);

						if(bCombo != 0)
						{
							GSProtocol.GCMagicAttackNumberSend(lpObj,AT_SKILL_COMBO,gObj[tObjNum].m_Index,1);
						}
					}
				}
			}
		}

		count++;
		if(count > MAX_VIEWPORT -1)
		{
			break;
		}
	}

	return TRUE;
}

BOOL CMasterSkillSystemS6::MLS_SkillBlowOfFury(int aIndex, CMagicInf * lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos, int aTargetIndex, BOOL isCombo)
{
	LPOBJ lpObj = &gObj[aIndex];
	int tObjNum;
	int count = 0;
	int DuelIndex = lpObj->m_iDuelUser;
	int HitCount = 0;
	int bAttack = 0;
	int EnableAttack;

	while ( true )
	{
		if(lpObj->VpPlayer2[count].state != 0)
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if(tObjNum >= 0)
			{
				EnableAttack = 0;

				if(lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
				{
					EnableAttack = 1;
				}
				else if(tObjNum == aTargetIndex || DuelIndex == tObjNum)
				{
					EnableAttack = 1;
				}
#ifdef GS_CASTLE
				else if(lpObj->VpPlayer2[count].type != OBJ_NPC && gObj[tObjNum].MapNumber == MAP_INDEX_CASTLESIEGE && g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
				{
					EnableAttack = 1;
				}
#endif
				else
				{
					int CallMonIndex = gObj[tObjNum].m_Index;

					if(gObj[tObjNum].Type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon >= 0)
					{
						CallMonIndex = gObj[gObj[tObjNum].m_RecallMon].m_Index;
					}

					if(gObjTargetGuildWarCheck(lpObj,&gObj[CallMonIndex])==1)
					{
						EnableAttack = 1;
					}
				}

				if(EnableAttack != 0)
				{
					int Distance = MagicDamageC.GetSkillDistance(lpMagic->m_Skill);

					if ( gObjCalDistance(lpObj, &gObj[tObjNum]) <= Distance )
					{
						gObjAddAttackProcMsgSendDelay(lpObj,50,tObjNum,500,lpMagic->m_Skill,isCombo);
						
						if(isCombo != 0)
						{
							GSProtocol.GCMagicAttackNumberSend(lpObj,AT_SKILL_COMBO,aIndex,1);
						}					
					}
				}
			}
		}

		count++;
		if(count > MAX_VIEWPORT -1)
		{
			break;
		}
	}

	return TRUE;
}

BOOL CMasterSkillSystemS6::MLS_SkillSwordOfDestructionStr(int aIndex, CMagicInf * lpMagic, BYTE X, BYTE Y, int aTargetIndex, BOOL bCombo)
{
	if(ObjectMaxRange(aIndex) == 0)
	{
	    return 0;
	}

	LPOBJ lpObj = &gObj[aIndex];
	BOOL EnableAttack;

    for(int n=0;n < MAX_VIEWPORT;n++)
	{
        if(lpObj->VpPlayer2[n].state <= 0)
		{
		    continue;
		}
		EnableAttack = FALSE;
		int index = lpObj->VpPlayer2[n].number;

		if(gObj[index].Type == OBJ_MONSTER && gObj[aIndex].m_RecallMon < 0)
		{
			EnableAttack = TRUE;
		}

		else if(aTargetIndex == index || index == lpObj->m_iDuelUser)
		{
			EnableAttack = TRUE;
		}

#ifdef GS_CASTLE
		else if(gObj[index].Type != OBJ_NPC &&
			gObj[index].MapNumber == MAP_INDEX_CASTLESIEGE &&
			g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE )
		{
			EnableAttack = TRUE;
		}
#endif

		int summon = aIndex;

		if(gObj[aIndex].Type == OBJ_MONSTER && gObj[aIndex].m_RecallMon >= 0)
		{
			summon = gObj[aIndex].m_RecallMon;
		}

		if(gObjTargetGuildWarCheck(&gObj[summon],lpObj) != 0)
		{
			EnableAttack = TRUE;
		}

		if(this->CalDistance_MLS(X,Y,gObj[index].X,gObj[index].Y) < 3 && EnableAttack)
		{
            gObjAddAttackProcMsgSendDelay(lpObj,50,index,400,lpMagic->m_Skill,bCombo);

			if(bCombo != 0)
			{
		        GSProtocol.GCMagicAttackNumberSend(lpObj,59,aIndex,1);
			}
		}
	}

	return 1;
}

BOOL CMasterSkillSystemS6::MLS_CheckWeaponSkill(LPOBJ lpObj, WORD Skill, WORD Level)
{
	for ( int i=0;i<MAX_MAGIC;i++ )
	{
		if ( lpObj->Magic[i].IsMagic() == TRUE && MagicDamageC.CheckMasterSkill(lpObj->Magic[i].m_Skill) == TRUE )
		{
			WORD CheckSkill = MagicDamageC.GetBrandOfSkill(lpObj->Magic[i].m_Skill);

			if ( CheckSkill == Skill )
			{
				return FALSE;
			}
		}
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

