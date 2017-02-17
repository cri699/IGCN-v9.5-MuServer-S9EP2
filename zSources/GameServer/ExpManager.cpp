//////////////////////////////////////////////////////////////////////
// ExpManager.cpp
#include "StdAfx.h"
#include "ExpManager.h"
#include "TLog.h"
#include "user.h"
#include "MasterLevelSkillTreeSystem.h"

CExpManager g_ExpManager;

CExpManager::CExpManager()
{
	this->m_bDebugMode = false;
	this->m_iExpCalcType = EXP_CALC_STATIC_ONLY;
	this->m_fStaticExp = 1.0;
	this->m_fStaticMLExp = 1.0;
	this->m_fEventExp = 0.0; 
	this->m_vtExpRanges.clear();
}

CExpManager::~CExpManager()
{

}

void CExpManager::LoadScript(LPSTR szFileName)
{
	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(szFileName);

	if (res.status != pugi::status_ok)
	{
		g_Log.MsgBox("[Exp Manager] Cannot load %s file (%s)", szFileName, res.description());
		return;
	}

	pugi::xml_node main = file.child("ExpSystem");

	this->m_iExpCalcType = main.attribute("CalcType").as_int();
	this->m_bDebugMode = main.attribute("DebugMode").as_bool();

	pugi::xml_node static_exp = main.child("StaticExp");

	this->m_fStaticExp = static_exp.attribute("Normal").as_float();
	this->m_fStaticMLExp = static_exp.attribute("Master").as_float();
	this->m_fEventExp = static_exp.attribute("Event").as_float();
	this->m_fQuestExp = static_exp.attribute("Quest").as_float();

	pugi::xml_node dynamic_exp = main.child("DynamicExpRangeList");
	this->m_vtExpRanges.clear();

	for (pugi::xml_node range = dynamic_exp.child("Range"); range; range = range.next_sibling())
	{
		EXP_RANGES m_Range;

		m_Range.iMinReset = range.attribute("MinReset").as_int();
		m_Range.iMaxReset = range.attribute("MaxReset").as_int();
		m_Range.iMinLevel = range.attribute("MinLevel").as_int();
		m_Range.iMaxLevel = range.attribute("MaxLevel").as_int();
		m_Range.fExp = range.attribute("NormalExp").as_float();
		m_Range.fMLExp = range.attribute("MasterExp").as_float();

		this->m_vtExpRanges.push_back(m_Range);
	}

	g_Log.Add("[Exp Manager] Loaded Data (Static Exp:%f Static MLExp:%f EventExp:%f QuestExp:%f), Count of Dynamic Exp ranges: %d, CalcType:%d, Debug:%d",
		this->m_fStaticExp, this->m_fStaticMLExp, this->m_fEventExp, this->m_fQuestExp, this->m_vtExpRanges.size(), this->m_iExpCalcType, this->m_bDebugMode);
}

float CExpManager::GetExpMultiplier(int iLevel, int iReset, bool bIsMasterLevel)
{
	float fExp = 0.0;

	if (this->m_iExpCalcType == EXP_CALC_STATIC_ONLY)
	{
		if (bIsMasterLevel)
		{
			fExp = this->m_fStaticMLExp;
		}

		else
		{
			fExp = this->m_fStaticExp;
		}

		fExp += this->m_fEventExp;
	}

	else
	{
		for (std::vector<EXP_RANGES>::iterator It = this->m_vtExpRanges.begin(); It != this->m_vtExpRanges.end(); It++)
		{
			if (iReset >= It->iMinReset && iReset <= It->iMaxReset && iLevel >= It->iMinLevel && iLevel <= It->iMaxLevel)
			{
				if (bIsMasterLevel)
				{
					fExp = It->fMLExp;
				}

				else
				{
					fExp = It->fExp;
				}

				break;
			}
		}

		if (this->m_iExpCalcType == EXP_CALC_STATIC_PLUS_DYNAMIC)
		{
			if (bIsMasterLevel)
			{
				fExp += this->m_fStaticMLExp;
			}

			else
			{
				fExp += this->m_fStaticExp;
			}

			fExp += this->m_fEventExp;
		}

		else if (this->m_iExpCalcType == EXP_CALC_STATIC_MULTIPLE_DYNAMIC)
		{
			if (bIsMasterLevel)
			{
				fExp *= (this->m_fStaticMLExp + this->m_fEventExp);
			}

			else
			{
				fExp *= (this->m_fStaticExp + this->m_fEventExp);
			}
		}
	}

	if (this->m_bDebugMode == true)
	{
		g_Log.AddC(TColor::Yellow, "[EXP DEBUG] Level:%d Reset:%d ML:%d ExpResult:%f (CalcType:%d)", iLevel, iReset, bIsMasterLevel, fExp, this->m_iExpCalcType);
	}

	return fExp;
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

