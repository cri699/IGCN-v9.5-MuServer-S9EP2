//////////////////////////////////////////////////////////////////////
// RingAttackEvent.cpp
#include "stdafx.h"
#include "RingAttackEvent.h"
#include "gObjMonster.h"
#include "GameMain.h"
#include "DSProtocol.h"
#include "protocol.h"
#include "TLog.h"
#include "winutil.h"
#include "BuffEffectSlot.h"
#include "configread.h"

// GS-N 0.99.60T 0x0045D690
// GS-N	1.00.18	JPN	0x0046CF70	-	Completed

CRingMonsterHerd::CRingMonsterHerd()
{
	return;
}


CRingMonsterHerd::~CRingMonsterHerd()
{
	return;
}

BOOL CRingMonsterHerd::Start()
{
	this->m_bEventSuccess = FALSE;
	return this->MonsterHerd::Start();
}

BOOL CRingMonsterHerd::MonsterHerdItemDrop(LPOBJ lpObj)
{
	
	if ( lpObj->Class == 135 )
	{
		int iIndex = gObjMonsterTopHitDamageUser(lpObj);

		if ( iIndex == -1 )
		{
			int itemnumber = ItemGetNumberMake(14, 13);
			ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, lpObj->X, lpObj->Y,
				itemnumber, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0);
			return TRUE;
		}

		int itemnumber = ItemGetNumberMake(14, 13);
		ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, lpObj->X, lpObj->Y,
			itemnumber, 0, 0, 0, 0, 0, iIndex, 0, 0, 0, 0, 0);
		char szTemp[256];
		wsprintf(szTemp, Lang.GetText(0,101), gObj[iIndex].Name, Lang.GetMap(0, lpObj->MapNumber));	// #error Apply Deathway fix here
		GSProtocol.AllSendServerMsg( szTemp ); 
		g_Log.Add("[Ring Event] White Wizard Killed by [%s][%s], MapNumber:%d",
			gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].MapNumber);

		return TRUE;

	}
	
	if ( lpObj->Class == 136 || lpObj->Class == 137)
	{
		if ( (rand()%100) < 30 )
		{
			int iIndex = gObjMonsterTopHitDamageUser(lpObj);
			int itemnumber = ItemGetNumberMake(13, 20);	// Wizards Ring
			ItemSerialCreateSend(lpObj->m_Index, lpObj->MapNumber, lpObj->X, lpObj->Y,
				itemnumber, 0, 30, 0, 0, 0, iIndex, 0, 0, 0, 0, 0);
			return TRUE;
		}

		MapC[lpObj->MapNumber].MoneyItemDrop(10000, (BYTE)lpObj->X, (BYTE)lpObj->Y);
		return TRUE;
	}

	return FALSE;

}

void CRingMonsterHerd::MonsterAttackAction(LPOBJ lpObj, LPOBJ lpTargetObj)
{
	if ( gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_STONE) || gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_STUN) || gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_SLEEP)
		|| gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_FREEZE_2) || gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_EARTH_BINDS) )
	{
		return;
	}

	if ( lpObj == NULL )
	{
		return;
	}

	if ( lpTargetObj == NULL )
	{
		return;
	}

	if ( lpObj->Connected != PLAYER_PLAYING || lpObj->Type != OBJ_MONSTER )
	{
		return;
	}

	if ( lpTargetObj->Connected != PLAYER_PLAYING )
	{
		return;
	}

	if ( lpObj->Class == 135 )
	{
		this->OrderAttack(lpObj, lpTargetObj, 90 );
	}
}



struct PMSG_REQ_REG_RINGGIFT
{
	PBMSG_HEAD2 h;	// C1:10
	int iINDEX;	// 4
	char szUID[11];	// 8
	BYTE btGiftKind;	// 13
};



void CRingMonsterHerd::SendEventGiftWinner(int iIndex, int iGiftKind)
{
	if ( gObjIsConnected(iIndex) == FALSE )
	{
		return;
	}

	if ( gObj[iIndex].UseEventServer != FALSE )
	{
		return;
	}

	gObj[iIndex].UseEventServer = TRUE;

	PMSG_REQ_REG_RINGGIFT pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0xBE, 0x10, sizeof(pMsg));
	pMsg.iINDEX  = iIndex;
	memcpy(pMsg.szUID, gObj[iIndex].AccountID, MAX_ACCOUNT_LEN);
	pMsg.szUID[MAX_ACCOUNT_LEN] = 0;	// #error Remove the +1 to avoid problems
	pMsg.btGiftKind = iGiftKind;

	wsDataCli.DataSend((PCHAR)&pMsg, sizeof(pMsg));

	g_Log.Add("[Ring Event] [%s][%s] Request to Register Gift - Gift Kind (%d)",
		gObj[iIndex].AccountID, gObj[iIndex].Name,  iGiftKind);

}


//*******************************************************



static int g_RingEventMapNum[MAX_RINGMONSTER_MAP] = { MAP_INDEX_RORENCIA,MAP_INDEX_DEVIAS,MAP_INDEX_NORIA};
static int g_RingMapPosition[MAX_RINGMONSTER_MAP][4] =
{

	66, 44, 191, 197,
	111, 14, 232, 125,
	136, 53, 223, 181
};

static int g_RingMapDestPosition[MAX_RINGMONSTER_MAP][8] = {	133, 79, 87, 126,
														133, 178, 180, 126,
														164, 42, 221, 85,
														164, 42, 221, 85,
														160, 45, 160, 45, 
														152, 117, 209, 133 };

CRingAttackEvent::CRingAttackEvent()
{
	this->m_bDoEvent = FALSE;
	this->m_bHasData = FALSE;
	this->m_iRADIUS_MIN = 3;
	this->m_iRADIUS_MAX = 6;
	this->m_iMOVE_RAND_SIZE = 15;
	InitializeCriticalSection(&this->m_critMonsterAddData);
}

CRingAttackEvent::~CRingAttackEvent()
{
	DeleteCriticalSection(&this->m_critMonsterAddData);
}

void CRingAttackEvent::StartEvent()
{
	int iRandX;
	int iRandY;

	if ( this->m_bHasData == 0 )
	{
		return;
	}

	for ( int i=0;i<MAX_RINGMONSTER_MAP;i++)
	{
		int iCount = 1000;

		while ( iCount-- != 0 )
		{
			iRandX = rand() % (g_RingMapPosition[i][2] - g_RingMapPosition[i][0]) + g_RingMapPosition[i][0];
			iRandY = rand() % (g_RingMapPosition[i][3] - g_RingMapPosition[i][1]) + g_RingMapPosition[i][1];

			if ( this->m_RingMonsterHerd[i].SetTotalInfo(g_RingEventMapNum[i] , 3, iRandX, iRandY ) != FALSE )
			{
				break;
			}
		}

		if ( iCount == 0 )
		{

		}
		else
		{
			if ( this->m_vtMonsterAddData.empty() != false )
			{
				g_Log.Add("[Ring Event] - Error : No Monster Data Exist");
				continue;
			}

			g_Log.Add("[Ring Event] - Monster Start Position MapNumber:%d, X:%d, Y:%d",
				g_RingEventMapNum[i], iRandX, iRandY);
		}

		EnterCriticalSection(&this->m_critMonsterAddData);

		for ( std::vector<RINGMONSTER_DATA>::iterator it = this->m_vtMonsterAddData.begin() ; it != this->m_vtMonsterAddData.end() ; it++)
		{
			RINGMONSTER_DATA & pRMD = *it;
			iCount = 100;

			while ( iCount-- != 0 )
			{
				if ( this->m_RingMonsterHerd[i].AddMonster(pRMD.m_iType, pRMD.m_bDoRegen, pRMD.m_bDoAttackFirst) != FALSE )
				{
					break;
				}
			}
		}

		LeaveCriticalSection(&this->m_critMonsterAddData);
		this->m_RingMonsterHerd[i].Start();
	}
}

void CRingAttackEvent::StopEvent()
{
	for ( int i=0;i<MAX_RINGMONSTER_MAP;i++)
	{
		this->m_RingMonsterHerd[i].Stop();
	}

	this->m_bMonsterToDest = FALSE;
}

void CRingAttackEvent::Init()
{
	if (g_ConfigRead.server.GetStateFromEventTable(g_ConfigRead.server.GetServerType(), EV_TABLE_RING) == false)
	{
		this->SetEventEnable(false);
	}

	if ( this->m_bDoEvent )
	{
		this->SetState(1);
	}

	else
	{
		this->SetState(0);
	}
}

BOOL CRingAttackEvent::Load(char * lpszFileName)
{
	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(lpszFileName);

	if ( res.status != pugi::status_ok )
	{
		g_Log.MsgBox("[Ring Event] Info file Load Fail [%s] [%s]", lpszFileName, res.description());
		return FALSE;
	}

	EnterCriticalSection(&this->m_critMonsterAddData);
	this->m_vtMonsterAddData.erase( this->m_vtMonsterAddData.begin(), this->m_vtMonsterAddData.end() );
	LeaveCriticalSection(&this->m_critMonsterAddData);

	this->m_vtEventTime.erase( this->m_vtEventTime.begin(), this->m_vtEventTime.end() );

	this->m_bHasData = FALSE;

	pugi::xml_node main = file.child("WhiteWizard");

	bool bEnable = main.attribute("Enable").as_bool();

	if (g_ConfigRead.server.GetStateFromEventTable(g_ConfigRead.server.GetServerType(), EV_TABLE_RING) == false)
	{
		bEnable = false;
	}

	this->SetEventEnable(bEnable);

	pugi::xml_node time = main.child("Time");
	
	this->m_iTIME_MIN_OPEN = time.attribute("ToOpen").as_int();
	this->m_iTIME_MIN_PLAY = time.attribute("PlayDuration").as_int();
	this->m_iTIME_MONSTER_TO_DEST = time.attribute("ToClose").as_int();

	pugi::xml_node move = main.child("Move");

	this->m_iMOVE_RAND_SIZE = move.attribute("RandomSize").as_int();
	this->m_iRADIUS_MIN = move.attribute("MinRadius").as_int();
	this->m_iRADIUS_MAX = move.attribute("MaxRadius").as_int();

	pugi::xml_node schedule = main.child("Schedule");

	for (pugi::xml_node start = schedule.child("Start"); start; start = start.next_sibling())
	{
		RINGMONSTER_EVENT_TIME pRET;

		pRET.m_iHour = start.attribute("Hour").as_int();
		pRET.m_iMinute = start.attribute("Minute").as_int();

		this->m_vtEventTime.push_back(pRET);
	}

	pugi::xml_node spawn_settings = main.child("SpawnSettings");

	for (pugi::xml_node monster = spawn_settings.child("Monster"); monster; monster = monster.next_sibling())
	{
		RINGMONSTER_DATA pRMD;

		pRMD.m_iType = monster.attribute("Index").as_int();
		pRMD.m_bDoRegen = monster.attribute("Respawn").as_int();
		pRMD.m_bDoAttackFirst = monster.attribute("AttackFirst").as_int();

		EnterCriticalSection(&this->m_critMonsterAddData);
		this->m_vtMonsterAddData.push_back(pRMD);
		LeaveCriticalSection(&this->m_critMonsterAddData);
	}

	g_Log.Add("%s file load!", lpszFileName);
	this->m_bHasData = TRUE;

	return TRUE;
}

void CRingAttackEvent::Run()
{
	if ( this->m_bMenualStart )
		return;

	if ( this->m_bDoEvent )
	{
		switch ( this->m_iEVENT_STATE )
		{
			case 0:
				this->ProcState_None();
				break;
			case 1:
				this->ProcState_Closed();
				break;
			case 2:
				this->ProcState_Playing();
		}
	}
}	

void CRingAttackEvent::CheckSync()
{
	if ( this->m_vtEventTime.empty() != 0 )
	{
		g_Log.AddC(TColor::Red,  "[Ring Event] No Schedule Time Data");
		this->SetState(0);

		return;
	}

	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);

	int iMIN_HOUR = 24;
	int iMIN_MINUTE = 60;
	BOOL bTIME_CHANGED = FALSE;
	std::vector<RINGMONSTER_EVENT_TIME>::iterator it;
	for ( it = this->m_vtEventTime.begin(); it != this->m_vtEventTime.end() ; it++ )
	{
		RINGMONSTER_EVENT_TIME & pRET = *it;

		if ( (sysTime.wHour * 60 + sysTime.wMinute) < (pRET.m_iHour * 60 + pRET.m_iMinute ))
		{
			if ( (iMIN_HOUR * 60 + iMIN_MINUTE) > (pRET.m_iHour * 60 + pRET.m_iMinute) )
			{
				bTIME_CHANGED = TRUE;
				iMIN_HOUR = pRET.m_iHour;
				iMIN_MINUTE = pRET.m_iMinute;
			}
		}
	}

	if ( bTIME_CHANGED == FALSE )
	{
		it = this->m_vtEventTime.begin();
		iMIN_HOUR = 24;
		iMIN_MINUTE = 60;

		for ( ; it != this->m_vtEventTime.end() ; it++ )
		{
			RINGMONSTER_EVENT_TIME & pRET = *it;

			if ( (iMIN_HOUR * 60 + iMIN_MINUTE) > (pRET.m_iHour * 60 + pRET.m_iMinute) )
			{
				bTIME_CHANGED = 2;
				iMIN_HOUR = pRET.m_iHour;
				iMIN_MINUTE = pRET.m_iMinute;
			}
		}
	}

	switch ( bTIME_CHANGED )
	{
		case 1:
			this->m_iTIME_MSEC_REMAIN = ((iMIN_HOUR * 60 * 60 + iMIN_MINUTE * 60) - (sysTime.wHour * 60 * 60 + sysTime.wMinute * 60 + sysTime.wSecond)) * 1000;
			break;
		case 2:
			this->m_iTIME_MSEC_REMAIN = (((iMIN_HOUR + 24) * 60 * 60 + iMIN_MINUTE * 60) - (sysTime.wHour * 60 * 60 + sysTime.wMinute * 60 + sysTime.wSecond)) * 1000;
			break;
		default:
			g_Log.AddC(TColor::Red,  "[Ring Event] No Schedule Time Data");
			this->SetState(0);
			return;
	}

	this->m_iTIME_TICK_COUNT = GetTickCount();

	g_Log.Add("[Ring Event] Sync Start Time. [%d] min remain (START HOUR:%d, MIN:%d)",
		this->m_iTIME_MSEC_REMAIN/60000, iMIN_HOUR, iMIN_MINUTE);

	this->m_iTIME_NOTIFY_COUNT = this->m_iTIME_MIN_OPEN;
}

void CRingAttackEvent::SetState(int iSTATE)
{
	if ( this->m_iEVENT_STATE < 0 || this->m_iEVENT_STATE > 2 )
		return;

	this->m_iEVENT_STATE = iSTATE;

	switch ( this->m_iEVENT_STATE )
	{
		case 0:
			this->SetState_None();
			break;
		case 1:
			this->SetState_Closed();
			break;
		case 2:
			this->SetState_Playing();
			break;
	}
}

void CRingAttackEvent::ProcState_None()
{
	return;
}

struct PMSG_ANS_CL_EFFECT
{
	PBMSG_HEAD h;	// C1:9E
	WORD wEffectNum;	// 4
};

void CRingAttackEvent::ProcState_Closed()
{
	int iTICK_MSEC = GetTickCount() - this->m_iTIME_TICK_COUNT;

	if ( iTICK_MSEC >= 1000 )
	{
		this->m_iTIME_MSEC_REMAIN -= iTICK_MSEC;
		this->m_iTIME_TICK_COUNT = GetTickCount();

		if ( this->m_bDoEvent )
		{
			if ( this->m_iTIME_MSEC_REMAIN <= (this->m_iTIME_MIN_OPEN * 60 * 1000 ) &&
				 this->m_iTIME_MSEC_REMAIN > 0 &&
				 (this->m_iTIME_MSEC_REMAIN / 60000 ) != this->m_iTIME_NOTIFY_COUNT )
			{
				this->m_iTIME_NOTIFY_COUNT = this->m_iTIME_MSEC_REMAIN / 60000;

				if ( (this->m_iTIME_NOTIFY_COUNT+1) == 3 )
				{
					char szTemp[256];
					wsprintf(szTemp, Lang.GetText(0,102));
					GSProtocol.AllSendServerMsg(szTemp);

					PMSG_ANS_CL_EFFECT pMsg;

					PHeadSetB((LPBYTE)&pMsg, 0x9E, sizeof(pMsg));
					pMsg.wEffectNum = 0;

					GSProtocol.DataSendAll((LPBYTE)&pMsg, pMsg.h.size);

					g_Log.Add("[Ring Event] - Before 3 minutes - Advertise");
				}
			}
		}
	}

	if ( this->m_iTIME_MSEC_REMAIN <= 0 )
	{
		char szTemp[256];
		wsprintf(szTemp, Lang.GetText(0,103));
		GSProtocol.AllSendServerMsg(szTemp);

		PMSG_ANS_CL_EFFECT pMsg;

		PHeadSetB((LPBYTE)&pMsg, 0x9E, sizeof(pMsg));
		pMsg.wEffectNum = 1;

		GSProtocol.DataSendAll((LPBYTE)&pMsg, pMsg.h.size);

		if ( this->m_bDoEvent )
		{
			this->SetState(2);
		}
		else
		{
			this->SetState(0);
		}

		g_Log.Add("[Ring Event] - Event Started");
	}
}

void CRingAttackEvent::ProcState_Playing()
{
	int iTICK_MSEC = GetTickCount() - this->m_iTIME_TICK_COUNT;

	if ( iTICK_MSEC >= 1000 )
	{
		this->m_iTIME_MSEC_REMAIN -= iTICK_MSEC;
		this->m_iTIME_TICK_COUNT = GetTickCount();

		if ( (rand()%10) == 0 )
		{
			this->Move();
		}
	}

	static int s_iTIME_REMAIN = -1;

	if ( ((this->m_iTIME_MSEC_REMAIN / 1000 ) % 30 ) == 0 && ((this->m_iTIME_MSEC_REMAIN / 1000) / 30) != s_iTIME_REMAIN  )
	{
		s_iTIME_REMAIN = this->m_iTIME_MSEC_REMAIN / 1000 / 30;

		for ( int i=0;i<MAX_RINGMONSTER_MAP;i++)
		{
			BOOL bLive = FALSE;

			for ( int n=0;n<g_ConfigRead.server.GetObjectStartUserIndex();n++)
			{
				if ( gObj[n].Class == 135 )
				{
					if ( gObj[n].m_bIsInMonsterHerd )
					{
						if ( gObj[n].Live && gObj[n].MapNumber == g_RingEventMapNum[i] )
						{
							bLive = TRUE;
						}
					}
				}
			}
		}
	}

	if ( this->m_iTIME_MSEC_REMAIN <= (this->m_iTIME_MONSTER_TO_DEST * 60 * 1000) && this->m_bMonsterToDest == FALSE )
	{
		
		for ( int i=0;i<MAX_RINGMONSTER_MAP;i++)
		{
			BYTE iRandX;
			BYTE iRandY;
			BYTE ucRAND;

			ucRAND = rand()%4;
			iRandX = g_RingMapDestPosition[i][ucRAND*2];
			iRandY = g_RingMapDestPosition[i][ucRAND*2+1];
		}

		this->m_bMonsterToDest = TRUE;
	}

	if ( this->m_iTIME_MSEC_REMAIN <= 0 )
	{
		char szTemp[256];
		wsprintf(szTemp, Lang.GetText(0,104));
		GSProtocol.AllSendServerMsg(szTemp);

		PMSG_ANS_CL_EFFECT pMsg;

		PHeadSetB((LPBYTE)&pMsg, 0x9E, sizeof(pMsg));
		pMsg.wEffectNum = 2;

		GSProtocol.DataSendAll((LPBYTE)&pMsg, pMsg.h.size);

		if ( this->m_bDoEvent )
		{
			this->SetState(1);
		}
		else
		{
			this->SetState(0);
		}

		g_Log.Add("[Ring Event] - Event Ended");
	}
}

void CRingAttackEvent::SetState_None()
{
	this->StopEvent();
}

void CRingAttackEvent::SetState_Closed()
{
	this->StopEvent();

	if ( this->m_bDoEvent )
	{
		this->CheckSync();
	}
	else
	{
		this->SetState(0);
	}
}

void CRingAttackEvent::SetState_Playing()
{
	this->m_iTIME_MSEC_REMAIN = this->m_iTIME_MIN_PLAY * 60 * 1000;
	this->StartEvent();
}

void CRingAttackEvent::Move()
{
	if ( this->m_bDoEvent )
	{
		if ( this->m_bMonsterToDest )
			return;

		BYTE iRandX;
		BYTE iRandY;
		BYTE iCX;
		BYTE iCY;

		for ( int i=0;i<MAX_RINGMONSTER_MAP;i++)
		{
			int iCount = 1000;

			if ( this->m_RingMonsterHerd[i].GetCurrentLocation(iCX, iCY) == FALSE )
				return;

			while ( iCount-- != 0 )
			{
				iRandX = (rand() % this->m_iMOVE_RAND_SIZE) * ( (rand() % 3 -1) * -1 ) + iCX;
				iRandY = (rand() % this->m_iMOVE_RAND_SIZE) * ( (rand() % 3 -1) * -1 ) + iCY;

				if ( iRandX < g_RingMapPosition[i][0] || iRandX > g_RingMapPosition[i][2] )
					continue;

				if ( iRandY < g_RingMapPosition[i][1] || iRandY > g_RingMapPosition[i][3] )
					continue;

				if ( this->m_RingMonsterHerd[i].CheckLocation(iRandX, iRandY) != FALSE )
					break;
			}

			this->m_RingMonsterHerd[i].MoveHerd(iRandX, iRandY);

			int iDIVIDE = this->m_iRADIUS_MAX - this->m_iRADIUS_MIN;

			if ( iDIVIDE <= 0 )
				iDIVIDE = 1;

			int iRADIUS = iDIVIDE + this->m_iRADIUS_MIN;

			if ( iRADIUS <= 3 )
				iRADIUS = 3;

			this->m_RingMonsterHerd[i].SetRadius((rand() % (this->m_iRADIUS_MAX - this->m_iRADIUS_MIN)) + this->m_iRADIUS_MIN);
		}
	}
}

void CRingAttackEvent::Start_Menual()
{
	this->SetMenualStart(TRUE);
	this->StopEvent();

	g_Log.Add("[Event Management] [Start] RingAttack Event!");
	this->m_iTIME_MIN_PLAY = 30;

	char szTemp[256];

	wsprintf(szTemp, Lang.GetText(0,103));
	GSProtocol.AllSendServerMsg(szTemp);

	PMSG_ANS_CL_EFFECT pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x9E, sizeof(pMsg));
	pMsg.wEffectNum = 1;

	GSProtocol.DataSendAll((LPBYTE)&pMsg, pMsg.h.size);
	this->SetState(2);
}


void CRingAttackEvent::End_Menual()
{
	this->SetMenualStart(0);
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

