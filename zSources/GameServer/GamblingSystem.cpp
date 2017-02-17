// GamblingSystem.cpp: implementation of the CGamblingItemBag class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GamblingSystem.h"
#include "user.h"
#include "LargeRand.h"
#include "DSProtocol.h"
#include "TLog.h"
#include "configread.h"

extern ITEM_ATTRIBUTE      ItemAttribute [];

CGamblingItemBag g_GamblingItemBag;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGamblingItemBag::CGamblingItemBag()
{

}

CGamblingItemBag::~CGamblingItemBag()
{

}

void CGamblingItemBag::GamblingItemBagInit(char *name)
{
	m_bLoad = FALSE;
	m_iBagObjectCount = 0;
	m_iRateKindCount = 0;

	int i;
	for(i = 0; i < 7; i++)	{	m_iAddOption[i]				= 0;	}
	for(i = 0; i < 5; i++)	{	m_iGamblingItemGetRate[i]	= 0;	}
	for(i = 0; i < 6; i++)	{										}
	for(i = 0; i < 6; i++)	{										}
	for(i = 0; i < 25; i++)	{	m_iDropRatePerItemCount[i]	= 0;	}
	
	LoadItem(name);
}

void CGamblingItemBag::LoadItem(char *script_file)
{
	m_bLoad = FALSE;
	m_iBagObjectCount = 0;

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(script_file);

	if (res.status != pugi::status_ok)
	{
		g_Log.Add("Error loading %s file (%s)", script_file, res.description());
		return;
	}

	pugi::xml_node main = file.child("GamblingSystem");

	this->m_bEnabled = main.attribute("Enable").as_bool();
	this->m_iGamblingValue = main.attribute("GamblingPrice").as_int();
	this->m_iGamblingShopTime = main.attribute("ShoppingDelayTime").as_int();

	int nDropRatePerItemCnt=0;
	int nRateCnt=0;
	int iCount=0;
	pugi::xml_node ratesettings = main.child("ItemRewardRateSettings");

	for (pugi::xml_node rate = ratesettings.child("Rate"); rate; rate = rate.next_sibling())
	{
		if( iCount >= 25 )
		{
			g_Log.MsgBox("Load Script Error %s",script_file);
			break;
		}

		m_stGamblingRate[iCount].iSkillRate = rate.attribute("Skill").as_int();
		m_stGamblingRate[iCount].iLuckRate = rate.attribute("Luck").as_int();
		m_stGamblingRate[iCount].iAddOptionRate = rate.attribute("Option").as_int();
		m_stGamblingRate[iCount].iExcellentRate = rate.attribute("Exc").as_int();
		m_stGamblingRate[iCount].iSelectedRate = rate.attribute("ItemChance").as_int();
		iCount++;
	}

	iCount = 0;
	pugi::xml_node optionchance = main.child("ItemOptionChance");

	for (pugi::xml_attribute option = optionchance.first_attribute(); option; option = option.next_attribute())
	{
		if( iCount >= 7 )
		{
			g_Log.MsgBox("Load Script Error %s",script_file);
			break;
		}

		m_iAddOption[iCount] = option.as_int();
		iCount++;
	}

	iCount = 0;
	pugi::xml_node rareitemchance = main.child("RareItemRewardChance");

	for (pugi::xml_attribute rareitem = rareitemchance.first_attribute(); rareitem; rareitem = rareitem.next_attribute())
	{
		if( iCount >= 5 )
		{
			g_Log.MsgBox("Load Script Error %s",script_file);
			break;
		}

		m_iGamblingItemGetRate[iCount] = rareitem.as_int();
		iCount++;
	}

	iCount = 0;
	pugi::xml_node excoptionchance = main.child("ExcOptionCountChance");

	for (pugi::xml_attribute exc = excoptionchance.first_attribute(); exc; exc = exc.next_attribute())
	{
		if( iCount >= 6 )
		{
			g_Log.MsgBox("Load Script Error %s",script_file);
			break;
		}

		m_iGamblingOptionCountRate[iCount] = exc.as_int();
		iCount++;
	}

	iCount = 0;
	pugi::xml_node excoptselectrate = main.child("ExcOptionSelectionRate");

	for (pugi::xml_node excrate = excoptselectrate.child("Rate"); excrate; excrate = excrate.next_sibling())
	{
		if( iCount >= 6 )
		{
			g_Log.MsgBox("Load Script Error %s",script_file);
			break;
		}

		m_iGamblingExOptionRate[iCount] = excrate.attribute("RareItem").as_int();
		m_iNormalExOptionRate[iCount] = excrate.attribute("OtherItems").as_int();
		iCount++;
	}

	nDropRatePerItemCnt = 0;
	pugi::xml_node itemrewardlist = main.child("ItemRewardList");

	for (pugi::xml_node section = itemrewardlist.child("Section"); section; section = section.next_sibling())
	{
		nDropRatePerItemCnt = 0;

		for (pugi::xml_node item = section.child("Item"); item; item = item.next_sibling())
		{
			BagObject[m_iBagObjectCount].m_type = item.attribute("Cat").as_int();	
			BagObject[m_iBagObjectCount].m_index = item.attribute("Index").as_int();
			BagObject[m_iBagObjectCount].m_minLevel = item.attribute("MinLevel").as_int();
			BagObject[m_iBagObjectCount].m_maxLevel = item.attribute("MaxLevel").as_int();
			BagObject[m_iBagObjectCount].m_isskill = item.attribute("Skill").as_int();
			BagObject[m_iBagObjectCount].m_isluck = item.attribute("Luck").as_int();
			BagObject[m_iBagObjectCount].m_isoption = item.attribute("Option").as_int();
			BagObject[m_iBagObjectCount].m_isexitem = item.attribute("Exc").as_int();

			if( BagObject[m_iBagObjectCount].m_minLevel > 
				BagObject[m_iBagObjectCount].m_maxLevel )
			{
				g_Log.MsgBox("Load Script Error %s",script_file);
				return;
			}
					
			nDropRatePerItemCnt++;
			m_iBagObjectCount++;

			if(m_iBagObjectCount > 150-1 )	break;
		}

		m_iDropRatePerItemCount[nRateCnt] = nDropRatePerItemCnt;
		nRateCnt++;

		if(nRateCnt > 24)	break;

		m_iRateKindCount = nRateCnt;
	}

	g_Log.Add("%s loaded successfully", script_file);
	m_bLoad = TRUE;
}

BYTE CGamblingItemBag::GetLevel(int n)
{
	if( n <0 || n > 150-1 )	return 0;
	if( BagObject[n].m_minLevel == BagObject[n].m_maxLevel ) {
		return BagObject[n].m_minLevel;
	}
	int sub = (BagObject[n].m_maxLevel-BagObject[n].m_minLevel)+1;
	int level = BagObject[n].m_minLevel+(rand()%sub);
	return level;
}

BYTE CGamblingItemBag::GetGamblingSystemItem(int aIndex, BYTE btMapNumber, BYTE cX, BYTE cY, BYTE btItemType)
{
	if(!this->m_bEnabled)	return 0;
	if(!m_bLoad)	return 0;

	float fDur = 0;
	int iType = 0;
	int iLevel = 0;
	int X = 0;
	int Y = 0;
	int iOption1 = 0;
	int iOption2 = 0;
	int iOption3 = 0;
	int iDropItemNum = 0;
	int iDropItemRate = 0;
	int iExOption = 0;
	LPOBJ lpObj = &gObj[aIndex];

	//LogAdd("GetGamblingSystemItem::GetBagCount -> %d", GetBagCount());

	if(GetBagCount() > 0)
	{
		if(m_iGamblingItemGetRate[btItemType] >= GetLargeRand() % 1000000)
		{
			iLevel = 0;
			fDur = 0;
			iOption1 = 1;
			iOption2 = 0;

			DWORD dwOptionTemp = GetLargeRand() % 1000000;

			//LogAdd("Option Rate MAx: %d", m_iAddOption[0] + m_iAddOption[1] + m_iAddOption[2] + m_iAddOption[3]);

			if( dwOptionTemp <= m_iAddOption[0])
			{
				iOption3 = 1;
			}

			else if( dwOptionTemp <= m_iAddOption[0] + m_iAddOption[1])
			{
				iOption3 = 2;
			}

			else if( dwOptionTemp <= m_iAddOption[0] + m_iAddOption[1] + m_iAddOption[2])
			{
				iOption3 = 3;
			}

			else if( dwOptionTemp <= m_iAddOption[0] + m_iAddOption[1] + m_iAddOption[2] + m_iAddOption[3])
			{
				iOption3 = 4;
			}

			else if( dwOptionTemp <= m_iAddOption[0] + m_iAddOption[1] + m_iAddOption[2] + m_iAddOption[3] + m_iAddOption[4])
			{
				iOption3 = 5;
			}

			else if( dwOptionTemp <= m_iAddOption[0] + m_iAddOption[1] + m_iAddOption[2] + m_iAddOption[3] + m_iAddOption[4] + m_iAddOption[5])
			{
				iOption3 = 6;
			}

			else if( dwOptionTemp <= m_iAddOption[0] + m_iAddOption[1] + m_iAddOption[2] + m_iAddOption[3] + m_iAddOption[4] + m_iAddOption[5] + m_iAddOption[6])
			{
				iOption3 = 7;
			}

			if ( g_ConfigRead.data.common.Is28Opt == false && iOption3 > 4 )
			{
				iOption3 = 4;
			}

			DWORD dwExOptionTemp = GetLargeRand() % 1000000;

			//LogAdd("Option Exc Rate MAx: %d", m_iGamblingOptionCountRate[0] + m_iGamblingOptionCountRate[1] + m_iGamblingOptionCountRate[2] + m_iGamblingOptionCountRate[3]);

			if( dwExOptionTemp <= m_iGamblingOptionCountRate[0])
			{
				iExOption = GamblingOptionRand(1);
			}

			else if( dwExOptionTemp <= m_iGamblingOptionCountRate[0] + m_iGamblingOptionCountRate[1])
			{
				iExOption = GamblingOptionRand(2);
			}

			else if( dwExOptionTemp <= m_iGamblingOptionCountRate[0] + m_iGamblingOptionCountRate[1] + m_iGamblingOptionCountRate[2])
			{
				iExOption = GamblingOptionRand(3);
			}

			else if( dwExOptionTemp <= m_iGamblingOptionCountRate[0] + m_iGamblingOptionCountRate[1] + m_iGamblingOptionCountRate[2] + m_iGamblingOptionCountRate[3])
			{
				iExOption = GamblingOptionRand(4);
			}

			else if( dwExOptionTemp <= m_iGamblingOptionCountRate[0] + m_iGamblingOptionCountRate[1] + m_iGamblingOptionCountRate[2] + m_iGamblingOptionCountRate[3] + m_iGamblingOptionCountRate[4])
			{
				iExOption = GamblingOptionRand(5);
			}

			else if( dwExOptionTemp <= m_iGamblingOptionCountRate[0] + m_iGamblingOptionCountRate[1] + m_iGamblingOptionCountRate[2] + m_iGamblingOptionCountRate[3] + m_iGamblingOptionCountRate[4] + m_iGamblingOptionCountRate[5])
			{
				iExOption = GamblingOptionRand(6);
			}

			switch ( btItemType ) //5 Types of Special Item
			{
				case 0:
					iType = ItemGetNumberMake(3, 11);
				break;
				case 1:
					iType = ItemGetNumberMake(5, 33);
				break;
				case 2:
					iType = ItemGetNumberMake(4, 24);
				break;
				case 3:
					iType = ItemGetNumberMake(2, 18);
				break;
				case 4:
					iType = ItemGetNumberMake(5, 34);
				break;
			}
		}
		else //else for common items like a bag
		{
			int iCount = 0;
			int iBagPositionStart = 0;
			int iBagPositionEnd = 0;
			int iSelectedGroup = 0;

			switch( btItemType )
			{
				case 0:
					iSelectedGroup = 0;
					break;
				case 1:
					iSelectedGroup = 5;
					break;
				case 2:
					iSelectedGroup = 10;
					break;
				case 3:
					iSelectedGroup = 15;
					break;
				case 4:
					iSelectedGroup = 20;
					break;
			}

			DWORD dwRandTemp = GetLargeRand() % 1000000;
			
			int iSelectedRate = 0;

			for( int i=iSelectedGroup; i<iSelectedGroup+5; i++)
			{
				iSelectedRate += m_stGamblingRate[i].iSelectedRate;
				if(dwRandTemp <= iSelectedRate)
				{
					iSelectedGroup = i;
					break;
				}
			}
			
			while(true) //Max 25
			{
				iBagPositionEnd += m_iDropRatePerItemCount[iCount];
				
				if(iSelectedGroup <= iCount)	break;
				
				iBagPositionStart += m_iDropRatePerItemCount[iCount];
				iCount++;
			}

			iDropItemNum = (GetLargeRand() % (iBagPositionEnd - iBagPositionStart)) + iBagPositionStart;
		
			fDur = 0;
			X = 0;
			Y = 0;

			iLevel = GetLevel(iDropItemNum);

			iType = ItemGetNumberMake(BagObject[iDropItemNum].m_type,BagObject[iDropItemNum].m_index);

			if(iType == -1)	return 0;

			if( BagObject[iDropItemNum].m_isskill != 0)
			{
				if(m_stGamblingRate[iSelectedGroup].iSkillRate >= GetLargeRand()%1000000)
				{
					iOption1 = 1;
				}
			}

			if(BagObject[iDropItemNum].m_isluck != 0)
			{
				if(m_stGamblingRate[iSelectedGroup].iLuckRate >= GetLargeRand()%1000000)
				{
					iOption2 = 1;
				}
			}

			if(BagObject[iDropItemNum].m_isoption != 0)
			{
				if(m_stGamblingRate[iSelectedGroup].iAddOptionRate >= GetLargeRand()%1000000)
				{
					int iOptionTemp = GetLargeRand() % 1000000;

					if( iOptionTemp <= m_iAddOption[0])
					{
						iOption3 = 1;
					}

					else if( iOptionTemp <= m_iAddOption[0] + m_iAddOption[1])
					{
						iOption3 = 2;
					}

					else if( iOptionTemp <= m_iAddOption[0] + m_iAddOption[1] + m_iAddOption[2])
					{
						iOption3 = 3;
					}

					else if( iOptionTemp <= m_iAddOption[0] + m_iAddOption[1] + m_iAddOption[2] + m_iAddOption[3])
					{
						iOption3 = 4;
					}

					else if( iOptionTemp <= m_iAddOption[0] + m_iAddOption[1] + m_iAddOption[2] + m_iAddOption[3] + m_iAddOption[4])
					{
						iOption3 = 5;
					}
				
					else if( iOptionTemp <= m_iAddOption[0] + m_iAddOption[1] + m_iAddOption[2] + m_iAddOption[3] + m_iAddOption[4] + m_iAddOption[5])
					{
						iOption3 = 6;
					}

					else if( iOptionTemp <= m_iAddOption[0] + m_iAddOption[1] + m_iAddOption[2] + m_iAddOption[3] + m_iAddOption[4] + m_iAddOption[5] + m_iAddOption[6])
					{
						iOption3 = 7;
					}

					if ( g_ConfigRead.data.common.Is28Opt == false )
					{
						iOption3 = 4;
					}
				}
			}
			
			if(BagObject[iDropItemNum].m_isexitem != 0)
			{
				if(m_stGamblingRate[iSelectedGroup].iExcellentRate >= GetLargeRand()%1000000)
				{
					iExOption = ExcellentOptionRand();
					iOption2 = 0;
					iOption1 = 1;
					iLevel = 0;	
				}
			}
		}

		ItemSerialCreateSend(lpObj->m_Index,btMapNumber,X,Y,iType,iLevel,fDur,iOption1,iOption2,iOption3,lpObj->m_Index,iExOption,0, 0, 0, 0);
		g_Log.Add("[ GAMBLING ][ TryGambling ] %s(%s) Try Item : [%s]%d Level:%d op1:%d op2:%d op3:%d ExOp:%d",lpObj->AccountID,lpObj->Name,ItemAttribute[iType].Name,iType,iLevel,iOption1,iOption2,iOption3,iExOption);
	}
	return 1;
}

BYTE CGamblingItemBag::GamblingOptionRand(int iOptionCount)
{
	BYTE NOption = 0;
	int iFailCount = 0;
	int iCount = 0;

	while(true)
	{
		if(m_iGamblingExOptionRate[iCount] >= GetLargeRand() % 1000000) {
			NOption |= 1 << iCount;
		}
		else iFailCount++;

		if(iFailCount >= 6-iOptionCount)
		{
			while(true)
			{
				iCount++;
				if(iCount >= 6)	break;

				NOption |= 1<<iCount;
			}
			break;
		}
		iCount++;
	}

	return NOption;
}

BYTE CGamblingItemBag::ExcellentOptionRand()
{
	BYTE NOption = 0;
	int iCount = 0;
	int iSuccessCount = 0;

	while(true)
	{
		if(m_iNormalExOptionRate[iCount] >= GetLargeRand() % 1000000)
		{
			NOption |= 1<<iCount;
			iSuccessCount++;
		}
		iCount++;

		if(iSuccessCount == 0)
		{
			if(iCount >= 6)	iCount = 0;
		}
		else if(iSuccessCount == 1)
		{
			if(iCount >= 6)	break;
		}
		else if(iSuccessCount >= 2)	break;
	}
	return NOption;
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

