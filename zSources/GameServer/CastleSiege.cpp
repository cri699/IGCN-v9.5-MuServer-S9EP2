//////////////////////////////////////////////////////////////////////
// CastleSiege.cpp
//HermeX's Master Piece of WebZen Dissassembly code (aka Decompilation)
//Made for GameServer 1.00.90_2 JPN Version (Season4.5)
//Status: 100% Functional and _ASM Code 100% Identical as Original GameServer.exe file!!!

#include "stdafx.h"
#include "CastleSiege.h"
#include "CastleDeepEvent.h"
#include "DSProtocol.h"
#include "GameMain.h"
#include "TNotice.h"
#include "TUnion.h"
#include "LifeStone.h"
#include "Mercenary.h"
#include "TLog.h"
#include "BuffEffectSlot.h"
#include "configread.h"
#include "winutil.h"

CCastleSiege g_CastleSiege;

#define CS_CHECK_UPGRADE(value) ( (( (value)   )<0)?FALSE:(( (value)  )>CS_MAX_UPGRADE-1)?FALSE:TRUE  )
#define XY_MACRO(value) ( (( (value)   )<0)?FALSE:(( (value)  )>255)?FALSE:TRUE  )

_CS_SCHEDULE_DATA::_CS_SCHEDULE_DATA()
{
	this->Clear();
}

_CS_SCHEDULE_DATA::~_CS_SCHEDULE_DATA()
{

}

void _CS_SCHEDULE_DATA::Clear()
{
	this->m_bIN_USE = FALSE;
	this->m_iSTATE = -1;
	this->m_iADD_DAY = -1;
	this->m_iADD_HOUR = -1; 
	this->m_iADD_MIN = -1;
	this->m_iGAP_SEC = 0;
}

_CS_REG_GUILD_DATA::_CS_REG_GUILD_DATA()
{
	this->Clear();
}

_CS_REG_GUILD_DATA::~_CS_REG_GUILD_DATA()
{

}

void _CS_REG_GUILD_DATA::Clear()
{
	this->m_bIN_USE = FALSE;
	this->m_strGuildName = "";
	this->m_iRegMarkCount = 0;
	this->m_iGuildMemberCount = 0;
	this->m_iGuildMasterLevel = 0;
	this->m_iSeqNum = 0;
	this->m_i64TotolScore = 0;
}

_CS_TOTAL_GUILD_DATA::_CS_TOTAL_GUILD_DATA()
{
	this->Clear();
}

_CS_TOTAL_GUILD_DATA::~_CS_TOTAL_GUILD_DATA()
{

}

void _CS_TOTAL_GUILD_DATA::Clear()
{
	this->m_bIN_USE = FALSE;
	this->m_iCsGuildID = 0;
	this->m_bGuildInvolved = 0;
	this->m_iGuildScore = 0; //season 2.5 add-on
}

_CS_MINIMAP_DATA::_CS_MINIMAP_DATA()
{
	this->Clear();
}

_CS_MINIMAP_DATA::~_CS_MINIMAP_DATA()
{

}

void _CS_MINIMAP_DATA::Clear()
{
	this->m_iGuildMasterIndex = -1;
	memset(this->m_szGuildName,0,sizeof(this->m_szGuildName));
	memset(this->m_stMiniMapPoint,0,sizeof(this->m_stMiniMapPoint));
	this->m_iMiniMapPointCount = 0;
}

_CS_NPC_DATA::_CS_NPC_DATA()
{
	this->Clear();
	this->m_iCS_GATE_LEVER_INDEX = -1;
};

_CS_NPC_DATA::~_CS_NPC_DATA()
{

};

void _CS_NPC_DATA::Clear()
{
	this->m_bIN_USE = FALSE;
	this->m_iNPC_NUM = -1;
	this->m_iNPC_INDEX = -1;
	this->m_iNPC_LIVE = 0;
	this->m_iNPC_AUTH = -1;
	this->m_bNPC_DBSAVE = FALSE;
	this->m_iNPC_SIDE = 0;
	this->m_iNPC_OBJINDEX = -1;
	this->m_iNPC_DF_LEVEL = 0;
	this->m_iNPC_RG_LEVEL = 0;
	this->m_iNPC_MAXHP = 0;
	this->m_iNPC_HP = 0;
	this->m_iNPC_SX = -1;
	this->m_iNPC_SY = -1;
	this->m_iNPC_DX = -1;
	this->m_iNPC_DY = -1;
	this->m_iNPC_DIR = -1;
	this->m_iNPC_BASE_DF_LEVEL = 0;
	this->m_iNPC_BASE_RG_LEVEL = 0;
	this->m_iNPC_BASE_MAXHP = 0;
	this->m_iNPC_BASE_HP = 0;
	this->m_iNPC_BASE_SX = -1;
	this->m_iNPC_BASE_SY = -1;
	this->m_iNPC_BASE_DX = -1;
	this->m_iNPC_BASE_DY = -1;
	this->m_iNPC_BASE_DIR = -1;
	this->m_iCsNpcExistVal = 0;
}

void _CS_NPC_DATA::SetBaseValue()
{
	this->m_iNPC_DF_LEVEL = this->m_iNPC_BASE_DF_LEVEL;
	this->m_iNPC_RG_LEVEL = this->m_iNPC_BASE_RG_LEVEL;

	this->m_iNPC_MAXHP = this->m_iNPC_BASE_MAXHP;
	this->m_iNPC_HP = this->m_iNPC_BASE_HP;

	this->m_iNPC_SX = this->m_iNPC_BASE_SX;
	this->m_iNPC_SY = this->m_iNPC_BASE_SY;

	this->m_iNPC_DX = this->m_iNPC_BASE_DX;
	this->m_iNPC_DY = this->m_iNPC_BASE_DY;

	this->m_iNPC_DIR = this->m_iNPC_BASE_DIR;
}

CCastleSiege::CCastleSiege()
{
	InitializeCriticalSection(&this->m_critScheduleData);
	InitializeCriticalSection(&this->m_critNpcData);
	InitializeCriticalSection(&this->m_critCsTotalGuildInfo);
	InitializeCriticalSection(&this->m_critCsBasicGuildInfo);
	InitializeCriticalSection(&this->m_critCsMiniMap);
}

CCastleSiege::~CCastleSiege()
{
	DeleteCriticalSection(&this->m_critScheduleData);
	DeleteCriticalSection(&this->m_critNpcData);
	DeleteCriticalSection(&this->m_critCsTotalGuildInfo);
	DeleteCriticalSection(&this->m_critCsBasicGuildInfo);
	DeleteCriticalSection(&this->m_critCsMiniMap);
}

BYTE CCastleSiege::ScheduleStateCompFunc(_CS_SCHEDULE_DATA & A,_CS_SCHEDULE_DATA & B) //Identical
{
	if(A.m_iSTATE > B.m_iSTATE)
	{
		return false;
	}

	return true;
}

void CCastleSiege::Clear()
{
	this->m_bCastleSiegeEnable = TRUE;
	this->m_bFileDataLoadOK = FALSE;
	this->m_bDbDataLoadOK = FALSE;
	this->m_bDoRun = FALSE;
	this->m_bCastleSiegeFriendlyFire = FALSE;

	this->m_iMapSvrGroup = -1;

	this->m_iCastleSiegeCycle = 14;
	this->m_iCastleSiegeState = -1;

	this->m_bFixCastleCycleStartDate = FALSE;

	this->m_i64CastleMoney = 0;

	memset(&this->m_tmFixCastleCycleStartDate,0,sizeof(this->m_tmFixCastleCycleStartDate));

	this->m_iFixCastleSpecificState = 0;
	this->m_bFixCastleStateStartDate = FALSE;

	memset(&this->m_tmFixCastleStateStartDate,0,sizeof(this->m_tmFixCastleStateStartDate));

	this->m_iCastleDataLoadState = 0;
	this->m_dwDbDataLoadTickCount = 0;
	this->m_iCS_REMAIN_MSEC = -1;

	memset(&this->m_tmStateStartDate,0,sizeof(this->m_tmStateStartDate));
	memset(&this->m_tmLeftCastleSiegeDate,0,sizeof(this->m_tmLeftCastleSiegeDate));

	this->m_bCsBasicGuildInfoLoadOK = FALSE;
	this->m_bCsTotalGuildInfoLoadOK = FALSE;

	this->m_btIsSiegeGuildList = 0;
	this->m_btIsCastleOccupied = 0;

	memset(this->m_szCastleOwnerGuild,0,sizeof(this->m_szCastleOwnerGuild));

	this->m_bEnterHuntZone = FALSE;

	this->m_dwNPC_DBSAVE_TICK_COUNT = GetTickCount();
	this->m_dwCALC_LEFTSIEGE_TICK_COUNT = 0;
	this->m_dwEVENT_MSG_TICK_COUNT = 0;
	this->m_dwCHECK_GATE_ALIVE_COUNT = 0;
	this->m_dwCS_LEFTTIME_TICK_COUNT = 0;
	this->m_dwCS_STARTTIME_TICK_COUNT = 0;
	this->m_dwCS_JOINSIDE_REFRESH_TICK_COUNT = 0;
	this->m_dwCS_JOINSIDE_REFRESH_TOTAL_COUNT = 0;
	this->m_dwCS_MINIMAP_SEND_TICK_COUNT = 0;

	this->m_bCastleTowerAccessable = FALSE;
	this->m_bRegCrownAvailable = FALSE;

	this->m_iCastleCrownAccessUser = -1;

	this->m_btCastleCrownAccessUserX = 0;
	this->m_btCastleCrownAccessUserY = 0;

	this->m_iCastleSwitchAccessUser1 = -1;
	this->m_iCastleSwitchAccessUser2 = -1;

	this->m_dwCrownAccessTime = 0;

	memset(this->m_szMiddleWinnerGuild,0,sizeof(this->m_szMiddleWinnerGuild));

	EnterCriticalSection(&this->m_critScheduleData);
	this->m_vtScheduleData.clear();
	LeaveCriticalSection(&this->m_critScheduleData);

	EnterCriticalSection(&this->m_critNpcData);
	this->m_vtNpcData.clear();
	LeaveCriticalSection(&this->m_critNpcData);

	this->m_bDbNpcCreated = FALSE;

	this->ResetAllUserCsJoinSide();
}

int CCastleSiege::Ready(int iMapSvrGroup)
{
	if(iMapSvrGroup < 0)
	{
		return false;
	}

	this->Clear();
	this->ClearAllNPC();

	this->m_iMapSvrGroup = iMapSvrGroup;
	this->m_iCastleDataLoadState = 1;

	return true;
}

int CCastleSiege::LoadPreFixData(char * lpszFileName)
{
	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(lpszFileName, pugi::parse_default | pugi::parse_comments | pugi::parse_declaration);

	if (res.status != pugi::status_ok)
	{
		g_Log.MsgBox("Error loading %s file (%s", lpszFileName, res.description());
		return 0;
	}

	pugi::xml_node castlesiege = file.child("CastleSiegeEvent");
	pugi::xml_node damagereduction = castlesiege.child("DamageReduction");
	pugi::xml_node cyclesettings = castlesiege.child("CycleSettings");
	pugi::xml_node cyclecorrection = castlesiege.child("CycleCorrection");

	_SYSTEMTIME tmToDay;

	GetLocalTime(&tmToDay);

	int iTODAY_DATE_NUM = MACRO2(( MACRO1(tmToDay.wDay) | MACRO1(tmToDay.wMonth) << 8 ) & 0xFFFF ) | MACRO2(tmToDay.wYear) << 16;
	__int64 iTODAY_DATE_NUM64 = (MACRO2(tmToDay.wMinute) | MACRO2(tmToDay.wHour) << 16) | ((__int64)(MACRO2(( MACRO1(tmToDay.wDay) | MACRO1(tmToDay.wMonth) << 8 ) & 0xFFFF ) | MACRO2(tmToDay.wYear) << 16) << 0x20);

	this->m_bCastleSiegeEnable = castlesiege.attribute("Enable").as_int();

	this->m_bCastleSiegeFriendlyFire = castlesiege.attribute("FriendlyFire").as_int();
	this->CastleSiegeResetAccumulationAfterKill = castlesiege.attribute("ResetAccumulatedTimeAfterKill").as_int();
	this->m_CastleLordMixDailyLimit = castlesiege.attribute("LordMixDailyLimit").as_int();
	this->m_TowerEntryWithoutAlliance = castlesiege.attribute("TowerEntryWithoutAlliance").as_int();

	this->CastleSiegeDmgReductionPercent = damagereduction.attribute("OppositeTeam").as_int(40);
	this->CastleSiegeSelfDmgReductionPercent = damagereduction.attribute("SelfTeam").as_int(20);

	this->m_tmFixCastleCycleStartDate.wYear = cyclesettings.attribute("StartYear").as_int();
	this->m_tmFixCastleCycleStartDate.wMonth = cyclesettings.attribute("StartMonth").as_int();
	this->m_tmFixCastleCycleStartDate.wDay = cyclesettings.attribute("StartDay").as_int();

	this->m_iFixCastleSpecificState = cyclecorrection.attribute("State").as_int();

	this->m_tmFixCastleStateStartDate.wYear = cyclecorrection.attribute("Year").as_int();
	this->m_tmFixCastleStateStartDate.wMonth = cyclecorrection.attribute("Month").as_int();
	this->m_tmFixCastleStateStartDate.wDay = cyclecorrection.attribute("Day").as_int();
	this->m_tmFixCastleStateStartDate.wHour = cyclecorrection.attribute("Hour").as_int();
	this->m_tmFixCastleStateStartDate.wMinute = cyclecorrection.attribute("Minute").as_int();

	if(this->m_tmFixCastleCycleStartDate.wYear == 0 || this->m_tmFixCastleCycleStartDate.wMonth == 0 || this->m_tmFixCastleCycleStartDate.wDay == 0)
	{
		this->m_bFixCastleCycleStartDate = FALSE;
	}
	else
	{
		_SYSTEMTIME tmEndDate = this->m_tmFixCastleCycleStartDate;
		GetNextDay(&tmEndDate, this->m_iCastleSiegeCycle, 0, 0, 0);

		int iEVENT_END_DATE_NUM = MACRO2(( MACRO1(tmEndDate.wDay) | MACRO1(tmEndDate.wMonth) << 8 ) & 0xFFFF ) | MACRO2(tmEndDate.wYear) << 16;

		if(iEVENT_END_DATE_NUM <= iTODAY_DATE_NUM)
		{
			this->m_bFixCastleCycleStartDate = FALSE;
			g_Log.MsgBox("[CastleSiege] CCastleSiege::LoadPreFixData() - iEVENT_END_DATE_NUM (%04d-%02d-%02d) <= iTODAY_DATE_NUM (%04d-%02d-%02d)", tmEndDate.wYear,tmEndDate.wMonth,tmEndDate.wDay,tmToDay.wYear,tmToDay.wMonth,tmToDay.wDay);
		}
		else
		{
			this->m_bFixCastleCycleStartDate = TRUE;
		}

		if(MONTH_MACRO(this->m_tmFixCastleCycleStartDate.wMonth) == FALSE	|| DAY_MACRO(this->m_tmFixCastleCycleStartDate.wDay) == FALSE)
		{
			this->m_bFixCastleCycleStartDate = FALSE;
		}
	}

	if(this->m_tmFixCastleStateStartDate.wYear == 0 || this->m_tmFixCastleStateStartDate.wMonth == 0 || this->m_tmFixCastleStateStartDate.wDay == 0)
	{
		this->m_bFixCastleStateStartDate = FALSE;
	}
	else if(this->m_iFixCastleSpecificState < CASTLESIEGE_STATE_REGSIEGE || this->m_iFixCastleSpecificState > CASTLESIEGE_STATE_ENDCYCLE)
	{
		this->m_bFixCastleStateStartDate = FALSE;
	}
	else
	{
		__int64 iSIEGE_DATE_NUM64 = (MACRO2(m_tmFixCastleStateStartDate.wMinute) | MACRO2(m_tmFixCastleStateStartDate.wHour) << 16) | ((__int64)(MACRO2(( MACRO1(m_tmFixCastleStateStartDate.wDay) | MACRO1(m_tmFixCastleStateStartDate.wMonth) << 8 ) & 0xFFFF ) | MACRO2(m_tmFixCastleStateStartDate.wYear) << 16) << 0x20);

		if(iSIEGE_DATE_NUM64 <= iTODAY_DATE_NUM64)
		{
			this->m_bFixCastleStateStartDate = FALSE;
			g_Log.MsgBox("[CastleSiege] CCastleSiege::LoadPreFixData() - iSIEGE_DATE_NUM64 (STATE:%d, %04d-%02d-%02d %02d:%02d:%02d) <= iTODAY_DATE_NUM64 (%04d-%02d-%02d %02d:%02d:%02d)", this->m_iFixCastleSpecificState, this->m_tmFixCastleStateStartDate.wYear, this->m_tmFixCastleStateStartDate.wMonth, this->m_tmFixCastleStateStartDate.wDay, this->m_tmFixCastleStateStartDate.wHour, this->m_tmFixCastleStateStartDate.wMinute, this->m_tmFixCastleStateStartDate.wSecond, tmToDay.wYear, tmToDay.wMonth, tmToDay.wDay, tmToDay.wHour, tmToDay.wMinute, tmToDay.wSecond);
		}
		else
		{
			this->m_bFixCastleStateStartDate = TRUE;
		}

		if(this->m_iFixCastleSpecificState == CASTLESIEGE_STATE_ENDSIEGE)
		{
			g_Log.MsgBox("[CastleSiege] CCastleSiege::LoadPreFixData() - FIX STATE 'CASTLESIEGE_STATE_ENDSIEGE' CAN'T BE USED");
			this->m_bFixCastleStateStartDate = FALSE;
		}

		if(MONTH_MACRO(this->m_tmFixCastleStateStartDate.wMonth) == FALSE || DAY_MACRO(this->m_tmFixCastleStateStartDate.wDay) == FALSE || HOUR_MACRO(this->m_tmFixCastleStateStartDate.wHour) == FALSE || MINUTE_MACRO(this->m_tmFixCastleStateStartDate.wMinute) == FALSE)
		{
			this->m_bFixCastleStateStartDate = FALSE;
		}
	}

	cyclesettings.attribute("StartYear").set_value(0);
	cyclesettings.attribute("StartMonth").set_value(0);
	cyclesettings.attribute("StartDay").set_value(0);
	cyclecorrection.attribute("State").set_value(0);
	cyclecorrection.attribute("Year").set_value(0);
	cyclecorrection.attribute("Month").set_value(0);
	cyclecorrection.attribute("Day").set_value(0);
	cyclecorrection.attribute("Hour").set_value(0);
	cyclecorrection.attribute("Minute").set_value(0);

	file.save_file(lpszFileName, "\t", pugi::format_default | pugi::format_no_declaration, pugi::encoding_utf8);

	GetLocalTime(&this->m_tmLordMixTime);
	this->m_CurrentLordMixLimit = this->m_CastleLordMixDailyLimit;

	g_Log.Add("[CastleSiege] Set CastleLordMix Limit:%d", this->m_CurrentLordMixLimit);

	return this->m_bFixCastleCycleStartDate & this->m_bFixCastleStateStartDate;
}

int CCastleSiege::LoadData(char * lpszFileName)
{
	using namespace std;

	if (lpszFileName == NULL || strcmp(lpszFileName, "") == NULL)
	{
		g_Log.MsgBox("[CastleSiege] file load error - File Name Error");
		return false;
	}

	if (this->m_iCastleDataLoadState != CASTLESIEGE_DATALOAD_1)
	{
		g_Log.AddC(TColor::Red, "[CastleSiege] CCastleSiege::LoadData() - m_iCastleDataLoadState != CASTLESIEGE_DATALOAD_1 (%d)", this->m_iCastleDataLoadState);
		return false;
	}

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(lpszFileName);

	if (res.status != pugi::status_ok)
	{
		g_Log.MsgBox("[CastleSiege] file load error - load_file() : %s (%s)", lpszFileName, res.description());
		return false;
	}

	std::vector<_CS_SCHEDULE_DATA> vtScheduleData;
	std::vector<_CS_NPC_DATA> vtNpcData;
	std::map<UINT, int> mpNpcDataCheck;

	int iCountDfUpgrade_Gate = 0;
	int iCountDfUpgrade_Statue = 0;
	int iCountHpUpgrade_Gate = 0;
	int iCountHpUpgrade_Statue = 0;
	int iCountRgUpgrade_Statue = 0;
	int iCountDfValue_Gate = 0;
	int iCountDfValue_Statue = 0;

	pugi::xml_node castlesiege = file.child("CastleSiegeEvent");
	pugi::xml_node cyclesettings = castlesiege.child("CycleSettings");

	int iCastleSiegeCycle = cyclesettings.attribute("Days").as_int();

	if (iCastleSiegeCycle <= 0)
	{
		g_Log.AddC(TColor::Red, "[CastleSiege] CCastleSiege::LoadData() - iCastleSiegeCycle <= 0 : %d", iCastleSiegeCycle);
		return false;
	}

	else
	{
		this->m_iCastleSiegeCycle = iCastleSiegeCycle;
	}

	int iSTATE = -1;
	int iSTART_DAY = -1;
	int iSTART_HOUR = -1;
	int iSTART_MIN = -1;

	for (pugi::xml_node cycle = cyclesettings.child("Cycle"); cycle; cycle = cycle.next_sibling())
	{
		iSTATE = cycle.attribute("Stage").as_int();
		iSTART_DAY = cycle.attribute("Day").as_int();
		iSTART_HOUR = cycle.attribute("Hour").as_int();
		iSTART_MIN = cycle.attribute("Minute").as_int();

		if (!vtScheduleData.empty())
		{
			for (std::vector<_CS_SCHEDULE_DATA>::iterator it = vtScheduleData.begin(); it != vtScheduleData.end(); it++)
			{
				_CS_SCHEDULE_DATA pScheduleData = _CS_SCHEDULE_DATA(*it);

				if (pScheduleData.m_bIN_USE != 0)
				{
					if (pScheduleData.m_iSTATE == iSTATE)
					{
						g_Log.AddC(TColor::Red, "[CastleSiege] CCastleSiege::LoadData() - Same State Exist : %d", iSTATE);
						return false;
					}

					if (pScheduleData.m_iSTATE > iSTATE)
					{
						int iSTATE1_TIME = pScheduleData.m_iADD_DAY * 24 * 60 + pScheduleData.m_iADD_HOUR * 60 + pScheduleData.m_iADD_MIN;
						int iSTATE2_TIME = iSTART_DAY * 24 * 60 + iSTART_HOUR * 60 + iSTART_MIN;

						if (iSTATE1_TIME <= iSTATE2_TIME)
						{
							g_Log.AddC(TColor::Red, "[CastleSiege] CCastleSiege::LoadData() - Date Order is wrong : %d", iSTATE2_TIME);
							return false;
						}
					}
					else
					{
						int iSTATE1_TIME = pScheduleData.m_iADD_DAY * 24 * 60 + pScheduleData.m_iADD_HOUR * 60 + pScheduleData.m_iADD_MIN;
						int iSTATE2_TIME = iSTART_DAY * 24 * 60 + iSTART_HOUR * 60 + iSTART_MIN;
						if (iSTATE1_TIME >= iSTATE2_TIME)
						{
							g_Log.AddC(TColor::Red, "[CastleSiege] CCastleSiege::LoadData() - Date Order is wrong : %d", iSTATE2_TIME);
							return false;
						}
					}
				}
			}
		}

		_CS_SCHEDULE_DATA pScheduleDataInsert;

		pScheduleDataInsert.m_bIN_USE = TRUE;
		pScheduleDataInsert.m_iSTATE = iSTATE;
		pScheduleDataInsert.m_iADD_DAY = iSTART_DAY;
		pScheduleDataInsert.m_iADD_HOUR = iSTART_HOUR;
		pScheduleDataInsert.m_iADD_MIN = iSTART_MIN;

		vtScheduleData.push_back(pScheduleDataInsert);
	}

	int iNPC_NUM = -1;
	int iNPC_INDEX = -1;
	int iNPC_SIDE = 0;
	int iNPC_DBSAVE = 0;
	int iNPC_DF_LEVEL = 0;
	int iNPC_RG_LEVEL = 0;
	int iNPC_MAXHP = 0;
	int iNPC_SX = -1;
	int iNPC_SY = -1;
	int iNPC_DX = -1;
	int iNPC_DY = -1;
	int iNPC_DIR = -1;

	pugi::xml_node npcsettings = castlesiege.child("NPCSettings");

	for (pugi::xml_node npc = npcsettings.child("NPC"); npc; npc = npc.next_sibling())
	{
		iNPC_NUM = npc.attribute("Index").as_int();
		iNPC_INDEX = npc.attribute("Number").as_int();
		iNPC_DBSAVE = npc.attribute("DBSave").as_int();
		iNPC_SIDE = npc.attribute("Side").as_int();
		iNPC_DF_LEVEL = npc.attribute("DefenseLevel").as_int();
		iNPC_RG_LEVEL = npc.attribute("HPRecLevel").as_int();
		iNPC_MAXHP = npc.attribute("MaxHP").as_int();
		iNPC_SX = npc.attribute("StartX").as_int();
		iNPC_SY = npc.attribute("StartY").as_int();
		iNPC_DX = npc.attribute("EndX").as_int();
		iNPC_DY = npc.attribute("EndY").as_int();
		iNPC_DIR = npc.attribute("Dir").as_int();

		if (iNPC_NUM < 0)
		{
			g_Log.MsgBox("[CastleSiege] CCastleSiege::LoadData() - Data Fail (iNPC_NUM < 0)");
			return false;
		}

		if (iNPC_INDEX < 0)
		{
			g_Log.MsgBox("[CastleSiege] CCastleSiege::LoadData() - Data Fail (iNPC_INDEX < 0)");
			return false;
		}

		if (iNPC_SIDE < 0)
		{
			g_Log.MsgBox("[CastleSiege] CCastleSiege::LoadData() - Data Fail (iNPC_SIDE < 0)");
			return false;
		}

		if (iNPC_DF_LEVEL < 0)
		{
			g_Log.MsgBox("[CastleSiege] CCastleSiege::LoadData() - Data Fail (iNPC_DF_LEVEL < 0)");
			return false;
		}

		if (iNPC_RG_LEVEL < 0)
		{
			g_Log.MsgBox("[CastleSiege] CCastleSiege::LoadData() - Data Fail (iNPC_RG_LEVEL < 0)");
			return false;
		}

		if (iNPC_MAXHP < 0)
		{
			g_Log.MsgBox("[CastleSiege] CCastleSiege::LoadData() - Data Fail (iNPC_MAXHP < 0)");
			return false;
		}

		if (iNPC_SX < 0)
		{
			g_Log.MsgBox("[CastleSiege] CCastleSiege::LoadData() - Data Fail (iNPC_SX < 0)");
			return false;
		}

		if (iNPC_SY < 0)
		{
			g_Log.MsgBox("[CastleSiege] CCastleSiege::LoadData() - Data Fail (iNPC_SY < 0)");
			return false;
		}

		UINT uNpcKey = MACRO2(iNPC_INDEX) | (MACRO2(iNPC_NUM) << 16);

		std::map<UINT, int>::iterator it = mpNpcDataCheck.find(uNpcKey);

		if (it != mpNpcDataCheck.end())
		{
			g_Log.MsgBox("[CastleSiege] CCastleSiege::LoadData() - Same Npc Key Exist : NUM:%d-IDX:%d", iNPC_NUM, iNPC_INDEX);
			return false;
		}

		mpNpcDataCheck.insert(pair <UINT, int >((MACRO2(iNPC_INDEX) | (MACRO2(iNPC_NUM) << 16)), 0));

		_CS_NPC_DATA pNpcDataInsert;

		pNpcDataInsert.m_bIN_USE = TRUE;
		pNpcDataInsert.m_iNPC_NUM = iNPC_NUM;
		pNpcDataInsert.m_iNPC_INDEX = iNPC_INDEX;
		pNpcDataInsert.m_iNPC_SIDE = iNPC_SIDE;
		pNpcDataInsert.m_bNPC_DBSAVE = iNPC_DBSAVE;

		pNpcDataInsert.m_iNPC_BASE_DF_LEVEL = iNPC_DF_LEVEL;
		pNpcDataInsert.m_iNPC_DF_LEVEL = pNpcDataInsert.m_iNPC_BASE_DF_LEVEL;

		pNpcDataInsert.m_iNPC_BASE_RG_LEVEL = iNPC_RG_LEVEL;
		pNpcDataInsert.m_iNPC_RG_LEVEL = pNpcDataInsert.m_iNPC_BASE_RG_LEVEL;

		pNpcDataInsert.m_iNPC_BASE_MAXHP = iNPC_MAXHP;
		pNpcDataInsert.m_iNPC_MAXHP = pNpcDataInsert.m_iNPC_BASE_MAXHP;

		pNpcDataInsert.m_iNPC_BASE_HP = iNPC_MAXHP;
		pNpcDataInsert.m_iNPC_HP = pNpcDataInsert.m_iNPC_BASE_HP;

		pNpcDataInsert.m_iNPC_BASE_SX = iNPC_SX;
		pNpcDataInsert.m_iNPC_SX = pNpcDataInsert.m_iNPC_BASE_SX;

		pNpcDataInsert.m_iNPC_BASE_SY = iNPC_SY;
		pNpcDataInsert.m_iNPC_SY = pNpcDataInsert.m_iNPC_BASE_SY;

		pNpcDataInsert.m_iNPC_BASE_DX = iNPC_DX;
		pNpcDataInsert.m_iNPC_DX = pNpcDataInsert.m_iNPC_BASE_DX;

		pNpcDataInsert.m_iNPC_BASE_DY = iNPC_DY;
		pNpcDataInsert.m_iNPC_DY = pNpcDataInsert.m_iNPC_BASE_DY;

		pNpcDataInsert.m_iNPC_BASE_DIR = iNPC_DIR;
		pNpcDataInsert.m_iNPC_DIR = pNpcDataInsert.m_iNPC_BASE_DIR;

		vtNpcData.push_back(pNpcDataInsert);
	}

	int iLEVEL = 0;
	int iGEMCOUNT = 0;
	int iZEN = 0;

	pugi::xml_node defenseupgrade = castlesiege.child("DefenseUpgrade");
	pugi::xml_node gates = defenseupgrade.child("Gates");
	pugi::xml_node statues = defenseupgrade.child("Statues");

	for (pugi::xml_node defense = gates.child("Defense"); defense; defense = defense.next_sibling())
	{
		iLEVEL = defense.attribute("Level").as_int();
		iGEMCOUNT = defense.attribute("ReqJewelOfGuardian").as_int();
		iZEN = defense.attribute("ReqZen").as_int();

		if (iCountDfUpgrade_Gate >= CS_MAX_UPGRADE)
		{
			break;
		}

		g_iNpcUpDfLevel_CGATE[iCountDfUpgrade_Gate][0] = iLEVEL;
		g_iNpcUpDfLevel_CGATE[iCountDfUpgrade_Gate][1] = iGEMCOUNT;
		g_iNpcUpDfLevel_CGATE[iCountDfUpgrade_Gate][2] = iZEN;

		iCountDfUpgrade_Gate++;
	}

	iLEVEL = 0;
	iGEMCOUNT = 0;
	iZEN = 0;

	for (pugi::xml_node defense = statues.child("Defense"); defense; defense = defense.next_sibling())
	{
		iLEVEL = defense.attribute("Level").as_int();
		iGEMCOUNT = defense.attribute("ReqJewelOfGuardian").as_int();
		iZEN = defense.attribute("ReqZen").as_int();

		if (iCountDfUpgrade_Statue >= CS_MAX_UPGRADE)
		{
			break;
		}

		g_iNpcUpDfLevel_CSTATUE[iCountDfUpgrade_Statue][0] = iLEVEL;
		g_iNpcUpDfLevel_CSTATUE[iCountDfUpgrade_Statue][1] = iGEMCOUNT;
		g_iNpcUpDfLevel_CSTATUE[iCountDfUpgrade_Statue][2] = iZEN;

		iCountDfUpgrade_Statue++;
	}

	int iHP = 0;
	iGEMCOUNT = 0;
	iZEN = 0;

	pugi::xml_node healthupgrade = castlesiege.child("HealthUpgrade");
	gates = healthupgrade.child("Gates");
	statues = healthupgrade.child("Statues");

	for (pugi::xml_node health = gates.child("Health"); health; health = health.next_sibling())
	{
		iHP = health.attribute("Value").as_int();
		iGEMCOUNT = health.attribute("ReqJewelOfGuardian").as_int();
		iZEN = health.attribute("ReqZen").as_int();

		if (iCountHpUpgrade_Gate >= CS_MAX_UPGRADE)
		{
			break;
		}

		g_iNpcUpMaxHP_CGATE[iCountHpUpgrade_Gate][0] = iHP;
		g_iNpcUpMaxHP_CGATE[iCountHpUpgrade_Gate][1] = iGEMCOUNT;
		g_iNpcUpMaxHP_CGATE[iCountHpUpgrade_Gate][2] = iZEN;

		iCountHpUpgrade_Gate++;
	}

	iHP = 0;
	iGEMCOUNT = 0;
	iZEN = 0;

	for (pugi::xml_node health = statues.child("Health"); health; health = health.next_sibling())
	{
		iHP = health.attribute("Value").as_int();
		iGEMCOUNT = health.attribute("ReqJewelOfGuardian").as_int();
		iZEN = health.attribute("ReqZen").as_int();

		if (iCountHpUpgrade_Statue >= CS_MAX_UPGRADE)
		{
			break;
		}

		g_iNpcUpMaxHP_CSTATUE[iCountHpUpgrade_Statue][0] = iHP;
		g_iNpcUpMaxHP_CSTATUE[iCountHpUpgrade_Statue][1] = iGEMCOUNT;
		g_iNpcUpMaxHP_CSTATUE[iCountHpUpgrade_Statue][2] = iZEN;

		iCountHpUpgrade_Statue++;
	}

	iLEVEL = 0;
	iGEMCOUNT = 0;
	iZEN = 0;

	pugi::xml_node statuehealthrec = castlesiege.child("StatueHealthRecoveryUpgrade");

	for (pugi::xml_node recovery = statuehealthrec.child("Recovery"); recovery; recovery = recovery.next_sibling())
	{
		iLEVEL = recovery.attribute("Level").as_int();
		iGEMCOUNT = recovery.attribute("ReqJewelOfGuardian").as_int();
		iZEN = recovery.attribute("ReqZen").as_int();

		if (iCountRgUpgrade_Statue >= CS_MAX_UPGRADE)
		{
			break;
		}

		g_iNpcUpRgLevel_CSTATUE[iCountRgUpgrade_Statue][0] = iLEVEL;
		g_iNpcUpRgLevel_CSTATUE[iCountRgUpgrade_Statue][1] = iGEMCOUNT;
		g_iNpcUpRgLevel_CSTATUE[iCountRgUpgrade_Statue][2] = iZEN;

		iCountRgUpgrade_Statue++;
	}

	int iDFVALUE = 0;

	pugi::xml_node defensesettings = castlesiege.child("DefenseSettings");
	gates = defensesettings.child("Gates");
	statues = defensesettings.child("Statues");

	for (pugi::xml_node defense = gates.child("Defense"); defense; defense = defense.next_sibling())
	{
		iDFVALUE = defense.attribute("Value").as_int();

		if (iCountDfValue_Gate >= CS_MAX_UPGRADE + 1)
		{
			break;
		}

		g_iNpcDefense_CGATE[iCountDfValue_Gate] = iDFVALUE;

		iCountDfValue_Gate++;
	}
	
	iDFVALUE = 0;

	for (pugi::xml_node defense = statues.child("Defense"); defense; defense = defense.next_sibling())
	{
		iDFVALUE = defense.attribute("Value").as_int();

		if (iCountDfValue_Statue >= CS_MAX_UPGRADE + 1)
		{
			break;
		}

		g_iNpcDefense_CSTATUE[iCountDfValue_Statue] = iDFVALUE;

		iCountDfValue_Statue++;
	}

	EnterCriticalSection(&this->m_critScheduleData);

	if(!vtScheduleData.empty())
	{
		for(std::vector<_CS_SCHEDULE_DATA>::iterator it = vtScheduleData.begin(); it != vtScheduleData.end(); it++)
		{
			this->m_vtScheduleData.push_back(*it);
		}
	}

	sort(this->m_vtScheduleData.begin(),this->m_vtScheduleData.end(),this->ScheduleStateCompFunc);

	if(!m_vtScheduleData.empty())
	{
		for(int iIDX = 0; iIDX < this->m_vtScheduleData.size(); iIDX++)
		{
			if(iIDX+1 < this->m_vtScheduleData.size())
			{
				int iIDX_SEC1 = this->m_vtScheduleData[iIDX].m_iADD_DAY * 24 * 60 * 60 + this->m_vtScheduleData[iIDX].m_iADD_HOUR * 60 * 60 + this->m_vtScheduleData[iIDX].m_iADD_MIN * 60;
				int iIDX_SEC2 = this->m_vtScheduleData[iIDX+1].m_iADD_DAY * 24 * 60 * 60 + this->m_vtScheduleData[iIDX+1].m_iADD_HOUR * 60 * 60 + this->m_vtScheduleData[iIDX+1].m_iADD_MIN * 60;

				int iIDX_RESULT = iIDX_SEC2 - iIDX_SEC1;

				if(iIDX_RESULT < 0)
				{
					g_Log.AddC(TColor::Red, "[CastleSiege] CCastleSiege::LoadData() - Date Order is wrong (sort fail) : %d-%d", this->m_vtScheduleData[iIDX].m_iSTATE, this->m_vtScheduleData[iIDX+1].m_iSTATE);
				}
				else
				{
					this->m_vtScheduleData[iIDX].m_iGAP_SEC = iIDX_RESULT;
				}
			}
			else
			{
				this->m_vtScheduleData[iIDX].m_iGAP_SEC = 0;
			}
		}
	}

	LeaveCriticalSection(&this->m_critScheduleData);

	EnterCriticalSection(&this->m_critNpcData);

	if(!vtNpcData.empty())
	{
		for(std::vector<_CS_NPC_DATA>::iterator it = vtNpcData.begin();it != vtNpcData.end(); it++)
		{
			this->m_vtNpcData.push_back(*it);
		}
	}

	LeaveCriticalSection(&this->m_critNpcData);

	std::vector<_CS_SCHEDULE_DATA>::iterator it1 = this->m_vtScheduleData.begin();

	while(it1 != this->m_vtScheduleData.end())
	{
		_CS_SCHEDULE_DATA stTEMP = *it1;
		char szTEMP[256] = {0};
		wsprintf(szTEMP,"STT:%d, DAY:%d, HOUR:%d, MIN:%d, GAP:%d\n", stTEMP.m_iSTATE,stTEMP.m_iADD_DAY,stTEMP.m_iADD_HOUR,stTEMP.m_iADD_MIN,stTEMP.m_iGAP_SEC);
		OutputDebugString(szTEMP);
		it1++;
	}

	std::vector<_CS_NPC_DATA>::iterator it2 = this->m_vtNpcData.begin();

	while(it2 != this->m_vtNpcData.end())
	{
		_CS_NPC_DATA stTEMP = *it2;
		char szTEMP[256] = {0};
		wsprintf(szTEMP,"NUM:%d, IDX:%d, DBSAVE:%s, SIDE:%d, DFLEVEL:%d, RGLEVEL:%d, MXHP:%d, (%d,%d)-(%d,%d), DIR:%d\n", stTEMP.m_iNPC_NUM,stTEMP.m_iNPC_INDEX,(stTEMP.m_bNPC_DBSAVE != 0) ? "YES" : "NO",stTEMP.m_iNPC_SIDE,stTEMP.m_iNPC_DF_LEVEL, stTEMP.m_iNPC_RG_LEVEL,stTEMP.m_iNPC_MAXHP,stTEMP.m_iNPC_SX,stTEMP.m_iNPC_SY,stTEMP.m_iNPC_DX,stTEMP.m_iNPC_DY,stTEMP.m_iNPC_DIR);
		OutputDebugString(szTEMP);
		it2++;
	}

	this->m_bFileDataLoadOK = TRUE;
	return true;
}

int CCastleSiege::DataRequest()
{
	if(this->m_bFileDataLoadOK == FALSE)
	{
		g_Log.AddC(TColor::Red, "[CastleSiege] CCastleSiege::DataRequest() - m_bFileDataLoadOK == FALSE");
		return false;
	}

	if(this->m_iMapSvrGroup < 0)
	{
		g_Log.AddC(TColor::Red, "[CastleSiege] CCastleSiege::DataRequest() - m_iMapSvrGroup < 0");
		return false;
	}

	if(this->m_iCastleDataLoadState != CASTLESIEGE_DATALOAD_2)
	{
		g_Log.AddC(TColor::Red, "[CastleSiege] CCastleSiege::DataRequest() - m_iCastleDataLoadState != CASTLESIEGE_DATALOAD_2 (%d)",m_iCastleDataLoadState);
		return false;
	}

	BOOL bDbRequestData = FALSE;

	if(this->m_dwDbDataLoadTickCount == 0)
	{
		bDbRequestData = TRUE;
		m_dwDbDataLoadTickCount = GetTickCount();
		g_Log.AddC(TColor::Green, "[CastleSiege] CCastleSiege::DataRequest() - REQUEST DATA FIRST");
	}
	else if(GetTickCount() - this->m_dwDbDataLoadTickCount > 3000)
	{
		bDbRequestData = TRUE;
		m_dwDbDataLoadTickCount = GetTickCount();
		g_Log.AddC(TColor::Red, "[CastleSiege] CCastleSiege::DataRequest() - REQUEST DATA AGAIN");
	}

	if(bDbRequestData != FALSE)
	{
		GS_GDReqCastleInitData(m_iMapSvrGroup,m_iCastleSiegeCycle);
	}

	return true;
}

int CCastleSiege::Init()
{
	if(this->m_iCastleDataLoadState != CASTLESIEGE_DATALOAD_4)
	{
		g_Log.AddC(TColor::Red, "[CastleSiege] CCastleSiege::Init() - m_iCastleDataLoadState != CASTLESIEGE_DATALOAD_4 (%d)",this->m_iCastleDataLoadState);
		g_Log.MsgBox("[CastleSiege] CCastleSiege::Init() - m_iCastleDataLoadState != CASTLESIEGE_DATALOAD_4 (%d)",this->m_iCastleDataLoadState);
		return false;
	}

	if(this->m_bFileDataLoadOK == FALSE || this->m_bDbDataLoadOK == FALSE)
	{
		g_Log.AddC(TColor::Red, "[CastleSiege] CCastleSiege::Init() - Data Load Fail (FILE:%d, DB:%d)", this->m_bFileDataLoadOK, this->m_bDbDataLoadOK);
		g_Log.MsgBox("[CastleSiege] CCastleSiege::Init() - Data Load Fail (FILE:%d, DB:%d)", this->m_bFileDataLoadOK, this->m_bDbDataLoadOK);
		return false;
	}

	g_Log.Add("[CastleSiege] CCastleSiege::Init() - START", this->m_bFileDataLoadOK, this->m_bDbDataLoadOK);

	this->m_iCastleDataLoadState = 0;

	this->CreateAllCsGateLever();

	if(this->CheckSync() == FALSE)
	{
		g_Log.AddC(TColor::Red, "[CastleSiege] CCastleSiege::Init() - CheckSync() == FALSE");
		g_Log.MsgBox("[CastleSiege] CCastleSiege::Init() - CheckSync() == FALSE");
		return false;
	}

	this->m_bDoRun = TRUE;

	this->m_dwNPC_DBSAVE_TICK_COUNT = GetTickCount();
	this->m_dwCALC_LEFTSIEGE_TICK_COUNT = 0;
	this->m_dwEVENT_MSG_TICK_COUNT = 0;
	this->m_dwCHECK_GATE_ALIVE_COUNT = 0;
	this->m_dwCS_STARTTIME_TICK_COUNT = 0;
	this->m_dwCS_LEFTTIME_TICK_COUNT = 0;
	this->m_dwCS_JOINSIDE_REFRESH_TICK_COUNT = 0;
	this->m_dwCS_JOINSIDE_REFRESH_TOTAL_COUNT = 0;
	this->m_dwCS_MINIMAP_SEND_TICK_COUNT = 0;
	
	return true;
}

void CCastleSiege::SetState(int iCastleSiegeState,BOOL bSetRemainMsec)
{
	if(iCastleSiegeState < CASTLESIEGE_STATE_NONE || iCastleSiegeState > CASTLESIEGE_STATE_ENDCYCLE)
	{
		return;
	}

	this->m_iCastleSiegeState = iCastleSiegeState;

	switch(this->m_iCastleSiegeState)
	{
	case CASTLESIEGE_STATE_NONE:			this->SetState_NONE(bSetRemainMsec); break;
	case CASTLESIEGE_STATE_IDLE_1:			this->SetState_IDLE_1(bSetRemainMsec); break;
	case CASTLESIEGE_STATE_REGSIEGE:		this->SetState_REGSIEGE(bSetRemainMsec); break;
	case CASTLESIEGE_STATE_IDLE_2:			this->SetState_IDLE_2(bSetRemainMsec); break;
	case CASTLESIEGE_STATE_REGMARK:			this->SetState_REGMARK(bSetRemainMsec); break;
	case CASTLESIEGE_STATE_IDLE_3:			this->SetState_IDLE_3(bSetRemainMsec); break;
	case CASTLESIEGE_STATE_NOTIFY:			this->SetState_NOTIFY(bSetRemainMsec); break;
	case CASTLESIEGE_STATE_READYSIEGE:		this->SetState_READYSIEGE(bSetRemainMsec); break;
	case CASTLESIEGE_STATE_STARTSIEGE:		this->SetState_STARTSIEGE(bSetRemainMsec); break;
	case CASTLESIEGE_STATE_ENDSIEGE:		this->SetState_ENDSIEGE(bSetRemainMsec); break;
	case CASTLESIEGE_STATE_ENDCYCLE:		this->SetState_ENDCYCLE(bSetRemainMsec); break;
	}

	if(this->m_bDbNpcCreated == FALSE)
	{
		this->m_bDbNpcCreated = TRUE;
		this->CreateDbNPC();
	}

	GetLocalTime(&this->m_tmStateStartDate);

	this->m_dwEVENT_MSG_TICK_COUNT = 0;
	this->m_dwCHECK_GATE_ALIVE_COUNT = 0;
}

void CCastleSiege::Run() //Identical
{
	if( this->m_bDoRun == FALSE )
	{
		return;
	}
	
	if( this->m_bCastleSiegeEnable == FALSE )
	{
		return;
	}

	switch( this->m_iCastleSiegeState )
	{
	case CASTLESIEGE_STATE_NONE:
		this->ProcState_NONE();
		break;
	case CASTLESIEGE_STATE_IDLE_1:
		this->ProcState_IDLE_1();
		break;
	case CASTLESIEGE_STATE_REGSIEGE:
		this->ProcState_REGSIEGE();
		break;
	case CASTLESIEGE_STATE_IDLE_2:
		this->ProcState_IDLE_2();
		break;
	case CASTLESIEGE_STATE_REGMARK:
		this->ProcState_REGMARK();
		break;
	case CASTLESIEGE_STATE_IDLE_3:
		this->ProcState_IDLE_3();
		break;
	case CASTLESIEGE_STATE_NOTIFY:
		this->ProcState_NOTIFY();
		break;
	case CASTLESIEGE_STATE_READYSIEGE:
		this->ProcState_READYSIEGE();
		break;
	case CASTLESIEGE_STATE_STARTSIEGE:
		this->ProcState_STARTSIEGE();
		break;
	case CASTLESIEGE_STATE_ENDSIEGE:
		this->ProcState_ENDSIEGE();
		break;
	case CASTLESIEGE_STATE_ENDCYCLE:
		this->ProcState_ENDCYCLE();
		break;
	}

	if( (GetTickCount() - this->m_dwNPC_DBSAVE_TICK_COUNT) > 300000 )
	{
		if( this->m_iCastleSiegeState != CASTLESIEGE_STATE_STARTSIEGE )
		{
			this->StoreDbNpc();
		}
		this->m_dwNPC_DBSAVE_TICK_COUNT = GetTickCount();
	}

	
	if( (GetTickCount() - this->m_dwCALC_LEFTSIEGE_TICK_COUNT) > 10000 )
	{
		this->m_dwCALC_LEFTSIEGE_TICK_COUNT = GetTickCount();
		this->CalcCastleLeftSiegeDate();
	}

	static int g_iLastSiegeUpdate = 0;

	if( (GetTickCount() - g_iLastSiegeUpdate) > 3000 )
	{
		g_iLastSiegeUpdate = GetTickCount();

		int iLeftSiegeDate = this->CalcCastleLeftSiegeDate();

		if( iLeftSiegeDate >= 0 )
		{
			SYSTEMTIME tmLeftDate = this->GetCastleLeftSiegeDate();
			g_Log.AddC(TColor::Blue,  "LEFT : SEC(%d), SIEGE-DAY(%04d-%02d-%02d %02d:%02d:%02d)", iLeftSiegeDate, tmLeftDate.wYear, tmLeftDate.wMonth, tmLeftDate.wDay, tmLeftDate.wHour, tmLeftDate.wMinute, tmLeftDate.wSecond);
		}
		else
		{
			switch( iLeftSiegeDate )
			{
			case CASTLESIEGE_LEFTTIME_ONSIGE:
				g_Log.AddC(TColor::Blue,  "[CastleSiege] LEFT-TIME : CASTLESIEGE_LEFTTIME_ONSIGE");
				break;
			case CASTLESIEGE_LEFTTIME_ENDSIEGE:
				g_Log.AddC(TColor::Blue,  "[CastleSiege] LEFT-TIME : CASTLESIEGE_LEFTTIME_ENDSIEGE");
				break;
			case CASTLESIEGE_LEFTTIME_NOTRUNNING:
				g_Log.AddC(TColor::Blue,  "[CastleSiege] LEFT-TIME : CASTLESIEGE_LEFTTIME_NOTRUNNING");
				break;
			}
		}
	}
}

void CCastleSiege::SetState_NONE(BOOL bSetRemainMsec)
{

}

void CCastleSiege::SetState_IDLE_1(BOOL bSetRemainMsec)
{
	g_Log.Add("SET SetState_IDLE_1()");

	if(bSetRemainMsec != FALSE)
	{
		time_t ttSTime;
		time_t ttETime;
		tm tmETime;
		double dResultSecond;

		memset(&tmETime,0,sizeof(tmETime));

		tmETime.tm_year = m_tmStartDate.wYear - 1900;
		tmETime.tm_mon = m_tmStartDate.wMonth - 1;
		tmETime.tm_mday = m_tmStartDate.wDay;

		ttETime = mktime(&tmETime);
		time(&ttSTime);

		dResultSecond = difftime(ttETime,ttSTime);
		this->m_iCS_REMAIN_MSEC = dResultSecond * (double)1000.0;
	}
}

void CCastleSiege::SetState_REGSIEGE(BOOL bSetRemainMsec)
{
	g_Log.Add("SET SetState_REGSIEGE()");

	if(bSetRemainMsec != FALSE)
	{
		int iGAP_SEC = GetStateGapSec(CASTLESIEGE_STATE_REGSIEGE);

		if(iGAP_SEC < 0)
		{
			this->m_bDoRun = FALSE;
			g_Log.MsgBox("[CastleSiege] CCastleSiege::SetState_REGSIEGE() - iGAP_SEC < 0");
		}

		this->m_iCS_REMAIN_MSEC = iGAP_SEC * 1000;
	}

	GS_GDReqRestartCastleState(m_iMapSvrGroup);
}

void CCastleSiege::SetState_IDLE_2(BOOL bSetRemainMsec)
{
	g_Log.Add("SET SetState_IDLE_2()");

	if(bSetRemainMsec != FALSE)
	{
		int iGAP_SEC = GetStateGapSec(CASTLESIEGE_STATE_IDLE_2);

		if(iGAP_SEC < 0)
		{
			this->m_bDoRun = FALSE;
			g_Log.MsgBox("[CastleSiege] CCastleSiege::SetState_IDLE_2() - iGAP_SEC < 0");
		}

		this->m_iCS_REMAIN_MSEC = iGAP_SEC * 1000;
	}
}

void CCastleSiege::SetState_REGMARK(BOOL bSetRemainMsec)
{
	g_Log.Add("SET SetState_REGMARK()");

	if(bSetRemainMsec != FALSE)
	{
		int iGAP_SEC = GetStateGapSec(CASTLESIEGE_STATE_REGMARK);

		if(iGAP_SEC < 0)
		{
			this->m_bDoRun = FALSE;
			g_Log.MsgBox("[CastleSiege] CCastleSiege::SetState_REGMARK() - iGAP_SEC < 0");
		}

		this->m_iCS_REMAIN_MSEC = iGAP_SEC * 1000;
	}
}

void CCastleSiege::SetState_IDLE_3(BOOL bSetRemainMsec)
{
	g_Log.Add("SET SetState_IDLE_3()");

	if(bSetRemainMsec != FALSE)
	{
		int iGAP_SEC = GetStateGapSec(CASTLESIEGE_STATE_IDLE_3);

		if(iGAP_SEC < 0)
		{
			this->m_bDoRun = FALSE;
			g_Log.MsgBox("[CastleSiege] CCastleSiege::SetState_IDLE_3() - iGAP_SEC < 0");
		}

		this->m_iCS_REMAIN_MSEC = iGAP_SEC * 1000;
	}
}

void CCastleSiege::SetState_NOTIFY(BOOL bSetRemainMsec)
{
	g_Log.Add("SET SetState_NOTIFY()");

	if(bSetRemainMsec != FALSE)
	{
		int iGAP_SEC = GetStateGapSec(CASTLESIEGE_STATE_NOTIFY);

		if(iGAP_SEC < 0)
		{
			this->m_bDoRun = FALSE;
			g_Log.MsgBox("[CastleSiege] CCastleSiege::SetState_NOTIFY() - iGAP_SEC < 0");
		}

		this->m_iCS_REMAIN_MSEC = iGAP_SEC * 1000;
	}

	this->CheckBuildCsGuildInfo();
}

void CCastleSiege::SetState_READYSIEGE(BOOL bSetRemainMsec)
{
	g_Log.Add("SET SetState_READYSIEGE()");

	if(bSetRemainMsec != FALSE)
	{
		int iGAP_SEC = GetStateGapSec(CASTLESIEGE_STATE_READYSIEGE);

		if(iGAP_SEC < 0)
		{
			this->m_bDoRun = 0;
			g_Log.MsgBox("[CastleSiege] CCastleSiege::SetState_READYSIEGE() - iGAP_SEC < 0");
		}

		this->m_iCS_REMAIN_MSEC = iGAP_SEC * 1000;
	}

	this->CheckBuildCsGuildInfo();
}

void CCastleSiege::SetState_STARTSIEGE(BOOL bSetRemainMsec)
{
	g_Log.Add("SET SetState_STARTSIEGE()");

	if(bSetRemainMsec != FALSE)
	{
		int iGAP_SEC = GetStateGapSec(CASTLESIEGE_STATE_STARTSIEGE);

		if(iGAP_SEC < 0)
		{
			this->m_bDoRun = FALSE;
			g_Log.MsgBox("[CastleSiege] CCastleSiege::SetState_STARTSIEGE() - iGAP_SEC < 0");
		}

		this->m_iCS_REMAIN_MSEC = iGAP_SEC * 1000;
	}

	this->m_bCastleTowerAccessable = FALSE;
	this->m_bRegCrownAvailable = FALSE;

	this->m_iCastleCrownAccessUser = -1;

	this->m_btCastleCrownAccessUserX = 0;
	this->m_btCastleCrownAccessUserY = 0;

	this->m_iCastleSwitchAccessUser1 = -1;
	this->m_iCastleSwitchAccessUser2 = -1;

	this->m_dwCrownAccessTime = 0;

	this->m_dwCHECK_GATE_ALIVE_COUNT = 0;
	this->m_dwCS_STARTTIME_TICK_COUNT = 0;
	this->m_dwCS_LEFTTIME_TICK_COUNT = GetTickCount();
	this->m_dwCS_JOINSIDE_REFRESH_TICK_COUNT = GetTickCount();
	this->m_dwCS_JOINSIDE_REFRESH_TOTAL_COUNT = 0;
	this->m_dwCS_MINIMAP_SEND_TICK_COUNT = GetTickCount();

	memset(this->m_szMiddleWinnerGuild,0,sizeof(this->m_szMiddleWinnerGuild));

	if(this->CheckAttackGuildExist() == FALSE)
	{
		this->CheckCastleSiegeResult();
		this->SetState(CASTLESIEGE_STATE_ENDSIEGE,TRUE);
		g_Log.AddC(TColor::Red, "[CastleSiege] CCastleSiege::SetState_STARTSIEGE() - CheckAttackGuildExist() == FALSE");
	}
	else
	{
		g_CastleDeepEvent.ClearMonster();
		this->StoreDbNpc();
		this->ClearNonDbNPC();
		this->CheckReviveNonDbNPC();

		if( this->CheckGuardianStatueExist() == FALSE )
		{
			this->ClearCastleTowerBarrier();
			this->SetCastleTowerAccessable(TRUE);
		}

		this->AdjustDbNpcLevel();
		this->SetAllCastleGateState(FALSE);
		this->SetAllUserCsJoinSide();
		this->SavePcRoomUserList();
		this->ReSpawnEnemyUser(TRUE);
		this->NotifyAllUserCsStartState(1);
	}
}

void CCastleSiege::SetState_ENDSIEGE(BOOL bSetRemainMsec)
{
	g_Log.Add("SET SetState_ENDSIEGE()");

	if(bSetRemainMsec != FALSE)
	{
		time_t ttSTime;
		time_t ttETime;
		tm tmETime;
		double dResultSecond;

		memset(&tmETime,0,sizeof(tmETime));

		tmETime.tm_year = m_tmEndDate.wYear - 1900;
		tmETime.tm_mon = m_tmEndDate.wMonth - 1;
		tmETime.tm_mday = m_tmEndDate.wDay;

		time(&ttSTime);
		ttETime = mktime(&tmETime);

		dResultSecond = difftime(ttETime,ttSTime);
		this->m_iCS_REMAIN_MSEC = dResultSecond * (double)1000.0;
	}

	this->ClearNonDbNPC();
	this->ResetAllUserCsJoinSide();
	this->SendAllUserAnyMsg(Lang.GetText(0,163),1);
}

void CCastleSiege::SetState_ENDCYCLE(BOOL bSetRemainMsec)
{
	g_Log.Add("SET SetState_ENDCYCLE()");

	SYSTEMTIME tmNowDate;

	GetLocalTime(&tmNowDate);

	tmNowDate.wHour = 0;
	tmNowDate.wMinute = 0;
	tmNowDate.wSecond = 0;
	tmNowDate.wMilliseconds = 0;

	tmNowDate = this->m_tmSiegeEndSchedule;

	this->m_tmStartDate = this->m_tmSiegeEndSchedule;

	GetNextDay(&tmNowDate, this->m_iCastleSiegeCycle, 0, 0, 0);

	GetNextDay(&this->m_tmSiegeEndSchedule, this->m_iCastleSiegeCycle, 0, 0, 0);

	this->m_tmEndDate = tmNowDate;

	g_Log.Add("[CastleSiege] State - End Cycle Schedule : (%d-%d-%d(%d:%d:%d)) (%d-%d-%d(%d:%d:%d))", this->m_tmStartDate.wYear, this->m_tmStartDate.wMonth, this->m_tmStartDate.wDay, this->m_tmStartDate.wHour, this->m_tmStartDate.wMinute, this->m_tmStartDate.wSecond, this->m_tmEndDate.wYear, this->m_tmEndDate.wMonth, this->m_tmEndDate.wDay, this->m_tmEndDate.wHour, this->m_tmEndDate.wMinute, this->m_tmEndDate.wSecond);
	g_Log.Add("[CastleSiege] State - End Cycle : Date-Changing Info (%d-%d-%d) (%d-%d-%d)", this->m_tmStartDate.wYear, this->m_tmStartDate.wMonth, this->m_tmStartDate.wDay, this->m_tmEndDate.wYear, this->m_tmEndDate.wMonth, this->m_tmEndDate.wDay);

	GS_GDReqSiegeDateChange(this->m_iMapSvrGroup, -1, this->m_tmStartDate.wYear, this->m_tmStartDate.wMonth, this->m_tmStartDate.wDay, this->m_tmEndDate.wYear, this->m_tmEndDate.wMonth, this->m_tmEndDate.wDay);
	GS_GDReqResetRegSiegeInfo(this->m_iMapSvrGroup);
	GS_GDReqResetSiegeGuildInfo(this->m_iMapSvrGroup);

	this->SetState(CASTLESIEGE_STATE_REGSIEGE, TRUE);
}

void CCastleSiege::ProcState_NONE()
{

}

void CCastleSiege::ProcState_IDLE_1()
{
	int iTICK_MSEC = ( GetTickCount() - this->m_iCS_TICK_COUNT );

	if( iTICK_MSEC >= 1000 )
	{
		this->m_iCS_REMAIN_MSEC -= iTICK_MSEC;
		this->m_iCS_TICK_COUNT = GetTickCount();
		g_Log.AddC(TColor::Green,  "RUN ProcState_IDLE_1()\tLEFT-MSEC:%d", this->m_iCS_REMAIN_MSEC);
	}

	if( this->m_iCS_REMAIN_MSEC <= 0 )
	{
		this->SetState(CASTLESIEGE_STATE_REGSIEGE, TRUE);
	}
}

void CCastleSiege::ProcState_REGSIEGE()
{
	int iTICK_MSEC = ( GetTickCount() - this->m_iCS_TICK_COUNT );

	if( iTICK_MSEC >= 1000 )
	{
		this->m_iCS_REMAIN_MSEC -= iTICK_MSEC;
		this->m_iCS_TICK_COUNT = GetTickCount();
		
		if( (GetTickCount() - this->m_dwEVENT_MSG_TICK_COUNT) > 1800000 )
		{
			this->SendAllUserAnyMsg(Lang.GetText(0,157), 1);
			this->m_dwEVENT_MSG_TICK_COUNT = GetTickCount();
		}
		g_Log.AddC(TColor::Green,  "RUN ProcState_REGSIEGE()\tLEFT-MSEC:%d", this->m_iCS_REMAIN_MSEC);
	}

	if( this->m_iCS_REMAIN_MSEC <= 0 )
	{
		this->SetState(CASTLESIEGE_STATE_IDLE_2, TRUE);
	}
}

void CCastleSiege::ProcState_IDLE_2()
{
	int iTICK_MSEC = ( GetTickCount() - this->m_iCS_TICK_COUNT );

	if( iTICK_MSEC >= 1000 )
	{
		this->m_iCS_REMAIN_MSEC -= iTICK_MSEC;
		this->m_iCS_TICK_COUNT = GetTickCount();
		g_Log.AddC(TColor::Green,  "RUN ProcState_IDLE_2()\tLEFT-MSEC:%d", this->m_iCS_REMAIN_MSEC);
	}

	if( this->m_iCS_REMAIN_MSEC <= 0 )
	{
		this->SetState(CASTLESIEGE_STATE_REGMARK, TRUE);
	}
}

void CCastleSiege::ProcState_REGMARK()
{
	int iTICK_MSEC = ( GetTickCount() - this->m_iCS_TICK_COUNT );

	if( iTICK_MSEC >= 1000 )
	{
		this->m_iCS_REMAIN_MSEC -= iTICK_MSEC;
		this->m_iCS_TICK_COUNT = GetTickCount();
		
		if( (GetTickCount() - this->m_dwEVENT_MSG_TICK_COUNT) > 1800000 )
		{
			this->SendAllUserAnyMsg(Lang.GetText(0,158), 1);
			this->m_dwEVENT_MSG_TICK_COUNT = GetTickCount();
		}
		g_Log.AddC(TColor::Green,  "RUN ProcState_REGMARK()\tLEFT-MSEC:%d", this->m_iCS_REMAIN_MSEC);
	}

	if( this->m_iCS_REMAIN_MSEC <= 0 )
	{
		this->SetState(CASTLESIEGE_STATE_IDLE_3, TRUE);
	}
}

void CCastleSiege::ProcState_IDLE_3()
{
	int iTICK_MSEC = ( GetTickCount() - this->m_iCS_TICK_COUNT );

	if( iTICK_MSEC >= 1000 )
	{
		this->m_iCS_REMAIN_MSEC -= iTICK_MSEC;
		this->m_iCS_TICK_COUNT = GetTickCount();
		
		if( (GetTickCount() - this->m_dwEVENT_MSG_TICK_COUNT) > 7200000 )
		{
			this->SendAllUserAnyMsg(Lang.GetText(0,159), 1);
			this->m_dwEVENT_MSG_TICK_COUNT = GetTickCount();
		}

		g_Log.AddC(TColor::Green,  "RUN ProcState_IDLE_3()\tLEFT-MSEC:%d", this->m_iCS_REMAIN_MSEC);
	}

	if( this->m_iCS_REMAIN_MSEC <= 0 )
	{
		this->SetState(CASTLESIEGE_STATE_NOTIFY, TRUE);
	}
}

void CCastleSiege::ProcState_NOTIFY()
{
	int iTICK_MSEC = ( GetTickCount() - this->m_iCS_TICK_COUNT );

	if( iTICK_MSEC >= 1000 )
	{
		this->m_iCS_REMAIN_MSEC -= iTICK_MSEC;
		this->m_iCS_TICK_COUNT = GetTickCount();
		
		if( (GetTickCount() - this->m_dwEVENT_MSG_TICK_COUNT) > 1800000 )
		{
			this->SendAllUserAnyMsg(Lang.GetText(0,160), 1);
			this->m_dwEVENT_MSG_TICK_COUNT = GetTickCount();
		}
		g_Log.AddC(TColor::Green,  "RUN ProcState_NOTIFY()\tLEFT-MSEC:%d", this->m_iCS_REMAIN_MSEC);
	}

	if( this->m_iCS_REMAIN_MSEC <= 0 )
	{
		this->SetState(CASTLESIEGE_STATE_READYSIEGE, TRUE);
	}
}

void CCastleSiege::ProcState_READYSIEGE()
{
	int iTICK_MSEC = ( GetTickCount() - this->m_iCS_TICK_COUNT );

	if( iTICK_MSEC >= 1000 )
	{
		this->m_iCS_REMAIN_MSEC -= iTICK_MSEC;
		this->m_iCS_TICK_COUNT = GetTickCount();
		
		if( (GetTickCount() - this->m_dwEVENT_MSG_TICK_COUNT) > 1800000 )
		{
			this->SendAllUserAnyMsg(Lang.GetText(0,161), 1);
			this->m_dwEVENT_MSG_TICK_COUNT = GetTickCount();
		}
		if( this->m_iCS_REMAIN_MSEC <= 300000 )
		{
			if( ( GetTickCount() - this->m_dwCS_STARTTIME_TICK_COUNT ) > 60000 )
			{
				char szBuff[256]={0};
				wsprintf(szBuff, Lang.GetText(0,193), (this->m_iCS_REMAIN_MSEC/60000)+1);
				this->SendMapServerGroupMsg(szBuff);
				this->m_dwCS_STARTTIME_TICK_COUNT = GetTickCount();
			}
		}
		g_Log.AddC(TColor::Green,  "RUN ProcState_READYSIEGE()\tLEFT-MSEC:%d", this->m_iCS_REMAIN_MSEC);
	}

	if( this->m_iCS_REMAIN_MSEC <= 0 )
	{
		this->SetState(CASTLESIEGE_STATE_STARTSIEGE, TRUE);
	}
}

void CCastleSiege::ProcState_STARTSIEGE()
{
	this->CheckMiddleWinnerGuild();

	int iTICK_MSEC = ( GetTickCount() - this->m_iCS_TICK_COUNT );

	if( iTICK_MSEC >= 1000 )
	{
		this->m_iCS_REMAIN_MSEC -= iTICK_MSEC;
		this->m_iCS_TICK_COUNT = GetTickCount();
		
		if( (GetTickCount() - this->m_dwCHECK_GATE_ALIVE_COUNT) > 5000 )
		{
			this->CheckCsDbNpcAlive();
			this->m_dwCHECK_GATE_ALIVE_COUNT = GetTickCount();
		}

		if( (GetTickCount() - this->m_dwCS_LEFTTIME_TICK_COUNT) > 180000 )
		{
			if( this->m_iCS_REMAIN_MSEC > 180000 )
			{
				GSProtocol.GCAnsCsLeftTimeAlarm( (this->m_iCS_REMAIN_MSEC/1000)/3600, ((this->m_iCS_REMAIN_MSEC/1000)%3600)/60);
				this->m_dwCS_LEFTTIME_TICK_COUNT = GetTickCount();
			}
		}

		if( this->m_iCS_REMAIN_MSEC <= 180000 )
		{
			if( (GetTickCount() - this->m_dwCS_LEFTTIME_TICK_COUNT) > 60000 )
			{
				GSProtocol.GCAnsCsLeftTimeAlarm( ((this->m_iCS_REMAIN_MSEC/1000)/3600), ((this->m_iCS_REMAIN_MSEC/1000)%3600)/60);
				this->m_dwCS_LEFTTIME_TICK_COUNT = GetTickCount();
			}
		}

		if( this->m_dwCS_JOINSIDE_REFRESH_TOTAL_COUNT < 3 )
		{
			if( (GetTickCount() - this->m_dwCS_JOINSIDE_REFRESH_TICK_COUNT) > 10000 )
			{
				this->SetAllUserCsJoinSide();
				this->m_dwCS_JOINSIDE_REFRESH_TICK_COUNT = GetTickCount();
				this->m_dwCS_JOINSIDE_REFRESH_TOTAL_COUNT++;
			}
		}

		if( (GetTickCount() - this->m_dwCS_MINIMAP_SEND_TICK_COUNT) > 3000 )
		{
			this->OperateMiniMapWork();
			this->m_dwCS_MINIMAP_SEND_TICK_COUNT = GetTickCount();
		}

		if( (GetTickCount() - this->m_dwEVENT_MSG_TICK_COUNT) > 1800000 )
		{
			this->SendAllUserAnyMsg(Lang.GetText(0,162), 1);
			this->m_dwEVENT_MSG_TICK_COUNT = GetTickCount();
		}

		g_Log.AddC(TColor::Green,  "RUN ProcState_STARTSIEGE()\tLEFT-MSEC:%d", this->m_iCS_REMAIN_MSEC);
	}
	
	if( this->m_iCS_REMAIN_MSEC <= 0 )
	{
		if( this->CheckCastleSiegeResult() == TRUE )
		{
			GS_GDReqResetCastleTaxInfo(this->m_iMapSvrGroup);
		}

		this->ReSpawnEnemyUser(FALSE);

		if( this->m_btIsCastleOccupied == 1)
		{
			GS_GDReqCastleOwnerChange(this->m_iMapSvrGroup, this->m_btIsCastleOccupied, this->m_szCastleOwnerGuild);
		}
		
		GS_GDReqSiegeEndedChange(this->m_iMapSvrGroup, TRUE);
		
		this->StoreDbNpc();
		this->NotifyAllUserCsStartState(0);
		
		GS_GDReqResetSiegeGuildInfo(this->m_iMapSvrGroup);
		
		this->SetState(CASTLESIEGE_STATE_ENDSIEGE, TRUE);
	}
}

void CCastleSiege::ProcState_ENDSIEGE()
{
	int iTICK_MSEC = (GetTickCount() - this->m_iCS_TICK_COUNT);

	if( iTICK_MSEC >= 1000 )
	{
		this->m_iCS_REMAIN_MSEC -= iTICK_MSEC;
		this->m_iCS_TICK_COUNT = GetTickCount();

		g_Log.AddC(TColor::Green,  "RUN ProcState_ENDSIEGE()\tLEFT-MSEC:%d", this->m_iCS_REMAIN_MSEC);
	}

	if( this->m_iCS_REMAIN_MSEC <= 0 )
	{
		this->SetState(CASTLESIEGE_STATE_ENDCYCLE, TRUE);
	}
}

void CCastleSiege::ProcState_ENDCYCLE()
{
	int iTICK_MSEC = (GetTickCount() - this->m_iCS_TICK_COUNT);

	if( iTICK_MSEC >= 1000 )
	{
		this->m_iCS_REMAIN_MSEC -= iTICK_MSEC;
		this->m_iCS_TICK_COUNT = GetTickCount();

		g_Log.AddC(TColor::Green,  "RUN ProcState_ENDCYCLE()\tLEFT-MSEC:%d", this->m_iCS_REMAIN_MSEC);
	}

	if( this->m_iCS_REMAIN_MSEC <= 0 )
	{
		this->SetState(CASTLESIEGE_STATE_IDLE_1, TRUE);
	}
}

BOOL CCastleSiege::FirstCreateDbNPC()
{
	char cBUFFER[4240];

	CSP_REQ_NPCSAVEDATA* lpMsg;
	CSP_NPCSAVEDATA* lpMsgBody;
	
	if( this->m_iCastleDataLoadState != CASTLESIEGE_DATALOAD_4 )
	{
		g_Log.AddC(TColor::Red,  "[CastleSiege] CCastleSiege::FirstCreateDbNPC() - m_iCastleDataLoadState != CASTLESIEGE_DATALOAD_4");
		return FALSE;
	}
	
	lpMsg = (CSP_REQ_NPCSAVEDATA*)cBUFFER;
	lpMsgBody = (CSP_NPCSAVEDATA*)&cBUFFER[sizeof(CSP_REQ_NPCSAVEDATA)];
	int iCOUNT = 0;

	EnterCriticalSection(&this->m_critNpcData);
	
	std::vector<_CS_NPC_DATA>::iterator it = this->m_vtNpcData.begin();
	BOOL bExist = FALSE;

	while( it != this->m_vtNpcData.end() )
	{
		_CS_NPC_DATA & pNpcData = _CS_NPC_DATA(*it);

		if( pNpcData.m_bNPC_DBSAVE == TRUE )
		{
			it->SetBaseValue();
			it->m_iNPC_LIVE = 1;
			it->m_iNPC_SIDE = 1;

			lpMsgBody[iCOUNT].iNpcNumber = pNpcData.m_iNPC_NUM;
			lpMsgBody[iCOUNT].iNpcIndex = pNpcData.m_iNPC_INDEX;
			lpMsgBody[iCOUNT].iNpcDfLevel = pNpcData.m_iNPC_BASE_DF_LEVEL;
			lpMsgBody[iCOUNT].iNpcRgLevel = pNpcData.m_iNPC_BASE_RG_LEVEL;
			lpMsgBody[iCOUNT].iNpcMaxHp = pNpcData.m_iNPC_BASE_MAXHP;
			lpMsgBody[iCOUNT].iNpcHp = pNpcData.m_iNPC_BASE_HP;
			lpMsgBody[iCOUNT].btNpcX = pNpcData.m_iNPC_BASE_SX;
			lpMsgBody[iCOUNT].btNpcY = pNpcData.m_iNPC_BASE_SY;
			lpMsgBody[iCOUNT].btNpcDIR = pNpcData.m_iNPC_BASE_DIR;
			iCOUNT++;
		}
		it++;
	}

	LeaveCriticalSection(&this->m_critNpcData);

	lpMsg->wMapSvrNum = this->m_iMapSvrGroup;
	lpMsg->iCount = iCOUNT;
	lpMsg->h.set((LPBYTE)&cBUFFER, 0x84, (iCOUNT * sizeof(CSP_NPCSAVEDATA)) + sizeof(CSP_REQ_NPCSAVEDATA));
	wsDataCli.DataSend((char*)cBUFFER, (iCOUNT * sizeof(CSP_NPCSAVEDATA)) + sizeof(CSP_REQ_NPCSAVEDATA));

	return TRUE;
}

BOOL CCastleSiege::SetCastleInitData(CSP_ANS_CSINITDATA* lpMsg)
{
	if( this->m_iCastleDataLoadState != CASTLESIEGE_DATALOAD_3 )
	{
		g_Log.AddC(TColor::Red,  "[CastleSiege] CCastleSiege::SetCastleInitData() - m_iCastleDataLoadState != CASTLESIEGE_DATALOAD_3");
		return FALSE;
	}

	if( lpMsg == NULL )
	{
		return FALSE;
	}

	memset(&this->m_tmStartDate, 0, sizeof(this->m_tmStartDate));
	memset(&this->m_tmEndDate, 0, sizeof(this->m_tmEndDate));
	
	this->m_tmStartDate.wYear = lpMsg->wStartYear;
	this->m_tmStartDate.wMonth = lpMsg->btStartMonth;
	this->m_tmStartDate.wDay = lpMsg->btStartDay;
	this->m_tmEndDate.wYear = lpMsg->wEndYear;
	this->m_tmEndDate.wMonth = lpMsg->btEndMonth;
	this->m_tmEndDate.wDay = lpMsg->btEndDay;
	this->m_btIsSiegeGuildList = lpMsg->btIsSiegeGuildList;
	this->m_btIsSiegeEnded = lpMsg->btIsSiegeEnded;
	this->m_btIsCastleOccupied = lpMsg->btIsCastleOccupied;

	memset(&this->m_szCastleOwnerGuild, 0, sizeof(this->m_szCastleOwnerGuild));
	memcpy(&this->m_szCastleOwnerGuild, lpMsg->szCastleOwnGuild, sizeof(lpMsg->szCastleOwnGuild));
	
	this->m_i64CastleMoney = lpMsg->i64CastleMoney;
	this->m_iTaxRateChaos = lpMsg->iTaxRateChaos;
	this->m_iTaxRateStore = lpMsg->iTaxRateStore;
	this->m_iTaxHuntZone = lpMsg->iTaxHuntZone;

	int iSTART_DATE_NUM = MACRO2(( MACRO1(this->m_tmStartDate.wDay) | MACRO1(this->m_tmStartDate.wMonth) << 8 ) & 0xFFFF ) | MACRO2(this->m_tmStartDate.wYear) << 16;
	int iEND_DATE_NUM = MACRO2(( MACRO1(this->m_tmEndDate.wDay) | MACRO1(this->m_tmEndDate.wMonth) << 8 ) & 0xFFFF ) | MACRO2(this->m_tmEndDate.wYear) << 16;
	
	this->m_tmSiegeEndSchedule = this->m_tmStartDate;

	SYSTEMTIME EndTime;
	GetStateDate(CASTLESIEGE_STATE_ENDCYCLE, &EndTime);

	g_Log.Add("[CastleSiege] CCastleSiege::LoadData() - Siege Schedule Start Date (%d-%d-%d)", m_tmStartDate.wYear, m_tmStartDate.wMonth, m_tmStartDate.wDay);
	
	g_Log.Add("[CastleSiege] CCastleSiege::LoadData() - Siege Schedule Date (%d-%d-%d)", EndTime.wDay, EndTime.wHour, EndTime.wMinute);

	GetNextDay(&m_tmSiegeEndSchedule, EndTime.wDay, EndTime.wHour, EndTime.wMinute, 0);

	g_Log.Add("[CastleSiege] CCastleSiege::LoadData() - Siege Schedule End Date (%d-%d-%d(%d:%d:%d)", this->m_tmSiegeEndSchedule.wYear, this->m_tmSiegeEndSchedule.wMonth, this->m_tmSiegeEndSchedule.wDay, this->m_tmSiegeEndSchedule.wHour, this->m_tmSiegeEndSchedule.wMinute, this->m_tmSiegeEndSchedule.wSecond);

	if( iSTART_DATE_NUM > iEND_DATE_NUM )
	{
		g_Log.AddC(TColor::Red,  "[CastleSiege] CCastleSiege::SetCastleInitData() - iSTART_DATE_NUM > iEND_DATE_NUM");
		return FALSE;
	}
	return TRUE;
}

BOOL CCastleSiege::SetCastleNpcData(CSP_CSINITDATA* lpMsg, int iCOUNT)
{
	if( this->m_iCastleDataLoadState != CASTLESIEGE_DATALOAD_3 )
	{
		g_Log.AddC(TColor::Red,  "[CastleSiege] CCastleSiege::SetCastleNpcData() - m_iCastleDataLoadState != CASTLESIEGE_DATALOAD_3");
		return FALSE;
	}

	if( lpMsg == NULL )
	{
		return FALSE;
	}

	if( iCOUNT < 0 )
	{
		return FALSE;
	}

	std::vector<_CS_NPC_DATA>::iterator it;

	for( int iNPC_COUNT = 0; iNPC_COUNT < iCOUNT; iNPC_COUNT++ )
	{
		EnterCriticalSection(&this->m_critNpcData);

		it = this->m_vtNpcData.begin();

		BOOL bExist = FALSE;

		while( it != this->m_vtNpcData.end() )
		{
			_CS_NPC_DATA & NpcData = _CS_NPC_DATA(*it);

			if( NpcData.m_iNPC_NUM == lpMsg[iNPC_COUNT].iNpcNumber )
			{
				if( NpcData.m_iNPC_INDEX == lpMsg[iNPC_COUNT].iNpcIndex )
				{
					it->m_iNPC_LIVE	= 1;
					it->m_iNPC_DF_LEVEL = lpMsg[iNPC_COUNT].iNpcDfLevel;
					it->m_iNPC_RG_LEVEL = lpMsg[iNPC_COUNT].iNpcRgLevel;
					it->m_iNPC_MAXHP = lpMsg[iNPC_COUNT].iNpcMaxHp;
					it->m_iNPC_HP = lpMsg[iNPC_COUNT].iNpcHp;
					it->m_iNPC_SX = lpMsg[iNPC_COUNT].btNpcX;
					it->m_iNPC_SY = lpMsg[iNPC_COUNT].btNpcY;
					it->m_iNPC_DX = -1;
					it->m_iNPC_DY = -1;
					it->m_iNPC_DIR = lpMsg[iNPC_COUNT].btNpcDIR;
					it->m_iNPC_SIDE = 1;
					bExist = TRUE;
					break;
				}
			}
			it++;
		}
		
		LeaveCriticalSection(&this->m_critNpcData);

		if( bExist == FALSE )
		{
			_CS_NPC_DATA pNpcData2;
			pNpcData2.m_bIN_USE	= TRUE;
			pNpcData2.m_iNPC_NUM = lpMsg[iNPC_COUNT].iNpcNumber;
			pNpcData2.m_iNPC_INDEX = lpMsg[iNPC_COUNT].iNpcIndex;
			pNpcData2.m_iNPC_LIVE = 1;
			pNpcData2.m_iNPC_SIDE = 1;
			pNpcData2.m_iNPC_BASE_DF_LEVEL = lpMsg[iNPC_COUNT].iNpcDfLevel;
			pNpcData2.m_iNPC_DF_LEVEL = pNpcData2.m_iNPC_BASE_DF_LEVEL;
			pNpcData2.m_iNPC_BASE_RG_LEVEL = lpMsg[iNPC_COUNT].iNpcRgLevel;
			pNpcData2.m_iNPC_RG_LEVEL = pNpcData2.m_iNPC_BASE_RG_LEVEL;

			pNpcData2.m_iNPC_BASE_MAXHP = lpMsg[iNPC_COUNT].iNpcMaxHp;
			pNpcData2.m_iNPC_MAXHP = pNpcData2.m_iNPC_BASE_MAXHP;
			pNpcData2.m_iNPC_BASE_HP = lpMsg[iNPC_COUNT].iNpcHp;
			pNpcData2.m_iNPC_HP = pNpcData2.m_iNPC_BASE_HP;
			pNpcData2.m_iNPC_BASE_SX = lpMsg[iNPC_COUNT].btNpcX;
			pNpcData2.m_iNPC_SX	= pNpcData2.m_iNPC_BASE_SX;
			pNpcData2.m_iNPC_BASE_SY = lpMsg[iNPC_COUNT].btNpcY;
			pNpcData2.m_iNPC_SY	= pNpcData2.m_iNPC_BASE_SY;
			pNpcData2.m_iNPC_BASE_DX = -1;
			pNpcData2.m_iNPC_DX	= pNpcData2.m_iNPC_BASE_DX;
			pNpcData2.m_iNPC_BASE_DY = -1;
			pNpcData2.m_iNPC_DY	= pNpcData2.m_iNPC_BASE_DY;
			pNpcData2.m_iNPC_BASE_DIR = lpMsg[iNPC_COUNT].btNpcDIR;
			pNpcData2.m_iNPC_DIR = pNpcData2.m_iNPC_BASE_DIR;

			EnterCriticalSection(&this->m_critNpcData);
			this->m_vtNpcData.push_back(pNpcData2);
			LeaveCriticalSection(&this->m_critNpcData);
		}
	}
	return TRUE;
}

int CCastleSiege::CheckSync()
{
	if(this->m_bFileDataLoadOK == FALSE)
	{
		g_Log.AddC(TColor::Red, "[CastleSiege] CCastleSiege::CheckSync() - m_bFileDataLoadOK == FALSE");
		return false;
	}

	this->m_iCS_TICK_COUNT = GetTickCount();

	_SYSTEMTIME tmToDay;

	GetLocalTime(&tmToDay);
	int iTODAY_DATE_NUM = MACRO2(( MACRO1(tmToDay.wDay) | MACRO1(tmToDay.wMonth) << 8 ) & 0xFFFF ) | MACRO2(tmToDay.wYear) << 16;
	int iCastleInitState = 0;

	if(this->m_bFixCastleCycleStartDate != FALSE)
	{
		this->m_tmStartDate.wYear = this->m_tmFixCastleCycleStartDate.wYear;
		this->m_tmStartDate.wMonth = this->m_tmFixCastleCycleStartDate.wMonth;
		this->m_tmStartDate.wDay = this->m_tmFixCastleCycleStartDate.wDay;

		this->m_tmEndDate = this->m_tmStartDate;
		GetNextDay(&this->m_tmEndDate, this->m_iCastleSiegeCycle, 0, 0, 0);
	}

	int iEVENT_END_DATE_NUM = MACRO2(( MACRO1(m_tmEndDate.wDay) | MACRO1(m_tmEndDate.wMonth) << 8 ) & 0xFFFF ) | MACRO2(m_tmEndDate.wYear) << 16;

	if(iEVENT_END_DATE_NUM <= iTODAY_DATE_NUM)
	{
		this->m_bFixCastleCycleStartDate = FALSE;
		g_Log.MsgBox("[CastleSiege] CCastleSiege::CheckSync() - iEVENT_END_DATE_NUM (%04d-%02d-%02d) <= iTODAY_DATE_NUM (%04d-%02d-%02d)",this->m_tmEndDate.wYear,this->m_tmEndDate.wMonth,this->m_tmEndDate.wDay,tmToDay.wYear,tmToDay.wMonth,tmToDay.wDay);
		return false;
	}

	 if(this->m_bFixCastleCycleStartDate != FALSE)
	 {
		 GS_GDReqSiegeDateChange(this->m_iMapSvrGroup, -1, this->m_tmStartDate.wYear, this->m_tmStartDate.wMonth, this->m_tmStartDate.wDay, this->m_tmEndDate.wYear, this->m_tmEndDate.wMonth, this->m_tmEndDate.wDay);
	 }

	 if(this->m_bFixCastleStateStartDate != FALSE)
	 {
		 BOOL bSpecificStateExist = FALSE;

		 EnterCriticalSection(&this->m_critScheduleData);

		 std::vector<_CS_SCHEDULE_DATA>::iterator it = this->m_vtScheduleData.begin();

		 while(it != this->m_vtScheduleData.end())
		 {
			 _CS_SCHEDULE_DATA & pScheduleData = (_CS_SCHEDULE_DATA &)*it;

			 if(pScheduleData.m_iSTATE == this->m_iFixCastleSpecificState)
			 {
				 bSpecificStateExist = TRUE;
				 break;
			 }

			 it++;
		 }

		 LeaveCriticalSection(&this->m_critScheduleData);

		 if(bSpecificStateExist != FALSE)
		 {
			 time_t ttSTime;
			 time_t ttETime;
			 tm tmETime;
			 double dResultSecond;
			 memset(&tmETime,0,sizeof(tmETime));

			 tmETime.tm_year = this->m_tmFixCastleStateStartDate.wYear - 1900;
			 tmETime.tm_mon = this->m_tmFixCastleStateStartDate.wMonth - 1;
			 tmETime.tm_mday = this->m_tmFixCastleStateStartDate.wDay;
			 tmETime.tm_hour = this->m_tmFixCastleStateStartDate.wHour;
			 tmETime.tm_min = this->m_tmFixCastleStateStartDate.wMinute;

			 time(&ttSTime);
			 ttETime = mktime(&tmETime);

			 dResultSecond = difftime(ttETime,ttSTime);

			 this->m_iCS_REMAIN_MSEC = dResultSecond * (double)1000.0;
			 this->m_iCastleSiegeState = this->m_iFixCastleSpecificState-1;
			 this->SetState(this->m_iCastleSiegeState, FALSE);
			 return true;
		 }
		 else
		 {
			 g_Log.MsgBox("[CastleSiege] CCastleSiege::CheckSync() - FIXED STATE:%d NOT FOUND IN SCHEDULE (P.S.> Check File 'MuCastleData.dat', 'commonserver.cfg')", this->m_iFixCastleSpecificState);
			 this->m_bFixCastleStateStartDate = FALSE;
		 }
	 }

	 if(this->m_btIsSiegeEnded != 0)
	 {
		 this->SetState(CASTLESIEGE_STATE_ENDSIEGE,TRUE);
		 return true;
	 }

	 int iEVENT_START_DATE_NUM = MACRO2(( MACRO1(this->m_tmStartDate.wDay) | MACRO1(this->m_tmStartDate.wMonth) << 8 ) & 0xFFFF ) | MACRO2(this->m_tmStartDate.wYear) << 16;

	 if(iEVENT_START_DATE_NUM > iTODAY_DATE_NUM)
	 {
		 this->SetState(CASTLESIEGE_STATE_IDLE_1,TRUE);
		 return true;
	 }
	 else
	 {
		 BOOL bStateSetted = FALSE;
		_CS_SCHEDULE_DATA pScheData_FR;
		_CS_SCHEDULE_DATA pScheData_RR;
		std::vector<_CS_SCHEDULE_DATA>::iterator it;

		 EnterCriticalSection(&this->m_critScheduleData);

		 it = this->m_vtScheduleData.begin();

		 while(it != this->m_vtScheduleData.end())
		 {
			_CS_SCHEDULE_DATA & pScheduleData = (_CS_SCHEDULE_DATA &)*it;;
			_SYSTEMTIME tmSchduleDate;

			tmSchduleDate = this->m_tmStartDate;
			GetNextDay(&tmSchduleDate,pScheduleData.m_iADD_DAY,0,0,0);
			tmSchduleDate.wHour = pScheduleData.m_iADD_HOUR;
			tmSchduleDate.wMinute = pScheduleData.m_iADD_MIN;

			__int64 i64ScheduleDateNum;
			__int64 i64ToDayDateNum;

			i64ScheduleDateNum = (MACRO2(tmSchduleDate.wMinute) | MACRO2(tmSchduleDate.wHour) << 16) | ((__int64)(MACRO2(( MACRO1(tmSchduleDate.wDay) | MACRO1(tmSchduleDate.wMonth) << 8 ) & 0xFFFF ) | MACRO2(tmSchduleDate.wYear) << 16) << 0x20);
			i64ToDayDateNum = (MACRO2(tmToDay.wMinute) | MACRO2(tmToDay.wHour) << 16) | ((__int64)(MACRO2(( MACRO1(tmToDay.wDay) | MACRO1(tmToDay.wMonth) << 8 ) & 0xFFFF ) | MACRO2(tmToDay.wYear) << 16) << 0x20);

			 if(i64ScheduleDateNum > i64ToDayDateNum)
			 {
				 bStateSetted = TRUE;
				 pScheData_RR = pScheduleData;
				 this->m_iCastleSiegeState = iCastleInitState;
				 break;
			 }

			 iCastleInitState = pScheduleData.m_iSTATE;
			 pScheData_FR = pScheduleData;

			 it++;
		 }

		 LeaveCriticalSection(&this->m_critScheduleData);

		 _SYSTEMTIME tmSchduleDate;
	
		time_t ttSTime;
		time_t ttETime;
		struct tm tmETime;
		double dResultSecond;

		if(bStateSetted == FALSE)
		{
			g_Log.MsgBox("[CastleSiege] CCastleSiege::CheckSync() - bStateSetted == FALSE  START_DATE (%04d-%02d-%02d), END_DATE (%04d-%02d-%02d)", this->m_tmStartDate.wYear, this->m_tmStartDate.wMonth, this->m_tmStartDate.wDay, this->m_tmEndDate.wYear, this->m_tmEndDate.wMonth, this->m_tmEndDate.wDay);
			return false;
		}

		tmSchduleDate = this->m_tmStartDate;
		GetNextDay(&tmSchduleDate,pScheData_RR.m_iADD_DAY,0,0,0);
		tmSchduleDate.wHour = pScheData_RR.m_iADD_HOUR;
		tmSchduleDate.wMinute = pScheData_RR.m_iADD_MIN;

		memset(&tmETime,0,sizeof(tmETime));

		tmETime.tm_year = tmSchduleDate.wYear - 1900;
		tmETime.tm_mon = tmSchduleDate.wMonth - 1;
		tmETime.tm_mday = tmSchduleDate.wDay;
		tmETime.tm_hour = tmSchduleDate.wHour;
		tmETime.tm_min = tmSchduleDate.wMinute;

		time(&ttSTime);
		ttETime = mktime(&tmETime);

		dResultSecond = difftime(ttETime,ttSTime);
		this->m_iCS_REMAIN_MSEC = dResultSecond * (double)1000.0;

		if(this->m_iCastleSiegeState == CASTLESIEGE_STATE_STARTSIEGE)
		{
			this->m_iCastleSiegeState = CASTLESIEGE_STATE_READYSIEGE;
			this->m_iCS_REMAIN_MSEC = 900000;
		}
		else if(this->m_iCastleSiegeState == CASTLESIEGE_STATE_READYSIEGE)
		{
			if(m_iCS_REMAIN_MSEC < 900000)
			{
				this->m_iCastleSiegeState = CASTLESIEGE_STATE_READYSIEGE;
				this->m_iCS_REMAIN_MSEC = 900000;
			}
		}

		this->SetState(this->m_iCastleSiegeState,FALSE);
	 }
	return true;
}

void CCastleSiege::GetNextDay(_SYSTEMTIME * st,int iAfterDay,int iAfterHour,int iAfterMin,int iAfterSec) //Identical
{
	__int64 i64;
	_FILETIME fst;
	st->wDayOfWeek = 0;
	st->wMilliseconds = 0;
	st->wSecond = 0;
	SystemTimeToFileTime(st,&fst);
	i64 = ((__int64)fst.dwHighDateTime << 0x20) + fst.dwLowDateTime;
	i64 = i64 + (__int64)iAfterDay * 864000000000 + (__int64)iAfterHour * 36000000000 +(__int64) iAfterMin * 600000000 + (__int64)iAfterSec * 10000000;
	fst.dwHighDateTime = i64 >> 0x20;
	fst.dwLowDateTime = i64 & 0xFFFFFFFF;
	FileTimeToSystemTime(&fst,st);
}

void CCastleSiege::GetStateDate(int iCastleSiegeState, SYSTEMTIME* st)
{
	EnterCriticalSection(&this->m_critScheduleData);

	for( std::vector<_CS_SCHEDULE_DATA>::iterator it = this->m_vtScheduleData.begin(); it != this->m_vtScheduleData.end(); it++ )
	{
		_CS_SCHEDULE_DATA & ScheduleData = *it;

		if( ScheduleData.m_iSTATE == iCastleSiegeState )
		{
			st->wDay = ScheduleData.m_iADD_DAY;
			st->wHour = ScheduleData.m_iADD_HOUR;
			st->wMinute = ScheduleData.m_iADD_MIN;
			st->wSecond = 0;
			break;
		}
	}
	LeaveCriticalSection(&this->m_critScheduleData);
}

int CCastleSiege::GetStateGapSec(int iCastleSiegeState)
{
	int iGAP_SEC = -1;

	EnterCriticalSection(&this->m_critScheduleData);

	for(std::vector<_CS_SCHEDULE_DATA>::iterator it = this->m_vtScheduleData.begin(); it != this->m_vtScheduleData.end(); it++)
	{
		_CS_SCHEDULE_DATA & pScheduleData = *it;

		if(pScheduleData.m_iSTATE == iCastleSiegeState)
		{
			iGAP_SEC = pScheduleData.m_iGAP_SEC;
			break;
		}
	}

	LeaveCriticalSection(&this->m_critScheduleData);

	return iGAP_SEC;
}

int CCastleSiege::CalcCastleLeftSiegeDate()
{
	memset(&this->m_tmLeftCastleSiegeDate, 0, sizeof(this->m_tmLeftCastleSiegeDate));

	if( this->m_iCastleSiegeState < CASTLESIEGE_STATE_IDLE_1 || CASTLESIEGE_STATE_ENDCYCLE < this->m_iCastleSiegeState )
	{
		return CASTLESIEGE_LEFTTIME_NOTRUNNING;
	}

	if( this->m_bDoRun == FALSE )
	{
		return CASTLESIEGE_LEFTTIME_NOTRUNNING;
	}

	if( this->m_iCastleSiegeState == CASTLESIEGE_STATE_STARTSIEGE )
	{
		return CASTLESIEGE_LEFTTIME_ONSIGE;
	}

	if( this->m_btIsSiegeEnded == 1 || this->m_iCastleSiegeState >= CASTLESIEGE_STATE_ENDSIEGE )
	{
		return CASTLESIEGE_LEFTTIME_ENDSIEGE;
	}

	int iGAP_SEC = CASTLESIEGE_LEFTTIME_ONSIGE;
	BOOL bAddStart = FALSE;

	EnterCriticalSection(&this->m_critScheduleData);
	
	for( std::vector<_CS_SCHEDULE_DATA>::iterator it = this->m_vtScheduleData.begin(); it != this->m_vtScheduleData.end(); it++ )
	{
		_CS_SCHEDULE_DATA & lpScheduleData = *it;

		if( lpScheduleData.m_iSTATE == 7 )
		{
			break;
		}

		if( bAddStart )
		{
			iGAP_SEC += lpScheduleData.m_iGAP_SEC;
		}

		if( lpScheduleData.m_iSTATE == this->m_iCastleSiegeState )
		{
			iGAP_SEC = (this->m_iCS_REMAIN_MSEC / 1000);
			bAddStart = TRUE;
		}
	}

	LeaveCriticalSection(&this->m_critScheduleData);

	if( iGAP_SEC >= 0 )
	{
		SYSTEMTIME tmLocalTime;
		GetLocalTime(&tmLocalTime);

		this->GetNextDay(&tmLocalTime, 0, 0, 0, iGAP_SEC);

		this->m_tmLeftCastleSiegeDate = tmLocalTime;

		if( iGAP_SEC == 0 )
		{
			g_Log.AddC(TColor::Red,  "[CastleSiege] CCastleSiege::CalcCastleLeftSiegeDate() - iGAP_SEC == 0");
		}

		return iGAP_SEC;
	}

	return CASTLESIEGE_LEFTTIME_NOTRUNNING;
}

BOOL CCastleSiege::GetCastleStateTerm(SYSTEMTIME * tmStateStartDate, SYSTEMTIME * tmStateEndDate)
{
	if( this->m_iCastleSiegeState < CASTLESIEGE_STATE_IDLE_1 || CASTLESIEGE_STATE_ENDCYCLE < this->m_iCastleSiegeState )
	{
		return FALSE;
	}

	if( this->m_bDoRun == FALSE )
	{
		return FALSE;
	}

	*tmStateStartDate = this->m_tmStateStartDate;

	GetLocalTime(tmStateEndDate);

	GetNextDay(tmStateEndDate, 0, 0, 0, this->m_iCS_REMAIN_MSEC / 1000);

	return TRUE;
}

int CCastleSiege::GetCurRemainSec()
{
	if( this->m_iCS_REMAIN_MSEC < 0 )
	{
		return -1;
	}

	return (this->m_iCS_REMAIN_MSEC / 1000);
}

void CCastleSiege::ClearDbNPC()
{
	for( int n = 0 ; n < g_ConfigRead.server.GetObjectMaxMonster(); n++ )
	{
		if( gObj[n].MapNumber == MAP_INDEX_CASTLESIEGE )
		{
			if( gObj[n].Class == 277 || gObj[n].Class == 283 || gObj[n].Class == 219 )
			{
				if( gObj[n].Class == 277 )
				{
					this->SetGateBlockState(gObj[n].X, gObj[n].Y, 1);
					gObjAddBuffEffect(&gObj[n], BUFFTYPE_CASTLE_DOOR_STATE, 0, 0, 0, 0, -10);
				}
				gObjDel(n);
			}
		}
	}
}

void CCastleSiege::ClearNonDbNPC()
{
	for(int n = 0; n < g_ConfigRead.server.GetObjectMaxMonster(); n++)
	{
		if(gObjIsConnected(n) && gObj[n].MapNumber == MAP_INDEX_CASTLESIEGE	&& gObj[n].m_btCsNpcType != NULL && gObj[n].m_btCsNpcType != 1)
		{
			if(gObj[n].Class == 278)
			{
				g_CsNPC_LifeStone.DeleteLifeStone(n);
			}

			if(gObj[n].Class == 287 || gObj[n].Class == 286)
			{
				g_CsNPC_Mercenary.DeleteMercenary(n);
			}

			if(gObj[n].Class == 219)
			{
				continue;
			}

			gObjDel(n);
		}
	}
}

void CCastleSiege::ClearNonDbNPC_MidWin()
{
	for( int n = 0 ; n < g_ConfigRead.server.GetObjectMaxMonster(); n++ )
	{
		if( gObjIsConnected(n) )
		{
			if( gObj[n].MapNumber == MAP_INDEX_CASTLESIEGE )
			{
				if( gObj[n].m_btCsNpcType && gObj[n].m_btCsNpcType != 1 )
				{
					if( gObj[n].Class == 278 )
					{
						g_CsNPC_LifeStone.DeleteLifeStone(n);
					}
					
					if( gObj[n].Class == 278 )
					{
						gObjDel(n);
					}
				}
			}
		}
	}
}

void CCastleSiege::ClearAllNPC()
{
	for(int n = 0; n < g_ConfigRead.server.GetObjectMaxMonster(); n++)
	{
		if(gObj[n].m_btCsNpcType)
		{
			gObjDel(n);
		}
	}
}

void CCastleSiege::CreateDbNPC()
{
	EnterCriticalSection(&this->m_critNpcData);

	std::vector<_CS_NPC_DATA>::iterator it = this->m_vtNpcData.begin();
	BOOL bExist = FALSE;

	while(it != this->m_vtNpcData.end())
	{
		_CS_NPC_DATA & pNpcData = _CS_NPC_DATA(*it);

		if(pNpcData.m_bIN_USE == 1 && pNpcData.m_iNPC_LIVE == 1)
		{
			int iNPC_INDEX = gObjAddMonster(MAP_INDEX_CASTLESIEGE);

			if(iNPC_INDEX >= 0)
			{
				gObjSetMonster(iNPC_INDEX, pNpcData.m_iNPC_NUM);
				gObj[iNPC_INDEX].m_PosNum = -1;
				gObj[iNPC_INDEX].X = pNpcData.m_iNPC_SX;
				gObj[iNPC_INDEX].Y = pNpcData.m_iNPC_SY;
				gObj[iNPC_INDEX].MapNumber = MAP_INDEX_CASTLESIEGE;
				gObj[iNPC_INDEX].TX = gObj[iNPC_INDEX].X;
				gObj[iNPC_INDEX].TY = gObj[iNPC_INDEX].Y;
				gObj[iNPC_INDEX].m_OldX = gObj[iNPC_INDEX].X;
				gObj[iNPC_INDEX].m_OldY = gObj[iNPC_INDEX].Y;
				gObj[iNPC_INDEX].Dir = pNpcData.m_iNPC_DIR;
				gObj[iNPC_INDEX].StartX = gObj[iNPC_INDEX].X;
				gObj[iNPC_INDEX].StartY = gObj[iNPC_INDEX].Y;
				gObj[iNPC_INDEX].DieRegen = 0;
				gObj[iNPC_INDEX].MaxRegenTime = 0;
				gObj[iNPC_INDEX].Life = pNpcData.m_iNPC_HP;
				gObj[iNPC_INDEX].MaxLife = pNpcData.m_iNPC_MAXHP;
				gObj[iNPC_INDEX].m_btCsNpcType = 1;

				int iNPC_DF_LEVEL = pNpcData.m_iNPC_DF_LEVEL;

				if(iNPC_DF_LEVEL < 0)
				{
					iNPC_DF_LEVEL = 0;
				}

				if(iNPC_DF_LEVEL > CS_MAX_UPGRADE)
				{
					iNPC_DF_LEVEL = CS_MAX_UPGRADE;
				}

				int iNPC_RG_LEVEL = pNpcData.m_iNPC_RG_LEVEL;

				if(iNPC_RG_LEVEL < 0)
				{
					iNPC_RG_LEVEL = 0;
				}

				if(iNPC_RG_LEVEL > CS_MAX_UPGRADE)
				{
					iNPC_RG_LEVEL = CS_MAX_UPGRADE;
				}

				switch(pNpcData.m_iNPC_NUM)
				{
				case 277:
					gObj[iNPC_INDEX].m_Defense = g_iNpcDefense_CGATE[iNPC_DF_LEVEL];
					gObj[iNPC_INDEX].m_MagicDefense = g_iNpcDefense_CGATE[iNPC_DF_LEVEL];
					gObj[iNPC_INDEX].m_btCsNpcDfLevel = iNPC_DF_LEVEL;
					break;
				case 283:
					gObj[iNPC_INDEX].m_Defense = g_iNpcDefense_CSTATUE[iNPC_DF_LEVEL];
					gObj[iNPC_INDEX].m_MagicDefense = g_iNpcDefense_CSTATUE[iNPC_DF_LEVEL];
					gObj[iNPC_INDEX].m_btCsNpcDfLevel = iNPC_DF_LEVEL;
					gObj[iNPC_INDEX].m_btCsNpcRgLevel = iNPC_RG_LEVEL;
					break;
				}

				it->m_btCsNpcExistVal1 = rand()%255+1; //Check
				gObj[iNPC_INDEX].m_btCsNpcExistVal1 = it->m_btCsNpcExistVal1;

				it->m_btCsNpcExistVal2 = rand()%256;
				gObj[iNPC_INDEX].m_btCsNpcExistVal2 = it->m_btCsNpcExistVal2;

				it->m_btCsNpcExistVal3 = rand()%256;
				gObj[iNPC_INDEX].m_btCsNpcExistVal3 = it->m_btCsNpcExistVal3;

				it->m_btCsNpcExistVal4 = rand()%256;
				gObj[iNPC_INDEX].m_btCsNpcExistVal4 = it->m_btCsNpcExistVal4;

				if(pNpcData.m_iNPC_NUM == 277)
				{
					this->SetGateBlockState(pNpcData.m_iNPC_SX,pNpcData.m_iNPC_SY,1);
					gObjAddBuffEffect(&gObj[iNPC_INDEX], BUFFTYPE_CASTLE_DOOR_STATE, 0, 0, 0, 0, -10);

					gObj[iNPC_INDEX].m_btCsGateOpen = 1;
					BOOL bCreateLever = FALSE;

					if(pNpcData.m_iCS_GATE_LEVER_INDEX == -1)
					{
						bCreateLever = TRUE;
					}
					else if(this->CheckLeverAlive(pNpcData.m_iCS_GATE_LEVER_INDEX) == FALSE)
					{
						bCreateLever = TRUE;
					}

					if(bCreateLever != FALSE)
					{
						int iLeverIndex = this->CreateCsGateLever(pNpcData.m_iNPC_SX,pNpcData.m_iNPC_SY+4);

						if(ObjectMaxRange(iLeverIndex))
						{
							it->m_iCS_GATE_LEVER_INDEX = iLeverIndex;

							BOOL bRETVAL = this->LinkCsGateLever(iLeverIndex,iNPC_INDEX);

							if(bRETVAL == FALSE)
							{
								g_Log.AddC(TColor::Red, "[CastleSiege] CCastleSiege::CreateDbNPC() ERROR - Failed to Create Gate Lever : GateIndex:%d",pNpcData.m_iNPC_INDEX);
							}
						}
						else
						{
							it->m_iCS_GATE_LEVER_INDEX = -1;
							g_Log.AddC(TColor::Red, "[CastleSiege] CCastleSiege::CreateDbNPC() ERROR - Failed to Create Gate Lever : GateIndex:%d",pNpcData.m_iNPC_INDEX);
						}
					}
					else
					{
						BOOL bRETVAL = this->LinkCsGateLever(pNpcData.m_iCS_GATE_LEVER_INDEX,iNPC_INDEX);

						if(bRETVAL == FALSE)
						{
							g_Log.AddC(TColor::Red, "[CastleSiege] CCastleSiege::CreateDbNPC() ERROR - Failed to Create Gate Lever : GateIndex:%d",pNpcData.m_iNPC_INDEX);
						}
					}
				}

				it->m_iNPC_OBJINDEX = iNPC_INDEX;
				it->m_iNPC_LIVE = 2;
			}
		}

		it++;
	}
	LeaveCriticalSection(&this->m_critNpcData);
}

void CCastleSiege::CreateNonDbNPC(BOOL bDelFirst)
{
	if( bDelFirst )
	{
		this->ClearNonDbNPC();
	}

	EnterCriticalSection(&this->m_critNpcData);

	std::vector<_CS_NPC_DATA>::iterator it = this->m_vtNpcData.begin();

	while(it != this->m_vtNpcData.end())
	{
		_CS_NPC_DATA & pNpcData = _CS_NPC_DATA(*it);

		if(pNpcData.m_bIN_USE == TRUE && pNpcData.m_bNPC_DBSAVE == FALSE)
		{
			int iNPC_INDEX = gObjAddMonster(MAP_INDEX_CASTLESIEGE);

			if(iNPC_INDEX >= 0)
			{
				gObjSetMonster(iNPC_INDEX, pNpcData.m_iNPC_NUM);
				gObj[iNPC_INDEX].m_PosNum = -1;
				gObj[iNPC_INDEX].X = pNpcData.m_iNPC_SX;
				gObj[iNPC_INDEX].Y = pNpcData.m_iNPC_SY;
				gObj[iNPC_INDEX].MapNumber = MAP_INDEX_CASTLESIEGE;
				gObj[iNPC_INDEX].TX = gObj[iNPC_INDEX].X;
				gObj[iNPC_INDEX].TY = gObj[iNPC_INDEX].Y;
				gObj[iNPC_INDEX].m_OldX = gObj[iNPC_INDEX].X;
				gObj[iNPC_INDEX].m_OldY = gObj[iNPC_INDEX].Y;
				gObj[iNPC_INDEX].Dir = pNpcData.m_iNPC_DIR;
				gObj[iNPC_INDEX].StartX = gObj[iNPC_INDEX].X;
				gObj[iNPC_INDEX].StartY = gObj[iNPC_INDEX].Y;
				gObj[iNPC_INDEX].DieRegen = 0;
				gObj[iNPC_INDEX].MaxRegenTime = 0;
				gObj[iNPC_INDEX].Life = pNpcData.m_iNPC_HP;
				gObj[iNPC_INDEX].MaxLife = pNpcData.m_iNPC_MAXHP;

				switch( pNpcData.m_iNPC_SIDE )
				{
				case 1:
					gObj[iNPC_INDEX].m_btCsNpcType = 2;
					break;
				case 2:
					gObj[iNPC_INDEX].m_btCsNpcType = 3;
					break;
				default:
					gObj[iNPC_INDEX].m_btCsNpcType = 3;
					break;
				}

				it->m_btCsNpcExistVal1 = rand()%255+1; //check
				gObj[iNPC_INDEX].m_btCsNpcExistVal1 = it->m_btCsNpcExistVal1;

				it->m_btCsNpcExistVal2 = rand()%256;
				gObj[iNPC_INDEX].m_btCsNpcExistVal2 = it->m_btCsNpcExistVal2;

				it->m_btCsNpcExistVal3 = rand()%256;
				gObj[iNPC_INDEX].m_btCsNpcExistVal3 = it->m_btCsNpcExistVal3;

				it->m_btCsNpcExistVal4 = rand()%256;
				gObj[iNPC_INDEX].m_btCsNpcExistVal4 = it->m_btCsNpcExistVal4;

				it->m_iNPC_OBJINDEX = iNPC_INDEX;
				it->m_iNPC_LIVE = 2;
			}
		}
		it++;
	}
	LeaveCriticalSection(&this->m_critNpcData);
}

int CCastleSiege::CheckAddDbNPC(int iIndex, int iNpcType, int iNpcIndex, BYTE & btResult)
{
	int iRETVAL = 0;

	EnterCriticalSection(&this->m_critNpcData);

	std::vector<_CS_NPC_DATA>::iterator it = this->m_vtNpcData.begin();

	while( it != this->m_vtNpcData.end() )
	{
		_CS_NPC_DATA & NpcData = _CS_NPC_DATA(*it);

		if( NpcData.m_iNPC_NUM == iNpcType && NpcData.m_iNPC_INDEX == iNpcIndex )
		{
			if( NpcData.m_bIN_USE == TRUE && NpcData.m_iNPC_LIVE > 0 && gObjIsConnected(NpcData.m_iNPC_OBJINDEX) && gObj[NpcData.m_iNPC_OBJINDEX].Class == iNpcType )
			{
				if( gObj[NpcData.m_iNPC_OBJINDEX].Live == 1 && gObj[NpcData.m_iNPC_OBJINDEX].m_iCsNpcExistVal == NpcData.m_iCsNpcExistVal && gObj[NpcData.m_iNPC_OBJINDEX].m_iCsNpcExistVal != 0 )
				{
					btResult = 0;
					iRETVAL = 0;
					break;
				}
			}
			
			int iNpcBuyCost = 0;

			switch( NpcData.m_iNPC_NUM )
			{
			case 277:
				iNpcBuyCost = NpcData.m_iNPC_BASE_MAXHP * 5;
				break;
			case 283:
				iNpcBuyCost = NpcData.m_iNPC_BASE_MAXHP * 3;
				break;
			}

			if( gObj[iIndex].m_PlayerData->Money < iNpcBuyCost )
			{
				btResult = 3;
				iRETVAL = 0;
				break;
			}
			
			GS_GDReqCastleNpcBuy(m_iMapSvrGroup, iIndex, iNpcType, iNpcIndex, NpcData.m_iNPC_DF_LEVEL, NpcData.m_iNPC_RG_LEVEL, NpcData.m_iNPC_MAXHP, NpcData.m_iNPC_HP, NpcData.m_iNPC_SX, NpcData.m_iNPC_SY, NpcData.m_iNPC_DIR, iNpcBuyCost);
			btResult = 1;
			iRETVAL = 1;
			break;
		}
		it++;
	}

	LeaveCriticalSection(&this->m_critNpcData);
		
	return iRETVAL;
}

int CCastleSiege::AddDbNPC(int iNpcType, int iNpcIndex)
{
	int iRETVAL = 0;
	
	EnterCriticalSection(&this->m_critNpcData);

	std::vector<_CS_NPC_DATA>::iterator it = this->m_vtNpcData.begin();

	while( it != this->m_vtNpcData.end() )
	{
		_CS_NPC_DATA & NpcData = _CS_NPC_DATA(*it);

		if( (NpcData.m_iNPC_NUM == iNpcType) && (NpcData.m_iNPC_INDEX == iNpcIndex) )
		{
			if( gObjIsConnected(NpcData.m_iNPC_OBJINDEX) )
			{
				if( gObj[NpcData.m_iNPC_OBJINDEX].Class == iNpcType )
				{
					if( gObj[NpcData.m_iNPC_OBJINDEX].Live == TRUE )
					{
						if( gObj[NpcData.m_iNPC_OBJINDEX].m_iCsNpcExistVal == NpcData.m_iCsNpcExistVal )
						{
							if( gObj[NpcData.m_iNPC_OBJINDEX].m_iCsNpcExistVal )
							{
								g_Log.Add("[CastleSiege] CCastleSiege::AddDbNPC() ERROR - DB NPC EXIST (CLS:%d, IDX:%d(%d), MAXHP:%d, HP:%d, DF:%d, RG:%d)",	NpcData.m_iNPC_NUM, NpcData.m_iNPC_INDEX, NpcData.m_iNPC_OBJINDEX, gObj[NpcData.m_iNPC_OBJINDEX].MaxLife, gObj[NpcData.m_iNPC_OBJINDEX].Life, gObj[NpcData.m_iNPC_OBJINDEX].m_btCsNpcDfLevel, gObj[NpcData.m_iNPC_OBJINDEX].m_btCsNpcRgLevel);
								break;
							}
						}
					}
				}
			}

			int iObjIndex = gObjAddMonster(MAP_INDEX_CASTLESIEGE);

			if( iObjIndex >= 0 )
			{
				::gObjSetMonster(iObjIndex, NpcData.m_iNPC_NUM);
					
				gObj[iObjIndex].m_PosNum = -1;
				gObj[iObjIndex].X = NpcData.m_iNPC_SX;
				gObj[iObjIndex].Y = NpcData.m_iNPC_SY;
				gObj[iObjIndex].MapNumber = MAP_INDEX_CASTLESIEGE;
				gObj[iObjIndex].TX = gObj[iObjIndex].X;
				gObj[iObjIndex].TY = gObj[iObjIndex].Y;
				gObj[iObjIndex].m_OldX = gObj[iObjIndex].X;
				gObj[iObjIndex].m_OldY = gObj[iObjIndex].Y;
				gObj[iObjIndex].Dir = NpcData.m_iNPC_DIR;
				gObj[iObjIndex].StartX = gObj[iObjIndex].X;
				gObj[iObjIndex].StartY = gObj[iObjIndex].Y;
				gObj[iObjIndex].DieRegen = 0;
				gObj[iObjIndex].MaxRegenTime = 0;
					
				gObj[iObjIndex].Life = NpcData.m_iNPC_BASE_HP;
				gObj[iObjIndex].MaxLife = NpcData.m_iNPC_BASE_MAXHP;
				gObj[iObjIndex].m_btCsNpcType = 1;

				int iBaseDFLevel = NpcData.m_iNPC_BASE_DF_LEVEL;

				if( iBaseDFLevel < 0 )	
				{
					iBaseDFLevel = 0;
				}

				if( iBaseDFLevel > 3 )
				{
					iBaseDFLevel = 3;
				}

				int iBaseRGLevel = NpcData.m_iNPC_BASE_RG_LEVEL;
					
				if( iBaseRGLevel < 0 )	
				{
					iBaseRGLevel = 0;
				}

				if( iBaseRGLevel > 3 )	
				{
					iBaseRGLevel = 3;
				}
				

				switch( NpcData.m_iNPC_NUM )
				{
				case 277:
					{
						gObj[iObjIndex].m_Defense = g_iNpcDefense_CGATE[iBaseDFLevel];
						gObj[iObjIndex].m_btCsNpcDfLevel = iBaseDFLevel;
					}
					break;
				case 283:
					{
						gObj[iObjIndex].m_Defense = g_iNpcDefense_CSTATUE[iBaseDFLevel];
						gObj[iObjIndex].m_btCsNpcDfLevel = iBaseDFLevel;
						gObj[iObjIndex].m_btCsNpcRgLevel = iBaseRGLevel;
					}
					break;
				}

				it->m_btCsNpcExistVal1 = ( rand() % 255) + 1;
				gObj[iObjIndex].m_btCsNpcExistVal1 = it->m_btCsNpcExistVal1;

				it->m_btCsNpcExistVal2 = ( rand() % 256);
				gObj[iObjIndex].m_btCsNpcExistVal2 = it->m_btCsNpcExistVal2;

				it->m_btCsNpcExistVal3 = ( rand() % 256);
				gObj[iObjIndex].m_btCsNpcExistVal3 = it->m_btCsNpcExistVal3;

				it->m_btCsNpcExistVal4 = ( rand() % 256);
				gObj[iObjIndex].m_btCsNpcExistVal4 = it->m_btCsNpcExistVal4;

				if( NpcData.m_iNPC_NUM == 277 )
				{
					this->SetGateBlockState(NpcData.m_iNPC_SX, NpcData.m_iNPC_SY, 1);
					gObjAddBuffEffect(&gObj[iObjIndex], BUFFTYPE_CASTLE_DOOR_STATE, 0, 0, 0, 0, -10);

					gObj[iObjIndex].m_btCsGateOpen = 1;
						
					BOOL bCreateLever = FALSE;
						
					if( NpcData.m_iCS_GATE_LEVER_INDEX == -1 )
					{
						bCreateLever = TRUE;
					}
					else if( this->CheckLeverAlive(NpcData.m_iCS_GATE_LEVER_INDEX) == FALSE )
					{
						bCreateLever = TRUE;
					}

					if( bCreateLever != 0 )
					{
						int iLevelerIndex = this->CreateCsGateLever(NpcData.m_iNPC_SX, NpcData.m_iNPC_SY+4);

						if( ObjectMaxRange(iLevelerIndex) )
						{
							it->m_iCS_GATE_LEVER_INDEX = iLevelerIndex;

							int iLinkIndex = this->LinkCsGateLever(iLevelerIndex, iObjIndex);

							if( iLinkIndex == 0 )
							{
								::g_Log.AddC(TColor::Red,  "[CastleSiege] CCastleSiege::CreateDbNPC() ERROR - Failed to Create Gate Lever : GateIndex:%d", NpcData.m_iNPC_INDEX);
							}
						}
						else
						{
							it->m_iCS_GATE_LEVER_INDEX = -1;
							::g_Log.AddC(TColor::Red,  "[CastleSiege] CCastleSiege::CreateDbNPC() ERROR - Failed to Create Gate Lever : GateIndex:%d", NpcData.m_iNPC_INDEX);
						}
					}
					else
					{
						int iLinkIndex = this->LinkCsGateLever(NpcData.m_iCS_GATE_LEVER_INDEX, iObjIndex);

						if( iLinkIndex == 0 )
						{
							::g_Log.AddC(TColor::Red,  "[CastleSiege] CCastleSiege::CreateDbNPC() ERROR - Failed to Create Gate Lever : GateIndex:%d", NpcData.m_iNPC_INDEX);
						}
					}
				}

				it->m_iNPC_OBJINDEX = iObjIndex;
				it->m_iNPC_LIVE = 2;
				it->m_bIN_USE = TRUE;

				iRETVAL = 1;

				if( NpcData.m_iNPC_NUM == 277 || NpcData.m_iNPC_NUM == 283 )
				{
					g_Log.Add("[CastleSiege][CreateDBNPC] (Index:%d, Num:%d, DF:%d, RG:%d, MaxHP:%d, HP:%d )", NpcData.m_iNPC_INDEX, NpcData.m_iNPC_NUM, NpcData.m_iNPC_DF_LEVEL, NpcData.m_iNPC_RG_LEVEL, NpcData.m_iNPC_MAXHP, NpcData.m_iNPC_HP);

				}
				break;
			}
		}

		it++;
	}
	LeaveCriticalSection(&this->m_critNpcData);

	return iRETVAL;
}

BOOL CCastleSiege::DelNPC(int iIndex, int iNpcType, int iMonsterExistVal, int bDbSave)
{
	if( !ObjectMaxRange(iIndex) )
	{
		return FALSE;
	}

	BOOL bEXIST = FALSE;
	BOOL bNPC_DB = FALSE;

	_CS_NPC_DATA pTempNpcData;

	EnterCriticalSection(&this->m_critNpcData);

	std::vector<_CS_NPC_DATA>::iterator it = this->m_vtNpcData.begin();

	while( it != this->m_vtNpcData.end() )
	{
		_CS_NPC_DATA* pNpcData;
		_CS_NPC_DATA NpcData( *it );
		pNpcData = &NpcData;

		if( pNpcData->m_iNPC_NUM == iNpcType && pNpcData->m_iNPC_OBJINDEX == iIndex)
		{
			if( pNpcData->m_iCsNpcExistVal == iMonsterExistVal && pNpcData->m_bIN_USE == TRUE )
			{
				if( pNpcData->m_iNPC_LIVE > 0 )
				{
					it->SetBaseValue();

					it->m_iNPC_OBJINDEX = OBJ_EMPTY;
					it->m_iNPC_LIVE = 0;

					if( pNpcData->m_iNPC_NUM == 277 || pNpcData->m_iNPC_NUM == 283 )
					{
						pNpcData->m_iNPC_HP = pNpcData->m_iNPC_BASE_HP;
						pNpcData->m_iNPC_MAXHP = pNpcData->m_iNPC_BASE_MAXHP;
						pNpcData->m_iNPC_RG_LEVEL = pNpcData->m_iNPC_BASE_RG_LEVEL;
						pNpcData->m_iNPC_DF_LEVEL = pNpcData->m_iNPC_BASE_DF_LEVEL;
					}
					
					bEXIST = TRUE;
					pTempNpcData = *pNpcData;

					if( pNpcData->m_iNPC_NUM == 277 )
					{
						this->SetGateBlockState(pNpcData->m_iNPC_SX, pNpcData->m_iNPC_SY, 1);
						gObjAddBuffEffect(&gObj[pNpcData->m_iNPC_OBJINDEX], BUFFTYPE_CASTLE_DOOR_STATE, 0, 0, 0, 0, -10);
						g_Log.Add("[CastleSiege][ State:%d ] Delete Castle Gate ( CLS:%d, IDX:%d, X:%d. Y:%d )", this->m_iCastleSiegeState, pNpcData->m_iNPC_NUM, pNpcData->m_iNPC_INDEX, pNpcData->m_iNPC_SX, pNpcData->m_iNPC_SY);
					}

					if( pNpcData->m_iNPC_NUM == 283 )
					{
						if( this->m_iCastleSiegeState == CASTLESIEGE_STATE_STARTSIEGE )
						{
							if( this->CheckGuardianStatueExist() == FALSE )
							{
								this->ClearCastleTowerBarrier();
								this->SetCastleTowerAccessable(TRUE);
							}
							g_Log.Add("[CastleSiege] Defend Statue is Broken (CLS:%d, IDX:%d, X:%d, Y:%d)", pNpcData->m_iNPC_NUM, pNpcData->m_iNPC_INDEX, pNpcData->m_iNPC_SX, pNpcData->m_iNPC_SY);
						}
					}
					break;
				}
			}
		}
		it++;
	}
	LeaveCriticalSection(&this->m_critNpcData);

	if( bEXIST == TRUE && bDbSave == TRUE && this->m_iCastleSiegeState != CASTLESIEGE_STATE_STARTSIEGE )
	{
		GS_GDReqNpcRemove(this->m_iMapSvrGroup, pTempNpcData.m_iNPC_NUM, pTempNpcData.m_iNPC_INDEX);
	}

	return TRUE;
}

BOOL CCastleSiege::RepairDbNPC(int iNpcType, int iNpcIndex, int iNpcHP, int iNpcMaxHP)
{
	BOOL bEXIST = FALSE;

	EnterCriticalSection(&this->m_critNpcData);

	std::vector<_CS_NPC_DATA>::iterator it = this->m_vtNpcData.begin();

	while( it != this->m_vtNpcData.end() )
	{
		_CS_NPC_DATA & NpcData = _CS_NPC_DATA(*it);

		if( NpcData.m_iNPC_NUM == iNpcType && NpcData.m_iNPC_INDEX == iNpcIndex && NpcData.m_bIN_USE == 1 && NpcData.m_iNPC_LIVE > 0 )
		{
			it->m_iNPC_HP = iNpcHP;
			it->m_iNPC_MAXHP = iNpcMaxHP;

			int iObjIndex = NpcData.m_iNPC_OBJINDEX;

			if(gObjIsConnected(iObjIndex) != FALSE && gObj[iObjIndex].m_btCsNpcType != 0 && gObj[iObjIndex].Class == iNpcType && gObj[iObjIndex].m_iCsNpcExistVal == NpcData.m_iCsNpcExistVal && gObj[iObjIndex].m_iCsNpcExistVal != 0 )
			{
				gObj[iObjIndex].Life = gObj[iObjIndex].MaxLife;
				bEXIST = TRUE;
			}
			break;
		}
		it++;
	}

	LeaveCriticalSection(&this->m_critNpcData);

	return bEXIST;
}

int CCastleSiege::PayForUpgradeDbNpc(int iIndex, int iNpcType, int iNpcIndex, int iNpcUpType, int iNpcUpValue, int iNpcUpIndex)
{
	int iNEED_GEMOFDEFEND = 0;
	int iNEED_MONEY = 0;

	if( CS_CHECK_UPGRADE(iNpcUpIndex) == FALSE )
	{
		g_Log.AddC(TColor::Red,  "[CastleSiege] CCastleSiege::PayForUpgradeDbNpc() ERROR - lpMsg->iNpcUpIndex is Out of Bound : %d", iNpcUpIndex);
		return FALSE;
	}

	_CS_NPC_DATA pNpcData;

	BOOL bIsLive = this->GetNpcData(iNpcType, iNpcIndex, pNpcData);

	if( bIsLive == FALSE )
	{
		g_Log.AddC(TColor::Red,  "[CastleSiege] CCastleSiege::PayForUpgradeDbNpc() ERROR - bIsLive == FALSE", iNpcUpIndex);
		return FALSE;
	}

	switch( iNpcType )
	{
	case 277:
		{
			switch( iNpcUpType )
			{
			case 1:
				{
					iNEED_GEMOFDEFEND = g_iNpcUpDfLevel_CGATE[iNpcUpIndex][1];
					iNEED_MONEY = g_iNpcUpDfLevel_CGATE[iNpcUpIndex][2];
					
					if( gObj[iIndex].m_PlayerData->Money < iNEED_MONEY )
					{
						GSProtocol.GCAnsNpcUpgrade(iIndex, 3, iNpcType, iNpcIndex, iNpcUpType, iNpcUpValue);
						return FALSE;
					}

					int iDelResult = DelGemOfDefend(iIndex, iNEED_GEMOFDEFEND);

					if( iDelResult == FALSE )
					{
						GSProtocol.GCAnsNpcUpgrade(iIndex, 4, iNpcType, iNpcIndex, iNpcUpType, iNpcUpValue);
						return FALSE;	
					}

					gObj[iIndex].m_PlayerData->Money -= iNEED_MONEY;
				}
				break;
			case 3:
				{
					iNEED_GEMOFDEFEND = g_iNpcUpMaxHP_CGATE[iNpcUpIndex][1];
					iNEED_MONEY = g_iNpcUpMaxHP_CGATE[iNpcUpIndex][2];

					if( gObj[iIndex].m_PlayerData->Money < iNEED_MONEY )
					{
						GSProtocol.GCAnsNpcUpgrade(iIndex, 3, iNpcType, iNpcIndex, iNpcUpType, iNpcUpValue);
						return FALSE;
					}

					int iDelResult = DelGemOfDefend(iIndex, iNEED_GEMOFDEFEND);

					if( iDelResult == FALSE )
					{
						GSProtocol.GCAnsNpcUpgrade(iIndex, 4, iNpcType, iNpcIndex, iNpcUpType, iNpcUpValue);
						return FALSE;	
					}

					gObj[iIndex].m_PlayerData->Money -= iNEED_MONEY;
				}
				break;
			default:
				{
					GSProtocol.GCAnsNpcUpgrade(iIndex, 5, iNpcType, iNpcIndex, iNpcUpType, iNpcUpValue);
					return FALSE;	
				}
				break;
			}
		}
		break;
	case 283:
		{
			switch( iNpcUpType )
			{
			case 1:
				{
					iNEED_GEMOFDEFEND = g_iNpcUpDfLevel_CSTATUE[iNpcUpIndex][1];
					iNEED_MONEY = g_iNpcUpDfLevel_CSTATUE[iNpcUpIndex][2];
					
					if( gObj[iIndex].m_PlayerData->Money < iNEED_MONEY )
					{
						GSProtocol.GCAnsNpcUpgrade(iIndex, 3, iNpcType, iNpcIndex, iNpcUpType, iNpcUpValue);
						return FALSE;
					}

					int iDelResult = DelGemOfDefend(iIndex, iNEED_GEMOFDEFEND);

					if( iDelResult == FALSE )
					{
						GSProtocol.GCAnsNpcUpgrade(iIndex, 4, iNpcType, iNpcIndex, iNpcUpType, iNpcUpValue);
						return FALSE;	
					}

					gObj[iIndex].m_PlayerData->Money -= iNEED_MONEY;
				}
				break;
			case 2:
				{
					iNEED_GEMOFDEFEND = g_iNpcUpRgLevel_CSTATUE[iNpcUpIndex][1];
					iNEED_MONEY = g_iNpcUpRgLevel_CSTATUE[iNpcUpIndex][2];

					if( gObj[iIndex].m_PlayerData->Money < iNEED_MONEY )
					{
						GSProtocol.GCAnsNpcUpgrade(iIndex, 3, iNpcType, iNpcIndex, iNpcUpType, iNpcUpValue);
						return FALSE;
					}

					int iDelResult = DelGemOfDefend(iIndex, iNEED_GEMOFDEFEND);

					if( iDelResult == FALSE )
					{
						GSProtocol.GCAnsNpcUpgrade(iIndex, 4, iNpcType, iNpcIndex, iNpcUpType, iNpcUpValue);
						return FALSE;	
					}

					gObj[iIndex].m_PlayerData->Money -= iNEED_MONEY;
				}
				break;
			case 3:
				{
					iNEED_GEMOFDEFEND = g_iNpcUpMaxHP_CSTATUE[iNpcUpIndex][1];
					iNEED_MONEY = g_iNpcUpMaxHP_CSTATUE[iNpcUpIndex][2];

					if( gObj[iIndex].m_PlayerData->Money < iNEED_MONEY )
					{
						GSProtocol.GCAnsNpcUpgrade(iIndex, 3, iNpcType, iNpcIndex, iNpcUpType, iNpcUpValue);
						return FALSE;
					}

					int iDelResult = DelGemOfDefend(iIndex, iNEED_GEMOFDEFEND);

					if( iDelResult == FALSE )
					{
						GSProtocol.GCAnsNpcUpgrade(iIndex, 4, iNpcType, iNpcIndex, iNpcUpType, iNpcUpValue);
						return FALSE;	
					}

					gObj[iIndex].m_PlayerData->Money -= iNEED_MONEY;
				}
				break;
			default:
				{
					GSProtocol.GCAnsNpcUpgrade(iIndex, 5, iNpcType, iNpcIndex, iNpcUpType, iNpcUpValue);
					return FALSE;	
				}
				break;
			}
		}
		break;
	}

	GSProtocol.GCMoneySend(iIndex, gObj[iIndex].m_PlayerData->Money);

	return TRUE;
}

BOOL CCastleSiege::UpgradeDbNPC(int iIndex, int iNpcType, int iNpcIndex, int iNpcUpType, int iNpcUpValue, int iNpcUpIndex)
{
	if( CS_CHECK_UPGRADE(iNpcUpIndex) == FALSE )
	{
		g_Log.AddC(TColor::Red,  "[CastleSiege] CCastleSiege::UpgradeDbNPC() ERROR - lpMsg->iNpcUpIndex is Out of Bound : %d", iNpcUpIndex);
		return FALSE;
	}

	_CS_NPC_DATA pNpcData;
	BOOL bIsLive;

	bIsLive = GetNpcData(iNpcType, iNpcIndex, pNpcData);

	if( bIsLive == FALSE )
	{
		g_Log.AddC(TColor::Red,  "[CastleSiege] CCastleSiege::UpgradeDbNPC() ERROR - bIsLive == FALSE", iNpcUpIndex);
		return FALSE;
	}

	switch( iNpcType )
	{
		case 277:
			{
				switch( iNpcUpType )
				{
					case 1:
						{
							this->UpgradeDbNPC_DFLEVEL(iNpcType, iNpcIndex, iNpcUpValue );	
						}
						break;

					case 3:
						{
							this->UpgradeDbNPC_MAXHP(iNpcType, iNpcIndex, iNpcUpValue );
						}
						break;

					default:
						{
							GSProtocol.GCAnsNpcUpgrade(iIndex, 5, iNpcType, iNpcIndex, iNpcUpType, iNpcUpValue);
							return FALSE;
						}
						break;
				}
			}
			break;

		case 283:
			{
				switch( iNpcUpType )
				{
					case 1:
						{
							this->UpgradeDbNPC_DFLEVEL(iNpcType, iNpcIndex, iNpcUpValue );
						}
						break;

					case 2:
						{
							this->UpgradeDbNPC_RGLEVEL(iNpcType, iNpcIndex, iNpcUpValue );
						}
						break;

					case 3:
						{
							this->UpgradeDbNPC_MAXHP(iNpcType, iNpcIndex, iNpcUpValue );
						}
						break;

					default:
						{
							GSProtocol.GCAnsNpcUpgrade(iIndex, 5, iNpcType, iNpcIndex, iNpcUpType, iNpcUpValue);
							return FALSE;
						}
						break;
				}
			}
			break;
	}

	return TRUE;
}

int CCastleSiege::UpgradeDbNPC_DFLEVEL(int iNpcType, int iNpcIndex, int iNpcDfLevel)
{
	BOOL bEXIST = FALSE;

	EnterCriticalSection(&this->m_critNpcData);

	std::vector<_CS_NPC_DATA>::iterator it = this->m_vtNpcData.begin();
	
	while( it != this->m_vtNpcData.end() )
	{
		_CS_NPC_DATA & NpcData = _CS_NPC_DATA(*it);

		if( NpcData.m_iNPC_NUM == iNpcType && NpcData.m_iNPC_INDEX == iNpcIndex && NpcData.m_bIN_USE == 1 && NpcData.m_iNPC_LIVE > 0 )
		{
			int iObjIndex = NpcData.m_iNPC_OBJINDEX;

			if( gObjIsConnected(iObjIndex) && gObj[iObjIndex].m_btCsNpcType != 0 &&	gObj[iObjIndex].m_iCsNpcExistVal == NpcData.m_iCsNpcExistVal && gObj[iObjIndex].m_iCsNpcExistVal != 0 )
			{
				it->m_iNPC_DF_LEVEL = iNpcDfLevel;
				
				bEXIST = TRUE;
	
				gObj[iObjIndex].m_Defense = g_iNpcDefense_CGATE[iNpcDfLevel];
				gObj[iObjIndex].m_MagicDefense = g_iNpcDefense_CGATE[iNpcDfLevel];
				gObj[iObjIndex].m_btCsNpcDfLevel = iNpcDfLevel;
			}
			break;
		}
		it++;
	}

	LeaveCriticalSection(&this->m_critNpcData);
	
	return bEXIST;
}

int CCastleSiege::UpgradeDbNPC_RGLEVEL(int iNpcType, int iNpcIndex, int iNpcRgLevel)
{
	BOOL bEXIST = FALSE;

	EnterCriticalSection(&this->m_critNpcData);

	std::vector<_CS_NPC_DATA>::iterator it = this->m_vtNpcData.begin();
	
	while( it != this->m_vtNpcData.end() )
	{
		_CS_NPC_DATA & NpcData = _CS_NPC_DATA(*it);

		if( NpcData.m_iNPC_NUM == iNpcType && NpcData.m_iNPC_INDEX == iNpcIndex && NpcData.m_bIN_USE == 1 && NpcData.m_iNPC_LIVE > 0 )
		{
			int iObjIndex = NpcData.m_iNPC_OBJINDEX;

			if( gObjIsConnected(iObjIndex) && gObj[iObjIndex].m_btCsNpcType != 0 &&	gObj[iObjIndex].m_iCsNpcExistVal == NpcData.m_iCsNpcExistVal && gObj[iObjIndex].m_iCsNpcExistVal != 0 )
			{
				it->m_iNPC_RG_LEVEL = iNpcRgLevel;
				bEXIST = TRUE;
				gObj[iObjIndex].m_btCsNpcRgLevel = iNpcRgLevel;
			}
			break;
		}
		it++;
	}

	LeaveCriticalSection(&this->m_critNpcData);
	
	return bEXIST;
}

int CCastleSiege::UpgradeDbNPC_MAXHP(int iNpcType, int iNpcIndex, int iNpcMaxHP)
{
	BOOL bEXIST = FALSE;

	EnterCriticalSection(&this->m_critNpcData);

	std::vector<_CS_NPC_DATA>::iterator it = this->m_vtNpcData.begin();
	
	while( it != this->m_vtNpcData.end() )
	{
		_CS_NPC_DATA & NpcData = _CS_NPC_DATA(*it);

		if( NpcData.m_iNPC_NUM == iNpcType && NpcData.m_iNPC_INDEX == iNpcIndex && NpcData.m_bIN_USE == 1 && NpcData.m_iNPC_LIVE > 0 )
		{
			int iObjIndex = NpcData.m_iNPC_OBJINDEX;

			if( gObjIsConnected(iObjIndex) && gObj[iObjIndex].m_btCsNpcType != 0 &&	gObj[iObjIndex].m_iCsNpcExistVal == NpcData.m_iCsNpcExistVal && gObj[iObjIndex].m_iCsNpcExistVal != 0 )
			{
				it->m_iNPC_MAXHP = iNpcMaxHP;
				it->m_iNPC_HP = iNpcMaxHP;
				bEXIST = TRUE;
				gObj[iObjIndex].Life = iNpcMaxHP;
				gObj[iObjIndex].MaxLife = iNpcMaxHP;
			}
			break;
		}
		it++;
	}

	LeaveCriticalSection(&this->m_critNpcData);
	
	return bEXIST;
}

void CCastleSiege::StoreDbNpc()
{
	g_Log.Add("[CastleSiege] CCastleSiege::StoreDbNpc() - << START >>");

	char cBUFFER[0x1090];

	CSP_REQ_NPCUPDATEDATA * lpMsg = (CSP_REQ_NPCUPDATEDATA *)cBUFFER;
	CSP_NPCUPDATEDATA * lpMsgBody = (CSP_NPCUPDATEDATA *)&cBUFFER[sizeof(CSP_REQ_NPCUPDATEDATA)];

	int iCOUNT = 0;

	EnterCriticalSection(&this->m_critNpcData);

	std::vector<_CS_NPC_DATA>::iterator it = this->m_vtNpcData.begin();
	BOOL bExist = FALSE;

	while(it != this->m_vtNpcData.end())
	{
		if(iCOUNT >= 150)
		{
			break;
		}

		_CS_NPC_DATA & pNpcData = _CS_NPC_DATA(*it);

		if(pNpcData.m_bNPC_DBSAVE == TRUE  && gObjIsConnected(pNpcData.m_iNPC_OBJINDEX) && gObj[pNpcData.m_iNPC_OBJINDEX].Class == pNpcData.m_iNPC_NUM && gObj[pNpcData.m_iNPC_OBJINDEX].m_iCsNpcExistVal == pNpcData.m_iCsNpcExistVal && gObj[pNpcData.m_iNPC_OBJINDEX].m_iCsNpcExistVal != NULL && gObj[pNpcData.m_iNPC_OBJINDEX].Live == TRUE && gObj[pNpcData.m_iNPC_OBJINDEX].Life > 0)
		{
			lpMsgBody[iCOUNT].iNpcNumber = pNpcData.m_iNPC_NUM;
			lpMsgBody[iCOUNT].iNpcIndex = pNpcData.m_iNPC_INDEX;
			lpMsgBody[iCOUNT].iNpcDfLevel = pNpcData.m_iNPC_DF_LEVEL;
			lpMsgBody[iCOUNT].iNpcRgLevel = pNpcData.m_iNPC_RG_LEVEL;
			lpMsgBody[iCOUNT].iNpcMaxHp = gObj[pNpcData.m_iNPC_OBJINDEX].MaxLife;
			lpMsgBody[iCOUNT].iNpcHp = gObj[pNpcData.m_iNPC_OBJINDEX].Life;
			lpMsgBody[iCOUNT].btNpcX = pNpcData.m_iNPC_SX;
			lpMsgBody[iCOUNT].btNpcY = pNpcData.m_iNPC_SY;
			lpMsgBody[iCOUNT].btNpcDIR = pNpcData.m_iNPC_DIR;

			g_Log.Add("[CastleSiege] CCastleSiege::StoreDbNpc() NPC INFO (CLS:%d, IDX:%d(%d), MAXHP:%d, HP:%d, DF:%d, RG:%d)", pNpcData.m_iNPC_NUM,pNpcData.m_iNPC_INDEX,pNpcData.m_iNPC_OBJINDEX,lpMsgBody[iCOUNT].iNpcMaxHp,lpMsgBody[iCOUNT].iNpcHp,lpMsgBody[iCOUNT].iNpcDfLevel,lpMsgBody[iCOUNT].iNpcRgLevel);
			iCOUNT++;
		}
		it++;
	}

	LeaveCriticalSection(&this->m_critNpcData);

	if(iCOUNT < 0)
	{
		iCOUNT = 0;
	}

	lpMsg->wMapSvrNum = this->m_iMapSvrGroup;
	lpMsg->iCount = iCOUNT;
	lpMsg->h.set((LPBYTE)&cBUFFER,0x89,iCOUNT * sizeof(CSP_NPCUPDATEDATA) + sizeof(CSP_REQ_NPCUPDATEDATA));
	wsDataCli.DataSend((char*)cBUFFER,iCOUNT * sizeof(CSP_NPCUPDATEDATA) + sizeof(CSP_REQ_NPCUPDATEDATA));

	g_Log.Add("[CastleSiege] CCastleSiege::StoreDbNpc() - << END >>");
}

void CCastleSiege::SendNpcStateList(int iIndex, int iNpcType)
{
	if( gObjIsConnected(iIndex) == FALSE )
	{
		return;
	}

	char cNPC_LIST[(sizeof(PMSG_NPCDBLIST) * 50)+sizeof(PMSG_ANS_NPCDBLIST)] = {0};

	int iNPC_COUNT = 0;

	PMSG_ANS_NPCDBLIST* lpMsgSend = (PMSG_ANS_NPCDBLIST*)&cNPC_LIST[0];
	PMSG_NPCDBLIST* lpMsgSendBody = (PMSG_NPCDBLIST*)&cNPC_LIST[sizeof(PMSG_ANS_NPCDBLIST)];

	EnterCriticalSection(&this->m_critNpcData);
	
	std::vector<_CS_NPC_DATA>::iterator it = this->m_vtNpcData.begin();

	while( it != this->m_vtNpcData.end() )
	{
		if( iNPC_COUNT >= 50 )
		{
			break;
		}
	
		_CS_NPC_DATA & NpcData = _CS_NPC_DATA(*it);

		if( NpcData.m_iNPC_NUM == iNpcType && NpcData.m_bIN_USE == TRUE )
		{
			int iLife = 0;
			int iMaxLife = 0;
			BYTE btNpcLive = 0;
			int iObjIndex = NpcData.m_iNPC_OBJINDEX;

			if( gObjIsConnected(iObjIndex) )
			{
				if( gObj[iObjIndex].Class == iNpcType && gObj[iObjIndex].Live == 1 && gObj[iObjIndex].Life > 0 && gObj[iObjIndex].m_btCsNpcType != 0 && gObj[iObjIndex].m_iCsNpcExistVal == NpcData.m_iCsNpcExistVal && gObj[iObjIndex].m_iCsNpcExistVal != 0 ) //HermeX Fix
				{
					iLife = gObj[iObjIndex].Life;
					iMaxLife = gObj[iObjIndex].MaxLife;
					btNpcLive = 1;
				}
				else
				{
					iLife = 0;
					iMaxLife = NpcData.m_iNPC_BASE_MAXHP;
					btNpcLive = 0;
				}
			}
			else
			{
				iLife = 0;
				iMaxLife = NpcData.m_iNPC_BASE_MAXHP;
				btNpcLive = 0;
			}

			lpMsgSendBody[iNPC_COUNT].iNpcNumber = NpcData.m_iNPC_NUM;
			lpMsgSendBody[iNPC_COUNT].iNpcIndex = NpcData.m_iNPC_INDEX;
			lpMsgSendBody[iNPC_COUNT].iNpcDfLevel = NpcData.m_iNPC_DF_LEVEL;
			lpMsgSendBody[iNPC_COUNT].iNpcRgLevel = NpcData.m_iNPC_RG_LEVEL;
			lpMsgSendBody[iNPC_COUNT].iNpcHp = iLife;
			lpMsgSendBody[iNPC_COUNT].iNpcMaxHp = iMaxLife;
			lpMsgSendBody[iNPC_COUNT].btNpcX = NpcData.m_iNPC_SX;
			lpMsgSendBody[iNPC_COUNT].btNpcY = NpcData.m_iNPC_SY;
			lpMsgSendBody[iNPC_COUNT].btNpcLive = btNpcLive;

			iNPC_COUNT++;
		}

		it++;
	}
	LeaveCriticalSection(&this->m_critNpcData);

	lpMsgSend->btResult = 1;
	lpMsgSend->h.set((LPBYTE)&cNPC_LIST, 0xB3, (sizeof(PMSG_NPCDBLIST) * iNPC_COUNT)+sizeof(PMSG_ANS_NPCDBLIST));
	lpMsgSend->iCount = iNPC_COUNT;

	IOCP.DataSend(iIndex, (LPBYTE)&cNPC_LIST, (sizeof(PMSG_NPCDBLIST) * iNPC_COUNT)+sizeof(PMSG_ANS_NPCDBLIST));
}

int CCastleSiege::GetNpcData(int iNpcType, int iNpcIndex, _CS_NPC_DATA & pRetNpcData)
{
	pRetNpcData.Clear();

	BOOL bEXIST = FALSE;
	
	EnterCriticalSection(&this->m_critNpcData);

	std::vector<_CS_NPC_DATA>::iterator it = this->m_vtNpcData.begin();

	while( it != this->m_vtNpcData.end() )
	{
		_CS_NPC_DATA & NpcData = _CS_NPC_DATA(*it);

		if( NpcData.m_iNPC_NUM == iNpcType && NpcData.m_iNPC_INDEX == iNpcIndex && NpcData.m_bIN_USE == 1 && NpcData.m_iNPC_LIVE > 0 )
		{
			int iObjIndex = NpcData.m_iNPC_OBJINDEX;

			if( gObjIsConnected(iObjIndex) && gObj[iObjIndex].m_btCsNpcType != 0 && gObj[iObjIndex].m_iCsNpcExistVal == NpcData.m_iCsNpcExistVal && gObj[iObjIndex].m_iCsNpcExistVal != 0 )
			{
				pRetNpcData = NpcData;
				bEXIST = TRUE;
			}
			break;
		}
		it++;
	}

	LeaveCriticalSection(&this->m_critNpcData);
	return bEXIST;
}

void CCastleSiege::AdjustDbNpcLevel()
{
	EnterCriticalSection(&this->m_critNpcData);

	std::vector<_CS_NPC_DATA>::iterator it = this->m_vtNpcData.begin();

	while(it != this->m_vtNpcData.end())
	{
		_CS_NPC_DATA & pNpcData = _CS_NPC_DATA(*it);

		if(pNpcData.m_bIN_USE == 1 && pNpcData.m_iNPC_LIVE > 0)
		{
			int iNPC_INDEX = pNpcData.m_iNPC_OBJINDEX;

			if(gObjIsConnected(iNPC_INDEX) && gObj[iNPC_INDEX].Class == pNpcData.m_iNPC_NUM	&& gObj[iNPC_INDEX].m_btCsNpcType != FALSE && gObj[iNPC_INDEX].m_iCsNpcExistVal == pNpcData.m_iCsNpcExistVal && gObj[iNPC_INDEX].m_iCsNpcExistVal != NULL)
			{
				switch(gObj[iNPC_INDEX].Class)
				{
				case 277:
					{
						int iDF_LEVEL = pNpcData.m_iNPC_DF_LEVEL;

						if(iDF_LEVEL < 0)
						{
							iDF_LEVEL = 0;
						}

						if(iDF_LEVEL > CS_MAX_UPGRADE)
						{
							iDF_LEVEL = CS_MAX_UPGRADE;
						}

						gObj[iNPC_INDEX].m_btCsNpcDfLevel = iDF_LEVEL;
						gObj[iNPC_INDEX].m_Defense = g_iNpcDefense_CGATE[iDF_LEVEL];
					}
					break;
				case 283:
					{
						int iDF_LEVEL = pNpcData.m_iNPC_DF_LEVEL;

						if(iDF_LEVEL < 0)
						{
							iDF_LEVEL = 0;
						}

						if(iDF_LEVEL > CS_MAX_UPGRADE)
						{
							iDF_LEVEL = CS_MAX_UPGRADE;
						}

						gObj[iNPC_INDEX].m_btCsNpcDfLevel = iDF_LEVEL;
						gObj[iNPC_INDEX].m_Defense = g_iNpcDefense_CSTATUE[iDF_LEVEL];
						gObj[iNPC_INDEX].m_btCsNpcRgLevel = pNpcData.m_iNPC_RG_LEVEL;
					}
					break;
				}
			}

			break;
		}
		it++;
	}

	LeaveCriticalSection(&m_critNpcData);
}

void CCastleSiege::SendCsGateStateViewPort(int iGateIndex, BYTE btOperate)
{
	if( gObjIsConnected(iGateIndex) == FALSE )
	{
		return;
	}

	LPOBJ lpObj = &gObj[iGateIndex];

	if( lpObj->VPCount2 < 1 )
	{
		return;
	}

	int tObjNum = -1;

	for( int i = 0; i < MAX_VIEWPORT; i++ )
	{
		tObjNum = lpObj->VpPlayer2[i].number;

		if( tObjNum >= 0 )
		{
			if( gObj[tObjNum].Type == OBJ_USER && gObjIsConnected(tObjNum) )
			{
				GSProtocol.GCAnsCsGateCurState(tObjNum, iGateIndex, btOperate);
			}
		}
	}
}

void CCastleSiege::CreateAllCsGateLever()
{
	EnterCriticalSection(&this->m_critNpcData);

	if(!this->m_vtNpcData.empty())
	{
		for(std::vector<_CS_NPC_DATA>::iterator it = this->m_vtNpcData.begin(); it != this->m_vtNpcData.end(); it++)
		{
			_CS_NPC_DATA & pNpcData = _CS_NPC_DATA(*it);

			if(pNpcData.m_iNPC_NUM == 277)
			{
				if(pNpcData.m_iCS_GATE_LEVER_INDEX != -1 && CheckLeverAlive(pNpcData.m_iCS_GATE_LEVER_INDEX) == 1)
				{
					continue;
				}

				int iLeverIndex = this->CreateCsGateLever(pNpcData.m_iNPC_SX,pNpcData.m_iNPC_SY + 4);

				if(ObjectMaxRange(iLeverIndex))
				{
					it->m_iCS_GATE_LEVER_INDEX = iLeverIndex;
				}
				else
				{
					it->m_iCS_GATE_LEVER_INDEX = -1;
					g_Log.AddC(TColor::Red, "[CastleSiege] CCastleSiege::CreateAllCsGateLever() ERROR - Failed to Create Gate Lever : GateIndex:%d",pNpcData.m_iNPC_INDEX);
				}
			}
		}
	}

	LeaveCriticalSection(&this->m_critNpcData);
}

int CCastleSiege::CreateCsGateLever(int iX, int iY)
{
	if(XY_MACRO(iX) == FALSE || XY_MACRO(iY) == FALSE)
	{
		return -1;
	}

	int iNPC_INDEX = gObjAddMonster(MAP_INDEX_CASTLESIEGE);

	if(iNPC_INDEX >= 0)
	{
		gObjSetMonster(iNPC_INDEX, 219);

		gObj[iNPC_INDEX].m_PosNum = -1;
		gObj[iNPC_INDEX].X = iX;
		gObj[iNPC_INDEX].Y = iY;
		gObj[iNPC_INDEX].MapNumber = MAP_INDEX_CASTLESIEGE;
		gObj[iNPC_INDEX].TX = gObj[iNPC_INDEX].X;
		gObj[iNPC_INDEX].TY = gObj[iNPC_INDEX].Y;
		gObj[iNPC_INDEX].m_OldX = gObj[iNPC_INDEX].X;
		gObj[iNPC_INDEX].m_OldY = gObj[iNPC_INDEX].Y;
		gObj[iNPC_INDEX].Dir = 1;
		gObj[iNPC_INDEX].StartX = gObj[iNPC_INDEX].X;
		gObj[iNPC_INDEX].StartY = gObj[iNPC_INDEX].Y;
		gObj[iNPC_INDEX].DieRegen = 0;
		gObj[iNPC_INDEX].MaxRegenTime = 0;
		gObj[iNPC_INDEX].Life = 10000.0f;
		gObj[iNPC_INDEX].MaxLife = 10000.0f;
		gObj[iNPC_INDEX].m_btCsNpcType = 2;
		gObj[iNPC_INDEX].m_iCsGateLeverLinkIndex = -1;
	
		return iNPC_INDEX;
	}	

	return -1;
}

int CCastleSiege::LinkCsGateLever(int iLeverIndex,int iGateIndex)
{
	if(!gObjIsConnected(iLeverIndex) || gObj[iLeverIndex].Class != 219)
	{
		return false;
	}

	if(!gObjIsConnected(iGateIndex) || gObj[iGateIndex].Class != 277)
	{
		return false;
	}

	gObj[iLeverIndex].m_iCsGateLeverLinkIndex = iGateIndex;
	gObj[iGateIndex].m_iCsGateLeverLinkIndex = iLeverIndex;
	return true;
}

int CCastleSiege::CheckLeverAlive(int iLeverIndex)
{
	if(gObjIsConnected(iLeverIndex) == FALSE || gObj[iLeverIndex].Class != 219 )
	{
		return false;
	}

	return true;
}

void CCastleSiege::CheckCsDbNpcAlive()
{
	if( m_iCastleSiegeState != CASTLESIEGE_STATE_STARTSIEGE )
	{
		return;
	}

	EnterCriticalSection(&this->m_critNpcData);

	if( this->m_vtNpcData.empty() == false )
	{
		for( std::vector<_CS_NPC_DATA>::iterator it = this->m_vtNpcData.begin(); it != this->m_vtNpcData.end(); it++ )
		{
			_CS_NPC_DATA & NpcData = _CS_NPC_DATA(*it);

			if( (NpcData.m_iNPC_NUM == 277 || NpcData.m_iNPC_NUM == 283) && NpcData.m_iNPC_OBJINDEX != OBJ_EMPTY )
			{
				if( !gObjIsConnected(NpcData.m_iNPC_OBJINDEX) || gObj[NpcData.m_iNPC_OBJINDEX].m_iCsNpcExistVal != NpcData.m_iCsNpcExistVal || gObj[NpcData.m_iNPC_OBJINDEX].m_iCsNpcExistVal == 0 || (gObj[NpcData.m_iNPC_OBJINDEX].Class != 277 && gObj[NpcData.m_iNPC_OBJINDEX].Class != 283) || gObj[NpcData.m_iNPC_OBJINDEX].Live == FALSE) //HermeX Fix
				{
					NpcData.m_iNPC_HP = NpcData.m_iNPC_BASE_HP;
					NpcData.m_iNPC_MAXHP = NpcData.m_iNPC_BASE_MAXHP;
					NpcData.m_iNPC_RG_LEVEL = NpcData.m_iNPC_BASE_RG_LEVEL;
					NpcData.m_iNPC_DF_LEVEL = NpcData.m_iNPC_BASE_DF_LEVEL;

					switch( NpcData.m_iNPC_NUM )
					{
						case 277:
							this->SetGateBlockState(NpcData.m_iNPC_SX, NpcData.m_iNPC_SY, 1);
							gObjAddBuffEffect(&gObj[NpcData.m_iNPC_OBJINDEX], BUFFTYPE_CASTLE_DOOR_STATE, 0, 0, 0, 0, -10);
							g_Log.Add("[CastleSiege] Castle Gate is Broken (CLS:%d, IDX:%d, X:%d, Y:%d)", NpcData.m_iNPC_NUM, NpcData.m_iNPC_INDEX, NpcData.m_iNPC_SX, NpcData.m_iNPC_SY);
							break;
						case 283:
							if( this->CheckGuardianStatueExist() == FALSE )
							{
								this->ClearCastleTowerBarrier();
								this->SetCastleTowerAccessable(TRUE);
							}
							g_Log.Add("[CastleSiege] Defend Statue is Broken (CLS:%d, IDX:%d, X:%d, Y:%d)", NpcData.m_iNPC_NUM, NpcData.m_iNPC_INDEX, NpcData.m_iNPC_SX, NpcData.m_iNPC_SY);
							break;
					}
					it->m_iNPC_OBJINDEX = OBJ_EMPTY;
				}
			}
		}
	}
	LeaveCriticalSection(&this->m_critNpcData);
}

BOOL CCastleSiege::CheckCsGateAlive(int iGateIndex)
{
	if( !gObjIsConnected(iGateIndex) || gObj[iGateIndex].Class != 277 ) 
	{
		return FALSE;
	}

	return TRUE;
}

int CCastleSiege::DelGemOfDefend(int iIndex, int iNeedGemOfDefend)
{
	if( gObjIsConnected(iIndex) == FALSE )
	{
		return FALSE;
	}

	if( iNeedGemOfDefend <= 0 )
	{
		return FALSE;
	}

	int iGemOfDefendCount = 0;

	for( int x = 0; x < INVENTORY_SIZE; x++ )
	{
		if( gObj[iIndex].pInventory[x].IsItem() == TRUE )
		{
			if( gObj[iIndex].pInventory[x].m_Type == ITEMGET(14,31) )
			{
				iGemOfDefendCount++;
			}
		}
	}

	if( iGemOfDefendCount < iNeedGemOfDefend )
	{
		return FALSE;
	}

	int iGemOfDefendDeletCount = 0;

	for( int x = 0; x < INVENTORY_SIZE; x++ )
	{
		if( gObj[iIndex].pInventory[x].IsItem() == TRUE )
		{
			if( gObj[iIndex].pInventory[x].m_Type == ITEMGET(14,31) )
			{
				g_Log.Add("[CastleSiege] [%s][%s] Deleting Gem of Defend (GUILD:%s) - Serial:%u", gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].m_PlayerData->GuildName, gObj[iIndex].pInventory[x].m_Number);

				gObjInventoryItemSet(iIndex, x, 0xff);
				gObjInventoryDeleteItem(iIndex, x);
				GSProtocol.GCInventoryItemDeleteSend(iIndex, x, 1);

				iGemOfDefendDeletCount++;

				if( iGemOfDefendDeletCount >= iNeedGemOfDefend )
				{
					break;
				}
			}
		}
	}

	return TRUE;
}

void CCastleSiege::SetTaxRate(int iTaxType, int iTaxRate)
{
	int iMaxTaxRate = 0;
	
	switch( iTaxType )
	{
	case 1:
		iMaxTaxRate = 3;
		break;
	case 2:
		iMaxTaxRate = 3;
		break;
	case 3:
		iMaxTaxRate = 300000;
		break;
	}

	if( iTaxRate < 0 || iTaxRate > iMaxTaxRate ) //season4.5 fix
	{
		return;
	}
	
	switch( iTaxType )
	{
	case 1:
		this->m_iTaxRateChaos = iTaxRate;
		break;
	case 2:
		this->m_iTaxRateStore = iTaxRate;
		break;
	case 3:
		this->m_iTaxHuntZone = iTaxRate;
		break;
	}
	
	g_Log.Add("[CastleSiege] Tax Rate Setted - Type:%d, Rate:%d", iTaxType, iTaxRate);
}

BOOL CCastleSiege::CheckCastleHasMoney(__int64 i64CastleMoney)
{
	if( this->m_i64CastleMoney >= i64CastleMoney )
	{
		return TRUE;
	}

	return FALSE;
}

void CCastleSiege::SetCastleMoney(__int64 i64CastleMoney)
{
	this->m_i64CastleMoney = i64CastleMoney;
}

void CCastleSiege::ResetCastleTaxInfo()
{
	this->m_i64CastleMoney = 0;
	this->m_iTaxRateChaos = 0;
	this->m_iTaxRateStore = 0;
	this->m_iTaxHuntZone = 0;
}

void CCastleSiege::SendCastleStateSync()
{
	GS_GDReqCastleStateSync(this->m_iMapSvrGroup, this->m_iCastleSiegeState, this->m_iTaxRateChaos, this->m_iTaxRateStore, this->m_iTaxHuntZone, this->m_szCastleOwnerGuild);
}

BOOL CCastleSiege::CheckGuildOwnCastle(char* lpszGuildName)
{
	if( lpszGuildName == NULL )
	{
		return FALSE;
	}

	if( strcmp(this->m_szCastleOwnerGuild, "") == 0 )
	{
		return FALSE;
	}

	if( strcmp(this->m_szCastleOwnerGuild, lpszGuildName) == 0 )
	{
		return TRUE;
	}

	return FALSE;
}

void CCastleSiege::ResetCastleCycle()
{
	this->m_bCsBasicGuildInfoLoadOK = FALSE;
	this->m_bCsTotalGuildInfoLoadOK = FALSE;

	EnterCriticalSection(&this->m_critCsBasicGuildInfo);
	this->m_mapCsBasicGuildInfo.clear();
	LeaveCriticalSection(&this->m_critCsBasicGuildInfo);

	EnterCriticalSection(&this->m_critCsTotalGuildInfo);
	this->m_mapCsTotalGuildInfo.clear();
	LeaveCriticalSection(&this->m_critCsTotalGuildInfo);
	
	this->m_bCastleTowerAccessable = FALSE;
	this->m_bRegCrownAvailable = FALSE;
	this->m_btIsSiegeGuildList = 0;
	this->m_btIsSiegeEnded = 0;
}

void CCastleSiege::SetGateBlockState(int iX,int iY,int iGateState)
{
	if(XY_MACRO(iX) == FALSE || XY_MACRO(iY) == FALSE)
	{
		return;
	}

	switch(iGateState)
	{
	case 0:
		{
			for(int i = iX -2; i <= iX + 3; i++)
			{
				for(int j = iY; j <= iY + 2;j++)
				{
					MapC[MAP_INDEX_CASTLESIEGE].m_attrbuf[j * 256 + i] |= 16;
				}
			}
		}
		break;
	case 1:
		{
			for(int i = iX -2; i <= iX + 3; i++)
			{
				for(int j = iY; j <= iY + 2;j++)
				{
					MapC[MAP_INDEX_CASTLESIEGE].m_attrbuf[j * 256 + i] &= ~16;
				}
			}
		}
		break;
	}
}

void CCastleSiege::CheckBuildCsGuildInfo()
{
	this->m_bCsBasicGuildInfoLoadOK = FALSE;
	this->m_bCsTotalGuildInfoLoadOK = FALSE;

	EnterCriticalSection(&this->m_critCsBasicGuildInfo);
	this->m_mapCsBasicGuildInfo.clear();
	LeaveCriticalSection(&this->m_critCsBasicGuildInfo);

	EnterCriticalSection(&this->m_critCsTotalGuildInfo);
	this->m_mapCsTotalGuildInfo.clear();
	LeaveCriticalSection(&this->m_critCsTotalGuildInfo);

	if(this->m_btIsSiegeGuildList == TRUE)
	{
		GS_GDReqCsLoadTotalGuildInfo(this->m_iMapSvrGroup);
	}
	else
	{
		GS_GDReqCalcRegGuildList(this->m_iMapSvrGroup);
	}
}

void CCastleSiege::SetCalcRegGuildList(CSP_CALCREGGUILDLIST* lpMsg, int iCOUNT)
{
	if( lpMsg == NULL )
	{
		return;
	}

	if( iCOUNT > 100 ) 
	{
		iCOUNT = 100;
	}

	if( iCOUNT <= 0 )
	{
		return;
	}

	this->m_bCsBasicGuildInfoLoadOK = FALSE;
	this->m_bCsTotalGuildInfoLoadOK = FALSE;

	std::vector<_CS_REG_GUILD_DATA> vtRegGuildScore;

	for( int iGCNT = 0; iGCNT < iCOUNT; iGCNT++ )
	{
		_CS_REG_GUILD_DATA GuildData;

		char szGuildName[16] = {0};

		memcpy(szGuildName, lpMsg[iGCNT].szGuildName, MAX_GUILD_LEN);
		
		GuildData.m_bIN_USE = TRUE;
		GuildData.m_strGuildName = szGuildName;
		GuildData.m_iRegMarkCount = lpMsg[iGCNT].iRegMarkCount;
		GuildData.m_iGuildMasterLevel = lpMsg[iGCNT].iGuildMasterLevel;
		GuildData.m_iGuildMemberCount = lpMsg[iGCNT].iGuildMemberCount;
		GuildData.m_iSeqNum = 0x100000 - lpMsg[iGCNT].iSeqNum; 

		int dwHighScore = ( ( lpMsg[iGCNT].iRegMarkCount * 5 ) + lpMsg[iGCNT].iGuildMemberCount + (lpMsg[iGCNT].iGuildMasterLevel / 4));

		#define MACRO3(value) (((__int64) value & 0xFFFFFFFF ))

		GuildData.m_i64TotolScore = ((__int64)(DWORD)MACRO3(lpMsg[iGCNT].iSeqNum)) | (((__int64)(DWORD)MACRO3(dwHighScore))<<32);

		vtRegGuildScore.push_back(GuildData);

		g_Log.Add("[CastleSiege][REG Mark Count] - [%s][%d]", szGuildName, lpMsg[iGCNT].iRegMarkCount);
	}
 
	if( vtRegGuildScore.empty() )
	{
		return;
	}

	sort(vtRegGuildScore.begin(), vtRegGuildScore.end(), CCastleSiege::RegGuildScoreCompFunc);

	int iGUILD_COUNT = vtRegGuildScore.size();

	if( iGUILD_COUNT > 3 ) 
	{
		iGUILD_COUNT = 3;
	}

	EnterCriticalSection(&this->m_critCsBasicGuildInfo);

	this->m_mapCsBasicGuildInfo.clear();

	for( int iGCNT = 0; iGCNT < iGUILD_COUNT; iGCNT++ )
	{
		_CS_TOTAL_GUILD_DATA GuildData;
		
		GuildData.m_bIN_USE = TRUE;
		GuildData.m_iCsGuildID = (iGCNT + 2);
		GuildData.m_bGuildInvolved = TRUE;
		GuildData.m_iGuildScore = (vtRegGuildScore[iGCNT].m_iRegMarkCount*5) + (vtRegGuildScore[iGCNT].m_iGuildMemberCount) + (vtRegGuildScore[iGCNT].m_iGuildMasterLevel/4); //season 2.5 add-on

		this->m_mapCsBasicGuildInfo.insert(std::pair<string,_CS_TOTAL_GUILD_DATA>(vtRegGuildScore[iGCNT].m_strGuildName, GuildData));
	}

	if( this->m_btIsCastleOccupied == TRUE )
	{
		if( strcmp(this->m_szCastleOwnerGuild, "") != 0 )
		{
			_CS_TOTAL_GUILD_DATA GuildData;

			GuildData.m_bIN_USE = TRUE;
			GuildData.m_iCsGuildID = 1;
			GuildData.m_bGuildInvolved = TRUE;
			GuildData.m_iGuildScore = 0; //season 2.5 add-on
			
			this->m_mapCsBasicGuildInfo.insert(std::pair<string,_CS_TOTAL_GUILD_DATA>(m_szCastleOwnerGuild, GuildData));
		}
	}

	LeaveCriticalSection(&this->m_critCsBasicGuildInfo);

	char cBUFFER[4200];

	CSP_REQ_CSGUILDUNIONINFO* lpMsgSend = (CSP_REQ_CSGUILDUNIONINFO*)cBUFFER;
	CSP_CSGUILDUNIONINFO* lpMsgSendBody = (CSP_CSGUILDUNIONINFO*)&cBUFFER[sizeof(CSP_REQ_CSGUILDUNIONINFO)];

	lpMsgSend->wMapSvrNum = this->m_iMapSvrGroup;
	lpMsgSend->iCount = 0;

	EnterCriticalSection(&this->m_critCsTotalGuildInfo);

	m_mapCsTotalGuildInfo.clear();

	std::map<string,_CS_TOTAL_GUILD_DATA>::iterator it = this->m_mapCsBasicGuildInfo.begin();

	for(; it != this->m_mapCsBasicGuildInfo.end(); it++)
	{
		if( lpMsgSend->iCount > 150 )
		{
			break;
		}

		this->m_mapCsTotalGuildInfo.insert(std::pair<string,_CS_TOTAL_GUILD_DATA>(it->first, it->second));

		string GuildName = it->first;

		memcpy(&lpMsgSendBody[lpMsgSend->iCount].szGuildName,  &GuildName[0], 8);
		lpMsgSendBody[lpMsgSend->iCount].iCsGuildID = it->second.m_iCsGuildID;

		lpMsgSend->iCount++;
	}

	LeaveCriticalSection(&this->m_critCsTotalGuildInfo);

	this->m_bCsBasicGuildInfoLoadOK = TRUE;

	lpMsgSend->h.set((LPBYTE)&cBUFFER, 0x86, (lpMsgSend->iCount * sizeof(CSP_CSGUILDUNIONINFO))+ sizeof(CSP_REQ_CSGUILDUNIONINFO));
	wsDataCli.DataSend((char*)cBUFFER, (lpMsgSend->iCount * sizeof(CSP_CSGUILDUNIONINFO))+ sizeof(CSP_REQ_CSGUILDUNIONINFO));
}

BYTE CCastleSiege::RegGuildScoreCompFunc(_CS_REG_GUILD_DATA & A, _CS_REG_GUILD_DATA & B)
{
	if(A.m_i64TotolScore < B.m_i64TotolScore)
	{
		return false;
	}
	return true;
}

void CCastleSiege::MakeCsTotalGuildInfo(CSP_CSGUILDUNIONINFO* lpMsg, int iCOUNT)
{
	if( lpMsg == NULL )
	{
		return;
	}

	if( this->m_bCsBasicGuildInfoLoadOK == FALSE )
	{
		g_Log.AddC(TColor::Red,  "[CastleSiege] CCastleSiege::MakeCsTotalGuildInfo() ERROR - m_bCsBasicGuildInfoLoadOK == FALSE");
		return;
	}
	if( iCOUNT <= 0 )
	{
		g_Log.AddC(TColor::Red,  "[CastleSiege] CCastleSiege::MakeCsTotalGuildInfo() ERROR - iCOUNT <= 0 : %d", iCOUNT);
		return;
	}

	EnterCriticalSection(&this->m_critCsTotalGuildInfo);

	for( int iGCNT = 0; iGCNT < iCOUNT; iGCNT++ )
	{
		char szGuildName[16] = {0};

		memcpy(szGuildName, lpMsg[iGCNT].szGuildName, MAX_GUILD_LEN);

		std::map<string,_CS_TOTAL_GUILD_DATA>::iterator it = this->m_mapCsTotalGuildInfo.find(szGuildName);

		if( it != this->m_mapCsTotalGuildInfo.end() )
		{
			continue;
		}

		_CS_TOTAL_GUILD_DATA GuildData;
		
		GuildData.m_bIN_USE = TRUE;
		GuildData.m_iCsGuildID = lpMsg[iGCNT].iCsGuildID;
		GuildData.m_bGuildInvolved = FALSE;

		this->m_mapCsTotalGuildInfo.insert(std::pair<string,_CS_TOTAL_GUILD_DATA>(szGuildName, GuildData));
	}

	LeaveCriticalSection(&this->m_critCsTotalGuildInfo);

	this->m_bCsTotalGuildInfoLoadOK = TRUE;
	this->SaveCsTotalGuildInfo();
	this->SetAllUserCsJoinSide();
}

void CCastleSiege::SaveCsTotalGuildInfo()
{
	if( this->m_bCsTotalGuildInfoLoadOK == FALSE )
	{
		g_Log.AddC(TColor::Red,  "[CastleSiege] CCastleSiege::SaveCsTotalGuildInfo() ERROR - m_bCsTotalGuildInfoLoadOK == FALSE");
		return;
	}

	char cBUFFER[3288]; //season 2.5 changed

	CSP_REQ_CSSAVETOTALGUILDINFO* lpMsgSend = (CSP_REQ_CSSAVETOTALGUILDINFO*)&cBUFFER[0];
	CSP_CSSAVETOTALGUILDINFO* lpMsgSendBody =(CSP_CSSAVETOTALGUILDINFO*)&cBUFFER[sizeof(CSP_REQ_CSSAVETOTALGUILDINFO)];

	lpMsgSend->wMapSvrNum = this->m_iMapSvrGroup;
	lpMsgSend->iCount = 0;

	EnterCriticalSection(&this->m_critCsTotalGuildInfo);

	std::map<string,_CS_TOTAL_GUILD_DATA>::iterator it = this->m_mapCsTotalGuildInfo.begin();

	for(; it != this->m_mapCsTotalGuildInfo.end(); it++)
	{
		if( lpMsgSend->iCount > 150 )
		{
			break;
		}

		string szGuildName = it->first;

		memcpy(&lpMsgSendBody[lpMsgSend->iCount].szGuildName[0], &szGuildName[0], MAX_GUILD_LEN);
		
		lpMsgSendBody[lpMsgSend->iCount].iCsGuildID = it->second.m_iCsGuildID;
		lpMsgSendBody[lpMsgSend->iCount].iGuildInvolved = it->second.m_bGuildInvolved;
		lpMsgSendBody[lpMsgSend->iCount].iGuildScore = it->second.m_iGuildScore; //season 2.5 add-on

		lpMsgSend->iCount++;
	}

	LeaveCriticalSection(&this->m_critCsTotalGuildInfo);

	lpMsgSend->h.set((LPBYTE)cBUFFER, 0x87, (lpMsgSend->iCount * sizeof(CSP_CSSAVETOTALGUILDINFO))+ sizeof(CSP_REQ_CSSAVETOTALGUILDINFO));
	wsDataCli.DataSend((char*)cBUFFER, (lpMsgSend->iCount * sizeof(CSP_CSSAVETOTALGUILDINFO))+ sizeof(CSP_REQ_CSSAVETOTALGUILDINFO));

	EnterCriticalSection(&this->m_critCsTotalGuildInfo);

	std::map<string,_CS_TOTAL_GUILD_DATA>::iterator it2 = this->m_mapCsTotalGuildInfo.begin();

	for( int iSize = this->m_mapCsTotalGuildInfo.size(); it2 != this->m_mapCsTotalGuildInfo.end(); it2++ )
	{
		string szGuildName = it2->first;

		_CS_TOTAL_GUILD_DATA GuildData = it2->second;

		g_Log.Add("[CastleSiege] Siege Guild List Save - GNAME:%s, CSGUID:%d, INVOLVED:%d, SCORE:%d", (char*)&szGuildName[0], GuildData.m_iCsGuildID, GuildData.m_bGuildInvolved, GuildData.m_iGuildScore); //season 2.5 changed
	}

	LeaveCriticalSection(&this->m_critCsTotalGuildInfo);
}

void CCastleSiege::SetCsTotalGuildInfo(CSP_CSLOADTOTALGUILDINFO* lpMsg, int iCOUNT)
{
	if( lpMsg == NULL )
	{
		return;
	}

	if( iCOUNT <= 0 )
	{
		g_Log.AddC(TColor::Red,  "[CastleSiege] CCastleSiege::SetCsTotalGuildInfo() ERROR - iCOUNT <= 0 : %d", iCOUNT);
		return;
	}

	for( int iGCNT = 0; iGCNT < iCOUNT; iGCNT++ )
	{
		char szGuildName[16] = {0};

		memcpy(szGuildName, lpMsg[iGCNT].szGuildName, MAX_GUILD_LEN);

		_CS_TOTAL_GUILD_DATA GuildData;
		
		GuildData.m_bIN_USE = TRUE;
		GuildData.m_iCsGuildID = lpMsg[iGCNT].iCsGuildID;
		GuildData.m_bGuildInvolved = lpMsg[iGCNT].iGuildInvolved;
		GuildData.m_iGuildScore = lpMsg[iGCNT].iGuildScore; //season 2.5 add-on

		EnterCriticalSection(&this->m_critCsTotalGuildInfo);
		this->m_mapCsTotalGuildInfo.insert(std::pair<string,_CS_TOTAL_GUILD_DATA>(szGuildName, GuildData));
		LeaveCriticalSection(&this->m_critCsTotalGuildInfo);
		
		if( lpMsg[iGCNT].iGuildInvolved == TRUE )
		{
			EnterCriticalSection(&this->m_critCsBasicGuildInfo);
			this->m_mapCsBasicGuildInfo.insert(std::pair<string,_CS_TOTAL_GUILD_DATA>(szGuildName, GuildData));
			LeaveCriticalSection(&this->m_critCsBasicGuildInfo);
		}
	}

	this->m_bCsBasicGuildInfoLoadOK = TRUE;
	this->m_bCsTotalGuildInfoLoadOK = TRUE;
	this->SetAllUserCsJoinSide();

	EnterCriticalSection(&this->m_critCsTotalGuildInfo);

	std::map<string,_CS_TOTAL_GUILD_DATA>::iterator it2 = this->m_mapCsTotalGuildInfo.begin();
	
	int iSize = this->m_mapCsTotalGuildInfo.size();

	for( ; it2 != this->m_mapCsTotalGuildInfo.end(); it2++ )
	{
		string GuildName = it2->first;

		_CS_TOTAL_GUILD_DATA GuildData = _CS_TOTAL_GUILD_DATA(it2->second);

		g_Log.Add("[CastleSiege] Siege Guild List Load - GNAME:%s, CSGUID:%d, INVOLVED:%d, SCORE:%d", &GuildName[0], GuildData.m_iCsGuildID, GuildData.m_bGuildInvolved, GuildData.m_iGuildScore); //season 2.5 changed
	}

	LeaveCriticalSection(&m_critCsTotalGuildInfo);
}

int CCastleSiege::GetCsJoinSide(char* lpszGuildName, BYTE * btCsJoinSide, bool * bCsGuildInvolved)
{
	*btCsJoinSide = 0;
	*bCsGuildInvolved = 0;

	if( lpszGuildName == NULL )
	{
		return FALSE;
	}

	if( this->m_bDoRun == FALSE )
	{
		return FALSE;
	}

	if( this->m_iCastleSiegeState < CASTLESIEGE_STATE_NOTIFY || CASTLESIEGE_STATE_STARTSIEGE < this->m_iCastleSiegeState )
	{
		return FALSE;
	}

	if( this->m_bCsTotalGuildInfoLoadOK == FALSE )
	{
		return FALSE;
	}
	
	BOOL bRETVAL = FALSE;

	EnterCriticalSection(&this->m_critCsTotalGuildInfo);

	std::map<string,_CS_TOTAL_GUILD_DATA>::iterator it = this->m_mapCsTotalGuildInfo.find(lpszGuildName);

	if( it != this->m_mapCsTotalGuildInfo.end() )
	{
		*btCsJoinSide = it->second.m_iCsGuildID;
		*bCsGuildInvolved = (it->second.m_bGuildInvolved != 0);
		bRETVAL = TRUE;
	}
	
	LeaveCriticalSection(&this->m_critCsTotalGuildInfo);

	return bRETVAL;
}

void CCastleSiege::SetAllUserCsJoinSide()
{
	if(this->m_bCsTotalGuildInfoLoadOK == FALSE)
	{
		return;
	}

	EnterCriticalSection(&this->m_critCsTotalGuildInfo);

	for(int iUSER = g_ConfigRead.server.GetObjectStartUserIndex(); iUSER < g_ConfigRead.server.GetObjectMax(); iUSER++)
	{
		if(gObjIsConnected(iUSER) == FALSE)
		{
			continue;
		}

		char szGuildName[16] = {0};

		memcpy(szGuildName,gObj[iUSER].m_PlayerData->GuildName,MAX_GUILD_LEN);

		std::map<string,_CS_TOTAL_GUILD_DATA>::iterator it = this->m_mapCsTotalGuildInfo.find(szGuildName);
		
		if( it != this->m_mapCsTotalGuildInfo.end() )
		{
			gObj[iUSER].m_btCsJoinSide = it->second.m_iCsGuildID;
			gObj[iUSER].m_bCsGuildInvolved = (it->second.m_bGuildInvolved != 0);
		}

		this->NotifySelfCsJoinSide(iUSER);

		gObj[iUSER].m_iAccumulatedCrownAccessTime = 0;
	}

	LeaveCriticalSection(&this->m_critCsTotalGuildInfo);
}

void CCastleSiege::ResetAllUserCsJoinSide()
{
	EnterCriticalSection(&this->m_critCsTotalGuildInfo);

	for(int iUSER = g_ConfigRead.server.GetObjectStartUserIndex(); iUSER < g_ConfigRead.server.GetObjectMax(); iUSER++)
	{
		if(::gObjIsConnected(iUSER) == FALSE)
		{
			continue;
		}

		gObj[iUSER].m_btCsJoinSide = 0;
		gObj[iUSER].m_bCsGuildInvolved = 0;
		gObj[iUSER].m_btLifeStoneCount = 0;

		this->NotifySelfCsJoinSide(iUSER);

		gObj[iUSER].m_iAccumulatedCrownAccessTime = 0; //HermeX Fix 
	}

	LeaveCriticalSection(&this->m_critCsTotalGuildInfo);
}

void CCastleSiege::NotifySelfCsJoinSide(int iIndex)
{
	if(gObjIsConnected(iIndex) == FALSE)
	{
		return;
	}

	int iState = 0;
	int iViewState = 0;

	BYTE btCsJoinSide = gObj[iIndex].m_btCsJoinSide; //loc4

	PMSG_ANS_NOTIFYCSJOINSIDE pMsg;

	if(btCsJoinSide < 0)
	{
		btCsJoinSide = 0;
	}

	if(btCsJoinSide > 4) //season3.0 changed
	{
		btCsJoinSide = 0;
	}

	LPOBJ lpObj = &gObj[iIndex];

	switch(btCsJoinSide) //season3 all changed
	{
	case 1:
		iState = 1;
		iViewState = 0x40000;
		if(this->m_iCastleSiegeState == CASTLESIEGE_STATE_STARTSIEGE || this->m_iCastleSiegeState == CASTLESIEGE_STATE_ENDSIEGE)
		{
			gObjAddBuffEffect(lpObj, BUFFTYPE_CASTLE_DEFENSE_MARK, 0, 0, 0, 0, -10);
		}
		else
		{
			gObjRemoveBuffEffect(lpObj, BUFFTYPE_CASTLE_DEFENSE_MARK);
		}
		break;
	case 2:
		iState = 1;
		iViewState = 0x80000;
		if(this->m_iCastleSiegeState == CASTLESIEGE_STATE_STARTSIEGE || this->m_iCastleSiegeState == CASTLESIEGE_STATE_ENDSIEGE)
		{
			gObjAddBuffEffect(lpObj, BUFFTYPE_CASTLE_ATTACK_MARK, 0, 0, 0, 0, -10);
		}
		else
		{
			gObjRemoveBuffEffect(lpObj, BUFFTYPE_CASTLE_ATTACK_MARK);
		}
		break;
	case 3:
		iState = 1;
		iViewState = 0x400000;
		if(this->m_iCastleSiegeState == CASTLESIEGE_STATE_STARTSIEGE || this->m_iCastleSiegeState == CASTLESIEGE_STATE_ENDSIEGE)
		{
			gObjAddBuffEffect(lpObj, BUFFTYPE_CASTLE_ATTACK_MARK_2, 0, 0, 0, 0, -10);
		}
		else
		{
			gObjRemoveBuffEffect(lpObj, BUFFTYPE_CASTLE_ATTACK_MARK_2);
		}
		break;
	case 4:
		iState = 1;
		iViewState = 0x800000;
		if(this->m_iCastleSiegeState == CASTLESIEGE_STATE_STARTSIEGE || this->m_iCastleSiegeState == CASTLESIEGE_STATE_ENDSIEGE)
		{
			gObjAddBuffEffect(lpObj, BUFFTYPE_CASTLE_ATTACK_MARK_3, 0, 0, 0, 0, -10);
		}
		else
		{
			gObjRemoveBuffEffect(lpObj, BUFFTYPE_CASTLE_ATTACK_MARK_3);
		}
		break;
	case 0: //season4.5 fix
	//default:
		iState = 0;
		iViewState = 0xC0000;
		gObjClearBuffEffect(lpObj, CLEAR_TYPE_CASTLESIEGE);
	}

	pMsg.h.set((LPBYTE)&pMsg,0xB2,0x19,sizeof(pMsg));

	if(btCsJoinSide == 2) //same result...
	{
		pMsg.btCsJoinSide = gObj[iIndex].m_btCsJoinSide;
	}
	else
	{
		pMsg.btCsJoinSide = btCsJoinSide;
	}

	//GCSendState iViewState

	IOCP.DataSend(iIndex,(LPBYTE)&pMsg,pMsg.h.size);
}

int CCastleSiege::OperateGate(int iObjIndex, int iMonsterExistVal, int bOpenType)
{
	if( gObjIsConnected(iObjIndex) == FALSE )
	{
		return FALSE;
	}

	BOOL bRETVAL = FALSE;

	EnterCriticalSection(&this->m_critNpcData);

	std::vector<_CS_NPC_DATA>::iterator it = this->m_vtNpcData.begin();

	while( it != this->m_vtNpcData.end() )
	{
		_CS_NPC_DATA & NpcData = _CS_NPC_DATA(*it);

		if( NpcData.m_iNPC_NUM == 277 && NpcData.m_iNPC_OBJINDEX == iObjIndex && NpcData.m_bIN_USE == TRUE && NpcData.m_iNPC_LIVE == 2 )
		{
			int iObjNumber = NpcData.m_iNPC_OBJINDEX;

			if( gObj[iObjNumber].m_btCsNpcType != 0 && iMonsterExistVal == NpcData.m_iCsNpcExistVal )
			{
				this->SetGateBlockState(NpcData.m_iNPC_SX, NpcData.m_iNPC_SY, bOpenType);
				gObjAddBuffEffect(&gObj[iObjNumber], BUFFTYPE_CASTLE_DOOR_STATE, 0, 0, 0, 0, -10);
				gObj[iObjNumber].m_btCsGateOpen = bOpenType;
				bRETVAL = TRUE;
			}
			break;
		}
		it++;
	}

	LeaveCriticalSection(&this->m_critNpcData);

	return bRETVAL;
}

int CCastleSiege::OperateGate(int iNpcIndex, int bOpenType)
{
	BOOL bRETVAL = FALSE;

	EnterCriticalSection(&this->m_critNpcData);

	std::vector<_CS_NPC_DATA>::iterator it = this->m_vtNpcData.begin();

	while( it != this->m_vtNpcData.end() )
	{
		_CS_NPC_DATA & NpcData = _CS_NPC_DATA(*it);

		if( NpcData.m_iNPC_NUM == 277 && NpcData.m_iNPC_INDEX == iNpcIndex && NpcData.m_bIN_USE == TRUE && NpcData.m_iNPC_LIVE == 2 )
		{
			int iObjNumber = NpcData.m_iNPC_OBJINDEX;

			if( gObjIsConnected(iObjNumber) && gObj[iObjNumber].m_btCsNpcType != 0 && gObj[iObjNumber].m_iCsNpcExistVal == NpcData.m_iCsNpcExistVal && gObj[iObjNumber].m_iCsNpcExistVal != 0 )
			{
					this->SetGateBlockState(NpcData.m_iNPC_SX, NpcData.m_iNPC_SY, bOpenType);
					
					if(bOpenType == 0)//Season 4.5 addon
					{
						gObjRemoveBuffEffect(&gObj[iObjNumber], BUFFTYPE_CASTLE_DOOR_STATE);
					}
					else
					{
						gObjAddBuffEffect(&gObj[iObjNumber], BUFFTYPE_CASTLE_DOOR_STATE, 0, 0, 0, 0, -10);
					}
					
					gObj[iObjNumber].m_btCsGateOpen = bOpenType;
					bRETVAL = TRUE;
			}
			break;
		}
		it++;
	}

	LeaveCriticalSection(&this->m_critNpcData);

	return bRETVAL;
}

int CCastleSiege::CheckAttackGuildExist()
{
	int iRETVAL = FALSE;

	EnterCriticalSection(&this->m_critCsTotalGuildInfo);

	if(!this->m_mapCsTotalGuildInfo.empty())
	{
		iRETVAL = TRUE;
	}

	LeaveCriticalSection(&this->m_critCsTotalGuildInfo);
	return iRETVAL;
}

BOOL CCastleSiege::CheckCastleOwnerMember(int iIndex)
{
	if( gObjIsConnected(iIndex) == FALSE )
	{
		return FALSE;
	}

	if( strcmp(this->m_szCastleOwnerGuild, "") == 0 )
	{
		return FALSE;
	}

	if( strcmp(gObj[iIndex].m_PlayerData->GuildName, "") == 0 )
	{
		return FALSE;
	}

	if( strcmp(gObj[iIndex].m_PlayerData->GuildName, this->m_szCastleOwnerGuild) != 0 )
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CCastleSiege::CheckCastleOwnerUnionMember(int iIndex)
{
	_GUILD_INFO_STRUCT* lpGuildInfo;
	TUnionInfo * pUnionInfo;

	if( ::gObjIsConnected(iIndex) == FALSE )
	{
		return FALSE;
	}

	if( strcmp(this->m_szCastleOwnerGuild, "") == 0 )
	{
		return FALSE;
	}

	if( strcmp(gObj[iIndex].m_PlayerData->GuildName, "") == 0 )
	{
		return FALSE;
	}

	lpGuildInfo = gObj[iIndex].m_PlayerData->lpGuild;

	if( lpGuildInfo == NULL )
	{
		return FALSE;
	}

	pUnionInfo = UnionManager.SearchUnion(lpGuildInfo->iGuildUnion);

	if( pUnionInfo == NULL )
	{
		return FALSE;
	}

	if( strcmp(pUnionInfo->m_szMasterGuild, this->m_szCastleOwnerGuild) == 0 )
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CCastleSiege::CheckGuardianStatueExist()
{
	for( int iIDX = 0; iIDX < g_ConfigRead.server.GetObjectMaxMonster(); iIDX++ )
	{
		if( gObj[iIDX].Class == 283 )
		{
			if( gObj[iIDX].Connected > PLAYER_LOGGED && gObj[iIDX].Live == 1 )
			{
				if( gObj[iIDX].MapNumber == MAP_INDEX_CASTLESIEGE )
				{
					return TRUE;
				}
			}
		}
	}

	return FALSE;
}

void CCastleSiege::ChangeWinnerGuild(int iCsJoinSide)
{
	char szGuildTo[16] = {0};
	char szGuildFrom[16] = {0};

	EnterCriticalSection(&this->m_critCsTotalGuildInfo);

	std::map<string,_CS_TOTAL_GUILD_DATA>::iterator it = this->m_mapCsTotalGuildInfo.begin();
	
	for(; it != this->m_mapCsTotalGuildInfo.end(); it++)
	{
		if(it->second.m_iCsGuildID == iCsJoinSide)
		{
			it->second.m_iCsGuildID = 1;
		
			if(it->second.m_bGuildInvolved == TRUE)
			{
				memcpy(&szGuildTo,&it->first[0], MAX_GUILD_LEN);
			}
		}
		else if(it->second.m_iCsGuildID == 1)
		{
			it->second.m_iCsGuildID = iCsJoinSide;
		
			if(it->second.m_bGuildInvolved == TRUE)
			{
				memcpy(&szGuildFrom,&it->first[0], MAX_GUILD_LEN);
			}
		}
	}

	for( int iUSER = g_ConfigRead.server.GetObjectStartUserIndex(); iUSER < g_ConfigRead.server.GetObjectMax(); iUSER++ )
	{
		if(!gObjIsConnected(iUSER))
		{
			continue;
		}
			
//		gObj[iUSER].m_ViewSkillState &= 0xFFF3FFFF;
//		gObj[iUSER].m_ViewSkillState &= 0xFFBFFFFF; //HermeX Fix
//		gObj[iUSER].m_ViewSkillState &= 0xFF7FFFFF; //HermeX Fix
		
		char szNewGuild[16] = {0};

		memcpy(&szNewGuild,gObj[iUSER].m_PlayerData->GuildName,MAX_GUILD_LEN);

		std::map<string,_CS_TOTAL_GUILD_DATA>::iterator it = this->m_mapCsTotalGuildInfo.find(szNewGuild);
	
		if( it != this->m_mapCsTotalGuildInfo.end() )
		{
			gObj[iUSER].m_btCsJoinSide = it->second.m_iCsGuildID;
			gObj[iUSER].m_bCsGuildInvolved = (it->second.m_bGuildInvolved != 0);
		}
		gObj[iUSER].m_iAccumulatedCrownAccessTime = 0;
	}

	LeaveCriticalSection(&this->m_critCsTotalGuildInfo);

	g_Log.Add("[CastleSiege] Castle Winner Guild Change From '%s' To '%s'", szGuildFrom, szGuildTo);
}

BOOL CCastleSiege::CheckMiddleWinnerGuild()
{
	if( this->m_dwCrownAccessTime == 0 )
	{
		return FALSE;
	}

	if( (gObjIsConnected(this->m_iCastleCrownAccessUser) == FALSE) || (gObjIsConnected(this->m_iCastleSwitchAccessUser1) == FALSE) || (gObjIsConnected(this->m_iCastleSwitchAccessUser2) == FALSE) )
	{
		return FALSE;
	}

	if( (strcmp(gObj[this->m_iCastleCrownAccessUser].m_PlayerData->GuildName, "") == 0) || (strcmp(gObj[this->m_iCastleSwitchAccessUser1].m_PlayerData->GuildName, "") == 0) || (strcmp(gObj[this->m_iCastleSwitchAccessUser2].m_PlayerData->GuildName, "") == 0) )
	{
		return FALSE;
	}

	if( (gObj[this->m_iCastleCrownAccessUser].m_btCsJoinSide >= 2 ) && (gObj[this->m_iCastleSwitchAccessUser1].m_btCsJoinSide >= 2 ) && (gObj[this->m_iCastleSwitchAccessUser2].m_btCsJoinSide >= 2 ) )
	{
		if( gObj[this->m_iCastleCrownAccessUser].m_bCsGuildInvolved == 1 )
		{
			if( (gObj[this->m_iCastleCrownAccessUser].m_btCsJoinSide == gObj[this->m_iCastleSwitchAccessUser1].m_btCsJoinSide) && (gObj[this->m_iCastleCrownAccessUser].m_btCsJoinSide == gObj[this->m_iCastleSwitchAccessUser2].m_btCsJoinSide) )
			{
				if( ( ( GetTickCount() - this->m_dwCrownAccessTime ) + gObj[this->m_iCastleCrownAccessUser].m_iAccumulatedCrownAccessTime ) >= 60000 ) //season4.5 changed
				{
					g_Log.Add("[CastleSiege] Castle Winner Has been Changed (GUILD:%s)", gObj[this->m_iCastleCrownAccessUser].m_PlayerData->GuildName);

					GSProtocol.GCAnsCsAccessCrownState(this->m_iCastleCrownAccessUser, 1);

					this->NotifyCrownState(2);

					memset(&this->m_szMiddleWinnerGuild, 0, sizeof(this->m_szMiddleWinnerGuild));
					memcpy(&this->m_szMiddleWinnerGuild, gObj[this->m_iCastleCrownAccessUser].m_PlayerData->GuildName, 8);

					this->ClearNonDbNPC_MidWin();
					this->ChangeWinnerGuild(gObj[this->m_iCastleCrownAccessUser].m_btCsJoinSide);
					this->NotifyAllUserCsProgState(1, gObj[this->m_iCastleCrownAccessUser].m_PlayerData->GuildName);
					this->ResetCrownAccessTickCount();
					this->ResetCrownUserIndex();
					this->SetCrownAccessUserX(0);
					this->SetCrownAccessUserY(0);
					this->ResetCrownSwitchUserIndex(217);
					this->ResetCrownSwitchUserIndex(218);
					this->m_bRegCrownAvailable = FALSE;
					this->ReSpawnEnemyUser(FALSE);
	
					GSProtocol.GCAnsCsLeftTimeAlarm( (this->m_iCS_REMAIN_MSEC/1000)/3600, ((this->m_iCS_REMAIN_MSEC/1000)%3600)/60);

					this->m_dwCS_LEFTTIME_TICK_COUNT = GetTickCount();

					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

int CCastleSiege::CheckCastleSiegeResult()
{
	char szMsg[256] = {0};
	int bRETVAL = FALSE;

	if(m_btIsCastleOccupied == TRUE && strcmp(m_szCastleOwnerGuild,"") != FALSE)
	{
		if(strcmp(m_szMiddleWinnerGuild,"") == FALSE || strcmp(m_szMiddleWinnerGuild,m_szCastleOwnerGuild) == FALSE)
		{
			this->m_btIsCastleOccupied = 1;
			wsprintf(szMsg,Lang.GetText(0,198),m_szCastleOwnerGuild);
			bRETVAL = FALSE;
		}
		else
		{
			this->m_btIsCastleOccupied = 1;
			memset(this->m_szCastleOwnerGuild,0,sizeof(this->m_szCastleOwnerGuild));
			memcpy(this->m_szCastleOwnerGuild,this->m_szMiddleWinnerGuild,8);
			wsprintf(szMsg,Lang.GetText(0,199),this->m_szCastleOwnerGuild);
			bRETVAL = TRUE;
		}
	}
	else
	{
		if(strcmp(m_szMiddleWinnerGuild,"") == FALSE)
		{
			this->m_btIsCastleOccupied = 0;
			wsprintf(szMsg,Lang.GetText(0,200));
			bRETVAL = FALSE;
		}
		else
		{
			this->m_btIsCastleOccupied = 1;
			memset(this->m_szCastleOwnerGuild,0,sizeof(this->m_szCastleOwnerGuild));
			memcpy(this->m_szCastleOwnerGuild,this->m_szMiddleWinnerGuild,8);
			wsprintf(szMsg,Lang.GetText(0,199),this->m_szCastleOwnerGuild);
			bRETVAL = TRUE;
		}
	}

	this->m_btIsSiegeEnded = 1;

	g_Log.Add("[CastleSiege] Castle Final Winner Guild : %s",(strcmp(this->m_szCastleOwnerGuild,"") == FALSE) ? "NONE" : this->m_szCastleOwnerGuild);
	this->SendMapServerGroupMsg(szMsg);

	return bRETVAL;
}

BOOL CCastleSiege::CheckUnionGuildMaster(int iIndex)
{
	_GUILD_INFO_STRUCT* lpGuildInfo;

	if( gObjIsConnected(iIndex) == FALSE )
	{
		return FALSE;
	}

	lpGuildInfo = gObj[iIndex].m_PlayerData->lpGuild;

	if( lpGuildInfo == NULL )
	{
		return FALSE;
	}

	if( gObj[iIndex].m_PlayerData->GuildStatus == GUILD_MASTER )
	{
		if( (lpGuildInfo->Number == lpGuildInfo->iGuildUnion) || (lpGuildInfo->iGuildUnion == 0) )
		{
			return TRUE;
		}
	}

	return FALSE;
}

void CCastleSiege::SetCrownSwitchUserIndex(int iMonsterClass, int iIndex)
{
	switch( iMonsterClass )
	{
	case 217:
			this->m_iCastleSwitchAccessUser1 = iIndex;
		break;
	case 218:
			this->m_iCastleSwitchAccessUser2 = iIndex;
		break;
	}
}

int CCastleSiege::GetCrownSwitchUserIndex(int iMonsterClass)
{
	int iUserIndex = OBJ_EMPTY;

	switch( iMonsterClass )
	{
	case 217:
			iUserIndex = this->m_iCastleSwitchAccessUser1;
		break;
	case 218:
			iUserIndex = this->m_iCastleSwitchAccessUser2;
		break;
	}

	return iUserIndex;
}

void CCastleSiege::ResetCrownSwitchUserIndex(int iMonsterClass)
{
	switch( iMonsterClass )
	{
	case 217:
			this->m_iCastleSwitchAccessUser1 = OBJ_EMPTY;
		break;
	case 218:
			this->m_iCastleSwitchAccessUser2 = OBJ_EMPTY;
		break;
	}
}

void CCastleSiege::SetCrownAccessTickCount()
{
	this->m_dwCrownAccessTime = GetTickCount();
}

void CCastleSiege::ResetCrownAccessTickCount()
{
	this->m_dwCrownAccessTime = 0;
}

int CCastleSiege::CheckOverlapCsMarks(int iIndex)
{
	for( int x = INVETORY_WEAR_SIZE; x < MAIN_INVENTORY_SIZE; x++ )
	{
		if( gObj[iIndex].pInventory[x].IsItem() == TRUE )
		{
			if( (gObj[iIndex].pInventory[x].m_Type == ITEMGET(14,21)) && (gObj[iIndex].pInventory[x].m_Level == 3) )
			{
				int dur = gObj[iIndex].pInventory[x].m_Durability;

				if( ITEM_DUR_RANGE(dur) )
				{
					return x;
				}
			}
		}
	}
	return -1;
}

int CCastleSiege::GetCsAttkGuildList(PMSG_CSATTKGUILDLIST* lpMsgBody, int & iCount)
{
	iCount = 0;

	if( lpMsgBody == NULL ) return 0;

	if( (this->m_iCastleSiegeState != CASTLESIEGE_STATE_NOTIFY) && (this->m_iCastleSiegeState != CASTLESIEGE_STATE_READYSIEGE) )
	{
		return 2;
	}

	if( (this->m_bCsBasicGuildInfoLoadOK == FALSE) || (this->m_bCsTotalGuildInfoLoadOK == FALSE ) )
	{
		return 3;
	}

	EnterCriticalSection(&this->m_critCsTotalGuildInfo);

	std::map<string,_CS_TOTAL_GUILD_DATA>::iterator it = this->m_mapCsTotalGuildInfo.begin();

	for(;  it != this->m_mapCsTotalGuildInfo.end(); it++)
	{
		lpMsgBody[iCount].btCsJoinSide = it->second.m_iCsGuildID;
		lpMsgBody[iCount].btGuildInvolved = it->second.m_bGuildInvolved;
		
		memset(&lpMsgBody[iCount].szGuildName, 0, sizeof(lpMsgBody[iCount].szGuildName));
		memcpy(&lpMsgBody[iCount].szGuildName, &it->first[0], sizeof(lpMsgBody[iCount].szGuildName));

		lpMsgBody[iCount].iGuildScore = it->second.m_iGuildScore; //season 2.5 add-on

		iCount++;
	}

	LeaveCriticalSection(&this->m_critCsTotalGuildInfo);

	return 1;
}

BOOL CCastleSiege::CheckTeleportMagicAxisY(int iStartY, int iTargetX, int iTargetY)
{
	for( int iSTEP = 0; iSTEP < 3; iSTEP++ )
	{
		if( (g_iCastleWallStepAxisY[iSTEP] >= iStartY) && (g_iCastleWallStepAxisY[iSTEP] <= iTargetY) && (iTargetX < 144) )
		{
			return FALSE;
		}
	}
	return TRUE;
}

void CCastleSiege::NotifyCsSelfLeftTime(int iIndex)
{
	GSProtocol.GCAnsSelfCsLeftTimeAlarm(iIndex, (this->m_iCS_REMAIN_MSEC/1000)/3600, ((this->m_iCS_REMAIN_MSEC/1000)%3600)/60);
}

void CCastleSiege::CheckReviveNonDbNPC()
{
	EnterCriticalSection(&this->m_critNpcData);

	std::vector<_CS_NPC_DATA>::iterator it = this->m_vtNpcData.begin();

	while(it != this->m_vtNpcData.end())
	{
		_CS_NPC_DATA & pNpcData = _CS_NPC_DATA(*it);

		if(pNpcData.m_bNPC_DBSAVE == 0)
		{
			int iNPC_INDEX = gObjAddMonster(MAP_INDEX_CASTLESIEGE);

			if(iNPC_INDEX >= 0)
			{
				gObjSetMonster(iNPC_INDEX,pNpcData.m_iNPC_NUM);

				gObj[iNPC_INDEX].m_PosNum = -1;
				gObj[iNPC_INDEX].X = pNpcData.m_iNPC_SX;
				gObj[iNPC_INDEX].Y = pNpcData.m_iNPC_SY;
				gObj[iNPC_INDEX].MapNumber = MAP_INDEX_CASTLESIEGE;
				gObj[iNPC_INDEX].TX = gObj[iNPC_INDEX].X;
				gObj[iNPC_INDEX].TY = gObj[iNPC_INDEX].Y;
				gObj[iNPC_INDEX].m_OldX = gObj[iNPC_INDEX].X;
				gObj[iNPC_INDEX].m_OldY = gObj[iNPC_INDEX].Y;
				gObj[iNPC_INDEX].Dir = pNpcData.m_iNPC_DIR;
				gObj[iNPC_INDEX].StartX = gObj[iNPC_INDEX].X;
				gObj[iNPC_INDEX].StartY = gObj[iNPC_INDEX].Y;
				gObj[iNPC_INDEX].DieRegen = 0;
				gObj[iNPC_INDEX].MaxRegenTime = 0;
				gObj[iNPC_INDEX].Life = pNpcData.m_iNPC_HP;
				gObj[iNPC_INDEX].MaxLife = pNpcData.m_iNPC_MAXHP;

				switch(pNpcData.m_iNPC_NUM)
				{
				case 215:
					gObj[iNPC_INDEX].m_btCsNpcType = 2;
					break;
				case 221:
					gObj[iNPC_INDEX].m_btCsNpcType = 3;
					break;
				case 222:
					gObj[iNPC_INDEX].m_btCsNpcType = 2;
					break;
				case 288:
					gObj[iNPC_INDEX].m_btCsNpcType = 2;
					break;
				default:
					gObj[iNPC_INDEX].m_btCsNpcType = 2;
					break;
				}

				it->m_bIN_USE = TRUE;
			}
		}

		it++;
	}

	LeaveCriticalSection(&this->m_critNpcData);
}

void CCastleSiege::CheckReviveGuardianStatue()
{
	EnterCriticalSection(&this->m_critNpcData);

	std::vector<_CS_NPC_DATA>::iterator it = this->m_vtNpcData.begin();

	while(it != this->m_vtNpcData.end())
	{

		_CS_NPC_DATA & pNpcData = _CS_NPC_DATA(*it);

		if(pNpcData.m_iNPC_NUM == 283 && pNpcData.m_iNPC_LIVE == 0 )
		{
			int iNPC_INDEX = gObjAddMonster(MAP_INDEX_CASTLESIEGE);

			if(iNPC_INDEX >= 0)
			{
				gObjSetMonster(iNPC_INDEX,pNpcData.m_iNPC_NUM);

				gObj[iNPC_INDEX].m_PosNum = -1;
				gObj[iNPC_INDEX].X = pNpcData.m_iNPC_SX;
				gObj[iNPC_INDEX].Y = pNpcData.m_iNPC_SY;
				gObj[iNPC_INDEX].MapNumber = MAP_INDEX_CASTLESIEGE;
				gObj[iNPC_INDEX].TX = gObj[iNPC_INDEX].X;
				gObj[iNPC_INDEX].TY = gObj[iNPC_INDEX].Y;
				gObj[iNPC_INDEX].m_OldX = gObj[iNPC_INDEX].X;
				gObj[iNPC_INDEX].m_OldY = gObj[iNPC_INDEX].Y;
				gObj[iNPC_INDEX].Dir = pNpcData.m_iNPC_DIR;
				gObj[iNPC_INDEX].StartX = gObj[iNPC_INDEX].X;
				gObj[iNPC_INDEX].StartY = gObj[iNPC_INDEX].Y;
				gObj[iNPC_INDEX].DieRegen = 0;
				gObj[iNPC_INDEX].MaxRegenTime = 0;
				gObj[iNPC_INDEX].Life = pNpcData.m_iNPC_HP;
				gObj[iNPC_INDEX].MaxLife = pNpcData.m_iNPC_MAXHP;
				gObj[iNPC_INDEX].m_btCsNpcType = 1;

				int iNPC_DF_LEVEL = pNpcData.m_iNPC_BASE_DF_LEVEL;

				if(iNPC_DF_LEVEL < 0)
				{
					iNPC_DF_LEVEL = 0;
				}

				if(iNPC_DF_LEVEL > CS_MAX_UPGRADE)
				{
					iNPC_DF_LEVEL = CS_MAX_UPGRADE;
				}

				int iNPC_RG_LEVEL = pNpcData.m_iNPC_BASE_RG_LEVEL;

				if(iNPC_RG_LEVEL < 0)
				{
					iNPC_RG_LEVEL = 0;
				}

				if(iNPC_RG_LEVEL > CS_MAX_UPGRADE)
				{
					iNPC_RG_LEVEL = CS_MAX_UPGRADE;
				}

				gObj[iNPC_INDEX].m_Defense = g_iNpcDefense_CSTATUE[iNPC_DF_LEVEL];
				gObj[iNPC_INDEX].m_btCsNpcDfLevel = iNPC_DF_LEVEL;
				gObj[iNPC_INDEX].m_btCsNpcRgLevel = iNPC_RG_LEVEL;

				it->m_btCsNpcExistVal1 = rand()%255+1;
				gObj[iNPC_INDEX].m_btCsNpcExistVal1 = it->m_btCsNpcExistVal1;

				it->m_btCsNpcExistVal2 = rand()%256;
				gObj[iNPC_INDEX].m_btCsNpcExistVal2 = it->m_btCsNpcExistVal2;

				it->m_btCsNpcExistVal3 = rand()%256;
				gObj[iNPC_INDEX].m_btCsNpcExistVal3 = it->m_btCsNpcExistVal3;

				it->m_btCsNpcExistVal4 = rand()%256;
				gObj[iNPC_INDEX].m_btCsNpcExistVal4 = it->m_btCsNpcExistVal4;

				it->m_iNPC_OBJINDEX = iNPC_INDEX;
				it->m_iNPC_LIVE = 2;
				it->m_bIN_USE = TRUE;
			}
		}
		it++;
	}

	LeaveCriticalSection(&this->m_critNpcData);
}

void CCastleSiege::ReSpawnAllUser()
{
	int iTX, iTY;
	BYTE btMapAttr;

	for( int iUSER = g_ConfigRead.server.GetObjectStartUserIndex(); iUSER < g_ConfigRead.server.GetObjectMax(); iUSER++ )
	{
		if( gObjIsConnected(iUSER) == FALSE )
		{
			continue;
		}

		if( gObj[iUSER].MapNumber != MAP_INDEX_CASTLESIEGE )
		{
			continue;
		}

		BYTE btJoinSide = gObj[iUSER].m_btCsJoinSide;

		if( btJoinSide < 0 ) 
		{
			btJoinSide = 0;
		}

		if( btJoinSide > 2 )
		{
			btJoinSide = 2;
		}

		BOOL bMoveOK = FALSE;

		switch( btJoinSide )
		{
		case 1:
			{
				for(int iCNT = 0; iCNT < 100; iCNT++)
				{
					iTX = (rand() % (g_iCsUserReSpawnArea[1][2] - g_iCsUserReSpawnArea[1][0])) + g_iCsUserReSpawnArea[1][0];
					iTY = (rand() % (g_iCsUserReSpawnArea[1][3] - g_iCsUserReSpawnArea[1][1])) + g_iCsUserReSpawnArea[1][1];

					btMapAttr = MapC[MAP_INDEX_CASTLESIEGE].m_attrbuf[iTY * 256 + iTX];

					if((btMapAttr & 4) != 4)
					{
						bMoveOK = TRUE;
						break;
					}
				}
			}
			break;
		case 2:
			{
				for(int iCNT = 0; iCNT < 100; iCNT++)
				{
					iTX = (rand() % (g_iCsUserReSpawnArea[2][2] - g_iCsUserReSpawnArea[2][0])) + g_iCsUserReSpawnArea[2][0];
					iTY = (rand() % (g_iCsUserReSpawnArea[2][3] - g_iCsUserReSpawnArea[2][1])) + g_iCsUserReSpawnArea[2][1];

					btMapAttr = MapC[MAP_INDEX_CASTLESIEGE].m_attrbuf[iTY * 256 + iTX];

					if((btMapAttr & 4) != 4)
					{
						bMoveOK = TRUE;
						break;
					}
				}
			}
			break;
		default:
			{
				for(int iCNT = 0; iCNT < 100; iCNT++)
				{
					iTX = (rand() % (g_iCsUserReSpawnArea[0][2] - g_iCsUserReSpawnArea[0][0])) + g_iCsUserReSpawnArea[0][0];
					iTY = (rand() % (g_iCsUserReSpawnArea[0][3] - g_iCsUserReSpawnArea[0][1])) + g_iCsUserReSpawnArea[0][1];

					btMapAttr = MapC[MAP_INDEX_CASTLESIEGE].m_attrbuf[iTY * 256 + iTX];

					if((btMapAttr & 4) != 4)
					{
						bMoveOK = TRUE;
						break;
					}
				}
			}
			break;
		}

		if( bMoveOK )
		{
			gObj[iUSER].m_State = 32;
			gObj[iUSER].X = iTX;
			gObj[iUSER].Y = iTY;
			gObj[iUSER].TX = iTX;
			gObj[iUSER].TX = iTY;
			gObj[iUSER].MapNumber = MAP_INDEX_CASTLESIEGE;
			gObj[iUSER].PathCount = 0;
			gObj[iUSER].Teleport = 0;

			gObjClearViewport(&gObj[iUSER]);
			GSProtocol.GCTeleportSend(&gObj[iUSER], -1, MAP_INDEX_CASTLESIEGE, gObj[iUSER].X, gObj[iUSER].Y, gObj[iUSER].Dir);

			if( gObj[iUSER].m_Change >= 0 )
			{
				gObjViewportListProtocolCreate(&gObj[iUSER]);
			}
			gObj[iUSER].RegenMapNumber = MAP_INDEX_CASTLESIEGE;
			gObj[iUSER].RegenMapX = iTX;
			gObj[iUSER].RegenMapY = iTY;
			gObj[iUSER].RegenOk = TRUE;
		}
	}
}

void CCastleSiege::ReSpawnEnemyUser(int bRefreshOwnerUser)
{
	int iTX;
	int iTY;
	BYTE btMapAttr;

	for(int iUSER = g_ConfigRead.server.GetObjectStartUserIndex(); iUSER < g_ConfigRead.server.GetObjectMax(); iUSER++)
	{
		if(gObjIsConnected(iUSER) == FALSE)
		{
			continue;
		}

		if(gObj[iUSER].MapNumber != MAP_INDEX_CASTLESIEGE && gObj[iUSER].MapNumber != MAP_INDEX_CASTLEHUNTZONE)
		{
			continue;
		}

		BYTE btCsJoinSide = gObj[iUSER].m_btCsJoinSide;

		if(btCsJoinSide < 0)
		{
			btCsJoinSide = 0;
		}

		if(btCsJoinSide > 2)
		{
			btCsJoinSide = 2;
		}

		int bMoveOK = FALSE;

		switch(btCsJoinSide)
		{
		case 1:
			if(bRefreshOwnerUser != FALSE)
			{
				gObj[iUSER].RegenMapNumber = gObj[iUSER].MapNumber;
				gObj[iUSER].RegenMapX = gObj[iUSER].X;
				gObj[iUSER].RegenMapY = gObj[iUSER].Y;

				if(gObj[iUSER].MapNumber == MAP_INDEX_CASTLEHUNTZONE) //season 2.5 add-on
				{
					for(int iCNT = 0; iCNT < 100; iCNT++)
					{
						iTX = (rand() % (g_iCsUserReSpawnArea[2][2] - g_iCsUserReSpawnArea[2][0])) + g_iCsUserReSpawnArea[2][0];
						iTY = (rand() % (g_iCsUserReSpawnArea[2][3] - g_iCsUserReSpawnArea[2][1])) + g_iCsUserReSpawnArea[2][1];

						btMapAttr = MapC[MAP_INDEX_CASTLESIEGE].m_attrbuf[iTY * 256 + iTX];

						if((btMapAttr & 4) != 4)
						{
							bMoveOK = TRUE;
							break;
						}
					}
					break;
				}

				gObjClearViewport(&gObj[iUSER]);

				GSProtocol.GCTeleportSend(&gObj[iUSER],-1,gObj[iUSER].MapNumber,gObj[iUSER].X,gObj[iUSER].Y,gObj[iUSER].Dir); //season 2.5 changed

				if(gObj[iUSER].m_Change >= 0)
				{
					gObjViewportListProtocolCreate(&gObj[iUSER]);
				}

				gObj[iUSER].RegenOk = TRUE;
			}
			bMoveOK = FALSE;
			break;
		case 2:
			{
				for(int iCNT = 0; iCNT < 100; iCNT++)
				{
					iTX = (rand() % (g_iCsUserReSpawnArea[2][2] - g_iCsUserReSpawnArea[2][0])) + g_iCsUserReSpawnArea[2][0];
					iTY = (rand() % (g_iCsUserReSpawnArea[2][3] - g_iCsUserReSpawnArea[2][1])) + g_iCsUserReSpawnArea[2][1];

					btMapAttr = MapC[MAP_INDEX_CASTLESIEGE].m_attrbuf[iTY * 256 + iTX];

					if((btMapAttr & 4) != 4)
					{
						bMoveOK = TRUE;
						break;
					}
				}
			}
			break;
		case 0:
			{
				for(int iCNT = 0; iCNT < 100; iCNT++)
				{
					iTX = (rand()% (g_iCsUserReSpawnArea[0][2] - g_iCsUserReSpawnArea[0][0])) + g_iCsUserReSpawnArea[0][0];
					iTY = (rand()% (g_iCsUserReSpawnArea[0][3] - g_iCsUserReSpawnArea[0][1])) + g_iCsUserReSpawnArea[0][1];

					btMapAttr = MapC[MAP_INDEX_CASTLESIEGE].m_attrbuf[iTY * 256 + iTX];

					if((btMapAttr & 4) != 4)
					{
						bMoveOK = TRUE;
						break;
					}
				}
			}
			break;
		}

		if(bMoveOK != FALSE)
		{
			gObj[iUSER].m_State = 32;
			gObj[iUSER].X = iTX;
			gObj[iUSER].Y = iTY;
			gObj[iUSER].TX = iTX;
			gObj[iUSER].TX = iTY;
			gObj[iUSER].MapNumber = MAP_INDEX_CASTLESIEGE;
			gObj[iUSER].PathCount = 0;
			gObj[iUSER].Teleport = 0;

			gObjClearViewport(&gObj[iUSER]);
			GSProtocol.GCTeleportSend(&gObj[iUSER],-1,MAP_INDEX_CASTLESIEGE,gObj[iUSER].X,gObj[iUSER].Y,gObj[iUSER].Dir);

			if(gObj[iUSER].m_Change >= 0)
			{
				gObjViewportListProtocolCreate(&gObj[iUSER]);
			}

			gObj[iUSER].RegenMapNumber = MAP_INDEX_CASTLESIEGE;
			gObj[iUSER].RegenMapX = iTX;
			gObj[iUSER].RegenMapY = iTY;
			gObj[iUSER].RegenOk = TRUE;
		}

		this->NotifySelfCsJoinSide(iUSER);
		gObj[iUSER].m_iAccumulatedCrownAccessTime = 0;
	}
}

void CCastleSiege::NotifyAllUserCsStartState(BYTE btStartState)
{
	PMSG_ANS_NOTIFYCSSTART pMsg;

	pMsg.h.set((LPBYTE)&pMsg,0xB2,0x17,sizeof(pMsg));
	pMsg.btStartState = btStartState;

	for(int i = g_ConfigRead.server.GetObjectStartUserIndex(); i < g_ConfigRead.server.GetObjectMax(); i++)
	{
		if(gObj[i].Connected == PLAYER_PLAYING && gObj[i].Type == OBJ_USER)
		{
			IOCP.DataSend(i,(LPBYTE)&pMsg,pMsg.h.size);
		}
	}
}

void CCastleSiege::NotifyAllUserCsProgState(BYTE btProgState, char* lpszGuildName)
{
	PMSG_ANS_NOTIFYCSPROGRESS pMsg;
	
	pMsg.h.set((LPBYTE)&pMsg, 0xB2, 0x18, sizeof(pMsg));

	pMsg.btCastleSiegeState = btProgState;

	memset(&pMsg.szGuildName, 0, sizeof(pMsg.szGuildName));
	memcpy(&pMsg.szGuildName, lpszGuildName, sizeof(pMsg.szGuildName));

	for( int i = g_ConfigRead.server.GetObjectStartUserIndex(); i < g_ConfigRead.server.GetObjectMax(); i++ )
	{
		if( (gObj[i].Connected == PLAYER_PLAYING) && (gObj[i].Type == OBJ_USER) )
		{
			if( gObj[i].MapNumber == MAP_INDEX_CASTLESIEGE )
			{
				IOCP.DataSend(i, (LPBYTE)&pMsg, pMsg.h.size);
			}
		}
	}
}

void CCastleSiege::ClearCastleTowerBarrier()
{
	for( int iIDX = 0; iIDX < g_ConfigRead.server.GetObjectMaxMonster(); iIDX++ )
	{
		if( ::gObjIsConnected(iIDX) == FALSE )
		{
			continue;
		}

		if( gObj[iIDX].Class == 215 )
		{
			gObjDel(iIDX);
		}
	}
}

void CCastleSiege::SetAllCastleGateState(BOOL bOpenType)
{
	EnterCriticalSection(&this->m_critNpcData);

	std::vector<_CS_NPC_DATA>::iterator it = this->m_vtNpcData.begin();

	BOOL bExist = FALSE;

	while(it != m_vtNpcData.end())
	{
		_CS_NPC_DATA & pNpcData = _CS_NPC_DATA(*it);

		if(pNpcData.m_iNPC_NUM == 277 && gObjIsConnected(pNpcData.m_iNPC_OBJINDEX) && gObj[pNpcData.m_iNPC_OBJINDEX].Class == 277)
		{
			switch(bOpenType)
			{
			case 0:
				SetGateBlockState(pNpcData.m_iNPC_SX,pNpcData.m_iNPC_SY,0);
				gObjRemoveBuffEffect(&gObj[pNpcData.m_iNPC_OBJINDEX], BUFFTYPE_CASTLE_DOOR_STATE);
				gObj[pNpcData.m_iNPC_OBJINDEX].m_btCsGateOpen = 0;
				break;
			case 1:
				SetGateBlockState(pNpcData.m_iNPC_SX,pNpcData.m_iNPC_SY,1);
				gObjAddBuffEffect(&gObj[pNpcData.m_iNPC_OBJINDEX], BUFFTYPE_CASTLE_DOOR_STATE, 0, 0, 0, 0, -10);
				gObj[pNpcData.m_iNPC_OBJINDEX].m_btCsGateOpen = 1;
				break;
			}
		}

		it++;
	}

	LeaveCriticalSection(&m_critNpcData);
}

void CCastleSiege::AddMiniMapDataReqUser(int iIndex)
{
	EnterCriticalSection(&this->m_critCsMiniMap);
		
	if( std::find(this->m_vtMiniMapReqUser.begin(), this->m_vtMiniMapReqUser.end(), iIndex) == this->m_vtMiniMapReqUser.end() )
	{
		this->m_vtMiniMapReqUser.push_back(iIndex);
	}

	LeaveCriticalSection(&this->m_critCsMiniMap);
}

void CCastleSiege::DelMiniMapDataReqUser(int iIndex)
{
	EnterCriticalSection(&this->m_critCsMiniMap);
	
	std::vector<int>::iterator it;
	
	if( ( it = std::find(this->m_vtMiniMapReqUser.begin(), this->m_vtMiniMapReqUser.end(), iIndex)) != this->m_vtMiniMapReqUser.end() )
	{
		this->m_vtMiniMapReqUser.erase(it);
	}

	LeaveCriticalSection(&this->m_critCsMiniMap);
}

void CCastleSiege::OperateMiniMapWork()
{
	if( this->m_bCsBasicGuildInfoLoadOK == FALSE )
	{
		return;
	}

	std::map<int, _CS_MINIMAP_DATA> mapMiniMapData;

	BYTE cBUFFER1[2264] = {0};

	PMSG_ANS_SENDMINIMAPDATA* lpMsgSend1 = (PMSG_ANS_SENDMINIMAPDATA*)cBUFFER1;
	PMSG_SENDMINIMAPDATA* lpMsgSendBody1 = (PMSG_SENDMINIMAPDATA*)&cBUFFER1[sizeof(PMSG_ANS_SENDMINIMAPDATA)];

	BYTE cBUFFER2[714] = {0};

	PMSG_ANS_SENDNPCMINIMAPDATA* lpMsgSend2 = (PMSG_ANS_SENDNPCMINIMAPDATA*)cBUFFER2;
	
	PMSG_SENDNPCMINIMAPDATA* lpMsgSendBody2 = (PMSG_SENDNPCMINIMAPDATA*)&cBUFFER2[sizeof(PMSG_ANS_SENDNPCMINIMAPDATA)];

	EnterCriticalSection(&this->m_critCsBasicGuildInfo);

	for(std::map<string,_CS_TOTAL_GUILD_DATA>::iterator it = this->m_mapCsBasicGuildInfo.begin();  it != this->m_mapCsBasicGuildInfo.end(); it++)
	{
		_CS_TOTAL_GUILD_DATA* GuildData = &it->second;
		_CS_MINIMAP_DATA MiniMapData;

		memcpy(MiniMapData.m_szGuildName, &it->first[0], MAX_GUILD_LEN);

		mapMiniMapData.insert( pair<int,_CS_MINIMAP_DATA>(GuildData->m_iCsGuildID, MiniMapData) );
	}

	LeaveCriticalSection(&this->m_critCsBasicGuildInfo);

	if( mapMiniMapData.empty() )
	{
		return;
	}

	EnterCriticalSection(&this->m_critNpcData);

	lpMsgSend2->iCount = 0;

	if( this->m_vtNpcData.empty() == false )
	{
		for( std::vector<_CS_NPC_DATA>::iterator it = this->m_vtNpcData.begin(); it != this->m_vtNpcData.end(); it++ )
		{
			if( lpMsgSend2->iCount >= 150 )
			{
				break;
			}

			_CS_NPC_DATA & NpcData = _CS_NPC_DATA(*it);

			if( (NpcData.m_iNPC_NUM == 277 || NpcData.m_iNPC_NUM == 283) && NpcData.m_iNPC_OBJINDEX != OBJ_EMPTY )
			{
				if( !gObjIsConnected(NpcData.m_iNPC_OBJINDEX) || gObj[NpcData.m_iNPC_OBJINDEX].m_iCsNpcExistVal != NpcData.m_iCsNpcExistVal || (gObj[NpcData.m_iNPC_OBJINDEX].Class != 277 && gObj[NpcData.m_iNPC_OBJINDEX].Class != 283) )
				{
					BYTE btNpcType = 0;

					switch( NpcData.m_iNPC_NUM )
					{
					case 277:
						btNpcType = 0;
						break;
					case 283:
						btNpcType = 1;
						break;
					default:
						continue;
						break;
					}

					lpMsgSendBody2[lpMsgSend2->iCount].btNpcType = btNpcType;
					lpMsgSendBody2[lpMsgSend2->iCount].btX = gObj[NpcData.m_iNPC_OBJINDEX].X;
					lpMsgSendBody2[lpMsgSend2->iCount].btY = gObj[NpcData.m_iNPC_OBJINDEX].Y;

					lpMsgSend2->iCount++;
				}
			}
		}
	}

	lpMsgSend2->h.set(cBUFFER2, 0xBB, (lpMsgSend2->iCount * sizeof(PMSG_SENDNPCMINIMAPDATA)) + sizeof(PMSG_ANS_SENDNPCMINIMAPDATA));

	LeaveCriticalSection(&this->m_critNpcData);

	EnterCriticalSection(&this->m_critCsMiniMap);

	this->m_mapMiniMapData.clear();

	std::map<int, _CS_MINIMAP_DATA>::iterator it2( mapMiniMapData.begin() );
	
	for(; it2 != mapMiniMapData.end(); it2++ )
	{
		this->m_mapMiniMapData.insert( pair<int, _CS_MINIMAP_DATA>(it2->first, it2->second) );
	}

	for( int iIDX = g_ConfigRead.server.GetObjectStartUserIndex(); iIDX < g_ConfigRead.server.GetObjectMax(); iIDX++ )
	{
		if( gObjIsConnected(iIDX) == FALSE )
		{
			continue;
		}

		if( gObj[iIDX].MapNumber != MAP_INDEX_CASTLESIEGE )
		{
			continue;
		}

		if( gObj[iIDX].m_btCsJoinSide == 0 )
		{
			continue;
		}

		std::map<int, _CS_MINIMAP_DATA>::iterator it3(this->m_mapMiniMapData.find(gObj[iIDX].m_btCsJoinSide));

		if( it3 != this->m_mapMiniMapData.end() )
		{
			if( it3->second.m_iMiniMapPointCount >= 1000 )
			{
				continue;
			}
		
			if( gObj[iIDX].m_PlayerData->GuildStatus == GUILD_MASTER && strcmp(gObj[iIDX].m_PlayerData->GuildName, "") != 0 && strcmp(gObj[iIDX].m_PlayerData->GuildName, it3->second.m_szGuildName) == 0)
			{
				it3->second.m_iGuildMasterIndex = iIDX;
				continue;
			}

			it3->second.m_stMiniMapPoint[it3->second.m_iMiniMapPointCount].btX = gObj[iIDX].X;
			it3->second.m_stMiniMapPoint[it3->second.m_iMiniMapPointCount].btY = gObj[iIDX].Y;
			it3->second.m_iMiniMapPointCount++;
		}
	}

	std::map<int, _CS_MINIMAP_DATA>::iterator it4(this->m_mapMiniMapData.begin());

	for( ; it4 != this->m_mapMiniMapData.end(); it4++ )
	{
		lpMsgSend1->iCount = 0;

		if( gObjIsConnected(it4->second.m_iGuildMasterIndex) == FALSE )
		{
			continue;
		}

		lpMsgSend1->iCount = it4->second.m_iMiniMapPointCount;

		if( lpMsgSend1->iCount > 1000 )
		{
			lpMsgSend1->iCount = 1000;
		}

		memcpy(lpMsgSendBody1, it4->second.m_stMiniMapPoint, (lpMsgSend1->iCount * sizeof(PMSG_SENDMINIMAPDATA)));

		lpMsgSend1->h.set(cBUFFER1, 0xB6, (lpMsgSend1->iCount * sizeof(PMSG_SENDMINIMAPDATA)) + sizeof(PMSG_ANS_SENDMINIMAPDATA));

		IOCP.DataSend(it4->second.m_iGuildMasterIndex, cBUFFER1, (lpMsgSend1->iCount * sizeof(PMSG_SENDMINIMAPDATA)) + sizeof(PMSG_ANS_SENDMINIMAPDATA));
		IOCP.DataSend(it4->second.m_iGuildMasterIndex, cBUFFER2, (lpMsgSend2->iCount * sizeof(PMSG_SENDNPCMINIMAPDATA)) + sizeof(PMSG_ANS_SENDNPCMINIMAPDATA));

	}

	LeaveCriticalSection(&this->m_critCsMiniMap);
}

void CCastleSiege::NotifyCrownState(BYTE btState)
{
	if( gObjIsConnected(m_iCrownIndex) == FALSE )
	{
		g_Log.AddC(TColor::Red,  "[CastleSiege] CCastleSiege::NotifyCrownState() - m_iCrownIndex isn't Valid");
		return;
	}

	if( gObj[m_iCrownIndex].Class != 216 )
	{
		g_Log.AddC(TColor::Red,  "[CastleSiege] CCastleSiege::NotifyCrownState() - m_iCrownIndex isn't Valid");
		return;
	}

	LPOBJ lpObj = &gObj[m_iCrownIndex];

	if(btState == 0)//Season 4.5 addon
	{
		gObjAddBuffEffect(&gObj[m_iCrownIndex], BUFFTYPE_CASTLE_CROWN_STATE, 0, 0, 0, 0, -10);
	}
	else
	{
		gObjRemoveBuffEffect(&gObj[m_iCrownIndex], BUFFTYPE_CASTLE_CROWN_STATE);
	}

	if( lpObj->VPCount2 < 1 )
	{
		return;
	}

	PMSG_ANS_NOTIFYCROWNSTATE pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0xB2, 0x16, sizeof(pMsg));
	pMsg.btCrownState = btState;

	int tObjNum = OBJ_EMPTY;

	for( int i = 0; i < MAX_VIEWPORT; i++ )
	{
		tObjNum = lpObj->VpPlayer2[i].number;

		if( tObjNum >= 0 )
		{
			if( gObj[tObjNum].Type == OBJ_USER && gObj[tObjNum].Live )
			{
				IOCP.DataSend(tObjNum, (LPBYTE)&pMsg, pMsg.h.size);
			}
		}
	}

	g_Log.Add("[CastleSiege] Crown State Changed (%d)", btState);
}

void CCastleSiege::NotifyCrownSwitchInfo(int iCrownSwitchIndex)
{
	if( gObjIsConnected(iCrownSwitchIndex) == FALSE )
	{
		g_Log.AddC(TColor::Red,  "[CastleSiege] CCastleSiege::NotifyCrownSwitchInfo() - iCrownSwitchIndex is Invalid");
		return;
	}

	if( (gObj[iCrownSwitchIndex].Class != 217) && (gObj[iCrownSwitchIndex].Class != 218) )
	{
		g_Log.AddC(TColor::Red,  "[CastleSiege] CCastleSiege::NotifyCrownSwitchInfo() - iCrownSwitchIndex is Invalid");
		return;
	}

	LPOBJ lpObj = &gObj[iCrownSwitchIndex];

	if( lpObj->VPCount2 < 1 )
	{
		return;
	}

	PMSG_ANS_NOTIFY_CROWNSWITCH_INFO pMsg;
	pMsg.h.set((LPBYTE)&pMsg, 0xB2, 0x20, sizeof(pMsg));

	pMsg.btIndex1 = SET_NUMBERH(LOWORD(iCrownSwitchIndex));
	pMsg.btIndex2 = SET_NUMBERL(LOWORD(iCrownSwitchIndex));

	pMsg.btSwitchState = 0;
	pMsg.btJoinSide = 0;

	memset(&pMsg.szGuildName, 0, MAX_GUILD_LEN);
	memset(&pMsg.szUserName, 0, MAX_ACCOUNT_LEN);

	int iCrownSwitchUserIndex = g_CastleSiege.GetCrownSwitchUserIndex(lpObj->Class); // wtf!

	if( gObjIsConnected(iCrownSwitchUserIndex) )
	{
		LPOBJ lpSwitchObj = &gObj[iCrownSwitchUserIndex];
		char szMasterGuildName[MAX_GUILD_LEN];

		pMsg.btSwitchState = TRUE;
		pMsg.btJoinSide = lpSwitchObj->m_btCsJoinSide;

		memcpy(&pMsg.szUserName, lpSwitchObj->Name, MAX_ACCOUNT_LEN);
		memcpy(&pMsg.szGuildName, lpSwitchObj->m_PlayerData->GuildName, MAX_GUILD_LEN);
		memset(&szMasterGuildName, 0, MAX_GUILD_LEN);

		if( lpSwitchObj->m_PlayerData->lpGuild )
		{
			if( UnionManager.GetUnionName(lpSwitchObj->m_PlayerData->lpGuild->iGuildUnion, szMasterGuildName) != -1 )
			{
				memcpy(&pMsg.szGuildName, szMasterGuildName, MAX_GUILD_LEN);
			}
		}

		IOCP.DataSend(iCrownSwitchUserIndex, (LPBYTE)&pMsg, pMsg.h.size);
	}
	
	int tObjNum = OBJ_EMPTY;

	for( int i = 0; i < MAX_VIEWPORT; i++ )
	{
		tObjNum = lpObj->VpPlayer2[i].number;

		if( tObjNum >= 0 )
		{
			if( gObj[tObjNum].Type == OBJ_USER && gObj[tObjNum].Live )
			{
				IOCP.DataSend(tObjNum, (LPBYTE)&pMsg, pMsg.h.size);
			}
		}
	}
}

void CCastleSiege::SendMapServerGroupMsg(char * lpszMsg)
{
	GS_GDReqMapSvrMsgMultiCast(this->m_iMapSvrGroup, lpszMsg);
}

void CCastleSiege::SendAllUserAnyData(LPBYTE lpMsg, int iSize)
{
	for( int i = g_ConfigRead.server.GetObjectStartUserIndex(); i < g_ConfigRead.server.GetObjectMax(); i++ )
	{
		if( (gObj[i].Connected == PLAYER_PLAYING) && (gObj[i].Type == OBJ_USER) )
		{
			IOCP.DataSend(i, lpMsg, iSize);
		}
	}
}

void CCastleSiege::SendAllUserAnyMsg(char * lpszMsg, int iType)
{
	PMSG_NOTICE pNotice;

	if( lpszMsg == NULL )
	{
		return;
	}

	switch( iType )
	{
		case 1:
			TNotice::MakeNoticeMsg(&pNotice, 0, lpszMsg);
			TNotice::SetNoticeProperty(&pNotice, 10, _ARGB(255, 255, 200, 80), 1, 0, 20);
			TNotice::SendNoticeToAllUser(&pNotice);
		break;

		case 2:
			{
				TNotice::MakeNoticeMsg(&pNotice, 0, lpszMsg);
				for(int i = g_ConfigRead.server.GetObjectStartUserIndex(); i < g_ConfigRead.server.GetObjectMax(); i++ )
				{
					if( (gObj[i].Connected == PLAYER_PLAYING) && (gObj[i].Type == OBJ_USER) )
					{
						IOCP.DataSend(i, (LPBYTE)&pNotice, pNotice.h.size);
					}
				}
			}
		break;

		default:
			break;
	}
}

void CCastleSiege::SendCsUserAnyData(LPBYTE lpMsg, int iSize)
{
	for( int i = g_ConfigRead.server.GetObjectStartUserIndex(); i < g_ConfigRead.server.GetObjectMax(); i++ )
	{
		if( (gObj[i].Connected == PLAYER_PLAYING) && (gObj[i].Type == OBJ_USER) )
		{
			if( gObj[i].MapNumber == MAP_INDEX_CASTLESIEGE )
			{
				IOCP.DataSend(i, lpMsg, iSize);
			}
		}
	}
}

void CCastleSiege::SendCsUserAnyMsg(char* lpszMsg)
{
	PMSG_NOTICE pNotice;

	if( lpszMsg == NULL )
	{
		return;
	}

	TNotice::MakeNoticeMsg(&pNotice, 0, lpszMsg);

	for( int i = g_ConfigRead.server.GetObjectStartUserIndex(); i < g_ConfigRead.server.GetObjectMax(); i++ )
	{
		if( (gObj[i].Connected == PLAYER_PLAYING) && (gObj[i].Type == OBJ_USER) )
		{
			if( gObj[i].MapNumber == MAP_INDEX_CASTLESIEGE )
			{
				IOCP.DataSend(i, (LPBYTE)&pNotice, pNotice.h.size);
			}
		}
	}
}

void CCastleSiege::OperateGmCommand(int iIndex, int iGmCommand, void* lpParam)
{
	if( gObjIsConnected(iIndex) == FALSE )
	{
		return;
	}

	if( (gObj[iIndex].Authority & 0x20) != 0x20 )
	{
		return;
	}

	if( (gObj[iIndex].GameMaster & GM_EVENT_MODIFY) != GM_EVENT_MODIFY )
	{
		MsgOutput(iIndex, Lang.GetText(0,882));
		return;
	}

	switch( iGmCommand )
	{
		case 0:
			{
				LPSTR szNewOwnerGuild = (char*)lpParam;

				if( szNewOwnerGuild == NULL )
				{
					MsgOutput(iIndex, "[Admin cmd] Invalid guild");
				}
				else
				{
					if( strcmp(szNewOwnerGuild, "") != 0 )
					{
						memset(&this->m_szCastleOwnerGuild, 0, sizeof(this->m_szCastleOwnerGuild));
						memcpy(&this->m_szCastleOwnerGuild, szNewOwnerGuild, 8);
						this->m_btIsCastleOccupied = 1;
						MsgOutput(iIndex, "[Admin cmd] Set new Owner guild: %s", this->m_szCastleOwnerGuild);
					}
					else
					{
						strcpy(this->m_szCastleOwnerGuild, "");
						this->m_btIsCastleOccupied = 0;
						MsgOutput(iIndex, "[Admin cmd] Castle is not occupied now");
					}
				}
			}
			break;
		case 1:
			{
				GetLocalTime(&this->m_tmStartDate);

				this->m_tmStartDate.wHour = 0;
				this->m_tmStartDate.wMinute = 0;
				this->m_tmStartDate.wSecond = 0;

				this->m_tmEndDate = this->m_tmStartDate;
				this->GetNextDay(&this->m_tmEndDate, this->m_iCastleSiegeCycle, 0, 0, 0);
				
				this->m_bFixCastleCycleStartDate = FALSE;
				this->m_btIsSiegeEnded = 0;

				GS_GDReqResetRegSiegeInfo(this->m_iMapSvrGroup);

				if( this->CheckSync() == FALSE )
				{
					g_Log.AddC(TColor::Red,  "[CastleSiege] CCastleSiege::Init() - CheckSync() == FALSE");
					g_Log.MsgBox("[CastleSiege] CCastleSiege::Init() - CheckSync() == FALSE");
					MsgOutput(iIndex, "[Error]: Sync failed!");
					return;
				}

				this->m_bDoRun = TRUE;
				this->m_dwNPC_DBSAVE_TICK_COUNT = GetTickCount();
				this->m_dwCALC_LEFTSIEGE_TICK_COUNT = 0;
				this->m_dwEVENT_MSG_TICK_COUNT = 0;
				this->m_dwCHECK_GATE_ALIVE_COUNT = 0;
				MsgOutput(iIndex, "[Admin cmd] CurrentState: REGSIEGE");
			}
			break;
		case 2:
			{
				this->SetState(CASTLESIEGE_STATE_REGMARK, TRUE);
				MsgOutput(iIndex, "[Admin cmd] CurrentState: REGMARK");
			}
			break;
		case 3:
			{
				this->m_btIsSiegeGuildList = FALSE;
				this->SetState(CASTLESIEGE_STATE_NOTIFY, TRUE);
				MsgOutput(iIndex, "[Admin cmd] CurrentState: NOTIFY");
			}
			break;
		case 4:
			{
				this->SetState(CASTLESIEGE_STATE_READYSIEGE, TRUE);
				MsgOutput(iIndex, "[Admin cmd] CurrentState; READYSIEGE");	
			}
			break;

		case 5:
			{
				if( this->m_iCastleSiegeState == CASTLESIEGE_STATE_STARTSIEGE )
				{
					this->m_iCS_REMAIN_MSEC = 0;
					MsgOutput(iIndex, "[Admin cmd] Siege ended");
				}
				else
				{
					MsgOutput(iIndex, "[Admin cmd] Siege is not started!");
				}
			}
			break;
		case 6:
			{
				if( strcmp(this->m_szCastleOwnerGuild, "") == 0 )
				{
					MsgOutput(iIndex, "[Admin cmd] Castle is not occupied");
				}
				else
				{
					MsgOutput(iIndex, "[Admin cmd] Current castle owner: %s", this->m_szCastleOwnerGuild);
				}
			}
			break;
		case 7:
			{
				if( this->m_bDoRun == FALSE )
				{
					MsgOutput(iIndex, "[Admin cmd] Siege is NOTRUNNING");
				}
				else
				{
					switch( this->m_iCastleSiegeState )
					{
					case CASTLESIEGE_STATE_NONE:
						MsgOutput(iIndex, "[Admin cmd] State: NONE");
						break;
					case CASTLESIEGE_STATE_IDLE_1:
						MsgOutput(iIndex, "[Admin cmd] State: IDLE_1");
						break;
					case CASTLESIEGE_STATE_REGSIEGE:
						MsgOutput(iIndex, "[Admin cmd] State: REGSIEGE (Time: %d/%d/%d)",
							((this->m_iCS_REMAIN_MSEC / 1000) % 3600), 
							((this->m_iCS_REMAIN_MSEC / 1000) % 3600) / 60, 
							((this->m_iCS_REMAIN_MSEC / 1000) % 3600) / 60);
						break;
					case CASTLESIEGE_STATE_IDLE_2:
						MsgOutput(iIndex, "[Admin cmd] State: IDLE_2 (Time: %d/%d/%d)",
							((this->m_iCS_REMAIN_MSEC / 1000) % 3600), 
							((this->m_iCS_REMAIN_MSEC / 1000) % 3600) / 60,	
							((this->m_iCS_REMAIN_MSEC / 1000) % 3600) / 60);
						break;
					case CASTLESIEGE_STATE_REGMARK:
						MsgOutput(iIndex, "[Admin cmd] State: REGMARK (Time: %d/%d/%d)",
							((this->m_iCS_REMAIN_MSEC / 1000) % 3600), 
							((this->m_iCS_REMAIN_MSEC / 1000) % 3600) / 60,	
							((this->m_iCS_REMAIN_MSEC / 1000) % 3600) / 60);
						break;
					case CASTLESIEGE_STATE_IDLE_3:
						MsgOutput(iIndex, "[Admin cmd] State: IDLE_3 (Time: %d/%d/%d)",
							((this->m_iCS_REMAIN_MSEC / 1000) % 3600), 
							((this->m_iCS_REMAIN_MSEC / 1000) % 3600) / 60,	
							((this->m_iCS_REMAIN_MSEC / 1000) % 3600) / 60);
						break;
					case CASTLESIEGE_STATE_NOTIFY:
						MsgOutput(iIndex, "[Admin cmd] State: NOTIFY (Time: %d/%d/%d)",
							((this->m_iCS_REMAIN_MSEC / 1000) % 3600), 
							((this->m_iCS_REMAIN_MSEC / 1000) % 3600) / 60,	
							((this->m_iCS_REMAIN_MSEC / 1000) % 3600) / 60);
						break;
					case CASTLESIEGE_STATE_READYSIEGE:
						MsgOutput(iIndex, "[Admin cmd] State: READYSIEGE (Time: %d/%d/%d)",
							((this->m_iCS_REMAIN_MSEC / 1000) % 3600), 
							((this->m_iCS_REMAIN_MSEC / 1000) % 3600) / 60,	
							((this->m_iCS_REMAIN_MSEC / 1000) % 3600) / 60);
						break;
					case CASTLESIEGE_STATE_STARTSIEGE:
						MsgOutput(iIndex, "[Admin cmd] State: STARTSIEGE (Time: %d/%d/%d)",
							((this->m_iCS_REMAIN_MSEC / 1000) % 3600), 
							((this->m_iCS_REMAIN_MSEC / 1000) % 3600) / 60,	
							((this->m_iCS_REMAIN_MSEC / 1000) % 3600) / 60);
						break;
					case CASTLESIEGE_STATE_ENDSIEGE:
						MsgOutput(iIndex, "[Admin cmd] State: ENDSIEGE (Time: %d/%d/%d)",
							((this->m_iCS_REMAIN_MSEC / 1000) % 3600), 
							((this->m_iCS_REMAIN_MSEC / 1000) % 3600) / 60,	
							((this->m_iCS_REMAIN_MSEC / 1000) % 3600) / 60);
						break;
					case CASTLESIEGE_STATE_ENDCYCLE:
						MsgOutput(iIndex, "[Admin cmd] State: ENDCYCLE");
						break;
					default:
						MsgOutput(iIndex, "[Admin cmd] State: ENDCYCLE");
						break;
					}
				}
			}
			break;
				case 8:
			{
				LPSTR szGuildName = (LPSTR)lpParam;

				if( this->m_iCastleSiegeState != CASTLESIEGE_STATE_STARTSIEGE )
				{
					MsgOutput(iIndex, "[Admin cmd] Siege is not started");
				}
				else if( szGuildName == NULL )
				{
					MsgOutput(iIndex, "[Admin cmd] Invalid guild");
				}
				else if( strcmp(szGuildName, "") == 0 )
				{
					MsgOutput(iIndex, "[Admin cmd] You have entered empty name of guild");
				}
				else
				{
					BYTE btCsJoinSide = 0;
					bool bCsGuildInvolved = false;
					int iResult = this->GetCsJoinSide(szGuildName, &btCsJoinSide, &bCsGuildInvolved);

					if( iResult == 1 )
					{
						if( btCsJoinSide > 1 && bCsGuildInvolved == 1 )
						{
							g_Log.Add("[CastleSiege] Castle Winner Has been Changed - GM (GUILD:%s)", szGuildName);

							this->NotifyCrownState(2);

							memset(&this->m_szMiddleWinnerGuild, 0, sizeof(this->m_szMiddleWinnerGuild));
							memcpy(&this->m_szMiddleWinnerGuild, szGuildName, 8);

							this->ClearNonDbNPC_MidWin();
							this->ChangeWinnerGuild(btCsJoinSide);
							this->NotifyAllUserCsProgState(CASTLESIEGE_STATE_IDLE_1, szGuildName);
							this->ResetCrownAccessTickCount();
							this->ResetCrownUserIndex();
							this->SetCrownAccessUserX(0);
							this->SetCrownAccessUserY(0);
							this->ResetCrownSwitchUserIndex(217);
							this->ResetCrownSwitchUserIndex(218);

							this->m_bRegCrownAvailable = FALSE;
							this->ReSpawnEnemyUser(FALSE);

							GSProtocol.GCAnsCsLeftTimeAlarm(((this->m_iCS_REMAIN_MSEC / 1000) % 3600), ((this->m_iCS_REMAIN_MSEC / 1000) % 3600) / 60);

							this->m_dwCS_LEFTTIME_TICK_COUNT = GetTickCount();

						}
						MsgOutput(iIndex, "[Admin cmd] Current Defense guild changed!");
					}
				}
			}
			break;
		case 9:
			{
				this->SetState(CASTLESIEGE_STATE_STARTSIEGE, 1);
				MsgOutput(iIndex, "[Admin cmd] Castle siege started!");
			}
			break;
		default:
			MsgOutput(iIndex, "[Admin cmd] Unknown Castle Siege command");
			break;
	}
}

void CCastleSiege::CreateDbNPC_INS()
{
	this->ClearDbNPC();

	EnterCriticalSection(&this->m_critNpcData);

	std::vector<_CS_NPC_DATA>::iterator it = this->m_vtNpcData.begin();

	while( it != this->m_vtNpcData.end() )
	{
		_CS_NPC_DATA* pNpcData;
		_CS_NPC_DATA NpcData( *it );
		pNpcData = &NpcData;

		if( pNpcData->m_bNPC_DBSAVE == TRUE )
		{
			it->SetBaseValue();
			it->m_iNPC_LIVE = 1;
			it->m_iNPC_SIDE = 1;
			it->m_iNPC_OBJINDEX = -1;
			it->m_iCS_GATE_LEVER_INDEX = -1;
		}
		it++;
	}

	LeaveCriticalSection(&this->m_critNpcData);

	this->m_bDbNpcCreated = FALSE;
}

void CCastleSiege::SavePcRoomUserList()
{
	//Something related to muonline.co.kr version (could be sort of a pcbang system for siege??)
}

void CCastleSiege::CastleLordMixRun()
{
	SYSTEMTIME CurrTime;
	GetLocalTime(&CurrTime);

	if(CurrTime.wDay <= this->m_tmLordMixTime.wDay)
	{
		if(CurrTime.wMonth <= this->m_tmLordMixTime.wMonth)
		{
			if(CurrTime.wYear <= this->m_tmLordMixTime.wYear)
			{
				return;
			}
		}
	}

	this->m_tmLordMixTime = CurrTime;
	this->m_CurrentLordMixLimit = this->m_CastleLordMixDailyLimit;

	g_Log.Add("[CastleSiege] Reset CastleLordMix Limit:%d", this->m_CurrentLordMixLimit);
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

