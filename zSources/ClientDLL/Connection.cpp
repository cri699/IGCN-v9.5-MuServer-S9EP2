//////////////////////////////////////////////////////////////////////
// Connection.cpp
#include "stdafx.h"
#include "Connection.h"
#include "ServerInfo.h"
#include "protocol.h"

tmuConnectToCS muConnectToCS = (tmuConnectToCS)MU_CONNECT_FUNC;
CONNECTION_TYPE g_Connection = NOT_CONNECTED;

void OnConnect()
{
	char * ip = g_ServerInfo->GetIp();
	//std::string deb = std::string(ip);
	muConnectToCS(ip, g_ServerInfo->GetPort());
	g_Connection = CS_CONNECTED;
}

void OnGsConnect()
{
	g_Connection = GS_CONNECTED;
}


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

