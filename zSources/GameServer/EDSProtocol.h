//////////////////////////////////////////////////////////////////////
// EDSProtocol.h
#ifndef EDSPROTOCOL_H
#define EDSPROTOCOL_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "prodef.h"
#include "protocol.h"

struct SDHP_USERCLOSE
{
	PBMSG_HEAD h;	//	
	char CharName[10];	//	3
	char GuildName[8];	//	D
	unsigned char Type;	//	15
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



struct SDHP_GUILDMEMBER_INFO
{
	PBMSG_HEAD h;	//	
	char GuildName[9];	//	3
	char MemberID[11];	//	C
	unsigned char btGuildStatus;	//	17
	unsigned char btGuildType;	//	18
	short pServer;	//	1A
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
	char szGuildNotice[90];	//	C
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

struct EXSDHP_SERVERGROUP_GENS_CHATTING_RECV
{
	PBMSG_HEAD h;	//	
	int iGensNum;	//	4
	char szCharacterName[10];	//	8
	char szChattingMsg[90];	//	12
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



struct EXSDHP_GUILD_ASSIGN_TYPE_RESULT
{
	PBMSG_HEAD h;	//	
	unsigned char btFlag;	//	3
	unsigned short wUserIndex;	//	4
	unsigned char btGuildType;	//	6
	unsigned char btResult;	//	7
	char szGuildName[9];	//	8
};



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


struct FHP_WAITFRIENDLIST_COUNT
{
	PBMSG_HEAD h;	//	
	short Number;	//	4
	char Name[10];	//	6
	char FriendName[10];	//	10
};

struct PMSG_FRIEND_ADD_REQ
{
	PBMSG_HEAD h;
	char Name[10];	// 3
};


struct PMSG_FRIEND_STATE_C
{
	PBMSG_HEAD h;	//	
	unsigned char ChatVeto;	//	3
};



struct FHP_FRIEND_STATE
{
	PBMSG_HEAD h;	//	
	short Number;	//	4
	char Name[10];	//	6
	char FriendName[10];	//	10
	unsigned char State;	//	1A
};

struct FHP_FRIEND_ADD_RESULT
{
	PBMSG_HEAD h;
	short Number;	// 4
	unsigned char Result;	// 6
	char Name[10];	// 7
	char FriendName[10];	// 11
	unsigned char Server;	// 1B
};

struct PMSG_FRIEND_ADD_SIN_RESULT
{
	PBMSG_HEAD h;	//	
	unsigned char Result;	//	3
	char Name[10];	//	4
};



struct FHP_WAITFRIEND_ADD_RESULT
{
	PBMSG_HEAD h;	//	
	short Number;	//	4
	unsigned char Result;	//	6
	char Name[10];	//	7
	char FriendName[10];	//	11
	unsigned char pServer;	//	1B
};



struct PMSG_FRIEND_DEL_REQ
{
	PBMSG_HEAD h;	//	
	char Name[10];	//	3
};



struct FHP_FRIEND_DEL_RESULT
{
	PBMSG_HEAD h;	//	
	short Number;	//	4
	unsigned char Result;	//	6
	char Name[10];	//	7
	char FriendName[10];	//	11
};



struct PMSG_FRIEND_ROOMCREATE_REQ
{
	PBMSG_HEAD h;	//	
	char Name[10];	//	3
};



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


#pragma pack (1)

struct PMSG_FRIEND_MEMO
{
	PWMSG_HEAD h;	//	
	unsigned long WindowGuid;	//	4
	char Name[10];	//	8
	char Subject[60];	//	12
	unsigned char Dir;	//	32
	unsigned char Action;	//	33
	short MemoSize;	//	34
	char Memo[1000];	//	36
};

#pragma pack ()



struct PMSG_JG_MEMO_SEND
{
	PWMSG_HEAD h;
	char Name[10];	// 4
	char TargetName[10];	// E
	char Subject[60];	// 18
	short MemoSize;	// 38
	char Memo[1000];	// 3A
};



struct FHP_FRIEND_MEMO_SEND_RESULT
{
	PBMSG_HEAD h;	//	
	short Number;	//	4
	char Name[10];	//	6
	unsigned char Result;	//	10
	unsigned long WindowGuid;	//	14
};



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




struct PMSG_FRIEND_READ_MEMO_REQ
{
	PBMSG_HEAD h;	//	
	unsigned short MemoIndex;	//	4
};



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



struct PMSG_FRIEND_MEMO_DEL_REQ
{
	PBMSG_HEAD h;	//	
	unsigned short MemoIndex;	//	4
};



struct FHP_FRIEND_MEMO_DEL_RESULT
{
	PBMSG_HEAD h;	//	
	unsigned char Result;	//	3
	unsigned short MemoIndex;	//	4
	short Number;	//	6
	char Name[10];	//	8
};


struct PMSG_ROOM_INVITATION
{
	PBMSG_HEAD h;	//	
	char Name[10];	//	3
	unsigned short RoomNumber;	//	E
	unsigned long WindowGuid;	//	10
};



struct FHP_FRIEND_INVITATION_RET
{
	PBMSG_HEAD h;
	unsigned char Result;	// 3
	short Number;	// 4
	char Name[10];	// 6
	DWORD WindowGuid;	// 10
};

// Season 8 Ep3
// Guild Matching

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

void ExDataServerProtocolCore(BYTE protoNum, LPBYTE aRecv, int aLen);
void ExDataServerLogin();
void GDCharClose(int type, LPSTR GuildName, LPSTR Name);
void ExDataServerLoginResult( SDHP_RESULT* lpMsg);
void GDCharCloseRecv( SDHP_USERCLOSE* lpMsg);
void GDGuildCreateSend(int aIndex, LPSTR Name, LPSTR Master, LPBYTE Mark, int iGuildType);
void GDGuildCreateRecv( SDHP_GUILDCREATE_RESULT* lpMsg);
void GDGuildDestroySend(int aIndex, LPSTR Name, LPSTR Master);
void GDGuildDestroyRecv( SDHP_GUILDDESTROY_RESULT* lpMsg);
void GDGuildMemberAdd(int aIndex, LPSTR Name, LPSTR MemberId);
void GDGuildMemberAddWithoutUserIndex(LPSTR Name, LPSTR MemberId);
void GDGuildMemberAddResult( SDHP_GUILDMEMBERADD_RESULT* lpMsg);
void GDGuildMemberAddResultWithoutUserIndex(SDHP_GUILDMEMBERADD_RESULT_WITHOUT_USERINDEX * lpMsg);
void GDGuildMemberDel(int aIndex, LPSTR Name, LPSTR MemberId);
void GDGuildMemberDelResult( SDHP_GUILDMEMBERDEL_RESULT* lpMsg);
void GDGuildUpdate(LPSTR Name, LPSTR Master, LPBYTE Mark,  int score, BYTE count);
void DGGuildUpdateRecv( SDHP_GUILDUPDATE* lpMsg);
void DGGuildMemberInfoRequest(int aIndex);
void DGGuildMemberInfo( SDHP_GUILDMEMBER_INFO* lpMsg);
void GDGuildInfoSendRequest(char *szGuildName);
void DGGuildMasterListRecv(LPBYTE lpData);
void DGGuildScoreUpdate(LPSTR guildname, int score);
void GDGuildScoreUpdateRecv( SDHP_GUILDSCOREUPDATE* lpMsg);
void GDGuildNoticeSave(LPSTR guild_name, LPSTR guild_notice);
void DGGuildNoticeRecv( SDHP_GUILDNOTICE* lpMsg);
void DGGuildListRequest();
void DGGuildList( SDHP_GUILDCREATED* lpMsg);
int __stdcall GuildListDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
void DGGuildListState( SDHP_GUILDLISTSTATE* lpMsg);
void GDGuildServerGroupChattingSend(int iGuildNum,  PMSG_CHATDATA* lpChatData);
void DGGuildServerGroupChattingRecv( EXSDHP_SERVERGROUP_GUILD_CHATTING_RECV* lpMsg);
void GDUnionServerGroupChattingSend(int iUnionNum,  PMSG_CHATDATA* lpChatData);
void DGUnionServerGroupChattingRecv( EXSDHP_SERVERGROUP_UNION_CHATTING_RECV* lpMsg);
void GDGensServerGroupChattingSend(int iGensInfluence, PMSG_CHATDATA* lpChatData);
void DGGensServerGroupChattingRecv(EXSDHP_SERVERGROUP_GENS_CHATTING_RECV *lpMsg);
void GDGuildReqAssignStatus(int aIndex, int iAssignType, LPSTR szTagetName, int iGuildStatus);
void DGGuildAnsAssignStatus( EXSDHP_GUILD_ASSIGN_STATUS_RESULT* lpMsg);
void GDGuildReqAssignType(int aIndex, int iGuildType);
void DGGuildAnsAssignType( EXSDHP_GUILD_ASSIGN_TYPE_RESULT* lpMsg);
void GDRelationShipReqJoin(int aIndex, int iTargetUserIndex, int iRelationShipType);
void DGRelationShipAnsJoin( EXSDHP_RELATIONSHIP_JOIN_RESULT* lpMsg);
void GDUnionBreakOff(int aIndex, int iUnionNumber);
void DGUnionBreakOff( EXSDHP_RELATIONSHIP_BREAKOFF_RESULT* lpMsg);
void GDRelationShipReqBreakOff(int aIndex, int iTargetUserIndex, int iRelationShipType);
void DGRelationShipAnsBreakOff( EXSDHP_RELATIONSHIP_BREAKOFF_RESULT* lpMsg);
void DGRelationShipNotificationRecv( EXSDHP_NOTIFICATION_RELATIONSHIP* lpMsg);
void DGRelationShipListRecv( EXSDHP_UNION_RELATIONSHIP_LIST* lpMsg);
void GDUnionListSend(int iUserIndex, int iUnionMasterGuildNumber);
void DGUnionListRecv(LPBYTE aRecv);
void GDRelationShipReqKickOutUnionMember(int aIndex, LPSTR szTargetGuildName);
void DGRelationShipAnsKickOutUnionMember( EXSDHP_KICKOUT_UNIONMEMBER_RESULT* lpMsg);
void FriendListRequest(int aIndex);
void FriendListResult(LPBYTE lpMsg);
void WaitFriendListResult( FHP_WAITFRIENDLIST_COUNT* lpMsg);
void FriendStateClientRecv( PMSG_FRIEND_STATE_C* lpMsg, int aIndex);
void FriendStateRecv( FHP_FRIEND_STATE* lpMsg);
void FriendAddRequest( PMSG_FRIEND_ADD_REQ * lpMsg, int aIndex);
void FriendAddResult( FHP_FRIEND_ADD_RESULT* lpMsg);
void WaitFriendAddRequest( PMSG_FRIEND_ADD_SIN_RESULT* lpMsg, int aIndex);
void WaitFriendAddResult( FHP_WAITFRIEND_ADD_RESULT* lpMsg);
void FriendDelRequest( PMSG_FRIEND_DEL_REQ* lpMsg, int aIndex);
void FriendDelResult( FHP_FRIEND_DEL_RESULT* lpMsg);
void FriendChatRoomCreateReq( PMSG_FRIEND_ROOMCREATE_REQ* lpMsg, int aIndex);
void FriendChatRoomCreateResult( FHP_FRIEND_CHATROOM_CREATE_RESULT* lpMsg);
void FriendMemoSend( PMSG_FRIEND_MEMO* lpMsg, int aIndex);
void MngFriendMemoSend( PMSG_JG_MEMO_SEND* lpMsg);
BOOL WithdrawUserMoney(LPSTR Type, struct OBJECTSTRUCT* lpObj, int Withdraw_Money);
void FriendMemoSendResult( FHP_FRIEND_MEMO_SEND_RESULT* lpMsg);
void FriendMemoListReq(int aIndex);
void FriendMemoList( FHP_FRIEND_MEMO_LIST* lpMsg);
void FriendMemoReadReq( PMSG_FRIEND_READ_MEMO_REQ* lpMsg, int aIndex);
void FriendMemoRead( FHP_FRIEND_MEMO_RECV* lpMsg);
void FriendMemoDelReq( PMSG_FRIEND_MEMO_DEL_REQ* lpMsg, int aIndex);
void FriendMemoDelResult( FHP_FRIEND_MEMO_DEL_RESULT* lpMsg);
void FriendRoomInvitationReq( PMSG_ROOM_INVITATION* lpMsg, int aIndex);
void FriendRoomInvitationRecv( FHP_FRIEND_INVITATION_RET* lpMsg);
void DGAnsGensInfo(_tagPMSG_ANS_GENS_INFO_EXDB *lpMsg);
void DGAnsRegGensMember(_tagPMSG_ANS_REG_GENS_MEMBER_EXDB *lpMsg);
void DGAnsSecedeGensMember(_tagPMSG_ANS_SECEDE_GENS_MEMBER_EXDB *lpMsg);
void DGAnsAbusingInfo(_tagPMSG_ANS_ABUSING_INFO *lpMsg);
void DGAnsGensReward(_tagPMSG_ANS_GENS_REWARD_EXDB *lpMsg);
void DGAnsGensMemberCount(_tagPMSG_ANS_GENS_MEMBER_COUNT_EXDB *lpMsg);
void DGAnsGensRewardDayCheck(_tagPMSG_ANS_GENS_REWARD_DAY_CHECK_EXDB *lpMsg);

// Guild Matching
void GDReqGuildMatchingList(int nUserIndex, int nPage);
void GDReqGuildMatchingListSearchWord(int nUserIndex, int nPage, char *szSearchWord);
void DGAnsGuildMatchingList(_stAnsGuildMatchingList *lpMsg);
void GDReqDelMatchingList(int nUserIndex, int nGuildNum);
void DGAnsDelMatchingList(_stAnsDelGuildMatchingList *lpMsg);
void GDReqRegGuildMatchingList(int nUserIndex, _stGuildMatchingList stGuildMatchingList);
void DGAnsRegGuildMatchingList(_stAnsGuildMatchingData *lpMsg);
void GDReqJoinGuildMatchingList(int nUserIndex, _stGuildMatchingAllowListDB stAllowList);
void DGAnsJoinGuildMatching(_stAnsWaitGuildMatching *lpMsg);
void GDReqCancelJoinGuildMatching(int nUserIndex, char *szName, int nType);
void DGAnsCancelJoinGuildMatching(_stAnsDelWaitGuildMatchingList *lpMsg);
void GDReqAllowJoinGuildMatching(int nUserIndex, int nAllowType, char *szName, char *szGuildName);
void DGAnsAllowJoinGuildMatching(_stAnsAllowJoinGuildMatching *lpMsg);
void GDReqWaitGuildMatchingList(int nGuildNumber, int nUserIndex);
void DGAnsWaitGuildMatchingList(_stAnsWaitGuildMatchingList *lpMsg);
void GDReqGetWaitStateListGuildMatching(int nUserIndex, char* szApplicantName);
void DGAnsGetWaitStateListGuildMatching(_stAnsWaitStateListGuildMatching *lpMsg);
void DGAnsNotiGuildMatching(_stAnsNotiGuildMatching *lpMsg);
void DGAnsNotiGuildMatchingForGuildMaster(_stAnsNotiGuildMatchingForGuildMaster *lpMsg);
void DGAnsUseGuildMatchingGuild(_stAnsUseGuildMatchingGuild *lpMsg);

// Party Matching
void GDReqRegWantedPartyMember(_PARTY_INFO_LISTDB stList, int nUserIndex);
void DGAnsRegWantedPartyMember(_stAnsRegWantedPartyMember *lpMsg);
void GDReqGetPartyMatchingList(int nPage, char *szSearchWord, int nType, int nUserIndex);
void DGAnsGetPartyMatchingList(_stAnsGetPartyMatchingList *lpMsg);
void GDReqJoinMemberPartyMatching(char *szMemberName, char *szLeaderName, char *szPassWord, int nUserIndex, int nUserDBNumber, int nLevel, BYTE btClass, BYTE btRandomParty, BYTE btGensType, BYTE btChangeUpClass);
void DGAnsJoinMemberPartyMatching(_stAnsJoinMemberPartyMatching *lpMsg);
void GDReqMemberJoinStateList(char *szMemberName, int nUserIndex);
void DGAnsMemberJoinStateList(_stAnsJoinMemberStateListPartyMatching *lpMsg);
void GDReqMemberJoinStateListLeader(char *szLeaderName, int nUserIndex);
void DGAnsMemberJoinStateListLeader(_stAnsWaitListPartyMatching *lpMsg);
void DGAnsSendPartyMemberListPartyMatching(_stAnsSendPartyMemberList *lpMsg);
void GDReqCancelPartyMatching(int nUserIndex, BYTE btType);
void DGAnsCancelPartyMatching(_stAnsCancelPartyMatching *lpMsg);
void GDReqAcceptMemberJoin(int nUserIndex, BYTE btType, char *szLeaderName, char *szMemberName, BYTE btManual);
void DGAnsAcceptMemberJoin(_stAnsAddPartyMember *lpMsg);
void DGFixAddPartyMember(_stAnsAddRealPartyMember *lpMsg);
void GDReqDeletePartyUser(int nUserIndex, char *szTargetName);
void DGAnsRequestJoinPartyMatchingNoti(_stAnsRequestJoinPartyMatchingNoti *lpMsg);
void DGAnsDeletePartyUser(_stAnsDelPartyUserPartyMatching *lpMsg);
void GDSendChatMsgPartyMatching(char *szMsg, char *szName, int nPartyMatchingIndex);
void DGRecvChatMsgPartyMatching(_stAnsChattingPartyMatching *lpMsg);
void GDReqSendPartyMemberList(char *szLeaderName);
#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

