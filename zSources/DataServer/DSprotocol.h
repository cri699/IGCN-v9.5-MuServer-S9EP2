// DSprotocol.h: interface for the DSprotocol class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DSPROTOCOL_H__2BBE1FF8_64DE_46AC_8344_13E7059DCBE3__INCLUDED_)
#define AFX_DSPROTOCOL_H__2BBE1FF8_64DE_46AC_8344_13E7059DCBE3__INCLUDED_

#include "Query.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*
 *	MAP OF DB ITEMS
 *
 *  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16
 *	00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 *
 *  1 - Type (0..4) bits
 *  2 - Level(3..6)-Luck(2)-Option(0..1)-Skill(7)
 *  3 - Dur
 *  4,5,6,7 - Serial
 *  8 - NewOption(0..5)-Option16(6)-(Type(7)
 *	9 - SetOption
 * 10 - ItemType(4..7) and ItemEffectEx(0..3)
 * 11 - JOH
 *  
 */
/*
 *	CHAR SET
 *	
 *	It Contains the Character Settings
 *	CS_CLASS(0)	Contains the Class and the ChangeUP
 *				You can use CS_GET_CLASS and CS_GET_CHANGEUP
 *	MAP of CHAR SET
 *
 *	    0         1	        2	      3	        4	      5	  	    6	      7
 *	0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000
 *	AAAB   CC EEEE EEEE GGGG GGGG JJJJ MMMM PPPP SSSS VVVV WWWW XXXX XXXX YYYY YYYY
 *
 *	    8         9		    10	      11	    12	      13	    14	      15
 *	0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000
 *	ZZZZ ZZZZ ILOR Uhhh aaaa aaad bbbb bbbc DDDD  i e FFFF HHHH KKKK NNNN QQQQ TTTT
 *
 *	    16	      17
 *	0000 0000 0000 0000
 *	       gf         
 *
 *	AAA Contain Class Character Definition (DK:1 DW:0 ELF:2 MG:3 DL:4)
 *	B Contains the Change UP (0:Normal 1:Evolved)
 *	CC Contains a value Acording m_ActionNumber fron gObj
 *
 *	DDDD Contains the 8..11 bits of Rigth Hand Item Index
 *	EEEE EEEE Contains the 0..7 bits Rigth Hand Item Index
 *
 *	FFFF Contains the 8..11 bits of Left Hand Item Index
 *	GGGG GGGG Contains the 0..7 bits of Left Hand Item Index
 *
 *	HHHH Contains the 5..8 Bits from Helmet Item Index
 *	I Contains the 4 bit from Helmet Item Index
 *	JJJJ Contains the 0..3 bits from Helmet Item Index
 *
 *	KKKK Contains the 5..8 Bits from Armor Item Index
 *	L Contains the 4 bit from Armor Item Index
 *	MMMM Contains the 0..3 bits from Armor Item Index
 *
 *	NNNN Contains the 5..8 Bits from Pants Item Index
 *	O Contains the 4 bit from Pants Item Index
 *	PPPP Contains the 0..3 bits from Pants Item Index
 *
 *	QQQQ Contains the 5..8 Bits from Gloves Item Index
 *	R Contains the 4 bit from Gloves Item Index
 *	SSSS Contains the 0..3 bits from Gloves Item Index
 *
 *	TTTT Contains the 5..8 Bits from Boots Item Index
 *	U Contains the 4 bit from Boots Item Index
 *	VVVV Contains the 0..3 bits from Boots Item Index
 *
 *	WW-WW Contains Wings and Helper(0..1, 0..1 bits) for Wings2 is set 0x0C
 *
 *	XXXX XXXX Contains Levels (Shine Levels) from Wings and Boots
 *	YYYY YYYY Contains Levels (Shine Levels) from Gloves and Pants and Armor
 *	ZZZZ ZZZZ Contains Levels (Shine Levels) from Helmet , Right and Left HAnd
 *
 *	aaaa aaa Contains the If the item is Excellent
 *
 *	bbbb bbb Contains the If the item is SetItem
 *	c	1 if the Char have the Full Set Item
 *
 *	d Unkown
 *
 *	e Have Dark Horse?
 *
 *	f	Fenrir Inc Attack Damage
 *	g	Fenrir Dec Attack Damage
 *	hhh Wing2 Types (m_Type - 2) Cape Of Lord 101(5)
 *	i Is Fenrir?
 */


#define CHAR_SET_SIZE		18

	#define CS_CLASS		0

	#define CS_WEAPON1_TYPE	1
	#define CS_WEAPON2_TYPE	2

	#define CS_WEAPON1_DATA	12
	#define CS_WEAPON2_DATA	13

	#define CS_HELMET1		13
	#define CS_HELMET2		9
	#define CS_HELMET3		3

	#define CS_ARMOR1		14
	#define CS_ARMOR2		9
	#define CS_ARMOR3		3

	#define CS_PANTS1		14
	#define CS_PANTS2		9
	#define CS_PANTS3		4

	#define CS_GLOVES1		15
	#define CS_GLOVES2		9
	#define CS_GLOVES3		4

	#define CS_BOOTS1		15
	#define CS_BOOTS2		9
	#define CS_BOOTS3		5


#define CHECK_CLASS(value, type) ( ((value)!=(type))?FALSE:TRUE )

#define CS_GET_CLASS(x) ( (((x)>>4)<<5)&(0xE0) )
#define CS_GET_CHANGEUP(x) ( ((x)&0x07) )
#define CS_SET_CLASS(x) ( ((x)<<5)& 0xE0 )
#define CS_SET_CHANGEUP(x) ( ((x) << 4) & 0x10 )

#define CS_SET_HELMET1(x) ( ((x) & 0x1E0) >> 5 )
#define CS_SET_HELMET2(x) ( ((x) & 0x10 ) << 3 )
#define CS_SET_HELMET3(x) ( ((x) & 0x0F ) << 4 )

#define CS_SET_ARMOR1(x) ( ((x) & 0x1E0) >> 1 )
#define CS_SET_ARMOR2(x) ( ((x) & 0x10 ) << 2 )
#define CS_SET_ARMOR3(x) ( ((x) & 0x0F )      )

#define CS_SET_PANTS1(x) ( ((x) & 0x1E0) >> 5 )
#define CS_SET_PANTS2(x) ( ((x) & 0x10 ) << 1 )
#define CS_SET_PANTS3(x) ( ((x) & 0x0F ) << 4 )

#define CS_SET_GLOVES1(x) ( ((x) & 0x1E0) >> 1 )
#define CS_SET_GLOVES2(x) ( ((x) & 0x10 )      )
#define CS_SET_GLOVES3(x) ( ((x) & 0x0F )      )

#define CS_SET_BOOTS1(x) ( ((x) & 0x1E0) >> 5 )
#define CS_SET_BOOTS2(x) ( ((x) & 0x10 ) >> 1 )
#define CS_SET_BOOTS3(x) ( ((x) & 0x0F ) << 4 )

#define CS_SET_WING1(x)  ( ((x) & 0x03 ) << 2 )
#define CS_SET_HELPER(x) ( ((x) & 0x03 )      )

#define CS_SET_SMALLLEVEL_RH(x)		( (x)       )
#define CS_SET_SMALLLEVEL_LH(x)		( (x) << 3  ) 
#define CS_SET_SMALLLEVEL_HELMET(x)	( (x) << 6  )
#define CS_SET_SMALLLEVEL_ARMOR(x)	( (x) << 9  )
#define CS_SET_SMALLLEVEL_PANTS(x)	( (x) << 12 )
#define CS_SET_SMALLLEVEL_GLOVES(x)	( (x) << 15 )
#define CS_SET_SMALLLEVEL_BOOTS(x)	( (x) << 18 )

#define CS_SET_SMALLLEVEL1(x)		( ((x) >> 16) & 0xFF )
#define CS_SET_SMALLLEVEL2(x)		( ((x) >> 8 ) & 0xFF )
#define CS_SET_SMALLLEVEL3(x)		((x) & 0xFF )

#define DBI_GET_TYPE(x) ( ((x)&((MAX_TYPE_ITEMS-1)<<8))>>4 )
#define DBI_GET_INDEX(x) ( (x) & 0xFF )
#define DBI_GET_LEVEL(x)  ( ((x)>>3)& MAX_ITEM_LEVEL )
#define DBI_GET_SKILL(x)  ( ((x) >> 7) & 0x01)
#define DBI_GET_LUCK(x)  ( ((x) >> 2) & 0x01)
#define DBI_GET_OPTION(x)  ( ( (x) ) & 0x03 )
#define DBI_GET_OPTION16(x) ( (x) & 0x40  )
#define DBI_GET_DINO_OPTION(x)  ( DBI_GET_OPTION16((x)) >> 4 )
#define DBI_GET_NOPTION(x)  ( (x) & 0x3F  )
#define DBI_GET_380OPTION(x) ( ((x) & 0x08) << 4  )
#define ITEM_GET_TYPE(x)  ( (x)/MAX_SUBTYPE_ITEMS  )
#define ITEM_GET_INDEX(x) ( (x)%MAX_SUBTYPE_ITEMS  )

#define CHAR_SET_SIZE 18
#define MAX_ITEM_INFO 12
#define INVENTORY_BINARY_SIZE 7584
#define MAX_DBITEM_INFO 32
#define CHAOS_BOX_SIZE 32

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


struct SDHP_CREATECHAR
{
	PBMSG_HEAD h;	// C1:04
	int UserNumber;	// 4
	int DBNumber;	// 8
	short Number;	// C
	char AccountId[11];	// E
	char Name[11];	// 18
	BYTE ClassSkin;	// 22
};

struct SDHP_VIPINFORECV
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

struct SDHP_CREATECHARRESULT
{
	PBMSG_HEAD h;	// C1:04
	unsigned char Result;	// 3
	short Number;	// 4
	char AccountId[11];	// 6
	char Name[11];	// 10
	BYTE Pos;	// 1A
	BYTE ClassSkin;	// 1B
	BYTE Equipment[24];	// 1C
	WORD Level;	// 34
};

struct SDHP_CHARDELETE
{
	PBMSG_HEAD h;	// C1:05
	short Number;	// 4
	char AccountID[11];	// 6
	char Name[11];	// 10
	BYTE Guild;	// [0:NoGuild] [1:Master] [2:Member] 1A
	char GuildName[8];	// 1B
};


struct SDHP_CHARDELETERESULT
{
	PBMSG_HEAD h;	// C1:05
	unsigned char Result;	// 3
	short Number;	// 4
	char AccountID[11];	// 6
};


struct SDHP_DBCHARINFOREQUEST
{
	PBMSG_HEAD h;	// C1:06
	char AccountID[11];	// 3
	char Name[11];	// E
	short Number;	// 1A
	bool IsSeasonX;
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
	BYTE dbInventory[7584];
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



/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Char List Count Packet
 *	Direction : GameServer -> Client
 *  Code     : 0xC1
 *	HeadCode : 0xF3
 *	SubCode  : 0x00
 *	BASE PACKET - Complete with PMSG_CHARLIST
 */
struct PMSG_CHARLISTCOUNT
{
	PBMSG_HEAD h;	// C1:F3:00
	BYTE subcode;	// 3
	BYTE MaxClass;	// 4
	BYTE MoveCnt;	// 5
	BYTE Count;	// 6
};


struct PMSG_CHARLIST
{
	BYTE Index;	// 0
	char Name[10];	// 1
	WORD Level;	// C
	BYTE CtlCode;	// E
	BYTE CharSet[CHAR_SET_SIZE];	// F
	BYTE btGuildStatus;	// 21
};



/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Get Char List Request Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 0x01
 */
struct SDHP_GETCHARLIST
{
	PBMSG_HEAD h;	// C1:01
	char Id[10];	// 3
	short Number;	// E
	int IsUnityBattleFieldServer;
};



/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Get Char List Result Packet
 *	Direction : GameServer -> Client
 *  Code     : 0xC1
 *	HeadCode : 0xF3
 *	SubCode  : 0x01
 */
struct PMSG_CHARCREATERESULT
{
	PBMSG_HEAD h;	// C1:F3:01
	BYTE subcode;	// 3
	unsigned char Result;	// 4
	unsigned char Name[10];	// 5
	BYTE pos;	// F
	WORD Level;	// 10
	BYTE Class;	// 12
	BYTE Equipment[24];	// 13
};

/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Get Item List Packet
 *	Direction : GameServer -> Client
 *  Code     : 0xC4
 *	HeadCode : 0xF3
 *	SubCode  : 0x10
 *	BASE PACKET - Complete with PMSG_INVENTORYLIST
 */
struct PMSG_INVENTORYLISTCOUNT
{
	PWMSG_HEAD h;	// C4:F3:10
	BYTE subcode;	// 4
	BYTE Count;	// 5
};

struct PMSG_INVENTORYLIST
{
	BYTE Pos;	// 0
	BYTE ItemInfo[MAX_ITEM_INFO];	// 1
};





/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Save Character Info Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC2
 *	HeadCode  : 0x07
 */
struct SDHP_DBCHAR_INFOSAVE
{
	PWMSG_HEAD h;	// C2:07
	char AccountID[11];
	char Name[11];	// 4
	short Level;
	short mLevel;
	BYTE Class;	// 12
	int LevelUpPoint;	// 14
	int mlPoint;
	UINT64 Exp;	// 18
	UINT64 mlExp;
	UINT64 NextExp;	// 1C
	UINT64 mlNextExp;
	int Money;	// 20
	WORD Str;	// 24
	WORD Dex;	// 26
	WORD Vit;	// 28
	WORD Energy;	// 2A
	DWORD Life;	// 2C
	DWORD MaxLife;	// 2E
	DWORD Mana;	// 30
	DWORD MaxMana;	// 32
	BYTE dbInventory[7584];	// 34
	BYTE dbMagicList[450];	// 6F4
	BYTE MapNumber;	// 7A8
	BYTE MapX;	// 7A9
	BYTE MapY;	// 7AA
	BYTE Dir;	// 7AB
	int PkCount;	// 7AC
	int PkLevel;	// 7B0
	int PkTime;	// 7B4
	BYTE dbQuest[100];	// 7B8
	BYTE CharInfoSave;	// 7EA
	WORD Leadership;	// 7EC
	WORD ChatLitmitTime;	// 7EE
	int iFruitPoint;	// 7F0
	int resets;
	char MarryName[11];
	char Married;
	int WinDuels;
	int LoseDuels;
	time_t ChatBlockTime;
	DWORD PenaltyMask;
};




/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Get Warehouse Info Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 0x08
 */
struct SDHP_GETWAREHOUSEDB
{
	PBMSG_HEAD h;	// C1:08
	char AccountID[11];	// 3
	short aIndex;	// E
};

/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu DB Char Item Save Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC2
 *	HeadCode  : 0x11
 */
struct SDHP_DBCHAR_ITEMSAVE
{
	PWMSG_HEAD h;	// C2:11
	char Name[11];	// 4
	BYTE dbInventory[7584];	// 34
};



/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Warehouse Money Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 0x12
 */
struct SDHP_WAREHOUSEMONEY_SAVE
{
	PBMSG_HEAD h;	// C1:12
	char AccountID[11];	// 3
	short aIndex;	// E
	int Money;	// 10
};



/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Item Serial Create Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 0x52, 0x55 (in Case of PET )
 */
struct SDHP_ITEMCREATE
{
	PBMSG_HEAD h;	// C1:XX
	BYTE x;	// 3
	BYTE y;	// 4
	BYTE MapNumber;	// 5 [235:Mu_2nd_Aniv], [236:CashShop]
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

/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Skill Key Data Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 0x60
 */
struct SDHP_SKILLKEYDATA
{
	PBMSG_HEAD h;
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

struct Request_PetItem_Info
{
	BYTE nPos;	// 0
	UINT64 nSerial;	// 4
};


/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Request Pet Info Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC2
 *	HeadCode  : 0x56
 *	BASE PACKET - Complete with Request_PetItem_Info
 */
struct SDHP_REQUEST_PETITEM_INFO
{
	PWMSG_HEAD h;
	char AccountID[11];	// 4
	WORD Number;	// 10
	BYTE InvenType;	// 12
	BYTE nCount;	// 13
	WORD ServerType;
	WORD ServerCode;
};

struct Recv_PetItem_Info
{
	BYTE nPos;	// 0
	UINT64 nSerial;	// 4
	BYTE Level;	// 8
	UINT64 Exp;	// C
};

struct SDHP_RECV_PETITEM_INFO
{
	PWMSG_HEAD h;
	char AccountID[11];	// 4
	WORD Number;	// 10
	BYTE InvenType;	// 12
	BYTE nCount;	// 13
};

struct Save_PetItem_Info
{
	UINT64 nSerial;	// 0
	BYTE Level;	// 3
	UINT64 Exp;	// 8
};

/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu PetInfo Save Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC2
 *	HeadCode  : 0x55
 *	BASE PACKET - Complete with Save_PetItem_Info
 */
struct SDHP_SAVE_PETITEM_INFO
{
	PWMSG_HEAD h;
	BYTE nCount;	// 4
	WORD ServerType;
	WORD ServerCode;
};

/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_CASTLEDATA
{
	PBMSG_HEAD2 h;	// C1:80:00
	WORD wMapSvrNum;	// 4
	int iCastleEventCycle;	// 8
};



/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_OWNERGUILDMASTER
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 4
	int iIndex;	// 8
};



/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_NPCBUY
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 4
	int iIndex;	// 8
	int iNpcNumber;	// C
	int iNpcIndex;	// 10
	int iNpcDfLevel;	// 14
	int iNpcRgLevel;	// 18
	int iNpcMaxHp;	// 1C
	int iNpcHp;	// 20
	BYTE btNpcX;	// 24
	BYTE btNpcY;	// 25
	BYTE btNpcDIR;	// 26
	int iBuyCost;	// 28
};




/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_NPCREPAIR
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 4
	int iIndex;	// 8
	int iNpcNumber;	// C
	int iNpcIndex;	// 10
	int iRepairCost;	// 14
};



/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_NPCUPGRADE
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 4
	int iIndex;	// 8
	int iNpcNumber;	// C
	int iNpcIndex;	// 10
	int iNpcUpType;	// 14
	int iNpcUpValue;	// 18
	int iNpcUpIndex;	// 1C
};



/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_TAXINFO
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 3
	int iIndex;	// 8
};



/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_TAXRATECHANGE
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 4
	int iIndex;	// 8
	int iTaxKind;	// C
	int iTaxRate;	// 10
};




/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_MONEYCHANGE
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 4
	int iIndex;	// 8
	int iMoneyChanged;	// C
};



/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_SDEDCHANGE
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 4
	int iIndex;	// 8
	WORD wStartYear;	// C
	BYTE btStartMonth;	// E
	BYTE btStartDay;	// F
	WORD wEndYear;	// 10
	BYTE btEndMonth;	// 12
	BYTE btEndDay;	// 13
};



/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_GUILDREGINFO
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 4
	int iIndex;	// 8
	char szGuildName[8];	// C
};


/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_SIEGEENDCHANGE
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 4
	BOOL bIsSiegeEnded;	// 8
};


/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_CASTLEOWNERCHANGE
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 4
	BOOL bIsCastleOccupied;	// 8
	char szOwnerGuildName[8];	// C
};



/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_REGATTACKGUILD
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 4
	int iIndex;	// 8
	char szEnemyGuildName[8];	// C
};



/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_CASTLESIEGEEND
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 4
};


/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_MAPSVRMULTICAST
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 4
	char szMsgText[128];	// 6
};

struct CSP_GLOBALPOST_MULTICAST
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 4
	char szSender[MAX_ACCOUNT_LEN];
	char szMessage[MAX_CHAT_LEN];
	char szServerName[50];
	BYTE btColorRGB[3];
};

/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_GUILDREGMARK
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 4
	int iIndex;	// 8
	char szGuildName[8];	// C
	int iItemPos;	// 14
};

/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_GUILDRESETMARK
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 4
	int iIndex;	// 8
	char szGuildName[8];	// C
};


/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_GUILDSETGIVEUP
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 4
	int iIndex;	// 8
	char szGuildName[8];	// C
	BOOL bIsGiveUp;	// 14
};


/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_NPCREMOVE
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 4
	int iNpcNumber;	// 8
	int iNpcIndex;	// C
};


/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_CASTLESTATESYNC
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 4
	int iCastleState;	// 8
	int iTaxRateChaos;	// C
	int iTaxRateStore;	// 10
	int iTaxHuntZone;	// 14
	char szOwnerGuildName[8];	// 18
};



/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_CASTLETRIBUTEMONEY
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 4
	int iCastleTributeMoney;	// 8
};



/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_RESETCASTLETAXINFO
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 4
};

/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_RESETSIEGEGUILDINFO
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 4
};

/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode   :
 */
struct CSP_REQ_RESETREGSIEGEINFO
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 2
};



/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 */
struct CSP_REQ_CSINITDATA
{
	PBMSG_HEAD h;
	WORD wMapSvrNum;	// 4
	int iCastleEventCycle;	// 8
};



/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 */
struct CSP_REQ_NPCDATA
{
	PBMSG_HEAD h;
	WORD wMapSvrNum;	// 4
	int iIndex;	// 8
};



/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 */
struct CSP_REQ_ALLGUILDREGINFO
{
	PBMSG_HEAD h;
	WORD wMapSvrNum;	// 4
	int iIndex;	// 8
};


/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 */
struct CSP_REQ_CALCREGGUILDLIST
{
	PBMSG_HEAD h;
	WORD wMapSvrNum;	// 4
};



/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : GameServer -> DataServer
 *  Code      : 0xC1
 *	HeadCode  : 
 */
struct CSP_REQ_CSLOADTOTALGUILDINFO
{
	PBMSG_HEAD h;
	WORD wMapSvrNum;	// 4
};


/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : DataServer -> GameServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode	  :
 */
struct CSP_ANS_MAPSVRMULTICAST
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 4
	char szMsgText[128];	// 6
};


/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : DataServer -> GameServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode	  :
 */
struct CSP_ANS_CASTLESTATESYNC
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;	// 4
	int iCastleState;	// 8
	int iTaxRateChaos;	// C
	int iTaxRateStore;	// 10
	int iTaxHuntZone;	// 14
	char szOwnerGuildName[8];	// 18
};


/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Castle Siege          Packet
 *	Direction : DataServer -> GameServer
 *  Code      : 0xC1
 *	HeadCode  : 
 *	SubCode	  :
 */
struct CSP_ANS_CASTLETRIBUTEMONEY
{
	PBMSG_HEAD2 h;
	int iResult;	// 4
	WORD wMapSvrNum;	// 8
};



struct CWP_REQ_CRYWOLFSYNC
{
	PBMSG_HEAD h;
	WORD wMapSvrNum;	// 4
	int iCrywolfState;	// 8
	int iOccupationState;	// C
	int iApplyBenefit;
	int iApplyPenalty;
	int iPlusChaosRate;
	int iGemDropPenaltyRate;
	int iGettingExpPenaltyRate;
	int iMinusMonHPBenefitRate;
	int iKundunHPRefillState;
};



struct CWP_ANS_CRYWOLFSYNC
{
	PBMSG_HEAD h;
	BYTE btResult;	// 3
	int iCrywolfState;	// 4
	int iOccupationState;	// 8
	int iApplyBenefit;
	int iApplyPenalty;
	int iPlusChaosRate;
	int iGemDropPenaltyRate;
	int iGettingExpPenaltyRate;
	int iMinusMonHPBenefitRate;
	int iKundunHPRefillState;
};



struct CWP_REQ_CRYWOLFINFOLOAD
{
	PBMSG_HEAD h;
	WORD wMapSvrNum;	// 4
};



struct CWP_ANS_CRYWOLFINFOLOAD
{
	PBMSG_HEAD h;
	BYTE btResult;	// 3
	int iCrywolfState;	// 4
	int iOccupationState;	// 8
};

struct CWP_REQ_CRYWOLFINFOSAVE
{
	PBMSG_HEAD h;
	WORD wMapSvrNum;	// 4
	int iCrywolfState;	// 8
	int iOccupationState;	// C
};



struct CWP_ANS_CRYWOLFINFOSAVE
{
	PBMSG_HEAD h;
	BYTE btResult;	// 3
};


struct SDHP_CHARACTER_TRANSFER
{
	PBMSG_HEAD h;	// C1:CF
	char Account[10];	// 3
	short Number;	// E
};

struct CSP_ANS_CSINITDATA
{
	struct PWMSG_HEAD h;	// C2:81
	int iResult;	// 0x04
	unsigned short wMapSvrNum;	// 0x08
	unsigned short wStartYear;	// 0x0A
	unsigned char btStartMonth;	// 0x0C
	unsigned char btStartDay;	// 0x0D
	unsigned short wEndYear;	// 0x0E
	unsigned char btEndMonth;	// 0x10
	unsigned char btEndDay;		// 0x11
	unsigned char btIsSiegeGuildList;	// 0x12
	unsigned char btIsSiegeEnded;	// 0x13
	unsigned char btIsCastleOccupied; // 0x14
	char szCastleOwnGuild[0x8];	// 0x15
	__int64 i64CastleMoney;	// 0x20
	int iTaxRateChaos;	// 0x28
	int iTaxRateStore;	// 0x2C
	int iTaxHuntZone;	// 0x30
	int iFirstCreate;	// 0x34
	int iCount;	// 0x38
};
// size 0x3C

struct CSP_CSINITDATA
{
	int iNpcNumber;	// 0
	int iNpcIndex;	// 4
	int iNpcDfLevel;// 8
	int iNpcRgLevel;// C
	int iNpcMaxHp;	// 10
	int iNpcHp;	// 14
	unsigned char btNpcX;//18
	unsigned char btNpcY;//19
	unsigned char btNpcDIR;//1A
};

struct CSP_ANS_OWNERGUILDMASTER
{
	PBMSG_HEAD2 h; // C1:80:1
	int iResult;	// 4
	unsigned short wMapSvrNum;	// 8
	int iIndex;	// C
	char szCastleOwnGuild[0x8];	// 10
	char szCastleOwnGuildMaster[0xA];	// 18
};

struct CSP_ANS_NPCBUY
{
	PBMSG_HEAD2 h;	// C1:80:03
	int iResult;	// 4
	unsigned short wMapSvrNum;	// 8
	int iIndex;	// C
	int iNpcNumber;	// 10
	int iNpcIndex;	// 14
	int iBuyCost;	// 18
};

struct CSP_ANS_NPCREPAIR
{
	PBMSG_HEAD2 h;	// C1:80:04
	int iResult;	// 4
	unsigned short wMapSvrNum;	// 8
	int iIndex;	// C
	int iNpcNumber;	// 10
	int iNpcIndex;	// 14
	int iNpcMaxHp;	// 18
	int iNpcHpl;	// 1C
	int iRepairCost;	// 20
};

struct CSP_ANS_NPCUPGRADE
{
	PBMSG_HEAD2 h;	// C1:80:05
	int iResult;	// 4
	unsigned short wMapSvrNum;	// 8
	int iIndex;	// C
	int iNpcNumber;	// 10
	int iNpcIndex;	// 14
	int iNpcUpType;	// 18
	int iNpcUpValue;	// 1C
	int iNpcUpIndex;	// 20
};

struct CSP_ANS_TAXINFO
{
	PBMSG_HEAD2 h;	// C1:80:06
	int iResult;	// 4
	unsigned short wMapSvrNum;	// 8
	int iIndex;	// C
	__int64 i64CastleMoney;	// 10
	int iTaxRateChaos;	// 18
	int iTaxRateStore;	// 1C
	int iTaxHuntZone;	// 20
};

struct CSP_ANS_TAXRATECHANGE
{
	PBMSG_HEAD2 h;	// C1:80:07
	int iResult;	// 4
	unsigned short wMapSvrNum;	// 8
	int iIndex;	// C
	int iTaxKind;	// 10
	int iTaxRate;	// 14
};

struct CSP_ANS_MONEYCHANGE
{
	PBMSG_HEAD2 h;	// C1:80:08
	int iResult;	// 4
	unsigned short wMapSvrNum;	// 8
	int iIndex;	// C
	int iMoneyChanged;	// 10
	__int64 i64CastleMoney;	// 18
};

struct CSP_ANS_SDEDCHANGE
{
	PBMSG_HEAD2 h;	// C1:80:09
	int iResult;	// 4
	unsigned short wMapSvrNum;	// 8
	int iIndex;	// C
	unsigned short wStartYear;	// 10
	unsigned char btStartMonth;	// 12
	unsigned char btStartDay;	// 13
	unsigned short wEndYear;	// 14
	unsigned char btEndMonth;	// 16
	char btEndDay;	// 17
};

struct CSP_ANS_GUILDREGINFO
{
	PBMSG_HEAD2 h;	// C1:80:0A
	int iResult;	// 4
	unsigned short wMapSvrNum;	// 8
	int iIndex;	// C
	char szGuildName[0x8];	// 10
	int iRegMarkCount;	// 18
	unsigned char bIsGiveUp;	// 1C
	unsigned char btRegRank;	// 1D
};

struct CSP_ANS_SIEGEENDCHANGE
{
	PBMSG_HEAD2 h;	// C1:80:0B
	int iResult;	// 4
	unsigned short wMapSvrNum;	// 8
	int bIsSiegeEnded;	// C
};

struct CSP_ANS_CASTLEOWNERCHANGE
{
	PBMSG_HEAD2 h;	// C1:80:0C
	int iResult;	// 4
	unsigned short wMapSvrNum;	// 8
	int bIsCastleOccupied;	// C
	char szOwnerGuildName[0x8];	// 10
};

struct CSP_ANS_REGATTACKGUILD
{
	PBMSG_HEAD2 h;	// C1:80:0D
	int iResult;	// 4
	unsigned short wMapSvrNum;	// 8
	int iIndex;	// C
	char szEnemyGuildName[0x8];	// 10
};

struct CSP_ANS_CASTLESIEGEEND
{
	PBMSG_HEAD2 h;	// C1:80:0E
	int iResult;	// 4
	unsigned short wMapSvrNum;	// 8
};

struct CSP_ANS_GUILDREGMARK
{
	PBMSG_HEAD2 h;	// C1:80:10
	int iResult;	// 4
	unsigned short wMapSvrNum;	// 8
	int iIndex;	// C
	char szGuildName[0x8];	// 10
	int iItemPos;	// 18
	int iRegMarkCount;	// 1C
};

struct CSP_ANS_GUILDRESETMARK
{
	PBMSG_HEAD2 h;	// C1:80:11
	int iResult;	// 4
	unsigned short wMapSvrNum;	// 8
	int iIndex;	// C
	char szGuildName[0x8];	// 10
	int iRegMarkCount;	// 18
};

struct CSP_ANS_GUILDSETGIVEUP
{
	PBMSG_HEAD2 h;	// C1:80:12
	int iResult;	// 4
	unsigned short wMapSvrNum;	// 8
	int iIndex;	// C
	char szGuildName[0x8];	// 10
	int bIsGiveUp;	// 18
	int iRegMarkCount;	// 1C
};

struct CSP_ANS_NPCREMOVE
{
	PBMSG_HEAD2 h;	// C1:80:16
	int iResult;	// 4
	unsigned short wMapSvrNum;	// 8
	int iNpcNumber;	// C
	int iNpcIndex;	// 10
};

struct CSP_ANS_RESETCASTLETAXINFO
{
	PBMSG_HEAD2 h;	// C1:80:19
	int iResult;	// 4
	unsigned short wMapSvrNum;	// 8
};

struct CSP_ANS_RESETSIEGEGUILDINFO
{
	PBMSG_HEAD2 h;	// C1:80:1A
	int iResult;	// 4
	unsigned short wMapSvrNum;	// 8
};

struct CSP_ANS_RESETREGSIEGEINFO
{
	PBMSG_HEAD2 h;	// C1:80:1B
	int iResult;	// 4
	unsigned short wMapSvrNum;	// 8
};

struct CSP_ANS_ALLGUILDREGINFO
{
	struct PWMSG_HEAD h;	// C2:83
	int iResult;	// 4
	unsigned short wMapSvrNum;	// 8
	int iIndex;	// C
	int iCount;	// 10
};

struct CSP_GUILDREGINFO
{
	char szGuildName[0x8];	// 0
	int iRegMarkCount;	// 8
	unsigned char bIsGiveUp;	// C
	unsigned char btRegRank;	// D
};

struct CSP_REQ_NPCSAVEDATA
{
	PWMSG_HEAD h;	// C1:84
	unsigned short wMapSvrNum;	// 4
	int iCount;	// 8
};

struct CSP_NPCSAVEDATA
{
	int iNpcNumber;	// 0
	int iNpcIndex;	// 4
	int iNpcDfLevel;	// 8
	int iNpcRgLevel;	// C
	int iNpcMaxHp;	// 10
	int iNpcHp;	// 14
	unsigned char btNpcX;	// 18
	unsigned char btNpcY;	// 19
	unsigned char btNpcDIR;	// 1A
};

struct CSP_ANS_NPCSAVEDATA
{
	PBMSG_HEAD h;	// C1:84
	int iResult;	// 4
	unsigned short wMapSvrNum;	// 8
};

struct CSP_ANS_CALCREGGUILDLIST
{
	PWMSG_HEAD h;	// C1:85
	int iResult;	// 4
	unsigned short wMapSvrNum;	// 8
	int iCount;	// C
};

struct CSP_CALCREGGUILDLIST
{
	char szGuildName[0x8];	// 0
	int iRegMarkCount;	// 8
	int iGuildMemberCount;	// C
	int iGuildMasterLevel;	// 10
	int iSeqNum;	// 14
};

struct CSP_REQ_CSGUILDUNIONINFO
{
	PWMSG_HEAD h;	// C2:86
	unsigned short wMapSvrNum;	// 4
	int iCount;	// 8
};

struct CSP_CSGUILDUNIONINFO
{
	char szGuildName[0x8];	// 0
	int iCsGuildID;	// 8
};

struct CSP_ANS_CSGUILDUNIONINFO
{
	PWMSG_HEAD h;	// C2:86
	int iResult;	// 4
	unsigned short wMapSvrNum;	// 8
	int iCount;	// C
};

#pragma pack (1)
struct CSP_REQ_CSSAVETOTALGUILDINFO
{
	PWMSG_HEAD h;	// C2:87
	unsigned short wMapSvrNum;	// 4
	int iCount;	// 8
};

struct CSP_CSSAVETOTALGUILDINFO
{
	char szGuildName[0x8];	// 0
	int iCsGuildID;	// 8
	int iGuildInvolved;	// C
	int iGuildScore;
};
#pragma pack ()

struct CSP_ANS_CSSAVETOTALGUILDINFO
{
	PBMSG_HEAD h;	// C1:87
	int iResult;	// 4
	unsigned short wMapSvrNum;	// 8
};

struct CSP_ANS_CSLOADTOTALGUILDINFO
{
	PWMSG_HEAD h;	// C2:88
	int iResult;	// 4
	unsigned short wMapSvrNum;	// 8
	int iCount;	// C
};

struct CSP_CSLOADTOTALGUILDINFO
{
	char szGuildName[0x8];	// 0
	int iCsGuildID;	// 8
	int iGuildInvolved;	// C
	int iGuildScore;
};

struct CSP_REQ_NPCUPDATEDATA
{
	PWMSG_HEAD h;	// C2:89
	unsigned short wMapSvrNum;	// 4
	int iCount;	// 8
};

struct CSP_NPCUPDATEDATA
{
	int iNpcNumber;	// 0
	int iNpcIndex;	// 4
	int iNpcDfLevel;	// 8
	int iNpcRgLevel;	// C
	int iNpcMaxHp;	// 10
	int iNpcHp;	// 14
	unsigned char btNpcX;	// 18
	unsigned char btNpcY;	// 19
	unsigned char btNpcDIR;	// 1A
};

struct CSP_ANS_NPCUPDATEDATA
{
	PBMSG_HEAD h;
	int iResult;
	unsigned short wMapSvrNum;
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

struct ISHOP_POINT_ADD
{
	PBMSG_HEAD		h;
	short			aIndex;
	char			AccountID[11];
	BYTE			Type;
	float			Coin;
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

struct ISHOP_CHARCARD_BUY
{
	PBMSG_HEAD		h;
	char			AccountID[11];
	BYTE			Type;
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

struct PMSG_ANS_REG_LUCKYCOIN
{
	PBMSG_HEAD2 h;
	int iIndex;
	char szUID[11];
	BYTE Result;
	BYTE Pos;
	int LuckyCoins;
};

struct PMSG_REQ_REGISTER_LUCKYCOIN
{
	PBMSG_HEAD2 h;
	int iIndex;
	char szAccountID[11];
	BYTE btPos;
};

struct PMSG_REQ_LUCKYCOIN
{
	PBMSG_HEAD2 h;
	int iIndex;
	char szUID[11];
};

struct PMSG_ANS_LUCKYCOIN
{
	PBMSG_HEAD2 h;
	int iIndex;
	char szUID[11];
	int LuckyCoins;
};

struct MUBOT_DATA // C2:AE
{
	PWMSG_HEAD		h;
	short			aIndex;
	char			Name[11];
	BYTE			MuBotData[257];
};

struct SECLOCK_REQ_SAVE
{
	PBMSG_HEAD		h;
	char			AccountID[11];
	int				Code;
};

/////////////////////////////// RANKING SERVER /////////////////////////////////////////////

/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu RankingServer DevilSquare Score
 *	Direction : GameServer -> RankingServer
 *  Code : 0xC1
 *	HeadCode : 0x01
 */

struct PMSG_ANS_EVENTUSERSCORE {
  struct PBMSG_HEAD2 h;
  char AccountID[10];	// 3
  char GameID[10];	// d
  int ServerCode;	// 18
  int Score;	// 1c
  int Class;	// 20
  int SquareNum; // 24
};
// <size 0x28>

/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu RankingServer Get BC Enter Count
 *	Direction : GameServer -> RankingServer
 *  Code : 0xC1
 *	HeadCode : 0x0B
 */

struct PMSG_REQ_BLOODCASTLE_ENTERCOUNT {
  struct PBMSG_HEAD2 h;
  char AccountID[10];	// 3
  char GameID[10];	// d
  int ServerCode;	// 18
  int iObjIndex;	// 1c
};

struct PMSG_REQ_ILLUSIONTEMPLE_ENTERCOUNT {
  struct PBMSG_HEAD2 h;
  char AccountID[10];	// 3
  char GameID[10];	// d
  int ServerCode;	// 18
  int iObjIndex;	// 1c
};
// <size 0x20>

/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu RankingServer BloodCastle_5TH Score
 *	Direction : GameServer -> RankingServer
 *  Code : 0xC1
 *	HeadCode : 0x0D
 */

struct PMSG_ANS_BLOODCASTLESCORE_5TH {
  struct PBMSG_HEAD2 h;
  char AccountID[10];	// 3
  char GameID[10];	// d
  int ServerCode;	// 18
  int Score;	// 1c
  int Class;	// 20
  int BridgeNum;	// 24
  int iLeftTime;	// 28
  int iAlivePartyCount;	// 2c
};
// <size 0x30>

struct PMSG_ANS_ILLUSIONTEMPLE_RANKING {
  struct PBMSG_HEAD2 h;
  char AccountID[10];	// 3
  char GameID[10];	// d
  int ServerCode;	// 1
  int Class;	// 20
  int Team;
  int TempleNum;	// 24
  int KillCount;
  int RelicsGivenCount;
  __int64 Experience;
  int TotalScore;
  BYTE IsWinner;
};

struct PMSG_ANS_CHAOSCASTLE_RANKING {
	struct PBMSG_HEAD2 h;
	char AccountID[10];
	char GameID[10];
	int ServerCode;
	int Class;
	int Castle;
	int PlayerKill;
	int MonsterKill;
	__int64 Experience;
	BYTE IsWinner;
};

struct PMSG_ANS_BLOODCASTLE_ENTERCOUNT {
  struct PBMSG_HEAD2 h;
  char AccountID[10];
  char GameID[10];
  int ServerCode;
  int iObjIndex;
  int iLeftCount;
};


///////////////////////////////////////// EVENT SERVER ////////////////////////////////////

struct PMSG_REQ_VIEW_EC_MN {
  /*<thisrel this+0x0>*/ /*|0x3|*/ struct PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iINDEX;
  /*<thisrel this+0x8>*/ /*|0xb|*/ char szUID[11];
};
// <size 0x14>

struct PMSG_REQ_REGISTER_EVENTCHIP {
  /*<thisrel this+0x0>*/ /*|0x3|*/ struct PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iINDEX;
  /*<thisrel this+0x8>*/ /*|0x1|*/ unsigned char Pos;
  /*<thisrel this+0x9>*/ /*|0xb|*/ char szUID[11];
};
// <size 0x14>

struct PMSG_REQ_RESET_EVENTCHIP {
  /*<thisrel this+0x0>*/ /*|0x3|*/ struct PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iINDEX;
  /*<thisrel this+0x8>*/ /*|0xb|*/ char szUID[11];
};
// <size 0x14>

struct PMSG_REQ_REGISTER_STONES {
  /*<thisrel this+0x0>*/ /*|0x3|*/ struct PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iINDEX;
  /*<thisrel this+0x8>*/ /*|0x1|*/ unsigned char iPosition;
  /*<thisrel this+0x9>*/ /*|0xb|*/ char szUID[11];
};
// <size 0x14>

struct PMSG_REQ_DELETE_STONES {
  /*<thisrel this+0x0>*/ /*|0x3|*/ struct PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iINDEX;
  /*<thisrel this+0x8>*/ /*|0xb|*/ char szUID[11];
  /*<thisrel this+0x13>*/ /*|0x1|*/ char bSUCCESS;
  /*<thisrel this+0x14>*/ /*|0x4|*/ int iStoneCount;
};
// <size 0x18>

struct PMSG_REQ_2ANIV_SERIAL {
  /*<thisrel this+0x0>*/ /*|0x3|*/ struct PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iINDEX;
  /*<thisrel this+0x8>*/ /*|0xb|*/ char szUID[11];
  /*<thisrel this+0x13>*/ /*|0x5|*/ char SERIAL1[5];
  /*<thisrel this+0x18>*/ /*|0x5|*/ char SERIAL2[5];
  /*<thisrel this+0x1d>*/ /*|0x5|*/ char SERIAL3[5];
  /*<thisrel this+0x24>*/ /*|0x4|*/ int iMEMB_GUID;
};
// <size 0x28>

struct PMSG_REQ_REG_RINGGIFT {
  /*<thisrel this+0x0>*/ /*|0x3|*/ struct PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iINDEX;
  /*<thisrel this+0x8>*/ /*|0xb|*/ char szUID[11];
  /*<thisrel this+0x13>*/ /*|0x1|*/ unsigned char btGiftKind;
};
// <size 0x14>

struct PMSG_REQ_REG_CC_OFFLINE_GIFT {
  /*<thisrel this+0x0>*/ /*|0x3|*/ struct PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iINDEX;
  /*<thisrel this+0x8>*/ /*|0xb|*/ char szUID[11];
  /*<thisrel this+0x14>*/ /*|0x2|*/ unsigned short wServerCode;
  /*<thisrel this+0x16>*/ /*|0xb|*/ char szNAME[11];
};
// <size 0x24>

struct PMSG_REQ_REG_DL_OFFLINE_GIFT {
  /*<thisrel this+0x0>*/ /*|0x3|*/ struct PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iINDEX;
  /*<thisrel this+0x8>*/ /*|0xb|*/ char szUID[11];
  /*<thisrel this+0x14>*/ /*|0x2|*/ unsigned short wServerCode;
  /*<thisrel this+0x16>*/ /*|0xb|*/ char szNAME[11];
};
// <size 0x24>

struct PMSG_REQ_REG_HT_OFFLINE_GIFT {
  /*<thisrel this+0x0>*/ /*|0x3|*/ struct PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iINDEX;
  /*<thisrel this+0x8>*/ /*|0xb|*/ char szUID[11];
  /*<thisrel this+0x14>*/ /*|0x2|*/ unsigned short wServerCode;
  /*<thisrel this+0x16>*/ /*|0xb|*/ char szNAME[11];
};
// <size 0x24>

/**************************************************************************/

struct PMSG_ANS_VIEW_EC_MN {
  /*<thisrel this+0x0>*/ /*|0x3|*/ struct PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iINDEX;
  /*<thisrel this+0x8>*/ /*|0xb|*/ char szUID[11];
  /*<thisrel this+0x13>*/ /*|0x1|*/ char bSUCCESS;
  /*<thisrel this+0x14>*/ /*|0x2|*/ short nEVENT_CHIPS;
  /*<thisrel this+0x18>*/ /*|0x4|*/ int iMUTO_NUM;
};
// <size 0x1c>

struct PMSG_ANS_REGISTER_EVENTCHIP {
  /*<thisrel this+0x0>*/ /*|0x3|*/ struct PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iINDEX;
  /*<thisrel this+0x8>*/ /*|0x1|*/ unsigned char Pos;
  /*<thisrel this+0x9>*/ /*|0xb|*/ char szUID[11];
  /*<thisrel this+0x14>*/ /*|0x1|*/ char bSUCCESS;
  /*<thisrel this+0x16>*/ /*|0x2|*/ short nEVENT_CHIPS;
};
// <size 0x18>

struct PMSG_ANS_RESET_EVENTCHIP {
  /*<thisrel this+0x0>*/ /*|0x3|*/ struct PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iINDEX;
  /*<thisrel this+0x8>*/ /*|0xb|*/ char szUID[11];
  /*<thisrel this+0x13>*/ /*|0x1|*/ char bSUCCESS;
};
// <size 0x14>

struct PMSG_ANS_VIEW_STONES {
  /*<thisrel this+0x0>*/ /*|0x3|*/ struct PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iINDEX;
  /*<thisrel this+0x8>*/ /*|0xb|*/ char szUID[11];
  /*<thisrel this+0x13>*/ /*|0x1|*/ char bSUCCESS;
  /*<thisrel this+0x14>*/ /*|0x4|*/ int iStoneCount;
};
// <size 0x18>

struct PMSG_ANS_REGISTER_STONES {
  /*<thisrel this+0x0>*/ /*|0x3|*/ struct PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iINDEX;
  /*<thisrel this+0x8>*/ /*|0x1|*/ unsigned char iPosition;
  /*<thisrel this+0x9>*/ /*|0xb|*/ char szUID[11];
  /*<thisrel this+0x14>*/ /*|0x1|*/ char bSUCCESS;
  /*<thisrel this+0x18>*/ /*|0x4|*/ int iStoneCount;
};
// <size 0x1c>

struct PMSG_ANS_DELETE_STONES {
  /*<thisrel this+0x0>*/ /*|0x3|*/ struct PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iINDEX;
  /*<thisrel this+0x8>*/ /*|0xb|*/ char szUID[11];
  /*<thisrel this+0x13>*/ /*|0x1|*/ char bSUCCESS;
  /*<thisrel this+0x14>*/ /*|0x4|*/ int iStoneCount;
};
// <size 0x18>

struct PMSG_ANS_2ANIV_SERIAL {
  /*<thisrel this+0x0>*/ /*|0x3|*/ struct PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iINDEX;
  /*<thisrel this+0x8>*/ /*|0xb|*/ char szUID[11];
  /*<thisrel this+0x13>*/ /*|0x1|*/ unsigned char btIsRegistered;
  /*<thisrel this+0x14>*/ /*|0x4|*/ int iGiftNumber;
};
// <size 0x18>

struct PMSG_ANS_REG_RINGGIFT {
  /*<thisrel this+0x0>*/ /*|0x3|*/ struct PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iINDEX;
  /*<thisrel this+0x8>*/ /*|0xb|*/ char szUID[11];
  /*<thisrel this+0x13>*/ /*|0x1|*/ unsigned char btIsRegistered;
  /*<thisrel this+0x14>*/ /*|0x1|*/ unsigned char btGiftSection;
  /*<thisrel this+0x15>*/ /*|0x1|*/ unsigned char btGiftKind;
};
// <size 0x18>

struct PMSG_ANS_REG_CC_OFFLINE_GIFT {
  /*<thisrel this+0x0>*/ /*|0x3|*/ struct PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iINDEX;
  /*<thisrel this+0x8>*/ /*|0xb|*/ char szUID[11];
  /*<thisrel this+0x14>*/ /*|0x2|*/ unsigned short wServerCode;
  /*<thisrel this+0x16>*/ /*|0xb|*/ char szNAME[11];
  /*<thisrel this+0x24>*/ /*|0x4|*/ int iResultCode;
  /*<thisrel this+0x28>*/ /*|0x32|*/ char szGIFT_NAME[50];
};
// <size 0x5c>

struct PMSG_ANS_REG_DL_OFFLINE_GIFT {
  /*<thisrel this+0x0>*/ /*|0x3|*/ struct PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iINDEX;
  /*<thisrel this+0x8>*/ /*|0xb|*/ char szUID[11];
  /*<thisrel this+0x14>*/ /*|0x2|*/ unsigned short wServerCode;
  /*<thisrel this+0x16>*/ /*|0xb|*/ char szNAME[11];
  /*<thisrel this+0x24>*/ /*|0x4|*/ int iResultCode;
  /*<thisrel this+0x28>*/ /*|0x32|*/ char szGIFT_NAME[50];
};
// <size 0x5c>

struct PMSG_ANS_REG_HT_OFFLINE_GIFT {
  /*<thisrel this+0x0>*/ /*|0x3|*/ struct PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iINDEX;
  /*<thisrel this+0x8>*/ /*|0xb|*/ char szUID[11];
  /*<thisrel this+0x14>*/ /*|0x2|*/ unsigned short wServerCode;
  /*<thisrel this+0x16>*/ /*|0xb|*/ char szNAME[11];
  /*<thisrel this+0x24>*/ /*|0x4|*/ int iResultCode;
  /*<thisrel this+0x28>*/ /*|0x32|*/ char szGIFT_NAME[50];
};
// <size 0x5c>

struct SDHP_EXPANDEDWAREHOUSE_SET
{
	PBMSG_HEAD h;
	char AccountID[11];
	BYTE ExpansionType;
	BYTE ExpansionLevel;
};

struct PMSG_ANS_SANTACHECK
{
	PBMSG_HEAD2 h;
	char AccountID[11];
	short gGameServerCode;
	int aIndex;
	WORD Result;
	WORD UseCount;
};

struct PMSG_REQ_SANTACHECK
{
	PBMSG_HEAD2 h;
	char AccountID[11];
	short gGameServerCode;
	int aIndex;
};

struct PMSG_REQ_SANTAGIFT
{
	PBMSG_HEAD2 h;
	char AccountID[11];
	short gGameServerCode;
	int aIndex;
};

struct PMSG_ANS_SANTAGIFT
{
	PBMSG_HEAD2 h;
	char AccountID[11];
	short gGameServerCode;
	int aIndex;
	WORD Result;
	WORD UseCount;
};

struct BAN_REQ_USER
{
	PBMSG_HEAD		h;
	BYTE			Type;
	BYTE			Ban;
	char			AccName[11];
};

struct PMSG_ANS_WARESAVE
{
	PBMSG_HEAD		h;
	int				iIndex;
	BYTE			Result;
	BYTE			CloseWindow;
};

struct PMSG_REQ_PENTAGRAMJEWEL
{
	PBMSG_HEAD h;
	int iUserIndex;
	BYTE btJewelPos;
	int iUserGuid;
	char szAccountID[MAX_ACCOUNT_LEN + 1];
	char szName[MAX_ACCOUNT_LEN + 1];
};

#pragma pack (1)
struct PMSG_ANS_PENTAGRAMJEWEL
{
	PWMSG_HEAD h;
	BYTE btJewelCnt;
	int iUserIndex;
	int iAnsType;
};

struct PMSG_REQ_SETPENTAGRAMJEWEL
{
	PWMSG_HEAD h; // 4
	int iUserIndex; // 4
	int iUserGuid; // 4
	char szAccountID[MAX_ACCOUNT_LEN + 1]; // 11
	char szName[MAX_ACCOUNT_LEN + 1]; // 11
	BYTE btJewelPos; // 1
	BYTE btJewelCnt; // 1
};
#pragma pack ()

struct PMSG_DEL_PENTAGRAMJEWEL
{
	PBMSG_HEAD h;
	int iUserGuid;
	char szAccountID[MAX_ACCOUNT_LEN + 1];
	char szName[MAX_ACCOUNT_LEN + 1];
	BYTE btJewelPos;
	BYTE btJewelIndex;
};

struct PMSG_INSERT_PENTAGRAMJEWEL
{
	PBMSG_HEAD h;
	int iUserGuid;
	char szAccountID[MAX_ACCOUNT_LEN + 1];
	char szName[MAX_ACCOUNT_LEN + 1];
	BYTE btJewelPos;
	BYTE btJewelIndex;
	BYTE btMainAttribute;
	BYTE btItemType;
	WORD wItemIndex;
	BYTE btLevel;
	BYTE btRank1OptionNum;
	BYTE btRank1Level;
	BYTE btRank2OptionNum;
	BYTE btRank2Level;
	BYTE btRank3OptionNum;
	BYTE btRank3Level;
	BYTE btRank4OptionNum;
	BYTE btRank4Level;
	BYTE btRank5OptionNum;
	BYTE btRank5Level;
};

#pragma pack (1)
struct PENTAGRAMJEWEL_INFO
{
	BYTE btJewelPos;
	BYTE btJewelIndex;
	BYTE btMainAttribute;
	BYTE btItemType;
	WORD wItemIndex;
	BYTE btLevel;
	BYTE btRank1OptionNum;
	BYTE btRank1Level;
	BYTE btRank2OptionNum;
	BYTE btRank2Level;
	BYTE btRank3OptionNum;
	BYTE btRank3Level;
	BYTE btRank4OptionNum;
	BYTE btRank4Level;
	BYTE btRank5OptionNum;
	BYTE btRank5Level;

	void Clear()
	{
		this->btJewelPos = -1;
		this->btJewelIndex = -1;
		this->btMainAttribute = -1;
		this->btItemType = -1;
		this->wItemIndex = -1;
		this->btLevel = 0;
		this->btRank1OptionNum = -1;
		this->btRank1Level = -1;
		this->btRank2OptionNum = -1;
		this->btRank2Level = -1;
		this->btRank3OptionNum = -1;
		this->btRank3Level = -1;
		this->btRank4OptionNum = -1;
		this->btRank4Level = -1;
		this->btRank5OptionNum = -1;
		this->btRank5Level = -1;
	}
};
#pragma pack ()


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

typedef struct
{
	PBMSG_HEAD h;
	char szCharName[MAX_ACCOUNT_LEN+1];
	short sExGameServerCode;
}SDHP_REQ_SET_EXGAMESERVERCODE,*LPSDHP_REQ_SET_EXGAMESERVERCODE;

struct DS_GET_MONSTERCNT
{
	PBMSG_HEAD2		h;
	char character[11];
	int cnt;
	short aIndex;
};

struct DS_SAVE_MONSTERCNT
{
	PBMSG_HEAD2		h;
	char character[11];
	int type;
	int cnt;
};


enum PERIODITEM_TYPE
{
	PERIODITEM_BUFF = 1,
	PERIODITEM_NORMAL = 2
};

struct PMSG_ANS_PERIODITEMEX_INSERT
{
	PBMSG_HEAD2 head;
	DWORD dwUserGuid;
	WORD wUserIndex;
	BYTE btResultCode;
	BYTE btItemtype;
	WORD wItemCode;
	BYTE btEffectCategory;
	BYTE btEffectType1;
	BYTE btEffectType2;
	UINT64 Serial;
	DWORD dwDuration;
	time_t lBuyDate;
	time_t lExpireDate;
};

struct PMSG_REQ_PERIODITEMEX_INSERT
{
	PBMSG_HEAD2 head;
	DWORD dwUserGuid;
	WORD wUserIndex;
	char chCharacterName[MAX_ACCOUNT_LEN+1];
	BYTE btItemType;
	WORD wItemCode;
	BYTE btEffectCategory;
	BYTE btEffectType1;
	BYTE btEffectType2;
	UINT64 Serial;
	DWORD dwDuration;
	time_t lBuyDate;
	time_t lExpireDate;
};

struct PMSG_ANS_PERIODITEMEX_DELETE
{
	PBMSG_HEAD2 head;
	DWORD dwUserGuid;
	WORD wUserIndex;
	BYTE btResultCode;
};

struct PMSG_REQ_PERIODITEMEX_DELETE
{
	PBMSG_HEAD2 head;
	DWORD dwUserGuid;
	WORD wUserIndex;
	UINT64 Serial;
	WORD wItemCode;
	BYTE btItemType;
	char chCharacterName[MAX_ACCOUNT_LEN+1];
};

struct ITEMPERIOD_DATEINFO
{
	BYTE btUsedInfo;
	BYTE btItemType;
	WORD wItemCode;
	BYTE btEffectCategory;
	BYTE btEffectType1;
	BYTE btEffectType2;
	UINT64 Serial;
	time_t lItemBuyDate;
	time_t lItemExpireDate;
};

typedef struct
{
	DWORD dwUserGuid;
	BYTE btItemCount;
	char chCharacterName[MAX_ACCOUNT_LEN+1];
	ITEMPERIOD_DATEINFO ItemData[100];
}PERIOD_ITEMLIST, *LPPERIOD_ITEMLIST;

struct PMSG_ANS_PERIODITEMEX_LIST
{
	PBMSG_HEAD2 head;
	DWORD dwUserGuid;
	WORD wUserIndex;
	BYTE btUsedInfo;
	BYTE btItemType;
	UINT64 Serial;
	time_t lItemBuyDate;
	time_t lItemExpireDate;
	WORD wItemCode;
	BYTE btEffectCategory;
	BYTE btEffectType1;
	BYTE btEffectType2;
};

struct PMSG_REQ_PERIODITEMEX_SELECT
{
	PBMSG_HEAD2 head;
	DWORD dwUserGuid;
	WORD wUserIndex;
	char chCharacterName[MAX_ACCOUNT_LEN+1];
};

struct PMSG_ANS_PERIODITEMEX_EXPIRED_ITEMLIST
{
	PWMSG_HEAD2 head;
	WORD wUserIndex;
	BYTE btExpiredItemCount;
	WORD wItemCode[90];
	UINT64 Serial[90];
};

struct PMSG_ANS_PERIODITEMEX_LIST_COUNT
{
	PBMSG_HEAD2 head;
	WORD wUserIndex;
	BYTE btResult;
	BYTE btListCount;
	DWORD dwUserGuid;
};

struct PERIOD_EXPIRED_ITEM
{
	WORD wItemCode;
	UINT64 Serial;
};

typedef struct
{
	WORD wItemCode;
	UINT64 Serial;
	WORD wDurabilitySmall;
}PMSG_LUCKYITME_DB_INFO, *LPPMSG_LUCKYITME_DB_INFO;

struct PMSG_REQ_LUCKYITEM_INSERT
{
	PBMSG_HEAD2 head;
	DWORD dwUserGuid;
	char szCharName[MAX_ACCOUNT_LEN + 1];
	PMSG_LUCKYITME_DB_INFO LuckyItemDBInfo;
};

struct PMSG_REQ_LUCKYITEM_INSERT_2ND
{
	PBMSG_HEAD2 head;
	DWORD dwUserGuid;
	BYTE btItemCnt;
	char szCharName[MAX_ACCOUNT_LEN + 1];
	PMSG_LUCKYITME_DB_INFO LuckyItemDBInfo[5];
};

struct PMSG_REQ_LUCKYITEM_DELETE
{
	PBMSG_HEAD2 head;
	WORD wUserIndex;
	char szCharName[MAX_ACCOUNT_LEN + 1];
	WORD wItemCode;
	UINT64 Serial;
};

struct PMSG_ANS_LUCKYITEM_SELECT
{
	PWMSG_HEAD head;
	WORD wUserIndex;
	BYTE btResultCode;
	BYTE btItemCnt;
};

struct PMSG_REQ_LUCKYITEM_SELECT
{
	PBMSG_HEAD2 head;
	WORD wUserIndex;
	char chCharacterName[MAX_ACCOUNT_LEN + 1];
};

struct DS_SAVE_PLAYERKILLER
{
	PBMSG_HEAD2		h;
	char Victim[11];
	char Killer[11];
};

typedef struct
{
	BYTE btItemCount;
	PERIOD_EXPIRED_ITEM ExpiredItem[90];
}PERIOD_EXPIRED_ITEMLIST, *LPPERIOD_EXPIRED_ITEMLIST;

struct _tagPMSG_REQ_ARCA_BATTLE_GUILD_JOIN_DS
{
	PBMSG_HEAD2 h;
	char szGuildMaster[11];
	char szGuildName[9];
	DWORD dwGuild;
	WORD wNumber;
};

struct _tagPMSG_ANS_ARCA_BATTLE_GUILD_JOIN_DS
{
	PBMSG_HEAD2 h;
	char btResult;
	WORD wNumber;
};

struct _tagPMSG_REQ_ARCA_BATTLE_GUILD_MEMBER_JOIN_DS
{
	PBMSG_HEAD2 h;
	char szCharName[11];
	char szGuildName[9];
	unsigned int dwGuild;
	WORD wNumber;
};

struct _tagPMSG_ANS_ARCA_BATTLE_GUILD_MEMBER_JOIN_DS
{
	PBMSG_HEAD2 h;
	char btResult;
	WORD wNumber;
};

struct _tagPMSG_REQ_ARCA_BATTLE_ENTER_DS
{
	PBMSG_HEAD2 h;
	char szCharName[11];
	BYTE btEnterSeq;
	WORD wNumber;
};

struct _tagPMSG_ANS_ARCA_BATTLE_ENTER_DS
{
	PBMSG_HEAD2 h;
	char btResult;
	BYTE btEnterSeq;
	WORD wNumber;
};

struct _stABWinGuildInfoDS
{
	char szGuildName[9];
	DWORD dwGuild;
	WORD wOccupyObelisk;
	WORD wObeliskGroup;
};

struct _tagPMSG_REQ_AB_WIN_GUILD_INFO_INSERT_DS
{
	PBMSG_HEAD2 h;
	char btGuildCnt;
	unsigned __int16 wMapSvrNum;
	_stABWinGuildInfoDS m_stABWinGuildInfoDS[5];
};

struct _tagPMSG_REQ_AB_WIN_GUILD_INFO_DS
{
	PBMSG_HEAD2 h;
	WORD wNumber;
};

struct _tagPMSG_ANS_AB_WIN_GUILD_INFO_DS
{
	PBMSG_HEAD2 h;
	char btGuildCnt;
	_stABWinGuildInfoDS m_stABWinGuildInfoDS[5];
};

struct _tagPMSG_REQ_AB_PROC_INSERT_DS
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;
	BYTE btProcState;
};

struct _tagPMSG_REQ_AB_PROC_STATE_DS
{
	PBMSG_HEAD2 h;
};

struct _tagPMSG_ANS_AB_PROC_STATE_DS
{
	PBMSG_HEAD2 h;
	BYTE btProcState;
};

struct _tagPMSG_REQ_AB_JOIN_MEMBER_UNDER_DS
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;
};

struct _stGuildUnderMember
{
	BYTE btGuildMemberCnt;
	char szGuildNames[9];
};

struct _tagPMSG_ANS_AB_JOIN_MEMBER_UNDER_DS
{
	PBMSG_HEAD2 h;
	BYTE btGuildCnt;
	_stGuildUnderMember GuildMemberCnt[6];
};

struct _tagPMSG_REQ_AB_JOIN_CANCEL_DS
{
	PBMSG_HEAD2 h;
	BYTE btMinGuildMemNum;
	WORD wMapSvrNum;
};

struct _stCancelGuildNames
{
	char szGuildNames[9];
};

struct _tagPMSG_ANS_AB_JOIN_CANCEL_DS
{
	PBMSG_HEAD2 h;
	BYTE btGuildCnt;
	_stCancelGuildNames CancelGuildNames[6];
};

struct _tagPMSG_REQ_AB_REG_MEMBER_CNT_DS
{
	PBMSG_HEAD2 h;
	int iIndex;
	int iGuildNumber;
};

struct _tagPMSG_ANS_AB_REG_MEMBER_CNT_DS
{
	PBMSG_HEAD2 h;
	WORD wNumber;
	BYTE btRegMemCnt;
};

struct _tagPMSG_REQ_REMOVE_ALL_GUILD_BUFF_DS
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;
};

struct _tagPMSG_ANS_REMOVE_ALL_GUILD_BUFF_DS
{
	PBMSG_HEAD2 h;
};

struct _tagPMSG_REQ_ARCA_BATTLE_MARK_CNT_DS
{
	PBMSG_HEAD2 h;
	WORD wNumber;
	DWORD dwGuildNum;
};

struct _tagPMSG_ANS_ARCA_BATTLE_MARK_CNT_DS
{
	PBMSG_HEAD2 h;
	BYTE btResult;
	WORD wNumber;
	DWORD dwMarkCnt;
};

struct _tagPMSG_REQ_ARCA_BATTLE_MARK_REG_DS
{
	PBMSG_HEAD2 h;
	WORD wNumber;
	char szGuildMaster[MAX_ACCOUNT_LEN+1];
	char szGuildName[MAX_GUILD_LEN+1];
	DWORD dwMarkCnt;
	DWORD dwGuildNum;
};

struct _tagPMSG_ANS_ARCA_BATTLE_MARK_REG_DS
{
	PBMSG_HEAD2 h;
	WORD wNumber;
};

struct _tagPMSG_REQ_ARCA_BATTLE_MARK_RANK_DS
{
	PBMSG_HEAD2 h;
	WORD wNumber;
	DWORD dwGuildNum;
};

struct _stArcaBattleMarkTopRankDS
{
	BYTE btRank;
	char szGuildNames[MAX_GUILD_LEN+1];
	DWORD dwMarkCnt;
};

struct _tagPMSG_ANS_ARCA_BATTLE_MARK_RANK_DS
{
	PBMSG_HEAD2 h;
	WORD wNumber;
	BYTE btRank;
	DWORD dwMarkCnt;
	BYTE btGuildCnt;
	_stArcaBattleMarkTopRankDS ArcaBattleMarkTopRank[6];
};

struct _tagPMSG_REQ_ARCA_BATTLE_MARK_REG_DEL_DS
{
	PBMSG_HEAD2 h;
	WORD wNumber;
	DWORD dwGuildNum;
};

struct _tagPMSG_REQ_ARCA_BATTLE_IS_TOP_RANK
{
	PBMSG_HEAD2 h;
	WORD wNumber;
	DWORD dwGuildNum;
};

struct _tagPMSG_REQ_AB_MARK_REG_ALL_DEL_DS
{
	PBMSG_HEAD2 h;
};

struct _tagPMSG_REQ_ALL_GUILD_MARK_CNT_DS
{
	PBMSG_HEAD2 h;
};

struct _stABAllGuildMark
{
	char szGuildName[MAX_GUILD_LEN+1];
	DWORD dwMarkCnt;
};

struct _tagPMSG_ANS_ALL_GUILD_MARK_CNT_DS
{
	PWMSG_HEAD2 h;
	WORD wGuildCnt;
	_stABAllGuildMark ABAllGuildMark[250];
};

struct _tagPMSG_REQ_AB_MARK_REG_UPDATE_DS
{
	PBMSG_HEAD2 h;
	DWORD dwGuildNum;
	DWORD dwMarkCnt;
};

struct _tagPMSG_REQ_GUILD_REG_INIT
{
	PBMSG_HEAD2 h;
};

struct _tagPMSG_REQ_AB_ALL_JOIN_USER_DS
{
	PBMSG_HEAD h;
};

struct _stABJoinUserInfoDS
{
	char szGuildName[MAX_GUILD_LEN+1];
	DWORD dwGuild;
	char szUserName[MAX_ACCOUNT_LEN+1];
};

struct _tagPMSG_ANS_AB_ALL_JOIN_USER_DS
{
	PWMSG_HEAD h;
	BYTE btUserCnt;
	_stABJoinUserInfoDS stABJoinUserInfo[200];
};

struct PMSG_REQ_PERIODBUFF_DELETE
{
	PBMSG_HEAD2 head;
	WORD wUserIndex;
	WORD wBuffIndex;
	char szCharacterName[11];
};

struct PMSG_REQ_PERIODBUFF_INSERT
{
	PBMSG_HEAD2 head;
	WORD wUserIndex;
	char szCharacterName[MAX_ACCOUNT_LEN+1];
	WORD wBuffIndex;
	char btEffectType1;
	char btEffectType2;
	DWORD dwDuration;
	time_t lExpireDate;
};

struct PMSG_REQ_PERIODBUFF_SELECT
{
	PBMSG_HEAD2 head;
	WORD wUserIndex;
	char szCharacterName[MAX_ACCOUNT_LEN+1];
};

struct PMSG_ANS_PERIODBUFF_SELECT
{
	PBMSG_HEAD2 h;
	WORD wUserIndex;
	char szCharacterName[MAX_ACCOUNT_LEN+1];
	BYTE btResultCode;
	WORD wBuffIndex;
	BYTE btEffectType1;
	BYTE btEffectType2;
	time_t lExpireDate;
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

typedef struct
{
	PBMSG_HEAD h;
	int iUserIndex;
	char szCharName[MAX_ACCOUNT_LEN + 1];
}PMSG_REQ_QUESTEXP_INFO, *LPPMSG_REQ_QUESTEXP_INFO;

typedef struct
{
	PWMSG_HEAD h;
	BYTE btQuestCnt;
	char szCharName[MAX_ACCOUNT_LEN + 1];
}_PMSG_QUESTEXP_INFO, *LP_PMSG_QUESTEXP_INFO;

typedef struct _QUESTEXP_INFO
{
	_QUESTEXP_INFO::_QUESTEXP_INFO()
	{
		this->dwQuestIndexID = 0;
		this->wProgState = 0;
		this->lStartDate = 0;
		this->lEndDate = 0;

		memset(this->btAskIndex, 0, 5);
		memset(this->btAskValue, 0, 5);
		memset(this->btAskState, 0, 5);
	}

	DWORD dwQuestIndexID;
	WORD wProgState;
	BYTE btAskIndex[5];
	BYTE btAskValue[5];
	BYTE btAskState[5];
	time_t lStartDate;
	time_t lEndDate;
}_QUESTEXP_INFO, *LP_QUESTEXP_INFO;

typedef struct
{
	PWMSG_HEAD head;
	BYTE btQuestCnt;
	int iUserIndex;
}PMSG_ANS_QUESTEXP_INFO, *LPPMSG_ANS_QUESTEXP_INFO;

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

struct _stCCFRankingInfo
{
	_stCCFRankingInfo()
	{
		memset(this->szCharName, 0x00, sizeof(this->szCharName));
		this->byRank = 0;
		this->nPoint = 0;
	}

	char szCharName[MAX_ACCOUNT_LEN + 1];
	BYTE byRank;
	int nPoint;
};

struct _tagPMSG_REQ_CCF_PERMISSION
{
	PBMSG_HEAD2 h;
	char szCharName[MAX_ACCOUNT_LEN + 1];
	BYTE byCCFType;
	int nIndex;
};

struct SDHP_REQ_CCF_RANKING
{
	PBMSG_HEAD2 h;
	BYTE byRankingType;
	int nServerCategory;
};

struct SDHP_RENEW_RANKING
{
	PBMSG_HEAD2 h;
	BYTE byRankingType;
};

struct _tagPMSG_REQ_SAVE_CCF_RESULT
{
	PBMSG_HEAD2 h;
	char szCharName[MAX_ACCOUNT_LEN + 1];
	int nPoint;
	int nCharClass;
	int nCharLevel;
	UINT64 nCharExp;
	BYTE byCCFType;
};

struct _tagPMSG_ANS_CCF_RANK
{
	PWMSG_HEAD h;
	BYTE byUserCnt;
	_stCCFRankingInfo RankingInfo[50];
};

struct PMSG_SEND_CCF_INFO_ALL_SVR
{
	PBMSG_HEAD2 h;
	int nMin;
	int nType;
	WORD wMapSvrNum;
};

struct SDHP_REQ_DBMUUN_INVEN_LOAD
{
	PBMSG_HEAD h;
	char AccountID[11];
	char Name[11];
	WORD aIndex;
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

struct _tagMuRummyCardUpdateDS
{
	_tagMuRummyCardUpdateDS()
	{
		this->btSlotNum = -1;
		this->btSeq = -1;
		this->btStatus = -1;
	}

	BYTE btSlotNum;
	BYTE btSeq;
	BYTE btStatus;
};

struct _tagPMSG_REQ_MURUMMY_SELECT_DS
{
	PBMSG_HEAD2 h;
	char AccountID[11];
	char Name[11];
	WORD aIndex;
};

struct _tagMuRummyCardInfoDS
{
	_tagMuRummyCardInfoDS()
	{
		this->btColor = -1;
		this->btNumber = -1;
		this->btSlotNum = -1;
		this->btSeq = -1;
		this->btStatus = -1;
	}

	BYTE btColor;
	BYTE btNumber;
	BYTE btSlotNum;
	BYTE btSeq;
	BYTE btStatus;
};

struct _tagPMSG_ANS_MURUMMY_SELECT_DS
{
	PBMSG_HEAD2 h;
	WORD aIndex;
	WORD wScore;
	BYTE btResult;
	_tagMuRummyCardInfoDS stMuRummyCardInfoDS[24];
};

struct _tagPMSG_REQ_MURUMMY_INSERT_DS
{
	PBMSG_HEAD2 h;
	char AccountID[11];
	char Name[11];
	WORD aIndex;
	_tagMuRummyCardInfoDS stMuRummyCardInfoDS[24];
};

struct _tagPMSG_REQ_MURUMMY_SCORE_UPDATE_DS
{
	PBMSG_HEAD2 h;
	char AccountID[11];
	char Name[11];
	WORD wScore;
	_tagMuRummyCardUpdateDS stCardUpdateDS[3];
};

struct _tagPMSG_REQ_MURUMMY_UPDATE_DS
{
	PBMSG_HEAD2 h;
	char AccountID[11];
	char Name[11];
	BYTE btSlotNum;
	BYTE btStatus;
	BYTE btSequence;
};

struct _tagPMSG_REQ_MURUMMY_DELETE_DS
{
	PBMSG_HEAD2 h;
	char AccountID[11];
	char Name[11];
};

struct _tagPMSG_REQ_MURUMMY_SLOTUPDATE_DS
{
	PBMSG_HEAD2 h;
	char AccountID[11];
	char Name[11];
	_tagMuRummyCardUpdateDS stCardUpdateDS;
};

struct _tagPMSG_REQ_MURUMMY_INFO_UPDATE_DS
{
	PBMSG_HEAD2 h;
	char AccountID[11];
	char Name[11];
	WORD wScore;
	_tagMuRummyCardUpdateDS stMuRummyCardUpdateDS[24];
};

struct _tagPMSG_REQ_MURUMMY_LOG_INSERT_DS
{
	PBMSG_HEAD2 h;
	char AccountID[11];
	char Name[11];
	WORD wScore;
};

struct SDHP_REQ_LOAD_MINESYSTEM_UPT_USERINFO
{
	PBMSG_HEAD2 h;
	WORD wUserIndex;
	char szCharName[MAX_ACCOUNT_LEN + 1];
};

struct SDHP_REQ_MINESYSTEM_UPT_USERINFO
{
	PBMSG_HEAD2 h;
	WORD wUserIndex;
	char szCharName[MAX_ACCOUNT_LEN + 1];
	WORD wTwinkleType;
	int iCurrentStage;
	BYTE byRequestType;
};

struct SDHP_ANS_MINESYSTEM_UPT_USERINFO
{
	PBMSG_HEAD2 h;
	WORD wUserIndex;
	BYTE byRequestType;
	BYTE Result;
};

struct SDHP_ANS_LOAD_MINESYSTEM_UPT_USERINFO
{
	PBMSG_HEAD2 h;
	WORD wUserIndex;
	BYTE byResult;
	char szCharName[MAX_ACCOUNT_LEN + 1];
	WORD wTwinkleType;
	int iCurrentStage;
};

struct PSHOP_ITEMVALUE_INFO_DS
{
	int nPShopItemInvenNum;
	UINT64 ItemSerial;
	int nMoney;
	WORD sBlessJewelValue;
	WORD sSoulJewelValue;
	WORD sChaosJewelValue;
};

struct PMSG_ANS_PSHOPITEMVALUE_INFO
{
	PWMSG_HEAD h;
	BYTE btItemCnt;
	int iUserIndex;
	PSHOP_ITEMVALUE_INFO_DS PShopItemValueInfo[32];
};

struct PMSG_UPDATE_PSHOPITEMVALUE_INFO
{
	PWMSG_HEAD h;
	BYTE btItemCnt;
	char AccountId[MAX_ACCOUNT_LEN + 1];
	char szName[MAX_ACCOUNT_LEN + 1];
	PSHOP_ITEMVALUE_INFO_DS PShopItemValueInfo[32];
};

struct PMSG_REQ_PSHOPITEMVALUE_INFO
{
	PBMSG_HEAD h;
	char AccountId[MAX_ACCOUNT_LEN + 1];
	char szName[MAX_ACCOUNT_LEN + 1];
	int iUserIndex;
};

struct PMSG_DEL_PSHOPITEM
{
	PBMSG_HEAD h;
	char AccountId[MAX_ACCOUNT_LEN + 1];
	char szName[MAX_ACCOUNT_LEN + 1];
	int nPShopItemInvenNum;
};

struct PMSG_MOVE_PSHOPITEM
{
	PBMSG_HEAD h;
	char AccountId[MAX_ACCOUNT_LEN + 1];
	char szName[MAX_ACCOUNT_LEN + 1];
	int nOldPShopItemInvenNum;
	int nNewPShopItemInvenNum;
};

struct _tagPMSG_REQ_AE_PLAY_DS
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;
	BYTE btPlay;
};

struct _tagPMSG_ANS_AE_PLAY_DS
{
	PBMSG_HEAD2 h;
	BYTE btPlay;
};

struct PMSG_REQ_CLASSDEF
{
	PBMSG_HEAD h;
};

struct CLASSDEF_DATA
{
	int Class;
	int Str;
	int Dex;
	int Vit;
	int Energy;
	float Life;
	float Mana;
	float LevelLife;
	float LevelMana;
	float VitalityToLife;
	float EnergyToMana;
	int Leadership;
};

struct PMSG_ANS_CLASSDEF
{
	PWMSG_HEAD h;
	int iCount;
	CLASSDEF_DATA m_ClassData[MAX_TYPE_PLAYER];
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

struct USERWAREHOUSE_DATA
{
	char szAccountID[MAX_ACCOUNT_LEN+1];
	int WarehouseID;
	DWORD LastChangeTick;
	int ChangeIDEnableState;
	bool WarehouseOpenState;
};

// Cancel Item Sale

struct SDHP_REQ_SHOP_REBUY_LIST
{
	PBMSG_HEAD2 h;
	int iIndex;
	char szAccountID[MAX_ACCOUNT_LEN+1];
	char szName[MAX_ACCOUNT_LEN+1];
	time_t CurrTime;
};

struct SDHP_SHOP_REBUY_ITEM
{
	BYTE btItemNumber;
	WORD wItemCount;
	time_t SellDate;
	time_t SellExpireDate;
	DWORD dwSellPrice;
	WORD wItemCode;
	BYTE btItemLevel;
	BYTE btItemOp1;
	BYTE btItemOp2;
	BYTE btItemOp3;
	BYTE btItemDur;
	BYTE btItemExcOp;
	BYTE btItemSetOp;
	BYTE btItemHarmonyOp;
	BYTE btItemOpEx;
	BYTE btItemSocketOp[5];
	BYTE btItemBonusSocketOp;
	UINT64 ItemSerial;
}; 

struct SDHP_ANS_SHOP_REBUY_LIST
{
	PWMSG_HEAD2 h;
	int iIndex;
	char szAccountID[MAX_ACCOUNT_LEN+1];
	char szName[MAX_ACCOUNT_LEN+1];
	BYTE btResult;
	int iCount;
};

struct SDHP_REQ_SHOP_REBUY_ADD_ITEM
{
	PBMSG_HEAD2 h;
	char szAccountID[MAX_ACCOUNT_LEN+1];
	char szName[MAX_ACCOUNT_LEN+1];
	time_t SellDate;
	time_t SellExpireDate;
	DWORD dwSellPrice;
	WORD wItemCode;
	BYTE btItemLevel;
	BYTE btItemOp1;
	BYTE btItemOp2;
	BYTE btItemOp3;
	BYTE btItemDur;
	BYTE btItemExcOp;
	BYTE btItemSetOp;
	BYTE btItemHarmonyOp;
	BYTE btItemOpEx;
	BYTE btItemSocketOp[5];
	BYTE btItemBonusSocketOp;
	UINT64 ItemSerial;
};

struct SDHP_REQ_SHOP_REBUY_GET_ITEM
{
	PBMSG_HEAD2 h;
	int iIndex;
	char szAccountID[MAX_ACCOUNT_LEN+1];
	char szName[MAX_ACCOUNT_LEN+1];
	DWORD dwItemPrice;
	WORD wItemCode;
	BYTE btItemNumber;
};

struct SDHP_ANS_SHOP_REBUY_GET_ITEM
{
	PBMSG_HEAD2 h;
	int iIndex;
	char szAccountID[MAX_ACCOUNT_LEN+1];
	char szName[MAX_ACCOUNT_LEN+1];
	BYTE btResult;
	BYTE btItemNumber;
	WORD wItemCount;
	DWORD dwSellPrice;
	WORD wItemCode;
	BYTE btItemLevel;
	BYTE btItemOp1;
	BYTE btItemOp2;
	BYTE btItemOp3;
	BYTE btItemDur;
	BYTE btItemExcOp;
	BYTE btItemSetOp;
	BYTE btItemHarmonyOp;
	BYTE btItemOpEx;
	BYTE btItemSocketOp[5];
	BYTE btItemBonusSocketOp;
	UINT64 ItemSerial;
};

struct SDHP_REQ_SHOP_REBUY_DELETE_ITEM
{
	PBMSG_HEAD2 h;
	char szAccountID[MAX_ACCOUNT_LEN+1];
	char szName[MAX_ACCOUNT_LEN+1];
	DWORD dwItemPrice;
	WORD wItemCode;
	BYTE btItemNumber;
};

// Gremory Case

struct _stReqGremoryCaseItemList
{
	PBMSG_HEAD2 h;
	int iIndex;
	char szAccountID[MAX_ACCOUNT_LEN+1];
	char szName[MAX_ACCOUNT_LEN+1];
};

struct _stGremoryCaseItem
{
	BYTE btStorageType;
	BYTE btRewardSource;
	BYTE btItemGUID;
	WORD wItemID;
	BYTE btItemLevel;
	BYTE btItemDurability;
	BYTE btItemSkill;
	BYTE btItemLuck;
	BYTE btItemOption;
	BYTE btItemExcOption;
	BYTE btItemSetOption;
	BYTE btItemSocketCount;
	BYTE btItemMainAttribute;
	WORD wMuunEvoItemType;
	BYTE btMuunEvoItemIndex;
	DWORD dwAuthCode;
	time_t iReceiveDate;
	time_t iExpireDate;
};

struct _stAnsGremoryCaseItemList
{
	PWMSG_HEAD2 h;
	int iIndex;
	char szAccountID[MAX_ACCOUNT_LEN+1];
	char szName[MAX_ACCOUNT_LEN+1];
	BYTE btResult;
	BYTE btCount;
};

struct _stReqAddItemToGremoryCase
{
	PBMSG_HEAD2 h;
	int iIndex;
	char szAccountID[MAX_ACCOUNT_LEN+1];
	char szName[MAX_ACCOUNT_LEN+1];
	_stGremoryCaseItem m_GremoryCaseItem;
};

struct _stAnsAddItemToGremoryCase
{
	PBMSG_HEAD2 h;
	int iIndex;
	char szAccountID[MAX_ACCOUNT_LEN+1];
	char szName[MAX_ACCOUNT_LEN+1];
	BYTE btResult;
	_stGremoryCaseItem m_GremoryCaseItem;
};

struct _stReqCheckUseItemGremoryCase
{
	PBMSG_HEAD2 h;
	int iIndex;
	char szAccountID[MAX_ACCOUNT_LEN+1];
	char szName[MAX_ACCOUNT_LEN+1];
	WORD wItemID;
	DWORD dwItemGUID;
	DWORD dwAuthCode;
};

struct _stAnsCheckUseItemGremoryCase
{
	PBMSG_HEAD2 h;
	int iIndex;
	char szAccountID[MAX_ACCOUNT_LEN+1];
	char szName[MAX_ACCOUNT_LEN+1];
	BYTE btResult;
	WORD wItemID;
	DWORD dwItemGUID;
	DWORD dwAuthCode;
};

struct _stReqDeleteItemFromGremoryCase
{
	PBMSG_HEAD2 h;
	char szAccountID[MAX_ACCOUNT_LEN+1];
	char szName[MAX_ACCOUNT_LEN+1];
	WORD wItemID;
	DWORD dwItemGUID;
	DWORD dwAuthCode;
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

struct SDHP_REQ_SET_CCF_WINNER_INFO
{
	PBMSG_HEAD2 h;
	char UBFName[MAX_ACCOUNT_LEN + 1];
	BYTE btCCFType;
	BYTE btRewardType;
};

struct SDHP_REQ_SET_CC_WINNER_INFO_UBF
{
	PBMSG_HEAD2 h;
	char UBFName[MAX_ACCOUNT_LEN + 1];
	BYTE btRewardType;
};

// DSF

struct PMSG_REQ_DSF_CAN_PARTY_ENTER
{
	PBMSG_HEAD2 h;
	int iUserIndex;
	char szAccountID1[MAX_ACCOUNT_LEN+1];
	char szUserName1[MAX_ACCOUNT_LEN+1];
	char szAccountID2[MAX_ACCOUNT_LEN+1];
	char szUserName2[MAX_ACCOUNT_LEN+1];
	BYTE btDSFType;
	int iEnterYear;
	BYTE btEnterMonth;
	BYTE btEnterDay;
};

struct PMSG_REQ_SAVE_DSF_PARTYPOINT
{
	PBMSG_HEAD2 h;
	char szAccountID1[MAX_ACCOUNT_LEN+1];
	char szUserName1[MAX_ACCOUNT_LEN+1];
	int nUserLevel1;
	char szAccountID2[MAX_ACCOUNT_LEN+1];
	char szUserName2[MAX_ACCOUNT_LEN+1];
	int nUserLevel2;
	int nDSFType;
	int nPoint;
	BYTE btEnterCnt;
	int nEnterYear;
	BYTE btEnterMonth;
	BYTE btEnterDay;
};

struct PMSG_REQ_DSF_PARTYRANKRENEW
{
	PBMSG_HEAD2 h;
	BYTE btDSFType;
	int nEnterYear;
	BYTE btEnterMonth;
	BYTE btEnterDay;
};

struct PMSG_REQ_DSF_GO_FINAL_PARTY
{
	PBMSG_HEAD2 h;
	BYTE btDSFType;
	int nEnterYear;
	BYTE btEnterMonth;
	BYTE btEnterDay;
};

struct PMSG_REQ_SAVE_DSF_REWARD_USER
{
	PBMSG_HEAD2 h;
	char szAccountID[MAX_ACCOUNT_LEN+1];
	char szUserName[MAX_ACCOUNT_LEN+1];
	int iClass;
	BYTE btDSFType;
	int nRewardYear;
	BYTE btRewardMonth;
	BYTE btRewardStartDay;
	BYTE btRewardEndDay;
};

struct PMSG_REQ_GET_DSF_REWARD
{
	PBMSG_HEAD2 h;
	char szAccountID[MAX_ACCOUNT_LEN+1];
	char szUserName[MAX_ACCOUNT_LEN+1];
	int nServerCode;
	int nUserIndex;
	int nRewardYear;
	BYTE btRewardMonth;
	BYTE btRewardDay;
};

struct PMSG_ANS_DSF_CAN_PARTY_ENTER
{
	PBMSG_HEAD2 h;
	int iUserIndex;
	BYTE btCount;
	BYTE btResult;
};

struct PMSG_ANS_DSF_PARTYRANKRENEW
{
	PBMSG_HEAD2 h;
	BYTE btResult;
};

struct PMSG_ANS_DSF_GO_FINAL_PARTY
{
	PWMSG_HEAD2 h;
	BYTE btPartyCnt;
	BYTE btDSFType;
};

struct DSF_GO_FINAL_PARTY
{
	char szUserName1[MAX_ACCOUNT_LEN+1];
	char szUserName2[MAX_ACCOUNT_LEN+1];
	WORD wServerCode1;
	WORD wServerCode2;
	BYTE btRank;
	BYTE btDSFType;
	int iPoint;
	int iEnterYear;
	BYTE btEnterMonth;
	BYTE btEnterDay;
};

struct PMSG_ANS_GET_DSF_REWARD
{
	PBMSG_HEAD2 h;
	int nUserIndex;
	BYTE btResult;
};

struct PMSG_REQ_SET_DSF_WINNER_INFO
{
	PBMSG_HEAD2 h;
	char UBFName[MAX_ACCOUNT_LEN + 1];
	BYTE btDSFType;
	BYTE btRewardType;
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
};

class CWarehouseUserData
{
public:

	CWarehouseUserData();
	~CWarehouseUserData();

	void Init();

	void AddUserData(char * szAccountID);
	void DelUserData(char * szAccountID);

	int SwitchWarehouse(char * szAccountID, int WarehouseID);
	void SetChangeEnableState(char * szAccountID, int State);
	void SetWarehouseOpenState(char * szAccountID, bool bState);

	int GetWarehouseID(char * szAccountID);
	bool GetWarehouseOpenState(char * szAccountID);

	void DGGetWarehouseList(int aIndex, SDHP_GETWAREHOUSEDB * aRecv);
	void GDSetWarehouseList(int aIndex, SDHP_GETWAREHOUSEDB_SAVE * aRecv);
	void GDSetWarehouseMoney(int aIndex, SDHP_WAREHOUSEMONEY_SAVE * aRecv);
	void GDReqSwitchWarehouse(int aIndex, PMSG_REQ_SWITCHWARE * aRecv);

private:
	std::vector<USERWAREHOUSE_DATA> m_vtWarehouseData;
	CRITICAL_SECTION m_WareDataCriti;
	CQuery m_WareDB;
};

#define MAX_CS_GUILDLIST 100
#define MAX_CS_NPC 200

class CDataServerProtocol
{
public:

	CDataServerProtocol();
	~CDataServerProtocol();
	BOOL Init();
	static void ProtocolCore(int aIndex, BYTE HeadCode, LPBYTE aRecv, int iSize);

private:

	void DataServerLogin(int aIndex, SDHP_SERVERINFO * lpMsg);
	void JGPGetCharList(int aIndex, SDHP_GETCHARLIST * aRecv);
	void JGCharacterCreateRequest(int aIndex, SDHP_CREATECHAR * aRecv);
	void JGCharDelRequest(int aIndex, SDHP_CHARDELETE * aRecv);
	void JGGetCharacterInfo(int aIndex, SDHP_DBCHARINFOREQUEST * aRecv);
	void GJSetCharacterInfo(int aIndex, SDHP_DBCHAR_INFOSAVE * aRecv);
	void GDUserItemSave(int aIndex, SDHP_DBCHAR_ITEMSAVE * aRecv);
	void ItemSerialCreateRecv(int aIndex, SDHP_ITEMCREATE * aRecv);
	void PetItemSerialCreateRecv(int aIndex, SDHP_ITEMCREATE * aRecv);
	void DGRecvPetItemInfo(int aIndex, SDHP_REQUEST_PETITEM_INFO * aRecv);
	void GDSavePetItemInfo(int aIndex, SDHP_SAVE_PETITEM_INFO * aRecv);
	void DGOptionDataRecv(int aIndex, SDHP_SKILLKEYDATA * aRecv);
	void DGMoveOtherServer(int aIndex, SDHP_CHARACTER_TRANSFER * aRecv);
	void GDDeleteTempUserInfo(int aIndex, SDHP_DELETE_TEMPUSERINFO * aRecv);
	void GS_DGAnsCastleInitData(int aIndex,CSP_REQ_CSINITDATA * aRecv);
	void GS_DGAnsOwnerGuildMaster(int aIndex,CSP_REQ_OWNERGUILDMASTER * aRecv);
	void GS_DGAnsCastleNpcBuy(int aIndex,CSP_REQ_NPCBUY * aRecv);
	void GS_DGAnsCastleNpcRepair(int aIndex,CSP_REQ_NPCREPAIR * aRecv);
	void GS_DGAnsCastleNpcUpgrade(int aIndex,CSP_REQ_NPCUPGRADE * aRecv);
	void GS_DGAnsTaxInfo(int aIndex,CSP_REQ_TAXINFO * aRecv);
	void GS_DGAnsTaxRateChange(int aIndex,CSP_REQ_TAXRATECHANGE * aRecv);
	void GS_DGAnsCastleMoneyChange(int aIndex,CSP_REQ_MONEYCHANGE * aRecv);
	void GS_DGAnsSiegeDateChange(int aIndex,CSP_REQ_SDEDCHANGE * aRecv);
	void GS_DGAnsGuildMarkRegInfo(int aIndex,CSP_REQ_GUILDREGINFO * aRecv);
	void GS_DGAnsSiegeEndedChange(int aIndex,CSP_REQ_SIEGEENDCHANGE * aRecv);
	void GS_DGAnsCastleOwnerChange(int aIndex,CSP_REQ_CASTLEOWNERCHANGE * aRecv);
	void GS_DGAnsRegAttackGuild(int aIndex,CSP_REQ_REGATTACKGUILD * aRecv);
	void GS_DGAnsRestartCastleState(int aIndex,CSP_REQ_CASTLESIEGEEND * aRecv);
	void GS_DGAnsMapSvrMsgMultiCast(int aIndex,CSP_REQ_MAPSVRMULTICAST * aRecv);
	void GS_DGAnsGlobalPostMultiCast(int aIndex, CSP_GLOBALPOST_MULTICAST * aRecv);
	void GS_DGAnsRegGuildMark(int aIndex,CSP_REQ_GUILDREGMARK * aRecv);
	void GS_DGAnsGuildMarkReset(int aIndex,CSP_REQ_GUILDRESETMARK * aRecv);
	void GS_DGAnsGuildSetGiveUp(int aIndex,CSP_REQ_GUILDSETGIVEUP * aRecv);
	void GS_DGAnsNpcRemove(int aIndex,CSP_REQ_NPCREMOVE * aRecv);
	void GS_DGAnsCastleStateSync(int aIndex,CSP_REQ_CASTLESTATESYNC * aRecv);
	void GS_DGAnsCastleTributeMoney(int aIndex,CSP_REQ_CASTLETRIBUTEMONEY * aRecv);
	void GS_DGAnsResetCastleTaxInfo(int aIndex,CSP_REQ_RESETCASTLETAXINFO * aRecv);
	void GS_DGAnsResetSiegeGuildInfo(int aIndex,CSP_REQ_RESETSIEGEGUILDINFO * aRecv);
	void GS_DGAnsResetRegSiegeInfo(int aIndex,CSP_REQ_RESETREGSIEGEINFO * aRecv);
	void GS_DGAnsAllGuildMarkRegInfo(int aIndex,CSP_REQ_ALLGUILDREGINFO * aRecv);
	void GS_DGAnsFirstCreateNPC(int aIndex,CSP_REQ_NPCSAVEDATA * aRecv);
	void GS_DGAnsCalcRegGuildList(int aIndex,CSP_REQ_CALCREGGUILDLIST * aRecv);
	void GS_DGAnsCsGulidUnionInfo(int aIndex,CSP_REQ_CSGUILDUNIONINFO * aRecv);
	void GS_DGAnsCsSaveTotalGuildInfo(int aIndex,CSP_REQ_CSSAVETOTALGUILDINFO * aRecv);
	void GS_DGAnsCsLoadTotalGuildInfo(int aIndex,CSP_REQ_CSLOADTOTALGUILDINFO * aRecv);
	void GS_DGAnsCastleNpcUpdate(int aIndex,CSP_REQ_NPCUPDATEDATA * aRecv);
	void DGAnsCrywolfSync(int aIndex,CWP_REQ_CRYWOLFSYNC * aRecv);
	void DGAnsCrywolfInfoLoad(int aIndex,CWP_REQ_CRYWOLFINFOLOAD * aRecv);
	void DGAnsCrywolfInfoSave(int aIndex,CWP_REQ_CRYWOLFINFOSAVE * aRecv);
	void DGAnsPeriodItemExInsert(int aIndex,PMSG_REQ_PERIODITEMEX_INSERT * aRecv);
	void DGAnsPeriodItemExSelect(int aIndex,PMSG_REQ_PERIODITEMEX_SELECT * aRecv);
	void DGAnsPeriodItemExDelete(int aIndex,PMSG_REQ_PERIODITEMEX_DELETE * aRecv);
	void ReqInGameShopItemList(short aIndex, ISHOP_REQ_ITEMLIST *aRecv);
	void ReqInGameShopPoint(short aIndex, ISHOP_REQ_POINT *aRecv);
	void ReqInGameShopAddPoint(short aindex, ISHOP_POINT_ADD *aRecv);
	void ReqInGameShopItemBuy(short aIndex, ISHOP_ITEM_BUY *aRecv);
	void ReqInGameShopPackageBuy(short aIndex, LPBYTE aRecv);
	void ReqInGameShopPackageGift(short aIndex, LPBYTE aRecv);
	void ReqInGameShopItemGift(short aIndex, ISHOP_ITEM_GIFT *aRecv);
	void ReqInGameShopItemUse(short aIndex, ISHOP_ITEM_USE *aRecv);
	void ReqInGameShopCharacterCardBuy(short aIndex, ISHOP_CHARCARD_BUY *aRecv);
	void ReqSetExpandedWarehouse(short aIndex, SDHP_EXPANDEDWAREHOUSE_SET *aRecv);
	void ReqInGameShopItemDelete(short aIndex, ISHOP_ITEM_DELETE *aRecv);
	void ReqInGameShopItemRollbackUse(short aIndex, ISHOP_ITEM_ROLLBACK *aRecv);
	void ReqMuBotDataSave(short aIndex, MUBOT_DATA *aRecv);
	void ReqLuckyCoinInfo(int aIndex,PMSG_REQ_LUCKYCOIN * lpMsg);
	void ReqRegLuckyCoin(int aIndex,PMSG_REQ_REGISTER_LUCKYCOIN * lpMsg);
	void DevilSqureScore(PMSG_ANS_EVENTUSERSCORE * pMsg);
	void GDReqBloodCastleEnterCount(int aIndex,PMSG_REQ_BLOODCASTLE_ENTERCOUNT * lpMsg);
	void BloodCastleScore_5TH(PMSG_ANS_BLOODCASTLESCORE_5TH * lpMsg);
	void IllusionTempleScore(PMSG_ANS_ILLUSIONTEMPLE_RANKING * lpMsg);
	void ChaosCastleScore(PMSG_ANS_CHAOSCASTLE_RANKING * lpMsg);
	void EGAnsEventChipInfo(int aIndex,PMSG_REQ_VIEW_EC_MN * lpMsg);
	void EGAnsRegEventChipInfo(int aIndex,PMSG_REQ_REGISTER_EVENTCHIP * lpMsg);
	void EGAnsResetEventChip(int aIndex,PMSG_REQ_RESET_EVENTCHIP * lpMsg);
	void EGAnsEventStoneInfo(int aIndex,PMSG_REQ_VIEW_EC_MN * lpMsg);
	void EGAnsRegEventStoneInfo(int aIndex,PMSG_REQ_REGISTER_STONES * lpMsg);
	void EGAnsResetStoneInfo(int aIndex,PMSG_REQ_RESET_EVENTCHIP * lpMsg);
	void EGAns2AnivRegSerial(int aIndex,PMSG_REQ_2ANIV_SERIAL * lpMsg);
	void EGAnsDeleteStones(int aIndex,PMSG_REQ_DELETE_STONES * lpMsg);
	void EGAnsRegCCOfflineGift(int aIndex,PMSG_REQ_REG_CC_OFFLINE_GIFT * lpMsg);
	void EGAnsRegDLOfflineGift(int aIndex,PMSG_REQ_REG_DL_OFFLINE_GIFT * lpMsg);
	void EGAnsRegHTOfflineGift(int aIndex,PMSG_REQ_REG_HT_OFFLINE_GIFT * lpMsg);
	void EGAnsLuckyCoinInfo(int aIndex,PMSG_REQ_LUCKYCOIN * lpMsg);
	void EGAnsRegLuckyCoin(int aIndex,PMSG_REQ_REGISTER_LUCKYCOIN * lpMsg);
	void EGReqSantaCheck(short aIndex, PMSG_REQ_SANTACHECK * aRecv);
	void EGReqSantaGift(short aIndex, PMSG_REQ_SANTAGIFT * aRecv);
	void ReqBanUser(short aIndex, BAN_REQ_USER * aRecv);
	void ReqSecLock(short aIndex, SECLOCK_REQ_SAVE *aRecv);
	void ReqSaveMonsterCount(short aIndex,DS_SAVE_MONSTERCNT * aRecv);
	void ReqSavePlayerKiller(short aIndex,DS_SAVE_PLAYERKILLER * aRecv);
	void GDReqArcaBattleGuildJoin(int aIndex, _tagPMSG_REQ_ARCA_BATTLE_GUILD_JOIN_DS *aRecv);
	void GDReqArcaBattleGuildMemberJoin(int aIndex, _tagPMSG_REQ_ARCA_BATTLE_GUILD_MEMBER_JOIN_DS *aRecv);
	void GDReqArcaBattleEnter(int aIndex, _tagPMSG_REQ_ARCA_BATTLE_ENTER_DS *aRecv);
	void GDReqArcaBattleWinGuildInfoInsert(int aIndex, _tagPMSG_REQ_AB_WIN_GUILD_INFO_INSERT_DS *aRecv);
	void GDReqArcaBattleWinGuildInfo(int aIndex, _tagPMSG_REQ_AB_WIN_GUILD_INFO_DS *aRecv);
	void GDReqDeleteArcaBattleInfo(int aIndex);
	void GDReqArcaBattleProcMultiCast(int aIndex, _tagPMSG_REQ_AB_PROC_INSERT_DS *aRecv);
	void GDReqArcaBattleProcState(int aIndex, _tagPMSG_REQ_AB_PROC_STATE_DS *aRecv);
	void GDReqArcaBattleJoinMemberUnder(int aIndex, _tagPMSG_REQ_AB_JOIN_MEMBER_UNDER_DS *aRecv);
	void GDReqArcaBattleJoinMemberUnderReq(int aIndex, _tagPMSG_REQ_AB_JOIN_CANCEL_DS *aRecv);
	void GDReqArcaBattleRegisteredMemberCnt(int aIndex, _tagPMSG_REQ_AB_REG_MEMBER_CNT_DS *aRecv);
	void GDReqRemoveAllGuildBuffMultiCast(int aIndex, _tagPMSG_REQ_REMOVE_ALL_GUILD_BUFF_DS *aRecv);
	void GDReqArcaBattleMarkCnt(int aIndex, _tagPMSG_REQ_ARCA_BATTLE_MARK_CNT_DS *aRecv);
	void GDReqArcaBattleMarkReg(int aIndex, _tagPMSG_REQ_ARCA_BATTLE_MARK_REG_DS *aRecv);
	void GDReqArcaBattleMarkRank(int aIndex, _tagPMSG_REQ_ARCA_BATTLE_MARK_RANK_DS *aRecv);
	void GDReqArcaBattleMarkRegDel(int aIndex, _tagPMSG_REQ_ARCA_BATTLE_MARK_REG_DEL_DS *aRecv);
	void GDReqArcaBattleIsTopRank(int aIndex, _tagPMSG_REQ_ARCA_BATTLE_IS_TOP_RANK *aRecv);
	void GDReqArcaBattleMarkRegAllDel(int aIndex);
	void GDReqArcaBattleAllGuildMarkCnt(int aIndex);
	void GDReqArcaBattleMarkRegSet(int aIndex, _tagPMSG_REQ_AB_MARK_REG_UPDATE_DS *aRecv);
	void GDReqArcaBattleGuildRegInit(int aIndex, _tagPMSG_REQ_GUILD_REG_INIT *aRecv);
	void GDReqArcaBattleAllJoinUser(int aIndex, _tagPMSG_REQ_AB_ALL_JOIN_USER_DS *aRecv);
	void GDReqGetPentagramJewel(int aIndex, PMSG_REQ_PENTAGRAMJEWEL *lpMsg);
	void GDReqSetPentagramJewel(int aIndex, LPBYTE lpRecv);
	void GDReqDelPentagramJewel(int aIndex, PMSG_DEL_PENTAGRAMJEWEL *lpMsg);
	void GDReqInsertPentagramJewel(int aIndex, PMSG_INSERT_PENTAGRAMJEWEL *lpMsg);
	void GDReqPeriodBuffInsert(int aIndex, PMSG_REQ_PERIODBUFF_INSERT *aRecv);
	void GDReqPeriodBuffDelete(int aIndex, PMSG_REQ_PERIODBUFF_DELETE *aRecv);
	void GDReqPeriodBuffSelect(int aIndex, PMSG_REQ_PERIODBUFF_SELECT *aRecv);
	void GDReqQuestExpInfoLoad(int aIndex, PMSG_REQ_QUESTEXP_INFO *aRecv);
	void GDReqQuestExpInfoSave(int aIndex, LPBYTE lpRecv);
	void GDReqLuckyItemInsert(int aIndex, PMSG_REQ_LUCKYITEM_INSERT* lpMsg);
	void GDReqLuckyItemInsert2nd(int aIndex, PMSG_REQ_LUCKYITEM_INSERT_2ND* lpMsg);
	void GDReqLuckyItemDelete(int aIndex, PMSG_REQ_LUCKYITEM_DELETE* lpMsg);
	void GDReqLuckyItemSelect(int aIndex, PMSG_REQ_LUCKYITEM_SELECT* lpMsg);
	void GDReqChaosCastleFinalSave(int aIndex, _tagPMSG_REQ_SAVE_CCF_RESULT* lpMsg);
	void GDReqChaosCastleFinalPermission(int aIndex, _tagPMSG_REQ_CCF_PERMISSION* lpMsg);
	void GDReqChaosCastleFinalLoad(int aIndex, SDHP_REQ_CCF_RANKING* lpMsg);
	void GDReqChaosCastleFinalRenew(int aIndex, SDHP_RENEW_RANKING* lpMsg);
	void GDReqChaosCastleFinalSendMsgAllSvr(int aIndex, PMSG_SEND_CCF_INFO_ALL_SVR* lpMsg);
	void GDReqLoadMuunInvenItem(int aIndex, SDHP_REQ_DBMUUN_INVEN_LOAD* lpMsg);
	void GDReqSaveMuunInvenItem(int aIndex, _tagSDHP_REQ_DBMUUN_INVEN_SAVE * aRecv);
	void GDReqLoadEventInvenItem(int aIndex, _tagSDHP_REQ_DBEVENT_INVEN_LOAD * aRecv);
	void GDReqSaveEventInvenItem(int aIndex, _tagSDHP_REQ_DBEVENT_INVEN_SAVE * aRecv);
	void GDReqCardInfo(int aIndex, _tagPMSG_REQ_MURUMMY_SELECT_DS * aRecv);
	void GDReqCardInfoInsert(int aIndex, _tagPMSG_REQ_MURUMMY_INSERT_DS * aRecv);
	void GDReqScoreUpdate(int aIndex, _tagPMSG_REQ_MURUMMY_SCORE_UPDATE_DS * aRecv);
	void GDReqCardInfoUpdate(int aIndex, _tagPMSG_REQ_MURUMMY_UPDATE_DS * aRecv);
	void GDReqScoreDelete(int aIndex, _tagPMSG_REQ_MURUMMY_DELETE_DS * aRecv);
	void GDReqSlotInfoUpdate(int aIndex, _tagPMSG_REQ_MURUMMY_SLOTUPDATE_DS * aRecv);
	void GDReqMuRummyInfoUpdate(int aIndex, _tagPMSG_REQ_MURUMMY_INFO_UPDATE_DS * aRecv);
	void GDReqMuRummyDBLog(int aIndex, _tagPMSG_REQ_MURUMMY_LOG_INSERT_DS * aRecv);
	void GDReqMineModifyUPTUserInfo(int aIndex, SDHP_REQ_MINESYSTEM_UPT_USERINFO * aRecv);
	void GDReqMineCheckIsUPTWhenUserConnect(int aIndex, SDHP_REQ_LOAD_MINESYSTEM_UPT_USERINFO * aRecv);
	void GDReqPShopItemValue(int aIndex, PMSG_REQ_PSHOPITEMVALUE_INFO * aRecv);
	void GDAllSavePShopItemValue(int aIndex, PMSG_UPDATE_PSHOPITEMVALUE_INFO * aRecv);
	void GDDelPShopItemValue(int aIndex, PMSG_DEL_PSHOPITEM * aRecv);
	void GDMovePShopItem(int aIndex, PMSG_MOVE_PSHOPITEM * aRecv);
	void GDReqAcheronGuardianProcMultiCast(int aIndex, _tagPMSG_REQ_AE_PLAY_DS * aRecv);
	void GDReqClassDefData(int aIndex);
	void GDReqReBuyItemList(int aIndex, SDHP_REQ_SHOP_REBUY_LIST * aRecv);
	void GDReqReBuyAddItem(int aIndex, SDHP_REQ_SHOP_REBUY_ADD_ITEM * aRecv);
	void GDReqReBuyGetItem(int aIndex, SDHP_REQ_SHOP_REBUY_GET_ITEM * aRecv);
	void GDReqDeleteSoldItem(int aIndex, SDHP_REQ_SHOP_REBUY_DELETE_ITEM * aRecv);
	void GDReqGremoryCaseItemList(int aIndex, _stReqGremoryCaseItemList * aRecv);
	void GDReqGremoryCaseAddItem(int aIndex, _stReqAddItemToGremoryCase * aRecv);
	void GDReqCheckUseItemGremoryCase(int aIndex, _stReqCheckUseItemGremoryCase * aRecv);
	void GDReqGremoryCaseDeleteItem(int aIndex, _stReqDeleteItemFromGremoryCase * aRecv);
	void GDReqUBFCheckIsJoinedUser(int aIndex, PMSG_REQ_UBF_ACCOUNT_USERINFO * aRecv);
	void GDReqUBFJoinUser(int aIndex, PMSG_UBF_REGISTER_ACCOUNT_USER * aRecv);
	void GDReqUBFCopyCharacter(int aIndex, PMSG_UBF_ACCOUNT_USER_COPY * aRecv);
	void GDReqUBFCancelUser(int aIndex, PMSG_UBF_REQ_CANCEL_REGISTER_USER * aRecv);
	void GDReqUBFGetRealName(int aIndex, PMSG_REQ_GET_UBF_REAL_NAME * aRecv);
	void GDReqUBFCopyPetItem(int aIndex, LPBYTE lpRecv);
	void GDReqUBFGetReward(int aIndex, PMSG_REQ_UBF_GET_REWARD * aRecv);
	void GDReqUBFSetGainReward(int aIndex, PMSG_REQ_UBF_SET_RECEIVED_REWARD * aRecv);
	void GDReqUBFSetCCFReward(int aIndex, SDHP_REQ_SET_CCF_WINNER_INFO * aRecv);
	void GDReqUBFSetCCBattleReward(int aIndex, SDHP_REQ_SET_CC_WINNER_INFO_UBF * aRecv);
	void GDReqUBFSetDSFReward(int aIndex, PMSG_REQ_SET_DSF_WINNER_INFO * aRecv);
	void GDReqDSFCanPartyEnter(int aIndex, PMSG_REQ_DSF_CAN_PARTY_ENTER * aRecv);
	void GDReqDSFSavePartyPoint(int aIndex, PMSG_REQ_SAVE_DSF_PARTYPOINT * aRecv);
	void GDReqDSFPartyRankRenew(int aIndex, PMSG_REQ_DSF_PARTYRANKRENEW * aRecv);
	void GDReqDSFGoFinalParty(int aIndex, PMSG_REQ_DSF_GO_FINAL_PARTY * aRecv);
	void GDReqDSFInsertRewardUser(int aIndex, PMSG_REQ_SAVE_DSF_REWARD_USER * aRecv);
	void GDReqDSFGetReward(int aIndex, PMSG_REQ_GET_DSF_REWARD * aRecv);
	void GDReqWishperOtherChannel(int aIndex, PMSG_RECV_CHATDATA_WHISPER * aRecv);
	void GDReqMapSrvGroupServerCount(int aIndex, DSMSG_REQ_SUBSERVER_COUNT * aRecv);
	void DGWhisperResponseRecv(int aIndex, DSMSG_ANS_WHISPER_RESULT * aRecv);
	void DGChaosMachineLogLostItems(int aIndex, DSMSG_CHAOSMACHINE_LOSTITEMS * aRecv);
	void GDDisconnectOtherChannel(int aIndex, PMSG_RECV_DC_OTHER_CHANNEL * aRecv);
	CQuery m_CharDB;
	CQuery m_AccDB;
	CQuery m_GSDB;
	CQuery m_PetDB;
	CQuery m_CastleDB;
	CQuery m_CrywolfDB;
	CQuery m_PentagramDB;
	CQuery m_EventDB;
	CQuery m_RankingDB;
	CQuery m_ItemShopDB;
	CQuery m_PeriodItemDB;
	CQuery m_CharMiscDB;
	CQuery m_ArcaDB;
	CQuery m_QuestExpDB;
	CQuery m_LuckyItemDB;
	CQuery m_CCFinalDB;
	CQuery m_RummyDB;
	CQuery m_MineDB;
	CQuery m_PShopDB;
	CQuery m_EventInvDB;
	CQuery m_MuunDB;
	CQuery m_OptionDataDB;
	CQuery m_ReBuyDB;
	CQuery m_GremoryCaseDB;
	CQuery m_BattleCoreDB;
	CQuery m_DSFinalDB;
	public:
	CQuery m_MembStatDB;
	private:
	CRITICAL_SECTION m_QuestExpDBCriti;
	CWarehouseUserData m_WareUserData;

	int m_iConnectionCount;
	int m_iItemCount;

};

#endif // !defined(AFX_DSPROTOCOL_H__2BBE1FF8_64DE_46AC_8344_13E7059DCBE3__INCLUDED_)


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

