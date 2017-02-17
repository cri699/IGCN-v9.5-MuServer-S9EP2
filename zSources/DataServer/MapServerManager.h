//////////////////////////////////////////////////////////////////////
// MapServerManager.h
#ifndef MAPSERVERMANAGER_H
#define MAPSERVERMANAGER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
using namespace std;

#define MAX_MAP_GROUPS 20
#define MAX_MAP_SUBGROUPS 20
#define MAX_SVR_PER_GROUP 400

struct _MAPSVR_DATA
{
public:
	_MAPSVR_DATA();
	virtual ~_MAPSVR_DATA();
	void Clear(int iInitSetVal);

public:
	BOOL m_bIN_USE;
	BYTE m_btMAPSVR_GROUP;
	short m_sSVR_CODE;
	char m_cIPADDR[16];
	WORD m_wPORT;
	short m_sMAP_MOVE[MAX_NUMBER_MAP];	
};

class CMapServerManager  
{
public:
	CMapServerManager();
	virtual ~CMapServerManager();

	BOOL LoadMapData(char* lpszFileName);
	BOOL GetSvrCodeData(WORD wServerCode, char* lpszIpAddress, WORD* lpwPort);
	BOOL GetMapSvrGroup(WORD wServerCode);

public:
	BOOL	m_bMapDataLoadOk;
	_MAPSVR_DATA m_MAPSVR_DATA[20][20];
	int m_iMAPSVR_GROUP_COUNT[20];
	std::map<int,_MAPSVR_DATA *> m_mapSVRCODE_MAP;
	CRITICAL_SECTION m_critSVRCODE_MAP;
	_MAPSVR_DATA* m_lpThisMapSvrData;
};

extern CMapServerManager	g_MapServerManager;

#endif


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

