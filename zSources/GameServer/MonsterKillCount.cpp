//////////////////////////////////////////////////////////////////////
// MonsterKillCount.cpp
#include "StdAfx.h"
#include "user.h"
#include "MonsterKillCount.h"
#include "DSProtocol.h"

CKillCountMng g_KillCountMng;

CKillCountMng::CKillCountMng()
{
	memset(i_MonsterIds,-1,sizeof(i_MonsterIds));
}

CKillCountMng::~CKillCountMng()
{

}

void CKillCountMng::CheckMonsterKillCount(int monsterid, LPOBJ lpObj)
{
	for(int i=0;i<MAX_MONSTER_TYPE;i++)
	{
		if(this->i_MonsterIds[i] == monsterid)
		{
			ReqSaveMonsterCount(lpObj->m_Index,monsterid);
			break;
		}
	}
}

void CKillCountMng::ReadConfig(LPSTR szFile)
{
	memset(i_MonsterIds, -1, sizeof(i_MonsterIds));

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(szFile);

	if (res.status != pugi::status_ok)
	{
		g_Log.MsgBox("Error loading %s file (%s)", szFile, res.description());
		return;
	}

	pugi::xml_node main = file.child("MonsterKillCount");
	int cnt = 0;

	for (pugi::xml_node monster = main.child("Monster"); monster; monster = monster.next_sibling())
	{
		int iMonsterIndex = monster.attribute("Index").as_int();
		bool bEnable = monster.attribute("Enable").as_bool();

		if (bEnable == true)
		{
			this->i_MonsterIds[cnt] = iMonsterIndex;
			cnt++;
		}
	}
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

