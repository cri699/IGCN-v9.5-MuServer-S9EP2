//////////////////////////////////////////////////////////////////////
// GMMng.h
#ifndef GMMNG_H
#define GMMNG_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_GM_COMMAND 150
#define MAX_GMCOMMAND_LEN 20
#define MAX_GAME_MASTER 5

#include "user.h"

struct COMMANDS_DATA
{
	char PostCMD[20];
	char GPostCMD[20];
	char ItemCMD[20];
	char SvInfoCMD[20];
	char CharInfoCMD[20];
	char HideCMD[20];
	char UnHideCMD[20];
	char ClearInvCMD[20];
	char AddSTRCMD[20];
	char AddAGICMD[20];
	char AddVITCMD[20];
	char ADDENECMD[20];
	char ADDCMDCMD[20];
	char ONLINECMD[20];
	char WARECMD[20];
	char PKCLEARCMD[20];
	char PKSETCMD[20];
	char PROPOSECMD[20];
	char ACCEPTCMD[20];
	char TRACEMARRYCMD[20];
	char DIVORCECMD[20];
	char SKINCMD[20];
	char BCStart[20];
	char CCStart[20];
	char DSStart[20];
	char ITStart[20];

	// CRYWOLF & CASTLE SIEGE
	char CWSetStateCMD[20];
	char CSSetOwnerCMD[20];
	char CSSetRegCMD[20];
	char CSSetMarkCMD[20];
	char CSSetNotifyCMD[20];
	char CSSetReadyCMD[20];
	char CSSetEndCMD[20];
	char CSGetOwnerCMD[20];
	char CSGetStateCMD[20];
	char CSSetStartCMD[20];
	char CSSetChangeSideCMD[20];

	// KANTURU
	char KTSetStandbyCMD[20];
	char KTSetMayaCMD[20];
	char KTSetTowerCMD[20];

	char WARCMD[20];
	char FireWorks[20];
	char Watch[20];
	char Trace[20];
	char DC[20];
	char GuildDC[20];
	char MoveCMD[20];
	char gMoveCMD[20];
	char StartSoccer[20];
	char EndSoccer[20];
	char EndWar[20];
	char ChatBan[20];
	char ChatUnban[20];
	char SetGW[20];
	char SoccerStart[20];
	char Requests[20];
	char KundunInfo[20];
	char SetKundunHP[20];

	char ResetCMD[20];

	char BanAccCMD[20];
	char UnBanAccCMD[20];

	char BanCharCMD[20];
	char UnBanCharCMD[20];

	char CrewCMD[20];

	char offTrade[20];
	char dcFriend[20];
	char setparty[20];

	char offlevel[20];

	char GremoryGiftCMD[20];

	char JoinMuChangeCMD[20]; // private
	char JoinMuUpgradeCMD[20]; // private
};

struct GM_DATA
{
	char Name[11];
	char ExpiryDate[20];
	DWORD AuthLevel;
};

class CGMCommand
{

public:

	CGMCommand() // line : 119
	{
		this->Init();
	}	// line : 122

	void Init() // line : 125
	{
		for ( int n = 0; n<  MAX_GM_COMMAND; n++ )
		{
			this->nCmdCode[0] = -1;	// #error Change the Zero for n
		}

		this->count=0;
	}	// line : 128

	void Add(LPSTR command_str, int command_code) // line : 131
	{
		strcpy(this->szCmd[this->count], command_str);
		this->nCmdCode[this->count]=command_code;

		if ( this->count < MAX_GM_COMMAND-1 )
		{
			this->count++;
		}
	}	// line : 135

public:
	
	char szCmd[MAX_GM_COMMAND][MAX_GMCOMMAND_LEN];	// 0
	int nCmdCode[MAX_GM_COMMAND];	// 7D0
	int count;	// 960
};

void PostSend(LPOBJ lpObj, char * szMessage);
void GlobalPostSend(LPOBJ lpObj,int Type,char Sender[20],const char*Message,...);

class CGMMng
{

public:

	CGMMng();
	virtual ~CGMMng();

	void Init();
	int GetCmd(LPSTR szCmd);
	int ManagementProc(LPOBJ lpObj, LPSTR szCmd, int aIndex);
	LPSTR GetTokenString();
	int GetTokenNumber();
	void GetInfinityArrowMPConsumption(LPOBJ lpObj);
	void ControlInfinityArrowMPConsumption0(LPOBJ lpObj, int iValue);
	void ControlInfinityArrowMPConsumption1(LPOBJ lpObj, int iValue);
	void ControlInfinityArrowMPConsumption2(LPOBJ lpObj, int iValue);
	void SetInfinityArrowTime(LPOBJ lpObj, int iValue);
	void ControlFireScreamDoubleAttackDistance(LPOBJ lpObj, int iValue);
	void ManagerInit();
	int  ManagerAdd(LPSTR name, int aIndex);
	void ManagerDel(LPSTR name);
	void ManagerSendData(LPSTR szMsg, int size);
	void BattleInfoSend(LPSTR Name1, BYTE score1, LPSTR Name2, BYTE score2);
	void DataSend(LPBYTE szMsg, int size);
	void LoadCommandFile(LPSTR szFile);
	void LoadGMFile(LPSTR szFile);

	BOOL CheckTraceMarryCondition(LPOBJ lpObj, LPOBJ lpTargetObj);

private:

	CGMCommand cCommand;	// 4
	char szManagerName[MAX_GAME_MASTER][11];	// 968
	int ManagerIndex[MAX_GAME_MASTER];	// 9A0
	COMMANDS_DATA m_CommandNames;
	std::vector<GM_DATA> m_GameMasters;

public:

	int WatchTargetIndex;	// 9B4

};

extern CGMMng cManager;

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

