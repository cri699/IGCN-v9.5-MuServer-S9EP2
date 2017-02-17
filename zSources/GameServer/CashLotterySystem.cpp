//////////////////////////////////////////////////////////////////////
// CashLotterySystem.cpp
// CashItemPeriodSystem.cpp: implementation of the CCashLotterySystem class.
// GS-N	1.00.18	JPN	0x00593630	-	Completed
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CashLotterySystem.h"
#include "LargeRand.h"
#include "user.h"
#include "TLog.h"
#include "configread.h"

CCashLotterySystem g_ChaosCard;
CCashLotterySystem g_ChaosCardMini;
CCashLotterySystem g_ChaosCardGold;
CCashLotterySystem g_ChaosCardRare;


CCashLotterySystem::CCashLotterySystem()
{
	return;
}

CCashLotterySystem::~CCashLotterySystem()
{
	return;
}

void CCashLotterySystem::Initialize()
{
	for ( int i=0;i<MAX_LOTTERY_ITEM_CATEGORY;i++)
	{
		this->LotteryItemListCount[i] = 0;
		this->LotteryItemOptionRate[i].iSkillOptionRate = 0;
		this->LotteryItemOptionRate[i].iLuckOptionRate = 0;
		this->LotteryItemOptionRate[i].iAddOptionRate = 0;
		this->LotteryItemOptionRate[i].iExOptionRate =0 ;
	}

	for (int i=0;i<MAX_LOTTERY_ITEM_CATEGORY;i++)
	{
		for ( int j=0;j<MAX_LOTTERY_ITEM_COUNT;j++)
		{
			this->LotteryItemList[i][j].btItemType = -1;
			this->LotteryItemList[i][j].wItemIndex = -1;
			this->LotteryItemList[i][j].btItemLevelMin = -1;
			this->LotteryItemList[i][j].btItemLevelMax = -1;
			this->LotteryItemList[i][j].btSkillOption = -1;
			this->LotteryItemList[i][j].btLuckOption = -1;
			this->LotteryItemList[i][j].btAddOption = -1;
			this->LotteryItemList[i][j].btExOption = -1;
		}
	}

	this->CategoryRandomPool.InitPool();
	this->AddOptionRandomPool.InitPool();
	this->ExOptionRandomPool.InitPool();
	this->iItemMaxHeight = 0;
	this->iItemMaxWidth = 0;
}


void CCashLotterySystem::Load(LPSTR pchFilename)
{
	this->Initialize();

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFilename);

	if ( res.status != pugi::status_ok )
	{
		g_Log.MsgBox("[Loterry System] Lottery Item List load failed. [%s] [%s]", pchFilename, res.description());
		return;
	}

	int iCategory = 0;
	int iDropRate = 0;
	int iSkillOptionRate = 0;
	int iLuckOptionRate = 0;
	int iAddOptionRate = 0;
	int iExOptionRate = 0;
	int iItemAddOption = 0;
	int iItemAddOptionRate = 0;
	int iItemExOption = 0;
	int iItemExOptionRate = 0;
	int iItemType = 0;
	int iItemIndex = 0;
	int iItemLevelMin = 0;
	int iItemLevelMax = 0;
	int iSkillOption = 0;
	int iLuckOption = 0;
	int iAddOption = 0;
	int iExOption = 0;
	
	pugi::xml_node main = file.child("ChaosCard");
	pugi::xml_node general = main.child("GeneralRateSettings");

	for (pugi::xml_node section = general.child("Section"); section; section = section.next_sibling())
	{
		iCategory = section.attribute("ID").as_int();
		iDropRate = section.attribute("DropRate").as_int();
		iSkillOptionRate = section.attribute("SkillRate").as_int();
		iLuckOptionRate = section.attribute("LuckRate").as_int();
		iAddOptionRate = section.attribute("OptionRate").as_int();
		iExOptionRate = section.attribute("ExcRate").as_int();

		this->SetitemDropRate(iCategory, iDropRate, iSkillOptionRate, iLuckOptionRate, iAddOptionRate, iExOptionRate);
	}

	pugi::xml_node option_settings = main.child("OptionDrawSettings");

	for (pugi::xml_node option = option_settings.child("Option"); option; option = option.next_sibling())
	{
		iItemAddOption = option.attribute("Level").as_int();
		iItemAddOptionRate = option.attribute("SuccessRate").as_int();

		this->AddOptionRandomPool.AddValue(iItemAddOption, iItemAddOptionRate);
	}

	pugi::xml_node excoption_settings = main.child("ExcOptionDrawRate");

	for (pugi::xml_node option = excoption_settings.child("Option"); option; option = option.next_sibling())
	{
		iItemExOption = option.attribute("Type").as_int();
		iItemExOptionRate = option.attribute("SuccessRate").as_int();

		this->ExOptionRandomPool.AddValue(iItemExOption, iItemExOptionRate);
	}

	for (pugi::xml_node section = main.child("Section"); section; section = section.next_sibling())
	{
		iCategory = section.attribute("ID").as_int();

		for (pugi::xml_node item = section.child("Item"); item; item = item.next_sibling())
		{
			iItemType = item.attribute("Cat").as_int();
			iItemIndex = item.attribute("Index").as_int();
			iItemLevelMin = item.attribute("MinLevel").as_int();
			iItemLevelMax = item.attribute("MaxLevel").as_int();
			iSkillOption = item.attribute("Skill").as_int();
			iLuckOption = item.attribute("Luck").as_int();
			iAddOption = item.attribute("Option").as_int();
			iExOption = item.attribute("Exc").as_int();

			this->InsertItem(iCategory, iItemType, iItemIndex, iItemLevelMin, iItemLevelMax, iSkillOption, iLuckOption, iAddOption, iExOption);
		}
	}
}



void CCashLotterySystem::SetitemDropRate(int iCategory, int iDropRate, int iSkillOptionRate, int iLuckOptionRate, int iAddOptionRate, int iExOptionRate)
{
	this->CategoryRandomPool.AddValue(iCategory, iDropRate);

	this->LotteryItemOptionRate[iCategory].iSkillOptionRate = iSkillOptionRate;
	this->LotteryItemOptionRate[iCategory].iLuckOptionRate = iLuckOptionRate;
	this->LotteryItemOptionRate[iCategory].iAddOptionRate = iAddOptionRate;
	this->LotteryItemOptionRate[iCategory].iExOptionRate = iExOptionRate;
}



BOOL CCashLotterySystem::InsertItem(int iItemCategory, int iItemType, int iItemIndex, int iItemLevelMin, int iItemLevelMax, int iSkillOption, int iLuckOption, int iAddOption, int iExOption)
{
	if (iItemCategory < 0 || iItemCategory > MAX_LOTTERY_ITEM_CATEGORY)
	{
		g_Log.MsgBox("Error - Max Category limit (20) reached");
		return FALSE;
	}

	int iItemCount = this->LotteryItemListCount[iItemCategory];

	if ( iItemCount < 0 || iItemCount >= MAX_LOTTERY_ITEM_COUNT )
	{
		g_Log.MsgBox("Error - Max Item limit (130) per category reached (Category: %d)", iItemCategory);
		return FALSE;
	}

	this->LotteryItemList[iItemCategory][iItemCount].btItemType = iItemType;
	this->LotteryItemList[iItemCategory][iItemCount].wItemIndex = iItemIndex;
	this->LotteryItemList[iItemCategory][iItemCount].btItemLevelMin = iItemLevelMin;
	this->LotteryItemList[iItemCategory][iItemCount].btItemLevelMax = iItemLevelMax;
	this->LotteryItemList[iItemCategory][iItemCount].btSkillOption = iSkillOption;
	this->LotteryItemList[iItemCategory][iItemCount].btLuckOption = iLuckOption;
	this->LotteryItemList[iItemCategory][iItemCount].btAddOption = iAddOption;
	this->LotteryItemList[iItemCategory][iItemCount].btExOption = iExOption;

	int iItemCode = 0;
	int iItemHeight = 0;
	int iItemWidth = 0;

	iItemCode = ITEMGET(iItemType, iItemIndex);
	iItemHeight = ItemAttribute[iItemCode].Height;
	iItemWidth = ItemAttribute[iItemCode].Width;

	if ( iItemWidth > this->iItemMaxWidth )
		this->iItemMaxWidth = iItemWidth;

	if ( iItemHeight > this->iItemMaxHeight )
		this->iItemMaxHeight = iItemHeight;

	this->LotteryItemListCount[iItemCategory]++;

	return TRUE;
}


int CCashLotterySystem::GetItem(CItem *lpItem)
{
	int iItemCategory = 0;
	int iMaxItemCountInCategory = 0;
	int iItemSelectNumber = 0;
	CItem ReturnItem;
	int iItemNumber = 0;
	int iItemType = 0;
	int iItemIndex = 0;
	int iItemLevel = 0;
	int iItemLevelMin = 0;
	int iItemLevelMax = 0;
	int iSkillOption = 0;
	int iLuckOption = 0;
	int iAddOption = 0;
	int iExOption = 0;
	int iSkillOptionRate = 0;
	int iLuckOptionRate = 0;
	int iAddOptionRate = 0;
	int iExOptionRate = 0;
	BYTE btSkillOption = 0;
	BYTE btLuckOption = 0;
	BYTE btAddOption = 0;
	BYTE btExOption = 0;
	BYTE btExOptionKey = 0;
	BYTE btExOptionValue1 = 0;
	BYTE btExOptionValue2 = 0;
	int iTemp = 0;
	int iRandomKey = 0;
	BYTE ExOption[MAX_EXOPTION_SIZE];

	if ( lpItem == NULL )
		return -1;

	iItemCategory = this->CategoryRandomPool.GetRandomValue(eRANDOMPOOL_BY_WEIGHT);

	if ( iItemCategory < 0 || iItemCategory > MAX_LOTTERY_ITEM_CATEGORY )
		return -1;

	iMaxItemCountInCategory = this->LotteryItemListCount[iItemCategory];

	if ( iMaxItemCountInCategory <= 0 ||  iMaxItemCountInCategory >= MAX_LOTTERY_ITEM_COUNT )
		return -1;

	iItemSelectNumber = rand() % iMaxItemCountInCategory;

	if ( iItemSelectNumber < 0 ||  iItemSelectNumber >= MAX_LOTTERY_ITEM_COUNT )
		return -1;

	iItemType = this->LotteryItemList[iItemCategory][iItemSelectNumber].btItemType;
	iItemIndex = this->LotteryItemList[iItemCategory][iItemSelectNumber].wItemIndex;
	iItemLevelMin = this->LotteryItemList[iItemCategory][iItemSelectNumber].btItemLevelMin;
	iItemLevelMax = this->LotteryItemList[iItemCategory][iItemSelectNumber].btItemLevelMax;
	iSkillOption = this->LotteryItemList[iItemCategory][iItemSelectNumber].btSkillOption;
	iLuckOption = this->LotteryItemList[iItemCategory][iItemSelectNumber].btLuckOption;
	iAddOption = this->LotteryItemList[iItemCategory][iItemSelectNumber].btAddOption;
	iExOption = this->LotteryItemList[iItemCategory][iItemSelectNumber].btExOption;

	if ( iItemType == -1 || iItemIndex == -1 || iItemLevelMin == -1 || iItemLevelMax == -1 || iSkillOption == -1 || iLuckOption == -1 || iAddOption == -1 || iExOption == -1 )
		return -1;

	iSkillOptionRate = this->LotteryItemOptionRate[iItemCategory].iSkillOptionRate;
	iLuckOptionRate = this->LotteryItemOptionRate[iItemCategory].iLuckOptionRate;
	iAddOptionRate = this->LotteryItemOptionRate[iItemCategory].iAddOptionRate;
	iExOptionRate = this->LotteryItemOptionRate[iItemCategory].iExOptionRate;

	iRandomKey = rand() % (iItemLevelMax - iItemLevelMin + 1);
	iItemLevel = iItemLevelMin + iRandomKey;

	if ( iItemLevel > iItemLevelMax || iItemLevel < iItemLevelMin )
		iItemLevel = iItemLevelMin;

	if ( iSkillOption == 1 )
	{
		iRandomKey = GetLargeRand() % 1000000;

		if ( iRandomKey < iSkillOptionRate )
			btSkillOption = 1;
	}

	if ( iLuckOption == 1 )
	{
		iRandomKey = GetLargeRand() % 1000000;

		if ( iRandomKey < iLuckOptionRate )
			btLuckOption = 1;
	}

	if ( iAddOption == 1 )
	{
		iRandomKey = GetLargeRand() % 1000000;

		if ( iRandomKey < iAddOptionRate )
		{
			btAddOption = this->AddOptionRandomPool.GetRandomValue(eRANDOMPOOL_BY_WEIGHT);
			iTemp = btAddOption;
		}
	}

	if ( iExOption == 1 )
	{
		iRandomKey = GetLargeRand() % 1000000;

		if ( iRandomKey < iExOptionRate )
		{
			btExOptionValue1 = this->ExOptionRandomPool.GetRandomValue(eRANDOMPOOL_BY_WEIGHT);
			btExOptionKey =  1 << (int)(btExOptionValue1);
			btExOption |= btExOptionKey;

			iItemLevel = 0;

			if ( (rand() % 4) == 0 )
			{
				btExOptionValue2 = this->ExOptionRandomPool.GetRandomValue(eRANDOMPOOL_BY_WEIGHT);

				if ( btExOptionValue1 != btExOptionValue2 )
				{
					btExOptionKey =  1 << (int)(btExOptionValue2);
					btExOption |= btExOptionKey;
				}
			}
		}
	}
	else if ( iExOption == 2 )
	{
		btExOptionValue1 = this->ExOptionRandomPool.GetRandomValue(eRANDOMPOOL_BY_WEIGHT);
		btExOptionKey =  1 << (int)(btExOptionValue1);
		btExOption |= btExOptionKey;
		iItemLevel = 0;

		if ( (rand() % 4) == 0 )
		{
			btExOptionValue2 = this->ExOptionRandomPool.GetRandomValue(eRANDOMPOOL_BY_WEIGHT);

			if ( btExOptionValue1 != btExOptionValue2 )
			{
				btExOptionKey =  1 << (int)(btExOptionValue2);
				btExOption |= btExOptionKey;
			}
		}
	}

	if ( btAddOption != iTemp )
		btAddOption = 0;

	if (btAddOption > 4 && g_ConfigRead.data.common.Is28Opt == false)
		btAddOption = 4;

	iItemNumber = ItemGetNumberMake(iItemType, iItemIndex);
	lpItem->Convert(iItemNumber, btSkillOption, btLuckOption, btAddOption, btExOption, 0, 0, 0, -1, 0, CURRENT_DB_VERSION);
	lpItem->m_Level = iItemLevel;
	lpItem->m_Durability = ItemGetDurability(iItemNumber, lpItem->m_Level, btExOption, lpItem->m_SetOption);

	if ( !lpItem->IsItem() )
		return -1;

	ItemIsBufExOption(ExOption, lpItem);

	g_Log.Add("[LotteryItem] Get Item Category:%d,Index:%d (Name:%s,Type:%d,Index:%d,Level:%d) Skill:%d,Luck:%d,AddOption:%d,ExOption(%d,%d,%d,%d,%d,%d)",
		iItemCategory, iItemSelectNumber, ItemAttribute[iItemNumber].Name, iItemType,
		iItemIndex, iItemLevel, btSkillOption, btLuckOption, btAddOption, 
		ExOption[0], ExOption[1], ExOption[2], ExOption[3], ExOption[4], ExOption[5]);

	return iItemCategory;
}



void CCashLotterySystem::GetRequireInvenSize(int * piHeight, int * piWidth)
{
	*piHeight = this->iItemMaxHeight;
	*piWidth  = this->iItemMaxWidth;
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

