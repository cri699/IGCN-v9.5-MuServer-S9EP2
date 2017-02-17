//////////////////////////////////////////////////////////////////////
// DoppelGanger.cpp
#include "stdafx.h"
#include "DoppelGanger.h"
#include "TLog.h"
#include "gObjMonster.h"
#include "MonsterAttr.h"
#include "GameMain.h"
#include "BuffEffectSlot.h"
#include "winutil.h"
#include "configread.h"
#include "TNotice.h"
#include "BloodCastle.h"
#include "DevilSquare.h"
#include "ChaosCastle.h"
#include "DoppelgangerLua.h"
#include "LuaFun.h"
#include "PersonalStore.h"
#include "CashShop.h"

CDoppelGanger g_DoppelGanger;

CDoppelGangerMonsterHerd::CDoppelGangerMonsterHerd(void)
{
	this->m_nPosIndex = 0;
	this->m_nStartPosInfo = 0;
	this->m_nEndPosInfo = 0;
	this->m_bMove = TRUE;
}

CDoppelGangerMonsterHerd::~CDoppelGangerMonsterHerd(void)
{
}

BOOL CDoppelGangerMonsterHerd::Start()
{
	return this->MonsterHerd::Start();
}

BOOL CDoppelGangerMonsterHerd::MonsterHerdItemDrop(OBJECTSTRUCT* lpObj)
{
	return FALSE;
}

void CDoppelGangerMonsterHerd::MonsterAttackAction(OBJECTSTRUCT *lpObj, OBJECTSTRUCT *lpTargetObj)
{
	if (!lpObj)
	{
		return;
	}

	if (gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_STONE) == TRUE)
	{
		return;
	}

	if (!lpTargetObj)
	{
		return;
	}

	if (lpObj->Connected < PLAYER_PLAYING)
	{
		return;
	}

	if (lpObj->Type != OBJ_USER)
	{
		return;
	}

	if (lpTargetObj->Connected < PLAYER_PLAYING)
	{
		return;
	}

	lpObj->NextActionTime = 1000;
}

int CDoppelGangerMonsterHerd::AddMonsterEX(int iMonsterType, int bAttackFirst, int nMonsterLevel, int nMonsterHp, int nMonsterAttMin, int nMonsterAttMax, int nMonsterDef)
{
	if (!this->m_bHasInfo)
	{
		return -1;
	}

	BYTE btMapNumber = this->m_iMapNumber;
	BYTE cX, cY;

	if (!this->GetRandomLocation(cX, cY))
	{
		return -1;
	}

	int iIndex = gObjAddMonster(btMapNumber);

	if (iIndex < 0)
	{
		g_Log.Add("[Doppelganger] AddMonsterEx Fail(%d) [Class: %d][%d][%d, %d]", iIndex, iMonsterType, this->m_iMapNumber, cX, cY);
		return -1;
	}

	gObj[iIndex].m_PosNum = -1;
	gObj[iIndex].X = cX;
	gObj[iIndex].Y = cY;
	gObj[iIndex].MapNumber = this->m_iMapNumber;
	gObj[iIndex].TX = gObj[iIndex].X;
	gObj[iIndex].TY = gObj[iIndex].Y;
	gObj[iIndex].m_OldX = gObj[iIndex].X;
	gObj[iIndex].m_OldY = gObj[iIndex].Y;
	gObj[iIndex].StartX = gObj[iIndex].X;
	gObj[iIndex].StartY = gObj[iIndex].Y;

	LPMONSTER_ATTRIBUTE iAttr = gMAttr.GetAttr(iMonsterType);

	if (iAttr == NULL)
	{
		gObjDel(iIndex);
		return -1;
	}

	gObj[iIndex].Level = iAttr->m_Level;
	gObjSetMonster(iIndex, iMonsterType);

	gObj[iIndex].Level = nMonsterLevel;
	gObj[iIndex].Life = nMonsterHp;
	gObj[iIndex].MaxLife = nMonsterHp;
	gObj[iIndex].m_Defense = nMonsterDef;
	gObj[iIndex].m_AttackDamageMin = nMonsterAttMin;
	gObj[iIndex].m_AttackDamageMax = nMonsterAttMax;
	gObj[iIndex].m_MagicDefense = nMonsterDef;
	gObj[iIndex].MaxRegenTime = 1000;
	gObj[iIndex].Dir = rand() % 8;
	gObj[iIndex].m_bIsInMonsterHerd = true;
	gObj[iIndex].m_bIsMonsterAttackFirst = bAttackFirst;
	gObj[iIndex].m_lpMonsterHerd = this;

	EnterCriticalSection(&this->m_critMonsterHerd);

	_MONSTER_HERD_DATA pMonsterData;
	pMonsterData.m_iIndex = iIndex;
	pMonsterData.m_iType = iMonsterType;
	pMonsterData.m_iX = cX;
	pMonsterData.m_iY = cY;
	pMonsterData.m_bRegen = FALSE;

	this->m_mapMonsterHerd.insert(std::pair<int, _MONSTER_HERD_DATA>(iIndex, pMonsterData));

	LeaveCriticalSection(&this->m_critMonsterHerd);

	return iIndex;
}

BOOL CDoppelGangerMonsterHerd::GetRandomLocation(BYTE &cX, BYTE &cY)
{
	if (this->m_bHasInfo == 0)
	{
		return FALSE;
	}

	if (MAX_MAP_RANGE(this->m_iMapNumber) == FALSE)
	{
		return FALSE;
	}

	int iCount = 100;

	while (iCount-- != 0)
	{
		cX = rand() % (this->m_iRADIUS + 1) * (((rand() % 2 == 0) ? -1 : 1)) + this->m_iCUR_X;
		cY = rand() % (this->m_iRADIUS + 1) * (((!(rand() % 2))) ? -1 : 1) + this->m_iCUR_Y;

		BYTE btMapAttr = MapC[this->m_iMapNumber].GetAttr(cX, cY);

		if (btMapAttr == 0 || btMapAttr == 2)
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CDoppelGangerMonsterHerd::SetTotalInfo(int iMapNumber, int iRadius, int nPosInfo, int bMove)
{
	if (MAX_MAP_RANGE(iMapNumber) == FALSE)
	{
		return FALSE;
	}

	if (((iRadius < 0) ? FALSE : (iRadius > 15) ? FALSE : TRUE) == FALSE)
	{
		return FALSE;
	}

	BYTE btX;
	BYTE btY;
	g_DoppelGanger.m_PosInfo.GetCenterPosition(iMapNumber, nPosInfo, &btX, &btY);
	this->m_iMapNumber = iMapNumber;
	this->m_iCUR_X = btX;
	this->m_iCUR_Y = btY;
	this->m_iRADIUS = iRadius;
	this->m_nStartPosInfo = nPosInfo;
	this->m_nPosIndex = nPosInfo;
	this->m_bMove = bMove;
	this->m_bHasInfo = TRUE;

	return TRUE;
}

void CDoppelGangerMonsterHerd::MonsterBaseAct(LPOBJ lpObj)
{
	LPOBJ lpTargetObj = NULL;

	if (lpObj->TargetNumber >= 0)
	{
		lpTargetObj = &gObj[lpObj->TargetNumber];
	}
	else
	{
		lpObj->m_ActState.Emotion = 0;
	}

	switch (lpObj->m_ActState.Emotion)
	{
	case 0:
	{
		if (lpObj->m_ActState.Attack != 0)
		{
			lpObj->m_ActState.Attack = 0;
			lpObj->TargetNumber = -1;
			lpObj->NextActionTime = 100;
		}

		if (lpObj->m_MoveRange > 0 &&
			!gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_FREEZE_2) &&
			!gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_EARTH_BINDS) &&
			!gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_STONE))
		{
			this->MonsterMoveAction(lpObj);
		}

		if (lpObj->Class == 529 || lpObj->Class == 530)
		{
			g_DoppelGanger.AngerKillerAttack(lpObj);
		}

		if (lpObj->m_bIsMonsterAttackFirst)
		{
			lpObj->TargetNumber = gObjMonsterSearchEnemy(lpObj, OBJ_USER);

			if (lpObj->TargetNumber >= 0)
			{
				if (lpObj->Class == 533)
				{
					lpObj->m_ActState.EmotionCount = 30;
					lpObj->NextActionTime = 1000;
					lpObj->m_ActState.Move = 0;
					lpObj->PathStartEnd = 0;
				}

				else
				{
					lpObj->m_ActState.EmotionCount = 10;
				}

				lpObj->m_ActState.Emotion = 1;
			}
		}
	}
	break;

	case 1:
		if (lpObj->Class == 529 || lpObj->Class == 530)
		{
			lpObj->m_ActState.Emotion = 0;
			lpObj->m_ActState.EmotionCount = 0;
		}

		if (lpObj->m_ActState.EmotionCount > 0)
		{
			lpObj->m_ActState.EmotionCount--;
		}

		else
		{
			lpObj->m_ActState.Emotion = 0;
		}

		if (lpObj->TargetNumber >= 0 && lpObj->PathStartEnd == 0)
		{
			int dis = gObjCalDistance(lpObj, lpTargetObj);
			int attackRange;

			if (lpObj->m_AttackType >= 100)
			{
				attackRange = lpObj->m_AttackRange + 2;
			}
			else
			{
				attackRange = lpObj->m_AttackRange;
			}

			if (dis > attackRange)
			{
				if (gObjMonsterGetTargetPos(lpObj) == TRUE)
				{
					if (MAX_MAP_RANGE(lpObj->MapNumber) == FALSE)
					{
						g_Log.AddC(TColor::Red, "[ERROR] MAX_MAP_RANGE (MonsterHerd DG) == FALSE (%d)", lpObj->MapNumber);
						break;
					}

					if (MapC[lpObj->MapNumber].CheckWall(lpObj->X, lpObj->Y, lpObj->MTX, lpObj->MTY) == TRUE)
					{
						lpObj->m_ActState.Move = 1;
						lpObj->NextActionTime = 100;
						lpObj->Dir = GetPathPacketDirPos(lpTargetObj->X - lpObj->X, lpTargetObj->Y - lpObj->Y);
					}

					else
					{
						this->MonsterMoveAction(lpObj);
						lpObj->m_ActState.Emotion = 0;
						lpObj->m_ActState.EmotionCount = 10;
					}
				}

				else
				{
					this->MonsterMoveAction(lpObj);
					lpObj->m_ActState.Emotion = 3;
					lpObj->m_ActState.EmotionCount = 10;
				}
			}

			else
			{
				int tuser = lpObj->TargetNumber;

				if (!ObjectMaxRange(tuser))
				{
					break;
				}

				int map = gObj[tuser].MapNumber;

				if (MAX_MAP_RANGE(map) == FALSE)
				{
					g_Log.AddC(TColor::Red, "[ERROR] MAX_MAP_RANGE (MonsterHerd DG) == FALSE (%d)", map);
					break;
				}

				if (MapC[map].CheckWall(lpObj->X, lpObj->Y, gObj[tuser].X, gObj[tuser].Y) == TRUE)
				{
					BYTE attr = MapC[map].GetAttr(gObj[tuser].X, gObj[tuser].Y);

					if ((attr & 1) != 1)
					{
						lpObj->m_ActState.Attack = 1;
					}
					else
					{
						lpObj->TargetNumber = -1;
						lpObj->m_ActState.Emotion = 1;
					}

					lpObj->Dir = GetPathPacketDirPos(lpTargetObj->X - lpObj->X, lpTargetObj->Y - lpObj->Y);
					lpObj->NextActionTime = 100;
				}
			}
		}
		break;

	case 3:
		if (lpObj->m_ActState.EmotionCount > 0)
		{
			lpObj->m_ActState.EmotionCount--;
		}
		else
		{
			lpObj->m_ActState.Emotion = 0;
		}

		lpObj->m_ActState.Move = 0;
		lpObj->m_ActState.Attack = 0;
		lpObj->NextActionTime = 400;

		break;
	}
}

BOOL CDoppelGangerMonsterHerd::MoveHerd()
{
	if (this->m_bHasInfo == FALSE)
	{
		return FALSE;
	}

	if (MAX_MAP_RANGE(this->m_iMapNumber) == FALSE)
	{
		return FALSE;
	}

	int tmpMapInfoNum = this->m_iMapNumber - MAP_INDEX_DOUBLE_GOER1;
	int nNextPosIndex = this->m_nPosIndex + 1;

	if (nNextPosIndex > MAX_DOPPELGANGER_POS_INFO - 1)
		nNextPosIndex = MAX_DOPPELGANGER_POS_INFO - 1;

	if (!this->m_bMove)
		nNextPosIndex = this->m_nPosIndex;

	BYTE X = 0, Y = 0;

	g_DoppelGanger.m_PosInfo.GetCenterPosition(this->m_iMapNumber, nNextPosIndex, &X, &Y);

	BYTE tmpcX = X;
	BYTE tmpcY = Y;
	BYTE btMapAttr = MapC[this->m_iMapNumber].GetAttr(X, Y);

	if (btMapAttr & 1 || btMapAttr & 4 || btMapAttr & 8)
	{
		return FALSE;
	}

	else
	{
		this->m_iCUR_X = X;
		this->m_iCUR_Y = Y;
		return TRUE;
	}

	return FALSE;
}

void CDoppelGangerMonsterHerd::ArrangeMonsterHerd()
{
	if (this->IsActive() == TRUE)
	{
		std::map<int, _MONSTER_HERD_DATA>::iterator it = this->m_mapMonsterHerd.begin();

		while (true)
		{
			if (it == this->m_mapMonsterHerd.end())
			{
				break;
			}

			_MONSTER_HERD_DATA * pMHD = &it->second;

			if (gObj[pMHD->m_iIndex].Live == TRUE)
			{
				break;
			}

			it++;
		}

		if (it == this->m_mapMonsterHerd.end())
		{
			this->SetActive(FALSE);
		}
	}
}

int CDoppelGangerMonsterHerd::IsActive()
{
	return this->m_bHerdActive;
}

void CDoppelGangerMonsterHerd::SetActive(BOOL bActive)
{
	this->m_bHerdActive = bActive;
}

void CDoppelGangerMonsterHerd::SetPosIndex(int nPosIndex)
{
	this->m_nPosIndex = nPosIndex;
}

int CDoppelGangerMonsterHerd::GetMonsterSize()
{
	return this->m_mapMonsterHerd.size();
}

void CDoppelGangerMonsterHerd::SetEndPosition(int nPosIndex)
{
	this->m_nEndPosInfo = nPosIndex;
}

int CDoppelGangerMonsterHerd::GetPosIndex()
{
	return this->m_nPosIndex;
}

int CDoppelGangerMonsterHerd::GetEndPosIndex()
{
	return this->m_nEndPosInfo;
}

int CDoppelGangerMonsterHerd::IsMove()
{
	return this->m_bMove;
}

// CDoppelGanger

CDoppelGanger::CDoppelGanger(void)
{
	this->m_nHerdCnt = 0;
	this->m_nMapNumber = MAP_INDEX_DOUBLE_GOER1;
	this->m_nPartyNumber = -1;
	this->m_nCurUserCount = 0;
	this->m_nStartUserCount = 0;
	this->m_nMaxLevel = 0;
	this->m_nMinLevel = 0;

	for (int i = 0; i < MAX_DOPPELGANGER_BOSS_INFO; i++)
	{
		this->m_nMiddleBossIndex[i] = -1;
		this->m_bMiddleBossKill[i] = false;
	}

	for (int i = 0; i < MAX_DOPPELGANGER_ICEWORKER_INFO; i++)
		this->m_nIceWorkerIndex[i] = -1;

	for (int i = 0; i < MAX_TOTAL_GOLDEN_LARVA; i++)
		this->m_nLarvaIndex[i] = -1;

	for (int i = 0; i < MAX_DOPPELGANGER_TREASURE_BOX; i++)
	{
		if (this->m_nTeasureBoxIndex[i] != -1)
			this->m_nTeasureBoxIndex[i] = -1;
	}

	this->m_nLastTreasureBoxIndex = -1;
	this->m_bSuccessResult = false;
	this->m_bMissionResult = true;
	this->m_nFirstMonsterIndex = -1;
	this->m_btFirstIndex = 0;
	this->m_nGoalMonsterCnt = 0;
	this->m_bSendMsgBeforePlay = false;
	this->m_bTreasureBoxRegen = false;
	this->m_bSendIceWorkerDel = false;
	this->m_bSendDoppelGangerEnd = 0;
	this->m_nBossRegenOrder = 0;
	InitializeCriticalSection(&this->m_critUserData);
	InitializeCriticalSection(&this->m_CSMonsterHerd);
	InitializeCriticalSection(&this->m_critTreasureBox);
}

CDoppelGanger::~CDoppelGanger(void)
{
	DeleteCriticalSection(&this->m_critUserData);
	DeleteCriticalSection(&this->m_CSMonsterHerd);
	DeleteCriticalSection(&this->m_critTreasureBox);
}

void CDoppelGanger::DoppelgangerProcessInit()
{
	if (g_ConfigRead.server.GetStateFromEventTable(g_ConfigRead.server.GetServerType(), EV_TABLE_DG) == false)
	{
		this->SetEventEnable(false);
		return;
	}

	lua_State *pLuaState = this->m_MULua.GetLua();

	if (pLuaState == NULL)
	{
		g_Log.Add("[Doppelganger] - Error - [%s] [%d]", __FILE__, __LINE__);
		return;
	}

	Luna<CDoppelgangerLua>::Register(pLuaState);
	this->m_MULua.DoFile(g_ConfigRead.GetPath("Scripts\\Events\\DoppelGanger.lua"));

	g_Generic_Call(this->m_MULua.GetLua(), "FN_LuaDopplegangerInit", ">");

	this->m_PosInfo.LoadDoppelgangerPosInfo(g_ConfigRead.GetPath("\\Events\\DoppelGanger\\IGC_DoppelGanger_PositionInfo.xml"));
	this->m_DPEventInfo.LoadDoppelgangerEventInfo(g_ConfigRead.GetPath("\\Events\\DoppelGanger\\IGC_DoppelGanger_EventInfo.xml"));
	this->m_DPDropItemInfo.LoadDoppelgangerItemBag(g_ConfigRead.GetPath("\\Events\\DoppelGanger\\IGC_DoppelGanger_DropInfo.xml"));

	for (int Cnt = 0; Cnt < MAX_DOPPELGANGER_MONSTER_ATTR; Cnt++)
	{
		this->m_pDoppelGangerAttr[Cnt] = this->GetDefaultMonsterInfo(Cnt + 529);
	}

	this->SetDoppelgangerState(DOPPELGANGER_STATE_NONE);
}

void CDoppelGanger::DoppelgangerInfoLoad()
{
	CIniReader ReadDG(g_ConfigRead.GetPath("\\Events\\DoppelGanger\\IGC_DoppelGanger.ini"));

	bool bEnable = ReadDG.ReadInt("DoppelGanger", "Enable", 1);

	if (g_ConfigRead.server.GetStateFromEventTable(g_ConfigRead.server.GetServerType(), EV_TABLE_DG) == false)
	{
		bEnable = false;
	}

	this->m_bDoppelGangerEnable = bEnable;
	this->m_fDPExpRate = ReadDG.ReadFloat("DoppelGanger", "ExpMultiplier", 1.0);
	this->m_fDPMasterLevelExpRate = ReadDG.ReadFloat("DoppelGanger", "MasterExpMultiplier", 1.0);
	this->m_iMirrorDimensionDropRate = ReadDG.ReadInt("DoppelGanger", "SignOfDimensionDropRate", 10);
	this->m_iMirrorDimensionDropLevel = ReadDG.ReadInt("DoppelGanger", "SignOfDimensionMonsterMinLevelDrop", 32);
}

void CDoppelGanger::SetDoppelgangerState(BYTE btState)
{
	switch (btState)
	{
	case DOPPELGANGER_STATE_NONE:
		this->SetDoppelgangerStateNone();
		break;
	case DOPPELGANGER_STATE_READY:
		this->SetDoppelgangerStateReady();
		break;
	case DOPPELGANGER_STATE_PLAY:
		this->SetDoppelgangerStatePlaying();
		break;
	case DOPPELGANGER_STATE_END:
		this->SetDoppelgangerStateEnd();
		break;
	}

	this->m_i64StateTime = GetTickCount64();
	this->m_btDoppelGangerState = btState;
}

BYTE CDoppelGanger::GetDoppelgangerState()
{
	return this->m_btDoppelGangerState;
}

void CDoppelGanger::SetDoppelgangerStateNone()
{
	g_Log.Add("[Doppelganger] State NONE");

	this->m_bDoppelGangerEnter = false;
	this->m_nMinLevel = 0;
	this->m_nMaxLevel = 0;

	for (int Cnt = 0; Cnt < MAX_DOPPELGANGER_TREASURE_BOX; Cnt++)
	{
		if (this->m_nTeasureBoxIndex[Cnt] != -1)
		{
			gObjDel(this->m_nTeasureBoxIndex[Cnt]);
			this->m_nTeasureBoxIndex[Cnt] = -1;
		}
	}

	if (this->m_nLastTreasureBoxIndex != -1)
	{
		gObjDel(this->m_nLastTreasureBoxIndex);
		this->m_nLastTreasureBoxIndex = -1;
	}

	if (this->GetUserCount() > 0)
	{
		for (int i = 0; i<MAX_DOPPELGANGER_USER_INFO; i++)
		{
			if (this->m_UserData[i].IsUser() == TRUE && gObj[this->m_UserData[i].m_nIndex].Connected > PLAYER_LOGGED)
			{
				gObjMoveGate(this->m_UserData[i].m_nIndex, 267);
			}
		}
	}

	this->ClearUserData();
	this->m_nStartUserCount = 0;
}

void CDoppelGanger::SetDoppelgangerStateReady()
{
	g_Log.Add("[Doppelganger] State READY");

	this->m_nFirstMonsterIndex = -1;
	this->m_btFirstIndex = 0;
	this->m_nGoalMonsterCnt = 0;
	this->m_bSuccessResult = false;
	this->m_bMissionResult = true;
	this->m_bSendMsgBeforeReady = false;
	this->m_bSendMsgBeforePlay = false;
	this->m_bSendIceWorkerDel = false;
	this->m_bSendDoppelGangerEnd = false;

	for (int i = 0; i < MAX_DOPPELGANGER_ICEWORKER_INFO; i++)
	{
		this->m_nIceWorkerIndex[i] = -1;
	}

	for (int i = 0; i < MAX_TOTAL_GOLDEN_LARVA; i++)
	{
		this->m_nLarvaIndex[i] = -1;
	}

	for (int i = 0; i < MAX_DOPPELGANGER_BOSS_INFO; i++)
	{
		this->m_nMiddleBossIndex[i] = -1;
		this->m_bMiddleBossKill[i] = false;
	}

	this->m_btIceWorkState = 1;
	this->m_bTreasureBoxRegen = false;
	this->SetKillerState(0);
	this->SetAngerKillerState(0);
	this->m_nBossRegenOrder = 0;

	EnterCriticalSection(&this->m_critTreasureBox);

	this->m_bOpenTreasureBox = false;
	this->m_bOpenLastTreasureBox = false;

	LeaveCriticalSection(&this->m_critTreasureBox);

	this->SendDoppelgangerState(DOPPELGANGER_STATE_READY);
}

void CDoppelGanger::SetDoppelgangerStatePlaying()
{
	g_Log.Add("[Doppelganger] State PLAY");

	ULONGLONG i64CurrentTime = GetTickCount64();

	this->m_i64CallbackTime = i64CurrentTime;
	this->m_i64AddHerdMonsterTime = i64CurrentTime;
	this->m_i64AddBossMonsterTime = i64CurrentTime;
	this->m_i64MonserPosSendTime = i64CurrentTime;

	this->m_nStartUserCount = this->m_nCurUserCount;

	this->SetUserAverageLevel();

	if (this->m_DPEventInfo.SetDoppelgangerEventInfo(this->GetUserCount(), this->GetUserMaxLevel()) == NULL)
	{
		g_Log.Add("[Doppelganger] SetDoppelgangerInfo is NULL return");
		this->SetDoppelgangerState(3);
		return;
	}

	char PlayerLog[512];

	wsprintf(PlayerLog, "[Doppelganger] PlayUser [%d][%d] ", this->m_nMapNumber, this->m_nStartUserCount);

	EnterCriticalSection(&this->m_critUserData);

	for (int i = 0; i<MAX_DOPPELGANGER_USER_INFO; i++)
	{
		if (this->m_UserData[i].IsUser() == TRUE)
		{
			if (gObj[this->m_UserData[i].m_nIndex].Connected > PLAYER_LOGGED)
			{
				int len = strlen(PlayerLog);
				wsprintf(PlayerLog + len, "(%s)(%s) ", gObj[this->m_UserData[i].m_nIndex].AccountID, gObj[this->m_UserData[i].m_nIndex].Name);
			}
		}
	}

	LeaveCriticalSection(&this->m_critUserData);

	g_Log.Add(PlayerLog);

	this->SendDoppelgangerState(DOPPELGANGER_STATE_PLAY);
	this->SendMapTileInfoAll(1);
	this->SendMonsterGoalCount();
}

void CDoppelGanger::SetDoppelgangerStateEnd()
{
	g_Log.Add("[Doppelganger] State END");

	for (int i = 0; i < MAX_DOPPELGANGER_MONSTER_INFO; i++)
	{
		this->m_MonsterHerd[i].Stop();
		this->m_MonsterHerd[i].SetPosIndex(0);
	}

	this->m_nHerdCnt = 0;

	this->DelLarvaAll();
	this->SendDoppelgangerState(DOPPELGANGER_STATE_END);
	this->SendDoppelgangerResultAll();

	if (this->m_nCurUserCount > 0 && this->m_nGoalMonsterCnt < MAX_DOPPELGANGER_MONSTER_IN_PORTAL)
	{
		this->m_bSuccessResult = true;

		for (int Cnt = 0; Cnt<MAX_DOPPELGANGER_USER_INFO; Cnt++)
		{
			if (this->m_UserData[Cnt].IsUser() == TRUE && gObj[this->m_UserData[Cnt].m_nIndex].Connected > PLAYER_LOGGED)
			{
				g_CashShop.AddCoin(&gObj[this->m_UserData[Cnt].m_nIndex], EVENT_RAKLION);
				this->AddLastTreasureBox(gObj[this->m_UserData[Cnt].m_nIndex].X, gObj[this->m_UserData[Cnt].m_nIndex].Y);
				break;
			}
		}
	}
}

void CDoppelGanger::ProcDoppelganger(ULONGLONG i64CurTime)
{
	if (!this->m_bDoppelGangerEnable)
	{
		return;
	}

	switch (this->m_btDoppelGangerState)
	{
	case DOPPELGANGER_STATE_NONE:
		this->ProcDoppelgangerState_None(i64CurTime);
		break;
	case DOPPELGANGER_STATE_READY:
		this->ProcDoppelgangerState_Ready(i64CurTime);
		break;
	case DOPPELGANGER_STATE_PLAY:
		this->ProcDoppelgangerState_Playing(i64CurTime);
		break;
	case DOPPELGANGER_STATE_END:
		this->ProcDoppelgangerState_End(i64CurTime);
		break;
	default:
		return;
	}
}

void CDoppelGanger::ProcDoppelgangerState_None(ULONGLONG i64CurTime)
{
	if (this->m_bDoppelGangerEnter == false && this->m_i64StateTime > 30000)
	{
		this->m_bDoppelGangerEnter = true;
	}

	if (this->GetUserCount() > 0)
	{
		if (GetTickCount64() - this->m_i64UserEnterTime > 30000)
		{
			this->SetDoppelgangerState(DOPPELGANGER_STATE_READY);
		}
	}
}

void CDoppelGanger::ProcDoppelgangerState_Ready(ULONGLONG i64CurTime)
{
	if ((60000 * this->m_i64ReadyTime - (GetTickCount64() - this->m_i64StateTime)) <= 30000 && this->m_bSendMsgBeforePlay == false)
	{
		this->m_bSendMsgBeforePlay = true;
		this->SendDoppelgangerTimerMsg(17);
	}

	if (i64CurTime - this->m_i64StateTime > 60000 * this->m_i64ReadyTime)
	{
		this->SetDoppelgangerState(DOPPELGANGER_STATE_PLAY);
	}
}

void CDoppelGanger::ProcDoppelgangerState_Playing(ULONGLONG i64CurTime)
{
	if (!this->m_nCurUserCount)
	{
		this->SetDoppelgangerState(DOPPELGANGER_STATE_END);
		return;
	}

	if (i64CurTime - this->m_i64MonserPosSendTime >= 1000)
	{
		this->SendDoppelgangerMonsterPos();
		this->SendDoppelgangerUserPos();
		this->m_i64MonserPosSendTime = i64CurTime;
	}

	if (i64CurTime - this->m_i64CallbackTime >= 1000)
	{
		this->ArrangeMonsterHerd();

		g_Generic_Call(this->m_MULua.GetLua(), "FN_LuaDoppelgangerCallback", "l>", i64CurTime);
		this->m_i64CallbackTime = i64CurTime;
	}

	for (int i = 0; i < MAX_DOPPELGANGER_MONSTER_INFO; i++)
	{
		if (this->m_MonsterHerd[i].IsActive() == TRUE)
		{
			this->m_MonsterHerd[i].MoveHerd();
		}
	}

	if (this->m_bTreasureBoxRegen == true && GetTickCount64() - this->m_i64TeasureBoxTime >= 60000)
	{
		this->OpenTreasureBox();

		for (int i = 0; i < MAX_DOPPELGANGER_TREASURE_BOX; i++)
		{
			if (this->m_nTeasureBoxIndex[i] != -1)
			{
				gObjDel(this->m_nTeasureBoxIndex[i]);
				this->m_nTeasureBoxIndex[i] = -1;
			}
		}

		this->m_bTreasureBoxRegen = false;
	}

	if (this->m_bSendIceWorkerDel == false && this->m_btIceWorkState == 0 && GetTickCount64() - this->m_i64IceWorkerRegenTime >= 30000)
	{
		this->m_bSendIceWorkerDel = true;
		this->SendDoppelgangerTimerMsg(18);
	}

	if (i64CurTime - this->m_i64StateTime > 60000 * this->m_i64PlayTime)
	{
		this->SetDoppelgangerState(DOPPELGANGER_STATE_END);
	}
}

void CDoppelGanger::ProcDoppelgangerState_End(ULONGLONG i64CurTime)
{
	if (i64CurTime - this->m_i64StateTime > 10000 * this->m_i64EndTime && this->GetUserCount() > 0 && this->m_bSuccessResult == false)
	{
		for (int i = 0; i < MAX_DOPPELGANGER_USER_INFO; i++)
		{
			if (this->m_UserData[i].IsUser() == TRUE)
			{
				if (gObj[this->m_UserData[i].m_nIndex].Connected <= PLAYER_LOGGED)
				{
					this->DelDoppelgangerUser(this->m_UserData[i].m_nIndex);
				}

				else
				{
					gObjMoveGate(this->m_UserData[i].m_nIndex, 267);
				}
			}
		}
	}

	if (this->m_bSendDoppelGangerEnd == false && i64CurTime - this->m_i64StateTime > 30000 * this->m_i64EndTime)
	{
		this->m_bSendDoppelGangerEnd = true;
		this->SendDoppelgangerTimerMsg(19);
	}

	if (i64CurTime - this->m_i64StateTime > 60000 * this->m_i64EndTime)
	{
		this->SetDoppelgangerState(DOPPELGANGER_STATE_NONE);
	}

	int nPartyNumber = this->m_nPartyNumber;

	if (nPartyNumber > -1 && nPartyNumber < g_ConfigRead.server.GetObjectMaxUser())
	{
		if (gParty.IsParty(nPartyNumber) == TRUE && gParty.GetCount(nPartyNumber) <= 1)
		{
			for (int index = 0; index < MAX_USER_IN_PARTY; index++)
			{
				int aIndex = gParty.m_PartyS[nPartyNumber].Number[index];

				if (aIndex >= 0)
				{
					gParty.Delete(nPartyNumber, index);
					gObj[aIndex].PartyNumber = -1;
					gObj[aIndex].PartyTargetUser = -1;
					GSProtocol.GCPartyDelUserSend(aIndex);
				}
			}

			gParty.Destroy(nPartyNumber);
		}

		else
		{
			for (int index = 0; index < MAX_USER_IN_PARTY; index++)
			{
				int aIndex = gParty.m_PartyS[nPartyNumber].Number[index];

				if (aIndex >= 0 && gObj[aIndex].Connected < PLAYER_PLAYING)
				{
					gParty.Delete(nPartyNumber, index);
					gObj[aIndex].PartyNumber = -1;
					gObj[aIndex].PartyTargetUser = -1;
				}
			}
		}

		GSProtocol.CGPartyListAll(nPartyNumber);
		this->m_nPartyNumber = -1;
	}
}

BOOL CDoppelGanger::EnterDoppelgangerEvent(int aIndex, BYTE btItemPos)
{
	PMSG_DOPPELGANGER_ENTER_RESULT pResult;
	PHeadSubSetB((LPBYTE)&pResult, 0xBF, 0x0E, sizeof(pResult));

	pResult.btResult = 0;

	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Type != OBJ_USER || lpObj->Connected <= PLAYER_LOGGED)
	{
		return FALSE;
	}

	if (lpObj->m_IfState.use > 0 && lpObj->m_IfState.type != 12)
	{
		return FALSE;
	}

	if (lpObj->m_bPShopOpen == true)
		g_PersonalStore.CGPShopReqClose(aIndex);

	if (this->GetDoppelgangerState() || this->m_bDoppelGangerEnter == false)
	{
		pResult.btResult = 2;
		IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return FALSE;
	}

	BOOL PKFlag = FALSE;

	if (lpObj->PartyNumber < 0)
	{
		if (lpObj->m_PK_Level >= 4)
		{
			PKFlag = TRUE;
		}
	}

	else
	{
		if (gParty.GetPKPartyPenalty(lpObj->PartyNumber) >= 5)
		{
			PKFlag = TRUE;
		}
	}

	if (PKFlag == TRUE)
	{
		pResult.btResult = 3;
		IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return FALSE;
	}

	btItemPos += 12;

	if (lpObj->pInventory[btItemPos].IsItem() == TRUE)
	{
		if (lpObj->pInventory[btItemPos].m_Type != ITEMGET(14, 111) &&
			lpObj->pInventory[btItemPos].m_Type != ITEMGET(13, 125))
		{
			pResult.btResult = 1;
			IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
			return FALSE;
		}

		if (lpObj->pInventory[btItemPos].m_Type == ITEMGET(13, 125) &&
			lpObj->pInventory[btItemPos].m_Durability < 1.0)
		{
			pResult.btResult = 1;
			IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
			return FALSE;
		}

		if (this->AddDoppelgangerUser(aIndex) == FALSE)
		{
			pResult.btResult = 2;
			IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
			return FALSE;
		}

		if (lpObj->pInventory[btItemPos].m_Type == ITEMGET(14, 111))
		{
			gObjInventoryDeleteItem(aIndex, btItemPos);
			GSProtocol.GCInventoryItemDeleteSend(aIndex, btItemPos, 0);
		}

		else if (lpObj->pInventory[btItemPos].m_Type == ITEMGET(13, 125))
		{
			lpObj->pInventory[btItemPos].m_Durability -= 1.0f;

			if (lpObj->pInventory[btItemPos].m_Durability < 1.0)
			{
				gObjInventoryDeleteItem(aIndex, btItemPos);
				GSProtocol.GCInventoryItemDeleteSend(aIndex, btItemPos, 0);
			}

			else
			{
				GSProtocol.GCItemDurSend2(aIndex, btItemPos, lpObj->pInventory[btItemPos].m_Durability, 0);
			}
		}

		IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		gObjMoveGate(aIndex, this->m_nGateNumber);
		this->SendMapTileInfo(lpObj, 4);

		g_Log.Add("[Doppelganger] Entered Player [%s][%s]", lpObj->AccountID, lpObj->Name);

		return TRUE;
	}

	return FALSE;
}

BOOL CDoppelGanger::LeaveDoppelganger(int aIndex)
{
	this->DelDoppelgangerUser(aIndex);
	g_Log.Add("[Doppelganger] Leaved Player [%s][%s]", gObj[aIndex].AccountID, gObj[aIndex].Name);
	return TRUE;
}

BOOL CDoppelGanger::AddDoppelgangerUser(int aIndex)
{
	EnterCriticalSection(&this->m_critUserData);

	if (this->m_nCurUserCount > 0)
	{
		if (gObj[aIndex].PartyNumber == -1 || gObj[aIndex].PartyNumber != this->m_nPartyNumber)
		{
			LeaveCriticalSection(&this->m_critUserData);
			return FALSE;
		}

		bool bUserSet = false;

		for (int cnt = 0; cnt < MAX_DOPPELGANGER_USER_INFO; cnt++)
		{
			if (this->m_UserData[cnt].IsUser() == FALSE)
			{
				this->m_UserData[cnt].m_nIndex = aIndex;
				this->m_UserData[cnt].m_nLevel = gObj[aIndex].Level + gObj[aIndex].m_PlayerData->MasterLevel;
				bUserSet = true;
				break;
			}
		}

		if (bUserSet == false)
		{
			LeaveCriticalSection(&this->m_critUserData);
			return FALSE;
		}
	}

	else
	{
		int nRandValue = this->GetRandomValue(4);
		this->m_nMapNumber = MAP_INDEX_DOUBLE_GOER1 + nRandValue;
		this->m_nGateNumber = DoppelGanger_Gate_Snow + nRandValue;
		this->m_nPartyNumber = gObj[aIndex].PartyNumber;

		this->m_UserData[0].m_nIndex = aIndex;
		this->m_UserData[0].m_nLevel = gObj[aIndex].Level + gObj[aIndex].m_PlayerData->MasterLevel;

		this->m_i64UserEnterTime = GetTickCount64();

		if (this->m_nPartyNumber > 0)
		{
			PMSG_SET_DEVILSQUARE pMsg;
			PHeadSetB((LPBYTE)&pMsg, 0x92, sizeof(pMsg));

			pMsg.Type = 16;

			for (int Cnt = 0; Cnt < MAX_DOPPELGANGER_USER_INFO; Cnt++)
			{
				int nUserIndex = gParty.m_PartyS[this->m_nPartyNumber].Number[Cnt];

				if (nUserIndex > 0)
				{
					if (BC_MAP_RANGE(gObj[nUserIndex].MapNumber) == FALSE &&
						DS_MAP_RANGE(gObj[nUserIndex].MapNumber) == FALSE &&
						CC_MAP_RANGE(gObj[nUserIndex].MapNumber) == FALSE &&
						IT_MAP_RANGE(gObj[nUserIndex].MapNumber) == FALSE &&
						IMPERIAL_MAP_RANGE(gObj[nUserIndex].MapNumber) == FALSE)
					{
						IOCP.DataSend(nUserIndex, (LPBYTE)&pMsg, pMsg.h.size);
					}
				}
			}
		}
	}

	this->m_nCurUserCount++;
	this->CalUserLevel();
	LeaveCriticalSection(&this->m_critUserData);

	return TRUE;
}

void CDoppelGanger::DelDoppelgangerUser(int aIndex)
{
	EnterCriticalSection(&this->m_critUserData);

	bool bUserDelete = false;

	for (int cnt = 0; cnt < MAX_DOPPELGANGER_USER_INFO; cnt++)
	{
		if (this->m_UserData[cnt].IsUser() == TRUE && this->m_UserData[cnt].m_nIndex == aIndex)
		{
			this->m_UserData[cnt].m_nIndex = -1;
			this->m_UserData[cnt].m_nLevel = 0;
			bUserDelete = true;
			break;
		}
	}

	if (bUserDelete == false)
	{
		LeaveCriticalSection(&this->m_critUserData);
		return;
	}

	this->m_nCurUserCount--;

	if (this->m_nCurUserCount <= 0)
	{
		this->m_nCurUserCount = 0;
		this->m_i64UserEnterTime = 0;
	}

	this->CalUserLevel();
	LeaveCriticalSection(&this->m_critUserData);
}

void CDoppelGanger::CalUserLevel()
{
	this->m_nMinLevel = 0;
	this->m_nMaxLevel = 0;

	for (int cnt = 0; cnt < MAX_DOPPELGANGER_USER_INFO; cnt++)
	{
		if (this->m_UserData[cnt].IsUser() == TRUE)
		{
			if (this->m_nMaxLevel < this->m_UserData[cnt].m_nLevel)
			{
				this->m_nMaxLevel = this->m_UserData[cnt].m_nLevel;
			}

			if (this->m_nMinLevel)
			{
				if (this->m_nMinLevel > this->m_UserData[cnt].m_nLevel)
				{
					this->m_nMinLevel = this->m_UserData[cnt].m_nLevel;
				}
			}

			else
			{
				this->m_nMinLevel = this->m_UserData[cnt].m_nLevel;
			}
		}
	}
}

void CDoppelGanger::ClearUserData()
{
	EnterCriticalSection(&this->m_critUserData);

	for (int i = 0; i < MAX_DOPPELGANGER_USER_INFO; i++)
	{
		this->m_UserData[i].m_nIndex = -1;
		this->m_UserData[i].m_nLevel = 0;
	}

	this->m_nCurUserCount = 0;
	this->m_nPartyNumber = -1;
	this->m_i64UserEnterTime = 0;
	this->m_nMaxLevel = 0;
	this->m_nMinLevel = 0;

	LeaveCriticalSection(&this->m_critUserData);
}

int CDoppelGanger::GetUserMaxLevel()
{
	return this->m_nMaxLevel;
}

int CDoppelGanger::GetUserMinLevel()
{
	return this->m_nMinLevel;
}

int CDoppelGanger::GetUserAverageLevel()
{
	return this->m_nAverageLevel;
}

void CDoppelGanger::SetUserAverageLevel()
{
	int nLevelSum = 0;

	if (this->GetUserCount())
	{
		for (int i = 0; i < MAX_DOPPELGANGER_USER_INFO; i++)
		{
			if (this->m_UserData[i].IsUser() == TRUE)
			{
				nLevelSum += gObj[this->m_UserData[i].m_nIndex].Level + gObj[this->m_UserData[i].m_nIndex].m_PlayerData->MasterLevel;
			}
		}

		this->m_nAverageLevel = nLevelSum / this->GetUserCount();
	}
}

int CDoppelGanger::GetUserCount()
{
	return this->m_nCurUserCount;
}

int CDoppelGanger::GetStartUserCount()
{
	return this->m_nStartUserCount;
}

void CDoppelGanger::SendNoticeMessage(char* lpMsg)
{
	PMSG_NOTICE pNotice;
	TNotice::MakeNoticeMsg(&pNotice, 0, lpMsg);

	for (int Cnt = 0; Cnt < MAX_DOPPELGANGER_USER_INFO; Cnt++)
	{
		if (this->m_UserData[Cnt].IsUser() == TRUE)
		{
			if (gObj[this->m_UserData[Cnt].m_nIndex].Connected > PLAYER_LOGGED)
			{
				IOCP.DataSend(this->m_UserData[Cnt].m_nIndex, (LPBYTE)&pNotice, pNotice.h.size);
			}
		}
	}
}

void CDoppelGanger::SendDoppelgangerState(BYTE btState)
{
	PMSG_DOPPELGANGER_STATE pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xBF, 0x10, sizeof(pMsg));

	pMsg.btState = btState;

	for (int Cnt = 0; Cnt < MAX_DOPPELGANGER_USER_INFO; Cnt++)
	{
		if (this->m_UserData[Cnt].IsUser() == TRUE)
		{
			if (gObj[this->m_UserData[Cnt].m_nIndex].Connected > PLAYER_LOGGED)
			{
				IOCP.DataSend(this->m_UserData[Cnt].m_nIndex, (LPBYTE)&pMsg, pMsg.h.size);
			}
		}
	}
}

void CDoppelGanger::PlatformLugardAct(LPOBJ lpNpc, LPOBJ lpObj)
{
	if (lpObj->Type != OBJ_USER)
	{
		return;
	}

	if (lpObj->m_bPShopOpen == true)
	{
		g_PersonalStore.CGPShopReqClose(lpObj->m_Index);
	}

	if (lpObj->m_PK_Level >= 4)
	{
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,336), lpObj->m_Index, 1);
		return;
	}

	ULONGLONG i64EntryTime = 0;

	if (this->GetDoppelgangerState() == DOPPELGANGER_STATE_READY)
	{
		i64EntryTime = this->m_i64EndTime + this->m_i64ReadyTime - (GetTickCount64() - this->m_i64StateTime) / 1000 / 60 + this->m_i64PlayTime + 1;
	}

	else if (this->GetDoppelgangerState() == DOPPELGANGER_STATE_PLAY)
	{
		i64EntryTime = this->m_i64PlayTime - (GetTickCount64() - this->m_i64StateTime) / 1000 / 60 + this->m_i64EndTime + 1;
	}

	else if (this->GetDoppelgangerState() == DOPPELGANGER_STATE_END)
	{
		i64EntryTime = this->m_i64EndTime - (GetTickCount64() - this->m_i64StateTime) / 1000 / 60 + 1;
	}

	else
	{
		if (this->m_bDoppelGangerEnter == true)
			this->GetUserCount();
	}

	PMSG_TALKRESULT pMsg;
	PHeadSetBE((LPBYTE)&pMsg, 0x30, sizeof(pMsg));

	pMsg.result = 0x23;
	pMsg.level1 = 0;

	IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
}

void CDoppelGanger::MiddleTreasureAct(LPOBJ lpNpc, LPOBJ lpObj)
{
	if (lpNpc->m_State == 0)
	{
		return;
	}

	if (abs(lpObj->X - lpNpc->X) > 3 || abs(lpObj->Y - lpNpc->Y) > 3)
	{
		return;
	}

	if (this->OpenTreasureBox() == FALSE)
	{
		return;
	}

	lpNpc->Life = 0.0;
	gObjLifeCheck(lpNpc, lpObj, 0, 1, 0, 0, 0, 0, 0);

	for (int Cnt = 0; Cnt < 3; Cnt++)
	{
		if (this->m_nTeasureBoxIndex[Cnt] != -1)
		{
			if (this->m_nTeasureBoxIndex[Cnt] != lpNpc->m_Index)
			{
				gObjDel(this->m_nTeasureBoxIndex[Cnt]);
				this->m_nTeasureBoxIndex[Cnt] = -1;
			}
		}
	}

	g_Log.Add("[Doppelganger] MiddleTreasureBox Open [%d][%d, %d][%s][%s]",
		this->m_nMapNumber, lpNpc->X, lpNpc->Y, lpObj->AccountID, lpObj->Name);

	int nItemDrop = this->GetRandomValue(1000);

	if (nItemDrop <= 100 || nItemDrop >= 500)
	{
		int nUserCnt = this->GetStartUserCount();
		int nMonsterLevel = this->GetMonsterLevelWeight() * this->GetDefaultMonsterLevel(532);
		int nMonsterHp = this->GetMonsterHpWeight() * this->GetDefaultMonsterHp(532);
		int nMonsterAttMin = this->GetMonsterAttackWeight() * this->GetDefaultMonsterAttackMin(532);
		int nMonsterAttMax = this->GetMonsterAttackWeight() * this->GetDefaultMonsterAttackMax(532);
		int nMonsterDef = this->GetMonsterDefenseWeight() * this->GetDefaultMonsterDefense(532);

		for (int i = 0; i < nUserCnt; i++)
		{
			BYTE btX = lpNpc->X;
			BYTE btY = lpNpc->Y;

			BOOL bResult = this->GetRandomLocation(&btX, &btY);

			if (bResult == FALSE)
			{
				btX = lpNpc->X;
				btY = lpNpc->Y;
			}

			int nRetIndex = this->AddMonsterLarva(this->m_nMapNumber, btX, btY, nMonsterLevel, nMonsterHp, nMonsterAttMin, nMonsterAttMax, nMonsterDef);

			if (nRetIndex != -1)
				this->AddLarvaIndex(nRetIndex);

			g_Log.Add("[Doppelganger] Create Larva (%d)[%d][%d, %d]", nRetIndex, this->m_nMapNumber, btX, btY);
		}
	}

	else
	{
		g_Log.Add("[Doppelganger] MiddleTreasureBox ItemDrop [%d][%d, %d]", this->m_nMapNumber, lpNpc->X, lpNpc->Y);

		int nLootCnt = this->m_DPEventInfo.GetMiddleItemDropCnt();

		for (int nCnt = 0; nCnt < nLootCnt; nCnt++)
		{
			BYTE btX = lpNpc->X;
			BYTE btY = lpNpc->Y;

			BOOL bResult = this->GetRandomLocation(&btX, &btY);

			if (bResult == FALSE)
			{
				btX = lpNpc->X;
				btY = lpNpc->Y;
			}

			int nRewardValue = this->GetRandomValue(3);
			int nItemBagIndex = this->m_DPEventInfo.GetMiddleItemBagIndex(nRewardValue);
			int nItemBagDropRate = this->m_DPEventInfo.GetMiddleItemDropRate(nRewardValue);
			this->m_DPDropItemInfo.DoppelgangerItemDrop(lpObj, nItemBagIndex, nItemBagDropRate, 0);
		}
	}
}

void CDoppelGanger::LastTreasureAct(LPOBJ lpNpc, LPOBJ lpObj)
{
	if (lpNpc->m_State == 0)
	{
		return;
	}

	if (abs(lpObj->X - lpNpc->X) > 3 || abs(lpObj->Y - lpNpc->Y) > 3)
	{
		return;
	}

	if (this->OpenLastTreasureBox() == FALSE)
	{
		return;
	}

	lpNpc->Life = 0.0;
	gObjLifeCheck(lpNpc, lpObj, 0, 1, 0, 0, 0, 0, 0);

	g_Log.Add("[Doppelganger] LastTreasureBox Open [%d][%d, %d][%s][%s]",
		this->m_nMapNumber, lpNpc->X, lpNpc->Y, lpObj->AccountID, lpObj->Name);

	int nLootCnt = this->m_DPEventInfo.GetLastItemDropCnt();

	for (int nCnt = 0; nCnt < nLootCnt; nCnt++)
	{
		BYTE btX = lpNpc->X;
		BYTE btY = lpNpc->Y;

		BOOL bResult = this->GetRandomLocation(&btX, &btY);

		if (bResult == FALSE)
		{
			btX = lpNpc->X;
			btY = lpNpc->Y;
		}

		int nRewardValue = this->GetRandomValue(6);
		int nItemBagIndex = this->m_DPEventInfo.GetLastItemBagIndex(nRewardValue);
		int nItemBagDropRate = this->m_DPEventInfo.GetLastItemDropRate(nRewardValue);
		this->m_DPDropItemInfo.DoppelgangerItemDrop(lpObj, nItemBagIndex, nItemBagDropRate, 1);
	}
}

BOOL CDoppelGanger::OpenTreasureBox()
{
	EnterCriticalSection(&this->m_critTreasureBox);

	if (this->m_bOpenTreasureBox == TRUE)
	{
		LeaveCriticalSection(&this->m_critTreasureBox);
		return FALSE;
	}

	else
	{
		this->m_bOpenTreasureBox = TRUE;
		LeaveCriticalSection(&this->m_critTreasureBox);
		return TRUE;
	}

	return FALSE;
}

BOOL CDoppelGanger::OpenLastTreasureBox()
{
	EnterCriticalSection(&this->m_critTreasureBox);

	if (this->m_bOpenLastTreasureBox == TRUE)
	{
		LeaveCriticalSection(&this->m_critTreasureBox);
		return FALSE;
	}

	else
	{
		this->m_bOpenLastTreasureBox = TRUE;
		LeaveCriticalSection(&this->m_critTreasureBox);
		return TRUE;
	}

	return FALSE;
}

int CDoppelGanger::GetRandomValue(int nRange)
{
	int nRand = rand() % 10000;
	return nRand % nRange;
}

int CDoppelGanger::GetMonsterHerdIndex()
{
	for (int Cnt = 0; Cnt < MAX_DOPPELGANGER_MONSTER_INFO; Cnt++)
	{
		if (this->m_MonsterHerd[Cnt].GetMonsterSize() == 0)
		{
			return Cnt;
		}
	}

	g_Log.Add("[Doppelganger] Error Not Enought HerdIndex");
	return -1;
}

void CDoppelGanger::IncMonsterHerdIndex() // empty
{

}

void CDoppelGanger::SetHerdStartPosInfo(int nHerdIndex, int nPosInfo, BOOL bMove)
{
	if (nHerdIndex <= MAX_DOPPELGANGER_MONSTER_INFO - 1)
	{
		this->m_MonsterHerd[nHerdIndex].SetTotalInfo(this->m_nMapNumber, 4, nPosInfo, bMove);
	}
}

void CDoppelGanger::SetHerdEndPosInfo(int nHerdIndex, int nPosInfo)
{
	if (nHerdIndex <= MAX_DOPPELGANGER_MONSTER_INFO - 1)
	{
		this->m_MonsterHerd[nHerdIndex].SetEndPosition(nPosInfo);
	}
}

void CDoppelGanger::AddMonsterHerd(int nHerdIndex, int nMonsterClass, BOOL bAttackFirst)
{
	if (nHerdIndex <= MAX_DOPPELGANGER_MONSTER_INFO - 1 && (nMonsterClass >= 529 && nMonsterClass <= 539))
	{
		float fWeight = 1.0;

		if (this->GetMissionResult() == FALSE)
		{
			fWeight = 2.0;
		}

		int nMonsterLevel = this->GetDefaultMonsterLevel(nMonsterClass) * this->GetMonsterLevelWeight();
		int nMonsterHp = this->GetDefaultMonsterHp(nMonsterClass) * this->GetMonsterHpWeight() * fWeight;
		int nMonsterAttMin = this->GetDefaultMonsterAttackMin(nMonsterClass) * this->GetMonsterAttackWeight() * fWeight;
		int nMonsterAttMax = this->GetDefaultMonsterAttackMax(nMonsterClass) * this->GetMonsterAttackWeight() * fWeight;
		int nMonsterDef = this->GetDefaultMonsterDefense(nMonsterClass) * this->GetMonsterDefenseWeight() * fWeight;

		if (nMonsterClass == 530 && this->GetKillerState() == 3)
			nMonsterHp = this->GetKillerHp();

		if (nMonsterClass == 529 && this->GetAngerKillerState() == 3)
			nMonsterHp = this->GetAngerKillerHp();

		int nIndex = this->m_MonsterHerd[nHerdIndex].AddMonsterEX(nMonsterClass, bAttackFirst, nMonsterLevel, nMonsterHp, nMonsterAttMin, nMonsterAttMax, nMonsterDef);

		if (nIndex < 0)
			g_Log.Add("Error AddMonsterEx Index is %d", nIndex);

		if (nIndex != -1 && nMonsterClass == 530)
		{
			this->SetMiddleBossIndex(530, nIndex);
			this->SetKillerHp(nMonsterHp);
			this->SetKillerState(2);
			g_Log.Add("[Doppelganger] Killer monster Regen [%d][%d, %d] (%d)", gObj[nIndex].MapNumber, gObj[nIndex].X, gObj[nIndex].Y, nIndex);
		}

		else if (nIndex != -1 && nMonsterClass == 529)
		{
			this->SetAngerKillerHp(nMonsterHp);
			this->SetAngerKillerState(2);
			g_Log.Add("[Doppelganger] AngerKiller monster Regen [%d][%d, %d] (%d)", gObj[nIndex].MapNumber, gObj[nIndex].X, gObj[nIndex].Y, nIndex);
		}

		else if (nIndex != -1 && nMonsterClass == 531)
		{
			this->AddIceWorkerIndex(nIndex);
			g_Log.Add("[Doppelganger] Iceworker monster Regen [%d][%d, %d] (%d)", gObj[nIndex].MapNumber, gObj[nIndex].X, gObj[nIndex].Y, nIndex);
		}
	}
}

void CDoppelGanger::AddMonsterBoss(int nHerdIndex, int nMonsterClass, BOOL bAttackFirst) // not used
{

}

void CDoppelGanger::AddMonsterNormal(int nMonsterClass, BYTE posX, BYTE posY)
{
	// empty
}

void CDoppelGanger::SetIceWorkerRegen(int nPosInfo)
{
	this->SendNoticeMessage(Lang.GetText(0,337));

	PMSG_DOPPELGANGER_ICEWORKER_INFO pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xBF, 0x11, sizeof(pMsg));

	this->m_i64IceWorkerRegenTime = GetTickCount64();
	this->m_btIceWorkState = 0;
	pMsg.btIceWorkerState = 0;
	pMsg.btIceWorkerPos = nPosInfo;

	for (int Cnt = 0; Cnt<MAX_DOPPELGANGER_USER_INFO; Cnt++)
	{
		if (this->m_UserData[Cnt].IsUser() == TRUE &&
			gObj[this->m_UserData[Cnt].m_nIndex].Connected > PLAYER_LOGGED)
		{
			IOCP.DataSend(this->m_UserData[Cnt].m_nIndex, (LPBYTE)&pMsg, pMsg.h.size);
		}
	}
}

void CDoppelGanger::AddIceWorkerIndex(int nIndex)
{
	for (int Cnt = 0; Cnt < MAX_DOPPELGANGER_ICEWORKER_INFO; Cnt++)
	{
		if (this->m_nIceWorkerIndex[Cnt] == -1)
		{
			this->m_nIceWorkerIndex[Cnt] = nIndex;
			return;
		}
	}
}

void CDoppelGanger::DelIceWorkerIndex(int nIndex)
{
	for (int Cnt = 0; Cnt < MAX_DOPPELGANGER_ICEWORKER_INFO; Cnt++)
	{
		if (this->m_nIceWorkerIndex[Cnt] == nIndex)
		{
			this->m_nIceWorkerIndex[Cnt] = -1;
			break;
		}
	}

	this->CheckIceWorker();
}

BOOL CDoppelGanger::CheckIceWorker()
{
	for (int i = 0; i < MAX_DOPPELGANGER_ICEWORKER_INFO; i++)
	{
		if (this->m_nIceWorkerIndex[i] != -1)
		{
			return FALSE;
		}
	}

	this->m_btIceWorkState = 1;
	this->m_bMissionResult = true;

	this->SendNoticeMessage(Lang.GetText(0,338));
	PMSG_DOPPELGANGER_ICEWORKER_INFO pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xBF, 0x11, sizeof(pMsg));

	pMsg.btIceWorkerState = 1;
	pMsg.btIceWorkerPos = 0;

	for (int Cnt = 0; Cnt < MAX_DOPPELGANGER_USER_INFO; Cnt++)
	{
		if (this->m_UserData[Cnt].IsUser() == TRUE && gObj[this->m_UserData[Cnt].m_nIndex].Connected > PLAYER_LOGGED)
		{
			IOCP.DataSend(this->m_UserData[Cnt].m_nIndex, (LPBYTE)&pMsg, pMsg.h.size);
		}
	}

	this->SendDoppelgangerTimerMsg(0xFF);
	g_Log.Add("[Doppelganger] Iceworker Die - Mission Clear");

	return TRUE;
}

void CDoppelGanger::AddLarvaIndex(int nIndex)
{
	for (int Cnt = 0; Cnt < MAX_TOTAL_GOLDEN_LARVA; Cnt++)
	{
		if (this->m_nLarvaIndex[Cnt] == -1)
		{
			this->m_nLarvaIndex[Cnt] = nIndex;
			return;
		}
	}
}

void CDoppelGanger::DelLarvaAll()
{
	for (int Cnt = 0; Cnt < MAX_TOTAL_GOLDEN_LARVA; Cnt++)
	{
		if (this->m_nLarvaIndex[Cnt] != -1)
		{
			gObjDel(this->m_nLarvaIndex[Cnt]);
			this->m_nLarvaIndex[Cnt] = -1;
		}
	}
}

void CDoppelGanger::MonsterHerdStart(int nHerdIndex)
{
	if (nHerdIndex <= MAX_DOPPELGANGER_MONSTER_INFO - 1)
	{
		this->m_MonsterHerd[nHerdIndex].Start();
	}
}

void CDoppelGanger::CheckDoppelgangerMonsterPos(LPOBJ lpObj)
{
	int nPosNum = this->GetDoppelgangerPosIndex(lpObj->X, lpObj->Y);
	int nLastPos = MAX_DOPPELGANGER_POS_INFO - 1;

	if (!lpObj->m_bIsInMonsterHerd)
	{
		return;
	}

	if (lpObj->Class == 531)
	{
		nLastPos = -1;
	}

	if (nPosNum == nLastPos)
	{
		lpObj->Live = FALSE;
		this->m_nGoalMonsterCnt++;

		g_Log.Add("[Doppelganger] Monster MagicCircle in: %d, MonsterType(%d)", this->m_nGoalMonsterCnt, lpObj->Class);
		gObjAddBuffEffect(lpObj, BUFFTYPE_DOPPELGANGER_PORTAL, 0, 0, 0, 0, 10);
		this->SendMonsterGoalCount();

		if (lpObj->Class == 530)
		{
			this->SetKillerHp(lpObj->Life);
			this->SetKillerState(3);
		}

		if (lpObj->Class == 529)
		{
			this->SetAngerKillerHp(lpObj->Life);
			this->SetAngerKillerState(3);
		}

		if (this->m_nGoalMonsterCnt >= MAX_DOPPELGANGER_MONSTER_IN_PORTAL)
		{
			this->m_bSuccessResult = false;
			this->SetDoppelgangerState(DOPPELGANGER_STATE_END);
		}
	}

	else
	{
		MonsterHerd * lpMH = lpObj->m_lpMonsterHerd;

		if (!lpMH)
		{
			return;
		}

		if (nPosNum > lpMH->GetPosIndex())
		{
			if (lpMH->IsMove() == TRUE)
			{
				int nTmpPosNum = nPosNum + 1;

				if (nTmpPosNum > nLastPos)
					nTmpPosNum = nPosNum;

				lpMH->SetPosIndex(nTmpPosNum);
			}

			else
			{
				int nTmpPosNum = lpMH->GetEndPosIndex();
				lpMH->SetPosIndex(nTmpPosNum);
			}
		}

		if (lpObj->Class != 531)
		{
			if (this->m_nFirstMonsterIndex == -1)
			{
				this->m_nFirstMonsterIndex = lpObj->m_Index;
				this->m_btFirstIndex = nPosNum;
			}

			else if (gObj[this->m_nFirstMonsterIndex].Live == TRUE)
			{
				if (nPosNum > this->m_btFirstIndex)
				{
					this->m_nFirstMonsterIndex = lpObj->m_Index;
					this->m_btFirstIndex = nPosNum;
				}
			}

			else
			{
				this->m_nFirstMonsterIndex = lpObj->m_Index;
				this->m_btFirstIndex = nPosNum;
			}
		}
	}
}

int CDoppelGanger::GetDoppelgangerPosIndex(BYTE btX, BYTE btY)
{
	return this->m_PosInfo.GetPosIndex(this->m_nMapNumber, btX, btY);
}

void CDoppelGanger::SendMonsterGoalCount()
{
	PMSG_DOPPELGANGER_GOAL_COUNT pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xBF, 0x14, sizeof(pMsg));

	pMsg.btMaxGoalCount = MAX_DOPPELGANGER_MONSTER_IN_PORTAL;
	pMsg.btCurGoalCount = this->m_nGoalMonsterCnt;

	for (int Cnt = 0; Cnt < MAX_DOPPELGANGER_USER_INFO; Cnt++)
	{
		if (this->m_UserData[Cnt].IsUser() == TRUE && gObj[this->m_UserData[Cnt].m_nIndex].Connected > PLAYER_LOGGED)
		{
			IOCP.DataSend(this->m_UserData[Cnt].m_nIndex, (LPBYTE)&pMsg, pMsg.h.size);
		}
	}
}

void CDoppelGanger::SendDoppelgangerMonsterPos()
{
	PMSG_DOPPELGANGER_MONSTER_POS pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xBF, 0x0F, sizeof(pMsg));

	pMsg.btMonsterPos = this->m_btFirstIndex;

	for (int Cnt = 0; Cnt<MAX_DOPPELGANGER_USER_INFO; Cnt++)
	{
		if (this->m_UserData[Cnt].IsUser() == TRUE && gObj[this->m_UserData[Cnt].m_nIndex].Connected > PLAYER_LOGGED)
		{
			IOCP.DataSend(this->m_UserData[Cnt].m_nIndex, (LPBYTE)&pMsg, pMsg.h.size);
		}
	}
}

void CDoppelGanger::SendDoppelgangerUserPos()
{
	BYTE SendByte[256];

	PMSG_DOPPELGANGER_USER_POS * lpMsg = (PMSG_DOPPELGANGER_USER_POS *)SendByte;

	lpMsg->wLeftTime = 60 * this->m_i64PlayTime - (GetTickCount64() - this->m_i64StateTime) / 1000;
	lpMsg->wCount = 0;

	for (int i = 0; i < MAX_DOPPELGANGER_USER_INFO; i++)
	{
		if (this->m_UserData[i].IsUser() == TRUE && gObj[this->m_UserData[i].m_nIndex].Connected > PLAYER_LOGGED)
		{
			DOPPELGANGER_USER_POS * lpMsgBody = (DOPPELGANGER_USER_POS *)(SendByte + sizeof(PMSG_DOPPELGANGER_USER_POS) + (sizeof(DOPPELGANGER_USER_POS) * lpMsg->wCount));

			lpMsgBody->wIndex = this->m_UserData[i].m_nIndex;
			lpMsgBody->btMapNumber = this->m_nMapNumber;
			lpMsgBody->btPosIndex = this->GetDoppelgangerPosIndex(gObj[this->m_UserData[i].m_nIndex].X, gObj[this->m_UserData[i].m_nIndex].Y);

			lpMsg->wCount++;
		}
	}

	int PacketSize = sizeof(PMSG_DOPPELGANGER_USER_POS) + (sizeof(DOPPELGANGER_USER_POS) * lpMsg->wCount);

	PHeadSubSetB((LPBYTE)lpMsg, 0xBF, 0x12, PacketSize);

	for (int Cnt = 0; Cnt < MAX_DOPPELGANGER_USER_INFO; Cnt++)
	{
		if (this->m_UserData[Cnt].IsUser() == TRUE && gObj[this->m_UserData[Cnt].m_nIndex].Connected > PLAYER_LOGGED)
		{
			IOCP.DataSend(this->m_UserData[Cnt].m_nIndex, SendByte, PacketSize);
		}
	}
}

void CDoppelGanger::SelfExplosion(int aIndex, CMagicInf * lpMagic, int aTargetIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->X < 0 || lpObj->X > 255)
	{
		return;
	}

	if (lpObj->Y < 0 || lpObj->Y > 255)
	{
		return;
	}

	int iMIN_X = lpObj->X - 2;
	int iMAX_X = lpObj->X + 2;
	int iMIN_Y = lpObj->Y - 2;
	int iMAX_Y = lpObj->Y + 2;

	if (iMIN_X < 0)
		iMIN_X = 0;

	if (iMIN_Y < 0)
		iMIN_Y = 0;

	if (iMIN_X > 255)
		iMIN_X = 255;

	if (iMIN_Y > 255)
		iMIN_Y = 255;

	GSProtocol.GCMagicAttackNumberSend(lpObj, lpMagic->m_Skill, aTargetIndex, TRUE);

	for (int Cnt = 0; Cnt < MAX_DOPPELGANGER_USER_INFO; Cnt++)
	{
		if (this->m_UserData[Cnt].IsUser() == TRUE && gObj[this->m_UserData[Cnt].m_nIndex].Connected > PLAYER_LOGGED)
		{
			if (gObj[this->m_UserData[Cnt].m_nIndex].X >= iMIN_X && gObj[this->m_UserData[Cnt].m_nIndex].X <= iMAX_X &&
				gObj[this->m_UserData[Cnt].m_nIndex].Y >= iMIN_Y && gObj[this->m_UserData[Cnt].m_nIndex].Y <= iMAX_Y)
			{
				gObjAddAttackProcMsgSendDelay(lpObj, 50, this->m_UserData[Cnt].m_nIndex, 500, lpMagic->m_Skill, 0);
				return;
			}
		}
	}
}

void CDoppelGanger::AngerKillerAttack(LPOBJ lpObj)
{
	if (lpObj->X < 0 || lpObj->X > 255)
	{
		return;
	}

	if (lpObj->Y < 0 || lpObj->Y > 255)
	{
		return;
	}

	int iMIN_X = lpObj->X - 2;
	int iMAX_X = lpObj->X + 2;
	int iMIN_Y = lpObj->Y - 2;
	int iMAX_Y = lpObj->Y + 2;

	if (iMIN_X < 0)
		iMIN_X = 0;

	if (iMIN_Y < 0)
		iMIN_Y = 0;

	if (iMIN_X > 255)
		iMIN_X = 255;

	if (iMIN_Y > 255)
		iMIN_Y = 255;

	for (int Cnt = 0; Cnt < MAX_DOPPELGANGER_USER_INFO; Cnt++)
	{
		if (this->m_UserData[Cnt].IsUser() == TRUE && gObj[this->m_UserData[Cnt].m_nIndex].Connected > PLAYER_LOGGED)
		{
			if (gObj[this->m_UserData[Cnt].m_nIndex].X >= iMIN_X && gObj[this->m_UserData[Cnt].m_nIndex].X <= iMAX_X &&
				gObj[this->m_UserData[Cnt].m_nIndex].Y >= iMIN_Y && gObj[this->m_UserData[Cnt].m_nIndex].Y <= iMAX_Y)
			{
				gObjAttack(lpObj, &gObj[this->m_UserData[Cnt].m_nIndex], 0, 0, 0, 0, 0, 0, 0);
				gObjBackSpring(&gObj[this->m_UserData[Cnt].m_nIndex], lpObj);
			}
		}
	}
}

void CDoppelGanger::SendDoppelgangerResult(LPOBJ lpObj, BYTE btResult)
{
	PMSG_DOPPELGANGER_MISSION_RESULT pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xBF, 0x13, sizeof(pMsg));

	pMsg.btMissionResult = btResult;
	pMsg.dwRewardExp = 0;

	IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
	g_Log.Add("[Doppelganger] Event fail (%d) [%s][%s]", btResult, lpObj->AccountID, lpObj->Name);
}

void CDoppelGanger::SendDoppelgangerResultAll()
{
	PMSG_DOPPELGANGER_MISSION_RESULT pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xBF, 0x13, sizeof(pMsg));

	if (this->m_nGoalMonsterCnt < MAX_DOPPELGANGER_MONSTER_IN_PORTAL)
	{
		pMsg.btMissionResult = 0;
	}

	else
	{
		pMsg.btMissionResult = 2;
	}

	pMsg.dwRewardExp = 0;

	for (int Cnt = 0; Cnt < MAX_DOPPELGANGER_USER_INFO; Cnt++)
	{
		if (this->m_UserData[Cnt].IsUser() == TRUE && gObj[this->m_UserData[Cnt].m_nIndex].Connected == PLAYER_PLAYING)
		{
			IOCP.DataSend(this->m_UserData[Cnt].m_nIndex, (LPBYTE)&pMsg, pMsg.h.size);
			g_Log.Add("[Doppelganger] Event Result (%d) [%s][%s]", pMsg.btMissionResult, gObj[this->m_UserData[Cnt].m_nIndex].AccountID, gObj[this->m_UserData[Cnt].m_nIndex].Name);
		}
	}

	g_Log.Add("[Doppelganger] Event Complete");
}

void CDoppelGanger::ArrangeMonsterHerd()
{
	if (!this->m_btIceWorkState && GetTickCount64() - this->m_i64IceWorkerRegenTime > 60000)
	{
		this->m_bMissionResult = false;
		this->SendNoticeMessage(Lang.GetText(0,339));

		for (int i = 0; i < MAX_DOPPELGANGER_ICEWORKER_INFO; i++)
		{
			if (this->m_nIceWorkerIndex[i] != -1)
			{
				gObj[this->m_nIceWorkerIndex[i]].Live = FALSE;
				gObjDel(this->m_nIceWorkerIndex[i]);
				this->m_nIceWorkerIndex[i] = -1;
			}
		}

		this->m_btIceWorkState = 1;

		PMSG_DOPPELGANGER_ICEWORKER_INFO pMsg;
		PHeadSubSetB((LPBYTE)&pMsg, 0xBF, 0x11, sizeof(pMsg));

		pMsg.btIceWorkerState = 1;
		pMsg.btIceWorkerPos = 0;

		for (int cnt = 0; cnt < MAX_DOPPELGANGER_USER_INFO; cnt++)
		{
			if (this->m_UserData[cnt].IsUser() == TRUE &&
				gObj[this->m_UserData[cnt].m_nIndex].Connected > PLAYER_LOGGED)
			{
				IOCP.DataSend(this->m_UserData[cnt].m_nIndex, (LPBYTE)&pMsg, pMsg.h.size);
			}
		}

		g_Log.Add("[Doppelganger] Iceworker Disappear Mission Fail");
	}

	for (int i = 0; i < MAX_DOPPELGANGER_MONSTER_INFO; i++)
	{
		if (this->m_MonsterHerd[i].IsActive() == FALSE)
		{
			this->m_MonsterHerd[i].Stop();
			this->m_MonsterHerd[i].SetPosIndex(0);
		}

		this->m_MonsterHerd[i].ArrangeMonsterHerd();
	}
}

void CDoppelGanger::MoveDoppelgangerMonsterProc()
{
	int DelayTime;
	int MoveTime;

	for (int i = 0; i < g_ConfigRead.server.GetObjectMaxMonster(); i++)
	{
		LPOBJ lpObj = &gObj[i];

		if (lpObj->Connected == PLAYER_PLAYING && lpObj->m_State == 2 && lpObj->Type == OBJ_MONSTER)
		{
			if (DG_MAP_RANGE(lpObj->MapNumber) == TRUE)
			{
				if (lpObj->PathCount != 0)
				{
					if (!gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_STONE) && !gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_STUN) &&
						!gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_FREEZE_2) && !gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_EARTH_BINDS) &&
						!gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_SLEEP))
					{
						if (lpObj->DelayLevel != 0)
						{
							DelayTime = 300;
						}

						else
						{
							DelayTime = 0;
						}

						if (lpObj->Type == OBJ_MONSTER && lpObj->m_bIsInMonsterHerd == true)
						{
							lpObj->m_MoveSpeed = 250;
						}

						if (lpObj->PathDir[lpObj->PathCur] % 2 == 0)
						{
							MoveTime = (lpObj->m_MoveSpeed + DelayTime)*(double)1.3;
						}

						else
						{
							MoveTime = lpObj->m_MoveSpeed + DelayTime;
						}

						if ((GetTickCount() - lpObj->PathTime) > MoveTime)
						{
							DWORD dwTmp = GetTickCount();

							if (lpObj->PathCur == 1)
							{
								lpObj->PathTime = dwTmp;
							}

							if (lpObj->PathCur < 14)
							{
								int nextX = lpObj->PathX[lpObj->PathCur];
								int nextY = lpObj->PathY[lpObj->PathCur];
								BYTE mapnumber = lpObj->MapNumber;

								if (MAX_MAP_RANGE(mapnumber) == FALSE)
								{
									continue;
								}

								BYTE attr = MapC[mapnumber].GetAttr(nextX, nextY);

								if (lpObj->Type == OBJ_USER && ((attr & 4) == 4 || (attr & 8) == 8))
								{
									g_Log.Add("[ CHECK POSITION ] MoveMosterProc [%s][%s] Map[%d]-(%d,%d) User(%d,%d) Can not Move Position Attr[%d]",
										lpObj->AccountID, lpObj->Name, lpObj->MapNumber, nextX, nextY, lpObj->X, lpObj->Y, attr);

									for (int n = 0; n < 15; n++)
									{
										lpObj->PathX[n] = 0;
										lpObj->PathY[n] = 0;
										lpObj->PathOri[n] = 0;
									}

									lpObj->PathCount = 0;
									lpObj->PathCur = 0;
									lpObj->PathTime = GetTickCount();

									if (lpObj->Type == OBJ_MONSTER || lpObj->Type == OBJ_NPC)
									{
										lpObj->PathStartEnd = 0;
									}

									if (lpObj->Type == OBJ_USER)
									{
										gObjSetPosition(lpObj->m_Index, lpObj->X, lpObj->Y);
									}
								}

								else
								{
									lpObj->X = lpObj->PathX[lpObj->PathCur];
									lpObj->Y = lpObj->PathY[lpObj->PathCur];
									lpObj->Dir = lpObj->PathDir[lpObj->PathCur];

									lpObj->PathTime = GetTickCount();
									lpObj->PathCur++;

									if (lpObj->PathCur >= lpObj->PathCount)
									{
										lpObj->PathCur = 0;
										lpObj->PathCount = 0;

										if (lpObj->Type == OBJ_MONSTER || lpObj->Type == OBJ_NPC)
										{
											lpObj->PathStartEnd = 0;
										}
									}
								}
							}
						}
					}
				}

				CreateFrustrum(lpObj->X, lpObj->Y, i);
			}
		}
	}
}

BOOL CDoppelGanger::GetRandomLocation(BYTE* cX, BYTE* cY)
{
	if (MAX_MAP_RANGE(this->m_nMapNumber) == FALSE)
	{
		return FALSE;
	}

	int iCount = 50;

	while (iCount-- != 0)
	{
		int iRandX = rand() % 3;

		*cX += (rand() % 2 == 0 ? -1 : 1) * (rand() % 3);
		*cY += (rand() % 2 == 0 ? -1 : 1) * (rand() % 3);

		BYTE attr = MapC[this->m_nMapNumber].GetAttr(*cX, *cY);

		if (attr == 0 || attr == 2)
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CDoppelGanger::GetRandomLocation(BYTE* cX, BYTE* cY, int nSeed)
{
	if (nSeed >= 0 && nSeed <= 7)
	{
		int nSeedX[8] = { -2, 0, 2, 2, 2, 0, -2, -2 };
		int nSeedY[8] = { -2, -2, -2, 0, 2, 2, 2, 0 };

		*cX += nSeedX[nSeed];
		*cY += nSeedY[nSeed];

		BYTE attr = MapC[this->m_nMapNumber].GetAttr(*cX, *cY);

		if (attr == 0 || attr == 2)
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CDoppelGanger::AddMiddleTreasureBoxAll(BYTE btX, BYTE btY)
{
	EnterCriticalSection(&this->m_critTreasureBox);
	this->m_bOpenTreasureBox = true;
	LeaveCriticalSection(&this->m_critTreasureBox);

	int nSeed = 0;

	for (int Cnt = 0; Cnt < MAX_DOPPELGANGER_TREASURE_BOX; Cnt++)
	{
		BYTE cX = btX;
		BYTE cY = btY;

		if (Cnt == 0)
		{
			cX = btX;
			cY = btY;
		}

		else
		{
			BOOL bRet = 0;

			for (int i = nSeed; i < 8; i++)
			{
				bRet = this->GetRandomLocation(&cX, &cY, i);

				if (bRet == TRUE)
				{
					nSeed++;
					break;
				}
			}

			if (bRet == FALSE)
			{
				g_Log.Add("MiddleTreasureBox RandLocation Error [%d, %d]", cX, cY);
				cX = btX;
				cY = btY;
			}
		}

		if (this->m_nTeasureBoxIndex[Cnt] != -1)
		{
			gObjDel(this->m_nTeasureBoxIndex[Cnt]);
			this->m_nTeasureBoxIndex[Cnt] = -1;
		}

		this->m_nTeasureBoxIndex[Cnt] = this->AddMiddleTreasureBox(this->m_nMapNumber, cX, cY);
	}

	this->m_i64TeasureBoxTime = GetTickCount64();
	this->m_bTreasureBoxRegen = true;

	EnterCriticalSection(&this->m_critTreasureBox);
	this->m_bOpenTreasureBox = false;
	LeaveCriticalSection(&this->m_critTreasureBox);

	return TRUE;
}

int CDoppelGanger::AddMiddleTreasureBox(int nMapNumber, BYTE cX, BYTE cY)
{
	int iIndex = gObjAddMonster(nMapNumber);

	if (iIndex < 0)
	{
		g_Log.Add("[Doppelganger] Create MiddleTreasureBox Fail [%d][%d, %d] (%d)", this->m_nMapNumber, cX, cY, iIndex);
		return iIndex;
	}

	gObj[iIndex].m_PosNum = -1;
	gObj[iIndex].X = cX;
	gObj[iIndex].Y = cY;
	gObj[iIndex].MapNumber = nMapNumber;
	gObj[iIndex].TX = gObj[iIndex].X;
	gObj[iIndex].TY = gObj[iIndex].Y;
	gObj[iIndex].m_OldX = gObj[iIndex].X;
	gObj[iIndex].m_OldY = gObj[iIndex].Y;
	gObj[iIndex].StartX = gObj[iIndex].X;
	gObj[iIndex].StartY = gObj[iIndex].Y;

	LPMONSTER_ATTRIBUTE iAttr = gMAttr.GetAttr(541);

	if (iAttr == NULL)
	{
		gObjDel(iIndex);
		return -1;
	}

	gObj[iIndex].DieRegen = FALSE;
	gObj[iIndex].m_State = 1;
	gObj[iIndex].Level = iAttr->m_Level;
	gObjSetMonster(iIndex, 541);
	gObj[iIndex].Life = 100.0;
	gObj[iIndex].MaxLife = 100.0;
	gObj[iIndex].m_Defense = 100;
	gObj[iIndex].m_AttackDamageMin = 100;
	gObj[iIndex].m_AttackDamageMax = 100;
	gObj[iIndex].MaxRegenTime = 1000;
	gObj[iIndex].Dir = 1;

	g_Log.Add("[Doppelganger] Create MiddleTreasureBox Success [%d][%d, %d] (%d)", this->m_nMapNumber, cX, cY, iIndex);
	return iIndex;
}

BOOL CDoppelGanger::AddLastTreasureBox(BYTE cX, BYTE cY)
{
	int iIndex = gObjAddMonster(this->m_nMapNumber);

	if (iIndex < 0)
	{
		g_Log.Add("[Doppelganger] Create LastTreasureBox Fail [%d][%d, %d] (%d)", this->m_nMapNumber, cX, cY, iIndex);
		return FALSE;
	}

	gObj[iIndex].m_PosNum = -1;
	gObj[iIndex].X = cX;
	gObj[iIndex].Y = cY;
	gObj[iIndex].MapNumber = this->m_nMapNumber;
	gObj[iIndex].TX = gObj[iIndex].X;
	gObj[iIndex].TY = gObj[iIndex].Y;
	gObj[iIndex].m_OldX = gObj[iIndex].X;
	gObj[iIndex].m_OldY = gObj[iIndex].Y;
	gObj[iIndex].StartX = gObj[iIndex].X;
	gObj[iIndex].StartY = gObj[iIndex].Y;

	LPMONSTER_ATTRIBUTE iAttr = gMAttr.GetAttr(542);

	if (iAttr == NULL)
	{
		gObjDel(iIndex);
		return FALSE;
	}

	gObj[iIndex].DieRegen = FALSE;
	gObj[iIndex].m_State = 1;
	gObj[iIndex].Level = iAttr->m_Level;
	gObjSetMonster(iIndex, 542);
	gObj[iIndex].Life = 100.0;
	gObj[iIndex].MaxLife = 100.0;
	gObj[iIndex].m_Defense = 100;
	gObj[iIndex].m_AttackDamageMin = 100;
	gObj[iIndex].m_AttackDamageMax = 100;
	gObj[iIndex].MaxRegenTime = 1000;
	gObj[iIndex].Dir = 1;

	this->m_nLastTreasureBoxIndex = iIndex;
	EnterCriticalSection(&this->m_critTreasureBox);
	this->m_bOpenLastTreasureBox = false;
	LeaveCriticalSection(&this->m_critTreasureBox);

	g_Log.Add("[Doppelganger] Create LastTreasureBox Success [%d][%d, %d] (%d)", this->m_nMapNumber, cX, cY, iIndex);
	return TRUE;
}

int CDoppelGanger::AddMonsterLarva(int nMapNumber, BYTE btX, BYTE btY, int nMonsterLevel, int nMonsterHp, int nMonsterAttMin, int nMonsterAttMax, int nMonsterDef)
{
	int iIndex = gObjAddMonster(nMapNumber);

	if (iIndex < 0)
	{
		g_Log.Add("[Doppelganger] Create Larva Fail [%d][%d, %d] (%d)", this->m_nMapNumber, btX, btY, iIndex);
		return iIndex;
	}

	gObj[iIndex].m_PosNum = -1;
	gObj[iIndex].X = btX;
	gObj[iIndex].Y = btY;
	gObj[iIndex].MapNumber = nMapNumber;
	gObj[iIndex].TX = gObj[iIndex].X;
	gObj[iIndex].TY = gObj[iIndex].Y;
	gObj[iIndex].m_OldX = gObj[iIndex].X;
	gObj[iIndex].m_OldY = gObj[iIndex].Y;
	gObj[iIndex].StartX = gObj[iIndex].X;
	gObj[iIndex].StartY = gObj[iIndex].Y;

	LPMONSTER_ATTRIBUTE iAttr = gMAttr.GetAttr(532);

	if (iAttr == NULL)
	{
		gObjDel(iIndex);
		return -1;
	}

	gObj[iIndex].DieRegen = FALSE;
	gObj[iIndex].Level = iAttr->m_Level;
	gObjSetMonster(iIndex, 532);
	gObj[iIndex].Level = nMonsterLevel;
	gObj[iIndex].Life = nMonsterHp;
	gObj[iIndex].MaxLife = nMonsterHp;
	gObj[iIndex].m_Defense = nMonsterDef;
	gObj[iIndex].m_AttackDamageMin = nMonsterAttMin;
	gObj[iIndex].m_AttackDamageMax = nMonsterAttMax;
	gObj[iIndex].m_MagicDefense = nMonsterDef;
	gObj[iIndex].MaxRegenTime = 1000;
	gObj[iIndex].Dir = rand() % 8;

	return iIndex;
}

void CDoppelGanger::DelTreasureBox(int nIndex, int nMonsterClass)
{
	if (nMonsterClass == 541)
	{
		for (int Cnt = 0; Cnt < MAX_DOPPELGANGER_TREASURE_BOX; Cnt++)
		{
			if (this->m_nTeasureBoxIndex[Cnt] == nIndex)
			{
				gObjDel(nIndex);
				this->m_nTeasureBoxIndex[Cnt] = -1;
			}
		}

		this->m_bTreasureBoxRegen = false;
	}

	else if (nMonsterClass == 542)
	{
		gObjDel(nIndex);
		this->m_nLastTreasureBoxIndex = -1;
	}

	g_Log.Add("[Doppelganger] Delete TreasureBox Success [%d][%d][%d]", this->m_nMapNumber, nIndex, nMonsterClass);
}

BOOL CDoppelGanger::CheckMapTile(int nMapNumber, BYTE btX, BYTE btY)
{
	return this->m_PosInfo.CheckStartMapTile(nMapNumber, btX, btY);
}

void CDoppelGanger::SendMapTileInfo(LPOBJ lpObj, BYTE btMapSetType)
{
	char cTEMP_BUF[256];

	PMSG_SETMAPATTR_COUNT * lpMsg = (PMSG_SETMAPATTR_COUNT *)(cTEMP_BUF);
	PMSG_SETMAPATTR * lpMsgBody = (PMSG_SETMAPATTR *)(cTEMP_BUF + sizeof(PMSG_SETMAPATTR_COUNT));

	PHeadSetB((LPBYTE)lpMsg, 0x46, sizeof(PMSG_SETMAPATTR_COUNT)+sizeof(PMSG_SETMAPATTR)*2);

	lpMsg->btType = 0;
	lpMsg->btCount = 1;
	lpMsg->btMapAttr = 4;
	lpMsg->btMapSetType = btMapSetType;

	this->m_PosInfo.GetStartMapAttr(this->m_nMapNumber, &lpMsgBody[0].btX, &lpMsgBody[0].btY, &lpMsgBody[1].btX, &lpMsgBody[1].btY);

	IOCP.DataSend(lpObj->m_Index, (LPBYTE)&cTEMP_BUF, lpMsg->h.size);
}

void CDoppelGanger::SendMapTileInfoAll(BYTE btMapSetType)
{
	for (int Cnt = 0; Cnt < MAX_DOPPELGANGER_USER_INFO; Cnt++)
	{
		if (this->m_UserData[Cnt].IsUser() == TRUE && gObj[this->m_UserData[Cnt].m_nIndex].Connected > PLAYER_LOGGED)
		{
			this->SendMapTileInfo(&gObj[this->m_UserData[Cnt].m_nIndex], btMapSetType);
		}
	}
}

LPMONSTER_ATTRIBUTE CDoppelGanger::GetDefaultMonsterInfo(int nClass)
{
	LPMONSTER_ATTRIBUTE lpm = gMAttr.GetAttr(nClass);

	if (lpm == NULL)
	{
		g_Log.MsgBox("Warning! : Can't find monster attribute. %d (%s %d)", nClass, __FILE__, __LINE__);
		return NULL;
	}

	return lpm;
}

int CDoppelGanger::GetDefaultMonsterLevel(int nClass)
{
	int Index = nClass - 529;

	if (Index < 0 || Index >= MAX_DOPPELGANGER_MONSTER_ATTR)
	{
		return 1;
	}

	return this->m_pDoppelGangerAttr[Index]->m_Level;
}

int CDoppelGanger::GetDefaultMonsterHp(int nClass)
{
	int Index = nClass - 529;

	if (Index < 0 || Index >= MAX_DOPPELGANGER_MONSTER_ATTR)
	{
		return 1;
	}

	return this->m_pDoppelGangerAttr[Index]->m_Hp;
}

int CDoppelGanger::GetDefaultMonsterAttackMax(int nClass)
{
	int Index = nClass - 529;

	if (Index < 0 || Index >= MAX_DOPPELGANGER_MONSTER_ATTR)
	{
		return 1;
	}

	return this->m_pDoppelGangerAttr[Index]->m_DamageMax;
}

int CDoppelGanger::GetDefaultMonsterAttackMin(int nClass)
{
	int Index = nClass - 529;

	if (Index < 0 || Index >= MAX_DOPPELGANGER_MONSTER_ATTR)
	{
		return 1;
	}

	return this->m_pDoppelGangerAttr[Index]->m_DamageMin;
}

int CDoppelGanger::GetDefaultMonsterDefense(int nClass)
{
	int Index = nClass - 529;

	if (Index < 0 || Index >= MAX_DOPPELGANGER_MONSTER_ATTR)
	{
		return 1;
	}

	return this->m_pDoppelGangerAttr[Index]->m_Defense;
}

float CDoppelGanger::GetMonsterLevelWeight()
{
	return this->m_DPEventInfo.GetMonsterLevelWeight();
}

float CDoppelGanger::GetMonsterHpWeight()
{
	return this->m_DPEventInfo.GetMonsterHpWeight();
}

float CDoppelGanger::GetMonsterAttackWeight()
{
	return this->m_DPEventInfo.GetMonsterAttackWeight();
}

float CDoppelGanger::GetMonsterDefenseWeight()
{
	return this->m_DPEventInfo.GetMonsterDefenseWeight();
}

BOOL CDoppelGanger::GetMissionResult()
{
	return this->m_bMissionResult;
}

void CDoppelGanger::SetMiddleBossIndex(int nClass, int nIndex)
{
	if (nClass == 530)
	{
		for (int Cnt = 0; Cnt < MAX_DOPPELGANGER_BOSS_INFO; Cnt++)
		{
			if (this->m_nMiddleBossIndex[Cnt] == -1)
			{
				this->m_nMiddleBossIndex[Cnt] = nIndex;
				return;
			}
		}
	}
}

void CDoppelGanger::SetMiddleBossKill(int nClass, int nIndex)
{
	if (nClass == 531)
	{
		this->m_nMiddleBossIndex[3] = 1;
	}

	else if (nClass == 529)
	{
		this->m_nMiddleBossIndex[2] = 1;
	}

	else
	{
		for (int Cnt = 0; Cnt < MAX_DOPPELGANGER_BOSS_INFO; Cnt++)
		{
			if (this->m_nMiddleBossIndex[Cnt] == nIndex)
			{
				this->m_bMiddleBossKill[Cnt] = true;
				return;
			}
		}
	}
}

int CDoppelGanger::GetMiddleBossKill(int nNumber)
{
	return this->m_bMiddleBossKill[nNumber];
}

void CDoppelGanger::SendDoppelgangerTimerMsg(BYTE btMsg)
{
	PMSG_SET_DEVILSQUARE pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x92, sizeof(pMsg));

	pMsg.Type = btMsg;

	for (int Cnt = 0; Cnt < MAX_DOPPELGANGER_USER_INFO; Cnt++)
	{
		if (this->m_UserData[Cnt].IsUser() == TRUE && gObj[this->m_UserData[Cnt].m_nIndex].Connected > PLAYER_LOGGED)
		{
			IOCP.DataSend(this->m_UserData[Cnt].m_nIndex, (LPBYTE)&pMsg, pMsg.h.size);
		}
	}
}

void CDoppelGanger::SaveDoppelgangerTmpInfo() // not used
{

}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

