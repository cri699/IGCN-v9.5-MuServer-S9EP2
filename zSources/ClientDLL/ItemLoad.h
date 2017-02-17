//////////////////////////////////////////////////////////////////////
// ItemLoad.h
#ifndef ITEMS_H
#define ITEMS_H

#define LOAD_ITEMS_CALLBACK 0x006185BE // S9

#define TOOLTIP_FIX 0x00800158 // S9
#define TOOLTIP_FIX_XOR_BUFF 0x0043B6D6  // S9
#define TOOLTIP_FIX_ADD_LINE 0x00D09CA0 // S9
#define TOOLTIP_FIX_BACK_JMP 0x008001A0 // S9

#define ITEM_BASE 0x4F8 // S9

void ToolTipFix();

void LoadItem(int id, const char* path, const char* item, int num);
void LoadItemTex(int id);
void LoadItemCustom(int id, const char* path, const char* item, int num, const char* folder);

void LoadCustomJewels();

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

