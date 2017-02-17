//////////////////////////////////////////////////////////////////////
// ResourceManager.h
#ifndef RESOURCES_MANAGER_H
#define RESOURCES_MANAGER_H

#include "ResourceLoader.h"

struct RESOURCE_STRUCT{
	std::string m_Type;
	std::string m_File;
	DWORD m_Id;
};

class CResourceManager{
public:
	void __fastcall DumpList(std::string file);
private:
	std::list<RESOURCE_STRUCT> m_List;
	void __fastcall RegisterResource(std::string type, std::string file, DWORD id);
	void __fastcall XorLine(char* buff, int len);
public:
	void __fastcall LoadResource(CResourceLoader& loader, std::string file);
	void __fastcall LoadResource(CResourceLoader& loader, DWORD Id, std::string file);
	void __fastcall LoadResource(CResourceLoader& loader, int Id, std::string path, std::string file, int num);
	void __fastcall LoadResource(CResourceLoader& loader, int Id, std::string path, std::string file);
	void __fastcall LoadResource(CResourceLoader& loader, int Id, std::string path, std::string file, int num, std::string folder);
public:
	void Preloader();
	void LoadGraphics();
};

extern CResourceManager ResourceManager;

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

