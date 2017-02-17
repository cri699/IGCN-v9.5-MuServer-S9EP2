// LifeStone.cpp: implementation of the CLifeStone class.
//	GS-CS	1.00.19	JPN	0x00562650	-	Completed
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LifeStone.h"
#include "CastleSiege.h"
#include "CastleSiegeSync.h"
#include "Gamemain.h"
#include "user.h"
#include "MapClass.h"
#include "TLog.h"
#include "configread.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLifeStone g_CsNPC_LifeStone;

CLifeStone::CLifeStone()
{
	return;
}

CLifeStone::~CLifeStone()
{
	return;
}

//////////////////////////////////////////////////////////////////////
// Member Functions
//////////////////////////////////////////////////////////////////////

int CLifeStone::CreateLifeStone(int iIndex)
{
	LPOBJ lpObj = &gObj[iIndex];
	int iMonsterIndex = -1;
	BYTE cX = lpObj->X;
	BYTE cY = lpObj->Y;

	if ( g_CastleSiegeSync.GetCastleState() != 7 )
	{
		MsgOutput(iIndex, Lang.GetText(0,179));
		return FALSE;
	}

	if ( lpObj->m_PlayerData->GuildStatus != 0x80 )
		return FALSE;

	if ( lpObj->m_btCsJoinSide < 2 )
	{
		MsgOutput(iIndex, Lang.GetText(0,176));
		return FALSE;
	}

	if ( lpObj->m_PlayerData->lpGuild->lpLifeStone  )
	{
		MsgOutput(iIndex, Lang.GetText(0,177));
		return FALSE;
	}

	if ( cX > 150 && cX < 210 && cY > 175 && cY < 230 )
	{
		MsgOutput(iIndex, Lang.GetText(0,178));
		return FALSE;
	}

	BYTE btMapAttr = MapC[lpObj->MapNumber].GetAttr(cX, cY);

	if ( gObj[iIndex].MapNumber != MAP_INDEX_CASTLESIEGE )
	{
		MsgOutput(iIndex, Lang.GetText(0,170));
		return FALSE;
	}

	iMonsterIndex = gObjAddMonster(lpObj->MapNumber);

	if ( iMonsterIndex >= 0 )
	{
		MONSTER_ATTRIBUTE * MAttr = gMAttr.GetAttr(278);

		if ( MAttr == NULL )
		{
			gObjDel(iMonsterIndex);
			return FALSE;
		}

		gObjSetMonster(iMonsterIndex, 278);
		
		gObj[iMonsterIndex].m_PlayerData = new USER_DATA();
		gObj[iMonsterIndex].Live = TRUE;
		gObj[iMonsterIndex].Life = MAttr->m_Hp;
		gObj[iMonsterIndex].MaxLife = MAttr->m_Hp;
		gObj[iMonsterIndex].m_PosNum = -1;
		gObj[iMonsterIndex].X = cX;
		gObj[iMonsterIndex].Y = cY;
		gObj[iMonsterIndex].MTX = cX;
		gObj[iMonsterIndex].MTY = cY;
		gObj[iMonsterIndex].TX = cX;
		gObj[iMonsterIndex].TY = cY;
		gObj[iMonsterIndex].m_OldX = cX;
		gObj[iMonsterIndex].m_OldY = cY;
		gObj[iMonsterIndex].StartX = cX;
		gObj[iMonsterIndex].StartY = cY;
		gObj[iMonsterIndex].MapNumber = lpObj->MapNumber;
		gObj[iMonsterIndex].m_MoveRange = 0;
		gObj[iMonsterIndex].Level = MAttr->m_Level;
		gObj[iMonsterIndex].Type = OBJ_MONSTER;
		gObj[iMonsterIndex].MaxRegenTime = 1000;
		gObj[iMonsterIndex].Dir = 1;
		gObj[iMonsterIndex].RegenTime = GetTickCount();
		gObj[iMonsterIndex].m_Attribute = 0;
		gObj[iMonsterIndex].DieRegen = 0;
		gObj[iMonsterIndex].m_btCsNpcType = OBJ_NPC;
		gObj[iMonsterIndex].m_btCsJoinSide = lpObj->m_btCsJoinSide;
		gObj[iMonsterIndex].m_PlayerData->lpGuild = lpObj->m_PlayerData->lpGuild;
		gObj[iMonsterIndex].m_btCreationState = 0;
		lpObj->m_PlayerData->lpGuild->lpLifeStone = &gObj[iMonsterIndex];

		MsgOutput(iIndex, Lang.GetText(0,180));

		g_Log.Add("[CastleSiege] LifeStone is created - [%s] [%s][%s] (Map:%d)(X:%d, Y:%d)",
			lpObj->m_PlayerData->lpGuild->Name, lpObj->AccountID, lpObj->Name, lpObj->MapNumber, cX, cY);

		lpObj->m_btLifeStoneCount++;

	}
	else
	{
		MsgOutput(iIndex, Lang.GetText(0,181));
		return FALSE;
	}

	return TRUE;
}

int CLifeStone::DeleteLifeStone(int iIndex)
{
	if ( iIndex < 0 || iIndex > g_ConfigRead.server.GetObjectMax()-1 )
		return FALSE;

	LPOBJ lpLifeStone = &gObj[iIndex];

	if ( lpLifeStone->m_PlayerData->lpGuild )
	{
		g_Log.Add("[CastleSiege] LifeStone is broken - [%s]", lpLifeStone->m_PlayerData->lpGuild->Name);
		lpLifeStone->m_PlayerData->lpGuild->lpLifeStone = NULL;
		delete lpLifeStone->m_PlayerData;
	}

	return TRUE;
}

int CLifeStone::SetReSpawnUserXY(int iUserIndex)
{
	if ( !gObjIsConnected(iUserIndex) )
		return FALSE;

	LPOBJ lpUser = &gObj[iUserIndex];

	if ( lpUser->MapNumber != MAP_INDEX_CASTLESIEGE )
		return FALSE;

	if ( lpUser->m_PlayerData->lpGuild == NULL )
		return FALSE;

	BYTE btCsJoinSide = lpUser->m_btCsJoinSide;

	if ( lpUser->m_PlayerData->lpGuild->lpLifeStone == NULL )
		return FALSE;

	LPOBJ lpLifeStone = lpUser->m_PlayerData->lpGuild->lpLifeStone;

	if ( lpLifeStone->m_btCreationState != 5 )
		return FALSE;

	lpUser->MapNumber = MAP_INDEX_CASTLESIEGE;
	lpUser->X = lpLifeStone->X;
	lpUser->Y = lpLifeStone->Y;

	return TRUE;
}

void CLifeStone::LifeStoneAct(int iIndex)
{
	if ( !gObjIsConnected(iIndex))
		return;

	LPOBJ lpObj = &gObj[iIndex];

	lpObj->m_iCreatedActivationTime++;
	BYTE btCreationState = lpObj->m_btCreationState;

	if ( lpObj->m_iCreatedActivationTime < 60 )
		lpObj->m_btCreationState = lpObj->m_iCreatedActivationTime / 12;
	else
		lpObj->m_btCreationState = 5;

	if ( btCreationState != lpObj->m_btCreationState )
		GSProtocol.GCSendObjectCreationState(iIndex);

	if ( lpObj->m_btCreationState < 5 )
		return;

	if ( lpObj->VPCount < 1 ) 
		return;

	int tObjNum = -1;

	for (int i=0;i<MAX_VIEWPORT;i++)
	{
		tObjNum = lpObj->VpPlayer[i].number;

		if ( tObjNum >= 0 )
		{
			if ( gObj[tObjNum].Type == OBJ_USER && gObj[tObjNum].Live )
			{
				if ( gObj[tObjNum].m_btCsJoinSide == lpObj->m_btCsJoinSide )
				{
					if ( abs(lpObj->Y - gObj[tObjNum].Y) <= 3 &&
						 abs(lpObj->X - gObj[tObjNum].X) <= 3 )
					{
						BOOL bLifeChange = FALSE;
						BOOL bManaChange = FALSE;
						BOOL bBpChange = FALSE;

						/*gObj[tObjNum].Life += 100.0f;
						gObj[tObjNum].Mana += 100.0f;
						gObj[tObjNum].BP += 100;*/

						if ( gObj[tObjNum].Life < (gObj[tObjNum].MaxLife + gObj[tObjNum].AddLife))
						{
							gObj[tObjNum].Life += ( gObj[tObjNum].MaxLife + gObj[tObjNum].AddLife ) / 100.0f;

							if ( gObj[tObjNum].Life > (gObj[tObjNum].MaxLife + gObj[tObjNum].AddLife))
								gObj[tObjNum].Life = gObj[tObjNum].MaxLife + gObj[tObjNum].AddLife;

							bLifeChange = TRUE;
						}

						if ( gObj[tObjNum].Mana < (gObj[tObjNum].MaxMana + gObj[tObjNum].AddMana))
						{
							gObj[tObjNum].Mana += ( gObj[tObjNum].MaxMana + gObj[tObjNum].AddMana ) / 100.0f;

							if ( gObj[tObjNum].Mana > (gObj[tObjNum].MaxMana + gObj[tObjNum].AddMana))
								gObj[tObjNum].Mana = gObj[tObjNum].MaxMana + gObj[tObjNum].AddMana;

							bManaChange = TRUE;
						}

						if ( gObj[tObjNum].BP < (gObj[tObjNum].MaxBP + gObj[tObjNum].AddBP))
						{
							gObj[tObjNum].BP += ( gObj[tObjNum].MaxBP + gObj[tObjNum].AddBP ) / 100;

							if ( gObj[tObjNum].BP > (gObj[tObjNum].MaxBP + gObj[tObjNum].AddBP))
								gObj[tObjNum].BP = gObj[tObjNum].MaxBP + gObj[tObjNum].AddBP;

							bBpChange = TRUE;
						}

						if (bLifeChange )
							GSProtocol.GCReFillSend(tObjNum, gObj[tObjNum].Life, 0xFF, 1, gObj[tObjNum].iShield);

						if (bManaChange ||bBpChange ) 
							GSProtocol.GCManaSend(tObjNum, gObj[tObjNum].Mana, 0xFF, 0, gObj[tObjNum].BP);
					}
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

