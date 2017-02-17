//////////////////////////////////////////////////////////////////////
// NewPVP.cpp
#include "stdafx.h"
#include "NewPVP.h"
#include "GameMain.h"
#include "TNotice.h"
#include "winutil.h"
#include "BuffEffect.h"
#include "BuffEffectSlot.h"
#include "MoveCommand.h"
#include "DarkSpirit.h"
#include "Gate.h"
#include "TLog.h"
#include "PartyClass.h"
#include "BagManager.h"
#include "MineSystem.h"
#include "IniReader.h"
#include "configread.h"
#include "MuunSystem.h"

extern PartyClass		gParty;

CNewPVP g_NewPVP;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNewPVP::CNewPVP(): m_bNewPVP(FALSE)
{
	memset(&m_DuelChannelList, 0, sizeof(m_DuelChannelList));
	memset(m_DuelChannel, 0, sizeof(m_DuelChannel));
	memset(&m_ObserverCount, 0, sizeof(m_ObserverCount));

	m_DuelChannelList.h.c = 0xC1;
	m_DuelChannelList.h.headcode = 0xAA;
	m_DuelChannelList.h.subcode = 0x06;
	m_DuelChannelList.h.size = sizeof(PMSG_ANS_DUEL_CHANNELLIST);

	InitializeCriticalSection(&this->m_csWaiting);
	InitializeCriticalSection(&this->m_csObserver);
}

CNewPVP::~CNewPVP()
{
	m_ObserverInfoList.clear();
	m_Waiting.clear();
	DeleteCriticalSection(&this->m_csWaiting);
	DeleteCriticalSection(&this->m_csObserver);
}

void CNewPVP::LoadData()
{
	CIniReader ReadDuel(g_ConfigRead.GetPath("IGC_Common.ini"));

	m_bNewPVP = ReadDuel.ReadInt("DuelSystem", "Enable", FALSE);
	m_iDuelDamageReduction = ReadDuel.ReadInt("DuelSystem", "DamageReduction", 60);

	g_Log.Add("[NewPVP] Info Set Enable:%d DamageReduction:%d", m_bNewPVP, m_iDuelDamageReduction);
}

static int g_GateObserver[DUEL_CHANNEL_MAX] =  {	303,304,305,306 };
static int g_GateRequester[DUEL_CHANNEL_MAX] = {	295,297,299,301 };
static int g_GateResponsor[DUEL_CHANNEL_MAX] = {	296,298,300,302 };

void CNewPVP::Run()
{
	EnterCriticalSection(&this->m_csWaiting);
	std::map<int, _tagWaiting>::iterator iter = m_Waiting.begin();

	while(iter!=m_Waiting.end())
	{
		_tagWaiting & waiting = iter->second;
		if(waiting.bExpired == 1)
		{
			iter = m_Waiting.erase(iter);
			continue;
		}

		DWORD dwRemainMsec = GetTickCount() - waiting.dwTime;			
		if( dwRemainMsec > 10000 )
		{
			OBJECTSTRUCT & requester = gObj[waiting.nRequester];
			OBJECTSTRUCT & responsor = gObj[waiting.nResponsor];
			Cancel(requester, responsor, FALSE);
		}		
		iter++;
	}

	LeaveCriticalSection(&this->m_csWaiting);
	
	for(int i = 0;i < DUEL_CHANNEL_MAX;i++)
	{
		int nStatus = m_DuelChannel[i].nStatus;
		switch(nStatus)
		{
		case 3:
			if(!CheckUsersOnConnect(i))	continue;
			break;
		case 2:
			{
				DWORD dwRemainMsec = GetTickCount() - m_DuelChannel[i].dwTime;
				int nNotifySec = 6 - dwRemainMsec / 1000;				
				if(nNotifySec < 6 && m_DuelChannel[i].nNotifyCount != nNotifySec )
				{
					m_DuelChannel[i].nNotifyCount = nNotifySec;
					BroadcastMessage(i, 3, 0, nNotifySec);
				}
				if(dwRemainMsec > 6000)
				{	
					if(!CheckUsersOnConnect(i))	continue;
					
					OBJECTSTRUCT &requester = gObj[m_DuelChannel[i].nIndex1];
					OBJECTSTRUCT &responsor = gObj[m_DuelChannel[i].nIndex2];
					
					gObjClearBuffEffect(&requester, CLEAR_TYPE_NON_PCS_ITEM_EFFECT);
					gObjClearBuffEffect(&responsor, CLEAR_TYPE_NON_PCS_ITEM_EFFECT);

					ReFill(requester);
					ReFill(responsor);

					BroadcastRound(i, 2, FALSE);
					SetStatus(3, i, requester, responsor);
				}
				break;
			}
		case 4:
			{
				DWORD dwRemainMsec = GetTickCount() - m_DuelChannel[i].dwTime;
				if(dwRemainMsec > 5000)
				{
					if(!CheckUsersOnConnect(i))	continue;
					
					OBJECTSTRUCT & requester = gObj[m_DuelChannel[i].nIndex1];
					OBJECTSTRUCT & responsor = gObj[m_DuelChannel[i].nIndex2];

					MoveGate(requester.m_Index, g_GateRequester[i]);
					MoveGate(responsor.m_Index,g_GateResponsor[i]);
					ReFill( requester);
					ReFill( responsor);
					BroadcastRound( i, 2, FALSE);
					SetStatus(3, i, requester, responsor);
				}
			}
			break;
		case DC_RESERVEDEND:
			{
				DWORD dwRemainMsec  = GetTickCount() - m_DuelChannel[i].dwTime;
				int nNotifySec = 10 - dwRemainMsec / 1000;
				if( nNotifySec < 6 && m_DuelChannel[i].nNotifyCount != nNotifySec )
				{
					m_DuelChannel[i].nNotifyCount = nNotifySec;
					BroadcastMessage(i, 3, 2, nNotifySec);
				}
				if( dwRemainMsec > 10000 )
				{
					OBJECTSTRUCT &requester = gObj[m_DuelChannel[i].nIndex1];
				  	OBJECTSTRUCT &responsor = gObj[m_DuelChannel[i].nIndex2];
					Leave( requester, responsor);
					LeaveChannelObserver(i);
					SetStatus(DC_IDLE, i, requester, responsor);
				}
			}
			break;
		}
		if( IS_START(nStatus) )	BroadcastDuelInfo(i, 2);
	}
}

int CNewPVP::GetDuelChannelId()
{
	for(int i = 0;i < DUEL_CHANNEL_MAX;i++)
	{
		if( m_DuelChannel[i].nStatus == DC_IDLE)
		{
			return i;
		}
	}

	return -1;
}

int CNewPVP::GetDuelChannelId(int nIndex)
{
	for( int i = 0; i < DUEL_CHANNEL_MAX; i++ )
	{
		if( m_DuelChannel[i].nIndex1 == nIndex || 
			m_DuelChannel[i].nIndex2 == nIndex )
		{
			return i;
		}
	}

	return -1;
}

int CNewPVP::GetDuelChannelId(char* lpszName)
{
	for( int i = 0; i < DUEL_CHANNEL_MAX; i++ )
	{
		if( strcmp(m_DuelChannel[i].szName1, lpszName) == 0|| 
			strcmp(m_DuelChannel[i].szName2, lpszName) == 0)
		{
			return i;
		}
	}
	
	return -1;
}

LPDUEL_CHANNEL CNewPVP::GetDuelChannel()
{
	for( int i = 0; i < DUEL_CHANNEL_MAX; i++ )
	{
		if( m_DuelChannel[i].nStatus == DC_IDLE )
		{
			return &m_DuelChannel[i];
		}
	}

	return NULL;
}

LPDUEL_CHANNEL CNewPVP::GetDuelChannel(char* lpszName)
{
	for( int i = 0; i < DUEL_CHANNEL_MAX; i++ )
	{
		if( strcmp(m_DuelChannel[i].szName1, lpszName) == 0 || 
			strcmp(m_DuelChannel[i].szName2, lpszName) == 0)
		{
			return &m_DuelChannel[i];
		}
	}

	return 0;
}

void CNewPVP::SetStatus(int nStatus, int nId, OBJECTSTRUCT &requester, OBJECTSTRUCT &responsor)
{
	LPDUEL_CHANNEL lpDuelChannel = &m_DuelChannel[nId];

	switch ( nStatus )
	{		
	case DC_IDLE:
		lpDuelChannel->nStatus = nStatus;
		lpDuelChannel->dwTime = 0;

		lpDuelChannel->nIndex1 = 0;
		lpDuelChannel->nIndex2 = 0;

		memset(lpDuelChannel->szName1, 0, MAX_ACCOUNT_LEN);
		memset(lpDuelChannel->szName2, 0, MAX_ACCOUNT_LEN);

		m_DuelChannelList.channel[nId].bStart = 0;
		m_DuelChannelList.channel[nId].bWatch = 0;

		memset(m_DuelChannelList.channel[nId].szName1, 0, MAX_ACCOUNT_LEN);
		memset(m_DuelChannelList.channel[nId].szName2, 0, MAX_ACCOUNT_LEN);

		SetDuelStatus(requester, responsor, nStatus);
		break;
	case 2:
		lpDuelChannel->nStatus = nStatus;
		lpDuelChannel->dwTime = GetTickCount();

		lpDuelChannel->nIndex1 = requester.m_Index;
		lpDuelChannel->nIndex2 = responsor.m_Index;

		memcpy(lpDuelChannel->szName1, requester.Name, MAX_ACCOUNT_LEN);
		memcpy(lpDuelChannel->szName2, responsor.Name, MAX_ACCOUNT_LEN);

		m_DuelChannelList.channel[nId].bStart = 1;
		m_DuelChannelList.channel[nId].bWatch = 1;

		memcpy(m_DuelChannelList.channel[nId].szName1, requester.Name, MAX_ACCOUNT_LEN);
		memcpy(m_DuelChannelList.channel[nId].szName2, responsor.Name, MAX_ACCOUNT_LEN);

		SetDuelStatus(requester, responsor, nStatus);
	  break;
	case 3:
	  lpDuelChannel->nStatus = nStatus;
	  lpDuelChannel->dwTime = GetTickCount();
	  break;
	case 4:
	  lpDuelChannel->nStatus = nStatus;
	  lpDuelChannel->dwTime = GetTickCount();
	  break;
	case DC_RESERVEDEND:
	  lpDuelChannel->nStatus = nStatus;
	  lpDuelChannel->dwTime = GetTickCount();
	  break;
	}
}

void CNewPVP::SetDuelStatus(OBJECTSTRUCT &requester, OBJECTSTRUCT &responsor, int nStatus)
{
	switch(nStatus)
	{
	case DC_IDLE:
		requester.m_iDuelUser = -1;
		requester.m_iDuelUserRequested = -1;
		requester.m_iDuelUserReserved = -1;
		
		responsor.m_iDuelUser = -1;
		responsor.m_iDuelUserRequested = -1;
		responsor.m_iDuelUserReserved = -1;
		break;
	case 1:
		requester.m_iDuelUserReserved = responsor.m_Index;
		requester.m_iDuelUser = -1;
		
		responsor.m_iDuelUserRequested = requester.m_Index;
		break;
	case 2:
		requester.m_iDuelUserReserved = -1;
        requester.m_btDuelScore = 0;
        requester.m_iDuelUser = responsor.m_Index;
        requester.m_iDuelTickCount = GetTickCount();
        
		responsor.m_iDuelUserRequested = -1;
        responsor.m_iDuelUserReserved = -1;
        responsor.m_btDuelScore = 0;
        responsor.m_iDuelUser = requester.m_Index;
        responsor.m_iDuelTickCount = GetTickCount();
		break;
	}
}

int CNewPVP::Reserve(OBJECTSTRUCT & requester,OBJECTSTRUCT& responsor)
{
	int nRet = GetDuelStatus(requester);
	if(nRet)	return nRet;

	nRet = GetDuelStatusByResponsor(requester);
	if(nRet)	return nRet;

	LPMOVE_COMMAND_DATA lpData = gMoveCommand.GetMoveCommandData(42);
	if(lpData == NULL){ 
		g_Log.Add("%s\t%s\t%s\t%s\t%d","lpData","ENEWPVP::E_NOT_FOUND_MOVEREQUESTDATA","NULL",__FILE__, __LINE__);
		return ENEWPVP::E_NOT_FOUND_MOVEREQUESTDATA; }

	if( !IsVulcanusMap(requester.MapNumber) )
	{
		if(IsVulcanusMap(requester.MapNumber) || requester.m_PlayerData->Money - lpData->NeedZen < 0)	return ENEWPVP::E_NOT_ENOUGH_MONEY;
	}
	if( !IsVulcanusMap(responsor.MapNumber) )
	{
		if(IsVulcanusMap(responsor.MapNumber) || responsor.m_PlayerData->Money - lpData->NeedZen < 0)	return ENEWPVP::E_NOT_ENOUGH_MONEY;
	}

	if( !CheckLimitLevel(requester.m_Index,g_GateRequester[0]) )	return ENEWPVP::E_LIMIT_LEVEL;
	if( !CheckLimitLevel(responsor.m_Index,g_GateResponsor[0]) )	return ENEWPVP::E_LIMIT_LEVEL;

	_tagWaiting waiting = {0};
    waiting.nRequester = requester.m_Index;
    waiting.nResponsor = responsor.m_Index;
    waiting.bExpired = 0;
    waiting.dwTime = GetTickCount();

	EnterCriticalSection(&this->m_csWaiting);

	std::pair< std::map<int,_tagWaiting>::iterator, bool > pair = m_Waiting.insert( std::make_pair(requester.m_Index,  waiting) );
	if(pair.second == false)
	{
		g_Log.Add("%s\t%s\t%s\t%s\t%d","pair.second","ENEWPVP::E_FAILED_ENTER","NULL",__FILE__,__LINE__);

		LeaveCriticalSection(&this->m_csWaiting);
		return ENEWPVP::E_FAILED_ENTER;
	}

	LeaveCriticalSection(&this->m_csWaiting);

	SetDuelStatus(requester, responsor, 1);

	PMSG_REQ_DUEL_ANSWER req;
    req.h.c = 0xC1;
    req.h.size = sizeof(PMSG_REQ_DUEL_ANSWER);
    req.h.headcode = 0xAA;
    req.h.subcode = 0x02;
    req.NumberH = HIBYTE(requester.m_Index);
    req.NumberL = LOBYTE(requester.m_Index);
    memcpy(req.szName, requester.Name, MAX_ACCOUNT_LEN);
    IOCP.DataSend(responsor.m_Index, (LPBYTE)&req, req.h.size);
	return 0;
}

int CNewPVP::Join(OBJECTSTRUCT &requester, OBJECTSTRUCT &responsor)
{
	if( IsDuel(requester) ) return ENEWPVP::E_ALREADY_DUELLING_1;
	if( IsDuelReserved(responsor) ) return ENEWPVP::E_ALREADY_DUELRESERVED_1;
	if( requester.m_iDuelUserReserved != responsor.m_Index ) return ENEWPVP::E_INVITE_MYSELF;

	EnterCriticalSection(&this->m_csWaiting);

	std::map<int, _tagWaiting>::iterator iter = m_Waiting.find(requester.m_Index);

	if(!(iter!=m_Waiting.end()))
	{ 
		g_Log.Add("%s\t%s\t%s\t%s\t%d","iter!=m_Waiting.end()","ENEWPVP::E_NOT_EXIST_USER","NULL", __FILE__,  __LINE__);

		LeaveCriticalSection(&this->m_csWaiting);
		return ENEWPVP::E_NOT_EXIST_USER;
	}

	_tagWaiting & waiting = iter->second;
	waiting.bExpired = 1;
	LeaveCriticalSection(&this->m_csWaiting);

	int	nId = GetDuelChannelId();
	if( nId < 0 || nId >= DUEL_CHANNEL_MAX ) return ENEWPVP::E_DUEL_MAX;

	if( m_DuelChannel[nId].nStatus != DC_IDLE ){ g_Log.Add("%s\t%s\t%s\t%s\t%d","m_DuelChannel[nId].nStatus==DC_IDLE","ENEWPVP::E_INVALID_STATUS","NULL", __FILE__, __LINE__); return ENEWPVP::E_INVALID_STATUS; }

	LPMOVE_COMMAND_DATA lpData = gMoveCommand.GetMoveCommandData(42);	
	if( lpData == NULL ){ g_Log.Add("%s\t%s\t%s\t%s\t%d","lpData","ENEWPVP::E_NOT_FOUND_MOVEREQUESTDATA","NULL",__FILE__, __LINE__); return ENEWPVP::E_NOT_FOUND_MOVEREQUESTDATA; }

	if( !CheckLimitLevel(requester.m_Index, g_GateRequester[nId]) )	return ENEWPVP::E_LIMIT_LEVEL;
	if( !CheckLimitLevel(responsor.m_Index, g_GateResponsor[nId]) ) return ENEWPVP::E_LIMIT_LEVEL;

	if( !IsVulcanusMap(requester.MapNumber) )
	{
		if( requester.m_PlayerData->Money - lpData->NeedZen >= 0 )
		{
			requester.m_PlayerData->Money -= lpData->NeedZen;
			GSProtocol.GCMoneySend(requester.m_Index, requester.m_PlayerData->Money);
		}
		else return ENEWPVP::E_NOT_ENOUGH_MONEY;
   	}
	
	if( !IsVulcanusMap(responsor.MapNumber) )
	{
		if( responsor.m_PlayerData->Money - lpData->NeedZen >= 0 )
		{
			responsor.m_PlayerData->Money -= lpData->NeedZen;
			GSProtocol.GCMoneySend(responsor.m_Index, responsor.m_PlayerData->Money);
		}
		else return ENEWPVP::E_NOT_ENOUGH_MONEY;
	}

	if (requester.m_PlayerData->m_bIsMining)
		g_MineSystem.CheckMoveMapWhileMining(requester.m_Index);
	if (responsor.m_PlayerData->m_bIsMining)
		g_MineSystem.CheckMoveMapWhileMining(responsor.m_Index);

	if (requester.m_PlayerData->m_MuBotEnable)
	{
		requester.m_PlayerData->m_MuBotEnable = false;
		GSProtocol.GCAnsMuBotUse(requester.m_Index, 1, requester.m_PlayerData->m_MuBotTotalTime, 0);
	}

	if (responsor.m_PlayerData->m_MuBotEnable)
	{
		responsor.m_PlayerData->m_MuBotEnable = false;
		GSProtocol.GCAnsMuBotUse(responsor.m_Index, 1, responsor.m_PlayerData->m_MuBotTotalTime, 0);
	}

	SetStatus(2, nId, requester, responsor);

	PMSG_ANS_DUEL_INVITE res = {0};
	PMSG_NOTICE pNotice;
	res.h.c = 0xC1;
	res.h.size = sizeof(PMSG_ANS_DUEL_INVITE);
	res.h.headcode = 0xAA;
	res.h.subcode = 0x01;
	res.nResult = 0x00;
	memcpy(res.szName, responsor.Name, MAX_ACCOUNT_LEN);

	res.NumberH = HIBYTE(requester.m_Index);
	res.NumberL = LOBYTE(requester.m_Index);
	memcpy(res.szName, requester.Name, MAX_ACCOUNT_LEN);
	IOCP.DataSend(responsor.m_Index, (LPBYTE)&res,res.h.size);

	res.NumberH = HIBYTE(responsor.m_Index);
	res.NumberL = LOBYTE(responsor.m_Index);
	memcpy(res.szName, responsor.Name, MAX_ACCOUNT_LEN);
	IOCP.DataSend(requester.m_Index, (LPBYTE)&res, res.h.size);
	
	BroadcastScore(nId, 1);
	MoveGate(requester.m_Index, g_GateRequester[nId]);
	MoveGate(responsor.m_Index, g_GateResponsor[nId]);

	TNotice::MakeNoticeMsgEx(&pNotice, 0, Lang.GetText(0,309), responsor.Name, requester.Name, nId+1);
	TNotice::SendNoticeToAllUser(&pNotice);

	g_Log.Add("[NewPVP] [%s][%s] Duel Started [%s][%s]",responsor.AccountID,responsor.Name,requester.AccountID, requester.Name);
	return 0;
}

void CNewPVP::Cancel(OBJECTSTRUCT &requester, OBJECTSTRUCT &responsor, BOOL bSendPacket)
{
	EnterCriticalSection(&this->m_csWaiting);
	std::map<int, _tagWaiting>::iterator iter = m_Waiting.find(requester.m_Index); 

	if(!(iter!=m_Waiting.end()))
	{ 
		g_Log.Add("%s\t%s\t%s\t%s\t%d","iter!=m_Waiting.end()","0","NULL", __FILE__,  __LINE__);
		LeaveCriticalSection(&this->m_csWaiting);
		return; 
	}

	_tagWaiting & waiting = iter->second;
	waiting.bExpired = 1;
	LeaveCriticalSection(&this->m_csWaiting);

	SetDuelStatus(requester, responsor, DC_IDLE);

	if( bSendPacket == TRUE )
    {
		PMSG_ANS_DUEL_INVITE res = {0};
		res.h.c = 0xC1;
		res.h.size = sizeof(PMSG_ANS_DUEL_INVITE);
		res.h.headcode = 0xAA;
		res.h.subcode = 0x01;
		memcpy(res.szName, responsor.Name, MAX_ACCOUNT_LEN);
		res.nResult = 0x0F;
      
		if( gObjIsConnected(&requester) )	IOCP.DataSend(requester.m_Index, (LPBYTE)&res,res.h.size);
		if( gObjIsConnected(&responsor) )	IOCP.DataSend(responsor.m_Index, (LPBYTE)&res, res.h.size);
    }
}

int CNewPVP::Leave(OBJECTSTRUCT &obj)
{
	int nId = GetDuelChannelId(obj.m_Index);
	if(nId < 0 || nId >= DUEL_CHANNEL_MAX){	g_Log.Add("%s\t%s\t%s\t%s\t%d","nId>=0 && nId<DUEL_CHANNEL_MAX","ENEWPVP::E_INVALID_CHANNELID","NULL",__FILE__, __LINE__); return ENEWPVP::E_INVALID_CHANNELID; }
	if( !IS_START(m_DuelChannel[nId].nStatus ) && m_DuelChannel[nId].nStatus != DC_RESERVEDEND ){ g_Log.Add("%s\t%s\t%s\t%s\t%d","IS_START(m_DuelChannel[nId].nStatus)||m_DuelChannel[nId].nStatus==DC_RESERVEDEND", "ENEWPVP::E_INVALID_STATUS","NULL",__FILE__, __LINE__); return ENEWPVP::E_INVALID_STATUS; }	

	LPOBJECTSTRUCT lpTargetObj = NULL;

	obj.m_iDuelUserReserved = -1;

	if( IsDuel(obj) )	lpTargetObj = (LPOBJECTSTRUCT)&gObj[obj.m_iDuelUser];

	if(lpTargetObj == NULL){ g_Log.Add("%s\t%s\t%s\t%s\t%d","lpTargetObj","ENEWPVP::E_INVALID_INDEX","NULL", __FILE__, __LINE__); return ENEWPVP::E_INVALID_INDEX; }

	if( m_DuelChannel[nId].nStatus != DC_RESERVEDEND )
    {
        SetStatus(DC_RESERVEDEND, nId, obj, *lpTargetObj);
        BroadcastMessage( nId, 1, 1, 10);
    }
	return 0;
}

void CNewPVP::Leave(OBJECTSTRUCT &requester, OBJECTSTRUCT &responsor)
{
	int nId = GetDuelChannelId(requester.m_Index);
	if(nId < 0 || nId >= DUEL_CHANNEL_MAX){ g_Log.Add("%s\t%s\t%s\t%s\t%d","nId>=0 && nId<DUEL_CHANNEL_MAX","0","NULL",__FILE__, __LINE__); return; }
	if( !IS_START(m_DuelChannel[nId].nStatus) && m_DuelChannel[nId].nStatus != DC_RESERVEDEND ){ g_Log.Add("%s\t%s\t%s\t%s\t%d","IS_START(m_DuelChannel[nId].nStatus)||m_DuelChannel[nId].nStatus==DC_RESERVEDEND", "0","NULL",__FILE__, __LINE__); return; }	

	PMSG_ANS_DUEL_EXIT res = {0};
	res.h.c = 0xC1;
    res.h.headcode = 0xAA;
    res.h.subcode = 0x03;
    res.h.size = sizeof(PMSG_ANS_DUEL_EXIT);
    res.nResult = 0x00;

	if( gObjIsConnected(&requester) )
    {
		requester.m_iDuelUser = -1;
        res.NumberH = HIBYTE(responsor.m_Index);
        res.NumberL = LOBYTE(responsor.m_Index);
        memcpy(res.szName, responsor.Name, MAX_ACCOUNT_LEN);

        IOCP.DataSend(requester.m_Index, (LPBYTE)&res, res.h.size);
        
		if( IsPKFieldMap(requester.MapNumber) )
		{
			gObjMoveGate(requester.m_Index, 17);
		}
    }
	
	if( gObjIsConnected(&responsor) )
	{
        responsor.m_iDuelUser = -1;
        res.NumberH = HIBYTE(responsor.m_Index);
        res.NumberL = LOBYTE(responsor.m_Index);
        memcpy(res.szName, requester.Name, MAX_ACCOUNT_LEN);
        
		IOCP.DataSend(responsor.m_Index, (LPBYTE)&res, res.h.size);
        
		if( IsPKFieldMap(responsor.MapNumber) )
		{
			gObjMoveGate(responsor.m_Index, 17);
		}
    }
}

void CNewPVP::SetScore(OBJECTSTRUCT &obj)
{
	int nId = GetDuelChannelId(obj.m_Index);
	if(nId < 0 || nId >= DUEL_CHANNEL_MAX) { g_Log.Add("%s\t%s\t%s\t%s\t%d","nId>=0 && nId<DUEL_CHANNEL_MAX","0","NULL",__FILE__, __LINE__); return;	}
	if( !IS_START(m_DuelChannel[nId].nStatus) ) { g_Log.Add("%s\t%s\t%s\t%s\t%d","IS_START(m_DuelChannel[nId].nStatus)", "0","NULL",__FILE__, __LINE__); return; }
    obj.m_btDuelScore++;
}

void CNewPVP::CheckScore(OBJECTSTRUCT& obj, OBJECTSTRUCT& target)
{
	int nId = GetDuelChannelId(obj.m_Index);
	if(nId < 0 || nId >= DUEL_CHANNEL_MAX){ g_Log.Add("%s\t%s\t%s\t%s\t%d","nId>=0 && nId<DUEL_CHANNEL_MAX","0","NULL",__FILE__, __LINE__); return; }
	if( !IS_START(m_DuelChannel[nId].nStatus) ) { g_Log.Add("%s\t%s\t%s\t%s\t%d","IS_START(m_DuelChannel[nId].nStatus)", "0","NULL",__FILE__, __LINE__); return; }

	LPDUEL_CHANNEL lpChannel = &m_DuelChannel[nId];

	OBJECTSTRUCT & requester = gObj[lpChannel->nIndex1];
	OBJECTSTRUCT & responsor = gObj[lpChannel->nIndex2];

	if( obj.m_btDuelScore >= 10 )
    {
		PMSG_NOTICE pNotice;
		BroadcastDuelInfo( nId, 2);
        BroadcastScore( nId, 3);
        SetStatus(DC_RESERVEDEND, nId, requester, responsor);
        BroadcastResult( nId, 3, obj);
        
		gObjAddBuffEffect(&obj, BUFFTYPE_GLORYOFGLADIATOR);

		gDarkSpirit[requester.m_Index-g_ConfigRead.server.GetObjectStartUserIndex()].ReSetTarget(responsor.m_Index);
        gDarkSpirit[responsor.m_Index-g_ConfigRead.server.GetObjectStartUserIndex()].ReSetTarget(requester.m_Index);
		g_CMuunSystem.ReSetTarget(&requester, responsor.m_Index);
		g_CMuunSystem.ReSetTarget(&responsor, requester.m_Index);
        obj.WinDuels++;
		target.LoseDuels++;

		TNotice::MakeNoticeMsgEx(&pNotice, 0, Lang.GetText(0,310), obj.Name, target.Name);
		TNotice::SendNoticeToAllUser(&pNotice);

		g_Log.Add("[NewPVP] [%s][%s] Win Duel, Loser [%s][%s]", obj.AccountID, obj.Name, target.AccountID, target.Name);
		return;
	}

	BroadcastScore(nId, 3);
    SetStatus(4, nId, requester, responsor);
}

void CNewPVP::ReFill(OBJECTSTRUCT &obj)
{
	if( !gObjIsConnected(&obj) ){ g_Log.Add("%s\t%s\t%s\t%s\t%d","gObjIsConnected(&obj)","0","NULL", __FILE__, __LINE__); return; }

	obj.BP = obj.MaxBP + obj.AddBP;
	obj.Mana = obj.MaxMana + obj.AddMana;
	GSProtocol.GCManaSend(obj.m_Index, (WORD)obj.Mana, 0xFF, 0, (WORD)obj.BP);

	obj.Life = obj.MaxLife + obj.AddLife;
	obj.iShield = obj.iMaxShield + obj.iAddShield;
	GSProtocol.GCReFillSend(obj.m_Index, obj.Life, 0xFF, 0, obj.iShield);
}

BOOL CNewPVP::MoveGate(int nIndex, int nGateNum)
{
	BYTE btMapNum = MAP_INDEX_VULCANROOM;
	BYTE btDir;
	short sX;
	short sY;
	short sLevel;
	int nMapNumber;

	nMapNumber =  gGateC.GetGate(nGateNum, sX, sY, btMapNum, btDir, sLevel);
	if( nMapNumber < 0)	return FALSE;

	gObjTeleport(nIndex, btMapNum, sX, sY);
	gObjViewportListProtocolCreate(&gObj[nIndex]);
	return TRUE;
}

BOOL CNewPVP::CheckLimitLevel(int nIndex, int nGateNum)
{
	BYTE btMapNum = MAP_INDEX_VULCANROOM;
	BYTE btDir;
	short sX;
	short sY;
	short sLevel;

	int nMapNumber =  gGateC.GetGate(nGateNum, sX, sY, btMapNum, btDir, sLevel);
	if( nMapNumber < 0)	return FALSE;

	if(gObj[nIndex].Level < sLevel)	return FALSE;
	return TRUE;
}

void CNewPVP::SetBuff(OBJECTSTRUCT &obj, BOOL bUse)
{
	if( bUse )
	{
		gObjAddBuffEffect(&obj, BUFFTYPE_INVISABLE, 0, 0, 0, 0, -10);
		gObjAddBuffEffect(&obj, BUFFTYPE_OBSERVER, 0, 0, 0, 0, -10);
		gObjViewportListProtocolDestroy(&obj);
		return;
	}

	gObjRemoveBuffEffect(&obj, BUFFTYPE_INVISABLE);
	gObjRemoveBuffEffect(&obj, BUFFTYPE_OBSERVER);
	gObjViewportListProtocolCreate(&obj);
}

BOOL CNewPVP::SendChannelList(OBJECTSTRUCT &npc, OBJECTSTRUCT &obj)
{
	IOCP.DataSend(obj.m_Index, (LPBYTE)&m_DuelChannelList, m_DuelChannelList.h.size);
	return FALSE;
}

int CNewPVP::GetChannelStatus(OBJECTSTRUCT &obj)
{
	DUEL_CHANNEL * lpChannel = GetDuelChannel(obj.Name);
	if(lpChannel != NULL)	return lpChannel->nStatus;
	return 0;
}

BOOL CNewPVP::IsSafeState(OBJECTSTRUCT &obj)
{
	DUEL_CHANNEL * lpChannel = GetDuelChannel(obj.Name);
	if(lpChannel == NULL)	return TRUE;
	
	if( lpChannel->nStatus == 2 || 
		lpChannel->nStatus == 4 || 
		lpChannel->nStatus == DC_RESERVEDEND )
	{
		return TRUE;
	}

	return FALSE;
}

int CNewPVP::IsObserver(OBJECTSTRUCT &obj)
{
	return gObjCheckUsedBuffEffect(&obj, BUFFTYPE_OBSERVER);
}

int CNewPVP::GetChannelIdByObserver(OBJECTSTRUCT & obj)
{
	EnterCriticalSection(&this->m_csObserver);

	std::map<int, _tagObserverInfo>::iterator iter = m_ObserverInfoList.find(obj.m_Index);
	if(!(iter!=m_ObserverInfoList.end()))
	{
		LeaveCriticalSection(&this->m_csObserver);
		return -1;
	}

	_tagObserverInfo & info = iter->second;

	if( info.nId < 0 || info.nId >= DUEL_CHANNEL_MAX)
    {
		g_Log.Add("%s\t%s\t%s\t%s\t%d","info.nId>=0 && info.nId<DUEL_CHANNEL_MAX","-1","NULL",__FILE__, __LINE__);
		LeaveCriticalSection(&this->m_csObserver);
		return -1;
    }

	int nId = info.nId;

	LeaveCriticalSection(&this->m_csObserver);
	return nId;
}

int CNewPVP::CheckUsersOnConnect(int nId)
{  
	if(nId < 0 || nId >= DUEL_CHANNEL_MAX){ g_Log.Add("%s\t%s\t%s\t%s\t%d","nId>=0 && nId<DUEL_CHANNEL_MAX","FALSE","NULL",__FILE__, __LINE__); return FALSE; }
	if( !IS_START(m_DuelChannel[nId].nStatus) ){ g_Log.Add("%s\t%s\t%s\t%s\t%d","IS_START(m_DuelChannel[nId].nStatus)", "FALSE","NULL",__FILE__, __LINE__); return FALSE; }

	OBJECTSTRUCT & requester = gObj[m_DuelChannel[nId].nIndex1];
	OBJECTSTRUCT & responsor = gObj[m_DuelChannel[nId].nIndex2];

	if( !gObjIsConnected(requester.m_Index) || !IsPKFieldMap( requester.MapNumber) ){ Leave(requester); return FALSE; }
	if( !gObjIsConnected(responsor.m_Index) || !IsPKFieldMap( responsor.MapNumber) ){ Leave(responsor); return FALSE; }
	return TRUE;
}

void CNewPVP::ChatMsgSend(OBJECTSTRUCT& obj,BYTE* Msg, int size)
{
	int nId = -1;

	if( IsDuel(obj) == TRUE )
	{
		nId = GetDuelChannelId(obj.m_Index);
	}

	else nId = GetChannelIdByObserver(obj);
	if(nId < 0 || nId >= DUEL_CHANNEL_MAX){ g_Log.Add("%s\t%s\t%s\t%s\t%d","nId>=0 && nId<DUEL_CHANNEL_MAX","0","NULL",__FILE__, __LINE__); return; }
	if(m_DuelChannel[nId].nStatus == DC_IDLE){ g_Log.Add("%s\t%s\t%s\t%s\t%d","m_DuelChannel[nId].nStatus!=DC_IDLE","0","NULL",__FILE__, __LINE__); return; }

	LPDUEL_CHANNEL lpChannel = &m_DuelChannel[nId];

	OBJECTSTRUCT& requester = gObj[lpChannel->nIndex1];
    OBJECTSTRUCT& responsor = gObj[lpChannel->nIndex2];

	if(gObjIsConnected(&requester))
	{
		IOCP.DataSend(requester.m_Index, Msg, size);
	}

	if(gObjIsConnected(&responsor))
	{
		IOCP.DataSend(responsor.m_Index, Msg, size);
	}

	BroadcastToObserver(nId, Msg, size);
}

void CNewPVP::Reset(OBJECTSTRUCT& obj)
{
	if( IsDuelRequested(obj) )	Cancel(obj, gObj[obj.m_iDuelUserRequested], TRUE);
	if( IsDuel(obj) )	Leave(obj);

	if( IsObserver(obj) )
	{
		EnterCriticalSection(&this->m_csObserver);
		std::map<int, _tagObserverInfo>::iterator iter = m_ObserverInfoList.find(obj.m_Index);

		if(!(iter!=m_ObserverInfoList.end()))
		{
			LeaveCriticalSection(&this->m_csObserver);
			return;
		}

		ObserverInfo & info = iter->second;
		int nId = info.nId;

		LeaveCriticalSection(&this->m_csObserver);
		LeaveChannel(nId, obj);
	}
}

void CNewPVP::GetObserverList(int nId, PMSG_DUEL_OBSERVERLIST_BROADCAST& res)
{
	int i = 0;
	EnterCriticalSection(&this->m_csObserver);

	for(std::map<int, _tagObserverInfo>::iterator iter = m_ObserverInfoList.begin();iter!=m_ObserverInfoList.end(); iter++)
	{
		ObserverInfo & info = iter->second;		
		if(info.nId == nId)
		{
			memcpy(&res.user[i], info.szName, MAX_ACCOUNT_LEN);
			i++;
			if( i >= 10 )	break;
		}
	}

	res.nCount = i;
	LeaveCriticalSection(&this->m_csObserver);
}

int CNewPVP::JoinChannel(int nId,OBJECTSTRUCT& obj)
{
	if( IsDuel(obj) ){ g_Log.Add("%s\t%s\t%s\t%s\t%d","!IsDuel(obj)","ENEWPVP::E_ALREADY_DUELLING", "NULL", __FILE__,  __LINE__); return ENEWPVP::E_ALREADY_DUELLING; }
	if( IsDuelRequested(obj) ){ g_Log.Add("%s\t%s\t%s\t%s\t%d","!IsDuelRequested(obj)","ENEWPVP::E_ALREADY_DUELREQUESTED","NULL",  __FILE__,  __LINE__); return ENEWPVP::E_ALREADY_DUELREQUESTED; }
	if( IsDuelReserved(obj) ){ g_Log.Add("%s\t%s\t%s\t%s\t%d","!IsDuelReserved(obj)","ENEWPVP::E_ALREADY_DUELRESERVED","NULL", __FILE__,  __LINE__); return ENEWPVP::E_ALREADY_DUELRESERVED; }
	if(nId < 0 || nId >= DUEL_CHANNEL_MAX){ g_Log.Add("%s\t%s\t%s\t%s\t%d","nId>=0 && nId<DUEL_CHANNEL_MAX","ENEWPVP::E_INVALID_CHANNELID","NULL",__FILE__, __LINE__); return ENEWPVP::E_INVALID_CHANNELID; }
	if( !IS_START(m_DuelChannel[nId].nStatus) ){ g_Log.Add("%s\t%s\t%s\t%s\t%d","IS_START(m_DuelChannel[nId].nStatus)", "ENEWPVP::E_INVALID_STATUS","NULL",__FILE__, __LINE__); return ENEWPVP::E_INVALID_STATUS; }
	if( m_ObserverCount[nId] + 1 > 10 )	return ENEWPVP::E_OBSERVER_MAX;
	if( !CheckLimitLevel(obj.m_Index, g_GateObserver[nId]) )	return ENEWPVP::E_LIMIT_LEVEL;

	EnterCriticalSection(&this->m_csObserver);
	std::map<int, _tagObserverInfo>::iterator iter = m_ObserverInfoList.find(obj.m_Index); 

	if( !(iter==m_ObserverInfoList.end()) )
	{
		LeaveCriticalSection(&this->m_csObserver);
		return ENEWPVP::E_FAILED_ENTER;
	}

	LeaveCriticalSection(&this->m_csObserver);

	OBJECTSTRUCT & requester = gObj[m_DuelChannel[nId].nIndex1];
    OBJECTSTRUCT & responsor = gObj[m_DuelChannel[nId].nIndex2];
	
	BroadcastJoinChannel(nId, obj);

	ObserverInfo info = {0};
	info.nId = nId;
    info.nIndex = obj.m_Index;
	memcpy(info.szName, obj.Name, MAX_ACCOUNT_LEN);

	EnterCriticalSection(&this->m_csObserver);
	std::pair< std::map<int,_tagObserverInfo>::iterator, bool > pair = m_ObserverInfoList.insert( std::make_pair(obj.m_Index,  info) );

	if(pair.second == false)
	{
		LeaveCriticalSection(&this->m_csObserver);
		return ENEWPVP::E_FAILED_ENTER;
	}

	m_ObserverCount[nId]++;
	LeaveCriticalSection(&this->m_csObserver);

	PMSG_ANS_DUEL_JOINCNANNEL res = {0};
	res.h.c = 0xC1;
    res.h.headcode = 0xAA;
    res.h.subcode = 0x07;
    res.h.size = sizeof(PMSG_ANS_DUEL_JOINCNANNEL);
	res.nResult = 0x00;
    res.nChannelId = nId;
	res.NumberH1 = HIBYTE(requester.m_Index);
    res.NumberL1 = LOBYTE(requester.m_Index);
    res.NumberH2 = HIBYTE(responsor.m_Index);
    res.NumberL2 = LOBYTE(responsor.m_Index);
    memcpy(res.szName1, requester.Name, MAX_ACCOUNT_LEN);
    memcpy(res.szName2, responsor.Name, MAX_ACCOUNT_LEN);
    IOCP.DataSend(obj.m_Index, (LPBYTE)&res,res.h.size);

	PMSG_DUEL_OBSERVERLIST_BROADCAST Msg = {0};
	Msg.h.c = 0xC1;
	Msg.h.headcode = 0xAA;
	Msg.h.subcode = 0x0B;
	Msg.h.size = sizeof(PMSG_DUEL_OBSERVERLIST_BROADCAST);

	GetObserverList(nId, Msg);

	IOCP.DataSend(obj.m_Index, (LPBYTE)&Msg, Msg.h.size);

	if( Msg.nCount!=m_ObserverCount[nId] )
    {
		g_Log.Add("[NewPVP] ChannelId[%d] Count[%d][%d]", nId, Msg.nCount, m_ObserverCount[nId]);
		
		g_Log.AddC(TColor::Yellow, "[NewPVP] [Debug] %s(%d):",obj.Name,Msg.nCount);
		for(int n = 0; n < Msg.nCount; n++)
		{
			g_Log.AddC(TColor::Yellow, "[NewPVP] [Debug] %s,",Msg.user[n].szName);
		}
	}

	if (obj.m_PlayerData->m_MuBotEnable == true)
	{
		obj.m_PlayerData->m_MuBotEnable = false;
		GSProtocol.GCAnsMuBotUse(obj.m_Index, 1, obj.m_PlayerData->m_MuBotTotalTime, 0);
	}
	
	CheckChannelListInfo( nId);
    BroadcastDuelInfo(nId, 2);
    BroadcastScore(nId, 2);
    SetBuff(obj, TRUE);
    MoveGate(obj.m_Index, g_GateObserver[nId]);
	return 0;
}

int CNewPVP::LeaveChannel(int nId,OBJECTSTRUCT& obj)
{
	if(nId < 0 || nId >= DUEL_CHANNEL_MAX){ g_Log.Add("%s\t%s\t%s\t%s\t%d","nId>=0 && nId<DUEL_CHANNEL_MAX","ENEWPVP::E_INVALID_CHANNELID","NULL",__FILE__, __LINE__); return ENEWPVP::E_INVALID_CHANNELID; }
	if( !IS_START(m_DuelChannel[nId].nStatus) ){ g_Log.Add("%s\t%s\t%s\t%s\t%d","IS_START(m_DuelChannel[nId].nStatus)", "ENEWPVP::E_INVALID_STATUS","NULL",__FILE__, __LINE__); return ENEWPVP::E_INVALID_STATUS; }	

	EnterCriticalSection(&this->m_csObserver);
	std::map<int, _tagObserverInfo>::iterator iter = m_ObserverInfoList.find(obj.m_Index); 

	if(!(iter!=m_ObserverInfoList.end()))
	{
		LeaveCriticalSection(&this->m_csObserver);
		return ENEWPVP::E_NOT_EXIST_USER;
	}

	m_ObserverInfoList.erase(iter);
	m_ObserverCount[nId]--;
	LeaveCriticalSection(&this->m_csObserver);

	BroadcastLeaveChannel( nId, obj);

	if( gObjIsConnected(&obj) )
    {
		PMSG_ANS_DUEL_LEAVECNANNEL res = {0};
		res.h.c = 0xC1;
		res.h.headcode = 0xAA;
		res.h.subcode = 0x09;
		res.h.size = sizeof(PMSG_ANS_DUEL_LEAVECNANNEL);
		res.nResult = 0x00;		
		IOCP.DataSend(obj.m_Index, (LPBYTE)&res, res.h.size);
          
		if( IsPKFieldMap(obj.MapNumber) )
		{
			if (obj.Level < gGateC.GetLevel(294))
			{
				if (obj.Class == CLASS_ELF)
				{
					MoveGate(obj.m_Index, 27);
				}

				else if(obj.Class == CLASS_SUMMONER)
				{
					MoveGate(obj.m_Index, 267);
				}

				else
				{
					MoveGate(obj.m_Index, 17);
				}
			}

			else
			{
				MoveGate(obj.m_Index, 294);
			}
		}

		SetBuff(obj, FALSE);
    }

	CheckChannelListInfo( nId);
	return 0;
}

void CNewPVP::LeaveChannelObserver(int nId)
{
	EnterCriticalSection(&this->m_csObserver);
	std::map<int, _tagObserverInfo>::iterator iter = m_ObserverInfoList.begin();

	while(iter!=m_ObserverInfoList.end())
	{
		ObserverInfo & info = iter->second;		
		if(info.nId == nId)
		{
			LPOBJECTSTRUCT lpObj = (LPOBJECTSTRUCT)&gObj[info.nIndex];

			if( IsPKFieldMap(lpObj->MapNumber) )
			{
				if (lpObj->Level < gGateC.GetLevel(294))
				{
					if (lpObj->Class == CLASS_ELF)
					{
						MoveGate(lpObj->m_Index, 27);
					}

					else if(lpObj->Class == CLASS_SUMMONER)
					{
						MoveGate(lpObj->m_Index, 267);
					}

					else
					{
						MoveGate(lpObj->m_Index, 17);
					}
				}

				else
				{
					MoveGate(lpObj->m_Index, 294);
				}
			}
			
			SetBuff(*lpObj, FALSE);
			iter = m_ObserverInfoList.erase(iter);
			continue;
		}		
		iter++;
	}

	m_ObserverCount[nId] = 0;
	LeaveCriticalSection(&this->m_csObserver);
}

void CNewPVP::CheckChannelListInfo(int nId)
{
	if( nId < 0 || nId >= DUEL_CHANNEL_MAX )	return;	
	m_DuelChannelList.channel[nId].bWatch = 1;
}

void CNewPVP::BroadcastScore(int nId, BYTE nFlag)
{
	if( nId < 0 || nId >= DUEL_CHANNEL_MAX ){ g_Log.Add("%s\t%s\t%s\t%s\t%d","nId>=0 && nId<DUEL_CHANNEL_MAX","0","NULL",__FILE__, __LINE__); return; }
	if( !IS_START(m_DuelChannel[nId].nStatus) ){ g_Log.Add("%s\t%s\t%s\t%s\t%d","IS_START(m_DuelChannel[nId].nStatus)", "0","NULL",__FILE__, __LINE__); return; }

	OBJECTSTRUCT & requester = gObj[m_DuelChannel[nId].nIndex1];
    OBJECTSTRUCT & responsor = gObj[m_DuelChannel[nId].nIndex2];

	PMSG_DUEL_SCORE_BROADCAST res = {0};
	res.h.c = 0xC1;
	res.h.headcode = 0xAA;
	res.h.subcode = 0x04;
	res.h.size = sizeof(PMSG_DUEL_SCORE_BROADCAST);
	res.NumberH1 = HIBYTE(requester.m_Index);
    res.NumberL1 = LOBYTE(requester.m_Index);
    res.NumberH2 = HIBYTE(responsor.m_Index);
    res.NumberL2 = LOBYTE(responsor.m_Index);
    res.btDuelScore1 = requester.m_btDuelScore;
    res.btDuelScore2 = responsor.m_btDuelScore;
      
	if( (nFlag & 1) == 1 )
    {
		IOCP.DataSend(requester.m_Index, (LPBYTE)&res, res.h.size);
        IOCP.DataSend(responsor.m_Index, (LPBYTE)&res, res.h.size);
    }
	if( (nFlag & 2) == 2 )
	{
        BroadcastToObserver(nId, (LPBYTE)&res, res.h.size);
	}
}

void CNewPVP::BroadcastResult(int nId, BYTE nFlag, OBJECTSTRUCT &obj)
{
	if( nId < 0 || nId >= DUEL_CHANNEL_MAX ){ g_Log.Add("%s\t%s\t%s\t%s\t%d","nId>=0 && nId<DUEL_CHANNEL_MAX","0","NULL",__FILE__, __LINE__); return; }
	if( m_DuelChannel[nId].nStatus != DC_RESERVEDEND ){ g_Log.Add("%s\t%s\t%s\t%s\t%d","m_DuelChannel[nId].nStatus==DC_RESERVEDEND", "0","NULL",__FILE__, __LINE__); return;	}
	if( !gObjIsConnected(obj.m_Index) ){ g_Log.Add("%s\t%s\t%s\t%s\t%d","gObjIsConnected(obj.m_Index)", "0","NULL",__FILE__, __LINE__); return; }
	if( !IsDuel(obj) ){ g_Log.Add("%s\t%s\t%s\t%s\t%d","IsDuel(obj)", "0","NULL",__FILE__, __LINE__); return; }

	OBJECTSTRUCT & targetObj = gObj[obj.m_iDuelUser];
     
	PMSG_DUEL_RESULT_BROADCAST res = {0};
	res.h.c = 0xC1;
	res.h.headcode = 0xAA;
	res.h.subcode = 0x0C;
	res.h.size = sizeof(PMSG_DUEL_RESULT_BROADCAST);
	memcpy(res.szWinner, obj.Name, MAX_ACCOUNT_LEN);
	memcpy(res.szLoser, targetObj.Name, MAX_ACCOUNT_LEN);
	
	if( (nFlag & 1) == 1 )
	{
		IOCP.DataSend(obj.m_Index, (LPBYTE)&res, res.h.size);
		IOCP.DataSend(targetObj.m_Index, (LPBYTE)&res, res.h.size);
    }
	if( (nFlag & 2) == 2 )
	{
		BroadcastToObserver(nId, (LPBYTE)&res,res.h.size);
	}
}

void CNewPVP::BroadcastDuelInfo(int nId, BYTE nFlag)
{
	if( nId < 0 || nId >= DUEL_CHANNEL_MAX ){ g_Log.Add("%s\t%s\t%s\t%s\t%d","nId>=0 && nId<DUEL_CHANNEL_MAX","0","NULL",__FILE__, __LINE__); return; }
	if( !IS_START(m_DuelChannel[nId].nStatus) ) return;

	OBJECTSTRUCT & requester = gObj[m_DuelChannel[nId].nIndex1];
    OBJECTSTRUCT & responsor = gObj[m_DuelChannel[nId].nIndex2];
	
	int nLifePer1 = 0;
    int nLifePer2 = 0;
    int ShieldPer1 = 0;
    int ShieldPer2 = 0;

	if(requester.AddLife +  requester.MaxLife <  requester.Life)
	{
		 requester.Life =  requester.AddLife +  requester.MaxLife;
	}

	if(requester.iShield > requester.iMaxShield + requester.iAddShield)
	{
		requester.iShield = requester.iMaxShield + requester.iAddShield;
	}

	if(responsor.AddLife +  responsor.MaxLife <  responsor.Life)
	{
		 responsor.Life =  responsor.AddLife +  responsor.MaxLife;
	}

	if(responsor.iShield > responsor.iMaxShield + responsor.iAddShield)
	{
		responsor.iShield = responsor.iMaxShield + responsor.iAddShield;
	}
	
	nLifePer1 = (requester.Life / ((float)(requester.AddLife) + requester.MaxLife) * 100.0f);
	nLifePer2 = (responsor.Life / ((float)(responsor.AddLife) + responsor.MaxLife) * 100.0f);
	ShieldPer1 = ((float)(requester.iShield) / (float)((int)(requester.iMaxShield) + (int)(requester.iAddShield)) * 100.0f);
	ShieldPer2 = ((float)(responsor.iShield) / (float)((int)(responsor.iMaxShield) + (int)(responsor.iAddShield)) * 100.0f);

	PMSG_DUEL_HP_BROADCAST res = {0};
	res.h.c = 0xC1;
	res.h.headcode = 0xAA;
	res.h.subcode = 0x05;
	res.h.size = sizeof(PMSG_DUEL_HP_BROADCAST);
	res.NumberH1 = HIBYTE(requester.m_Index);
    res.NumberL1 = LOBYTE(requester.m_Index);
	res.NumberH2 = HIBYTE(responsor.m_Index);
    res.NumberL2 = LOBYTE(responsor.m_Index);
	res.nHP1 = nLifePer1;
	res.nHP2 = nLifePer2;
	res.nShield1 = ShieldPer1;
	res.nShield2 = ShieldPer2;
	
	if( (nFlag & 1) == 1 )
	{
		IOCP.DataSend(requester.m_Index, (LPBYTE)&res, res.h.size);
		IOCP.DataSend(responsor.m_Index, (LPBYTE)&res, res.h.size);
	}
	if( (nFlag & 2) == 2 )
	{
		BroadcastToObserver(nId, (LPBYTE)&res, res.h.size);
	}
}

void CNewPVP::BroadcastJoinChannel(int nId, OBJECTSTRUCT &obj)
{
	if( nId < 0 || nId >= DUEL_CHANNEL_MAX ){ g_Log.Add("%s\t%s\t%s\t%s\t%d","nId>=0 && nId<DUEL_CHANNEL_MAX","0","NULL",__FILE__, __LINE__); return; }
	if( !IS_START(m_DuelChannel[nId].nStatus) ){ g_Log.Add("%s\t%s\t%s\t%s\t%d","IS_START(m_DuelChannel[nId].nStatus)","0","NULL",__FILE__, __LINE__); return; }

	PMSG_DUEL_JOINCNANNEL_BROADCAST res = {0};
	res.h.c = 0xC1;
    res.h.headcode = 0xAA;
    res.h.subcode = 0x08;
	res.h.size = sizeof(PMSG_DUEL_JOINCNANNEL_BROADCAST);
    memcpy(res.szName, obj.Name, MAX_ACCOUNT_LEN);
    BroadcastToObserver(nId, (LPBYTE)&res, res.h.size);
}

void CNewPVP::BroadcastLeaveChannel(int nId, OBJECTSTRUCT &obj)
{
	if( nId < 0 || nId >= DUEL_CHANNEL_MAX ){ g_Log.Add("%s\t%s\t%s\t%s\t%d","nId>=0 && nId<DUEL_CHANNEL_MAX","0","NULL",__FILE__, __LINE__); return; }
	if( !IS_START(m_DuelChannel[nId].nStatus) ){ g_Log.Add("%s\t%s\t%s\t%s\t%d","IS_START(m_DuelChannel[nId].nStatus)","0","NULL",__FILE__, __LINE__); return; }

	PMSG_DUEL_LEAVECNANNEL_BROADCAST res = {0};
	res.h.c = 0xC1;
    res.h.headcode = 0xAA;
    res.h.subcode = 0x0A;
	res.h.size = sizeof(PMSG_DUEL_LEAVECNANNEL_BROADCAST);
    memcpy(res.szName, obj.Name, MAX_ACCOUNT_LEN);
    BroadcastToObserver(nId, (LPBYTE)&res,res.h.size);
}

void CNewPVP::BroadcastToObserver( int nId, BYTE *lpData, int nSize)
{
	int nSendCount = 0;
	int SendIndex[10];
	memset(SendIndex, 0x00, sizeof(SendIndex));

	EnterCriticalSection(&this->m_csObserver);

	for(std::map<int, _tagObserverInfo>::iterator iter = m_ObserverInfoList.begin();iter!=m_ObserverInfoList.end(); iter++)
	{
		ObserverInfo & info = iter->second;		
		if(info.nId == nId)
		{
			if( gObjIsConnected(info.nIndex) )
			{
				SendIndex[nSendCount] = info.nIndex;
				nSendCount++;
			}
		}
	}

	LeaveCriticalSection(&this->m_csObserver);

	for (int i = 0; i < nSendCount; i++)
	{
		IOCP.DataSend(SendIndex[i], lpData, nSize);
	}
}

void CNewPVP::BroadcastMessage( int nId, BYTE nFlag, BYTE nMsgType, int nNotifySec)
{
	if( nId < 0 || nId >= DUEL_CHANNEL_MAX ){ g_Log.Add("%s\t%s\t%s\t%s\t%d","nId>=0 && nId<DUEL_CHANNEL_MAX","0","NULL",__FILE__, __LINE__); return; }
	if( nNotifySec <= 0 )	return;

	OBJECTSTRUCT &requester = gObj[m_DuelChannel[nId].nIndex1];
    OBJECTSTRUCT &responsor = gObj[m_DuelChannel[nId].nIndex2];

	PMSG_NOTICE res = {0} ;

	switch(nMsgType)
	{
	case 0:	TNotice::MakeNoticeMsgEx(&res, 1, Lang.GetText(0,323), nNotifySec);	break;
	case 1:	TNotice::MakeNoticeMsgEx(&res, 1, Lang.GetText(0,324), nNotifySec);	break;
	case 2:	TNotice::MakeNoticeMsgEx(&res, 1, Lang.GetText(0,325), nNotifySec);	break;
	default:	return;
	}

	if( (nFlag & 1) == 1 )
    {
		if( gObjIsConnected(&requester) )	IOCP.DataSend(requester.m_Index, (LPBYTE)&res,res.h.size);
		if( gObjIsConnected(&responsor) )	IOCP.DataSend(responsor.m_Index, (LPBYTE)&res,res.h.size);
    }
	if( (nFlag & 2) == 2 )
	{
      BroadcastToObserver(nId, (LPBYTE)&res, res.h.size);
	}
}

void CNewPVP::BroadcastRound(int nId, BYTE nFlag, BOOL bEnd)
{
	if( nId < 0 || nId >= DUEL_CHANNEL_MAX ){ g_Log.Add("%s\t%s\t%s\t%s\t%d","nId>=0 && nId<DUEL_CHANNEL_MAX","0","NULL",__FILE__, __LINE__); return; }

	OBJECTSTRUCT &requester = gObj[m_DuelChannel[nId].nIndex1];
    OBJECTSTRUCT &responsor = gObj[m_DuelChannel[nId].nIndex2];

	PMSG_DUEL_ROUNDSTART_BROADCAST res = {0}; 
    res.h.c = 0xC1;
    res.h.headcode = 0xAA;
    res.h.subcode = 0x0D;
    res.h.size = sizeof(PMSG_DUEL_ROUNDSTART_BROADCAST);
    res.nFlag = bEnd;

    if( (nFlag & 1) == 1 )
    {
		IOCP.DataSend(requester.m_Index,(LPBYTE)&res,res.h.size);
		IOCP.DataSend(responsor.m_Index, (LPBYTE)&res, res.h.size);
    }
    if( (nFlag & 2) == 2 )
	{
		BroadcastToObserver(nId, (LPBYTE)&res,res.h.size);
	}
}

BOOL CNewPVP::CheckPKPenalty(LPOBJECTSTRUCT lpObj)
{
	if( !g_ConfigRead.pk.bPkPenaltyDisable )
	{
		if( lpObj->PartyNumber >= 0 )
		{
			if( gParty.GetPKPartyPenalty( lpObj->PartyNumber) >= 6 )
			{
				return TRUE;
			}
		}

		else if( lpObj->m_PK_Level >= 6 )
		{
			 return TRUE;
		}
    }
	return FALSE;
}

BOOL CNewPVP::IsSelfDefense(LPOBJECTSTRUCT lpObj)
{
	BOOL bRetVal = FALSE;
	for ( int i = 0; i < MAX_SELF_DEFENSE; i++ )
	{
		if( lpObj->SelfDefense[i] >= 0 )
		{
			bRetVal = TRUE;
			break;
		}
	}
	return bRetVal;
}

BOOL CNewPVP::IsGuildWar(LPOBJECTSTRUCT lpObj)
{
	if( lpObj->m_PlayerData->lpGuild && 
		lpObj->m_PlayerData->lpGuild->WarState == 1)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CNewPVP::DropItem(LPOBJECTSTRUCT lpObj, LPOBJECTSTRUCT lpMonsterObj)
{
	if( !IsVulcanusMap(lpObj->MapNumber) ){ g_Log.Add("%s\t%s\t%s\t%s\t%d","IsVulcanusMap(lpObj->MapNumber)","FALSE","NULL",__FILE__,  __LINE__); return FALSE; }
	if( m_bNewPVP != TRUE){ g_Log.Add("%s\t%s\t%s\t%s\t%d","m_bNewPVP==TRUE","FALSE","NULL", __FILE__,  __LINE__); return FALSE; }
	if( !gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_GLORYOFGLADIATOR) )	return FALSE;
	if( !gObjGetTotalValueOfEffect(lpObj, EFFECTTYPE_VULCANUS_ITEMDROPRATE) )	return FALSE;

	if (g_BagManager.SearchAndUseBag(lpObj->m_Index, BAG_EVENT, EVENTBAG_NEWPVP, lpMonsterObj->m_Index) == false)
	{
		return FALSE;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

