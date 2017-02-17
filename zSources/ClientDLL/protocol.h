//////////////////////////////////////////////////////////////////////
// protocol.h
#ifndef PROTOCOLCORE_H
#define PROTOCOLCORE_H

#define SET_NUMBERH(x) ( (BYTE)((DWORD)(x)>>(DWORD)8) )
#define SET_NUMBERL(x) ( (BYTE)((DWORD)(x) & 0xFF) )

struct PBMSG_HEAD2	// Packet - Byte Type
{

public:

	void set( LPBYTE lpBuf, BYTE head, BYTE sub, BYTE size)
	{
		lpBuf[0] = 0xC1;
		lpBuf[1] = size;
		lpBuf[2] = head;
		lpBuf[3] = sub;
	};

	BYTE c;
	BYTE size;
	BYTE headcode;
	BYTE subcode;
};

struct PWMSG_HEAD2
{

public:

	void set(LPBYTE lpBuf, BYTE head, BYTE sub, int size)
	{
		lpBuf[0] = 0xC2;
		lpBuf[1] = SET_NUMBERH(size);
		lpBuf[2] = SET_NUMBERL(size);
		lpBuf[3] = head;
		lpBuf[4] = sub;
	}

	BYTE c;
	BYTE sizeH;
	BYTE sizeL;
	BYTE headcode;
	BYTE subcode;
};

struct PWMSG_HEAD	// Packet - Word Type
{
public:

	void set(LPBYTE lpBuf, BYTE head, int size)
	{
		lpBuf[0] = 0xC2;
		lpBuf[1] = SET_NUMBERH(size);
		lpBuf[2] = SET_NUMBERL(size);
		lpBuf[3] = head;
	};

	void setE(LPBYTE lpBuf, BYTE head, int size)	// line : 49
	{
		lpBuf[0] = 0xC4;
		lpBuf[1] = SET_NUMBERH(size);
		lpBuf[2] = SET_NUMBERL(size);
		lpBuf[3] = head;
	};

	BYTE c;
	BYTE sizeH;
	BYTE sizeL;
	BYTE headcode;
};

struct PBMSG_HEAD	// Packet - Byte Type
{
public:
	void set(LPBYTE lpBuf, BYTE head, BYTE size)	// line : 18
	{
		lpBuf[0] = 0xC1;
		lpBuf[1] = size;
		lpBuf[2] = head;
	};	// line : 22

	void setE(LPBYTE lpBuf, BYTE head, BYTE size)	// line : 25
	{
		lpBuf[0] = 0xC3;
		lpBuf[1] = size;
		lpBuf[2] = head;
	};	// line : 29

	BYTE c;
	BYTE size;
	BYTE headcode;
};
struct PMSG_DEFAULT2
{
	PBMSG_HEAD h;
	BYTE subcode;
};

struct PMSG_SEND_CRC{
	PBMSG_HEAD h;
	BYTE m_SubCode;
	DWORD m_Crc[5];
};

struct PMSG_ATTACKRESULT
{
	PBMSG_HEAD h;	// C1:DC
	BYTE NumberH;	// 3
	BYTE NumberL;	// 4
	BYTE DamageH;	// 5
	BYTE DamageL;	// 6
	BYTE Unk;
	BYTE DamageType;	// 7
	BYTE btShieldDamageH;	// 8
	BYTE btShieldDamageL;	// 9
	int IGCDamage;
};

struct PMSG_DIEPLAYER
{
	PBMSG_HEAD h;	// C1:17
	BYTE NumberH;	// 3
	BYTE NumberL;	// 4
	BYTE Skill;	// 5
	BYTE KillerNumberH;	// 6
	BYTE KillerNumberL;	// 7
};

struct PMSG_REFILL
{
	PBMSG_HEAD h;	// C1:26
	BYTE IPos;	// 3
	BYTE LifeH;	// 4
	BYTE LifeL;	// 5
	BYTE Flag;	// 6
	BYTE btShieldH;	// 7
	BYTE btShieldL;	// 8
	int IGCLife;
};

struct PMSG_MANASEND
{
	PBMSG_HEAD h;	// C1:27
	BYTE IPos;	// 3
	BYTE ManaH;	// 4
	BYTE ManaL;	// 5
	BYTE BPH;	// 6
	BYTE BPL;	// 7
	int IGCMana;
};

#pragma pack (1)
struct PMSG_JOINRESULT
{
	PBMSG_HEAD h;	// C1:F1
	BYTE scode;	// 3
	BYTE result;	// 4
	BYTE NumberH;	// 5
	BYTE NumberL;	// 6
	BYTE CliVersion[5];	// 7
	char EncData[8];
	char DecData[8];
};
#pragma pack ()

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
	int Ruud; // Season X, not used in S9
	int IGCLife;
	int IGCMaxLife;
	int IGCMana;
	int IGCMaxMana;
};
#pragma pack (1)
struct PMSG_CHARREGEN
{
	struct PBMSG_HEAD h;
	BYTE subcode;
	BYTE MapX;
	BYTE MapY;
	BYTE MapNumber;
	BYTE Dir;
	WORD Life;
	WORD Mana;
	WORD wShield;
	WORD BP;

	BYTE ExpHHH; //
	BYTE ExpHHL; //
	BYTE ExpHLH; //
	BYTE ExpHLL; //
	BYTE ExpLHH; //
	BYTE ExpLHL; //
	BYTE ExpLLH; //
	BYTE ExpLLL; //

	DWORD Money;
	int IGCLife;
	int IGCMana;
};
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

struct PMSG_IDPASS_OLD
{
	PBMSG_HEAD h;
	BYTE subcode;	// 3
	char Id[10];	// 4
	char Pass[20];	// E
	DWORD TickCount;	// 18
	BYTE CliVersion[5];	// 1C
	BYTE CliSerial[16];	// 21
};

struct PMSG_IDPASS_NEW
{
	PBMSG_HEAD h;
	BYTE subcode;	// 3
	char Id[10];	// 4
	char Pass[20];	// E
	char HWID[100];
	DWORD TickCount;	// 18
	BYTE CliVersion[5];	// 1C
	BYTE CliSerial[16];	// 21
	DWORD ServerSeason;
};

struct PMSG_CLIENTTIME_NEW
{
	PBMSG_HEAD h;
	WORD TimeH;
	WORD TimeL;
	WORD AttackSpeed;	// 8
	WORD Agility;
	WORD MagicSpeed;	// A
	char Version[10];
};
#pragma pack ()

struct PMSG_LEVELUP
{
	PBMSG_HEAD h;	// C1:F3:05
	BYTE subcode;	// 3
	WORD Level;	// 4
	WORD LevelUpPoint;	// 6
	WORD MaxLife;	// 8
	WORD MaxMana;	// A
	WORD wMaxShield;	// C
	WORD MaxBP;	// E
	short AddPoint;	// 10
	short MaxAddPoint;	// 12
	short MinusPoint;	// 14
	short MaxMinusPoint;	// 16
	int IGCMaxLife;
	int IGCMaxMana;
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
#pragma pack (1)
struct PMSG_MASTER_LEVEL_UP_SEND
{
	PBMSG_HEAD h;
	BYTE subcode;
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
#pragma pack ()

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
struct PMSG_ALTERPSHOPVAULT
{
	PBMSG_HEAD h;
	BYTE subcode;
	BYTE type;
};
#pragma pack ()
struct PMSG_SETAGILITYBUG
{
	PBMSG_HEAD h;
	BYTE subcode;
	BYTE value;
};

struct PMSG_FRIEND_ROOMCREATE_RESULT
{
	PBMSG_HEAD h;
	unsigned char ServerIp[15];	// 3
	unsigned short RoomNumber;	// 12
	unsigned long Ticket;	// 14
	unsigned char Type;	// 18
	char Name[10];	// 19
	unsigned char Result;	// 23
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
struct PMSG_CONNECT_INFO
{
	PBMSG_HEAD		h;
	UCHAR			SubHead;
	char			IP[16];
	USHORT			Port;
};
struct PWMSG_DEFAULT2
{
	PWMSG_HEAD h;
	BYTE subcode;
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
struct PMSG_MUBOT_SWITCH
{
	PBMSG_HEAD2 h;
	BYTE Status;
};

struct PMSG_MUBOT_USE
{
	PBMSG_HEAD2 h;
	BYTE Time;
	BYTE TimeMultipler;
	DWORD Money;
	BYTE Status;
};

struct PMSG_CLIENTTIME_OLD
{
	PBMSG_HEAD h;
	DWORD Time;	// 4
	WORD AttackSpeed;	// 8
	WORD MagicSpeed;	// A
};

struct PMSG_POST_DATA
{
	PBMSG_HEAD2 h;
	BYTE btColorRGB[3];
	char szServerName[50];
	char szTag[20];
	char szName[10];
	char szMessage[90];
};

struct PMSG_SET_CHAT_COLOR // C1:FA:12
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

bool CliProtocolCore(LPBYTE aRecv, BYTE ProtoNum, int len, bool Encrypt);

void GCAttackResult(PMSG_ATTACKRESULT *lpMsg);
void GCDiePlayerSend(PMSG_DIEPLAYER *lpMsg);
void GCRefillSend(PMSG_REFILL* lpMsg);
void GCJoinResultSend(PMSG_JOINRESULT *lpMsg);
void GCCharMapJoinResult(PMSG_CHARMAPJOINRESULT *lpMsg);
void GCCharRegen(PMSG_CHARREGEN * lpMsg);
void GCLevelUpSend(PMSG_LEVELUP *lpMsg);
void GCLevelUpPointAdd(PMSG_LVPOINTADDRESULT *lpMsg);
void GCMasterLevelUpSend(PMSG_MASTER_LEVEL_UP_SEND *lpMsg);
void GCSetAgilityFix(PMSG_SETAGILITYBUG* lpMsg);
void GCSetCashItemMoveEnable();
void GCFriendRoomCreate(PMSG_FRIEND_ROOMCREATE_RESULT* lpMsg);
void GCEnableSiegeSkills();
void CGSendFilesCrc();
void GCSetCharSet(WORD CharSet);
void GCIGCStatsAdd(PMSG_ADDSTATS* lpMsg);
void GCManaSend(PMSG_MANASEND *lpMsg);
void __fastcall GCOffTradeReq(BYTE* aRecv);
inline void GCSendAntihackBreach(DWORD dwErrorCode);
void GCRareItemSetPrices(PMSG_RAREITEMPIRCESEND * lpMsg);
void GCAnsMuBotRecvUse(PMSG_MUBOT_USE * aRecv);
void GCDisableReconnect();
void CGEnableMUBot(int state);
void GCDisableReconnectSystem();
void GCAlterPShopVault(int type);
void GCDropSellMod();
void GCCustomPost(PMSG_POST_DATA *lpMsg);
void GCSetChatColors(PMSG_SET_CHAT_COLOR * lpMsg);
#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

