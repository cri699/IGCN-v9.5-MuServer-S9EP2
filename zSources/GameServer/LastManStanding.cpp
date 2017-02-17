//////////////////////////////////////////////////////////////////////
// LastManStanding.cpp
#include "StdAfx.h"
#include "LastManStanding.h"
#include "user.h"
#include "TLog.h"
#include "TNotice.h"
#include "configread.h"
#include "GameMain.h"
#include "BagManager.h"

/* MAP INFO
146,78 kreisais augsejais sturis
146,60 kreisais apaksejais sturis

160,60 labais apaksejais sturis
160, 78 labais augsejais sturis
*/
CLastManStanding g_LastManStanding;
time_t rawtime;
struct tm * timeinfo;
CLastManStanding::CLastManStanding()
{
	memset(this->m_Rooms,0x00,sizeof(this->m_Rooms));
	
	for(int i=0;i<LMS_MAX_ROOMS;i++){
		m_Rooms[i].m_iEventCoolDown = 260;
		m_Rooms[i].bState = 0;
		bTimerSent = false;
	}
	this->m_vtStartList.clear();
	this->iRegNpcIndex = -1;
}

CLastManStanding::~CLastManStanding()
{

}

void CLastManStanding::LoadConfig(LPSTR lpszFileName)
{
	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(lpszFileName);

	if (res.status != pugi::status_ok)
	{
		g_Log.MsgBox("[PVP_LastManStanding] - Can't Load %s (%s)", lpszFileName, res.description());
		return;
	}

	pugi::xml_node main = file.child("LastManStanding");

	this->m_Cfg.bEnable = main.attribute("Enable").as_int();
	this->m_Cfg.iDieCount = main.attribute("DieCount").as_int();
	this->m_Cfg.iMinPlayers = main.attribute("MinPlayers").as_int();
	this->m_Cfg.iMaxPlayers = main.attribute("MaxPlayers").as_int();

	pugi::xml_node event_cfg = main.child("Event");
	
	this->m_Cfg.iDuration	= event_cfg.attribute("Duration").as_int();
	this->m_Cfg.iPVPMap		= event_cfg.attribute("MapNumber").as_int();
	this->m_Cfg.iPVPMapX1	= event_cfg.attribute("StartX").as_int();
	this->m_Cfg.iPVPMapY1	= event_cfg.attribute("StartY").as_int();
	this->m_Cfg.iPVPMapX2	= event_cfg.attribute("EndX").as_int();
	this->m_Cfg.iPVPMapY2	= event_cfg.attribute("EndY").as_int();

	pugi::xml_node register_cfg = main.child("Registration");

	this->m_Cfg.iRegTime		= register_cfg.attribute("Time").as_int(15);
	this->m_Cfg.iNPCId			= register_cfg.attribute("NPCIndex").as_int();
	this->m_Cfg.iRegNPCDir		= register_cfg.attribute("Dir").as_int();
	this->m_Cfg.iRegNPCMap		= register_cfg.attribute("MapNumber").as_int();
	this->m_Cfg.iRegNPCX		= register_cfg.attribute("PosX").as_int();
	this->m_Cfg.iRegNPCY		= register_cfg.attribute("PosY").as_int();

	this->m_Cfg.iReqItem		= register_cfg.attribute("ReqSpecialItem").as_int();
	this->m_Cfg.iReqItemCat		= register_cfg.attribute("ItemCat").as_int();
	this->m_Cfg.iReqItemIdx		= register_cfg.attribute("ItemIndex").as_int();
	this->m_Cfg.iReqItemCount	= register_cfg.attribute("SpecialItemCount").as_int();
	this->m_Cfg.iReqZen			= register_cfg.attribute("ReqMoney").as_int();

	pugi::xml_node schedule = main.child("Schedule");

	for (pugi::xml_node start = schedule.child("Start"); start; start = start.next_sibling())
	{
		sLMS_START_TIME sTime;

		sTime.StartHour = start.attribute("Hour").as_int();
		sTime.StartMinute = start.attribute("Minute").as_int();

		this->m_vtStartList.push_back(sTime);
	}

	pugi::xml_node rooms_cfg = main.child("RoomSettings");
	int count = 0;

	for (pugi::xml_node room = rooms_cfg.child("Room"); room; room = room.next_sibling())
	{
		this->m_Rooms[count].MinReset	= room.attribute("MinReset").as_int();
		this->m_Rooms[count].MaxReset	= room.attribute("MaxReset").as_int();
		this->m_Rooms[count].Map		= room.attribute("MapNumber").as_int();
		this->m_Rooms[count].X1			= room.attribute("StartX").as_int();
		this->m_Rooms[count].Y1			= room.attribute("StartY").as_int();
		this->m_Rooms[count].X2			= room.attribute("EndX").as_int();
		this->m_Rooms[count].Y2			= room.attribute("EndY").as_int();

		count++;
	}

	this->m_iRoomCount = count;


}

void CLastManStanding::SpawnRegNPC()
{
	if(g_ConfigRead.server.GetServerType() != SERVER_NORMAL)
	{
		return;
	}

	if(this->iRegNpcIndex != -1)
	{
		gObjDel(this->iRegNpcIndex);
	}

	this->iRegNpcIndex = gObjAddMonster(this->m_Cfg.iRegNPCMap);

	if (this->iRegNpcIndex == -1)
	{
		g_Log.AddC(TColor::Red, "[PVP_LastManStanding] Failed to add Registration NPC");
		return;
	}

	LPOBJ lpNpc = &gObj[this->iRegNpcIndex];
	lpNpc->Live = TRUE;
	lpNpc->Dir = this->m_Cfg.iRegNPCDir;
	lpNpc->X = this->m_Cfg.iRegNPCX;
	lpNpc->Y = this->m_Cfg.iRegNPCY;
	lpNpc->TX = lpNpc->X;
	lpNpc->TY = lpNpc->Y;
	lpNpc->m_OldX = lpNpc->X;
	lpNpc->m_OldY = lpNpc->Y;

	gObjSetMonster(this->iRegNpcIndex, this->m_Cfg.iNPCId);

	g_Log.Add("[PVP_LastManStanding] Registration NPC Added [%d][%d][%d]",lpNpc->MapNumber,lpNpc->X,lpNpc->Y);
}

void CLastManStanding::Init()
{
	if (g_ConfigRead.server.GetStateFromEventTable(g_ConfigRead.server.GetServerType(), EV_TABLE_LMS) == false)
	{
		this->m_Cfg.bEnable = false;
	}
}

int CLastManStanding::AddUser(int aIndex)
{
	if (!ObjectMaxRange(aIndex))
		return -1;

	if (!gObjIsConnectedGP(aIndex))
		return -1;

	int resets = gObj[aIndex].m_PlayerData->m_iResets;

	for (int i = 0; i < m_iRoomCount; i++)
	{
		if (resets >= this->m_Rooms[i].MinReset  && resets <= this->m_Rooms[i].MaxReset && gObj[aIndex].Level > 6) 
		{
			if (this->m_Rooms[i].regCount >= this->m_Cfg.iMaxPlayers)
			{
				MsgOutput(aIndex, Lang.GetText(0,355));
				return -1;
			}

			if (this->m_Rooms[i].regCount >= LMS_MAX_USER_PER_ROOM) 
			{
				MsgOutput(aIndex, Lang.GetText(0,355));
				return -1;
			}

			for (int n = 0; n < LMS_MAX_USER_PER_ROOM; n++)
			{
				if (this->m_Rooms[i].m_Data[n].iIndex == 0)
				{
					this->m_Rooms[i].m_Data[n].iIndex = aIndex;
					this->m_Rooms[i].m_Data[n].iScore = 0;
					this->m_Rooms[i].m_Data[n].bWinner = false;
					this->m_Rooms[i].m_Data[n].iDeaths = 0;
					gObj[aIndex].m_PlayerData->RegisterdLMS = 1;
					gObj[aIndex].m_PlayerData->RegisteredLMSRoom = i;
					this->m_Rooms[i].regCount++;
					g_Log.Add("[PVP_LastManStanding][%s][%s] Register on room [%d]", gObj[aIndex].AccountID, gObj[aIndex].Name, i);
					return true;
				}
			}
		}
	}

	MsgOutput(aIndex, Lang.GetText(0,356));
	return -1;
}

void CLastManStanding::DelUser(int aindex)
{
	if(aindex < g_ConfigRead.server.GetObjectStartUserIndex())
	{
		g_Log.Add("[PVP_LastManStanding] Invalid index %d", aindex);
		return;
	}

	int room = gObj[aindex].m_PlayerData->RegisteredLMSRoom;
	for(int i=0;i<LMS_MAX_USER_PER_ROOM;i++)
	{
		if(this->m_Rooms[room].m_Data[i].iIndex == aindex)
		{
			this->m_Rooms[room].m_Data[i].iIndex = 0;
			this->m_Rooms[room].m_Data[i].iScore = 0;
			this->m_Rooms[room].m_Data[i].bWinner = false;
			this->m_Rooms[room].m_Data[i].iDeaths = 0;
			gObj[aindex].m_PlayerData->RegisterdLMS = 0;
			gObj[aindex].m_PlayerData->RegisteredLMSRoom = -1;
			this->m_Rooms[room].m_iLiveUser--;
			gObjMoveGate(aindex,17);
			g_Log.Add("[PVP_LastManStanding] Room [%d] Delete user [%s]",i, gObj[aindex].Name);
			return;
		}
	}
	g_Log.Add("[PVP_LastManStanding]  DelUser() user not found... %d ",aindex);
}

void CLastManStanding::UserQuit(int aindex)
{
		if(aindex < g_ConfigRead.server.GetObjectStartUserIndex())
	{
		g_Log.Add("[PVP_LastManStanding] Invalid index %d", aindex);
		return;
	}

	int room = gObj[aindex].m_PlayerData->RegisteredLMSRoom;
	for(int i=0;i<LMS_MAX_USER_PER_ROOM;i++)
	{
		if(this->m_Rooms[room].m_Data[i].iIndex == aindex)
		{
			this->m_Rooms[room].m_Data[i].iIndex = 0;
			this->m_Rooms[room].m_Data[i].iScore = 0;
			this->m_Rooms[room].m_Data[i].bWinner = false;
			this->m_Rooms[room].m_Data[i].iDeaths = 0;
			gObj[aindex].m_PlayerData->RegisterdLMS = 0;
			gObj[aindex].m_PlayerData->RegisteredLMSRoom = -1;

			if(this->m_Rooms[room].bState == 1)
			{
				this->m_Rooms[room].regCount--; // decrease only if in registration period
			}
			g_Log.Add("[PVP_LastManStanding] Room [%d] Delete user [%s]",i, gObj[aindex].Name);
			return;
		}
	}
	g_Log.Add("[PVP_LastManStanding]  UserQuit() user not found... %d ",aindex);
}

bool bTIME_TO_START = false;
WORD m_wCurrentMin = 0;
int StartMinute = 0;

void CLastManStanding::Run()
{
	if(this->m_Cfg.bEnable == false)
	{
		return;
	}

  time ( &rawtime );
  timeinfo = localtime ( &rawtime );
  m_wCurrentMin = timeinfo->tm_hour * 60 + timeinfo->tm_min;
  if(!bTIME_TO_START)
  {
	   for(std::vector<sLMS_START_TIME>::iterator it = this->m_vtStartList.begin(); it != this->m_vtStartList.end(); it++)
		{
			if(it->StartHour == timeinfo->tm_hour && it->StartMinute == timeinfo->tm_min)
			{
				bTIME_TO_START = true;
				StartMinute = it->StartHour * 60 + it->StartMinute;
				break;
			}
	   }
  }
  if(bTIME_TO_START)
  {
	  for(int n=0;n<this->m_iRoomCount;n++)
	  {
		  if(m_wCurrentMin == StartMinute && m_Rooms[n].bState == 0)
		  {
			  if(n==0)
			  {
				  PMSG_NOTICE pNotice;
				  TNotice::MakeNoticeMsgEx(&pNotice, 0, Lang.GetText(0,357), Lang.GetMap(0, this->m_Cfg.iRegNPCMap), this->m_Cfg.iRegTime);
				  TNotice::SendNoticeToAllUser(&pNotice);
			  }
			  m_Rooms[n].bState = 1;
			  
		  
		  }
		  if(m_wCurrentMin == StartMinute + this->m_Cfg.iRegTime && m_Rooms[n].bState == 1)
		  {
			  if(n==0)
			  {
				  PMSG_NOTICE pNotice;
				  TNotice::MakeNoticeMsgEx(&pNotice, 0, Lang.GetText(0,358), Lang.GetMap(0, this->m_Cfg.iRegNPCMap));
				  TNotice::SendNoticeToAllUser(&pNotice);
			  }
			  
			  if (m_Rooms[n].regCount >= this->m_Cfg.iMinPlayers)
			  {
				m_Rooms[n].bState = 2;
				this->MoveAllUser(n);
				g_Log.Add("[PVP_LastManStanding] Room [%d] - State = 2",n);
				g_Log.Add("[PVP_LastManStanding] Move All users Room[%d]",n);
			  }
			  else
			  {
				  this->EndEvent(n);
				  g_Log.Add("[PVP_LastManStanding] Room [%d] - NOT ENOUGH USER",n);
			  }
		  }
		/*  if(timeinfo->tm_min == LMS_OPENMINUTE+2 && timeinfo->tm_sec >= 30)
		  {
			  if(!this->bTimerSent)
			  {
				PMSG_SET_DEVILSQUARE pMsg;

				PHeadSetB((LPBYTE)&pMsg, 0x92, sizeof(pMsg));
				pMsg.Type = 17;

				this->SendToPlayers((LPBYTE)&pMsg, pMsg.h.size);
				this->bTimerSent = true;
			  }
		  }*/
		  if(m_wCurrentMin == StartMinute + this->m_Cfg.iRegTime + 1 && m_Rooms[n].bState == 2)
		  {
			  if(n==0)
			  {
				  PMSG_NOTICE pNotice;
				  TNotice::MakeNoticeMsgEx(&pNotice, 0, Lang.GetText(0,359), Lang.GetMap(0, this->m_Cfg.iRegNPCMap));
				  TNotice::SendNoticeToAllUser(&pNotice);
			  }
			  m_Rooms[n].bState = 3;
			  g_Log.Add("[PVP_LastManStanding] Room [%d] - State = 3",n);
		  }

		  if(m_Rooms[n].bState == 3)
		  {
				  for(int i=0;i<m_Rooms[n].regCount;i++)
				  {
					  if(m_Rooms[n].m_Data[i].iIndex != 0 && gObj[m_Rooms[n].m_Data[i].iIndex].MapNumber != this->m_Cfg.iPVPMap)
					  {
						  g_Log.Add("[PVP_LastManStanding] User %s left the map.",gObj[m_Rooms[n].m_Data[i].iIndex].Name);
						  this->DelUser(m_Rooms[n].m_Data[i].iIndex);
						  if(m_Rooms[n].m_iLiveUser <= 1)
						  {
							for(int i=0;i<m_Rooms[n].regCount;i++)
							{
								if(this->m_Rooms[n].m_Data[i].iIndex != 0)
								{
									if(m_Rooms[n].m_Data[i].iScore > 0)
									{
										this->RewardUser(m_Rooms[n].m_Data[i].iIndex);
										char text[200]; 

										sprintf(text, Lang.GetText(0,360), gObj[this->m_Rooms[n].m_Data[i].iIndex].Name, n, this->m_Rooms[n].m_Data[i].iScore);						
										AllPlayerMsgSend(text);
									}
								}
							}
							g_Log.Add("[PVP_LastManStanding] Room [%d] - State = 4",n);
							this->m_Rooms[n].bState = 4 ;
						  }
					  }
				  }		  
		  }
		  if(m_wCurrentMin > StartMinute + this->m_Cfg.iRegTime + this->m_Cfg.iDuration + 1 && m_Rooms[n].bState == 3)
		  {
				  PMSG_NOTICE pNotice;
				  TNotice::MakeNoticeMsgEx(&pNotice, 0, Lang.GetText(0,348), Lang.GetMap(0, this->m_Cfg.iRegNPCMap));
				  TNotice::SendNoticeToAllUser(&pNotice);	
				  g_Log.Add("[PVP_LastManStanding] Room [%d] - State = OVER_TIME",n);
				this->EndEvent(n);
		  }
		  if(m_Rooms[n].bState == 4)
		  {
			  this->m_Rooms[n].m_iEventCoolDown--;
			  // g_Log.Add("[PVP Last Man Standing][Debug] Event cool down %d sec.", this->m_iEventCoolDown);		
			  // g_Log.Add("[PVP_LastManStanding] Room [%d] - State = COOL_DOWN",n);
			  if(this->m_Rooms[n].m_iEventCoolDown == 0)
			  {
				  this->EndEvent(n);
			  }
		  }
	  }
  }
}

void CLastManStanding::MoveAllUser(int Room)
{
	if(this->m_Rooms[Room].bState == 2){
		for(int i=0;i<m_Rooms[Room].regCount;i++)
		{
			if(this->m_Rooms[Room].m_Data[i].iIndex > 0)
			{
				if(gObjIsConnectedGP(this->m_Rooms[Room].m_Data[i].iIndex))
				{
					this->m_Rooms[Room].m_iLiveUser++;
					int xrand = this->m_Rooms[Room].X2 - this->m_Rooms[Room].X1;
					int yrand = this->m_Rooms[Room].Y2 - this->m_Rooms[Room].Y1;
					gObjTeleport(this->m_Rooms[Room].m_Data[i].iIndex, m_Rooms[Room].Map, m_Rooms[Room].X1+rand()%xrand, m_Rooms[Room].Y1+rand()%yrand);
					g_Log.Add("[PVP_LastManStanding] Room [%d] Move all user",Room);
				}
				else
				{
					this->DelUser(m_Rooms[Room].m_Data[i].iIndex);
					g_Log.Add("[PVP_LastManStanding] Room [%d] Move user fail, user is offline",Room);
				}
				Sleep(100);
			}
		}
	}
	/*	for(int n =0;n<LMS_MAX_ROOMS;n++)
		{
			if(this->m_Rooms[n].bState == 2){
				for(int i=0;i<m_Rooms[n].regCount;i++)
				{
					if(this->m_Rooms[n].m_Data[i].iIndex > 0)
					{
						if(gObjIsConnectedGP(this->m_Rooms[n].m_Data[i].iIndex))
						{
							this->m_Rooms[n].m_iLiveUser++;
							int xrand = this->m_Rooms[n].X2 - this->m_Rooms[n].X1;
							int yrand = this->m_Rooms[n].Y2 - this->m_Rooms[n].Y1;
							gObjTeleport(this->m_Rooms[n].m_Data[i].iIndex, m_Rooms[n].Map, m_Rooms[n].X1+rand()%xrand, m_Rooms[n].Y1+rand()%yrand);
							g_Log.Add("[PVP_LastManStanding] Room [%d] Move all user",n);
						}
						else
						{
							this->DelUser(m_Rooms[n].m_Data[i].iIndex);
							g_Log.Add("[PVP_LastManStanding] Room [%d] Move user fail, user is offline",n);
						}
						Sleep(100);
					}
				}
		}
	}*/
}

void CLastManStanding::EndEvent(int room)
{
	this->m_Rooms[room].bState = 0;
	this->m_Rooms[room].regCount = 0;
	bTimerSent = false;
	for(int i=0;i<m_Rooms[room].regCount;i++)
	{
		this->DelUser(m_Rooms[room].m_Data[i].iIndex);
	}
	

	for(int i=0;i<this->m_iRoomCount;i++)
	{
		if(this->m_Rooms[i].bState != 0){
			g_Log.Add("[PVP_LastManStanding] Room [%d] bState != 0",i);
			return;
		}
	}
	g_Log.Add("[PVP_LastManStanding] Room [%d] Event end",room);
	bTIME_TO_START = false;
	StartMinute = 0;
}

void CLastManStanding::UserDie(int aIndex, int aTargetIndex)
{
	int room = gObj[aIndex].m_PlayerData->RegisteredLMSRoom;
	for(int i=0;i<this->m_Rooms[room].regCount;i++)
	{
		if(this->m_Rooms[room].m_Data[i].iIndex == aIndex)
		{
			
			this->m_Rooms[room].m_Data[i].iDeaths++;
			g_Log.Add("[PVP_LastManStanding] Room [%d] [%d] Killed [%d] Death[%d] [%s]",room,aTargetIndex,aIndex,this->m_Rooms[room].m_Data[i].iDeaths,gObj[this->m_Rooms[room].m_Data[i].iIndex].Name);
			if(this->m_Rooms[room].m_Data[i].iDeaths == this->m_Cfg.iDieCount)
			{
				MsgOutput(aIndex, Lang.GetText(0,361));
				this->DelUser(aIndex);
				if(m_Rooms[room].m_iLiveUser == 1)
				{
					for(int i=0;i<m_Rooms[room].regCount;i++)
					{
						if(this->m_Rooms[room].m_Data[i].iIndex != 0)
						{
							if(m_Rooms[room].m_Data[i].iScore > 0)
							{
								this->m_Rooms[room].m_Data[i].bWinner = true;
								char text[200]; 
								sprintf(text, Lang.GetText(0,362), gObj[this->m_Rooms[room].m_Data[i].iIndex].Name, room, this->m_Rooms[room].m_Data[i].iScore);
								this->RewardUser(this->m_Rooms[room].m_Data[i].iIndex);
								g_Log.Add("[PVP_LastManStanding] Room [%d] reward user [%s]",room,gObj[this->m_Rooms[room].m_Data[i].iIndex].Name);
								AllPlayerMsgSend(text);
							}
								this->m_Rooms[room].bState =4 ;
							
						}
					}
				}
				
			}
		}
		else if(this->m_Rooms[room].m_Data[i].iIndex == aTargetIndex)
		{
			this->m_Rooms[room].m_Data[i].iScore++;
			MsgOutput(aTargetIndex, Lang.GetText(0,363), this->m_Rooms[room].m_Data[i].iScore);
			g_Log.Add("[PVP_LastManStanding] Room [%d] [%d] Killed [%d] Score [%d] [%s]",room,aIndex, aTargetIndex,this->m_Rooms[room].m_Data[i].iScore,gObj[this->m_Rooms[room].m_Data[i].iIndex].Name);

		}
	}
}

void CLastManStanding::AllPlayerMsgSend( char* chatmsg)
{
	PMSG_NOTICE pNotice;
	
	TNotice::MakeNoticeMsg((TNotice*)&pNotice, 0x00,  chatmsg);

	for ( int n = g_ConfigRead.server.GetObjectStartUserIndex() ; n < g_ConfigRead.server.GetObjectMax() ; n++)
	{
		if ( gObj[n].Connected == PLAYER_PLAYING && gObj[n].m_PlayerData->RegisterdLMS == 1 )
		{
			if ( gObj[n].Type == OBJ_USER )
			{
				IOCP.DataSend(n, (unsigned char*)&pNotice , pNotice.h.size );
			}
		}
	}
}

void CLastManStanding::RewardUser(int aIndex)
{
	g_BagManager.SearchAndUseBag(aIndex, BAG_EVENT, EVENTBAG_LMS, aIndex);
}

void CLastManStanding::SendToPlayers(LPBYTE lpMsg, int iSize)
{
	for(int i=0;i<this->m_iRoomCount;++i)
	{
		for(int n=0;n<this->m_Rooms[i].m_iLiveUser;n++)
		{
			IOCP.DataSend(this->m_Rooms[i].m_Data[n].iIndex, lpMsg, iSize);
		}
	}
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

