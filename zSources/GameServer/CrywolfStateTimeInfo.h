// CrywolfStateTimeInfo.h: interface for the CCrywolfStateTimeInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CRYWOLFSTATETIMEINFO_H__0CB24640_4832_4A12_85E0_88DF55431959__INCLUDED_)
#define AFX_CRYWOLFSTATETIMEINFO_H__0CB24640_4832_4A12_85E0_88DF55431959__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCrywolfStateTimeInfo
{

public:

	CCrywolfStateTimeInfo()
	{
		this->Reset();
	}

	void Reset()
	{
		this->m_bUsed = FALSE;
		this->m_iStateNumber = -1;	
		this->m_iYear = -1;	
		this->m_iMonth = -1;	
		this->m_iDay = -1;	
		this->m_iDayOfWeek = -1;	
		this->m_iHour = -1;	
		this->m_iMinute = -1;	
		this->m_iSecond = -1;	
		this->m_iContinuanceTime = 0;	
		this->m_dwAppliedTickCount = 0;	
	}

	void SetAppliedTime()
	{
		this->m_dwAppliedTickCount = GetTickCount();
	}

	void ResetAppliedTime()
	{
		this->m_dwAppliedTickCount = 0;
	}

	BOOL CheckContinuanceTime()
	{
		if ( (GetTickCount() - this->m_dwAppliedTickCount ) > this->m_iContinuanceTime )
		{
			return FALSE;
		}

		return TRUE;
	}

	BOOL CheckScheduleTime()
	{
		SYSTEMTIME Time;
		GetLocalTime(&Time);

		int iYear = Time.wYear;
		int iMonth = Time.wMonth;
		int iDay = Time.wDay;
		int iDayOfWeek = Time.wDayOfWeek;
		int iHour = Time.wHour;
		int iMinute = Time.wMinute;
		int iCrywolfState = -1;

		if( this->m_iMonth == -1 || this->m_iMonth == iMonth )
		{
			if( this->m_iDayOfWeek == -1 || this->m_iDayOfWeek == iDayOfWeek )
			{
				if( this->m_iDay == -1 || this->m_iDay == iDay )
				{
					if( this->m_iHour == iHour )
					{
						if( this->m_iMinute == iMinute )
						{
							return true;
						}
					}
				}
			}
		}
		return false;
	}

	int GetLeftTime()
	{
		if ( this->CheckContinuanceTime() )
		{
			int iLeftTime = this->m_iContinuanceTime - (GetTickCount() - this->m_dwAppliedTickCount);

			if ( iLeftTime < 0 )
				iLeftTime = 0;

			return iLeftTime;
		}

		return 0;
	}

public:

	BOOL m_bUsed;	// 0
	int m_iStateNumber;	// 4
	int m_iYear;	// 8
	int m_iMonth;	// C
	int m_iDay;	// 10
	int m_iDayOfWeek;	// 14
	int m_iHour;	// 18
	int m_iMinute;	// 1C
	int m_iSecond;	// 20
	int m_iContinuanceTime;	// 24
	DWORD m_dwAppliedTickCount;	// 28

};

#endif // !defined(AFX_CRYWOLFSTATETIMEINFO_H__0CB24640_4832_4A12_85E0_88DF55431959__INCLUDED_)


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

