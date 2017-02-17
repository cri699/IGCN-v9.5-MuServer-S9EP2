//////////////////////////////////////////////////////////////////////
// GensExt.cpp
#include "stdafx.h"
#include "GensExt.h"

WORD iBattleZoneMapList[103];
int iBattleZoneWarpList[60];
int iBattleZoneWarpCount;

void GensInitData()
{
	memset(iBattleZoneMapList, 0, sizeof(iBattleZoneMapList));
	memset(iBattleZoneWarpList, -1, sizeof(iBattleZoneWarpList));

	iBattleZoneWarpCount = 0;
}

void __declspec(naked) GensWarpMenuFix()
{
	__asm
	{
		MOV EDX, iBattleZoneWarpCount;
		CMP DWORD PTR SS:[EBP-0x38],EDX
		JGE endloop
		LEA ECX,DWORD PTR SS:[EBP-0x08]
		MOV EDX, 0x0052C329
		CALL EDX
		MOV EAX, DWORD PTR DS:[EAX]
		MOV ECX, DWORD PTR SS:[EBP-0x38]
		MOV EAX, DWORD PTR DS:[EAX]
		MOV EDX, DWORD PTR SS:[EBP-0x38]
		MOV EBX, iBattleZoneWarpList[EDX*4]
		CMP EAX, EBX
		JNZ skip
		LEA ECX, DWORD PTR SS:[EBP-0x08]
		MOV EDX, 0x0052C329
		CALL EDX
		MOV EAX, DWORD PTR DS:[EAX]
		MOV BYTE PTR DS:[EAX+0x55],1
		MOV EDX, 0x009DCCF1
		JMP EDX

skip:
		MOV EDX, 0x009DCCEF
		JMP EDX

endloop:
		MOV EDX, 0x009DCCF1
		JMP EDX
	}
}

bool GensIsBattleZoneMap(int iMapNumber)
{
	return iBattleZoneMapList[iMapNumber];
}

void GCSetBattleZoneData(_tagPMSG_SEND_GENS_BATTLEZONE_DATA * lpMsg)
{
	memcpy(iBattleZoneMapList, lpMsg->iMapList, sizeof(lpMsg->iMapList));
	memcpy(iBattleZoneWarpList, lpMsg->iWarpList, sizeof(lpMsg->iWarpList));

	iBattleZoneWarpCount = 0;

	for (int i = 0; i < 60; i++)
	{
		if (iBattleZoneWarpList[i] != -1)
		{
			iBattleZoneWarpCount++;
		}
	}
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

