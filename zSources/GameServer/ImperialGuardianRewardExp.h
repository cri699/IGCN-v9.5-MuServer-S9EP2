//////////////////////////////////////////////////////////////////////
// ImperialGuardianRewardExp.h
#pragma once

struct _stRewardExpInfo
{
	_stRewardExpInfo()
	{
		this->m_nMinLevel = 0;
		this->m_nMaxLevel = 0;
		this->m_nRewardExp = 0;
	}

	int m_nMinLevel;
	int m_nMaxLevel;
	int m_nRewardExp;
};

class CImperialGuardianRewardExp
{
public:
	CImperialGuardianRewardExp(void);
	virtual ~CImperialGuardianRewardExp(void);

	void LoadScript(const char* lpFileName);
	int GetRewardExp(int nUserIndex);

private:
	void Init();

	bool m_bLoadScript;
	std::vector<_stRewardExpInfo> m_vtRewardExpInfo;

};



//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

