//////////////////////////////////////////////////////////////////////
// MapAttribute.cpp
#include "stdafx.h"
#include "MapAttribute.h"
#include "TLog.h"

CMapAttribute g_MapAttr;

CMapAttribute::CMapAttribute(void)
{
}

CMapAttribute::~CMapAttribute(void)
{
}

void CMapAttribute::LoadFile(char * szFile)
{
	for (int i = 0; i < MAX_NUMBER_MAP; i++)
	{
		this->m_MapAttr[i].Clear();
	}

	pugi::xml_document file;
	pugi::xml_parse_result result = file.load_file(szFile);

	if (result.status != pugi::status_ok)
	{
		g_Log.MsgBox("Failed to load %s file!", szFile);
		g_Log.AddC(TColor::Red, "[ERROR] MapAttribute load failed: %s", szFile);
		return;
	}

	pugi::xml_node main_section = file.child("MapAttribute");
	pugi::xml_node config_section = main_section.child("Config");

	for (pugi::xml_node map = config_section.child("Map"); map; map = map.next_sibling())
	{
		int MapNumber = map.attribute("Number").as_int(-1);

		if ( MapNumber < 0 || MapNumber > MAX_NUMBER_MAP )
		{
			g_Log.MsgBox("[Map Attribute] Invalid MapNumber (%d)", MapNumber);
			continue;
		}

		int PvPType = map.attribute("PvPConfig").as_int(0);
		int ItemRateDropBonus = map.attribute("ItemDropRateBonus").as_int(0);
		float ExpBonus = map.attribute("ExpBonus").as_float(0.0);
		float MasterExpBonus = map.attribute("MasterExpBonus").as_float(0.0);
		int IsVipMap = map.attribute("VipLevel").as_int(0);
		int PvMType = map.attribute("PvMConfig").as_int(0);
		int PkInc = map.attribute("PkLevelIncrease").as_int(1);
		int GensSet = map.attribute("Gens").as_int(-1);
		int UserRegen = map.attribute("RegenOnSamePlace").as_int(0);
		int BlockEntry = map.attribute("BlockEntry").as_int(0);

		this->m_MapAttr[MapNumber].PvPType = PvPType;
		this->m_MapAttr[MapNumber].ItemRateDropBonus = ItemRateDropBonus;
		this->m_MapAttr[MapNumber].ExpBonus = ExpBonus;
		this->m_MapAttr[MapNumber].MasterExpBonus = MasterExpBonus;
		this->m_MapAttr[MapNumber].IsVipMap = IsVipMap;
		this->m_MapAttr[MapNumber].PvMType = PvMType;
		this->m_MapAttr[MapNumber].PkIncrease = PkInc;
		this->m_MapAttr[MapNumber].GensSet = GensSet;
		this->m_MapAttr[MapNumber].UserRegen = UserRegen;
		this->m_MapAttr[MapNumber].BlockEntry = BlockEntry;
	}

	g_Log.AddC(TColor::DarkOrange, "MapAttribute::LoadFile() -> Success!");
}


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

