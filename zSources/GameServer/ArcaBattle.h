//////////////////////////////////////////////////////////////////////
// ArcaBattle.h
#ifndef ARCABATTLE_H
#define ARCABATTLE_H

#include "prodef.h"
#include "user.h"

#define MAX_ARCA_BATTLE_PLAYERS						200
#define MAX_ARCA_BATTLE_GUILD_MASTER_GATE_INFO		10
#define MAX_ARCA_BATTLE_BOOTY_MIX_INFO				10

enum ARCA_BATTLE_OBELISK_STATE
{
	ARCA_BATTLE_OBELISK_UNCAPTURED = 0,
	ARCA_BATTLE_OBELISK_CAPTURING = 1,
	ARCA_BATTLE_OBELISK_CAPTURED = 2
};

enum ARCA_BATTLE_GATENUM_ZONES
{
	ZONE_ALKMAR = 427,
	ZONE_UBAID = 431,
	ZONE_DEBENTER = 439
};

enum ARCA_BATTLE_STATE
{
	ARCA_STATE_SYNC = 1,
	ARCA_STATE_CLOSED = 2,
	ARCA_STATE_MASTER_REG = 3,
	ARCA_STATE_MEMBER_REG = 4,
	ARCA_STATE_READY = 5,
	ARCA_STATE_MAKE_PARTY = 6,
	ARCA_STATE_PLAYING = 7,
	ARCA_STATE_RESULT = 8,
	ARCA_STATE_CHANNEL_CLOSE = 9,
	ARCA_STATE_END = 10
};

// Data arrays

struct _tagARCA_USER_INFO
{
	_tagARCA_USER_INFO()
	{
		this->Clear();
	}

	void Clear()
	{
		memset(this->szGuildName, 0x00, sizeof(szGuildName));
		this->dwGuildNum = 0;
		memset(this->szUserName, 0x00, sizeof(szUserName));
	}

	char szGuildName[MAX_GUILD_LEN + 1];
	DWORD dwGuildNum;
	char szUserName[MAX_ACCOUNT_LEN + 1];
};

struct _tagAURA_INFO
{
	_tagAURA_INFO()
	{
		this->m_iGroupNumber = -1;
		this->m_iMapIndex = -1;
		this->m_iAuraPosX = -1;
		this->m_iAuraPosY = -1;
	}

	int m_iGroupNumber;
	int m_iMapIndex;
	int m_iAuraPosX;
	int m_iAuraPosY;
};

struct _tagOBELISK_INFO
{
	_tagOBELISK_INFO()
	{
		this->Clear();
	}

	void Clear()
	{
		this->m_iGroupNumber = 0;
		this->m_iAttrKind = 0;
		this->m_iMapIndex = -1;
		this->m_iHP = 0;
		this->m_iOccupyHP = 0;
		this->m_iPosX = -1;
		this->m_iPosY = -1;
		this->m_iMonsterNum = 0;
		this->m_i64Tick = 0;
		this->m_i64RegenTime = 0;
		memset(&this->m_stAuraInfo, -1, sizeof(this->m_stAuraInfo));
	}

	int m_iGroupNumber;
	int m_iAttrKind;
	int m_iHP;
	int m_iOccupyHP;
	int m_iMapIndex;
	int m_iPosX;
	int m_iPosY;
	int m_iMonsterNum;
	ULONGLONG m_i64Tick;
	ULONGLONG m_i64RegenTime;
	_tagAURA_INFO m_stAuraInfo[3];
};

struct _tagARCA_BATTLE_BOOTY_REWARD
{
	_tagARCA_BATTLE_BOOTY_REWARD() {
	  this->iItemLink = 0;
	  this->iItemType = 0;
	  this->iItemIndex = 0;
	  this->iItemLevel = 0;
	  this->iDurab = 0;
	  this->iRate = 0;
	}

	int iItemLink;
	int iItemType;
	int iItemIndex;
	int iItemLevel;
	int iDurab;
	int iRate;
};
struct _tagARCA_BATTLE_BOOTY_MIX
{
	_tagARCA_BATTLE_BOOTY_MIX(){
		  this->iItemLink = 0;
		  this->iMinCnt = 0;
		  this->iMaxCnt = 0;
		  this->iBaseRate = 0;
		  this->iAddRate = 0;
		  this->iBootyRewardCnt = 0;
		 
	}

	int iItemLink;
	int iMinCnt;
	int iMaxCnt;
	int iBaseRate;
	int iAddRate;
	int iBootyRewardCnt;
	_tagARCA_BATTLE_BOOTY_REWARD stBootyReward[20];
};

struct _stABAcquiredPoints
{
	_stABAcquiredPoints()
	 {
	  this->Clear();
	 }

	 void Clear()
	{
	  this->dwBootyCnt = 0;
	  this->dwContributePoints = 0;
	  this->dwKillPoints = 0;
	 }

	 unsigned int dwBootyCnt;
	 unsigned int dwContributePoints;
	 unsigned int dwKillPoints;
};
struct _tagArcaBattleUserInfo
{
	void Clear()
	{
		memset(this->szCharName, 0x00, sizeof(this->szCharName));
		this->bBootyDrop = false;
		this->iIndex = -1;
		this->m_stAcquiredPoints.Clear();
	}

	char szCharName[11];
	int iIndex;
	bool bBootyDrop;
	_stABAcquiredPoints m_stAcquiredPoints;
};

struct _tagAURA_STATE
{
	_tagAURA_STATE()
	{
		this->m_iGroupNumber = -1;
		this->m_btAuraState = 0;
		this->m_iHoverUserIndex = 0;
		this->m_iIndex = -1;
	}

	int m_iGroupNumber;
	char m_btAuraState;
	int m_iHoverUserIndex;
	int m_iIndex;
};

struct _tagOBELISK_STATE
{
	_tagOBELISK_STATE()
	{
		this->Clear();
	}

	void Clear()
	{
		this->m_iGroupNumber = -1;
		this->m_iMonIndex = -1;
		this->m_iAuraState = 0;
		this->m_iAuraReleaseGuildNum = -1;
		this->m_iOccupyGuildNum = -1;
		this->m_iObeliskState = 0;
		memset(this->m_szOccupyGuildName, 0, 9);
		memset(this->m_stAuraState, -1, sizeof(this->m_stAuraState));
	}

	int m_iGroupNumber;
	int m_iMonIndex;
	int m_iAuraState;
	int m_iAuraReleaseGuildNum;
	int m_iObeliskState;
	int m_iOccupyGuildNum;
	char m_szOccupyGuildName[9];
	_tagAURA_STATE m_stAuraState[3];
};

struct _tagARCA_BATTLE_RESULT_INFO
{
	int iGuildNum;
	int iObeliskAttrKind;
};

struct _tagOccupyGuildInfo
{
	int iObeliskGroupNum;
	int iGuildNum;
	int iObeliskAttrKind;
	int nDuration;
};

struct _stABWinGuildInfo
{
	void Clear()
	{
		  memset(this->szGuildName, 0, MAX_GUILD_LEN+1);
		  this->dwGuild = 0;
		  this->wOccupyObelisk = 0;
		  this->wObeliskGroup = 0;
	}

	char szGuildName[MAX_GUILD_LEN+1];
	DWORD dwGuild;
	WORD wOccupyObelisk;
	WORD wObeliskGroup;
};

struct _tagARCA_BATTLE_START_TIME
{
	_tagARCA_BATTLE_START_TIME()
	{
		  this->m_iHour = 0;
		  this->m_iMinute = 0;
		  this->m_btEvent = 0;
	}

	int m_iHour;
	int m_iMinute;
	char m_btEvent;
};

struct _stABMonRegenArea
{
	_stABMonRegenArea()
	{
		this->m_nBeginX = -1;
		this->m_nBeginY = -1;
		this->m_nEndX = -1;
		this->m_nEndY = -1;
	}

	int m_nBeginX;
	int m_nBeginY;
	int m_nEndX;
	int m_nEndY;
};

/* 3515 */
struct _stABMonIndexInf
{
	_stABMonIndexInf()
	{
		this->m_nMonsterIndex = -1;
	}

	int m_nMonsterIndex;
};

struct _stABMonPosition
{
	void Clear()
	{
		this->m_nGroupNumber = -1;
		this->m_nRegenAreaCnt = 0;
	}

	_stABMonPosition()
	{
		this->Clear();
	}

	int m_nGroupNumber;
	int m_nRegenAreaCnt;
	int m_nMonsterKindCnt;
	_stABMonRegenArea m_stRegenArea[50];
	_stABMonIndexInf m_stMonsterIndexInfo[20];
};

struct _stABMonAccountNumInfo
{
	_stABMonAccountNumInfo()
	{
		this->m_nIndex = -1;
		this->bItemDrop = false;
	}

	int m_nIndex;
	bool bItemDrop;
};

struct _stABMonGroupInfo
{
	_stABMonGroupInfo()
	{
		this->Clear();
	}

	void Clear()
	{
		this->m_nGroupNumber = -1;
		this->m_nMapNumber = -1;
		this->m_nCnt = 0;
		this->m_nRegenTimeMin = 0;
		this->m_i64NextRegenTime = 0;
	}

	int m_nGroupNumber;
	int m_nMapNumber;
	int m_nCnt;
	int m_nRegenTimeMin;
	ULONGLONG m_i64NextRegenTime;
	_stABMonAccountNumInfo m_stMonsterAccountNumInfo[255];
};

struct _tagAB_USER_AREA
{
	_tagAB_USER_AREA()
	{
		this->map_number = -1;
		this->beginX = -1;
		this->beginY = -1;
		this->endX = -1;
		this->endY = -1;
	}

	int map_number;
	int beginX;
	int beginY;
	int endX;
	int endY;
};

/* 3521 */
struct _stAB_USER_POSITION
{
	_stAB_USER_POSITION()
	{
		this->group_number = -1;
		this->area_cnt = 0;
	}

	int group_number;
	int area_cnt;
	_tagAB_USER_AREA stUserPos[10];
};

struct _tagAB_GUILD_MEMBER_INFO_LOG
{
	 _tagAB_GUILD_MEMBER_INFO_LOG()
	 {
		 this->Clear();
	 }

	 void Clear()
	 {
		 this->nGuildNum = -1;
		 this->bLogPrint = false;
	 }
	  int nGuildNum;
	  bool bLogPrint;
};

struct _stAB_GUILD_MASTER_GATE_INFO
{
	 _stAB_GUILD_MASTER_GATE_INFO()
	 {
		this->Clear();
	 }

	 void Clear()
	 {
		  this->nGuildNum = -1;
		  this->nGateNum = -1;
	 }

	 int nGuildNum;
	 int nGateNum;
};

struct _stAB_GUILD_MEMBER_GATE_INFO
{
	_stAB_GUILD_MEMBER_GATE_INFO()
	{
		this->Clear();
	}

	void Clear()
	{
		this->nGuildNum = -1;
		szMemberName.clear();
		this->nGateNum = -1;
	}

	int nGuildNum;
	std::string szMemberName;
	int nGateNum;
};

// Protocols
struct _stAuraInfo
{
  int m_iAuraPosX;
  int m_iAuraPosY;
};

struct _stArcaBattleObeliskInfo
{
  int iGroupNumber;
  int iAttrKind;
  int iMapIndex;
  int iPosX;
  int iPosY;
  _stAuraInfo m_StAuraInfo[3];
};

struct _tagPMSG_ARCA_BATTLE_PLAY_INFO
{
	PBMSG_HEAD2 h;
	WORD wGuildGroupNum;
	_stArcaBattleObeliskInfo m_stObeliskInfo[5];
};

struct _tagPMSG_REQ_REMOVE_ALL_GUILD_BUFF_DS
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;
};

struct _tagPMSG_ANS_REMOVE_ALL_GUILD_BUFF_DS
{
	PBMSG_HEAD2 h;
};

struct _tagPMSG_ANS_REMOVE_GUILD_BUFF_DS
{
  PBMSG_HEAD2 h;
  char szGuildName[9];
};

struct _tagPMSG_SEND_AB_WIN_GUILD_INFO
{
  PBMSG_HEAD2 h;
  char szGuildName[9];
};

/* 3635 */
struct _stObeliskLife
{
  char btLife;
  char btAttribute;
};

/* 3636 */
struct _tagPMSG_SEND_OBELISK_LIFE
{
  PBMSG_HEAD2 h;
  _stObeliskLife m_stObeliskLife[5];
};

struct _tagPMSG_SEND_OBELISK_STATE
{
  PBMSG_HEAD2 h;
  char btObeliskAttribute;
  char btObeliskState;
  char btAuraState;
};

struct _tagPMSG_ARCA_BATTLE_OBELISK_INFO
{
  PBMSG_HEAD2 h;
  char m_btObeliskAttribute;
  char m_btAuraState[3];
};

struct _tagPMSG_ARCA_BATTLE_STATE
{
  PBMSG_HEAD2 h;
  char m_btArcaBattleState;
};

struct _stABObeliskOccupy
{
  unsigned __int16 wObeliskKind;
  char szGuildName[9];
};

struct _tagPMSG_AB_RESULT
{
	PBMSG_HEAD2 h;
	char btABResult;
	unsigned int dwContributePoint;
	unsigned int dwKillPoint;
	unsigned int dwRewardExp;
	unsigned __int16 wBootyCnt;
	_stABObeliskOccupy m_stABObeliskOccupy[5];
};

// DS
struct _tagPMSG_REQ_AB_WIN_GUILD_INFO_DS
{
	PBMSG_HEAD2 h;
	WORD wNumber;
};

struct _tagPMSG_REQ_AB_PROC_STATE_DS
{
	PBMSG_HEAD2 h;
};

struct _stABWinGuildInfoDS
{
	char szGuildName[9];
	DWORD dwGuild;
	WORD wOccupyObelisk;
	WORD wObeliskGroup;
};

struct _tagPMSG_REQ_AB_WIN_GUILD_INFO_INSERT_DS
{
	PBMSG_HEAD2 h;
	char btGuildCnt;
	unsigned __int16 wMapSvrNum;
	_stABWinGuildInfoDS m_stABWinGuildInfoDS[5];
};

struct _tagPMSG_REQ_ARCA_BATTLE_INFO_DS
{
	PBMSG_HEAD2 h;
};

struct _tagPMSG_REQ_AB_PROC_INSERT_DS
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;
	BYTE btProcState;
};

struct _tagPMSG_REQ_AB_JOIN_MEMBER_UNDER_DS
{
	PBMSG_HEAD2 h;
	WORD wMapSvrNum;
};

struct _tagPMSG_REQ_AB_JOIN_CANCEL_DS
{
	PBMSG_HEAD2 h;
	BYTE btMinGuildMemNum;
	WORD wMapSvrNum;
};

struct _stCancelGuildNames
{
	char szGuildNames[9];
};

struct _tagPMSG_ANS_AB_JOIN_CANCEL_DS
{
	PBMSG_HEAD2 h;
	BYTE btGuildCnt;
	_stCancelGuildNames CancelGuildNames[6];
};

struct _tagPMSG_SEND_AB_JOIN_CANCEL
{
	PBMSG_HEAD2 h;
};

struct _tagPMSG_ANS_AB_REG_MEMBER_CNT_DS
{
	PBMSG_HEAD2 h;
	WORD wNumber;
	BYTE btRegMemCnt;
};

struct _tagPMSG_ANS_AB_REG_MEMBER_CNT
{
	PBMSG_HEAD2 h;
	BYTE btResult;
	BYTE btRegMemCnt;
};

struct _tagPMSG_REQ_AB_REG_MEMBER_CNT_DS
{
	PBMSG_HEAD2 h;
	int iIndex;
	int iGuildNumber;
};

struct PMSG_ANS_REGISTERED_MEMBER_CNT
{
	PBMSG_HEAD2 h;
	BYTE unk1;
	BYTE unk2;
};

struct _tagPMSG_ANS_AB_WIN_GUILD_INFO_DS
{
	PBMSG_HEAD2 h;
	char btGuildCnt;
	_stABWinGuildInfoDS m_stABWinGuildInfoDS[5];
};

struct _tagPMSG_REQ_ARCA_BATTLE_IS_TOP_RANK
{
	PBMSG_HEAD2 h;
	WORD wNumber;
	DWORD dwGuildNum;
};

struct _stArcaBattleMarkTopRankDS
{
	BYTE btRank;
	char szGuildNames[MAX_GUILD_LEN+1];
	DWORD dwMarkCnt;
};

struct _tagPMSG_ANS_ARCA_BATTLE_MARK_RANK_DS
{
	PBMSG_HEAD2 h;
	WORD wNumber;
	BYTE btRank;
	DWORD dwMarkCnt;
	BYTE btGuildCnt;
	_stArcaBattleMarkTopRankDS ArcaBattleMarkTopRank[6];
};

struct _tagPMSG_REQ_ARCA_BATTLE_MARK_RANK_DS
{
	PBMSG_HEAD2 h;
	WORD wNumber;
	DWORD dwGuildNum;
};

struct _tagPMSG_REQ_ARCA_BATTLE_MARK_CNT_DS
{
	PBMSG_HEAD2 h;
	WORD wNumber;
	DWORD dwGuildNum;
};

struct _tagPMSG_ANS_ARCA_BATTLE_MARK_CNT_DS
{
	PBMSG_HEAD2 h;
	BYTE btResult;
	WORD wNumber;
	DWORD dwMarkCnt;
};

struct _tagPMSG_REQ_ARCA_BATTLE_MARK_REG_DS
{
	PBMSG_HEAD2 h;
	WORD wNumber;
	char szGuildMaster[MAX_ACCOUNT_LEN+1];
	char szGuildName[MAX_GUILD_LEN+1];
	DWORD dwMarkCnt;
	DWORD dwGuildNum;
};

struct _tagPMSG_REQ_ARCA_BATTLE_MARK_REG_DEL_DS
{
	PBMSG_HEAD2 h;
	WORD wNumber;
	DWORD dwGuildNum;
};

struct _tagPMSG_REQ_AB_MARK_REG_ALL_DEL_DS
{
	PBMSG_HEAD2 h;
};

struct _tagPMSG_ANS_ARCA_BATTLE_MARK_REG_DS
{
	PBMSG_HEAD2 h;
	WORD wNumber;
};

struct _tagPMSG_REQ_ALL_GUILD_MARK_CNT_DS
{
	PBMSG_HEAD2 h;
};

struct _stABAllGuildMark
{
	char szGuildName[MAX_GUILD_LEN+1];
	DWORD dwMarkCnt;
};

struct _tagPMSG_ANS_ALL_GUILD_MARK_CNT_DS
{
	PWMSG_HEAD2 h;
	WORD wGuildCnt;
	_stABAllGuildMark ABAllGuildMark[250];
};

struct _tagPMSG_REQ_AB_MARK_REG_UPDATE_DS
{
	PBMSG_HEAD2 h;
	DWORD dwGuildNum;
	DWORD dwMarkCnt;
};

struct _tagPMSG_REQ_GUILD_REG_INIT
{
	PBMSG_HEAD2 h;
};

struct _tagPMSG_REQ_AB_ALL_JOIN_USER_DS
{
	PBMSG_HEAD h;
};

struct _stABJoinUserInfoDS
{
	char szGuildName[MAX_GUILD_LEN+1];
	DWORD dwGuild;
	char szUserName[MAX_ACCOUNT_LEN+1];
};

struct _tagPMSG_ANS_AB_ALL_JOIN_USER_DS
{
	PWMSG_HEAD h;
	BYTE btUserCnt;
	_stABJoinUserInfoDS stABJoinUserInfo[200];
};

#pragma pack (1)

struct _stABCurJoinGuildUserInfo
{
	char szUserName[MAX_ACCOUNT_LEN+1];
	BYTE btStatus;
	BYTE btPosX;
	BYTE btPosY;
	WORD wGuildMasterGateNum;
};

struct _stPMSG_AB_JOIN_USER_COUNT
{
	_stPMSG_AB_JOIN_USER_COUNT()
	{
		this->Result = 0;
		this->btUserCnt = 0;
		this->iGuildNumber = 0;
		memset(this->szGuildMasterName, 0x00, MAX_ACCOUNT_LEN+1);
		this->dwGuildMasterGateNum = 0;
		this->btGuildMasterPosX = 0;
		this->btGuildMasterPosY = 0;
	}

	PWMSG_HEAD2 h;
	BYTE Result;
	BYTE btUserCnt;
	int iGuildNumber;
	char szGuildMasterName[11];
	int dwGuildMasterGateNum;
	BYTE btGuildMasterPosX;
	BYTE btGuildMasterPosY;
};

struct _tagPMSG_ARCA_BATTLE_CURRENT_STATUS
{
	_tagPMSG_ARCA_BATTLE_CURRENT_STATUS()
	{
		this->iObelGroupNum = 0;
		this->iAttrKind = 0;
		this->btCurStatus = 0;
		this->dwGuild = 0;
		memset(this->szGuildName, 0x00, MAX_GUILD_LEN + 1);
	}

	PBMSG_HEAD2 h;
	int iObelGroupNum;
	int iAttrKind;
	BYTE btCurStatus;
	DWORD dwGuild;
	char szGuildName[MAX_GUILD_LEN+1];
};

struct _tagPMSG_ANS_MARK_REG
{
	PBMSG_HEAD2 h;
	BYTE btMarkCnt1;
	BYTE btMarkCnt2;
	BYTE btMarkCnt3;
	BYTE btMarkCnt4;
};

struct _tagPMSG_ANS_MARK_REG_ERROR_CODE
{
	PBMSG_HEAD2 h;
	BYTE btResult;
};

struct _stArcaBattleMarkTopRank
{
	BYTE btRank;
	char szGuildNames[MAX_GUILD_LEN+1];
	BYTE btMarkCnt1;
	BYTE btMarkCnt2;
	BYTE btMarkCnt3;
	BYTE btMarkCnt4;
};

struct _tagPMSG_ANS_MARK_RANK
{
	_tagPMSG_ANS_MARK_RANK()
	{
		this->btRank = 0;
		this->btMarkCnt1 = 0;
		this->btMarkCnt2 = 0;
		this->btMarkCnt3 = 0;
		this->btMarkCnt4 = 0;
		this->btGuildCnt = 0;
	}

	PBMSG_HEAD2 h;
	BYTE btRank;
	BYTE btMarkCnt1;
	BYTE btMarkCnt2;
	BYTE btMarkCnt3;
	BYTE btMarkCnt4;
	BYTE btGuildCnt;
};

struct _tagPMSG_ARCA_BATTLE_NOTI_MSG
{
	PBMSG_HEAD2 h;
	int iNotiIndex;
	int iValue;
};

struct _stABJoinUserInfo
{
	_stABJoinUserInfo()
	{
		this->Clear();
	}

	void Clear()
	{
		memset(this->szGuildName, 0x00, MAX_GUILD_LEN+1);
		this->dwGuild = 0;
		memset(this->szUserName, 0x00, MAX_ACCOUNT_LEN+1);
		this->wGuildMasterGateNum = 0;
	}

	char szGuildName[MAX_GUILD_LEN+1];
	int dwGuild;
	char szUserName[MAX_ACCOUNT_LEN+1];
	WORD wGuildMasterGateNum;
};

#pragma pack ()

class CArcaBattle
{
public:
	CArcaBattle();
	virtual ~CArcaBattle();

	void Clear();
	bool LoadScriptArcaBattle(char *lpszFileName);
	void BootyExchange(OBJECTSTRUCT *lpObj);
	int LoadScriptBootyMix(char *lpszFileName);
	int GetBootyRewardItem(int iBootyCnt, _tagARCA_BATTLE_BOOTY_REWARD *pBootyRewardItem);

	WORD GetObeliskAttrToRewardBuffIndex(int iObeliskAttr);
	void SendArcaBattlePlayInfo(OBJECTSTRUCT *obj, WORD wGuildGroupNum);
	void WinGuildMemberAddBuff(OBJECTSTRUCT *lpObj, unsigned __int16 wObeliskAttr);
	void GiveRewardBuffWinningGuild();
	void ReqRemoveRewardGuildBuff(char *szGuildName);
	void ReqRemoveRewardBuffWinningGuild();
	void DGAnsRemoveRewardBuff(_tagPMSG_ANS_REMOVE_GUILD_BUFF_DS *lpMsg);
	void RemoveGuildBuff(char *szGuildName, WORD wBuffIndex);
	void RemoveRewardBuff(char *szGuildName);
	void DGAnsRemoveAllRewardBuff();
	void ReqGuildMemberDelBuffDel(OBJECTSTRUCT *lpObj, char *szGuildName);
	void ReqGuildMemberDelBuffDel(char *szName, char *szGuildName);
	void AddArcaBattleWinGuildInfo(_stABWinGuildInfoDS *pABWinGuildInfoDS, int iGuildCnt);
	void SetDataLoadWinGuild(int bReqABWinGuildInfo);
	int IsArcaBattleWinGuild(OBJECTSTRUCT *obj);
	int IsArcaBattleOccupyZone(OBJECTSTRUCT *obj, int gt);
	void SendArcaBattleOccupyZone(OBJECTSTRUCT *obj, int gt);

	void Init();
	int CheckSync();

	void SetState(int iAB_State);
	int IsArcaBattleServer();
	int IsArcaBattleDay();
	void GDReqArcaBattleProcMultiCast(int iState);

	void SetStateReady();
	void SetStateClosed();
	void SetStatePlayEnd();
	void SetStateMasterReg();

	int GetState();

	int DBDataRequest();

	void ProcStateClosed();
	void ProcStateMasterReg();
	void ProcStateReg();
	void ProcStateReady();
	void ProcStatePaty();
	void ProcStatePlaying();
	void ProcStateResult();
	void ProcStateChannelClose();
	void ProcStatePlayEnd();

	void DeleteArcaBattleAllMonster();

	void Run();

	void SendMapServerGroupMsg(char *lpszMsg);
	int GetJoinMemberCnt();

	int EnterArcaBattleEvent(int aIndex);
	_tagArcaBattleUserInfo * GetUserInfo(char *szName);

	void SendObeliskLife();
	void SendObeliskState();
	int RegenObelisk();
	void SetRandomObeliskAttr();
	int SetObelisk();
	void ChkAuraUserHover();
	int GetChkAura();
	int GetMaxObeliskCnt();

	int IsEnableAttackObelisk(OBJECTSTRUCT *obj, int iMonNumber);
	int IsPkEnable(OBJECTSTRUCT *obj, OBJECTSTRUCT *Tarobj);

	void SendArcaBattleStateAll(int iState);
	void SendArcaBattleCurrentState(OBJECTSTRUCT *obj);

	int IsOccupyObelisk(OBJECTSTRUCT *obj, int iMonIndex);

	void NotifyArcaBattle(int iNotifyNum);

	void GDReqWinGuildInfo();
	void GDReqProcState();
	void GDReqWinGuildInfoInsert();

	void SendPlayResult();

	void RegenMonsterRun();
	void DeleteMonster(int nGroupNumber);
	void RegenMonster(int nGroupNumber);
	int SetPosMonster(int aIndex, int nMapNumber, int nBeginX, int nBeginY, int nEndX, int nEndY);
	int GetBoxPosition(int mapnumber, int ax, int ay, int aw, int ah, short &mx, short &my);

	void AddContributePoint(OBJECTSTRUCT *obj, OBJECTSTRUCT *Tarobj);
	void AddKillPoint(OBJECTSTRUCT *obj, OBJECTSTRUCT *Tarobj);

	void BootyItemGetCnt(OBJECTSTRUCT *lpObj);
	void CalcRewardExp(OBJECTSTRUCT *lpObj, UINT64 & iRewardExp);

	int DropItem(OBJECTSTRUCT *lpObj, OBJECTSTRUCT *lpMonsterObj);

	void GDReqDeleteArcaBattleInfo();
	void GDReqJoinMemberUnder();
	void GDReqArcaBattleJoinMemberUnderReq();
	void DGAnsArcaBattleJoinMemberUnderReq(_tagPMSG_ANS_AB_JOIN_CANCEL_DS *lpMsg);
	void DGAnsRegisteredMemberCnt(_tagPMSG_ANS_AB_REG_MEMBER_CNT_DS *lpMsg);

	int GetGuildMasterPos(OBJECTSTRUCT *obj, int nGateNum, short & x, short & y);
	void SetGuildMasterGateNum(int nGateNum, int nGuildNum);
	int GetGuildMasterGateNum(int nGuildNum);

	int GetUserPos(int nGateNum, short  & nOutX, short  & nOutY);
	void SetUserMapInfo(OBJECTSTRUCT *lpObj, int nGateNum);

	void SetArcaBattleProcMultiCast(int bABInitState);

	void GDReqArcaBattleIsTopRank(int iIndex, DWORD dwGuildNum);
	bool CGReqMarkReg(int iIndex);
	void GCAnsMarkReg(int iIndex, DWORD dwMarkCnt);
	void GCAnsMarkRegErrCode(int iResult, int iIndex);

	void CGReqMarkRegButtonClick(OBJECTSTRUCT *lpObj);

	void CGReqMarkRank(int iIndex);
	void GCAnsMarkRank(int iIndex, BYTE btRank, DWORD dwMarkCnt, BYTE btTopRankCnt, _stArcaBattleMarkTopRankDS *pArcaBattleMarkTopRank);

	void GDReqMarkCnt(int iIndex, DWORD dwGuildNum);
	void DGAnsMarkCnt(_tagPMSG_ANS_ARCA_BATTLE_MARK_CNT_DS *lpMsg);
	void GDReqMarkReg(int iIndex, DWORD dwMarkCnt);
	void GDReqMarkRank(int iIndex, DWORD dwGuildNum);
	void DGAnsMarkRank(_tagPMSG_ANS_ARCA_BATTLE_MARK_RANK_DS *lpMsg);
	void GDReqMarkRegDel(int iIndex, DWORD dwGuildNum);
	void GDReqMarkRegAllDel();
	void DGAnsMarkReg(_tagPMSG_ANS_ARCA_BATTLE_MARK_REG_DS *lpMsg);
	void GDReqAllGuildMarkCnt();
	void DGAnsAllGuildMarkCnt(_tagPMSG_ANS_ALL_GUILD_MARK_CNT_DS *lpMsg);

	void SetRegenObelisk(int iGroupNum, int iAttribute);

	void SetCheatABStart();
	void SetCheatABEnd();
	void SetCheatABNextProc();

	void CheatGDReqArcaBattleGuildJoin(OBJECTSTRUCT *lpObj);
	void CheatGDReqArcaBattleGuildMemberJoin(OBJECTSTRUCT *lpObj, const char *lpszName);

	void CheatABOccupyObelisk(OBJECTSTRUCT *lpObj, int iObeliskGroup);

	void SetJoinMemberCnt(int iJoinGuildMemberCnt);

	void CheatGDReqMarkReg(int iIndex, char *szGuildName, DWORD dwGuildNum, DWORD dwMarkCnt);
	void CheatGDReqMarkRegSet(int iIndex, DWORD dwMarkCnt);

	void GuildMemberAssignStatus(OBJECTSTRUCT *lpObj, int iGuildStatus);

	void GDReqGuildRegInit();

	void PrintGuildMemberLog(_GUILD_INFO_STRUCT *lpGuild);

	void GDReqArcaBattleAllJoinUser();
	void DGAnsArcaBattleAllJoinUser(_tagPMSG_ANS_AB_ALL_JOIN_USER_DS *lpMsg);

	void SetDataLoadProcState(int bReqABProcState);

	void GCArcaBattleUserInfo(int iUserIndex);
	void GCArcaBattleCurrentStatus(int iObeliskIndex, int iAttrKind, BYTE btObeliskState, int iGuildNum, char *szGuildName, int iUserIndex);
	void GCArcaBattleSendNotiMsg(BYTE btNoticeType, int iNoticeValue, char *szGuildName);
	void GCArcaBattleCurrentStatusForReConnectUser(int iUserIndex);

	int IsDataLoadWinGuild() { return this->m_bReqABWinGuildInfo; }
	int IsDataLoadProcState() { return this->m_bReqABProcState; }
	int IsInitABProcMultiCast() { return this->m_bABInitState; }

	void SetEventEnable(BOOL bEnable) { this->m_bDoEvent = bEnable; }

private: // variables
	bool m_bDoEvent;
	_tagARCA_BATTLE_BOOTY_MIX m_stBootyMix[10];
	int m_iBootyMixCnt;
	_tagArcaBattleUserInfo m_UserData[300];
	_tagOBELISK_INFO m_stObeliskInfo[5];
	_tagOBELISK_STATE m_stObeliskState[5];
	_tagARCA_BATTLE_RESULT_INFO m_stArcaBattleResultInfo[5];
	_tagOccupyGuildInfo m_stOccupyGuildInfo[5];
	_stABWinGuildInfo m_stABWinGuildInfo[5];
	int m_iWinGuildCnt;
	bool m_bReqABWinGuildInfo;
	bool m_bReqABProcState;
	int m_iAB_State;
	LONGLONG m_i64_AB_REMAIN_MSEC;
	LONGLONG m_i64_AB_TICK_COUNT;
	int m_iAB_NOTIFY_COUNT;
	LONGLONG m_i64_AB_10TICK_COUNT;
	ULONGLONG m_i64_DbDataLoadTickCount;
	int m_iAB_DayOfWeekCycle[7];
	SYSTEMTIME m_tmStartDate;
	int m_iJoinGuildMemberCnt;
	ULONGLONG m_i64_AB_TICK_COUNT_2;
	ULONGLONG m_i64_AB_TICK_COUNT_SEC;
	int m_iAB_MasterRegMin;
	int m_iAB_RegTimeMin;
	int m_iAB_WaitTimeMin;
	int m_iAB_WaitPartyTimeMin;
	int m_iAB_PlayNotifyTimeMin;
	int m_iAB_PlayTimeMin;
	int m_iAB_ChannelTimeMin;
	std::vector<_tagARCA_BATTLE_START_TIME> m_vtArcaBattleOpenTime;
	int m_iCurUserCount;
	int m_iObeliskCnt;
	_stABMonPosition m_stMonsterPosition[50];
	_stABMonGroupInfo m_stMonsterGroupInfo[50];
	_stAB_GUILD_MASTER_GATE_INFO m_stGMasterGateNum[10];
	_stAB_USER_POSITION m_stAB_UserPos[5];
	_stABJoinUserInfo m_stABJoinUserInfo[200];
	int m_iABJoinUserCnt;
	int m_iSkipObelisk;
	bool m_bABInitState;
	bool m_bWindowClose;
	int m_iAuraChk;
	_tagAB_GUILD_MEMBER_INFO_LOG m_stGuildMemberInfoLog[10];
};

extern CArcaBattle g_ArcaBattle;

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

