//////////////////////////////////////////////////////////////////////
// TLog.cpp
#include "stdafx.h"
#include "TLog.h"
#include "winutil.h"
#include "GameMain.h"
#include "configread.h"

TLog g_Log;

TLog::TLog()
{
	InitializeCriticalSection(&this->m_LogFileCriticalSection);
	InitializeCriticalSection(&this->m_LogViewCriticalSection);
}

TLog::~TLog()
{
	DeleteCriticalSection(&this->m_LogFileCriticalSection);
	DeleteCriticalSection(&this->m_LogViewCriticalSection);
}

void TLog::LogInit(bool bLogPrint, int iMaxLogFileSize)
{
	this->SetMaxLogFileSize(iMaxLogFileSize);

	this->m_iCurrentLine = 0;
	this->m_fLog = nullptr;

	m_StatusColors.StandByBG = CreateSolidBrush(RGB(0x00, 0x00, 0x00)); // Black;
	m_StatusColors.StandByText = RGB(0xFF, 0xFF, 0xFF); // White;

	this->m_bWriteLog = bLogPrint;

	int iStrSize = sizeof(m_LogStr[0]);

	for (int i = 0; i < LOG_TEXT_LINE; i++)
	{
		memset(&this->m_LogStr[i], 0, iStrSize);
	}

	if (this->m_bWriteLog == true)
	{
		this->LogDataSet_Start();
	}
}

void TLog::LogDataSet_Start()
{
	char szTemp[255];
	memset(szTemp, 0, sizeof(szTemp));

	tm * today;
	time_t ltime;

	time(&ltime);
	today = localtime(&ltime);

	today->tm_year = today->tm_year + 1900;

	this->m_CurDate.Day = today->tm_mday;
	this->m_CurDate.Month = today->tm_mon + 1;
	this->m_CurDate.Year = today->tm_year;

	this->m_StartDate.Day = this->m_CurDate.Day;
	this->m_StartDate.Month = this->m_CurDate.Month;
	this->m_StartDate.Year = this->m_CurDate.Year;

	wsprintf(szTemp, "Log\\%02d.%02d.%04d_%s.log", this->m_StartDate.Day, this->m_StartDate.Month, this->m_StartDate.Year, g_ConfigRead.server.GetServerName());

	CreateDirectory("Log", NULL);

	if (IsFile(szTemp) == TRUE)
	{
		for (int i = 2; ; i++)
		{
			memset(szTemp, 0, sizeof(szTemp));

			wsprintf(szTemp, "Log\\%02d.%02d.%04d_%s_start-%d.log", this->m_StartDate.Day, this->m_StartDate.Month, this->m_StartDate.Year, g_ConfigRead.server.GetServerName(), i);

			if (IsFile(szTemp) == false)
			{
				break;
			}
		}
	}

	memset(this->m_szLogFileName_Orig, 0, sizeof(this->m_szLogFileName_Orig));
	memcpy(this->m_szLogFileName_Orig, szTemp, strlen(szTemp) - 4);
	strcpy(this->m_szLogFileName, szTemp);

	if (this->m_fLog == nullptr)
	{
		this->m_fLog = fopen(this->m_szLogFileName, "a+t");

		if (m_fLog == nullptr)
		{
			this->MsgBox("Log file create error.");
		}
	}
}

void TLog::LogDataSet_Check()
{
	tm * today;
	time_t ltime;

	time(&ltime);
	today = localtime(&ltime);

	today->tm_year = today->tm_year + 1900;

	int iYear = today->tm_year;
	int iMonth = today->tm_mon + 1;
	int iDay = today->tm_mday;

	char szTemp[255];
	memset(szTemp, 0, sizeof(szTemp));

	EnterCriticalSection(&this->m_LogFileCriticalSection);

	if ((iDay == this->m_CurDate.Day) && (iMonth == this->m_CurDate.Month) && (iYear == this->m_CurDate.Year))
	{
		DWORD dwFileSize = CalcFileSize(this->m_szLogFileName);

		if (dwFileSize < this->m_dwMaxLogFileSize)
		{
			LeaveCriticalSection(&this->m_LogFileCriticalSection);
			return;
		}

		CreateDirectory("Log", NULL);

		if (this->m_fLog != nullptr)
		{
			fclose(this->m_fLog);
			this->m_fLog = nullptr;
		}

		for (int i = 1; ; i++)
		{
			memset(szTemp, 0, sizeof(szTemp));

			wsprintf(szTemp, "%s_part-%d.log", this->m_szLogFileName_Orig, i);

			if (IsFile(szTemp) == false)
			{
				break;
			}
		}
	}
	else
	{
		this->m_CurDate.Day = iDay;
		this->m_CurDate.Month = iMonth;
		this->m_CurDate.Year = iYear;

		if (this->m_fLog != nullptr)
		{
			fclose(this->m_fLog);
			this->m_fLog = nullptr;
		}

		CreateDirectory("Log", NULL);
		wsprintf(szTemp, "Log\\%02d.%02d.%04d_-_%02d.%02d.%04d_%s.log", this->m_StartDate.Day, this->m_StartDate.Month, this->m_StartDate.Year, this->m_CurDate.Day, this->m_CurDate.Month, this->m_CurDate.Year, g_ConfigRead.server.GetServerName());

		char szTemp2[255];
		sprintf(szTemp2, "Log\\%02d.%02d.%04d_%s.log", this->m_CurDate.Day, this->m_CurDate.Month, this->m_CurDate.Year, g_ConfigRead.server.GetServerName());

		memset(this->m_szLogFileName_Orig, 0x00, sizeof(this->m_szLogFileName_Orig));
		memcpy(this->m_szLogFileName_Orig, szTemp2, strlen(szTemp2) - 4);
	}

	strcpy(this->m_szLogFileName, szTemp);

	if (this->m_fLog == nullptr)
	{
		this->m_fLog = fopen(this->m_szLogFileName, "a+t");

		if (this->m_fLog == nullptr)
		{
			this->MsgBox("Log File create error.");
		}
	}

	LeaveCriticalSection(&this->m_LogFileCriticalSection);
}

void TLog::LogTextClear()
{
	EnterCriticalSection(&this->m_LogViewCriticalSection);

	int iStrSize = sizeof(this->m_LogStr[0]);

	for (int i = 0; i < LOG_TEXT_LINE; i++)
	{
		memset(&this->m_LogStr[i], 0, iStrSize);
	}

	LeaveCriticalSection(&this->m_LogViewCriticalSection);
}

bool TLog::LogDateChange()
{
	tm * today;
	time_t ltime;

	time(&ltime);

	today = localtime(&ltime);
	today->tm_year = today->tm_year + 1900;
	today->tm_mon = today->tm_mon + 1;

	if (today->tm_mday <= m_CurDate.Day)
	{
		if (today->tm_mon <= m_CurDate.Month)
		{
			if (today->tm_year <= m_CurDate.Year)
			{
				return false;
			}
		}
	}

	LogDataSet_Check();
	return true;
}

void TLog::LogTextAdd(TColor::T tColor, char * szMsg, size_t iLen)
{
	EnterCriticalSection(&this->m_LogViewCriticalSection);

    if(iLen >= LOG_TEXT_LENGTH)
    {
        iLen			= LOG_TEXT_LENGTH - 1;
    }
    
    m_iCurrentLine++;
    
    if(m_iCurrentLine >= LOG_TEXT_LINE)
    {
        m_iCurrentLine		= 0;
    }
    
    memcpy(m_LogStr[m_iCurrentLine].Text, szMsg, iLen);
    
    m_LogStr[m_iCurrentLine].Text[iLen]	= '\0';
    m_LogStr[m_iCurrentLine].Lenght		= iLen;
    m_LogStr[m_iCurrentLine].Color		= tColor; // Phew;

	LeaveCriticalSection(&this->m_LogViewCriticalSection);
}

void TLog::MsgBox(char * szlog, ...)
{
    char szBuffer[512];
    memset(szBuffer, 0, sizeof(szBuffer));
    
    va_list pArguments;
    
    va_start(pArguments, szlog);
    vsprintf(szBuffer, szlog, pArguments);
    va_end(pArguments);
    
    MessageBox(NULL, szBuffer, "error", MB_OK | MB_APPLMODAL);
}

int TLog::MsgBox(UINT uType, char * szCaption, char * szlog, ...)
{
    char szBuffer[512];
    memset(szBuffer, 0, sizeof(szBuffer));
    
    va_list pArguments;
    
    va_start(pArguments, szlog);
    vsprintf(szBuffer, szlog, pArguments);
    va_end(pArguments);
    
    return MessageBox(NULL, szBuffer, szCaption, uType);
}

void TLog::AddMSB(char * szLog, ...)
{
	char szArgBuff[2048];
	char szLogBuff[2048];
	memset(szArgBuff, 0, sizeof(szArgBuff));
	memset(szLogBuff, 0, sizeof(szLogBuff));

	va_list lpArguments;

	va_start(lpArguments, szLog);
	vsprintf(szArgBuff, szLog, lpArguments);
	va_end(lpArguments);

	tm * today;
	time_t ltime;

	time(&ltime);
	today = localtime(&ltime);

	sprintf(szLogBuff, "[%.8s] ", asctime(today) + 11);
	strcat(szLogBuff, szArgBuff);

	LogTextAdd(TColor::Firebrick, szLogBuff, strlen(szLogBuff));

	if (m_bWriteLog == true)
	{
		EnterCriticalSection(&this->m_LogFileCriticalSection);

		fprintf(this->m_fLog, "%s\n", szLogBuff);
		fflush(this->m_fLog);

		LeaveCriticalSection(&this->m_LogFileCriticalSection);

		LogDataSet_Check();
	}

	MessageBox(NULL, szLogBuff, "[WARNING]", MB_OK);
}

void TLog::Add(char * szLog, ...)
{
	__try
	{
		char szArgBuff[2048];
		char szLogBuff[2048];
		memset(szArgBuff, 0, sizeof(szArgBuff));
		memset(szLogBuff, 0, sizeof(szLogBuff));

		va_list lpArguments;

		va_start(lpArguments, szLog);
		vsprintf(szArgBuff, szLog, lpArguments);
		va_end(lpArguments);

		tm * today;
		time_t ltime;

		time(&ltime);
		today = localtime(&ltime);

		sprintf(szLogBuff, "[%.8s] ", asctime(today) + 11);
		strcat(szLogBuff, szArgBuff);

		LogTextAdd(TColor::WhiteSmoke, szLogBuff, strlen(szLogBuff));

		if (m_bWriteLog == true)
		{
			EnterCriticalSection(&this->m_LogFileCriticalSection);

			fprintf(this->m_fLog, "%s\n", szLogBuff);
			fflush(this->m_fLog);

			LeaveCriticalSection(&this->m_LogFileCriticalSection);

			LogDataSet_Check();
		}

	}

	__except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH)
	{

	}
}

void TLog::AddC(TColor::T tColor, char * szLog, ...)
{
	__try
	{
		char szArgBuff[2048];
		char szLogBuff[2048];
		memset(szArgBuff, 0, sizeof(szArgBuff));
		memset(szLogBuff, 0, sizeof(szLogBuff));

		va_list lpArguments;

		va_start(lpArguments, szLog);
		vsprintf(szArgBuff, szLog, lpArguments);
		va_end(lpArguments);

		tm * today;
		time_t ltime;

		time(&ltime);
		today = localtime(&ltime);

		sprintf(szLogBuff, "[%.8s] ", asctime(today) + 11);
		strcat(szLogBuff, szArgBuff);

		LogTextAdd(tColor, szLogBuff, strlen(szLogBuff));

		if (m_bWriteLog == true)
		{
			EnterCriticalSection(&this->m_LogFileCriticalSection);

			fprintf(this->m_fLog, "%s\n", szLogBuff);
			fflush(this->m_fLog);

			LeaveCriticalSection(&this->m_LogFileCriticalSection);

			LogDataSet_Check();
		}

	}

	__except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH)
	{

	}
}

void TLog::AddF(char * szLog, ...)
{
    if(m_bWriteLog == false)
    {
        return;
    }
    
	char szArgBuff[2048];
	char szLogBuff[2048];
	memset(szArgBuff, 0, sizeof(szArgBuff));
	memset(szLogBuff, 0, sizeof(szLogBuff));

	va_list lpArguments;

	va_start(lpArguments, szLog);
	vsprintf(szArgBuff, szLog, lpArguments);
	va_end(lpArguments);

	tm * today;
	time_t ltime;

	time(&ltime);
	today = localtime(&ltime);

	sprintf(szLogBuff, "[%.8s] ", asctime(today) + 11);
	strcat(szLogBuff, szArgBuff);
    
	EnterCriticalSection(&this->m_LogFileCriticalSection);

	fprintf(this->m_fLog, "%s\n", szLogBuff);
	fflush(this->m_fLog);

	LeaveCriticalSection(&this->m_LogFileCriticalSection);

	LogDataSet_Check();
}

void TLog::AddL(TColor::T tColor, char * szLog, ...)
{
	char szArgBuff[2048];
	char szLogBuff[2048];
	memset(szArgBuff, 0, sizeof(szArgBuff));
	memset(szLogBuff, 0, sizeof(szLogBuff));

	va_list lpArguments;

	va_start(lpArguments, szLog);
	vsprintf(szArgBuff, szLog, lpArguments);
	va_end(lpArguments);

	tm * today;
	time_t ltime;

	time(&ltime);
	today = localtime(&ltime);

	sprintf(szLogBuff, "[%.8s] ", asctime(today) + 11);
	strcat(szLogBuff, szArgBuff);

	LogTextAdd(tColor, szLogBuff, strlen(szLogBuff));
}

void TLog::AddHeadHex(char * szText, LPBYTE lpBuf, int iSize)
{
    if(m_bWriteLog == false)
    {
        return;
    }

	char szLogBuff[2048];
	memset(szLogBuff, 0, sizeof(szLogBuff));

    
    tm * today;
    time_t ltime;
    
    time(& ltime);
    today = localtime(&ltime);
    
    LogDataSet_Check();
    
    sprintf(szLogBuff, "[%.8s][%s][%d] Buffer: (", asctime(today) + 11, szText, iSize);
    
    for(int n = 0; n < iSize; n++)
    {
		char szHexBuff[8];
		sprintf(szHexBuff, "%02X ", lpBuf[n]);
		strcat(szLogBuff, szHexBuff);
    }
    
	strcat(szLogBuff - 1, ")");

    if(m_bWriteLog == true)
    {
		EnterCriticalSection(&this->m_LogFileCriticalSection);

		fprintf(this->m_fLog, "%s\n", szLogBuff);
		fflush(this->m_fLog);

		LeaveCriticalSection(&this->m_LogFileCriticalSection);

		LogDataSet_Check();
    }
}

void TLog::LogTextPaint(HWND hWnd)
{
	EnterCriticalSection(&this->m_LogViewCriticalSection);
    HDC hDC				= GetDC(hWnd);
    
    RECT m_Rect;
    
    GetClientRect(hWnd, & m_Rect);
    
    m_Rect.top		= 15;
    
    FillRect(hDC, & m_Rect, m_StatusColors.StandByBG);
    
    int iCorX		= 5;
    int iCorY		= m_Rect.top + 5;
    
    int iLinesPerSize	= (m_Rect.bottom - 5 - iCorY) / 15;
    
    int n			= m_iCurrentLine;
    
    size_t iStrLen	= 0;
    
    iCorY			+= (iLinesPerSize * 15);
    
    while(iLinesPerSize-- > 0)
    {
        iStrLen		= m_LogStr[n].Lenght;
        
        if(iStrLen < 1)
        {
            break;
        }
        
        SetBkMode(hDC, TRANSPARENT);
        
        SelectObject(hDC, m_HFont);
        
        SetTextColor(hDC, m_LogStr[n].Color);
        
        iCorY		-= 15;
        
        TextOut(hDC, iCorX, iCorY, m_LogStr[n].Text, (int)iStrLen);
        
        n--;
        
        if(n < 0)
        {
            n		= LOG_TEXT_LINE - 1;
        }
    }
    
    ReleaseDC(hWnd, hDC);
	LeaveCriticalSection(&this->m_LogViewCriticalSection);
}



//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

