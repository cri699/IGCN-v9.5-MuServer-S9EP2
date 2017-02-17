//////////////////////////////////////////////////////////////////////
// mdump.cpp
#include "stdafx.h"
#include <windows.h>
#include "mdump.h"
#include <DBGHELP.H>
#include <stdio.h>
typedef BOOL(WINAPI *MINIDUMPWRITEDUMP)( // Callback ??? ??
	HANDLE hProcess,
	DWORD dwPid,
	HANDLE hFile,
	MINIDUMP_TYPE DumpType,
	CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
	CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
	CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam);

LPTOP_LEVEL_EXCEPTION_FILTER PreviousExceptionFilter = NULL;

LONG WINAPI UnHandledExceptionFilter(struct _EXCEPTION_POINTERS *exceptionInfo)
{
	HMODULE    DllHandle = NULL;

	// Windows 2000 ???? ?? DBGHELP? ???? ??? ??? ??.
	DllHandle = LoadLibrary(_T("DBGHELP.DLL"));

	if (DllHandle)
	{
		MINIDUMPWRITEDUMP Dump = (MINIDUMPWRITEDUMP)GetProcAddress(DllHandle, "MiniDumpWriteDump");

		if (Dump)
		{
			TCHAR        DumpPath[MAX_PATH] = { 0, };
			SYSTEMTIME    SystemTime;

			GetLocalTime(&SystemTime);

			sprintf_s(DumpPath, MAX_PATH, _T("%d-%d-%d_%dh%dm%ds.dmp"),
				SystemTime.wYear,
				SystemTime.wMonth,
				SystemTime.wDay,
				SystemTime.wHour,
				SystemTime.wMinute,
				SystemTime.wSecond);

			HANDLE FileHandle = CreateFile(
				DumpPath,
				GENERIC_WRITE,
				FILE_SHARE_WRITE,
				NULL, CREATE_ALWAYS,
				FILE_ATTRIBUTE_NORMAL,
				NULL);

			if (FileHandle != INVALID_HANDLE_VALUE)
			{
				_MINIDUMP_EXCEPTION_INFORMATION MiniDumpExceptionInfo;

				MiniDumpExceptionInfo.ThreadId = GetCurrentThreadId();
				MiniDumpExceptionInfo.ExceptionPointers = exceptionInfo;
				MiniDumpExceptionInfo.ClientPointers = NULL;

				BOOL Success = Dump(
					GetCurrentProcess(),
					GetCurrentProcessId(),
					FileHandle,
					(_MINIDUMP_TYPE)(MiniDumpWithUnloadedModules +
					MiniDumpWithHandleData +
					MiniDumpWithFullMemory), //webzen...
					&MiniDumpExceptionInfo,
					NULL,
					NULL);

				if (Success)
				{
					CloseHandle(FileHandle);

					return EXCEPTION_EXECUTE_HANDLER;
				}
			}

			CloseHandle(FileHandle);
		}
	}

	return EXCEPTION_CONTINUE_SEARCH;
}

BOOL CMiniDump::Begin(VOID)
{
	SetErrorMode(SEM_FAILCRITICALERRORS);

	PreviousExceptionFilter = SetUnhandledExceptionFilter(UnHandledExceptionFilter);

	return true;
}

BOOL CMiniDump::End(VOID)
{
	SetUnhandledExceptionFilter(PreviousExceptionFilter);

	return true;
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

