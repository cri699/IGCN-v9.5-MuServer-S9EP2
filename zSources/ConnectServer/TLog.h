//////////////////////////////////////////////////////////////////////
// TLog.h
#ifndef LOGPROC_H
#define LOGPROC_H

#define LOG_TEXT_LINE	90
#define LOG_TEXT_LENGTH	200

#include "TColor.h"

typedef struct
{
    int Day;
    int Month;
    int Year;
} CTLDATE;

typedef struct
{
    char		Text[LOG_TEXT_LENGTH];
    size_t		Lenght;
    TColor::T	Color;
} CTLVISIBLESTRUCT;

typedef struct
{
    HBRUSH					StandByBG;
    COLORREF				StandByText;
} STUATS_COLORS_INFO;

class TLog
{
private:
	CTLVISIBLESTRUCT		m_LogStr[LOG_TEXT_LINE];

	HFONT					m_HFont;
	STUATS_COLORS_INFO		m_StatusColors;

	int						m_iCurrentLine; // Current Line of Log;
	FILE* 					m_fLog; // File Variable for Log;

	char					m_szLogFileName[255];
	char					m_szLogFileName_Orig[255];

	DWORD					m_dwMaxLogFileSize;

	CRITICAL_SECTION		m_LogFileCriticalSection;
	CRITICAL_SECTION		m_LogViewCriticalSection;

	CTLDATE					m_StartDate;
	CTLDATE					m_CurDate;

	bool					m_bWriteLog;

	void LogDataSet_Start();
	void LogDataSet_Check();

	void LogTextAdd(TColor::T tColor, char * szMsg, size_t iLen);

public:
	TLog();
	virtual ~TLog();

	void LogInit(bool bLogPrint, int iMaxLogFileSize);

	void LogTextClear();
	bool LogDateChange();

	void Add(char * szLog, ...);
	void AddC(TColor::T tColor, char * szLog, ...);
	void AddF(char * szLog, ...);
	void AddL(TColor::T tColor, char * szLog, ...);
	void AddHeadHex(char * szText, LPBYTE lpBuf, int iSize);
	void LogTextPaint(HWND hWnd);

	void MsgBox(char * szlog, ...);
	int MsgBox(UINT uType, char * szCaption, char * szlog, ...);
	void AddMSB(char * szLog, ...);

	void SetMaxLogFileSize(USHORT usMaxMBSize)
	{
		EnterCriticalSection(&this->m_LogFileCriticalSection);

		m_dwMaxLogFileSize = (DWORD)usMaxMBSize * (DWORD)(1024 * 1024);

		LeaveCriticalSection(&this->m_LogFileCriticalSection);
	};
};

extern TLog			g_Log;

#endif



//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

