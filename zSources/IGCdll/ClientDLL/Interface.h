//////////////////////////////////////////////////////////////////////
// Interface.h
#ifndef INTERFACE_H
#define INTERFACE_H

void muDrawVersionCB();
void MoveWndFix();
void ScaleFormAction(DWORD Form, LPSTR ElementName, LPSTR Command, ...);
//004F2C53   90               NOP

//004F2AB6   68 C6010000      PUSH 1C6

#define MU_DRAW_VERSION_FIX1 0x004DEC26 // S8E3
#define MU_DRAW_VERSION_FIX2 0x00506003 // S8E3
#define MU_DRAW_VERSION_FIX3 0x004DEB99 // S8E3
#endif
//004F2BC6   68 907E1A01      PUSH mainIGCT.011A7E90



//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

