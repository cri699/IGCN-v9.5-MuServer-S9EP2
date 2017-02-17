//////////////////////////////////////////////////////////////////////
// MonsterKillCount.h
#ifndef MONSTER_KILL_COUNT_H
#define MONSTER_KILL_COUNT_H


class CKillCountMng
{
public:
	CKillCountMng();
	~CKillCountMng();

	void ReadConfig(LPSTR szFile);

	void CheckMonsterKillCount(int monsterid, LPOBJ lpObj);

private:
	int i_MonsterIds[MAX_MONSTER_TYPE];
	
};
extern CKillCountMng g_KillCountMng;
#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

