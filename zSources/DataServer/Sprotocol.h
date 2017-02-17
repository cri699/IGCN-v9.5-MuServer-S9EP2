// Sprotocol.h: interface for the Sprotocol class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPROTOCOL_H__5309DA38_D6AD_420B_896B_CB391FCD3624__INCLUDED_)
#define AFX_SPROTOCOL_H__5309DA38_D6AD_420B_896B_CB391FCD3624__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WzUdp.h"
#include "MapServerManager.h"

#define MAX_LOGIN_USER	(45000)
#define MAX_LOGIN_SERVER (MAX_MAP_GROUPS*MAX_MAP_SUBGROUPS+1)

enum PASSWORD_ENCRYPT_TYPES
{
	PWENC_NONE = 0,
	PWENC_WZMD5 = 1,
	PWENC_MD5 = 2,
	PWENC_SHA2 = 3
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
struct tagJOIN_SERVER_USER_DATA
{
	char m_AccoundID[11];
	char m_HWID[100];
	WORD m_ServerCode;
	WORD m_ServerGroup;
	bool m_bRequestMapSvrMove;
	int iUserNumber;
	DWORD dwPlayTime;
	char m_Ip[17];
	int iPayCode;
	int iUsedTime;
	char szEndDays[9];
	DWORD dwCheckTime;
	int iIndex;
	bool m_bOffTrade;	
	char IpAddr[17];
};

struct tagJOIN_SERVER_SERVER_DATA
{
	int  m_ServerIndex;
	char m_szServerName[50];
	WORD m_wServerCode;
	WORD m_wPort;
	WORD m_wCurrentUser;
	WORD m_wMaxUser;
	BYTE m_VipServer;
	WORD m_MaxHWIDUseCount;
};

extern void MuLoginDeleteMapMove(char * szAccountID);
// EventControl
//-----------------------------
#define DRAGON_EVENT		0
#define XMAS_EVENT			1
#define FIRECRACKER_EVENT	2
#define HEARTOFLOVE_EVENT	3
#define MEDAL_EVENT			4

// OptionControl
//-----------------------------
#define OPTION_RELOAD		0
#define MONSTER_RELOAD		1
#define SHOP_RELOAD			2
#define ALLOPTION_RELOAD	3
#define EVENTINFO_RELOAD	4
#define ETCINFO_RELOAD		5
#define OPTION_CONTROL		6





/* * * * * * * * * * * * * * * * * * * * * 
 *	Packet User Close by Id
 *	Direction : GameServer -> JoinServer
 *  Code : 0xC1
 *	HeadCode : 0x05
 */
struct SDHP_USERCLOSE_ID
{
	PBMSG_HEAD h;
	char szId[10];
	char szName[10];
	unsigned short Level;
	unsigned char DbClass;

};



/* * * * * * * * * * * * * * * * * * * * * 
 *	Packet Id Pass Result
 *	Direction : JoinServer -> GameServer
 *  Code : 0xC1
 *	HeadCode : 0x01
 */
struct SDHP_IDPASSRESULT
{
	PBMSG_HEAD h;
	unsigned char result;	// 3
	short Number;	// 4
	char Id[10];	//	6
	int UserNumber;
	int DBNumber;
	char JoominNumber[20];
};



/* * * * * * * * * * * * * * * * * * * * * 
 *	Packet Join Fail
 *	Direction : GameServer -> JoinServer
 *  Code : 0xC1
 *	HeadCode : 0x02
 */
struct SDHP_JOINFAIL
{
	PBMSG_HEAD h;
	short Number;
	char Id[11];
	int UserNumber;
	int DBNumber;

};



/* * * * * * * * * * * * * * * * * * * * * 
 *	Packet Join Fail
 *	Direction : GameServer -> JoinServer
 *  Code : 0xC1
 *	HeadCode : 0x04
 */
struct SDHP_COMMAND_BLOCK
{
	PBMSG_HEAD h;
	short Number;
	char Id[11];
	int UserNumber;
	int DBNumber;
	unsigned char BlockCode;
};


struct SDHP_BILLSEARCH
{
	PBMSG_HEAD h;	// C1:06
	char Id[10];	// 3
	short Number;	// E
};


/* * * * * * * * * * * * * * * * * * * * * 
 *	Packet Bill Search Result
 *	Direction : JoinServer -> GameServer
 *  Code : 0xC1
 *	HeadCode : 0x06
 */
struct SDHP_BILLSEARCH_RESULT
{
	PBMSG_HEAD h;
	char Id[10];
	short Number;
	unsigned char cCertifyType;
	unsigned char PayCode;
	char EndsDays[12];
	int EndTime;
	unsigned char cVipType;
};


/* * * * * * * * * * * * * * * * * * * * * 
 *	Packet Bill Kill User
 *	Direction : JoinServer -> GameServer
 *  Code : 0xC1
 *	HeadCode : 0x07, 0x09
 */
struct SDHP_BILLKILLUSER
{
	struct PBMSG_HEAD h;
	char Id[10];
	short Number;
	bool ForceDisconnect;
};





/* * * * * * * * * * * * * * * * * * * * * 
 *	Packet Other Join Message
 *	Direction : JoinServer -> GameServer
 *  Code : 0xC1
 *	HeadCode : 0x08
 */
struct SDHP_OTHERJOINMSG
{
	struct PBMSG_HEAD h;
	char AccountID[10];
};


/* * * * * * * * * * * * * * * * * * * * * 
 *	Packet Other Join Message
 *	Direction : JoinServer -> GameServer
 *  Code : 0xC1
 *	HeadCode : 0x20
 */
struct SDHP_EXITMSG
{
	PBMSG_HEAD h;
	unsigned char ExitCode[3];
};


/* * * * * * * * * * * * * * * * * * * * * 
 *	Packet Other Join Message
 *	Direction : JoinServer -> GameServer
 *  Code : 0xC1
 *	HeadCode : 0x30
 */
struct SDHP_EVENTSTART
{
	PBMSG_HEAD h;
	unsigned char Event;
};

/* * * * * * * * * * * * * * * * * * * * * 
 *	Packet Other Join Message
 *	Direction : JoinServer -> GameServer
 *  Code : 0xC1
 *	HeadCode : 0x40
 */
struct SDHP_EVENTCONTROL
{
	PBMSG_HEAD h;
	unsigned char Event;
	unsigned char Stat;
};

/* * * * * * * * * * * * * * * * * * * * * 
 *	Packet Other Join Message
 *	Direction : JoinServer -> GameServer
 *  Code : 0xC1
 *	HeadCode : 0x41
 */
struct SDHP_OPTIONCONTROL
{
	PBMSG_HEAD h;
	unsigned char Option;
};



/* * * * * * * * * * * * * * * * * * * * * 
 *	Packet Join Fail
 *	Direction : GameServer -> JoinServer
 *  Code : 0xC1
 *	HeadCode : 0x30
 */
struct SDHP_LOVEHEARTEVENT
{
	PBMSG_HEAD h;
	char Account[10];
	char Name[10];
};




/* * * * * * * * * * * * * * * * * * * * * 
 *	Packet Other Join Message
 *	Direction : JoinServer -> GameServer
 *  Code : 0xC1
 *	HeadCode : 0x61
 */
struct SDHP_LOVEHEARTEVENT_RESULT
{
	PBMSG_HEAD h;
	unsigned char Result;
	char Name[10];
	int Number;
};




/* * * * * * * * * * * * * * * * * * * * * 
 *	Packet Join Fail
 *	Direction : GameServer -> JoinServer
 *  Code : 0xC1
 *	HeadCode : 0x31
 */
struct SDHP_LOVEHEARTCREATE
{
	PBMSG_HEAD h;
	unsigned char x;
	unsigned char y;
	unsigned char MapNumber;
};



/* * * * * * * * * * * * * * * * * * * * * 
 *	Packet Other Join Message
 *	Direction : JoinServer -> GameServer
 *  Code : 0xC1
 *	HeadCode : 0x62
 */
struct SDHP_NOTICE
{
	PBMSG_HEAD h;
	char Notice[61];
};



/* * * * * * * * * * * * * * * * * * * * * 
 *	Packet Other Join Message
 *	Direction : JoinServer -> GameServer
 *  Code : 0xC1
 *	HeadCode : 0x63
 */
struct SDHP_USER_NOTICE
{
	PBMSG_HEAD h;
	char szId[10];
	char Notice[61];
};



/* * * * * * * * * * * * * * * * * * * * * 
 *	Packet Join Fail
 *	Direction : GameServer -> JoinServer
 *  Code : 0xC1
 *	HeadCode : 0x7A
 */
struct PMSG_REQ_MAPSVRMOVE
{
	PBMSG_HEAD h;
	int iIndex;
	char szAccountID[11];
	char szCharName[11];
	char szPassword[20];
	unsigned char btSecurityLock;
	unsigned int dwSecurityCode;
	unsigned short wCurMapSvrCode;
	unsigned short wDstMapSvrCode;
	unsigned short wMapNumber;
	unsigned char btX;
	unsigned char btY;
};


/* * * * * * * * * * * * * * * * * * * * * 
 *	Packet Other Join Message
 *	Direction : JoinServer -> GameServer
 *  Code : 0xC1
 *	HeadCode : 0x7A
 */
struct PMSG_ANS_MAPSVRMOVE
{
	PBMSG_HEAD h;
	int iIndex;
	char szAccountID[11];
	char szCharName[11];
	unsigned short wCurMapSvrCode;
	unsigned short wDstMapSvrCode;
	unsigned short wMapNumber;
	unsigned char btX;
	unsigned char btY;
	int iResult;
	int iJoinAuthCode1;
	int iJoinAuthCode2;
	int iJoinAuthCode3;
	int iJoinAuthCode4;
};




/* * * * * * * * * * * * * * * * * * * * * 
 *	Packet Map Server Auth Answer
 *	Direction : JoinServer -> GameServer
 *  Code : 0xC1
 *	HeadCode : 0x7A ?????????????????????????????? #error
 */
struct PMSG_ANS_MAPSVRAUTH
{
	PBMSG_HEAD h;
	int iIndex;
	char szAccountID[11];
	char szCharName[11];
	char szPassword[20];
	BYTE btSecurityLock;
	DWORD dwSecurityCode;
	WORD wPrevMapSvrCode;
	WORD wMapNumber;
	BYTE btX;
	BYTE btY;
	int iResult;
	int iUserNumber;
	int iDBNumber;
	unsigned char btBlockCode;
};



/* * * * * * * * * * * * * * * * * * * * * 
 *	Packet Join Fail
 *	Direction : GameServer -> JoinServer
 *  Code : 0xC1
 *	HeadCode : 0x7B 
 */
struct PMSG_REQ_MAPSVRAUTH
{
	PBMSG_HEAD h;
	int iIndex;
	char szAccountID[11];
	char szCharName[11];
	unsigned short wDstMapSvrCode;
	int iJoinAuthCode1;
	int iJoinAuthCode2;
	int iJoinAuthCode3;
	int iJoinAuthCode4;
};

/* * * * * * * * * * * * * * * * * * * * * 
 *	Packet Notify Max User
 *	Direction : GameServer -> JoinServer
 *  Code : 0xC1
 *	HeadCode : 0x7C
 */
struct PMSG_NOTIFY_MAXUSER
{
	PBMSG_HEAD h;
	int iSvrCode;	// 4
	int iMaxUserCount;	// 8
	int iCurUserCount;	//	C
};

struct PMSG_SET_OFFTRADE
{
	PBMSG_HEAD h;
	char szAccountID[11];
	char szName[11];
	bool m_bState;	// 4
};

/* * * * * * * * * * * * * * * * * * * * * 
 *	Packet Join Fail
 *	Direction : GameServer -> 
 *  Code : 0xC1
 *	HeadCode : 0x60
 */
/*struct PMSG_JG_MEMO_SEND
{
	PWMSG_HEAD h;
	char Name[10];
	char TargetName[10];
	char Subject[32];
	short MemoSize;
	char Memo[1000];
};*/

#pragma pack(1)

struct tagUSER_DATA 
{
	PMSG_ANS_MAPSVRMOVE pMapServerMoveData;
	int iUserNumber;
	DWORD dwTick;
	BYTE btSecurityLock;
	DWORD dwSecurityCode;
	char szPassword[20];
};

#pragma pack()

struct PMSG_LOGINSEVER_LIVE
{
	PBMSG_HEAD h;	// C1:02
	DWORD RQueue;
};

struct ISHOP_VIP_BUY
{
	PBMSG_HEAD		h;
	char			AccountID[11];
	BYTE			Type;
	int				Days;
};

class CLoginServerData
{
public:

	CLoginServerData();
	~CLoginServerData();

	void Init();

	void MuLoginDeleteServer(WORD ServerCode);
	int MuLoginFindServer(WORD ServerCode);
	int MuLoginAddServer(int ServerIndex, LPTSTR ServerName, WORD ServerCode, WORD Port, BYTE ServerVIP, WORD MaxHWIDUseCount);

	int GetVIPLevel(int Index)
	{
		return this->m_MuLoginServerData[Index].m_VipServer;
	}

	int GetHWIDUseCount(int Index)
	{
		return this->m_MuLoginServerData[Index].m_MaxHWIDUseCount;
	}

	LPTSTR GetServerName(int Index)
	{
		return this->m_MuLoginServerData[Index].m_szServerName;
	}

	int GetServerIndex(int Index)
	{
		return this->m_MuLoginServerData[Index].m_ServerIndex;
	}

	int CheckServerUserLimit(int Index)
	{
		return this->m_MuLoginServerData[Index].m_wCurrentUser >= this->m_MuLoginServerData[Index].m_wMaxUser;
	}

	void GetUserLimitData(int Index, int & CurUser, int & MaxUser)
	{
		CurUser = this->m_MuLoginServerData[Index].m_wCurrentUser;
		MaxUser = this->m_MuLoginServerData[Index].m_wMaxUser;
	}

	void SetUserLimitData(int Index, int CurUser, int MaxUser)
	{
		this->m_MuLoginServerData[Index].m_wCurrentUser = CurUser;
		this->m_MuLoginServerData[Index].m_wMaxUser = MaxUser;
	}

private:

	tagJOIN_SERVER_SERVER_DATA m_MuLoginServerData[MAX_LOGIN_SERVER];
	int m_MuLoginServerDataCount;

};

class CLoginUserData
{
public:

	CLoginUserData();
	~CLoginUserData();
	friend class CLoginServerProtocol;

	void Init();

	int MuLoginAddUser(WORD ServerCode, WORD ServerGroup, char * szAccountID, int UserIndex, char * szIp, char * szHWID);
	int MuLoginFindUser(char * szAccountID);
	void MuLoginDeleteMapMove(char * szAccountID);
	void MuLoginDeleteUser(char * szAccountID);
	void MuLoginDeleteUser(WORD ServerCode, WORD ServerGroup);
	unsigned long MakeAccountKey(LPTSTR lpszAccountID);
	int CheckMoveTimeOut(char * szAccountID);
	int CheckAccountID(LPTSTR szAccountID);
	int SetVIPData(LPTSTR szAccountID, int VIPType, LPTSTR szDate, int Minutes);

	bool CheckHWIDLimit_Group(WORD ServerGroup, LPTSTR szHWID);
	bool CheckHWIDLimit_Local(WORD ServerCode, LPTSTR szHWID, DWORD HWIDMaxUse);

	CRITICAL_SECTION critUserData;
	std::vector<tagUSER_DATA> m_vecMapMove;

	void IncUserNumber()
	{
		this->m_iUserNumber++;
	}

	void DecUserNumber()
	{
		this->m_iUserNumber--;
	}

	long GetUserNumber()
	{
		return this->m_iUserNumber;
	}

	LPSTR GetAccountID(int Index)
	{
		return this->m_MuLoginUserData[Index].m_AccoundID;
	}

	LPSTR GetHWID(int Index)
	{
		return this->m_MuLoginUserData[Index].m_HWID;
	}

	LPSTR GetIpAddr(int Index)
	{
		return this->m_MuLoginUserData[Index].IpAddr;
	}

	int GetUserIndex(int Index)
	{
		return this->m_MuLoginUserData[Index].iIndex;
	}

	int GetUserNumber(int Index)
	{
		return this->m_MuLoginUserData[Index].iUserNumber;
	}

	int GetServerCode(int Index)
	{
		return this->m_MuLoginUserData[Index].m_ServerCode;
	}

	bool CheckMapServerMove(int Index)
	{
		return this->m_MuLoginUserData[Index].m_bRequestMapSvrMove;
	}

	void SetMapServerMove(int Index, bool Move)
	{
		this->m_MuLoginUserData[Index].m_bRequestMapSvrMove = Move;
	}

	bool CheckOffTrade(int Index)
	{
		return this->m_MuLoginUserData[Index].m_bOffTrade;
	}

	void SetOffTrade(int Index, bool OffTrade)
	{
		this->m_MuLoginUserData[Index].m_bOffTrade = OffTrade;
	}

	void SetServerCode(int Index, WORD ServerCode)
	{
		this->m_MuLoginUserData[Index].m_ServerCode = ServerCode;
	}

	void SetUserIndex(int Index, int iUserIndex)
	{
		this->m_MuLoginUserData[Index].iIndex = iUserIndex;
	}
	int m_MuLoginUserDataCount;
private:
	tagJOIN_SERVER_USER_DATA m_MuLoginUserData[MAX_LOGIN_USER];
	int m_iUserNumber;
};

class CLoginServerProtocol
{
public:

	CLoginServerProtocol();
	~CLoginServerProtocol();
	BOOL Init();
	static void ProtocolCore(int aIndex, BYTE HeadCode, LPBYTE aRecv, int iSize);
	BOOL DisconnectPlayer(LPTSTR szAccountID);
	void DisconnectServer(WORD ServerCode);
	void CheckVIPTimeProc();

private:

	void JoinServerLogin(int aIndex, SDHP_SERVERINFO * lpMsg);
	void JGPAccountRequest(int aIndex, SDHP_IDPASS * aRecv);
	void GJPAccountFail(int aIndex, SDHP_JOINFAIL * aRecv);
	void GJPAccountBlock(int aIndex, SDHP_COMMAND_BLOCK * aRecv);
	void GJPUserClose(int aIndex, SDHP_USERCLOSE_ID * aRecv );
	void JGOtherJoin(int aIndex, LPTSTR szAccountID);
	void GCUserKillSend(int MuLoginIndex, bool IsForceDC);
	void GCJoinBillCheckSend(int aIndex, SDHP_BILLSEARCH * aRecv);
	void LoveHeartEventRecv(int aIndex, SDHP_LOVEHEARTEVENT * aRecv);
	void LoveHeartCreateSend(int aIndex, SDHP_LOVEHEARTCREATE * aRecv);
	void GJReqMapSvrMove(int aIndex, PMSG_REQ_MAPSVRMOVE * aRecv);
	void GJReqMapSvrAuth(int aIndex, PMSG_REQ_MAPSVRAUTH * aRecv);
	void GJNotifyMaxUserCount(int aIndex, PMSG_NOTIFY_MAXUSER * aRecv);
	void GJReqSetOffTrade(int aIndex, PMSG_SET_OFFTRADE * aRecv);
	void WJKillUser(int aIndex,SDHP_USERCLOSE_ID * aRecv);
	void GJReqVipAdd(int aIndex, ISHOP_VIP_BUY *aRecv);
	static unsigned long __stdcall ConnectServerThread(LPVOID);
	void InsertDataMuLog(LPTSTR ServerName, LPTSTR Id, LPTSTR Ip, LPTSTR State, LPTSTR HWID);

	CRITICAL_SECTION userCheck;
	char m_Salt[30];
	WzUdp m_ConnecServerUDP;

	CQuery m_AccountDB;
	CQuery m_LogDB;
	CQuery m_VIPDB;

	CLoginServerData m_ServerData;
public:
	CLoginUserData   m_UserData;

};

#endif // !defined(AFX_SPROTOCOL_H__5309DA38_D6AD_420B_896B_CB391FCD3624__INCLUDED_)


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

