//////////////////////////////////////////////////////////////////////
// AntiSwear.h

class CAntiSwear
{
public:
	CAntiSwear(void);
	virtual ~CAntiSwear(void);

	bool LoadFile(const char* File);
	bool CompareText(const char* Text);

	std::vector<std::string> m_SwearWords;
};

extern CAntiSwear SwearFilter;

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

