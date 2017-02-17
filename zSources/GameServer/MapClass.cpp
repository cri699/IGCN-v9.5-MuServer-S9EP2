//////////////////////////////////////////////////////////////////////
// MapClass.cpp
// ------------------------------
// Decompiled by Deathway
// Date : 2007-03-09
// ------------------------------
#include "stdafx.h"
#include "MapClass.h"
#include "TLog.h"
#include "GameMain.h"
#include "user.h"
#include "Protocol.h"
#include "BloodCastle.h"
#include "IllusionTempleEvent_Renewal.h"
#include "configread.h"

// GS-N 0.99.60T 0x00478170 - Completed
//	GS-N	1.00.18	JPN	0x0048F140	-	Completed

inline bool MaxItemTypeRange(int x)
{
	if (x < 0 || x > g_ConfigRead.server.GetObjectMaxItem() - 1)
	{
		return false;
	}

	return true;
}

MapClass::MapClass()
{
	this->m_Weather =0;
	this->m_WeatherTimer=GetTickCount();
	this->m_NextWeatherTimer=(rand()%10000)+10000;
	this->init();
	
	SetRect(&this->gRegenRect[0], 130, 116, 151, 137);
	SetRect(&this->gRegenRect[1], 106, 236, 112, 243);
	SetRect(&this->gRegenRect[2], 197, 35, 218, 50);
	SetRect(&this->gRegenRect[3], 174, 101, 187, 125);
	SetRect(&this->gRegenRect[4], 201, 70, 213, 81);
	SetRect(&this->gRegenRect[5], 89, 135, 90, 136);
	SetRect(&this->gRegenRect[6], 89, 135, 90, 136);
	SetRect(&this->gRegenRect[7], 14, 11, 27, 23);
	SetRect(&this->gRegenRect[8], 187, 54, 203, 69);
	SetRect(&this->gRegenRect[33], 82, 8, 87, 14);
	SetRect(&this->gRegenRect[34], 133, 41, 140, 44);
	SetRect(&this->gRegenRect[51], 40, 214, 43, 224);
	SetRect(&this->gRegenRect[79], 126, 142, 129, 148);
	this->m_ItemCount=0;
}



MapClass::~MapClass()
{
	this->free();
}



BOOL MapNumberCheck(int map)
{
	if ( map < 0 || map > MAX_NUMBER_MAP-1 )
	{
		return FALSE;
	}
	return TRUE;
}



void MapClass::GetLevelPos( short level, short& ox, short& oy)
{

	int x;
	int y;
	int levelpos = 0;
	int loopcount = 50;
	BYTE attr;
	int w;
	int h;

	while ( loopcount-- != 0 )
	{
		w = this->gRegenRect[levelpos].right - this->gRegenRect[levelpos].left;
		h = this->gRegenRect[levelpos].bottom - this->gRegenRect[levelpos].top;

		__try
		{
			x = (short)(this->gRegenRect[levelpos].left + (rand()%w));
			y = (short)(this->gRegenRect[levelpos].top  + (rand()%h));
		}
		__except(w=1, h=1,1)
		{
		}

		attr = this->GetAttr(x, y);

		if ( (attr & 4) != 4  && (attr & 8) != 8 ) 
		{
			ox = x;
			oy = y;
			return;
		}
	}
	
	g_Log.Add("Error: Cannot find the locatin %s %d", __FILE__, __LINE__);
	x = this->gRegenRect[levelpos].left;
	y = this->gRegenRect[levelpos].top;
}




void MapClass::GetRandomLengthPos(short& x, short& y, int length)
{
	int lc = 10;	// loopcount
	int px;
	int py;
	BYTE attr;
	int maxlen;

	if ( length < 1 )
	{
		length = 1;
	}

	maxlen = length*2;

	while ( lc-- != 0 )
	{
		px = (x-length)+(rand()%maxlen);
		py = (y-length)+(rand()%maxlen);

		attr = this->GetAttr(px, py);

		if ( (attr & 4) != 4  && (attr & 8) != 8 )
		{
			x = px;
			y = py;
			return;
		}
	}
}



void MapClass::GetMapPos(short Map, short& ox, short& oy)
{
	if ( Map != 8 )
	{
		if ( Map == 33 )
		{
			Map = 33;
		}
		else if ( Map == 34 )
		{
			Map = 34;
		}
		else if ( Map == 41 )
		{
			Map = 41;
		}
		else if ( Map == 42 )
		{
			Map = 42;
		}
		else if ( Map == 51 )
		{
			Map = 51;
		}
		else if ( Map > 4 && Map != 7 )
		{
			Map = 0;
		}
	}

	int x;
	int y;
	int levelpos = Map;
	int loopcount=50;
	BYTE attr;
	int w;
	int h;

	while ( loopcount-- != 0 )
	{
		w = this->gRegenRect[levelpos].right - this->gRegenRect[levelpos].left;
		h = this->gRegenRect[levelpos].bottom - this->gRegenRect[levelpos].top;

		__try
		{
			x = (short)(this->gRegenRect[levelpos].left + (rand()%w));
			y = (short)(this->gRegenRect[levelpos].top  + (rand()%h));
		}
		__except(w=1,h=1,1)
		{
		}
		attr = this->GetAttr(x, y);

		if ( (attr & 4) != 4  && (attr & 8) != 8 )
		{
			ox = x;
			oy = y;
			return;
		}
	}
				
	g_Log.Add("Error: Cannot find the locatin %s %d", __FILE__, __LINE__);
	x = this->gRegenRect[levelpos].left;
	y = this->gRegenRect[levelpos].top;
}


void MapClass::LoadMapAttr(char * filename, int MapNumber)
{
	this->thisMapNumber = MapNumber;

	if (this->AttrLoad(filename) == FALSE )
	{
		g_Log.MsgBox("Map attribute reading error");
		g_Log.AddC(TColor::Red, "[Maps] Error: %s", filename);
	}
}



BYTE MapClass::GetWeather()
{
	BYTE weather = this->m_Weather *16;
	weather |= this->m_WeatherVariation ;
	return weather;
}



void MapClass::SetWeather(BYTE a_weather, BYTE a_variation)
{
	LPOBJ lpObj;
	BYTE weather;
	int n;

	this->m_Weather = a_weather;
	this->m_WeatherVariation = a_variation;
	weather = this->m_Weather * 16;	// like << 4
	weather |= this->m_WeatherVariation;
	
	for ( n= g_ConfigRead.server.GetObjectStartUserIndex() ; n<g_ConfigRead.server.GetObjectMax() ; n++ )
	{
		lpObj = &gObj[n];

		if ( lpObj->Connected > PLAYER_CONNECTED && lpObj->Live != 0 && lpObj->MapNumber == this->thisMapNumber )
		{
			GSProtocol.CGWeatherSend(n, weather);
		}
	}
}



void MapClass::WeatherVariationProcess()
{
	LPOBJ lpObj;
	BYTE weather;

	if ( (GetTickCount() - this->m_WeatherTimer) > this->m_NextWeatherTimer)
	{
		this->m_WeatherTimer = GetTickCount();
		this->m_NextWeatherTimer = (rand()%10000) + 10000;
		this->m_WeatherVariation = rand()%10;
		this->m_Weather = rand()%3;

		weather = this->m_Weather*16;
		weather |= this->m_WeatherVariation;

		for (int n= g_ConfigRead.server.GetObjectStartUserIndex() ; n<g_ConfigRead.server.GetObjectMax() ; n++ )
		{
			lpObj = &gObj[n];

			if ( lpObj->Connected > PLAYER_CONNECTED && lpObj->Live != 0 && lpObj->MapNumber == this->thisMapNumber )
			{
				GSProtocol.CGWeatherSend(n, weather);
			}
		}
	}
}



void MapClass::SaveItemInfo()
{
	return;	// Here goes a Macro
}



void MapClass::ItemInit()
{
	if (this->m_cItem != 0)
	{
		delete this->m_cItem;
		this->m_cItem = 0;
	}

	this->m_cItem = new CMapItem[g_ConfigRead.server.GetObjectMaxItem()];

	if (this->m_cItem == NULL)
	{
		g_Log.MsgBox("ERROR - MEMORY ALLOCATION FAILED");
		ExitProcess(1);
	}
}



int MapClass::MonsterItemDrop(int type, int level, float dur, int x, int y, BYTE Option1, BYTE Option2, BYTE Option3, BYTE NOption, BYTE SOption, int aIndex, UINT64 number, BYTE ItemEffectEx, BYTE *SocketOption, BYTE SocketBonusOption, DWORD PeriodDuration)
{
	int count;
	int counttot=0;
	BYTE attr = this->GetAttr(x, y);

	if ( (attr & 4) == 4  || (attr & 8) == 8 )
	{
		return -1;
	}
	count = this->m_ItemCount;

	while ( true )
	{
		if ( this->m_cItem[count].IsItem() == FALSE )
		{
			this->m_cItem[count].CreateItem(type, level, x, y, dur, Option1, Option2, Option3, NOption, SOption, number, ItemEffectEx, SocketOption, SocketBonusOption);
			this->m_cItem[count].m_UserIndex = aIndex;
			this->m_cItem[count].m_PeriodItemDuration = PeriodDuration;
			this->m_ItemCount++;

			if (this->m_ItemCount > g_ConfigRead.server.GetObjectMaxItem() - 1)
			{
				this->m_ItemCount = 0;
			}

			return count;

		}

		count++;

		if (count > g_ConfigRead.server.GetObjectMaxItem() - 1)
		{
			count = 0;
		}

		counttot++;

		if (counttot > g_ConfigRead.server.GetObjectMaxItem() - 1)
		{
			break;
		}
	}

	g_Log.Add("Error: Items limit on Map reached, check MapItemCount and disappear time settings, %s %d", __FILE__, __LINE__ );

	return -1;
}





BOOL MapClass::ItemDrop(int type, int level, float dur, int x, int y, BYTE Option1, BYTE Option2, BYTE Option3, BYTE NOption, BYTE SOption, UINT64 number, int aIndex, int PetLevel, UINT64 PetExp, BYTE ItemEffectEx, BYTE *SocketOption, BYTE SocketBonusOption, DWORD PeriodDuration)
{
	int count;
	int counttot = 0;
	BYTE attr = this->GetAttr(x, y);

	if ( (attr&4) == 4 || (attr&8) == 8 )
	{
		return FALSE;
	}

	count = this->m_ItemCount;

	while ( true )
	{
		if ( this->m_cItem[count].IsItem() == FALSE )
		{
			this->m_cItem[count].DropCreateItem(type, level, x, y, dur, Option1, Option2, Option3, NOption, SOption, number, PetLevel, PetExp, ItemEffectEx, SocketOption, SocketBonusOption);
			this->m_cItem[count].m_UserIndex = aIndex;
			this->m_cItem[count].m_PeriodItemDuration = PeriodDuration;
			this->m_ItemCount++;

			if (this->m_ItemCount > g_ConfigRead.server.GetObjectMaxItem() - 1)
			{
				this->m_ItemCount = 0;
			}

			if ( BC_MAP_RANGE(this->thisMapNumber) != FALSE )
			{
				if ( type == ITEMGET(13,19) )
				{
					if ( ((level<0)?FALSE:(level>2)?FALSE:TRUE) != FALSE )
					{
						int BloodCastleIndex = g_BloodCastle.GetBridgeIndex(this->thisMapNumber);
						g_BloodCastle.ThrowQuestItemByUser(BloodCastleIndex, number);
					}
				}
			}

			return TRUE;
		}

		count++;

		if (count > g_ConfigRead.server.GetObjectMaxItem() - 1)
		{
			count = 0;
		}

		counttot++;

		if (counttot > g_ConfigRead.server.GetObjectMaxItem() - 1)
		{
			return FALSE;
		}
	}

	g_Log.Add("Error: Items limit on Map reached, check MapItemCount and disappear time settings, %s %d", __FILE__, __LINE__ );

	return FALSE;
}





BOOL MapClass::MoneyItemDrop(int money, int x, int y)
{
	int count;
	int counttot = 0;

	BYTE attr = this->GetAttr(x, y);

	if ( (attr&4) == 4 || (attr&8) == 8 )
	{
		return FALSE;
	}

	count = this->m_ItemCount;

	while ( true )
	{
		if ( this->m_cItem[count].IsItem() == FALSE )
		{
			this->m_cItem[count].m_Type = ITEMGET(14,15);
			this->m_cItem[count].m_SellMoney = money;
			this->m_cItem[count].m_BuyMoney = money;
			this->m_cItem[count].px = x;
			this->m_cItem[count].py = y;
			this->m_cItem[count].live = 1;
			this->m_cItem[count].Give = 0;
			this->m_cItem[count].m_State = 1;
			this->m_cItem[count].m_Time = GetTickCount() + ::gZenDurationTime * 1000;
			this->m_cItem[count].m_LootTime = 0;
			this->m_ItemCount++;

			if (this->m_ItemCount > g_ConfigRead.server.GetObjectMaxItem() - 1)
			{
				this->m_ItemCount = 0;
			}

			return TRUE;
		}

		count++;

		if (count > g_ConfigRead.server.GetObjectMaxItem() - 1)
		{
			count = 0;
		}

		counttot++;

		if (counttot > g_ConfigRead.server.GetObjectMaxItem() - 1)
		{
			return FALSE;
		}
	}

	return FALSE;
}



BOOL MapClass::ItemGive(int aIndex, int item_num, bool bFailNotSend)
{
	if (((item_num<0) ? FALSE : (item_num>g_ConfigRead.server.GetObjectMaxItem() - 1) ? FALSE : TRUE) == FALSE)
	{
		g_Log.Add("Error: Item number error %s %d", __FILE__, __LINE__);
		return FALSE;
	}

	if ( gObj[aIndex].MapNumber != this->thisMapNumber )
	{
		g_Log.Add("Map Number error %s %d (%d) %s", __FILE__, __LINE__, gObj[aIndex].MapNumber, gObj[aIndex].Name);
		return FALSE;
	}

	if ( this->m_cItem[item_num].IsItem() == FALSE )
	{
		g_Log.Add("Error: Item does not exist %s %d %s", __FILE__, __LINE__, gObj[aIndex].Name);	// Apply Deathway Fix herw
		return FALSE;
	}

	if ( this->m_cItem[item_num].Give == true )
	{
		return FALSE;
	}

	if ( this->m_cItem[item_num].live == false )
	{
		return FALSE;
	}

	int disx = this->m_cItem[item_num].px - gObj[aIndex].X;
	int disy = this->m_cItem[item_num].py - gObj[aIndex].Y;

	if(gObj[aIndex].m_bOffLevel == false)
	{
		if ( disx > 2 || disx < -2 )
		{
			return FALSE;
		}

		if ( disy > 2 || disy < -2 )
		{
			return FALSE;
		}
	}

	int lootresult = 1;

	if ( gLootingTime > 0 )
	{
		if ( this->m_cItem[item_num].m_UserIndex != -1 )
		{
			if ( GetTickCount() < this->m_cItem[item_num].m_LootTime )
			{
				if ( aIndex != this->m_cItem[item_num].m_UserIndex )
				{
					lootresult = 0;

					if ( this->m_cItem[item_num].m_QuestItem == false )
					{
						if ( gObj[aIndex].PartyNumber >= 0 )
						{
							if ( gObj[aIndex].PartyNumber == gObj[this->m_cItem[item_num].m_UserIndex].PartyNumber )
							{
								if ( BC_MAP_RANGE(gObj[aIndex].MapNumber) != FALSE )
								{
									if ( this->m_cItem[item_num].m_Type == ITEMGET(12,15) ||( this->m_cItem[item_num].m_Type == ITEMGET(13,19) && ((this->m_cItem[item_num].m_Level < 0)?FALSE:(this->m_cItem[item_num].m_Level > 2)?FALSE:TRUE) != FALSE ) )
									{
										lootresult = 0;
									}
									else
									{
										lootresult = 1;
									}
								}
								else
								{
									lootresult = 1;
								}
							}
						}
					}
				}
			}
		}
	}

	if ( lootresult == 0 )
	{
		if ( bFailNotSend == false )
		{
			char szTemp[256];

			wsprintf(szTemp, Lang.GetText(0,60), gObj[aIndex].Name );
			GSProtocol.GCServerMsgStringSend(szTemp, aIndex, 1);
			
		}

		return FALSE;
	}
	else
	{
		this->m_cItem[item_num].m_State = 8;
		this->m_cItem[item_num].Give = true;
		this->m_cItem[item_num].live = false;

		return TRUE;
	}
}



void MapClass::StateSetDestroy()
{
	int CurTime = GetTickCount();

	for (int n = 0; n<g_ConfigRead.server.GetObjectMaxItem(); n++)
	{
		if ( this->m_cItem[n].IsItem() != FALSE )
		{
			if ( this->m_cItem[n].m_State == 1 )
			{
				this->m_cItem[n].m_State = 2;
			}
			else if ( this->m_cItem[n].m_State == 8 )
			{
				this->m_cItem[n].Clear();
				this->m_cItem[n].live = 0;
				this->m_cItem[n].Give = 1;
			}
		}

		if ( this->m_cItem[n].IsItem() != FALSE )
		{
			if ( this->m_cItem[n].m_State != 8 )
			{
				if ( CurTime > this->m_cItem[n].m_Time )
				{
					g_Log.Add("Item has disappeared (%s:%d / Level: %d / Skill: %d / Serial: %I64d)", this->m_cItem[n].GetName(), this->m_cItem[n].m_Type, this->m_cItem[n].m_Level, this->m_cItem[n].m_Special[0] , this->m_cItem[n].m_Number);
					this->m_cItem[n].m_State = 8;

					if (this->m_cItem[n].m_Type == ITEMGET(14, 223))
					{
						g_IT_Event.SetStatusRegenTime(this->thisMapNumber);
					}
				}
			}
		}
	}
}



BOOL MapClass::AttrLoad(char* filename)
{
	FILE * fp;
	unsigned char head;

	fp = fopen(filename,"rb");

	if ( fp == NULL )
	{
		return false;
	}
	
	fread(&head, sizeof(head), 1, fp);
	fread(&this->m_width, sizeof(char), 1, fp);
	fread(&this->m_height, sizeof(char), 1, fp);

	if ( this->m_width > 255 || this->m_height > 255 )
	{
		g_Log.MsgBox("Attribute file exceeds maximum size");
		fclose(fp);
		return false;
	}

	if ( this->m_attrbuf != NULL )
	{
		GlobalFree(this->m_attrbuf);
		this->m_attrbuf = NULL;
	}

	this->m_attrbuf = (UCHAR*)GlobalAlloc(GPTR, 65536);
	fread(this->m_attrbuf, 256, 256, fp);
	fclose(fp);
	this->path->SetMapDimensions( 256, 256, (UCHAR *)this->m_attrbuf, this->thisMapNumber);
	return true;
}



BYTE MapClass::GetAttr(int x, int y )
{
	if ( x < 0 )
	{
		return 4;
	}

	if ( y < 0 )
	{
		return 4;
	}

	if( x > 255)
		return 4;
	if ( y > 255)
		return 4;

	if ( x > this->m_width -1 )
	{
		return 4;
	}

	if ( y > this->m_height -1 )
	{
		return 4;
	}

	return this->m_attrbuf[y*256 + x];

}



BOOL MapClass::GetStandAttr(int x, int y)
{
	if ( x > (this->m_width-1) )
	{
		return FALSE;
	}

	if ( y > (this->m_height-1) )
	{
		return FALSE;
	}

	if ( x < 1 || x > 255 )
	{
		return FALSE;
	}

	if ( y < 1 || y > 255 )
	{
		return FALSE;
	}

	BYTE attr = this->m_attrbuf[(y * 256 + x)];

	if ( (attr&2) == 2 )
	{
		return FALSE;
	}

	if ( (attr&4) == 4 )
	{
		return FALSE;
	}

	if ( (attr&8) == 8 )
	{
		return FALSE;
	}

	return TRUE;
}




void MapClass::SearchStandAttr(short& x, short& y)
{
	int scount = 10;
	int tx;
	int ty;
	int tindex;
	int subx = 1;

	tx = x;
	ty = y;
	tindex = y * 256;

	if ( (tx-10) < 0 )
	{
		subx = 1;
	}

	if ( (tx+10) > 255 )
	{
		subx = -1;
	}

	while ( scount-- != 0 )
	{
		if ( ((this->m_attrbuf[tindex + tx]&4)!= 4) && ((this->m_attrbuf[tindex + tx]&2)!= 2) && ((this->m_attrbuf[tindex + tx]&8)!= 8) )
		{
			x = tx;
			y = ty;
			break;
		}

		tx += subx;
	}
}



void MapClass::SetStandAttr(int x, int y)
{
	if ( x > this->m_width-1 )
	{
		return;
	}

	if ( y > this->m_height-1 )
	{
		return;
	}

	this->m_attrbuf[y*256+x] |= 2;
}



void MapClass::ClearStandAttr(int x, int y)
{
	if ( ((x<0)?FALSE:(x>(this->m_width-1))?FALSE:TRUE) == FALSE )
	{
		return;
	}

	if ( ((y<0)?FALSE:(y>(this->m_height-1))?FALSE:TRUE) == FALSE )
	{
		return;
	}

	if ( (this->m_attrbuf[y*256+x]&2)==2 )
	{
		this->m_attrbuf[y*256+x] &= 0xFD;
	}
}




bool MapClass::PathFinding2(int sx, int sy, int tx, int ty, PATH_t* a)
{
	bool Success = this->path->FindPath(sx, sy, tx, ty, true);

	if ( Success == false )
	{
		Success = this->path->FindPath(sx, sy, tx, ty, false);
	}

	if ( Success != false )
	{
		int path = this->path->GetPath();

		if ( path > 1 )
		{
			a->PathNum = path;
			BYTE * px = this->path->GetPathX();
			BYTE * py = this->path->GetPathY();

			for (int n=0;n<a->PathNum;n++)
			{
				a->PathX[n] = px[n];
				a->PathY[n] = py[n];
			}

			a->CurrentPath = 0;
			return 1;
		}

	}

	return false;
}


int TERRAIN_INDEX_REPEAT(int x, int y);



BOOL MapClass::CheckWall(int sx1, int sy1, int sx2, int sy2)
{
	int Index = TERRAIN_INDEX_REPEAT(sx1,sy1);
	int nx1;
	int ny1;
	int d1;
	int d2;
	int len1;
	int len2;
	int px1 = sx2 - sx1;
	int py1 = sy2 - sy1;

	if ( px1 < 0 )
	{
		px1 = -px1;
		nx1 = -1;
	}
	else
	{
		nx1 = 1;
	}

	if ( py1 < 0 )
	{
		py1 = -py1;
		ny1 = -256;
	}
	else
	{
		ny1 = 256;
	}

	if ( px1 > py1 )
	{
		len1 = px1;
		len2= py1;
		d1 = ny1;
		d2 = nx1;
	}
	else
	{
		len1 = py1;
		len2 = px1;
		d1 = nx1;
		d2 = ny1;
	}

	int error = 0;
	int count = 0;
	int Shadow = 0;

	do
	{
		if ( (this->m_attrbuf[Index]&4) == 4 )
		{
			return FALSE;
		}

		error += len2;

		if ( error > len1/2 )
		{
			Index += d1;
			error -= len1;
		}
		
		Index += d2;
		count++;	
	}
	while ( count <= len1 );

	return TRUE;
}



int TERRAIN_INDEX_REPEAT(int x, int y)
{
	return (BYTE)y * 256 + (BYTE)x;
}



bool MapClass::PathFinding3(int sx, int sy, int tx, int ty, PATH_t* a)
{
	bool Success = this->path->FindPath2(sx, sy, tx, ty, TRUE);

	if ( Success == false )
	{
		Success = this->path->FindPath2(sx, sy, tx, ty, FALSE);
	}

	if ( Success != false )
	{
		int lc3 = this->path->GetPath();

		if ( lc3 > 1 )
		{
			a->PathNum = lc3;
			BYTE * px = this->path->GetPathX();
			BYTE * py = this->path->GetPathY();

			for (int n=0;n<a->PathNum;n++)
			{
				a->PathX[n] = px[n];
				a->PathY[n] = py[n];
			}

			a->CurrentPath = 0;
			return 1;
		}

	}

	return false;
}

bool MapClass::PathFinding4(int sx, int sy, int tx, int ty, PATH_t *a)
{
	bool Success = this->path->FindPath3(sx, sy, tx, ty, true);

	if ( Success == false )
	{
		Success = this->path->FindPath3(sx, sy, tx, ty, false);
	}

	if ( Success != false )
	{
		int lc3 = this->path->GetPath();

		if ( lc3 > 1 )
		{
			a->PathNum = lc3;
			BYTE * px = this->path->GetPathX();
			BYTE * py = this->path->GetPathY();

			for (int n=0;n<a->PathNum;n++)
			{
				a->PathX[n] = px[n];
				a->PathY[n] = py[n];
			}

			a->CurrentPath = 0;
			return 1;
		}

	}

	return false;
}


BYTE MapClass::CheckWall2(int sx1, int sy1, int sx2, int sy2)
{
	int Index = TERRAIN_INDEX_REPEAT(sx1,sy1);
	int nx1;
	int ny1;
	int d1;
	int d2;
	int len1;
	int len2;
	int px1 = sx2 - sx1;
	int py1 = sy2 - sy1;

	if ( px1 < 0 )
	{
		px1 = -px1;
		nx1 = -1;
	}
	else
	{
		nx1 = 1;
	}

	if ( py1 < 0 )
	{
		py1 = -py1;
		ny1 = -256;
	}
	else
	{
		ny1 = 256;
	}

	if ( px1 > py1 )
	{
		len1 = px1;
		len2= py1;
		d1 = ny1;
		d2 = nx1;
	}
	else
	{
		len1 = py1;
		len2 = px1;
		d1 = nx1;
		d2 = ny1;
	}

	int start = 0;
	BOOL error = FALSE;
	int count = 0;
	int Shadow = 0;

	do
	{
		if ( start == 0 )
		{
			if ( (this->m_attrbuf[Index]&2) == 2 )
			{
				return 2;
			}
		}
		else
		{
			start = 1;
		}

		if ( (this->m_attrbuf[Index]&4) == 4 )
		{
			return 4;
		}

		error +=len2;

		if ( error > (len1/2) )
		{
			Index += d1;
			error -= len1;
		}

		Index += d2;
		count++;
	}
	while ( count <= len1 );
	
	return 1;
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

