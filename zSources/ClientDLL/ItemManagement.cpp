//////////////////////////////////////////////////////////////////////
// ItemManagement.cpp
#include "stdafx.h"
#include "ItemManagement.h"
#include "protocol.h"
#include "HookManager.h"
#include "offsets.h"

CItemManagement g_Items;
SHOPPRICELIST g_Shop[120];
INVPRICELIST g_Inv[237];

int CItemManagement::Value(int itemid,BYTE type)
{
	if(type == 0)
	{
		if(itemid == 0x1C10)
		{
			return 666;
		}
	}
	else if(type == 1)
	{
		if(itemid == 0x1C10)
		{
			return 777;
		}
	}
	return 0;
}
// type 0 = shop, 1- inventory, 2-repair/
int repairValue(int pos)
{
	return 1000;
}
int Value(int itemid, BYTE X, BYTE Y, BYTE type, int equipslot, int Equiped)
{
	int pos = X + Y * 8;
	if(type == 0)
	{
		return g_Shop[pos].SellPrice;
	}
	if(type == 1)
	{
		if(Equiped == 1)
		{
			return g_Inv[equipslot].SellPrice;
		}
		else
		{
			return g_Inv[pos+12].SellPrice;
		}
	}
	if(type == 2) // repair
	{
		repairValue(pos);
	}
	return 2;
}

int getEquiped(DWORD pItem)
{
		DWORD EquipAddr = *(DWORD*)(0x846CA18)+0x1248;
		if((DWORD)pItem > EquipAddr && (DWORD)pItem < EquipAddr+(12*0x7C))
		{
		 return 1;
		}
		else
		{
		 return 0;
		}
}

void __declspec(naked) SetPrice()
{
	DWORD equipped;
	_asm 
	{
		PUSH ESI;
		
		/*PUSH 0; // type

		MOV EAX, DWORD PTR SS:[EBP+8];
		MOV EAX, DWORD PTR DS:[EAX+8];
		SAR EAX,3;
		AND EAX, 0xF;
		PUSH EAX; // Level*/
		

		///MOVZX ECX, BYTE PTR DS:[EAX+0x4D];
		//PUSH ECX;

	//	MOV EAX, pItem;
		//MOV EDX, DWORD PTR DS:[EAX+0xC];
		
		///CMP EDX, 0xFF;
		//JE NotEquip;
		MOV EAX,DWORD PTR SS:[EBP+8];

		PUSH EAX;
		call getEquiped;
		mov equipped, eax;
		MOV EAX,DWORD PTR SS:[EBP+8];
		PUSH equipped;
		PUSH DWORD PTR DS:[EAX+0xC]; // EQUIPPED
		PUSH DWORD PTR SS:[EBP+0xC]; // TYPE
		PUSH BYTE PTR DS: [EAX+0x4E]; // Y
		PUSH BYTE PTR DS: [EAX+0x4D]; // X
		PUSH  DWORD PTR DS:[EAX+4]; // item id
		

		call Value;
		MOV ESP, EBP;
		POP EBP;
		RETN;
	}

}
//0059E68B   . 56             PUSH ESI

void ParsePricePacket(BYTE * aRecv)
{
	PMSG_PRICE_INFO * packet = (PMSG_PRICE_INFO*)&aRecv[0];

	int count = packet->count;
	int type = packet->Type;

	int lOfs = sizeof(PMSG_PRICE_INFO);
	if(type == 0)
	{
		for(int i=0;i<count;i++)
		{
			PMSG_PRICE_ITEM_LIST *pList = (PMSG_PRICE_ITEM_LIST*)&aRecv[lOfs];
			g_Shop[pList->Pos].SellPrice = pList->BuyPrice;
			lOfs+= sizeof(PMSG_PRICE_ITEM_LIST);
		}
	}
	if(type == 1)
	{
		for(int i=0;i<count;i++)
		{
			PMSG_PRICE_ITEM_LIST *pList = (PMSG_PRICE_ITEM_LIST*)&aRecv[lOfs];
			g_Inv[pList->Pos].SellPrice = pList->SellPrice;
			g_Inv[pList->Pos].BuyPrice = pList->BuyPrice;
			lOfs+= sizeof(PMSG_PRICE_ITEM_LIST);
		}
	}

}

void ParsePriceCMPacket(BYTE * aRecv)
{
	PMSG_PRICE_INFO * packet = (PMSG_PRICE_INFO*)&aRecv[0];

	int count = packet->count;
	int type = packet->Type;

	int lOfs = sizeof(PMSG_PRICE_INFO);
	if(type == 1)
	{
		for(int i=0;i<count;i++)
		{
			PMSG_PRICE_ITEM_LIST *pList = (PMSG_PRICE_ITEM_LIST*)&aRecv[lOfs];
			//'/g_Inv[pList->Pos].SellPrice = pList->SellPrice;
			//g_Inv[pList->Pos].BuyPrice = pList->BuyPrice;
			g_Shop[pList->Pos].SellPrice = pList->BuyPrice;
			lOfs+= sizeof(PMSG_PRICE_ITEM_LIST);
		}
	}

}

void _declspec(naked) AllowLuckyMoveToWH()
{
	//005C52D3   83F8 04          CMP EAX,4 // banka
	//005C52D6   74 78            JE SHORT mainIGC.005C5350
	
	//009B4ED4   E9 8F000000      JMP mainIGC.009B4F68 // personal store
	//009B4ED9   90               NOP

}

void _declspec(naked) LuckyItemToolTipFix()
{
	_asm
	{
		CMP DWORD PTR DS:[eax+0x4],0x1CA0;
		JNZ CheckLuckyBless;
		MOV EDX,0x00A135AD;
		JMP EDX;
CheckLuckyBless:
		CMP DWORD PTR DS:[eax+0x4],0x1C0D;
		JNZ CheckLuckySoul;
		MOV EDX,0x00A135AD;
		JMP EDX;
CheckLuckySoul:
		CMP DWORD PTR DS:[eax+0x4],0x1C0E;
		JNZ ItemInvalid;
		MOV EDX,0x00A135AD;
		JMP EDX;
ItemInvalid:
		MOV EDX,0x00A135BC;
		JMP EDX;
	}
}

void _declspec(naked) LuckyItemEnhancementFix()
{
	_asm
	{
		CMP DWORD PTR DS:[eax+0x4],0x1CA0;
		JNZ CheckLuckyBless;
		MOV EDX,0x00A152A1;
		JMP EDX;
CheckLuckyBless:
		CMP DWORD PTR DS:[eax+0x4],0x1C0D;
		JNZ CheckLuckySoul;
		MOV EDX,0x00A152A1;
		JMP EDX;
CheckLuckySoul:
		CMP DWORD PTR DS:[eax+0x4],0x1C0E;
		JNZ ItemInvalid;
		MOV EDX,0x00A152A1;
		JMP EDX;
ItemInvalid:
		MOV EDX,0x00A152B3;
		JMP EDX;
	}
}

int ErrtelMixStaffCheck(DWORD pItem)
{
  signed int i;

  for ( i = 0; i < *(BYTE *)(pItem + 46); ++i )
  {
    if ( *(WORD *)(pItem + 2 * i + 48) == 1 ) // check Staffs here, 1
      return *(BYTE *)(i + pItem + 64);
  }
  return 0;
}

void __declspec(naked) ErrtelMixStaffFix() // 0x009E3824
{
	__asm
	{
		LEA ECX, DWORD PTR SS:[EBP-0x38]
		MOV EDX, 0x007FF2D5;
		CALL EDX;
		PUSH DWORD PTR DS:[EAX];
		CALL ErrtelMixStaffCheck;
		CMP EAX, 4;
		JGE Allow;

		LEA ECX, DWORD PTR SS:[EBP-0x38]
		MOV EDX, 0x007FF2D5;
		CALL EDX;

		MOV EDX, 0x009E382C;
		JMP EDX;

Allow:
		MOV EDX, 0x009E3838;
		JMP EDX;
	}
}

void __declspec(naked) ItemLevelReqFix() // 0x009486F9
{
	__asm
	{
		ADD EAX, 0x14;
		CMP EAX, 0x190;
		JG FixLevel;
		JMP End;

FixLevel:
		MOV EAX, 0x190;
		JMP End;

End:
		MOV ECX, DWORD PTR SS : [EBP - 0x28];
		MOV EDX, 0x009486FF;
		JMP EDX;
	}
}

/* 00800FC6   E8 6F5DC3FF      CALL main.00436D3A
00800FCB   50               PUSH EAX
00800FCC   8B45 88          MOV EAX,DWORD PTR SS:[EBP-78]
00800FCF   6BC0 64          IMUL EAX,EAX,64 */

char * pShopPriceTag = "Zen: %s";
void __declspec(naked) AlterPShopDisplayCurrency()
{
	_asm
	{
		PUSH pShopPriceTag;
		MOV EAX, DWORD PTR SS:[EBP-0x78];
		IMUL EAX,EAX, 0x64;
		MOV EDX, 0x00800FD2;
		JMP EDX;
		//00800FD2   05 882D5B08      ADD EAX,main.085B2D88                    ; ASCII "Selling Price"

	}
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

