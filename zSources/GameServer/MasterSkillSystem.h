//////////////////////////////////////////////////////////////////////
// MasterSkillSystem.h
#ifndef MASTER_SKILL_SYSTEM
#define MASTER_SKILL_SYSTEM

#pragma pack (1)

struct PMSG_MASTER_INFO_SEND
{
    PBMSG_HEAD2 h; //C1:F3:50
	short MasterLevel;
	BYTE MLExpHHH;
	BYTE MLExpHHL;
	BYTE MLExpHLH;
	BYTE MLExpHLL;
	BYTE MLExpLHH;
	BYTE MLExpLHL;
	BYTE MLExpLLH;
	BYTE MLExpLLL;
	BYTE MLNextExpHHH;
	BYTE MLNextExpHHL;
	BYTE MLNextExpHLH;
	BYTE MLNextExpHLL;
	BYTE MLNextExpLHH;
	BYTE MLNextExpLHL;
	BYTE MLNextExpLLH; 
	BYTE MLNextExpLLL;
	WORD MasterPoint;
	WORD MaxLife;
	WORD MaxMana;
	WORD MaxShield;
	WORD MaxBP;
	int IGCLife;
	int IGCMana;
};

struct PMSG_MASTER_LEVEL_UP_SEND
{
    PBMSG_HEAD2 h;
	WORD MasterLevel;
	WORD GainPoint;
	WORD MLPoint;
	WORD MaxPoint;
	WORD MaxLife;
	WORD MaxMana;
	WORD MaxShield;
    WORD MaxBP;
	int IGCMaxLife;
	int IGCMaxMana;
};

struct PMSG_MASTER_SKILL_RECV
{
    PBMSG_HEAD2 h; //C1:F3:52
	WORD MasterSkill;
    WORD MasterEmpty;
};

struct PMSG_MASTER_SKILL_SEND
{
    PBMSG_HEAD2 h; //C1:F3:52
    BYTE type;
	BYTE flag;
	WORD MasterPoint;
	WORD MasterSkill;
    WORD MasterEmpty;
	DWORD ChkSum;
};

#pragma pack ()
class CMasterLevelSystem
{
public:
	CMasterLevelSystem(void);
	virtual ~CMasterLevelSystem(void);
	bool MasterLevelUp(LPOBJ lpObj, UINT64 addexp, int iMonsterType, const char* szEventType);
	UINT64 gObjNextMLExpCal(LPOBJ lpObj);
	bool IsMasterLevelUser(LPOBJ lpObj);
	bool CheckMLGetExp(LPOBJ lpObj, LPOBJ lpTargetObj);
	void SetExpTable();

	void SendMLData(LPOBJ lpObj);
	void InitData(LPOBJ lpObj);
	int GetDieDecExpRate(LPOBJ lpObj);
	int GetDieDecMoneyRate(LPOBJ lpObj);

	UINT64 * gMasterExperience;
};
//extern CMasterSkillSystem g_MasterLevelSystem;

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

