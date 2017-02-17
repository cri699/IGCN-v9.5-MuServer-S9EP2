//////////////////////////////////////////////////////////////////////
// Engine.cpp
#include "stdafx.h"
#include "Engine.h"
#include "TLog.h"
#include "ConnectServer.h"
#include "protocol.h"

User Users[MAX_USER];

int SCount=0;

void UserInit()
{
	for (int n = 0; n < MAX_USER; n++)
	{
		memset(&Users[n], 0, sizeof(User));
		Users[n].PerSocketContext = new _PER_SOCKET_CONTEXT;
	}

	SCount = 0;
}

int iReturnIpCount(char *IP)
{
	int count = 0;

	for (int i = 0; i < MAX_USER; i++)
	{
		if (!strcmp(IP, Users[i].IP))
		{
			count++;
		}
	}

	return count;

}

short UserAdd(SOCKET s, char *IP)
{
	int count = SCount;
	int totalcount = 0;

	while (true)
	{
		if (iReturnIpCount(IP) > g_MaxConnectionsPerIP && strcmpi(IP, g_WhiteListIP) != 0)
		{
			g_Log.AddC(TColor::Red, "IP: [%s] Reached Maximum Allowed Connections", IP);
			return -1;
		}

		if (count < MAX_USER && Users[count].ConnectionState == 0)
		{
			Users[count].ConnectionState = 1;
			strcpy(Users[count].IP, IP);
			Users[count].socket = s;
			Users[count].News = false;
			Users[count].PacketCount = 0;
			Users[count].i64PacketTime = GetTickCount64();
			SCount++;
			g_Log.Add("Connection Accept: %s", IP);
			return (short)count;
		}

		count++;

		if (count >= MAX_USER)
		{
			count = 0;
		}

		totalcount++;

		if (totalcount >= MAX_USER)
		{
			break;
		}
	}

	return -1;
}

short UserDelete(int index)
{
	if (Users[index].ConnectionState < 1)
	{
		return -1;
	}

	Users[index].ConnectionState = 0;
	SCount--;

	g_Log.Add("Disconnected: %s (%d)", Users[index].IP, index);

	return 1;
}

bool UserSet(int index, unsigned int Port, int type, char* Name)
{
	if (Users[index].ConnectionState < 1)
	{
		return FALSE;
	}

	if (Users[index].ConnectionState == 2)
	{
		return FALSE;
	}

	Users[index].ConnectionState = 2;
	Users[index].Port = Port;
	Users[index].Type = type;

	return TRUE;
}

void UserRun()
{
	for (int i = 0; i < MAX_USER; i++)
	{
		if (Users[i].ConnectionState != 0)
		{
			if (GetTickCount64() - Users[i].ConnectionState >= 1000)
			{
				Users[i].PacketCount = 0;
				Users[i].i64PacketTime = GetTickCount64();
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

