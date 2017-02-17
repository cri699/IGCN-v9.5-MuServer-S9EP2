//////////////////////////////////////////////////////////////////////
// ImperialGuardianRewardExp.cpp
#include "stdafx.h"
#include "ImperialGuardianRewardExp.h"
#include "TLog.h"
#include "user.h"

CImperialGuardianRewardExp::CImperialGuardianRewardExp(void)
{
	this->Init();
}

CImperialGuardianRewardExp::~CImperialGuardianRewardExp(void)
{
}

void CImperialGuardianRewardExp::Init()
{
	this->m_bLoadScript = false;
	this->m_vtRewardExpInfo.clear();
}

void CImperialGuardianRewardExp::LoadScript(const char* lpFileName)
{
	this->m_bLoadScript = false;
	this->m_vtRewardExpInfo.clear();

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(lpFileName);

	if ( res.status != pugi::status_ok )
	{
		g_Log.Add("%s file load error (%s)", lpFileName, res.description());
		return;
	}

	pugi::xml_node main = file.child("ImperialGuardian");

	_stRewardExpInfo rewardExpInfo;

	for (pugi::xml_node range = main.child("Range"); range; range = range.next_sibling())
	{
		rewardExpInfo.m_nMinLevel = range.attribute("PlayerMinLevel").as_int();
		rewardExpInfo.m_nMaxLevel = range.attribute("PlayerMaxLevel").as_int();
		rewardExpInfo.m_nRewardExp = range.attribute("ExpReward").as_int();

		this->m_vtRewardExpInfo.push_back(rewardExpInfo);
	}

	g_Log.Add("%s file load", lpFileName);
	this->m_bLoadScript = true;
}

int CImperialGuardianRewardExp::GetRewardExp(int nUserIndex)
{
	if ( ObjectMaxRange(nUserIndex) == false )
	{
		g_Log.Add("error : %s %d", __FILE__, __LINE__);
		return 0;
	}

	if ( gObj[nUserIndex].Type != OBJ_USER )
	{
		g_Log.Add("error : %s %d", __FILE__, __LINE__);
		return 0;
	}

	int nSize = this->m_vtRewardExpInfo.size();
	LPOBJ lpObj = &gObj[nUserIndex];
	int nUserLevel = lpObj->Level + lpObj->m_PlayerData->MasterLevel;

	for ( int i=0;i<nSize;i++ )
	{
		_stRewardExpInfo rewardExpInfo = this->m_vtRewardExpInfo.at(i);

		if ( nUserLevel >= rewardExpInfo.m_nMinLevel && nUserLevel <= rewardExpInfo.m_nMaxLevel )
		{
			return rewardExpInfo.m_nRewardExp;
		}
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

