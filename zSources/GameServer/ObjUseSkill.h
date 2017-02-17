//////////////////////////////////////////////////////////////////////
// ObjUseSkill.h
#ifndef OBJUSESKILL_H
#define OBJUSESKILL_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MagicInf.h"
#include "user.h"
#include "MuLua.h"

struct SkillConfig
{
	int SternTime;
	int SwellManaTime;
	int CloakingTime;

	int IceArrowTime;

	int PoisonPercent; // custom
	int DeathPoisonPercent; // custom

	bool EnableSiegeOnAllMaps;
	bool UseCalcForTripleShot; // custom - joinmu

	int PoisonSkillSuccessRate;
	int PoisonSkillTime;

	bool EnableWizardSkillWhileTeleport;
};

class CObjUseSkill
{

public:

	CObjUseSkill();
	virtual ~CObjUseSkill();
	friend class CObjAttack;

	bool LoadSkillInfo();

	int SpeedHackCheck(int aIndex);
	int EnableSkill(WORD Skill);
	int GetUseMana(int aIndex, CMagicInf* lpMagic);
	int GetAddUseMana(LPOBJ lpObj, CMagicInf* lpMagic);
	int GetUseBP(int aIndex, CMagicInf* lpMagic);
	void UseSkill(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	void UseSkill(int aIndex, CMagicInf* lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos, int aTargetIndex);
	int RunningSkill(int aIndex, int aTargetIndex, CMagicInf* lpMagic, BOOL bCombo);
	void MaGumSkillDefenseDown(int aIndex, int aTargetIndex, int skill_level);
	void KnightSkillAddLife(int aIndex, CMagicInf * lpMagic);
	void WizardMagicDefense(int aIndex, int aTargetIndex, CMagicInf * lpMagic);
	void SkillDefense(int aIndex, int aTargetIndex, CMagicInf * lpMagic);
	void SkillAttack(int aIndex, int aTargetIndex, CMagicInf * lpMagic);
	int SkillChangeUse(int aIndex);
	void SkillHealing(int aIndex, int aTargetIndex, CMagicInf * lpMagic);
	int SkillMonsterCall(int aIndex, int MonsterType, int x, int y);
	int SkillDeathPoison(int aIndex, CMagicInf* lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos, int aTargetIndex);
	int SkillDeathPoisonForMedusa(int aIndex, CMagicInf* lpMagic, BYTE x, BYTE y, int aTargetIndex);
	int SkillSuddenIce(int aIndex, CMagicInf* lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos, int aTargetIndex);
	int SkillHellFire2Start(int aIndex, CMagicInf* lpMagic);
	int SkillHellFire2(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	void SkillBlowOfFury(int aIndex, CMagicInf* lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos, int aTargetIndex, BOOL isCombo);
	void SkillWheel(int aIndex, CMagicInf* lpMagic, int aTargetIndex, BOOL isCombo);
	int SkillKnightBlow(int aIndex, int aTargetIndex, CMagicInf* lpMagic, BOOL isCombo);
	void SkillPowerSlash(int aIndex, CMagicInf* lpMagic, BYTE x, BYTE y, BYTE dir, BYTE Targetangle, int aTargetIndex);
	void SkillFrustrum(int aIndex, float fangle, float tx, float ty);
	int GetTargetLinePath(int sx, int sy, int tx, int ty,  int* PathX,int* PathY, int distance);
	int GetAngle(int x, int y, int tx,  int ty);
	int CalDistance(int x, int y, int x1, int y1);
	void SkillFrustrum2(int aIndex, float fangle, float tx, float ty);
	void SkillFrustrum3(int aIndex, float fangle, float tx, float ty, float f1, float f2);
	int SkillAreaCheck(int angel, int x, int y,  int tx, int ty,  int dis,  int arc);
	int SkillSpear(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int SkillFireBurst(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int SkillDarkHorseAttack(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int RecallPartyCheck(int aIndex, int skill_level);
	void SkillRecallParty(int aIndex, int skill_level);
	void SkillAddCriticalDamage(int aIndex, int skill_level, CMagicInf* lpMagic);
	void SkillElectricSpark(int aIndex, CMagicInf* lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos, int aTargetIndex);
	int SkillSummon(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int SkillImmuneToMagic(int aIndex, CMagicInf* lpMagic);
	int SkillImmuneToHarm(int aIndex, CMagicInf* lpMagic);
	int SkillKnightRush(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int SkillWizardJavalin(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int SkillElfRush(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int SkillMagumOneFlash(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int SkillMagumDeathCannon(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int SkillDarklordSpaceSplit(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int SkillBrandOfSkill(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int SkillRemoveStun(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int SkillAddMana(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int SkillCloaking(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int SkillRemoveCloaking(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int SkillRemoveMagic(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int RemoveCloakingEffect(int aIndex);
	int SkillStun(int aIndex, int aTargetIndex, CMagicInf* lpMagic, BYTE btX, BYTE btY, BYTE btDir, BYTE btTargetPos);
	int RemoveAllCharacterInvalidMagicAndSkillState(LPOBJ lpObj);
	int RemoveSpecificSkillState(LPOBJ lpObj, int iSkillNumber);
	int SkillFenrirAttack(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int SkillInfinityArrow(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int SkillFireScream(int aIndex, int aTargetIndex, CMagicInf* lpMagic);
	int FireScreamExplosionAttack(LPOBJ lpObj, LPOBJ lpTargetObj, int iAttackDamage, int iAddDamage);
	void SpecificSkillAdditionTreat(LPOBJ lpObj, LPOBJ lpTargetObj, CMagicInf* lpMagic, int iAttackDamage);
	int SkillBookSahamutt(int aIndex, CMagicInf * lpMagic, BYTE x, BYTE y, int aTargetIndex);
	int SkillSummonerSleep(int aIndex, int aTargetIndex, CMagicInf * lpMagic);
	int SkillDrainLife(int aIndex, CMagicInf * lpMagic, int aTargetIndex);
	int SkillDamageReflect(int aIndex, CMagicInf * lpMagic, int aTargetIndex);
	int SkillBookNeil(int aIndex, CMagicInf * lpMagic, BYTE x, BYTE y, int aTargetIndex);
	int SkillInnovation(int aIndex, CMagicInf * lpMagic, BYTE x, BYTE y, int aTargetIndex);
	int SkillWeakness(int aIndex, CMagicInf * lpMagic, BYTE x, BYTE y, int aTargetIndex);
	int SkillChainLighting(int aIndex, CMagicInf * lpMagic, int aTargetIndex);
	int SkillBlowOfDestruction(int aIndex, CMagicInf * lpMagic, BYTE x, BYTE y, int aTargetIndex, BOOL bCombo);
	int SkillAreaMonsterAttack(int aIndex, CMagicInf * lpMagic, BYTE x, BYTE y, int aTargetIndex, int Distance, int MultiAttack, int AttackDelay);
	void SkillLightingShock(int aIndex, CMagicInf * lpMagic, int aTargetIndex);
	int SkillMagicCircle(int aIndex, int aTargetIndex, CMagicInf * lpMagic);
	int SkillShieldRefill(int aIndex, CMagicInf * lpMagic, int aTargetIndex);
	int SkillSwordPower(int aIndex, CMagicInf * lpMagic, int aTargetIndex);
	int SkillGiganticStorm(int aIndex, CMagicInf * lpMagic, BYTE x, BYTE y, int aTargetIndex);
	int SkillFiveShot(int aIndex, CMagicInf * lpMagic, BYTE TargetPos, int aTargetIndex);
	int SkillBirds(int aIndex, CMagicInf * lpMagic, BYTE TargetPos, int aTargetIndex);
	int SkillFlameStrike(int aIndex, CMagicInf * lpMagic, BYTE TargetPos, int aTargetIndex);
	void NewSkillProc(int aIndex, CMagicInf * lpMagic, BYTE x, BYTE y, int aTargetIndex, int aMsgSubCode, BOOL bCombo);
	int SkillMonkBuff(int aIndex, CMagicInf * lpMagic);
	int SkillMonkBuffApplyParty(int aIndex, CMagicInf * lpMagic);
	int SkillMonkBarrageJustOneTarget(int aIndex, CMagicInf *lpMagic, int aTargetIndex);
	int SkillCharge(int aIndex, CMagicInf *lpMagic, int aTargetIndex);
	int SkillMonkDarkSideGetTargetIndex(int aIndex, int aTargetIndex, CMagicInf* lpMagic, WORD *Target);
	void SkillDarkSide(int aindex, int aTargetIndex, CMagicInf * lpMagic);
	int SkillPhoenixShot(int aIndex, CMagicInf * lpMagic, BYTE x, BYTE y, int aTargetIndex);
	int SkillEvilMonster(int aIndex, int aTargetIndex, CMagicInf *lpMagic);
	void SplashDamage(LPOBJ lpObj, LPOBJ lpTargetObj, int nSkill, int nAttackDamage, int nDistance, int nDamageRate);
	void SkillSummonForLordSilvester(int aIndex, int iTargetIndex, CMagicInf *lpMagic);
	void SkillSpinStep(int aIndex, int iTargetIndex, CMagicInf *lpMagic);
	void SkillGrowLancerAttackOneTarget(int aIndex, int aTargetIndex, CMagicInf *lpMagic);
	int SkillBreche(int aIndex, CMagicInf* lpMagic, BYTE x, BYTE y, int aTargetIndex);
	int SkillShiningPeak(int aIndex, CMagicInf *lpMagic, int aTargetIndex);
	int SkillBuffGrowLancer(int aIndex, CMagicInf *lpMagic, int aTargetIndex);
	int SkillBuffPartyGrowLancer(int aIndex, CMagicInf *lpMagic, int aTargetIndex);

	SkillConfig	m_SkillData;
	MULua m_Lua;
};

int  DecPartyMemberHPandMP(LPOBJ lpObj);

extern CObjUseSkill gObjUseSkill;

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

