// TMonsterAIMovePath.cpp: implementation of the TMonsterAIMovePath class.
//	GS-N	1.00.18	JPN	0x00561880	-	Completed
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TMonsterAIMovePath.h"
#include "TLog.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TMonsterAIMovePath::TMonsterAIMovePath()
{
	this->DelAllAIMonsterMovePath();
}


TMonsterAIMovePath::~TMonsterAIMovePath()
{
	return;
}



BOOL TMonsterAIMovePath::LoadData(LPSTR lpszFileName, LPSTR lpszSectionType)
{
	this->m_bDataLoad = FALSE;

	if ( !lpszFileName || !strcmp(lpszFileName, ""))
	{
		g_Log.MsgBox("[Monster AI MovePath] - File load error : File Name Error");
		return FALSE;
	}

	if ( !lpszSectionType || !strcmp(lpszSectionType, ""))
	{
		g_Log.MsgBox("[Monster AI MovePath] - File load error : Section Name Error");
		return FALSE;
	}

	try
	{
		pugi::xml_document file;
		pugi::xml_parse_result res = file.load_file(lpszFileName);

		if ( res.status != pugi::status_ok )
		{
			g_Log.MsgBox("[Monster AI MovePath] - Can't Load %s (%s)", lpszFileName, res.description());
			return FALSE;
		}

		this->DelAllAIMonsterMovePath();

		pugi::xml_node main = file.child("AIMovePath");
		pugi::xml_node section = main.child(lpszSectionType);

		for (pugi::xml_node spot = main.child("Spot"); spot; spot = spot.next_sibling())
		{
			int iSpotType = spot.attribute("Type").as_int();
			int iMapNumber = spot.attribute("MapNumber").as_int();
			int iX = spot.attribute("PosX").as_int();
			int iY = spot.attribute("PosY").as_int();

			this->m_MovePathInfo[this->m_iMovePathSpotCount].m_iType = iSpotType;
			this->m_MovePathInfo[this->m_iMovePathSpotCount].m_iMapNumber = iMapNumber;
			this->m_MovePathInfo[this->m_iMovePathSpotCount].m_iPathX = iX;
			this->m_MovePathInfo[this->m_iMovePathSpotCount].m_iPathY = iY;

			this->m_iMovePathSpotCount++;

			if ( this->m_iMovePathSpotCount > MAX_MONSTER_AI_MOVE_PATH )
			{
				g_Log.MsgBox("[Monster AI MovePath] Exceed Max Move Path-Spot ");
				this->DelAllAIMonsterMovePath();
				return FALSE;
			}
		}

		g_Log.AddC(TColor::Red,  "[Monster AI MovePath ] - %s file is Loaded", lpszFileName);
		this->m_bDataLoad = TRUE;
	}

	catch(DWORD)
	{
		g_Log.MsgBox("[Monster AI MovePath] - Loading Exception Error (%s) File. ", lpszFileName);
	}

	return FALSE;
}


BOOL TMonsterAIMovePath::DelAllAIMonsterMovePath()
{
	for ( int i=0;i<MAX_MONSTER_AI_MOVE_PATH;i++)
	{
		this->m_MovePathInfo[i].Reset();
	}

	this->m_iMovePathSpotCount = 0;
	this->m_bDataLoad = FALSE;

	return FALSE;
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

