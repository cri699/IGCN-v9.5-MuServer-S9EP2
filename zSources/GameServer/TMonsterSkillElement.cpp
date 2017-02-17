// TMonsterSkillElement.cpp: implementation of the TMonsterSkillElement class.
/*	GS-N	1.00.18	JPN	0x00554F40	-	Completed
		BOOL TMonsterSkillElement::ApplyElementHP(int iIndex, int iTargetIndex)	-	Wrong compilation with same result
		BOOL TMonsterSkillElement::ApplyElementMP(int iIndex, int iTargetIndex)	-	Wrong compilation with same result*/
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TMonsterSkillElement.h"
#include "ObjUseSkill.h"
#include "protocol.h"
#include "gObjMonster.h"
#include "Gamemain.h"
#include "TLog.h"
#include "winutil.h"
#include "BuffEffect.h"
#include "BuffEffectSlot.h"
#include "LuckyItemManager.h"

BOOL TMonsterSkillElement::s_bDataLoad = FALSE;
TMonsterSkillElement TMonsterSkillElement::s_MonsterSkillElementArray[MAX_MONSTER_SKILL_ELEMENT];

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TMonsterSkillElement::TMonsterSkillElement()
{
	this->Reset();
}

TMonsterSkillElement::~TMonsterSkillElement()
{
	return;
}

void TMonsterSkillElement::Reset()
{
	this->m_iSuccessRate = -1;
	this->m_iContinuanceTime = -1;
	this->m_iIncAndDecType = -1;
	this->m_iIncAndDecValue = -1;
	this->m_iNullifiedSkill = -1;
	this->m_iCharacterClass = -1;
	this->m_iCharacterLevel = -1;
	this->m_iElementType = -1;
	this->m_iElementNumber = -1;
	memset(this->m_szElementName, 0, sizeof(this->m_szElementName));
}

BOOL TMonsterSkillElement::LoadData(LPSTR lpszFileName)
{	
	TMonsterSkillElement::s_bDataLoad = FALSE;

	if ( !lpszFileName || !strcmp(lpszFileName, ""))
	{
		g_Log.MsgBox("[Monster Element] - File load error : File Name Error");
		return FALSE;
	}

	try
	{
		pugi::xml_document file;
		pugi::xml_parse_result res = file.load_file(lpszFileName);

		if ( res.status != pugi::status_ok )
		{
			g_Log.MsgBox("[Monster Element] - Can't Load %s (%s)", lpszFileName, res.description());
			return FALSE;
		}

		TMonsterSkillElement::DelAllSkillElement();

		pugi::xml_node main = file.child("MonsterSkill");

		for (pugi::xml_node element = main.child("Element"); element; element = element.next_sibling())
		{
			char szElementName[50] = {0};
			memcpy(szElementName, element.attribute("Name").as_string(), sizeof(szElementName));

			int iElementNumber = element.attribute("Number").as_int();						
			int iElementType = element.attribute("Type").as_int();
			int iSuccessRate = element.attribute("SuccessRate").as_int();
			int iContinuanceTime = element.attribute("ContinuanceTime").as_int();
			int iIncAndDecType = element.attribute("IncAndDecType").as_int();
			int iIncAndDecValue = element.attribute("IncAndDecValue").as_int();
			int iNullifiedMagic = element.attribute("NullifiedMagic").as_int();
			int iCharacterClass = element.attribute("CharacterClass").as_int();
			int iCharacterLevel = element.attribute("CharacterLevel").as_int();

			if ( iElementNumber < 0 || iElementNumber >= MAX_MONSTER_SKILL_ELEMENT )
			{
				g_Log.MsgBox("[Monster Element] - ElementNumber(%d) Error (%s) File. ", iElementNumber, lpszFileName);
				continue;
			}

			TMonsterSkillElement::s_MonsterSkillElementArray[iElementNumber].m_iSuccessRate = iSuccessRate;
			TMonsterSkillElement::s_MonsterSkillElementArray[iElementNumber].m_iContinuanceTime = iContinuanceTime;
			TMonsterSkillElement::s_MonsterSkillElementArray[iElementNumber].m_iIncAndDecType = iIncAndDecType;
			TMonsterSkillElement::s_MonsterSkillElementArray[iElementNumber].m_iIncAndDecValue = iIncAndDecValue;
			TMonsterSkillElement::s_MonsterSkillElementArray[iElementNumber].m_iNullifiedSkill = iNullifiedMagic;
			TMonsterSkillElement::s_MonsterSkillElementArray[iElementNumber].m_iCharacterClass = iCharacterClass;
			TMonsterSkillElement::s_MonsterSkillElementArray[iElementNumber].m_iCharacterLevel = iCharacterLevel;
			TMonsterSkillElement::s_MonsterSkillElementArray[iElementNumber].m_iElementType = iElementType;
			TMonsterSkillElement::s_MonsterSkillElementArray[iElementNumber].m_iElementNumber = iElementNumber;
			memcpy(TMonsterSkillElement::s_MonsterSkillElementArray[iElementNumber].m_szElementName, szElementName, sizeof(TMonsterSkillElement::s_MonsterSkillElementArray[iElementNumber].m_szElementName));
		}

		g_Log.AddC(TColor::Red,  "[Monster Element] - %s file is Loaded", lpszFileName);
		TMonsterSkillElement::s_bDataLoad = TRUE;
	}

	catch (DWORD)
	{
		g_Log.MsgBox("[Monster Element] - Loading Exception Error (%s) File. ", lpszFileName);	
	}

	return FALSE;
}

BOOL TMonsterSkillElement::DelAllSkillElement()
{
	for(int i=0;i<MAX_MONSTER_SKILL_ELEMENT;i++)
	{
		TMonsterSkillElement::s_MonsterSkillElementArray[i].Reset();
	}

	return FALSE;
}

TMonsterSkillElement * TMonsterSkillElement::FindSkillElement(int iElementNumber)
{
	if ( iElementNumber < 0 || iElementNumber >= MAX_MONSTER_SKILL_ELEMENT )
	{
		g_Log.Add("[Monster Element] FindSkillElement() Error - (ElementNumber=%d) ",
			iElementNumber);

		return NULL;
	}

	if ( TMonsterSkillElement::s_MonsterSkillElementArray[iElementNumber].m_iElementNumber == iElementNumber )
	{
		return &TMonsterSkillElement::s_MonsterSkillElementArray[iElementNumber];
	}

	g_Log.Add("[Monster Element] FindSkillElement() Error - (ElementNumber=%d) ",
		iElementNumber);

	return  NULL;
}


void TMonsterSkillElement::ForceSkillElement(int iIndex, int iTargetIndex)
{
	int iSuccessRate = this->m_iSuccessRate;

	if (this->m_iElementType == MSE_TYPE_STUN && gObj[iTargetIndex].Type == OBJ_USER)
	{
		iSuccessRate -= gObj[iTargetIndex].m_PlayerData->m_Resistance_Stun;
	}

	if ( (rand()%100) > iSuccessRate )
		return;

	switch ( this->m_iElementType )
	{
		case MSE_TYPE_STUN:
			this->ApplyElementStun(iIndex, iTargetIndex);
			break;
		case MSE_TYPE_MOVE:
			this->ApplyElementMove(iIndex, iTargetIndex);
			break;
		case MSE_TYPE_HP:
			this->ApplyElementHP(iIndex, iTargetIndex);
			break;
		case MSE_TYPE_MP:
			this->ApplyElementMP(iIndex, iTargetIndex);
			break;
		case MSE_TYPE_AG:
			this->ApplyElementAG(iIndex, iTargetIndex);
			break;
		case MSE_TYPE_DEFENSE:
			this->ApplyElementDefense(iIndex, iTargetIndex);
			break;
		case MSE_TYPE_ATTACK:
			this->ApplyElementAttack(iIndex, iTargetIndex);
			break;
		case MSE_TYPE_DURABILITY:
			this->ApplyElementDurability(iIndex, iTargetIndex);
			break;
		case MSE_TYPE_SUMMON:
			this->ApplyElementSummon(iIndex, iTargetIndex);
			break;
		case MSE_TYPE_PUSH:
			this->ApplyElementPush(iIndex, iTargetIndex);
			break;
		case MSE_TYPE_STAT_ENERGY:
			this->ApplyElementStatEnergy(iIndex, iTargetIndex);
			break;
		case MSE_TYPE_STAT_STRENGTH:
			this->ApplyElementStatStrength(iIndex, iTargetIndex);
			break;
		case MSE_TYPE_STAT_DEXITERITY:
			this->ApplyElementStatDexiterity(iIndex, iTargetIndex);
			break;
		case MSE_TYPE_STAT_VITALITY:
			this->ApplyElementStatVitality(iIndex, iTargetIndex);
			break;
		case MSE_TYPE_REMOVE_SKILL:
			this->ApplyElementRemoveSkill(iIndex, iTargetIndex);
			break;
		case MSE_TYPE_RESIST_SKILL:
			this->ApplyElementResistSkill(iIndex, iTargetIndex);
			break;
		case MSE_TYPE_IMMUNE_SKILL:
			this->ApplyElementImmuneSkill(iIndex, iTargetIndex);
			break;
		case MSE_TYPE_TELEPORT_SKILL:
			this->ApplyElementTeleportSkill(iIndex, iTargetIndex);
			break;
		case MSE_TYPE_DOUBLE_HP:
			this->ApplyElementDoubleHP(iIndex, iTargetIndex);
			break;
		case MSE_TYPE_POISON:
			this->ApplyElementPoison(iIndex, iTargetIndex);
			break;
		case MSE_TYPE_NORMALATTACK:
			this->ApplyElementNormalAttack(iIndex, iTargetIndex);
			break;
		case MSE_TYPE_IMMUNE_ALL_SKILL:
			this->ApplyElementImmuneAllSkill(iIndex, iTargetIndex);
			break;
		case MSE_TYPE_PERCENT_DAMAGE:
			this->ApplyElementPercentDamageNormalAttack(iIndex, iTargetIndex);
			break;
		default:
			g_Log.AddC(TColor::Red,  "[TMonsterSkillElement] ForceSkillElement(): m_iElementType unknown value: %d", this->m_iElementType);
			break;
	}
}


BOOL TMonsterSkillElement::ApplyElementStun(int iIndex, int iTargetIndex)
{
	LPOBJ lpObj = &gObj[iIndex];
	LPOBJ lpTargetObj = &gObj[iTargetIndex];

	if(this->m_iContinuanceTime > 0)
	{
		gObjAddBuffEffect(lpTargetObj, BUFFTYPE_STUN, 0, 0, 0, 0, 3);
		gObjSetPosition(iTargetIndex, lpTargetObj->X, lpTargetObj->Y);
	}

	return FALSE;
}


BOOL TMonsterSkillElement::ApplyElementMove(int iIndex, int iTargetIndex)
{
	LPOBJ lpObj = &gObj[iIndex];
	LPOBJ lpTargetObj = &gObj[iTargetIndex];

	return FALSE;
}



BOOL TMonsterSkillElement::ApplyElementHP(int iIndex, int iTargetIndex)
{
	LPOBJ lpObj = &gObj[iIndex];
	LPOBJ lpTargetObj = &gObj[iTargetIndex];
	int iIncDecValue = 0;

	if ( this->m_iIncAndDecType != MSE_INCDEC_TYPE_NONE )
	{
		if ( this->m_iIncAndDecType == MSE_INCDEC_TYPE_PERCENTINC ||
			 this->m_iIncAndDecType == MSE_INCDEC_TYPE_PERCENTDEC ||
			 this->m_iIncAndDecType == MSE_INCDEC_TYPE_CYCLE_PERCENTINC ||
			 this->m_iIncAndDecType == MSE_INCDEC_TYPE_CYCLE_PERCENTDEC )
		{
			iIncDecValue = (lpTargetObj->Life * this->m_iIncAndDecValue) / 100.0f;

			if ( this->m_iIncAndDecType == MSE_INCDEC_TYPE_PERCENTDEC )	// #error LACKING THE OTHER NEGATEVE???
				iIncDecValue = -iIncDecValue;
		}

		if ( this->m_iIncAndDecType == MSE_INCDEC_TYPE_CONSTANTINC ||
			 this->m_iIncAndDecType == MSE_INCDEC_TYPE_CONSTANTDEC ||
			 this->m_iIncAndDecType == MSE_INCDEC_TYPE_CYCLE_CONSTANTINC ||
			 this->m_iIncAndDecType == MSE_INCDEC_TYPE_CYCLE_CONSTANTDEC )
		{
			iIncDecValue = this->m_iIncAndDecValue;

			if ( this->m_iIncAndDecType == MSE_INCDEC_TYPE_CONSTANTDEC ||
				 this->m_iIncAndDecType == MSE_INCDEC_TYPE_CYCLE_CONSTANTDEC )
				iIncDecValue = -iIncDecValue;
		}

		if ( this->m_iIncAndDecType > MSE_INCDEC_TYPE_CYCLE_PERCENT )
		{
			lpTargetObj->m_MonsterSkillElementInfo.m_iSkillElementAutoHPTime = this->m_iContinuanceTime;
			lpTargetObj->m_MonsterSkillElementInfo.m_iSkillElementAutoHP = iIncDecValue;
			lpTargetObj->m_MonsterSkillElementInfo.m_iSkillElementAutoHPCycle = MSE_INCDEC_TYPE_CYCLE_PERCENT;

			if ( this->m_iIncAndDecType > MSE_INCDEC_TYPE_CYCLE_CONSTANT )
				lpTargetObj->m_MonsterSkillElementInfo.m_iSkillElementAutoHPCycle = MSE_INCDEC_TYPE_CYCLE_CONSTANT;
			else
				lpTargetObj->m_MonsterSkillElementInfo.m_iSkillElementAutoHPCycle = MSE_INCDEC_TYPE_CYCLE_PERCENT;
		}
	}

	g_Log.AddC(TColor::Red,  "(%s) used Heal: (%f) + (%d) -> (%f)", lpTargetObj->Name, lpTargetObj->Life, iIncDecValue, lpTargetObj->Life+iIncDecValue);

	lpTargetObj->Life += iIncDecValue;

	if ( lpTargetObj->Life < 0 )
	{
		lpTargetObj->Life = 0;
		GSProtocol.GCReFillSend(lpTargetObj->m_Index, lpTargetObj->Life, 0xFF, 0, lpTargetObj->iShield);

		return TRUE;
	}

	if ( (lpTargetObj->MaxLife + lpTargetObj->AddLife) < lpTargetObj->Life )
	{
		lpTargetObj->Life = lpTargetObj->MaxLife + lpTargetObj->AddLife;
		GSProtocol.GCReFillSend(lpTargetObj->m_Index, lpTargetObj->Life, 0xFF, 0, lpTargetObj->iShield);

		return TRUE;
	}

	GSProtocol.GCReFillSend(lpTargetObj->m_Index, lpTargetObj->Life, 0xFF, 0, lpTargetObj->iShield);

	return FALSE;
}


BOOL TMonsterSkillElement::ApplyElementMP(int iIndex, int iTargetIndex)
{
	LPOBJ lpObj = &gObj[iIndex];
	LPOBJ lpTargetObj = &gObj[iTargetIndex];
	int iIncDecValue = 0;

	if ( this->m_iIncAndDecType != MSE_INCDEC_TYPE_NONE )
	{
		if ( this->m_iIncAndDecType < MSE_INCDEC_TYPE_CONSTANTINC )
		{
			iIncDecValue = lpTargetObj->Mana * this->m_iIncAndDecValue / 100.0f;

			if ( this->m_iIncAndDecType == MSE_INCDEC_TYPE_PERCENTDEC )
				iIncDecValue = -iIncDecValue;
		}

		if ( this->m_iIncAndDecType >= MSE_INCDEC_TYPE_CONSTANTINC )
		{
			iIncDecValue = this->m_iIncAndDecValue;

			if ( this->m_iIncAndDecType == MSE_INCDEC_TYPE_CONSTANTDEC )
				iIncDecValue = -iIncDecValue;
		}
	}

	lpTargetObj->Mana += iIncDecValue;

	if ( lpTargetObj->Mana < 0 )
	{
		lpTargetObj->Mana = 0;
		GSProtocol.GCManaSend(lpTargetObj->m_Index, lpTargetObj->Mana, 0xFF, 0, lpTargetObj->BP);

		return TRUE;
	}

	if ( (lpTargetObj->MaxMana + lpTargetObj->AddMana) < lpTargetObj->Mana )
	{
		lpTargetObj->Mana = lpTargetObj->MaxMana + lpTargetObj->AddMana;
		GSProtocol.GCManaSend(lpTargetObj->m_Index, lpTargetObj->Mana, 0xFF, 0, lpTargetObj->BP);

		return TRUE;
	}

	GSProtocol.GCManaSend(lpTargetObj->m_Index, lpTargetObj->Mana, 0xFF, 0, lpTargetObj->BP);
	return FALSE;
}


BOOL TMonsterSkillElement::ApplyElementAG(int iIndex, int iTargetIndex)
{
	LPOBJ lpObj = &gObj[iIndex];
	LPOBJ lpTargetObj = &gObj[iTargetIndex];
	int iIncDecValue = 0;

	if ( this->m_iIncAndDecType != MSE_INCDEC_TYPE_NONE )
	{
		if ( this->m_iIncAndDecType < MSE_INCDEC_TYPE_CONSTANTINC )
		{
			iIncDecValue = lpTargetObj->BP * this->m_iIncAndDecValue / 100;

			if ( this->m_iIncAndDecType == MSE_INCDEC_TYPE_PERCENTDEC )
				iIncDecValue = -iIncDecValue;
		}

		if ( this->m_iIncAndDecType >= MSE_INCDEC_TYPE_CONSTANTINC )
		{
			iIncDecValue = this->m_iIncAndDecValue;

			if ( this->m_iIncAndDecType == MSE_INCDEC_TYPE_CONSTANTDEC )
				iIncDecValue = -iIncDecValue;
		}
	}

	lpTargetObj->BP += iIncDecValue;

	if ( lpTargetObj->BP < 0 )
	{
		lpTargetObj->BP = 0;
		GSProtocol.GCManaSend(lpTargetObj->m_Index, lpTargetObj->Mana, 0xFF, 0, lpTargetObj->BP);

		return TRUE;
	}

	if ( (lpTargetObj->MaxBP + lpTargetObj->AddBP) < lpTargetObj->BP )
	{
		lpTargetObj->BP = lpTargetObj->MaxBP + lpTargetObj->AddBP;
		GSProtocol.GCManaSend(lpTargetObj->m_Index, lpTargetObj->Mana, 0xFF, 0, lpTargetObj->BP);

		return TRUE;
	}

	GSProtocol.GCManaSend(lpTargetObj->m_Index, lpTargetObj->Mana, 0xFF, 0, lpTargetObj->BP);

	return FALSE;
}



BOOL TMonsterSkillElement::ApplyElementDefense(int iIndex, int iTargetIndex)
{
	LPOBJ lpObj = &gObj[iIndex];
	LPOBJ lpTargetObj = &gObj[iTargetIndex];
	int iIncDecValue = 0;

	if ( this->m_iIncAndDecType != MSE_INCDEC_TYPE_NONE )
	{
		if ( this->m_iIncAndDecType < MSE_INCDEC_TYPE_CONSTANTINC )
		{
			iIncDecValue = lpTargetObj->m_Defense * this->m_iIncAndDecValue / 100;

			if ( this->m_iIncAndDecType == MSE_INCDEC_TYPE_PERCENTDEC )
				iIncDecValue = -iIncDecValue;
		}	

		if ( this->m_iIncAndDecType >= MSE_INCDEC_TYPE_CONSTANTINC )
		{
			iIncDecValue = this->m_iIncAndDecValue;

			if ( this->m_iIncAndDecType == MSE_INCDEC_TYPE_CONSTANTDEC )
				iIncDecValue = -iIncDecValue;
		}
	}

	lpTargetObj->m_MonsterSkillElementInfo.m_iSkillElementDefenseTime = this->m_iContinuanceTime;
	lpTargetObj->m_MonsterSkillElementInfo.m_iSkillElementDefense = iIncDecValue;

	if ( IMPERIAL_MAP_RANGE(lpObj->MapNumber) )
	{
		if ( this->m_iIncAndDecType == 12 || this->m_iIncAndDecType == 2 )
		{
			gObjAddBuffEffect(lpTargetObj, 58, 23, 10, 0, 0, 10);
		}
	}

	return FALSE;
}


BOOL TMonsterSkillElement::ApplyElementAttack(int iIndex, int iTargetIndex)
{
	LPOBJ lpObj = &gObj[iIndex];
	LPOBJ lpTargetObj = &gObj[iTargetIndex];
	int iIncDecValue = 0;

	if ( this->m_iIncAndDecType != MSE_INCDEC_TYPE_NONE )
	{
		if ( this->m_iIncAndDecType < MSE_INCDEC_TYPE_CONSTANTINC )
		{
			iIncDecValue = lpTargetObj->m_Defense * this->m_iIncAndDecValue / 100;

			if ( this->m_iIncAndDecType == MSE_INCDEC_TYPE_PERCENTDEC )
				iIncDecValue = -iIncDecValue;
		}	

		if ( this->m_iIncAndDecType >= MSE_INCDEC_TYPE_CONSTANTINC )
		{
			iIncDecValue = this->m_iIncAndDecValue;

			if ( this->m_iIncAndDecType == MSE_INCDEC_TYPE_CONSTANTDEC )
				iIncDecValue = -iIncDecValue;
		}
	}

	lpTargetObj->m_MonsterSkillElementInfo.m_iSkillElementAttackTime = this->m_iContinuanceTime;
	lpTargetObj->m_MonsterSkillElementInfo.m_iSkillElementAttack = iIncDecValue;

	return FALSE;
}


BOOL TMonsterSkillElement::ApplyElementDurability(int iIndex, int iTargetIndex)
{
	LPOBJ lpObj = &gObj[iIndex];
	LPOBJ lpTargetObj = &gObj[iTargetIndex];

	if ( lpTargetObj->Type != OBJ_USER )
		return FALSE;

	int iEquipmentPos = rand()%6+2;	// Armors
	CItem * lpEquipment = &lpTargetObj->pInventory[iEquipmentPos];

	if ( lpEquipment == NULL || lpEquipment->IsItem() == FALSE )
		return FALSE;

	if (g_LuckyItemManager.IsLuckyItemEquipment(lpEquipment->m_Type))
	{
		return false;
	}

	int iDurEquipment = lpEquipment->m_Durability;
	int iIncDecValue = 0;

	if ( this->m_iIncAndDecType != MSE_INCDEC_TYPE_NONE )
	{
		if ( this->m_iIncAndDecType < MSE_INCDEC_TYPE_CONSTANTINC )
		{
			iIncDecValue = iDurEquipment * this->m_iIncAndDecValue / 100;

			if ( this->m_iIncAndDecType == MSE_INCDEC_TYPE_PERCENTDEC )
				iIncDecValue = -iIncDecValue;
		}	

		if ( this->m_iIncAndDecType >= MSE_INCDEC_TYPE_CONSTANTINC )
		{
			iIncDecValue = this->m_iIncAndDecValue;

			if ( this->m_iIncAndDecType == MSE_INCDEC_TYPE_CONSTANTDEC )
				iIncDecValue = -iIncDecValue;
		}
	}

	iDurEquipment += iIncDecValue;

	if ( lpEquipment )
	{
		lpEquipment->m_Durability = iDurEquipment;

		if ( lpEquipment->m_Durability < 0.0f )
			lpEquipment->m_Durability = 0;

		GSProtocol.GCItemDurSend(lpTargetObj->m_Index, iEquipmentPos, lpEquipment->m_Durability, 0);
	}

	return FALSE;
}


BOOL TMonsterSkillElement::ApplyElementSummon(int iIndex, int iTargetIndex)
{
	LPOBJ lpObj = &gObj[iIndex];
	LPOBJ lpTargetObj = &gObj[iTargetIndex];

	if ( lpTargetObj->Connected < PLAYER_PLAYING ||
		 lpTargetObj->Type != OBJ_MONSTER )
		 return FALSE;

	if (lpObj->Class == 459 && lpObj->Connected == PLAYER_PLAYING && lpObj->MapNumber == MAP_INDEX_HATCHERY && iIndex == iTargetIndex)//Season 4.5 addon
	{
		g_Log.Add("[TMonsterSkillElement][ApplyElementSummon] Selupan use summon.");
		return FALSE;
	}

	lpTargetObj->Life = lpTargetObj->MaxLife + lpTargetObj->AddLife;
	lpTargetObj->Mana = lpTargetObj->MaxMana + lpTargetObj->AddMana;
	lpTargetObj->Live = TRUE;
	lpTargetObj->m_SkillInfo.PoisonType = 0;
	lpTargetObj->m_SkillInfo.IceType = 0;
	lpTargetObj->Teleport = 0;
	lpTargetObj->m_ViewState = 0;
	gObjRemoveBuffEffect(lpTargetObj, BUFFTYPE_POISON);
	gObjRemoveBuffEffect(lpTargetObj, BUFFTYPE_FREEZE);
	gObjRemoveBuffEffect(lpTargetObj, BUFFTYPE_STONE);

	for ( int i=0;i<MAX_SELF_DEFENSE;i++)
		lpTargetObj->SelfDefenseTime[i] = 0;

	gObjClearBuffEffect(lpTargetObj, CLEAR_TYPE_LOGOUT);

	gObjTimeCheckSelfDefense(lpTargetObj);
	gObjViewportListProtocolDestroy(lpTargetObj);
	gObjViewportClose(lpTargetObj);
	lpTargetObj->m_ActState.Attack = 0;
	lpTargetObj->m_ActState.EmotionCount = 0;
	lpTargetObj->m_ActState.Escape = 0;
	lpTargetObj->m_ActState.Move = 0;
	lpTargetObj->m_ActState.Rest = 0;
	lpTargetObj->m_ActState.Emotion = 0;
	lpTargetObj->TargetNumber = -1;
	lpTargetObj->NextActionTime = 5000;

	if ( this->m_iIncAndDecValue < 0 )
		this->m_iIncAndDecValue = 10;

	BYTE cX;
	BYTE cY;
	BOOL bGetPosition = FALSE;
	int iCount = 100;

	while ( iCount-- != 0 )
	{
		cX = ( rand() % (this->m_iIncAndDecValue+1) ) * (((rand()%2==0)?-1:1)) + lpObj->X;
		cY = ( rand() % (this->m_iIncAndDecValue+1) ) * (((rand()%2==0)?-1:1)) + lpObj->Y;

		BYTE btMapAttr = MapC[lpObj->MapNumber].GetAttr(cX, cY);

		if ( btMapAttr == 0 )
		{
			bGetPosition = TRUE;
			break;
		}
	}

	if ( bGetPosition == FALSE )
	{
		lpObj->Live = FALSE;
		lpObj->m_State = 4;
		lpObj->RegenTime = GetTickCount();
		lpObj->DieRegen = 1;

		return FALSE;
	}

	lpTargetObj->X = cX;
	lpTargetObj->Y = cY;
	lpTargetObj->MTX = lpTargetObj->X;
	lpTargetObj->MTY = lpTargetObj->Y;
	lpTargetObj->TX = lpTargetObj->X;
	lpTargetObj->TY = lpTargetObj->Y;
	lpTargetObj->StartX = lpTargetObj->X;
	lpTargetObj->StartY = lpTargetObj->Y;

	gObjMonsterHitDamageInit(lpTargetObj);
	CreateFrustrum(lpTargetObj->X, lpTargetObj->Y, lpTargetObj->m_Index);
	lpTargetObj->DieRegen = 0;
	lpTargetObj->m_State = 1;
	lpTargetObj->m_Agro->ResetAll();
	gObjViewportListCreate(lpTargetObj->m_Index);

	return FALSE;
}


BOOL TMonsterSkillElement::ApplyElementPush(int iIndex, int iTargetIndex)
{
	LPOBJ lpObj = &gObj[iIndex];
	LPOBJ lpTargetObj = &gObj[iTargetIndex];

	if (lpObj->Class == 459 && lpObj->Connected == 3 && lpObj->MapNumber == 58 && iIndex == iTargetIndex)//Season 4.5 addon
	{
		return FALSE;
	}

	if ( this->m_iNullifiedSkill == -1 )
		this->m_iNullifiedSkill = 0;

	if ( this->m_iIncAndDecValue < 1 )
		this->m_iIncAndDecValue = 3;

	if ( this->m_iNullifiedSkill == 0 )
	{
		gObjBackSpring2(lpTargetObj, lpObj,this->m_iIncAndDecValue);
	}
	else
	{
		gObjAddMsgSendDelay(lpObj, 57, iTargetIndex, this->m_iNullifiedSkill, this->m_iIncAndDecValue);
	}

	return FALSE;
}


BOOL TMonsterSkillElement::ApplyElementStatEnergy(int iIndex, int iTargetIndex)
{
	LPOBJ lpObj = &gObj[iIndex];
	LPOBJ lpTargetObj = &gObj[iTargetIndex];

	return FALSE;
}

BOOL TMonsterSkillElement::ApplyElementStatStrength(int iIndex, int iTargetIndex)
{
	LPOBJ lpObj = &gObj[iIndex];
	LPOBJ lpTargetObj = &gObj[iTargetIndex];

	return FALSE;
}

BOOL TMonsterSkillElement::ApplyElementStatDexiterity(int iIndex, int iTargetIndex)
{
	LPOBJ lpObj = &gObj[iIndex];
	LPOBJ lpTargetObj = &gObj[iTargetIndex];

	return FALSE;
}

BOOL TMonsterSkillElement::ApplyElementStatVitality(int iIndex, int iTargetIndex)
{
	LPOBJ lpObj = &gObj[iIndex];
	LPOBJ lpTargetObj = &gObj[iTargetIndex];

	return FALSE;
}

BOOL TMonsterSkillElement::ApplyElementRemoveSkill(int iIndex, int iTargetIndex)
{
	LPOBJ lpObj = &gObj[iIndex];
	LPOBJ lpTargetObj = &gObj[iTargetIndex];

	gObjUseSkill.RemoveSpecificSkillState(lpTargetObj, this->m_iNullifiedSkill);

	return FALSE;
}

BOOL TMonsterSkillElement::ApplyElementResistSkill(int iIndex, int iTargetIndex)
{
	LPOBJ lpObj = &gObj[iIndex];
	LPOBJ lpTargetObj = &gObj[iTargetIndex];

	lpTargetObj->m_MonsterSkillElementInfo.m_iSkillElementResistNumber = this->m_iNullifiedSkill;
	lpTargetObj->m_MonsterSkillElementInfo.m_iSkillElementResistTime = this->m_iContinuanceTime;

	return FALSE;
}

BOOL TMonsterSkillElement::ApplyElementImmuneSkill(int iIndex, int iTargetIndex)
{
	LPOBJ lpObj = &gObj[iIndex];
	LPOBJ lpTargetObj = &gObj[iTargetIndex];

	lpTargetObj->m_MonsterSkillElementInfo.m_iSkillElementImmuneNumber = this->m_iNullifiedSkill;
	lpTargetObj->m_MonsterSkillElementInfo.m_iSkillElementImmuneTime = this->m_iContinuanceTime;
	gObjAddBuffEffect(lpObj, BUFFTYPE_MONSTER_MAGIC_IMMUNE, 0, 0, 0, 0, this->m_iContinuanceTime);
	return FALSE;
}

BOOL TMonsterSkillElement::ApplyElementTeleportSkill(int iIndex, int iTargetIndex)
{
	if ( !ObjectMaxRange(iIndex))
		return FALSE;

	LPOBJ lpObj = &gObj[iIndex];

	if (gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_STONE) || gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_STUN) || gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_SLEEP) ||
		gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_FREEZE_2) || gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_EARTH_BINDS))
	{
		return FALSE;
	}

	BYTE x;
	BYTE y;
	int depth = rand()%4 + 3;

	if ( (rand()%2) == 0 )
		x = lpObj->X + depth;
	else
		x = lpObj->X - depth;

	if ( (rand()%2) == 0 )
		y = lpObj->Y + depth;
	else
		y = lpObj->Y - depth;

	if ( gObjCheckTeleportArea(iIndex, x, y) == FALSE )
	{
		g_Log.AddC(TColor::Red,  "[%s][%s] Try Teleport Not Move Area [%d,%d]",
			lpObj->AccountID, lpObj->Name,	x, y);
		
		return FALSE;
	}

	PMSG_MAGICATTACK_RESULT pAttack;

	PHeadSetBE((LPBYTE)&pAttack, 0x19, sizeof(pAttack));
	WORD MagicNumber = 6;
	pAttack.MagicNumberH = SET_NUMBERH(MagicNumber);
	pAttack.MagicNumberL = SET_NUMBERL(MagicNumber);
	pAttack.SourceNumberH = SET_NUMBERH(iIndex);
	pAttack.SourceNumberL = SET_NUMBERL(iIndex);
	pAttack.TargetNumberH = SET_NUMBERH(iIndex);
	pAttack.TargetNumberL = SET_NUMBERL(iIndex);

	if ( lpObj->Type == OBJ_USER )
		IOCP.DataSend(iIndex, (LPBYTE)&pAttack, pAttack.h.size);	

	GSProtocol.MsgSendV2(lpObj, (LPBYTE)&pAttack, pAttack.h.size);

	g_Log.AddC(TColor::Pink,  "[%s] Warp [%d,%d] -> [%d,%d]",
		lpObj->Name, lpObj->X, lpObj->Y, x, y);

	gObjTeleportMagicUse(iIndex, x, y);
	lpObj->TargetNumber = -1;

	return FALSE;
}

BOOL TMonsterSkillElement::ApplyElementDoubleHP(int iIndex, int iTargetIndex)
{
	LPOBJ lpObj = &gObj[iIndex];
	LPOBJ lpTargetObj = &gObj[iTargetIndex];

	if ( lpTargetObj->Connected < PLAYER_PLAYING )
		return FALSE;

	gObjAddMsgSendDelay(lpObj, 55, iTargetIndex, 400, 0);
	gObjAddMsgSendDelay(lpObj, 55, iTargetIndex, 1000, 0);

	return FALSE;
}


BOOL TMonsterSkillElement::ApplyElementPoison(int iIndex, int iTargetIndex)
{
	LPOBJ lpObj = &gObj[iIndex];
	LPOBJ lpTargetObj = &gObj[iTargetIndex];

	if ( lpTargetObj->Connected < PLAYER_PLAYING )
		return FALSE;

	if ( !gObjCheckUsedBuffEffect(lpTargetObj, BUFFTYPE_POISON) )
	{
		if ( this->m_iNullifiedSkill  < 1 )
		{
			if ( retResistance(lpTargetObj, 1) == 0 )
			{
				lpTargetObj->lpAttackObj = lpObj;
				gObjAddBuffEffect(lpTargetObj, BUFFTYPE_POISON, EFFECTTYPE_POISON_DMG_TICK, 3, 0, 0, this->m_iIncAndDecValue);
			}
		}
		else
		{
			gObjAddMsgSendDelay(lpObj, 56, iTargetIndex, this->m_iNullifiedSkill, this->m_iIncAndDecValue);
		}
	}

	return FALSE;
}


BOOL TMonsterSkillElement::ApplyElementNormalAttack(int iIndex, int iTargetIndex)
{
	LPOBJ lpObj = &gObj[iIndex];
	LPOBJ lpTargetObj = &gObj[iTargetIndex];

	if ( lpTargetObj->Connected < PLAYER_PLAYING )
		return FALSE;

	if ( this->m_iNullifiedSkill == -1 )
		this->m_iNullifiedSkill = 0;

	lpObj->Dir = GetPathPacketDirPos(lpTargetObj->X-lpObj->X, lpTargetObj->Y-lpObj->Y);

	if ( this->m_iIncAndDecType != -1 && this->m_iIncAndDecValue != -1 )
	{
		if ( lpTargetObj->pInventory[9].m_Type != ITEMGET(this->m_iIncAndDecType, this->m_iIncAndDecValue)  &&
			 lpTargetObj->pInventory[10].m_Type != ITEMGET(this->m_iIncAndDecType, this->m_iIncAndDecValue) &&
			 lpTargetObj->pInventory[11].m_Type != ITEMGET(this->m_iIncAndDecType, this->m_iIncAndDecValue) )
		{
			lpTargetObj->Life = 0;
			gObjLifeCheck(lpTargetObj, lpObj, 100, 0, 0, 0, 0 ,0, 0);
			return FALSE;
		}
	}

	if ( this->m_iNullifiedSkill == 0 )
	{
		gObjAttack(lpObj, lpTargetObj, 0, 0, 0, 0 ,0, 0, 0);
	}
	else
	{
		gObjAddMsgSendDelay(lpObj, 55, iTargetIndex, this->m_iNullifiedSkill, 0);
	}

	return FALSE;
}

BOOL TMonsterSkillElement::ApplyElementImmuneAllSkill(int iIndex, int iTargetIndex)
{
	LPOBJ lpObj = &gObj[iIndex];
	LPOBJ lpTargetObj = &gObj[iTargetIndex];

	lpTargetObj->m_MonsterSkillElementInfo.m_iSkillElementImmuneAllTime = this->m_iContinuanceTime;

	int DurationTime = 0;

	if (this->m_iContinuanceTime == -1)
	{
		DurationTime = -10;
	}
	else
	{
		DurationTime = this->m_iContinuanceTime / 1000;
	}

	gObjAddBuffEffect(lpTargetObj, BUFFTYPE_MONSTER_MAGIC_IMMUNE, 0, 0, 0, 0, DurationTime);
	gObjAddBuffEffect(lpTargetObj, BUFFTYPE_MONSTER_MELEE_IMMUNE, 0, 0, 0, 0, DurationTime);

	return FALSE;
}

BOOL TMonsterSkillElement::ApplyElementPercentDamageNormalAttack(int iIndex, int iTargetIndex)
{
	LPOBJ lpObj = &gObj[iIndex];
	LPOBJ lpTargetObj = &gObj[iTargetIndex];

	if ( lpTargetObj->Connected < PLAYER_PLAYING )
		return FALSE;

	if ( this->m_iNullifiedSkill == -1 )
		this->m_iNullifiedSkill = 0;

	if ( this->m_iIncAndDecType != -1 && this->m_iIncAndDecValue != -1 )
	{
		if ( lpTargetObj->pInventory[9].m_Type != ITEMGET(this->m_iIncAndDecType, this->m_iIncAndDecValue)  &&
			 lpTargetObj->pInventory[10].m_Type != ITEMGET(this->m_iIncAndDecType, this->m_iIncAndDecValue) &&
			 lpTargetObj->pInventory[11].m_Type != ITEMGET(this->m_iIncAndDecType, this->m_iIncAndDecValue) )
		{
			lpTargetObj->Life = 0;
			gObjLifeCheck(lpTargetObj, lpObj, 100, 0, 0, 0, 0 ,0, 0);
			return FALSE;
		}
	}

	if ( this->m_iNullifiedSkill == 0 )
	{
		int nDamage = 20 * lpTargetObj->MaxLife / 100;
		gObjAttack(lpObj, lpTargetObj, 0, 0, 0, nDamage, 0, 0, 0);
	}

	else
	{
		gObjAddMsgSendDelay(lpObj, 62, iTargetIndex, this->m_iNullifiedSkill, 0);
	}

	return FALSE;
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

