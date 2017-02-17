//////////////////////////////////////////////////////////////////////
// mdump.cpp

#include "stdafx.h"
#include <assert.h>
#include "mdump.h"
LPCSTR MiniDumper::m_szAppName;

MiniDumper::MiniDumper( LPCSTR szAppName )
{
	// if this assert fires then you have two instances of MiniDumper
	// which is not allowed
	assert( m_szAppName==NULL );

	m_szAppName = szAppName ? strdup(szAppName) : "Application";

	::SetUnhandledExceptionFilter( TopLevelFilter );
	
}

LONG MiniDumper::TopLevelFilter( struct _EXCEPTION_POINTERS *pExceptionInfo )
{
	LONG retval = EXCEPTION_CONTINUE_SEARCH;
	HWND hParent = NULL;						// find a better value for your app

	// firstly see if dbghelp.dll is around and has the function we need
	// look next to the EXE first, as the one in System32 might be old 
	// (e.g. Windows 2000)
	HMODULE hDll = NULL;
	//g_CallStack.SaveCallStack(true);
	char szDbgHelpPath[_MAX_PATH];

	if (GetModuleFileName( NULL, szDbgHelpPath, _MAX_PATH ))
	{
		char *pSlash = strrchr( szDbgHelpPath, '\\' );
		if (pSlash)
		{
			_tcscpy( pSlash+1, "DBGHELP.DLL" );
			hDll = ::LoadLibrary( szDbgHelpPath );
		}
	}

	if (hDll==NULL)
	{
		// load any version we can
		hDll = ::LoadLibrary( "DBGHELP.DLL" );
	}

	LPCTSTR szResult = NULL;

	if (hDll)
	{
		MINIDUMPWRITEDUMP pDump = (MINIDUMPWRITEDUMP)::GetProcAddress( hDll, "MiniDumpWriteDump" );
		if (pDump)
		{
			TCHAR		szDumpPath[MAX_PATH] = { 0, };
			SYSTEMTIME    SystemTime;

			GetLocalTime(&SystemTime);

			sprintf_s(szDumpPath, MAX_PATH, _T("%d-%d-%d_%dh%dm%ds-CS.dmp"),
				SystemTime.wYear,
				SystemTime.wMonth,
				SystemTime.wDay,
				SystemTime.wHour,
				SystemTime.wMinute,
				SystemTime.wSecond);

			// ask the user if they want to save a dump file
				// create the file
				HANDLE hFile = ::CreateFile( szDumpPath, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS,
											FILE_ATTRIBUTE_NORMAL, NULL );

				if (hFile!=INVALID_HANDLE_VALUE)
				{
					_MINIDUMP_EXCEPTION_INFORMATION ExInfo;

					ExInfo.ThreadId = ::GetCurrentThreadId();
					ExInfo.ExceptionPointers = pExceptionInfo;
					ExInfo.ClientPointers = NULL;

					// write the dump
					BOOL bOK = pDump( GetCurrentProcess(), GetCurrentProcessId(), hFile, (MINIDUMP_TYPE)262, &ExInfo, NULL, NULL );
					if (bOK)
					{
						retval = EXCEPTION_EXECUTE_HANDLER;
					}

					::CloseHandle(hFile);
				}
			}
		}
		else
		{
			szResult = "DBGHELP.DLL too old";
		}

	if (szResult)
	//	::MessageBox( NULL, szResult, m_szAppName, MB_OK );

	return retval;
}


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

