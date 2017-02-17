//////////////////////////////////////////////////////////////////////
// zzzitem.h
// ------------------------------
// Decompiled by Deathway
// Date : 2007-03-09
// ------------------------------
#ifndef ZZZITEM_H
#define ZZZITEM_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_TYPE_PLAYER 8 
#define MAX_ITEM_LEVEL	15
// ItemAttribute Begin:9439368 END 9446B68 Array[512]

// **************************
// Item Limits
// **************************
#define MAX_TYPE_ITEMS 16
#define MAX_SUBTYPE_ITEMS 512

#define MAX_EXOPTION_SIZE 8

// ********************
// MACRO for get Item
// ********************
#define ITEMGET(x,y) ( (x)*MAX_SUBTYPE_ITEMS + (y))

// **************
// Pet Level
// **************
#define MAX_PET_LEVEL 70

#define MAX_ITEM_DURABILITY	255

#define MAX_ITEMS (MAX_TYPE_ITEMS*MAX_SUBTYPE_ITEMS)

#define MAX_ITEM_SPECIAL_ATTRIBUTE 7

#define DBI_GET_TYPE(x) ( ((x)&((MAX_TYPE_ITEMS-1)<<8))>>4 )
#define DBI_GET_INDEX(x) ( (x) & 0xFF )
#define DBI_GET_LEVEL(x)  ( ((x)>>3)& 0xF )
#define DBI_GET_SKILL(x)  ( ((x) >> 7) & 0x01)
#define DBI_GET_LUCK(x)  ( ((x) >> 2) & 0x01)
#define DBI_GET_OPTION(x)  ( ( (x) ) & 0x03 )
#define DBI_GET_OPTION16(x) ( (x) & 0x40  )
#define DBI_GET_DINO_OPTION(x)  ( DBI_GET_OPTION16((x)) >> 4 )
#define DBI_GET_NOPTION(x)  ( (x) & 0x3F  )
#define DBI_GET_380OPTION(x) ( ((x) & 0x08) << 4  )
#define ITEM_GET_TYPE(x)  ( (x)/MAX_SUBTYPE_ITEMS  )
#define ITEM_GET_INDEX(x) ( (x)%MAX_SUBTYPE_ITEMS  )

#define DBI_TYPE			0
#define DBI_OPTION_DATA		1
#define DBI_DUR				2
#define DBI_SERIAL1			3
#define DBI_SERIAL2			4
#define DBI_SERIAL3			5
#define DBI_SERIAL4			6
#define DBI_NOPTION_DATA	7
#define DBI_SOPTION_DATA	8
#define DBI_OPTION380_DATA	9
#define DBI_JOH_DATA		10
#define DBI_SOCKET_1		11
#define DBI_SOCKET_2		12
#define DBI_SOCKET_3		13
#define DBI_SOCKET_4		14
#define DBI_SOCKET_5		15
#define DBI_SERIAL5			16
#define DBI_SERIAL6			17
#define DBI_SERIAL7			18
#define DBI_SERIAL8			19

#define MAX_ITEM_INFO		12
#define MAX_DBITEM_INFO		32

#define I_TYPE		0
#define I_OPTION	1
#define I_DUR		2
#define I_NOPTION	3
#define I_SOPTION	4
#define I_380OPTION	5
#define I_JOHOPTION	6
#define I_SOCKET1	7
#define I_SOCKET2	8
#define I_SOCKET3	9
#define I_SOCKET4	10
#define I_SOCKET5	11

#define MAX_SOCKET_OPTION 5

#define ITEMTYPE_COMMON		0
#define ITEMTYPE_NORMAL		1
#define ITEMTYPE_SOCKET		2
#define ITEMTYPE_380		3
#define ITEMTYPE_LUCKY		4
#define ITEMTYPE_EVENT		5
#define ITEMTYPE_ANGEL		6
#define ITEMTYPE_CHAOS		7

enum DETAIL_ITEM_TYPE
{
	ITEM_TWOHAND_SWORD = 0,
	ITEM_ONEHAND_SWORD = 1,
	ITEM_MACE = 2,
	ITEM_SPEAR = 3,
	ITEM_ONEHAND_STAFF = 4,
	ITEM_TWOHAND_STAFF = 5,
	ITEM_SHIELD = 6,
	ITEM_BOW = 7,
	ITEM_CROSSBOW = 8,
	ITEM_SUMMONER_STAFF = 9,
	ITEM_SUMMONER_BOOK = 10,
	ITEM_DARKLORD_SCEPTER = 11,
	ITEM_RAGEFIGHTER_WEAPON = 12
};

enum ITEM_KIND_TYPE_A
{
	ITEM_KIND_A_COMMON = 0,
	ITEM_KIND_A_WEAPON = 1,
	ITEM_KIND_A_PENDANT = 2,
	ITEM_KIND_A_ARMOR = 3,
	ITEM_KIND_A_RING = 4,
	ITEM_KIND_A_HELPER = 5,
	ITEM_KIND_A_WING = 6,
	ITEM_KIND_A_PREMIUM_BUFF = 7,
	ITEM_KIND_A_PENTAGRAM_ITEM = 8,
	ITEM_KIND_A_JEWEL = 9,
	ITEM_KIND_A_SKILL_ITEM = 10,
	ITEM_KIND_A_EVENT_INVENTORY_ITEM = 11,
	ITEM_KIND_A_MUUN_INVENTORY_ITEM = 12,
};

enum ITEM_KIND_TYPE_B
{
	ITEM_KIND_B_COMMON = 0,
	ITEM_KIND_B_SWORD_KNIGHT = 1,
	ITEM_KIND_B_SWORD_MAGUMSA = 2,
	ITEM_KIND_B_SWORD_RAGEFIGHTER = 3,
	ITEM_KIND_B_AXE = 4,
	ITEM_KIND_B_MACE = 5,
	ITEM_KIND_B_SCEPTER = 6,
	ITEM_KIND_B_SPEAR = 7,
	ITEM_KIND_B_BOW = 8,
	ITEM_KIND_B_CROSSBOW = 9,
	ITEM_KIND_B_ARROW = 10,
	ITEM_KIND_B_BOLT = 11,
	ITEM_KIND_B_STAFF_WIZARD = 12,
	ITEM_KIND_B_STAFF_SUMMONER = 13,
	ITEM_KIND_B_BOOK_SUMMONER = 14,
	ITEM_KIND_B_SHIELD = 15,
	ITEM_KIND_B_HELMET = 16,
	ITEM_KIND_B_ARMOR = 17,
	ITEM_KIND_B_PANTS = 18,
	ITEM_KIND_B_GLOVES = 19,
	ITEM_KIND_B_BOOTS = 20,
	ITEM_KIND_B_WING_TALISMAN = 21,
	ITEM_KIND_B_SMALL_WING = 22,
	ITEM_KIND_B_WING_1ST = 23,
	ITEM_KIND_B_WING_2ND = 24,
	ITEM_KIND_B_WING_3RD = 25,
	ITEM_KIND_B_LORD_CAPE = 26,
	ITEM_KIND_B_RAGEFIGHTER_CAPE = 27,
	ITEM_KIND_B_MONSTER_WING = 28,
	ITEM_KIND_B_PENDANT_PHYSICAL_ATTACK = 29,
	ITEM_KIND_B_PENDANT_MAGIC_ATTACK = 30,
	ITEM_KIND_B_RING = 31,
	ITEM_KIND_B_EVENT_CHANGE_RING = 32,
	ITEM_KIND_B_COMMON_CHANGE_RING = 33,
	ITEM_KIND_B_PREMIUM_PENDANT = 34,
	ITEM_KIND_B_PREMIUM_RING = 35,
	ITEM_KIND_B_SKILL_WIZARD_MAGUMSA = 36,
	ITEM_KIND_B_SKILL_SUMMONER = 37,
	ITEM_KIND_B_SKILL_KNIGHT_ELF_MAGUMSA = 38,
	ITEM_KIND_B_DARKLORD = 39,
	ITEM_KIND_B_SOCKET_SEED = 40,
	ITEM_KIND_B_SOCKET_SPHERE = 41,
	ITEM_KIND_B_SOCKET_SEED_SPHERE = 42,
	ITEM_KIND_B_PENTAGRAM_ITEM = 43,
	ITEM_KIND_B_PENTAGRAM_JEWEL = 44,
	ITEM_KIND_B_HELPER = 45,
	ITEM_KIND_B_EQUIPMENT_ITEM = 47,
	ITEM_KIND_B_PREMIUM_SCROLL = 48,
	ITEM_KIND_B_PREMIUM_ELIXIR = 49,
	ITEM_KIND_B_PREMIUM_POTION = 50,
	ITEM_KIND_B_POTION = 51,
	ITEM_KIND_B_RIBBON_BOX = 52,
	ITEM_KIND_B_EVENT_INVITATION = 53,
	ITEM_KIND_B_TICKET = 54,
	ITEM_KIND_B_QUEST_ITEM = 55,
	ITEM_KIND_B_COMMON_JEWEL = 56,
	ITEM_KIND_B_PACKAGE_JEWEL = 57,
	ITEM_KIND_B_JEWELOFHARMONY_GEMSTONE = 58,
	ITEM_KIND_B_JEWELOFHARMONY_REFINE_STONE = 59,
	ITEM_KIND_B_CCF_WING = 60,
	ITEM_KIND_B_GOLDCOLOSSUS_WING = 62,
	ITEM_KIND_B_MUUN_ITEM = 63,
	ITEM_KIND_B_MUUN_EXCHANGE = 67,
};

#define ITEM_DUR_RANGE(x) (((x)<0)?FALSE:((x)>MAX_ITEM_DURABILITY-1)?FALSE:TRUE )

typedef struct
{
	char Name[96]; // 0
	BYTE HaveItemInfo; // 20
	BYTE TwoHand;	// 21
	BYTE Level; // 22
	BYTE Width;	// 23
	BYTE Height;	// 24
	char Serial; // 25
	BYTE OptionFlag; // 26
	BYTE MondownFlag; // 27
	BYTE AttackSpeed; // 28
	BYTE WalkSpeed; // 29
	BYTE DamageMin; // 2A
	BYTE DamageMax; // 2B
	BYTE SuccessfulBlocking; // 2C
	BYTE Defense; // 2D
	BYTE MagicDefense; // 2E
	BYTE Speed;	// 2F
	BYTE Durability; // 30
	BYTE MagicDurability; // 31
	BYTE AttackDur;	// 32
	BYTE DefenceDur; // 33
	WORD RequireStrength; // 34
	WORD RequireDexterity; // 36
	WORD RequireEnergy; // 38
	WORD RequireLevel; // 3A
	WORD Value; // 3C
	BYTE RequireClass[MAX_TYPE_PLAYER]; // 3E
	BYTE Resistance[MAX_ITEM_SPECIAL_ATTRIBUTE]; // unk43 - Ice poisonous lightning non ground wind water
	WORD RequireVitality;	// 4A
	int BuyMoney; // 4C
	int MagicPW; // 50
	float RepaireMoneyRate;	// 54
	float AllRepaireMoneyRate; // 58
	bool QuestItem;	// 5C
	BYTE SetAttr; // 5D
	BYTE ResistanceType;	// 5E
	int ItemSlot; // 60
	int SkillType; // 64
	int RequireLeadership;	// 68
	int ItemCategory;
	char ModelPath[32];
	char ModelFile[32];
	BYTE Dump;
	BYTE Transaction;
	BYTE PersonalStore;
	BYTE StoreWarehouse;
	BYTE SellToNPC;
	BYTE Repair;
	BYTE ItemKindA;
	BYTE ItemKindB;
	BYTE ItemOverlap;

}  ITEM_ATTRIBUTE, * LPITEM_ATTRIBUTE;

extern  ITEM_ATTRIBUTE ItemAttribute[MAX_ITEMS];


class CItem
{

public:

	CItem();

	void Convert(int type, BYTE Option1, BYTE Option2, BYTE Option3, BYTE Attribute2, BYTE SetOption, BYTE ItemEffectEx, BYTE *SocketOption, BYTE SocketBonusOption, BYTE PeriodItemOption, BYTE DbVersion);
	void Value();
	void OldValue();
	int GetSize(int& w, int& h);
	void Clear();
	int IsItem();
	int IsSetItem();
	int GetAddStatType();
	int GetDetailItemType();
	void SetPetItemInfo(int petlevel, UINT64 petexp);
	int AddPetItemExp(UINT64 petexp);
	int DecPetItemExp(int percent);
	void PetValue();
	int PetItemLevelDown(UINT64 exp);
	int ItemDamageMin();
	int ItemDefense();
	int IsClass(char aClass, int ChangeUP);
	LPSTR GetName();
	int GetLevel();
	void PlusSpecial(int* Value, int Special);
	void PlusSpecialPercent(int* Value, int Special, WORD Percent);
	void PlusSpecialPercentEx(int* Value, int SourceValue, int Special);
	void PlusSpecialPercentEx(float* Value, float SourceValue, float Special);
	void SetItemPlusSpecialStat(int* Value, int Special);
	int GetWeaponType();
	void PlusSpecialSetRing(unsigned char* Value);
	UINT64 GetNumber();
	int IsExtItem();
	int IsDinorantReduceAttackDamaege();
	int IsFenrirIncLastAttackDamage();
	int IsFenrirDecLastAttackDamage();
	int IsFenrirIllusionDecDmg();
	int IsFenrirIllusion();				//Golden Fenrir
	int IsFenrirAddExp();
	int SimpleDurabilityDown(int iDur);
	int DurabilityDown(int dur, int aIndex);
	int DurabilityDown2(int dur, int aIndex);
	int NormalWeaponDurabilityDown(int defence, int aIndex);
	int BowWeaponDurabilityDown(int defence, int aIndex);
	int StaffWeaponDurabilityDown(int defence, int aIndex);
	int LuckyItemArmorDurabilityDown(int damagemin, int aIndex);
	int ArmorDurabilityDown(int damagemin, int aIndex);
	int CheckDurabilityState();
	int IsCurseSpell();
	bool IsMuunItemPeriodExpire();
	void SetMuunItemPeriodExpire();
	void SetMuunItemPeriodReset();
	int GetMuunItemRank();
	//Season 4 EP1
	void MakeSocketSlot(BYTE SlotCount);

	// Cash Shop
	void SetPeriodItem();
	void SetPeriodItemExpire();
	BOOL IsPeriodItem();
	BOOL IsPeriodItemExpire();


	

	UINT64 m_Number; // 0
	char m_serial;	// 4
	short m_Type; // 6
	short m_Level; // 8
	BYTE m_Part;	// A
	BYTE m_Class;	// B
	BYTE m_TwoHand;	// C
	BYTE m_AttackSpeed;	// D
	BYTE m_WalkSpeed;	// E
	WORD m_DamageMin;	// 10
	WORD m_DamageMax;	// 12
	BYTE m_SuccessfulBlocking;	// 14
	WORD m_Defense;	// 16
	WORD m_MagicDefense;	// 18
	BYTE m_Speed;	// 1A
	WORD m_DamageMinOrigin;	// 1C
	WORD m_DefenseOrigin;	// 1E
	WORD m_Magic;	// 20
	WORD m_CurseSpell;
	float m_Durability; // 24
	WORD m_DurabilitySmall; // 28
	float m_BaseDurability;	// 2C
	BYTE m_SpecialNum;	// 30
	WORD m_Special[8];	// 31
	BYTE m_SpecialValue[8];	// 39
	WORD m_RequireStrength;	// 42
	WORD m_RequireDexterity;	// 44
	WORD m_RequireEnergy;	// 46
	WORD m_RequireLevel;	// 48
	WORD m_RequireVitality;	// 4A
	WORD m_RequireLeaderShip;	// 4C
	WORD m_Leadership;	// 4E
	BYTE m_RequireClass[MAX_TYPE_PLAYER];	// 50
	BYTE m_Resistance[MAX_ITEM_SPECIAL_ATTRIBUTE];	// 55
	int m_Value;	// 5C
	UINT64 m_SellMoney;	// 60
	UINT64 m_BuyMoney;	// 64
	int m_iPShopValue;	// 68
	bool m_bItemExist;	// 6C
	int m_OldSellMoney;	// 70
	int m_OldBuyMoney;	// 74
	BYTE m_Option1;	// 78  Skill
	BYTE m_Option2;	// 79  Luck
	BYTE m_Option3;	// 7A  Option
	BYTE m_NewOption;	// 7B ExcellentOption
	float m_DurabilityState[4];	// 7C
	float m_CurrentDurabilityState;	// 8C
	bool m_SkillChange;	// 90
	bool m_QuestItem;	// 91
	BYTE m_SetOption;	// 92
	BYTE m_SetAddStat;	// 93
	bool m_IsValidItem;	// 94
	BYTE m_SkillResistance[MAX_ITEM_SPECIAL_ATTRIBUTE];	// 95
	BOOL m_IsLoadPetItemInfo;	// 9C
	int  m_PetItem_Level;	// A0
	UINT64 m_PetItem_Exp; // A4
	BYTE m_JewelOfHarmonyOption;	// A8
	WORD m_HJOpStrength;	// AA
	WORD m_HJOpDexterity;	// AC
	BYTE m_ItemOptionEx;	// AE
	BYTE m_BonusSocketOption;
	BYTE m_SocketOption[MAX_SOCKET_OPTION];
	BYTE m_ImproveDurabilityRate;
	BYTE m_PeriodItemOption; // bit0 : period item flag, bit1 : expired item
	short m_wPShopBlessValue;
	short m_wPShopSoulValue;
	short m_wPShopChaosValue;
};

class CPetItemExp
{

public:

	UINT64 m_DarkSpiritExpTable[MAX_PET_LEVEL+2];
	UINT64 m_DarkHorseExpTable[MAX_PET_LEVEL+2];

public:

	CPetItemExp()
	{
		this->m_DarkSpiritExpTable[0] = 0;
		this->m_DarkSpiritExpTable[1] = 0;

		for ( int i = 2 ; i < MAX_PET_LEVEL+2 ; i++ )
		{
			this->m_DarkSpiritExpTable[i] = 100i64 * (UINT64)(i+10) * (UINT64)i * (UINT64)i * (UINT64)i;
		}

		this->m_DarkHorseExpTable[0] = 0;
		this->m_DarkHorseExpTable[1] = 0;

		for (int i = 2 ; i < MAX_PET_LEVEL+2 ; i++ )
		{
			this->m_DarkHorseExpTable[i] = 100i64 * (UINT64)(i+10) * (UINT64)i * (UINT64)i * (UINT64)i;
		}
	};
};

struct RESETITEMLIST
{
	int itemIndex;
	int iResets;
}; extern RESETITEMLIST gObjResetItem[1000];

extern int g_MaxItemIndexOfEachItemType[MAX_TYPE_ITEMS];

void BufferItemtoConvert3(unsigned char* buf, int& type, BYTE& level, BYTE& op1, BYTE& op2, BYTE& op3, BYTE& dur);
void ItemByteConvert7(unsigned char * buf, CItem * const item , int maxitem);
void ItemByteConvert10(unsigned char * buf, CItem * const item , int maxitem);
void ItemByteConvert16(LPBYTE buf, CItem * const item , int maxitem);
void ItemByteConvert32(LPBYTE buf, CItem * const item , int maxitem);
void ItemByteConvert(unsigned char* buf, CItem item);
void ItemByteConvert(unsigned char* buf, int type, BYTE Option1, BYTE Option2, BYTE Option3, BYTE level, BYTE dur, BYTE Noption, BYTE SetOption, BYTE JewelOfHarmonyOption, BYTE ItemEffectEx, BYTE *SocketOption, BYTE SocketBonusOption, BYTE PeriodItemOption);
int ItemGetNumberMake(int type, int index);
void ItemGetSize(int index, int & width, int & height);
BOOL HasItemDurability(int index);
int ItemGetDurability(int index, int itemLevel, int ExcellentItem, int SetItem);
int ItemGetAttackDurability(int index);
int ItemGetDefenseDurability(int index);
float GetRepairItemRate(int index);
float GetAllRepairItemRate(int index);
void CalRepairRate( int itemtype, int itemsubtype, LPITEM_ATTRIBUTE p);
BYTE CalcExcOption(int MaxOpt);
BOOL OpenItemScript(char* FileName);
int zzzItemLevel(int type, int index, int level);
int GetLevelItem(int type, int index, int level);
int GetSerialItem(int type);
int IsItem(int item_num);
LPITEM_ATTRIBUTE GetItemAttr(int item_num);
int GetItemGroup(int item_num);
int IsDumpItem(int item_num);
int IsTransactionItem(int item_num);
int IsPersonalStoreItem(int item_num);
int IsStoreWarehouseItem(int item_num);
int IsSellToNPCItem(int item_num);
int IsRepairItem(int item_num);
int IsOverlapItem(int item_num);
void LoadResetItemList(LPSTR szFile);
int IsJumpingEventItem(int iItemCode);
int IsExpensiveItem(CItem * item);
int GetItemKindA(int item_num);
int GetItemKindB(int item_num);
bool CheckCanWearResetItem(int aIndex, int itemid);

extern CPetItemExp gPetItemExp;
#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

