//////////////////////////////////////////////////////////////////////
// RaklionMonsterMng.cpp
#include "stdafx.h"
#include "RaklionMonsterMng.h"
#include "GameMain.h"
#include "TLog.h"
#include "configread.h"
CRaklionMonsterMng g_RaklionMonsterMng;

CRaklionMonsterMng::CRaklionMonsterMng()
{
	this->ResetLoadData();
}

CRaklionMonsterMng::~CRaklionMonsterMng()
{
}

void CRaklionMonsterMng::ResetLoadData()
{
	for (int iCount = 0; iCount<MAX_RAKLION_MONSTER; iCount++)
	{
		memset(&this->m_SetBaseInfo[iCount], -1, sizeof(this->m_SetBaseInfo[iCount]));
	}

	this->m_iMaxMonsterCount = 0;
}

BOOL CRaklionMonsterMng::LoadData(LPSTR lpszFileName)
{
	if (g_ConfigRead.server.GetStateFromEventTable(g_ConfigRead.server.GetServerType(), EV_TABLE_RAKLION) == false)
	{
		return FALSE;
	}

	this->m_iFileDataLoad = FALSE;

	if (!lpszFileName || !strcmp(lpszFileName, ""))
	{
		g_Log.MsgBox("[Raklion][MonsterSetBase] - File load error : File Name Error");
		return FALSE;
	}

	try
	{
		pugi::xml_document file;
		pugi::xml_parse_result res = file.load_file(lpszFileName);

		if (res.status != pugi::status_ok)
		{
			g_Log.MsgBox("[Raklion][MonsterSetBase] - Can't Load %s (%s)", lpszFileName, res.description());
			return FALSE;
		}

		this->ResetLoadData();

		pugi::xml_node main = file.child("RaklionEvent");

		for (pugi::xml_node monster = main.child("Monster"); monster; monster = monster.next_sibling())
		{
			BYTE btGroup = monster.attribute("Group").as_int();
			WORD wType = monster.attribute("Index").as_int();
			BYTE btMapNumber = monster.attribute("MapNumber").as_int();
			BYTE btDistance = monster.attribute("Distance").as_int();
			BYTE btX = monster.attribute("StartX").as_int();
			BYTE btY = monster.attribute("StartY").as_int();
			BYTE btDir = monster.attribute("Dir").as_int();

			if (this->m_iMaxMonsterCount < 0 || this->m_iMaxMonsterCount >= MAX_RAKLION_MONSTER)
			{
				g_Log.MsgBox("[Raklion][MonsterSetBase] - Exceed Max Info Count (%d)", this->m_iMaxMonsterCount);
				break;
			}

			this->SetMonsterSetBaseInfo(this->m_iMaxMonsterCount, btGroup, wType, btMapNumber, btDistance, btX, btY, btDir);
			this->m_iMaxMonsterCount++;
		}

		g_Log.AddC(TColor::Red, "[Raklion][MonsterSetBase] - %s file is Loaded", lpszFileName);
		this->m_iFileDataLoad = TRUE;
	}

	catch (DWORD)
	{
		g_Log.MsgBox("[Raklion][MonsterSetBase] Loading Exception Error (%s) File. ", lpszFileName);
	}

	return this->m_iFileDataLoad;
}

void CRaklionMonsterMng::SetMonsterSetBaseInfo(int iIndex, BYTE btGroup, WORD wType, BYTE btMapNumber, BYTE btDistance, BYTE btX, BYTE btY, BYTE btDir)
{
	this->m_SetBaseInfo[iIndex].btGroup = btGroup;
	this->m_SetBaseInfo[iIndex].wType = wType;
	this->m_SetBaseInfo[iIndex].btMapNumber = btMapNumber;
	this->m_SetBaseInfo[iIndex].btDistance = btDistance;
	this->m_SetBaseInfo[iIndex].btX = btX;
	this->m_SetBaseInfo[iIndex].btY = btY;
	this->m_SetBaseInfo[iIndex].btDir = btDir;
}

void CRaklionMonsterMng::SetAllMonsters()
{
	for (int iIndex = 0; iIndex < this->m_iMaxMonsterCount; iIndex++)
	{
		int result = gObjAddMonster(this->m_SetBaseInfo[iIndex].btMapNumber);

		if (result == -1)
		{
			g_Log.MsgBox("Error - cannot add Raklion Monster");
			return;
		}

		this->SetPosition(result, iIndex);
		gObjSetMonster(result, this->m_SetBaseInfo[iIndex].wType);
	}
}

int CRaklionMonsterMng::SetPosition(int iMonsterIndex, int iTableNum)
{
	if (!ObjectMaxRange(iMonsterIndex))
	{
		g_Log.Add("ERROR : %s %d", __FILE__, __LINE__);
		return FALSE;
	}

	if (iTableNum < 0 || iTableNum > MAX_RAKLION_MONSTER - 1)
	{
		g_Log.Add("ERROR : %s %d", __FILE__, __LINE__);
		return FALSE;
	}

	LPOBJ lpObj = &gObj[iMonsterIndex];

	lpObj->m_PosNum = iTableNum;
	lpObj->X = this->m_SetBaseInfo[iTableNum].btX;
	lpObj->Y = this->m_SetBaseInfo[iTableNum].btY;
	lpObj->MapNumber = this->m_SetBaseInfo[iTableNum].btMapNumber;
	lpObj->TX = lpObj->X;
	lpObj->TY = lpObj->Y;
	lpObj->m_OldX = lpObj->X;
	lpObj->m_OldY = lpObj->Y;
	lpObj->Dir = this->m_SetBaseInfo[iTableNum].btDir;
	lpObj->StartX = lpObj->X;
	lpObj->StartY = lpObj->Y;

	if (this->GetPosition(iTableNum, lpObj->MapNumber, lpObj->X, lpObj->Y) == FALSE)
	{
		g_Log.Add("Error location Raklion Monster Error (%d) (%d)", iMonsterIndex, iTableNum);
		return FALSE;
	}

	lpObj->TX = lpObj->X;
	lpObj->TY = lpObj->Y;
	lpObj->m_OldX = lpObj->X;
	lpObj->m_OldY = lpObj->Y;
	lpObj->Dir = this->m_SetBaseInfo[iTableNum].btDir;
	lpObj->StartX = lpObj->X;
	lpObj->StartY = lpObj->Y;

	if (lpObj->Dir == (BYTE)-1)
	{
		lpObj->Dir = rand() % 8;
	}

	lpObj->m_iPentagramMainAttribute = 0;

	return TRUE;
}

int CRaklionMonsterMng::GetPosition(int TableNum, short MapNumber, short & x, short & y)
{
	if (TableNum < 0 || TableNum > MAX_RAKLION_MONSTER - 1)
	{
		g_Log.Add("ERROR : %s %d", __FILE__, __LINE__);
		return FALSE;
	}

	int iOrx = this->m_SetBaseInfo[TableNum].btX;
	int iOry = this->m_SetBaseInfo[TableNum].btY;
	BYTE btAttr = MapC[MapNumber].GetAttr(iOrx, iOry);

	if ((btAttr & 1) != 1 && (btAttr & 4) != 4 && (btAttr & 8) != 8)
	{
		x = iOrx;
		y = iOry;

		return TRUE;
	}

	int iCount = 100;	// Number of Tries

	while (iCount-- != 0)
	{
		iOrx = this->m_SetBaseInfo[TableNum].btX - 3;
		iOry = this->m_SetBaseInfo[TableNum].btY - 3;
		iOrx += rand() % 7;
		iOry += rand() % 7;

		btAttr = MapC[MapNumber].GetAttr(iOrx, iOry);

		if ((btAttr & 1) != 1 && (btAttr & 4) != 4 && (btAttr & 8) != 8)
		{
			x = iOrx;
			y = iOry;

			return TRUE;
		}
	}

	return FALSE;
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

