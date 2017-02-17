//////////////////////////////////////////////////////////////////////
// MuTimer.h
#ifndef MUTIMER_H
#define MUTIMER_H

#include "QueueTimer.h"

#pragma once

class CMuTimer
{
public:
	friend VOID WINAPI TimerProcQueue(LPVOID lpParameter, BOOL TimerOrWaitFired);
	friend VOID WINAPI TimerProcQueue2(LPVOID lpParameter, BOOL TimerOrWaitFired);
	CMuTimer();
	virtual ~CMuTimer();

	void SetMuTimer();

	void SetTimerEnd() { this->m_bTimerExpire = TRUE; }
	int GetTimerEnd() { return this->m_bTimerExpire; }

private:
	CQueueTimer m_MuTimer1;
	CQueueTimer m_MuTimer2;
	int m_bTimerExpire;
};

VOID WINAPI TimerProcQueue(LPVOID lpParameter, BOOL TimerOrWaitFired);
VOID WINAPI TimerProcQueue2(LPVOID lpParameter, BOOL TimerOfWaitFired);

extern CMuTimer g_MuTimer;

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

