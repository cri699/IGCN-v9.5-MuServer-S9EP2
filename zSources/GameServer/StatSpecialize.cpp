//////////////////////////////////////////////////////////////////////
// StatSpecialize.cpp
#include "stdafx.h"
#include "StatSpecialize.h"
#include "TLog.h"
#include "configread.h"
#include "winutil.h"
#include "GameMain.h"

CStatSpecialize g_StatSpec;

CStatSpecialize::CStatSpecialize(void) : m_Lua(true)
{
	memset(this->m_StatOptions, 0x00, sizeof(this->m_StatOptions));
}

CStatSpecialize::~CStatSpecialize(void)
{
}

bool CStatSpecialize::LoadFile(char *szFile)
{
	this->m_bFileLoad = false;

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(szFile);

	if (res.status != pugi::status_ok)
	{
		g_Log.MsgBox("Failed to load %s file (%s)", szFile, res.description());
		return false;
	}

	pugi::xml_node main = file.child("StatSpecialize");
	int Count = 0;

	for(pugi::xml_node option = main.child("Option"); option; option = option.next_sibling())
	{
		this->m_StatOptions[Count].StatOptionID = option.attribute("ID").as_int();
		this->m_StatOptions[Count].StatOptionClass[CLASS_KNIGHT] = option.attribute("DK").as_bool();
		this->m_StatOptions[Count].StatOptionClass[CLASS_WIZARD] = option.attribute("DW").as_bool();
		this->m_StatOptions[Count].StatOptionClass[CLASS_ELF] = option.attribute("Elf").as_bool();
		this->m_StatOptions[Count].StatOptionClass[CLASS_MAGUMSA] = option.attribute("MG").as_bool();
		this->m_StatOptions[Count].StatOptionClass[CLASS_DARKLORD] = option.attribute("DL").as_bool();
		this->m_StatOptions[Count].StatOptionClass[CLASS_SUMMONER] = option.attribute("SU").as_bool();
		this->m_StatOptions[Count].StatOptionClass[CLASS_RAGEFIGHTER] = option.attribute("RF").as_bool();
		this->m_StatOptions[Count].StatOptionClass[CLASS_GROWLANCER] = option.attribute("GL").as_bool();
		Count++;

		if (Count >= MAX_STAT_OPTIONS)
		{
			break;
		}
	}

	this->m_Lua.DoFile(g_ConfigRead.GetPath("\\Scripts\\Specialization\\StatSpec.lua"));

	this->m_bFileLoad = true;
	return true;
}

void CStatSpecialize::ClearUserOption(OBJECTSTRUCT *lpObj)
{
	for (int i = 0; i < MAX_USER_STAT_OPTIONS; i++)
	{
		lpObj->m_PlayerData->m_StatSpecOption[i].Clear();
	}
}

void CStatSpecialize::CalcStatOption(LPOBJ lpObj, int StatOptionID)
{
	if (this->m_bFileLoad == false)
	{
		return;
	}

	double Percent = 0.0;
	this->m_Lua.Generic_Call("StatSpec_GetPercent", "iiiiiii>d", 
		(int)lpObj->Class, StatOptionID, (int)lpObj->m_PlayerData->Strength, (int)lpObj->m_PlayerData->Dexterity,
		(int)lpObj->m_PlayerData->Vitality, (int)lpObj->m_PlayerData->Energy, (int)lpObj->Leadership, &Percent);

	if (Percent == 0.0)
	{
		return;
	}

	this->SetStatOption(lpObj, StatOptionID, Percent);
}

void CStatSpecialize::SetStatOption(LPOBJ lpObj, int StatOptionID, double StatOptionPercent)
{
	int ArrayIndex = this->GetUserArrayIndex(lpObj);

	if (ArrayIndex == -1)
	{
		return;
	}

	lpObj->m_PlayerData->m_StatSpecOption[ArrayIndex].StatOptionID = StatOptionID;

	switch ( StatOptionID )
	{
		case STAT_OPTION_INC_ATTACK_POWER:
			{
				int iEffectValueMin = lpObj->m_AttackDamageMinLeft * StatOptionPercent / 100.0;
				int iEffectValueMax = lpObj->m_AttackDamageMaxLeft * StatOptionPercent / 100.0;
				lpObj->m_AttackDamageMinLeft += iEffectValueMin;
				lpObj->m_AttackDamageMinRight += iEffectValueMin;
				lpObj->m_AttackDamageMaxLeft += iEffectValueMax;
				lpObj->m_AttackDamageMaxRight += iEffectValueMax;
				lpObj->m_PlayerData->m_StatSpecOption[ArrayIndex].StatOptionValMin = iEffectValueMin;
				lpObj->m_PlayerData->m_StatSpecOption[ArrayIndex].StatOptionValMax = iEffectValueMax;
			}
			break;

		case STAT_OPTION_INC_ATTACK_RATE:
			{
				int iEffectValue = lpObj->m_PlayerData->m_AttackRatePvM * StatOptionPercent / 100.0;
				lpObj->m_PlayerData->m_AttackRatePvM += iEffectValue;
				lpObj->m_PlayerData->m_StatSpecOption[ArrayIndex].StatOptionValMin = iEffectValue;
			}
			break;

		case STAT_OPTION_INC_ATTACK_RATE_PVP:
			{
				int iEffectValue = lpObj->m_PlayerData->m_AttackRatePvP * StatOptionPercent / 100.0;
				lpObj->m_PlayerData->m_AttackRatePvP += iEffectValue;
				lpObj->m_PlayerData->m_StatSpecOption[ArrayIndex].StatOptionValMin = iEffectValue;
			}
			break;

		case STAT_OPTION_INC_DEFENSE:
			{
				int iEffectValue = lpObj->m_Defense * StatOptionPercent / 100.0;
				lpObj->m_Defense += iEffectValue;
				lpObj->m_PlayerData->m_StatSpecOption[ArrayIndex].StatOptionValMin = iEffectValue;
			}
			break;

		case STAT_OPTION_INC_ATTACK_SPEED:
			{
				int iEffectValue = lpObj->m_AttackSpeed * StatOptionPercent / 100.0;
				lpObj->m_AttackSpeed += iEffectValue;
				lpObj->m_PlayerData->m_StatSpecOption[ArrayIndex].StatOptionValMin = iEffectValue;
			}
			break;

		case STAT_OPTION_INC_DEFENSE_RATE:
			{
				int iEffectValue = lpObj->m_SuccessfulBlocking * StatOptionPercent / 100.0;
				lpObj->m_SuccessfulBlocking += iEffectValue;
				lpObj->m_PlayerData->m_StatSpecOption[ArrayIndex].StatOptionValMin = iEffectValue;
			}
			break;

		case STAT_OPTION_INC_DEFENSE_RATE_PVP:
			{
				int iEffectValue = lpObj->m_PlayerData->m_DefenseRatePvP * StatOptionPercent / 100.0;
				lpObj->m_PlayerData->m_DefenseRatePvP += iEffectValue;
				lpObj->m_PlayerData->m_StatSpecOption[ArrayIndex].StatOptionValMin = iEffectValue;
			}
			break;

		case STAT_OPTION_INC_MAGIC_DAMAGE:
			{
				int iEffectValueMin = lpObj->m_MagicDamageMin * StatOptionPercent / 100.0;
				int iEffectValueMax = lpObj->m_MagicDamageMax * StatOptionPercent / 100.0;
				lpObj->m_MagicDamageMin += iEffectValueMin;
				lpObj->m_MagicDamageMax += iEffectValueMax;
				lpObj->m_PlayerData->m_StatSpecOption[ArrayIndex].StatOptionValMin = iEffectValueMin;
				lpObj->m_PlayerData->m_StatSpecOption[ArrayIndex].StatOptionValMax = iEffectValueMax;
			}
			break;

		case STAT_OPTION_INC_CURSE_DAMAGE:
			{
				int iEffectValueMin = lpObj->m_CurseDamageMin * StatOptionPercent / 100.0;
				int iEffectValueMax = lpObj->m_CurseDamageMax * StatOptionPercent / 100.0;
				lpObj->m_CurseDamageMin += iEffectValueMin;
				lpObj->m_CurseDamageMax += iEffectValueMax;
				lpObj->m_PlayerData->m_StatSpecOption[ArrayIndex].StatOptionValMin = iEffectValueMin;
				lpObj->m_PlayerData->m_StatSpecOption[ArrayIndex].StatOptionValMax = iEffectValueMax;
			}
			break;
	}
}

int CStatSpecialize::GetUserArrayIndex(OBJECTSTRUCT *lpObj)
{
	int ArrayIndex = -1;

	for (int i = 0; i < MAX_USER_STAT_OPTIONS; i++)
	{
		if (lpObj->m_PlayerData->m_StatSpecOption[i].StatOptionID == 0)
		{
			ArrayIndex = i;
			break;
		}
	}

	return ArrayIndex;
}

void CStatSpecialize::SendOptionList(OBJECTSTRUCT *lpObj)
{
	 PMSG_STAT_SPEC_OPTION pMsg;
	 PHeadSubSetB((LPBYTE)&pMsg, 0x59, 0x00, sizeof(pMsg));
	 int OptionCount = 0;

	 for (int i = 0; i < MAX_USER_STAT_OPTIONS; i++)
	 {
		 if(lpObj->m_PlayerData->m_StatSpecOption[i].StatOptionID != 0)
		 {
			 pMsg.m_StatOption[OptionCount].StatOptionID = lpObj->m_PlayerData->m_StatSpecOption[i].StatOptionID;
			 pMsg.m_StatOption[OptionCount].StatOptionValMin = lpObj->m_PlayerData->m_StatSpecOption[i].StatOptionValMin;
			 pMsg.m_StatOption[OptionCount].StatOptionValMax = lpObj->m_PlayerData->m_StatSpecOption[i].StatOptionValMax;
			 OptionCount++;
		 }
	 }

	 IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

