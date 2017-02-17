//////////////////////////////////////////////////////////////////////
// MapClass.h
// ------------------------------
// Decompiled by Deathway
// Date : 2007-03-09
// ------------------------------
#ifndef MAPCLASS_H
#define MAPCLASS_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "zzzpath.h"
#include "MapItem.h"


#define MAX_NUMBER_MAP 103

#define MAX_MAP_RANGE(x) (((x)<0)?FALSE:((x)>MAX_NUMBER_MAP-1)?FALSE:TRUE)

extern inline bool MaxItemTypeRange(int x);

class MapClass
{

public:

	//void __thiscall IndexAutoPlus();
	void SaveItemInfo();
	void GetLevelPos( short level, short& ox, short& oy);
	void GetMapPos(short Map, short& ox, short& oy);
	void GetRandomLengthPos(short& x, short& y, int length);
	void SearchStandAttr(short& x, short& y);
	BOOL AttrLoad(char* filename);
	void LoadMapAttr(char* filename, int MapNumber);
	BOOL CheckWall(int sx1, int sy1, int sx2, int sy2);
	BYTE CheckWall2(int sx1, int sy1, int sx2, int sy2);
	void ItemInit();
	int ItemGive(int aIndex, int item_num, bool bFailNotSend);
	int ItemDrop(int type, int level, float dur, int x, int y, BYTE Option1, BYTE Option2, BYTE Option3, BYTE NOption, BYTE SOption, UINT64 number, int aIndex, int PetLevel, UINT64 PetExp, BYTE ItemEffectEx, BYTE *SocketOption, BYTE SocketBonusOption, DWORD PeriodDuration);
	int MonsterItemDrop(int type, int level, float dur, int x, int y, BYTE Option1, BYTE Option2, BYTE Option3, BYTE NOption, BYTE SOption, int aIndex, UINT64 number, BYTE ItemEffectEx, BYTE *SocketOption, BYTE SocketBonusOption, DWORD PeriodDuration);
	int MoneyItemDrop(int money, int x, int y);
	void WeatherVariationProcess();
	BYTE GetWeather();
	void SetWeather(BYTE a_weather, BYTE a_variation);
	void StateSetDestroy();
	bool PathFinding2(int sx, int sy, int tx, int ty, PATH_t* a);
	bool PathFinding3(int sx, int sy, int tx, int ty, PATH_t* a);
	bool PathFinding4(int sx, int sy, int tx, int ty, PATH_t* a);
	void SetStandAttr(int x, int y);
	void ClearStandAttr(int x, int y);
	int GetStandAttr(int x, int y);
	BYTE GetAttr(int x, int y);

	void init()	// line : 129
	{
		this->m_attrbuf = 0;
		this->path = new PATH;
		this->m_width = 0;
		this->m_height = 0;
		this->m_cItem = 0;
	}	// line : 134
	
	void free()	// line : 136
	{
		if ( this->m_attrbuf != 0 )
		{
			GlobalFree(this->m_attrbuf);
			this->m_attrbuf=0;
		}

		if ( this->path != 0 )
		{
			delete this->path;
			this->path = 0;
		}

		if (this->m_cItem != 0)
		{
			delete [] this->m_cItem;
			this->m_cItem = NULL;
		}
	}	// line : 147

	MapClass();
	virtual ~MapClass();

public:

	BYTE m_Weather;	// 4
	BYTE m_WeatherVariation;	// 5
	DWORD m_WeatherTimer;	// 8
	DWORD m_NextWeatherTimer;	// C
	LPBYTE m_attrbuf;	// 10
	int m_width;	// 14
	int m_height;	// 18
	PATH* path;	// 1C
	class CMapItem * m_cItem;	// 20
	int m_ItemCount;	// 50DE0
	RECT gRegenRect[MAX_NUMBER_MAP];	// 50DE4
	int thisMapNumber;	// 51064
};

BOOL MapNumberCheck(int map);

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

