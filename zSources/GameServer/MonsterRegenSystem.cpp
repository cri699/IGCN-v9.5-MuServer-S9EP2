//////////////////////////////////////////////////////////////////////
// MonsterRegenSystem.cpp
#include "StdAfx.h"
#include "user.h"
#include "MonsterRegenSystem.h"
#include "TLog.h"
#include "MapClass.h"
#include "GameMain.h"
#include "ArcaBattle.h"
#include "TNotice.h"
#include "configread.h"
#include "BagManager.h"

CMonsterRegenSystem g_MonsterRegenSystem;

CMonsterRegenSystem::CMonsterRegenSystem(void)
{
}

CMonsterRegenSystem::~CMonsterRegenSystem(void)
{
}

void CMonsterRegenSystem::LoadScript(char* lpFileName)
{
	this->m_bLoadScript = false;

	if ( !lpFileName )
	{
		g_Log.Add("[MonsterGroupRegen] lpFileName is null!!");
		return;
	}

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(lpFileName);

	if ( res.status != pugi::status_ok )
	{
		g_Log.AddC(TColor::Red, "[%s] file load error [%s]", lpFileName, res.description());
		return;
	}

	pugi::xml_node main_section = file.child("MonsterGroupRegenSystem");
	pugi::xml_node group_settings = main_section.child("GroupSettings");
	pugi::xml_node spot_settings = main_section.child("SpotSettings");
	pugi::xml_node monster_settings = main_section.child("MonsterSettings");
	pugi::xml_node spawn_settings = main_section.child("SpawnSettings");

	this->m_bSendMessage = main_section.attribute("SpawnNotice").as_int();

	for (pugi::xml_node group = group_settings.child("Group"); group; group = group.next_sibling())
	{
		int nGroupNumber = group.attribute("Index").as_int();

		if (nGroupNumber < 0 || nGroupNumber > MAX_MONSTER_GROUP_INFO)
		{
			g_Log.Add("[MonsterGroupRegen] Invalid monster groupNumber");
			return;
		}

		this->m_stMonsterGroupInfo[nGroupNumber].m_nGroupNumber = nGroupNumber;
		this->m_stMonsterGroupInfo[nGroupNumber].m_nMapNumber = group.attribute("MapNumber").as_int();
		this->m_stMonsterGroupInfo[nGroupNumber].m_nBossMonsterClass = group.attribute("BossMonsterIndex").as_int();
		this->m_stMonsterGroupInfo[nGroupNumber].m_nDuration = group.attribute("Duration").as_int();
		this->m_stMonsterGroupInfo[nGroupNumber].m_sSpawnNotice = group.attribute("SpawnNotice").as_string();
	}
					
	//int nTempGroupNumber = 0;

	for (pugi::xml_node group = spot_settings.child("Group"); group; group = group.next_sibling())
	{
		int nGroupNumber = group.attribute("Index").as_int();

		if (nGroupNumber < 0 || nGroupNumber > MAX_MONSTER_GROUP_INFO)
		{
			g_Log.Add("[MonsterGroupRegen] Invalid Monster groupNumber");
			return;
		}
		int nAreaCnt = 0;

		for(pugi::xml_node area = group.child("Area"); area; area = area.next_sibling())
		{
			this->m_stMonsterPosition[nGroupNumber].m_nGroupNumber = nGroupNumber;
			this->m_stMonsterPosition[nGroupNumber].m_stRegenArea[nAreaCnt].m_nBeginX = area.attribute("StartX").as_int();
			this->m_stMonsterPosition[nGroupNumber].m_stRegenArea[nAreaCnt].m_nBeginY = area.attribute("StartY").as_int();
			this->m_stMonsterPosition[nGroupNumber].m_stRegenArea[nAreaCnt].m_nEndX = area.attribute("EndX").as_int();
			this->m_stMonsterPosition[nGroupNumber].m_stRegenArea[nAreaCnt].m_nEndY = area.attribute("EndY").as_int();
			nAreaCnt++; 
		}
		this->m_stMonsterPosition[nGroupNumber].m_nRegenAreaCnt = nAreaCnt;
	}


	for (pugi::xml_node group = monster_settings.child("Group"); group; group = group.next_sibling())
	{
		int nGroupNumber = group.attribute("Index").as_int();

		if (nGroupNumber < 0 || nGroupNumber > MAX_MONSTER_GROUP_INFO)
		{
			g_Log.Add("[MonsterGroupRegen] Invalid Monster groupNumber");
			return;
		}

		if (this->m_stMonsterPosition[nGroupNumber].m_nGroupNumber < 0)
		{
			g_Log.Add("[MonsterGroupRegen] Invalid groupNumber(%d) ", nGroupNumber);
			return;
		}
		int nArrayIndex = 0;

		for(pugi::xml_node monster = group.child("Monster"); monster; monster = monster.next_sibling())
		{
			this->m_stMonsterPosition[nGroupNumber].m_stMonsterIndexInfo[nArrayIndex].m_nMonsterIndex = monster.attribute("Index").as_int();
			this->m_stMonsterPosition[nGroupNumber].m_stMonsterIndexInfo[nArrayIndex].m_nCnt = monster.attribute("Count").as_int();
			this->m_stMonsterPosition[nGroupNumber].m_stMonsterIndexInfo[nArrayIndex].m_nOverrideDefaultSettings = monster.attribute("OverrideDefaultSettings").as_int();
			this->m_stMonsterPosition[nGroupNumber].m_stMonsterIndexInfo[nArrayIndex].m_nEventId = monster.attribute("EventID").as_int();
			this->m_stMonsterPosition[nGroupNumber].m_stMonsterIndexInfo[nArrayIndex].m_nHP = monster.attribute("HP").as_int();
			this->m_stMonsterPosition[nGroupNumber].m_stMonsterIndexInfo[nArrayIndex].m_nDamageMin = monster.attribute("DamageMin").as_int();
			this->m_stMonsterPosition[nGroupNumber].m_stMonsterIndexInfo[nArrayIndex].m_nDamageMax = monster.attribute("DamageMax").as_int();
			this->m_stMonsterPosition[nGroupNumber].m_stMonsterIndexInfo[nArrayIndex].m_nDefense = monster.attribute("Defense").as_int();
			this->m_stMonsterPosition[nGroupNumber].m_stMonsterIndexInfo[nArrayIndex].m_nAttackRate = monster.attribute("AttackRate").as_int();
			this->m_stMonsterPosition[nGroupNumber].m_stMonsterIndexInfo[nArrayIndex].m_nDefenseRate = monster.attribute("DefenseRate").as_int();
			nArrayIndex++; 
		}

		if (nArrayIndex >= MAX_MONSTER_INDEX_INFO)
		{
			g_Log.Add("[MonsterGroupRegen] Out of Max MonsterKindIndex");
			return;
		}
	}

	for (pugi::xml_node group = spawn_settings.child("Group"); group; group = group.next_sibling())
	{
		int nGroupNumber = group.attribute("Index").as_int();

		if (nGroupNumber < 0 || nGroupNumber > MAX_MONSTER_GROUP_INFO)
		{
			g_Log.Add("[MonsterGroupRegen] Invalid Monster groupNumber");
			return;
		}

		if (this->m_stMonsterGroupInfo[nGroupNumber].m_nGroupNumber < 0)
		{
			g_Log.Add("[MonsterGroupRegen] Invalid groupNumber(%d) ", nGroupNumber);
			return;
		}
		int nTimeArrayIndex = 0;

		for(pugi::xml_node spawn = group.child("Spawn"); spawn; spawn = spawn.next_sibling())
		{
			this->m_stMonsterGroupInfo[nGroupNumber].m_stRegenTimeTable[nTimeArrayIndex].m_nBeginHour = spawn.attribute("StartHour").as_int();
			this->m_stMonsterGroupInfo[nGroupNumber].m_stRegenTimeTable[nTimeArrayIndex].m_nBeginMinute = spawn.attribute("StarMinute").as_int();
			this->m_stMonsterGroupInfo[nGroupNumber].m_stRegenTimeTable[nTimeArrayIndex].m_nAppearanceRate = spawn.attribute("AppearanceRate").as_int();
			nTimeArrayIndex++; 
		}

		if (nTimeArrayIndex >= MAX_REGEN_TIME_TABLE)
		{
			g_Log.Add("[MonsterGroupRegen] Out of Max TimeTable");
			return;
		}
	}

	g_Log.AddC(TColor::DarkOrange,"[%s]::FileLoad -> Success!", lpFileName);
	this->m_bLoadScript = true;
}

void CMonsterRegenSystem::Run() // used SYSTETIME instead of ATL::CTime, we dont use MFC
{
	if ( !this->m_bLoadScript )
	{
		return;
	}

	if ( g_ArcaBattle.IsArcaBattleServer() )
	{
		return;
	}

	UINT64 i64Tick = GetTickCount64();
	SYSTEMTIME m_Time;

	GetLocalTime(&m_Time);

	if ( this->m_nDay != m_Time.wDay )
	{
		this->m_nDay = m_Time.wDay;
	}

	for ( int i=0; i < MAX_MONSTER_GROUP_INFO && this->m_stMonsterGroupInfo[i].m_nGroupNumber > -1; i++ )
	{
		if (this->IsRegenTime(i, m_Time.wHour, m_Time.wMinute, 1) == TRUE)
		{
		
			this->DeleteMonster(i);
			this->RegenMonster(i);
			g_Log.Add("[MonsterGroupRegen] regen attempt [GROUP]:%d", i);
		}
		
	}
}

void CMonsterRegenSystem::RegenMonster(int nGroupNumber)
{
	if ( nGroupNumber < 0 || nGroupNumber > MAX_MONSTER_GROUP_INFO-1 )
	{
		g_Log.Add("error : %s %d", __FILE__, __LINE__);
		return;
	}

	int nRegenArea = rand() % this->m_stMonsterPosition[nGroupNumber].m_nRegenAreaCnt;
	PMSG_NOTICE pNotice;
	char szMsg[256];
	 
	WORD nMapNumber = this->m_stMonsterGroupInfo[nGroupNumber].m_nMapNumber;
	WORD nMonsterArrayIndex = 0;

	for ( int k=0; k<MAX_MONSTER_INDEX_INFO; k++ )
	{
		int nMonsterIndex = this->m_stMonsterPosition[nGroupNumber].m_stMonsterIndexInfo[k].m_nMonsterIndex;
		int nMonsterCount = this->m_stMonsterPosition[nGroupNumber].m_stMonsterIndexInfo[k].m_nCnt;
		int nBeginX = this->m_stMonsterPosition[nGroupNumber].m_stRegenArea[nRegenArea].m_nBeginX;
		int nBeginY = this->m_stMonsterPosition[nGroupNumber].m_stRegenArea[nRegenArea].m_nBeginY;
		int nEndX = this->m_stMonsterPosition[nGroupNumber].m_stRegenArea[nRegenArea].m_nEndX;
		int nEndY = this->m_stMonsterPosition[nGroupNumber].m_stRegenArea[nRegenArea].m_nEndY;

		for ( int n=0; n<nMonsterCount && nMapNumber != -1; n++ )
		{
			int nResult = gObjAddMonster(nMapNumber);
			this->m_stMonsterGroupInfo[nGroupNumber].m_stMonsterAccountNumInfo[nMonsterArrayIndex].m_nIndex = nResult;

			if ( nResult < 0 )
			{
				g_Log.Add("[MonsterGroupRegen] Add Monster fail!! [GROUP]: %d, [INDEX]: %d", nGroupNumber, nMonsterIndex);
			}

			else
			{
				if ( this->SetPosMonster(nResult, nMapNumber, nBeginX, nBeginY, nEndX, nEndY) == true )
				{
					if ( gObjSetMonster(nResult, nMonsterIndex) == TRUE )
					{
						if (this->m_stMonsterPosition[nGroupNumber].m_stMonsterIndexInfo[k].m_nOverrideDefaultSettings == TRUE)
						{
							gObj[nResult].m_AttackDamageMin = this->m_stMonsterPosition[nGroupNumber].m_stMonsterIndexInfo[k].m_nDamageMin;
							gObj[nResult].m_AttackDamageMax = this->m_stMonsterPosition[nGroupNumber].m_stMonsterIndexInfo[k].m_nDamageMax;
							gObj[nResult].m_Defense = this->m_stMonsterPosition[nGroupNumber].m_stMonsterIndexInfo[k].m_nDefense;
							gObj[nResult].Life = this->m_stMonsterPosition[nGroupNumber].m_stMonsterIndexInfo[k].m_nHP;
							gObj[nResult].m_AttackRating = this->m_stMonsterPosition[nGroupNumber].m_stMonsterIndexInfo[k].m_nAttackRate;
							gObj[nResult].m_SuccessfulBlocking = this->m_stMonsterPosition[nGroupNumber].m_stMonsterIndexInfo[k].m_nDefenseRate;
						}

						nMonsterArrayIndex++;
						this->m_stMonsterGroupInfo[nGroupNumber].m_nLiveCount++;
						if ( gObj[nResult].Class == this->m_stMonsterGroupInfo[nGroupNumber].m_nBossMonsterClass )
						{
							this->m_stMonsterGroupInfo[nGroupNumber].m_nBossMonsterIndex = nResult;

							
							if (this->m_bSendMessage == true)
							{
								LPMONSTER_ATTRIBUTE lpm = gMAttr.GetAttr(gObj[nResult].Class);

								TNotice::MakeNoticeMsg(&pNotice, 0, (char*)this->m_stMonsterGroupInfo[nGroupNumber].m_sSpawnNotice.c_str());

								for (int i = g_ConfigRead.server.GetObjectStartUserIndex(); i < g_ConfigRead.server.GetObjectMax(); i++)
								{
									if (gObj[i].Connected == PLAYER_PLAYING && gObj[i].Type == OBJ_USER)
									{
										IOCP.DataSend(i, (LPBYTE)&pNotice, pNotice.h.size);
									}
								}
							}
						}

						g_Log.Add("[MonsterGroupRegen] Group: %d, MonsterIndex: %d, MapNumber: %d, X: %d, Y: %d", nGroupNumber, nMonsterIndex, nMapNumber, gObj[nResult].X, gObj[nResult].Y);
					}

					else
					{
						gObjDel(nResult);
						g_Log.Add("error : %s %d", __FILE__, __LINE__);
					}
				}

				else
				{
					gObjDel(nResult);
					g_Log.Add("error : %s %d", __FILE__, __LINE__);
				}
			}
		}
	}
}

bool CMonsterRegenSystem::SetPosMonster(int aIndex, int nMapNumber, int nBeginX, int nBeginY, int nEndX, int nEndY)
{
	if ( !ObjectMaxRange(aIndex) )
	{
		g_Log.Add("error : %s %d", __FILE__, __LINE__);
		return false;
	}

	LPOBJ lpObj = &gObj[aIndex];

	lpObj->m_PosNum = -1;
	lpObj->MapNumber = nMapNumber;
	this->GetBoxPosition(nMapNumber, nBeginX, nBeginY, nEndX, nEndY, lpObj->X, lpObj->Y);
	lpObj->TX = lpObj->X;
	lpObj->TY = lpObj->Y;
	lpObj->m_OldX = lpObj->X;
	lpObj->m_OldY = lpObj->Y;
	lpObj->Dir = rand()%8;
	lpObj->StartX = lpObj->X;
	lpObj->StartY = lpObj->Y;

	return true;
}

bool CMonsterRegenSystem::GetBoxPosition(int mapnumber, int ax, int ay, int aw, int ah, short &mx, short &my)
{
	int count = 100 ; 
	int w;
	int h;
	int tx; 
	int ty;
	BYTE attr;

	while ( count-- != 0)
	{
		w = aw - ax;
		h = ah - ay;

		__try
		{
			tx = ax + (rand()%w);
			ty = ay + (rand()%h);
		}
		__except (w=1,h=1,1)
		{

		}

		attr = MapC[mapnumber].GetAttr(tx, ty);

		if ( ((attr&1) != 1) && ((attr &4) != 4) && ((attr&8) != 8) )
		{
			mx = tx;
			my = ty;
			return true;
		}
	}

	return false;
}

bool CMonsterRegenSystem::IsLiveBossState(int nGroupNumber)
{
	if ( nGroupNumber < 0 || nGroupNumber > MAX_MONSTER_GROUP_INFO-1 )
	{
		return false;
	}

	int nBossMonsterIndex = this->m_stMonsterGroupInfo[nGroupNumber].m_nBossMonsterIndex;

	if ( !ObjectMaxRange(nBossMonsterIndex) )
	{
		return false;
	}

	if ( gObj[nBossMonsterIndex].Live == TRUE )
	{
		return true;
	}

	return false;
}

void CMonsterRegenSystem::DeleteMonster(int nGroupNumber)
{
	if ( nGroupNumber < 0 || nGroupNumber > MAX_MONSTER_GROUP_INFO-1 )
	{
		return;
	}

	for ( int i=0; i < MAX_MONSTER_ACCOUNT_NUM_INFO && this->m_stMonsterGroupInfo[nGroupNumber].m_stMonsterAccountNumInfo[i].m_nIndex > -1; i++ )
	{
		gObjDel(this->m_stMonsterGroupInfo[nGroupNumber].m_stMonsterAccountNumInfo[i].m_nIndex);
		g_Log.Add("[MonsterGroupRegen] Delete Monster - %d", this->m_stMonsterGroupInfo[nGroupNumber].m_stMonsterAccountNumInfo[i].m_nIndex);
		this->m_stMonsterGroupInfo[nGroupNumber].m_stMonsterAccountNumInfo[i].m_nIndex = -1;
	}
}

void CMonsterRegenSystem::SendAllUserAnyMsg(int iType, char *lpszMsg) // NOT USED IN 1.01.04 GS
{

}


bool CMonsterRegenSystem::IsRegenTime(int nGroupNumber, int nCurHour, int nCurMin, bool bSetRegen)
{
	for ( int i=0; i<MAX_REGEN_TIME_TABLE; i++ )
	{
		if ( this->m_stMonsterGroupInfo[nGroupNumber].m_stRegenTimeTable[i].m_nBeginHour == nCurHour &&
			this->m_stMonsterGroupInfo[nGroupNumber].m_stRegenTimeTable[i].m_nBeginMinute == nCurMin )
		{
			if (this->m_stMonsterGroupInfo[nGroupNumber].m_stRegenTimeTable[i].m_bRegen == false)
			{
				if (bSetRegen && !this->m_stMonsterGroupInfo[nGroupNumber].m_stRegenTimeTable[i].m_bFailedAppearanceRate)
				{
					if (rand() % 100 < this->m_stMonsterGroupInfo[nGroupNumber].m_stRegenTimeTable[i].m_nAppearanceRate)
					{
						this->m_stMonsterGroupInfo[nGroupNumber].m_stRegenTimeTable[i].m_bRegen = true;
						return true;
					}
					else
					{
						this->m_stMonsterGroupInfo[nGroupNumber].m_stRegenTimeTable[i].m_bFailedAppearanceRate = true;
						g_Log.Add("[MonsterRegenSystem] Invasion time has arrived but Appearance rate was unsuccesful");
						return false;
					}

				}
			}
		}
		else if (this->m_stMonsterGroupInfo[nGroupNumber].m_stRegenTimeTable[i].m_bFailedAppearanceRate)
		{
			this->m_stMonsterGroupInfo[nGroupNumber].m_stRegenTimeTable[i].m_bFailedAppearanceRate = false;
		}
		else if (this->m_stMonsterGroupInfo[nGroupNumber].m_stRegenTimeTable[i].m_bRegen == true)
		{
			this->m_stMonsterGroupInfo[nGroupNumber].m_stRegenTimeTable[i].m_bRegen = false;
		}

	}
	return false;
}

bool CMonsterRegenSystem::MonsterKillCheck(LPOBJ lpPlayer, LPOBJ lpMonster)
{
	for (int i = 0; i < MAX_MONSTER_GROUP_INFO && this->m_stMonsterGroupInfo[i].m_nGroupNumber > -1; i++)
	{
		if (this->m_stMonsterGroupInfo[i].m_nLiveCount > 0)
		{
			for (int n = 0; n<MAX_MONSTER_ACCOUNT_NUM_INFO && this->m_stMonsterGroupInfo[i].m_stMonsterAccountNumInfo[n].m_nIndex > -1; n++)
			{
				if (this->m_stMonsterGroupInfo[i].m_stMonsterAccountNumInfo[n].m_nIndex == lpMonster->m_Index)
				{
					for (int k = 0; k < MAX_MONSTER_INDEX_INFO; k++)
					{
						if (this->m_stMonsterPosition[i].m_stMonsterIndexInfo[k].m_nMonsterIndex == lpMonster->Class)
						{
							if (this->m_stMonsterPosition[i].m_stMonsterIndexInfo[k].m_nOverrideDefaultSettings == TRUE)
							{
								g_BagManager.SearchAndUseBag(lpPlayer->m_Index, BAG_EVENT, this->m_stMonsterPosition[i].m_stMonsterIndexInfo[k].m_nEventId, lpMonster->m_Index);
								this->m_stMonsterGroupInfo[i].m_nLiveCount--;
								return true;
							}
						}
					}
				}
			}
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

