//////////////////////////////////////////////////////////////////////
// ItemLoad.cpp
#include "stdafx.h"
#include "ItemLoad.h"
//#include "Interface.h"
//#include "Sound.h"
#include "ResourceManager.h"
#include "AntiHack.h"

char ItemString[] = "Item\\";
char PlayerString[] = "Player\\";

/*005F7421  |> 6A 00          PUSH 0                                   ; /Arg3 = 00000000
	Drop sound

	0060DA23   . E8 689CFFFF    CALL main2.00607690                      ; \main2.00607690
effect

007DA842     81FA 0E1C0000  CMP EDX,1C0E
hover highlighting

00836D36   . 3D 0D1C0000    CMP EAX,1C0D
action

*/

/* Tooltip color:
256 - standard
257 - blue
258 - red
259 - gold
260 - green
261 - red bg
262 - archangel
263 - blue bg
264 - orange bg
265 - blue exc (ancient?)
266 - gray
267 - light pink
268 - socket
269 - orange
270 - unk
	*/

int tipCurrId = 0;
void __declspec(naked) ToolTipFix()
{

	__asm {
		MOV EDX,DWORD PTR SS:[EBP-0x10];
		PUSH EDX;
		MOV EAX,DWORD PTR SS:[EBP-0x114];
		PUSH EAX;
		MOV EDX, TOOLTIP_FIX_XOR_BUFF;
		CALL EDX;
		ADD ESP,0x8;

		MOV EAX, DWORD PTR SS:[EBP-0x114];
		XOR EDX, EDX;
		MOV DL, BYTE PTR DS:[EAX+4];
		CMP EDX, 0;
		JE LeaveLoop;

		MOV ECX,DWORD PTR SS:[EBP-0x10];
		PUSH ECX;
		MOV EDX,DWORD PTR SS:[EBP-0x114];
		PUSH EDX;
//		MOV EAX,DWORD PTR SS:[EBP-0x118];
		MOV EAX, tipCurrId;
		IMUL EAX,EAX,0x7C;
		MOV ECX,DWORD PTR SS:[EBP-0x224];
		LEA EDX,DWORD PTR DS:[ECX+EAX+0x4];
		PUSH EDX;
		MOV EDX, TOOLTIP_FIX_ADD_LINE;
		CALL EDX;
		ADD ESP,0xC;

		MOV EAX, tipCurrId;
		ADD EAX, 1;
		MOV tipCurrId, EAX;

LeaveLoop:
		MOV EAX,DWORD PTR SS:[EBP-0x114];
		ADD EAX,DWORD PTR SS:[EBP-0x10];
		MOV DWORD PTR SS:[EBP-0x114],EAX;

		MOV EDX, TOOLTIP_FIX_BACK_JMP;
		JMP EDX;
	}
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

