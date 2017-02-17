//////////////////////////////////////////////////////////////////////
// ScriptLuaMem.h
#pragma once
class CScriptLuaMem
{
public:

	CScriptLuaMem();
	virtual		~CScriptLuaMem();

	bool		LoadScript(LPSTR pchFileName);

	char*		GetScript();
	bool		IsScriptLoaded();

private:
	bool		bScriptLoaded;
	char*		chScriptData;
};



//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

