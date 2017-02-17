//////////////////////////////////////////////////////////////////////
// EventManagement.cpp
#include "stdafx.h"
#include "EventManagement.h"
#include "TLog.h"
#include "DragonEvent.h"
#include "AttackEvent.h"
#include "EledoradoEvent.h"
#include "RingAttackEvent.h"
#include "XMasAttackEvent.h"

// GS-N 0.99.60T 0x00464EC0
//	GS-M	1.00.18	JPN	0x00477D60	-	Completed

CEventManagement g_EventManager;


CEventManagement::CEventManagement()
{
	this->m_bEventStart = false;
	this->m_bHasData = false;
	this->Clear();
}


CEventManagement::~CEventManagement()
{
	return;
}


void CEventManagement::Clear()
{
	this->m_vtEventTime.clear();
	this->m_mapEventObj.clear();
	this->m_wToday_Year = 0;
	this->m_wToday_Month = 0;
	this->m_wToday_Day = 0;
}



void CEventManagement::Init()
{
	if ( this->m_bHasData != false && this->m_bEventStart != false )
	{
		SYSTEMTIME sysTime;

		GetLocalTime(&sysTime);
		this->m_wToday_Year = sysTime.wYear;
		this->m_wToday_Month = sysTime.wMonth;
		this->m_wToday_Day = sysTime.wDay;
		this->m_wToday_DayOfWeek = sysTime.wDayOfWeek;

		std::map<int, void *>::iterator _Itor;
		
		_Itor = this->m_mapEventObj.find(EVENT_ID_DRAGONHERD);

		if ( _Itor != this->m_mapEventObj.end() )
		{
			CDragonEvent * lpEvent = (CDragonEvent *)_Itor->second;
			lpEvent->SetMenualStart(FALSE);

			for (std::vector<EVENT_ID_TIME>::iterator it = this->m_vtEventTime.begin() ; it != this->m_vtEventTime.end(); it++)
			{
				if ( (*(it)).m_eEventKind == EVENT_ID_DRAGONHERD )
				{
					lpEvent->SetMenualStart(TRUE);
				}
			}
		}

		_Itor = this->m_mapEventObj.find(EVENT_ID_UNDERTROOP);

		if ( _Itor != this->m_mapEventObj.end() )
		{
			CAttackEvent * lpEvent = (CAttackEvent *)_Itor->second;
			lpEvent->SetMenualStart(FALSE);

			for (std::vector<EVENT_ID_TIME>::iterator it = this->m_vtEventTime.begin() ; it != this->m_vtEventTime.end(); it++)
			{
				if ( (*(it)).m_eEventKind == EVENT_ID_UNDERTROOP )
				{
					lpEvent->SetMenualStart(TRUE);
				}
			}
		}

		_Itor = this->m_mapEventObj.find(EVENT_ID_ELDORADO);

		if ( _Itor != this->m_mapEventObj.end() )
		{
			CEledoradoEvent * lpEvent = (CEledoradoEvent *)_Itor->second;
			lpEvent->SetMenualStart(FALSE);

			for (std::vector<EVENT_ID_TIME>::iterator it = this->m_vtEventTime.begin() ; it != this->m_vtEventTime.end(); it++)
			{
				if ( (*(it)).m_eEventKind == EVENT_ID_ELDORADO )
				{
					lpEvent->SetMenualStart(TRUE);
				}
			}
		}

		_Itor = this->m_mapEventObj.find(EVENT_ID_WHITEMAGE);

		if ( _Itor != this->m_mapEventObj.end() )
		{
			CRingAttackEvent * lpEvent = (CRingAttackEvent *)_Itor->second;
			lpEvent->SetMenualStart(FALSE);

			for (std::vector<EVENT_ID_TIME>::iterator it = this->m_vtEventTime.begin() ; it != this->m_vtEventTime.end(); it++)
			{
				if ( (*(it)).m_eEventKind == EVENT_ID_WHITEMAGE )	// #error Deathway Fix Here (GS says ELEDORADO
				{
					lpEvent->SetMenualStart(TRUE);
				}
			}
		}

		_Itor = this->m_mapEventObj.find(EVENT_ID_XMASSANTA);

		if ( _Itor != this->m_mapEventObj.end() )
		{
			CXMasAttackEvent * lpEvent = (CXMasAttackEvent *)_Itor->second;
			lpEvent->SetMenualStart(FALSE);

			for (std::vector<EVENT_ID_TIME>::iterator it = this->m_vtEventTime.begin() ; it != this->m_vtEventTime.end(); it++)
			{
				if ( (*(it)).m_eEventKind == EVENT_ID_XMASSANTA )	// #error Deathway Fix Here (GS says ELEDORADO
				{
					lpEvent->SetMenualStart(TRUE);
				}
			}
		}
	}
	else
	{
		std::map<int, void *>::iterator _Itor;
		
		_Itor = this->m_mapEventObj.find(EVENT_ID_DRAGONHERD);
		
		if ( _Itor != this->m_mapEventObj.end() )
		{
			CDragonEvent * lpEvent = (CDragonEvent *)_Itor->second;
			lpEvent->SetMenualStart(FALSE);
		}

	
		_Itor = this->m_mapEventObj.find(EVENT_ID_UNDERTROOP);
		
		if ( _Itor != this->m_mapEventObj.end() )
		{
			CAttackEvent * lpEvent = (CAttackEvent *)_Itor->second;
			lpEvent->SetMenualStart(FALSE);
		}

		_Itor = this->m_mapEventObj.find(EVENT_ID_ELDORADO);
		
		if ( _Itor != this->m_mapEventObj.end() )
		{
			CEledoradoEvent * lpEvent = (CEledoradoEvent *)_Itor->second;
			lpEvent->SetMenualStart(FALSE);
		}

		_Itor = this->m_mapEventObj.find(EVENT_ID_WHITEMAGE);
		
		if ( _Itor != this->m_mapEventObj.end() )
		{
			CRingAttackEvent * lpEvent = (CRingAttackEvent *)_Itor->second;
			lpEvent->SetMenualStart(FALSE);
		}

		_Itor = this->m_mapEventObj.find(EVENT_ID_XMASSANTA);
		
		if ( _Itor != this->m_mapEventObj.end() )
		{
			CXMasAttackEvent * lpEvent = (CXMasAttackEvent *)_Itor->second;
			lpEvent->SetMenualStart(FALSE);
		}
	}
}


void CEventManagement::Run()
{
	if ( this->m_bEventStart == false || this->m_bHasData == false || this->m_vtEventTime.empty() != FALSE )
	{
		return;
	}

	SYSTEMTIME sysTime;
	std::vector<EVENT_ID_TIME>::iterator it;

	GetLocalTime(&sysTime);

	if ( this->m_wToday_Year != sysTime.wYear || this->m_wToday_Month != sysTime.wMonth || this->m_wToday_Day != sysTime.wDay )
	{
		for ( it = this->m_vtEventTime.begin() ; it != this->m_vtEventTime.end() ; it++ )
		{
			(*(it)).m_bEventStarted = false;
		}

		g_Log.Add("[Event Management] Wrong date: %02d %02d %02d %02d", this->m_wToday_Year, this->m_wToday_Month, this->m_wToday_Day, this->m_wToday_DayOfWeek);	// Deathway Need Translation

		this->m_wToday_Year = sysTime.wYear;
		this->m_wToday_Month = sysTime.wMonth;
		this->m_wToday_Day = sysTime.wDay;
		this->m_wToday_DayOfWeek = sysTime.wDayOfWeek;
	}

	for ( it = this->m_vtEventTime.begin() ; it != this->m_vtEventTime.end() ; it++ )
	{
		if ( sysTime.wHour == (*(it)).m_iHour && sysTime.wMinute == (*(it)).m_iMinute && (sysTime.wDayOfWeek == (*(it)).m_iDayOfWeek || (*(it)).m_iDayOfWeek == -1) && (*(it)).m_bEventStarted == false )
		{
			g_Log.Add("[Event Management] Found Event: (%d) %02d %02d (state=%d)",
				(*(it)).m_eEventKind , (*(it)).m_iHour, sysTime.wMinute , (*(it)).m_bEventStarted);	// #error Deathway Need Translation

			(*(it)).m_bEventStarted = true;
			this->StartEvent((*(it)).m_eEventKind);
		}
	}
}




void CEventManagement::StartEvent(int eEventKind)
{
	switch ( eEventKind )
	{
		case EVENT_ID_DRAGONHERD:
			{
				std::map<int, void *>::iterator _Itor = this->m_mapEventObj.find(EVENT_ID_DRAGONHERD);

				if ( _Itor != this->m_mapEventObj.end() )
				{
					CDragonEvent * lpEvent = (CDragonEvent *)_Itor->second;
					lpEvent->Start_Menual();
				}
			}
			break;

		case EVENT_ID_UNDERTROOP:
			{
				std::map<int, void *>::iterator _Itor = this->m_mapEventObj.find(EVENT_ID_UNDERTROOP);

				if ( _Itor != this->m_mapEventObj.end() )
				{
					CAttackEvent * lpEvent = (CAttackEvent *)_Itor->second;
					lpEvent->Start_Menual();
				}
			}
			break;

		case EVENT_ID_ELDORADO:
			{
				std::map<int, void *>::iterator _Itor = this->m_mapEventObj.find(EVENT_ID_ELDORADO);

				if ( _Itor != this->m_mapEventObj.end() )
				{
					CEledoradoEvent * lpEvent = (CEledoradoEvent *)_Itor->second;
					lpEvent->Start_Menual();
				}
			}
			break;

		case EVENT_ID_WHITEMAGE:
			{
				std::map<int, void *>::iterator _Itor = this->m_mapEventObj.find(EVENT_ID_WHITEMAGE);

				if ( _Itor != this->m_mapEventObj.end() )
				{
					CRingAttackEvent * lpEvent = (CRingAttackEvent *)_Itor->second;
					lpEvent->Start_Menual();
				}
			}
			break;
		case EVENT_ID_XMASSANTA:
			{
				std::map<int, void *>::iterator _Itor = this->m_mapEventObj.find(EVENT_ID_XMASSANTA);

				if ( _Itor != this->m_mapEventObj.end() )
				{
					CXMasAttackEvent * lpEvent = (CXMasAttackEvent *)_Itor->second;
					lpEvent->Start_Menual();
				}
			}
			break;
	}
}




bool CEventManagement::Load(LPSTR lpszFileName)
{
	this->m_bHasData = false;
	this->Clear();

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(lpszFileName);

	if ( res.status != pugi::status_ok )
	{
		g_Log.MsgBox("[DOTH] Info file Load Fail [%s] [%s]", lpszFileName, res.description());

		return false;
	}

	pugi::xml_node main = file.child("InvasionManager");

	this->m_bEventStart = main.attribute("Enable").as_bool();

	for (pugi::xml_node invasion = main.child("Invasion"); invasion; invasion = invasion.next_sibling())
	{
		EVENT_ID_TIME EIT;

		EIT.m_eEventKind = invasion.attribute("Type").as_int();

		for (pugi::xml_node start = invasion.child("Start"); start; start = start.next_sibling())
		{
			EIT.m_iDayOfWeek = start.attribute("DayOfWeek").as_int();
			EIT.m_iHour = start.attribute("Hour").as_int();
			EIT.m_iMinute = start.attribute("Minute").as_int();
			EIT.m_bEventStarted = false;

			this->m_vtEventTime.push_back(EIT);
		}
	}

	g_Log.Add("[DOTH] %s file load!", lpszFileName);

	this->m_bHasData = true;

	return true;
}




void CEventManagement::RegEvent(int eEventKind, void * lpEventObj)
{
	this->m_mapEventObj.insert( std::pair<int, void *>(eEventKind, lpEventObj) );
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

