//////////////////////////////////////////////////////////////////////
// Engine.h
#ifndef ENGINE_H
#define ENGINE_H

#include "IOCP.h"

#define MAX_USER		1500

struct User
{
	int Index;
	int ConnectionState;
	char IP[20];
	unsigned int Port;
	unsigned char Type;
	SOCKET socket;
	_PER_SOCKET_CONTEXT * PerSocketContext;
	bool News;
	int PacketCount;
	ULONGLONG i64PacketTime;
};

void UserInit();
short UserAdd(SOCKET s, char* IP);
short UserDelete(int index);
bool UserSet(int index, unsigned int Port, int type, char* Name);
void UserRun();

extern User Users[MAX_USER];

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

