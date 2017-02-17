// ------------------------------
// Decompiled by Deathway
// Date : 2007-03-09
// ------------------------------
//------------------------------------------
// WinUtil.h	: Status Exactly as GameServer
//------------------------------------------
#ifndef __WINUTIL_H
#define __WINUTIL_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "protocol.h"
#include "user.h"

// Macros

#define MACRO1(value) (( value & 0xFF) & 0xFF)
#define MACRO2(value) (( value & 0xFFFF ) & 0xFFFF)

#define MONTH_MACRO(value) ( (( (value)   )<0)?FALSE:(( (value)  )>12)?FALSE:TRUE  )
#define DAY_MACRO(value) ( (( (value - 1)   )<0)?FALSE:(( (value - 1)  )>30)?FALSE:TRUE  )
#define HOUR_MACRO(value) ( (( (value)   )<0)?FALSE:(( (value)  )>23)?FALSE:TRUE  )
#define MINUTE_MACRO(value) ( (( (value)   )<0)?FALSE:(( (value)  )>59)?FALSE:TRUE  )

//------------------------------------------
// WinUtil.cpp Functions
//------------------------------------------

void BuxConvert(char* buf, int size);
BOOL SQLSyntexCheck(char* SQLString);
BOOL SQLSyntexCheckConvert(char* SQLString);
BOOL SpaceSyntexCheck(char* string);
BOOL StrHangulCheck(char* str);
BOOL PercentSyntaxCheck(char* string);
void FileSaveString(char* filename, char* string);
void ProcessClientHWID(char* input, char* output);
BOOL IsFile(char* filename);
void GetTodayString(char* szDate);
void PHeadSetB(LPBYTE lpBuf, BYTE head, int size);
void PHeadSubSetB(LPBYTE lpBuf, BYTE head, BYTE sub, int size);
void PHeadSetW( LPBYTE lpBuf, BYTE head, int size) ;
void PHeadSubSetW(LPBYTE lpBuf, BYTE head, BYTE sub, int size);
void PHeadSetBE(LPBYTE lpBuf, BYTE head,int size);
void PHeadSubSetBE(LPBYTE lpBuf, BYTE head,BYTE sub, int size);
void PHeadSetWE(LPBYTE lpBuf, BYTE head, int size);
void PHeadSubSetWE(LPBYTE lpBuf, BYTE head, BYTE sub, int size);
void SystemTimeToUnixTime(SYSTEMTIME *systemTime, time_t *unixTime);
void UnixTimeToFileTime(time_t t, LPFILETIME pft);
void UnixTimeToSystemTime(time_t t, LPSYSTEMTIME pst);
int fsize(char* file);
DWORD CalcFileSize(char * szFilePath);

static inline double round(double val)
{
    return floor(val + 0.5);
}

class char_ID
{

public:

	char_ID(LPSTR szName)	// line : 44
	{
		memset(this->Name, 0, sizeof(this->Name));

		if ( szName != NULL )
		{
			memcpy(this->Name, szName, MAX_ACCOUNT_LEN);
		}
	}	// line : 49

	LPSTR GetBuffer()	// line : 52
	{
		return this->Name;
	}	// line : 54

	int  GetLength()	// line : 63
	{
		return strlen(this->Name);
	}	// line : 65

private:

	char Name[MAX_ACCOUNT_LEN+1];	// 0

};



#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

