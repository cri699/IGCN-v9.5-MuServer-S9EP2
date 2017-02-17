// RaklionUtil.h: interface for the CRaklionUtil class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RAKLIONUTIL_H__AAC761F9_5D0B_432D_827F_F2AA9F9F6331__INCLUDED_)
#define AFX_RAKLIONUTIL_H__AAC761F9_5D0B_432D_827F_F2AA9F9F6331__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "prodef.h"

typedef struct
{	
	PBMSG_HEAD2 h;
	BYTE btState;
	BYTE btDetailState;
} PMSG_ANS_RAKLION_STATE_CHANGE, *LPPMSG_ANS_RAKLION_STATE_CHANGE;

#pragma pack(1)
typedef struct
{
	PBMSG_HEAD2 h;
	BYTE btResult;
} PMSG_ANS_RAKLION_BATTLE_RESULT, *LPPMSG_ANS_RAKLION_BATTLE_RESULT;
#pragma pack()

typedef struct
{
	PBMSG_HEAD2 h;
	BYTE btCurrentState;
	BYTE btCurrentDetailState;
} PMSG_ANS_RAKLION_CURRENT_STATE, *LPPMSG_ANS_RAKLION_CURRENT_STATE;

#pragma pack(1)
typedef struct
{
	PBMSG_HEAD2 h;
	BYTE btObjClassH;
	BYTE btObjClassL;
	BYTE btType;
} PMSG_NOTIFY_RAKLION_WIDE_AREA_ATTACK, *LPPMSG_NOTIFY_RAKLION_WIDE_AREA_ATTACK;
#pragma pack()

typedef struct
{
	PBMSG_HEAD2 h;
	BYTE btMonsterCount;
	BYTE btUserCount;
} PMSG_NOTIFY_RAKLION_USER_MONSTER_COUNT, *LPPMSG_NOTIFY_RAKLION_USER_MONSTER_COUNT;

class CRaklionUtil  
{
public:
	CRaklionUtil();
	virtual ~CRaklionUtil();
	
	void NotifyRaklionChangeState(int iState, int iDetailState);
	void NotifyRaklionSuccessValue(int iSuccessValue);
	void NotifyRaklionCurrentState(int iIndex, int iState, int iDetailState);
	void NotifyRaklionWideAreaAttack(int iIndex, int iTargetIndex, int iSkillType);
	void NotifyRaklionUserMonsterCount(int iMonsterCount, int iUserCount);
	void NotifyLeftTopMsgRaklionBattleUser(char* lpszMsg, ...);
	void SendMsgRaklionBossMapUser(char* lpszMsg, ...);
	void SendDataRaklionBossMapUser(BYTE* lpMsg, int iSize);
	void SendMsgRaklionMapUser(char* lpszMsg, ...);
	void SendDataRaklionMapUser(BYTE* lpMsg, int iSize);
	void SendMsgAllUser(char* lpszMsg, ...);
	void SendDataAllUser(BYTE* lpMsg, int iSize);
	void SendMsgToUser(int iIndex, char* lpszMsg, ...);
	void SendDataToUser(int iIndex, BYTE* lpMsg, int iSize);
	void SendSystemMsgToUser(int iIndex, char* lpszMsg, ...);

};

extern CRaklionUtil	g_RaklionUtil;

#endif // !defined(AFX_RAKLIONUTIL_H__AAC761F9_5D0B_432D_827F_F2AA9F9F6331__INCLUDED_)


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

