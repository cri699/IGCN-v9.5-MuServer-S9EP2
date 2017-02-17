//////////////////////////////////////////////////////////////////////
// LuckyItemManager.h
#pragma once

#include "prodef.h"
#include "user.h"

#define	MAX_LUCKYINFO_LEVELRATE			16
#define MAX_LUCKYINFO_ITEMEQUIPMENT		7
#define	MAX_LUCKYINFO_SMELTRATE			10

struct _ITEM_LEVEL_RATE
{
	int iLevel;
	int	iRate;

	_ITEM_LEVEL_RATE::_ITEM_LEVEL_RATE()
	{
		this->iLevel		= 0;
		this->iRate			= 0;
	}
};

struct _LUCKY_ITEM_EQUIPMENT
{
	int	iItemNum;
	int	iSkill;
	int	iLuckOption;
	int	iAddOption;
	int iExOption;
	int	iClass;

	_LUCKY_ITEM_EQUIPMENT::_LUCKY_ITEM_EQUIPMENT()
	{
		this->iItemNum		= -1;
		this->iSkill		= 0;
		this->iLuckOption	= 0;
		this->iAddOption	= 0;
		this->iClass		= -1;
	}
};

struct _SMELT_RATE
{
	int	iMinDurab;
	int	iMaxDurab;
	int	iSmeltRate;

	_SMELT_RATE::_SMELT_RATE()
	{
		this->iMinDurab		= 0;
		this->iMaxDurab		= 0;
		this->iSmeltRate	= 0;
	}
};

struct _LUCKY_ITEM_INFO
{
	int						iItemNum;
	int						iItemKind;
	int						iAddOpt3Rate[7];
	int						iAddSetOptRate[2];
	_ITEM_LEVEL_RATE		ItemLevelRate[MAX_LUCKYINFO_LEVELRATE];
	_LUCKY_ITEM_EQUIPMENT	LuckyItemEquipment[MAX_LUCKYINFO_ITEMEQUIPMENT];
	_SMELT_RATE				SmeltRate[MAX_LUCKYINFO_SMELTRATE];

	_LUCKY_ITEM_INFO::_LUCKY_ITEM_INFO()
	{
		this->iItemNum	= -1;
		this->iItemKind	= -1;
		memset(this->iAddOpt3Rate, 0, sizeof(iAddOpt3Rate));
	}
};

struct PMSG_LUCKYITME_DB_INFO
{
	WORD	wItemCode;
	UINT64	Serial;
	WORD	wDurabilitySmall;
};

struct PMSG_REQ_LUCKYITEM_INSERT
{
	PBMSG_HEAD2 head;
	DWORD	dwUserGuid;
	char	szCharName[MAX_ACCOUNT_LEN + 1];
	PMSG_LUCKYITME_DB_INFO LuckyItemDBInfo;
};

struct PMSG_REQ_LUCKYITEM_INSERT_2ND
{
	PBMSG_HEAD2 head;
	DWORD	dwUserGuid;
	BYTE	btItemCnt;
	char	szCharName[MAX_ACCOUNT_LEN + 1];
	PMSG_LUCKYITME_DB_INFO LuckyItemDBInfo[5];
};

struct PMSG_REQ_LUCKYITEM_DELETE
{
	PBMSG_HEAD2 head;
	WORD	wUserIndex;
	char	szCharName[MAX_ACCOUNT_LEN+1];
	WORD	wItemCode;
	UINT64	Serial;
};

struct PMSG_ANS_LUCKYITEM_SELECT
{
	PWMSG_HEAD	head;
	WORD	wUserIndex;
	BYTE	btResultCode;
	BYTE	btItemCnt;
};

struct PMSG_REQ_LUCKYITEM_SELECT
{
	PBMSG_HEAD2 head;
	WORD	wUserIndex;
	char	chCharacterName[MAX_ACCOUNT_LEN+1];
};

struct LuckyItemManager
{
public:
	LuckyItemManager();
	~LuckyItemManager();

	int		LoadLuckyItemEquipment(int iType, _LUCKY_ITEM_INFO * pLuckyItemInfo, char * lpszFileName);
	int		LoadLuckyItemInfo(char * lpszFileName);
	void	AddLuckyItemInfo(_LUCKY_ITEM_INFO * pLuckyItemInfo, int iItemCnt);

	_LUCKY_ITEM_INFO * GetLuckyItemInfo(int iItemNum);
	_LUCKY_ITEM_INFO * GetCharClassLuckyItemInfo(int iItemNum, int iCharClass);
	_LUCKY_ITEM_EQUIPMENT * GetLuckyItemEquipment(_LUCKY_ITEM_INFO *const pLuckyItemInfo, int iCharClass);

	int		IsLuckyItemTicket(int iItemNum);
	int		IsLuckyItemEquipment(int iItemNum);

	_SMELT_RATE * GetSmeltingRateInfo(int iItemNum);

	void	LuckyItemTicketExchange(OBJECTSTRUCT * lpObj);
	BYTE	GambleGetLevel(_ITEM_LEVEL_RATE * pItemLevelRandRate, int iIndex);
	void	LuckyItemSmelting(OBJECTSTRUCT * lpObj);
	int		GambleLuckyItemSmelting(int iItemNum, int iDur);
	int		GetLuckyItemDurabilityTime();
	BYTE	GetLevel(int iMinLv, int iMaxLv);
	int		LuckyItemRepaire(OBJECTSTRUCT * lpObj, int source, int target);

	int		GDReqLuckyItemInsert(CItem * pItem, int iObjIndex);
	int		GDReqLuckyItemInsert(short wItemCode, UINT64 dwSerial, short wDurabilitySmall, int iObjIndex);
	void	GDReqLuckyItemInsert2nd(int iObjIndex);
	void	GDReqLuckyItemDelete(short wItemCode, UINT64 dwSerial, int iObjIndex);
	void	DGAnsLuckyItemList(PMSG_ANS_LUCKYITEM_SELECT * lpRecv);
	void	GDReqLuckyItemSelect(OBJECTSTRUCT * lpObj);

private:
	int									m_iTicketCnt;
	std::map<int, _LUCKY_ITEM_INFO>		m_Map_LuckyItemInfo;

}; extern LuckyItemManager g_LuckyItemManager;


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

