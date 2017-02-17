// ItemAddOption.cpp: implementation of the CItemAddOption class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ItemAddOption.h"
#include "Protocol.h"
#include "TLog.h"

CItemAddOption	g_ItemAddOption;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CItemAddOption::CItemAddOption()
{
	Initialize();
}

CItemAddOption::~CItemAddOption()
{

}

void CItemAddOption::Load(char *chFileName)
{
	Initialize();

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(chFileName);

	if (res.status != pugi::status_ok)
	{
		g_Log.MsgBox("Failed to load %s file (%s)", chFileName, res.description());
		return;
	}

	int iItemAddOptionIndex = 0;

	pugi::xml_node main = file.child("ItemOptionManager");

	for (pugi::xml_node section = main.child("Section"); section; section = section.next_sibling())
	{
		int iSectionID = section.attribute("ID").as_int();

		if (iSectionID == 0) // 380 item Pink Options, not used by ItemAddOption
		{
			continue;
		}

		for (pugi::xml_node item = section.child("Item"); item; item = item.next_sibling())
		{
			int iItemType = item.attribute("Cat").as_int();
			int iItemIndex = item.attribute("Index").as_int();
			int iItemOption1 = item.attribute("Option1").as_int();
			int iItemValue1 = item.attribute("Value1").as_int();
			int iItemOption2 = item.attribute("Option2").as_int();
			int iItemValue2 = item.attribute("Value2").as_int();
			int iItemValidTime = item.attribute("Time").as_int();

			int iItemNumber = ITEMGET(iItemType, iItemIndex);
			m_ItemAddOption[iItemAddOptionIndex].wOptionType = iSectionID;
			m_ItemAddOption[iItemAddOptionIndex].iItemNumber = iItemNumber;
			m_ItemAddOption[iItemAddOptionIndex].iItemType = iItemType;
			m_ItemAddOption[iItemAddOptionIndex].iItemIndex = iItemIndex;
			m_ItemAddOption[iItemAddOptionIndex].wEffectType1 = iItemOption1;
			m_ItemAddOption[iItemAddOptionIndex].iEffectValue1 = iItemValue1;
			m_ItemAddOption[iItemAddOptionIndex].wEffectType2 = iItemOption2;
			m_ItemAddOption[iItemAddOptionIndex].iEffectValue2 = iItemValue2;
			m_ItemAddOption[iItemAddOptionIndex].iEffectValidTime = iItemValidTime;

			iItemAddOptionIndex++;
			if (iItemAddOptionIndex > 100 )
			{
				g_Log.MsgBox("[ItemAddOption] Overflow ItemAddOption Table Index");
			}
		}
	}
}

void CItemAddOption::Initialize()
{
	for ( int i=0; i<100; i++)
	{
		m_ItemAddOption[i].iItemNumber = 0;
		m_ItemAddOption[i].iItemType = 0;
		m_ItemAddOption[i].iItemIndex = 0;
		m_ItemAddOption[i].wEffectType1 = 0;
		m_ItemAddOption[i].iEffectValue1 = 0;
		m_ItemAddOption[i].wEffectType2 = 0;
		m_ItemAddOption[i].iEffectValue2 = 0;
		m_ItemAddOption[i].iEffectValidTime = 0;
	}
}

LPITEMEFFECT CItemAddOption::SearchItemEffect(int iItemNumber)
{
	for( int i=0;i<100;i++)
	{
		if( m_ItemAddOption[i].iItemNumber == iItemNumber )
		{
			return &m_ItemAddOption[i];
		}
	}
	return NULL;
}

bool CItemAddOption::IsValidEffect(int iItemNumber)
{
	if( SearchItemEffect(iItemNumber) == NULL )	return false;
	return true;
}

LPITEMEFFECT CItemAddOption::GetItemEffectData(int iItemNumber)
{
	return SearchItemEffect(iItemNumber);
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

