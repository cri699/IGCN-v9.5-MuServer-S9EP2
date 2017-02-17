//////////////////////////////////////////////////////////////////////
// DoppelGangerPosInfo.h
#ifndef DOPPELGANGER_POS_INFO_H
#define DOPPELGANGER_POS_INFO_H

#define MAX_DOPPELGANGER_MAPS			4
#define MAX_DOPPELGANGER_POS_INFO		23
#define MAX_DOPPELGANGER_COORDINATES	4

#pragma once

class CDoppelGangerPosInfo
{
public:
	CDoppelGangerPosInfo();
	virtual ~CDoppelGangerPosInfo();


	void				LoadDoppelgangerPosInfo(LPSTR lpFile);
	void				GetCenterPosition(int nMapNumber, int nPosIndex, BYTE *btX, BYTE *btY);
	int					GetPosIndex(int nMapNumber, BYTE btX, BYTE btY);
	void				GetStartMapAttr(int nMapNumber, BYTE *btX1, BYTE *btY1, BYTE *btX2, BYTE *btY2);
	int					CheckStartMapTile(int nMapNumber, BYTE btX, BYTE btY);

private:

	BOOL				m_bLoad;
	BYTE				m_arDGMapInfo[MAX_DOPPELGANGER_MAPS][MAX_DOPPELGANGER_POS_INFO][MAX_DOPPELGANGER_COORDINATES];
	BYTE				m_btDoppelTmpMapAttr[MAX_DOPPELGANGER_MAPS][MAX_DOPPELGANGER_COORDINATES];
};

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

