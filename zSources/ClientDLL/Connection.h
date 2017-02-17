//////////////////////////////////////////////////////////////////////
// Connection.h
#ifndef CONNECTION_H
#define CONNECTION_H

#define MU_CONNECT_FUNC 0x0063CEDA // S9
#define CONNECT_HOOK1 0x004DEA94 // S9
#define CONNECT_HOOK2 0x0043EA60 // S9
#define GSCONNECT_HOOK1 0x09E568EF // S9
#define GSCONNECT_HOOK2 0x004FAB84 // S9
#define CHATSERVER_PORT 0x004BAC22+1 // S9

enum CONNECTION_TYPE
{
	NOT_CONNECTED = 0,
	CS_CONNECTED,
	GS_CONNECTED,
};

typedef void (*tmuConnectToCS)(char* hostname, int port);
extern tmuConnectToCS muConnectToCS;

void OnConnect();
void OnGsConnect();

extern CONNECTION_TYPE g_Connection;
#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

