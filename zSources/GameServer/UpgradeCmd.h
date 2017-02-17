//////////////////////////////////////////////////////////////////////
// UpgradeCmd.h
#ifndef UPGRADECMD_H
#define UPGRADECMD_H

struct UPGRADECMD_ITEM_DATA
{
	UPGRADECMD_ITEM_DATA()
	{
		this->wItemID = -1;
		this->btItemLevel = 0;
		this->btOption = 0;
		this->btSetOption = 0;
		this->btExc = 0;
		memset(this->btSocketOption, -1, sizeof(this->btSocketOption));
	}

	WORD wItemID;
	BYTE btItemLevel;
	BYTE btOption;
	BYTE btSetOption;
	BYTE btExc;
	BYTE btSocketOption[5];
};

struct UPGRADECMD_DATA
{
	int iUpgradeID;
	UPGRADECMD_ITEM_DATA m_NeedItem;
	UPGRADECMD_ITEM_DATA m_GetItem;
};

#pragma once
class CUpgradeCmd
{
public:
	CUpgradeCmd();
	virtual ~CUpgradeCmd();

	void LoadFile(char * szFile);
	int DoUpgrade(int aIndex);

private:
	bool m_bSystemEnable;
	std::map<int, UPGRADECMD_DATA> m_mapUpgradeData;
};

extern CUpgradeCmd g_UpgradeCmd;

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

