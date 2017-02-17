//////////////////////////////////////////////////////////////////////
// ScriptLuaMem.cpp
#include "stdafx.h"
#include "ScriptLuaMem.h"
#include "winutil.h"
#include "TLog.h"
#include "MuLua.h"

CScriptLuaMem::CScriptLuaMem()
{
	this->chScriptData = NULL;
	this->bScriptLoaded = false;
}

CScriptLuaMem::~CScriptLuaMem()
{
	delete [] chScriptData;
}

bool CScriptLuaMem::LoadScript(LPSTR pchFileName)
{
	this->bScriptLoaded = false;

	if (!IsFile(pchFileName))
	{
		g_Log.MsgBox("%s file load fail!", pchFileName);
		return false;
	}

	int fileSize;
	std::ifstream file(pchFileName, std::ios::binary);

	file.seekg(0, std::ios::end);
	fileSize = file.tellg();
	file.seekg(0, std::ios::beg);

	if (this->chScriptData != NULL)
	{
		delete this->chScriptData;
	}
	this->chScriptData = new char[fileSize+1];

	if (this->chScriptData == NULL)
	{
		g_Log.MsgBox("Memory allocation failed!");
		return false;
	}

	file.read((char*)this->chScriptData, fileSize);

	this->chScriptData[fileSize] = NULL;

	file.close();

	MULua TestLua;

	if (TestLua.DoFile(this->chScriptData, fileSize) != true)
	{
		return false;
	}

	this->bScriptLoaded = true;

	return true;
}

char* CScriptLuaMem::GetScript()
{
	if (this->chScriptData == NULL)
	{
		return NULL;
	}

	return this->chScriptData;
}

bool CScriptLuaMem::IsScriptLoaded()
{
	return this->bScriptLoaded;
}



//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

