// ------------------------------
// Decompiled by Deathway
// Date : 2007-03-09
// ------------------------------
// Gate.cpp
// GS-N 0.99.60T 0x004FF2E0
//	GS-N	1.00.18	JPN	0x0052F200	-	Completed

#include "stdafx.h"
#include "gate.h"
#include "TLog.h"
#include "GameMain.h"

CGate gGateC;

CGate::CGate ()
{
	return;
}

CGate::~CGate()
{
	return;
}

void CGate::Init()
{
	int n;

	for (n=0;n<MAX_GATES;n++)
	{
		this->m_This[n] = -1;
		this->m_MinLevel[n] = 0;
	}
}

void CGate::Load(char* filename)
{
	this->Init();

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(filename);

	if (res.status != pugi::status_ok)
	{
		g_Log.MsgBox("%s file not found - Please rename IGC_GateSettings.xml.s6 or IGC_GateSettings.xml.s8 to IGC_GateSettings.xml in your Data folder according to server version", filename);
		ExitProcess(1);
	}

	pugi::xml_node gatesettings = file.child("GateSettings");

	int number;

	for (pugi::xml_node gate = gatesettings.child("Gate"); gate; gate = gate.next_sibling())
	{
		number = gate.attribute("Index").as_int();

		this->m_This[number] = number;
		this->m_Flag[number] = gate.attribute("Flag").as_int();
		this->m_MapNumber[number] = gate.attribute("MapNumber").as_int();
		this->m_Sx[number] = gate.attribute("StartX").as_int();
		this->m_Sy[number] = gate.attribute("StartY").as_int();
		this->m_Ex[number] = gate.attribute("EndX").as_int();
		this->m_Ey[number] = gate.attribute("EndY").as_int();
		this->m_TargetGate[number] = gate.attribute("Target").as_int();
		this->m_Dir[number] = gate.attribute("Direction").as_int();
		this->m_MinLevel[number] = gate.attribute("MinLevel").as_int();
	}

	g_Log.Add("[%s] Gate information data load complete.", filename);
}

BOOL CGate::IsGate(int GateNumber)
{
	if ( (GateNumber < 0) || (GateNumber > MAX_GATES-1) )
	{
		g_Log.Add("Gate information exceeds  %d (%s %d)", GateNumber, __FILE__, __LINE__);
		return 0;
	}

	if ( this->m_This[GateNumber] == (BYTE)-1 )
	{
		g_Log.Add("Gate information error: -1 (%s %d)", __FILE__, __LINE__);
		return 0;
	}

	return 1;
}


int CGate::GetGate(int mgt, short& x, short& y, BYTE& MapNumber, BYTE& dir, short& Level)
{
	int gt;
	int tx;
	int ty;
	int loopcount;
	BYTE attr;
	
	if (this->IsGate(mgt) == 0)
	{
		return -1;
	}

	if (!MAX_MAP_RANGE(MapNumber))
	{
		return -1;
	}

	gt=this->m_TargetGate[mgt];
	if ( gt == 0 )
	{
		gt=mgt;	// This is for M Key
	}
	
	loopcount=10;
	while ( loopcount-- != 0 )
	{
		// For X coords
		if ( (this->m_Ex[gt] - this->m_Sx[gt]) > 0 )
		{
			tx = this->m_Sx[gt] + ( rand() % ( this->m_Ex[gt] - this->m_Sx[gt] ) );	// Choose any X position between XBegin and XEnd
		}
		else
		{
			tx = this->m_Sx[gt];	// If there is amistake while putting values in gate.txt
		}
		// For Y coords
		if ( (this->m_Ey[gt] - this->m_Sy[gt]) > 0 )
		{
			ty = this->m_Sy[gt] + ( rand() % ( this->m_Ey[gt] - this->m_Sy[gt] ) );	// Choose any Y position between YBegin and YEnd
		}
		else
		{
			ty = this->m_Sy[gt];	// If there is amistake while putting values in gate.txt
		}

		attr = MapC[MapNumber].GetAttr( tx, ty);

		if ( (attr&4) != 4 && (attr&2) != 2 && (attr&8) != 8 )
		{
			break;
		}
	}

	x = tx;
	y = ty;
	MapNumber = this->m_MapNumber[gt];
	dir = this->m_Dir[gt];
	Level = this->m_MinLevel[gt];
	return gt;

}
			

int CGate::GetLevel(int GateNumber)
{
	if (GateNumber < 0 || GateNumber > MAX_GATES-1 )
	{
		g_Log.Add("Gate information exceeds  %d (%s %d)", GateNumber, __FILE__, __LINE__);
		return -1;
	}

	return this->m_MinLevel[GateNumber];
}


BOOL CGate::IsInGate(int aIndex, int GateNumber)
{
	LPOBJ lpObj = &gObj[aIndex];

	if ( this->IsGate(GateNumber) == FALSE )
	{
		return false;
	}

	int minlevel = this->m_MinLevel[GateNumber];

	if ( GateNumber != 273 && (lpObj->Class == 4 || lpObj->Class == 3 || lpObj->Class == 6) )
	{
		if (minlevel > 0 )
		{
			minlevel = minlevel / 3 * 2;
		}
	}

	if (lpObj->Level < minlevel)
	{
		return FALSE;
	}

	if ( lpObj->MapNumber != this->m_MapNumber[GateNumber] )
	{
		return FALSE;
	}

	if ( lpObj->X < this->m_Sx[GateNumber]-5 || lpObj->X > this->m_Ex[GateNumber]+5 || lpObj->Y < this->m_Sy[GateNumber]-5 || lpObj->Y > this->m_Ey[GateNumber]+5 )
	{
		return FALSE;
	}

	return true;
}

BOOL CGate::CheckGateLevel(int aIndex, int GateNumber)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (this->IsGate(GateNumber) == FALSE)
	{
		return false;
	}

	int minlevel = this->m_MinLevel[GateNumber];

	if (GateNumber != 273 && (lpObj->Class == 4 || lpObj->Class == 3 || lpObj->Class == 6))
	{
		if (minlevel > 0)
		{
			minlevel = (minlevel / 3) * 2;
		}
	}

	if (lpObj->Level < minlevel)
	{
		return false;
	}

	return true;
}


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

