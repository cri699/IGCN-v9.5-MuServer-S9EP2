//////////////////////////////////////////////////////////////////////
// DevilSquareGround.cpp
#include "stdafx.h"
#include "DevilSquareGround.h"
#include "DevilSquare.h"
#include "GameMain.h"
#include "TLog.h"
#include "winutil.h"
#include "CrywolfSync.h"
#include "CashShop.h"
#include "BuffEffectSlot.h"
#include "QuestExpProgMng.h"
#include "configread.h"

// GS-N 0.99.60T - 0x00457190 
// GS-N	1.00.18	JPN	0x004667C0	-	Completed

bool DevilSquareScoreSort(LPOBJ const & lhs,LPOBJ const & rhs)
{
	if ( lhs->m_nEventScore == rhs->m_nEventScore )
	{
		if ( lhs->Level < rhs->Level)
		{
			return true;
		}

		return false;
	}

	if ( lhs->m_nEventScore > rhs->m_nEventScore)
	{
		return true;
	}

	return false;
}


CDevilSquareGround::CDevilSquareGround()
{
	this->Init(0);
	InitializeCriticalSection(&this->m_criti);
}



CDevilSquareGround::~CDevilSquareGround()
{
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

	DeleteCriticalSection(&this->m_criti);
}



void CDevilSquareGround::Init(int Index)
{
	this->m_dwObjCount = 0;
	this->m_iIndex = Index;
	memset(this->m_DevilSquareMonsterInfo, 0xFF, sizeof(this->m_DevilSquareMonsterInfo));
	memset(this->m_DevilSquareBossMonsterInfo, 0xFF, sizeof(this->m_DevilSquareBossMonsterInfo));
	memset(this->m_iUserIndex, 0xFF, sizeof(this->m_iUserIndex));
}



void CDevilSquareGround::Clear()
{
	this->m_dwObjCount = 0;
	memset(this->m_iUserIndex, 0xFF, sizeof(this->m_iUserIndex));
}



void CDevilSquareGround::Set(WORD type, int starttime, int endtime)
{
	for (int i=0;i<MAX_ST_DEVILSQUARE_MONSTER;i++)
	{
		if ( this->m_DevilSquareMonsterInfo[i].m_Type == (WORD)-1  )
		{
			this->m_DevilSquareMonsterInfo[i].m_Type = type;
			this->m_DevilSquareMonsterInfo[i].m_RegenStartTime = starttime;
			this->m_DevilSquareMonsterInfo[i].m_RegenEndTime = endtime;
			this->m_DevilSquareMonsterInfo[i].m_iDevilSquareNumber = this->m_iIndex;
			
			return;
		}
	}
}



void CDevilSquareGround::SetBoss(WORD type, int starttime, int x, int y, int tx, int ty)
{
	for (int i=0;i<MAX_ST_DEVILSQUARE_MONSTER;i++)
	{
		if ( this->m_DevilSquareBossMonsterInfo[i].m_Type == (WORD)-1  )
		{
			this->m_DevilSquareBossMonsterInfo[i].m_Type = type;
			this->m_DevilSquareBossMonsterInfo[i].m_RegenStartTime = starttime;
			this->m_DevilSquareBossMonsterInfo[i].X = x;
			this->m_DevilSquareBossMonsterInfo[i].Y = y;
			this->m_DevilSquareBossMonsterInfo[i].TX = tx;
			this->m_DevilSquareBossMonsterInfo[i].TY = ty;
			this->m_DevilSquareBossMonsterInfo[i].m_iDevilSquareNumber = this->m_iIndex;
			
			return;
		}
	}
}



void CDevilSquareGround::SetBonus(int index, int exp, int zen)
{
	this->m_Bonus[index][0] = exp;
	this->m_Bonus[index][1] = zen;
}

void CDevilSquareGround::ApplyBonusRate(float fRewardExpRate)
{
	for (int i = 0; i < 4; i++)
	{
		this->m_Bonus[i][0] *= fRewardExpRate;
	}
}


void CDevilSquareGround::IncObjCount()
{
	InterlockedIncrement((LPLONG)&this->m_dwObjCount);
}



long  CDevilSquareGround::GetObjCount()
{
	return this->m_dwObjCount;
}



void CDevilSquareGround::RegenBossMonster(int currtime)
{
	for ( int i=0;i<MAX_ST_DEVILSQUARE_MONSTER;i++)
	{
		if ( this->m_DevilSquareBossMonsterInfo[i].m_Type != (WORD)-1 )
		{
			if ( currtime == this->m_DevilSquareBossMonsterInfo[i].m_RegenStartTime )
			{
				int iMapNumber = (this->m_DevilSquareBossMonsterInfo[i].m_iDevilSquareNumber >= DEVIL_SQUARE_GROUND_1&& this->m_DevilSquareBossMonsterInfo[i].m_iDevilSquareNumber <= DEVIL_SQUARE_GROUND_4)?MAP_INDEX_DEVILSQUARE:(this->m_DevilSquareBossMonsterInfo[i].m_iDevilSquareNumber>=DEVIL_SQUARE_GROUND_5  && this->m_DevilSquareBossMonsterInfo[i].m_iDevilSquareNumber <= DEVIL_SQUARE_GROUND_7 )?MAP_INDEX_DEVILSQUARE2:-1;

				int iMonIndex = gObjAddMonster(iMapNumber);

				if ( iMonIndex >= 0 )
				{
					gObjSetMonster(iMonIndex, this->m_DevilSquareBossMonsterInfo[i].m_Type);
					gObj[iMonIndex].MapNumber = iMapNumber;
					gMSetBase.GetBoxPosition(gObj[iMonIndex].MapNumber,
						this->m_DevilSquareBossMonsterInfo[i].X, this->m_DevilSquareBossMonsterInfo[i].Y,
						this->m_DevilSquareBossMonsterInfo[i].TX, this->m_DevilSquareBossMonsterInfo[i].TY,
						gObj[iMonIndex].X, gObj[iMonIndex].Y);
					int map = gObj[iMonIndex].MapNumber;
					int x = gObj[iMonIndex].X;
					int y = gObj[iMonIndex].Y;

					g_Log.Add("[DevilSquare] Boss %d %d %d create ", map, x, y);

					gObj[iMonIndex].TX = gObj[iMonIndex].X;
					gObj[iMonIndex].TY = gObj[iMonIndex].Y;
					gObj[iMonIndex].MTX = gObj[iMonIndex].X;
					gObj[iMonIndex].MTY = gObj[iMonIndex].Y;
					gObj[iMonIndex].StartX = gObj[iMonIndex].X;
					gObj[iMonIndex].StartY = gObj[iMonIndex].Y;
					gObj[iMonIndex].m_bDevilSquareIndex = this->m_iIndex;
					gObj[iMonIndex].DieRegen = FALSE;
					gObj[iMonIndex].m_PosNum = -1;
					gObj[iMonIndex].Live = TRUE;
					gObj[iMonIndex].m_State = 1;
				}
			}
		}
	}
}




BOOL CDevilSquareGround::FindMonsterType(WORD type)
{
	for ( int i=0;i<MAX_ST_DEVILSQUARE_MONSTER;i++)
	{
		if ( this->m_DevilSquareMonsterInfo[i].m_Type == type )
		{
			return TRUE;
		}
	}

	return FALSE;
}



WORD CDevilSquareGround::GetMonsterType(int currtime)
{
	WORD sTemp[MAX_ST_DEVILSQUARE_MONSTER];
	int icount = 0;

	for ( int i=0;i<MAX_ST_DEVILSQUARE_MONSTER;i++)
	{
		if ( this->m_DevilSquareMonsterInfo[i].m_Type != (WORD)-1 )
		{
			if ( currtime >= this->m_DevilSquareMonsterInfo[i].m_RegenStartTime )
			{
				if ( currtime <= this->m_DevilSquareMonsterInfo[i].m_RegenEndTime )
				{
					sTemp[icount] = this->m_DevilSquareMonsterInfo[i].m_Type;
					icount++;
				}
			}
		}
	}

	if ( icount == 0 )
	{
		return (BYTE)-1;
	}

	return sTemp[rand()%icount];
}




void CDevilSquareGround::ClearScore()
{
	this->m_DevilSquareRankList.clear();
}



void CDevilSquareGround::SortScore()
{
	std::sort( this->m_DevilSquareRankList.begin(), this->m_DevilSquareRankList.end(), DevilSquareScoreSort);
}



void CDevilSquareGround::InsertObj(LPOBJ lpObj)
{
	if ( lpObj->Authority != 2 )
	{
		if ( lpObj->Class >= 0 && lpObj->Class < MAX_TYPE_PLAYER)
		{
			if ( DS_LEVEL_RANGE(lpObj->m_bDevilSquareIndex) != FALSE )
			{
				lpObj->m_nEventScore +=  this->m_BonusScoreTable[lpObj->Class][lpObj->m_bDevilSquareIndex] / 100;
			}
		}

		this->m_DevilSquareRankList.push_back(lpObj);
	}
}



void CDevilSquareGround::SendScore()
{
	if ( this->m_DevilSquareRankList.size() < 1 )
	{
		return;
	}

	BYTE count = 1;
	int iUserCount = this->m_DevilSquareRankList.size();
	UINT64 iExp = 0;
	std::vector<OBJECTSTRUCT *>::iterator Itor;
	for ( Itor = this->m_DevilSquareRankList.begin() ; Itor != this->m_DevilSquareRankList.end() ; Itor++ )
	{
		memcpy(this->m_DevilSquareScoreInfoTOP10.Score[count].Name , (*(Itor))->Name, MAX_ACCOUNT_LEN);
		this->m_DevilSquareScoreInfoTOP10.Score[count].TotalScore = (*(Itor))->m_nEventScore;

		if ( iUserCount <= 6 )
		{
			if ( count < 4 )	// For DS from 0 to 3
			{
				this->m_DevilSquareScoreInfoTOP10.Score[count].BonusZen = this->m_Bonus[count-1][0];
				this->m_DevilSquareScoreInfoTOP10.Score[count].BonusExp = this->m_Bonus[count-1][1];
			}
			else
			{
				this->m_DevilSquareScoreInfoTOP10.Score[count].BonusZen = this->m_Bonus[3][0];
				this->m_DevilSquareScoreInfoTOP10.Score[count].BonusExp = this->m_Bonus[3][1];
			}
		}
		else
		{
			int lc5 = count*100/iUserCount;

			if ( count == 1 )
			{
				this->m_DevilSquareScoreInfoTOP10.Score[count].BonusZen = this->m_Bonus[0][0];
				this->m_DevilSquareScoreInfoTOP10.Score[count].BonusExp = this->m_Bonus[0][1];
			}
			else if ( lc5 <= 30 )
			{
				this->m_DevilSquareScoreInfoTOP10.Score[count].BonusZen = this->m_Bonus[1][0];
				this->m_DevilSquareScoreInfoTOP10.Score[count].BonusExp = this->m_Bonus[1][1];
			}
			else if ( lc5 <= 50 )
			{
				this->m_DevilSquareScoreInfoTOP10.Score[count].BonusZen = this->m_Bonus[2][0];
				this->m_DevilSquareScoreInfoTOP10.Score[count].BonusExp = this->m_Bonus[2][1];
			}
			else
			{
				this->m_DevilSquareScoreInfoTOP10.Score[count].BonusZen = this->m_Bonus[3][0];
				this->m_DevilSquareScoreInfoTOP10.Score[count].BonusExp = this->m_Bonus[3][1];
			}
		}

		count++;

		if ( count >= 10 )
		{
			break;
		}
	}

	this->m_DevilSquareScoreInfoTOP10.Count  = count;
	int iSize = count * 24 + 5;

	PHeadSetB((LPBYTE)&this->m_DevilSquareScoreInfoTOP10, 0x93, iSize);
	count = 1;

	Itor = this->m_DevilSquareRankList.begin();

	g_Log.Add("[DevilSquare] Rank [%d]", this->m_iIndex);

	for ( ; Itor != this->m_DevilSquareRankList.end(); Itor++ )
	{
		if ( iUserCount <= 6 )
		{
			if ( count < 3 )
			{
				this->m_DevilSquareScoreInfoTOP10.Score[0].BonusZen = this->m_Bonus[count-1][0];
				this->m_DevilSquareScoreInfoTOP10.Score[0].BonusExp = this->m_Bonus[count-1][1];
			}
			else
			{
				this->m_DevilSquareScoreInfoTOP10.Score[0].BonusZen = this->m_Bonus[3][0];
				this->m_DevilSquareScoreInfoTOP10.Score[0].BonusExp = this->m_Bonus[3][1];
			}
		}
		else
		{
			int lc7 = count*100/iUserCount;

			if ( count ==1 )
			{
				this->m_DevilSquareScoreInfoTOP10.Score[0].BonusZen = this->m_Bonus[0][0];
				this->m_DevilSquareScoreInfoTOP10.Score[0].BonusExp = this->m_Bonus[0][1];
			}
			else if ( lc7 <= 30 )
			{
				this->m_DevilSquareScoreInfoTOP10.Score[0].BonusZen = this->m_Bonus[1][0];
				this->m_DevilSquareScoreInfoTOP10.Score[0].BonusExp = this->m_Bonus[1][1];
			}
			else if ( lc7 <= 50 )
			{
				this->m_DevilSquareScoreInfoTOP10.Score[0].BonusZen = this->m_Bonus[2][0];
				this->m_DevilSquareScoreInfoTOP10.Score[0].BonusExp = this->m_Bonus[2][1];
			}
			else
			{
				this->m_DevilSquareScoreInfoTOP10.Score[0].BonusZen = this->m_Bonus[3][0];
				this->m_DevilSquareScoreInfoTOP10.Score[0].BonusExp = this->m_Bonus[3][1];
			}
		}

		if( (*(Itor))->Type != OBJ_USER)
		{
			continue;
		}

		if ( g_CrywolfSync.GetOccupationState() == 1 && g_CrywolfSync.GetApplyPenalty() == TRUE)
		{
			this->m_DevilSquareScoreInfoTOP10.Score[0].BonusExp =  (this->m_DevilSquareScoreInfoTOP10.Score[0].BonusExp * g_CrywolfSync.GetGettingExpPenaltyRate()) / 100;
		}

		iExp = this->m_DevilSquareScoreInfoTOP10.Score[0].BonusExp;
		CheckItemOptForGetExpExRenewal(*Itor, NULL, iExp, 0, true);

		(*(Itor))->m_PlayerData->Experience += iExp;

		bool ret = false;

		if (iExp > 0)
		{
			ret = gObjLevelUp((*(Itor)), iExp, 0, "Devil Square");
		}

		(*(Itor))->m_nEventMoney = this->m_DevilSquareScoreInfoTOP10.Score[0].BonusZen;

		if ( gObjCheckMaxZen((*(Itor))->m_Index, (*(Itor))->m_nEventMoney) == FALSE )
		{
			int Zen = MAX_ZEN - (*(Itor))->m_PlayerData->Money;
			(*(Itor))->m_PlayerData->Money += Zen;
		}
		else
		{
			(*(Itor))->m_PlayerData->Money += (*(Itor))->m_nEventMoney;
		}

		GSProtocol.GCMoneySend((*(Itor))->m_Index, (*(Itor))->m_PlayerData->Money);

		if ( ret == true )
		{
			if ( iExp > 0 )
			{
				GSProtocol.GCKillPlayerExpSend((*(Itor))->m_Index, (WORD)-1, iExp, 0, 0);
			}
		}

		this->SendRankingInfo((*(Itor)));

		g_Log.Add("Rank :[%d] : [%s][%s][%d][%d][%d]", count, (*(Itor))->AccountID, (*(Itor))->Name, (*(Itor))->m_nEventMoney, (*(Itor))->m_nEventExp, (*(Itor))->m_nEventScore);
		memcpy(this->m_DevilSquareScoreInfoTOP10.Score[0].Name , (*(Itor))->Name, MAX_ACCOUNT_LEN);
		this->m_DevilSquareScoreInfoTOP10.Score[0].TotalScore = (*(Itor))->m_nEventScore;

		if ( iExp == 0 )
		{
			this->m_DevilSquareScoreInfoTOP10.Score[0].BonusExp = 0;
		}
		else
		{
			this->m_DevilSquareScoreInfoTOP10.Score[0].BonusExp = iExp;
		}

		this->m_DevilSquareScoreInfoTOP10.MyRank = count;
		count++;

		IOCP.DataSend((*(Itor))->m_Index, (BYTE *)&this->m_DevilSquareScoreInfoTOP10 , iSize);

		g_CashShop.AddCoin((*(Itor)), EVENT_DS);
		g_QuestExpProgMng.ChkUserQuestTypeEventMap(QUESTEXP_ASK_DEVILSQUARE_CLEAR, (*(Itor)), (*(Itor))->m_bDevilSquareIndex, 1);

		(*(Itor))->m_nEventScore = 0;
		(*(Itor))->m_nEventMoney = 0;
		(*(Itor))->m_nEventExp = 0;
	}
}




void CDevilSquareGround::SendRankingInfo(LPOBJ lpObj)
{
	if ( lpObj->m_nEventScore <= 0 )
	{
		return;
	}

	PMSG_ANS_EVENTUSERSCORE pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0xBD;
	pMsg.h.subcode = 0x01;
	pMsg.h.size = sizeof(pMsg);
	pMsg.Score = lpObj->m_nEventScore;
	pMsg.SquareNum = lpObj->m_bDevilSquareIndex;
	pMsg.Class = lpObj->Class;
	pMsg.ServerCode = g_ConfigRead.server.GetGameServerCode();
	memcpy(pMsg.AccountID, lpObj->AccountID, MAX_ACCOUNT_LEN);
	memcpy(pMsg.GameID, lpObj->Name, MAX_ACCOUNT_LEN);

	wsDataCli.DataSend((char *)&pMsg, pMsg.h.size);
}

BOOL CDevilSquareGround::AddUser(int aIndex)
{
	EnterCriticalSection(&this->m_criti);

	for(int i=0;i<10;i++)
	{
		if(this->m_iUserIndex[i] == -1)
		{
			this->m_iUserIndex[i] = aIndex;
			LeaveCriticalSection(&this->m_criti);
			return TRUE;
		}
	}

	LeaveCriticalSection(&this->m_criti);
	return FALSE;

}

BOOL CDevilSquareGround::DelUser(int aIndex)
{
	EnterCriticalSection(&this->m_criti);

	for(int i=0;i<10;i++)
	{
		if(this->m_iUserIndex[i] == aIndex)
		{
			this->m_iUserIndex[i] = -1;
			LeaveCriticalSection(&this->m_criti);
			return TRUE;
		}
	}

	LeaveCriticalSection(&this->m_criti);
	return FALSE;
}

BOOL CDevilSquareGround::ChangerUserIndex(int iOldIndex, int iNewIndex)
{
	EnterCriticalSection(&this->m_criti);

	for(int i=0;i<10;i++)
	{
		if(this->m_iUserIndex[i] == iOldIndex)
		{
			this->m_iUserIndex[i] = iNewIndex;
			LeaveCriticalSection(&this->m_criti);
			return TRUE;
		}
	}

	LeaveCriticalSection(&this->m_criti);
	return FALSE;
}

int CDevilSquareGround::GetPlayUserCountRightNow()
{
	int Count = 0;

	for(int i=0;i<10;i++)
	{
		if ( this->m_iUserIndex[i] != -1 )
		{
			if ( ObjectMaxRange(this->m_iUserIndex[i]) == TRUE )
			{
				if ( gObj[this->m_iUserIndex[i]].Connected == PLAYER_PLAYING )
				{
					Count++;
				}
			}
		}
	}

	return Count;
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

