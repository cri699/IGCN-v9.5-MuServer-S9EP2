// TMonsterAIElement.cpp: implementation of the TMonsterAIElement class.
//	GS-N	1.00.18	JPN	0x0055D120	-	Completed
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TMonsterAIElement.h"
#include "TMonsterAIGroup.h"
#include "TMonsterSkillManager.h"
#include "TMonsterSkillUnit.h"
#include "TMonsterAIUtil.h"
#include "CrywolfUtil.h"
#include "KanturuUtil.h"
#include "KanturuMonsterMng.h"
#include "TLog.h"
#include "Gamemain.h"
#include "winutil.h"
#include "BuffEffectSlot.h"
#include "configread.h"

static CKanturuUtil KANTURU_UTIL;

bool TMonsterAIElement::s_bDataLoad = FALSE;
TMonsterAIElement TMonsterAIElement::s_MonsterAIElementArray[MAX_AI_ELEMENT];
TMonsterAIMovePath TMonsterAIElement::s_MonsterAIMovePath[MAX_NUMBER_MAP];

static CCrywolfUtil UTIL;
static TMonsterAIUtil MONSTER_UTIL;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TMonsterAIElement::TMonsterAIElement()
{
	this->Reset();
}

TMonsterAIElement::~TMonsterAIElement()
{

}

void TMonsterAIElement::Reset()
{
	this->m_iElementClass = -1;
	this->m_iElementType = -1;
	this->m_iElementNumber = -1;
	this->m_iSuccessRate = -1;
	this->m_iDelayTime = -1;
	this->m_iTargetType = -1;
	this->m_iX = -1;
	this->m_iY = -1;
	memset(this->m_szElementName, 0, sizeof(this->m_szElementName));
}



BOOL TMonsterAIElement::LoadData(LPSTR lpszFileName)
{
	TMonsterAIElement::s_bDataLoad = FALSE;

	if ( lpszFileName == NULL || strcmp(lpszFileName, "") == 0 )
	{
		g_Log.MsgBox("[Monster AI Element] - File load error : File Name Error");
		return FALSE;
	}

	try
	{
		pugi::xml_document file;
		pugi::xml_parse_result res = file.load_file(lpszFileName);

		if ( res.status != pugi::status_ok )
		{
			g_Log.MsgBox("[Monster AI Element] - Can't Load %s (%s)", lpszFileName, res.description());
			return FALSE;
		}

		TMonsterAIElement::DelAllAIElement();

		pugi::xml_node main = file.child("MonsterAI");

		for (pugi::xml_node element = main.child("Element"); element; element = element.next_sibling())
		{
			char szElementName[50]={0};
			memcpy(szElementName, element.attribute("Name").as_string(), sizeof(szElementName));

			int iElementNumber = element.attribute("Number").as_int();
			int iElementClass = element.attribute("Class").as_int();
			int iElementType = element.attribute("Type").as_int();
			int iSuccessRate = element.attribute("SuccessRate").as_int();
			int iDelayTime = element.attribute("DelayTime").as_int();
			int iTargetType = element.attribute("TargetType").as_int();
			int iX = element.attribute("X").as_int();
			int iY = element.attribute("Y").as_int();

			if ( iElementNumber < 0 || iElementNumber >= MAX_AI_ELEMENT )
			{
				g_Log.MsgBox("[Monster AI Element] - ElementNumber(%d) Error (%s) File. ", iElementNumber, lpszFileName);
				continue;
			}

			if ( iElementClass < 0 || iElementClass >= MAX_AI_ELEMENT_CLASS )
			{
				g_Log.MsgBox("[Monster AI Element] - ElementClass(%d) Error (%s) File. ", iElementClass, lpszFileName);
				continue;
			}

			TMonsterAIElement::s_MonsterAIElementArray[iElementNumber].m_iElementNumber = iElementNumber;
			TMonsterAIElement::s_MonsterAIElementArray[iElementNumber].m_iElementClass = iElementClass;
			TMonsterAIElement::s_MonsterAIElementArray[iElementNumber].m_iElementType = iElementType;
			TMonsterAIElement::s_MonsterAIElementArray[iElementNumber].m_iSuccessRate = iSuccessRate;
			TMonsterAIElement::s_MonsterAIElementArray[iElementNumber].m_iDelayTime = iDelayTime;
			TMonsterAIElement::s_MonsterAIElementArray[iElementNumber].m_iTargetType = iTargetType;
			TMonsterAIElement::s_MonsterAIElementArray[iElementNumber].m_iX = iX;
			TMonsterAIElement::s_MonsterAIElementArray[iElementNumber].m_iY = iY;
			memcpy(TMonsterAIElement::s_MonsterAIElementArray[iElementNumber].m_szElementName, szElementName, sizeof(szElementName));
		}

		g_Log.AddC(TColor::Red, "[Monster AI Element] - %s file is Loaded", lpszFileName);
		TMonsterAIElement::s_bDataLoad = TRUE;
	}

	catch (DWORD)
	{
		g_Log.MsgBox("[Monster AI Element] - Loading Exception Error (%s) File. ", lpszFileName);
	}

	TMonsterAIElement::s_MonsterAIMovePath[MAP_INDEX_CRYWOLF_FIRSTZONE].LoadData(g_ConfigRead.GetPath("\\Monsters\\AI\\IGC_Monster_AI_MovePath.xml"), "CryWolf");
	return FALSE;
}



BOOL TMonsterAIElement::DelAllAIElement()
{
	for ( int i=0;i<MAX_AI_ELEMENT;i++)
	{
		TMonsterAIElement::s_MonsterAIElementArray[i].Reset();
	}

	for ( int j=0;j<MAX_NUMBER_MAP;j++)
	{
		TMonsterAIElement::s_MonsterAIMovePath[j].DelAllAIMonsterMovePath();
	}

	return FALSE;
}



TMonsterAIElement * TMonsterAIElement::FindAIElement(int iElementNumber)
{
	if ( (iElementNumber < 0 && iElementNumber != -1) || iElementNumber >= MAX_AI_ELEMENT )
	{
		return NULL;
	}

	if ( TMonsterAIElement::s_MonsterAIElementArray[iElementNumber].m_iElementNumber == iElementNumber )
	{
		return &TMonsterAIElement::s_MonsterAIElementArray[iElementNumber];
	}
	return NULL;
}



BOOL TMonsterAIElement::ForceAIElement(int iIndex, int iTargetIndex, TMonsterAIState *pAIState)
{
	LPOBJ lpObj = &gObj[iIndex];

	if ( (rand()%100) > this->m_iSuccessRate )
		return FALSE;

	switch ( this->m_iElementType )
	{
		case MAE_TYPE_COMMON_NORMAL:
			this->ApplyElementCommon(iIndex, iTargetIndex, pAIState);
			break;
		case MAE_TYPE_MOVE_NORMAL:
			this->ApplyElementMove(iIndex, iTargetIndex, pAIState);
			break;
		case MAE_TYPE_MOVE_TARGET:
			this->ApplyElementMoveTarget(iIndex, iTargetIndex, pAIState);
			break;
		case MAE_TYPE_GROUP_MOVE:
			this->ApplyElementGroupMove(iIndex, iTargetIndex, pAIState);
			break;
		case MAE_TYPE_ATTACK_NORMAL:
			this->ApplyElementAttack(iIndex, iTargetIndex, pAIState);
			break;
		case MAE_TYPE_ATTACK_AREA:
			this->ApplyElementAttackArea(iIndex, iTargetIndex, pAIState);
			break;
		case MAE_TYPE_ATTACK_PENETRATION:
			this->ApplyElementAttackPenetration(iIndex, iTargetIndex, pAIState);
			break;
		case MAE_TYPE_HEAL_SELF:
			this->ApplyElementHealSelf(iIndex, iTargetIndex, pAIState);
			break;
		case MAE_TYPE_HEAL_GROUP:
			this->ApplyElementHealGroup(iIndex, iTargetIndex, pAIState);
			break;
		case MAE_TYPE_AVOID_NORMAL:
			this->ApplyElementAvoid(iIndex, iTargetIndex, pAIState);
			break;
		/*case MAE_TYPE_HELP_HP:
			this->ApplyElement(iIndex, iTargetIndex, pAIState);
			break;*/
		/*case MAE_TYPE_HELP_BUFF:
			this->ApplyElement(iIndex, iTargetIndex, pAIState);
			break;*/
		/*case MAE_TYPE_HELP_TARGET:
			this->ApplyElement(iIndex, iTargetIndex, pAIState);
			break;*/
		/*case MAE_TYPE_SPECIAL:
			this->ApplyElement(iIndex, iTargetIndex, pAIState);
			break;*/
		case MAE_TYPE_SPECIAL_SOMMON:
			this->ApplyElementSpecialSommon(iIndex, iTargetIndex, pAIState);
			break;
		case MAE_TYPE_SPECIAL_NIGHTMARE_SUMMON:
			this->ApplyElementNightmareSummon(iIndex, iTargetIndex, pAIState);
			break;
		case MAE_TYPE_SPECIAL_WARP:
			this->ApplyElementNightmareWarp(iIndex, iTargetIndex, pAIState);
			break;
		case MAE_TYPE_SPECIAL_SKILLATTACK:
			this->ApplyElementSkillAttack(iIndex, iTargetIndex, pAIState);
			break;
		case MAE_TYPE_SPECIAL_CHANGEAI:
			this->ApplyElementAIChange(iIndex, iTargetIndex, pAIState);
			break;
		/*case MAE_TYPE_EVENT:
			this->ApplyElement(iIndex, iTargetIndex, pAIState);
			break;*/
		case MAE_TYPE_SPECIAL_IMMUNE:
			this->ApplyElementSpecialImmune(iIndex, iTargetIndex, pAIState);
			break;
	}
	return TRUE;
}



BOOL TMonsterAIElement::ApplyElementCommon(int iIndex, int iTargetIndex, TMonsterAIState * pAIState)
{
	return FALSE;
}

BOOL TMonsterAIElement::ApplyElementMove(int iIndex, int iTargetIndex, TMonsterAIState * pAIState)
{
	LPOBJ lpObj = &gObj[iIndex];
	UTIL.SendCrywolfChattingMsg(iIndex, "Element-이동");

	if ( lpObj->PathStartEnd )
		return FALSE;

	BOOL bFindXY = FALSE;

	if ( pAIState->m_iTransitionType == 2 )
		bFindXY = MONSTER_UTIL.GetXYToChase(lpObj);
	else
		bFindXY = MONSTER_UTIL.GetXYToPatrol(lpObj);

	if ( bFindXY )
		MONSTER_UTIL.FindPathToMoveMonster(lpObj, lpObj->MTX, lpObj->MTY, 5, 1);

	return FALSE;
}

BOOL TMonsterAIElement::ApplyElementMoveTarget(int iIndex, int iTargetIndex, TMonsterAIState * pAIState)
{
	LPOBJ lpObj = &gObj[iIndex];
	UTIL.SendCrywolfChattingMsg(iIndex, "Element-타겟이동");

	if ( lpObj->PathStartEnd )
		return FALSE;

	if ( lpObj->X == this->m_iX &&
		 lpObj->Y == this->m_iY )
	{
		this->ApplyElementMove(iIndex, iTargetIndex, pAIState);
		return FALSE;
	}

	BOOL bFindXY = TRUE;
	int iTargetX = this->m_iX;
	int iTargetY = this->m_iY;
	int iTargetDistance = sqrt( static_cast<float>( ((lpObj->X - iTargetX)*(lpObj->X - iTargetX))+ ((lpObj->Y - iTargetY)*(lpObj->Y - iTargetY)) ));
	
	if ( TMonsterAIElement::s_MonsterAIMovePath[lpObj->MapNumber].m_bDataLoad )
	{
		if ( iTargetDistance > 10 )
		{
			int iMinCost = 1000000;
			int iMidX = -1;
			int iMidY = -1;
			int iSpotNum = -1;

			for ( int i=0;i<MAX_MONSTER_AI_MOVE_PATH;i++)
			{
				TMonsterAIMovePathInfo & PathInfo = TMonsterAIElement::s_MonsterAIMovePath[MAP_INDEX_CRYWOLF_FIRSTZONE].m_MovePathInfo[i];
				float fDistX = lpObj->X - PathInfo.m_iPathX;
				float fDistY = lpObj->Y - PathInfo.m_iPathY;
				int iPathSpotDist =  sqrt( (fDistX*fDistX) + (fDistY*fDistY) );

				if ( iPathSpotDist < 20 )
				{
					fDistX = iTargetX - PathInfo.m_iPathX;
					fDistY = iTargetY - PathInfo.m_iPathY;
					int iMidDist = sqrt( (fDistX*fDistX) + (fDistY*fDistY) );

					if ( iMinCost > iMidDist )
					{
						if ( iMidDist )
						{
							iMinCost = iMidDist;
							iMidX = PathInfo.m_iPathX;
							iMidY = PathInfo.m_iPathY;
							iSpotNum = i;
						}
					}
				}
			}

			if ( iMinCost != 1000000 )
			{
				iTargetX = iMidX;
				iTargetY = iMidY;
			}
		}
	}

	if ( bFindXY )
	{
		if ( MONSTER_UTIL.FindPathToMoveMonster(lpObj, iTargetX, iTargetY, 7, FALSE) )
			lpObj->PathStartEnd = 1;
		else
			lpObj->PathStartEnd = 0;
	}

	return FALSE;
}


BOOL TMonsterAIElement::ApplyElementGroupMove(int iIndex, int iTargetIndex, TMonsterAIState * pAIState)
{
	LPOBJ lpObj = &gObj[iIndex];
	UTIL.SendCrywolfChattingMsg(iIndex, "Element-그룹이동");

	if ( lpObj->PathStartEnd )
		return FALSE;

	BOOL bFindXY = FALSE;
	BOOL bFoundLeader = TRUE;
	int iLeaderIndex = -1;
	iLeaderIndex = TMonsterAIGroup::FindGroupLeader(lpObj->m_iGroupNumber);

	if ( iLeaderIndex == -1 || gObj[iLeaderIndex].Live == FALSE )
		bFoundLeader = FALSE;

	if ( bFoundLeader && gObjCalDistance(lpObj, &gObj[iLeaderIndex]) > 6 )
	{
		lpObj->TargetNumber = iLeaderIndex;
		bFindXY = MONSTER_UTIL.GetXYToChase(lpObj);
	}
	else if ( pAIState->m_iTransitionType == 2 )
	{
		bFindXY = MONSTER_UTIL.GetXYToChase(lpObj);
	}
	else
	{
		bFindXY = MONSTER_UTIL.GetXYToPatrol(lpObj);
	}

	if ( bFindXY )
	{
		if ( MONSTER_UTIL.FindPathToMoveMonster(lpObj, lpObj->MTX, lpObj->MTY, 5, TRUE) )
		{

		}
		else
		{
			MONSTER_UTIL.GetXYToPatrol(lpObj);
			MONSTER_UTIL.FindPathToMoveMonster(lpObj, lpObj->MTX, lpObj->MTY, 5, TRUE);
		}
	}

	return FALSE;
}

BOOL TMonsterAIElement::ApplyElementAttack(int iIndex, int iTargetIndex, TMonsterAIState * pAIState)
{
	LPOBJ lpObj = &gObj[iIndex];
	UTIL.SendCrywolfChattingMsg(iIndex, "Element-공격");

	if ( TMonsterSkillManager::CheckMonsterSkill(lpObj->Class) && lpObj->Class != 519 ) // Medic has only BUFF in MONSTERSKILL, so disable check for him
	{
		BOOL bEnableAttack = TRUE;
		int iTargetNumber = lpObj->TargetNumber;

		if ( iTargetNumber < 0 )
			bEnableAttack = FALSE;

		else if ( !gObj[iTargetNumber].Live || gObj[iTargetNumber].Teleport )
			bEnableAttack = FALSE;

		else if ( gObj[iTargetNumber].Connected <= PLAYER_LOGGED ||
			 gObj[iTargetNumber].CloseCount != -1 )
		{
			bEnableAttack = FALSE;
		}

		if ( !bEnableAttack )
		{
			lpObj->TargetNumber = -1;
			lpObj->m_ActState.Emotion = 0;
			lpObj->m_ActState.Attack = 0;
			lpObj->m_ActState.Move = 0;
			lpObj->NextActionTime = 1000;

			return FALSE;
		}

		LPOBJ lpTargetObj = &gObj[iTargetNumber];
		lpObj->Dir = GetPathPacketDirPos(lpTargetObj->X - lpObj->X, lpTargetObj->Y - lpObj->Y);

		if ( (rand()%4) == 0 )
		{
			PMSG_ATTACK pAttackMsg;

			pAttackMsg.AttackAction = 0x78;
			pAttackMsg.DirDis = lpObj->Dir;
			pAttackMsg.NumberH = SET_NUMBERH(iTargetNumber);
			pAttackMsg.NumberL = SET_NUMBERL(iTargetNumber);

			GSProtocol.GCActionSend(lpObj, 0x78, lpObj->m_Index, iTargetNumber);
			gObjAttack(lpObj, &gObj[iTargetNumber], 0, 0, 0, 0, 0, 0, 0);
		}
		else
		{
			TMonsterSkillManager::UseMonsterSkill(lpObj->m_Index, iTargetNumber, 0, -1, NULL);
		}

		lpObj->m_ActState.Attack = 0;
		return FALSE;
	}
	else
	{
		int iTargetNumber = lpObj->TargetNumber;

		if (!ObjectMaxRange(iTargetNumber))
		{
			return FALSE;
		}

		LPOBJ lpTargetObj = &gObj[iTargetNumber];
		lpObj->Dir = GetPathPacketDirPos(lpTargetObj->X - lpObj->X, lpTargetObj->Y - lpObj->Y);

		PMSG_ATTACK pAttackMsg;

		pAttackMsg.AttackAction = 0x78;
		pAttackMsg.DirDis = lpObj->Dir;
		pAttackMsg.NumberH = SET_NUMBERH(iTargetNumber);
		pAttackMsg.NumberL = SET_NUMBERL(iTargetNumber);

		GSProtocol.CGAttack((PMSG_ATTACK *)&pAttackMsg, lpObj->m_Index);
		GSProtocol.GCActionSend(lpObj, 0x78, lpObj->m_Index, lpTargetObj->m_Index);
		gObjAttack(lpObj, &gObj[iTargetNumber], 0, 0, 0, 0, 0, 0, 0);

		return FALSE;
	}
}

struct PMSG_NOTIFY_REGION_MONSTER_ATTACK
{
	PBMSG_HEAD2 h;	// C1:BD:0C
	BYTE btObjClassH;	// 4
	BYTE btObjClassL;	// 5
	BYTE btSourceX;	// 6
	BYTE btSourceY;	// 7
	BYTE btPointX;	// 8
	BYTE btPointY;	// 9
};


BOOL TMonsterAIElement::ApplyElementAttackArea(int iIndex, int iTargetIndex, TMonsterAIState * pAIState)
{
	LPOBJ lpObj = &gObj[iIndex];
	UTIL.SendCrywolfChattingMsg(iIndex, "Element-영역공격");

	int iTargetX = this->m_iX + (rand()%5) * ((rand()%2==0)?1:-1 ) ;
	int iTargetY = this->m_iY + (rand()%5) * ((rand()%2==0)?1:-1 ) ;

	for (int i= g_ConfigRead.server.GetObjectStartUserIndex();i<g_ConfigRead.server.GetObjectMax();i++)
	{
		if ( !gObjIsConnected(i))
			continue;

		LPOBJ lpTargetObj = &gObj[i];

		if ( !lpObj->Live )
			continue;

		if ( lpObj->MapNumber != lpTargetObj->MapNumber )
			continue;

		int iTargetDistance = sqrt( static_cast<float> ( ((lpTargetObj->X - iTargetX)*(lpTargetObj->X - iTargetX)) + ((lpTargetObj->Y - iTargetY)*(lpTargetObj->Y - iTargetY)) ) );

		if ( iTargetDistance < 10 )
		{
			PMSG_NOTIFY_REGION_MONSTER_ATTACK pMsg;

			PHeadSubSetB((LPBYTE)&pMsg, 0xBD, 0x0C, sizeof(pMsg));
			pMsg.btObjClassH = SET_NUMBERH(lpObj->Class);
			pMsg.btObjClassL = SET_NUMBERL(lpObj->Class);
			pMsg.btSourceX = lpObj->X;
			pMsg.btSourceY = lpObj->Y;
			pMsg.btPointX = iTargetX;
			pMsg.btPointY = iTargetY;

			IOCP.DataSend(lpTargetObj->m_Index, (LPBYTE)&pMsg, sizeof(pMsg));
		}

		if ( iTargetDistance < 6 )
		{
			gObjAttack(lpObj, lpTargetObj, 0, 0, 0, 0, 0, 0, 0);
		}
	}

	return FALSE;
}

BOOL TMonsterAIElement::ApplyElementAttackPenetration(int iIndex, int iTargetIndex, TMonsterAIState * pAIState)
{
	LPOBJ lpObj = &gObj[iIndex];
	iTargetIndex = lpObj->TargetNumber;

	if ( iTargetIndex == -1 )
		return FALSE;

	if ( gObj[iTargetIndex].Live == 0 )
		return FALSE;

	TMonsterSkillManager::UseMonsterSkill(iIndex, iTargetIndex, 2, -1, NULL);
	return FALSE;
}


BOOL TMonsterAIElement::ApplyElementAvoid(int iIndex, int iTargetIndex, TMonsterAIState * pAIState)
{
	LPOBJ lpObj = &gObj[iIndex];
	UTIL.SendCrywolfChattingMsg(iIndex, "Element-회피");

	BOOL bFindXY = MONSTER_UTIL.GetXYToEascape(lpObj);

	if ( bFindXY )
	{
		MONSTER_UTIL.FindPathToMoveMonster(lpObj, lpObj->MTX, lpObj->MTY, 5, 1);
	}

	return FALSE;
}


BOOL TMonsterAIElement::ApplyElementHealSelf(int iIndex, int iTargetIndex, TMonsterAIState * pAIState)
{
	LPOBJ lpObj = &gObj[iIndex];
	UTIL.SendCrywolfChattingMsg(iIndex, "Element-셀프치료");

	lpObj->Life += lpObj->Life * 20.0f / 100.0f;
	UTIL.SendCrywolfChattingMsg(iIndex, "HP : %d", (int)lpObj->Life);
	//lpObj->m_ViewSkillState |= 8;
	//GCStateInfoSend(lpObj, 1, lpObj->m_ViewSkillState);

	return FALSE;
}


BOOL TMonsterAIElement::ApplyElementHealGroup(int iIndex, int iTargetIndex, TMonsterAIState * pAIState)
{
	LPOBJ lpObj = &gObj[iIndex];
	UTIL.SendCrywolfChattingMsg(iIndex, "Element-그룹치료");
	TMonsterAIGroupMember * pMemb = TMonsterAIGroup::FindGroupMemberToHeal(lpObj->m_Index, lpObj->m_iGroupNumber, lpObj->m_iGroupMemberGuid, 6);

	if ( pMemb )
	{
		LPOBJ lpTargetObj = &gObj[pMemb->m_iObjIndex];

		if ( lpTargetObj->Live == 0 )
			return FALSE;

		TMonsterSkillUnit * lpSkillUnit = NULL;

		if(lpObj->Class == 519)
		{
			TMonsterSkillManager::UseMonsterSkill(iIndex, pMemb->m_iObjIndex, 44, -1, NULL);
		}
		else
		{
			lpSkillUnit = TMonsterSkillManager::FindMonsterSkillUnit(lpObj->m_Index, 21);
		}

		if ( lpSkillUnit )
		{
			lpSkillUnit->RunSkill(iIndex, lpTargetObj->m_Index);
		}

		UTIL.SendCrywolfChattingMsg(iIndex, "그룹치료 HP : %d", (int)lpTargetObj->Life);
		UTIL.SendCrywolfChattingMsg(lpTargetObj->m_Index, "HP : %d", (int)lpTargetObj->Life);
	}

	return FALSE;
}


BOOL TMonsterAIElement::ApplyElementSpecialSommon(int iIndex, int iTargetIndex, TMonsterAIState * pAIState)
{
	LPOBJ lpObj = &gObj[iIndex];
	UTIL.SendCrywolfChattingMsg(iIndex, "Element-특수소환");
	TMonsterAIGroupMember * pMemb = TMonsterAIGroup::FindGroupMemberToSommon(lpObj->m_Index, lpObj->m_iGroupNumber, lpObj->m_iGroupMemberGuid);

	if ( pMemb )
	{
		LPOBJ lpTargetObj = &gObj[pMemb->m_iObjIndex];

		if ( lpTargetObj->Live != 0 )
			return FALSE;

		TMonsterSkillUnit * lpSkillUnit = TMonsterSkillManager::FindMonsterSkillUnit(lpObj->m_Index, 30);

		if ( lpSkillUnit )
		{
			lpSkillUnit->RunSkill(iIndex, lpTargetObj->m_Index);
		}
	}

	return FALSE;
}

BOOL TMonsterAIElement::ApplyElementSpecialImmune(int iIndex, int iTargetIndex, TMonsterAIState * pAIState)
{
	LPOBJ lpObj = &gObj[iIndex];
	
	this->m_iX = 10;
	this->m_iY = 10;

	gObjAddBuffEffect(lpObj, BUFFTYPE_MONSTER_MAGIC_IMMUNE, 0, 0, 0, 0, 10);
	gObjAddBuffEffect(lpObj, BUFFTYPE_MONSTER_MELEE_IMMUNE, 0, 0, 0, 0, 10);

	return FALSE;
}

BOOL TMonsterAIElement::ApplyElementNightmareSummon(int iIndex, int iTargetIndex, TMonsterAIState * pAIState)
{
	LPOBJ lpObj = &gObj[iIndex];

	if ( lpObj->TargetNumber == -1 )
		iTargetIndex = lpObj->m_Index;

	iTargetIndex = lpObj->TargetNumber;

	if (iTargetIndex == -1)
	{
		return FALSE;
	}

	TMonsterSkillUnit * lpSkillUnit = TMonsterSkillManager::FindMonsterSkillUnit(lpObj->m_Index, 30); 

	if (lpSkillUnit)
	{
		GSProtocol.GCUseMonsterSkillSend(&gObj[iIndex], &gObj[iTargetIndex], lpSkillUnit->m_iUnitNumber);

		if (g_ConfigRead.server.GetServerType() != SERVER_CASTLE)
		{
			int iRegenMonster = g_KanturuMonsterMng.SetKanturuMonster(6);
		}
	}

	return FALSE;
}

BOOL TMonsterAIElement::ApplyElementNightmareWarp(int iIndex, int iTargetIndex, TMonsterAIState * pAIState)
{
	LPOBJ lpObj = &gObj[iIndex];

	BYTE x = this->m_iX;
	BYTE y = this->m_iY;

	PMSG_MAGICATTACK_RESULT pAttack;

	PHeadSetBE((unsigned char *)&pAttack,0x19,sizeof(pAttack));
	WORD MagicNumber = 6;
	pAttack.MagicNumberH = HIBYTE(MagicNumber);
	pAttack.MagicNumberL = LOBYTE(MagicNumber);
	pAttack.SourceNumberH = SET_NUMBERH(iIndex);
	pAttack.SourceNumberL = SET_NUMBERL(iIndex);
	pAttack.TargetNumberH = SET_NUMBERH(iIndex);
	pAttack.TargetNumberL = SET_NUMBERL(iIndex);

	if ( lpObj->Type == OBJ_USER )
		IOCP.DataSend(iIndex,(unsigned char *)&pAttack,pAttack.h.size);

	GSProtocol.MsgSendV2(lpObj,(unsigned char*)&pAttack,pAttack.h.size);

	g_Log.AddC(TColor::Pink,  "[%s] Warp [%d,%d] -> [%d,%d]",
		lpObj->Name, lpObj->X, lpObj->Y, x, y);

	gObjTeleportMagicUse(iIndex,x,y);
	lpObj->TargetNumber = -1;

	return FALSE;
}



BOOL TMonsterAIElement::ApplyElementSkillAttack(int iIndex, int iTargetIndex, TMonsterAIState * pAIState)
{
	LPOBJ lpObj = &gObj[iIndex];

	if ( TMonsterSkillManager::CheckMonsterSkill(lpObj->Class) )
	{
		BOOL bEnableAttack = TRUE;
		int iTargetNumber = lpObj->TargetNumber;

		if (iTargetNumber < 0 )
			bEnableAttack = FALSE;

		else if ( !gObj[iTargetNumber].Live || gObj[iTargetNumber].Teleport )
			bEnableAttack = FALSE;

		else if ( gObj[iTargetNumber].Connected <= PLAYER_LOGGED ||
			 gObj[iTargetNumber].CloseCount != -1 )
		{
			bEnableAttack = FALSE;
		}

		if ( !bEnableAttack )
		{
			lpObj->TargetNumber = -1;
			lpObj->m_ActState.Emotion = 0;
			lpObj->m_ActState.Attack = 0;
			lpObj->m_ActState.Move = 0;
			lpObj->NextActionTime = 1000;

			return FALSE;
		}

		LPOBJ lpTargetObj = &gObj[iTargetNumber];
		lpObj->Dir = GetPathPacketDirPos(lpTargetObj->X - lpObj->X, lpTargetObj->Y - lpObj->Y);
		int iRate1 = this->m_iTargetType;
		int iRate2 = this->m_iX;
		int iRate3 = this->m_iY;
		int iRandom = rand() % 100;

		if ( iRandom < iRate1 )
			TMonsterSkillManager::UseMonsterSkill(lpObj->m_Index, iTargetNumber, 0, -1, NULL);
		else if ( iRandom < (iRate1+iRate2) )
			TMonsterSkillManager::UseMonsterSkill(lpObj->m_Index, iTargetNumber, 1, -1, NULL);
		else if ( iRandom < (iRate1+iRate2+iRate3) )
			TMonsterSkillManager::UseMonsterSkill(lpObj->m_Index, iTargetNumber, 2, -1, NULL);

		lpObj->m_ActState.Attack = 0;
		return FALSE;
	}

	return FALSE;
}
		

BOOL TMonsterAIElement::ApplyElementAIChange(int iIndex, int iTargetIndex, TMonsterAIState * pAIState)
{
	TMonsterAIGroup::ChangeAIOrder(this->m_iTargetType, this->m_iX);
	g_Log.AddC(TColor::Red,  "[AI Change] Group %d AI Order %d",
		this->m_iTargetType, this->m_iX);

	return FALSE;
}


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

