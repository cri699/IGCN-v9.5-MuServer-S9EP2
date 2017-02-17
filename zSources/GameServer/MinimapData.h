//////////////////////////////////////////////////////////////////////
// MinimapData.h
#ifndef MINIMAPDATA_H
#define MINIMAPDATA_H

#include "winutil.h"
#include "PartyClass.h"
#include "user.h"

enum MAPOBJECT_TYPE
{
	MAP_OBJECT_PORTAL = 0,
	MAP_OBJECT_NPC = 1
};

enum MAPOBJECT_NPC_TYPE
{
	MAP_OBJECT_NPC_ELFBUFFER = 1,
	MAP_OBJECT_NPC_QUEST_EVENT = 2,
	MAP_OBJECT_NPC_REPAIR = 3,
	MAP_OBJECT_NPC_POTION = 4,
	MAP_OBJECT_NPC_VAULT = 5
};

struct MINIMAP_PARTY_USER
{
	char szName[MAX_ACCOUNT_LEN+1];
	BYTE btMapNumber;
	BYTE btX;
	BYTE btY;
};

struct PMSG_MINIMAP_PARTY_USER
{
	PBMSG_HEAD2 h;
	BYTE btCount;
	MINIMAP_PARTY_USER m_User[MAX_USER_IN_PARTY];
};

#pragma pack (1)
struct PMSG_MINIMAP_DATA
{
	PBMSG_HEAD2 h;
	BYTE btID;
	BYTE btObjectType;
	BYTE btObjectID;
	BYTE btIsDisabled;
	BYTE btX;
	BYTE btY;
	char szName[31];
};

#pragma pack ()

struct MINIMAP_DATA
{
	BYTE btMapNumber;
	BYTE btID;
	BYTE btObjectType;
	BYTE btObjectID;
	BYTE btIsDisabled;
	BYTE btX;
	BYTE btY;
	char szName[31];
};

class CMinimapData
{
public:
	CMinimapData(void);
	virtual ~CMinimapData(void);

	bool LoadFile(char * szFile);
	void SendData(int aIndex);
	void SendPartyData(int party_number);

private:
	bool m_bFileLoad;
	std::vector<MINIMAP_DATA> m_vtMinimapData;
};

extern CMinimapData g_MinimapData;

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

