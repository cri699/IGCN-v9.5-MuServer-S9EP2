//////////////////////////////////////////////////////////////////////
// RaklionMonsterMng.h
#ifndef RAKLION_MONSTER_MNG_H
#define RAKLION_MONSTER_MNG_H

#pragma once

#define MAX_RAKLION_MONSTER	26		// Selupan + 15 eggs + 10 summons = 26

struct RAKLION_MONSTER_SETBASE_INFO
{
	BYTE btGroup;	// 0
	WORD wType;	// 2
	BYTE btMapNumber;	// 4
	BYTE btDistance;	// 5
	BYTE btX;	// 6
	BYTE btY;	// 7
	BYTE btDir;	// 8
};

class CRaklionMonsterMng
{
public:
	CRaklionMonsterMng();
	virtual ~CRaklionMonsterMng();

	void ResetLoadData();
	BOOL LoadData(LPSTR lpszFileName);

	void SetMonsterSetBaseInfo(int iIndex, BYTE btGroup, WORD wType, BYTE btMapNumber, BYTE btDistance, BYTE btX, BYTE btY, BYTE btDir);

	void SetAllMonsters();
	int SetPosition(int iMonsterIndex, int iTableNum);
	int GetPosition(int TableNum, short MapNumber, short & x, short & y);

public:
	int m_iMaxMonsterCount;	// 4
	RAKLION_MONSTER_SETBASE_INFO m_SetBaseInfo[MAX_RAKLION_MONSTER];
	int m_iFileDataLoad;
};

extern CRaklionMonsterMng g_RaklionMonsterMng;

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

