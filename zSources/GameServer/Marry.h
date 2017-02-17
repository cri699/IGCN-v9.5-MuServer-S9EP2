//////////////////////////////////////////////////////////////////////
// Marry.h
#ifndef _CMARRY_H_
#define _CMARRY_H_

#include "user.h"

struct MARRY_ITEM
{
	int m_iSide;
	WORD m_wItemID;
	int m_iItemCount;
};

class CMarry
{
public:
	CMarry();
	virtual ~CMarry();

	void LoadFile(char *filename);

	void Propose(int aIndex, int uIndex);
	bool Accept(int aIndex);
	void Divorce(int aIndex);

private:
	bool CheckPosition(int aIndex, int uIndex);
	bool CheckRequiredItem(int aIndex, int iSide);
	bool CheckDivorceItem(int aIndex);
	void GiveGiftItem(int aIndex, int iSide);
	void DeleteRequiredItem(int aIndex, int iSide);
	void DeleteDivorceItem(int aIndex);

	bool m_bFileLoad;
	bool m_bMarryEnable;
	int m_iMarryMinUserLevel;
	int m_iMarryNeedMoney;

	WORD m_wMarryMapNumber;
	BYTE m_btMarryStartX;
	BYTE m_btMarryStartY;
	BYTE m_btMarryEndX;
	BYTE m_btMarryEndY;

	bool m_bHomoSexualAllow;
	bool m_bNeedSpecialItemForMarry;

	bool m_bGiftEnable;

	bool m_bDivorceAllow;
	bool m_bNeedSpecialItemForDivorce;

	std::vector<MARRY_ITEM> m_vtMarryItem;
	std::vector<MARRY_ITEM> m_vtGiftItem;
	std::vector<MARRY_ITEM> m_vtDivorceItem;

	CRITICAL_SECTION m_criti;
};

inline bool isFemale(int aIndex)
{
	return (gObj[aIndex].Class == CLASS_ELF || gObj[aIndex].Class == CLASS_SUMMONER || gObj[aIndex].Class == CLASS_GROWLANCER) ? true : false;
}

inline bool isMale(int aIndex)
{
	return (gObj[aIndex].Class == CLASS_WIZARD || gObj[aIndex].Class == CLASS_KNIGHT || gObj[aIndex].Class == CLASS_MAGUMSA ||
			gObj[aIndex].Class == CLASS_DARKLORD || gObj[aIndex].Class == CLASS_RAGEFIGHTER) ? true : false; 
}
extern CMarry Marry;

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

