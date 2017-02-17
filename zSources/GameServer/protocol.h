// ------------------------------
// Decompiled by Deathway
// Date : 2007-05-09
// ------------------------------
#ifndef PROTOCOL_H
#define PROTOCOL_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "prodef.h"
#include "GuildClass.h"
#include "user.h"
#include "MapClass.h"
#include "GensSystemProtocol.h"
#include "Shop.h"

struct PWMSG_COUNT 
{
	PWMSG_HEAD h;
	BYTE count;
};

struct PBMSG_COUNT 
{
	PBMSG_HEAD h;
	BYTE count;
};

struct PBMSG_COUNT2 
{
	PBMSG_HEAD2 h;
	BYTE count;
};

struct PWMSG_COUNT2 
{
	PWMSG_HEAD2 h;
	BYTE count;
};

struct PMSG_ITEMVIEWPORTCREATE 
{
	BYTE NumberH;
	BYTE NumberL;
	BYTE px;
	BYTE py;
	BYTE ItemInfo[MAX_ITEM_INFO];
};

typedef struct 
{
	BYTE	NumberH;
	BYTE	NumberL;
	BYTE	X;
	BYTE	Y;
	BYTE	CharSet[18];
	char	Id[MAX_ACCOUNT_LEN];
	BYTE	TX;
	BYTE	TY;
	BYTE	DirAndPkLevel;
	BYTE	PentagramMainAttribute;
	BYTE	MuunItemH;
	BYTE	MuunItemL;
	BYTE	MuunSubItemH;
	BYTE	MuunSubItemL;
	BYTE	MuunRideItemH;
	BYTE	MuunRideItemL;
	BYTE	LevelH;
	BYTE	LevelL;
	BYTE	MaxLifeHH;
	BYTE	MaxLifeHL;
	BYTE	MaxLifeLH;
	BYTE	MaxLifeLL;
	BYTE	CurLifeHH;
	BYTE	CurLifeHL;
	BYTE	CurLifeLH;
	BYTE	CurLifeLL;
	WORD	nServerCodeOfHomeWorld;
	BYTE	BuffEffectCount;
	BYTE	BuffEffectList[MAX_BUFFEFFECT];
} PMSG_VIEWPORTCREATE, * LPPMSG_VIEWPORTCREATE;

typedef struct 
{
	BYTE	NumberH;
	BYTE	NumberL;
	BYTE	X;
	BYTE	Y;
	BYTE	SkinH;
	BYTE	SkinL;
	char	Id[MAX_ACCOUNT_LEN];
	BYTE	TX;
	BYTE	TY;
	BYTE	DirAndPkLevel;
	BYTE	CharSet[18];
	BYTE	PentagramMainAttribute;
	BYTE	MuunItemH;
	BYTE	MuunItemL;
	BYTE	MuunSubItemH;
	BYTE	MuunSubItemL;
	BYTE	MuunRideItemH;
	BYTE	MuunRideItemL;
	BYTE	LevelH;
	BYTE	LevelL;
	BYTE	MaxLifeHH;
	BYTE	MaxLifeHL;
	BYTE	MaxLifeLH;
	BYTE	MaxLifeLL;
	BYTE	CurLifeHH;
	BYTE	CurLifeHL;
	BYTE	CurLifeLH;
	BYTE	CurLifeLL;
	WORD	nServerCodeOfHomeWorld;
	BYTE	BuffEffectCount;
	BYTE	BuffEffectList[MAX_BUFFEFFECT];
} PMSG_VIEWPORTCREATE_CHANGE, * LPPMSG_VIEWPORTCREATE_CHANGE;

typedef struct 
{
	BYTE	NumberH;
	BYTE	NumberL;	
} PMSG_VIEWPORTDESTROY, *LPPMSG_VIEWPORTDESTROY;

struct PMSG_MONSTER_VIEWPORTCREATE
{
	BYTE	NumberH;
	BYTE	NumberL;
	BYTE	Type_HI;
	BYTE	Type_LO;
	BYTE	X;
	BYTE	Y;
	BYTE	TX;
	BYTE	TY;
	BYTE	Path;
	BYTE	PentagramMainAttribute;
	BYTE	LevelH;
	BYTE	LevelL;
	BYTE	MaxLifeHH;
	BYTE	MaxLifeHL;
	BYTE	MaxLifeLH;
	BYTE	MaxLifeLL;
	BYTE	CurLifeHH;
	BYTE	CurLifeHL;
	BYTE	CurLifeLH;
	BYTE	CurLifeLL;
	BYTE	BuffEffectCount;
	BYTE	BuffEffectList[MAX_BUFFEFFECT];
};

struct PMSG_CALLMONSTER_VIEWPORTCREATE
{
	BYTE	NumberH;
	BYTE	NumberL;
	BYTE	Type_HI;
	BYTE	Type_LO;
	BYTE	X;
	BYTE	Y;
	BYTE	TX;
	BYTE	TY;
	BYTE	Path;	
	BYTE	Id[MAX_ACCOUNT_LEN];
	BYTE	PentagramMainAttribute;
	BYTE	LevelH;
	BYTE	LevelL;
	BYTE	MaxLifeHH;
	BYTE	MaxLifeHL;
	BYTE	MaxLifeLH;
	BYTE	MaxLifeLL;
	BYTE	CurLifeHH;
	BYTE	CurLifeHL;
	BYTE	CurLifeLH;
	BYTE	CurLifeLL;
	BYTE	BuffEffectCount;
	BYTE	BuffEffectList[MAX_BUFFEFFECT];
};

struct PMSG_GUILDVIEWPORT_USER 
{
	BYTE NumberH;
	BYTE NumberL;
	BYTE GNumberH;
	BYTE GNumberL;
};

struct PMSG_GUILDVIEWPORT 
{
	BYTE NumberH;
	BYTE NumberL;
	char GuildName[8];
	BYTE Mark[32];
};

struct PMSG_DISABLE_RECONNECT // can use 1 for both
{
	PBMSG_HEAD h;
	BYTE subcode;
	BYTE Trash[100];
};

struct PMSG_ANTIHACK_BREACH
{
	PBMSG_HEAD h;
	BYTE subcode;
	DWORD dwErrorCode;
};

struct PMSG_ANTIHACK_CHECK
{
	PBMSG_HEAD h;
	BYTE subcode;
	BYTE checkdata[5];
};

struct PMSG_ATTACKSPEEDSEND
{
	PBMSG_HEAD2		h;
	DWORD			AttackSpeed;
	DWORD			MagicSpeed;
};

struct PMSG_PROPERTYITEMCOUNT
{
	PBMSG_HEAD2		h;
	BYTE			btCount;
};

struct PMSG_REQ_USETRANSFORMATIONRING
{
	PBMSG_HEAD2		h;
	BYTE			btState;
};

struct PMSG_PROPERTYITEMINFO
{
	PBMSG_HEAD2		h;
	short			ItemID;
	short			Pos;
	int				Time;
};

struct PMSG_EX_GAMESERVER_MOVE_SEND
{
	PBMSG_HEAD2 h;
	BYTE btResult;
	BYTE btGameServerCode;
};

struct PMSG_EX_GAMESERVER_MOVE_RESULT
{
	PBMSG_HEAD2 h;
	BYTE btResult;
};

struct PMSG_AGILITYSEND
{
	PBMSG_HEAD h;
	BYTE subcode;
	WORD Agility;
};

struct PMSG_ANS_MAPSVRTAXINFO
{
	PBMSG_HEAD2 h;	// C1:B2:1A
	BYTE btTaxType;	// 4
	BYTE btTaxRate;	// 5
};
struct SDHP_IDPASS
{
	PBMSG_HEAD h;	// C1:01	[11::LOGIN-TEST]
	char Id[10];	// 3
	char Pass[20];	// D
	short Number;	// 18
	char HWID[100];
	char IpAddress[17];	// 1A
};
#pragma pack (1)
struct PMSG_ADDSTATS
{
	PBMSG_HEAD h;
	BYTE subcode;
	BYTE statstype;
	WORD Points;
	WORD LUP;
	int MaxLifeAndMana;
};

struct PMSG_IDPASS
{
	PBMSG_HEAD h;
	BYTE subcode;	// 3
	char Id[10];	// 4
	char Pass[20];	// E
	char HWID[100]; // NEW
	DWORD TickCount;	// 18
	BYTE CliVersion[5];	// 1C
	BYTE CliSerial[16];	// 21  
	DWORD ServerSeason;
};
#pragma pack ()

struct PMSG_MONEY
{
	PBMSG_HEAD h;	// C1:81
	BYTE Result;	// 3
	int wMoney;	// 4
	int iMoney;	// 8
};

struct PMSG_ATTACK
{
	PBMSG_HEAD h;
	BYTE NumberH;	// 3
	BYTE NumberL;	// 4
	BYTE AttackAction;	// 5
	BYTE DirDis;	// 6
};

struct PMSG_MAGICATTACK
{
	PBMSG_HEAD h;
	BYTE NumberH;
	BYTE MagicNumberH;
	BYTE NumberL;
	BYTE MagicNumberL;
	BYTE Dis;
};

struct PMSG_DURATION_MAGIC_RECV
{
	PBMSG_HEAD h;
	BYTE X;
	BYTE MagicNumberH;
	BYTE Y;
	BYTE MagicNumberL;
	BYTE Dir;
	BYTE NumberH;
	BYTE Dis;
	BYTE NumberL;
	BYTE TargetPos;
	BYTE MagicKey;
};

struct PMSG_BEATTACK_COUNT
{
	PBMSG_HEAD h;	// C1:D7
	BYTE MagicNumberH;
	BYTE Count;
	BYTE MagicNumberL;
	BYTE X;
	BYTE Serial;
	BYTE Y;
};

struct PMSG_BEATTACK
{
	BYTE NumberH;	// 0
	BYTE MagicKey;	// 1
	BYTE NumberL;	// 2
};

struct PMSG_ACTIONRESULT
{
	PBMSG_HEAD h;
	BYTE NumberH;	// 3
	BYTE NumberL;	// 4
	BYTE Dir;	// 5
	BYTE ActionNumber;	// 6
	BYTE TargetNumberH;	// 7
	BYTE TargetNumberL;	// 8
};

#pragma pack (1)

struct PMSG_RAGEATTACK_REQ
{
	PBMSG_HEAD h;
	BYTE NumberH;
	BYTE MagicNumberH;
	BYTE NumberL;
	BYTE MagicNumberL;
	BYTE Empty;
};
#pragma pack ()

struct PMSG_KILLPLAYER_EXT
{
	PBMSG_HEAD h;	// C3:9C
	BYTE NumberH;	// 3
	BYTE NumberL;	// 4
	WORD ExpH;	// 6
	WORD ExpL;	// 8
	BYTE DamageH;	// A
	BYTE DamageL;	// B
};

struct PMSG_ANS_END_DUEL
{
	PBMSG_HEAD h;	// C1:AB
	BYTE NumberH;	// 3
	BYTE NumberL;	// 4
	char szName[10];	// 5
};

/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Send Warehouse Info Packet
 *	Direction : GameServer -> Client
 *  Code      : 0xC3
 *	HeadCode  : 0x30
 */
struct PMSG_TALKRESULT
{
	PBMSG_HEAD h;
	unsigned char result;	// 3
	BYTE level1;	// 4
	BYTE level2;	// 5
	BYTE level3;	// 6
	BYTE level4;	// 7
	BYTE level5;	// 8
	BYTE level6;	// 9
	BYTE level7;
};

/* * * * * * * * * * * * * * * * * * * * * 
 *	Mu Result Packet
 *	Direction : GameServer -> Client
 *  Code     : 0xC1
 *	HeadCode : 0xF3
 *	SubCode  : 0x02
 */
struct PMSG_RESULT
{
	PBMSG_HEAD h;
	unsigned char subcode;	// 3
	unsigned char result;	// 4
};

struct PMSG_SUMMONER_INFO
{
	PBMSG_HEAD2 h;
	BYTE Result;
};

/* * * * * * * * * * * * * * * * * * * * * 
 *	Packet to Send Blod Castle State
 *	Direction : GameServer -> Client
 *	Code      : 0xC1
 *	HeadCode  : 0x9B
 */
struct PMSG_STATEBLOODCASTLE
{
	PBMSG_HEAD h;	// C1:9B
	BYTE btPlayState;		// 3
	WORD wRemainSec;	// 4
	WORD wMaxKillMonster;	// 6
	WORD wCurKillMonster;	// 8
	WORD wUserHaveWeapon;	// A
	BYTE btWeaponNum;	// C
};

struct PMSG_KILLCOUNT
{
	PBMSG_HEAD2 h;
	BYTE btKillCount;
};

struct SDHP_DBCHARINFOREQUEST
{
	PBMSG_HEAD h;	// C1:06
	char AccountID[11];	// 3
	char Name[11];	// E
	short Number;	// 1A
	bool IsSeasonX;
};

struct PMSG_ATTACKRESULT
{
	PBMSG_HEAD h;	// C1:DC
	BYTE NumberH;	// 3
	BYTE NumberL;	// 4
	BYTE DamageH;	// 5
	BYTE DamageL;	// 6
	BYTE DamageTypeH;
	BYTE DamageTypeL;	// 7
	BYTE btShieldDamageH;	// 8
	BYTE btShieldDamageL;	// 9
	int IGCDamage;
};

struct PMSG_ELEMENTALDAMAGE
{
	PBMSG_HEAD h;
	BYTE NumberH;
	BYTE NumberL;
	BYTE btElement;
	BYTE btTargetH;
	BYTE btTargetL;
	DWORD Damage;
};
#pragma pack (1)
struct PMSG_PENTAGRAMJEWEL_INFO
{
	PWMSG_HEAD2 h;
	BYTE btResult;
	BYTE btJewelCnt;
	BYTE btJewelPos;
};

struct PMSG_REQ_IN_PENTAGRAM_JEWEL
{
	PBMSG_HEAD2 h;
	int iPentagramPos;
	int iJewelPos;
};

struct PMSG_ANS_IN_PENTAGRAM_JEWEL
{
	PBMSG_HEAD2 h;
	BYTE btResult;
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

struct PMSG_PENTAGRAM_JEWEL_INOUT
{
	PBMSG_HEAD2 h;
	BYTE btResult;
};

struct PMSG_REQ_OUT_PENTAGRAM_JEWEL
{
	PBMSG_HEAD2 h;
	int iPentagramPos;
	BYTE btSocketIndex;
};

struct PMSG_ANS_OUT_PENTAGRAM_JEWEL
{
	PBMSG_HEAD2 h;
	BYTE Result;
	BYTE btJewelPos;
	BYTE btJewelDBIndex;
};

#pragma pack ()

struct PMSG_RECV_POSISTION_SET
{
	PBMSG_HEAD h;	// C1:D6
	BYTE NumberH;	// 3
	BYTE NumberL;	// 4
	BYTE X;	// 5
	BYTE Y;	// 6
};

struct PMSG_USEREQUIPMENTCHANGED
{
	PBMSG_HEAD h;
	BYTE NumberH;	// 3
	BYTE NumberL;	// 4
	BYTE ItemInfo[MAX_ITEM_INFO];	// 5
	BYTE Element;
};

struct PMSG_SERVERCMD
{
	PBMSG_HEAD h;
	BYTE subcode;	// 3
	BYTE CmdType;	// 4
	BYTE X;	// 5
	BYTE Y;	// 6
};

struct PMSG_ITEMTHROW_RESULT
{
	PBMSG_HEAD h;	// C1:23
	unsigned char Result;	// 3
	BYTE Ipos;	// 4
};

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

struct PMSG_GUILDCREATED_RESULT
{
	PBMSG_HEAD h;	// C1:56
	BYTE Result;	// 3
	BYTE btGuildType;	// 4
};

struct PMSG_SIMPLE_GUILDVIEWPORT_COUNT
{
	PWMSG_HEAD h;	// C2:65
	BYTE Count;	// 4
};

struct PMSG_SIMPLE_GUILDVIEWPORT
{
	int GuildNumber;	// 0
	BYTE btGuildStatus;	// 4
	BYTE btGuildType;	// 5
	BYTE btGuildRelationShip;	// 6
	BYTE NumberH;	// 7
	BYTE NumberL;	// 8
	BYTE btOwnerStatus; //9 season4 add-on
};

struct PMSG_RECVMOVE
{
	PBMSG_HEAD h;
	BYTE NumberH;	// 3
	BYTE NumberL;	// 4
	BYTE X;	// 5
	BYTE Y;	// 6
	BYTE Path;	// 7
};

struct PMSG_MAGICATTACK_RESULT
{
	PBMSG_HEAD h;
	BYTE SourceNumberH;
	BYTE SourceNumberL;
	BYTE MagicNumberH;
	BYTE MagicNumberL;
	BYTE TargetNumberH;
	BYTE TargetNumberL;
};

struct PMSG_SET_DEVILSQUARE
{
	PBMSG_HEAD h;	// C1:92
	BYTE Type;	// 3
};

struct PMSG_GUILD_ASSIGN_STATUS_RESULT
{
	PBMSG_HEAD h;	// C1:E1
	BYTE btType;	// 3
	BYTE btResult;	// 4
	char szTagetName[10];	// 5
};

struct PMSG_GUILD_ASSIGN_TYPE_RESULT
{
	PBMSG_HEAD h;	// C1:E2
	BYTE btGuildType;	// 3
	BYTE btResult;	// 4
};

//////////////////////////////////////////////////////////////////////////////
//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
//////////////////////////////////////////////////////////////////////////////

#pragma pack (1)
struct PMSG_CLIENTTIME
{
	PBMSG_HEAD h;
	WORD TimeH;
	WORD TimeL;
	WORD AttackSpeed;	// 8
	WORD Agility;
	WORD MagicSpeed;	// A
	char Version[10];
	DWORD ServerSeason;
};
#pragma pack ()

struct PMSG_AHINFO
{
	PBMSG_HEAD h;
	BYTE SUB;
	DWORD Time;	// 4
	WORD AttackSpeed;	// 8
	WORD MagicSpeed;	// A
	char dump1[15];
	BYTE IsModifiedGameSpeed;
	char dump2[15];
	BYTE IsModifiedAttackSpeed;
	char dump3[15];
	BYTE IsModifiedMagicSpeed;
	char dump4[15];
};
#pragma pack(1)
struct PMSG_CHECK_MAINEXE_RESULT
{
	PBMSG_HEAD2 h;
	DWORD m_dwKey;	// 4
};
#pragma pack()



struct PMSG_CHATDATA
{
	PBMSG_HEAD h;	//	
	char chatid[10];	//	3
	char chatmsg[90];	//	D
};


struct PMSG_CHATDATA_NUMBER
{
	PBMSG_HEAD h;
	BYTE NumberH;	// 3
	BYTE NumberL;	// 4
	char chatmsg[90];	// 5
};

struct PMSG_CHATDATA_WHISPER
{
	PBMSG_HEAD h;	// C1:02
	char id[10];	// 3
	char chatmsg[90];	// D  
};

struct PMSG_CLIENTCLOSE
{
	PBMSG_HEAD h;
	BYTE SubCode;	// 3
	BYTE Flag;	// 4
};


struct PMSG_CLIENTMSG
{
	PBMSG_HEAD h;
	BYTE SubCode;	// 3
	BYTE Flag;	// 4
	BYTE subFlag;	// 5
};


struct PMSG_CHARCREATE
{
	PBMSG_HEAD h;
	BYTE subcode;	// 3
	char Name[10];	// 4
	BYTE ClassSkin;	// E
};


struct PMSG_CHARDELETE
{
	PBMSG_HEAD h;
	BYTE subcode;	// 3
	char Name[10];	// 4
	char LastJoominNumber[20];	// E
};


struct PMSG_CHARMAPJOIN
{
	PBMSG_HEAD h;
	BYTE subcode;	// 3
	char Name[10];	// 4
	BYTE btSlotIndex;
};


struct PMSG_LVPOINTADD
{
	PBMSG_HEAD h;
	BYTE subcode;	// 3
	BYTE Type;	// 4
};

struct PMSG_LVPOINTADDRESULT
{
	PBMSG_HEAD h;	// C1:F3:06
	BYTE subcode;	// 3
	BYTE ResultType;	// 4
	WORD MaxLifeAndMana;	// 6
	WORD wMaxShield;	// 8
	WORD MaxBP;	// A
	int IGCMaxLifeAndMana;
};

struct PMSG_SKILLKEY
{
	PBMSG_HEAD h;	// C1:F3:30
	BYTE subcode;	// 3
	BYTE SkillKey[20];	// 4
	BYTE GameOption;	// E
	BYTE QkeyDefine;	// F
	BYTE WkeyDefine;	// 10
	BYTE EkeyDefine;	// 11
	BYTE ChatWindow;	// 13
	BYTE RkeyDefine;
	int QWERLevel;
};

struct PMSG_EX_SKILL_COUNT 
{
	PBMSG_HEAD h;	// C1:BA
	BYTE NumberH;
	BYTE NumberL;
	WORD Type;
	BYTE Count;
};

struct PMSG_ITEMGETREQUEST
{
	PBMSG_HEAD h;
	BYTE NumberH;	// 3
	BYTE NumberL;	// 4
};


struct PMSG_ITEMTHROW
{
	PBMSG_HEAD h;
	BYTE px;	// 3
	BYTE py;	// 4
	BYTE Ipos;	// 5
};


struct PMSG_INVENTORYITEMMOVE
{
	PBMSG_HEAD h;
	BYTE sFlag;	// 3
	BYTE source_item_num;	// 4
	BYTE sItemInfo[MAX_ITEM_INFO];	// 5
	BYTE tFlag;	// C
	BYTE target_item_num;	// D
};


struct PMSG_TALKREQUEST
{
	PBMSG_HEAD h;
	BYTE NumberH;	// 3
	BYTE NumberL;	// 4
};


struct PMSG_BUYREQUEST
{
	PBMSG_HEAD h;
	BYTE Pos;	// 3
};


struct PMSG_SELLREQUEST
{
	PBMSG_HEAD h;
	BYTE Pos;	// 3
};


struct PMSG_ITEMDURREPAIR
{
	PBMSG_HEAD h;
	BYTE Position;	// [0xFF:Repair ALL] 3
	BYTE Requestpos;	// 4
};


struct PMSG_TRADE_REQUEST
{
	PBMSG_HEAD h;
	BYTE NumberH;	// 3
	BYTE NumberL;	// 4
};

struct PMSG_USE_STAT_FRUIT 
{
	PBMSG_HEAD h;
	BYTE result;
	WORD btStatValue;
	BYTE btFruitType;
};

struct PMSG_TRADE_RESPONSE
{
	PBMSG_HEAD h;	// C1:37
	BYTE Response;	// 3
	BYTE Id[10];	// 4
	WORD Level;	// E
	int GuildNumber;	// 10
};


struct PMSG_TRADE_GOLD
{
	PBMSG_HEAD h;
	DWORD Money;	// 4
};


struct PMSG_TRADE_OKBUTTON
{
	PBMSG_HEAD h;	// C1:3C
	BYTE Flag;	// 3
};

struct PMSG_PARTYREQUEST
{
	PBMSG_HEAD h;
	BYTE NumberH;	// 3
	BYTE NumberL;	// 4
};


struct PMSG_PARTYREQUESTRESULT
{
	PBMSG_HEAD h;
	BYTE Result;	// 3
	BYTE NumberH;	// 4
	BYTE NumberL;	// 5
};


struct PMSG_PARTYDELUSER
{
	PBMSG_HEAD h;
	BYTE Number;	// 3
	char szTargetName[MAX_ACCOUNT_LEN + 1];
};

struct PMSG_PARTYLIST
{
	char szId[10];	// 0
	BYTE Number;	// A
	BYTE MapNumber;	// B
	BYTE X;	// C
	BYTE Y;	// D
	int Life;	// 10
	int MaxLife;	// 14
	int nServerChannel;
	int nMana;
	int nMaxMana;
};

struct PMSG_PARTYLISTCOUNT
{
	PBMSG_HEAD h;	// C1:42
	BYTE Result;	// 3
	BYTE Count;	// 4
};


struct PMSG_GUILDJOINQ
{
	PBMSG_HEAD h;
	BYTE NumberH;	// 3
	BYTE NumberL;	// 4
};


struct PMSG_GUILDQRESULT
{
	PBMSG_HEAD h;
	BYTE Result;	// 3
	BYTE NumberH;	// 4
	BYTE NumberL;	// 5
};


struct PMSG_GUILDDELUSER
{
	PBMSG_HEAD h;
	char Name[10];	// 3
	char Password[20];	// D
};


struct PMSG_GUILDMASTERANSWER
{
	PBMSG_HEAD h;
	BYTE Result;	// 3
};


struct PMSG_GUILDINFOSAVE
{
	PBMSG_HEAD h;
	BYTE btGuildType;	// 3
	char GuildName[8];	// 4
	BYTE Mark[32];	// C
};


struct PMSG_GUILDWARSEND_RESULT
{
	PBMSG_HEAD h;
	BYTE Result;	// 3
};


struct PMSG_WAREHOUSEMONEYINOUT
{
	PBMSG_HEAD h;
	BYTE Type;	// [0x01:Deposit] [0x02:Withdraw] 3
	int Money;	// 4
};


struct PMSG_WAREHOUSEPASSSEND
{
	PBMSG_HEAD h;
	BYTE Type;	// [0x00:Open Warehouse] [0x01:Set Pass] [0x02:Delete Pass] 3
	short Pass;	// 4
	char LastJoominNumber[20];	// 6
};

struct PMSG_CHAOSMIX
{
	PBMSG_HEAD h;
	BYTE Type;	// 3
	BYTE Pos;	// 4
	BYTE MixCount;
};

struct PMSG_MOVE
{
	PBMSG_HEAD h;	// C1:1D
	BYTE X;	// 3
	BYTE Y;	// 4
	BYTE Path[8];	// 5
};


struct PMSG_POSISTION_SET
{
	PBMSG_HEAD h;
	BYTE X;	// 3
	BYTE Y;	// 4
};

struct PMSG_ACTION
{
	PBMSG_HEAD h;
	BYTE Dir;	// 3
	BYTE ActionNumber;	// 4
	BYTE iTargetIndexH;	// 5
	BYTE iTargetIndexL;	// 6
};

struct PMSG_MAGICCANCEL 
{
	PBMSG_HEAD h;
	BYTE MagicNumberL;
	BYTE MagicNumberH;	// 3
	BYTE NumberH;	// 4
	BYTE NumberL;	// 5
};


struct PMSG_TELEPORT
{
	PBMSG_HEAD h;
	WORD MoveNumber;	// 3
	BYTE MapX;	// 4
	BYTE MapY;	// 5
};

struct PMSG_MAPMOVE_CHECKSUM
{
	PBMSG_HEAD2 h;
	DWORD dwKeyValue;
};

#pragma pack (1)
struct PMSG_REQ_MAPMOVE
{
	PBMSG_HEAD2 h;
	DWORD dwBlockKey;
	WORD wMapIndex;
};
#pragma pack ()

struct PMSG_ANS_MAPMOVE
{
	PBMSG_HEAD2 h;
	BYTE btResult;
};

struct PMSG_TARGET_TELEPORT
{
	PBMSG_HEAD h;
	BYTE NumberH;	// 3
	BYTE NumberL;	// 4
	BYTE MapX;	// 5
	BYTE MapY;	// 6
};

struct PMSG_DURATION_MAGIC_SEND
{
	PBMSG_HEAD h;
	BYTE X;
	BYTE Y;
	BYTE Dir;
	BYTE MagicNumberH;
	BYTE NumberH;
	BYTE MagicNumberL;
	BYTE NumberL;

};

struct PMSG_USEITEM
{
	PBMSG_HEAD h;
	BYTE inventoryPos;	// 3
	BYTE invenrotyTarget;	// 4
	BYTE btItemUseType;	// 5
};

struct PMSG_USEEXPANSIONITEM
{
	PBMSG_HEAD h;
	BYTE Result;
};

struct PMSG_REQ_MOVEDEVILSQUARE
{
	PBMSG_HEAD h;
	BYTE SquareNumber;	// 3
	BYTE InvitationItemPos;	// 4
};


struct PMSG_REQ_DEVILSQUARE_REMAINTIME
{
	PBMSG_HEAD h;
	BYTE hEventType;	// 3
	BYTE btItemLevel;	// 4
};


struct PMSG_PING_RESULT
{
	PBMSG_HEAD h;
};


struct PMSG_REGEVENTCHIP
{
	PBMSG_HEAD h;
	BYTE Type;	// 3
	BYTE ChipPos;	// 4
};


struct PMSG_GETMUTONUMBER
{
	PBMSG_HEAD h;
};


struct PMSG_EXCHANGE_EVENTCHIP
{
	PBMSG_HEAD h;
	BYTE btType;	// [0x00:Change Renas] [0x01:Change Stones]	3
};

struct PMSG_SETQUEST 
{
	PBMSG_HEAD h;	// C1:A1
	BYTE QuestIndex;	// 3
	BYTE State;	// 4
};

struct PMSG_REQ_MOVEBLOODCASTLE
{
	PBMSG_HEAD h;
	BYTE iBridgeNumber;	// 3
	BYTE iItemPos;	// 4
};

struct PMSG_ANS_ENTER_ON_QUESTNPC
{
	PBMSG_HEAD2 h;
	BYTE btResult;
};

struct PMSG_REQ_MOVECHAOSCASTLE
{
	PBMSG_HEAD2 h;
	BYTE iChaosCastleNumber;	// 4
	BYTE iItemPos;	// 5
};
struct PMSG_REQ_MOVEILLUSIONTEMPLE
{
	PBMSG_HEAD2 h;
	BYTE ID;
	BYTE iItemPos;
};
struct PMSG_REQ_USEILLUSIONTEMPLESKILL
{
	PBMSG_HEAD2 h;
	BYTE MagicNumberH;
	BYTE MagicNumberL;
	BYTE btTargetObjIndexH;
	BYTE btTargetObjIndexL;
	BYTE btDis;
};

struct PMSG_REQ_REPOSUSER_IN_CC
{
	PBMSG_HEAD2 h;
	BYTE btPX;	// 4
	BYTE btPY;	// 5
};


struct PMSG_REQ_CL_ENTERCOUNT
{
	PBMSG_HEAD h;
	BYTE btEventType;	// 3
};


struct PMSG_REQ_2ANV_LOTTO_EVENT
{
	PBMSG_HEAD h;
	char SERIAL1[5];	// 3
	char SERIAL2[5];	// 8
	char SERIAL3[5];	// D
};


struct PMSG_REQ_MOVE_OTHERSERVER
{
	PBMSG_HEAD h;
	char LastJoominNumber[20];	// 3
};


struct PMSG_PACKETCHECKSUM
{
	PBMSG_HEAD h;
	BYTE funcindex;	// 3
	DWORD CheckSum;	// 4
};

struct PMSG_REQ_START_DUEL
{
	PBMSG_HEAD h;
	BYTE NumberH;	// 3
	BYTE NumberL;	// 4
	char szName[10];	// 5
};


struct PMSG_REQ_END_DUEL
{
	PBMSG_HEAD h;
};



struct PMSG_ANS_DUEL_OK
{
	PBMSG_HEAD h;
	bool bDuelOK;	// 3
	BYTE NumberH;	// 4
	BYTE NumberL;	// 5
	char szName[10];	// 6
};


struct PMSG_REQUEST_PET_ITEM_COMMAND
{
	PBMSG_HEAD h;
	BYTE PetType;	// 3
	BYTE Command;	// 4
	BYTE NumberH;	// 5
	BYTE NumberL;	// 6
};


struct PMSG_REQUEST_PET_ITEMINFO
{
	PBMSG_HEAD h;
	BYTE PetType;	// 3
	BYTE InvenType;	// 4
	BYTE nPos;	// 5
};

struct PMSG_SEND_PCPOINT
{
	PBMSG_HEAD2 h;
	WORD PCPoint;
	WORD MaxPCPoint;
};

struct PMSG_REQ_OPEN_PCPOINTSHOP
{ 
	PBMSG_HEAD2 h;
	BYTE Result;
};

struct PMSG_REQ_BUY_PCPOINTSHOP
{
	PBMSG_HEAD2 h;
	BYTE Pos;
};

struct PMSG_BUYRESULT
{
	PBMSG_HEAD h;	// C1:32
	BYTE Result;	// 3
	BYTE ItemInfo[MAX_ITEM_INFO];	// 4
};

struct PMSG_REQ_GUILDVIEWPORT
{
	PBMSG_HEAD h;
	int GuildNumber;	// 4
};

struct PMSG_SHOPITEMCOUNT
{
	PWMSG_HEAD h;	// C2:31
	BYTE Type;	// 4
	BYTE count;	// 5
};

struct PMSG_GUILD_ASSIGN_STATUS_REQ
{
	PBMSG_HEAD h;
	BYTE btType;	// 3
	BYTE btGuildStatus;	// 4
	char szTagetName[10];	// 5
};


struct PMSG_GUILD_ASSIGN_TYPE_REQ
{
	PBMSG_HEAD h;
	BYTE btGuildType;	// 3
};


struct PMSG_RELATIONSHIP_JOIN_BREAKOFF_REQ
{
	PBMSG_HEAD h;	// C1:E5
	BYTE btRelationShipType;	// 3
	BYTE btRequestType;	// 4
	BYTE btTargetUserIndexH;	// 5
	BYTE btTargetUserIndexL;	// 6
};


struct PMSG_RELATIONSHIP_JOIN_BREAKOFF_ANS
{
	PBMSG_HEAD h;	// C1:E6
	BYTE btRelationShipType;	// 3
	BYTE btRequestType;	// 4
	BYTE btResult;	// 5
	BYTE btTargetUserIndexH;	// 6
	BYTE btTargetUserIndexL;	// 7
};


struct PMSG_UNIONLIST_REQ {

  

  
  PBMSG_HEAD h;

  

  

  

  

  
};

struct PMSG_KICKOUT_UNIONMEMBER_REQ
{
	PBMSG_HEAD2 h;
	char szTargetGuildName[8];	// 4
};


struct PMSG_REQ_MAPSERVERAUTH
{
	PBMSG_HEAD2 h;
	char szAccountID[12];	// 4
	char szCharName[12];	// 10
	int iJoinAuthCode1;	// 1C
	int iJoinAuthCode2;	// 20
	int iJoinAuthCode3;	// 24
	int iJoinAuthCode4;	// 28
	int iTickCount;	// 2C
	BYTE btCliVersion[5];	// 30
	BYTE btCliSerial[16];	// 35

};

struct PMSG_REQ_CASTLESIEGESTATE
{
	PBMSG_HEAD2 h;
};

struct PMSG_REQ_REGCASTLESIEGE
{
	PBMSG_HEAD2 h;  
};

struct PMSG_REQ_GIVEUPCASTLESIEGE
{
	PBMSG_HEAD2 h;
	BYTE btGiveUp;
};

struct PMSG_REQ_GUILDREGINFO
{
	PBMSG_HEAD2 h;
};

struct CSP_ANS_GUILDREGINFO
{
	PBMSG_HEAD2 h;
	int iResult;	// 4
	WORD wMapSvrNum;	// 8
	int iIndex;	// C
	char szGuildName[8];	// 10
	int iRegMarkCount;	// 18
	BYTE bIsGiveUp;	// 1C
	BYTE btRegRank;	// 1D
};

struct PMSG_REQ_REGGUILDMARK
{
	PBMSG_HEAD2 h;
	BYTE btItemPos;
};

struct CSP_ANS_GUILDREGMARK
{
	struct PBMSG_HEAD2 h;
	int iResult;
	WORD wMapSvrNum;
	int iIndex;
	char szGuildName[8];
	int iItemPos;
	int iRegMarkCount;
};

struct PMSG_REQ_NPCBUY
{
	PBMSG_HEAD2 h;
	int iNpcNumber;
	int iNpcIndex;
};

struct PMSG_REQ_NPCREPAIR
{
	PBMSG_HEAD2 h;
	int iNpcNumber;
	int iNpcIndex;
};

struct PMSG_REQ_NPCUPGRADE
{
	PBMSG_HEAD2 h;
	int iNpcNumber;
	int iNpcIndex;
	int iNpcUpType;
	int iNpcUpValue;
};

struct PMSG_REQ_TAXMONEYINFO
{
	PBMSG_HEAD2 h;
};

struct PMSG_REQ_TAXRATECHANGE
{
	PBMSG_HEAD2 h;
	BYTE btTaxType;
	BYTE btTaxRate1;
	BYTE btTaxRate2;
	BYTE btTaxRate3;
	BYTE btTaxRate4;
};

struct PMSG_REQ_MONEYDRAWOUT
{
	PBMSG_HEAD2 h;
	BYTE btMoney1;
	BYTE btMoney2;
	BYTE btMoney3;
	BYTE btMoney4;
};

struct PMSG_REQ_CSGATEOPERATE
{
	PBMSG_HEAD2 h;
	BYTE btOperate;
	BYTE btIndex1;
	BYTE btIndex2;
};

struct PMSG_REQ_MINIMAPDATA
{
	PBMSG_HEAD2 h;
};

struct PMSG_REQ_STOPMINIMAPDATA
{
	PBMSG_HEAD2 h;
};

struct PMSG_REQ_CSCOMMAND
{
	PBMSG_HEAD2 h;
	BYTE btTeam;
	BYTE btX;
	BYTE btY;
	BYTE btCommand;
};

struct PMSG_REQ_CSHUNTZONEENTER
{
	PBMSG_HEAD2 h;
	BYTE btHuntZoneEnter;
};

struct PMSG_REQ_NPCDBLIST
{
	PBMSG_HEAD h;
	BYTE btMonsterCode;
};

struct PMSG_REQ_CSREGGUILDLIST
{
	PBMSG_HEAD h;
};

struct PMSG_REQ_CSATTKGUILDLIST
{
	PBMSG_HEAD h;
};

struct PMSG_REQ_USEWEAPON
{
	PBMSG_HEAD2 h;
	BYTE btObjIndexH;
	BYTE btObjIndexL;
	BYTE btTargetIndex;
};

struct PMSG_REQ_WEAPON_DAMAGE_VALUE
{
	PBMSG_HEAD2 h;
	BYTE btObjIndexH;
	BYTE btObjIndexL;
};

struct PMSG_REQ_GUILDMARK_OF_CASTLEOWNER
{
	PBMSG_HEAD2 h;
};

struct PMSG_REQ_JEWEL_MIX
{
	PBMSG_HEAD2 h;
	BYTE btJewelType;
	BYTE btJewelMix;
};

struct PMSG_REQ_JEWEL_UNMIX
{
	PBMSG_HEAD2 h;
	BYTE btJewelType;
	BYTE btJewelLevel;
	BYTE btJewelPos;
};

struct PMSG_REQ_CRYWOLF_INFO
{
	PBMSG_HEAD2 h;
};

struct PMSG_REQ_CRYWOLF_ALTAR_CONTRACT
{
	PBMSG_HEAD2 h;
	BYTE btObjIndexH;
	BYTE btObjIndexL;
};

struct PMSG_REQ_CRYWOLF_BENEFIT_PLUS_CHAOSRATE
{
	PBMSG_HEAD2 h;
};

struct PMSG_REQ_KANTURU_STATE_INFO
{
	PBMSG_HEAD2 h;
};

struct PMSG_REQ_ENTER_KANTURU_BOSS_MAP
{
	PBMSG_HEAD2 h;
};

struct CSP_ANS_CSINITDATA
{
	PWMSG_HEAD h;	// 0
	int iResult;		// 4
	WORD wMapSvrNum;	// 8
	WORD wStartYear;	// a
	BYTE btStartMonth;	// c
	BYTE btStartDay;	// d
	WORD wEndYear; // e
	BYTE btEndMonth; // 10
	BYTE btEndDay;	// 11
	BYTE btIsSiegeGuildList;	// 12
	BYTE btIsSiegeEnded;	// 13
	BYTE btIsCastleOccupied;	// 14
	char szCastleOwnGuild[8]; // 15
	__int64 i64CastleMoney; // 20
	int iTaxRateChaos; // 28
	int iTaxRateStore; // 2c
	int iTaxHuntZone; // 30
	int iFirstCreate; // 34
	int iCount; // 38
};

struct CSP_CSINITDATA
{
	int iNpcNumber; // 0
	int iNpcIndex; // 4
	int iNpcDfLevel; // 8
	int iNpcRgLevel; // c
	int iNpcMaxHp; // 10
	int iNpcHp; // 14
	BYTE btNpcX; // 18
	BYTE btNpcY; // 19
	BYTE btNpcDIR; // 1a
};

struct CSP_CALCREGGUILDLIST
{
	char szGuildName[8]; // 0
	int iRegMarkCount; // 8
	int iGuildMemberCount; // c
	int iGuildMasterLevel; // 10
	int iSeqNum; // 14
};

struct CSP_CSGUILDUNIONINFO
{
	char szGuildName[8]; // 0
	int iCsGuildID; // 8
};

struct CSP_CSLOADTOTALGUILDINFO
{
	char szGuildName[8]; // 0
	int iCsGuildID; // 8
	int iGuildInvolved; // c
	int iGuildScore; //10 season 2.5 add-on
};

struct PMSG_CSATTKGUILDLIST
{
	BYTE btCsJoinSide; // 0
	BYTE btGuildInvolved; // 1
	char szGuildName[8]; // 2
	int iGuildScore; //A season 2.5 add-on
};

struct CSP_ANS_NPCDATA
{
	PWMSG_HEAD h; // 0
	int iResult; // 4
	WORD wMapSvrNum; // 8
	int iIndex; // c
	int iCount; // 10
};

struct PMSG_ANS_NOTIFYCSSTART
{
	PBMSG_HEAD2 h; // 0
	BYTE btStartState; // 4
};

struct PMSG_REQ_MOVE_TO_CASTLE_HUNTZONE
{
    PBMSG_HEAD2 h;
    int iPrice;
};

struct PMSG_ANS_CASTLESIEGESTATE
{
	PBMSG_HEAD2 h;
	BYTE btResult;
	char cCastleSiegeState;
	BYTE btStartYearH;
	BYTE btStartYearL;
	BYTE btStartMonth;
	BYTE btStartDay;
	BYTE btStartHour;
	BYTE btStartMinute;
	BYTE btEndYearH;
	BYTE btEndYearL;
	BYTE btEndMonth;
	BYTE btEndDay;
	BYTE btEndHour;
	BYTE btEndMinute;
	BYTE btSiegeStartYearH;
	BYTE btSiegeStartYearL;
	BYTE btSiegeStartMonth;
	BYTE btSiegeStartDay;
	BYTE btSiegeStartHour;
	BYTE btSiegeStartMinute;
	char cOwnerGuild[8];
	char cOwnerGuildMaster[10];
	BYTE btStateLeftSec1;
	BYTE btStateLeftSec2;
	BYTE btStateLeftSec3;
	BYTE btStateLeftSec4;
};

struct PMSG_ANS_REGCASTLESIEGE
{
	PBMSG_HEAD2 h;
	BYTE btResult;
	char szGuildName[8];
};

struct PMSG_ANS_GIVEUPCASTLESIEGE
{
	PBMSG_HEAD2 h;
	BYTE btResult;
	BYTE btIsGiveUp;
	char szGuildName[8];
};

struct PMSG_ANS_GUILDREGINFO
{
	PBMSG_HEAD2 h;
	BYTE btResult;
	char szGuildName[8];
	BYTE btGuildMark1;
	BYTE btGuildMark2;
	BYTE btGuildMark3;
	BYTE btGuildMark4;
	BYTE btIsGiveUp;
	BYTE btRegRank;
};

struct PMSG_ANS_REGGUILDMARK
{
	PBMSG_HEAD2 h;
	BYTE btResult;
	char szGuildName[8];
	BYTE btGuildMark1;
	BYTE btGuildMark2;
	BYTE btGuildMark3;
	BYTE btGuildMark4;
};

struct PMSG_ANS_NPCBUY
{
	PBMSG_HEAD2 h;
	BYTE btResult;
	int iNpcNumber;
	int iNpcIndex;
};

struct PMSG_ANS_NPCREPAIR
{
	PBMSG_HEAD2 h;
	BYTE btResult;
	int iNpcNumber;
	int iNpcIndex;
	int iNpcHP;
	int iNpcMaxHP;
};

struct PMSG_ANS_NPCUPGRADE
{
	PBMSG_HEAD2 h;
	BYTE btResult;
	int iNpcNumber;
	int iNpcIndex;
	int iNpcUpType;
	int iNpcUpValue;
};

struct PMSG_ANS_TAXMONEYINFO
{
	PBMSG_HEAD2 h;
	BYTE btResult;
	BYTE btTaxRateChaos;
	BYTE btTaxRateStore;
	BYTE btMoney1;
	BYTE btMoney2;
	BYTE btMoney3;
	BYTE btMoney4;
	BYTE btMoney5;
	BYTE btMoney6;
	BYTE btMoney7;
	BYTE btMoney8;
};

struct PMSG_ANS_TAXRATECHANGE
{
	PBMSG_HEAD2 h;
	BYTE btResult;
	BYTE btTaxType;
	BYTE btTaxRate1;
	BYTE btTaxRate2;
	BYTE btTaxRate3;
	BYTE btTaxRate4;
};

struct PMSG_ANS_MONEYDRAWOUT
{
	PBMSG_HEAD2 h;
	BYTE btResult;
	BYTE btMoney1;
	BYTE btMoney2;
	BYTE btMoney3;
	BYTE btMoney4;
	BYTE btMoney5;
	BYTE btMoney6;
	BYTE btMoney7;
	BYTE btMoney8;
};

struct PMSG_ANS_CSGATESTATE
{
	PBMSG_HEAD2 h;
	BYTE btResult;
	BYTE btIndex1;
	BYTE btIndex2;
};

struct PMSG_ANS_CSGATEOPERATE
{
	PBMSG_HEAD2 h;
	BYTE btResult;
	BYTE btOperate;
	BYTE btIndex1;
	BYTE btIndex2;
};

struct PMSG_ANS_CSGATECURSTATE
{
	PBMSG_HEAD2 h;
	BYTE btOperate;
	BYTE btIndex1;
	BYTE btIndex2;
};

struct PMSG_ANS_NOTIFYSWITCHPROC
{
	PBMSG_HEAD2 h;
	BYTE btIndex1;
	BYTE btIndex2;
	BYTE btUserIndex1;
	BYTE btUserIndex2;
	BYTE btSwitchState;
};

struct PMSG_ANS_NOTIFYCROWNPROC
{
	PBMSG_HEAD2 h;
	BYTE btCrownState;
	DWORD dwAccumulatedCrownAccessTime;
};

struct PMSG_ANS_CSCOMMAND
{
	PBMSG_HEAD2 h;
	BYTE btTeam;
	BYTE btX;
	BYTE btY;
	BYTE btCommand;
};

struct PMSG_ANS_NPCDBLIST
{
	PWMSG_HEAD h;
	BYTE btResult;
	int iCount;
};

struct PMSG_ANS_CSATTKGUILDLIST
{
	PWMSG_HEAD h;
	BYTE btResult;
	int iCount;
};

struct PMSG_NOTIFY_OBJECT_CREATION_STATE
{
	struct PBMSG_HEAD2 h;
	BYTE btObjIndexH;
	BYTE btObjIndexL;
	BYTE btCreationState;
};

struct PMSG_ANS_MOVE_TO_CASTLE_HUNTZONE
{
    PBMSG_HEAD2 h; // +0x0(0x4)
    BYTE btResult; // +0x4(0x1)
};

struct PMSG_ANS_GUILDMARK_OF_CASTLEOWNER
{
	PBMSG_HEAD2 h;	// C1:B9:02
	BYTE GuildMarkOfCastleOwner[32];	// 4
};

struct PMSG_ANS_USEWEAPON
{
	PBMSG_HEAD2 h;	// C1:B7:01
	BYTE btResult;	// [1:Attacker] [2:Targets]	4
	BYTE btObjIndexH;	// 5
	BYTE btObjIndexL;	// 6
	BYTE btWeaponType;	// [1:Slingshot attack] [2:Slingshot defense]	7
	BYTE btPointX;	// 8
	BYTE btPointY;	// 9
};

struct PMSG_ANS_CSHUNTZONEENTER
{
	PBMSG_HEAD2 h;	// C1:B2:1F
	BYTE btResult;	// 4
	BYTE btHuntZoneEnter;	// 5
};

struct PMSG_ANS_NOTIFYCSPROGRESS
{
	PBMSG_HEAD2 h;	// C1:B2:18
	BYTE btCastleSiegeState;	// 4
	BYTE szGuildName[8];	// 5
};

struct PMSG_ANS_JEWEL_UNMIX
{
	PBMSG_HEAD2 h;	// C1:BC:[00:01]
	BYTE btResult;	// 4
};

struct PMSG_ANS_MINIMAPDATA
{
	PBMSG_HEAD2 h;
	BYTE btResult;	// 4
};

struct PMSG_ANS_CSLEFTTIMEALARM
{
	PBMSG_HEAD2 h;	// C1:B2:1E
	BYTE btHour;	// 4
	BYTE btMinute;	// 5
};

struct PMSG_REQ_LUCKYCOIN_REGISTER
{
	PBMSG_HEAD2 h;
};

struct PMSG_ANS_LUCKYCOIN_REGISTER
{
	PBMSG_HEAD2 h;
	unsigned char btResult;
	int iLuckyCoin;
};

struct PMSG_ANS_LUCKYCOININFO
{
	PBMSG_HEAD2 h;
	int iLuckyCoin;
};

struct PMSG_REQ_LUCKYCOIN_TRADE
{
	PBMSG_HEAD2 h;
	int iLuckyCoinTradeCount;
};

struct PMSG_MUBOT_SWITCH
{
	PBMSG_HEAD2 h;
	BYTE Status;
};

struct _tagPMSG_REQ_INVENTORY_EQUIPMENT_ITEM
{
	PBMSG_HEAD2 h;
	BYTE btItemPos;
	BYTE btValue;
};

struct _tagPMSG_ANS_INVENTORY_EQUIPMENT_ITEM
{
	PBMSG_HEAD2 h;
	BYTE btItemPos;
	BYTE btResult;
};

struct PMSG_MUBOT_USE
{
	PBMSG_HEAD2 h;
	BYTE Time;
	BYTE TimeMultipler;
	DWORD Money;
	BYTE Status;
};

#pragma pack (1)
struct PMSG_ANS_LUCKYCOIN_TRADE
{
	PBMSG_HEAD2 h;
	BYTE btResult;
};

struct PMSG_MUBOT_DATASAVE
{
	PWMSG_HEAD h;
	BYTE MuBotData[257];
};

#pragma pack ()



#pragma pack (1)

struct PMSG_TELEPORT_RESULT
{
	PBMSG_HEAD h;	// C3:1C
	BYTE Unk;
	WORD Type;
	BYTE MapNumber;	// 4
	BYTE MapX;	// 5
	BYTE MapY;	// 6
	BYTE Dir;	// 7
};

struct PMSG_REQ_ENTER_DOPPELGANGER
{
	PBMSG_HEAD2 h;
	BYTE btPos;
};

struct PMSG_REQ_ENTER_ZONE
{
	PBMSG_HEAD2 h;
	BYTE btResult;
};

struct PMSG_NOTIFY_ZONE_TIME
{
	PBMSG_HEAD2 h;
	BYTE m_btMsgType;
	BYTE m_btDayOfWeek;
	WORD m_wZoneIndex;
	DWORD m_dwRemainTime;
	DWORD m_dwRemainMonster;
};

//BYTE Packet[13] = {0xC1, 0x0D, 0xF7, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
struct PMSG_NOTIFY_ZONE_CLEAR
{
	PBMSG_HEAD2 h;
	DWORD m_nType;
	DWORD m_nRewardExp;
};
struct PMSG_ENTER_ZONE_RESULT
{
	PBMSG_HEAD2 h;
	BYTE m_btResult;
	BYTE m_btDayOfWeek;
	BYTE m_btZoneIndex;
	BYTE m_btWeather;
	DWORD m_dwRemainTime;
};

struct PMSG_RAGE_ATTACK_RANGE
{
	PBMSG_HEAD h;
	BYTE MagicNumberH;
	BYTE MagicNumberL;
	BYTE TargetIndexH;
	BYTE TargetIndexL;
};

struct PMSG_RAGEATTACK
{
	PBMSG_HEAD h;
	BYTE SourceNumberH;
	BYTE SourceNumberL;
	BYTE MagicNumberH;
	BYTE MagicNumberL;
	BYTE TargetNumberH;
	BYTE TargetNumberL;
};

#pragma pack (1)
struct PMSG_RAGE_ATTACK_RANGE_ANS
{
	PBMSG_HEAD2 h;
	WORD MagicNumber;
	WORD Target[5];
};

struct PMSG_TARGET_HP
{
	PBMSG_HEAD2 h; // C1:FA:05
	WORD m_Index;
	DWORD m_Hp;
	DWORD m_MaxHp;
	WORD m_Level;
	bool m_bIceEffect;
	bool m_bPoisonEffect;
};

struct PMSG_ADDON_REQUEST
{
	BYTE HEAD;
	BYTE SIZE;
	BYTE OPCODE;
	BYTE OPCODE2;
	WORD M_ADDON;
	DWORD M_CRC1;
	DWORD M_CRC2;
};

struct PMSG_TARGET_HP_INFO
{
	WORD m_Index;
	DWORD m_Hp;
	DWORD m_MaxHp;
	WORD m_Level;
	bool m_bIceEffect;
	bool m_bPoisonEffect;
};


struct PMSG_TARGET_HP_DELETE
{
	WORD m_Index;
};

struct PMSG_SEND_AH_INFO{
	PBMSG_HEAD2 h;
	BYTE type; // 0 - hack detected; 1 - Agi
	DWORD Agi;
	DWORD MainTick;
};

struct PMSG_REQ_CRCDATA
{
	PBMSG_HEAD2 h;
	DWORD MainExe;
	DWORD IGCDLL;
	DWORD PlayerBMD;
	DWORD HackDB;
	DWORD SkillCRC;
};

#pragma pack ()

struct PMSG_REQ_REFINE_PENTAGRAM_JEWEL
{
	PBMSG_HEAD2 h;
	BYTE btRefineKind;
};

struct PMSG_REQ_UPGRADE_PENTAGRAM_JEWEL
{
	PBMSG_HEAD2 h;
	BYTE btUpgradeType;
	BYTE btTargetValue;
};

struct PMSG_OBJECT_LIFE_INFO
{
	PBMSG_HEAD2 h;
	BYTE ObjNumberH;
	BYTE ObjNumberL;
	BYTE MaxLifeHH;
	BYTE MaxLifeHL;
	BYTE MaxLifeLH;
	BYTE MaxLifeLL;
	BYTE CurLifeHH;
	BYTE CurLifeHL;
	BYTE CurLifeLH;
	BYTE CurLifeLL;
};

#pragma pack (1)
struct PMSG_PRICE_INFO
{
	PWMSG_HEAD h;	// C2:95
	BYTE subc;
	BYTE Type;	// 4
	BYTE count;	// 5
};

struct PMSG_PRICE_ITEM_LIST
{
	BYTE Pos;
	UINT64 SellPrice;
	UINT64 BuyPrice;
};

#pragma pack ()
// NewPVP Protocol
/************************************************************************/
/* STRUCTS BELOW FOR PROTOCOL CLIENT ONLY                               */
/************************************************************************/
typedef struct _tagPMSG_REQ_DUEL_INVITE
{
	PBMSG_HEAD2 h;
	BYTE NumberH;
	BYTE NumberL;
	char szName[MAX_ACCOUNT_LEN];
} PMSG_REQ_DUEL_INVITE, *LPPMSG_REQ_DUEL_INVITE;

typedef struct _tagPMSG_ANS_DUEL_ANSWER
{
	PBMSG_HEAD2 h;
	BYTE bDuelOK;
	BYTE NumberH;
	BYTE NumberL;
} PMSG_ANS_DUEL_ANSWER, *LPPMSG_ANS_DUEL_ANSWER;

typedef struct _tagPMSG_REQ_DUEL_EXIT
{
	PBMSG_HEAD2 h;
} PMSG_REQ_DUEL_EXIT, *LPPMSG_REQ_DUEL_EXIT;

typedef struct _tagPMSG_REQ_DUEL_JOINCNANNEL
{
	PBMSG_HEAD2 h;
	BYTE nChannelId;
} PMSG_REQ_DUEL_JOINCNANNEL, *LPPMSG_REQ_DUEL_JOINCNANNEL;

typedef struct _tagPMSG_REQ_DUEL_LEAVECNANNEL
{
	PBMSG_HEAD2 h;
	BYTE nChannelId;
} PMSG_REQ_DUEL_LEAVECNANNEL, *LPPMSG_REQ_DUEL_LEAVECNANNEL;

struct _tagPMSG_REQ_ARCA_BATTLE_JOIN
{
  PBMSG_HEAD2 h;
};

struct _tagPMSG_REQ_ARCA_BATTLE_ENTER
{
  PBMSG_HEAD2 h;
  char btEnterSeq;
};

struct _tagPMSG_REQ_ACHERON_ENTER
{
  PBMSG_HEAD2 h;
};

struct _tagPMSG_ANS_ACHERON_ENTER
{
  PBMSG_HEAD2 h;
  char btResult;
};

struct PMSG_REQ_MASTERLEVEL_SKILL
{
    PBMSG_HEAD2 h; //C1:F3:52
	int iMasterLevelSkill;
};

struct PMSG_SETCHARSET
{
	PBMSG_HEAD h;
	BYTE subcode;
	WORD charset;
};

struct PMSG_SETAGILITYBUG
{
	PBMSG_HEAD h;
	BYTE subcode;
	BYTE value;
};

struct PMSG_CHAIN_MAGIC
{
	PBMSG_HEAD2 h;
	BYTE MagicNumberH;
	BYTE MagicNumberL;
	WORD wUserIndex;
	BYTE nCount;
};

struct PMSG_CHAIN_MAGIC_OBJECT
{
	WORD nTargetIndex;
};

struct PMSG_RECV_TRAP // it must similar to any "correct" packet
{
	PBMSG_HEAD2 h;
	WORD wUserIndex;
	time_t CurrTime;
};

// Season 8 Matching System
// Guild Matching Packets

struct _stGuildMatchingList
{
	char szMemo[41];
	char szRegistrant[MAX_ACCOUNT_LEN + 1];
	char szGuildName[MAX_GUILD_LEN + 1];
	BYTE btGuildMemberCnt;
	BYTE btGuildMasterClass;
	BYTE btInterestType;
	BYTE btLevelRange;
	BYTE btClassType;
	int nGuildMasterLevel;
	int nBoardNumber;
	int nGuildNumber;
	BYTE btGensType;
};

struct PMSG_REQ_GUILDMATCHINGLIST
{
	PBMSG_HEAD2 h;
	int nPage;
};

struct PMSG_REQ_GUILDMATCHINGLISTSEARCHWORD
{
	PBMSG_HEAD2 h;
	int nPage;
	char szSearchWord[11];
};

struct PMSG_REQ_REGGUILDMATCHINGDATA
{
	PBMSG_HEAD2 h;
	char szMemo[41];
	BYTE btInterestType;
	BYTE btLevelRange;
	BYTE btClassType;
};

struct PMSG_REQ_CANCEL_GUILDMATCHINGLIST
{
	PBMSG_HEAD2 h;
};

struct PMSG_REQ_JOIN_GUILDMATCHING
{
	PBMSG_HEAD2 h;
	int nGuildNumber;
};

struct PMSG_REQ_ALLOW_JOIN_GUILDMATCHING
{
	PBMSG_HEAD2 h;
	int nAllowType;
	char szName[11];
};

struct PMSG_REQ_ALLOWLIST_GUILDMATCHING
{
	PBMSG_HEAD2 h;
};

struct PMSG_ANS_GUILDMATCHINGLIST
{
	PWMSG_HEAD2 h;
	int nPage;
	int nTotalPage;
	int nListCount;
	int nResult;
	_stGuildMatchingList stGuildMatchingList;
};

struct PMSG_ANS_REGGUILDMATCHINGDATA
{
	PBMSG_HEAD2 h;
	int nResult;
};

struct PMSG_ANS_CANCEL_JOIN_GUILDMATCHING
{
	PBMSG_HEAD2 h;
	int nResult;
};

struct PMSG_ANS_CANCEL_GUILDMATCHINGLIST
{
	PBMSG_HEAD2 h;
	int nResult;
};

struct PMSG_ANS_JOIN_GUILDMATCHING
{
	PBMSG_HEAD2 h;
	int nResult;
};

struct PMSG_ANS_ALLOW_JOIN_GUILDMATCHING
{
	PBMSG_HEAD2 h;
	char szMemberName[MAX_ACCOUNT_LEN + 1];
	int nAllowType;
	int nResult;
};

struct PMSG_ANS_WAIT_GUILDMATCHING
{
	PWMSG_HEAD2 h;
	int nListCount;
	int nResult;
};

struct PMSG_ANS_ALLOWLIST_STATE_GUILDMATCHING
{
	PBMSG_HEAD2 h;
	char szGuildMasterName[MAX_ACCOUNT_LEN + 1];
	char szGuildName[MAX_GUILD_LEN + 1];
	int nResult;
};

struct PMSG_ANS_NOTI_GUILDMATCHING
{
	PBMSG_HEAD2 h;
	int nResult;
};

struct PMSG_ANS_NOTI_GUILDMATCHING_FORGUILDMASTER
{
	PBMSG_HEAD2 h;
	int nResult;
};

// Party Matching

struct _PARTY_INFO_LISTDB
{
	char szLeaderName[MAX_ACCOUNT_LEN + 1];
	char szTitle[41];
	char szPassword[5];
	WORD nMinLevel;
	WORD nMaxLevel;
	WORD nHuntingGround;
	int nLeaderLevel;
	BYTE btLeaderClass;
	BYTE btWantedClass;
	BYTE btCurPartyMemberCnt;
	BYTE btUsePassword;
	BYTE btWantedClassDetailInfo[7];
	BYTE btGensType;
	BYTE btApprovalType;
};

struct _PARTY_INFO_LIST
{
	char szLeaderName[MAX_ACCOUNT_LEN + 1];
	char szTitle[41];
	WORD nMinLevel;
	WORD nMaxLevel;
	WORD nHuntingGround;
	int nLeaderLevel;
	BYTE btLeaderClass;
	BYTE btWantedClass;
	BYTE btCurPartyMemberCnt;
	BYTE btUsePassword;
	BYTE btWantedClassDetailInfo[7];
	int nServerChannel;
	BYTE btGensType;
};

struct PMSG_ANS_REG_WANTED_PARTYMEMBER
{
	PBMSG_HEAD2 h;
	int nResult;
};

struct PMSG_ANS_GET_PARTYMATCHINGLIST
{
	PWMSG_HEAD2 h;
	int nListCount;
	int nPage;
	int nTotalPage;
	int nResult;
};

struct PMSG_ANS_MEMBERJOIN_PARTYMATCHINGLIST
{
	PBMSG_HEAD2 h;
	int nResult;
};

struct PMSG_ANS_MEMBERJOIN_STATELIST_PARTYMATCHING
{
	PBMSG_HEAD2 h;
	int nResult;
	int nLeaderChannel;
	char szLeaderName[MAX_ACCOUNT_LEN + 1];
};

struct PMSG_ANS_WAITLIST_PARTYMATCHING
{
	PWMSG_HEAD2 h;
	int nListCnt;
	int nResult;
};

struct PMSG_ANS_CANCEL_JOIN_PARTYMATCHING
{
	PBMSG_HEAD2 h;
	BYTE btType;
	int nResult;
};

struct PMSG_ANS_ACCEPTMEMBER_PARTYMATCHING
{
	PBMSG_HEAD2 h;
	char szMemberName[MAX_ACCOUNT_LEN + 1];
	BYTE btType;
	BYTE btFlag;
	int nResult;
};

struct PMSG_ANS_NOTI_JOINPARTYMATCHING
{
	PBMSG_HEAD2 h;
};

struct PMSG_REQ_REG_WANTED_PARTYMEMBER
{
	PBMSG_HEAD2 h;
	char szTitle[41];
	char szPassword[5];
	WORD nMinLevel;
	WORD nMaxLevel;
	WORD nHuntingGround;
	BYTE btWantedClass;
	BYTE btWantedClassDetailInfo[7];
	BYTE btUsePassword;
	BYTE btApprovalType;
};

struct PMSG_REQ_GET_PARTYMATCHINGLIST
{
	PBMSG_HEAD2 h;
	int nPage;
	char szSearchWord[11];
	BYTE btUseSearchWord;
};

struct PMSG_REQ_MEMBERJOIN_PARTYMATCHINGLIST
{
	PBMSG_HEAD2 h;
	char szLeaderName[MAX_ACCOUNT_LEN + 1];
	char szPassword[5];
	BYTE btUsePassword;
	BYTE btUseRandomParty;
};

struct PMSG_REQ_MEMBERJOIN_STATELIST_PARTYMATCHING
{
	PBMSG_HEAD2 h;
};

struct PMSG_REQ_WAITLIST_PARTYMATCHING
{
	PBMSG_HEAD2 h;
};

struct PMSG_REQ_ACCEPTMEMBER_PARTYMATCHING
{
	PBMSG_HEAD2 h;
	char szMemberName[MAX_ACCOUNT_LEN + 1];
	BYTE btType;
};

struct PMSG_REQ_CANCEL_JOIN_PARTYMATCHING
{
	PBMSG_HEAD2 h;
	BYTE btType;
};

struct PMSG_ANS_PARTYMATCHING_ERROR
{
	PBMSG_HEAD2 h;
	int iResult;
};

// Chaos Castle Survival of the Fittest
struct PMSG_CCF_UI_ONOFF
{
	PBMSG_HEAD2 h;
};

struct PMSG_REQ_REPOSUSER_IN_CCF
{
	PBMSG_HEAD2 h;
	BYTE btPX;	// 4
	BYTE btPY;	// 5
};

struct PMSG_REQ_CCF_RANKING
{
	PBMSG_HEAD2 h;
	BYTE byRankingType;
};

#pragma pack (1)
struct PMSG_RESULT_CCF_ENTERCHECK
{
	PBMSG_HEAD2 h;
	BYTE byResult;
	BYTE byRemainTime;
	BYTE byRemainTime_LOW;
};
#pragma pack()

// Muun
struct PMSG_MUUNITEM_DUR
{
	PBMSG_HEAD2 h;
	BYTE IPos;
	BYTE Dur;
};
struct PMSG_MUUN_INVEN_SUB_EQUIP_OPEN
{
	PBMSG_HEAD2 h;
	BYTE Result;
};

struct PMSG_MUUNITEM_CONDITION_STATUS
{
	PBMSG_HEAD2 h;
	BYTE btIPos;
	BYTE btStatus;
};

struct PMSG_USEITEM_MUUN_INVEN
{
	PBMSG_HEAD2 h;
	BYTE inventoryPos;
	BYTE invenrotyTarget;
	BYTE btItemUseType;
};
struct PMSG_USEITEM_MUUN_INVEN_RESULT
{
	PBMSG_HEAD2 h;
	BYTE btResult;
	BYTE btItemUseType;
};

struct PMSG_USER_MUUN_EQUIPMENT_CHANGED
{
	PBMSG_HEAD2 h;
	BYTE NumberH;
	BYTE NumberL;
	BYTE ItemInfo[12];
};

struct PMSG_PETINVENTORY_DELETE
{
	PBMSG_HEAD2 h;
	BYTE IPos;
	BYTE Flag;
};

struct PMSG_MUUN_INVEN_ITEM_MOD
{
	PBMSG_HEAD2 h;
	BYTE subcode;
	BYTE Pos;
	BYTE ItemInfo[12];
};

struct PMSG_MUUNITEM_GETREQUEST
{
	PBMSG_HEAD2 h;
	BYTE NumberH;
	BYTE NumberL;
};

struct PMSG_MUUNITEM_GETRESULT
{
	PBMSG_HEAD2 h;
	BYTE result;
	BYTE Data[12];
};

struct PMSG_MUUN_ITEM_SELLRESULT
{
	PBMSG_HEAD2 h;
	BYTE Result;
	DWORD Money;
};

struct PMSG_MUUN_ITEM_SELLREQUEST
{
	PBMSG_HEAD2 h;
	BYTE Pos;
};

// Minimap Party member pos

struct stMemberPosInfo
{
	char szName[MAX_ACCOUNT_LEN + 1];
	BYTE btMapNumber;
	BYTE btPosX;
	BYTE btPosY;
};

struct PMSG_MEMBER_POS_INFO_SEND
{
	PBMSG_HEAD2 h;
	BYTE btCount;
	stMemberPosInfo stPosInfo[5];
};

struct PMSG_REQ_NPC_POSITION
{
	PBMSG_HEAD2 h;
	BYTE btType;
};

// Event Inventory
struct PMSG_EVENTITEM_GETREQUEST
{
	PBMSG_HEAD2 h;
	BYTE NumberH;
	BYTE NumberL;
};

struct PMSG_EVENTITEM_GETRESULT
{
	PBMSG_HEAD2 h;
	BYTE result;
	BYTE Data[12];
};

struct PMSG_EVENTITEM_THROW
{
	PBMSG_HEAD2 h;
	BYTE px;
	BYTE py;
	BYTE Ipos;
};

struct PMSG_EVENTITEM_THROW_RESULT
{
	PBMSG_HEAD2 h;
	unsigned char Result;
	BYTE Ipos;
};

struct PMSG_EVENTINVENTORY_DELETE
{
	PBMSG_HEAD2 h;
	BYTE IPos;
	BYTE Flag;
};

struct PMSG_EVENTITEM_DUR
{
	PBMSG_HEAD2 h;
	BYTE IPos;
	BYTE Dur;
};

struct PMSG_EVENT_INVEN_ITEM_MOD
{
	PBMSG_HEAD2 h;
	BYTE Pos;
	BYTE ItemInfo[12];
};

// Exp info
struct PMSG_RES_EXPEVENTINFO
{
	PBMSG_HEAD2 h;
	WORD wPCBangRate;
	WORD wExpRate;
};

// Mining System

struct PMSG_REQ_MINETWINKLE
{
	PBMSG_HEAD2 h;
	WORD wUserIndex;
	WORD wTwinkleIndex;
	WORD wTwinkleType;
	BYTE byMapNumber;
};

struct PMSG_ANS_MINETWINKLE
{
	PBMSG_HEAD2 h;
	WORD wUserIndex;
	WORD wTwinkleIndex;
	WORD wTwinkleType;
	BYTE byMapNumber;
	BYTE byCurrentStage;
	int iRewardJewelNumber;
	int iResult;
};

struct PMSG_REQ_MINETWINKLE_REWARD
{
	PBMSG_HEAD2 h;
	WORD wUserIndex;
	WORD wTwinkleIndex;
	WORD wTwinkleType;
	BYTE byMapNumber;
	BYTE byIsStopMine;
};

struct PMSG_ANS_MINETWINKLE_REWARD
{
	PBMSG_HEAD2 h;
	WORD wUserIndex;
	WORD wTwinkleIndex;
	WORD wTwinkleType;
	BYTE byMapNumber;
	BYTE byCurrentStage;
	int iResult;
};

struct PMSG_ANS_MINETWINKLE_END_ANIMATION
{
	PBMSG_HEAD2 h;
	WORD wUserIndex;
	WORD wTwinkleIndex;
	BYTE byMapNumber;
};

struct PMSG_ANS_MINETWINKLE_CANCEL
{
	PBMSG_HEAD2 h;
	WORD wUserIndex;
	WORD wTwinkleIndex;
	WORD wTwinkleType;
	BYTE byMapNumber;
	BYTE byCurrentStage;
};

// Illusion Temple Renewal
struct PMSG_ITL_RELICS_GET_USER
{
	PBMSG_HEAD2 h;
	WORD wUserIndex;
	char Name[MAX_ACCOUNT_LEN];
	BYTE byGet;
};

struct PMSG_ANS_ITL_RELATE_RELICS
{
	PBMSG_HEAD2 h;
	BYTE byError;
	BYTE byNumberH;
	BYTE byNumberL;
	WORD wNpcType;
	BYTE byAct;
};

struct PMSG_ITL_STONEINFO
{
	PBMSG_HEAD2 h;
	BYTE byMapTagIndex;
	WORD wOccupiedStoneType;
	BYTE byOccupiedStoneState;
	int nNpcIndex;
};

struct PMSG_ITL_PLAY_STATE
{
	PBMSG_HEAD2 h;
	WORD wRemainSec;
	BYTE byAlliedPoint;
	BYTE btIllusionPoint;
	BYTE btMyTeam;
};

struct PMSG_ITL_STATE
{
	PBMSG_HEAD2 h;
	BYTE btTempleNumber;
	BYTE btITLState;
};

struct PMSG_ITL_SKILL_END
{
	PBMSG_HEAD2 h;
	BYTE MagicNumberH;
	BYTE MagicNumberL;
	WORD wObjIndex;
};

struct PMSG_ILLUSION_TEMPLE_USE_MAGIC_RESULT
{
	PBMSG_HEAD2 h;
	BYTE btResult;
	BYTE MagicNumberH;
	BYTE MagicNumberL;
	WORD wSourceObjIndex;
	WORD wTargetObjIndex;
};

struct PMSG_ITR_USER_ADD_EXP
{
	char GameID[MAX_ACCOUNT_LEN];
	BYTE btMapNumber;
	BYTE btTeam;
	BYTE btClass;
	BYTE btUserKillCount;
};

struct PMSG_ITR_RESULT
{
	PBMSG_HEAD2 h;
	BYTE btAlliedPoint;
	BYTE btIllusionPoint;
	BYTE btUserCount;
	BYTE byWinTeamIndex;
};

struct _ITL_AnswerEnter
{
	PBMSG_HEAD2 h;
	BYTE bReason;
	char Name[MAX_ACCOUNT_LEN + 1];
};

struct _ITR_AcceptEnter
{
	PBMSG_HEAD2 h;
	BYTE byError;
	char Name[MAX_ACCOUNT_LEN + 1];
};

struct PMSG_REQ_ENTER_ITR
{
	PBMSG_HEAD2 h;
};

struct PMSG_REQ_ACCEPTENTER_ITR
{
	PBMSG_HEAD2 h;
	BYTE byEnterOK;
};

struct PMSG_REQ_ITL_RELATE_RELICS
{
	PBMSG_HEAD2 h;
	BYTE NumberH;
	BYTE NumberL;
	WORD wNpcType;
	BYTE byPrePacket;
	BYTE byAct;
};

struct PMSG_ITL_USE_MAGIC_RESULT
{
	PBMSG_HEAD2 h;
	BYTE btResult;
	BYTE MagicNumberH;
	BYTE MagicNumberL;
	WORD wSourceObjIndex;
	WORD wTargetObjIndex;
};

struct PMSG_ITL_RESULT
{
	PBMSG_HEAD2 h;
	BYTE btUserCount;
	char szAlliedGuildName[MAX_GUILD_LEN + 1];
	char szIllusionGuildName[MAX_GUILD_LEN + 1];
	BYTE byWin;
};

struct PMSG_ITL_USER_ADD_EXP
{
	char GameID[MAX_ACCOUNT_LEN];
	BYTE btMapNumber;
	BYTE btTeam;
	BYTE btClass;
	BYTE btUserKillCount;
};

struct PMSG_ANS_ITL_SCHEDULE
{
	PWMSG_HEAD2 h;
	BYTE byCount;
	BYTE LeagueDay1;
	BYTE LeagueDay2;
	BYTE LeagueDay3;
	BYTE LeagueDay4;
	BYTE LeagueDay5;
	BYTE LeagueDay6;
	BYTE LeagueDay7;
	BYTE SemiFinal;
	BYTE Final;
	BYTE ITL_STATE;
};

struct __ITL_GUILD_RANK
{
	char szGuildName[MAX_GUILD_LEN + 1];
	BYTE byRank;
	BYTE byITLType;
	BYTE byWin;
	BYTE byLose;
};

struct PMSG_ITL_GUILD_RANKINFO
{
	PWMSG_HEAD2 h;
	BYTE byResult;
	BYTE byCnt;
};

struct PMSG_ANS_REWARDLIST
{
	PWMSG_HEAD2 h;
	BYTE byResult;
	BYTE byCount;
};

struct _tagITL_REWARDLIST
{
	BYTE byRank;
	BYTE byEnteredCnt;
	BYTE byPoint;
	bool bGotReward;
	char szCharName[MAX_ACCOUNT_LEN + 1];
	char szGuildName[MAX_GUILD_LEN + 1];
};

struct _tagPMSG_REQ_ACHERON_EVENT_ENTER
{
	PBMSG_HEAD2 h;
};

struct _tagPMSG_ANS_ACHERON_EVENT_ENTER
{
	PBMSG_HEAD2 h;
	BYTE btResult;
};

struct _tagPMSG_SEND_WINDOW_CLOSE
{
	PBMSG_HEAD2 h;
};

struct PMSG_RAREITEMPIRCESEND
{
	PWMSG_HEAD2 h;
	BYTE btItemSellPriceDivisor;
	int iJewelOfBlessPrice;
	int	iJewelOfSoulPrice;
	int iJewelOfChaosPrice;
	int iJewelOfLifePrice;
	int iJewelOfCreationPrice;
	int iCrestOfMonarchPrice;
	int iLochFeatherPrice;
	int iJewelOfGuardianPrice;
	int iWereRabbitEggPrice;
};

struct STAT_BONUS
{
	WORD BonusOptionID;
	WORD BonusOptionVal1;
	WORD BonusOptionVal2;
};
 
struct PMSG_STAT_BONUS
{
	PBMSG_HEAD2 h;
	STAT_BONUS m_BonusOption[5];
};

struct PMSG_MUUN_RIDE_SELECT
{
	PBMSG_HEAD2 h;
	WORD wItemNum;
};

struct PMSG_REQ_MUUN_EXCHANGE
{
	PBMSG_HEAD2 h;
	BYTE btSelect;
};

struct PMSG_SEND_MUUN_RIDE_VIEWPORT_INFO
{
	PWMSG_HEAD2 h;
	BYTE Count;
};
struct _tagMuunRideViewPortInfo
{
	BYTE NumberH;
	BYTE NumberL;
	BYTE MuunRideItemH;
	BYTE MuunRideItemL;
};

struct PMSG_ANS_MUUN_EXCHANGE
{
	PBMSG_HEAD2 h;
	BYTE btResult;
};

struct PMSG_MUUN_ATTACK_COMMAND
{
	PBMSG_HEAD2 h;
	BYTE Slot;
	BYTE SkillType;
	BYTE NumberH;
	BYTE NumberL;
	BYTE TargetNumberH;
	BYTE TargetNumberL;
};

// UBF

struct PMSG_REQ_UBF_INFO
{
	PBMSG_HEAD2 h;
	BYTE btUBFKind;
};

struct PMSG_REQ_UBF_JOIN
{
	PBMSG_HEAD2 h;
	BYTE btUBFKind;
};

struct PMSG_REQ_UBF_CANCEL
{
	PBMSG_HEAD2 h;
	BYTE btCanceled;
};

struct PMSG_REQ_UBF_REAL_NAME
{
	PBMSG_HEAD2 h;
	BYTE btRequest;
};

struct PMSG_REQ_GET_REWARD
{
	PBMSG_HEAD2 h;
	BYTE btBattleKind;
};

struct PMSG_ANS_UBF_JOIN
{
	PBMSG_HEAD2 h;
	BYTE btResult;
	WORD nLeftSec;
};

struct PMSG_ANS_UBF_INFO
{
	PBMSG_HEAD2 h;
	BYTE btResult;
};

struct PMSG_ANS_UBF_CANCEL
{
	PBMSG_HEAD2 h;
	BYTE btResult;
};

struct PMSG_ANS_GET_REWARD
{
	PBMSG_HEAD2 h;
	BYTE btResult;
};

// DSF

struct PMSG_REQ_DSF_ACCEPTENTER
{
	PBMSG_HEAD2 h;
	BYTE btEnterOK;
};

struct PMSG_REQ_DSF_GO_FINAL_PARTY_INFO
{
	PBMSG_HEAD2 h;
	BYTE btDSFType;
};

struct PMSG_REQ_DSF_GET_REWARD
{
	PBMSG_HEAD2 h;
	BYTE btDSFType;
};

struct PMSG_ANS_DSF_ENTER
{
	PBMSG_HEAD2 h;
	BYTE btResult;
	char Name[MAX_ACCOUNT_LEN+1];
	BYTE btDSFType;
};

struct PMSG_ANS_DSF_ACCEPTENTER
{
	PBMSG_HEAD2 h;
	BYTE btError;
	char Name[MAX_ACCOUNT_LEN+1];
};

struct PMSG_ANS_DSF_GO_FINAL_PARTY_INFO
{
	PWMSG_HEAD2 h;
	BYTE btResult;
	BYTE btCnt;
	BYTE btDSFType;
};

struct DSF_GO_FINAL_PARTY
{
	DSF_GO_FINAL_PARTY()
	{
		this->Clear();
	}

	void Clear()
	{
		memset(szUserName1, 0x00, sizeof(szUserName1));
		memset(szUserName2, 0x00, sizeof(szUserName2));
		this->wServerCode1 = -1;
		this->wServerCode2 = -1;
		this->btRank = 0;
		this->btDSFType = 0;
		this->iPoint = 0;
		this->iEnterYear = 0;
		this->btEnterMonth = 0;
		this->btEnterDay = 0;
	}

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

struct PMSG_ANS_DSF_SCHEDULE
{
	PBMSG_HEAD2 h;
	BYTE LeagueDay1;
	BYTE LeagueDay2;
	BYTE LeagueDay3;
	BYTE LeagueDay4;
	BYTE SemiFinalDay1;
	BYTE SemiFinalDay2;
	BYTE Final;
	BYTE CurDay;
	BYTE LeagueHour[10];
	BYTE LeagueMin[10];
	BYTE SemiFinalHour[10];
	BYTE SemiFinalMin[10];
	BYTE FinalHour;
	BYTE FinalMin;
	BYTE RewardStartDay;
	BYTE RewardEndDay;
};

struct PMSG_DSF_TEAM_USERNAME
{
	PWMSG_HEAD2 h;
	int nPartyCount;
	BYTE btResult;
};

struct DSF_TEAM_PARTY_USER_NAME
{
	int iTeamIndex;
	char szUserName1[MAX_ACCOUNT_LEN+1];
	char szUserName2[MAX_ACCOUNT_LEN+1];
	int iTeamLevel;
};

struct PMSG_DSF_STATE
{
	PBMSG_HEAD2 h;
	BYTE btDSFState;
};

struct PMSG_DSF_PLAY_STATE
{
	PBMSG_HEAD2 h;
	WORD wRemainSec;
	int iTeamPoint[20];
	BYTE btMyTeam;
};

struct PMSG_DSF_RESULT
{
	PBMSG_HEAD2 h;
	int iTeamPoint[20];
};

struct PMSG_ANS_DSF_GET_REWARD
{
	PBMSG_HEAD2 h;
	BYTE btResult;
};

// Rest of S9
struct PMSG_ANS_USE_POPUP
{
	PBMSG_HEAD2 h;
	int nPopupType;
};

struct PMSG_SEND_EVENT_BANNER
{
	PBMSG_HEAD2 h;
	int iBannerType;
};

struct PMSG_ALTERPSHOPVAULT
{
	PBMSG_HEAD h;
	BYTE subcode;
	BYTE type;
};

struct PMSG_POST_DATA
{
	PBMSG_HEAD2 h;
	BYTE btColorRGB[3];
	char szServerName[50];
	char szTag[20];
	char szName[MAX_ACCOUNT_LEN];
	char szMessage[MAX_CHAT_LEN];
};

struct PMSG_SET_CHAT_COLOR
{
	PBMSG_HEAD2 h;
	BYTE btInfoMsg[3];
	BYTE btErrorMsg[3];
	BYTE btChatMsg[3];
	BYTE btWhisperMsg[3];
	BYTE btPartyMsg[3];
	BYTE btGuildMsg[3];
	BYTE btGensMsg[3];
	BYTE btAllianceMsg[3];
	BYTE btGMChatMsg[3];
};

struct PMSG_SEND_HITHACK_INFO // C1:FA:14
{
	PBMSG_HEAD2 h;
	DWORD dwHitHackVal1;
	DWORD dwHitHackVal2;
};

// Season X
struct BLOCKCHAT_DATA
{
	BYTE btBlockEnable;
	BYTE btIndex;
	char szName[MAX_ACCOUNT_LEN + 1];
};

struct PMSG_BLOCKCHAT_LIST // C2:6E:00
{
	PWMSG_HEAD2 h;
	BYTE btType;
	BYTE btSubType;
	BYTE Count;
	BLOCKCHAT_DATA m_BlockData[20];
};

struct PMSG_EVENTENTRY_COUNT
{
	PBMSG_HEAD2 h; // C1:F1:FD
	BYTE btSendType; // 0 - MaxCount, 1 - CurrentCount
	BYTE btValue[6]; // BC,CC,DS,DG,IF,IT
};

struct PMSG_EVENTENTRY_NOTICE
{
	PBMSG_HEAD2 h; // C1:F1:FE
	BYTE btEventType;
	BYTE btNoticeState;
};

struct PMSG_RUUD
{
	PBMSG_HEAD2 h; // C1:D0:F1
	int iTotalRuud;
	int iObtainedRuud;
	BYTE btObtainNotiSend;
};

struct PMSG_REQ_CHAOS_MULTIMIX_CHECK
{
	PBMSG_HEAD h;
	BYTE btMixType;
	BYTE btMixCount;
};

struct PMSG_ANS_CHAOS_MULTIMIX_CHECK
{
	PBMSG_HEAD h;
	BYTE btResult;
};

class GameProtocol
{
public:

	GameProtocol();
	~GameProtocol();

	void ProtocolCore(BYTE protoNum, LPBYTE aRecv, int aLen, int aIndex, BOOL Encrypt);
	void MsgSendV2(LPOBJ lpObj, unsigned char* Msg, int size);
	void ChatSendV2(LPOBJ lpObj, unsigned char* Msg, int size);
	void CGLiveClient(PMSG_CLIENTTIME* lpClientTime, short aIndex);
	void GCCheckMainExeKeySend(int aIndex);
	WORD EncryptCheckSumKey(WORD wSource);
	void CGCheckMainRecv(PMSG_CHECK_MAINEXE_RESULT* lpMsg, int aIndex);
	void PEchoProc(unsigned char* aMsg, int aLen, short aIndex);
	void GCResultSend(int aIndex, unsigned char headcode, unsigned char result);
	void ChatSend(LPOBJ lpObj, char* szChat);
	void vChatSend(LPOBJ lpObj, char* szChat, ...);
	void AllSendServerMsg(char* chatmsg);
	void DataSendAll(unsigned char* lpMsg, int iMsgSize);
	void ChatTargetSend(LPOBJ lpObj, char* szChat, int senduser);
	void PChatProc(PMSG_CHATDATA* lpChat, short aIndex);
	void CGChatRecv(PMSG_CHATDATA_NUMBER* lpMsg, int aIndex);
	void GCServerMsgSend(unsigned char msg, int aIndex);
	void GCServerMsgStringSend(char* szMsg, int aIndex, BYTE type);
	void GCServerMsgStringSendEx(int aIndex, BYTE type, LPSTR szMsg, ...);
	void GCServerMsgStringSendAll(LPSTR  szMsg, BYTE type);
	void GCServerMsgStringSendAllEx(BYTE type, LPSTR szMsg, ...);
	void GCServerMsgStringSendGuild(_GUILD_INFO_STRUCT* lpNode, char* szMsg, unsigned char type);
	void GCEventStateSend(int aIndex, unsigned char state, unsigned char event);
	void GCMapEventStateSend(int map, unsigned char state, unsigned char event);
	void CGChatWhisperRecv(PMSG_CHATDATA_WHISPER* lpMsg, int aIndex);
	void SCPJoinResultSend(int aIndex, unsigned char result);
	void CSPJoinIdPassRequest(PMSG_IDPASS* lpMsg, int aIndex);
	void GCJoinBillCheckSend(char* AccountId, int aIndex);
	void GCJoinResult(unsigned char result, int aIndex);
	void GCJoinSocketResult(unsigned char result, SOCKET Socket);
	void CGClientCloseMsg(PMSG_CLIENTCLOSE* lpMsg, int aIndex);
	void GCCloseMsgSend(int aIndex, unsigned char result);
	void CGClientMsg(PMSG_CLIENTMSG* lpMsg, int aIndex);
	void CGPCharacterCreate(PMSG_CHARCREATE* lpMsg, int aIndex);
	void CGPCharDel(PMSG_CHARDELETE* lpMsg, int aIndex);
	void CGPCharacterMapJoinRequest(PMSG_CHARMAPJOIN* lpMsg, int aIndex);
	void GCLevelUpMsgSend(int aIndex, int iSendEffect);
	void CGLevelUpPointAdd(PMSG_LVPOINTADD* lpMsg, int aIndex);
	void GCInventoryItemOneSend(int aIndex, int pos);
	void GCPkLevelSend(int aIndex, unsigned char pklevel);
	void GCMagicListOneSend(int aIndex, char Pos, unsigned short type, unsigned char level, unsigned short skill, BYTE btListType);
	void GCMagicListOneDelSend(int aIndex, char Pos, unsigned short type, BYTE level, unsigned short skill, BYTE btListType);
	void GCMagicListMultiSend(LPOBJ lpObj, BYTE btListType);
	void GCEquipmentSend(int aIndex);
	void GCRecallMonLife(int aIndex, int maxlife, int life);
	void GCTimeViewSend(int aIndex, int second);
	void GCGoalSend(int aIndex, char* Name1, unsigned char score1, char* Name2, unsigned char score2);
	void GCSkillKeyRecv(PMSG_SKILLKEY* lpMsg, int aIndex);
	void GCSkillKeySend(int aIndex, unsigned char* keybuffer, unsigned char GO, unsigned char Qk, unsigned char Wk, unsigned char Ek, /*<regrel ebp+0x20>*/ /*|0x1|*/ unsigned char ChatWnd, unsigned char Rk, int QWER);
	void GCMoneySend(int aIndex, unsigned long money);
	void GCItemInventoryPutSend(int aIndex, unsigned char result, unsigned char iteminfo1, unsigned char iteminfo2);
	void CGItemGetRequest(PMSG_ITEMGETREQUEST* lpMsg, int aIndex);
	bool CGItemDropRequest(PMSG_ITEMTHROW* lpMsg, int aIndex, int drop_type);
	void GCItemMoveResultSend(int aIndex, unsigned char result, unsigned char pos, unsigned char* const ItemInfo);
	void CGInventoryItemMove(PMSG_INVENTORYITEMMOVE* lpMsg, int aIndex);
	void GCEquipmentChange(int aIndex, int pos);
	void CGTalkRequestRecv(PMSG_TALKREQUEST* lpMsg, int aIndex);
	void GCUserWarehouseSend(LPOBJ lpObj);
	void CGBuyRequestRecv(PMSG_BUYREQUEST* lpMsg, int aIndex);
	void CGSellRequestRecv(PMSG_SELLREQUEST* lpMsg, int aIndex);
	int  GetNeedMoneyItemDurRepaire(/*int aIndex, */class CItem* DurItem, int RequestPos);
	void ItemDurRepaire(LPOBJ lpObj, /*int aIndex, */class CItem* DurItem, int pos, int RequestPos);
	void CGModifyRequestItem(PMSG_ITEMDURREPAIR* lpMsg, int aIndex);
	void CGTradeRequestSend(PMSG_TRADE_REQUEST* lpMsg, int aIndex);
	void CGTradeResponseRecv(PMSG_TRADE_RESPONSE* lpMsg, int aIndex);
	bool GCTradeResponseSend(BYTE response, int aIndex, LPSTR id, WORD level, int GuildNumber);
	int GCTradeOtherDel(int aIndex, BYTE tradeindex);
	int GCTradeOtherAdd(int aIndex, BYTE tradeindex, LPBYTE iteminfo);
	void CGTradeMoneyRecv(PMSG_TRADE_GOLD* lpMsg, int aIndex);
	bool GCTradeMoneyOther(int aIndex, DWORD money);
	bool GCTradeOkButtonSend(int aIndex, unsigned char flag);
	void CGTradeOkButtonRecv(PMSG_TRADE_OKBUTTON* lpMsg, int aIndex);
	void CGTradeCancelButtonRecv(int aIndex);
	void CGTradeResult(int aIndex, unsigned char result);
	void CGPartyRequestRecv(PMSG_PARTYREQUEST* lpMsg, int aIndex);
	void CGPartyRequestResultRecv(PMSG_PARTYREQUESTRESULT* lpMsg, int aIndex);
	void CGPartyList(int aIndex);
	void CGPartyListAll(int pnumber);
	void CGPartyDelUser(PMSG_PARTYDELUSER* lpMsg, int aIndex, int nType);
	void GCPartyDelUserSend(int aIndex);
	void GCPartyDelUserSendNoMessage(int aIndex);
	void CGGuildRequestRecv(PMSG_GUILDJOINQ* lpMsg, int aIndex);
	void CGGuildRequestResultRecv(PMSG_GUILDQRESULT* lpMsg, int aIndex);
	void CGGuildListAll(int pnumber);
	void CGGuildDelUser(PMSG_GUILDDELUSER* lpMsg, int aIndex);
	void GCGuildDelUserResult(int aIndex, unsigned char Result);
	void GCGuildMasterQuestionSend(int aIndex);
	void CGGuildMasterAnswerRecv(PMSG_GUILDMASTERANSWER* lpMsg, int aIndex);
	void GCGuildMasterManagerRun(int aIndex);
	void CGGuildMasterInfoSave(int aIndex,PMSG_GUILDINFOSAVE* lpMsg);
	void CGGuildMasterCreateCancel(int aIndex);
	void GCGuildViewportNowPaint(int aIndex, char* guildname, unsigned char* Mark, int isGuildMaster);
	void GCGuildViewportDelNow(int aIndex, int isGuildMaster);
	void GCManagerGuildWarEnd(char* GuildName);
	void GCManagerGuildWarSet(char* GuildName1, char* GuildName2, int type);
	void GCGuildWarRequestResult(char* GuildName, int aIndex, int type);
	void GCGuildWarRequestSend(char* GuildName, int aIndex, int type);
	void GCGuildWarRequestSendRecv(PMSG_GUILDWARSEND_RESULT* lpMsg, int aIndex);
	void GCGuildWarDeclare(int aIndex, char* _guildname);
	void GCGuildWarEnd(int aIndex, unsigned char result, char* _guildname);
	void GCGuildWarScore(int aIndex);
	void CGWarehouseMoneyInOut(int aIndex,PMSG_WAREHOUSEMONEYINOUT* lpMsg);
	void GCWarehouseInventoryMoneySend(int aIndex, unsigned char result, int money, int wmoney);
	void CGWarehouseUseEnd(int aIndex);
	void GCWarehouseStateSend(int aIndex, unsigned char state);
	void GCWarehouseRecivePassword(int aIndex,PMSG_WAREHOUSEPASSSEND* lpMsg);
	void GCUserChaosBoxSend(LPOBJ lpObj, int iChaosBoxType);
	void CGChaosBoxItemMixButtonClick(PMSG_CHAOSMIX* aRecv, int aIndex);
	void CGChaosBoxUseEnd(int aIndex);
	void PMoveProc(PMSG_MOVE* lpMove, int aIndex);
	void RecvPositionSetProc(PMSG_POSISTION_SET* lpMove, int aIndex);
	void CGAttack(PMSG_ATTACK* lpMsg, int aIndex);
	void GCDamageSendPoison(int aIndex, int damage, int iShieldDamage);
	void GCDamageSend(int aIndex, int TargetIndex, int AttackDamage, int MSBFlag, int MSBDamage, int iShieldDamage);
	void GCKillPlayerExpSend(int aIndex, int TargetIndex, int exp, int AttackDamage, int MSBFlag);
	void GCKillPlayerMasterExpSend(int aIndex, int TargetIndex, int exp, int AttackDamage, BOOL MSBFlag);
	void GCDiePlayerSend(LPOBJ lpObj, int TargetIndex, BYTE skill, int KillerIndex);
	void GCActionSend(LPOBJ lpObj, BYTE ActionNumber, int aIndex, int aTargetIndex);
	void CGActionRecv(PMSG_ACTION* lpMsg, int aIndex);
	void CGMagicAttack(LPBYTE lpRecv, int aIndex);
	void CGRageAttack(LPBYTE lpRecv, int aIndex);
	void CGRageAttackRange(PMSG_RAGE_ATTACK_RANGE* lpMsg, int aIndex);
	void GCMagicAttackNumberSend(LPOBJ lpObj, WORD MagicNumber, int usernumber, unsigned char skillsuccess);
	void GCMonkMagicAttack(LPOBJ lpObj, WORD MagicNumber, int usernumber,  unsigned char skillsuccess);
	void GCMagicCancelSend(LPOBJ lpObj, WORD MagicNumber);
	void GCMagicCancel(PMSG_MAGICCANCEL * lpMsg, int aIndex);
	void GCUseEffectItem(LPOBJ lpObj, int iEffectUseOption, int iOptionType, int iEffectType, int iLeftTime);
	void GCUseEffectItem2(LPOBJ lpObj, int iEffectUseOption, int iOptionType, int iEffectType, int iLeftTime);
	void CGMagicCancel(PMSG_MAGICCANCEL* lpMsg, int aIndex);
	void GCUseMonsterSkillSend(LPOBJ lpObj, LPOBJ lpTargetObj, int iSkillNumber);
	void GCStateInfoSend(LPOBJ lpObj, unsigned char state, int BuffViewport);
	void CGTeleportRecv(PMSG_TELEPORT* lpMsg, int aIndex);
	void CGReqMapMove(PMSG_REQ_MAPMOVE *lpMsg, int iIndex);
	void GCSendMapMoveCheckSum(LPOBJ lpObj);
	void CGTargetTeleportRecv(PMSG_TARGET_TELEPORT* lpMsg, int aIndex);
	void GCTeleportSend(LPOBJ lpObj, unsigned short MoveNumber, BYTE MapNumber, BYTE MapX, BYTE MapY, BYTE Dir);
	void CGBeattackRecv(unsigned char* lpRecv, int aIndex, int magic_send);
	void CGDurationMagicRecv(LPBYTE lpRecv, int aIndex);
	void ObjectMapJoinPositionSend(short aIndex);
	void CGUseItemRecv(PMSG_USEITEM* lpMsg, int aIndex);
	void GCReFillSend(int aIndex, int Life, BYTE Ipos, unsigned char flag,  WORD wShield);
	void GCManaSend(int aIndex, int Mana, BYTE Ipos, unsigned char flag,  WORD BP);
	void GCInventoryItemDeleteSend(int aIndex, BYTE pos, unsigned char flag);
	void GCItemUseSpecialTimeSend(int aIndex, unsigned char number, int time);
	void GCItemDurSend(int aIndex, BYTE pos, unsigned char dur, unsigned char flag);
	void GCItemDurSend2(int aIndex, BYTE pos, unsigned char dur, unsigned char flag);
	void CGWeatherSend(int aIndex, BYTE weather);
	void GCServerCmd(int aIndex, BYTE type, unsigned char Cmd1, unsigned char Cmd2);
	void GCReqmoveDevilSquare(PMSG_REQ_MOVEDEVILSQUARE* lpMsg, int aIndex);
	void GCReqDevilSquareRemainTime(PMSG_REQ_DEVILSQUARE_REMAINTIME* lpMsg, int aIndex);
	void AllSendMsg(unsigned char* Msg, int size);
	void AllSendSameMapMsg(unsigned char* Msg, int size, unsigned char mapnumber);
	void GCSendPing(int aIndex);
	void GCPingSendRecv(PMSG_PING_RESULT* aRecv, int aIndex);
	void GCRegEventChipRecv(PMSG_REGEVENTCHIP* lpMsg, int aIndex);
	void GCGetMutoNumRecv(PMSG_GETMUTONUMBER* lpMsg, int aIndex);
	void GCUseEndEventChipRescv(int aIndex);
	void GCUseRenaChangeZenRecv(PMSG_EXCHANGE_EVENTCHIP* lpMsg, int aIndex);
	void CGRequestQuestInfo(int aIndex);
	void GCSendQuestInfo(int aIndex, int QuestIndex);
	void CGSetQuestState(PMSG_SETQUEST* lpMsg, int aIndex);
	void GCSendQuestPrize(int aIndex, unsigned char Type, unsigned char Count);
	void CGCloseWindow(int aIndex);
	void CGRequestEnterBloodCastle(PMSG_REQ_MOVEBLOODCASTLE* lpMsg, int iIndex);
	void CGRequestEnterChaosCastle(PMSG_REQ_MOVECHAOSCASTLE* lpMsg, int iIndex);
	void CGRequestRepositionUserInChaosCastle(PMSG_REQ_REPOSUSER_IN_CC* lpMsg, int aIndex);
	void CGRequestEventEnterCount(PMSG_REQ_CL_ENTERCOUNT* lpMsg, int aIndex);
	void CGRequestLottoRegister(PMSG_REQ_2ANV_LOTTO_EVENT* lpMsg, int aIndex);
	void CGReqMoveOtherServer(PMSG_REQ_MOVE_OTHERSERVER* lpMsg, int aIndex);
	void GCPacketCheckSumRecv(PMSG_PACKETCHECKSUM* aRecv, int aIndex);
	void CGDuelStartRequestRecv(PMSG_REQ_START_DUEL* lpMsg, int aIndex);
	void CGDuelEndRequestRecv(PMSG_REQ_END_DUEL* lpMsg, int aIndex);
	void CGDuelOkRequestRecv(PMSG_ANS_DUEL_OK* lpMsg, int aIndex);
	void GCSendDuelScore(int aIndex1, int aIndex2);
	void GCSendGetItemInfoForParty(int aIndex, class CMapItem* lpItem);
	void GCSendEffectInfo(int aIndex, BYTE btType);
	void CGRequestPetItemCommand(PMSG_REQUEST_PET_ITEM_COMMAND* lpMsg, int aIndex);
	void CGRequestPetItemInfo(PMSG_REQUEST_PET_ITEMINFO* lpMsg, int aIndex);
	void GCGuildViewportInfo(PMSG_REQ_GUILDVIEWPORT* aRecv, int aIndex);
	void CGGuildAssignStatus(PMSG_GUILD_ASSIGN_STATUS_REQ* aRecv, int aIndex);
	void CGGuildAssignType(PMSG_GUILD_ASSIGN_TYPE_REQ* aRecv, int aIndex);
	void CGRelationShipReqJoinBreakOff(PMSG_RELATIONSHIP_JOIN_BREAKOFF_REQ* aRecv, int aIndex);
	void CGRelationShipAnsJoinBreakOff(PMSG_RELATIONSHIP_JOIN_BREAKOFF_ANS* aRecv, int aIndex);
	void CGUnionList(PMSG_UNIONLIST_REQ* aRecv, int aIndex);
	void CGRelationShipReqKickOutUnionMember(PMSG_KICKOUT_UNIONMEMBER_REQ* aRecv, int aIndex);
	void CGReqMapSvrAuth(PMSG_REQ_MAPSERVERAUTH* lpMsg, int iIndex);
	void GCAnsMapSvrAuth(int iIndex, int iResult);
	void CGReqCastleSiegeState(PMSG_REQ_CASTLESIEGESTATE* lpMsg, int iIndex);
	void GCAnsCastleSiegeState(int iIndex, int iResult, char* lpszGuildName, char* lpszGuildMaster);
	void CGReqRegCastleSiege(PMSG_REQ_REGCASTLESIEGE* lpMsg, int iIndex);
	void GCAnsRegCastleSiege(int iIndex, int iResult, char* lpszGuildName);
	void CGReqGiveUpCastleSiege(PMSG_REQ_GIVEUPCASTLESIEGE* lpMsg, int iIndex);
	void GCAnsGiveUpCastleSiege(int iIndex, int iResult, int bGiveUp, int iMarkCount, char* lpszGuildName);
	void CGReqGuildRegInfo(PMSG_REQ_GUILDREGINFO* lpMsg, int iIndex);
	void GCAnsGuildRegInfo(int iIndex, int iResult,CSP_ANS_GUILDREGINFO* lpMsgResult);
	void CGReqRegGuildMark(PMSG_REQ_REGGUILDMARK* lpMsg, int iIndex);
	void GCAnsRegGuildMark(int iIndex, int iResult,CSP_ANS_GUILDREGMARK* lpMsgResult);
	void CGReqNpcBuy(PMSG_REQ_NPCBUY* lpMsg, int iIndex);
	void GCAnsNpcBuy(int iIndex, int iResult, int iNpcNumber, int iNpcIndex);
	void CGReqNpcRepair(PMSG_REQ_NPCREPAIR* lpMsg, int iIndex);
	void GCAnsNpcRepair(int iIndex, int iResult, int iNpcNumber, int iNpcIndex, int iNpcHP, int iNpcMaxHP);
	void CGReqNpcUpgrade(PMSG_REQ_NPCUPGRADE* lpMsg, int iIndex);
	void GCAnsNpcUpgrade(int iIndex, int iResult, int iNpcNumber, int iNpcIndex, int iNpcUpType, int iNpcUpValue);
	void CGReqTaxMoneyInfo(PMSG_REQ_TAXMONEYINFO* lpMsg, int iIndex);
	void GCAnsTaxMoneyInfo(int iIndex, int iResult, BYTE btTaxRateChaos, BYTE btTaxRateStore, __int64 i64Money);
	void CGReqTaxRateChange(PMSG_REQ_TAXRATECHANGE* lpMsg, int iIndex);
	void GCAnsTaxRateChange(int iIndex, int iResult, BYTE btTaxType, int iTaxRate);
	void CGReqMoneyDrawOut(PMSG_REQ_MONEYDRAWOUT* lpMsg, int iIndex);
	void GCAnsMoneyDrawOut(int iIndex, int iResult, __int64 i64Money);
	void GCAnsCsGateState(int iIndex, int iResult, int iGateIndex);
	void CGReqCsGateOperate(PMSG_REQ_CSGATEOPERATE* lpMsg, int iIndex);
	void GCAnsCsGateOperate(int iIndex, int iResult, int iGateIndex, int iGateOperate);
	void GCAnsCsGateCurState(int iIndex, int iGateIndex, int iGateOperate);
	void GCAnsCsAccessSwitchState(int iIndex, int iSwitchIndex, int iSwitchUserIndex, BYTE btSwitchState);
	void GCAnsCsAccessCrownState(int iIndex, BYTE btCrownState);
	void GCAnsCsNotifyStart(int iIndex, BYTE btStartState);
	void GCAnsCsNotifyProgress(int iIndex, BYTE btCastleSiegeState, LPCSTR lpszGuildName);
	void GCAnsCsMapSvrTaxInfo(int iIndex, BYTE btTaxType, BYTE btTaxRate);
	void CGReqCsMiniMapData(PMSG_REQ_MINIMAPDATA* lpMsg, int iIndex);
	void GCAnsCsMiniMapData(int iIndex, BYTE btResult);
	void CGReqStopCsMiniMapData(PMSG_REQ_STOPMINIMAPDATA* lpMsg, int iIndex);
	void CGReqCsSendCommand(PMSG_REQ_CSCOMMAND* lpMsg, int iIndex);
	void GCAnsCsSendCommand(int iCsJoinSide, BYTE btTeam, BYTE btX, BYTE btY, BYTE btCommand);
	void GCAnsCsLeftTimeAlarm(BYTE btHour, BYTE btMinute);
	void GCAnsSelfCsLeftTimeAlarm(int iIndex, BYTE btHour, BYTE btMinute);
	void CGReqCsSetEnterHuntZone(PMSG_REQ_CSHUNTZONEENTER* lpMsg, int iIndex);
	void GCAnsCsSetEnterHuntZone(int iIndex, BYTE btResult, BYTE btEnterHuntZone);
	void CGReqNpcDbList(PMSG_REQ_NPCDBLIST* lpMsg, int iIndex);
	void CGReqCsRegGuildList(PMSG_REQ_CSREGGUILDLIST* lpMsg, int iIndex);
	void CGReqCsAttkGuildList(PMSG_REQ_CSATTKGUILDLIST* lpMsg, int iIndex);
	void CGReqWeaponUse(PMSG_REQ_USEWEAPON* aRecv, int iIndex);
	void CGReqWeaponDamageValue(PMSG_REQ_WEAPON_DAMAGE_VALUE* aRecv, int iIndex);
	void CGReqGuildMarkOfCastleOwner(PMSG_REQ_GUILDMARK_OF_CASTLEOWNER* aRecv, int iIndex);
	void CGReqJewelMix(PMSG_REQ_JEWEL_MIX* lpMsg, int iIndex);
	void GCAnsJewelMix(int iIndex, int iResult);
	void CGReqJewelUnMix(PMSG_REQ_JEWEL_UNMIX* lpMsg, int iIndex);
	void GCAnsJewelUnMix(int iIndex, int iResult);
	void CGReqCrywolfInfo(PMSG_REQ_CRYWOLF_INFO* lpMsg, int iIndex);
	void GCAnsCrywolfInfo(int iIndex, BYTE btOccupationState, BYTE btCrywolfState);
	void CGReqAlatrContract(PMSG_REQ_CRYWOLF_ALTAR_CONTRACT* lpMsg, int iIndex);
	void CGReqPlusChaosRate(PMSG_REQ_CRYWOLF_BENEFIT_PLUS_CHAOSRATE* lpMsg, int iIndex);
	void CGReqKanturuStateInfo(PMSG_REQ_KANTURU_STATE_INFO* lpMsg, int iIndex);
	void GCReqEnterKanturuBossMap(PMSG_REQ_ENTER_KANTURU_BOSS_MAP* lpMsg, int iIndex);
	void GCIllusionTempleSendReward(int aIndex);
	void WereWolfQuarrelCheck(int aIndex);
	void GateKeeperCheck(int aIndex);
	void GCMasterLevelUpMsgSend(int aIndex);
	void CGReqIllusionTempleUseSkill(PMSG_REQ_USEILLUSIONTEMPLESKILL * aRecv, int aIndex);
	void GCSendIllusionTempleKillPoint(int aIndex, BYTE KillPoint);
	void CGReqLuckyCoinRegister(PMSG_REQ_LUCKYCOIN_REGISTER * aRecv, int aIndex);
	void CGReqLuckyCoinInfo(int aIndex);
	void CGReqLuckyCoinTrade(PMSG_REQ_LUCKYCOIN_TRADE * aRecv, int aIndex);
	void GCAnsLuckyCoinTrade(int aIndex, BYTE Result);
	void CGReqMuBotUse(int aIndex, PMSG_MUBOT_SWITCH *aRecv);
	void GCAnsMuBotUse(int aIndex, BYTE Status, DWORD Time, DWORD Money);
	void CGReqMuBotSaveData(int aIndex, PMSG_MUBOT_DATASAVE *aRecv);
	void GCAnsMuBotData(int aIndex, BYTE MuBotData[257]);
	void CGReqCastleHuntZoneEntrance(PMSG_REQ_MOVE_TO_CASTLE_HUNTZONE* aRecv, int aIndex);
	void GCSendObjectCreationState(int iObjectIndex);
	void GCSendObjectHP(int aIndex, int aTargetIndex);
	void CGReqAntiCheatRecv(int aIndex, PMSG_SEND_AH_INFO * aRecv);
	void CGReqFileCrcRecv(int aIndex, PMSG_REQ_CRCDATA * aRecv);
	void GCElementalDamageSend(int aIndex, int TargetIndex, int Damage, int Elemental);
	void GCSendErtelInfo(int aIndex);
	void CGReqEnterAcheron(int aIndex);
	void GCObjectLifeInfo(int iIndex, short sObjNum, int iMaxLife, int iCurLife);
	void CGAgilityRecv(int aIndex, PMSG_AGILITYSEND * aRecv);
	void GCPriceSend(int aIndex, BYTE type, SHOP_DATA *lpShopData);
	int OnCGInviteDuel(LPPMSG_REQ_DUEL_INVITE lpMsg, int aIndex);
	int OnCGAnswerDuel(LPPMSG_ANS_DUEL_ANSWER lpMsg, int aIndex);
	int OnCGLeaveDuel(LPPMSG_REQ_DUEL_EXIT lpMsg, int aIndex);
	int OnDuelChannelJoin(LPPMSG_REQ_DUEL_JOINCNANNEL lpMsg, int aIndex);
	int OnDuelChannelLeave(LPPMSG_REQ_DUEL_LEAVECNANNEL lpMsg, int aIndex);
	void CGReqRegGensMember(_tagPMSG_REQ_REG_GENS_MEMBER *lpMsg, int iIndex);
	void CGReqSecedeGensMember(_tagPMSG_REQ_SEGEDE_GENS_MEMBER *lpMsg, int iIndex);
	void CGReqGensReward(_tagPMSG_GENS_REWARD_CODE *lpMsg, int iIndex);
	void CGReqGensMemberInfo(_tagPMSG_REQ_GENS_INFO *lpMsg, int iIndex);
	void CGInventoryEquipment(_tagPMSG_REQ_INVENTORY_EQUIPMENT_ITEM *lpMsg, int aIndex);
	void CGReqUseTransformationRing(PMSG_REQ_USETRANSFORMATIONRING *lpMsg, int iIndex);
	void GCSendAttackSpeed(int iIndex);
	void GCChaosMachinePriceSend(int aIndex);
	void CGReqEnterDoppelGanger(PMSG_REQ_ENTER_DOPPELGANGER *lpMsg, int iIndex);
	void CGReqEnterZone(PMSG_REQ_ENTER_ZONE *lpMsg, int iIndex);	
	void CGReqInJewelPentagramItem(PMSG_REQ_IN_PENTAGRAM_JEWEL *lpMsg, int aIndex);
	void GCAnsInJewelPentagramItem(int aIndex, int iResult, BYTE btJewelPos, BYTE btJewelIndex, BYTE btItemType, WORD wItemIndex, BYTE btMainAttribute, BYTE btLevel, BYTE btRank1OptionNum, BYTE btRank1Level, BYTE btRank2OptionNum, BYTE btRank2Level, BYTE btRank3OptionNum, BYTE btRank3Level, BYTE btRank4OptionNum, BYTE btRank4Level, BYTE btRank5OptionNum, BYTE btRank5Level);
	void CGReqOutJewelPentagramItem(PMSG_REQ_OUT_PENTAGRAM_JEWEL *lpMsg, int aIndex);
	void GCAnsOutJewelPentagramItem(int iIndex, int iResult, BYTE btJewelPos, BYTE btJewelDBIndex);
	void CGReqRefinePentagramJewel(PMSG_REQ_REFINE_PENTAGRAM_JEWEL *lpMsg, int aIndex);
	void GCAnsRefinePentagramJewel(int iIndex, BYTE btResult);
	void CGReqUpgradePentagramJewel(PMSG_REQ_UPGRADE_PENTAGRAM_JEWEL *lpMsg, int aIndex);
	void GCAnsUpgradePentagramJewel(int iIndex, BYTE btResult);
	void GCAnsPentagramJewelInOut(int iIndex, BYTE btResult);
	void CGReqArcaBattleGuildMasterJoin(_tagPMSG_REQ_ARCA_BATTLE_JOIN *lpMsg, int iIndex);
	void CGReqArcaBattleGuildMemberJoin(_tagPMSG_REQ_ARCA_BATTLE_JOIN *lpMsg, int iIndex);
	void CGReqArcaBattleEnter(_tagPMSG_REQ_ARCA_BATTLE_ENTER *lpMsg, int iIndex);
	void CGReqAcheronEnter(_tagPMSG_REQ_ACHERON_ENTER *lpMsg, int iIndex);
	void CGReqArcaBattleBootyExchange(int iIndex);
	void CGReqSpritemapExchange(int iIndex);
	void CGReqRegisteredMemberCnt(int iIndex);
	void GCSendRecvCheck(int aIndex);
	void CGReqAntihackBreach(int aIndex, PMSG_ANTIHACK_BREACH *lpMsg);
	void CGReqAntihackCheck(int aIndex, PMSG_ANTIHACK_CHECK *lpMsg);
	void GCMuunItemDurSend(int aIndex, BYTE pos, BYTE dur);
	void GCMuunEquipmentChange(int aIndex, int pos);
	void GCMuunInventoryItemDeleteSend(int aIndex, BYTE pos, BYTE flag);
	void GCMuunInventoryItemOneSend(int aIndex, int pos); 
	void CGMuunItemGetRequest(PMSG_MUUNITEM_GETREQUEST *lpMsg, int aIndex);
	void CGMuunItemSellRequestRecv(PMSG_MUUN_ITEM_SELLREQUEST *lpMsg, int aIndex);
	void CGReqGuildMatchingList(PMSG_REQ_GUILDMATCHINGLIST *lpMsg, int nUserIndex);
	void CGReqGuildMatchingListSearchWord(PMSG_REQ_GUILDMATCHINGLISTSEARCHWORD *lpMsg, int nUserIndex);
	void CGReqRegGuildMatchingList(PMSG_REQ_REGGUILDMATCHINGDATA *lpMsg, int nUserIndex);
	void CGReqCancelGuildMatchingList(PMSG_REQ_CANCEL_GUILDMATCHINGLIST *lpMsg, int nUserIndex);
	void CGReqJoinGuildMatching(PMSG_REQ_JOIN_GUILDMATCHING *lpMsg, int nUserIndex);
	void CGReqCancelJoinGuildMatching(int nUserIndex);
	void CGReqAllowJoinGuildMatching(PMSG_REQ_ALLOW_JOIN_GUILDMATCHING *lpMsg, int nUserIndex);
	void CGReqGetAllowListJoinGuildMatching(PMSG_REQ_ALLOWLIST_GUILDMATCHING *lpMsg, int nUserIndex);
	void CGReqGetWaitStateListGuildMatching(int nUserIndex);

	void CGReqRegWantedPartyMember(PMSG_REQ_REG_WANTED_PARTYMEMBER *lpMsg, int nUserIndex);
	void CGReqGetPartyMatchingList(PMSG_REQ_GET_PARTYMATCHINGLIST *lpMsg, int nUserIndex);
	void CGReqJoinMemberPartyMatching(PMSG_REQ_MEMBERJOIN_PARTYMATCHINGLIST *lpMsg, int nUserIndex);
	void CGReqMemberJoinStateList(PMSG_REQ_MEMBERJOIN_STATELIST_PARTYMATCHING *lpMsg, int nUserIndex);
	void CGReqMemberJoinStateListLeader(PMSG_REQ_WAITLIST_PARTYMATCHING *lpMsg, int nUserIndex);
	void CGReqAcceptJoinMemberPartyMatching(PMSG_REQ_ACCEPTMEMBER_PARTYMATCHING *lpMsg, int nUserIndex);
	void CGReqCancelPartyMatching(PMSG_REQ_CANCEL_JOIN_PARTYMATCHING *lpMsg, int nUserIndex);

	void GCDisplayBuffeffectPartyMember(int aIndex);
	void GCPartyMemberPosition(int nIndex);
	void CGReqSendMemberPosInfoStart(int nIndex);
	void CGReqSendMemberPosInfoStop(int nIndex);
	void GCReqSendNpcPosInfo(PMSG_REQ_NPC_POSITION *lpMsg, int nIndex);

	void CGReqCCF_DayTime(int iIndex);
	void CGReqCCF_EnterCheck(int iIndex);
	void CG_CCF_UI_OnOff(PMSG_CCF_UI_ONOFF *lpMsg, int nUserIndex);
	int	 CGReqEnterChaosCastleFinal(int iIndex);
	void CGRequestRepositionUserInCCF(PMSG_REQ_REPOSUSER_IN_CCF *lpMsg, int aIndex);
	void CGReq_CCF_Ranking(PMSG_REQ_CCF_RANKING *lpMsg, int aIndex);

	void CGReqEventInvenOpen(LPBYTE lpRecv, int aIndex);
	void CGEventItemGetRequest(PMSG_EVENTITEM_GETREQUEST *lpMsg, int aIndex);
	bool CGEventItemDropRequest(PMSG_EVENTITEM_THROW *lpMsg, int aIndex);
	void GCEventInventoryItemListSend(int aIndex);
	void GCEventItemDurSend(int aIndex, BYTE pos, BYTE dur);
	void GCEventInvenItemOneSend(int aIndex, int pos);
	void GCEventInventoryItemDeleteSend(int aIndex, BYTE pos, unsigned char flag);

	void GCResSendExpEventInfo(int aIndex);

	void CGReq_ITL_Relics(PMSG_REQ_ITL_RELATE_RELICS *lpMsg, int iIndex);
	void CG_Req_Enter_ITR(PMSG_REQ_ENTER_ITR *lpMsg, int iIndex);
	void CG_ReqAcceptEnterITR(PMSG_REQ_ACCEPTENTER_ITR *lpMsg, int iIndex);
	BYTE EnterITRCheckUser(int iIndex, int &TroubleUserIndex);

	void CGReqUBFMyCharacterInfo(PMSG_REQ_UBF_INFO *lpMsg, int iIndex);
	void CGReqUBFJoin(PMSG_REQ_UBF_JOIN *lpMsg, int iIndex);
	void CGReqUBFCancel(PMSG_REQ_UBF_CANCEL *lpMsg, int iIndex);
	void CGReqUBFGetRealName(PMSG_REQ_UBF_REAL_NAME *lpMsg, int iIndex);
	void CGReqUBFGetReward(PMSG_REQ_GET_REWARD *lpMsg, int iIndex);

	void CGReqDSFSchedule(int iIndex);
	void CGReqDSFEnter(int iIndex);
	int EnterDSFCheckUser(int iIndex, int & TroubleUserIndex);
	void CGReqAcceptEnterDSF(PMSG_REQ_DSF_ACCEPTENTER *lpMsg, int iIndex);
	void CGReqDSFGoFinalParty(PMSG_REQ_DSF_GO_FINAL_PARTY_INFO *lpMsg, int iIndex);
	void CGReqDSFGetReward(PMSG_REQ_DSF_GET_REWARD *lpMsg, int iIndex);

	void CGReqDoUsePopupType(int aIndex);
	void GCSendEventBanner(int aIndex, int iBannerType);

	void GCSendRareItemPriceInfo(int aIndex);
	void GCSendDisableReconnect(int aIndex);
	bool PacketCheckTime(int aIndex);
	void GCSendDisableReconnectSystem(int aIndex);
	void GCAlterPShopVault(int aIndex, BYTE type);

	void GCSetCharColors(int aIndex);

	void CGRecvHitHackValues(int aIndex, PMSG_SEND_HITHACK_INFO * lpMsg);
private:
	bool DataEncryptCheck(int aIndex, BYTE protoNum, BOOL Encrypt);
};

extern GameProtocol GSProtocol;


#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

