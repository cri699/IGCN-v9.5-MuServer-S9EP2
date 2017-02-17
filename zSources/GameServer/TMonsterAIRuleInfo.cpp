// TMonsterAIRuleInfo.cpp: implementation of the TMonsterAIRuleInfo class.
//	GS-N	1.00.18	JPN	0x00562FB0	-	Completed
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TMonsterAIRuleInfo.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TMonsterAIRuleInfo::TMonsterAIRuleInfo()
{
	this->Reset();
}

void TMonsterAIRuleInfo::Reset()
{
	this->m_iRuleNumber = -1;
	this->m_iMonsterClass = -1;
	this->m_iMonsterAIUnit = 0;
	this->m_iRuleCondition = -1;
	this->m_iWaitTime = -1;
	this->m_iContinuanceTime = -1;
	this->m_iMonth = -1;
	this->m_iDay = -1;
	this->m_iWeekDay = -1;
	this->m_iHour = -1;
	this->m_iMinute = -1;
	this->m_iAppliedTime = -1;
	this->m_iApplyingTime = -1;
	memset(this->m_szRuleDesc, 0, sizeof(this->m_szRuleDesc));
}


BOOL TMonsterAIRuleInfo::IsValid()
{
	BOOL bIsValid = FALSE;

	switch ( this->m_iRuleCondition )
	{
		case 1:
			bIsValid = this->CheckConditionSpecificDate();
			break;

		case 71:
			bIsValid = this->CheckConditionCrywolfStart();
			break;

		case 72:
			bIsValid = this->CheckConditionCrywolfEnd();
			break;
	}

	if ( !bIsValid )
	{
		this->m_iAppliedTime = -1;
		this->m_iApplyingTime = -1;

		return FALSE;
	}

	if ( this->m_iAppliedTime == -1 )
		this->m_iAppliedTime = GetTickCount();

	if ( (this->m_iAppliedTime + (this->m_iWaitTime * 1000)) > GetTickCount() )
		return FALSE;

	if ( (  this->m_iAppliedTime + this->m_iWaitTime * 1000 + this->m_iContinuanceTime * 1000) < GetTickCount() )
		return FALSE;

	return TRUE;
}

#define _SECOND ((__int64) 10000000)
#define _MINUTE (60 * _SECOND)
#define _HOUR   (60 * _MINUTE)
#define _DAY    (24 * _HOUR)

inline unsigned __int64 to_integral( const SYSTEMTIME& st )
{
   FILETIME ft ;
   SystemTimeToFileTime( &st, &ft ) ;
   ULARGE_INTEGER integer ;
   integer.LowPart = ft.dwLowDateTime ;
   integer.HighPart = ft.dwHighDateTime ;
   return integer.QuadPart ;
}

inline bool operator== ( const SYSTEMTIME& a, const SYSTEMTIME& b )
{ return to_integral(a) == to_integral(b) ; }
inline bool operator< ( const SYSTEMTIME& a, const SYSTEMTIME& b )
{ return to_integral(a) < to_integral(b) ; }
inline bool operator<= ( const SYSTEMTIME& a, const SYSTEMTIME& b )
{ return to_integral(a) <= to_integral(b) ; }
inline bool operator!= ( const SYSTEMTIME& a, const SYSTEMTIME& b ) 
{ return !( a==b ) ; }
inline bool operator> ( const SYSTEMTIME& a, const SYSTEMTIME& b ) 
{ return !( a<=b ) ; }
inline bool operator>= ( const SYSTEMTIME& a, const SYSTEMTIME& b ) 
{ return !( a<b ) ; }
inline SYSTEMTIME operator+ (const SYSTEMTIME &a, const SYSTEMTIME &b )
{
	FILETIME a1;
	FILETIME a2;
	FILETIME a3;
	SYSTEMTIME result;
	SystemTimeToFileTime(&a, &a1);
	SystemTimeToFileTime(&b, &a2);

	ULONGLONG qw1;
	ULONGLONG qw2;
	qw1 = (((ULONGLONG) a1.dwHighDateTime) << 32) + a1.dwLowDateTime;
	qw2 = (((ULONGLONG) a2.dwHighDateTime) << 32) + a2.dwLowDateTime;

	qw1 += qw2;

	a3.dwHighDateTime = (DWORD)(qw1 >> 32);
	a3.dwLowDateTime = (DWORD)(qw1 & 0xFFFFFFFF);
	FileTimeToSystemTime(&a3, &result);

	return result;
}

BOOL TMonsterAIRuleInfo::CheckConditionSpecificDate()
{
	SYSTEMTIME tCurrentTime;
	SYSTEMTIME tValidRange;
	GetLocalTime(&tCurrentTime);
	FILETIME pTmp;
	SystemTimeToFileTime(&tCurrentTime, &pTmp);
	ULONGLONG qwResult;
	qwResult = (((ULONGLONG) pTmp.dwHighDateTime) << 32) + pTmp.dwLowDateTime;
	qwResult += (this->m_iWaitTime+this->m_iContinuanceTime) * _SECOND;
	pTmp.dwHighDateTime = (DWORD)(qwResult >> 32);
	pTmp.dwLowDateTime = (DWORD)(qwResult & 0xFFFFFFFF);
	FileTimeToSystemTime(&pTmp, &tValidRange);

	int iYear = tCurrentTime.wYear;
	int iMonth = tCurrentTime.wMonth;
	int iDay = tCurrentTime.wDay;
	int iDayOfWeek = tCurrentTime.wDayOfWeek;
	int iHour = tCurrentTime.wHour;
	int iMinute = tCurrentTime.wMinute;

	SYSTEMTIME tConditionTime;

	if ( this->m_iMonth != -1 )
	{
		if ( this->m_iMonth != iMonth )
		{
			tConditionTime.wYear = iYear;
			tConditionTime.wMonth = this->m_iMonth;

			if ( tCurrentTime < tConditionTime  ||
				 tCurrentTime > tConditionTime+tValidRange )
			{
				return FALSE;
			}
		}
	}

	if ( this->m_iDay != -1 )
	{
		if ( this->m_iDay != iDay )
		{
			tConditionTime.wYear = iYear;
			tConditionTime.wMonth = (this->m_iMonth == -1)?iMonth : this->m_iMonth;
			tConditionTime.wDay = this->m_iDay;

			if ( tCurrentTime < tConditionTime  ||
				 tCurrentTime > tConditionTime+tValidRange )
			{
				return FALSE;
			}
		}
	}

	if ( this->m_iWeekDay != -1 )
	{
		if ( this->m_iWeekDay != iDayOfWeek )
		{
			return FALSE;
		}
	}

	if ( this->m_iHour != -1 )
	{
		if ( this->m_iHour != iHour )
		{
			tConditionTime.wYear = iYear;
			tConditionTime.wMonth = (this->m_iMonth == -1)?iMonth : this->m_iMonth;
			tConditionTime.wDay = (this->m_iDay == -1)?iDay : this->m_iDay;
			tConditionTime.wHour = this->m_iHour;

			if ( tCurrentTime < tConditionTime  ||
				 tCurrentTime > tConditionTime+tValidRange )
			{
				return FALSE;
			}
		}
	}

	if ( this->m_iMinute != -1 )
	{
		if ( this->m_iMinute != iMinute )
		{
			tConditionTime.wYear = iYear;
			tConditionTime.wMonth = (this->m_iMonth == -1)?iMonth : this->m_iMonth;
			tConditionTime.wDay = (this->m_iDay == -1)?iDay : this->m_iDay;
			tConditionTime.wHour = (this->m_iHour == -1)?iHour : this->m_iHour;
			tConditionTime.wMinute = this->m_iMinute;

			if ( tCurrentTime < tConditionTime  ||
				 tCurrentTime > tConditionTime+tValidRange )
			{
				return FALSE;
			}
		}
	}
	
	return TRUE;
}	

BOOL TMonsterAIRuleInfo::CheckConditionCrywolfStart()
{
	return FALSE;
}

BOOL TMonsterAIRuleInfo::CheckConditionCrywolfEnd()
{
	return FALSE;
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

