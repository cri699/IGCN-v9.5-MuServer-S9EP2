//////////////////////////////////////////////////////////////////////
// DamageExtension.cpp
#include "stdafx.h"
#include "DamageExtension.h"
#include "protocol.h"
// global variables
int dmgDisplay = 0;
int manaDisplay = 0;
int maxmanaDisplay = 0;
int hpDisplay = 0;
int maxHPDisplay = 0;

// start of functions
void __declspec(naked) dmgSendHook()
{
	_asm {
		push ecx
		fldz
		fstp dword ptr ss:[esp]
		push 0
		push 1
		push ecx
		FLD DWORD PTR SS:[EBP-0x48]
		FSTP DWORD PTR SS:[ESP]
		LEA EAX,DWORD PTR SS:[EBP-0xC]
		PUSH EAX
		push dmgDisplay; 
		MOV EAX,DWORD PTR SS:[EBP-0x34]
		ADD EAX,0x114 // S9
		PUSH EAX
		mov edx, 0x007B8440 // S9
		call edx
		mov edx, 0x00645F68 // S9
		jmp edx;
	}
}

void __declspec(naked) dmgSendRFHook()
{
	_asm {
		PUSH DWORD PTR SS : [EBP - 0x11];
		PUSH DWORD PTR SS : [EBP - 0x12];
		PUSH DWORD PTR SS : [EBP - 0x25];
		PUSH DWORD PTR SS : [EBP - 0x18];
		XOR EAX, EAX;
		MOV EAX, dmgDisplay;
		PUSH EAX;
		mov edx, 0x00645B74; // S9
		jmp edx;
	}
}

void RFSkillDamageDisplayFix()
{
	DWORD OldProtect;
	VirtualProtect((LPVOID)0x00524297, 0x381, PAGE_EXECUTE_READWRITE, &OldProtect); // S9

	BYTE Fix1[9] = { 0x8B, 0x44, 0x08, 0x04, 0x90, 0x89, 0x45, 0xE4, 0x90 };
	memcpy((LPVOID)0x00524297, Fix1, 9); // S9

	BYTE Fix2[5] = { 0xFF, 0x75, 0xE4, 0x90, 0x90 };
	memcpy((LPVOID)0x00524476, Fix2, 5); // S9
	memcpy((LPVOID)0x005244D8, Fix2, 5); // S9
	memcpy((LPVOID)0x0052453A, Fix2, 5); // S9
	memcpy((LPVOID)0x005245B1, Fix2, 5); // S9
	memcpy((LPVOID)0x005245EF, Fix2, 5); // S9
	memcpy((LPVOID)0x00524617, Fix2, 5); // S9
}

char SetMPString[] = "SetMP";
char SetMPValString[] = "%d %d";
char SetHPString[] = "SetHP";

void __declspec(naked) manaSendHook()
{
	_asm {
		MOVZX EAX,WORD PTR SS:[EBP-0x10]
		MOV ECX,DWORD PTR SS:[EBP-0x18]
		MOV DWORD PTR DS:[ECX+0x94],EAX
		MOVZX EAX,WORD PTR SS:[EBP-0xC]
		MOV ECX,DWORD PTR SS:[EBP-0x18]
		MOV DWORD PTR DS:[ECX+0x98],EAX
		MOVZX EAX,WORD PTR SS:[EBP-0xC]
		PUSH maxmanaDisplay						// max mana
		MOVZX EAX,WORD PTR SS:[EBP-0x10]
		PUSH manaDisplay						// current mana
		PUSH OFFSET SetMPValString				// ; ASCII "%d %d"
		PUSH OFFSET SetMPString					// ; ASCII "SetMP"
		PUSH DWORD PTR SS:[EBP-0x18]
		mov edx, 0x0095A6EA // S9
		call edx;
		ADD ESP,14
		LEAVE
		RETN
	}
}

void __declspec(naked) HPSendHook()
{
	_asm {
		MOVZX EAX,WORD PTR SS:[EBP-0x14]
		MOV ECX,DWORD PTR SS:[EBP-0x18]
		MOV DWORD PTR DS:[ECX+0x88],EAX
		MOVZX EAX,WORD PTR SS:[EBP-0x8]
		MOV ECX,DWORD PTR SS:[EBP-0x18]
		MOV DWORD PTR DS:[ECX+0x8C],EAX
		MOVZX EAX,WORD PTR SS:[EBP-0x8]
		PUSH maxHPDisplay
		MOVZX EAX,WORD PTR SS:[EBP-0x14]
		PUSH hpDisplay
		PUSH OFFSET SetMPValString                
		PUSH OFFSET SetHPString               
		PUSH DWORD PTR SS:[EBP-0x18]
		MOV EDX, 0x0095A6EA // S9
		CALL EDX
		MOV EDX, 0x009CB1BC // S9
		jmp edx;
	}
}

void __declspec(naked) ManaSendCHook()
{
	_asm {
		MOV EAX, DWORD PTR DS : [0x851ACC4] // S9
		MOVZX EAX,WORD PTR DS:[EAX+0x128] // NEW S9
		PUSH maxmanaDisplay
		MOV EAX, DWORD PTR DS : [0x851ACC4] // S9
		MOVZX EAX,WORD PTR DS:[EAX+0x124] // NEW S9
		PUSH manaDisplay
		PUSH 0x0D5
		MOV ECX,0x0851ACD8 // S9
		mov edx,0x00436D3A // S9
		call edx
		mov edx, 0x009AB2B0 // S9
		jmp edx
	}
}

void __declspec(naked) ManaSendDKHook()
{
	_asm 
	{
		PUSH maxmanaDisplay
		PUSH manaDisplay
		PUSH 0x0D5
		MOV ECX, 0x0851ACD8 // S9
		mov edx, 0x00436D3A // S9
		call edx
		mov edx, 0x009AB275 // S9
		jmp edx
	}
}

void __declspec(naked) HPSendDKHook()
{
	_asm {
		MOVZX EAX, WORD PTR DS : [0x8B97870] // S9
		PUSH maxHPDisplay;
		MOV EAX, DWORD PTR DS : [0x851ACC4] // S9
		MOVZX EAX,WORD PTR DS:[EAX+0x122] // NEW S9
		PUSH hpDisplay
		PUSH 0x0D3
		MOV ECX, 0x0851ACD8 // S9
		mov edx, 0x00436D3A // S9
		call edx
		mov edx,0x009AAF0A // S9
		jmp edx
	}
}

void __declspec(naked) HPSendCHook()
{
	_asm {
		PUSH maxHPDisplay
		PUSH hpDisplay
		PUSH 0x0D3
		MOV ECX, 0x0851ACD8 // S9
		mov edx, 0x00436D3A // S9
		call edx
		mov edx, 0x009AAF45 // S9
		jmp edx
	}
}

void __declspec(naked) FixSkill65k()
{
	//00917321   3B45 DC          CMP EAX,DWORD PTR SS:[EBP-24]
	//00917324   0F8D A3010000    JGE main.009174CD
	//0091732A   E8 42AC0300      CALL main.00951F71

	_asm
	{
		MOV EDX, DWORD PTR SS:[EBP-0x24]
		cmp manaDisplay, EDX;
		JGE success;

		mov EDX,0x0092DA7F; // S9
		jmp EDX;
success:
		mov edx, 0x0092DC22; // S9
		jmp edx;

	}

}

/*00800FC6   E8 6F5DC3FF      CALL main.00436D3A
00800FCB   50               PUSH EAX
00800FCC   8B45 88          MOV EAX,DWORD PTR SS:[EBP-78]
00800FCF   6BC0 64          IMUL EAX,EAX,64
*/


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

