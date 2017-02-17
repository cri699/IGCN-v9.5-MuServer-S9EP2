//////////////////////////////////////////////////////////////////////
// ProhibitedSymbols.h
#ifndef PROHIBITEDSYMBOLS_H
#define PROHIBITEDSYMBOLS_H
#define TYPE_CHAT 0
#define TYPE_NAME 1
#define TYPE_GUILDNAME 2
struct _PROSYM_STRUCT{

	wchar_t * symbol;
	BYTE system[3];
};
struct _REGEX
{
	char Regex[200];
};
class CProhibitedSymbols
{
public:
	void LoadSymbolFile(LPSTR szFile);
	bool Validate(char* string, int len, BYTE Type);
	std::map<wchar_t,_PROSYM_STRUCT> m_ProhibitedSymbols;
	int m_nMode;
	_REGEX m_sRegEx[3];
}; extern CProhibitedSymbols g_prohibitedSymbols;

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

