//////////////////////////////////////////////////////////////////////
// GMMng.cpp
#include "stdafx.h"
#include "GMMng.h"
#include "LogToFile.h"
#include "TLog.h"
#include "GameMain.h"
#include "protocol.h"
#include "winutil.h"
#include "MoveCommand.h"
#include "GuildClass.h"
#include "TNotice.h"
#include "ObjCalCharacter.h"
#include "BattleSoccerManager.h"
#include "giocp.h"
#include "Kanturu.h"
#include "SkillAdditionInfo.h"
#include "RaklionBattleUserMng.h"
#include "BuffEffectSlot.h"
#include "AntiSwear.h"
#include "Marry.h"
#include "MapServerManager.h"
#include "DSProtocol.h"
#include "Crywolf.h"
#include "BloodCastle.h"
#include "DevilSquare.h"
#include "ChaosCastle.h"
#include "configread.h"
#include "PentagramSystem.h"
#include "GensSystem.h"
#include "ImperialGuardian.h"
#include "MasterLevelSkillTreeSystem.h"
#include "ItemSocketOptionSystem.h"
#include "SendNPCInfo.h"
#include "VipSys.h"
#include "SProtocol.h"
#include "MuunSystem.h"
#include "ItemOptionTypeMng.h"
#include "OfflineLevelling.h"
#include "ChaosCastleFinal.h"
#include "QuestExpProgMng.h"
#include "ChangeCmd.h"
#include "UpgradeCmd.h"

CGMMng cManager;
static CLogToFile * CommandLog;

CGMMng::CGMMng()
{
	this->ManagerInit();
}

CGMMng::~CGMMng()
{
	delete CommandLog;
	return;
}

void CGMMng::LoadCommandFile(LPSTR file)
{
	GetPrivateProfileString("commands", "PostCMD", "/post", m_CommandNames.PostCMD, 20, file);
	GetPrivateProfileString("commands", "ItemCMD", "/item", m_CommandNames.ItemCMD, 20, file);
	GetPrivateProfileString("commands", "AddSTRCMD", "/addstr", m_CommandNames.AddSTRCMD, 20, file);
	GetPrivateProfileString("commands", "AddAGICMD", "/addagi", m_CommandNames.AddAGICMD, 20, file);
	GetPrivateProfileString("commands", "AddVITCMD", "/addvit", m_CommandNames.AddVITCMD, 20, file);
	GetPrivateProfileString("commands", "AddENECMD", "/addene", m_CommandNames.ADDENECMD, 20, file);
	GetPrivateProfileString("commands", "AddCMDCMD", "/addcmd", m_CommandNames.ADDCMDCMD, 20, file);
	GetPrivateProfileString("commands", "OnlineCMD", "/online", m_CommandNames.ONLINECMD, 20, file);
	GetPrivateProfileString("commands", "PKClearCMD", "/pkclear", m_CommandNames.PKCLEARCMD, 20, file);
	GetPrivateProfileString("commands", "WareCMD", "/ware", m_CommandNames.WARECMD, 20, file);
	GetPrivateProfileString("commands", "MarryProposeCMD", "/prop", m_CommandNames.PROPOSECMD, 20, file);
	GetPrivateProfileString("commands", "MarryAcceptCMD", "/accept", m_CommandNames.ACCEPTCMD, 20, file);
	GetPrivateProfileString("commands", "MarryTeleportCMD", "/huwi", m_CommandNames.TRACEMARRYCMD, 20, file);
	GetPrivateProfileString("commands", "SkinCMD", "/skin", m_CommandNames.SKINCMD, 20, file);
	GetPrivateProfileString("commands", "ForceBCStart", "/bcstart", m_CommandNames.BCStart, 20, file);
	GetPrivateProfileString("commands", "ForceDSStart", "/DSstart", m_CommandNames.DSStart, 20, file);
	GetPrivateProfileString("commands", "ForceCCStart", "/CCstart", m_CommandNames.CCStart, 20, file);
	GetPrivateProfileString("commands", "ForceITStart", "/ITstart", m_CommandNames.ITStart, 20, file);
	GetPrivateProfileString("commands", "WARCMD", "/war", m_CommandNames.WARCMD, 20, file);
	GetPrivateProfileString("commands", "FireWorks", "/fw", m_CommandNames.FireWorks, 20, file);
	GetPrivateProfileString("commands", "WatchCMD", "/watch", m_CommandNames.Watch, 20, file);
	GetPrivateProfileString("commands", "TraceCMD", "/trace", m_CommandNames.Trace, 20, file);
	GetPrivateProfileString("commands", "DisconnectCMD", "/dc", m_CommandNames.DC, 20, file);
	GetPrivateProfileString("commands", "GuildDCCMD", "/gdc", m_CommandNames.GuildDC, 20, file);
	GetPrivateProfileString("commands", "MoveCMD", "/move", m_CommandNames.MoveCMD, 20, file);
	GetPrivateProfileString("commands", "MoveGuild", "/moveguild", m_CommandNames.gMoveCMD, 20, file);
	GetPrivateProfileString("commands", "StartSoccer", "/startsoccer", m_CommandNames.StartSoccer, 20, file);
	GetPrivateProfileString("commands", "StopSoccer", "/stopsoccer", m_CommandNames.EndSoccer, 20, file);
	GetPrivateProfileString("commands", "EndWar", "/endwar", m_CommandNames.EndWar, 20, file);
	GetPrivateProfileString("commands", "ChatBan", "/cb", m_CommandNames.ChatBan, 20, file);
	GetPrivateProfileString("commands", "UnbanChat", "/ucb", m_CommandNames.ChatUnban, 20, file);
	GetPrivateProfileString("commands", "SetGW", "/setgw", m_CommandNames.SetGW, 20, file);
	GetPrivateProfileString("commands", "SoccerStart", "/battlesoccer", m_CommandNames.SoccerStart, 20, file);
	GetPrivateProfileString("commands", "SetRequest", "/requests", m_CommandNames.Requests, 20, file);
	GetPrivateProfileString("commands", "SendKundunInfo", "/kundunhp", m_CommandNames.KundunInfo, 20, file);
	GetPrivateProfileString("commands", "KundunSetLife", "/kundunsetlife", m_CommandNames.SetKundunHP, 20, file);
	GetPrivateProfileString("commands", "ResetCMD", "/reset", m_CommandNames.ResetCMD, 20, file);
	GetPrivateProfileString("commands", "SvInfoCMD", "/svinfo", m_CommandNames.SvInfoCMD, 20, file);
	GetPrivateProfileString("commands", "CharInfoCMD", "/charinfo", m_CommandNames.CharInfoCMD, 20, file);
	GetPrivateProfileString("commands", "HideCMD", "/hide", m_CommandNames.HideCMD, 20, file);
	GetPrivateProfileString("commands", "UnHideCMD", "/unhide", m_CommandNames.UnHideCMD, 20, file);
	GetPrivateProfileString("commands", "ClearInvCMD", "/clearinv", m_CommandNames.ClearInvCMD, 20, file);
	GetPrivateProfileString("commands", "MarryDivorceCMD", "/divorce", m_CommandNames.DIVORCECMD, 20, file);

	GetPrivateProfileString("commands", "CrywolfControlCMD", "/crywolfset", m_CommandNames.CWSetStateCMD, 20, file);
	GetPrivateProfileString("commands", "CastleSiegeSetOwnerCMD", "/cssetowner", m_CommandNames.CSSetOwnerCMD, 20, file);
	GetPrivateProfileString("commands", "CastleSiegeSetRegisterCMD", "/cssetregister", m_CommandNames.CSSetRegCMD, 20, file);
	GetPrivateProfileString("commands", "CastleSiegeSetRegMarkCMD", "/cssetregmark", m_CommandNames.CSSetMarkCMD, 20, file);
	GetPrivateProfileString("commands", "CastleSiegeSetNotifyCMD", "/cssetnotify", m_CommandNames.CSSetNotifyCMD, 20, file);
	GetPrivateProfileString("commands", "CastleSiegeSetReadyCMD", "/cssetready", m_CommandNames.CSSetReadyCMD, 20, file);
	GetPrivateProfileString("commands", "CastleSiegeSetEndCMD", "/cssetend", m_CommandNames.CSSetEndCMD, 20, file);
	GetPrivateProfileString("commands", "CastleSiegeGetOwnerCMD", "/csgetowner", m_CommandNames.CSGetOwnerCMD, 20, file);
	GetPrivateProfileString("commands", "CastleSiegeGetStateCMD", "/csgetstate", m_CommandNames.CSGetStateCMD, 20, file);
	GetPrivateProfileString("commands", "CastleSiegeSetStartCMD", "/cssetstart", m_CommandNames.CSSetStartCMD, 20, file);
	GetPrivateProfileString("commands", "CastleSiegeChangeSideCMD", "/cschangeside", m_CommandNames.CSSetChangeSideCMD, 20, file);

	GetPrivateProfileString("commands", "KanturuSetStandbyCMD", "/ktstandby", m_CommandNames.KTSetStandbyCMD, 20, file);
	GetPrivateProfileString("commands", "KanturuSetMayaCMD", "/ktmaya", m_CommandNames.KTSetMayaCMD, 20, file);
	GetPrivateProfileString("commands", "KanturuSetTowerCMD", "/kttower", m_CommandNames.KTSetTowerCMD, 20, file);

	GetPrivateProfileString("commands", "BanCharCMD", "/banchar", m_CommandNames.BanCharCMD, 20, file);
	GetPrivateProfileString("commands", "UnBanCharCMD", "/unbanchar", m_CommandNames.UnBanCharCMD, 20, file);
	GetPrivateProfileString("commands", "BanAccCMD", "/banacc", m_CommandNames.BanAccCMD, 20, file);
	GetPrivateProfileString("commands", "UnBanAccCMD", "/unbanacc", m_CommandNames.UnBanAccCMD, 20, file);

	GetPrivateProfileString("commands", "CrewCMD", "/crew", m_CommandNames.CrewCMD, 20, file);
	GetPrivateProfileString("commands", "GlobalPostCMD", "/gpost", m_CommandNames.GPostCMD, 20, file);
	GetPrivateProfileString("commands", "SetPKCMD", "/setpk", m_CommandNames.PKSETCMD, 20, file);
	GetPrivateProfileString("commands", "OffTradeCMD", "/off", m_CommandNames.offTrade, 20, file);
	GetPrivateProfileString("commands", "DCFriendCMD", "/dcfriend", m_CommandNames.dcFriend, 20, file);
	GetPrivateProfileString("commands", "SetPartyCMD", "/setparty", m_CommandNames.setparty, 20, file);
	GetPrivateProfileString("commands", "OffLevelCMD", "/offlevel", m_CommandNames.offlevel, 20, file);
	GetPrivateProfileString("commands", "GremoryGiftCMD", "/addgift", m_CommandNames.GremoryGiftCMD, 20, file);

	GetPrivateProfileString("joinmu", "ChangeItemCMD", "/changeitem", m_CommandNames.JoinMuChangeCMD, 20, file);
	GetPrivateProfileString("joinmu", "UpgradeItemCMD", "/upgradeitem", m_CommandNames.JoinMuUpgradeCMD, 20, file);

}
void CGMMng::Init()
{
	if(CommandLog == NULL)
	{
		CommandLog = new CLogToFile("COMMANDS_LOG", g_ConfigRead.server.GetCommandLogPath(), 256);
	}

	this->cCommand.Init();
	this->LoadCommandFile(g_ConfigRead.GetPath("IGC_Commands.ini"));

	this->cCommand.Add(m_CommandNames.FireWorks,217);
	this->cCommand.Add(m_CommandNames.Watch,216);
	this->cCommand.Add(m_CommandNames.Trace,215);
	this->cCommand.Add(m_CommandNames.DC,100);
	this->cCommand.Add(m_CommandNames.GuildDC,112);
	this->cCommand.Add(m_CommandNames.MoveCMD,101);
	this->cCommand.Add(m_CommandNames.gMoveCMD,108);
	this->cCommand.Add(m_CommandNames.StartSoccer,109);
	this->cCommand.Add(m_CommandNames.EndSoccer,110);
	this->cCommand.Add(m_CommandNames.EndWar,111);
	this->cCommand.Add(m_CommandNames.ChatBan,104);
	this->cCommand.Add(m_CommandNames.ChatUnban,106);
	this->cCommand.Add("/remaintime",201);
	this->cCommand.Add(m_CommandNames.SetGW,202);
	this->cCommand.Add(m_CommandNames.Requests,203);
	this->cCommand.Add(m_CommandNames.SoccerStart, 204);
	this->cCommand.Add(m_CommandNames.KundunInfo,320);
	this->cCommand.Add(m_CommandNames.SetKundunHP,321);
	if (g_ConfigRead.server.GetServerType() == SERVER_CASTLE)
	{
		this->cCommand.Add(m_CommandNames.CSSetOwnerCMD, 331);
		this->cCommand.Add(m_CommandNames.CSSetRegCMD, 332);
		this->cCommand.Add(m_CommandNames.CSSetMarkCMD, 333);
		this->cCommand.Add(m_CommandNames.CSSetNotifyCMD, 334);
		this->cCommand.Add(m_CommandNames.CSSetReadyCMD, 335);
		this->cCommand.Add(m_CommandNames.CSSetEndCMD, 336);
		this->cCommand.Add(m_CommandNames.CSGetOwnerCMD, 337);
		this->cCommand.Add(m_CommandNames.CSGetStateCMD, 338);
		this->cCommand.Add(m_CommandNames.CSSetStartCMD, 339);
		this->cCommand.Add(m_CommandNames.CSSetChangeSideCMD, 340);
		this->cCommand.Add(m_CommandNames.CWSetStateCMD, 398);
	}
	this->cCommand.Add(m_CommandNames.WARCMD,200);
	this->cCommand.Add(m_CommandNames.PostCMD, 391);	// PostMsg
	this->cCommand.Add(m_CommandNames.PKCLEARCMD, 392);	// ClearPkCommand
	this->cCommand.Add(m_CommandNames.AddSTRCMD, 393);
	this->cCommand.Add(m_CommandNames.AddAGICMD, 394);
	this->cCommand.Add(m_CommandNames.AddVITCMD, 395);
	this->cCommand.Add(m_CommandNames.ADDENECMD, 396);
	this->cCommand.Add(m_CommandNames.ADDCMDCMD, 397);
	this->cCommand.Add(m_CommandNames.ItemCMD, 390);
	this->cCommand.Add(m_CommandNames.ONLINECMD,400);
	this->cCommand.Add("/info",401);
	this->cCommand.Add("/i",402);
	this->cCommand.Add(m_CommandNames.WARECMD,403);
	this->cCommand.Add(m_CommandNames.PROPOSECMD,404);
	this->cCommand.Add(m_CommandNames.ACCEPTCMD,405);
	this->cCommand.Add(m_CommandNames.TRACEMARRYCMD,406);
	this->cCommand.Add(m_CommandNames.SKINCMD,420);
	this->cCommand.Add(m_CommandNames.BCStart,408);
	this->cCommand.Add(m_CommandNames.DSStart,409);
	this->cCommand.Add(m_CommandNames.CCStart,410);
	this->cCommand.Add(m_CommandNames.ITStart,411);
	this->cCommand.Add(m_CommandNames.ResetCMD,412);
	if (g_ConfigRead.server.GetServerType() != SERVER_CASTLE)
	{
		this->cCommand.Add(m_CommandNames.KTSetStandbyCMD, 413);
		this->cCommand.Add(m_CommandNames.KTSetMayaCMD, 414);
		this->cCommand.Add(m_CommandNames.KTSetTowerCMD, 415);
	}
	this->cCommand.Add(m_CommandNames.HideCMD,416);
	this->cCommand.Add(m_CommandNames.UnHideCMD,417);
	this->cCommand.Add(m_CommandNames.ClearInvCMD,418);
	this->cCommand.Add(m_CommandNames.SvInfoCMD,423);
	this->cCommand.Add(m_CommandNames.CharInfoCMD, 399);
	this->cCommand.Add(m_CommandNames.PKSETCMD, 421);
	this->cCommand.Add(m_CommandNames.DIVORCECMD, 422);
	this->cCommand.Add(m_CommandNames.BanAccCMD, 424);
	this->cCommand.Add(m_CommandNames.UnBanAccCMD, 425);
	this->cCommand.Add(m_CommandNames.BanCharCMD, 426);
	this->cCommand.Add(m_CommandNames.UnBanCharCMD, 427);
	this->cCommand.Add(m_CommandNames.CrewCMD, 428);
	this->cCommand.Add(m_CommandNames.GPostCMD, 429);
	this->cCommand.Add("/zxcgetaccinformationforuserzxc", 430);
	this->cCommand.Add("/unlock", 431);
	this->cCommand.Add("/lock", 432);
	this->cCommand.Add("/idle", 434);
	this->cCommand.Add(m_CommandNames.offTrade,433);
	this->cCommand.Add(m_CommandNames.dcFriend,440);
	this->cCommand.Add("/ifsetcheat",441);
	this->cCommand.Add("/ifsetday",442);
	this->cCommand.Add("/ifwarpzone",443);
	this->cCommand.Add("/mlsreset", 444);
	this->cCommand.Add("/gethp", 445);
	this->cCommand.Add("/trans", 446);
	this->cCommand.Add(m_CommandNames.setparty, 447);
	this->cCommand.Add("/test", 448);
	this->cCommand.Add(m_CommandNames.offlevel, 449);
	this->cCommand.Add(m_CommandNames.GremoryGiftCMD, 450);
	this->cCommand.Add("/offdebug", 451);
	this->cCommand.Add("/ccsblow", 452);
	this->cCommand.Add("/ccstrap", 453);
	this->cCommand.Add("/questchangeday", 454);
	this->cCommand.Add(m_CommandNames.JoinMuChangeCMD, 455);
	this->cCommand.Add(m_CommandNames.JoinMuUpgradeCMD, 456);
	this->WatchTargetIndex = -1;
}

void CGMMng::ManagerInit()
{
	for ( int n = 0;n<MAX_GAME_MASTER ; n++ )
	{
		this->ManagerIndex[n] = -1;
	}
}

int CGMMng::ManagerAdd(char* name, int aIndex)
{
	for ( int n=0 ; n<MAX_GAME_MASTER ; n++ )
	{
		if ( this->ManagerIndex[n] == -1 )
		{
			strcpy(this->szManagerName[n], name);
			this->ManagerIndex[n] = aIndex;
			return n;
		}
	}

	return -1;
}

void CGMMng::ManagerDel(char* name)
{
	for ( int n=0;n<MAX_GAME_MASTER;n++)
	{
		if ( this->ManagerIndex[n] != -1 )
		{
			if ( strcmp(this->szManagerName[n], name) == 0 )
			{
				this->ManagerIndex[n] = -1;
				break;
			}
		}
	}
}

void CGMMng::ManagerSendData(LPSTR szMsg, int size)
{
	for ( int n=0;n<MAX_GAME_MASTER;n++ )
	{
		if ( this->ManagerIndex[n] != -1 )
		{
			GSProtocol.GCServerMsgStringSend(szMsg, this->ManagerIndex[n], 0);
		}
	}
}

void CGMMng::DataSend(unsigned char* szMsg, int size)
{
	for ( int n=0;n<MAX_GAME_MASTER;n++ )
	{
		if ( this->ManagerIndex[n] != -1 )
		{
			::IOCP.DataSend(this->ManagerIndex[n], szMsg, size);
		}
	}
}

void CGMMng::BattleInfoSend(char* Name1, BYTE score1, char* Name2, BYTE score2)
{
	for ( int n=0;n<MAX_GAME_MASTER;n++ )
	{
		if ( this->ManagerIndex[n] != -1 )
		{
			GSProtocol.GCGoalSend(this->ManagerIndex[n], Name1, score1, Name2, score2);
		}
	}
}

int CGMMng::GetCmd(char* szCmd)
{
	for ( int n	=0; n< MAX_GM_COMMAND ; n++ )
	{
		if ( this->cCommand.nCmdCode[n] > 0 )
		{
			if ( stricmp(szCmd, this->cCommand.szCmd[n]) == 0 )
			{
				return this->cCommand.nCmdCode[n];
			}
		}
	}

	return 0;
}


char* CGMMng::GetTokenString()
{
	char seps[2] = " ";
	return strtok(0, seps);
}

int CGMMng::GetTokenNumber()
{
	char seps[2] = " ";
	char * szToken = strtok(0, seps);

	if ( szToken != NULL )
	{
		return atoi(szToken);
	}

	return 0;
}

void PostSend(LPOBJ lpObj, char * szMessage)
{
	PMSG_POST_DATA pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xFA, 0x10, sizeof(pMsg));

	pMsg.btColorRGB[0] = g_ConfigRead.data.post.btPostColorRGB[0];
	pMsg.btColorRGB[1] = g_ConfigRead.data.post.btPostColorRGB[1];
	pMsg.btColorRGB[2] = g_ConfigRead.data.post.btPostColorRGB[2];

	memcpy(pMsg.szName, lpObj->Name, MAX_ACCOUNT_LEN);
	memcpy(pMsg.szServerName, g_ConfigRead.server.GetServerName(), 50);

	strcpy(pMsg.szTag, Lang.GetText(0,334));
	strcpy(pMsg.szMessage, szMessage);
	
	GSProtocol.DataSendAll((LPBYTE)&pMsg, sizeof(pMsg));
}

int CGMMng::ManagementProc(LPOBJ lpObj, char* szCmd, int aIndex)
{
	char seps[2] = " ";
	char * szCmdToken;
	char string[256];
	char szId[20];
	char * pId = szId;
	int len = strlen(szCmd);
	int command_number;

	if ( len < 1 || len > 250 )
	{
		return 0;
	}

	memset(szId, 0, sizeof(szId));
	strcpy(string, szCmd);
	szCmdToken = strtok(string, seps);
	command_number = this->GetCmd(szCmdToken);

	if(command_number != 392 && command_number != 412)
	{
		CommandLog->Output("[%s][%s][%s] Used command: %s [AUTH: %d]", 
			lpObj->AccountID, lpObj->Name, lpObj->m_PlayerData->Ip_addr, szCmdToken,
			lpObj->Authority);
	}

	switch ( command_number )
	{
		case 331:
		{
			pId = this->GetTokenString();
			g_CastleSiege.OperateGmCommand(lpObj->m_Index, 0, pId);
		}
		break;
		case 332:
		{
			pId = this->GetTokenString();
			g_CastleSiege.OperateGmCommand(lpObj->m_Index, 1, pId);
		}
		break;
		case 333:
		{
			pId = this->GetTokenString();
			g_CastleSiege.OperateGmCommand(lpObj->m_Index, 2, pId);
		}
		break;
		case 334:
		{
			pId = this->GetTokenString();
			g_CastleSiege.OperateGmCommand(lpObj->m_Index, 3, pId);
		}
		break;
		case 335:
		{
			pId = this->GetTokenString();
			g_CastleSiege.OperateGmCommand(lpObj->m_Index, 4, pId);
		}
		break;
		case 336:
		{
			pId = this->GetTokenString();
			g_CastleSiege.OperateGmCommand(lpObj->m_Index, 5, pId);
		}
		break;
		case 337:
		{
			pId = this->GetTokenString();
			g_CastleSiege.OperateGmCommand(lpObj->m_Index, 6, pId);
		}
		break;
		case 338:
		{
			pId = this->GetTokenString();
			g_CastleSiege.OperateGmCommand(lpObj->m_Index, 7, pId);
		}
		break;
		case 339:
		{
			pId = this->GetTokenString();
			g_CastleSiege.OperateGmCommand(lpObj->m_Index, 9, pId);
		}
		break;
		case 340:
		{
			pId = this->GetTokenString();
			g_CastleSiege.OperateGmCommand(lpObj->m_Index, 8, pId);
		}
		break;
		case 217:	//116:
			{
				if ( (lpObj->Authority & 2) != 2 && (lpObj->Authority & 0x20) != 0x20 )
				{
					return 0;
				}

				if ( (lpObj->GameMaster & GM_COMMAND_FIRECRACKER) != GM_COMMAND_FIRECRACKER )
				{
					MsgOutput(lpObj->m_Index, Lang.GetText(0,424));
					return 0;
				}

				g_Log.Add( "Use GM Command -> [ %s ]\t[ %s ]\t[ %s ] : %s",
					lpObj->m_PlayerData->Ip_addr, lpObj->AccountID, lpObj->Name, "FIRECRACK.");

				CommandLog->Output("Use GM Command -> [ %s ]\t[ %s ]\t[ %s ] : %s",
					lpObj->m_PlayerData->Ip_addr, lpObj->AccountID, lpObj->Name, "FIRECRACK.");

				int x = lpObj->X;
				int y = lpObj->Y;
				PMSG_SERVERCMD pMsg;

				PHeadSubSetB((LPBYTE)&pMsg,0xF3,0x40, sizeof(pMsg));
				pMsg.CmdType = 0;

				for ( int i=0;i<15;i++)
				{
					pMsg.X = x+(rand() % 5)*2 - 4;
					pMsg.Y = y+(rand() % 5)*2 - 4;
					GSProtocol.MsgSendV2(lpObj,(UCHAR*)&pMsg, sizeof(pMsg));
					::IOCP.DataSend(lpObj->m_Index ,(UCHAR*)&pMsg, sizeof(pMsg));
				}	
			}
			break;

		case 216:	//115:
			{
				if ( (lpObj->Authority & 2) != 2 && (lpObj->Authority & 0x20) != 0x20 )
				{
					return 0;
				}

				if ( (lpObj->GameMaster & GM_COMMAND_DC_MOVE) != GM_COMMAND_DC_MOVE )
				{
					MsgOutput(lpObj->m_Index, Lang.GetText(0,425));
					return 0;
				}

				pId = this->GetTokenString();

				if ( pId == NULL )
				{
					return 0;
				}

				LPOBJ lpTargetObj = gObjFind(pId);

				if ( lpTargetObj == NULL )
				{
					return 0;
				}

				/*LogAddTDC(1,"Use GM Command -> [ %s ]	[ %s ]	[ %s ] / Target : [%s][%s] : %s ",
					lpObj->m_PlayerData->Ip_addr, lpObj->AccountID, lpObj->Name, lpTargetObj->AccountID,
					lpTargetObj->Name, "User Watching");*/

				char szTemp[256];

				if ( this->WatchTargetIndex == lpTargetObj->m_Index )
				{
					this->WatchTargetIndex = -1;
					
					wsprintf(szTemp, "%s - invalid target", lpTargetObj->Name);
					GSProtocol.GCServerMsgStringSend(szTemp, lpObj->m_Index, 1);
				}
				else
				{
					wsprintf(szTemp, "%s - monitoring set", lpTargetObj->Name);
					GSProtocol.GCServerMsgStringSend(szTemp, lpObj->m_Index, 1);
					this->WatchTargetIndex = lpTargetObj->m_Index;
				}
			}
			break;

		case 215:	//114:
			{
				if ( (lpObj->Authority & 2) != 2 && (lpObj->Authority & 0x20) != 0x20 )
				{
					return 0;
				}

				if ( (lpObj->GameMaster & GM_COMMAND_DC_MOVE) != GM_COMMAND_DC_MOVE )
				{
					MsgOutput(lpObj->m_Index, Lang.GetText(0,425));
					return 0;
				}

				pId = this->GetTokenString();

				if ( pId == NULL )
				{
					return 0;
				}

				int map;
				int iX;
				int iY;
				LPOBJ lpTargetObj = gObjFind(pId);
				int iIndex;

				if ( lpTargetObj == NULL )
				{
					return 0;
				}

				g_Log.Add("Use GM Command -> [ %s ]\t[ %s ]\t[ %s ] / Target : [%s][%s] : %s",
					lpObj->m_PlayerData->Ip_addr, lpObj->AccountID, lpObj->Name, lpTargetObj->AccountID,
					lpTargetObj->Name, "User Tracking");

				GMLog->Output("Use GM Command -> [ %s ]\t[ %s ]\t[ %s ] / Target : [%s][%s] : %s",
					lpObj->m_PlayerData->Ip_addr, lpObj->AccountID, lpObj->Name, lpTargetObj->AccountID,
					lpTargetObj->Name, "User Tracking");

				map = lpTargetObj->MapNumber;
				iX = lpTargetObj->X;
				iY = lpTargetObj->Y;
				iIndex = lpObj->m_Index;

				if ( iIndex >= 0 )
				{
					gObjTeleport(iIndex, map, iX, iY);
				}
				if ( map == MAP_INDEX_HATCHERY )
				{
					g_RaklionBattleUserMng.AddUserData(lpObj->m_Index);
				}
			}
			break;
		case 214:	//113:
			{
				if ( (lpObj->Authority & 2) != 2 )
				{
					return 0;
				}

				if ( (lpObj->GameMaster & GM_COMMAND_DC_MOVE) != GM_COMMAND_DC_MOVE )
				{
					MsgOutput(lpObj->m_Index, Lang.GetText(0,425));
					return 0;
				}

				g_Log.Add("Use GM Command -> [ %s ]\t[ %s ]\t[ %s ] : %s",
					lpObj->m_PlayerData->Ip_addr, lpObj->AccountID, lpObj->Name, 
					"User Stat (connection)");
				int lc151 = 0;
				int lc152 = 400;
				int iTokenNumber = this->GetTokenNumber();

				if ( iTokenNumber > 0 )
				{
					lc151 = iTokenNumber;
				}

				int iTokenNumber2 = this->GetTokenNumber();

				if ( iTokenNumber2 > 0 )
				{
					lc152 = iTokenNumber2;
				}

				gObjSendUserStatistic(lpObj->m_Index, lc151, lc152);
			}
			break;

		case 100:	//100:
			{
				if ( (lpObj->AuthorityCode &4) != 4 )
				{
					return 0;
				}

				if ( (lpObj->GameMaster & GM_COMMAND_DC_MOVE) != GM_COMMAND_DC_MOVE )
				{
					MsgOutput(lpObj->m_Index, Lang.GetText(0,425));
					return 0;
				}

				pId = this->GetTokenString();

				if ( pId == NULL )
				{
					return 0;
				}

				int iTargetIndex = gObjGetIndex(pId);
				

				if ( iTargetIndex >= 0 )
				{
					LPOBJ lpTargetObj = gObjFind(pId);

					if ( lpTargetObj == NULL )
					{
						return 0;
					}

					if(lpTargetObj->m_bOff != false)
					{
						GJSetCharacterInfo(lpTargetObj, iTargetIndex, FALSE);
						gObj[iTargetIndex].m_bOff = false;
						gObjDel(iTargetIndex);
					}

					g_Log.Add("Use GM Command -> [ %s ]\t[ %s ]\t[ %s ] / Target : [%s][%s] : %s",
						lpObj->m_PlayerData->Ip_addr, lpObj->AccountID, lpObj->Name, lpTargetObj->AccountID,
						lpTargetObj->Name, "User Disconnect");

					GMLog->Output("Use GM Command -> [ %s ]\t[ %s ]\t[ %s ] / Target : [%s][%s] : %s",
						lpObj->m_PlayerData->Ip_addr, lpObj->AccountID, lpObj->Name, lpTargetObj->AccountID,
						lpTargetObj->Name, "User Disconnect");

					g_Log.Add("%s is forced to disconnect", pId);

					GSProtocol.GCSendDisableReconnect(iTargetIndex);
					//IOCP.CloseClient(iTargetIndex);
				}
				else if(g_ConfigRead.data.common.IsJoinMu)
				{
					GDReqDisconnectOtherChannel(pId);
				}
			}
			break;

		case 112:	//108:
			{
				if ( (lpObj->AuthorityCode &4) != 4 )
				{
					return 0;
				}

				if ( (lpObj->GameMaster & GM_COMMAND_GUILD) != GM_COMMAND_GUILD )
				{
					MsgOutput(lpObj->m_Index, Lang.GetText(0,426));
					return 0;
				}

				g_Log.Add( "Use GM Command -> [ %s ]\t[ %s ]\t[ %s ] : %s",
					lpObj->m_PlayerData->Ip_addr, lpObj->AccountID, lpObj->Name,
					"Guild Disconnect");

				GMLog->Output("Use GM Command -> [ %s ]\t[ %s ]\t[ %s ] : %s",
					lpObj->m_PlayerData->Ip_addr, lpObj->AccountID, lpObj->Name,
					"Guild Disconnect");

				pId = this->GetTokenString();

				if ( pId == NULL )
				{
					return 0;
				}

				_GUILD_INFO_STRUCT * lpGuild = Guild.SearchGuild(pId);
				int iIndex;

				if ( lpGuild != NULL )
				{
					for ( int i=0;i<MAX_USER_GUILD ; i++ )
					{
						if ( lpGuild->Index[i] >= 0 )
						{
							iIndex = lpGuild->Index[i];

							if ( iIndex >= 0 )
							{
								g_Log.Add("%s is forced to disconnect", pId);
								IOCP.CloseClient(iIndex);
							}
						}
					}
				}
			}
			break;

		case 101:	//101:
			{
				if ( (lpObj->GameMaster & GM_COMMAND_GUILD) == GM_COMMAND_GUILD && (lpObj->GameMaster & GM_COMMAND_DC_MOVE) == GM_COMMAND_DC_MOVE )
				{
					pId = this->GetTokenString();

					if ( pId == NULL )
					{
						return 0;
					}

					int iTokenNumber1 = this->GetTokenNumber();
					int iTokenNumber2 = this->GetTokenNumber();
					int iTokenNumber3 = this->GetTokenNumber();
					int iIndex = gObjGetIndex(pId);
					LPOBJ lpTargetObj;

					if ( iIndex >= 0 )
					{
						lpTargetObj = gObjFind(pId);

						if ( lpTargetObj == NULL )
						{
							return 0;
						}

						int nCurrentMap = lpTargetObj->MapNumber;

						g_Log.Add( "Use GM Command -> [ %s ]\t[ %s ]\t[ %s ] / Target : [%s][%s] : %s",
							lpObj->m_PlayerData->Ip_addr, lpObj->AccountID, lpObj->Name, lpTargetObj->AccountID,
							lpTargetObj->Name, "User SetPosition");

						GMLog->Output("Use GM Command -> [ %s ]\t[ %s ]\t[ %s ] / Target : [%s][%s] : %s",
							lpObj->m_PlayerData->Ip_addr, lpObj->AccountID, lpObj->Name, lpTargetObj->AccountID,
							lpTargetObj->Name, "User SetPosition");

						gObjTeleport(iIndex, iTokenNumber1, iTokenNumber2, iTokenNumber3);

						if (nCurrentMap != iTokenNumber1)
						{
							g_SendNPCInfo.SendNpcCoordinate(lpTargetObj->m_Index, lpTargetObj->MapNumber, 0);
							g_SendNPCInfo.SendPortalCoordinate(lpTargetObj->m_Index, lpTargetObj->MapNumber);
						}
					}
				}
				else
				{
					pId = this->GetTokenString();

					if ( pId != NULL )
					{
						int lc165 = -1;
						int lc166 = 0;
						int lc167 = 0;

						if ( lpObj->Teleport != 0 )
						{
							GSProtocol.GCServerMsgStringSend(Lang.GetText(0,153), lpObj->m_Index, 1);
							return 0;
						}

						if ( (lpObj->m_IfState.use) != 0 )
						{
							if ( lpObj->m_IfState.type  == 3 )
							{
								lpObj->TargetNpcNumber = -1;
								lpObj->m_IfState.type = 0;
								lpObj->m_IfState.use = 0;
							}
						}

						if ( lpObj->m_IfState.use > 0 )
						{
							GSProtocol.GCServerMsgStringSend(Lang.GetText(0,153), lpObj->m_Index, 1);
							return 0;
						}

						if ( gObj[aIndex].IsInBattleGround != false )
						{
							GSProtocol.GCServerMsgStringSend(Lang.GetText(0,153), lpObj->m_Index, 1);
							return 0;
						}
					}
				}
			}
			break;

		case 108:	//104:
			{
				if ( (lpObj->AuthorityCode &8)!= 8 )
				{
					return 0;
				}

				if ( (lpObj->GameMaster & GM_COMMAND_GUILD) != GM_COMMAND_GUILD )
				{
					MsgOutput(lpObj->m_Index, Lang.GetText(0,426));
					return 0;
				}

				g_Log.Add( "Use GM Command -> [ %s ]\t[ %s ]\t[ %s ] : %s",
					lpObj->m_PlayerData->Ip_addr, lpObj->AccountID, lpObj->Name, 
					"Guild SetPosition");

				GMLog->Output("Use GM Command -> [ %s ]\t[ %s ]\t[ %s ] : %s",
					lpObj->m_PlayerData->Ip_addr, lpObj->AccountID, lpObj->Name, 
					"Guild SetPosition");

				pId = this->GetTokenString();

				if ( pId == NULL )
				{
					return 0;
				}

				int iTokenNumber1 = this->GetTokenNumber();
				int iTokenNumber2 = this->GetTokenNumber();
				int iTokenNumber3 = this->GetTokenNumber();
				_GUILD_INFO_STRUCT* lpGuild = Guild.SearchGuild(pId);
				int iIndex;

				if (lpGuild != NULL )
				{
					for ( int i=0;i<MAX_USER_GUILD;i++)
					{
						if (lpGuild->Index[i] >= 0 )
						{
							iIndex = lpGuild->Index[i];
							gObjTeleport(iIndex, iTokenNumber1, iTokenNumber2++, iTokenNumber3);
						}
					}
				}
			}
			break;

		case 109:	//105:
			{
				if ( (lpObj->Authority &2)== 2 )
				{
					if ( (lpObj->GameMaster & GM_COMMAND_BATTLESOCCER) != GM_COMMAND_BATTLESOCCER )
					{
						MsgOutput(lpObj->m_Index, Lang.GetText(0,427));
						return 0;
					}

					g_Log.Add( "Use GM Command -> [ %s ]\t[ %s ]\t[ %s ] : %s",
						lpObj->m_PlayerData->Ip_addr, lpObj->AccountID, lpObj->Name,
						"Start BattleSoccer");

					GMLog->Output("Use GM Command -> [ %s ]\t[ %s ]\t[ %s ] : %s",
						lpObj->m_PlayerData->Ip_addr, lpObj->AccountID, lpObj->Name,
						"Start BattleSoccer");

					BattleSoccerGoalStart(0);
				}
			}
			break;

		case 110:	//106:
			{
				if ( (lpObj->Authority &2) == 2 )
				{
					if ( (lpObj->GameMaster & GM_COMMAND_BATTLESOCCER) != GM_COMMAND_BATTLESOCCER )
					{
						MsgOutput(lpObj->m_Index, Lang.GetText(0,856));
						return 0;
					}

					g_Log.Add( "Use GM Command -> [ %s ]\t[ %s ]\t[ %s ] : %s",
						lpObj->m_PlayerData->Ip_addr, lpObj->AccountID, lpObj->Name,
						"Stop BattleSoccer");

					GMLog->Output("Use GM Command -> [ %s ]\t[ %s ]\t[ %s ] : %s",
						lpObj->m_PlayerData->Ip_addr, lpObj->AccountID, lpObj->Name,
						"Stop BattleSoccer");

					BattleSoccerGoalEnd(0);
				}
				else
				{
					if ( gObj[aIndex].m_PlayerData->lpGuild != NULL )
					{
						if (gObj[aIndex].m_PlayerData->lpGuild->WarType == 1 )
						{
							strcmp(gObj[aIndex].Name, gObj[aIndex].m_PlayerData->lpGuild->Names[0] );
						}
					}
				}
			}

			break;

		case 111:	//107:
			{
				if ( (lpObj->Authority & 2) == 2 )
				{
					if ( (lpObj->GameMaster & GM_COMMAND_GUILD) != GM_COMMAND_GUILD )
					{
						MsgOutput(lpObj->m_Index, Lang.GetText(0,426));
						return 0;
					}

					g_Log.Add( "Use GM Command -> [ %s ]\t[ %s ]\t[ %s ] : %s",
						lpObj->m_PlayerData->Ip_addr, lpObj->AccountID, lpObj->Name, "End GuildWar");

					GMLog->Output("Use GM Command -> [ %s ]\t[ %s ]\t[ %s ] : %s",
						lpObj->m_PlayerData->Ip_addr, lpObj->AccountID, lpObj->Name, "End GuildWar");

					char * szGuild = this->GetTokenString();

					if ( szGuild != NULL )
					{
						GSProtocol.GCManagerGuildWarEnd(szGuild);
					}
				}
				else
				{
					if ( gObj[aIndex].m_PlayerData->lpGuild != NULL && gObj[aIndex].m_PlayerData->lpGuild->lpTargetGuildNode != NULL)
					{
						if ( strcmp( gObj[aIndex].Name, gObj[aIndex].m_PlayerData->lpGuild->Names[0] ) ==  0)
						{
							if ( gObj[aIndex].m_PlayerData->lpGuild->BattleGroundIndex >= 0 && gObj[aIndex].m_PlayerData->lpGuild->WarType == 1 )
							{
								::gObjAddMsgSendDelay(&gObj[aIndex], 7, aIndex, 10000, 0);

								char szTemp[100];

								wsprintf(szTemp, Lang.GetText(0,61), gObj[aIndex].m_PlayerData->lpGuild->Names[0] );
								GSProtocol.GCServerMsgStringSendGuild(gObj[aIndex].m_PlayerData->lpGuild, szTemp, 1);
								GSProtocol.GCServerMsgStringSendGuild(gObj[aIndex].m_PlayerData->lpGuild->lpTargetGuildNode, szTemp, 1);
							}
						}
					}
				}
			}
			break;

		case 104:	//102:
			{
				if ( (lpObj->AuthorityCode & 0x20 ) != 0x20 )
				{
					return FALSE;
				}

				if ( (lpObj->GameMaster & GM_COMMAND_DC_MOVE) != GM_COMMAND_DC_MOVE )
				{
					MsgOutput(lpObj->m_Index, Lang.GetText(0,425));
					return 0;
				}

				g_Log.Add(  "Use GM Command -> [ %s ]\t[ %s ]\t[ %s ] : %s", lpObj->m_PlayerData->Ip_addr, lpObj->AccountID,
					lpObj->Name, "Ban Chatting");

				GMLog->Output("Use GM Command -> [ %s ]\t[ %s ]\t[ %s ] : %s", lpObj->m_PlayerData->Ip_addr, lpObj->AccountID,
					lpObj->Name, "Ban Chatting");

				pId = this->GetTokenString();
				int BanTime = this->GetTokenNumber();

				if ( pId == NULL )
				{
					return FALSE;
				}

				time_t bantime = time(NULL) + (BanTime * 60);

				int Index = ::gObjGetIndex(pId);

				if ( Index >= 0 )
				{
					gObj[Index].PenaltyMask |= 1;
					gObj[Index].Penalty |= 2;
					gObj[Index].ChatBlockTime = bantime;
					MsgOutput(lpObj->m_Index, Lang.GetText(0,428));
				}

			}
			break;

		case 106:	//103:
			{
				if ( (lpObj->AuthorityCode & 32 ) != 32 )
				{
					return FALSE;
				}

				if ( (lpObj->GameMaster & GM_COMMAND_DC_MOVE) != GM_COMMAND_DC_MOVE )
				{
					MsgOutput(lpObj->m_Index, Lang.GetText(0,425));
					return 0;
				}

				g_Log.Add( "Use GM Command -> [ %s ]\t[ %s ]\t[ %s ] : %s", lpObj->m_PlayerData->Ip_addr, lpObj->AccountID,
					lpObj->Name, "Free Ban-Chatting");

				GMLog->Output("Use GM Command -> [ %s ]\t[ %s ]\t[ %s ] : %s", lpObj->m_PlayerData->Ip_addr, lpObj->AccountID,
					lpObj->Name, "Free Ban-Chatting");

				pId = this->GetTokenString();

				if ( pId == NULL )
				{
					return FALSE;
				}

				int Index = ::gObjGetIndex(pId);

				if ( Index >= 0 )
				{
					gObj[Index].PenaltyMask &= ~1;
					gObj[Index].Penalty &= ~2;
					gObj[Index].ChatBlockTime = 0;
					MsgOutput(lpObj->m_Index, Lang.GetText(0,428));
				}
			}
			break;

		case 200:	//109:
			{
				pId = this->GetTokenString();

				if ( pId != NULL )
				{
					if ( strlen(pId) >= 1 )
					{
						GSProtocol.GCGuildWarRequestResult(pId, aIndex, 0);
					}
				}
			}

			break;

		case 202:	//111:
			{
				if ( (lpObj->Authority & 2 ) == 2 )
				{
					if ( (lpObj->GameMaster & GM_COMMAND_GUILD) != GM_COMMAND_GUILD )
					{
						MsgOutput(lpObj->m_Index, Lang.GetText(0,426));
						return 0;
					}

					g_Log.Add( "Use GM Command -> [ %s ]\t[ %s ]\t[ %s ] : %s", lpObj->m_PlayerData->Ip_addr, lpObj->AccountID,
						lpObj->Name, "Set GuildWar");

					GMLog->Output("Use GM Command -> [ %s ]\t[ %s ]\t[ %s ] : %s", lpObj->m_PlayerData->Ip_addr, lpObj->AccountID,
						lpObj->Name, "Set GuildWar");
		
					pId = this->GetTokenString();

					if ( pId != NULL )
					{
						char * Rival = this->GetTokenString();

						if ( Rival != NULL )
						{
							if ( strlen(pId) >= 1 )
							{
								if ( strlen(Rival) >= 1 )
								{
									GSProtocol.GCManagerGuildWarSet(pId, Rival, 1);
								}
							}
						}
					}
				}
			}

			break;

		case 204:
			{
				if (gEnableBattleSoccer != FALSE)
				{
					pId = this->GetTokenString();

					if (pId != NULL)
					{
						if (strlen(pId) >= 1)
						{
							GSProtocol.GCGuildWarRequestResult(pId, aIndex, 1);
						}
					}
				}
			}
			break;

		case 201:	//110:
			{
				gObjBillRequest(&gObj[aIndex]);
			}
			break;
			// Commands by [IGC] drakelv
		case 391:
			{
				if (lpObj->ChatLimitTime > 0)
				{
					MsgOutput(lpObj->m_Index, Lang.GetText(0,139), lpObj->ChatLimitTime);
					return FALSE;
				}

				if(lpObj->Level < g_ConfigRead.data.post.wPostLevel)
				{
					MsgOutput(lpObj->m_Index, Lang.GetText(0,429), g_ConfigRead.data.post.wPostLevel);
					return FALSE;
				}

				if(lpObj->m_PlayerData->Money < g_ConfigRead.data.post.iPostPrice)
				{
					MsgOutput(lpObj->m_Index, Lang.GetText(0,430), g_ConfigRead.data.post.iPostPrice);
					return FALSE;
				}

				if (strlen((char*)szCmd + strlen(this->m_CommandNames.PostCMD)) >= MAX_CHAT_LEN)
				{
					MsgOutput(aIndex, Lang.GetText(0,642));
					return FALSE;
				}

				if(SwearFilter.CompareText(szCmd+strlen(this->m_CommandNames.PostCMD)) == true)
				{
					lpObj->m_PlayerData->SwearWarning++;

					if(lpObj->m_PlayerData->SwearWarning >= 3)
					{
						GSProtocol.GCSendDisableReconnect(aIndex);
						MsgOutput(aIndex, Lang.GetText(0,431));
					}

					else
					{
						MsgOutput(aIndex, Lang.GetText(0,432), lpObj->m_PlayerData->SwearWarning);
						lpObj->m_PlayerData->Money -= g_ConfigRead.data.post.iPostPrice * 2;
						GSProtocol.GCMoneySend(lpObj->m_Index, lpObj->m_PlayerData->Money);
					}

					return 0;
				}

				if(GetTickCount() - lpObj->m_PostCMDLastUseTick < (g_ConfigRead.data.post.dwPostCooldown * 1000) )
				{
					MsgOutput(aIndex,Lang.GetText(0,601),g_ConfigRead.data.post.dwPostCooldown);
					return 0;
				}

				lpObj->m_PostCMDLastUseTick = GetTickCount();
				lpObj->m_PlayerData->Money -= g_ConfigRead.data.post.iPostPrice;
				GSProtocol.GCMoneySend(lpObj->m_Index,lpObj->m_PlayerData->Money);

				PostSend(lpObj, (char*)szCmd+strlen(this->m_CommandNames.PostCMD));
				break;
			}
			
		case 429: // MultiCast Post
			{
				if (lpObj->ChatLimitTime > 0)
				{
					MsgOutput(lpObj->m_Index, Lang.GetText(0,139), lpObj->ChatLimitTime);
					return FALSE;
				}

				if(lpObj->Level < g_ConfigRead.data.post.wGPostLevel)
				{
					MsgOutput(lpObj->m_Index, Lang.GetText(0,429), g_ConfigRead.data.post.wPostLevel);
					return FALSE;
				}

				if(lpObj->m_PlayerData->Money < g_ConfigRead.data.post.iGPostPrice)
				{
					MsgOutput(lpObj->m_Index, Lang.GetText(0,430), g_ConfigRead.data.post.iPostPrice);
					return FALSE;
				}

				if(SwearFilter.CompareText(szCmd+strlen(m_CommandNames.GPostCMD)) == true)
				{
					lpObj->m_PlayerData->SwearWarning++;

					if(lpObj->m_PlayerData->SwearWarning >= 3)
					{
						GSProtocol.GCSendDisableReconnect(aIndex);
						MsgOutput(aIndex, Lang.GetText(0,431));
						//IOCP.CloseClient(aIndex);
					}

					else
					{
						MsgOutput(aIndex, Lang.GetText(0,432), lpObj->m_PlayerData->SwearWarning);
						lpObj->m_PlayerData->Money -= g_ConfigRead.data.post.iPostPrice * 2;
						GSProtocol.GCMoneySend(lpObj->m_Index, lpObj->m_PlayerData->Money);
					}

					return 0;
				}

				if(GetTickCount() - lpObj->m_PostCMDLastUseTick < (g_ConfigRead.data.post.dwGPostCooldown * 1000) )
				{
					MsgOutput(aIndex,Lang.GetText(0,601),g_ConfigRead.data.post.dwGPostCooldown);
					return 0;
				}

				lpObj->m_PostCMDLastUseTick = GetTickCount();			
				lpObj->m_PlayerData->Money -= g_ConfigRead.data.post.iPostPrice;
				GSProtocol.GCMoneySend(lpObj->m_Index,lpObj->m_PlayerData->Money);

				GS_GDReqGlobalPostMultiCast(g_MapServerManager.GetMapSvrGroup(), lpObj->m_Index, (char*)szCmd + strlen(m_CommandNames.GPostCMD));
				break;
			}
			break;
		case 430:
			{
				/* pId = GetTokenString();
				int index = gObjGetIndex(pId);

				if(index == -1)
					break;

				MsgOutput(aIndex, "Name:%s Login:%s", pId, gObj[index].AccountID); */
			}
			break;
		case 393:
		{
			int Pontos;
			Pontos = GetTokenNumber();
			if(Pontos == NULL)
			{
				GSProtocol.GCServerMsgStringSend(Lang.GetText(0,434),lpObj->m_Index,1);
				return FALSE;
			}
			if(lpObj->m_PlayerData->LevelUpPoint < Pontos || Pontos < 1)
			{
				GSProtocol.GCServerMsgStringSend(Lang.GetText(0,435),lpObj->m_Index,1);
				return FALSE;
			}
			if((Pontos + lpObj->m_PlayerData->Strength) > g_MaxStrength)
			{
				GSProtocol.GCServerMsgStringSend(Lang.GetText(0,436),lpObj->m_Index,1);
				return FALSE;
			}
			lpObj->m_PlayerData->LevelUpPoint -= Pontos;
			lpObj->m_PlayerData->Strength += Pontos;

			PMSG_ADDSTATS pStats;
			pStats.h.c = 0xC1;
			pStats.h.headcode = 0xFA;
			pStats.h.size = sizeof(pStats);
			pStats.subcode = 0x90;
			pStats.Points = lpObj->m_PlayerData->Strength;
			pStats.statstype = 0;
			pStats.LUP = lpObj->m_PlayerData->LevelUpPoint;
			IOCP.DataSend(aIndex,(LPBYTE)&pStats,sizeof(pStats));

			gObjCalCharacter.CalcCharacter(aIndex);
			return TRUE;
		}
		break;
		case 394:
		{
			int Pontos;
			Pontos = GetTokenNumber();

			if(Pontos == NULL)
			{
				GSProtocol.GCServerMsgStringSend(Lang.GetText(0,434),lpObj->m_Index,1);
				return FALSE;
			}
			if(lpObj->m_PlayerData->LevelUpPoint < Pontos || Pontos < 1)
			{
				GSProtocol.GCServerMsgStringSend(Lang.GetText(0,435),lpObj->m_Index,1);
				return FALSE;
			}
			if((Pontos + lpObj->m_PlayerData->Dexterity) > g_MaxAgility)
			{
				GSProtocol.GCServerMsgStringSend(Lang.GetText(0,436),lpObj->m_Index,1);
				return FALSE;
			}

			EnterCriticalSection(&lpObj->m_PlayerData->AgiCheckCriti);
			lpObj->m_PlayerData->AgilityCheckDelay = GetTickCount();
			lpObj->m_PlayerData->Dexterity += Pontos;
			lpObj->m_PlayerData->LevelUpPoint -= Pontos;
			LeaveCriticalSection(&lpObj->m_PlayerData->AgiCheckCriti);

			PMSG_ADDSTATS pStats;
			pStats.h.c = 0xC1;
			pStats.h.headcode = 0xFA;
			pStats.h.size = sizeof(pStats);
			pStats.subcode = 0x90;
			pStats.Points = lpObj->m_PlayerData->Dexterity;
			pStats.statstype = 1;
			pStats.LUP = lpObj->m_PlayerData->LevelUpPoint;
			IOCP.DataSend(aIndex,(LPBYTE)&pStats,sizeof(pStats));

			gObjCalCharacter.CalcCharacter(aIndex);
			return TRUE;
		}
		break;
		case 395:
		{
			int Pontos;
			Pontos = GetTokenNumber();
			if(Pontos == NULL)
			{
				GSProtocol.GCServerMsgStringSend(Lang.GetText(0,434),lpObj->m_Index,1);
				return FALSE;
			}
			if(lpObj->m_PlayerData->LevelUpPoint < Pontos || Pontos < 1)
			{
				GSProtocol.GCServerMsgStringSend(Lang.GetText(0,435),lpObj->m_Index,1);
				return FALSE;
			}
			if((Pontos + lpObj->m_PlayerData->Vitality) > g_MaxVitality)
			{
				GSProtocol.GCServerMsgStringSend(Lang.GetText(0,436),lpObj->m_Index,1);
				return FALSE;
			}

			lpObj->m_PlayerData->LevelUpPoint -= Pontos;
			lpObj->m_PlayerData->Vitality += Pontos;
			lpObj->MaxLife += lpObj->VitalityToLife * Pontos;
			gObjCalcMaxLifePower(lpObj->m_Index);

			PMSG_ADDSTATS pStats;
			pStats.h.c = 0xC1;
			pStats.h.headcode = 0xFA;
			pStats.h.size = sizeof(pStats);
			pStats.subcode = 0x90;
			pStats.Points = lpObj->m_PlayerData->Vitality;
			pStats.statstype = 2;
			pStats.LUP = lpObj->m_PlayerData->LevelUpPoint;
			pStats.MaxLifeAndMana = lpObj->MaxLife + lpObj->AddLife;
			IOCP.DataSend(aIndex,(LPBYTE)&pStats,sizeof(pStats));

			gObjCalCharacter.CalcCharacter(lpObj->m_Index);
			return TRUE;
		}
		case 396:
		{
			int Pontos;
			Pontos = GetTokenNumber();
			if(Pontos == NULL)
			{
				GSProtocol.GCServerMsgStringSend(Lang.GetText(0,434),lpObj->m_Index,1);
				return FALSE;
			}
			if(lpObj->m_PlayerData->LevelUpPoint < Pontos || Pontos < 1)
			{
				GSProtocol.GCServerMsgStringSend(Lang.GetText(0,435),lpObj->m_Index,1);
				return FALSE;
			}
			if((Pontos + lpObj->m_PlayerData->Energy) > g_MaxEnergy)
			{
				GSProtocol.GCServerMsgStringSend(Lang.GetText(0,436),lpObj->m_Index,1);
				return FALSE;
			}

			lpObj->m_PlayerData->Energy += Pontos;
			lpObj->m_PlayerData->LevelUpPoint -= Pontos;
			lpObj->MaxMana += lpObj->EnergyToMana * Pontos;

			PMSG_ADDSTATS pStats;
			pStats.h.c = 0xC1;
			pStats.h.headcode = 0xFA;
			pStats.h.size = sizeof(pStats);
			pStats.subcode = 0x90;
			pStats.Points = lpObj->m_PlayerData->Energy;
			pStats.statstype = 3;
			pStats.LUP = lpObj->m_PlayerData->LevelUpPoint;
			pStats.MaxLifeAndMana = lpObj->MaxMana + lpObj->AddMana;
			IOCP.DataSend(aIndex,(LPBYTE)&pStats,sizeof(pStats));

			gObjCalCharacter.CalcCharacter(lpObj->m_Index);
			return TRUE;
			break;
		}
		case 397:
		{
			if(lpObj->Class != 4)
			{
				GSProtocol.GCServerMsgStringSend(Lang.GetText(0,441),lpObj->m_Index,1);
				return FALSE;
			}

			int Pontos;
			Pontos = GetTokenNumber();
			if(Pontos == NULL)
			{
				GSProtocol.GCServerMsgStringSend(Lang.GetText(0,434),lpObj->m_Index,1);
				return FALSE;
			}
			if(lpObj->m_PlayerData->LevelUpPoint < Pontos || Pontos < 1)
			{
				GSProtocol.GCServerMsgStringSend(Lang.GetText(0,435),lpObj->m_Index,1);
				return FALSE;
			}
			if((Pontos + lpObj->Leadership) > g_MaxCommand)
			{
				GSProtocol.GCServerMsgStringSend(Lang.GetText(0,436),lpObj->m_Index,1);
				return FALSE;
			}

			lpObj->Leadership += Pontos;
			lpObj->m_PlayerData->LevelUpPoint -= Pontos;

			PMSG_ADDSTATS pStats;
			pStats.h.c = 0xC1;
			pStats.h.headcode = 0xFA;
			pStats.h.size = sizeof(pStats);
			pStats.subcode = 0x90;
			pStats.Points = lpObj->Leadership;
			pStats.statstype = 4;
			pStats.LUP = lpObj->m_PlayerData->LevelUpPoint;
			IOCP.DataSend(aIndex,(LPBYTE)&pStats,sizeof(pStats));

			gObjCalCharacter.CalcCharacter(aIndex);
			return TRUE;
		}
		case 435:
		{
			int Pontos;
			Pontos = GetTokenNumber();
			if(Pontos == NULL)
			{
				GSProtocol.GCServerMsgStringSend(Lang.GetText(0,434),lpObj->m_Index,1);
				return FALSE;
			}
			if(lpObj->m_PlayerData->LevelUpPoint < Pontos || Pontos < 1)
			{
				GSProtocol.GCServerMsgStringSend(Lang.GetText(0,435),lpObj->m_Index,1);
				return FALSE;
			}
			if((Pontos + lpObj->m_PlayerData->Strength) > g_MaxStrength)
			{
				GSProtocol.GCServerMsgStringSend(Lang.GetText(0,436),lpObj->m_Index,1);
				return FALSE;
			}
			lpObj->m_PlayerData->LevelUpPoint -= Pontos;
			lpObj->m_PlayerData->Strength += Pontos;
			char Msg[100];
			sprintf(Msg, Lang.GetText(0,437), Pontos);
			GSProtocol.GCServerMsgStringSend(Msg,lpObj->m_Index,1);
			return TRUE;
		}
		break;
		case 436:
		{
			int Pontos = GetTokenNumber();

			if(Pontos == NULL)
			{
				GSProtocol.GCServerMsgStringSend(Lang.GetText(0,434),lpObj->m_Index,1);
				return FALSE;
			}
			if(lpObj->m_PlayerData->LevelUpPoint < Pontos || Pontos < 1)
			{
				GSProtocol.GCServerMsgStringSend(Lang.GetText(0,435),lpObj->m_Index,1);
				return FALSE;
			}
			if((Pontos + lpObj->m_PlayerData->Dexterity) > g_MaxAgility)
			{
				GSProtocol.GCServerMsgStringSend(Lang.GetText(0,436),lpObj->m_Index,1);
				return FALSE;
			}

			EnterCriticalSection(&lpObj->m_PlayerData->AgiCheckCriti);
			lpObj->m_PlayerData->AgilityCheckDelay = GetTickCount();
			lpObj->m_PlayerData->Dexterity += Pontos;
			lpObj->m_PlayerData->LevelUpPoint -= Pontos;
			LeaveCriticalSection(&lpObj->m_PlayerData->AgiCheckCriti);

			char Msg[100];
			sprintf(Msg, Lang.GetText(0,438), Pontos);
			GSProtocol.GCServerMsgStringSend(Msg,lpObj->m_Index,1);

			return TRUE;
		}
		break;
		case 437:
		{
			int Pontos;
			Pontos = GetTokenNumber();
			if(Pontos == NULL)
			{
				GSProtocol.GCServerMsgStringSend(Lang.GetText(0,434),lpObj->m_Index,1);
				return FALSE;
			}
			if(lpObj->m_PlayerData->LevelUpPoint < Pontos || Pontos < 1)
			{
				GSProtocol.GCServerMsgStringSend(Lang.GetText(0,435),lpObj->m_Index,1);
				return FALSE;
			}
			if((Pontos + lpObj->m_PlayerData->Vitality) > g_MaxVitality)
			{
				GSProtocol.GCServerMsgStringSend(Lang.GetText(0,407),lpObj->m_Index,1);
				return FALSE;
			}

			lpObj->m_PlayerData->LevelUpPoint -= Pontos;
			lpObj->m_PlayerData->Vitality += Pontos;
			char Msg[100];
			sprintf(Msg, Lang.GetText(0,439), Pontos);
			GSProtocol.GCServerMsgStringSend(Msg,lpObj->m_Index,1);
			gObjCalCharacter.CalcCharacter(lpObj->m_Index);
			return TRUE;
		}
		case 438:
		{
			int Pontos;
			Pontos = GetTokenNumber();
			if(Pontos == NULL)
			{
				GSProtocol.GCServerMsgStringSend(Lang.GetText(0,434),lpObj->m_Index,1);
				return FALSE;
			}
			if(lpObj->m_PlayerData->LevelUpPoint < Pontos || Pontos < 1)
			{
				GSProtocol.GCServerMsgStringSend(Lang.GetText(0,435),lpObj->m_Index,1);
				return FALSE;
			}
			if((Pontos + lpObj->m_PlayerData->Energy) > g_MaxEnergy)
			{
				GSProtocol.GCServerMsgStringSend(Lang.GetText(0,436),lpObj->m_Index,1);
				return FALSE;
			}

			lpObj->m_PlayerData->Energy += Pontos;
			lpObj->m_PlayerData->LevelUpPoint -= Pontos;
			char Msg[100];
			sprintf(Msg, Lang.GetText(0,440), Pontos);
			GSProtocol.GCServerMsgStringSend(Msg,lpObj->m_Index,1);
			gObjCalCharacter.CalcCharacter(lpObj->m_Index);
			return TRUE;
			break;
		}
		case 439:
		{
			if(lpObj->Class != 4)
			{
				GSProtocol.GCServerMsgStringSend(Lang.GetText(0,441),lpObj->m_Index,1);
				return FALSE;
			}

			int Pontos;
			Pontos = GetTokenNumber();
			if(Pontos == NULL)
			{
				GSProtocol.GCServerMsgStringSend(Lang.GetText(0,434),lpObj->m_Index,1);
				return FALSE;
			}
			if(lpObj->m_PlayerData->LevelUpPoint < Pontos || Pontos < 1)
			{
				GSProtocol.GCServerMsgStringSend(Lang.GetText(0,435),lpObj->m_Index,1);
				return FALSE;
			}
			if((Pontos + lpObj->Leadership) > g_MaxCommand)
			{
				GSProtocol.GCServerMsgStringSend(Lang.GetText(0,436),lpObj->m_Index,1);
				return FALSE;
			}

			lpObj->Leadership += Pontos;
			lpObj->m_PlayerData->LevelUpPoint -= Pontos;
			char Msg[100];
			sprintf(Msg, Lang.GetText(0,442), Pontos);
			GSProtocol.GCServerMsgStringSend(Msg,lpObj->m_Index,1);
			return TRUE;
		}

		case 423:
			{
				SYSTEMTIME t;
				GetLocalTime(&t);
				PMSG_NOTICE pNotice;
				TNotice::MakeNoticeMsgEx(&pNotice, 0, Lang.GetText(0,443), g_ConfigRead.server.GetServerName(), g_ConfigRead.server.GetGameServerCode(), gObjTotalUser, g_ConfigRead.server.GetObjectMaxUser());
				TNotice::SendNoticeToUser(aIndex, &pNotice);
				TNotice::MakeNoticeMsgEx(&pNotice, 0, Lang.GetText(0,444), t.wHour, t.wMinute, t.wSecond, t.wDay, t.wMonth, t.wYear);
				TNotice::SendNoticeToUser(aIndex, &pNotice);
				break;
			}

		case 390:
			{
				if ( lpObj->Authority != 0x20 )
				{
					MsgOutput(aIndex, Lang.GetText(0,417));
					return FALSE;
				}

				if ( (lpObj->GameMaster & GM_COMMAND_ITEM_CREATE) != GM_COMMAND_ITEM_CREATE )
				{
					MsgOutput(lpObj->m_Index, Lang.GetText(0,418));
					return 0;
				}

				if(gIsDropCmd == 0)
				{
					return 0;
				}

				int type, index, ItemLevel, ItemSkill, ItemLuck, ItemOpt, ItemExc, ItemAncient, ItemExpireTime, ItemSocketCount, MainAttribute, MuunEvoItemType, MuunEvoItemIndex;
				type = GetTokenNumber();
				index = GetTokenNumber();
				ItemLevel = GetTokenNumber();
				ItemSkill = GetTokenNumber();
				ItemLuck = GetTokenNumber();
				ItemOpt = GetTokenNumber();
				ItemExc = GetTokenNumber();
				ItemAncient = GetTokenNumber();
				ItemExpireTime = GetTokenNumber();
				ItemSocketCount = GetTokenNumber();
				MainAttribute = GetTokenNumber();
				MuunEvoItemType = GetTokenNumber();
				MuunEvoItemIndex = GetTokenNumber();
		
				if( (type >= 0 && type <= 15) )
				{
					CItem Item;
					Item.m_Type = ItemGetNumberMake(type, index);

					if (g_SocketOptionSystem.IsEnableSocketItem(&Item) == true)
					{
						if (ItemSocketCount == 0)
						{
							g_SocketOptionSystem.MakeSocketSlot(&Item, rand() % 5 + 1);
						}

						else
						{
							g_SocketOptionSystem.MakeSocketSlot(&Item, ItemSocketCount);
						}

						MainAttribute = -1;
					}

					else if (g_PentagramSystem.IsPentagramItem(Item.m_Type) == true)
					{
						if (ItemSocketCount == 0)
						{
							g_PentagramSystem.GetMakePentagramSlotCountNKind(Item.m_SocketOption, Item.m_Type);
						}

						else
						{
							BYTE Slot[5] = { 0, 0, 0, 0, 0 };

							for (int i = 0; i < ItemSocketCount; i++)
							{
								Slot[i] = 1;
							}

							g_PentagramSystem.MakePentagramSocketSlot(&Item, Slot[0], Slot[1], Slot[2], Slot[3], Slot[4]);
						}
					}

					else if (g_CMuunSystem.IsStoneofEvolution(Item.m_Type) == TRUE)
					{
						int iMuunEvolutionItemID = ITEMGET(MuunEvoItemType, MuunEvoItemIndex);

						Item.m_SocketOption[0] = iMuunEvolutionItemID >> 8;
						Item.m_SocketOption[1] = ITEM_GET_INDEX(iMuunEvolutionItemID);
					}

					if (ItemExc == -1)
					{
						LPITEM_ATTRIBUTE p = GetItemAttr(Item.m_Type);

						if (!p)
						{
							return 0;
						}

						ItemExc = g_ItemOptionTypeMng.CommonExcOptionRand(p->ItemKindA);
					}

					if(type == 13 && (index == 4 || index == 5))
					{
						PetItemSerialCreateSend(aIndex, gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y, Item.m_Type,ItemLevel,0,ItemSkill,ItemLuck,ItemOpt,-1,ItemExc,ItemAncient);
					}
					else
					{
						ItemSerialCreateSend(aIndex, 227, gObj[aIndex].X, gObj[aIndex].Y, Item.m_Type, ItemLevel, 0, ItemSkill, ItemLuck, ItemOpt, -1, ItemExc, ItemAncient, ItemExpireTime, Item.m_SocketOption, MainAttribute);
					}

					g_Log.Add( "[%s][%s][%s] Created Item using Admin Command (%s/%d/%d) Auth: %d", lpObj->AccountID, lpObj->Name, lpObj->m_PlayerData->Ip_addr, Lang.GetMap(0, lpObj->MapNumber), lpObj->X, lpObj->Y, lpObj->Authority);
					GMLog->Output("[%s][%s][%s] Created Item using Admin Command (%s/%d/%d) Auth: %d", lpObj->AccountID, lpObj->Name, lpObj->m_PlayerData->Ip_addr, Lang.GetMap(0, lpObj->MapNumber), lpObj->X, lpObj->Y, lpObj->Authority);
				}
				break;
			}
			
		case 399:
			{
				MsgOutput(aIndex, Lang.GetText(0,445));
				MsgOutput(aIndex, Lang.GetText(0,446), lpObj->AccountID, lpObj->Name, lpObj->Level, lpObj->m_PlayerData->MasterLevel, lpObj->Class);
				MsgOutput(aIndex, Lang.GetText(0,447), lpObj->m_PlayerData->SwearWarning, Lang.GetMap(0, lpObj->MapNumber), lpObj->X, lpObj->Y);
				MsgOutput(aIndex, Lang.GetText(0,448), gObjIsConnectedGP(gObjGetIndex(gObj[aIndex].MarryName)));
				break;
			}

		case 404:
			{
				pId = this->GetTokenString();

				if(pId == 0)
				{
					return 0;
				}
				
				Marry.Propose(aIndex, gObjGetIndex(pId));
				break;
			}
		case 405:
			{
				Marry.Accept(aIndex);
				break;
			}

		case 421:
			{
				pId = this->GetTokenString();
				int pklevel = this->GetTokenNumber();
				int pkcount = this->GetTokenNumber();

				if(pId == 0)
				{
					return 0;
				}

				if ( (lpObj->GameMaster & GM_COMMAND_PK_MODIFY) != GM_COMMAND_PK_MODIFY )
				{
					MsgOutput(lpObj->m_Index, Lang.GetText(0,449));
					return 0;
				}

				short index = gObjGetIndex(pId);

				if(index == -1) break;
				gObj[index].m_PK_Level = pklevel;
				gObj[index].m_PK_Count = pkcount;
				GSProtocol.GCPkLevelSend(index, gObj[index].m_PK_Level);
				break;
			}

		case 422:
			{
				Marry.Divorce(aIndex);
				break;
			}

		case 406:
			{
				if(gObj[aIndex].Married == 0)
					return 0;

				if(strlen(gObj[aIndex].MarryName) <= 0)
					return 0;

				int uindex = gObjGetIndex(gObj[aIndex].MarryName);

				if(uindex == -1)
					return 0;

				if(this->CheckTraceMarryCondition(&gObj[aIndex], &gObj[uindex]) == FALSE)
				{
					return 0;
				}

				int nSrcMapNumber = gObj[aIndex].MapNumber;
				int nTarMapNumber = gObj[uindex].MapNumber;

				gObjTeleport(aIndex, gObj[uindex].MapNumber, gObj[uindex].X, gObj[uindex].Y);
				if (nSrcMapNumber != nTarMapNumber)
				{
					g_SendNPCInfo.SendNpcCoordinate(aIndex, nTarMapNumber, 0);
					g_SendNPCInfo.SendPortalCoordinate(aIndex, nTarMapNumber);
				}				break;
			}

		case 392:
			{
				if(!g_ConfigRead.pk.bPkClearEnable)
					break;

				if (gObj[aIndex].m_PK_Level <= 3)
				{
					MsgOutput(aIndex, Lang.GetText(0,501));
					return 0;
				}

				UINT64 Price;

				if(g_ConfigRead.pk.bIsCostMultiplyByKillCount)
				{
					Price = g_ConfigRead.pk.iPkClearCost * gObj[aIndex].m_PK_Count;
				}
				else
				{
					Price = g_ConfigRead.pk.iPkClearCost;
				}
				if (Price > MAX_ZEN)
				{
					Price = MAX_ZEN;
				}

				if(gObj[aIndex].m_PlayerData->Money < Price)
				{
					MsgOutput(aIndex, Lang.GetText(0,450), Price);
				}
				else
				{
					gObj[aIndex].m_PlayerData->Money -= Price;
					GSProtocol.GCMoneySend(aIndex, gObj[aIndex].m_PlayerData->Money);
					gObj[aIndex].m_PK_Level = 3;
					gObj[aIndex].m_PK_Count = 0;
					GSProtocol.GCPkLevelSend(aIndex, gObj[aIndex].m_PK_Level);
				}
				break;
			}
		case 403: // multi warehouse by drakelv
			{
				if(!IsMultiWareHouse)
				{
					return 0;
				}

				int WareID = this->GetTokenNumber();

				if(gObj[aIndex].m_IfState.use == 1 && gObj[aIndex].m_IfState.type == 6)
				{
					MsgOutput(aIndex, Lang.GetText(0,111));
					return 0;
				}

				if (gObj[aIndex].m_btOpenWarehouse == TRUE)
				{
					MsgOutput(aIndex, Lang.GetText(0, 111));
					return 0;
				}

				if(WareID < 0 || WareID > g_WareHouseCount)
				{
					MsgOutput(aIndex, Lang.GetText(0,110));
					return 0;
				}

				int Time = (GetTickCount() - gObj[aIndex].WarehouseTick) / 1000;

				if(Time < 30)
				{
					MsgOutput(aIndex, Lang.GetText(0,451), 30 - Time);
					return 0;
				}

				if(gObj[aIndex].WarehouseSaveLock == TRUE)
				{
					MsgOutput(aIndex, Lang.GetText(0,598));
					return 0;
				}

				GDReqSwitchWare(aIndex, WareID);
			}
			break;
				
		case 203:	//112:
			{
				pId = this->GetTokenString();

				if ( pId != NULL )
				{
					BOOL bState;

					if ( strcmp(pId, "on" ) == 0 )
					{
						bState = TRUE;
					}
					else if ( strcmp(pId, "off") == 0 )
					{
						bState = FALSE;
					}

					if ( bState >= FALSE && bState <= TRUE )
					{
						gObjSetTradeOption(aIndex, bState);
						gObjSetDuelOption(aIndex, bState);
					}
				}
			}
			break;

		case 320:	//117:
			{
				if ( (lpObj->Authority &2) != 2 && (lpObj->Authority &0x20) != 0x20 )
				{
					return FALSE;
				}

				if ( (lpObj->GameMaster & GM_EVENT_MODIFY) != GM_EVENT_MODIFY )
				{
					MsgOutput(lpObj->m_Index, Lang.GetText(0,453));
					return 0;
				}

				g_Log.Add("[KUNDUN] Use GM Command -> [ %s ]\t[ %s ]\t[ %s ] : %s", lpObj->m_PlayerData->Ip_addr, lpObj->AccountID,
					lpObj->Name, "Kundun Event Start");

				for ( int n=0;n<MAX_VIEWPORT;n++)
				{
					if ( lpObj->VpPlayer[n].state != 0 )
					{
						if ( lpObj->VpPlayer[n].type == OBJ_MONSTER )
						{
							if ( lpObj->VpPlayer[n].number >= 0 )
							{
								LPOBJ lpTarget = &gObj[lpObj->VpPlayer[n].number];

								if ( lpTarget->Class == 275 )
								{
									TNotice pNotice(1);

									pNotice.SendToUser(lpObj->m_Index, "KUNDUN HP = %7.0f / %7.0f", lpTarget->Life, lpTarget->MaxLife);
									pNotice.SendToUser(lpObj->m_Index, "HP Refill Sec = %d HP Refill Value = %d HP Refill Time = %d", giKundunRefillHPSec, giKundunRefillHP, giKundunRefillHPTime);
								}
							}
						}
					}
				}

			}
			break;

		case 321:	//118:
			{
				if ( (lpObj->Authority &2 ) != 2 && (lpObj->Authority &0x20) != 0x20 )
				{
					return FALSE;
				}

				if ( (lpObj->GameMaster & GM_EVENT_MODIFY) != GM_EVENT_MODIFY )
				{
					MsgOutput(lpObj->m_Index, Lang.GetText(0,453));
					return 0;
				}

				g_Log.Add("[KUNDUN] Use GM Command -> [ %s ]\t[ %s ]\t[ %s ] : %s", lpObj->m_PlayerData->Ip_addr, lpObj->AccountID,
					lpObj->Name, "Kundun SetHP");

				int iLife = this->GetTokenNumber();

				for ( int n=0;n<MAX_VIEWPORT;n++)
				{
					if ( lpObj->VpPlayer[n].state != FALSE )
					{
						if ( lpObj->VpPlayer[n].type == OBJ_MONSTER )
						{
							if ( lpObj->VpPlayer[n].number >= 0 )
							{
								LPOBJ lpTarget = &gObj[lpObj->VpPlayer[n].number];

								if ( lpTarget->Class == 275 )
								{
									if  ( iLife <= 5000 )
									{
										iLife = 5000 ;
									}

									if ( iLife > lpTarget->MaxLife )
									{
										iLife = lpTarget->MaxLife;
									}

									lpTarget->Life = iLife;
								
									TNotice pNotice(1);

									pNotice.SendToUser(lpObj->m_Index, "Kundun HP = %7.0f / %7.0f", lpTarget->Life, lpTarget->MaxLife);
								}
							}
						}
					}
				}
			}
			break;

		case 322:	//119:
			{
				if ( (lpObj->Authority &2 ) != 2 && (lpObj->Authority &0x20) != 0x20 )
				{
					return FALSE;
				}

				if ( (lpObj->GameMaster & GM_EVENT_MODIFY) != GM_EVENT_MODIFY )
				{
					MsgOutput(lpObj->m_Index, Lang.GetText(0,453));
					return 0;
				}

				g_Log.Add("[KUNDUN] Use GM Command -> [ %s ]\t[ %s ]\t[ %s ] : %s", lpObj->m_PlayerData->Ip_addr, lpObj->AccountID,
					lpObj->Name, "Kundun SetHPRefill");

				int RefillHP = this->GetTokenNumber();

				if ( RefillHP <= 0 || RefillHP > 5000000 )
				{
					return 0;
				}

				giKundunRefillHP = RefillHP;

				TNotice pNotice(0);

				pNotice.SendToUser(lpObj->m_Index, "Kundun HP Refill Sec = %d Refill HP = %d Refill HP Time = %d", giKundunRefillHPSec, giKundunRefillHP, giKundunRefillHPTime);

			}

			break;

		case 323:	//120:
			{
				if ( (lpObj->Authority &2 ) != 2 && (lpObj->Authority &0x20) != 0x20 )
				{
					return FALSE;
				}

				if ( (lpObj->GameMaster & GM_EVENT_MODIFY) != GM_EVENT_MODIFY )
				{
					MsgOutput(lpObj->m_Index, Lang.GetText(0,453));
					return 0;
				}

				g_Log.Add("[KUNDUN] Use GM Command -> [ %s ]\t[ %s ]\t[ %s ] : %s", lpObj->m_PlayerData->Ip_addr, lpObj->AccountID,
					lpObj->Name, "Kundun SetHPRefillSec");

				int RefillHPSec = this->GetTokenNumber();

				if ( RefillHPSec <= 0 || RefillHPSec > 10000 )
				{
					return 0;
				}

				giKundunRefillHPSec = RefillHPSec;

				TNotice pNotice(0);

				pNotice.SendToUser(lpObj->m_Index, "Kundun HP Refill Sec = %d Refill HP = %d Refill HP Time = %d", giKundunRefillHPSec, giKundunRefillHP, giKundunRefillHPTime);
			}

			break;

		case 324:	//121:
			{
				if ( (lpObj->Authority &2 ) != 2 && (lpObj->Authority &0x20) != 0x20 )
				{
					return FALSE;
				}

				if ( (lpObj->GameMaster & GM_EVENT_MODIFY) != GM_EVENT_MODIFY )
				{
					MsgOutput(lpObj->m_Index, Lang.GetText(0,453));
					return 0;
				}

				g_Log.Add("[KUNDUN] Use GM Command -> [ %s ]\t[ %s ]\t[ %s ] : %s", lpObj->m_PlayerData->Ip_addr, lpObj->AccountID,
					lpObj->Name, "Kundun SetHPRefillTime");

				int RefillHPTime = this->GetTokenNumber();

				if ( RefillHPTime < 0 || RefillHPTime > 60000 )
				{
					return 0;
				}

				giKundunRefillHPTime = RefillHPTime;

				TNotice pNotice(0);

				pNotice.SendToUser(lpObj->m_Index, "Kundun HP Refill Sec = %d Refill HP = %d Refill HP Time = %d", giKundunRefillHPSec, giKundunRefillHP, giKundunRefillHPTime);

			}
			break;
		case 400:
			{
				int oncount = 0;
				int gmoncount = 0;
				for(int g= g_ConfigRead.server.GetObjectStartUserIndex();g<g_ConfigRead.server.GetObjectMax();g++)
				{
					if(gObj[g].Connected == 3)
					{
						oncount++;
					}
					if(gObj[g].Connected == 3 && gObj[g].Authority == 32)
					{
						gmoncount++;
					}
				}
				MsgOutput(lpObj->m_Index,Lang.GetText(0,454),oncount,gmoncount);
				return TRUE;
			}
			break;
		case 401:
			{
				if ((lpObj->Authority & 2) != 2 && (lpObj->Authority & 0x20) != 0x20)
				{
					return FALSE;
				}

				pId = this->GetTokenString();

				if (pId == NULL)
				{
					return 0;
				}

				int iTargetIndex = gObjGetIndex(pId);

				if (!ObjectMaxRange(iTargetIndex))
				{
					return 0;
				}

				if (gObj[iTargetIndex].Type != OBJ_USER)
				{
					return 0;
				}

				MsgOutput(lpObj->m_Index, "Character: %s", gObj[iTargetIndex].Name);
				MsgOutput(lpObj->m_Index, "AccountID: %s, Level:%d MasterLevel:%d", gObj[iTargetIndex].AccountID, gObj[iTargetIndex].Level, gObj[iTargetIndex].m_PlayerData->MasterLevel);
				MsgOutput(lpObj->m_Index, "Map: %s, X:%d Y:%d", Lang.GetMap(0, gObj[iTargetIndex].MapNumber), gObj[iTargetIndex].X, gObj[iTargetIndex].Y);

			}
			break;
		case 398:
		{
			int value;
			value = GetTokenNumber();

			g_Crywolf.OperateGmCommand(lpObj->m_Index,value);
		}
		break;
		case 408:
			{
				if( (gObj[aIndex].Authority & 2) != 2 && (gObj[aIndex].Authority & 0x20) != 0x20 ) /*Check if user is GM*/
					return 0;

				if( (gObj[aIndex].GameMaster & GM_EVENT_MODIFY) != GM_EVENT_MODIFY )
				{
					MsgOutput(aIndex, Lang.GetText(0,453));
					return 0;
				}

				g_BloodCastle.SetCheatEventOpen(350000);
				MsgOutput(aIndex,Lang.GetText(0,455));
			}
			break;
		case 409:
			{
				if( (gObj[aIndex].Authority & 2) != 2 && (gObj[aIndex].Authority & 0x20) != 0x20 ) /*Check if user is GM*/
					return 0;

				if( (gObj[aIndex].GameMaster & GM_EVENT_MODIFY) != GM_EVENT_MODIFY )
				{
					MsgOutput(aIndex, Lang.GetText(0,453));
					return 0;
				}

				g_DevilSquare.SetCheatOpenTime(1);
				MsgOutput(aIndex,Lang.GetText(0,456));
			}
			break;
		case 410:
			{
				if( (gObj[aIndex].Authority & 2) != 2 && (gObj[aIndex].Authority & 0x20) != 0x20 ) /*Check if user is GM*/
					return 0;

				if( (gObj[aIndex].GameMaster & GM_EVENT_MODIFY) != GM_EVENT_MODIFY )
				{
					MsgOutput(aIndex, Lang.GetText(0,453));
					return 0;
				}

				for(int i=0;i<MAX_CHAOSCASTLE_LEVEL;i++)
				{
					g_ChaosCastle.m_stChaosCastleData[i].m_iCC_REMAIN_MSEC = 350000;
				}
				MsgOutput(aIndex,Lang.GetText(0,457));
			}
			break;
		case 412:
			{
				if(!g_ConfigRead.data.reset.enable)
					return 0;

				if(lpObj->Level < g_ConfigRead.data.reset.iMinLevel)
				{
					MsgOutput(aIndex,Lang.GetText(0,458),g_ConfigRead.data.reset.iMinLevel);
					return 0;
				}
				if(lpObj->m_PlayerData->Money < g_ConfigRead.data.reset.iCost)
				{
					MsgOutput(aIndex,Lang.GetText(0,459),g_ConfigRead.data.reset.iCost);
					return 0;
				}

				if(lpObj->m_PlayerData->m_iResets >= g_ConfigRead.data.reset.iResetLimit)
				{
					MsgOutput(aIndex,Lang.GetText(0,460));
					return 0;
				}

				if (BC_MAP_RANGE(lpObj->MapNumber) || DS_MAP_RANGE(lpObj->MapNumber) || CC_MAP_RANGE(lpObj->MapNumber) || IT_MAP_RANGE(lpObj->MapNumber) ||
					DG_MAP_RANGE(lpObj->MapNumber) || IMPERIAL_MAP_RANGE(lpObj->MapNumber) || CRYWOLF_MAP_RANGE(lpObj->MapNumber) || lpObj->MapNumber == MAP_INDEX_CASTLESIEGE ||
					lpObj->MapNumber == MAP_INDEX_CASTLEHUNTZONE || g_GensSystem.IsMapBattleZone(lpObj->MapNumber) || lpObj->MapNumber == MAP_INDEX_KANTURU_BOSS || lpObj->MapNumber == MAP_INDEX_KALIMA1
					|| lpObj->MapNumber == MAP_INDEX_KALIMA2 || lpObj->MapNumber == MAP_INDEX_KALIMA3 || lpObj->MapNumber == MAP_INDEX_KALIMA4 || lpObj->MapNumber == MAP_INDEX_KALIMA5 || lpObj->MapNumber == MAP_INDEX_KALIMA6
					|| lpObj->MapNumber == MAP_INDEX_KALIMA7 || lpObj->MapNumber == MAP_INDEX_ARCA_WAR || lpObj->MapNumber == MAP_INDEX_DEBENTER_ARCA_WAR)
				{
					MsgOutput(aIndex, Lang.GetText(0,510));
					return FALSE;
				}

				if(g_ConfigRead.data.reset.RemoveEquipment)
				{
					for(int i=0;i<12;i++)
					{
						if(lpObj->pInventory[i].IsItem() == true)
						{
							MsgOutput(aIndex,Lang.GetText(0,461));
							return 0;
						}
					}
				}

				if (lpObj->PartyNumber >= 0)
				{
					PMSG_PARTYDELUSER pMsg;
					int pindex = gParty.GetIndex(lpObj->PartyNumber, lpObj->m_Index, lpObj->DBNumber);

					if (pindex >= 0)
					{
						pMsg.Number = pindex;
						GSProtocol.CGPartyDelUser(&pMsg, lpObj->m_Index, 0);
					}
				}

				if (lpObj->m_SkillInfo.LordSummonTime != 0)
				{
					lpObj->m_SkillInfo.LordSummonTime = 0;
					GSProtocol.GCServerMsgStringSend(Lang.GetText(0, 146), aIndex, 1);
				}

				if (lpObj->m_PlayerData->m_MuBotEnable == true)
				{
					lpObj->m_PlayerData->m_MuBotEnable = false;
					GSProtocol.GCAnsMuBotUse(lpObj->m_Index, 1, lpObj->m_PlayerData->m_MuBotTotalTime, 0);
				}

				gObjClearBuffEffect(lpObj, CLEAR_TYPE_NON_PCS_ITEM_EFFECT);

				lpObj->Level = 1;
				lpObj->m_PlayerData->Experience = 0;
				lpObj->m_PlayerData->NextExp = 0;
				bool CharQuit = false;

				if(g_ConfigRead.data.reset.ResetML == true)
				{
					lpObj->m_PlayerData->MasterLevel = 0;
					lpObj->m_PlayerData->MasterExperience = 0;
					lpObj->m_PlayerData->MasterNextExp = g_MasterLevelSkillTreeSystem.gObjNextMLExpCal(lpObj);
				}

				if(g_ConfigRead.data.reset.ResetStats == true)
				{
					if(g_ConfigRead.data.reset.SaveOldPoint == FALSE)
					{
						lpObj->m_PlayerData->LevelUpPoint = 0;
					}

					EnterCriticalSection(&lpObj->m_PlayerData->AgiCheckCriti);
					lpObj->m_PlayerData->AgilityCheckDelay = GetTickCount();
					lpObj->m_PlayerData->Strength = DCInfo.DefClass[lpObj->Class].Strength;
					lpObj->m_PlayerData->Dexterity = DCInfo.DefClass[lpObj->Class].Dexterity;
					lpObj->m_PlayerData->Vitality = DCInfo.DefClass[lpObj->Class].Vitality;
					lpObj->m_PlayerData->Energy = DCInfo.DefClass[lpObj->Class].Energy;
					lpObj->Leadership = DCInfo.DefClass[lpObj->Class].Leadership;
					LeaveCriticalSection(&lpObj->m_PlayerData->AgiCheckCriti);

					if (lpObj->m_PlayerData->VipType == 0)
					{
						lpObj->m_PlayerData->LevelUpPoint += (lpObj->m_PlayerData->m_iResets + 1) * g_ConfigRead.data.reset.NormalPointPerReset;
					}

					else if (lpObj->m_PlayerData->VipType > 0)
					{
						lpObj->m_PlayerData->LevelUpPoint += (lpObj->m_PlayerData->m_iResets + 1) * g_VipSystem.GetPointPerReset(lpObj);
					}

					CharQuit = true;
				}

				lpObj->MaxLife = DCInfo.DefClass[lpObj->Class].Life + ((lpObj->Level+lpObj->m_PlayerData->MasterLevel) - 1) * DCInfo.DefClass[lpObj->Class].LevelLife  + ((lpObj->m_PlayerData->Vitality - DCInfo.DefClass[lpObj->Class].Vitality ) * DCInfo.DefClass[lpObj->Class].VitalityToLife);
				lpObj->MaxMana = DCInfo.DefClass[lpObj->Class].Mana + ((lpObj->Level+lpObj->m_PlayerData->MasterLevel) - 1) * DCInfo.DefClass[lpObj->Class].LevelMana  + ((lpObj->m_PlayerData->Energy - DCInfo.DefClass[lpObj->Class].Energy ) * DCInfo.DefClass[lpObj->Class].EnergyToMana);

				lpObj->m_PlayerData->m_iResets += 1;
				gObjNextExpCal(lpObj);
				lpObj->m_PlayerData->Money -=  g_ConfigRead.data.reset.iCost;
				GSProtocol.GCMoneySend(aIndex,lpObj->m_PlayerData->Money);
				gObjCalCharacter.CalcCharacter(lpObj->m_Index);
				GJSetCharacterInfo(lpObj,aIndex,0);
				GSProtocol.GCLevelUpMsgSend(aIndex,1);

				if(g_ConfigRead.data.reset.ResetML == true)
				{
					GSProtocol.GCMasterLevelUpMsgSend(aIndex);
				}

				if(lpObj->Class == CLASS_ELF)
				{
					gObjTeleport(aIndex,3,171,108);
				}
				else if(lpObj->Class == CLASS_SUMMONER)
				{
					gObjTeleport(aIndex,51,51,224);
				}
				else
				{
					gObjTeleport(aIndex,0,128,135);
				}

				MsgOutput(aIndex,Lang.GetText(0,462),lpObj->m_PlayerData->m_iResets);

				if(CharQuit == true || g_ConfigRead.data.reset.MoveToCharacterSelect == true)
				{
					MsgOutput(aIndex, Lang.GetText(0,463));
					gObjCloseSet(aIndex, 1);
				}
			}
			break;
		break;
		case 413:
			g_Kanturu.OperateGmCommand(lpObj->m_Index, 0);
			break;
		case 414:
			g_Kanturu.OperateGmCommand(lpObj->m_Index, 1);
			break;
		case 415:
			g_Kanturu.OperateGmCommand(lpObj->m_Index, 2);
			break;
		case 416:
			if((lpObj->Authority & 2) == 2 || (lpObj->Authority &0x20) == 0x20)
			{
				if ( (lpObj->GameMaster & GM_COMMAND_INVISIBLE) != GM_COMMAND_INVISIBLE )
				{
					MsgOutput(lpObj->m_Index, Lang.GetText(0,464));
					return 0;
				}

				gObjAddBuffEffect(lpObj, BUFFTYPE_INVISABLE, 0, 0, 0, 0, -10);
				gObjViewportListProtocolDestroy(lpObj);
			}
			break;
		case 417:
			if((lpObj->Authority & 2) == 2 || (lpObj->Authority &0x20) == 0x20)
			{
				if ( (lpObj->GameMaster & GM_COMMAND_INVISIBLE) != GM_COMMAND_INVISIBLE )
				{
					MsgOutput(lpObj->m_Index, Lang.GetText(0,464));
					return 0;
				}

				gObjRemoveBuffEffect(lpObj, BUFFTYPE_INVISABLE);
				gObjViewportListProtocolCreate(lpObj);
			}
			break;
		case 418:
			for(int i=0;i<INVENTORY_SIZE;++i)
			{
				if(lpObj->pInventory[i].IsItem() == TRUE)
				{
					if (g_PentagramSystem.IsPentagramItem(&lpObj->pInventory[i]) == true)
					{
						for (int j = 0; j < 5; j++)
						{
							if (lpObj->pInventory[i].m_SocketOption[j] < 0xFE)
							{
								g_PentagramSystem.DelPentagramJewelInfo(aIndex, 0, lpObj->pInventory[i].m_SocketOption[j]);
							}
						}

						g_PentagramSystem.GCPentagramJewelInfo(aIndex, 0);
					}

					gObjInventoryDeleteItem(aIndex, i);
					GSProtocol.GCInventoryItemDeleteSend(aIndex, i, 1);
				}

				
			}
			lpObj->m_PlayerData->Money = 0;
			GSProtocol.GCMoneySend(lpObj->m_Index, lpObj->m_PlayerData->Money);
			g_Log.AddC(TColor::Red,  "[%s][%s] Cleared inventory", lpObj->AccountID, lpObj->Name);
			MsgOutput(lpObj->m_Index, Lang.GetText(0,465));
			break;
		case 420:
			{
				if((lpObj->Authority & 2) != 2 && (lpObj->Authority &0x20) != 0x20)
					break;

				if ( (lpObj->GameMaster & GM_COMMAND_SKIN) != GM_COMMAND_SKIN )
				{
					MsgOutput(lpObj->m_Index, Lang.GetText(0,466));
					return 0;
				}

				int skin = GetTokenNumber();
				if(skin <= 0 && skin > 1024)
				{
					lpObj->m_Change = -1;
					gObjViewportListProtocolCreate(lpObj);
					break;
				}

				

				lpObj->m_Change = skin;
				gObjViewportListProtocolCreate(lpObj);
			}
			break;
		case 424:
			{
				if((lpObj->Authority & 2) != 2 && (lpObj->Authority &0x20) != 0x20)
					break;

				if ( (lpObj->GameMaster & GM_COMMAND_BAN) != GM_COMMAND_BAN )
				{
					MsgOutput(lpObj->m_Index, Lang.GetText(0,467));
					return 0;
				}

				pId = this->GetTokenString();

				if (pId == NULL)
				{
					return 0;
				}

				for(int i=g_ConfigRead.server.GetObjectStartUserIndex();i<g_ConfigRead.server.GetObjectMax();i++)
				{
					if(gObj[i].Connected >= PLAYER_LOGGED)
					{
						if(gObj[i].AccountID[0] == pId[0])
						{
							if(gObj[i].AccountID[1] == pId[1])
							{
								if(!strcmp(gObj[i].AccountID, pId))
								{
									GDReqBanUser(i, 1, 1);
									MsgOutput(aIndex, Lang.GetText(0,468), pId);
									return 1;
								}
							}
						}
					}
				}
				if(g_ConfigRead.data.common.IsJoinMu)
				{
					BAN_REQ_USER pBan;
					PHeadSetB((LPBYTE)&pBan, 0xFA, sizeof(pBan));

					memcpy(pBan.AccName, pId, sizeof(pBan.AccName));
					pBan.Ban = 1;
					pBan.Type = 1;

					wsDataCli.DataSend((char *)&pBan, pBan.h.size);
				}
				else
				{
					MsgOutput(aIndex, Lang.GetText(0,469), pId);
				}
			}
			break;
		case 425:
			{
				if((lpObj->Authority & 2) != 2 && (lpObj->Authority &0x20) != 0x20)
					break;

				if ( (lpObj->GameMaster & GM_COMMAND_BAN) != GM_COMMAND_BAN )
				{
					MsgOutput(lpObj->m_Index, Lang.GetText(0,467));
					return 0;
				}

				pId = this->GetTokenString();

				if (pId == NULL)
				{
					return 0;
				}

				BAN_REQ_USER pBan;
				PHeadSetB((LPBYTE)&pBan, 0xFA, sizeof(pBan));

				memcpy(pBan.AccName, pId, sizeof(pBan.AccName));
				pBan.Ban = 0;
				pBan.Type = 1;

				wsDataCli.DataSend((char *)&pBan, pBan.h.size);

				MsgOutput(aIndex, Lang.GetText(0,470), pId);
			}
			break;
		case 426:
			{
				if((lpObj->Authority & 2) != 2 && (lpObj->Authority &0x20) != 0x20)
					break;

				if ( (lpObj->GameMaster & GM_COMMAND_BAN) != GM_COMMAND_BAN )
				{
					MsgOutput(lpObj->m_Index, Lang.GetText(0,467));
					return 0;
				}

				pId = this->GetTokenString();

				if (pId == NULL)
				{
					return 0;
				}

				LPOBJ lpBan = gObjFind(pId);
				
				if(lpBan == NULL && !g_ConfigRead.data.common.IsJoinMu)
				{
					MsgOutput(aIndex, Lang.GetText(0,471), pId);
					return 0;
				}
				else if(g_ConfigRead.data.common.IsJoinMu && lpBan == NULL)
				{
					BAN_REQ_USER pBan;
					PHeadSetB((LPBYTE)&pBan, 0xFA, sizeof(pBan));

					memcpy(pBan.AccName, pId, sizeof(pBan.AccName));
					pBan.Ban = 1;
					pBan.Type = 0;

					wsDataCli.DataSend((char *)&pBan, pBan.h.size);
					return 1;
				}

				GDReqBanUser(lpBan->m_Index, 0, 1);
				MsgOutput(aIndex, Lang.GetText(0,472), pId);
			}
			break;
		case 427:
			{
				if((lpObj->Authority & 2) != 2 && (lpObj->Authority &0x20) != 0x20)
					break;

				if ( (lpObj->GameMaster & GM_COMMAND_BAN) != GM_COMMAND_BAN )
				{
					MsgOutput(lpObj->m_Index, Lang.GetText(0,467));
					return 0;
				}

				pId = this->GetTokenString();

				if (pId == NULL)
				{
					return 0;
				}

				BAN_REQ_USER pBan;
				PHeadSetB((LPBYTE)&pBan, 0xFA, sizeof(pBan));

				memcpy(pBan.AccName, pId, sizeof(pBan.AccName));
				pBan.Ban = 0;
				pBan.Type = 0;

				wsDataCli.DataSend((char *)&pBan, pBan.h.size);

				MsgOutput(aIndex, Lang.GetText(0,473), pId);
			}
			break;
		case 428:

			MsgOutput(aIndex, Lang.GetText(0,474));

			for(int i=g_ConfigRead.server.GetObjectStartUserIndex();i<g_ConfigRead.server.GetObjectMax();i++)
			{
				if(gObj[i].Connected == PLAYER_PLAYING)
				{
					if((gObj[i].Authority & 2) == 2 || (gObj[i].Authority & 0x20) == 0x20)
					{
						if(gObj[i].GameMaster != 0)
						{
							MsgOutput(aIndex, "%s", gObj[i].Name);
						}
					}
				}
			}
			break;

		case 431:
			{
				int code = GetTokenNumber();

				if(gObj[aIndex].m_PlayerData->m_iSecurityCode == code)
				{
					MsgOutput(aIndex, Lang.GetText(0,475));
					gObj[aIndex].m_PlayerData->m_bSecurityCheck = true;
				}

				else
				{
					MsgOutput(aIndex, Lang.GetText(0,476));
				}
			}

			break;

		case 432:
			{
				int code = GetTokenNumber();

				GDReqSecLock(aIndex, code);
				gObj[aIndex].m_PlayerData->m_iSecurityCode = code;

				MsgOutput(aIndex, Lang.GetText(0,477), code);
			}

			break;
		case 433:
			{
				if(!g_ConfigRead.offtrade.enabled)
				{
					MsgOutput(aIndex,Lang.GetText(0,478));
					return 0;
				}

				if(!g_ConfigRead.offtrade.Maps[gObj[aIndex].MapNumber])
				{
					MsgOutput(aIndex,Lang.GetText(0,479));
					return 0;
				}

				if (g_ConfigRead.server.GetServerType() != SERVER_NORMAL && gObj[aIndex].MapNumber != MAP_INDEX_LORENMARKET)
				{
					MsgOutput(aIndex,Lang.GetText(0,479));
					return 0;
				}

				if(gObj[aIndex].m_bPShopOpen)
				{
					gObj[aIndex].m_bOff = true;
					GJReqSetOffTrade(&gObj[aIndex]);
					MsgOutput(aIndex,Lang.GetText(0,480));
					BYTE p[4] = {0xC1, 0x04, 0xFA, 0x0D};
					IOCP.DataSend(aIndex,p,sizeof(p));
				}
				else
				{
					MsgOutput(aIndex,Lang.GetText(0,481));
				}
				
			}
			break;
		case 440:
			{
				char *password;
				pId = this->GetTokenString();
				password = this->GetTokenString();

				if(pId == NULL)
				{
					MsgOutput(aIndex, Lang.GetText(0,538));
					return 0;

				}

				LPOBJ lpDcOBJ = gObjFind(pId); 
				if(lpDcOBJ == NULL)
				{
					MsgOutput(aIndex,Lang.GetText(0,482));
					return 0;
				}

				if(password == NULL)
				{
					MsgOutput(aIndex,Lang.GetText(0,483));
					return 0;
				}
				if(gObjPasswordCheck(lpDcOBJ->m_Index,password))
				{
					//gObjCloseSet(lpDcOBJ->m_Index,0);
					GSProtocol.GCSendDisableReconnect(lpDcOBJ->m_Index);
					//IOCP.CloseClient(lpDcOBJ->m_Index);
					MsgOutput(aIndex,Lang.GetText(0,485));
				}
				else
				{
					MsgOutput(aIndex, Lang.GetText(0,484));
				}

			}
			break;
		case 441:
			{
				if ((lpObj->Authority & 2) != 2 && (lpObj->Authority & 0x20) != 0x20)
				{
					return 0;
				}

				WORD wCheatMode = this->GetTokenNumber();

				if ( wCheatMode < 0 )
					wCheatMode = 0;

				g_ImperialGuardian.SetCheatModeGM(wCheatMode);

				switch ( wCheatMode )
				{
					case 1:
						GSProtocol.GCServerMsgStringSend("[EventDungeon] CheatMode On", aIndex, 1);
						break;
					case 2:
						GSProtocol.GCServerMsgStringSend("[EventDungeon] CheatMode On", aIndex, 1);
						break;
					case 3:
						GSProtocol.GCServerMsgStringSend("[EventDungeon] CheatMode On - LOG START", aIndex, 1);
						break;
					default:
						GSProtocol.GCServerMsgStringSend("[EventDungeon] CheatMode Off", aIndex, 1);
						break;
				}
			}
			break;
		case 442:
			{
				if ((lpObj->Authority & 2) != 2 && (lpObj->Authority & 0x20) != 0x20)
				{
					return 0;
				}

				int nDayOfWeeks = this->GetTokenNumber();

				if ( nDayOfWeeks < 0 || nDayOfWeeks > 6 )
				{
					break;
				}

				else
				{
					g_ImperialGuardian.SetDayOfWeekGM(nDayOfWeeks);
					GSProtocol.GCServerMsgStringSend("[EventDungeon] SetDayOfWeek Success", aIndex, 1);
				}
			}
			break;
		case 443:
			{
				if ((lpObj->Authority & 2) != 2 && (lpObj->Authority & 0x20) != 0x20)
				{
					return 0;
				}

				int nZoneIndex = this->GetTokenNumber();

				if ( nZoneIndex < 0 || nZoneIndex > 3 )
				{
					nZoneIndex = 0;
				}

				g_ImperialGuardian.WarpZoneGM(aIndex, nZoneIndex);
			}
			break;
		case 444:
			{
				if ((lpObj->Authority & 2) != 2 && (lpObj->Authority & 0x20) != 0x20)
				{
					return 0;
				}

				int nTreeType = this->GetTokenNumber();
				g_MasterLevelSkillTreeSystem.ResetMasterSkill(aIndex, nTreeType);
			}
			break;
		case 445:
			{
				MsgOutput(aIndex, "HP/MaxHP (%d/%d)", (int)gObj[aIndex].Life, (int)(gObj[aIndex].AddLife + gObj[aIndex].MaxLife));
			}
			break;
		case 446:
			{
				if ((lpObj->Authority & 2) != 2 && (lpObj->Authority & 0x20) != 0x20)
				{
					return 0;
				}

				if ((lpObj->GameMaster & GM_COMMAND_DC_MOVE) != GM_COMMAND_DC_MOVE)
				{
					MsgOutput(lpObj->m_Index, Lang.GetText(0,425));
					return 0;
				}

				pId = this->GetTokenString();

				if (pId == NULL)
				{
					return 0;
				}

				int map;
				int iX;
				int iY;
				LPOBJ lpTargetObj = gObjFind(pId);
				int iIndex;

				if (lpTargetObj == NULL)
				{
					return 0;
				}

				g_Log.Add( "Use GM Command -> [ %s ]\t[ %s ]\t[ %s ] / Target : [%s][%s] : %s",
					lpObj->m_PlayerData->Ip_addr, lpObj->AccountID, lpObj->Name, lpTargetObj->AccountID,
					lpTargetObj->Name, "User Trans");

				GMLog->Output("Use GM Command -> [ %s ]\t[ %s ]\t[ %s ] / Target : [%s][%s] : %s",
					lpObj->m_PlayerData->Ip_addr, lpObj->AccountID, lpObj->Name, lpTargetObj->AccountID,
					lpTargetObj->Name, "User Trans");

				map = lpObj->MapNumber;
				iX = lpObj->X;
				iY = lpObj->Y;
				iIndex = lpTargetObj->m_Index;

				int nGMMapNumber = map;
				int nUserMapNumber = lpTargetObj->MapNumber;

				if (iIndex >= 0)
				{
					gObjTeleport(iIndex, map, iX, iY);

					if (nGMMapNumber != nUserMapNumber)
					{
						g_SendNPCInfo.SendNpcCoordinate(iIndex, map, 0);
						g_SendNPCInfo.SendPortalCoordinate(iIndex, map);
					}
				}

				if (map == MAP_INDEX_HATCHERY)
				{
					g_RaklionBattleUserMng.AddUserData(lpObj->m_Index);
				}
			}
			break;
		case 447:
		{
			if (g_ConfigRead.data.common.AutoPartySet == 0){
				MsgOutput(aIndex, Lang.GetText(0,551), m_CommandNames.setparty);
			return 0; }
			pId = this->GetTokenString();
			
			if (pId == NULL)
			{
				MsgOutput(aIndex, Lang.GetText(0,552), m_CommandNames.setparty);
				return 0;
			}
			if (strlen(pId) < 4)
			{
				MsgOutput(aIndex, Lang.GetText(0,553));
				return 0;
			}
			if (strlen(pId) > 10)
			{
				MsgOutput(aIndex, Lang.GetText(0,554));
				return 0;
			}
			if (g_GensSystem.IsMapBattleZone(lpObj->MapNumber) == TRUE)
			{
				MsgOutput(aIndex, Lang.GetText(0,533));
				return 0;
			}
			memcpy(lpObj->m_PlayerData->PartyPassword, pId,10);

			MsgOutput(aIndex, Lang.GetText(0,555), lpObj->m_PlayerData->PartyPassword);

			lpObj->m_PlayerData->bActiveSetParty = true;
		}
		break;
		case 448:
		{
			BYTE TEST[] = { 0xC1, 0x06, 0xF1, 0xFE, 0x01, 0x01 };
			IOCP.DataSend(aIndex, TEST, 6);
		}
		break;
		case 449:
			{
				if(g_OffLevel.b_Enabled == true)
				{
					if(lpObj->m_PlayerData->VipType < g_OffLevel.m_General.VipType)
					{
						MsgOutput(aIndex, Lang.GetText(0,602), g_OffLevel.m_General.VipType);
						return 0;
					}
					if(g_OffLevel.m_PerMapAttr[lpObj->MapNumber].Disable != 1)
					{
						for(int i=0;i<5;i++)
						{
							CMagicInf* lpMagic = gObjGetMagicSearch(lpObj,g_OffLevel.m_SkillDef[lpObj->Class].SkillNumber[i]);

							if(lpMagic != 0)
							{
								if(!g_OffLevel.ChargePlayer(aIndex))
								{
									MsgOutput(aIndex,Lang.GetText(0,289));
									return 0;
								}

								g_OffLevel.AddUser(aIndex,lpMagic->m_Skill);
								MsgOutput(aIndex,Lang.GetText(0,603), MagicDamageC.m_Name[g_OffLevel.m_SkillDef[lpObj->Class].SkillNumber[i]], lpMagic->m_Skill);
								lpObj->m_bOffLevel = true;
								lpObj->m_bOff = true;
								lpObj->m_bPShopOpen = true;
								lpObj->m_dwOffLevelTime = GetTickCount64();
								sprintf(lpObj->m_szPShopText,Lang.GetText(0,610));
								GJReqSetOffTrade(&gObj[aIndex]);
								MsgOutput(aIndex,Lang.GetText(0,480));
								
								BYTE p[4] = {0xC1, 0x04, 0xFA, 0x0D};
								IOCP.DataSend(aIndex,p,sizeof(p));
								return 1;
							}
							else
							{
								if( g_OffLevel.m_SkillDef[lpObj->Class].SkillNumber[i] > 0)
								{
									MsgOutput(aIndex, Lang.GetText(0,604), MagicDamageC.m_Name[g_OffLevel.m_SkillDef[lpObj->Class].SkillNumber[i]], g_OffLevel.m_SkillDef[lpObj->Class].SkillNumber[i]);
								}
							}
						}
					}
				}				
			}
			break;
		case 450: // GM Gift by Gremory Case
			{
				if ( lpObj->Authority != 0x20 )
				{
					MsgOutput(aIndex, Lang.GetText(0,417));
					return FALSE;
				}

				if ( (lpObj->GameMaster & GM_COMMAND_ITEM_CREATE) != GM_COMMAND_ITEM_CREATE )
				{
					MsgOutput(lpObj->m_Index, Lang.GetText(0,418));
					return 0;
				}

				pId = this->GetTokenString();

				if ( pId == NULL )
				{
					return 0;
				}

				LPOBJ lpTargetObj = gObjFind(pId);

				if ( lpTargetObj == NULL )
				{
					return 0;
				}

				int StorageType, ItemType, ItemIndex, ItemLevel, ItemDurability, ItemSkill, ItemLuck, ItemOption, ItemExcOption, ItemSetOption, ItemSocketCount, ItemMainAttribute, MuunEvoItemType, MuunEvoItemIndex, DaysToExpire;

				StorageType = this->GetTokenNumber();
				ItemType = this->GetTokenNumber();
				ItemIndex = this->GetTokenNumber();
				ItemLevel = this->GetTokenNumber();
				ItemDurability = this->GetTokenNumber();
				ItemSkill = this->GetTokenNumber();
				ItemLuck = this->GetTokenNumber();
				ItemOption = this->GetTokenNumber();
				ItemExcOption = this->GetTokenNumber();
				ItemSetOption = this->GetTokenNumber();
				ItemSocketCount = this->GetTokenNumber();
				ItemMainAttribute = this->GetTokenNumber();
				MuunEvoItemType = this->GetTokenNumber();
				MuunEvoItemIndex = this->GetTokenNumber();
				DaysToExpire = this->GetTokenNumber();

				if (StorageType != GC_STORAGE_SERVER && StorageType != GC_STORAGE_CHARACTER)
				{
					MsgOutput(aIndex, Lang.GetText(0,641));
					return 0;
				}

				if (ItemGetNumberMake(ItemType, ItemIndex) == -1)
				{
					return 0;
				}

				if (DaysToExpire < 1)
				{
					DaysToExpire = 1;
				}

				_stGremoryCaseItem stItem;
				stItem.btStorageType = StorageType;
				stItem.btRewardSource = GC_REWARD_GM_REWARD;
				stItem.wItemID = ItemGetNumberMake(ItemType, ItemIndex);
				stItem.btItemLevel = ItemLevel;
				stItem.btItemDurability = ItemDurability;
				stItem.btItemSkill = ItemSkill;
				stItem.btItemLuck = ItemLuck;
				stItem.btItemOption = ItemOption;
				stItem.btItemExcOption = ItemExcOption;
				stItem.btItemSetOption = ItemSetOption;
				stItem.btItemSocketCount = ItemSocketCount;
				stItem.btItemMainAttribute = ItemMainAttribute;
				stItem.wMuunEvoItemType = MuunEvoItemType;
				stItem.btMuunEvoItemIndex = MuunEvoItemIndex;

				g_GremoryCase.GDReqAddItemToGremoryCase(lpTargetObj->m_Index, stItem, DaysToExpire);

				g_Log.Add( "[%s][%s][%s] Added GM Gift to Gremory Case using Admin Command (%s/%d/%d) Auth: %d Target: %s", lpObj->AccountID, lpObj->Name, lpObj->m_PlayerData->Ip_addr, Lang.GetMap(0, lpObj->MapNumber), lpObj->X, lpObj->Y, lpObj->Authority, lpTargetObj->Name);
				GMLog->Output("[%s][%s][%s] Created Added GM Gift to Gremory Case using Admin Command (%s/%d/%d) Auth: %d Target: %s", lpObj->AccountID, lpObj->Name, lpObj->m_PlayerData->Ip_addr, Lang.GetMap(0, lpObj->MapNumber), lpObj->X, lpObj->Y, lpObj->Authority, lpTargetObj->Name);
			}
			break;
		case 451:
			{
				if ((lpObj->Authority & 2) != 2 && (lpObj->Authority & 0x20) != 0x20)
				{
					return 0;
				}

				if(g_OffLevel.b_Enabled == true)
				{
					if(lpObj->m_PlayerData->VipType < g_OffLevel.m_General.VipType)
					{
						MsgOutput(aIndex, Lang.GetText(0,602), g_OffLevel.m_General.VipType);
						return 0;
					}

					if(g_OffLevel.m_PerMapAttr[lpObj->MapNumber].Disable != 1)
					{
						for(int i=0;i<5;i++)
						{
							CMagicInf* lpMagic = gObjGetMagicSearch(lpObj,g_OffLevel.m_SkillDef[lpObj->Class].SkillNumber[i]);

							// Check SafeZone
							BYTE btAttr = MapC[lpObj->MapNumber].GetAttr(lpObj->X, lpObj->Y);

							if ((btAttr & 1) == 1)
							{
								MsgOutput(aIndex,Lang.GetText(0,635));
								return FALSE;
							}

							// Check Magic
							if(lpMagic != 0)
							{
								g_OffLevel.AddUser(aIndex,lpMagic->m_Skill);
								MsgOutput(aIndex,Lang.GetText(0,603), MagicDamageC.m_Name[g_OffLevel.m_SkillDef[lpObj->Class].SkillNumber[i]], lpMagic->m_Skill);
								lpObj->m_bOffLevel = true;
								lpObj->m_bOff = true;
								lpObj->m_bPShopOpen = true;
								sprintf(lpObj->m_szPShopText,Lang.GetText(0,610));
								GJReqSetOffTrade(&gObj[aIndex]);
								MsgOutput(aIndex,Lang.GetText(0,480));
								return 1;
							}
							else
							{
								if( g_OffLevel.m_SkillDef[lpObj->Class].SkillNumber[i] > 0)
								{
									MsgOutput(aIndex,Lang.GetText(0,604), MagicDamageC.m_Name[g_OffLevel.m_SkillDef[lpObj->Class].SkillNumber[i]], g_OffLevel.m_SkillDef[lpObj->Class].SkillNumber[i]);
								}
							}
						}
					}
				}				
			}
			break;
		case 452:
			{
				if ((lpObj->Authority & 2) != 2 && (lpObj->Authority & 0x20) != 0x20)
				{
					return 0;
				}

				int iBlow = this->GetTokenNumber();

				if (iBlow != 0 && iBlow != 1)
				{
					MsgOutput(aIndex, "[CCS-Debug] Blow Switch values: 0/1");
					return 0;
				}

				g_ChaosCastleFinal.m_bCCF_CHEAT_BLOW = iBlow;
				g_Log.Add("[ChaosCastleFinal][CHEAT][CCF BLOW] [%s]Changed m_bCCF_CHEAT_BLOW:%d ", lpObj->Name, g_ChaosCastleFinal.m_bCCF_CHEAT_BLOW);
			}
			break;
		case 453:
			{
				if ((lpObj->Authority & 2) != 2 && (lpObj->Authority & 0x20) != 0x20)
				{
					return 0;
				}

				int iTrap = this->GetTokenNumber();

				if (iTrap != 0 && iTrap != 1)
				{
					MsgOutput(aIndex, "[CCS-Debug] Trap Switch values: 0/1");
					return 0;
				}

				g_ChaosCastleFinal.m_bCCF_CHEAT_TRAP = iTrap;
				g_Log.Add("[ChaosCastleFinal][CHEAT][CCF BLOW] [%s]Changed m_bCCF_CHEAT_TRAP:%d ", lpObj->Name, g_ChaosCastleFinal.m_bCCF_CHEAT_TRAP);
			}
			break;
		case 454:
			{
				if ((lpObj->Authority & 2) != 2 && (lpObj->Authority & 0x20) != 0x20)
				{
					return 0;
				}

				int iDay = this->GetTokenNumber();

				g_QuestExpProgMng.SetCheatChangeDay(iDay);
				MsgOutput(aIndex, "QuestExp Cheat Day:%d", g_QuestExpProgMng.GetCheatChangeDay());
			}
			break;

		case 455:
			{
				int iResult = g_ChangeCmd.DoChange(aIndex);

				if (iResult == -1)
				{
					return 0;
				}

				if (iResult == 0)
				{
					MsgOutput(aIndex, Lang.GetText(0,639));
				}

				else
				{
					MsgOutput(aIndex, Lang.GetText(0,640), iResult);
				}
			}
			break;

		case 456:
		{
			int iResult = g_UpgradeCmd.DoUpgrade(aIndex);

			if (iResult == -1)
			{
				return 0;
			}

			if (iResult == 0)
			{
				MsgOutput(aIndex, "No upgrade has been made!");
			}

			else
			{
				MsgOutput(aIndex, "Item Successfully upgraded!");
			}
		}
		break;

	}
	return 0;
}





void CGMMng::GetInfinityArrowMPConsumption(LPOBJ lpObj)
{
	MsgOutput(lpObj->m_Index, Lang.GetText(0,486),
		g_SkillAdditionInfo.GetInfinityArrowMPConsumptionPlus0(),
		g_SkillAdditionInfo.GetInfinityArrowMPConsumptionPlus1(),
		g_SkillAdditionInfo.GetInfinityArrowMPConsumptionPlus2());
}

					

void CGMMng::ControlInfinityArrowMPConsumption0(LPOBJ lpObj, int iValue)
{
	g_SkillAdditionInfo.SetInfinityArrowMPConsumptionPlus0(iValue);
	MsgOutput(lpObj->m_Index, Lang.GetText(0,487),iValue);

}	

void CGMMng::ControlInfinityArrowMPConsumption1(LPOBJ lpObj, int iValue)
{
	g_SkillAdditionInfo.SetInfinityArrowMPConsumptionPlus1(iValue);
	MsgOutput(lpObj->m_Index, Lang.GetText(0,488),iValue);

}

void CGMMng::ControlInfinityArrowMPConsumption2(LPOBJ lpObj, int iValue)
{
	g_SkillAdditionInfo.SetInfinityArrowMPConsumptionPlus2(iValue);
	MsgOutput(lpObj->m_Index, Lang.GetText(0,489),iValue);

}

void CGMMng::SetInfinityArrowTime(LPOBJ lpObj, int iValue)
{
	if ( lpObj->Class == CLASS_ELF && lpObj->Type == OBJ_USER && lpObj->m_PlayerData->ChangeUP >= 1 )
	{
		gObjAddBuffEffect(lpObj, BUFFTYPE_INFINITY_ARROW, 0, 0, 0, 0, iValue);
		MsgOutput(lpObj->m_Index, Lang.GetText(0,490), iValue);
	}
	else
	{
		MsgOutput(lpObj->m_Index, Lang.GetText(0,491));
	}
}


void CGMMng::ControlFireScreamDoubleAttackDistance(LPOBJ lpObj, int iValue)
{
	g_SkillAdditionInfo.SetFireScreamExplosionAttackDistance(iValue);
	MsgOutput(lpObj->m_Index, Lang.GetText(0,492), iValue);
}

void ServerMsgSend(LPOBJ lpObj,int Type,char Sender[20],const char*Message,...)
{
	PMSG_CHATDATA_WHISPER pMsg;

	/*char szBuffer[1024];
	va_list pArguments;
	va_start(pArguments,Message);
	vsprintf(szBuffer,Message,pArguments);
	va_end(pArguments);
	BYTE *Packet = new BYTE[200];
	memset(Packet,0x00,200);
	*Packet = 0xC1;
	if(Type)*(Packet+2)=0x02;
	else *(Packet+2)=0x00;
	memcpy((Packet+3),Sender,strlen(Sender));
	memcpy((Packet+13),szBuffer,strlen(szBuffer));
	int Len = (strlen(szBuffer)+0x13);
	*(Packet+1)=Len;
	if(!lpObj)GSProtocol.DataSendAll(Packet,Len);
	else
		if(lpObj->Connected)
			IOCP.DataSend(lpObj->m_Index,Packet,Len);
	delete [] Packet;*/
}

BOOL CGMMng::CheckTraceMarryCondition(LPOBJ lpObj, LPOBJ lpTargetObj)
{
	if(lpObj->Married == 0)
	{
		return FALSE;
	}

	if(strlen(lpObj->MarryName) <= 0)
	{
		return FALSE;
	}

	if(lpObj->Level < 200)
	{
		return FALSE;
	}

	if(lpTargetObj->Level < 200)
	{
		return FALSE;
	}

	if(BC_MAP_RANGE(lpTargetObj->MapNumber) || DS_MAP_RANGE(lpTargetObj->MapNumber) || CC_MAP_RANGE(lpTargetObj->MapNumber) || IT_MAP_RANGE(lpTargetObj->MapNumber) ||
		DG_MAP_RANGE(lpTargetObj->MapNumber) || IMPERIAL_MAP_RANGE(lpTargetObj->MapNumber) || CRYWOLF_MAP_RANGE(lpTargetObj->MapNumber) || lpTargetObj->MapNumber == MAP_INDEX_CASTLESIEGE ||
		lpTargetObj->MapNumber == MAP_INDEX_CASTLEHUNTZONE || g_GensSystem.IsMapBattleZone(lpTargetObj->MapNumber) || lpTargetObj->MapNumber == MAP_INDEX_KANTURU_BOSS || lpTargetObj->MapNumber == MAP_INDEX_KALIMA1
		|| lpTargetObj->MapNumber == MAP_INDEX_KALIMA2 || lpTargetObj->MapNumber == MAP_INDEX_KALIMA3 || lpTargetObj->MapNumber == MAP_INDEX_KALIMA4 || lpTargetObj->MapNumber == MAP_INDEX_KALIMA5 || lpTargetObj->MapNumber == MAP_INDEX_KALIMA6
		|| lpTargetObj->MapNumber == MAP_INDEX_KALIMA7 || lpTargetObj->MapNumber == MAP_INDEX_ARCA_WAR || lpTargetObj->MapNumber == MAP_INDEX_DEBENTER_ARCA_WAR )
	{
		return FALSE;
	}

	LPMOVE_COMMAND_DATA lpMoveData = gMoveCommand.GetMoveCommandDataByMapNumber(lpTargetObj->MapNumber);

	if ( lpMoveData == NULL )
	{
		return FALSE;
	}

	if (lpMoveData->NeedLevel > lpObj->Level)
	{
		MsgOutput(lpObj->m_Index,Lang.GetText(0,612));
		return FALSE;
	}

	if(lpTargetObj->MapNumber == MAP_INDEX_ICARUS)
	{
		if ( (lpObj->pInventory[lpObj->m_btInvenPetPos].m_Type != ITEMGET(13,3) &&
			 lpObj->pInventory[7].m_Type != ITEMGET(13,30) &&
			 lpObj->pInventory[lpObj->m_btInvenPetPos].m_Type != ITEMGET(13,37) &&
			 lpObj->pInventory[7].m_Type != ITEMGET(12,36) &&//Season 4.5 addon
			 lpObj->pInventory[7].m_Type != ITEMGET(12,37) &&//Season 4.5 addon
			 lpObj->pInventory[7].m_Type != ITEMGET(12,38) &&//Season 4.5 addon
			 lpObj->pInventory[7].m_Type != ITEMGET(12,39) &&//Season 4.5 addon
			 lpObj->pInventory[7].m_Type != ITEMGET(12,40) &&//Season 4.5 addon
			 lpObj->pInventory[7].m_Type != ITEMGET(12,41) &&//Season 4.5 addon
			 lpObj->pInventory[7].m_Type != ITEMGET(12,42) &&//Season 4.5 addon
			 lpObj->pInventory[7].m_Type != ITEMGET(12,43) &&//Season 4.5 addon
			 lpObj->pInventory[lpObj->m_btInvenPetPos].m_Type != ITEMGET(13,4) &&//Season 4.5 addon
			 !(lpObj->pInventory[7].m_Type >= ITEMGET(12,262) && lpObj->pInventory[7].m_Type <= ITEMGET(12,265)) &&
			 !(lpObj->pInventory[7].m_Type >= ITEMGET(12,130) && lpObj->pInventory[7].m_Type <= ITEMGET(12,135)) &&
			 (lpObj->pInventory[7].m_Type < ITEMGET(12,0) || lpObj->pInventory[7].m_Type > ITEMGET(12,6)) ) ||
			 lpObj->pInventory[lpObj->m_btInvenPetPos].m_Type == ITEMGET(13,2) ||
			 lpObj->pInventory[11].m_Type == ITEMGET(13,10) ||
			 lpObj->pInventory[10].m_Type == ITEMGET(13,10)  )
		{
			return FALSE;
		}
	}

	BOOL bPlayerKiller = FALSE; //season 2.5 add-on

	if( lpObj->PartyNumber >= 0) //season 2.5 add-on
	{
		if( (gParty.GetPKPartyPenalty(lpObj->PartyNumber)) > 4)
		{
			bPlayerKiller = TRUE;
		}
	}
	
	else
	{
		if( lpObj->m_PK_Level > 4 )
		{
			bPlayerKiller = TRUE;
		}
	}

	if (bPlayerKiller == TRUE)
	{
		return FALSE;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

