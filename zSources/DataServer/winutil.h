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

//------------------------------------------
// WinUtil.cpp Functions
//------------------------------------------

void BuxConvert(char* buf, int size);
BOOL QuoteSpaceSyntexCheck(char* string);
BOOL SQLSyntexCheck(char* SQLString);
BOOL SQLSyntexCheckConvert(char* SQLString);
BOOL SpaceSyntexCheck(char* string);
BOOL PercentSyntaxCheck(char* string);
BOOL StrHangulCheck(char* str);
void FileSaveString(char* filename, char* string);
BOOL IsFile(char* filename);
void GetTodayString(char* szDate);
void PHeadSetB(LPBYTE lpBuf, BYTE head, int size);
void PHeadSubSetB(LPBYTE lpBuf, BYTE head, BYTE sub, int size);
void PHeadSetW( LPBYTE lpBuf, BYTE head, int size) ;
void PHeadSubSetW(LPBYTE lpBuf, BYTE head, BYTE sub, int size);
void PHeadSetBE(LPBYTE lpBuf, BYTE head,int size);
void PHeadSubSetBE(LPBYTE lpBuf, BYTE head,BYTE sub, int size);
void ParseHardwareID(char *HWID);
char* ValidateAndResolveIP(char* input);
DWORD CalcFileSize(char * szFilePath);

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

	size_t GetLength()	// line : 63
	{
		return strlen(this->Name);
	}	// line : 65

private:

	char Name[MAX_ACCOUNT_LEN+1];	// 0

};

inline unsigned __int64 to_integral(const SYSTEMTIME& st)
{
	FILETIME ft;
	SystemTimeToFileTime(&st, &ft);
	ULARGE_INTEGER integer;
	integer.LowPart = ft.dwLowDateTime;
	integer.HighPart = ft.dwHighDateTime;
	return integer.QuadPart;
}
inline bool operator== (const SYSTEMTIME& a, const SYSTEMTIME& b)
{
	return to_integral(a) == to_integral(b);
}
inline bool operator< (const SYSTEMTIME& a, const SYSTEMTIME& b)
{
	return to_integral(a) < to_integral(b);
}
inline bool operator<= (const SYSTEMTIME& a, const SYSTEMTIME& b)
{
	return to_integral(a) <= to_integral(b);
}
inline bool operator!= (const SYSTEMTIME& a, const SYSTEMTIME& b)
{
	return !(a == b);
}
inline bool operator> (const SYSTEMTIME& a, const SYSTEMTIME& b)
{
	return !(a <= b);
}
inline bool operator>= (const SYSTEMTIME& a, const SYSTEMTIME& b)
{
	return !(a<b);
}



#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

