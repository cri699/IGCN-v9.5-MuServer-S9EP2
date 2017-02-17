// ------------------------------
// Decompiled by Deathway
// Date : 2007-03-09
// ------------------------------
// WinUtil.cpp
//	GS-N	1.00.18	JPN	0x004A4E40	-	Completed

#include "stdafx.h"
#include "winutil.h"

static unsigned char bBuxCode[3]={0xFC, 0xCF, 0xAB};	// Xox Key for some interesthing things :)

void BuxConvert(char* buf, int size)
{
	int n;
	for (n=0;n<size;n++)
	{
		buf[n]^=bBuxCode[n%3] ;		// Nice trick from WebZen
	}
}

BOOL SQLSyntexCheck(char* SQLString)
{
	char* temp;

	temp=strchr(SQLString, 39);
	if (temp==0)
	{
		temp=strchr(SQLString, 32);
		if (temp==0)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}
}

BOOL SQLSyntexCheckConvert(char* SQLString)
{
	char tempSQL[256];
	char* temp;
	char* temp2;

	temp2 = SQLString;
	int lOfs=0;
	int tOfs=0;
	int sOfs=0;

	

	if (SQLString==0)
	{
		return FALSE;
	}
	while (true)
	{
		temp = strchr(temp2, 39);
		if (temp != 0 )
		{
			if (temp - temp2 != 0)
			{
				memcpy(&tempSQL[tOfs], temp2, temp-temp2);
			}
			lOfs = temp - (temp2 + sOfs);
			tOfs += lOfs;
			temp2 += lOfs;

			tempSQL[tOfs]=0;

			tOfs += 2;
			temp2++;

			strcat(tempSQL, "''");
			
		}
		else
		{
			strcpy(&tempSQL[tOfs], temp2);
			break;
		}
	}

	
	strcpy(SQLString, tempSQL);		// Strange unseless jump here
	return TRUE;
}

BOOL SpaceSyntexCheck(char* string)
{
	char* temp;
	
	temp = strchr(string, 32);
	if (temp ==0 )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL StrHangulCheck(char* str)
{
	BYTE code1;
	BYTE code2;
	BYTE code;
	int i = 0;
	int n = 0;
	char	szTemp[256] = "";
	int result = 1;
	int count = 0xFF;	// -1

	while ((count--) != 0)
	{
	
		code1=str[i];
		i++;

		if (code1==0)
		{
			break;
		}
		else
		{
			if (code1<128)
			{
				szTemp[n]=code1;
				n++;
				continue;
			}
			else
			{
				code2=str[i];
				i++;
				code=code2 | (code1<<8);

				if ((code)>0)
				{
					szTemp[n]=code1;
					n++;
					szTemp[n]=code2;
					n++;
				}
				else
				{
					result =0;
				}
				if(code2==0)
				{
					break;
				}
			}
		}
	}
	
	
	strcpy(str, szTemp);
	return TRUE;
}

BOOL PercentSyntaxCheck(char* string)
{
	char* temp;

	temp = strchr(string, 37);
	if (temp == 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void FileSaveString(char* filename, char* string)
{
	FILE* fp;
	
	fp=fopen(filename, "at");

	if (fp==0)
	{
		return;
	}
	else
	{
		fprintf(fp, string);
		fprintf(fp, "\n");
		fclose(fp);
	}
}

void ProcessClientHWID(char* input, char* output)
{
	char temp[40];
	memcpy(temp, input, 24);
	temp[24] = 0;

	strcpy(output, temp);
}

BOOL IsFile(char* filename)
{
	FILE *fp;
	
	fp=fopen(filename,"rb");
	if (fp==0)
	{
		return FALSE;
	}
	else
	{
		fclose(fp);
		return TRUE;
	}
}

void GetTodayString(char* szDate)
{
	tm* today;
	time_t ltime;
	

	time(&ltime);
	today=localtime(&ltime);

	today->tm_year = today->tm_year +1900;

	wsprintf(szDate, "%02d%02d%02d", today->tm_year , today->tm_mon +1 , today->tm_mday );
}

void PHeadSetB(LPBYTE lpBuf, BYTE head, int size)
{
	lpBuf[0] =0xC1;		// Packets
	lpBuf[1] =size;
	lpBuf[2] =head;
}
	

void PHeadSubSetB(LPBYTE lpBuf, BYTE head, BYTE sub, int size)
{
	lpBuf[0] =0xC1;	// Packets
	lpBuf[1] =size;
	lpBuf[2] =head;
	lpBuf[3] =sub;
}

void PHeadSetW(LPBYTE lpBuf, BYTE head,  int size) 
{
	lpBuf[0] = 0xC2;	// Packets Header
	lpBuf[1]= SET_NUMBERH(size);
	lpBuf[2]= SET_NUMBERL(size);
	lpBuf[3]= head;
}

void PHeadSubSetW(LPBYTE lpBuf, BYTE head, BYTE sub,  int size)
{
	lpBuf[0]=0xC2;	// packet header
	lpBuf[1]=SET_NUMBERH(size);
	lpBuf[2]=SET_NUMBERL(size);
	lpBuf[3]=head;
	lpBuf[4]=sub;
}

void PHeadSetBE( LPBYTE lpBuf, BYTE head,int size)
{
	lpBuf[0] =0xC3;
	lpBuf[1] =size;
	lpBuf[2] =head;
}

void PHeadSubSetBE(LPBYTE lpBuf, BYTE head,BYTE sub, int size)
{
	lpBuf[0] =0xC3;
	lpBuf[1] =size;
	lpBuf[2] =head;
	lpBuf[3] =sub;
}

void PHeadSetWE(LPBYTE lpBuf, BYTE head, int size)
{
	lpBuf[0] = 0xC4;	// Packets Header
	lpBuf[1] = SET_NUMBERH(size);
	lpBuf[2] = SET_NUMBERL(size);
	lpBuf[3] = head;
}

void PHeadSubSetWE(LPBYTE lpBuf, BYTE head, BYTE sub, int size)
{
	lpBuf[0] = 0xC4;	// packet header
	lpBuf[1] = SET_NUMBERH(size);
	lpBuf[2] = SET_NUMBERL(size);
	lpBuf[3] = head;
	lpBuf[4] = sub;
}

void UnixTimeToFileTime(time_t t, LPFILETIME pft)
{
	// Note that LONGLONG is a 64-bit value
	LONGLONG ll;

	ll = Int32x32To64(t, 10000000) + 116444736000000000;
	pft->dwLowDateTime = (DWORD)ll;
	pft->dwHighDateTime = ll >> 32;
}

void SystemTimeToUnixTime(SYSTEMTIME *systemTime, time_t *unixTime)
{
	LARGE_INTEGER jan1970FT = { 0 };
	jan1970FT.QuadPart = 116444736000000000I64; // january 1st 1970

	LARGE_INTEGER utcFT = { 0 };
	SystemTimeToFileTime(systemTime, (FILETIME*)&utcFT);

	unsigned __int64 utcDosTime = (utcFT.QuadPart - jan1970FT.QuadPart) / 10000000;

	*unixTime = (time_t)utcDosTime;
}

void UnixTimeToSystemTime(time_t t, LPSYSTEMTIME pst)
{
	FILETIME ft;

	UnixTimeToFileTime(t, &ft);
	FileTimeToSystemTime(&ft, pst);
}

int fsize(char* file)
{
	int size;
	FILE* fh;

	fh = fopen(file, "rb"); //binary mode
	if (fh != NULL){
		if (fseek(fh, 0, SEEK_END)){
			fclose(fh);
			return -1;
		}

		size = ftell(fh);
		fclose(fh);
		return size;
	}

	return -1; //error
}

DWORD CalcFileSize(char * szFilePath)
{
	DWORD dwLength = 0;
	HANDLE hFile = NULL;

	hFile = CreateFile(szFilePath, 0, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		dwLength = GetFileSize(hFile, 0);

		CloseHandle(hFile);
	}

	return dwLength;
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

