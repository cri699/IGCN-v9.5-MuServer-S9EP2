//////////////////////////////////////////////////////////////////////
// GensExt.h
#ifndef GENS_EXT_H
#define GENS_EXT_H

extern WORD iBattleZoneMapList[103]; // MAX_NUMBER_MAP
extern int iBattleZoneWarpList[60]; // MAX_MOVE_COMMAND

extern int iBattleZoneWarpCount;

struct _tagPMSG_SEND_GENS_BATTLEZONE_DATA
{
	BYTE c;
	BYTE sizeH;
	BYTE sizeL;
	BYTE headcode;
	BYTE subcode;
	WORD iMapList[103];
	int iWarpList[60];
};

void GensInitData();
void GensWarpMenuFix();
bool GensIsBattleZoneMap(int iMapNumber);
void GCSetBattleZoneData(_tagPMSG_SEND_GENS_BATTLEZONE_DATA * lpMsg);

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

