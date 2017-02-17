// TServerInfoDisplayer.cpp: implementation of the TServerInfoDisplayer class.
//	GS-N	1.00.18	JPN	0x004A9500	-	Completed
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TServerInfoDisplayer.h"

TServerInfoDisplayer g_ServerInfoDisplayer;

static char * ServerTypeText[] =
{
	"[-  Standby  -]",
	"[-  Active  -]"

};

static char * ErrorMessge[] = 
{
	" ERROR: SQLSTATE error detected. Review logs and fix the issue (use Options -> Error Status reset to clear it after)"
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TServerInfoDisplayer::TServerInfoDisplayer()
{
	this->m_bServerState = 0;
	this->m_bServerState = FALSE;
	this->InitGDIObject();
}

TServerInfoDisplayer::~TServerInfoDisplayer()
{
	this->DelGDIObject();
}


void TServerInfoDisplayer::InitGDIObject()
{
	this->m_hFont = CreateFont(80, 0, 0, 0, FW_THIN, FALSE, FALSE, FALSE, 
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH|FF_DONTCARE, "Times");

	this->m_hActiveBrush = CreateSolidBrush(RGB(110, 240, 120));	// GREEN
	this->m_hStandbyBrush = CreateSolidBrush(RGB(105, 105, 105));	// GRAY
}


void TServerInfoDisplayer::DelGDIObject()
{
	DeleteObject(this->m_hFont);
	DeleteObject(this->m_hActiveBrush);
	DeleteObject(this->m_hStandbyBrush);
}



void TServerInfoDisplayer::Run(HWND hWnd)
{
	if ( GetTickCount() - this->m_dwCheckTime > 60000 )
	{
		this->SetState(SERVER_STANDBY);
	}

	this->PaintAllInfo(hWnd, 0, 20);
}


void TServerInfoDisplayer::PaintAllInfo(HWND hWnd, int iTopLeftX, int iTopLeftY)
{
	HDC hDC = GetDC(hWnd);
	RECT rect;
	GetClientRect(hWnd, &rect);
	rect.top = 20;
	rect.bottom = 100;
	int iLineCount = 0;
	int iIndex=0;

	int iOldBkMode = SetBkMode(hDC, TRANSPARENT);

	if ( this->m_bServerState == SERVER_ACTIVE )
	{
		FillRect(hDC, &rect, this->m_hActiveBrush);
		iIndex = 1;
	}

	else if ( this->m_bServerState == SERVER_STANDBY )
	{
		FillRect(hDC, &rect, this->m_hStandbyBrush);
		iIndex = 0;
	}

	if ( this->m_bSQLError == TRUE )
	{
		rect.top = 20;
		SetTextColor(hDC, RGB(0, 0, 255));
		TextOut(hDC, iTopLeftX, iTopLeftY+iLineCount*15, ErrorMessge[0], (int)strlen(ErrorMessge[0]));
		iLineCount++;
	}

	HFONT pOldFont = (HFONT)SelectObject(hDC, this->m_hFont);
	SetTextColor(hDC, RGB(250, 250, 250));

	TextOut(hDC, 250, 20, ServerTypeText[iIndex], (int)strlen(ServerTypeText[iIndex]));
	SelectObject(hDC, pOldFont);
	SetBkMode(hDC, iOldBkMode);
	ReleaseDC(hWnd, hDC);
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

