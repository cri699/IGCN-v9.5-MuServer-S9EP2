//////////////////////////////////////////////////////////////////////
// DoppelGangerItemBag.cpp
#include "stdafx.h"
#include "DoppelGangerItemBag.h"
#include "TLog.h"
#include "GameMain.h"
#include "LargeRand.h"
#include "configread.h"

CDoppelGangerItemBag::CDoppelGangerItemBag(void)
{
}

CDoppelGangerItemBag::~CDoppelGangerItemBag(void)
{
}

void CDoppelGangerItemBag::DoppelgangerItemBagInit()
{
	this->vtDPItemBag.clear();
}

void CDoppelGangerItemBag::LoadDoppelgangerItemBag(LPSTR lpFile)
{
	this->vtDPItemBag.clear();
	this->m_bLoad = FALSE;

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(lpFile);

	if ( res.status != pugi::status_ok )
	{
		g_Log.MsgBox("%s load fail (%s)", lpFile, res.description());
		return;
	}

	pugi::xml_node main = file.child("DoppelGanger");
	pugi::xml_node section_rates = main.child("SectionRateSettings");

	for (pugi::xml_node section = section_rates.child("Section"); section; section = section.next_sibling())
	{
		__tagDOPPELGANGER_DROP_ITEM_GROUP_INFO tmpItemGroupInfo;

		tmpItemGroupInfo.nGroupIndex = section.attribute("ID").as_int();
		tmpItemGroupInfo.nSkillRate = section.attribute("SkillRate").as_int();
		tmpItemGroupInfo.nLuckRate = section.attribute("LuckRate").as_int();
		tmpItemGroupInfo.nAddOptRate = section.attribute("OptionRate").as_int();
		tmpItemGroupInfo.nExOptRate = section.attribute("ExcRate").as_int();

		this->vtDPItemBag.insert(std::pair<int, __tagDOPPELGANGER_DROP_ITEM_GROUP_INFO>(tmpItemGroupInfo.nGroupIndex, tmpItemGroupInfo));
	}

	pugi::xml_node item_option = main.child("ItemOptionDrawSettings");
	int iCount = 0;

	for (pugi::xml_attribute rate = item_option.first_attribute(); rate; rate = rate.next_attribute())
	{
		if (iCount >= 7)
		{
			g_Log.MsgBox("Load Script Error %s", lpFile);
			break;
		}

		this->m_nAddOptRate[iCount] = rate.as_int();
		iCount++;
	}

	pugi::xml_node exc_option_count = main.child("ExcOptionCountDrawSettings");
	iCount = 0;

	for (pugi::xml_attribute rate = exc_option_count.first_attribute(); rate; rate = rate.next_attribute())
	{
		if (iCount >= 6)
		{
			g_Log.MsgBox("Load Script Error %s", lpFile);
			break;
		}

		this->m_nExItemAddOptRate[iCount] = rate.as_int();
		iCount++;
	}

	pugi::xml_node exc_option_type = main.child("ExcOptionTypeDrawSettings");
	iCount = 0;

	for (pugi::xml_attribute rate = exc_option_type.first_attribute(); rate; rate = rate.next_attribute())
	{
		if (iCount >= 6)
		{
			g_Log.MsgBox("Load Script Error %s", lpFile);
			break;
		}

		this->m_nExItemOptKindRate[iCount] = rate.as_int();
		iCount++;
	}

	for (pugi::xml_node item_section = main.child("Section"); item_section; item_section = item_section.next_sibling())
	{
		int iSectionID = item_section.attribute("ID").as_int();

		if (this->vtDPItemBag.find(iSectionID) == this->vtDPItemBag.end())
		{
			g_Log.MsgBox("Load Script Error (Section %d not found) %s", iSectionID, lpFile);
			break;
		}

		for (pugi::xml_node item = item_section.child("Item"); item; item = item.next_sibling())
		{
			CItemBagAttrEx tmpItemBagAttr;

			tmpItemBagAttr.m_type		= item.attribute("Cat").as_int();
			tmpItemBagAttr.m_index		= item.attribute("Index").as_int();
			tmpItemBagAttr.m_minLevel	= item.attribute("MinLevel").as_int();
			tmpItemBagAttr.m_maxLevel	= item.attribute("MaxLevel").as_int();
			tmpItemBagAttr.m_isskill	= item.attribute("Skill").as_int();
			tmpItemBagAttr.m_isluck		= item.attribute("Luck").as_int();
			tmpItemBagAttr.m_isoption	= item.attribute("Option").as_int();
			tmpItemBagAttr.m_isexitem	= item.attribute("Exc").as_int();

			this->vtDPItemBag[iSectionID].vtDropItemGroup.push_back(tmpItemBagAttr);
		}
	}

	this->m_bLoad = TRUE;
}

BOOL CDoppelGangerItemBag::DoppelgangerItemDrop(OBJECTSTRUCT *lpObj, int nItemBagIndex, int nDropRate, BYTE btFlag)
{
	if ( this->m_bLoad == FALSE )
	{
		return FALSE;
	}

	BOOL bItemDrop = TRUE;
	int nTmpItemBagIndex = nItemBagIndex;

	if ( this->vtDPItemBag.find(nTmpItemBagIndex) == this->vtDPItemBag.end() )
	{
		g_Log.Add("[Doppelganger] ItemBagIndex max over!!");
		return FALSE;
	}

	if ( (GetLargeRand() % 1000000) >= nDropRate )
	{
		bItemDrop = FALSE;
	}

	BYTE cDropX = lpObj->X;
	BYTE cDropY = lpObj->Y;
	
	if ( bItemDrop == FALSE )
	{
		int Count = rand()%3 + 2;

		for ( int i=0;i<Count;i++ )
		{
			cDropX = lpObj->X;
			cDropY = lpObj->Y;

			if ( !gObjGetRandomItemDropLocation(lpObj->MapNumber, cDropX, cDropY, 4, 4, 10) )
			{
				cDropX = lpObj->X;
				cDropY = lpObj->Y;
			}

			MapC[lpObj->MapNumber].MoneyItemDrop(10000, cDropX, cDropY);

			g_Log.Add("[Doppelganger] Drop Zen(%d) [%s][%s] [%d Zen]", btFlag, lpObj->AccountID, lpObj->Name, 10000);
		}
	}

	else
	{
		if ( !gObjGetRandomItemDropLocation(lpObj->MapNumber, cDropX, cDropY, 4, 4, 10) )
		{
			cDropX = lpObj->X;
			cDropY = lpObj->Y;
		}

		if ( this->vtDPItemBag[nTmpItemBagIndex].vtDropItemGroup.size() == 0 )
		{
			g_Log.Add("Error : DOPPELGANGER ItemBag is count 0");
			return FALSE;
		}

		int nItemNum = GetLargeRand() % this->vtDPItemBag[nTmpItemBagIndex].vtDropItemGroup.size();

		CItemBagAttrEx * pItemBagAttr = &this->vtDPItemBag[nTmpItemBagIndex].vtDropItemGroup[nItemNum];

		float fDur = 0.0;
		int iOption1 = 0;
		int iOption2 = 0;
		int iOption3 = 0;
		int iExOption = 0;
		int iLevel;

		if ( pItemBagAttr->m_maxLevel == pItemBagAttr->m_minLevel )
		{
			iLevel = pItemBagAttr->m_maxLevel;
		}

		else
		{
			iLevel = GetLargeRand() % (pItemBagAttr->m_maxLevel - pItemBagAttr->m_minLevel + 1) + pItemBagAttr->m_minLevel;
		}

		int iType = ItemGetNumberMake(pItemBagAttr->m_type, pItemBagAttr->m_index);

		if ( iType == -1 )
		{
			return FALSE;
		}

		if ( pItemBagAttr->m_isskill )
		{
			if ( GetLargeRand() % 1000000 < this->vtDPItemBag[nTmpItemBagIndex].nSkillRate )
			{
				iOption1 = TRUE;
			}
		}

		if ( pItemBagAttr->m_isluck )
		{
			if ( GetLargeRand() % 1000000 < this->vtDPItemBag[nTmpItemBagIndex].nLuckRate )
			{
				iOption2 = TRUE;
			}
		}

		if ( pItemBagAttr->m_isoption )
		{
			if (GetLargeRand() % 1000000 < this->vtDPItemBag[nTmpItemBagIndex].nAddOptRate)
			{
				DWORD dwOptionTemp = GetLargeRand() % 1000000;

				if (dwOptionTemp <= this->m_nAddOptRate[0])
				{
					iOption3 = 1;
				}

				else if (dwOptionTemp <= this->m_nAddOptRate[0] + this->m_nAddOptRate[1])
				{
					iOption3 = 2;
				}

				else if (dwOptionTemp <= this->m_nAddOptRate[0] + this->m_nAddOptRate[1] + this->m_nAddOptRate[2])
				{
					iOption3 = 3;
				}

				else if (dwOptionTemp <= this->m_nAddOptRate[0] + this->m_nAddOptRate[1] + this->m_nAddOptRate[2] + this->m_nAddOptRate[3])
				{
					iOption3 = 4;
				}

				else if (dwOptionTemp <= this->m_nAddOptRate[0] + this->m_nAddOptRate[1] + this->m_nAddOptRate[2] + this->m_nAddOptRate[3] + this->m_nAddOptRate[4])
				{
					iOption3 = 5;
				}

				else if (dwOptionTemp <= this->m_nAddOptRate[0] + this->m_nAddOptRate[1] + this->m_nAddOptRate[2] + this->m_nAddOptRate[3] + this->m_nAddOptRate[4] + this->m_nAddOptRate[5])
				{
					iOption3 = 6;
				}

				else if (dwOptionTemp <= this->m_nAddOptRate[0] + this->m_nAddOptRate[1] + this->m_nAddOptRate[2] + this->m_nAddOptRate[3] + this->m_nAddOptRate[4] + this->m_nAddOptRate[5] + this->m_nAddOptRate[6])
				{
					iOption3 = 7;
				}

				if (g_ConfigRead.data.common.Is28Opt == false && iOption3 > 4)
				{
					iOption3 = 4;
				}
			}
		}

		if ( pItemBagAttr->m_isexitem > 0 )
		{
			BOOL bExItemDrop = 0;

			if ( pItemBagAttr->m_isexitem == 2 )
				bExItemDrop = TRUE;

			else
			{
				if ( GetLargeRand() % 1000000 < this->vtDPItemBag[nTmpItemBagIndex].nExOptRate )
				{
					bExItemDrop = TRUE;
				}
			}

			if ( bExItemDrop == TRUE )
			{
				int nExOptionCount = 0;

				int nExOptAddRate = GetLargeRand() % 1000000;

				if ( nExOptAddRate >= this->m_nExItemAddOptRate[0] )
				{
					if (nExOptAddRate >= this->m_nExItemAddOptRate[1] + this->m_nExItemAddOptRate[0])
					{
						if (nExOptAddRate >= this->m_nExItemAddOptRate[2] + this->m_nExItemAddOptRate[1] + this->m_nExItemAddOptRate[0])
						{
							if (nExOptAddRate >= this->m_nExItemAddOptRate[3] + this->m_nExItemAddOptRate[2] + this->m_nExItemAddOptRate[1] + this->m_nExItemAddOptRate[0])
							{
								if (nExOptAddRate >= this->m_nExItemAddOptRate[4] + this->m_nExItemAddOptRate[3] + this->m_nExItemAddOptRate[2] + this->m_nExItemAddOptRate[1] + this->m_nExItemAddOptRate[0])
								{
									if (nExOptAddRate < this->m_nExItemAddOptRate[5] + this->m_nExItemAddOptRate[4] + this->m_nExItemAddOptRate[3] + this->m_nExItemAddOptRate[2] + this->m_nExItemAddOptRate[1] + this->m_nExItemAddOptRate[0])
									{
										nExOptionCount = 6;
									}
								}

								else
								{
									nExOptionCount = 5;
								}
							}

							else
							{
								nExOptionCount = 4;
							}
						}

						else
						{
							nExOptionCount = 3;
						}
					}

					else
					{
						nExOptionCount = 2;
					}
				}

				else
				{
					nExOptionCount = 1;
				}

				int LoopCnt = 50;

				while ( LoopCnt-- != 0 )
				{
					if ( nExOptionCount <= 0 )
						break;

					int nExOptIndex = rand()%6;

					if ( (iExOption & (1 << nExOptIndex)) != 1 << nExOptIndex && (GetLargeRand() % 1000000) < this->m_nExItemOptKindRate[nExOptIndex] )
					{
						iExOption |= 1 << nExOptIndex;
						--nExOptionCount;
					}
				}

				if ( iExOption )
				{
					iOption1 = 1;
					iOption2 = 2;
					iLevel = 0;
				}
			}
		}

		if ( iType == ITEMGET(12,15) || iType == ITEMGET(14,13) || iType == ITEMGET(14,14) || iType == ITEMGET(14,22) )
		{
			iOption1 = 0;
			iOption2 = 0;
			iOption3 = 0;
			iLevel = 0;
		}

		if ( iType == ITEMGET(13,0) || iType == ITEMGET(13,1) || iType == ITEMGET(13,2) ||
			iType == ITEMGET(13,8) || iType == ITEMGET(13,9) || iType == ITEMGET(13,12) ||
			iType == ITEMGET(13,13) || iType == ITEMGET(13,38) || iType == ITEMGET(13,39) ||
			iType == ITEMGET(13,40) || iType == ITEMGET(13,41) )
		{
			iLevel = 0;
			fDur = 255.0;
		}
			
		_stGremoryCaseItem stItem;
		stItem.btStorageType = GC_STORAGE_CHARACTER;
		stItem.btRewardSource = GC_REWARD_DOPPELGANGER;
		stItem.wItemID = iType;
		stItem.btItemLevel = iLevel;
		stItem.btItemDurability = fDur;
		stItem.btItemSkill = iOption1;
		stItem.btItemLuck = iOption2;
		stItem.btItemOption = iOption3;
		stItem.btItemExcOption = iExOption;

		g_GremoryCase.GDReqAddItemToGremoryCase(lpObj->m_Index, stItem, 30);

		g_Log.Add("[Doppelganger] Drop Item(%d) [%s][%s] : (%d)(%d/%d) Item:(%s)%d Level:%d op1:%d op2:%d op3:%d ExOp:%d",
			btFlag, lpObj->AccountID, lpObj->Name, lpObj->MapNumber, cDropX, cDropY, ItemAttribute[iType].Name, iType, iLevel, iOption1, iOption2, iOption3, iExOption);
	}
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

