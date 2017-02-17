//////////////////////////////////////////////////////////////////////
// MapServerManager.cpp
 #include "stdafx.h"
#include "MapServerManager.h"
#include "MapClass.h"
#include "GameMain.h"
#include "TLog.h"
#include "user.h"
#include "configread.h"

// GS-N 0.99.60T 0x0051BF30
//	GS-N	1.00.18	JPN	0x0054D8C0	-	Completed

_MAPSVR_DATA::_MAPSVR_DATA()
{
	this->Clear(1);
}

_MAPSVR_DATA::~_MAPSVR_DATA()
{
	return;
}

void _MAPSVR_DATA::Clear(int iInitSetVal)
{
	this->m_bIN_USE = FALSE;
	this->m_btMAPSVR_GROUP = -1;
	this->m_sSVR_CODE = -1;
	memset(this->m_cIPADDR, 0, sizeof(this->m_cIPADDR));
	this->m_wPORT = 0;

	for ( int iMAP_COUNT =0;iMAP_COUNT<MAX_NUMBER_MAP;iMAP_COUNT++)
	{
		switch ( iInitSetVal )
		{
			case -1:
				this->m_sMAP_MOVE[iMAP_COUNT] = -2;
				break;
			case 0:
				this->m_sMAP_MOVE[iMAP_COUNT] = -1;
				break;
			default:
				this->m_sMAP_MOVE[iMAP_COUNT] = -3;
		}
	}
}



CMapServerManager::CMapServerManager()
{
	srand(time(NULL)); 
	this->m_bMapDataLoadOk=FALSE;
	this->m_lpThisMapSvrData = NULL;

	InitializeCriticalSection(&this->m_critSVRCODE_MAP);
}

CMapServerManager::~CMapServerManager()
{
	DeleteCriticalSection(&this->m_critSVRCODE_MAP);
}

void CMapServerManager::Clear()
{
	this->m_bMapDataLoadOk = FALSE;

	for ( int iGROUP_COUNT=0;iGROUP_COUNT<MAX_MAP_GROUPS;iGROUP_COUNT++)
	{
		this->m_iMAPSVR_GROUP_COUNT[iGROUP_COUNT] = 0;

		for ( int iSUB_GROUP_COUNT=0;iSUB_GROUP_COUNT<MAX_MAP_SUBGROUPS;iSUB_GROUP_COUNT++)
		{
			this->m_MAPSVR_DATA[iGROUP_COUNT][iSUB_GROUP_COUNT].Clear(1);
		}
	}

	this->m_mapSVRCODE_MAP.clear();
}

BOOL CMapServerManager::LoadData(char* lpszFileName)
{
	if ( (lpszFileName == NULL) || ( strcmp(lpszFileName, "") == 0 ) ) 
	{
		g_Log.MsgBox("File not found %s", lpszFileName);
		ExitProcess(1);
	}

	EnterCriticalSection(&this->m_critSVRCODE_MAP);

	this->Clear();

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(lpszFileName);

	if (res.status != pugi::status_ok)
	{
		g_Log.MsgBox("File not found %s", lpszFileName);
		ExitProcess(1);	
	}

	pugi::xml_node mapserver = file.child("MapServer");
	pugi::xml_node serverinfo = mapserver.child("ServerInfo");
	pugi::xml_node serverlist = mapserver.child("ServerList");
	pugi::xml_node servermapping = mapserver.child("ServerMapping");
	char *cvstr;
	char szTemp[128];

	strcpy(szGameServerExeSerial, serverinfo.attribute("Serial").as_string());
	strcpy(szTemp, serverinfo.attribute("Version").as_string());

	cvstr = strtok(szTemp, ".");
	szClientVersion[0] = cvstr[0];
	cvstr = strtok(NULL, ".");
	szClientVersion[1] = cvstr[0];
	szClientVersion[2] = cvstr[1];
	cvstr = strtok(NULL, ".");
	szClientVersion[3] = cvstr[0];
	szClientVersion[4] = cvstr[1];

	for (pugi::xml_node server = serverlist.child("Server"); server; server = server.next_sibling())
	{
		char szIpAddr[16] = { 0 };

		short sSVR_CODE = server.attribute("Code").as_int();
		short sMAPSVR_GROUP = server.attribute("Group").as_int();
		int iInitSetVal = server.attribute("Initiation").as_int();

		memcpy(szIpAddr, server.attribute("IP").as_string(), 16);
		szIpAddr[15] = 0;

		WORD wPortNum = server.attribute("Port").as_int();

		if (sSVR_CODE < 0)
		{
			g_Log.MsgBox("[MapServerMng] CMapServerManager::LoadData() - file load error : sSVR_CODE < 0 (SVR:%d) - 1",
				sSVR_CODE);

			return FALSE;
		}

		if (iInitSetVal != -1 && iInitSetVal != 0 && iInitSetVal != 1)
		{
			g_Log.MsgBox("[MapServerMng] CMapServerManager::LoadData() - file load error : iInitSetting Value:%d (SVR:%d) - 1",
				iInitSetVal, sSVR_CODE);

			return FALSE;
		}

		if (!strcmp(szIpAddr, ""))
		{
			g_Log.MsgBox("[MapServerMng] CMapServerManager::LoadData() - file load error : No IpAddress (SVR:%d)",
				sSVR_CODE);

			return FALSE;
		}

		if (CHECK_LIMIT(sMAPSVR_GROUP, MAX_MAP_GROUPS) == FALSE)
		{
			g_Log.MsgBox("[MapServerMng] CMapServerManager::LoadData() - file load error : Map Server Group Index (IDX:%d)",
				sMAPSVR_GROUP);

			return FALSE;
		}

		if (this->m_iMAPSVR_GROUP_COUNT[sMAPSVR_GROUP] >= MAX_MAP_SUBGROUPS)
		{
			g_Log.MsgBox("[MapServerMng] CMapServerManager::LoadData() - file load error : No Space to Save SvrInfo (SVR:%d)",
				sSVR_CODE);

			return FALSE;
		}

		_MAPSVR_DATA * lpMapSvrData = NULL;

		lpMapSvrData = &this->m_MAPSVR_DATA[sMAPSVR_GROUP][this->m_iMAPSVR_GROUP_COUNT[sMAPSVR_GROUP]];

		lpMapSvrData->Clear(iInitSetVal);
		lpMapSvrData->m_bIN_USE = TRUE;
		lpMapSvrData->m_sSVR_CODE = sSVR_CODE;
		lpMapSvrData->m_btMAPSVR_GROUP = sMAPSVR_GROUP;
		lpMapSvrData->m_wPORT = wPortNum;
		memcpy(lpMapSvrData->m_cIPADDR, szIpAddr, 16);
		lpMapSvrData->m_cIPADDR[15] = 0;

		this->m_mapSVRCODE_MAP.insert(std::pair<int, _MAPSVR_DATA *>(sSVR_CODE, lpMapSvrData));
		this->m_iMAPSVR_GROUP_COUNT[sMAPSVR_GROUP]++;
	}

	for (pugi::xml_node server = servermapping.child("Server"); server; server = server.next_sibling())
	{
		_MAPSVR_DATA * lpMapSvrData = NULL;

		short sSVR_CODE = server.attribute("Code").as_int();
		BYTE btNotMoveOption = server.attribute("MoveAble").as_int();
		WORD wMapNum = server.attribute("MapNumber").as_int();
		short sDEST_SVR_CODE = server.attribute("DestServerCode").as_int();

		if (sSVR_CODE < 0)
		{
			g_Log.MsgBox("[MapServerMng] CMapServerManager::LoadData() - file load error : sSVR_CODE < 0 (SVR:%d) - 2",
				sSVR_CODE);

			return FALSE;
		}

		if (sDEST_SVR_CODE < -2)
		{
			g_Log.MsgBox("[MapServerMng] CMapServerManager::LoadData() - file load error : sDEST_SVR_CODE < -1 (SVR:%d, DEST_SVR:%d) - 2",
				sSVR_CODE, sDEST_SVR_CODE);

			return FALSE;
		}

		std::map<int, _MAPSVR_DATA *>::iterator it = this->m_mapSVRCODE_MAP.find(sSVR_CODE);

		if (it == this->m_mapSVRCODE_MAP.end())
		{
			g_Log.MsgBox("[MapServerMng] CMapServerManager::LoadData() - file load error : sSVR_CODE wasn't registered (SVR:%d)",
				sSVR_CODE);

			return FALSE;
		}

		lpMapSvrData = it->second;

		if (lpMapSvrData == NULL)
		{
			g_Log.MsgBox("[MapServerMng] CMapServerManager::LoadData() - file load error : lpMapSvrData == NULL (SVR:%d)",
				sSVR_CODE);

			return FALSE;
		}

		if (lpMapSvrData->m_bIN_USE == FALSE)
		{
			g_Log.MsgBox("[MapServerMng] CMapServerManager::LoadData() - file load error : lpMapSvrData->m_bIN_USE == FALSE (SVR:%d)",
				sSVR_CODE);

			return FALSE;
		}

		if (lpMapSvrData->m_sSVR_CODE != sSVR_CODE)
		{
			g_Log.MsgBox("[MapServerMng] CMapServerManager::LoadData() - file load error : lpMapSvrData->m_sSVR_CODE != sSVR_CODE (SVR:%d)",
				sSVR_CODE);

			return FALSE;
		}

		if (btNotMoveOption != 0 && btNotMoveOption != 1)
		{
			g_Log.MsgBox("[MapServerMng] CMapServerManager::LoadData() - file load error : lpMapSvrData->m_sSVR_CODE != sSVR_CODE (SVR:%d, OPT:%d)",
				sSVR_CODE, btNotMoveOption);

			return FALSE;
		}

		if (CHECK_LIMIT(wMapNum, MAX_NUMBER_MAP) == FALSE)
		{
			g_Log.MsgBox("[MapServerMng] CMapServerManager::LoadData() - file load error : Map Number is out of bound (SVR:%d, MAP:%d)",
				sSVR_CODE, wMapNum);

			return FALSE;
		}

		switch (btNotMoveOption)
		{
		case 0:
			lpMapSvrData->m_sMAP_MOVE[wMapNum] = sDEST_SVR_CODE;
			break;
		case 1:
			lpMapSvrData->m_sMAP_MOVE[wMapNum] = -3;
			break;
		default:
			g_Log.MsgBox("[MapServerMng] CMapServerManager::LoadData() - file load error : lpMapSvrData->m_sSVR_CODE != sSVR_CODE (SVR:%d, OPT:%d)",
				sSVR_CODE, btNotMoveOption);
			break;
		}
	}

	std::map<int  ,_MAPSVR_DATA *>::iterator it = this->m_mapSVRCODE_MAP.find(g_ConfigRead.server.GetGameServerCode());

	if ( it != this->m_mapSVRCODE_MAP.end() )
	{
		this->m_lpThisMapSvrData = it->second;
	}

	else
	{
		this->m_lpThisMapSvrData = NULL;
	}

	if ( this->m_lpThisMapSvrData == NULL )
	{
		g_Log.MsgBox("[MapServerMng] CMapServerManager::LoadData() - file load error : This GameServerCode (%d) doesn't Exist at file '%s' != sSVR_CODE",
			g_ConfigRead.server.GetGameServerCode(), lpszFileName);

		return FALSE;
	}

	this->m_bMapDataLoadOk = TRUE;

	LeaveCriticalSection(&this->m_critSVRCODE_MAP);

	return TRUE;
}


BOOL CMapServerManager::CheckMapCanMove(int iMAP_NUM)
{
	if ( MapNumberCheck(iMAP_NUM) == 0 )
	{
		return FALSE;
	}

	_MAPSVR_DATA * lpMapSvrData = this->m_lpThisMapSvrData;

	if ( lpMapSvrData == NULL )
	{
		return FALSE;
	}

	if ( lpMapSvrData->m_bIN_USE == FALSE )
	{
		return FALSE;
	}

	short sMAP_MOVE_INFO = lpMapSvrData->m_sMAP_MOVE[iMAP_NUM];

	if ( sMAP_MOVE_INFO != -3 )
	{
		return FALSE;
	}

	return TRUE;
}



short CMapServerManager::CheckMoveMapSvr(int iIndex, int iMAP_NUM, short sSVR_CODE_BEFORE)
{
	if ( this->m_bMapDataLoadOk == FALSE )
		return g_ConfigRead.server.GetGameServerCode();

	if ( !gObjIsConnected(iIndex))
		return g_ConfigRead.server.GetGameServerCode();

	if ( !MapNumberCheck(iMAP_NUM))
	{
		g_Log.AddC(TColor::Red,  "[MapServerMng] CheckMoveMapSvr() - Map Index doesn't exist [%s][%s] : %d",
			gObj[iIndex].AccountID, gObj[iIndex].Name, iMAP_NUM);

		return g_ConfigRead.server.GetGameServerCode();
	}

	_MAPSVR_DATA * lpMapSvrData = this->m_lpThisMapSvrData;

	if ( lpMapSvrData == NULL )
	{
		g_Log.AddC(TColor::Red,  "[MapServerMng] CheckMoveMapSvr() - m_lpThisMapSvrData == NULL [%s][%s] : %d",
			gObj[iIndex].AccountID, gObj[iIndex].Name, iMAP_NUM);

		return g_ConfigRead.server.GetGameServerCode();
	}

	if ( lpMapSvrData->m_bIN_USE == FALSE )
	{
		g_Log.AddC(TColor::Red,  "[MapServerMng] CheckMoveMapSvr() - lpMapSvrData->m_bIN_USE == FALSE [%s][%s] : %d",
			gObj[iIndex].AccountID, gObj[iIndex].Name, iMAP_NUM);

		return g_ConfigRead.server.GetGameServerCode();
	}

	if (gObj[iIndex].MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL)
		iMAP_NUM = MAP_INDEX_LORENMARKET;
	if (ITL_MAP_RANGE(gObj[iIndex].MapNumber))
		iMAP_NUM = MAP_INDEX_LORENMARKET;
	if (IT_MAP_RANGE(gObj[iIndex].MapNumber))
		iMAP_NUM = MAP_INDEX_LORENMARKET;

	short sMAP_MOVE_INFO = lpMapSvrData->m_sMAP_MOVE[iMAP_NUM];

	switch ( sMAP_MOVE_INFO )
	{
		case -1:
			{
				_MAPSVR_DATA * lpDestMapSvrData = NULL;

				if ( sSVR_CODE_BEFORE != -1 )
				{
					EnterCriticalSection(&this->m_critSVRCODE_MAP);

					std::map<int, _MAPSVR_DATA *>::iterator it = this->m_mapSVRCODE_MAP.find(sSVR_CODE_BEFORE);

					if ( it != this->m_mapSVRCODE_MAP.end() )
					{
						lpDestMapSvrData = it->second;
					}

					LeaveCriticalSection(&this->m_critSVRCODE_MAP);

					if ( lpDestMapSvrData != NULL )
					{
						if ( lpDestMapSvrData->m_sMAP_MOVE[iMAP_NUM] == -3 )
						{
							return sSVR_CODE_BEFORE;
						}
					}
				}

				std::vector<_MAPSVR_DATA *> vtMapSvrData;

				EnterCriticalSection(&this->m_critSVRCODE_MAP);

				for ( std::map<int ,_MAPSVR_DATA *>::iterator it = this->m_mapSVRCODE_MAP.begin() ; it != this->m_mapSVRCODE_MAP.end() ;it++)
				{
					_MAPSVR_DATA * lpTempMapSvrData = it->second;

					if ( lpTempMapSvrData != NULL &&
						 lpTempMapSvrData->m_bIN_USE == TRUE &&
						 lpTempMapSvrData->m_sMAP_MOVE[iMAP_NUM] == -3)
					{
						vtMapSvrData.push_back(it->second);
					}
				}

				LeaveCriticalSection(&this->m_critSVRCODE_MAP);

				short sDestServerCode = -1;

				if ( vtMapSvrData.empty() == 0 )
				{
					sDestServerCode = vtMapSvrData[rand()%vtMapSvrData.size()]->m_sSVR_CODE;
				}

				if ( sDestServerCode != -1 )
				{
					g_Log.Add("[MapServerMng] CheckMoveMapSvr() - MapServer Check OK [%s][%s] : MAP-%d / SVR-%d(State Map:%d X:%d Y:%d)",
						gObj[iIndex].AccountID, gObj[iIndex].Name, iMAP_NUM, sDestServerCode,
						gObj[iIndex].MapNumber, gObj[iIndex].X, gObj[iIndex].Y);

					return sDestServerCode;
				}
			}
			break;

		case -2:
			{
				_MAPSVR_DATA * lpDestMapSvrData = NULL;

				if ( sSVR_CODE_BEFORE != -1 )
				{
					EnterCriticalSection(&this->m_critSVRCODE_MAP);

					std::map<int, _MAPSVR_DATA *>::iterator it = this->m_mapSVRCODE_MAP.find(sSVR_CODE_BEFORE);

					if ( it != this->m_mapSVRCODE_MAP.end() )
					{
						lpDestMapSvrData = it->second;
					}

					LeaveCriticalSection(&this->m_critSVRCODE_MAP);

					if ( lpDestMapSvrData != NULL )
					{
						if ( lpDestMapSvrData->m_sMAP_MOVE[iMAP_NUM] == -3 )
						{
							return sSVR_CODE_BEFORE;
						}
					}
				}
				
				short sDestServerCode = -1;

				EnterCriticalSection(&this->m_critSVRCODE_MAP);

				for ( std::map<int ,_MAPSVR_DATA *>::iterator it = this->m_mapSVRCODE_MAP.begin() ; it != this->m_mapSVRCODE_MAP.end() ;it++)
				{
					_MAPSVR_DATA * lpTempMapSvrData = it->second;

					if ( lpTempMapSvrData != NULL &&
						 lpTempMapSvrData->m_bIN_USE == TRUE &&
						 lpTempMapSvrData->m_sMAP_MOVE[iMAP_NUM] == -3)
					{
						sDestServerCode = lpTempMapSvrData->m_sSVR_CODE;
					}
				}

				LeaveCriticalSection(&this->m_critSVRCODE_MAP);

				if ( sDestServerCode != -1 )
				{
					g_Log.Add("[MapServerMng] CheckMoveMapSvr() - MapServer Check OK [%s][%s] : MAP-%d / SVR-%d(State Map:%d X:%d Y:%d)",
						gObj[iIndex].AccountID, gObj[iIndex].Name, iMAP_NUM, sDestServerCode,
						gObj[iIndex].MapNumber, gObj[iIndex].X, gObj[iIndex].Y);

					return sDestServerCode;
				}
			}
			break;

		case -3:
			g_Log.Add("[MapServerMng] CheckMoveMapSvr() - MapServer Check OK [%s][%s] : MAP-%d / SVR-%d (State Map:%d X:%d Y:%d)",
				gObj[iIndex].AccountID, gObj[iIndex].Name, iMAP_NUM, g_ConfigRead.server.GetGameServerCode(),
				gObj[iIndex].MapNumber, gObj[iIndex].X, gObj[iIndex].Y);

			return g_ConfigRead.server.GetGameServerCode();
			break;

		default:
			if ( sMAP_MOVE_INFO > -1 )
			{
				_MAPSVR_DATA * lpDestMapSvrData = NULL;

				EnterCriticalSection(&this->m_critSVRCODE_MAP);

				std::map<int, _MAPSVR_DATA *>::iterator it = this->m_mapSVRCODE_MAP.find(sMAP_MOVE_INFO);

				if ( it != this->m_mapSVRCODE_MAP.end() )
				{
					lpDestMapSvrData = it->second;
				}

				LeaveCriticalSection(&this->m_critSVRCODE_MAP);

				if ( lpDestMapSvrData != NULL &&
					 lpDestMapSvrData->m_bIN_USE == TRUE &&
					 lpDestMapSvrData->m_sMAP_MOVE[iMAP_NUM] == -3)
				{
					return sMAP_MOVE_INFO;
				}

				short sDestServerCode = -1;

				EnterCriticalSection(&this->m_critSVRCODE_MAP);

				for ( it = this->m_mapSVRCODE_MAP.begin() ; it != this->m_mapSVRCODE_MAP.end() ;it++)
				{
					_MAPSVR_DATA * lpTempMapSvrData = it->second;

					if ( lpTempMapSvrData != NULL &&
						 lpTempMapSvrData->m_bIN_USE == TRUE &&
						 lpTempMapSvrData->m_sMAP_MOVE[iMAP_NUM] == -3)
					{
						sDestServerCode = lpTempMapSvrData->m_sSVR_CODE;
					}
				}
				
				LeaveCriticalSection(&this->m_critSVRCODE_MAP);

				if ( sDestServerCode != -1 )
				{
					g_Log.Add("[MapServerMng] CheckMoveMapSvr() - MapServer Check OK [%s][%s] : MAP-%d / SVR-%d(State Map:%d X:%d Y:%d)",
						gObj[iIndex].AccountID, gObj[iIndex].Name, iMAP_NUM, sDestServerCode,
						gObj[iIndex].MapNumber, gObj[iIndex].X, gObj[iIndex].Y);

					return sDestServerCode;
				}
			}
			else
			{
				g_Log.AddC(TColor::Red,  "[MapServerMng] CheckMoveMapSvr() - Unknown MapMove Info [%s][%s] : MAP-%d / INFO-%d",
					gObj[iIndex].AccountID, gObj[iIndex].Name, iMAP_NUM, sMAP_MOVE_INFO);

				return g_ConfigRead.server.GetGameServerCode();
			}
	}

	return g_ConfigRead.server.GetGameServerCode();
}



BOOL CMapServerManager::GetSvrCodeData(WORD wServerCode, char* lpszIpAddress, WORD * lpwPort)
{
	if ( !lpszIpAddress || !lpwPort )
		return FALSE;

	_MAPSVR_DATA * lpMapSvrData = NULL;

	EnterCriticalSection(&this->m_critSVRCODE_MAP);

	std::map<int, _MAPSVR_DATA *>::iterator it = this->m_mapSVRCODE_MAP.find(wServerCode);

	if ( it != this->m_mapSVRCODE_MAP.end() )
	{
		lpMapSvrData = it->second;
	}

	LeaveCriticalSection(&this->m_critSVRCODE_MAP);

	if ( lpMapSvrData == NULL )
		return FALSE;

	strcpy(lpszIpAddress, lpMapSvrData->m_cIPADDR);
	*lpwPort = lpMapSvrData->m_wPORT;

	return TRUE;
}



	
CMapServerManager g_MapServerManager;

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

