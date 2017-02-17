//////////////////////////////////////////////////////////////////////
// BonusEvent.cpp
#include "stdafx.h"
#include "BonusEvent.h"
#include "TLog.h"
#include "TNotice.h"

CBonusEvent g_BonusEvent;

CBonusEvent::CBonusEvent()
{
	InitializeCriticalSection(&this->m_criti);
	this->m_vtEventData.clear();
	this->hThread = NULL;
}

CBonusEvent::~CBonusEvent() 
{
	if (this->hThread != NULL)
	{
		TerminateThread(this->hThread, 0);
	}

	DeleteCriticalSection(&this->m_criti);
}

void CBonusEvent::LoadFile(char *szFile)
{
	EnterCriticalSection(&this->m_criti);

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(szFile);

	if (res.status != pugi::status_ok)
	{
		g_Log.MsgBox("Error loading %s file (%s)", szFile, res.description());
		LeaveCriticalSection(&this->m_criti);
		return;
	}

	this->m_vtEventData.clear();

	pugi::xml_node main = file.child("BonusEvent");

	this->m_bEventEnable = main.attribute("Enable").as_bool();
	this->m_bEventNotice = main.attribute("DisplayNotice").as_bool();
	
	for (pugi::xml_node Event = main.child("Event"); Event; Event = Event.next_sibling())
	{
		std::shared_ptr<BONUS_EVENT_DATA> bonus_event_ptr_(new BONUS_EVENT_DATA);

		bonus_event_ptr_->wDayOfWeek = Event.attribute("DayOfWeek").as_int();
		bonus_event_ptr_->wStartHour = Event.attribute("StartHour").as_int();
		bonus_event_ptr_->wEndHour = Event.attribute("EndHour").as_int();
		bonus_event_ptr_->AddExp = Event.attribute("ExpMultiplier").as_float();
		bonus_event_ptr_->AddMLExp = Event.attribute("MasterExpMultiplier").as_float();
		bonus_event_ptr_->AddDrop = Event.attribute("ItemDropBonus").as_int();
		bonus_event_ptr_->AddExcDrop = Event.attribute("ExcDropBonus").as_int();

		this->m_vtEventData.push_back(bonus_event_ptr_);
	}

	if (this->hThread == NULL)
	{
		this->hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)CBonusEvent::Run, this, NULL, NULL);
	}

	LeaveCriticalSection(&this->m_criti);
}

void CBonusEvent::Run(void *lpParam)
{
	while (true)
	{
		CBonusEvent *lpThis = (CBonusEvent *)lpParam;

		if (lpThis->m_bEventEnable == false)
		{
			Sleep(1000);
			continue;
		}

		SYSTEMTIME time;
		GetLocalTime(&time);

		std::shared_ptr<BONUS_EVENT_DATA> active_event_ptr = NULL;

		for (std::vector<std::shared_ptr<BONUS_EVENT_DATA>>::iterator It = lpThis->m_vtEventData.begin(); It != lpThis->m_vtEventData.end(); It++)
		{
			std::shared_ptr<BONUS_EVENT_DATA> event_ptr = *It;

			if (time.wDayOfWeek != event_ptr->wDayOfWeek && event_ptr->wDayOfWeek != (WORD)-1)
			{
				continue;
			}

			if (time.wHour >= event_ptr->wStartHour && time.wHour < event_ptr->wEndHour)
			{
				active_event_ptr = event_ptr;
				break;
			}
		}

		if (active_event_ptr != NULL)
		{
			EnterCriticalSection(&lpThis->m_criti);
			BOOL bChange = lpThis->m_curEvent_ptr != active_event_ptr ? TRUE : FALSE;
			LeaveCriticalSection(&lpThis->m_criti);

			if (bChange == TRUE)
			{
				if (lpThis->m_bEventNotice == true)
				{
					int nRemainMin = (active_event_ptr->wEndHour * 60) - (time.wHour * 60 + time.wMinute);
					PMSG_NOTICE pNotice;
					TNotice::MakeNoticeMsgEx(&pNotice, 0, Lang.GetText(0,600), active_event_ptr->wEndHour - active_event_ptr->wStartHour, nRemainMin);
					TNotice::SendNoticeToAllUser(&pNotice);

					g_Log.Add("[Bonus Event] Started Event (AddExp:%f) (AddMLExp:%f) (AddDrop:%f) (AddExcDrop:%f)",
						active_event_ptr->AddExp, active_event_ptr->AddMLExp, active_event_ptr->AddDrop, active_event_ptr->AddExcDrop);
				}

				EnterCriticalSection(&lpThis->m_criti);
				lpThis->m_curEvent_ptr = active_event_ptr;
				LeaveCriticalSection(&lpThis->m_criti);
			}
		}

		else
		{
			EnterCriticalSection(&lpThis->m_criti);
			BOOL bChange = lpThis->m_curEvent_ptr != NULL ? TRUE : FALSE;
			LeaveCriticalSection(&lpThis->m_criti);

			if (bChange == TRUE)
			{
				EnterCriticalSection(&lpThis->m_criti);
				lpThis->m_curEvent_ptr = NULL;
				LeaveCriticalSection(&lpThis->m_criti);
			}
		}

		Sleep(1000);
	}
}

float CBonusEvent::GetAddExp()
{
	EnterCriticalSection(&this->m_criti);

	if (this->m_curEvent_ptr == NULL)
	{
		LeaveCriticalSection(&this->m_criti);
		return 0.0;
	}

	float fAddExp = this->m_curEvent_ptr->AddExp;

	LeaveCriticalSection(&this->m_criti);
	return fAddExp;
}

float CBonusEvent::GetAddMLExp()
{
	EnterCriticalSection(&this->m_criti);

	if (this->m_curEvent_ptr == NULL)
	{
		LeaveCriticalSection(&this->m_criti);
		return 0.0;
	}

	float fAddMLExp = this->m_curEvent_ptr->AddMLExp;

	LeaveCriticalSection(&this->m_criti);
	return fAddMLExp;
}

WORD CBonusEvent::GetAddDrop()
{
	EnterCriticalSection(&this->m_criti);

	if (this->m_curEvent_ptr == NULL)
	{
		LeaveCriticalSection(&this->m_criti);
		return 0;
	}

	WORD wAddDrop = this->m_curEvent_ptr->AddDrop;

	LeaveCriticalSection(&this->m_criti);
	return wAddDrop;
}

WORD CBonusEvent::GetAddExcDrop()
{
	EnterCriticalSection(&this->m_criti);

	if (this->m_curEvent_ptr == NULL)
	{
		LeaveCriticalSection(&this->m_criti);
		return 0;
	}

	WORD wAddExcDrop = this->m_curEvent_ptr->AddExcDrop;

	LeaveCriticalSection(&this->m_criti);
	return wAddExcDrop;
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

