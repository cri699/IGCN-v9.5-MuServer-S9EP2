//////////////////////////////////////////////////////////////////////
// MuunInfoMng.h
#ifndef MUUNINFOMNG_H
#define MUUNINFOMNG_H

#include "MuunInfo.h"

#define MAX_MUUN_ITEM 100

class CMuunInfoMng
{
public:
	CMuunInfoMng();
	~CMuunInfoMng();

	bool LoadScriptMuunSystemInfo(char *lpszFileName);
	bool LoadScriptMuunSystemOption(const char *lpszFileName);

	bool InsertMuunOptionInfo(CMuunOpt *pCMuunOpt);
	time_t ConvertStringToTime(const char *pchTime);

	CMuunInfo * GetMuunInfo(int iIndex);
	CMuunInfo * GetMuunItemNumToMuunInfo(int iMuunItemNum);

	int GetBeforeEvolutionMuunItemIndex(int iItemNum);

private:
	CMuunInfo m_CMuunInfo[MAX_MUUN_ITEM];
};
#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

