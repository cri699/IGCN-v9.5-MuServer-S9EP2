//////////////////////////////////////////////////////////////////////
// MasterSkillSystemS6.h
#pragma once
#include "masterskillsystem.h"
#include "user.h"

enum {
	MST_DK = 1,
	MST_DW = 2,
	MST_ELF = 4,
	MST_SUM = 8,
	MST_MG = 16,
	MST_DL = 32,
	MST_RF = 64
};

enum MST_ACTIVE_SKILLS
{
	MST_IRONDEFENSE = 323,
	MST_CYCLONE_STR = 326,
	MST_SLASH_STR = 327,
	MST_FALLINGSLASH_STR = 328,
	MST_LUNGE_STR = 329,
	MST_TWISTINGSLASH_STR = 330,
	MST_RAGEFUL_STR = 331,
	MST_TWISTINGSLASH_MAS = 332,
	MST_RAGEFUL_MAS = 333,
	MST_DEATHSTAB_STR = 336,
	MST_STRIKEOFDESTRUCTION_STR = 337,
	MST_DEATHSTAB_PRO = 339,
	MST_STRIKEOFDESTRUCTION_PRO = 340,
	MST_DEATHSTAB_MAS = 342,
	MST_STRIKEOFDESTRUCTION_MAS = 343,
	MST_BLOODSTORM = 344,
	MST_BLOODSTORM_STR = 346,
	MST_SWELLLIFE_STR = 356,
	MST_SWELLLIFE_PRO = 360,
	MST_SWELLLIFE_MAS = 363,
	MST_SM_FLAME_STR = 378,
	MST_SM_LIGHTING_STR = 379,
	MST_MAGICCIRCLE_STR = 380,
	MST_SM_INFERNO_STR = 381,
	MST_SM_BLAST_STR = 382,
	MST_MAGICCIRCLE_MAS = 383,
	MST_SM_POISON_STR = 384,
	MST_SM_EVILSPIRIT_STR = 385,
	MST_SM_DECAY_STR = 387
};



struct MASTER_SKILL_TREE_DATA
{
	int iIndex;
	int Unk1;
	int Unk2;
	int Unk3;
	int Unk4;
	int Unk5;
	int MaxLevel;
	int Unk6;
	WORD PreviousSkillReq;
	int UNK7;
	WORD SkillId;
	WORD UNK8;
	WORD UNK9;
	WORD UNK10;
	WORD UNK11;

	void Reset()
	{
		this->iIndex = 0;
		this->Unk1 = 0;
		this->Unk2 = 0;
		this->Unk3 = 0;
		this->Unk4 = 0;
		this->Unk5 = 0;
		this->MaxLevel = 0;
		this->Unk6 = 0;
		this->PreviousSkillReq = 0;
		this->UNK7 = 0;
		this->SkillId = 0;
		this->UNK8 = 0;
		this->UNK9 = 0;
		this->UNK10 = 0;
		this->UNK11 = 0;
	}
};

struct _MASTERSKILLTREEDATA_STRUCT
{
	BYTE btMasterSkillId;
	BYTE btTmp;
	WORD wType;
	BYTE btGroup;
	BYTE btRequiredPoints;
	BYTE btMaxLevel;
	BYTE btUnk3;
	int Skill1;
	int Skill2;
	int Skill3;
	float defvalue;
};
struct MASTERSKILL_INFO
{
	BYTE btMasterSkillId;
	WORD wType;
	BYTE btGroupType;
	BYTE btReqPts;
	BYTE btMaxLevel;
	BYTE btTmpUnk;
	int ReqSkillID1;
	int ReqSkillID2;
	int SkillID;
	float LevelVal[20];

	void Reset()
	{
		this->btMasterSkillId = 0;
		this->wType = 0;
		this->btGroupType= 0;
		this->btReqPts = 0;
		this->btMaxLevel = 0;
		this->btTmpUnk = 0;
		this->ReqSkillID1 = 0;
		this->ReqSkillID2 = 0;
		this->SkillID = 0;
		
		for(int i=0;i<20;i++)
		{
			this->LevelVal[i] = 0.00;
		}
	}
};

struct PMSG_ANS_ADDMASTERSKILL
{
	PBMSG_HEAD2	h;
	BYTE btResult;			// 4
	WORD wLevelPoint;		// 6
	BYTE btMasterSkillId;	// 8
	DWORD dwSkillId;		// C
	BYTE btLevel;			// 10
	float value;			// 14
	float nextvalue;		// 18
};

struct PMSG_MASTERSKILL_COUNT
{
	PWMSG_HEAD2	h;
	int count;				// 8
};

struct MASTERSKILL_STRUCT
{
	BYTE btMasterSkillId;
	BYTE btLevel;
	float value;
	float nextvalue;
};

class CMasterSkillSystemS6 :
	public CMasterLevelSystem
{
public:
	CMasterSkillSystemS6(void);
	virtual ~CMasterSkillSystemS6(void);

	void ResetData();
	bool LoadDataFile(LPSTR lpszFile);
	void ResetInfo();
	bool LoadInfoFile(LPSTR lpszFile);

	void AddMasterSkill(int aIndex, PMSG_MASTER_SKILL_RECV * lpMsg);
	void AnsAddMasterSkill(int aIndex, BYTE btResult, int MasterSkill, int level);
	void GCSendMasterSkillList(int aIndex);

	bool CheckMasterySkill(LPOBJ lpObj, int skill);

	float GetEffectValue(LPOBJ lpObj, int Skill);

	void CalcEffect(OBJECTSTRUCT* lpObj);
	void DecreasePoint(OBJECTSTRUCT* lpObj, int MasterPoint);

	bool SetSkillInfo(OBJECTSTRUCT* lpObj, unsigned char* buf);
	void ConvertSkillToByte(OBJECTSTRUCT * lpObj, unsigned char* buf);
	int AddSkillInfo(OBJECTSTRUCT* lpObj, WORD skill, BYTE level);
	int SearchSkill(OBJECTSTRUCT* lpObj, WORD skill);
	int GetSkillLevel(OBJECTSTRUCT* lpObj, WORD skill);
	void ApplyActiveSkillEffect(LPOBJ lpObj, WORD skill);
	void SkillTreeReset(LPOBJ lpObj, BYTE Type);
	
	BOOL UseMasterSkill(int aIndex, CMagicInf * lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos, int aTargetIndex);

	BOOL MLS_CheckWeaponSkill(LPOBJ lpObj, WORD Skill, WORD Level);
	BOOL MLS_CheckExistSkill(LPOBJ lpObj, WORD Skill);

private:

	BYTE FindSkillIndex(WORD Skillid, BYTE Class);
	WORD FindSkillArrayNum(WORD SkillId);
	BYTE GetClass(BYTE Class);

	MASTER_SKILL_TREE_DATA m_SkillTreeData[200];
	MASTERSKILL_INFO m_SkillInfo[512];

	// Skills

	int gObjMagicAddMLS(OBJECTSTRUCT* lpObj, WORD Skill, WORD Level);
	int CalDistance_MLS(int x, int y, int x1, int y1);

	

	
	BOOL MLS_SkillIronDefense(int aIndex, CMagicInf* lpMagic, int aTargetIndex);

	BOOL MLS_SkillCycloneStr(int aIndex, CMagicInf* lpMagic, int aTargetIndex, BOOL bCombo);
	BOOL MLS_SkillSlashStr(int aIndex, CMagicInf* lpMagic, int aTargetIndex, BOOL bCombo);
	BOOL MLS_SkillFallingSlashStr(int aIndex, CMagicInf* lpMagic, int aTargetIndex, BOOL bCombo);
	BOOL MLS_SkillLungeStr(int aIndex, CMagicInf* lpMagic, int aTargetIndex, BOOL bCombo);
	BOOL MLS_SkillWheel(int aIndex, CMagicInf* lpMagic, int aTargetIndex, BOOL bCombo);
	BOOL MLS_SkillBlowOfFury(int aIndex, CMagicInf * lpMagic, BYTE x, BYTE y, BYTE dir, BYTE TargetPos, int aTargetIndex, BOOL bCombo);
	BOOL MLS_SkillSwordOfDestructionStr(int aIndex, CMagicInf * lpMagic, BYTE x, BYTE y, int aTargetIndex, BOOL bCombo);

	BOOL MLS_KnightSkillAddLifeStr();
	BOOL MLS_KnightSkillAddLifeProf();
	BOOL MLS_KnightSkillAddLifeMas();
	
};

extern CMasterSkillSystemS6 MasterLevelSystemS6;

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

