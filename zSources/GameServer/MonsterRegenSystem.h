//////////////////////////////////////////////////////////////////////
// MonsterRegenSystem.h
#ifndef MONSTERREGENSYSTEM_H
#define MONSTERREGENSYSTEM_H

#define MAX_REGEN_AREA 50
#define MAX_MONSTER_INDEX_INFO 20
#define MAX_REGEN_TIME_TABLE 24
#define MAX_MONSTER_ACCOUNT_NUM_INFO 255

#define MAX_REGEN_MANAGED_INFO 255
#define MAX_MONSTER_POSITION 255
#define MAX_MONSTER_GROUP_INFO 255

struct _stRegenManagedInfo
{
	_stRegenManagedInfo()
	{
		this->m_nGroupNumber = -1;
		this->m_nMonsterCnt = 0;
		this->m_dwRegenTime = 0;
	}

	int m_nGroupNumber;
	int m_nMonsterCnt;
	DWORD m_dwRegenTime;
};

struct _stRegenArea
{
	_stRegenArea()
	{
		this->m_nBeginX = -1;
		this->m_nBeginY = -1;
		this->m_nEndX = -1;
		this->m_nEndY = -1;
	}

	int m_nBeginX;
	int m_nBeginY;
	int m_nEndX;
	int m_nEndY;
};

struct _stMonsterIndexInf
{
	_stMonsterIndexInf()
	{
		this->m_nMonsterIndex = -1;
		this->m_nCnt = 0;
		this->m_nOverrideDefaultSettings = 0;
		this->m_nHP = 0;
		this->m_nDamageMin = 0;
		this->m_nDamageMax = 0;
		this->m_nDefense = 0;
		this->m_nAttackRate = 0;
		this->m_nDefenseRate = 0;
		this->m_nEventId = -1;
	}

	int m_nMonsterIndex;
	int m_nCnt;
	int m_nOverrideDefaultSettings;
	int m_nEventId;
	int m_nHP;
	int m_nDamageMin;
	int m_nDamageMax;
	int m_nDefense;
	int m_nAttackRate;
	int m_nDefenseRate;
};

struct _stMonsterPosition
{
	_stMonsterPosition()
	{
		this->m_nGroupNumber = -1;
		this->m_nRegenAreaCnt = 0;
	}

	_stRegenArea m_stRegenArea[MAX_REGEN_AREA];
	_stMonsterIndexInf m_stMonsterIndexInfo[MAX_MONSTER_INDEX_INFO];
	int m_nGroupNumber;
	int m_nRegenAreaCnt;
};

struct _stRegenTimeTable
{
	_stRegenTimeTable()
	{
		this->m_nBeginHour = -1;
		this->m_nBeginMinute = -1;
		this->m_bRegen = 0;
		this->m_nAppearanceRate = 0;
		this->m_bFailedAppearanceRate = false;
	}

	int m_nBeginHour;
	int m_nBeginMinute;
	int m_nAppearanceRate;
	int m_bRegen;
	bool m_bFailedAppearanceRate;
};

struct _stMonsterAccountNumInfo
{
	int m_nIndex;

	_stMonsterAccountNumInfo()
	{
		this->m_nIndex = -1;
	}
};

struct _stMonsterGroupInfo
{
	_stMonsterGroupInfo()
	{
		this->m_nGroupNumber = -1;
		this->m_nMapNumber = -1;
		this->m_nBossMonsterIndex = -1;
		this->m_nBossMonsterClass = -1;
		this->m_nDuration = -1;
		this->m_nLiveCount = 0;
		this->m_sSpawnNotice = "";
	}

	int m_nGroupNumber;
	int m_nMapNumber;
	int m_nBossMonsterClass;
	int m_nBossMonsterIndex;
	int m_nDuration;
	int m_nLiveCount;
	std::string m_sSpawnNotice;
	_stRegenTimeTable m_stRegenTimeTable[MAX_REGEN_TIME_TABLE];
	_stMonsterAccountNumInfo m_stMonsterAccountNumInfo[MAX_MONSTER_ACCOUNT_NUM_INFO];	
};

class CMonsterRegenSystem
{
public:
	CMonsterRegenSystem(void);
	virtual ~CMonsterRegenSystem(void);

	void LoadScript(char* lpFileName);
	void Run();
	bool MonsterKillCheck(LPOBJ lpPlayer, LPOBJ lpMonster);
private:
	void RegenMonster(int nGroupNumber);
	bool SetPosMonster(int aIndex, int nMapNumber, int nBeginX, int nBeginY, int nEndX, int nEndY);
	bool GetBoxPosition(int mapnumber, int ax, int ay, int aw, int ah, short &mx, short &my);
	bool IsLiveBossState(int nGroupNumber);
	void DeleteMonster(int nGroupNumber);
	void SendAllUserAnyMsg(int iType, char *lpszMsg);
	bool IsRegenTime(int nGroupNumber, int nCurHour, int nCurMin, bool bSetRegen);


	bool						m_bLoadScript;
	int							m_nDay;
	bool						m_bSendMessage;
	_stRegenManagedInfo			m_stRegenManagedInfo[MAX_REGEN_MANAGED_INFO];
	_stMonsterPosition			m_stMonsterPosition[MAX_MONSTER_POSITION];
	_stMonsterGroupInfo			m_stMonsterGroupInfo[MAX_MONSTER_GROUP_INFO];
};

extern CMonsterRegenSystem g_MonsterRegenSystem;

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

