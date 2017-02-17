//////////////////////////////////////////////////////////////////////
// SProtocol.cpp
// ------------------------------
// Decompiled by Deathway
// Date : 2007-03-09
// ------------------------------
// GS-N 0.99.60T 0x00424EB0
//	GS-N	1.00.18	JPN	0x0042DC70	-	Completed

#include "stdafx.h"
#include "SProtocol.h"
#include "wsJoinServerCli.h"
#include "GameServer.h"
#include "GameMain.h"
#include "protocol.h"
#include "TLog.h"
#include "TNotice.h"
#include "winutil.h"
#include "Shop.h"
#include "MapServerManager.h"
#include "DSProtocol.h"
#include "EDSProtocol.h"
#include "resource.h"
#include "configread.h"
#include "VipSys.h"

void SProtocolCore(BYTE protoNum, LPBYTE aRecv, int aLen)
{

	#if(TRACE_PROTOCOL==1)
		LogAddHeadHex("JOIN_SERVER", aRecv, aLen);
	#endif

	switch ( protoNum )
	{
		case 0x00:
			JGServerLoginResult((SDHP_RESULT *)aRecv);
			break;

		case 0x01:
			JGPAccountRequest((SDHP_IDPASSRESULT *)aRecv);
			break;

		case 0x06:
			GJPBillCeckRecv((SDHP_BILLSEARCH_RESULT *)aRecv);
			break;

		case 0x07:
			GJPUserKillRecv((SDHP_BILLKILLUSER *)aRecv);
			break;

		case 0x08:
			JGOtherJoin((SDHP_OTHERJOINMSG *)aRecv);
			break;

		case 0x09:
			GJPUserDisconnectRecv((SDHP_BILLKILLUSER *)aRecv);
			break;

		case 0x20:
			JGPExitCodeRecv((SDHP_EXITMSG *)aRecv);
			break;

		case 0x60:
			JGPEventStart((SDHP_EVENTSTART *)aRecv);
			break;

		case 0x61:
			JGPEventControl((SDHP_EVENTCONTROL *)aRecv);
			break;

		case 0x62:
			JGPOptionControl((SDHP_OPTIONCONTROL *)aRecv);
			break;

		case 0x63:
			JGPSendMail((PMSG_JG_MEMO_SEND *)aRecv);
			break;

		case 0x30:
			LoveHeartEventRecv((SDHP_LOVEHEARTEVENT_RESULT *)aRecv);
			break;

		case 0x40:
			AllNoticeRecv((SDHP_NOTICE *)aRecv);
			break;

		case 0x41:
			UserNoticeRecv((SDHP_USER_NOTICE *)aRecv);
			break;

		case 0x7A:
			JGAnsMapSvrMove((PMSG_ANS_MAPSVRMOVE *)aRecv);
			break;

		case 0x7B:
			JGAnsMapSvrAuth((PMSG_ANS_MAPSVRAUTH *)aRecv);
			break;
	}
}


void GJServerLogin()
{
	SDHP_SERVERINFO pInfo;

	pInfo.h.c = 0xC1;
	pInfo.h.size = sizeof(SDHP_SERVERINFO);
	pInfo.h.headcode = 0x00;
	pInfo.Port = g_ConfigRead.server.GetGameServerPort();
	pInfo.Type = 1;
	strcpy(pInfo.ServerName , g_ConfigRead.server.GetServerName());
	pInfo.ServerCode = g_ConfigRead.server.GetGameServerCode();
	pInfo.ServerVIP = g_VipSystem.IsServerVIP();
	pInfo.MaxHWIDUseCount = g_ConfigRead.server.GetHWIDUseLimit();

	wsJServerCli.DataSend((char*)&pInfo, pInfo.h.size);
}



void JGServerLoginResult( SDHP_RESULT * lpMsg)	
{
	if ( lpMsg->Result == false )
	{
		g_Log.Add("Authentication error from authentication server");
		return;
	}

	JoinServerConnected=TRUE;
	gServerReady++;
	SendMessage(ghWnd, WM_START_SERVER, 0, 0);

}


void GJPUserClose(LPSTR szAccountID)
{
	SDHP_USERCLOSE_ID pClose;

	pClose.h.c =0xC1;
	pClose.h.size= sizeof(SDHP_USERCLOSE_ID);
	pClose.h.headcode = 0x05;
	memcpy(pClose.szId , szAccountID, 10);

	wsJServerCli.DataSend((char*)&pClose, pClose.h.size);
}

void GJPUserClose(LPOBJ lpObj)
{
	SDHP_USERCLOSE_ID pClose;

	pClose.h.c =0xC1;
	pClose.h.size= sizeof(SDHP_USERCLOSE_ID);
	pClose.h.headcode = 0x05;
	memcpy(pClose.szId , lpObj->AccountID, 10);
	memcpy(pClose.szName, lpObj->BackName, 10);
	pClose.Level = lpObj->Level;
	pClose.DbClass = lpObj->m_PlayerData->DbClass;

	wsJServerCli.DataSend((char*)&pClose, pClose.h.size);
}

void JGPAccountRequest(SDHP_IDPASSRESULT * lpMsg)
{
	char szId[11];
	short aIndex;
	char szJN[21]="";
	
	memset(szId, 0, sizeof(szId));
	memcpy(szId, lpMsg->Id , sizeof(lpMsg->Id));
	memcpy(szJN, lpMsg->JoominNumber , sizeof(lpMsg->JoominNumber));

	aIndex = lpMsg->Number ;
	
	if(lpMsg->result == 3) // offtrade allow login-back if user is on offtrade
	{
		for(int i=g_ConfigRead.server.GetObjectStartUserIndex();i<g_ConfigRead.server.GetObjectMax();i++)
		{
			if(!strcmpi(szId,gObj[i].AccountID))
			{
				if(gObj[i].m_bOff)
				{
					GJSetCharacterInfo(&gObj[i], i, FALSE);
					gObjDel(i);
					gObj[i].m_bOff = false;
					

					lpMsg->result = 1;
					break;
				}
			}
		}
	}
	switch ( lpMsg->result  )
	{
		case 1:
		case 15:
			if (gObjSetAccountLogin(aIndex, szId, lpMsg->UserNumber , lpMsg->DBNumber, szJN ) == 0 )
			{
				GJPAccountFail(aIndex, szId, lpMsg->DBNumber , lpMsg->UserNumber  );
				lpMsg->result  = 4;
			}
			else
			{	
				g_Log.Add("Login [%s] : (%s) Numer: %d / DB: %d", gObj[aIndex].m_PlayerData->Ip_addr, szId, lpMsg->UserNumber , lpMsg->DBNumber );
			}

			if ( lpMsg->UserNumber  == 0 && lpMsg->DBNumber  == 0)
			{
				g_Log.Add("Both user and DB number are 0 (%s) %d %d", szId, lpMsg->UserNumber , lpMsg->DBNumber );
			}
	}

	if ( lpMsg->result  == 15 )
	{
		lpMsg->result  = 1;
		gObj[aIndex].m_cAccountItemBlock = 1;
	}
	
	gObj[aIndex].LoginMsgSnd = 0;

	if (gObj[aIndex].LoginMsgCount > 3 )
	{
		lpMsg->result = 8;
	}

	GSProtocol.GCJoinResult(lpMsg->result , aIndex);

	if ( lpMsg->result  != 1 )
	{
		memset(gObj[aIndex].m_PlayerData->HWID, 0, sizeof(gObj[aIndex].m_PlayerData->HWID));
		if ( gObj[aIndex].LoginMsgCount > 3 )
		{
			IOCP.CloseClient(aIndex);
		}
	}
}



				
void GJPAccountFail(short number, LPSTR Id, int DBNumber, int UserNumber)
{
	SDHP_JOINFAIL pMsg;

	memset(&pMsg, 0, sizeof(SDHP_JOINFAIL));

	pMsg.h.c=0xC1;
	pMsg.h.headcode= 0x02;
	pMsg.h.size = sizeof (SDHP_JOINFAIL);
	pMsg.Number = number;
	pMsg.DBNumber = DBNumber;
	pMsg.UserNumber = UserNumber;

	strcpy(pMsg.Id , Id);

	wsJServerCli.DataSend((char*)&pMsg, pMsg.h.size);
}


void GJPAccountBlock(short number, LPSTR Id, int DBNumber, int UserNumber, int BlockCode)
{
	SDHP_COMMAND_BLOCK pMsg;

	memset(&pMsg, 0, sizeof(SDHP_COMMAND_BLOCK));

	pMsg.h.c=0xC1;
	pMsg.h.headcode= 0x04;
	pMsg.h.size = sizeof (SDHP_COMMAND_BLOCK);
	pMsg.Number = number;
	pMsg.DBNumber = DBNumber;
	pMsg.UserNumber = UserNumber;
	pMsg.BlockCode = BlockCode;

	strcpy(pMsg.Id , Id);

	wsJServerCli.DataSend((char*)&pMsg, pMsg.h.size);
}


void GJPBillCeckRecv(SDHP_BILLSEARCH_RESULT * lpMsg)
{
	int aIndex = lpMsg->Number ;
	char szId[11];
	char EndsDays[13];

	szId[10] = 0;

	memcpy(szId, lpMsg->Id, sizeof(lpMsg->Id));

	if (gObjIsAccontConnect(aIndex, szId) == 0 )
	{
		return ;
	}

	memset(EndsDays, 0, sizeof(EndsDays));
	memcpy(EndsDays, lpMsg->EndsDays, sizeof(lpMsg->EndsDays));

	if ( m_ObjBill[aIndex- g_ConfigRead.server.GetObjectStartUserIndex()].SetBill( lpMsg->cCertifyType , lpMsg->PayCode, lpMsg->EndTime , EndsDays) == TRUE )
	{
		gObjAddMsgSendDelay(&gObj[aIndex], 1000, aIndex, 100, 0);

		LPOBJ lpObj = &gObj[aIndex];
		g_VipSystem.SetVipForUser(lpObj, lpMsg->cVipType);

		g_Log.Add("[%s] VIP Info Set [Type: %d] [Effect: %d]", lpObj->AccountID, lpObj->m_PlayerData->VipType, lpObj->m_PlayerData->VipEffect);
	}
}


void GJPUserKillRecv(SDHP_BILLKILLUSER * lpMsg)
{
	char szId[11];
	int number;
	szId[10] = 0;

	memcpy(szId, lpMsg->Id , sizeof(lpMsg->Id) );
	number = lpMsg->Number;

	if (gObjIsAccontConnect(number, szId) == 0 )
	{
		return;
	}

	if(gObj[number].m_bOff == true)
	{
		GJSetCharacterInfo(&gObj[number], number, FALSE);
		gObj[number].m_bOff = false;
		gObjDel(number);
		GSProtocol.GCSendDisableReconnect(number);
		//IOCP.CloseClient(number);
		return;
	}

	if (lpMsg->ForceDisconnect == true)
	{
		GSProtocol.GCSendDisableReconnect(number);
		//IOCP.CloseClient(number);
	}

	else
	{
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,151), number, 0);
		GSProtocol.GCSendDisableReconnect(number);
		gObjUserKill(number);
	}
}


void JGOtherJoin(SDHP_OTHERJOINMSG * lpMsg)
{
	char szId[11];
	szId[10]=0;
	int n;

	memcpy(szId, lpMsg->AccountID, sizeof(lpMsg->AccountID));

	for (n= g_ConfigRead.server.GetObjectStartUserIndex(); n<g_ConfigRead.server.GetObjectMax();n++)
	{
		if ( gObj[n].Connected >= PLAYER_LOGGED )
		{
			if ( gObj[n].AccountID[0] == szId[0] )
			{
				if ( gObj[n].AccountID[1] == szId[1] )
				{
					if ( strcmp(gObj[n].AccountID , szId) == 0 )
					{
						GSProtocol.GCServerMsgStringSend(Lang.GetText(0,41), n, 1 );
						break;
					}
				}
			}
		}
	}
}

void GJPUserDisconnectRecv( SDHP_BILLKILLUSER * lpMsg)
{
	char szId[11];
	szId[10]=0;
	int number;

	memcpy( szId, lpMsg->Id , sizeof( lpMsg->Id ) );
	number = lpMsg->Number;

	if ( gObjIsAccontConnect(number, szId ) == 0 )
	{
		return;
	}

	GSProtocol.GCServerMsgStringSend(Lang.GetText(0,152), number, 0 );
	GSProtocol.GCSendDisableReconnect(number);
	gObjUserKill(number);
}


void JGPExitCodeRecv( SDHP_EXITMSG * lpMsg )
{
	if ( lpMsg->ExitCode[0] == 70 && lpMsg->ExitCode[1] == 70 && lpMsg->ExitCode[2] == 88 )
	{
		SendMessage( ghWnd, WM_COMMAND, ID_DISCONNECTWITHMESSAGE_3MINUTES, 0 );
	}
}



void JGPEventStart( SDHP_EVENTSTART * lpMsg )
{
	if ( lpMsg->Event == 1 )
	{
		if (DragonEvent->GetState() != 0 )
		{
			DragonEvent->End();
		}
		else
		{
			DragonEvent->Start();
		}
	}
}



void JGPEventControl( SDHP_EVENTCONTROL * lpMsg )
{
	switch ( lpMsg->Event  )
	{
		case DRAGON_EVENT:
			if ( lpMsg->Stat != 0 )
			{
				if (DragonEvent->GetState() == 0 )
				{
					DragonEvent->Start();
				}
			}
			else
			{
				DragonEvent->End();
			}
			break;

		case XMAS_EVENT:
			gXMasEvent = lpMsg->Stat;
			break;

		case FIRECRACKER_EVENT:
			gFireCrackerEvent = lpMsg->Stat;
			break;

		case HEARTOFLOVE_EVENT:
			gHeartOfLoveEvent = lpMsg->Stat;
			break;

		case MEDAL_EVENT:
			gMedalEvent = lpMsg->Stat;
			break;
	}
}


void JGPOptionControl( SDHP_OPTIONCONTROL * lpMsg )
{
	switch ( lpMsg->Option )
	{
		case OPTION_RELOAD:
			ReadCommonServerInfo();
			break;

		case MONSTER_RELOAD:
			GameMonsterAllCloseAndReLoad();
			break;

		case SHOP_RELOAD:
			g_ShopMng.LoadShopList(g_ConfigRead.GetPath("IGC_ShopList.xml"));
			break;

		case ALLOPTION_RELOAD:
			ReadCommonServerInfo();
			GameMonsterAllCloseAndReLoad();
			g_ShopMng.LoadShopList(g_ConfigRead.GetPath("IGC_ShopList.xml"));
			break;

		case EVENTINFO_RELOAD:
			ReadEventInfo(MU_EVENT_ALL );
			break;

		case ETCINFO_RELOAD:
			ReadGameEtcInfo(MU_ETC_ALL );
			break;
	}
}


void LoveHeartEventSend(int aIndex, LPSTR AcountId, LPSTR Name)
{
	SDHP_LOVEHEARTEVENT pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0x30;
	pMsg.h.size = sizeof(SDHP_LOVEHEARTEVENT);

	memcpy(pMsg.Account , AcountId, sizeof(pMsg.Account));
	memcpy(pMsg.Name , Name, sizeof(pMsg.Name) );

	wsJServerCli.DataSend((char*)&pMsg, pMsg.h.size);
}



void LoveHeartEventRecv( SDHP_LOVEHEARTEVENT_RESULT * lpMsg )
{
	char szName[11];
	int number;
	char Msg[256];

	szName[10]=0;

	memcpy(szName, lpMsg->Name, sizeof(lpMsg->Name));
	number = lpMsg->Number;

	switch ( lpMsg->Result )
	{
		case 0: // Registered Hearts of Love PangPangEvent
			{
				wsprintf(Msg, Lang.GetText(0,17), number );

				for (int n = g_ConfigRead.server.GetObjectStartUserIndex();n<g_ConfigRead.server.GetObjectMax();n++)
				{
					if ( gObj[n].Connected >= PLAYER_LOGGED )
					{
						if (gObj[n].Name[0] == szName[0] )
						{
							if ( gObj[n].Name[1] == szName[1] )
							{
								if ( strcmp( gObj[n].Name, szName ) == 0 )
								{
									GSProtocol.GCServerMsgStringSend(Msg, n, 1);
									wsprintf(Msg, Lang.GetText(0,18));
									GSProtocol.GCServerMsgStringSend(Msg, n, 1);
									return;
								}
							}
						}
					}
					continue;
				} // For
			}
		break;

		case 1: // Winner of the Love Pang Pang Event
			wsprintf(Msg, Lang.GetText(0,19), szName, number);
			GSProtocol.AllSendServerMsg(Msg);
			break;
	}
}



void LoveHeartCreateSend()
{
	SDHP_LOVEHEARTCREATE pMsg;

	pMsg.h.c= 0xC1;
	pMsg.h.headcode = 0x31;
	pMsg.h.size=sizeof(SDHP_LOVEHEARTCREATE);

	wsJServerCli.DataSend((char*)&pMsg, pMsg.h.size);
}



void AllNoticeRecv( SDHP_NOTICE * lpMsg)
{
	char szNotice[61];

	szNotice[60]=0;

	memcpy(szNotice, lpMsg->Notice , sizeof(lpMsg->Notice)-1);

	g_Log.Add("[Mu Manager][Notice Log][Type:ALL] %s", szNotice);

	if ( strlen(szNotice) > 1 )
	{
		GSProtocol.AllSendServerMsg(szNotice);
	}
}



void UserNoticeRecv( SDHP_USER_NOTICE * lpMsg)
{
	char szNotice[61];
	char szId[11];

	szNotice[60]=0;
	szId[10]=0;

	memcpy(szNotice, lpMsg->Notice , sizeof(lpMsg->Notice)-1);
	memcpy(szId, lpMsg->szId , sizeof(lpMsg->szId));

	g_Log.Add("[Mu Manager][Notice Log][Type:USER] (%s) %s", szId, szNotice);

	if ( strlen(szNotice) > 1 )
	{
		if ( strlen(szId) > 1 )
		{
			for ( int n = g_ConfigRead.server.GetObjectStartUserIndex();n<g_ConfigRead.server.GetObjectMax(); n++)
			{
				if ( gObj[n].Connected >= PLAYER_LOGGED )
				{
					if ( gObj[n].AccountID[0] == szId[0] )
					{
						if ( gObj[n].AccountID[1] == szId[1] )
						{
							if ( strcmp( gObj[n].AccountID , szId) == 0 )
							{
								PMSG_NOTICE pNotice;
								TNotice::MakeNoticeMsg(&pNotice, 0, szNotice);
								TNotice::SetNoticeProperty(&pNotice, 10,  _ARGB(255, 254, 81, 81), 1, 0, 20);
								TNotice::SendNoticeToUser(n, &pNotice);
								break;
							}
						}
					}
				}
			} // For
		}
	}
}



void GJReqMapSvrMove(int iIndex, WORD wDesMapSvrCode, WORD wMapNumber, BYTE btX, BYTE btY)
{
	PMSG_REQ_MAPSVRMOVE pMsg;
	
	if ( gObjIsConnected(iIndex) == PLAYER_EMPTY )
	{
		g_Log.AddC(TColor::Red,  "[MapServerMng] Packet Error GJ [0x7A] - User not Connected : %d", iIndex);
	}

	else if ( gObj[iIndex].Type != OBJ_USER )
	{
		g_Log.AddC(TColor::Red,  "[MapServerMng] Packet Error GJ [0x7A] - User is not character : %d", iIndex);
	}

	else if ( gObj[iIndex].m_bMapSvrMoveReq_2 == true )
	{
		g_Log.AddC(TColor::Red,  "[MapServerMng] Packet Error GJ [0x7A] - Request already sent : %d", iIndex);
	}

	else
	{
		gObj[iIndex].m_bMapSvrMoveReq_2 = true;

		PHeadSetB((LPBYTE)&pMsg, 0x7A, sizeof(PMSG_REQ_MAPSVRMOVE));
		pMsg.iIndex = iIndex;

		memcpy(pMsg.szAccountID , gObj[iIndex].AccountID, sizeof( pMsg.szAccountID)-1);
		pMsg.szAccountID[10] = 0;
		memcpy(pMsg.szCharName, gObj[iIndex].Name, sizeof( pMsg.szCharName)-1);
		pMsg.szCharName[10] = 0;

		memcpy(pMsg.szPassword, gObj[iIndex].m_PlayerData->Password, sizeof(pMsg.szPassword));

		pMsg.wCurMapSvrCode = g_ConfigRead.server.GetGameServerCode();
		pMsg.wDstMapSvrCode = wDesMapSvrCode;
		pMsg.wMapNumber = wMapNumber;
		pMsg.btX = btX;
		pMsg.btY = btY;
		pMsg.btSecurityLock = (BYTE)gObj[iIndex].m_PlayerData->m_bSecurityCheck;
		pMsg.dwSecurityCode = gObj[iIndex].m_PlayerData->m_iSecurityCode;

		wsJServerCli.DataSend((char*)&pMsg, pMsg.h.size);
	}
}



struct PMSG_ANS_MAPSERVERCHANGE
{
	PBMSG_HEAD2 h;	// C1:B1:00
	char szMapSvrIpAddress[16];	// 4
	WORD wMapSvrPort;	// 14
	WORD wMapSvrCode;	// 16
	int iJoinAuthCode1;	// 18
	int iJoinAuthCode2;	// 1C
	int iJoinAuthCode3;	// 20
	int iJoinAuthCode4;	// 24
};

void JGAnsMapSvrMove(PMSG_ANS_MAPSVRMOVE * lpMsg)
{
	if ( gObjIsConnected(lpMsg->iIndex ) == FALSE )
	{
		g_Log.AddC(TColor::Red,  "[MapServerMng] Packet Error JG [0x7A] - User not Connected : %d", lpMsg->iIndex);
		return;
	}

	LPOBJ lpObj = &gObj[lpMsg->iIndex];

	if ( strcmp( lpObj->AccountID , lpMsg->szAccountID )  ||  strcmp( lpObj->Name, lpMsg->szCharName) )
	{
		g_Log.AddC(TColor::Red,  "[MapServerMng] Packet Error JG [0x7A] - User Info Mismatch [%s][%s]-[%s][%s] (%d)", lpMsg->szAccountID, lpMsg->szCharName, lpObj->AccountID, lpObj->Name, lpObj->m_Index);
		return;
	}

	if ( lpObj->Type != OBJ_USER)
	{
		g_Log.AddC(TColor::Red,  "[MapServerMng] Packet Error JG [0x7A] - User is not character [%s][%s]-[%s][%s] (%d)", lpMsg->szAccountID, lpMsg->szCharName, lpObj->AccountID, lpObj->Name, lpObj->m_Index);		
		return;
	}

	if ( lpObj->m_bMapSvrMoveReq_2 == false )
	{
		g_Log.AddC(TColor::Red,  "[MapServerMng] Packet Error JG [0x7A] - Request was not send by GS [%s][%s]-[%s][%s] (%d)", lpMsg->szAccountID, lpMsg->szCharName, lpObj->AccountID, lpObj->Name, lpObj->m_Index);		
		return;
	}

	if ( lpMsg->iResult == 0	 )
	{
		PMSG_ANS_MAPSERVERCHANGE pMsg;

		PHeadSubSetBE((LPBYTE)&pMsg, 0xB1, 0x00, sizeof(pMsg));
		
		pMsg.wMapSvrCode = g_MapServerManager.GetMapSvrGroup();
		pMsg.iJoinAuthCode1 = lpMsg->iJoinAuthCode1 ;
		pMsg.iJoinAuthCode2 = lpMsg->iJoinAuthCode2 ;
		pMsg.iJoinAuthCode3 = lpMsg->iJoinAuthCode3;
		pMsg.iJoinAuthCode4 = lpMsg->iJoinAuthCode4 ;
	
		BOOL bRESULT = g_MapServerManager.GetSvrCodeData(lpMsg->wDstMapSvrCode, pMsg.szMapSvrIpAddress, &pMsg.wMapSvrPort);

		if ( bRESULT == FALSE )
		{
			g_Log.AddC(TColor::Red,  "[MapServerMng] Packet Error JG [0x7A] - Failed to find Map Info [%s]-[%s] (%d)",
				lpObj->AccountID, lpObj->Name, lpMsg->wDstMapSvrCode);
			return;
		}

		GJSetCharacterInfo(lpObj, lpObj->m_Index, TRUE);
		lpObj->m_bMapSvrMoveQuit = TRUE;
		lpObj->m_dwMapSvrQuitTick = GetTickCount();

		IOCP.DataSend(lpMsg->iIndex, (LPBYTE)&pMsg, pMsg.h.size);

		g_Log.Add("[MapServerMng] [%s][%s] JG [0x7A] - Move Map Request OK : MAPSVR_D:%d",
			lpObj->AccountID, lpObj->Name, lpMsg->wDstMapSvrCode);
	}
	else
	{
		PMSG_ANS_MAPSERVERCHANGE pMsg = { 0 };//Season 4.5 addon
		memset(&pMsg, 0x00, sizeof(pMsg));
		PHeadSubSetBE((LPBYTE)&pMsg, 0xB1, 0x00, sizeof(pMsg));//Season 4.5 addon
		IOCP.DataSend(lpMsg->iIndex, (LPBYTE)&pMsg, pMsg.h.size);//Season 4.5 addon
		
		lpObj->m_PlayerData->MoveGate = -1;//Season 4.5 addon
		lpObj->m_State = 2;//Season 4.5 addon
		lpObj->MapNumber = 0;
		lpObj->X = 130;
		lpObj->Y = 130;

		PMSG_NOTICE pNotice;

		TNotice::MakeNoticeMsgEx(&pNotice, 0, Lang.GetText(0,560));
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pNotice, pNotice.h.size);

		g_Log.AddC(TColor::Red, "[MapServerMng] Packet Error JG [0x7A] - Failed to Authorize User [%s][%s] Result:%d",
			lpObj->AccountID, lpObj->Name, lpMsg->iResult);

		lpObj->m_bMapSvrMoveReq_2 = false;
	}
}
			

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

void GJReqMapSvrAuth(int iIndex, LPSTR lpszAccountID, LPSTR lpszCharName, int iJA1, int iJA2, int iJA3, int iJA4)
{
	if ( !ObjectMaxRange(iIndex))
	{
		g_Log.AddC(TColor::Red,  "[MapServerMng] Packet Error GJ [0x7B] - Index out of bound : %d", iIndex);
		return;
	}

	gObj[iIndex].m_bMapSvrMoveReq = true;

	PMSG_REQ_MAPSVRAUTH pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x7B, sizeof(pMsg));
	pMsg.iIndex = iIndex;
	memcpy(pMsg.szAccountID, lpszAccountID, MAX_ACCOUNT_LEN);
	memcpy(pMsg.szCharName, lpszCharName, MAX_ACCOUNT_LEN);
	pMsg.wDstMapSvrCode = g_ConfigRead.server.GetGameServerCode();
	pMsg.iJoinAuthCode1 = iJA1;
	pMsg.iJoinAuthCode2 = iJA2;
	pMsg.iJoinAuthCode3 = iJA3;
	pMsg.iJoinAuthCode4 = iJA4;

	wsJServerCli.DataSend((PCHAR)&pMsg, pMsg.h.size);
}

void JGAnsMapSvrAuth(PMSG_ANS_MAPSVRAUTH * lpMsg)
{
	if ( !ObjectMaxRange(lpMsg->iIndex))
	{
		g_Log.AddC(TColor::Red,  "[MapServerMng] Packet Error JG [0x7B] - Index out of bound : %d",
			lpMsg->iIndex);
		
		return;
	}

	LPOBJ lpObj = &gObj[lpMsg->iIndex];
	memcpy(lpObj->Name, lpMsg->szCharName, MAX_ACCOUNT_LEN);

	if ( lpObj->m_bMapSvrMoveReq == false )
	{
		g_Log.AddC(TColor::Red,  "[MapServerMng] MapSvr MoveReq Error JG [0x7B] - No Request [%s] (%d)",
			lpObj->AccountID, lpObj->m_Index);

		gObjDel(lpMsg->iIndex);
		return;
	}

	BOOL bAuthOK = FALSE;
	int iSendResult = 0;
	char szId[11];
	char szPw[21] = "";
	int iIndex = lpMsg->iIndex;
	SDHP_DBCHARINFOREQUEST pMsg;
	char szName[11];

	memset(szId, 0, sizeof(szId));
	memcpy(szId, lpMsg->szAccountID, MAX_ACCOUNT_LEN);
	memcpy(szPw, lpMsg->szPassword, sizeof(lpMsg->szPassword));
	
	switch ( lpMsg->iResult )
	{
		case 0:
			iSendResult = 0;
			bAuthOK = FALSE;

			if ( !gObjSetAccountLogin(iIndex, szId, lpMsg->iUserNumber, lpMsg->iDBNumber, szPw))
			{
				GJPAccountFail(iIndex, szId, lpMsg->iDBNumber, lpMsg->iUserNumber);
				lpMsg->iResult = 4;
			}
			else
			{
				g_Log.Add("Login [%s] : (%s) Numer: %d / DB: %d", gObj[iIndex].m_PlayerData->Ip_addr, szId, lpMsg->iUserNumber, lpMsg->iDBNumber);
				iSendResult = 1;
				bAuthOK = TRUE;
			}

			if ( lpMsg->iUserNumber == 0 && lpMsg->iDBNumber == 0 )
			{
				g_Log.Add("Both use and DB number are 0 (%s) %d %d", szId, lpMsg->iUserNumber, lpMsg->iDBNumber);
			}

			switch ( lpMsg->btBlockCode )
			{
				case 70:	// 0x46
					gObj[iIndex].m_cAccountItemBlock = 1;
					break;
			}
			break;

		default:
			iSendResult = 0;
			bAuthOK = FALSE;
			break;
	}

	GSProtocol.GCAnsMapSvrAuth(iIndex, iSendResult);

	if ( bAuthOK == FALSE )
	{
		g_Log.AddC(TColor::Red,  "[MapServerMng] [%s][%s] JG [0x7B] - Join Result Failed : RST: [%d], BLK: [%c]",
			szId, lpMsg->szCharName, lpMsg->iResult, lpMsg->btBlockCode);
		gObjDel(lpMsg->iIndex);

		return;
	}
	
	gObj[iIndex].m_sPrevMapSvrCode = lpMsg->wPrevMapSvrCode;
	gObj[iIndex].m_sDestMapNumber  = lpMsg->wMapNumber;
	gObj[iIndex].m_btDestX = lpMsg->btX;
	gObj[iIndex].m_btDestY = lpMsg->btY;
	gObj[iIndex].bEnableDelCharacter = FALSE;
	gObj[iIndex].m_PlayerData->m_bSecurityCheck = lpMsg->btSecurityLock;
	gObj[iIndex].m_PlayerData->m_iSecurityCode = lpMsg->dwSecurityCode;
	PHeadSetB((LPBYTE)&pMsg, 0x06, sizeof(pMsg));
	memset(szName, 0, sizeof(szName));
	memcpy(szName, lpMsg->szCharName, MAX_ACCOUNT_LEN);
	//BuxConvert(szName, MAX_ACCOUNT_LEN);
	memcpy(pMsg.Name, szName, MAX_ACCOUNT_LEN);
	strcpy(pMsg.AccountID, lpMsg->szAccountID);
	pMsg.Number = lpObj->m_Index;
	pMsg.IsSeasonX = g_ConfigRead.SeasonX;

	if ( g_ArcaBattle.IsArcaBattleServer() == TRUE )
	{
		g_ArcaBattle.SendArcaBattlePlayInfo(&gObj[iIndex], 1);
		g_ArcaBattle.SendArcaBattleCurrentState(&gObj[iIndex]);
	}

	wsDataCli.DataSend((PCHAR)&pMsg, pMsg.h.size);

	g_Log.Add("[MapServerMng] [%s][%s] JG [0x7B] - Join Result OK : RST:%d, BLK:%c",
		szId, lpMsg->szCharName, lpMsg->iResult, lpMsg->btBlockCode);

}


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

void GJNotifyMaxUserCount()
{
	PMSG_NOTIFY_MAXUSER pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x7C, sizeof(PMSG_NOTIFY_MAXUSER));
	pMsg.iSvrCode = g_ConfigRead.server.GetGameServerCode();
	pMsg.iMaxUserCount = g_ConfigRead.server.GetObjectMaxUser();
	pMsg.iCurUserCount = gObjTotalUser;
	
	wsJServerCli.DataSend((char*)&pMsg, pMsg.h.size);
}

void JGPSendMail(PMSG_JG_MEMO_SEND * lpMsg)
{
	MngFriendMemoSend(lpMsg);
}



struct SDHP_REQ_UPDATE_MATCHDB_USERCHARACTERS
{
	PBMSG_HEAD h;	// C1:16
	char szId[10];	// 3
	char szName[10];	// D
	WORD Level;	// 18
	BYTE DbClass;	// 1A
};

void GJUpdateMatchDBUserCharacters(LPOBJ lpObj)
{
	SDHP_REQ_UPDATE_MATCHDB_USERCHARACTERS pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x16, sizeof(SDHP_REQ_UPDATE_MATCHDB_USERCHARACTERS));
	pMsg.DbClass = lpObj->m_PlayerData->DbClass;
	pMsg.Level = lpObj->Level;
	memcpy(pMsg.szId, lpObj->AccountID, sizeof(pMsg.szId));
	memcpy(pMsg.szName, lpObj->BackName, sizeof(pMsg.szName));

	wsJServerCli.DataSend((PCHAR)&pMsg, pMsg.h.size);
}

void GJReqSetOffTrade(LPOBJ lpObj)
{
	PMSG_SET_OFFTRADE pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x80, sizeof(PMSG_SET_OFFTRADE));
	pMsg.m_bState = true;
	memcpy(pMsg.szAccountID, lpObj->AccountID, sizeof(pMsg.szAccountID));
	memcpy(pMsg.szName, lpObj->Name, sizeof(pMsg.szName));

	wsJServerCli.DataSend((PCHAR)&pMsg, pMsg.h.size);
}

void GJVipBuy(char AccountID[11], BYTE Type, int Days)
{
	ISHOP_VIP_BUY pVip;

	PHeadSetB((LPBYTE)&pVip, 0xD3, sizeof(pVip));

	memcpy(pVip.AccountID, AccountID, 11);

	pVip.Type = Type;
	pVip.Days = Days;

	wsJServerCli.DataSend((PCHAR)&pVip, pVip.h.size);
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

