//////////////////////////////////////////////////////////////////////
// EventDungeonItemBag.cpp
#include "stdafx.h"
#include "EventDungeonItemBag.h"
#include "SetItemOption.h"
#include "TLog.h"
#include "user.h"
#include "DSProtocol.h"
#include "LargeRand.h"
#include "MapClass.h"
#include "GameMain.h"

CEventDungeonItemBag::CEventDungeonItemBag(void)
{
}

CEventDungeonItemBag::~CEventDungeonItemBag(void)
{
}

void CEventDungeonItemBag::LoadScript(char* szFileName)
{
	this->m_bLoad = FALSE;

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(szFileName);

	if ( res.status != pugi::status_ok )
	{
		g_Log.Add("%s file load error (%s)", szFileName, res.description());
		return;
	}

	pugi::xml_node main = file.child("ImperialGuardian");
	pugi::xml_node level_range = main.child("LevelRangeSettings");

	for (pugi::xml_node section = level_range.child("Section"); section; section = section.next_sibling())
	{
		int nLevelIndex = section.attribute("ID").as_int();

		if (nLevelIndex >= 10)
		{
			g_Log.MsgBox("Level index max over - %s", szFileName);
			return;
		}

		this->eventDungeonItemInfo[nLevelIndex].m_nLevelIndex = nLevelIndex;
		this->eventDungeonItemInfo[nLevelIndex].m_bUse = TRUE;
		this->eventDungeonItemInfo[nLevelIndex].m_nMinLevel = section.attribute("PlayerMinLevel").as_int();
		this->eventDungeonItemInfo[nLevelIndex].m_nMaxLevel = section.attribute("PlayerMaxLevel").as_int();
	}

	pugi::xml_node money_rate = main.child("MoneyDropSettings");

	for (pugi::xml_node section = money_rate.child("Section"); section; section = section.next_sibling())
	{
		int nLevelIndex = section.attribute("ID").as_int();

		if (nLevelIndex >= 10)
		{
			g_Log.MsgBox("Level index max over - %s", szFileName);
			return;
		}

		this->eventDungeonItemInfo[nLevelIndex].m_nDropZen = section.attribute("MoneyCount").as_int();
	}

	pugi::xml_node exc_rate = main.child("ExcItemDropSettings");

	for (pugi::xml_node section = exc_rate.child("Section"); section; section = section.next_sibling())
	{
		int nLevelIndex = section.attribute("ID").as_int();

		if (nLevelIndex >= 10)
		{
			g_Log.MsgBox("Level index max over - %s", szFileName);
			return;
		}

		this->eventDungeonItemInfo[nLevelIndex].m_nExOptionAddCntRate = section.attribute("SuccessRate").as_int();
	}

	for (pugi::xml_node item_section = main.child("Section"); item_section; item_section = item_section.next_sibling())
	{
		int nCnt = 0;
		int nLevelIndex = item_section.attribute("ID").as_int();

		if (nLevelIndex >= 10)
		{
			g_Log.MsgBox("Level index max over - %s", szFileName);
			return;
		}

		this->eventDungeonItemInfo[nLevelIndex].m_nDropCount = item_section.attribute("ItemDropCount").as_int();

		CEventItemBagAttr eventitembagattr;

		for (pugi::xml_node item = item_section.child("Item"); item; item = item.next_sibling())
		{
			eventitembagattr.m_btType = item.attribute("Cat").as_int();
			eventitembagattr.m_btIndex = item.attribute("Index").as_int();
			eventitembagattr.m_btMinLevel = item.attribute("MinLevel").as_int();
			eventitembagattr.m_btMaxLevel = item.attribute("MaxLevel").as_int();
			eventitembagattr.m_btIsSkill = item.attribute("Skill").as_int();
			eventitembagattr.m_btIsLuck = item.attribute("Luck").as_int();
			eventitembagattr.m_btIsOption = item.attribute("Option").as_int();
			eventitembagattr.m_btIsExItem = item.attribute("Exc").as_int();
			eventitembagattr.m_btIsSetItem = item.attribute("SetItem").as_int();
			eventitembagattr.m_nDropRate = item.attribute("DropRate").as_int();

			if (nCnt > 0)
			{
				CEventItemBagAttr previtem = this->eventDungeonItemInfo[nLevelIndex].m_vtItemBagAttr.at(nCnt - 1);
				eventitembagattr.m_nDropRate += previtem.m_nDropRate;
			}

			this->eventDungeonItemInfo[nLevelIndex].m_vtItemBagAttr.push_back(eventitembagattr);
			nCnt++;
		}
	}

	this->m_bLoad = TRUE;
	g_Log.Add("%s file load", szFileName);
}

int CEventDungeonItemBag::GetLevelIndex(int nUserLevel)
{
	for ( int i=0;i<10;i++ )
	{
		if ( nUserLevel >= this->eventDungeonItemInfo[i].m_nMinLevel && nUserLevel <= this->eventDungeonItemInfo[i].m_nMaxLevel )
		{
			return this->eventDungeonItemInfo[i].m_nLevelIndex;
		}
	}

	return -1;
}

int CEventDungeonItemBag::GetRegItemCount(int nUserLevel)
{
	int nLevelIndex = this->GetLevelIndex(nUserLevel);
	
	if ( nLevelIndex < 0 )
	{
		return 0;
	}

	return this->eventDungeonItemInfo[nLevelIndex].m_vtItemBagAttr.size();
}

int CEventDungeonItemBag::GetDropZen(int nUserLevel)
{
	int nLevelIndex = this->GetLevelIndex(nUserLevel);
	
	if ( nLevelIndex < 0 )
	{
		return 0;
	}

	return this->eventDungeonItemInfo[nLevelIndex].m_nDropZen;
}

int CEventDungeonItemBag::GetDropItemCount(int nUserLevel)
{
	int nLevelIndex = this->GetLevelIndex(nUserLevel);
	
	if ( nLevelIndex < 0 )
	{
		return 0;
	}

	return this->eventDungeonItemInfo[nLevelIndex].m_nDropCount;
}

int CEventDungeonItemBag::GetExcDropRate(int nUserLevel)
{
	int nLevelIndex = this->GetLevelIndex(nUserLevel);

	if (nLevelIndex < 0)
	{
		return 0;
	}

	return this->eventDungeonItemInfo[nLevelIndex].m_nExOptionAddCntRate;
}

int CEventDungeonItemBag::GetLevel(int nMinLevel, int nMaxLevel)
{
	if ( nMinLevel == nMaxLevel )
	{
		return nMinLevel;
	}

	return nMinLevel + rand() % ( nMaxLevel - nMinLevel + 1 );
}

BOOL CEventDungeonItemBag::DropEventItem(int nIndex, int nMaxUserLevel)
{
	if ( !this->m_bLoad )
	{
		return FALSE;
	}

	LPOBJ lpObj = &gObj[nIndex];
	CEventItemBagAttr BagObject;
	CEventItemBagAttr BagObject2;
	int nItemCount = this->GetRegItemCount(nMaxUserLevel);
	int nLevelIndex = this->GetLevelIndex(nMaxUserLevel);
	int nDropItemCount = this->GetDropItemCount(nMaxUserLevel);

	if ( nItemCount > 0 && nLevelIndex > -1 )
	{
		for (int n=0; n < nDropItemCount; n++ )
		{
			int x = lpObj->X;
			int y = lpObj->Y;
			DWORD nRand = GetLargeRand() % 1000000;
			bool bDropItem = false;
			bool bSetItem = false;

			for ( int i = nItemCount -1; i >= 0; i-- )
			{
				if ( !i )
				{
					BagObject = this->eventDungeonItemInfo[nLevelIndex].m_vtItemBagAttr[0];
					bSetItem = BagObject.m_btIsSetItem;
					bDropItem = true;
					break;
				}

				BagObject = this->eventDungeonItemInfo[nLevelIndex].m_vtItemBagAttr[i];
				BagObject2 = this->eventDungeonItemInfo[nLevelIndex].m_vtItemBagAttr[i-1];

				if ( nRand >= (1000000-BagObject.m_nDropRate) && nRand < (1000000-BagObject2.m_nDropRate) )
				{
					bDropItem = true;
					break;
				}
			}

			if ( bDropItem == true )
			{
				if ( BagObject.m_btIsSetItem )
				{
					this->CreateSetItem(nIndex, BagObject);
				}

				else
				{
					this->CreateNormalItem(nIndex, nMaxUserLevel, BagObject);
				}
			}
		}
	}

	int x = lpObj->X;
	int y = lpObj->Y;
	int nDropZen = this->GetDropZen(nMaxUserLevel);

	if ( nDropZen > 0 )
	{
		MapC[lpObj->MapNumber].MoneyItemDrop(nDropZen, x, y);
	}

	return TRUE;
}

BOOL CEventDungeonItemBag::CreateNormalItem(int nIndex, int nMaxUserLevel, CEventItemBagAttr BagObject)
{
	int nLevel = this->GetLevel(BagObject.m_btMinLevel, BagObject.m_btMaxLevel);
	int nType = ItemGetNumberMake(BagObject.m_btType, BagObject.m_btIndex);
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;
	int ExOption = 0;
	LPOBJ lpObj = &gObj[nIndex];

	if ( nType == -1 )
	{
		return FALSE;
	}

	if ( BagObject.m_btIsSkill )
	{
		if ( BagObject.m_btIsSkill == 0xFF )
		{
			Option1 = TRUE;
		}

		else
		{
			Option1 = rand()%2;
		}
	}

	if ( BagObject.m_btIsLuck )
	{
		if ( BagObject.m_btIsLuck == 0xFF )
		{
			Option2 = TRUE;
		}

		else
		{
			Option2 = rand()%2;
		}
	}

	if ( BagObject.m_btIsOption )
	{
		if ( rand()%5 == 0 )
		{
			Option3 = 3;
		}

		else
		{
			Option3 = rand()%3;
		}
	}

	if ( BagObject.m_btIsExItem ) 
	{
		int nExOptionCnt = (GetLargeRand() % 1000000) >= this->GetExcDropRate(nMaxUserLevel) ? 1 : 2;

		for ( int i=0;i<nExOptionCnt;i++ )
		{
			int nExOptIndex = rand()%6;

			if ( (ExOption & (1 << nExOptIndex)) == 1 << nExOptIndex )
			{
				nExOptIndex++;

				if ( nExOptIndex > 5 )
					nExOptIndex = 0;
			}

			ExOption |= 1 << nExOptIndex;
		}

		if ( ExOption )
		{
			Option1 = TRUE;
			Option2 = FALSE;
			nLevel = 0;
		}
	}

	if ( nType == ITEMGET(12,15) || nType == ITEMGET(14,13) || nType == ITEMGET(14,14) )
	{
		Option1 = FALSE;
		Option2 = FALSE;
		Option3 = FALSE;
		nLevel = 0;
	}

	if ( nType == ITEMGET(13,0) || nType == ITEMGET(13,1) || nType == ITEMGET(13,2) || nType == ITEMGET(13,8) || nType == ITEMGET(13,9) || nType == ITEMGET(13,12) || nType == ITEMGET(13,13) )
	{
		nLevel = 0;
	}

	BYTE btDropX = lpObj->X;
	BYTE btDropY = lpObj->Y;

	if( !gObjGetRandomItemDropLocation(lpObj->MapNumber, btDropX, btDropY, 2, 2, 10) )
	{
		btDropX = lpObj->X;
		btDropY = lpObj->Y;
	}

	ItemSerialCreateSend(nIndex, lpObj->MapNumber, btDropX, btDropY, nType, nLevel, 0, Option1, Option2, Option3, nIndex, ExOption, 0, 0, 0, 0);
	g_Log.Add("[IMPERIALGUARDIAN] Drop Item : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d ExOp:%d",
		lpObj->MapNumber, btDropX, btDropY, ItemAttribute[nType].Name, nType, nLevel, Option1, Option2, Option3, ExOption);

	return TRUE;
}

int CEventDungeonItemBag::CreateSetItem(int nIndex, CEventItemBagAttr BagObject)
{
	int nType = ItemGetNumberMake(BagObject.m_btType, BagObject.m_btIndex);
	int SetOption = gSetItemOption.GenSetOption(nType);
	int Option2 = 0;
	int Option3 = 0;
	LPOBJ lpObj = &gObj[nIndex];
	BYTE btMapNumber = gObj[nIndex].MapNumber;
	int option2rand = 4;
	int option3rand = rand()%100;
	int optionc = rand()%3;

	if ( rand()% 100 < 4 )
		Option2 = TRUE;

	if ( optionc )
	{
		if ( optionc == 1 )
		{
			if ( option3rand < 8 )
			{
				Option3 = 2;
			}
		}

		else if ( optionc == 2 )
		{
			if ( option3rand < 12 )
			{
				Option3 = 1;
			}
		}
	}

	else
	{
		if ( option3rand < 4 )
			Option3 = 3;
	}

	BYTE btDropX = lpObj->X;
	BYTE btDropY = lpObj->Y;

	if ( !gObjGetRandomItemDropLocation(btMapNumber, btDropX, btDropY, 2, 2, 10) )
	{
		btDropX = lpObj->X;
		btDropY = lpObj->Y;
	}

	int tmpSetOption = 0;

	if ( SetOption & 1 )
	{
		tmpSetOption = 1;
	}

	else
	{
		if ( SetOption & 2 )
			tmpSetOption = 2;
	}

	ItemSerialCreateSend(nIndex, btMapNumber, btDropX, btDropY, nType, 0, 0, TRUE, Option2, Option3, nIndex, 0, SetOption, 0, 0, 0);
	g_Log.Add("[IMPERIALGUARDIAN] Drop Set Item [%s][%s] : (%d)(%d/%d) Itemnum:[%d] skill:[%d] luck:[%d] option:[%d] SetOption:[%d], SetName:[%s]",
		lpObj->AccountID, lpObj->Name, btMapNumber, btDropX, btDropY, nType, TRUE, Option2, Option3, SetOption, gSetItemOption.GetSetOptionName(tmpSetOption));

	return TRUE;
}





//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

