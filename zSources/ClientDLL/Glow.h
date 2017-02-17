//////////////////////////////////////////////////////////////////////
// Glow.h
#ifndef GLOW_H
#define GLOW_H

#include "ResourceLoader.h"

struct ITEM_GLOW{
	WORD Id;
	bool bUsed;
	float color[3];
};

class CGlow : public CResourceLoader{
	ITEM_GLOW m_Glow[512*15];
public:
	CGlow();
	void Load(std::string file);
	std::string Type(){return "ItemGlow";}
	bool SetGlow(DWORD ItemID, float* color);
};

void OnSetGlow();
void OnSetGlowEx(DWORD ItemID, DWORD unk1, float GlowPower, float* OutColor);

extern CGlow* g_Glow;

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

