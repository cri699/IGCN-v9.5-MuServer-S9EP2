// Fprotocol.h: interface for the Fprotocol class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FPROTOCOL_H__A35D7AEF_4EC5_4387_9DAA_293F0080F4E5__INCLUDED_)
#define AFX_FPROTOCOL_H__A35D7AEF_4EC5_4387_9DAA_293F0080F4E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

using namespace std;

struct strCmp
{
	bool operator()( const std::string s1, const std::string s2 ) const 
	{
		return strcmp( s1.data(), s2.data() ) < 0;
	}
};

struct FHP_FRIENDLIST_REQ 
{

  /*<thisrel this+0x0>*/ /*|0x3|*/ struct PBMSG_HEAD h;
  /*<thisrel this+0x4>*/ /*|0x2|*/ short Number;
  /*<thisrel this+0x6>*/ /*|0xa|*/ char Name[10];
  /*<thisrel this+0x10>*/ /*|0x1|*/ unsigned char pServer;

};
// <size 0x12>

struct FHP_FRIENDLIST 
{

  /*<thisrel this+0x0>*/ /*|0xa|*/ char Name[10];
  /*<thisrel this+0xa>*/ /*|0x1|*/ unsigned char Server;
};
// <size 0xb>

struct FHP_FRIENDLIST_COUNT 
{

  /*<thisrel this+0x0>*/ /*|0x4|*/ struct PWMSG_HEAD h;
  /*<thisrel this+0x4>*/ /*|0x2|*/ short Number;
  /*<thisrel this+0x6>*/ /*|0xa|*/ char Name[10];
  /*<thisrel this+0x10>*/ /*|0x1|*/ unsigned char Count;
  /*<thisrel this+0x11>*/ /*|0x1|*/ unsigned char MailCount;
  /*<thisrel this+0x12>*/ /*|0x1|*/ unsigned char Server;
};
// <size 0x12>

struct FHP_WAITFRIENDLIST_COUNT
{
	PBMSG_HEAD h;	//	
	short Number;	//	4
	char Name[10];	//	6
	char FriendName[10];	//	10
};

struct FHP_FRIEND_STATE_C 
{

  /*<thisrel this+0x0>*/ /*|0x3|*/ struct PBMSG_HEAD h;
  /*<thisrel this+0x4>*/ /*|0x2|*/ short Number;
  /*<thisrel this+0x6>*/ /*|0xa|*/ char Name[10];
  /*<thisrel this+0x10>*/ /*|0x1|*/ unsigned char State;
};
// <size 0x12>


struct FHP_FRIEND_STATE
{
	PBMSG_HEAD h;	//	
	short Number;	//	4
	char Name[10];	//	6
	char FriendName[10];	//	10
	unsigned char State;	//	1A
};

struct FHP_FRIEND_ADD_REQ 
{
  /*<thisrel this+0x0>*/ /*|0x3|*/ struct PBMSG_HEAD h;
  /*<thisrel this+0x4>*/ /*|0x2|*/ short Number;
  /*<thisrel this+0x6>*/ /*|0xa|*/ char Name[10];
  /*<thisrel this+0x10>*/ /*|0xa|*/ char FriendName[10];
};
// <size 0x1a>

struct FHP_FRIEND_ADD_RESULT
{
	PBMSG_HEAD h;
	short Number;	// 4
	unsigned char Result;	// 6
	char Name[10];	// 7
	char FriendName[10];	// 11
	unsigned char Server;	// 1B
};

struct FHP_WAITFRIEND_ADD_REQ 
{

  /*<thisrel this+0x0>*/ /*|0x3|*/ struct PBMSG_HEAD h;
  /*<thisrel this+0x3>*/ /*|0x1|*/ unsigned char Result;
  /*<thisrel this+0x4>*/ /*|0x2|*/ short Number;
  /*<thisrel this+0x6>*/ /*|0xa|*/ char Name[10];
  /*<thisrel this+0x10>*/ /*|0xa|*/ char FriendName[10];
};
// <size 0x1a>

struct FHP_WAITFRIEND_ADD_RESULT
{
	PBMSG_HEAD h;	//	
	short Number;	//	4
	unsigned char Result;	//	6
	char Name[10];	//	7
	char FriendName[10];	//	11
	unsigned char pServer;	//	1B
};


struct FHP_FRIEND_DEL_RESULT
{
	PBMSG_HEAD h;	//	
	short Number;	//	4
	unsigned char Result;	//	6
	char Name[10];	//	7
	char FriendName[10];	//	11
};

struct FHP_FRIEND_MEMO_SEND 
{
  /*<thisrel this+0x0>*/ /*|0x4|*/ struct PWMSG_HEAD h;
  /*<thisrel this+0x4>*/ /*|0x2|*/ short Number;
  /*<thisrel this+0x8>*/ /*|0x4|*/ unsigned long WindowGuid;
  /*<thisrel this+0xc>*/ /*|0xa|*/ char Name[10];
  /*<thisrel this+0x16>*/ /*|0xa|*/ char ToName[10];
  /*<thisrel this+0x20>*/ /*|0x20|*/ char Subject[60];
  /*<thisrel this+0x40>*/ /*|0x1|*/ unsigned char Dir;
  /*<thisrel this+0x41>*/ /*|0x1|*/ unsigned char Action;
  /*<thisrel this+0x42>*/ /*|0x2|*/ short MemoSize;
  /*<thisrel this+0x44>*/ /*|0x12|*/ unsigned char Photo[18];
  /*<thisrel this+0x56>*/ /*|0x3e8|*/ char Memo[1000];
};
// <size 0x440>

struct FHP_FRIEND_MEMO_SEND_OLD 
{
	struct PWMSG_HEAD h;
	short Number;
	unsigned long WindowGuid;
	char Name[10];
	char ToName[10];
	char Subject[32];
	unsigned char Dir;
	unsigned char Action;
	short MemoSize;
	unsigned char Photo[13];
	char Memo[1000];
};

struct FHP_FRIEND_MEMO_SEND_RESULT
{
	PBMSG_HEAD h;	//	
	short Number;	//	4
	char Name[10];	//	6
	unsigned char Result;	//	10
	unsigned long WindowGuid;	//	14
};

struct FHP_FRIEND_MEMO_LIST_REQ 
{

  /*<thisrel this+0x0>*/ /*|0x3|*/ struct PBMSG_HEAD h;
  /*<thisrel this+0x4>*/ /*|0x2|*/ unsigned short Number;
  /*<thisrel this+0x6>*/ /*|0xa|*/ char Name[10];

};
// <size 0x10>

struct FHP_FRIEND_MEMO_LIST
{
	PWMSG_HEAD h;	//	
	unsigned short Number;	//	4
	unsigned short MemoIndex;	//	6
	char SendName[10];	//	8
	char RecvName[10];	//	12
	char Date[30];	//	1C
	char Subject[60];	//	3A
	unsigned char read;	//	5A
};

struct FHP_FRIEND_MEMO_RECV_REQ 
{

  /*<thisrel this+0x0>*/ /*|0x3|*/ struct PBMSG_HEAD h;
  /*<thisrel this+0x4>*/ /*|0x2|*/ short Number;
  /*<thisrel this+0x6>*/ /*|0x2|*/ unsigned short MemoIndex;
  /*<thisrel this+0x8>*/ /*|0xa|*/ char Name[10];

};
// <size 0x12>

struct FHP_FRIEND_MEMO_RECV
{
	PWMSG_HEAD h;	//	
	short Number;	//	4
	char Name[10];	//	6
	unsigned short MemoIndex;	//	10
	short MemoSize;	//	12
	unsigned char Photo[18];	//	14
	unsigned char Dir;	//	26
	unsigned char Action;	//	27
	char Memo[1000];	//	28
};

struct FHP_FRIEND_MEMO_RECV_OLD
{
	PWMSG_HEAD h;
	short Number;
	char Name[10];
	unsigned short MemoIndex;
	short MemoSize;
	unsigned char Photo[13];
	unsigned char Dir;
	unsigned char Action;
	char Memo[1000];
};

struct FHP_FRIEND_MEMO_DEL_REQ 
{

  /*<thisrel this+0x0>*/ /*|0x3|*/ struct PBMSG_HEAD h;
  /*<thisrel this+0x4>*/ /*|0x2|*/ short Number;
  /*<thisrel this+0x6>*/ /*|0x2|*/ unsigned short MemoIndex;
  /*<thisrel this+0x8>*/ /*|0xa|*/ char Name[10];

};
// <size 0x12>


struct FHP_FRIEND_MEMO_DEL_RESULT
{
	PBMSG_HEAD h;	//	
	unsigned char Result;	//	3
	unsigned short MemoIndex;	//	4
	short Number;	//	6
	char Name[10];	//	8
};

struct MEMO_HEADER
{
	unsigned short MemoIndex;
	char SendName[10];
	char RecvName[10];
	char Date[30];
	char Subject[60];
	unsigned char read;
};

struct MEMO_SEND_HEADER
{
	char Name[10];
	char ToName[10];
	char Subject[60];
	unsigned char Dir;
	unsigned char Action;
};

struct MEMO_READ
{
	unsigned char Dir;
	unsigned char Action;
	char Memo[1000];
};

#pragma pack (1)

struct NAME
{
	char m_szName[11];
	NAME()
	{
		ZeroMemory(this, sizeof(*this));
	}
	NAME(char *szName)
	{
		ZeroMemory(this, sizeof(*this));
		strncpy(m_szName, szName, 10);
	}
};

typedef struct _FRIEND_MASTER
{
	int m_iGUID;
	int m_iMemoTotal;
	int m_iNumber;
	BYTE m_btState;
	int m_iServer;
	std::vector<NAME> m_vecFriends;

	_FRIEND_MASTER()
	{
		m_iGUID = -1;
		m_iMemoTotal = 0;
		m_iNumber = -1;
		m_btState = -1;
		m_iServer = -1;
		m_vecFriends.clear();
	}
}FRIEND_MASTER, *LPFRIEND_MASTER;

typedef struct _REQUEST_CMD
{
	int m_aIndex;
	BYTE m_btData[1024];
	_REQUEST_CMD()
	{
		ZeroMemory(this, sizeof(*this));
	}
}REQUEST_CMD, *LPREQUEST_CMD;
#pragma pack()

class CFriendSystem
{
public:

	CFriendSystem();
	~CFriendSystem();

	BOOL FriendDBConnect();

	void FriendListRequest(int aIndex, FHP_FRIENDLIST_REQ* lpMsg);
	void FriendStateClientRecv(int aIndex, FHP_FRIEND_STATE_C* lpMsg);
	void FriendAddRequest(int aIndex, FHP_FRIEND_ADD_REQ* lpMsg);
	void WaitFriendAddRequest(int aIndex, FHP_WAITFRIEND_ADD_REQ* lpMsg);
	void FriendDelRequest(int aIndex, FHP_FRIEND_ADD_REQ* lpMsg);

	void FriendMemoSend(int aIndex, FHP_FRIEND_MEMO_SEND* lpMsg);
	void FriendMemoListReq(int aIndex, FHP_FRIEND_MEMO_LIST_REQ *lpMsg);
	void FriendMemoReadReq(int aIndex, FHP_FRIEND_MEMO_RECV_REQ* lpMsg);
	void FriendMemoDelReq(int aIndex, FHP_FRIEND_MEMO_DEL_REQ* lpMsg);

	int ExDBGetIndexByCode(int iServerCode);

	BOOL UserGuidDBCreate(char *szName);
	BOOL GetDBGuidAndMemoTotal(char *szName, int& guid, int& memo_total);
	BOOL GetDBFriendList(char *szMaster);

	BOOL FriendExists(char *szMaster, char *szFriend);
	int GetFriendGuid(char *szMaster);
	BYTE GetFriendState(char *szMaster);
	int GetFriendServer(char *szMaster);
	int GetFriendNumber(char *szMaster);

	void FriendListSend(int aIndex, char *szMaster);

	BOOL GetDBWaitFriend(char *szMaster, OUT char *szWaitFriend);
	BOOL FriendWaitSend(int aIndex, char *szMaster);

	void SendState(int aIndex, char *szMaster, int iNumber, char *szName, BYTE btState);
	void SendStateToAllFriends(int aIndex, char *szMaster);

	BOOL AddFriend(char *szMaster, char *szFriend);
	BOOL DelFriend(char *szMaster, char *szFriend);
	int AddDBWaitFriend(char *szMaster, char *szFriend);
	int DelDBWaitFriend(char *szMaster, char *szFriend);

	int WaitFriendAdd(char *szMaster, char *szFriend);
	BOOL AddDBFriend(char *szMaster, char *szFriend);
	BOOL DelDBFriend(char *szMaster, char *szFriend);

	void FriendClose(int aIndex, LPBYTE lpMsg);

	BOOL CreateFriendMaster(char *szMaster);
	BOOL CreateFriendMaster(char *szMaster, int iNumber, int iServer);
	BOOL DeleteFriendMaster(char *szMaster);

	LPFRIEND_MASTER GetFriendMaster(char *szMaster);

	void MemoHeaderSend(int aIndex, WORD wNumber, MEMO_HEADER * lpMemoHead );
	void MemoListSend(int aIndex, WORD wNumber, char *szName);

	int DBWriteMail(MEMO_SEND_HEADER * lpMemoSendHdr, LPBYTE Photo, BYTE btPhotoSize, char *sMemo, int memo_size);
	int DBReadMail(int iMemoId, int iGuid, MEMO_READ * lpMemoRead, LPDWORD lpdwMemoSize, LPBYTE lpPhoto, LPBYTE lpPhotoSize);
	int DBDelMail(char *szName, int iMemoId);

	CRITICAL_SECTION m_csMapFriendMaster;

private:

	std::map<string, FRIEND_MASTER, strCmp> m_MapFriendManager;
	CQuery m_MailDB;
	CQuery m_FriendDB;
	int m_iMaxFriends;

};

#endif // !defined(AFX_FPROTOCOL_H__A35D7AEF_4EC5_4387_9DAA_293F0080F4E5__INCLUDED_)


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

