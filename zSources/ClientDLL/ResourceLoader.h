//////////////////////////////////////////////////////////////////////
// ResourceLoader.h
#ifndef RESOURCE_LOADER_H
#define RESOURCE_LOADER_H

#include "Exception.h"

class CResourceLoader{
public:
	virtual void Load(std::string file)
	{
		throw CException("%s - default resources loader not found", file.c_str());
	}
	virtual void Load(DWORD Id, std::string file)
	{
		throw CException("%s - default resources loader not found", file.c_str());
	}
	virtual void Load(DWORD Id, std::string path, std::string file)
	{
		throw CException("%s - default resources loader not found", file.c_str());
	}
	virtual void Load(DWORD Id, std::string path, std::string file, int num)
	{
		throw CException("%s - default resources loader not found", file.c_str());
	}
	virtual void Load(int Id, std::string path, std::string file, int num, std::string folder)
	{
		throw CException("%s - default resources loader not found", file.c_str());
	}
	virtual std::string Type()
	{
		return "Default";
	}
};

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

