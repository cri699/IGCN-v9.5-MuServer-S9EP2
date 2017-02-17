//////////////////////////////////////////////////////////////////////
// DarkSpirit.cpp
#include "stdafx.h"
#include "DarkSpirit.h"
#include "MapClass.h"
#include "GameMain.h"
#include "protocol.h"
#include "BattleSoccerManager.h"
#include "gObjMonster.h"
#include "ObjAttack.h"
#include "ChaosCastle.h"
#include "BuffEffectSlot.h"
#include "IllusionTempleEvent_Renewal.h"
#include "configread.h"
#include "ImperialGuardian.h"
#include "GensSystem.h"
#include "AcheronGuardianEvent.h"
#include "zzzitem.h"
#include "NewPVP.h"
#include "ArcaBattle.h"
// GS-N 0.99.60T 0x004AA110
//	GS-N	1.00.18	JPN	0x004C7E00	-	Completed

CDarkSpirit * gDarkSpirit;



CDarkSpirit::CDarkSpirit() : m_ActionMode(PetItem_Mode_Normal)
{
	InitializeCriticalSection(&this->m_SpiritCriti);
	this->Init();
}



CDarkSpirit::~CDarkSpirit()
{
	DeleteCriticalSection(&this->m_SpiritCriti);
	return;
}



void CDarkSpirit::Init()
{
	this->m_AttackDamageMin = 0;
	this->m_AttackDamageMax = 0;
	this->m_AttackSpeed = 0;
	this->m_SuccessAttackRate = 0;
	this->m_dwLastAttackTime = 0;
	this->m_iMasterIndex = -1;
	this->m_iTargetIndex = -1;
	this->m_pPetItem = NULL;
}



void CDarkSpirit::Run()
{
	EnterCriticalSection(&this->m_SpiritCriti);

	int masteridx = this->m_iMasterIndex;

	if ( masteridx < g_ConfigRead.server.GetObjectStartUserIndex() || masteridx > g_ConfigRead.server.GetObjectMax()-1 )
	{
		LeaveCriticalSection(&this->m_SpiritCriti);
		return;
	}

	if(!gObjIsConnected(masteridx))
	{
		LeaveCriticalSection(&this->m_SpiritCriti);
		return;
	}

	LPOBJ lpObj = &gObj[masteridx];

	if ( lpObj->Class != 4 ) // dñl
	{
		LeaveCriticalSection(&this->m_SpiritCriti);
		return;
	}

	if ( this->m_pPetItem == NULL )
	{
		LeaveCriticalSection(&this->m_SpiritCriti);
		return;
	}

	if ( this->m_pPetItem->m_Durability <= 0.0f )
	{
		LeaveCriticalSection(&this->m_SpiritCriti);
		return;
	}

	if (!MAX_MAP_RANGE(lpObj->MapNumber))
	{
		LeaveCriticalSection(&this->m_SpiritCriti);
		return;
	}

	BYTE attr = MapC[lpObj->MapNumber].GetAttr(lpObj->X, lpObj->Y);

	if ( (attr&1) != 0 )
	{
		LeaveCriticalSection(&this->m_SpiritCriti);
		return;
	}

	if ( this->m_dwLastAttackTime > GetTickCount() )
	{
		LeaveCriticalSection(&this->m_SpiritCriti);
		return;
	}

	int nAttackSpeed = this->m_AttackSpeed;

	if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncDarkSpiritAttackSpeed > 0.0)
	{
		nAttackSpeed += lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncDarkSpiritAttackSpeed;
	}

	this->m_dwLastAttackTime = ( GetTickCount() + 1500 ) - ( nAttackSpeed * 10 );

	int iActionMode = this->m_ActionMode;
	LeaveCriticalSection(&this->m_SpiritCriti);

	switch ( iActionMode )
	{
		case PetItem_Mode_Normal:
			this->ModeNormal();
			break;

		case PetItem_Mode_Attack_Random:
			this->ModeAttackRandom();
			break;

		case PetItem_Mode_Attack_WithMaster:
			this->ModeAttackWithMaster();
			break;

		case PetItem_Mode_Attack_Target:
			this->ModeAttakTarget();
			break;
	}
}



void CDarkSpirit::ModeNormal()
{
	EnterCriticalSection(&this->m_SpiritCriti);

	if ( this->m_iMasterIndex < g_ConfigRead.server.GetObjectStartUserIndex() || this->m_iMasterIndex > g_ConfigRead.server.GetObjectMax()-1 )
	{
		return;
	}

	LPOBJ lpObj = &gObj[this->m_iMasterIndex];

	LeaveCriticalSection(&this->m_SpiritCriti);
}



void CDarkSpirit::ModeAttackRandom()
{
	EnterCriticalSection(&this->m_SpiritCriti);

	if ( this->m_iMasterIndex < g_ConfigRead.server.GetObjectStartUserIndex() || this->m_iMasterIndex > g_ConfigRead.server.GetObjectMax()-1 )
	{
		LeaveCriticalSection(&this->m_SpiritCriti);
		return;
	}

	LPOBJ lpObj = &gObj[this->m_iMasterIndex];

	int tObjNum;
	int count = 0;
	int FindObj[MAX_VIEWPORT];
	int FindObjCount = 0;
	int dis;
	int DuelIndex = lpObj->m_iDuelUser;
	BOOL EnableAttack;
	int iDamageType = 0;
	int iActionType = 0;
	int iCriticalDamage = this->m_CriticalDamage + lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddCriticalDamageRate;
	int iExcellentDamage = this->m_ExcellentDamage + lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddExcellentDamageRate;
	int iDoubleDamage = this->m_DoubleDamage + lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncDarkSpiritDoubleDamageRate;

	LeaveCriticalSection(&this->m_SpiritCriti);

	if ((rand() % 100) < iCriticalDamage)
	{
		iDamageType = 1;
		iActionType = 1;
	}

	if ((rand() % 100) < iExcellentDamage)
	{
		iDamageType = 2;
		iActionType = 1;
	}

	if ((rand() % 100) < iDoubleDamage)
	{
		iDamageType = 3;
		iActionType = 1;
	}

	while ( true )
	{
		if ( lpObj->VpPlayer2[count].state != 0 )
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if ( tObjNum >= 0 )
			{
				EnableAttack = FALSE;
				
				if ( gObj[tObjNum].Life > 0.0f && (gObj[tObjNum].Class < 100 || gObj[tObjNum].Class  > 110 ) )
				{
					if ( lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
					{
						EnableAttack = TRUE;
					}

					else if ( DuelIndex == tObjNum )
					{
						EnableAttack = TRUE;
					}

					else
					{
						int lc85 = gObj[tObjNum].m_Index;
						
						if ( gObj[tObjNum].Type == OBJ_MONSTER )
						{
							if ( gObj[tObjNum].m_RecallMon >= 0 )
							{
								lc85 = gObj[gObj[tObjNum].m_RecallMon].m_Index; 
							}
						}

						if ( gObjTargetGuildWarCheck(lpObj, &gObj[lc85]) == TRUE )
						{
							EnableAttack = TRUE;
						}

						if ( gObj[tObjNum].Type == OBJ_USER && g_GensSystem.IsMapBattleZone(lpObj->MapNumber) == TRUE && g_GensSystem.IsPkEnable(lpObj, &gObj[lc85]) == TRUE )
						{
							EnableAttack = TRUE;
						}

						if(gObj[tObjNum].Class >= 678 && gObj[tObjNum].Class <= 680)
						{
							EnableAttack = TRUE;
						}

						if ( IMPERIAL_MAP_RANGE(gObj[tObjNum].MapNumber) == TRUE )
						{
							EnableAttack = g_ImperialGuardian.IsAttackAbleMonster(tObjNum);
						}
					}

					if ( EnableAttack != FALSE )
					{
						if ( lpObj->MapNumber == gObj[tObjNum].MapNumber )
						{
							dis = gObjCalDistance(lpObj, &gObj[tObjNum]);
							
							if ( dis < RAVEN_ATTACK_DISTANCE-2 )
							{
								FindObj[FindObjCount] = tObjNum;
								FindObjCount++;
							}
						}
					}
				}
			}
		}

		count++;	
		if ( count > MAX_VIEWPORT-1 )
		{
			break;
		}
	}

	if ( FindObjCount != 0 )
	{
		if ( iDamageType != 0 )
		{
			int target = FindObj[rand()%FindObjCount];
			this->SendAttackMsg( lpObj->m_Index, target, iDamageType, iActionType);
		}
		else
		{
			int target = FindObj[rand()%FindObjCount];
			this->RangeAttack( lpObj->m_Index, target);
		}
	}
}




void CDarkSpirit::ModeAttackWithMaster()
{
	EnterCriticalSection(&this->m_SpiritCriti);

	if ( this->m_iMasterIndex < g_ConfigRead.server.GetObjectStartUserIndex() || this->m_iMasterIndex > g_ConfigRead.server.GetObjectMax()-1 )
	{
		LeaveCriticalSection(&this->m_SpiritCriti);
		return;
	}

	if ( this->m_iTargetIndex == -1 )
	{
		LeaveCriticalSection(&this->m_SpiritCriti);
		return;
	}

	LPOBJ lpObj = &gObj[this->m_iMasterIndex];
	LPOBJ lpTargetObj = &gObj[this->m_iTargetIndex];

	int iCriticalDamage = this->m_CriticalDamage + lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddCriticalDamageRate;
	int iExcellentDamage = this->m_ExcellentDamage + lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddExcellentDamageRate;
	int iDoubleDamage = this->m_DoubleDamage + lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncDarkSpiritDoubleDamageRate;
	int dis;

	LeaveCriticalSection(&this->m_SpiritCriti);

	if ( lpObj->MapNumber == lpTargetObj->MapNumber )
	{
		if ( lpTargetObj->Life > 0.0f )
		{
			dis = gObjCalDistance(lpObj, lpTargetObj);

			if ( dis < RAVEN_ATTACK_DISTANCE )
			{
				int iDamageType = 0;
				int iActionType = 0;

				if ((rand() % 100) < iCriticalDamage)
				{
					iDamageType = 1;
					iActionType = 1;
				}

				if ((rand() % 100) < iExcellentDamage)
				{
					iDamageType = 2;
					iActionType = 1;
				}

				if ((rand() % 100) < iDoubleDamage)
				{
					iDamageType = 3;
					iActionType = 1;
				}

				if ( iDamageType != FALSE )
				{
					this->SendAttackMsg(lpObj->m_Index, lpTargetObj->m_Index, iDamageType, iActionType);
				}
				else
				{
					this->RangeAttack(lpObj->m_Index, lpTargetObj->m_Index);
				}
			}
		}
		else
		{
			this->ReSetTarget(lpTargetObj->m_Index);
		}
	}
}




void CDarkSpirit::ModeAttakTarget()
{
	EnterCriticalSection(&this->m_SpiritCriti);

	if ( this->m_iMasterIndex < g_ConfigRead.server.GetObjectStartUserIndex() || this->m_iMasterIndex > g_ConfigRead.server.GetObjectMax()-1 )
	{
		LeaveCriticalSection(&this->m_SpiritCriti);
		return;
	}

	if ( this->m_iTargetIndex == -1 )
	{
		LeaveCriticalSection(&this->m_SpiritCriti);
		return;
	}

	LPOBJ lpObj = &gObj[this->m_iMasterIndex];
	LPOBJ lpTargetObj = &gObj[this->m_iTargetIndex];
	int count = 0;
	int dis;

	LeaveCriticalSection(&this->m_SpiritCriti);

	if (g_GensSystem.IsMapBattleZone(lpObj->MapNumber) == TRUE && g_GensSystem.IsPkEnable(lpObj, lpTargetObj) == FALSE)
	{
		return;
	}

	if ( lpObj->MapNumber == lpTargetObj->MapNumber )
	{
		if ( lpTargetObj->Life > 0.0f )
		{
			dis = gObjCalDistance(lpObj, lpTargetObj);

			if ( dis < RAVEN_ATTACK_DISTANCE )
			{
				this->SendAttackMsg(lpObj->m_Index, lpTargetObj->m_Index, 1, 1);
			}
		}
		else
		{
			this->ReSetTarget(lpTargetObj->m_Index);
		}
	}
}




void CDarkSpirit::RangeAttack(int aIndex, int aTargetIndex)
{
	EnterCriticalSection(&this->m_SpiritCriti);

	if ( this->m_iMasterIndex < g_ConfigRead.server.GetObjectStartUserIndex() || this->m_iMasterIndex > g_ConfigRead.server.GetObjectMax()-1 )
	{
		LeaveCriticalSection(&this->m_SpiritCriti);
		return;
	}

	LeaveCriticalSection(&this->m_SpiritCriti);

	LPOBJ lpObj = &gObj[aIndex];
	int StartDis = 1;
	int tObjNum;
	int count = 0;
	int loopcount = 0;
	int attackcheck;
	int EnableAttack;
	int HitCount = 0;

	this->SendAttackMsg(lpObj->m_Index, aTargetIndex, 0, 0);

	while ( true )
	{
		if ( lpObj->VpPlayer2[count].state != 0 )
		{
			tObjNum = lpObj->VpPlayer2[count].number;

			if ( tObjNum >= 0 && aTargetIndex != tObjNum )
			{
				EnableAttack = FALSE;
				
				if ( lpObj->VpPlayer2[count].type == OBJ_MONSTER && gObj[tObjNum].m_RecallMon < 0)
				{
					EnableAttack = TRUE;
				}

				else if ( tObjNum == aTargetIndex )
				{
					EnableAttack = TRUE;
				}

				else
				{
					int lc10 = gObj[tObjNum].m_Index;

					if ( gObj[tObjNum].Type == OBJ_MONSTER )
					{
						if ( gObj[tObjNum].m_RecallMon >= 0 )
						{
							lc10 = gObj[gObj[tObjNum].m_RecallMon].m_Index;
						}
					}

					if ( gObjTargetGuildWarCheck(lpObj, &gObj[lc10]) == TRUE )
					{
						EnableAttack = TRUE;
					}

					if ( gObj[tObjNum].Type == OBJ_USER && g_GensSystem.IsMapBattleZone(lpObj->MapNumber) == TRUE && g_GensSystem.IsPkEnable(lpObj, &gObj[tObjNum]) == TRUE )
					{
						EnableAttack = TRUE;
					}

					if(gObj[tObjNum].Class >= 678 && gObj[tObjNum].Class <= 680)
					{
						EnableAttack = TRUE;
					}
				}

				if ( EnableAttack != FALSE )
				{
					attackcheck = FALSE;

					if ( loopcount == 0 )
					{
						attackcheck = TRUE;
					}
					else if ( (rand()%3) == 0 )
					{
						attackcheck = TRUE;
					}

					if ( attackcheck != FALSE )
					{
						if ( gObjCalDistance(&gObj[aTargetIndex], &gObj[tObjNum]) < RAVEN_ATTACK_DISTANCE-3 )
						{
							this->SendAttackMsg(lpObj->m_Index, tObjNum, 2, 0);
							HitCount++;

							if ( HitCount > 3 )
							{
								break;
							}
						}
					}
				}
			}				
		}

		count++;

		if ( count > MAX_VIEWPORT-1 )
		{
			break;
		}
	}
}


struct PMSG_PET_ITEM_ATTACK_COMMAND
{
	PBMSG_HEAD h;	// C1:A8
	BYTE PetType;	//	3
	BYTE SkillType;	// 4
	BYTE NumberH;	// 5
	BYTE NumberL;	// 6
	BYTE TargetNumberH;	// 7
	BYTE TargetNumberL;	// 8
};

void CDarkSpirit::SendAttackMsg(int aIndex, int aTargetIndex, int iDamageType, int iActionType)
{
	PMSG_PET_ITEM_ATTACK_COMMAND pMsg;

	if ( iDamageType == 3 )
	{
		iDamageType = 0;
	}

	pMsg.PetType = 0;
	pMsg.SkillType = (iDamageType)? 0 : 1;
	pMsg.NumberH = SET_NUMBERH(aIndex);
	pMsg.NumberL = SET_NUMBERL(aIndex);
	pMsg.TargetNumberH = SET_NUMBERH(aTargetIndex);
	pMsg.TargetNumberL = SET_NUMBERL(aTargetIndex);
	pMsg.h.set((LPBYTE)&pMsg, 0xA8, sizeof(pMsg));
	
	IOCP.DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
	GSProtocol.MsgSendV2(&gObj[aIndex], (UCHAR*)&pMsg, pMsg.h.size);

	if (iActionType == 1)
	{
		EnterCriticalSection(&this->m_SpiritCriti);
		
		LPOBJ lpObj = &gObj[this->m_iMasterIndex];
		int iDamageType = 0;
		int iCriticalDamage = this->m_CriticalDamage + lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddCriticalDamageRate;
		int iExcellentDamage = this->m_ExcellentDamage + lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddExcellentDamageRate;
		int iDoubleDamage = this->m_DoubleDamage + lpObj->m_PlayerData->m_MPSkillOpt.iMpsIncDarkSpiritDoubleDamageRate;

		LeaveCriticalSection(&this->m_SpiritCriti);

		if (iCriticalDamage > rand() % 100)
		{
			iDamageType = 1;
		}

		if (iExcellentDamage > rand() % 100)
		{
			iDamageType = 2;
		}

		if (iDoubleDamage > rand() % 100)
		{
			iDamageType = 3;
		}
	}

	gObjAddAttackProcMsgSendDelay(&gObj[aIndex], 51, aTargetIndex, 600, iDamageType, iActionType);
}



void CDarkSpirit::SetTarget(int aTargetIndex)
{
	EnterCriticalSection(&this->m_SpiritCriti);

	if ( this->m_iMasterIndex == -1 )
	{
		LeaveCriticalSection(&this->m_SpiritCriti);
		return;
	}

	if ( this->m_iMasterIndex < g_ConfigRead.server.GetObjectStartUserIndex() || this->m_iMasterIndex > g_ConfigRead.server.GetObjectMax()-1 )
	{
		LeaveCriticalSection(&this->m_SpiritCriti);
		return;
	}

	LPOBJ lpObj = &gObj[this->m_iMasterIndex];

	if ( lpObj->Class != 4 )	// DL
	{
		LeaveCriticalSection(&this->m_SpiritCriti);
		return;
	}

	this->m_iTargetIndex = aTargetIndex;
	LeaveCriticalSection(&this->m_SpiritCriti);
}



void CDarkSpirit::ReSetTarget(int aTargetIndex)
{
	EnterCriticalSection(&this->m_SpiritCriti);

	if ( this->m_iMasterIndex == -1 )
	{
		LeaveCriticalSection(&this->m_SpiritCriti);
		return;
	}

	if ( this->m_iMasterIndex < g_ConfigRead.server.GetObjectStartUserIndex() || this->m_iMasterIndex > g_ConfigRead.server.GetObjectMax()-1 )
	{
		LeaveCriticalSection(&this->m_SpiritCriti);
		return;
	}

	LPOBJ lpObj = &gObj[this->m_iMasterIndex];

	if ( lpObj->Class != 4 )	// DL
	{
		LeaveCriticalSection(&this->m_SpiritCriti);
		return;
	}

	if ( aTargetIndex != this->m_iTargetIndex )
	{
		LeaveCriticalSection(&this->m_SpiritCriti);
		return;
	}

	this->m_iTargetIndex = -1;
	int iActionMode = this->m_ActionMode;

	LeaveCriticalSection(&this->m_SpiritCriti);

	if ( iActionMode == PetItem_Mode_Attack_Target )
	{
		this->SetMode(PetItem_Mode_Normal, -1);
	}
}



struct PMSG_SEND_PET_ITEM_COMMAND
{
	PBMSG_HEAD h;	// C1:A7
	BYTE PetType;	// 3
	BYTE Command;	// 4
	BYTE NumberH;	// 5
	BYTE NumberL;	// 6
};

void CDarkSpirit::SetMode(ePetItem_Mode mode, int iTargetindex)
{
	EnterCriticalSection(&this->m_SpiritCriti);

	if ( this->m_iMasterIndex == -1 )
	{
		LeaveCriticalSection(&this->m_SpiritCriti);
		return;
	}

	if ( this->m_iMasterIndex < g_ConfigRead.server.GetObjectStartUserIndex() || this->m_iMasterIndex > g_ConfigRead.server.GetObjectMax()-1 )
	{
		LeaveCriticalSection(&this->m_SpiritCriti);
		return;
	}

	LPOBJ lpObj = &gObj[this->m_iMasterIndex];

	if ( lpObj->Class != CLASS_DARKLORD )	// DL
	{
		LeaveCriticalSection(&this->m_SpiritCriti);
		return;
	}

	if ( mode == PetItem_Mode_Attack_Target || this->m_ActionMode != mode)
	{
		if ( mode == PetItem_Mode_Attack_Target )
		{
			if ( ObjectMaxRange(iTargetindex) != FALSE )
			{
				this->m_iTargetIndex = iTargetindex;
			}

			else
			{
				LeaveCriticalSection(&this->m_SpiritCriti);
				return;
			}
		}

		this->m_ActionMode = mode;
		int iTargetIndex = this->m_iTargetIndex;

		LeaveCriticalSection(&this->m_SpiritCriti);

		PMSG_SEND_PET_ITEM_COMMAND pMsg;

		pMsg.h.set((LPBYTE)&pMsg, 0xA7, sizeof(pMsg));
		pMsg.PetType = 0;
		pMsg.Command = mode;
		pMsg.NumberH = SET_NUMBERH(iTargetIndex);
		pMsg.NumberL = SET_NUMBERL(iTargetIndex);
		IOCP.DataSend(lpObj->m_Index, (UCHAR*)&pMsg, sizeof(pMsg));
		return;
	}

	LeaveCriticalSection(&this->m_SpiritCriti);
}




void CDarkSpirit::Set(int aIndex, CItem * pPetItem)
{
	EnterCriticalSection(&this->m_SpiritCriti);
	LPOBJ lpObj = &gObj[aIndex];

	if ( lpObj->Class != 4 )	// DL
	{
		LeaveCriticalSection(&this->m_SpiritCriti);
		return;
	}

	BOOL bChange = FALSE;

	if ( this->m_iMasterIndex == -1 )
	{
		bChange = TRUE;
	}

	this->Init();

	if ( pPetItem->m_Type != ITEMGET(13,5) ) // Talon of Dark Raven
	{
		LeaveCriticalSection(&this->m_SpiritCriti);
		return;
	}

	if ( pPetItem->m_Durability <= 0.0f )
	{
		LeaveCriticalSection(&this->m_SpiritCriti);
		return;
	}

	int petitemlevel = pPetItem->m_PetItem_Level;
	int leadership = lpObj->Leadership + lpObj->AddLeadership;

	g_ConfigRead.m_ItemCalcLua.Generic_Call("DarkSpirit_CalcValues", "ii>iiddiiii", leadership, petitemlevel,
		&this->m_AttackDamageMin, &this->m_AttackDamageMax, &this->m_CriticalDamage, &this->m_ExcellentDamage,
		&this->m_DoubleDamage, &this->m_IgnoreEnemy, &this->m_AttackSpeed, &this->m_SuccessAttackRate);

	this->m_iMasterIndex = aIndex;
	this->m_pPetItem = pPetItem;

	LeaveCriticalSection(&this->m_SpiritCriti);

	if ( bChange != FALSE )
	{
		this->SetMode(PetItem_Mode_Normal, -1);
	}
}



BOOL CDarkSpirit::Attack(LPOBJ lpObj, LPOBJ lpTargetObj, CMagicInf * lpMagic, int iDamageType, int iActionType)
{
	int skillSuccess = 0;
	LPOBJ lpCallObj;
	LPOBJ lpCallTargetObj;
	int MsgDamage = 0;
	int ManaChange = 0;
	int iTempShieldDamage = 0;
	int iTotalShieldDamage = 0;

	if ( (lpObj->Authority&2) == 2 || (lpTargetObj->Authority&2) == 2 ) //s4 add-on
	{
		return FALSE;
	}

	if ( (lpObj->Authority&32) == 32 || (lpTargetObj->Authority&32) == 32) //s4 add-on
	{
		if(gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_INVISABLE) == TRUE)
		{
			return FALSE;
		}

		if(gObjCheckUsedBuffEffect(lpTargetObj, BUFFTYPE_INVISABLE) == TRUE)
		{
			return FALSE;
		}
	}

	if ( lpObj->MapNumber != lpTargetObj->MapNumber )
	{
		return FALSE;
	}

	if ( IMPERIAL_MAP_RANGE(lpObj->MapNumber) == TRUE && g_ImperialGuardian.IsAttackAbleMonster(lpTargetObj->m_Index) == false )
	{
		return FALSE;
	}

	if (g_ConfigRead.server.GetServerType() == SERVER_CASTLE)
	{
		if (g_Crywolf.GetCrywolfState() == 3 || g_Crywolf.GetCrywolfState() == 5)
		{
			if (CRYWOLF_MAP_RANGE(lpTargetObj->MapNumber))
			{
				if (lpTargetObj->Type == OBJ_MONSTER)
				{
					return FALSE;
				}
			}
		}
	}

	int skill = 0;
	
	if ( lpMagic )
		skill = lpMagic->m_Skill;

	skillSuccess = TRUE;

	if ( lpObj->m_PlayerData->GuildNumber > 0 )
	{
		if ( lpObj->m_PlayerData->lpGuild )
		{
			if ( lpObj->m_PlayerData->lpGuild->WarState )
			{
				if ( lpObj->m_PlayerData->lpGuild->WarType == 1 )
				{
					if ( !GetBattleSoccerGoalMove(0) )
					{
						return TRUE;
					}
				}
			}
		}
	}

	if ( lpTargetObj->Type == OBJ_MONSTER )
	{
		if ( lpTargetObj->Class == 200 )
		{
			if ( skill )
			{
				gObjMonsterStateProc(lpTargetObj, 7, lpObj->m_Index, 0);
			}
			else
			{
				gObjMonsterStateProc(lpTargetObj, 6, lpObj->m_Index, 0);
			}

			return TRUE;
		}
	}

	if ( lpObj->Type == OBJ_USER )
	{
		if ( !gObjIsConnected(lpObj))
		{
			return FALSE;
		}
	}

	if ( lpTargetObj->Type == OBJ_USER )
	{
		if ( !gObjIsConnected(lpTargetObj))
		{
			return FALSE;
		}
	}

	if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_MONSTER )	// PLAYER VS MONSTER
	{
		if ( lpObj->m_RecallMon >= 0 )
		{
			if ( lpObj->m_RecallMon == lpTargetObj->m_Index )
			{
				return FALSE;
			}
		}
	}

	if ( !gObjAttackQ(lpTargetObj))
	{
		return FALSE;
	}

	if (g_ArcaBattle.IsArcaBattleServer() == TRUE && g_AcheronGuardianEvent.IsPlayStart() == false && g_ArcaBattle.IsEnableAttackObelisk(lpObj, lpTargetObj->Class) == false)
	{
		return FALSE;
	}

	if (lpTargetObj->Type == OBJ_USER && g_NewPVP.IsDuel(*lpTargetObj) && g_NewPVP.IsSafeState(*lpTargetObj))
	{
		return FALSE;
	}

	if (lpObj->Type == OBJ_USER && g_NewPVP.IsDuel(*lpObj) && g_NewPVP.IsSafeState(*lpObj))
	{
		return FALSE;
	}

	if (lpTargetObj->Type == OBJ_USER && g_NewPVP.IsObserver(*lpTargetObj))
	{
		return FALSE;
	}

	if (lpObj->Type == OBJ_USER && g_NewPVP.IsObserver(*lpObj))
	{
		return FALSE;
	}

	if (g_ConfigRead.server.GetServerType() == SERVER_CASTLE)
	{
		if (g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
		{
			if (lpObj->m_btCsJoinSide > 0)
			{
				if (lpObj->m_btCsJoinSide == lpTargetObj->m_btCsJoinSide)
				{
					return FALSE;
				}
			}
		}
	}

	lpObj->m_TotalAttackCount++;

	if ( this->CheckAttackArea(lpObj, lpTargetObj) == FALSE )
	{
		return FALSE;
	}

	lpCallObj = lpObj;
	lpCallTargetObj = lpTargetObj;

	if ( lpTargetObj->Type == OBJ_MONSTER )
	{
		if ( lpTargetObj->m_RecallMon >= 0 )
		{
			lpCallTargetObj = &gObj[lpTargetObj->m_RecallMon];
		}
	}

	if ( this->PkCheck(lpCallObj, lpTargetObj) == FALSE )
	{
		return FALSE;
	}

	int Strength = lpObj->m_PlayerData->Strength + lpObj->AddStrength;
	int Dexterity = lpObj->m_PlayerData->Dexterity + lpObj->AddDexterity;
	int Vitality = lpObj->m_PlayerData->Vitality + lpObj->AddVitality;
	int Energy = lpObj->m_PlayerData->Energy + lpObj->AddEnergy;
	BOOL bIsOnDuel = gObjDuelCheck(lpObj, lpTargetObj);

	if ( bIsOnDuel )
	{
		lpObj->m_iDuelTickCount = GetTickCount();
		lpTargetObj->m_iDuelTickCount = GetTickCount();
	}

	int MSBFlag = 0;
	
	if ( iDamageType == 1 )
	{
		iDamageType = 3;
	}

	else if ( iDamageType == 2 )
	{
		MsgDamage = 2;
	}

	else
	{
		MsgDamage = 0;
	}

	BOOL bAllMiss = FALSE;
	int AttackDamage = 0;

	if ( g_ShieldSystemOn == TRUE )
	{
		if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
		{
			if ( !this->MissCheckPvP(lpObj, lpTargetObj, skill, skillSuccess, bAllMiss) )
			{
				return FALSE;
			}
		}

		else if ( !this->MissCheck(lpObj, lpTargetObj, skill, skillSuccess, bAllMiss) )
		{
			return FALSE;
		}

	}

	else if ( !this->MissCheck(lpObj, lpTargetObj, skill, skillSuccess, bAllMiss) )
	{
		return FALSE;
	}

	int iOriginTargetDefense = 0;
	int targetdefense = this->GetTargetDefense(lpObj, lpTargetObj, MsgDamage, iOriginTargetDefense);

	EnterCriticalSection(&this->m_SpiritCriti);
	float fIgnoreEnemy = this->m_IgnoreEnemy;
	LeaveCriticalSection(&this->m_SpiritCriti);

	if (lpObj->Type == OBJ_USER)
	{
		if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsDarkSpiritIgnoreEnemyDefense > 0.0)
		{
			fIgnoreEnemy += lpObj->m_PlayerData->m_MPSkillOpt.iMpsDarkSpiritIgnoreEnemyDefense;
		}
	}

	if (rand() % 100 < fIgnoreEnemy)
	{
		targetdefense = 0;
		MsgDamage = 1;
	}

	AttackDamage = this->GetAttackDamage(lpObj, targetdefense, iDamageType);

	if (iDamageType == 0 && iActionType == 0)
	{
		AttackDamage /= 1.5;
	}

	if (iDamageType == 2)
	{
		AttackDamage += AttackDamage * 20.0 / 100.0;
	}

	if ( bAllMiss )
	{
		AttackDamage = ( AttackDamage * 30 ) / 100;
	}

	if ( lpTargetObj->DamageMinus )
	{
		int beforeDamage = AttackDamage;
		AttackDamage -= ( ( AttackDamage * (int)lpTargetObj->DamageMinus) / 100 );
	}

	int tlevel = lpObj->Level / 10;

	if ( AttackDamage < tlevel )
	{
		if ( tlevel < 1 )
		{
			tlevel = 1;
		}

		AttackDamage = tlevel;
	}

	if ( lpTargetObj->m_SkillNumber == 18 )
	{
		if ( AttackDamage > 1 )
		{
			AttackDamage >>= 1;
		}
	}

	if (g_ConfigRead.pet.DamageDisableForPet[DAMAGE_OFF_RAVEN] == false)
	{
		int idamage = AttackDamage * 2 / 100;

		lpObj->pInventory[1].m_DurabilitySmall += idamage - (idamage * lpObj->m_PlayerData->m_MPSkillOpt.iMpsPetDurDownSpeed / 100);
		int DurabilityVal = 65000;

		if ( lpObj->pInventory[1].m_DurabilitySmall >= DurabilityVal )
		{
			lpObj->pInventory[1].m_DurabilitySmall = 0;
			lpObj->pInventory[1].m_Durability -= 1.0f;

			if ( lpObj->pInventory[1].m_Durability < 1.0f )
			{
				lpObj->pInventory[1].m_Durability = 0;
				UINT64 iPetExp = lpObj->pInventory[1].m_PetItem_Exp;

				if ( lpObj->pInventory[1].DecPetItemExp(10) )
				{
					g_Log.Add("[%s][%s][PetItemExpDown] [%s] Level:[%d]Exp:[%I64d]DecExp[%I64d]",
						lpObj->AccountID, lpObj->Name, lpObj->pInventory[1].GetName(),
						lpObj->pInventory[1].m_PetItem_Level,
						lpObj->pInventory[1].m_PetItem_Exp, iPetExp - lpObj->pInventory[1].m_PetItem_Exp);

					this->Set(lpObj->m_Index, &lpObj->pInventory[1]);
					this->SendLevelmsg(lpObj->m_Index, 1, 0, 0xFF);
				}

				g_Log.Add("[%s][%s] Dark Raven Item is Broken because durability is exhausted [%I64d]",
					lpObj->AccountID, lpObj->Name, lpObj->pInventory[1].m_Number);
			}

			GSProtocol.GCItemDurSend(lpObj->m_Index, 1, lpObj->pInventory[1].m_Durability, 0);
		}
	}

	if ( gObjAngelSprite(lpTargetObj) == TRUE )
	{
		if ( AttackDamage > 1 )
		{
			float  damage = (AttackDamage * 8) / 10.0f;
			AttackDamage = damage;
		}
	}

	if ( gObjWingSprite(lpTargetObj) == TRUE )
	{
		CItem * Wing = &lpTargetObj->pInventory[7];

		if ( AttackDamage > 1 && Wing->m_Type != ITEMGET(13, 30) )
		{
			double WingDamageBlock;

			if (lpTargetObj->m_PlayerData->m_MPSkillOpt.iMpsAddWingDamageBlock <= 0.0)
			{
				WingDamageBlock = 0.0;
			}

			else
			{
				WingDamageBlock = lpTargetObj->m_PlayerData->m_MPSkillOpt.iMpsAddWingDamageBlock;
			}

			g_ConfigRead.m_ItemCalcLua.Generic_Call("Wings_CalcAbsorb", "iiid>i", AttackDamage, Wing->m_Type, Wing->m_Level, WingDamageBlock, &AttackDamage);
		}
	}

	if ( gObjDenorantSprite(lpTargetObj ) )
	{
		CItem * Dinorant = &lpTargetObj->pInventory[8];
		int dinorantdecdamage = 90 - Dinorant->IsDinorantReduceAttackDamaege();
		lpObj->Life -= 1.0f;

		if ( lpObj->Life < 0.0f )
		{
			lpObj->Life = 0.0f;
		}
		else
		{
			AttackDamage = AttackDamage * dinorantdecdamage / 100;
		}

		GSProtocol.GCReFillSend(lpObj->m_Index, lpObj->Life, 0xFF, 0, lpObj->iShield);
	}

	if ( gObjDarkHorse(lpTargetObj ) )
	{
		CItem * Darkhorse = &lpTargetObj->pInventory[8];
		int decdamage = 100 - ((Darkhorse->m_PetItem_Level + 30) / 2 );

		lpTargetObj->Life -= 1.0f;

		if ( lpTargetObj->Life < 0.0f )
		{
			lpTargetObj->Life = 0.0f;
		}
		else
		{
			AttackDamage = AttackDamage * decdamage / 100;
		}

		GSProtocol.GCReFillSend(lpTargetObj->m_Index, lpTargetObj->Life, 0xFF, 0, lpTargetObj->iShield);
	}

	if ( lpTargetObj->Live )
	{
		if ( lpTargetObj->m_SkillInfo.SoulBarrierDefence && AttackDamage > 0)
		{
			int replacemana = (WORD)lpTargetObj->Mana * 2 / 100;

			if ( replacemana < lpTargetObj->Mana )
			{
				lpTargetObj->Mana -= replacemana;
				int decattackdamage = AttackDamage * lpTargetObj->m_SkillInfo.SoulBarrierDefence / 100;
				AttackDamage -= decattackdamage;
				ManaChange = TRUE;

			}
		}

		if ( g_ShieldSystemOn == FALSE )
		{
			if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
			{
				if ( CC_MAP_RANGE(lpObj->MapNumber ) && CC_MAP_RANGE(lpTargetObj->MapNumber) )
				{
					AttackDamage = AttackDamage * 50 / 100;
				}
			}

			if (lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER)
			{
				if (lpObj->MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL && lpTargetObj->MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL)
				{
					AttackDamage = AttackDamage * 50 / 100;
				}
			}
		}

		if (g_ConfigRead.server.GetServerType() == SERVER_CASTLE)
		{
			if (g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
			{
				if (lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER)
				{
					if (lpObj->MapNumber == MAP_INDEX_CASTLESIEGE && lpTargetObj->MapNumber == MAP_INDEX_CASTLESIEGE)
					{
						if (lpObj->m_btCsJoinSide == lpTargetObj->m_btCsJoinSide)
						{
							AttackDamage = AttackDamage * g_CastleSiege.CastleSiegeSelfDmgReductionPercent / 100;
						}
						else if (g_ShieldSystemOn == FALSE)
						{
							AttackDamage = AttackDamage * g_CastleSiege.CastleSiegeDmgReductionPercent / 100;
						}
					}
				}
			}
		}

		if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_MONSTER )
		{
			if ( lpTargetObj->Class == 283 )
			{
				if ( gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_BLESS_POTION) == TRUE || gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_SOUL_POTION) == TRUE )
				{
					AttackDamage += (AttackDamage * 20) / 100;
				}
				else 
				{
					if ( lpObj->m_iAccumulatedDamage > 100 )
					{
						gObjWeaponDurDownInCastle(lpObj, lpTargetObj, 1);
						lpObj->m_iAccumulatedDamage = 0;
					}
					else
					{
						lpObj->m_iAccumulatedDamage += AttackDamage;
					}

					AttackDamage = AttackDamage * 20 / 100;
				}
			}

			if ( lpTargetObj->Class == 277 )
			{
				if ( gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_BLESS_POTION) == TRUE || gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_SOUL_POTION) == TRUE )
				{
					AttackDamage += (AttackDamage * 20) / 100;
				}
				else 
				{
					if ( lpObj->m_iAccumulatedDamage > 100 )
					{
						gObjWeaponDurDownInCastle(lpObj, lpTargetObj, 1);
						lpObj->m_iAccumulatedDamage = 0;
					}
					else
					{
						lpObj->m_iAccumulatedDamage += AttackDamage;
					}

					AttackDamage = AttackDamage * 20 / 100;
				}
			}
		}

		if (bIsOnDuel == TRUE)
		{
			AttackDamage = AttackDamage * g_NewPVP.m_iDuelDamageReduction / 100;
		}

		if (g_GensSystem.IsMapBattleZone(lpObj->MapNumber) && g_GensSystem.IsMapBattleZone(lpTargetObj->MapNumber))
		{
			if (lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER)
			{
				if (g_GensSystem.IsPkEnable(lpObj, lpTargetObj) == true)
				{
					AttackDamage = AttackDamage * g_GensSystem.GetDamageReduction() / 100;
				}
			}
		}

		if (lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER)
		{
			AttackDamage = AttackDamage * g_ConfigRead.calc.PvPDamageRate_DarkSpirit[lpObj->Class][lpTargetObj->Class] / 100;
		}

		else if (lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_MONSTER)
		{
			AttackDamage = AttackDamage * g_ConfigRead.calc.PvMDamageRate_DarkSpirit / 100;
		}

		if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
		{
			iTempShieldDamage = this->GetShieldDamage(lpObj, lpTargetObj, AttackDamage);
			lpTargetObj->iShield -= iTempShieldDamage;
			lpTargetObj->Life -= AttackDamage - iTempShieldDamage;
			iTotalShieldDamage += iTempShieldDamage;

			if ( lpTargetObj->Life < 0.0f )
			{
				lpTargetObj->Life = 0.0f;
			}
		}
		else
		{
			lpTargetObj->Life -= AttackDamage;

			if ( lpTargetObj->Life < 0.0f )
			{
				lpTargetObj->Life = 0.0f;
			}
		}
	}

	if ( lpTargetObj->Type == OBJ_MONSTER )
	{
		gObjAddMsgSendDelay(lpTargetObj, 0, lpObj->m_Index, 100, 0);
		lpTargetObj->LastAttackerID = lpObj->m_Index;

		if ( lpTargetObj->m_iCurrentAI )
		{
			lpTargetObj->m_Agro->IncAgro(lpObj->m_Index, AttackDamage / 100);
		}
	}

	if ( gObjCheckUsedBuffEffect(lpTargetObj, BUFFTYPE_STONE) == true )
	{
		gObjRemoveBuffEffect(lpTargetObj, BUFFTYPE_STONE);
		GSProtocol.GCMagicCancelSend(lpTargetObj, 51);
	}

	BOOL selfdefense = 0;
	lpCallObj = lpTargetObj;
	
	if ( lpTargetObj->Type == OBJ_MONSTER )
	{
		if ( lpTargetObj->m_RecallMon >= 0 )
		{
			lpCallObj = &gObj[lpTargetObj->m_RecallMon];
		}
	}

	if ( AttackDamage >= 1 )
	{
		if ( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER )
		{
			if ( gObjDuelCheck(lpObj, lpTargetObj) )
			{
				selfdefense = 0;
			}
			else if ( CC_MAP_RANGE(lpObj->MapNumber) || CC_MAP_RANGE(lpTargetObj->MapNumber) )
			{
				selfdefense = 0;
			}
			else if (lpObj->MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL || lpTargetObj->MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL)
			{
				selfdefense = 0;
			}
			else if ( IT_MAP_RANGE(lpObj->MapNumber) || IT_MAP_RANGE(lpTargetObj->MapNumber) )
			{
				selfdefense = 0;
			}
			else
			{
				selfdefense = 1;
			}

			if ( gObjGetRelationShip(lpObj, lpTargetObj) == 2 )
			{
				selfdefense = FALSE;
			}

			if (g_ConfigRead.server.GetServerType() == SERVER_CASTLE)
			{
				if (g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
				{
					if (lpObj->m_btCsJoinSide > 0)
					{
						selfdefense = FALSE;
					}
				}
			}
		}
		else if ( lpTargetObj->Type == OBJ_MONSTER && lpObj->Type == OBJ_USER )
		{
			if ( lpTargetObj->m_RecallMon >= 0 )
			{
				selfdefense = TRUE;
			}
		}

		if ( lpTargetObj->Type == OBJ_USER )
		{
			gObjArmorRandomDurDown(lpTargetObj, lpObj);
		}
	}

	if ( selfdefense == TRUE )
	{
		if ( !gObjTargetGuildWarCheck(lpObj, lpCallObj) )
		{
			if(lpCallObj->PartyNumber >= 0) //Season 2.5 add-on
			{
				//season3 removed party for LOL
				int number = 0;
				int partynum = lpCallObj->PartyNumber;
				
				if( (gParty.GetPKPartyPenalty(partynum)) < 5)
				{
					gObjCheckSelfDefense(lpObj, lpCallObj->m_Index);
				}	
			}
			else
			{
				gObjCheckSelfDefense(lpObj, lpCallObj->m_Index);
			}
		}
	}

	if ( AttackDamage >= 5 )	// To make strong hit
	{
		if ( lpTargetObj->Type == OBJ_MONSTER )
		{
			if ( (rand()%26) == 0 )
			{
				gObjAddMsgSendDelay(lpTargetObj,4, lpObj->m_Index, 100, 0);
			}
		}
		else if ( (rand()%4) == 0 )
		{
			if ( !gObjUniriaSprite(lpTargetObj) )
			{
				MSBFlag = 1;
			}
		}
	}

	if (lpTargetObj->Type == OBJ_USER)
	{
		if (lpTargetObj->pInventory[lpTargetObj->m_btInvenPetPos].IsItem() == TRUE && lpTargetObj->pInventory[lpTargetObj->m_btInvenPetPos].m_Type == ITEMGET(13,37))
		{
			if (g_ConfigRead.data.common.DisableMSBEffect[MSB_DISABLE_CHARACTER_FENRIR] == true)
			{
				MSBFlag = FALSE;
			}
		}

		else
		{
			if (g_ConfigRead.data.common.DisableMSBEffect[MSB_DISABLE_CHARACTER] == true)
			{
				MSBFlag = FALSE;
			}
		}
	}

	else if (lpTargetObj->Type == OBJ_MONSTER)
	{
		if (g_ConfigRead.data.common.DisableMSBEffect[MSB_DISABLE_MONSTER] == true)
		{
			MSBFlag = FALSE;
		}
	}

	if ( ManaChange )
	{
		GSProtocol.GCManaSend(lpTargetObj->m_Index, lpTargetObj->Mana, 0xFF, 0, lpTargetObj->BP);
	}

	if ( lpObj->Type == OBJ_USER )
	{
		if ( lpObj->m_Change == 9 )
		{
			GSProtocol.GCMagicAttackNumberSend(lpObj, 3, lpTargetObj->m_Index, 1);
		}
	}

	lpObj->m_Rest = 0;

	if ( AttackDamage > 0 )
	{
		int atd_reflect = (int)((float)AttackDamage * lpTargetObj->DamageReflect / 100.0f);

		if(lpTargetObj->DamageReflect > g_ConfigRead.calc.ReflectDamage)
		{
			lpTargetObj->DamageReflect = g_ConfigRead.calc.ReflectDamage;
		}

		if ( atd_reflect )
		{
			gObjAddMsgSendDelay(lpTargetObj, 10, lpObj->m_Index, 10, atd_reflect);
		}

		if ( lpObj->Type == OBJ_USER && (rand()%100) < lpObj->m_PlayerData->SetOpReflectionDamage )
		{
			gObjAddMsgSendDelay(lpTargetObj, 10, lpObj->m_Index, 10, AttackDamage);
		}

		if ( g_ShieldSystemOn == TRUE )	// #error Remove the //
		{
			AttackDamage -= iTotalShieldDamage;
		}

		gObjLifeCheck(lpTargetObj, lpObj, AttackDamage, 0, MSBFlag, MsgDamage, skill, iTotalShieldDamage, 0);
	}
	else
	{
		GSProtocol.GCDamageSend(lpObj->m_Index, lpTargetObj->m_Index, 0, 0, MsgDamage, 0);
	}

	if ( lpObj->Life <= 0.0f && lpObj->Type == OBJ_USER )
	{
		if ( lpObj->m_CheckLifeTime <= 0 )
		{
			lpObj->lpAttackObj = lpTargetObj;

			if ( lpTargetObj->Type == OBJ_USER )
			{
				lpObj->m_bAttackerKilled = true;
			}
			else
			{
				lpObj->m_bAttackerKilled = false;
			}

			lpObj->m_CheckLifeTime = 3;
		}
	}

	return TRUE;
}



int  CDarkSpirit::GetAttackDamage(LPOBJ lpObj, int targetDefense, int crititcaldamage)
{
	if (lpObj->Type != OBJ_USER)
	{
		return 0;
	}

	EnterCriticalSection(&this->m_SpiritCriti);

	int ad;
	int sub = this->m_AttackDamageMax - this->m_AttackDamageMin;
	int AttackDamage;

	if (sub < 0)
		sub = 0;

	if ( crititcaldamage == 1 )
	{
		AttackDamage = this->m_AttackDamageMin + sub;
	}

	else if ( crititcaldamage == 2 )
	{
		AttackDamage = this->m_AttackDamageMax;
		AttackDamage += ((this->m_AttackDamageMin + sub) * 20) / 100.0f;
	}

	else
	{
		__try
		{
			AttackDamage = this->m_AttackDamageMin + (rand() % (sub + 1));
		}

		__except (sub = 1, 1)
		{

		}
	}

	if (GetItemKindA(lpObj->pInventory[0].m_Type) == ITEM_KIND_A_WEAPON && GetItemKindB(lpObj->pInventory[0].m_Type) == ITEM_KIND_B_SCEPTER)
	{
		int damage = lpObj->pInventory[0].m_Magic / 2;
		damage -= damage * lpObj->pInventory[0].m_CurrentDurabilityState;
		damage = AttackDamage * damage / 100;
		AttackDamage += damage;
	}

	if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddDarkSpiritDamage > 0.0)
	{
		AttackDamage += lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddDarkSpiritDamage;
	}

	if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddPetAttack > 0.0)
	{
		AttackDamage += lpObj->m_PlayerData->m_MPSkillOpt.iMpsAddPetAttack;
	}

	AttackDamage = AttackDamage * g_ConfigRead.calc.DarkRavenDamageMultiplier / 100.0f;
	ad = AttackDamage - targetDefense;

	LeaveCriticalSection(&this->m_SpiritCriti);
	return ad;
}



BOOL CDarkSpirit::MissCheck(LPOBJ lpObj, LPOBJ lpTargetObj, int skill,  int skillSuccess, BOOL& bAllMiss)
{
	EnterCriticalSection(&this->m_SpiritCriti);
	int SuccessAttackRate = this->m_SuccessAttackRate;
	LeaveCriticalSection(&this->m_SpiritCriti);

	int TargetSuccessfulBlocking = lpTargetObj->m_SuccessfulBlocking;
	int MsgDamage = 0;

	if (IT_MAP_RANGE(lpTargetObj->MapNumber) && lpTargetObj->Type == OBJ_USER)
	{
		if (g_IT_Event.GetIllusionTempleState(lpTargetObj->MapNumber) == 2)
		{
			if (g_IT_Event.CheckSkillProdection(lpTargetObj->m_nITR_Index, lpTargetObj->MapNumber) == TRUE)
			{
				GSProtocol.GCDamageSend(lpObj->m_Index, lpTargetObj->m_Index, 0, 0, 0, 0);
				return 0;
			}

			if (lpTargetObj->PartyNumber == lpObj->PartyNumber)
			{
				GSProtocol.GCDamageSend(lpObj->m_Index, lpTargetObj->m_Index, 0, 0, 0, 0);
				return 0;
			}
		}
	}

	if (lpObj->Type == OBJ_USER)
	{
		if (lpObj->Class == CLASS_RAGEFIGHTER && lpObj->m_PlayerData->m_MPSkillOpt.iMpsAttackSuccessRate_Monk > 0.0)
		{
			SuccessAttackRate += lpObj->m_PlayerData->m_MPSkillOpt.iMpsAttackSuccessRate_Monk;
		}

		else if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsAttackSuccessRate > 0.0)
		{
			SuccessAttackRate += lpObj->m_PlayerData->m_MPSkillOpt.iMpsAttackSuccessRate;
		}

	}

	if ( SuccessAttackRate < TargetSuccessfulBlocking )
	{
		bAllMiss = TRUE;
	}

	if ( bAllMiss != FALSE )
	{
		if ( (rand()%100) >= 5 )
		{
			GSProtocol.GCDamageSend(lpObj->m_Index, lpTargetObj->m_Index, 0, 0, MsgDamage, 0);
			return FALSE;
		}
	}
	else if ( SuccessAttackRate )
	{
		if ( (rand()%SuccessAttackRate) < TargetSuccessfulBlocking)
		{
			GSProtocol.GCDamageSend(lpObj->m_Index, lpTargetObj->m_Index, 0, 0, MsgDamage, 0);
			return FALSE;
		}
	}

	return TRUE;
}


BOOL CDarkSpirit::MissCheckPvP(LPOBJ lpObj, LPOBJ lpTargetObj, int skill,  int skillSuccess, BOOL& bAllMiss)
{
	float iAttackRate = 0;
	float iDefenseRate = 0;
	int iAttackSuccessRate = 0;
	int AttackLevel = 0;
	int DefenseLevel = 0;

	if(IT_MAP_RANGE(lpTargetObj->MapNumber) && lpTargetObj->Type == OBJ_USER)
	{
		if (g_IT_Event.GetIllusionTempleState(lpTargetObj->MapNumber) == 2)
		{
			if (g_IT_Event.CheckSkillProdection(lpTargetObj->m_nITR_Index, lpTargetObj->MapNumber) == TRUE)
			{
				GSProtocol.GCDamageSend(lpObj->m_Index, lpTargetObj->m_Index, 0, 0, 0, 0);
				return 0;
			}
			
			if (lpTargetObj->PartyNumber == lpObj->PartyNumber)
			{
				GSProtocol.GCDamageSend(lpObj->m_Index, lpTargetObj->m_Index, 0, 0, 0, 0);
				return 0;
			}
		}
	}

	AttackLevel = lpObj->Level + lpObj->m_PlayerData->MasterLevel;

	switch ( lpObj->Class )
	{
		case CLASS_WIZARD:
			iAttackRate = ( lpObj->m_PlayerData->Dexterity * 4 ) / 10 + 3 * AttackLevel;
			break;
		case CLASS_KNIGHT:
			iAttackRate = ( lpObj->m_PlayerData->Dexterity * 45 ) / 10 + 3 * AttackLevel;
			break;
		case CLASS_ELF:
			iAttackRate = ( lpObj->m_PlayerData->Dexterity * 6 ) / 10 + 3 * AttackLevel;
			break;
		case CLASS_MAGUMSA:
			iAttackRate = ( lpObj->m_PlayerData->Dexterity * 35 ) / 10 + 3 * AttackLevel;
			break;
		case CLASS_DARKLORD:
			iAttackRate = ( lpObj->m_PlayerData->Dexterity * 4 ) + 3 * AttackLevel;
			break;
		case CLASS_SUMMONER:
			iAttackRate = ( lpObj->m_PlayerData->Dexterity * 35 ) / 10 + 3 * AttackLevel;
			break;
		default:
			iAttackRate = ( lpObj->m_PlayerData->Dexterity * 4 ) + 3 * AttackLevel;
			break;
	}

	DefenseLevel = lpTargetObj->Level + lpTargetObj->m_PlayerData->MasterLevel;
	
	switch ( lpTargetObj->Class )
	{
		case CLASS_WIZARD:
			iDefenseRate = ( lpTargetObj->m_PlayerData->Dexterity / 4 ) + 2 * DefenseLevel;
			break;
		case CLASS_KNIGHT:
			iDefenseRate = ( lpTargetObj->m_PlayerData->Dexterity / 2 ) + 2 * DefenseLevel;
			break;
		case CLASS_ELF:
			iDefenseRate = ( lpTargetObj->m_PlayerData->Dexterity / 10 ) + 2 * DefenseLevel;
			break;
		case CLASS_MAGUMSA:
			iDefenseRate = ( lpTargetObj->m_PlayerData->Dexterity / 4 ) + 2 * DefenseLevel;
			break;
		case CLASS_DARKLORD:
			iDefenseRate = ( lpTargetObj->m_PlayerData->Dexterity / 2 ) + 2 * DefenseLevel;
			break;
		case CLASS_SUMMONER:
			iDefenseRate = ( lpTargetObj->m_PlayerData->Dexterity / 2 ) + 2 * DefenseLevel;
			break;
		default:
			iDefenseRate = ( lpTargetObj->m_PlayerData->Dexterity / 4 ) + 2 * DefenseLevel;
			break;
	}

	if ( iAttackRate <= 0.0f || iDefenseRate <= 0.0f || AttackLevel <= 0 || DefenseLevel <= 0 )
		return FALSE;

	iAttackRate += lpObj->m_PlayerData->m_ItemOptionExFor380.OpAddAttackSuccessRatePVP;
	iDefenseRate += lpTargetObj->m_PlayerData->m_ItemOptionExFor380.OpAddDefenseSuccessRatePvP;

	iAttackRate += lpObj->m_PlayerData->m_JewelOfHarmonyEffect.HJOpAddAttackSuccessRatePVP;
	iDefenseRate += lpTargetObj->m_PlayerData->m_JewelOfHarmonyEffect.HJOpAddDefenseSuccessRatePvP;

	iAttackRate += lpObj->m_PlayerData->m_MPSkillOpt.iMpsPVPAttackDmgRate;
	iDefenseRate += lpTargetObj->m_PlayerData->m_MPSkillOpt.iMpsPVPBlockingRate;

	float iExpressionA = ( iAttackRate * 10000.0f ) / ( iAttackRate + iDefenseRate );	// #formula
	float iExpressionB = ( AttackLevel * 10000 ) / ( AttackLevel + DefenseLevel );	// #formula

	iExpressionA /= 10000.0f;
	iExpressionB /= 10000.0f;

	iAttackSuccessRate = 100.0f * iExpressionA * g_fSuccessAttackRateOption * iExpressionB;

	if ( (DefenseLevel - AttackLevel) >= 100 )
	{
		iAttackSuccessRate -= 5;
	}
	else if ( (DefenseLevel - AttackLevel) >= 200 )
	{
		iAttackSuccessRate -= 10;
	}
	else if ( (DefenseLevel - AttackLevel) >= 300 )
	{
		iAttackSuccessRate -= 15;
	}

	DWORD dwRate = rand() % 100;

	if ( dwRate > iAttackSuccessRate )
	{
		GSProtocol.GCDamageSend(lpObj->m_Index, lpTargetObj->m_Index, 0, 0, 0, 0);
		return FALSE;
	}

	return TRUE;
}

	

void CDarkSpirit::ChangeCommand(int command, int targetindex)
{
	CDarkSpirit::ePetItem_Mode setmode = PetItem_Mode_Normal;

	if ( command == PetItem_Mode_Normal )
	{
		setmode = PetItem_Mode_Normal;
	}

	else if ( command == PetItem_Mode_Attack_Random )
	{
		setmode = PetItem_Mode_Attack_Random;
	}

	else if ( command == PetItem_Mode_Attack_WithMaster )
	{
		setmode = PetItem_Mode_Attack_WithMaster;
	}

	else if ( command == PetItem_Mode_Attack_Target )
	{
		setmode = PetItem_Mode_Attack_Target;
	}

	this->SetMode(setmode, targetindex);
}

void __cdecl CDarkSpirit::SendLevelmsg(int aIndex, int nPos, int PetType, int InvenType)
{
	PMSG_SEND_PET_ITEMINFO pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0xA9, sizeof(pMsg));
	pMsg.PetType = PetType;
	pMsg.InvenType = 0xFE;
	pMsg.nPos = nPos;
	pMsg.Level = gObj[aIndex].pInventory[nPos].m_PetItem_Level;

	if (PetType == 1)
	{
		pMsg.Exp = gObj[aIndex].pInventory[nPos].m_PetItem_Exp - gPetItemExp.m_DarkHorseExpTable[pMsg.Level];
	}

	else
	{
		pMsg.Exp = gObj[aIndex].pInventory[nPos].m_PetItem_Exp - gPetItemExp.m_DarkSpiritExpTable[pMsg.Level];
	}

	pMsg.Dur = gObj[aIndex].pInventory[nPos].m_Durability;

	IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}


int CDarkSpirit::GetShieldDamage(LPOBJ lpObj, LPOBJ lpTargetObj, int iAttackDamage)
{
	int iShieldDamage = 0;

	if (g_ShieldSystemOn == FALSE)
		return 0;

	if (iAttackDamage <= 0)
		return 0;

	if (lpObj->Type != OBJ_USER)
		return 0;

	if (lpTargetObj->Type != OBJ_USER)
		return 0;

	int iReduceLife = 0;
	int iReduceShield = 0;
	int iReduceLifeForEffect = 0; 
	bool bReduceShieldGage = 0;
	int iDamageDevideToSDRate = g_iDamageDevideToSDRate;
	iDamageDevideToSDRate -= lpObj->m_PlayerData->m_JewelOfHarmonyEffect.HJOpDecreaseSDRate;
	iDamageDevideToSDRate += lpTargetObj->m_PlayerData->m_JewelOfHarmonyEffect.HJOpAddSDRate;

	if ( iDamageDevideToSDRate < 0 )
		iDamageDevideToSDRate = 0;

	if ( iDamageDevideToSDRate > 100 )
		iDamageDevideToSDRate = 100;

	if ( lpObj->m_PlayerData->m_JewelOfHarmonyEffect.HJOpAddIgnoreSDRate > 0 )
	{
		int iRand = rand() % 100;
		int iIgnoreSDRate = lpObj->m_PlayerData->m_JewelOfHarmonyEffect.HJOpAddIgnoreSDRate - lpTargetObj->m_PlayerData->m_Resistance_SD;

		if (iRand < iIgnoreSDRate)
		{
			iDamageDevideToSDRate = 0;
		}
	}

	if ( (lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER) && ( lpObj->m_PlayerData->m_JewelOfHarmonyEffect.HJOpDecreaseSDRate || lpTargetObj->m_PlayerData->m_JewelOfHarmonyEffect.HJOpAddSDRate || lpObj->m_PlayerData->m_JewelOfHarmonyEffect.HJOpAddIgnoreSDRate ) )
	{
		g_Log.Add("[JewelOfHarmony][PvP System] Attacker:[%s][%s]-SD Decrease[%d] SD Ignore[%d] Defender:[%s][%s] SD Increase Option[%d] - SD Rate[%d]",
			lpObj->AccountID, lpObj->Name,
			lpObj->m_PlayerData->m_JewelOfHarmonyEffect.HJOpDecreaseSDRate, lpObj->m_PlayerData->m_JewelOfHarmonyEffect.HJOpAddIgnoreSDRate, lpTargetObj->AccountID, lpTargetObj->Name, lpTargetObj->m_PlayerData->m_JewelOfHarmonyEffect.HJOpAddSDRate, iDamageDevideToSDRate);
	}

	iReduceShield = iAttackDamage * iDamageDevideToSDRate / 100;
	iReduceLife = iAttackDamage - iReduceShield;

	if ( (lpTargetObj->iShield-iReduceShield) <0 )
	{
		iReduceLife += iReduceShield  - lpTargetObj->iShield;
		iReduceShield = lpTargetObj->iShield;

		if ( lpTargetObj->iShield > 0 )
		{
			bReduceShieldGage = true;
		}
	}

	iReduceLifeForEffect = ( lpTargetObj->MaxLife + lpTargetObj->AddLife ) * 20.0f / 100.0f;

	if ( bReduceShieldGage == true && iReduceLife > iReduceLifeForEffect )
	{
		if ( !CC_MAP_RANGE(lpTargetObj->MapNumber) && lpTargetObj->MapNumber != MAP_INDEX_CHAOSCASTLE_SURVIVAL )
		{
			GSProtocol.GCSendEffectInfo(lpTargetObj->m_Index, 17);
		}
	}

	return iReduceShield;
}


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

