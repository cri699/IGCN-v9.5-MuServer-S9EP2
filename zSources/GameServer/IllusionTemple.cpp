//////////////////////////////////////////////////////////////////////
// IllusionTemple.cpp
#include "stdafx.h"
#include "BloodCastle.h"
#include "ChaosCastle.h"
#include "IllusionTemple.h"
#include "GameMain.h"
#include "GameServer.h"
#include "logproc.h"
#include "..\include\readscript.h"
#include "DSProtocol.h"
#include "Protocol.h"
#include "TNotice.h"
#include "..\common\winutil.h"
#include "ObjUseSkill.h"
#include "ChaosBox.h"
#include "CastleSiegeSync.h"
#include "DSProtocol.h"
#include "CrywolfSync.h"
#include "BuffEffect.h"
#include "CashShop.h"
#include "MasterSkillSystem.h"
#include "LogToFile.h"
CIllusionTemple g_IllusionTemple;

CIllusionTemple::CIllusionTemple()
{
}

CIllusionTemple::~CIllusionTemple()
{
}

void CIllusionTemple::Run()
{
	if(this->EventEnabled == true)
	{
		for(int i=0;i<MAX_ILLUSION_TEMPLE_LEVEL;++i)
		{
			switch(this->m_IllusionTempleData[i].State)
			{
			case IT_STATE_CLOSED:
				this->ProcState_Closed(i);
				break;
			case IT_STATE_PLAYING:
				this->ProcState_Playing(i);
				break;
			case IT_STATE_PLAYEND:
				this->ProcState_Playend(i);
				break;
			case IT_STATE_NONE:
				this->ProcState_None(i);
				break;
			}
		}
	}
}

void CIllusionTemple::Init(bool Enabled)
{
	this->EventEnabled = Enabled;
#if GS_CASTLE == 1
	for(int i=0; i<MAX_ILLUSION_TEMPLE_LEVEL;++i)
	{
		this->SetState(i, IT_STATE_NONE);
	}
	return;
#endif
	for(int i=0; i<MAX_ILLUSION_TEMPLE_LEVEL;++i)
	{
		this->m_IllusionTempleData[i].StatuePosNum[0] = -1;
		this->m_IllusionTempleData[i].StatuePosNum[1] = -1;
		this->m_IllusionTempleData[i].IllusionGuardianPosNum = -1;
		this->m_IllusionTempleData[i].IllusionBoxPosNum = -1;
		this->m_IllusionTempleData[i].AlliesBoxPosNum = -1;
		this->m_IllusionTempleData[i].AlliesGuardianPosNum = -1;
		this->m_IllusionTempleData[i].StatueIndex[0] = -1;
		this->m_IllusionTempleData[i].StatueIndex[1] = -1;
		this->m_IllusionTempleData[i].IllusionGuardianIndex = -1;
		this->m_IllusionTempleData[i].IllusionBoxIndex = -1;
		this->m_IllusionTempleData[i].AlliesBoxIndex = -1;
		this->m_IllusionTempleData[i].AlliesGuardianIndex = -1;
		for(int j=0;j<MAX_ILLUSION_TEMPLE_TEAMS;++j)
		{
			for(int k=0;k<MAX_ILLUSION_TEMPLE_TEAM_USER;++k)
			{
				this->m_IllusionTempleData[i].Teams[j].TeamMate[k].Clear();
			}
			this->m_IllusionTempleData[i].Teams[j].Clear();
		}
		for(int n=0;n<MAX_ILLUSION_TEMPLE_USER;++n)
		{
			this->m_IllusionTempleData[i].WaitingPlayers[n].Clear();
		}
	}
	for(int i=0; i< gMSetBase.m_Count; ++i)
	{
		if(IT_MAP_RANGE(gMSetBase.m_Mp[i].m_MapNumber) == TRUE)
		{
			WORD btMonsterType = gMSetBase.m_Mp[i].m_Type;
			BYTE btMapNumber = gMSetBase.m_Mp[i].m_MapNumber;
			BYTE btIllusionTempleIndex = gMSetBase.m_Mp[i].m_MapNumber - MAP_INDEX_ILLUSIONTEMPLE1;
			if(btMonsterType == 380)
			{
				if(this->m_IllusionTempleData[btIllusionTempleIndex].StatuePosNum[0] == -1)
				{
					this->m_IllusionTempleData[btIllusionTempleIndex].StatuePosNum[0] = i;
				}
				else if(this->m_IllusionTempleData[btIllusionTempleIndex].StatuePosNum[1] == -1)
				{
					this->m_IllusionTempleData[btIllusionTempleIndex].StatuePosNum[1] = i;
				}
			}
			else if(btMonsterType == 381)
			{
				this->m_IllusionTempleData[btIllusionTempleIndex].AlliesGuardianPosNum = i;
			}
			else if(btMonsterType == 382)
			{
				this->m_IllusionTempleData[btIllusionTempleIndex].IllusionGuardianPosNum = i;
			}
			else if(btMonsterType == 383)
			{
				this->m_IllusionTempleData[btIllusionTempleIndex].AlliesBoxPosNum = i;
			}
			else if(btMonsterType == 384)
			{
				this->m_IllusionTempleData[btIllusionTempleIndex].IllusionBoxPosNum = i;
			}

		}
	}
	for(int i=0; i<MAX_ILLUSION_TEMPLE_LEVEL;++i)
	{
		this->SetState(i, IT_STATE_CLOSED);
	}
}

void CIllusionTemple::Load(LPSTR file)
{
	SMDFile = fopen(file, "r");
	if(SMDFile == NULL)
	{
		MsgBox("[Illusion Temple]: File not found: [%s]", file);
		return;
	}
	int Token;
	int type;
	this->v_IllusionStartTime.erase(this->v_IllusionStartTime.begin(), this->v_IllusionStartTime.end());

	while(true)
	{
		Token = GetToken();
		if(Token == 2)
			break;
		type = TokenNumber;

		while(true)
		{
			if(type == 0)
			{
				Token = GetToken();
				if(strcmp("end", TokenString) == 0)
					break;

				this->OpenTime = TokenNumber;
					
				Token = GetToken();
				this->ReadyTime = TokenNumber;

				Token = GetToken();
				this->PlayTime = TokenNumber;

				Token = GetToken();
				this->RestTime = TokenNumber;
			}
			else if(type == 1)
			{
				Token = GetToken();
				if(strcmp("end", TokenString) == 0)
					break;

				ILLUSION_START_TIME Time;
				Time.Hour = TokenNumber;

				Token = GetToken();
				Time.Minute = TokenNumber;

				this->v_IllusionStartTime.push_back(Time);
			}
		}
	}
	fclose(SMDFile);
	LogAdd("%s file load!", file);
}

void CIllusionTemple::SetState(int IllusionTempleIndex, int State)
{
	if(IT_BATTLEZONE_RANGE(IllusionTempleIndex) == false)
	{
		return;
	}
	if(State < IT_STATE_NONE || State > IT_STATE_PLAYEND)
	{
		return;
	}
	this->m_IllusionTempleData[IllusionTempleIndex].State = State;

	switch (this->m_IllusionTempleData[IllusionTempleIndex].State)
	{
	case IT_STATE_CLOSED:
		{
			this->SetState_Closed(IllusionTempleIndex);
			break;
		}
	case IT_STATE_PLAYING:
		{
			this->SetState_Playing(IllusionTempleIndex);
			break;
		}
	case IT_STATE_PLAYEND:
		{
			this->SetState_PlayEnd(IllusionTempleIndex);
			break;
		}
	case IT_STATE_NONE:
		{
			this->SetState_None(IllusionTempleIndex);
			break;
		}
	}
}

void CIllusionTemple::ProcState_Closed(int IllusionTempleIndex)
{
	int TickCount = GetTickCount() - this->m_IllusionTempleData[IllusionTempleIndex].Tick;
	if(TickCount >= 1000) // 1sekunda timera
	{
		this->PlayerWaitingProcess(IllusionTempleIndex);
		this->m_IllusionTempleData[IllusionTempleIndex].RemainMSEC -= TickCount;
		this->m_IllusionTempleData[IllusionTempleIndex].Tick = GetTickCount();

		if(gIllusionTempleEvent)
		{
			if(this->m_IllusionTempleData[IllusionTempleIndex].RemainMSEC <= (this->OpenTime * 60 * 1000) && this->m_IllusionTempleData[IllusionTempleIndex].Open == false)
			{
				this->m_IllusionTempleData[IllusionTempleIndex].Open = true;
				this->m_IllusionTempleData[IllusionTempleIndex].CanParty = true;
			}
			if(this->m_IllusionTempleData[IllusionTempleIndex].RemainMSEC <= (this->OpenTime * 60 * 1000) && this->m_IllusionTempleData[IllusionTempleIndex].RemainMSEC > 0 && (this->m_IllusionTempleData[IllusionTempleIndex].RemainMSEC/60000) !=this->m_IllusionTempleData[IllusionTempleIndex].Notify)
			{
				this->m_IllusionTempleData[IllusionTempleIndex].Notify = this->m_IllusionTempleData[IllusionTempleIndex].RemainMSEC / 60000;
				if(IllusionTempleIndex == 0)
				{
					PMSG_NOTICE pNotice;
					TNotice::MakeNoticeMsgEx(&pNotice, 0, Lang.GetText(0,288), this->m_IllusionTempleData[IllusionTempleIndex].Notify+1);
					this->SendAllUserAnyMsg((LPBYTE)&pNotice, pNotice.h.size);
				}
			}
			if(this->m_IllusionTempleData[IllusionTempleIndex].RemainMSEC <= 30000 && this->m_IllusionTempleData[IllusionTempleIndex].RemainMSEC > 0 && this->m_IllusionTempleData[IllusionTempleIndex].BeforeEnter == false)
			{
				this->m_IllusionTempleData[IllusionTempleIndex].BeforeEnter = true;
				if(IllusionTempleIndex == 0)
				{
					PMSG_SET_DEVILSQUARE pMsg;
					PHeadSetB((LPBYTE)&pMsg, 0x92, sizeof(pMsg));
					pMsg.Type = 14;
					for(int i=OBJ_STARTUSERINDEX; i<OBJMAX;++i)
					{
						if(gObj[i].Connected == PLAYER_PLAYING)
						{
							if(!CC_MAP_RANGE(gObj[i].MapNumber))
							{
								if(!BC_MAP_RANGE(gObj[i].MapNumber))
								{
									if(!IT_MAP_RANGE(gObj[i].MapNumber))
									{
										IOCP.DataSend(i, (LPBYTE)&pMsg, pMsg.h.size);
									}
								}
							}
						}
					}
				}
			}
		}

		for(int i=0;i<MAX_ILLUSION_TEMPLE_USER;++i)
		{
			if(this->m_IllusionTempleData[IllusionTempleIndex].WaitingPlayers[i].UserIndex != -1)
			{
				if(gObj[this->m_IllusionTempleData[IllusionTempleIndex].WaitingPlayers[i].UserIndex].MapNumber != IllusionTempleIndex+MAP_INDEX_ILLUSIONTEMPLE1)
				{
					this->DeletePlayer(IllusionTempleIndex, this->m_IllusionTempleData[IllusionTempleIndex].WaitingPlayers[i].UserIndex, true);
				}
			}
		}

		if(this->m_IllusionTempleData[IllusionTempleIndex].RemainMSEC <= 0 && this->m_IllusionTempleData[IllusionTempleIndex].BeforeSemiPlay == false)
		{
			if(gIllusionTempleEvent)
			{
				this->m_IllusionTempleData[IllusionTempleIndex].Open = false;
				this->m_IllusionTempleData[IllusionTempleIndex].CanParty = false;
				this->m_IllusionTempleData[IllusionTempleIndex].BeforeSemiPlay = true;
				this->m_IllusionTempleData[IllusionTempleIndex].RemainMSEC = 5000;
				this->SetTeams(IllusionTempleIndex);
				this->DeleteInvitation(IllusionTempleIndex);
				if(IllusionTempleIndex == 0)
				{
					PMSG_NOTICE pNotice;
					TNotice::MakeNoticeMsg(&pNotice, 0, Lang.GetText(0,289));
					this->SendAllUserAnyMsg((LPBYTE)&pNotice, pNotice.h.size);
				}
			}
			else
			{
				this->SetState(IllusionTempleIndex, IT_STATE_CLOSED);
			}
		}
	}
	if(gIllusionTempleEvent && this->m_IllusionTempleData[IllusionTempleIndex].RemainMSEC <= 0 && this->m_IllusionTempleData[IllusionTempleIndex].BeforeSemiPlay == true)
	{
		this->SetState(IllusionTempleIndex, IT_STATE_PLAYING);
	}
}

void CIllusionTemple::CheckSync(int IllusionTempleIndex)
{
	if(IT_BATTLEZONE_RANGE(IllusionTempleIndex) == FALSE)
	{
		return;
	}
	if(this->v_IllusionStartTime.empty() == true)
	{
		LogAddC(2, "[Illusion Temple] (%d) Not found Event times, disabling event", IllusionTempleIndex+1);
		this->SetState(IllusionTempleIndex, IT_STATE_NONE);
		return;
	}

	SYSTEMTIME SysTime;
	GetLocalTime(&SysTime);
	int Hour = 24;
	int Minute = 60;
	int Changed = false;
	std::vector<ILLUSION_START_TIME>::iterator Iterator;
	for(Iterator = this->v_IllusionStartTime.begin(); Iterator != this->v_IllusionStartTime.end(); ++Iterator)
	{
		ILLUSION_START_TIME & Timer = *Iterator;

		if( (SysTime.wHour * 60 + SysTime.wMinute) < (Timer.Hour * 60 + Timer.Minute) )
		{
			if(( ( Hour * 60 + Minute ) > (Timer.Hour * 60 + Timer.Minute) ))
			{
				Hour = Timer.Hour;
				Minute = Timer.Minute;
				Changed = true;
			}
		}
	}
		
	if ( Changed == 0 )
	{
		Iterator = this->v_IllusionStartTime.begin();
		Hour = 24;
		Minute = 60;

		for ( ; Iterator != this->v_IllusionStartTime.end() ; ++Iterator )
		{
			ILLUSION_START_TIME & Timer = *Iterator;

			if ( ( Hour * 60 + Minute ) > ( Timer.Hour * 60 + Timer.Minute ) )
			{
				Changed = 2;
				Hour = Timer.Hour;
				Minute = Timer.Minute;
			}
		}
	}
	switch (Changed)
	{
	case 1:
		this->m_IllusionTempleData[IllusionTempleIndex].RemainMSEC = (((Hour * 60 * 60) + (Minute * 60)) - ((SysTime.wHour * 60 * 60) + (SysTime.wMinute * 60) + (SysTime.wSecond))) * 1000;
		break;
	case 2:
		this->m_IllusionTempleData[IllusionTempleIndex].RemainMSEC = ((((Hour+24) * 60 * 60) + (Minute * 60)) - ((SysTime.wHour * 60 * 60) + (SysTime.wMinute * 60) + (SysTime.wSecond))) * 1000;
		break;
	default:
		LogAddC(2, "[Illusion Temple] (%d) Not found Time Sync, disabling event", IllusionTempleIndex+1);
		this->SetState(IllusionTempleIndex, IT_STATE_NONE);
		return;
	}
	this->m_IllusionTempleData[IllusionTempleIndex].Tick = GetTickCount();
	LogAddTD("[Illusion Temple] (%d) Sync Start Time. [%d] min remain (START HOUR:%d, MIN:%d)",
		IllusionTempleIndex+1, this->m_IllusionTempleData[IllusionTempleIndex].RemainMSEC / 60000, Hour, Minute);
}

void CIllusionTemple::SetState_Closed(int IllusionTempleIndex)
{
	if(IT_BATTLEZONE_RANGE(IllusionTempleIndex) == false)
	{
		return;
	}
	for(int i=0;i<MAX_ILLUSION_TEMPLE_TEAM_USER;++i)
	{
		if(int aIndex = this->m_IllusionTempleData[IllusionTempleIndex].Teams[0].TeamMate[i].UserIndex != -1)
		{
			if(gObjIsConnected(aIndex))
			{
				gObj[aIndex].PartyNumber = -1; 
				gObj[aIndex].PartyTargetUser = -1;
				gObj[aIndex].m_cIllusionTempleIndex = -1;
				gObj[aIndex].m_cIllusionTempleSubIndex = -1;
				gObj[aIndex].m_cIllusionTempleTeam = -1;
				gObj[aIndex].m_cIllusionTempleTeamIndex = -1;
				GSProtocol.GCPartyDelUserSend(aIndex);
			}
		}
	}
	for(int i=0;i<MAX_ILLUSION_TEMPLE_TEAM_USER;++i)
	{
		if(int aIndex = this->m_IllusionTempleData[IllusionTempleIndex].Teams[1].TeamMate[i].UserIndex != -1)
		{
			if(gObjIsConnected(aIndex))
			{
				gObj[aIndex].PartyNumber = -1; 
				gObj[aIndex].PartyTargetUser = -1;
				gObj[aIndex].m_cIllusionTempleIndex = -1;
				gObj[aIndex].m_cIllusionTempleSubIndex = -1;
				gObj[aIndex].m_cIllusionTempleTeam = -1;
				gObj[aIndex].m_cIllusionTempleTeamIndex = -1;
				GSProtocol.GCPartyDelUserSend(aIndex);
			}
		}
	}
	gParty.Destroy(this->m_IllusionTempleData[IllusionTempleIndex].Teams[0].PartyID);
	gParty.Destroy(this->m_IllusionTempleData[IllusionTempleIndex].Teams[1].PartyID);
	this->m_IllusionTempleData[IllusionTempleIndex].Clear();
	this->m_IllusionTempleData[IllusionTempleIndex].Teams[0].Clear();
	this->m_IllusionTempleData[IllusionTempleIndex].Teams[1].Clear();
	for(int i=0;i<MAX_ILLUSION_TEMPLE_USER;++i)
	{
		this->m_IllusionTempleData[IllusionTempleIndex].WaitingPlayers[i].Clear();
	}
	this->DeleteMonsters(IllusionTempleIndex);
	for(int n=OBJ_STARTUSERINDEX;n<OBJMAX;++n)
	{
		if(gObj[n].MapNumber == (IllusionTempleIndex+MAP_INDEX_ILLUSIONTEMPLE1))
		{
			if(gObj[n].Connected >= PLAYER_LOGGED)
			{
				gObjMoveGate(n, 267);
			}
		}
	}
	this->CheckSync(IllusionTempleIndex);
	LogAddTD("[Illusion Temple] (%d) SetState CLOSED", IllusionTempleIndex+1);
}

void CIllusionTemple::DeleteMonsters(int IllusionTempleIndex)
{
	for(int i=0;i<OBJ_MAXMONSTER;++i)
	{
		if(gObj[i].MapNumber == (IllusionTempleIndex+MAP_INDEX_ILLUSIONTEMPLE1))
		{
			if(this->m_IllusionTempleData[IllusionTempleIndex].State == IT_STATE_PLAYEND)
			{
				if(gObj[i].Class != 381 && gObj[i].Class != 382)
				{
					gObjDel(i);
				}
			}
			else
			{
				gObjDel(i);
			}
		}
	}
}

void CIllusionTemple::SendAllUserAnyMsg(LPBYTE Msg, int Size)
{
	for(int i=OBJ_STARTUSERINDEX;i<OBJMAX;++i)
	{
		if(gObj[i].Connected == PLAYER_PLAYING)
		{
			IOCP.DataSend(i, Msg, Size);
		}
	}
}

int CIllusionTemple::GetUserLevelRoom(int aIndex)
{
	for(int i=0;i<MAX_ILLUSION_TEMPLE_LEVEL;++i)
	{
		if(gObj[aIndex].ChangeUP == 2)
		{
			return 5;
		}

		else if(gObj[aIndex].Level >= g_IllusionLevel[i].MinLevel && gObj[aIndex].Level <= g_IllusionLevel[i].MaxLevel)
		{
			return i;
		}
	}
	return -1;
}

void CIllusionTemple::EnterToWaitingRoom(int aIndex, int IllusionTempleIndex, int ItemSerial, int ItemID, int ItemPos)
{
	for(int i=0;i<MAX_ILLUSION_TEMPLE_USER;++i)
	{
		if(this->m_IllusionTempleData[IllusionTempleIndex].WaitingPlayers[i].UserIndex == -1)
		{
			this->m_IllusionTempleData[IllusionTempleIndex].WaitingPlayers[i].UserIndex = aIndex;
			this->m_IllusionTempleData[IllusionTempleIndex].WaitingPlayers[i].ItemSerial = ItemSerial;
			this->m_IllusionTempleData[IllusionTempleIndex].WaitingPlayers[i].ItemID = ItemID;
			this->m_IllusionTempleData[IllusionTempleIndex].WaitingPlayers[i].ItemPos = ItemPos;
			gObj[aIndex].m_cIllusionTempleSubIndex = i;
			break;
		}
	}
	if(gObj[aIndex].m_cIllusionTempleSubIndex == -1)
	{
		gObjMoveGate(aIndex, 267);
		LogAddC(2, "[Illusion Temple] (%d) [%s][%s] CIllusionTemple::EnterToWaitingRoom(): ERROR: m_cIllusionTempleSubIndex == -1",
			IllusionTempleIndex, gObj[aIndex].AccountID, gObj[aIndex].Name);
		return;
	}

	this->m_IllusionTempleData[IllusionTempleIndex].UserCount++;
	this->SendStateMsg(IllusionTempleIndex, false, aIndex, -1, 0);
	PMSG_ILLUSION_WAITING_ROOM pRoom;
	PHeadSubSetB((LPBYTE)&pRoom, 0xBF, 0x03, sizeof(pRoom));
	pRoom.IllusionTempleIndex = IllusionTempleIndex;
	pRoom.Unk = 0;
	IOCP.DataSend(aIndex, (LPBYTE)&pRoom, pRoom.h.size);
}

int CIllusionTemple::GetWaitingUserCount(int IllusionTempleIndex)
{
	return this->m_IllusionTempleData[IllusionTempleIndex].UserCount;
}

bool CIllusionTemple::CheckCanEnter(int aIndex, int IllusionTempleIndex)
{
	if(!IT_BATTLEZONE_RANGE(IllusionTempleIndex))
		return false;

	return this->m_IllusionTempleData[IllusionTempleIndex].Open;
}

void CIllusionTemple::SendStateMsg(int IllusionTempleIndex, bool ForAll, int aIndex, int Destination, BYTE Msg)
{
	PMSG_ILLUSION_SET_STATE pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xBF, 0x09, sizeof(pMsg));
	pMsg.IllusionTempleIndex = IllusionTempleIndex;
	pMsg.State = Msg;
	if(ForAll)
	{
		if(Destination == 1)
		{
			this->SendIllusionTempleRoomAnyMsg((LPBYTE)&pMsg, pMsg.h.size, IllusionTempleIndex);
		}
		else if(Destination == 2)
		{
			this->SendIllusionTempleTeamAnyMsg((LPBYTE)&pMsg, pMsg.h.size, IllusionTempleIndex);
		}
	}
	else
	{
		IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
	}
}

void CIllusionTemple::SetTeams(int IllusionTempleIndex)
{
	int blue=0;
	int yellow=0;
	int team;
	for(int i=0;i<MAX_ILLUSION_TEMPLE_USER;++i)
	{
		if(gObjIsConnected(this->m_IllusionTempleData[IllusionTempleIndex].WaitingPlayers[i].UserIndex))
		{
			if(IT_MAP_RANGE(gObj[this->m_IllusionTempleData[IllusionTempleIndex].WaitingPlayers[i].UserIndex].MapNumber))
			{
				if(gObj[this->m_IllusionTempleData[IllusionTempleIndex].WaitingPlayers[i].UserIndex].m_cIllusionTempleIndex == IllusionTempleIndex)
				{
					if(yellow == 5)
					{
						this->m_IllusionTempleData[IllusionTempleIndex].WaitingPlayers[i].Team = 1;
						++blue;
					}
					else if(blue == 5)
					{
						this->m_IllusionTempleData[IllusionTempleIndex].WaitingPlayers[i].Team = 0;
						++yellow;
					}
					else
					{
						if(blue == yellow)
						{
							team = rand()%2;
							this->m_IllusionTempleData[IllusionTempleIndex].WaitingPlayers[i].Team = team;
							if(team == 0) ++yellow;
							else ++blue;
						}
						else
						{
							team = yellow > blue ? 1 : 0;
							this->m_IllusionTempleData[IllusionTempleIndex].WaitingPlayers[i].Team = team;
							if(team == 0) ++yellow;
							else ++blue;
						}
					}
				}
			}
		}
	}
	yellow = 0;
	blue = 0;
	for(int i=0;i<MAX_ILLUSION_TEMPLE_USER;++i)
	{
		if(gObjIsConnected(this->m_IllusionTempleData[IllusionTempleIndex].WaitingPlayers[i].UserIndex))
		{
			if(IT_MAP_RANGE(gObj[this->m_IllusionTempleData[IllusionTempleIndex].WaitingPlayers[i].UserIndex].MapNumber))
			{
				if(gObj[this->m_IllusionTempleData[IllusionTempleIndex].WaitingPlayers[i].UserIndex].m_cIllusionTempleIndex == IllusionTempleIndex)
				{
					if(this->m_IllusionTempleData[IllusionTempleIndex].WaitingPlayers[i].Team == 0)
					{
						this->m_IllusionTempleData[IllusionTempleIndex].Teams[0].TeamMate[yellow].UserIndex = this->m_IllusionTempleData[IllusionTempleIndex].WaitingPlayers[i].UserIndex;
						gObj[this->m_IllusionTempleData[IllusionTempleIndex].WaitingPlayers[i].UserIndex].m_cIllusionTempleTeam = 0;
						gObj[this->m_IllusionTempleData[IllusionTempleIndex].WaitingPlayers[i].UserIndex].m_cIllusionTempleTeamIndex = yellow;
						LogAddTD("[Illusion Temple] (%d) [%s][%s] - Allies Team", IllusionTempleIndex+1, gObj[this->m_IllusionTempleData[IllusionTempleIndex].WaitingPlayers[i].UserIndex].AccountID,
						gObj[this->m_IllusionTempleData[IllusionTempleIndex].WaitingPlayers[i].UserIndex].Name);
						++this->m_IllusionTempleData[IllusionTempleIndex].Teams[0].Count;
						++yellow;
					}
					else
					{
						this->m_IllusionTempleData[IllusionTempleIndex].Teams[1].TeamMate[blue].UserIndex = this->m_IllusionTempleData[IllusionTempleIndex].WaitingPlayers[i].UserIndex;
						gObj[this->m_IllusionTempleData[IllusionTempleIndex].WaitingPlayers[i].UserIndex].m_cIllusionTempleTeam = 1;
						gObj[this->m_IllusionTempleData[IllusionTempleIndex].WaitingPlayers[i].UserIndex].m_cIllusionTempleTeamIndex = blue;
						LogAddTD("[Illusion Temple] (%d) [%s][%s] - Illusions Team", IllusionTempleIndex+1, gObj[this->m_IllusionTempleData[IllusionTempleIndex].WaitingPlayers[i].UserIndex].AccountID,
						gObj[this->m_IllusionTempleData[IllusionTempleIndex].WaitingPlayers[i].UserIndex].Name);
						++this->m_IllusionTempleData[IllusionTempleIndex].Teams[1].Count;
						++blue;
					}

					gObj[this->m_IllusionTempleData[IllusionTempleIndex].WaitingPlayers[i].UserIndex].m_IfState.use = 0;
				}
			}
		}
	}
	LogAddTD("[Illusion Temple] (%d) Allies [%d] vs. Illusions [%d]", IllusionTempleIndex+1, yellow, blue);
}

void CIllusionTemple::SetPartyTeam(int IllusionTempleIndex)
{
	int PartyID;
	for(int i=0;i<MAX_ILLUSION_TEMPLE_TEAMS;++i)
	{
		PartyID = gParty.Create(this->m_IllusionTempleData[IllusionTempleIndex].Teams[i].TeamMate[0].UserIndex, gObj[this->m_IllusionTempleData[IllusionTempleIndex].Teams[i].TeamMate[0].UserIndex].DBNumber,
			gObj[this->m_IllusionTempleData[IllusionTempleIndex].Teams[i].TeamMate[0].UserIndex].Level);
		gObj[this->m_IllusionTempleData[IllusionTempleIndex].Teams[i].TeamMate[0].UserIndex].PartyNumber = PartyID;
		for(int n=1;n<MAX_ILLUSION_TEMPLE_TEAM_USER;++n) // n = 1, poniewaz gracz nr 0 jest Party Masterem
		{
			if(this->m_IllusionTempleData[IllusionTempleIndex].Teams[i].TeamMate[n].UserIndex != -1)
			{
				gObj[this->m_IllusionTempleData[IllusionTempleIndex].Teams[i].TeamMate[n].UserIndex].PartyNumber = -1;
				gParty.Add(PartyID, this->m_IllusionTempleData[IllusionTempleIndex].Teams[i].TeamMate[n].UserIndex,
					gObj[this->m_IllusionTempleData[IllusionTempleIndex].Teams[i].TeamMate[n].UserIndex].DBNumber,
					gObj[this->m_IllusionTempleData[IllusionTempleIndex].Teams[i].TeamMate[n].UserIndex].Level);
				gObj[this->m_IllusionTempleData[IllusionTempleIndex].Teams[i].TeamMate[n].UserIndex].PartyNumber = PartyID;
			}
		}
		this->m_IllusionTempleData[IllusionTempleIndex].Teams[i].PartyID = PartyID;
		gParty.Paint(PartyID);
		GSProtocol.CGPartyListAll(PartyID);
		gParty.PartyMemberLifeSend(PartyID);
	}
}

void CIllusionTemple::IllusionPlayers(int IllusionTempleIndex)
{
	for(int i=0;i<MAX_ILLUSION_TEMPLE_TEAMS;++i)
	{
		for(int n=0;n<MAX_ILLUSION_TEMPLE_TEAM_USER;++n)
		{
			if(i == 0)
			{
				if(this->m_IllusionTempleData[IllusionTempleIndex].Teams[i].TeamMate[n].UserIndex != -1)
				{
					gObj[this->m_IllusionTempleData[IllusionTempleIndex].Teams[i].TeamMate[n].UserIndex].m_Change = YellowIllusion;
					gObjViewportListProtocolCreate(&gObj[this->m_IllusionTempleData[IllusionTempleIndex].Teams[i].TeamMate[n].UserIndex]);
				}
			}
			else if(i == 1)
			{
				if(this->m_IllusionTempleData[IllusionTempleIndex].Teams[i].TeamMate[n].UserIndex != -1)
				{
					gObj[this->m_IllusionTempleData[IllusionTempleIndex].Teams[i].TeamMate[n].UserIndex].m_Change = BlueIllusion;
					gObjViewportListProtocolCreate(&gObj[this->m_IllusionTempleData[IllusionTempleIndex].Teams[i].TeamMate[n].UserIndex]);
				}
			}
		}
	}
}

void CIllusionTemple::SetState_Playing(int IllusionTempleIndex)
{
	if(!IT_BATTLEZONE_RANGE(IllusionTempleIndex))
	{
		return;
	}
	if(this->m_IllusionTempleData[IllusionTempleIndex].UserCount < 4)
	{
		LogAddTD("[Illusion Temple] (%d) Failed to start: Lack of user %d/4", IllusionTempleIndex+1,
			this->m_IllusionTempleData[IllusionTempleIndex].UserCount);
		this->SetState_Closed(IllusionTempleIndex);
		return;
	}
	this->m_IllusionTempleData[IllusionTempleIndex].RemainMSEC = this->ReadyTime * 60 * 1000;
	this->CreateGuards(IllusionTempleIndex);
	this->MoveToBattleArea(IllusionTempleIndex);
	this->SendStateMsg(IllusionTempleIndex, true, -1, 2, 1);
	this->SetPartyTeam(IllusionTempleIndex);
	this->IllusionPlayers(IllusionTempleIndex);
	this->SetKillPoint(IllusionTempleIndex, true, -1, 10);
	LogAddTD("[Illusion Temple] (%d) SetState PLAYING", IllusionTempleIndex+1);
}

void CIllusionTemple::MoveToBattleArea(int IllusionTempleIndex)
{
	for(int i=0;i<MAX_ILLUSION_TEMPLE_USER;++i)
	{
		if(this->m_IllusionTempleData[IllusionTempleIndex].WaitingPlayers[i].UserIndex != -1)
		{
			if(this->m_IllusionTempleData[IllusionTempleIndex].WaitingPlayers[i].Team == 0)
			{
				gObjMoveGate(this->m_IllusionTempleData[IllusionTempleIndex].WaitingPlayers[i].UserIndex, g_IllusionGatesYellow[IllusionTempleIndex].Gate);
			}
			else
			{
				gObjMoveGate(this->m_IllusionTempleData[IllusionTempleIndex].WaitingPlayers[i].UserIndex, g_IllusionGatesBlue[IllusionTempleIndex].Gate);
			}
		}
	}
}

void CIllusionTemple::CreateGuards(int IllusionTempleIndex)
{
	if(!IT_BATTLEZONE_RANGE(IllusionTempleIndex))
	{
		return;
	}
	this->m_IllusionTempleData[IllusionTempleIndex].AlliesGuardianIndex = -1;
	this->m_IllusionTempleData[IllusionTempleIndex].IllusionGuardianIndex = -1;
	int AlliedGuard = this->m_IllusionTempleData[IllusionTempleIndex].AlliesGuardianPosNum;
	int IllusionGuard = this->m_IllusionTempleData[IllusionTempleIndex].IllusionGuardianPosNum;
	if(AlliedGuard != -1 && IllusionGuard != -1)
	{
		int result = gObjAddMonster(gMSetBase.m_Mp[AlliedGuard].m_MapNumber);
		WORD Class = gMSetBase.m_Mp[AlliedGuard].m_Type;
		if(result >= 0)
		{
			gObjSetPosMonster(result, AlliedGuard);
			gObjSetMonster(result, Class);
			gObj[result].Class = Class;
			gObj[result].DieRegen = 0;
			gObj[result].m_cIllusionTempleIndex = IllusionTempleIndex;
			this->m_IllusionTempleData[IllusionTempleIndex].AlliesGuardianIndex = result;
			LogAddTD("[Illusion Temple] (%d) Create NPC Allied Guard", IllusionTempleIndex+1);
		}
		result = gObjAddMonster(gMSetBase.m_Mp[IllusionGuard].m_MapNumber);
		Class = gMSetBase.m_Mp[IllusionGuard].m_Type;
		if(result >= 0)
		{
			gObjSetPosMonster(result, IllusionGuard);
			gObjSetMonster(result, Class);
			gObj[result].Class = Class;
			gObj[result].DieRegen = 0;
			gObj[result].m_cIllusionTempleIndex = IllusionTempleIndex;
			this->m_IllusionTempleData[IllusionTempleIndex].IllusionGuardianIndex = result;
			LogAddTD("[Illusion Temple] (%d) Create NPC Illusion Guard", IllusionTempleIndex+1);
		}
	}
}

void CIllusionTemple::SetKillPoint(int IllusionTempleIndex, bool ForAll, int aIndex, int KillPoint)
{
	PMSG_ILLUSION_KILL_POINT pPoint;
	PHeadSubSetB((LPBYTE)&pPoint, 0xBF, 0x06, sizeof(pPoint));
	if(ForAll)
	{
		for(int i=0;i<MAX_ILLUSION_TEMPLE_TEAMS;++i)
		{
			for(int n=0;n<MAX_ILLUSION_TEMPLE_TEAM_USER;++n)
			{
				if(this->m_IllusionTempleData[IllusionTempleIndex].Teams[i].TeamMate[n].UserIndex != -1)
				{
					pPoint.KillPoint = KillPoint - gObj[this->m_IllusionTempleData[IllusionTempleIndex].Teams[i].TeamMate[n].UserIndex].m_btKillCount;
					gObj[this->m_IllusionTempleData[IllusionTempleIndex].Teams[i].TeamMate[n].UserIndex].m_btKillCount = KillPoint;
					IOCP.DataSend(this->m_IllusionTempleData[IllusionTempleIndex].Teams[i].TeamMate[n].UserIndex, (LPBYTE)&pPoint, pPoint.h.size);
				}
			}
		}
		return;
	}
	else
	{
		pPoint.KillPoint = KillPoint - gObj[aIndex].m_btKillCount;
		gObj[aIndex].m_btKillCount = KillPoint;
		IOCP.DataSend(aIndex, (LPBYTE)&pPoint, pPoint.h.size);
	}
}

void CIllusionTemple::SendIllusionTempleTeamAnyMsg(LPBYTE Msg, int Size, int IllusionTempleIndex)
{
	for(int i=0;i<MAX_ILLUSION_TEMPLE_TEAMS;++i)
	{
		for(int n=0;n<MAX_ILLUSION_TEMPLE_TEAM_USER;++n)
		{
			IOCP.DataSend(this->m_IllusionTempleData[IllusionTempleIndex].Teams[i].TeamMate[n].UserIndex, Msg, Size);
		}
	}
}

void CIllusionTemple::SendIllusionTempleRoomAnyMsg(LPBYTE Msg, int Size, int IllusionTempleIndex)
{
	for(int i=0;i<MAX_ILLUSION_TEMPLE_USER;++i)
	{
		IOCP.DataSend(this->m_IllusionTempleData[IllusionTempleIndex].WaitingPlayers[i].UserIndex, Msg, Size);
	}
}

void CIllusionTemple::ProcState_Playing(int IllusionTempleIndex)
{
	int Tick = GetTickCount() - this->m_IllusionTempleData[IllusionTempleIndex].Tick;
	if(Tick >= 1000)
	{
		this->PlayerProcess(IllusionTempleIndex);
		this->m_IllusionTempleData[IllusionTempleIndex].RemainMSEC -= Tick;
		this->m_IllusionTempleData[IllusionTempleIndex].Tick = GetTickCount();

		if(this->m_IllusionTempleData[IllusionTempleIndex].RemainMSEC <= 30000 &&
			this->m_IllusionTempleData[IllusionTempleIndex].BeforePlay == false)
		{
			this->m_IllusionTempleData[IllusionTempleIndex].BeforePlay = true;
			PMSG_SET_DEVILSQUARE pMsg;
			PHeadSetB((LPBYTE)&pMsg, 0x92, sizeof(pMsg));
			pMsg.Type = 15;
			this->SendIllusionTempleTeamAnyMsg((LPBYTE)&pMsg, pMsg.h.size, IllusionTempleIndex);
		}
		if(this->m_IllusionTempleData[IllusionTempleIndex].RemainMSEC <= 0 &&
			this->m_IllusionTempleData[IllusionTempleIndex].PlayStarted == false && this->m_IllusionTempleData[IllusionTempleIndex].BeforeSemiPlayEnd == false)
		{
			this->m_IllusionTempleData[IllusionTempleIndex].PlayStarted = true;
			this->m_IllusionTempleData[IllusionTempleIndex].RemainMSEC = this->PlayTime * 60 * 1000;
			this->CreateBoxes(IllusionTempleIndex);
			this->CreateStatues(IllusionTempleIndex);
			this->SendStateMsg(IllusionTempleIndex, true, -1, 2, 2);
			LogAddTD("[Illusion Temple] (%d) Quest Started", IllusionTempleIndex+1);
		}
		if(this->m_IllusionTempleData[IllusionTempleIndex].PlayStarted == false &&
			this->m_IllusionTempleData[IllusionTempleIndex].Open == false)
		{
			int CurrentUser = this->GetCurPlayUser(IllusionTempleIndex);
			if(CurrentUser < 1)
			{
				LogAddTD("[Illusion Temple] (%d) Reset Event - no user", IllusionTempleIndex+1);
				this->SetState(IllusionTempleIndex, IT_STATE_CLOSED);
				return;
			}
		}
		if(this->m_IllusionTempleData[IllusionTempleIndex].PlayStarted == true && this->m_IllusionTempleData[IllusionTempleIndex].BeforeSemiPlayEnd == false)
		{
			int aIndex;
			for(int i=0;i<MAX_ILLUSION_TEMPLE_TEAMS;++i)
			{
				for(int n=0;n<MAX_ILLUSION_TEMPLE_TEAM_USER;++n)
				{
					if((aIndex = this->m_IllusionTempleData[IllusionTempleIndex].Teams[i].TeamMate[n].UserIndex) != -1)
					{
						this->MinimapProcess(IllusionTempleIndex, n, i, aIndex);
					}
				}
			}

			int SacredSerial = -1;
			bool Statue = true;
			if(this->m_IllusionTempleData[IllusionTempleIndex].HeroIndex == -1 || !gObjIsConnected(this->m_IllusionTempleData[IllusionTempleIndex].HeroIndex))
			{

				if(this->m_IllusionTempleData[IllusionTempleIndex].StatueIndex[0] != -1)
				{
					if(gObjIsConnected(this->m_IllusionTempleData[IllusionTempleIndex].StatueIndex[0]) == false ||
						gObj[this->m_IllusionTempleData[IllusionTempleIndex].StatueIndex[0]].Live == false)
					{
						if(this->m_IllusionTempleData[IllusionTempleIndex].StatueIndex[1] != -1)
						{
							if(gObjIsConnected(this->m_IllusionTempleData[IllusionTempleIndex].StatueIndex[1]) == false ||
								gObj[this->m_IllusionTempleData[IllusionTempleIndex].StatueIndex[1]].Live == false)
							{
								Statue = false;
							}
						}
					}
				}
				
				for(int i=0;i<300;++i)
				{
					if(MapC[MAP_INDEX_ILLUSIONTEMPLE1+IllusionTempleIndex].m_cItem[i].m_Number == this->m_IllusionTempleData[IllusionTempleIndex].SacredItemSerial &&
						MapC[MAP_INDEX_ILLUSIONTEMPLE1+IllusionTempleIndex].m_cItem[i].live == true)
					{
						SacredSerial = i;
					}
				}

				if(SacredSerial == -1 && Statue == false)
				{
					this->m_IllusionTempleData[IllusionTempleIndex].SacredItemSerial = -1;
					this->CreateStatues(IllusionTempleIndex);
				}
			}

			if(this->m_IllusionTempleData[IllusionTempleIndex].HeroIndex != -1)
			{
				if(gObj[this->m_IllusionTempleData[IllusionTempleIndex].HeroIndex].Live == 0)
				{
					this->DropSacredItemFromHero(IllusionTempleIndex);
				}
			}

			char WinTeam = this->CheckWinnerTeam(IllusionTempleIndex, false);
			if(WinTeam >= 0 || WinTeam == -2 )
			{
				this->m_IllusionTempleData[IllusionTempleIndex].PlayStarted = false;
				this->m_IllusionTempleData[IllusionTempleIndex].WinTeam = this->CheckWinnerTeam(IllusionTempleIndex, true);
				this->m_IllusionTempleData[IllusionTempleIndex].BeforeSemiPlayEnd = true;
				this->m_IllusionTempleData[IllusionTempleIndex].RemainMSEC = 3000;
				this->DeleteMonsters(IllusionTempleIndex);
			}
		}
		if(this->m_IllusionTempleData[IllusionTempleIndex].BeforeSemiPlayEnd == true &&
			this->m_IllusionTempleData[IllusionTempleIndex].RemainMSEC <= 0)
		{
			this->TeleportToSecureArea(IllusionTempleIndex);
			this->RankingInfo(IllusionTempleIndex);
			this->SetState(IllusionTempleIndex, IT_STATE_PLAYEND);
		}
		if(this->m_IllusionTempleData[IllusionTempleIndex].PlayStarted == true && this->m_IllusionTempleData[IllusionTempleIndex].RemainMSEC <= 0)
		{
			this->m_IllusionTempleData[IllusionTempleIndex].PlayStarted = false;
			this->m_IllusionTempleData[IllusionTempleIndex].WinTeam = this->CheckWinnerTeam(IllusionTempleIndex, true);
			this->m_IllusionTempleData[IllusionTempleIndex].BeforeSemiPlayEnd = true;
			this->m_IllusionTempleData[IllusionTempleIndex].RemainMSEC = 3000;
			this->DeleteMonsters(IllusionTempleIndex);
		}
	}
}

void CIllusionTemple::MinimapProcess(int IllusionTempleIndex, int IllusionTempleSubIndex, char Team, int aIndex)
{
	PMSG_ILLUSION_INTERFACE pTeam = {0};
	PHeadSubSetB((LPBYTE)&pTeam, 0xBF, 0x01, sizeof(pTeam));
	pTeam.HeroIndex = this->m_IllusionTempleData[IllusionTempleIndex].HeroIndex;
	if(this->m_IllusionTempleData[IllusionTempleIndex].HeroIndex != -1)
	{
		pTeam.HeroX = gObj[this->m_IllusionTempleData[IllusionTempleIndex].HeroIndex].X;
		pTeam.HeroY = gObj[this->m_IllusionTempleData[IllusionTempleIndex].HeroIndex].Y;
	}
	pTeam.AlliesPoint = this->m_IllusionTempleData[IllusionTempleIndex].Teams[0].Score;
	pTeam.IllusionsPoint = this->m_IllusionTempleData[IllusionTempleIndex].Teams[1].Score;
	pTeam.Time = ((this->m_IllusionTempleData[IllusionTempleIndex].RemainMSEC / 1000));
	pTeam.PlayerTeam = Team;
	pTeam.Teammates = this->m_IllusionTempleData[IllusionTempleIndex].Teams[Team].Count-1;
	int user = 0;
	for(int i=0; i <MAX_ILLUSION_TEMPLE_TEAM_USER; ++i)
	{
		if(this->m_IllusionTempleData[IllusionTempleIndex].Teams[Team].TeamMate[i].UserIndex != -1 && this->m_IllusionTempleData[IllusionTempleIndex].Teams[Team].TeamMate[i].UserIndex != aIndex)
		{
			pTeam.Teammate[user].Switch = 1;
			pTeam.Teammate[user].Unk = 0;
			pTeam.Teammate[user].Index = this->m_IllusionTempleData[IllusionTempleIndex].Teams[Team].TeamMate[i].UserIndex;
			pTeam.Teammate[user].X = gObj[this->m_IllusionTempleData[IllusionTempleIndex].Teams[Team].TeamMate[i].UserIndex].X;
			pTeam.Teammate[user].Y = gObj[this->m_IllusionTempleData[IllusionTempleIndex].Teams[Team].TeamMate[i].UserIndex].Y;
			user++;
		}
	}
	IOCP.DataSend(aIndex, (LPBYTE)&pTeam, pTeam.h.size);
}

int CIllusionTemple::GetCurPlayUser(int IllusionTempleIndex)
{
	if(!IT_BATTLEZONE_RANGE(IllusionTempleIndex))
	{
		return -1;
	}
	int cnt = 0;
	for(int i=0;i<MAX_ILLUSION_TEMPLE_TEAMS;++i)
	{
		for(int n=0;n<MAX_ILLUSION_TEMPLE_TEAM_USER;++n)
		{
			if(gObjIsConnected(this->m_IllusionTempleData[IllusionTempleIndex].Teams[i].TeamMate[n].UserIndex) == true &&
				gObj[this->m_IllusionTempleData[IllusionTempleIndex].Teams[i].TeamMate[n].UserIndex].MapNumber == MAP_INDEX_ILLUSIONTEMPLE1+IllusionTempleIndex)
			{
				cnt++;
			}
		}
	}
	return cnt;
}

void CIllusionTemple::CreateBoxes(int IllusionTempleIndex)
{
	if(!IT_BATTLEZONE_RANGE(IllusionTempleIndex))
	{
		return;
	}
	this->m_IllusionTempleData[IllusionTempleIndex].AlliesBoxIndex = -1;
	this->m_IllusionTempleData[IllusionTempleIndex].IllusionBoxIndex = -1;
	int AlliedBox = this->m_IllusionTempleData[IllusionTempleIndex].AlliesBoxPosNum;
	int IllusionBox = this->m_IllusionTempleData[IllusionTempleIndex].IllusionBoxPosNum;
	
	int result = gObjAddMonster(gMSetBase.m_Mp[AlliedBox].m_MapNumber);
	WORD Class;
	if(result >= 0)
	{
		Class = gMSetBase.m_Mp[AlliedBox].m_Type;
		gObjSetPosMonster(result, AlliedBox);
		gObjSetMonster(result, Class);
		gObj[result].Class = Class;
		gObj[result].m_cIllusionTempleIndex = IllusionTempleIndex;
		this->m_IllusionTempleData[IllusionTempleIndex].AlliesBoxIndex = result;
		LogAddTD("[Illusion Temple] (%d) Create NPC Allies Box", IllusionTempleIndex+1);
	}
	result = gObjAddMonster(gMSetBase.m_Mp[IllusionBox].m_MapNumber);
	if(result >= 0)
	{
		Class = gMSetBase.m_Mp[IllusionBox].m_Type;
		gObjSetPosMonster(result, IllusionBox);
		gObjSetMonster(result, Class);
		gObj[result].Class = Class;
		gObj[result].m_cIllusionTempleIndex = IllusionTempleIndex;
		this->m_IllusionTempleData[IllusionTempleIndex].IllusionBoxIndex = result;
		LogAddTD("[Illusion Temple] (%d) Create NPC Illusions Box", IllusionTempleIndex+1);
	}
}

void CIllusionTemple::CreateStatues(int IllusionTempleIndex)
{
	if(this->m_IllusionTempleData[IllusionTempleIndex].StatueIndex[0] != -1)
	{
		gObjDel(this->m_IllusionTempleData[IllusionTempleIndex].StatueIndex[0]);
	}
	if(this->m_IllusionTempleData[IllusionTempleIndex].StatueIndex[1] != -1)
	{
		gObjDel(this->m_IllusionTempleData[IllusionTempleIndex].StatueIndex[1]);
	}
	this->m_IllusionTempleData[IllusionTempleIndex].StatueIndex[0] = -1;
	this->m_IllusionTempleData[IllusionTempleIndex].StatueIndex[1] = -1;
	int Statue = rand()%10000;
	int result = gObjAddMonster(MAP_INDEX_ILLUSIONTEMPLE1 + IllusionTempleIndex);
	if(result >= 0)
	{
		if(Statue % 2 == 0)
		{
			gObjSetPosMonster(result, this->m_IllusionTempleData[IllusionTempleIndex].StatuePosNum[0]);
			gObjSetMonster(result, gMSetBase.m_Mp[this->m_IllusionTempleData[IllusionTempleIndex].StatuePosNum[0]].m_Type);
			gObj[result].DieRegen = 0;
			gObj[result].Live = 1;
			gObj[result].m_PosNum = -1;
			gObj[result].m_cIllusionTempleIndex = IllusionTempleIndex;
			this->m_IllusionTempleData[IllusionTempleIndex].StatueIndex[0] = result;
		}
		else
		{
			gObjSetPosMonster(result, this->m_IllusionTempleData[IllusionTempleIndex].StatuePosNum[1]);
			gObjSetMonster(result, gMSetBase.m_Mp[this->m_IllusionTempleData[IllusionTempleIndex].StatuePosNum[1]].m_Type);
			gObj[result].DieRegen = 0;
			gObj[result].Live = 1;
			gObj[result].m_PosNum = -1;
			gObj[result].m_cIllusionTempleIndex = IllusionTempleIndex;
			this->m_IllusionTempleData[IllusionTempleIndex].StatueIndex[1] = result;
		}
	}
	LogAddTD("[Illusion Temple] (%d) Create NPC Statue", IllusionTempleIndex+1);
}

void CIllusionTemple::DropSacredItem(int IllusionTempleIndex, LPOBJ Statue, LPOBJ User)
{
	if(!IT_BATTLEZONE_RANGE(IllusionTempleIndex))
	{
		return;
	}
	if(User->m_IfState.use == 1 && User->m_IfState.type != 12)
	{
		return;
	}
	LogAddTD("[Illusion Temple] (%d) (%s)(%s) dropped Cursed Castle Water", IllusionTempleIndex+1, User->AccountID, User->Name);
	User->m_IfState.use = 0;
	User->m_IfState.type = 0;
	if(this->m_IllusionTempleData[IllusionTempleIndex].MonstersCreated == false)
	{
		this->m_IllusionTempleData[IllusionTempleIndex].MonstersCreated = true;
		this->CreateMonsters(IllusionTempleIndex);
	}
}

bool CIllusionTemple::CheckSacredItemSerial(int IllusionTempleIndex, CMapItem *Item)
{
	if(!IT_BATTLEZONE_RANGE(IllusionTempleIndex))
	{
		return false;
	}
	if(this->m_IllusionTempleData[IllusionTempleIndex].SacredItemSerial == -1)
	{
		return false;
	}
	if(Item->IsItem() == true)
	{
		if(Item->m_Type == ITEMGET(14,64))
		{
			if(this->m_IllusionTempleData[IllusionTempleIndex].SacredItemSerial == Item->m_Number)
			{
				return true;
			}
		}
	}
	return false;
}

void CIllusionTemple::GiveSacredItem(int IllusionTempleIndex, LPOBJ Statue, LPOBJ User)
{
	if(!IT_BATTLEZONE_RANGE(IllusionTempleIndex))
	{
		return;
	}
	if(this->m_IllusionTempleData[IllusionTempleIndex].SacredItemSerial == -1)
	{
		return;
	}
	if(this->m_IllusionTempleData[IllusionTempleIndex].HeroIndex == -1)
	{
		return;
	}
	if(User->m_IfState.use == 1 && User->m_IfState.type != 12)
	{
		return;
	}
	for(int i=INVENTORY_BAG_START; i< INVENTORY_SIZE; ++i)
	{
		if(User->pInventory[i].IsItem() == true)
		{
			if(User->pInventory[i].m_Type == ITEMGET(14,64))
			{
				if(User->pInventory[i].m_Number == this->m_IllusionTempleData[IllusionTempleIndex].SacredItemSerial)
				{
					if(User->m_Index == this->m_IllusionTempleData[IllusionTempleIndex].HeroIndex)
					{
						gObjInventoryDeleteItem(User->m_Index, i);
						GSProtocol.GCInventoryItemDeleteSend(User->m_Index, i, 1);
						User->m_IfState.type = 0;
						User->m_IfState.use = 0;
						this->m_IllusionTempleData[IllusionTempleIndex].Teams[User->m_cIllusionTempleTeam].Score++;
						this->m_IllusionTempleData[IllusionTempleIndex].Teams[User->m_cIllusionTempleTeam].TeamMate[User->m_cIllusionTempleTeamIndex].Score++;
						this->m_IllusionTempleData[IllusionTempleIndex].HeroIndex = -1;
						this->m_IllusionTempleData[IllusionTempleIndex].SacredItemSerial = -1;

						if(Statue->Class == 383)
						{
							LogAddTD("[Illusion Temple] (%d) (%s)(%s) Scored a point for Allies. A:[%d] I:[%d] UserPoint:[%d]", IllusionTempleIndex+1, User->AccountID, User->Name,
								this->m_IllusionTempleData[IllusionTempleIndex].Teams[0].Score,
								this->m_IllusionTempleData[IllusionTempleIndex].Teams[1].Score,
								this->m_IllusionTempleData[IllusionTempleIndex].Teams[User->m_cIllusionTempleTeam].TeamMate[User->m_cIllusionTempleTeamIndex].Score);
						}
						if(Statue->Class == 384)
						{
							LogAddTD("[Illusion Temple] (%d) (%s)(%s) Scored a point for Illusions. A:[%d] I:[%d] UserPoint:[%d]", IllusionTempleIndex+1, User->AccountID, User->Name,
								this->m_IllusionTempleData[IllusionTempleIndex].Teams[0].Score,
								this->m_IllusionTempleData[IllusionTempleIndex].Teams[1].Score,
								this->m_IllusionTempleData[IllusionTempleIndex].Teams[User->m_cIllusionTempleTeam].TeamMate[User->m_cIllusionTempleTeamIndex].Score);
						}

						this->CreateStatues(IllusionTempleIndex);
					}
				}
			}
		}
	}
}

void CIllusionTemple::CreateMonsters(int IllusionTempleIndex)
{
	int result;
	for(int i=0;i<gMSetBase.m_Count; ++i)
	{
		if(gMSetBase.m_Mp[i].m_MapNumber == IllusionTempleIndex+MAP_INDEX_ILLUSIONTEMPLE1)
		{
			if(gMSetBase.m_Mp[i].m_Type != 380 && gMSetBase.m_Mp[i].m_Type != 381 &&
				gMSetBase.m_Mp[i].m_Type != 382 && gMSetBase.m_Mp[i].m_Type != 383 &&
				gMSetBase.m_Mp[i].m_Type != 384)
			{
				result = gObjAddMonster(gMSetBase.m_Mp[i].m_MapNumber);
				if(result >= 0)
				{
					gObjSetPosMonster(result, i);
					gObjSetMonster(result, gMSetBase.m_Mp[i].m_Type);
					gObj[result].RegenTime = 10000;
				}
			}
		}
	}
}
void CIllusionTemple::DropSacredItemFromHero(int IllusionTempleIndex)
{
	if(!IT_BATTLEZONE_RANGE(IllusionTempleIndex))
	{
		return;
	}
	PMSG_ITEMTHROW pMsg;
	pMsg.h.c = 0xC6; // taki trik xD
	pMsg.Ipos = -1;
	int aIndex = this->m_IllusionTempleData[IllusionTempleIndex].HeroIndex;
	if(!OBJMAX_RANGE(aIndex) || !gObjIsConnected(aIndex))
	{
		return;
	}
	for(int i=12;i<108;++i)
	{
		if(gObj[aIndex].pInventory[i].IsItem() == true)
		{
			if(gObj[aIndex].pInventory[i].m_Type == ITEMGET(14,64))
			{
				if(gObj[aIndex].pInventory[i].m_Number == this->m_IllusionTempleData[IllusionTempleIndex].SacredItemSerial)
				{
					pMsg.Ipos = i;
					break;
				}
			}
		}
	}
	if(pMsg.Ipos == -1)
	{
		LogAddC(2, "[Illusion Temple] (%d) Error: Sacred Item not exist", IllusionTempleIndex+1);
		return;
	}
	pMsg.px = gObj[aIndex].X;
	pMsg.py = gObj[aIndex].Y;
	if(GSProtocol.CGItemDropRequest(&pMsg, aIndex, 1))
	{
		InterlockedExchange((LPLONG)&this->m_IllusionTempleData[IllusionTempleIndex].HeroIndex, -1);
	}
}
bool CIllusionTemple::AttackCheck(LPOBJ lpObj, LPOBJ lpTargetObj, bool AllowPk)
{
	if(lpObj->m_cIllusionTempleIndex != lpTargetObj->m_cIllusionTempleIndex)
	{
		return false;
	}
	if(lpObj->MapNumber != lpTargetObj->MapNumber)
	{
		return false;
	}
	if(lpObj->m_cIllusionTempleSubIndex == -1 || lpTargetObj->m_cIllusionTempleSubIndex == -1)
	{
		return false;
	}
	if(this->m_IllusionTempleData[lpObj->m_cIllusionTempleIndex].PlayStarted != true)
	{
		return false;
	}
	if(lpObj->m_cIllusionTempleTeam == -1 || lpTargetObj->m_cIllusionTempleTeam == -1)
	{
		return false;
	}
	if(this->m_IllusionTempleData[lpObj->m_cIllusionTempleIndex].PlayStarted == false)
	{
		return false;
	}
	if(lpObj->m_cIllusionTempleTeam == lpTargetObj->m_cIllusionTempleTeam && AllowPk == false)
	{
		return true;
	}
	if(lpObj->m_cIllusionTempleTeam != lpTargetObj->m_cIllusionTempleTeam)
	{
		return true;
	}
	else
	{
		return false;
	}
	return false;
}
void CIllusionTemple::AddKillPoint(int IllusionTempleIndex, int aIndex, int Points)
{
	if(!IT_BATTLEZONE_RANGE(IllusionTempleIndex))
	{
		return;
	}
	PMSG_ILLUSION_KILL_POINT pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xBF, 0x06, sizeof(pMsg));
	gObj[aIndex].m_btKillCount += Points;
	pMsg.KillPoint = gObj[aIndex].m_btKillCount;
	IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
	LogAddTD("[Illusion Temple] (%d) (Bug Tracker) Added %d kill point", IllusionTempleIndex+1, Points);
}
void CIllusionTemple::AddKillCount(int aIndex)
{
	int Team = gObj[aIndex].m_cIllusionTempleTeam;
	int Index = gObj[aIndex].m_cIllusionTempleIndex;
	int TeamIndex = gObj[aIndex].m_cIllusionTempleTeamIndex;

	if(this->m_IllusionTempleData[Index].Teams[Team].TeamMate[TeamIndex].UserIndex == aIndex)
	{
		InterlockedIncrement((LPLONG)&this->m_IllusionTempleData[Index].Teams[Team].TeamMate[TeamIndex].KillCount);
	}
}
void CIllusionTemple::UseSkill(int IllusionTempleIndex, PMSG_REQ_USEILLUSIONTEMPLESKILL * aRecv, int aIndex)
{
	if(!IT_BATTLEZONE_RANGE(IllusionTempleIndex))
	{
		return;
	}
	WORD TargetIndex = MAKE_NUMBERW(aRecv->PlayerIndexH, aRecv->PlayerIndexL);
	if(TargetIndex == 0)
	{
		TargetIndex = aIndex;
	}
	switch (aRecv->SkillID)
	{
		case AT_SKILL_ILLUSION_SHIELD:
			this->SkillShield(aIndex);
			break;
		case AT_SKILL_ILLUSION_PARALYZE:
			this->SkillParalyze(aIndex, TargetIndex);
			break;
		case AT_SKILL_ILLUSION_TELEPORT:
			this->SkillTeleport(aIndex);
			break;
		case AT_SKILL_ILLUSION_ATTACK:
			this->SkillAttack(aIndex, TargetIndex);
			break;
	}
}
void CIllusionTemple::DisableSkill(int IllusionTempleIndex, LPOBJ lpObj, int Skill)
{
	PMSG_ILLUSION_DISABLESKILL pSkill;
	PHeadSubSetB((LPBYTE)&pSkill, 0xBF, 0x07, sizeof(pSkill));
	pSkill.aIndex = lpObj->m_Index;
	pSkill.SkillIDH = LOBYTE(Skill);
	pSkill.SkillIDL = HIBYTE(Skill);
	IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pSkill, pSkill.h.size);
	GSProtocol.MsgSendV2(lpObj, (LPBYTE)&pSkill, pSkill.h.size);
}
int CIllusionTemple::CheckWinnerTeam(int IllusionTempleIndex, bool EndTime)
{
	if(this->m_IllusionTempleData[IllusionTempleIndex].Teams[0].Score >= 7)
	{
		return 0;
	}
	else if(this->m_IllusionTempleData[IllusionTempleIndex].Teams[1].Score >= 7)
	{
		return 1;
	}

	if((this->m_IllusionTempleData[IllusionTempleIndex].Teams[0].Count <= 0 ||
		this->m_IllusionTempleData[IllusionTempleIndex].Teams[1].Count <= 0) && EndTime == false)
	{
		if(this->m_IllusionTempleData[IllusionTempleIndex].Teams[0].Score >= 2 ||
			this->m_IllusionTempleData[IllusionTempleIndex].Teams[1].Score >= 2)
		{
			if(this->m_IllusionTempleData[IllusionTempleIndex].Teams[0].Score > this->m_IllusionTempleData[IllusionTempleIndex].Teams[1].Score)
			{
				return 0;
			}
			else
			{
				return 1;
			}
		}
		else
		{
			return -2;
		}
	}

	if((this->m_IllusionTempleData[IllusionTempleIndex].Teams[0].Score >= 2 ||
		this->m_IllusionTempleData[IllusionTempleIndex].Teams[1].Score >= 2) && EndTime == true)
	{
		if(this->m_IllusionTempleData[IllusionTempleIndex].Teams[0].Score > this->m_IllusionTempleData[IllusionTempleIndex].Teams[1].Score)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
	else
	{
		return -1;
	}
}

struct ILLUSION_EXP_DATA
{
	int Exp;
}  g_IT_Exp[MAX_ILLUSION_TEMPLE_LEVEL] = {
	300000,
	340000,
	380000,
	420000,
	460000,
	500000

},	g_IT_PointExp[MAX_ILLUSION_TEMPLE_LEVEL] =  {
	50000,
	60000,
	70000,
	80000,
	90000,
	100000

}, g_IT_KillExp[MAX_ILLUSION_TEMPLE_LEVEL] = {
	600,
	1200,
	1800,
	2400,
	3000,
	3600

}, g_IT_BallExp[MAX_ILLUSION_TEMPLE_LEVEL] = {
	1200,
	2400,
	3600,
	4800,
	6000,
	7200

}, g_IT_PartyExp[5] = {
	10,
	20,
	30,
	40,
	50
};

void CIllusionTemple::RankingInfo(int IllusionTempleIndex)
{
	int Users[10] = {0};
	int k = 0;

	for(int i=0;i<MAX_ILLUSION_TEMPLE_TEAMS;++i)
	{
		for(int n=0;n<MAX_ILLUSION_TEMPLE_TEAM_USER;++n)
		{
			if(this->m_IllusionTempleData[IllusionTempleIndex].Teams[i].TeamMate[n].UserIndex != -1)
			{
				Users[k] = this->m_IllusionTempleData[IllusionTempleIndex].Teams[i].TeamMate[n].UserIndex;
				++k;
			}
		}
	}

	this->GiveRewardExp(IllusionTempleIndex);

	int PacketSize = 0;
	BYTE buf[256] = {0};
	PMSG_ILLUSION_STATISTICS * pStats = (PMSG_ILLUSION_STATISTICS *)(buf);
	PMSG_ILLUSION_STATISTICS_DATA * pData = (PMSG_ILLUSION_STATISTICS_DATA *)(buf+sizeof(PMSG_ILLUSION_STATISTICS));

	pStats->AlliedTeam = this->m_IllusionTempleData[IllusionTempleIndex].Teams[0].Score;
	pStats->IllusionTeam = this->m_IllusionTempleData[IllusionTempleIndex].Teams[1].Score;
	pStats->PlayersCount = this->m_IllusionTempleData[IllusionTempleIndex].UserCount;

	for(int i=0;i<this->m_IllusionTempleData[IllusionTempleIndex].UserCount;++i)
	{
		pData = (PMSG_ILLUSION_STATISTICS_DATA *)(buf+sizeof(PMSG_ILLUSION_STATISTICS) + (i * sizeof(PMSG_ILLUSION_STATISTICS_DATA)));

		pData->Class = gObj[Users[i]].Class;
		pData->Team = gObj[Users[i]].m_cIllusionTempleTeam;
		pData->ChangeUP = 0x32; // unknown data, always 50, so..
		pData->Unk[0] = 0xC9;
		pData->Unk[1] = 0xA2;
		pData->Unk[2] = 0x12;

		if(pData->Team == this->m_IllusionTempleData[IllusionTempleIndex].WinTeam)
		{
			pData->Experience = g_IT_Exp[IllusionTempleIndex].Exp;
		}
		else
		{
			pData->Experience = 0;
		}
		memcpy(pData->Name, gObj[Users[i]].Name, sizeof(pData->Name));
	}

	PacketSize = sizeof(PMSG_ILLUSION_STATISTICS) + (pStats->PlayersCount * sizeof(PMSG_ILLUSION_STATISTICS_DATA));
	pStats->h.set((PBYTE)pStats, 0xBF, 0x04, PacketSize);

	this->SendIllusionTempleTeamAnyMsg(buf, PacketSize, IllusionTempleIndex);
}
void CIllusionTemple::TeleportToSecureArea(int IllusionTempleIndex)
{
	for(int i=0;i<MAX_ILLUSION_TEMPLE_TEAM_USER;++i)
	{
		if(this->m_IllusionTempleData[IllusionTempleIndex].Teams[0].TeamMate[i].UserIndex != -1)
		{
			gObjMoveGate(this->m_IllusionTempleData[IllusionTempleIndex].Teams[0].TeamMate[i].UserIndex, g_IllusionGatesYellow[IllusionTempleIndex].Gate);
		}
	}
	for(int i=0;i<MAX_ILLUSION_TEMPLE_TEAM_USER;++i)
	{
		if(this->m_IllusionTempleData[IllusionTempleIndex].Teams[1].TeamMate[i].UserIndex != -1)
		{
			gObjMoveGate(this->m_IllusionTempleData[IllusionTempleIndex].Teams[1].TeamMate[i].UserIndex, g_IllusionGatesBlue[IllusionTempleIndex].Gate);
		}
	}
}
void CIllusionTemple::SetState_PlayEnd(int IllusionTempleIndex)
{
	if(!IT_BATTLEZONE_RANGE(IllusionTempleIndex))
	{
		return;
	}
	this->SendStateMsg(IllusionTempleIndex,true,-1,2,3);
	this->m_IllusionTempleData[IllusionTempleIndex].BeforeClosed = true;
	this->DeleteMonsters(IllusionTempleIndex);
	this->m_IllusionTempleData[IllusionTempleIndex].RemainMSEC = this->RestTime * 60 * 1000;
	for(int i=12;i<108;++i)
	{
		if(this->m_IllusionTempleData[IllusionTempleIndex].HeroIndex != -1)
		{
			if(gObj[this->m_IllusionTempleData[IllusionTempleIndex].HeroIndex].pInventory[i].m_Number == this->m_IllusionTempleData[IllusionTempleIndex].SacredItemSerial)
			{
				gObjInventoryDeleteItem(this->m_IllusionTempleData[IllusionTempleIndex].HeroIndex, i);
				GSProtocol.GCInventoryItemDeleteSend(this->m_IllusionTempleData[IllusionTempleIndex].HeroIndex, i, 1);
			}
		}
		else
		{
			break;
		}
	}
	this->m_IllusionTempleData[IllusionTempleIndex].SacredItemSerial = -1;
			
	if(this->m_IllusionTempleData[IllusionTempleIndex].WinTeam == 0)
	{
		LogAddTD("[Illusion Temple] (%d) Winner Team: Allies [%d]:[%d]", IllusionTempleIndex+1, this->m_IllusionTempleData[IllusionTempleIndex].Teams[0].Score,
			this->m_IllusionTempleData[IllusionTempleIndex].Teams[1].Score);
	}
	else if(this->m_IllusionTempleData[IllusionTempleIndex].WinTeam == 1 )
	{
		LogAddTD("[Illusion Temple] (%d) Winner Team: Illusions [%d]:[%d]", IllusionTempleIndex+1, this->m_IllusionTempleData[IllusionTempleIndex].Teams[0].Score,
			this->m_IllusionTempleData[IllusionTempleIndex].Teams[1].Score);
	}
	else
	{
		LogAddTD("[Illusion Temple] (%d) It's a Tie [%d]:[%d]", IllusionTempleIndex+1, this->m_IllusionTempleData[IllusionTempleIndex].Teams[0].Score,
			this->m_IllusionTempleData[IllusionTempleIndex].Teams[1].Score);
	}
	LogAddTD("[Illusion Temple] (%d) SetState PLAYEND", IllusionTempleIndex+1);
}
void CIllusionTemple::ProcState_Playend(int IllusionTempleIndex)
{
	int TickCount = GetTickCount() - this->m_IllusionTempleData[IllusionTempleIndex].Tick;
	if(TickCount >= 1000) // 1sekunda timera
	{
		this->m_IllusionTempleData[IllusionTempleIndex].RemainMSEC -= TickCount;
		this->m_IllusionTempleData[IllusionTempleIndex].Tick = GetTickCount();
		this->PlayerProcess(IllusionTempleIndex);
		if(this->m_IllusionTempleData[IllusionTempleIndex].RemainMSEC <= 0 && this->m_IllusionTempleData[IllusionTempleIndex].BeforeClosed == true)
		{
			this->SetState(IllusionTempleIndex, IT_STATE_CLOSED);
		}
	}
}
void CIllusionTemple::ProcState_None(int IllusionTempleIndex)
{
	return;
}
void CIllusionTemple::SetState_None(int IllusionTempleIndex)
{
	return;
}
void CIllusionTemple::DecKillPoint(int IllusionTempleIndex, int aIndex, int Points)
{
	if(!IT_BATTLEZONE_RANGE(IllusionTempleIndex))
	{
		return;
	}
	PMSG_ILLUSION_KILL_POINT pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xBF, 0x06, sizeof(pMsg));
	gObj[aIndex].m_btKillCount -= Points;
	pMsg.KillPoint = gObj[aIndex].m_btKillCount;
	IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
	LogAddTD("[Illusion Temple] (%d) (Bug Tracker) Decreased %d kill point", IllusionTempleIndex+1, Points);
}
int CIllusionTemple::GetViewportState()
{
	if(this->m_IllusionTempleData[0].State == 0)
	{
		return 0;
	}
	if(this->m_IllusionTempleData[0].State == 1 && this->m_IllusionTempleData[0].RemainMSEC > this->OpenTime * 60 * 1000)
	{
		return 1;
	}
	else if(this->m_IllusionTempleData[0].State == 1 && this->m_IllusionTempleData[0].RemainMSEC <= this->OpenTime * 60 * 1000)
	{
		return 2;
	}
	if(this->m_IllusionTempleData[0].State == 2)
	{
		return 3;
	}
	if(this->m_IllusionTempleData[0].State == 3)
	{
		return 4;
	}
	return 0;
}
void CIllusionTemple::PlayerProcess(int IllusionTempleIndex)
{
	int UserIndex;
	for(int i=0;i<MAX_ILLUSION_TEMPLE_TEAMS ;++i)
	{
		for(int n=0;n<MAX_ILLUSION_TEMPLE_TEAM_USER;++n)
		{
			UserIndex = this->m_IllusionTempleData[IllusionTempleIndex].Teams[i].TeamMate[n].UserIndex;
			if(UserIndex != -1)
			{
				if(!gObjIsConnected(UserIndex))
				{
					this->DeletePlayer(IllusionTempleIndex, UserIndex, false, i);
				}
				if(gObj[UserIndex].MapNumber != IllusionTempleIndex+MAP_INDEX_ILLUSIONTEMPLE1)
				{
					this->DeletePlayer(IllusionTempleIndex, UserIndex, false, i);
				}
			}
		}
	}
}
void CIllusionTemple::PlayerWaitingProcess(int IllusionTempleIndex)
{
	int UserIndex;
	for(int i=0;i<MAX_ILLUSION_TEMPLE_USER;++i)
	{
		UserIndex = this->m_IllusionTempleData[IllusionTempleIndex].WaitingPlayers[i].UserIndex;
		if(UserIndex != -1)
		{
			if(!gObjIsConnected(UserIndex))
			{
				this->DeletePlayer(IllusionTempleIndex, UserIndex, true);
			}
			if(!IT_MAP_RANGE(gObj[UserIndex].MapNumber))
			{
				this->DeletePlayer(IllusionTempleIndex, UserIndex, true);
			}
		}
	}
}
int	CIllusionTemple::GetRemainTime(int IllusionTempleIndex, int aIndex)
{
	int iREMAIN_MINUTE = 0;

	if(!IT_BATTLEZONE_RANGE(IllusionTempleIndex))
	{
		IllusionTempleIndex = this->GetUserLevelRoom(aIndex);
	}

	if (this->m_IllusionTempleData[IllusionTempleIndex].State == 1)
	{
		iREMAIN_MINUTE = (this->m_IllusionTempleData[IllusionTempleIndex].RemainMSEC / 60000) - (this->OpenTime + 1);
	}
	else
	{
		SYSTEMTIME sysTime;
		GetLocalTime(&sysTime);
		int iMIN_HOUR = 24;
		int iMIN_MINUTE = 60;
		BOOL bTIME_CHANGED = FALSE;
		std::vector<ILLUSION_START_TIME>::iterator it;

		for( it = this->v_IllusionStartTime.begin();  it != this->v_IllusionStartTime.end(); it++)
		{
			ILLUSION_START_TIME & pRET = *it;
			
			if ( (sysTime.wHour * 60 + sysTime.wMinute) < (pRET.Hour * 60 + pRET.Minute))
			{
				if ( (iMIN_HOUR * 60 + iMIN_MINUTE) > (pRET.Hour * 60 + pRET.Minute))
				{
					bTIME_CHANGED = 1;
					iMIN_HOUR = pRET.Hour;
					iMIN_MINUTE = pRET.Minute;
				}
			}
		}

		
					
		if ( bTIME_CHANGED == 0 )
		{
			it = this->v_IllusionStartTime.begin();
			iMIN_HOUR = 24;
			iMIN_MINUTE = 60;

			for( ;  it != this->v_IllusionStartTime.end(); it++)
			{
				ILLUSION_START_TIME & pRET = *it;
			
				if ( (iMIN_HOUR * 60 + iMIN_MINUTE) > (pRET.Hour * 60 + pRET.Minute))
				{
					bTIME_CHANGED = 2;
					iMIN_HOUR = pRET.Hour;
					iMIN_MINUTE = pRET.Minute;
				}
			}
		}

		switch ( bTIME_CHANGED )
		{
			case 1:
				iREMAIN_MINUTE = (((iMIN_HOUR * 60 * 60 + iMIN_MINUTE * 60) - (sysTime.wHour * 60 * 60 + sysTime.wMinute * 60 + sysTime.wSecond))) /60 + 1;
				break;
			case 2:
				iREMAIN_MINUTE = ((((iMIN_HOUR + 24) * 60 * 60 + iMIN_MINUTE * 60) - (sysTime.wHour * 60 * 60 + sysTime.wMinute * 60 + sysTime.wSecond)) ) / 60 + 1;
				break;
			default:
				iREMAIN_MINUTE = -1;
				break;
		}
	}

	if ( iREMAIN_MINUTE < 0 )
		iREMAIN_MINUTE = 0;

	return iREMAIN_MINUTE;
}

void CIllusionTemple::DeletePlayer(int IllusionTempleIndex, int aIndex, bool WaitingRoom, char Team)
{
	if(WaitingRoom)
	{
		for(int i=0;i<MAX_ILLUSION_TEMPLE_USER;++i)
		{
			if(this->m_IllusionTempleData[IllusionTempleIndex].WaitingPlayers[i].UserIndex == aIndex)
			{
				InterlockedExchange((LPLONG)&this->m_IllusionTempleData[IllusionTempleIndex].WaitingPlayers[i].Team, -1);
				InterlockedExchange((LPLONG)&this->m_IllusionTempleData[IllusionTempleIndex].WaitingPlayers[i].UserIndex, -1);
				InterlockedDecrement((LPLONG)&this->m_IllusionTempleData[IllusionTempleIndex].UserCount);
				gObj[this->m_IllusionTempleData[IllusionTempleIndex].WaitingPlayers[i].UserIndex].m_cIllusionTempleIndex = -1;
				gObj[this->m_IllusionTempleData[IllusionTempleIndex].WaitingPlayers[i].UserIndex].m_cIllusionTempleSubIndex = -1;
				this->Payback(this->m_IllusionTempleData[IllusionTempleIndex].WaitingPlayers[i].UserIndex);
				LogAddTD("[Illusion Temple] (%d) (%s)(%s) - Leave from Waiting Room", IllusionTempleIndex+1,
					gObj[aIndex].AccountID, gObj[aIndex].Name);
				break;
			}
		}
	}
	else
	{
		if(Team == -1)
		{
			LogAddC(2, "[Illusion Temple] (%d) CIllusionTemple::DeletePlayer ERROR: Team == -1", IllusionTempleIndex+1);
			return;
		}
		for(int i=0;i<MAX_ILLUSION_TEMPLE_TEAM_USER;++i)
		{
			if(this->m_IllusionTempleData[IllusionTempleIndex].Teams[Team].TeamMate[i].UserIndex == aIndex)
			{
				if(this->m_IllusionTempleData[IllusionTempleIndex].HeroIndex == aIndex)
				{
					this->DropSacredItemFromHero(IllusionTempleIndex);
				}
				InterlockedDecrement((LPLONG)&this->m_IllusionTempleData[IllusionTempleIndex].Teams[Team].Count);
				InterlockedDecrement((LPLONG)&this->m_IllusionTempleData[IllusionTempleIndex].UserCount);
				gParty.Delete(this->m_IllusionTempleData[IllusionTempleIndex].Teams[Team].PartyID, aIndex); 
				GSProtocol.GCPartyDelUserSend(aIndex);
				gObj[this->m_IllusionTempleData[IllusionTempleIndex].Teams[Team].TeamMate[i].UserIndex].m_cIllusionTempleIndex = -1;
				gObj[this->m_IllusionTempleData[IllusionTempleIndex].Teams[Team].TeamMate[i].UserIndex].m_cIllusionTempleSubIndex = -1;
				gObj[this->m_IllusionTempleData[IllusionTempleIndex].Teams[Team].TeamMate[i].UserIndex].m_cIllusionTempleTeam = -1;
				gObj[this->m_IllusionTempleData[IllusionTempleIndex].Teams[Team].TeamMate[i].UserIndex].m_cIllusionTempleTeamIndex = -1;
				this->m_IllusionTempleData[IllusionTempleIndex].Teams[Team].TeamMate[i].Clear();
				LogAddTD("[Illusion Temple] (%d) (%s)(%s) - Leave from Event", IllusionTempleIndex+1,
					gObj[aIndex].AccountID, gObj[aIndex].Name);
				break;
			}
		}
	}
}

int CIllusionTemple::CalcExp(int IllusionTempleIndex, int Team, int TeamIndex)
{
	int exp = 0;
	if(this->m_IllusionTempleData[IllusionTempleIndex].WinTeam == Team)
	{
		exp = (g_IT_Exp[IllusionTempleIndex].Exp + ((this->m_IllusionTempleData[IllusionTempleIndex].Teams[Team].Score -
			this->m_IllusionTempleData[IllusionTempleIndex].Teams[Team == 0 ? 1 : 0].Score)*g_IT_PointExp[IllusionTempleIndex].Exp)) * 2;
	}
	else
	{
		exp = (g_IT_Exp[IllusionTempleIndex].Exp + (this->m_IllusionTempleData[IllusionTempleIndex].Teams[Team].Score * g_IT_PointExp[IllusionTempleIndex].Exp));
	}

	exp += (this->m_IllusionTempleData[IllusionTempleIndex].Teams[Team].TeamMate[TeamIndex].KillCount * g_IT_KillExp[IllusionTempleIndex].Exp);
	exp += (this->m_IllusionTempleData[IllusionTempleIndex].Teams[Team].TeamMate[TeamIndex].Score * g_IT_BallExp[IllusionTempleIndex].Exp);
	exp += g_IT_PartyExp[this->m_IllusionTempleData[IllusionTempleIndex].Teams[Team].Count-1].Exp;

	LogAddTD("[Illusion Temple] (%d) [Bug Tracker]: Exp (%d)",
		IllusionTempleIndex, exp);

	return exp;
}

void CIllusionTemple::GiveReward(int aIndex)
{
	if ( !gObjIsConnected(aIndex))
		return;

	if ( !IT_MAP_RANGE(gObj[aIndex].MapNumber) )
	{
		return;
	}

	if ( gObj[aIndex].m_cIllusionTempleTeam == this->m_IllusionTempleData[gObj[aIndex].m_cIllusionTempleIndex].WinTeam )
	{
		IllusionTempleReward->DropIllusionItem(aIndex, 255, gObj[aIndex].X, gObj[aIndex].Y);
		g_CashShop.AddCoin(&gObj[aIndex], 6);
	}
}

int CIllusionTemple::GiveRewardExp(int IllusionTempleIndex)
{
	int iRET_EXP = 0;
	int iMAX_LEVCOUNT = 0;
	int iUserIndex;
	int iCAL_EXP;

	for(int i=0;i<MAX_ILLUSION_TEMPLE_TEAM_USER;++i)
	{
		iUserIndex = this->m_IllusionTempleData[IllusionTempleIndex].Teams[this->m_IllusionTempleData[IllusionTempleIndex].WinTeam].TeamMate[i].UserIndex;

		if ( !gObjIsConnected(iUserIndex))
			return 0;

		int iEXP = this->CalcExp(IllusionTempleIndex, gObj[iUserIndex].m_cIllusionTempleTeam, gObj[iUserIndex].m_cIllusionTempleTeamIndex);

		iCAL_EXP = iEXP;

		if ( iEXP <= 0 )
			return 0;

		if ( g_CrywolfSync.GetOccupationState() == 1 &&
			g_iCrywolfApplyMvpPenalty )
		{
			iEXP = iEXP * g_CrywolfSync.GetGettingExpPenaltyRate() / 100;
		}

		if(!MasterLevelSystem.CanGainMasterExp(&gObj[iUserIndex]))
		{
			if(iCAL_EXP > gObj[iUserIndex].Experience+gObj[iUserIndex].NextExp)
			{
				iCAL_EXP = gObj[iUserIndex].NextExp;
			}
		}
		else
		{
			if(iCAL_EXP > gObj[iUserIndex].MasterExperience+gObj[iUserIndex].MasterNextExp)
			{
				iCAL_EXP = gObj[iUserIndex].MasterNextExp;
			}
		}

		iRET_EXP = iCAL_EXP;

		if ( gObj[iUserIndex].Type == OBJ_USER )
		{
			while ( iCAL_EXP > 0 )
			{
				if ( iCAL_EXP > 0 )
				{
					iCAL_EXP = this->LevelUp(iUserIndex, iCAL_EXP, 6);
				}

				if ( iCAL_EXP == 0)
				{
					return iRET_EXP;
				}

				iMAX_LEVCOUNT++;

				if ( iMAX_LEVCOUNT > 1000 )
					break;
			}
		
			PMSG_KILLPLAYER_EXT pkillMsg;

			PHeadSetBE((LPBYTE)&pkillMsg, 0x9C, sizeof(pkillMsg));
			pkillMsg.NumberH = -1;
			pkillMsg.NumberL = -1;
			pkillMsg.ExpH = SET_NUMBERHW(iEXP);
			pkillMsg.ExpL = SET_NUMBERLW(iEXP);
			pkillMsg.DamageH = 0;
			pkillMsg.DamageL = 0;

			IOCP.DataSend(iUserIndex, (LPBYTE)&pkillMsg, pkillMsg.h.size);
		}
	}
	return iCAL_EXP;
}

int CIllusionTemple::LevelUp(int UserIndex, int Exp, int Event)
{
	if ( OBJMAX_RANGE(UserIndex) == FALSE )
	{
		return 0;
	}

	if ( MasterLevelSystem.CanGainMasterExp(&gObj[UserIndex]))
	{
		MasterLevelSystem.LevelUp(&gObj[UserIndex], Exp, 0, Event);
		return 0;
	}

	int iLEFT_EXP = 0;

	LogAddTD("Experience : Map[%d]-(%d,%d) [%s][%s](%d) %u %d MonsterIndex : %d, EventType : %d",
		gObj[UserIndex].MapNumber, gObj[UserIndex].X, gObj[UserIndex].Y,	
		gObj[UserIndex].AccountID,	gObj[UserIndex].Name,
		gObj[UserIndex].Level, gObj[UserIndex].Experience,
		Exp, 0, Event);

	::gObjSetExpPetItem(UserIndex, Exp);

	if ( gObj[UserIndex].Level >= MAX_CHAR_LEVEL )
	{
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,45), gObj[UserIndex].m_Index, 1);
		return 0;
	}

	if ( (gObj[UserIndex].Experience + Exp) < gObj[UserIndex].NextExp )
	{
		gObj[UserIndex].Experience += Exp;
	}
	else
	{
		iLEFT_EXP = gObj[UserIndex].Experience + Exp - gObj[UserIndex].NextExp;
		gObj[UserIndex].Experience = gObj[UserIndex].NextExp;
		gObj[UserIndex].Level++;

		if ( gObj[UserIndex].Class == CLASS_DARKLORD || gObj[UserIndex].Class == CLASS_MAGUMSA || gObj[UserIndex].Class == CLASS_RAGEFIGHTER )
		{
			gObj[UserIndex].LevelUpPoint += 7;
		}
		else
		{
			gObj[UserIndex].LevelUpPoint += 5;
		}

		if ( gObj[UserIndex].PlusStatQuestClear != false )
		{
			gObj[UserIndex].LevelUpPoint++;

			LogAddTD("[%s][%s] LevelUp PlusStatQuest Clear AddStat %d",
				gObj[UserIndex].AccountID, gObj[UserIndex].Name, gObj[UserIndex].LevelUpPoint);
		}

		gObj[UserIndex].MaxLife += DCInfo.DefClass[gObj[UserIndex].Class].LevelLife;
		gObj[UserIndex].MaxMana += DCInfo.DefClass[gObj[UserIndex].Class].LevelMana;
		gObj[UserIndex].Life = gObj[UserIndex].MaxLife;
		gObj[UserIndex].Mana = gObj[UserIndex].MaxMana;
		gObjNextExpCal(&gObj[UserIndex]);
		gObjSetBP(gObj[UserIndex].m_Index);
		GSProtocol.GCLevelUpMsgSend(gObj[UserIndex].m_Index, 1);
		gObjCalcMaxLifePower(gObj[UserIndex].m_Index);
		LogAddTD(Lang.GetText(0, 132), gObj[UserIndex].AccountID, gObj[UserIndex].Name, gObj[UserIndex].Level);
		return 0;
	}
	return iLEFT_EXP;
}

void CIllusionTemple::DeleteInvitation(int IllusionTempleIndex)
{
	for(int i=0;i<MAX_ILLUSION_TEMPLE_USER;++i)
	{
		if(this->m_IllusionTempleData[IllusionTempleIndex].WaitingPlayers[i].UserIndex == -1)
			return;

		if(this->m_IllusionTempleData[IllusionTempleIndex].WaitingPlayers[i].ItemPos == -1)
			return;

		CItem Item = gObj[this->m_IllusionTempleData[IllusionTempleIndex].WaitingPlayers[i].UserIndex].pInventory[this->m_IllusionTempleData[IllusionTempleIndex].WaitingPlayers[i].ItemPos];

		if(Item.m_Type == ITEMGET(13,51))
		{
			gObjInventoryDeleteItem(this->m_IllusionTempleData[IllusionTempleIndex].WaitingPlayers[i].UserIndex, this->m_IllusionTempleData[IllusionTempleIndex].WaitingPlayers[i].ItemPos);
			GSProtocol.GCInventoryItemDeleteSend(this->m_IllusionTempleData[IllusionTempleIndex].WaitingPlayers[i].UserIndex,
				this->m_IllusionTempleData[IllusionTempleIndex].WaitingPlayers[i].ItemPos, 1);
		}
		else if(Item.m_Type == ITEMGET(13,61) && Item.m_Durability < 1.0f)
		{
			gObjInventoryDeleteItem(this->m_IllusionTempleData[IllusionTempleIndex].WaitingPlayers[i].UserIndex, this->m_IllusionTempleData[IllusionTempleIndex].WaitingPlayers[i].ItemPos);
			GSProtocol.GCInventoryItemDeleteSend(this->m_IllusionTempleData[IllusionTempleIndex].WaitingPlayers[i].UserIndex,
				this->m_IllusionTempleData[IllusionTempleIndex].WaitingPlayers[i].ItemPos, 1);
		}

		else
		{
			LogAddC(2, "[Illusion Temple] (%d) ERROR: CIllusionTemple::DeleteInvitation() return",
				IllusionTempleIndex+1);
		}

	}
}

bool CIllusionTemple::InvitationUsedCheck(LPOBJ lpObj, int ItemPos)
{
	int IllusionTempleIndex = lpObj->m_cIllusionTempleIndex;
	if(IllusionTempleIndex == -1) return false;
	int IllusionTempleSubIndex = lpObj->m_cIllusionTempleSubIndex;
	if(IllusionTempleSubIndex == -1) return false;

	if(this->m_IllusionTempleData[IllusionTempleIndex].WaitingPlayers[IllusionTempleSubIndex].UserIndex == lpObj->m_Index)
	{
		if(this->m_IllusionTempleData[IllusionTempleIndex].WaitingPlayers[IllusionTempleSubIndex].ItemPos == ItemPos)
		{
			return true;
		}
	}
	return false;
}

void CIllusionTemple::Payback(int aIndex)
{
	int IllusionTempleIndex = gObj[aIndex].m_cIllusionTempleIndex;
	if(IllusionTempleIndex == -1) return;
	int IllusionTempleSubIndex = gObj[aIndex].m_cIllusionTempleSubIndex;
	if(IllusionTempleSubIndex == -1) return;

	gObj[aIndex].pInventory[this->m_IllusionTempleData[IllusionTempleIndex].WaitingPlayers[IllusionTempleSubIndex].ItemPos].m_Durability += 1.0f;
	GSProtocol.GCItemDurSend2(aIndex, this->m_IllusionTempleData[IllusionTempleIndex].WaitingPlayers[IllusionTempleSubIndex].ItemPos, gObj[aIndex].pInventory[this->m_IllusionTempleData[IllusionTempleIndex].WaitingPlayers[IllusionTempleSubIndex].ItemPos].m_Durability, 0);
}

void CIllusionTemple::SkillShield(int aIndex)
{
	if(gObj[aIndex].m_btKillCount < 5)
	{
		this->SendSkillUse(aIndex, aIndex, AT_SKILL_ILLUSION_SHIELD, 0);
	}
	else
	{
		this->DecKillPoint(gObj[aIndex].m_cIllusionTempleIndex, aIndex, 5);
		BuffEffectC.EnableBuff(aIndex, EFFECT_ILLUSION_SHIELD, 15, 0, 0, 0, 0);
		this->SendSkillUse(aIndex, aIndex, AT_SKILL_ILLUSION_SHIELD, 1);
	}
}

void CIllusionTemple::SkillParalyze(int aIndex, int aTargetIndex)
{
	if(gObj[aIndex].m_btKillCount < 5)
	{
		this->SendSkillUse(aIndex, aIndex, AT_SKILL_ILLUSION_PARALYZE, 0);
	}
	if(gCheckSkillDistance(aIndex, aTargetIndex, AT_SKILL_ILLUSION_PARALYZE) == true)
	{
		BuffEffectC.EnableBuff(aTargetIndex, EFFECT_ILLUSIONPARALYZE, 15, 0, 0, 0, 0);
		this->DecKillPoint(gObj[aIndex].m_cIllusionTempleIndex, aIndex, 5);
		this->SendSkillUse(aIndex, aTargetIndex, AT_SKILL_ILLUSION_PARALYZE, 1);
	}
	else
	{
		this->SendSkillUse(aIndex, aTargetIndex, AT_SKILL_ILLUSION_PARALYZE, 0);
	}
}

void CIllusionTemple::SkillTeleport(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->m_btKillCount < 5)
	{
		this->SendSkillUse(aIndex, aIndex, AT_SKILL_ILLUSION_TELEPORT, 0);
	}

	if(BuffEffectC.BuffSearch(lpObj, EFFECT_STERN) || BuffEffectC.BuffSearch(lpObj, EFFECT_SLEEP))
	{
		this->SendSkillUse(aIndex, aIndex, AT_SKILL_ILLUSION_TELEPORT, 0);
		return;
	}

	if(this->m_IllusionTempleData[gObj[aIndex].m_cIllusionTempleIndex].HeroIndex == -1)
	{
		this->SendSkillUse(aIndex, aIndex, AT_SKILL_ILLUSION_TELEPORT, 0);
		return;
	}

	if(this->m_IllusionTempleData[gObj[aIndex].m_cIllusionTempleIndex].HeroIndex == aIndex)
	{
		this->SendSkillUse(aIndex, aIndex, AT_SKILL_ILLUSION_TELEPORT, 0);
		return;
	}

	LPOBJ lpHeroObj = &gObj[this->m_IllusionTempleData[gObj[aIndex].m_cIllusionTempleIndex].HeroIndex];

	this->DecKillPoint(gObj[aIndex].m_cIllusionTempleIndex, aIndex, 10);
	this->SendSkillUse(aIndex, aIndex, AT_SKILL_ILLUSION_TELEPORT, 1);
	
	short X = 0;
	short Y = 0;
	int loopcount = 50;
	bool teleport = false;

	while ( loopcount-- != 0 )
	{
		X = lpHeroObj->X + rand()%3 -1;
		Y = lpHeroObj->Y + rand()%3 -1;

		if(gObjCheckTeleportArea(aIndex, X, Y) == 0)
		{
			teleport = true;
			break;
		}
	}
	if(teleport = false)
	{
		X = lpHeroObj->X;
		Y = lpHeroObj->Y;
	}
	gObjTeleportMagicUse(aIndex, X, Y);
}

void CIllusionTemple::SkillAttack(int aIndex, int aTargetIndex)
{
	if(gObj[aIndex].m_btKillCount < 5)
	{
		this->SendSkillUse(aIndex, aIndex, AT_SKILL_ILLUSION_ATTACK, 0);
	}
	if(gCheckSkillDistance(aIndex, aTargetIndex, AT_SKILL_ILLUSION_ATTACK) == true)
	{
		int Damage = gObj[aTargetIndex].iShield / 2;
		gObj[aTargetIndex].iShield = Damage;
		GSProtocol.GCDamageSend(aIndex, aTargetIndex, 0, 0, 0, Damage);
		this->DecKillPoint(gObj[aIndex].m_cIllusionTempleIndex, aIndex, 5);
		this->SendSkillUse(aIndex, aTargetIndex, AT_SKILL_ILLUSION_ATTACK, 1);
	}
	else
	{
		GSProtocol.GCDamageSend(aIndex, aTargetIndex, 0, 0, 0, 0);
		this->SendSkillUse(aIndex, aTargetIndex, AT_SKILL_ILLUSION_ATTACK, 0);
	}
}

void CIllusionTemple::SendSkillUse(int aIndex, int aTargetIndex, WORD SkillID, BYTE skillsuccess)
{
	PMSG_ILLUSION_ENABLESKILL pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xBF, 0x02, sizeof(pMsg));

	pMsg.aIndex = aIndex;
	pMsg.TargetIndex = aTargetIndex;
	pMsg.SkillID = SkillID;
	pMsg.SkillSuccess = skillsuccess;

	GSProtocol.MsgSendV2(&gObj[aIndex], reinterpret_cast<LPBYTE>(&pMsg), pMsg.h.size);

	if(gObj[aIndex].Type == OBJ_USER)
		IOCP.DataSend(aIndex, reinterpret_cast<LPBYTE>(&pMsg), pMsg.h.size);
}



//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

