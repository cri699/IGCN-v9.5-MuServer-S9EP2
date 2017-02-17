//////////////////////////////////////////////////////////////////////
// JewelMixSystem.cpp
// ------------------------------
// Decompiled by Deathway
// Date : 2007-05-09
// ------------------------------
// Gs-N 0.99.60T 0x004E1EB0
//	GS-N	1.00.18	JPN	0x0050A790	-	Completed
#include "stdafx.h"
#include "JewelMixSystem.h"
#include "PeriodItemEx.h"
#include "DSprotocol.h"
#include "TLog.h"
#include "user.h"
#include "configread.h"

int CJewelMixSystem::UnMixJewelPrice;

CJewelMixSystem::CJewelMixSystem()
{
	return;
}

CJewelMixSystem::~CJewelMixSystem()
{
	return;
}

int CJewelMixSystem::GetJewelCount(int iIndex, int iJewelType)
{
	if ( !gObjIsConnected(iIndex) )
		return -1;

	int iItemType;

	switch ( iJewelType )
	{
		case 0:
			iItemType = ITEMGET(14,13);
			break;
		case 1:
			iItemType = ITEMGET(14,14);
			break;
		default:
			return -1;
	}	
	
	int iItemCount = gObjGetItemCountInIventory(iIndex, iItemType);
	return iItemCount;
}


struct JEWELMIX_INFO
{
	int m_iJewelCount;	// 0
	int m_iMixMoney;	// 4
	int m_iSourceType;	// 8
	int m_iChangeType;	// C
};

static JEWELMIX_INFO g_JewelMixInfo[10][3] = { 10, 500000, ITEMGET(14,13), ITEMGET(12,30),
												   20, 1000000, ITEMGET(14,13), ITEMGET(12,30),
												   30, 1500000, ITEMGET(14,13), ITEMGET(12,30), // bless
												   10, 500000, ITEMGET(14,14), ITEMGET(12,31),
												   20, 1000000, ITEMGET(14,14), ITEMGET(12,31),
												   30, 1500000, ITEMGET(14,14), ITEMGET(12,31), // soul
												   10, 500000, ITEMGET(14,16), ITEMGET(12,136),
												   20, 1000000, ITEMGET(14,16), ITEMGET(12,136),
												   30, 1500000, ITEMGET(14,16), ITEMGET(12,136), // life
												   10, 500000, ITEMGET(14,22), ITEMGET(12,137),
												   20, 1000000, ITEMGET(14,22), ITEMGET(12,137),
												   30, 1500000, ITEMGET(14,22), ITEMGET(12,137), //creation
												   10, 500000, ITEMGET(14,31), ITEMGET(12,138),
												   20, 1000000, ITEMGET(14,31), ITEMGET(12,138),
												   30, 1500000, ITEMGET(14,31), ITEMGET(12,138), // guardian
												   10, 500000, ITEMGET(14,41), ITEMGET(12,139),
												   20, 1000000, ITEMGET(14,41), ITEMGET(12,139),
												   30, 1500000, ITEMGET(14,41), ITEMGET(12,139), // gemstone
												   10, 500000, ITEMGET(14,42), ITEMGET(12,140),
												   20, 1000000, ITEMGET(14,42), ITEMGET(12,140),
												   30, 1500000, ITEMGET(14,42), ITEMGET(12,140), // harmony
												   10, 500000, ITEMGET(12,15), ITEMGET(12,141),
												   20, 1000000, ITEMGET(12,15), ITEMGET(12,141),
												   30, 1500000, ITEMGET(12,15), ITEMGET(12,141), // chaos
												   10, 500000, ITEMGET(14,43), ITEMGET(12,142),
												   20, 1000000, ITEMGET(14,43), ITEMGET(12,142),
												   30, 1500000, ITEMGET(14,43), ITEMGET(12,142), // lower refine stone
												   10, 500000, ITEMGET(14,44), ITEMGET(12,143),
												   20, 1000000, ITEMGET(14,44), ITEMGET(12,143),
												   30, 1500000, ITEMGET(14,44), ITEMGET(12,143), // Higher refining stone

};

void CJewelMixSystem::LoadMixJewelPrice() // PRIVATE CONFIG FOR JOINMU / NEWYORKER
{
	CIniReader ReadJewelMix(g_ConfigRead.GetPath("IGC_Common.ini"));

	g_JewelMixInfo[0][0].m_iMixMoney = ReadJewelMix.ReadInt("joinmu", "Bless10MixPrice", 500000);
	g_JewelMixInfo[0][1].m_iMixMoney = ReadJewelMix.ReadInt("joinmu", "Bless20MixPrice", 1000000);
	g_JewelMixInfo[0][2].m_iMixMoney = ReadJewelMix.ReadInt("joinmu", "Bless30MixPrice", 1500000);

	g_JewelMixInfo[1][0].m_iMixMoney = ReadJewelMix.ReadInt("joinmu", "Soul10MixPrice", 500000);
	g_JewelMixInfo[1][1].m_iMixMoney = ReadJewelMix.ReadInt("joinmu", "Soul20MixPrice", 1000000);
	g_JewelMixInfo[1][2].m_iMixMoney = ReadJewelMix.ReadInt("joinmu", "Soul30MixPrice", 1500000);

	g_JewelMixInfo[2][0].m_iMixMoney = ReadJewelMix.ReadInt("joinmu", "Life10MixPrice", 500000);
	g_JewelMixInfo[2][1].m_iMixMoney = ReadJewelMix.ReadInt("joinmu", "Life20MixPrice", 1000000);
	g_JewelMixInfo[2][2].m_iMixMoney = ReadJewelMix.ReadInt("joinmu", "Life30MixPrice", 1500000);

	g_JewelMixInfo[3][0].m_iMixMoney = ReadJewelMix.ReadInt("joinmu", "Creation10MixPrice", 500000);
	g_JewelMixInfo[3][1].m_iMixMoney = ReadJewelMix.ReadInt("joinmu", "Creation20MixPrice", 1000000);
	g_JewelMixInfo[3][2].m_iMixMoney = ReadJewelMix.ReadInt("joinmu", "Creation30MixPrice", 1500000);

	g_JewelMixInfo[4][0].m_iMixMoney = ReadJewelMix.ReadInt("joinmu", "Guardian10MixPrice", 500000);
	g_JewelMixInfo[4][1].m_iMixMoney = ReadJewelMix.ReadInt("joinmu", "Guardian20MixPrice", 1000000);
	g_JewelMixInfo[4][2].m_iMixMoney = ReadJewelMix.ReadInt("joinmu", "Guardian30MixPrice", 1500000);

	g_JewelMixInfo[5][0].m_iMixMoney = ReadJewelMix.ReadInt("joinmu", "Gemstone10MixPrice", 500000);
	g_JewelMixInfo[5][1].m_iMixMoney = ReadJewelMix.ReadInt("joinmu", "Gemstone20MixPrice", 1000000);
	g_JewelMixInfo[5][2].m_iMixMoney = ReadJewelMix.ReadInt("joinmu", "Gemstone30MixPrice", 1500000);

	g_JewelMixInfo[6][0].m_iMixMoney = ReadJewelMix.ReadInt("joinmu", "Harmony10MixPrice", 500000);
	g_JewelMixInfo[6][1].m_iMixMoney = ReadJewelMix.ReadInt("joinmu", "Harmony20MixPrice", 1000000);
	g_JewelMixInfo[6][2].m_iMixMoney = ReadJewelMix.ReadInt("joinmu", "Harmony30MixPrice", 1500000);

	g_JewelMixInfo[7][0].m_iMixMoney = ReadJewelMix.ReadInt("joinmu", "Chaos10MixPrice", 500000);
	g_JewelMixInfo[7][1].m_iMixMoney = ReadJewelMix.ReadInt("joinmu", "Chaos20MixPrice", 1000000);
	g_JewelMixInfo[7][2].m_iMixMoney = ReadJewelMix.ReadInt("joinmu", "Chaos30MixPrice", 1500000);

	g_JewelMixInfo[8][0].m_iMixMoney = ReadJewelMix.ReadInt("joinmu", "LowerRefineStone10MixPrice", 500000);
	g_JewelMixInfo[8][1].m_iMixMoney = ReadJewelMix.ReadInt("joinmu", "LowerRefineStone20MixPrice", 1000000);
	g_JewelMixInfo[8][2].m_iMixMoney = ReadJewelMix.ReadInt("joinmu", "LowerRefineStone30MixPrice", 1500000);

	g_JewelMixInfo[9][0].m_iMixMoney = ReadJewelMix.ReadInt("joinmu", "HigherRefineStone10MixPrice", 500000);
	g_JewelMixInfo[9][1].m_iMixMoney = ReadJewelMix.ReadInt("joinmu", "HigherRefineStone20MixPrice", 1000000);
	g_JewelMixInfo[9][2].m_iMixMoney = ReadJewelMix.ReadInt("joinmu", "HigherRefineStone30MixPrice", 1500000);

	CJewelMixSystem::UnMixJewelPrice = ReadJewelMix.ReadInt("joinmu", "UnMixJewelPrice", 1000000);
}
									 

int CJewelMixSystem::GetJewelCountPerLevel(int iJewelType, int iJewelLevel)
{
	if ( !CHECK_LIMIT(iJewelLevel, 3) )
		return -1;

	int iJewelCount = 0;

	switch ( iJewelType )
	{
		case 0:
			iJewelCount = g_JewelMixInfo[0][iJewelLevel].m_iJewelCount;
			break;
		case 1:
			iJewelCount = g_JewelMixInfo[1][iJewelLevel].m_iJewelCount;
			break;
		case 2:
			iJewelCount = g_JewelMixInfo[2][iJewelLevel].m_iJewelCount;
			break;
		case 3:
			iJewelCount = g_JewelMixInfo[3][iJewelLevel].m_iJewelCount;
			break;
		case 4:
			iJewelCount = g_JewelMixInfo[4][iJewelLevel].m_iJewelCount;
			break;
		case 5:
			iJewelCount = g_JewelMixInfo[5][iJewelLevel].m_iJewelCount;
			break;
		case 6:
			iJewelCount = g_JewelMixInfo[6][iJewelLevel].m_iJewelCount;
			break;
		case 7:
			iJewelCount = g_JewelMixInfo[7][iJewelLevel].m_iJewelCount;
			break;
		case 8:
			iJewelCount = g_JewelMixInfo[8][iJewelLevel].m_iJewelCount;
			break;
		case 9:
			iJewelCount = g_JewelMixInfo[9][iJewelLevel].m_iJewelCount;
			break;
		/*case 10: // array overrun
			iJewelCount = g_JewelMixInfo[10][iJewelLevel].m_iJewelCount;
			break;*/
		default:
			return -1;
	}

	return iJewelCount;
}

BOOL CJewelMixSystem::MixJewel( int iIndex, int iJewelType, int iMixType)
{
	if ( !gObjIsConnected(iIndex))
		return FALSE;

/*	if ( gObjCheckInventorySerial0Item(&gObj[iIndex]))
	{
		MsgOutput(iIndex, lMsg.Get(MSGGET(13,26)));
		GCAnsJewelMix(iIndex, 4);

		g_Log.Add("[ANTI-HACK][Serial 0 Item] [Mix Jewel] (%s)(%s)",
			gObj[iIndex].AccountID, gObj[iIndex].Name);

		return FALSE;
	}*/

	if ( gObj[iIndex].m_IfState.use != 1 || gObj[iIndex].m_IfState.type != 12 )
	{
		g_Log.Add("[ANTI-HACK][protocol] [Mix Jewel] (%s)(%s)",
			gObj[iIndex].AccountID, gObj[iIndex].Name);

		return FALSE;
	}

	if ( gObj[iIndex].ChaosLock == TRUE )
	{
		g_Log.Add("[JewelMix] [%s][%s] Chaos Mix is already working",
			gObj[iIndex].AccountID, gObj[iIndex].Name);

		GSProtocol.GCAnsJewelMix(iIndex, 0);
		return FALSE;
	}

	gObj[iIndex].ChaosLock = TRUE;

	if ( !CHECK_LIMIT(iJewelType, 10) )
	{
		g_Log.Add("[JewelMix] [%s][%s] Mix iJewelType is out of bound : %d",
			gObj[iIndex].AccountID, gObj[iIndex].Name, iJewelType);

		gObj[iIndex].ChaosLock = FALSE;
		GSProtocol.GCAnsJewelMix(iIndex, 2);
		return FALSE;
	}

	if ( !CHECK_LIMIT(iMixType, 10))
	{
		g_Log.Add("[JewelMix] [%s][%s] iMixType is out of bound : %d",
			gObj[iIndex].AccountID, gObj[iIndex].Name, iMixType);

		gObj[iIndex].ChaosLock = FALSE;
		GSProtocol.GCAnsJewelMix(iIndex, 3);
		return FALSE;
	}

	int iItemType = g_JewelMixInfo[iJewelType][iMixType].m_iSourceType;
	int iJewelCount = g_JewelMixInfo[iJewelType][iMixType].m_iJewelCount;
	int iMixMoney = g_JewelMixInfo[iJewelType][iMixType].m_iMixMoney;
	int iChangeType = g_JewelMixInfo[iJewelType][iMixType].m_iChangeType;

	if ( iJewelCount <= 0 )
	{
		gObj[iIndex].ChaosLock = FALSE;
		GSProtocol.GCAnsJewelMix(iIndex, 0);
		return FALSE;
	}

	int iUserJewelCount = gObjGetItemCountInIventory(iIndex, iItemType);

	if ( iJewelCount > iUserJewelCount )
	{
		g_Log.Add("[JewelMix] [%s][%s] lack of jewel to mix : %d / %d",
			gObj[iIndex].AccountID, gObj[iIndex].Name,
			iUserJewelCount, iJewelCount);

		gObj[iIndex].ChaosLock = FALSE;
		GSProtocol.GCAnsJewelMix(iIndex, 4);
		return FALSE;
	}

	if ( iMixMoney > gObj[iIndex].m_PlayerData->Money )
	{
		g_Log.Add("[JewelMix] [%s][%s] lack of money to mix : %d / %d",
			gObj[iIndex].AccountID, gObj[iIndex].Name,
			gObj[iIndex].m_PlayerData->Money, iMixMoney);

		gObj[iIndex].ChaosLock = FALSE;
		GSProtocol.GCAnsJewelMix(iIndex, 5);
		return FALSE;
	}

	int iDelJewelCount = 0;
	BOOL bItemDelOK = FALSE;

	for ( int x= INVETORY_WEAR_SIZE ; x< MAIN_INVENTORY_SIZE; x++)
	{
		if ( gObj[iIndex].pInventory[x].IsItem() == TRUE )
		{
			if ( gObj[iIndex].pInventory[x].m_Type == iItemType )
			{
				g_Log.Add("[JewelMix] [%s][%s] Mix - Delete Jewel, Type:%d, Level:%d, Serial:%I64d",
					gObj[iIndex].AccountID, gObj[iIndex].Name,
					gObj[iIndex].pInventory[x].m_Type,
					gObj[iIndex].pInventory[x].m_Level,
					gObj[iIndex].pInventory[x].m_Number);

				gObjInventoryItemSet(iIndex, x, 0xFF);
				gObj[iIndex].pInventory[x].Clear();
				iDelJewelCount++;

				if ( iJewelCount <= iDelJewelCount )
				{
					bItemDelOK = TRUE;
					break;
				}
			}
		}
	}

	GCItemListSend(iIndex);
	g_PeriodItemEx.OnRequestPeriodItemList(&gObj[iIndex]);

	if ( bItemDelOK == FALSE )
	{
		g_Log.Add("[JewelMix] [%s][%s] lack of jewel to mix (in deleting) : %d / %d",
			gObj[iIndex].AccountID, gObj[iIndex].Name,
			iDelJewelCount, iJewelCount);

		gObj[iIndex].ChaosLock = FALSE;
		GSProtocol.GCAnsJewelMix(iIndex, 0);
		return FALSE;
	}

	ItemSerialCreateSend(iIndex, 235, gObj[iIndex].X,
						gObj[iIndex].Y, iChangeType,
						iMixType, 0, 0, 0, 0,
						iIndex, 0, 0, 0, 0, 0);

	gObj[iIndex].ChaosLock = FALSE;
	gObj[iIndex].m_PlayerData->Money -= iMixMoney;
	GSProtocol.GCMoneySend(iIndex, gObj[iIndex].m_PlayerData->Money);
	GSProtocol.GCAnsJewelMix(iIndex, 1);

	g_Log.Add("[JewelMix] [%s][%s] jewel mix succeed : ItemType:%d, JewelCount:%d",
		gObj[iIndex].AccountID, gObj[iIndex].Name,
		iItemType, iJewelCount);

	return TRUE;
}

BOOL CJewelMixSystem::UnMixJewel(int iIndex, int iJewelType, int iJewelLevel, int iInventoryPos)
{
	if ( !gObjIsConnected(iIndex))
		return FALSE;

/*	if ( gObjCheckInventorySerial0Item(&gObj[iIndex]))
	{
		MsgOutput(iIndex, lMsg.Get(MSGGET(13,26)));
		GCAnsJewelUnMix(iIndex, 0);

		g_Log.Add("[ANTI-HACK][Serial 0 Item] [UnMix Jewel] (%s)(%s)",
			gObj[iIndex].AccountID, gObj[iIndex].Name);

		return FALSE;
	}*/

	if ( gObj[iIndex].m_IfState.use != 1 || gObj[iIndex].m_IfState.type != 12 )
	{
		g_Log.Add("[ANTI-HACK][protocol] [UnMix Jewel] (%s)(%s)",
			gObj[iIndex].AccountID, gObj[iIndex].Name);

		return FALSE;
	}

	if ( gObj[iIndex].ChaosLock == TRUE )
	{
		g_Log.Add("[JewelMix] [%s][%s] Chaos Mix is already working",
			gObj[iIndex].AccountID, gObj[iIndex].Name);

		GSProtocol.GCAnsJewelUnMix(iIndex, 0);
		return FALSE;
	}

	gObj[iIndex].ChaosLock = TRUE;

	if ( !CHECK_LIMIT(iJewelType, 10) )
	{
		g_Log.Add("[JewelMix] [%s][%s] UnMix iJewelType is out of bound : %d",
			gObj[iIndex].AccountID, gObj[iIndex].Name, iJewelType);

		gObj[iIndex].ChaosLock = FALSE;
		GSProtocol.GCAnsJewelUnMix(iIndex, 2);
		return FALSE;
	}

	if ( !CHECK_LIMIT(iInventoryPos, MAIN_INVENTORY_SIZE))
	{
		g_Log.Add("[JewelMix] [%s][%s] iInventoryPos is out of bound : %d",
			gObj[iIndex].AccountID, gObj[iIndex].Name, iInventoryPos);

		gObj[iIndex].ChaosLock = FALSE;
		GSProtocol.GCAnsJewelUnMix(iIndex, 5);
		return FALSE;
	}

	int iInventoryItemType;
	int iInventoryItemLevel;

	if ( gObj[iIndex].pInventory[iInventoryPos].IsItem() == TRUE )
	{
		if ( gObj[iIndex].pInventory[iInventoryPos].m_Level == iJewelLevel )
		{
			iInventoryItemType = gObj[iIndex].pInventory[iInventoryPos].m_Type;
			iInventoryItemLevel = gObj[iIndex].pInventory[iInventoryPos].m_Level;
		}
		else
		{
			g_Log.Add("[JewelMix] [%s][%s] iJewelLevel is different from request : %d / %d",
				gObj[iIndex].AccountID, gObj[iIndex].Name,
				gObj[iIndex].pInventory[iInventoryPos].m_Level, iJewelLevel);

			gObj[iIndex].ChaosLock = FALSE;
			GSProtocol.GCAnsJewelUnMix(iIndex, 3);
			return FALSE;
		}
	}
	else
	{
		g_Log.Add("[JewelMix] [%s][%s] Item to unmix is not exist",
			gObj[iIndex].AccountID, gObj[iIndex].Name);
	
		gObj[iIndex].ChaosLock = FALSE;
		GSProtocol.GCAnsJewelUnMix(iIndex, 4);
		return FALSE;
	}

	if ( !CHECK_LIMIT(iInventoryItemLevel, 3))
	{
		g_Log.Add("[JewelMix] [%s][%s] iInventoryItemLevel is out of bound : %d",
			gObj[iIndex].AccountID, gObj[iIndex].Name, iInventoryItemLevel);

		gObj[iIndex].ChaosLock = FALSE;
		GSProtocol.GCAnsJewelUnMix(iIndex, 3);
		return FALSE;
	}

	int iItemType = g_JewelMixInfo[iJewelType][iInventoryItemLevel].m_iChangeType;
	int iJewelCount = g_JewelMixInfo[iJewelType][iInventoryItemLevel].m_iJewelCount;
	int iChangeType = g_JewelMixInfo[iJewelType][iInventoryItemLevel].m_iSourceType;
	int iMixMoney = CJewelMixSystem::UnMixJewelPrice;

	if ( iInventoryItemType != iItemType )
	{
		g_Log.Add("[JewelMix] [%s][%s] iItemType is different from request : %d / %d",
			gObj[iIndex].AccountID, gObj[iIndex].Name,
			iInventoryItemType, iItemType);

		gObj[iIndex].ChaosLock = FALSE;
		GSProtocol.GCAnsJewelUnMix(iIndex, 6);
		return FALSE;
	}

	if ( iMixMoney > gObj[iIndex].m_PlayerData->Money )
	{
		g_Log.Add("[JewelMix] [%s][%s] lack of money to unmix : %d / %d",
			gObj[iIndex].AccountID, gObj[iIndex].Name,
			gObj[iIndex].m_PlayerData->Money, iMixMoney);

		gObj[iIndex].ChaosLock = FALSE;
		GSProtocol.GCAnsJewelUnMix(iIndex, 8);
		return FALSE;
	}

	int iEmptyCount = CheckInventoryEmptySpaceCount(&gObj[iIndex], 1, 1);

	if ( iEmptyCount < iJewelCount )
	{
		g_Log.Add("[JewelMix] [%s][%s] lack of empty slot to unmix : %d / %d",
			gObj[iIndex].AccountID, gObj[iIndex].Name,
			iEmptyCount, iJewelCount);

		gObj[iIndex].ChaosLock = FALSE;
		GSProtocol.GCAnsJewelUnMix(iIndex, 7);
		return FALSE;
	}

	g_Log.Add("[JewelMix] [%s][%s] UnMix - Delete Jewel, Type:%d, Level:%d, Serial:%I64d",
		gObj[iIndex].AccountID, gObj[iIndex].Name,
		gObj[iIndex].pInventory[iInventoryPos].m_Type,
		gObj[iIndex].pInventory[iInventoryPos].m_Level,
		gObj[iIndex].pInventory[iInventoryPos].m_Number);

	gObjInventoryItemSet(iIndex, iInventoryPos, 0xFF);
	gObjInventoryDeleteItem(iIndex, iInventoryPos);
	GSProtocol.GCInventoryItemDeleteSend(iIndex, iInventoryPos, 1);

	int iCrtJewelCount = 0;
	BOOL bItemCrtOK = FALSE;

	for (int x = 0; x < iJewelCount; x++)
	{
		ItemSerialCreateSend(iIndex, 235, gObj[iIndex].X, gObj[iIndex].Y, iChangeType, 0, 0, 0, 0, 0, iIndex, 0, 0, 0, 0, 0);
		iCrtJewelCount++;

		if ( iJewelCount <= iCrtJewelCount )
		{
			bItemCrtOK = TRUE;
			break;
		}
	}

	gObj[iIndex].ChaosLock = FALSE;

	if ( bItemCrtOK == TRUE )
	{
		g_Log.Add("[JewelMix] [%s][%s] jewel unmix succeed : ItemType:%d, JewelCount:%d",
			gObj[iIndex].AccountID, gObj[iIndex].Name,
			iItemType, iJewelCount);

		gObj[iIndex].m_PlayerData->Money -= iMixMoney;
		GSProtocol.GCMoneySend(iIndex, gObj[iIndex].m_PlayerData->Money);
		GSProtocol.GCAnsJewelUnMix(iIndex, 1);
	}
	else
	{
		g_Log.Add("[JewelMix] [%s][%s] jewel unmix failed : ItemType:%d, JewelCount:%d",
			gObj[iIndex].AccountID, gObj[iIndex].Name,
			iItemType, iJewelCount);

		GSProtocol.GCAnsJewelUnMix(iIndex, 0);
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

