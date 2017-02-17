//////////////////////////////////////////////////////////////////////
// MasterLevelSkillTreeSystem.h
#ifndef MASTERLEVELSKILLTREESYSTEM_H
#define MASTERLEVELSKILLTREESYSTEM_H

#include "StdAfx.h"
#include "user.h"
#include "classdef.h"
#include "MasterSkillSystem.h"
#include "MuLua.h"
#include "ObjAttack.h"

#define MAX_SKILL_POS 4
#define MAX_TREE_TYPE 3
#define MAX_MASTER_SKILL_TYPE 450
#define MAX_MASTER_SKILL_RANK 9

struct MLST_element
{
	int		m_dwIndex;
	BYTE	m_byConsumePoint;
	BYTE	m_byMaxPoint;
	DWORD	m_dwParentSkill1;
	DWORD	m_dwParentSkill2;
	DWORD	m_dwSkillID;
};

struct MLSTree
{
	MLST_element		iElement[MAX_MASTER_SKILL_RANK][MAX_SKILL_POS];
};

struct MLST_table
{
	BYTE		btClass;
	BYTE		btType;
	BYTE		btWidth;
	MLSTree		iAbility[MAX_TREE_TYPE];
};

enum MLST_Value_Types
{
	MLST_VALUE_NORMAL = 0,
	MLST_VALUE_DAMAGE = 1,
	MLST_VALUE_MANA_INC = 2
};

struct MLST_Value_Table
{
	MLST_Value_Table();
	~MLST_Value_Table();

	void Init(int iMaxSkillLevel)
	{
		if (this->fValueTable != NULL)
		{
			delete[] this->fValueTable;
			this->fValueTable = NULL;
		}

		this->fValueTable = new float[iMaxSkillLevel + 1];
		this->iValueType = MLST_VALUE_NORMAL;

		for (int i = 0; i <= iMaxSkillLevel; i++)
		{
			this->fValueTable[i] = 0.0;
		}
	}

	int iValueType;
	float * fValueTable;
};

static const int CHARACTER_CLASS_TABLE[MAX_TYPE_PLAYER][2] = {
	2,		CLASS_WIZARD,
	1,		CLASS_KNIGHT,
	4,		CLASS_ELF,
	16,		CLASS_MAGUMSA,
	8,		CLASS_SUMMONER,
	32,		CLASS_DARKLORD,
	64,		CLASS_RAGEFIGHTER,
	128,	CLASS_GROWLANCER
};

struct PMSG_MASTERLEVELSKILLTREE_COUNT
{
	PWMSG_HEAD2	h;
	int count;				// 8
};

struct PMSG_MASTERLEVELSKILLTREE
{
	BYTE btMasterSkillUIIndex;
	BYTE btMasterSkillLevel;
	float fMasterSkillCurValue;
	float fMasterSkillNextValue;
	BYTE btUnk;
};

struct PMSG_ANS_MASTERLEVELSKILLTREE
{
	PBMSG_HEAD2	h;
	BYTE btResult;			// 4
	WORD wMasterLevelPoint;		// 6
	BYTE btMasterSkillUIIndex;	// 8
	int dwMasterSkillIndex;		// C
	int dwMasterSkillLevel;			// 10
	float fMasterSkillCurValue;			// 14
	float fMasterSkillNextValue;		// 18
};

#pragma once
class CMasterLevelSkillTreeSystem : public CMasterLevelSystem
{
public:
	CMasterLevelSkillTreeSystem(void);
	virtual ~CMasterLevelSkillTreeSystem(void);
	friend CObjAttack;
	friend void gObjCharZeroSet(int aIndex);

	void Init();
	void AddToValueTable(int iSkillValueID, const char * szFunctionName, int iSkillValueType);
	void Load(const char *lpszFileName);

	void InitPassiveSkillData(OBJECTSTRUCT* lpObj);
	void CalcPassiveSkillData(OBJECTSTRUCT* lpObj, int iMLSkill, int iSkillLevel);
	int CheckSkillCondition(OBJECTSTRUCT *lpObj, int iMLSkill, int iSkillLevel);
	int CheckCompleteBrandOfSkill(OBJECTSTRUCT *lpObj, int iMLSkill);
	int CheckMasterLevelSkill(int iMLSkill);
	int IsExistMLSBasedSkill(OBJECTSTRUCT *lpObj, int iSkill);
	int GetBaseMasterLevelSkill(int iMLSkill);
	int gObjMagicAdd_MLS(OBJECTSTRUCT *lpObj, int iMLSkill, int iSkillLevel);
	void CGReqGetMasterLevelSkillTree(int aIndex);
	void CGReqGetMasterLevelSkill(PMSG_REQ_MASTERLEVEL_SKILL *lpMsg, int aIndex);
	int RunningSkill_MLS(OBJECTSTRUCT *lpObj, int aTargetIndex, CMagicInf *lpMagic, BOOL bCombo, BYTE x, BYTE y, BYTE aTargetPos);
	int CheckUsableWeaponSkill(OBJECTSTRUCT *lpObj, int nCount);
	int GetRequireMLPoint(int iClass, int iSkill);
	int GetMaxPointOfMasterSkill(int iClass, int iSkill);
	float GetMasterSkillValue(int iType, int iPoint);
	float GetMasterSkillManaIncRate(int iPoint);
	float GetBrandOfMasterSkillValue(OBJECTSTRUCT *lpObj, int iSkill, int iStep);
	int CheckPreviousRankSkill(OBJECTSTRUCT *lpObj, int iSkill, int iLevel);
	int CheckSkillLevel(OBJECTSTRUCT *lpObj, int iSkill);
	int GetMasterSkillUIIndex(int iClass, int iSkill);
	int GetParentSkill1(int iClass, int iSkill);
	int GetParentSkill2(int iClass, int iSkill);
	void SetItemMLPassiveSkill(OBJECTSTRUCT *lpObj, int iType);
	void SetPetItemMLPassiveSkill(OBJECTSTRUCT *lpObj, int iType);
	void SetWingMLPassiveSkill(OBJECTSTRUCT *lpObj, int iType);
	int GetCurSkillLevel(OBJECTSTRUCT *lpObj, int iSkill);
	int GetScriptClassCode(int iClass);
	float RoundValue(long double dbValue, int iPos);
	BYTE ResetMasterSkill(int aIndex, int nTreeType);
	BYTE CheckMasterSkillPoint(int aIndex, int nTreeType);
	BYTE CheckSkillInTree(int iClass, int iSkill, int iTreeKind);
	float GetSkillAttackDamage(OBJECTSTRUCT *lpObj, int iSkill);

	// Skills
	void MLS_WizardMagicDefense(OBJECTSTRUCT *lpObj, int aTargetIndex, CMagicInf *lpMagic);
	void MLS_Healing(OBJECTSTRUCT *lpObj, int aTargetIndex, CMagicInf *lpMagic);
	void MLS_Wheel(OBJECTSTRUCT *lpObj, CMagicInf *lpMagic, int aTargetIndex, BOOL isCombo);
	void MLS_SkillSword4(OBJECTSTRUCT *lpObj, CMagicInf *lpMagic, int aTargetIndex, BOOL isCombo);
	void MLS_SkillSword4_Magumsa(OBJECTSTRUCT *lpObj, CMagicInf *lpMagic, int aTargetIndex, BOOL isCombo);
	void MLS_SkillSword5(OBJECTSTRUCT *lpObj, CMagicInf *lpMagic, int aTargetIndex, BOOL isCombo);
	void MLS_SkillSword1(OBJECTSTRUCT *lpObj, CMagicInf *lpMagic, int aTargetIndex, BOOL isCombo);
	void MLS_SkillSword2(OBJECTSTRUCT *lpObj, CMagicInf *lpMagic, int aTargetIndex, BOOL isCombo);
	void MLS_SkillStrengthenDesruction(OBJECTSTRUCT *lpObj, CMagicInf *lpMagic, BYTE x, BYTE y, int aTargetIndex, int isCombo);
	void MLS_KnightSkillAddLife(OBJECTSTRUCT *lpObj, CMagicInf *lpMagic);
	void MLS_SkillFulKnightSkillAddLife(OBJECTSTRUCT *lpObj, CMagicInf *lpMagic);
	void MLS_MasteryKnightSkillAddLife(OBJECTSTRUCT *lpObj, CMagicInf *lpMagic);
	void MLS_SkillStrengthenMagicPower(OBJECTSTRUCT *lpObj, CMagicInf *lpMagic, int aTargetIndex);
	void MLS_SkillSkillfulMagicPower(OBJECTSTRUCT *lpObj, CMagicInf *lpMagic, int aTargetIndex);
	void MLS_SkillStrengthenFlame(OBJECTSTRUCT *lpObj, CMagicInf *lpMagic, int aTargetIndex);
	void MLS_SkillStrengthenThunder(OBJECTSTRUCT *lpObj, CMagicInf *lpMagic, int aTargetIndex);
	void MLS_SkillStrengthenInferno(OBJECTSTRUCT *lpObj, CMagicInf *lpMagic);
	void MLS_SkillStrengthenHellFire(OBJECTSTRUCT *lpObj, CMagicInf *lpMagic);
	void MLS_SkillStrengthenPoison(OBJECTSTRUCT *lpObj, CMagicInf *lpMagic, int aTargetIndex);
	void MLS_SkillStrengthenExPoison(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, int aTargetIndex);
	void MLS_SkillStrengthenIce(OBJECTSTRUCT *lpObj, CMagicInf *lpMagic, int aTargetIndex);
	void MLS_SkillWizardMagicDefense(OBJECTSTRUCT *lpObj, CMagicInf *lpMagic, int aTargetIndex);
	void MLS_SkillStrengthenPenetration(OBJECTSTRUCT *lpObj, CMagicInf *lpMagic, int aTargetIndex);
	void MLS_SkillInfinityArrow(int aIndex, int aTargetIndex, CMagicInf *lpMagic);
	void MLS_SkillStrengthenBerserker(int aIndex, CMagicInf *lpMagic);
	void MLS_SkillSkillfulBerserker(int aIndex, CMagicInf *lpMagic);
	void MLS_SkillMasteryBerserker(int aIndex, CMagicInf *lpMagic);
	void MLS_SkillStrengthenBlast(OBJECTSTRUCT *lpObj, CMagicInf *lpMagic, int aTargetIndex);
	void MLS_SkillStrendthenAddCriticalDamage(int aIndex, CMagicInf *lpMagic);
	void MLS_SkillSkillFulAddCriticalDamage(int aIndex, CMagicInf *lpMagic);
	void MLS_SkillMasteryAddCriticalDamage(int aIndex, CMagicInf *lpMagic);
	void MLS_DarkHorseAttack(OBJECTSTRUCT *lpObj, int aTargetIndex, CMagicInf *lpMagic);
	void MLS_FireBurst(OBJECTSTRUCT *lpObj, int aTargetIndex, CMagicInf *lpMagic);
	void MLS_SkillStrengthenSpear(int aIndex, int aTargetIndex, CMagicInf *lpMagic);
	void MLS_SkillAreaMonsterAttack(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, int aTargetIndex, int aDistance, BOOL bPVP, int nSuccessRate, int nDelay);
	void MLS_SuddenIce(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, int aTargetIndex);
	int MLS_CalDistance(int x, int y, int x1, int y1);
	int MLS_GetAngle(int x, int y, int tx, int ty);
	void MLS_SkillFrustrum(int aIndex, float fangle, float tx, float ty);
	void MLS_KnightBlow(int aIndex, int aTargetIndex, CMagicInf *lpMagic, BOOL isCombo);
	void MLS_SkillBlowOfFury(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, int aTargetIndex, BOOL isCombo);
	void MLS_SkillDefense(int aIndex, int aTargetIndex, CMagicInf *lpMagic);
	void MLS_SkillAttack(int aIndex, int aTargetIndex, CMagicInf *lpMagic);
	void MLS_SkillStrengthenDefense(int aIndex, int aTargetIndex, CMagicInf *lpMagic);
	void MLS_SkillStrenghtenAttack(int aIndex, int aTargetIndex, CMagicInf *lpMagic);
	void MLS_MaGumSkillDefenseDown(int aIndex, int aTargetIndex);
	void MLS_PowerSlash(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, BYTE Targetangle, int aTargetIndex);
	void MLS_FireScream(int aIndex, int aTargetIndex, CMagicInf *lpMagic);
	void MLS_SkillMonkBarrageJustOneTarget(int aIndex, CMagicInf *lpMagic, int aTargetIndex);
	void MLS_SkillStrengthenDragonRoar(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, int aTargetIndex, int aDistance, BOOL bPVP, int nDelay);
	void MLS_SkillStrengthenBlockRate(int aIndex, CMagicInf *lpMagic);
	void MLS_SkillMasteryBlockRate(int aIndex, CMagicInf *lpMagic);
	void MLS_SkillStrengthenIncVital(int aIndex, CMagicInf *lpMagic);
	void MLS_SkillStrengthenIgnoreEnemyDefense(int aIndex, CMagicInf *lpMagic);
	void MLS_SkillBloodStorm(OBJECTSTRUCT *lpObj, CMagicInf *lpMagic, BYTE x, BYTE y, int aTargetIndex, BOOL isCombo);
	void MLS_SkillEarthPrison(OBJECTSTRUCT *lpObj, int aTargetIndex, CMagicInf *lpMagic);
	void MLS_SkillPoisonArrow(OBJECTSTRUCT *lpObj, CMagicInf *lpMagic, int aTargetIndex, BOOL isCombo);
	void MLS_SkillCure(OBJECTSTRUCT *lpObj, CMagicInf *lpMagic, int aTargetIndex);
	void MLS_SkillMonsterCall(int aIndex, int MonsterType, int x, int y);
	void MLS_SkillPartyHealing(int aIndex, int aTargetIndex, CMagicInf *lpMagic);
	void MLS_SkillBless(OBJECTSTRUCT *lpObj, int aTargetIndex, CMagicInf *lpMagic);
	void MLS_SkillStrengthenBless(OBJECTSTRUCT *lpObj, int aTargetIndex, CMagicInf *lpMagic);
	void MLS_SkillBlind(OBJECTSTRUCT *lpObj, int aTargetIndex, CMagicInf *lpMagic);
	void MLS_SkillIronDefense(OBJECTSTRUCT *lpObj, int aTargetIndex, CMagicInf *lpMagic);
	void MLS_SkillStrengthenIronDefense(OBJECTSTRUCT *lpObj, int aTargetIndex, CMagicInf *lpMagic);
	void MLS_SkillBloodHowling(OBJECTSTRUCT *lpObj, int aTargetIndex, CMagicInf *lpMagic);
	void MLS_SkillStrengthenBloodHowling(OBJECTSTRUCT *lpObj, int aTargetIndex, CMagicInf *lpMagic);
	void MLS_SkillfulKnightBlow(int aIndex, int aTargetIndex, CMagicInf *lpMagic, BOOL isCombo);
	void MLS_MasteryKnightBlow(int aIndex, int aTargetIndex, CMagicInf *lpMagic, BOOL isCombo);
	void MLS_SkillSkillfulDesruction(OBJECTSTRUCT *lpObj, CMagicInf *lpMagic, BYTE x, BYTE y, int aTargetIndex, BOOL isCombo);
	void MLS_SkillMasteryDesruction(OBJECTSTRUCT *lpObj, CMagicInf *lpMagic, BYTE x, BYTE y, int aTargetIndex, BOOL isCombo);
	void MLS_MasterySuddenIce(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, int aTargetIndex);
	void MLS_SkillStrengthenMultiShot(int aIndex, CMagicInf *lpMagic, BYTE bAngle, int aTargetIndex);
	void MLS_SkillHellBust(int aIndex, int aTargetIndex, CMagicInf *lpMagic);
	void MLS_SkillHellBustStart(int aIndex, CMagicInf *lpMagic);
	void MLS_SkillMasteryWizardMagicDefense(OBJECTSTRUCT *lpObj, CMagicInf *lpMagic, int aTargetIndex);
	void MLS_SkillStrengthenWeakness(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, int aTargetIndex);
	void MLS_SkillStrengthenEnervation(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, int aTargetIndex);
	void MLS_SkillMasteryIce(OBJECTSTRUCT *lpObj, CMagicInf *lpMagic, int aTargetIndex);
	void MLS_SkillStrengthenFlameStrike(int aIndex, CMagicInf *lpMagic, BYTE bAngle, int aTargetIndex);
	void MLS_SkillGiganticStorm(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, int aTargetIndex);
	void MLS_SkillMasteryDefenseDown(int aIndex, int aTargetIndex, CMagicInf *lpMagic);
	void MLS_SkillMasteryAddCriticalDamage2(int aIndex, CMagicInf *lpMagic);
	void MLS_SkillFrustrum3(int aIndex, BYTE bAngle, float tx1, float ty1, float tx2, float ty2);
	void MLS_SkillStrengthenElectricSpark(int aIndex, CMagicInf *lpMagic, BYTE x, BYTE y, BYTE TargetPos, int aTargetIndex);
	void MLS_SkillStrengthenchaoticDesair(int aIndex, CMagicInf *lpMagic, BYTE bAngle, int aTargetIndex);
	bool MLS_WeaponSkillDel(OBJECTSTRUCT *lpObj, WORD aSkill, BYTE Level);
	bool MLS_WeaponSkillAdd(OBJECTSTRUCT *lpObj, WORD aSkill, BYTE Level);
	void MLS_SkillSleep(int aIndex, int aTargetIndex, CMagicInf *lpMagic);
	void MLS_SkillChainLightning(int aIndex, int aTargetIndex, CMagicInf *lpMagic);
	void MLS_SkillLightningShock(int aIndex, int aTargetIndex, CMagicInf *lpMagic);
	void MLS_SkillDrainLife(int aIndex, int aTargetIndex, CMagicInf *lpMagic);
	// FINISHED

	int GetMaxMasterSkillLevel() { return this->m_wMaxMasterSkillLevel; }
private:

	MLST_table			m_MLSTable[MAX_TYPE_PLAYER];
	MLST_Value_Table	* m_MLSValueTable; //[MAX_MASTER_SKILL_TYPE];
	MULua				m_Lua;
	WORD				m_wMaxMasterSkillLevel; // private custom
};

extern CMasterLevelSkillTreeSystem g_MasterLevelSkillTreeSystem;

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

