//////////////////////////////////////////////////////////////////////
// ChangeCmd.h
#ifndef CHANGECMD_H
#define CHANGECMD_H

struct CHANGE_ITEM_DATA
{
	WORD ItemID;
	BYTE CoinType;
	DWORD CoinValue;
};

#pragma once
class CChangeCmd
{
public:
	CChangeCmd();
	virtual ~CChangeCmd();

	void LoadFile(char *szFile);
	int DoChange(int aIndex);

private:
	bool m_bSystemEnable;
	std::map<int, CHANGE_ITEM_DATA> m_mapChangeData;
};

extern CChangeCmd g_ChangeCmd;

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

