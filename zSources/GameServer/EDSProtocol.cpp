//////////////////////////////////////////////////////////////////////
// EDSProtocol.cpp
#include "stdafx.h"
#include "EDSProtocol.h"
#include "GuildClass.h"
#include "winutil.h"
#include "TLog.h"
#include "TUnion.h"
#include "GameServer.h"
#include "GameMain.h"
#include "TNotice.h"
#include "GensSystemProtocol.h"
#include "GensSystem.h"
#include "configread.h"
#include "CastleSiegeSync.h"
#include "ArcaBattle.h"
#include "NewPVP.h"
#include "PartyClass.h"
#include "AcheronGuardianEvent.h"

// GS-N 0.99.60T 0x0044B770
// GS-N	1.00.18	JPN	0x0045A3E0	-	Completed

enum EXDB_DATA_TRANSFER_TYPE {
  EX_SEND_P2P = 0x1,
  EX_SEND_BROADCASTING = 0x0,
};

enum EXDB_RESULT {
  EX_RESULT_SUCCESS = 0x1,
  EX_RESULT_FAIL = 0x0,
  EX_RESULT_FAIL_FOR_CASTLE = 0x10,
};

int gGuildListTotal;
int gGuildListCount;
HWND GuildListhSts;
HWND GuildListhPrs;
HWND GuildListhDlg;

void ExDataServerProtocolCore(BYTE protoNum, LPBYTE aRecv, int aLen)
{
	#if(TRACE_PROTOCOL==1)
		LogAddHeadHex("EX_DATA_SERVER", aRecv, aLen);
	#endif
	
	switch ( protoNum )
	{
		case 0x00:
			ExDataServerLoginResult((SDHP_RESULT *)aRecv);
			break;

		case 0x02:
			GDCharCloseRecv((SDHP_USERCLOSE *)aRecv);
			break;

		case 0x30:
			GDGuildCreateRecv((SDHP_GUILDCREATE_RESULT *)aRecv);
			break;

		case 0x31:
			GDGuildDestroyRecv((SDHP_GUILDDESTROY_RESULT *)aRecv);
			break;

		case 0x32:
			GDGuildMemberAddResult((SDHP_GUILDMEMBERADD_RESULT *)aRecv);
			break;

		case 0x33:
			GDGuildMemberDelResult((SDHP_GUILDMEMBERDEL_RESULT *)aRecv);
			break;

		case 0x35:
			DGGuildMemberInfo((SDHP_GUILDMEMBER_INFO *)aRecv);
			break;

		case 0x36:
			DGGuildMasterListRecv(aRecv);
			break;

		case 0x37:
			GDGuildScoreUpdateRecv((SDHP_GUILDSCOREUPDATE *)aRecv);
			break;

		case 0x38:
			DGGuildNoticeRecv((SDHP_GUILDNOTICE *)aRecv);
			break;

		case 0x39:
			GDGuildMemberAddResultWithoutUserIndex((SDHP_GUILDMEMBERADD_RESULT_WITHOUT_USERINDEX *)aRecv);
			break;

		case 0x40:
			DGGuildList((SDHP_GUILDCREATED *)aRecv);
			break;
		case 0x41:
			//DGGuildListState((SDHP_GUILDLISTSTATE *)aRecv);
			break;

		case 0x50:
			DGGuildServerGroupChattingRecv((EXSDHP_SERVERGROUP_GUILD_CHATTING_RECV *)aRecv);
			break;

		case 0x51:
			DGUnionServerGroupChattingRecv((EXSDHP_SERVERGROUP_UNION_CHATTING_RECV *)aRecv);
			break;

		case 0x52:
			DGGensServerGroupChattingRecv((EXSDHP_SERVERGROUP_GENS_CHATTING_RECV *)aRecv);
			break;

		///////////////////////////////////////////
		case 0xE1:
			DGGuildAnsAssignStatus((EXSDHP_GUILD_ASSIGN_STATUS_RESULT *)aRecv);
			break;

		case 0xE2:
			DGGuildAnsAssignType((EXSDHP_GUILD_ASSIGN_TYPE_RESULT *)aRecv);
			break;

		case 0xE5:
			DGRelationShipAnsJoin((EXSDHP_RELATIONSHIP_JOIN_RESULT *)aRecv);
			break;

		case 0xE6:
			DGRelationShipAnsBreakOff((EXSDHP_RELATIONSHIP_BREAKOFF_RESULT *)aRecv);
			break;

		case 0xE7:
			DGRelationShipListRecv((EXSDHP_UNION_RELATIONSHIP_LIST *)aRecv);
			break;

		case 0xE8:
			DGRelationShipNotificationRecv((EXSDHP_NOTIFICATION_RELATIONSHIP *)aRecv);
			break;

		case 0xE9:
			DGUnionListRecv(aRecv);
			break;

		case 0xEB:
			{
				PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;

				switch ( lpDef->subcode )
				{
					case 0x01:
						DGRelationShipAnsKickOutUnionMember((EXSDHP_KICKOUT_UNIONMEMBER_RESULT *)aRecv);
						break;
				}
			}
			break;
		//////////////////////////////////////////////////////////
		case 0x60:
			FriendListResult(aRecv);
			break;

		case 0x61:
			WaitFriendListResult((FHP_WAITFRIENDLIST_COUNT *)aRecv);
			break;

		case 0x62:
			FriendStateRecv((FHP_FRIEND_STATE *)aRecv);
			break;

		case 0x63:
			FriendAddResult((FHP_FRIEND_ADD_RESULT *)aRecv);
			break;

		case 0x64:
			WaitFriendAddResult((FHP_WAITFRIEND_ADD_RESULT *)aRecv);
			break;

		case 0x65:
			FriendDelResult((FHP_FRIEND_DEL_RESULT *)aRecv);
			break;

		case 0x66:
			FriendChatRoomCreateResult((FHP_FRIEND_CHATROOM_CREATE_RESULT *)aRecv);
			break;

		case 0x70:
			FriendMemoSendResult((FHP_FRIEND_MEMO_SEND_RESULT *)aRecv);
			break;

		case 0x71:
			FriendMemoList((FHP_FRIEND_MEMO_LIST *)aRecv);
			break;

		case 0x72:
			FriendMemoRead((FHP_FRIEND_MEMO_RECV *)aRecv);
			break;

		case 0x73:
			FriendMemoDelResult((FHP_FRIEND_MEMO_DEL_RESULT *)aRecv);
			break;

		case 0x74:
			FriendRoomInvitationRecv((FHP_FRIEND_INVITATION_RET *)aRecv);
			break;

		case 0xA3:
			{
				switch (aRecv[3])
				{
					case 0x01:
						return;
					case 0x02:
						DGAnsRegGuildMatchingList((_stAnsGuildMatchingData *)aRecv);
						break;
					case 0x03:
						DGAnsDelMatchingList((_stAnsDelGuildMatchingList *)aRecv);
						break;
					case 0x04:
						DGAnsJoinGuildMatching((_stAnsWaitGuildMatching *)aRecv);
						break;
					case 0x05:
						DGAnsCancelJoinGuildMatching((_stAnsDelWaitGuildMatchingList *)aRecv);
						break;
					case 0x06:
						DGAnsAllowJoinGuildMatching((_stAnsAllowJoinGuildMatching *)aRecv);
						break;
					case 0x08:
						DGAnsGetWaitStateListGuildMatching((_stAnsWaitStateListGuildMatching *)aRecv);
						break;
					case 0x09:
						DGAnsNotiGuildMatching((_stAnsNotiGuildMatching *)aRecv);
						break;
					case 0x10:
						DGAnsNotiGuildMatchingForGuildMaster((_stAnsNotiGuildMatchingForGuildMaster *)aRecv);
						break;
					case 0x11:
						DGAnsUseGuildMatchingGuild((_stAnsUseGuildMatchingGuild *)aRecv);
						break;
					default:
						if (aRecv[4])
						{
							if (aRecv[4] == 0x07)
							{
								DGAnsWaitGuildMatchingList((_stAnsWaitGuildMatchingList *)aRecv);
							}
						}

						else
						{
							DGAnsGuildMatchingList((_stAnsGuildMatchingList *)aRecv);
						}
						break;
				}
			}
			break;

		case 0xA4:
			{
				switch (aRecv[3])
				{
					case 0x00:
						DGAnsRegWantedPartyMember((_stAnsRegWantedPartyMember *)aRecv);
						break;
					case 0x02:
						DGAnsJoinMemberPartyMatching((_stAnsJoinMemberPartyMatching *)aRecv);
						break;
					case 0x03:
						DGAnsMemberJoinStateList((_stAnsJoinMemberStateListPartyMatching *)aRecv);
						break;
					case 0x05:
						DGAnsAcceptMemberJoin((_stAnsAddPartyMember *)aRecv);
						break;
					case 0x06:
						DGAnsCancelPartyMatching((_stAnsCancelPartyMatching *)aRecv);
						break;
					case 0x07:
						DGAnsDeletePartyUser((_stAnsDelPartyUserPartyMatching *)aRecv);
						break;
					case 0x08:
						DGAnsRequestJoinPartyMatchingNoti((_stAnsRequestJoinPartyMatchingNoti *)aRecv);
						break;
					case 0x10:
						DGFixAddPartyMember((_stAnsAddRealPartyMember *)aRecv);
						break;
					case 0x11:
						DGAnsSendPartyMemberListPartyMatching((_stAnsSendPartyMemberList *)aRecv);
						break;
					case 0x12:
						DGRecvChatMsgPartyMatching((_stAnsChattingPartyMatching *)aRecv);
						break;
					default:
						if (aRecv[4] == 0x01)
						{
							DGAnsGetPartyMatchingList((_stAnsGetPartyMatchingList *)aRecv);
						}
						else if (aRecv[4] == 0x04)
						{
							DGAnsMemberJoinStateListLeader((_stAnsWaitListPartyMatching *)aRecv);
						}
						break;
				}
			}
			break;

		case 0xF8:
			{
				PMSG_DEFAULT2 * lpMsg = (PMSG_DEFAULT2 *)aRecv;

				switch ( lpMsg->subcode )
				{
					case 0x02:
						DGAnsGensInfo((_tagPMSG_ANS_GENS_INFO_EXDB *)aRecv);
						break;
					case 0x04:
						DGAnsRegGensMember((_tagPMSG_ANS_REG_GENS_MEMBER_EXDB *)aRecv);
						break;
					case 0x06:
						DGAnsSecedeGensMember((_tagPMSG_ANS_SECEDE_GENS_MEMBER_EXDB *)aRecv);
						break;
					case 0x0A:
						DGAnsAbusingInfo((_tagPMSG_ANS_ABUSING_INFO *)aRecv);
						break;
					case 0x0D:
						DGAnsGensReward((_tagPMSG_ANS_GENS_REWARD_EXDB *)aRecv);
						break;
					case 0x10:
						DGAnsGensMemberCount((_tagPMSG_ANS_GENS_MEMBER_COUNT_EXDB *)aRecv);
						break;
					case 0x14:
						DGAnsGensRewardDayCheck((_tagPMSG_ANS_GENS_REWARD_DAY_CHECK_EXDB *)aRecv);
						break;
				}
			}
			break;
	}
}



	
void ExDataServerLogin()
{
	SDHP_SERVERINFO pInfo;

	pInfo.h.c = 0xC1;
	pInfo.h.size = sizeof(SDHP_SERVERINFO);
	pInfo.h.headcode = 0x00;
	pInfo.Port = g_ConfigRead.server.GetGameServerPort();
	pInfo.Type = 0x01;
	pInfo.ServerCode = g_ConfigRead.server.GetGameServerCode();
	strcpy(pInfo.ServerName, g_ConfigRead.server.GetServerName());

	wsExDbCli.DataSend((char*)&pInfo, pInfo.h.size);
}


void GDCharClose(int type, char* GuildName, char* Name)
{
	SDHP_USERCLOSE pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0x02;
	pMsg.Type = type;
	memcpy(pMsg.CharName, Name, sizeof(pMsg.CharName));
	memset(pMsg.GuildName, 0, sizeof(pMsg.GuildName));

	if ( GuildName != NULL )
	{
		memcpy(pMsg.GuildName, GuildName, sizeof(pMsg.GuildName));
	}

	wsExDbCli.DataSend((char*)&pMsg, pMsg.h.size);
}



void ExDataServerLoginResult(SDHP_RESULT * lpMsg)
{
	if ( lpMsg->Result == false )
	{
		g_Log.Add("ExDB Server Login Fail: %d", lpMsg->Result);

		return;
	}

	for (int i=0;i<g_ConfigRead.server.GetObjectMax();i++)
	{
		if ( gObjIsConnectedGP(i) == TRUE )
		{
			if ( gObj[i].m_FriendServerOnline == FRIEND_SERVER_STATE_OFFLINE )
			{
				gObj[i].m_FriendServerOnline = FALSE;
			}
		}
	}
	
}

void GDCharCloseRecv(SDHP_USERCLOSE * lpMsg )
{
	char szName[MAX_ACCOUNT_LEN+1];
	char szGuildName[MAX_GUILD_LEN+1];

	memset(szName, 0, sizeof(szName));
	memset(szGuildName, 0, sizeof(szGuildName));
	memcpy(szName, lpMsg->CharName, sizeof(lpMsg->CharName));
	memcpy(szGuildName, lpMsg->GuildName, sizeof(lpMsg->GuildName));

	Guild.CloseMember(szGuildName, szName);
}


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

void GDGuildCreateSend(int aIndex, char* Name, char* Master,unsigned char* Mark, int iGuildType)
{
	SDHP_GUILDCREATE pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0x30;
	pMsg.h.size = sizeof(pMsg);

	memcpy(pMsg.Mark, Mark, sizeof(pMsg.Mark));
	pMsg.Master[MAX_ACCOUNT_LEN] = 0;
	pMsg.GuildName[MAX_GUILD_LEN] = 0;
	memcpy(pMsg.Master, Master, MAX_ACCOUNT_LEN);
	memcpy(pMsg.GuildName, Name, MAX_GUILD_LEN);
	pMsg.NumberH = SET_NUMBERH(aIndex);
	pMsg.NumberL = SET_NUMBERL(aIndex);
	pMsg.btGuildType = iGuildType;

	wsExDbCli.DataSend((char*)&pMsg, pMsg.h.size);
}



void GDGuildCreateRecv(SDHP_GUILDCREATE_RESULT * lpMsg)
{
	int aIndex = -1;
	PMSG_GUILDCREATED_RESULT pMsg;
	_GUILD_INFO_STRUCT * lpNode;
	char szMaster[MAX_ACCOUNT_LEN+1];
	char szGuildName[MAX_GUILD_LEN+1];

	szMaster[MAX_ACCOUNT_LEN] = 0;
	szGuildName[MAX_GUILD_LEN] = 0;
	memcpy(szMaster, lpMsg->Master, MAX_ACCOUNT_LEN);
	memcpy(szGuildName, lpMsg->GuildName, MAX_GUILD_LEN);

	if (lpMsg->Result == 0 )
	{
		aIndex = MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);

		PMSG_GUILDCREATED_RESULT pResult;

		PHeadSetB((LPBYTE)&pResult, 0x56, sizeof(pResult));
		pResult.Result = 0;
		pResult.btGuildType = lpMsg->btGuildType;

		IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

		if ( gObj[aIndex].m_IfState.use != 0 && gObj[aIndex].m_IfState.type == 5 )
		{
			gObj[aIndex].m_IfState.use = 0;
		}

		return;
	}

	if ( lpMsg->Flag == 1 )
	{
		aIndex = MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);

		if ( gObjIsConnectedGP(aIndex) == TRUE )
		{
			if ( szMaster[0] == gObj[aIndex].Name[0] )
			{
				if ( strcmp(szMaster, gObj[aIndex].Name) == 0 )
				{
					pMsg.h.c = 0xC1;
					pMsg.h.headcode = 0x56;
					pMsg.h.size = sizeof(pMsg);
					pMsg.Result = lpMsg->Result;
					pMsg.btGuildType = lpMsg->btGuildType;

					IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
				}
			}
		}
	}
	
	if ( lpMsg->Result == 1 )
	{
		lpNode = Guild.AddGuild(lpMsg->GuildNumber, szGuildName, lpMsg->Mark, szMaster, 0);

		if ( aIndex >= g_ConfigRead.server.GetObjectStartUserIndex() && lpNode != NULL )
		{
			gObj[aIndex].m_PlayerData->lpGuild = lpNode;
			gObj[aIndex].m_PlayerData->GuildNumber = lpMsg->GuildNumber;
			gObj[aIndex].m_PlayerData->GuildStatus = 0x80;
			lpNode->iGuildRival = 0;
			lpNode->iGuildUnion = 0;
			lpNode->iTimeStamp = 0;
			gObj[aIndex].m_PlayerData->iGuildUnionTimeStamp = 0;

			g_Log.Add("[U.System] Guild is Created - Guild : %s / [%s][%s]",
				szGuildName, gObj[aIndex].AccountID, gObj[aIndex].Name);

			g_Log.Add("Guild War State: %d %d %s", lpNode->WarState, lpNode->WarDeclareState, szGuildName);
			GSProtocol.GCGuildViewportNowPaint(aIndex, szGuildName, lpMsg->Mark, TRUE);
		}
	}
}

struct SDHP_GUILDDESTROY
{
	PBMSG_HEAD h;
	BYTE NumberH;	// 3
	BYTE NumberL;	// 4
	char GuildName[8];	// 5
	char Master[10];	// D
};


void GDGuildDestroySend(int aIndex, LPSTR Name, LPSTR Master)
{
	SDHP_GUILDDESTROY pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0x31;
	pMsg.h.size = sizeof(pMsg);
	pMsg.NumberH = SET_NUMBERH(aIndex);
	pMsg.NumberL = SET_NUMBERL(aIndex);
	memcpy(pMsg.Master, Master, sizeof(pMsg.Master));
	memcpy(pMsg.GuildName, Name, sizeof(pMsg.GuildName));

	wsExDbCli.DataSend((char*)&pMsg, pMsg.h.size);
}



void GDGuildDestroyRecv(SDHP_GUILDDESTROY_RESULT * lpMsg)
{
	int aIndex = -1;
	char szMaster[MAX_ACCOUNT_LEN+1];
	char szGuildName[MAX_GUILD_LEN+1];
	szMaster[MAX_ACCOUNT_LEN] = 0;
	szGuildName[MAX_GUILD_LEN] = 0;

	memcpy(szMaster, lpMsg->Master, MAX_ACCOUNT_LEN);
	memcpy(szGuildName, lpMsg->GuildName, MAX_GUILD_LEN);

	if ( lpMsg->Flag == 1 )
	{
		aIndex = MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);

		if ( gObjIsConnected(aIndex) == TRUE )
		{
			if ( szMaster[0] == gObj[aIndex].Name[0] )
			{
				if ( strcmp(szMaster, gObj[aIndex].Name) == 0 )
				{
					GSProtocol.GCResultSend(aIndex, 0x53, lpMsg->Result);

					if ( lpMsg->Result == 1 || lpMsg->Result == 4 )
					{
						GSProtocol.GCGuildViewportDelNow(aIndex, TRUE);

						if (gObj[aIndex].m_PlayerData->m_bUseGuildMatching == true)
						{
							if (gObj[aIndex].m_PlayerData->GuildNumber > 0)
							{
								GDReqDelMatchingList(aIndex, gObj[aIndex].m_PlayerData->GuildNumber);
							}

							else
							{
								GDReqCancelJoinGuildMatching(aIndex, gObj[aIndex].Name, 0);
							}
						}
					}
				}
			}
		}
	}

	if ( lpMsg->Result == 1 || lpMsg->Result == 4)
	{
		_GUILD_INFO_STRUCT * lpGuild = Guild.SearchGuild(szGuildName);

		if ( lpGuild == NULL )
		{
			return;
		}

		_GUILD_INFO_STRUCT * lpRival = Guild.SearchGuild_Number(lpGuild->iGuildRival);

		if ( lpRival != NULL )
		{
			lpRival->iGuildRival = 0;
			lpRival->szGuildRivalName[0] = 0;
		}

		lpGuild->iGuildUnion = 0;
		lpGuild->iGuildRival = 0;

		for ( int n=0;n<MAX_USER_GUILD;n++)
		{
			int iGuildMemberIndex = lpGuild->Index[n];

			if ( lpGuild->Use[n] > 0 && iGuildMemberIndex != -1 )
			{
				LPOBJ lpObj = &gObj[iGuildMemberIndex];
				
				if ( lpObj == NULL )
				{
					continue;
				}

				if ( gObjIsConnectedGP(iGuildMemberIndex) == FALSE )
				{
					continue;
				}

				gObjNotifyUpdateUnionV1(lpObj);
				gObjNotifyUpdateUnionV2(lpObj);
				lpObj->m_PlayerData->lpGuild = NULL;
				lpObj->m_PlayerData->GuildNumber = 0;
				lpObj->m_PlayerData->GuildStatus = -1;
				lpObj->m_PlayerData->iGuildUnionTimeStamp = 0;

				GSProtocol.GCResultSend(n,0x53, 1);
				GSProtocol.GCGuildViewportDelNow(lpObj->m_Index, FALSE);

				g_Log.Add("Guild delete: %s guild member delete", gObj[n].Name);
			}
		}

		g_ArcaBattle.RemoveRewardBuff(szGuildName);
		UnionManager.DelUnion(lpGuild->Number);
		Guild.DeleteGuild(szGuildName, szMaster);

		g_Log.Add("(%s) Guild delete [ Master: %s ]", szGuildName, szMaster);
	}
}


struct SDHP_GUILDMEMBERADD
{
	PBMSG_HEAD h;
	char GuildName[9];	// 3
	char MemberID[11];	// C
	BYTE NumberH;	// 17
	BYTE NumberL;	// 18
};


void GDGuildMemberAdd(int aIndex, LPSTR Name, LPSTR MemberId)
{
	SDHP_GUILDMEMBERADD pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0x32;
	pMsg.h.size = sizeof(pMsg);
	pMsg.NumberH = SET_NUMBERH(aIndex);
	pMsg.NumberL = SET_NUMBERL(aIndex);
	pMsg.MemberID[MAX_ACCOUNT_LEN] = 0;	// #error should be MAX_ACCOUNT_LEN
	pMsg.GuildName[MAX_GUILD_LEN] = 0;	// #error should be MAX_GUILD_LEN
	memcpy(pMsg.MemberID, MemberId, MAX_ACCOUNT_LEN);
	memcpy(pMsg.GuildName, Name, MAX_GUILD_LEN);

	wsExDbCli.DataSend((char*)&pMsg, pMsg.h.size);
}

struct SDHP_GUILDMEMBERADD_WITHOUT_USERINDEX
{
	PBMSG_HEAD h;
	char GuildName[9];	// 3
	char MemberID[11];	// C
};

void GDGuildMemberAddWithoutUserIndex(LPSTR Name, LPSTR MemberId)
{
	SDHP_GUILDMEMBERADD_WITHOUT_USERINDEX pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0x39;
	pMsg.h.size = sizeof(pMsg);
	pMsg.MemberID[MAX_ACCOUNT_LEN] = 0;	// #error should be MAX_ACCOUNT_LEN
	pMsg.GuildName[MAX_GUILD_LEN] = 0;	// #error should be MAX_GUILD_LEN
	memcpy(pMsg.MemberID, MemberId, MAX_ACCOUNT_LEN);
	memcpy(pMsg.GuildName, Name, MAX_GUILD_LEN);

	wsExDbCli.DataSend((char*)&pMsg, pMsg.h.size);
}



void GDGuildMemberAddResult(SDHP_GUILDMEMBERADD_RESULT * lpMsg)
{
	_GUILD_INFO_STRUCT * lpNode;
	int aIndex = -1;
	int HereUserIndex=-1;
	char szMember[MAX_ACCOUNT_LEN+1];
	char szGuildName[MAX_GUILD_LEN+1];
	szMember[MAX_ACCOUNT_LEN] = 0;
	szGuildName[MAX_GUILD_LEN] = 0;

	memcpy(szMember, lpMsg->MemberID, MAX_ACCOUNT_LEN);
	memcpy(szGuildName, lpMsg->GuildName, MAX_GUILD_LEN);

	if ( lpMsg->Flag == 1 )
	{
		aIndex = MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);

		if ( gObjIsConnectedGP(aIndex) == TRUE )
		{
			//if ( szMember[0] == gObj[aIndex].Name[0] )
			//{
				if ( strcmp(szMember, gObj[aIndex].Name) == 0 )
				{
					GSProtocol.GCResultSend(aIndex, 0x51, lpMsg->Result);
					HereUserIndex = aIndex;
				}
			//}
		}
	}

	if ( lpMsg->Result == 1 )
	{
		lpNode = Guild.AddMember(szGuildName, szMember, HereUserIndex, -1, 0, lpMsg->pServer);

		if (ObjectMaxRange(aIndex) && lpNode != NULL )
		{
			gObj[aIndex].m_PlayerData->lpGuild = lpNode;
			gObj[aIndex].m_PlayerData->GuildStatus = 0;
			gObj[aIndex].m_PlayerData->GuildNumber = lpNode->Number;
			gObj[aIndex].m_PlayerData->iGuildUnionTimeStamp = 0;
			GSProtocol.GCGuildViewportNowPaint(aIndex, szGuildName, gObj[aIndex].m_PlayerData->lpGuild->Mark, FALSE);

			LPOBJ lpObj = &gObj[aIndex];

			for ( int n = 0; n < MAX_VIEWPORT; ++n )
			{
				
				int iTargetIndex = lpObj->VpPlayer2[n].number;

				if(iTargetIndex >= 0)
				{
					LPOBJ lpTargetObj = &gObj[iTargetIndex];

					if ( lpTargetObj->VpPlayer2[n].type == OBJ_USER )
					{
						if ( gObjIsConnected(lpTargetObj) && lpTargetObj->m_PlayerData->lpGuild != NULL )
						{
							if ( lpObj->m_PlayerData->lpGuild == lpTargetObj->m_PlayerData->lpGuild )
							{
								gObjClearViewport(lpTargetObj);
								gObjViewportListCreate(lpTargetObj->m_Index);
								gObjViewportListProtocolCreate(lpTargetObj);
							}
						}
					}
				}
			}

			gObjClearViewport(lpObj);
			gObjViewportListCreate(lpObj->m_Index);
			gObjViewportListProtocolCreate(lpObj);
		}
	}
}

void GDGuildMemberAddResultWithoutUserIndex(SDHP_GUILDMEMBERADD_RESULT_WITHOUT_USERINDEX * lpMsg)
{
	_GUILD_INFO_STRUCT * lpNode;
	int aIndex = -1;
	int HereUserIndex = -1;
	char szMember[MAX_ACCOUNT_LEN + 1];
	char szGuildName[MAX_GUILD_LEN + 1];
	szMember[MAX_ACCOUNT_LEN] = 0;
	szGuildName[MAX_GUILD_LEN] = 0;

	memcpy(szMember, lpMsg->MemberID, MAX_ACCOUNT_LEN);
	memcpy(szGuildName, lpMsg->GuildName, MAX_GUILD_LEN);

	if (lpMsg->Flag == 1)
	{
		aIndex = lpMsg->Number;

		if (gObjIsConnectedGP(aIndex) == TRUE)
		{
			if (strcmp(szMember, gObj[aIndex].Name) == 0)
			{
				GSProtocol.GCResultSend(aIndex, 0x51, lpMsg->Result);
				HereUserIndex = aIndex;
			}
		}
	}

	if (lpMsg->Result == 1)
	{
		lpNode = Guild.AddMember(szGuildName, szMember, HereUserIndex, -1, 0, lpMsg->pServer);

		if (ObjectMaxRange(aIndex) && lpNode != NULL && gObj[aIndex].Type == OBJ_USER)
		{
			gObj[aIndex].m_PlayerData->lpGuild = lpNode;
			gObj[aIndex].m_PlayerData->GuildStatus = 0;
			gObj[aIndex].m_PlayerData->GuildNumber = lpNode->Number;
			gObj[aIndex].m_PlayerData->iGuildUnionTimeStamp = 0;
			GSProtocol.GCGuildViewportNowPaint(aIndex, szGuildName, gObj[aIndex].m_PlayerData->lpGuild->Mark, FALSE);

			LPOBJ lpObj = &gObj[aIndex];

			for (int n = 0; n < MAX_VIEWPORT; ++n)
			{

				int iTargetIndex = lpObj->VpPlayer2[n].number;

				if (iTargetIndex >= 0)
				{
					LPOBJ lpTargetObj = &gObj[iTargetIndex];

					if (lpTargetObj->VpPlayer2[n].type == OBJ_USER)
					{
						if (gObjIsConnected(lpTargetObj) && lpTargetObj->m_PlayerData->lpGuild != NULL)
						{
							if (lpObj->m_PlayerData->lpGuild == lpTargetObj->m_PlayerData->lpGuild)
							{
								gObjClearViewport(lpTargetObj);
								gObjViewportListCreate(lpTargetObj->m_Index);
								gObjViewportListProtocolCreate(lpTargetObj);
							}
						}
					}
				}
			}

			gObjClearViewport(lpObj);
			gObjViewportListCreate(lpObj->m_Index);
			gObjViewportListProtocolCreate(lpObj);
		}
	}
}


struct SDHP_GUILDMEMBERDEL
{
	PBMSG_HEAD h;	// C1:33
	BYTE NumberH;	// 3
	BYTE NumberL;	// 4
	char GuildName[8];	// 5
	char MemberID[10];	// D
};


void GDGuildMemberDel(int aIndex, LPSTR Name, LPSTR MemberId)
{
	SDHP_GUILDMEMBERDEL pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0x33;
	pMsg.h.size = sizeof(pMsg);
	pMsg.NumberH = SET_NUMBERH(aIndex);
	pMsg.NumberL = SET_NUMBERL(aIndex);
	memcpy(pMsg.MemberID, MemberId, MAX_ACCOUNT_LEN);
	memcpy(pMsg.GuildName, Name, MAX_GUILD_LEN);

	wsExDbCli.DataSend((char*)&pMsg, pMsg.h.size);
}



void GDGuildMemberDelResult(SDHP_GUILDMEMBERDEL_RESULT * lpMsg)
{
	int aIndex = -1;
	char szMember[MAX_ACCOUNT_LEN+1];
	char szGuildName[MAX_GUILD_LEN+1];
	szMember[MAX_ACCOUNT_LEN] = 0;
	szGuildName[MAX_GUILD_LEN] = 0;

	memcpy(szMember, lpMsg->MemberID, MAX_ACCOUNT_LEN);
	memcpy(szGuildName, lpMsg->GuildName, MAX_GUILD_LEN);

	if ( lpMsg->Flag == 1 )
	{
		aIndex = MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);

		if ( gObjIsConnected(aIndex) == TRUE )
		{
			//if ( szMember[0] == gObj[aIndex].Name[0] )
			//{
				if ( strcmp(szMember, gObj[aIndex].Name) == 0 )
				{
					GSProtocol.GCResultSend(aIndex, 0x53, lpMsg->Result);
					g_Log.Add("Request to delete guild member caused to send 53 %s", gObj[aIndex].Name);
				}
				else
				{
					GSProtocol.GCResultSend(aIndex, 0x53, 5);
				}
			//}
		}
		else
		{
			g_Log.Add("%s is not connected", gObj[aIndex].Name);
		}
	}

	if ( lpMsg->Result == 1 )
	{
		int iMember = 0;

		while ( true )
		{
			if ( gObj[iMember].Connected > PLAYER_LOGGED && gObj[iMember].Type == OBJ_USER)
			{
				if ( gObj[iMember].m_PlayerData->GuildNumber > 0 )
				{
					if ( gObj[iMember].Name[0] == szMember[0] && gObj[iMember].Name[1] == szMember[1] )
					{
						if ( strcmp(gObj[iMember].Name, szMember) == 0 )
						{
							GSProtocol.GCGuildViewportDelNow(iMember, FALSE);
							gObj[iMember].m_PlayerData->lpGuild = NULL;
							gObj[iMember].m_PlayerData->GuildNumber = 0;
							gObj[iMember].m_PlayerData->GuildStatus = -1;
							gObjNotifyUpdateUnionV1(&gObj[iMember]);

							// User is ejected from guild %d %s
							g_Log.Add("User is ejected from guild %d %s", aIndex, gObj[iMember].Name);
							break;
						}
					}
				}
			}

			if ( iMember < g_ConfigRead.server.GetObjectMax()-1 )
			{
				iMember++;
			}
			else
			{
				break;
			}
		}

		Guild.DelMember(szGuildName, szMember);
	}
	else
	{
		g_Log.Add("%s failed to delete from guild, result: %d", szGuildName, lpMsg->Result);
	}
}



void GDGuildUpdate(LPSTR Name, LPSTR Master, GUILDMARK Mark, int score, int count)
{
	SDHP_GUILDUPDATE pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0x34;
	pMsg.h.size = sizeof(pMsg);
	memcpy(pMsg.GuildName, Name, MAX_GUILD_LEN);
	memcpy(pMsg.Mark, Mark, sizeof(pMsg.Mark));
	memcpy(pMsg.Master, Master, MAX_ACCOUNT_LEN);
	pMsg.Score = score;
	pMsg.Count = count;

	wsExDbCli.DataSend((char*)&pMsg, pMsg.h.size);
}

void GDGuildUpdateRecv(LPBYTE lpMsg)
{
	return;	// NO MAcro
}



struct SDHP_GUILDMEMBER_INFO_REQUEST
{
	PBMSG_HEAD h;	// C1:35
	BYTE NumberH;	// 3
	BYTE NumberL;	// 4
	char MemberID[10];	// 5
};

void DGGuildMemberInfoRequest(int aIndex)
{
	SDHP_GUILDMEMBER_INFO_REQUEST pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x35, sizeof(pMsg));
	pMsg.NumberH = SET_NUMBERH(aIndex);
	pMsg.NumberL = SET_NUMBERL(aIndex);
	memcpy(pMsg.MemberID, gObj[aIndex].Name, sizeof(pMsg.MemberID));

	wsExDbCli.DataSend((char*)&pMsg, pMsg.h.size);
}





void DGGuildMemberInfo(SDHP_GUILDMEMBER_INFO * lpMsg)
{
	BYTE GuildUserBuf[256] = { 0 };
	BYTE GuildInfoBuf[256] = { 0 };
	PMSG_SIMPLE_GUILDVIEWPORT pMsg;
	char szGuildName[MAX_GUILD_LEN + 1];
	char szName[MAX_ACCOUNT_LEN + 1];

	szGuildName[MAX_GUILD_LEN] = 0;
	szName[MAX_ACCOUNT_LEN] = 0;
	memcpy(szGuildName, lpMsg->GuildName, MAX_GUILD_LEN);
	memcpy(szName, lpMsg->MemberID, MAX_ACCOUNT_LEN);

	int GuildInfoOfs = sizeof(PMSG_SIMPLE_GUILDVIEWPORT_COUNT);
	int GuildUserOfs = sizeof(PMSG_SIMPLE_GUILDVIEWPORT_COUNT);

	for (int n = g_ConfigRead.server.GetObjectStartUserIndex(); n < g_ConfigRead.server.GetObjectMax(); n++)
	{
		if (gObj[n].Connected > PLAYER_LOGGED)
		{
			if (gObj[n].Name[0] == lpMsg->MemberID[0])
			{
				if (strcmp(lpMsg->MemberID, gObj[n].Name) == 0)
				{
					if (g_ConfigRead.server.GetServerType() == SERVER_CASTLE)
					{
						szGuildName[8] = NULL;
						g_CastleSiege.GetCsJoinSide(szGuildName, &gObj[n].m_btCsJoinSide, &gObj[n].m_bCsGuildInvolved);
						g_CastleSiege.NotifySelfCsJoinSide(n);
					}

					strcpy(gObj[n].m_PlayerData->GuildName, szGuildName);
					gObj[n].m_PlayerData->lpGuild = Guild.SearchGuild(gObj[n].m_PlayerData->GuildName);

					if (gObj[n].m_PlayerData->lpGuild != NULL)
					{
						gObj[n].m_PlayerData->GuildStatus = lpMsg->btGuildStatus;
						gObj[n].m_PlayerData->GuildNumber = gObj[n].m_PlayerData->lpGuild->Number;
						Guild.ConnectUser(gObj[n].m_PlayerData->lpGuild, lpMsg->GuildName, gObj[n].Name, n, lpMsg->pServer);
						/*gObj[n].lpGuild->Notice[MAX_GUILD_NOTICE_LEN-1] = 0;
						gObj[n].lpGuild->Notice[MAX_GUILD_NOTICE_LEN] = 0;*/

						if (strlen(gObj[n].m_PlayerData->lpGuild->Notice) > 0)
						{
							GSProtocol.GCServerMsgStringSend(gObj[n].m_PlayerData->lpGuild->Notice, n, 2);
						}

						g_Log.Add("[U.System][Guild Status][Info] Guild : %s / [%s][%s] / %d",
							szGuildName, gObj[n].AccountID, szName, gObj[n].m_PlayerData->GuildStatus);

						g_Log.Add("[%s][%s] GuildName: %s", gObj[n].AccountID, szName, szGuildName);

						pMsg.GuildNumber = gObj[n].m_PlayerData->GuildNumber;
						pMsg.NumberH = SET_NUMBERH(n) & 0x7F;
						pMsg.NumberL = SET_NUMBERL(n);

						if (strcmp(gObj[n].m_PlayerData->lpGuild->Names[0], gObj[n].Name) == 0) // Case Guild Master
						{
							pMsg.NumberH |= 0x80;
						}

						pMsg.btGuildStatus = gObj[n].m_PlayerData->GuildStatus;

						if (gObj[n].m_PlayerData->lpGuild != NULL)
						{
							pMsg.btGuildType = gObj[n].m_PlayerData->lpGuild->btGuildType;
						}
						else
						{
							pMsg.btGuildType = G_TYPE_NONE;
						}

						//Season 4.5 Addon start
						if (g_CastleSiegeSync.CheckCastleOwnerMember(gObj[n].m_Index) == 1 ||
							g_CastleSiegeSync.CheckCastleOwnerUnionMember(gObj[n].m_Index) == 1)
						{
							pMsg.btOwnerStatus = 1;
						}
						else
						{
							pMsg.btOwnerStatus = 0;
						}
						//Season 4.5 Addon end

						memcpy(&GuildInfoBuf[GuildInfoOfs], &pMsg, sizeof(PMSG_SIMPLE_GUILDVIEWPORT));
						GuildInfoOfs += sizeof(PMSG_SIMPLE_GUILDVIEWPORT);

						PMSG_SIMPLE_GUILDVIEWPORT_COUNT pGVCount;

						pGVCount.h.c = 0xC2;
						pGVCount.h.headcode = 0x65;
						pGVCount.h.sizeH = SET_NUMBERH(GuildInfoOfs);
						pGVCount.h.sizeL = SET_NUMBERL(GuildInfoOfs);
						pGVCount.Count = 0x01;
						memcpy(GuildInfoBuf, &pGVCount, sizeof(PMSG_SIMPLE_GUILDVIEWPORT_COUNT));

						IOCP.DataSend(n, (LPBYTE)GuildInfoBuf, GuildInfoOfs);

						if (!Guild.SearchGuild(gObj[n].m_PlayerData->lpGuild->szGuildRivalName))
						{
							GDGuildInfoSendRequest(gObj[n].m_PlayerData->lpGuild->szGuildRivalName);
						}

						if (gObj[n].m_PlayerData->lpGuild->WarState != 0)
						{
							GSProtocol.GCGuildWarScore(n);
							GSProtocol.GCGuildWarDeclare(n, gObj[n].m_PlayerData->lpGuild->TargetGuildName);
						}
						if (g_ArcaBattle.IsArcaBattleServer() && g_AcheronGuardianEvent.IsPlayStart() == false)
							g_ArcaBattle.PrintGuildMemberLog(gObj[n].m_PlayerData->lpGuild);

					}
					else
					{
						if (g_ConfigRead.server.GetServerType() == SERVER_CASTLE)
						{
							gObj[n].m_btCsJoinSide = 0;
							gObj[n].m_bCsGuildInvolved = 0;
						}

						g_Log.Add("error: Failed to receive guild information while connectting");
					}

					return;
				}
			}
		}
	}

	Guild.SetServer(szGuildName, szName, lpMsg->pServer);
}



struct SDHP_GUILDALL
{
	char MemberID[11];	// 0
	BYTE btGuildStatus;	// B
	short pServer;	// C
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


void DGGuildMasterListRecv(unsigned char * lpData)
{
	SDHP_GUILDALL_COUNT * lpCount;
	SDHP_GUILDALL * lpList;
	int lOfs = sizeof(SDHP_GUILDALL_COUNT);
	char szGuildName[MAX_GUILD_LEN+1];
	char szMasterName[MAX_ACCOUNT_LEN+1];

	memset(szMasterName, 0, sizeof(szMasterName));
	memset(szGuildName, 0, sizeof(szGuildName));
	lpCount = (SDHP_GUILDALL_COUNT *)lpData;
	memcpy(szGuildName, lpCount->GuildName, sizeof(lpCount->GuildName)-1);
	memcpy(szMasterName, lpCount->Master, sizeof(lpCount->Master)-1);

	if ( strlen(szGuildName) < 1 )
	{
		return;
	}

	if ( lpCount->Count < 1 )
	{
		return;
	}

	Guild.AddGuild(lpCount->Number, szGuildName, lpCount->Mark, szMasterName, lpCount->score);
	Guild.SetGuildType(szGuildName, lpCount->btGuildType);

	_GUILD_INFO_STRUCT * lpGuild = Guild.SearchGuild_Number(lpCount->Number);

	if ( lpGuild != NULL )
	{
		lpGuild->iGuildUnion = lpCount->iGuildUnion;
		lpGuild->iGuildRival = lpCount->iGuildRival;
		memcpy(lpGuild->szGuildRivalName, lpCount->szGuildRivalName, sizeof(lpCount->szGuildRivalName)-1);
	}

	for ( int n=0;n<lpCount->Count;n++)
	{
		lpList = (SDHP_GUILDALL *)&lpData[lOfs];

		memcpy(szMasterName, lpList->MemberID, sizeof(lpList->MemberID)-1);
		
		if ( Guild.AddMember(szGuildName, szMasterName, -1, lpCount->Count,lpList->btGuildStatus, lpList->pServer) == FALSE )
		{
			break;
		}

		lOfs += sizeof(SDHP_GUILDALL);
	}
}

struct SDHP_GUILDMEMBER_INFO_GUILDNAME_REQUEST
{
	PBMSG_HEAD h;
	char szGuildName[MAX_GUILD_LEN + 1];
};

void GDGuildInfoSendRequest(char *szGuildName)
{
	SDHP_GUILDMEMBER_INFO_GUILDNAME_REQUEST pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x42, sizeof(SDHP_GUILDMEMBER_INFO_GUILDNAME_REQUEST));
	memset(pMsg.szGuildName, 0x00, sizeof(pMsg.szGuildName));
	memcpy(pMsg.szGuildName, szGuildName, MAX_GUILD_LEN);
	pMsg.szGuildName[MAX_GUILD_LEN] = 0;

	wsExDbCli.DataSend((char *)&pMsg, pMsg.h.size);
}


void DGGuildScoreUpdate(LPSTR guildname, int score)
{
	SDHP_GUILDSCOREUPDATE pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x37, sizeof(SDHP_GUILDSCOREUPDATE));
	pMsg.Score = score;
	strcpy(pMsg.GuildName, guildname);

	wsExDbCli.DataSend((char*)&pMsg, pMsg.h.size);

	g_Log.Add("Guild Score send: %s %d", guildname, score);
}



void GDGuildScoreUpdateRecv(SDHP_GUILDSCOREUPDATE * lpMsg)
{
	_GUILD_INFO_STRUCT * lpNode = Guild.SearchGuild(lpMsg->GuildName);

	if ( lpNode == NULL )
	{
		return;
	}

	lpNode->TotalScore = lpMsg->Score;
	g_Log.Add("%s Guild Score update (%d)", lpMsg->GuildName, lpMsg->Score);
}



void GDGuildNoticeSave(LPSTR guild_name, LPSTR guild_notice)
{
	SDHP_GUILDNOTICE pMsg;

	int len = strlen((char*)guild_notice);

	if ( len < 1 )
	{
		return;
	}

	if ( len > MAX_GUILD_NOTICE_LEN-2 )
	{
		return;
	}

	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0x38;
	pMsg.h.size = sizeof(SDHP_GUILDNOTICE);
	strcpy(pMsg.GuildName, guild_name);
	strcpy(pMsg.szGuildNotice, guild_notice);

	wsExDbCli.DataSend((char*)&pMsg, pMsg.h.size);
}



void DGGuildNoticeRecv(SDHP_GUILDNOTICE * lpMsg)
{
	_GUILD_INFO_STRUCT * lpNode = Guild.SearchGuild(lpMsg->GuildName);

	if ( lpNode == NULL )
	{
		return;
	}

	strcpy(lpNode->Notice, lpMsg->szGuildNotice);
	g_Log.Add("Notice (%s) : %s", lpMsg->GuildName, lpMsg->szGuildNotice);

	for ( int n=0;n<MAX_USER_GUILD;n++)
	{
		if ( lpNode->Use[n] > 0 && lpNode->Index[n] >= 0 )
		{
			for(int i=0;i<sizeof(lpMsg->szGuildNotice);i++)
			{
				if(lpMsg->szGuildNotice[i] == '%')
				{
					GSProtocol.GCServerMsgStringSend(Lang.GetText(0,537), lpNode->Index[n], 2);
					return;
				}
			}
			GSProtocol.GCServerMsgStringSend(lpMsg->szGuildNotice, lpNode->Index[n], 2);
		}
	}
}


struct SDHP_GUILDLISTREQUEST
{
	PBMSG_HEAD h;	// C1:40
};

void DGGuildListRequest()
{
	SDHP_GUILDLISTREQUEST pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x40, sizeof(SDHP_GUILDLISTREQUEST));

	wsExDbCli.DataSend((char*)&pMsg, pMsg.h.size);
}






void DGGuildList(SDHP_GUILDCREATED * lpMsg)
{
	char szTemp[256];

	Guild.AddGuild(lpMsg->Number, lpMsg->GuildName, lpMsg->Mark, lpMsg->Master, lpMsg->score);

}

struct EXSDHP_SERVERGROUP_GUILD_CHATTING_SEND
{
	PBMSG_HEAD h;	// C1:50
	int iGuildNum;	// 4
	char szCharacterName[10];	// 8
	char szChattingMsg[90];	// 12
};

void GDGuildServerGroupChattingSend(int iGuildNum,  PMSG_CHATDATA* lpChatData)
{
	EXSDHP_SERVERGROUP_GUILD_CHATTING_SEND pMsg = {0};

	pMsg.h.set((LPBYTE)&pMsg, 0x50, sizeof(EXSDHP_SERVERGROUP_GUILD_CHATTING_SEND));
	pMsg.iGuildNum = iGuildNum;
	memcpy(pMsg.szCharacterName, lpChatData->chatid, sizeof(pMsg.szCharacterName));
	memcpy(pMsg.szChattingMsg, lpChatData->chatmsg, sizeof(pMsg.szChattingMsg));

	wsExDbCli.DataSend((char*)&pMsg, pMsg.h.size);
}

void DGGuildServerGroupChattingRecv( EXSDHP_SERVERGROUP_GUILD_CHATTING_RECV* lpMsg)
{
	PMSG_CHATDATA pMsg ={0};

	pMsg.h.set((LPBYTE)&pMsg, 0x00, sizeof(pMsg));	// #error MAYBE the header is wrong
	memcpy(pMsg.chatid, lpMsg->szCharacterName, sizeof(pMsg.chatid));
	memcpy(pMsg.chatmsg, lpMsg->szChattingMsg, sizeof(pMsg.chatmsg));
	_GUILD_INFO_STRUCT * lpGuildInfo = Guild.SearchGuild_Number(lpMsg->iGuildNum);

	if ( lpGuildInfo == NULL )
		return;

	int iUserIndex = -1;

	if ( lpGuildInfo->Count >= 0 )
	{
		for ( int i=0;i<MAX_USER_GUILD;i++)
		{
			if ( lpGuildInfo->Use[i] != FALSE )
			{
				iUserIndex = lpGuildInfo->Index[i];

				if ( iUserIndex >= 0 )
				{
					if ( lpGuildInfo->Names[i][0] == gObj[iUserIndex].Name[0] )
					{
						if ( !strcmp(lpGuildInfo->Names[i], gObj[iUserIndex].Name ))
						{
							IOCP.DataSend(iUserIndex, (LPBYTE)&pMsg, pMsg.h.size);
						}
					}
				}
			}
		}
	}
}


struct EXSDHP_SERVERGROUP_UNION_CHATTING_SEND
{
	PBMSG_HEAD h;
	int iUnionNum;	// 4
	char szCharacterName[10];	// 8
	char szChattingMsg[90];	// 12
};

void GDUnionServerGroupChattingSend(int iUnionNum,  PMSG_CHATDATA* lpChatData)
{
	EXSDHP_SERVERGROUP_UNION_CHATTING_SEND pMsg = {0};

	pMsg.h.set((LPBYTE)&pMsg, 0x51, sizeof(pMsg));
	pMsg.iUnionNum = iUnionNum;
	memcpy(pMsg.szCharacterName, lpChatData->chatid, sizeof(pMsg.szCharacterName));
	memcpy(pMsg.szChattingMsg, lpChatData->chatmsg, sizeof(pMsg.szChattingMsg));

	wsExDbCli.DataSend((PCHAR)&pMsg, pMsg.h.size);
}

void DGUnionServerGroupChattingRecv( EXSDHP_SERVERGROUP_UNION_CHATTING_RECV* lpMsg)
{
	PMSG_CHATDATA pMsg ={0};

	pMsg.h.set((LPBYTE)&pMsg, 0x00, sizeof(pMsg));
	memcpy(pMsg.chatid, lpMsg->szCharacterName, sizeof(pMsg.chatid));
	memcpy(pMsg.chatmsg, lpMsg->szChattingMsg, sizeof(pMsg.chatmsg));
	int iUnionNum = lpMsg->iUnionNum;
	int iGuildCount =0;
	int iGuildList[100] ={0};

	if ( UnionManager.GetGuildUnionMemberList(iUnionNum, iGuildCount, iGuildList) == TRUE)
	{
		int iUserIndex = -1;

		for ( int i=0;i<iGuildCount;i++)
		{
			_GUILD_INFO_STRUCT * lpGuildInfo = Guild.SearchGuild_Number(iGuildList[i]);

			if ( lpGuildInfo == NULL )
				continue;

			for ( int n=0;n<MAX_USER_GUILD;n++)
			{
				if ( lpGuildInfo->Use[n] != FALSE )
				{
					iUserIndex = lpGuildInfo->Index[n];

					if ( iUserIndex >= 0 )
					{
						if ( lpGuildInfo->Names[n][0] == gObj[iUserIndex].Name[0] )
						{
							if ( !strcmp(lpGuildInfo->Names[n], gObj[iUserIndex].Name ))
							{
								IOCP.DataSend(iUserIndex, (LPBYTE)&pMsg, pMsg.h.size);
							}
						}
					}
				}
			}
		}
	}
}

struct EXSDHP_GUILD_ASSIGN_STATUS_REQ
{
	PBMSG_HEAD h;
	WORD wUserIndex;	// 4
	BYTE btType;	// 6
	BYTE btGuildStatus;	// 7
	char szGuildName[9];	// 8
	char szTargetName[11];	// 11
};

void GDGuildReqAssignStatus(int aIndex, int iAssignType, LPSTR szTagetName, int iGuildStatus)
{
	EXSDHP_GUILD_ASSIGN_STATUS_REQ pMsg = {0};

	pMsg.h.set((LPBYTE)&pMsg, 0xE1, sizeof(EXSDHP_GUILD_ASSIGN_STATUS_REQ));
	pMsg.btType = iAssignType;
	pMsg.btGuildStatus = iGuildStatus;
	pMsg.wUserIndex = aIndex;
	pMsg.szGuildName[MAX_GUILD_LEN] = 0;
	pMsg.szTargetName[MAX_ACCOUNT_LEN] = 0;
	memcpy(pMsg.szGuildName, gObj[aIndex].m_PlayerData->lpGuild->Name, MAX_GUILD_LEN);
	memcpy(pMsg.szTargetName, szTagetName, MAX_ACCOUNT_LEN);

	wsExDbCli.DataSend((char*)&pMsg, pMsg.h.size);
}




void DGGuildAnsAssignStatus(EXSDHP_GUILD_ASSIGN_STATUS_RESULT * lpMsg)
{
	if ( lpMsg->btFlag == 1 )
	{
		_GUILD_INFO_STRUCT * lpNode = Guild.SearchGuild(lpMsg->szGuildName);

		if ( lpNode == NULL )
		{
			lpMsg->btResult = 0;
		}

		PMSG_GUILD_ASSIGN_STATUS_RESULT pMsg = {0};

		pMsg.h.set((LPBYTE)&pMsg, 0xE1, sizeof(PMSG_GUILD_ASSIGN_STATUS_RESULT));
		pMsg.btType = lpMsg->btType;
		pMsg.btResult = lpMsg->btResult;
		memcpy(pMsg.szTagetName, lpMsg->szTargetName, sizeof(pMsg.szTagetName));

		IOCP.DataSend(lpMsg->wUserIndex, (LPBYTE)&pMsg, sizeof(pMsg));

		if ( lpMsg->btResult == 0 )
		{
			return;
		}

		Guild.SetGuildMemberStatus(lpMsg->szGuildName, lpMsg->szTargetName, lpMsg->btGuildStatus);

		g_Log.Add("[U.System][Guild Status][Assign] Guild : ( %s ) Member : ( %s ) Status : ( %d )",
			lpMsg->szGuildName, lpMsg->szTargetName, lpMsg->btGuildStatus);

		for ( int i=0;i<MAX_USER_GUILD;i++)
		{
			if ( lpNode->Use[i] > 0 && lpNode->Index[i] >= 0)
			{
				TNotice _Notice(0);

				if ( lpMsg->btGuildStatus == GUILD_ASSISTANT )
				{
					_Notice.SendToUser(lpNode->Index[i], Lang.GetText(0,212), lpMsg->szTargetName);
				}
				else if ( lpMsg->btGuildStatus == GUILD_BATTLE_MASTER )
				{
					_Notice.SendToUser(lpNode->Index[i], Lang.GetText(0,190), lpMsg->szTargetName);
				}
				else
				{
					_Notice.SendToUser(lpNode->Index[i], Lang.GetText(0,191), lpMsg->szTargetName);
				}
			}
		}
	}
	else if ( lpMsg->btFlag == 0 )
	{
		_GUILD_INFO_STRUCT * lpNode = Guild.SearchGuild(lpMsg->szGuildName);

		if ( lpNode == NULL )
		{
			return;
		}
		
		Guild.SetGuildMemberStatus(lpMsg->szGuildName, lpMsg->szTargetName, lpMsg->btGuildStatus);

		g_Log.Add("[U.System][Guild Status][Assign] Guild : ( %s ) Member : ( %s ) Status : ( %d )",
			lpMsg->szGuildName, lpMsg->szTargetName, lpMsg->btGuildStatus);
		
		if ( lpMsg->btResult == 0 )
		{
			return;
		}

		for ( int n=0;n<MAX_USER_GUILD;n++)
		{
			if ( lpNode->Use[n] > 0 && lpNode->Index[n] >= 0)
			{
				TNotice _Notice(0);

				if ( lpMsg->btGuildStatus == GUILD_ASSISTANT )
				{
					_Notice.SendToUser(lpNode->Index[n], Lang.GetText(0,212), lpMsg->szTargetName);
				}
				else if ( lpMsg->btGuildStatus == GUILD_BATTLE_MASTER )
				{
					_Notice.SendToUser(lpNode->Index[n], Lang.GetText(0,190), lpMsg->szTargetName);
				}
				else
				{
					_Notice.SendToUser(lpNode->Index[n], Lang.GetText(0,191), lpMsg->szTargetName);
				}
			}
		}
	}
}

struct EXSDHP_GUILD_ASSIGN_TYPE_REQ
{
	PBMSG_HEAD h;
	WORD wUserIndex;	// 4
	BYTE btGuildType;	// 6;
	char szGuildName[9];	// 7
};



void GDGuildReqAssignType(int aIndex, int iGuildType)
{
	EXSDHP_GUILD_ASSIGN_TYPE_REQ pMsg = {0};

	pMsg.h.set((LPBYTE)&pMsg, 0xE2, sizeof(EXSDHP_GUILD_ASSIGN_TYPE_REQ));
	pMsg.btGuildType = iGuildType;
	pMsg.wUserIndex = aIndex;
	pMsg.szGuildName[MAX_GUILD_LEN] = 0;
	memcpy(pMsg.szGuildName, gObj[aIndex].m_PlayerData->lpGuild->Name, MAX_GUILD_LEN);

	wsExDbCli.DataSend((char*)&pMsg, pMsg.h.size);
}





void DGGuildAnsAssignType(EXSDHP_GUILD_ASSIGN_TYPE_RESULT * lpMsg)
{
	if ( lpMsg->btFlag == 1 )
	{
		_GUILD_INFO_STRUCT * lpNode = Guild.SearchGuild(lpMsg->szGuildName);

		if ( lpNode == NULL )
		{
			lpMsg->btResult = 0;
		}

		PMSG_GUILD_ASSIGN_TYPE_RESULT pMsg = {0};

		pMsg.h.set((LPBYTE)&pMsg, 0xE2, sizeof(PMSG_GUILD_ASSIGN_TYPE_RESULT));
		pMsg.btGuildType = lpMsg->btGuildType;
		pMsg.btResult = lpMsg->btResult;

		IOCP.DataSend(lpMsg->wUserIndex, (LPBYTE)&pMsg,sizeof(pMsg));

		if ( lpMsg->btResult == 0 )
		{
			return;
		}

		Guild.SetGuildType(lpMsg->szGuildName, lpMsg->btGuildType);

		for ( int i=0;i<MAX_USER_GUILD;i++)
		{
			if ( lpNode->Use[i] > 0 && lpNode->Index[i] >= 0)
			{
				TNotice pNotice(0);
				pNotice.SendToUser(lpNode->Index[i], Lang.GetText(0,192), lpMsg->szGuildName, lpMsg->btGuildType);
			}
		}
	}
	else if ( lpMsg->btFlag == 0 )
	{
		_GUILD_INFO_STRUCT * lpNode = Guild.SearchGuild(lpMsg->szGuildName);

		if ( lpNode == NULL )
		{
			return;
		}

		Guild.SetGuildType(lpMsg->szGuildName, lpMsg->btGuildType);

		if ( lpMsg->btResult == 0 )
		{
			return;
		}		
		
		for ( int n=0;n<MAX_USER_GUILD;n++)
		{
			if ( lpNode->Use[n] > 0 && lpNode->Index[n] >= 0)
			{
				TNotice _Notice(0);
				_Notice.SendToUser(lpNode->Index[n], Lang.GetText(0,192), lpMsg->szGuildName, lpMsg->btGuildType);
			}
		}
	}
}



struct EXSDHP_RELATIONSHIP_JOIN_REQ
{
	PBMSG_HEAD h;
	WORD wRequestUserIndex;	// 4
	WORD wTargetUserIndex;	// 6
	BYTE btRelationShipType;	// 8
	int iRequestGuildNum;	// C
	int iTargetGuildNum;	// 10
};

void GDRelationShipReqJoin(int aIndex, int iTargetUserIndex, int iRelationShipType)
{
	if(gObj[aIndex].Type != OBJ_USER || gObj[iTargetUserIndex].Type != OBJ_USER)
	{
		return;
	}

	_GUILD_INFO_STRUCT * lpGuildInfo = gObj[aIndex].m_PlayerData->lpGuild;
	_GUILD_INFO_STRUCT * lpTargetGuildInfo = gObj[iTargetUserIndex].m_PlayerData->lpGuild;

	if ( !lpGuildInfo || !lpTargetGuildInfo )
	{
		return;
	}

	EXSDHP_RELATIONSHIP_JOIN_REQ pMsg = {0};

	pMsg.h.set((LPBYTE)&pMsg, 0xE5, sizeof(EXSDHP_RELATIONSHIP_JOIN_REQ));
	pMsg.wRequestUserIndex = aIndex;
	pMsg.wTargetUserIndex = iTargetUserIndex;
	pMsg.btRelationShipType = iRelationShipType;
	pMsg.iRequestGuildNum = gObj[aIndex].m_PlayerData->lpGuild->Number;
	pMsg.iTargetGuildNum = gObj[iTargetUserIndex].m_PlayerData->lpGuild->Number;

	wsExDbCli.DataSend((char*)&pMsg, pMsg.h.size);
}





void DGRelationShipAnsJoin(EXSDHP_RELATIONSHIP_JOIN_RESULT * lpMsg)
{
	_GUILD_INFO_STRUCT * lpReqGuild = Guild.SearchGuild_Number(lpMsg->iRequestGuildNum);
	_GUILD_INFO_STRUCT * lpTargetGuild = Guild.SearchGuild_Number(lpMsg->iTargetGuildNum);

	if ( lpMsg->btFlag == 1 )
	{
		if ( !lpReqGuild || !lpTargetGuild )
		{
			lpMsg->btResult = 0;
		}

		PMSG_RELATIONSHIP_JOIN_BREAKOFF_ANS pMsg ={0};

		pMsg.h.set((LPBYTE)&pMsg, 0xE6, sizeof(pMsg));
		pMsg.btResult = lpMsg->btResult;
		pMsg.btRequestType = G_RELATION_OPERATION_JOIN;
		pMsg.btRelationShipType = lpMsg->btRelationShipType;
		pMsg.btTargetUserIndexH = SET_NUMBERH(lpMsg->wTargetUserIndex);
		pMsg.btTargetUserIndexL = SET_NUMBERL(lpMsg->wTargetUserIndex);

		IOCP.DataSend(lpMsg->wRequestUserIndex, (LPBYTE)&pMsg, sizeof(pMsg));

		pMsg.btTargetUserIndexH = SET_NUMBERH(lpMsg->wRequestUserIndex);
		pMsg.btTargetUserIndexL = SET_NUMBERL(lpMsg->wRequestUserIndex);

		IOCP.DataSend(lpMsg->wTargetUserIndex, (LPBYTE)&pMsg, sizeof(pMsg));

		if ( lpMsg->btResult == EX_RESULT_FAIL ||lpMsg->btResult == EX_RESULT_FAIL_FOR_CASTLE )
			return;

	}
	else if ( lpMsg->btFlag == 0 )
	{
		if ( !lpReqGuild || !lpTargetGuild )
			return;

		if ( lpMsg->btResult == EX_RESULT_FAIL ||lpMsg->btResult == EX_RESULT_FAIL_FOR_CASTLE )
			return;

	}

	if (lpMsg->btRelationShipType == G_RELATIONSHIP_UNION )
	{
		lpReqGuild->SetGuildUnion(lpTargetGuild->Number);
		lpTargetGuild->SetGuildUnion(lpTargetGuild->Number);

		g_Log.Add("[U.System][Union][Join] ( Union Master Guild: %s ), (Union Member Guild: %s)",
			lpTargetGuild->Name, lpReqGuild->Name);
	}
	else if ( lpMsg->btRelationShipType == G_RELATIONSHIP_RIVAL )
	{
		lpReqGuild->SetGuildRival(lpTargetGuild->Number);
		lpTargetGuild->SetGuildRival(lpReqGuild->Number);
		memcpy(lpReqGuild->szGuildRivalName, lpMsg->szTargetGuildName, MAX_GUILD_LEN);
		memcpy(lpTargetGuild->szGuildRivalName, lpMsg->szRequestGuildName, MAX_GUILD_LEN);

		g_Log.Add("[U.System][Rival][Join] (  %s ) vs ( %s )",
			lpReqGuild->Name, lpTargetGuild->Name);
	}

	for ( int n=0;n<MAX_USER_GUILD;n++)
	{
		if ( lpReqGuild->Use[n] > 0 )
		{
			if ( lpReqGuild->Index[n] >= 0 )
			{
				TNotice _Notice(1);

				if ( lpMsg->btRelationShipType == G_RELATIONSHIP_UNION )
				{
					_Notice.SendToUser(lpReqGuild->Index[n], Lang.GetText(0,182), lpTargetGuild->Name);
				}
				else if ( lpMsg->btRelationShipType == G_RELATIONSHIP_RIVAL )
				{
					_Notice.SendToUser(lpReqGuild->Index[n], Lang.GetText(0,183), lpTargetGuild->Name);
				}
			}
		}
	}

	for (int n=0;n<MAX_USER_GUILD;n++)
	{
		if ( lpTargetGuild->Use[n] > 0 )
		{
			if ( lpTargetGuild->Index[n] >= 0 )
			{
				TNotice _Notice(1);

				if ( lpMsg->btRelationShipType == G_RELATIONSHIP_UNION )
				{
					_Notice.SendToUser(lpTargetGuild->Index[n], Lang.GetText(0,184), lpReqGuild->Name, lpTargetGuild->Name);
				}
				else if ( lpMsg->btRelationShipType == G_RELATIONSHIP_RIVAL )
				{
					_Notice.SendToUser(lpTargetGuild->Index[n], Lang.GetText(0,183), lpReqGuild->Name);
				}
			}
		}
	}
}



struct EXSDHP_RELATIONSHIP_BREAKOFF_REQ
{
	PBMSG_HEAD h;
	WORD wRequestUserIndex;	// 4
	WORD wTargetUserIndex;	// 6
	BYTE btRelationShipType;	// 8
	int iRequestGuildNum;	// C
	int iTargetGuildNum;	// 10
};

void GDUnionBreakOff(int aIndex, int iUnionNumber)
{
	if ( gObj[aIndex].Type != OBJ_USER )
	{
		return;
	}

	_GUILD_INFO_STRUCT * lpGuildInfo = gObj[aIndex].m_PlayerData->lpGuild;

	if ( !lpGuildInfo )
	{
		return;
	}

	EXSDHP_RELATIONSHIP_BREAKOFF_REQ pMsg = {0};

	pMsg.h.set((LPBYTE)&pMsg, 0xE6, sizeof(EXSDHP_RELATIONSHIP_BREAKOFF_REQ));
	pMsg.wRequestUserIndex = aIndex;
	pMsg.wTargetUserIndex = -1;
	pMsg.btRelationShipType = G_RELATIONSHIP_UNION;
	pMsg.iRequestGuildNum = lpGuildInfo->Number;
	pMsg.iTargetGuildNum = iUnionNumber;

	wsExDbCli.DataSend((char*)&pMsg, pMsg.h.size);

}

//*******************************************************
// START REIVEW HERE

void DGUnionBreakOff(EXSDHP_RELATIONSHIP_BREAKOFF_RESULT * lpMsg)
{
	_GUILD_INFO_STRUCT * lpReqGuild = Guild.SearchGuild_Number(lpMsg->iRequestGuildNum);
	_GUILD_INFO_STRUCT * lpTargetGuild = Guild.SearchGuild_Number(lpMsg->iTargetGuildNum);

	if ( lpMsg->btFlag == 1 )
	{
		if ( !lpReqGuild )
		{
			lpMsg->btResult = 0;
		}

		PMSG_RELATIONSHIP_JOIN_BREAKOFF_ANS pMsg ={0};

		pMsg.h.set((LPBYTE)&pMsg, 0xE6, sizeof(pMsg));
		pMsg.btResult = lpMsg->btResult;
		pMsg.btRequestType = G_RELATION_OPERATION_BREAKOFF;
		pMsg.btRelationShipType = G_RELATIONSHIP_UNION;
		pMsg.btTargetUserIndexH = SET_NUMBERH(lpMsg->wTargetUserIndex);
		pMsg.btTargetUserIndexL = SET_NUMBERL(lpMsg->wTargetUserIndex);

		IOCP.DataSend(lpMsg->wRequestUserIndex, (LPBYTE)&pMsg, sizeof(pMsg));

		if ( lpMsg->btResult == EX_RESULT_FAIL ||lpMsg->btResult == EX_RESULT_FAIL_FOR_CASTLE )
			return;

	}
	else if ( lpMsg->btFlag == 0 )
	{
		if ( !lpReqGuild )
			return;

		if ( lpMsg->btResult == EX_RESULT_FAIL ||lpMsg->btResult == EX_RESULT_FAIL_FOR_CASTLE )
			return;

	}

	lpReqGuild->SetGuildUnion(0);

	g_Log.Add("[GS][U.System][Union][BreakOff] Request Union Member Guild: %s)",
		lpReqGuild->Name);

	for ( int n=0;n<MAX_USER_GUILD;n++)
	{
		if ( lpReqGuild->Use[n] > 0 )
		{
			if ( lpReqGuild->Index[n] >= 0 )
			{
				TNotice _Notice(1);
				_Notice.SendToUser(lpReqGuild->Index[n], Lang.GetText(0,185));
			}
		}
	}

	if ( !lpTargetGuild)
		return;

	g_Log.Add("[GS][U.System][Union][BreakOff] ( Union Master Guild: %s ), (Union Member Guild: %s)",
		lpTargetGuild->Name, lpReqGuild->Name);

	for (int n=0;n<MAX_USER_GUILD;n++)
	{
		if ( lpTargetGuild->Use[n] > 0 )
		{
			if ( lpTargetGuild->Index[n] >= 0 )
			{
				TNotice _Notice(1);
				_Notice.SendToUser(lpTargetGuild->Index[n], Lang.GetText(0,186), lpReqGuild->Name, lpTargetGuild->Name);
			}
		}
	}
}



void GDRelationShipReqBreakOff(int aIndex, int iTargetUserIndex, int iRelationShipType)
{
	if ( gObj[aIndex].Type != OBJ_USER || gObj[iTargetUserIndex].Type != OBJ_USER )
	{
		return;
	}

	_GUILD_INFO_STRUCT * lpGuildInfo = gObj[aIndex].m_PlayerData->lpGuild;
	_GUILD_INFO_STRUCT * lpTargetGuildInfo = gObj[iTargetUserIndex].m_PlayerData->lpGuild;

	if ( !lpGuildInfo || !lpTargetGuildInfo )
		return;

	EXSDHP_RELATIONSHIP_BREAKOFF_REQ pMsg = {0};

	pMsg.h.set((LPBYTE)&pMsg, 0xE6, sizeof(EXSDHP_RELATIONSHIP_BREAKOFF_REQ));
	pMsg.wRequestUserIndex = aIndex;
	pMsg.wTargetUserIndex = iTargetUserIndex;
	pMsg.btRelationShipType = iRelationShipType;
	pMsg.iRequestGuildNum = lpGuildInfo->Number;
	pMsg.iTargetGuildNum = lpTargetGuildInfo->Number;

	wsExDbCli.DataSend((char*)&pMsg, pMsg.h.size);
}



void DGRelationShipAnsBreakOff(EXSDHP_RELATIONSHIP_BREAKOFF_RESULT * lpMsg)
{
	if ( lpMsg->btRelationShipType == G_RELATIONSHIP_UNION )
	{
		DGUnionBreakOff(lpMsg);
		return;
	}

	_GUILD_INFO_STRUCT * lpReqGuild = Guild.SearchGuild_Number(lpMsg->iRequestGuildNum);
	_GUILD_INFO_STRUCT * lpTargetGuild = Guild.SearchGuild_Number(lpMsg->iTargetGuildNum);

	if ( lpMsg->btFlag == 1 )
	{
		if ( !lpReqGuild || !lpTargetGuild )
		{
			lpMsg->btResult = 0;
		}

		PMSG_RELATIONSHIP_JOIN_BREAKOFF_ANS pMsg ={0};

		pMsg.h.set((LPBYTE)&pMsg, 0xE6, sizeof(pMsg));
		pMsg.btResult = lpMsg->btResult;
		pMsg.btRequestType = 2;
		pMsg.btRelationShipType = lpMsg->btRelationShipType;
		pMsg.btTargetUserIndexH = SET_NUMBERH(lpMsg->wTargetUserIndex);
		pMsg.btTargetUserIndexL = SET_NUMBERL(lpMsg->wTargetUserIndex);

		if ( lpMsg->wRequestUserIndex != -1 )
			IOCP.DataSend(lpMsg->wRequestUserIndex, (LPBYTE)&pMsg, sizeof(pMsg));

		pMsg.btTargetUserIndexH = SET_NUMBERH(lpMsg->wRequestUserIndex);
		pMsg.btTargetUserIndexL = SET_NUMBERL(lpMsg->wRequestUserIndex);

		if ( lpMsg->wTargetUserIndex != -1 )
			IOCP.DataSend(lpMsg->wTargetUserIndex, (LPBYTE)&pMsg, sizeof(pMsg));

		if ( lpMsg->btResult == EX_RESULT_FAIL ||lpMsg->btResult == EX_RESULT_FAIL_FOR_CASTLE )
			return;

	}
	else if ( lpMsg->btFlag == 0 )
	{
		if ( !lpReqGuild || !lpTargetGuild )
			return;

		if ( lpMsg->btResult == EX_RESULT_FAIL ||lpMsg->btResult == EX_RESULT_FAIL_FOR_CASTLE )
			return;

	}

	if ( lpMsg->btRelationShipType == G_RELATIONSHIP_RIVAL )
	{
		lpReqGuild->SetGuildRival(0);
		lpTargetGuild->SetGuildRival(0);
		lpReqGuild->szGuildRivalName[0]=0;
		lpTargetGuild->szGuildRivalName[0]=0;
	}

	g_Log.Add("[U.System][Rival][BreakOff] (  %s ) vs ( %s )",
		lpReqGuild->Name, lpTargetGuild->Name);

	for ( int n=0;n<MAX_USER_GUILD;n++)
	{
		if ( lpReqGuild->Use[n] > 0 )
		{
			if ( lpReqGuild->Index[n] >= 0 )
			{
				TNotice _Notice(1);

				if ( lpMsg->btRelationShipType == G_RELATIONSHIP_UNION )
				{
					_Notice.SendToUser(lpReqGuild->Index[n], Lang.GetText(0,187), lpTargetGuild->Name);
				}
				else if ( lpMsg->btRelationShipType == G_RELATIONSHIP_RIVAL )
				{
					_Notice.SendToUser(lpReqGuild->Index[n], Lang.GetText(0,188), lpTargetGuild->Name);
				}
			}
		}
	}
}



void DGRelationShipNotificationRecv(EXSDHP_NOTIFICATION_RELATIONSHIP * lpMsg)
{
	if ( lpMsg->btUpdateFlag == EX_RESULT_FAIL_FOR_CASTLE )
	{
		_GUILD_INFO_STRUCT * lpGuildInfo = Guild.SearchGuild_Number(lpMsg->iGuildList[0]);

		if ( lpGuildInfo )
		{
			lpGuildInfo->iGuildUnion = 0;
			lpGuildInfo->SetTimeStamp();

			g_Log.Add("[U.System][Union][Auto BreakOff] ( Union Master Guild: %s ), (Union Member Guild: %s)",
				lpGuildInfo->Name, lpGuildInfo->Name);

			for ( int n=0;n<MAX_USER_GUILD;n++)
			{
				if ( lpGuildInfo->Use[n] > 0 )
				{
					if ( lpGuildInfo->Index[n] >= 0 )
					{
						TNotice _Notice(1);
						_Notice.SendToUser(lpGuildInfo->Index[n], Lang.GetText(0,189), lpGuildInfo->Name);
					}
				}
			}
		}
	}

	for ( int i=0;i<lpMsg->btGuildListCount;i++)
	{
		_GUILD_INFO_STRUCT * lpGuildInfo = Guild.SearchGuild_Number(lpMsg->iGuildList[i]);

		if ( lpGuildInfo )
		{
			lpGuildInfo->SetTimeStamp();
		}
	}
}



void DGRelationShipListRecv(EXSDHP_UNION_RELATIONSHIP_LIST * lpMsg)
{
	if ( lpMsg->btRelationShipType == G_RELATIONSHIP_UNION )
	{
		UnionManager.AddUnion(lpMsg->iUnionMasterGuildNumber, lpMsg->szUnionMasterGuildName);
		UnionManager.SetGuildUnionMemberList(lpMsg->iUnionMasterGuildNumber, lpMsg->btRelationShipMemberCount, lpMsg->iRelationShipMember);
	}
	else if ( lpMsg->btRelationShipType == G_RELATIONSHIP_RIVAL )
	{
		UnionManager.AddUnion(lpMsg->iUnionMasterGuildNumber, lpMsg->szUnionMasterGuildName);
		UnionManager.SetGuildRivalMemberList(lpMsg->iUnionMasterGuildNumber, lpMsg->btRelationShipMemberCount, lpMsg->iRelationShipMember);
	}
}


struct EXSDHP_UNION_LIST_REQ
{
	PBMSG_HEAD h;
	WORD wRequestUserIndex;	// 4
	int iUnionMasterGuildNumber;	// 8
};

void GDUnionListSend(int iUserIndex, int iUnionMasterGuildNumber)
{
	EXSDHP_UNION_LIST_REQ pMsg = {0};

	pMsg.h.set((LPBYTE)&pMsg, 0xE9, sizeof(EXSDHP_UNION_LIST_REQ));
	pMsg.wRequestUserIndex = iUserIndex;
	pMsg.iUnionMasterGuildNumber = iUnionMasterGuildNumber;

	wsExDbCli.DataSend((char*)&pMsg, pMsg.h.size);
}


struct EXSDHP_UNION_LIST
{
	BYTE btMemberNum;	// 0
	BYTE Mark[32];	// 1
	char szGuildName[8];	// 21
};

struct EXSDHP_UNION_LIST_COUNT
{
	PWMSG_HEAD h;
	BYTE btCount;	// 4
	BYTE btResult;	// 5
	WORD wRequestUserIndex;	// 6
	int iTimeStamp;	// 8
	BYTE btRivalMemberNum;	// C
	BYTE btUnionMemberNum;	// D
};

struct PMSG_UNIONLIST_COUNT
{
	PWMSG_HEAD h;
	BYTE btCount;	// 4
	BYTE btResult;	// 5
	BYTE btRivalMemberNum;	// 6
	BYTE btUnionMemberNum;	// 7
};


struct PMSG_UNIONLIST
{
	BYTE btMemberNum;	// 0
	BYTE Mark[32];	// 1
	char szGuildName[8];	// 21
};

void DGUnionListRecv(LPBYTE aRecv)
{
	EXSDHP_UNION_LIST_COUNT * lpRecvMsg = (EXSDHP_UNION_LIST_COUNT *)aRecv;
	EXSDHP_UNION_LIST * lpRecvMsgBody = (EXSDHP_UNION_LIST *)(aRecv + 0x10 ) ;	// #error it 0x10 ???
	char cBUFFER_V1[6000];
	memset(cBUFFER_V1, 0, sizeof(cBUFFER_V1));
	int iSize = MAKE_NUMBERW(lpRecvMsg->h.sizeH, lpRecvMsg->h.sizeL);
	int iBodySize = iSize - 0x10;	// #error

	if ( iBodySize < 0 )
		return;

	if ( iBodySize > 6000 )
		return;

	if ( lpRecvMsg->btCount < 0 || lpRecvMsg->btCount > 100 )
		return;

	PMSG_UNIONLIST_COUNT * lpMsg = (PMSG_UNIONLIST_COUNT *)cBUFFER_V1;
	PMSG_UNIONLIST * lpMsgBody = (PMSG_UNIONLIST *)(cBUFFER_V1+sizeof(PMSG_UNIONLIST_COUNT));
	lpMsg->btResult = lpRecvMsg->btResult;
	lpMsg->btCount = lpRecvMsg->btCount;
	lpMsg->btRivalMemberNum = lpRecvMsg->btRivalMemberNum;
	lpMsg->btUnionMemberNum = lpRecvMsg->btUnionMemberNum;

	for (int i=0;i<lpMsg->btCount;i++)
	{
		lpMsgBody[i].btMemberNum = lpRecvMsgBody[i].btMemberNum;
		memcpy(lpMsgBody[i].szGuildName, lpRecvMsgBody[i].szGuildName, sizeof(lpMsgBody->szGuildName));
		memcpy(lpMsgBody[i].Mark, lpRecvMsgBody[i].Mark, sizeof(lpMsgBody->Mark));
	}

	PHeadSetW((LPBYTE)lpMsg, 0xE9, lpMsg->btCount * sizeof(PMSG_UNIONLIST) + 0x10);	// #warning Check plus operation ( why 0x10 ??? )
	IOCP.DataSend(lpRecvMsg->wRequestUserIndex, (LPBYTE)lpMsg, MAKE_NUMBERW(lpMsg->h.sizeH, lpMsg->h.sizeL));
}



struct EXSDHP_KICKOUT_UNIONMEMBER_REQ
{
	PBMSG_HEAD2 h;
	WORD wRequestUserIndex;	// 4
	BYTE btRelationShipType;	// 6
	char szUnionMasterGuildName[8];	// 7
	char szUnionMemberGuildName[8];	// F
};


void GDRelationShipReqKickOutUnionMember(int aIndex, LPSTR szTargetGuildName)
{
	if ( gObj[aIndex].Type != OBJ_USER )
	{
		return;
	}

	_GUILD_INFO_STRUCT * lpGuildInfo = gObj[aIndex].m_PlayerData->lpGuild;

	if ( lpGuildInfo == NULL )
	{
		return;
	}

	EXSDHP_KICKOUT_UNIONMEMBER_REQ pMsg = {0};
	pMsg.h.set((LPBYTE)&pMsg, 0xEB, 0x01, sizeof(EXSDHP_KICKOUT_UNIONMEMBER_REQ));
	pMsg.wRequestUserIndex = aIndex;
	pMsg.btRelationShipType = GUILD_RELATION_UNION;
	memcpy(pMsg.szUnionMasterGuildName, lpGuildInfo->Name, sizeof(pMsg.szUnionMasterGuildName));
	memcpy(pMsg.szUnionMemberGuildName, szTargetGuildName, sizeof(pMsg.szUnionMemberGuildName));

	wsExDbCli.DataSend((char*)&pMsg, pMsg.h.size);
}


struct PMSG_KICKOUT_UNIONMEMBER_RESULT
{
	PBMSG_HEAD2 h;
	BYTE btResult;	// 4
	BYTE btRequestType;	// 5
	BYTE btRelationShipType;	// 6
};

void DGRelationShipAnsKickOutUnionMember(EXSDHP_KICKOUT_UNIONMEMBER_RESULT * lpMsg)
{

	_GUILD_INFO_STRUCT * lpUnionMasterGuild = Guild.SearchGuild(lpMsg->szUnionMasterGuildName);
	_GUILD_INFO_STRUCT * lpUnionMemberGuild = Guild.SearchGuild(lpMsg->szUnionMemberGuildName);

	if ( lpMsg->btFlag == 1 )
	{
		if ( !lpUnionMasterGuild )
		{
			lpMsg->btResult = 0;
		}

		PMSG_KICKOUT_UNIONMEMBER_RESULT pMsg ={0};

		pMsg.h.set((LPBYTE)&pMsg, 0xEB, 0x01, sizeof(pMsg));
		pMsg.btResult = lpMsg->btResult;
		pMsg.btRequestType = 2;
		pMsg.btRelationShipType = 1;

		IOCP.DataSend(lpMsg->wRequestUserIndex, (LPBYTE)&pMsg, sizeof(pMsg));

		if ( lpMsg->btResult == 0 || lpMsg->btResult == 0x10 )
			return;
	}
	else if ( lpMsg->btFlag == 0 )
	{
		if ( !lpUnionMemberGuild )
			return;

		if ( lpMsg->btResult == 0 || lpMsg->btResult == 0x10 )
			return;
	}		

	if ( lpUnionMasterGuild )
	{
		for ( int n=0;n<MAX_USER_GUILD;n++)
		{
			if ( lpUnionMasterGuild->Use[n] > 0 )
			{
				if ( lpUnionMasterGuild->Index[n] >= 0 )
				{
					TNotice _Notice(1);
					_Notice.SendToUser(lpUnionMasterGuild->Index[n], Lang.GetText(0,186),
						lpMsg->szUnionMemberGuildName, lpMsg->szUnionMasterGuildName);
				}
			}
		}
	}

	g_Log.Add("[GS][U.System][Union][KickOut] ( Union Master Guild: %s ), (Union Member Guild: %s)",
		lpMsg->szUnionMasterGuildName, lpMsg->szUnionMemberGuildName);

	if ( !lpUnionMemberGuild )
		return;

	lpUnionMemberGuild->SetGuildUnion(0);

	for ( int n=0;n<MAX_USER_GUILD;n++)
	{
		if ( lpUnionMemberGuild->Use[n] > 0 )
		{
			if ( lpUnionMemberGuild->Index[n] >= 0 )
			{
				TNotice _Notice(1);
				_Notice.SendToUser(lpUnionMemberGuild->Index[n], Lang.GetText(0,185));
			}
		}
	}

}



struct FHP_FRIENDLIST_REQ
{
	PBMSG_HEAD h;
	short Number;	// 4
	char Name[10];	// 6
	BYTE pServer;	// 10
};



void FriendListRequest(int aIndex)
{
	if ( gObjIsConnectedGP(aIndex) == FALSE )
	{
		g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	if (gObj[aIndex].m_FriendServerOnline == 2 )
	{
		g_Log.Add("error-L3 : [%s][%s] FriendServer Online Request ", gObj[aIndex].AccountID, gObj[aIndex].Name);
		return;
	}

	if (gObj[aIndex].m_FriendServerOnline == FRIEND_SERVER_STATE_OFFLINE )
	{
		g_Log.Add("error-L3 : [%s][%s] FriendServer Offline", gObj[aIndex].AccountID, gObj[aIndex].Name);
		return;
	}

	FHP_FRIENDLIST_REQ pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x60, sizeof(FHP_FRIENDLIST_REQ));
	memcpy(pMsg.Name, gObj[aIndex].Name, sizeof(pMsg.Name));
	pMsg.Number = aIndex;
	pMsg.pServer = g_ConfigRead.server.GetGameServerCode();
	
	wsExDbCli.DataSend((char*)&pMsg, pMsg.h.size);

	g_Log.Add("[%s] Friend List Request", gObj[aIndex].Name);

	gObj[aIndex].m_FriendServerOnline = 2;
	gObj[aIndex].m_PlayerData->m_nServerChannel = g_ConfigRead.server.GetGameServerCode();

}


struct FHP_FRIENDLIST
{
	char Name[10];	// 0
	BYTE Server;	// A
};

struct FHP_FRIENDLIST_COUNT
{
	PWMSG_HEAD h;
	short Number;	// 4
	char Name[10];	// 6
	BYTE Count;// 10
	BYTE MailCount;	// 11
	BYTE Server;
};

struct PMSG_FRIEND_LIST_COUNT
{
	PWMSG_HEAD h;
	BYTE MemoCount;	// 4
	BYTE MailTotal;	// 5
	BYTE Count;	// 6
};

void FriendListResult(LPBYTE lpMsg)
{

	FHP_FRIENDLIST_COUNT * lpCount;
	FHP_FRIENDLIST * lpList;
	int lOfs = sizeof(FHP_FRIENDLIST_COUNT);
	PMSG_FRIEND_LIST_COUNT pCount;
	int pOfs = sizeof(PMSG_FRIEND_LIST_COUNT);

	lpCount = (FHP_FRIENDLIST_COUNT *)lpMsg;
	lpList  = (FHP_FRIENDLIST *)(&lpMsg[lOfs * sizeof(FHP_FRIENDLIST)]);

	char_ID Name( lpCount->Name);
	BYTE TmpSendBuf[2000];
	
	if ( !gObjIsConnectedGP(lpCount->Number, Name.GetBuffer()))
	{
		g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	gObj[lpCount->Number].m_FriendServerOnline = 0;

	pCount.Count = lpCount->Count;
	pCount.MemoCount = lpCount->MailCount;
	pCount.MailTotal = 50;

	if ( pCount.Count != 0 )
	{
		for ( int n=0;n<pCount.Count;n++)
		{
			lpList = (FHP_FRIENDLIST *)&lpMsg[lOfs];
			memcpy(&TmpSendBuf[pOfs], lpList, sizeof(FHP_FRIENDLIST));
			lOfs+=sizeof(FHP_FRIENDLIST);
			pOfs+=sizeof(FHP_FRIENDLIST);	// #warning why plus that??
		}
	}

	pCount.h.set((LPBYTE)&pCount, 0xC0, pOfs);
	memcpy(TmpSendBuf, &pCount, sizeof(PMSG_FRIEND_LIST_COUNT));

	IOCP.DataSend(lpCount->Number, TmpSendBuf, pOfs);

	g_Log.Add("[%s] Friend List Count (%d) Send",
		gObj[lpCount->Number].Name, pCount.Count);
}

struct PMSG_FRIEND_ADD_SIN_REQ
{
	PBMSG_HEAD h;
	char Name[10];	// 3
};


void WaitFriendListResult(FHP_WAITFRIENDLIST_COUNT * lpMsg)
{
	PMSG_FRIEND_ADD_SIN_REQ pMsg;
	char_ID Name(lpMsg->Name);

	if ( !gObjIsConnectedGP(lpMsg->Number, Name.GetBuffer()))
	{
		g_Log.Add("error-L2 : (%s) Index %s %d ",
			Name.GetBuffer(), __FILE__, __LINE__);
		return;
	}

	if ( Name.GetLength() < 1 )
		return;

	pMsg.h.set((LPBYTE)&pMsg, 0xC2, sizeof(pMsg));
	memcpy(pMsg.Name, lpMsg->FriendName, sizeof(pMsg.Name));

	IOCP.DataSend(lpMsg->Number, (LPBYTE)&pMsg, sizeof(pMsg));

	g_Log.Add("[%s] WaitFriend List [%s]", gObj[lpMsg->Number].Name, Name.GetBuffer());
}


struct FHP_FRIEND_STATE_C
{
	PBMSG_HEAD h;
	short Number;	// 4
	char Name[10];	// 6
	BYTE State;	// 10
};

void FriendStateClientRecv(PMSG_FRIEND_STATE_C * lpMsg, int aIndex)
{
	if ( !gObjIsConnectedGP(aIndex))
	{
		g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	FHP_FRIEND_STATE_C pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x62, sizeof(pMsg));
	memcpy(pMsg.Name, gObj[aIndex].Name, sizeof(pMsg.Name));
	pMsg.State = lpMsg->ChatVeto;

	wsExDbCli.DataSend((PCHAR)&pMsg, pMsg.h.size);
}



void FriendStateRecv(FHP_FRIEND_STATE * lpMsg)
{
	char_ID Name(lpMsg->Name);

	if ( !gObjIsConnectedGP(lpMsg->Number, Name.GetBuffer()))
	{
		g_Log.Add("error-L2 : (%s) Index %s %d ", Name.GetBuffer(), __FILE__, __LINE__);
		return;
	}

	PMSG_FRIEND_STATE pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0xC4, sizeof(pMsg));
	memcpy(pMsg.Name, lpMsg->FriendName, sizeof(pMsg.Name));
	pMsg.State = lpMsg->State;

	IOCP.DataSend(lpMsg->Number, (LPBYTE)&pMsg, sizeof(pMsg));

	g_Log.Add("[%s] Friend State (%d)", gObj[lpMsg->Number].Name, lpMsg->State);
}



struct FHP_FRIEND_ADD_REQ
{
	PBMSG_HEAD h;
	short Number;	// 4
	char Name[10];	// 6
	char FriendName[10];	// 10
};


struct PMSG_FRIEND_ADD_RESULT
{
	PBMSG_HEAD h;
	unsigned char Result;	// 3
	char Name[10];	// 4
	BYTE State;	// E
};


void FriendAddRequest(PMSG_FRIEND_ADD_REQ * lpMsg, int aIndex)
{
	if ( !gObjIsConnectedGP(aIndex))
	{
		g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	//Season 4.5 Addon start
    int aTargetIndex = gObjGetIndex(lpMsg->Name);
 
    if(ObjectMaxRange(aTargetIndex) == true && gObj[aTargetIndex].Type != OBJ_USER  )
    {
		g_Log.AddC(TColor::Red,"[HACKTOOL] : NPC-FriendAddRequest npc:%d ip:%s account:%s name:%s State:%d",
			gObj[aTargetIndex].Class,gObj[aIndex].m_PlayerData->Ip_addr,gObj[aIndex].AccountID,
			gObj[aIndex].Name,gObj[aIndex].Connected);
		GSProtocol.GCSendDisableReconnect(aIndex);
        //IOCP.CloseClient(aIndex);
        return;
    }
    //Season 4.5 Addon end

	FHP_FRIEND_ADD_REQ pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x63, sizeof(pMsg));
	memcpy(pMsg.Name, gObj[aIndex].Name, sizeof(pMsg.Name));
	memcpy(pMsg.FriendName, lpMsg->Name, sizeof(pMsg.FriendName));
	pMsg.Number = aIndex;
	char_ID szFriendName(lpMsg->Name);
	
	g_Log.Add("[%s] Friend add request Name: [%s]", gObj[aIndex].Name,		szFriendName.GetBuffer());

	if ( !strcmp(gObj[aIndex].Name, szFriendName.GetBuffer()))
	{
		g_Log.Add("error-L3 : [%s] Friend add : Not self", gObj[aIndex].Name);
		
		PMSG_FRIEND_ADD_RESULT pResult;

		pResult.h.set((LPBYTE)&pResult, 0xC1, sizeof(pResult));
		memcpy(pResult.Name, lpMsg->Name, sizeof(pMsg.Name));
		pResult.Result = 5;
		pResult.State = -1;

		IOCP.DataSend(aIndex, (LPBYTE)&pResult, sizeof(pResult));
	}
	else
	{
		wsExDbCli.DataSend((PCHAR)&pMsg, pMsg.h.size);
	}
}		



void FriendAddResult(FHP_FRIEND_ADD_RESULT * lpMsg)
{
	char_ID Name(lpMsg->Name);

	if ( !gObjIsConnectedGP(lpMsg->Number, Name.GetBuffer()))
	{
		g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	PMSG_FRIEND_ADD_RESULT pMsg;
	char_ID szFriendName(lpMsg->FriendName);

	pMsg.h.set((LPBYTE)&pMsg, 0xC1, sizeof(pMsg));
	memcpy(pMsg.Name, lpMsg->FriendName, sizeof(pMsg.Name));
	pMsg.Result = lpMsg->Result;
	pMsg.State = lpMsg->Server;

	IOCP.DataSend(lpMsg->Number, (LPBYTE)&pMsg, sizeof(pMsg));

	g_Log.Add("[%s] Friend add result : (%d) friend Name : (%s)",
		Name.GetBuffer(), lpMsg->Result, szFriendName.GetBuffer());
}


struct FHP_WAITFRIEND_ADD_REQ
{
	PBMSG_HEAD h;
	unsigned char Result;	// 3
	short Number;	// 4
	char Name[10];	// 6
	char FriendName[10];	// 10
};

void WaitFriendAddRequest(PMSG_FRIEND_ADD_SIN_RESULT * lpMsg, int aIndex)
{
	if ( !gObjIsConnectedGP(aIndex))
	{
		g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	FHP_WAITFRIEND_ADD_REQ pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x64, sizeof(pMsg));
	memcpy(pMsg.Name, gObj[aIndex].Name, sizeof(pMsg.Name));
	memcpy(pMsg.FriendName, lpMsg->Name, sizeof(pMsg.FriendName));
	pMsg.Result = lpMsg->Result;
	pMsg.Number = aIndex;

	wsExDbCli.DataSend((PCHAR)&pMsg, pMsg.h.size);

	{
		char_ID fname(lpMsg->Name);
		g_Log.Add("[%s] WaitFriend add request [%s]", gObj[aIndex].Name, fname.GetBuffer());
	}
}



void WaitFriendAddResult(FHP_WAITFRIEND_ADD_RESULT * lpMsg)
{
	char_ID Name(lpMsg->Name);

	if ( !gObjIsConnectedGP(lpMsg->Number, Name.GetBuffer()))
	{
		g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	PMSG_FRIEND_ADD_RESULT pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0xC1, sizeof(pMsg));
	memcpy(pMsg.Name, lpMsg->FriendName, sizeof(pMsg.Name));
	pMsg.Result = lpMsg->Result;
	pMsg.State = lpMsg->pServer;

	char_ID szFriendName(lpMsg->FriendName);

	g_Log.Add("[%s] WaitFriend add result (%d) [%s]",
		Name.GetBuffer(), lpMsg->Result, szFriendName.GetBuffer());

	IOCP.DataSend(lpMsg->Number, (LPBYTE)&pMsg, sizeof(pMsg));
}



void FriendDelRequest(PMSG_FRIEND_DEL_REQ * lpMsg, int aIndex)
{
	if ( !gObjIsConnectedGP(aIndex))
	{
		g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	FHP_FRIEND_ADD_REQ pMsg;
	char_ID Name(lpMsg->Name);

	pMsg.h.set((LPBYTE)&pMsg, 0x65, sizeof(pMsg));
	memcpy(pMsg.Name, gObj[aIndex].Name, sizeof(pMsg.Name));
	memcpy(pMsg.FriendName, Name.GetBuffer(), sizeof(pMsg.FriendName));
	pMsg.Number = aIndex;

	wsExDbCli.DataSend((PCHAR)&pMsg, pMsg.h.size);

	g_Log.Add("[%s] Friend del request [%s]", gObj[aIndex].Name, Name.GetBuffer());
}


struct PMSG_FRIEND_DEL_RESULT
{
	PBMSG_HEAD h;
	unsigned char Result;	// 3
	char Name[10];	// 4
};

void FriendDelResult(FHP_FRIEND_DEL_RESULT * lpMsg)
{
	char_ID Name(lpMsg->Name);

	if ( !gObjIsConnectedGP(lpMsg->Number, Name.GetBuffer()))
	{
		g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	PMSG_FRIEND_DEL_RESULT pMsg;
	char_ID FriendName(lpMsg->FriendName);

	pMsg.h.set((LPBYTE)&pMsg, 0xC3, sizeof(pMsg));
	memcpy(pMsg.Name, lpMsg->FriendName, sizeof(pMsg.Name));
	pMsg.Result = lpMsg->Result;

	g_Log.Add("[%s] Friend del result (%d) [%s]",
		Name.GetBuffer(), lpMsg->Result, FriendName.GetBuffer());

	IOCP.DataSend(lpMsg->Number, (LPBYTE)&pMsg, sizeof(pMsg));
}

struct FHP_FRIEND_CHATROOM_CREATE_REQ
{
	PBMSG_HEAD h;
	short Number;	// 4
	char Name[10];	// 6
	char fName[10];	// 10
};


void FriendChatRoomCreateReq(PMSG_FRIEND_ROOMCREATE_REQ * lpMsg, int aIndex)
{
	if ( !gObjIsConnectedGP(aIndex))
	{
		g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	FHP_FRIEND_CHATROOM_CREATE_REQ pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x66, sizeof(pMsg));
	memcpy(pMsg.Name, gObj[aIndex].Name, sizeof(pMsg.Name));
	pMsg.Number = aIndex;
	memcpy(pMsg.fName, lpMsg->Name, sizeof(pMsg.fName));
	
	wsExDbCli.DataSend((PCHAR)&pMsg, pMsg.h.size);

	char_ID szName(lpMsg->Name);

	g_Log.Add("[%s] Chatroom create request [%s]", gObj[aIndex].Name, szName.GetBuffer());
}


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


void FriendChatRoomCreateResult(FHP_FRIEND_CHATROOM_CREATE_RESULT * lpMsg)
{
	char_ID Name(lpMsg->Name);

	if ( !gObjIsConnectedGP(lpMsg->Number, Name.GetBuffer()))
	{
		g_Log.Add("error-L2 : Index %s %d (%s)", __FILE__, __LINE__, Name.GetBuffer());
		return;
	}

	PMSG_FRIEND_ROOMCREATE_RESULT pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0xCA, sizeof(pMsg));
	memcpy(pMsg.ServerIp, lpMsg->ServerIp, sizeof(pMsg.ServerIp));
	pMsg.RoomNumber = lpMsg->RoomNumber;
	pMsg.Ticket = lpMsg->Ticket;
	pMsg.Type = lpMsg->Type;
	pMsg.Result = lpMsg->Result;
	memcpy(pMsg.Name, lpMsg->FriendName, sizeof(pMsg.Name));

	IOCP.DataSend(lpMsg->Number, (LPBYTE)&pMsg, sizeof(pMsg));

	g_Log.Add("[%s] Chatroom create result (%d) ticket:(%d)",
		gObj[lpMsg->Number].Name, lpMsg->Result, lpMsg->Ticket);
}



struct FHP_FRIEND_MEMO_SEND
{
	PWMSG_HEAD h;
	short Number;	// 4
	DWORD WindowGuid;	// 8
	char Name[10];	// C
	char ToName[10];	// 16
	char Subject[60];	// 20
	BYTE Dir;	// 40
	BYTE Action;	// 41
	short MemoSize;	// 42
	BYTE Photo[18];	// 44
	char Memo[1000];	// 56
};

void FriendMemoSend(PMSG_FRIEND_MEMO * lpMsg, int aIndex)
{
	if ( !gObjIsConnectedGP(aIndex))
	{
		g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	char_ID Name(lpMsg->Name);

	if ( gObj[aIndex].m_PlayerData->Money < 1000 )
	{
		FHP_FRIEND_MEMO_SEND_RESULT pResult;

		pResult.Number = aIndex;
		pResult.Result = 7;
		pResult.WindowGuid = lpMsg->WindowGuid;
		memcpy(pResult.Name, gObj[aIndex].Name, sizeof(pResult.Name));

		FriendMemoSendResult(&pResult);

		return;
	}

	if ( lpMsg->MemoSize < 0 || lpMsg->MemoSize > 1000 )
	{
		FHP_FRIEND_MEMO_SEND_RESULT pResult;

		pResult.Number = aIndex;
		pResult.Result = 0;
		pResult.WindowGuid = lpMsg->WindowGuid;
		memcpy(pResult.Name, gObj[aIndex].Name, sizeof(pResult.Name));

		FriendMemoSendResult(&pResult);

		return;
	}

	if ( Name.GetLength() < 1 )
	{
		FHP_FRIEND_MEMO_SEND_RESULT pResult;

		pResult.Number = aIndex;
		pResult.Result = 0;
		pResult.WindowGuid = lpMsg->WindowGuid;
		memcpy(pResult.Name, gObj[aIndex].Name, sizeof(pResult.Name));

		FriendMemoSendResult(&pResult);

		return;
	}

	FHP_FRIEND_MEMO_SEND pMsg;
	int bsize = lpMsg->MemoSize + sizeof(pMsg) - sizeof(pMsg.Memo);

	pMsg.h.set((LPBYTE)&pMsg, 0x70, bsize);
	pMsg.WindowGuid = lpMsg->WindowGuid;
	pMsg.MemoSize = lpMsg->MemoSize;
	pMsg.Number = aIndex;
	pMsg.Dir = lpMsg->Dir;
	pMsg.Action = lpMsg->Action;
	memcpy(pMsg.Subject, lpMsg->Subject, sizeof(pMsg.Subject));
	memcpy(pMsg.Name, gObj[aIndex].Name, sizeof(pMsg.Name));
	memcpy(pMsg.Photo, gObj[aIndex].CharSet, sizeof(pMsg.Photo));
	memcpy(pMsg.ToName, lpMsg->Name, sizeof(pMsg.ToName));
	memcpy(pMsg.Memo, lpMsg->Memo, lpMsg->MemoSize);

	wsExDbCli.DataSend((PCHAR)&pMsg, bsize);

	g_Log.Add("[%s] Friend mail send %s (Size:%d)", gObj[aIndex].Name, Name.GetBuffer(), bsize);
}



void MngFriendMemoSend(PMSG_JG_MEMO_SEND * lpMsg)
{
	char_ID Name(lpMsg->Name);

	if ( lpMsg->MemoSize < 0 || lpMsg->MemoSize > 1000 )
		return;

	if ( Name.GetLength() < 1 )
		return;

	FHP_FRIEND_MEMO_SEND pMsg;

	int bsize = lpMsg->MemoSize + sizeof(pMsg) - sizeof(pMsg.Memo);

	pMsg.h.set((LPBYTE)&pMsg, 0x70, bsize);
	pMsg.WindowGuid = 0;
	pMsg.MemoSize = lpMsg->MemoSize;
	pMsg.Number = -1;
	pMsg.Dir = 0;
	pMsg.Action = 0;
	memcpy(pMsg.Subject, lpMsg->Subject, sizeof(pMsg.Subject));
	memcpy(pMsg.Name, lpMsg->Name , sizeof(pMsg.Name));
	memset(pMsg.Photo, 0, sizeof(pMsg.Photo));
	memcpy(pMsg.ToName, lpMsg->TargetName, sizeof(pMsg.ToName));
	memcpy(pMsg.Memo, lpMsg->Memo, lpMsg->MemoSize);

	wsExDbCli.DataSend((PCHAR)&pMsg, bsize);

	g_Log.Add("JoinServer mail send %s (Size:%d)", Name.GetBuffer(), bsize);
}



BOOL WithdrawUserMoney(LPSTR Type, LPOBJ lpObj, int Withdraw_Money)
{
	int oldmoney;

	if ( (lpObj->m_PlayerData->Money - Withdraw_Money) >= 0 )
	{
		oldmoney = lpObj->m_PlayerData->Money;
		lpObj->m_PlayerData->Money -= Withdraw_Money;

		GSProtocol.GCMoneySend(lpObj->m_Index, lpObj->m_PlayerData->Money);
		
		g_Log.Add("[%s][%s] (%s) Pay Money(In Inventory) : %d - %d = %d",
			lpObj->AccountID, lpObj->Name, Type, oldmoney, Withdraw_Money, lpObj->m_PlayerData->Money);

		return TRUE;
	}

	return FALSE;
}


struct PMSG_FRIEND_MEMO_RESULT
{
	PBMSG_HEAD h;
	BYTE Result;	// 3
	DWORD WindowGuid;	// 4
};

void FriendMemoSendResult(FHP_FRIEND_MEMO_SEND_RESULT * lpMsg)
{
	char_ID Name(lpMsg->Name);

	if ( !strcmp(Name.GetBuffer(), "webzen"))	// #warning Deathway
	{
		g_Log.Add("JoinServer Send Mail result : (%d)", lpMsg->Result);
		return;
	}

	if ( !gObjIsConnectedGP(lpMsg->Number, Name.GetBuffer()))
	{
		g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	PMSG_FRIEND_MEMO_RESULT pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0xC5, sizeof(pMsg));
	pMsg.Result = lpMsg->Result;
	pMsg.WindowGuid = lpMsg->WindowGuid;

	IOCP.DataSend(lpMsg->Number, (LPBYTE)&pMsg, sizeof(pMsg));

	g_Log.Add("[%s] Friend mail Send result : (%d) guid:(%d)",
		gObj[lpMsg->Number].Name, lpMsg->Result, lpMsg->WindowGuid);

	if ( pMsg.Result == 1 )
	{
		WithdrawUserMoney("Mail", &gObj[lpMsg->Number], 1000);
	}
}

struct FHP_FRIEND_MEMO_LIST_REQ
{
	PBMSG_HEAD h;
	WORD Number;	// 4
	char Name[10];	// 6
};


void FriendMemoListReq(int aIndex)
{
	if ( !gObjIsConnectedGP(aIndex))
	{
		g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	FHP_FRIEND_MEMO_LIST_REQ pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x71, sizeof(pMsg));
	pMsg.Number = aIndex;
	memcpy(pMsg.Name, gObj[aIndex].Name, sizeof(pMsg.Name));

	wsExDbCli.DataSend((PCHAR)&pMsg, pMsg.h.size);

	g_Log.Add("[%s] Friend mail list request", gObj[aIndex].Name);
}

struct PMSG_FRIEND_MEMO_LIST
{
	PBMSG_HEAD h;
	WORD Number;	// 4
	char Name[10];	// 6
	char Date[30];	// 10
	char Subject[60];	// 2E
	unsigned char read;	// 4E
};


void FriendMemoList(FHP_FRIEND_MEMO_LIST * lpMsg)
{
	char_ID SendName(lpMsg->SendName);
	char_ID RecvName(lpMsg->RecvName);

	if ( !gObjIsConnectedGP(lpMsg->Number, RecvName.GetBuffer()))
	{
		g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	int memoindex;
	char subject[61]="";
	char date[31]="";

	memoindex = lpMsg->MemoIndex;
	memcpy(subject, lpMsg->Subject, sizeof(lpMsg->Subject));
	memcpy(date, lpMsg->Date, sizeof(lpMsg->Date));

	PMSG_FRIEND_MEMO_LIST pMsg;

	PHeadSetBE((LPBYTE)&pMsg, 0xC6, sizeof(pMsg));
	memcpy(pMsg.Date, lpMsg->Date, sizeof(pMsg.Date));
	memcpy(pMsg.Name, lpMsg->SendName, sizeof(pMsg.Name));
	memcpy(pMsg.Subject, lpMsg->Subject, sizeof(pMsg.Subject));
	pMsg.Number = memoindex;
	pMsg.read = lpMsg->read;

	IOCP.DataSend(lpMsg->Number, (LPBYTE)&pMsg, sizeof(pMsg));
	
	g_Log.Add("[%s] Friend Mail list : (%d:%10s:%s:%s)",
		SendName.GetBuffer(), memoindex, date, RecvName.GetBuffer(), subject);
}

struct FHP_FRIEND_MEMO_RECV_REQ
{
	PBMSG_HEAD h;
	short Number;	// 4
	WORD MemoIndex;	// 6
	char Name[10];	// 8
};


void FriendMemoReadReq(PMSG_FRIEND_READ_MEMO_REQ * lpMsg, int aIndex)
{
	if ( !gObjIsConnectedGP(aIndex))
	{
		g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	FHP_FRIEND_MEMO_RECV_REQ pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x72, sizeof(pMsg));
	pMsg.MemoIndex = lpMsg->MemoIndex;
	pMsg.Number = aIndex;
	memcpy(pMsg.Name, gObj[aIndex].Name, sizeof(pMsg.Name));

	wsExDbCli.DataSend((PCHAR)&pMsg, pMsg.h.size);

	g_Log.Add("[%s] Friend Mail read Index:%d", 
		gObj[aIndex].Name, lpMsg->MemoIndex);

}

struct PMSG_FRIEND_READ_MEMO
{
	PWMSG_HEAD h;
	WORD MemoIndex;	// 4
	short MemoSize;	// 6
	BYTE Photo[18];	// 8
	BYTE Dir;	// 1A
	BYTE Action;	//1B
	char Memo[1000];	// 1C
};


void FriendMemoRead(FHP_FRIEND_MEMO_RECV * lpMsg)
{
	char_ID Name(lpMsg->Name);

	if ( !gObjIsConnectedGP(lpMsg->Number, Name.GetBuffer()))
	{
		g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	PMSG_FRIEND_READ_MEMO pMsg;

	int nsize = sizeof(pMsg)-1000;

	if ( lpMsg->MemoSize < 0 || lpMsg->MemoSize > 1000 )
	{
		g_Log.Add("error-L2 : Friend Memo Max %s %d", __FILE__, __LINE__);
		return;
	}

	nsize += lpMsg->MemoSize;

	pMsg.h.setE((LPBYTE)&pMsg, 0xC7, nsize);
	pMsg.MemoIndex = lpMsg->MemoIndex;
	pMsg.MemoSize = lpMsg->MemoSize;
	pMsg.Dir = lpMsg->Dir;
	pMsg.Action = lpMsg->Action;
	memcpy(pMsg.Photo, lpMsg->Photo,sizeof(pMsg.Photo));
	memcpy(pMsg.Memo, lpMsg->Memo, lpMsg->MemoSize);

	IOCP.DataSend(lpMsg->Number, (LPBYTE)&pMsg, nsize);

	g_Log.Add("[%s] Friend Mail read (%d)",
		gObj[lpMsg->Number].Name, lpMsg->MemoIndex);
}

struct FHP_FRIEND_MEMO_DEL_REQ
{
	PBMSG_HEAD h;
	short Number;	// 4
	WORD MemoIndex;	// 6
	char Name[10];	// 8
};


void FriendMemoDelReq(PMSG_FRIEND_MEMO_DEL_REQ * lpMsg, int aIndex)
{
	if ( !gObjIsConnectedGP(aIndex))
	{
		g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	FHP_FRIEND_MEMO_DEL_REQ pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x73, sizeof(pMsg));
	pMsg.MemoIndex = lpMsg->MemoIndex;
	pMsg.Number = aIndex;
	memcpy(pMsg.Name, gObj[aIndex].Name, sizeof(pMsg.Name));

	wsExDbCli.DataSend((PCHAR)&pMsg, pMsg.h.size);

	g_Log.Add("[%s] Friend mail delete request Index:(%d)",
		gObj[aIndex].Name, lpMsg->MemoIndex);
}


struct PMSG_FRIEND_MEMO_DEL_RESULT
{
	PBMSG_HEAD h;
	unsigned char Result;	// 3
	WORD MemoIndex;	// 4
};

void FriendMemoDelResult(FHP_FRIEND_MEMO_DEL_RESULT * lpMsg)
{
	char_ID Name(lpMsg->Name);

	if ( !gObjIsConnectedGP(lpMsg->Number, Name.GetBuffer()))
	{
		g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	PMSG_FRIEND_MEMO_DEL_RESULT pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0xC8, sizeof(pMsg));
	pMsg.MemoIndex = lpMsg->MemoIndex;
	pMsg.Result = lpMsg->Result;

	IOCP.DataSend(lpMsg->Number, (LPBYTE)&pMsg, sizeof(pMsg));

	g_Log.Add("[%s] Friend mail delete request (%d) index:(%d)",
		Name.GetBuffer(), lpMsg->Result, lpMsg->MemoIndex);
}


struct FHP_FRIEND_INVITATION_REQ
{
	PBMSG_HEAD h;
	short Number;	// 4
	char Name[10];	// 6
	char FriendName[10];	// 10
	WORD RoomNumber;	// 1A
	DWORD WindowGuid;	// 1C
};

void FriendRoomInvitationReq(PMSG_ROOM_INVITATION * lpMsg, int aIndex)
{
	if ( !gObjIsConnectedGP(aIndex))
	{
		g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	FHP_FRIEND_INVITATION_REQ pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0x74, sizeof(pMsg));
	pMsg.Number = aIndex;
	pMsg.RoomNumber = lpMsg->RoomNumber;
	pMsg.WindowGuid = lpMsg->WindowGuid;
	memcpy(pMsg.FriendName, lpMsg->Name, sizeof(pMsg.FriendName));
	memcpy(pMsg.Name, gObj[aIndex].Name, sizeof(pMsg.Name));

	wsExDbCli.DataSend((PCHAR)&pMsg, pMsg.h.size);

	g_Log.Add("[%s] Friend Invistation request room:%d winguid:%d",
		gObj[aIndex].Name, lpMsg->RoomNumber, lpMsg->WindowGuid);
}



struct PMSG_ROOM_INVITATION_RESULT
{
	PBMSG_HEAD h;
	unsigned char Result;	// 3
	DWORD WindowGuid;	// 4
};

void FriendRoomInvitationRecv(FHP_FRIEND_INVITATION_RET* lpMsg)
{
	char_ID Name(lpMsg->Name);

	if ( !gObjIsConnectedGP(lpMsg->Number, Name.GetBuffer()))
	{
		g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	PMSG_ROOM_INVITATION_RESULT pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0xCB, sizeof(pMsg));
	pMsg.Result = lpMsg->Result;
	pMsg.WindowGuid = lpMsg->WindowGuid;

	IOCP.DataSend(lpMsg->Number, (LPBYTE)&pMsg, sizeof(pMsg));

	g_Log.Add("[%s] Friend Invitation result :%d",
		Name.GetBuffer(), pMsg.Result);
}

void DGAnsGensInfo(_tagPMSG_ANS_GENS_INFO_EXDB *lpMsg)
{
	int iObjIndex = MAKE_NUMBERW(lpMsg->wIndexH, lpMsg->wIndexL);

	LPOBJ lpObj = &gObj[iObjIndex];

	if (gObjIsConnectedGP(iObjIndex))
	{
		if (lpMsg->iResult)
		{
			g_GensSystem.SetUserBattleZoneEnable(lpObj, BATTLE_ZONE_BLOCK);
			g_GensSystem.SetGensInfluence(lpObj, NONE_INFLUENCE);
			g_GensSystem.SetContributePoint(lpObj, 0);
		}
		else
		{
			if (lpMsg->bInfluence != LEAVED_INFLUENCE && lpMsg->bInfluence)
			{
				g_GensSystem.SetUserBattleZoneEnable(lpObj, BATTLE_ZONE_UNBLOCK);
			}

			g_GensSystem.SetGensInfluence(lpObj, lpMsg->bInfluence);
			g_GensSystem.SetContributePoint(lpObj, lpMsg->iContributePoint);
			g_GensSystem.SetGensRanking(lpObj, lpMsg->iGensRanking);
			g_GensSystem.SetGensClass(lpObj, lpMsg->iGensClass);
			g_GensSystem.GensViewportListProtocol(lpObj);
		}

		g_Log.Add("[GensSystem] Get DB GensInfo [%s][%s] GensName [%s]", lpObj->AccountID, lpObj->Name, g_GensSystem.GetGensInfluenceName(lpObj));
	
		g_GensSystem.SendGensInfo(lpObj);

		if (g_GensSystem.IsMapBattleZone(lpObj->MapNumber))
		{
			if (!g_GensSystem.IsUserBattleZoneEnable(lpObj))
			{
				gObjTeleport(lpObj->m_Index, 0, 140, 125);
				GSProtocol.GCServerMsgStringSend(Lang.GetText(0,423), lpObj->m_Index, 1);
			}
		}
	}
	else
	{
		g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
	}
}

//-> Completed
void DGAnsRegGensMember(_tagPMSG_ANS_REG_GENS_MEMBER_EXDB *lpMsg)
{
	int iObjIndex = MAKE_NUMBERW(lpMsg->bIndexH, lpMsg->bIndexL);

	LPOBJ lpObj = &gObj[iObjIndex];

	if (gObjIsConnectedGP(lpObj->m_Index))
	{
		_tagPMSG_ANS_REG_GENS_MEBMER pMsg;

		if (!lpMsg->bResult)
		{
			g_GensSystem.SetGensInfluence(lpObj, lpMsg->bInfluence);
			g_GensSystem.SetUserBattleZoneEnable(lpObj, BATTLE_ZONE_UNBLOCK);
			g_GensSystem.GensViewportListProtocol(lpObj);
			g_GensSystem.SetContributePoint(lpObj, g_GensSystem.GetInitialContributePoint());
	 		g_GensSystem.GDReqSaveContributePoint(lpObj);
		}

		PHeadSubSetBE((LPBYTE)&pMsg, 0xF8, 0x02, sizeof(pMsg));
		pMsg.bResult = lpMsg->bResult; 
		pMsg.iInfluence = lpMsg->bInfluence;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		
		switch (lpMsg->bResult)
		{
		case 0:
			g_Log.Add("[GensSystem] [%s][%s] Gens Request - Join [%s]", lpObj->AccountID, lpObj->Name, g_GensSystem.GetGensInfluenceName(lpObj));
	 		break;

		case 1:
			g_Log.Add("[GensSystem] [%s][%s] Gens Joining Failed to Gens be Same : [%s]", lpObj->AccountID, lpObj->Name, g_GensSystem.GetGensInfluenceName(lpObj));
			break;

		case 2:
			g_Log.Add("[GensSystem] [%s][%s] Gens Joining Failed to Below Gens leaving Time", lpObj->AccountID, lpObj->Name);
			break;

		case 3:
			g_Log.Add("[GensSystem] [%s][%s] Gens Joining Failed to Below 50Lv", lpObj->AccountID, lpObj->Name);
			break;

		case 4:
			g_Log.Add("[GensSystem] [%s][%s] Gens Joining Failed to Gens be Different Guild Leader", lpObj->AccountID, lpObj->Name);
			break;

		case 5:
			g_Log.Add("[GensSystem] [%s][%s] Gens Joining Failed to Not Register Guild Leader GuildName [%s]", lpObj->AccountID, lpObj->Name, lpObj->m_PlayerData->GuildName);      
			break;

		case 6:
			g_Log.Add("[GensSystem] [%s][%s] Gens Joining Failed to Already Partymember", lpObj->AccountID, lpObj->Name);
			break;

		case 7:
			g_Log.Add("[GensSystem] [%s][%s] Gens Joining Failed to Union GuildMaster", lpObj->AccountID, lpObj->Name);
			break;

		default:
			return;
		}
	}
	else
	{
		g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
	}
}

//-> Completed
void DGAnsSecedeGensMember(_tagPMSG_ANS_SECEDE_GENS_MEMBER_EXDB *lpMsg)
{
	int iObjIndex = MAKE_NUMBERW(lpMsg->bIndexH, lpMsg->bIndexL);

	LPOBJ lpObj = &gObj[iObjIndex];

	if (gObjIsConnectedGP(lpObj->m_Index))
	{
		switch (lpMsg->bResult)
		{
		case 0:
			g_Log.Add("[GensSystem] [%s][%s] Gens Request - Leave [%s]", lpObj->AccountID, lpObj->Name, g_GensSystem.GetGensInfluenceName(lpObj));
			break;

		case 1:
			g_Log.Add("[GensSystem] [%s][%s] Gens Leaving Failed to Not Register Gens", lpObj->AccountID, lpObj->Name);
			break;

		case 2:
			g_Log.Add("[GensSystem] [%s][%s] Gens Leaving Failed to GuildMaster GensName [%s]", lpObj->AccountID, lpObj->Name, g_GensSystem.GetGensInfluenceName(lpObj));
			break;

		case 3:
			g_Log.Add("[GensSystem] [%s][%s] Gens Leaving Failed to Not Influence NPC : [%s]", lpObj->AccountID, lpObj->Name, g_GensSystem.GetGensInfluenceName(lpObj));
			break;

		default:
			break;
		}

		if (!lpMsg->bResult)
		{
			g_GensSystem.SetGensInfluence(lpObj, LEAVED_INFLUENCE);
			g_GensSystem.SetUserBattleZoneEnable(lpObj, BATTLE_ZONE_BLOCK);
			g_GensSystem.SetContributePoint(lpObj, 0);

			for( int i = 0 ; i < 10 ; i++ )
			{
				memset(&lpObj->m_PlayerData->KillUserName[i*11], 0x00, 11);

				lpObj->m_PlayerData->m_KillUserCnt[i] = 0;
				lpObj->m_PlayerData->m_KillUserTotCnt = 0;
			}

			g_GensSystem.GensViewportListProtocol(lpObj);
			g_GensSystem.SetGensClass(lpObj, 0);
			g_GensSystem.SetGensRanking(lpObj, 0);
		}

		_tagPMSG_ANS_SECEDE_GENS_MEMBER_EXDB pMsg;
		
		PHeadSubSetBE((LPBYTE)&pMsg, 0xF8, 0x04, sizeof(pMsg)); 
		pMsg.bResult = lpMsg->bResult;		
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, 0x05); 
	}
	else
	{
		g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
	}
}

//-> Completed
void DGAnsAbusingInfo(_tagPMSG_ANS_ABUSING_INFO *lpMsg)
{
	int iObjIndex = MAKE_NUMBERW(lpMsg->bIndexH, lpMsg->bIndexL);

	LPOBJ lpObj = &gObj[iObjIndex];

	if (gObjIsConnectedGP(iObjIndex))
	{
		for (int i = 0; i < 10; ++i)
		{
			if (strlen(&lpMsg->szCharNames[i]) > 0)
			{
				lpObj->m_PlayerData->KillUserName[i * 11 + MAX_ACCOUNT_LEN] = 0;
				memcpy(&lpObj->m_PlayerData->KillUserName[i * 11], &lpMsg->szCharNames[i * 11], 10);
				lpObj->m_PlayerData->m_KillUserCnt[i] = lpMsg->KillCount[i];
				++lpObj->m_PlayerData->m_KillUserTotCnt;
			}
		}
	}
	else
	{
		g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
	}
}

//-> Completed
void DGAnsGensReward(_tagPMSG_ANS_GENS_REWARD_EXDB *lpMsg)
{
	unsigned char btResult;

	int iObjIndex = MAKE_NUMBERW(lpMsg->bIndexH, lpMsg->bIndexL);
	
	LPOBJ lpObj = &gObj[iObjIndex];

	if (gObjIsConnectedGP(iObjIndex))
	{
		if (lpMsg->bResult)
		{
	 		btResult = lpMsg->bResult;
		}
		
		else
 		{
			if (g_GensSystem.SendGensRewardItem(lpObj, lpMsg->iGensClass))
 			{
		 		btResult = 0;
				g_GensSystem.ReqExDBGensRewardComplete(lpObj);
			}

	 		else
	 		{
		 		btResult = 3;
	 		}
		}

		g_Log.Add("[GensSystem] Send Gens Ranking Reward [%s][%s] Result [%d]", lpObj->AccountID, lpObj->Name, btResult);
		g_GensSystem.SendGensReward(lpObj, btResult);
	}

	else
	{
		g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
	}
}

//-> Completed
void DGAnsGensMemberCount(_tagPMSG_ANS_GENS_MEMBER_COUNT_EXDB *lpMsg)
{
	g_GensSystem.SetGensMemberCount(1, lpMsg->iVanertMemberCount);
	g_GensSystem.SetGensMemberCount(2, lpMsg->iDuprianMemberCount);
}

//-> Completed
void DGAnsGensRewardDayCheck(_tagPMSG_ANS_GENS_REWARD_DAY_CHECK_EXDB *lpMsg)
{
	int iObjIndex = MAKE_NUMBERW(lpMsg->bIndexH, lpMsg->bIndexL);

	LPOBJ lpObj = &gObj[iObjIndex];

	if (gObjIsConnectedGP(iObjIndex))
	{
		if (lpMsg->iRewardDay)
		{
			GSProtocol.GCServerMsgStringSend(Lang.GetText(0,498), iObjIndex, 1);
		}
	}

	else
	{
		g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
	}
}

struct EXSDHP_SERVERGROUP_GENS_CHATTING_SEND
{
	PBMSG_HEAD h;	// C1:50
	int iGensNum;	// 4
	char szCharacterName[10];	// 8
	char szChattingMsg[90];	// 12
};

void GDGensServerGroupChattingSend(int iGensInfluence, PMSG_CHATDATA* lpChatData)
{
	EXSDHP_SERVERGROUP_GENS_CHATTING_SEND pMsg = {0};

	pMsg.h.set((LPBYTE)&pMsg, 0x52, sizeof(EXSDHP_SERVERGROUP_GENS_CHATTING_SEND));

	pMsg.iGensNum = iGensInfluence;
	memcpy(pMsg.szCharacterName, lpChatData->chatid, MAX_ACCOUNT_LEN);
	memcpy(pMsg.szChattingMsg, lpChatData->chatmsg, 90);

	wsExDbCli.DataSend((char *)&pMsg, pMsg.h.size);
}

void DGGensServerGroupChattingRecv(EXSDHP_SERVERGROUP_GENS_CHATTING_RECV *lpMsg)
{
	PMSG_CHATDATA pMsg ={0};

	pMsg.h.set((LPBYTE)&pMsg, 0x00, sizeof(pMsg));	// #error MAYBE the header is wrong
	memcpy(pMsg.chatid, lpMsg->szCharacterName, sizeof(pMsg.chatid));
	memcpy(pMsg.chatmsg, lpMsg->szChattingMsg, sizeof(pMsg.chatmsg));

	if(lpMsg->iGensNum == DUPRIAN_INFLUENCE || lpMsg->iGensNum == VANERT_INFLUENCE)
	{
		for(int i=g_ConfigRead.server.GetObjectStartUserIndex();i<g_ConfigRead.server.GetObjectMax();i++)
		{
			LPOBJ lpObj = &gObj[i];

			if(lpObj->Connected == PLAYER_PLAYING)
			{
				if(lpObj->m_PlayerData->m_GensInfluence == lpMsg->iGensNum)
				{
					IOCP.DataSend(i, (LPBYTE)&pMsg, pMsg.h.size);
				}
			}
		}
	}
}

// Guild Matching
void GDReqGuildMatchingList(int nUserIndex, int nPage)
{
	_stReqGuildMatchingList pMsg;
	
	PHeadSubSetB((LPBYTE)&pMsg, 0xA3, 0x00, sizeof(pMsg));

	pMsg.nPage = nPage;
	pMsg.nUserIndex = nUserIndex;

	if (!gObjIsConnectedGP(nUserIndex))
	{
		g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	g_Log.Add("[GMATCHING][GD][0xA3][0x00] - %s ", gObj[nUserIndex].Name);
	wsExDbCli.DataSend((char *)&pMsg, pMsg.h.size);
}

void GDReqGuildMatchingListSearchWord(int nUserIndex, int nPage, char *szSearchWord)
{
	_stReqGuildMatchingListSearchWord pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0xA3, 0x01, sizeof(pMsg));

	pMsg.nPage = nPage;
	pMsg.nUserIndex = nUserIndex;

	if (!gObjIsConnectedGP(nUserIndex))
	{
		g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	memcpy(pMsg.szSearchWord, szSearchWord, 11);
	g_Log.Add("[GMATCHING][GD][0xA3][0x01] - %s [SEARCHWORD]- %s", gObj[nUserIndex].Name, szSearchWord);
	wsExDbCli.DataSend((char *)&pMsg, pMsg.h.size);
}

void DGAnsGuildMatchingList(_stAnsGuildMatchingList *lpMsg)
{
	BYTE btSendBuffer[2000];
	memset(btSendBuffer, 0x00, sizeof(btSendBuffer));

	int nUserIndex = lpMsg->nUserIndex;

	PMSG_ANS_GUILDMATCHINGLIST pMsg;
	_stGuildMatchingList stList;

	if (!gObjIsConnectedGP(nUserIndex))
	{
		g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	int nHeaderOffset = 24;
	int nDataOffset = 0;
	int nListCount = lpMsg->nListCount;
	int nResult = lpMsg->nResult;

	if (nListCount >= 0 && nListCount <= 9)
	{
		for (int i = 0; i < nListCount; i++)
		{
			memcpy(&stList, &lpMsg->stGuildMatchingList[i], sizeof(_stGuildMatchingList));
			memcpy(&btSendBuffer[nHeaderOffset + nDataOffset], &stList, sizeof(_stGuildMatchingList));
			nDataOffset += sizeof(_stGuildMatchingList);
		}
	}

	pMsg.nListCount = lpMsg->nListCount;
	pMsg.nPage = lpMsg->nPage;
	pMsg.nResult = lpMsg->nResult;
	pMsg.nTotalPage = lpMsg->nTotalPage;

	gObj[nUserIndex].m_PlayerData->m_bUseGuildMatching = true;
	PHeadSubSetW((LPBYTE)&pMsg, 0xED, 0x00, nDataOffset + nHeaderOffset);

	memcpy(&btSendBuffer, &pMsg, nHeaderOffset);
	IOCP.DataSend(nUserIndex, btSendBuffer, nHeaderOffset + nDataOffset);
	g_Log.Add("[GMATCHING][GC][0xED][0x00] - %s, ResultCode :%d ", gObj[nUserIndex].Name, nResult);
}

void GDReqDelMatchingList(int nUserIndex, int nGuildNum)
{
	_stReqDelGuildMatchingList pMsg;

	if (!gObjIsConnectedGP(nUserIndex))
	{
		g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	PHeadSubSetB((LPBYTE)&pMsg, 0xA3, 0x03, sizeof(pMsg));
	pMsg.nGuildNumber = nGuildNum;
	pMsg.nUserIndex = nUserIndex;

	g_Log.Add("[GMATCHING][GD][0xA3][0x03] - %s ", gObj[nUserIndex].Name);
	wsExDbCli.DataSend((char *)&pMsg, pMsg.h.size);
}

void DGAnsDelMatchingList(_stAnsDelGuildMatchingList *lpMsg)
{
	PMSG_ANS_CANCEL_GUILDMATCHINGLIST pMsg;
	int nUserIndex = lpMsg->nUserIndex;

	if (!gObjIsConnectedGP(nUserIndex))
	{
		g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	pMsg.nResult = lpMsg->nResult;
	PHeadSubSetB((LPBYTE)&pMsg, 0xED, 0x03, sizeof(pMsg));
	gObj[nUserIndex].m_PlayerData->m_bUseGuildMatching = false;

	g_Log.Add("[GMATCHING][GC][0xED][0x03] - %s ResultCode:%d ", gObj[nUserIndex].Name, pMsg.nResult);
	IOCP.DataSend(nUserIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void GDReqRegGuildMatchingList(int nUserIndex, _stGuildMatchingList stGuildMatchingList)
{
	_stReqGuildMatchingData pMsg;

	if (!gObjIsConnectedGP(nUserIndex))
	{
		g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	BYTE btGensType = g_GensSystem.GetGensInfluence(&gObj[nUserIndex]);

	if (!btGensType || btGensType == 0xFF)
	{
		return;
	}

	PHeadSubSetB((LPBYTE)&pMsg, 0xA3, 0x02, sizeof(pMsg));
	pMsg.nUserIndex = nUserIndex;
	memcpy(&pMsg._stGuildMatchingList, &stGuildMatchingList, sizeof(stGuildMatchingList));

	g_Log.Add("[GMATCHING][GD][0xA3][0x02] - %s ", gObj[nUserIndex].Name);
	wsExDbCli.DataSend((char *)&pMsg, pMsg.h.size);
}

void DGAnsRegGuildMatchingList(_stAnsGuildMatchingData *lpMsg)
{
	PMSG_ANS_REGGUILDMATCHINGDATA pMsg;
	int nUserIndex = lpMsg->nUserIndex;

	if (!gObjIsConnectedGP(nUserIndex))
	{
		g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	pMsg.nResult = lpMsg->nResult;
	PHeadSubSetB((LPBYTE)&pMsg, 0xED, 0x02, sizeof(pMsg));
	
	g_Log.Add("[GMATCHING][GC][0xED][0x02] - %s ResultCode:%d ", gObj[nUserIndex].Name, pMsg.nResult);
	IOCP.DataSend(nUserIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void GDReqJoinGuildMatchingList(int nUserIndex, _stGuildMatchingAllowListDB stAllowList)
{
	_stRegWaitGuildMatching pMsg;

	if (!gObjIsConnectedGP(nUserIndex))
	{
		g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	pMsg.nUserIndex = nUserIndex;
	memcpy(&pMsg.stAllowList, &stAllowList, sizeof(stAllowList));
	PHeadSubSetB((LPBYTE)&pMsg, 0xA3, 0x04, sizeof(pMsg));

	g_Log.Add("[GMATCHING][GC][0xA3][0x04] - %s ", gObj[nUserIndex].Name);
	wsExDbCli.DataSend((char *)&pMsg, pMsg.h.size);
}

void DGAnsJoinGuildMatching(_stAnsWaitGuildMatching *lpMsg)
{
	PMSG_ANS_JOIN_GUILDMATCHING pMsg;
	int nUserIndex = lpMsg->nUserIndex;

	if (!gObjIsConnectedGP(nUserIndex))
	{
		g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	pMsg.nResult = lpMsg->nResult;
	PHeadSubSetB((LPBYTE)&pMsg, 0xED, 0x04, sizeof(pMsg));

	g_Log.Add("[GMATCHING][GC][0xED][0x04] - %s ResultCode:%d ", gObj[nUserIndex].Name, pMsg.nResult);
	IOCP.DataSend(nUserIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void GDReqCancelJoinGuildMatching(int nUserIndex, char *szName, int nType)
{
	_stReqDelWaitGuildMatchingList pMsg;

	if (!gObjIsConnectedGP(nUserIndex))
	{
		g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	PHeadSubSetB((LPBYTE)&pMsg, 0xA3, 0x05, sizeof(pMsg));

	memcpy(&pMsg.szName, szName, MAX_ACCOUNT_LEN + 1);
	pMsg.nUserIndex = nUserIndex;
	pMsg.nType = nType;

	g_Log.Add("[GMATCHING][GD][0xA3][0x05] - %s ", gObj[nUserIndex].Name);
	wsExDbCli.DataSend((char *)&pMsg, pMsg.h.size);
}

void DGAnsCancelJoinGuildMatching(_stAnsDelWaitGuildMatchingList *lpMsg)
{
	PMSG_ANS_CANCEL_JOIN_GUILDMATCHING pMsg;
	int nUserIndex = lpMsg->nUserIndex;

	if (lpMsg->nType)
	{
		return;
	}

	if (!gObjIsConnectedGP(nUserIndex))
	{
		g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	pMsg.nResult = lpMsg->nResult;
	PHeadSubSetB((LPBYTE)&pMsg, 0xED, 0x05, sizeof(pMsg));

	g_Log.Add("[GMATCHING][GC][0xED][0x05] - %s ResultCode:%d ", gObj[nUserIndex].Name, pMsg.nResult);
	IOCP.DataSend(nUserIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void GDReqAllowJoinGuildMatching(int nUserIndex, int nAllowType, char *szName, char *szGuildName)
{
	_stReqAllowJoinGuildMatching pMsg;

	if (!gObjIsConnectedGP(nUserIndex))
	{
		g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	BOOL bError = FALSE;

	if (nAllowType == 1)
	{
		if (g_CastleSiegeSync.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
		{
			MsgOutput(nUserIndex, Lang.GetText(0,568));
			bError = TRUE;
		}

		if (IT_MAP_RANGE(gObj[nUserIndex].MapNumber) == TRUE || ITL_MAP_RANGE(gObj[nUserIndex].MapNumber) == TRUE)
		{
			MsgOutput(nUserIndex, Lang.GetText(0,569));
			bError = TRUE;
		}

		if (g_NewPVP.IsDuel(gObj[nUserIndex]) == TRUE)
		{
			GSProtocol.GCServerMsgStringSend(Lang.GetText(0,570), nUserIndex, 1);
			bError = TRUE;
		}

		if (g_NewPVP.IsObserver(gObj[nUserIndex]) == TRUE)
		{
			GSProtocol.GCServerMsgStringSend(Lang.GetText(0,571), nUserIndex, 1);
			bError = TRUE;
		}

		int iArcaBattleState = g_ArcaBattle.GetState();

		if (iArcaBattleState > 2 && iArcaBattleState < 9)
		{
			GSProtocol.GCResultSend(nUserIndex, 0x51, 0xA4);
			bError = TRUE;
		}

		if (gObj[nUserIndex].m_IfState.use > 0)
		{
			GSProtocol.GCResultSend(nUserIndex, 0x51, 0x06);
			bError = TRUE;
		}

		if ((gObj[nUserIndex].m_Option & 1) != 1)
		{
			GSProtocol.GCResultSend(nUserIndex, 0x51, 0x00);
			bError = TRUE;
		}

		if (gObj[nUserIndex].m_PlayerData->GuildNumber <= 0)
		{
			GSProtocol.GCResultSend(nUserIndex, 0x51, 0x04);
			bError = TRUE;
		}

		if (strcmp(gObj[nUserIndex].m_PlayerData->lpGuild->Names[0], gObj[nUserIndex].Name))
		{
			GSProtocol.GCResultSend(nUserIndex, 0x51, 0x04);
			bError = TRUE;
		}

		if (gObj[nUserIndex].m_PlayerData->lpGuild->WarState)
		{
			GSProtocol.GCResultSend(nUserIndex, 0x51, 0x06);
			bError = TRUE;
		}

		int MaxGuildMember;

		if (gObj[nUserIndex].Class == CLASS_DARKLORD)
			MaxGuildMember = (gObj[nUserIndex].Level + gObj[nUserIndex].m_PlayerData->MasterLevel) / 10 + gObj[nUserIndex].Leadership / 10;
		else
			MaxGuildMember = (gObj[nUserIndex].Level + gObj[nUserIndex].m_PlayerData->MasterLevel) / 10;

		if (MaxGuildMember > g_ConfigRead.data.common.MaxGuildMember)
			MaxGuildMember = g_ConfigRead.data.common.MaxGuildMember;

		if (gObj[nUserIndex].m_PlayerData->lpGuild->TotalCount >= MaxGuildMember)
		{
			GSProtocol.GCResultSend(nUserIndex, 0x51, 0x02);
			bError = TRUE;
		}
	}

	if (bError)
	{
		PMSG_ANS_ALLOW_JOIN_GUILDMATCHING pErrorMsg;

		pErrorMsg.nAllowType = nAllowType;
		pErrorMsg.nResult = -7;
		memcpy(&pErrorMsg.szMemberName, szName, MAX_ACCOUNT_LEN + 1);

		g_Log.Add("[GMATCHING][GC][0xED][0x06] - %s ResultCode:%d ", gObj[nUserIndex].Name, pErrorMsg.nResult);
		PHeadSubSetB((LPBYTE)&pErrorMsg, 0xED, 0x06, sizeof(pErrorMsg));
		IOCP.DataSend(nUserIndex, (LPBYTE)&pErrorMsg, pErrorMsg.h.size);
	}

	else
	{
		PHeadSubSetB((LPBYTE)&pMsg, 0xA3, 0x06, sizeof(pMsg));
		pMsg.nAllowType = nAllowType;
		pMsg.nUserIndex = nUserIndex;
		memcpy(pMsg.szName, szName, MAX_ACCOUNT_LEN+1);
		memcpy(pMsg.szGuildName, szGuildName, MAX_GUILD_LEN + 1);

		g_Log.Add("[GMATCHING][GD][0xA3][0x06] - %s ", gObj[nUserIndex].Name);
		wsExDbCli.DataSend((char *)&pMsg, pMsg.h.size);
	}
}

void DGAnsAllowJoinGuildMatching(_stAnsAllowJoinGuildMatching *lpMsg)
{
	int nUserIndex = lpMsg->nUserIndex;

	if (!gObjIsConnectedGP(nUserIndex))
	{
		g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	int nAllowType = lpMsg->nAllowType;
	int nResult = lpMsg->nResult;

	char szName[MAX_ACCOUNT_LEN + 1];
	char szGuildName[MAX_GUILD_LEN + 1];

	memcpy(szGuildName, lpMsg->szGuildName, MAX_GUILD_LEN + 1);
	memcpy(szName, lpMsg->szMemberName, MAX_ACCOUNT_LEN + 1);

	if (nAllowType == 1 && nResult == 0)
	{
		GDGuildMemberAddWithoutUserIndex(szGuildName, szName);
	}

	PMSG_ANS_ALLOW_JOIN_GUILDMATCHING pMsg;
	pMsg.nAllowType = nAllowType;
	pMsg.nResult = nResult;
	memcpy(&pMsg.szMemberName, szName, MAX_ACCOUNT_LEN + 1);

	g_Log.Add("[GMATCHING][GC][0xED][0x06] - %s ResultCode:%d ", gObj[nUserIndex].Name, pMsg.nResult);
	PHeadSubSetB((LPBYTE)&pMsg, 0xED, 0x06, sizeof(pMsg));
	IOCP.DataSend(nUserIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void GDReqWaitGuildMatchingList(int nGuildNumber, int nUserIndex)
{
	_stReqWaitGuildMatchingList pMsg;

	if (!gObjIsConnectedGP(nUserIndex))
	{
		g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	PHeadSubSetB((LPBYTE)&pMsg, 0xA3, 0x07, sizeof(pMsg));

	pMsg.nUserIndex = nUserIndex;
	pMsg.nGuildNumber = nGuildNumber;

	g_Log.Add("[GMATCHING][GD][0xA3][0x07] - %s ", gObj[nUserIndex].Name);
	wsExDbCli.DataSend((char *)&pMsg, pMsg.h.size);
}

void DGAnsWaitGuildMatchingList(_stAnsWaitGuildMatchingList *lpMsg)
{
	BYTE btSendBuffer[2000];
	memset(btSendBuffer, 0x00, sizeof(btSendBuffer));

	int nUserIndex = lpMsg->nUserIndex;

	if (!gObjIsConnectedGP(nUserIndex))
	{
		g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	_stGuildMatchingAllowList stList;
	PMSG_ANS_WAIT_GUILDMATCHING pMsg;

	int nHeaderOffset = 16;
	int nDataOffset = 0;
	int nListCount = lpMsg->nListCount;
	int nResult = lpMsg->nResult;

	if (nListCount > 0 && nListCount <= 80)
	{
		for (int i = 0; i < nListCount; i++)
		{
			stList.btClass = lpMsg->stAllowList[i].btApplicantClass;
			stList.nLevel = lpMsg->stAllowList[i].nApplicantLevel;
			memcpy(stList.szName, &lpMsg->stAllowList[i].szApplicantName, MAX_ACCOUNT_LEN + 1);

			memcpy(&btSendBuffer[nHeaderOffset + nDataOffset], &stList, sizeof(_stGuildMatchingAllowList));
			nDataOffset += sizeof(_stGuildMatchingAllowList);
		}
	}

	pMsg.nListCount = nListCount;
	pMsg.nResult = nResult;
	PHeadSubSetW((LPBYTE)&pMsg, 0xED, 0x07, nHeaderOffset + nDataOffset);
	memcpy(&btSendBuffer, &pMsg, sizeof(pMsg));

	g_Log.Add("[GMATCHING][GC][0xED][0x07] - %s ResultCode:%d ", gObj[nUserIndex].Name, nResult);
	IOCP.DataSend(nUserIndex, btSendBuffer, nHeaderOffset + nDataOffset);
}

void GDReqGetWaitStateListGuildMatching(int nUserIndex, char *szApplicantName)
{
	if (!gObjIsConnectedGP(nUserIndex))
	{
		g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	_stReqWaitStateListGuildMatching pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0xA3, 0x08, sizeof(pMsg));
	pMsg.nUserIndex = nUserIndex;
	memcpy(&pMsg.szName, szApplicantName, MAX_ACCOUNT_LEN + 1);

	g_Log.Add("[GMATCHING][GD][0xA3][0x08] - %s ", gObj[nUserIndex].Name);
	wsExDbCli.DataSend((char *)&pMsg, pMsg.h.size);
}

void DGAnsGetWaitStateListGuildMatching(_stAnsWaitStateListGuildMatching *lpMsg)
{
	int nUserIndex = lpMsg->nUserIndex;

	if (!gObjIsConnectedGP(nUserIndex))
	{
		g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	PMSG_ANS_ALLOWLIST_STATE_GUILDMATCHING pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xED, 0x08, sizeof(pMsg));

	pMsg.nResult = lpMsg->nResult;
	memcpy(pMsg.szGuildMasterName, lpMsg->szGuildMasterName, MAX_ACCOUNT_LEN + 1);
	memcpy(pMsg.szGuildName, lpMsg->szGuildName, MAX_GUILD_LEN + 1);

	g_Log.Add("[GMATCHING][GC][0xED][0x08] - %s ResultCode:%d ", gObj[nUserIndex].Name, pMsg.nResult);
	IOCP.DataSend(nUserIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void DGAnsNotiGuildMatching(_stAnsNotiGuildMatching *lpMsg)
{
	int nUserIndex = lpMsg->nUserIndex;

	if (!gObjIsConnectedGP(nUserIndex))
	{
		g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	if (lpMsg->nResult)
	{
		gObj[nUserIndex].m_PlayerData->m_bUseGuildMatching = false;

		PMSG_ANS_NOTI_GUILDMATCHING pMsg;
		PHeadSubSetB((LPBYTE)&pMsg, 0xED, 0x09, sizeof(pMsg));
		pMsg.nResult = lpMsg->nResult;

		g_Log.Add("[GMATCHING][GC][0xED][0x09] - %s ResultCode:%d ", gObj[nUserIndex].Name, pMsg.nResult);
		IOCP.DataSend(nUserIndex, (LPBYTE)&pMsg, pMsg.h.size);

		GDReqCancelJoinGuildMatching(nUserIndex, gObj[nUserIndex].Name, 1);
	}

	else
	{
		gObj[nUserIndex].m_PlayerData->m_bUseGuildMatching = true;
	}
}

void DGAnsNotiGuildMatchingForGuildMaster(_stAnsNotiGuildMatchingForGuildMaster *lpMsg)
{
	int nUserIndex = lpMsg->nUserIndex;

	if (!gObjIsConnectedGP(nUserIndex))
	{
		g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	PMSG_ANS_NOTI_GUILDMATCHING_FORGUILDMASTER pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xED, 0x10, sizeof(pMsg));

	pMsg.nResult = lpMsg->nResult;

	g_Log.Add("[GMATCHING][GC][0xED][0x10] - %s ResultCode:%d ", gObj[nUserIndex].Name, pMsg.nResult);
	IOCP.DataSend(nUserIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void DGAnsUseGuildMatchingGuild(_stAnsUseGuildMatchingGuild *lpMsg)
{
	int nUserIndex = lpMsg->nUserIndex;

	if (!gObjIsConnectedGP(nUserIndex))
	{
		g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	gObj[nUserIndex].m_PlayerData->m_bUseGuildMatching = true;
	g_Log.Add("[GMATCHING][0xA3][0x11] - %s", gObj[nUserIndex].Name);
}

// Party Matching

void GDReqRegWantedPartyMember(_PARTY_INFO_LISTDB stList, int nUserIndex)
{
	if (!gObjIsConnectedGP(nUserIndex))
	{
		g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	int nPartyNumber = gObj[nUserIndex].PartyNumber;

	if (nPartyNumber >= 0 && gParty.m_PartyS[nPartyNumber].Number[0] != nUserIndex)
	{
		PMSG_ANS_REG_WANTED_PARTYMEMBER pMsg;
		PHeadSubSetB((LPBYTE)&pMsg, 0xEF, 0x00, sizeof(pMsg));

		pMsg.nResult = -3;

		g_Log.Add("[PMATCHING][GC][0xEF][0x00] (%s) resultCode:%d ", gObj[nUserIndex].Name, -3);
		IOCP.DataSend(nUserIndex, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	_stReqRegWantedPartyMember pMsg;

	pMsg.btCurrentPartyCnt = stList.btCurPartyMemberCnt;
	pMsg.btUsePassWord = stList.btUsePassword;
	pMsg.btWantedClass = stList.btWantedClass;
	pMsg.nHuntingGround = stList.nHuntingGround;
	pMsg.nMaxLevel = stList.nMaxLevel;
	pMsg.nMinLevel = stList.nMinLevel;
	pMsg.nUserIndex = nUserIndex;
	pMsg.btGensType = stList.btGensType;
	pMsg.nLeaderLevel = stList.nLeaderLevel;
	pMsg.btLeaderClass = stList.btLeaderClass;
	pMsg.btApprovalType = stList.btApprovalType;
	memcpy(pMsg.szPartyLeaderName, stList.szLeaderName, MAX_ACCOUNT_LEN + 1);
	memcpy(pMsg.szPassword, stList.szPassword, 5);
	memcpy(pMsg.szTitle, stList.szTitle, 41);
	memcpy(pMsg.btWantedClassDetailInfo, stList.btWantedClassDetailInfo, 7);
	PHeadSubSetB((LPBYTE)&pMsg, 0xA4, 0x00, sizeof(pMsg));

	g_Log.Add("[PMATCHING][GD][0xA4][0x00] (%s)", gObj[nUserIndex].Name);
	wsExDbCli.DataSend((char *)&pMsg, pMsg.h.size);
}

void DGAnsRegWantedPartyMember(_stAnsRegWantedPartyMember *lpMsg)
{
	int nUserIndex = lpMsg->nUserIndex;

	if (!gObjIsConnectedGP(nUserIndex))
	{
		g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	PMSG_ANS_REG_WANTED_PARTYMEMBER pMsg;

	if (lpMsg->nResult == 0)
	{
		gObj[nUserIndex].m_PlayerData->m_bUsePartyMatching = true;
		gObj[nUserIndex].m_PlayerData->m_bPartyMatchingLeader = true;
	}

	PHeadSubSetB((LPBYTE)&pMsg, 0xEF, 0x00, sizeof(pMsg));
	pMsg.nResult = lpMsg->nResult;

	g_Log.Add("[PMATCHING][GC][0xEF][0x00] (%s) resultCode:%d ", gObj[nUserIndex].Name, pMsg.nResult);
	IOCP.DataSend(nUserIndex, (LPBYTE)&pMsg, pMsg.h.size);

	int nPartyNum = gObj[nUserIndex].PartyNumber;

	if (nPartyNum >= 0 && lpMsg->nResult == 0)
	{
		_stReqAddPartyMember pAddMsg;
		PHeadSubSetB((LPBYTE)&pAddMsg, 0xA4, 0x05, sizeof(pAddMsg));

		int nPartyCount = gParty.m_PartyS[nPartyNum].Count;
		for (int i = 1; i < nPartyCount; i++)
		{
			if(ObjectMaxRange(gParty.m_PartyS[nPartyNum].Number[i]) == false)
			{
				continue;
			}

			if(gObj[gParty.m_PartyS[nPartyNum].Number[i]].Type != OBJ_USER)
			{
				continue;
			}

			pAddMsg.btType = 0;
			pAddMsg.nUserIndex = nUserIndex;
			pAddMsg.btAlreadyParty = TRUE;
			pAddMsg.nMemberIndex = gParty.m_PartyS[nPartyNum].Number[i];
			memcpy(pAddMsg.szLeaderName, gObj[nUserIndex].Name, MAX_ACCOUNT_LEN + 1);
			memcpy(pAddMsg.szMemberName, gObj[gParty.m_PartyS[nPartyNum].Number[i]].Name, MAX_ACCOUNT_LEN + 1);

			g_Log.Add("[PMATCHING][GD][0xA4][0x05] LeaderName:%s, MemberName:%s ", pAddMsg.szLeaderName, pAddMsg.szMemberName);
			wsExDbCli.DataSend((char *)&pAddMsg, pAddMsg.h.size);
		}
	}
}

void GDReqGetPartyMatchingList(int nPage, char *szSearchWord, int nType, int nUserIndex)
{
	if (!gObjIsConnectedGP(nUserIndex))
	{
		g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	_stReqGetPartyMatchingList pMsg;

	pMsg.nPage = nPage;
	pMsg.nType = nType;
	pMsg.nUserIndex = nUserIndex;

	switch (gObj[nUserIndex].Class)
	{
		case CLASS_WIZARD:
			pMsg.btClass = 4;
			break;
		case CLASS_KNIGHT:
			pMsg.btClass = 1;
			break;
		case CLASS_ELF:
			pMsg.btClass = 2;
			break;
		case CLASS_MAGUMSA:
			pMsg.btClass = 8;
			break;
		case CLASS_DARKLORD:
			pMsg.btClass = 16;
			break;
		case CLASS_SUMMONER:
			pMsg.btClass = 32;
			break;
		case CLASS_RAGEFIGHTER:
			pMsg.btClass = 64;
			break;
		default:
			pMsg.btClass = 0;
			break;
	}

	pMsg.nLevel = gObj[nUserIndex].Level + gObj[nUserIndex].m_PlayerData->MasterLevel;
	pMsg.btGens = g_GensSystem.GetGensInfluence(&gObj[nUserIndex]);
	memcpy(pMsg.szSearchWord, szSearchWord, 11);
	PHeadSubSetB((LPBYTE)&pMsg, 0xA4, 0x01, sizeof(pMsg));

	g_Log.Add("[PMATCHING][GD][0xA4][0x01] (%s)", gObj[nUserIndex].Name);
	wsExDbCli.DataSend((char *)&pMsg, pMsg.h.size);
}

void DGAnsGetPartyMatchingList(_stAnsGetPartyMatchingList *lpMsg)
{
	int nUserIndex = lpMsg->nUserIndex;

	if (!gObjIsConnectedGP(nUserIndex))
	{
		g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	PMSG_ANS_GET_PARTYMATCHINGLIST pMsg;
	_PARTY_INFO_LIST stList;
	BYTE btSendBuffer[2000];
	memset(btSendBuffer, 0x00, sizeof(btSendBuffer));

	int nHeaderOffset = 24;
	int nDataOffset = 0;
	int nListCount = lpMsg->nListCount;

	if (nListCount > 0 && nListCount <= 6)
	{
		for (int i = 0; i < nListCount; i++)
		{
			memcpy(&stList, &lpMsg->stPartyInfoList[i], sizeof(stList));
			memcpy(&btSendBuffer[nHeaderOffset + nDataOffset], &stList, sizeof(stList));

			nDataOffset += sizeof(stList);
		}
	}

	PHeadSubSetW((LPBYTE)&pMsg, 0xEF, 0x01, nHeaderOffset + nDataOffset);
	pMsg.nListCount = lpMsg->nListCount;
	pMsg.nPage = lpMsg->nPage;
	pMsg.nTotalPage = lpMsg->nTotalPage;
	pMsg.nResult = lpMsg->nResult;
	
	memcpy(&btSendBuffer, &pMsg, sizeof(pMsg));
	IOCP.DataSend(nUserIndex, btSendBuffer, nHeaderOffset + nDataOffset);
	g_Log.Add("[PMATCHING][GC][0xEF][0x01] (%s) resultCode:%d ", gObj[nUserIndex].Name, pMsg.nResult);
}

void GDReqJoinMemberPartyMatching(char *szMemberName, char *szLeaderName, char *szPassWord, int nUserIndex, int nUserDBNumber, int nLevel, BYTE btClass, BYTE btRandomParty, BYTE btGensType, BYTE btChangeUpClass)
{
	if (!gObjIsConnectedGP(nUserIndex))
	{
		g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	if (gObj[nUserIndex].PartyNumber > 0)
	{
		PMSG_ANS_MEMBERJOIN_PARTYMATCHINGLIST pMsg;

		pMsg.nResult = -6;
		PHeadSubSetB((LPBYTE)&pMsg, 0xEF, 0x02, sizeof(pMsg));

		g_Log.Add("[PMATCHING][GC][0xEF][0x02] (%s) resultCode:%d ", gObj[nUserIndex].Name, -6);
		IOCP.DataSend(nUserIndex, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	_stReqJoinMemberPartyMatching pMsg;

	pMsg.btClass = btClass;
	pMsg.btRandomParty = btRandomParty;
	pMsg.nLevel = nLevel;
	pMsg.nUserDBNumber = nUserDBNumber;
	pMsg.nUserIndex = nUserIndex;
	pMsg.btGensType = btGensType;
	pMsg.btChangeUpClass = btChangeUpClass;
	memcpy(pMsg.szLeaderName, szLeaderName, MAX_ACCOUNT_LEN + 1);
	memcpy(pMsg.szPassword, szPassWord, 5);
	memcpy(pMsg.szMemberName, szMemberName, MAX_ACCOUNT_LEN + 1);
	PHeadSubSetB((LPBYTE)&pMsg, 0xA4, 0x02, sizeof(pMsg));

	g_Log.Add("[PMATCHING][GD][0xA4][0x02] (%s)  ", gObj[nUserIndex].Name);
	wsExDbCli.DataSend((char *)&pMsg, pMsg.h.size);
}

void DGAnsJoinMemberPartyMatching(_stAnsJoinMemberPartyMatching *lpMsg)
{
	int nUserIndex = lpMsg->nUserIndex;

	if (!gObjIsConnectedGP(nUserIndex))
	{
		g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	if (lpMsg->nResult == 0)
	{
		gObj[nUserIndex].m_PlayerData->m_bUsePartyMatching = true;
	}

	PMSG_ANS_MEMBERJOIN_PARTYMATCHINGLIST pMsg;
	pMsg.nResult = lpMsg->nResult;
	PHeadSubSetB((LPBYTE)&pMsg, 0xEF, 0x02, sizeof(pMsg));

	g_Log.Add("[PMATCHING][GC][0xEF][0x02] (%s) resultCode:%d ", gObj[nUserIndex].Name, pMsg.nResult);
	IOCP.DataSend(nUserIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void GDReqMemberJoinStateList(char *szMemberName, int nUserIndex)
{
	if (!gObjIsConnectedGP(nUserIndex))
	{
		g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	_stReqJoinMemberStateListPartyMatching pMsg;
	pMsg.nUserIndex = nUserIndex;
	memcpy(pMsg.szMemberName, szMemberName, MAX_ACCOUNT_LEN + 1);
	PHeadSubSetB((LPBYTE)&pMsg, 0xA4, 0x03, sizeof(pMsg));

	g_Log.Add("[PMATCHING][GD][0xA4][0x03] (%s)  ", gObj[nUserIndex].Name);
	wsExDbCli.DataSend((char *)&pMsg, pMsg.h.size);
}
void DGAnsMemberJoinStateList(_stAnsJoinMemberStateListPartyMatching *lpMsg)
{
	int nUserIndex = lpMsg->nUserIndex;

	if (!gObjIsConnectedGP(nUserIndex))
	{
		g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	PMSG_ANS_MEMBERJOIN_STATELIST_PARTYMATCHING pMsg;
	pMsg.nResult = lpMsg->nResult;
	pMsg.nLeaderChannel = lpMsg->btLeaderChannel + 1;
	memcpy(pMsg.szLeaderName, lpMsg->szLeaderName, MAX_ACCOUNT_LEN + 1);

	PHeadSubSetB((LPBYTE)&pMsg, 0xEF, 0x03, sizeof(pMsg));

	g_Log.Add("[PMATCHING][GC][0xEF][0x03] (%s) resultCode:%d ", gObj[nUserIndex].Name, pMsg.nResult);
	IOCP.DataSend(nUserIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void GDReqMemberJoinStateListLeader(char *szLeaderName, int nUserIndex)
{
	if (!gObjIsConnectedGP(nUserIndex))
	{
		g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	_stReqWaitListPartyMatching pMsg;
	pMsg.nUserIndex = nUserIndex;
	memcpy(pMsg.szLeaderName, szLeaderName, MAX_ACCOUNT_LEN + 1);
	PHeadSubSetB((LPBYTE)&pMsg, 0xA4, 0x04, sizeof(pMsg));

	g_Log.Add("[PMATCHING][GD][0xA4][0x04] (%s)  ", gObj[nUserIndex].Name);
	wsExDbCli.DataSend((char *)&pMsg, pMsg.h.size);
}

void DGAnsMemberJoinStateListLeader(_stAnsWaitListPartyMatching *lpMsg)
{
	int nListCount = lpMsg->nListCount;
	int nResult = lpMsg->nResult;
	int nUserIndex = lpMsg->nUserIndex;

	if (!gObjIsConnectedGP(nUserIndex))
	{
		g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}
	
	_PARTY_MEMBER_WAIT_LIST stList;
	BYTE btSendBuffer[2000];
	memset(btSendBuffer, 0x00, sizeof(btSendBuffer));

	int nHeaderOffset = 16;
	int nDataOffset = 0;

	if (nListCount > 0 && nListCount <= 10)
	{
		for (int i = 0; i < nListCount; i++)
		{
			memcpy(&stList, &lpMsg->stList[i], sizeof(stList));
			memcpy(&btSendBuffer[nHeaderOffset + nDataOffset], &stList, sizeof(stList));
			nDataOffset += sizeof(stList);
		}
	}

	PMSG_ANS_WAITLIST_PARTYMATCHING pMsg;
	pMsg.nListCnt = nListCount;
	pMsg.nResult = nResult;
	PHeadSubSetW((LPBYTE)&pMsg, 0xEF, 0x04, nHeaderOffset + nDataOffset);
	memcpy(&btSendBuffer, &pMsg, sizeof(pMsg));

	IOCP.DataSend(nUserIndex, btSendBuffer, nHeaderOffset + nDataOffset);
	g_Log.Add("[PMATCHING][GC][0xEF][0x04] (%s) resultCode:%d ", gObj[nUserIndex].Name, pMsg.nResult);
}

void DGAnsSendPartyMemberListPartyMatching(_stAnsSendPartyMemberList *lpMsg)
{
	int nUserIndex = lpMsg->nUserIndex;

	if (!gObjIsConnectedGP(nUserIndex))
	{
		g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	PMSG_PARTYLIST pList;
	PMSG_PARTYLISTCOUNT pCount;

	gObj[nUserIndex].m_PlayerData->m_bUsePartyMatching = true;
	PHeadSetB((LPBYTE)&pCount, 0x42, sizeof(pCount));
	pCount.Result = 0;
	pCount.Count = lpMsg->nMemberCount;

	if (gObj[nUserIndex].m_PlayerData->m_bPartyMatchingLeader == true)
	{
		int nPartyNumber = gObj[nUserIndex].PartyNumber;

		if (nPartyNumber >= 0)
		{
			gParty.m_PartyS[nPartyNumber].nMatchingPartyCount = lpMsg->nMemberCount;
			gObj[nUserIndex].m_PlayerData->m_nPartyMatchingMemberCount = lpMsg->nMemberCount;
		}
	}

	if (!lpMsg->nMemberCount && !gObj[nUserIndex].m_PlayerData->m_bPartyMatchingLeader)
	{
		gObj[nUserIndex].m_PlayerData->m_bUsePartyMatching = false;

		if (gObj[nUserIndex].PartyNumber >= 0)
		{
			int index = gParty.GetIndex(gObj[nUserIndex].PartyNumber, nUserIndex, gObj[nUserIndex].DBNumber);
			gParty.Delete(gObj[nUserIndex].PartyNumber, index);

			if (gParty.GetCount(gObj[nUserIndex].PartyNumber) < 1)
			{
				gParty.Destroy(gObj[nUserIndex].PartyNumber);
			}

			gObj[nUserIndex].PartyNumber = -1;
			gObj[nUserIndex].PartyTargetUser = -1;
		}

		GSProtocol.GCPartyDelUserSendNoMessage(nUserIndex);
	}
	
	char szLeaderName[MAX_ACCOUNT_LEN + 1];
	memcpy(szLeaderName, lpMsg->stList[0].Name, MAX_ACCOUNT_LEN + 1);

	BYTE sendbuf[512];
	int lOfs = sizeof(PMSG_PARTYLISTCOUNT);
	int nCurrentServer = FALSE;

	g_Log.Add("[PMATCHING][GC][0x42]Send to %s", gObj[nUserIndex].Name);

	for (int n = 0; n < MAX_USER_IN_PARTY; n++)
	{
		if (lpMsg->stList[n].bUse == TRUE)
		{
			memset(&pList, 0, sizeof(pList));
			memcpy(pList.szId, lpMsg->stList[n].Name, MAX_ACCOUNT_LEN);
			pList.nServerChannel = lpMsg->stList[n].nServerChannel;
			int nMemberIndex = lpMsg->stList[n].nUserIndex;

			if (lpMsg->stList[n].bFlag && ObjectMaxRange(nMemberIndex) && gObj[nMemberIndex].Type == OBJ_USER)
			{
				pList.MapNumber = gObj[nMemberIndex].MapNumber;
				pList.X = gObj[nMemberIndex].X;
				pList.Y = gObj[nMemberIndex].Y;
				pList.Life = gObj[nMemberIndex].Life;
				pList.MaxLife = gObj[nMemberIndex].MaxLife + gObj[nMemberIndex].AddLife;
				pList.nMana = gObj[nMemberIndex].Mana;
				pList.nMaxMana = gObj[nMemberIndex].MaxMana + gObj[nMemberIndex].MaxMana;
				gObj[nMemberIndex].m_PlayerData->m_nPartyMatchingIndex = lpMsg->nPartyMatchingIndex;
				nCurrentServer = TRUE;
				GSProtocol.GCDisplayBuffeffectPartyMember(nMemberIndex);
			}

			else
			{
				pList.Life = 0;
				pList.MapNumber = 0;
				pList.MaxLife = 0;
				pList.X = 0;
				pList.Y = 0;
				nCurrentServer = 0;
			}

			pList.Number = n;
			g_Log.Add("[PMATCHING][GC][0x42] PartyMember:%s LeaderName:%s, IsCurrentServer:%d (%d/%d life)(%d/%d mana)",
				lpMsg->stList[n].Name, szLeaderName, nCurrentServer, (int)pList.Life, (int)pList.MaxLife, (int)pList.nMana, (int)pList.nMaxMana);

			memcpy(&sendbuf[lOfs], &pList, sizeof(pList));
			lOfs += sizeof(pList);
		}
	}

	pCount.h.size = lOfs;
	memcpy(sendbuf, &pCount, sizeof(pCount));
	g_Log.Add("[PMATCHING][GC][0x42] = END = ");
	IOCP.DataSend(nUserIndex, sendbuf, lOfs);
}

void GDReqCancelPartyMatching(int nUserIndex, BYTE btType)
{
	_stReqCancelPartyMatching pMsg;

	pMsg.btType = btType;
	pMsg.nUserIndex = nUserIndex;
	memcpy(pMsg.szName, gObj[nUserIndex].Name, MAX_ACCOUNT_LEN + 1);
	PHeadSubSetB((LPBYTE)&pMsg, 0xA4, 0x06, sizeof(pMsg));

	g_Log.Add("[PMATCHING][GD][0xA4][0x06] (%s)  ", gObj[nUserIndex].Name);
	wsExDbCli.DataSend((char *)&pMsg, pMsg.h.size);
}

void DGAnsCancelPartyMatching(_stAnsCancelPartyMatching *lpMsg)
{
	int nUserIndex = lpMsg->nUserIndex;

	if (!gObjIsConnectedGP(nUserIndex))
	{
		g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	if (!lpMsg->nResult)
	{
		if (gObj[nUserIndex].PartyNumber < 0)
		{
			GSProtocol.GCPartyDelUserSend(nUserIndex);
		}

		else
		{
			GSProtocol.CGPartyList(nUserIndex);
		}

		gObj[nUserIndex].m_PlayerData->m_bPartyMatchingLeader = false;
		gObj[nUserIndex].m_PlayerData->m_bUsePartyMatching = false;
		gObj[nUserIndex].m_PlayerData->m_nPartyMatchingIndex = -1;
		gObj[nUserIndex].m_PlayerData->m_nPartyMatchingMemberCount = 0;
	}

	PMSG_ANS_CANCEL_JOIN_PARTYMATCHING pMsg;
	pMsg.btType = lpMsg->btType;
	pMsg.nResult = lpMsg->nResult;
	PHeadSubSetB((LPBYTE)&pMsg, 0xEF, 0x06, sizeof(pMsg));

	g_Log.Add("[PMATCHING][GC][0xEF][0x06] (%s) resultCode:%d ", gObj[nUserIndex].Name, pMsg.nResult);
	IOCP.DataSend(nUserIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void GDReqAcceptMemberJoin(int nUserIndex, BYTE btType, char *szLeaderName, char *szMemberName, BYTE btManual)
{
	_stReqAddPartyMember pMsg;

	pMsg.nUserIndex = nUserIndex;
	pMsg.btType = btType;
	pMsg.btAlreadyParty = FALSE;
	pMsg.btManualJoin = btManual;
	memcpy(pMsg.szLeaderName, szLeaderName, MAX_ACCOUNT_LEN + 1);
	memcpy(pMsg.szMemberName, szMemberName, MAX_ACCOUNT_LEN + 1);
	PHeadSubSetB((LPBYTE)&pMsg, 0xA4, 0x05, sizeof(pMsg));

	g_Log.Add("[PMATCHING][GD][0xA4][0x05] (%s)  ", gObj[nUserIndex].Name);
	wsExDbCli.DataSend((char *)&pMsg, pMsg.h.size);
}

void DGAnsAcceptMemberJoin(_stAnsAddPartyMember *lpMsg)
{
	if (lpMsg->btManualJoin != FALSE)
	{
		return;
	}
	 
	PMSG_ANS_ACCEPTMEMBER_PARTYMATCHING pMsg;
	pMsg.nResult = lpMsg->nResult;
	pMsg.btType = lpMsg->btType;
	PHeadSubSetB((LPBYTE)&pMsg, 0xEF, 0x05, sizeof(pMsg));
	memcpy(pMsg.szMemberName, lpMsg->szMemberName, MAX_ACCOUNT_LEN + 1);

	if (lpMsg->btSendType)
	{
		if (gObjIsConnectedGP(lpMsg->nMemberIndex) == FALSE)
		{
			g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
			return;
		}

		pMsg.btFlag = TRUE;

		if (!lpMsg->btType)
		{
			gObj[lpMsg->nMemberIndex].m_PlayerData->m_bUsePartyMatching = false;
		}

		g_Log.Add("[PMATCHING][GC][0xEF][0x05] (%s/Member) resultCode:%d ", gObj[lpMsg->nMemberIndex].Name, pMsg.nResult);
		IOCP.DataSend(lpMsg->nMemberIndex, (LPBYTE)&pMsg, sizeof(pMsg));
	}

	else
	{
		if (gObjIsConnectedGP(lpMsg->nUserIndex) == FALSE)
		{
			g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
			return;
		}

		pMsg.btFlag = FALSE;
		g_Log.Add("[PMATCHING][GC][0xEF][0x05] (%s/Leader) resultCode:%d ", gObj[lpMsg->nUserIndex].Name, pMsg.nResult);
		IOCP.DataSend(lpMsg->nUserIndex, (LPBYTE)&pMsg, sizeof(pMsg));
	}
}

void DGFixAddPartyMember(_stAnsAddRealPartyMember *lpMsg)
{
	int nLeaderIndex = lpMsg->nLeaderUserIndex;
	int nMemberIndex = lpMsg->nMemberUserIndex;

	if (gObjIsConnectedGP(nLeaderIndex) == FALSE)
	{
		g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	if (gObjIsConnectedGP(nMemberIndex) == FALSE)
	{
		g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	if (gObj[nLeaderIndex].PartyNumber < 0)
	{
		gObj[nLeaderIndex].PartyNumber = gParty.Create(nLeaderIndex, gObj[nLeaderIndex].DBNumber, gObj[nLeaderIndex].Level);
		gParty.m_PartyS[gObj[nLeaderIndex].PartyNumber].bUsePartyMatching = true;
	}

	if (gObj[nLeaderIndex].PartyNumber >= 0)
	{
		int nPartyNumber = gObj[nLeaderIndex].PartyNumber;
		gParty.m_PartyS[nPartyNumber].bUsePartyMatching = true;
		
		int addresult = gParty.Add(nPartyNumber, nMemberIndex, gObj[nMemberIndex].DBNumber, gObj[nMemberIndex].Level);

		if (addresult < 0)
		{
			if (addresult == -1)
			{
				GSProtocol.GCResultSend(nLeaderIndex, 0x41, 2);
				GSProtocol.GCResultSend(nMemberIndex, 0x41, 2);
			}
		}
		else
		{
			gObj[nMemberIndex].PartyNumber = gObj[nLeaderIndex].PartyNumber;
			gParty.Paint(nPartyNumber);
		}
	}
}

void GDReqDeletePartyUser(int nUserIndex, char *szTargetName)
{
	_stReqDelPartyUserPartyMatching pMsg;
	pMsg.nUserIndex = nUserIndex;
	memcpy(pMsg.szTargetName, szTargetName, MAX_ACCOUNT_LEN + 1);
	PHeadSubSetB((LPBYTE)&pMsg, 0xA4, 0x07, sizeof(pMsg));

	g_Log.Add("[PMATCHING][GD][0xA4][0x07] (%s)  ", gObj[nUserIndex].Name);
	wsExDbCli.DataSend((char *)&pMsg, pMsg.h.size);
}

void DGAnsRequestJoinPartyMatchingNoti(_stAnsRequestJoinPartyMatchingNoti *lpMsg)
{
	int nUserIndex = lpMsg->nUserIndex;

	if (!gObjIsConnectedGP(nUserIndex))
	{
		g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	PMSG_ANS_NOTI_JOINPARTYMATCHING pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xEF, 0x08, sizeof(pMsg));

	g_Log.Add("[PMATCHING][GC][0xEF][0x08] (%s)", gObj[nUserIndex].Name);
	IOCP.DataSend(nUserIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void DGAnsDeletePartyUser(_stAnsDelPartyUserPartyMatching *lpMsg)
{
	int nTargetIndex = lpMsg->nTargetIndex;
	int nResult = lpMsg->nResult;

	if (!lpMsg->btType)
	{
		return;
	}

	if (!gObjIsConnectedGP(nTargetIndex))
	{
		g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	if (!nResult)
	{
		gObj[nTargetIndex].m_PlayerData->m_bUsePartyMatching = false;
		gObj[nTargetIndex].m_PlayerData->m_bPartyMatchingLeader = false;
		gObj[nTargetIndex].m_PlayerData->m_nPartyMatchingIndex = -1;
	}

	g_Log.Add("[PMATCHING][0xA4][0x07][DeleteUser] targetName => %s ", gObj[nTargetIndex].Name);
}

void GDSendChatMsgPartyMatching(char *szMsg, char *szName, int nPartyMatchingIndex)
{
	_stReqChattingPartyMatching pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xA4, 0x12, sizeof(pMsg));
	memcpy(pMsg.szChat, szMsg, sizeof(pMsg.szChat));
	memcpy(pMsg.szSendCharName, szName, MAX_ACCOUNT_LEN + 1);
	pMsg.nPartyIndex = nPartyMatchingIndex;

	wsExDbCli.DataSend((char *)&pMsg, pMsg.h.size);
}

void DGRecvChatMsgPartyMatching(_stAnsChattingPartyMatching *lpMsg)
{
	int nTargetIndex = lpMsg->nRecvUserIndex;

	if (!gObjIsConnectedGP(nTargetIndex))
	{
		g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	if (gObj[nTargetIndex].m_PlayerData->m_bUsePartyMatching == false)
	{
		return;
	}

	if (gObj[nTargetIndex].m_PlayerData->m_nPartyMatchingIndex != lpMsg->nPartyIndex)
	{
		return;
	}

	PMSG_CHATDATA pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x00, 0x4B);

	memcpy(pMsg.chatmsg, lpMsg->szChat, 63);
	memcpy(pMsg.chatid, lpMsg->szSendCharName, MAX_ACCOUNT_LEN);

	IOCP.DataSend(nTargetIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void GDReqSendPartyMemberList(char *szLeaderName)
{
	_stReqSendPartyMemberList pMsg;
	
	PHeadSubSetB((LPBYTE)&pMsg, 0xA4, 0x11, sizeof(pMsg));
	memcpy(pMsg.szLeaderName, szLeaderName, MAX_ACCOUNT_LEN + 1);

	g_Log.Add("[PMATCHING][GD][0xA4][0x11] (%s)  ", szLeaderName);
	wsExDbCli.DataSend((char *)&pMsg, pMsg.h.size);
}

// finished o.O

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

