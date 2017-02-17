//////////////////////////////////////////////////////////////////////
// DoppelGangerPosInfo.cpp
#include "stdafx.h"
#include "DoppelGangerPosInfo.h"
#include "TLog.h"
#include "user.h"

CDoppelGangerPosInfo::CDoppelGangerPosInfo(void)
{
}

CDoppelGangerPosInfo::~CDoppelGangerPosInfo(void)
{
}

void CDoppelGangerPosInfo::LoadDoppelgangerPosInfo(LPSTR lpFile)
{
	this->m_bLoad = FALSE;

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(lpFile);

	if ( res.status != pugi::status_ok )
	{
		g_Log.MsgBox("%s load fail (%s)", lpFile, res.description());
		return;
	}

	pugi::xml_node main = file.child("DoppelGanger");
	pugi::xml_node pos_info = main.child("PositionInfo");

	for (pugi::xml_node map = pos_info.child("Map"); map; map = map.next_sibling())
	{
		int iMapNumber = map.attribute("Number").as_int();

		if (iMapNumber < MAP_INDEX_DOUBLE_GOER1 || iMapNumber > MAP_INDEX_DOUBLE_GOER4)
		{
			g_Log.MsgBox("Error: Wrong Map Number (%d) (%s)", iMapNumber, lpFile);
			return;
		}

		int nCount = 0;

		for (pugi::xml_node position = map.child("Pos"); position; position = position.next_sibling())
		{
			this->m_arDGMapInfo[iMapNumber - MAP_INDEX_DOUBLE_GOER1][nCount][0] = position.attribute("StartX").as_int();
			this->m_arDGMapInfo[iMapNumber - MAP_INDEX_DOUBLE_GOER1][nCount][1] = position.attribute("StartY").as_int();
			this->m_arDGMapInfo[iMapNumber - MAP_INDEX_DOUBLE_GOER1][nCount][2] = position.attribute("EndX").as_int();
			this->m_arDGMapInfo[iMapNumber - MAP_INDEX_DOUBLE_GOER1][nCount][3] = position.attribute("EndY").as_int();
			nCount++;
		}
	}

	pugi::xml_node waiting_zones = main.child("WaitingZones");

	for (pugi::xml_node map = waiting_zones.child("Map"); map; map = map.next_sibling())
	{
		int iMapNumber = map.attribute("Number").as_int();

		if (iMapNumber < MAP_INDEX_DOUBLE_GOER1 || iMapNumber > MAP_INDEX_DOUBLE_GOER4)
		{
			g_Log.MsgBox("Error: Wrong Map Number (%d) (%s)", iMapNumber, lpFile);
			return;
		}

		this->m_btDoppelTmpMapAttr[iMapNumber - MAP_INDEX_DOUBLE_GOER1][0] = map.attribute("StartX").as_int();
		this->m_btDoppelTmpMapAttr[iMapNumber - MAP_INDEX_DOUBLE_GOER1][1] = map.attribute("StartY").as_int();
		this->m_btDoppelTmpMapAttr[iMapNumber - MAP_INDEX_DOUBLE_GOER1][2] = map.attribute("EndX").as_int();
		this->m_btDoppelTmpMapAttr[iMapNumber - MAP_INDEX_DOUBLE_GOER1][3] = map.attribute("EndY").as_int();
	}

	this->m_bLoad = TRUE;
}

void CDoppelGangerPosInfo::GetCenterPosition(int nMapNumber, int nPosIndex, BYTE *btX, BYTE *btY)
{
	BYTE btTmpX = this->m_arDGMapInfo[nMapNumber - MAP_INDEX_DOUBLE_GOER1][nPosIndex][2] - this->m_arDGMapInfo[nMapNumber - MAP_INDEX_DOUBLE_GOER1][nPosIndex][0];
	*btX = btTmpX / 2 + this->m_arDGMapInfo[nMapNumber - MAP_INDEX_DOUBLE_GOER1][nPosIndex][0];
	BYTE btTmpY = this->m_arDGMapInfo[nMapNumber - MAP_INDEX_DOUBLE_GOER1][nPosIndex][3] - this->m_arDGMapInfo[nMapNumber - MAP_INDEX_DOUBLE_GOER1][nPosIndex][1];
	*btY = btTmpY / 2 + this->m_arDGMapInfo[nMapNumber - MAP_INDEX_DOUBLE_GOER1][nPosIndex][1];
}

int CDoppelGangerPosInfo::GetPosIndex(int nMapNumber, BYTE btX, BYTE btY)
{
	for (int i = 0; i < MAX_DOPPELGANGER_POS_INFO; i++)
	{
		if (btX > this->m_arDGMapInfo[nMapNumber - MAP_INDEX_DOUBLE_GOER1][i][0] && btX <= this->m_arDGMapInfo[nMapNumber - MAP_INDEX_DOUBLE_GOER1][i][2] &&
			btY > this->m_arDGMapInfo[nMapNumber - MAP_INDEX_DOUBLE_GOER1][i][1] && btY <= this->m_arDGMapInfo[nMapNumber - MAP_INDEX_DOUBLE_GOER1][i][3])
		{
			return i;
		}
	}

	return 0;
}

void CDoppelGangerPosInfo::GetStartMapAttr(int nMapNumber, BYTE *btX1, BYTE *btY1, BYTE *btX2, BYTE *btY2)
{
	*btX1 = this->m_btDoppelTmpMapAttr[nMapNumber - MAP_INDEX_DOUBLE_GOER1][0];
	*btY1 = this->m_btDoppelTmpMapAttr[nMapNumber - MAP_INDEX_DOUBLE_GOER1][1];
	*btX2 = this->m_btDoppelTmpMapAttr[nMapNumber - MAP_INDEX_DOUBLE_GOER1][2];
	*btY2 = this->m_btDoppelTmpMapAttr[nMapNumber - MAP_INDEX_DOUBLE_GOER1][3];
}

int CDoppelGangerPosInfo::CheckStartMapTile(int nMapNumber, BYTE btX, BYTE btY)
{
	return btX <= this->m_btDoppelTmpMapAttr[nMapNumber - MAP_INDEX_DOUBLE_GOER1][0] ||
		btX >= this->m_btDoppelTmpMapAttr[nMapNumber - MAP_INDEX_DOUBLE_GOER1][2] ||
		btY <= this->m_btDoppelTmpMapAttr[nMapNumber - MAP_INDEX_DOUBLE_GOER1][1] ||
		btY >= this->m_btDoppelTmpMapAttr[nMapNumber - MAP_INDEX_DOUBLE_GOER1][3];
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

