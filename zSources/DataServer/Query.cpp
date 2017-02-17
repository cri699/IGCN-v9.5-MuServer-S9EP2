// Query.cpp: implementation of the CQuery class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Query.h"
#include "TLog.h"
#include "LogToFile.h"
#include "DelayHandler.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define szModule "CQuery"

CLogToFile CQuery::m_LogToFile("SQL", ".\\SQL_LOG", 256);

CQuery::CQuery()
{
	this->m_ColCount = -1;
	this->m_hConnection = NULL;
	this->m_hEnviroment = NULL;
	this->m_hStmt = NULL;
	memset(this->m_SQLColName, 0, sizeof(this->m_SQLColName));
	memset(this->m_SQLData, 0, sizeof(this->m_SQLData));
	SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &this->m_hEnviroment);
	SQLSetEnvAttr(this->m_hEnviroment,SQL_ATTR_ODBC_VERSION,(SQLPOINTER)SQL_OV_ODBC3, SQL_NTS);
}

CQuery::~CQuery()
{
	if ( this->m_hStmt != SQL_NULL_HANDLE )
		SQLFreeHandle(SQL_HANDLE_STMT, this->m_hStmt);

	if ( this->m_hConnection != SQL_NULL_HANDLE )
		SQLFreeHandle(SQL_HANDLE_DBC, this->m_hConnection);

	if ( this->m_hEnviroment != SQL_NULL_HANDLE )
		SQLFreeHandle(SQL_HANDLE_ENV, this->m_hEnviroment);
}

BOOL CQuery::Connect(TCHAR * lpszDNS, TCHAR * lpszUser, TCHAR * lpszPassword, TCHAR * lpszServer)
{
	lstrcpy(this->m_szDNS, lpszDNS);
	lstrcpy(this->m_szUser, lpszUser);
	lstrcpy(this->m_szPassword, lpszPassword);
	SQLHANDLE rgbValue;

	SQLSMALLINT	cbOutCon			= 0;
	SQLCHAR InCon[256]				= {0};
	SQLCHAR OutCon[256]				= {0};

	SQLAllocHandle(SQL_HANDLE_DBC, this->m_hEnviroment, & this->m_hConnection);
	SQLSetConnectAttr(this->m_hConnection, SQL_LOGIN_TIMEOUT, &rgbValue, 0);

	wsprintf((char *)InCon, "DRIVER=SQL Server;Server=%s;DATABASE=%s;UID=%s;PWD=%s;", lpszServer,
		lpszDNS, lpszUser, lpszPassword);

	SQLRETURN Result = SQLDriverConnect(this->m_hConnection, NULL, InCon, sizeof(InCon), OutCon, sizeof(OutCon), & cbOutCon, SQL_DRIVER_NOPROMPT);

	if ( Result == SQL_ERROR)
	{
		SQLSMALLINT sRecord = 1, MsgLen;
		SQLTCHAR SqlState[6], SQLMsgError[SQL_MAX_MESSAGE_LENGTH];
		SQLINTEGER NativeError;

		while ( SQLGetDiagRec(SQL_HANDLE_DBC, this->m_hConnection, sRecord, SqlState, &NativeError, SQLMsgError, sizeof(SQLMsgError), &MsgLen) != SQL_NO_DATA )
		{
			g_Log.AddC(TColor::Blue, "SQLSTATE: %s, Diagnosis: %s", SqlState, SQLMsgError);
			CQuery::m_LogToFile.Output("SQLSTATE: %s, Diagnosis: %s", SqlState, SQLMsgError);
			sRecord++;
		}

		return FALSE;
	}

	Result = SQLAllocHandle(SQL_HANDLE_STMT, this->m_hConnection, &this->m_hStmt);

	if ( Result != SQL_SUCCESS && Result != SQL_SUCCESS_WITH_INFO )
		return FALSE;

	return TRUE;
}

BOOL CQuery::ReConnect()
{
	return this->Connect(this->m_szDNS, this->m_szUser, this->m_szPassword, g_ServerName);
}


BOOL CQuery::ExecQuery(TCHAR * lpszStatement, ...)
{
	TCHAR szStatement[1024];

	va_list pArguments;
	va_start(pArguments, lpszStatement);
	vsprintf(szStatement, lpszStatement, pArguments);
	va_end(pArguments);

	while (true)
	{
		CQuery::m_LogToFile.Output(szStatement);

		if(g_iShowAllQueriesInDS == TRUE)
		{
			g_Log.AddL(TColor::Aqua, szStatement);
		}

		SQLRETURN Result = SQLExecDirect(this->m_hStmt, (SQLTCHAR *)szStatement, SQL_NTS);

		if (Result != SQL_SUCCESS && Result != SQL_SUCCESS_WITH_INFO && Result != SQL_NO_DATA)
		{
			g_Log.AddC(TColor::Blue, "[SQL Error] Error executing: %s", szStatement);
			g_ServerInfoDisplayer.SetSQLError();

			bool bReconnect = false;
			this->Diagnosis(bReconnect);

			if (bReconnect == true)
			{
				Sleep(1);
				continue;
			}

			return FALSE;
		}

		SQLLEN RowCount;
		SQLRowCount(this->m_hStmt, &RowCount);
		SQLNumResultCols(this->m_hStmt, &this->m_ColCount);

		if (this->m_ColCount >= MAX_COLUMNS - 1)
		{
			g_Log.AddC(TColor::Red, "[CQuery] ColCount >= MAX_COLUMNS-1");
			return FALSE;
		}

		// Case just EXEC
		if (this->m_ColCount == 0)
		{
			this->Close();
			return TRUE;
		}

		for (int iColPos = 0; iColPos < this->m_ColCount; iColPos++)
		{
			SQLBindCol(this->m_hStmt, iColPos + 1, SQL_CHAR, this->m_SQLData[iColPos],
				sizeof(this->m_SQLData[0]) - 1, &this->m_SQLDataLen[iColPos]);
			SQLDescribeCol(this->m_hStmt, iColPos + 1, this->m_SQLColName[iColPos],
				sizeof(this->m_SQLColName[iColPos]), NULL, NULL, NULL, NULL, NULL);
		}

		return TRUE;
	}
}

void CQuery::Close()
{
	SQLCloseCursor(this->m_hStmt);
	SQLFreeStmt(this->m_hStmt, SQL_UNBIND);
}


SQLRETURN CQuery::Fetch()
{
	return SQLFetch(this->m_hStmt);
}

int CQuery::FindIndex(LPTSTR ColName)
{
	for(short i=0;i<this->m_ColCount;i++)
	{
		if ( this->m_SQLColName[i][0] == ColName[0] )
		{
			if ( lstrcmp((TCHAR *)this->m_SQLColName[i], (TCHAR *)ColName) == 0 )
			{
				return i;
			}
		}
	}

	return -1;
}

void CQuery::GetAsString(LPTSTR ColName, LPTSTR pOutBuffer,int size)
{
	int iIndex = this->FindIndex(ColName);

	if ( iIndex != -1 )
	{
		this->GetAsString(iIndex, pOutBuffer, size);
		return;
	}
	else
	{
		pOutBuffer[0] = 0;
	}
}

void CQuery::GetAsString(int iIndex, LPTSTR pOutBuffer, int size)
{
	if (iIndex == -1)
	{
		pOutBuffer[0] = 0;
	}

	else if (this->m_SQLDataLen[iIndex] == SQL_NULL_DATA)
	{
		pOutBuffer[0] = 0;
	}

	else
	{
		strncpy(pOutBuffer, this->m_SQLData[iIndex], size);
	}
}

DWORD CQuery::GetAsInteger(LPTSTR ColName)
{
	int iIndex =this->FindIndex(ColName);

	if ( iIndex != -1 )
	{
		return this->GetAsInteger(iIndex);
	}

	return -1;
}

DWORD CQuery::GetAsInteger(int iIndex)
{
	if (iIndex == -1)
	{
		return -1;
	}

	else if (this->m_SQLDataLen[iIndex] == SQL_NULL_DATA)
	{
		return -1;
	}

	else
	{
		return atoi(this->m_SQLData[iIndex]);
	}

	return -1;
}

INT64 CQuery::GetAsInteger64(LPTSTR ColName)
{
	int iIndex = this->FindIndex(ColName);

	if ( iIndex != -1 )
	{
		return this->GetAsInteger64(iIndex);
	}

	return -1LL;
}

INT64 CQuery::GetAsInteger64(int iIndex)
{
	if (iIndex == -1)
	{
		return -1LL;
	}

	else if (this->m_SQLDataLen[iIndex] == SQL_NULL_DATA)
	{
		return -1LL;
	}

	else
	{
		return _atoi64(this->m_SQLData[iIndex]);
	}

	return -1LL;
}

float CQuery::GetAsFloat(LPTSTR ColName)
{
	int iIndex =this->FindIndex(ColName);

	if ( iIndex != -1 )
	{
		return this->GetAsFloat(iIndex);
	}

	return -1;
}

float CQuery::GetAsFloat(int iIndex)
{
	if (iIndex == -1)
	{
		return -1;
	}

	else if (this->m_SQLDataLen[iIndex] == SQL_NULL_DATA)
	{
		return -1;
	}

	else
	{
		return (float)atof(this->m_SQLData[iIndex]);
	}

	return -1;
}

void CQuery::Diagnosis(bool &bReconnect)
{
	SQLSMALLINT sRecord = 1, MsgLen;
	SQLTCHAR SqlState[6], SQLMsgError[SQL_MAX_MESSAGE_LENGTH];
	SQLINTEGER NativeError;

	while ( SQLGetDiagRec(SQL_HANDLE_STMT, this->m_hStmt, sRecord, SqlState, &NativeError, SQLMsgError, sizeof(SQLMsgError), &MsgLen) != SQL_NO_DATA )
	{	
		g_Log.AddC(TColor::Blue, "SQLSTATE: %s, Diagnosis: %s", SqlState, SQLMsgError);
		CQuery::m_LogToFile.Output("SQLSTATE: %s, Diagnosis: %s", SqlState, SQLMsgError);
		sRecord++;
	}

	if ( !lstrcmp((TCHAR *)SqlState, "08S01"))	// Communication Link Failure
	{
		if(this->ReConnect() == TRUE)
		{
			g_DelayHandler.IncreaseQuerySessionId();
		}

		bReconnect = true;
	}
}

int CQuery::GetResult(int iIndex)
{
	return atoi(this->m_SQLData[iIndex]);
}

int CQuery::GetAsBinary(LPTSTR lpszStatement, LPBYTE OUT lpszReturnBuffer, int size)
{
	SQLCHAR * pSQLBuf;
	SQLINTEGER BufLen = -1;
	SQLCHAR SQLBinary[MAX_BINARY_SIZE];
	SQLINTEGER lOfs = 0;
	SQLLEN SQLLen;
	SQLRETURN SQLResult;

	while (true)
	{
		CQuery::m_LogToFile.Output(lpszStatement);

		if (g_iShowAllQueriesInDS == TRUE)
		{
			g_Log.AddL(TColor::Aqua, lpszStatement);
		}

		SQLResult = SQLExecDirect(this->m_hStmt, (SQLTCHAR *)lpszStatement, SQL_NTS);

		if (SQLResult != SQL_SUCCESS)
		{
			g_Log.AddC(TColor::Blue, "[SQL Error] Error executing: %s", lpszStatement);
			g_ServerInfoDisplayer.SetSQLError();

			bool bReconnect = false;
			this->Diagnosis(bReconnect);

			if (bReconnect == true)
			{
				Sleep(1);
				continue;
			}

			return -1;
		}

		while ( true )
		{
			SQLResult = SQLFetch(this->m_hStmt);

			if ( SQLResult == SQL_NO_DATA )
				break;

			pSQLBuf = lpszReturnBuffer;

			while (true)
			{
				SQLResult = SQLGetData(this->m_hStmt, 1, SQL_C_BINARY, SQLBinary, sizeof(SQLBinary), &SQLLen);

				if (SQLResult == SQL_NO_DATA)
					break;

				if (SQLResult == SQL_ERROR)
					break;

				if (SQLLen == SQL_NULL_DATA)
				{
					this->Close();
					return 0;
				}

				if (SQLResult == SQL_SUCCESS)
				{
					BufLen = SQLLen;
				}

				else
				{
					BufLen = MAX_BINARY_SIZE;
				}

				lOfs += BufLen;
				memcpy_s(pSQLBuf, size, SQLBinary, BufLen);
				pSQLBuf = &pSQLBuf[lOfs];
			}
		}

		this->Close();
		return BufLen;
	}
}

void CQuery::SetAsBinary(LPTSTR lpszStatement, LPBYTE lpBinaryBuffer, SQLUINTEGER BinaryBufferSize)
{
	CQuery::m_LogToFile.Output(lpszStatement);

	if (g_iShowAllQueriesInDS == TRUE)
	{
		g_Log.AddL(TColor::Aqua, lpszStatement);
	}

	SQLLEN cbValueSize = -100LL - BinaryBufferSize;
	SQLPOINTER pToken;
	BYTE cBUFFER[MAX_BINARY_SIZE];
	SQLRETURN Result;

	SQLBindParameter(this->m_hStmt, 1, SQL_PARAM_INPUT, SQL_C_BINARY , SQL_LONGVARBINARY, BinaryBufferSize, 0, (SQLPOINTER)1, 0, &cbValueSize);
	SQLExecDirect(this->m_hStmt, (SQLTCHAR *)lpszStatement, SQL_NTS);
	
	Result = SQLParamData(this->m_hStmt, &pToken);

	int lOfs=0;

	while ( Result == SQL_NEED_DATA )
	{
		memcpy(cBUFFER, lpBinaryBuffer, BinaryBufferSize);
		Result = SQLPutData(this->m_hStmt, cBUFFER, BinaryBufferSize);
		Result = SQLParamData(this->m_hStmt, &pToken);
		lOfs += BinaryBufferSize;
	}

	SQLParamData(this->m_hStmt, &pToken);
	this->Close();
}

void CQuery::Disconnect()
{
	if ( this->m_hStmt )
		SQLFreeHandle(SQL_HANDLE_STMT, this->m_hStmt);

	if ( this->m_hConnection )
		SQLDisconnect(this->m_hConnection);

	if ( this->m_hConnection )
		SQLFreeHandle(SQL_HANDLE_DBC, this->m_hConnection);

	this->m_hStmt = NULL;
	this->m_hConnection = NULL;
}

SQLRETURN CQuery::BindParameterBinaryOutput(int nCol, BYTE *nValue, int iSize, SQLLEN *lLength)
{
	return SQLBindParameter(this->m_hStmt, nCol, 4, -2, -3, iSize, 0, nValue, iSize, lLength);
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

