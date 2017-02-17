//////////////////////////////////////////////////////////////////////
// GensSystem.h
#ifndef GENSSYSTEM_H
#define GENSSYSTEM_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GensSystemProtocol.h"
#include "MoveCommand.h"

#define MAX_GENS_CLASS 14

struct BATTLEZONE_MAP_DATA
{
	WORD m_wMapNumber;
	BYTE m_btEntryAllowType;
	float m_fExpBonus[MAX_INFLUENCE_TYPE - 1];
	float m_fMLExpBonus[MAX_INFLUENCE_TYPE - 1];
	int m_iDropBonus[MAX_INFLUENCE_TYPE - 1];
	int m_iMLDropBonus[MAX_INFLUENCE_TYPE - 1];
	int m_iExcDropBonus[MAX_INFLUENCE_TYPE - 1];
	int m_iMLExcDropBonus[MAX_INFLUENCE_TYPE - 1];
};

class GensSystem
{
public:
	GensSystem();
	virtual ~GensSystem();

	int GDReqAbusingInfo(LPOBJ lpObj); 
	int ReqExDBGensInfo(LPOBJ lpObj);
	void ReqRegGensMember(LPOBJ lpObj, BYTE btInfluence);
	int AnsRegGensMember(int iObjIndex, BYTE btResult);
	int ReqSecedeGensMember(LPOBJ lpObj);
	int AnsSecedeGensMember(int iObjIndex, BYTE btResult);

	bool IsInfluenceNPC(LPOBJ lpObj);

	int SendGensInfo(LPOBJ lpObj);
	int SendPkPenaltyMapMove(LPOBJ lpObj);
	int GDReqSaveContributePoint(LPOBJ lpObj);
	int DBSaveAbusingKillUserName(LPOBJ lpObj);
	void BattleZoneChatMsgSend(LPOBJ lpObj, LPBYTE Msg, int size);
	int IsMoveMapBattleZone(int iMapIndex);
	void GensViewportListProtocol(LPOBJ lpObj);
	int IsMapBattleZone(int iMapIndex);
	void SetGensInfluence(LPOBJ lpObj, int iInfluence);
	int IsPkEnable(LPOBJ lpObj, LPOBJ lpTargetObj);
	int GetGensInfluence(LPOBJ lpObj);
	char *GetGensInfluenceName(LPOBJ lpObj);
	bool IsRegGensInfluence(LPOBJ lpObj);
	int SetUserBattleZoneEnable(LPOBJ lpObj, int bBattleZoneEnable);
	int IsUserBattleZoneEnable(LPOBJ lpObj);
	int SetContributePoint(LPOBJ lpObj, int iContributePoint);
	int AddContributePoint(LPOBJ lpObj, int iContributePoint);
	int SubContributePoint(LPOBJ lpObj, int iContributePoint);
	int GetContributePoint(LPOBJ lpObj);
	int PkPenaltyAddNeedZenMapMove(LPOBJ lpObj);
	void PkPenaltyDropInvenItem(LPOBJ lpObj);
	void PkPenaltyDropZen(LPOBJ lpObj);
	void SendPKPenaltyDebuff(LPOBJ lpObj);
	void CalcContributePoint(LPOBJ lpObj, LPOBJ lpTargetObj);
	int ChkKillUserName(LPOBJ lpObj, char *szCharName);
	int AbusingPenalty(LPOBJ lpObj, int iKillUserIndex);
	int InsertKillUserName(LPOBJ lpObj, char *szCharName);
	void MoveInBattleZonePartySplit(LPOBJ lpObj);
	void AbusingInfoReset(LPOBJ lpObj);
	int CalGensClass(LPOBJ lpObj);
	int SetGensRanking(LPOBJ lpObj, int iGensRanking);
	int SetGensClass(LPOBJ lpObj, int iGensClass);
	int GetGensClass(LPOBJ lpObj);
	int GetNextContributePoint(LPOBJ lpObj);
	int SetGensMemberCount(int iInfluence, int iMemberCount);
	int GetGensMemberCount(int iInfluence);
	int ReqExDBGensRewardCheck(LPOBJ lpObj, int iInfluence);
	int ReqExDBGensRewardComplete(LPOBJ lpObj);
	int SendGensReward(LPOBJ lpObj, BYTE btResult);
	int SendGensRewardItem(LPOBJ lpObj, int iGensClass);
	bool GensRewardInventoryCheck(LPOBJ lpObj, int iItemCount);
	int ReqExDBGensMemberCount(LPOBJ lpObj);
	int ReqExDBSetGensRewardDay();
	int ReqExDBSetGensRanking();
	int LoadData(char *lpszFileName);
	int ReqGensRewardDay(LPOBJ lpObj);

	int GetPKPartyLevel(LPOBJ lpObj);

	void SendBattleZoneData(LPOBJ lpObj);

	bool CanGensSeeOppositeChat() { return this->m_bGensCanSeeOppositeGensChat; }
	bool CanGensJoinPartyWhileOppositeGens() { return this->m_bGensCanJoinPartyWhileOppositeGens; }
	bool CanGensJoinGuildWhileOppositeGens() { return this->m_bGensCanJoinGuildWhileOppositeGens; }
	bool CanGensJoinUnionWhileOppositeGens() { return this->m_bGensCanJoinUnionWhileOppositeGens; }
	bool CanGensCreatePartyOnBattleZone() { return this->m_bGensCanCreatePartyOnBattleZone; }

	bool IsGensMuBlueMode() { return this->m_bGensMapFromMuBlue; }

	int GetInitialContributePoint() { return this->m_iGensInitialContributePoint; }

	int GetAbusingResetTime() { return this->m_iGensAbusingResetTime; }
	int GetGensAttackWarpDelay() { return this->m_iGensAttackWarpDelay; }

	int GetDamageReduction() { return this->m_iGensDamageReduction; }

	float GetBattleZoneExpBonus(LPOBJ lpObj);
	int GetBattleZoneDropBonus(LPOBJ lpObj);
	int GetBattleZoneExcDropBonus(LPOBJ lpObj);

	bool GetEntryAllowType(LPOBJ lpObj, BYTE btMapNumber);

private:
	BOOL								m_bPkPenaltyDropZen;
	BOOL								m_bPkPenaltyDropInvenItem;									
	
	int									m_iGensAbusingPenaltyWarning;
	int									m_iGensAbusingPenaltyLimit;
	int									m_iGensAbusingResetTime;
	int									m_iGensInitialContributePoint;
	int									m_iGensPkPenaltyAddNeedZenMapMove;

	int									m_iGensDamageReduction;
										
	bool								m_bGensCanSeeOppositeGensChat;
	bool								m_bGensCanJoinPartyWhileOppositeGens;
	bool								m_bGensCanJoinGuildWhileOppositeGens;
	bool								m_bGensCanJoinUnionWhileOppositeGens;
	bool								m_bGensCanCreatePartyOnBattleZone;
	bool								m_bGensMapFromMuBlue;		
										
	int									m_iVanertMemberCount;
										
	int									m_iDuprianMemberCount;
										
	bool								m_bFileLoad;

	DWORD								m_iGensAttackWarpDelay; // custom for joinmu

	std::map<int, BATTLEZONE_MAP_DATA>	m_mapBattleZoneData;
	int									m_wBattleZoneWarp[MAX_MOVE_COMMAND];

	CRITICAL_SECTION					criti;
	GENS_RANKING_DATA					m_GensRanking[MAX_GENS_CLASS];
};

extern GensSystem g_GensSystem;

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

