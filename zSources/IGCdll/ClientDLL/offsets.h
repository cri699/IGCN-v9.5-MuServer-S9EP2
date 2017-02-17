//////////////////////////////////////////////////////////////////////
// offsets.h
#define MU_MAP_ADDR 0x1206ED0 // S9
#define LOAD_ITEMS_CALLBACK 0x006185BE // S9

#define VERSION_HOOK1 0x006E86A8+3 // S9
#define VERSION_HOOK2 0x004FAD4F+3 // S9
#define VERSION_HOOK3 0x09ED4BAF+3 // S9
#define SERIAL_HOOK1 0x006E86EB+3 // S9
#define SERIAL_HOOK2 0x004FAD92+3 // S9

#define CHARSET_ADDRESS 0x1205B4C // S9
#define XSHOP_FIX 0x005C43E4 // S9

#define MU_WINDOW_SIZE 0x1206F10 // S9
#define MU_DRAW_VERSION 0x004DF445 // S9

extern DWORD* g_Map;

#define MAKE_NUMBERW(x,y)  ( (WORD)(((BYTE)((y)&0xFF)) |   ((BYTE)((x)&0xFF)<<8 ))  )

#define MU_GET_CHARR_POS1 0x00969F14 // S9
#define MU_GET_CHARR_POS2 0x00829DDE // S9
#define MU_GET_CHARR_POS3 0x00840443 // S9
#define SETGLOW_EX 0x005F5016 // S9
#define SETGLOW_BACKJMP_EX702 0x005F501C // S9

#define PROTOCOL_SOCKET_ADDR 0x8B97A08+0xC // S9

#define CTRL_FREEZE_FIX 0x0053125F+1 // S9
#define MAPSRV_DELACCID_FIX 0x004FAC1D // S9

#define JEWEL_OF_BLESS_PRICE 0x59C295+3 // S9
#define JEWEL_OF_SOUL_PRICE 0x59C2B1+3  // S9
#define JEWEL_OF_CHAOS_PRICE 0x59C2CD+3 // S9
#define JEWEL_OF_LIFE_PRICE 0x59C2E9+3 // S9
#define JEWEL_OF_CREATION_PRICE 0x59C305+3 // S9
#define CREST_OF_MONARCH_PRICE 0x0059C409+3 // S9
#define LOCH_FEATHER_PRICE 0x0059C3FC+3 // S9
#define JEWEL_OF_GUARDIAN_PRICE 0x0059CBE7+3 // S9
#define WERERABBIT_EGG_PRICE 0x0059DF54+3 // S9
#define ITEM_SELL_DIVISOR 0x0059DA9F+1 // S9

//123CA8C

#define pGameWindow				*(HWND*)(0x123CA8C)

//00958F37   55               PUSH EBP

#define pGetPreviewStruct		((DWORD(__thiscall*)(LPVOID This, int ViewportID)) 0x958F37)
#define pPreviewThis			((LPVOID(*)()) 0x437357)

// UI
#define pMessageBoxThis			((DWORD(__cdecl*)()) 0x00969F14) // S9

enum ObjState
{
	SelectServer	= 2,
	SwitchCharacter = 4,
	GameProcess		= 5,
};

#define pPlayerState			*(int*)0x1205338 // s9e2

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

