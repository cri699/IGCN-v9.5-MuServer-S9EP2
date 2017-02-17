//////////////////////////////////////////////////////////////////////
// SendNPCInfo.h
#ifndef SENDNPCINFO_H
#define SENDNPCINFO_H

#include "winutil.h"

struct _stSendNPCInfo
{
	int nNpcClass;
	int nIdentNo;
	BYTE btTag;
	BYTE btSync;
	BYTE btMapNumber;
	BYTE btPosX;
	BYTE btPosY;
	char szName[31];
};

struct _stSendPortalInfo
{
	int nIdentNo;
	BYTE btPosX;
	BYTE btPosY;
	BYTE btMapNumber;
	char szName[31];
};

#pragma pack (1)
struct PMSG_ANS_NPC_POSITION
{
	PBMSG_HEAD2 h;
	BYTE btIdentNo;
	BYTE btIsNpc;
	BYTE btTag;
	BYTE btType;
	BYTE btPosX;
	BYTE btPosY;
	char szName[31];
};

struct PMSG_ANS_ITL_NPC_POSITION
{
	PBMSG_HEAD2 h;
	BYTE btIdentNo;
	BYTE btIsNpc;
	BYTE btTag;
	BYTE btType;
	BYTE btPosX;
	BYTE btPosY;
};

#pragma pack ()

class CSendNPCInfo
{
public:
	CSendNPCInfo(void);
	virtual ~CSendNPCInfo(void);

	void LoadScript(char *lpFileName);
	void SendNpcCoordinate(int nDestUserIndex, BYTE btMapIndex, BYTE btAddType);
	void SendPortalCoordinateMoveNpc(BYTE btBeforeMapNumber, BYTE btAfterMapNumber, BYTE btBeforeX, BYTE btBeforeY, BYTE btX, BYTE btY, int nNpcClass);
	void SendPortalCoordinate(int nDestUserIndex, BYTE btMapNumber);
	void Find_ITL_Npc(BYTE byMapNumber, WORD wNpcType, BYTE & byMapTagIndex);
	void SendITLNpcCoordinate(int nDestUserIndex, BYTE btMapIndex);

private:
	bool								m_bLoadScript;
	std::vector<_stSendNPCInfo>			m_vtSendNpcInfo;
	std::vector<_stSendPortalInfo>		m_vtSendPortalInfo;
};

extern CSendNPCInfo g_SendNPCInfo;

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

