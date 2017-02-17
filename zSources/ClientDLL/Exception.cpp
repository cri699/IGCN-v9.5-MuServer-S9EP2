//////////////////////////////////////////////////////////////////////
// Exception.cpp
#include "stdafx.h"
#include "Exception.h"

CException::CException( const char* format, ... )
{
	char sException[1024] = {0};
	va_list va;
	va_start(va, format);
	vsprintf_s(sException, format, va);
	va_end(va);
	m_Exception = sException;
}

CException::CException( const CException& e )
{
	m_Exception = e.m_Exception;
}

CException CException::operator=( const CException& e )
{
	m_Exception = e.m_Exception;
	return 0;
}

const char* CException::what()
{
	return m_Exception.data();
}


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

