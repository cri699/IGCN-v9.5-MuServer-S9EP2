//////////////////////////////////////////////////////////////////////
// MagicDamage.h
#ifndef MAGICDAMAGE_H
#define MAGICDAMAGE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_SKILL			800
#define MAX_REQ_SKILL_STATUS	3

#include "classdef.h"

enum eSkillType
{
	AT_SKILL_POISON = 0x1,
	AT_SKILL_METEO = 0x2,
	AT_SKILL_THUNDER = 0x3,
	AT_SKILL_FIREBALL = 0x4,
	AT_SKILL_FLAME = 0x5,
	AT_SKILL_TELEPORT = 0x6,
	AT_SKILL_SLOW = 0x7,
	AT_SKILL_STORM = 0x8,
	AT_SKILL_EVIL = 0x9,
	AT_SKILL_HELL = 0xa,
	AT_SKILL_POWERWAVE = 0xb,
	AT_SKILL_FLASH = 0xc,
	AT_SKILL_BLAST = 0xd,
	AT_SKILL_INFERNO = 0xe,
	AT_SKILL_TARGET_TELEPORT = 0xf,
	AT_SKILL_MAGICDEFENSE = 0x10,
	AT_SKILL_ENERGYBALL = 0x11,
	AT_SKILL_BLOCKING = 0x12,
	AT_SKILL_SWORD1 = 0x13,
	AT_SKILL_SWORD2 = 0x14,
	AT_SKILL_SWORD3 = 0x15,
	AT_SKILL_SWORD4 = 0x16,
	AT_SKILL_SWORD5 = 0x17,
	AT_SKILL_CROSSBOW = 0x18,
	AT_SKILL_BOW = 0x19,
	AT_SKILL_HEALING = 0x1a,
	AT_SKILL_DEFENSE = 0x1b,
	AT_SKILL_ATTACK = 0x1c,
	AT_SKILL_CALLMON1 = 0x1e,
	AT_SKILL_CALLMON2 = 0x1f,
	AT_SKILL_CALLMON3 = 0x20,
	AT_SKILL_CALLMON4 = 0x21,
	AT_SKILL_CALLMON5 = 0x22,
	AT_SKILL_CALLMON6 = 0x23,
	AT_SKILL_CALLMON7 = 0x24,
	AT_SKILL_WHEEL = 0x29,
	AT_SKILL_BLOWOFFURY = 0x2a,
	AT_SKILL_STRIKE = 0x2b,
	AT_SKILL_KNIGHTSPEAR = 0x2f,
	AT_SKILL_KNIGHTADDLIFE = 0x30,
	AT_SKILL_KNIGHTDINORANT = 0x31,
	AT_SKILL_ELFHARDEN = 0x33,
	AT_SKILL_PENETRATION = 0x34,
	AT_SKILL_DEFENSEDOWN = 0x37,
	AT_SKILL_SWORD6 = 0x38,
	AT_SKILL_5CROSSBOW = 0x36,
	AT_SKILL_EXPPOISON = 0x26,
	AT_SKILL_EXPICE = 0x27,
	AT_SKILL_EXPHELL = 0x28,
	AT_SKILL_EXPHELL_START = 0x3a,
	AT_SKILL_IMPROVE_AG_REFILL = 0x35,
	AT_SKILL_DEVILFIRE = 0x32,
	AT_SKILL_COMBO = 0x3b,
	AT_SKILL_SPEAR = 0x3c,
	AT_SKILL_FIREBURST = 0x3d,
	AT_SKILL_DARKHORSE_ATTACK = 0x3e,
	AT_SKILL_RECALL_PARTY = 0x3f,
	AT_SKILL_ADD_CRITICALDAMAGE = 0x40,
	AT_SKILL_ELECTRICSPARK = 0x41,
	AT_SKILL_LONGSPEAR = 0x42,
	AT_SKILL_RUSH = 0x2c,
	AT_SKILL_JAVALIN = 0x2d,
	AT_SKILL_DEEPIMPACT = 0x2e,
	AT_SKILL_ONE_FLASH = 0x39,
	AT_SKILL_DEATH_CANNON = 0x49,
	AT_SKILL_SPACE_SPLIT = 0x4a,
	AT_SKILL_BRAND_OF_SKILL = 0x4b,
	AT_SKILL_STUN = 0x43,
	AT_SKILL_REMOVAL_STUN = 0x44,
	AT_SKILL_ADD_MANA = 0x45,
	AT_SKILL_INVISIBLE = 0x46,
	AT_SKILL_REMOVAL_INVISIBLE = 0x47,
	AT_SKILL_REMOVAL_MAGIC = 0x48,
	AT_SKILL_FENRIR_ATTACK = 0x4c,
	AT_SKILL_INFINITY_ARROW = 0x4d,
	AT_SKILL_FIRESCREAM = 0x4e,
	AT_SKILL_EXPLOSION = 0x4f,
	AT_SKILL_SUMMON = 0xc8,
	AT_SKILL_IMMUNE_TO_MAGIC = 0xc9,
	AT_SKILL_IMMUNE_TO_HARM = 0xca,
	AT_SKILL_ILLUSION_SHIELD = 0xd2,
	AT_SKILL_ILLUSION_PARALYZE = 0xd3,
	AT_SKILL_ILLUSION_TELEPORT = 0xd4,
	AT_SKILL_ILLUSION_ATTACK = 0xd5,
	AT_SKILL_DRAIN_LIFE = 0xd6,
	AT_SKILL_CHAIN_LIGHTING = 0xd7,
	AT_SKILL_DAMAGE_REFLECT = 0xd9,
	AT_SKILL_SWORD_POWER = 0xda,
	AT_SKILL_SLEEP = 0xdb,
	AT_SKILL_WEAKNESS = 0xdd,
	AT_SKILL_INNOVATION = 0xde,
	AT_SKILL_SAHAMUTT = 0xdf,
	AT_SKILL_NEIL = 0xe0,
	AT_SKILL_RAGEUL = 0xe1,
	AT_SKILL_LIGHTING_SHOCK = 0xe6,
	AT_SKILL_BLOW_OF_DESTRUCTION = 0xe8,
	AT_SKILL_MAGIC_CIRCLE = 0xe9,
	AT_SKILL_SD_RECOVERY = 0xea,
	AT_SKILL_FIVE_SHOT = 0xeb,
	AT_SKILL_FLAME_STRIKE = 0xec,
	AT_SKILL_GIGANTIC_STORM = 0xed,
	AT_SKILL_BIRDS = 0xee,
	AT_SKILL_SUICIDE = 0xef,
	AT_SKILL_LARGE_BLOWER = 0x104,
	AT_SKILL_UPPER_BEAST = 0x105,
	AT_SKILL_CHAIN_DRIVE = 0x106,
	AT_SKILL_DARK_SIDE = 0x107,
	AT_SKILL_DRAGON_ROAR = 0x108,
	AT_SKILL_DRAGON_SLASH = 0x109,
	AT_SKILL_REDARMOR_IGNORE = 0x10a,
	AT_SKILL_FITNESS = 0x10b,
	AT_SKILL_DSRINCREASE = 0x10c,
	AT_SKILL_CHARGE = 0x10d,
	AT_SKILL_PHOENIX_SHOT = 0x10e,
	AT_SKILL_SPIN_STEP = 0x10f,
	AT_SKILL_CIRCLE_SHIELD = 0x110,
	AT_SKILL_OBSIDIAN = 0x111,
	AT_SKILL_MAGIC_PIN = 0x112,
	AT_SKILL_CLASH = 0x113,
	AT_SKILL_HARSH_STRIKE = 0x114,
	AT_SKILL_SHINING_PEAK = 0x115,
	AT_SKILL_WRATH = 0x116,
	AT_SKILL_BRECHE = 0x117,
	AT_SKILL_GL_EXPLOSION = 0x118,
	AT_SKILL_MAGIC_PIN_EXPLOSION = 0x119,
	AT_SKILL_LORDSILVESTER_SUMMON = 0x26e
};

struct MASTER_SKILL_INFO
{
	int SkillID;
	int Rank;
	int Group;
	int MasterPoint;
	int SkillType;
	int Effect;
};

class CMagicDamage
{

private:

	int Get(int skill);
	void SetEx(int iSkill, int iSkillUseType, int iSkillBrand, int iKillCount, int* pReqStatus);
	void Set(char* name, int skill, int damage,  int rlevel,  int mana,  int bp,  int dis, int Attr,  int iSkillType, unsigned char* ReqClass, int rEnergy, int rLeadership, int rdelay, BYTE byItemSkill, BYTE byItemDamage, BYTE byBufIndex);
	void SetMasterLevelSkillInfo(int iSkill, int iSkillRank, int iSkillGroup, int iRequireMLPoint, int iHP, int iSD, WORD wKeepTime, int iRequireStr, int iRequireDex);
	int CheckSkillAttr(int skill, int attr);

public:

	CMagicDamage();
	virtual ~CMagicDamage();

	void Init();
	void LogSkillList(char* filename);
	int SkillGet(int skill);
	int SkillGetMana(int skill);
	int SkillGetRequireEnergy(int skill);
	int SkillGetRequireLevel(int skill);
	int SkillGetRequireClass(int Cclass, int ChangeUP, int skill);
	int SkillGetBP(int skill);
	int GetSkillAttr(int skill);
	int GetskillRequireLeadership(int skill);
	int GetDelayTime(int skill);
	int GetSkillType(int iSkill);
	int CheckStatus(int iSkill, int iGuildStatus);
	int CheckBrandOfSkill(int iSkill);
	int CheckKillCount(int iSkill, int iKillCount);
	int GetSkillDistance(int skill);
	int GetRequireMLPoint(int skill);
	int GetRequireStrength(int skill);
	int GetRequireDexterity(int skill);
	WORD GetBrandOfSkill(int iSkill);
	WORD GetNormalBrandOfSkill(int iSkill);
	BYTE IsItemAttachedSkill(int iSkill);
	int GetSkillUseType(int iSkill);
	int GetSkillLevel(int iSkill);
	int GetSkillGroup(int iSkill);
	int GetSkillRank(int iSkill);
	int GetSkillKeepTime(int iSkill);
	char m_Name[MAX_SKILL][50];	// 404

private:
	int m_Damage[MAX_SKILL];	// 4
	unsigned char m_rLevel[MAX_SKILL];	// 3604
	unsigned short m_Mana[MAX_SKILL];	// 3704
	unsigned char m_Distance[MAX_SKILL];	// 3904
	int m_RequireEnergy[MAX_SKILL];	// 3A04
	unsigned char m_RequireClass[MAX_SKILL][MAX_TYPE_PLAYER+1];	// 3E04
	int m_RequireLevel[MAX_SKILL];	// 4304
	int m_RequireLeadership[MAX_SKILL];	// 4704
	int m_Delay[MAX_SKILL];	// 4B04
	WORD m_BrainPower[MAX_SKILL];	// 4F04
	int m_Attr[MAX_SKILL];	// 5104
	int m_iSkillType[MAX_SKILL];	// 5504
	int m_iSkillUseType[MAX_SKILL];	// 5904
	int m_iSkillBrand[MAX_SKILL];	// 5D04
	int m_iKillCount[MAX_SKILL];	// 6104
	int m_iRequireStatus[MAX_SKILL][MAX_REQ_SKILL_STATUS];	// 6504
	WORD m_iSkillRank[MAX_SKILL];
	WORD m_iSkillGroup[MAX_SKILL];
	WORD m_nRequirePoint[MAX_SKILL];
	WORD m_iHP[MAX_SKILL];
	WORD m_iSD[MAX_SKILL];
	WORD m_wKeepTime[MAX_SKILL];
	WORD m_iRequireStr[MAX_SKILL];
	WORD m_iRequireDex[MAX_SKILL];
	BYTE m_byItemAttachedSkill[MAX_SKILL];
	BYTE m_byIsDamage[MAX_SKILL];
	BYTE m_byBufIndex[MAX_SKILL];

	enum 
	{
		SKILL_TYPE_NONE = 0xff,
		SKILL_TYPE_PHYSICS = 0x0,
		SKILL_TYPE_MAGIC = 0x1,
		SKILL_TYPE_PASSIVE = 0x2,
		SKILL_TYPE_MASTER_PASSIVE = 0x3,
		SKILL_TYPE_MASTER_MAGIC = 0x4,
	};

};

extern CMagicDamage MagicDamageC;

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

