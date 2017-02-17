//////////////////////////////////////////////////////////////////////
// EventEntryLevel.h
#ifndef EVENT_ENTRY_LEVEL_H
#define EVENT_ENTRY_LEVEL_H

#pragma once

static int g_BloodCastleEnterLevel_Normal[7][2]; // 7 levels, BC 8 is ALWAYS MASTER LEVEL
static int g_BloodCastleEnterLevel_Magumsa[7][2];

static int g_DevilSquareEnterLevel_Normal[6][2]; // 6 levels, DS 7 is ALWAYS MASTER
static int g_DevilSquareEnterLevel_Magumsa[6][2];

void LoadBCEntryLevel();
void LoadDSEntryLevel();

void HookBCLevel();
void ModifyDSLevel();

#pragma pack (1)
struct BC_LEVEL_INFO_MAIN
{
	int iBloodCastleLevel;
	int iMinLevel;
	int iMaxLevel;
	char bIsMagumsa;
};
#pragma pack ()

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

