//////////////////////////////////////////////////////////////////////
// user.h
// ------------------------------
// Decompiled by Deathway
// Date : 2007-05-09
// ------------------------------
#ifndef USER_H__
#define USER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ComboAttack.h"
#include "MagicInf.h"
#include "giocp.h"
#include "zzzitem.h"
#include "classdef.h"
#include "GuildClass.h"
#include "TDurMagicKeyChecker.h"
#include "TMonsterAIAgro.h"
#include "TMonsterSkillElementInfo.h"
#include "JewelOfHarmonySystem.h"
#include "ItemSystemFor380.h"
#include "QuestExpUserInfo.h"
#include "itemsocketoptiondefine.h"
#include "MuLua.h"
#include "MuunInfo.h"
#include "MuRummyInfo.h"
#include "SkillDelay.h"
#include "Shop.h"
#include "GremoryCase.h"
#include "UnityBattleField.h"

#define CRYWOLF_MAP_RANGE(map) ( ( ((map)) == MAP_INDEX_CRYWOLF_FIRSTZONE  )?TRUE:FALSE  )
#define IMPERIAL_MAP_RANGE(mapnumber) ( ((mapnumber) < MAP_INDEX_IMPERIAL1)?FALSE:((mapnumber) > MAP_INDEX_IMPERIAL4 )?FALSE:TRUE )
#define DG_MAP_RANGE(mapnumber) ( ((mapnumber) < MAP_INDEX_DOUBLE_GOER1)?FALSE:((mapnumber) > MAP_INDEX_DOUBLE_GOER4 )?FALSE:TRUE )
#define IT_MAP_RANGE(value) ( (( (value)   )<MAP_INDEX_ILLUSIONTEMPLE1)?FALSE:(( (value)  )>MAP_INDEX_ILLUSIONTEMPLE6)?FALSE:TRUE  )
#define ITL_MAP_RANGE(mapnumber) ( ((mapnumber) < MAP_INDEX_ILLUSIONTEMPLE7)?FALSE:((mapnumber) > MAP_INDEX_ILLUSIONTEMPLE8 )?FALSE:TRUE )

extern int MaxViewportMonster;

inline bool ObjectMaxRange(int Index);

#define MAX_MONSTER_SEND_MSG	20
#define MAX_MONSTER_SEND_ATTACK_MSG 100
#define MAX_MONSTER_TYPE 1024
#define MAX_GUILD_LEN 8
#define MAX_ACCOUNT_LEN 10
#define MAX_CHARNAME_LEN 10
#define TRADE_BOX_SIZE 32
#define MAGIC_SIZE 150
#define INVETORY_WEAR_SIZE	12
#define INVENTORY_BAG_START INVETORY_WEAR_SIZE
#define TRADE_BOX_MAP_SIZE 32
#define PSHOP_SIZE 32
#define PSHOP_MAP_SIZE 32
#define PSHOP_START_RANGE 204
#define PSHOP_END_RANGE 236
#define MAX_SELF_DEFENSE 5
#define MAX_ST_HIT_DAMAGE 40
#define MAX_ZEN 2000000000
#define MAX_WAREHOUSE_ZEN	1000000000
#define ACTIVE_ITEM_SEARCH 0
#define CHAOS_BOX_SIZE 32
#define CHAOS_BOX_MAP_SIZE 32
#define MAX_BUFFEFFECT	32
#define MAX_MUUN_EFFECT_LIST 2

#define INVENTORY_SIZE				237	//-> 236
#define WAREHOUSE_SIZE				240	//-> 240
#define WAREHOUSE_BUFF				4096
#define MAIN_INVENTORY_SIZE			204	//-> 204
#define INVENTORY_MAP_SIZE			225	//-> 224
#define MAX_INVENTORY_H				24
#define MAX_INVENTORY_H2			28
#define MAX_INVENTORY_LEN			192
#define MAX_WAREHOUSE_H				30
#define MUUN_INVENTORY_SIZE			62
#define EVENT_INVENTORY_SIZE		32
#define EVENT_INVENTORY_MAP_SIZE	32

enum MAP_INDEX
{
	MAP_INDEX_EMPTY = -1,
	MAP_INDEX_RORENCIA = 0x0,
	MAP_INDEX_DUNGEON = 0x1,
	MAP_INDEX_DEVIAS = 0x2,
	MAP_INDEX_NORIA = 0x3,
	MAP_INDEX_LOSTTOWER = 0x4,
	MAP_INDEX_RESERVED = 0x5,
	MAP_INDEX_BATTLESOCCER = 0x6,
	MAP_INDEX_ATHLANSE = 0x7,
	MAP_INDEX_TARKAN = 0x8,
	MAP_INDEX_DEVILSQUARE = 0x9,
	MAP_INDEX_ICARUS = 0xa,
	MAP_INDEX_BLOODCASTLE1 = 0xb,
	MAP_INDEX_BLOODCASTLE2 = 0xc,
	MAP_INDEX_BLOODCASTLE3 = 0xd,
	MAP_INDEX_BLOODCASTLE4 = 0xe,
	MAP_INDEX_BLOODCASTLE5 = 0xf,
	MAP_INDEX_BLOODCASTLE6 = 0x10,
	MAP_INDEX_BLOODCASTLE7 = 0x11,
	MAP_INDEX_CHAOSCASTLE1 = 0x12,
	MAP_INDEX_CHAOSCASTLE2 = 0x13,
	MAP_INDEX_CHAOSCASTLE3 = 0x14,
	MAP_INDEX_CHAOSCASTLE4 = 0x15,
	MAP_INDEX_CHAOSCASTLE5 = 0x16,
	MAP_INDEX_CHAOSCASTLE6 = 0x17,
	MAP_INDEX_KALIMA1 = 0x18,
	MAP_INDEX_KALIMA2 = 0x19,
	MAP_INDEX_KALIMA3 = 0x1a,
	MAP_INDEX_KALIMA4 = 0x1b,
	MAP_INDEX_KALIMA5 = 0x1c,
	MAP_INDEX_KALIMA6 = 0x1d,
	MAP_INDEX_CASTLESIEGE = 0x1e,
	MAP_INDEX_CASTLEHUNTZONE = 0x1f,
	MAP_INDEX_DEVILSQUARE2 = 0x20,
	MAP_INDEX_AIDA = 0x21,
	MAP_INDEX_CRYWOLF_FIRSTZONE = 0x22,
	MAP_INDEX_CRYWOLF_SECONDZONE = 0x23,
	MAP_INDEX_KALIMA7 = 0x24,
	MAP_INDEX_KANTURU1 = 0x25,
	MAP_INDEX_KANTURU2 = 0x26,
	MAP_INDEX_KANTURU_BOSS = 0x27,
	MAP_INDEX_GAMEMASTER_MAP = 0x28, //0x28 OK
	MAP_INDEX_BARRACKS = 0x29, //0x29 OK
	MAP_INDEX_REFUGEE = 0x2a, //0x2a OK
	MAP_INDEX_ILLUSIONTEMPLE1 = 0x2d, //0x2d OK
	MAP_INDEX_ILLUSIONTEMPLE2 = 0x2e, //0x2e OK
	MAP_INDEX_ILLUSIONTEMPLE3 = 0x2f, //0x32 OK
	MAP_INDEX_ILLUSIONTEMPLE4 = 0x30, //0x30 OK
	MAP_INDEX_ILLUSIONTEMPLE5 = 0x31, //0x31 OK
	MAP_INDEX_ILLUSIONTEMPLE6 = 0x32, //0x32 OK
	MAP_INDEX_ELBELAND = 0x33, // 0x33 OK
	MAP_INDEX_BLOODCASTLE8 = 0x34,
	MAP_INDEX_CHAOSCASTLE7 = 0x35,
	MAP_INDEX_CALMNESS = 0x38,
	MAP_INDEX_RAKLION = 0x39,
	MAP_INDEX_HATCHERY = 0x3A,
	MAP_INDEX_SANTAVILLAGE = 0x3E,
	MAP_INDEX_VULCAN = 0x3F,
	MAP_INDEX_VULCANROOM = 0x40,
	MAP_INDEX_DOUBLE_GOER1 = 0x41,
	MAP_INDEX_DOUBLE_GOER2 = 0x42,
	MAP_INDEX_DOUBLE_GOER3 = 0x43,
	MAP_INDEX_DOUBLE_GOER4 = 0x44,
	MAP_INDEX_IMPERIAL1	= 0x45,
	MAP_INDEX_IMPERIAL2 = 0x46,
	MAP_INDEX_IMPERIAL3 = 0x47,
	MAP_INDEX_IMPERIAL4 = 0x48,
	MAP_INDEX_LORENMARKET = 0x4f,
	MAP_INDEX_KALRUTAN1 = 0x50,
	MAP_INDEX_KALRUTAN2 = 0x51,
	MAP_INDEX_ACHERON = 0x5b,
	MAP_INDEX_ARCA_WAR = 0x5c,
	MAP_INDEX_DEBENTER = 0x5F,
	MAP_INDEX_DEBENTER_ARCA_WAR = 0x60,
	MAP_INDEX_CHAOSCASTLE_SURVIVAL = 0x61,
	MAP_INDEX_ILLUSIONTEMPLE7 = 0x62,
	MAP_INDEX_ILLUSIONTEMPLE8 = 0x63,
	MAP_INDEX_URUK_MOUNTAIN = 0x64,
	MAP_INDEX_URUK_MOUNTAIN_ARCA_WAR = 0x65,
	MAP_INDEX_DEVILSQUARE_FINAL = 0x66
};

enum EXP_GETTING_EVENT_TYPE {
	EVENT_TYPE_NONE = 0x0,
	EVENT_TYPE_PARTY = 0x1,
	EVENT_TYPE_DEVILSQUARE = 0x2,
	EVENT_TYPE_CHAOSCASTLE = 0x3,
	EVENT_TYPE_BLOODCASTLE = 0x4,
	EVENT_TYPE_CRYWOLF = 0x5,
	EVENT_TYPE_ILLUSIONTEMPLE = 0x6,
};

enum WING_CHECK_FLAG
{
	WCF_ITEMNUMBER = 100,
	WCF_ITEMTYPE = 101,
};

enum WING_TYPE
{
	NONE_WING = 10,
	FIRST_WING = 11,
	SECOND_WING = 12,
	THIRD_WING = 13,
	LITTLE_WING = 14,
	MONSTER_WRING = 15,
	CHAOSCASTLE_WING = 16,
	GOODANDDEVIL_WING = 17,
};

#define MAX_RESISTENCE_TYPE 7

#define R_ICE			0
#define R_POISON		1
#define R_LIGHTNING		2
#define R_FIRE			3
#define R_EARTH			4
#define R_WIND			5
#define R_WATER			6

#define OBJ_EMPTY -1
#define OBJ_MONSTER 2
#define OBJ_USER 1
#define OBJ_NPC	3
#define MAX_PARTY_LEVEL_DIFFERENCE	130
#define MAX_MAGIC 150
#define MAX_VIEWPORT 75
#define MAX_ROAD_PATH_TABLE 16
#define MAX_ARRAY_FRUSTRUM 4
#define PLAYER_EMPTY  0
#define PLAYER_CONNECTED 1
#define PLAYER_LOGGED 2
#define PLAYER_PLAYING 3
#define MAX_CHECKSUM_KEY 1024
#define FRIEND_SERVER_STATE_LOGIN_FAIL	0
#define FRIEND_SERVER_STATE_OFFLINE		1
#define FRIEND_SERVER_STATE_ONLINE		2

#define MAIN_INVENTORY_RANGE(x) (((x)<0)?FALSE:((x)>MAIN_INVENTORY_SIZE-1)?FALSE:TRUE )
#define INVENTORY_RANGE(x) (((x)<0)?FALSE:((x)>INVENTORY_SIZE-1)?FALSE:TRUE )
#define WAREHOUSE_RANGE(x) (((x)<0)?FALSE:((x)>WAREHOUSE_SIZE-1)?FALSE:TRUE )
#define TRADE_BOX_RANGE(x) (((x)<0)?FALSE:((x)>TRADE_BOX_SIZE-1)?FALSE:TRUE )
#define OBJMON_RANGE(x) ( ((x)<0)?FALSE:((x)>g_ConfigRead.server.GetObjectMaxMonster()-1)?FALSE:TRUE)
#define PSHOP_RANGE(x) ( (((x)-MAIN_INVENTORY_SIZE)<0)?FALSE:(((x)-MAIN_INVENTORY_SIZE)>(INVENTORY_SIZE-MAIN_INVENTORY_SIZE-1))?FALSE:TRUE )
#define ATTRIBUTE_RANGE(x) (((x)<51)?FALSE:((x)>58)?FALSE:TRUE )

#define CURRENT_DB_VERSION	3
/*
 *	CHAR SET
 *	
 *	It Contains the Character Settings
 *	CS_CLASS(0)	Contains the Class and the ChangeUP
 *				You can use CS_GET_CLASS and CS_GET_CHANGEUP
 *	MAP of CHAR SET
 *
 *	    0         1			2		  3			4		  5			6		  7
 *	0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000
 *	AAAB   CC EEEE EEEE GGGG GGGG JJJJ MMMM PPPP SSSS VVVV WWWW XXXX XXXX YYYY YYYY
 *
 *	    8         9			10		  11		12		  13		14		  15
 *	0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000
 *	ZZZZ ZZZZ ILOR U    aaaa aaad bbbb bbbc DDDD    e FFFF HHHH KKKK NNNN QQQQ TTTT
 *
 *		16		  17
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
 *	WWWW Contains Wings and Helper
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


#define CHECK_LIMIT(value, limit) ( ((value)<0)?FALSE:((value)>((limit)-1))?FALSE:TRUE  )
#define CHECK_LIMIT2(value, base, limit) ( ((value)<base)?FALSE:((value)>((limit)-1))?FALSE:TRUE  )
#define CHECK_CLASS(value, type) ( ((value)!=(type))?FALSE:TRUE )

// Old CS defines
#define CS_SET_CLASS(x)			( ((x)<<5)& 0xE0 )		 //
#define CS_GET_CLASS(x)			( (((x)>>4)<<5)&(0xE0) ) //

#define CS_SET_CHANGEUP			0x10
#define CS_SET_CHANGEUP2		0x08

#define CS_GET_CHANGEUP(x)		( ((x)&0x07) )		 //

// Season X CS defines
#define CS_SET_CLASS_SX(x)		( ((x)<<4))		 //
#define CS_GET_CLASS_SX(x)		( (((x)>>4)<<4)) //

#define CS_SET_CHANGEUP_SX		0x08
#define CS_SET_CHANGEUP2_SX		0x04

#define CS_GET_CHANGEUP_SX(x)	( ((x)&0x07) )		 //

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

#include "ViewPortGuild.h"

typedef union tagPATHTABLE
{
	short sPathTable[16];
	char cPathTable[32];
} PATHTABLE, * LPPATHTABLE;

typedef struct tagActionState
{
	unsigned long Rest:1;	// 0
	unsigned long Attack:1;	// 1
	unsigned long Move:1;	// 2
	unsigned long Escape:1;	// 3
	unsigned long Emotion:4;	// 4
	unsigned long EmotionCount:8;	// 8

} ACTION_STATE, * LPACTION_STATE;

typedef struct tagInterfaceState
{
	DWORD use	: 2;
	DWORD state : 4;
	DWORD type	: 10;

} INTERFACE_STATE, * LPINTERFACE_STATE;

extern short RoadPathTable[MAX_ROAD_PATH_TABLE];
extern BOOL g_EnergyCheckOff;

struct HITDAMAGE_STRUCT
{
	short number;	// 0
	int HitDamage;	// 4
	ULONGLONG LastHitTime;	// 8
};

struct VIEWPORT_STRUCT
{
	char state;	// 0
	short number;	// 2
	unsigned char type;	// 4
	short index;	// 6
	int dis;	// 8
};

struct VIEWPORT_PLAYER_STRUCT
{
	char state;	// 0
	short number;	// 2
	unsigned char type;	// 4
	short index;	// 6
	int dis;	// 8

};

struct MessageStateMachine
{
	int MsgCode;	// 0
	int SendUser;	// 4
	ULONGLONG MsgTime;	// 8
	int SubCode;	// C

	MessageStateMachine()
	{
		this->MsgCode = -1;
	}

};

struct ExMessageStateMachine
{
	int MsgCode;	// 0
	int SendUser;	// 4
	ULONGLONG MsgTime;	// 8
	int SubCode;	// C
	int SubCode2;	// 10

	ExMessageStateMachine()
	{
		this->MsgCode = -1;
	}
};

struct WHISPER_STRUCT
{
	bool iWhisperSent;
	WORD wResponseCount;
};
struct SKILL_INFO
{
	void Clear()
	{
		this->GhostPhantomX = 0;
		this->GhostPhantomY = 0;
		this->RemedyOfLoveEffect = 0;
		this->RemedyOfLoveTime = 0;
		this->LordSummonTime = 0;
		this->LordSummonMapNumber = 0;
		this->LordSummonX = 0;
		this->LordSummonY = 0;
		this->SoulBarrierDefence = 0;
		this->SoulBarrierManaRate = 0;
		this->PoisonType = 0;
		this->IceType = 0;
		this->fInfinityArrowIncRate = 0.0;
		this->fCircleShieldRate = 0.0;
	}

	BYTE	GhostPhantomX;
	BYTE	GhostPhantomY;
	short	RemedyOfLoveEffect;
	short	RemedyOfLoveTime;
	WORD	LordSummonTime;
	BYTE	LordSummonMapNumber;
	BYTE	LordSummonX;
	BYTE	LordSummonY;
	int		SoulBarrierDefence;
	int		SoulBarrierManaRate;
	BYTE	PoisonType;
	BYTE	IceType;
	float	fInfinityArrowIncRate;
	float	fCircleShieldRate;
};

struct _MASTERLEVEL_PASSIVE_SKILL_OPTION
{
	_MASTERLEVEL_PASSIVE_SKILL_OPTION()
	{
		this->Clear();
	}

	void Clear()
	{
		iMpsAttackSuccessRate = 0.0;
		iMpsBlockingSuccessRate = 0.0;
		iMpsPVPAttackDmgRate = 0.0;
		iMpsPVPBlockingRate = 0.0;
		iMpsDownDur1 = 0.0;
		btMpsDownDur1Level = 0.0;
		iMpsDownDur2 = 0.0;
		iMpsDownDur3 = 0.0;
		iMpsResistancePoison = 0.0;
		iMpsResistanceThunder = 0.0;
		iMpsResistanceIce = 0.0;
		iMpsAutoRecoverLife = 0.0;
		iMpsAutoRecoverMana = 0.0;
		iMpsAutoRecoverAG = 0.0;
		iMpsAddPhysicDamage = 0.0;
		iMpsAddDamage = 0.0;
		iMpsTwoHandSwordAddDamage = 0.0;
		iMpsSwordAddDamage = 0.0;
		iMpsScultAddDamage = 0.0;
		iMpsSpearAddDamage = 0.0;
		iMpsTwoHandsPvPAttack = 0.0;
		iMpsAddAttackSpeed = 0.0;
		iMpsIgnoreEnemyDefence = 0.0;
		iMpsAddDoubleDamage = 0.0;
		iMpsAddSturn = 0.0;
		iMpsMaxManaRate = 0.0;
		iMpsAddMagicDamage = 0.0;
		iMpsTwoHandsMagicPower = 0.0;
		iMpsStaffMagicPower = 0.0;
		iMpsIncShield = 0.0;
		iMpsStaffAttackSpeed = 0.0;
		iMpsTwoHandsStaffManAttack = 0.0;
		iMpsShieldBlockRate = 0.0;
		iMpsBowAddDamage = 0.0;
		iMpsBowAddAttackSpeed = 0.0;
		iMpsTwoHandsBowAddDamage = 0.0;
		iMpsTwoHandsBowManAttack = 0.0;
		iMpsElfAddShield = 0.0;
		iMpsElfShieldBlockRate = 0.0;
		iMpsElfAddPhysicDamage = 0.0;
		iMpsCallMonHp = 0.0;
		iMpsCallMonDefense = 0.0;
		iMpsCallMonAttack = 0.0;
		iMpsIncMagicPower = 0.0;
		iMpsIncDamageFireProper = 0.0;
		iMpsIncDamageWindProper = 0.0;
		iMpsIncDamageThunderProper = 0.0;
		iMpsIncDotDamage = 0.0;
		iMpsAddSturn2 = 0.0;
		iMpsAddSpring = 0.0;
		iMpsAddMagicPowerStick = 0.0;
		iMpsAddCurseMagicBook = 0.0;
		iMpsAddManAttackStick = 0.0;
		iMpsAddAttackSpeedMagicBook = 0.0;
		iMpsIncPercentBerserker1 = 0.0;
		iMpsIncPercentBerserker2 = 0.0;
		iMpsIncValueBerserker3 = 0.0;
		iMpsIncMinMagicDamage = 0.0;
		iMpsIncDarkHorseDefense = 0.0;
		iMpsAddForceWaveDamage = 0.0;
		iMpsAddDarkSpiritDamage = 0.0;
		iMpsAddCriticalDamageRate = 0.0;
		iMpsAddExcellentDamageRate = 0.0;
		iMpsAddCeptorDamage = 0.0;
		iMpsIncCeptorManAttack = 0.0;
		iMpsAddShieldDefense = 0.0;
		iMpsAddShieldBlockingRage = 0.0;
		iMpsAddPetAttack = 0.0;
		iMpsAddControlAttack = 0.0;
		iMpsAddMinAttack = 0.0;
		iMpsAddMaxAttack = 0.0;
		iMpsCriticalRateInc = 0.0;
		iMpsAddBlockingRate_Monk = 0.0;
		iMpsMonkWeaponDamage = 0.0;
		iMpsMonkDoubleDamage = 0.0;
		iMpsMonkAddVitalToDefense = 0.0;
		iMpsPlusZen = 0.0;
		iMpsDefence = 0.0;
		iMpsMaxHP = 0.0;
		iMpsMaxBP = 0.0;
		iMpsMaxMana = 0.0;
		iMpsMonsterDieGetMana = 0.0;
		iMpsMonsterDieGetLife = 0.0;
		iMpsMonsterDieGetSD = 0.0;
		iMpsPlusExp = 0.0;
		iMpsMaxSD = 0.0;
		iMpsSDSpeed = 0.0;
		iMpsMaxAttackDamage = 0.0;
		iMpsMinAttackDamage = 0.0;
		iMpsDecreaseMana = 0.0;
		iMpsMaxMagicDamage = 0.0;
		iMpsMinMagicDamage = 0.0;
		iMpsPetDurDownSpeed = 0.0;
		iMpsMaxAttackMagicDamage = 0.0;
		iMpsMinAttackMagicDamage = 0.0;
		iMpsImmuneRate = 0.0;
		iMpsIncSetItemDefense = 0.0;
		iMpsReturnEnemyAttack = 0.0;
		iMpsIncEnergyStat = 0.0;
		iMpsIncVitalStat = 0.0;
		iMpsIncDexStat = 0.0;
		iMpsIncPowerStat = 0.0;
		iMpsAddWingDamageBlock = 0.0;
		iMpsAddWingDefense = 0.0;
		iMpsAddWingDamage = 0.0;
		iMpsIncLeadershipStat = 0.0;
		iMpsIncCriticalDamageRate = 0.0;
		iMpsRecoverManaRate = 0.0;
		iMpsRecoverHPRate = 0.0;
		iMpsIncExcellentDamageRate = 0.0;
		iMpsIncDoubleDamageRate = 0.0;
		iMpsIncIgnoreEnemyBlock = 0.0;
		iMpsRecoverSDRate = 0.0;
		iMpsIncTripleDamageRate = 0.0;
		iMpsComboDamage = 0.0;
		iMpsIncMaxBP = 0.0;
		iMpsMasManaRate_Wizard = 0.0;
		iMpsIncMaxMagicDamage = 0.0;
		iMpsIncExcellentDamageRate_Darklord = 0.0;
		iMpsIncDamageBloodHowling = 0.0;
		iMpsIncDarkSpiritAttackSpeed = 0.0;
		iMpsIncDarkSpiritDoubleDamageRate = 0.0;
		iMpsDarkSpiritIgnoreEnemyDefense = 0.0;
		iMpsDownDur1_Monk = 0.0;
		btMpsDownDur1Level_Monk = 0.0;
		iMpsPVPBlockingRate_Monk = 0.0;
		iMpsMaxSD_Monk = 0.0;
		iMpsAutoRecoverMana_Monk = 0.0;
		iMpsResistancePoison_Monk = 0.0;
		iMpsDownDur2_Monk = 0.0;
		iMpsSDSpeed_Monk = 0.0;
		iMpsAutoRecoverLife_Monk = 0.0;
		iMpsResistanceThunder_Monk = 0.0;
		iMpsDefence_Monk = 0.0;
		iMpsAutoRecoverAG_Monk = 0.0;
		iMpsResistanceIce_Monk = 0.0;
		iMpsDownDur3_Monk = 0.0;
		iMpsBlockingSuccessRate_Monk = 0.0;
		iMpsImmuneRate_Monk = 0.0;
		iMpsIncSetItemDefense_Monk = 0.0;
		iMpsReturnEnemyAttack_Monk = 0.0;
		iMpsIncEnergyStat_Monk = 0.0;
		iMpsIncVitalStat_Monk = 0.0;
		iMpsIncDexStat_Monk = 0.0;
		iMpsIncPowerStat_Monk = 0.0;
		iMpsAttackSuccessRate_Monk = 0.0;
		iMpsMaxHP_Monk = 0.0;
		iMpsMaxMana_Monk = 0.0;
		iMpsMaxBP_Monk = 0.0;
		iMpsPVPAttackDmgRate_Monk = 0.0;
		iMpsDecreaseMana_Monk = 0.0;
		iMpsMonsterDieGetSD_Monk = 0.0;
		iMpsMonsterDieGetLife_Monk = 0.0;
		iMpsAddMinAttack_Monk = 0.0;
		iMpsMonsterDieGetMana_Monk = 0.0;
		iMpsAddMaxAttack_Monk = 0.0;
		iMpsIncCriticalDamageRate_Monk = 0.0;
		iMpsRecoverManaRate_Monk = 0.0;
		iMpsRecoverHPRate_Monk = 0.0;
		iMpsIncExcellentDamageRate_Monk = 0.0;
		iMpsIncDoubleDamageRate_Monk = 0.0;
		iMpsIncIgnoreEnemyBlock_Monk = 0.0;
		iMpsRecoverSDRate_Monk = 0.0;
		iMpsIncTripleDamageRate_Monk = 0.0;
	}

	float	iMpsAttackSuccessRate; // done
	float	iMpsBlockingSuccessRate; // done
	float	iMpsPVPAttackDmgRate; // done
	float	iMpsPVPBlockingRate; // done
	float	iMpsDownDur1; // done
	BYTE	btMpsDownDur1Level; // done
	float	iMpsDownDur2; // done
	float	iMpsDownDur3; // done
	float	iMpsResistancePoison; // done
	float	iMpsResistanceThunder; // done
	float	iMpsResistanceIce; // done
	float	iMpsAutoRecoverLife; // done
	float	iMpsAutoRecoverMana; // done
	float	iMpsAutoRecoverAG; // done
	float	iMpsAddPhysicDamage; // done
	float	iMpsAddDamage; // not used?
	float	iMpsTwoHandSwordAddDamage; // done
	float	iMpsSwordAddDamage; // done
	float	iMpsScultAddDamage; // done
	float	iMpsSpearAddDamage; // done
	float	iMpsTwoHandsPvPAttack; // done
	float	iMpsAddAttackSpeed; // done
	float	iMpsIgnoreEnemyDefence; // done, but no reference in CMasterLevelSkillTreeSystem class
	float	iMpsAddDoubleDamage; // done
	float	iMpsAddSturn; // done
	float	iMpsMaxManaRate; // done
	float	iMpsAddMagicDamage; // done
	float	iMpsTwoHandsMagicPower; // done
	float	iMpsStaffMagicPower; // done
	float	iMpsIncShield; // done
	float	iMpsStaffAttackSpeed; // done
	float	iMpsTwoHandsStaffManAttack;  // done
	float	iMpsShieldBlockRate; // done
	float	iMpsBowAddDamage; // done
	float	iMpsBowAddAttackSpeed; // done
	float	iMpsTwoHandsBowAddDamage; // done
	float	iMpsTwoHandsBowManAttack; // done
	float	iMpsElfAddShield; // done
	float	iMpsElfShieldBlockRate; // done
	float	iMpsElfAddPhysicDamage; // done
	float	iMpsCallMonHp; // done
	float	iMpsCallMonDefense; // done
	float	iMpsCallMonAttack; // done
	float	iMpsIncMagicPower; // done
	float	iMpsIncDamageFireProper; // done
	float	iMpsIncDamageWindProper; // done
	float	iMpsIncDamageThunderProper; // done
	float	iMpsIncDotDamage; // done
	float	iMpsAddSturn2; // done
	float	iMpsAddSpring; // done
	float	iMpsAddMagicPowerStick; // done
	float	iMpsAddCurseMagicBook; // done
	float	iMpsAddManAttackStick; // done
	float	iMpsAddAttackSpeedMagicBook; // done
	float	iMpsIncPercentBerserker1; // done
	float	iMpsIncPercentBerserker2; // done
	float	iMpsIncValueBerserker3; // done
	float	iMpsIncMinMagicDamage; // done
	float	iMpsIncDarkHorseDefense; // done
	float	iMpsAddForceWaveDamage; // not used
	float	iMpsAddDarkSpiritDamage; // done
	float	iMpsAddCriticalDamageRate; // done
	float	iMpsAddExcellentDamageRate; // done
	float	iMpsAddCeptorDamage; // done
	float	iMpsIncCeptorManAttack; // done
	float	iMpsAddShieldDefense; // done
	float	iMpsAddShieldBlockingRage; // done
	float	iMpsAddPetAttack; // done
	float	iMpsAddControlAttack; // done
	float	iMpsAddMinAttack; // done
	float	iMpsAddMaxAttack; // done
	float	iMpsCriticalRateInc; // done
	float	iMpsAddBlockingRate_Monk; // done
	float	iMpsMonkWeaponDamage; // done
	float	iMpsMonkDoubleDamage; // done
	float	iMpsMonkAddVitalToDefense; // done
	float	iMpsPlusZen; // done, but no reference in CMasterLevelSkillTreeSystem class
	float	iMpsDefence; // done
	float	iMpsMaxHP; // done
	float	iMpsMaxBP; // done
	float	iMpsMaxMana; // done
	float	iMpsMonsterDieGetMana; // done
	float	iMpsMonsterDieGetLife; // done
	float	iMpsMonsterDieGetSD; // done
	float	iMpsPlusExp; // not used
	float	iMpsMaxSD; // done
	float	iMpsSDSpeed; // done
	float	iMpsMaxAttackDamage; // done
	float	iMpsMinAttackDamage; // done
	float	iMpsDecreaseMana; // done
	float	iMpsMaxMagicDamage; // done
	float	iMpsMinMagicDamage; // done
	float	iMpsPetDurDownSpeed; // done
	float	iMpsMaxAttackMagicDamage; // done, but no reference in CMasterLevelSkillTreeSystem class
	float	iMpsMinAttackMagicDamage; // done, but no reference in CMasterLevelSkillTreeSystem class
	float	iMpsImmuneRate; // done
	float	iMpsIncSetItemDefense; // done
	float	iMpsReturnEnemyAttack; // done
	float	iMpsIncEnergyStat; // done
	float	iMpsIncVitalStat; // done
	float	iMpsIncDexStat; // done
	float	iMpsIncPowerStat; // done
	float	iMpsAddWingDamageBlock; // done
	float	iMpsAddWingDefense; // done
	float	iMpsAddWingDamage; // done
	float	iMpsIncLeadershipStat; // done
	float	iMpsIncCriticalDamageRate; // done
	float	iMpsRecoverManaRate; // done
	float	iMpsRecoverHPRate; // done
	float	iMpsIncExcellentDamageRate; // done
	float	iMpsIncDoubleDamageRate; // done
	float	iMpsIncIgnoreEnemyBlock; // done
	float	iMpsRecoverSDRate; // done
	float	iMpsIncTripleDamageRate; // done
	float	iMpsComboDamage; // done
	float	iMpsIncMaxBP; // done
	float	iMpsMasManaRate_Wizard; // done
	float	iMpsIncMaxMagicDamage; // done
	float	iMpsIncExcellentDamageRate_Darklord; // done
	float	iMpsIncDamageBloodHowling; // done
	float	iMpsIncDarkSpiritAttackSpeed; // done
	float	iMpsIncDarkSpiritDoubleDamageRate; // done
	float	iMpsDarkSpiritIgnoreEnemyDefense; // done
	// MONK done
	float	iMpsDownDur1_Monk;
	BYTE	btMpsDownDur1Level_Monk;
	float	iMpsPVPBlockingRate_Monk;
	float	iMpsMaxSD_Monk;
	float	iMpsAutoRecoverMana_Monk;
	float	iMpsResistancePoison_Monk;
	float	iMpsDownDur2_Monk;
	float	iMpsSDSpeed_Monk;
	float	iMpsAutoRecoverLife_Monk;
	float	iMpsResistanceThunder_Monk;
	float	iMpsDefence_Monk;
	float	iMpsAutoRecoverAG_Monk;
	float	iMpsResistanceIce_Monk;
	float	iMpsDownDur3_Monk;
	float	iMpsBlockingSuccessRate_Monk;
	float	iMpsImmuneRate_Monk;
	float	iMpsIncSetItemDefense_Monk;
	float	iMpsReturnEnemyAttack_Monk;
	float	iMpsIncEnergyStat_Monk;
	float	iMpsIncVitalStat_Monk;
	float	iMpsIncDexStat_Monk;
	float	iMpsIncPowerStat_Monk;
	float	iMpsAttackSuccessRate_Monk;
	float	iMpsMaxHP_Monk;
	float	iMpsMaxMana_Monk;
	float	iMpsMaxBP_Monk;
	float	iMpsPVPAttackDmgRate_Monk;
	float	iMpsDecreaseMana_Monk;
	float	iMpsMonsterDieGetSD_Monk;
	float	iMpsMonsterDieGetLife_Monk;
	float	iMpsAddMinAttack_Monk;
	float	iMpsMonsterDieGetMana_Monk;
	float	iMpsAddMaxAttack_Monk;
	float	iMpsIncCriticalDamageRate_Monk;
	float	iMpsRecoverManaRate_Monk;
	float	iMpsRecoverHPRate_Monk;
	float	iMpsIncExcellentDamageRate_Monk;
	float	iMpsIncDoubleDamageRate_Monk;
	float	iMpsIncIgnoreEnemyBlock_Monk;
	float	iMpsRecoverSDRate_Monk;
	float	iMpsIncTripleDamageRate_Monk;
};

struct EFFECTLIST
{
	BYTE BuffIndex;
	BYTE EffectCategory;
	BYTE EffectType1;
	BYTE EffectType2;
	int EffectValue1;
	int EffectValue2;
	ULONGLONG EffectSetTime;
	int EffectDuration;
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

struct PENTAGRAM_OPTION
{
	int m_iRuby_1RankAddDamage;
	int m_iRuby_2RankOptionNum;
	int m_iRuby_2RankAddAttackRelationshipRate;
	int m_iRuby_3RankOptionNum;
	int m_iRuby_3RankAddDamage;
	int m_iRuby_4RankOptionNum;
	int m_iRuby_4RankAddDamage;
	int m_iRuby_5RankOptionNum;
	int m_iRuby_5RankCriticalDamageRate;
	int m_iSapph_1RankAddDefense;
	int m_iSapph_2RankOptionNum;
	int m_iSapph_2RankAddDefenseRelationshipRate;
	int m_iSapph_3RankOptionNum;
	int m_iSapph_3RankAddDefense;
	int m_iSapph_4RankOptionNum;
	int m_iSapph_4RankAddDefense;
	int m_iSapph_5RankOptionNum;
	int m_iSapph_5RankMinusTargetDamageRate;
	int m_iEme_1RankAddAttackRate;
	int m_iEme_2RankOptionNum;
	int m_iEme_2RankAddAttackRelationshipRate;
	int m_iEme_3RankOptionNum;
	int m_iEme_3RankAddDamage;
	int m_iEme_4RankOptionNum;
	int m_iEme_4RankAddDamage;
	int m_iEme_5RankOptionNum;
	int m_iEme_5RankAddDamageRate;
	int m_iTopa_1RankOptionNum;
	int m_iTopa_1RankAddDefenseSuccessRate;
	int m_iTopa_2RankOptionNum;
	int m_iTopa_2RankAddDefenseRelationshipRate;
	int m_iTopa_3RankOptionNum;
	int m_iTopa_3RankAddDefense;
	int m_iTopa_4RankOptionNum;
	int m_iTopa_4RankAddDefense;
	int m_iTopa_5RankOptionNum;
	int m_iTopa_5RankDamageAbsorbRate;
	int m_iOnyx_1RankSlowMoveSkillRate;
	int m_iOnyx_2RankAddStrength;
	int m_iOnyx_2RankAddDexterity;
	int m_iOnyx_2RankAddEnergy;
	int m_iOnyx_2RankAddVitality;
	int m_iOnyx_3RankAddMaxLife;
	int m_iOnyx_3RankAddMaxMana;
	int m_iOnyx_3RankAddMaxAG;
	int m_iOnyx_3RankAddMaxSD;
	int m_iOnyx_4RankOptionNum;
	int m_iOnyx_4RankAddExllentDamageRate;
	int m_iOnyx_5RankHalfValueSkillRate;
	bool m_isAddPentaAttack;
	bool m_isAddPentaDefense;
	bool m_isAddCriPentaDamage;
	bool m_isChangePentaDefense;
	bool m_isChangePentaAttack;
	bool m_isAddResistByStrongRelationShip;
	bool m_isAddResistByPentaAttack;
};

struct _BOT_BUFF_LIST
{
	WORD wBuffId;
	WORD wDuration;
	int iEffect;
	WORD wEffectType;
};

struct EXC_WING_OPTION
{
	EXC_WING_OPTION()
	{
		this->Clear();
	}

	void Clear()
	{
		this->iWingOpIgnoreEnemyDefense = 0;
		this->iWingOpReturnEnemyDamage = 0;
		this->iWingOpRecoveryHP = 0;
		this->iWingOpRecoveryMana = 0;
		this->iWingOpAddDoubleDamage = 0;
	}

	int iWingOpIgnoreEnemyDefense;
	int iWingOpReturnEnemyDamage;
	int iWingOpRecoveryHP;
	int iWingOpRecoveryMana;
	int iWingOpAddDoubleDamage;
};

struct STAT_USER_OPTION
{
	STAT_USER_OPTION()
	{
		this->Clear();
	}

	void Clear()
	{
		this->StatOptionID = 0;
		this->StatOptionValMin = 0;
		this->StatOptionValMax = 0;
	}

	WORD StatOptionID;
	WORD StatOptionValMin;
	WORD StatOptionValMax;
};

struct _tagMUUN_EFFECT_LIST
{
	_tagMUUN_EFFECT_LIST()
	{
		this->Clear();
	}

	void Clear()
	{
		this->nIndex = 0;
		this->nMuunItemNum = 0;
		this->nCategory = 0;
		this->bOptEnable = 0;
		this->nOptType = 0;
		this->nOptValue = 0;
		this->bAddOptEnable = 0;
		this->nAddOptType = 0;
		this->nAddOptValue = 0;
		this->nSetTime = 0;
		this->nDuration = 0;
		this->pCMuunInfo = NULL;
		this->nTotalVal = 0;
		this->nSkillDelayTime = 0;
		this->bSkillUsed = false;
		this->nTargetIndex = -1;
		this->nTickTime = 0;
	}

	int nIndex;
	int nMuunItemNum;
	char nCategory;
	bool bOptEnable;
	char nOptType;
	int nOptValue;
	bool bAddOptEnable;
	char nAddOptType;
	int nAddOptValue;
	DWORD nSetTime;
	int nDuration;
	CMuunInfo *pCMuunInfo;
	int nTotalVal;
	int nSkillDelayTime;
	int nTickTime;
	bool bSkillUsed;
	int nTargetIndex;
};

struct _tagMOVE_MAPSERVER_AUTHINFO
{
	char szCharName[MAX_ACCOUNT_LEN+2];
	int iJA1;
	int iJA2;
	int iJA3;
	int iJA4;
};

struct CharacterNameOfUBF
{
	void Clear()
	{
		memset(this->szCharacterName, 0, MAX_ACCOUNT_LEN+1);
		memset(this->szCharacterNameOfUBF, 0, MAX_ACCOUNT_LEN+1);
		this->nServerCodeOfHomeWorld = 0;
	}

	char szCharacterName[MAX_ACCOUNT_LEN+1];
	char szCharacterNameOfUBF[MAX_ACCOUNT_LEN+1];
	WORD nServerCodeOfHomeWorld;
};

class CNameConvert
{
public:
	void InitData();
	void InputData(int cnt, const char *name, const char *realNameOfUBF, WORD serverCode, int Length);
	char * ConvertNameToRealName(const char *name);
	char * ConvertRealNameToName(const char *realName);
	WORD FindServerCodeAtRealName(const char *realName);
	WORD FindServerCodeAtCharacterName(const char *Name);
	char * ConvertSlotIndexToName(int slotindex);
	int IsDuplicationOfName();

private:
	CharacterNameOfUBF m_perCharacterInfo[5];
};

extern ExMessageStateMachine ** gSMAttackProcMsg;
extern MessageStateMachine ** gSMMsg;


class USER_DATA
{
public:
	USER_DATA();
	virtual ~USER_DATA();

	void Init(bool VipReset = true);

public:
	char Ip_addr[16];	// 18
	char HWID[100];
	UINT64 Experience;	// AC
	UINT64 NextExp; // B0
	UINT64 MasterExperience;
	UINT64 MasterNextExp;
	short MasterLevel;
	int LevelUpPoint; // A4
	int	MasterPoint;
	int	MasterPointUsed;
	int   iFruitPoint;	// A8
	int   Money; // B4
	WORD Strength; // B8
	WORD Dexterity;	// BA
	WORD Vitality;	// BC
	WORD Energy;	// BE
	BYTE  DbClass;	// 9E
	BYTE  ChangeUP; // 9F
	_GUILD_INFO_STRUCT * lpGuild;	// 2D4
	char GuildName[11]; // 2D8
	int GuildStatus;	// 2E4
	int iGuildUnionTimeStamp;	// 2E8
	int GuildNumber;	// 2D0
	DWORD LastMoveTime;
	int m_iResets;
	BYTE VipType;
	char VipEffect;
	BYTE SantaCount;
	DWORD m_GoblinTime;
	bool m_bSecurityCheck;
	int m_iSecurityCode;
	BYTE RegisterdLMS;
	BYTE RegisteredLMSRoom;
	JEWELOFHARMONY_ITEM_EFFECT m_JewelOfHarmonyEffect;	// 1914
	ITEMOPTION_FOR380ITEM_EFFECT m_ItemOptionExFor380;	// 193A
	BOOL	m_bKanturuEntranceByNPC;
	bool m_iGensInfoLoad;
	bool m_iQuestInfoLoad;
	int m_WCoinP;
	int m_WCoinC;
	int m_GoblinPoint;
	int m_iPeriodItemEffectIndex;	// 195C
	SOCKET_OPTION_LIST m_SeedOptionList[35]; // Size=210 Offset=9392
	SOCKET_OPTION_LIST m_BonusOptionList[7]; // Size=42 Offset=9602
	SOCKET_OPTION_LIST m_SetOptionList[2]; // Size=12 Offset=9644
	unsigned short m_btRefillHPSocketOption; // Size=2 Offset=9656
	unsigned short m_btRefillMPSocketOption; // Size=2 Offset=9658
	unsigned short m_wSocketOptionMonsterDieGetHp; // Size=2 Offset=9660
	unsigned short m_wSocketOptionMonsterDieGetMana; // Size=2 Offset=9662
	unsigned char m_btAGReduceRate; // Size=1 Offset=9664
	bool m_MuBotEnable;
	DWORD m_MuBotTotalTime;
	BYTE m_MuBotPayTime;
	DWORD m_MuBotTick;
	BYTE m_InventoryExpansion;
	BYTE m_WarehouseExpansion;
	DWORD LastAuthTime;
	BYTE LastXorKey1[4];
	BYTE LastXorKey2[4];
	bool ISBOT;
	WORD wBotIndex;
	BYTE SkillHellFire2State;	// FA4
	BYTE SkillHellFire2Count;	// FA5
	DWORD SkillHellFire2Time;	// FA8
	BYTE SkillStrengthenHellFire2State;	// FA4
	BYTE SkillStrengthenHellFire2Count;	// FA5
	DWORD SkillStrengthenHellFire2Time;	// FA8
	unsigned char m_ReqWarehouseOpen;	// FAC
	short SetOpAddSkillAttack;	// FB4
	short SetOpAddExDamage;	// FB6
	short SetOpAddExDamageSuccessRate;	// FB8
	short SetOpAddCriticalDamage;	// FBA
	short SetOpAddCriticalDamageSuccessRate;	// FBC
	short SetOpIncAGValue;	// FBE
	short SetOpAddDamage;	// FC0
	short SetOpAddMinAttackDamage;	// FC2
	short SetOpAddMaxAttackDamage;	// FC4
	short SetOpAddAttackDamage;	// FC6
	short SetOpAddDefence;	// FC8
	short SetOpAddDefenceRate;	// FCA
	short SetOpAddMagicPower;	// FCC
	BYTE SetOpIgnoreDefense;	// FCE
	BYTE SetOpDoubleDamage;	// FCF
	BYTE SetOpTwoHandSwordImproveDamage;	// FD0
	BYTE SetOpImproveSuccessAttackRate;	// FD1
	BYTE SetOpReflectionDamage;	// FD2
	BYTE SetOpImproveSheldDefence;	// FD3
	BYTE SetOpDecreaseAG;	// FD4
	BYTE SetOpImproveItemDropRate;	// FD5
	bool IsFullSetItem;	// FD6
	DWORD PotionTime;
	int PacketsPerSecond;
	DWORD PacketTime;
	DWORD LivePacketTime;
	BYTE bt_BotType;
	BYTE LiveCheckDetectCount;
	char  Password[20];	// 7E
	bool  PlusStatQuestClear; // 8C
	bool  ComboSkillquestClear;	// 8D
	ComboSkillData comboSkill; // 90
	int m_GensRanking;
	int m_GensClass;
	int m_ContributePoint;
	int m_NextContributePoint;
	int m_GensInfluence;
	int m_UserBattleZoneEnable;
	char KillUserName[11 * MAX_ACCOUNT_LEN];
	int m_KillUserCnt[10];
	int m_KillUserTotCnt;
	ULONGLONG m_GensAbusingTick;
	bool GensReward;
	char SwearWarning;
	BYTE m_Quest[100];	// EE0
	bool m_SendQuestInfo;	// F12
	DWORD AgilityCheckDelay;
	BYTE AgilityCheckCount;
	CRITICAL_SECTION AgiCheckCriti;
	BYTE RFSkillCount;
	WORD DarkSideCount;
	WORD DarkSideTarget[5];
	CRITICAL_SECTION m_DarkSideRFCriti;
	CRITICAL_SECTION m_MasterSkillCriti;
	BYTE m_btSculptPos;
	BYTE m_btCharmPos;
	BYTE m_btArtifactPos;
	BYTE m_btExpUpCharmPos;
	int IGC_KillCount;
	WORD MoveGate;
	BOOL PentagramInfoLoad;
	bool BlessSkillCast;
	DWORD m_dwMSBFlagAttackDisable;
	int m_iMiningLevel;
	int m_iMiningObjectId;
	int m_iMiningJewelId;
	_MASTERLEVEL_PASSIVE_SKILL_OPTION m_MPSkillOpt;
	UserQuestInfo m_UserQuestInfo[MAX_QUESTEXP_USER_INFO];
	int m_bUserQuestInfoSent;
	bool m_bUsePolymorphSkin;
	ULONGLONG LastSkillUseTick;
	DWORD LastSkillUseNumber;
	DWORD LastSkillUseCount;
	PENTAGRAM_OPTION m_PentagramOptions;
	class CItem * pPentagramMixBox;
	unsigned char * pPentagramMixBoxMap;
	int PentagramMixMoney;
	int PentagramMixSuccessRate;
	int m_PentagramMixLock;
	bool m_bIsPentagramMixCompleted;
	PENTAGRAMJEWEL_INFO m_PentagramJewelInfo_Inven[250];
	PENTAGRAMJEWEL_INFO m_PentagramJewelInfo_Warehouse[250];
	DWORD MathAuthenElapsed;
	DWORD AntiHackCheckTime;
	int m_AttackRatePvM;
	double m_AttackRatePvP;
	double m_DefenseRatePvP;
	bool bActiveSetParty;
	char PartyPassword[10];
	bool m_bUseGuildMatching;
	bool m_bUsePartyMatching;
	bool m_bPartyMatchingLeader;
	int m_nServerChannel;
	int m_nPartyMatchingIndex;
	int m_nPartyMatchingMemberCount;
	CMuRummyInfo *m_pCMuRummyInfo;
	bool m_bSendMemberPos;
	bool m_bSendNpcPos;
	int  m_Resistance_Stun;
	int  m_Resistance_Double;
	int  m_Resistance_Perfect;
	int  m_Resistance_SD;
	int  m_Resistance_Excellent;
	int  m_Resistance_Critical;
	bool m_bIsMining;
	bool m_EnableUseChangeSkin;
	DWORD m_dwMapMoveKeyValue;
	EXC_WING_OPTION m_WingExcOption;
	STAT_USER_OPTION m_StatSpecOption[12];
	bool m_IsOffLevelling;
	USER_SHOP_REBUY_ITEM m_CancelItemSaleList[MAX_CANCEL_ITEMS_SALE];
	bool m_bIsCancelItemSale;
	int m_JoinUnityBattle;
	int m_nServerCodeOfHomeWorld;
	char m_RealNameOfUBF[11];
	CNameConvert m_NameConvertOfUBF;
	bool m_bGremoryCaseDBInfo;
	GREMORYCASE_ITEM_DATA m_GremoryCaseData[MAX_GREMORYCASE_STORAGE_TYPES][MAX_GREMORYCASE_STORAGE_ITEMS];
	CSkillDelay SkillDelay;
	DWORD m_dwGensHitTeleportDelay; // Private custom for joinmu
	DWORD dwOpenWareCnt;
	DWORD dwCloseWareCnt;
	bool bHasSameSerialItem;
	DWORD dwHitHackCounter1;
	DWORD dwLastHitHackValue1[7];
	DWORD dwHitHackCounter2;
	DWORD dwLastHitHackValue2[7];
	DWORD64 dwLastHitHackTick;
	int Ruud;
};

enum NPC_TYPES
{
	NPC_NONE = 0,
	NPC_SHOP = 1,
	NPC_WAREHOUSE = 2,
	NPC_CHAOSMIX = 3,
	NPC_GOLDARCHER = 4,
	NPC_PENTAGRAMMIX = 5,
};

enum GM_SYSTEM_FLAGS
{
	GM_EVENT_MODIFY = 0x01,
	GM_COMMAND_FIRECRACKER = 0x02,
	GM_COMMAND_DC_MOVE = 0x04,
	GM_COMMAND_GUILD = 0x08,
	GM_COMMAND_BATTLESOCCER = 0x10,
	GM_COMMAND_ITEM_CREATE = 0x20,
	GM_COMMAND_PK_MODIFY = 0x40,
	GM_COMMAND_INVISIBLE = 0x80,
	GM_COMMAND_SKIN = 0x100,
	GM_COMMAND_BAN = 0x200,
	GM_USE_GMSHOP = 0x400,
	GM_MONSTERS_INVISIBLE = 0x800
};

class OBJECTSTRUCT
{
public:

	int m_Index;	// 0
	int Connected;	// 4
	char LoginMsgSnd;	// 8
	char LoginMsgCount;	// 9
	char CloseCount;	// A
	char CloseType;	// B
	bool bEnableDelCharacter;	// C
	_PER_SOCKET_CONTEXT* PerSocketContext;	// 10
	unsigned int m_socket;	// 14
	int UserNumber;	// 28
	int DBNumber;	// 2C
	unsigned char EnableCharacterCreate;
	DWORD AutoSaveTime;	// 34
	DWORD ConnectCheckTime;	// 38
	DWORD CheckTick;	// 3C
	unsigned char CheckSpeedHack;	// 40
	DWORD CheckTick2;	// 44
	unsigned char CheckTickCount;	// 48
	int iPingTime;	// 50
	unsigned char m_TimeCount;	// 54
	DWORD m_dwPKTimer;	// 58
	short CheckSumTableNum;	// 5C
	DWORD CheckSumTime;	// 60
	WORD Type;	// 64
	unsigned char Live;	// 66
	char  AccountID[11];	// 68
	char  Name[11];	// 73
	WORD  Class; // 9C
	short Level; // A0
	double Life; // C0
	double MaxLife; // C4
	int   m_iScriptMaxLife;	// C8
	double FillLife;	// CC
	double FillLifeMax;	// D0
	double Mana;	// D4
	double MaxMana;	// D8
	WORD  Leadership;	// DC
	WORD  AddLeadership;	// DE
	WORD  ChatLimitTime;	// E0
	BYTE  ChatLimitTimeSec;	// E2
	BYTE  FillLifeCount;	// E3
	int AddStrength;	// E4
	int AddDexterity;	// E6
	int AddVitality;	// E8
	int AddEnergy;	// EA
	int   BP;	// EC
	int   MaxBP;	// F0
	int	  AddBP;	// F4
	float VitalityToLife;	// F8
	float EnergyToMana;	// FC
	int  m_PK_Count;	// 100
	char  m_PK_Level;	// 101
	int   m_PK_Time;	// 104
	int	  m_PK_TotalCount;
	short X; // 108
	short Y; // 10A
	BYTE  Dir; // 10C
	BYTE  MapNumber; // 10D
	short SaveX;
	short SaveY;
	char  SaveMapNumber;
	short DieX;
	short DieY;
	char  DieMapNumber;
	int AddLife;	// 110
	int AddMana;	// 114
	int   iShield;	// 118
	int   iMaxShield;	// 11C
	int   iAddShield;	// 120
	int   iFillShieldMax;	// 124
	int   iFillShield;	// 128
	int   iFillShieldCount;	// 12C
	DWORD dwShieldAutoRefillTimer;	// 130
	BYTE  DamageMinus;	// 134
	BYTE  DamageReflect;	// 135
	short MonsterDieGetMoney;	// 136
	BYTE  MonsterDieGetLife;	// 138
	BYTE  MonsterDieGetMana;	// 139
	BYTE  AutoHPRecovery;
	BYTE  StartX;	// 13A
	BYTE  StartY;	// 13B
	short m_OldX;	// 13C
	short m_OldY;	// 13E
	short TX;	// 140
	short TY;	// 142
	short MTX;	// 144
	short MTY;	// 146
	int   PathCount;	// 148
	int   PathCur;	// 14C
	char  PathStartEnd;	// 150
	short PathOri[15];	// 152
	short PathX[15];	// 170
	short PathY[15];	// 18E
	char  PathDir[15];	// 1AC
	DWORD PathTime; // 1BC
	DWORD Authority;	// 1C4
	DWORD AuthorityCode;	// 1C8
	DWORD Penalty;	// 1CC
	DWORD GameMaster;
	DWORD PenaltyMask;
	time_t ChatBlockTime;
	BYTE  m_cAccountItemBlock; // 1D0
	ACTION_STATE m_ActState;	// 1D4
	BYTE  m_ActionNumber;	// 1D8
	DWORD m_ActionTime;
	BYTE  m_ActionCount;
	DWORD m_ChatFloodTime;
	BYTE  m_ChatFloodCount;
	DWORD m_State;	// 1DC
	unsigned char m_Rest;	// 1E1
	BYTE		m_ViewState;
	BYTE		m_BuffEffectCount;
	EFFECTLIST	m_BuffEffectList[MAX_BUFFEFFECT];
	DWORD m_LastMoveTime;	// 1E8
	DWORD m_LastAttackTime;	// 1EC
	BYTE m_FriendServerOnline;	// 1F0
	int m_DetectSpeedHackTime;	// 1F4
	unsigned long m_SumLastAttackTime;	// 1F8
	unsigned long m_DetectCount;	// 1FC
	int m_DetectedHackKickCount;	// 200
	int m_SpeedHackPenalty;	// 204
	unsigned char m_AttackSpeedHackDetectedCount;	// 208
	unsigned long m_PacketCheckTime;	// 20C
	unsigned char m_ShopTime;	// 210
	unsigned long m_TotalAttackTime;	// 214
	int m_TotalAttackCount;	// 218
	unsigned long TeleportTime;	// 21C
	char Teleport;	// 220
	char KillerType;	// 221
	char DieRegen;	// 222
	char RegenOk;	// 223
	BYTE  RegenMapNumber;	// 224
	BYTE  RegenMapX;	// 225
	BYTE  RegenMapY;	// 226
	DWORD RegenTime;	// 228
	DWORD MaxRegenTime;	// 22C
	short m_PosNum;	// 230
	DWORD LifeRefillTimer;	// 234
	DWORD CurActionTime;	// 238
	DWORD NextActionTime;	// 23C
	DWORD DelayActionTime;	// 240
	char DelayLevel;	// 244
	char m_iMonsterBattleDelay;	// 24B
	char m_cKalimaGateExist; // 24C
	int m_iKalimaGateIndex; // 250
	char m_cKalimaGateEnterCount;	// 254
	OBJECTSTRUCT * lpAttackObj;	// 258
	short m_SkillNumber;	// 25C
	DWORD m_SkillTime;	// 260
	bool m_bAttackerKilled;	// 264
	char m_ManaFillCount;	// 265
	char m_LifeFillCount;	// 266
	int SelfDefense[MAX_SELF_DEFENSE];	// 268
	DWORD SelfDefenseTime[MAX_SELF_DEFENSE];	// 27C
	DWORD MySelfDefenseTime;	// 290
	int PartyNumber; // 2C8
	int PartyTargetUser;	// 2CC
	char Married;
	char MarryName[11];
	char MarryRequested;
	int WinDuels;
	int LoseDuels;
	short MarryRequestIndex;
	DWORD MarryRequestTime;
	int m_RecallMon;	// 2EC
	int m_Change;	// 2F0
	short TargetNumber;	// 2F4
	short TargetNpcNumber;	// 2F6
	short LastAttackerID;	// 2FA
	int m_AttackDamageMin;	// 2FC
	int m_AttackDamageMax;	// 300
	int m_MagicDamageMin;	// 304
	int m_MagicDamageMax;	// 308
	int m_CurseDamageMin;
	int m_CurseDamageMax;
	int m_AttackDamageLeft;	// 30C
	int m_AttackDamageRight;	// 310
	int m_AttackDamageMaxLeft;	// 314
	int m_AttackDamageMinLeft;	// 318
	int m_AttackDamageMaxRight;	// 31C
	int m_AttackDamageMinRight;	// 320
	int m_AttackRating;	// 324
	int m_AttackSpeed;	// 328
	int m_MagicSpeed;	// 32C
	int m_Defense;	// 330
	int m_MagicDefense;	// 334
	int m_SuccessfulBlocking;	// 338
	int m_CurseSpell;
	short m_MoveSpeed;	// 33C
	short m_MoveRange;	// 33E
	short m_AttackRange;	// 340
	short m_AttackType;	// 342
	short m_ViewRange;	// 344
	short m_Attribute;	// 346
	short m_ItemRate;	// 348
	short m_MoneyRate;	// 34A
	int m_CriticalDamage;	// 34C
	int m_ExcelentDamage;	// 350
	CMagicInf * m_lpMagicBack;	// 354
	CMagicInf * Magic;	// 358
	char MagicCount;	// 35C
	unsigned char UseMagicNumber;	// 35D
	unsigned long UseMagicTime;	// 360
	char UseMagicCount;	// 364
	short OSAttackSerial;	// 366
	unsigned char SASCount;	// 368
	DWORD SkillAttackTime;	// 36C
	char CharSet[18]; 	// 370
	char m_Resistance[MAX_RESISTENCE_TYPE];	// 382
	char m_AddResistance[MAX_RESISTENCE_TYPE];	// 389
	int FrustrumX[MAX_ARRAY_FRUSTRUM];	// 390
	int FrustrumY[MAX_ARRAY_FRUSTRUM]; // 3A4
	VIEWPORT_STRUCT * VpPlayer;	// 3B0
	VIEWPORT_PLAYER_STRUCT * VpPlayer2;	// 734
	int VPCount; // AB8
	int VPCount2;	// ABC
	HITDAMAGE_STRUCT * sHD;	// AC0
	short sHDCount;	// CA0
	tagInterfaceState m_IfState;	// CA4
	DWORD m_InterfaceTime;	// CA8
	CItem * pInventory; // CAC
	LPBYTE  pInventoryMap; // CB0
	char * pInventoryCount;	// CB4
	char pTransaction;	// CB8
	CItem * Inventory1;	//CBC
	LPBYTE  InventoryMap1;	// CC0
	char InventoryCount1;	// CC4
	CItem * Inventory2;	// CC8
	LPBYTE InventoryMap2;	// CCC
	char InventoryCount2;	// CD0
	CItem * Trade; // CD4
	LPBYTE TradeMap;	// unkCD8
	int TradeMoney;	// CDC
	bool TradeOk;	// CE0
	CItem * pWarehouse; // CE4
	char WarehouseID;
	DWORD WarehouseTick;
	LPBYTE pWarehouseMap; // CE8
	char WarehouseCount;	// CEC
	short WarehousePW; // CEE
	BYTE WarehouseLock;	// CF0
	BYTE WarehouseUnfailLock;	// CF1
	int WarehouseMoney;	// CF4
	CItem * pChaosBox;	// CFC
	LPBYTE pChaosBoxMap;	// D00
	int ChaosMoney;	// D04
	int ChaosSuccessRate;	// D08
	BYTE ChaosMassMixCurrItem;
	BYTE ChaosMassMixSuccessCount;
	BOOL ChaosLock;	// D0C
	DWORD m_Option;	// D10
	int m_nEventScore;	// D14
	int m_nEventExp;	// D18
	int m_nEventMoney;	// D1C
	BYTE m_bDevilSquareIndex;	// D20
	bool m_bDevilSquareAuth;	// D21
	char m_cBloodCastleIndex;	// D22
	char m_cBloodCastleSubIndex;	// D23
	int m_iBloodCastleEXP;	// D24
	bool m_bBloodCastleComplete;	// D28
	char m_cChaosCastleIndex;	// D29
	char m_cChaosCastleSubIndex;	// D2A
	int m_iChaosCastleBlowTime;	// D2C
	bool m_bCCF_UI_Using;
	BYTE m_byCCF_CanEnter;
	int m_nCCF_CertiTick;
	int m_nCCF_UserIndex;
	int m_nCCF_BlowTime;
	BYTE m_byKillUserCount;
	BYTE m_byKillMobCount;
	bool m_bCCF_Quit_Msg;
	char m_cIllusionTempleIndex;
	int m_nZoneIndex;
	char m_cKillUserCount;	// D30
	char m_cKillMonsterCount;	// D31
	int m_iDuelUserReserved;	// D34
	int m_iDuelUserRequested;	// D38
	int m_iDuelUser;	// D3C
	int m_iDuelRoom;
	BYTE m_btDuelScore;	// D40
	int m_iDuelTickCount;	// D44
	bool IsInBattleGround;	// ECA
	bool HaveWeaponInHand;	// ECB
	short EventChipCount;	// ECC
	int LuckyCoinCount;
	int MutoNumber;	// ED0
	bool UseEventServer;	// ED4
	bool LoadWareHouseInfo;	// ED8
	int iStoneCount;	// EDC
	int m_MaxLifePower;	// F20
	int m_CheckLifeTime;	// F30
	unsigned char m_MoveOtherServer;	// F34
	char BackName[11];	// F35
	bool m_bPShopOpen;	// D48
	bool m_bPShopTransaction;	// D49	
	bool m_bPShopItemChange;	// D4A
	bool m_bPShopRedrawAbs;	// D4B
	char m_szPShopText[37];		// D4C
	bool m_bPShopWantDeal;	// D70
	int m_iPShopDealerIndex;	// D74
	char m_szPShopDealerName[10];	// D78
	CRITICAL_SECTION m_critPShopTrade;	// D84
	int m_iVpPShopPlayer[MAX_VIEWPORT];	// D9C
	WORD m_wVpPShopPlayerCount;	// EC8
	char m_BossGoldDerconMapNumber;	// F40
	char m_LastTeleportTime;	// F42
	BYTE m_ClientHackLogCount;	// F43
	bool m_bIsInMonsterHerd;	// F44
	bool m_bIsMonsterAttackFirst;	// F48
	class MonsterHerd * m_lpMonsterHerd;	// F4C
	int fSkillFrustrumX[MAX_ARRAY_FRUSTRUM];	// F84
	int fSkillFrustrumY[MAX_ARRAY_FRUSTRUM];	// F94
	TDurMagicKeyChecker * DurMagicKeyChecker;	// FD8
	bool bIsChaosMixCompleted;	// 110E
	bool SkillLongSpearChange;	// 110F
	int iObjectSecTimer;	// 1514
	bool m_bMapSvrMoveQuit;	// 1518
	bool m_bMapSvrMoveReq;	// 1519
	bool m_bMapSvrMoveReq_2;
	DWORD m_dwMapSvrQuitTick;	// 151C
	short m_sPrevMapSvrCode;	// 1520
	short m_sDestMapNumber;	// 1522
	BYTE m_btDestX;	// 1524
	BYTE m_btDestY;	// 1525
	union
	{
		struct
		{
				BYTE	m_btCsNpcExistVal1;
				BYTE	m_btCsNpcExistVal2;
				BYTE	m_btCsNpcExistVal3;
				BYTE	m_btCsNpcExistVal4;
		};
		int		m_iCsNpcExistVal;
	};

	BYTE	m_btCsNpcType;

	BYTE	m_btCsGateOpen;

	int		m_iCsGateLeverLinkIndex;

	BYTE	m_btCsNpcDfLevel;
	BYTE	m_btCsNpcRgLevel;

	BYTE	m_btCsJoinSide;
	bool	m_bCsGuildInvolved;
	bool m_bIsCastleNPCUpgradeCompleted;	// 1526
	BYTE m_btWeaponState;	// 1527
	int m_iWeaponUser;	// 1528
	BYTE m_btKillCount;	// 152C
	int m_iAccumulatedDamage;	// 1544
	BYTE	m_btLifeStoneCount;
	BYTE	m_btCreationState;
	int		m_iCreatedActivationTime;
	int m_iAccumulatedCrownAccessTime;	// 1550
	TMonsterSkillElementInfo m_MonsterSkillElementInfo;	// 1554
	int m_iBasicAI;	// 15A4
	int m_iCurrentAI;	// 15A8
	int m_iCurrentAIState;	// 15AC
	int m_iLastAIRunTime;	// 15B0
	int m_iGroupNumber;	// 15B4
	int m_iSubGroupNumber;	// 15B8
	int m_iGroupMemberGuid;	// 15BC
	int m_iRegenType;	// 15C0
	TMonsterAIAgro * m_Agro;	// 15C4
	int m_iLastAutomataRuntime;	// 18E8
	int m_iLastAutomataDelay;	// 18EC
	int m_iCrywolfMVPScore;	// 18F0
	DWORD m_dwLastCheckTick;	// 18F4
	int m_iAutoRecuperationTime;	// 18F8
	int m_iSkillDistanceErrorCount;	// 190C
	DWORD m_dwSkillDistanceErrorTick;	// 1910
	SKILL_INFO	m_SkillInfo;
	int BufferIndex;
	int BuffId;
	int BuffPlayerIndex;
	DWORD AgiCheckTime;
	BYTE WarehouseSaveLock;
	DWORD CrcCheckTime;
	bool m_bOff;
	bool m_bOffLevel;
	DWORD64 m_dwOffLevelTime;
	USER_DATA * m_PlayerData;
	_BOT_BUFF_LIST m_BotBuffs[10];
	UINT64 MonsterMoneyDrop;
	BYTE m_btOpenWarehouse;
	WORD m_wMineCount;
	BYTE m_btMiningLevel;
	WORD m_LastAttackNumber;
	DWORD m_LastAttackTick;
	WORD m_SkillCheckWrongCastCount;
	short m_RegenSysGroupNum;
	_tagMOVE_MAPSERVER_AUTHINFO m_MapServerAuthInfo;
	bool m_bBlind;
	int m_iPentagramMainAttribute;
	int m_iPentagramAttributePattern;
	int m_iPentagramDefense;
	int m_iPentagramAttackMin;
	int m_iPentagramAttackMax;
	int m_iPentagramAttackRating;
	int m_iPentagramDefenseRating;
	int m_iPentagramDamageMax;
	int m_iPentagramDamageMin;
	int m_iPentagramDamageOrigin;
	BYTE m_btNpcType;
	bool m_bGMSummon;
	DWORD m_Disappear_Monster;
	DWORD m_Disappear_NewYearMonster;
	int m_SummonCharDBNum;
	bool m_bIsHelpMon;
	int m_nITR_Index;
	int m_nITR_RelicsTick;
	bool m_bITR_GettingRelics;
	bool m_bITR_RegisteringRelics;
	short m_wITR_NpcType;
	BYTE m_byITR_StoneState;
	int m_nITLIndex;
	bool m_bITL_ScheduleInfoSend;
	bool m_bITL_GuildRankInfoSend;
	bool m_bITL_TournamentInfoSend;
	int m_nITLRelicsTick;
	bool m_bGettingRelics;
	bool m_bRegisteringRelics;
	WORD m_wITLNpcType;
	BYTE m_byStoneState;
	BYTE m_byEnterITLCount;
	bool m_bEnterCountLoad;
	BYTE m_byEnterITLUserCount;
	DWORD m_PostCMDLastUseTick;
	int m_nDSFIndex;
	int m_nRecallMonIndex[3];
	int m_nBossIndexLordSilvester;
	int m_nPhaseLordSilvester;	
	DWORD64 m_dwDCTimer;
	WORD m_wMuunItem;
	WORD m_wMuunSubItem;
	WORD m_wMuunRideItem;
	BYTE m_btInvenPetPos;
	WORD m_wInvenPet;
	CItem *pMuunInventory;
	CItem *pMuunInventory1;
	CItem *pMuunInventory2;
	CItem *pEventInventory;
	CItem *pEventInventory1;
	CItem *pEventInventory2;
	BYTE  *pEventInventoryMap;
	BYTE  *pEventInventoryMap1;
	BYTE  *pEventInventoryMap2;
	_tagMUUN_EFFECT_LIST m_MuunEffectList[MAX_MUUN_EFFECT_LIST];
	bool EventInventoryLoad;
	bool bMuunInventoryLoad;
	DWORD dwCheckMuunItemTime;
	BOOL bSubEquip;
	int m_iMuunItmePeriodDataIndex;
	WHISPER_STRUCT m_Whispering;
	BYTE m_btExpType;
};

typedef OBJECTSTRUCT * LPOBJ;

// sizeof ( BILL_CLASS ) == 0x18
class BILL_CLASS
{

private:

	char cCertifyType;	// 0
	BYTE PayCode;	// 1
	int EndTime;	// 4
	char EndsDays[13];	// 8

public:

	BILL_CLASS()	// line : 253
	{
		this->Init();
	}

	void Init()	// line : 219
	{
		this->cCertifyType=-1;
		this->PayCode=0;
		this->EndTime=0;
		this->EndsDays[0]=0;
	}	// line : 224

	BOOL SetBill(BYTE certify_type, BYTE pay, DWORD end_time, char* ends_days)	// line : 228
	{
		this->cCertifyType = certify_type;
		this->PayCode = pay;
		this->EndTime = end_time;
		strcpy(this->EndsDays, ends_days);
		return TRUE;
	}	// line : 237

	//int __thiscall IsEndTime();
	BYTE GetPayCode() {return this->PayCode;}	// line : 252
	char* GetEndsDays() {return &this->EndsDays[0];}	// line : 253
	int GetEndTime() { return this->EndTime;}	// line : 254
	int GetCertify() {return this->cCertifyType;}	// line : 255
};


#include "protocol.h"

//extern CViewportGuild ViewGuildMng;
extern OBJECTSTRUCT * gObj;
extern BILL_CLASS * m_ObjBill;

extern int gItemLoop;
extern int gItemLoop2;
extern int gItemLoopMax;
extern int gObjTotalUser;
extern int gObjTotalMonster;
extern int gDisconnect;
extern int GuildInfoOfs;
extern int GuildInfoCount;
extern int GuilUserOfs;
extern int GuildUserCount;
extern int lOfsChange;
extern int ChangeCount; // 8bf8b44

extern int gObjCallMonCount;
extern int gObjMonCount;
extern int gObjCount;



void gObjSkillUseProcTime500(LPOBJ lpObj);
void MonsterAndMsgProc();
void MoveMonsterProc();
void gObjSetState();
void gObjInit(); // constructor ?
void gObjEnd(); // destructor ?
void gObjClearViewportOfMine(LPOBJ lpObj);
void gObjClearViewport(LPOBJ lpObj);
void gObjCloseSet(int aIndex, int Flag);
void gObjCharTradeClear(LPOBJ lpObj);
void gObjCharZeroSet(int aIndex);
int gObjGetSocket(SOCKET socket);
int gObjGetHWIDUseCount(LPTSTR HWID);
void gObjSetTradeOption(int aIndex, int option);
bool IsDuelEnable(int aIndex);
bool IsOnDuel(int aIndex1, int aIndex2);
void gObjSetDuelOption(int aIndex, int option);
int GetMapMoveLevel(LPOBJ lpObj, int mapnumber, int max_over);
void DbItemSetInByte(LPOBJ lpObj, struct SDHP_DBCHAR_INFORESULT* lpMsg, bool* bAllItemExist);
void gObjSetBP(int aIndex);
bool gObjSetCharacter(unsigned char* lpdata, int aIndex);
int gObjCanItemTouch(LPOBJ lpObj, int type);
void gObjMagicTextSave(LPOBJ lpObj);
void ItemIsBufExOption(unsigned char* buf, class CItem* lpItem);
void gObjStatTextSave(LPOBJ lpObj);
void gObjItemTextSave(LPOBJ lpObj);
void gObjWarehouseTextSave(LPOBJ lpObj);
void gObjAuthorityCodeSet(LPOBJ lpObj);
int gObjSetPosMonster(int aIndex, int PosTableNum);
int gObjSetMonster(int aIndex, int MonsterClass);
int gObjGetSocket(SOCKET socket);
int GetMapMoveLevel(LPOBJ lpObj, int mapnumber, int max_over);
void ItemIsBufExOption(unsigned char* buf, class CItem* lpItem);
short gObjAddSearch(SOCKET aSocket, char* ip);
short gObjAdd(SOCKET aSocket, char* ip, int aIndex);
short gObjMonsterRecall(int iMapNumber);
short gObjAddMonster(int iMapNumber);
short gObjAddCallMon();
void gObjUserKill(int aIndex);
void gObjAllLogOut();
void gObjAllDisconnect();
void gObjTradeSave(LPOBJ lpObj, int index);
void gObjBillRequest(LPOBJ lpObj);
short gObjMemFree(int index);
int gObjGameClose(int aIndex);
short gObjDel(int index);
short gObjSometimeClose(int index);
int gObjIsGamePlaing(LPOBJ lpObj);
int gObjIsConnectedGP(int aIndex, char* CharName);
int gObjIsConnectedGP(int aIndex);
int gObjIsConnected(LPOBJ lpObj, int dbnumber);
int gObjIsConnected(int aIndex);
int gObjIsConnected(LPOBJ lpObj);
int gObjIsConnectedEx(int aIndex);
int gObjIsAccontConnect(int aIndex, char* accountid);
int gObjPasswordCheck(int aIndex, char* szInPassword);
int gObjCheckXYMapTile(LPOBJ lpObj, int iDbgName);
int gObjSetAccountLogin(int aIndex, char* szId, int aUserNumber, int aDBNumber, char* Password);
int gObjGetUserNumber(int aIndex);
int gObjGetNumber(int aIndex, int& UN, int& DBN);
LPSTR gObjGetAccountId(int aIndex);
int gObjGetIndex(char* szId);
int gObjUserIdConnectCheck(char* szId, int index);
unsigned char GetPathPacketDirPos(int px, int py);
int gObjCalDistance(LPOBJ lpObj1, LPOBJ lpObj2);
int gObjPositionCheck(LPOBJ lpObj);
int gObjCheckTileArea(int aIndex, int x, int y, int dis);
int ExtentCheck(int x, int y, int w, int h);
BOOL CheckOutOfInventory(int aIndex, int sy, int height);	//-> 1.01.00
BOOL InventoryExtentCheck(int x, int y, int w, int h);		//-> 1.01.00
BOOL CheckOutOfWarehouse(int aIndex, int sy, int height);	//-> 1.01.00
BOOL WarehouseExtentCheck(int x, int y, int w, int h);		//-> 1.01.00
void gObjSetInventory1Pointer(LPOBJ lpObj);
void gObjSetInventory2Pointer(LPOBJ lpObj);
void gObjAddMsgSend(LPOBJ lpObj, int aMsgCode, int aIndex, int SubCode);
void gObjAddMsgSendDelay(LPOBJ lpObj, int aMsgCode, int aIndex, int delay, int SubCode);
void gObjAddMsgSendDelayInSpecificQPos(LPOBJ lpObj, int aMsgCode, int aIndex, int delay, int SubCode, int iQPosition);
void gObjAddAttackProcMsgSendDelay(LPOBJ lpObj, int aMsgCode, int aIndex, int delay, int SubCode, int SubCode2);
void gObjMsgProc(LPOBJ lpObj);
void gObjStateProc(LPOBJ lpObj, int aMsgCode, int aIndex, int SubCode);
void gObjStateAttackProc(LPOBJ lpObj, int aMsgCode, int aIndex, int SubCode, int SubCode2);
int gObjBackSpring(LPOBJ lpObj, LPOBJ lpTargetObj);
int BackSpringCheck(int& x, int& y, int& dir, unsigned char map);
int gObjBackSpring2(LPOBJ lpObj, LPOBJ lpTargetObj, int count);
bool gObjLevelUp(LPOBJ lpObj, UINT64 addexp, int iMonsterType, const char * szEventType);
int gObjLevelUpPointAdd(unsigned char type, LPOBJ lpObj);
void gObjGiveItemSearch(LPOBJ lpObj, int maxlevel);
void gObjGiveItemWarehouseSearch(LPOBJ lpObj, int maxlevel);
int gObjGuildMasterCapacityTest(LPOBJ lpObj);
void gObjNextExpCal(LPOBJ lpObj);
int retResistance(LPOBJ lpObj, int Resistance_Type);
int retCalcSkillResistance(LPOBJ lpTargetObj, int isDouble);
int gObjAttackQ(LPOBJ lpObj);
void gObjPlayerKiller(LPOBJ lpObj, LPOBJ lpTargetObj, int MSBDamage);
void gObjUserDie(LPOBJ lpObj, LPOBJ lpTargetObj);
void gObjInterfaceTimeCheck(LPOBJ lpObj);
void gObjPkDownTimeCheck(LPOBJ lpObj, int TargetLevel);
int gObjAngelSprite(LPOBJ lpObj);
int gObjDemonSprite(LPOBJ lpObj);
int gObjSatanSprite(LPOBJ lpObj);
int gObjSafeGuardSprite(LPOBJ lpObj);
int gObjFindInventoryEquipment(int aIndex);
int gObjSetInventoryEquipment(LPOBJ lpObj, unsigned char btPos);
int gObjInventoryEquipment(LPOBJ lpObj);
int gObjWingSprite(LPOBJ lpObj);
int gObjUniriaSprite(LPOBJ lpObj);
void gObjSpriteDamage(LPOBJ lpObj, int damage);
void gObjSecondDurDown(LPOBJ lpObj);
void gObjChangeDurProc(LPOBJ lpObj);
void gObjWingDurProc(LPOBJ lpObj);
void gObjPenaltyDurDown(LPOBJ lpObj, LPOBJ lpTargetObj);
void gObjWeaponDurDown(LPOBJ lpObj, LPOBJ lpTargetObj, int type);
void gObjArmorRandomDurDown(LPOBJ lpObj, LPOBJ lpAttackObj);
bool gObjIsSelfDefense(LPOBJ lpObj, int aTargetIndex);
void gObjCheckSelfDefense(LPOBJ lpObj, int aTargetIndex);
void gObjTimeCheckSelfDefense(LPOBJ lpObj);
int gObjAttack(LPOBJ lpObj, LPOBJ lpTargetObj, class CMagicInf* lpMagic, int magicsend, unsigned char MSBFlag, int AttackDamage, BOOL bCombo, BYTE RFAttack, BYTE byReflect, BYTE byPentagramAttack = TRUE);
void gObjMonsterExpDivision(LPOBJ lpMonObj, LPOBJ lpObj, int AttackDamage, int MSBFlag);
void gObjMonsterExpDivisionRenewal(LPOBJ lpMonObj, LPOBJ lpObj, int AttackDamage, int MSBFlag);
UINT64 gObjMonsterExpSingle(LPOBJ lpObj, LPOBJ lpTargetObj, int dmg, int tot_dmg, bool& bSendExp, UINT64& nDropZen);
UINT64 gObjMonsterExpSingleRenewal(LPOBJ lpObj, LPOBJ lpTargetObj, int dmg, int tot_dmg, bool &bSendExp);
void gObjExpParty(LPOBJ lpObj, LPOBJ lpTargetObj, int AttackDamage, int MSBFlag);
UINT64 gObjExpPartyRenewal(int nPartyNumber, int nLastAttackUserIndex, LPOBJ lpTargetObj, int AttackDamage, int nTotalDamage, int MSBFlag);
void gObjMonsterDieLifePlus(LPOBJ lpObj, LPOBJ lpTartObj);
void gObjLifeCheck(LPOBJ lpTargetObj, LPOBJ lpObj, int AttackDamage, int DamageSendType, int MSBFlag, int MSBDamage, unsigned short Skill, int iShieldDamage, int iElementalDamage);
int gObjInventoryTrans(int aIndex);
int gObjInventoryCommit(int aIndex);
int gObjInventoryRollback(int aIndex);
void gObjInventoryItemSet(int aIndex, int itempos, unsigned char set_byte);
void gObjInventoryItemBoxSet(int aIndex, int itempos, int xl, int yl, unsigned char set_byte);
bool gObjFixInventoryPointer(int aIndex);
BYTE gObjInventoryDeleteItem(int aIndex, int itempos);
BYTE gObjWarehouseDeleteItem(int aIndex, int itempos);
BYTE gObjChaosBoxDeleteItem(int aIndex, int itempos);
unsigned char gObjInventoryInsertItem(LPOBJ lpObj, int type, int index, int level, int iSerial, int iDur);
DWORD gGetItemNumber();
void gPlusItemNumber();
unsigned char gObjInventoryInsertItem(LPOBJ lpObj, int type, int index, int level, int iSerial, int iDur);
unsigned char gObjOnlyInventoryInsertItem(int aIndex, class CItem item);
unsigned char gObjInventoryInsertItem(LPOBJ lpObj, int type, int index, int level);
int gObjSearchItem(LPOBJ lpObj, int item_type, int add_dur, int nLevel);
int gObjSearchItemMinus(LPOBJ lpObj, int pos, int m_dur);
unsigned char gObjShopBuyInventoryInsertItem(int aIndex, class CItem item);
unsigned char gObjShopBuyMuunInventoryInsertItem(int aIndex, class CItem item);
unsigned char gObjShopBuyEventInventoryInsertItem(int aIndex, class CItem item);
unsigned char gObjInventoryInsertItemTemp(LPOBJ lpObj, class CMapItem* Item);
unsigned char gObjInventoryInsertItem(int aIndex, class CMapItem* item);
unsigned char gObjInventoryInsertItem(int aIndex, class CItem item);
unsigned char gObjMonsterInventoryInsertItem(LPOBJ lpObj, int type, int index, int level, int op1, int op2, int op3);
unsigned char gObjInventoryRectCheck(int aIndex, int sx, int sy, int width, int height);
unsigned char gObjOnlyInventoryRectCheck(int aIndex, int sx, int sy, int width, int height);
int CheckInventoryEmptySpace(LPOBJ lpObj, int iItemHeight, int iItemWidth);
int gObjIsItemPut(LPOBJ lpObj, class CItem* lpItem, int pos);
unsigned char gObjWerehouseRectCheck(int aIndex, int sx, int sy, int width, int height);
unsigned char gObjMapRectCheck(unsigned char* lpMapBuf, int sx, int sy, int ex, int ey, int width, int height);
void gObjWarehouseItemBoxSet(int aIndex, int itempos, int xl, int yl, unsigned char set_byte);
void gObjWarehouseItemSet(int aIndex, int itempos, unsigned char set_byte);
void gObjChaosItemBoxSet(int aIndex, int itempos, int xl, int yl, unsigned char set_byte);
void gObjChaosItemSet(int aIndex, int itempos, unsigned char set_byte);
unsigned char gObjChaosBoxInsertItemPos(int aIndex, class CItem item, int pos, int source);
unsigned char gObjWarehouseInsertItemPos(int aIndex, class CItem item, int pos, int source);
unsigned char gObjInventoryInsertItemPos(int aIndex, class CItem item, int pos, int RequestCheck);
int gObjInventorySearchSerialNumber(LPOBJ lpObj, UINT64 serial);
int gObjWarehouseSearchSerialNumber(LPOBJ lpObj, UINT64 sirial);
unsigned char gObjInventoryMoveItem(int aIndex, unsigned char source, unsigned char target, int& durSsend, int& durTsend, unsigned char sFlag, unsigned char tFlag, unsigned char* siteminfo);
unsigned char gObjTradeRectCheck(int aIndex, int sx, int sy, int width, int height);
int gObjTradeItemBoxSet(int aIndex, int itempos, int xl, int yl, unsigned char set_byte);
unsigned char gObjTradeInventoryMove(LPOBJ lpObj, unsigned char source, unsigned char target);
unsigned char gObjInventoryTradeMove(LPOBJ lpObj, unsigned char source, unsigned char target);
unsigned char gObjTradeTradeMove(LPOBJ lpObj, unsigned char source, unsigned char target);
void gObjTradeCancel(int aIndex);
void gObjTempInventoryItemBoxSet(unsigned char* TempMap, int itempos, int xl, int yl, unsigned char set_byte);
unsigned char gObjTempInventoryRectCheck(int aIndex, unsigned char* TempMap, int sx, int sy, int width, int height);
unsigned char gObjTempInventoryInsertItem(LPOBJ lpObj, class CItem item, unsigned char* TempMap);
int TradeItemInventoryPutTest(int aIndex);
int TradeitemInventoryPut(int aIndex);
void gObjTradeOkButton(int aIndex);
unsigned char LevelSmallConvert(int level);
unsigned char LevelSmallConvert(int aIndex, int inventoryindex);
void gObjAttackDamageCalc(int aIndex);
void gObjMakePreviewCharSet(int aIndex);
void gObjViewportPaint(HWND hWnd);
void InitFrustrum();
void InitFrustrumEx(); // 1.01.10B GS KOR, changed values
void CreateFrustrum(int x, int y, int aIndex);
bool TestFrustrum2(int x, int y, int aIndex);
int gObjCheckViewport(int aIndex, int x, int y);
void gObjViewportClose(LPOBJ lpObj);
void gObjViewportListCreate(short aIndex);
int ViewportAdd(int aIndex, int aAddIndex, int aType);
int ViewportAdd2(int aIndex, int aAddIndex, int aType);
void ViewportDel(short aIndex, int aDelIndex);
void Viewport2Del(short aIndex, int aDelIndex);
void gObjViewportAllDel(short aIndex);
void gObjViewportListDestroy(short aIndex);;
void gObjStateSetCreate(int aIndex);
void gObjSecondProc();
void gObjManaPotionFill(LPOBJ lpObj);
void gObjRestPotionFill(LPOBJ lpObj);
void gObjUseDrink(LPOBJ lpObj, int level);
void gObjViewportListProtocolDestroy(LPOBJ lpObj);
void gObjViewportListProtocolCreate(LPOBJ lpObj);
void gObjViewportListProtocol(short aIndex);
void gObjTeleportMagicUse(int aIndex, unsigned char x, unsigned char y);
int gObjMoveGate(int aIndex, int gt);
void gObjTeleport(int aIndex, int map, int x, int y);
void gObjMoveDataLoadingOK(int aIndex);
class CMagicInf* gObjGetMagic(LPOBJ lpObj, int mIndex);
class CMagicInf* gObjGetMagicSearch(LPOBJ lpObj, unsigned short skillnumber);
int gObjMagicManaUse(LPOBJ lpObj, class CMagicInf* lpMagic);
int gObjMagicBPUse(LPOBJ lpObj, class CMagicInf* lpMagic);
void gObjMagicAddEnergyCheckOnOff(int flag);
int gObjMagicAdd(LPOBJ lpObj, unsigned short aSkill, unsigned char Level);
int gObjMagicDel(LPOBJ lpObj, unsigned short aSkill, unsigned char Level);
int gObjMagicAdd(LPOBJ lpObj, unsigned short Type, unsigned short Index, unsigned char Level, unsigned short& SkillNumber);
int gObjWeaponMagicAdd(LPOBJ lpObj, unsigned short aSkill, BYTE Level);
int gObjMonsterMagicAdd(LPOBJ lpObj, unsigned short aSkill, BYTE Level);
int gObjMagicEnergyCheck(LPOBJ lpObj, unsigned short aSkill);
int gObjSpecialItemLevelUp(LPOBJ lpObj, int source, int target);
int gObjSpecialItemRepair(LPOBJ lpObj, int source, int target);
int gObjItemLevelUp(LPOBJ lpObj, int source, int target);
int gObjItemRandomLevelUp(LPOBJ lpObj, int source, int target);
int gObjItemRandomOption3Up(LPOBJ lpObj, int source, int target);
void gObjAbilityReSet(LPOBJ lpObj);
int gObjTargetGuildWarCheck(LPOBJ lpObj, LPOBJ lpTargetObj);
void gObjGuildWarEndSend(struct _GUILD_INFO_STRUCT* lpGuild1, struct _GUILD_INFO_STRUCT* lpGuild2, unsigned char Result1, unsigned char Result2);
void gObjGuildWarEndSend(LPOBJ lpObj, unsigned char Result1, unsigned char Result2);
void gObjGuildWarEnd(struct _GUILD_INFO_STRUCT* lpGuild, struct _GUILD_INFO_STRUCT* lpTargetGuild);
int gObjGuildWarProc(struct _GUILD_INFO_STRUCT* lpGuild1, struct _GUILD_INFO_STRUCT* lpGuild2, int score);
int gObjGuildWarCheck(LPOBJ lpObj, LPOBJ lpTargetObj);
int gObjGuildWarMasterClose(LPOBJ lpObj);
int gObjGuildWarItemGive(struct _GUILD_INFO_STRUCT* lpWinGuild, struct _GUILD_INFO_STRUCT* lpLoseGuild);
int gObjGetPkTime(LPOBJ lpObj, int& hour, int& min);
int gObjMonsterCall(int aIndex, int MonsterType, int x, int y);
void gObjMonsterCallKill(int aIndex);
bool gObjCheckMoveArea(int aIndex, int X, int Y);
int gObjCheckTeleportArea(int aIndex, unsigned char x, unsigned char y);
int gObjCheckAttackAreaUsedPath(int aIndex, int TarObjIndex);
int gObjCheckattackAreaUsedViewPort(int aIndex, int TarObjIndex);
int gObjCheckAttackArea(int aIndex, int TarObjIndex);
int gUserFindDevilSquareInvitation(int aIndex);
int gUserFindDevilSquareKeyEyes(int aIndex);
void gObjSendUserStatistic(int aIndex, int startLevel, int endLevel);
LPOBJ gObjFind(char* targetcharname);
int gObjFind10EventChip(int aIndex);
int gObjDelete10EventChip(int aIndex);
void gObjSetPosition(int aIndex, int x, int y);
int gObjGetItemCountInEquipment(int aIndex, int itemtype, int itemindex, int itemlevel);
int gObjGetItemCountInIventory(int aIndex, int itemnum);
int gObjGetItemCountInIventory(int aIndex, int itemtype, int itemindex, int itemlevel);
int gObjGetManaItemPos(int aIndex);
void gObjDelteItemCountInInventory(int aIndex, int itemtype, int itemindex, int count);
void gObjGetStatPointState(int aIndex, short& AddPoint, short& MaxAddPoint, short& MinusPoint, short& MaxMinusPoint);
int gObjCheckStatPointUp(int aIndex);
int gObjCheckStatPointDown(int aIndex);
void gObjUseCircle(int aIndex, int pos);
void gObjUsePlusStatFruit(int aIndex, int pos);
void gObjUseMinusStatFruit(int aIndex, int pos);
void CashShopExMinusStatFruit(int aIndex, int pos);
void gObjCalcMaxLifePower(int aIndex);
void gObjDelayLifeCheck(int aIndex);
int gObjDuelCheck(LPOBJ lpObj);
int gObjDuelCheck(LPOBJ lpObj, LPOBJ lpTargetObj);
void gObjResetDuel(LPOBJ lpObj);
void SkillFrustrum(unsigned char bangle, int aIndex);
int SkillTestFrustrum(int x, int y, int aIndex);
int gObjCheckMaxZen(int aIndex, int nAddZen);
void MakeRandomSetItem(int aIndex);
void MakeRandomSetItem(int aIndex, _stGremoryCaseItem & stItem);
void MakeRewardSetItem(int aIndex, BYTE cDropX, BYTE cDropY, int iRewardType, int iMapnumber);
void MakeRewardSetItem(int aIndex, BYTE cDropX, BYTE cDropY, int iRewardType, int iMapnumber, _stGremoryCaseItem & stItem);
void gObjRecall(int aIndex, int mapnumber, int x, int y);
void gObjSetExpPetItem(int aIndex, UINT64 exp);
int gObjGetRandomItemDropLocation(int iMapNumber, BYTE& cX, BYTE& cY, int iRangeX, int iRangeY, int iLoopCount);
int gObjGetRandomFreeLocation(int iMapNumber, BYTE& cX, BYTE& cY, int iRangeX, int iRangeY, int iLoopCount);
int gObjCheckAttackTypeMagic(int iClass, int iSkill);
int gObjGetGuildUnionNumber(LPOBJ lpObj);
void gObjGetGuildUnionName(LPOBJ lpObj, char* szUnionName, int iUnionNameLen);
int gObjCheckRival(LPOBJ lpObj, LPOBJ lpTargetObj);
int gObjGetRelationShip(LPOBJ lpObj, LPOBJ lpTargetObj);
void gObjNotifyUpdateUnionV1(LPOBJ lpObj);
void gObjNotifyUpdateUnionV2(LPOBJ lpObj);
void gObjUnionUpdateProc(int aIndex);
void gObjSetKillCount(int aIndex, int iOption);
void gObjNotifyUseWeaponV1(LPOBJ lpOwnerObj, LPOBJ lpWeaponObj, int iTargetX, int iTargetY);
void gObjNotifyUseWeaponDamage(LPOBJ lpWeaponObj, int iTargetX, int iTargetY);
void gObjUseBlessAndSoulPotion(int aIndex, int iItemLevel);
void gObjWeaponDurDownInCastle(LPOBJ lpObj, LPOBJ lpTargetObj, int iDecValue);
void gObjReady4Relife(LPOBJ lpObj);
void gObjCheckTimeOutValue(LPOBJ lpObj, DWORD& rNowTick);
void MsgOutput(int aIndex, char* msg, ...);
void gProcessAutoRecuperation(LPOBJ lpObj);
void gObjShieldAutoRefill(LPOBJ lpObj);
int gObjCheckOverlapItemUsingDur(int iUserIndex, int iMaxOverlapped, int iItemType, int iItemLevel);
int gObjOverlapItemUsingDur(class CItem* lpItem, int iMapNumber, int iItemNumber, int iUserIndex, int iMaxOverlapped, int iItemType, int iItemLevel);
int gObjCheckSerial0ItemList(class CItem* lpItem);
int gObjCheckInventorySerial0Item(LPOBJ lpObj);
BOOL gCheckSkillDistance(int aIndex, int aTargetIndex, int iSkillNum);
void gObjSaveChaosBoxItemList(LPOBJ lpObj);
void gObjQuestMonsterManagement(LPOBJ lpObj, LPOBJ lpTargetObj);
void gObjMuBotPayForUse(LPOBJ lpObj);
BYTE gObjOnlyInventory1RectCheck(int aIndex, int sx, int sy, int width, int height);
BYTE gObjOnlyInventory2RectCheck(int aIndex, int sx, int sy, int width, int height);
void gObjReqMapSvrAuth(LPOBJ lpObj);
BOOL gObjGetRandomFreeArea(int iMapNumber, BYTE &cX, BYTE &cY, int iSX, int iSY, int iDX, int iDY, int iLoopCount);
void gObjDeleteBokBlessEtc(LPOBJ lpObj);
BOOL gObjItemLevelDown(LPOBJ lpObj, int source, int target);
BOOL gObjItemExcessAdd(LPOBJ lpObj, int source, int target);
BOOL gObjItemDoubleUp(LPOBJ lpObj, int source, int target);
BOOL gObjItemRandomDoubleLevelUp(LPOBJ lpObj, int source, int target);
BOOL gObjItemKondarAdd(LPOBJ lpObj, int source, int target);
BOOL gObjItemRandomOptionNew3Up(LPOBJ lpObj, int source, int target);
BOOL gObjItemKundumAdd(LPOBJ lpObj, int source, int target);
BOOL gObjItemBalanceAdd(LPOBJ lpObj, int source, int target);
BOOL gObjItemScienceAdd(LPOBJ lpObj, int source, int target);
BOOL gObjItemLuckAdd(LPOBJ lpObj, int source, int target);
BOOL gObjItemSpeedAdd(LPOBJ lpObj, int source, int target);
int gObjCalcHPPercent(double Life, double MaxLife);
bool CheckAuthorityCondition(int AuthorityCode, LPOBJ lpObj);
void GetBundleCount(int count,int div,int& itemcount,int& left);
int gObjGetJewelCountInInventory(int aIndex, BYTE type);
int gGetPartyMaxLevel(int nPartyNumber);
int gGetLowHPMonster(int nZoneIndex, int nIndex, int nDistance);
int gObjGamblingInventoryCheck(int aIndex, int iWidth, int iHeight);
void gObjAddSpecificSkillsForSiege(LPOBJ lpObj);
void gObjPentagramMixBoxSet(int aIndex, int itempos, int xl, int yl, unsigned char set_byte);
void gObjPentagramMixItemSet(int aIndex, int itempos, unsigned char set_byte);
unsigned char gObjPentagramMixBoxInsertItemPos(int aIndex, CItem item, int pos, int source);
unsigned char gObjPentagramMixBoxInsertItem(int aIndex, CItem item);
unsigned char gObjPentagramMixBoxRectCheck(int aIndex, int sx, int sy, int width, int height);
unsigned char gObjPentagramMixBoxDeleteItem(int aIndex, int itempos);
int gTransPentagramJewelInfoTest(int aIndex, int targetIndex, CItem sourceItem);
int gTransPentagramJewelInfo(int aIndex, BYTE madeItemPos, int targetIndex, CItem sourceItem, int *nJewelCount);
void gObjAutoPartySet(LPOBJ lpObj /* master */, LPOBJ lpTargetObj /*requestor*/);
BYTE gObjMuunInventoryInsertItemPos(int aIndex, CItem item, int pos);
bool gObjFixMuunInventoryPointer(int aIndex);
void gObjSetMuunInventory1Pointer(OBJECTSTRUCT *lpObj);
void gObjSetMuunInventory2Pointer(OBJECTSTRUCT *lpObj);
BYTE gObjMuunInvenMove(OBJECTSTRUCT *lpObj, int *durSsend, int *durTsend, BYTE source, BYTE target, BYTE *siteminfo);
BYTE gObjChkMuunInventoryEmpty(OBJECTSTRUCT *lpObj);
BYTE gObjMuunInventoryInsertItem(int aIndex, CItem item);
BYTE gObjMuunInventoryInsertItem(int aIndex, CMapItem *item);
BYTE gObjMuunInventoryDeleteItem(int aIndex, int itempos);
void gObjEventInventoryItemSet(int aIndex, int itempos, BYTE set_byte);
void gObjEventInventoryItemBoxSet(int aIndex, int itempos, int xl, int yl, BYTE set_byte);
bool gObjFixEventInventoryPointer(int aIndex);
bool gObjEventInventoryDeleteItem(int aIndex, int itempos);
BYTE gObjEventInventoryInsertItemTemp(LPOBJ lpObj, CMapItem * Item);
BYTE gObjEventInventoryInsertItem(int aIndex, CMapItem * item);
BYTE gObjEventInventoryInsertItem(int aIndex, CItem item);
BYTE gObjEventInvenItemOverlap(LPOBJ lpObj, int *durSsend, int *durTsend, BYTE source, BYTE target);
BYTE gObjEventInvenMove(LPOBJ lpObj, int *durSsend, int *durTsend, BYTE source, BYTE target);
BYTE gObjEventInventoryTradeMove(LPOBJ lpObj, BYTE source, BYTE target);
BYTE gObjTradeEventInventoryMove(LPOBJ lpObj, BYTE source, BYTE target);
void gObjTempEventInventoryItemBoxSet(BYTE * TempMap, int itempos, int xl, int yl, BYTE set_byte);
BYTE gObjTempEventInventoryRectCheck(int aIndex, BYTE * TempMap, int sx, int sy, int width, int height);
BYTE gObjTempEventInventoryInsertItem(LPOBJ lpObj, CItem item, BYTE * TempMap);
int CheckEventInventoryEmptySpace(LPOBJ lpObj, int iItemHeight, int iItemWidth);
int IsEventItem(CItem item);
int IsEventItem(int iType);
BYTE gObjEventInventoryRectCheck(int aIndex, int sx, int sy, int width, int height);
BYTE gObjEventInventoryInsertItemPos(int aIndex, CItem item, int pos, BOOL RequestCheck);
int gObjEventInventorySearchSerialNum(LPOBJ lpObj, UINT64 serial);
void gObjSetEventInventory1Pointer(LPOBJ lpObj);
void gObjSetEventInventory2Pointer(LPOBJ lpObj);
WING_TYPE GetWingType(WORD type, WING_CHECK_FLAG eFlag);
int UseBundleOfBlessJewel(LPOBJ lpObj, int source, int target);
int IsExceptionJewelOfBlessInchant(LPOBJ lpObj, int source, int target);
int IsNotApplyJewelToItems(WORD sItemType);
int IsTransformCharacterSkin(int pChange);
int gCountBlessSoulChaosJewel(int aIndex, short & sBlessJewelCount, short & s10BlessJewelCount, short & s20BlessJewelCount, short & s30BlessJewelCount, short & sSoulJewelCount, short & s10SoulJewelCount, short & s20SoulJewelCount, short & s30SoulJewelCount, short & sChaosJewelCount, short & s10ChaosJewelCount, short & s20ChaosJewelCount, short & s30ChaosJewelCount);
int gJewelBuyItemValueTest(int aBuyerIndex, int aSellerIndex, short sBlessJewelPrice, short sSoulJewelPrice, short sChaosJewelPrice, short sBlessJewelCount, short s10BlessJewelCount, short s20BlessJewelCount, short s30BlessJewelCount, short sSoulJewelCount, short s10SoulJewelCount, short s20SoulJewelCount, short s30SoulJewelCount, short sChaosJewelCount, short s10ChaosJewelCount, short s20ChaosJewelCount, short s30ChaosJewelCount, short &sBlessJewelNeedCount, short &s10BlessJewelNeedCount, short &s20BlessJewelNeedCount, short &s30BlessJewelNeedCount, short &sSoulJewelNeedCount, short &s10SoulJewelNeedCount, short &s20SoulJewelNeedCount, short &s30SoulJewelNeedCount, short &sChaosJewelNeedCount, short &s10ChaosJewelNeedCount, short &s20ChaosJewelNeedCount, short &s30ChaosJewelNeedCount);
int gJewelInventoryPutTest(int aBuyerIndex, int aSellerIndex, short sBlessJewelPrice, short sSoulJewelPrice, short sChaosJewelPrice);
int gJewelInventoryPut(int aBuyerIndex, int aSellerIndex, short sBlessJewelPrice, short sSoulJewelPrice, short sChaosJewelPrice, short sBlessJewelNeedCount, short s10BlessJewelNeedCount, short s20BlessJewelNeedCount, short s30BlessJewelNeedCount, short sSoulJewelNeedCount, short s10SoulJewelNeedCount, short s20SoulJewelNeedCount, short s30SoulJewelNeedCount, short sChaosJewelNeedCount, short s10ChaosJewelNeedCount, short s20ChaosJewelNeedCount, short s30ChaosJewelNeedCount);
int CheckInventoryEmptySpaceCount(LPOBJ lpObj, int iItemHeight, int iItemWidth);
BOOL SummonGoldColossusMonster(LPOBJ lpObj, int monsterIndex, int nCount, int bIsHelpMon);
BOOL SummonPetEventMonster(LPOBJ lpObj);
BOOL NewYearSummonMonster(LPOBJ lpObj, int monsterIndex);
int gObjGetAutoPartyUserCount();
int gObjGetOffTradeUsercount();
void gObjDisconnectOffTraders();
void gObjDisconnectOffLevelers();
void gObjInvenPetDamage(LPOBJ lpObj, int damage);
BOOL gObjUseInvenPetCheck(LPOBJ lpObj, CItem *lpItem, int flag);
BOOL IsInvenPet(WORD sIndex);
BOOL IsInvenItem(WORD sIndex);
int gObjCalDistanceTX(LPOBJ lpObj1, LPOBJ lpObj2);
void Check_SameSerialItem(int aIndex, BYTE bCase, bool & bCheckFail);
bool gObjChaosBoxPutItemTest(int aIndex, CItem Item, BYTE btCount);
BYTE gObjChaosBoxInsertItemTemp(LPOBJ lpObj, CItem * Item);
#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

