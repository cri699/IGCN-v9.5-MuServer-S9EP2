//////////////////////////////////////////////////////////////////////
// ConMember.cpp
 // ------------------------------
// Decompiled by Deathway
// Date : 2007-03-09
// ------------------------------
// GS-N 0.99.60T 0x00483EE0 - Emulated completed :)
//	GS-N	1.00.18	JPN	0x0049F450	-	Completed
#include "stdafx.h"
#include "ConMember.h"
#include "TLog.h"

CConMember ConMember;


CConMember::CConMember()
{
	this->m_dwTickCount = GetTickCount();
	return;
}

CConMember::~CConMember()
{
	return;
}

void CConMember::Init()
{
	this->m_szAccount.clear();
}

BOOL CConMember::IsMember(char * AccountID )
{
	if ( (AccountID == NULL ) || ( strcmp(AccountID, "") == 0 ) )
	{
		return FALSE;
	}

	std::map<std::string, int>::iterator it = this->m_szAccount.find( (std::string) AccountID );
	
	if ( it != this->m_szAccount.end() )
	{
		return TRUE;
	}

	return FALSE;
}

void CConMember::Load(char* filename)
{
	int count=0;
	int Token;

	this->Init();

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(filename);

	if ( res.status != pugi::status_ok )
	{
		g_Log.MsgBox("File %s not found (%s)", filename, res.description());
		return;
	}

	pugi::xml_node main = file.child("ConnectMember");

	for (pugi::xml_node account = main.child("Account"); account; account = account.next_sibling())
	{
		this->m_szAccount.insert(std::pair<std::string, int>(account.attribute("Name").as_string(), 0));
	}
}

void CConMember::Run()
{
	if ( GetTickCount() - this->m_dwTickCount >= 300000 )
	{
		this->m_dwTickCount = GetTickCount();
		this->Load("IGC_ConnectMember.xml");
	}
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

