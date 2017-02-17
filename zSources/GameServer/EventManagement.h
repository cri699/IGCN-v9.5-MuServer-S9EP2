//////////////////////////////////////////////////////////////////////
// EventManagement.h
#ifndef EVENTMANAGEMENT_H
#define EVENTMANAGEMENT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


struct EVENT_ID_TIME
{
	int m_eEventKind;	// 0
	int m_iDayOfWeek;
	int m_iHour;	// 4
	int m_iMinute;	// 8
	bool m_bEventStarted;	// C
};


class CEventManagement
{
public:

	CEventManagement();
	virtual ~CEventManagement();

	void Clear();
	void Init();
	bool Load(LPSTR lpszFileName);
	void Run();
	void StartEvent(int eEventKind);
	void RegEvent(int eEventKind, void* lpEventObj);

private:

	BOOL m_bEventStart;	// 4
	BOOL m_bHasData;	// 8
	std::vector<EVENT_ID_TIME> m_vtEventTime;	// C
	std::map<int,void *> m_mapEventObj;	// 1C
	WORD m_wToday_Year;	// 2C
	WORD m_wToday_Month;	// 2E
	WORD m_wToday_Day;	// 30
	WORD m_wToday_DayOfWeek;

	enum {
	  EVENT_ID_DRAGONHERD = 0x0,
	  EVENT_ID_UNDERTROOP = 0x1,
	  EVENT_ID_ELDORADO = 0x2,
	  EVENT_ID_WHITEMAGE = 0x3,
	  EVENT_ID_XMASSANTA = 0x4,
	  EVENT_ID_PVPLMS = 0x6
	};
};

extern CEventManagement g_EventManager;

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

