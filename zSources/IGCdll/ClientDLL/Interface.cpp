//////////////////////////////////////////////////////////////////////
// Interface.cpp
#include "stdafx.h"
#include "Interface.h"
#include "Text.h"
#include "Macros.h"
#include "offsets.h"

	std::string siteUrl;
	std::string signature;

void Render()
{
		siteUrl = "iDev.Games";
	signature = "Server Powered by iDev.Games - DAO VAN TRONG";
		int wsize[2];
	muGetWindowSize(wsize);
	int yPos = 0x1E0-(0xD*0x280/wsize[1]);

	SIZE size;
	//muGetTextSizeXY((char*)siteUrl.c_str(), hTextFont, &size);
	g_Font[ARIAL_14]->GetTextSize((char*)siteUrl.c_str(), size);

	//int mouse[2];
	//muGetMousePos(mouse);

	/*if(mouse[0] > 0x108 && mouse[0] < (0x108+size.cx) && mouse[1] > (yPos-10) && mouse[1] < ((yPos-10)+size.cy))
	{
		//muSetTextFontColor("______________________", 0x108, yPos-10, hTextFont, RGBA(0, 0, 255, 255));
		//muSetTextFontColor((char*)siteUrl.c_str(), 0x108-5, yPos-11, hTitleFont, RGBA(0, 0, 255, 255));
		g_Font[ARIAL_UNDERLINE_14]->DrawColor(0x108, yPos-11, RGBA(0, 0, 255, 255), (char*)siteUrl.c_str());
	}
	else{*/
		//muSetTextFontColor("______________________", 0x108, yPos-10, hTextFont, RGBA(0, 0, 255, 255));
		//muSetTextFontColor((char*)siteUrl.c_str(), 0x108, yPos-11, hTextFont, RGBA(0, 0, 255, 255));
		g_Font[ARIAL_14]->DrawColor(0x108, yPos-11, RGBA(0, 0, 255, 255), (char*)siteUrl.c_str());
	//}
	g_Font[ARIAL_14]->DrawColor(0x100, yPos, RGBA(255, 255, 255, 255), (char*)signature.c_str());
//	muSetText(, 0x200, yPos);
	g_Font[ARIAL_14]->DrawColor(0x200, yPos, RGBA(255, 255, 255, 255), "© 2013 Webzen & IGCN." );
}
void muDrawVersionCB()
{
		if(*g_Map == 0x49 || *g_Map == 0x5E)
		{
			Render();
		}
}
/*
00C6248B   8343 08 FF       ADD DWORD PTR DS:[EBX+8],-1
00C6248F   75 07            JNZ SHORT main.00C62498
00C62491   8BCB             MOV ECX,EBX
00C62493   E8 18CBEBFF      CALL main.00B1EFB0
00C62498   8D4C24 20        LEA ECX,DWORD PTR SS:[ESP+20]
00C6249C   E8 FF25EEFF      CALL main.00B44AA0
00C624A1   5F               POP EDI
00C624A2   5D               POP EBP
*/

//00C6248B
void __declspec(naked) MoveWndFix()
{
	__asm {
		PUSH 4;
		PUSH EBX;
		ADD DWORD PTR SS:[ESP], 8;
		CALL DWORD PTR DS:[IsBadWritePtr];
		CMP EAX, 0;
		JE ValidPtr;
		MOV EDX, 0x00C624A1;
		JMP EDX;
ValidPtr:
		ADD DWORD PTR DS:[EBX+8],-1;
		JNZ lb1;
		MOV EDX, 0x00C62491;
		JMP EDX;
lb1:
		MOV EDX, 0x00C62498;
		JMP EDX;
	}
}

void __declspec(naked) ScaleFormAction(DWORD Form, LPSTR ElementName, LPSTR Command, ...)
{
	__asm
	{
		MOV EDX, 0x0095A6EA; // S9
		JMP EDX;
	}
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

