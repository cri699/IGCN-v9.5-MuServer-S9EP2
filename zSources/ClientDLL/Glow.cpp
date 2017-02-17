//////////////////////////////////////////////////////////////////////
// Glow.cpp
#include "stdafx.h"
#include "Glow.h"
#include "Macros.h"
#include "ItemLoad.h"
#include "offsets.h"
#include "Exception.h"

CGlow* g_Glow = new CGlow;

void CGlow::Load(std::string file)
{
	FILE* hFile = fopen(file.c_str(), "rb");
	if(!hFile)
	{
		MessageBoxA(0,"Glow file does not exist", "ERROR",0);
	}

	fread(m_Glow, 1, sizeof(ITEM_GLOW)*512*15, hFile);
	fclose(hFile);
}

void __declspec(naked) OnSetGlowEx( DWORD ItemID, DWORD unk1, float GlowPower, float* OutColor )
{
	__asm {
		PUSH EBP;
		MOV EBP,ESP;
		SUB ESP,0x14;
		PUSH OutColor;
		PUSH ItemID;
		MOV ECX, g_Glow;
		CALL CGlow::SetGlow;
		CMP AL, 1;
		JE LeaveFunc;
		MOV EDX, SETGLOW_BACKJMP_EX702;
		JMP EDX;
LeaveFunc:
		LEAVE;
		RET;
	}
	g_Glow->SetGlow(ItemID, OutColor);
}

#pragma optimize("g", on)
bool CGlow::SetGlow(DWORD ItemID, float* color)
{
/*	if(g_CustomMonsters->OnSetGlowColor(ItemID, color))
		return true;*/
/*	if(g_Addons->IsActive(ADDON_SILVER))
		if(g_CustomMonsters->OnSetGlowColor(ItemID, color))
			return true;*/
	ItemID -= ITEM_BASE;
	if(ItemID > (15*512))
		return false;
	else if(m_Glow[ItemID].bUsed)
	{
		//color[0] = (sin((float)GetTickCount()/1200.0f)/2.0f)+0.5f;
		//color[1] = (sin((float)GetTickCount()/1000.0f)/2.0f)+0.5f;
		//color[2] = (cos((float)GetTickCount()/1400.0f)/2.0f)+0.5f;
		color[0] = m_Glow[ItemID].color[0];
		color[1] = m_Glow[ItemID].color[1];
		color[2] = m_Glow[ItemID].color[2];
		return true;
	}
	return false;
}

CGlow::CGlow()
{
	memset(m_Glow, 0, sizeof(m_Glow));
}

#pragma optimize("g", off)

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

