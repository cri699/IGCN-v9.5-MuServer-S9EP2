// EDSprotocol.cpp: implementation of the EDSprotocol class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "EDSprotocol.h"
#include "ServerEngine.h"
#include "DSProtocol.h"
#include "giocp.h"
#include "TLog.h"
#include "winutil.h"
#include "Fprotocol.h"

CExDataServerProtocol::CExDataServerProtocol()
{
	this->m_MapGuildManager.clear();
	this->m_MapUnionManager.clear();
}

CExDataServerProtocol::~CExDataServerProtocol()
{

}

LPGUILD_INFO_STRUCT CExDataServerProtocol::GetGuild(char *szGuild)
{
	char szG[9]={0};
	strncpy(szG, szGuild, 8);
	std::map<std::string, tagGUILD_INFO_STRUCT, strCmp>::iterator it = this->m_MapGuildManager.find(szG);

	if(it == this->m_MapGuildManager.end())
		return NULL;

	return &it->second;
}

LPGUILD_INFO_STRUCT CExDataServerProtocol::GetGuild(int iNumber)
{
	std::map<std::string, tagGUILD_INFO_STRUCT, strCmp>::iterator it, end;
	it = this->m_MapGuildManager.begin();
	end = this->m_MapGuildManager.end();

	for(; it != end; it++)
	{
		if(it->second.m_iNumber == iNumber)
			break;
	}
	
	if(it == end)
		return NULL;

	return &it->second;
}

LPGUILD_MEMBER CExDataServerProtocol::GetGuildMember(char *szGuild, char *szName)
{
	LPGUILD_INFO_STRUCT lpGuild;

	lpGuild = GetGuild(szGuild);
	if( lpGuild == FALSE )
		return NULL;

	char szN[11]={0};
	strncpy(szN, szName, 10);

	MAP_GUILD_MEMBER::iterator it;
	it = lpGuild->m_mapGuildMember.find(szN);

	if(it == lpGuild->m_mapGuildMember.end())
		return NULL;

	return &it->second;
}

LPGUILD_MEMBER CExDataServerProtocol::GetGuildMember(int nGuildNumber, char *szName)
{
	LPGUILD_INFO_STRUCT lpGuild;

	lpGuild = GetGuild(nGuildNumber);
	if (lpGuild == FALSE)
		return NULL;

	char szN[11] = { 0 };
	strncpy(szN, szName, 10);

	MAP_GUILD_MEMBER::iterator it;
	it = lpGuild->m_mapGuildMember.find(szN);

	if (it == lpGuild->m_mapGuildMember.end())
		return NULL;

	return &it->second;
}

int CExDataServerProtocol::GetGuildMemberCount(char *szGuildName)
{
	LPGUILD_INFO_STRUCT lpGuild;

	lpGuild = GetGuild(szGuildName);
	if (lpGuild == FALSE)
		return 0;

	return (int)lpGuild->m_mapGuildMember.size();
}

int CExDataServerProtocol::GetGuildMemberCount(int nGuildNumber)
{
	LPGUILD_INFO_STRUCT lpGuild;

	lpGuild = GetGuild(nGuildNumber);
	if (lpGuild == FALSE)
		return 0;

	return (int)lpGuild->m_mapGuildMember.size();
}

BOOL CExDataServerProtocol::GuildExists(char *szGuild)
{
	if(this->GetGuild(szGuild) != NULL)
		return TRUE;
	else
		return FALSE;
}

BOOL CExDataServerProtocol::GuildMemberExists(char *szGuild, char *szName)
{
	if(this->GetGuildMember(szGuild, szName) != NULL)
		return TRUE;
	else
		return FALSE;
}

LPUNION_DATA CExDataServerProtocol::GetUnionData(int iGuild)
{
	std::map<int, tagUNION_DATA>::iterator it;
	it = this->m_MapUnionManager.find(iGuild);
	if(it == this->m_MapUnionManager.end())
		return NULL;

	return &it->second;
}

BOOL CExDataServerProtocol::DBConnect()
{
	if (this->m_GuildDB.Connect(g_MuOnlineDNS, g_UserID, g_Password, g_ServerName) == FALSE)
	{
		g_Log.AddC(TColor::Red, "[ERROR] - EXDATASERVER CANNOT CONNECT TO MSSQL (GUILD)");
		return FALSE;
	}

	if (this->m_UnionDB.Connect(g_MuOnlineDNS, g_UserID, g_Password, g_ServerName) == FALSE)
	{
		g_Log.AddC(TColor::Red, "[ERROR] - EXDATASERVER CANNOT CONNECT TO MSSQL (UNION)");
		return FALSE;
	}

	if (this->m_GMatchingDB.Connect(g_MuOnlineDNS, g_UserID, g_Password, g_ServerName) == FALSE)
	{
		g_Log.AddC(TColor::Red, "[ERROR] - EXDATASERVER CANNOT CONNECT TO MSSQL (GUILD MATCHING)");
		return FALSE;
	}

	if (this->m_PMatchingDB.Connect(g_MuOnlineDNS, g_UserID, g_Password, g_ServerName) == FALSE)
	{
		g_Log.AddC(TColor::Red, "[ERROR] - EXDATASERVER CANNOT CONNECT TO MSSQL (PARTY MATCHING)");
		return FALSE;
	}

	g_Log.AddC(TColor::Green, "[SUCCESS] - EXDATASERVER CONNECT MSSQL SUCCESS");

	return TRUE;
}

BOOL CExDataServerProtocol::Init()
{
	if (!g_UseExDataServer || g_DSMode == TRUE)
	{
		return FALSE;
	}

	if(this->DBConnect() == FALSE)
		return FALSE;

	DWORD dwStartTickCount = GetTickCount();

	CQuery * TempGuild = new CQuery;
	CQuery * TempMember = new CQuery;
	CQuery * TempMark = new CQuery;
	CQuery * TempNotice = new CQuery;

	TempGuild->Connect(g_MuOnlineDNS, g_UserID, g_Password, g_ServerName);
	TempMember->Connect(g_MuOnlineDNS, g_UserID, g_Password, g_ServerName);
	TempMark->Connect(g_MuOnlineDNS, g_UserID, g_Password, g_ServerName);
	TempNotice->Connect(g_MuOnlineDNS, g_UserID, g_Password, g_ServerName);

	this->m_MapGuildManager.clear();

	if(TempGuild->ExecQuery("SELECT * FROM Guild") == FALSE)
	{
		g_Log.AddC(TColor::Red, "[MuOnlineDB] Guild Info is missing");
		TempGuild->Close();
		return FALSE;
	}

	if(TempGuild->Fetch() != SQL_NO_DATA)
	{
		do
		{
			char szMsg[128];

			char szGuildName[9] = {0};
			char szMaster[11] = {0};
			int iNumber, iType, iScore;
			char szNotice[129] = {0};
			BYTE Mark[32] = {0};
			
			tagGUILD_INFO_STRUCT* lpGuildInfo = NULL;
			
			iNumber = TempGuild->GetAsInteger("Number");
			TempGuild->GetAsString("G_Name", szGuildName, sizeof(szGuildName)-1);

			TempGuild->GetAsString("G_Master", szMaster, sizeof(szMaster) -1 );
//			TempGuild->GetAsString("G_Notice", szNotice, sizeof(szNotice) -1);

			wsprintf(szMsg, "SELECT G_Notice FROM Guild Where G_Name = '%s'", szGuildName);
			TempNotice->GetAsBinary(szMsg, (BYTE *)szNotice,sizeof(szNotice));
			
			iScore = TempGuild->GetAsInteger("G_Score");
			iType = TempGuild->GetAsInteger("G_Type");

			ZeroMemory(szMsg, sizeof(szMsg));
			wsprintf(szMsg,"SELECT G_Mark FROM Guild WHERE G_Name = '%s'",szGuildName);
			TempMark->GetAsBinary(szMsg, Mark,sizeof(Mark));

			if(AddGuild(szGuildName, szMaster, iNumber, szNotice, iScore, Mark, iType))
			{
				lpGuildInfo = GetGuild(szGuildName);
				lpGuildInfo->m_iRivalGuild = TempGuild->GetAsInteger("G_Rival");
				lpGuildInfo->m_iUnionGuild = TempGuild->GetAsInteger("G_Union");

//				AddUnion(iNumber, lpGuildInfo->m_iUnionGuild));
			}
			else
			{
				g_Log.AddC(TColor::Red, "Can't add guild [%s][%s]", szGuildName, szMaster);
			}

			TempMember->ExecQuery("SELECT * FROM GuildMember WHERE G_Name = '%s'",szGuildName);

			int iMember = 0;

			while(iMember < MAX_MEMBER_GUILD && TempMember->Fetch() != SQL_NO_DATA)
			{
				int iStatus;
				char szMembName[11] = {0};
				
				iStatus = TempMember->GetAsInteger("G_Status");
				TempMember->GetAsString("Name", szMembName,sizeof(szMembName)-1);

				AddGuildMember(szGuildName, szMembName, iStatus);
				iMember++;
			}

			TempMember->Close();
		}
		while(TempGuild->Fetch() != SQL_NO_DATA);
	}

	TempGuild->Close();
	TempGuild->Disconnect();
	TempMark->Disconnect();
	TempMember->Disconnect();
	TempNotice->Disconnect();

	delete [] TempGuild;
	delete [] TempMark;
	delete [] TempMember;
	delete [] TempNotice;

	// after all guilds info is loaded
	// add unions :D

	std::map<std::string, tagGUILD_INFO_STRUCT, strCmp>::iterator it, end;
	it = this->m_MapGuildManager.begin();
	end = this->m_MapGuildManager.end();

	for(; it != end; it++)
		AddUnion(it->second.m_iNumber, it->second.m_iUnionGuild);

	g_Log.AddC(TColor::Green, "[MuOnlineDB] GuildData Loaded in %d msec", GetTickCount() - dwStartTickCount);

	this->m_FriendSystemEDS.FriendDBConnect();
	this->m_GensSystemEDS.InitGensSystem();
	this->m_PartyMatchingEDS.Init();

	return TRUE;
}

void CExDataServerProtocol::ProtocolCore(int aIndex, BYTE HeadCode, LPBYTE aRecv, int iSize)
{
#if (TRACE_PACKET == 1 )
	LogAddHeadHex("EX_DATA_SERVER", aRecv, iSize);
#endif
	switch ( HeadCode )
	{
		case 0x00:
			m_EXDSProtocol.ExDataServerLogin(aIndex, (SDHP_SERVERINFO *)aRecv);
			break;
		case 0x02:
			m_EXDSProtocol.GDCharCloseRecv(aIndex, (SDHP_USERCLOSE *) aRecv);
			break;
		
		case 0x30:
			m_EXDSProtocol.GDGuildCreateSend(aIndex, (SDHP_GUILDCREATE *)aRecv);
			break;

		case 0x31:
			m_EXDSProtocol.GDGuildDestroyRecv(aIndex, (SDHP_GUILDDESTROY *) aRecv);
			break;

		case 0x32:
			m_EXDSProtocol.GDGuildMemberAdd(aIndex, (SDHP_GUILDMEMBERADD *) aRecv);
			break;
		case 0x33:
			m_EXDSProtocol.GDGuildMemberDel(aIndex, (SDHP_GUILDMEMBERDEL *) aRecv);
			break;

		/*case 0x34:	Packet Sended to Update Guilds - Not Called by any function of GS, so here is useless
			break;*/

		case 0x35:
			m_EXDSProtocol.DGGuildMemberInfoRequest(aIndex, (SDHP_GUILDMEMBER_INFO_REQUEST *) aRecv);
			break;

		case 0x37:
			m_EXDSProtocol.DGGuildScoreUpdate(aIndex, (SDHP_GUILDSCOREUPDATE *) aRecv);
			break;
		case 0x38:
			m_EXDSProtocol.GDGuildNoticeSave(aIndex, (SDHP_GUILDNOTICE *) aRecv);
			break;
		case 0x39:
			m_EXDSProtocol.GDGuildMemberAddWithoutUserIndex(aIndex, (SDHP_GUILDMEMBERADD_WITHOUT_USERINDEX *)aRecv);
			break;

		case 0x50:
			m_EXDSProtocol.GDGuildServerGroupChattingSend(aIndex, (EXSDHP_SERVERGROUP_GUILD_CHATTING_RECV *) aRecv);
			break;
		case 0x51:
			m_EXDSProtocol.GDUnionServerGroupChattingSend(aIndex, (EXSDHP_SERVERGROUP_UNION_CHATTING_RECV *) aRecv);
			break;
		case 0x52:
			m_EXDSProtocol.GDGensServerGroupChattingSend(aIndex, (EXSDHP_SERVERGROUP_GENS_CHATTING_RECV *) aRecv);
			break;
		case 0x53:
			{
				PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;

				switch ( lpDef->subcode )
				{
					case 0x01:
						m_EXDSProtocol.GDReqGuildPeriodBuffInsert(aIndex, (PMSG_REQ_GUILD_PERIODBUFF_INSERT *) aRecv);
						break;
					case 0x02:
						m_EXDSProtocol.GDReqGuildPeriodBuffDelete(aIndex, (PMSG_REQ_GUILD_PERIODBUFF_DELETE *) aRecv);
						break;
				}
			}
			break;	
		case 0xE1:
			m_EXDSProtocol.GDGuildReqAssignStatus(aIndex, (EXSDHP_GUILD_ASSIGN_STATUS_REQ *) aRecv);
			break;
		case 0xE2:
			m_EXDSProtocol.GDGuildReqAssignType(aIndex, (EXSDHP_GUILD_ASSIGN_TYPE_REQ *) aRecv);
			break;

		case 0xE5:
			m_EXDSProtocol.DGRelationShipAnsJoin(aIndex, (EXSDHP_RELATIONSHIP_JOIN_REQ *)aRecv);
			break;

		case 0xE6:
			m_EXDSProtocol.DGRelationShipAnsBreakOff(aIndex, (EXSDHP_RELATIONSHIP_BREAKOFF_REQ *)aRecv);
			break;

		case 0xE9:
			m_EXDSProtocol.DGUnionListRecv(aIndex, (EXSDHP_UNION_LIST_REQ *)aRecv);
			break;

		case 0xEB:
			{
				PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;

				switch ( lpDef->subcode )
				{
					case 0x01:
						m_EXDSProtocol.DGRelationShipAnsKickOutUnionMember(aIndex, (EXSDHP_KICKOUT_UNIONMEMBER_REQ *)aRecv);
						break;
				}
			}
			break;	
		case 0x60:
			m_EXDSProtocol.m_FriendSystemEDS.FriendListRequest(aIndex, (FHP_FRIENDLIST_REQ*)aRecv);
			break;
		case 0x62:
			m_EXDSProtocol.m_FriendSystemEDS.FriendStateClientRecv( aIndex, (FHP_FRIEND_STATE_C*)aRecv);
			break;

		case 0x63:
			m_EXDSProtocol.m_FriendSystemEDS.FriendAddRequest(aIndex, (FHP_FRIEND_ADD_REQ*)aRecv);
			break;

		case 0x64:
			m_EXDSProtocol.m_FriendSystemEDS.WaitFriendAddRequest( aIndex, (FHP_WAITFRIEND_ADD_REQ*)aRecv);
			break;

		case 0x65:
			m_EXDSProtocol.m_FriendSystemEDS.FriendDelRequest(aIndex, (FHP_FRIEND_ADD_REQ*)aRecv);
			break;
		case 0x70:
			m_EXDSProtocol.m_FriendSystemEDS.FriendMemoSend( aIndex, (FHP_FRIEND_MEMO_SEND*)aRecv);
			break;
		case 0x71:
			m_EXDSProtocol.m_FriendSystemEDS.FriendMemoListReq( aIndex, (FHP_FRIEND_MEMO_LIST_REQ*)aRecv);
			break;
		case 0x72:
			m_EXDSProtocol.m_FriendSystemEDS.FriendMemoReadReq( aIndex, (FHP_FRIEND_MEMO_RECV_REQ*)aRecv);
			break;
		case 0x73:
			m_EXDSProtocol.m_FriendSystemEDS.FriendMemoDelReq( aIndex, (FHP_FRIEND_MEMO_DEL_REQ*)aRecv);
			break;

		case 0x66:
			m_EXDSProtocol.FriendChatRoomCreateReq(aIndex, (FHP_FRIEND_CHATROOM_CREATE_REQ *) aRecv);
			break;

		case 0x74:
			m_EXDSProtocol.FriendChatRoomInvitationReq(aIndex, (FHP_FRIEND_INVITATION_REQ *) aRecv);
			break;

		case 0xA0:
			m_EXDSProtocol.FriendChatRoomCreateAns(aIndex, (FCHP_CHATROOM_CREATE_RESULT*) aRecv);
			break;

		case 0xA3:
			{
				PMSG_DEFAULT2 * lpMsg = (PMSG_DEFAULT2 *)(aRecv);

				switch (lpMsg->subcode)
				{
					case 0x00:
						m_EXDSProtocol.GDReqGuildMatchingList(aIndex, (_stReqGuildMatchingList *)aRecv);
						break;
					case 0x01:
						m_EXDSProtocol.GDReqGuildMatchingListSearchWord(aIndex, (_stReqGuildMatchingListSearchWord *)aRecv);
						break;
					case 0x02:
						m_EXDSProtocol.GDReqRegGuildMatchingList(aIndex, (_stReqGuildMatchingData *)aRecv);
						break;
					case 0x03:
						m_EXDSProtocol.GDReqDelMatchingList(aIndex, (_stReqDelGuildMatchingList *)aRecv);
						break;
					case 0x04:
						m_EXDSProtocol.GDReqJoinGuildMatchingList(aIndex, (_stRegWaitGuildMatching *)aRecv);
						break;
					case 0x05:
						m_EXDSProtocol.GDReqCancelJoinGuildMatching(aIndex, (_stReqDelWaitGuildMatchingList *)aRecv);
						break;
					case 0x06:
						m_EXDSProtocol.GDReqAllowJoinGuildMatching(aIndex, (_stReqAllowJoinGuildMatching *)aRecv);
						break;
					case 0x07:
						m_EXDSProtocol.GDReqWaitGuildMatchingList(aIndex, (_stReqWaitGuildMatchingList *)aRecv);
						break;
					case 0x08:
						m_EXDSProtocol.GDReqGetWaitStateListGuildMatching(aIndex, (_stReqWaitStateListGuildMatching *)aRecv);
						break;
				}
			}
			break;

		case 0xA4:
			{
				PMSG_DEFAULT2 * lpMsg = (PMSG_DEFAULT2 *)(aRecv);

				switch (lpMsg->subcode)
				{
					case 0x00:
						m_EXDSProtocol.GDReqRegWantedPartyMember(aIndex, (_stReqRegWantedPartyMember *)aRecv);
						break;
					case 0x01:
						m_EXDSProtocol.GDReqGetPartyMatchingList(aIndex, (_stReqGetPartyMatchingList *)aRecv);
						break;
					case 0x02:
						m_EXDSProtocol.GDReqJoinMemberPartyMatching(aIndex, (_stReqJoinMemberPartyMatching *)aRecv);
						break;
					case 0x03:
						m_EXDSProtocol.GDReqJoinMemberStateList(aIndex, (_stReqJoinMemberStateListPartyMatching *)aRecv);
						break;
					case 0x04:
						m_EXDSProtocol.GDReqJoinMemberStateListLeader(aIndex, (_stReqWaitListPartyMatching *)aRecv);
						break;
					case 0x05:
						m_EXDSProtocol.GDReqAcceptMemberJoin(aIndex, (_stReqAddPartyMember *)aRecv);
						break;
					case 0x06:
						m_EXDSProtocol.GDReqCancelPartyMatching(aIndex, (_stReqCancelPartyMatching *)aRecv);
						break;
					case 0x07:
						m_EXDSProtocol.GDReqDeletePartyUser(aIndex, (_stReqDelPartyUserPartyMatching *)aRecv);
						break;
					case 0x11:
						m_EXDSProtocol.GDReqSendPartyMemberList(aIndex, (_stReqSendPartyMemberList *)aRecv);
						break;
					case 0x12:
						m_EXDSProtocol.GDSendChatMsgPartyMatching(aIndex, (_stReqChattingPartyMatching *)aRecv);
						break;
				}
			}
			break;

		case 0xF8:
			{
				PMSG_DEFAULT2 * lpMsg = (PMSG_DEFAULT2 *)(aRecv);

				switch ( lpMsg->subcode )
				{
					case 0x01:
						m_EXDSProtocol.m_GensSystemEDS.GDReqGensInfo(aIndex, (_tagPMSG_REQ_ABUSING_INFO *) aRecv);
						break;

					case 0x03:
						m_EXDSProtocol.m_GensSystemEDS.GDReqRegGensMember(aIndex, (_tagPMSG_REQ_REG_GENS_MEMBER_EXDB *) aRecv);
						break;

					case 0x05:
						m_EXDSProtocol.m_GensSystemEDS.GDReqSecedeGensMember(aIndex, (_tagPMSG_REQ_SECEDE_GENS_MEMBER_EXDB *) aRecv);
						break;

					case 0x07:
						m_EXDSProtocol.m_GensSystemEDS.GDReqSaveContributePoint(aIndex, (_tagPMSG_REQ_SAVE_CONTRIBUTE_POINT_EXDB *) aRecv);
						break;

					case 0x08:
						m_EXDSProtocol.m_GensSystemEDS.GDReqSaveAbusingKillUserName(aIndex, (_tagPMSG_REQ_SAVE_ABUSING_KILLUSER_EXDB *)aRecv);
						break;
						
					case 0x09:
						m_EXDSProtocol.m_GensSystemEDS.GDReqAbusingInfo(aIndex, (_tagPMSG_REQ_ABUSING_INFO *)aRecv);
						break;

					case 0x0C:
						m_EXDSProtocol.m_GensSystemEDS.GDReqGensRewardCheck(aIndex, (_tagPMSG_REQ_GENS_REWARD_CHECK_EXDB *)aRecv);
						break;

					case 0x0E:
						m_EXDSProtocol.m_GensSystemEDS.GDReqGensRewardComplete(aIndex, (_tagPMSG_REQ_GENS_REWARD_COMPLETE_EXDB *)aRecv);
						break;

					case 0x0F:
						m_EXDSProtocol.m_GensSystemEDS.GDReqGensMemberCount(aIndex, (_tagPMSG_REQ_GENS_MEMBER_COUNT *)aRecv);
						break;

					case 0x11:
						m_EXDSProtocol.m_GensSystemEDS.GDReqGensMemberCount(aIndex, (_tagPMSG_REQ_GENS_MEMBER_COUNT *)aRecv);
						break;

					case 0x12:
						m_EXDSProtocol.m_GensSystemEDS.ManualRefreshRanking(1);
						break;

					case 0x13:
						m_EXDSProtocol.m_GensSystemEDS.GDReqGensRewardDay(aIndex, (_tagPMSG_REQ_GENS_REWARD_DAY *)aRecv);
						break;

				}
			}
		break;
						

	}
}

void CExDataServerProtocol::ExDataServerLogin(int aIndex, SDHP_SERVERINFO * lpMsg)
{
	SDHP_RESULT pResult = {0};

	PHeadSetB((LPBYTE)&pResult, 0x00, sizeof(pResult));
	pResult.Result = 1;

	// Case GameServer
	if ( lpMsg->Type == 1 )
	{
		int i;
		for(i=0; i < g_dwMaxServerGroups; i++)
		{
			if(g_Server[i].m_Index != -1
				&& g_Server[i].m_State == SS_GUILD
				&& g_Server[i].m_ServerCode == lpMsg->ServerCode)
				break;
		}

		if(i == g_dwMaxServerGroups)
		{
			g_Server[aIndex].m_State = SS_GUILD;
			g_Server[aIndex].m_ServerCode = lpMsg->ServerCode;

			std::map<std::string, tagGUILD_INFO_STRUCT, strCmp>::iterator it, end;
			it = this->m_MapGuildManager.begin();
			end = this->m_MapGuildManager.end();
			for (; it != end; it++)
				it->second.m_bGSHasData = 0;

			g_Log.Add("[ExDB] GameServer with ServerCode[%d] connected.", lpMsg->ServerCode);

		}
		else
		{
			g_Log.AddC(TColor::Red, "[ExDB] GameServer with ServerCode[%d] is already connected.", lpMsg->ServerCode);
			pResult.Result = 0;
		}
	}
	// Case ChatServer
	else if ( lpMsg->Type == 2 )
	{
		g_Server[aIndex].m_State = SS_CHAT;
		g_Log.AddC(TColor::Red, "[ChatServer] Chat Server Connected");
	}

	DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size, __FUNCTION__);
}

void CExDataServerProtocol::GDCharCloseRecv(int aIndex, SDHP_USERCLOSE * aRecv)
{
	if (this->m_PartyMatchingEDS.DisconnectMember(aRecv->CharName) == TRUE)
	{
		PARTYMATCHING_PARTYDATA * lpPartyData = this->m_PartyMatchingEDS.GetPartyDataByMember(aRecv->CharName);

		if (lpPartyData)
		{
			this->SendPartyMatchingMemberList(lpPartyData->szLeaderName, 0);
		}
	}

	this->m_PMatchingDB.ExecQuery("EXEC IGC_DeleteWaitPartyMatchingList '%s'", aRecv->CharName);
	this->m_PMatchingDB.Fetch();
	this->m_PMatchingDB.Close();

	if( aRecv->Type == 1 )
	{
		this->m_FriendSystemEDS.FriendClose(aIndex, (LPBYTE)aRecv);
	}

	else
	{
		this->m_FriendSystemEDS.FriendClose(aIndex, (LPBYTE)aRecv);

		char szGuild[9] = {0};
        char szName[11] = {0};
        strncpy(szGuild, aRecv->GuildName, 8);
        strncpy(szName, aRecv->CharName, 10);

		LPGUILD_MEMBER lpMemb;
		lpMemb = GetGuildMember(aRecv->GuildName, aRecv->CharName);

		if (lpMemb)
		{
			lpMemb->m_btConnected = -1;
			lpMemb->m_iUserIndex = -1;
		}
			

		g_Log.Add("[ExDB] member close: [%s]", aRecv->CharName);

		for(int i=0; i < g_dwMaxServerGroups; i++)
		{
			if(g_Server[i].m_Index != -1
				&& g_Server[i].m_Type == ST_EXDATASERVER
				&& g_Server[i].m_State == SS_GUILD)
			{
				DataSend(i, (LPBYTE)aRecv, aRecv->h.size, __FUNCTION__);
			}
		}
	}
}

void CExDataServerProtocol::GDGuildCreateSend(int aIndex, SDHP_GUILDCREATE * aRecv)
{
	SDHP_GUILDCREATE_RESULT Result;

	PHeadSetB((LPBYTE)&Result, 0x30, sizeof(Result));

	memcpy(Result.Master, aRecv->Master, sizeof(Result.Master));
	memcpy(Result.GuildName, aRecv->GuildName, sizeof(Result.GuildName));
	memcpy(Result.Mark, aRecv->Mark, sizeof(Result.Mark));
	Result.NumberL = aRecv->NumberL;
	Result.NumberH = aRecv->NumberH;

	char szMaster[11];
	memcpy(szMaster, aRecv->Master, 10);

	Result.Result = 0;
	Result.Flag = 1;

	if(strlen(aRecv->GuildName) < 4)
	{
		Result.Result = 2;
		Result.Flag = 1;
		g_Log.AddC(TColor::Firebrick, "GuildName is too short [%s]", aRecv->GuildName);

		g_Log.Add("[Guild Create Request] Send Result [%d].", Result.Result);
		DataSend(aIndex, (LPBYTE)&Result, sizeof(Result), __FUNCTION__);
		return;
	}

	if(strlen(aRecv->Master) < 3)
	{
		Result.Result = 2;
		Result.Flag = 1;
		g_Log.AddC(TColor::Firebrick, "GuildMaster Name is too short [%s]", aRecv->Master);

		g_Log.Add("[Guild Create Request] Send Result [%d].", Result.Result);
		DataSend(aIndex, (LPBYTE)&Result, sizeof(Result), __FUNCTION__);
		return;
	}

	if(!SpaceSyntexCheck(aRecv->GuildName))
	{
		Result.Result = 4;
		Result.Flag = 1;

		g_Log.Add("[Guild Create Request] Send Result [%d].", Result.Result);
		DataSend(aIndex, (LPBYTE)&Result, Result.h.size, __FUNCTION__);
		return;
	}

	else if(!QuoteSpaceSyntexCheck(aRecv->GuildName))
	{
		Result.Result = 5;
		Result.Flag = 1;

		g_Log.Add("[Guild Create Request] Send Result [%d].", Result.Result);
		DataSend(aIndex, (LPBYTE)&Result, Result.h.size, __FUNCTION__);
		return;
	}

	else if(!PercentSyntaxCheck(aRecv->GuildName))
	{
		Result.Result = 5;
		Result.Flag = 1;

		g_Log.Add("[Guild Create Request] Send Result [%d].", Result.Result);
		DataSend(aIndex, (LPBYTE)&Result, Result.h.size, __FUNCTION__);
		return;
	}

	else
	{
		g_Log.Add("[Guild Create Request] Guild [%s], GuildMaster [%s].", Result.GuildName, Result.Master);
		int res = CreateDBGuild(aRecv->GuildName, aRecv->Master, aRecv->Mark);
		
		switch(res)
		{
			case 1:
				Result.Result = 0;
			break;
			
			case 2:
				Result.Result = 3;
			break;

			case 3:
				Result.Result = -1;
			break;
		}

		if(res == 0)
		{
			Result.GuildNumber = GetDBGuildNumber(aRecv->GuildName);
			if(Result.GuildNumber != -1)
			{
				AddGuild(aRecv->GuildName, aRecv->Master, Result.GuildNumber, NULL, 0, aRecv->Mark, aRecv->btGuildType);
				if(UpdateGuildMemberStatus(aRecv->GuildName, aRecv->Master, 128))
				{
					if(UpdateGuildType(aRecv->GuildName, aRecv->btGuildType))
					{
				
						Result.Result = 1;

						for(int i=0; i < g_dwMaxServerGroups; i++)
						{
							if(g_Server[i].m_Index != -1
								&& g_Server[i].m_Type == ST_EXDATASERVER
								&& g_Server[i].m_State == SS_GUILD)
							{
								if(i == aIndex)
									Result.Flag = 1;
								else
									Result.Flag = 0;
								g_Log.Add("[Guild Create Request] Send Result [%d] to Server [%d]", Result.Result, g_Server[i].m_ServerCode);			
								DataSend(i, (LPBYTE)&Result, Result.h.size, __FUNCTION__);
							}
						}
						
						DGGuildMasterListSend(aRecv->GuildName);
						SendGuildMemberInfo(aIndex, aRecv->Master, MAKE_NUMBERW(Result.NumberH, Result.NumberL));
						g_Log.Add("[Guild Create Request] Create Guild: [%s], Master: [%s]", aRecv->GuildName, aRecv->Master);
						return;
					}
					else
					{
						g_Log.Add("[Guild Create Request] [ERROR] UpdateGuildType: Guild: [%s], Type: [%d]", aRecv->GuildName, aRecv->btGuildType);
					}
				}
				else
				{
					g_Log.Add("[Guild Create Request] [ERROR] UpdateGuildMemberStatus: Guild: [%s], Member: [%s]", aRecv->GuildName, aRecv->Master);
				}
			}
		}
		else
		{
			g_Log.Add("[Guild Create Request] Send Result [%d].", Result.Result);
			DataSend(aIndex, (LPBYTE)&Result, Result.h.size, __FUNCTION__);
			return;
		}
	}
}

int CExDataServerProtocol::CreateDBGuild(char* szGuild, char* szMaster, LPBYTE pMark)
{
	if(this->GuildExists(szGuild))
	{
		g_Log.AddC(TColor::Firebrick, "Guild [%s] already exists.", szGuild);
		return 1;
	}

	// check if player is already in guild
	this->m_GuildDB.ExecQuery("SELECT Name FROM GuildMember where Name='%s'", szMaster);

	if(this->m_GuildDB.Fetch() != SQL_NO_DATA)
	{
		this->m_GuildDB.Close();
		g_Log.AddC(TColor::Firebrick, "[%s] is already in guild", szMaster);
		return 2;
	}

	this->m_GuildDB.Close();

	// create guild
	if(this->m_GuildDB.ExecQuery("EXEC WZ_GuildCreate '%s', '%s'", szGuild, szMaster) == FALSE)
	{
		this->m_GuildDB.Close();
		g_Log.AddC(TColor::Red, "Error executing WZ_GuildCreate");
		return 3;
	}
	
	if(this->m_GuildDB.Fetch() == SQL_NO_DATA)
	{
		this->m_GuildDB.Close();
		g_Log.AddC(TColor::Red, "Error executing WZ_GuildCreate");
		return 3;
	}

	int res = this->m_GuildDB.GetResult(0);
	this->m_GuildDB.Close();

	if( res != 0)
	{
		g_Log.AddC(TColor::Red, "WZ_GuildCreate error: [%d]", res);
		return 3;
	}

	// set G_Mark
	char szTmp[128]={0};
	wsprintf(szTmp, "UPDATE Guild SET G_Mark=? where G_Name='%s'", szGuild);
	this->m_GuildDB.SetAsBinary(szTmp, pMark, 32);

	return 0;
}

int CExDataServerProtocol::GetDBGuildNumber(char *szGuild)
{
	this->m_GuildDB.ExecQuery("SELECT Number FROM Guild where G_Name='%s'", szGuild);

	this->m_GuildDB.Fetch();
	int num = this->m_GuildDB.GetAsInteger("Number");

	this->m_GuildDB.Close();

	return num;
}

BOOL CExDataServerProtocol::AddGuild(char *szGuild, char *szMaster, int iNumber, char *lpNotice, int iScore, LPBYTE pMark, int iType)
{
	if(GuildExists(szGuild))
		return FALSE;

	tagGUILD_INFO_STRUCT Guild;
	Guild.m_iNumber = iNumber;
	strcpy(Guild.m_szGuildName, szGuild);
	strcpy(Guild.m_szGuildMaster, szMaster);
	memcpy(Guild.m_Mark, pMark, 32);
	if(lpNotice)
		strncpy(Guild.m_Notice, lpNotice, 128);
	Guild.m_Score = iScore;
	Guild.m_btGuildType = iType;
	Guild.m_bGSHasData = FALSE;
	
	GUILD_MEMBER memb;
	memb.m_btConnected = -1;
	memb.m_btStatus = 128;
	strncpy(memb.m_szMemberName, szMaster, 10);
	
	Guild.m_mapGuildMember[szMaster] = memb;
	this->m_MapGuildManager[szGuild] = Guild;

	CreateRelationShipData(iNumber);

	g_Log.Add("[AddGuild] Guild [%s][%d] Create Guild Master: [%s]", Guild.m_szGuildName, iNumber, Guild.m_szGuildMaster);

	return TRUE;
}

BOOL CExDataServerProtocol::DelGuild(char *szGuild)
{
	std::map<std::string, tagGUILD_INFO_STRUCT, strCmp>::iterator it;
	it = this->m_MapGuildManager.find(szGuild);
	if(it == this->m_MapGuildManager.end())
		return FALSE;

	it->second.m_mapGuildMember.clear();
	this->m_MapGuildManager.erase(it);

	return TRUE;
}

BOOL CExDataServerProtocol::MasterDBExists(char *szMaster)
{
	BOOL res = FALSE;
	if(this->m_GuildDB.ExecQuery("SELECT G_Master FROM Guild where G_Master='%s'", szMaster))
	{
		if(this->m_GuildDB.Fetch() == SQL_NO_DATA)
			res = FALSE;
		else
			res = TRUE;
	}

	this->m_GuildDB.Close();

	return TRUE;
}

BOOL CExDataServerProtocol::DelDBGuild(char *szGuild)
{
	BOOL res;

	res = this->m_GuildDB.ExecQuery("DELETE FROM Guild where G_Name='%s'", szGuild);
	this->m_GuildDB.Close();

	return res;
}

BOOL CExDataServerProtocol::DelAllDBGuildMember(char *szGuild)
{
	BOOL res;

	res = this->m_GuildDB.ExecQuery("DELETE FROM GuildMember where G_Name='%s'", szGuild);

	this->m_GuildDB.Close();

	return res;
}

BOOL CExDataServerProtocol::AddGuildMember(char *szGuild, char *szName, BYTE btStatus, BYTE btConnected)
{
	LPGUILD_INFO_STRUCT lpGuild;
	lpGuild = this->GetGuild(szGuild);
	if(lpGuild == NULL)
		return FALSE;

	if(this->GuildMemberExists(szGuild, szName))
		return FALSE;

	GUILD_MEMBER Memb;
	strcpy(Memb.m_szMemberName, szName);
	Memb.m_btStatus = btStatus;
	Memb.m_btConnected = btConnected;

	lpGuild->m_mapGuildMember[szName] = Memb;
	
	g_Log.Add("[AddGuildMember] Guild [%s] Member Add: [%s]", szGuild, Memb.m_szMemberName);
	return TRUE;
}

BOOL CExDataServerProtocol::UpdateGuildMemberStatus(char *szGuild, char *szName, BYTE btStatus)
{
	if( this->m_GuildDB.ExecQuery("UPDATE GuildMember SET G_Status=%d WHERE Name='%s'", 
		btStatus, szName) == FALSE)
	{
		this->m_GuildDB.Close();
		return FALSE;
	}
	
	this->m_GuildDB.Close();

	LPGUILD_MEMBER lpMemb;
	lpMemb = GetGuildMember(szGuild, szName);
	if(lpMemb == NULL)
	{
		if(GuildExists(szGuild))
		{
			g_Log.AddC(TColor::Firebrick, "[ERROR] Guild Member Status: member [%s] doesn't exist in guild [%s]", szName, szGuild);
			return FALSE;
		}
		else
		{
			g_Log.AddC(TColor::Firebrick, "[ERROR] Guild Member Status: guild [%s] doesn't exist", szGuild);
			return FALSE;
		}
	}

	lpMemb->m_btStatus = btStatus;

	return TRUE;
}

BOOL CExDataServerProtocol::UpdateGuildType(char *szGuild, BYTE btType)
{
	if( this->m_GuildDB.ExecQuery("UPDATE Guild SET G_Type=%d WHERE G_Name='%s'", 
		btType, szGuild) == FALSE)
	{
		this->m_GuildDB.Close();
		return FALSE;
	}
	
	this->m_GuildDB.Close();

	LPGUILD_INFO_STRUCT lpGuild;
	lpGuild = GetGuild(szGuild);
	
	if(lpGuild == NULL)
	{
		g_Log.AddC(TColor::Red, "[ERROR] Guild Type: guild [%s] doesn't exist", szGuild);
		return FALSE;
	}

	lpGuild->m_btGuildType = btType;

	return TRUE;
}

void CExDataServerProtocol::SendGuildMemberInfo(int aIndex, char *szName, int iUserIndex)
{
//	char szMember[11] = {0};
//	strncpy(szMember, szName, 10);

	char szGuild[11] = {0};
	int iLevel, iStatus;
	if(GetGuildMemberInfo(szName, szGuild, iLevel, iStatus) == FALSE)
	{
		g_Log.Add("[Send Guild Member Info] No info returned from DB.");
		return;
	}

	LPGUILD_INFO_STRUCT lpGuild;
	LPGUILD_MEMBER lpMemb;

	lpGuild = GetGuild(szGuild);
	if(lpGuild == NULL)
	{
		g_Log.Add("[Send Guild Member Info] error: Guild [%s] not found.", szGuild );
		return;
	}

	lpMemb = GetGuildMember(szGuild, szName);
	if(lpMemb == NULL)
	{
		g_Log.Add("[Send Guild Member Info] error: Member [%s] not found in Guild [%s].", szName, szGuild );
		return;
	}

	lpMemb->m_btConnected = g_Server[aIndex].m_ServerCode;
	lpMemb->m_iUserIndex = iUserIndex;

	DGGuildMasterListSend(szGuild);

	DGRelationShipListSend(aIndex, lpGuild->m_iNumber, 1, 0);
	DGRelationShipListSend(aIndex, lpGuild->m_iNumber, 2, 0);

	SDHP_GUILDMEMBER_INFO GuildMembInfo;
	ZeroMemory(&GuildMembInfo, sizeof(GuildMembInfo));
	PHeadSetB((LPBYTE)&GuildMembInfo, 0x35, sizeof(GuildMembInfo));

	GuildMembInfo.btGuildStatus = lpMemb->m_btStatus;
	strncpy(GuildMembInfo.MemberID, lpMemb->m_szMemberName, 10);
	GuildMembInfo.pServer = lpMemb->m_btConnected;
	GuildMembInfo.btGuildType = lpGuild->m_btGuildType;
	strncpy(GuildMembInfo.GuildName, lpGuild->m_szGuildName, 8);

	g_Log.Add("[Send Guild Member Info] Guild information send: Name[%s] Guild[%s]", szName, szGuild);

	for(int i=0; i < g_dwMaxServerGroups; i++)
	{
		if(g_Server[i].m_Index != -1
			&& g_Server[i].m_Type == ST_EXDATASERVER
			&& g_Server[i].m_State == SS_GUILD)
			{

				DataSend(i, (LPBYTE)&GuildMembInfo, GuildMembInfo.h.size, __FUNCTION__);
			}
	}

	if (lpMemb->m_btStatus == 128)
	{
		this->SendUseGuildMatchingGuild(lpMemb->m_szMemberName, lpGuild->m_iNumber);
		this->SendNotiGuildMatchingForGuildMaster(lpGuild->m_iNumber);
	}
}

BOOL CExDataServerProtocol::GetGuildMemberInfo(char *szName, OUT char *szGuild, OUT int& riLevel, OUT int& riStatus)
{
	this->m_GuildDB.ExecQuery("SELECT G_Name, G_Level, G_Status FROM GuildMember where Name='%s'",
		szName);

	SQLRETURN res = this->m_GuildDB.Fetch();
	if(res == SQL_NO_DATA && res != SQL_SUCCESS && res != SQL_SUCCESS_WITH_INFO)
	{
		this->m_GuildDB.Close();
		return FALSE;
	}

	this->m_GuildDB.GetAsString("G_Name", szGuild,8);
	riLevel = this->m_GuildDB.GetAsInteger("G_Level");
	riStatus = this->m_GuildDB.GetAsInteger("G_Status");

	this->m_GuildDB.Close();

	return TRUE;
}

void CExDataServerProtocol::DGGuildMasterListSend(char *szGuild)
{
	LPGUILD_INFO_STRUCT lpGuild;
	lpGuild = GetGuild(szGuild);

	if(lpGuild == NULL)
		return;

	if (lpGuild->m_bGSHasData == TRUE)
	{
		return;
	}

	int bufsize = sizeof(SDHP_GUILDALL_COUNT)+sizeof(SDHP_GUILDALL)*MAX_MEMBER_GUILD;
	BYTE *lpData = new BYTE[bufsize];
	ZeroMemory(lpData, bufsize);

	SDHP_GUILDALL_COUNT* lpGuildAllCnt;
	lpGuildAllCnt = (SDHP_GUILDALL_COUNT*)lpData;

	SDHP_GUILDALL* lpGuildAll;
	lpGuildAll = (SDHP_GUILDALL*)(lpData+sizeof(SDHP_GUILDALL_COUNT));

	lpGuildAllCnt->btGuildType = lpGuild->m_btGuildType;
	strncpy(lpGuildAllCnt->GuildName, lpGuild->m_szGuildName, 8);
	strncpy(lpGuildAllCnt->Master, lpGuild->m_szGuildMaster, 10);

	lpGuildAllCnt->iGuildRival = lpGuild->m_iRivalGuild;
	if(lpGuild->m_iRivalGuild)
	{
		LPGUILD_INFO_STRUCT lpG;
		lpG = GetGuild(lpGuild->m_iRivalGuild);
		if(lpG)
			strncpy(lpGuildAllCnt->szGuildRivalName, lpG->m_szGuildName, 8);

	}
	lpGuildAllCnt->iGuildUnion = lpGuild->m_iUnionGuild;

	memcpy(lpGuildAllCnt->Mark, lpGuild->m_Mark, 32);
	lpGuildAllCnt->Number = lpGuild->m_iNumber;
	lpGuildAllCnt->score = lpGuild->m_Score;

	MAP_GUILD_MEMBER::iterator it, end;
	it = lpGuild->m_mapGuildMember.begin();
	end = lpGuild->m_mapGuildMember.end();

	int i;
	for(i=0; i < MAX_MEMBER_GUILD && it != end; it++, i++)
	{
		strncpy(lpGuildAll[i].MemberID, it->second.m_szMemberName, 10);
		lpGuildAll[i].btGuildStatus = it->second.m_btStatus;
		lpGuildAll[i].pServer = it->second.m_btConnected;

		g_Log.Add("[GuildMasterListSend] Guild [%s] Member send[%s]", szGuild, it->second.m_szMemberName);
	}

	lpGuildAllCnt->Count = i;

	int datasize = sizeof(SDHP_GUILDALL_COUNT)+ i*sizeof(SDHP_GUILDALL);

	PHeadSetW((LPBYTE)lpGuildAllCnt, 0x36, datasize);

	for(i=0; i < g_dwMaxServerGroups; i++)
	{
		if(g_Server[i].m_Index != -1
			&& g_Server[i].m_Type == ST_EXDATASERVER 
			&& g_Server[i].m_State == SS_GUILD)
			{

				DataSend(i, (LPBYTE)lpData, datasize, __FUNCTION__);
			}
	}

	delete [] lpData;

	lpGuild->m_bGSHasData = TRUE;
}

void CExDataServerProtocol::DGGuildInfoRequest(int aIndex, SDHP_GUILDMEMBER_INFO_GUILDNAME_REQUEST * aRecv)
{
	LPGUILD_INFO_STRUCT lpGuild;
	lpGuild = GetGuild(aRecv->szGuildName);

	if (lpGuild == NULL)
		return;

	if (lpGuild->m_bGSHasData == TRUE)
	{
		return;
	}

	int bufsize = sizeof(SDHP_GUILDALL_COUNT) + sizeof(SDHP_GUILDALL)*MAX_MEMBER_GUILD;
	BYTE *lpData = new BYTE[bufsize];
	ZeroMemory(lpData, bufsize);

	SDHP_GUILDALL_COUNT* lpGuildAllCnt;
	lpGuildAllCnt = (SDHP_GUILDALL_COUNT*)lpData;

	SDHP_GUILDALL* lpGuildAll;
	lpGuildAll = (SDHP_GUILDALL*)(lpData + sizeof(SDHP_GUILDALL_COUNT));

	lpGuildAllCnt->btGuildType = lpGuild->m_btGuildType;
	strncpy(lpGuildAllCnt->GuildName, lpGuild->m_szGuildName, 8);
	strncpy(lpGuildAllCnt->Master, lpGuild->m_szGuildMaster, 10);

	lpGuildAllCnt->iGuildRival = lpGuild->m_iRivalGuild;
	if (lpGuild->m_iRivalGuild)
	{
		LPGUILD_INFO_STRUCT lpG;
		lpG = GetGuild(lpGuild->m_iRivalGuild);
		if (lpG)
			strncpy(lpGuildAllCnt->szGuildRivalName, lpG->m_szGuildName, 8);

	}
	lpGuildAllCnt->iGuildUnion = lpGuild->m_iUnionGuild;

	memcpy(lpGuildAllCnt->Mark, lpGuild->m_Mark, 32);
	lpGuildAllCnt->Number = lpGuild->m_iNumber;
	lpGuildAllCnt->score = lpGuild->m_Score;

	MAP_GUILD_MEMBER::iterator it, end;
	it = lpGuild->m_mapGuildMember.begin();
	end = lpGuild->m_mapGuildMember.end();

	int i;
	for (i = 0; i < MAX_MEMBER_GUILD && it != end; it++, i++)
	{
		strncpy(lpGuildAll[i].MemberID, it->second.m_szMemberName, 10);
		lpGuildAll[i].btGuildStatus = it->second.m_btStatus;
		lpGuildAll[i].pServer = it->second.m_btConnected;

		g_Log.Add("[GuildMasterListSend] Guild [%s] Member send[%s]", aRecv->szGuildName, it->second.m_szMemberName);
	}

	lpGuildAllCnt->Count = i;

	int datasize = sizeof(SDHP_GUILDALL_COUNT) + i*sizeof(SDHP_GUILDALL);

	PHeadSetW((LPBYTE)lpGuildAllCnt, 0x36, datasize);

	for (i = 0; i < g_dwMaxServerGroups; i++)
	{
		if (g_Server[i].m_Index != -1
			&& g_Server[i].m_Type == ST_EXDATASERVER
			&& g_Server[i].m_State == SS_GUILD)
		{

			DataSend(i, (LPBYTE)lpData, datasize, __FUNCTION__);
		}
	}

	delete[] lpData;

	lpGuild->m_bGSHasData = TRUE;
}

void CExDataServerProtocol::DGGuildMemberInfoRequest(int aIndex, SDHP_GUILDMEMBER_INFO_REQUEST * aRecv)
{
	char szName[11] = {0};
	strncpy(szName, aRecv->MemberID, 10);

	g_Log.Add("[Guild Member Info Request] Member [%s].",szName);

	int iUserIndex = MAKE_NUMBERW(aRecv->NumberH, aRecv->NumberL);

	SendGuildMemberInfo(aIndex, szName, iUserIndex);
}

void CExDataServerProtocol::GDGuildDestroyRecv(int aIndex, SDHP_GUILDDESTROY * aRecv)
{
	SDHP_GUILDDESTROY_RESULT DelResult;
	ZeroMemory(&DelResult, sizeof(DelResult));
	PHeadSetB((LPBYTE)&DelResult, 0x31, sizeof(DelResult));

	strncpy(DelResult.GuildName, aRecv->GuildName, 8);
	strncpy(DelResult.Master, aRecv->Master, 10);
	DelResult.NumberH = aRecv->NumberH;
	DelResult.NumberL = aRecv->NumberL;

	DelResult.Flag = 1;
	DelResult.Result = 0;

	g_Log.Add("[Guild Destroy Request] GuildName [%s], GuildMaster [%s].", DelResult.GuildName, DelResult.Master);

	if(strlen(DelResult.Master) < 3)
	{
		g_Log.AddC(TColor::Firebrick, "error-L3 MasterName < 3: [%s]", DelResult.Master);
		DelResult.Result = 3;
	}

	if(strlen(DelResult.GuildName) < 2)
	{
		g_Log.AddC(TColor::Firebrick, "error-L3 GuildName < 2: [%s]", DelResult.GuildName);
		DelResult.Result = 3;
	}

	if(DelResult.Result == 3)
	{
		for(int i=0; i < g_dwMaxServerGroups; i++)
		{
			if(g_Server[i].m_Index != -1
				&& g_Server[i].m_Type == ST_EXDATASERVER
				&& g_Server[i].m_State == SS_GUILD)
				{
					if(i == aIndex)
						DelResult.Flag = 1;
					else
						DelResult.Flag = 0;

					g_Log.Add("[Guild Destroy Request] Send Result [%d] to Server [%d].", DelResult.Result, g_Server[i].m_ServerCode);
					DataSend(i, (LPBYTE)&DelResult, DelResult.h.size, __FUNCTION__);
				}
		}
		return;
	}

	if(IsCSGuild(aRecv->GuildName))
	{
		DelResult.Result = 3;

		for(int i=0; i < g_dwMaxServerGroups; i++)
		{
			if(g_Server[i].m_Index != -1
				&& g_Server[i].m_Type == ST_EXDATASERVER
				&& g_Server[i].m_State == SS_GUILD)
				{
					if(i == aIndex)
						DelResult.Flag = 1;
					else
						DelResult.Flag = 0;
				
					g_Log.Add("[Guild Destroy Request] Send Result [%d] to Server [%d].", DelResult.Result, g_Server[i].m_ServerCode);
					DataSend(i, (LPBYTE)&DelResult, DelResult.h.size, __FUNCTION__);
				}
		}

		return;
	}

	DelResult.Result = 2;

	LPGUILD_INFO_STRUCT lpGuild;
	lpGuild = GetGuild(DelResult.GuildName);
	if(lpGuild)
	{
		if(lpGuild->m_iUnionGuild != 0)
		{		
			if(lpGuild->m_iUnionGuild != lpGuild->m_iNumber)
			{
				if(UnionBreak(GetGuild(lpGuild->m_iUnionGuild), lpGuild) == 0)
					DelResult.Result = 3;
			}
			else
				DelResult.Result = 3;
		}

		if(lpGuild->m_iRivalGuild != 0)
		{
			if(RivalBreak(lpGuild, GetGuild(lpGuild->m_iRivalGuild)) == 0)
				DelResult.Result = 3;
		}

	}
	else
		g_Log.Add("[Guild Destroy Request] Guild [%s] not found.", DelResult.Result);

	if(DelResult.Result == 3)
	{
		for(int i=0; i < g_dwMaxServerGroups; i++)
		{
			if(g_Server[i].m_Index != -1
				&& g_Server[i].m_Type == ST_EXDATASERVER
				&& g_Server[i].m_State == SS_GUILD)
				{
					if(i == aIndex)
						DelResult.Flag = 1;
					else
						DelResult.Flag = 0;

					g_Log.Add("[Guild Destroy Request] Send Result [%d] to Server [%d].", DelResult.Result, g_Server[i].m_ServerCode);
					DataSend(i, (LPBYTE)&DelResult, DelResult.h.size, __FUNCTION__);
				}
		}
		return;
	}

	if(MasterDBExists(DelResult.Master))
	{
		if(DelAllDBGuildMember(DelResult.GuildName))
		{
			if(DelDBGuild(DelResult.GuildName))
			{
				DelGuild(DelResult.GuildName);
				DelResult.Result = 4;
			}
			else
			{
				g_Log.Add("[Guild Destroy Request] [DelAllDBGuildMember] Failed to delete All GuildMember in DB : %s", DelResult.GuildName);
			}
		}
		else
		{
			g_Log.Add("[Guild Destroy Request] [DelDBGuild] Failed to delete Guild in DB : %s", DelResult.GuildName);
		}
	}
	else
		g_Log.Add("[Guild Destroy Request] GuildMaster [%s] not found in DB.", DelResult.Result);

	for(int i=0; i < g_dwMaxServerGroups; i++)
	{
		if(g_Server[i].m_Index != -1
			&& g_Server[i].m_Type == ST_EXDATASERVER
			&& g_Server[i].m_State == SS_GUILD)
			{
				if(i == aIndex)
					DelResult.Flag = 1;
				else
					DelResult.Flag = 0;

				g_Log.Add("[Guild Destroy Request] Send Result [%d] to Server [%d].", DelResult.Result, g_Server[i].m_ServerCode);
				DataSend(i, (LPBYTE)&DelResult, DelResult.h.size, __FUNCTION__);
			}
	}
}

BOOL CExDataServerProtocol::IsCSGuild(char *szGuild)
{
	if(this->m_GuildDB.ExecQuery("EXEC WZ_CS_CheckSiegeGuildList '%s'", szGuild) == FALSE)
	{
		this->m_GuildDB.Close();
		g_Log.Add("WZ_CS_CheckSiegeList = 1");
		return TRUE;
	}
	else
	{
		BOOL res;
		if(this->m_GuildDB.Fetch() == SQL_NO_DATA)
			res = TRUE;
		else
			res = this->m_GuildDB.GetResult(0);

		this->m_GuildDB.Close();

		g_Log.Add("WZ_CS_CheckSiegeList = %d", res);

		return res;
	}
}

BOOL CExDataServerProtocol::UpdateScore(char *szGuild, int iScore)
{
	if(this->m_GuildDB.ExecQuery("UPDATE Guild SET G_Score=%d WHERE G_Name='%s'", iScore, szGuild))
	{
		this->m_GuildDB.Close();
		LPGUILD_INFO_STRUCT lpGuild;
		lpGuild = GetGuild(szGuild);
		if(lpGuild == NULL)
		{
			g_Log.AddC(TColor::Firebrick, "error-L2: %s Guild not found", szGuild);
			return FALSE;
		}

		lpGuild->m_Score = iScore;

		g_Log.AddC(TColor::Firebrick, "[%s] Guild Score Update: [%d]", szGuild, iScore);
		return TRUE;
	}
	else
	{
		this->m_GuildDB.Close();
		return FALSE;
	}

}

BOOL CExDataServerProtocol::SaveNotice(char *szGuild, char *szNotice)
{

	// set Notice
	char szTmp[256]={0};
	char szDBNotice[129] = {0};

	//sprintf(szDBNotice, szNotice);
	memcpy(szDBNotice,szNotice,strlen(szNotice));
	for(int i=0;i<sizeof(szNotice);i++)
	{
		if(szNotice[i] == '%')
		{
			return TRUE;
		}
	}
	wsprintf(szTmp, "UPDATE Guild SET G_Notice=? where G_Name='%s'", szGuild);

	this->m_GuildDB.SetAsBinary(szTmp, (BYTE *)szDBNotice, 128);

	return TRUE;
}

void CExDataServerProtocol::DGGuildScoreUpdate(int aIndex, SDHP_GUILDSCOREUPDATE * aRecv)
{
	g_Log.Add("[Guild Score Update Request] GuildName [%s], Score [%d]", aRecv->GuildName, aRecv->Score);

	if(strlen(aRecv->GuildName) < 4)
	{
		g_Log.AddC(TColor::Firebrick, "error-L2: [%s] guild name is short", aRecv->GuildName);
		return;
	}

	if(UpdateScore(aRecv->GuildName, aRecv->Score))
	{

		g_Log.Add("[Guild Score Update Request] Score updated.");
		for(int i=0; i < g_dwMaxServerGroups; i++)
		{
			if(g_Server[i].m_Index != -1
				&& g_Server[i].m_Type == ST_EXDATASERVER
				&& g_Server[i].m_State == SS_GUILD)
				{
					DataSend(i, (LPBYTE)aRecv, aRecv->h.size, __FUNCTION__);
				}
		}
	}
	else
		g_Log.Add("[Guild Score Update Request] Failed to update guild score.");
}

void CExDataServerProtocol::GDGuildNoticeSave(int aIndex, SDHP_GUILDNOTICE * aRecv)
{
	g_Log.Add("[Guild Notice Save Request] GuildName [%s].", aRecv->GuildName);

	if(strlen(aRecv->GuildName) < 4)
	{
		g_Log.AddC(TColor::Firebrick, "error-L2: [%s] guild name is short", aRecv->GuildName);
		return;
	}

	if (PercentSyntaxCheck(aRecv->szGuildNotice) == FALSE)
	{
		return;
	}

	if(SaveNotice(aRecv->GuildName, aRecv->szGuildNotice))
	{

		g_Log.Add("[Guild Notice Save Request] Notice Saved.");
		for(int i=0; i < g_dwMaxServerGroups; i++)
		{
			if(g_Server[i].m_Index != -1
				&& g_Server[i].m_Type == ST_EXDATASERVER
				&& g_Server[i].m_State == SS_GUILD)
				{

					DataSend(i, (LPBYTE)aRecv, aRecv->h.size, __FUNCTION__);
				}
		}
	}
	else
		g_Log.Add("[Guild Notice Save Request] Failed to save.");

}

void CExDataServerProtocol::GDGuildServerGroupChattingSend(int aIndex, EXSDHP_SERVERGROUP_GUILD_CHATTING_RECV * aRecv)
{

	for(int i=0; i < g_dwMaxServerGroups; i++)
	{
		if(g_Server[i].m_Index != -1
			&& g_Server[i].m_Type == ST_EXDATASERVER
			&& g_Server[i].m_State == SS_GUILD)
			{
				if(i != aIndex)
					DataSend(i, (LPBYTE)aRecv, aRecv->h.size, __FUNCTION__);
			}
	}
}

void CExDataServerProtocol::GDUnionServerGroupChattingSend(int aIndex,EXSDHP_SERVERGROUP_UNION_CHATTING_RECV * aRecv)
{
	for(int i=0; i < g_dwMaxServerGroups; i++)
	{
		if(g_Server[i].m_Index != -1
			&& g_Server[i].m_Type == ST_EXDATASERVER
			&& g_Server[i].m_State == SS_GUILD)
			{
				if(i != aIndex)
					DataSend(i, (LPBYTE)aRecv, aRecv->h.size, __FUNCTION__);
			}
	}
}

void CExDataServerProtocol::GDGensServerGroupChattingSend(int aIndex,EXSDHP_SERVERGROUP_GENS_CHATTING_RECV * aRecv)
{
	for(int i=0; i < g_dwMaxServerGroups; i++)
	{
		if(g_Server[i].m_Index != -1
			&& g_Server[i].m_Type == ST_EXDATASERVER
			&& g_Server[i].m_State == SS_GUILD)
			{
				if(i != aIndex)
					DataSend(i, (LPBYTE)aRecv, aRecv->h.size, __FUNCTION__);
			}
	}
}

void CExDataServerProtocol::GDReqGuildPeriodBuffInsert(int aIndex, PMSG_REQ_GUILD_PERIODBUFF_INSERT * aRecv)
{
	LPGUILD_INFO_STRUCT lpGuild = this->GetGuild(aRecv->szGuildName);

	if ( !lpGuild )
	{
		return;
	}

	for ( std::map<string, GUILD_MEMBER, strCmp>::iterator It = lpGuild->m_mapGuildMember.begin(); It != lpGuild->m_mapGuildMember.end(); It++ )
	{
		this->m_GuildDB.ExecQuery("EXEC IGC_PeriodBuffInsert '%s', %d, %d, %d, %d, %I64d", It->first.c_str(), aRecv->wBuffIndex, aRecv->btEffectType1, aRecv->btEffectType2, aRecv->dwDuration, aRecv->lExpireDate);
		this->m_GuildDB.Close();
	}
}

void CExDataServerProtocol::GDReqGuildPeriodBuffDelete(int aIndex, PMSG_REQ_GUILD_PERIODBUFF_DELETE * aRecv)
{
	for (int i=0;i<aRecv->btGuildCnt;i++)
	{
		this->m_GuildDB.ExecQuery("EXEC IGC_PeriodBuffDeleteGuild %d", aRecv->wBuffIndex[i]);
		this->m_GuildDB.Close();
	}
}

void CExDataServerProtocol::GDGuildReqAssignStatus(int aIndex, EXSDHP_GUILD_ASSIGN_STATUS_REQ * aRecv)
{
	EXSDHP_GUILD_ASSIGN_STATUS_RESULT Result;
	ZeroMemory(&Result, sizeof(Result));
	PHeadSetB((LPBYTE)&Result, 0xE1, sizeof(Result));

	Result.btGuildStatus = aRecv->btGuildStatus;
	Result.btType = aRecv->btType;
	strncpy(Result.szGuildName, aRecv->szGuildName, 8);
	strncpy(Result.szTargetName, aRecv->szTargetName, 10);
	Result.wUserIndex = aRecv->wUserIndex;

	g_Log.Add("[GuildMember Assign Status Request] GuildName [%s], Member [%s], Status [%d].",
		Result.szGuildName, Result.szTargetName, Result.btGuildStatus);

	Result.btFlag = 1;
	Result.btResult = 0;

	if(UpdateGuildMemberStatus(aRecv->szGuildName, aRecv->szTargetName, aRecv->btGuildStatus))
	{
		Result.btResult = 1;
	}

	g_Log.Add("[GuildMember Assign Status Request] Send Result [%d].", Result.btResult);
	DataSend(aIndex, (LPBYTE)&Result, Result.h.size, __FUNCTION__);

	if(Result.btResult)
	{
		Result.btFlag = 0;
		for(int i=0; i < g_dwMaxServerGroups; i++)
		{
			if(g_Server[i].m_Index != -1
				&& g_Server[i].m_Type == ST_EXDATASERVER
				&& g_Server[i].m_State == SS_GUILD)
				{
					if(i != aIndex)
					{
						g_Log.Add("[GuildMember Assign Status Request] Send Result [%d] to Server [%d].", Result.btResult, g_Server[i].m_ServerCode);
						DataSend(i, (LPBYTE)&Result, Result.h.size, __FUNCTION__);
					}
				}
		}	
	}
}

void CExDataServerProtocol::GDGuildReqAssignType(int aIndex, EXSDHP_GUILD_ASSIGN_TYPE_REQ * aRecv)
{
	EXSDHP_GUILD_ASSIGN_TYPE_RESULT Result;
	ZeroMemory(&Result, sizeof(Result));
	PHeadSetB((LPBYTE)&Result, 0xE2, sizeof(Result));

	Result.btGuildType = aRecv->btGuildType;
	strncpy(Result.szGuildName, aRecv->szGuildName, 8);
	Result.wUserIndex = aRecv->wUserIndex;

	g_Log.Add("[Guild Assign Type Request] GuildName [%s], Type [%d].", Result.szGuildName, Result.btGuildType);

	Result.btFlag = 1;
	Result.btResult = 0;

	if(UpdateGuildType(aRecv->szGuildName, aRecv->btGuildType))
	{
		Result.btResult = 1;
	}

	g_Log.Add("[Guild Assign Type Request] Send Result [%d].", Result.btResult);
	DataSend(aIndex, (LPBYTE)&Result, Result.h.size, __FUNCTION__);

	if(Result.btResult)
	{
		Result.btFlag = 0;
		for(int i=0; i < g_dwMaxServerGroups; i++)
		{
			if(g_Server[i].m_Index != -1
				&& g_Server[i].m_Type == ST_EXDATASERVER
				&& g_Server[i].m_State == SS_GUILD)
				{
					if(i != aIndex)
					{
						g_Log.Add("[Guild Assign Type Request] Send Result [%d] to Server [%d].", Result.btResult, g_Server[i].m_ServerCode);
						DataSend(i, (LPBYTE)&Result, Result.h.size, __FUNCTION__);
					}
				}
		}	
	}

}

BOOL CExDataServerProtocol::DBGuildMemberExists(char *szName)
{
	BOOL res = FALSE;

	if(this->m_GuildDB.ExecQuery("SELECT Name FROM GuildMember where Name='%s'", szName))
	{
		if(this->m_GuildDB.Fetch() == SQL_NO_DATA)
			res = FALSE;
		else
			res = TRUE;
	}

	this->m_GuildDB.Close();

	return res;
}

BOOL CExDataServerProtocol::AddDBGuildMember(char *szGuild, char *szName)
{
	BOOL res = this->m_GuildDB.ExecQuery("INSERT GuildMember (Name, G_Name) VALUES ('%s','%s')", szName, szGuild);
	return res;
}

void CExDataServerProtocol::GDGuildMemberAdd(int aIndex, SDHP_GUILDMEMBERADD * aRecv)
{
	SDHP_GUILDMEMBERADD_RESULT Result;
	ZeroMemory(&Result, sizeof(Result));

	PHeadSetB((LPBYTE)&Result, 0x32, sizeof(Result));

	Result.NumberH = aRecv->NumberH;
	Result.NumberL = aRecv->NumberL;
	strncpy(Result.GuildName, aRecv->GuildName, 8);
	strncpy(Result.MemberID, aRecv->MemberID, 10);

	g_Log.Add("[Guild Join Request] GuildName [%s], Member [%s].", Result.GuildName, Result.MemberID);

	Result.pServer = g_Server[aIndex].m_ServerCode;
	Result.Flag = 1;

	if(strlen(Result.GuildName) < 4)
	{
		Result.Result = 0;
		g_Log.AddC(TColor::Firebrick, "error-L2: [%s] Guild Name is short", Result.GuildName);

		g_Log.Add("[Guild Join Request] Send Result [%d].", Result.Result);
		DataSend(aIndex, (LPBYTE)&Result, Result.h.size, __FUNCTION__);
		return;
	}

	if(strlen(Result.MemberID) < 4)
	{
		Result.Result = 0;
		g_Log.AddC(TColor::Firebrick, "error-L2: [%s] Member Name is short", Result.MemberID);

		g_Log.Add("[Guild Join Request] Send Result [%d].", Result.Result);
		DataSend(aIndex, (LPBYTE)&Result, Result.h.size, __FUNCTION__);
		return;
	}

	if(DBGuildMemberExists(Result.MemberID))
	{
		Result.Result = 4;
		g_Log.Add("[Guild Join Request] [DBGuildMemberExists] Member [%s] is already in guild", Result.MemberID);

		g_Log.Add("[Guild Join Request] Send Result [%d].", Result.Result);
		DataSend(aIndex, (LPBYTE)&Result, Result.h.size, __FUNCTION__);
		return;
	}

	if(AddDBGuildMember(Result.GuildName, Result.MemberID) == FALSE)
	{
		Result.Result = 4;
		g_Log.AddC(TColor::Firebrick, "[Guild Join Request] [AddDBGuildMember] failed to add [%s] in guild [%s]", Result.MemberID, Result.GuildName);

		g_Log.Add("[Guild Join Request] Send Result [%d].", Result.Result);
		DataSend(aIndex, (LPBYTE)&Result, Result.h.size, __FUNCTION__);
		return;
	}
	else
		Result.Result = 1;

	if(AddGuildMember(Result.GuildName, Result.MemberID, 0, g_Server[aIndex].m_ServerCode) == FALSE)
	{
		g_Log.Add("error-L3: Failed to add member [%s] to guild [%s]", Result.MemberID, Result.GuildName);
	}

	for(int i=0; i < g_dwMaxServerGroups; i++)
	{
		if (g_Server[i].m_Index != -1
			&& g_Server[i].m_Type == ST_EXDATASERVER
			&& g_Server[i].m_State == SS_GUILD)
		{
			if (i == aIndex)
				Result.Flag = 1;
			else
				Result.Flag = 0;

			g_Log.Add("[Guild Join Request] Send Result [%d] to Server [%d]", Result.Result, g_Server[i].m_ServerCode);

			DataSend(i, (LPBYTE)&Result, Result.h.size, __FUNCTION__);

		}
	}

	this->UpdateGuildMatchingMemberCount(Result.GuildName, this->GetGuildMemberCount(Result.GuildName));
	SendGuildMemberInfo(aIndex, Result.MemberID, MAKE_NUMBERW(Result.NumberH, Result.NumberL));
}

void CExDataServerProtocol::GDGuildMemberAddWithoutUserIndex(int aIndex, SDHP_GUILDMEMBERADD_WITHOUT_USERINDEX * aRecv)
{
	SDHP_GUILDMEMBERADD_RESULT_WITHOUT_USERINDEX Result;
	ZeroMemory(&Result, sizeof(Result));

	PHeadSetB((LPBYTE)&Result, 0x39, sizeof(Result));

	strncpy(Result.GuildName, aRecv->GuildName, 8);
	strncpy(Result.MemberID, aRecv->MemberID, 10);

	g_Log.Add("[Guild Join Request] GuildName [%s], Member [%s].", Result.GuildName, Result.MemberID);

	Result.pServer = g_Server[aIndex].m_ServerCode;
	Result.Flag = 1;
	Result.Number = -1;

	if (strlen(Result.GuildName) < 4)
	{
		Result.Result = 0;
		g_Log.AddC(TColor::Firebrick, "error-L2: [%s] Guild Name is short", Result.GuildName);

		g_Log.Add("[Guild Join Request] Send Result [%d].", Result.Result);
		DataSend(aIndex, (LPBYTE)&Result, Result.h.size, __FUNCTION__);
		return;
	}

	if (strlen(Result.MemberID) < 4)
	{
		Result.Result = 0;
		g_Log.AddC(TColor::Firebrick, "error-L2: [%s] Member Name is short", Result.MemberID);

		g_Log.Add("[Guild Join Request] Send Result [%d].", Result.Result);
		DataSend(aIndex, (LPBYTE)&Result, Result.h.size, __FUNCTION__);
		return;
	}

	if (DBGuildMemberExists(Result.MemberID))
	{
		Result.Result = 4;
		g_Log.Add("[Guild Join Request] [DBGuildMemberExists] Member [%s] is already in guild", Result.MemberID);

		g_Log.Add("[Guild Join Request] Send Result [%d].", Result.Result);
		DataSend(aIndex, (LPBYTE)&Result, Result.h.size, __FUNCTION__);
		return;
	}

	if (AddDBGuildMember(Result.GuildName, Result.MemberID) == FALSE)
	{
		Result.Result = 4;
		g_Log.AddC(TColor::Firebrick, "[Guild Join Request] [AddDBGuildMember] failed to add [%s] in guild [%s]", Result.MemberID, Result.GuildName);

		g_Log.Add("[Guild Join Request] Send Result [%d].", Result.Result);
		DataSend(aIndex, (LPBYTE)&Result, Result.h.size, __FUNCTION__);
		return;
	}
	else
		Result.Result = 1;

	if (AddGuildMember(Result.GuildName, Result.MemberID, 0, g_Server[aIndex].m_ServerCode) == FALSE)
	{
		g_Log.Add("error-L3: Failed to add member [%s] to guild [%s]", Result.MemberID, Result.GuildName);
	}

	EnterCriticalSection(&this->m_FriendSystemEDS.m_csMapFriendMaster);

	LPFRIEND_MASTER lpUserData = this->m_FriendSystemEDS.GetFriendMaster(Result.MemberID);

	if (lpUserData)
	{
		Result.Number = lpUserData->m_iNumber;
	}

	LeaveCriticalSection(&this->m_FriendSystemEDS.m_csMapFriendMaster);

	for (int i = 0; i < g_dwMaxServerGroups; i++)
	{
		if (g_Server[i].m_Index != -1
			&& g_Server[i].m_Type == ST_EXDATASERVER
			&& g_Server[i].m_State == SS_GUILD)
		{
			if (i == aIndex)
				Result.Flag = 1;
			else
				Result.Flag = 0;

			g_Log.Add("[Guild Join Request] Send Result [%d] to Server [%d]", Result.Result, g_Server[i].m_ServerCode);

			DataSend(i, (LPBYTE)&Result, Result.h.size, __FUNCTION__);

		}
	}

	this->UpdateGuildMatchingMemberCount(Result.GuildName, this->GetGuildMemberCount(Result.GuildName));
}

BOOL CExDataServerProtocol::DelDBGuildMember(char *szName)
{
	BOOL res = this->m_GuildDB.ExecQuery("DELETE FROM GuildMember where Name='%s'", szName);
	return res;
}

BOOL CExDataServerProtocol::DelGuildMember(char *szGuild, char *szName)
{
	LPGUILD_INFO_STRUCT lpGuild;
	lpGuild = GetGuild(szGuild);
	if(lpGuild == NULL)
		return FALSE;

	MAP_GUILD_MEMBER::iterator it, end;
	it = lpGuild->m_mapGuildMember.find(szName);
	end = lpGuild->m_mapGuildMember.end();

	if(it == end)
		return FALSE;

	lpGuild->m_mapGuildMember.erase(it);

	return TRUE;
}

void CExDataServerProtocol::GDGuildMemberDel(int aIndex, SDHP_GUILDMEMBERDEL * aRecv)
{
	SDHP_GUILDMEMBERDEL_RESULT Result;
	ZeroMemory(&Result, sizeof(Result));

	PHeadSetB((LPBYTE)&Result, 0x33, sizeof(Result));

	Result.NumberH = aRecv->NumberH;
	Result.NumberL = aRecv->NumberL;
	strncpy(Result.GuildName, aRecv->GuildName, 8);
	strncpy(Result.MemberID, aRecv->MemberID, 10);

	g_Log.Add("[Guild Member Withdraw Request] GuildName [%s], Member [%s].", Result.GuildName, Result.MemberID);

	Result.Flag = 1;
	Result.Result = 3;

	if(strlen(Result.GuildName) < 4)
	{
		g_Log.AddC(TColor::Firebrick, "error-L2: [%s] Guild Name is short", Result.GuildName);
		
		g_Log.Add("[Guild Member Withdraw Request] Send Result [%d]", Result.Result);
		DataSend(aIndex, (LPBYTE)&Result, Result.h.size, __FUNCTION__);
		return;
	}

	if(strlen(Result.MemberID) < 4)
	{
		g_Log.AddC(TColor::Firebrick, "error-L2: [%s] Member Name is short", Result.MemberID);

		g_Log.Add("[Guild Member Withdraw Request] Send Result [%d]", Result.Result);
		DataSend(aIndex, (LPBYTE)&Result, Result.h.size, __FUNCTION__);
		return;
	}

	if(DelDBGuildMember(Result.MemberID) == FALSE)
	{
		g_Log.Add("[Guild Member Withdraw Request] [DelDBGuildMember] Failed delete member [%s] from guild [%s]", Result.MemberID, Result.GuildName);

		g_Log.Add("[Guild Member Withdraw Request] Send Result [%d]", Result.Result);
		DataSend(aIndex, (LPBYTE)&Result, Result.h.size, __FUNCTION__);
		return;
	}

	if(DelGuildMember(Result.GuildName, Result.MemberID) == FALSE)
	{
		g_Log.Add("[Guild Member Withdraw Request] [DelGuildMember] Guild[%s] Member[%s] delete fail.", Result.GuildName, Result.MemberID);
	}
	else
		Result.Result = 1;

	for(int i=0; i < g_dwMaxServerGroups; i++)
	{
		if(g_Server[i].m_Index != -1
			&& g_Server[i].m_Type == ST_EXDATASERVER
			&& g_Server[i].m_State == SS_GUILD)
			{
				if(i == aIndex)
					Result.Flag = 1;
				else
					Result.Flag = 0;

				g_Log.Add("[Guild Member Withdraw Request] Send Result [%d] to Server[%d]", Result.Result, g_Server[i].m_ServerCode);

				DataSend(i, (LPBYTE)&Result, Result.h.size, __FUNCTION__);
			}
	}

	this->UpdateGuildMatchingMemberCount(Result.GuildName, this->GetGuildMemberCount(Result.GuildName));
}

BOOL CExDataServerProtocol::CreateRelationShipData(int iGuild)
{
	if(GetUnionData(iGuild))
		return FALSE;

	UNION_DATA ud;
	ud.m_vecUnionMember.push_back(iGuild);

	this->m_MapUnionManager[iGuild] = ud;

	g_Log.Add("[CreateRelationShipData] Guild:[%d]", iGuild);

	return TRUE;
}

BOOL DestroyRelationShipData(int iGuild)
{
	return TRUE;
}

void CExDataServerProtocol::DGRelationShipAnsJoin(int aIndex, EXSDHP_RELATIONSHIP_JOIN_REQ * aRecv)
{
	EXSDHP_RELATIONSHIP_JOIN_RESULT Result;

	PHeadSetB((LPBYTE)&Result, 0xE5, sizeof(Result));

	g_Log.Add("[RelationShip Join Request] RelationShipType [%d], RequestGuild [%d], TargetGuild [%d].",
		aRecv->btRelationShipType, aRecv->iRequestGuildNum, aRecv->iTargetGuildNum);

	Result.btRelationShipType = aRecv->btRelationShipType;
	Result.iRequestGuildNum = aRecv->iRequestGuildNum;
	Result.iTargetGuildNum = aRecv->iTargetGuildNum;
	Result.wRequestUserIndex = aRecv->wRequestUserIndex;
	Result.wTargetUserIndex = aRecv->wTargetUserIndex;
	Result.btFlag = 1;
	LPGUILD_INFO_STRUCT lpRG, lpTG;

	lpRG = GetGuild(aRecv->iRequestGuildNum);
	lpTG = GetGuild(aRecv->iTargetGuildNum);

	if(lpRG && lpTG)
	{
		strncpy(Result.szRequestGuildName, lpRG->m_szGuildName, 9);
		strncpy(Result.szTargetGuildName, lpTG->m_szGuildName, 9);
		Result.btResult = RelationShipOperation(aRecv->iRequestGuildNum, aRecv->iTargetGuildNum, aRecv->btRelationShipType, 0);
	}
	else
	{
		if(lpRG == NULL)
			g_Log.Add("[RelationShip Join Request] RequestGuild with number [%d] not found.", aRecv->iRequestGuildNum);

		if(lpTG == NULL)
			g_Log.Add("[RelationShip Join Request] TargetGuild with number [%d] not found.", aRecv->iTargetGuildNum);

	}

	g_Log.Add("[RelationShip Join Request] Send Result [%d]", Result.btResult);
	DataSend(aIndex, (LPBYTE)&Result, Result.h.size, __FUNCTION__);

	if(Result.btResult != 0 && Result.btResult != 0x10)
	{
		Result.btFlag = 0;
		for(int i=0; i < g_dwMaxServerGroups; i++)
		{
			if(g_Server[i].m_Index != -1
				&& g_Server[i].m_Type == ST_EXDATASERVER
				&& g_Server[i].m_State == SS_GUILD)
				{
					if(i != aIndex)
					{
						g_Log.Add("[RelationShip Join Request] Send Result [%d] to Server [%d]", 
							Result.btResult, g_Server[i].m_ServerCode);

						DataSend(i, (LPBYTE)&Result, Result.h.size, __FUNCTION__);
					}
				}
		}
	}
}

void CExDataServerProtocol::DGRelationShipAnsBreakOff(int aIndex, EXSDHP_RELATIONSHIP_BREAKOFF_REQ * aRecv)
{
	EXSDHP_RELATIONSHIP_BREAKOFF_RESULT Result;
	ZeroMemory(&Result, sizeof(Result));

	PHeadSetB((LPBYTE)&Result, 0xE6, sizeof(Result));

	g_Log.Add("[RelationShip Break Request] RelationShipType [%d], RequestGuild [%d], TargetGuild [%d].", 
		aRecv->btRelationShipType, aRecv->iRequestGuildNum, aRecv->iTargetGuildNum);

	Result.btRelationShipType = aRecv->btRelationShipType;
	Result.iRequestGuildNum = aRecv->iRequestGuildNum;
	Result.iTargetGuildNum = aRecv->iTargetGuildNum;
	Result.wRequestUserIndex = aRecv->wRequestUserIndex;
	Result.wTargetUserIndex = aRecv->wTargetUserIndex;
	Result.btFlag = 1;

	LPGUILD_INFO_STRUCT lpRG, lpTG;

	lpRG = GetGuild(aRecv->iRequestGuildNum);
	lpTG = GetGuild(aRecv->iTargetGuildNum);

	if(lpRG && lpTG)
	{
		Result.btResult = RelationShipOperation(aRecv->iRequestGuildNum, aRecv->iTargetGuildNum, aRecv->btRelationShipType, 1);
	}
	else
	{
		if(lpRG == NULL)
			g_Log.Add("[RelationShip Break Request] RequestGuild with number [%d] not found.", aRecv->iRequestGuildNum);

		if(lpTG == NULL)
			g_Log.Add("[RelationShip Break Request] TargetGuild with number [%d] not found.", aRecv->iTargetGuildNum);

	}

	g_Log.Add("[RelationShip Break Request] Send Result [%d]", Result.btResult);
	DataSend(aIndex, (LPBYTE)&Result, Result.h.size, __FUNCTION__);

	if(Result.btResult != 0 && Result.btResult != 0x10)
	{
		Result.btFlag = 0;
		for(int i=0; i < g_dwMaxServerGroups; i++)
		{
			if(g_Server[i].m_Index != -1
				&& g_Server[i].m_Type == ST_EXDATASERVER
				&& g_Server[i].m_State == SS_GUILD)
				{
					if(i != aIndex)
					{
						g_Log.Add("[RelationShip Break Request] Send Result [%d] to Server [%d]", 
							Result.btResult, g_Server[i].m_ServerCode);

						DataSend(i, (LPBYTE)&Result, Result.h.size, __FUNCTION__);
					}
				}
		}
	}

}

void CExDataServerProtocol::DGUnionListRecv(int aIndex, EXSDHP_UNION_LIST_REQ * aRecv)
{
	EXSDHP_UNION_LIST *lpList;
	EXSDHP_UNION_LIST_COUNT *lpListCnt;

	LPUNION_DATA lpUD = NULL;
	int cnt = 0, size, res = 0;

	g_Log.Add("[Union List Request] UnionMasterGuild Number: [%d]", aRecv->iUnionMasterGuildNumber);

	LPGUILD_INFO_STRUCT lpGuild, lpG;

	if(aRecv->iUnionMasterGuildNumber != 0)
	{
		lpGuild = GetGuild(aRecv->iUnionMasterGuildNumber);
		if(lpGuild)
			g_Log.Add("[Union List Request] UnionMasterGuild Name: [%s]", lpGuild->m_szGuildName);
		else
			g_Log.Add("[Union List Request] Guild with number [%d] not found.", aRecv->iUnionMasterGuildNumber);
		lpUD = GetUnionData(aRecv->iUnionMasterGuildNumber);
		cnt = (int)lpUD->m_vecUnionMember.size();
	}
	
	size = sizeof(*lpListCnt)+sizeof(*lpList)*cnt;
	LPBYTE lpData = new BYTE[size];
	ZeroMemory(lpData, size);

	lpListCnt = (EXSDHP_UNION_LIST_COUNT *)lpData;
	lpList = (EXSDHP_UNION_LIST *)(lpData+sizeof(*lpListCnt));
	
	PHeadSetW((LPBYTE)lpListCnt, 0xE9, size);

	lpListCnt->iTimeStamp = GetTickCount();
	lpListCnt->wRequestUserIndex = aRecv->wRequestUserIndex;

	if(lpGuild && cnt != 0)
	{
		LPUNION_DATA lpUD;
		if(lpGuild->m_iUnionGuild)
			lpUD = GetUnionData(lpGuild->m_iUnionGuild);
		else
			lpUD = GetUnionData(lpGuild->m_iNumber);
		if(lpUD)
		{
			int s1 = (int)lpUD->m_vecUnionMember.size();
			lpListCnt->btUnionMemberNum = s1;

			for(int j=0; j < s1; j++)
			{
				lpG = GetGuild(lpUD->m_vecUnionMember[j]);
				if(lpG == NULL)
					continue;

				if(lpG->m_iRivalGuild == 0)
					continue;

				lpG = GetGuild(lpG->m_iRivalGuild);
				if(lpG == NULL)
					continue;

				LPUNION_DATA lpUD2;
				if(lpG->m_iUnionGuild)
					lpUD2 = GetUnionData(lpG->m_iUnionGuild);
				else
					lpUD2 = GetUnionData(lpG->m_iNumber);

				if(lpUD2 == NULL)
					continue;

				lpListCnt->btRivalMemberNum += (unsigned char)lpUD2->m_vecUnionMember.size();
			}
		}

		
		lpListCnt->btCount = cnt;
		for(int i=0; i < cnt; i++)
		{
			lpG = GetGuild(lpUD->m_vecUnionMember[i]);
			if(lpG)
			{
				lpList[i].btMemberNum = (unsigned char)lpG->m_mapGuildMember.size();
				memcpy(lpList[i].Mark, lpG->m_Mark, 32);
				strncpy(lpList[i].szGuildName, lpG->m_szGuildName, 8);
				g_Log.Add("[Union List Request] Send GuildName: [%s].", lpG->m_szGuildName);
			}
		}

		lpListCnt->btResult = 1;
	}

	g_Log.Add("[Union List Request] Send UnionList with result: [%d]", lpListCnt->btResult);

	DataSend(aIndex, lpData, size, __FUNCTION__);

	delete [] lpData;
}

int CExDataServerProtocol::RelationShipOperation(int iReqGuild, int iTargGuild, int relation_type, int operation)
{
	LPGUILD_INFO_STRUCT lpReqGuild;
	LPGUILD_INFO_STRUCT lpTargGuild;

	lpReqGuild = GetGuild(iReqGuild);
	lpTargGuild = GetGuild(iTargGuild);

	if(lpReqGuild == NULL || lpTargGuild == NULL)
		return 0;

	if(IsCSGuild(lpReqGuild->m_szGuildName))
		return 0x10;

	if(IsCSGuild(lpTargGuild->m_szGuildName))
		return 0x10;

	switch(operation)
	{
		case 0:
			return RelationShipJoin(lpReqGuild, lpTargGuild, relation_type);

		case 1:
			return RelationShipBreak(lpTargGuild, lpReqGuild, relation_type);

		default:
			return 0;
	}
}

int CExDataServerProtocol::RelationShipJoin(LPGUILD_INFO_STRUCT lpReqGuild, LPGUILD_INFO_STRUCT lpTargGuild, int type)
{
	switch(type)
	{
		case 1:
			return UnionJoin(lpReqGuild, lpTargGuild);

		case 2:
			return RivalJoin(lpReqGuild, lpTargGuild);

		default:
			return 0;
	}
}

int CExDataServerProtocol::UnionJoin(LPGUILD_INFO_STRUCT lpReqGuild, LPGUILD_INFO_STRUCT lpTargGuild)
{
	if(AddUnion(lpReqGuild, lpTargGuild) == FALSE)
		return 0;

	if(UpdateDBUnion(lpReqGuild->m_iNumber, lpTargGuild->m_iNumber) == FALSE)
		return 0;

	if(UpdateDBUnion(lpTargGuild->m_iNumber, lpTargGuild->m_iNumber) == FALSE)
		return 0;
	
	DGRelationShipListSend(-1, lpTargGuild->m_iNumber, 1, 1);
	DGRelationShipListSend(-1, lpTargGuild->m_iNumber, 2, 1);
	DGRelationShipNotificationSend(lpTargGuild->m_iNumber, 1);

	SendListToAllRivals(lpTargGuild);

	return 1;
}

int CExDataServerProtocol::RivalJoin(LPGUILD_INFO_STRUCT lpReqGuild, LPGUILD_INFO_STRUCT lpTargGuild)
{
	if(lpReqGuild->m_iRivalGuild != 0 || lpTargGuild->m_iRivalGuild != 0)
		return 0;

	if(UpdateDBRival(lpReqGuild->m_iNumber, lpTargGuild->m_iNumber) == FALSE)
		return 0;

	if(UpdateDBRival(lpTargGuild->m_iNumber, lpReqGuild->m_iNumber) == FALSE)
		return 0;

	lpReqGuild->m_iRivalGuild = lpTargGuild->m_iNumber;
	lpTargGuild->m_iRivalGuild = lpReqGuild->m_iNumber;

	DGRelationShipListSend(-1, lpReqGuild->m_iNumber, 2, 1);
	DGRelationShipListSend(-1, lpTargGuild->m_iNumber, 2, 1);

	SendListToAllRivals(lpTargGuild);

	return 1;
}

int CExDataServerProtocol::RelationShipBreak(LPGUILD_INFO_STRUCT lpMasterGuild, LPGUILD_INFO_STRUCT lpKickGuild, int type)
{
	switch(type)
	{
		case 1:
			return UnionBreak(lpMasterGuild, lpKickGuild);

		case 2:
			return RivalBreak(lpMasterGuild, lpKickGuild);

		default:
			return 0;
	}
}

int CExDataServerProtocol::UnionBreak(LPGUILD_INFO_STRUCT lpMasterGuild, LPGUILD_INFO_STRUCT lpKickGuild)
{
	if(lpMasterGuild == NULL || lpKickGuild == NULL)
		return 0;

	if(KickUnion(lpMasterGuild, lpKickGuild) == FALSE)
		return 0;

	if(UpdateDBUnion(lpKickGuild->m_iNumber, 0) == FALSE)
		return 0;

	if(lpMasterGuild->m_iUnionGuild == 0)
	{
		if(UpdateDBUnion(lpMasterGuild->m_iNumber, 0) == FALSE)
			return 0;

		DGRelationShipNotificationSend(lpMasterGuild->m_iNumber, 0x10);
	}

	DGRelationShipListSend(-1, lpMasterGuild->m_iNumber, 1, 1);
	DGRelationShipListSend(-1, lpMasterGuild->m_iNumber, 2, 1);
	DGRelationShipNotificationSend(lpMasterGuild->m_iNumber, 1);

	DGRelationShipListSend(-1, lpKickGuild->m_iNumber, 1, 1);
	DGRelationShipListSend(-1, lpKickGuild->m_iNumber, 2, 1);
	DGRelationShipNotificationSend(lpKickGuild->m_iNumber, 1);

	SendListToAllRivals(lpMasterGuild);
	SendListToAllRivals(lpKickGuild);

	return 1;
}

int CExDataServerProtocol::RivalBreak(LPGUILD_INFO_STRUCT lpMasterGuild, LPGUILD_INFO_STRUCT lpKickGuild)
{
	if(lpMasterGuild == NULL || lpKickGuild == NULL)
		return 0;

	if(lpMasterGuild->m_iRivalGuild != lpKickGuild->m_iNumber ||
		lpKickGuild->m_iRivalGuild != lpMasterGuild->m_iNumber)
		return 0;

	if(UpdateDBRival(lpMasterGuild->m_iNumber, 0) == FALSE)
		return 0;

	if(UpdateDBRival(lpKickGuild->m_iNumber, 0) == FALSE)
		return 0;

	lpMasterGuild->m_iRivalGuild = 0;
	lpKickGuild->m_iRivalGuild = 0;

	DGRelationShipListSend(-1, lpMasterGuild->m_iNumber, 2, 1);
	DGRelationShipListSend(-1, lpKickGuild->m_iNumber, 2, 1);

	return 1;
}

BOOL CExDataServerProtocol::AddUnion(LPGUILD_INFO_STRUCT lpReqGuild, LPGUILD_INFO_STRUCT lpTargGuild)
{
	if(lpReqGuild == NULL || lpTargGuild == NULL)
		return FALSE;

	if( lpReqGuild->m_iUnionGuild != 0)
		return FALSE;

	if( lpTargGuild->m_iUnionGuild != 0 && lpTargGuild->m_iNumber != lpTargGuild->m_iUnionGuild)
		return FALSE;

	if(AddUnion(lpReqGuild->m_iNumber, lpTargGuild->m_iNumber))
	{
		lpReqGuild->m_iUnionGuild = lpTargGuild->m_iNumber;
		lpTargGuild->m_iUnionGuild = lpTargGuild->m_iNumber;
		return TRUE;
	}

	return FALSE;
}

BOOL CExDataServerProtocol::AddUnion(int iReqGuild, int iTargGuild)
{
	if(iReqGuild == iTargGuild)
		return FALSE;

	LPUNION_DATA lpUD = GetUnionData(iTargGuild);
	if(lpUD == NULL)
	{
		if(iTargGuild)
			g_Log.Add("[AddUnion] FAILED - ReqGuild: [%d], TargGuild: [%d]", iReqGuild, iTargGuild);

		return FALSE;
	}

	lpUD->m_vecUnionMember.push_back(iReqGuild);

	g_Log.Add("[AddUnion] SUCCESS - ReqGuild: [%d], TargGuild: [%d]", iReqGuild, iTargGuild);
	return TRUE;

}

BOOL CExDataServerProtocol::KickUnion(LPGUILD_INFO_STRUCT lpMasterGuild, LPGUILD_INFO_STRUCT lpKickGuild)
{
	if(lpMasterGuild == NULL || lpKickGuild == NULL)
		return FALSE;

	if(lpMasterGuild->m_iNumber == lpKickGuild->m_iNumber)
		return FALSE;

	LPUNION_DATA lpUD = GetUnionData(lpMasterGuild->m_iNumber);
	if(lpUD == NULL)
		return FALSE;
	
	int size = (int)lpUD->m_vecUnionMember.size();
	int i;
	for(i=0; i < size; i++)
	{
		if(lpUD->m_vecUnionMember[i] == lpKickGuild->m_iNumber)
			break;
	}

	if(i == size)
		return FALSE;

	lpUD->m_vecUnionMember.erase(lpUD->m_vecUnionMember.begin()+i);

	lpKickGuild->m_iUnionGuild = 0;
	if(lpUD->m_vecUnionMember.size() < 2)
		lpMasterGuild->m_iUnionGuild = 0;

	return TRUE;
}


BOOL CExDataServerProtocol::UpdateDBUnion(int iGuild, int iUnion)
{
	BOOL res = this->m_UnionDB.ExecQuery("UPDATE Guild SET G_Union=%d WHERE Number=%d", iUnion, iGuild);
	return res;
}

BOOL CExDataServerProtocol::UpdateDBRival(int iGuild, int iRival)
{
	BOOL res = this->m_UnionDB.ExecQuery("UPDATE Guild SET G_Rival=%d WHERE Number=%d", iRival, iGuild);
	return res;
}

void CExDataServerProtocol::DGRelationShipListSend(int aIndex, int iGuild, int relation_type, BOOL snd_all)
{
	EXSDHP_UNION_RELATIONSHIP_LIST List;
	ZeroMemory(&List, sizeof(List));

	g_Log.Add("[RelationShip List Send] RelationShipType [%d], GuildNumber [%d].", relation_type, iGuild);

	PHeadSetW((LPBYTE)&List, 0xE7, sizeof(List));
	List.btRelationShipType = relation_type;
	LPGUILD_INFO_STRUCT lpGuild;

	lpGuild = GetGuild(iGuild);
	if(lpGuild == NULL)
	{
		g_Log.Add("[RelationShip List Send] error: Guild with number [%d] not found.", iGuild);
		return;
	}

	if(lpGuild->m_iUnionGuild != 0)
		iGuild = lpGuild->m_iUnionGuild;

	g_Log.Add("[RelationShip List Send] UnionMasterGuild: [%d]", iGuild);

	lpGuild = GetGuild(iGuild);
	if(lpGuild == NULL)
	{
		g_Log.Add("[RelationShip List Send] error: Guild with number [%d] not found.", iGuild);
		return;
	}

	List.iUnionMasterGuildNumber = lpGuild->m_iNumber;

	if(lpGuild->m_iUnionGuild)
		strncpy(List.szUnionMasterGuildName, lpGuild->m_szGuildName, 8);

	if(relation_type == 1)
	{
		LPUNION_DATA lpUD = GetUnionData(lpGuild->m_iNumber);
		if(lpUD == NULL)
		{
			g_Log.Add("[RelationShip List Send] error: No union data for Guild [%s].", lpGuild->m_szGuildName);
			return;
		}

		int size = (int)lpUD->m_vecUnionMember.size();
		int i;
		for(i=0; i < size; i++)
			List.iRelationShipMember[i] = lpUD->m_vecUnionMember[i];	
		
		List.btRelationShipMemberCount = size;
		g_Log.Add("[RelationShip List Send] RelationShipMemberCount: [%d] sent.", size);
	}
	else if(relation_type == 2)
	{

		LPUNION_DATA lpUD = GetUnionData(lpGuild->m_iNumber);
		if(lpUD == NULL)
		{
			g_Log.Add("[RelationShip List Send] error: No union data for Guild [%s].", lpGuild->m_szGuildName);
			return;
		}

		int size = (int)lpUD->m_vecUnionMember.size();
		int i, j;
		LPGUILD_INFO_STRUCT lpG = NULL;
		int cnt = 0;

		for(i=0; i < size; i++)
		{
			lpG = GetGuild(lpUD->m_vecUnionMember[i]);
			if(lpG == NULL)
				continue;

			if(lpG->m_iRivalGuild == 0)
				continue;

			// get rival guild
			lpG = GetGuild(lpG->m_iRivalGuild);

			if(lpG == NULL)
				continue;

			LPUNION_DATA lpUD2 = NULL;

			// get allies of rival guild
			if(lpG->m_iUnionGuild)
				lpG = GetGuild(lpG->m_iUnionGuild);
						
			if(lpG == NULL)
				continue;

			lpUD2 = GetUnionData(lpG->m_iNumber);
			if(lpUD2 == NULL)
				continue;

			// get all allies of rival guild
			int s2 = (int)lpUD2->m_vecUnionMember.size();
			for(j=0; j < s2; j++)
			{
				List.iRelationShipMember[cnt] = lpUD2->m_vecUnionMember[j];
				cnt++;
			}

		}

		List.btRelationShipMemberCount = cnt;

		g_Log.Add("[RelationShip List Send] RelationShipMemberCount[%d] send.", cnt);

/*
		if(lpGuild->m_iRivalGuild)
		{
			LPUNION_DATA lpUD = GetUnionData(lpGuild->m_iRivalGuild);
			if(lpUD == NULL)
				return;

			int size = lpUD->m_vecUnionMember.size();
			int i;
			for(i=0; i < size; i++)
				List.iRelationShipMember[i] = lpUD->m_vecUnionMember[i];
			
			List.btRelationShipMemberCount = size;
		}
		else
		{
			List.btRelationShipMemberCount = 0;
		}
*/
	}

	if(snd_all)
	{
		for(int i=0; i < g_dwMaxServerGroups; i++)
		{
			if(g_Server[i].m_Index != -1
				&& g_Server[i].m_Type == ST_EXDATASERVER
				&& g_Server[i].m_State == SS_GUILD)
				{

					g_Log.Add("[RelationShip List Send] List Send to Server [%d].", g_Server[i].m_ServerCode);
					DataSend(i, (LPBYTE)&List, sizeof(List), __FUNCTION__);
				}
		}
	}
	{
		if(aIndex != -1)
		{
			g_Log.Add("[RelationShip List Send] List Send.");
			DataSend(aIndex, (LPBYTE)&List, sizeof(List), __FUNCTION__);
		}
		else
			g_Log.Add("[RelationShip List Send] error: ServerIndex = -1.");
	}
}

void CExDataServerProtocol::DGRelationShipNotificationSend(int iGuild, int iUpdateFlag)
{
	LPGUILD_INFO_STRUCT lpGuild;
	lpGuild = GetGuild(iGuild);

	g_Log.Add("[RelationShip Notification Send] Guild [%d].", iGuild);

	if(lpGuild == NULL)
	{
		g_Log.Add("[RelationShip Notification Send] error: Guild [%d] not found.", iGuild);
		return;
	}

	if(lpGuild->m_iUnionGuild != 0)
		iGuild = lpGuild->m_iUnionGuild;

	g_Log.Add("[RelationShip Notification Send] UnionMasterGuild: [%d].", iGuild);

	LPUNION_DATA lpUD;

	lpUD = GetUnionData(iGuild);
	if(lpUD == NULL)
	{
		g_Log.Add("[RelationShip Notification Send] error: No union data for Guild [%d].", iGuild);
		return;
	}

	EXSDHP_NOTIFICATION_RELATIONSHIP List;
	ZeroMemory(&List, sizeof(List));

	PHeadSetW((LPBYTE)&List, 0xE8, sizeof(List));
	List.btUpdateFlag = iUpdateFlag;
	int size = (int)lpUD->m_vecUnionMember.size();
	List.btGuildListCount = size;
	
	g_Log.Add("[RelationShip Notification Send] Guild List Count: [%d].", size);

	int i;
	for(i=0; i < size; i++)
	{
		List.iGuildList[i] = lpUD->m_vecUnionMember[i];
	}

	for(i=0; i < g_dwMaxServerGroups; i++)
	{
		if(g_Server[i].m_Index != -1
			&& g_Server[i].m_Type == ST_EXDATASERVER
			&& g_Server[i].m_State == SS_GUILD)
			{

				g_Log.Add("[RelationShip Notification Send] Send to Server [%d].", g_Server[i].m_ServerCode);
				DataSend(i, (LPBYTE)&List, sizeof(List), __FUNCTION__);
			}
	}
}

void CExDataServerProtocol::DGRelationShipAnsKickOutUnionMember(int aIndex, EXSDHP_KICKOUT_UNIONMEMBER_REQ *aRecv)
{
	EXSDHP_KICKOUT_UNIONMEMBER_RESULT Result;
	ZeroMemory(&Result, sizeof(Result));

	PHeadSubSetB((LPBYTE)&Result, 0xEB, 1, sizeof(Result));
	Result.btRelationShipType = aRecv->btRelationShipType;
	Result.btResult = 0;
	strncpy(Result.szUnionMasterGuildName, aRecv->szUnionMasterGuildName, 8);
	strncpy(Result.szUnionMemberGuildName, aRecv->szUnionMemberGuildName, 8);
	Result.wRequestUserIndex = aRecv->wRequestUserIndex;
	Result.btResult = 0;
	Result.btFlag = 1;

	g_Log.Add("[Kick Union Member Request] UnionMasterGuild [%s], UnionMemberGuild [%s]", Result.
		szUnionMasterGuildName, Result.szUnionMemberGuildName);

	LPGUILD_INFO_STRUCT lpMasterGuild, lpKickGuild;
	lpMasterGuild = GetGuild(Result.szUnionMasterGuildName);
	lpKickGuild = GetGuild(Result.szUnionMemberGuildName);
	
	if(lpMasterGuild && lpKickGuild)
	{
		Result.btResult = RelationShipOperation(lpKickGuild->m_iNumber, lpMasterGuild->m_iNumber, aRecv->btRelationShipType, 1);
	}
	else
	{
		if(lpMasterGuild == NULL)
			g_Log.Add("[Kick Union Member Request] error: UnionMasterGuild [%s] not found.", Result.szUnionMasterGuildName);

		if(lpKickGuild)
			g_Log.Add("[Kick Union Member Request] error: UnionMemberGuild [%s] not found.", Result.szUnionMemberGuildName);
	}

	g_Log.Add("[Kick Union Member Request] Send Result [%d].", Result.btResult);
	DataSend(aIndex, (LPBYTE)&Result, Result.h.size, __FUNCTION__);

	if(Result.btResult != 0 && Result.btResult != 0x10)
	{
		Result.btFlag = 0;
		for(int i=0; i < g_dwMaxServerGroups; i++)
		{
			if(g_Server[i].m_Index != -1
				&& g_Server[i].m_Type == ST_EXDATASERVER
				&& g_Server[i].m_State == SS_GUILD)
				{
					if(i != aIndex)
					{
						g_Log.Add("[Kick Union Member Request] Send Result [%d] to Server [%d]", Result.btResult, g_Server[i].m_ServerCode);
						DataSend(i, (LPBYTE)&Result, Result.h.size, __FUNCTION__);
					}
				}
		}
	}
}

void CExDataServerProtocol::SendListToAllRivals(LPGUILD_INFO_STRUCT lpGuild)
{
	if(lpGuild == NULL)
		return;

	if(lpGuild->m_iUnionGuild)
		lpGuild = GetGuild(lpGuild->m_iUnionGuild);

	if(lpGuild == NULL)
		return;

	LPUNION_DATA lpUD = GetUnionData(lpGuild->m_iNumber);
	if(lpUD == NULL)
		return;

	int size = (int)lpUD->m_vecUnionMember.size();
	for(int i=0; i < size; i++)
	{
		LPGUILD_INFO_STRUCT lpG;
		lpG = GetGuild(lpUD->m_vecUnionMember[i]);
		if(lpG == NULL)
			continue;

		if(lpG->m_iRivalGuild)
		{
			DGRelationShipListSend(-1, lpG->m_iRivalGuild, 2, 1);
			DGRelationShipNotificationSend(lpG->m_iRivalGuild, 1);
		}
	}
}

int CExDataServerProtocol::GetChatServer()
{
	for(int i=0; i < g_dwMaxServerGroups; i++)
	{
		if(g_Server[i].m_Index != -1
			&& g_Server[i].m_Type == ST_EXDATASERVER
			&& g_Server[i].m_State == SS_CHAT)
			{
				return i;
			}
	}

	return -1;
}

void CExDataServerProtocol::FCHRoomCreateReq(int aIndex, char *szName, char *szFriendName, short Number, short ServerId, short FriendNumber, short FriendServerId)
{
	g_Log.Add("[ChatServer] Room Create Request Name [%s], FriendName [%s].", szName, szFriendName);
	if(aIndex < 0)
	{
		g_Log.AddC(TColor::Red, "error-L3: ChatServer index: [%d]", aIndex);
		return;
	}

	FCHP_CHATROOM_CREATE_REQ Req;
	ZeroMemory(&Req, sizeof(Req));

	PHeadSetB((LPBYTE)&Req, 0xA0, sizeof(Req));

	strncpy(Req.Name, szName, 10);
	strncpy(Req.FriendName, szFriendName, 10);

	Req.Number = Number;
	Req.FriendNumber = FriendNumber;
	Req.ServerId = ServerId;
	Req.FriendServerId = FriendServerId;
	Req.Type = 1;

	DataSend(aIndex, (LPBYTE)&Req, Req.h.size, __FUNCTION__);
}

void CExDataServerProtocol::FriendChatRoomCreateAns(int aIndex, FCHP_CHATROOM_CREATE_RESULT* lpMsg)
{
	FHP_FRIEND_CHATROOM_CREATE_RESULT Result;
	ZeroMemory(&Result, sizeof(Result));
	
	PHeadSetB((LPBYTE)&Result, 0x66, sizeof(Result));

	Result.Result = lpMsg->Result;
	Result.Number = lpMsg->Number;
	Result.RoomNumber = lpMsg->RoomNumber;
	Result.Ticket = lpMsg->Ticket;
	Result.Type = lpMsg->Type;
	strncpy(Result.Name, lpMsg->Name, 10);
	strncpy(Result.FriendName, lpMsg->FriendName, 10);

	if (aIndex >= 0 && g_Server[aIndex].m_Type == ST_EXDATASERVER && g_Server[aIndex].m_State == SS_CHAT) // fixed
	{
		strncpy(Result.ServerIp, szWANIP, sizeof(Result.ServerIp));
	}
	
	char szName[11] = {0};
	char szFriend[11] = {0};
	strncpy(szName, Result.Name, 10);
	strncpy(szFriend, Result.FriendName, 10);

	if(lpMsg->Result)
	{
		g_Log.Add("[ChatServer] Room Create Result [%d], Name [%s], Friend [%s], Room [%d], Ticket[%d]", 
			lpMsg->Result, szName, szFriend, lpMsg->RoomNumber, lpMsg->Ticket);
	}
	else
	{
		g_Log.Add("[ChatServer] Room Create Result [%d], Name [%s], Friend [%s].", 
			lpMsg->Result, szName, szFriend);

	}

	DataSend(lpMsg->ServerId, (LPBYTE)&Result, Result.h.size, __FUNCTION__);
}

void CExDataServerProtocol::FriendChatRoomCreateReq(int aIndex, FHP_FRIEND_CHATROOM_CREATE_REQ* lpMsg)
{
	FCHP_CHATROOM_CREATE_RESULT Result;
	ZeroMemory(&Result, sizeof(Result));

	strncpy(Result.Name, lpMsg->Name, 10);
	strncpy(Result.FriendName, lpMsg->fName, 10);
	Result.Number = lpMsg->Number;
	Result.RoomNumber = -1;
	Result.ServerId = aIndex;
	Result.Ticket = -1;
	Result.Type = 1;

	char szMaster[11] = {0};
	char szFriend[11] = {0};
	strncpy(szMaster, lpMsg->Name, 10);
	strncpy(szFriend, lpMsg->fName, 10);

	g_Log.Add("[ChatRoom Create Request] Name [%s], FriendName [%s].", szMaster, szFriend);
	int ChatId = GetChatServer();

	if(ChatId < 0)
	{
		Result.Result = 2;
		g_Log.AddC(TColor::Firebrick, "error-L1: ChatServer Not Found.");
		FriendChatRoomCreateAns(-1, &Result);
		return;
	}

	
	Result.Result = -1;


	if(this->m_FriendSystemEDS.FriendExists(szMaster, szFriend) == FALSE)
	{
		g_Log.Add("[ChatRoom Create Request] Friend [%s] not found in [%s]'s friend list.", szFriend, szMaster);
		FriendChatRoomCreateAns(-1, &Result);
		return;
	}

	int FriendServerId = this->m_FriendSystemEDS.ExDBGetIndexByCode(this->m_FriendSystemEDS.GetFriendState(szFriend));
	if(FriendServerId < 0)
	{
		FriendChatRoomCreateAns(-1, &Result);
		return;
	}

	FCHRoomCreateReq(GetChatServer(), szMaster, szFriend, 
		lpMsg->Number, aIndex, this->m_FriendSystemEDS.GetFriendNumber(szFriend), FriendServerId);

}

void CExDataServerProtocol::FCHChatRoomInvitationReq(int aIndex, short RoomNumber,	char *szName, short Number,	short ServerId,	BYTE Type)
{
	g_Log.Add("[ChatServer] Room: [%d] Invitation Request - FriendName: [%s].", RoomNumber, szName);

	if(aIndex < 0)
	{
		g_Log.AddC(TColor::Red, "error-L3: ChatServer index: [%d]", aIndex);
		return;
	}

	FCHP_CHATROOM_INVITATION_REQ Req;
	ZeroMemory(&Req, sizeof(Req));

	PHeadSetB((LPBYTE)&Req, 0xA1, sizeof(Req));

	Req.Number = Number;
	Req.RoomNumber = RoomNumber;
	Req.ServerId = ServerId;
	Req.Type = Type;
	strncpy(Req.Name, szName, 10);

	DataSend(aIndex, (LPBYTE)&Req, Req.h.size, __FUNCTION__);
}

void CExDataServerProtocol::FriendChatRoomInvitationReq(int aIndex, FHP_FRIEND_INVITATION_REQ * lpMsg)
{
	FHP_FRIEND_INVITATION_RET Result;
	ZeroMemory(&Result, sizeof(Result));

	PHeadSetB((LPBYTE)&Result, 0x74, sizeof(Result));
	Result.Number = lpMsg->Number;
	Result.WindowGuid = lpMsg->WindowGuid;
	Result.Result = 0;
	strncpy(Result.Name, lpMsg->Name, 10);

	char szMaster[11] = {0};
	char szFriend[11] = {0};

	strncpy(szMaster, lpMsg->Name, 10);
	strncpy(szFriend, lpMsg->FriendName, 10);

	g_Log.Add("[ChatRoom Invitation Request] Name[%s] Room[%d] FriendName[%s].", szMaster, lpMsg->RoomNumber, szFriend);
	if(this->m_FriendSystemEDS.FriendExists(szMaster, szFriend))
	{
		int FriendServerId = this->m_FriendSystemEDS.ExDBGetIndexByCode(this->m_FriendSystemEDS.GetFriendState(szFriend));
		if(FriendServerId > -1)
		{
			FCHChatRoomInvitationReq(GetChatServer(), lpMsg->RoomNumber, 
				szFriend, this->m_FriendSystemEDS.GetFriendNumber(szFriend), FriendServerId, 1);
			Result.Result = 1;
		}
	}
	
	g_Log.Add("[ChatRoom Invitation Request] Send Result [%d].", Result.Result);
	DataSend(aIndex, (LPBYTE)&Result, Result.h.size, __FUNCTION__);
}

void CExDataServerProtocol::GensManualRefreshRanking(BYTE Type)
{
	if (g_UseExDataServer == TRUE && !g_DSMode)
	{
		this->m_GensSystemEDS.ManualRefreshRanking(Type);
	}
	else
	{
		g_Log.AddC(TColor::Red, "Option available only with ExDataServer enabled");
		return;
	}
}

void CExDataServerProtocol::GensRankingProcess()
{
	if (g_UseExDataServer == TRUE && !g_DSMode)
	{
		this->m_GensSystemEDS.GensRankingProcess();
	}
	else
	{
		return;
	}
}

// Gens System Class

GensSystem_EDS::GensSystem_EDS() : m_RankingLog("Gens_Ranking", ".\\GENS_RANKING_LOG", 256)
{
	InitializeCriticalSection(&this->m_GensCriticalSection);
	memset(this->m_GensRankingData, 0x00, sizeof(this->m_GensRankingData));

	this->m_mapGensRankDuprian.clear();
	this->m_mapGensRankVanert.clear();

	this->m_MainDB = new CQuery();
	this->m_RankingDB_1 = new CQuery();
	this->m_RankingDB_2 = new CQuery();
	this->m_DuprianUserCount = 0;
	this->m_VanertUserCount = 0;
	this->m_LeaveDateDayDiff = 0;
	
	this->m_GensRankingUpdateTimeTick = GetTickCount();
}

GensSystem_EDS::~GensSystem_EDS()
{
	DeleteCriticalSection(&this->m_GensCriticalSection);
	delete this->m_MainDB;
	delete this->m_RankingDB_1;
	delete this->m_RankingDB_2;
};

BOOL GensSystem_EDS::InitGensSystem()
{
	if (!g_UseExDataServer || g_DSMode == TRUE)
	{
		return FALSE;
	}

	g_Log.AddC(TColor::Yellow, "[Gens System] Initializing Gens...");

	g_GensRankingUpdateTime = GetPrivateProfileInt("GensSystem", "GensRankingUpdateTimeHour", 2, ".\\DataServer.ini");
	GetPrivateProfileString("GensSystem", "GensRankingPath", "..\\IGCData\\IGC_GensSystem.xml", g_GensRankingPath, sizeof(g_GensRankingPath), ".\\DataServer.ini");

	this->LoadGensData(g_GensRankingPath);
	this->m_MainDB->Connect(g_MuOnlineDNS, g_UserID, g_Password, g_ServerName);
	this->m_RankingDB_1->Connect(g_MuOnlineDNS, g_UserID, g_Password, g_ServerName);
	this->m_RankingDB_2->Connect(g_MuOnlineDNS, g_UserID, g_Password, g_ServerName);

	this->m_MainDB->ExecQuery("SELECT GensRankingMonth FROM GameServerInfo WHERE Number=0");
	this->m_MainDB->Fetch();

	this->m_RewardMonth = this->m_MainDB->GetAsInteger("GensRankingMonth");
	this->m_MainDB->Close();

	g_Log.AddC(TColor::Yellow, "[Gens System] Current Month: %d", this->m_RewardMonth);

	this->MakeRanking();

	return TRUE;
}

BOOL GensSystem_EDS::LoadGensData(LPSTR lpszFileName)
{
	if (lpszFileName == NULL)
	{
		g_Log.MsgBox("[ GensRanking ] - File load error : File Name Error");
		return FALSE;
	}

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(lpszFileName);

	if (res.status != pugi::status_ok)
	{
		g_Log.MsgBox("[ GensRanking ] - Can't Load %s (%s)", lpszFileName, res.description());
		return FALSE;
	}

	pugi::xml_node main = file.child("GensSystem");
	pugi::xml_node reward = main.child("RewardSettings");

	this->m_GensRewardStartDay = reward.attribute("StartDay").as_int();
	this->m_GensRewardEndDay = reward.attribute("EndDay").as_int();
	this->m_MinRankToGainReward = reward.attribute("MinRankToGainReward").as_int(300);

	pugi::xml_node ranking = main.child("RankingSettings");

	for (pugi::xml_node gens = ranking.child("Gens"); gens; gens = gens.next_sibling())
	{
		int iClass = gens.attribute("Class").as_int();

		if (iClass < 1 || iClass > MAX_GENS_CLASS)
		{
			g_Log.MsgBox("Wrong Gens Class in %s file (%d)", lpszFileName, iClass);
			continue;
		}

		this->m_GensRankingData[iClass - 1].GensClass = iClass;
		this->m_GensRankingData[iClass - 1].ContributePoint = gens.attribute("ReqContributePoint").as_int();
		this->m_GensRankingData[iClass - 1].RankingPosition = gens.attribute("RankingPosition").as_int();
		this->m_GensRankingData[iClass - 1].RewardItemGroup = gens.attribute("RewardItemCat").as_int();
		this->m_GensRankingData[iClass - 1].RewardItemNumber = gens.attribute("RewardItemIndex").as_int();
		this->m_GensRankingData[iClass - 1].RewardItemCount = gens.attribute("RewardItemCount").as_int();
	}

	return TRUE;
}

int GensSystem_EDS::CalcGensClass(int ContributePoint, int Ranking)
{
	if (ContributePoint < this->m_GensRankingData[8].ContributePoint)
	{
		if (ContributePoint < this->m_GensRankingData[9].ContributePoint)
		{
			if (ContributePoint < this->m_GensRankingData[10].ContributePoint)
			{
				if (ContributePoint < this->m_GensRankingData[11].ContributePoint)
				{
					if (ContributePoint < this->m_GensRankingData[12].ContributePoint)
					{
						if (ContributePoint < this->m_GensRankingData[13].ContributePoint)
						{
							return 0;
						}
						else 
						{
							return 14;
						}
					}
					else
					{
						return 13;
					}
				}
				else 
				{
					return 12;
				}
			}
			else
			{
				return 11;
			}
		}
		else
		{
			return 10;
		}
	}
	else
	{
		if (Ranking > this->m_GensRankingData[0].RankingPosition)
		{
			if (Ranking > this->m_GensRankingData[1].RankingPosition)
			{
				if (Ranking > this->m_GensRankingData[2].RankingPosition)
				{
					if (Ranking > this->m_GensRankingData[3].RankingPosition)
					{
						if (Ranking > this->m_GensRankingData[4].RankingPosition)
						{
							if (Ranking > this->m_GensRankingData[5].RankingPosition)
							{
								if (Ranking > this->m_GensRankingData[6].RankingPosition)
								{
									if (Ranking > this->m_GensRankingData[7].RankingPosition)
									{
										return 9;
									}
									else
									{
										return 8;
									}
								}
								else
								{
									return 7;
								}
							}
							else 
							{
								return 6;
							}
						}
						else 
						{
							return 5;
						}
					}
					else
					{
						return 4;
					}
				}
				else
				{
					return 3;
				}
			}
			else 
			{
				return 2;
			}
		}
		else
		{
			return 1;
		}
	}
	return 0;
}

int GensSystem_EDS::GetGensInfluence(char* szUserName)
{
	EnterCriticalSection(&this->m_GensCriticalSection);

	this->m_MainDB->ExecQuery("EXEC IGC_GensLoad '%s'", szUserName);

	if (this->m_MainDB->Fetch() == SQL_NO_DATA)
	{
		this->m_MainDB->Close();
		LeaveCriticalSection(&this->m_GensCriticalSection);
		return -1;
	}

	int Influence = this->m_MainDB->GetAsInteger("Influence");
	this->m_MainDB->Close();

	LeaveCriticalSection(&this->m_GensCriticalSection);
	return Influence;
}

void GensSystem_EDS::GensRankingProcess()
{
	if(GetTickCount() - this->m_GensRankingUpdateTimeTick < g_GensRankingUpdateTime*1000*60*60)
	{
		return;
	}

	this->MakeRanking();

	g_Log.AddC(TColor::Yellow, "[Gens System] Processing Ranking Refresh (by Timer)");
	this->m_GensRankingUpdateTimeTick = GetTickCount();
}

void GensSystem_EDS::GDReqGensInfo(int aIndex, _tagPMSG_REQ_ABUSING_INFO *aRecv)
{
	EnterCriticalSection(&this->m_GensCriticalSection);

	_tagPMSG_ANS_GENS_INFO_EXDB pMsg;
	
	PHeadSubSetB((LPBYTE)&pMsg, 0xF8, 0x02, sizeof(pMsg));

	this->m_MainDB->ExecQuery("EXEC IGC_GensLoad '%s'", aRecv->Name);

	if(this->m_MainDB->Fetch() == SQL_NO_DATA)
	{
		pMsg.iResult = 1;

		pMsg.bInfluence = 0;
		pMsg.iContributePoint = 0;
		pMsg.iGensClass = 0;
		pMsg.iGensRanking = 0;
	}

	else
	{
		pMsg.iResult = 0;

		pMsg.bInfluence = this->m_MainDB->GetAsInteger("Influence");
		pMsg.iContributePoint = this->m_MainDB->GetAsInteger("Points");
		pMsg.iGensClass = this->m_MainDB->GetAsInteger("Class");

		std::map<string, GENS_USER, strCmp>::iterator It;

		if(pMsg.bInfluence == 1)
		{
			It = this->m_mapGensRankDuprian.find(aRecv->Name);

			if(It == this->m_mapGensRankDuprian.end())
			{
				pMsg.iGensRanking = 0;
			}

			else
			{
				pMsg.iGensRanking = It->second.iRank;
			}
		}

		else if(pMsg.bInfluence == 2)
		{
			It = this->m_mapGensRankVanert.find(aRecv->Name);

			if(It == this->m_mapGensRankVanert.end())
			{
				pMsg.iGensRanking = 0;
			}

			else
			{
				pMsg.iGensRanking = It->second.iRank;
			}
		}
			

		g_Log.Add("[Gens System] [%s] Get Data from DB [I:%d] [P:%d] [C:%d] [RANK:%d]", aRecv->Name, pMsg.bInfluence, pMsg.iContributePoint, pMsg.iGensClass, pMsg.iGensRanking);
	}

	this->m_MainDB->Close();

	pMsg.wIndexH = aRecv->bIndexH;
	pMsg.wIndexL = aRecv->bIndexL;

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size, __FUNCTION__);

	LeaveCriticalSection(&this->m_GensCriticalSection);
}

void GensSystem_EDS::GDReqRegGensMember(int aIndex, _tagPMSG_REQ_REG_GENS_MEMBER_EXDB *aRecv)
{
	EnterCriticalSection(&this->m_GensCriticalSection);

	_tagPMSG_ANS_REG_GENS_MEMBER_EXDB pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0xF8, 0x04, sizeof(pMsg));

	this->m_MainDB->ExecQuery("EXEC IGC_GensAdd '%s', %d, %d", aRecv->Name, aRecv->bInfluence, g_GensLeaveAfterDays);
	this->m_MainDB->Fetch();

	pMsg.bResult = this->m_MainDB->GetResult(0);

	this->m_MainDB->Close();

	pMsg.bIndexH = aRecv->bIndexH;
	pMsg.bIndexL = aRecv->bIndexL;
	pMsg.bInfluence = aRecv->bInfluence;

	g_Log.Add("[Gens System] Request to Register: Name:%s Influence:%d -> RESULT: %d", aRecv->Name, aRecv->bInfluence, pMsg.bResult);

	if(pMsg.bResult == 0)
	{
		GENS_USER pUser;

		pUser.iContributePoint = 0;
		pUser.iGensClass = 14;
		pUser.iRank = 0;
		memcpy(pUser.Name, aRecv->Name, 10);
		pUser.Name[10] = 0;

		if(pMsg.bInfluence == 1)
		{
			this->m_mapGensRankDuprian[aRecv->Name] = pUser;
			g_Log.AddC(TColor::Green, "[Gens System] Add New User (DUPRIAN) (NAME:%s)", pUser.Name);
		}
		
		else if(pMsg.bInfluence == 2)
		{
			this->m_mapGensRankVanert[aRecv->Name] = pUser;
			g_Log.AddC(TColor::Blue, "[Gens System] Add New User (VANERT) (NAME:%s)", pUser.Name);
		}
	}

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size, __FUNCTION__);

	LeaveCriticalSection(&this->m_GensCriticalSection);
}
	
void GensSystem_EDS::GDReqSecedeGensMember(int aIndex, _tagPMSG_REQ_SECEDE_GENS_MEMBER_EXDB *aRecv)
{
	EnterCriticalSection(&this->m_GensCriticalSection);

	_tagPMSG_ANS_SECEDE_GENS_MEMBER_EXDB pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0xF8, 0x06, sizeof(pMsg));

	this->m_MainDB->ExecQuery("EXEC IGC_GensDelete '%s'", aRecv->Name);
	this->m_MainDB->Fetch();

	pMsg.bResult = this->m_MainDB->GetResult(0);

	this->m_MainDB->Close();

	pMsg.bIndexH = aRecv->bIndexH;
	pMsg.bIndexL = aRecv->bIndexL;

	g_Log.Add("[Gens System] Request to Secede: Name:%s -> RESULT: %d", aRecv->Name, pMsg.bResult);

	if(pMsg.bResult == 0)
	{
		std::map<string, GENS_USER, strCmp>::iterator It;

		It = this->m_mapGensRankDuprian.find(aRecv->Name);

		if(It != this->m_mapGensRankDuprian.end())
		{
			this->m_mapGensRankDuprian.erase(It);
			g_Log.AddC(TColor::Green, "[Gens System] Delete User (DUPRIAN) (NAME:%s)", aRecv->Name);
		}

		else
		{
			It = this->m_mapGensRankVanert.find(aRecv->Name);

			if(It != this->m_mapGensRankVanert.end())
			{
				this->m_mapGensRankVanert.erase(It);
				g_Log.AddC(TColor::Blue, "[Gens System] Delete User (VANERT) (NAME:%s)", aRecv->Name);
			}
		}
	}
		
	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size, __FUNCTION__);

	LeaveCriticalSection(&this->m_GensCriticalSection);
}

void GensSystem_EDS::GDReqSaveContributePoint(int aIndex, _tagPMSG_REQ_SAVE_CONTRIBUTE_POINT_EXDB *aRecv)
{
	EnterCriticalSection(&this->m_GensCriticalSection);

	this->m_MainDB->ExecQuery("EXEC IGC_GensSave '%s', %d, %d", aRecv->Name, aRecv->iContributePoint, aRecv->iGensClass);
	this->m_MainDB->Fetch();

	int iResult = this->m_MainDB->GetResult(0);
	this->m_MainDB->Close();

	if(iResult == 1)
	{
		g_Log.Add("[Gens System] Save Result: OK (Name:%s ContributePoint:%d GensClass:%d)", aRecv->Name, aRecv->iContributePoint, aRecv->iGensClass);
	}

	else
	{
		g_Log.Add("[Gens System] Save Result: FAIL (Name:%s ContributePoint:%d GensClass:%d)", aRecv->Name, aRecv->iContributePoint, aRecv->iGensClass);
	}

	LeaveCriticalSection(&this->m_GensCriticalSection);
}

void GensSystem_EDS::GDReqSaveAbusingKillUserName(int aIndex, _tagPMSG_REQ_SAVE_ABUSING_KILLUSER_EXDB *aRecv)
{
	EnterCriticalSection(&this->m_GensCriticalSection);

	int Count = 0;

	this->m_MainDB->ExecQuery("DELETE FROM IGC_GensAbuse WHERE Name = '%s'", aRecv->Name);

	while ( strlen(&aRecv->KillName[Count*11]) > 0 )
	{
		this->m_MainDB->ExecQuery("EXEC IGC_GensAbuseSave '%s', '%s', '%d'", aRecv->Name, &aRecv->KillName[Count*11], aRecv->KillCount[Count]);
		this->m_MainDB->Close();
		Count++;
	}

	LeaveCriticalSection(&this->m_GensCriticalSection);
}

void GensSystem_EDS::GDReqAbusingInfo(int aIndex, _tagPMSG_REQ_ABUSING_INFO *aRecv)
{
	EnterCriticalSection(&this->m_GensCriticalSection);

	_tagPMSG_ANS_ABUSING_INFO pMsg;

	this->m_MainDB->ExecQuery("EXEC IGC_GensAbuseLoad '%s'", aRecv->Name);

	memset(pMsg.KillCount, 0x00, sizeof(pMsg.KillCount));
	memset(pMsg.szCharNames, 0x00, sizeof(pMsg.szCharNames));

	pMsg.iKillUserCount = 0;

	while(this->m_MainDB->Fetch() == SQL_SUCCESS)
	{
		this->m_MainDB->GetAsString("KillName", &pMsg.szCharNames[pMsg.iKillUserCount*11], 10);
		pMsg.KillCount[pMsg.iKillUserCount] = this->m_MainDB->GetAsInteger("KillCount");

		pMsg.iKillUserCount++;

		if ( pMsg.iKillUserCount >= 10 )
		{
			break;
		}
	}

	this->m_MainDB->Close();

	memcpy(pMsg.Name, aRecv->Name, 11);
	
	pMsg.bIndexH = aRecv->bIndexH;
	pMsg.bIndexL = aRecv->bIndexL;

	PHeadSubSetB((LPBYTE)&pMsg, 0xF8, 0x0A, sizeof(pMsg));

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size, __FUNCTION__);

	LeaveCriticalSection(&this->m_GensCriticalSection);
}

void GensSystem_EDS::GDReqGensRewardCheck(int aIndex, _tagPMSG_REQ_GENS_REWARD_CHECK_EXDB *aRecv)
{
	EnterCriticalSection(&this->m_GensCriticalSection);

	_tagPMSG_ANS_GENS_REWARD_EXDB pMsg;
	SYSTEMTIME CurrTime;
	GetLocalTime(&CurrTime);

	pMsg.iInfluence = aRecv->iInfluence;
	pMsg.bIndexH = aRecv->wIndexH;
	pMsg.bIndexL = aRecv->wIndexL;

	PHeadSubSetB((LPBYTE)&pMsg, 0xF8, 0x0D, sizeof(pMsg));

	if(aRecv->iInfluence == LEAVED_INFLUENCE || aRecv->iInfluence == NONE_INFLUENCE)
	{
		pMsg.bResult = 6;
		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size, __FUNCTION__);
		LeaveCriticalSection(&this->m_GensCriticalSection);
		return;
	}

	if(!(CurrTime.wDay >= this->m_GensRewardStartDay && CurrTime.wDay <= this->m_GensRewardEndDay))
	{
		pMsg.bResult = 1;
		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size, __FUNCTION__);
		LeaveCriticalSection(&this->m_GensCriticalSection);
		return;
	}

	this->m_MainDB->ExecQuery("EXEC IGC_GensRewardCheck '%s'", aRecv->Name);
	this->m_MainDB->Fetch();

	pMsg.iGensClass = this->m_MainDB->GetAsInteger("Class");
	int Reward = this->m_MainDB->GetAsInteger("Reward");

	if (Reward == 0) // No Reward
	{
		this->m_MainDB->Close();
		pMsg.bResult = 2;
		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size, __FUNCTION__);
		LeaveCriticalSection(&this->m_GensCriticalSection);
		return;
	}

	if (Reward == 2) // Already given
	{
		this->m_MainDB->Close();
		pMsg.bResult = 4;
		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size, __FUNCTION__);
		LeaveCriticalSection(&this->m_GensCriticalSection);
		return;
	}

	pMsg.bResult = 0;

	this->m_MainDB->Close();
	
	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size, __FUNCTION__);

	LeaveCriticalSection(&this->m_GensCriticalSection);
}

void GensSystem_EDS::GDReqGensRewardComplete(int aIndex, _tagPMSG_REQ_GENS_REWARD_COMPLETE_EXDB *aRecv)
{
	EnterCriticalSection(&this->m_GensCriticalSection);
	this->m_MainDB->ExecQuery("EXEC IGC_GensReward '%s'", aRecv->Name);
	this->m_MainDB->Close();
	LeaveCriticalSection(&this->m_GensCriticalSection);
}

void GensSystem_EDS::GDReqGensMemberCount(int aIndex, _tagPMSG_REQ_GENS_MEMBER_COUNT *aRecv)
{
	EnterCriticalSection(&this->m_GensCriticalSection);

	_tagPMSG_ANS_GENS_MEMBER_COUNT_EXDB pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0xF8, 0x10, sizeof(pMsg));

	pMsg.iDuprianMemberCount = this->m_DuprianUserCount;
	pMsg.iVanertMemberCount = this->m_VanertUserCount;

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size, __FUNCTION__);

	LeaveCriticalSection(&this->m_GensCriticalSection);
}

void GensSystem_EDS::GDReqGensRewardDay(int aIndex, _tagPMSG_REQ_GENS_REWARD_DAY *aRecv)
{
	EnterCriticalSection(&this->m_GensCriticalSection);

	_tagPMSG_ANS_GENS_REWARD_DAY_CHECK_EXDB pMsg;
	SYSTEMTIME CurrTime;
	GetLocalTime(&CurrTime);

	PHeadSubSetB((LPBYTE)&pMsg, 0xF8, 0x14, sizeof(pMsg));

	pMsg.bIndexH = aRecv->bIndexH;
	pMsg.bIndexL = aRecv->bIndexL;

	if(CurrTime.wDay >= this->m_GensRewardStartDay && CurrTime.wDay <= this->m_GensRewardEndDay)
	{
		pMsg.iRewardDay = TRUE;
	}

	else
	{
		pMsg.iRewardDay = FALSE;
	}

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size, __FUNCTION__);

	LeaveCriticalSection(&this->m_GensCriticalSection);
}

void GensSystem_EDS::ReloadCurrentMonth()
{
	EnterCriticalSection(&this->m_GensCriticalSection);

	this->m_RankingDB_1->ExecQuery("SELECT GensRankingMonth FROM GameServerInfo WHERE Number=0");
	this->m_RankingDB_1->Fetch();

	this->m_RewardMonth = this->m_RankingDB_1->GetAsInteger("GensRankingMonth");
	this->m_RankingDB_1->Close();

	LeaveCriticalSection(&this->m_GensCriticalSection);
}

void GensSystem_EDS::MakeRanking()
{
	EnterCriticalSection(&this->m_GensCriticalSection);

	g_Log.AddC(TColor::Yellow, "[Gens System] Ranking Making [START]");
	this->m_RankingLog.Output("--------------- [ GENS RANKING START ] ---------------");

	this->m_mapGensRankDuprian.clear();
	this->m_mapGensRankVanert.clear();

	int Position = 1;
	int Class;
	int DBRank;
	float DPoint = 0.0, VPoint = 0.0, AllPoints = 0.0;

	this->m_RankingDB_1->ExecQuery("SELECT * From IGC_Gens WHERE Influence=1 ORDER By Points DESC");

	while(this->m_RankingDB_1->Fetch() != SQL_NO_DATA)
	{
		GENS_USER pRank;

		pRank.Name[MAX_ACCOUNT_LEN] = 0;

		pRank.iContributePoint = this->m_RankingDB_1->GetAsInteger("Points");
		pRank.iRank = Position;

		Class = this->m_RankingDB_1->GetAsInteger("Class");
		DBRank = this->m_RankingDB_1->GetAsInteger("Rank");

		pRank.iGensClass = CalcGensClass(pRank.iContributePoint, pRank.iRank);

		this->m_RankingDB_1->GetAsString("Name", pRank.Name, MAX_ACCOUNT_LEN);

		if(pRank.iGensClass != Class)
		{
			this->m_RankingDB_2->ExecQuery("UPDATE IGC_Gens SET Class=%d WHERE Name = '%s'", pRank.iGensClass, pRank.Name);
		}

		if(pRank.iRank != DBRank)
		{
			this->m_RankingDB_2->ExecQuery("UPDATE IGC_Gens SET Rank=%d WHERE Name = '%s'", pRank.iRank, pRank.Name);
		}

		this->m_mapGensRankDuprian[pRank.Name] = pRank;

		Position++;
		DPoint += pRank.iContributePoint;
	}

	this->m_RankingDB_1->Close();

	Position = 1;

	this->m_RankingDB_1->ExecQuery("SELECT * From IGC_Gens WHERE Influence=2 ORDER By Points DESC");

	while(this->m_RankingDB_1->Fetch() != SQL_NO_DATA)
	{
		GENS_USER pRank;

		pRank.Name[MAX_ACCOUNT_LEN] = 0;

		pRank.iContributePoint = this->m_RankingDB_1->GetAsInteger("Points");
		pRank.iRank = Position;

		Class = this->m_RankingDB_1->GetAsInteger("Class");
		DBRank = this->m_RankingDB_1->GetAsInteger("Rank");

		pRank.iGensClass = CalcGensClass(pRank.iContributePoint, pRank.iRank);
		pRank.iReward = this->m_RankingDB_1->GetAsInteger("Reward");

		this->m_RankingDB_1->GetAsString("Name", pRank.Name, MAX_ACCOUNT_LEN);

		if(pRank.iGensClass != Class)
		{
			this->m_RankingDB_2->ExecQuery("UPDATE IGC_Gens SET Class=%d WHERE Name = '%s'", pRank.iGensClass, pRank.Name);
		}

		if(pRank.iRank != DBRank)
		{
			this->m_RankingDB_2->ExecQuery("UPDATE IGC_Gens SET Rank=%d WHERE Name = '%s'", pRank.iRank, pRank.Name);
		}

		this->m_mapGensRankVanert[pRank.Name] = pRank;

		Position++;
		VPoint += pRank.iContributePoint;
	}

	this->m_RankingDB_1->Close();

	AllPoints = VPoint + DPoint;

	SYSTEMTIME m_CurrentTime;
	GetLocalTime(&m_CurrentTime);

	if(this->m_RewardMonth != m_CurrentTime.wMonth)
	{
		this->m_RewardMonth = m_CurrentTime.wMonth;
		this->m_RankingDB_2->ExecQuery("UPDATE GameServerInfo SET GensRankingMonth=%d WHERE Number=0", this->m_RewardMonth);

		if(m_CurrentTime.wDay >= this->m_GensRewardStartDay && m_CurrentTime.wDay <= this->m_GensRewardEndDay)
		{
			std::map<string, GENS_USER, strCmp>::iterator It;

			for(It = this->m_mapGensRankDuprian.begin(); It != this->m_mapGensRankDuprian.end(); It++)
			{
				if(It->second.iRank <= this->m_MinRankToGainReward)
				{
					if(this->m_GensRankingData[It->second.iGensClass-1].RewardItemCount > 0)
					{
						g_Log.AddC(TColor::Green, "[Gens System] [DUPRIAN] [%s] Reward available [RANK:%d]", It->first.c_str(), It->second.iRank);
						this->m_RankingDB_2->ExecQuery("UPDATE IGC_Gens SET Reward = 1 WHERE Name = '%s'", It->first.c_str());
					}

					else
					{
						this->m_RankingDB_2->ExecQuery("UPDATE IGC_Gens SET Reward = 0 WHERE Name = '%s'", It->first.c_str());
					}
				}

				else
				{
					this->m_RankingDB_2->ExecQuery("UPDATE IGC_Gens SET Reward = 0 WHERE Name = '%s'", It->first.c_str());
				}
			}

			for(It = this->m_mapGensRankVanert.begin(); It != this->m_mapGensRankVanert.end(); It++)
			{
				if(It->second.iRank <= this->m_MinRankToGainReward)
				{
					if(this->m_GensRankingData[It->second.iGensClass-1].RewardItemCount > 0)
					{
						g_Log.AddC(TColor::Blue, "[Gens System] [VANERT] [%s] Reward available [RANK:%d]", It->first.c_str(), It->second.iRank);
						this->m_RankingDB_2->ExecQuery("UPDATE IGC_Gens SET Reward = 1 WHERE Name = '%s'", It->first.c_str());
					}

					else
					{
						this->m_RankingDB_2->ExecQuery("UPDATE IGC_Gens SET Reward = 0 WHERE Name = '%s'", It->first.c_str());
					}
				}

				else
				{
					this->m_RankingDB_2->ExecQuery("UPDATE IGC_Gens SET Reward = 0 WHERE Name = '%s'", It->first.c_str());
				}
			}
		}
	}

	for(std::map<string, GENS_USER, strCmp>::iterator It = this->m_mapGensRankDuprian.begin(); It != this->m_mapGensRankDuprian.end(); It++)
	{
		g_Log.AddC(TColor::Green, "[Gens System] RANKING (DUPRIAN): [NAME:%s] [CONTRIBUTION:%d] [CLASS:%d] [RANK:%d]", It->second.Name, It->second.iContributePoint, It->second.iGensClass, It->second.iRank);
		this->m_RankingLog.Output("RANKING (DUPRIAN): [NAME:%s] [CONTRIBUTION:%d] [CLASS:%d] [RANK:%d]", It->second.Name, It->second.iContributePoint, It->second.iGensClass, It->second.iRank);
	}

	for(std::map<string, GENS_USER, strCmp>::iterator It = this->m_mapGensRankVanert.begin(); It != this->m_mapGensRankVanert.end(); It++)
	{
		g_Log.AddC(TColor::Blue, "[Gens System] RANKING (VANERT): [NAME:%s] [CONTRIBUTION:%d] [CLASS:%d] [RANK:%d]", It->second.Name, It->second.iContributePoint, It->second.iGensClass, It->second.iRank);
		this->m_RankingLog.Output("RANKING (VANERT): [NAME:%s] [CONTRIBUTION:%d] [CLASS:%d] [RANK:%d]", It->second.Name, It->second.iContributePoint, It->second.iGensClass, It->second.iRank);
	}

	float DPercent = 0.0;
	float VPercent = 0.0;

	if (AllPoints > 0.0)
	{
		DPoint *= 100.0 / AllPoints;
		VPoint *= 100.0 / AllPoints;
	}

	g_Log.AddC(TColor::Yellow, "[Gens System] (D:%0.2f) VS (V:%0.2f)", DPercent, VPercent);
	this->m_RankingLog.Output("RANKING RESULTS: (D:%f) VS (V:%f)", DPercent, VPercent);

	g_Log.AddC(TColor::Yellow, "[Gens System] Ranking Making [END]");
	this->m_RankingLog.Output("--------------- [ GENS RANKING END ] ---------------");

	LeaveCriticalSection(&this->m_GensCriticalSection);
}

void GensSystem_EDS::ManualRefreshRanking(BYTE Type)
{
	g_Log.AddC(TColor::Yellow, "[Gens System] Processing Ranking refresh (Manual, by: %s)", Type == 0 ? "System Administrator" : "Game Master");

	this->ReloadCurrentMonth();
	this->MakeRanking();
}

void CExDataServerProtocol::GDReqGuildMatchingList(int aIndex, _stReqGuildMatchingList * lpMsg)
{
	_stAnsGuildMatchingList pMsg;

	this->m_GMatchingDB.ExecQuery("EXEC IGC_GetGuildMatchingList %d, %d", lpMsg->nPage, 9);
	int nCount = 0;

	do
	{
		if (this->m_GMatchingDB.Fetch() == SQL_NO_DATA)
			break;

		pMsg.stGuildMatchingList[nCount].nBoardNumber = this->m_GMatchingDB.GetAsInteger("identNo");
		this->m_GMatchingDB.GetAsString("GuildName", pMsg.stGuildMatchingList[nCount].szGuildName, MAX_GUILD_LEN);
		this->m_GMatchingDB.GetAsString("GuildMasterName", pMsg.stGuildMatchingList[nCount].szRegistrant, MAX_ACCOUNT_LEN);
		pMsg.stGuildMatchingList[nCount].nGuildNumber = this->m_GMatchingDB.GetAsInteger("GuildNum");
		pMsg.stGuildMatchingList[nCount].nGuildMasterLevel = this->m_GMatchingDB.GetAsInteger("GuildMasterLevel");
		pMsg.stGuildMatchingList[nCount].btGuildMasterClass = this->m_GMatchingDB.GetAsInteger("GuildMasterClass");
		pMsg.stGuildMatchingList[nCount].btGuildMemberCnt = this->m_GMatchingDB.GetAsInteger("GuildMemberCnt");
		this->m_GMatchingDB.GetAsString("Memo", pMsg.stGuildMatchingList[nCount].szMemo, sizeof(pMsg.stGuildMatchingList[nCount].szMemo));
		pMsg.stGuildMatchingList[nCount].btInterestType = this->m_GMatchingDB.GetAsInteger("InterestType");
		pMsg.stGuildMatchingList[nCount].btLevelRange = this->m_GMatchingDB.GetAsInteger("LevelRange");
		pMsg.stGuildMatchingList[nCount].btClassType = this->m_GMatchingDB.GetAsInteger("ClassType");
		pMsg.stGuildMatchingList[nCount].btGensType = this->m_GMatchingDB.GetAsInteger("GensType");

		nCount++;

	} while (nCount < 9);

	this->m_GMatchingDB.Close();

	this->m_GMatchingDB.ExecQuery("EXEC IGC_GetGuildMatchingListCount");
	this->m_GMatchingDB.Fetch();

	int nTotalEntry = this->m_GMatchingDB.GetAsInteger("Count");

	this->m_GMatchingDB.Close();

	PHeadSubSetW((LPBYTE)&pMsg, 0xA3, 0x00, sizeof(pMsg));

	pMsg.nUserIndex = lpMsg->nUserIndex;
	pMsg.nPage = lpMsg->nPage;

	int nTotalPage = 0;

	if (nTotalEntry > 0)
	{
		nTotalPage = nTotalEntry / 9;

		if (nTotalEntry % 9 > 0)
		{
			nTotalPage++;
		}

		if (nTotalEntry < 9)
		{
			nTotalPage = 1;
		}
	}

	pMsg.nTotalPage = nTotalPage;
	pMsg.nListCount = nCount;

	if (nCount <= 0)
	{
		pMsg.nResult = -1;
	}

	else
	{
		pMsg.nResult = 0;
	}

	DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg), __FUNCTION__);
}

void CExDataServerProtocol::GDReqGuildMatchingListSearchWord(int aIndex, _stReqGuildMatchingListSearchWord *lpMsg)
{
	_stAnsGuildMatchingList pMsg;

	this->m_GMatchingDB.ExecQuery("EXEC IGC_GetGuildMatchingListKeyword %d, %d, '%s'", lpMsg->nPage, 9, lpMsg->szSearchWord);
	int nCount = 0;

	do
	{
		if (this->m_GMatchingDB.Fetch() == SQL_NO_DATA)
			break;

		pMsg.stGuildMatchingList[nCount].nBoardNumber = this->m_GMatchingDB.GetAsInteger("identNo");
		this->m_GMatchingDB.GetAsString("GuildName", pMsg.stGuildMatchingList[nCount].szGuildName, MAX_GUILD_LEN);
		this->m_GMatchingDB.GetAsString("GuildMasterName", pMsg.stGuildMatchingList[nCount].szRegistrant, MAX_ACCOUNT_LEN);
		pMsg.stGuildMatchingList[nCount].nGuildNumber = this->m_GMatchingDB.GetAsInteger("GuildNum");
		pMsg.stGuildMatchingList[nCount].nGuildMasterLevel = this->m_GMatchingDB.GetAsInteger("GuildMasterLevel");
		pMsg.stGuildMatchingList[nCount].btGuildMasterClass = this->m_GMatchingDB.GetAsInteger("GuildMasterClass");
		pMsg.stGuildMatchingList[nCount].btGuildMemberCnt = this->m_GMatchingDB.GetAsInteger("GuildMemberCnt");
		this->m_GMatchingDB.GetAsString("Memo", pMsg.stGuildMatchingList[nCount].szMemo, sizeof(pMsg.stGuildMatchingList[nCount].szMemo));
		pMsg.stGuildMatchingList[nCount].btInterestType = this->m_GMatchingDB.GetAsInteger("InterestType");
		pMsg.stGuildMatchingList[nCount].btLevelRange = this->m_GMatchingDB.GetAsInteger("LevelRange");
		pMsg.stGuildMatchingList[nCount].btClassType = this->m_GMatchingDB.GetAsInteger("ClassType");
		pMsg.stGuildMatchingList[nCount].btGensType = this->m_GMatchingDB.GetAsInteger("GensType");

		nCount++;

	} while (nCount < 9);

	this->m_GMatchingDB.Close();

	this->m_GMatchingDB.ExecQuery("EXEC IGC_GetGuildMatchingListCount");
	this->m_GMatchingDB.Fetch();

	int nTotalEntry = this->m_GMatchingDB.GetAsInteger("Count");

	this->m_GMatchingDB.Close();

	PHeadSubSetW((LPBYTE)&pMsg, 0xA3, 0x00, sizeof(pMsg));

	pMsg.nUserIndex = lpMsg->nUserIndex;
	pMsg.nPage = lpMsg->nPage;

	int nTotalPage = 0;

	if (nTotalEntry > 0)
	{
		nTotalPage = nTotalEntry / 9;

		if (nTotalEntry % 9 > 0)
		{
			nTotalPage++;
		}

		if (nTotalEntry < 9)
		{
			nTotalPage = 1;
		}
	}

	pMsg.nTotalPage = nTotalPage;
	pMsg.nListCount = nCount;

	if (nCount <= 0)
	{
		pMsg.nResult = -1;
	}

	else
	{
		pMsg.nResult = 0;
	}

	DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg), __FUNCTION__);
}

void CExDataServerProtocol::GDReqRegGuildMatchingList(int aIndex, _stReqGuildMatchingData * lpMsg)
{
	_stAnsGuildMatchingData pMsg;

	this->m_GMatchingDB.ExecQuery("EXEC IGC_InsertGuildMatchingList '%s','%s',%d,%d,%d,%d,'%s',%d,%d,%d,%d",
		lpMsg->_stGuildMatchingList.szGuildName, lpMsg->_stGuildMatchingList.szRegistrant, lpMsg->_stGuildMatchingList.nGuildNumber, lpMsg->_stGuildMatchingList.nGuildMasterLevel,
		lpMsg->_stGuildMatchingList.btGuildMasterClass, lpMsg->_stGuildMatchingList.btGuildMemberCnt, lpMsg->_stGuildMatchingList.szMemo, lpMsg->_stGuildMatchingList.btInterestType,
		lpMsg->_stGuildMatchingList.btLevelRange, lpMsg->_stGuildMatchingList.btClassType, lpMsg->_stGuildMatchingList.btGensType);

	this->m_GMatchingDB.Fetch();
	this->m_GMatchingDB.Close();

	pMsg.nUserIndex = lpMsg->nUserIndex;
	pMsg.nResult = 0;
	PHeadSubSetB((LPBYTE)&pMsg, 0xA3, 0x02, sizeof(pMsg));

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size, __FUNCTION__);
}

void CExDataServerProtocol::GDReqDelMatchingList(int aIndex, _stReqDelGuildMatchingList * lpMsg)
{
	_stAnsDelGuildMatchingList pMsg;
	_stGuildMatchingAllowListDB stAllowList[80];

	this->m_GMatchingDB.ExecQuery("EXEC IGC_DeleteGuildMatchingData %d", lpMsg->nGuildNumber);
	
	this->m_GMatchingDB.Fetch();
	this->m_GMatchingDB.Close();

	pMsg.nResult = 0;
	pMsg.nUserIndex = lpMsg->nUserIndex;
	PHeadSubSetB((LPBYTE)&pMsg, 0xA3, 0x03, sizeof(pMsg));

	LPGUILD_INFO_STRUCT lpGuild = this->GetGuild(lpMsg->nGuildNumber);

	if (!lpGuild)
	{
		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size, __FUNCTION__);
		return;
	}

	memset(&stAllowList, 0x00, sizeof(stAllowList));

	this->m_GMatchingDB.ExecQuery("EXEC IGC_GetWaitGuildMatching %d, %d", lpMsg->nGuildNumber, 0);

	int Count = 0;

	while (this->m_GMatchingDB.Fetch() == SQL_SUCCESS)
	{
		stAllowList[Count].btApplicantClass = this->m_GMatchingDB.GetAsInteger("ApplicantClass");
		stAllowList[Count].nApplicantLevel = this->m_GMatchingDB.GetAsInteger("ApplicantLevel");
		this->m_GMatchingDB.GetAsString("ApplicantName", stAllowList[Count].szApplicantName, sizeof(stAllowList[Count].szApplicantName));

		Count++;
	}

	this->m_GMatchingDB.Close();

	_stAnsNotiGuildMatching pNotiMsg;
	PHeadSubSetB((LPBYTE)&pNotiMsg, 0xA3, 0x09, sizeof(pNotiMsg));

	LPFRIEND_MASTER lpUser = NULL;

	EnterCriticalSection(&this->m_FriendSystemEDS.m_csMapFriendMaster);

	for (int i = 0; i < Count && i < 80; i++)
	{
		lpUser = this->m_FriendSystemEDS.GetFriendMaster(stAllowList[i].szApplicantName);

		if (!lpUser)
		{
			continue;
		}

		int ServerIndex = this->m_FriendSystemEDS.ExDBGetIndexByCode(lpUser->m_iServer);

		if (ServerIndex == -1)
		{
			continue;
		}

		pNotiMsg.nResult = 3;
		pNotiMsg.nUserIndex = lpUser->m_iNumber;

		DataSend(ServerIndex, (LPBYTE)&pNotiMsg, pNotiMsg.h.size, __FUNCTION__);
	}

	LeaveCriticalSection(&this->m_FriendSystemEDS.m_csMapFriendMaster);

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size, __FUNCTION__);
}

void CExDataServerProtocol::GDReqJoinGuildMatchingList(int aIndex, _stRegWaitGuildMatching * lpMsg)
{
	_stAnsWaitGuildMatching pMsg;

	this->m_GMatchingDB.ExecQuery("EXEC IGC_GetGuildMatchingWaitState '%s', %d", lpMsg->stAllowList.szApplicantName, lpMsg->stAllowList.btState);

	int State = -1;

	if (this->m_GMatchingDB.Fetch() != SQL_NO_DATA)
	{
		State = this->m_GMatchingDB.GetAsInteger("State");
	}

	this->m_GMatchingDB.Close();

	if (State >= 0)
	{
		pMsg.nResult = -1;
	}

	else
	{
		if (this->m_GMatchingDB.ExecQuery("EXEC IGC_InsertGuildMatchingWaitList %d,'%s',%d,%d,%d",
			lpMsg->stAllowList.nGuildNumber, lpMsg->stAllowList.szApplicantName, lpMsg->stAllowList.btApplicantClass,
			lpMsg->stAllowList.nApplicantLevel, lpMsg->stAllowList.btState) != TRUE)
		{
			pMsg.nResult = -4;
		}

		else
		{
			pMsg.nResult = 0;
		}

		this->m_GMatchingDB.Fetch();
		this->m_GMatchingDB.Close();
	}

	PHeadSubSetB((LPBYTE)&pMsg, 0xA3, 0x04, sizeof(pMsg));
	pMsg.nUserIndex = lpMsg->nUserIndex;

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size, __FUNCTION__);

	if (pMsg.nResult == 0)
	{
		this->SendNotiGuildMatchingForGuildMaster(lpMsg->stAllowList.nGuildNumber);
	}
}

void CExDataServerProtocol::GDReqCancelJoinGuildMatching(int aIndex, _stReqDelWaitGuildMatchingList * lpMsg)
{
	_stAnsDelWaitGuildMatchingList pMsg;

	if(this->m_GMatchingDB.ExecQuery("EXEC IGC_DelGuildMatchingWaitState '%s'", lpMsg->szName) == SQL_ERROR)
	{
		pMsg.nResult = -1;
	}

	else
	{
		pMsg.nResult = 0;
	}

	this->m_GMatchingDB.Fetch();
	this->m_GMatchingDB.Close();

	PHeadSubSetB((LPBYTE)&pMsg, 0xA3, 0x05, sizeof(pMsg));

	pMsg.nUserIndex = lpMsg->nUserIndex;
	pMsg.nType = lpMsg->nType;

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size, __FUNCTION__);
}

void CExDataServerProtocol::GDReqAllowJoinGuildMatching(int aIndex, _stReqAllowJoinGuildMatching * lpMsg)
{
	_stAnsAllowJoinGuildMatching pMsg;

	// Step 1
	// Check if guild member exists (user is already in guild)

	BOOL bIsAlreadyInGuild	= this->DBGuildMemberExists(lpMsg->szName);
	int SetMatchingState	= 0;
	int	Result				= 0;
	int NotiState			= 1;

	if (bIsAlreadyInGuild == TRUE) // Fail - user is already in this guild
	{
		Result = -4;
	}

	if (bIsAlreadyInGuild == FALSE)
	{
		// Step 2
		// Check matching state
		this->m_GMatchingDB.ExecQuery("EXEC IGC_GetGuildMatchingWaitState '%s', %d", lpMsg->szName, 0);
		this->m_GMatchingDB.Fetch();

		int State = this->m_GMatchingDB.GetAsInteger("State");
		this->m_GMatchingDB.Close();

		if (State != 0) // Fail - wrong state
		{
			Result = -3;
		}

		else
		{

			if (lpMsg->nAllowType == 1)
			{
				SetMatchingState = 1;
				// Step 3
				// Get target guild
				LPGUILD_INFO_STRUCT lpGuild = this->GetGuild(lpMsg->szGuildName);

				if (!lpGuild) // Fail - no guild in memory
				{
					SetMatchingState = 2;
					NotiState = 2;
					Result = -6;
				}

				else
				{
					// Step 4
					// Check gens type between guild and user

					int OwnInfluence = this->m_GensSystemEDS.GetGensInfluence(lpMsg->szName);
					int TarInfluence = this->m_GensSystemEDS.GetGensInfluence(lpGuild->m_szGuildMaster);

					if (OwnInfluence != TarInfluence) // Fail - gens influence mismatch
					{
						SetMatchingState = 2;
						NotiState = 2;
						Result = -5;
					}
				}
			}

			else
			{
				NotiState = 2;
				SetMatchingState = 2;
			}

			// Step 5
			// Set Matching State for user
			this->m_GMatchingDB.ExecQuery("EXEC IGC_SetGuildMatchingWaitState '%s', %d", lpMsg->szName, SetMatchingState);
			this->m_GMatchingDB.Fetch();

			if (this->m_GMatchingDB.GetAsInteger("result") != 0) // Fail - procedure error
			{
				Result = -2;
			}

			this->m_GMatchingDB.Close();
		}
	}

	// Step 6
	// Send result

	pMsg.nResult = Result;
	pMsg.nAllowType = lpMsg->nAllowType;
	pMsg.nUserIndex = lpMsg->nUserIndex;
	memcpy(pMsg.szGuildName, lpMsg->szGuildName, MAX_GUILD_LEN + 1);
	memcpy(pMsg.szMemberName, lpMsg->szName, MAX_ACCOUNT_LEN + 1);

	PHeadSubSetB((LPBYTE)&pMsg, 0xA3, 0x06, sizeof(pMsg));

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size, __FUNCTION__);

	// Step 7
	// Notify guild member about matching result

	_stAnsNotiGuildMatching pNotiMsg;
	PHeadSubSetB((LPBYTE)&pNotiMsg, 0xA3, 0x09, sizeof(pNotiMsg));

	EnterCriticalSection(&this->m_FriendSystemEDS.m_csMapFriendMaster);

	LPFRIEND_MASTER lpUser = this->m_FriendSystemEDS.GetFriendMaster(pMsg.szMemberName);
	BOOL IsUserFound = FALSE;
	int iUserServerCode = -1;

	if (lpUser)
	{
		pNotiMsg.nUserIndex = lpUser->m_iNumber;
		pNotiMsg.nResult = NotiState;
		IsUserFound = TRUE;
		iUserServerCode = lpUser->m_iServer;
	}

	for (int i = 0; i < g_dwMaxServerGroups; i++)
	{
		if (g_Server[i].m_Index != -1
			&& g_Server[i].m_State == SS_GUILD
			&& g_Server[i].m_Type == ST_EXDATASERVER
			&& g_Server[i].m_ServerCode == iUserServerCode
			&& IsUserFound == TRUE)
		{
			DataSend(i, (LPBYTE)&pNotiMsg, pNotiMsg.h.size, __FUNCTION__);
		}
	}

	LeaveCriticalSection(&this->m_FriendSystemEDS.m_csMapFriendMaster);
}

void CExDataServerProtocol::GDReqWaitGuildMatchingList(int aIndex, _stReqWaitGuildMatchingList * lpMsg)
{
	_stAnsWaitGuildMatchingList pMsg;

	this->m_GMatchingDB.ExecQuery("EXEC IGC_GetWaitGuildMatching %d, %d", lpMsg->nGuildNumber, 0);

	int Count = 0;

	while (this->m_GMatchingDB.Fetch() == SQL_SUCCESS)
	{
		pMsg.stAllowList[Count].btApplicantClass = this->m_GMatchingDB.GetAsInteger("ApplicantClass");
		pMsg.stAllowList[Count].nApplicantLevel = this->m_GMatchingDB.GetAsInteger("ApplicantLevel");
		this->m_GMatchingDB.GetAsString("ApplicantName", pMsg.stAllowList[Count].szApplicantName, sizeof(pMsg.stAllowList[Count].szApplicantName));

		Count++;
	}

	this->m_GMatchingDB.Close();

	if (Count <= 0)
	{
		pMsg.nResult = -3;
	}

	else
	{
		pMsg.nResult = 0;
	}

	pMsg.nListCount = Count;
	pMsg.nUserIndex = lpMsg->nUserIndex;

	PHeadSubSetW((LPBYTE)&pMsg, 0xA3, 0x07, sizeof(pMsg));
	DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg), __FUNCTION__);
}

void CExDataServerProtocol::GDReqGetWaitStateListGuildMatching(int aIndex, _stReqWaitStateListGuildMatching * lpMsg)
{
	_stAnsWaitStateListGuildMatching pMsg;

	this->m_GMatchingDB.ExecQuery("EXEC IGC_GetGuildMatchingWaitStateList '%s'", lpMsg->szName);
	
	if (this->m_GMatchingDB.Fetch() == SQL_NO_DATA)
	{
		pMsg.nResult = -2;
	}

	else
	{
		pMsg.nResult = 0;
		this->m_GMatchingDB.GetAsString("GuildName", pMsg.szGuildName, MAX_GUILD_LEN + 1);
		this->m_GMatchingDB.GetAsString("GuildMasterName", pMsg.szGuildMasterName, MAX_ACCOUNT_LEN + 1);
	}

	this->m_GMatchingDB.Close();

	pMsg.nUserIndex = lpMsg->nUserIndex;
	PHeadSubSetB((LPBYTE)&pMsg, 0xA3, 0x08, sizeof(pMsg));

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size, __FUNCTION__);
}

void CExDataServerProtocol::GuildMatchingSendAcceptAndRejectInfo(int aIndex, char* szUserName, int nUserIndex, int nServerIndex)
{
	_stAnsNotiGuildMatching pNotiMsg;

	if (!szUserName)
	{
		return;
	}

	this->m_GMatchingDB.ExecQuery("EXEC IGC_GetGuildMatchingAcceptNRejectInfo '%s'", szUserName);

	if (this->m_GMatchingDB.Fetch() == SQL_NO_DATA)
	{
		this->m_GMatchingDB.Close();
		return;
	}

	PHeadSubSetB((LPBYTE)&pNotiMsg, 0xA3, 0x09, sizeof(pNotiMsg));

	pNotiMsg.nUserIndex = nUserIndex;
	pNotiMsg.nResult = this->m_GMatchingDB.GetAsInteger("State");

	this->m_GMatchingDB.Close();

	DataSend(nServerIndex, (LPBYTE)&pNotiMsg, sizeof(pNotiMsg), __FUNCTION__);
}

void CExDataServerProtocol::SendNotiGuildMatchingForGuildMaster(int nGuildNumber)
{
	_stAnsNotiGuildMatchingForGuildMaster pMsg;

	LPGUILD_INFO_STRUCT lpGuild = this->GetGuild(nGuildNumber);

	if (!lpGuild)
	{
		return;
	}

	EnterCriticalSection(&this->m_FriendSystemEDS.m_csMapFriendMaster);

	LPFRIEND_MASTER lpUser = this->m_FriendSystemEDS.GetFriendMaster(lpGuild->m_szGuildMaster);

	if (!lpUser)
	{
		LeaveCriticalSection(&this->m_FriendSystemEDS.m_csMapFriendMaster);
		return;
	}

	this->m_GMatchingDB.ExecQuery("EXEC IGC_IsApplicantListGuildMatching %d", nGuildNumber);

	if (this->m_GMatchingDB.Fetch() == SQL_NO_DATA)
	{
		this->m_GMatchingDB.Close();
		LeaveCriticalSection(&this->m_FriendSystemEDS.m_csMapFriendMaster);
		return;
	}

	int nResult = this->m_GMatchingDB.GetAsInteger("result");
	this->m_GMatchingDB.Close();

	if (nResult != 1)
	{
		LeaveCriticalSection(&this->m_FriendSystemEDS.m_csMapFriendMaster);
		return;
	}

	PHeadSubSetB((LPBYTE)&pMsg, 0xA3, 0x10, sizeof(pMsg));

	pMsg.nUserIndex = lpUser->m_iNumber;
	pMsg.nResult = 0;

	int ServerIndex = this->m_FriendSystemEDS.ExDBGetIndexByCode(lpUser->m_iServer);

	if (ServerIndex == -1)
	{
		LeaveCriticalSection(&this->m_FriendSystemEDS.m_csMapFriendMaster);
		return;
	}

	DataSend(ServerIndex, (LPBYTE)&pMsg, pMsg.h.size, __FUNCTION__);
	LeaveCriticalSection(&this->m_FriendSystemEDS.m_csMapFriendMaster);
}

void CExDataServerProtocol::SendUseGuildMatchingGuild(char* szName, int nGuildNumber)
{
	_stAnsUseGuildMatchingGuild pMsg;

	this->m_GMatchingDB.ExecQuery("EXEC IGC_GetGuildMatchingRegCheck %d", nGuildNumber);
	
	if (this->m_GMatchingDB.Fetch() == SQL_NO_DATA)
	{
		this->m_GMatchingDB.Close();
		return;
	}

	int nCount = this->m_GMatchingDB.GetAsInteger("count");
	this->m_GMatchingDB.Close();

	if (nCount <= 0)
	{
		return;
	}

	LPGUILD_MEMBER lpMember = this->GetGuildMember(nGuildNumber, szName);

	if (!lpMember)
	{
		return;
	}

	EnterCriticalSection(&this->m_FriendSystemEDS.m_csMapFriendMaster);

	LPFRIEND_MASTER lpUser = this->m_FriendSystemEDS.GetFriendMaster(szName);

	if (!lpUser)
	{
		LeaveCriticalSection(&this->m_FriendSystemEDS.m_csMapFriendMaster);
		return;
	}

	PHeadSubSetB((LPBYTE)&pMsg, 0xA3, 0x11, sizeof(pMsg));
	pMsg.nUserIndex = lpUser->m_iNumber;
	int ServerIndex = this->m_FriendSystemEDS.ExDBGetIndexByCode(lpUser->m_iServer);
	LeaveCriticalSection(&this->m_FriendSystemEDS.m_csMapFriendMaster);

	if (ServerIndex == -1)
	{
		return;
	}

	DataSend(ServerIndex, (LPBYTE)&pMsg, pMsg.h.size, __FUNCTION__);
}

void CExDataServerProtocol::UpdateGuildMatchingMemberCount(char *szGuildName, int nGuildMemberCnt)
{
	this->m_GMatchingDB.ExecQuery("EXEC IGC_UpdateGuildMatchingMemberCount '%s', %d", szGuildName, nGuildMemberCnt);
	this->m_GMatchingDB.Fetch();
	this->m_GMatchingDB.Close();
}

void CExDataServerProtocol::GDReqRegWantedPartyMember(int aIndex, _stReqRegWantedPartyMember * lpMsg)
{
	_stAnsRegWantedPartyMember pMsg;

	EnterCriticalSection(&this->m_FriendSystemEDS.m_csMapFriendMaster);

	LPFRIEND_MASTER lpLeaderData = this->m_FriendSystemEDS.GetFriendMaster(lpMsg->szPartyLeaderName);

	if (!lpLeaderData)
	{
		LeaveCriticalSection(&this->m_FriendSystemEDS.m_csMapFriendMaster);
		return;
	}

	this->m_PMatchingDB.ExecQuery("EXEC IGC_InsertPartyMatchingList '%s', '%s','%s', %d, %d, %d, %d,%d, %d,%d , %d,%d,%d,%d,%d,%d,%d, %d,%d,%d,%d",
		lpMsg->szPartyLeaderName, lpMsg->szTitle, lpMsg->szPassword, lpMsg->nMinLevel, lpMsg->nMaxLevel, lpMsg->nHuntingGround,
		lpMsg->btWantedClass, lpMsg->btCurrentPartyCnt, lpMsg->btUsePassWord, lpMsg->btApprovalType,
		lpMsg->btWantedClassDetailInfo[0], lpMsg->btWantedClassDetailInfo[1], lpMsg->btWantedClassDetailInfo[2], lpMsg->btWantedClassDetailInfo[3],
		lpMsg->btWantedClassDetailInfo[4], lpMsg->btWantedClassDetailInfo[5], lpMsg->btWantedClassDetailInfo[6], lpLeaderData->m_iServer + 1,
		lpMsg->btGensType, lpMsg->nLeaderLevel, lpMsg->btLeaderClass);

	if (this->m_PMatchingDB.Fetch() != SQL_SUCCESS)
	{
		pMsg.nResult = -1;
	}

	else
	{
		this->m_PartyMatchingEDS.CreateParty(lpLeaderData->m_iServer, aIndex, lpMsg->nUserIndex, lpMsg->szPartyLeaderName);
		pMsg.nResult = 0;
	}

	pMsg.nUserIndex = lpMsg->nUserIndex;
	PHeadSubSetB((LPBYTE)&pMsg, 0xA4, 0x00, sizeof(pMsg));

	this->m_PMatchingDB.Close();
	LeaveCriticalSection(&this->m_FriendSystemEDS.m_csMapFriendMaster);

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size, __FUNCTION__);
}

void CExDataServerProtocol::GDReqGetPartyMatchingList(int aIndex, _stReqGetPartyMatchingList * lpMsg)
{
	_stAnsGetPartyMatchingList pMsg;

	if (lpMsg->nType == 0)
	{
		this->m_PMatchingDB.ExecQuery("EXEC IGC_GetPartyMatchingList %d, %d", lpMsg->nPage, 6);
	}

	else if (lpMsg->nType == 1)
	{
		this->m_PMatchingDB.ExecQuery("EXEC IGC_GetPartyMatchingListKeyword %d, %d, '%s'", lpMsg->nPage, 6, lpMsg->szSearchWord);
	}

	else if (lpMsg->nType == 2)
	{
		this->m_PMatchingDB.ExecQuery("EXEC IGC_GetPartyMatchingListJoinAble %d, %d,%d,%d, %d", lpMsg->nLevel, lpMsg->btClass, lpMsg->btGens, lpMsg->nPage, 6);
	}

	int nCount = 0;

	do
	{
		if (this->m_PMatchingDB.Fetch() == SQL_NO_DATA)
			break;

		this->m_PMatchingDB.GetAsString("PartyLeaderName", pMsg.stPartyInfoList[nCount].szLeaderName, sizeof(pMsg.stPartyInfoList[nCount].szLeaderName));
		this->m_PMatchingDB.GetAsString("Title", pMsg.stPartyInfoList[nCount].szTitle, sizeof(pMsg.stPartyInfoList[nCount].szTitle));
		pMsg.stPartyInfoList[nCount].nMinLevel = this->m_PMatchingDB.GetAsInteger("MinLevel");
		pMsg.stPartyInfoList[nCount].nMaxLevel = this->m_PMatchingDB.GetAsInteger("MaxLevel");
		pMsg.stPartyInfoList[nCount].nHuntingGround = this->m_PMatchingDB.GetAsInteger("HuntingGround");
		pMsg.stPartyInfoList[nCount].btWantedClass = this->m_PMatchingDB.GetAsInteger("WantedClass");
		pMsg.stPartyInfoList[nCount].btCurPartyMemberCnt = this->m_PMatchingDB.GetAsInteger("CurMemberCount");
		pMsg.stPartyInfoList[nCount].btUsePassword = this->m_PMatchingDB.GetAsInteger("UsePassWord");
		pMsg.stPartyInfoList[nCount].btWantedClassDetailInfo[0] = this->m_PMatchingDB.GetAsInteger("WantedClassDetailInfo1");
		pMsg.stPartyInfoList[nCount].btWantedClassDetailInfo[1] = this->m_PMatchingDB.GetAsInteger("WantedClassDetailInfo2");
		pMsg.stPartyInfoList[nCount].btWantedClassDetailInfo[2] = this->m_PMatchingDB.GetAsInteger("WantedClassDetailInfo3");
		pMsg.stPartyInfoList[nCount].btWantedClassDetailInfo[3] = this->m_PMatchingDB.GetAsInteger("WantedClassDetailInfo4");
		pMsg.stPartyInfoList[nCount].btWantedClassDetailInfo[4] = this->m_PMatchingDB.GetAsInteger("WantedClassDetailInfo5");
		pMsg.stPartyInfoList[nCount].btWantedClassDetailInfo[5] = this->m_PMatchingDB.GetAsInteger("WantedClassDetailInfo6");
		pMsg.stPartyInfoList[nCount].btWantedClassDetailInfo[6] = this->m_PMatchingDB.GetAsInteger("WantedClassDetailInfo7");
		pMsg.stPartyInfoList[nCount].btGensType = this->m_PMatchingDB.GetAsInteger("GensType");
		pMsg.stPartyInfoList[nCount].nServerChannel = this->m_PMatchingDB.GetAsInteger("LeaderChannel");
		pMsg.stPartyInfoList[nCount].nLeaderLevel = this->m_PMatchingDB.GetAsInteger("LeaderLevel");
		pMsg.stPartyInfoList[nCount].btLeaderClass = this->m_PMatchingDB.GetAsInteger("LeaderClass");

		nCount++;
	} while (nCount < 6);

	this->m_PMatchingDB.Close();

	int nTotalCount = 0;

	if (lpMsg->nType == 2)
	{
		this->m_PMatchingDB.ExecQuery("EXEC IGC_GetPartyMatchingListJoinAbleTotalCount %d, %d, %d", lpMsg->nLevel, lpMsg->btClass, lpMsg->btGens);
	}

	else
	{
		this->m_PMatchingDB.ExecQuery("EXEC IGC_GetPartyMatchingListCount");
	}

	this->m_PMatchingDB.Fetch();

	int nTotalEntry = this->m_PMatchingDB.GetAsInteger("Count");

	this->m_PMatchingDB.Close();

	PHeadSubSetW((LPBYTE)&pMsg, 0xA4, 0x01, sizeof(pMsg));

	pMsg.nUserIndex = lpMsg->nUserIndex;
	pMsg.nPage = lpMsg->nPage;

	int nTotalPage = 0;

	if (nTotalEntry > 0)
	{
		nTotalPage = nTotalEntry / 6;

		if (nTotalEntry % 6 > 0)
		{
			nTotalPage++;
		}

		if (nTotalEntry < 6)
		{
			nTotalPage = 1;
		}
	}

	pMsg.nTotalPage = nTotalPage;
	pMsg.nListCount = nCount;
	pMsg.nResult = 0;

	DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg), __FUNCTION__);
}

void CExDataServerProtocol::GDReqJoinMemberPartyMatching(int aIndex, _stReqJoinMemberPartyMatching * lpMsg)
{
	_stAnsJoinMemberPartyMatching pMsg;

	this->m_PMatchingDB.ExecQuery("EXEC IGC_GetPartyMatchingWaitList '%s'", lpMsg->szMemberName);

	BOOL bExistOnWaitList = FALSE;

	EnterCriticalSection(&this->m_FriendSystemEDS.m_csMapFriendMaster);

	LPFRIEND_MASTER lpPartyMaster = this->m_FriendSystemEDS.GetFriendMaster(lpMsg->szLeaderName);
	LPFRIEND_MASTER lpPartyMember = this->m_FriendSystemEDS.GetFriendMaster(lpMsg->szMemberName);

	int MasterServerIndex = -1, MemberServerIndex = -1;

	if (lpPartyMaster)
	{
		MasterServerIndex = this->m_FriendSystemEDS.ExDBGetIndexByCode(lpPartyMaster->m_iServer);
	}

	if (lpPartyMember)
	{
		MemberServerIndex = this->m_FriendSystemEDS.ExDBGetIndexByCode(lpPartyMember->m_iServer);
	}

	if (this->m_PMatchingDB.Fetch() != SQL_NO_DATA)
	{
		bExistOnWaitList = TRUE;
	}

	this->m_PMatchingDB.Close();

	if (bExistOnWaitList == TRUE)
	{
		pMsg.nResult = -4;
	}

	else
	{
		if (lpMsg->btRandomParty == TRUE)
		{
			char szLeaderName[MAX_ACCOUNT_LEN + 1];
			memset(szLeaderName, 0x00, sizeof(szLeaderName));

			this->m_PMatchingDB.ExecQuery("EXEC IGC_GetPartyMatchingRandom %d, %d, %d", lpMsg->nLevel, lpMsg->btClass, lpMsg->btGensType);

			if (this->m_PMatchingDB.Fetch() == SQL_NO_DATA)
			{
				this->m_PMatchingDB.Close();
				pMsg.nResult = -3;
			}

			else
			{
				this->m_PMatchingDB.GetAsString("PartyLeaderName", szLeaderName, sizeof(szLeaderName));
				this->m_PMatchingDB.Close();

				lpPartyMaster = this->m_FriendSystemEDS.GetFriendMaster(szLeaderName);

				if (!lpPartyMaster)
				{
					PHeadSubSetB((LPBYTE)&pMsg, 0xA4, 0x02, sizeof(pMsg));
					pMsg.nResult = -2;
					DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size, __FUNCTION__);
					LeaveCriticalSection(&this->m_FriendSystemEDS.m_csMapFriendMaster);

					return;
				}

				MasterServerIndex = this->m_FriendSystemEDS.ExDBGetIndexByCode(lpPartyMaster->m_iServer);

				this->m_PMatchingDB.ExecQuery("EXEC IGC_GetPartyMatchingPassWordInfo '%s'", szLeaderName);
				this->m_PMatchingDB.Fetch();

				int AcceptType = this->m_PMatchingDB.GetAsInteger("AcceptType");
				this->m_PMatchingDB.Close();

				this->m_PMatchingDB.ExecQuery("EXEC IGC_GetPartyMatchingWaitListForLeader '%s'", szLeaderName);

				int nCount = 0;

				while (this->m_PMatchingDB.Fetch() == SQL_SUCCESS)
				{
					nCount++;
				}

				this->m_PMatchingDB.Close();

				if (nCount >= 10)
				{
					pMsg.nResult = -7;
				}

				else
				{
					if (AcceptType == 1)
					{
						this->AutoAddPartyMember(szLeaderName, lpMsg->szMemberName, lpMsg->nUserIndex, aIndex);
						LeaveCriticalSection(&this->m_FriendSystemEDS.m_csMapFriendMaster);
						return;
					}

					this->m_PMatchingDB.ExecQuery("EXEC IGC_InsertWaitPartyMatching '%s', '%s', %d, %d, %d", szLeaderName, lpMsg->szMemberName, lpMsg->btChangeUpClass, lpMsg->nLevel, lpMsg->nUserDBNumber);
					this->m_PMatchingDB.Fetch();
					this->m_PMatchingDB.Close();
					pMsg.nResult = 0;
				}
			}
		}

		else
		{
			char szPassword[5];
			int UsePassword, AcceptType;

			if (!strcmp(lpMsg->szLeaderName, lpMsg->szMemberName))
			{
				pMsg.nResult = -5;
			}

			else
			{
				this->m_PMatchingDB.ExecQuery("EXEC IGC_GetPartyMatchingPassWordInfo '%s'", lpMsg->szLeaderName);

				if (this->m_PMatchingDB.Fetch() == SQL_NO_DATA || lpPartyMaster == NULL)
				{
					this->m_PMatchingDB.Close();
					pMsg.nResult = -2;
				}

				else
				{
					this->m_PMatchingDB.GetAsString("PassWord", szPassword, sizeof(szPassword));
					UsePassword = this->m_PMatchingDB.GetAsInteger("UsePassWord");
					AcceptType = this->m_PMatchingDB.GetAsInteger("AcceptType");

					this->m_PMatchingDB.Close();

					this->m_PMatchingDB.ExecQuery("EXEC IGC_GetPartyMatchingWaitListForLeader '%s'", lpMsg->szLeaderName);

					int nCount = 0;

					while (this->m_PMatchingDB.Fetch() == SQL_SUCCESS)
					{
						nCount++;
					}

					this->m_PMatchingDB.Close();

					if (nCount >= 10)
					{
						pMsg.nResult = -7;
					}

					else
					{
						if (UsePassword == TRUE)
						{
							if (strcmp(szPassword, lpMsg->szPassword))
							{
								pMsg.nResult = -1;
							}

							else
							{
								if (AcceptType == 1)
								{
									this->AutoAddPartyMember(lpMsg->szLeaderName, lpMsg->szMemberName, lpMsg->nUserIndex, aIndex);
									return;
								}

								this->m_PMatchingDB.ExecQuery("EXEC IGC_InsertWaitPartyMatching '%s', '%s', %d, %d, %d", lpMsg->szLeaderName, lpMsg->szMemberName, lpMsg->btChangeUpClass, lpMsg->nLevel, lpMsg->nUserDBNumber);
								this->m_PMatchingDB.Fetch();
								this->m_PMatchingDB.Close();
								pMsg.nResult = 0;
							}
						}

						else
						{
							if (AcceptType == 1)
							{
								this->AutoAddPartyMember(lpMsg->szLeaderName, lpMsg->szMemberName, lpMsg->nUserIndex, aIndex);
								return;
							}

							this->m_PMatchingDB.ExecQuery("EXEC IGC_InsertWaitPartyMatching '%s', '%s', %d, %d, %d", lpMsg->szLeaderName, lpMsg->szMemberName, lpMsg->btChangeUpClass, lpMsg->nLevel, lpMsg->nUserDBNumber);
							this->m_PMatchingDB.Fetch();
							this->m_PMatchingDB.Close();
							pMsg.nResult = 0;
						}
					}
				}
			}
		}
	}

	if (pMsg.nResult == 0 && lpPartyMaster != NULL && lpPartyMaster->m_iServer != -1)
	{
		_stAnsRequestJoinPartyMatchingNoti pNotiMsg;
		PHeadSubSetB((LPBYTE)&pNotiMsg, 0xA4, 0x08, sizeof(pNotiMsg));

		pNotiMsg.nUserIndex = lpPartyMaster->m_iNumber;

		DataSend(MasterServerIndex, (LPBYTE)&pNotiMsg, pNotiMsg.h.size, __FUNCTION__);
	}

	PHeadSubSetB((LPBYTE)&pMsg, 0xA4, 0x02, sizeof(pMsg));

	pMsg.nUserIndex = lpMsg->nUserIndex;

	LeaveCriticalSection(&this->m_FriendSystemEDS.m_csMapFriendMaster);
	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size, __FUNCTION__);
}

void CExDataServerProtocol::GDReqJoinMemberStateList(int aIndex, _stReqJoinMemberStateListPartyMatching *lpMsg)
{
	_stAnsJoinMemberStateListPartyMatching pMsg;

	this->m_PMatchingDB.ExecQuery("EXEC IGC_GetPartyMatchingWaitList '%s'", lpMsg->szMemberName);

	if (this->m_PMatchingDB.Fetch() == SQL_NO_DATA)
	{
		pMsg.nResult = -1;
		pMsg.nUserIndex = lpMsg->nUserIndex;
	}

	else
	{
		pMsg.nResult = 0;
		pMsg.nUserIndex = lpMsg->nUserIndex;

		this->m_PMatchingDB.GetAsString("LeaderName", pMsg.szLeaderName, sizeof(pMsg.szLeaderName));

		EnterCriticalSection(&this->m_FriendSystemEDS.m_csMapFriendMaster);
		LPFRIEND_MASTER lpLeader = this->m_FriendSystemEDS.GetFriendMaster(pMsg.szLeaderName);

		if (lpLeader)
		{
			pMsg.btLeaderChannel = lpLeader->m_iServer;
		}

		else
		{
			pMsg.btLeaderChannel = -1;
		}

		LeaveCriticalSection(&this->m_FriendSystemEDS.m_csMapFriendMaster);
	}

	this->m_PMatchingDB.Close();

	PHeadSubSetB((LPBYTE)&pMsg, 0xA4, 0x03, sizeof(pMsg));
	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size, __FUNCTION__);
}

void CExDataServerProtocol::GDReqJoinMemberStateListLeader(int aIndex, _stReqWaitListPartyMatching *lpMsg)
{
	_stAnsWaitListPartyMatching pMsg;

	this->m_PMatchingDB.ExecQuery("EXEC IGC_GetPartyMatchingWaitListForLeader '%s'", lpMsg->szLeaderName);

	int nCount = 0;

	while (this->m_PMatchingDB.Fetch() == SQL_SUCCESS)
	{
		this->m_PMatchingDB.GetAsString("MemberName", pMsg.stList[nCount].szName, sizeof(pMsg.stList[nCount].szName));
		pMsg.stList[nCount].btClass = this->m_PMatchingDB.GetAsInteger("Class");
		pMsg.stList[nCount].nLevel = this->m_PMatchingDB.GetAsInteger("MemberLevel");

		nCount++;

		if (nCount >= 10)
			break;
	}

	this->m_PMatchingDB.Close();

	pMsg.nListCount = nCount;
	pMsg.nResult = 0;
	pMsg.nUserIndex = lpMsg->nUserIndex;
	PHeadSubSetW((LPBYTE)&pMsg, 0xA4, 0x04, sizeof(pMsg));

	DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg), __FUNCTION__);
}

void CExDataServerProtocol::GDReqAcceptMemberJoin(int aIndex, _stReqAddPartyMember *lpMsg)
{
	_stAnsAddPartyMember pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xA4, 0x05, sizeof(pMsg));

	BOOL bUserFound = FALSE;
	EnterCriticalSection(&this->m_FriendSystemEDS.m_csMapFriendMaster);
	LPFRIEND_MASTER lpMember = this->m_FriendSystemEDS.GetFriendMaster(lpMsg->szMemberName);
	memcpy(pMsg.szMemberName, lpMsg->szMemberName, MAX_ACCOUNT_LEN + 1);

	int nMasterIndex = -1, nMemberIndex = -1, nMemberServerIndex = -1, nMemberServer = -1;

	if (lpMember)
	{
		bUserFound = TRUE;
		nMemberIndex = lpMember->m_iNumber;
		nMemberServerIndex = this->m_FriendSystemEDS.ExDBGetIndexByCode(lpMember->m_iServer);
		nMemberServer = lpMember->m_iServer;
	}

	else if (lpMsg->btAlreadyParty == TRUE)
	{
		bUserFound = TRUE;
		nMemberIndex = lpMsg->nMemberIndex;
		nMemberServerIndex = aIndex;
		nMemberServer = g_Server[aIndex].m_ServerCode;
	}

	LeaveCriticalSection(&this->m_FriendSystemEDS.m_csMapFriendMaster);

	pMsg.nUserIndex = lpMsg->nUserIndex;
	pMsg.nMemberIndex = nMemberIndex;
	pMsg.btManualJoin = lpMsg->btManualJoin;

	if (lpMsg->btAlreadyParty == TRUE)
	{
		int iResult = this->m_PartyMatchingEDS.AddPartyMember(lpMsg->szLeaderName, lpMsg->szMemberName, nMemberIndex, nMemberServer, nMemberServerIndex);

		if (iResult == 0)
		{
			PARTYMATCHING_PARTYDATA * lpPartyData = this->m_PartyMatchingEDS.GetPartyDataByMaster(lpMsg->szLeaderName);

			if (lpPartyData)
			{
				this->m_PMatchingDB.ExecQuery("EXEC IGC_UpdatePartyMatchingMemberCount '%s', %d", lpMsg->szLeaderName, lpPartyData->m_UserCount);
				this->m_PMatchingDB.Fetch();
				this->m_PMatchingDB.Close();
			}
		}
	}

	else
	{
		if (bUserFound == FALSE)
		{
			pMsg.nResult = -1;
			pMsg.btSendType = 0;
			DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size, __FUNCTION__);
			return;
		}

		if (lpMsg->btType == 1)
		{
			pMsg.nResult = this->m_PartyMatchingEDS.AddPartyMember(lpMsg->szLeaderName, lpMsg->szMemberName, nMemberIndex, nMemberServer, nMemberServerIndex);
			pMsg.btSendType = 0;
			pMsg.btType = lpMsg->btType;

			if (pMsg.nResult == 0)
			{
				PARTYMATCHING_PARTYDATA * lpPartyData = this->m_PartyMatchingEDS.GetPartyDataByMaster(lpMsg->szLeaderName);

				if (lpPartyData)
				{
					this->m_PMatchingDB.ExecQuery("EXEC IGC_UpdatePartyMatchingMemberCount '%s', %d", lpMsg->szLeaderName, lpPartyData->m_UserCount);
					this->m_PMatchingDB.Fetch();
					this->m_PMatchingDB.Close();
				}
			}

			DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size, __FUNCTION__);

			pMsg.btSendType = 1;
			DataSend(nMemberServerIndex, (LPBYTE)&pMsg, pMsg.h.size, __FUNCTION__);

			if (nMemberServerIndex == aIndex)
			{
				this->FixAddPartyMember(lpMsg->nUserIndex, nMemberIndex, aIndex);
			}

			this->SendPartyMatchingMemberList(lpMsg->szLeaderName, 0);
		}

		else
		{
			pMsg.nUserIndex = lpMsg->nUserIndex;
			pMsg.nMemberIndex = nMemberIndex;
			pMsg.btType = lpMsg->btAlreadyParty;
			pMsg.btSendType = 0;
			pMsg.nResult = 0;

			DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size, __FUNCTION__);

			pMsg.btSendType = 1;
			DataSend(nMemberServerIndex, (LPBYTE)&pMsg, pMsg.h.size, __FUNCTION__);
		}

		this->m_PMatchingDB.ExecQuery("EXEC IGC_DeleteWaitPartyMatchingList '%s'", lpMsg->szMemberName);
		this->m_PMatchingDB.Fetch();
		this->m_PMatchingDB.Close();
	}
}

void CExDataServerProtocol::GDReqCancelPartyMatching(int aIndex, _stReqCancelPartyMatching *lpMsg)
{
	_stAnsCancelPartyMatching pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xA4, 0x06, sizeof(pMsg));

	pMsg.btType = lpMsg->btType;
	pMsg.nResult = 0;

	if (lpMsg->btType)
	{
		this->m_PMatchingDB.ExecQuery("EXEC IGC_DeleteWaitPartyMatchingList '%s'", lpMsg->szName);
		this->m_PMatchingDB.Fetch();
		this->m_PMatchingDB.Close();

		PARTYMATCHING_PARTYDATA * lpPartyData = this->m_PartyMatchingEDS.GetPartyDataByMember(lpMsg->szName);

		if (lpPartyData)
		{
			this->m_PartyMatchingEDS.DelPartyMember(lpPartyData->szLeaderName, lpMsg->szName);

			this->m_PMatchingDB.ExecQuery("EXEC IGC_UpdatePartyMatchingMemberCount '%s', %d", lpPartyData->szLeaderName, lpPartyData->m_UserCount);
			this->m_PMatchingDB.Fetch();
			this->m_PMatchingDB.Close();

			this->SendPartyMatchingMemberList(lpPartyData->szLeaderName, 0);
		}
	}

	else
	{
		this->m_PMatchingDB.ExecQuery("EXEC IGC_DeletePartyMatchingList '%s'", lpMsg->szName);
		this->m_PMatchingDB.Fetch();
		this->m_PMatchingDB.Close();

		PARTYMATCHING_PARTYDATA * lpPartyData = this->m_PartyMatchingEDS.GetPartyDataByMaster(lpMsg->szName);

		if (lpPartyData)
		{
			if (strcmp(lpPartyData->szLeaderName, lpMsg->szName))
			{
				this->m_PartyMatchingEDS.DelPartyMember(lpMsg->szName, lpPartyData->szLeaderName);
		
				this->m_PMatchingDB.ExecQuery("EXEC IGC_UpdatePartyMatchingMemberCount '%s', %d", lpPartyData->szLeaderName, lpPartyData->m_UserCount);
				this->m_PMatchingDB.Fetch();
				this->m_PMatchingDB.Close();
			}

			else
			{
				if (lpPartyData->m_UserCount > 1)
				{
					for (int i = 1; i < 5; i++)
					{
						if (lpPartyData->m_UserData[i].iUsedInfo == TRUE && lpPartyData->m_UserData[i].iUserConnected == TRUE)
						{
							pMsg.nUserIndex = lpPartyData->m_UserData[i].iUserIndex;
							DataSend(lpPartyData->m_UserData[i].iServerIndex, (LPBYTE)&pMsg, pMsg.h.size, __FUNCTION__);
						}
					}
				}

				this->m_PartyMatchingEDS.DeleteParty(lpMsg->szName);

				this->m_PMatchingDB.ExecQuery("EXEC IGC_GetPartyMatchingWaitListForLeader '%s'", lpPartyData->szLeaderName);

				int nCount = 0;

				_PARTY_MEMBER_WAIT_LIST stList[80];

				while (this->m_PMatchingDB.Fetch() == SQL_SUCCESS)
				{
					this->m_PMatchingDB.GetAsString("MemberName", stList[nCount].szName, sizeof(stList[nCount].szName));
					stList[nCount].btClass = this->m_PMatchingDB.GetAsInteger("Class");
					stList[nCount].nLevel = this->m_PMatchingDB.GetAsInteger("MemberLevel");

					nCount++;
				}

				this->m_PMatchingDB.Close();

				if (nCount > 0)
				{
					EnterCriticalSection(&this->m_FriendSystemEDS.m_csMapFriendMaster);

					for (int i = 0; i < nCount; i++)
					{
						LPFRIEND_MASTER lpUser = this->m_FriendSystemEDS.GetFriendMaster(stList[i].szName);

						if (lpUser)
						{
							pMsg.btType = lpMsg->btType;
							pMsg.nUserIndex = lpUser->m_iNumber;
							pMsg.nResult = 1;

							int ServerIndex = this->m_FriendSystemEDS.ExDBGetIndexByCode(lpUser->m_iServer);

							DataSend(ServerIndex, (LPBYTE)&pMsg, pMsg.h.size, __FUNCTION__);
						}
					}

					LeaveCriticalSection(&this->m_FriendSystemEDS.m_csMapFriendMaster);
				}
			}
		}

		else
		{
			pMsg.nResult = -1;
		}
	}

	pMsg.btType = lpMsg->btType;
	pMsg.nUserIndex = lpMsg->nUserIndex;

	DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size, __FUNCTION__);
}

void CExDataServerProtocol::GDReqDeletePartyUser(int aIndex, _stReqDelPartyUserPartyMatching *lpMsg)
{
	_stAnsDelPartyUserPartyMatching pMsg;

	PARTYMATCHING_PARTYDATA * lpPartyData = this->m_PartyMatchingEDS.GetPartyDataByMember(lpMsg->szTargetName);
	PARTYMATCHING_PARTYUSERDATA * lpMemberData = this->m_PartyMatchingEDS.GetPartyMemberData(lpMsg->szTargetName);

	if (!lpPartyData || !lpMemberData)
	{
		return;
	}

	char szLeaderName[MAX_ACCOUNT_LEN + 1];
	memcpy(szLeaderName, lpPartyData->m_UserData[0].szUserName, MAX_ACCOUNT_LEN + 1);

	BOOL bResult = 0;

	int nLeaderServerIndex = lpPartyData->m_UserData[0].iServerIndex;
	int nLeaderIndex = lpPartyData->m_UserData[0].iUserIndex;
	int nUserServerIndex = -1;
	int nUserIndex = -1;

	for (int i = 0; i < 5; i++)
	{
		if (!strcmp(lpPartyData->m_UserData[i].szUserName, lpMsg->szTargetName))
		{
			nUserIndex = lpPartyData->m_UserData[i].iUserIndex;
			nUserServerIndex = lpPartyData->m_UserData[i].iServerIndex;
		}
	}

	if (nUserIndex < 0)
	{
		bResult = -1;
	}

	else
	{
		if (strcmp(szLeaderName, lpMsg->szTargetName))
		{
			this->SendPartyMatchingOneList(lpMsg->szTargetName);
			this->m_PartyMatchingEDS.DelPartyMember(szLeaderName, lpMsg->szTargetName);

			this->m_PMatchingDB.ExecQuery("EXEC IGC_UpdatePartyMatchingMemberCount '%s', %d", szLeaderName, lpPartyData->m_UserCount);
			this->m_PMatchingDB.Fetch();
			this->m_PMatchingDB.Close();

			this->SendPartyMatchingMemberList(szLeaderName, 0);
		}

		else
		{
			this->m_PMatchingDB.ExecQuery("EXEC IGC_DeletePartyMatchingList '%s'", szLeaderName);
			this->m_PMatchingDB.Fetch();
			this->m_PMatchingDB.Close();

			this->SendPartyMatchingMemberList(szLeaderName, 1);
			this->m_PartyMatchingEDS.DeleteParty(szLeaderName);

			_stAnsCancelPartyMatching pDeleteMsg;
			PHeadSubSetB((LPBYTE)&pDeleteMsg, 0xA4, 0x06, sizeof(pDeleteMsg));

			pDeleteMsg.nUserIndex = nLeaderIndex;
			pDeleteMsg.btType = 0;
			pDeleteMsg.nResult = 0;

			DataSend(nLeaderServerIndex, (LPBYTE)&pDeleteMsg, pDeleteMsg.h.size, __FUNCTION__);

			this->m_PMatchingDB.ExecQuery("EXEC IGC_GetPartyMatchingWaitListForLeader '%s'", lpPartyData->szLeaderName);

			int nCount = 0;

			_PARTY_MEMBER_WAIT_LIST stList[80];

			while (this->m_PMatchingDB.Fetch() == SQL_SUCCESS)
			{
				this->m_PMatchingDB.GetAsString("MemberName", stList[nCount].szName, sizeof(stList[nCount].szName));
				stList[nCount].btClass = this->m_PMatchingDB.GetAsInteger("Class");
				stList[nCount].nLevel = this->m_PMatchingDB.GetAsInteger("MemberLevel");

				nCount++;
			}

			this->m_PMatchingDB.Close();

			if (nCount > 0)
			{
				for (int i = 0; i < nCount; i++)
				{
					this->m_PMatchingDB.ExecQuery("EXEC IGC_DeleteWaitPartyMatchingList '%s'", stList[i].szName);
					this->m_PMatchingDB.Fetch();
					this->m_PMatchingDB.Close();
				}
			}
		}
	}

	PHeadSubSetB((LPBYTE)&pMsg, 0xA4, 0x07, sizeof(pMsg));
	pMsg.nUserIndex = lpMsg->nUserIndex;
	pMsg.nTargetIndex = nUserIndex;
	pMsg.nResult = 0;
	pMsg.btType = 0;

	if (pMsg.nUserIndex == pMsg.nTargetIndex)
	{
		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size, __FUNCTION__);
	}

	else
	{
		pMsg.btType = 0;
		DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size, __FUNCTION__);

		if (nUserServerIndex < 0)
		{
			g_Log.Add("[Party Matching] User not connected (%s)", lpMsg->szTargetName);
			return;
		}

		pMsg.btType = 1;
		DataSend(nUserServerIndex, (LPBYTE)&pMsg, pMsg.h.size, __FUNCTION__);
	}
}

void CExDataServerProtocol::GDReqSendPartyMemberList(int aIndex, _stReqSendPartyMemberList * lpMsg)
{
	this->SendPartyMatchingMemberList(lpMsg->szLeaderName, 0);
}

void CExDataServerProtocol::GDSendChatMsgPartyMatching(int aIndex, _stReqChattingPartyMatching * lpMsg)
{
	_stAnsChattingPartyMatching pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xA4, 0x12, sizeof(pMsg));

	PARTYMATCHING_PARTYDATA * lpPartyData = this->m_PartyMatchingEDS.GetPartyDataByNumber(lpMsg->nPartyIndex);

	if (!lpPartyData)
	{
		return;
	}

	pMsg.nPartyIndex = lpMsg->nPartyIndex;
	memset(pMsg.szChat, 0x00, sizeof(pMsg.szChat));
	memcpy(pMsg.szSendCharName, lpMsg->szSendCharName, MAX_ACCOUNT_LEN + 1);
	memcpy(pMsg.szChat, lpMsg->szChat, 63);

	for (int i = 0; i < 5; i++)
	{
		if (lpPartyData->m_UserData[i].iUsedInfo == TRUE && lpPartyData->m_UserData[i].iUserConnected == TRUE)
		{
			pMsg.nRecvUserIndex = lpPartyData->m_UserData[i].iUserIndex;
			memcpy(pMsg.szSendCharName, lpMsg->szSendCharName, MAX_ACCOUNT_LEN + 1);
			memcpy(pMsg.szChat, lpMsg->szChat, 63);

			DataSend(lpPartyData->m_UserData[i].iServerIndex, (LPBYTE)&pMsg, pMsg.h.size, __FUNCTION__);
		}
	}
}

void CExDataServerProtocol::AutoAddPartyMember(char *szLeaderName, char *szMemberName, int nUserIndex, int nServerIndex)
{
	EnterCriticalSection(&this->m_FriendSystemEDS.m_csMapFriendMaster);

	LPFRIEND_MASTER lpMaster = this->m_FriendSystemEDS.GetFriendMaster(szLeaderName);
	LPFRIEND_MASTER lpUser = this->m_FriendSystemEDS.GetFriendMaster(szMemberName);

	if (!lpMaster || !lpUser)
	{
		LeaveCriticalSection(&this->m_FriendSystemEDS.m_csMapFriendMaster);
		return;
	}

	int nMasterServerIndex = this->m_FriendSystemEDS.ExDBGetIndexByCode(lpMaster->m_iServer);

	_stAnsAddPartyMember pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xA4, 0x05, sizeof(pMsg));

	PARTYMATCHING_PARTYDATA * lpPartyData = this->m_PartyMatchingEDS.GetPartyDataByMaster(szLeaderName);

	if (lpPartyData == NULL)
	{
		pMsg.nResult = -3;
		pMsg.btSendType = 1;

		DataSend(nServerIndex, (LPBYTE)&pMsg, pMsg.h.size, __FUNCTION__);

		g_Log.Add("[Party Matching] Add Error - party with leader (%s) not exists", szLeaderName);
		LeaveCriticalSection(&this->m_FriendSystemEDS.m_csMapFriendMaster);
		return;
	}

	pMsg.nResult = this->m_PartyMatchingEDS.AddPartyMember(szLeaderName, szMemberName, nUserIndex, lpUser->m_iServer, nServerIndex);
	pMsg.nUserIndex = lpMaster->m_iNumber;
	pMsg.nMemberIndex = lpUser->m_iNumber;
	pMsg.btType = 2;
	pMsg.btSendType = 0;
	pMsg.btManualJoin = 0;

	if (pMsg.nResult == 0)
	{
		lpPartyData = this->m_PartyMatchingEDS.GetPartyDataByMaster(szLeaderName);

		if (lpPartyData)
		{
			this->m_PMatchingDB.ExecQuery("EXEC IGC_UpdatePartyMatchingMemberCount '%s', %d", szLeaderName, lpPartyData->m_UserCount);
			this->m_PMatchingDB.Fetch();
			this->m_PMatchingDB.Close();
		}
	}

	DataSend(nMasterServerIndex, (LPBYTE)&pMsg, pMsg.h.size, __FUNCTION__);

	pMsg.btSendType = 1;
	DataSend(nServerIndex, (LPBYTE)&pMsg, pMsg.h.size, __FUNCTION__);

	if (nMasterServerIndex == nServerIndex)
	{
		this->FixAddPartyMember(lpMaster->m_iNumber, lpUser->m_iNumber, nServerIndex);
	}

	LeaveCriticalSection(&this->m_FriendSystemEDS.m_csMapFriendMaster);
	this->SendPartyMatchingMemberList(szLeaderName, 0);
}

void CExDataServerProtocol::FixAddPartyMember(int nLeaderIndex, int nMemberIndex, int nServerIndex)
{
	_stAnsAddRealPartyMember pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xA4, 0x10, sizeof(pMsg));

	pMsg.nLeaderUserIndex = nLeaderIndex;
	pMsg.nMemberUserIndex = nMemberIndex;

	DataSend(nServerIndex, (LPBYTE)&pMsg, pMsg.h.size, __FUNCTION__);
}

void CExDataServerProtocol::SendPartyMatchingMemberList(char* szLeaderName, BYTE btType)
{
	PARTYMATCHING_PARTYDATA * lpPartyData = this->m_PartyMatchingEDS.GetPartyDataByMaster(szLeaderName);

	if (!lpPartyData)
	{
		return;
	}

	if (lpPartyData->m_UserCount <= 1)
	{
		btType = 1;
	}

	for (int i = 0; i < 5; i++)
	{
		if (lpPartyData->m_UserData[i].iUserConnected == TRUE && lpPartyData->m_UserData[i].iUsedInfo == TRUE)
		{
			_stAnsSendPartyMemberList pMsg;
			PHeadSubSetB((LPBYTE)&pMsg, 0xA4, 0x11, sizeof(pMsg));

			pMsg.nMemberCount = lpPartyData->m_UserCount;
			pMsg.nUserIndex = lpPartyData->m_UserData[i].iUserIndex;
			pMsg.nPartyMatchingIndex = lpPartyData->m_PartyMatchingIndex;

			if (btType)
			{
				pMsg.nMemberCount = 0;
			}

			else
			{
				for (int j = 0; j < 5; j++)
				{
					if (lpPartyData->m_UserData[j].iUsedInfo == TRUE)
					{
						memcpy(pMsg.stList[j].Name, lpPartyData->m_UserData[j].szUserName, MAX_ACCOUNT_LEN + 1);
						pMsg.stList[j].bUse = lpPartyData->m_UserData[j].iUsedInfo;
						pMsg.stList[j].nServerChannel = lpPartyData->m_UserData[j].iServerNumber + 1;
						pMsg.stList[j].nUserIndex = lpPartyData->m_UserData[j].iUserIndex;
						pMsg.stList[j].bFlag = lpPartyData->m_UserData[j].iServerIndex == lpPartyData->m_UserData[i].iServerIndex; // j == i
					}
				}
			}

			DataSend(lpPartyData->m_UserData[i].iServerIndex, (LPBYTE)&pMsg, pMsg.h.size, __FUNCTION__);
		}
	}
}

void CExDataServerProtocol::SendPartyMatchingOneList(char *szMemberName)
{
	PARTYMATCHING_PARTYDATA * lpPartyData = this->m_PartyMatchingEDS.GetPartyDataByMember(szMemberName);

	if (!lpPartyData)
	{
		return;
	}

	for (int i = 0; i < 5; i++)
	{
		if (lpPartyData->m_UserData[i].iUsedInfo == TRUE && lpPartyData->m_UserData[i].iUserConnected == TRUE)
		{
			if (!strcmp(szMemberName, lpPartyData->m_UserData[i].szUserName))
			{
				_stAnsSendPartyMemberList pMsg;
				PHeadSubSetB((LPBYTE)&pMsg, 0xA4, 0x11, sizeof(pMsg));

				pMsg.nMemberCount = lpPartyData->m_UserCount;
				pMsg.nPartyMatchingIndex = lpPartyData->m_PartyMatchingIndex;
				pMsg.nUserIndex = lpPartyData->m_UserData[i].iUserIndex;
				pMsg.nMemberCount = 0;

				DataSend(lpPartyData->m_UserData[i].iServerIndex, (LPBYTE)&pMsg, pMsg.h.size, __FUNCTION__);
				return;
			}
		}
	}
}

CPartyMatchingData::CPartyMatchingData()
{
	this->m_vtPartyData.clear();
	InitializeCriticalSection(&this->m_criti);
}

CPartyMatchingData::~CPartyMatchingData()
{
	DeleteCriticalSection(&this->m_criti);
}

void CPartyMatchingData::Init()
{
	if (this->m_DBQuery.Connect(g_MuOnlineDNS, g_UserID, g_Password, g_ServerName) == FALSE)
	{
		g_Log.AddC(TColor::Red, "[ERROR] - EXDATASERVER CANNOT CONNECT TO MSSQL (PARTY MATCHING (2))");
	}

	this->m_DBQuery.ExecQuery("EXEC IGC_DeleteAllPartyMatchingList");
	this->m_DBQuery.Fetch();
	this->m_DBQuery.Close();
}

PARTYMATCHING_PARTYDATA * CPartyMatchingData::GetPartyDataByMaster(char *szLeaderName)
{
	for (std::vector<PARTYMATCHING_PARTYDATA>::iterator It = this->m_vtPartyData.begin(); It != this->m_vtPartyData.end(); It++)
	{
		if (strcmp(It->szLeaderName, szLeaderName) == 0)
		{
			return &*It;
		}
	}

	return NULL;
}

PARTYMATCHING_PARTYDATA * CPartyMatchingData::GetPartyDataByMember(char *szMemberName)
{
	for (std::vector<PARTYMATCHING_PARTYDATA>::iterator It = this->m_vtPartyData.begin(); It != this->m_vtPartyData.end(); It++)
	{
		for (int i = 0; i < 5;i++)
		{
			if (!strcmp(It->m_UserData[i].szUserName, szMemberName))
			{
				return &*It;
			}
		}
	}

	return NULL;
}

PARTYMATCHING_PARTYDATA * CPartyMatchingData::GetPartyDataByNumber(int nPartyMatchingIndex)
{
	for (std::vector<PARTYMATCHING_PARTYDATA>::iterator It = this->m_vtPartyData.begin(); It != this->m_vtPartyData.end(); It++)
	{
		if (It->m_PartyMatchingIndex == nPartyMatchingIndex)
		{
			return &*It;
		}
	}

	return NULL;
}

PARTYMATCHING_PARTYUSERDATA * CPartyMatchingData::GetPartyMemberData(char *szMemberName)
{
	for (std::vector<PARTYMATCHING_PARTYDATA>::iterator It = this->m_vtPartyData.begin(); It != this->m_vtPartyData.end(); It++)
	{
		for (int i = 0; i < 5; i++)
		{
			if (!strcmp(It->m_UserData[i].szUserName, szMemberName))
			{
				return &It->m_UserData[i];
			}
		}
	}

	return NULL;
}

void CPartyMatchingData::CreateParty(int Server, int ServerIndex, int nLeaderIndex, char *szLeaderName)
{
	EnterCriticalSection(&this->m_criti);

	if (this->GetPartyDataByMaster(szLeaderName) != NULL)
	{
		g_Log.AddC(TColor::Red, "[Party Matching] ERROR - Party with Leader %s already exists", szLeaderName);
		return;
	}

	PARTYMATCHING_PARTYDATA m_PartyData;

	m_PartyData.m_PartyMatchingIndex = (int)this->m_vtPartyData.size();
	m_PartyData.m_UserCount = 1;
	memcpy(m_PartyData.szLeaderName, szLeaderName, MAX_ACCOUNT_LEN + 1);
	m_PartyData.m_UserData[0].iUsedInfo = TRUE;
	m_PartyData.m_UserData[0].iUserConnected = TRUE;
	m_PartyData.m_UserData[0].iServerIndex = ServerIndex;
	m_PartyData.m_UserData[0].iServerNumber = Server;
	m_PartyData.m_UserData[0].iUserIndex = nLeaderIndex;
	memcpy(m_PartyData.m_UserData[0].szUserName, szLeaderName, MAX_ACCOUNT_LEN + 1);

	this->m_vtPartyData.push_back(m_PartyData);

	g_Log.Add("[Party Matching] Create Party (Leader: %s) (Total Party Count:%d)", szLeaderName, this->m_vtPartyData.size());

	LeaveCriticalSection(&this->m_criti);
}

void CPartyMatchingData::DeleteParty(char *szLeaderName)
{
	EnterCriticalSection(&this->m_criti);
	PARTYMATCHING_PARTYDATA * lpPartyData = this->GetPartyDataByMaster(szLeaderName);

	if (!lpPartyData)
	{
		return;
	}

	for (std::vector<PARTYMATCHING_PARTYDATA>::iterator it = this->m_vtPartyData.begin(); it != this->m_vtPartyData.end(); it++)
	{
		if (strcmp(szLeaderName, it->szLeaderName) == 0)
		{
			this->m_vtPartyData.erase(it);

			g_Log.Add("[Party Matching] Delete Party (Leader: %s) (Total Party Count:%d)", szLeaderName, this->m_vtPartyData.size());
			break;
		}
	}

	LeaveCriticalSection(&this->m_criti);
}

int CPartyMatchingData::AddPartyMember(char *szLeaderName, char *szMemberName, int nMemberIndex, int nMemberServer, int nMemberServerIndex)
{
	PARTYMATCHING_PARTYDATA * lpPartyData = NULL;
	BOOL bPartyFound = FALSE;
	int bResult = 0;

	EnterCriticalSection(&this->m_criti);
	
	lpPartyData = this->GetPartyDataByMaster(szLeaderName);

	if (lpPartyData != NULL)
	{
		bPartyFound = TRUE;
	}

	if (bPartyFound == FALSE)
	{
		bResult = -1;
	}

	else
	{
		for (int i = 0; i < 5; i++)
		{
			if (strcmp(szMemberName, lpPartyData->m_UserData[i].szUserName) == 0)
			{
				bResult = -4;
				break;
			}
		}

		if (bResult != -4)
		{
			bResult = -2;

			for (int i = 0; i < 5; i++)
			{
				if (lpPartyData->m_UserData[i].iUsedInfo == FALSE)
				{
					lpPartyData->m_UserCount++;
					lpPartyData->m_UserData[i].iUsedInfo = TRUE;
					lpPartyData->m_UserData[i].iUserConnected = TRUE;
					lpPartyData->m_UserData[i].iUserIndex = nMemberIndex;
					lpPartyData->m_UserData[i].iServerNumber = nMemberServer;
					lpPartyData->m_UserData[i].iServerIndex = nMemberServerIndex;
					memcpy(lpPartyData->m_UserData[i].szUserName, szMemberName, MAX_ACCOUNT_LEN + 1);

					g_Log.Add("[Party Matching] Add Party Member (Leader: %s) (Member: %s) (Total users in party: %d)", szLeaderName, szMemberName, lpPartyData->m_UserCount);
					bResult = 0;
					break;
				}
			}
		}
	}

	LeaveCriticalSection(&this->m_criti);

	return bResult;
}

int CPartyMatchingData::DelPartyMember(char *szLeaderName, char *szMemberName)
{
	EnterCriticalSection(&this->m_criti);

	PARTYMATCHING_PARTYDATA * lpPartyData = this->GetPartyDataByMaster(szLeaderName);

	if (lpPartyData == NULL)
	{
		LeaveCriticalSection(&this->m_criti);
		return -1;
	}

	for (int i = 0; i < 5; i++)
	{
		if (lpPartyData->m_UserData[i].iUsedInfo == TRUE)
		{
			if (!strcmp(szMemberName, lpPartyData->m_UserData[i].szUserName))
			{
				lpPartyData->m_UserData[i].iUsedInfo = FALSE;
				lpPartyData->m_UserData[i].iUserConnected = FALSE;
				lpPartyData->m_UserData[i].iServerIndex = -1;
				lpPartyData->m_UserData[i].iServerNumber = -1;
				lpPartyData->m_UserData[i].iUserIndex = -1;
				memset(lpPartyData->m_UserData[i].szUserName, 0x00, sizeof(lpPartyData->m_UserData[i].szUserName));
				lpPartyData->m_UserCount--;

				g_Log.Add("[Party Matching] Delete Party Member (Leader: %s) (Member: %s) (Total users in party: %d)", szLeaderName, szMemberName, lpPartyData->m_UserCount);

				LeaveCriticalSection(&this->m_criti);
				return 0;
			}
		}
	}

	LeaveCriticalSection(&this->m_criti);
	return -2;
}

int CPartyMatchingData::ConnectMember(char *szMemberName, int nMemberIndex, int nChannelNumber, int nChannelIndex)
{
	PARTYMATCHING_PARTYDATA * lpPartyData = this->GetPartyDataByMember(szMemberName);

	if (!lpPartyData)
	{
		return -1;
	}

	EnterCriticalSection(&this->m_criti);

	int iArrayNum;

	for (iArrayNum = 0;; iArrayNum++)
	{
		if (iArrayNum >= 5)
		{
			LeaveCriticalSection(&this->m_criti);
			return 0;
		}

		if (!strcmp(lpPartyData->m_UserData[iArrayNum].szUserName, szMemberName))
			break;
	}

	lpPartyData->m_UserData[iArrayNum].iUserIndex = nMemberIndex;
	lpPartyData->m_UserData[iArrayNum].iServerNumber = nChannelNumber;
	lpPartyData->m_UserData[iArrayNum].iServerIndex = nChannelIndex;
	lpPartyData->m_UserData[iArrayNum].iUserConnected = TRUE;

	g_Log.Add("[Party Matching] Member connected (Name:%s) (Leader:%s)", szMemberName, lpPartyData->szLeaderName);
	LeaveCriticalSection(&this->m_criti);

	return 1;
}

int CPartyMatchingData::DisconnectMember(char *szMemberName)
{
	PARTYMATCHING_PARTYDATA * lpPartyData = this->GetPartyDataByMember(szMemberName);

	if (!lpPartyData)
	{
		return -1;
	}

	EnterCriticalSection(&this->m_criti);

	BOOL bMemberFound = FALSE;
	BOOL bPartyDelete = TRUE;

	for (int i = 0; i < 5; i++)
	{
		if (!strcmp(szMemberName, lpPartyData->m_UserData[i].szUserName))
		{
			lpPartyData->m_UserData[i].iServerIndex = -1;
			lpPartyData->m_UserData[i].iServerNumber = -1;
			lpPartyData->m_UserData[i].iUserConnected = FALSE;

			g_Log.Add("[Party Matching] Member disconnected (Name:%s) (Leader:%s)", szMemberName, lpPartyData->szLeaderName);
			bMemberFound = TRUE;
		}

		if (i >= 0 && lpPartyData->m_UserData[i].iUsedInfo == TRUE)
		{
			if (lpPartyData->m_UserData[i].iUserConnected == TRUE)
			{
				bPartyDelete = FALSE;
			}
		}
	}

	if (lpPartyData->m_UserCount <= 1)
		bPartyDelete = TRUE;

	if (bPartyDelete == TRUE)
	{
		this->m_DBQuery.ExecQuery("EXEC IGC_DeletePartyMatchingList '%s'", lpPartyData->szLeaderName);
		this->m_DBQuery.Fetch();
		this->m_DBQuery.Close();

		this->m_DBQuery.ExecQuery("EXEC IGC_GetPartyMatchingWaitListForLeader '%s'", lpPartyData->szLeaderName);

		int nCount = 0;

		_PARTY_MEMBER_WAIT_LIST stList[80];

		while (this->m_DBQuery.Fetch() == SQL_SUCCESS)
		{
			this->m_DBQuery.GetAsString("MemberName", stList[nCount].szName, sizeof(stList[nCount].szName));
			stList[nCount].btClass = this->m_DBQuery.GetAsInteger("Class");
			stList[nCount].nLevel = this->m_DBQuery.GetAsInteger("MemberLevel");

			nCount++;
		}

		this->m_DBQuery.Close();

		this->DeleteParty(lpPartyData->szLeaderName);

		for (int i = 0; i < nCount; i++)
		{
			this->m_DBQuery.ExecQuery("EXEC IGC_DeleteWaitPartyMatchingList '%s'", stList[nCount].szName);
			this->m_DBQuery.Fetch();
			this->m_DBQuery.Close();
		}
	}

	LeaveCriticalSection(&this->m_criti);
	return bMemberFound;
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

