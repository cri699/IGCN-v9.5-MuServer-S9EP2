//////////////////////////////////////////////////////////////////////
// DatabaseInstaller.h
#pragma once
class CDatabaseInstaller
{
public:
	CDatabaseInstaller(void);
	virtual ~CDatabaseInstaller(void);

	int GetDBVersion();

	void InstallUpdates();

	bool Install7041();
	bool Install7042();

	void Init();
	void End();

	CQuery * Database;
	
	bool m_IsConnected;
};



//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

