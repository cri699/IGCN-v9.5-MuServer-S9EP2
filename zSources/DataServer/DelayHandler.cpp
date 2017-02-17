// DelayHandler.cpp: implementation of the DelayHandler class.
// Decompilation Completed -> All Same as WebZen
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DelayHandler.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
DelayHandler	g_DelayHandler;

DelayHandler::DelayHandler()
{
	InitializeCriticalSection(&this->m_critHeadCode);
	this->m_lQuerySessionId = 0;
	this->Initialize();
}

DelayHandler::~DelayHandler()
{
	DeleteCriticalSection(&this->m_critHeadCode);
}

void DelayHandler::Initialize()
{
	this->AddHeadCode(0x00);
	this->AddHeadCode(0x01);
	this->AddHeadCode(0x02);
	this->AddHeadCode(0x03);
	this->AddHeadCode(0x04);
	this->AddHeadCode(0x05);
	this->AddHeadCode(0x07);
	this->AddHeadCode(0x09);
	this->AddHeadCode(0x11);
	this->AddHeadCode(0x12);
	this->AddHeadCode(0x20);
	this->AddHeadCode(0x52);
	this->AddHeadCode(0x54);
	this->AddHeadCode(0x55);
	this->AddHeadCode(0x57);
	this->AddHeadCode(0x60);
	this->AddHeadCode(0x80);
	this->AddHeadCode(0x81);
	this->AddHeadCode(0x82);
	this->AddHeadCode(0x83);
	this->AddHeadCode(0x84);
	this->AddHeadCode(0x85);
	this->AddHeadCode(0x86);
	this->AddHeadCode(0x87);
	this->AddHeadCode(0x88);
	this->AddHeadCode(0x89);
	this->AddHeadCode(0xCF);
}

void DelayHandler::AddHeadCode(BYTE btHeadCode)
{
	EnterCriticalSection(&this->m_critHeadCode);

	this->m_mapHeadCode.insert( std::pair<int, int>(btHeadCode, 0) );

	LeaveCriticalSection(&this->m_critHeadCode);
}

bool DelayHandler::CheckHeadCode(BYTE btHeadCode)
{
	bool bRetVal = false;

	EnterCriticalSection(&this->m_critHeadCode);

	std::map<int, int>::iterator it = this->m_mapHeadCode.find(btHeadCode);

	if ( it != this->m_mapHeadCode.end() )
	{
		bRetVal = true;
	}

	LeaveCriticalSection(&this->m_critHeadCode);

	return bRetVal;
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

