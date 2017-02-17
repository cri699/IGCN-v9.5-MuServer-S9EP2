//////////////////////////////////////////////////////////////////////
// ExpManager.h
#ifndef EXPMANAGER_H
#define EXPMANAGER_H

struct EXP_RANGES
{
	float fExp;
	float fMLExp;
	int iMinLevel;
	int iMaxLevel;
	int iMinReset;
	int iMaxReset;
};

enum EXP_CALC_TYPES
{
	EXP_CALC_STATIC_ONLY = 0,
	EXP_CALC_STATIC_PLUS_DYNAMIC = 1,
	EXP_CALC_STATIC_MULTIPLE_DYNAMIC = 2,
	EXP_CALC_DYNAMIC_ONLY = 3
};

class CExpManager
{
public: 
	CExpManager();
	~CExpManager();

	void LoadScript(LPSTR file);
	float GetExpMultiplier(int iLevel, int iReset, bool bIsMasterLevel);
	float GetStaticExp() { return this->m_fStaticExp; }
	float GetStaticMLExp() { return this->m_fStaticMLExp; }
	float GetEventExp() { return this->m_fEventExp; }
	float GetQuestExp() { return this->m_fQuestExp; }

private:
	int		m_iExpCalcType;
	bool	m_bDebugMode;
	float	m_fStaticExp;
	float	m_fStaticMLExp;
	float	m_fEventExp;
	float	m_fQuestExp;
	std::vector<EXP_RANGES> m_vtExpRanges;
}; 

extern CExpManager g_ExpManager;

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

