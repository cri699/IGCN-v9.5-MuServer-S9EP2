//////////////////////////////////////////////////////////////////////
// EventEntryLevel.cpp
#include "stdafx.h"
#include "EventEntryLevel.h"
#include "HookManager.h"
// warning
// it's private custom
// don't add it to public release

void LoadBCEntryLevel()
{
	char szTemp[128];

	FILE * hFile = fopen("./Data/Local/EventEntry.bmd", "r"); // check if file exists

	if (hFile == NULL)
	{
		return;
	}

	fclose(hFile);

	for (int i = 0; i < 7; i++)
	{
		sprintf(szTemp, "BC%dLevelMin_Normal", i + 1);
		g_BloodCastleEnterLevel_Normal[i][0] = GetPrivateProfileInt("BloodCastle", szTemp, 0, "./Data/Local/EventEntry.bmd");

		sprintf(szTemp, "BC%dLevelMax_Normal", i + 1);
		g_BloodCastleEnterLevel_Normal[i][1] = GetPrivateProfileInt("BloodCastle", szTemp, 0, "./Data/Local/EventEntry.bmd");

		sprintf(szTemp, "BC%dLevelMin_MGDLRF", i + 1);
		g_BloodCastleEnterLevel_Magumsa[i][0] = GetPrivateProfileInt("BloodCastle", szTemp, 0, "./Data/Local/EventEntry.bmd");

		sprintf(szTemp, "BC%dLevelMax_MGDLRF", i + 1);
		g_BloodCastleEnterLevel_Magumsa[i][1] = GetPrivateProfileInt("BloodCastle", szTemp, 0, "./Data/Local/EventEntry.bmd");
	}

	HookManager.MakeJmpHook(0x009F45E3, HookBCLevel);
}

void LoadDSEntryLevel()
{
	char szTemp[128];

	FILE * hFile = fopen("./Data/Local/EventEntry.bmd", "r"); // check if file exists

	if (hFile == NULL)
	{
		return;
	}

	fclose(hFile);

	for (int i = 0; i < 6; i++)
	{
		sprintf(szTemp, "DS%dLevelMin_Normal", i + 1);
		g_DevilSquareEnterLevel_Normal[i][0] = GetPrivateProfileInt("DevilSquare", szTemp, 0, "./Data/Local/EventEntry.bmd");

		sprintf(szTemp, "DS%dLevelMax_Normal", i + 1);
		g_DevilSquareEnterLevel_Normal[i][1] = GetPrivateProfileInt("DevilSquare", szTemp, 0, "./Data/Local/EventEntry.bmd");

		sprintf(szTemp, "DS%dLevelMin_MGDLRF", i + 1);
		g_DevilSquareEnterLevel_Magumsa[i][0] = GetPrivateProfileInt("DevilSquare", szTemp, 0, "./Data/Local/EventEntry.bmd");

		sprintf(szTemp, "DS%dLevelMax_MGDLRF", i + 1);
		g_DevilSquareEnterLevel_Magumsa[i][1] = GetPrivateProfileInt("DevilSquare", szTemp, 0, "./Data/Local/EventEntry.bmd");
	}

	ModifyDSLevel();
}

void __declspec(naked) HookBCLevel()
{
	BC_LEVEL_INFO_MAIN m_BCInfo;

	for (int i = 0; i < 7; i++)
	{
		m_BCInfo.iBloodCastleLevel = i + 1;
		m_BCInfo.iMinLevel = g_BloodCastleEnterLevel_Normal[i][0];
		m_BCInfo.iMaxLevel = g_BloodCastleEnterLevel_Normal[i][1];
		m_BCInfo.bIsMagumsa = false;

		__asm
		{
			LEA EAX, m_BCInfo;
			PUSH EAX;
			MOV ECX, DWORD PTR SS : [EBP - 0x74];
			ADD ECX, 0x74;
			MOV EDX, 0x009F4EEB;
			CALL EDX;
		}
	}

	for (int i = 0; i < 7; i++)
	{
		m_BCInfo.iBloodCastleLevel = i + 1;
		m_BCInfo.iMinLevel = g_BloodCastleEnterLevel_Magumsa[i][0];
		m_BCInfo.iMaxLevel = g_BloodCastleEnterLevel_Magumsa[i][1];
		m_BCInfo.bIsMagumsa = true;

		__asm
		{
			LEA EAX, m_BCInfo;
			PUSH EAX;
			MOV ECX, DWORD PTR SS : [EBP - 0x74];
			ADD ECX, 0x74;
			MOV EDX, 0x009F4EEB;
			CALL EDX;
		}
	}

	__asm
	{
		MOV EDX, 0x009F46C1;
		JMP EDX;
	}
}

void ModifyDSLevel()
{
	DWORD OldProtect;
	VirtualProtect((LPVOID)(0x00838418), 319, PAGE_EXECUTE_READWRITE, &OldProtect);

	*(DWORD*)(0x00838412 + 6) = (DWORD)g_DevilSquareEnterLevel_Normal[0][0];
	*(DWORD*)(0x0083841F + 6) = (DWORD)g_DevilSquareEnterLevel_Normal[0][1];
	*(DWORD*)(0x0083842C + 6) = (DWORD)g_DevilSquareEnterLevel_Normal[1][0];
	*(DWORD*)(0x00838439 + 6) = (DWORD)g_DevilSquareEnterLevel_Normal[1][1];
	*(DWORD*)(0x00838446 + 6) = (DWORD)g_DevilSquareEnterLevel_Normal[2][0];
	*(DWORD*)(0x00838453 + 6) = (DWORD)g_DevilSquareEnterLevel_Normal[2][1];
	*(DWORD*)(0x00838460 + 6) = (DWORD)g_DevilSquareEnterLevel_Normal[3][0];
	*(DWORD*)(0x0083846D + 6) = (DWORD)g_DevilSquareEnterLevel_Normal[3][1];
	*(DWORD*)(0x0083847A + 6) = (DWORD)g_DevilSquareEnterLevel_Normal[4][0];
	*(DWORD*)(0x00838487 + 6) = (DWORD)g_DevilSquareEnterLevel_Normal[4][1];
	*(DWORD*)(0x00838494 + 6) = (DWORD)g_DevilSquareEnterLevel_Normal[5][0];
	*(DWORD*)(0x008384A1 + 6) = (DWORD)g_DevilSquareEnterLevel_Normal[5][1];

	*(DWORD*)(0x008384C2 + 6) = (DWORD)g_DevilSquareEnterLevel_Magumsa[0][0];
	*(DWORD*)(0x008384CF + 6) = (DWORD)g_DevilSquareEnterLevel_Magumsa[0][1];
	*(DWORD*)(0x008384DC + 6) = (DWORD)g_DevilSquareEnterLevel_Magumsa[1][0];
	*(DWORD*)(0x008384E9 + 6) = (DWORD)g_DevilSquareEnterLevel_Magumsa[1][1];
	*(DWORD*)(0x008384F6 + 6) = (DWORD)g_DevilSquareEnterLevel_Magumsa[2][0];
	*(DWORD*)(0x00838503 + 6) = (DWORD)g_DevilSquareEnterLevel_Magumsa[2][1];
	*(DWORD*)(0x00838510 + 6) = (DWORD)g_DevilSquareEnterLevel_Magumsa[3][0];
	*(DWORD*)(0x0083851D + 6) = (DWORD)g_DevilSquareEnterLevel_Magumsa[3][1];
	*(DWORD*)(0x0083852A + 6) = (DWORD)g_DevilSquareEnterLevel_Magumsa[4][0];
	*(DWORD*)(0x00838537 + 6) = (DWORD)g_DevilSquareEnterLevel_Magumsa[4][1];
	*(DWORD*)(0x00838544 + 6) = (DWORD)g_DevilSquareEnterLevel_Magumsa[5][0];
	*(DWORD*)(0x00838551 + 6) = (DWORD)g_DevilSquareEnterLevel_Magumsa[5][1];
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

