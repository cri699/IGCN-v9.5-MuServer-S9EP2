//////////////////////////////////////////////////////////////////////
// ImperialGuardian.h
#ifndef IMPERIALGUARDIAN_H
#define IMPERIALGUARDIAN_H

#include "ImperialGuardianRewardExp.h"
#include "EventDungeonItemBag.h"
#include "user.h"

#define MAX_ZONE 4

#define IG_STATE_READY 0
#define IG_STATE_TIMEATTACK 1
#define IG_STATE_WAITPLAYER 2
#define IG_STATE_LOOTTIME 3
#define IG_STATE_WARPNEXTZONE 4
#define IG_STATE_MISSIONFAIL 5
#define IG_STATE_ALLKICK 6
#define IG_STATE_MISSIONCLEAR 7

struct _stMonsterIndexInfo
{
	_stMonsterIndexInfo()
	{
		this->m_nIndex = -1;
		this->m_bLive = false;
		this->m_bAttackAble = true;
	}

	int m_nIndex;
	bool m_bLive;
	bool m_bAttackAble;
};

struct _stZoneInfo
{
	int m_nZoneState;
	int m_nPartyNumber;
	bool m_bWaitPlayer;
	bool m_bGoTimeAttack;
	bool m_bGoLooting;
	bool m_bGoWaitPlayer;
	bool m_bAblePortal;
	bool m_bStartRegenMonster;
	bool m_bSendWarningMsg;
	WORD m_nMonsterRegenTableIndex;
	int m_nMaxUserLevel;
	int m_nSendRemainCreatePortal;
	std::vector<_stMonsterIndexInfo> m_vtMonsterIndexInfo;
	std::vector<int> m_vtUserIndex;
	std::vector<int> m_vtRegPartyUser;
	int m_dwRemainTimeAttack;
	int m_dwRemainLootTime;
	int m_dwRemainWaitPlayerTime;
	int m_dwTickCount[3];
	std::vector<int> m_vtOpenedGate;
};

struct _stIFMonsterPos
{
	WORD m_Type;
	BYTE m_MapNumber;
	BYTE m_Dis;
	BYTE m_X;
	BYTE m_Y;
	BYTE m_Dir;
	BYTE m_Zone;
	BYTE m_Day;
};

class CImperialGuardian
{

public:

	CImperialGuardian();
	~CImperialGuardian();

	void Init();
	void InitEventDungeon();
	void LoadScript(LPSTR Filename);
	void LoadMonsterScript(LPSTR Filename);
	void LoadItemBag();
	void ImperialGuardianProcessInit();

	void Run();
	void CGEnterPortal(int nUserIndex, int nDestZoneIndex);
	int GetCurrentZoneIndex(int nUserNumber);
	void GCMissionFailUserDie(int nUserNumber);
	void DropItem(int nIndex, int nMonsterIndex);
	bool IsAttackAbleMonster(int nMonsterIndex);
	void DestroyGate(int nZoneIndex, int nIndex, int nTargetIndex);
	_stZoneInfo * GetZoneInfo(_stZoneInfo * result, int nZoneIndex);
	void SetTargetMoveAllMonster(int nZoneIndex, int nTargetNumber);
	void MonsterBaseAct(OBJECTSTRUCT* lpObj);
	void SetDayOfWeekGM(int nDayOfWeeks);
	void SetCheatModeGM(WORD nCheatMode);
	void WarpZoneGM(int nUserIndex, int nZoneIndex);
	int GetMixNeedZen();

	int GetPaperScrapDropRate() { return this->m_nPaperScrapDropRate; }
	int GetPaperScrapDropLevel() { return this->m_nPaperScrapDropLevel; }

private:

	void ProcReady(int ZoneIndex);
	void ProcBeginTimeAttack(int ZoneIndex);
	void ProcBeginLootTime(int ZoneIndex);
	void ProcBeginWaitPlayer(int ZoneIndex);
	void ProcAllWarpNextZone(int ZoneIndex);
	void ProcAllKick(int ZoneIndex);
	void ProcMissionFail(int ZoneIndex);
	void ProcMissionClear(int ZoneIndex);
	bool IsEmptyZone(int ZoneIndex);
	void KickInvalidUser();
	bool CheckValidUser(int nUserNumber);
	int GetZoneState(int nZoneIndex);
	bool SetZoneState(int nZoneIndex, int nState);	
	void RegenMonster(int nZoneIndex, int nMonsterRegenTableIndex, int nMaxUserLevel, bool bOnlyCreateGate);
	void DeleteMonster(int nZoneIndex, int nMonsterRegenTableIndex);
	void InitZone(int nZoneIndex);
	bool SetPosMonster(int nIndex, std::vector<_stIFMonsterPos>::iterator Iter);
	bool SetMonster(int nIndex, int MonsterClass, int nMaxLevel);
	int GetGateNumber(int nZoneIndex, int nDayOfWeek);
	int GetMapNumber(int nDayOfWeek);
	void ProcCheckDungeon(int nZoneIndex);
	int GetLiveMonsterCount(int nZoneIndex);
	void CheckLiveMonster(int nZoneIndex);
	bool AddUserInZone(int nZoneIndex, int nUserNumber);
	void RemoveUserInZone(int nZoneIndex, int nUserNumber);
	void RemoverUserInAllZone(int nUserNumber);
	int GetPlayerCount(int nZoneIndex);
	int GetTotalPlayerCount();
	bool IsLastZone(int nZoneIndex);
	bool GCNotifyZoneClear(int nZoneIndex);
	bool GCNotifyAllZoneClear(int nZoneIndex);
	void GCMissionFail(int nZoneIndex);
	void GCSendDataToUser(int nIndex, char *lpMsg, int nSize);
	void GCNotifyRemainTickCount(int nZoneIndex, char btMsgType, DWORD dwRemainTick);
	void SetGateBlockState(int nMapNumber, int nZoneIndex, int nX, int nY, int iGateState, int nDir);
	void GCSendCastleGateInfo(int nGateIndex, int nZoneIndex, int nUserIndex);
	BOOL CheckOverlapMysteriousPaper(int nIndex, int nItemLevel);
	void RollBackUserPos(int nUserNumber);
	BOOL CheckGaionOrderPaper(int nIndex);
	BOOL CheckFullSecromicon(int nIndex);
	void SetAtackAbleState(int nZoneIndex, int nMonsterClass, bool bState);
	void GCSendServerMsgAll(int nZoneIndex, char *szMsg);
	int ImperialGuardianLevelUp(int iIndex, int iAddExp);
	void RegAllPartyUser(int nZoneIndex, int nFirstEnterUserIndex);
	bool IsRegPartyUser(int nZoneIndex, int nUserIndex);
	void UserMonsterCountCheck();

	WORD m_nCheatMode;
	int m_nDay;
	int m_nWeather;
	bool m_bLoadScript;
	bool m_bLoadItemBag;
	bool m_bEnableEvent;
	_stZoneInfo m_stZoneInfo[MAX_ZONE];
	std::vector<_stIFMonsterPos> m_vtMonsterPosInfo;
	int m_nDayOfWeeksForCheat;
	int m_dwDefaultLootTime;
	int m_dwDefaultWaitPlayerTime;
	int m_dwDefaultTimeAttackTime;
	int m_nMixNeedZen;
	int m_nPaperScrapDropRate;
	int m_nPaperScrapDropLevel;
	CImperialGuardianRewardExp m_rewardExp;
	CEventDungeonItemBag * pEventDungeonItemBagGaion;
	CEventDungeonItemBag * pEventDungeonItemBag;
	CEventDungeonItemBag * pEventDungeonItemBagStatue;
	CRITICAL_SECTION m_criUserIndex;
};

extern CImperialGuardian g_ImperialGuardian;

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

