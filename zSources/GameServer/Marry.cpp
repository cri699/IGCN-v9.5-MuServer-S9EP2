//////////////////////////////////////////////////////////////////////
// Marry.cpp
#include "stdafx.h"
#include "Marry.h"
#include "protocol.h"
#include "TLog.h"
#include "winutil.h"
#include "user.h"
#include "DSProtocol.h"
#include "GameMain.h"
CMarry Marry;

CMarry::CMarry()
{
	InitializeCriticalSection(&this->m_criti);
}

CMarry::~CMarry()
{
	DeleteCriticalSection(&this->m_criti);
}

void CMarry::LoadFile(char * filename)
{
	EnterCriticalSection(&this->m_criti);

	this->m_bFileLoad = false;

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(filename);

	if (res.status != pugi::status_ok)
	{
		g_Log.MsgBox("Error loading %s file (%s)", filename, res.description());

		LeaveCriticalSection(&this->m_criti);
		return;
	}

	pugi::xml_node main = file.child("MarrySystem");

	this->m_bMarryEnable = main.attribute("Enable").as_bool();
	this->m_iMarryMinUserLevel = main.attribute("ReqLevel").as_int();
	this->m_iMarryNeedMoney = main.attribute("ReqMoney").as_int();

	pugi::xml_node location = main.child("Location");

	this->m_wMarryMapNumber = location.attribute("MapNumber").as_int();
	this->m_btMarryStartX = location.attribute("StartX").as_int();
	this->m_btMarryStartY = location.attribute("StartY").as_int();
	this->m_btMarryEndX = location.attribute("EndX").as_int();
	this->m_btMarryEndY = location.attribute("EndY").as_int();

	pugi::xml_node wedding = main.child("WeddingSettings");

	this->m_bHomoSexualAllow = wedding.attribute("Homosexual").as_bool();
	this->m_bNeedSpecialItemForMarry = wedding.attribute("ReqSpecialItem").as_bool();

	this->m_vtMarryItem.clear();
	
	for (pugi::xml_node item = wedding.child("Item"); item; item = item.next_sibling())
	{
		MARRY_ITEM m_Item;

		m_Item.m_iSide = item.attribute("Side").as_int();
		int iItemID = ItemGetNumberMake(item.attribute("ItemCat").as_int(), item.attribute("ItemIndex").as_int());

		if (iItemID == -1)
		{
			g_Log.MsgBox("ERROR - Wrong item in %s file (%d %d)", filename, item.attribute("ItemCat").as_int(), item.attribute("ItemIndex").as_int());
			continue;
		}

		m_Item.m_wItemID = iItemID;
		m_Item.m_iItemCount = item.attribute("ItemCount").as_int();

		this->m_vtMarryItem.push_back(m_Item);
	}

	pugi::xml_node gift = main.child("WeddingGift");

	this->m_bGiftEnable = gift.attribute("Enable").as_bool();

	this->m_vtGiftItem.clear();

	for (pugi::xml_node item = gift.child("Item"); item; item = item.next_sibling())
	{
		MARRY_ITEM m_Item;

		m_Item.m_iSide = item.attribute("Side").as_int();
		int iItemID = ItemGetNumberMake(item.attribute("ItemCat").as_int(), item.attribute("ItemIndex").as_int());

		if (iItemID == -1)
		{
			g_Log.MsgBox("ERROR - Wrong item in %s file (%d %d)", filename, item.attribute("ItemCat").as_int(), item.attribute("ItemIndex").as_int());
			continue;
		}

		m_Item.m_wItemID = iItemID;
		m_Item.m_iItemCount = item.attribute("ItemCount").as_int();

		this->m_vtGiftItem.push_back(m_Item);
	}

	pugi::xml_node divorce = main.child("DivorceSettings");

	this->m_bDivorceAllow = divorce.attribute("Allow").as_bool();
	this->m_bNeedSpecialItemForDivorce = divorce.attribute("ReqSpecialItem").as_bool();

	this->m_vtDivorceItem.clear();

	for (pugi::xml_node item = divorce.child("Item"); item; item = item.next_sibling())
	{
		MARRY_ITEM m_Item;

		m_Item.m_iSide = 0;
		int iItemID = ItemGetNumberMake(item.attribute("ItemCat").as_int(), item.attribute("ItemIndex").as_int());

		if (iItemID == -1)
		{
			g_Log.MsgBox("ERROR - Wrong item in %s file (%d %d)", filename, item.attribute("ItemCat").as_int(), item.attribute("ItemIndex").as_int());
			continue;
		}

		m_Item.m_wItemID = iItemID;
		m_Item.m_iItemCount = item.attribute("ItemCount").as_int();

		this->m_vtDivorceItem.push_back(m_Item);
	}

	this->m_bFileLoad = true;
	LeaveCriticalSection(&this->m_criti);
}

void CMarry::Propose(int aIndex, int uIndex)
{
	if (ObjectMaxRange(aIndex) == 0 || ObjectMaxRange(uIndex) == 0)
	{
		return;
	}

	if (gObj[uIndex].Type != OBJ_USER)
	{
		return;
	}

	if (gObj[aIndex].Name == gObj[uIndex].Name)
	{
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,393), aIndex, 1);
		return;
	}

	if (!this->m_bMarryEnable)
	{
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,373), aIndex, 1);
		return;
	}

	if (gObj[aIndex].Married != 0 || gObj[uIndex].Married != 0)
	{
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,374), aIndex, 1);
		return;
	}

	if (isFemale(aIndex) == isFemale(uIndex) && !this->m_bHomoSexualAllow)
	{
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,375), aIndex, 1);
		return;
	}

	if (isMale(aIndex) == isMale(uIndex) && !this->m_bHomoSexualAllow)
	{
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,375), aIndex, 1);
		return;
	}

	if (this->m_iMarryNeedMoney > gObj[aIndex].m_PlayerData->Money || this->m_iMarryNeedMoney > gObj[uIndex].m_PlayerData->Money)
	{
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,376), aIndex, 1);
		return;
	}

	if (this->m_iMarryMinUserLevel > gObj[aIndex].Level || this->m_iMarryMinUserLevel > gObj[uIndex].Level)
	{
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,377), aIndex, 1);
		return;
	}

	if (!this->CheckPosition(aIndex, uIndex))
	{
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,378), aIndex, 1);
		return;
	}

	bool bCheck1 = this->CheckRequiredItem(aIndex, 1);
	bool bCheck2 = this->CheckRequiredItem(uIndex, 2);

	if (bCheck1 == false || bCheck2 == false)
	{
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,379), aIndex, 1);
		return;
	}

	gObj[uIndex].MarryRequested = 1;
	gObj[uIndex].MarryRequestIndex = aIndex;
	gObj[uIndex].MarryRequestTime = GetTickCount();

	char msg[128];
	wsprintf(msg, Lang.GetText(0,380), gObj[aIndex].Name);
	GSProtocol.GCServerMsgStringSend(msg, uIndex, 1);
	GSProtocol.GCServerMsgStringSend(Lang.GetText(0,381), aIndex, 1);

	g_Log.Add("[Marry][%s][%s] Request to marry with [%s][%s]", gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[uIndex].AccountID, gObj[uIndex].Name);
}

bool CMarry::Accept(int aIndex)
{
	if (ObjectMaxRange(aIndex) == 0)
	{
		return false;
	}

	char info[255];
	int uIndex = gObj[aIndex].MarryRequestIndex;

	if (gObj[aIndex].MarryRequested != 1)
	{
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,382), aIndex, 1);
		return false;
	}

	if (GetTickCount() - gObj[aIndex].MarryRequestTime > 60000)
	{
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,383), aIndex, 1);
		gObj[aIndex].MarryRequested = 0;
		return false;
	}

	if (!this->CheckPosition(aIndex, uIndex))
	{
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,384), aIndex, 1);
		return false;
	}

	if (gObj[uIndex].m_PlayerData->Money < gObj[uIndex].m_PlayerData->Money - this->m_iMarryNeedMoney || gObj[aIndex].m_PlayerData->Money < gObj[aIndex].m_PlayerData->Money - this->m_iMarryNeedMoney)
	{
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,385), aIndex, 1);
		return false;
	}

	bool bCheck1 = this->CheckRequiredItem(aIndex, 1);
	bool bCheck2 = this->CheckRequiredItem(uIndex, 2);

	if (bCheck1 == false || bCheck2 == false)
	{
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,386), aIndex, 1);
		return false;
	}

	this->DeleteRequiredItem(aIndex, 1);
	this->DeleteRequiredItem(uIndex, 2);

	gObj[aIndex].Married = 1;
	gObj[aIndex].MarryRequestIndex = 0;
	gObj[aIndex].MarryRequestTime = 0;
	strcpy(gObj[aIndex].MarryName, gObj[uIndex].Name);

	gObj[aIndex].m_PlayerData->Money -= this->m_iMarryNeedMoney;
	GSProtocol.GCMoneySend(aIndex, gObj[aIndex].m_PlayerData->Money);

	GSProtocol.GCServerMsgStringSend(Lang.GetText(0,387), aIndex, 1);

	gObj[uIndex].Married = 1;
	gObj[uIndex].MarryRequestIndex = 0;
	gObj[uIndex].MarryRequestTime = 0;
	strcpy(gObj[uIndex].MarryName, gObj[aIndex].Name);

	gObj[uIndex].m_PlayerData->Money -= this->m_iMarryNeedMoney;
	GSProtocol.GCMoneySend(uIndex, gObj[uIndex].m_PlayerData->Money);

	GSProtocol.GCServerMsgStringSend(Lang.GetText(0,388), uIndex, 1);
		
	PMSG_SERVERCMD ServerCmd;

	PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));
	ServerCmd.CmdType = 0;
	ServerCmd.X = gObj[uIndex].X;
	ServerCmd.Y = gObj[uIndex].Y;

	GSProtocol.MsgSendV2(gObj, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
	IOCP.DataSend(uIndex, (LPBYTE)&ServerCmd, sizeof(ServerCmd));

	ServerCmd.X = gObj[aIndex].X;
	ServerCmd.Y = gObj[aIndex].Y;

	GSProtocol.MsgSendV2(gObj, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
	IOCP.DataSend(aIndex, (LPBYTE)&ServerCmd, sizeof(ServerCmd));

	if (this->m_bGiftEnable == true)
	{
		this->GiveGiftItem(aIndex, 1);
		this->GiveGiftItem(uIndex, 2);
	}

	wsprintf(info, Lang.GetText(0,389), gObj[uIndex].Name, gObj[aIndex].Name);
	GSProtocol.AllSendServerMsg(info);

	g_Log.Add("[Marry] New Marriage: %s [%d]  %s [%d]", gObj[uIndex].Name, gObj[aIndex].Married, gObj[aIndex].Name, gObj[uIndex].Married);
	return true;
}

void CMarry::Divorce(int aIndex)
{
	if (gObj[aIndex].Married == 0)
	{
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,390), aIndex, 1);
		return;
	}

	if (this->m_bDivorceAllow == false)
	{
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,614), aIndex, 1);
		return;
	}

	if (!this->CheckDivorceItem(aIndex))
	{
		MsgOutput(aIndex, Lang.GetText(0,391));
		return;
	}

	GSProtocol.GCServerMsgStringSend(Lang.GetText(0,392), aIndex, 1);

	int uIndex = gObjGetIndex(gObj[aIndex].MarryName);

	if (uIndex != -1)
	{
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,392), uIndex, 1);
		gObj[uIndex].Married = 0;
		strcpy(gObj[uIndex].MarryName, "");
	}

	gObj[aIndex].Married = 0;
	strcpy(gObj[aIndex].MarryName, "");

	this->DeleteDivorceItem(aIndex);

	g_Log.Add("[Marry] Divorce: [%s] [%d]", gObj[aIndex].Name, gObj[uIndex].Name);
}

bool CMarry::CheckPosition(int aIndex, int uIndex)
{
	int aX = gObj[aIndex].X, aY = gObj[aIndex].Y, aM = gObj[aIndex].MapNumber, uX = gObj[uIndex].X, uY = gObj[uIndex].Y, uM = gObj[uIndex].MapNumber;

	if((aX >= this->m_btMarryStartX && aX <= this->m_btMarryEndX) && (aY >= this->m_btMarryStartY && aY <= this->m_btMarryEndY) && (uX >= this->m_btMarryStartX && uX <= this->m_btMarryEndX) && (uY >= this->m_btMarryStartY && uY <= this->m_btMarryEndY))
	{
		return true;
	}

	return false;
}

bool CMarry::CheckRequiredItem(int aIndex, int iSide)
{
	if(!this->m_bNeedSpecialItemForMarry) return true;

	EnterCriticalSection(&this->m_criti);

	for (std::vector<MARRY_ITEM>::iterator It = this->m_vtMarryItem.begin(); It != this->m_vtMarryItem.end(); It++)
	{
		if (It->m_iSide != iSide)
		{
			continue;
		}

		if (gObjGetItemCountInIventory(aIndex, It->m_wItemID) < It->m_iItemCount)
		{
			LeaveCriticalSection(&this->m_criti);
			return false;
		}
	}

	LeaveCriticalSection(&this->m_criti);
	return true;
}

bool CMarry::CheckDivorceItem(int aIndex)
{
	if(!this->m_bNeedSpecialItemForDivorce) return true;

	EnterCriticalSection(&this->m_criti);

	for (std::vector<MARRY_ITEM>::iterator It = this->m_vtDivorceItem.begin(); It != this->m_vtDivorceItem.end(); It++)
	{
		if (gObjGetItemCountInIventory(aIndex, It->m_wItemID) < It->m_iItemCount)
		{
			LeaveCriticalSection(&this->m_criti);
			return false;
		}
	}

	LeaveCriticalSection(&this->m_criti);
	return true;
}

void CMarry::GiveGiftItem(int aIndex, int iSide)
{
	if (!this->m_bGiftEnable) return;

	EnterCriticalSection(&this->m_criti);

	for (std::vector<MARRY_ITEM>::iterator It = this->m_vtGiftItem.begin(); It != this->m_vtGiftItem.end(); It++)
	{
		if (It->m_iSide != iSide)
		{
			continue;
		}

		for (int i = 0; i < It->m_iItemCount; i++)
		{
			ItemSerialCreateSend(aIndex, 235, 0, 0, It->m_wItemID, 0, 0, 0, 0, 0, aIndex, 0, 0, 0, 0, 0);
		}
	}

	LeaveCriticalSection(&this->m_criti);
}

void CMarry::DeleteRequiredItem(int aIndex, int iSide)
{
	if(!this->m_bNeedSpecialItemForMarry) return;

	EnterCriticalSection(&this->m_criti);

	for (std::vector<MARRY_ITEM>::iterator It = this->m_vtMarryItem.begin(); It != this->m_vtMarryItem.end(); It++)
	{
		if (It->m_iSide != iSide)
		{
			continue;
		}

		for (int i = 0; i < It->m_iItemCount; i++)
		{
			for(int i=0;i<MAIN_INVENTORY_SIZE;i++)
			{
				if(gObj[aIndex].pInventory[i].IsItem())
				{
					if (gObj[aIndex].pInventory[i].m_Type == It->m_wItemID)
					{
						gObjInventoryDeleteItem(aIndex, i);
						GSProtocol.GCInventoryItemDeleteSend(aIndex, i, 1);
						break;
					}
				}	
			}
		}
	}

	LeaveCriticalSection(&this->m_criti);
}

void CMarry::DeleteDivorceItem(int aIndex)
{
	if(!this->m_bNeedSpecialItemForDivorce) return;

	EnterCriticalSection(&this->m_criti);

	for (std::vector<MARRY_ITEM>::iterator It = this->m_vtDivorceItem.begin(); It != this->m_vtDivorceItem.end(); It++)
	{
		for (int i = 0; i < It->m_iItemCount; i++)
		{
			for (int i = 0; i<MAIN_INVENTORY_SIZE; i++)
			{
				if (gObj[aIndex].pInventory[i].IsItem())
				{
					if (gObj[aIndex].pInventory[i].m_Type == It->m_wItemID)
					{
						gObjInventoryDeleteItem(aIndex, i);
						GSProtocol.GCInventoryItemDeleteSend(aIndex, i, 1);
						break;
					}
				}
			}
		}
	}

	LeaveCriticalSection(&this->m_criti);
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

