//////////////////////////////////////////////////////////////////////
// DSProtocol.h
// ------------------------------
// Decompiled by Deathway
// Date : 2007-05-09
// ------------------------------
#ifndef DSPROTOCOL_H
#define DSPROTOCOL_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "prodef.h"
#include "user.h"
#include "ArcaBattle.h"
#include "DevilSquareFinal.h"

/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Char List Count Packet
 *	Direction : DataServer -> GameServer
 *  Code : 0xC2
 *	HeadCode : 0x01
 *	BASE PACKET - Complete with SDHP_CHARLIST
 */
struct SDHP_CHARLISTCOUNT
{
	PWMSG_HEAD h;
	short Number;	// 4
	BYTE Count;	// 6
	int DbNumber;	// 8
	int SecurityCode;
	BYTE EnableCharacterCreate;
	char AccountId[11];	// D
	BYTE MoveCnt;	// 18
	BYTE WhExpansion;
};

struct SDHP_CHARLIST
{
	BYTE Index;	// 0
	char Name[11];	// 1
	char UnityBFRealName[11];
	WORD ServerCodeOfHomeWorld;
	WORD Level;	// C
	BYTE Class;	// E
	BYTE CtlCode;	// F
	BYTE dbInventory[48];	// 10
	BYTE btGuildStatus;	// 41
	WORD Resets;
	BYTE PK_Level;
};

struct SDHP_SUMMONER
{
	PBMSG_HEAD h;
	char AccountId[10];
	short Number;
	BYTE btSummoner;
};

struct SDHP_CREATECHARRESULT
{
	PBMSG_HEAD h;
	unsigned char Result;	// 3
	short Number;	// 4
	char AccountId[11];	// 6
	char Name[11];	// 10
	BYTE Pos;	// 1A
	BYTE ClassSkin;	// 1B
	BYTE Equipment[24];	// 1C
	WORD Level;	// 34
};


struct SDHP_CHARDELETERESULT
{
	PBMSG_HEAD h;
	unsigned char Result;	// 3
	short Number;	// 4
	char AccountID[11];	// 6
};


struct SDHP_DBCHAR_INFORESULT
{
	PWMSG_HEAD h;
	unsigned char result;	// 4
	short Number;	// 6
	char AccountID[11];	// 8
	char Name[11];	// 12
	BYTE Class;	// 1D
	short Level;	// 1E
	short mLevel;
	int LevelUpPoint;	// 20
	int mlPoint;
	UINT64 Exp;	// 24
	UINT64 NextExp;	// 28
	UINT64 mlExp;
	UINT64 mlNextExp;
	int Money;	// 2C
	WORD Str;	// 30
	WORD Dex;	// 32
	WORD Vit;	// 34
	WORD Energy;	// 36
	DWORD Life;	// 38
	DWORD MaxLife;	// 3A
	DWORD Mana;	// 3C
	DWORD MaxMana;	// 3E
	BYTE dbInventory[7584];	// 34
	BYTE dbMagicList[450];	// 700
	BYTE MapNumber;	// 7B4
	BYTE MapX;	// 7B5
	BYTE MapY;	// 7B6
	BYTE Dir;	// 7B7
	int PkCount;	// 7B8
	int PkLevel;	// 7BC
	int PkTime;	// 7C0
	BYTE CtlCode;	// 7C4
	DWORD GmCode;
	WORD GmExpDays;
	BYTE AccountCtlCode;	// 7C6
	BYTE dbQuest[100];	// 7C7
	WORD Leadership;	// 7FA
	WORD ChatLitmitTime;	// 7FC
	int iFruitPoint;	// 800
	int resets;
	char MarryName[11];
	char Married;
	char InventoryExpansion;
	char WarehouseExpansion;
	int WinDuels;
	int LoseDuels;
	int Ruud;
	time_t ChatBlockTime;
	DWORD PenaltyMask;
};

/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Get Character Info Packet
 *	Direction : GameServer -> Client
 *  Code     : 0xC3
 *	HeadCode : 0xF3
 *	SubCode  : 0x03
 */
struct PMSG_CHARMAPJOINRESULT
{
	PBMSG_HEAD h;	// C3:F3:03
	BYTE subcode;	// 3
	BYTE MapX;	// 4
	BYTE MapY;	// 5
	BYTE MapNumber;	// 6
	BYTE Dir;	// 7
	BYTE ExpHHH; //
	BYTE ExpHHL; //
	BYTE ExpHLH; //
	BYTE ExpHLL; //
	BYTE ExpLHH; //
	BYTE ExpLHL; //
	BYTE ExpLLH; //
	BYTE ExpLLL; //
	BYTE NextExpHHH; //
	BYTE NextExpHHL; //
	BYTE NextExpHLH; //
	BYTE NextExpHLL; //
	BYTE NextExpLHH; //
	BYTE NextExpLHL; //
	BYTE NextExpLLH; //
	BYTE NextExpLLL; //
	WORD LevelUpPoint;	// 10
	WORD Str;	// 12
	WORD Dex;	// 14
	WORD Vit;	// 16
	WORD Energy;	// 18
	WORD Life;	// 1A
	WORD MaxLife;	// 1C
	WORD Mana;	// 1E
	WORD MaxMana;	// 20
	WORD wShield;	// 22
	WORD wMaxShield;	// 24
	WORD BP;	// 26
	WORD MaxBP;	// 28
	int Money;	// 2C
	BYTE PkLevel;	// 30
	BYTE CtlCode;	// 31
	short AddPoint;	// 32
	short MaxAddPoint;	// 34
	WORD Leadership;	// 36
	WORD wMinusPoint;	// 38
	WORD wMaxMinusPoint;	// 3A
	WORD InvExpansion;
	int Ruud; // Season X Addon
	int IGCLife;
	int IGCMaxLife;
	int IGCMana;
	int IGCMaxMana;
};

/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Warehouse DB Save Packet
 *	Direction : GameServer -> DataServer [0x09]
 *	            DataServer -> GameServer [0x08]
 *  Code      : 0xC2
 *	HeadCode  : 0x08, 0x09
 */
struct SDHP_GETWAREHOUSEDB_SAVE
{
	PWMSG_HEAD h;
	char AccountID[11];	// 4
	short aIndex;	// E
	int Money;	// 10
	BYTE dbItems[7680];	// 14
	short pw;	// 796
	BYTE WarehouseID;
	BYTE CloseWindow;
};


struct SDHP_GETWAREHOUSEDB_RESULT
{
	PBMSG_HEAD h;
	char AccountID[11];	// 3
	short aIndex;	// E
	BYTE WarehouseID;
};


struct SDHP_ITEMCREATERECV
{
	PBMSG_HEAD h;
	BYTE x;	// 3
	BYTE y;	// 4
	BYTE MapNumber;	// 5 [235:Mu_2nd_Aniv], [236:CashShop]
	UINT64 m_Number;	// 8
	short Type;	// 6
	BYTE Level;	// 8
	BYTE Dur;	// 9
	BYTE Op1;	// A
	BYTE Op2;	// B
	BYTE Op3;	// C
	BYTE NewOption;	// D
	int aIndex;	// 10
	int lootindex;	// 14
	BYTE SetOption;	// 16
	time_t lDuration;
	BYTE SocketOption[5];
	BYTE MainAttribute;
};

struct SDHP_SKILLKEYDATA_SEND
{
	PBMSG_HEAD h;
	int aIndex;	// 4
	char Name[11];	// 8
	BYTE SkillKeyBuffer[20];	// 12
	BYTE GameOption;	// 1C
	BYTE QkeyDefine;	// 1D
	BYTE WkeyDefine;	// 1E
	BYTE EkeyDefine;	// 1F
	BYTE ChatWindow;	// 20
	BYTE RkeyDefine;
	int QWERLevelDefine;
	BYTE EnableTransformMode;
};

struct SDHP_CHARACTER_TRANSFER_RESULT
{
	PBMSG_HEAD h;
	char Account[10];	// 3
	short Number;	// E
	unsigned char Result;	// 10
};

struct SDHP_QMKMLS_LOAD
{
	PBMSG_HEAD h;
	int aIndex;
	char Name[11];
};

struct SDHP_MLS_DATA
{
	PBMSG_HEAD h;
	char Name[11];
	short ML_Level;
	long long ML_Experience;
	long long ML_NextExperience;
	short ML_Point;
};

struct SDHP_MLS_DATA_REQUEST
{
	PBMSG_HEAD h;
	BYTE SubCode;
	int Index;
	char Name[11];
	BYTE Unknow1;
	short ML_Level;
	WORD Unknow2;
	long long ML_Experience;
	long long ML_NextExperience;
	short ML_Point;
	short Unknow3;
	int Unknow4;
};

struct GSP_REQ_GENS_JOIN
{
	PBMSG_HEAD2		h;
	char			Name[11];
	short			aIndex;
	char			GensID;
};

struct GSP_ANS_GENS_JOIN
{
	PBMSG_HEAD2		h;
	char			Result;
	short			aIndex;
	char			Name[11];
	char			GensID;
	char			GensPos;
	int				GensExp;
	int				GensNextExp;
};

struct GSP_REQ_GENS_INFO
{
	PBMSG_HEAD2		h;
	short			aIndex;
	char			Name[11];
};

struct GSP_ANS_GENS_INFO
{
	PBMSG_HEAD2		h;
	short			aIndex;
	char			Name[11];
	char			GensID;
	char			GensPos;
	int				GensExp;
	int				GensNextExp;
	short			GensRank;
	bool			GensReward;
};

struct GSP_REQ_GENS_QUIT
{
	PBMSG_HEAD2		h;
	short			aIndex;
	char			Name[11];
};

struct GSP_ANS_GENS_QUIT
{
	PBMSG_HEAD2		h;
	short			aIndex;
	char			Name[11];
	char			Result;
};

struct GSP_REQ_GENS_RANK
{
	PBMSG_HEAD2		h;
	short			aIndex;
	char			Name[11];
	short			GensID;
};

struct GSP_ANS_GENS_RANK
{
	PBMSG_HEAD2		h;
	short			aIndex;
	char			Name[11];
	short			GensID;
	char			Result;
	short			Rank;
	char			LeadingGens;
};

struct ISHOP_REQ_ITEMLIST
{
	PBMSG_HEAD2		h;
	short			aIndex;
	BYTE			InvType;
	BYTE			InvNum;
	char			AccountID[11];
};

struct ISHOP_ANS_ITEMLIST
{
	PWMSG_HEAD2		h;
	short			aIndex;
	BYTE			InvType;
	BYTE			InvNum;
	char			AccountID[11];
	int				Result;
	int				Count;
};

struct ISHOP_ITEMLIST
{
	int				UniqueCode;
	int				AuthCode;
	int				UniqueID1;
	int				UniqueID2;
	int				UniqueID3;
	BYTE			InventoryType;
	char			GiftName[10];
	char			Message[200];
};

struct ISHOP_ITEM_BUY
{
	PBMSG_HEAD		h;
	short			aIndex;
	char			AccountID[11];
	int				ID1;
	int				ID2;
	int				ID3;
	int				Price;
	BYTE			CoinType;
};

struct ISHOP_ITEM_BUYANS
{
	PBMSG_HEAD		h;
	short			aIndex;
	char			AccountID[11];
	BYTE			Result;
	int				ID1;
	int				ID2;
	int				ID3;
};

struct ISHOP_ITEM_USE
{
	PBMSG_HEAD		h;
	short			aIndex;
	char			AccountID[11];
	int				UniqueCode;
	int				AuthCode;
};

struct ISHOP_ITEM_USEANS
{
	PBMSG_HEAD		h;
	short			aIndex;
	char			AccountID[11];
	BYTE			Result;
	int				UniqueCode;
	int				AuthCode;
	int				ID1;
	int				ID2;
	int				ID3;
};

struct ISHOP_REQ_POINT
{
	PBMSG_HEAD		h;
	short			aIndex;
	char			AccountID[11];
};

struct ISHOP_ANS_POINT
{
	PBMSG_HEAD		h;
	short			aIndex;
	char			AccountID[11];
	BYTE			Result;
	float			CoinP;
	float			CoinC;
	float			Goblin;
};

struct ISHOP_POINT_ADD
{
	PBMSG_HEAD		h;
	short			aIndex;
	char			AccountID[11];
	BYTE			Type;
	float			Coin;
};

struct ISHOP_ITEM_GIFT
{
	PWMSG_HEAD		h;
	short			aIndex;
	char			AccountID[11];
	int				ID1;
	int				ID2;
	int				ID3;
	int				Price;
	BYTE			CoinType;
	char			Name[11];
	char			TargetName[11];
	char			Message[200];
};

struct ISHOP_ITEM_GIFTANS
{
	PBMSG_HEAD		h;
	short			aIndex;
	char			AccountID[11];
	BYTE			Result;
	int				ID1;
	int				ID2;
	int				ID3;
};

struct ISHOP_ITEM_DELETE
{
	PBMSG_HEAD		h;
	char			AccountID[11];
	int				UniqueCode;
	int				AuthCode;
};

struct ISHOP_ITEM_ROLLBACK
{
	PBMSG_HEAD		h;
	char			AccountID[11];
	int				UniqueCode;
	int				AuthCode;
};

struct MUBOT_DATA
{
	PWMSG_HEAD		h;
	short			aIndex;
	char			Name[11];
	BYTE			MuBotData[257];
};

struct BAN_REQ_USER
{
	PBMSG_HEAD		h;
	BYTE			Type;
	BYTE			Ban;
	char			AccName[11];
};

#pragma pack (1)
struct PMSG_RESET_INGAME_SEND
{
	PBMSG_HEAD2 h;
	WORD Resets;
};
#pragma pack ()

struct SDHP_VIPINFOSEND
{
	PBMSG_HEAD h;
	int iIndex;
	char AccountId[11];
	char Name[11];
};

struct SDHP_VIPINFORESULT
{
	PBMSG_HEAD h;
	int iIndex;
	char AccountId[11];
	char Name[11];
	BYTE VipType;
};

struct ISHOP_VIP_BUY
{
	PBMSG_HEAD		h;
	char			AccountID[11];
	BYTE			Type;
	int				Days;
};

struct ISHOP_CHARCARD_BUY
{
	PBMSG_HEAD		h;
	char			AccountID[11];
	BYTE			Type;
};

struct ISHOP_ITEM_BUY_PACKAGE
{
	PWMSG_HEAD2		h;
	short			aIndex;
	char			AccountID[11];
	int				Price;
	BYTE			CoinType;
	WORD			Count;
};

struct ISHOP_ITEM_GIFT_PACKAGE
{
	PWMSG_HEAD2		h;
	short			aIndex;
	char			AccountID[11];
	int				Price;
	BYTE			CoinType;
	char			Name[11];
	char			TargetName[11];
	char			Message[200];
	WORD			Count;
};

struct ISHOP_ITEM_PACKAGE
{
	int				ID1;
	int				ID2;
	int				ID3;
};

struct PMSG_ANS_WARESAVE
{
	PBMSG_HEAD		h;
	int				iIndex;
	BYTE			Result;
	BYTE			CloseWindow;
};

struct SECLOCK_REQ_SAVE
{
	PBMSG_HEAD		h;
	char			AccountID[11];
	int				Code;
};

struct ATTRIBUTE_ADD_ERTEL
{
	PBMSG_HEAD2		h;
	char			AccountID[11];
	int				aIndex;
	BYTE			Elemental;
	BYTE			SocketID;
	BYTE			ItemIndex;
	WORD			ItemNumber;
	BYTE			PentagramPos;
	BYTE			ErtelPos;
	BYTE			OptionID[5];
	BYTE			OptionLevel[5];
};

struct ATTRIBUTE_ADD_ERTELANS
{
	PBMSG_HEAD2		h;
	BYTE			Result;
	int				aIndex;
	BYTE			Elemental;
	BYTE			SocketID;
	BYTE			ItemIndex;
	WORD			ItemNumber;
	BYTE			PentagramPos;
	BYTE			ErtelPos;
	BYTE			OptionID[5];
	BYTE			OptionLevel[5];
};

typedef struct 
{
	PBMSG_HEAD h;
	unsigned short ServerCode;
	char CharName[MAX_ACCOUNT_LEN+1];
}SDHP_DELETE_TEMPUSERINFO,* LPSDHP_DELETE_TEMPUSERINFO;

struct SDHP_DELETE_TEMPUSERINFO_RESULT 
{
	PBMSG_HEAD h;
	char CharName[MAX_ACCOUNT_LEN+1];
};
struct DS_GET_MONSTERCNT
{
	PBMSG_HEAD2		h;
	char character[11];
	int cnt;
	short aIndex;
};

extern void(*ItemSerialCreateSend)(int aIndex, BYTE MapNumber, BYTE x, BYTE y, int type, BYTE level, BYTE dur, BYTE Op1, BYTE Op2, BYTE Op3, int LootIndex, BYTE NewOption, BYTE SetOption, time_t lDuration, BYTE *SocketOption, BYTE MainAttribute);

struct DS_SAVE_MONSTERCNT
{
	PBMSG_HEAD2		h;
	char character[11];
	int type;
};

struct DS_SAVE_PLAYERKILLER
{
	PBMSG_HEAD2		h;
	char Victim[11];
	char Killer[11];
};

struct _tagPMSG_ANS_ARCA_BATTLE_GUILD_JOIN_DS
{
	PBMSG_HEAD2 h;
	BYTE btResult;
	WORD wNumber;
};

struct _tagPMSG_ANS_ARCA_BATTLE_JOIN
{
	PBMSG_HEAD2 h;
	char btResult;
};

struct _tagPMSG_REQ_ARCA_BATTLE_GUILD_JOIN_DS
{
	PBMSG_HEAD2 h;
	char szGuildMaster[11];
	char szGuildName[9];
	DWORD dwGuild;
	WORD wNumber;
};

struct _tagPMSG_ANS_ARCA_BATTLE_GUILD_MEMBER_JOIN_DS
{
	PBMSG_HEAD2 h;
	char btResult;
	WORD wNumber;
};

struct _tagPMSG_ANS_ARCA_BATTLE_ENTER_DS
{
	PBMSG_HEAD2 h;
	char btResult;
	BYTE btEnterSeq;
	WORD wNumber;
};

struct _tagPMSG_ANS_ARCA_BATTLE_ENTER
{
	PBMSG_HEAD2 h;
	char btResult;
};

struct _tagPMSG_ANS_AB_PROC_STATE_DS
{
	PBMSG_HEAD2 h;
	BYTE btProcState;
};

struct _stGuildUnderMember
{
	BYTE btGuildMemberCnt;
	char szGuildNames[9];
};

/* 5188 */
struct _tagPMSG_ANS_AB_JOIN_MEMBER_UNDER_DS
{
	PBMSG_HEAD2 h;
	BYTE btGuildCnt;
	_stGuildUnderMember GuildMemberCnt[6];
};

struct _tagPMSG_ANS_AB_JOIN_MEMBER_UNDER
{
	PBMSG_HEAD2 h;
	char btMinGuildMember;
	char btGuildMemberCnt;
};

struct _tagPMSG_REQ_AB_GUILD_JOIN_SELECT_DS
{
	PBMSG_HEAD2 h;
	char szGuildMaster[11];
	char szGuildName[9];
	WORD wNumber;
};

struct _tagPMSG_REQ_AB_GUILD_JOIN_SELECT2ND_DS
{
	PBMSG_HEAD2 h;
	WORD wNumber;
	DWORD dwGuild;
};

struct _tagPMSG_REQ_ARCA_BATTLE_GUILD_MEMBER_JOIN_DS
{
	PBMSG_HEAD2 h;
	char szCharName[11];
	char szGuildName[9];
	unsigned int dwGuild;
	WORD wNumber;
};

struct _tagPMSG_REQ_ARCA_BATTLE_ENTER_DS
{
	PBMSG_HEAD2 h;
	char szCharName[11];
	BYTE btEnterSeq;
	WORD wNumber;
};

struct SDHP_REQ_DBMUUN_INVEN_LOAD
{
	PBMSG_HEAD h;
	char AccountID[11];
	char Name[11];
	WORD aIndex;
};

struct SDHP_ANS_CCF_INFO_NOTIFY
{
	PBMSG_HEAD2 h;
	int nMin;
	int nType;
};

struct SDHP_ANS_CCF_GETPERMISSION
{
	PBMSG_HEAD2 h;
	int nResult;
	int nIndex;
};

struct SDHP_REQ_SET_CCF_WINNER_INFO
{
	PBMSG_HEAD2 h;
	char UBFName[MAX_ACCOUNT_LEN + 1];
	BYTE btCCFType;
	BYTE btRewardType;
};

struct _tagPMSG_REQ_SAVE_CHAOSCASTLE_KILLPOINT_UBF
{
	_tagPMSG_REQ_SAVE_CHAOSCASTLE_KILLPOINT_UBF()
	{
		memset(this->szCharName, 0x00, sizeof(this->szCharName));
		this->nPoint = 0;
	}

	PBMSG_HEAD2 h;
	char szCharName[MAX_ACCOUNT_LEN+1];
	int nPoint;
	int nIndex;
	int nCastleIndex;
};

struct SDHP_ANS_KILLPOINT_RESULT_CC_UBF
{
	PBMSG_HEAD2 h;
	int nIndex;
	int nResult;
	int nCurrentPoint;
	int nTotalPoint;
};

struct _tagSDHP_ANS_DBMUUN_INVEN_LOAD
{
	PWMSG_HEAD h;
	BYTE dbItems[1984];
	char SubEquip;
	WORD aIndex;
};

struct _tagSDHP_REQ_DBMUUN_INVEN_SAVE
{
	PWMSG_HEAD h;
	char AccountID[11];
	char Name[11];
	BYTE dbInventory[1984];
};

struct _tagSDHP_REQ_DBEVENT_INVEN_SAVE
{
	PWMSG_HEAD h;
	char AccountID[11];
	char Name[11];
	BYTE dbInventory[1024];
};

struct _tagSDHP_REQ_DBEVENT_INVEN_LOAD
{
	PBMSG_HEAD h;
	char AccountID[11];
	char Name[11];
	WORD aIndex;
};

struct _tagSDHP_ANS_DBEVENT_INVEN_LOAD
{
	PWMSG_HEAD h;
	BYTE dbItems[1024];
	WORD aIndex;
};

struct PMSG_REQ_SWITCHWARE
{
	PBMSG_HEAD h;
	int iIndex;
	char szAccountID[MAX_ACCOUNT_LEN+1];
	int WarehouseID;
};

struct PMSG_ANS_SWITCHWARE
{
	PBMSG_HEAD h;
	int iIndex;
	char szAccountID[MAX_ACCOUNT_LEN+1];
	int WarehouseID;
	int Result;
};

// UBF (Unity Battle Field aka Battle Core)

struct PMSG_UBF_REGISTER_ACCOUNT_USER
{
	PBMSG_HEAD2 h;
	char szAccountID[MAX_ACCOUNT_LEN+1];
	char szName[MAX_ACCOUNT_LEN+1];
	char szBattleFieldName[MAX_ACCOUNT_LEN+1];
	int iUserIndex;
	WORD ServerCode;
	BYTE btRegisterState;
	BYTE btRegisterMonth;
	BYTE btRegisterDay;
};

struct PMSG_UBF_REGISTER_ACCOUNT_USER_RESULT
{
	PBMSG_HEAD2 h;
	int iUserIndex;
	BYTE btResult;
	WORD nLeftSec;
};

struct PMSG_UBF_ACCOUNT_USER_COPY
{
	PBMSG_HEAD2 h;
	int iUserIndex;
	char szAccountID[MAX_ACCOUNT_LEN+1];
	char szName[MAX_ACCOUNT_LEN+1];
	WORD ServerCode;
	BYTE btPromotionCode;
};

struct PMSG_UBF_ACCOUNT_USER_COPY_RESULT
{
	PBMSG_HEAD2 h;
	int iUserIndex;
	BYTE btResult;
	BYTE btSubResult;
};

struct PMSG_UBF_ACCOUNT_USER_COPY_PETITEM
{
	PWMSG_HEAD h;
	char szAccountID[MAX_ACCOUNT_LEN];
	int iUserIndex;
	BYTE btCount;
	int IsUnityBattleFieldServer;
	int ServerCode;
};

struct PMSG_UBF_COPY_PETITEM
{
	BYTE btItemPos;
	UINT64 i64ItemSerial;
};

struct PMSG_REQ_UBF_ACCOUNT_USERINFO
{
	PBMSG_HEAD2 h;
	int iUserIndex;
	char szAccountID[MAX_ACCOUNT_LEN+1];
	char szName[MAX_ACCOUNT_LEN+1];
	int iServerCode;
	int IsUnityBattleFieldServer;
	BYTE btObserverMode;
};

struct PMSG_ANS_UBF_ACCOUNT_USERINFO
{
	PBMSG_HEAD2 h;
	int iUserIndex;
	BYTE btResult;
	BYTE btRegisterState;
	BYTE btRegisterMonth;
	BYTE btRegisterDay;
	BYTE btObserverMode;
};

struct PMSG_UBF_REQ_CANCEL_REGISTER_USER
{
	PBMSG_HEAD2 h;
	char szAccountID[MAX_ACCOUNT_LEN+1];
	char szName[MAX_ACCOUNT_LEN+1];
	int iUserIndex;
	WORD ServerCode;
	BYTE btCanceled;
};

struct PMSG_UBF_ANS_CANCEL_REGISTER_USER
{
	PBMSG_HEAD2 h;
	int iUserIndex;
	BYTE btCanceledResult;
	BYTE btDeletedResult;
};

struct PMSG_REQ_GET_UBF_REAL_NAME
{
	PBMSG_HEAD2 h;
	int iUserIndex;
	char szUBFName[MAX_ACCOUNT_LEN+1];
};

struct PMSG_ANS_GET_UBF_REAL_NAME
{
	PBMSG_HEAD2 h;
	int iUserIndex;
	char szUBFName[MAX_ACCOUNT_LEN+1];
	char szRealName[MAX_ACCOUNT_LEN+1];
	int iServerCode;
	BYTE btReturn;
};

struct PMSG_REQ_UBF_GET_REWARD
{
	PBMSG_HEAD2 h;
	int iUserIndex;
	int iServerCode;
	BYTE btBattleKind;
	char szAccountID[MAX_ACCOUNT_LEN+1];
	char szName[MAX_ACCOUNT_LEN+1];
};

struct PMSG_ANS_UBF_GET_REWARD
{
	PBMSG_HEAD2 h;
	int iUserIndex;
	BYTE btBattleKind;
	BYTE btReturn;
	BYTE btStage1;
	BYTE btStage2;
	BYTE btStage3;
	BYTE btRewardCount1[2];
	BYTE btRewardCount2[2];
	BYTE btRewardCount3[2];
};

struct PMSG_REQ_UBF_SET_RECEIVED_REWARD
{
	PBMSG_HEAD2 h;
	int iUserIndex;
	int iServerCode;
	char szAccountID[MAX_ACCOUNT_LEN+1];
	char szName[MAX_ACCOUNT_LEN+1];
	BYTE btReceivedReward;
	BYTE btBattleKind;
};

struct PMSG_ANS_UBF_SET_RECEIVED_REWARD
{
	PBMSG_HEAD2 h;
	int iUserIndex;
	BYTE btReturn;
};

struct PMSG_RECV_CHATDATA_WHISPER
{
	PBMSG_HEAD h;	// C1:02
	WORD wMapSvrNum;
	int OriginGSIndex;
	int OriginPlayerIndex;
	char id[10];	
	char fromId[10];
	char chatmsg[90];
};

struct DSMSG_REQ_SUBSERVER_COUNT
{
	PBMSG_HEAD2 h;	// C1:C3:00
	WORD wMapSvrGroup;
};

struct DSMSG_ANS_SUBSERVER_COUNT
{
	PBMSG_HEAD2 h;	// C1:C3:01
	WORD wSvrCount;
};

struct DSMSG_ANS_WHISPER_RESULT
{
	PBMSG_HEAD2 h;	// C1:C3:02
	int OriginGSIndex; // sent back as is
	int OriginPlayerIndex; // sent back as is
	BYTE btResult; // Result 1 - sucess , 0 - fail
	
};

struct DSMSG_GS_WHISPER_RESULT
{
	PBMSG_HEAD2 h;	// C1:C3:03
	int OriginPlayerIndex; // sent back as is
	BYTE btResult; // Result 1 - sucess , 0 - fail
};

struct DSMSG_CHAOSMACHINE_LOSTITEMS
{
	PWMSG_HEAD h;
	char szAccountID[MAX_ACCOUNT_LEN + 1];
	char szName[MAX_ACCOUNT_LEN + 1];
	BYTE btLostItems[CHAOS_BOX_SIZE * MAX_DBITEM_INFO];
};

struct PMSG_RECV_DC_OTHER_CHANNEL
{
	PBMSG_HEAD2 h; // C1:C3:04
	char szName[MAX_ACCOUNT_LEN + 1];
	WORD wMapSrvGroup;
};// Season X

struct PMSG_DSREQ_BLOCKCHAT_LIST
{
	PBMSG_HEAD2 h;
	int iIndex;
	char szName[MAX_ACCOUNT_LEN + 1];
};

struct PMSG_DSANS_BLOCKCHAT_LIST
{
	PWMSG_HEAD2 h;
	int iIndex;
	char szName[MAX_ACCOUNT_LEN + 1];

};

void DataServerProtocolCore(BYTE protoNum, LPBYTE aRecv, int aLen);
void DataServerLogin();
void DataServerLoginResult( SDHP_RESULT* lpMsg);
void JGPGetCharList(LPBYTE lpRecv);
void DataServerGetCharListRequest( short aIndex);
void JGCharacterCreateRequest( SDHP_CREATECHARRESULT* lpMsg);
void JGCharacterCreateFailSend(int aIndex, char* id);
void JGCharDelRequest( SDHP_CHARDELETERESULT* lpMsg);
void JGGetCharacterInfo( SDHP_DBCHAR_INFORESULT* lpMsg);
void GCItemListSend(int aIndex);
void GJSetCharacterInfo(struct OBJECTSTRUCT* lpObj, int aIndex, BOOL bMapServerMove);
void GDGetWarehouseList(int aIndex, char* AccountID);
void DGGetWarehouseList( SDHP_GETWAREHOUSEDB_SAVE* lpMsg);
void GDGetWarehouseNoItem( SDHP_GETWAREHOUSEDB_RESULT* lpMsg);
void GDSetWarehouseList(int aIndex, BOOL bCloseWindow);
void DGSetWarehouseList(PMSG_ANS_WARESAVE* lpMsg);
void GDUserItemSave( OBJECTSTRUCT* lpObj);
void GDSetWarehouseMoney(int aIndex);
void GDGameServerInfoSave();
void ItemCreate(int aIndex, BYTE MapNumber, BYTE x, BYTE y, int type, BYTE level, BYTE dur, BYTE Op1, BYTE Op2, BYTE Op3, int LootIndex, BYTE NewOption, BYTE SetOption, time_t lDuration, BYTE *SocketOption, BYTE MainAttribute);
void PetItemSerialCreateSend(int aIndex, BYTE MapNumber, BYTE x, BYTE y, int type, BYTE level,BYTE dur, BYTE Op1, BYTE Op2, BYTE Op3, int LootIndex, BYTE NewOption, BYTE SetOption);
void ExpItemSerialCreateSend(int aIndex, BYTE MapNumber, BYTE x, BYTE y, int type, BYTE level, BYTE dur, BYTE Op1, BYTE Op2, BYTE Op3, int LootIndex, BYTE NewOption, BYTE SetOption, time_t ExpTime);
void ItemSerialCreateRecv( SDHP_ITEMCREATERECV* lpMsg);
void ItemMovePathSave(char* ActID, char* Name, BYTE level, BYTE mapnumber, BYTE x, BYTE y, char* Item, BYTE op1, BYTE op2, BYTE op3, DWORD serial);
void DGOptionDataSend(int aIndex, char* szName, LPBYTE KeyBuffer, BYTE GO,BYTE Qk, BYTE Wk, BYTE Ek, BYTE ChatWnd, BYTE Rk, int QWER, BYTE Transform);
void DGOptionDataRecv( SDHP_SKILLKEYDATA_SEND* lpMsg);
void DGDeleteTempUserInfo(SDHP_DELETE_TEMPUSERINFO_RESULT * aRecv);
void DGMoveOtherServer( SDHP_CHARACTER_TRANSFER_RESULT* lpMsg);
void gObjRequestPetItemInfo(int aIndex, int inventype);
void DGRecvPetItemInfo(LPBYTE lpData);
void gObjSavePetItemInfo(int aIndex, int inventype);
void GS_GDReqCastleTotalInfo(int iMapSvrGroup, int iCastleEventCycle);
void GS_GDReqOwnerGuildMaster(int iMapSvrGroup, int aIndex);
void GS_GDReqCastleNpcBuy(int iMapSvrGroup, int aIndex, int iNpcNumber, int iNpcIndex, int iNpcDfLevel, int iNpcRgLevel, int iNpcMaxHP, int iNpcHP, BYTE btNpcX, BYTE btNpcY, BYTE btNpcDIR,  int iBuyCost);
void GS_GDReqCastleNpcRepair(int iMapSvrGroup, int aIndex, int iNpcNumber, int iNpcIndex, int iRepairCost);
void GS_GDReqCastleNpcUpgrade(int iMapSvrGroup, int aIndex, int iNpcNumber, int iNpcIndex, int iNpcUpType, int iNpcUpValue, int iNpcUpIndex);
void GS_GDReqTaxInfo(int iMapSvrGroup, int aIndex);
void GS_GDReqTaxRateChange(int iMapSvrGroup, int aIndex, int iTaxType, int iTaxRate);
void GS_GDReqCastleMoneyChange(int iMapSvrGroup, int aIndex, int iMoneyChange);
//void GS_GDReqCastleDateChange(int iMapSvrGroup, int aIndex,WORD wStartYear, BYTE btStartMonth, BYTE btStartDay, WORD wEndYear, BYTE btEndMonth, BYTE btEndDay);
void GS_GDReqSiegeDateChange(int iMapSvrGroup, int aIndex, WORD wStartYear, BYTE btStartMonth, BYTE btStartDay, WORD wEndYear, BYTE btEndMonth, BYTE btEndDay);
void GS_GDReqGuildMarkRegInfo(int iMapSvrGroup, int aIndex);
void GS_GDReqSiegeEndedChange(int iMapSvrGroup, int bIsSiegeEnded);
void GS_GDReqCastleOwnerChange(int iMapSvrGroup, int bIsCastleOccupied, char* lpszGuildName);
void GS_GDReqRegAttackGuild(int iMapSvrGroup, int aIndex);
void GS_GDReqRestartCastleState(int iMapSvrGroup);
void GS_GDReqMapSvrMsgMultiCast(int iMapSvrGroup, char* lpszMsgText);
void GS_GDReqGlobalPostMultiCast(int iMapSvrGroup, int aIndex, char * szMessage);
void GS_GDReqRegGuildMark(int iMapSvrGroup, int aIndex, int iItemPos);
void GS_GDReqGuildMarkReset(int iMapSvrGroup, int aIndex, char* lpszGuildName);
void GS_GDReqGuildSetGiveUp(int iMapSvrGroup, int aIndex, int bIsGiveUp);
void GS_GDReqNpcRemove(int iMapSvrGroup, int iNpcNumber, int iNpcIndex);
void GS_GDReqCastleStateSync(int iMapSvrGroup, int iCastleState, int iTaxRateChaos, int iTaxRateStore, int iTaxHuntZone, char* lpszOwnerGuild);
void GS_GDReqCastleTributeMoney(int iMapSvrGroup, int iCastleTributeMoney);
void GS_GDReqResetCastleTaxInfo(int iMapSvrGroup);
void GS_GDReqResetSiegeGuildInfo(int iMapSvrGroup);
void GS_GDReqResetRegSiegeInfo(int iMapSvrGroup);
void GS_GDReqCastleInitData(int iMapSvrGroup, int iCastleEventCycle);
void GS_GDReqCastleNpcInfo(int iMapSvrGroup, int aIndex);
void GS_GDReqAllGuildMarkRegInfo(int iMapSvrGroup, int aIndex);
void GS_GDReqCalcRegGuildList(int iMapSvrGroup);
void GS_GDReqCsLoadTotalGuildInfo(int iMapSvrGroup);
void GS_DGAnsCastleTotalInfo(LPBYTE lpRecv);
void GS_DGAnsOwnerGuildMaster(LPBYTE lpRecv);
void GS_DGAnsCastleNpcBuy(LPBYTE lpRecv);
void GS_DGAnsCastleNpcRepair(LPBYTE lpRecv);
void GS_DGAnsCastleNpcUpgrade(LPBYTE lpRecv);
void GS_DGAnsTaxInfo(LPBYTE lpRecv);
void GS_DGAnsTaxRateChange(LPBYTE lpRecv);
void GS_DGAnsCastleMoneyChange(LPBYTE lpRecv);
void GS_DGAnsSiegeDateChange(LPBYTE lpRecv);
void GS_DGAnsGuildMarkRegInfo(LPBYTE lpRecv);
void GS_DGAnsSiegeEndedChange(LPBYTE lpRecv);
void GS_DGAnsCastleOwnerChange(LPBYTE lpRecv);
void GS_DGAnsRegAttackGuild(LPBYTE lpRecv);
void GS_DGAnsRestartCastleState(LPBYTE lpRecv);
void GS_DGAnsMapSvrMsgMultiCast(LPBYTE lpRecv);
void GS_DGAnsGlobalPostMultiCast(LPBYTE lpRecv);
void GS_DGAnsRegGuildMark(LPBYTE lpRecv);
void GS_DGAnsGuildMarkReset(LPBYTE lpRecv);
void GS_DGAnsGuildSetGiveUp(LPBYTE lpRecv);
void GS_DGAnsNpcRemove(LPBYTE lpRecv);
void GS_DGAnsCastleStateSync(LPBYTE lpRecv);
void GS_DGAnsCastleTributeMoney(LPBYTE lpRecv);
void GS_DGAnsResetCastleTaxInfo(LPBYTE lpRecv);
void GS_DGAnsResetSiegeGuildInfo(LPBYTE lpRecv);
void GS_DGAnsResetRegSiegeInfo(LPBYTE lpRecv);
void GS_DGAnsCastleInitData(LPBYTE lpRecv);
void GS_DGAnsCastleNpcInfo(LPBYTE lpRecv);
void GS_DGAnsAllGuildMarkRegInfo(LPBYTE lpRecv);
void GS_DGAnsFirstCreateNPC(LPBYTE lpRecv);
void GS_DGAnsCalcREgGuildList(LPBYTE lpRecv);
void GS_DGAnsCsGulidUnionInfo(LPBYTE lpRecv);
void GS_DGAnsCsSaveTotalGuildInfo(LPBYTE lpRecv);
void GS_DGAnsCsLoadTotalGuildInfo(LPBYTE lpRecv);
void GS_DGAnsCastleNpcUpdate(LPBYTE lpRecv);
void GDReqCrywolfSync(int iMapSvrGroup, int iCrywolfState, int iOccupationState);
void DGAnsCrywolfSync(LPBYTE lpRecv);
void GDReqCrywolfInfoLoad(int iMapSvrGroup);
void DGAnsCrywolfInfoLoad(LPBYTE lpRecv);
void GDReqCrywolfInfoSave(int iMapSvrGroup, int iCrywolfState, int iOccupationState);
void DGAnsCrywolfInfoSave(LPBYTE lpRecv);
void GDReqGensJoin(int aIndex, int GensID);
void DGAnsGensJoin(GSP_ANS_GENS_JOIN *lpMsg);
void GDReqGensInfo(int aIndex);
void DGAnsGensInfo(GSP_ANS_GENS_INFO *lpMsg);
void GDReqGensQuit(int aIndex);
void DGAnsGensQuit(GSP_ANS_GENS_QUIT *lpMsg);
void GDReqGensRank(int aIndex);
void DGAnsGensRank(GSP_ANS_GENS_RANK *lpMsg);
void GDReqGensSave(int aIndex);

void GDReqInGameShopItemList(int aIndex, BYTE InventoryType, BYTE InventoryNum);
void DGAnsInGameShopItemList(LPBYTE lpRecv);
void GDReqInGameShopPoint(int aIndex);
void DGAnsInGameShopPoint(ISHOP_ANS_POINT *lpMsg);
void GDReqInGameShopPointAdd(int aIndex, BYTE Type, float Coin);
void GDReqInGameShopItemBuy(int aIndex, int ID1, int ID2, int ID3, int Price, BYTE CoinType);
void DGAnsInGameShopItemBuy(LPBYTE lpRecv);
void GDReqInGameShopItemGift(int aIndex, int ID1, int ID2, int ID3, int Price, BYTE CoinType, char Target[11], char Message[200]);
void DGAnsInGameShopItemGift(LPBYTE lpRecv);
void GDReqInGameShopItemUse(int aIndex, int UniqueCode, int AuthCode);
void DGAnsInGameShopItemUse(ISHOP_ITEM_USEANS *lpMsg);
void GDReqInGameShopItemDelete(int aIndex, int UniqueCode, int AuthCode);
void GDReqInGameShopItemRollbackUse(int aIndex, int UniqueCode, int AuthCode);

void GDReqMuBotSave(int aIndex, BYTE MuBotData[257]);
void DGAnsMuBotData(MUBOT_DATA *lpMsg);
void GDReqBanUser(int aIndex, BYTE Type, BYTE Ban);
void GDSetExpWare(int aIndex, BYTE ExpansionType, BYTE ExpansionLevel);
void GDCharCardBuy(char AccountID[11], BYTE Type);
void GDReqSecLock(int aIndex, int Code);
void ReqMonsterCountAns(DS_GET_MONSTERCNT * aRecv);
void ReqMonsterCount(short aIndex);
void ReqSaveMonsterCount(short aIndex, int monsterid);
void ReqSavePlayerKiller(short kIndex, short vIndex);

void GDReqArcaBattleGuildJoin(OBJECTSTRUCT *obj);
void DGAnsArcaBattleGuildJoin(_tagPMSG_ANS_ARCA_BATTLE_GUILD_JOIN_DS *lpMsg);

void DGAnsArcaBattleGuildJoinSelect(_tagPMSG_ANS_ARCA_BATTLE_GUILD_JOIN_DS *lpMsg);

void GDReqArcaBattleGuildMemberJoin(OBJECTSTRUCT *obj);
void DGAnsArcaBattleGuildMemberJoin(_tagPMSG_ANS_ARCA_BATTLE_GUILD_MEMBER_JOIN_DS *lpMsg);

void GDReqArcaBattleEnter(OBJECTSTRUCT *obj, BYTE btEnterSeq);
void DGAnsArcaBattleEnter(_tagPMSG_ANS_ARCA_BATTLE_ENTER_DS *lpMsg);

void DGAnsArcaBattleWinGuildInfo(_tagPMSG_ANS_AB_WIN_GUILD_INFO_DS *lpMsg);

void DGReqArcaBattleProcMultiCast(_tagPMSG_ANS_AB_PROC_STATE_DS *lpMsg);

void GDAnsJoinMemberUnder(_tagPMSG_ANS_AB_JOIN_MEMBER_UNDER_DS *lpMsg);

void DGAns_ChaosCastle_KillPoint_Result(SDHP_ANS_KILLPOINT_RESULT_CC_UBF *lpMsg);
void DGAns_CCF_Info_Notify(SDHP_ANS_CCF_INFO_NOTIFY *lpMsg);
void DGAns_GetCCFPermission(SDHP_ANS_CCF_GETPERMISSION *lpMsg);

void GCMuunInventoryItemListSend(int aIndex);

void GDReqSaveEventInvenItem(OBJECTSTRUCT *lpObj);
void GDReqLoadEventInvenItem(OBJECTSTRUCT *lpObj);
void DGLoadEventInvenItem(_tagSDHP_ANS_DBEVENT_INVEN_LOAD *lpMsg);

void GDReqSwitchWare(int aIndex, int WareID);
void DGAnsSwitchWare(PMSG_ANS_SWITCHWARE *lpMsg);

void DGAnsUBFAccountUserInfo(PMSG_ANS_UBF_ACCOUNT_USERINFO *lpMsg);
void DGUBFRegisterAccountUserResult(PMSG_UBF_REGISTER_ACCOUNT_USER_RESULT *lpMsg);
void DGUBFAccountUserCopyResult(PMSG_UBF_ACCOUNT_USER_COPY_RESULT *lpMsg);
void DGAnsUBFGetReward(PMSG_ANS_UBF_GET_REWARD *lpMsg);
void DGAnsSetReceivedWinnerItem(PMSG_ANS_UBF_SET_RECEIVED_REWARD *lpMsg);
void DGUBFAnsCancelToJoinResult(PMSG_UBF_ANS_CANCEL_REGISTER_USER *lpMsg);
void DGUBFAnsGetRealNameAndServerCode(PMSG_ANS_GET_UBF_REAL_NAME *lpMsg);
void GDReqCopyPetItemInfo(int aIndex);

void GDReqDSFCanPartyEnter(int aIndex);
void DG_DSF_CanPartyEnter(PMSG_ANS_DSF_CAN_PARTY_ENTER *lpMsg);
void DG_DSF_PartyRankRenew(PMSG_ANS_DSF_PARTYRANKRENEW *lpMsg);
void DG_DSF_GoFinalParty(LPBYTE lpRecv);
void DG_DSF_GetReward(PMSG_ANS_GET_DSF_REWARD *lpMsg);
void DGReqOtherChannelWishper(PMSG_RECV_CHATDATA_WHISPER *lpMsg);
void GDReqMapSrvGroupServerCount();
void DGWhisperResponseRecv(DSMSG_GS_WHISPER_RESULT * aRecv);
void GDWhisperResultSend(BYTE Result, int OriginPlayerIndex, int OriginGS);
void GDDisconnectOtherChannel(PMSG_RECV_DC_OTHER_CHANNEL * aRecv);
void GDReqDisconnectOtherChannel(char * charname);
void DGDisconnectOtherChannel(PMSG_RECV_DC_OTHER_CHANNEL * aRecv);
extern int gMapSrvGroupOnlineServer;
#endif /*DSPROTOCOL_H*/

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

