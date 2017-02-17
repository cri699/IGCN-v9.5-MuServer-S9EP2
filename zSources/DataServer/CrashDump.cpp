// CrashDump.cpp: implementation of the CCrashDump class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define szModule "CCrashDump"

CCrashDump g_CrashDump;

void (*CCrashDump::m_fn)();
MINIDUMPWRITEDUMP CCrashDump::m_fnMiniDump;

CCrashDump::CCrashDump():
m_hDll(NULL)
{


	TCHAR szDbgHelpPath[MAX_PATH]={0};

	// Сначала попробуем загрузить библиотеку рядом с EXE, т.к.
	// в System32 может быть старая версия.

	m_hDll = ::LoadLibrary( ".\\DBGHELP.DLL" );

	if (m_hDll==NULL)
	{
		// Если загрузка не удалась - пробуем загрузить любую
		// доступную версию
		m_hDll = ::LoadLibrary( "DBGHELP.DLL" );
	}

	//.//MessageBoxA(0,"DRAKONS",0,0);

	if(m_hDll)
	{
		//если библиотека загружена - получаем адрес MiniDumpWriteDump()
		m_fnMiniDump = (MINIDUMPWRITEDUMP)::GetProcAddress( m_hDll, "MiniDumpWriteDump" );
		
		//если всё ок - установим перехватчик
		if(m_fnMiniDump)
		{
			::SetUnhandledExceptionFilter( TopLevelFilter );
		}
	}
}

CCrashDump::~CCrashDump()
{

	if(m_hDll)
		FreeLibrary(m_hDll);
}

void CCrashDump::SetCrashCallFun(void (*fn)())
{
	m_fn = fn;
}

LONG __stdcall CCrashDump::TopLevelFilter( struct _EXCEPTION_POINTERS *pExceptionInfo )
{
	//по умолчанию завершим процесс стандартным образом
	LONG retval = EXCEPTION_CONTINUE_SEARCH;
 
 
	LPCTSTR szResult = NULL;
 
	TCHAR        DumpPath[MAX_PATH] = { 0, };
	SYSTEMTIME    SystemTime;

	GetLocalTime(&SystemTime);

	sprintf_s(DumpPath, MAX_PATH, _T("%d-%d-%d_%dh%dm%ds-DS.dmp"),
		SystemTime.wYear,
		SystemTime.wMonth,
		SystemTime.wDay,
		SystemTime.wHour,
		SystemTime.wMinute,
		SystemTime.wSecond); 

	// Создать файл
	HANDLE hFile = ::CreateFile( DumpPath, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, 
	                            CREATE_ALWAYS,
								FILE_ATTRIBUTE_NORMAL, NULL );


	if (hFile != INVALID_HANDLE_VALUE)
	{
		_MINIDUMP_EXCEPTION_INFORMATION ExInfo;
 
		ExInfo.ThreadId = ::GetCurrentThreadId();
		ExInfo.ExceptionPointers = pExceptionInfo;
		ExInfo.ClientPointers = NULL;

		// И записать в него дамп
		BOOL bOK = m_fnMiniDump( GetCurrentProcess(), GetCurrentProcessId(), 
								hFile, (MINIDUMP_TYPE)262, &ExInfo, NULL, NULL );

		retval = EXCEPTION_EXECUTE_HANDLER;
		::CloseHandle(hFile);

	}

	if(m_fn)
		m_fn();
 
	return retval;
}


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

