//////////////////////////////////////////////////////////////////////
// MuunInfo.h
#ifndef MUUNINFO_H
#define MUUNINFO_H
#include "MuunOpt.h"

class CMuunInfo : public CMuunOpt
{
public:
	CMuunInfo();
	~CMuunInfo();

	void SetIndex(int iIndex);
	int GetIndex();

	void SetItemNum(int iItemNum);
	int GetItemNum();

	void SetMuunType(int iMuunType);
	int GetMuunType();

	void SetMuunRank(int iMuunRank);
	int GetMuunRank();

	void SetOptIndex(int iOptIndex);
	int GetOptIndex();

	void SetAddOptType(int iAddOptType);
	int GetAddOptType();

	void SetAddOptVal(int iAddOptVal);
	int GetAddOptVal();

	void SetAddOptStart(time_t lAddOptStart);
	time_t GetAddOptStart();

	void SetAddOptEnd(time_t lAddOptEnd);
	time_t GetAddOptEnd();

	void SetEvolutionMuunNum(int iEvolutionMuunNum);
	int GetEvolutionMuunNum();

	void SetDelayTime(int iDelayTime);
	int GetDelayTime();

	void Clear();

private:
	int m_iIndex;
	int m_iItemNum;
	int m_iMuunType;
	int m_iMuunRank;
	int m_iOptIndex;
	int m_iAddOptType;
	int m_iAddOptVal;
	time_t m_lAddOptStart;
	time_t m_lAddOptEnd;
	int m_iEvolutionMuunNum;
	int m_iDelayTime;
};
#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

