//////////////////////////////////////////////////////////////////////
// NpcTalk.cpp
#include "stdafx.h"
#include "user.h"
#include "NpcTalk.h"
#include "gObjMonster.h"
#include "winutil.h"
#include "protocol.h"
#include "GameMain.h"
#include "BloodCastle.h"
#include "DevilSquare.h"
#include "QuestInfo.h"
#include "CastleSiegeSync.h"
#include "KanturuEntranceNPC.h"
#include "JewelOfHarmonySystem.h"
#include "Event.h"
#include "BuffEffect.h"
#include "BuffEffectSlot.h"
#include "DoppelGanger.h"
#include "DSProtocol.h"
#include "LastManStanding.h"
#include "configread.h"
#include "GensSystem.h"
#include "NewPVP.h"
#include "user.h"
#include "QuestExpProgMng.h"
#include "PentagramMixSystem.h"

// GS-N 0.99.60T 0x005211D0
//	GS-N	1.00.18	JPN	0x005527F0	-	Completed

BOOL NpcTalk(LPOBJ lpNpc, LPOBJ lpObj)
{
	int npcnum = lpNpc->Class;

	if ( npcnum < 0 )
	{
		return FALSE;
	}

	if ( (lpObj->m_IfState.use) > 0 )
	{
		return TRUE;
	}

	if ( npcnum == 229 ) // Marlon
	{
		gQeustNpcTeleport.TalkRefAdd();
		lpObj->TargetNpcNumber = lpNpc->m_Index;
	}

	if ( NpcQuestCheck(lpNpc, lpObj) != FALSE )
	{
		return TRUE;
	}

	g_QuestExpProgMng.ChkUserQuestType(QUESTEXP_ASK_NPC_TALK, lpObj, lpNpc, 0);

	if (npcnum == 568) //-> Wandering Merchant Zyro
	{
		gQuestExpNpcTeleport.TalkRefAdd();
		lpObj->TargetNpcNumber = lpNpc->m_Index;

		if (Npc_Dialog(lpNpc, lpObj))
		{
			return true;
		}
	}

	switch ( npcnum )
	{
		case 367:
			if ( NpcMainatenceMachine ( lpNpc, lpObj ) == TRUE )
			{
				return TRUE;
			}
			break;

		case 259:
			if ( NpcReira ( lpNpc, lpObj ) == TRUE )
			{
				return TRUE;
			}
			break;

		case 254:
			if ( NpcFasi ( lpNpc, lpObj ) == TRUE )
			{
				return TRUE;
			}
			break;

		case 241:
			if ( NpcGuildMasterTalk( lpNpc, lpObj ) == TRUE )
			{
				return TRUE;
			}
			break;
		
		case 239:
			if ( NpcBattleAnnouncer( lpNpc, lpObj ) == TRUE )
			{
				return TRUE;
			}
			break;

		case 249:
			if ( NpcRolensiaGuard( lpNpc, lpObj ) == TRUE )
			{
				return TRUE;
			}
			break;

		case 238:
			if ( NpcChaosGoblelin( lpNpc, lpObj ) == TRUE )
			{
				return TRUE;
			}
			break;

		case 237:
			if ( NpcRusipher( lpNpc, lpObj ) == TRUE )
			{
				return TRUE;
			}
			break;

		case 253:
			if ( NpcPosionGirl( lpNpc, lpObj ) == TRUE )
			{
				return TRUE;
			}
			break;

		case 245:
			if ( NpcDeviasWizard( lpNpc, lpObj ) == TRUE )
			{
				return TRUE;
			}
			break;

		case 247:
			if ( NpcDeviasGuard( lpNpc, lpObj ) == TRUE )
			{
				return TRUE;
			}
			break;

		case 220:
			if( NpcCastleHuntZoneGuard(lpNpc,lpObj) == TRUE)
			{
				return TRUE;
			}
			break;

		case 240:
			if ( NpcWarehouse( lpNpc, lpObj ) == TRUE )
			{
				return TRUE;
			}
			break;

		case 226:
			if ( NpcDarkSpiritTrainer( lpNpc, lpObj ) == TRUE )
			{
				return TRUE;
			}
			break;

		case 242:
			if ( NpcNoriaRara( lpNpc, lpObj ) == TRUE )
			{
				return TRUE;
			}
			break;

		case 244:
			if ( NpcDeviasMadam( lpNpc, lpObj ) == TRUE )
			{
				return TRUE;
			}
			break;

		case 236:
			if ( NpcEventChipNPC( lpNpc, lpObj ) == TRUE )
			{
				return TRUE;
			}
			break;

		case 234:
			if ( NpcServerDivision( lpNpc, lpObj ) == TRUE )
			{
				return TRUE;
			}
			break;

		case 232:
			if ( NpcAngelKing( lpNpc, lpObj ) == TRUE )
			{
				return TRUE;
			}
			break;

		case 233:
			if ( NpcAngelMessanger( lpNpc, lpObj ) == TRUE )
			{
				return TRUE;
			}
			break;

		case 255:
			if ( NpcRolensiaMadam( lpNpc, lpObj ) == TRUE )
			{
				return TRUE;
			}
			break;

		case 250:
			if ( NpcRoadMerchant( lpNpc, lpObj ) == TRUE )
			{
				return TRUE;
			}
			break;

		case 246:
			if ( NpcDeviasWeapon( lpNpc, lpObj ) == TRUE )
			{
				return TRUE;
			}
			break;

		case 251:
			if ( NpcRorensiaSmith( lpNpc, lpObj ) == TRUE )
			{
				return TRUE;
			}
			break;

		case 243:
			if ( NpcNoriJangIn( lpNpc, lpObj ) == TRUE )
			{
				return TRUE;
			}
			break;

		case 256:
			if ( NpcJewelMixDealer( lpNpc, lpObj ) == TRUE )
			{
				return TRUE;
			}
			break;

		case 221:
			if ( NpcSiegeMachine_Attack( lpNpc, lpObj ) == TRUE )
			{
				return TRUE;
			}
			break;

		case 222:
			if ( NpcSiegeMachine_Defense( lpNpc, lpObj ) == TRUE )
			{
				return TRUE;
			}
			break;

		case 223:
			if ( NpcElderCircle( lpNpc, lpObj ) == TRUE )
			{
				return TRUE;
			}
			break;

		case 224:
			if ( NpcCastleGuard( lpNpc, lpObj ) == TRUE )
			{
				return TRUE;
			}
			break;

		case 219:
			if ( NpcCastleGateLever( lpNpc, lpObj ) == TRUE )
			{
				return TRUE;
			}
			break;

		case 216:
			if ( NpcCastleCrown( lpNpc, lpObj ) == TRUE )
			{
				return TRUE;
			}
			break;

		case 217:
		case 218:
			if ( NpcCastleSwitch( lpNpc, lpObj ) == TRUE )
			{
				return TRUE;
			}
			break;

		case 257:
			return Npc_Dialog(lpNpc, lpObj);

		case 368:
		case 369:
		case 370:
			if ( g_kJewelOfHarmonySystem.NpcJewelOfHarmony( lpNpc, lpObj ) == TRUE )
			{
				return TRUE;
			}
			break;

		case 371:
			{
				if ( NpcLeoHelper( lpNpc, lpObj) == TRUE)
				{
					return TRUE;
				}
			} 
			break;

		case 375:
			if ( NpcChaosCardMaster( lpNpc, lpObj ) == TRUE )
			{
				return TRUE;
			}
			break;
		case 407:
			if(NpcWerewolfQuarrel(lpNpc, lpObj) == TRUE )
			{
				return TRUE;
			}
			break;
		case 408:
			if(NpcGateKeeper(lpNpc, lpObj) == TRUE )
			{
				return TRUE;
			}
			break;
		case 450:
		case 478:
			{
				return FALSE;
			}
			break;
		case 452:
			if(NpcSeedMaster(lpNpc, lpObj) == TRUE )
			{
				return TRUE;
			}
			break;
		case 453:
			if(NpcSeedResearcher(lpNpc, lpObj) == TRUE )
			{
				return TRUE;
			}
			break;
		case 465:
			if(NpcSantaClause(lpNpc, lpObj) == TRUE )
			{
				return TRUE;
			}
			break;
		case 467:
			{
				GSProtocol.GCServerCmd(lpObj->m_Index, 0x11, 0x01, 0x00);
			}
			break;
		case 468:
			{
				gObjAddBuffEffect(lpObj, BUFFTYPE_SANTA_POWER, EFFECTTYPE_POWER_UP, 30, 0, 0, 1800);
			}
			break;
		case 469:
			{
				gObjAddBuffEffect(lpObj, BUFFTYPE_SANTA_GUARD, EFFECTTYPE_GUARD_UP, 100, 0, 0, 1800);
			}
			break;
		case 470:
			{
				lpObj->Life = lpObj->MaxLife + lpObj->AddLife;
				GSProtocol.GCReFillSend(lpObj->m_Index, lpObj->Life, 0xFF, 0, lpObj->iShield);
			}
			break;
		case 471:
			{
				lpObj->Mana = lpObj->MaxMana + lpObj->AddMana;
				GSProtocol.GCManaSend(lpObj->m_Index, lpObj->Mana, 0xFF, 0, lpObj->BP);
			}
			break;
		case 472:
			{
				gObjAddBuffEffect(lpObj, BUFFTYPE_SANTA_HEAL, EFFECTTYPE_HP, 500, 0, 0, 1800);
			}
			break;
		case 473:
			{
				gObjAddBuffEffect(lpObj, BUFFTYPE_SANTA_PROTECT, EFFECTTYPE_MANA, 500, 0, 0, 1800);
			}
			break;
		case 474:
			{
				gObjAddBuffEffect(lpObj, BUFFTYPE_SANTA_SPEED, EFFECTTYPE_ATTACKSPEED, 15, 0, 0, 1800);
			}
			break;
		case 475:
			{
				gObjAddBuffEffect(lpObj, BUFFTYPE_SANTA_LUCKY, EFFECTTYPE_AG_RECOVERY_UP, 10, 0, 0, 1800);
			}
			break;
		case 479:
			{
				return NpcTitus(lpNpc, lpObj);
			}
			break;
		case 522:
			{
				return NpcJerint(lpNpc, lpObj);
			}
			break;
		case 540:
			{
				return NpcLugard(lpNpc, lpObj);
			}
			break;
		case 541:
			{
				return NpcSilverBox(lpNpc, lpObj);
			}
			break;
		case 542:
			{
				return NpcGoldBox(lpNpc, lpObj);
			}
			break;
		case 543:
		case 544:
			return Npc_Dialog(lpNpc, lpObj);
		case 546:
			{
				return NpcJewelMixDealer(lpNpc, lpObj);
			}
			break;
		case 547:
			{
				return NpcJulia(lpNpc, lpObj);
			}
			break;
		case 566:
		case 567:
			return Npc_Dialog(lpNpc, lpObj);
		case 581:
			return Npc_Dialog(lpNpc, lpObj);
		case 579:
			return NpcDavid(lpNpc, lpObj);
			break;
		case 580:
			return NpcAcheronEntrance(lpNpc, lpObj);
		case 582:
			return NpcElementalMaster(lpNpc, lpObj);
		case 584:
			return NpcArcaWar(lpNpc, lpObj);
		case 651:
			return NpcMarketWall(lpNpc, lpObj);
		case 682:
			return NpcMonicaTalk(lpNpc,lpObj);
	case 258:
			{
				if(g_LastManStanding.m_Cfg.bEnable)
				{
					if(lpNpc->MapNumber == g_LastManStanding.m_Cfg.iRegNPCMap && lpNpc->X == g_LastManStanding.m_Cfg.iRegNPCX && lpNpc->Y ==  g_LastManStanding.m_Cfg.iRegNPCY)
					{
						if(g_LastManStanding.m_Rooms[0].bState != 1) // rooms[0] because the state at this point is same for all rooms
						{
							MsgOutput(lpObj->m_Index,Lang.GetText(0,395));
							return false;
						}
						if(lpObj->m_PlayerData->RegisterdLMS == 1)
						{
							MsgOutput(lpObj->m_Index,Lang.GetText(0,396));
							return false;
						}
						if(lpObj->PartyNumber >= 0)
						{
							MsgOutput(lpObj->m_Index,Lang.GetText(0,397));
							return false;
						}
						if(g_LastManStanding.m_Cfg.iReqItem)
						{
							int invitationpos = -1;
							int removedCount = 0;
							if (gObjGetItemCountInIventory(lpObj->m_Index, ITEMGET(g_LastManStanding.m_Cfg.iReqItemCat, g_LastManStanding.m_Cfg.iReqItemIdx)) < g_LastManStanding.m_Cfg.iReqItemCount)
							{
								MsgOutput(lpObj->m_Index, Lang.GetText(0,398), g_LastManStanding.m_Cfg.iReqItemCount, ItemAttribute[ITEMGET(g_LastManStanding.m_Cfg.iReqItemCat, g_LastManStanding.m_Cfg.iReqItemIdx)].Name);
								return false;

							}
							for (short i = 0; i < MAIN_INVENTORY_SIZE; ++i)
							{
								if(lpObj->pInventory[i].m_Type > 0 && lpObj->pInventory[i].m_Type == ITEMGET(g_LastManStanding.m_Cfg.iReqItemCat,g_LastManStanding.m_Cfg.iReqItemIdx))
								{
									if (removedCount < g_LastManStanding.m_Cfg.iReqItemCount)
									{
										gObjInventoryDeleteItem(lpObj->m_Index, i);
										GSProtocol.GCInventoryItemDeleteSend(lpObj->m_Index, i, 1);
										removedCount++;
									}
									else
									{
										break;
									}	
								}
							}
						/*	if(invitationpos == -1)
							{
								MsgOutput(lpObj->m_Index,Lang.GetText(0,398),g_LastManStanding.m_Cfg.iReqItemCount,ItemAttribute[ITEMGET(g_LastManStanding.m_Cfg.iReqItemCat,g_LastManStanding.m_Cfg.iReqItemIdx)].Name);
								return false;
							}*/
							if(lpObj->m_PlayerData->Money < g_LastManStanding.m_Cfg.iReqZen)
							{
								MsgOutput(lpObj->m_Index,Lang.GetText(0,399),g_LastManStanding.m_Cfg.iReqZen);
								return false;
							}
							lpObj->m_PlayerData->Money -= g_LastManStanding.m_Cfg.iReqZen;
							GSProtocol.GCMoneySend(lpObj->m_Index, lpObj->m_PlayerData->Money);

							if(g_LastManStanding.AddUser(lpObj->m_Index)){
							MsgOutput(lpObj->m_Index,Lang.GetText(0,400));
							}
						}
						else
						{
							if(lpObj->m_PlayerData->Money < g_LastManStanding.m_Cfg.iReqZen)
							{
								MsgOutput(lpObj->m_Index,Lang.GetText(0,401),g_LastManStanding.m_Cfg.iReqZen);
								return false;
							}
							if(g_LastManStanding.AddUser(lpObj->m_Index)){
								MsgOutput(lpObj->m_Index,Lang.GetText(0,402));
							}
						}
					}
				}
			}
			break;
	}

	return FALSE;
}

BOOL NpcMainatenceMachine(LPOBJ lpNpc, LPOBJ lpObj)
{
	if (g_ConfigRead.server.GetServerType() == SERVER_CASTLE)
	{
		return TRUE;
	}

	g_KanturuEntranceNPC.NotifyEntranceInfo(lpObj->m_Index);
	return TRUE;
}

BOOL NpcReira(LPOBJ lpNpc, LPOBJ lpObj)
{
	return FALSE;
}

BOOL NpcBattleAnnouncer(LPOBJ lpNpc, LPOBJ lpObj)
{
	return 0;
}

BOOL NpcFasi(LPOBJ lpNpc, LPOBJ lpObj)
{
	if ( g_DevilSquare.IsEventEnable() != FALSE )
	{
		GSProtocol.GCServerCmd(lpObj->m_Index, 1, 6, 0);
	}

	BOOL bPlayerKiller = FALSE; //Season 2.5 add-on
	
	if(lpObj->PartyNumber >= 0) //Season 2.5 add-on
	{
		if(gParty.GetPKPartyPenalty(lpObj->PartyNumber) > 4)
		{
			bPlayerKiller = TRUE;
		}
	}
	else if(lpObj->m_PK_Level > 4)
	{
		bPlayerKiller = TRUE;
	}

	if ( bPlayerKiller == TRUE )
	{
		int hour = 0;
		int min = 0;

		if ( lpNpc->TargetNumber != lpObj->m_Index )
		{
			if ( gObjGetPkTime(lpObj, hour, min) == TRUE )
			{
				DWORD dwNeedPoint = 0;

				switch ( lpObj->m_PK_Count )
				{
					case 1:
						dwNeedPoint = g_ConfigRead.pk.GetPKReqPoint(4);
						break;
					case 2:
						dwNeedPoint = g_ConfigRead.pk.GetPKReqPoint(5);
						break;
					case 3:
						dwNeedPoint = g_ConfigRead.pk.GetPKReqPoint(6);
						break;
					default:
						if (lpObj->m_PK_Level > 3)
						{
							dwNeedPoint = g_ConfigRead.pk.iPKPointIncForMurderer * (lpObj->m_PK_Count - 3) + g_ConfigRead.pk.GetPKReqPoint(6);
						}
						break;
				}

				if ( dwNeedPoint != 0 )
				{
					dwNeedPoint -= lpObj->m_PK_Time;
				}
	
				char szTemp[100];

				wsprintf(szTemp, Lang.GetText(0,16), dwNeedPoint);

				GSProtocol.ChatTargetSend(lpNpc, szTemp, lpObj->m_Index);

				lpNpc->TargetNumber = lpObj->m_Index;

				return TRUE;
			}
		}
		else if ( g_ConfigRead.pk.bPkPenaltyDisable == FALSE && lpObj->m_PK_Level > 4 )
		{
			int numbertext = rand() % 3;

			switch ( numbertext )
			{
				case 0:
					GSProtocol.ChatTargetSend(lpNpc, Lang.GetText(0,34), lpObj->m_Index);
					break;

				case 1:
					GSProtocol.ChatTargetSend(lpNpc, Lang.GetText(0,35), lpObj->m_Index);
					break;

				case 2:
					GSProtocol.ChatTargetSend(lpNpc, Lang.GetText(0,36), lpObj->m_Index);
					break;
			}

			lpNpc->TargetNumber = -1;	// Apply Deathway Fix, change 0 to -1;
			return TRUE;
		}
	}

	return FALSE;
}



BOOL NpcGuildMasterTalk(LPOBJ lpNpc, LPOBJ lpObj)
{
	if ( g_ConfigRead.data.common.GuildCreate == FALSE )
	{
		GSProtocol.ChatTargetSend(lpNpc, Lang.GetText(0,403), lpObj->m_Index);
		return TRUE;
	}

	if ( g_GensSystem.GetGensInfluence(lpObj) == FALSE )
	{
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,404), lpObj->m_Index, 1);
		return TRUE;
	}

	
	if ( g_ArcaBattle.GetState() > 2 && g_ArcaBattle.GetState() < 9 )
	{
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,405), lpObj->m_Index, 1);
		return TRUE;
	}

	if ( lpObj->m_PlayerData->GuildNumber > 0 )
	{
		GSProtocol.ChatTargetSend(lpNpc, Lang.GetText(0,48), lpObj->m_Index);
		return TRUE;
	}

	int capacity = gObjGuildMasterCapacityTest(lpObj);

	if ( capacity == 0 )
	{
		GSProtocol.ChatTargetSend(lpNpc, Lang.GetText(0,49), lpObj->m_Index);
	}
	else if ( capacity == 2 )
	{
		GSProtocol.ChatTargetSend(lpNpc, Lang.GetText(0,50), lpObj->m_Index);
	}
	else
	{
		GSProtocol.GCGuildMasterQuestionSend(lpObj->m_Index);
	}

	return TRUE;
}



BOOL NpcRolensiaGuard(LPOBJ lpNpc, LPOBJ lpObj)
{
	if ( gEnableEventNPCTalk != FALSE )
	{
		GSProtocol.GCServerCmd(lpObj->m_Index, 4, 0, 0);
	}
	return TRUE;
}



BOOL NpcChaosGoblelin(LPOBJ lpNpc, LPOBJ lpObj)
{
	gUserFindDevilSquareKeyEyes(lpObj->m_Index);

	return FALSE;
}


BOOL g_bUseLotterySystem = TRUE;

BOOL NpcChaosCardMaster(LPOBJ lpNpc, LPOBJ lpObj)
{
	if ( g_bUseLotterySystem == FALSE )
		return FALSE;

	if ( bCanChaosBox == TRUE )
	{
		if ( lpObj->m_IfState.use > 0 )
			return FALSE;

		if ( lpObj->m_bPShopOpen == true )
		{
			g_Log.Add("[%s][%s] is Already Opening PShop, ChaosBox Failed",
				lpObj->AccountID, lpObj->Name);

			GSProtocol.GCServerMsgStringSend(Lang.GetText(0,112), lpObj->m_Index, 1);
			return TRUE;
		}

		PMSG_TALKRESULT pMsg;

		pMsg.h.c = 0xC3;
		pMsg.h.headcode = 0x30;
		pMsg.h.size = sizeof(pMsg);
		pMsg.result = 0x15;

		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

		lpObj->m_IfState.use = 1;
		lpObj->m_IfState.type = 7;

		g_Log.Add("[JewelMix] [%s][%s] Jewel Mix Dealer Start",
			lpObj->AccountID, lpObj->Name);

		gObjItemTextSave(lpObj);
		gObjWarehouseTextSave(lpObj);
	}

	return TRUE;
}



BOOL NpcRusipher(LPOBJ lpNpc, LPOBJ lpObj)
{
	if ( g_DevilSquare.IsEventEnable() == FALSE )
	{
		return TRUE;
	}

	int aIndex = lpObj->m_Index;

	if ( (lpObj->m_IfState.use ) > 0 )
	{
		return TRUE;
	}

	if ( gUserFindDevilSquareInvitation(aIndex) == FALSE )
	{
		GSProtocol.GCServerCmd(lpObj->m_Index, 1, 2 ,0);
	}
	else if ( g_DevilSquare.GetState() == 1 )
	{
		PMSG_TALKRESULT pMsg;

		pMsg.h.c = 0xC3;
		pMsg.h.headcode = 0x30;
		pMsg.h.size = sizeof(pMsg);
		pMsg.result = 0x04;

		IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

		lpObj->m_IfState.use = 1;
		lpObj->m_IfState.type = 12;
	}
	else
	{
		GSProtocol.GCServerCmd(lpObj->m_Index, 1, 3, 0);
	}

	return TRUE;
}



BOOL NpcPosionGirl(LPOBJ lpNpc, LPOBJ lpObj)
{
	return FALSE;
}

BOOL NpcDeviasWizard(LPOBJ lpNpc, LPOBJ lpObj)
{
	return FALSE;
}


BOOL NpcDeviasGuard(LPOBJ lpNpc, LPOBJ lpObj)
{
	return TRUE;
}


BOOL NpcDeviasWareHousemen(LPOBJ lpNpc, LPOBJ lpObj)
{
	return FALSE;
}

BOOL NpcWarehouse(LPOBJ lpNpc, LPOBJ lpObj)
{
	return FALSE;
}


BOOL NpcNoriaRara(LPOBJ lpNpc, LPOBJ lpObj)
{
	if ( gEnableEventNPCTalk != FALSE )
	{
		GSProtocol.GCServerCmd(lpObj->m_Index, 4, 2, 0);
	}

	return FALSE;
}

BOOL NpcDeviasMadam(LPOBJ lpNpc, LPOBJ lpObj)
{
	if ( gEnableEventNPCTalk != FALSE )
	{
		GSProtocol.GCServerCmd(lpObj->m_Index, 4, 1, 0);
	}

	return FALSE;
}

struct PMSG_REQ_VIEW_EC_MN
{
	PBMSG_HEAD2 h;	// C1:01
	int iINDEX;	// 4
	char szUID[11];	// 8
};

BOOL NpcEventChipNPC(LPOBJ lpNpc, LPOBJ lpObj)
{
	if ( (lpObj->m_IfState.use) > 0 )
	{
		return TRUE;
	}
	
	if ( lpNpc->MapNumber == 0 )
	{
		PMSG_REQ_VIEW_EC_MN pMsgTha;

		PHeadSubSetB((LPBYTE)&pMsgTha, 0xBE, 0x01, sizeof(pMsgTha));
		pMsgTha.iINDEX = lpObj->m_Index;
		strcpy(pMsgTha.szUID, lpObj->AccountID);
		pEventObj = lpNpc;

		wsDataCli.DataSend((char*)&pMsgTha, sizeof(pMsgTha));
		
		lpObj->m_IfState.type = 9;
		lpObj->m_IfState.state = 0;
		lpObj->m_IfState.use = 1;

		return TRUE;
	}

	if ( lpNpc->MapNumber == 2 )
	{
		PMSG_EVENTCHIPINFO Result;

		PHeadSetB((LPBYTE)&Result, 0x94, sizeof(Result));
		Result.Type = 2;
		
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&Result, Result.h.size);

		lpObj->m_IfState.type = 9;
		lpObj->m_IfState.state = 0;
		lpObj->m_IfState.use = 1;

		return TRUE;
	}

	return FALSE;
}


BOOL NpcRorensiaSmith(LPOBJ lpNpc, LPOBJ lpObj)
{
	return FALSE;
}



BOOL NpcNoriJangIn(LPOBJ lpNpc, LPOBJ lpObj)
{
	return FALSE;
}



BOOL NpcQuestCheck(LPOBJ lpNpc, LPOBJ lpObj)
{
	if ( g_QuestInfo.NpcTalk(lpNpc, lpObj) != FALSE )
	{
		return TRUE;
	}

	return FALSE;
}



BOOL NpcServerDivision(LPOBJ lpNpc, LPOBJ lpObj)
{
	if ( gEnableServerDivision == FALSE )
	{
		return TRUE;
	}

	if ( lpObj->m_PlayerData->lpGuild != NULL )
	{
		GSProtocol.GCServerCmd(lpObj->m_Index, 6, 0, 0);
		return TRUE;
	}

	lpObj->m_IfState.type = 11;
	lpObj->m_IfState.state = 0;
	lpObj->m_IfState.use = 1;

	return FALSE;
}


BOOL NpcRoadMerchant(LPOBJ lpNpc, LPOBJ lpObj)
{
	return FALSE;
}



BOOL NpcAngelKing(LPOBJ lpNpc, LPOBJ lpObj)
{
	return g_BloodCastle.NpcAngelKing(lpNpc, lpObj);
}


BOOL NpcAngelMessanger(LPOBJ lpNpc, LPOBJ lpObj)
{
	if ( (lpObj->m_IfState.use ) > 0 )
	{
		return TRUE;
	}

	int iIndex = lpObj->m_Index;

	if ( gObj[iIndex].m_PK_Level >= 4 && g_ConfigRead.data.common.PKEnterEventAllow == false )
	{
		GSProtocol.GCServerCmd(iIndex, 0x39, 0, 0);
		return TRUE;
	}

	int iITEM_LEVEL = 0;

	iITEM_LEVEL = g_BloodCastle.CheckEnterItem(lpObj->m_Index);

	if ( iITEM_LEVEL != 0 )
	{
		if ( iITEM_LEVEL != 10 && g_BloodCastle.CheckEnterLevel(lpObj->m_Index, iITEM_LEVEL-1) )
		{
			if ( g_BloodCastle.CheckEnterFreeTicket(lpObj->m_Index) == true )
			{
				iITEM_LEVEL = 10;
			}
		}

		if ( iITEM_LEVEL == 10 )
		{
			for (int i=0;i<MAX_CLOACK_LEVEL;i++)
			{
				if ( g_BloodCastle.CheckEnterLevel(lpObj->m_Index, i) == FALSE )
				{
					iITEM_LEVEL = i;
				}
			}
		}

		if ( g_BloodCastle.GetCurrentState(iITEM_LEVEL-1) == BC_STATE_CLOSED && g_BloodCastle.CheckCanEnter(iITEM_LEVEL-1) != false )
		{
			PMSG_TALKRESULT pMsg;

			pMsg.h.c = 0xC3;
			pMsg.h.headcode = 0x30;
			pMsg.h.size = sizeof(pMsg);
			pMsg.result = 0x06;

			IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

			lpObj->m_IfState.use = 1;
			lpObj->m_IfState.type = 12;
		}
		else
		{
			GSProtocol.GCServerCmd(lpObj->m_Index, 1, 0x14, 0);
		}
	}
	else
	{
		GSProtocol.GCServerCmd(lpObj->m_Index, 1, 0x15, 0);
	}

	return FALSE;
}



BOOL NpcRolensiaMadam(LPOBJ lpNpc, LPOBJ lpObj)
{
	return FALSE;
}



BOOL NpcDeviasWeapon(LPOBJ lpNpc, LPOBJ lpObj)
{
	return FALSE;
}



BOOL NpcDarkSpiritTrainer(LPOBJ lpNpc, LPOBJ lpObj)
{
	if ( bCanChaosBox == TRUE )
	{
		PMSG_TALKRESULT pMsg;

		pMsg.h.c = 0xC3;
		pMsg.h.headcode = 0x30;
		pMsg.h.size = sizeof(pMsg);
		
		if ( lpObj->m_bPShopOpen == true )
		{
			g_Log.Add("[%s][%s] is Already Opening PShop, ChaosBox Failed",
				lpObj->AccountID, lpObj->Name);
			GSProtocol.GCServerMsgStringSend(Lang.GetText(0,112), lpObj->m_Index, 1);

			return TRUE;
		}

		lpObj->m_IfState.type = 13;
		lpObj->m_IfState.state = 0;
		lpObj->m_IfState.use = 1;
		
		pMsg.result = 0x07;
		lpObj->bIsChaosMixCompleted = false;
		pMsg.level1 = gDQChaosSuccessRateLevel1;
		pMsg.level2 = gDQChaosSuccessRateLevel2;
		pMsg.level3 = gDQChaosSuccessRateLevel3;
		pMsg.level4 = gDQChaosSuccessRateLevel4;
		pMsg.level5 = gDQChaosSuccessRateLevel5;
		pMsg.level6 = gDQChaosSuccessRateLevel6;
		pMsg.level7 = gDQChaosSuccessRateLevel7;

		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

		GSProtocol.GCAnsCsMapSvrTaxInfo(lpObj->m_Index, 1, g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index));
		gObjInventoryTrans(lpObj->m_Index);

		g_Log.Add("[%s][%s] Open Chaos Box", lpObj->AccountID, lpObj->Name);
		gObjItemTextSave(lpObj);
		gObjWarehouseTextSave(lpObj);
	}

	return TRUE;
}



BOOL NpcJewelMixDealer(LPOBJ lpNpc, LPOBJ lpObj)
{
	if ( bCanChaosBox == TRUE )
	{
		if ( (lpObj->m_IfState.use) > 0 )
		{
			return TRUE;
		}

		PMSG_TALKRESULT pMsg;

		pMsg.h.c = 0xC3;
		pMsg.h.headcode = 0x30;
		pMsg.h.size = sizeof(pMsg);
		pMsg.result = 0x09;

		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

		lpObj->m_IfState.use = 1;
		lpObj->m_IfState.type = 12;

		g_Log.Add("[JewelMix] [%s][%s] Jewel Mix Dealer Start", lpObj->AccountID, lpObj->Name);

		gObjItemTextSave(lpObj);
		gObjWarehouseTextSave(lpObj);

	}

	return TRUE;
}


struct PMSG_ANS_USE_WEAPON_INTERFACE
{
	PBMSG_HEAD2 h;	// C1:B7:00
	BYTE btResult;	// 4
	BYTE btWeaponType;	// 5
	BYTE btObjIndexH;	// 6
	BYTE btObjIndexL;	// 7
};

BOOL NpcSiegeMachine_Attack(LPOBJ lpNpc, LPOBJ lpObj)
{
	if ( (lpObj->m_IfState.use) > 0 )
	{
		return TRUE;
	}

	PMSG_ANS_USE_WEAPON_INTERFACE pResult = {0};

	PHeadSubSetB((LPBYTE)&pResult, 0xB7, 0x00, sizeof(pResult));

	pResult.btResult = 0;
	pResult.btWeaponType = 1;
	pResult.btObjIndexH = SET_NUMBERH(lpNpc->m_Index);
	pResult.btObjIndexL = SET_NUMBERL(lpNpc->m_Index);

	if ( gObjIsConnectedGP(lpNpc->m_iWeaponUser) == FALSE )
	{
		lpNpc->m_iWeaponUser = 0;
		lpNpc->m_btWeaponState = 1;
	}

	lpNpc->m_btWeaponState = 1;

	if ( lpNpc->m_btWeaponState == 1 )
	{
		pResult.btResult = 1;
		lpNpc->m_btWeaponState = 2;
		lpNpc->m_iWeaponUser = lpObj->m_Index;

		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);

		lpObj->m_IfState.use = 1;
		lpObj->m_IfState.type = 15;
	}
	else
	{
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
	}

	return TRUE;
}


BOOL NpcSiegeMachine_Defense(LPOBJ lpNpc, LPOBJ lpObj)
{
	if ( (lpObj->m_IfState.use ) > 0 )
	{
		return TRUE;
	}

	PMSG_ANS_USE_WEAPON_INTERFACE pResult = {0};

	PHeadSubSetB((LPBYTE)&pResult, 0xB7, 0x00, sizeof(pResult));

	pResult.btResult = 0;
	pResult.btWeaponType = 2;
	pResult.btObjIndexH = SET_NUMBERH(lpNpc->m_Index);
	pResult.btObjIndexL = SET_NUMBERL(lpNpc->m_Index);

	if ( gObjIsConnectedGP(lpNpc->m_iWeaponUser) == FALSE )
	{
		lpNpc->m_iWeaponUser = 0;
		lpNpc->m_btWeaponState = 1;
	}

	lpNpc->m_btWeaponState = 1;

	if ( lpNpc->m_btWeaponState == 1 )
	{
		pResult.btResult = 1;
		lpNpc->m_btWeaponState = 2;
		lpNpc->m_iWeaponUser = lpObj->m_Index;

		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);

		lpObj->m_IfState.use = 1;
		lpObj->m_IfState.type = 16;
	}
	else
	{
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
	}

	return TRUE;
}

BOOL NpcElderCircle(LPOBJ lpNpc, LPOBJ lpObj) //GS-CS Decompiled 100%
{
	if ( (lpObj->m_IfState.use ) > 0 )
	{
		return TRUE;
	}

	if (g_ConfigRead.server.GetServerType() != SERVER_CASTLE)
	{
		return TRUE;
	}

	if(g_CastleSiege.GetCastleState() == 7) //Good
	{
		GSProtocol.ChatTargetSend(lpNpc,(Lang.GetText(0,164)), lpObj->m_Index);
		return TRUE;
	}

	if(strcmp(lpObj->m_PlayerData->GuildName, "") == 0 ) //Good
	{
		GSProtocol.ChatTargetSend(lpNpc,(Lang.GetText(0,165)), lpObj->m_Index);
		return TRUE;
	}

	if(g_CastleSiege.CheckGuildOwnCastle(lpObj->m_PlayerData->GuildName)) //Good
	{
		if( lpObj->m_PlayerData->GuildStatus != 128 )
		{
			if ( lpObj->m_PlayerData->GuildStatus != 64 )
			{
				GSProtocol.ChatTargetSend(lpNpc,(Lang.GetText(0,165)), lpObj->m_Index);
				return TRUE;
			}
		}
	}
	else
	{
		GSProtocol.ChatTargetSend(lpNpc,(Lang.GetText(0,165)), lpObj->m_Index);
		return TRUE;
	}

	PMSG_TALKRESULT pMsg;

	pMsg.h.c = 0xC3;
	pMsg.h.headcode = 0x30;
	pMsg.h.size = sizeof(pMsg);
	pMsg.result = 0x0C;

	lpObj->m_IfState.use = 1;
	lpObj->m_IfState.type = 12;

	pMsg.level1 = 0;
	pMsg.level2 = 0;
	pMsg.level3 = 0;
	pMsg.level4 = 0;
	pMsg.level5 = 0;
	pMsg.level6 = 0;
	pMsg.level7 = 0;

	if( bCanChaosBox == TRUE) //Good
	{
		lpObj->m_IfState.type = 7;

		lpObj->bIsChaosMixCompleted = false;
		lpObj->m_bCsGuildInvolved = false; //Good

		pMsg.level1 = 1; //Good

		GSProtocol.GCAnsCsMapSvrTaxInfo(lpObj->m_Index, 1, g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index));
		gObjInventoryTrans(lpObj->m_Index);

		g_Log.Add("[%s][%s] Open Chaos Box", lpObj->AccountID, lpObj->Name);
		gObjItemTextSave(lpObj);
		gObjWarehouseTextSave(lpObj);
	}

	IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
	return TRUE;
}



BOOL NpcCastleGuard(LPOBJ lpNpc, LPOBJ lpObj) //GS-CS Decompiled 100%
{
	if ( (lpObj->m_IfState.use ) > 0 )
	{
		return TRUE;
	}

	if (g_ConfigRead.server.GetServerType() != SERVER_CASTLE)
	{
		return TRUE;
	}

	if(g_CastleSiege.GetCastleState() == 7)
	{
		GSProtocol.ChatTargetSend(lpNpc,(Lang.GetText(0,166)), lpObj->m_Index);
		return TRUE;
	}

	PMSG_TALKRESULT pMsg;

	pMsg.h.c = 0xC3;
	pMsg.h.headcode = 0x30;
	pMsg.h.size = sizeof(pMsg);
	pMsg.result = 0x0D;

	IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

	lpObj->m_IfState.use = 1;
	lpObj->m_IfState.type = 12;

	return TRUE;
}



BOOL NpcCastleGateLever(LPOBJ lpNpc, LPOBJ lpObj) //GS-CS Decompiled 100%
{
	if ( (lpObj->m_IfState.use) > 0 )
	{
		return TRUE;
	}

	if (g_ConfigRead.server.GetServerType() != SERVER_CASTLE)
	{
		return TRUE;
	}

	BOOL bControlEnable = 0;
	BYTE btResult = 0;
	int iGateIndex = -1;

	if(g_CastleSiege.GetCastleState() == 7)
	{
		if(lpObj->m_btCsJoinSide != TRUE)
		{
			btResult = 4;
			bControlEnable = FALSE;
		}
		else
		{
			bControlEnable = TRUE;
		}
	}
	else
	{
		if(!g_CastleSiege.CheckCastleOwnerMember(lpObj->m_Index))
		{
			if(!g_CastleSiege.CheckCastleOwnerUnionMember(lpObj->m_Index))
			{
				btResult = 4;
				bControlEnable = FALSE;
			}
			else
			{
				bControlEnable = TRUE;
			}
		}
		else
		{
			bControlEnable = TRUE;
		}
	}

	if(bControlEnable != FALSE)
	{
		if(g_CastleSiege.CheckLeverAlive(lpNpc->m_Index) == TRUE)
		{
			if(g_CastleSiege.CheckCsGateAlive(lpNpc->m_iCsGateLeverLinkIndex) == TRUE)
			{
				btResult = 1;
				iGateIndex = lpNpc->m_iCsGateLeverLinkIndex;
			}
			else
			{
				btResult = 2;
			}
		}
	}
	else
	{
		btResult = 3;
	}

	GSProtocol.GCAnsCsGateState(lpObj->m_Index,btResult,iGateIndex);

	if(btResult != 1)
	{
		return TRUE;
	}

	lpObj->m_IfState.use = 1;
	lpObj->m_IfState.type = 12;

	return TRUE;
}



BOOL NpcCastleCrown(LPOBJ lpNpc, LPOBJ lpObj) //GS-CS Decompiled 100%
{
	if (g_ConfigRead.server.GetServerType() != SERVER_CASTLE)
	{
		return TRUE;
	}

	if ( gObjIsConnected(lpObj->m_Index) == FALSE ) //Good
	{
		return TRUE;
	}

	if(g_CastleSiege.GetCastleState() != 7) //Good
	{
		return TRUE;
	}
	
	if(lpObj->m_btCsJoinSide < 2 || lpObj->m_bCsGuildInvolved == FALSE )
	{
		return TRUE;
	}

	if((lpObj->Y - lpNpc->Y) > 3 || (lpObj->X - lpNpc->X) > 3)
	{
		return TRUE;
	}

	if(g_CastleSiege.GetRegCrownAvailable() == FALSE )
	{
		return TRUE;
	}

	if(g_CastleSiege.CheckUnionGuildMaster(lpObj->m_Index) == FALSE )
	{
		return TRUE;
	}

	int iUserIndex = g_CastleSiege.GetCrownUserIndex();

	if(gObjIsConnected(iUserIndex) == FALSE )
	{
		int iSwitchIndex1 = g_CastleSiege.GetCrownSwitchUserIndex(217);
		int iSwitchIndex2 = g_CastleSiege.GetCrownSwitchUserIndex(218);

		if(gObjIsConnected(iSwitchIndex1) == FALSE || gObjIsConnected(iSwitchIndex2) == FALSE )
		{
			g_Log.Add("[CastleSiege] [%s][%s] Failed to Register Castle Crown (GUILD:%s)",lpObj->AccountID,lpObj->Name,lpObj->m_PlayerData->GuildName);
			return TRUE;
		}

		if(lpObj->m_btCsJoinSide != gObj[iSwitchIndex1].m_btCsJoinSide || lpObj->m_btCsJoinSide != gObj[iSwitchIndex2].m_btCsJoinSide)
		{
			GSProtocol.GCAnsCsAccessCrownState(lpObj->m_Index,4);
			g_Log.Add("[CastleSiege] [%s][%s] Failed to Register Castle Crown (GUILD:%s) (S1:%s/%s)(S2:%s/%s)",lpObj->AccountID,lpObj->Name,lpObj->m_PlayerData->GuildName,gObj[iSwitchIndex1].Name,gObj[iSwitchIndex1].m_PlayerData->GuildName,gObj[iSwitchIndex2].Name,gObj[iSwitchIndex2].m_PlayerData->GuildName);
			return TRUE;
		}
		else
		{
			GSProtocol.GCAnsCsAccessCrownState(lpObj->m_Index,0);
			g_CastleSiege.SetCrownUserIndex(lpObj->m_Index);
			g_CastleSiege.SetCrownAccessUserX(lpObj->X);
			g_CastleSiege.SetCrownAccessUserY(lpObj->Y);
			g_CastleSiege.SetCrownAccessTickCount();
			g_CastleSiege.NotifyAllUserCsProgState(0,lpObj->m_PlayerData->GuildName);
			g_Log.Add("[CastleSiege] [%s][%s] Start to Register Castle Crown (GUILD:%s)",lpObj->AccountID,lpObj->Name,lpObj->m_PlayerData->GuildName);
		}
	}
	else if(lpObj->m_Index != iUserIndex)
	{
		GSProtocol.GCAnsCsAccessCrownState(lpObj->m_Index,3);
	}

	return TRUE;
}



BOOL NpcCastleSwitch(LPOBJ lpNpc, LPOBJ lpObj) //GS-CS Decompiled 100%
{
	if (g_ConfigRead.server.GetServerType() != SERVER_CASTLE)
	{
		return TRUE;
	}

	if(gObjIsConnected(lpObj->m_Index) == FALSE )
	{
		return TRUE;
	}

	if(g_CastleSiege.GetCastleState() != 7)
	{
		return TRUE;
	}
	
	if(lpObj->m_btCsJoinSide < 2)
	{
		return TRUE;
	}

	if((lpObj->Y - lpNpc->Y) > 3 || (lpObj->X - lpNpc->X) > 3)
	{
		return TRUE;
	}

	if(g_CastleSiege.CheckGuardianStatueExist())
	{
		MsgOutput(lpObj->m_Index,Lang.GetText(0,201));
		return TRUE;
	}

	int iUserIndex = g_CastleSiege.GetCrownSwitchUserIndex(lpNpc->Class);

	if(!gObjIsConnected(iUserIndex))
	{
		GSProtocol.GCAnsCsAccessSwitchState(lpObj->m_Index,lpNpc->m_Index,-1,1);
		g_CastleSiege.SetCrownSwitchUserIndex(lpNpc->Class,lpObj->m_Index);
		g_Log.Add("[CastleSiege] [%s][%s] Start to Push Castle Crown Switch (GUILD:%s) - CS X:%d/Y:%d",lpObj->AccountID,lpObj->Name,lpObj->m_PlayerData->GuildName,lpNpc->X,lpNpc->Y);
	}
	else if(lpObj->m_Index != iUserIndex)
	{
		GSProtocol.GCAnsCsAccessSwitchState(lpObj->m_Index,lpNpc->m_Index,iUserIndex,2);
	}

	return TRUE;
}

BOOL NpcSeedMaster(LPOBJ lpNpc, LPOBJ lpObj)
{
	PMSG_TALKRESULT pMsg;

	pMsg.h.c = 0xC3;
	pMsg.h.headcode = 0x30;
	pMsg.h.size = sizeof(pMsg);
	pMsg.result = 0x17;
	lpObj->m_IfState.type = 7;
	lpObj->m_IfState.state = 0;
	lpObj->m_IfState.use = 1;
	lpObj->bIsChaosMixCompleted = false;

	gObjInventoryTrans(lpObj->m_Index);

	g_Log.Add("[SeedMaster][%s][%s] Open Chaos Box",
		lpObj->AccountID, lpObj->Name);

	gObjItemTextSave(lpObj);
	gObjWarehouseTextSave(lpObj);
	GSProtocol.GCAnsCsMapSvrTaxInfo(lpObj->m_Index, 1, g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index));

	IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
	return TRUE;
}
BOOL NpcSeedResearcher(LPOBJ lpNpc, LPOBJ lpObj)
{
	PMSG_TALKRESULT pMsg;

	pMsg.h.c = 0xC3;
	pMsg.h.headcode = 0x30;
	pMsg.h.size = sizeof(pMsg);
	pMsg.result = 0x18;
	lpObj->m_IfState.type = 7;
	lpObj->m_IfState.state = 0;
	lpObj->m_IfState.use = 1;
	lpObj->bIsChaosMixCompleted = false;

	gObjInventoryTrans(lpObj->m_Index);

	g_Log.Add("[SeedResearcher][%s][%s] Open Chaos Box",
		lpObj->AccountID, lpObj->Name);

	gObjItemTextSave(lpObj);
	gObjWarehouseTextSave(lpObj);
	GSProtocol.GCAnsCsMapSvrTaxInfo(lpObj->m_Index, 1, g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index));

	IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
	return TRUE;
}

bool NpcTitus(LPOBJ lpNpc, LPOBJ lpObj)
{
	if( lpObj->m_IfState.use > 0 )
	{
		return TRUE;
	}

	g_NewPVP.SendChannelList(*lpNpc, *lpObj);

	PMSG_TALKRESULT pResult;

	pResult.h.c = 0xC3;
	pResult.h.headcode = 0x30;
	pResult.h.size = sizeof(pResult);
	pResult.result = 0x21;
	IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
			
	lpObj->m_IfState.use = 1;
	lpObj->m_IfState.type = 12;

	return TRUE;
}

bool NpcLugard(LPOBJ lpNpc, LPOBJ lpObj)
{
	if ( gObjIsConnected(lpObj) == false )
	{
		return false;
	}

	if ( g_DoppelGanger.IsEventEnable() == false )
	{
		return false;
	}

	g_DoppelGanger.PlatformLugardAct(lpNpc, lpObj);
	return true;
}

bool NpcSilverBox(LPOBJ lpNpc, LPOBJ lpObj)
{
	if ( gObjIsConnected(lpObj) == false )
	{
		return false;
	}

	g_DoppelGanger.MiddleTreasureAct(lpNpc, lpObj);
	return true;
}

bool NpcGoldBox(LPOBJ lpNpc, LPOBJ lpObj)
{
	if ( gObjIsConnected(lpObj) == false )
	{
		return false;
	}

	g_DoppelGanger.LastTreasureAct(lpNpc, lpObj);
	return true;
}

bool NpcJerint(LPOBJ lpNpc, LPOBJ lpObj)
{
	PMSG_TALKRESULT pMsg;
	PHeadSetBE((LPBYTE)&pMsg, 0x30, 5);
	pMsg.result = 0x24;
	pMsg.level1 = 0;
	lpObj->TargetNpcNumber = lpNpc->m_Index;
	IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, 5);
	return true;
}

bool NpcJulia(LPOBJ lpNpc, LPOBJ lpObj)
{
	PMSG_TALKRESULT pMsg;
	PHeadSetBE((LPBYTE)&pMsg, 0x30, 5);
	pMsg.result = 0x25;
	pMsg.level1 = 0;
	IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, 5);
	return true;
}

struct PMSG_ANS_GUARD_IN_CASTLE_HUNTZONE {
  /*<thisrel this+0x0>*/ /*|0x4|*/ struct PBMSG_HEAD2 h;
  /*<thisrel this+0x4>*/ /*|0x1|*/ BYTE btResult;
  /*<thisrel this+0x5>*/ /*|0x1|*/ BYTE btUsable;
  /*<thisrel this+0x8>*/ /*|0x4|*/ int iCurrentPrice;
  /*<thisrel this+0xc>*/ /*|0x4|*/ int iMaxPrice;
  /*<thisrel this+0x10>*/ /*|0x4|*/ int iUnitOfPrice;
};
// <size 0x14>

BOOL NpcCastleHuntZoneGuard(LPOBJ lpNpc, LPOBJ lpObj)
{
	if(gObjIsConnected(lpObj->m_Index) == FALSE )
	{
		return TRUE;
	}

	if ( (lpObj->m_IfState.use ) > 0 )
	{
		return TRUE;
	}

	if (g_ConfigRead.server.GetServerType() != SERVER_CASTLE)
	{
		return TRUE;
	}

	PMSG_ANS_GUARD_IN_CASTLE_HUNTZONE pResult = {0};

	PHeadSubSetB((LPBYTE)&pResult,0xB9,0x03,sizeof(PMSG_ANS_GUARD_IN_CASTLE_HUNTZONE));
	pResult.btResult = TRUE;
	pResult.iMaxPrice = 300000;
	pResult.iUnitOfPrice = 10000;
	pResult.btUsable = 0;
	pResult.iCurrentPrice = 0;

	pResult.iCurrentPrice = g_CastleSiegeSync.GetTaxHuntZone(lpObj->m_Index,FALSE);

	if(g_CastleSiege.GetHuntZoneEnter())
	{
		pResult.btUsable = TRUE;
	}

	if(lpObj->m_PlayerData->lpGuild != NULL)
	{
		if(g_CastleSiege.CheckCastleOwnerUnionMember(lpObj->m_Index))
		{
			pResult.btResult = 2;
		}

		if(g_CastleSiege.CheckCastleOwnerMember(lpObj->m_Index))
		{
			if(lpObj->m_PlayerData->GuildStatus == G_MASTER)
			{
				pResult.btResult = 3;
			}
		}
	}
	else
	{
		if(g_CastleSiege.GetHuntZoneEnter())
		{
			pResult.btResult = TRUE;
		}
	}

	IOCP.DataSend(lpObj->m_Index,(LPBYTE)&pResult,pResult.h.size);

	return TRUE;
}

bool NpcDavid(LPOBJ lpNpc, LPOBJ lpObj)
{
	PMSG_TALKRESULT pMsg;
	pMsg.h.c = 0xC3;
	pMsg.h.headcode = 0x30;
	pMsg.h.size = sizeof(pMsg);
	pMsg.result = 0x26;
	lpObj->m_IfState.type = 7;
	lpObj->m_IfState.state = 0;
	lpObj->m_IfState.use = 1;
	lpObj->bIsChaosMixCompleted = false;

	g_Log.Add("[LuckyItem Master][%s][%s] Open Chaos Box",
		lpObj->AccountID, lpObj->Name);

	gObjItemTextSave(lpObj);
	gObjWarehouseTextSave(lpObj);
	GSProtocol.GCAnsCsMapSvrTaxInfo(lpObj->m_Index, 1, g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index));
	IOCP.DataSend(lpObj->m_Index,(LPBYTE)&pMsg,pMsg.h.size);
	return FALSE;
}

bool NpcLeoHelper(LPOBJ lpNpc,LPOBJ lpObj)
{
	if(gObjIsConnected(lpObj->m_Index) == FALSE)
	{
		return TRUE;
	}

	static int msg = 0x26;

	msg++;

	PMSG_TALKRESULT pMsg;
	pMsg.h.c = 0xC3;
	pMsg.h.headcode = 0x30;
	pMsg.h.size = sizeof(pMsg);
	pMsg.result = msg;

	IOCP.DataSend(lpObj->m_Index,(LPBYTE)&pMsg,pMsg.h.size);

	g_Log.Add("%x", msg);

	return TRUE;
}

bool NpcSantaClause(LPOBJ lpNpc, LPOBJ lpObj)
{
	if(gObjIsConnected(lpObj->m_Index) == FALSE)
	{
		return TRUE;
	}

	if(lpObj->m_PlayerData->m_iResets < g_ConfigRead.data.common.XMasSantaMinReset)
	{
		return TRUE;
	}

	PMSG_REQ_SANTACHECK pSanta;
	PHeadSubSetB((LPBYTE)&pSanta, 0xBE, 0x20, sizeof(pSanta));

	memcpy(pSanta.AccountID, lpObj->AccountID, 11);
	pSanta.aIndex = lpObj->m_Index;
	pSanta.gGameServerCode = g_ConfigRead.server.GetGameServerCode();

	wsDataCli.DataSend((char *)&pSanta, sizeof(pSanta));

	return TRUE;
}

bool NpcAcheronEntrance(LPOBJ lpNpc, LPOBJ lpObj)
{
	if ( gObjIsConnected(lpObj->m_Index) == FALSE )
	{
		return TRUE;
	}

	lpObj->TargetNumber = lpNpc->m_Index;

	PMSG_TALKRESULT pMsg;
	PHeadSetBE((LPBYTE)&pMsg, 0x30, sizeof(pMsg));
	pMsg.result = 0x27;

	IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

	g_Log.Add("[NPC] [%s][%s] Talk with Acheron Entrance NPC", lpObj->AccountID, lpObj->Name);

	return TRUE;
}

bool NpcArcaWar(LPOBJ lpNpc, LPOBJ lpObj)
{
	if ( gObjIsConnected(lpObj->m_Index) == FALSE )
	{
		return TRUE;
	}

	lpObj->TargetNumber = lpNpc->m_Index;

	PMSG_TALKRESULT pMsg;
	PHeadSetBE((LPBYTE)&pMsg, 0x30, sizeof(pMsg));
	pMsg.result = 0x28;

	IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

	g_Log.Add("[NPC] [%s][%s] Talk with Arca War NPC", lpObj->AccountID, lpObj->Name);

	return TRUE;
}

bool NpcElementalMaster(LPOBJ lpNpc, LPOBJ lpObj)
{
	if (lpObj->m_IfState.use > 0)
		return FALSE;

	if (lpObj->m_bPShopOpen == true)
	{
		g_Log.Add("[%s][%s] is Already Opening PShop, PentagramMixBox Failed",
			lpObj->AccountID, lpObj->Name);

		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,112), lpObj->m_Index, 1);
		return TRUE;
	}

	PMSG_TALKRESULT pMsg;

	pMsg.h.c = 0xC3;
	pMsg.h.headcode = 0x30;
	pMsg.h.size = sizeof(pMsg);
	pMsg.result = 0x29;
	lpObj->m_IfState.type = 7;
	lpObj->m_IfState.state = 0;
	lpObj->m_IfState.use = 1;
	lpObj->m_PlayerData->m_bIsPentagramMixCompleted = false;

	gObjInventoryTrans(lpObj->m_Index);
	g_PentagramMixSystem.PentagramMixBoxInit(lpObj);

	g_Log.Add("[PentagramMix] [%s][%s] Pentagram Mix Start",
		lpObj->AccountID, lpObj->Name);

	gObjItemTextSave(lpObj);
	gObjWarehouseTextSave(lpObj);

	IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
	return TRUE;
}

//1.01.00	-> 617C20 -> 99% (Need fix state)
bool Npc_Dialog(LPOBJ lpNpc, LPOBJ lpObj) //-> Complete
{
	PMSG_ANS_DIG_NPC_CLICK pResult;
	char * GensName;

	if (gObjIsConnected(lpObj->m_Index))
	{
		lpObj->TargetNpcNumber = lpNpc->m_Index;

		switch (lpNpc->Class) // Not original, but easier
		{
		case 543:
		{
			pResult.dwContributePoint = g_GensSystem.GetContributePoint(lpObj);
			GensName = g_GensSystem.GetGensInfluenceName(lpObj);

			g_Log.Add("[Gens System] Gens_Duprian_NPC [%s][%s] GensName [%s] Total ContributePoint [%d]", lpObj->AccountID, lpObj->Name, GensName, pResult.dwContributePoint);
		}
			break;

		case 544:
		{
			pResult.dwContributePoint = g_GensSystem.GetContributePoint(lpObj);
			GensName = g_GensSystem.GetGensInfluenceName(lpObj);

			g_Log.Add("[Gens System] Gens_Vanert_NPC [%s][%s] GensName [%s] Total ContributePoint [%d]", lpObj->AccountID, lpObj->Name, GensName, pResult.dwContributePoint);
		}
			break;

		case 257: case 566:
		case 567: case 568:
		case 581:
		{
			pResult.dwContributePoint = 0;
		}
			break;
		}

		if (lpNpc->Class == 568)
		{
			lpObj->m_IfState.type = 20;
			lpObj->m_IfState.state = 0;
			lpObj->m_IfState.use = 1;
		}

		pResult.h.c = 0xC3;
		pResult.h.headcode = 0xF9;
		pResult.h.subcode = 1;
		pResult.h.size = sizeof(pResult);
		pResult.wNPCIndex = lpNpc->Class;

		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);

		return true;
	}

	return true;
}

bool NpcShadowPhantom(int iObjIndex) //-> Complete
{
	LPOBJ lpObj = &gObj[iObjIndex];

	if (!gObjIsConnected(lpObj->m_Index))
	{
		return true;
	}

	if (lpObj->Type != OBJ_USER)
	{
		return true;
	}

	if (lpObj->Level > g_iShadowPhantomMaxLevel || lpObj->m_PlayerData->ChangeUP == 2)
	{
		GSProtocol.GCServerCmd(lpObj->m_Index, 13, 0, 0);
		return true;
	}

	int iIncleaseAttackPower = 0;
	int iIncleaseDefensePower = 0;

	if (lpObj->Level <= 180)
	{
		iIncleaseAttackPower = lpObj->Level / 3 + 45;
		iIncleaseDefensePower = lpObj->Level / 5 + 50;
	}
	else
	{
		iIncleaseAttackPower = 105;
		iIncleaseDefensePower = 86;
	}

	gObjAddBuffEffect(lpObj, BUFFTYPE_ATTDEF_POWER_INC, EFFECTTYPE_IMPROVE_DAMAGE, iIncleaseAttackPower, EFFECTTYPE_IMPROVE_DEFENSE, iIncleaseDefensePower, (lpObj->Level / 6 + 30) * 60);
	return true;
}

bool NpcMarketWall(LPOBJ lpNpc, LPOBJ lpObj)
{
	if (!gObjIsConnected(lpObj->m_Index))
	{
		return false;
	}

	PMSG_TALKRESULT pResult;

	pResult.h.c = 0xC3;
	pResult.h.headcode = 0x30;
	pResult.h.size = sizeof(pResult);
	pResult.result = 0x30;

	IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);

	return true;
}

bool NpcMonicaTalk(LPOBJ lpNpc, LPOBJ lpObj)
{
	if (!gObjIsConnected(lpObj->m_Index))
		return false;

	if (lpObj->m_IfState.use > 0)
		return false;

    PMSG_TALKRESULT pResult; 

	pResult.h.c = 0xC3;
	pResult.h.headcode = 0x30;
	pResult.h.size = sizeof(pResult);
	pResult.result = 0x33;

	IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
	return true;
}
bool NpcWerewolfQuarrel(LPOBJ lpNpc, LPOBJ lpObj)
{
	if (gObjIsConnected(lpObj->m_Index))
	{
		lpObj->TargetNpcNumber = lpNpc->m_Index;
	}

	return true;
}

bool NpcGateKeeper(LPOBJ lpNpc, LPOBJ lpObj)
{
	if (gObjIsConnected(lpObj->m_Index))
	{
		lpObj->TargetNpcNumber = lpNpc->m_Index;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

