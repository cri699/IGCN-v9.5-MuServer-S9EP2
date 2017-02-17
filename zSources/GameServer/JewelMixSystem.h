//////////////////////////////////////////////////////////////////////
// JewelMixSystem.h
// ------------------------------
// Decompiled by Deathway
// Date : 2007-05-09
// ------------------------------
#ifndef JEWELMIXSYSTEM_H
#define JEWELMIXSYSTEM_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CJewelMixSystem
{

public:

	CJewelMixSystem();
	virtual ~CJewelMixSystem();

	static void LoadMixJewelPrice();

	static int GetJewelCount(int iIndex, int iJewelType);
	static int GetJewelCountPerLevel(int iJewelType, int iJewelLevel);
	static int MixJewel(int iIndex, int iJewelType, int iMixType);
	static int UnMixJewel(int iIndex, int iJewelType, int iJewelLevel, int iInventoryPos);

private:
	static int UnMixJewelPrice;
};

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

