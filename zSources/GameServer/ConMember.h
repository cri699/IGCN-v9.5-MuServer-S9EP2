//////////////////////////////////////////////////////////////////////
// ConMember.h
// ------------------------------
// Decompiled by Deathway
// Date : 2007-03-09
// ------------------------------

#ifndef CONMEMBER_H
#define CONMEBMER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CConMember
{
public:

	CConMember();
	virtual ~CConMember();

	void Init();
	BOOL IsMember(LPSTR AccountID);
	void Load(LPSTR filename);
	void Run();

private:
	
	std::map<std::string, int> m_szAccount;
	DWORD m_dwTickCount;

};


extern CConMember ConMember;

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

