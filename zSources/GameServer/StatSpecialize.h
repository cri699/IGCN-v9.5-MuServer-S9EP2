//////////////////////////////////////////////////////////////////////
// StatSpecialize.h
#ifndef STATSPECIALIZE_H
#define STATSPECIALIZE_H

#include "user.h"
#include "classdef.h"
#include "MuLua.h"

#define MAX_STAT_OPTIONS 12
#define MAX_USER_STAT_OPTIONS 5

enum STAT_SPECIALIZE_OPTIONS
{
	STAT_OPTION_INC_ATTACK_POWER = 1,
	STAT_OPTION_INC_ATTACK_RATE = 2,
	STAT_OPTION_INC_ATTACK_RATE_PVP = 3,
	STAT_OPTION_INC_DEFENSE = 4,
	STAT_OPTION_INC_ATTACK_SPEED = 5,
	STAT_OPTION_INC_DEFENSE_RATE = 6,
	STAT_OPTION_INC_DEFENSE_RATE_PVP = 7,
	STAT_OPTION_INC_LIFE = 8,
	STAT_OPTION_INC_MAGIC_DAMAGE = 9,
	STAT_OPTION_INC_CURSE_DAMAGE = 10,
	STAT_OPTION_INC_MANA = 11,
	STAT_OPTION_INC_PET_DAMAGE = 12
};

struct STAT_OPTION
{
	int StatOptionID;
	BYTE StatOptionClass[MAX_TYPE_PLAYER];
};

struct PMSG_STAT_SPEC_OPTION
{
	PBMSG_HEAD2 h;
	STAT_USER_OPTION m_StatOption[5];
};

#pragma once
class CStatSpecialize
{
public:
	CStatSpecialize(void);
	virtual ~CStatSpecialize(void);

	bool LoadFile(char *szFile);
	void CalcStatOption(OBJECTSTRUCT *lpObj, int StatOptionID);
	void SendOptionList(OBJECTSTRUCT *lpObj);
	void ClearUserOption(OBJECTSTRUCT *lpObj);

private:
	void SetStatOption(OBJECTSTRUCT *lpObj, int StatOptionID, double StatOptionPercent);
	int GetUserArrayIndex(OBJECTSTRUCT *lpObj);

	bool m_bFileLoad;
	STAT_OPTION m_StatOptions[MAX_STAT_OPTIONS];
	MULua m_Lua;
};

extern CStatSpecialize g_StatSpec;

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

