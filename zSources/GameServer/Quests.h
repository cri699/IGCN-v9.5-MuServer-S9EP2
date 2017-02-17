//////////////////////////////////////////////////////////////////////
// Quests.h
#include "user.h"

#define MAX_QUEST_INDEX		21

enum
{
	TUTORIAL_NOTSTARTED = 0,
	TUTORIAL_FINISHED = 1,
	TUTORIAL_ONGOING = 2
};

struct REQUEST_DATA
{
	int RequirementType;
	short RequirementID;
	int RequiredCount;
	bool DeleteItem;
};

struct REWARD_DATA
{
	 int RewardType;
	 short RewardID;
	 int RewardCount;
	 int RewardLevel;
	 int RewardSkill;
	 int RewardLuck;
	 int RewardOption;
	 int RewardExc;
	 int RewardSet;
};

struct DAILYQUEST_DATA
{
	BYTE btINDEX;
	BYTE btSTATE;
	BYTE btDAY;
	BYTE btREQUESTLEVEL;
};

struct QUESTITEM_DATA
{
	BYTE btINDEX;
	BYTE btSTATE;
	WORD wMonsterID;
	WORD wItemID;
	DWORD dwRate;
	BYTE btItemCount;
};

struct QUEST_INTERNAL_DATA
{
	BYTE btGUID;
	BYTE btINDEX;
	BYTE btSTATE;
	BYTE btDIALOG;
	BYTE btANSFIRST;
	BYTE btANSSECOND;
	BYTE btANSTHIRD;
	WORD wMINLEVEL;
	WORD wMAXLEVEL;
	BYTE btCLASS[7];
	WORD wNPCID;
	BYTE btRESETID;
	BYTE btSTAGE;
	BYTE btREQSTAGE;
	BYTE btEVENTLEVEL;
	BYTE btRANDOMREWARD;
	BYTE btDIALOGCLASS[7];
	REQUEST_DATA wREQUEST[5];
	REWARD_DATA wREWARD[5];
};

struct PMSG_REQ_QUESTINFO
{
	PBMSG_HEAD2 h;
	WORD QuestState;
	WORD QuestIndex;
};

struct PMSG_REQ_QUESTANSWER
{
	PBMSG_HEAD2 h;
	WORD QuestState;
	WORD QuestIndex;
	BYTE Answer;
};

struct PMSG_QUEST_INIT
{
	PBMSG_HEAD2 h;
};

struct PMSG_REQ_QUEST_DETAIL
{
	PBMSG_HEAD2 h;
	WORD QuestState;
	WORD QuestIndex;
};

struct PMSG_REQ_QUESTEND
{
	PBMSG_HEAD2 h;
	WORD QuestState;
	WORD QuestIndex;
};

struct PMSG_ANS_QUESTEND
{
	PBMSG_HEAD2 h;
	WORD QuestState;
	WORD QuestIndex;
	BYTE btResult;
};

struct PMSG_QUESTDELETE
{
	PBMSG_HEAD2 h;
	WORD QuestState;
	WORD QuestIndex;
};

struct PMSG_QUESTNOTIFY
{
	PBMSG_HEAD2 h;
	WORD QuestState;
	WORD QuestIndex;
	WORD QuestState2;
	WORD QuestIndex2;
};

struct PMSG_REQ_QUESTINTERFACEUSE
{
	PBMSG_HEAD2 h;
	WORD QuestState;
	WORD QuestIndex;
};

struct PMSG_REQ_QUESTITEMUSE
{
	PBMSG_HEAD2 h;
	WORD Unk;
	WORD wResult;
	WORD QuestState;
	WORD QuestIndex;
};

struct PMSG_QUESTLISTSENDCHAR{
	BYTE indexL;
	BYTE indexH;
	BYTE typeL;
	BYTE typeH;
};

#pragma pack (1)
struct REQUIREMENT_INFO
{
	 int RequirementType;
	 short RequirementID;
	 int RequiredCount;
	 int Count;
	 BYTE RequirementItemInfo[12];
};

struct REWARD_INFO
{
	 int RewardType;
	 short RewardID;
	 int RewardCount;
	 BYTE RewardItemInfo[12];
};

struct PMSG_QUEST_DETAIL // C1:F6:0C (NPC) || C1:F6:1B (T MENU)
{
	 PBMSG_HEAD2 h;
	 short QuestID;
	 short QuestGroup;
	 unsigned char NeedsCount;
	 unsigned char PrizeCount;
	 unsigned char RandRewardCount;
	 REQUIREMENT_INFO Needs[5];
	 REWARD_INFO Prizes[5];
};

#pragma pack ()

#pragma once
class CQuests
{
public:
	CQuests(void);
	virtual ~CQuests(void);

	void							ConvertByteToData(LPOBJ lpObj, unsigned char* buf);
	void							ConvertDataToByte(LPOBJ lpObj, unsigned char* buf);
	void							LoadData(LPSTR lpszFile);
	void							CGReqQuestList(int aIndex);
	void							CGReqAnswer(int aIndex, PMSG_REQ_QUESTINFO *aRecv);
	void							CGReqDialog(int aIndex, PMSG_REQ_QUESTANSWER *aRecv);
	void							CGReqQuestListLogin(int aIndex);
	void							CGReqQuestFinish(int aIndex, PMSG_REQ_QUESTEND * aRecv);
	void							CGReqQuestDelete(int aIndex, PMSG_QUESTDELETE *aRecv);
	void							CGReqQuestInterfaceUse(int aIndex, PMSG_REQ_QUESTINTERFACEUSE *aRecv);
	void							GCInitQuest(int aIndex);
	void							CGReqQuestSpecificInfo(int aIndex, PMSG_REQ_QUEST_DETAIL *aRecv);
	void							StartTutorial(int aIndex);
	void							KillMonsterProc(LPOBJ lpMonsterObj);
	void							CGReqShadowPhantomBuff(int aIndex);
	bool							GetDailyQuest(int aIndex, BYTE btLevel);
	void							KillGensProc(LPOBJ lpObj, LPOBJ lpTargetObj);
	void							FinishEventProc(int aIndex, BYTE btEventType, BYTE btEventLevel);
	void							DestroyBloodCastleGate(int aIndex, BYTE btEventLevel);
	void							ChaosCastleKillProc(int aIndex, BYTE btEventLevel, BYTE btType);
	void							AddDevilSquarePoint(int aIndex, BYTE btEventLevel, BYTE btPoint);
	bool							DropQuestItem(LPOBJ lpMonsterObj);
	bool							DropQuestItemParty(LPOBJ lpObj, LPOBJ lpMonsterObj);

private:

	QUEST_INTERNAL_DATA				m_Quests[MAX_QUEST_INDEX][255];
	DAILYQUEST_DATA					m_DailyQuest[30];
	QUESTITEM_DATA					m_QuestItems[30];
	QUEST_INTERNAL_DATA *			GetQuestData(WORD State, WORD Index);
	bool							CheckRequirements(LPOBJ lpObj, QUEST_INTERNAL_DATA * lpQuest);
	bool							AddQuest(LPOBJ lpObj, bool Overwrite, WORD Index, WORD State);
	bool							DelQuest(LPOBJ lpObj, WORD Index, WORD State);
	int								FindQuestArrayNum(LPOBJ lpObj, WORD Index, WORD State);
	int								FindQuestArrayNum(LPOBJ lpObj, WORD Index);
	QUEST_INTERNAL_DATA *			FindDailyQuest(LPOBJ lpObj, BYTE btDay, BYTE btReqLevel);
	bool							IsDailyQuest(WORD Index, WORD State, BYTE & btLevel);
	int								LevelUp(LPOBJ lpObj, DWORD Experience);
	bool							CreateItem(LPOBJ lpObj, QUEST_INTERNAL_DATA * lpQuest, int RewardNum);
	bool							CreateRandomItem(LPOBJ lpObj, QUEST_INTERNAL_DATA * lpQuest);
	int								GetItemCount(LPOBJ lpObj, int ItemNumber);
	int								GetRequiredCount(LPOBJ lpObj, int ReqID, QUEST_INTERNAL_DATA * lpQuest, int ArrayNum);
	bool							CheckGens(LPOBJ lpObj, WORD NpcID);
	UINT64							CalcExperience(UINT64 i64Experience, LPOBJ lpObj);
	int								CalcMoney(int Money);
	void							GCAnsQuestSpecificInfo(LPOBJ lpObj, QUEST_INTERNAL_DATA * lpQuest);
	void							GCNotifyQuest(int aIndex, WORD Index, WORD State);
	void							GiveReward(LPOBJ lpObj, QUEST_INTERNAL_DATA * lpQuest);
	void							DeleteItem(LPOBJ lpObj, QUEST_INTERNAL_DATA * lpQuest, int RewardNum);
	bool							CheckNPC(LPOBJ lpObj, WORD NPCID);
	void							SetEventQuest(LPOBJ lpObj);
	void							SetQuestStage(LPOBJ lpObj, QUEST_INTERNAL_DATA *lpQuest);
	bool							CheckStage(LPOBJ lpObj, WORD Index, WORD State);
	bool							CheckLevel(LPOBJ lpObj, WORD MinLevel, WORD MaxLevel);
};

extern CQuests g_Quests;


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

