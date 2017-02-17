//////////////////////////////////////////////////////////////////////
// DevilSquare.cpp
// ------------------------------
// Decompiled by Deathway
// Date : 2007-05-09
// ------------------------------
// GS-N 0.99.60T 0x00453780
// GS-N	1.00.18	JPN	0x00462B60	-	Completed
#include "stdafx.h"
#include "DevilSquare.h"
#include "TLog.h"
#include "GameMain.h"
#include "TNotice.h"
#include "gObjMonster.h"
#include "BloodCastle.h"
#include "ChaosCastle.h"
#include "winutil.h"
#include "CrywolfSync.h"
#include "MasterLevelSkillTreeSystem.h"
#include "configread.h"
#include "BuffEffectSlot.h"
#include "VipSys.h"
#include "QuestExpProgMng.h"
#include "BonusEvent.h"
#include "ExpManager.h"
#include "MapAttribute.h"
#include "LargeRand.h"

CDevilSquare g_DevilSquare;

CDevilSquare::CDevilSquare()
{
	this->m_bQuit = FALSE;
	this->m_iCloseTime = 2;
	this->m_iOpenTime = 2;
	this->m_iPlaytime = 10;
	this->m_BonusScoreTable[0][0] = 0;
	this->m_BonusScoreTable[0][1] = 0;
	this->m_BonusScoreTable[0][2] = 0;
	this->m_BonusScoreTable[0][3] = 170;
	
	this->m_BonusScoreTable[1][0] = 20;
	this->m_BonusScoreTable[1][1] = 90;
	this->m_BonusScoreTable[1][2] = 120;
	this->m_BonusScoreTable[1][3] = 400;

	this->m_BonusScoreTable[2][0] = 10;
	this->m_BonusScoreTable[2][1] = 10;
	this->m_BonusScoreTable[2][2] = 10;
	this->m_BonusScoreTable[2][3] = 200;

	this->m_BonusScoreTable[3][0] = 0;
	this->m_BonusScoreTable[3][1] = 0;
	this->m_BonusScoreTable[3][2] = 0;
	this->m_BonusScoreTable[3][3] = 0;

	this->m_BonusScoreTable[0][4] = 170;
	this->m_BonusScoreTable[0][5] = 170;

	this->m_BonusScoreTable[1][4] = 400;
	this->m_BonusScoreTable[1][5] = 400;

	this->m_BonusScoreTable[2][4] = 200;
	this->m_BonusScoreTable[2][5] = 200;

	this->m_BonusScoreTable[3][4] = 0;
	this->m_BonusScoreTable[3][5] = 0;

	this->m_BonusScoreTable[4][0] = 0;
	this->m_BonusScoreTable[4][1] = 0;
	this->m_BonusScoreTable[4][2] = 0;
	this->m_BonusScoreTable[4][3] = 0;
	this->m_BonusScoreTable[4][4] = 0;
	this->m_BonusScoreTable[4][5] = 0;
}



CDevilSquare::~CDevilSquare()
{
	this->m_bQuit = TRUE;
}



void CDevilSquare::Init()
{
	this->m_bQuit = TRUE;

	boost::posix_time::milliseconds sleep_time(500);
	boost::this_thread::sleep(sleep_time);

	if (g_ConfigRead.server.GetStateFromEventTable(g_ConfigRead.server.GetServerType(), EV_TABLE_DS) == false)
	{
		this->SetEventEnable(false);
	}

	if (this->IsEventEnable() == false)
	{
		this->SetState(DevilSquare_NONE);
	}

	else
	{
		this->SetState(DevilSquare_CLOSE);
	}

	this->m_bQuit = FALSE;
}


void CDevilSquare::Load(char * filename)
{
	for ( int i=0;i<MAX_DEVILSQUARE_GROUND;i++)
	{
		this->m_DevilSquareGround[i].Init(i);
	}

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(filename);

	if ( res.status != pugi::status_ok )
	{
		g_Log.MsgBox("[DevilSquare] Info file Load Fail [%s] [%s]", filename, res.description());
		return;
	}

	pugi::xml_node main = file.child("DevilSquare_Classic");

	bool bEnable = main.attribute("Enable").as_bool();

	if (g_ConfigRead.server.GetStateFromEventTable(g_ConfigRead.server.GetServerType(), EV_TABLE_DS) == false)
	{
		bEnable = false;
	}

	this->SetEventEnable(bEnable);
	this->SetMaxUserInSquare(main.attribute("MaxPlayerPerRoom").as_int());
	this->SetEyeDropRate(main.attribute("DevilsEyeDropRate").as_int());
	this->SetKeyDropRate(main.attribute("DevilsKeyDropRate").as_int());

	pugi::xml_node time = main.child("Time");

	this->m_iCloseTime = time.attribute("Close").as_int();
	this->m_iOpenTime = time.attribute("ToOpen").as_int();
	this->m_iPlaytime = time.attribute("PlayDuration").as_int();

	pugi::xml_node schedule = main.child("Schedule");

	for (pugi::xml_node start = schedule.child("Start"); start; start = start.next_sibling())
	{
		DEVILSQUARE_TIME Time = {0};

		Time.m_iHOUR = start.attribute("Hour").as_int();
		Time.m_iMINUTE = start.attribute("Minute").as_int();
		
		this->m_vtDevilSquareTime.push_back(Time);
	}

	pugi::xml_node monster_settings = main.child("MonsterAppearanceSettings");

	for (pugi::xml_node square = monster_settings.child("Square"); square; square = square.next_sibling())
	{
		int nSquare = square.attribute("Level").as_int();
		int nMonsterType = square.attribute("MonsterIndex").as_int();
		int nStartTime = square.attribute("SpawnStartTime").as_int();
		int nEndTime = square.attribute("SpawnEndTime").as_int();

		if ( nSquare < 0 || nSquare >= MAX_DEVILSQUARE_GROUND )
		{
			g_Log.MsgBox("[DevilSquare] Invalid Square Index [%d]", nSquare);
			return;
		}

		this->m_DevilSquareGround[nSquare].Set(nMonsterType, nStartTime, nEndTime);
	}

	pugi::xml_node boss_settings = main.child("BossAppearanceSettings");

	for (pugi::xml_node square = boss_settings.child("Square"); square; square = square.next_sibling())
	{
		int nSquare = square.attribute("Level").as_int();
		int nMonsterType = square.attribute("MonsterIndex").as_int();
		int nSpawnTime = square.attribute("SpawnTime").as_int();
		int nStartX = square.attribute("StartX").as_int();
		int nStartY = square.attribute("StartY").as_int();
		int nEndX = square.attribute("EndX").as_int();
		int nEndY = square.attribute("EndY").as_int();

		if ( nSquare < 0 || nSquare >= MAX_DEVILSQUARE_GROUND )
		{
			g_Log.MsgBox("[DevilSquare] Invalid Square Index [%d]", nSquare);
			return;
		}

		this->m_DevilSquareGround[nSquare].SetBoss(nMonsterType, nSpawnTime, nStartX, nStartY, nEndX, nEndY);
	}

	pugi::xml_node ranking_settings = main.child("RankingBonusSettings");

	for (pugi::xml_node square = ranking_settings.child("Square"); square; square = square.next_sibling())
	{
		int nSquare = square.attribute("Level").as_int();
		int nRank = square.attribute("RankingPlace").as_int();
		int nExp = square.attribute("ExpBonus").as_int();
		int nZen = square.attribute("ZenBonus").as_int();

		if ( nSquare < 0 || nSquare >= MAX_DEVILSQUARE_GROUND )
		{
			g_Log.MsgBox("[DevilSquare] Invalid Square Index [%d]", nSquare);
			return;
		}

		this->m_DevilSquareGround[nSquare].SetBonus(nRank, nZen, nExp);
	}

	pugi::xml_node exp_settings = main.child("RewardExpSettings");

	for (pugi::xml_node square = exp_settings.child("Square"); square; square = square.next_sibling())
	{
		int nSquare = square.attribute("Level").as_int();
		float fMultiplier = square.attribute("Multiplier").as_float();

		if ( nSquare < 0 || nSquare >= MAX_DEVILSQUARE_GROUND )
		{
			g_Log.MsgBox("[DevilSquare] Invalid Square Index [%d]", nSquare);
			return;
		}

		this->m_DevilSquareGround[nSquare].ApplyBonusRate(fMultiplier);
	}

	// private custom
	pugi::xml_node entry_level = main.child("EntryLevel");

	for (pugi::xml_node square = entry_level.child("Square"); square; square = square.next_sibling())
	{
		int nSquare = square.attribute("Level").as_int();

		if (nSquare < 0 || nSquare >= MAX_DEVILSQUARE_GROUND)
		{
			g_Log.MsgBox("[DevilSquare] Invalid Square Index [%d]", nSquare);
			return;
		}

		g_sttDEVILSQUARE_LEVEL[nSquare].NormalCharacterMinLevel = square.attribute("NormalMinLevel").as_int();
		g_sttDEVILSQUARE_LEVEL[nSquare].NormalCharacterMaxLevel = square.attribute("NormalMaxLevel").as_int();
		g_sttDEVILSQUARE_LEVEL[nSquare].SpecialCharacterMinLevel = square.attribute("SpecialMinLevel").as_int();
		g_sttDEVILSQUARE_LEVEL[nSquare].SpecialCharacterMaxLevel = square.attribute("SpecialMaxLevel").as_int();
	}

	g_Log.Add("%s file load!", filename);
}

void CDevilSquare::LoadMonster(LPSTR szFile)
{
	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(szFile);

	if (res.status != pugi::status_ok)
	{
		g_Log.MsgBox("%s file load fail (%s)", szFile, res.description());
		return;
	}

	pugi::xml_node main = file.child("DevilSquare_Classic");
	int nCount = 0;

	for (pugi::xml_node monster = main.child("Monster"); monster; monster = monster.next_sibling())
	{
		int iEventLevel = monster.attribute("EventLevel").as_int();
		nCount = 0;

		if (DS_LEVEL_RANGE(iEventLevel) == FALSE)
		{
			continue;
		}

		int iMapNumber = monster.attribute("MapNumber").as_int();

		if (DS_MAP_RANGE(iMapNumber) == FALSE)
		{
			continue;
		}

		for (pugi::xml_node spawn = monster.child("Spawn"); spawn; spawn = spawn.next_sibling())
		{
			this->m_DSMP[iEventLevel][nCount].m_Type		= spawn.attribute("Index").as_int();
			this->m_DSMP[iEventLevel][nCount].m_MapNumber	= iMapNumber;
			this->m_DSMP[iEventLevel][nCount].m_Dis			= spawn.attribute("Distance").as_int();
			this->m_DSMP[iEventLevel][nCount].m_X			= spawn.attribute("StartX").as_int();
			this->m_DSMP[iEventLevel][nCount].m_Y			= spawn.attribute("StartY").as_int();
			this->m_DSMP[iEventLevel][nCount].m_W			= spawn.attribute("EndX").as_int();
			this->m_DSMP[iEventLevel][nCount].m_H			= spawn.attribute("EndY").as_int();
			this->m_DSMP[iEventLevel][nCount].m_Dir			= spawn.attribute("Dir").as_int();
			this->m_DSMP[iEventLevel][nCount].m_Count		= spawn.attribute("Count").as_int();
			nCount++;
		}

		if (nCount == 10 || nCount < 0)
		{
			g_Log.Add("[DevilSquare][LoadMonster] Monster Count Invalid");
			nCount = 10;
		}

		this->m_DSMP_CNT[iEventLevel] = nCount;
	}

	g_Log.Add("%s file load!", szFile);
}



void CDevilSquare::SetState(enum eDevilSquareState eState)
{
	this->m_eState = eState;

	switch ( this->m_eState )
	{
		case DevilSquare_CLOSE:	this->SetClose();	break;
		case DevilSquare_OPEN:		this->SetOpen();	break;
		case DevilSquare_PLAYING:	this->SetPlaying();	break;
		case DevilSquare_NONE:		this->SetNone();
	}
}



void CDevilSquare::SetClose()
{
	g_Log.Add("[DevilSquare] Close");

	this->ClearMonstr();
	this->CalcScore();

	for ( int i=0;i<MAX_DEVILSQUARE_GROUND;i++)
	{
		this->m_DevilSquareGround[i].Clear();
	}

	this->m_iRemainTime = this->m_iCloseTime;
	this->CheckSync();
	this->m_bFlag = 0;

	if ( this->m_iRemainTime <= 1 )
	{
		this->m_iTime = GetTickCount64();
		this->m_iremainTimeSec = 60;
	}
	else
	{
		this->m_iTime = GetTickCount64() + 60000;
		this->m_iremainTimeSec = -1;
	}
	
	this->m_bSendTimeCount = FALSE;
}



DEVILSQUARE_MONSTER_POSITION * CDevilSquare::GetMonsterPosData(int iPosNum, int iSquareNumber)
{
	if (DS_LEVEL_RANGE(iSquareNumber) == FALSE)
	{
		return NULL;
	}

	if (iPosNum < 0 || iPosNum >= 10)
	{
		return NULL;
	}

	return &this->m_DSMP[iSquareNumber][iPosNum];
}

void CDevilSquare::SetOpen()
{
	this->m_iRemainTime = this->m_iOpenTime;

	if ( this->m_iOpenTime <= 1 )
	{
		this->m_iTime = GetTickCount64();
		this->m_iremainTimeSec = 60;
	}
	else
	{
		this->m_iTime = GetTickCount64() + 60000;
		this->m_iremainTimeSec = -1;
	}

	this->m_bSendTimeCount = FALSE;
}



void CDevilSquare::SetPlaying()
{
	this->gObjScoreClear();
	this->SetMonster();

	this->m_iRemainTime = this->m_iPlaytime;

	if ( this->m_iPlaytime <= 1 )
	{
		this->m_iTime = GetTickCount64();
		this->m_iremainTimeSec = 60;
	}
	else
	{
		this->m_iTime = GetTickCount64() + 60000;
		this->m_iremainTimeSec = -1;
	}
	
	this->m_bSendTimeCount = FALSE;
}

void CDevilSquare::SetNone()
{
	return;
}


int  CDevilSquare::GetDevilSquareIndex(int iGateNumber)
{
	switch ( iGateNumber )
	{

		case 58:
		case 59:
		case 60:
		case 61:
			return iGateNumber - 58;	// Devil 1 - 4
			break;

		case 111:
		case 112:
			return iGateNumber - 107;	// Devil 5 - 7
			break;

		case 270:
			return 6;
			break;

		default:
			return -1;
			break;
	}
}



int  CDevilSquare::GetUserLevelToEnter(int iUserIndex, WORD& wMoveGate)
{
	wMoveGate = -1;

	if ( gObjIsConnected(iUserIndex) == FALSE )
	{
		return -1;
	}

	int iENTER_LEVEL = -1;

	for ( int i=0;i<MAX_DEVILSQUARE_GROUND;i++)
	{
		if ( gObj[iUserIndex].Class == 4 || gObj[iUserIndex].Class == 3 || gObj[iUserIndex].Class == 6 )
		{
			if (gObj[iUserIndex].m_PlayerData->ChangeUP == 2)
			{
				iENTER_LEVEL = 6;
				wMoveGate = g_sttDEVILSQUARE_LEVEL[6].MoveGate;
				break;
			}

			if ( gObj[iUserIndex].Level >= g_sttDEVILSQUARE_LEVEL[i].SpecialCharacterMinLevel  && gObj[iUserIndex].Level <= g_sttDEVILSQUARE_LEVEL[i].SpecialCharacterMaxLevel && gObj[iUserIndex].m_PlayerData->ChangeUP != 2 )
			{
				iENTER_LEVEL = i;
				wMoveGate = g_sttDEVILSQUARE_LEVEL[i].MoveGate;
				break;
			}
		}
		else
		{
			if (gObj[iUserIndex].m_PlayerData->ChangeUP == 2)
			{
				iENTER_LEVEL = 6;
				wMoveGate = g_sttDEVILSQUARE_LEVEL[6].MoveGate;
				break;
			}

			if ( gObj[iUserIndex].Level >= g_sttDEVILSQUARE_LEVEL[i].NormalCharacterMinLevel  && gObj[iUserIndex].Level <= g_sttDEVILSQUARE_LEVEL[i].NormalCharacterMaxLevel & gObj[iUserIndex].m_PlayerData->ChangeUP != 2 )
			{
				iENTER_LEVEL = i;
				wMoveGate = g_sttDEVILSQUARE_LEVEL[i].MoveGate;
				break;
			}
		}
	}

	return iENTER_LEVEL;
}



void CDevilSquare::Run()
{
	if ( this->m_bQuit == TRUE )
	{
		return;
	}

	if ( this->IsEventEnable() == true  )
	{
		switch ( this->m_eState )
		{
			case DevilSquare_CLOSE:	this->ProcClose();		break;
			case DevilSquare_OPEN:		this->ProcOpen();		break;
			case DevilSquare_PLAYING:	this->ProcPlaying();	break;
		}
	}
}

void CDevilSquare::CheckSync()
{
	if ( this->m_vtDevilSquareTime.empty() != false )
	{
		return;
	}

	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	time_t ltime;
	tm * timer;
	time(&ltime);
	timer = localtime( &ltime );
	int iMIN_HOUR = 24;
	int iMIN_MINUTE = 60;
	BOOL bTIME_CHANGED = FALSE;
	std::vector<DEVILSQUARE_TIME>::iterator it;

	for (it = this->m_vtDevilSquareTime.begin() ; it != this->m_vtDevilSquareTime.end() ; it++ )
	{
		DEVILSQUARE_TIME & stDSTime = *it;

		if ( (sysTime.wHour * 60 + sysTime.wMinute) < (stDSTime.m_iHOUR * 60 + stDSTime.m_iMINUTE) )
		{
			if ( ( iMIN_HOUR * 60 + iMIN_MINUTE ) > ( stDSTime.m_iHOUR * 60 + stDSTime.m_iMINUTE ) )
			{
				bTIME_CHANGED = TRUE;
				iMIN_HOUR = stDSTime.m_iHOUR;
				iMIN_MINUTE = stDSTime.m_iMINUTE;
			}
		}
	}

	if ( bTIME_CHANGED == FALSE )
	{
		it = this->m_vtDevilSquareTime.begin();
		iMIN_HOUR = 24;
		iMIN_MINUTE = 60;

		for ( ; it != this->m_vtDevilSquareTime.end() ; it++ )
		{
			DEVILSQUARE_TIME & stDSTime = *it;

			if ( ( iMIN_HOUR * 60 + iMIN_MINUTE ) > ( stDSTime.m_iHOUR * 60 + stDSTime.m_iMINUTE ) )
			{
				bTIME_CHANGED = 2;
				iMIN_HOUR = stDSTime.m_iHOUR;
				iMIN_MINUTE = stDSTime.m_iMINUTE;
			}
		}
	}

	switch ( bTIME_CHANGED )
	{
		case TRUE:
			this->m_iRemainTime = (((iMIN_HOUR * 60) + (iMIN_MINUTE)) - ((sysTime.wHour * 60) + (sysTime.wMinute)));
			break;

		case 2:
			this->m_iRemainTime = ((((iMIN_HOUR+24) * 60) + (iMIN_MINUTE)) - ((sysTime.wHour * 60) + (sysTime.wMinute)));
			break;

		default:
			break;

	}

	g_Log.Add("[Devil Square] Sync Open Time. [%d]min remain", this->m_iRemainTime);
}


void CDevilSquare::ProcClose()
{
	if ( this->m_iremainTimeSec != -1 )
	{
		ULONGLONG i64Time = (GetTickCount64() - this->m_iTime)/1000;

		if ( i64Time != 0 )
		{
			this->m_iremainTimeSec -= i64Time;
			this->m_iTime += i64Time * 1000;

			if ( this->m_iremainTimeSec <= 30 && this->m_bSendTimeCount == FALSE )
			{
				PMSG_SET_DEVILSQUARE pMsg;

				PHeadSetB((LPBYTE)&pMsg, 0x92, sizeof(pMsg));
				pMsg.Type = DevilSquare_CLOSE;

				for ( int i=g_ConfigRead.server.GetObjectStartUserIndex();i<g_ConfigRead.server.GetObjectMax();i++)
				{
					if ( gObj[i].Connected == PLAYER_PLAYING && gObj[i].Type == OBJ_USER)
					{
						if ( BC_MAP_RANGE(gObj[i].MapNumber) == FALSE )
						{
							if ( CC_MAP_RANGE(gObj[i].MapNumber) == FALSE )
							{
								IOCP.DataSend(i, (BYTE *)&pMsg, pMsg.h.size);
							}
						}
					}
				}

				this->m_bSendTimeCount = TRUE;
			}		

			if (this->m_iremainTimeSec < 1 )
			{
				PMSG_NOTICE pNotice;

				TNotice::MakeNoticeMsg((TNotice *)&pNotice, 0, Lang.GetText(0,4));

				for ( int i=g_ConfigRead.server.GetObjectStartUserIndex();i<g_ConfigRead.server.GetObjectMax();i++)
				{
					if ( gObj[i].Connected == PLAYER_PLAYING && gObj[i].Type == OBJ_USER)
					{
						if ( BC_MAP_RANGE(gObj[i].MapNumber) == FALSE )
						{
							if ( CC_MAP_RANGE(gObj[i].MapNumber) == FALSE )
							{
								IOCP.DataSend(i, (BYTE *)&pNotice, pNotice.h.size);
							}
						}
					}
				}

				g_Log.Add(pNotice.Notice);
				this->SetState(DevilSquare_OPEN);
			}
		
		}
	}
	else if ( this->m_iTime < GetTickCount64() )
	{
		this->ClearMonstr();
		this->m_iRemainTime--;

		if ( this->m_bFlag == 0 )
		{
			for ( int i=g_ConfigRead.server.GetObjectStartUserIndex();i<g_ConfigRead.server.GetObjectMax();i++)
			{
				if ( gObj[i].Connected == PLAYER_PLAYING )
				{
					if ( DS_MAP_RANGE(gObj[i].MapNumber) != FALSE )
					{
						gObjMoveGate(i, 27);
					}
				}
			}

			this->ItemClear();
		}
		else
		{
			this->CheckInvalidUser();
		}

		if ( this->m_iRemainTime <= 15 )
		{
			if ( (this->m_iRemainTime%5)== 0 )
			{
				PMSG_NOTICE pNotice;

				TNotice::MakeNoticeMsgEx((TNotice *)&pNotice, 0, Lang.GetText(0,5), this->m_iRemainTime);

				for ( int i=g_ConfigRead.server.GetObjectStartUserIndex();i<g_ConfigRead.server.GetObjectMax();i++)
				{
					if ( gObj[i].Connected == PLAYER_PLAYING && gObj[i].Type == OBJ_USER)
					{
						if ( BC_MAP_RANGE(gObj[i].MapNumber) == FALSE )
						{
							if ( CC_MAP_RANGE(gObj[i].MapNumber) == FALSE )
							{
								IOCP.DataSend(i, (BYTE *)&pNotice, pNotice.h.size);
							}
						}
					}
				}

				g_Log.Add((char*)pNotice.Notice);
			}
		}

		if ( this->m_iRemainTime == 1 && this->m_iremainTimeSec == -1 )
		{
			this->m_iremainTimeSec = 60;
			this->m_iTime = GetTickCount64();
		}
		else
		{
			this->m_iTime = GetTickCount64() + 60000;
		}
	}
}



void CDevilSquare::ProcOpen()
{
	if ( this->m_iremainTimeSec != -1 )
	{
		ULONGLONG i64Time = (GetTickCount64() - this->m_iTime)/1000;

		if ( i64Time != 0 )
		{
			this->m_iremainTimeSec -= i64Time;
			this->m_iTime += i64Time * 1000;

			if ( this->m_iremainTimeSec <= 30 && this->m_bSendTimeCount == FALSE )
			{
				PMSG_SET_DEVILSQUARE pMsg;

				PHeadSetB((LPBYTE)&pMsg, 0x92, sizeof(pMsg));
				pMsg.Type = DevilSquare_OPEN;

				for ( int i=g_ConfigRead.server.GetObjectStartUserIndex();i<g_ConfigRead.server.GetObjectMax();i++)
				{
					if ( gObj[i].Connected == PLAYER_PLAYING && gObj[i].Type == OBJ_USER)
					{
						if ( BC_MAP_RANGE(gObj[i].MapNumber) == FALSE )
						{
							if ( CC_MAP_RANGE(gObj[i].MapNumber) == FALSE )
							{
								if ( DG_MAP_RANGE(gObj[i].MapNumber) == FALSE )
								{
									if ( IMPERIAL_MAP_RANGE(gObj[i].MapNumber) == FALSE )
									{
										IOCP.DataSend(i, (BYTE *)&pMsg, pMsg.h.size);
									}
								}
							}
						}
					}
				}

				this->m_bSendTimeCount = TRUE;
			}

			if (this->m_iremainTimeSec < 1 )
			{
				this->SendEventStartMsg();
				this->SetState(DevilSquare_PLAYING);
			}
		
		}
	}
	else
	{
		if ( this->m_iTime < GetTickCount64() )
		{
			this->ClearMonstr();
			this->m_iRemainTime--;

			if ( this->m_iRemainTime > 0 )
			{
				PMSG_NOTICE pNotice;

				TNotice::MakeNoticeMsgEx(&pNotice, 0, Lang.GetText(0,6), this->m_iRemainTime);

				for ( int i=g_ConfigRead.server.GetObjectStartUserIndex();i<g_ConfigRead.server.GetObjectMax();i++)
				{
					if ( gObj[i].Connected == PLAYER_PLAYING && gObj[i].Type == OBJ_USER)
					{
						if ( BC_MAP_RANGE(gObj[i].MapNumber) == FALSE )
						{
							if ( CC_MAP_RANGE(gObj[i].MapNumber) == FALSE )
							{
								IOCP.DataSend(i, (BYTE *)&pNotice, pNotice.h.size);
							}
						}
					}
				}

				g_Log.Add((char*)pNotice.Notice);

				this->m_iTime = GetTickCount64();
			}

			if (this->m_iRemainTime == 1 && this->m_iremainTimeSec == -1)
			{
				this->m_iremainTimeSec = 60;
				this->m_iTime = GetTickCount64();
			}
			else
			{
				this->m_iTime = GetTickCount64() + 60000;
			}
		}
	}
}





void CDevilSquare::ProcPlaying()
{
	if ( this->m_iremainTimeSec != -1 )
	{
		ULONGLONG i64Time = (GetTickCount64() - this->m_iTime)/1000;

		if ( i64Time != 0 )
		{
			this->m_iremainTimeSec -= i64Time;
			this->m_iTime += i64Time * 1000;

			if ( this->m_iremainTimeSec <= 30 && this->m_bSendTimeCount == FALSE )
			{
				PMSG_SET_DEVILSQUARE pMsg;

				PHeadSetB((LPBYTE)&pMsg, 0x92, sizeof(pMsg));
				pMsg.Type = DevilSquare_PLAYING;
				GSProtocol.AllSendSameMapMsg((UCHAR*)&pMsg, sizeof(pMsg), MAP_INDEX_DEVILSQUARE);
				GSProtocol.AllSendSameMapMsg((UCHAR*)&pMsg, sizeof(pMsg), MAP_INDEX_DEVILSQUARE2);
				this->m_bSendTimeCount = TRUE;
			}


			if ( this->m_iremainTimeSec < 1 )
			{
				this->SetState(DevilSquare_CLOSE);
			}
		}
	}
	else
	{
		if ( this->m_iTime < GetTickCount64() )
		{
			this->m_iRemainTime--;

			if ( this->m_iRemainTime > 0 )
			{
				if ( (this->m_iRemainTime%5) == 0 )
				{
					PMSG_NOTICE pNotice;

					TNotice::MakeNoticeMsgEx((TNotice *)&pNotice, 0, Lang.GetText(0,7), this->m_iRemainTime);
					g_Log.Add((char*)pNotice.Notice);
					GSProtocol.AllSendSameMapMsg((UCHAR*)&pNotice, pNotice.h.size, MAP_INDEX_DEVILSQUARE);
					GSProtocol.AllSendSameMapMsg((UCHAR*)&pNotice, pNotice.h.size, MAP_INDEX_DEVILSQUARE2);
				}

				for ( int i=0;i<MAX_DEVILSQUARE_GROUND;i++)
				{
					this->m_DevilSquareGround[i].RegenBossMonster(this->m_iPlaytime - this->m_iRemainTime);
				}

				if ( this->m_iRemainTime == 1 && this->m_iremainTimeSec == -1 )
				{
					this->m_iremainTimeSec = 60;
					this->m_iTime = GetTickCount64();
				}
				else
				{
					this->m_iTime = GetTickCount64() + 60000;
				}
			}
		}
	}
}

void CDevilSquare::ProcNone()
{
	return;
}



void CDevilSquare::SetMonster()
{
	int result;

 	for (int i = 0; i < MAX_DEVILSQUARE_GROUND; i++)
	{
		for (int j = 0; j < this->m_DSMP_CNT[i]; j++)
		{
			if (DS_MAP_RANGE(this->m_DSMP[i][j].m_MapNumber) != FALSE)
			{
				for (int k = 0; k < this->m_DSMP[i][j].m_Count; k++)
				{
					WORD wMonIndex = this->m_DSMP[i][j].m_Type;
					result = gObjAddMonster(this->m_DSMP[i][j].m_MapNumber);

					if (result >= 0)
					{
						gObj[result].m_PosNum = j;
						gObj[result].X = this->m_DSMP[i][j].m_X;
						gObj[result].Y = this->m_DSMP[i][j].m_Y;
						gObj[result].MapNumber = this->m_DSMP[i][j].m_MapNumber;

						this->GetPosition(j, i, this->m_DSMP[i][j].m_MapNumber, gObj[result].X, gObj[result].Y);

						gObj[result].TX = gObj[result].X;
						gObj[result].TY = gObj[result].Y;
						gObj[result].m_OldX = gObj[result].X;
						gObj[result].m_OldY = gObj[result].Y;
						gObj[result].Dir = this->m_DSMP[i][j].m_Dir;
						gObj[result].StartX = gObj[result].X;
						gObj[result].StartY = gObj[result].Y;
						gObjSetMonster(result, wMonIndex);
						gObj[result].MaxRegenTime = 1000;	// Rgeneration in 1 second
						gObj[result].m_bDevilSquareIndex = i;
					}
				}
			}
		}
	}
}

void CDevilSquare::ClearMonstr()
{
	for ( int n=0;n<g_ConfigRead.server.GetObjectMaxMonster();n++)
	{
		if ( DS_MAP_RANGE(gObj[n].MapNumber) != FALSE )
		{
			gObjDel(n);
		}
	}
}

void CDevilSquare::gDevilSquareMonsterRegen(LPOBJ lpObj)
{
	BYTE devilsquareindex = lpObj->m_bDevilSquareIndex;

	if ( devilsquareindex < 0 || devilsquareindex >= MAX_DEVILSQUARE_GROUND )
	{
		g_Log.Add("[DevilSquare] Invalid DevilSquareIndex [%d]", devilsquareindex);
		return;
	}

	if ( lpObj->m_PosNum == -1 )
	{
		gObjDel(lpObj->m_Index);
		return;
	}

	WORD monstertype = this->m_DevilSquareGround[devilsquareindex].GetMonsterType(this->m_iPlaytime - this->m_iRemainTime );

	if ( monstertype == (BYTE)-1 )
	{
		g_Log.Add("[DevilSquare] [%d] Invalid MonterType", monstertype);
		return;
	}

	for ( int n=0;n<MAX_MAGIC;n++)
	{
		lpObj->Magic[n].Clear();
	}

	gObjSetMonster(lpObj->m_Index, monstertype);
	lpObj->DieRegen = FALSE;
	gObjMonsterRegen(lpObj);
	CreateFrustrum(lpObj->X, lpObj->Y, lpObj->m_Index);
	lpObj->m_bDevilSquareIndex = devilsquareindex;
	lpObj->MaxRegenTime = 1000;

	g_Log.Add("[DevilSquare] Monster Regen [%d][%d][%d,%d]",
		monstertype, devilsquareindex, lpObj->X, lpObj->Y);
}



void CDevilSquare::SendEventStartMsg()
{
	PMSG_NOTICE pToEventer;
	PMSG_NOTICE pWithOutEvneter;

	TNotice::MakeNoticeMsg(&pToEventer, 0, Lang.GetText(0, 8));
	TNotice::MakeNoticeMsg(&pWithOutEvneter, 0, Lang.GetText(0, 9));

	for (int n = g_ConfigRead.server.GetObjectStartUserIndex(); n < g_ConfigRead.server.GetObjectMax(); n++)
	{
		if (gObj[n].Connected == PLAYER_PLAYING)
		{
			if (DS_MAP_RANGE(gObj[n].MapNumber) != FALSE)
			{
				IOCP.DataSend(n, (BYTE *)&pToEventer, pToEventer.h.size);
			}
			else
			{
				IOCP.DataSend(n, (BYTE *)&pWithOutEvneter, pWithOutEvneter.h.size);
			}
		}
	}

	g_Log.Add("[DevilSquare] Start Event");
}



void CDevilSquare::DieProcDevilSquare(LPOBJ lpObj)
{
	char msg[255];

	wsprintf(msg, Lang.GetText(0,10), lpObj->m_nEventScore);
	GSProtocol.GCServerMsgStringSend(msg, lpObj->m_Index, 1);

	if ( lpObj->m_nEventScore <= 0 )
		return;

	PMSG_ANS_EVENTUSERSCORE pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0xBD;
	pMsg.h.subcode = 0x01;
	pMsg.h.size = sizeof(pMsg);
	pMsg.SquareNum = lpObj->m_bDevilSquareIndex;
	pMsg.Class = lpObj->Class;

	if ( lpObj->Class == 1 )
	{
		if ( lpObj->Class >= 0 && lpObj->Class < MAX_TYPE_PLAYER )
		{
			if ( DS_LEVEL_RANGE(lpObj->m_bDevilSquareIndex) != FALSE )
			{
				lpObj->m_nEventScore+= this->m_BonusScoreTable[lpObj->Class][lpObj->m_bDevilSquareIndex]/100;
				
			}
		}

		pMsg.Score = lpObj->m_nEventScore;
	}
	else
	{
		pMsg.Score = lpObj->m_nEventScore;
	}

	pMsg.ServerCode = g_ConfigRead.server.GetGameServerCode();
	memcpy(pMsg.AccountID, lpObj->AccountID, sizeof(pMsg.AccountID));
	memcpy(pMsg.GameID, lpObj->Name, sizeof(pMsg.GameID));

	wsDataCli.DataSend((char *)&pMsg, pMsg.h.size);

	g_Log.Add("[DevilSquare] Dead [%s][%s][%d][%d]",
		lpObj->AccountID, lpObj->Name, 
		lpObj->m_nEventExp, lpObj->m_nEventScore);

	lpObj->m_nEventScore = 0;
	lpObj->m_nEventMoney = 0;
	lpObj->m_nEventExp = 0;

}




UINT64 CDevilSquare::gObjMonsterExpSingle(LPOBJ lpObj, LPOBJ lpTargetObj, int dmg, int tot_dmg)
{
	BYTE devilsquareindex = lpObj->m_bDevilSquareIndex;

	if (devilsquareindex < 0 || devilsquareindex >= MAX_DEVILSQUARE_GROUND)
	{
		return 0;
	}

	UINT64 exp;
	UINT64 maxexp = 0;
	int level = ((lpTargetObj->Level + 25) * lpTargetObj->Level) / 3;

	if ((lpTargetObj->Level + 10) < lpObj->Level)
	{
		level = (level*(lpTargetObj->Level + 10)) / lpObj->Level;
	}

	if (lpTargetObj->Level >= 65)
	{
		level += (lpTargetObj->Level - 64)*(lpTargetObj->Level / 4);
	}

	if (level > 0)
	{
		maxexp = level / 2;
	}
	else
	{
		level = 0;
	}

	if (maxexp < 1)
	{
		exp = level;
	}
	else
	{
		exp = level + rand() % maxexp;
	}

	exp = (dmg * exp) / tot_dmg;

	if (g_MasterLevelSkillTreeSystem.IsMasterLevelUser(lpObj) == false)
	{
		DWORD mymaxexp = gLevelExperience[lpObj->Level];

		if (exp > mymaxexp)
		{
			exp = mymaxexp;
		}
	}
	else
	{
		DWORD mymaxexp;

		if (lpObj->m_PlayerData->MasterLevel == g_ConfigRead.data.common.MLUserMaxLevel)
		{
			mymaxexp = g_MasterLevelSkillTreeSystem.gMasterExperience[lpObj->m_PlayerData->MasterLevel];
		}
		else
		{
			mymaxexp = g_MasterLevelSkillTreeSystem.gMasterExperience[lpObj->m_PlayerData->MasterLevel + 1];
		}

		if (exp > mymaxexp)
		{
			exp = mymaxexp;
		}
	}

	UINT64 dwDefaultExp = exp;
	float fVipBonus = g_VipSystem.GetExpBonus(lpObj);
	float fEventBonus = 0.0;
	float fMapBonus = 0.0;
	float fBaseExp = 0.0;

	if (!g_MasterLevelSkillTreeSystem.IsMasterLevelUser(lpObj))
	{
		fEventBonus = g_BonusEvent.GetAddExp();
		fMapBonus = g_MapAttr.GetExpBonus(lpObj->MapNumber);
		fBaseExp = g_ExpManager.GetExpMultiplier(lpObj->Level, lpObj->m_PlayerData->m_iResets, false);
	}

	else
	{
		fEventBonus = g_BonusEvent.GetAddMLExp();
		fMapBonus = g_MapAttr.GetMasterExpBonus(lpObj->MapNumber);
		fBaseExp = g_ExpManager.GetExpMultiplier(lpObj->Level + lpObj->m_PlayerData->MasterLevel, lpObj->m_PlayerData->m_iResets, true);
	}

	exp *= (fBaseExp + fVipBonus + fEventBonus + fMapBonus);

	if (g_CrywolfSync.GetOccupationState() == 1 && g_CrywolfSync.GetApplyPenalty() == TRUE)
	{
		exp = (exp * g_CrywolfSync.GetGettingExpPenaltyRate()) / 100;
	}

	if (g_MasterLevelSkillTreeSystem.CheckMLGetExp(lpObj, lpTargetObj) == FALSE)
	{
		exp = 0;
	}

	if (exp > 0)
	{
		if (lpObj->Type == OBJ_USER)
		{
			CheckItemOptForGetExpExRenewal(lpObj, NULL, exp, dwDefaultExp, false);

			if (!g_MasterLevelSkillTreeSystem.IsMasterLevelUser(lpObj))
			{
				lpObj->m_PlayerData->Experience += exp;
			}
			else
			{
				lpObj->m_PlayerData->MasterExperience += exp;
			}

			lpObj->m_nEventExp += exp;

			if (gObjLevelUp(lpObj, exp, lpTargetObj->Class, "Devil Square Monster (Single)") == false)
			{
				return 0;
			}
		}
	}

	return exp;
}

		

void CDevilSquare::gObjExpParty(LPOBJ lpObj, LPOBJ lpTargetObj, int AttackDamage, BOOL MSBFlag)
{
	if(lpObj->Type != OBJ_USER)
	{
		return;
	}

	BYTE devilsquareindex = lpObj->m_bDevilSquareIndex;

	if ( devilsquareindex < 0 || devilsquareindex >= MAX_DEVILSQUARE_GROUND )
	{
		return;
	}

	int n;
	UINT64 exp;
	UINT64 maxexp = 0;
	int totalexp;
	int level = ((lpTargetObj->Level + 25) * lpTargetObj->Level) / 3;
	int number;
	int partynum = 0;
	int totallevel = 0;
	int partylevel;
	int partycount;
	int dis[MAX_USER_IN_PARTY];
	int viewplayer = 0;
	int viewpercent = 100;
	BOOL bApplaySetParty = FALSE;
	bool bCheckSetParty[MAX_TYPE_PLAYER];
	partynum = lpObj->PartyNumber;
	LPOBJ lpPartyObj;

	
	int toplevel = 0;

	for (n=0;n<MAX_USER_IN_PARTY;n++)
	{
		number = gParty.m_PartyS[partynum].Number[n];

		if ( number >= 0 )
		{
			lpPartyObj = &gObj[number];

			if ( lpPartyObj->Level > toplevel )
			{
				toplevel = lpPartyObj->Level;
			}
		}
	}

	if ( ObjectMaxRange(partynum) == FALSE )
	{
		g_Log.Add("error : %s %d", __FILE__, __LINE__);
		return;
	}

	partycount = gParty.m_PartyS[partynum].Count;

	for ( n =0;n<MAX_USER_IN_PARTY;n++)
	{
		number = gParty.m_PartyS[partynum].Number[n];

		if ( number >= 0 )
		{
			lpPartyObj = &gObj[number];

			if ( lpTargetObj->MapNumber ==lpPartyObj->MapNumber )
			{
				dis[n] = gObjCalDistance(lpTargetObj, &gObj[number]);

				if ( dis[n] < 10 )
				{
					lpPartyObj = &gObj[number];

					if ( toplevel >= (lpPartyObj->Level + 200 ) ) // #formula
					{
						totallevel = totallevel + lpPartyObj->Level + 200;
					}
					else
					{
						totallevel += lpPartyObj->Level;
					}

					viewplayer++;
					bCheckSetParty[lpPartyObj->Class] = true;
				}
			}
		}
	}

	if ( bCheckSetParty[0] != false && bCheckSetParty[1] != false && bCheckSetParty[2] != false )
	{
		bApplaySetParty = TRUE;
	}

	if ( viewplayer > 1 )
	{
		if ( bApplaySetParty != FALSE )
		{
			if ( viewplayer == 3 )
			{
				viewpercent = gSetPartyExp3;
			}
			else if ( viewplayer == 4 )
			{
				viewpercent = gSetPartyExp4;
			}
			else if ( viewplayer >= 5 )
			{
				viewpercent = gSetPartyExp5;
			}
			else
			{
				viewpercent = gSetPartyExp2;
			}
		}
		else
		{
			if ( viewplayer == 2 )
			{
				viewpercent = gPartyExp2;
			}
			else if ( viewplayer == 3 )
			{
				viewpercent = gPartyExp3;
			}
			else if ( viewplayer == 4 )
			{
				viewpercent = gPartyExp4;
			}
			else if ( viewplayer >= 5 )
			{
				viewpercent = gPartyExp5;
			}
			else
			{
				viewpercent = gPartyExp1;
			}
		}

		partylevel = totallevel / viewplayer;
	}
	else
	{
		partylevel = totallevel;
	}

	if ( (lpTargetObj->Level +10) < partylevel )
	{
		level = (level * (lpTargetObj->Level+10) ) / partylevel;
	}

	if ( lpTargetObj->Level >= 65 )
	{
		if ( viewplayer == 1 )
		{
			level += (lpTargetObj->Level - 64) * (lpTargetObj->Level/ 4);
		}
		else
		{
			level += (200.0 - (lpObj->Level * 0.2));
		}
	}

	if ( level > 0 )
	{
		maxexp = level / 2;
	}
	else
	{
		level = 0;
	}

	if ( maxexp < 1 )
	{
		totalexp = level;
	}
	else
	{
		totalexp = level + rand()%maxexp;
	}

	if ( lpTargetObj->Type == OBJ_MONSTER )
	{
		lpTargetObj->MonsterMoneyDrop = totalexp;
	}

	for ( n=0;n<MAX_USER_IN_PARTY;n++)
	{
		number = gParty.m_PartyS[partynum].Number[n];

		if ( number >= 0 )
		{
			lpPartyObj = &gObj[number];

			if ( lpTargetObj->MapNumber == lpPartyObj->MapNumber )
			{
				if ( dis[n] < 10 )
				{
					if(g_MasterLevelSkillTreeSystem.IsMasterLevelUser(lpPartyObj))
					{
						UINT64 myexp;

						if(lpPartyObj->m_PlayerData->MasterLevel == g_ConfigRead.data.common.MLUserMaxLevel)
						{
							myexp = g_MasterLevelSkillTreeSystem.gMasterExperience[lpPartyObj->m_PlayerData->MasterLevel];
						}
						else
						{
							myexp = g_MasterLevelSkillTreeSystem.gMasterExperience[lpPartyObj->m_PlayerData->MasterLevel+1];
						}				
						if ( exp > myexp  )
						{
							exp = myexp;
						}
					}

					else
					{
						UINT64 mymaxexp = gLevelExperience[lpObj->Level];
						if ( exp > mymaxexp )
						{
							exp = mymaxexp;
						}
					}

					exp = ((totalexp * viewpercent* lpPartyObj->Level ) / totallevel ) / 100;

					if( lpPartyObj->Type == OBJ_USER )
					{
						if ( lpTargetObj->Type == OBJ_USER )
						{
							exp = 0;
						}
					}

					UINT64 dwDefaultExp = exp;
					float fVipBonus = g_VipSystem.GetExpBonus(lpPartyObj);
					float fEventBonus = 0.0;
					float fMapBonus = 0.0;
					float fBaseExp = 0.0;

					if(g_MasterLevelSkillTreeSystem.IsMasterLevelUser(lpPartyObj))
					{
						fEventBonus = g_BonusEvent.GetAddMLExp();
						fMapBonus = g_MapAttr.GetMasterExpBonus(lpPartyObj->MapNumber);
						fBaseExp = g_ExpManager.GetExpMultiplier(lpPartyObj->Level + lpPartyObj->m_PlayerData->MasterLevel, lpPartyObj->m_PlayerData->m_iResets, true);
					}
					else
					{
						fEventBonus = g_BonusEvent.GetAddExp();
						fMapBonus = g_MapAttr.GetExpBonus(lpPartyObj->MapNumber);
						fBaseExp = g_ExpManager.GetExpMultiplier(lpPartyObj->Level, lpPartyObj->m_PlayerData->m_iResets, false);
					}

					exp *= (fBaseExp + fVipBonus + fEventBonus + fMapBonus);

					if ( g_CrywolfSync.GetOccupationState() == 1 && g_CrywolfSync.GetApplyPenalty() == TRUE)
					{
						exp =  (exp * g_CrywolfSync.GetGettingExpPenaltyRate()) / 100;
					}

					if (g_MasterLevelSkillTreeSystem.CheckMLGetExp(lpPartyObj, lpTargetObj) == FALSE)
					{
						exp = 0;
					}

					if ( exp > 0 )
					{
						if ( lpPartyObj->Type == OBJ_USER )
						{
							CheckItemOptForGetExpExRenewal(lpPartyObj, NULL, exp, dwDefaultExp, false);

							if (g_MasterLevelSkillTreeSystem.IsMasterLevelUser(lpPartyObj) )
							{
								lpPartyObj->m_PlayerData->MasterExperience += exp;
							}
							else
							{
								lpPartyObj->m_PlayerData->Experience += exp;
							}

							lpPartyObj->m_nEventExp += exp;

							if ( gObjLevelUp(lpPartyObj, exp, lpTargetObj->Class, "Devil Square Monster (Party)") == false )
							{
								continue;
							}
						}
					}

					if ( lpPartyObj->Type == OBJ_USER && exp > 0 )
					{
						GSProtocol.GCKillPlayerExpSend(lpPartyObj->m_Index, lpTargetObj->m_Index, exp, AttackDamage, MSBFlag);
					}
				}
			}
		}
	}
}




void CDevilSquare::gObjMonsterScoreDivision(LPOBJ lpMonObj, LPOBJ lpObj, int AttackDamage, BOOL MSBFlag)
{
	::gObjMonsterHitDamageUserDel(lpMonObj);
	lpMonObj->MonsterMoneyDrop = 0;

	LPOBJ lpTargetObj;
	int HitIndex;
	int LastHitObjNum = ::gObjMonsterLastHitDamageUser(lpMonObj, HitIndex);

	if ( LastHitObjNum != -1 )
	{
		lpTargetObj = &gObj[LastHitObjNum];
		int lc5 = lpMonObj->sHD[HitIndex].HitDamage / lpMonObj->MaxLife * lpMonObj->Level;
		lc5 *= lpTargetObj->m_bDevilSquareIndex + 1;
		lpTargetObj->m_nEventScore += lc5;
		g_QuestExpProgMng.ChkUserQuestTypeEventMap(QUESTEXP_ASK_DEVILSQUARE_POINT_GAIN, lpTargetObj, lpTargetObj->m_bDevilSquareIndex, 2);
	}
}



void CDevilSquare::ItemClear()
{
	ULONGLONG CurTime = GetTickCount64();

	for ( int i=0;i<g_ConfigRead.server.GetObjectMaxItem();i++)
	{
		MapC[MAP_INDEX_DEVILSQUARE].m_cItem[i].m_State = 8;
	}
}



void CDevilSquare::CalcScore()
{
	for ( int n=0;n<MAX_DEVILSQUARE_GROUND;n++)
	{
		this->m_DevilSquareGround[n].ClearScore();
	}

	for (int n = g_ConfigRead.server.GetObjectStartUserIndex();n<g_ConfigRead.server.GetObjectMax();n++)
	{
		if ( gObj[n].Connected == PLAYER_PLAYING )
		{
			if ( DS_MAP_RANGE(gObj[n].MapNumber) != FALSE )
			{
				if ( DS_LEVEL_RANGE(gObj[n].m_bDevilSquareIndex) != FALSE )
				{
					this->m_DevilSquareGround[gObj[n].m_bDevilSquareIndex].InsertObj(&gObj[n]);
				}
			}
		}
	}

	for (int n=0;n<MAX_DEVILSQUARE_GROUND;n++)
	{
		this->m_DevilSquareGround[n].SortScore();
		this->m_DevilSquareGround[n].SendScore();
	}
}

void CDevilSquare::gObjScoreClear()
{
	for (int n = g_ConfigRead.server.GetObjectStartUserIndex();n<g_ConfigRead.server.GetObjectMax();n++)
	{
		if ( gObj[n].Connected == PLAYER_PLAYING )
		{
			if ( DS_MAP_RANGE(gObj[n].MapNumber) != FALSE )
			{
				gObj[n].m_nEventScore = 0;
				gObj[n].m_nEventMoney = 0;
				gObj[n].m_nEventExp = 0;
			}
		}
	}
}

void CDevilSquare::CheckInvalidUser()
{
	for (int n = g_ConfigRead.server.GetObjectStartUserIndex();n<g_ConfigRead.server.GetObjectMax();n++)
	{
		if ( gObj[n].Connected == PLAYER_PLAYING )
		{
			if ( DS_MAP_RANGE(gObj[n].MapNumber) != FALSE )
			{
				if ( this->m_eState == DevilSquare_CLOSE )
				{
					g_Log.AddC(TColor::Red,  "[DevilSquare] [%s][%s] Found user in DevilSquare [State:Close]",
						gObj[n].AccountID, gObj[n].Name);
					gObjUserKill(gObj[n].m_Index);
				}
			}
		}
	}
}

BOOL CDevilSquare::AddUser(BYTE cSquareNumber, int aIndex)
{
	if ( DS_LEVEL_RANGE(cSquareNumber) == FALSE )
	{
		return FALSE;
	}

	return this->m_DevilSquareGround[cSquareNumber].AddUser(aIndex);
}

BOOL CDevilSquare::DelUser(BYTE cSquareNumber, int aIndex)
{
	if ( DS_LEVEL_RANGE(cSquareNumber) == FALSE )
	{
		return FALSE;
	}

	return this->m_DevilSquareGround[cSquareNumber].DelUser(aIndex);
}

BOOL CDevilSquare::ChangeUserIndex(int iOldIndex, int iNewIndex, BYTE cSquareNumber)
{
	if ( DS_LEVEL_RANGE(cSquareNumber) == FALSE )
	{
		return FALSE;
	}

	return this->m_DevilSquareGround[cSquareNumber].ChangerUserIndex(iOldIndex, iNewIndex);
}

int CDevilSquare::GetPlayUserCountRightNow(BYTE cSquareNumber)
{
	if ( DS_LEVEL_RANGE(cSquareNumber) == FALSE )
	{
		return -1;
	}

	return this->m_DevilSquareGround[cSquareNumber].GetPlayUserCountRightNow();
}

BOOL CDevilSquare::LeaveDevilSquare(BYTE cSquareNumber, int aIndex)
{
	return this->DelUser(cSquareNumber, aIndex);
}

void CDevilSquare::IncObjCount(BYTE cSquareNumber)
{
	if (DS_LEVEL_RANGE(cSquareNumber) == FALSE)
	{
		return;
	}

	this->m_DevilSquareGround[cSquareNumber].IncObjCount();
}

int CDevilSquare::GetObjCount(BYTE cSquareNumber)
{
	if (DS_LEVEL_RANGE(cSquareNumber) == FALSE)
	{
		return -1;
	}

	return this->m_DevilSquareGround[cSquareNumber].GetObjCount();
}

BOOL CDevilSquare::GetPosition(int TableNum, int iEventLevel, short MapNumber, short & x, short & y)
{
	int count = 100;
	BYTE attr;
	int tx;
	int ty;
	int w;
	int h;

	if (DS_LEVEL_RANGE(iEventLevel) == FALSE)
	{
		return false;
	}

	if (TableNum < 0 || TableNum >= 10)
	{
		return false;
	}

	while (count-- != 0)
	{
		w = this->m_DSMP[iEventLevel][TableNum].m_W - this->m_DSMP[iEventLevel][TableNum].m_X;
		h = this->m_DSMP[iEventLevel][TableNum].m_H - this->m_DSMP[iEventLevel][TableNum].m_Y;

		if (w < 1)
		{
			w = 1;
		}

		if (h < 1)
		{
			h = 1;
		}

		int iRX = GetLargeRand() % w;
		int iRY = GetLargeRand() % h;

		tx = this->m_DSMP[iEventLevel][TableNum].m_X + iRX;
		ty = this->m_DSMP[iEventLevel][TableNum].m_Y + iRY;
		attr = MapC[MapNumber].GetAttr(tx, ty);

		if (((attr & 1) != 1) && ((attr & 4) != 4) && ((attr & 8) != 8))
		{
			x = tx;
			y = ty;
			return TRUE;
		}
	}

	return false;
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

