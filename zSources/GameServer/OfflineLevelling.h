//////////////////////////////////////////////////////////////////////
// OfflineLevelling.h
#ifndef OFFLINELEVELLING_H
#define OFFLINELEVELLINH_H
enum SkillTypes
{
	MAGIC_ATTACK,
	BE_ATTACK,
	DURATION_MAGIC
};
struct OFF_LEVEL_ITEM_PICK_SETTINGS
{
	bool bPickUpItems;
	int PickUpZen;
	int PickUpExc;
	int PickUpSocket;
	int PickUpAncient;
};
struct OFF_LEVEL_ITEM_PICK_LIST
{
	WORD ItemId;
};
struct OFF_LEVEL_GENERAL_SETTINGS
{
	int VipType;
	int CoinType;
	int CoinValue;
	int Immortal;
	int AutoRepairItems;
	DWORD HitInterval;
	DWORD ChargeInterval;
	DWORD MaxDuration;
	DWORD MaxVipDuration;
};
struct OFF_LEVEL_PLAYERS
{
	int aIndex;
	WORD wSkillNumber;
	DWORD dwOffTime;
};

struct OFF_LEVEL_PER_MAP_ATTRIBUTE
{
	int Disable;
	int CoinType;
	int CoinValue;

};

struct OFF_LEVEL_CLASS_SKILLS
{
	WORD SkillNumber[5];
};

struct OFF_SKILL_CATEGORIES
{
	WORD SkillId;
	BYTE SkillType; 
};
class COfflineLevelling
{
public:
	COfflineLevelling();
	~COfflineLevelling();
	void Initiate();
	void Run();
	BOOL LoadFile(LPCSTR szFile);
	BOOL LoadSkillDefinitions(LPCSTR szFile);
	BOOL AddUser(int aIndex, WORD wSkillId);
	BOOL DeleteUser(int aIndex);
	int FindUser(int aIndex);
	void FindAndAttack(LPOBJ user);
	bool ChargePlayer(int aIndex);
	int GetSkillAttackType(WORD wSkillID);
	BOOL SkillDistanceCheck(int aIndex, int aTargetIndex, int iSkillNum);
	int GetOffLevelerCount();
	bool CheckUseTime(int aIndex);
	void CheckAndPickUpItem(int aIndex, CMapItem* cMapItem,short tObjNum);
	void CheckRepairItems(int aIndex);
	// var
	std::map<int, OFF_LEVEL_PLAYERS> m_OffPlayerData;
	CRITICAL_SECTION m_OfflevelCriti;
	OFF_LEVEL_GENERAL_SETTINGS m_General;
	OFF_LEVEL_PER_MAP_ATTRIBUTE m_PerMapAttr[101];
	OFF_LEVEL_CLASS_SKILLS m_SkillDef[7];
	OFF_SKILL_CATEGORIES m_SkillCategories[700];
	OFF_LEVEL_ITEM_PICK_SETTINGS m_PickItemSettings;
	std::map<int, OFF_LEVEL_ITEM_PICK_LIST> m_PickItems;
	bool b_Enabled;
}; extern COfflineLevelling g_OffLevel;

void OffLevelThread();

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

