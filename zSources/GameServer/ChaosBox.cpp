//////////////////////////////////////////////////////////////////////
// ChaosBox.cpp
#include "stdafx.h"
#include "ChaosBox.h"
#include "TLog.h"
#include "GameMain.h"
#include "winutil.h"
#include "DSProtocol.h"
#include "CastleSiegeSync.h"
#include "BloodCastle.h"
#include "DevilSquare.h"
#include "SetItemOption.h"
#include "JewelOfHarmonySystem.h"
#include "giocp.h"
#include "CrywolfSync.h"
#include "CashLotterySystem.h"
#include "configread.h"
#include "ImperialGuardian.h"
#include "BagManager.h"
#include "LuckyItemManager.h"
#include "ItemSocketOptionSystem.h"
#include "PentagramSystem.h"
#include "VipSys.h"

CMixSystem g_MixSystem;

CMixSystem::CMixSystem()
{
	this->CHAOS_GEM_ITEMINDEX = ITEMGET(12, 15);
	this->BLESS_GEM_ITEMINDEX = ITEMGET(14, 13);
	this->SOUL_GEM_ITEMINDEX = ITEMGET(14, 14);
	this->CREATURE_GEM_ITEMINDEX = ITEMGET(14, 22);
	this->BLESS_GEM_BUNDLE_ITEMINDEX = ITEMGET(12, 30);
	this->SOUL_GEM_BULDLE_ITEMINDEX = ITEMGET(12, 31);
	this->CONDOR_FEATHER_ITEMINDEX = ITEMGET(13, 53);
	this->CONDOR_FLAME_ITEMINDEX = ITEMGET(13, 52);
	this->WING_OF_STORM_ITEMINDEX = ITEMGET(12, 36);
	this->WING_OF_ETERNAL_ITEMINDEX = ITEMGET(12, 37);
	this->WING_OF_ILLUSION_ITEMINDEX = ITEMGET(12, 38);
	this->WING_OF_RUIN_ITEMINDEX = ITEMGET(12, 39);
	this->CAPE_OF_EMPEROR_ITEMINDEX = ITEMGET(12, 40);
	this->WING_OF_DIMENSION_ITEMINDEX = ITEMGET(12, 43);
	this->CAPE_OF_OVERRULE_ITEMINDEX = ITEMGET(12, 50);
	this->CLOAK_OF_TRANSCEDENCE_ITEMINDEX = ITEMGET(12, 270);
	this->FRUIT_ITEMINDEX = ITEMGET(13, 15);
	this->DINORANT_ITEMINDEX = ITEMGET(13, 3);
	this->BLOODCASTLE_INVITATION_ITEMINDEX = ITEMGET(13, 38);
	this->DEVILSQUARE_INVITATION_ITEMINDEX = ITEMGET(14, 19);
	this->SMALL_SD_POTION_ITEMINDEX = ITEMGET(14, 35);
	this->MEDIUM_SD_POTION_ITEMINDEX = ITEMGET(14, 36);
	this->LARGE_SD_POTION_ITEMINDEX = ITEMGET(14, 37);
	this->BLESS_POTION_ITEMINDEX = ITEMGET(14, 7);
	this->SOUL_POTION_ITEMINDEX = ITEMGET(14, 7);
	this->GOLDEN_BOX_ITEMINDEX = ITEMGET(14, 123);
	this->SILVER_BOX_ITEMINDEX = ITEMGET(14, 124);
	this->HIGH_REFINE_STONE_ITEMINDEX = ITEMGET(14, 44);
	this->LOW_REFINE_STONE_ITEMINDEX = ITEMGET(14, 43);
	this->HARMONY_JEWEL_ITEMINDEX = ITEMGET(14, 42);
	this->m_i3rdWingOpt_LuckRate = 50;
}

CMixSystem::~CMixSystem()
{

}

BOOL CMixSystem::ChaosBoxCheck(LPOBJ lpObj)
{
	if ( lpObj->pChaosBox == NULL )
	{
		return false;
	}

	return true;
}

BOOL CMixSystem::ChaosBoxInit(LPOBJ lpObj)
{
	if ( lpObj->pChaosBox == NULL )
	{
		return false;
	}

	if ( lpObj->pChaosBoxMap == NULL )
	{
		return false;
	}

	for (int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		lpObj->pChaosBox[n].Clear();
	}

	memset(lpObj->pChaosBoxMap, (BYTE)-1, CHAOS_BOX_MAP_SIZE);

	return true;
}


BOOL CMixSystem::ChaosBoxItemDown(LPOBJ lpObj)
{
	if ( lpObj->pChaosBox == NULL )
	{
		return FALSE;
	}

	for (int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if (lpObj->pChaosBox[n].m_Type == ITEMGET(12, 15) || lpObj->pChaosBox[n].m_Type == ITEMGET(14, 13) || lpObj->pChaosBox[n].m_Type == ITEMGET(14, 14) ||
			lpObj->pChaosBox[n].m_Type == ITEMGET(13, 83) || lpObj->pChaosBox[n].m_Type == ITEMGET(13, 84) || lpObj->pChaosBox[n].m_Type == ITEMGET(13, 85) ||
			lpObj->pChaosBox[n].m_Type == ITEMGET(13, 86) || lpObj->pChaosBox[n].m_Type == ITEMGET(13, 87) || lpObj->pChaosBox[n].m_Type == ITEMGET(13, 88) ||
			lpObj->pChaosBox[n].m_Type == ITEMGET(13, 89) || lpObj->pChaosBox[n].m_Type == ITEMGET(13, 90) || lpObj->pChaosBox[n].m_Type == ITEMGET(13, 91) ||
			lpObj->pChaosBox[n].m_Type == ITEMGET(13, 92) || lpObj->pChaosBox[n].m_Type == ITEMGET(14, 53) || lpObj->pChaosBox[n].m_Type == ITEMGET(14, 103) ||
			lpObj->pChaosBox[n].m_Type == ITEMGET(14, 103) || lpObj->pChaosBox[n].m_Type == ITEMGET(14, 104) || lpObj->pChaosBox[n].m_Type == ITEMGET(14, 105) ||
			lpObj->pChaosBox[n].m_Type == ITEMGET(14, 106) || lpObj->pChaosBox[n].m_Type == ITEMGET(14, 107) || lpObj->pChaosBox[n].m_Type == ITEMGET(14, 108))
		{
			lpObj->pChaosBox[n].Clear();
		}
		else
		{
			if(lpObj->pChaosBox[n].IsItem() == FALSE)
				continue;

			int op = lpObj->pChaosBox[n].m_Option1;

			if ( op > 0 )
			{
				if ( (rand() % 2) == 0 )
				{
					op--;
				}
			}

			lpObj->pChaosBox[n].m_Option1 = op;
			op = lpObj->pChaosBox[n].m_Option3;

			if ( op > 0 )
			{
				if ( (rand() % 2) == 0 )
				{
					op--;
				}
			}

			lpObj->pChaosBox[n].m_Option3 = op;

			if ( lpObj->pChaosBox[n].m_Level > 0 )
			{
				lpObj->pChaosBox[n].m_Level = rand() % lpObj->pChaosBox[n].m_Level;
			}

			float dur = ItemGetDurability(lpObj->pChaosBox[n].m_Type, lpObj->pChaosBox[n].m_Level, lpObj->pChaosBox[n].IsExtItem(), lpObj->pChaosBox[n].IsSetItem());
			
			lpObj->pChaosBox[n].m_Durability =  dur * lpObj->pChaosBox[n].m_Durability / lpObj->pChaosBox[n].m_BaseDurability;
			lpObj->pChaosBox[n].Convert(lpObj->pChaosBox[n].m_Type, lpObj->pChaosBox[n].m_Option1,
				lpObj->pChaosBox[n].m_Option2, lpObj->pChaosBox[n].m_Option3, lpObj->pChaosBox[n].m_NewOption,
				lpObj->pChaosBox[n].m_SetOption, lpObj->pChaosBox[n].m_ItemOptionEx, 0, -1, 0, CURRENT_DB_VERSION);
		}
	}

	return TRUE;
}

int CMixSystem::ChaosBoxMix(LPOBJ lpObj, int & Result2, int & WingNum)
{
	BYTE ExOption[8];
	int ChaosDiamond = 0;
	int ChaosItems = 0;

	if ( lpObj->pChaosBox == NULL )
	{
		return 0;
	}

	int value = 0;
	int add = 0;
	int nv = 0;
	Result2 = 0;
	int WingCharmItemNum = -1; //season4.5 add-on
	lpObj->ChaosSuccessRate = 0;
	lpObj->ChaosMoney = 0;
	int iCharmOfLuckCount = 0;
	int iCharmOfWing = 0;
	WingNum = -1;

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			lpObj->pChaosBox[n].OldValue();
			add = 0;
			nv = 0;

			if (g_LuckyItemManager.IsLuckyItemEquipment(lpObj->pChaosBox[n].m_Type))	//1.01.00
			{
				return false;
			}

			if ( lpObj->pChaosBox[n].m_Level >= MIN_CHAOS_ITEM_LEVEL && (lpObj->pChaosBox[n].m_Option3 *4) >= MIN_CHAOS_ITEM_LEVEL )
			{
				nv = lpObj->pChaosBox[n].m_OldBuyMoney;
				value += lpObj->pChaosBox[n].m_OldBuyMoney;
				add = 1;

				if ( lpObj->pChaosBox[n].m_Type == ITEMGET(2,6) || lpObj->pChaosBox[n].m_Type == ITEMGET(4,6) || lpObj->pChaosBox[n].m_Type == ITEMGET(5,7) )	// Chaos Items
				{
					Result2 = 1;
				}
			}

			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(12,15) )
			{
				value += lpObj->pChaosBox[n].m_OldBuyMoney;
				nv = lpObj->pChaosBox[n].m_OldBuyMoney;
				ChaosDiamond++;
				add = 1;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,13) || lpObj->pChaosBox[n].m_Type == ITEMGET(14,14) )	// Jewel of Bless, Jewel of Soul
			{
				value += lpObj->pChaosBox[n].m_OldBuyMoney;
				nv = lpObj->pChaosBox[n].m_OldBuyMoney;
				add = 1;
			}

			//season4.5 add-on
			if (lpObj->pChaosBox[n].m_Type == ITEMGET(13, 83) ||
				lpObj->pChaosBox[n].m_Type == ITEMGET(13, 84) ||
				lpObj->pChaosBox[n].m_Type == ITEMGET(13, 85) ||
				lpObj->pChaosBox[n].m_Type == ITEMGET(13, 86) ||
				lpObj->pChaosBox[n].m_Type == ITEMGET(13, 87))
			{
				WingCharmItemNum = lpObj->pChaosBox[n].m_Type;
			}

			if (lpObj->pChaosBox[n].m_Type == ITEMGET(13,14)) //updated 1.01.00
			{
				g_Log.Add("[%s][%s] Fail to mix because of Item (13,14). It can't be in default ChaosMix", lpObj->AccountID, lpObj->Name);
				return false;
			}

			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,53) )	// Charm Of Luck
			{
				iCharmOfLuckCount += lpObj->pChaosBox[n].m_Durability;
			}

			if ( add != 0  )
			{
				ChaosItems++;
			}

			ItemIsBufExOption(ExOption, &lpObj->pChaosBox[n]);
			g_Log.Add("[%s][%s] CBMix [%d,%s,%d,%d,%d,%d]serial:[%I64d][%d][%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d]",
				lpObj->AccountID, lpObj->Name, n, lpObj->pChaosBox[n].GetName(),
				lpObj->pChaosBox[n].m_Level, lpObj->pChaosBox[n].m_Option1,
				lpObj->pChaosBox[n].m_Option2, lpObj->pChaosBox[n].m_Option3,
				lpObj->pChaosBox[n].m_Number, (int)lpObj->pChaosBox[n].m_Durability,
				nv, ExOption[0], ExOption[1], ExOption[2], ExOption[3], ExOption[4], ExOption[5],
				ExOption[6], lpObj->pChaosBox[n].m_SetOption);
		}
	}

	if ( ChaosDiamond == 0 )
	{
		value = 0;	// 0% of success
	}

	if ( ChaosItems < 2 )
	{
		value = 0;// 0% of success
	}

	if ( iCharmOfLuckCount > 10 )
		return FALSE;
	
	lpObj->ChaosSuccessRate = value / 20000;
	lpObj->ChaosSuccessRate += iCharmOfLuckCount;

	if ( lpObj->ChaosSuccessRate  > 100 )
	{
		lpObj->ChaosSuccessRate = 100;
	}

	lpObj->ChaosMoney = lpObj->ChaosSuccessRate * 10000;	// Required Money to MIX anc createe a Chaos Item

	g_Log.Add("[%s][%s] CBMix need Zen : %d SuccessRate : %d, CharmRate : %d",
		lpObj->AccountID, lpObj->Name, lpObj->ChaosMoney, 
		lpObj->ChaosSuccessRate, iCharmOfLuckCount);

	if (WingCharmItemNum > -1) //season4.5 add-on
	{
		int WingCharmIndex = ITEM_GET_TYPE(WingCharmItemNum);

		switch (WingCharmIndex)
		{
		case 83:
			WingNum = 2;
			break;
		case 84:
			WingNum = 1;
			break;
		case 85:
			WingNum = 0;
			break;
		case 86:
			WingNum = 41;
			break;
		case 87:
			WingNum = 30;
			break;
		default:
			g_Log.Add("[MixSystem] WingCharmItemNum => %d , WingCharmIndex => %d", WingCharmItemNum, WingCharmIndex);
			break;
		}
	}

	return value;
}

void CMixSystem::DefaultChaosMix(LPOBJ lpObj)
{
	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;
	int aIndex = lpObj->m_Index;
	BOOL fail = TRUE;
	int MixResult2;
	int WingNum;
	lpObj->ChaosLock = TRUE;
	int WingCharmIndex = -1;

	if (this->ChaosBoxMix(lpObj, MixResult2, WingCharmIndex) == 0)
	{
		pMsg.Result = CB_INCORRECT_MIX_ITEMS;
		IOCP.DataSend(aIndex, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	int iChaosTaxMoney = (int)((__int64)lpObj->ChaosMoney * (__int64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (__int64)100);

	if (iChaosTaxMoney < 0)
	{
		iChaosTaxMoney = 0;
	}

	lpObj->ChaosMoney += iChaosTaxMoney;

	if (lpObj->ChaosMoney < 0)
	{
		lpObj->ChaosMoney = 0;
	}

	if (lpObj->m_PlayerData->Money < lpObj->ChaosMoney)
	{
		pMsg.Result = CB_NOT_ENOUGH_ZEN;

		IOCP.DataSend(aIndex, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

	}
	else
	{
		if (lpObj->ChaosSuccessRate > 0)
		{
			if (lpObj->ChaosSuccessRate >= 100)
			{
				lpObj->ChaosSuccessRate = 100;
			}

			if ((rand() % 100) <= (lpObj->ChaosSuccessRate - 1))
			{
				fail = FALSE;
				pMsg.Result = CB_SUCCESS;

				int Level = rand() % 5;
				int Option1 = 0;
				int Option2 = 0;
				int Option3 = 0;

				if ((rand() % 100)< (lpObj->ChaosSuccessRate / 5 + 6))
				{
					Option1 = 1;
				}

				if ((rand() % 100)< (lpObj->ChaosSuccessRate / 5 + 4))
				{
					Option2 = 1;
				}

				int OpProb;
				int OpType = rand() % 3;
				OpProb = rand() % 100;

				switch (OpType)
				{
				case 0:
					if (OpProb < (lpObj->ChaosSuccessRate / 5 + 4))
					{
						Option3 = 3;
					}
					break;

				case 1:
					if (OpProb < (lpObj->ChaosSuccessRate / 5 + 8))
					{
						Option3 = 2;
					}
					break;

				case 2:
					if (OpProb < (lpObj->ChaosSuccessRate / 5 + 12))
					{
						Option3 = 1;
					}
					break;
				}

				if (MixResult2 == TRUE)
				{
					int WingType = rand() % 4;
					int WingNum = 0;

					if (WingCharmIndex > -1) //season4.5 add-on
					{
						WingNum = WingCharmIndex + ITEMGET(12, 0);
					}
					else if (WingType == 0)
					{
						WingNum = ITEMGET(12, 0);
					}
					else if (WingType == 1)
					{
						WingNum = ITEMGET(12, 1);
					}
					else if (WingType == 2)
					{
						WingNum = ITEMGET(12, 2);
					}
					else if (WingType == 3) //season3 add-on
					{
						WingNum = ITEMGET(12, 41);
					}

					::ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, WingNum, 0, 255, Option1, Option2, Option3, -1, 0, 0, 0, 0, 0);
				}
				else
				{
					int ChaosItemType = rand() % 3;
					int ChaosItemNum = 0;

					if (ChaosItemType == 0)	// Chaos Dragon Axe
					{
						ChaosItemNum = ITEMGET(2, 6);
					}
					else if (ChaosItemType == 1)	// Chaos Nature Bow
					{
						ChaosItemNum = ITEMGET(4, 6);
					}
					else if (ChaosItemType == 2)	// Chaos Lighting Staff
					{
						ChaosItemNum = ITEMGET(5, 7);
					}

					::ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, ChaosItemNum, Level, 255, Option1, Option2, Option3, -1, 0, 0, 0, 0, 0);
				}
			}
		}

		lpObj->m_PlayerData->Money -= lpObj->ChaosMoney;
		g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
		GSProtocol.GCMoneySend(lpObj->m_Index, lpObj->m_PlayerData->Money);

		if (fail == TRUE)
		{
			ChaosBoxItemDown(lpObj);
			GSProtocol.GCUserChaosBoxSend(lpObj, 0);
			IOCP.DataSend(aIndex, (BYTE*)&pMsg, pMsg.h.size);
			g_Log.Add("[%s][%s] CBMix Fail %d Money : %d-%d",
				lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate,
				lpObj->m_PlayerData->Money, lpObj->ChaosMoney);
			lpObj->ChaosLock = FALSE;
		}
		else
		{
			g_Log.Add("[%s][%s] CBMix Success Rate:%d Money : %d",
				lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate,
				lpObj->ChaosMoney);
		}
	}

	::gObjInventoryCommit(lpObj->m_Index);
}

int CMixSystem::CheckDevilSquareItem(LPOBJ lpObj, int & eventitemcount, int & itemlevel)
{
	BOOL bChaoseGem = FALSE;
	BOOL bEyeOfDevil = FALSE;
	BOOL bKeyOfDevil = FALSE;
	eventitemcount = 0;
	BOOL FoundOtherItem = FALSE;
	int level = -1;
	BOOL bLevelCheck = FALSE;
	int iCharmOfLuckCount = 0;
	BOOL bCharmOfLuckOver = FALSE;

	g_Log.Add("[DevilSquare] Search DevilSquareItem");

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(12,15) )	// Chaos
			{
				bChaoseGem = TRUE;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,17) )	// Eye
			{
				eventitemcount +=1;
				bEyeOfDevil = TRUE;

				if ( level != lpObj->pChaosBox[n].m_Level )
				{
					if ( level == -1 )
					{
						level = lpObj->pChaosBox[n].m_Level;
					}
					else
					{
						bLevelCheck = TRUE;
					}
				}
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,18) )	// Key
			{
				eventitemcount +=1;
				bKeyOfDevil = TRUE;

				if ( level != lpObj->pChaosBox[n].m_Level )
				{
					if ( level == -1 )
					{
						level = lpObj->pChaosBox[n].m_Level;
					}
					else
					{
						bLevelCheck = TRUE;
					}
				}
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,53) )	// Charm Of Luck
			{
				iCharmOfLuckCount += lpObj->pChaosBox[n].m_Durability;

				if ( iCharmOfLuckCount > 10 )
				{
					bCharmOfLuckOver = TRUE;
				}
			}
			else
			{
				FoundOtherItem = TRUE;
			}
		}
	}

	itemlevel = level;
	lpObj->ChaosSuccessRate = iCharmOfLuckCount;

	if ( FoundOtherItem != FALSE )
	{
		g_Log.Add("[DevilSquare] Other DQItems Found");
		return 0;
	}

	if ( bLevelCheck != FALSE )
	{
		g_Log.Add("[DevilSquare] DiffLevel DQItems Found");
		return 3;
	}

	if ( bCharmOfLuckOver == TRUE )
	{
		g_Log.Add("[DevilSquare] Charm Of Luck Count Over");
		return 4;
	}

	if ( bChaoseGem != FALSE && bEyeOfDevil != FALSE && bKeyOfDevil != FALSE )
	{
		LogDQChaosItem(lpObj);
		return 1;
	}

	if ( bEyeOfDevil != FALSE && bKeyOfDevil != FALSE )
	{
		LogDQChaosItem(lpObj);
		return 2;
	}

	g_Log.Add("[DevilSquare] DQItems Not Found");

	return 0;
}

void CMixSystem::LogDQChaosItem(LPOBJ lpObj)
{
	BYTE ExOption[MAX_EXOPTION_SIZE];

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			::ItemIsBufExOption(ExOption, &lpObj->pChaosBox[n] );

			g_Log.Add("[DevilSquare,%d] [%s][%s] CBMix [%d,%s,%d,%d,%d,%d]serial:[%I64d][%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d] 380Ex[%d] HO:[%d,%d]",
				lpObj->pChaosBox[n].m_Level,
				lpObj->AccountID, lpObj->Name, n, lpObj->pChaosBox[n].GetName(), lpObj->pChaosBox[n].m_Level,
				lpObj->pChaosBox[n].m_Option1, lpObj->pChaosBox[n].m_Option2, lpObj->pChaosBox[n].m_Option3,
				lpObj->pChaosBox[n].m_Number, (int)lpObj->pChaosBox[n].m_Durability,
				ExOption[0], ExOption[1], ExOption[2], ExOption[3], ExOption[4], ExOption[5], ExOption[6],
				lpObj->pChaosBox[n].m_SetOption, lpObj->pChaosBox[n].m_ItemOptionEx >> 7,
				g_kJewelOfHarmonySystem.GetItemStrengthenOption(&lpObj->pChaosBox[n]),
				g_kJewelOfHarmonySystem.GetItemOptionLevel(&lpObj->pChaosBox[n]));
		}
	}
}




BOOL CMixSystem::DevilSquareEventChaosMix(LPOBJ lpObj, BOOL bCheckType, int iItemLevel)
{
	BOOL fail = TRUE;
	int aIndex = lpObj->m_Index;
	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;
	lpObj->ChaosLock = TRUE;

	g_Log.Add("[DevilSquare] Chaos Mix Start");
	INT nChaosNeedMoney = 0;
	int iCharmOfLuckCount = 0;
	iCharmOfLuckCount = lpObj->ChaosSuccessRate;

	if ( iCharmOfLuckCount > 10 )
	{
		pMsg.Result = 0xF0;
		lpObj->ChaosLock = FALSE;
		IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
	}

	switch ( iItemLevel )
	{
		case 0:
			lpObj->ChaosSuccessRate = 60;
			nChaosNeedMoney = 100000;
			break;

		case 1:
			lpObj->ChaosSuccessRate = gDQChaosSuccessRateLevel1;
			nChaosNeedMoney = 100000;
			break;

		case 2:
			lpObj->ChaosSuccessRate = gDQChaosSuccessRateLevel2;
			nChaosNeedMoney = 200000;
			break;

		case 3:
			lpObj->ChaosSuccessRate = gDQChaosSuccessRateLevel3;
			nChaosNeedMoney = 400000;
			break;

		case 4:
			lpObj->ChaosSuccessRate = gDQChaosSuccessRateLevel4;
			nChaosNeedMoney = 700000;
			break;

		case 5:
			lpObj->ChaosSuccessRate = gDQChaosSuccessRateLevel5;
			nChaosNeedMoney = 1100000;
			break;

		case 6:
			lpObj->ChaosSuccessRate = gDQChaosSuccessRateLevel6;
			nChaosNeedMoney = 1600000;
			break;

		case 7:
			lpObj->ChaosSuccessRate = gDQChaosSuccessRateLevel7;
			nChaosNeedMoney = 2000000;
			break;

		default:
			g_Log.Add("[DevilSquare] [%s][%s] Invalid DQItem Level [%d]",
				lpObj->AccountID, lpObj->Name, iItemLevel);

			pMsg.Result = CB_INVALID_ITEM_LEVEL;
			IOCP.DataSend(aIndex, (BYTE *)&pMsg, pMsg.h.size);
			lpObj->ChaosLock = FALSE;
			return 1;
			break;
	}

	int iChaosTaxMoney = (int)((__int64)nChaosNeedMoney * (__int64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (__int64)100);

	if ( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	nChaosNeedMoney += iChaosTaxMoney;

	if ( nChaosNeedMoney < 0 )
	{
		nChaosNeedMoney = 0;
	}

	if ( (lpObj->m_PlayerData->Money - nChaosNeedMoney) < 0 )
	{
		g_Log.Add("[DevilSquare] [%s][%s] CBMix Not Enough Money [%d] need zen [%d]",
			lpObj->AccountID, lpObj->Name, lpObj->m_PlayerData->Money, nChaosNeedMoney);
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		IOCP.DataSend(aIndex, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return 1;

	}

	if ( g_CrywolfSync.GetOccupationState() == 0 && g_CrywolfSync.GetApplyBenefit() == TRUE )
	{	
		lpObj->ChaosSuccessRate += g_CrywolfSync.GetPlusChaosRate();
	}

	lpObj->ChaosSuccessRate += iCharmOfLuckCount;

	int iRate = rand() % 100;

	if ( bCheckType == TRUE )
	{
		if ( iRate < lpObj->ChaosSuccessRate )
		{
			int DevilInv = ITEMGET(14,19);	// Devil Ticket
			ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, DevilInv, iItemLevel, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0);
			fail = FALSE;
		}
	}

	lpObj->m_PlayerData->Money -= nChaosNeedMoney;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GSProtocol.GCMoneySend(lpObj->m_Index, lpObj->m_PlayerData->Money);

	if ( fail == TRUE )
	{
		ChaosBoxInit(lpObj);
		GSProtocol.GCUserChaosBoxSend(lpObj, 0);
		IOCP.DataSend(aIndex, (BYTE *)&pMsg, pMsg.h.size);

		g_Log.Add("[DevilSquare,%d] [%s][%s] CBMix Fail %d Money : %d-%d", iItemLevel,
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate,
			lpObj->m_PlayerData->Money, nChaosNeedMoney);
		lpObj->ChaosLock = FALSE;
	}
	else
	{
		g_Log.Add("[DevilSquare,%d] [%s][%s] CBMix Success Rate:%d Money : %d", iItemLevel,
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate,
			nChaosNeedMoney);
	}

	::gObjInventoryCommit(lpObj->m_Index);

	return TRUE;
}



void CMixSystem::LogPlusItemLevelChaosItem(LPOBJ lpObj, int iPlusMixLevel)
{
	int iMixLevel = 0;
	BYTE ExOption[MAX_EXOPTION_SIZE];

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			::ItemIsBufExOption(ExOption, &lpObj->pChaosBox[n]);

			if ( iPlusMixLevel == 3 )
				iMixLevel = 1;
			else if ( iPlusMixLevel == 4 )
				iMixLevel = 2;
			else if ( iPlusMixLevel == 22 )
				iMixLevel = 3;
			else if ( iPlusMixLevel == 23 )
				iMixLevel = 4;
			else if ( iPlusMixLevel == 49 )
				iMixLevel = 5;
			else if ( iPlusMixLevel == 50 )
				iMixLevel = 6;

			g_Log.Add("[PlusItemLevel,%d] [%s][%s] CBMix [%d,%s,%d,%d,%d,%d]serial:[%I64d][%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d] 380Ex:[%d] HO:[%d,%d]", iMixLevel,
				lpObj->AccountID, lpObj->Name, n, lpObj->pChaosBox[n].GetName(), lpObj->pChaosBox[n].m_Level,
				lpObj->pChaosBox[n].m_Option1, lpObj->pChaosBox[n].m_Option2, lpObj->pChaosBox[n].m_Option3,
				lpObj->pChaosBox[n].m_Number, (int)lpObj->pChaosBox[n].m_Durability,
				ExOption[0], ExOption[1], ExOption[2], ExOption[3], ExOption[4], ExOption[5], ExOption[6], lpObj->pChaosBox[n].m_SetOption,
				lpObj->pChaosBox[n].m_ItemOptionEx >> 7,
				g_kJewelOfHarmonySystem.GetItemStrengthenOption(&lpObj->pChaosBox[n]),	// #error Cahnge to GetItemStrengthenOption
				g_kJewelOfHarmonySystem.GetItemOptionLevel(&lpObj->pChaosBox[n]));
		}
	}
}



void CMixSystem::LogChaosItem(LPOBJ lpObj, LPSTR sLogType)
{
	BYTE ExOption[MAX_EXOPTION_SIZE];

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			::ItemIsBufExOption(ExOption, &lpObj->pChaosBox[n]);

			g_Log.Add("[%s] [%s][%s] CBMix [%d,%s,%d,%d,%d,%d]serial:[%I64d][%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d] 380:[%d] HO:[%d,%d]",
				sLogType, lpObj->AccountID, lpObj->Name, n, lpObj->pChaosBox[n].GetName(), lpObj->pChaosBox[n].m_Level,
				lpObj->pChaosBox[n].m_Option1, lpObj->pChaosBox[n].m_Option2, lpObj->pChaosBox[n].m_Option3, 
				lpObj->pChaosBox[n].m_Number, (int)lpObj->pChaosBox[n].m_Durability, 
				ExOption[0], ExOption[1], ExOption[2], ExOption[3], ExOption[4], ExOption[5], ExOption[6], lpObj->pChaosBox[n].m_SetOption,
				lpObj->pChaosBox[n].m_ItemOptionEx >> 7,
				g_kJewelOfHarmonySystem.GetItemStrengthenOption(&lpObj->pChaosBox[n]),
				g_kJewelOfHarmonySystem.GetItemOptionLevel(&lpObj->pChaosBox[n]));
		}
	}
}

BOOL CMixSystem::PlusItemLevelChaosMix(LPOBJ lpObj, int mixType)
{
	int ChaosGemCount = 0;
	int BlessGemCount = 0;
	int SoulGemCount = 0;
	int Plus9ItemCount = 0;
	int Plus10ItemCount = 0;
	int PlusItemPos = -1;
	int OtherItemFound = 0;
	int Plus11ItemCount = 0;
	int Plus12ItemCount = 0;
	int Plus13ItemCount = 0;
	int Plus14ItemCount = 0;
	int ExtraBlessGemCount = 0;
	int ExtraSoulGemCount = 0;
	int iCharmOfLuckCount = 0;
	int iElementalCharmOfLuckCount = 0;
	int iCharmOfAssembly = 0;
	int iElementalCharmOfAssembly = 0;
	BYTE btItemLuckValue = 0;

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(12,15) )
			{
				ChaosGemCount++;
			}
			else if (  lpObj->pChaosBox[n].m_Type == ITEMGET(14,13) )
			{
				BlessGemCount++;
			}
			else if (  lpObj->pChaosBox[n].m_Type == ITEMGET(14,14) )
			{
				SoulGemCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,53) )	// Charm Of Luck
			{
				iCharmOfLuckCount += lpObj->pChaosBox[n].m_Durability;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,96) )
			{
				iCharmOfAssembly++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,189) ) // Elemental Charm of Luck
			{
				iElementalCharmOfLuckCount += 10; // Always 10%
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,190) )
			{
				iElementalCharmOfAssembly++;
			}
			else if ( lpObj->pChaosBox[n].m_Level == 9 )
			{
				Plus9ItemCount++;
				PlusItemPos = n;
			}
			else if ( lpObj->pChaosBox[n].m_Level == 10 )
			{
				Plus10ItemCount++;
				PlusItemPos = n;
			}
			else if ( lpObj->pChaosBox[n].m_Level == 11 )
			{
				Plus11ItemCount++;
				PlusItemPos = n;
			}
			else if ( lpObj->pChaosBox[n].m_Level == 12 )
			{
				Plus12ItemCount++;
				PlusItemPos = n;
			}
			else if ( lpObj->pChaosBox[n].m_Level == 13 )
			{
				Plus13ItemCount++;
				PlusItemPos = n;
			}
			else if ( lpObj->pChaosBox[n].m_Level == 14 )
			{
				Plus14ItemCount++;
				PlusItemPos = n;
			}
			else
			{
				OtherItemFound ++;
			}
		}
	}

	if ( iCharmOfLuckCount > 10 )
	{
		PMSG_CHAOSMIXRESULT pResult;

		PHeadSetB((LPBYTE)&pResult, 0x86, sizeof(pResult));

		g_Log.Add("[PlusItemLevel] [%s][%s] CBMix Charm of luck over 10 (%d)",
			lpObj->AccountID, lpObj->Name, iCharmOfLuckCount);

		pResult.Result = 0xF0;

		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);

		lpObj->ChaosLock = FALSE;

		return 0;
	}

	if ( iElementalCharmOfLuckCount > 10 )
	{
		PMSG_CHAOSMIXRESULT pResult;

		PHeadSetB((LPBYTE)&pResult, 0x86, sizeof(pResult));

		g_Log.Add("[PlusItemLevel] [%s][%s] CBMix Charm of Luck over 10 (%d)",
			lpObj->AccountID, lpObj->Name, iCharmOfLuckCount);

		pResult.Result = 0xF0;

		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);

		lpObj->ChaosLock = FALSE;

		return 0;
	}

	if ( g_PentagramSystem.IsPentagramItem(lpObj->pChaosBox[PlusItemPos].m_Type) == true)
	{
		if ( iCharmOfLuckCount != 0 || iCharmOfAssembly != 0 )
		{
			PMSG_CHAOSMIXRESULT pResult;

			PHeadSetB((LPBYTE)&pResult, 0x86, sizeof(pResult));

			g_Log.Add("[PlusItemLevel] [%s][%s] CBMix Wrong Charm of luck/Assembly",
				lpObj->AccountID, lpObj->Name, iCharmOfLuckCount);

			pResult.Result = 0xF0;

			IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);

			lpObj->ChaosLock = FALSE;
	
			return 0;
		}
	}

	else
	{
		if ( iElementalCharmOfLuckCount != 0 || iElementalCharmOfAssembly != 0 )
		{
			PMSG_CHAOSMIXRESULT pResult;

			PHeadSetB((LPBYTE)&pResult, 0x86, sizeof(pResult));

			g_Log.Add("[PlusItemLevel] [%s][%s] CBMix Wrong Charm of luck/Assembly",
				lpObj->AccountID, lpObj->Name, iCharmOfLuckCount);

			pResult.Result = 0xF0;

			IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);

			lpObj->ChaosLock = FALSE;
	
			return 0;
		}
	}
	
	if ( OtherItemFound != FALSE )
	{
		return FALSE;
	}

	lpObj->ChaosMoney = 0;
	int MixType = 0;

	if ( Plus9ItemCount == 1 && ChaosGemCount == 1 && BlessGemCount == 1 && SoulGemCount == 1 && Plus10ItemCount == 0 && Plus11ItemCount == 0 && Plus12ItemCount == 0 )
	{
		MixType = 3;
		lpObj->ChaosMoney = 2000000;
		ExtraBlessGemCount = BlessGemCount - 1;
		ExtraSoulGemCount = SoulGemCount - 1;
	}
	else if ( Plus10ItemCount == 1 && ChaosGemCount == 1 && BlessGemCount == 2 && SoulGemCount == 2 && Plus9ItemCount == 0 && Plus11ItemCount == 0 && Plus12ItemCount == 0 )
	{
		MixType = 4;
		lpObj->ChaosMoney = 4000000;
		ExtraBlessGemCount = BlessGemCount - 2;
		ExtraSoulGemCount = SoulGemCount - 2;
	}
	else if ( Plus11ItemCount == 1 && ChaosGemCount == 1 && BlessGemCount == 3 && SoulGemCount == 3 && Plus9ItemCount == 0 && Plus10ItemCount == 0 && Plus12ItemCount == 0 )
	{
		MixType = 22;
		lpObj->ChaosMoney = 6000000;
		ExtraBlessGemCount = BlessGemCount - 3;
		ExtraSoulGemCount = SoulGemCount - 3;
	}
	else if ( Plus12ItemCount == 1 && ChaosGemCount == 1 && BlessGemCount == 4 && SoulGemCount == 4 && Plus9ItemCount == 0 && Plus10ItemCount == 0 && Plus11ItemCount == 0 )
	{
		MixType = 23;
		lpObj->ChaosMoney = 8000000;
		ExtraBlessGemCount = BlessGemCount - 4;
		ExtraSoulGemCount = SoulGemCount - 4;
	}

	else if ( Plus13ItemCount == 1 && ChaosGemCount == 1 && BlessGemCount == 5 && SoulGemCount == 5 && Plus9ItemCount == 0 && Plus10ItemCount == 0 && Plus11ItemCount == 0 )
	{
		MixType = 49;
		lpObj->ChaosMoney = 10000000;
		ExtraBlessGemCount = BlessGemCount - 5;
		ExtraSoulGemCount = SoulGemCount - 5;
	}

	else if ( Plus14ItemCount == 1 && ChaosGemCount == 1 && BlessGemCount == 6 && SoulGemCount == 6 && Plus9ItemCount == 0 && Plus10ItemCount == 0 && Plus11ItemCount == 0 )
	{
		MixType = 50;
		lpObj->ChaosMoney = 12000000;
		ExtraBlessGemCount = BlessGemCount - 6;
		ExtraSoulGemCount = SoulGemCount - 6;
	}

	if ( MixType != mixType )
	{
		MixType = 0;
	}

	if ( MixType == 0 )
	{
		return FALSE;
	}

	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;
	lpObj->ChaosLock = TRUE;
	this->LogPlusItemLevelChaosItem(lpObj, MixType);
	g_Log.Add("[PlusItemLevel] Chaos Mix Start");

	int iChaosTaxMoney = (int)((__int64)lpObj->ChaosMoney * (__int64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (__int64)100);

	if ( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	lpObj->ChaosMoney += iChaosTaxMoney;

	if ( lpObj->ChaosMoney < 0 )
	{
		lpObj->ChaosMoney = 0;
	}

	if ( lpObj->m_PlayerData->Money < lpObj->ChaosMoney )
	{
		g_Log.Add("[PlusItemLevel] [%s][%s] CBMix Not Enough Money [%d] need zen [%d]",
			lpObj->AccountID, lpObj->Name, lpObj->m_PlayerData->Money, lpObj->ChaosMoney);
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		IOCP.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		
		return TRUE;
	}

	lpObj->m_PlayerData->Money -= lpObj->ChaosMoney;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GSProtocol.GCMoneySend(lpObj->m_Index, lpObj->m_PlayerData->Money);

	int nItemGrade;

	if ((lpObj->pChaosBox[PlusItemPos].IsExtItem() == TRUE || 
		lpObj->pChaosBox[PlusItemPos].IsSetItem() == TRUE ||
		g_kItemSystemFor380.Is380Item(&lpObj->pChaosBox[PlusItemPos]) == TRUE) &&
		g_SocketOptionSystem.IsSocketItem(&lpObj->pChaosBox[PlusItemPos]) == FALSE)
	{
		nItemGrade = 1;
	}

	else if (g_SocketOptionSystem.IsSocketItem(&lpObj->pChaosBox[PlusItemPos]) == TRUE)
	{
		nItemGrade = 2;
	}

	else if (g_PentagramSystem.IsPentagramItem(&lpObj->pChaosBox[PlusItemPos]) == TRUE)
	{
		nItemGrade = 3;
	}

	else
	{
		nItemGrade = 0;
	}

	if ( (lpObj->pChaosBox[PlusItemPos].m_Type >= ITEMGET(12,0) && lpObj->pChaosBox[PlusItemPos].m_Type <= ITEMGET(12,6)) ||
		 (lpObj->pChaosBox[PlusItemPos].m_Type >= ITEMGET(12,36) && lpObj->pChaosBox[PlusItemPos].m_Type <= ITEMGET(12,43)) ||
		 (lpObj->pChaosBox[PlusItemPos].m_Type == ITEMGET(13,30)) )
	{
		nItemGrade = 0;
	}

	if (lpObj->m_PlayerData->VipType != 0)
	{
		lpObj->ChaosSuccessRate = g_VipSystem.GetPlusItemMixRate(lpObj, mixType, nItemGrade);

		if ( lpObj->pChaosBox[PlusItemPos].m_Option2 != FALSE )	// if Have Luck
		{
			lpObj->ChaosSuccessRate += g_VipSystem.GetPlusItemAddLuckRate(lpObj, nItemGrade);
		}
	}

	else
	{
		if ( lpObj->pChaosBox[PlusItemPos].m_Level == 9 )
		{
			lpObj->ChaosSuccessRate = g_ConfigRead.mix.PlusItem10Rate[nItemGrade];
		}
		else if ( lpObj->pChaosBox[PlusItemPos].m_Level == 10 )
		{
			lpObj->ChaosSuccessRate = g_ConfigRead.mix.PlusItem11Rate[nItemGrade];
		}
		else if ( lpObj->pChaosBox[PlusItemPos].m_Level == 11 )
		{
			lpObj->ChaosSuccessRate = g_ConfigRead.mix.PlusItem12Rate[nItemGrade];
		}
		else if ( lpObj->pChaosBox[PlusItemPos].m_Level == 12 )
		{
			lpObj->ChaosSuccessRate = g_ConfigRead.mix.PlusItem13Rate[nItemGrade];
		}
		else if ( lpObj->pChaosBox[PlusItemPos].m_Level == 13 )
		{
			lpObj->ChaosSuccessRate = g_ConfigRead.mix.PlusItem14Rate[nItemGrade];
		}
		else if ( lpObj->pChaosBox[PlusItemPos].m_Level == 14 )
		{
			lpObj->ChaosSuccessRate = g_ConfigRead.mix.PlusItem15Rate[nItemGrade];
		}

		if ( lpObj->pChaosBox[PlusItemPos].m_Option2 != FALSE )	// if Have Luck
		{
			lpObj->ChaosSuccessRate += g_ConfigRead.mix.PlusItemAddLuckRate[nItemGrade];
		}
	}

	lpObj->ChaosSuccessRate += iCharmOfLuckCount;
	lpObj->ChaosSuccessRate += iElementalCharmOfLuckCount;

	if ( lpObj->ChaosSuccessRate > 100 )
	{
		lpObj->ChaosSuccessRate = 100;
	}

	btItemLuckValue = lpObj->pChaosBox[PlusItemPos].m_Option2;
	
	int Result = rand()%100;

	if ( Result < lpObj->ChaosSuccessRate )
	{
		lpObj->pChaosBox[PlusItemPos].m_Level++;
		pMsg.Result = CB_SUCCESS;

		CItem Item;

		float Dur = ItemGetDurability(lpObj->pChaosBox[PlusItemPos].m_Type, lpObj->pChaosBox[PlusItemPos].m_Level,
			lpObj->pChaosBox[PlusItemPos].IsExtItem(), lpObj->pChaosBox[PlusItemPos].IsSetItem());

		Item.m_Level = lpObj->pChaosBox[PlusItemPos].m_Level;
		Item.m_Durability = Dur * lpObj->pChaosBox[PlusItemPos].m_Durability / lpObj->pChaosBox[PlusItemPos].m_BaseDurability;
		Item.m_JewelOfHarmonyOption = lpObj->pChaosBox[PlusItemPos].m_JewelOfHarmonyOption;

		BYTE SocketOption[5];
		BYTE SocketIndex;
		g_SocketOptionSystem.GetSocketOption(&lpObj->pChaosBox[PlusItemPos], SocketOption, SocketIndex);

		if (g_PentagramSystem.IsPentagramItem(&lpObj->pChaosBox[PlusItemPos]))
		{
			for (int i = 0; i < 5; i++)
			{
				SocketOption[i] = lpObj->pChaosBox[PlusItemPos].m_SocketOption[i];
			}

			SocketIndex = lpObj->pChaosBox[PlusItemPos].m_BonusSocketOption;
		}

		Item.Convert(lpObj->pChaosBox[PlusItemPos].m_Type, lpObj->pChaosBox[PlusItemPos].m_Option1, lpObj->pChaosBox[PlusItemPos].m_Option2,
			lpObj->pChaosBox[PlusItemPos].m_Option3, lpObj->pChaosBox[PlusItemPos].m_NewOption,
			lpObj->pChaosBox[PlusItemPos].m_SetOption, lpObj->pChaosBox[PlusItemPos].m_ItemOptionEx, SocketOption, SocketIndex, 0, CURRENT_DB_VERSION);

		Item.m_Number = lpObj->pChaosBox[PlusItemPos].m_Number;

		ItemByteConvert(pMsg.ItemInfo, Item);
		ChaosBoxInit(lpObj);
		::gObjChaosBoxInsertItemPos(lpObj->m_Index, Item, 0, -1);
		gObjChaosItemSet(lpObj->m_Index, 0, 1);

		IOCP.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);

		if (g_ConfigRead.mix.PlusItemMix15ResultNotice[nItemGrade] != 0 && MixType == 50)
		{
			GSProtocol.GCServerMsgStringSendAllEx(0, Lang.GetText(0,611), lpObj->Name, ItemAttribute[Item.m_Type].Name);
		}

		g_Log.Add("[PlusItemLevel] [%s][%s] CBMix Success %d / %d Money : %d-%d [%d], CharmRate : %d",
			lpObj->AccountID, lpObj->Name, Result, lpObj->ChaosSuccessRate, lpObj->m_PlayerData->Money,
			lpObj->ChaosMoney, Item.m_Level, iCharmOfLuckCount);
	}
	else if(iCharmOfAssembly == 0 && iElementalCharmOfAssembly == 0)
	{
		if (g_PentagramSystem.IsPentagramItem(&lpObj->pChaosBox[PlusItemPos]) == true)
		{
			g_PentagramSystem.DelPentagramJewelInfo(lpObj->m_Index, &lpObj->pChaosBox[PlusItemPos]);
		}

		int FailLevel = lpObj->pChaosBox[PlusItemPos].m_Level+1;
		ChaosBoxInit(lpObj);
		GSProtocol.GCUserChaosBoxSend(lpObj, 0);

		IOCP.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);

		g_Log.Add("[PlusItemLevel] [%s][%s] CBMix Fail %d / %d Money : %d-%d [%d], CharmRate : %d",
			lpObj->AccountID, lpObj->Name, Result, lpObj->ChaosSuccessRate, lpObj->m_PlayerData->Money,
			lpObj->ChaosMoney, FailLevel, iCharmOfLuckCount);
	}
	else
	{
		int FailLevel = lpObj->pChaosBox[PlusItemPos].m_Level+1;
		this->ChaosMixCharmItemUsed(lpObj);
		GSProtocol.GCUserChaosBoxSend(lpObj, 0);

		IOCP.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);

		g_Log.Add("[PlusItemLevel] [%s][%s] CBMix Fail, Charm of Chaos Assembly Used, %d / %d Money : %d-%d [%d], CharmRate : %d",
			lpObj->AccountID, lpObj->Name, Result, lpObj->ChaosSuccessRate, lpObj->m_PlayerData->Money,
			lpObj->ChaosMoney, FailLevel, iCharmOfLuckCount);
	}

	lpObj->ChaosLock = FALSE;
	return TRUE;
}

void CMixSystem::ChaosMixCharmItemUsed(LPOBJ lpObj)
{
	for (int n = 0; n<CHAOS_BOX_SIZE; n++)
	{
		if (lpObj->pChaosBox[n].IsItem() == TRUE)
		{
			lpObj->pChaosBox[n].m_Level = 0;

			int iItemCode = lpObj->pChaosBox[n].m_Type;

			if (this->IsDeleteItem(iItemCode) == TRUE)
			{
				lpObj->pChaosBox[n].Clear();
				lpObj->pChaosBoxMap[n] = -1;
			}
		}
	}
}

BOOL CMixSystem::IsDeleteItem(int iItemCode)
{
	switch (iItemCode)
	{
	case ITEMGET(14, 53):
	case ITEMGET(12, 15):
	case ITEMGET(14, 13):
	case ITEMGET(14, 14):
	case ITEMGET(14, 96):
	case ITEMGET(14, 189):
	case ITEMGET(14, 190):
		return TRUE;
	}

	return FALSE;
}

BOOL CMixSystem::PegasiaChaosMix(LPOBJ lpObj)
{
	lpObj->ChaosLock = TRUE;
	int UniriaCount = 0;
	int ChoasGemCount = 0;
	int iCharmOfLuckCount = 0;

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(13,2) && lpObj->pChaosBox[n].m_Durability == 255.0f )	// Uniria
			{
				UniriaCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(12,15) ) // Chaos Gem
			{
				ChoasGemCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,53) )	// Charm Of Luck
			{
				iCharmOfLuckCount += lpObj->pChaosBox[n].m_Durability;
			}
		}
	}

	if ( UniriaCount != 10 || ChoasGemCount != 1 )
	{
		lpObj->ChaosLock = FALSE;
		return FALSE;
	}

	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;

	if ( iCharmOfLuckCount > 10 )
	{
		lpObj->ChaosLock = FALSE;
		pMsg.Result = 0xF0;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

		return 0;
	}

	int nChaosNeedMoney = 500000;
	int iChaosTaxMoney = (int)((__int64)nChaosNeedMoney * (__int64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (__int64)100);

	if ( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	nChaosNeedMoney += iChaosTaxMoney;

	if ( nChaosNeedMoney < 0 )
	{
		nChaosNeedMoney = 0;
	}

	if ( lpObj->m_PlayerData->Money < nChaosNeedMoney )
	{
		lpObj->ChaosLock = FALSE;
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		
		IOCP.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return TRUE;
	}

	LogChaosItem(lpObj, "DinorantMix");
	g_Log.Add("[DinorantMix] Chaos Mix Start");
	lpObj->ChaosSuccessRate = 70;	// Succes Rate for Dinorant
	lpObj->ChaosSuccessRate += iCharmOfLuckCount;
	lpObj->m_PlayerData->Money -= nChaosNeedMoney;

	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GSProtocol.GCMoneySend(lpObj->m_Index, lpObj->m_PlayerData->Money);

	if ( (rand()%100 ) < lpObj->ChaosSuccessRate )
	{
		int Dinorant = ITEMGET(13,3);
		int Option3 = 0;

		if ( (rand()% 100) < 30 )
		{
			Option3 = 1 <<  ((rand()%3)) ;

			if ( (rand()%5) == 0 )
			{
				Option3 |= 1 << (rand()%3);
			}
		}

		::ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, Dinorant, 0, 255, 1, 0, Option3, -1, 0, 0, 0, 0, 0);
		::gObjInventoryCommit(lpObj->m_Index);

		g_Log.Add("[DinorantMix] [%s][%s] CBMix Success %d Money : %d-%d, CharmRate : %d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->m_PlayerData->Money, nChaosNeedMoney, iCharmOfLuckCount);

		return TRUE;
	}
	else
	{
		ChaosBoxInit(lpObj);	// Errase Chaos Box
		GSProtocol.GCUserChaosBoxSend(lpObj, 0);
		IOCP.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);

		g_Log.Add("[DinorantMix] [%s][%s] CBMix Fail %d Money : %d-%d, CharmRate : %d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->m_PlayerData->Money, nChaosNeedMoney, iCharmOfLuckCount);

		lpObj->ChaosLock = FALSE;
		return FALSE;
	}
}

BOOL CMixSystem::CircleChaosMix(LPOBJ lpObj)	// Fruits
{
	lpObj->ChaosLock = TRUE;

	int CreatureGemCount = 0;
	int ChoasGemCount = 0;
	int iCharmOfLuckCount = 0;

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,22) )	// Jewel of Creation
			{
				CreatureGemCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(12,15) ) // Chaos Gem
			{
				ChoasGemCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,53) )	// Charm Of Luck
			{
				iCharmOfLuckCount += lpObj->pChaosBox[n].m_Durability;
			}
		}
	}

	if ( CreatureGemCount != 1 || ChoasGemCount != 1 )
	{
		return FALSE;
	}

	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;

	if ( iCharmOfLuckCount > 10 )
	{
		lpObj->ChaosLock = FALSE;
		pMsg.Result = 0xF0;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

		return 1;
	}

	if ( lpObj->Level < 10 )
	{
		pMsg.Result = CB_LOW_LEVEL_USER;

		IOCP.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return TRUE;
	}

	int nChaosNeedMoney = 3000000;
	int iChaosTaxMoney = (int)((__int64)nChaosNeedMoney * (__int64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (__int64)100);

	if ( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	nChaosNeedMoney += iChaosTaxMoney;

	if ( nChaosNeedMoney < 0 )
	{
		nChaosNeedMoney = 0;
	}

	if ( lpObj->m_PlayerData->Money < nChaosNeedMoney )
	{
		lpObj->ChaosLock = FALSE;
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		
		IOCP.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return TRUE;
	}

	LogChaosItem(lpObj, "CircleMix");
	g_Log.Add("[CircleMix] Chaos Mix Start");
	lpObj->ChaosSuccessRate = 90;	// Succes Rate for Fruit
	lpObj->ChaosSuccessRate += iCharmOfLuckCount;
	lpObj->m_PlayerData->Money -= nChaosNeedMoney;

	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GSProtocol.GCMoneySend(lpObj->m_Index, lpObj->m_PlayerData->Money);

	if ( (rand()%100 ) < lpObj->ChaosSuccessRate )
	{
		int Fruit = ITEMGET(13,15);	// Fruit
		int FruitType;
		int RandonValue = rand() % 100;

		if ( RandonValue < 30 )
		{
			FruitType = 0;
		}
		else if ( RandonValue < 55 )
		{
			FruitType = 1;
		}
		else if ( RandonValue < 75 )
		{
			FruitType = 2;
		}
		else if ( RandonValue < 95 )
		{
			FruitType = 3;
		}
		else
		{
			FruitType = 4;
		}

		::ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, Fruit, FruitType, 255, 1, 0, 0, -1, 0, 0, 0, 0, 0);
		::gObjInventoryCommit(lpObj->m_Index);

		g_Log.Add("[CircleMix] [%s][%s] CBMix Success %d Money : %d-%d, CharmRate : %d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->m_PlayerData->Money, nChaosNeedMoney, iCharmOfLuckCount);

		return TRUE;
	}
	else
	{
		ChaosBoxInit(lpObj);
		GSProtocol.GCUserChaosBoxSend(lpObj, 0);

		IOCP.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		g_Log.Add("[CircleMix] [%s][%s] CBMix Fail %d Money : %d-%d, CharmRate : %d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->m_PlayerData->Money, nChaosNeedMoney, iCharmOfLuckCount);
		lpObj->ChaosLock = FALSE;

		return FALSE;
	}
}

BOOL CMixSystem::WingChaosMix(LPOBJ lpObj)
{
	lpObj->ChaosLock = TRUE;

	int WingCount = 0;
	int ChoasGemCount = 0;
	int LokesFeathersCount = 0;
	int WingIndex = -1;
	DWORD iChaosMoney = 0;
	int iWingChaosMoney = 0;
	int iSleeveOfLord = 0;
	int iCharmOfLuckCount = 0;
	int WingCharmItemNum = -1; //season4.5 add-on

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( (lpObj->pChaosBox[n].m_Type >= ITEMGET(12,3) && lpObj->pChaosBox[n].m_Type <= ITEMGET(12,6)) || lpObj->pChaosBox[n].m_Type == ITEMGET(13,30) )	
			{
				lpObj->ChaosLock = FALSE;

				return FALSE;
			}
			if ( (lpObj->pChaosBox[n].m_Type >= ITEMGET(12,0) && lpObj->pChaosBox[n].m_Type <= ITEMGET(12,2)) || lpObj->pChaosBox[n].m_Type == ITEMGET(12,41) ) 
			{
				WingCount++;
				WingIndex = n;
				iWingChaosMoney = lpObj->pChaosBox[n].m_BuyMoney;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(12,15) ) // Chaos
			{
				ChoasGemCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(13,14) ) // Feather
			{
				if ( lpObj->pChaosBox[n].m_Level == 0 ) // Feather
				{
					LokesFeathersCount++;
				}
				else	// Crst of Monarch
				{
					iSleeveOfLord ++;
				}
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,53) )	// Charm Of Luck
			{
				iCharmOfLuckCount += lpObj->pChaosBox[n].m_Durability;
			}
			else if (lpObj->pChaosBox[n].m_Type == ITEMGET(13, 88) || //season4.5 add-on
				lpObj->pChaosBox[n].m_Type == ITEMGET(13, 89) ||
				lpObj->pChaosBox[n].m_Type == ITEMGET(13, 90) ||
				lpObj->pChaosBox[n].m_Type == ITEMGET(13, 91) ||
				lpObj->pChaosBox[n].m_Type == ITEMGET(13, 92))
			{
				WingCharmItemNum = lpObj->pChaosBox[n].m_Type;
			}

			else if ( lpObj->pChaosBox[n].IsExtItem() != FALSE )
			{
				if ( lpObj->pChaosBox[n].m_Level >= 4 )
				{
					iChaosMoney += lpObj->pChaosBox[n].m_BuyMoney;
				}
			}
		}
	}

	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;

	

	if ( iSleeveOfLord == 1 )
	{
		if ( WingCount != 1 || ChoasGemCount != 1 || LokesFeathersCount != 0 )
		{
			lpObj->ChaosLock = FALSE;
			pMsg.Result = CB_INCORRECT_MIX_ITEMS;
			IOCP.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
			return FALSE;
		}
	}
	else if ( LokesFeathersCount == 1 )
	{
		if ( WingCount != 1 || ChoasGemCount != 1 || iSleeveOfLord != 0 )
		{
			pMsg.Result = CB_INCORRECT_MIX_ITEMS;
			IOCP.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
			lpObj->ChaosLock = FALSE;

			return FALSE;
		}
	}
	else
	{
		lpObj->ChaosLock = FALSE;
		pMsg.Result = CB_INCORRECT_MIX_ITEMS;
		IOCP.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);

		return FALSE;
	}

	if ( iCharmOfLuckCount > 10 )
	{
		lpObj->ChaosLock = FALSE;
		pMsg.Result = 0xF0;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

		return FALSE;
	}

	int nChaosNeedMoney = 5000000;
	int iChaosTaxMoney = (int)((__int64)nChaosNeedMoney * (__int64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (__int64)100);

	if ( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	nChaosNeedMoney += iChaosTaxMoney;

	if ( nChaosNeedMoney < 0 )
	{
		nChaosNeedMoney = 0;
	}

	if ( lpObj->m_PlayerData->Money < nChaosNeedMoney )
	{
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		
		IOCP.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return TRUE;
	}

	CItem * pWing = &lpObj->pChaosBox[WingIndex];
	lpObj->ChaosSuccessRate = (DWORD)((DWORD)iWingChaosMoney / (DWORD)4000000);
	lpObj->ChaosSuccessRate += iChaosMoney / 40000;

	if ( lpObj->ChaosSuccessRate == 0 )
	{
		pMsg.Result = CB_INCORRECT_MIX_ITEMS;
		IOCP.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return FALSE;
	}

	LogChaosItem(lpObj, "WingMix,2");
	g_Log.Add("[WingMix,2] Chaos Mix Start");

	if (lpObj->m_PlayerData->VipType != 0)
	{
		if ( iSleeveOfLord == 1 )
		{
			if ( lpObj->ChaosSuccessRate > g_VipSystem.GetCapeOfLordRate(lpObj) )
			{
				lpObj->ChaosSuccessRate = g_VipSystem.GetCapeOfLordRate(lpObj);
			}
		}

		else if ( LokesFeathersCount == 1 )
		{
			if ( lpObj->ChaosSuccessRate > g_VipSystem.GetWing2ndRate(lpObj) )
			{
				lpObj->ChaosSuccessRate =   g_VipSystem.GetWing2ndRate(lpObj);
			}
		}

		else if ( lpObj->ChaosSuccessRate >  g_VipSystem.GetWing2ndRate(lpObj) )
		{
			lpObj->ChaosSuccessRate =  g_VipSystem.GetWing2ndRate(lpObj);
		}
	}

	else
	{
		if ( iSleeveOfLord == 1 )
		{
			if ( lpObj->ChaosSuccessRate > g_ConfigRead.mix.CapeLordMaxRate)
			{
				lpObj->ChaosSuccessRate = g_ConfigRead.mix.CapeLordMaxRate;
			}
		}

		else if ( LokesFeathersCount == 1 )
		{
			if ( lpObj->ChaosSuccessRate > g_ConfigRead.mix.Wing2MaxRate )
			{
				lpObj->ChaosSuccessRate =  g_ConfigRead.mix.Wing2MaxRate;
			}
		}

		else if ( lpObj->ChaosSuccessRate > g_ConfigRead.mix.Wing2MaxRate )
		{
			lpObj->ChaosSuccessRate = g_ConfigRead.mix.Wing2MaxRate;
		}
	}

	lpObj->ChaosSuccessRate += iCharmOfLuckCount;
	lpObj->m_PlayerData->Money  -= nChaosNeedMoney;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GSProtocol.GCMoneySend(lpObj->m_Index, lpObj->m_PlayerData->Money);

	if ( (rand()%100) < lpObj->ChaosSuccessRate )
	{
		int iWingLevel = 0;
		int iWingType;
		int iItemType;
		int iItemSubType;

		if (iSleeveOfLord != 0)	//1.01.00 updated
		{
			if (rand() % 2)
			{
				iItemType = 13;
				iItemSubType = 30;
			}
			else
			{
				iItemType = 12;
				iItemSubType = 49;
			}
		}
		else
		{
			iItemType = 12;

			int iRand = rand() % 5;

			if (iRand == 4)
			{
				iItemSubType = 42;
			}
			else
			{
				iItemSubType = iRand + 3;
			}
		}

		if (WingCharmItemNum > -1) //season4.5 add-on
		{
			int WingCharmIndex = WingCharmItemNum ^ ITEMGET(13, 0);

			switch (WingCharmIndex)
			{
			case 88:
				iItemSubType = 5;
				break;
			case 89:
				iItemSubType = 4;
				break;
			case 90:
				iItemSubType = 3;
				break;
			case 91:
				iItemSubType = 42;
				break;
			case 92:
				iItemSubType = 6;
				break;
			default:
				g_Log.Add("[MixSystem][WingChaosMix] WingCharmItemNum => %d , WingCharmIndex => %d", WingCharmItemNum, WingCharmIndex);
				break;
			}
		}

		int iWingNum = ITEMGET(iItemType, iItemSubType);
		int iOption1 = 0;
		int iOption2 = 0;
		int iOption3 = 0;

		if ( (rand()%5) == 0 )
		{
			iOption1 = 1;
		}

		int iRandomValue = rand() % 100;
		int iRandomValue2 = rand() % 3;

		switch ( iRandomValue2 )
		{
			case 0:
				if ( iRandomValue < 4 )
				{
					iOption2 = 3;	// +12
				}
				break;

			case 1:
				if ( iRandomValue < 10 )
				{
					iOption2 = 2;	// +8
				}
				break;

			case 2:
				if ( iRandomValue < 20 )
				{
					iOption2 = 1;	// +4;
				}
		}

		if (iSleeveOfLord != FALSE)	//1.01.00 updated
		{
			if ((rand() % 5) == 0)
			{
				if (iItemType == 12 && iItemSubType == 49)
				{
					iOption3 = 1 << (rand() % 3);
					if ((rand() % 2) != 0)
					{
						iOption3 |= 0x20;
					}
				}
				else
				{
					iOption3 = 1 << (rand() % 4);
				}
			}
			// ----
			if (iItemType == 13 && iItemSubType == 30)
			{
				iOption3 |= 0x20;
			}
		}
		else
		{
			if ((rand() % 5) == 0)
			{
				iOption3 = 1 << (rand() % 3);
			}

			if ((rand() % 2) != 0)
			{
				iOption3 |= 0x20;
			}

		}

		::ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, iWingNum, iWingLevel, 0, 0, iOption1, iOption2, -1, iOption3, 0, 0, 0, 0);
		::gObjInventoryCommit(lpObj->m_Index);
		::g_Log.Add("[WingMix,2] [%s][%s] CBMix Success %d Money : %d-%d, CharmRate : %d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->m_PlayerData->Money, nChaosNeedMoney, iCharmOfLuckCount);
		return TRUE;
	}
	else
	{
		for (int n=0;n<CHAOS_BOX_SIZE;n++)
		{
			lpObj->pChaosBox[n].Clear();
		}

		GSProtocol.GCUserChaosBoxSend(lpObj, 0);
		IOCP.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		::g_Log.Add("[WingMix,2] [%s][%s] CBMix Fail %d Money : %d-%d, CharmRate : %d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->m_PlayerData->Money, nChaosNeedMoney, iCharmOfLuckCount);
		
		lpObj->ChaosLock = FALSE;

		return FALSE;
	}
}

BOOL CMixSystem::IllusionTempleItemChaosMix(LPOBJ lpObj)
{
	lpObj->ChaosLock = TRUE;
	int ChoasGemCount = 0;
	int CovenantCount = 0;
	int OldScrollCount = 0;
	int CovenantLevel = 0;
	int OldScrollLevel = 0;
	int iCharmOfLuckCount = 0;

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(12,15) ) // Chaos
			{
				ChoasGemCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(13,49) ) // Old Scroll
			{
				OldScrollCount++;
				OldScrollLevel = lpObj->pChaosBox[n].m_Level;
			}

			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(13,50) ) // Covenant
			{
				CovenantCount++;
				CovenantLevel = lpObj->pChaosBox[n].m_Level;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,53) )	// Charm Of Luck
			{
				iCharmOfLuckCount += lpObj->pChaosBox[n].m_Durability;
			}

		}
	}

	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;

	
	if ( ChoasGemCount == 1 )
	{
		if ( ChoasGemCount != 1 || OldScrollCount != 1 || CovenantCount != 1 || OldScrollLevel != CovenantLevel )
		{
			pMsg.Result = CB_INCORRECT_MIX_ITEMS;
			IOCP.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
			lpObj->ChaosLock = FALSE;

			return FALSE;
		}
	}
	else if ( OldScrollCount == 1 )
	{
		if ( ChoasGemCount != 1 || OldScrollCount != 1 || CovenantCount != 1 || OldScrollLevel != CovenantLevel )
		{
			pMsg.Result = CB_INCORRECT_MIX_ITEMS;
			IOCP.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
			lpObj->ChaosLock = FALSE;

			return FALSE;
		}
	}
	else if ( CovenantCount == 1 )
	{
		if ( ChoasGemCount != 1 || OldScrollCount != 1 || CovenantCount != 1 || OldScrollLevel != CovenantLevel )
		{
			pMsg.Result = CB_INCORRECT_MIX_ITEMS;
			IOCP.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
			lpObj->ChaosLock = FALSE;

			return FALSE;
		}
	}
	else
	{
		lpObj->ChaosLock = FALSE;
		pMsg.Result = CB_INCORRECT_MIX_ITEMS;
		IOCP.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);

		return FALSE;
	}

	if ( iCharmOfLuckCount > 10 )
	{
		lpObj->ChaosLock = FALSE;
		pMsg.Result = 0xF0;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

		return FALSE;
	}

	int nChaosNeedMoney; //Fine ;D

	switch(OldScrollLevel)
		{
		    case 1:
				nChaosNeedMoney = 3000000;
			    break;
			case 2:
                nChaosNeedMoney = 5000000;
				break;
			case 3:
				nChaosNeedMoney = 7000000;
				break;
			case 4:
				nChaosNeedMoney = 9000000;
				break;
			case 5:
				nChaosNeedMoney = 11000000;
				break;
			case 6:
				nChaosNeedMoney = 13000000;
				break;
			default:
			    nChaosNeedMoney = 3000000;
				break;
		}

	int iChaosTaxMoney = (int)((__int64)nChaosNeedMoney * (__int64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (__int64)100);

	if ( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	nChaosNeedMoney += iChaosTaxMoney;

	if ( nChaosNeedMoney < 0 )
	{
		nChaosNeedMoney = 0;
	}

	if ( lpObj->m_PlayerData->Money < nChaosNeedMoney )
	{
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		
		IOCP.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return TRUE;
	}

	lpObj->ChaosSuccessRate = 70;

	if ( g_CrywolfSync.GetOccupationState() == 0 && g_CrywolfSync.GetApplyBenefit() == TRUE )
	{	
		lpObj->ChaosSuccessRate += g_CrywolfSync.GetPlusChaosRate();
	}

	if ( lpObj->ChaosSuccessRate == 0 && ChoasGemCount != 1 && OldScrollCount != 1 && CovenantCount != 1 && OldScrollLevel != CovenantLevel )
	{
		pMsg.Result = CB_INCORRECT_MIX_ITEMS;
		IOCP.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return FALSE;
	}

	LogChaosItem(lpObj,"IllusionTemple Ticket");
	g_Log.Add("[IllusionTemple Ticket,%d] Chaos Mix Start",OldScrollLevel);

	if ( ChoasGemCount == 1 && OldScrollCount == 1 && CovenantCount == 1 && OldScrollLevel == CovenantLevel )
	{
		if ( lpObj->ChaosSuccessRate > 80)
		{
			lpObj->ChaosSuccessRate = 80;
		}
	}
	else if ( lpObj->ChaosSuccessRate > 100 )
	{
		lpObj->ChaosSuccessRate = 100;
	}

	lpObj->ChaosSuccessRate += iCharmOfLuckCount;
	lpObj->m_PlayerData->Money  -= nChaosNeedMoney;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);

	GSProtocol.GCMoneySend(lpObj->m_Index, lpObj->m_PlayerData->Money);

	if ( (rand()%100) < lpObj->ChaosSuccessRate )
	{
		int iBloodScroll = ITEMGET(13,51);
		::ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, iBloodScroll, OldScrollLevel, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0);
		::gObjInventoryCommit(lpObj->m_Index);
		::g_Log.Add("[IllusionTemple Ticket,%d] [%s][%s] CBMix Success %d Money : %d-%d, CharmRate : %d",
			OldScrollLevel,lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->m_PlayerData->Money, nChaosNeedMoney, iCharmOfLuckCount);
		return TRUE;
	}
	else
	{
		for (int n=0;n<CHAOS_BOX_SIZE;n++)
		{
			lpObj->pChaosBox[n].Clear();
		}

		GSProtocol.GCUserChaosBoxSend(lpObj, 0);
		IOCP.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		::g_Log.Add("[IllusionTemple Ticket,%d] [%s][%s] CBMix Fail %d Money : %d-%d, CharmRate : %d",
			OldScrollLevel,lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->m_PlayerData->Money, nChaosNeedMoney, iCharmOfLuckCount);
		
		lpObj->ChaosLock = FALSE;

		return FALSE;
	}
}

BOOL CMixSystem::IsMixPossibleItem(int iItemCode)
{
	if (iItemCode == this->CONDOR_FEATHER_ITEMINDEX || iItemCode == this->CONDOR_FLAME_ITEMINDEX)
	{
		return true;
	}

	return false;
}

BOOL CMixSystem::CheckWingItem(int iItemCode)
{
	if (iItemCode == ITEMGET(12, 0) ||
		iItemCode == ITEMGET(12, 1) ||
		iItemCode == ITEMGET(12, 2) ||
		iItemCode == ITEMGET(12, 41))
	{
		return true;
	}

	return false;
}

BOOL CMixSystem::Check2ndWingItem(int iItemCode)
{
	if (iItemCode == ITEMGET(12, 3) ||
		iItemCode == ITEMGET(12, 4) ||
		iItemCode == ITEMGET(12, 5) ||
		iItemCode == ITEMGET(12, 6) ||
		iItemCode == ITEMGET(13, 30) ||
		iItemCode == ITEMGET(12, 42) ||
		iItemCode == ITEMGET(12, 49))
	{
		return true;
	}
	return false;
}

BOOL CMixSystem::Check3rdWingItem(int iItemCode)
{
	if (iItemCode == this->WING_OF_STORM_ITEMINDEX ||
		iItemCode == this->WING_OF_ETERNAL_ITEMINDEX ||
		iItemCode == this->WING_OF_ILLUSION_ITEMINDEX ||
		iItemCode == this->WING_OF_RUIN_ITEMINDEX ||
		iItemCode == this->CAPE_OF_EMPEROR_ITEMINDEX ||
		iItemCode == this->WING_OF_DIMENSION_ITEMINDEX ||
		iItemCode == this->CAPE_OF_OVERRULE_ITEMINDEX)
	{
		return true;
	}

	return false;
}

BOOL CMixSystem::CheckLevelCondition(CItem * lpItem, short Level, BYTE Op1, BYTE Op2, BYTE Op3, BYTE SetOption, BYTE NewOption)
{
	if (lpItem == NULL){ return FALSE; }
	if (Level != FALSE){ if (lpItem->m_Level < Level){ return FALSE; } }
	if (Op1 != FALSE){ if (lpItem->m_Option1 < Op1){ return FALSE; } }
	if (Op2 != FALSE){ if (lpItem->m_Option2 < Op2){ return FALSE; } }
	if (Op3 != FALSE){ if (lpItem->m_Option3 < Op3){ return FALSE; } }
	if (SetOption != FALSE){ if (!lpItem->m_SetOption){ return FALSE; } }
	if (NewOption != FALSE){ if (!lpItem->m_NewOption){ return FALSE; } }
	return TRUE;
}

BOOL CMixSystem::ThirdWingLevel1ChaosMix(LPOBJ lpObj)
{
	lpObj->ChaosLock = TRUE;

	int WingCount = 0;
	int ChoasGemCount = 0;
	int BundleOfSoulCount = 0;
	int JewelOfCreationCount = 0;
	int SetItemCount = 0;
	DWORD iChaosMoney = 0;
	int iCharmOfLuckCount = 0;

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if (this->Check2ndWingItem(lpObj->pChaosBox[n].m_Type) != FALSE)
			{
				if (this->CheckLevelCondition(&lpObj->pChaosBox[n], 9, 0, 0, 1, 0, 0) != FALSE)
				{
					WingCount++;
				}
			}
			else if (lpObj->pChaosBox[n].m_Type == this->CHAOS_GEM_ITEMINDEX) // Chaos
			{
				ChoasGemCount++;
			}
			else if (lpObj->pChaosBox[n].m_Type == this->SOUL_GEM_BULDLE_ITEMINDEX) // Bundle of Soul
			{
				BundleOfSoulCount++;
			}
			else if (lpObj->pChaosBox[n].m_Type == this->CREATURE_GEM_ITEMINDEX) // Jewel of Creation
			{
				JewelOfCreationCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,53) ) // Charm Of Luck
			{
				iCharmOfLuckCount += lpObj->pChaosBox[n].m_Durability;
			}
			else if (this->CheckLevelCondition(&lpObj->pChaosBox[n], 7, 0, 0, 1, 1, 0) != FALSE)
			{
				SetItemCount++;
				iChaosMoney += lpObj->pChaosBox[n].m_BuyMoney;
			}
		}
	}

	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;

	if ( WingCount != 0 && JewelOfCreationCount != 0 )
	{
		if ( ChoasGemCount != 1 || BundleOfSoulCount != 1 || JewelOfCreationCount != 1 || SetItemCount < 1 )
		{
			lpObj->ChaosLock = FALSE;
			pMsg.Result = CB_INCORRECT_MIX_ITEMS;
			IOCP.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
			return FALSE;
		}
	}
	else
	{
		lpObj->ChaosLock = FALSE;
		pMsg.Result = CB_INCORRECT_MIX_ITEMS;
		IOCP.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);

		return FALSE;
	}

	if ( iCharmOfLuckCount > 10 )
	{
		lpObj->ChaosLock = FALSE;
		pMsg.Result = 0xF0;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

		return FALSE;
	}

	if ( iChaosMoney > 0 )
	{
		lpObj->ChaosSuccessRate = iChaosMoney / 300000; //Good 60%
	}

	if ( lpObj->ChaosSuccessRate < 1 )
		lpObj->ChaosSuccessRate = 1;

	if (lpObj->m_PlayerData->VipType != 0)
	{
		if (lpObj->ChaosSuccessRate > g_VipSystem.GetFeatherOfCondorRate(lpObj))
		{
			lpObj->ChaosSuccessRate = g_VipSystem.GetFeatherOfCondorRate(lpObj);
		}
	}

	else
	{
		if (lpObj->ChaosSuccessRate > g_ConfigRead.mix.FeatherCondorMaxRate)
		{
			lpObj->ChaosSuccessRate = g_ConfigRead.mix.FeatherCondorMaxRate;
		}
	}

	LogChaosItem(lpObj, "ThirdWingLevel1_Mix");
	g_Log.Add("[ThirdWing Mix][Level 01] Chaos Mix Start");

	lpObj->ChaosSuccessRate += iCharmOfLuckCount;

	int nChaosNeedMoney = lpObj->ChaosSuccessRate * 200000;
	int iChaosTaxMoney = (int)((__int64)nChaosNeedMoney * (__int64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (__int64)100);

	if ( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	nChaosNeedMoney += iChaosTaxMoney;

	if ( nChaosNeedMoney < 0 )
	{
		nChaosNeedMoney = 0;
	}

	if ( lpObj->m_PlayerData->Money < nChaosNeedMoney )
	{
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		
		IOCP.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return TRUE;
	}

	lpObj->m_PlayerData->Money  -= nChaosNeedMoney;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GSProtocol.GCMoneySend(lpObj->m_Index, lpObj->m_PlayerData->Money);

	if ( (rand()%100) < lpObj->ChaosSuccessRate )
	{
		int iItemType;
		int iItemSubType;

		iItemType = 13;
		iItemSubType = 53;
		int iWingNum = ITEMGET(iItemType, iItemSubType);

		::ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, iWingNum, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0);
		::gObjInventoryCommit(lpObj->m_Index);
		::g_Log.Add("[ThirdWing Mix][Level 01] [%s][%s] CBMix Success %d Money : %d-%d, CharmRate : %d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->m_PlayerData->Money, nChaosNeedMoney, iCharmOfLuckCount);
		return TRUE;
	}
	else
	{
		this->ThirdWingMixFail(lpObj);
		GSProtocol.GCUserChaosBoxSend(lpObj, 0);
		IOCP.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		::g_Log.Add("[ThirdWing Mix][Level 01] [%s][%s] CBMix Fail %d Money : %d-%d, CharmRate : %d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->m_PlayerData->Money, nChaosNeedMoney, iCharmOfLuckCount);
		
		lpObj->ChaosLock = FALSE;

		return FALSE;
	}
}

BOOL CMixSystem::AdvancedWingMix(LPOBJ lpObj)
{
	lpObj->ChaosLock = TRUE;

	int ChoasGemCount = 0;
	int BundleOfSoulCount = 0;
	int BundleOfBlessCount = 0;
	int BundleOfCreationCount = 0;
	int BundleOfHarmonyCount = 0;
	int ExcItemCount = 0;
	DWORD iChaosMoney = 0;
	int iCharmOfLuckCount = 0;

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( (lpObj->pChaosBox[n].m_Type >= ITEMGET(12,0) && lpObj->pChaosBox[n].m_Type <= ITEMGET(12,6)) || lpObj->pChaosBox[n].m_Type == ITEMGET(13,30) )	
			{
				lpObj->ChaosLock = FALSE;

				return FALSE;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(12,15) ) // Chaos
			{
				ChoasGemCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(12,31) ) // Bundle of Soul
			{
				BundleOfSoulCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(12,30) ) // Bundle of Bless
			{
				BundleOfBlessCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(12,137) ) // Jewel of Creation Bundle
			{
				BundleOfCreationCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(12,140) ) // Harmony Bundle
			{
				BundleOfHarmonyCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,53) ) // Charm Of Luck
			{
				iCharmOfLuckCount += lpObj->pChaosBox[n].m_Durability;
			}
		/*	else if ( lpObj->pChaosBox[n].IsExtItem() != FALSE) //Exc Item
			{
				if ( lpObj->pChaosBox[n].m_Level >= 9 && lpObj->pChaosBox[n].m_Option3 >= 1 )
				{
					ExcItemCount++;
					iChaosMoney += lpObj->pChaosBox[n].m_BuyMoney;
				}
			}*/
		}
	}

	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;

	if ( BundleOfHarmonyCount != 0 && BundleOfCreationCount != 0 )
	{
		if ( ChoasGemCount != 1 || BundleOfSoulCount != 1 || BundleOfBlessCount != 1 || BundleOfCreationCount != 1 || BundleOfHarmonyCount < 1)
		{
			pMsg.Result = CB_INCORRECT_MIX_ITEMS;
			IOCP.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
			lpObj->ChaosLock = FALSE;

			return FALSE;
		}
	}
	else
	{
		lpObj->ChaosLock = FALSE;
		pMsg.Result = CB_INCORRECT_MIX_ITEMS;
		IOCP.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);

		return FALSE;
	}

	if ( iCharmOfLuckCount > 10 )
	{
		lpObj->ChaosLock = FALSE;
		pMsg.Result = 0xF0;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
	}

	lpObj->ChaosSuccessRate = 0;
	lpObj->ChaosSuccessRate += iChaosMoney / 3000000; //Good 40%

	if ( lpObj->ChaosSuccessRate < 1 )
	{
		lpObj->ChaosSuccessRate = 1;
	}

	else if ( lpObj->ChaosSuccessRate > g_ConfigRead.mix.Wing3MaxRate )
	{
		lpObj->ChaosSuccessRate = g_ConfigRead.mix.Wing3MaxRate;
	}

	lpObj->ChaosSuccessRate += iCharmOfLuckCount;

	int nChaosNeedMoney = lpObj->ChaosSuccessRate * 200000;
	int iChaosTaxMoney = (int)((__int64)nChaosNeedMoney * (__int64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (__int64)100);

	lpObj->ChaosSuccessRate = 100;
	if ( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	nChaosNeedMoney += iChaosTaxMoney;

	if ( nChaosNeedMoney < 0 )
	{
		nChaosNeedMoney = 0;
	}

	if ( lpObj->m_PlayerData->Money < nChaosNeedMoney )
	{
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		
		IOCP.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return TRUE;
	}

	LogChaosItem(lpObj, "AdvancedWing_Mix");
	g_Log.Add("[Advanced Wing Mix]Chaos Mix Start");
	lpObj->m_PlayerData->Money  -= nChaosNeedMoney;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GSProtocol.GCMoneySend(lpObj->m_Index, lpObj->m_PlayerData->Money);

	if ( (rand()%100) < lpObj->ChaosSuccessRate )
	{
		int iWingLevel = 0;
		int iItemType;
		int iItemSubType;
		int iWingType;

		iItemType = 12;
		iWingType = rand()%8;
		/*if(iWingType == 5)
		{
			iItemSubType = 43;
		}
		else if(iWingType == 6)
		{
			iItemSubType = 50;
		}
		else*/
		//{
			iItemSubType = iWingType + 51;
		//}
		//iItemSubType = rand()%5 + 36;
		
		int iWingNum = ITEMGET(iItemType, iItemSubType);
		int iOption1 = 0;
		int iOption2 = 0;
		int iOption3 = 0;

		if ( (rand()%5) == 0 )
		{
			iOption1 = 1;
		}

		int iRandomValue = rand() % 100;
		int iRandomValue2 = rand() % 3;

		switch ( iRandomValue2 )
		{
			case 0:
				if ( iRandomValue < 4 )
				{
					iOption2 = 3;	// +12
				}
				break;

			case 1:
				if ( iRandomValue < 10 )
				{
					iOption2 = 2;	// +8
				}
				break;

			case 2:
				if ( iRandomValue < 20 )
				{
					iOption2 = 1;	// +4;
				}
		}

		//int ExOption;

/*		if ( CondorFeatherCount != 0 && CondorFlameCount != 0)
		{ 
			if ( (rand()%5) == 0 )
			{
				iOption3 = 1 << (rand()%3);
			}

			if ( (rand()%2) != 0 )
			{
				iOption3 |= 0x20;
			}
		}*/
		
		::ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, iWingNum, iWingLevel, 0, 0, iOption1, iOption2, -1, iOption3, 0, 0, 0, 0);
		::gObjInventoryCommit(lpObj->m_Index);
		::g_Log.Add("[Advanced Wing Mix][%s][%s] CBMix Success %d Money : %d-%d, CharmRate : %d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->m_PlayerData->Money, nChaosNeedMoney, iCharmOfLuckCount);
		return TRUE;
	}
	else
	{
		this->ThirdWingMixFail(lpObj);
		GSProtocol.GCUserChaosBoxSend(lpObj, 0);
		IOCP.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		::g_Log.Add("[Advanced Wing Mix][%s][%s] CBMix Fail %d Money : %d-%d, CharmRate : %d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->m_PlayerData->Money, nChaosNeedMoney, iCharmOfLuckCount);
		
		lpObj->ChaosLock = FALSE;

		return FALSE;
	}
}

BOOL CMixSystem::ThirdWingLevel2ChaosMix(LPOBJ lpObj)
{
	lpObj->ChaosLock = TRUE;

	int ChoasGemCount = 0;
	int BundleOfSoulCount = 0;
	int BundleOfBlessCount = 0;
	int JewelOfCreationCount = 0;
	int CondorFlameCount = 0;
	int CondorFeatherCount = 0;
	int ExcItemCount = 0;
	DWORD iChaosMoney = 0;
	int iCharmOfLuckCount = 0;

	for (int n = 0; n < CHAOS_BOX_SIZE; n++)
	{
		if (lpObj->pChaosBox[n].IsItem() == TRUE)
		{
			if (lpObj->pChaosBox[n].m_Type == this->CHAOS_GEM_ITEMINDEX) // Chaos
			{
				ChoasGemCount++;
			}
			else if (lpObj->pChaosBox[n].m_Type == this->SOUL_GEM_BULDLE_ITEMINDEX) // Bundle of Soul
			{
				BundleOfSoulCount++;
			}
			else if (lpObj->pChaosBox[n].m_Type == this->BLESS_GEM_BUNDLE_ITEMINDEX) // Bundle of Bless
			{
				BundleOfBlessCount++;
			}
			else if (lpObj->pChaosBox[n].m_Type == this->CREATURE_GEM_ITEMINDEX) // Jewel of Creation
			{
				JewelOfCreationCount++;
			}
			else if (lpObj->pChaosBox[n].m_Type == this->CONDOR_FLAME_ITEMINDEX) // Condor Flame
			{
				CondorFlameCount++;
			}
			else if (lpObj->pChaosBox[n].m_Type == this->CONDOR_FEATHER_ITEMINDEX) // Condor Feather
			{
				CondorFeatherCount++;
			}
			else if (lpObj->pChaosBox[n].m_Type == ITEMGET(14, 53)) // Charm Of Luck
			{
				iCharmOfLuckCount += lpObj->pChaosBox[n].m_Durability;
			}

			else if (this->CheckLevelCondition(&lpObj->pChaosBox[n], 7, 0, 0, 1, 0, 1) != FALSE)
			{
				ExcItemCount++;
				iChaosMoney += lpObj->pChaosBox[n].m_BuyMoney;
			}
		}
	}

	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;

	if (CondorFeatherCount != 0 && CondorFlameCount != 0 && JewelOfCreationCount != 0)
	{
		if (ChoasGemCount != 1 || BundleOfSoulCount != 1 || BundleOfBlessCount != 1 || JewelOfCreationCount != 1 || ExcItemCount < 1)
		{
			pMsg.Result = CB_INCORRECT_MIX_ITEMS;
			IOCP.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
			lpObj->ChaosLock = FALSE;

			return FALSE;
		}
	}
	else
	{
		lpObj->ChaosLock = FALSE;
		pMsg.Result = CB_INCORRECT_MIX_ITEMS;
		IOCP.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);

		return FALSE;
	}

	if (iCharmOfLuckCount > 10)
	{
		lpObj->ChaosLock = FALSE;
		pMsg.Result = 0xF0;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
	}

	lpObj->ChaosSuccessRate = 0;
	lpObj->ChaosSuccessRate += iChaosMoney / 3000000; //Good 40%

	if (lpObj->ChaosSuccessRate < 1)
	{
		lpObj->ChaosSuccessRate = 1;
	}

	if (lpObj->m_PlayerData->VipType != 0)
	{
		if (lpObj->ChaosSuccessRate > g_VipSystem.GetWing3rdRate(lpObj))
		{
			lpObj->ChaosSuccessRate = g_VipSystem.GetWing3rdRate(lpObj);
		}
	}

	else
	{
		if (lpObj->ChaosSuccessRate > g_ConfigRead.mix.Wing3MaxRate)
		{
			lpObj->ChaosSuccessRate = g_ConfigRead.mix.Wing3MaxRate;
		}
	}

	lpObj->ChaosSuccessRate += iCharmOfLuckCount;

	int nChaosNeedMoney = lpObj->ChaosSuccessRate * 200000;
	int iChaosTaxMoney = (int)((__int64)nChaosNeedMoney * (__int64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (__int64)100);

	if (iChaosTaxMoney < 0)
	{
		iChaosTaxMoney = 0;
	}

	nChaosNeedMoney += iChaosTaxMoney;

	if (nChaosNeedMoney < 0)
	{
		nChaosNeedMoney = 0;
	}

	if (lpObj->m_PlayerData->Money < nChaosNeedMoney)
	{
		pMsg.Result = CB_NOT_ENOUGH_ZEN;

		IOCP.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return TRUE;
	}

	LogChaosItem(lpObj, "ThirdWingLevel2_Mix");
	g_Log.Add("[ThirdWing Mix][Level 02] Chaos Mix Start");
	lpObj->m_PlayerData->Money -= nChaosNeedMoney;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GSProtocol.GCMoneySend(lpObj->m_Index, lpObj->m_PlayerData->Money);

	if ((rand() % 100) < lpObj->ChaosSuccessRate)
	{
		int iWingType = rand() % 7;	//1.01.00 updated
		int iWingNum = 0;

		if (iWingType == 0)
		{
			iWingNum = this->WING_OF_STORM_ITEMINDEX;
		}
		else if (iWingType == 1)
		{
			iWingNum = this->WING_OF_ETERNAL_ITEMINDEX;
		}
		else if (iWingType == 2)
		{
			iWingNum = this->WING_OF_ILLUSION_ITEMINDEX;
		}
		else if (iWingType == 3)
		{
			iWingNum = this->WING_OF_RUIN_ITEMINDEX;
		}
		else if (iWingType == 4)
		{
			iWingNum = this->CAPE_OF_EMPEROR_ITEMINDEX;
		}
		else if (iWingType == 5)
		{
			iWingNum = this->WING_OF_DIMENSION_ITEMINDEX;
		}
		else if (iWingType == 6)	//1.01.00 updated
		{
			iWingNum = this->CAPE_OF_OVERRULE_ITEMINDEX;
		}

		int iOption1 = 0;

		if ((rand() % 1000) <= m_i3rdWingOpt_LuckRate)
		{
			iOption1 = 1;
		}

		int iOption2 = 0;
		int iExcOpt = 0;
		int iOptTypeRand = rand() % 2;
		int iOptRateRand;
		int loc28;
		int loc29;

		switch (iOptTypeRand)
		{
		case 0:
			iOptRateRand = rand() % 1000;
			if (iOptRateRand < 400)
			{
				iExcOpt |= 0x10;
			}
			break;

		case 1:
			iOptRateRand = rand() % 1000;
			if (iOptRateRand < 300)
			{
				iExcOpt |= 0x20;
			}
			break;
		}

		int iOpt3TypeLevel = rand() % 4;
		int iOpt3TypeLevelRate = rand() % 1000;

		switch (iOpt3TypeLevel)
		{
		case 1:
		{
			if (iOpt3TypeLevelRate < 120)
			{
				iOption2 = 1;
			}
		}
			break;

		case 2:
		{
			if (iOpt3TypeLevelRate < 60)
			{
				iOption2 = 2;
			}
		}
			break;

		case 3:
		{
			if (iOpt3TypeLevelRate < 30)
			{
				iOption2 = 3;
			}
		}
			break;
		}

		int iRandOpt = rand() % 4;
		int iRandOptRate = rand() % 1000;

		switch (iRandOpt)
		{
		case 0:
		{
			if (iRandOptRate < 40)
			{
				iExcOpt |= 1;
			}
		}
			break;
		case 1:
		{
			if (iRandOptRate < 20)
			{
				iExcOpt |= 2;
			}
		}
			break;

		case 2:
		{
			if (iRandOptRate < 70)
			{
				iExcOpt |= 4;
			}
		}
			break;
		case 3:
		{
			if (iRandOptRate < 70)
			{
				iExcOpt |= 8;
			}
		}
			break;
		}

		::ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, iWingNum, 0, 0, 0, iOption1, iOption2, -1, iExcOpt, 0, 0, 0, 0);
		::gObjInventoryCommit(lpObj->m_Index);
		::g_Log.Add("[ThirdWing Mix][Level 02] [%s][%s] CBMix Success %d Money : %d-%d, CharmRate : %d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->m_PlayerData->Money, nChaosNeedMoney, iCharmOfLuckCount);
		return TRUE;
	}
	else
	{
		this->ThirdWingMixFail(lpObj);
		GSProtocol.GCUserChaosBoxSend(lpObj, 0);
		IOCP.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		::g_Log.Add("[ThirdWing Mix][Level 02] [%s][%s] CBMix Fail %d Money : %d-%d, CharmRate : %d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->m_PlayerData->Money, nChaosNeedMoney, iCharmOfLuckCount);

		lpObj->ChaosLock = FALSE;

		return FALSE;
	}
}

void CMixSystem::ThirdWingMixFail(LPOBJ lpObj)
{
	if (lpObj->pChaosBox == NULL)
	{
		return;
	}

	for (int n = 0; n<CHAOS_BOX_SIZE; n++) //loc2
	{
		if (lpObj->pChaosBox[n].IsItem() == TRUE)
		{
			if (Check2ndWingItem(lpObj->pChaosBox[n].m_Type) != FALSE)
			{
				ThirdWingMixFailItemPanalty(&lpObj->pChaosBox[n]);
			}
			else if (CheckLevelCondition(&lpObj->pChaosBox[n], 0, 0, 0, 0, 1, 0) != FALSE)
			{
				ThirdWingMixFailItemPanalty(&lpObj->pChaosBox[n]);
			}
			else if (CheckLevelCondition(&lpObj->pChaosBox[n], 0, 0, 0, 0, 0, 1) != FALSE)
			{
				ThirdWingMixFailItemPanalty(&lpObj->pChaosBox[n]);
			}
			else
			{
				lpObj->pChaosBox[n].Clear();
			}
		}
	}
}

void CMixSystem::ThirdWingMixFailItemPanalty(CItem * lpItem)
{
	if (lpItem == NULL)
	{
		return;
	}

	if ((rand() % 2) < 1)
	{
		lpItem->m_Level -= 2;
	}
	else
	{
		lpItem->m_Level -= 3;
	}

	lpItem->m_Option3 = 0;
	lpItem->Convert(lpItem->m_Type, lpItem->m_Option1, lpItem->m_Option2, lpItem->m_Option3, lpItem->m_NewOption, lpItem->m_SetOption, lpItem->m_ItemOptionEx, lpItem->m_SocketOption, lpItem->m_BonusSocketOption, 0, CURRENT_DB_VERSION);
}

void CMixSystem::DevilSquareItemChaosMix(LPOBJ lpObj)
{
	lpObj->ChaosLock = TRUE;

	if ( g_DevilSquare.IsEventEnable() == true )
	{
		PMSG_CHAOSMIXRESULT pMsg;
		PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
		pMsg.Result = CB_ERROR;
		int aIndex = lpObj->m_Index;
		int eventitemcount;
		int itemlevel;
		int Ret = CheckDevilSquareItem(lpObj, eventitemcount, itemlevel);

		if ( Ret != FALSE )
		{
			if ( Ret == 3 )
			{
				pMsg.Result = CB_INCORRECT_MIX_ITEMS;
				IOCP.DataSend(aIndex, (BYTE*)&pMsg, pMsg.h.size);
				g_Log.Add("[DevilSquare] DiffLevel Devil's Key or Eyes [%d]", eventitemcount);
				lpObj->ChaosLock = FALSE;

				return;
			}
			
			if ( Ret == 2 )
			{
				pMsg.Result = CB_INCORRECT_MIX_ITEMS;
				IOCP.DataSend(aIndex, (BYTE*)&pMsg, pMsg.h.size);
				g_Log.Add("[DevilSquare] Not Found Chaos Gem [%d]", eventitemcount);
				lpObj->ChaosLock = FALSE;
				
				return;
			}

			if ( Ret == 4 )
			{
				pMsg.Result = 0xF0;
				IOCP.DataSend(aIndex, (BYTE*)&pMsg, pMsg.h.size);
				g_Log.Add("[DevilSquare] Charm Of Luck Over 10 [%d]", eventitemcount);
				lpObj->ChaosLock = FALSE;
				
				return;
			}

			
			if ( eventitemcount > 2 )
			{
				pMsg.Result = CB_TOO_MANY_ITEMS;
				IOCP.DataSend(aIndex, (BYTE*)&pMsg, pMsg.h.size);
				g_Log.Add("[DevilSquare] Too many Devil's Key or Eyes [%d]", eventitemcount);
				lpObj->ChaosLock = FALSE;

				return;
			}

			if ( lpObj->Level < 10 )
			{
				pMsg.Result = CB_LOW_LEVEL_USER;
				IOCP.DataSend(aIndex, (BYTE*)&pMsg, pMsg.h.size);
				lpObj->ChaosLock = FALSE;

				return;
			}

			DevilSquareEventChaosMix(lpObj, Ret, itemlevel);
			return;
		}
		
		if ( eventitemcount > 1 )
		{
			pMsg.Result = CB_LACKING_MIX_ITEMS;
			IOCP.DataSend(aIndex, (BYTE*)&pMsg, pMsg.h.size);
			lpObj->ChaosLock = FALSE;

			return;
		}
	}

	lpObj->ChaosLock = FALSE;
}

void CMixSystem::BloodCastleItemChaosMix(LPOBJ lpObj)
{
	lpObj->ChaosLock = TRUE;
	
	PMSG_CHAOSMIXRESULT pMsg;
	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;

	int aIndex = lpObj->m_Index;

	int iRET_VAL = g_BloodCastle.CheckChoasMixItem(aIndex);

	int cMapNum = g_BloodCastle.GetBridgeMapNumber(iRET_VAL-1);

	if ( BC_MAP_RANGE(cMapNum) != FALSE )
	{
		if ( g_BloodCastle.BloodCastleChaosMix(aIndex, iRET_VAL) == false )
		{
			lpObj->ChaosLock = FALSE;
		}

		return;
	}

	switch ( iRET_VAL )
	{
		case 9:
			pMsg.Result = CB_NO_BC_CORRECT_ITEMS	;
			IOCP.DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
			lpObj->ChaosLock = FALSE;
			break;

		case 10:
			pMsg.Result = CB_NO_BC_CORRECT_ITEMS	;
			IOCP.DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
			lpObj->ChaosLock = FALSE;
			break;

		case 11:
			pMsg.Result = CB_NO_BC_CORRECT_ITEMS	;
			IOCP.DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
			lpObj->ChaosLock = FALSE;
			break;

		case 12:
			pMsg.Result = CB_INVALID_ITEM_LEVEL	;
			IOCP.DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
			lpObj->ChaosLock = FALSE;
			break;

		case 13:
			pMsg.Result = CB_BC_NOT_ENOUGH_ZEN	;
			IOCP.DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
			lpObj->ChaosLock = FALSE;
			break;

		case 14:
			pMsg.Result = CB_USER_CLASS_LOW_LEVEL	;
			IOCP.DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
			lpObj->ChaosLock = FALSE;
			break;
		case 15:
			pMsg.Result = 0xF0;
			IOCP.DataSend(aIndex, (BYTE*)&pMsg, pMsg.h.size);
			lpObj->ChaosLock = FALSE;
			break;
		default:
			lpObj->ChaosLock = FALSE;
	}

}

void CMixSystem::SetItemChaosMix(LPOBJ lpObj)
{
	lpObj->ChaosLock = TRUE;

	int ChoasGemCount = 0;
	int BlessGemCount = 0;
	int SoulGemCount = 0;
	int MetalOfAncientCount = 0;
	int MixItemCount = 0;
	int MixSetItemIndex = 0;
	int iMixItemChaosMoney = 0;
	DWORD iChaosMoney = 0;
	int iMetalOfAncientItemLevel = -1;

	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,13) )
			{
				BlessGemCount++;

				if ( BlessGemCount > 3 )
				{
					iChaosMoney += lpObj->pChaosBox[n].m_BuyMoney;
				}
			}

			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,14) )
			{
				SoulGemCount++;

				if ( SoulGemCount > 3 )
				{
					iChaosMoney += lpObj->pChaosBox[n].m_BuyMoney;
				}
			}

			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,16) )
			{
				iChaosMoney += lpObj->pChaosBox[n].m_BuyMoney;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(12,15) )
			{
				ChoasGemCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Level >= 6 && lpObj->pChaosBox[n].IsExtItem() != FALSE)
			{
				if ( lpObj->pChaosBox[n].IsSetItem() != FALSE )
				{
					MixItemCount++;
					MixSetItemIndex = n;
					iMixItemChaosMoney += lpObj->pChaosBox[n].m_BuyMoney;
				}
			}
			else if ( ((lpObj->pChaosBox[n].m_Type >= ITEMGET(13,8) && lpObj->pChaosBox[n].m_Type < ITEMGET(13,14)) || (lpObj->pChaosBox[n].m_Type >= ITEMGET(13,20) && lpObj->pChaosBox[n].m_Type < ITEMGET(13,28) ) ) && ( lpObj->pChaosBox[n].m_Option3 >= 2  && lpObj->pChaosBox[n].m_Level >= 3 ) )	
			{
				if ( lpObj->pChaosBox[n].IsSetItem() != FALSE )
				{
					MixItemCount++;
					MixSetItemIndex = n;
					iMixItemChaosMoney += lpObj->pChaosBox[n].m_BuyMoney;
				}
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,27) ) // Ancien Metal, Apply Deathway Fix here
			{
				MetalOfAncientCount++;
				iMetalOfAncientItemLevel = lpObj->pChaosBox[n].m_Level;
			}
			else
			{
				IOCP.DataSend(lpObj->m_Index, (BYTE*)&pMsg, pMsg.h.size);
				lpObj->ChaosLock = FALSE;
			}
		}
	}

	if ( ChoasGemCount < 1 || BlessGemCount < 3 || SoulGemCount < 3 || MetalOfAncientCount != 1 || MixItemCount != 1 )
	{
		IOCP.DataSend(lpObj->m_Index, (BYTE*)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;
	}

	LogChaosItem(lpObj, "SetItemMix");
	g_Log.Add("[SetItemMix] Chaos Mix Start");

	if ( gSetItemOption.CheckMixContition(lpObj->pChaosBox[MixSetItemIndex].m_Type, iMetalOfAncientItemLevel ) == FALSE )
	{
		IOCP.DataSend(lpObj->m_Index, (BYTE*)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	lpObj->ChaosSuccessRate = iMixItemChaosMoney / 40000;
	lpObj->ChaosSuccessRate = iChaosMoney / 400000;

	if ( lpObj->ChaosSuccessRate > 80 )
	{
		lpObj->ChaosSuccessRate = 80;
	}

	int nChaosNeedMoney = (lpObj->ChaosSuccessRate - 50 ) * 1000000;

	if ( lpObj->m_PlayerData->Money < nChaosNeedMoney )
	{
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		IOCP.DataSend(lpObj->m_Index, (BYTE*)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	int setoption = gSetItemOption.MakeSetOption(lpObj->pChaosBox[MixSetItemIndex].m_Type, iMetalOfAncientItemLevel);

	if ( setoption == 0 )
	{
		pMsg.Result = CB_ERROR;
		IOCP.DataSend(lpObj->m_Index, (BYTE*)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	lpObj->m_PlayerData->Money -= nChaosNeedMoney;
	GSProtocol.GCMoneySend(lpObj->m_Index, lpObj->m_PlayerData->Money);

	if ( (rand()%100) < lpObj->ChaosSuccessRate )
	{
		lpObj->pChaosBox[MixSetItemIndex].m_SetOption = setoption;

		if ( (rand()%100 ) < 80 )
		{
			lpObj->pChaosBox[MixSetItemIndex].m_SetOption |= 4;
		}
		else
		{
			lpObj->pChaosBox[MixSetItemIndex].m_SetOption |= 8;
		}

		pMsg.Result = CB_SUCCESS;
		CItem Item;

		float Dur = ItemGetDurability(lpObj->pChaosBox[MixSetItemIndex].m_Type, lpObj->pChaosBox[MixSetItemIndex].m_Level,
			lpObj->pChaosBox[MixSetItemIndex].IsExtItem(), gSetItemOption.IsSetItem(lpObj->pChaosBox[MixSetItemIndex].m_Type));

		Item.m_Level = lpObj->pChaosBox[MixSetItemIndex].m_Level;
		Item.m_Durability = Dur * lpObj->pChaosBox[MixSetItemIndex].m_Durability / lpObj->pChaosBox[MixSetItemIndex].m_BaseDurability;
		Item.m_JewelOfHarmonyOption = lpObj->pChaosBox[MixSetItemIndex].m_JewelOfHarmonyOption;

		Item.Convert(lpObj->pChaosBox[MixSetItemIndex].m_Type, lpObj->pChaosBox[MixSetItemIndex].m_Option1, lpObj->pChaosBox[MixSetItemIndex].m_Option2,
			lpObj->pChaosBox[MixSetItemIndex].m_Option3, lpObj->pChaosBox[MixSetItemIndex].m_NewOption,
			lpObj->pChaosBox[MixSetItemIndex].m_SetOption, lpObj->pChaosBox[MixSetItemIndex].m_ItemOptionEx, lpObj->pChaosBox[MixSetItemIndex].m_SocketOption, lpObj->pChaosBox[MixSetItemIndex].m_BonusSocketOption, 0, CURRENT_DB_VERSION);

		Item.m_Number = lpObj->pChaosBox[MixSetItemIndex].m_Number;

		ItemByteConvert(pMsg.ItemInfo, Item);
		ChaosBoxInit(lpObj);
		::gObjChaosBoxInsertItemPos(lpObj->m_Index, Item, 0, -1);
		gObjChaosItemSet(lpObj->m_Index, 0, 1);

		IOCP.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);

		g_Log.Add("[SetItemMix] [%s][%s] CBMix Success %d Money : %d-%d (SetName:%s)", lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate,
			lpObj->m_PlayerData->Money, lpObj->ChaosMoney, gSetItemOption.GetSetOptionName(lpObj->pChaosBox[MixSetItemIndex].m_Type, setoption));
	}
	else
	{
		ChaosBoxInit(lpObj);
		GSProtocol.GCUserChaosBoxSend(lpObj, 0);

		IOCP.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);

		g_Log.Add("[SetItemMix] [%s][%s] CBMix Fail %d Money : %d-%d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->m_PlayerData->Money,
			nChaosNeedMoney);

		lpObj->ChaosLock = FALSE;
	}
}


void CMixSystem::DarkHorseChaosMix(LPOBJ lpObj)
{
	lpObj->ChaosLock = TRUE;
	int ChoasGemCount = 0;
	int BlessGemCount = 0;
	int SoulGemCount = 0;
	int CreatureGemCount = 0;
	int SoulOfDarkHorse = 0;
	DWORD iChaosMoney = 0;

	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;
	int iCharmOfLuckCount = 0;
	int iInvalidItemCount = 0;

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,13) )
			{
				BlessGemCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,14) )
			{
				SoulGemCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,22) )	// Creation
			{
				CreatureGemCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(12,15) )	// SUPE CHOAS HAHAHA
			{
				ChoasGemCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(13,31) && lpObj->pChaosBox[n].m_Level == 0 )	// Spirit
			{
				SoulOfDarkHorse++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,53) )	// Charm Of Luck
			{
				iCharmOfLuckCount += lpObj->pChaosBox[n].m_Durability;
			}
			else
			{
				iInvalidItemCount++;
			}
			

		}
	}

	if ( ChoasGemCount != 1 || BlessGemCount != 5 || SoulGemCount != 5 || CreatureGemCount != 1 || SoulOfDarkHorse != 1 || iInvalidItemCount > 0 )
	{
		IOCP.DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	if ( iCharmOfLuckCount > 10 )
	{
		pMsg.Result = 0xF0;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return ;
	}

	LogChaosItem(lpObj, "DarkHorseMix");
	g_Log.Add("[DarkHorseMix] Chaos Mix Start");
	lpObj->ChaosSuccessRate = 60;
	lpObj->ChaosSuccessRate += iCharmOfLuckCount;

	int nChaosNeedMoney = 5000000;

	int iChaosTaxMoney = (int)((__int64)nChaosNeedMoney * (__int64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (__int64)100);

	if ( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	nChaosNeedMoney += iChaosTaxMoney;

	if ( nChaosNeedMoney < 0 )
	{
		nChaosNeedMoney = 0;
	}

	if ( lpObj->m_PlayerData->Money < nChaosNeedMoney )
	{
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		
		IOCP.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	lpObj->m_PlayerData->Money -= nChaosNeedMoney;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GSProtocol.GCMoneySend(lpObj->m_Index, lpObj->m_PlayerData->Money);

	if ( (rand()%100) < lpObj->ChaosSuccessRate )
	{
		int Level = 0;
		int ItemType = 13;
		int ItemSubType = 4;
		int ItemNum = ITEMGET(ItemType, ItemSubType);
		int Option1 = 0;
		int Option2 = 0;
		int Option3 = 0;

		int Add = 0;

		if ( (rand()%100) < (lpObj->ChaosSuccessRate/5+6) )
		{
			Add = 1;
		}

		if ( (rand()%5) == 0 )
		{
			Option1 = 1;
		}

		int lc22 = rand()%100;	
		int lc23 = rand()%3;
		//int lc24 = lc23;

		switch ( lc23 )
		{
			case 0:
				if ( lc22 <  4 )
				{
					Option2 = 3;
				}
				break;

			case 1:
				if ( lc22 <  10 )
				{
					Option2 = 2;
				}
				break;

			case 2:
				if ( lc22 <  20 )
				{
					Option2 = 1;
				}
				break;
		}

		::PetItemSerialCreateSend(lpObj->m_Index, -2, 0, 0, ItemNum, Level, 0,Add,  Option1, Option2, -1, 0, 0); 
		::gObjInventoryCommit(lpObj->m_Index);
		g_Log.Add("[DarkHorseMix] [%s][%s] CBMix Success %d Money : %d-%d, CharmRate : %d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate,
			lpObj->m_PlayerData->Money, nChaosNeedMoney, iCharmOfLuckCount);
	}
	else
	{
		ChaosBoxInit(lpObj);
		GSProtocol.GCUserChaosBoxSend(lpObj, 1);
		IOCP.DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);

		g_Log.Add("[DarkHorseMix] [%s][%s] CBMix Fail %d Money : %d-%d, CharmRate : %d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate,
			lpObj->m_PlayerData->Money, nChaosNeedMoney, iCharmOfLuckCount);
		lpObj->ChaosLock = FALSE;
	}
}

void CMixSystem::DarkSpiritChaosMix(LPOBJ lpObj)
{
	lpObj->ChaosLock = TRUE;
	int ChoasGemCount = 0;
	int BlessGemCount = 0;
	int SoulGemCount = 0;
	int CreatureGemCount = 0;
	int SoulOfSpirit = 0;
	DWORD iChaosMoney = 0;

	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;
	int iCharmOfLuckCount = 0;
	int iInvalidItemCount = 0;

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,13) )
			{
				BlessGemCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,14) )
			{
				SoulGemCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,22) )	// Creation
			{
				CreatureGemCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(12,15) )	// SUPE CHOAS HAHAHA
			{
				ChoasGemCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(13,31) && lpObj->pChaosBox[n].m_Level == 1 )	// Spirit
			{
				SoulOfSpirit++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,53) )	// Charm Of Luck
			{
				iCharmOfLuckCount += lpObj->pChaosBox[n].m_Durability;
			}
			else
			{
				iInvalidItemCount++;
			}
		}
	}

	if ( ChoasGemCount != 1 || BlessGemCount != 2 || SoulGemCount != 2 || CreatureGemCount != 1 || SoulOfSpirit != 1 || iInvalidItemCount > 0 )
	{
		IOCP.DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	if ( iCharmOfLuckCount > 10 )
	{
		pMsg.Result = 0xF0;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return ;
	}

	LogChaosItem(lpObj, "DarkSpiritMix");
	g_Log.Add("[DarkSpiritMix] Chaos Mix Start");
	lpObj->ChaosSuccessRate = 60;
	lpObj->ChaosSuccessRate += iCharmOfLuckCount;

	int nChaosNeedMoney = 1000000;

	int iChaosTaxMoney = (int)((__int64)nChaosNeedMoney * (__int64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (__int64)100);

	if ( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	nChaosNeedMoney += iChaosTaxMoney;

	if ( nChaosNeedMoney < 0 )
	{
		nChaosNeedMoney = 0;
	}

	if ( lpObj->m_PlayerData->Money < nChaosNeedMoney )
	{
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		
		IOCP.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	lpObj->m_PlayerData->Money -= nChaosNeedMoney;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GSProtocol.GCMoneySend(lpObj->m_Index, lpObj->m_PlayerData->Money);

	if ( (rand()%100) < lpObj->ChaosSuccessRate )
	{
		int Level = 0;
		int ItemType = 13;
		int ItemSubType = 5;
		int ItemNum = ITEMGET(ItemType, ItemSubType);
		int Option1 = 0;
		int Option2 = 0;
		int Option3 = 0;

		int Add = 0;

		if ( (rand()%100) < (lpObj->ChaosSuccessRate/5+6) )
		{
			Add = 1;
		}

		if ( (rand()%5) == 0 )
		{
			Option1 = 1;
		}

		int lc22 = rand()%100;	
		int lc23 = rand()%3;
//		int lc24 = lc23;

		switch ( lc23 )
		{
			case 0:
				if ( lc22 <  4 )
				{
					Option2 = 3;
				}
				break;

			case 1:
				if ( lc22 <  10 )
				{
					Option2 = 2;
				}
				break;

			case 2:
				if ( lc22 <  20 )
				{
					Option2 = 1;
				}
				break;
		}

		::PetItemSerialCreateSend(lpObj->m_Index, -2, 0, 0, ItemNum, Level, 0, Add, Option1, Option2, -1, 0, 0); 
		::gObjInventoryCommit(lpObj->m_Index);
		g_Log.Add("[DarkSpiritMix] [%s][%s] CBMix Success %d Money : %d-%d, CharmRate : %d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate,
			lpObj->m_PlayerData->Money, nChaosNeedMoney, iCharmOfLuckCount);
	}
	else
	{
		ChaosBoxInit(lpObj);
		GSProtocol.GCUserChaosBoxSend(lpObj, 1);
		IOCP.DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);

		g_Log.Add("[DarkSpiritMix] [%s][%s] CBMix Fail %d Money : %d-%d, CharmRate : %d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate,
			lpObj->m_PlayerData->Money, nChaosNeedMoney, iCharmOfLuckCount);
		lpObj->ChaosLock = FALSE;
	}
}

void CMixSystem::BlessPotionChaosMix(LPOBJ lpObj)
{
	lpObj->ChaosLock = TRUE;
	int iBlessGemCount = 0;
	DWORD iChaosMoney = 0;
	int iInvalidItemCount = 0;

	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,13) )
			{
				iBlessGemCount++;
			}
			else
			{
				iInvalidItemCount++;
			}
		}
	}

	if ( iBlessGemCount == 0 )
		return;

	if ( iInvalidItemCount > 0 )
		return;

	if ( iBlessGemCount > 25 )
	{
		MsgOutput(lpObj->m_Index, Lang.GetText(0,182));
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	LogChaosItem(lpObj, "BlessPotionMix");
	g_Log.Add("[BlessPotionMix] Chaos Mix Start");
	lpObj->ChaosSuccessRate = 100;

	int nChaosNeedMoney = 100000;

	int iChaosTaxMoney = (int)((__int64)nChaosNeedMoney * (__int64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (__int64)100);

	if ( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	nChaosNeedMoney += iChaosTaxMoney;

	if ( nChaosNeedMoney < 0 )
	{
		nChaosNeedMoney = 0;
	}

	if ( lpObj->m_PlayerData->Money < nChaosNeedMoney )
	{
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		
		IOCP.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	lpObj->m_PlayerData->Money -= nChaosNeedMoney;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GSProtocol.GCMoneySend(lpObj->m_Index, lpObj->m_PlayerData->Money);

	if ( (rand()%100) < lpObj->ChaosSuccessRate )
	{
		int ItemNum = ITEMGET(14,7);
		int Level = 0;
		int Dur = iBlessGemCount * 10;

		ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, ItemNum, Level, Dur, 0, 0, 0, -1, 0, 0, 0, 0, 0);
		::gObjInventoryCommit(lpObj->m_Index);

		g_Log.Add("[BlessPotionMix] [%s][%s] CBMix Success %d Money : %d-%d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, 
			lpObj->m_PlayerData->Money, nChaosNeedMoney);
	}
	else
	{
		ChaosBoxInit(lpObj);
		::GSProtocol.GCUserChaosBoxSend(lpObj, 1);
		IOCP.DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);

		g_Log.Add("[BlessPotionMix] [%s][%s] CBMix Fail %d Money : %d-%d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, 
			lpObj->m_PlayerData->Money, nChaosNeedMoney);

		lpObj->ChaosLock = FALSE;
	}
}

void CMixSystem::SoulPotionChaosMix(LPOBJ lpObj)
{
	lpObj->ChaosLock = TRUE;
	int iSoulGemCount = 0;
	DWORD iChaosMoney = 0;
	int iInvalidItemCount = 0;

	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,14) )
			{
				iSoulGemCount++;
			}
			else
			{
				iInvalidItemCount++;
			}
		}
	}

	if ( iSoulGemCount == 0 )
		return;

	if ( iInvalidItemCount > 0 )
		return;

	if ( iSoulGemCount > 25 )
	{
		MsgOutput(lpObj->m_Index, Lang.GetText(0,182));
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	LogChaosItem(lpObj, "SoulPotionMix");
	g_Log.Add("[SoulPotionMix] Chaos Mix Start");
	lpObj->ChaosSuccessRate = 100;

	int nChaosNeedMoney = 50000;

	int iChaosTaxMoney = (int)((__int64)nChaosNeedMoney * (__int64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (__int64)100);

	if ( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	nChaosNeedMoney += iChaosTaxMoney;

	if ( nChaosNeedMoney < 0 )
	{
		nChaosNeedMoney = 0;
	}

	if ( lpObj->m_PlayerData->Money < nChaosNeedMoney )
	{
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		
		IOCP.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	lpObj->m_PlayerData->Money -= nChaosNeedMoney;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GSProtocol.GCMoneySend(lpObj->m_Index, lpObj->m_PlayerData->Money);

	if ( (rand()%100) < lpObj->ChaosSuccessRate )
	{
		int ItemNum = ITEMGET(14,7);
		int Level = 1;
		int Dur = iSoulGemCount * 10;

		ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, ItemNum, Level, Dur, 0, 0, 0, -1, 0, 0, 0, 0, 0);
		::gObjInventoryCommit(lpObj->m_Index);

		g_Log.Add("[SoulPotionMix] [%s][%s] CBMix Success %d Money : %d-%d",	// #error Change to SoulPotionMix
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, 
			lpObj->m_PlayerData->Money, nChaosNeedMoney);
	}
	else
	{
		ChaosBoxInit(lpObj);
		::GSProtocol.GCUserChaosBoxSend(lpObj, 1);
		IOCP.DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);

		g_Log.Add("[SoulPotionMix] [%s][%s] CBMix Fail %d Money : %d-%d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, 
			lpObj->m_PlayerData->Money, nChaosNeedMoney);

		lpObj->ChaosLock = FALSE;
	}
}



void CMixSystem::LifeStoneChaosMix(LPOBJ lpObj)
{
	lpObj->ChaosLock = TRUE;
	int iChoasGemCount = 0;
	int iBlessGemCount = 0;
	int iSoulGemCount = 0;
	int iProtectionGemCount = 0;
	int iInvalidItemCount = 0;
	DWORD iChaosMoney = 0;

	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,13) )
			{
				iBlessGemCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,14) )
			{
				iSoulGemCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,31) )	// Guardian Jewel
			{
				iProtectionGemCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(12,15) )	// Habla pe super CHOAS
			{
				iChoasGemCount++;
			}
			else 
			{
				iInvalidItemCount++;
			}
		}
	}

	if ( iChoasGemCount != 1 || iBlessGemCount != 5 || iSoulGemCount != 5 || iProtectionGemCount != 1 || iInvalidItemCount > 0 )
	{
		IOCP.DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	LogChaosItem(lpObj, "LifeStoneMix");
	g_Log.Add("[LifeStoneMix] Chaos Mix Start");
	lpObj->ChaosSuccessRate = 100;

	int nChaosNeedMoney = 5000000;

	int iChaosTaxMoney = (int)((__int64)nChaosNeedMoney * (__int64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (__int64)100);

	if ( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	nChaosNeedMoney += iChaosTaxMoney;

	if ( nChaosNeedMoney < 0 )
	{
		nChaosNeedMoney = 0;
	}

	if ( lpObj->m_PlayerData->Money < nChaosNeedMoney )
	{
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		
		IOCP.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	lpObj->m_PlayerData->Money -= nChaosNeedMoney;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GSProtocol.GCMoneySend(lpObj->m_Index, lpObj->m_PlayerData->Money);

	if ( (rand()%100) < lpObj->ChaosSuccessRate )
	{
		ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, ITEMGET(13,11) , 1, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0);
		::gObjInventoryCommit(lpObj->m_Index);

		g_Log.Add("[LifeStoneMix] [%s][%s] CBMix Success %d Money : %d-%d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, 
			lpObj->m_PlayerData->Money, nChaosNeedMoney);
	}
	else
	{
		ChaosBoxInit(lpObj);
		::GSProtocol.GCUserChaosBoxSend(lpObj, 1);
		IOCP.DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);

		g_Log.Add("[LifeStoneMix] [%s][%s] CBMix Fail %d Money : %d-%d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, 
			lpObj->m_PlayerData->Money, nChaosNeedMoney);

		lpObj->ChaosLock = FALSE;
	}
}

void CMixSystem::CastleSpecialItemMix(LPOBJ lpObj)
{
	lpObj->ChaosLock = TRUE;

	int iDefendGemCount = 0;
	int iBlessGemMixCount = 0;
	int iSoulGemMixCount = 0;
	int iInvalidItemCount = 0;
	DWORD iChaosMoney = 0;

	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;

	if( g_CastleSiegeSync.CheckCastleOwnerMember(lpObj->m_Index)  )
	{
		if( lpObj->m_PlayerData->GuildStatus != GUILD_MASTER )
		{
			g_Log.Add("[CastleSpecialMix] [%s][%s] Item Mix Failed - No Auth",
				lpObj->AccountID, lpObj->Name);

			IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

			lpObj->ChaosLock = FALSE;

			return;
		}
	}
	else
	{
		g_Log.Add("[CastleSpecialMix] [%s][%s] Item Mix Failed - No Auth",
			lpObj->AccountID, lpObj->Name);

		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

		lpObj->ChaosLock = FALSE;

		return;	
	}

	if( g_ConfigRead.server.GetServerType() != SERVER_CASTLE )
	{
		g_Log.Add("[CastleSpecialMix] [%s][%s] Item Mix Failed - Wrong Server",
			lpObj->AccountID, lpObj->Name);

		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

		lpObj->ChaosLock = FALSE;

		return;
	}

	if( g_CastleSiege.GetLordMixLimit() <= 0 )
	{
		g_Log.Add("[CastleSpecialMix] [%s][%s] Item Mix Failed - Mix Count Limit Over",
			lpObj->AccountID, lpObj->Name);

		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

		lpObj->ChaosLock = FALSE;

		return;		
	}

	for( int n = 0; n < CHAOS_BOX_SIZE; n++ )
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(12,30) )
			{
				iBlessGemMixCount += lpObj->pChaosBox[n].m_Level + 1;
			}
			else if( lpObj->pChaosBox[n].m_Type == ITEMGET(12,31) )
			{
				iSoulGemMixCount += lpObj->pChaosBox[n].m_Level + 1;
			}
			else if( lpObj->pChaosBox[n].m_Type == ITEMGET(14,31) )
			{
				iDefendGemCount++;
			}
			else
			{
				iInvalidItemCount++;
			}
		}
	}

	if( iBlessGemMixCount != 3 || iSoulGemMixCount != 3 || iDefendGemCount != 30 || iInvalidItemCount > 0 )
	{
		g_Log.Add("[CastleSpecialMix] [%s][%s] Item Mix Failed - Item Error (DG:%d, BGx10:%d, SGx10:%d, Other:%d)",
			lpObj->AccountID, lpObj->Name, iBlessGemMixCount, iSoulGemMixCount, iDefendGemCount, iInvalidItemCount);

		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

		lpObj->ChaosLock = FALSE;

		return;	
	}

	LogChaosItem(lpObj, "Castle Special Mix");
	g_Log.Add("[CastleSpecialMix] [%s][%s] Chaos Mix Start", lpObj->AccountID, lpObj->Name);

	lpObj->ChaosSuccessRate = 100;

	int nChaosNeedMoney = 1000000000;

	if( lpObj->m_PlayerData->Money < nChaosNeedMoney )
	{
		g_Log.Add("[CastleSpecialMix] [%s][%s] Item Mix Failed - Lack of Money (%d/%d)",
			lpObj->AccountID, lpObj->Name, lpObj->m_PlayerData->Money, nChaosNeedMoney);

		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

		lpObj->ChaosLock = FALSE;

		return;	
	}

	lpObj->m_PlayerData->Money -= nChaosNeedMoney;

	GSProtocol.GCMoneySend(lpObj->m_Index, lpObj->m_PlayerData->Money);

	if( (rand()%100) < lpObj->ChaosSuccessRate )
	{
		CItem Item;
		time_t Duration;

		if (g_BagManager.GetItemFromBag(lpObj->m_Index, BAG_EVENT, EVENTBAG_LORDMIX, lpObj->m_Index, Item, Duration) == false)
		{
			ChaosBoxInit(lpObj);
			GSProtocol.GCUserChaosBoxSend(lpObj, 1);

			IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

			lpObj->ChaosLock = FALSE;
			return;
		}

		ItemSerialCreateSend(lpObj->m_Index, 255, 0, 0, Item.m_Type, Item.m_Level, Item.m_Durability, Item.m_Option1, Item.m_Option2, Item.m_Option3, lpObj->m_Index, Item.m_NewOption, Item.m_SetOption, Duration, Item.m_SocketOption, Item.m_BonusSocketOption);
		gObjInventoryCommit(lpObj->m_Index);
		
		g_Log.Add("[CastleSpecialMix] [%s][%s] CBMix Success %d Money : %d-%d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->m_PlayerData->Money, nChaosNeedMoney);

		g_CastleSiege.DecreaseLordMixLimit();

	}
	else
	{
		ChaosBoxInit(lpObj);
		GSProtocol.GCUserChaosBoxSend(lpObj, 1);

		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

		g_Log.Add("[CastleSpecialMix] [%s][%s] CBMix Fail %d Money : %d-%d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->m_PlayerData->Money, nChaosNeedMoney);
	}

	lpObj->ChaosLock = FALSE;
}

void CMixSystem::HiddenTreasureBoxItemMix(LPOBJ lpObj)
{
	lpObj->ChaosLock = TRUE;
	int iBlueCrystal = 0;
	int iRedCrystal = 0;
	int iBlackCrystal = 0;
	int iInvalidItemCount = 0;
	DWORD iChaosMoney = 0;

	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(12,26) )
			{
				switch ( lpObj->pChaosBox[n].m_Level )
				{
					case 1:
						iRedCrystal++;
						break;
					case 2:
						iBlueCrystal++;
						break;
					case 3:
						iBlackCrystal++;
						break;
				}
			}
			else
			{
				iInvalidItemCount++;
			}
		}
	}

	int iCrystalMixType = -1;

	if ( iInvalidItemCount == 0 )
	{
		if ( iBlueCrystal == 7 )
		{
			if ( iRedCrystal == 0 && iBlackCrystal == 0 )
			{
				iCrystalMixType = 0;
				lpObj->ChaosSuccessRate = 100;
			}
		}
		else if ( iRedCrystal == 5 )
		{
			if ( iBlueCrystal == 0 && iBlackCrystal == 0 )
			{
				iCrystalMixType = 1;
				lpObj->ChaosSuccessRate = 100;
			}
		}
		else if ( iBlackCrystal == 3 )
		{
			if ( iRedCrystal == 0 && iBlueCrystal == 0 )
			{
				iCrystalMixType = 2;
				lpObj->ChaosSuccessRate = 100;
			}
		}
		else if ( iBlueCrystal == 1 && iRedCrystal == 1 && iBlackCrystal == 1 )
		{
			{
				iCrystalMixType = 3;
				lpObj->ChaosSuccessRate = 100;
			}
		}
		else
		{
			g_Log.Add("[Hidden TreasureBox Event] [%s][%s] Item Mix Failed",
				lpObj->AccountID, lpObj->Name);
		
			IOCP.DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);
			lpObj->ChaosLock = FALSE;
		}
	}
	else
	{
		g_Log.Add("[Hidden TreasureBox Event] [%s][%s] Item Mix Failed",
			lpObj->AccountID, lpObj->Name);
	
		IOCP.DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	LogChaosItem(lpObj, "Hidden TreasureBox Mix");
	g_Log.Add("[Hidden TreasureBox Event] [%s][%s] Chaos Mix Start",
		lpObj->AccountID, lpObj->Name);

	if ( (rand()%100) < lpObj->ChaosSuccessRate )
	{
		switch ( iCrystalMixType )
		{
			case 0:	case 1:	case 2:
				ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, ITEMGET(12,26) , 4, 1, 0, 0, 0, -1, 0, 0, 0, 0, 0);
				break;
			case 3:
				ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, ITEMGET(12,26) , 5, 1, 0, 0, 0, -1, 0, 0, 0, 0, 0);
				break;
			default:
				g_Log.Add("[Hidden TreasureBox Event] [%s][%s] CBMix Failed - iCrystalMixType is wrong : %d",
					lpObj->AccountID, lpObj->Name, iCrystalMixType);
				IOCP.DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);
				lpObj->ChaosLock = FALSE;
				return;
		}

		::gObjInventoryCommit(lpObj->m_Index);
		g_Log.Add("[Hidden TreasureBox Event] [%s][%s] CBMix Success:%d Type:%d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, iCrystalMixType);
	}
	else
	{
		g_Log.Add("[Hidden TreasureBox Event] [%s][%s] CBMix Fail Rate:%d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate);

		ChaosBoxInit(lpObj);
		::GSProtocol.GCUserChaosBoxSend(lpObj, 1);
		IOCP.DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);

		lpObj->ChaosLock = FALSE;
	}

	lpObj->ChaosLock = FALSE;
}



void CMixSystem::Fenrir_01Level_Mix(LPOBJ lpObj)
{
	lpObj->ChaosLock = TRUE;
	int iStuffCount_01 = 0;
	int iStuffCount_02 = 0;
	int iChaosGemCount = 0;
	int iInvalidItemCount = 0;
	DWORD iChaosMoney = 0;
	int iCharmOfLuckCount = 0;

	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(13,32) )
			{
				iStuffCount_01 += lpObj->pChaosBox[n].m_Durability;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(13,33) )
			{
				iStuffCount_02 += lpObj->pChaosBox[n].m_Durability;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(12,15) )
			{
				iChaosGemCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,53) )	// Charm Of Luck
			{
				iCharmOfLuckCount += lpObj->pChaosBox[n].m_Durability;
			}
			else
			{
				iInvalidItemCount++;
			}
		}
	}

	if ( iStuffCount_01 != 20 || iStuffCount_02 != 20 || iChaosGemCount != 1 || iInvalidItemCount > 0 )
	{
		IOCP.DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}
	
	if ( iCharmOfLuckCount > 10 )
	{
		pMsg.Result = 0xF0;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	LogChaosItem(lpObj, "Fenrir_01Level_Mix");
	g_Log.Add("[Fenrir Mix][Level 01] Chaos Mix Start");

	lpObj->ChaosSuccessRate = g_iFenrir_01Level_MixRate;
	lpObj->ChaosSuccessRate += iCharmOfLuckCount;
	int nChaosNeedMoney = 0;	// #error Set the Correct price before call the CstleSyegeTributes

	int iChaosTaxMoney = (int)((__int64)nChaosNeedMoney * (__int64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (__int64)100);

	if ( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	nChaosNeedMoney += iChaosTaxMoney;

	if ( nChaosNeedMoney < 0 )
	{
		nChaosNeedMoney = 0;
	}

	if ( lpObj->m_PlayerData->Money < nChaosNeedMoney )
	{
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		
		IOCP.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	lpObj->m_PlayerData->Money -= nChaosNeedMoney;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GSProtocol.GCMoneySend(lpObj->m_Index, lpObj->m_PlayerData->Money);

	if ( (rand()%100) < lpObj->ChaosSuccessRate )
	{
		int type = ITEMGET(13,35);
		int level = 0;
		int dur = 1;

		ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, type , level, dur, 0, 0, 0, -1, 0, 0, 0, 0, 0);
		::gObjInventoryCommit(lpObj->m_Index);

		g_Log.Add("[Fenrir Mix][Level 01] [%s][%s] CBMix Success %d Money : %d-%d, CharmRate : %d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->m_PlayerData->Money, nChaosNeedMoney,
			iCharmOfLuckCount);

	}
	else
	{
		ChaosBoxInit(lpObj);
		::GSProtocol.GCUserChaosBoxSend(lpObj, 0);
		IOCP.DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);

		g_Log.Add("[Fenrir Mix][Level 01] [%s][%s] CBMix Fail %d Money : %d-%d, CharmRate : %d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->m_PlayerData->Money, nChaosNeedMoney,
			iCharmOfLuckCount);

		lpObj->ChaosLock = FALSE;
	}
}

void CMixSystem::Fenrir_02Level_Mix(LPOBJ lpObj)
{
	lpObj->ChaosLock = TRUE;
	int iStuffCount_01 = 0;
	int iStuffCount_02 = 0;
	int iChaosGemCount = 0;
	int iInvalidItemCount = 0;
	DWORD iChaosMoney = 0;
	int iCharmOfLuckCount = 0;

	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(13,34) )
			{
				iStuffCount_01 += lpObj->pChaosBox[n].m_Durability;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(13,35) )
			{
				iStuffCount_02 ++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(12,15) )
			{
				iChaosGemCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,53) )	// Charm Of Luck
			{
				iCharmOfLuckCount += lpObj->pChaosBox[n].m_Durability;
			}
			else
			{
				iInvalidItemCount++;
			}
		}
	}

	if ( iStuffCount_01 != 10 || iStuffCount_02 != 5 || iChaosGemCount != 1 || iInvalidItemCount > 0 )
	{
		IOCP.DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}
	
	if ( iCharmOfLuckCount > 10 )
	{
		pMsg.Result = 0xF0;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	LogChaosItem(lpObj, "Fenrir_02Level_Mix");
	g_Log.Add("[Fenrir Mix][Level 02] Chaos Mix Start");

	lpObj->ChaosSuccessRate = g_iFenrir_02Level_MixRate;
	lpObj->ChaosSuccessRate += iCharmOfLuckCount;
	int nChaosNeedMoney = 0;	// #error Set the Correct price before call the CstleSyegeTributes

	int iChaosTaxMoney = (int)((__int64)nChaosNeedMoney * (__int64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (__int64)100);

	if ( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	nChaosNeedMoney += iChaosTaxMoney;

	if ( nChaosNeedMoney < 0 )
	{
		nChaosNeedMoney = 0;
	}

	if ( lpObj->m_PlayerData->Money < nChaosNeedMoney )
	{
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		
		IOCP.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	lpObj->m_PlayerData->Money -= nChaosNeedMoney;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GSProtocol.GCMoneySend(lpObj->m_Index, lpObj->m_PlayerData->Money);

	if ( (rand()%100) < lpObj->ChaosSuccessRate )
	{
		int type = ITEMGET(13,36);
		int level = 0;
		int dur = 1;

		ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, type , level, dur, 0, 0, 0, -1, 0, 0, 0, 0, 0);
		::gObjInventoryCommit(lpObj->m_Index);

		g_Log.Add("[Fenrir Mix][Level 02] [%s][%s] CBMix Success %d Money : %d-%d, CharmRate : %d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->m_PlayerData->Money, nChaosNeedMoney,
			iCharmOfLuckCount);

	}
	else
	{
		ChaosBoxInit(lpObj);
		::GSProtocol.GCUserChaosBoxSend(lpObj, 0);
		IOCP.DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);

		g_Log.Add("[Fenrir Mix][Level 02] [%s][%s] CBMix Fail %d Money : %d-%d, CharmRate : %d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->m_PlayerData->Money, nChaosNeedMoney,
			iCharmOfLuckCount);

		lpObj->ChaosLock = FALSE;
	}
}



void CMixSystem::Fenrir_03Level_Mix(LPOBJ lpObj)
{
	lpObj->ChaosLock = TRUE;
	int iStuffCount_01 = 0;
	int iLifeGemCount = 0;
	int iChaosGemCount = 0;
	int iInvalidItemCount = 0;
	DWORD iChaosMoney = 0;
	int iCharmOfLuckCount = 0;

	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(13,36) )
			{
				iStuffCount_01 ++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,16) )
			{
				iLifeGemCount ++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(12,15) )
			{
				iChaosGemCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,53) )	// Charm Of Luck
			{
				iCharmOfLuckCount += lpObj->pChaosBox[n].m_Durability;
			}
			else
			{
				iInvalidItemCount++;
			}
		}
	}

	if ( iStuffCount_01 != 1 || iLifeGemCount != 3 || iChaosGemCount != 1 || iInvalidItemCount > 0 )
	{
		IOCP.DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}
	
	if ( iCharmOfLuckCount > 10 )
	{
		pMsg.Result = 0xF0;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	LogChaosItem(lpObj, "Fenrir_03Level_Mix");
	g_Log.Add("[Fenrir Mix][Level 03] Chaos Mix Start");

	lpObj->ChaosSuccessRate = g_iFenrir_03Level_MixRate;
	lpObj->ChaosSuccessRate += iCharmOfLuckCount;
	int nChaosNeedMoney = 10000000;

	int iChaosTaxMoney = (int)((__int64)nChaosNeedMoney * (__int64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (__int64)100);

	if ( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	nChaosNeedMoney += iChaosTaxMoney;

	if ( nChaosNeedMoney < 0 )
	{
		nChaosNeedMoney = 0;
	}

	if ( lpObj->m_PlayerData->Money < nChaosNeedMoney )
	{
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		
		IOCP.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	lpObj->m_PlayerData->Money -= nChaosNeedMoney;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GSProtocol.GCMoneySend(lpObj->m_Index, lpObj->m_PlayerData->Money);

	if ( (rand()%100) < lpObj->ChaosSuccessRate )
	{
		int type = ITEMGET(13,37);
		int level = 0;
		int dur = 255;
		int op1 = 1;

		ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, type , level, dur, op1, 0, 0, -1, 0, 0, 0, 0, 0);
		::gObjInventoryCommit(lpObj->m_Index);

		g_Log.Add("[Fenrir Mix][Level 03] [%s][%s] CBMix Success %d Money : %d-%d, CharmRate : %d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->m_PlayerData->Money, nChaosNeedMoney,
			iCharmOfLuckCount);

	}
	else
	{
		ChaosBoxInit(lpObj);
		::GSProtocol.GCUserChaosBoxSend(lpObj, 0);
		IOCP.DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);

		g_Log.Add("[Fenrir Mix][Level 03] [%s][%s] CBMix Fail %d Money : %d-%d, CharmRate : %d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->m_PlayerData->Money, nChaosNeedMoney,
			iCharmOfLuckCount);

		lpObj->ChaosLock = FALSE;
	}
}

void CMixSystem::Fenrir_04Upgrade_Mix(LPOBJ lpObj)
{
	lpObj->ChaosLock = TRUE;
	int iStuffCount_01 = 0;
	int iLifeGemCount = 0;
	int iChaosGemCount = 0;
	int iAttackStuffCount = 0;
	int iDefendStuffCount = 0;
	int iAttackStuffPrice = 0;
	int iDefendStuffPrice = 0;
	int iInvalidItemCount = 0;
	DWORD iChaosMoney = 0;
	BOOL bFenrirDamageInc = FALSE;
	BOOL bFenrirDamageDec = FALSE;
	int iCharmOfLuckCount = 0;

	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(13,37) && lpObj->pChaosBox[n].IsPeriodItem() == FALSE )
			{
				iStuffCount_01 ++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,16) )
			{
				iLifeGemCount ++;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(12,15) )
			{
				iChaosGemCount++;
			}
			else if ( lpObj->pChaosBox[n].m_Type >= ITEMGET(0,0) &&
					  lpObj->pChaosBox[n].m_Type < ITEMGET(6,0) &&
					  lpObj->pChaosBox[n].m_Level >= 4 &&
					  lpObj->pChaosBox[n].m_Option3 >= 1)
			{
				iAttackStuffCount++;
				iAttackStuffPrice += lpObj->pChaosBox[n].m_BuyMoney;
			}
			else if ( lpObj->pChaosBox[n].m_Type >= ITEMGET(6,0) &&
					  lpObj->pChaosBox[n].m_Type < ITEMGET(12,0) &&
					  lpObj->pChaosBox[n].m_Level >= 4 &&
					  lpObj->pChaosBox[n].m_Option3 >= 1)
			{
				iDefendStuffCount++;
				iDefendStuffPrice += lpObj->pChaosBox[n].m_BuyMoney;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,53) )	// Charm Of Luck
			{
				iCharmOfLuckCount += lpObj->pChaosBox[n].m_Durability;
			}
			else
			{
				iInvalidItemCount++;
			}
		}
	}

	if ( iStuffCount_01 != 1 || iLifeGemCount != 5 || iChaosGemCount != 1 || ( iAttackStuffCount == 0 && iDefendStuffCount == 0 ) || iInvalidItemCount > 0 )
	{
		IOCP.DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}
	
	if ( iCharmOfLuckCount > 10 )
	{
		pMsg.Result = 0xF0;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	if ( iAttackStuffCount > 0 )
	{
		bFenrirDamageInc = TRUE;
		iChaosMoney = iAttackStuffPrice;
	}

	if ( iDefendStuffCount > 0 )
	{
		bFenrirDamageDec = TRUE;
		iChaosMoney = iDefendStuffPrice;
	}

	if ( bFenrirDamageInc && bFenrirDamageDec )
	{
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	if ( !bFenrirDamageInc && !bFenrirDamageDec )
	{
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	LogChaosItem(lpObj, "Fenrir_04Level_Mix");
	g_Log.Add("[Fenrir Mix][Level 04] Chaos Mix Start");

	int nChaosNeedMoney = 10000000;

	int iChaosTaxMoney = (int)((__int64)nChaosNeedMoney * (__int64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (__int64)100);

	if ( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	nChaosNeedMoney += iChaosTaxMoney;

	if ( nChaosNeedMoney < 0 )
	{
		nChaosNeedMoney = 0;
	}

	if ( lpObj->m_PlayerData->Money < nChaosNeedMoney )
	{
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		
		IOCP.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	lpObj->m_PlayerData->Money -= nChaosNeedMoney;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GSProtocol.GCMoneySend(lpObj->m_Index, lpObj->m_PlayerData->Money);

	if ( iChaosMoney > 1000000 )
		iChaosMoney = 1000000;

	lpObj->ChaosSuccessRate = iChaosMoney * 100 / 1000000;

	if ( lpObj->ChaosSuccessRate > 79 )
		lpObj->ChaosSuccessRate = 79;

	lpObj->ChaosSuccessRate += iCharmOfLuckCount;

	if ( (rand()%100) < lpObj->ChaosSuccessRate )
	{
		int type = ITEMGET(13,37);
		int level = 0;
		int dur = 255;
		int op1 = 1;
		int nop = 0;

		if ( bFenrirDamageInc )
		{
			nop |= 1;
		}

		if ( bFenrirDamageDec )
		{
			nop |= 2;
		}

		ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, type , level, dur, op1, 0, 0, -1, nop, 0, 0, 0, 0);
		::gObjInventoryCommit(lpObj->m_Index);

		g_Log.Add("[Fenrir Mix][Level 04] [%s][%s] CBMix Success %d Money : %d-%d, CharmRate : %d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->m_PlayerData->Money, nChaosNeedMoney,
			iCharmOfLuckCount);

	}
	else
	{
		ChaosBoxInit(lpObj);
		::GSProtocol.GCUserChaosBoxSend(lpObj, 0);
		IOCP.DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);

		g_Log.Add("[Fenrir Mix][Level 04] [%s][%s] CBMix Fail %d Money : %d-%d, CharmRate : %d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->m_PlayerData->Money, nChaosNeedMoney,
			iCharmOfLuckCount);

		lpObj->ChaosLock = FALSE;
	}
}



void CMixSystem::ShieldPotionLv1_Mix(LPOBJ lpObj)
{
	int iHealthPotionCount = 0;
	int iInvalidItemCount = 0;
	int iChaosMixPrice = 0;
	int iCharmOfLuckCount = 0;

	if ( g_ShieldSystemOn == FALSE )
		return;

	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,3) )
			{
				iHealthPotionCount+=lpObj->pChaosBox[n].m_Durability;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,53) )	// Charm Of Luck
			{
				iCharmOfLuckCount += lpObj->pChaosBox[n].m_Durability;
			}
			else
			{
				iInvalidItemCount++;
			}
		}
	}

	if ( iInvalidItemCount > 0 || iHealthPotionCount != 3 )
	{
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	if ( iCharmOfLuckCount > 10 )
	{
		pMsg.Result = 0xF0;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	iChaosMixPrice = g_iShieldPotionLv1MixMoney;
	int iChaosTaxMoney = iChaosMixPrice * g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / 100;

	if ( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	iChaosMixPrice += iChaosTaxMoney;

	if ( iChaosMixPrice < 0 )
	{
		iChaosMixPrice = 0;
	}

	if ( lpObj->m_PlayerData->Money < iChaosMixPrice )
	{
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		
		IOCP.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	lpObj->m_PlayerData->Money -= iChaosMixPrice;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GSProtocol.GCMoneySend(lpObj->m_Index, lpObj->m_PlayerData->Money);

	LogChaosItem(lpObj, "PotionMix][ShieldPotion Lv1 Mix");
	g_Log.Add("[PotionMix][ShieldPotion Lv1 Mix] - Mix Start");
	
	int iRate = rand() % 100;
	iRate -= iCharmOfLuckCount;

	if ( iRate < g_iShieldPotionLv1MixSuccessRate )
	{
		int ItemNum = ITEMGET(14,35);

		ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, ItemNum, 0, 1, 0, 0, 0, lpObj->m_Index, 0, 0, 0, 0, 0);
		::gObjInventoryCommit(lpObj->m_Index);

		g_Log.Add("[ChaosMix][Shield Potion] Lv1 Potion Mix Success [%s][%s], Money(%d-%d), CharmRate : %d",
			lpObj->AccountID, lpObj->Name, 	lpObj->m_PlayerData->Money, iChaosMixPrice, iCharmOfLuckCount);
	}
	else
	{
		ChaosBoxInit(lpObj);
		::GSProtocol.GCUserChaosBoxSend(lpObj, 0);
		IOCP.DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);

		g_Log.Add("[ChaosMix][Shield Potion] Lv1 Potion Mix [%s][%s] CBMix Fail %d Money : %d-%d, CharmRate : %d",
			lpObj->AccountID, lpObj->Name, 	g_iShieldPotionLv1MixSuccessRate,
			lpObj->m_PlayerData->Money, iChaosMixPrice, iCharmOfLuckCount);
	}

	lpObj->ChaosLock = FALSE;
}



void CMixSystem::ShieldPotionLv2_Mix(LPOBJ lpObj)
{
	int iHealthPotionCount = 0;
	int iInvalidItemCount = 0;
	int iChaosMixPrice = 0;
	int iCharmOfLuckCount = 0;

	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,38) )
			{
				iHealthPotionCount+=lpObj->pChaosBox[n].m_Durability;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,53) )	// Charm Of Luck
			{
				iCharmOfLuckCount += lpObj->pChaosBox[n].m_Durability;
			}
			else
			{
				iInvalidItemCount++;
			}
		}
	}

	if ( iInvalidItemCount > 0 || iHealthPotionCount != 3 )
	{
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	if ( iCharmOfLuckCount > 10 )
	{
		pMsg.Result = 0xF0;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	iChaosMixPrice = g_iShieldPotionLv2MixMoney;
	int iChaosTaxMoney = iChaosMixPrice * g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / 100;

	if ( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	iChaosMixPrice += iChaosTaxMoney;

	if ( iChaosMixPrice < 0 )
	{
		iChaosMixPrice = 0;
	}

	if ( lpObj->m_PlayerData->Money < iChaosMixPrice )
	{
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		
		IOCP.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	lpObj->m_PlayerData->Money -= iChaosMixPrice;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	
	LogChaosItem(lpObj, "PotionMix][ShieldPotion Lv2 Mix");
	g_Log.Add("[PotionMix][ShieldPotion Lv2 Mix] - Mix Start");

	GSProtocol.GCMoneySend(lpObj->m_Index, lpObj->m_PlayerData->Money);
	int iRate = rand() % 100;
	iRate -= iCharmOfLuckCount;

	if ( iRate < g_iShieldPotionLv2MixSuccessRate )
	{
		int ItemNum = ITEMGET(14,36);

		ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, ItemNum, 0, 1, 0, 0, 0, lpObj->m_Index, 0, 0, 0, 0, 0);
		::gObjInventoryCommit(lpObj->m_Index);

		g_Log.Add("[ChaosMix][Shield Potion] Lv2 Potion Mix Success [%s][%s], Money(%d-%d), CharmRate : %d",
			lpObj->AccountID, lpObj->Name, 	lpObj->m_PlayerData->Money, iChaosMixPrice, iCharmOfLuckCount);
	}
	else
	{
		ChaosBoxInit(lpObj);
		::GSProtocol.GCUserChaosBoxSend(lpObj, 0);
		IOCP.DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);

		g_Log.Add("[ChaosMix][Shield Potion] Lv2 Potion Mix [%s][%s] CBMix Fail %d Money : %d-%d, CharmRate : %d",
			lpObj->AccountID, lpObj->Name, 	g_iShieldPotionLv2MixSuccessRate,
			lpObj->m_PlayerData->Money, iChaosMixPrice, iCharmOfLuckCount);
	}

	lpObj->ChaosLock = FALSE;
}

void CMixSystem::ShieldPotionLv3_Mix(LPOBJ lpObj)
{
	int iHealthPotionCount = 0;
	int iInvalidItemCount = 0;
	int iChaosMixPrice = 0;
	int iCharmOfLuckCount = 0;

	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,39) )
			{
				iHealthPotionCount+=lpObj->pChaosBox[n].m_Durability;
			}
			else if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,53) )	// Charm Of Luck
			{
				iCharmOfLuckCount += lpObj->pChaosBox[n].m_Durability;
			}
			else
			{
				iInvalidItemCount++;
			}
		}
	}

	if ( iInvalidItemCount > 0 || iHealthPotionCount != 3 )
	{
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	if ( iCharmOfLuckCount > 10 )
	{
		pMsg.Result = 0xF0;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	iChaosMixPrice = g_iShieldPotionLv3MixMoney;
	int iChaosTaxMoney = iChaosMixPrice * g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / 100;

	if ( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	iChaosMixPrice += iChaosTaxMoney;

	if ( iChaosMixPrice < 0 )
	{
		iChaosMixPrice = 0;
	}

	if ( lpObj->m_PlayerData->Money < iChaosMixPrice )
	{
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		
		IOCP.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	lpObj->m_PlayerData->Money -= iChaosMixPrice;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GSProtocol.GCMoneySend(lpObj->m_Index, lpObj->m_PlayerData->Money);
	
	LogChaosItem(lpObj, "PotionMix][ShieldPotion Lv3 Mix");
	g_Log.Add("[PotionMix][ShieldPotion Lv3 Mix] - Mix Start");

	int iRate = rand() % 100;
	iRate -= iCharmOfLuckCount;

	if ( iRate < g_iShieldPotionLv3MixSuccessRate )
	{
		int ItemNum = ITEMGET(14,37);

		ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, ItemNum, 0, 1, 0, 0, 0, lpObj->m_Index, 0, 0, 0, 0, 0);
		::gObjInventoryCommit(lpObj->m_Index);

		g_Log.Add("[ChaosMix][Shield Potion] Lv3 Potion Mix Success [%s][%s], Money(%d-%d), CharmRate : %d",
			lpObj->AccountID, lpObj->Name, 	lpObj->m_PlayerData->Money, iChaosMixPrice, iCharmOfLuckCount);
	}
	else
	{
		ChaosBoxInit(lpObj);
		::GSProtocol.GCUserChaosBoxSend(lpObj, 0);
		IOCP.DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);

		g_Log.Add("[ChaosMix][Shield Potion] Lv3 Potion Mix [%s][%s] CBMix Fail %d Money : %d-%d, CharmRate : %d",
			lpObj->AccountID, lpObj->Name, 	g_iShieldPotionLv3MixSuccessRate,
			lpObj->m_PlayerData->Money, iChaosMixPrice, iCharmOfLuckCount);
	}

	lpObj->ChaosLock = FALSE;
}



void CMixSystem::LotteryItemMix(LPOBJ lpObj)
{
	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;

	int iLotteryItemCount = 0;
	int iCardID = 0;
	int iInvalidItemCount = 0;
	int iItemHeight = 0;
	int iItemWidth = 0;

	lpObj->ChaosLock = TRUE;

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			if ( lpObj->pChaosBox[n].m_Type == ITEMGET(14,54) || lpObj->pChaosBox[n].m_Type == ITEMGET(14,92) || lpObj->pChaosBox[n].m_Type == ITEMGET(14,93) || lpObj->pChaosBox[n].m_Type == ITEMGET(14,95) )
			{
				iLotteryItemCount++;
				iCardID = lpObj->pChaosBox[n].m_Type;
			}
			else
			{
				iInvalidItemCount++;
			}
		}
	}

	if ( iInvalidItemCount > 0 || iLotteryItemCount != 1 )
	{
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		if ( iLotteryItemCount != 1 )
		{
			g_Log.AddC(TColor::Red, "[CashShop][ChaosCardMix] User Try do Mix without having Chaos Card - User(ID:%s,Name:%s)", lpObj->AccountID, lpObj->Name);
		}

		return;
	}

	LogChaosItem(lpObj, "CashLottery");
	iItemHeight = 4;
	iItemWidth = 2;

	if ( !CheckInventoryEmptySpace(lpObj, iItemHeight, iItemWidth))
	{
		pMsg.Result = 0xF1;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	CItem LotteryItem;

	if(iCardID == ITEMGET(14,54))
	{
		if ( g_ChaosCard.GetItem(&LotteryItem) == -1 )
		{
			IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
			lpObj->ChaosLock = FALSE;
			g_Log.Add("[ChaosMix][ChaosCard] Lottery Item Mix Failed. Can't Get Item from List.");

			return;
		}
	}
	else if(iCardID == ITEMGET(14,92))
	{
		if ( g_ChaosCardGold.GetItem(&LotteryItem) == -1 )
		{
			IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
			lpObj->ChaosLock = FALSE;
			g_Log.Add("[ChaosMix][ChaosCard Gold] Lottery Item Mix Failed. Can't Get Item from List.");

			return;
		}
	}
	else if(iCardID == ITEMGET(14,93))
	{
		if ( g_ChaosCardRare.GetItem(&LotteryItem) == -1 )
		{
			IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
			lpObj->ChaosLock = FALSE;
			g_Log.Add("[ChaosMix][ChaosCard Rare] Lottery Item Mix Failed. Can't Get Item from List.");

			return;
		}
	}
	else if(iCardID == ITEMGET(14,95))
	{
		if ( g_ChaosCardMini.GetItem(&LotteryItem) == -1 )
		{
			IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
			lpObj->ChaosLock = FALSE;
			g_Log.Add("[ChaosMix][ChaosCard Mini] Lottery Item Mix Failed. Can't Get Item from List.");

			return;
		}
	}

	ItemSerialCreateSend(lpObj->m_Index, 255, 0, 0, LotteryItem.m_Type,
		LotteryItem.m_Level, LotteryItem.m_Durability,
		LotteryItem.m_Option1, LotteryItem.m_Option2, LotteryItem.m_Option3,
		lpObj->m_Index, LotteryItem.m_NewOption, 0, 0, 0, 0);

	gObjInventoryCommit(lpObj->m_Index);

	g_Log.Add("[ChaosMix][ChaosCardMix] Chaos Card Item Mix Success [%s][%s]",
		lpObj->AccountID, lpObj->Name);

	BYTE btExOption[MAX_EXOPTION_SIZE];

	ItemIsBufExOption(btExOption, &LotteryItem);
	
	g_Log.Add("[CashShop][ChaosCardMix] - User(ID:%s,Name:%s) Item(Name:%s,Type:%d,Level:%d,Dur:%d,Skill:%d,Luck:%d,AddOption:%d,Ex:(%d,%d,%d,%d,%d,%d))",
		lpObj->AccountID, lpObj->Name, ItemAttribute[LotteryItem.m_Type].Name,
		LotteryItem.m_Type, LotteryItem.m_Level, (int)LotteryItem.m_Durability,
		LotteryItem.m_Option1, LotteryItem.m_Option2, LotteryItem.m_Option3,
		btExOption[0], btExOption[1], btExOption[2], btExOption[3], btExOption[4], btExOption[5]);
}

void CMixSystem::SeedExtractMix(LPOBJ lpObj)
{
	int iExcItemCount = 0;
	int iSetItemCount = 0;
	int iHarmonyCount = 0;
	int iChoasGemCount = 0;
	int iCreationGemCount = 0;
	int iInvalidItemCount = 0;
	int iSeedExtractRate = 0;
	DWORD iChaosMoney = 0;
	int iMixMoney = 0;
	int iChaosTaxMoney = 0;
	int iSetItemNum = 0;
	int iExcItemNum = 0;
	int iHarmonyItemNum = 0;
	int iChoasGemItemNum = 0;
	int iCreationItemNum = 0;

	PMSG_CHAOSMIXRESULT pMsg;
	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;
	lpObj->ChaosLock = TRUE;

	for (int n = 0; n<CHAOS_BOX_SIZE; n++)
	{
		if (lpObj->pChaosBox[n].IsItem() == TRUE)
		{
			if (lpObj->pChaosBox[n].IsExtItem() == TRUE && lpObj->pChaosBox[n].m_Level >= 4)
			{
				iExcItemCount++;
				iChaosMoney += lpObj->pChaosBox[n].m_BuyMoney;
				iExcItemNum = n;
			}
			else if (lpObj->pChaosBox[n].IsSetItem() != FALSE && lpObj->pChaosBox[n].m_Level >= 4)
			{
				iSetItemCount++;
				iChaosMoney += lpObj->pChaosBox[n].m_BuyMoney;
				iSetItemNum = n;
			}
			else if (lpObj->pChaosBox[n].m_Type == ITEMGET(14, 42))
			{
				iHarmonyCount++;
				iHarmonyItemNum = n;
			}
			else if (lpObj->pChaosBox[n].m_Type == ITEMGET(12, 15))
			{
				iChoasGemCount++;
				iChoasGemItemNum = n;
			}
			else if (lpObj->pChaosBox[n].m_Type == ITEMGET(14, 22))
			{
				iCreationGemCount++;
				iCreationItemNum = n;
			}
			else
			{
				iInvalidItemCount++;
			}
		}
	}

	if (iInvalidItemCount > 0)
	{
		pMsg.Result = CB_INCORRECT_MIX_ITEMS;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;
	}

	if (iExcItemCount != 1 || iSetItemCount != 1 || iHarmonyCount != 1 || iChoasGemCount != 1 || iCreationGemCount != 1)
	{
		pMsg.Result = CB_INCORRECT_MIX_ITEMS;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;
	}

	iChaosTaxMoney = (int)((__int64) g_SocketOptionSystem.m_RequireMoneyForSeedExtract * (__int64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (__int64)100);

	if (iChaosTaxMoney < 0)
	{
		iChaosTaxMoney = 0;
	}

	iMixMoney = g_SocketOptionSystem.m_RequireMoneyForSeedExtract + iChaosTaxMoney;

	if (lpObj->m_PlayerData->Money < iMixMoney)
	{
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;
	}

	LogChaosItem(lpObj, "SeedExtract");
	lpObj->m_PlayerData->Money -= iMixMoney;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GSProtocol.GCMoneySend(lpObj->m_Index, lpObj->m_PlayerData->Money);

	iSeedExtractRate = 80 + (iChaosMoney / 2000000);

	if (iSeedExtractRate > 90)
	{
		iSeedExtractRate = 90;
	}

	if (rand() % 100 > iSeedExtractRate)
	{
		lpObj->pChaosBox[iSetItemNum].m_Level -= rand() % 3;
		lpObj->pChaosBox[iExcItemNum].m_Level -= rand() % 3;
		lpObj->pChaosBox[iHarmonyItemNum].Clear();
		lpObj->pChaosBox[iChoasGemItemNum].Clear();
		lpObj->pChaosBox[iCreationItemNum].Clear();
		GSProtocol.GCUserChaosBoxSend(lpObj, 0);
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		g_Log.Add("[%s][%s] Seed Extract Mix Failed.", lpObj->AccountID, lpObj->Name);
		return;
	}

	BYTE SocketOptionIndex = 0;
	SocketOptionIndex = g_SocketOptionSystem.GetExtractSeed();
	CSeedItem * lpSeedData = g_SocketOptionSystem.GetSeedData(SocketOptionIndex);

	if (lpSeedData != NULL)
	{
		pMsg.Result = 1;
		ItemSerialCreateSend(lpObj->m_Index, 0xFF, 0, 0, lpSeedData->m_ItemCode, lpSeedData->m_ItemLevel, 1, 0, 0, 0, lpObj->m_Index, 0, 0, 0, 0, 0);
		gObjInventoryCommit(lpObj->m_Index);
		g_Log.Add("[%s][%s] Seed Extract Mix Success - SeedInfo ( Index : %d, Type : %d, Level : %d, ItemCode : %d )",
			lpObj->AccountID, lpObj->Name, lpSeedData->m_SeedIndex, lpSeedData->m_SeedType, lpSeedData->m_ItemLevel, lpSeedData->m_ItemCode);
	}
	else
	{
		lpObj->pChaosBox[iSetItemNum].m_Level -= rand() % 3;
		lpObj->pChaosBox[iExcItemNum].m_Level -= rand() % 3;
		lpObj->pChaosBox[iHarmonyItemNum].Clear();
		lpObj->pChaosBox[iChoasGemItemNum].Clear();
		lpObj->pChaosBox[iCreationItemNum].Clear();
		GSProtocol.GCUserChaosBoxSend(lpObj, 0);
		g_Log.Add("[%s][%s] Seed Extract Mix Failed - SeedData is NULL", lpObj->AccountID, lpObj->Name);
		lpObj->ChaosLock = FALSE;
	}
}

void CMixSystem::SeedSphereCompositeMix(LPOBJ lpObj)
{
	int iSeedItemCount = 0; //2
	int iSphereItemCount = 0; //3
	int iInvalidItemCount = 0; //4
	int iChoasGemCount = 0; //5
	int iCreationGemCount = 0; //6
	int iChaosTaxMoney = 0; //7
	int iMixMoney = 0; //8
	DWORD iChaosMoney = 0; //9
	int iSeedSphereCompositeRate = 0; //10
	CItem * lpSeedItem = NULL; //11
	CItem * lpSphereItem = NULL; //12
	int iTetraGoldCount = 0;
	int iTetraSilverCount = 0;
	int iTetraBronzeCount = 0;
	int iSeedSpear3rdCount = 0;
	int iChaosJewel10BunchCount = 0;
	int iCreationJewel10BunchCount = 0;
	BOOL bMakeSeedSpear4th = FALSE;

	PMSG_CHAOSMIXRESULT pMsg;
	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;
	lpObj->ChaosLock = TRUE;

	for (int n = 0; n<CHAOS_BOX_SIZE; n++) //loc21
	{
		if (lpObj->pChaosBox[n].IsItem() == TRUE)
		{
			if (g_SocketOptionSystem.IsSeedItem(lpObj->pChaosBox[n].m_Type) == 1)
			{
				iSeedItemCount++;
				iChaosMoney += lpObj->pChaosBox[n].m_BuyMoney;
				lpSeedItem = &lpObj->pChaosBox[n];
			}

			else if (g_SocketOptionSystem.IsSphereItem(lpObj->pChaosBox[n].m_Type) == 1)
			{
				iSphereItemCount++;
				iChaosMoney += lpObj->pChaosBox[n].m_BuyMoney;
				lpSphereItem = &lpObj->pChaosBox[n];
			}

			else if (lpObj->pChaosBox[n].m_Type == ITEMGET(12, 15))
			{
				iChoasGemCount++;
			}

			else if (lpObj->pChaosBox[n].m_Type == ITEMGET(14, 22))
			{
				iCreationGemCount++;
			}

			else if (lpObj->pChaosBox[n].m_Type >= ITEMGET(12,112) && lpObj->pChaosBox[n].m_Type <= ITEMGET(12,117))
			{
				iSeedSpear3rdCount++;
				lpSeedItem = &lpObj->pChaosBox[n];
			}

			else if (lpObj->pChaosBox[n].m_Type == ITEMGET(14,245))
			{
				iTetraGoldCount++;
			}

			else if (lpObj->pChaosBox[n].m_Type == ITEMGET(14,246))
			{
				iTetraSilverCount++;
			}

			else if (lpObj->pChaosBox[n].m_Type == ITEMGET(14,247))
			{
				iTetraBronzeCount++;
			}

			else if (lpObj->pChaosBox[n].m_Type == ITEMGET(12,141) && lpObj->pChaosBox[n].m_Level == 0)
			{
				iChaosJewel10BunchCount++;
			}

			else if (lpObj->pChaosBox[n].m_Type == ITEMGET(12,137) && lpObj->pChaosBox[n].m_Level == 0)
			{
				iCreationJewel10BunchCount++;
			}

			else
			{
				iInvalidItemCount++;
			}
		}
	}

	if (iInvalidItemCount > 0)
	{
		pMsg.Result = CB_INCORRECT_MIX_ITEMS;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;
	}

	if (iSeedSpear3rdCount != 0)
	{
		if ( iSphereItemCount != 1 || iSeedSpear3rdCount != 1 || iChaosJewel10BunchCount != 1 || iCreationJewel10BunchCount != 1 )
		{
			pMsg.Result = CB_INCORRECT_MIX_ITEMS;
			IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
			lpObj->ChaosLock = FALSE;
			return;
		}

		bMakeSeedSpear4th = TRUE;
	}

	if (bMakeSeedSpear4th == FALSE && (iSphereItemCount != 1 || iSeedItemCount != 1 || iChoasGemCount != 1 || iCreationGemCount != 1))
	{
		pMsg.Result = CB_INCORRECT_MIX_ITEMS;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;
	}

	if (iTetraBronzeCount + iTetraSilverCount + iTetraGoldCount > 1)
	{
		pMsg.Result = CB_INCORRECT_MIX_ITEMS;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;
	}

	iChaosTaxMoney = (int)((__int64)g_SocketOptionSystem.m_RequireMoneyForSeedSphereComposite * (__int64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (__int64)100);

	if (iChaosTaxMoney < 0)
	{
		iChaosTaxMoney = 0;
	}

	iMixMoney = g_SocketOptionSystem.m_RequireMoneyForSeedSphereComposite + iChaosTaxMoney;

	if (lpObj->m_PlayerData->Money < iMixMoney)
	{
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;
	}

	LogChaosItem(lpObj, "SeedSphereComposite");

	if (bMakeSeedSpear4th == TRUE)
	{
		if (iTetraGoldCount == 1)
		{
			iSeedSphereCompositeRate = (iChaosMoney / 200000) + 80;
		}

		else if (iTetraSilverCount == 1)
		{
			iSeedSphereCompositeRate = (iChaosMoney / 200000) + 65;
		}

		else if (iTetraBronzeCount == 1)
		{
			iSeedSphereCompositeRate = (iChaosMoney / 200000) + 50;
		}

		else
		{
			iSeedSphereCompositeRate = (iChaosMoney / 200000) + 20;
		}
	}

	else
	{
		iSeedSphereCompositeRate = 80 + (iChaosMoney / 200000);
	}

	lpObj->m_PlayerData->Money -= iMixMoney;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GSProtocol.GCMoneySend(lpObj->m_Index, lpObj->m_PlayerData->Money);

	if (rand() % 100 > iSeedSphereCompositeRate) //fail rate
	{
		this->ChaosBoxInit(lpObj);
		GSProtocol.GCUserChaosBoxSend(lpObj, 0);
		g_Log.Add("[%s][%s] SeedSphere Composite Mix Failed.", lpObj->AccountID, lpObj->Name);
	}
	else
	{
		BYTE OptionIndex = 0;
		BYTE SphereLevel = 0; //58
		CSeedSphereItem SeedSphereItem;

		if (bMakeSeedSpear4th == TRUE)
		{
			OptionIndex = g_SocketOptionSystem.GetSeedIndexForSeedSpearItemCode(lpSeedItem->m_Type, lpSeedItem->m_Level);
		}

		else
		{
			OptionIndex = g_SocketOptionSystem.GetSeedIndex(lpSeedItem->m_Type, lpSeedItem->m_Level);
		}

		SphereLevel = g_SocketOptionSystem.GetSphereLevel(lpSphereItem->m_Type);

		g_SocketOptionSystem.MakeSeedSphere(&SeedSphereItem, OptionIndex, SphereLevel);

		if (SeedSphereItem.m_ItemCode != 0)
		{
			pMsg.Result = 1;
			ItemSerialCreateSend(lpObj->m_Index, 0xFF, 0, 0, SeedSphereItem.m_ItemCode, SeedSphereItem.m_ItemLevel, 1, 0, 0, 0, lpObj->m_Index, 0, 0, 0, 0, 0);
			gObjInventoryCommit(lpObj->m_Index);
			g_Log.Add("[%s][%s] SeedSphere Composite Mix Success - SeedSphere Info ( Index : %d, Type : %d, Level : %d, ItemCode : %d )",
				lpObj->AccountID, lpObj->Name, SeedSphereItem.m_SeedSphereIndex, SeedSphereItem.m_SeedType, SeedSphereItem.m_SphereLevel, SeedSphereItem.m_ItemCode);
			return;
		}

		this->ChaosBoxInit(lpObj);
		GSProtocol.GCUserChaosBoxSend(lpObj, 0);
		g_Log.Add("[%s][%s] SeedSphere Composite Mix Failed - SeedSphere Data is NULL", lpObj->AccountID, lpObj->Name);
	}

	lpObj->ChaosLock = FALSE;
}

void CMixSystem::SetSeedSphereMix(LPOBJ lpObj, BYTE btPos)
{
	int iSocketItemCount = 0; //2
	int iInvalidItemCount = 0; //3
	int iSeedSpearItemCount = 0; //4
	int iChoasGemCount = 0; //5
	int iCreationGemCount = 0; //6
	CItem * pSocketItem = NULL; //7
	CItem * pSeedSpearItem = NULL; //8
	CItem Item;
	int iChaosTaxMoney = 0;
	int iMixMoney = 0;

	PMSG_CHAOSMIXRESULT pMsg;
	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;

	if (btPos >= 5)
	{
		pMsg.Result = CB_INCORRECT_MIX_ITEMS;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;
	}

	lpObj->ChaosLock = TRUE;

	for (int n = 0; n<CHAOS_BOX_SIZE; n++) //loc21
	{
		if (lpObj->pChaosBox[n].IsItem() == TRUE)
		{
			if (g_SocketOptionSystem.IsEnableSocketItem(lpObj->pChaosBox[n].m_Type) == TRUE)
			{
				if (lpObj->pChaosBox[n].m_SocketOption[btPos] == 0xFE) //Slot with NO OPTION
				{
					iSocketItemCount++; //
					pSocketItem = &lpObj->pChaosBox[n];
				}
			}
			else if (g_SocketOptionSystem.IsSeedSphereItem(lpObj->pChaosBox[n].m_Type) == TRUE)
			{
				iSeedSpearItemCount++;
				pSeedSpearItem = &lpObj->pChaosBox[n];
			}
			else if (lpObj->pChaosBox[n].m_Type == ITEMGET(12, 15))
			{
				iChoasGemCount++;
			}
			else if (lpObj->pChaosBox[n].m_Type == ITEMGET(14, 22))
			{
				iCreationGemCount++;
			}
			else
			{
				iInvalidItemCount++;
			}
		}
	}

	if (iInvalidItemCount > 0)
	{
		pMsg.Result = CB_INCORRECT_MIX_ITEMS;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;
	}

	if (iSocketItemCount != 1 && iSeedSpearItemCount != 1 && iChoasGemCount != 1 && iCreationGemCount != 1)
	{
		pMsg.Result = CB_INCORRECT_MIX_ITEMS;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;
	}

	if (g_SocketOptionSystem.CheckItemForSameSeedType(pSocketItem, pSeedSpearItem->m_Type, pSeedSpearItem->m_Level) == 0)
	{
		pMsg.Result = CB_INCORRECT_MIX_ITEMS;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;
	}

	iChaosTaxMoney = (int)((__int64)g_SocketOptionSystem.m_RequireMoneyForSeedSphereSet * (__int64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (__int64)100);

	if (iChaosTaxMoney < 0)
	{
		iChaosTaxMoney = 0;
	}

	iMixMoney = g_SocketOptionSystem.m_RequireMoneyForSeedSphereSet + iChaosTaxMoney;

	if (lpObj->m_PlayerData->Money < iMixMoney)
	{
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;
	}

	LogChaosItem(lpObj, "SetSeedSphere");

	BYTE SocketByteOption = 0;
	SocketByteOption = g_SocketOptionSystem.GetSeedSphereIndexForItemCode(pSeedSpearItem->m_Type, pSeedSpearItem->m_Level);

	if (SocketByteOption == 0xFF)
	{
		pMsg.Result = 7;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		g_Log.Add("[%s][%s] Set Seed Mix Failed - SeedSphere is NULL", lpObj->AccountID, lpObj->Name);
		return;
	}

	if (g_SocketOptionSystem.CheckSetSeedSphereItemType(pSocketItem, SocketByteOption) == 0)
	{
		pMsg.Result = CB_INCORRECT_MIX_ITEMS;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		g_Log.Add("[%s][%s] Set Seed Mix Failed - Wrong Item Type for Set Socket", lpObj->AccountID, lpObj->Name);
		return;
	}

	lpObj->m_PlayerData->Money -= iMixMoney;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GSProtocol.GCMoneySend(lpObj->m_Index, lpObj->m_PlayerData->Money);

	g_SocketOptionSystem.SetSocketSlot(pSocketItem, SocketByteOption, btPos);

	float Dur = ItemGetDurability(pSocketItem->m_Type, pSocketItem->m_Level, pSocketItem->IsExtItem(), pSocketItem->IsSetItem());

	Item.m_Number = pSocketItem->m_Number;
	Item.m_Level = pSocketItem->m_Level;
	Item.m_Durability = Dur * pSocketItem->m_Durability / pSocketItem->m_BaseDurability;
	Item.m_JewelOfHarmonyOption = pSocketItem->m_JewelOfHarmonyOption;

	Item.Convert(pSocketItem->m_Type, pSocketItem->m_Option1, pSocketItem->m_Option2, pSocketItem->m_Option3, pSocketItem->m_NewOption,
		pSocketItem->m_SetOption, pSocketItem->m_ItemOptionEx, pSocketItem->m_SocketOption, pSocketItem->m_BonusSocketOption, 0, CURRENT_DB_VERSION);

	lpObj->ChaosLock = FALSE;

	ItemByteConvert(pMsg.ItemInfo, Item);
	this->ChaosBoxInit(lpObj);
	::gObjChaosBoxInsertItemPos(lpObj->m_Index, Item, 0, -1);
	gObjChaosItemSet(lpObj->m_Index, 0, 1);
	GSProtocol.GCUserChaosBoxSend(lpObj, 0);

	g_Log.Add("[%s][%s] Set SeedSphere Mix Success - ItemInfo ( Name : %s, ItemCode : %d, Level : %d, SocketOption[%d,%d,%d,%d,%d], BonusOption : %d )",
		lpObj->AccountID, lpObj->Name, ItemAttribute[Item.m_Type].Name, Item.m_Type, Item.m_Level, Item.m_SocketOption[0], Item.m_SocketOption[1],
		Item.m_SocketOption[2], Item.m_SocketOption[3], Item.m_SocketOption[4], Item.m_BonusSocketOption);

	IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
}

void CMixSystem::SeedSphereRemoveMix(LPOBJ lpObj, BYTE btPos)
{
	int iSocketItemCount = 0; //2
	int iInvalidItemCount = 0; //3
	int iSeedSphereItemCount = 0;
	int iSocketItemPosition = 0;
	int iSeedSpherePosition = 0;

	int iMixMoney = 0; //7
	int iChaosTaxMoney = 0; //8

	int iChaosJewelCount = 0;
	int iJewelOfHarmonyCount = 0;
	int iGemOfDefendCount = 0;
	
	int iChaosJewelPosition[5];
	int iJewelOfHarmonyPosition[5];

	int iGemOfDefendPosition = -1;

	iChaosJewelPosition[0] = -1;
	iChaosJewelPosition[1] = -1;
	iChaosJewelPosition[2] = -1;
	iChaosJewelPosition[3] = -1;
	iChaosJewelPosition[4] = -1;

	iJewelOfHarmonyPosition[0] = -1;
	iJewelOfHarmonyPosition[1] = -1;
	iJewelOfHarmonyPosition[2] = -1;
	iJewelOfHarmonyPosition[3] = -1;
	iJewelOfHarmonyPosition[4] = -1;

	CItem * lpSocketItem = NULL;

	PMSG_CHAOSMIXRESULT pMsg;
	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;

	if (btPos >= 5)
	{
		pMsg.Result = CB_INCORRECT_MIX_ITEMS;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;
	}

	lpObj->ChaosLock = TRUE;

	for (int n = 0; n<CHAOS_BOX_SIZE; n++) //loc21
	{
		if (lpObj->pChaosBox[n].IsItem() == TRUE)
		{
			if (g_SocketOptionSystem.IsEnableSocketItem(lpObj->pChaosBox[n].m_Type) == 1)
			{
				if (lpObj->pChaosBox[n].m_SocketOption[btPos] != 0xFE && lpObj->pChaosBox[n].m_SocketOption[btPos] != 0xFF) //Slot with OPTION
				{
					iSocketItemCount++; //
					iSocketItemPosition = n;
					lpSocketItem = &lpObj->pChaosBox[n];
				}
			}

			else if(lpObj->pChaosBox[n].m_Type == ITEMGET(12,15))
			{
				if (iChaosJewelCount >= 0 && iChaosJewelCount < 5)
				{
					iChaosJewelPosition[iChaosJewelCount] = n;
				}

				iChaosJewelCount++;
			}

			else if(lpObj->pChaosBox[n].m_Type == ITEMGET(14,31))
			{
				iGemOfDefendPosition = n;
				iGemOfDefendCount++;
			}

			else if(lpObj->pChaosBox[n].m_Type == ITEMGET(14,42))
			{
				if (iJewelOfHarmonyCount >= 0 && iJewelOfHarmonyCount < 5)
				{
					iJewelOfHarmonyPosition[iJewelOfHarmonyCount] = n;
				}

				iJewelOfHarmonyCount++;
			}			

			else
			{
				iInvalidItemCount++;
			}
		}
	}

	if (iInvalidItemCount > 0)
	{
		pMsg.Result = CB_INCORRECT_MIX_ITEMS;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;
	}

	if (iSocketItemCount != 1)
	{
		pMsg.Result = CB_INCORRECT_MIX_ITEMS;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;
	}

	if (iChaosJewelCount != 5 || iJewelOfHarmonyCount != 5 || iGemOfDefendCount != 1)
	{
		pMsg.Result = CB_INCORRECT_MIX_ITEMS;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;
	}

	iChaosTaxMoney = (int)((__int64)g_SocketOptionSystem.m_RequireMoneyForSeedSphereRemove * (__int64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (__int64)100);

	if (iChaosTaxMoney < 0)
	{
		iChaosTaxMoney = 0;
	}

	iMixMoney = g_SocketOptionSystem.m_RequireMoneyForSeedSphereRemove + iChaosTaxMoney;

	if (lpObj->m_PlayerData->Money < iMixMoney)
	{
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;
	}

	BYTE btSocketOption = lpObj->pChaosBox[iSocketItemPosition].m_SocketOption[btPos];
	BYTE btSeedIndex = btSocketOption % 50;
	BOOL bEmptyPlace = CheckInventoryEmptySpace(lpObj, 1, 1);

	if (bEmptyPlace == FALSE)
	{
		pMsg.Result = CB_LACKING_MIX_ITEMS;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;
	}

	CSeedItem * lpSeedData = g_SocketOptionSystem.GetSeedData(btSeedIndex);

	if (lpSeedData == NULL)
	{
		pMsg.Result = CB_INCORRECT_MIX_ITEMS;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;
	}

	lpObj->m_PlayerData->Money -= iMixMoney;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GSProtocol.GCMoneySend(lpObj->m_Index, lpObj->m_PlayerData->Money);

	LogChaosItem(lpObj, "SeedSphereRemove");

	if (rand() % 100 < 80)
	{
		g_SocketOptionSystem.ClearSocketSlot(&lpObj->pChaosBox[iSocketItemPosition], btPos);
		pMsg.Result = CB_SUCCESS;

		ItemSerialCreateSend(lpObj->m_Index, 219, 0, 0, lpSeedData->m_ItemCode, lpSeedData->m_ItemLevel, 1, 0, 0, 0, lpObj->m_Index, 0, 0, 0, 0, 0);
	}

	else
	{
		pMsg.Result = CB_ERROR;
	}
	
	int levelitemdur = ItemGetDurability(lpSocketItem->m_Type, lpSocketItem->m_Level, lpSocketItem->IsExtItem(), lpSocketItem->IsSetItem());

	CItem NewItem;
	NewItem.m_Number = lpSocketItem->m_Number;
	NewItem.m_Level = lpSocketItem->m_Level;
	NewItem.m_Durability = levelitemdur * lpSocketItem->m_Durability / lpSocketItem->m_BaseDurability;
	NewItem.m_JewelOfHarmonyOption = lpSocketItem->m_JewelOfHarmonyOption;

	NewItem.Convert(lpSocketItem->m_Type, lpSocketItem->m_Option1, lpSocketItem->m_Option2, lpSocketItem->m_Option3,
		lpSocketItem->m_NewOption, lpSocketItem->m_SetOption, lpSocketItem->m_ItemOptionEx,
		lpSocketItem->m_SocketOption, lpSocketItem->m_BonusSocketOption, 0, 3);

	lpObj->ChaosLock = FALSE;

	ItemByteConvert(pMsg.ItemInfo, NewItem);
	
	this->ChaosBoxInit(lpObj);
	gObjChaosBoxInsertItemPos(lpObj->m_Index, NewItem, 0, -1);
	
	gObjChaosItemSet(lpObj->m_Index, 0, 1);
	GSProtocol.GCUserChaosBoxSend(lpObj, 0);

	g_Log.Add("[%s][%s] SeedSphere Remove Success - ItemInfo ( Name : %s, ItemCode : %d, Level : %d, SocketOption[%d,%d,%d,%d,%d], BonusOption : %d )",
		lpObj->AccountID, lpObj->Name, ItemAttribute[lpObj->pChaosBox[iSocketItemPosition].m_Type].Name, lpObj->pChaosBox[iSocketItemPosition].m_Type,
		lpObj->pChaosBox[iSocketItemPosition].m_Level, lpObj->pChaosBox[iSocketItemPosition].m_SocketOption[0], lpObj->pChaosBox[iSocketItemPosition].m_SocketOption[1],
		lpObj->pChaosBox[iSocketItemPosition].m_SocketOption[2], lpObj->pChaosBox[iSocketItemPosition].m_SocketOption[3], lpObj->pChaosBox[iSocketItemPosition].m_SocketOption[4], lpObj->pChaosBox[iSocketItemPosition].m_BonusSocketOption);

	IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
}

void CMixSystem::SecromiconMix(LPOBJ lpObj)
{
	PMSG_CHAOSMIXRESULT pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x86, sizeof(pMsg));
	int first = 0;
	int second = 0;
	int third = 0;
	int fourth = 0;
	int fifth = 0;
	int sixth = 0;
	int invalid = 0;

	lpObj->ChaosLock = TRUE;

	for(int n=0;n<CHAOS_BOX_SIZE;++n)
	{
		if(lpObj->pChaosBox[n].IsItem() == TRUE)
		{
			if(lpObj->pChaosBox[n].m_Type == ITEMGET(14,103))
				first++;
			else if(lpObj->pChaosBox[n].m_Type == ITEMGET(14,104))
				second++;
			else if(lpObj->pChaosBox[n].m_Type == ITEMGET(14,105))
				third++;
			else if(lpObj->pChaosBox[n].m_Type == ITEMGET(14,106))
				fourth++;
			else if(lpObj->pChaosBox[n].m_Type == ITEMGET(14,107))
				fifth++;
			else if(lpObj->pChaosBox[n].m_Type == ITEMGET(14,108))
				sixth++;
			else
				invalid++;
		}
	}

	if(invalid)
	{
		pMsg.Result = CB_INCORRECT_MIX_ITEMS;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	if(first == 1 && second == 1 && third == 1 && fourth == 1 && fifth == 1 && sixth == 1)
	{
		int iMixCost = g_ImperialGuardian.GetMixNeedZen();	
		int iMixTax = iMixCost * g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / 100;
		if(iMixTax < 0)
		iMixTax = 0;
		iMixCost += iMixTax;
		if(lpObj->m_PlayerData->Money < iMixCost)
		{
			pMsg.Result = CB_NOT_ENOUGH_ZEN;
			IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
			lpObj->ChaosLock = FALSE;
			return;
		}
		LogChaosItem(lpObj, "SecromiconMix");
		lpObj->m_PlayerData->Money -= iMixCost;
		g_CastleSiegeSync.AddTributeMoney(iMixTax);
		GSProtocol.GCMoneySend(lpObj->m_Index, lpObj->m_PlayerData->Money);

		ItemSerialCreateSend(lpObj->m_Index, 255, 0, 0,ITEMGET(14,109), 0, 0, 0, 0, 0, lpObj->m_Index, 0, 0, 0, 0, 0);
		gObjInventoryCommit(lpObj->m_Index);

		g_Log.Add("[SecromiconMix] (%s)(%s) Success ItemID: %d", lpObj->AccountID, lpObj->Name, ITEMGET(14,109));
	}
	lpObj->ChaosLock = FALSE;
}

void CMixSystem::PremiumBoxMix(LPOBJ lpObj)
{
	PMSG_CHAOSMIXRESULT pMsg = {0};
	PHeadSetB((LPBYTE)&pMsg, 0x86, sizeof(pMsg));
	char SealedSilver = 0;
	char SealedGold = 0;
	char SilverKey = 0;
	char GoldKey = 0;
	char Invalid = 0;
	lpObj->ChaosLock = TRUE;

	for(int n=0;n<CHAOS_BOX_SIZE;++n)
	{
		if(lpObj->pChaosBox[n].IsItem() == TRUE)
		{
			if(lpObj->pChaosBox[n].m_Type == ITEMGET(14,112))
				SilverKey++;
			else if(lpObj->pChaosBox[n].m_Type == ITEMGET(14,113))
				GoldKey++;
			else if(lpObj->pChaosBox[n].m_Type == ITEMGET(14,121))
				SealedGold++;
			else if(lpObj->pChaosBox[n].m_Type == ITEMGET(14,122))
				SealedSilver++;
			else
				Invalid++;
		}
	}

	if(Invalid)
	{
		pMsg.Result = CB_INCORRECT_MIX_ITEMS;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}
	LogChaosItem(lpObj, "PremiumBoxMix");
	if(SealedSilver == 1 && SilverKey == 1)
	{
		ItemSerialCreateSend(lpObj->m_Index, 255, 0, 0,ITEMGET(14,124), 0, 0, 0, 0, 0, lpObj->m_Index, 0, 0, 0, 0, 0);
		gObjInventoryCommit(lpObj->m_Index);

		g_Log.Add("[PremiumBoxMix] (%s)(%s) Success: Silver Box", lpObj->AccountID, lpObj->Name);

		lpObj->ChaosLock = FALSE;
		return;
	}
	else if(SealedGold == 1 && GoldKey == 1)
	{
		ItemSerialCreateSend(lpObj->m_Index, 255, 0, 0,ITEMGET(14,123), 0, 0, 0, 0, 0, lpObj->m_Index, 0, 0, 0, 0, 0);
		gObjInventoryCommit(lpObj->m_Index);

		g_Log.Add("[PremiumBoxMix] (%s)(%s) Success: Golden Box", lpObj->AccountID, lpObj->Name);

		lpObj->ChaosLock = FALSE;
		return;
	}
	else
	{
		pMsg.Result = CB_INCORRECT_MIX_ITEMS;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}
}

void CMixSystem::CherryBlossomMix(LPOBJ lpObj)
{
	PMSG_CHAOSMIXRESULT pMsg;
	int iWhiteCherryBlossom = 0;
	int iWhiteCherryBlossomDur = 0;
	int iRedCherryBlossom = 0;
	int iRedCherryBlossomDur = 0;
	int iGoldCherryBlossom = 0;
	int iGoldCherryBlossomDur = 0;
	int iInvalidItemCount = 0;
	int iMixType = -1;

	PHeadSetB((LPBYTE)&pMsg, 0x86, sizeof(pMsg));
	pMsg.Result = CB_ERROR;
	
	lpObj->ChaosLock = TRUE;

	for (int n = 0; n < CHAOS_BOX_SIZE; n++)
	{
		if (lpObj->pChaosBox[n].IsItem() == TRUE)
		{
			if (lpObj->pChaosBox[n].m_Type == ITEMGET(14, 88))
			{
				iWhiteCherryBlossom++;
				iWhiteCherryBlossomDur += lpObj->pChaosBox[n].m_Durability;
				iMixType = 0;
			}

			else if (lpObj->pChaosBox[n].m_Type == ITEMGET(14, 89))
			{
				iRedCherryBlossom++;
				iRedCherryBlossomDur += lpObj->pChaosBox[n].m_Durability;
				iMixType = 1;
			}

			else if (lpObj->pChaosBox[n].m_Type == ITEMGET(14, 90))
			{
				iGoldCherryBlossom++;
				iGoldCherryBlossomDur += lpObj->pChaosBox[n].m_Durability;
				iMixType = 2;
			}

			else
			{
				iInvalidItemCount++;
				iMixType = -1;
			}
		}
	}

	if (iInvalidItemCount != 0)
	{
		pMsg.Result = CB_INCORRECT_MIX_ITEMS;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;
	}

	if (iMixType == 0)
	{
		if (iWhiteCherryBlossomDur != g_ConfigRead.mix.CherryBlossomWhiteNeedCount)
		{
			pMsg.Result = CB_INCORRECT_MIX_ITEMS;
			IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
			lpObj->ChaosLock = FALSE;
			return;
		}
	}

	else if (iMixType == 1)
	{
		if (iRedCherryBlossomDur != g_ConfigRead.mix.CherryBlossomRedNeedCount)
		{
			pMsg.Result = CB_INCORRECT_MIX_ITEMS;
			IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
			lpObj->ChaosLock = FALSE;
			return;
		}
	}

	else if (iMixType == 2)
	{
		if (iGoldCherryBlossomDur != g_ConfigRead.mix.CherryBlossomGoldNeedCount)
		{
			pMsg.Result = CB_INCORRECT_MIX_ITEMS;
			IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
			lpObj->ChaosLock = FALSE;
			return;
		}
	}

	else
	{
		pMsg.Result = CB_INCORRECT_MIX_ITEMS;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;
	}

	LogChaosItem(lpObj, "CherryBlosssomMix");

	CItem Item;
	time_t Duration;
	int BagType;

	if (iMixType == 0)
	{
		BagType = EVENTBAG_CHERRYBLOSSOM_WHITE;
	}

	else if (iMixType == 1)
	{
		BagType = EVENTBAG_CHERRYBLOSSOM_RED;
	}

	else if (iMixType == 2)
	{
		BagType = EVENTBAG_CHERRYBLOSSOM_GOLD;
	}

	if (g_BagManager.GetItemFromBag(lpObj->m_Index, BAG_EVENT, BagType, lpObj->m_Index, Item, Duration) == false)
	{
		ChaosBoxInit(lpObj);
		GSProtocol.GCUserChaosBoxSend(lpObj, 0);
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;
	}

	ItemSerialCreateSend(lpObj->m_Index, 255, 0, 0, Item.m_Type, Item.m_Level, Item.m_Durability, Item.m_Option1, Item.m_Option2, Item.m_Option3, lpObj->m_Index, Item.m_NewOption, Item.m_SetOption, Duration, Item.m_SocketOption, Item.m_BonusSocketOption);

	pMsg.Result = CB_SUCCESS;
	IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

	g_Log.Add("[CherryBlossomMix] Item Mix Success [%s][%s]",
		lpObj->AccountID, lpObj->Name);

	lpObj->ChaosLock = FALSE;

}

void CMixSystem::ItemRefineMix(LPOBJ lpObj)
{
	lpObj->ChaosLock = TRUE;

	WORD ItemType = 0xFFFF;

	BYTE SonicItem = 0;
	BYTE CycloneItem = 0;
	BYTE BlastItem = 0;
	BYTE MagmaItem = 0;
	BYTE HornItem = 0;
	BYTE AngelicItem = 0;
	BYTE DevilItem = 0;
	BYTE SpiteItem = 0;
	BYTE AsuraItem = 0;
	BYTE ThunderboltItem = 0;
	BYTE MiracleItem = 0;
	BYTE SummonItem = 0;
	BYTE IceWalkerItem = 0;
	BYTE MammothItem = 0;
	BYTE GiantItem = 0;
	BYTE CoolutinItem = 0;
	BYTE IronKnightItem = 0;
	BYTE DarkMammothItem = 0;
	BYTE DarkGiantItem = 0;
	BYTE DarkCoolutinItem = 0;
	BYTE DarkIronKnightItem = 0;
	BYTE SocketItem = 0;
	int SocketValue = 0;
	BYTE Invalid = 0;
	int iCharmOfLuckCount = 0;

	PMSG_CHAOSMIXRESULT pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x86, sizeof(pMsg));
	pMsg.Result = CB_ERROR;
	
	for(int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if(lpObj->pChaosBox[n].IsItem() == TRUE)
		{
			if(lpObj->pChaosBox[n].m_Type == ITEMGET(14,191))
			{
				SonicItem++;
				ItemType = ITEMGET(0,29);
			}

			else if(lpObj->pChaosBox[n].m_Type == ITEMGET(14,192))
			{
				CycloneItem++;
				ItemType = ITEMGET(0,36);
			}

			else if(lpObj->pChaosBox[n].m_Type == ITEMGET(14,193))
			{
				BlastItem++;
				ItemType = ITEMGET(0,37);
			}

			else if(lpObj->pChaosBox[n].m_Type == ITEMGET(14,194))
			{
				MagmaItem++;
				ItemType = ITEMGET(3,12);
			}

			else if(lpObj->pChaosBox[n].m_Type == ITEMGET(14,195))
			{
				HornItem++;
				ItemType = ITEMGET(2,20);
			}

			else if(lpObj->pChaosBox[n].m_Type == ITEMGET(14,196))
			{
				AngelicItem++;
				ItemType = ITEMGET(4,25);
			}

			else if(lpObj->pChaosBox[n].m_Type == ITEMGET(14,197))
			{
				DevilItem++;
				ItemType = ITEMGET(4,26);
			}

			else if(lpObj->pChaosBox[n].m_Type == ITEMGET(14,198))
			{
				MiracleItem++;
				ItemType = ITEMGET(5,35);
			}

			else if(lpObj->pChaosBox[n].m_Type == ITEMGET(14,199))
			{
				SpiteItem++;
				ItemType = ITEMGET(5,37);
			}

			else if(lpObj->pChaosBox[n].m_Type == ITEMGET(14,200))
			{
				AsuraItem++;
				ItemType = ITEMGET(0,30);
			}
			
			else if(lpObj->pChaosBox[n].m_Type == ITEMGET(14,202))
			{
				SummonItem++;
				ItemType = ITEMGET(5,32);
			}

			else if(lpObj->pChaosBox[n].m_Type == ITEMGET(14,201))
			{
				ThunderboltItem++;
				ItemType = ITEMGET(2,19);
			}

			else if(lpObj->pChaosBox[n].m_Type == ITEMGET(14,180))
			{
				IceWalkerItem++;
			}

			else if(lpObj->pChaosBox[n].m_Type == ITEMGET(14,181))
			{
				MammothItem++;
			}

			else if(lpObj->pChaosBox[n].m_Type == ITEMGET(14,182))
			{
				GiantItem++;
			}

			else if(lpObj->pChaosBox[n].m_Type == ITEMGET(14,183))
			{
				CoolutinItem++;
			}

			else if(lpObj->pChaosBox[n].m_Type == ITEMGET(14,184))
			{
				IronKnightItem++;
			}

			else if(lpObj->pChaosBox[n].m_Type == ITEMGET(14,185))
			{
				DarkMammothItem++;
			}

			else if(lpObj->pChaosBox[n].m_Type == ITEMGET(14,186))
			{
				DarkGiantItem++;
			}

			else if(lpObj->pChaosBox[n].m_Type == ITEMGET(14,187))
			{
				DarkCoolutinItem++;
			}

			else if(lpObj->pChaosBox[n].m_Type == ITEMGET(14,188))
			{
				DarkIronKnightItem++;
			}

			else if(g_SocketOptionSystem.IsSocketItem(&lpObj->pChaosBox[n]) && lpObj->pChaosBox[n].m_Level >= 7 && lpObj->pChaosBox[n].m_Option3 >= 1)
			{
				SocketItem++;
				SocketValue += lpObj->pChaosBox[n].m_BuyMoney;
			}

			else
			{
				if(lpObj->pChaosBox[n].m_Type == ITEMGET(14,53))
				{
					iCharmOfLuckCount += 10;
				}
				else
				{
					Invalid++;
				}
			}
		}
	}

	if(Invalid)
	{
		pMsg.Result = CB_INCORRECT_MIX_ITEMS;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;
	}

	bool Error = false;

	switch ( ItemType )
	{
		case ITEMGET(0,29):
			if(SonicItem != 1 || DarkIronKnightItem != 1 || IceWalkerItem != 2)
			{
				Error = true;
			}
			break;
		case ITEMGET(0,36):
			if(CycloneItem != 1 || DarkMammothItem != 1 || MammothItem != 2)
			{
				Error = true;
			}
			break;
		case ITEMGET(0,37):
			if(BlastItem != 1 || DarkIronKnightItem != 1 || GiantItem != 2)
			{
				Error = true;
			}
			break;
		case ITEMGET(3,12):
			if(MagmaItem != 1 || DarkMammothItem != 1 || CoolutinItem != 2)
			{
				Error = true;
			}
			break;
		case ITEMGET(2,20):
			if(HornItem != 1 || DarkGiantItem != 1 || GiantItem != 2)
			{
				Error = true;
			}
			break;
		case ITEMGET(4,25):
			if(AngelicItem != 1 || DarkMammothItem != 1 || IceWalkerItem != 2)
			{
				Error = true;
			}
			break;
		case ITEMGET(4,26):
			if(DevilItem != 1 || DarkCoolutinItem != 1 || IronKnightItem != 2)
			{
				Error = true;
			}
			break;
		case ITEMGET(5,35):
			if(MiracleItem != 1 || DarkMammothItem != 1 || GiantItem != 2)
			{
				Error = true;
			}
			break;
		case ITEMGET(5,37):
			if(SpiteItem != 1 || DarkGiantItem != 1 || IronKnightItem != 2)
			{
				Error = true;
			}
			break;
		case ITEMGET(0,30):
			if(AsuraItem != 1 || DarkIronKnightItem != 1 || IronKnightItem != 2)
			{
				Error = true;
			}
			break;
		case ITEMGET(5,32):
			if(SummonItem != 1 || DarkMammothItem != 1 || IronKnightItem != 2)
			{
				Error = true;
			}
			break;
		case ITEMGET(2,19):
			if(ThunderboltItem != 1 || DarkCoolutinItem != 1 || MammothItem != 2)
			{
				Error = true;
			}
			break;
	}

	if(SocketItem != 3)
	{
		Error = true;
	}

	if(Invalid)
	{
		pMsg.Result = CB_INCORRECT_MIX_ITEMS;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;
	}

	LogChaosItem(lpObj, "SocketWeaponMix");
	g_Log.Add("[SocketWeaponMix] Chaos Mix Start");

	int nChaosNeedMoney = g_ConfigRead.mix.SocketWeaponRequireMoney;

	int iChaosTaxMoney = (int)((__int64)nChaosNeedMoney * (__int64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (__int64)100);

	if ( iChaosTaxMoney < 0 )
	{
		iChaosTaxMoney = 0;
	}

	nChaosNeedMoney += iChaosTaxMoney;

	if ( nChaosNeedMoney < 0 )
	{
		nChaosNeedMoney = 0;
	}

	if ( lpObj->m_PlayerData->Money < nChaosNeedMoney )
	{
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		
		IOCP.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	lpObj->m_PlayerData->Money -= nChaosNeedMoney;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GSProtocol.GCMoneySend(lpObj->m_Index, lpObj->m_PlayerData->Money);

	lpObj->ChaosSuccessRate = SocketValue / 360000;
	lpObj->ChaosSuccessRate += iCharmOfLuckCount;

	if (lpObj->ChaosSuccessRate > g_ConfigRead.mix.SocketWeaponMaxRate)
	{
		lpObj->ChaosSuccessRate = g_ConfigRead.mix.SocketWeaponMaxRate;
	}

	lpObj->ChaosMoney = g_ConfigRead.mix.SocketWeaponRequireMoney;

	if ( rand() % 100 <= lpObj->ChaosSuccessRate )
	{
		CItem Item;
		Item.m_Type = ItemType;

		Item.m_Level = 0;
		Item.m_Option2 = (rand()%10000) < 2000 ? TRUE : FALSE;
		Item.m_Option1 = TRUE;
		Item.m_Option3 = 0;

		BYTE SocketOption[5];

		for (int i = 0; i < 5; i++)
		{
			SocketOption[i] = -1;
		}

		ItemSerialCreateSend(lpObj->m_Index, 255, 0, 0,Item.m_Type, Item.m_Level, 0, Item.m_Option1, Item.m_Option2, Item.m_Option3, lpObj->m_Index, Item.m_NewOption, 0, 0, SocketOption, 0);
		gObjInventoryCommit(lpObj->m_Index);

		g_Log.Add("[SocketWeaponMix] [%s][%s] Mix Success.", lpObj->AccountID, lpObj->Name);
	}
	else
	{
		ChaosBoxInit(lpObj);
		GSProtocol.GCUserChaosBoxSend(lpObj, 0);
		IOCP.DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);
		g_Log.Add("[SocketWeaponMix] [%s][%s] Mix Failed.", lpObj->AccountID, lpObj->Name);
	}

	lpObj->ChaosLock = FALSE;
}

void CMixSystem::MonsterWingMix(LPOBJ lpObj)
{
	lpObj->ChaosLock = TRUE;

	int WingCount = 0;
	int ChaosCount = 0;
	int CreationCount = 0;
	int DeathKingMaterialCount = 0;
	int HellMaineMaterialCount = 0;
	int DarkPhoenixMaterialCount = 0;
	int DeathBeamMaterialCount = 0;
	BOOL IsItemCape = FALSE;
	int InvalidItemCount = 0;
	int LuckyHarmAddRate = 0;
	DWORD ChaosMoney = 0;

	for (int i = 0; i < CHAOS_BOX_SIZE; i++)
	{
		if (lpObj->pChaosBox[i].IsItem() == TRUE)
		{
			if (lpObj->pChaosBox[i].m_Type == ITEMGET(12, 3) || lpObj->pChaosBox[i].m_Type == ITEMGET(12, 4) ||
				lpObj->pChaosBox[i].m_Type == ITEMGET(12, 5) || lpObj->pChaosBox[i].m_Type == ITEMGET(12, 6) ||
				lpObj->pChaosBox[i].m_Type == ITEMGET(13, 30) || lpObj->pChaosBox[i].m_Type == ITEMGET(12, 42) ||
				lpObj->pChaosBox[i].m_Type == ITEMGET(12, 49))
			{
				WingCount++;
				ChaosMoney = lpObj->pChaosBox[i].m_BuyMoney;

				if (lpObj->pChaosBox[i].m_Type == ITEMGET(13, 30) || lpObj->pChaosBox[i].m_Type == ITEMGET(12, 49))
				{
					IsItemCape = TRUE;
				}
			}

			else
			{
				switch (lpObj->pChaosBox[i].m_Type)
				{
				case ITEMGET(12, 15):
					ChaosCount++;
					break;
				case ITEMGET(14, 22):
					CreationCount++;
					break;
				case ITEMGET(14, 176):
					DeathKingMaterialCount++;
					break;
				case ITEMGET(14, 177):
					HellMaineMaterialCount++;
					break;
				case ITEMGET(14, 178):
					DarkPhoenixMaterialCount++;
					break;
				case ITEMGET(14, 179):
					DeathBeamMaterialCount++;
					break;
				case ITEMGET(14, 53):
					LuckyHarmAddRate = lpObj->pChaosBox[i].m_Durability;
					break;
				default:
					InvalidItemCount++;
					break;
				}
			}
		}
	}

	PMSG_CHAOSMIXRESULT pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x86, sizeof(pMsg));
	pMsg.Result = CB_ERROR;

	if (InvalidItemCount || WingCount != 1 || ChaosCount != 1 || CreationCount != 1 || (DeathKingMaterialCount + HellMaineMaterialCount + DarkPhoenixMaterialCount + DeathBeamMaterialCount) != 1)
	{
		g_Log.Add("[ERROR][CBMIX][MONSTER_WING][Invalid Recipe Item][%s][%s]", lpObj->AccountID, lpObj->Name);
		
		lpObj->ChaosLock = FALSE;
		pMsg.Result = CB_INCORRECT_MIX_ITEMS;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	if (LuckyHarmAddRate > 10)
	{
		lpObj->ChaosLock = FALSE;
		pMsg.Result = 0xF0;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	if (IsItemCape == TRUE)
	{
		lpObj->ChaosSuccessRate = ChaosMoney / 500000;
	}

	else
	{
		lpObj->ChaosSuccessRate = ChaosMoney / 9000000;
	}

	if (lpObj->ChaosSuccessRate < 0)
		lpObj->ChaosSuccessRate = 0;

	if (lpObj->ChaosSuccessRate == 0)
	{
		g_Log.Add("[ERROR][CBMIX][MONSTER_WING][Success rate is 0][%s][%s]", lpObj->AccountID, lpObj->Name);

		lpObj->ChaosLock = FALSE;
		pMsg.Result = CB_INCORRECT_MIX_ITEMS;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	LogChaosItem(lpObj, "MonsterWingMix");

	if (lpObj->m_PlayerData->VipType != 0)
	{
		if (lpObj->ChaosSuccessRate > g_VipSystem.GetWing25Rate(lpObj))
		{
			lpObj->ChaosSuccessRate = g_VipSystem.GetWing25Rate(lpObj);
		}
	}
	else
	{
		if (lpObj->ChaosSuccessRate > g_ConfigRead.mix.Wing25MaxRate)
		{
			lpObj->ChaosSuccessRate = g_ConfigRead.mix.Wing25MaxRate;
		}
	}

	 int nChaosNeedMoney = 100000 * lpObj->ChaosSuccessRate;

	int iChaosTaxMoney = (int)((__int64)nChaosNeedMoney * (__int64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (__int64)100);

	if (iChaosTaxMoney < 0)
	{
		iChaosTaxMoney = 0;
	}

	nChaosNeedMoney += iChaosTaxMoney;

	if (nChaosNeedMoney < 0)
	{
		nChaosNeedMoney = 0;
	}

	if (lpObj->m_PlayerData->Money < nChaosNeedMoney)
	{
		pMsg.Result = CB_NOT_ENOUGH_ZEN;

		IOCP.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	lpObj->ChaosSuccessRate += LuckyHarmAddRate;
	lpObj->m_PlayerData->Money -= nChaosNeedMoney;

	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GSProtocol.GCMoneySend(lpObj->m_Index, lpObj->m_PlayerData->Money);

	if (rand() % 100 < lpObj->ChaosSuccessRate)
	{
		int WingType;

		if (DeathKingMaterialCount == 1)
		{
			WingType = 262;
		}

		else if (HellMaineMaterialCount == 1)
		{
			WingType = 263;
		}

		else if (DarkPhoenixMaterialCount == 1)
		{
			WingType = 264;
		}

		else if (DeathBeamMaterialCount == 1)
		{
			WingType = 265;
		}

		else
		{
			g_Log.Add("[ERROR][CBMIX][MONSTER_WING][NOT MATCH MATERIAL][%s][%s]", lpObj->AccountID, lpObj->Name);
			
			lpObj->ChaosLock = FALSE;
			pMsg.Result = CB_INCORRECT_MIX_ITEMS;
			IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
			return;
		}

		int Option2 = FALSE;

		if (rand()%1000 <= 200)
			Option2 = TRUE;

		int Option3 = 0;

		int iRandValue = rand() % 1000;

		if (iRandValue >= 120)
		{
			if (iRandValue >= 180)
			{
				if (iRandValue >= 210)
				{
					Option3 = 0;
				}

				else
				{
					Option3 = 3;
				}
			}

			else
			{
				Option3 = 2;
			}
		}

		else
		{
			Option3 = 1;
		}

		int NOption = 0;

		if (rand() % 2 == 0)
		{
			NOption = 16;
		}

		int iRandomValue = rand() % 1000;
		int iRandomValue2 = rand() % 2;

		if (iRandomValue2 == 1 && iRandomValue < 70)
		{
			NOption |= 2;
		}

		else if (iRandomValue < 50)
		{
			NOption |= 1;
		}

		ItemSerialCreateSend(lpObj->m_Index, 255, 0, 0, ITEMGET(12, WingType), 0, 0, 0, Option2, Option3, -1, NOption, 0, 0, 0, 0);
		gObjInventoryCommit(lpObj->m_Index);

		g_Log.Add("[MonsterWing Mix] [%s][%s] CBMix Success %d Money : %d-%d, CharmRate : %d", lpObj->AccountID, lpObj->Name,
			lpObj->ChaosSuccessRate, lpObj->m_PlayerData->Money, nChaosNeedMoney, LuckyHarmAddRate);
	}

	else
	{
		for (int i = 0; i < CHAOS_BOX_SIZE; i++)
		{
			lpObj->pChaosBox[i].Clear();
		}

		GSProtocol.GCUserChaosBoxSend(lpObj, 0);
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

		g_Log.Add("[MonsterWing Mix] [%s][%s] CBMix Fail %d Money : %d-%d, CharmRate : %d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->m_PlayerData->Money, nChaosNeedMoney, LuckyHarmAddRate);
	}

	lpObj->ChaosLock = FALSE;
}

void CMixSystem::SummonsMix(LPOBJ lpObj)
{
	lpObj->ChaosLock = true;

	int Summons1Count = 0;
	int Summons2Count = 0;
	int Summons3Count = 0;
	int nChaosNeedMoney = 0;
	int MixGrade = 0;

	for (int n = 0; n < CHAOS_BOX_SIZE; n++)
	{
		if (lpObj->pChaosBox[n].IsItem() == TRUE)
		{
			switch (lpObj->pChaosBox[n].m_Type)
			{
				case ITEMGET(14,210):
					Summons1Count++;
					break;
				case ITEMGET(14,211):
					Summons2Count++;
					break;
				case ITEMGET(14,212):
					Summons3Count++;
					break;
			}
		}
	}

	PMSG_CHAOSMIXRESULT pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x86, sizeof(pMsg));

	pMsg.Result = CB_ERROR;

	if (lpObj->Level < 10)
	{
		pMsg.Result = CB_LOW_LEVEL_USER;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, sizeof(pMsg));

		lpObj->ChaosLock = FALSE;
		return;
	}

	if (Summons1Count == 2)
	{
		MixGrade = 1;
		nChaosNeedMoney = 100000;
		lpObj->ChaosSuccessRate = 50;
	}

	else if(Summons1Count == 1 && Summons2Count == 1)
	{
		MixGrade = 2;
		nChaosNeedMoney = 200000;
		lpObj->ChaosSuccessRate = 50;
	}

	else if (Summons1Count == 1 && Summons3Count == 1)
	{
		MixGrade = 3;
		nChaosNeedMoney = 300000;
		lpObj->ChaosSuccessRate = 40;
	}

	else
	{
		return;
	}

	int iChaosTaxMoney = nChaosNeedMoney * g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / 100;
	
	if (iChaosTaxMoney < 0)
		iChaosTaxMoney = 0;

	nChaosNeedMoney += iChaosTaxMoney;

	if (nChaosNeedMoney < 0)
		nChaosNeedMoney = 0;

	if (lpObj->m_PlayerData->Money < nChaosNeedMoney)
	{
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, sizeof(pMsg));

		lpObj->ChaosLock = FALSE;
		return;
	}

	this->LogChaosItem(lpObj, "SummonsMix");
	g_Log.Add("[SummonsMix] Chaos Mix Start");

	lpObj->m_PlayerData->Money -= nChaosNeedMoney;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GSProtocol.GCMoneySend(lpObj->m_Index, lpObj->m_PlayerData->Money);

	if (rand() % 100 < lpObj->ChaosSuccessRate)
	{
		int itemtype;

		if (MixGrade == 1)
		{
			itemtype = ITEMGET(14, 211);
		}

		else if (MixGrade == 2)
		{
			itemtype = ITEMGET(14, 212);
		}

		else if (MixGrade == 3)
		{
			itemtype = ITEMGET(14, 213);
		}

		ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, itemtype, 0, -1, 0, 0, 0, -1, 0, 0, 0, 0, 0);
		gObjInventoryCommit(lpObj->m_Index);

		g_Log.Add("[SummonsMix][%s][%s] CBMix Success %d Money : %d - %d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->m_PlayerData->Money, nChaosNeedMoney);
	}

	else
	{
		this->ChaosBoxInit(lpObj);
		GSProtocol.GCUserChaosBoxSend(lpObj, 0);

		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

		g_Log.Add("[SummonsMix] [%s][%s] CBMix Fail %d Money : %d-%d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate, lpObj->m_PlayerData->Money, nChaosNeedMoney);
	}

	lpObj->ChaosLock = FALSE;
}

void CMixSystem::CCFRewardMix(LPOBJ lpObj)
{
	lpObj->ChaosLock = TRUE;

	int BadgeItemCount = 0;
	int ChaosItemCount = 0;
	int JewelOfSoul10BundleCount = 0;
	int JewelOfBless10BundleCount = 0;
	int JewelOfLife10BundleCount = 0;
	int InvalidItemCount = 0;

	for (int i = 0; i < CHAOS_BOX_SIZE; i++)
	{
		if (lpObj->pChaosBox[i].IsItem() == TRUE)
		{
			if (lpObj->pChaosBox[i].m_Type == ITEMGET(12,15))
			{
				ChaosItemCount++;
			}

			else if (lpObj->pChaosBox[i].m_Type == ITEMGET(12,136))
			{
				JewelOfLife10BundleCount++;
			}

			else if (lpObj->pChaosBox[i].m_Type == ITEMGET(12,30))
			{
				JewelOfBless10BundleCount++;
			}

			else if (lpObj->pChaosBox[i].m_Type == ITEMGET(12,31))
			{
				JewelOfSoul10BundleCount++;
			}

			else if (lpObj->pChaosBox[i].m_Type == ITEMGET(13,276))
			{
				BadgeItemCount++;
			}

			else
			{
				InvalidItemCount++;
			}
		}
	}

	PMSG_CHAOSMIXRESULT pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x86, sizeof(pMsg));
	pMsg.Result = CB_ERROR;

	if (ChaosItemCount != 1 || JewelOfLife10BundleCount != 1 || JewelOfBless10BundleCount != 1 || JewelOfSoul10BundleCount != 1 || BadgeItemCount != 1 || InvalidItemCount != 0)
	{
		lpObj->ChaosLock = FALSE;
		pMsg.Result = CB_INCORRECT_MIX_ITEMS;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	int nChaosNeedMoney = 10000000;
	int iChaosTaxMoney = nChaosNeedMoney * g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / 100;
	
	if (iChaosTaxMoney < 0)
		iChaosTaxMoney = 0;

	nChaosNeedMoney += iChaosTaxMoney;
	lpObj->ChaosSuccessRate = 100;

	if (lpObj->m_PlayerData->Money < nChaosNeedMoney)
	{
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, sizeof(pMsg));

		lpObj->ChaosLock = FALSE;
		return;
	}

	this->LogChaosItem(lpObj, "Chaos Castle Survival Reward Mix");
	g_Log.Add("[Chaos Castle Survival Reward Mix] Chaos Mix Start");

	lpObj->m_PlayerData->Money -= nChaosNeedMoney;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GSProtocol.GCMoneySend(lpObj->m_Index, lpObj->m_PlayerData->Money);

	int iWingNum = ITEMGET(12,268);

	int iOption1 = 0;
	int iOption2 = 0;
	int iExcOpt = 0;
	int iOptTypeRand = rand() % 2;
	int iOptRateRand;

	switch (iOptTypeRand)
	{
	case 0:
		iOptRateRand = rand() % 1000;
		if (iOptRateRand < 400)
		{
			iExcOpt |= 0x10;
		}
		break;

	case 1:
		iOptRateRand = rand() % 1000;
		if (iOptRateRand < 300)
		{
			iExcOpt |= 0x20;
		}
		break;
	}

	int iOpt3TypeLevel = rand() % 4;
	int iOpt3TypeLevelRate = rand() % 1000;

	switch (iOpt3TypeLevel)
	{
	case 1:
	{
		if (iOpt3TypeLevelRate < 120)
		{
			iOption2 = 1;
		}
	}
		break;

	case 2:
	{
		if (iOpt3TypeLevelRate < 60)
		{
			iOption2 = 2;
		}
	}
		break;

	case 3:
	{
		if (iOpt3TypeLevelRate < 30)
		{
			iOption2 = 3;
		}
	}
		break;
	}

	int iRandOpt = rand() % 4;
	int iRandOptRate = rand() % 1000;

	switch (iRandOpt)
	{
	case 0:
	{
		if (iRandOptRate < 40)
		{
			iExcOpt |= 1;
		}
	}
		break;
	case 1:
	{
		if (iRandOptRate < 20)
		{
			iExcOpt |= 2;
		}
	}
		break;

	case 2:
	{
		if (iRandOptRate < 70)
		{
			iExcOpt |= 4;
		}
	}
		break;
	case 3:
	{
		if (iRandOptRate < 70)
		{
			iExcOpt |= 8;
		}
	}
		break;
	}

	::ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, iWingNum, 0, 0, 0, iOption1, iOption2, -1, iExcOpt, 0, 0, 0, 0);
	::gObjInventoryCommit(lpObj->m_Index);
	::g_Log.Add("[Chaos Castle Survival Reward Mix] [%s][%s] CBMix Success Money : %d-%d",
		lpObj->AccountID, lpObj->Name, lpObj->m_PlayerData->Money, nChaosNeedMoney);

	lpObj->ChaosLock = FALSE;
}

void CMixSystem::DSFRewardMix(LPOBJ lpObj)
{
	lpObj->ChaosLock = TRUE;

	int BadgeItemCount = 0;
	int ChaosItemCount = 0;
	int JewelOfSoul10BundleCount = 0;
	int JewelOfBless10BundleCount = 0;
	int JewelOfLife10BundleCount = 0;
	int InvalidItemCount = 0;

	for (int i = 0; i < CHAOS_BOX_SIZE; i++)
	{
		if (lpObj->pChaosBox[i].IsItem() == TRUE)
		{
			if (lpObj->pChaosBox[i].m_Type == ITEMGET(12,15))
			{
				ChaosItemCount++;
			}

			else if (lpObj->pChaosBox[i].m_Type == ITEMGET(12,136))
			{
				JewelOfLife10BundleCount++;
			}

			else if (lpObj->pChaosBox[i].m_Type == ITEMGET(12,30))
			{
				JewelOfBless10BundleCount++;
			}

			else if (lpObj->pChaosBox[i].m_Type == ITEMGET(12,31))
			{
				JewelOfSoul10BundleCount++;
			}

			else if (lpObj->pChaosBox[i].m_Type == ITEMGET(13,277))
			{
				BadgeItemCount++;
			}

			else
			{
				InvalidItemCount++;
			}
		}
	}

	PMSG_CHAOSMIXRESULT pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x86, sizeof(pMsg));
	pMsg.Result = CB_ERROR;

	if (ChaosItemCount != 1 || JewelOfLife10BundleCount != 1 || JewelOfBless10BundleCount != 1 || JewelOfSoul10BundleCount != 1 || BadgeItemCount != 1 || InvalidItemCount != 0)
	{
		lpObj->ChaosLock = FALSE;
		pMsg.Result = CB_INCORRECT_MIX_ITEMS;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	int nChaosNeedMoney = 10000000;
	int iChaosTaxMoney = nChaosNeedMoney * g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / 100;
	
	if (iChaosTaxMoney < 0)
		iChaosTaxMoney = 0;

	nChaosNeedMoney += iChaosTaxMoney;
	lpObj->ChaosSuccessRate = 100;

	if (lpObj->m_PlayerData->Money < nChaosNeedMoney)
	{
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, sizeof(pMsg));

		lpObj->ChaosLock = FALSE;
		return;
	}

	this->LogChaosItem(lpObj, "DSF Reward Mix");
	g_Log.Add("[DSF Reward Mix] Chaos Mix Start");

	lpObj->m_PlayerData->Money -= nChaosNeedMoney;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GSProtocol.GCMoneySend(lpObj->m_Index, lpObj->m_PlayerData->Money);

	int iItemNum = ITEMGET(13,268);

	::ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, iItemNum, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0);
	::gObjInventoryCommit(lpObj->m_Index);
	::g_Log.Add("[DSF Reward Mix] [%s][%s] CBMix Success Money : %d-%d",
		lpObj->AccountID, lpObj->Name, lpObj->m_PlayerData->Money, nChaosNeedMoney);

	lpObj->ChaosLock = FALSE;
}

void CMixSystem::CheckEmptySpace_MultiMix(PMSG_REQ_CHAOS_MULTIMIX_CHECK * aRecv, int aIndex)
{
	if (!gObjIsConnectedGP(aIndex))
	{
		return;
	}

	CItem MixItem;

	switch (aRecv->btMixType)
	{
		case CHAOS_TYPE_FRUIT:
			MixItem.m_Type = this->FRUIT_ITEMINDEX;
			break;
		case CHAOS_TYPE_DINORANT:
			MixItem.m_Type = this->DINORANT_ITEMINDEX;
			break;
		case CHAOS_TYPE_BLOODCATLE:
			MixItem.m_Type = this->BLOODCASTLE_INVITATION_ITEMINDEX;
			break;
		case CHAOS_TYPE_DEVILSQUARE:
			MixItem.m_Type = this->DEVILSQUARE_INVITATION_ITEMINDEX;
			break;
		case CHAOS_TYPE_COMPOUNDPOTION_LV1:
			MixItem.m_Type = this->SMALL_SD_POTION_ITEMINDEX;
			break;
		case CHAOS_TYPE_COMPOUNTPOTION_LV2:
			MixItem.m_Type = this->MEDIUM_SD_POTION_ITEMINDEX;
			break;
		case CHAOS_TYPE_COMPOUNTPOTION_LV3:
			MixItem.m_Type = this->LARGE_SD_POTION_ITEMINDEX;
			break;
		case CHAOS_TYPE_BLESS_POTION:
			MixItem.m_Type = this->BLESS_POTION_ITEMINDEX;
			break;
		case CHAOS_TYPE_SOUL_POTION:
			MixItem.m_Type = this->SOUL_POTION_ITEMINDEX;
			break;
		case CHAOS_TYPE_PREMIUM_BOX:
			MixItem.m_Type = this->GOLDEN_BOX_ITEMINDEX;
			break;
		case CHAOS_TYPE_JEWELOFHARMONY_MIX_SMELTINGITEM:
			MixItem.m_Type = this->HIGH_REFINE_STONE_ITEMINDEX;
			break;
		case CHAOS_TYPE_JEWELOFHARMONY_PURITY:
			MixItem.m_Type = this->HARMONY_JEWEL_ITEMINDEX;
			break;
	}

	bool bResult = gObjChaosBoxPutItemTest(aIndex, MixItem, aRecv->btMixCount);

	PMSG_ANS_CHAOS_MULTIMIX_CHECK pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x88, sizeof(pMsg));

	pMsg.btResult = bResult;
	IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void CMixSystem::BlessPotionChaosMix_Multi(LPOBJ lpObj, int iMixCount)
{
	lpObj->ChaosLock = TRUE;
	int iBlessGemCount = 0;
	DWORD iChaosMoney = 0;
	int iInvalidItemCount = 0;

	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;

	for (int n = 0; n<CHAOS_BOX_SIZE; n++)
	{
		if (lpObj->pChaosBox[n].IsItem() == TRUE)
		{
			if (lpObj->pChaosBox[n].m_Type == ITEMGET(14, 13))
			{
				iBlessGemCount++;
			}
			else
			{
				iInvalidItemCount++;
			}
		}
	}

	if (iBlessGemCount == 0)
		return;

	if (iBlessGemCount != iMixCount)
		return;

	if (iInvalidItemCount > 0)
		return;

	if (iBlessGemCount > 25)
	{
		MsgOutput(lpObj->m_Index, Lang.GetText(0, 182));
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	LogChaosItem(lpObj, "BlessPotionMix");
	g_Log.Add("[BlessPotionMix] Chaos Mix Start");
	lpObj->ChaosSuccessRate = 100;

	int nChaosNeedMoney = 100000 * iMixCount;

	int iChaosTaxMoney = (int)((__int64)nChaosNeedMoney * (__int64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (__int64)100);

	if (iChaosTaxMoney < 0)
	{
		iChaosTaxMoney = 0;
	}

	nChaosNeedMoney += iChaosTaxMoney;

	if (nChaosNeedMoney < 0)
	{
		nChaosNeedMoney = 0;
	}

	if (lpObj->m_PlayerData->Money < nChaosNeedMoney)
	{
		pMsg.Result = CB_NOT_ENOUGH_ZEN;

		IOCP.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	lpObj->m_PlayerData->Money -= nChaosNeedMoney;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GSProtocol.GCMoneySend(lpObj->m_Index, lpObj->m_PlayerData->Money);
	
	pMsg.Result = CB_MULTIMIX_RESULT;
	pMsg.Pos = iMixCount;
	memset(pMsg.ItemInfo, 0x00, sizeof(pMsg.ItemInfo));

	lpObj->ChaosMassMixCurrItem = 0;
	lpObj->ChaosMassMixSuccessCount = 0;

	for (int i = 0; i < iMixCount; i++)
	{
		if ((rand() % 100) < lpObj->ChaosSuccessRate)
		{
			BYTE btMixNumber = 1 << (i % 8);
			pMsg.ItemInfo[i / 8] |= btMixNumber;

			lpObj->ChaosMassMixSuccessCount++;
			g_Log.Add("[BlessPotionMix] [%s][%s] Mass Combination ID:%d Success %d Money : %d-%d",
				lpObj->AccountID, lpObj->Name, i, lpObj->ChaosSuccessRate, lpObj->m_PlayerData->Money,
				nChaosNeedMoney);
		}

		else
		{
			g_Log.Add("[BlessPotionMix] [%s][%s] Mass Combination ID:%d Fail %d Money : %d-%d",
				lpObj->AccountID, lpObj->Name, i, lpObj->ChaosSuccessRate, lpObj->m_PlayerData->Money,
				nChaosNeedMoney);
		}
	}

	IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

	if (lpObj->ChaosMassMixSuccessCount == 0)
	{
		ChaosBoxInit(lpObj);
		::GSProtocol.GCUserChaosBoxSend(lpObj, 1);

		pMsg.Result = CB_ERROR;
		IOCP.DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);

		g_Log.Add("[BlessPotionMix] [%s][%s] CBMix Fail %d Money : %d-%d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate,
			lpObj->m_PlayerData->Money, nChaosNeedMoney);

		lpObj->ChaosLock = FALSE;
	}

	else
	{
		for (int i = 0; i < lpObj->ChaosMassMixSuccessCount; i++)
		{
			int ItemNum = ITEMGET(14, 7);
			int Level = 0;
			int Dur = 10;

			ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, ItemNum, Level, Dur, 0, 0, 0, -1, 0, 0, 0, 0, 0);
		}

		gObjInventoryCommit(lpObj->m_Index);
	}
}

void CMixSystem::SoulPotionChaosMix_Multi(LPOBJ lpObj, int iMixCount)
{
	lpObj->ChaosLock = TRUE;
	int iSoulGemCount = 0;
	DWORD iChaosMoney = 0;
	int iInvalidItemCount = 0;

	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;

	for (int n = 0; n<CHAOS_BOX_SIZE; n++)
	{
		if (lpObj->pChaosBox[n].IsItem() == TRUE)
		{
			if (lpObj->pChaosBox[n].m_Type == ITEMGET(14, 14))
			{
				iSoulGemCount++;
			}
			else
			{
				iInvalidItemCount++;
			}
		}
	}

	if (iSoulGemCount == 0)
		return;

	if (iSoulGemCount != iMixCount)
		return;

	if (iInvalidItemCount > 0)
		return;

	if (iSoulGemCount > 25)
	{
		MsgOutput(lpObj->m_Index, Lang.GetText(0, 182));
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	LogChaosItem(lpObj, "SoulPotionMix");
	g_Log.Add("[SoulPotionMix] Chaos Mix Start");
	lpObj->ChaosSuccessRate = 100;

	int nChaosNeedMoney = 50000 * iMixCount;

	int iChaosTaxMoney = (int)((__int64)nChaosNeedMoney * (__int64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (__int64)100);

	if (iChaosTaxMoney < 0)
	{
		iChaosTaxMoney = 0;
	}

	nChaosNeedMoney += iChaosTaxMoney;

	if (nChaosNeedMoney < 0)
	{
		nChaosNeedMoney = 0;
	}

	if (lpObj->m_PlayerData->Money < nChaosNeedMoney)
	{
		pMsg.Result = CB_NOT_ENOUGH_ZEN;

		IOCP.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	lpObj->m_PlayerData->Money -= nChaosNeedMoney;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GSProtocol.GCMoneySend(lpObj->m_Index, lpObj->m_PlayerData->Money);

	pMsg.Result = CB_MULTIMIX_RESULT;
	pMsg.Pos = iMixCount;
	memset(pMsg.ItemInfo, 0x00, sizeof(pMsg.ItemInfo));

	lpObj->ChaosMassMixCurrItem = 0;
	lpObj->ChaosMassMixSuccessCount = 0;

	for (int i = 0; i < iMixCount; i++)
	{
		if ((rand() % 100) < lpObj->ChaosSuccessRate)
		{
			BYTE btMixNumber = 1 << (i % 8);
			pMsg.ItemInfo[i / 8] |= btMixNumber;

			lpObj->ChaosMassMixSuccessCount++;
			g_Log.Add("[SoulPotionMix] [%s][%s] Mass Combination ID:%d Success %d Money : %d-%d",
				lpObj->AccountID, lpObj->Name, i, lpObj->ChaosSuccessRate, lpObj->m_PlayerData->Money,
				nChaosNeedMoney);
		}

		else
		{
			g_Log.Add("[SoulPotionMix] [%s][%s] Mass Combination ID:%d Fail %d Money : %d-%d",
				lpObj->AccountID, lpObj->Name, i, lpObj->ChaosSuccessRate, lpObj->m_PlayerData->Money,
				nChaosNeedMoney);
		}
	}

	IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

	if (lpObj->ChaosMassMixSuccessCount == 0)
	{
		ChaosBoxInit(lpObj);
		::GSProtocol.GCUserChaosBoxSend(lpObj, 1);

		pMsg.Result = CB_ERROR;
		IOCP.DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);

		g_Log.Add("[SoulPotionMix] [%s][%s] CBMix Fail %d Money : %d-%d",
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate,
			lpObj->m_PlayerData->Money, nChaosNeedMoney);

		lpObj->ChaosLock = FALSE;
	}

	else
	{
		for (int i = 0; i < lpObj->ChaosMassMixSuccessCount; i++)
		{
			int ItemNum = ITEMGET(14, 7);
			int Level = 1;
			int Dur = 10;

			ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, ItemNum, Level, Dur, 0, 0, 0, -1, 0, 0, 0, 0, 0);
		}

		gObjInventoryCommit(lpObj->m_Index);
	}
}

void CMixSystem::CircleChaosMix_Multi(LPOBJ lpObj, int iMixCount)	// Fruits Mass Mix
{
	lpObj->ChaosLock = TRUE;

	int CreatureGemCount = 0;
	int ChoasGemCount = 0;

	for (int n = 0; n<CHAOS_BOX_SIZE; n++)
	{
		if (lpObj->pChaosBox[n].IsItem() == TRUE)
		{
			if (lpObj->pChaosBox[n].m_Type == ITEMGET(14, 22))	// Jewel of Creation
			{
				CreatureGemCount++;
			}
			else if (lpObj->pChaosBox[n].m_Type == ITEMGET(12, 15)) // Chaos Gem
			{
				ChoasGemCount++;
			}
		}
	}

	if (CreatureGemCount != iMixCount || ChoasGemCount != iMixCount)
	{
		return;
	}

	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;

	if (lpObj->Level < 10)
	{
		pMsg.Result = CB_LOW_LEVEL_USER;

		IOCP.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	int nChaosNeedMoney = 3000000 * iMixCount;
	int iChaosTaxMoney = (int)((__int64)nChaosNeedMoney * (__int64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (__int64)100);

	if (iChaosTaxMoney < 0)
	{
		iChaosTaxMoney = 0;
	}

	nChaosNeedMoney += iChaosTaxMoney;

	if (nChaosNeedMoney < 0)
	{
		nChaosNeedMoney = 0;
	}

	if (lpObj->m_PlayerData->Money < nChaosNeedMoney)
	{
		lpObj->ChaosLock = FALSE;
		pMsg.Result = CB_NOT_ENOUGH_ZEN;

		IOCP.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	LogChaosItem(lpObj, "CircleMix");
	g_Log.Add("[CircleMix] Chaos Mix Start");
	lpObj->ChaosSuccessRate = 90;	// Succes Rate for Fruit
	lpObj->m_PlayerData->Money -= nChaosNeedMoney;

	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GSProtocol.GCMoneySend(lpObj->m_Index, lpObj->m_PlayerData->Money);

	pMsg.Result = CB_MULTIMIX_RESULT;
	pMsg.Pos = iMixCount;

	memset(pMsg.ItemInfo, 0x00, sizeof(pMsg.ItemInfo));

	lpObj->ChaosMassMixCurrItem = 0;
	lpObj->ChaosMassMixSuccessCount = 0;

	for (int i = 0; i < iMixCount; i++)
	{
		if ((rand() % 100) < lpObj->ChaosSuccessRate)
		{
			BYTE btMixNumber = 1 << (i % 8);
			pMsg.ItemInfo[i / 8] |= btMixNumber;

			lpObj->ChaosMassMixSuccessCount++;
			g_Log.Add("[CircleMix] [%s][%s] Mass Combination ID:%d Success %d Money : %d-%d",
				lpObj->AccountID, lpObj->Name, i, lpObj->ChaosSuccessRate, lpObj->m_PlayerData->Money,
				nChaosNeedMoney);
		}

		else
		{
			g_Log.Add("[CircleMix] [%s][%s] Mass Combination ID:%d Fail %d Money : %d-%d",
				lpObj->AccountID, lpObj->Name, i, lpObj->ChaosSuccessRate, lpObj->m_PlayerData->Money,
				nChaosNeedMoney);
		}
	}

	IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

	if (lpObj->ChaosMassMixSuccessCount == 0)
	{
		ChaosBoxInit(lpObj);
		GSProtocol.GCUserChaosBoxSend(lpObj, 0);

		lpObj->ChaosLock = FALSE;
		return;
	}

	else
	{
		for (int i = 0; i < lpObj->ChaosMassMixSuccessCount; i++)
		{
			int Fruit = ITEMGET(13, 15);	// Fruit
			int FruitType;
			int RandonValue = rand() % 100;

			if (RandonValue < 30)
			{
				FruitType = 0;
			}
			else if (RandonValue < 55)
			{
				FruitType = 1;
			}
			else if (RandonValue < 75)
			{
				FruitType = 2;
			}
			else if (RandonValue < 95)
			{
				FruitType = 3;
			}
			else
			{
				FruitType = 4;
			}

			ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, Fruit, FruitType, 255, 1, 0, 0, -1, 0, 0, 0, 0, 0);
		}

		gObjInventoryCommit(lpObj->m_Index);
	}
}

void CMixSystem::PegasiaChaosMix_Multi(LPOBJ lpObj, int iMixCount)
{
	lpObj->ChaosLock = TRUE;
	int UniriaCount = 0;
	int ChoasGemCount = 0;

	for (int n = 0; n<CHAOS_BOX_SIZE; n++)
	{
		if (lpObj->pChaosBox[n].IsItem() == TRUE)
		{
			if (lpObj->pChaosBox[n].m_Type == ITEMGET(13, 2) && lpObj->pChaosBox[n].m_Durability == 255.0f)	// Uniria
			{
				UniriaCount++;
			}
			else if (lpObj->pChaosBox[n].m_Type == ITEMGET(12, 15)) // Chaos Gem
			{
				ChoasGemCount++;
			}
		}
	}

	if (UniriaCount != (10 * iMixCount) || ChoasGemCount != iMixCount)
	{
		lpObj->ChaosLock = FALSE;
		return;
	}

	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;

	int nChaosNeedMoney = 500000 * iMixCount;
	int iChaosTaxMoney = (int)((__int64)nChaosNeedMoney * (__int64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (__int64)100);

	if (iChaosTaxMoney < 0)
	{
		iChaosTaxMoney = 0;
	}

	nChaosNeedMoney += iChaosTaxMoney;

	if (nChaosNeedMoney < 0)
	{
		nChaosNeedMoney = 0;
	}

	if (lpObj->m_PlayerData->Money < nChaosNeedMoney)
	{
		lpObj->ChaosLock = FALSE;
		pMsg.Result = CB_NOT_ENOUGH_ZEN;

		IOCP.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	LogChaosItem(lpObj, "DinorantMix");
	g_Log.Add("[DinorantMix] Chaos Mix Start");
	lpObj->ChaosSuccessRate = 70;	// Succes Rate for Dinorant
	lpObj->m_PlayerData->Money -= nChaosNeedMoney;

	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GSProtocol.GCMoneySend(lpObj->m_Index, lpObj->m_PlayerData->Money);

	pMsg.Result = CB_MULTIMIX_RESULT;
	pMsg.Pos = iMixCount;

	memset(pMsg.ItemInfo, 0x00, sizeof(pMsg.ItemInfo));

	lpObj->ChaosMassMixCurrItem = 0;
	lpObj->ChaosMassMixSuccessCount = 0;

	for (int i = 0; i < iMixCount; i++)
	{
		if ((rand() % 100) < lpObj->ChaosSuccessRate)
		{
			BYTE btMixNumber = 1 << (i % 8);
			pMsg.ItemInfo[i / 8] |= btMixNumber;

			lpObj->ChaosMassMixSuccessCount++;
			g_Log.Add("[DinorantMix] [%s][%s] Mass Combination ID:%d Success %d Money : %d-%d",
				lpObj->AccountID, lpObj->Name, i, lpObj->ChaosSuccessRate, lpObj->m_PlayerData->Money,
				nChaosNeedMoney);
		}

		else
		{
			g_Log.Add("[DinorantMix] [%s][%s] Mass Combination ID:%d Fail %d Money : %d-%d",
				lpObj->AccountID, lpObj->Name, i, lpObj->ChaosSuccessRate, lpObj->m_PlayerData->Money,
				nChaosNeedMoney);
		}
	}

	IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

	if (lpObj->ChaosMassMixSuccessCount == 0)
	{
		ChaosBoxInit(lpObj);
		GSProtocol.GCUserChaosBoxSend(lpObj, 0);

		lpObj->ChaosLock = FALSE;
		return;
	}

	else
	{
		for (int i = 0; i < lpObj->ChaosMassMixSuccessCount; i++)
		{
			int Dinorant = ITEMGET(13, 3);
			int Option3 = 0;

			if ((rand() % 100) < 30)
			{
				Option3 = 1 << ((rand() % 3));

				if ((rand() % 5) == 0)
				{
					Option3 |= 1 << (rand() % 3);
				}
			}

			ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, Dinorant, 0, 255, 1, 0, Option3, -1, 0, 0, 0, 0, 0);
		}

		gObjInventoryCommit(lpObj->m_Index);
	}
}

void CMixSystem::BloodCastleItemChaosMix_Multi(LPOBJ lpObj, int iMixCount)
{
	lpObj->ChaosLock = TRUE;

	PMSG_CHAOSMIXRESULT pMsg;
	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;

	int iIndex = lpObj->m_Index;

	int iCHAOS_MIX_LEVEL = 0;
	BOOL bIsChaosGemExist = FALSE;
	BOOL bIsAngelKingPaperExist = FALSE;
	BOOL bIsBloodBoneExist = FALSE;
	BOOL bIsOtherItemExist = FALSE;
	int iEventItemCount = 0;
	int iAngelKingPaperLevel = 0;
	int iBloodBoneLevel = 0;

	for (int i = 0; i<CHAOS_BOX_SIZE; i++)
	{
		if (gObj[iIndex].pChaosBox[i].IsItem() == TRUE)
		{
			if (gObj[iIndex].pChaosBox[i].m_Type == ITEMGET(12, 15)) // Chaos
			{
				bIsChaosGemExist++;
			}
			else if (gObj[iIndex].pChaosBox[i].m_Type == ITEMGET(13, 16)) //Scroll of Archangel
			{
				int iSCROLL_LEVEL = gObj[iIndex].pChaosBox[i].m_Level;
				iEventItemCount++;
				bIsAngelKingPaperExist++;
				iAngelKingPaperLevel = iSCROLL_LEVEL;
			}
			else if (gObj[iIndex].pChaosBox[i].m_Type == ITEMGET(13, 17)) //Blood Bone
			{
				int iBLOOD_BONE_LEVEL = gObj[iIndex].pChaosBox[i].m_Level;
				iEventItemCount++;
				bIsBloodBoneExist++;
				iBloodBoneLevel = iBLOOD_BONE_LEVEL;
			}
			else
			{
				bIsOtherItemExist = TRUE;
			}
		}
	}

	if (bIsOtherItemExist != FALSE)
	{
		lpObj->ChaosLock = FALSE;
		return;
	}

	if (bIsAngelKingPaperExist == FALSE && bIsBloodBoneExist == FALSE)
	{
		lpObj->ChaosLock = FALSE;
		return;
	}

	if (bIsAngelKingPaperExist == FALSE || bIsBloodBoneExist == FALSE)
	{
		pMsg.Result = CB_NO_BC_CORRECT_ITEMS;
		IOCP.DataSend(iIndex, (UCHAR*)&pMsg, pMsg.h.size);

		lpObj->ChaosLock = FALSE;
		return;
	}

	if (iEventItemCount > (2 * iMixCount))
	{
		pMsg.Result = CB_INVALID_ITEM_LEVEL;
		IOCP.DataSend(iIndex, (UCHAR*)&pMsg, pMsg.h.size);

		lpObj->ChaosLock = FALSE;
		return;
	}

	if (iAngelKingPaperLevel != iBloodBoneLevel)
	{
		pMsg.Result = CB_NO_BC_CORRECT_ITEMS;
		IOCP.DataSend(iIndex, (UCHAR*)&pMsg, pMsg.h.size);

		lpObj->ChaosLock = FALSE;
		return;
	}

	if (BC_BRIDGE_RANGE(iAngelKingPaperLevel - 1) == FALSE)
	{
		pMsg.Result = CB_NO_BC_CORRECT_ITEMS;
		IOCP.DataSend(iIndex, (UCHAR*)&pMsg, pMsg.h.size);

		lpObj->ChaosLock = FALSE;
		return;
	}

	if (BC_BRIDGE_RANGE(iBloodBoneLevel - 1) == FALSE)
	{
		pMsg.Result = CB_NO_BC_CORRECT_ITEMS;
		IOCP.DataSend(iIndex, (UCHAR*)&pMsg, pMsg.h.size);

		lpObj->ChaosLock = FALSE;
		return;
	}

	if (bIsChaosGemExist == FALSE)
	{
		pMsg.Result = CB_NO_BC_CORRECT_ITEMS;
		IOCP.DataSend(iIndex, (UCHAR*)&pMsg, pMsg.h.size);

		lpObj->ChaosLock = FALSE;
		return;
	}

	if (gObj[iIndex].Class == CLASS_DARKLORD || gObj[iIndex].Class == CLASS_MAGUMSA || gObj[iIndex].Class == CLASS_RAGEFIGHTER)
	{
		if (gObj[iIndex].Level < g_sttBLOODCASTLE_LEVEL[0].iLOWER_BOUND_MAGUMSA)
		{
			pMsg.Result = CB_USER_CLASS_LOW_LEVEL;
			IOCP.DataSend(iIndex, (UCHAR*)&pMsg, pMsg.h.size);

			lpObj->ChaosLock = FALSE;
			return;
		}
	}
	else
	{
		if (gObj[iIndex].Level < g_sttBLOODCASTLE_LEVEL[0].iLOWER_BOUND)
		{
			pMsg.Result = CB_USER_CLASS_LOW_LEVEL;
			IOCP.DataSend(iIndex, (UCHAR*)&pMsg, pMsg.h.size);

			lpObj->ChaosLock = FALSE;
			return;
		}
	}

	if (bIsChaosGemExist == iMixCount && bIsAngelKingPaperExist == iMixCount && bIsBloodBoneExist == iMixCount)
	{
		int cMapNum = g_BloodCastle.GetBridgeMapNumber(iAngelKingPaperLevel - 1);

		if (BC_MAP_RANGE(cMapNum) == FALSE)
		{
			return;
		}

		g_Log.Add("[Blood Castle] BloodCastle Ticket Mass Mix Chaos Mix Start (Account:%s, Name:%s, Level:%d) (MixCount:%d)", gObj[iIndex].AccountID, gObj[iIndex].Name, iAngelKingPaperLevel, iMixCount);

		char szTemp[64];
		wsprintf(szTemp, "BloodCastle Ticket Mix,%d", iAngelKingPaperLevel); //Season 2.5 add-on

		g_MixSystem.LogChaosItem(&gObj[iIndex], szTemp);

		int iMIX_SUCCESS_RATE = g_iBC_ChoasMixSuccessRate[iAngelKingPaperLevel - 1];

		if (iMIX_SUCCESS_RATE < 0 || iMIX_SUCCESS_RATE > 100)
		{
			IOCP.DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);
			g_Log.Add("[Blood Castle] BloodCastle Ticket Mix Chaos Mass Mix Failed - MixRate Out of Bound (Account:%s, Name:%s, Level:%d)", gObj[iIndex].AccountID, gObj[iIndex].Name, iAngelKingPaperLevel);
			return;
		}

		if (g_CrywolfSync.GetOccupationState() == 0 && g_CrywolfSync.GetApplyBenefit() == TRUE)
		{
			iMIX_SUCCESS_RATE += g_CrywolfSync.GetPlusChaosRate();
		}

		if (iMIX_SUCCESS_RATE > 80)
		{
			iMIX_SUCCESS_RATE = 80;
		}

		int iMIX_NEED_MONEY = g_iBC_ChoasMixMoney[iAngelKingPaperLevel - 1] * iMixCount;
		int iChaosTaxMoney = (int)((__int64)(iMIX_NEED_MONEY)* (__int64)(g_CastleSiegeSync.GetTaxRateChaos(iIndex)) / (__int64)100);

		if (iChaosTaxMoney < 0)
		{
			iChaosTaxMoney = 0;
		}

		iMIX_NEED_MONEY += iChaosTaxMoney;

		if (iMIX_NEED_MONEY <  0)
		{
			IOCP.DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);
			g_Log.Add("[Blood Castle] BloodCastle Ticket Mix Chaos Mass Mix Failed - MixMoney < 0 (Account:%s, Name:%s, Level:%d)", gObj[iIndex].AccountID, gObj[iIndex].Name, iAngelKingPaperLevel);
			return;
		}

		if ((gObj[iIndex].m_PlayerData->Money - iMIX_NEED_MONEY) < 0)
		{
			pMsg.Result = CB_BC_NOT_ENOUGH_ZEN;
			IOCP.DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);
			g_Log.Add("[Blood Castle] BloodCastle Ticket Mix Chaos Mass Mix Failed - Not Enough Money (Account:%s, Name:%s, Level:%d)", gObj[iIndex].AccountID, gObj[iIndex].Name, iAngelKingPaperLevel);
			return;
		}

		gObj[iIndex].m_PlayerData->Money -= iMIX_NEED_MONEY;
		g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
		GSProtocol.GCMoneySend(iIndex, gObj[iIndex].m_PlayerData->Money);

		pMsg.Result = CB_MULTIMIX_RESULT;
		pMsg.Pos = iMixCount;

		memset(pMsg.ItemInfo, 0x00, sizeof(pMsg.ItemInfo));

		lpObj->ChaosMassMixCurrItem = 0;
		lpObj->ChaosMassMixSuccessCount = 0;

		for (int i = 0; i < iMixCount; i++)
		{
			if ((rand() % 100) < iMIX_SUCCESS_RATE)
			{
				BYTE btMixNumber = 1 << (i % 8);
				pMsg.ItemInfo[i / 8] |= btMixNumber;

				lpObj->ChaosMassMixSuccessCount++;
				g_Log.Add("[BloodCastle Ticket Mix] [%s][%s] Mass Combination ID:%d Success %d Money : %d-%d",
					lpObj->AccountID, lpObj->Name, i, iMIX_SUCCESS_RATE, lpObj->m_PlayerData->Money,
					iMIX_NEED_MONEY);
			}

			else
			{
				g_Log.Add("[BloodCastle Ticket Mix] [%s][%s] Mass Combination ID:%d Fail %d Money : %d-%d",
					lpObj->AccountID, lpObj->Name, i, iMIX_SUCCESS_RATE, lpObj->m_PlayerData->Money,
					iMIX_NEED_MONEY);
			}
		}

		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

		if (lpObj->ChaosMassMixSuccessCount == 0)
		{
			ChaosBoxInit(lpObj);
			GSProtocol.GCUserChaosBoxSend(lpObj, 0);

			lpObj->ChaosLock = FALSE;
			return;
		}

		for (int i = 0; i < lpObj->ChaosMassMixSuccessCount; i++)
		{
			int item_num = ITEMGET(13, 18);
			ItemSerialCreateSend(iIndex, -1, 0, 0, item_num, iAngelKingPaperLevel, 255, 0, 0, 0, -1, 0, 0, 0, 0, 0);
		}

		gObjInventoryCommit(iIndex);

		return;
	}
}

void CMixSystem::DevilSquareItemChaosMix_Multi(LPOBJ lpObj, int iMixCount)
{
	lpObj->ChaosLock = TRUE;

	if (g_DevilSquare.IsEventEnable() == false)
	{
		lpObj->ChaosLock = FALSE;
		return;
	}

	PMSG_CHAOSMIXRESULT pMsg;
	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;
	int aIndex = lpObj->m_Index;

	BOOL bChaoseGem = FALSE;
	BOOL bEyeOfDevil = FALSE;
	BOOL bKeyOfDevil = FALSE;
	BOOL FoundOtherItem = FALSE;
	int eventitemcount = 0;
	int level = -1;
	BOOL bLevelCheck = FALSE;

	g_Log.Add("[DevilSquare] Search DevilSquareItem");

	for (int n = 0; n<CHAOS_BOX_SIZE; n++)
	{
		if (lpObj->pChaosBox[n].IsItem() == TRUE)
		{
			if (lpObj->pChaosBox[n].m_Type == ITEMGET(12, 15))	// Chaos
			{
				bChaoseGem++;
			}

			else if (lpObj->pChaosBox[n].m_Type == ITEMGET(14, 17))	// Eye
			{
				eventitemcount++;
				bEyeOfDevil++;

				if (level != lpObj->pChaosBox[n].m_Level)
				{
					if (level == -1)
					{
						level = lpObj->pChaosBox[n].m_Level;
					}
					else
					{
						bLevelCheck = TRUE;
					}
				}
			}

			else if (lpObj->pChaosBox[n].m_Type == ITEMGET(14, 18))	// Key
			{
				eventitemcount++;
				bKeyOfDevil++;

				if (level != lpObj->pChaosBox[n].m_Level)
				{
					if (level == -1)
					{
						level = lpObj->pChaosBox[n].m_Level;
					}
					else
					{
						bLevelCheck = TRUE;
					}
				}
			}

			else
			{
				FoundOtherItem = TRUE;
			}
		}
	}

	if (FoundOtherItem != FALSE)
	{
		g_Log.Add("[DevilSquare] Other DQItems Found");
		
		lpObj->ChaosLock = FALSE;
		return;
	}

	if (bLevelCheck != FALSE)
	{
		pMsg.Result = CB_INCORRECT_MIX_ITEMS;
		IOCP.DataSend(aIndex, (BYTE*)&pMsg, pMsg.h.size);
		g_Log.Add("[DevilSquare] DiffLevel Devil's Key or Eyes [%d]", eventitemcount);
		lpObj->ChaosLock = FALSE;

		return;
	}

	if (eventitemcount > (2 * iMixCount))
	{
		pMsg.Result = CB_TOO_MANY_ITEMS;
		IOCP.DataSend(aIndex, (BYTE*)&pMsg, pMsg.h.size);
		g_Log.Add("[DevilSquare] Too many Devil's Key or Eyes [%d]", eventitemcount);
		lpObj->ChaosLock = FALSE;

		return;
	}

	if (lpObj->Level < 10)
	{
		pMsg.Result = CB_LOW_LEVEL_USER;
		IOCP.DataSend(aIndex, (BYTE*)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	g_Log.Add("[DevilSquare] Chaos Mix Start");
	INT nChaosNeedMoney = 0;

	switch (level)
	{
	case 0:
		lpObj->ChaosSuccessRate = 60;
		nChaosNeedMoney = 100000;
		break;

	case 1:
		lpObj->ChaosSuccessRate = gDQChaosSuccessRateLevel1;
		nChaosNeedMoney = 100000;
		break;

	case 2:
		lpObj->ChaosSuccessRate = gDQChaosSuccessRateLevel2;
		nChaosNeedMoney = 200000;
		break;

	case 3:
		lpObj->ChaosSuccessRate = gDQChaosSuccessRateLevel3;
		nChaosNeedMoney = 400000;
		break;

	case 4:
		lpObj->ChaosSuccessRate = gDQChaosSuccessRateLevel4;
		nChaosNeedMoney = 700000;
		break;

	case 5:
		lpObj->ChaosSuccessRate = gDQChaosSuccessRateLevel5;
		nChaosNeedMoney = 1100000;
		break;

	case 6:
		lpObj->ChaosSuccessRate = gDQChaosSuccessRateLevel6;
		nChaosNeedMoney = 1600000;
		break;

	case 7:
		lpObj->ChaosSuccessRate = gDQChaosSuccessRateLevel7;
		nChaosNeedMoney = 2000000;
		break;

	default:
		g_Log.Add("[DevilSquare] [%s][%s] Invalid DQItem Level [%d]",
			lpObj->AccountID, lpObj->Name, level);

		pMsg.Result = CB_INVALID_ITEM_LEVEL;
		IOCP.DataSend(aIndex, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;
		return;
	}

	nChaosNeedMoney *= iMixCount;

	int iChaosTaxMoney = (int)((__int64)nChaosNeedMoney * (__int64)g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / (__int64)100);

	if (iChaosTaxMoney < 0)
	{
		iChaosTaxMoney = 0;
	}

	nChaosNeedMoney += iChaosTaxMoney;

	if (nChaosNeedMoney < 0)
	{
		nChaosNeedMoney = 0;
	}

	if ((lpObj->m_PlayerData->Money - nChaosNeedMoney) < 0)
	{
		g_Log.Add("[DevilSquare] [%s][%s] CBMix Not Enough Money [%d] need zen [%d]",
			lpObj->AccountID, lpObj->Name, lpObj->m_PlayerData->Money, nChaosNeedMoney);
		pMsg.Result = CB_NOT_ENOUGH_ZEN;
		IOCP.DataSend(aIndex, (BYTE *)&pMsg, pMsg.h.size);

		lpObj->ChaosLock = FALSE;
		return;

	}

	if (g_CrywolfSync.GetOccupationState() == 0 && g_CrywolfSync.GetApplyBenefit() == TRUE)
	{
		lpObj->ChaosSuccessRate += g_CrywolfSync.GetPlusChaosRate();
	}

	lpObj->m_PlayerData->Money -= nChaosNeedMoney;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GSProtocol.GCMoneySend(lpObj->m_Index, lpObj->m_PlayerData->Money);

	pMsg.Result = CB_MULTIMIX_RESULT;
	pMsg.Pos = iMixCount;

	memset(pMsg.ItemInfo, 0x00, sizeof(pMsg.ItemInfo));

	lpObj->ChaosMassMixCurrItem = 0;
	lpObj->ChaosMassMixSuccessCount = 0;

	for (int i = 0; i < iMixCount; i++)
	{
		if ((rand() % 100) < lpObj->ChaosSuccessRate)
		{
			BYTE btMixNumber = 1 << (i % 8);
			pMsg.ItemInfo[i / 8] |= btMixNumber;

			lpObj->ChaosMassMixSuccessCount++;
			g_Log.Add("[DevilSquare,%d] [%s][%s] Mass Combination ID:%d Success %d Money : %d-%d", level,
				lpObj->AccountID, lpObj->Name, i, lpObj->ChaosSuccessRate, lpObj->m_PlayerData->Money,
				nChaosNeedMoney);
		}

		else
		{
			g_Log.Add("[DevilSquare,%d] [%s][%s] Mass Combination ID:%d Fail %d Money : %d-%d", level,
				lpObj->AccountID, lpObj->Name, i, lpObj->ChaosSuccessRate, lpObj->m_PlayerData->Money,
				nChaosNeedMoney);
		}
	}

	IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

	if (lpObj->ChaosMassMixSuccessCount == 0)
	{
		ChaosBoxInit(lpObj);
		GSProtocol.GCUserChaosBoxSend(lpObj, 0);

		g_Log.Add("[DevilSquare,%d] [%s][%s] CBMix Fail %d Money : %d-%d", level,
			lpObj->AccountID, lpObj->Name, lpObj->ChaosSuccessRate,
			lpObj->m_PlayerData->Money, nChaosNeedMoney);

		lpObj->ChaosLock = FALSE;
		return;
	}

	for (int i = 0; i < lpObj->ChaosMassMixSuccessCount; i++)
	{
		int item_num = ITEMGET(14, 19);
		ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, item_num, level, 255, 0, 0, 0, -1, 0, 0, 0, 0, 0);
	}

	gObjInventoryCommit(lpObj->m_Index);
}

void CMixSystem::ShieldPotionLv1_Mix_Multi(LPOBJ lpObj, int iMixCount)
{
	int iHealthPotionCount = 0;
	int iInvalidItemCount = 0;
	int iChaosMixPrice = 0;

	if (g_ShieldSystemOn == FALSE)
		return;

	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;

	for (int n = 0; n < CHAOS_BOX_SIZE; n++)
	{
		if (lpObj->pChaosBox[n].IsItem() == TRUE)
		{
			if (lpObj->pChaosBox[n].m_Type == ITEMGET(14, 3))
			{
				iHealthPotionCount += lpObj->pChaosBox[n].m_Durability;
			}

			else
			{
				iInvalidItemCount++;
			}
		}
	}

	if (iInvalidItemCount > 0 || iHealthPotionCount != (3 * iMixCount))
	{
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	iChaosMixPrice = g_iShieldPotionLv1MixMoney * iMixCount;
	int iChaosTaxMoney = iChaosMixPrice * g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / 100;

	if (iChaosTaxMoney < 0)
	{
		iChaosTaxMoney = 0;
	}

	iChaosMixPrice += iChaosTaxMoney;

	if (iChaosMixPrice < 0)
	{
		iChaosMixPrice = 0;
	}

	if (lpObj->m_PlayerData->Money < iChaosMixPrice)
	{
		pMsg.Result = CB_NOT_ENOUGH_ZEN;

		IOCP.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	lpObj->m_PlayerData->Money -= iChaosMixPrice;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GSProtocol.GCMoneySend(lpObj->m_Index, lpObj->m_PlayerData->Money);

	LogChaosItem(lpObj, "PotionMix][ShieldPotion Lv1 Mix");
	g_Log.Add("[PotionMix][ShieldPotion Lv1 Mix] - Mix Start");

	pMsg.Result = CB_MULTIMIX_RESULT;
	pMsg.Pos = iMixCount;

	memset(pMsg.ItemInfo, 0x00, sizeof(pMsg.ItemInfo));

	lpObj->ChaosMassMixCurrItem = 0;
	lpObj->ChaosMassMixSuccessCount = 0;

	for (int i = 0; i < iMixCount; i++)
	{
		if ((rand() % 100) < g_iShieldPotionLv1MixSuccessRate)
		{
			BYTE btMixNumber = 1 << (i % 8);
			pMsg.ItemInfo[i / 8] |= btMixNumber;

			lpObj->ChaosMassMixSuccessCount++;

			g_Log.Add("[ChaosMix][Shield Potion] Lv1 Potion Mass Mix (%d) Success [%s][%s], Money(%d-%d)",
				lpObj->AccountID, lpObj->Name, lpObj->m_PlayerData->Money, iChaosMixPrice);
		}

		else
		{
			g_Log.Add("[ChaosMix][Shield Potion] Lv1 Potion Mass Mix Fail (%d) [%s][%s], Money(%d-%d)",
				lpObj->AccountID, lpObj->Name, lpObj->m_PlayerData->Money, iChaosMixPrice);
		}
	}

	IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

	if (lpObj->ChaosMassMixSuccessCount == 0)
	{
		ChaosBoxInit(lpObj);
		::GSProtocol.GCUserChaosBoxSend(lpObj, 0);

		lpObj->ChaosLock = FALSE;
		return;
	}

	for (int i = 0; i < lpObj->ChaosMassMixSuccessCount; i++)
	{
		int ItemNum = ITEMGET(14, 35);
		ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, ItemNum, 0, 1, 0, 0, 0, lpObj->m_Index, 0, 0, 0, 0, 0);
	}

	gObjInventoryCommit(lpObj->m_Index);
}

void CMixSystem::ShieldPotionLv2_Mix_Multi(LPOBJ lpObj, int iMixCount)
{
	int iHealthPotionCount = 0;
	int iInvalidItemCount = 0;
	int iChaosMixPrice = 0;

	if (g_ShieldSystemOn == FALSE)
		return;

	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;

	for (int n = 0; n < CHAOS_BOX_SIZE; n++)
	{
		if (lpObj->pChaosBox[n].IsItem() == TRUE)
		{
			if (lpObj->pChaosBox[n].m_Type == ITEMGET(14, 38))
			{
				iHealthPotionCount += lpObj->pChaosBox[n].m_Durability;
			}

			else
			{
				iInvalidItemCount++;
			}
		}
	}

	if (iInvalidItemCount > 0 || iHealthPotionCount != (3 * iMixCount))
	{
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	iChaosMixPrice = g_iShieldPotionLv2MixMoney * iMixCount;
	int iChaosTaxMoney = iChaosMixPrice * g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / 100;

	if (iChaosTaxMoney < 0)
	{
		iChaosTaxMoney = 0;
	}

	iChaosMixPrice += iChaosTaxMoney;

	if (iChaosMixPrice < 0)
	{
		iChaosMixPrice = 0;
	}

	if (lpObj->m_PlayerData->Money < iChaosMixPrice)
	{
		pMsg.Result = CB_NOT_ENOUGH_ZEN;

		IOCP.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	lpObj->m_PlayerData->Money -= iChaosMixPrice;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GSProtocol.GCMoneySend(lpObj->m_Index, lpObj->m_PlayerData->Money);

	LogChaosItem(lpObj, "PotionMix][ShieldPotion Lv2 Mix");
	g_Log.Add("[PotionMix][ShieldPotion Lv2 Mix] - Mix Start");

	pMsg.Result = CB_MULTIMIX_RESULT;
	pMsg.Pos = iMixCount;

	memset(pMsg.ItemInfo, 0x00, sizeof(pMsg.ItemInfo));

	lpObj->ChaosMassMixCurrItem = 0;
	lpObj->ChaosMassMixSuccessCount = 0;

	for (int i = 0; i < iMixCount; i++)
	{
		if ((rand() % 100) < g_iShieldPotionLv2MixSuccessRate)
		{
			BYTE btMixNumber = 1 << (i % 8);
			pMsg.ItemInfo[i / 8] |= btMixNumber;

			lpObj->ChaosMassMixSuccessCount++;

			g_Log.Add("[ChaosMix][Shield Potion] Lv2 Potion Mass Mix (%d) Success [%s][%s], Money(%d-%d)",
				lpObj->AccountID, lpObj->Name, lpObj->m_PlayerData->Money, iChaosMixPrice);
		}

		else
		{
			g_Log.Add("[ChaosMix][Shield Potion] Lv2 Potion Mass Mix Fail (%d) [%s][%s], Money(%d-%d)",
				lpObj->AccountID, lpObj->Name, lpObj->m_PlayerData->Money, iChaosMixPrice);
		}
	}

	IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

	if (lpObj->ChaosMassMixSuccessCount == 0)
	{
		ChaosBoxInit(lpObj);
		::GSProtocol.GCUserChaosBoxSend(lpObj, 0);

		lpObj->ChaosLock = FALSE;
		return;
	}

	for (int i = 0; i < lpObj->ChaosMassMixSuccessCount; i++)
	{
		int ItemNum = ITEMGET(14, 36);
		ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, ItemNum, 0, 1, 0, 0, 0, lpObj->m_Index, 0, 0, 0, 0, 0);
	}

	gObjInventoryCommit(lpObj->m_Index);
}

void CMixSystem::ShieldPotionLv3_Mix_Multi(LPOBJ lpObj, int iMixCount)
{
	int iHealthPotionCount = 0;
	int iInvalidItemCount = 0;
	int iChaosMixPrice = 0;

	if (g_ShieldSystemOn == FALSE)
		return;

	PMSG_CHAOSMIXRESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;

	for (int n = 0; n < CHAOS_BOX_SIZE; n++)
	{
		if (lpObj->pChaosBox[n].IsItem() == TRUE)
		{
			if (lpObj->pChaosBox[n].m_Type == ITEMGET(14, 39))
			{
				iHealthPotionCount += lpObj->pChaosBox[n].m_Durability;
			}

			else
			{
				iInvalidItemCount++;
			}
		}
	}

	if (iInvalidItemCount > 0 || iHealthPotionCount != (3 * iMixCount))
	{
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	iChaosMixPrice = g_iShieldPotionLv3MixMoney * iMixCount;
	int iChaosTaxMoney = iChaosMixPrice * g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index) / 100;

	if (iChaosTaxMoney < 0)
	{
		iChaosTaxMoney = 0;
	}

	iChaosMixPrice += iChaosTaxMoney;

	if (iChaosMixPrice < 0)
	{
		iChaosMixPrice = 0;
	}

	if (lpObj->m_PlayerData->Money < iChaosMixPrice)
	{
		pMsg.Result = CB_NOT_ENOUGH_ZEN;

		IOCP.DataSend(lpObj->m_Index, (BYTE *)&pMsg, pMsg.h.size);
		lpObj->ChaosLock = FALSE;

		return;
	}

	lpObj->m_PlayerData->Money -= iChaosMixPrice;
	g_CastleSiegeSync.AddTributeMoney(iChaosTaxMoney);
	GSProtocol.GCMoneySend(lpObj->m_Index, lpObj->m_PlayerData->Money);

	LogChaosItem(lpObj, "PotionMix][ShieldPotion Lv3 Mix");
	g_Log.Add("[PotionMix][ShieldPotion Lv3 Mix] - Mix Start");

	pMsg.Result = CB_MULTIMIX_RESULT;
	pMsg.Pos = iMixCount;

	memset(pMsg.ItemInfo, 0x00, sizeof(pMsg.ItemInfo));

	lpObj->ChaosMassMixCurrItem = 0;
	lpObj->ChaosMassMixSuccessCount = 0;

	for (int i = 0; i < iMixCount; i++)
	{
		if ((rand() % 100) < g_iShieldPotionLv3MixSuccessRate)
		{
			BYTE btMixNumber = 1 << (i % 8);
			pMsg.ItemInfo[i / 8] |= btMixNumber;

			lpObj->ChaosMassMixSuccessCount++;

			g_Log.Add("[ChaosMix][Shield Potion] Lv3 Potion Mass Mix (%d) Success [%s][%s], Money(%d-%d)",
				lpObj->AccountID, lpObj->Name, lpObj->m_PlayerData->Money, iChaosMixPrice);
		}

		else
		{
			g_Log.Add("[ChaosMix][Shield Potion] Lv3 Potion Mass Mix Fail (%d) [%s][%s], Money(%d-%d)",
				lpObj->AccountID, lpObj->Name, lpObj->m_PlayerData->Money, iChaosMixPrice);
		}
	}

	IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

	if (lpObj->ChaosMassMixSuccessCount == 0)
	{
		ChaosBoxInit(lpObj);
		::GSProtocol.GCUserChaosBoxSend(lpObj, 0);

		lpObj->ChaosLock = FALSE;
		return;
	}

	for (int i = 0; i < lpObj->ChaosMassMixSuccessCount; i++)
	{
		int ItemNum = ITEMGET(14, 37);
		ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, ItemNum, 0, 1, 0, 0, 0, lpObj->m_Index, 0, 0, 0, 0, 0);
	}

	gObjInventoryCommit(lpObj->m_Index);
}

void CMixSystem::PremiumBoxMix_Multi(LPOBJ lpObj, int iMixCount)
{
	PMSG_CHAOSMIXRESULT pMsg = { 0 };
	PHeadSetB((LPBYTE)&pMsg, 0x86, sizeof(pMsg));
	char SealedSilver = 0;
	char SealedGold = 0;
	char SilverKey = 0;
	char GoldKey = 0;
	char Invalid = 0;

	lpObj->ChaosLock = TRUE;

	for (int n = 0; n<CHAOS_BOX_SIZE; ++n)
	{
		if (lpObj->pChaosBox[n].IsItem() == TRUE)
		{
			if (lpObj->pChaosBox[n].m_Type == ITEMGET(14, 112))
				SilverKey++;
			else if (lpObj->pChaosBox[n].m_Type == ITEMGET(14, 113))
				GoldKey++;
			else if (lpObj->pChaosBox[n].m_Type == ITEMGET(14, 121))
				SealedGold++;
			else if (lpObj->pChaosBox[n].m_Type == ITEMGET(14, 122))
				SealedSilver++;
			else
				Invalid++;
		}
	}

	if (Invalid)
	{
		pMsg.Result = CB_INCORRECT_MIX_ITEMS;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	LogChaosItem(lpObj, "PremiumBoxMix");
	BYTE btMixType = 0xFF;

	if (SealedSilver == iMixCount && SilverKey == iMixCount && SealedGold == 0 && GoldKey == 0)
	{
		btMixType = 0;
	}

	else if (SealedGold == iMixCount && GoldKey == iMixCount && SealedSilver == 0 && SilverKey == 0)
	{
		btMixType = 1;
	}

	if (btMixType == 0xFF)
	{
		pMsg.Result = CB_INCORRECT_MIX_ITEMS;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	lpObj->ChaosSuccessRate = 100;

	pMsg.Result = CB_MULTIMIX_RESULT;
	pMsg.Pos = iMixCount;

	memset(pMsg.ItemInfo, 0x00, sizeof(pMsg.ItemInfo));

	lpObj->ChaosMassMixCurrItem = 0;
	lpObj->ChaosMassMixSuccessCount = 0;

	for (int i = 0; i < iMixCount; i++)
	{
		if ((rand() % 100) < lpObj->ChaosSuccessRate)
		{
			BYTE btMixNumber = 1 << (i % 8);
			pMsg.ItemInfo[i / 8] |= btMixNumber;

			lpObj->ChaosMassMixSuccessCount++;

			g_Log.Add("[PremiumBoxMix] Mass Mix (%d) Success [%s][%s]", 
				i, lpObj->AccountID, lpObj->Name);
		}

		else
		{
			g_Log.Add("[PremiumBoxMix] Mass Mix Fail (%d) [%s][%s]", 
				i,lpObj->AccountID, lpObj->Name);
		}
	}

	IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

	if (lpObj->ChaosMassMixSuccessCount == 0)
	{
		ChaosBoxInit(lpObj);
		::GSProtocol.GCUserChaosBoxSend(lpObj, 0);

		lpObj->ChaosLock = FALSE;
		return;
	}

	for (int i = 0; i < lpObj->ChaosMassMixSuccessCount; i++)
	{
		int iBoxType = -1;

		if (btMixType == 0)
		{
			iBoxType = ITEMGET(14, 124);
		}

		else if (btMixType == 1)
		{
			iBoxType = ITEMGET(14, 123);
		}

		ItemSerialCreateSend(lpObj->m_Index, -1, 0, 0, iBoxType, 0, 1, 0, 0, 0, lpObj->m_Index, 0, 0, 0, 0, 0);
	}
}


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

