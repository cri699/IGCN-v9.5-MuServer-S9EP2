//////////////////////////////////////////////////////////////////////
// CustomWings.cpp
#include "stdafx.h"
#include "CustomWings.h"
//00591530   807D C8 07       CMP BYTE PTR SS:[EBP-38],7



void _declspec(naked) AsmHookPreviewCharSet()
{
	_asm
	{
		MOV EAX, DWORD PTR SS:[EBP+0xC];
		MOVZX EAX, BYTE PTR DS:[EAX+0x0F];
		CMP EAX, 8
		JE FixWing8;
		CMP EAX, 9
		JE FixWing9
		CMP EAX, 10
		JE FixWing10
		CMP EAX, 11
		JE FixWing11
		CMP EAX, 12
		JE FixWing12
		CMP EAX, 13
		JE FixWing13
		CMP EAX, 14
		JE FixWing14
		JMP ReturnToOrg;
ReturnToOrg:
		MOV EAX,DWORD PTR SS:[EBP+0xC];
		MOVZX EAX,BYTE PTR DS:[EAX+8]
		MOV EDX, 0x005891E3; // S8 E3
		JMP EDX;
FixWing8:
		MOV EAX,0x1D01
		MOV ECX,DWORD PTR SS:[EBP-0x10]
		MOV WORD PTR DS:[ECX+0x218],AX
		MOV EDX, 0x0058925A // S8 E3
		jmp EDX;
FixWing9:
		MOV EAX,0x1D02
		MOV ECX,DWORD PTR SS:[EBP-0x10]
		MOV WORD PTR DS:[ECX+0x218],AX
		MOV EDX, 0x0058925A // S8 E3
		jmp EDX;
FixWing10:
		MOV EAX,0x1D03
		MOV ECX,DWORD PTR SS:[EBP-0x10]
		MOV WORD PTR DS:[ECX+0x218],AX
		MOV EDX, 0x0058925A // S8 E3
		jmp EDX;

FixWing11:
		MOV EAX,0x1D04
		MOV ECX,DWORD PTR SS:[EBP-0x10]
		MOV WORD PTR DS:[ECX+0x218],AX
		MOV EDX, 0x0058925A // S8 E3
		jmp EDX;
FixWing12:
		MOV EAX,0x1D05
		MOV ECX,DWORD PTR SS:[EBP-0x10]
		MOV WORD PTR DS:[ECX+0x218],AX
		MOV EDX, 0x0058925A // S8 E3
		jmp EDX;
FixWing13:
		MOV EAX,0x1D06
		MOV ECX,DWORD PTR SS:[EBP-0x10]
		MOV WORD PTR DS:[ECX+0x218],AX
		MOV EDX, 0x0058925A // S8 E3
		jmp EDX;
FixWing14:
		MOV EAX,0x1D07
		MOV ECX,DWORD PTR SS:[EBP-0x10]
		MOV WORD PTR DS:[ECX+0x218],AX
		MOV EDX, 0x0058925A // S8 E3
		jmp EDX;

	}
}

void _declspec(naked) FixWingOpts()
{
	_asm
	{
		CMP DWORD PTR SS:[EBP-0x598],0x1D08
		MOV EDX, 0x006056B2; // S8 E3
		JG JumpBackJG;
		CMP DWORD PTR SS:[EBP-0x598],0x1D08
		MOV EDX, 0x006056B2; // S8 E3
		JGE JumpBackJGE;
		CMP DWORD PTR SS:[EBP-0x598],0x1CDE
		JL JumpBackJL1CDE;
		CMP DWORD PTR SS:[EBP-0x598],0x1CE1
		JLE JumpBackJGE;
		CMP DWORD PTR SS:[EBP-0x598],0x1CEB
		JLE JumpBackJL1CDE;
		CMP DWORD PTR SS:[EBP-0x598],0x1CED
		JLE JumpBackJLE1CED;
		CMP DWORD PTR SS:[EBP-0x598],0x1CF1
		JLE JumpBackJL1CDE;
		CMP DWORD PTR SS:[EBP-0x598],0x1D08
		JLE JumpBackFinalCheck;
		MOV EDX, 0x00605AC7 // S8 E3
		JMP EDX;
			//JMP 00608954
JumpBackJL1CDE:
		MOV EDX, 0x00605AC7 // S8 E3
		JMP EDX;
JumpBackJGE:
		MOV EDX, 0x0060592B; // S8 E3
		jmp edx;
JumpBackJG:
		MOV EDX, 0x006056B2; // S8 E3
		jmp edx;
JumpBackJLE1CED:
		mov edx, 0x00605901; // S8 E3
		jmp edx;
JumpBackFinalCheck:
		mov edx, 0x0060590D; // S8 E3
		jmp edx;
	}
}


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

