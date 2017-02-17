#include "stdafx.h"
#include "MonsterSetBase.h"
#include "GameMain.h"
#include "MapServerManager.h"
#include "TLog.h"
#include "LargeRand.h"
#include "configread.h"
// GS-N 0.99.60T 0x0041AB30 - Completed
//	GS-N	1.00.18	JPN	0x00421BB0	-	Completed

CMonsterSetBase::CMonsterSetBase()
{
	return;
}

CMonsterSetBase::~CMonsterSetBase()
{
	return;
}

void CMonsterSetBase::Init()
{
	this->m_Mp = new MONSTER_POSITION[g_ConfigRead.server.GetObjectMaxMonster()];
}

void CMonsterSetBase::Delete()
{
	delete [] this->m_Mp;
}

void CMonsterSetBase::SetBoxPosition(int TableNum, int mapnumber, int ax, int ay, int aw, int ah)
{
	this->m_Mp[TableNum].m_MapNumber = mapnumber;
	this->m_Mp[TableNum].m_W  = aw;
	this->m_Mp[TableNum].m_H  = ah;
	this->m_Mp[TableNum].m_X  = ax;
	this->m_Mp[TableNum].m_Y  = ay;
}

BOOL CMonsterSetBase::GetBoxPosition(int mapnumber, int ax, int ay, int aw, int ah, short &mx, short &my)
{
	int count = 100 ; 
	int w;
	int h;
	int tx; 
	int ty;
	BYTE attr;

	while ( count-- != 0)
	{
		w = aw - ax;
		h = ah - ay;

		__try
		{
			tx = ax + (rand()%w);
			ty = ay + (rand()%h);
		}
		__except (w=1,h=1,1)
		{

		}

		attr = MapC[mapnumber].GetAttr(tx, ty);

		if ( ((attr&1) != 1) && ((attr &4) != 4) && ((attr&8) != 8) )
		{
			mx = tx;
			my = ty;
			return TRUE;
		}
	}

	return false;
}

BOOL CMonsterSetBase::GetPosition(int TableNum, short MapNumber, short & x, short & y)
{
	int count = 100;
	BYTE attr;
	int tx;
	int ty;
	int w;
	int h;

	if ( TableNum < 0 || TableNum > g_ConfigRead.server.GetObjectMaxMonster()-1 )
	{
		return false;
	}

	if ( this->m_Mp[TableNum].m_ArrangeType == Arrange_MultiMonsterSpawn || this->m_Mp[TableNum].m_ArrangeType == Arrange_ElementalMonsterMultiSpawn )
	{
		while ( count-- != 0 )
		{
			w = this->m_Mp[TableNum].m_W - this->m_Mp[TableNum].m_X;
			h = this->m_Mp[TableNum].m_H - this->m_Mp[TableNum].m_Y;

			if ( w < 1 )
			{
				w = 1;
			}

			if ( h < 1 )
			{
				h = 1;
			}

			int iRX = GetLargeRand()%w;
			int iRY = GetLargeRand()%h;

			tx = this->m_Mp[TableNum].m_X +  iRX;
			ty = this->m_Mp[TableNum].m_Y +  iRY;
			attr = MapC[MapNumber].GetAttr(tx, ty);

			if ( ((attr&1) != 1) && ((attr &4) != 4) && ((attr&8) != 8) )
			{
				x = tx;
				y = ty;
				return TRUE;
			}
		}
	}

	else if ( this->m_Mp[TableNum].m_ArrangeType == Arrange_NpcSpawn )
	{
		x = this->m_Mp[TableNum].m_X;
		y = this->m_Mp[TableNum].m_Y;
		return true;
	}

	else if ( this->m_Mp[TableNum].m_ArrangeType == Arrange_SingleMonsterSpawn )
	{
		while ( count-- != 0 )
		{
			int Orx = this->m_Mp[TableNum].m_X -3;
			int Ory = this->m_Mp[TableNum].m_Y -3;

			Orx += rand() % 7;
			Ory += rand() % 7;

			attr = MapC[MapNumber].GetAttr(Orx, Ory);

			if ( ((attr&1) != 1) && ((attr &4) != 4) && ((attr&8) != 8) )
			{
				x = Orx;
				y = Ory;
				return TRUE;
			}
		}
	}

	return false;
}

void CMonsterSetBase::LoadSetBase(char * filename)
{
	int rcount;
	int copycount;
	BYTE Sdir;

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(filename);

	if (res.status != pugi::status_ok)
	{
		g_Log.MsgBox("file open error %s (%s)", filename, res.description());
		return;
	}

	this->m_Count = 0;

	pugi::xml_node main = file.child("MonsterSpawn");

	for (pugi::xml_node map = main.child("Map"); map; map = map.next_sibling())
	{
		int iMapNumber = map.attribute("Number").as_int();

		if (MAX_MAP_RANGE(iMapNumber) == false)
		{
			continue;
		}

		for (pugi::xml_node spot = map.child("Spot"); spot; spot = spot.next_sibling())
		{
			int iSpotType = spot.attribute("Type").as_int();

			if (iSpotType < Arrange_NpcSpawn || iSpotType > Arrange_ElementalMonsterMultiSpawn)
			{
				g_Log.MsgBox("Error in %s file: Wrong Spot Type: %d", filename, iSpotType);
				continue;
			}

			for (pugi::xml_node spawn = spot.child("Spawn"); spawn; spawn = spawn.next_sibling())
			{
				this->m_Mp[this->m_Count].m_Type = spawn.attribute("Index").as_int();

				if (this->m_Mp[this->m_Count].m_Type == (WORD)-1)
				{
					continue;
				}

				this->m_Mp[this->m_Count].m_MapNumber = iMapNumber;
				this->m_Mp[this->m_Count].m_Dis = spawn.attribute("Distance").as_int();
				this->m_Mp[this->m_Count].m_X = spawn.attribute("StartX").as_int();
				this->m_Mp[this->m_Count].m_Y = spawn.attribute("StartY").as_int();
				this->m_Mp[this->m_Count].m_ArrangeType = iSpotType;

				if (iSpotType == Arrange_MultiMonsterSpawn || iSpotType == Arrange_ElementalMonsterMultiSpawn)
				{
					this->m_Mp[this->m_Count].m_W = spawn.attribute("EndX").as_int();
					this->m_Mp[this->m_Count].m_H = spawn.attribute("EndY").as_int();
				}

				else if (iSpotType == Arrange_SingleMonsterSpawn)
				{
					this->m_Mp[this->m_Count].m_W = 0;
					this->m_Mp[this->m_Count].m_H = 0;

					int w = this->m_Mp[this->m_Count].m_X - 3;
					int h = this->m_Mp[this->m_Count].m_Y - 3;

					w += rand() % 7;
					h += rand() % 7;

					this->m_Mp[this->m_Count].m_X = w;
					this->m_Mp[this->m_Count].m_Y = h;
				}

				else
				{
					this->m_Mp[this->m_Count].m_W = 0;
					this->m_Mp[this->m_Count].m_H = 0;
				}

				this->m_Mp[this->m_Count].m_Dir = spawn.attribute("Dir").as_int();
				Sdir = this->m_Mp[this->m_Count].m_Dir;

				if (this->m_Mp[this->m_Count].m_Dir == (BYTE)-1)
					this->m_Mp[this->m_Count].m_Dir = rand() % 8;

				if (iSpotType == Arrange_MultiMonsterSpawn || iSpotType == Arrange_ElementalMonsterMultiSpawn)
				{
					rcount = spawn.attribute("Count").as_int();
					copycount = this->m_Count;

					if (iSpotType == Arrange_ElementalMonsterMultiSpawn)
					{
						this->m_Mp[this->m_Count].m_PentagramMainAttribute = spawn.attribute("Element").as_int();
					}

					if (rcount > 1)
					{
						for (int k = 0; k < rcount - 1; k++)
						{
							if (g_MapServerManager.CheckMapCanMove(this->m_Mp[copycount].m_MapNumber) == FALSE)
								break;

							this->m_Count++;

							if (this->m_Count > g_ConfigRead.server.GetObjectMaxMonster() - 1)
							{
								g_Log.MsgBox("Monster attribute max over %s %d", __FILE__, __LINE__);
								return;
							}

							this->m_Mp[this->m_Count].m_Dis = this->m_Mp[copycount].m_Dis;
							this->m_Mp[this->m_Count].m_Type = this->m_Mp[copycount].m_Type;
							this->m_Mp[this->m_Count].m_MapNumber = this->m_Mp[copycount].m_MapNumber;
							this->m_Mp[this->m_Count].m_W = this->m_Mp[copycount].m_W;
							this->m_Mp[this->m_Count].m_H = this->m_Mp[copycount].m_H;
							this->m_Mp[this->m_Count].m_X = this->m_Mp[copycount].m_X;
							this->m_Mp[this->m_Count].m_Y = this->m_Mp[copycount].m_Y;
							this->m_Mp[this->m_Count].m_ArrangeType = this->m_Mp[copycount].m_ArrangeType;

							if (Sdir == (BYTE)-1)
								this->m_Mp[this->m_Count].m_Dir = rand() % 8;
							else
								this->m_Mp[this->m_Count].m_Dir = this->m_Mp[copycount].m_Dir;

							if (iSpotType == Arrange_ElementalMonsterMultiSpawn)
							{
								this->m_Mp[this->m_Count].m_PentagramMainAttribute = this->m_Mp[copycount].m_PentagramMainAttribute;
							}
						}
					}
				}

				if (g_MapServerManager.CheckMapCanMove(this->m_Mp[this->m_Count].m_MapNumber) == FALSE)
					continue;

				this->m_Count++;

				if (this->m_Count > g_ConfigRead.server.GetObjectMaxMonster() - 1)
				{
					g_Log.MsgBox("Monster attribute max over %d (%s %d)", this->m_Count, __FILE__, __LINE__);
					return;
				}
			}
		}
	}
}

void CMonsterSetBase::GetPentagramMainAttribute(int TableNum, int* iPentagramMainAtrribute)
{
	if ( TableNum < 0 || TableNum > g_ConfigRead.server.GetObjectMaxMonster()-1 )
	{
		g_Log.Add("ERROR : %s %d", ".\\MonsterSetBase.cpp", __LINE__);
		return;
	}

	if ( this->m_Mp[TableNum].m_PentagramMainAttribute > 0 )
	{
		*iPentagramMainAtrribute = this->m_Mp[TableNum].m_PentagramMainAttribute;
	}
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

