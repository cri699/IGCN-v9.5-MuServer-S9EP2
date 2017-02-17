//////////////////////////////////////////////////////////////////////
// SkillHitBox.cpp
// ------------------------------
// Decompiled by Deathway
// Date : 2007-03-09
// ------------------------------
#include "stdafx.h"
#include "SkillHitBox.h"
#include "winutil.h"
#include "TLog.h"

// GS-N 0.99.60T 0x46E710
//	GS-N	1.00.18	JPN	0x004856C0	-	Completed

CSkillHitBox SkillSpearHitBox;
CSkillHitBox SkillElectricSparkHitBox;

CSkillHitBox::CSkillHitBox()
{
	return;
}

CSkillHitBox::~CSkillHitBox()
{
	return;
}

void CSkillHitBox::Init()
{
	memset(this->m_Table, 0, sizeof(this->m_Table));
}


int CSkillHitBox::Check(int dir, int x, int y)
{
	if ( x <0 || x >= 21 || y < 0 || y >= 21 || dir < 0 || dir >= 36 )
	{
		return 0;
	}

	return this->m_Table[dir][y * 21 + x];
}



int CSkillHitBox::HitCheck(int dir, int x, int y, int tx, int ty)
{
	dir /= 10;

	if ( dir < 0 || dir >= 36 )
	{
		return 0;
	}

	int targetx = tx - x;
	int targety = ty - y;

	targetx += 10;
	targety += 10;

	if ( targetx <0 || abs(targetx) >= 21 || targety <0 || abs(targety) >= 21 )
	{
		return 0;
	}

	return this->m_Table[dir][targety * 21 + targetx];
}

BOOL CSkillHitBox::Load(char * filename )
{
	if(!IsFile(filename))
	{
		g_Log.MsgBox("%s file load fail", filename);
		return FALSE;
	}

	std::streampos fileSize;
	std::ifstream file(filename, std::ios::binary);

	file.seekg(0, std::ios::end);
	fileSize = file.tellg();
	file.seekg(0, std::ios::beg);

	if ((int)fileSize != sizeof(this->m_Table))
	{
		g_Log.MsgBox("Wrong file size - %s", filename);
		return FALSE;
	}

	file.read((char *)this->m_Table, fileSize);

	file.close();

	g_Log.Add("%s file loaded", filename);
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

