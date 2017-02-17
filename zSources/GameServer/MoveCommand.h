//////////////////////////////////////////////////////////////////////
// MoveCommand.h
#ifndef MOVECOMMAND_H
#define MOVECOMMAND_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "user.h"

#define MAX_MOVE_COMMAND 60

typedef struct
{
	int Index;	// 0
	char Name[255];	// 3
	char EngName[255];	// 103
	int NeedZen;	// 204
	int NeedLevel;	// 208
	int MaxLevel;
	int GateNumber;	// 20C
} MOVE_COMMAND_DATA, *LPMOVE_COMMAND_DATA;

typedef struct
{
	int MoveLevel;	// 0
	int MapNumber;	// 4
	int X;	// 8
	int Y;	// C
	int TX;	// 10
	int TY;	// 14
} MOVE_MOVE_LEVEL, *LPMOVE_MOVE_LEVEL;

class CMoveCommand
{

private:

	int FindIndex(char* mapname);
	int FindIndex(int iMapIndex);

public:

	CMoveCommand();
	virtual ~CMoveCommand();

	void Init();
	int Load(char* Buffer, int iSize);
	int Load(char* filename);
	int LoadMoveLevel(char* filename);
	int GetMoveLevel(int mapnumber, int x, int y, int Class);
	int CheckMainToMove(LPOBJ lpObj);
	int CheckEquipmentToMove(LPOBJ lpObj, int iTargetMapNumber);
	int CheckInterfaceToMove(LPOBJ lpObj);
	int Move(LPOBJ lpObj, int iMapIndex);
	int MoveFree2Kalima(LPOBJ lpObj);
	BOOL CheckMoveMapBound(int iMapIndex);
	LPMOVE_COMMAND_DATA GetMoveCommandData(int nMapIndex);
	LPMOVE_COMMAND_DATA GetMoveCommandDataByMapNumber(WORD wMapNumber);
private:

	MOVE_COMMAND_DATA m_MoveCommandData[MAX_MOVE_COMMAND];
	MOVE_MOVE_LEVEL m_MoveLevel[MAX_MOVE_COMMAND];
	
};

extern CMoveCommand gMoveCommand;

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

