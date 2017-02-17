// CrywolfStatue.cpp: implementation of the CCrywolfStatue class.
//	GS-N	1.00.18	JPN	0x00567640	-	Completed
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CrywolfStatue.h"
#include "CrywolfAltar.h"
#include "CrywolfUtil.h"
#include "user.h"
#include "BuffEffectSlot.h"

CCrywolfStatue g_CrywolfNPC_Statue;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCrywolfStatue::CCrywolfStatue()
{

}

CCrywolfStatue::~CCrywolfStatue()
{

}


int CCrywolfStatue::GetStatueViewState(int iPriestNumber)
{
	int iViewState = 0;

	switch ( iPriestNumber )
	{
		case 0:
			iViewState = 0;
			break;
		case 1:
			iViewState = 1;
			break;
		case 2:
			iViewState = 2;
			break;
		case 3:
			iViewState = 4;
			break;
		case 4:
			iViewState = 8;
			break;
		case 5:
			iViewState = 256;
			break;
	}

	return iViewState;
}

void CCrywolfStatue::SetStatueViewState(LPOBJ lpObj, int iPriestNumber)
{
	switch ( iPriestNumber )
	{
		case 0:
			gObjAddBuffEffect(lpObj, BUFFTYPE_CRYWOLF_STATUE_LV1, 0, 0, 0, 0, -10);
			break;
		case 1:
			gObjAddBuffEffect(lpObj, BUFFTYPE_CRYWOLF_STATUE_LV2, 0, 0, 0, 0, -10);
			break;
		case 2:
			gObjAddBuffEffect(lpObj, BUFFTYPE_CRYWOLF_STATUE_LV3, 0, 0, 0, 0, -10);
			break;
		case 3:
			gObjAddBuffEffect(lpObj, BUFFTYPE_CRYWOLF_STATUE_LV4, 0, 0, 0, 0, -10);
			break;
		case 4:
			gObjAddBuffEffect(lpObj, BUFFTYPE_CRYWOLF_STATUE_LV5, 0, 0, 0, 0, -10);
			break;
	}
}


void CCrywolfStatue::CrywolfStatueAct(int iIndex)
{
	if ( !gObjIsConnected(iIndex))
		return;

	if ( gObj[iIndex].Type != OBJ_NPC || CHECK_CLASS(204, gObj[iIndex].Class) == FALSE )
		return;

	LPOBJ lpObj = &gObj[iIndex];
	int iContractedAlterCount = g_CrywolfNPC_Altar.GetContractedAltarCount();
	int iPriestHPSum = g_CrywolfNPC_Altar.GetPriestHPSum();
	int iPriestMaxHPSum = g_CrywolfNPC_Altar.GetPriestMaxHPSum();

	if ( iContractedAlterCount == 0 || this->m_Shield.m_iShieldHP == 0 )
	{
		if ( this->m_Shield.m_iShieldState == 1 )
		{
			UTIL.SendCrywolfUserAnyMsg(2, Lang.GetText(0,253));
			this->m_Shield.m_iShieldState = 0;
			this->m_Shield.m_iShieldHP = 0;
			this->m_Shield.m_iShieldMaxHP = 0;
			this->m_Shield.m_iPriestNumber = iContractedAlterCount;
			this->SetStatueViewState(lpObj, iContractedAlterCount);
		}
		else if ( this->m_Shield.m_iPriestNumber != iContractedAlterCount )
		{
			this->m_Shield.m_iShieldState = 1;
			this->m_Shield.m_iShieldHP = iPriestHPSum;
			this->m_Shield.m_iShieldMaxHP = iPriestMaxHPSum;
			this->m_Shield.m_iPriestNumber = iContractedAlterCount;
			UTIL.SendCrywolfUserAnyMsg(2, Lang.GetText(0,254), iContractedAlterCount, iPriestHPSum);
			this->SetStatueViewState(lpObj, iContractedAlterCount);
		}

		return;
	}
	else if ( this->m_Shield.m_iShieldState == 0 )
	{
		UTIL.SendCrywolfUserAnyMsg(2, Lang.GetText(0,255));
		this->SetStatueViewState(lpObj, iContractedAlterCount);
	}
	else if ( this->m_Shield.m_iPriestNumber != iContractedAlterCount )
	{
		UTIL.SendCrywolfUserAnyMsg(2, Lang.GetText(0,254), iContractedAlterCount, iPriestHPSum);
		this->SetStatueViewState(lpObj, iContractedAlterCount);
	}

	this->m_Shield.m_iShieldState = 1;
	this->m_Shield.m_iShieldHP = iPriestHPSum;
	this->m_Shield.m_iShieldMaxHP = iPriestMaxHPSum;
	this->m_Shield.m_iPriestNumber = iContractedAlterCount;
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

