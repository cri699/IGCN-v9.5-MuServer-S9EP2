//////////////////////////////////////////////////////////////////////
// ResourceManager.cpp
#include "stdafx.h"
#include "ResourceManager.h"
#include "Exception.h"
#include "ServerInfo.h"
#include "Text.h"
#include "Glow.h"
/*
#include "Items.h"
#include "Interface.h"
#include "HPBar.h"
#include "ProcessScan.h"
#include "IgcDll.h"
#include "Text.h"
#include "Textures.h"
#include "GuiCore.h"
#include "GuiWindow.h"
#include "AntiSpeed.h"*/

CResourceManager ResourceManager;

#pragma optimize(on, "gpty")

void __fastcall CResourceManager::XorLine(char* buff, int len)
{
	BYTE xorTable[] = {0xA1, 0x11, 0x8B, 0x0F, 0x28, 0x17, 0x95, 0xEE, 0x1B, 0xCD, 0x58, 0x31, 0x0E, 0x4D, 0x7E, 0x41, 0x21, 0xFD, 0xFE, 0x7C, 0xC1, 0xC2, 0xAF, 0x3C, 0xFA, 0x14};
	for(int i = 0; i < len; i++)
	{
		buff[i]^=xorTable[i%5];
	}
}

void __fastcall CResourceManager::DumpList( std::string file )
{
	FILE* hFile = fopen(file.c_str(), "wt");
	if(!hFile)
		return;

	SYSTEMTIME time;
	GetLocalTime(&time);

	char line[1024];
	int lineLen;

	memset(line, 0, 1024);
	sprintf(line, "%d-%d-%d %d:%d Resource Manager Dump\r\n\r\n", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute);
	lineLen = strlen(line);
	XorLine(line, lineLen);
	fwrite(&lineLen, 1, 2, hFile);
	fwrite(line, 1, lineLen, hFile);
	
	memset(line, 0, 1024);
	sprintf(line, "Loaded resources:\r\n");
	lineLen = strlen(line);
	XorLine(line, lineLen);
	fwrite(&lineLen, 1, 2, hFile);
	fwrite(line, 1, lineLen, hFile);

	std::list<RESOURCE_STRUCT>::iterator it;
	for(it = m_List.begin(); it != m_List.end(); it++)
	{
		memset(line, 0, 1024);
		sprintf(line, "- Loader: %s;\t0x%04X\t%s\r\n", (*it).m_Type.c_str(), (*it).m_Id, (*it).m_File.c_str());
		lineLen = strlen(line);
		XorLine(line, lineLen);
		fwrite(&lineLen, 1, 2, hFile);
		fwrite(line, 1, lineLen, hFile);
	}
	fclose(hFile);
}

void __fastcall CResourceManager::RegisterResource(std::string type, std::string file, DWORD id)
{
	RESOURCE_STRUCT res;
	res.m_Id = id;
	res.m_Type = type;
	res.m_File = file;
	m_List.push_back(res);
}

void __fastcall CResourceManager::LoadResource( CResourceLoader& loader, std::string file )
{
//MUTATE_START
	try{
		RegisterResource(loader.Type(), file, -1);
		loader.Load(file);
	}
	catch(CException& e)
	{
		//CErrorLog::OutException(e.what());
		//WriteDump();
		MessageBoxA(NULL, e.what(), "ResourceManager", MB_ICONINFORMATION|MB_OK);
		ExitProcess(0);
	}
//MUTATE_END
}

void __fastcall CResourceManager::LoadResource( CResourceLoader& loader, DWORD Id, std::string file )
{
	try{
		RegisterResource(loader.Type(), file, Id);
		loader.Load(Id, file);
	}
	catch(CException& e)
	{
		//CErrorLog::OutException(e.what());
		//WriteDump();
		MessageBoxA(NULL, e.what(), "ResourceManager", MB_ICONINFORMATION|MB_OK);
		ExitProcess(0);
	}
}

void __fastcall CResourceManager::LoadResource( CResourceLoader& loader, int Id, std::string path, std::string file, int num )
{
	try{
		RegisterResource(loader.Type(), path+file, Id);
		loader.Load(Id, path, file, num);
	}
	catch(CException& e)
	{
		//CErrorLog::OutException(e.what());
		//WriteDump();
		MessageBoxA(NULL, e.what(), "ResourceManager", MB_ICONINFORMATION|MB_OK);
		ExitProcess(0);
	}
}

void __fastcall CResourceManager::LoadResource( CResourceLoader& loader, int Id, std::string path, std::string file )
{
	try{
		RegisterResource(loader.Type(), path+file, Id);
		loader.Load(Id, path, file);
	}
	catch(CException& e)
	{
		//CErrorLog::OutException(e.what());
		//WriteDump();
		MessageBoxA(NULL, e.what(), "ResourceManager", MB_ICONINFORMATION|MB_OK);
		ExitProcess(0);
	}
}

void __fastcall CResourceManager::LoadResource( CResourceLoader& loader, int Id, std::string path, std::string file, int num, std::string folder )
{
	try{
		RegisterResource(loader.Type(), path+file, Id);
		loader.Load(Id, path, file, num, folder);
	}
	catch(CException& e)
	{
		//CErrorLog::OutException(e.what());
		//WriteDump();
		MessageBoxA(NULL, e.what(), "ResourceManager", MB_ICONINFORMATION|MB_OK);
		ExitProcess(0);
	}
}

void CResourceManager::Preloader()
{
//	STR_ENCRYPT_START
	//LoadResource(*g_ServerInfo, "Data\\Local\\Serverinfo.bmd");
//	STR_ENCRYPT_END
//	STR_ENCRYPT_START
	LoadResource(*g_Glow, ".\\Data\\Local\\Glow.bmd");
//	STR_ENCRYPT_END
//	STR_ENCRYPT_START
	//LoadResource(*g_ProcessScan, "Data\\Local\\DataBase.bmd");
//	STR_ENCRYPT_END
}

void CResourceManager::LoadGraphics()
{
/*	LoadResource(CItemLoader(), MU_ITEM(14, 194), "Data\\IGC\\Jewels\\", "jewel of blood", -1, "IGC\\Jewels\\");
	LoadResource(CItemLoader(), MU_ITEM(14, 195), "Data\\IGC\\Jewels\\", "Jewel of Curse", -1, "IGC\\Jewels\\");
	LoadResource(CItemLoader(), MU_ITEM(14, 196), "Data\\IGC\\Jewels\\", "jewel of excess", -1, "IGC\\Jewels\\");
	LoadResource(CItemLoader(), MU_ITEM(14, 197), "Data\\IGC\\Jewels\\", "jewel of balance", -1, "IGC\\Jewels\\");
	LoadResource(CItemLoader(), MU_ITEM(14, 198), "Data\\IGC\\Jewels\\", "jewel o science", -1, "IGC\\Jewels\\");
	LoadResource(CItemLoader(), MU_ITEM(14, 199), "Data\\IGC\\Jewels\\", "jewel od madness", -1, "IGC\\Jewels\\");
	LoadResource(CItemLoader(), MU_ITEM(14, 200), "Data\\IGC\\Jewels\\", "jewel of black chaos", -1, "IGC\\Jewels\\");
	LoadResource(CItemLoader(), MU_ITEM(14, 201), "Data\\IGC\\Jewels\\", "jewel of dark bless", -1, "IGC\\Jewels\\");
	LoadResource(CItemLoader(), MU_ITEM(14, 202), "Data\\IGC\\Jewels\\", "jewel of dark life", -1, "IGC\\Jewels\\");
	LoadResource(CItemLoader(), MU_ITEM(14, 203), "Data\\IGC\\Jewels\\", "jewel of dark soul", -1, "IGC\\Jewels\\");
	LoadResource(CItemLoader(), MU_ITEM(14, 204), "Data\\IGC\\Jewels\\", "jewel of kondar", -1, "IGC\\Jewels\\");
	LoadResource(CItemLoader(), MU_ITEM(14, 205), "Data\\IGC\\Jewels\\", "jewel of kundun", -1, "IGC\\Jewels\\");
	LoadResource(CItemLoader(), MU_ITEM(14, 206), "Data\\IGC\\Jewels\\", "jewel of love", -1, "IGC\\Jewels\\");
	LoadResource(CItemLoader(), MU_ITEM(14, 207), "Data\\IGC\\Jewels\\", "jewel of love left part", -1, "IGC\\Jewels\\");
	LoadResource(CItemLoader(), MU_ITEM(14, 208), "Data\\IGC\\Jewels\\", "jewel of love right part", -1, "IGC\\Jewels\\");
	LoadResource(CItemLoader(), MU_ITEM(14, 209), "Data\\IGC\\Jewels\\", "jewel of venom", -1, "IGC\\Jewels\\");
	LoadResource(CItemLoader(), MU_ITEM(14, 210), "Data\\IGC\\Jewels\\", "jewel of wisdom", -1, "IGC\\Jewels\\");
	LoadResource(CItemLoader(), MU_ITEM(14, 211), "Data\\IGC\\Jewels\\", "Jewel of Maya", -1, "IGC\\Jewels\\");
	LoadResource(CItemLoader(), MU_ITEM(14, 212), "Data\\IGC\\Jewels\\", "Jewel of Tarkan", -1, "IGC\\Jewels\\");
	LoadResource(CItemLoader(), MU_ITEM(14, 213), "Data\\IGC\\Jewels\\", "jewel of disorder", -1, "IGC\\Jewels\\");
	LoadResource(CItemLoader(), MU_ITEM(14, 214), "Data\\IGC\\Jewels\\", "AncientGem", -1, "IGC\\Jewels\\");
	LoadResource(CItemLoader(), MU_ITEM(14, 215), "Data\\IGC\\Jewels\\", "Socket gem", -1, "IGC\\Jewels\\");
	LoadResource(CItemLoader(), MU_ITEM(14, 216), "Data\\IGC\\Jewels\\", "Jewel of Luck", -1, "IGC\\Jewels\\");
	LoadResource(CItemLoader(), MU_ITEM(14, 217), "Data\\IGC\\Jewels\\", "Gem of God", -1, "IGC\\Jewels\\");
	LoadResource(CItemLoader(), MU_ITEM(14, 218), "Data\\IGC\\Jewels\\", "Gem of Death", -1, "IGC\\Jewels\\");
	LoadResource(CItemLoader(), MU_ITEM(14, 219), "Data\\IGC\\Jewels\\", "Jewel of Dispense", -1, "IGC\\Jewels\\");
	LoadResource(CItemLoader(), MU_ITEM(14, 220), "Data\\IGC\\Jewels\\", "Jewel of Dream", -1, "IGC\\Jewels\\");
	LoadResource(CItemLoader(), MU_ITEM(14, 221), "Data\\IGC\\Jewels\\", "jewel of nigthmare", -1, "IGC\\Jewels\\");
	LoadResource(CItemLoader(), MU_ITEM(14, 222), "Data\\IGC\\Jewels\\", "Jewel of Sincerity", -1, "IGC\\Jewels\\");
	LoadResource(CItemLoader(), MU_ITEM(14, 223), "Data\\IGC\\Jewels\\", "Jewel of Destruction", -1, "IGC\\Jewels\\");
	LoadResource(CItemLoader(), MU_ITEM(14, 224), "Data\\IGC\\Jewels\\", "Jewel of Sea", -1, "IGC\\Jewels\\");
	LoadResource(CItemLoader(), MU_ITEM(14, 225), "Data\\IGC\\Jewels\\", "Gem of Speed", -1, "IGC\\Jewels\\");
	LoadResource(CItemLoader(), MU_ITEM(14, 226), "Data\\IGC\\Jewels\\", "Gem of Accuracy", -1, "IGC\\Jewels\\");
	LoadResource(CItemLoader(), MU_ITEM(14, 227), "Data\\IGC\\Jewels\\", "Gem of Blow", -1, "IGC\\Jewels\\");
	LoadResource(CItemLoader(), MU_ITEM(14, 228), "Data\\IGC\\Jewels\\", "Gem of Impact", -1, "IGC\\Jewels\\");
	LoadResource(CItemLoader(), MU_ITEM(14, 229), "Data\\IGC\\Jewels\\", "Gem of Dodges", -1, "IGC\\Jewels\\");
	LoadResource(CItemLoader(), MU_ITEM(14, 230), "Data\\IGC\\Jewels\\", "Gem of Fortune", -1, "IGC\\Jewels\\");
	LoadResource(CItemLoader(), MU_ITEM(14, 231), "Data\\IGC\\Jewels\\", "Gem of Weakness", -1, "IGC\\Jewels\\");
	LoadResource(CItemLoader(), MU_ITEM(14, 232), "Data\\IGC\\Jewels\\", "Gem of Reflect", -1, "IGC\\Jewels\\");
	LoadResource(CItemLoader(), MU_ITEM(14, 233), "Data\\IGC\\Jewels\\", "Gem of Vitality", -1, "IGC\\Jewels\\");
	LoadResource(CItemLoader(), MU_ITEM(14, 234), "Data\\IGC\\Jewels\\", "Gem of Energy", -1, "IGC\\Jewels\\");

	LoadResource(CTextureLoader(), 0x6000, "IGC\\IGCUI\\Panel.tga");
	LoadResource(CTextureLoader(), 0x6001, "IGC\\IGCUI\\btn_up.tga");
	LoadResource(CTextureLoader(), 0x6002, "IGC\\IGCUI\\btn_down.tga");
	LoadResource(CTextureLoader(), 0x6003, "IGC\\IGCUI\\IGC_LOGO.tga");*/

#ifdef USE_HP_BAR
	g_HpBar->LoadTextures();
#endif

	char FontName[] = "Arial";
	int BaseFontSize = 0;
	g_Font[ARIAL_8] = new CGlFont(FontName, BaseFontSize+8, FW_NORMAL, FALSE, FALSE);
	g_Font[ARIAL_12] = new CGlFont(FontName, BaseFontSize+12, FW_NORMAL, FALSE, FALSE);
	g_Font[ARIAL_14] = new CGlFont(FontName, BaseFontSize+14, FW_NORMAL, FALSE, FALSE);
	g_Font[ARIAL_15] = new CGlFont(FontName, BaseFontSize+15, FW_NORMAL, FALSE, FALSE);
	g_Font[ARIAL_16] = new CGlFont(FontName, BaseFontSize+16, FW_NORMAL, FALSE, FALSE);

	g_Font[ARIAL_BLACK_10] = new CGlFont(FontName, BaseFontSize+10, FW_BOLD, FALSE, FALSE);
	g_Font[ARIAL_BLACK_12] = new CGlFont(FontName, BaseFontSize+12, FW_BOLD, FALSE, FALSE);
	g_Font[ARIAL_BLACK_14] = new CGlFont(FontName, BaseFontSize+14, FW_BOLD, FALSE, FALSE);
	g_Font[ARIAL_BLACK_15] = new CGlFont(FontName, BaseFontSize+15, FW_BOLD, FALSE, FALSE);
	g_Font[ARIAL_BLACK_16] = new CGlFont(FontName, BaseFontSize+16, FW_BOLD, FALSE, FALSE);

	g_Font[ARIAL_UNDERLINE_10] = new CGlFont(FontName, BaseFontSize+10, FW_NORMAL, FALSE, TRUE);
	g_Font[ARIAL_UNDERLINE_12] = new CGlFont(FontName, BaseFontSize+12, FW_NORMAL, FALSE, TRUE);
	g_Font[ARIAL_UNDERLINE_14] = new CGlFont(FontName, BaseFontSize+14, FW_NORMAL, FALSE, TRUE);
	g_Font[ARIAL_UNDERLINE_15] = new CGlFont(FontName, BaseFontSize+15, FW_NORMAL, FALSE, TRUE);
	g_Font[ARIAL_UNDERLINE_16] = new CGlFont(FontName, BaseFontSize+16, FW_NORMAL, FALSE, TRUE);

	g_Font[ARIAL_ITALIC_10] = new CGlFont(FontName, BaseFontSize+8, FW_NORMAL, TRUE, FALSE);
	g_Font[ARIAL_ITALIC_12] = new CGlFont(FontName, BaseFontSize+12, FW_NORMAL, TRUE, FALSE);
	g_Font[ARIAL_ITALIC_14] = new CGlFont(FontName, BaseFontSize+14, FW_NORMAL, TRUE, FALSE);
	g_Font[ARIAL_ITALIC_15] = new CGlFont(FontName, BaseFontSize+15, FW_NORMAL, TRUE, FALSE);
	g_Font[ARIAL_ITALIC_16] = new CGlFont(FontName, BaseFontSize+16, FW_NORMAL, TRUE, FALSE);
	/*
	g_Textures = new CTexManager;
	g_Textures->LoadTextures();

	CGuiWindow* wnd = new CGuiWindow(100.0f, 100.0f, 300.0f, 300.0f, "Test Panel");
	//g_Gui->AddObj(wnd);*/

	//CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ASThread, NULL, NULL, NULL);
}
#pragma optimize(off, "gpty")

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

