// EDSprotocol.h: interface for the EDSprotocol class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EDSPROTOCOL_H__675D1E8D_B9AC_45E3_8077_17EC82A9A5A5__INCLUDED_)
#define AFX_EDSPROTOCOL_H__675D1E8D_B9AC_45E3_8077_17EC82A9A5A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

using namespace std;

#include <map>
#include "GensSystemProtocol.h"
#include "Fprotocol.h"
#include "LogToFile.h"

#define MAX_MEMBER_GUILD	80

struct SDHP_USERCLOSE
{
	PBMSG_HEAD h;	//	
	char CharName[10];	//	3
	char GuildName[8];	//	D
	unsigned char Type;	//	15
};

struct SDHP_GUILDCREATE
{
	PBMSG_HEAD h;
	char GuildName[9];	// 3
	char Master[11];	// C
	BYTE Mark[32];	// 17
	BYTE NumberH;	// 37
	BYTE NumberL;	// 38
	BYTE btGuildType;	// 39
};

struct SDHP_GUILDMEMBER_INFO_GUILDNAME_REQUEST
{
	PBMSG_HEAD h;
	char szGuildName[MAX_GUILD_LEN + 1];
};

struct SDHP_GUILDCREATE_RESULT
{
	PBMSG_HEAD h;	//	
	unsigned char Result;	//	3
	unsigned char Flag;	//	4
	unsigned long GuildNumber;	//	8
	unsigned char NumberH;	//	C
	unsigned char NumberL;	//	D
	char Master[11];	//	E
	char GuildName[9];	//	19
	unsigned char Mark[32];	//	22
	unsigned char btGuildType;	//	42
};


struct SDHP_GUILDDESTROY
{
	PBMSG_HEAD h;
	BYTE NumberH;	// 3
	BYTE NumberL;	// 4
	char GuildName[8];	// 5
	char Master[10];	// D
};

struct SDHP_GUILDDESTROY_RESULT
{
	PBMSG_HEAD h;	//	
	unsigned char Result;	//	3
	unsigned char Flag;	//	4
	unsigned char NumberH;	//	5
	unsigned char NumberL;	//	6
	char GuildName[9];	//	7
	char Master[11];	//	10
};

struct SDHP_GUILDMEMBERADD
{
	PBMSG_HEAD h;
	char GuildName[9];	// 3
	char MemberID[11];	// C
	BYTE NumberH;	// 17
	BYTE NumberL;	// 18
};

struct SDHP_GUILDMEMBERADD_WITHOUT_USERINDEX
{
	PBMSG_HEAD h;
	char GuildName[9];	// 3
	char MemberID[11];	// C
};

struct SDHP_GUILDMEMBERADD_RESULT
{
	PBMSG_HEAD h;	//	
	unsigned char Result;	//	3
	unsigned char Flag;	//	4
	unsigned char NumberH;	//	5
	unsigned char NumberL;	//	6
	char GuildName[9];	//	7
	char MemberID[11];	//	10
	short pServer;	//	1C
};

struct SDHP_GUILDMEMBERADD_RESULT_WITHOUT_USERINDEX
{
	PBMSG_HEAD h;	//	
	unsigned char Result;	//	3
	unsigned char Flag;	//	4
	char GuildName[9];	//	7
	char MemberID[11];	//	10
	short pServer;	//	1C
	short Number;
};


struct SDHP_GUILDMEMBERDEL
{
	PBMSG_HEAD h;	// C1:33
	BYTE NumberH;	// 3
	BYTE NumberL;	// 4
	char GuildName[8];	// 5
	char MemberID[10];	// D
};



struct SDHP_GUILDMEMBERDEL_RESULT
{
	PBMSG_HEAD h;	//	
	unsigned char Result;	//	3
	unsigned char Flag;	//	4
	unsigned char NumberH;	//	5
	unsigned char NumberL;	//	6
	char GuildName[9];	//	7
	char MemberID[11];	//	10
};



struct SDHP_GUILDUPDATE
{
	PBMSG_HEAD h;	//	
	char GuildName[9];	//	3
	char Master[11];	//	C
	unsigned char Mark[32];	//	17
	int Score;	//	28
	unsigned char Count;	//	3C
};


struct SDHP_GUILDMEMBER_INFO_REQUEST
{
	PBMSG_HEAD h;	// C1:35
	BYTE NumberH;	// 3
	BYTE NumberL;	// 4
	char MemberID[10];	// 5
};


struct SDHP_GUILDMEMBER_INFO
{
	PBMSG_HEAD h;	//	
	char GuildName[9];	//	3
	char MemberID[11];	//	C
	unsigned char btGuildStatus;	//	17
	unsigned char btGuildType;	//	18
	short pServer;	//	1A
};

struct SDHP_GUILDALL_COUNT
{
	PWMSG_HEAD h;
	int Number;	// 4
	char GuildName[9];	// 8
	char Master[11];	// 11
	unsigned char Mark[32];	// 1C
	int score;	// 3C
	BYTE btGuildType;	// 40
	int iGuildUnion;	// 44
	int iGuildRival;	// 48
	char szGuildRivalName[9];	// 4C
	unsigned char Count;	// 55
};

struct SDHP_GUILDALL
{
	char MemberID[11];	// 0
	BYTE btGuildStatus;	// B
	short pServer;	// C
};




struct SDHP_GUILDSCOREUPDATE
{
	PBMSG_HEAD h;	//	
	char GuildName[9];	//	3
	int Score;	//	C
};



struct SDHP_GUILDNOTICE
{
	PBMSG_HEAD h;	//	
	char GuildName[9];	//	3
	char szGuildNotice[60];	//	C
};



struct SDHP_GUILDCREATED
{
	PBMSG_HEAD h;	//	
	unsigned short Number;	//	4
	char GuildName[9];	//	6
	char Master[11];	//	F
	unsigned char Mark[32];	//	1A
	int score;	//	3C
};

struct SDHP_GUILDLISTSTATE
{
	PBMSG_HEAD h;	//	
	unsigned char State;	//	3
	int Count;	//	4
};


struct EXSDHP_GUILD_ASSIGN_STATUS_REQ
{
	PBMSG_HEAD h;
	WORD wUserIndex;	// 4
	BYTE btType;	// 6
	BYTE btGuildStatus;	// 7
	char szGuildName[9];	// 8
	char szTargetName[11];	// 11
};

struct PMSG_REQ_GUILD_PERIODBUFF_INSERT
{
	PBMSG_HEAD2 head;
	char szGuildName[MAX_GUILD_LEN+1];
	WORD wBuffIndex;
	char btEffectType1;
	char btEffectType2;
	DWORD dwDuration;
	time_t lExpireDate;
};

struct PMSG_REQ_GUILD_PERIODBUFF_DELETE
{
	PBMSG_HEAD2 head;
	char btGuildCnt;
	WORD wBuffIndex[5];
};

struct EXSDHP_GUILD_ASSIGN_TYPE_REQ
{
	PBMSG_HEAD h;
	WORD wUserIndex;	// 4
	BYTE btGuildType;	// 6;
	char szGuildName[9];	// 7
};



struct EXSDHP_GUILD_ASSIGN_TYPE_RESULT
{
	PBMSG_HEAD h;	//	
	unsigned char btFlag;	//	3
	unsigned short wUserIndex;	//	4
	unsigned char btGuildType;	//	6
	unsigned char btResult;	//	7
	char szGuildName[9];	//	8
};



struct EXSDHP_GUILD_ASSIGN_STATUS_RESULT
{
	PBMSG_HEAD h;	//	
	unsigned char btFlag;	//	3
	unsigned short wUserIndex;	//	4
	unsigned char btType;	//	6
	unsigned char btResult;	//	7
	unsigned char btGuildStatus;	//	8
	char szGuildName[9];	//	9
	char szTargetName[11];	//	12
};

struct EXSDHP_SERVERGROUP_GENS_CHATTING_RECV
{
	PBMSG_HEAD h;	//	
	int iGensNum;	//	4
	char szCharacterName[10];	//	8
	char szChattingMsg[90];	//	12
};

struct EXSDHP_SERVERGROUP_GUILD_CHATTING_RECV
{
	PBMSG_HEAD h;	//	
	int iGuildNum;	//	4
	char szCharacterName[10];	//	8
	char szChattingMsg[90];	//	12
};

struct EXSDHP_SERVERGROUP_UNION_CHATTING_RECV
{
	PBMSG_HEAD h;	//	
	int iUnionNum;	//	4
	char szCharacterName[10];	//	8
	char szChattingMsg[90];	//	12
};



struct EXSDHP_RELATIONSHIP_JOIN_REQ 
{

  /*<thisrel this+0x0>*/ /*|0x3|*/ struct PBMSG_HEAD h;
  /*<thisrel this+0x4>*/ /*|0x2|*/ unsigned short wRequestUserIndex;
  /*<thisrel this+0x6>*/ /*|0x2|*/ unsigned short wTargetUserIndex;
  /*<thisrel this+0x8>*/ /*|0x1|*/ unsigned char btRelationShipType;
  /*<thisrel this+0xc>*/ /*|0x4|*/ int iRequestGuildNum;
  /*<thisrel this+0x10>*/ /*|0x4|*/ int iTargetGuildNum;
};
// <size 0x14>



struct EXSDHP_RELATIONSHIP_JOIN_RESULT
{
	PBMSG_HEAD h;	//	
	unsigned char btFlag;	//	3
	unsigned short wRequestUserIndex;	//	4
	unsigned short wTargetUserIndex;	//	6
	unsigned char btResult;	//	8
	unsigned char btRelationShipType;	//	9
	int iRequestGuildNum;	//	C
	int iTargetGuildNum;	//	10
	char szRequestGuildName[9];	//	14
	char szTargetGuildName[9];	//	1D
};

struct EXSDHP_RELATIONSHIP_BREAKOFF_REQ 
{

  /*<thisrel this+0x0>*/ /*|0x3|*/ struct PBMSG_HEAD h;
  /*<thisrel this+0x4>*/ /*|0x2|*/ unsigned short wRequestUserIndex;
  /*<thisrel this+0x6>*/ /*|0x2|*/ unsigned short wTargetUserIndex;
  /*<thisrel this+0x8>*/ /*|0x1|*/ unsigned char btRelationShipType;
  /*<thisrel this+0xc>*/ /*|0x4|*/ int iRequestGuildNum;
  /*<thisrel this+0x10>*/ /*|0x4|*/ int iTargetGuildNum;
};
// <size 0x14>

struct EXSDHP_RELATIONSHIP_BREAKOFF_RESULT
{
	PBMSG_HEAD h;	//	
	unsigned char btFlag;	//	3
	unsigned short wRequestUserIndex;	//	4
	unsigned short wTargetUserIndex;	//	6
	unsigned char btResult;	//	8
	unsigned char btRelationShipType;	//	9
	int iRequestGuildNum;	//	C
	int iTargetGuildNum;	//	10
};



struct EXSDHP_NOTIFICATION_RELATIONSHIP
{
	PWMSG_HEAD h;	//	
	unsigned char btFlag;	//	4
	unsigned char btUpdateFlag;	//	5
	unsigned char btGuildListCount;	//	6
	int iGuildList[100];	//	8
};



struct EXSDHP_UNION_RELATIONSHIP_LIST
{
	PWMSG_HEAD h;	//	
	unsigned char btFlag;	//	4
	unsigned char btRelationShipType;	//	5
	unsigned char btRelationShipMemberCount;	//	6
	char szUnionMasterGuildName[9];	//	7
	int iUnionMasterGuildNumber;	//	10
	int iRelationShipMember[100];	//	14
};

struct EXSDHP_KICKOUT_UNIONMEMBER_REQ 
{

  /*<thisrel this+0x0>*/ /*|0x4|*/ struct PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x2|*/ unsigned short wRequestUserIndex;
  /*<thisrel this+0x6>*/ /*|0x1|*/ unsigned char btRelationShipType;
  /*<thisrel this+0x7>*/ /*|0x8|*/ char szUnionMasterGuildName[8];
  /*<thisrel this+0xf>*/ /*|0x8|*/ char szUnionMemberGuildName[8];

};
// <size 0x18>

struct EXSDHP_KICKOUT_UNIONMEMBER_RESULT
{
	PBMSG_HEAD2 h;	//	
	unsigned char btFlag;	//	4
	unsigned short wRequestUserIndex;	//	5
	unsigned char btRelationShipType;	//	8
	unsigned char btResult;	//	9
	char szUnionMasterGuildName[9];	//	A
	char szUnionMemberGuildName[9];	//	13
};

struct EXSDHP_UNION_LIST_REQ 
{

  /*<thisrel this+0x0>*/ /*|0x3|*/ struct PBMSG_HEAD h;
  /*<thisrel this+0x4>*/ /*|0x2|*/ unsigned short wRequestUserIndex;
  /*<thisrel this+0x8>*/ /*|0x4|*/ int iUnionMasterGuildNumber;
};
// <size 0xc>

struct EXSDHP_UNION_LIST 
{

  /*<thisrel this+0x0>*/ /*|0x1|*/ unsigned char btMemberNum;
  /*<thisrel this+0x1>*/ /*|0x20|*/ unsigned char Mark[32];
  /*<thisrel this+0x21>*/ /*|0x8|*/ char szGuildName[8];

};
// <size 0x29>

struct EXSDHP_UNION_LIST_COUNT 
{

  /*<thisrel this+0x0>*/ /*|0x4|*/ struct PWMSG_HEAD h;
  /*<thisrel this+0x4>*/ /*|0x1|*/ unsigned char btCount;
  /*<thisrel this+0x5>*/ /*|0x1|*/ unsigned char btResult;
  /*<thisrel this+0x6>*/ /*|0x2|*/ unsigned short wRequestUserIndex;
  /*<thisrel this+0x8>*/ /*|0x4|*/ int iTimeStamp;
  /*<thisrel this+0xc>*/ /*|0x1|*/ unsigned char btRivalMemberNum;
  /*<thisrel this+0xd>*/ /*|0x1|*/ unsigned char btUnionMemberNum;
};
// <size 0x10>

struct FHP_FRIEND_CHATROOM_CREATE_REQ 
{

  /*<thisrel this+0x0>*/ /*|0x3|*/ struct PBMSG_HEAD h;
  /*<thisrel this+0x4>*/ /*|0x2|*/ short Number;
  /*<thisrel this+0x6>*/ /*|0xa|*/ char Name[10];
  /*<thisrel this+0x10>*/ /*|0xa|*/ char fName[10];
};
// <size 0x1a>

struct FHP_FRIEND_CHATROOM_CREATE_RESULT
{
	PBMSG_HEAD h;	//	
	unsigned char Result;	//	3
	short Number;	//	4
	char Name[10];	//	6
	char FriendName[10];	//	10
	char ServerIp[15];	//	1A
	unsigned short RoomNumber;	//	2A
	unsigned long Ticket;	//	2C
	unsigned char Type;	//	30
};

struct FCHP_CHATROOM_CREATE_REQ
{
	PBMSG_HEAD h;
	char Name[10];
	char FriendName[10];
	BYTE Type;
	short Number;
	short ServerId;
	short FriendNumber;
	short FriendServerId;
};

struct FCHP_CHATROOM_CREATE_RESULT
{
	PBMSG_HEAD h;
	BYTE Result;
	short RoomNumber;
	char Name[10];
	char FriendName[10];
	short Number;
	short ServerId;
	DWORD Ticket;
	DWORD Trash;
	BYTE Type;
};

struct FHP_FRIEND_INVITATION_REQ 
{

  /*<thisrel this+0x0>*/ /*|0x3|*/ struct PBMSG_HEAD h;
  /*<thisrel this+0x4>*/ /*|0x2|*/ short Number;
  /*<thisrel this+0x6>*/ /*|0xa|*/ char Name[10];
  /*<thisrel this+0x10>*/ /*|0xa|*/ char FriendName[10];
  /*<thisrel this+0x1a>*/ /*|0x2|*/ unsigned short RoomNumber;
  /*<thisrel this+0x1c>*/ /*|0x4|*/ unsigned long WindowGuid;
};
// <size 0x20>

struct FHP_FRIEND_INVITATION_RET
{
	PBMSG_HEAD h;
	unsigned char Result;	// 3
	short Number;	// 4
	char Name[10];	// 6
	DWORD WindowGuid;	// 10
};

struct FCHP_CHATROOM_INVITATION_REQ
{
	PBMSG_HEAD h;
	short RoomNumber;
	char Name[10];
	short Number;
	short ServerId;
	BYTE Type;
};
#pragma pack(1)

typedef struct _GUILD_MEMBER
{
	char m_szMemberName[11];
	BYTE m_btStatus;
	BYTE m_btConnected;
	int	 m_iUserIndex;
	_GUILD_MEMBER()
	{
		ZeroMemory(m_szMemberName, sizeof(m_szMemberName));
		m_btStatus = 0;
		m_btConnected = -1;
		m_iUserIndex = -1;
	}
}GUILD_MEMBER, *LPGUILD_MEMBER;

typedef map<string, GUILD_MEMBER, strCmp> MAP_GUILD_MEMBER;

typedef struct tagGUILD_INFO_STRUCT
{
	int m_iNumber;
	char m_szGuildName[9];
	char m_szGuildMaster[11];
	BYTE m_btGuildType;

	MAP_GUILD_MEMBER m_mapGuildMember;
	int m_iRivalGuild;
	int m_iUnionGuild;
	BYTE m_Mark[32];
	int m_Score;
	char m_Notice[128];
	BOOL m_bGSHasData;
	tagGUILD_INFO_STRUCT()
	{
		m_iNumber = -1;
		memset(m_szGuildName, 0, sizeof(m_szGuildName));
		memset(m_szGuildMaster, 0, sizeof(m_szGuildMaster));
		m_btGuildType = -1;
		memset(m_Mark, 0, sizeof(m_Mark));
		memset(m_Notice,0,sizeof(m_Notice));
		m_iRivalGuild = 0;
		m_iUnionGuild = 0;
		m_Score = 0;
		m_mapGuildMember.clear();
		m_bGSHasData = FALSE;
	}

}GUILD_INFO_STRUCT, *LPGUILD_INFO_STRUCT;

typedef struct GENS_USER
{
	char Name[11];
	int  iContributePoint;
	int  iGensClass;
	int	 iRank;
	int	 iReward;
} GENS_USER;

typedef struct tagUNION_DATA
{
	std::vector<int>	m_vecUnionMember;
}UNION_DATA, *LPUNION_DATA;

#pragma pack ()

// Season 8 Ep3
// Guild Matching

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

struct _stReqGuildMatchingList
{
	PBMSG_HEAD2 h;
	int nUserIndex;
	int nPage;
};

struct _stReqGuildMatchingListSearchWord
{
	PBMSG_HEAD2 h;
	int nUserIndex;
	int nPage;
	char szSearchWord[11];
};

struct _stAnsGuildMatchingList
{
	PWMSG_HEAD2 h;
	int nUserIndex;
	int nPage;
	int nTotalPage;
	int nListCount;
	int nResult;
	_stGuildMatchingList stGuildMatchingList[9];
};

struct _stReqDelGuildMatchingList
{
	PBMSG_HEAD2 h;
	int nUserIndex;
	int nGuildNumber;
};

struct _stReqGuildMatchingData
{
	PBMSG_HEAD2 h;
	int nUserIndex;
	_stGuildMatchingList _stGuildMatchingList;
};

struct _stGuildMatchingAllowListDB
{
	int nGuildNumber;
	char szApplicantName[MAX_ACCOUNT_LEN + 1];
	BYTE btApplicantClass;
	int nApplicantLevel;
	BYTE btState;
};

struct _stRegWaitGuildMatching
{
	PBMSG_HEAD2 h;
	int nUserIndex;
	_stGuildMatchingAllowListDB stAllowList;
};

struct _stReqDelWaitGuildMatchingList
{
	PBMSG_HEAD2 h;
	int nUserIndex;
	int nType;
	char szName[MAX_ACCOUNT_LEN + 1];
};

struct _stAnsDelGuildMatchingList
{
	PBMSG_HEAD2 h;
	int nUserIndex;
	int nResult;
};

struct _stAnsGuildMatchingData
{
	PBMSG_HEAD2 h;
	int nUserIndex;
	int nResult;
};

struct _stAnsWaitGuildMatching
{
	PBMSG_HEAD2 h;
	int nUserIndex;
	int nResult;
};

struct _stAnsDelWaitGuildMatchingList
{
	PBMSG_HEAD2 h;
	int nUserIndex;
	int nType;
	int nResult;
};

struct _stReqAllowJoinGuildMatching
{
	PBMSG_HEAD2 h;
	int nUserIndex;
	int nAllowType;
	char szName[MAX_ACCOUNT_LEN + 1];
	char szGuildName[MAX_GUILD_LEN + 1];
};

struct _stAnsAllowJoinGuildMatching
{
	PBMSG_HEAD2 h;
	int nUserIndex;
	int nAllowType;
	char szMemberName[MAX_ACCOUNT_LEN + 1];
	char szGuildName[MAX_GUILD_LEN + 1];
	int nResult;
};

struct _stReqWaitGuildMatchingList
{
	PBMSG_HEAD2 h;
	int nUserIndex;
	int nGuildNumber;
};

struct _stAnsWaitGuildMatchingList
{
	PWMSG_HEAD2 h;
	int nUserIndex;
	int nListCount;
	int nResult;
	_stGuildMatchingAllowListDB stAllowList[80];
};

struct _stGuildMatchingAllowList
{
	char szName[MAX_ACCOUNT_LEN + 1];
	BYTE btClass;
	int nLevel;
};

struct _stReqWaitStateListGuildMatching
{
	PBMSG_HEAD2 h;
	int nUserIndex;
	char szName[MAX_ACCOUNT_LEN + 1];

};

struct _stAnsWaitStateListGuildMatching
{
	PBMSG_HEAD2 h;
	char szGuildMasterName[MAX_ACCOUNT_LEN + 1];
	char szGuildName[MAX_GUILD_LEN + 1];
	int nUserIndex;
	int nResult;
};

struct _stAnsNotiGuildMatching
{
	PBMSG_HEAD2 h;
	int nUserIndex;
	int nResult;
};

struct _stAnsNotiGuildMatchingForGuildMaster
{
	PBMSG_HEAD2 h;
	int nUserIndex;
	int nResult;
};

struct _stAnsUseGuildMatchingGuild
{
	PBMSG_HEAD2 h;
	int nUserIndex;
};

// Party Matching
struct _stReqRegWantedPartyMember
{
	PBMSG_HEAD2 h;
	int nUserIndex;
	char szPartyLeaderName[MAX_ACCOUNT_LEN + 1];
	char szTitle[41];
	char szPassword[5];
	WORD nMinLevel;
	WORD nMaxLevel;
	WORD nHuntingGround;
	WORD nLeaderLevel;
	BYTE btWantedClass;
	BYTE btUsePassWord;
	BYTE btCurrentPartyCnt;
	BYTE btApprovalType;
	BYTE btWantedClassDetailInfo[7];
	BYTE btGensType;
	BYTE btLeaderClass;
};

struct _stAnsRegWantedPartyMember
{
	PBMSG_HEAD2 h;
	int nUserIndex;
	int nResult;
};

struct _stReqAddPartyMember
{
	PBMSG_HEAD2 h;
	int nUserIndex;
	int nMemberIndex;
	BYTE btType;
	BYTE btManualJoin;
	BYTE btAlreadyParty;
	char szLeaderName[MAX_ACCOUNT_LEN + 1];
	char szMemberName[MAX_ACCOUNT_LEN + 1];
};

struct _stReqGetPartyMatchingList
{
	PBMSG_HEAD2 h;
	int nUserIndex;
	int nPage;
	int nType;
	int nLevel;
	BYTE btClass;
	BYTE btGens;
	char szSearchWord[11];
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

struct _stAnsGetPartyMatchingList
{
	PWMSG_HEAD2 h;
	int nUserIndex;
	int nListCount;
	int nPage;
	int nTotalPage;
	int nResult;
	_PARTY_INFO_LIST stPartyInfoList[6];
};

struct _stReqJoinMemberPartyMatching
{
	PBMSG_HEAD2 h;
	char szMemberName[MAX_ACCOUNT_LEN + 1];
	char szLeaderName[MAX_ACCOUNT_LEN + 1];
	char szPassword[5];
	int nUserIndex;
	int nUserDBNumber;
	int nLevel;
	BYTE btClass;
	BYTE btRandomParty;
	BYTE btGensType;
	BYTE btChangeUpClass;
};

struct _stAnsJoinMemberPartyMatching
{
	PBMSG_HEAD2 h;
	int nUserIndex;
	int nResult;
};

struct _stReqJoinMemberStateListPartyMatching
{
	PBMSG_HEAD2 h;
	int nUserIndex;
	char szMemberName[MAX_ACCOUNT_LEN + 1];
};

struct _stAnsJoinMemberStateListPartyMatching
{
	PBMSG_HEAD2 h;
	int nUserIndex;
	int nResult;
	BYTE btLeaderChannel;
	char szLeaderName[MAX_ACCOUNT_LEN + 1];
};

struct _stReqWaitListPartyMatching
{
	PBMSG_HEAD2 h;
	int nUserIndex;
	char szLeaderName[MAX_ACCOUNT_LEN + 1];
};

struct _PARTY_MEMBER_WAIT_LIST
{
	char szName[MAX_ACCOUNT_LEN + 1];
	BYTE btClass;
	int nLevel;
};

struct _stAnsWaitListPartyMatching
{
	PWMSG_HEAD2 h;
	int nUserIndex;
	int nListCount;
	int nResult;
	_PARTY_MEMBER_WAIT_LIST stList[10];
};

struct _SEND_PARTYMEMBER_INFO
{
	_SEND_PARTYMEMBER_INFO()
	{
		this->bUse = FALSE;
	}

	int bUse;
	int bFlag;
	char Name[MAX_ACCOUNT_LEN + 1];
	int nUserIndex;
	int nServerChannel;
};

struct _stAnsSendPartyMemberList
{
	PBMSG_HEAD2 h;
	int nUserIndex;
	int nMemberCount;
	int nPartyMatchingIndex;
	_SEND_PARTYMEMBER_INFO stList[5];
};

struct _stReqCancelPartyMatching
{
	PBMSG_HEAD2 h;
	BYTE btType;
	char szName[MAX_ACCOUNT_LEN + 1];
	int nUserIndex;
};

struct _stAnsCancelPartyMatching
{
	PBMSG_HEAD2 h;
	BYTE btType;
	int nUserIndex;
	int nResult;
};

struct _stAnsAddPartyMember
{
	PBMSG_HEAD2 h;
	int nResult;
	int nUserIndex;
	int nMemberIndex;
	BYTE btType;
	BYTE btSendType;
	BYTE btManualJoin;
	char szMemberName[MAX_ACCOUNT_LEN + 1];
};

struct _stAnsAddRealPartyMember
{
	PBMSG_HEAD2 h;
	int nLeaderUserIndex;
	int nMemberUserIndex;
};

struct _stReqDelPartyUserPartyMatching
{
	PBMSG_HEAD2 h;
	char szTargetName[MAX_ACCOUNT_LEN + 1];
	int nUserIndex;
};

struct _stAnsRequestJoinPartyMatchingNoti
{
	PBMSG_HEAD2 h;
	int nUserIndex;
};

struct _stAnsDelPartyUserPartyMatching
{
	PBMSG_HEAD2 h;
	int nUserIndex;
	int nTargetIndex;
	int nResult;
	BYTE btType;
};

struct _stReqChattingPartyMatching
{
	PBMSG_HEAD2 h;
	int nPartyIndex;
	char szChat[63];
	char szSendCharName[MAX_ACCOUNT_LEN + 1];
};

struct _stAnsChattingPartyMatching
{
	PBMSG_HEAD2 h;
	int nRecvUserIndex;
	int nPartyIndex;
	char szChat[63];
	char szSendCharName[MAX_ACCOUNT_LEN + 1];
};

struct _stReqSendPartyMemberList
{
	PBMSG_HEAD2 h;
	char szLeaderName[MAX_ACCOUNT_LEN + 1];
};

struct PARTYMATCHING_PARTYUSERDATA
{
	PARTYMATCHING_PARTYUSERDATA()
	{
		this->iUsedInfo = FALSE;
		this->iUserConnected = FALSE;
		this->iUserIndex = -1;
		this->iServerNumber = -1;
		this->iServerIndex = -1;
	}

	int iUsedInfo;
	int iUserConnected;
	int iUserIndex;
	int iServerNumber;
	int iServerIndex;
	char szUserName[MAX_ACCOUNT_LEN + 1];
};

struct PARTYMATCHING_PARTYDATA
{
	PARTYMATCHING_PARTYDATA()
	{
		this->m_PartyMatchingIndex = -1;
		this->m_UserCount = 0;
	}

	int								m_PartyMatchingIndex;
	int								m_UserCount;
	char							szLeaderName[MAX_ACCOUNT_LEN + 1];
	PARTYMATCHING_PARTYUSERDATA		m_UserData[5];
};

class CPartyMatchingData
{
public:
	CPartyMatchingData();
	~CPartyMatchingData();

	void Init();

	void CreateParty(int Server, int ServerIndex, int nLeaderIndex, char *szLeaderName);
	void DeleteParty(char *szLeaderName);

	PARTYMATCHING_PARTYDATA * GetPartyDataByMaster(char *szLeaderName);
	PARTYMATCHING_PARTYDATA * GetPartyDataByMember(char *szMemberName);
	PARTYMATCHING_PARTYDATA * GetPartyDataByNumber(int nPartyMatchingIndex);

	PARTYMATCHING_PARTYUSERDATA * GetPartyMemberData(char *szMemberName);

	int AddPartyMember(char *szLeaderName, char *szMemberName, int nMemberIndex, int nMemberServer, int nMemberServerIndex);
	int DelPartyMember(char *szLeaderName, char *szMemberName);

	int ConnectMember(char *szMemberName, int nMemberIndex, int nChannelNumber, int nChannelIndex);
	int DisconnectMember(char *szMemberName);

private:
	CRITICAL_SECTION							m_criti;
	std::vector<PARTYMATCHING_PARTYDATA>		m_vtPartyData;
	CQuery										m_DBQuery;
};

#define MAX_GENS_CLASS				14
#define GENS_REWARD_UNAVAILABLE		0
#define GENS_REWARD_WAITING			1
#define GENS_REWARD_ALREADY_GIVEN	2

class GensSystem_EDS
{
public:

	GensSystem_EDS();
	~GensSystem_EDS();

	BOOL InitGensSystem();
	void GensRankingProcess();
	void ManualRefreshRanking(BYTE Type);
	BOOL LoadGensData(LPSTR lpszFile);
	int GetGensInfluence(char *szUserName);

	void GDReqRegGensMember(int aIndex, _tagPMSG_REQ_REG_GENS_MEMBER_EXDB *aRecv);
	void GDReqSecedeGensMember(int aIndex, _tagPMSG_REQ_SECEDE_GENS_MEMBER_EXDB *aRecv);
	void GDReqAbusingInfo(int aIndex, _tagPMSG_REQ_ABUSING_INFO *aRecv);
	void GDReqGensInfo(int aIndex, _tagPMSG_REQ_ABUSING_INFO *aRecv);
	void GDReqSaveContributePoint(int aIndex, _tagPMSG_REQ_SAVE_CONTRIBUTE_POINT_EXDB *aRecv);
	void GDReqSaveAbusingKillUserName(int aIndex, _tagPMSG_REQ_SAVE_ABUSING_KILLUSER_EXDB *aRecv);
	void GDReqGensRewardCheck(int aIndex, _tagPMSG_REQ_GENS_REWARD_CHECK_EXDB *aRecv);
	void GDReqGensRewardComplete(int aIndex, _tagPMSG_REQ_GENS_REWARD_COMPLETE_EXDB *aRecv);
	void GDReqGensMemberCount(int aIndex, _tagPMSG_REQ_GENS_MEMBER_COUNT *aRecv);
	void GDReqSetGensRewardDay(int aIndex, _tagPMSG_SET_GENS_REWARD_DAY_EXDB *aRecv);
	void GDReqGensRewardDay(int aIndex, _tagPMSG_REQ_GENS_REWARD_DAY *aRecv);

private:

	int CalcGensClass(int ContributePoint, int Ranking);
	void MakeRanking();
	void ReloadCurrentMonth();

	std::map<string, GENS_USER, strCmp> m_mapGensRankDuprian;
	std::map<string, GENS_USER, strCmp> m_mapGensRankVanert;

	_tagGENS_RANKING m_GensRankingData[14];

	CRITICAL_SECTION m_GensCriticalSection;
	DWORD m_GensRankingUpdateTimeTick;
	DWORD m_RewardMonth;
	CQuery * m_MainDB;
	CQuery * m_RankingDB_1;
	CQuery * m_RankingDB_2;
	CLogToFile m_RankingLog;
	int m_DuprianUserCount;
	int m_VanertUserCount;
	int m_GensRankingMonth;
	int m_LeaveDateDayDiff;
	int m_GensRewardStartDay;
	int m_GensRewardEndDay;
	int m_MinRankToGainReward;
	TCHAR g_GensRankingPath[96];
};

class CExDataServerProtocol
{
public:

	CExDataServerProtocol();
	~CExDataServerProtocol();
	
	friend class CFriendSystem;
	static void ProtocolCore(int aIndex, BYTE HeadCode, LPBYTE aRecv, int iSize);
	BOOL Init();
	BOOL DBConnect();
	
	void GensManualRefreshRanking(BYTE Type);
	void GensRankingProcess();

	void GuildMatchingSendAcceptAndRejectInfo(int aIndex, char* szUserName, int nUserIndex, int nServerIndex);

private:

	void ExDataServerLogin(int aIndex, SDHP_SERVERINFO * lpMsg);	
	void GDCharCloseRecv(int aIndex, SDHP_USERCLOSE * aRecv);
	void GDGuildCreateSend(int aIndex, SDHP_GUILDCREATE * aRecv);
	void GDGuildDestroyRecv(int aIndex, SDHP_GUILDDESTROY * aRecv);
	void GDGuildMemberAdd(int aIndex, SDHP_GUILDMEMBERADD * aRecv);
	void GDGuildMemberAddWithoutUserIndex(int aIndex, SDHP_GUILDMEMBERADD_WITHOUT_USERINDEX * aRecv);
	void GDGuildMemberDel(int aIndex, SDHP_GUILDMEMBERDEL * aRecv);
	void DGGuildMemberInfoRequest(int aIndex, SDHP_GUILDMEMBER_INFO_REQUEST * aRecv);
	void DGGuildScoreUpdate(int aIndex, SDHP_GUILDSCOREUPDATE * aRecv);
	void GDGuildNoticeSave(int aIndex, SDHP_GUILDNOTICE * aRecv);
	void GDGuildReqAssignStatus(int aIndex, EXSDHP_GUILD_ASSIGN_STATUS_REQ * aRecv);
	void GDGuildReqAssignType(int aIndex, EXSDHP_GUILD_ASSIGN_TYPE_REQ * aRecv);
	void GDGuildServerGroupChattingSend(int aIndex,EXSDHP_SERVERGROUP_GUILD_CHATTING_RECV * aRecv);
	void GDUnionServerGroupChattingSend(int aIndex,EXSDHP_SERVERGROUP_UNION_CHATTING_RECV * aRecv);
	void GDGensServerGroupChattingSend(int aIndex,EXSDHP_SERVERGROUP_GENS_CHATTING_RECV * aRecv);
	void DGRelationShipAnsJoin(int aIndex, EXSDHP_RELATIONSHIP_JOIN_REQ * aRecv);
	void DGRelationShipAnsBreakOff(int aIndex, EXSDHP_RELATIONSHIP_BREAKOFF_REQ * aRecv);
	void DGUnionListRecv(int aIndex, EXSDHP_UNION_LIST_REQ * aRecv);
	void DGRelationShipAnsKickOutUnionMember(int aIndex, EXSDHP_KICKOUT_UNIONMEMBER_REQ *aRecv);
	void DGGuildMasterListSend(char *szGuild);
	void DGGuildInfoRequest(int aIndex, SDHP_GUILDMEMBER_INFO_GUILDNAME_REQUEST *aRecv);
	void DGRelationShipListSend(int aIndex, int iGuild, int relation_type, BOOL snd_all);
	void DGRelationShipNotificationSend(int iGuild, int iUpdateFlag);
	void FCHRoomCreateReq(int aIndex, char *szName, char *szFriendName, short Number, short ServerId, short FriendNumber, short FriendServerId);
	void FriendChatRoomCreateReq(int aIndex, FHP_FRIEND_CHATROOM_CREATE_REQ* lpMsg);
	void FCHChatRoomCreateReq(int aIndex, char *szName, char *szFriendName, short Number, short ServerId, short FriendNumber, short FriendServerId);
	void FriendChatRoomCreateAns(int aIndex, FCHP_CHATROOM_CREATE_RESULT* lpMsg);
	void FriendChatRoomInvitationReq(int aIndex, FHP_FRIEND_INVITATION_REQ * lpMsg);
	void FCHChatRoomInvitationReq(int aIndex, short RoomNumber,	char *szName, short Number,	short ServerId,	BYTE Type);
	void GDReqGuildPeriodBuffInsert(int aIndex, PMSG_REQ_GUILD_PERIODBUFF_INSERT *aRecv);
	void GDReqGuildPeriodBuffDelete(int aIndex, PMSG_REQ_GUILD_PERIODBUFF_DELETE *aRecv);

	void CloseExDataServer();

	LPGUILD_INFO_STRUCT GetGuild(char *szGuild);
	LPGUILD_INFO_STRUCT GetGuild(int iNumber);
	LPGUILD_MEMBER GetGuildMember(char *szGuild, char *szName);
	LPGUILD_MEMBER GetGuildMember(int nGuildNumber, char *szName);
	LPUNION_DATA GetUnionData(int iGuild);

	int GetGuildMemberCount(char *szGuild);
	int GetGuildMemberCount(int nGuildNumber);

	int CreateDBGuild(char* szGuild, char* szMaster, LPBYTE pMark);
	int GetDBGuildNumber(char *szGuild);

	BOOL GuildExists(char *szGuild);
	BOOL AddGuild(char *szGuild, char *szMaster, int iNumber, char *lpNotice, int iScore, LPBYTE pMark, int iType);
	BOOL DelGuild(char *szGuild);
	BOOL MasterDBExists(char *szMaster);
	BOOL DelDBGuild(char *szGuild);

	BOOL GuildMemberExists(char *szGuild, char *szName);
	BOOL AddGuildMember(char *szGuild, char *szName, BYTE btStatus = 0, BYTE btConnected = -1);
	BOOL DelAllDBGuildMember(char *szGuild);
	BOOL DBGuildMemberExists(char *szName);
	BOOL AddDBGuildMember(char *szGuild, char *szName);
	BOOL DelDBGuildMember(char *szName);
	BOOL DelGuildMember(char *szGuild, char *szName);

	BOOL UpdateGuildMemberStatus(char *szGuild, char *szName, BYTE btStatus);
	BOOL UpdateGuildType(char *szGuild, BYTE btType);

	void SendGuildMemberInfo(int aIndex, char *szName, int iUserIndex);
	BOOL GetGuildMemberInfo(char *szName, OUT char *szGuild, OUT int& riLevel, OUT int& riStatus);

	BOOL IsCSGuild(char *szGuild);

	BOOL UpdateScore(char *szGuild, int iScore);
	BOOL SaveNotice(char *szGuild, char *szNotice);

	BOOL CreateRelationShipData(int iGuild);
	BOOL DestroyRelationShipData(int iGuild);

	int RelationShipOperation(int iReqGuild, int iTargGuild, int relation_type, int operation);

	int RelationShipJoin(LPGUILD_INFO_STRUCT lpReqGuild, LPGUILD_INFO_STRUCT lpTargGuild, int type);
	int UnionJoin(LPGUILD_INFO_STRUCT lpReqGuild, LPGUILD_INFO_STRUCT lpTargGuild);
	int RivalJoin(LPGUILD_INFO_STRUCT lpReqGuild, LPGUILD_INFO_STRUCT lpTargGuild);

	int RelationShipBreak(LPGUILD_INFO_STRUCT lpMasterGuild, LPGUILD_INFO_STRUCT lpKickGuild, int type);
	int UnionBreak(LPGUILD_INFO_STRUCT lpMasterGuild, LPGUILD_INFO_STRUCT lpKickGuild);
	int RivalBreak(LPGUILD_INFO_STRUCT lpMasterGuild, LPGUILD_INFO_STRUCT lpKickGuild);

	BOOL AddUnion(LPGUILD_INFO_STRUCT lpReqGuild, LPGUILD_INFO_STRUCT lpTargGuild);
	BOOL AddUnion(int iReqGuild, int iTargGuild);
	BOOL KickUnion(LPGUILD_INFO_STRUCT lpMasterGuild, LPGUILD_INFO_STRUCT lpKickGuild);

	BOOL UpdateDBUnion(int iGuild, int iUnion);
	BOOL UpdateDBRival(int iGuild, int iRival);

	void RelationShipBreakSend(int iGuild, int iType);
	void SendListToAllRivals(LPGUILD_INFO_STRUCT lpGuild);

	int GetChatServer();

	void GDReqGuildMatchingList(int aIndex, _stReqGuildMatchingList * lpMsg);
	void GDReqGuildMatchingListSearchWord(int aIndex, _stReqGuildMatchingListSearchWord * lpMsg);
	void GDReqRegGuildMatchingList(int aIndex, _stReqGuildMatchingData * lpMsg);
	void GDReqDelMatchingList(int aIndex, _stReqDelGuildMatchingList * lpMsg);
	void GDReqJoinGuildMatchingList(int aIndex, _stRegWaitGuildMatching * lpMsg);
	void GDReqCancelJoinGuildMatching(int aIndex, _stReqDelWaitGuildMatchingList * lpMsg);
	void GDReqAllowJoinGuildMatching(int aIndex, _stReqAllowJoinGuildMatching * lpMsg);
	void GDReqWaitGuildMatchingList(int aIndex, _stReqWaitGuildMatchingList * lpMsg);
	void GDReqGetWaitStateListGuildMatching(int aIndex, _stReqWaitStateListGuildMatching * lpMsg);

	void SendNotiGuildMatchingForGuildMaster(int nGuildNumber);
	void SendUseGuildMatchingGuild(char *szName, int nGuildNumber);
	void UpdateGuildMatchingMemberCount(char *szGuildName, int nGuildMemberCnt);

	void GDReqRegWantedPartyMember(int aIndex, _stReqRegWantedPartyMember * lpMsg);
	void GDReqGetPartyMatchingList(int aIndex, _stReqGetPartyMatchingList * lpMsg);
	void GDReqJoinMemberPartyMatching(int aIndex, _stReqJoinMemberPartyMatching * lpMsg);
	void GDReqJoinMemberStateList(int aIndex, _stReqJoinMemberStateListPartyMatching *lpMsg);
	void GDReqJoinMemberStateListLeader(int aIndex, _stReqWaitListPartyMatching *lpMsg);
	void GDReqAcceptMemberJoin(int aIndex, _stReqAddPartyMember *lpMsg);
	void GDReqCancelPartyMatching(int aIndex, _stReqCancelPartyMatching *lpMsg);
	void GDReqDeletePartyUser(int aIndex, _stReqDelPartyUserPartyMatching *lpMsg);
	void GDReqSendPartyMemberList(int aIndex, _stReqSendPartyMemberList * lpMsg);
	void GDSendChatMsgPartyMatching(int aIndex, _stReqChattingPartyMatching * lpMsg);

	void FixAddPartyMember(int nLeaderIndex, int nMemberIndex, int nServerIndex);
	void SendPartyMatchingMemberList(char *szLeaderName, BYTE btType);
	void SendPartyMatchingOneList(char *szMemberName);
	void AutoAddPartyMember(char *szLeaderName, char *szMemberName, int nUserIndex, int nServerIndex);

	std::map<std::string, tagGUILD_INFO_STRUCT,strCmp> m_MapGuildManager;
	std::map<int, tagUNION_DATA> m_MapUnionManager;

	GensSystem_EDS				m_GensSystemEDS;
	CFriendSystem				m_FriendSystemEDS;
	CPartyMatchingData			m_PartyMatchingEDS;
	CQuery m_GuildDB;
	CQuery m_UnionDB;
	CQuery m_GMatchingDB;
	CQuery m_PMatchingDB;
};

#endif // !defined(AFX_EDSPROTOCOL_H__675D1E8D_B9AC_45E3_8077_17EC82A9A5A5__INCLUDED_)



//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

