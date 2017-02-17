// CrywolfAltar.cpp: implementation of the CCrywolfAltar class.
//	GS-N	1.00.18	JPN	0x00565f30	-	Completed
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CrywolfAltar.h"
#include "CrywolfUtil.h"
#include "TLog.h"
#include "BuffEffectSlot.h"
#include "configread.h"

CCrywolfAltar g_CrywolfNPC_Altar;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCrywolfAltar::CCrywolfAltar()
{
	return;
}

CCrywolfAltar::~CCrywolfAltar()
{
	return;
}


int CCrywolfAltar::GetAltarState(int iClass)
{
	int iAltarState = 0;

	switch ( iClass )
	{
		case 205:
			iAltarState = this->m_AltarInfo[0].GetAltarState();
			break;
		case 206:
			iAltarState = this->m_AltarInfo[1].GetAltarState();
			break;
		case 207:
			iAltarState = this->m_AltarInfo[2].GetAltarState();
			break;
		case 208:
			iAltarState = this->m_AltarInfo[3].GetAltarState();
			break;
		case 209:
			iAltarState = this->m_AltarInfo[4].GetAltarState();
			break;
	}

	return iAltarState;
}



int CCrywolfAltar::GetAltarRemainContractCount(int iClass)
{
	int iAltarRemainContractCount = 0;

	switch ( iClass )
	{
		case 205:
			iAltarRemainContractCount = this->m_AltarInfo[0].GetRemainContractCount();
			break;
		case 206:
			iAltarRemainContractCount = this->m_AltarInfo[1].GetRemainContractCount();
			break;
		case 207:
			iAltarRemainContractCount = this->m_AltarInfo[2].GetRemainContractCount();
			break;
		case 208:
			iAltarRemainContractCount = this->m_AltarInfo[3].GetRemainContractCount();
			break;
		case 209:
			iAltarRemainContractCount = this->m_AltarInfo[4].GetRemainContractCount();
			break;
	}

	return iAltarRemainContractCount;
}



int CCrywolfAltar::GetAlatarNumber(int iClass)
{
	int iAltarNumber = 0;

	switch ( iClass )
	{
		case 205:
			iAltarNumber = 0;
			break;
		case 206:
			iAltarNumber = 1;
			break;
		case 207:
			iAltarNumber = 2;
			break;
		case 208:
			iAltarNumber = 3;
			break;
		case 209:
			iAltarNumber = 4;
			break;
	}

	return iAltarNumber;
}



int CCrywolfAltar::GetAltarUserIndex(int iClass)
{
	int iUserIndex = -1;

	switch ( iClass )
	{
		case 205:
			iUserIndex = this->m_AltarInfo[0].m_iUserIndex;
			break;
		case 206:
			iUserIndex = this->m_AltarInfo[1].m_iUserIndex;
			break;
		case 207:
			iUserIndex = this->m_AltarInfo[2].m_iUserIndex;
			break;
		case 208:
			iUserIndex = this->m_AltarInfo[3].m_iUserIndex;
			break;
		case 209:
			iUserIndex = this->m_AltarInfo[4].m_iUserIndex;
			break;
	}

	return iUserIndex;
}


void CCrywolfAltar::SetAltarValidContract(int iAltarObjIndex, int iClass)
{
	int iAltarNumber = this->GetAlatarNumber(iClass);
	CCrywolfAltarInfo * AltarInfo = &this->m_AltarInfo[iAltarNumber];

	if ( AltarInfo->GetAltarState() != 2 )
		return;

	if ( AltarInfo->m_dwValidContractTime < GetTickCount() )
	{
		MsgOutput(AltarInfo->m_iUserIndex, Lang.GetText(0,243), iAltarNumber);
		UTIL.SendCrywolfUserAnyMsg(2, Lang.GetText(0,244), gObj[AltarInfo->m_iUserIndex].Name, iAltarNumber);

		g_Log.Add("[Crywolf][Altar op.]  [%s][%s] Set Valid Contract Altar[%d]",
			gObj[AltarInfo->m_iUserIndex].AccountID, gObj[AltarInfo->m_iUserIndex].Name, iAltarNumber);

		AltarInfo->SetAltarState(1);

		gObjAddBuffEffect(&gObj[iAltarObjIndex], 23, 0, 0, 0, 0, -10);
	}
}


BOOL CCrywolfAltar::CheckAltarValidContract(int iClass)
{
	int iAltarNumber = this->GetAlatarNumber(iClass);
	CCrywolfAltarInfo * AltarInfo = &this->m_AltarInfo[iAltarNumber];

	if ( AltarInfo->GetAltarState() == 1 )
		return TRUE;

	return FALSE;
}


BOOL CCrywolfAltar::SetAltarUserIndex(int iAltarObjIndex, int iClass, int iUserIndex)
{
	int iAltarNumber = this->GetAlatarNumber(iClass);
	CCrywolfAltarInfo * AltarInfo = &this->m_AltarInfo[iAltarNumber];
	DWORD dwCurrentTime = GetTickCount();

	if ( gObjIsConnected(AltarInfo->m_iUserIndex) != FALSE )
	{
		MsgOutput(iUserIndex, Lang.GetText(0,245), iAltarNumber);
		return FALSE;
	}

	if ( (dwCurrentTime-AltarInfo->m_dwLastValidContractTime) < 10000 )
	{
		MsgOutput(iUserIndex, Lang.GetText(0,246), iAltarNumber);
		return FALSE;
	}

	if ( AltarInfo->m_iContractCount > MAX_ALTAR_CONTRACTS ) 
	{
		MsgOutput(iUserIndex, Lang.GetText(0,247), iAltarNumber);
		return FALSE;
	}

	if ( abs(gObj[iAltarObjIndex].Y - gObj[iUserIndex].Y) > 0 || abs(gObj[iAltarObjIndex].X - gObj[iUserIndex].X) > 0 )
	{
		MsgOutput(iUserIndex, Lang.GetText(0,248), iAltarNumber);
		return FALSE;
	}

	AltarInfo->SetAltarState(2);
	AltarInfo->m_iUserIndex = iUserIndex;
	AltarInfo->m_dwAppliedContractTime = dwCurrentTime;
	AltarInfo->m_dwValidContractTime = dwCurrentTime + 5000;	// It Get Active after 5 Seconds
	AltarInfo->m_dwLastValidContractTime = dwCurrentTime;
	AltarInfo->m_iContractCount++;
	gObjAddBuffEffect(&gObj[iAltarObjIndex], 24, 0, 0, 0, 0, -10);

	UTIL.SendCrywolfUserAnyMsg(2, Lang.GetText(0,249), gObj[AltarInfo->m_iUserIndex].Name, iAltarNumber);

	g_Log.Add("[Crywolf][Altar Op.] [%s][%s] Attempt to contract Altar[%d]",
		gObj[AltarInfo->m_iUserIndex].AccountID, gObj[AltarInfo->m_iUserIndex].Name,
		iAltarNumber);

	MsgOutput(iUserIndex, Lang.GetText(0,250));
	MsgOutput(iUserIndex, Lang.GetText(0,251));

	return TRUE;
}
	

void CCrywolfAltar::ResetAltarUserIndex(int iAltarObjIndex, int iClass)
{
	int iAltarNumber = this->GetAlatarNumber(iClass);
	CCrywolfAltarInfo * AltarInfo = &this->m_AltarInfo[iAltarNumber];

	if ( gObjIsConnected(AltarInfo->m_iUserIndex) != FALSE )
	{
		MsgOutput(AltarInfo->m_iUserIndex, Lang.GetText(0,252), iAltarNumber);
		g_Log.Add("[Crywolf][Altar Op.] [%s][%s] Remove contract Altar[%d]",
		gObj[AltarInfo->m_iUserIndex].AccountID, gObj[AltarInfo->m_iUserIndex].Name,
		iAltarNumber);
	}

	if ( AltarInfo->m_iContractCount >= MAX_ALTAR_CONTRACTS )
	{
		AltarInfo->SetAltarState(3);
		gObjAddBuffEffect(&gObj[iAltarObjIndex], 22, 0, 0, 0, 0, -10);
	}
	else
	{
		gObjAddBuffEffect(&gObj[iAltarObjIndex], 21, 0, 0, 0, 0, -10);
		AltarInfo->SetAltarState(0);
	}

	AltarInfo->m_iUserIndex = -1;
	AltarInfo->m_dwAppliedContractTime = 0;
	AltarInfo->m_dwValidContractTime = 0;
	AltarInfo->m_dwLastValidContractTime = GetTickCount();
}


void CCrywolfAltar::SetAltarObjIndex(int iClass, int iObjIndex)
{
	int iAltarNumber = this->GetAlatarNumber(iClass);
	this->ResetAltar(iClass);
	CCrywolfAltarInfo * AltarInfo = &this->m_AltarInfo[iAltarNumber];
	AltarInfo->SetAltarIndex(iObjIndex);
}


void CCrywolfAltar::SetAllAltarObjectIndex()
{
	for ( int i=0;i<g_ConfigRead.server.GetObjectMaxMonster();i++)
	{
		if ( !gObjIsConnected(i) )
			continue;

		if ( gObj[i].MapNumber != MAP_INDEX_CRYWOLF_FIRSTZONE )
			continue;

		if ( CRYWOLF_ALTAR_CLASS_RANGE(gObj[i].Class ) != FALSE )
			g_CrywolfNPC_Altar.SetAltarObjIndex(gObj[i].Class, i);
	}
}

void CCrywolfAltar::ResetAllAltar()
{
	this->ResetAltar(205);
	this->ResetAltar(206);
	this->ResetAltar(207);
	this->ResetAltar(208);
	this->ResetAltar(209);
}


void CCrywolfAltar::ResetAltar(int iClass)
{
	int iAltarNumber = this->GetAlatarNumber(iClass);
	CCrywolfAltarInfo * AltarInfo = &this->m_AltarInfo[iAltarNumber];

	if ( AltarInfo->m_iAltarIndex == -1 )
		return;

	gObjClearBuffEffect(&gObj[AltarInfo->m_iAltarIndex], CLEAR_TYPE_LOGOUT);
	AltarInfo->Reset();
}


void CCrywolfAltar::SetAllAltarViewState(int iAltarState)
{
	this->SetAltarViewState(205, iAltarState);
	this->SetAltarViewState(206, iAltarState);
	this->SetAltarViewState(207, iAltarState);
	this->SetAltarViewState(208, iAltarState);
	this->SetAltarViewState(209, iAltarState);
}


void CCrywolfAltar::SetAltarViewState(int iClass, int iAltarState)
{
	int iAltarNumber = this->GetAlatarNumber(iClass);
	CCrywolfAltarInfo * AltarInfo = &this->m_AltarInfo[iAltarNumber];

	if ( AltarInfo->m_iAltarIndex == -1 )
		return;

	gObjAddBuffEffect(&gObj[AltarInfo->m_iAltarIndex], 21, 0, 0, 0, 0, -10);
}

int CCrywolfAltar::GetContractedAltarCount()
{
	int iContractedAltarCount = 0;

	for ( int i=0;i<MAX_CRYWOLF_ALTAR;i++)
	{
		CCrywolfAltarInfo * AltarInfo = &this->m_AltarInfo[i];

		if ( AltarInfo->GetAltarState() != 1 )
			continue;

		iContractedAltarCount++;
	}

	return iContractedAltarCount;
}


int CCrywolfAltar::GetPriestHPSum()
{
	int iPriestHPSum = 0;

	for ( int i=0;i<MAX_CRYWOLF_ALTAR;i++)
	{
		CCrywolfAltarInfo * AltarInfo = &this->m_AltarInfo[i];

		if ( AltarInfo->GetAltarState() != 1 )
			continue;

		if ( !gObjIsConnected(AltarInfo->m_iUserIndex))
			continue;

		LPOBJ lpUserObj = &gObj[AltarInfo->m_iUserIndex];
		iPriestHPSum += lpUserObj->Life;
		iPriestHPSum += lpUserObj->AddLife;
	}

	return iPriestHPSum;
}

int CCrywolfAltar::GetPriestMaxHPSum()
{
	int iPriestHPSum = 0;

	for ( int i=0;i<MAX_CRYWOLF_ALTAR;i++)
	{
		CCrywolfAltarInfo * AltarInfo = &this->m_AltarInfo[i];

		if ( AltarInfo->GetAltarState() != 1 )
			continue;

		if ( !gObjIsConnected(AltarInfo->m_iUserIndex))
			continue;

		LPOBJ lpUserObj = &gObj[AltarInfo->m_iUserIndex];
		iPriestHPSum += lpUserObj->AddLife + lpUserObj->MaxLife;
		int iOption = 0;
		gObjGetValueOfBuffIndex(lpUserObj, BUFFTYPE_HP_INC, &iOption, 0);
		iPriestHPSum += iOption;
	}

	return iPriestHPSum;
}


void CCrywolfAltar::CrywolfAltarAct(int iIndex)
{
	if ( !gObjIsConnected(iIndex))
		return;

	if ( gObj[iIndex].Type != OBJ_NPC || CRYWOLF_ALTAR_CLASS_RANGE(gObj[iIndex].Class) == FALSE )
		return;

	int iUserIndex = this->GetAltarUserIndex(gObj[iIndex].Class);

	if ( gObjIsConnected(iUserIndex) == FALSE )
	{
		if ( iUserIndex != -1 )
		{
			this->ResetAltarUserIndex(iIndex, gObj[iIndex].Class);
		}

		return;
	}

	if ( gObjIsConnected(&gObj[iUserIndex]) == FALSE )
	{
		this->ResetAltarUserIndex(iIndex, gObj[iIndex].Class);
		return;
	}

	LPOBJ lpObj = &gObj[iIndex];
	LPOBJ lpUserObj = &gObj[iUserIndex];

	if ( lpUserObj->Live != FALSE && lpUserObj->MapNumber == MAP_INDEX_CRYWOLF_FIRSTZONE )
	{
		if ( abs(lpObj->Y-lpUserObj->Y) <= 0 && abs(lpObj->X - lpUserObj->X) <= 0 )
		{
			if ( this->CheckAltarValidContract(gObj[iIndex].Class) != FALSE )
			{

			}
			else
			{
				this->SetAltarValidContract(iIndex, gObj[iIndex].Class);
			}
		}
		else
		{
			this->ResetAltarUserIndex(iIndex, gObj[iIndex].Class);
		}
	}
	else
	{
		this->ResetAltarUserIndex(iIndex, gObj[iIndex].Class);
	}
}



//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

