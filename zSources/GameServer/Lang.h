//////////////////////////////////////////////////////////////////////
// Lang.h
#pragma once

struct Language
{
	void Clear()
	{
		this->LangID = 0;
		this->Enable = false;
		this->Codepage = 0;

		this->m_vtText.clear();
		this->m_vtMap.clear();

		strcpy(this->FileName, "");
	}

	int LangID;
	char FileName[64];
	bool Enable;
	int Codepage;

	std::map<int, std::string> m_vtText;
	std::map<int, std::string> m_vtMap;
};

struct LangData
{
	LangData()
	{
		this->Init();
	}

	void Init()
	{
		this->DefaultLang = 0;
		this->m_mapLanguages.clear();
	}

	int DefaultLang;
	std::map<int, Language> m_mapLanguages;
};

class CLanguage
{
public:
	void Init();
	bool LoadLanguage(Language & Lang);

	char *GetText(int LangID, int TextID);
	char *GetMap(int LangID, int MapID);
	WORD GetCodePage(int LangID);
	int GetDefaultLangID() { return this->m_LangData.DefaultLang; }

private:
	LangData m_LangData;
};

extern CLanguage Lang;

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

