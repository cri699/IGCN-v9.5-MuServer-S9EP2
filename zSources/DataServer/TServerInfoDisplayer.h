// TServerInfoDisplayer.h: interface for the TServerInfoDisplayer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TSERVERINFODISPLAYER_H__DE0B5062_F5FC_4F24_8EF8_67E2D276CF7D__INCLUDED_)
#define AFX_TSERVERINFODISPLAYER_H__DE0B5062_F5FC_4F24_8EF8_67E2D276CF7D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum SERVER_STATE
{
	SERVER_STANDBY = 0,
	SERVER_ACTIVE = 1
};

class TServerInfoDisplayer
{

public:

	TServerInfoDisplayer();
	virtual ~TServerInfoDisplayer();

	void Run(HWND hWnd);
	void CheckLogFileSize();
	void SetState(BOOL bActive){ this->m_bServerState = bActive; this->m_dwCheckTime = GetTickCount();}
	void SetSQLError()
	{ 
		this->m_bSQLError = TRUE;
	}
	void ResetSQLError()
	{
		this->m_bSQLError = FALSE;
	}
	void InitGDIObject();
	void DelGDIObject();
	void PaintAllInfo(HWND hWnd, int iTopLeftX, int iTopLeftY);

private:

	DWORD m_dwCheckTime;
	BOOL m_bServerState;
	BOOL m_bSQLError;
	HFONT m_hFont;	// 1C
	HBRUSH m_hActiveBrush;	// 20
	HBRUSH m_hStandbyBrush;	// 24
};

extern TServerInfoDisplayer g_ServerInfoDisplayer;

#endif // !defined(AFX_TSERVERINFODISPLAYER_H__DE0B5062_F5FC_4F24_8EF8_67E2D276CF7D__INCLUDED_)

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

