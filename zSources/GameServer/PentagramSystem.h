//////////////////////////////////////////////////////////////////////
// PentagramSystem.h
#ifndef PENTAGRAMSYSTEM_H
#define PENTAGRAMSYSTEM_H

#include "zzzitem.h"
#include "user.h"

#define EL_NONE			0
#define EL_FIRE			1
#define EL_WATER		2
#define EL_EARTH		3
#define EL_WIND			4
#define EL_DARKNESS		5

#define IN_INVENTORY	0
#define IN_WAREHOUSE	1
#define IN_TRADE		2
#define IN_PSHOP		3

#define EL_SETOPTION_NONE	0
#define EL_SETOPTION_ADD_DAMAGE 1
#define EL_SETOPTION_ADD_DEFENSE 2
#define EL_SETOPTION_ADD_CRITICALDMG 3
#define EL_SETOPTION_ADD_FROM_NORMAL_DEFENSE 4
#define EL_SETOPTION_ADD_FROM_NORMAL_DAMAGE 5
#define EL_SETOPTION_ADD_RESIST_AGAINST_ELEMENT 6
#define EL_SETOPTION_ADD_RESIST_AGAINST_ATTACK 7

struct SERVER_ATTRIBUTE_DEFINE
{
	void Clear()
	{
		this->ListIndex = 0;
		this->ServerIndex = 0;
		this->FireRate = 0;
		this->WaterRate = 0;
		this->WindRate = 0;
		this->EarthRate = 0;
		this->DarkRate = 0;
	}

	int ListIndex;
	int ServerIndex;
	int FireRate;
	int WaterRate;
	int WindRate;
	int EarthRate;
	int DarkRate;
};

struct PENTAGRAM_ITEM_OPEN_SOCKET_RATE
{
	void Clear()
	{
		this->SocketCount = 0;
		this->SocketOpenRate = 0;
		this->SocketOpenSet = 0;
		this->Slot_1 = 0;
		this->Slot_2 = 0;
		this->Slot_3 = 0;
		this->Slot_4 = 0;
		this->Slot_5 = 0;
	}

	int SocketCount;
	int SocketOpenRate;
	int SocketOpenSet;
	int Slot_1;
	int Slot_2;
	int Slot_3;
	int Slot_4;
	int Slot_5;
};

struct MONSTER_DROP_ITEM_RATE
{
	void Clear()
	{
		this->MonsterClass = 0;

		for (int i = 0; i<5; i++)
		{
			this->DropItemType[i] = 0;
			this->DropItemIndex[i] = 0;
			this->DropItemRate[i] = 0;
		}
	}

	int MonsterClass;
	int DropItemType[6];
	int DropItemIndex[6];
	int DropItemRate[6];
};

struct JEWEL_OUT_RATE
{
	void Clear()
	{
		this->JewelOutIndex = 0;
		this->JewelRank = 0;
		this->JewelLevel = 0;
		this->OutRate = 0;
	}

	int JewelOutIndex;
	int JewelRank;
	int JewelLevel;
	int OutRate;
};

struct PENTAGRAM_SET_EFFECT
{
	void Clear()
	{
		this->SetIndex = 0;

		for (int i = 0; i < 6; i++)
		{
			this->RON[i] = 0;
		}

		this->Value1 = 0;
		this->Value2 = 0;
		this->Value3 = 0;
		this->Value4 = 0;
	}

	int SetIndex;
	int RON[6];
	int Value1;
	int Value2;
	int Value3;
	int Value4;
};

struct PENTAGRAM_HAVE_SET_OPTION
{
	int BundleIndex;
	int ItemType;
	int ItemIndex;
	int SetOptionIndex[26];
};

struct PENTAGRAM_ITEM_OPTION
{
	void Clear()
	{
		this->ItemType = 0;
		this->ItemIndex = 0;
		memset(this->ItemName, 0, sizeof(this->ItemName));
		this->Grade = 0;
		
		for (int i = 0; i < 7; i++)
		{
			this->OptionNum[i] = -1;
		}
	}

	int ItemType;
	int ItemIndex;
	char ItemName[64];
	int Grade;
	int OptionNum[7];
};

struct PENTAGRAM_ITEM_OPTION_ENABLE_NEED
{
	void Clear()
	{
		this->OptionNum = 0;
		memset(this->OptionName, 0, sizeof(this->OptionName));

		for (int i = 0; i < 3; i++)
		{
			this->Need_ErrtelKind[i] = -1;
			this->Need_ErrtelRank[i] = -1;
			this->Need_ErrtelLevel[i] = -1;
		}
	}

	int OptionNum;
	char OptionName[64];
	int Need_ErrtelKind[3];
	int Need_ErrtelRank[3];
	int Need_ErrtelLevel[3];
};

struct PENTAGRAM_SOCKET_RATE_BY_GRADE
{
	void Clear()
	{
		this->Grade = 0;

		for (int i = 0; i < 5; i++)
		{
			this->SocketRate[i] = 0;
		}
	}

	int Grade;
	int SocketRate[5];
};

#pragma pack (1)
struct PMSG_ANS_PENTAGRAMJEWEL
{
	PWMSG_HEAD h;
	BYTE btJewelCnt;
	int iUserIndex;
	int iAnsType;
};
#pragma pack ()

class CPentagramSystem
{
public:
	CPentagramSystem(void);
	virtual ~CPentagramSystem(void);

	void Initialize_Drop();
	bool LoadDropScript(char* pchFileName);
	void Initialize_JewelOutRate();
	bool LoadJewelOutRate(char* pchFileName);
	void Initialize_SetOption();
	bool LoadPentagramSetOptionScript(char* pchFileName);
	void Initialize_PentagramItemOption();
	bool LoadPentagramOptionScript(char* pchFileName);

	bool IsPentagramItem(CItem *lpItemData);
	bool IsPentagramItem(int ItemCode);

	bool IsPentagramJewel(CItem *lpItemData);
	bool IsPentagramJewel(int ItemCode);

	void ClearPentagramItem(int aIndex);
	void CalcPentagramItem(int aIndex, CItem *lpItemData);

	bool IsEnableDropPentagramItemMap(int iMapIndex);
	int AttributeMonsterItemDrop(OBJECTSTRUCT *lpObj);

	BYTE GetMakePentagramSlotCountNKind(BYTE *btEnableSlot, int iType);
	bool SetPentagramMainAttribute(CItem *lpItemData, BYTE btAttributeNumber);
	bool MakePentagramSocketSlot(CItem *lpItemData, BYTE btSocketSlot1, BYTE btSocketSlot2, BYTE btSocketSlot3, BYTE btSocketSlot4, BYTE btSocketSlot5);
	bool ClearPentagramSocketSlot(int aIndex, int iInventoryPos, CItem *lpTargetItem, BYTE btSocketSlotIndex);
	bool SetPentagramSocketSlot(CItem *lpTargetItem, BYTE bt1RankOptionNum, BYTE bt1RankLevel, BYTE bt2RankOptionNum, BYTE bt2RankLevel, BYTE bt3RankOptionNum, BYTE bt3RankLevel, BYTE bt4RankOptionNum, BYTE bt4RankLevel, BYTE bt5RankOptionNum, BYTE bt5RankLevel, BYTE curRank);

	bool SwitchPentagramJewel(int aIndex, CItem *lpSourceItem, int iSwitchType);
	bool AddPentagramJewelInfo(int aIndex, int iJewelPos, int iJewelIndex, int iItemType, int iItemIndex, int iMainAttribute, int iJewelLevel, BYTE btRank1OptionNum, BYTE btRank1Level, BYTE btRank2OptionNum, BYTE btRank2Level, BYTE btRank3OptionNum, BYTE btRank3Level, BYTE btRank4OptionNum, BYTE btRank4Level, BYTE btRank5OptionNum, BYTE btRank5Level);
	bool DelPentagramJewelInfo(int aIndex, CItem *lpItemData);
	bool DelPentagramJewelInfo(int aIndex, int iJewelPos, int iJewelIndex);

	void DBREQ_GetPentagramJewel(OBJECTSTRUCT *lpObj, char *szAccountId, int iJewelPos);
	void DBANS_GetPentagramJewel(LPBYTE lpRecv);
	void GCPentagramJewelInfo(int aIndex, int iJewelPos);
	void DBREQ_SetPentagramJewel(int aIndex, int iJewelPos);
	void DBREQ_DelPentagramJewel(int aIndex, int iJewelPos, int iJewelIndex);
	void DBREQ_InsertPentagramJewel(int aIndex, int iJewelPos, int iJewelIndex, int iItemType, int iItemIndex, int iMainAttribute, int iJewelLevel, BYTE btRank1, BYTE btRank1Level, BYTE btRank2, BYTE btRank2Level, BYTE btRank3, BYTE btRank3Level, BYTE btRank4, BYTE btRank4Level, BYTE btRank5, BYTE btRank5Level);

	int PentagramJewel_IN(int aIndex, int iPentagramItemPos, int iJewelItemPos);
	int PentagramJewel_OUT(int aIndex, int iPentagramItemPos, BYTE btSocketIndex, BYTE *btJewelPos, BYTE *btJewelDBIndex);

	bool GCTransPentagramJewelViewInfo(int aIndex, CItem *lpItemData);
	bool GCPShopPentagramJewelViewInfo(int aIndex, int aSourceIndex);

	int IsEnableToTradePentagramItem(OBJECTSTRUCT *lpObj);
	int IsEnableTransPentagramJewelInfo(int aIndex, int targetIndex);

	int CheckOverlapMythrilPiece(int iIndex, int iItemType, int iMainAttribute);

	int AddTradeCount(OBJECTSTRUCT *lpObj, int source, int target);

private:

	SERVER_ATTRIBUTE_DEFINE m_ServerAttributeDefine[14];
	PENTAGRAM_ITEM_OPEN_SOCKET_RATE m_PentagramItemOpenSocketRate[13];
	MONSTER_DROP_ITEM_RATE m_MonsterDropItemRate[30];
	JEWEL_OUT_RATE m_JewelOutRate[55];
	PENTAGRAM_SET_EFFECT m_AttackSetEffect[6];
	PENTAGRAM_SET_EFFECT m_RelationShipSetEffect[20];
	PENTAGRAM_HAVE_SET_OPTION m_PentagramHaveSetOption[20];
	PENTAGRAM_ITEM_OPTION m_PentagramItemOption[20];
	PENTAGRAM_ITEM_OPTION_ENABLE_NEED m_PentagramItemOptionEnableNeed[10];
	PENTAGRAM_SOCKET_RATE_BY_GRADE m_PentagramSocketRateByGrade[3];
};

extern CPentagramSystem g_PentagramSystem;

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

