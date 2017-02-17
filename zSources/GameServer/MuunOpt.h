//////////////////////////////////////////////////////////////////////
// MuunOpt.h
#ifndef MUUNOPT_H
#define MUUNOPT_H

#define MAX_MUUN_OPT_LEVEL_VALUE	6

enum MUUN_OPTIONS
{
	MUUN_INC_ATTACK_POWER = 1,
	MUUN_INC_DEFENSE = 2,
	MUUN_INC_EXCELLENT_DAMAGE = 3,
	MUUN_INC_CRITICAL_DAMAGE = 4,
	MUUN_INC_SKILL_POWER = 5,
	MUUN_DEC_ENEMY_DAMAGE = 6,
	MUUN_INC_MAX_ATTACK_POWER = 7,
	MUUN_INC_ATTACK_POWER_NONPVP = 8,
	MUUN_INC_EXCELLENT_DAMAGE_NONPVP = 10,
	MUUN_INC_CRITICAL_DAMAGE_NONPVP = 11,
	MUUN_INC_SKILL_POWER_NONPVP = 12,
	MUUN_INC_MAX_ATTACK_POWER_NONPVP = 14,
	MUUN_DEC_ITEM_REPAIR_FEE = 15,
	MUUN_INC_ZEN_FROM_MONSTER = 17,
	MUUN_INC_PENTAGRAM_DAMAGE = 18,
	MUUN_INC_PENTAGRAM_DEFENSE = 19,
	MUUN_ATTACK_SKILL = 50,
	MUUN_DEC_ENEMY_ATTACK_SKILL = 51,
	MUUN_ATTACK_SKILL_2 = 52,
	MUUN_ATTACK_SKILL_NONPVP = 53
};

class CMuunOpt
{
public:
	CMuunOpt();
	~CMuunOpt();

	void SetOptType(int iOptType);
	int GetOptType();

	void SetMuunLvVal(int *iMuunLvVal);
	void GetMuunLvVal(int *iOutMuunLvVal);
	int GetMuunLvVal(int iMuunLv);

	void SetConditionType(int iConditionType);
	int GetConditionType();

	void SetConditionVal1(int iConditionVal1);
	int GetConditionVal1();

	void SetConditionVal2(int iConditionVal2);
	int GetConditionVal2();

	void Clear();

private:
	int m_iOptType;
	int m_iMuunLvVal[MAX_MUUN_OPT_LEVEL_VALUE];
	int m_iConditionType;
	int m_iConditionVal1;
	int m_iConditionVal2;
};
#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

