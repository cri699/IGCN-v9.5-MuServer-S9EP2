//////////////////////////////////////////////////////////////////////
// QueueTimer.h
#ifndef QUEUETIMER_H
#define QUEUETIMER_H

#include "TSync.h"

#pragma once

class CQueueTimer
{
public:
	CQueueTimer();
	virtual ~CQueueTimer();

	void RegCallBack(WAITORTIMERCALLBACK callback);
	void QueueTimerInit();
	
	void PushEventID(int eventID);
	void PopEventID();
	int GetEventID();

	int CreateTimer(int eventID, DWORD period, DWORD flags);
	void DeleteQueueTimer();

	void SyncLock() { this->m_Sync.Lock(); }
	void SyncUnlock() { this->m_Sync.Unlock(); }

private:

	WAITORTIMERCALLBACK m_callback;
	HANDLE m_hTimerQueue;
	std::stack<int> m_EventIdStack;
	TSync m_Sync;
};

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

