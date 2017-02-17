//////////////////////////////////////////////////////////////////////
// ObjCalCharacter.h
#ifndef OBJCALCHARACTER_H
#define OBJCALCHARACTER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "user.h"
#include "zzzitem.h"

class CObjCalCharacter
{
public:

	CObjCalCharacter();
	virtual ~CObjCalCharacter();
	
	void Init();

	void CalcCharacter(int aIndex);
	void CalcSetItemStat(LPOBJ lpObj);
	void GetSetItemOption(LPOBJ lpObj, LPBYTE pSetOptionTable, LPBYTE pSetOptionCountTable, int * pSetOptionCount);
	void CalcSetItemOption(LPOBJ lpObj);
	void SetItemStatPlusSpecial(LPOBJ lpObj, int option, int ivalue);
	void SetItemPlusSpecial(LPOBJ lpObj, int option, int ivalue);
	void SetItemApply(LPOBJ lpObj);
	void CalcMLSkillItemOption(LPOBJ lpObj);
	BOOL ValidItem(LPOBJ lpObj, CItem * lpItem, int pos);
	void CalcShieldPoint(LPOBJ lpObj);
	void PremiumItemApply(LPOBJ lpObj);

private:

	MULua m_Lua;
};

extern CObjCalCharacter gObjCalCharacter;
#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

