//////////////////////////////////////////////////////////////////////
// protocol.cpp
// ------------------------------
// Decompiled by Deathway
// Date : 2007-05-09
// ------------------------------
// GS-N 0.99.60T 0x00426C80F
//	GS-N	1.00.18	JPN	0x0042FBF0	-	Completed
// PMoveProc - Weird form of compilation
// void GameProtocol::CGCloseWindow(int aIndex) Weird form of compilation
#include "stdafx.h"
#include "protocol.h"
#include "user.h"
#include "giocp.h"
#include "Event.h"
#include "TLog.h"
#include "GameMain.h"
#include "winutil.h"
#include "TNotice.h"
#include "QuestInfo.h"
#include "ConMember.h"
#include "GameServer.h"
#include "GMMng.h"
#include "BattleSoccerManager.h"
#include "BuffEffectSlot.h"
#include "ChaosBox.h"
#include "CastleSiegeSync.h"
#include "TUnion.h"
#include "JewelMixSystem.h"
#include "Weapon.h"
#include "ImperialGuardian.h"
#include "MasterLevelSkillTreeSystem.h"
#include "KalimaGate.h"
#include "ObjUseSkill.h"
#include "ObjCalCharacter.h"
#include "NpcTalk.h"
#include "Shop.h"
#include "PeriodItemEx.h"
#include "BattleSoccer.h"
#include "BattleSoccerManager.h"
#include "gObjMonster.h"
#include "ComboAttack.h"
#include "Gate.h"
#include "MoveCommand.h"
#include "MultiAttackHackCheck.h"
#include "ItemAddOption.h"
#include "JewelOfHarmonySystem.h"
#include "PacketCheckSum.h"
#include "DarkSpirit.h"
#include "Crywolf.h"
#include "CrywolfAltar.h"
#include "CrywolfSync.h"
#include "DevilSquare.h"
#include "KanturuEntranceNPC.h"
#include "CashShop.h"
#include "PentagramSystem.h"
#include "IllusionTempleEvent_Renewal.h"
#include "Sprotocol.h"
#include "DSProtocol.h"
#include "EDSProtocol.h"
#include "Mercenary.h"
#include "ItemSocketOptionSystem.h"
#include "Guardian.h"
#include "DoppelGanger.h"
#include "AntiSwear.h"
#include "ChaosCastle.h"
#include "ChaosCastleFinal.h"
#include "GamblingSystem.h"
#include "configread.h"
//#include "bcore.h"
#include "GensSystem.h"
#include "GensSystemProtocol.h"
#include "NewPVP.h"
#include "MapServerManager.h"
#include "MineSystem.h"
#include "VipSys.h"
#include "LifeStone.h"
#include "QuestExpProtocol.h"
#include "QuestExpProgMng.h"
#include "QuestExpInfo.h"
#include "QuestExpLuaBind.h"
#include "LuaFun.h"
#include "BagManager.h"
#include "SetItemOption.h"
#include "LuckyItemManager.h"
#include "PentagramSystem.h"
#include "PentagramMixSystem.h"
#include "ProhibitedSymbols.h"
#include "PersonalStore.h"
#include "MuunSystem.h"
#include "MuRummyMng.h"
#include "KeyGenerater.h"
#include "AcheronGuardianEvent.h"
#include "GremoryCase.h"
#include "DevilSquareFinal.h"
#include "BotSystem.h"
#include "ExpManager.h"

GameProtocol GSProtocol;

GameProtocol::GameProtocol()
{
}

GameProtocol::~GameProtocol()
{
}

bool GameProtocol::PacketCheckTime(int aIndex)
{	
	if(!ObjectMaxRange(aIndex))
		return false;

	LPOBJ lpObj = &gObj[aIndex];

	if ( ( GetTickCount() - lpObj->m_PacketCheckTime ) < 300 )
	{
		return false;
	}

	lpObj->m_PacketCheckTime = GetTickCount();
	return true;
}

bool GameProtocol::DataEncryptCheck(int aIndex, BYTE protoNum, BOOL Encrypt)
{
#ifndef EMU_NOCRYPT
	if(!ObjectMaxRange(aIndex))
		return false;

	if ( Encrypt == FALSE )
	{
		g_Log.Add("Error-L1 : Not Encrypt %s %d", gObj[aIndex].AccountID, protoNum);
		this->GCSendDisableReconnect(aIndex);
		//IOCP.CloseClient(aIndex);
		return false;
	}
#endif
	return true;
}

void GameProtocol::ProtocolCore(BYTE protoNum, unsigned char * aRecv, int aLen, int aIndex, BOOL Encrypt)
{
	// Check if we wanna log the packets
	if ( gStalkProtocol )
	{
		if ( gStalkProtocolId[0] == gObj[aIndex].AccountID[0] )
		{
			if ( gStalkProtocolId[1] == gObj[aIndex].AccountID[1] )
			{
				if ( !strcmp(gStalkProtocolId, gObj[aIndex].AccountID ) )
				{
					g_Log.AddHeadHex("DATA RECV", aRecv, aLen);
				}
			}
		}
	}

	if ( protoNum == 0xF1 || protoNum == 0xF3 )
	{
		switch ( protoNum )
		{
			case 0x0E:
				if ( this->DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
				{
					this->CGLiveClient((PMSG_CLIENTTIME *)aRecv, aIndex);
				}
				break;
			case 0xF1:
				{
					PMSG_DEFAULT2 * lpMsg2 = (PMSG_DEFAULT2 *)aRecv;
					
					switch ( lpMsg2->subcode )
					{
						case 0x01:
							if ( this->DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
							{
								this->CSPJoinIdPassRequest((PMSG_IDPASS *)aRecv, aIndex);
							}
							break;
						case 0x02:
							if ( this->DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
							{
								this->CGClientCloseMsg((PMSG_CLIENTCLOSE *)aRecv, aIndex);
							}
							break;
						case 0x03:
							if ( this->DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
							{
								this->CGClientMsg((PMSG_CLIENTMSG *)aRecv, aIndex);
							}
							break;
						default:
							g_Log.AddC(TColor::Blue,  "UNKNOWN F1 PACKET: %x %x %x %x %x %x %x %x %x %x", aRecv[0], aRecv[1], aRecv[2], aRecv[3], aRecv[4], aRecv[5], aRecv[6], aRecv[7], aRecv[8], aRecv[9]);
							break;

					}
				}
				break;
			case 0xF3:
				{
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;

					switch ( lpDef->subcode )
					{
						case 0x00:
							DataServerGetCharListRequest(aIndex);
							break;
						case 0x01:
							this->CGPCharacterCreate((PMSG_CHARCREATE *)aRecv, aIndex);
							break; 
						case 0x02:
							this->CGPCharDel((PMSG_CHARDELETE *)aRecv, aIndex);
							break;
						case 0x03:
							this->CGPCharacterMapJoinRequest((PMSG_CHARMAPJOIN *)aRecv, aIndex);
							break;
						case 0x06:
							this->CGLevelUpPointAdd((PMSG_LVPOINTADD *)aRecv, aIndex);
							break;
						case 0x10: // not used packet
							break;
						case 0x12:
							gObjMoveDataLoadingOK(aIndex);
							break;
						case 0x15: // by drakelv & wizzy
						{
							struct PMSG_CHARCHECK
							{
								PBMSG_HEAD2 h; // C1:15
								char szName[MAX_ACCOUNT_LEN];
							};

							struct PMSG_CHARCHECK_RESULT
							{
								PBMSG_HEAD2 h; // C1:15
								char szName[MAX_ACCOUNT_LEN];
								BYTE bIsFromOtherAccount; // if i put 0x0e == 1 it pop's the selection screen
							};

							PMSG_CHARCHECK_RESULT pMsg;

							PHeadSubSetB((LPBYTE)&pMsg, 0xF3, 0x15, sizeof(pMsg));
							memcpy(pMsg.szName, gObj[aIndex].Name, MAX_ACCOUNT_LEN);
							pMsg.bIsFromOtherAccount = 0; // if i put 0x0e == 1 it pop's the selection screen

							IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
 
						}
						break;
						case 0x21:
							this->CGReqUseTransformationRing((PMSG_REQ_USETRANSFORMATIONRING *)aRecv, aIndex);
							break;
						case 0x26:
							this->CGReqDoUsePopupType(aIndex);
							break;
						case 0x30:
							this->GCSkillKeyRecv((PMSG_SKILLKEY *)aRecv, aIndex);
							break;
						case 0x52:
							g_MasterLevelSkillTreeSystem.CGReqGetMasterLevelSkill((PMSG_REQ_MASTERLEVEL_SKILL *)aRecv, aIndex);
							break;
						case 0x31:
							g_Log.AddC(TColor::Blue,  "WARNING: TRAP MESSAGE PACKET FROM MAIN: %x %x %x %x %x %x %x %x %x %x", Encrypt, aRecv[0], aRecv[1], aRecv[2], aRecv[3], aRecv[4], aRecv[5], aRecv[6], aRecv[7], aRecv[8], aRecv[9]);
							break;
						default:
							g_Log.AddC(TColor::Blue,  "UNKNOWN F3 PACKET: %x %x %x %x %x %x %x %x %x %x", aRecv[0], aRecv[1], aRecv[2], aRecv[3], aRecv[4], aRecv[5], aRecv[6], aRecv[7], aRecv[8], aRecv[9]);
							break;
					}
				}
				break;
		}
	}
	else
	{
		LPOBJ lpObj = &gObj[aIndex];

		switch ( protoNum )
		{
			case 0x00:
				this->PChatProc((PMSG_CHATDATA *)aRecv, aIndex);
				break;
			case 0x01:
				this->CGChatRecv((PMSG_CHATDATA_NUMBER *)aRecv, aIndex);
				break;
			case 0x02:
				this->CGChatWhisperRecv((PMSG_CHATDATA_WHISPER *)aRecv, aIndex);
				break;
			case 0x03:
				this->CGCheckMainRecv((PMSG_CHECK_MAINEXE_RESULT *)aRecv, aIndex);
				break;
			case 0x0E:
				this->CGLiveClient((PMSG_CLIENTTIME *)aRecv, aIndex);
				break;
			case 0xD4:
				this->PMoveProc((PMSG_MOVE *)aRecv, aIndex);
				break;
			case 0x15:
				this->RecvPositionSetProc((PMSG_POSISTION_SET *)aRecv, aIndex);
				break;
			case 0x11:
				this->CGAttack((PMSG_ATTACK *)aRecv, aIndex);
				break;
			case 0x18:
				this->CGActionRecv((PMSG_ACTION *)aRecv, aIndex);
				break;
			case 0x19:
				if ( this->DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
				{
					this->CGMagicAttack(aRecv, aIndex);
				}
				break;
			case 0x1B:
				this->CGMagicCancel((PMSG_MAGICCANCEL *)aRecv, aIndex);
				break;
			case 0x1C:
				if ( this->DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
				{
					this->CGTeleportRecv((PMSG_TELEPORT *)aRecv, aIndex);
				}
				break;
			case 0x8E:
				if (aRecv[3] == 0x02)
				{
					this->CGReqMapMove((PMSG_REQ_MAPMOVE *)aRecv, aIndex);
				}
				break;
			case 0xB0:
				if ( this->DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
				{
					this->CGTargetTeleportRecv((PMSG_TARGET_TELEPORT *)aRecv, aIndex);
				}
				break;
			case 0xDF:
				this->CGBeattackRecv(aRecv, aIndex, FALSE);
				break;
			case 0x1E:
				if ( this->DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
				{
					this->CGDurationMagicRecv(aRecv, aIndex);
				}
				break;
			case 0x22:
				if ( this->DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
				{
					this->CGItemGetRequest((PMSG_ITEMGETREQUEST *)aRecv, aIndex);
				}
				break;
			case 0x23:
				this->CGItemDropRequest((PMSG_ITEMTHROW *)aRecv, aIndex, 0);
				break;
			case 0x24:
				this->CGInventoryItemMove((PMSG_INVENTORYITEMMOVE *)aRecv, aIndex);
				break;
			case 0x26:
				this->CGUseItemRecv((PMSG_USEITEM *)aRecv, aIndex);
				break;
			case 0x30:
				if ( this->DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
				{
					this->CGTalkRequestRecv((PMSG_TALKREQUEST *)aRecv, aIndex);
				}
				break;
			case 0x31:
				this->CGCloseWindow(aIndex);
				break;
			case 0x32:
				if ( this->DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
				{
					this->CGBuyRequestRecv((PMSG_BUYREQUEST *)aRecv, aIndex);
				}
				break;
			case 0x33:
				if ( this->DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
				{
					CGSellRequestRecv((PMSG_SELLREQUEST *)aRecv, aIndex);
				}
				break;
			case 0x34:
				this->CGModifyRequestItem((PMSG_ITEMDURREPAIR *)aRecv, aIndex);
				break;
			case 0x36:
				if ( this->DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
				{
					this->CGTradeRequestSend((PMSG_TRADE_REQUEST *)aRecv, aIndex);
				}
				break;
			case 0x37:
				this->CGTradeResponseRecv((PMSG_TRADE_RESPONSE *)aRecv, aIndex);
				break;
			case 0x3A:
				this->CGTradeMoneyRecv((PMSG_TRADE_GOLD *)aRecv, aIndex);
				break;
			case 0x3C:
				if ( this->DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
				{
					this->CGTradeOkButtonRecv((PMSG_TRADE_OKBUTTON *)aRecv, aIndex);
				}
				break;
			case 0x3D:
				if ( this->DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
				{
					this->CGTradeCancelButtonRecv(aIndex);
				}
				break;
			case 0x3F:
				{
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;

					switch ( lpDef->subcode )
					{
						case 0x01:
							g_PersonalStore.CGPShopReqSetItemPrice((PMSG_REQ_PSHOP_SETITEMPRICE *)aRecv, aIndex);
							break;
						case 0x02:
							g_PersonalStore.CGPShopReqOpen((PMSG_REQ_PSHOP_OPEN *)aRecv, aIndex);
							break;
						case 0x03:
							g_PersonalStore.CGPShopReqClose(aIndex);
							break;
						case 0x05:
							g_PersonalStore.CGPShopReqBuyList((PMSG_REQ_BUYLIST_FROM_PSHOP *)aRecv, aIndex);
							break;
						case 0x06:
							g_PersonalStore.CGPShopReqBuyItem(aRecv, aIndex);
							break;
						case 0x07:
							g_PersonalStore.CGPShopReqCloseDeal((PMSG_REQ_PSHOPDEAL_CLOSE *)aRecv, aIndex);
							break;
					}
				}
				break;
			case 0x40:
				if ( this->DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
				{
					this->CGPartyRequestRecv((PMSG_PARTYREQUEST *)aRecv, aIndex);
				}
				break;
			case 0x41:
				if ( this->DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
				{
					this->CGPartyRequestResultRecv((PMSG_PARTYREQUESTRESULT *)aRecv, aIndex);
				}
				break;
			case 0x42:
				this->CGPartyList(aIndex);
				break;
			case 0x43:
				if ( this->PacketCheckTime(aIndex) == TRUE )
				{
					this->CGPartyDelUser((PMSG_PARTYDELUSER *)aRecv, aIndex, 1);
				}
				break;
			case 0x4A:
				this->CGRageAttack(aRecv, aIndex);
				break;
			case 0x4B:
				this->CGRageAttackRange((PMSG_RAGE_ATTACK_RANGE *)aRecv, aIndex);
				break;
			case 0x50:
				this->CGGuildRequestRecv((PMSG_GUILDJOINQ *)aRecv, aIndex);
				break;
			case 0x51:
				this->CGGuildRequestResultRecv((PMSG_GUILDQRESULT *)aRecv, aIndex);
				break;
			case 0x52:
				this->CGGuildListAll(aIndex);
				break;
			case 0x53:
				this->CGGuildDelUser((PMSG_GUILDDELUSER *)aRecv, aIndex);
				break;
			case 0x54:
				this->CGGuildMasterAnswerRecv((PMSG_GUILDMASTERANSWER *)aRecv, aIndex);
				break;
			case 0x55:
				this->CGGuildMasterInfoSave(aIndex, (PMSG_GUILDINFOSAVE *)aRecv);
				break;
			case 0x57:
				this->CGGuildMasterCreateCancel(aIndex);
				break;
			case 0x61:
				this->GCGuildWarRequestSendRecv((PMSG_GUILDWARSEND_RESULT *)aRecv, aIndex);
				break;
			case 0x66:
				this->GCGuildViewportInfo((PMSG_REQ_GUILDVIEWPORT *)aRecv, aIndex);
				break;
			case 0x6F:
				{
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;
					
					switch ( lpDef->subcode )
					{
						case 0x00:
							g_CancelItemSale.CGReqSoldItemList(aIndex);
							break;
						case 0x01:
							g_CancelItemSale.CGReqEndCancelItemSale(aIndex);
							break;
						case 0x02:
							g_CancelItemSale.CGReqReBuyItem((PMSG_REQ_REBUY_ITEM *)aRecv, aIndex);
							break;
					}
				}
				break;
			case 0xB1:
				{
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;
					
					switch ( lpDef->subcode )
					{
						case 0x01:
							if ( this->DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
							{
								this->CGReqMapSvrAuth((PMSG_REQ_MAPSERVERAUTH *)aRecv, aIndex);
							}
							break;
					}
				}
				break;
			case 0xB2:
				{
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;

					switch ( lpDef->subcode )
					{
						case 0x00:
							this->CGReqCastleSiegeState((PMSG_REQ_CASTLESIEGESTATE *)aRecv, aIndex);
							break;
						case 0x01:
							this->CGReqRegCastleSiege((PMSG_REQ_REGCASTLESIEGE *)aRecv, aIndex);
							break;
						case 0x02:
							this->CGReqGiveUpCastleSiege((PMSG_REQ_GIVEUPCASTLESIEGE *)aRecv, aIndex);
							break;
						case 0x03:
							this->CGReqGuildRegInfo((PMSG_REQ_GUILDREGINFO *)aRecv, aIndex);
							break;
						case 0x04:
							this->CGReqRegGuildMark((PMSG_REQ_REGGUILDMARK *)aRecv, aIndex);
							break;
						case 0x05:
							this->CGReqNpcBuy((PMSG_REQ_NPCBUY *)aRecv, aIndex);
							break;
						case 0x06:
							this->CGReqNpcRepair((PMSG_REQ_NPCREPAIR *)aRecv, aIndex);
							break;
						case 0x07:
							this->CGReqNpcUpgrade((PMSG_REQ_NPCUPGRADE *)aRecv, aIndex);
							break;
						case 0x08:
							this->CGReqTaxMoneyInfo((PMSG_REQ_TAXMONEYINFO *)aRecv, aIndex);
							break;
						case 0x09:
							this->CGReqTaxRateChange((PMSG_REQ_TAXRATECHANGE *)aRecv, aIndex);
							break;
						case 0x10:
							this->CGReqMoneyDrawOut((PMSG_REQ_MONEYDRAWOUT *)aRecv, aIndex);
							break;
						case 0x12:
							this->CGReqCsGateOperate((PMSG_REQ_CSGATEOPERATE *)aRecv, aIndex);
							break;
						case 0x1B:
							this->CGReqCsMiniMapData((PMSG_REQ_MINIMAPDATA *)aRecv, aIndex);
							break;
						case 0x1C:
							this->CGReqStopCsMiniMapData((PMSG_REQ_STOPMINIMAPDATA *)aRecv, aIndex);
							break;
						case 0x1D:
							this->CGReqCsSendCommand((PMSG_REQ_CSCOMMAND *)aRecv, aIndex);
							break;
						case 0x1F:
							this->CGReqCsSetEnterHuntZone((PMSG_REQ_CSHUNTZONEENTER *)aRecv, aIndex);
							break;
					}
				}
				break;
			case 0xB3:
				this->CGReqNpcDbList((PMSG_REQ_NPCDBLIST *)aRecv, aIndex);
				break;
			case 0xB4:
				this->CGReqCsRegGuildList((PMSG_REQ_CSREGGUILDLIST *)aRecv, aIndex);
				break;
			case 0xB5:
				this->CGReqCsAttkGuildList((PMSG_REQ_CSATTKGUILDLIST *)aRecv, aIndex);
				break;
			case 0xB7:
				{
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;

					switch ( lpDef->subcode )
					{
						case 0x01:
							this->CGReqWeaponUse((PMSG_REQ_USEWEAPON *)aRecv, aIndex);
							break;
						case 0x04:
							this->CGReqWeaponDamageValue((PMSG_REQ_WEAPON_DAMAGE_VALUE *)aRecv, aIndex);
							break;
					}
				}
				break;
			case 0xB9:
				{
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;

					switch ( lpDef->subcode )
					{
						case 0x02:
							this->CGReqGuildMarkOfCastleOwner((PMSG_REQ_GUILDMARK_OF_CASTLEOWNER *)aRecv, aIndex);
							break;
						case 0x05:
							CGReqCastleHuntZoneEntrance((PMSG_REQ_MOVE_TO_CASTLE_HUNTZONE *)aRecv, aIndex);
							break;
					}
				}
				break;
			case 0xBC:
				{
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;

					switch ( lpDef->subcode )
					{
						case 0x00:
							this->CGReqJewelMix((PMSG_REQ_JEWEL_MIX *)aRecv, aIndex);
							break;
						case 0x01:
							this->CGReqJewelUnMix((PMSG_REQ_JEWEL_UNMIX *)aRecv, aIndex);
							break;
					}
				}
				break;
			case 0xBD:
				{
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;

					switch ( lpDef->subcode )
					{
						case 0x00:
							this->CGReqCrywolfInfo((PMSG_REQ_CRYWOLF_INFO *)aRecv, aIndex);
							break;
						case 0x03:
							this->CGReqAlatrContract((PMSG_REQ_CRYWOLF_ALTAR_CONTRACT *)aRecv, aIndex);
							break;
						case 0x09:
							this->CGReqPlusChaosRate((PMSG_REQ_CRYWOLF_BENEFIT_PLUS_CHAOSRATE *)aRecv, aIndex);
							break;
					}
				}
				break;
			case 0xBF:
				{
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;

					switch( lpDef->subcode )
					{
						case 0x02:
							this->CGReqIllusionTempleUseSkill((PMSG_REQ_USEILLUSIONTEMPLESKILL *)aRecv, aIndex);
							break;
						case 0x05:
							this->GCIllusionTempleSendReward(aIndex);
							break;
						case 0x0B:
							this->CGReqLuckyCoinInfo(aIndex);
							break;
						case 0x0C:
							this->CGReqLuckyCoinRegister(reinterpret_cast<PMSG_REQ_LUCKYCOIN_REGISTER *>(aRecv), aIndex);
							break;
						case 0x0D:
							this->CGReqLuckyCoinTrade(reinterpret_cast<PMSG_REQ_LUCKYCOIN_TRADE *>(aRecv), aIndex);
							break;
						case 0x0E:
							this->CGReqEnterDoppelGanger(reinterpret_cast<PMSG_REQ_ENTER_DOPPELGANGER *>(aRecv), aIndex);
							break;
						case 0x17:
							{
								if(gObj[aIndex].MapNumber == MAP_INDEX_LORENMARKET)
								{
									gObjMoveGate(aIndex, 17);
								}
								else
								{
									gObjMoveGate(aIndex, 333);
								}
							}
							break;
						case 0x20:
							this->CGInventoryEquipment((_tagPMSG_REQ_INVENTORY_EQUIPMENT_ITEM *)aRecv, aIndex);
							break;
						case 0x51:
							this->CGReqMuBotUse(aIndex, reinterpret_cast<PMSG_MUBOT_SWITCH *>(aRecv));
							break;
						case 0x6A:
							this->CGReq_ITL_Relics((PMSG_REQ_ITL_RELATE_RELICS *)aRecv, aIndex);
							break;
						case 0x70:
							this->CG_Req_Enter_ITR((PMSG_REQ_ENTER_ITR *)aRecv, aIndex);
							break;
						case 0x71:
							this->CG_ReqAcceptEnterITR((PMSG_REQ_ACCEPTENTER_ITR *)aRecv, aIndex);
							break;
					}
				}
				break;
			case 0xCD:
				{
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;

					switch( lpDef->subcode )
					{
						case 0x01:
							this->CGReqUBFMyCharacterInfo((PMSG_REQ_UBF_INFO *)aRecv, aIndex);
							break;
						case 0x02:
							this->CGReqUBFJoin((PMSG_REQ_UBF_JOIN *)aRecv, aIndex);
							break;
						case 0x06:
							this->CGReqUBFGetReward((PMSG_REQ_GET_REWARD *)aRecv, aIndex);
							break;
						case 0x07:
							this->CGReqUBFCancel((PMSG_REQ_UBF_CANCEL *)aRecv, aIndex);
							break;
						case 0x08:
							this->CGReqUBFGetRealName((PMSG_REQ_UBF_REAL_NAME *)aRecv, aIndex);
							break;
					}
				}
				break;			
			case 0xE1:
				this->CGGuildAssignStatus((PMSG_GUILD_ASSIGN_STATUS_REQ *)aRecv, aIndex);
				break;
			case 0xE2:
				this->CGGuildAssignType((PMSG_GUILD_ASSIGN_TYPE_REQ *)aRecv, aIndex);
				break;
			case 0xE5:
				CGRelationShipReqJoinBreakOff((PMSG_RELATIONSHIP_JOIN_BREAKOFF_REQ *)aRecv, aIndex);
				break;
			case 0xE6:
				this->CGRelationShipAnsJoinBreakOff((PMSG_RELATIONSHIP_JOIN_BREAKOFF_ANS *)aRecv, aIndex);
				break;
			case 0xE7:
				{
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;

					switch (lpDef->subcode)
					{
						case 0x01:
							this->CGReqSendMemberPosInfoStart(aIndex);
							break;
						case 0x02:
							this->CGReqSendMemberPosInfoStop(aIndex);
							break;
						case 0x03:
							this->GCReqSendNpcPosInfo((PMSG_REQ_NPC_POSITION *)aRecv, aIndex);
							break;
					}
				}
				break;
			case 0xE9:
				this->CGUnionList((PMSG_UNIONLIST_REQ *)aRecv, aIndex);
				break;
			case 0xEB:
				{
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;

					switch ( lpDef->subcode )
					{
						case 0x01:
							this->CGRelationShipReqKickOutUnionMember((PMSG_KICKOUT_UNIONMEMBER_REQ *)aRecv, aIndex);
							break;
					}
				}
				break;
			case 0x71:
				this->GCPingSendRecv((PMSG_PING_RESULT *)aRecv, aIndex);
				break;
			case 0x72:
				if ( this->DataEncryptCheck(aIndex, protoNum, Encrypt) != FALSE )
				{
					this->GCPacketCheckSumRecv((PMSG_PACKETCHECKSUM *)aRecv, aIndex);
				}
				break;
			case 0x81:
				this->CGWarehouseMoneyInOut(aIndex, (PMSG_WAREHOUSEMONEYINOUT *)aRecv);
				break;
			case 0x82:
				this->CGWarehouseUseEnd(aIndex);
				break;
			case 0x83:
				this->GCWarehouseRecivePassword(aIndex, (PMSG_WAREHOUSEPASSSEND *)aRecv);
				break;
			case 0x86:
				this->CGChaosBoxItemMixButtonClick((PMSG_CHAOSMIX *)aRecv, aIndex);
				break;
			case 0x87:
				this->CGChaosBoxUseEnd(aIndex);
				break;
			case 0x88:
				g_MixSystem.CheckEmptySpace_MultiMix((PMSG_REQ_CHAOS_MULTIMIX_CHECK *)aRecv, aIndex);
				break;
			case 0x90:
				this->GCReqmoveDevilSquare((PMSG_REQ_MOVEDEVILSQUARE *)aRecv, aIndex);
				break;
			case 0x91:
				this->GCReqDevilSquareRemainTime((PMSG_REQ_DEVILSQUARE_REMAINTIME *)aRecv, aIndex);
				break;
			case 0x95:
				this->GCRegEventChipRecv((PMSG_REGEVENTCHIP *)aRecv, aIndex);
				break;
			case 0x96:
				this->GCGetMutoNumRecv((PMSG_GETMUTONUMBER *)aRecv, aIndex);
				break;
			case 0x97:
				this->GCUseEndEventChipRescv(aIndex);
				break;
			case 0x98:
				this->GCUseRenaChangeZenRecv((PMSG_EXCHANGE_EVENTCHIP *)aRecv, aIndex);
				break;
			case 0x99:
				this->CGReqMoveOtherServer((PMSG_REQ_MOVE_OTHERSERVER *)aRecv, aIndex);
				break;
			case 0xA0:
				this->CGRequestQuestInfo(aIndex);
				break;
			case 0xA2:
				this->CGSetQuestState((PMSG_SETQUEST *)aRecv, aIndex);
				break;
			case 0xA7:
				this->CGRequestPetItemCommand((PMSG_REQUEST_PET_ITEM_COMMAND *)aRecv, aIndex);
				break;
			case 0xA9:
				this->CGRequestPetItemInfo((PMSG_REQUEST_PET_ITEMINFO *)aRecv, aIndex);
				break;
			case 0xAA : 
				{
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;

					WORD wPacketType = MAKEWORD(lpDef->subcode,0xAA);
					switch( wPacketType )
					{
						case MSG_DUEL_INVITE :
							{
								LPPMSG_REQ_DUEL_INVITE lpReq = (LPPMSG_REQ_DUEL_INVITE)aRecv;
								int nRet = OnCGInviteDuel(lpReq, aIndex);
								if( nRet != ENEWPVP::E_NO_ERROR )
								{
									PMSG_ANS_DUEL_INVITE res = {0};
									res.h.c = 0xC1;
									res.h.size = sizeof(PMSG_ANS_DUEL_INVITE);
									res.h.headcode = 0xAA;
									res.h.subcode = 0x01;
									res.nResult = nRet;
									IOCP.DataSend(aIndex, (LPBYTE)&res, res.h.size);
								}
							}
							break;

						case MSG_DUEL_ANSWER :
							{
								LPPMSG_ANS_DUEL_ANSWER lpReq = (LPPMSG_ANS_DUEL_ANSWER)aRecv;
								int nRet = OnCGAnswerDuel(lpReq, aIndex);
							}
							break;

						case MSG_DUEL_LEAVE :
							{
								LPPMSG_REQ_DUEL_EXIT lpReq = (LPPMSG_REQ_DUEL_EXIT)aRecv;
								int nRet = OnCGLeaveDuel(lpReq, aIndex);
								if( nRet != ENEWPVP::E_NO_ERROR )
								{
									PMSG_ANS_DUEL_EXIT res = {0};
									res.h.c = 0xC1;
									res.h.size = sizeof(PMSG_ANS_DUEL_EXIT);
									res.h.headcode = 0xAA;
									res.h.subcode = 0x03;
									res.nResult = nRet;
									IOCP.DataSend(aIndex, (LPBYTE)&res, res.h.size);
								}
							}
							break;

						case MSG_DUEL_JOINCNANNEL :
							{
								int nRet = OnDuelChannelJoin((LPPMSG_REQ_DUEL_JOINCNANNEL)aRecv, aIndex);
								if( nRet != ENEWPVP::E_NO_ERROR )
								{
									PMSG_ANS_DUEL_JOINCNANNEL res = {0};
									res.h.c = 0xC1;
									res.h.size = sizeof(PMSG_ANS_DUEL_JOINCNANNEL);
									res.h.headcode = 0xAA;
									res.h.subcode = 0x07;
									res.nResult = nRet;
									IOCP.DataSend(aIndex, (LPBYTE)&res, res.h.size);
								}
							}
							break;

						case MSG_DUEL_LEAVECNANNEL :
							{
								LPPMSG_REQ_DUEL_LEAVECNANNEL lpReq = (LPPMSG_REQ_DUEL_LEAVECNANNEL)aRecv;
								int nRet = OnDuelChannelLeave(lpReq, aIndex);
								if( nRet != ENEWPVP::E_NO_ERROR )
								{
									PMSG_ANS_DUEL_LEAVECNANNEL res = {0};
									res.h.c = 0xC1;
									res.h.size = sizeof(PMSG_ANS_DUEL_LEAVECNANNEL);
									res.h.headcode = 0xAA;
									res.h.subcode = 0x09;
									res.nResult = nRet;
									IOCP.DataSend(aIndex, (LPBYTE)&res, res.h.size);
								}
							}
							break;	
						}
				}
				break;
			case 0x9A:
				this->CGRequestEnterBloodCastle((PMSG_REQ_MOVEBLOODCASTLE*)aRecv, aIndex);
				break;
			case 0x9F:
				this->CGRequestEventEnterCount((PMSG_REQ_CL_ENTERCOUNT *)aRecv, aIndex);
				break;
			case 0x9D:
				this->CGRequestLottoRegister((PMSG_REQ_2ANV_LOTTO_EVENT *)aRecv, aIndex);
				break;
			case 0xAE:
				this->CGReqMuBotSaveData(aIndex, reinterpret_cast<PMSG_MUBOT_DATASAVE *>(aRecv));
				break;
			case 0xAF:
				{
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;

					switch ( lpDef->subcode )
					{
						case 0x01:
							this->CGRequestEnterChaosCastle((PMSG_REQ_MOVECHAOSCASTLE *)aRecv, aIndex);
							break;
						case 0x02:
							this->CGRequestRepositionUserInChaosCastle((PMSG_REQ_REPOSUSER_IN_CC *)aRecv, aIndex);
							break;
						case 3:
							CGReqCCF_DayTime(aIndex);
							break;
						case 5:
							CGReqCCF_EnterCheck(aIndex);
							break;
						case 6:
							CGRequestRepositionUserInCCF((PMSG_REQ_REPOSUSER_IN_CCF *)aRecv, aIndex);
							break;
						case 7:
							CGReq_CCF_Ranking((PMSG_REQ_CCF_RANKING *)aRecv, aIndex);
							break;
						case 8:
							CG_CCF_UI_OnOff((PMSG_CCF_UI_ONOFF *)aRecv, aIndex);
							break;
					}
				}
				break;
			case 0xC0:
				FriendListRequest(aIndex);
				break;
			case 0xC1:
				FriendAddRequest((PMSG_FRIEND_ADD_REQ *)aRecv, aIndex);
				break;
			case 0xC2:
				WaitFriendAddRequest((PMSG_FRIEND_ADD_SIN_RESULT *)aRecv, aIndex);
				break;
			case 0xC3:
				FriendDelRequest((PMSG_FRIEND_DEL_REQ *)aRecv, aIndex);
				break;
			case 0xC4:
				FriendStateClientRecv((PMSG_FRIEND_STATE_C *)aRecv, aIndex);
				break;
			case 0xC5:
				FriendMemoSend((PMSG_FRIEND_MEMO *)aRecv, aIndex);
				break;
			case 0xC7:
				FriendMemoReadReq((PMSG_FRIEND_READ_MEMO_REQ *)aRecv, aIndex);
				break;
			case 0xC8:
				FriendMemoDelReq((PMSG_FRIEND_MEMO_DEL_REQ *)aRecv, aIndex);
				break;
			case 0xC9:
				FriendMemoListReq(aIndex);
				break;
			case 0xCA:
				FriendChatRoomCreateReq((PMSG_FRIEND_ROOMCREATE_REQ *)aRecv, aIndex);
				break;
			case 0xCB:
				FriendRoomInvitationReq((PMSG_ROOM_INVITATION *)aRecv, aIndex);
				break;
			case 0xD0:
				{			
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;

					switch ( lpDef->subcode )
					{
						case 0x07: //WereWolf
							this->WereWolfQuarrelCheck(aIndex);
							break;
						case 0x08: //GateKeeper
							this->GateKeeperCheck(aIndex);
							break;
						case 0x0A: // Exit from Santa Village
							if (gGateC.CheckGateLevel(aIndex, 22) == true)
							{
								gObjMoveGate(aIndex, 22);
							}
							else
							{
								char szTemp[256];
								sprintf(szTemp, Lang.GetText(0,211), gGateC.GetLevel(22));

								this->GCServerMsgStringSend(szTemp, aIndex, 1);
							}
							break;
						case 0x10: // Santa Clause
							EGReqSantaGift(aIndex);
							break;
					}
				}
				break;
			case 0xD1:
				{
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;

					switch ( lpDef->subcode )
					{
						case 0x00:
							this->CGReqKanturuStateInfo((PMSG_REQ_KANTURU_STATE_INFO *)aRecv, aIndex);
							break;
						case 0x01:
							this->GCReqEnterKanturuBossMap((PMSG_REQ_ENTER_KANTURU_BOSS_MAP *)aRecv, aIndex);
							break;
					}
				}
				break;
				
			case 0xD2:
				{
					
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;

					switch ( lpDef->subcode )
					{
						case 0x01:
							g_CashShop.CGCashPoint(lpObj);
							break;
						case 0x02:
							g_CashShop.CGCashShopOpen(lpObj, (PMSG_REQ_INGAMESHOPOPEN *)aRecv);
							break;
						case 0x03:
							g_CashShop.CGCashItemBuy(lpObj, (PMSG_REQ_INGAMESHOP_ITEMBUY *)aRecv);
							break;
						case 0x04:
							g_CashShop.CGCashItemGift(lpObj, (PMSG_REQ_CASHITEM_GIFT *)aRecv);
							break;
						case 0x05:
							g_CashShop.CGCashInventoryItemCount(lpObj, (PMSG_REQ_INGAMESHOP_INVENTORY *)aRecv);
							break;
						case 0x0B:
							g_CashShop.CGCashInventoryItemUse(lpObj, (PMSG_REQ_INGAMESHOP_ITEMUSE *)aRecv);
							break;
						default:
							g_Log.AddC(TColor::Red,  "InGameShop Unknown Packet: %X %X", lpDef->h.headcode, lpDef->subcode);
					}
				}
				break;

			case 0xEC:
				{
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;

					switch ( lpDef->subcode )
					{

						case 0:
							this->CGReqInJewelPentagramItem((PMSG_REQ_IN_PENTAGRAM_JEWEL *)aRecv, aIndex);
							break;

						case 1:
							this->CGReqOutJewelPentagramItem((PMSG_REQ_OUT_PENTAGRAM_JEWEL *)aRecv, aIndex);
							break;

						case 2:
							this->CGReqRefinePentagramJewel((PMSG_REQ_REFINE_PENTAGRAM_JEWEL *)aRecv, aIndex);
							break;

						case 3:
							this->CGReqUpgradePentagramJewel((PMSG_REQ_UPGRADE_PENTAGRAM_JEWEL *)aRecv, aIndex);
							break;

						case 0x31:
							g_PersonalStore.CGReqSearchItemInPShop((PMSG_REQ_SEARCH_ITEM_PSHOP *)aRecv, aIndex);
							break;

						case 0x33:
							g_PersonalStore.CGReqPShopLog((PMSG_REQ_PSHOP_LOG *)aRecv, aIndex);
							break;

						default:
							g_Log.AddC(TColor::Red,  "%X %X %X %X %X %X %X %X %X %X %X %X", aRecv[0], aRecv[1], aRecv[2], aRecv[3], aRecv[4], aRecv[5], aRecv[6], aRecv[7], aRecv[8], aRecv[9], aRecv[10], aRecv[11]);
							break;
					}
				}

				break;

			case 0xDB:
				{
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;

					switch ( lpDef->subcode )
					{
						case 0x00:
							this->CGReqDSFSchedule(aIndex);
							break;
						case 0x01:
							GDReqDSFCanPartyEnter(aIndex);
							break;
						case 0x02:
							this->CGReqAcceptEnterDSF((PMSG_REQ_DSF_ACCEPTENTER *)aRecv, aIndex);
							break;
						case 0x03:
							this->CGReqDSFGoFinalParty((PMSG_REQ_DSF_GO_FINAL_PARTY_INFO *)aRecv, aIndex);
							break;
						case 0x09:
							this->CGReqDSFGetReward((PMSG_REQ_DSF_GET_REWARD *)aRecv, aIndex);
							break;
					}
				}
				break;

			case 0xED: // Guild Matching
				{
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;

					switch (lpDef->subcode)
					{
						case 0:
							this->CGReqGuildMatchingList((PMSG_REQ_GUILDMATCHINGLIST *)aRecv, aIndex);
							break;
						case 1:
							this->CGReqGuildMatchingListSearchWord((PMSG_REQ_GUILDMATCHINGLISTSEARCHWORD *)aRecv, aIndex);
							break;
						case 2:
							this->CGReqRegGuildMatchingList((PMSG_REQ_REGGUILDMATCHINGDATA *)aRecv, aIndex);
							break;
						case 3:
							this->CGReqCancelGuildMatchingList((PMSG_REQ_CANCEL_GUILDMATCHINGLIST *)aRecv, aIndex);
							break;
						case 4:
							this->CGReqJoinGuildMatching((PMSG_REQ_JOIN_GUILDMATCHING *)aRecv, aIndex);
							break;
						case 5:
							this->CGReqCancelJoinGuildMatching(aIndex);
							break;
						case 6:
							this->CGReqAllowJoinGuildMatching((PMSG_REQ_ALLOW_JOIN_GUILDMATCHING *)aRecv, aIndex);
							break;
						case 7:
							this->CGReqGetAllowListJoinGuildMatching((PMSG_REQ_ALLOWLIST_GUILDMATCHING *)aRecv, aIndex);
							break;
						case 8:
							this->CGReqGetWaitStateListGuildMatching(aIndex);
							break;
						default:
							return;
					}
				}
				break;

			case 0xEF: // Party Matching
				{
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;

					switch (lpDef->subcode)
					{
						case 0:
							this->CGReqRegWantedPartyMember((PMSG_REQ_REG_WANTED_PARTYMEMBER *)aRecv, aIndex);
							break;
						case 1:
							this->CGReqGetPartyMatchingList((PMSG_REQ_GET_PARTYMATCHINGLIST *)aRecv, aIndex);
							break;
						case 2:
							this->CGReqJoinMemberPartyMatching((PMSG_REQ_MEMBERJOIN_PARTYMATCHINGLIST *)aRecv, aIndex);
							break;
						case 3:
							this->CGReqMemberJoinStateList((PMSG_REQ_MEMBERJOIN_STATELIST_PARTYMATCHING *)aRecv, aIndex);
							break;
						case 4:
							this->CGReqMemberJoinStateListLeader((PMSG_REQ_WAITLIST_PARTYMATCHING *)aRecv, aIndex);
							break;
						case 5:
							this->CGReqAcceptJoinMemberPartyMatching((PMSG_REQ_ACCEPTMEMBER_PARTYMATCHING *)aRecv, aIndex);
							break;
						case 6:
							this->CGReqCancelPartyMatching((PMSG_REQ_CANCEL_JOIN_PARTYMATCHING *)aRecv, aIndex);
							break;
						default:
							return;
					}
				}
				break;

			case 0xF6:
				{
					switch (aRecv[3])
					{
						case 0x30:
							CGReqQuestExp((PMSG_REQ_NPC_QUESTEXP *)aRecv, aIndex);
							break;

						case 0x31:
							CGReqAttDefPowerInc((PMSG_REQ_ATTDEF_POWER_INC *)aRecv, aIndex);
							break;

						case 0x21:
							CGReqEventItemQuestList((PMSG_REQ_EVENT_ITEM_EP_LIST *)aRecv, aIndex);
							break;

						case 0x0A:
							CGReqQuestSwitch((PMSG_REQ_QUESTEXP *)aRecv, aIndex);
							break;

						case 0x0B:
							CGReqQuestProgress((PMSG_REQ_QUESTEXP *)aRecv, aIndex);
							break;

						case 0x0D:
							CGReqQuestComplete((PMSG_REQ_QUESTEXP_COMPLETE *)aRecv, aIndex);
							break;

						case 0x0F:
							CGReqQuestGiveUp((PMSG_REQ_QUESTEXP_GIVEUP *)aRecv, aIndex);
							break;

						case 0x1A:
							CGReqProgressQuestList((PMSG_REQ_QUESTEXP_PROGRESS_LIST *)aRecv, aIndex);
							break;

						case 0x1B:
							CGReqProgressQuestInfo((PMSG_REQ_QUESTEXP_PROGRESS_INFO *)aRecv, aIndex);
							break;

						case 0x10:
							CGReqTutorialKeyComplete((PMSG_REQ_QUESTEXP_ASK_COMPLETE *)aRecv, aIndex);
							break;

						default:
							g_Log.AddC(TColor::Red, "UNKNOWN QUEST PROTOCOL (%x/%x/%x)", aRecv[2], aRecv[3], aRecv[4]);
							break;
					}
				}
				break;
			case 0xF7:
				{
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;

					switch ( lpDef->subcode )
					{
						case 0x01:
							this->CGReqEnterZone((PMSG_REQ_ENTER_ZONE *)aRecv, aIndex);
							break;
					}
				}
				break;
			case 0xF8:
				{
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;

					switch (lpDef->subcode)
					{
					case 0x01:
						this->CGReqRegGensMember((_tagPMSG_REQ_REG_GENS_MEMBER *)aRecv, aIndex);
						break;
					case 0x03:
						this->CGReqSecedeGensMember((_tagPMSG_REQ_SEGEDE_GENS_MEMBER *)aRecv, aIndex);
						break;
					case 0x09:
						this->CGReqGensReward((_tagPMSG_GENS_REWARD_CODE *)aRecv, aIndex);
						break;
					case 0x0B:
						this->CGReqGensMemberInfo((_tagPMSG_REQ_GENS_INFO *)aRecv, aIndex);
						break;
					case 0x20:
						this->CGReqAcheronEnter((_tagPMSG_REQ_ACHERON_ENTER *)aRecv, aIndex);
						break;
					case 0x30:
						this->CGReqArcaBattleGuildMasterJoin((_tagPMSG_REQ_ARCA_BATTLE_JOIN *)aRecv, aIndex);
						break;
					case 0x32:
						this->CGReqArcaBattleGuildMemberJoin((_tagPMSG_REQ_ARCA_BATTLE_JOIN *)aRecv, aIndex);
						break;
					case 0x34:
						this->CGReqArcaBattleEnter((_tagPMSG_REQ_ARCA_BATTLE_ENTER *)aRecv, aIndex);
						break;
					case 0x36:
						this->CGReqArcaBattleBootyExchange(aIndex);
						break;
					case 0x3C:
						this->CGReqSpritemapExchange(aIndex);
						break;
					case 0x41:
						this->CGReqRegisteredMemberCnt(aIndex);
						break;
					case 0x43:
						g_ArcaBattle.CGReqMarkReg(aIndex);
						break;
					case 0x45:
						g_ArcaBattle.CGReqMarkRank(aIndex);
						break;
					case 0x4B:
						g_AcheronGuardianEvent.CGReqAcheronEventEnter((_tagPMSG_REQ_ACHERON_EVENT_ENTER *)aRecv, aIndex);
						break;
					default:
						g_Log.AddC(TColor::Blue, "UNKNOWN F8 PACKET: %x %x %x %x %x %x %x %x %x %x", aRecv[0], aRecv[1], aRecv[2], aRecv[3], aRecv[4], aRecv[5], aRecv[6], aRecv[7], aRecv[8], aRecv[9]);
						break;
					}
				}
				break;
			case 0xFA://why wpe dont catch it lol ehh
				{
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;

					switch (lpDef->subcode)
					{
					case 0x08:
					{
						this->CGReqAntiCheatRecv(aIndex, (PMSG_SEND_AH_INFO *)aRecv);
					}
						break;
					case 0x09:
					{
						gObjUseSkill.SkillChangeUse(lpObj->m_Index);
						DGGuildMemberInfoRequest(aIndex);
					}
						break;
					case 0x0A:
					{
						this->CGReqAntihackBreach(aIndex, (PMSG_ANTIHACK_BREACH *)aRecv);
					}
						break;
					case 0x0D:
					{
						this->CGReqFileCrcRecv(aIndex, (PMSG_REQ_CRCDATA *)aRecv);
					}
						break;
					case 0x11:
						this->CGReqAntihackCheck(aIndex, (PMSG_ANTIHACK_CHECK *)aRecv);
						break;
					case 0x15:
						this->CGRecvHitHackValues(aIndex,(PMSG_SEND_HITHACK_INFO*)aRecv);
						break;
					}
				}
				break;

			case 0x4C:
				{
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;

					switch(lpDef->subcode)
					{
						case 0x00:
							g_MineSystem.MineTwinkle((PMSG_REQ_MINETWINKLE *)aRecv, aIndex);
							break;
						case 0x01:
							g_MineSystem.RewardMineTwinkle((PMSG_REQ_MINETWINKLE_REWARD *)aRecv, aIndex);
							break;
						case 0x03:
							g_MineSystem.FailMineTwinkle((PMSG_ANS_MINETWINKLE_END_ANIMATION *)aRecv, aIndex, false);
							break;
					}
				}
				break;

			case 0x4D:
				{
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;

					switch (lpDef->subcode)
					{
						case 0x00:
							if (DataEncryptCheck(aIndex, protoNum, Encrypt))
							{
								this->CGEventItemGetRequest((PMSG_EVENTITEM_GETREQUEST *)aRecv, aIndex);
							}
							break;
						case 0x01: 
							this->CGEventItemDropRequest((PMSG_EVENTITEM_THROW *)aRecv, aIndex);
							break;
						case 0x0F:
							this->CGReqEventInvenOpen(aRecv, aIndex);
							break;
						case 0x10:
							g_CMuRummyMng.CGReqMuRummyStart((_tagPMSG_REQ_MURUMMY_INFO *)aRecv, aIndex);
							break;
						case 0x11:
							g_CMuRummyMng.CGReqCardReveal((_tagPMSG_REQ_REVEAL_CARD *)aRecv, aIndex);
							break;
						case 0x12:
							g_CMuRummyMng.CGReqCardMove((_tagPMSG_REQ_CARD_MOVE *)aRecv, aIndex);
							break;
						case 0x13:
							g_CMuRummyMng.CGReqCardReMove((_tagPMSG_REQ_CARD_REMOVE *)aRecv, aIndex);
							break;
						case 0x14:
							g_CMuRummyMng.CGReqCardMatch((_tagPMSG_REQ_CARD_MATCH *)aRecv, aIndex);
							break;
						case 0x15:
							g_CMuRummyMng.CGReqMuRummyEnd((_tagPMSG_REQ_MURUMMY_END *)aRecv, aIndex);
							break;
					}
				}
				break;
			case 0x4E:
				{		
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;

					switch (lpDef->subcode)
					{
						case 0x00:
							if (DataEncryptCheck(aIndex, protoNum, Encrypt))
							{
								this->CGMuunItemGetRequest((PMSG_MUUNITEM_GETREQUEST *)aRecv, aIndex);
							}
							break;
						case 0x08:
							g_CMuunSystem.CGMuunInventoryUseItemRecv((PMSG_USEITEM_MUUN_INVEN *)aRecv, aIndex);
							break;
						case 0x09:
							if (DataEncryptCheck(aIndex, protoNum, Encrypt))
							{
								CGMuunItemSellRequestRecv((PMSG_MUUN_ITEM_SELLREQUEST *)aRecv, aIndex);
							}
							break;	
						case 0x11:
							g_CMuunSystem.CGReqRideSelect((PMSG_MUUN_RIDE_SELECT*)aRecv,aIndex);
							break;
						case 0x13:
							g_CMuunSystem.CGMuunExchangeItem((PMSG_REQ_MUUN_EXCHANGE *)aRecv, aIndex);
							break;
					}
				}
				break;
			case 0x4F:
				{
					PMSG_DEFAULT2 * lpDef = (PMSG_DEFAULT2 *)aRecv;

					switch (lpDef->subcode)
					{
						case 0x02:
							g_GremoryCase.CGReqGetItemFromGremoryCase((PMSG_ADD_GREMORYCASE_ITEM_TO_INVENTORY_REQ *)aRecv, aIndex);
							break;
						case 0x05:
							g_GremoryCase.CGReqOpenGremoryCase(aIndex);
							break;
					}
				}
				break;
			case 0xFB:
				break;
			case 0xFC:
				break;
			default:
				g_Log.AddC(TColor::Red,  "error-L2 (ProtocolCore) : Account: %s Name: %s : Invalid packet received OP: %X (%s,%d) Connection State: %d",
 					gObj[aIndex].AccountID, gObj[aIndex].Name, protoNum, __FILE__, __LINE__, gObj[aIndex].Connected);
				this->GCSendDisableReconnect(aIndex);
				//IOCP.CloseClient(aIndex);
		}	
	}
}

void GameProtocol::MsgSendV2(LPOBJ lpObj, unsigned char* Msg, int size)
{
	for ( int n=0;n<MAX_VIEWPORT;n++)
	{
		if ( lpObj->VpPlayer2[n].type == OBJ_USER )
		{
			if ( lpObj->VpPlayer2[n].state != 0 )
			{
				IOCP.DataSend(lpObj->VpPlayer2[n].number, Msg, size);
			}
		}
	}
}

void GameProtocol::CGLiveClient(PMSG_CLIENTTIME * lpClientTime, short aIndex)
{

	if ( gObj[aIndex].Connected > PLAYER_CONNECTED )
	{
		gObj[aIndex].ConnectCheckTime = GetTickCount();
	}
		
	if (strcmpi(lpClientTime->Version, DLL_VERSION) != 0)
	{
		g_Log.AddC(TColor::Red, "[IP: %s][AccountID: %s] using invalid version of DLL, review and correct Server/Client configuration. Required version of DLL for actual Game Server is %s", gObj[aIndex].m_PlayerData->Ip_addr, gObj[aIndex].AccountID, DLL_VERSION);

		if (g_ConfigRead.antihack.isInvalidDLLVersionDC)
		{
			//this->GCSendDisableReconnect(aIndex);
			this->GCServerMsgStringSendEx(aIndex, 1, Lang.GetText(0,547));
			gObjCloseSet(aIndex, 0);
			return;
		}
	}

	if ( gObj[aIndex].CheckSpeedHack != false )
	{
		DWORD dwTime = MAKE_NUMBERDW(lpClientTime->TimeH, lpClientTime->TimeL);

		int systemtick = GetTickCount() - gObj[aIndex].CheckTick2;
		int usertick = dwTime - gObj[aIndex].CheckTick;
		int checktime = systemtick - usertick;

		gObj[aIndex].CheckTickCount++;

		if ( gObj[aIndex].CheckTickCount > 45 )
		{
			gObj[aIndex].CheckTick = dwTime;
			gObj[aIndex].CheckTickCount = 0;
		}
	}

	if ( gObj[aIndex].Type != OBJ_USER )
	{
		return;
	}

	if ( gObj[aIndex].Connected == PLAYER_PLAYING )
	{
		if ( gObj[aIndex].m_AttackSpeed < lpClientTime->AttackSpeed || gObj[aIndex].m_MagicSpeed < lpClientTime->MagicSpeed )
		{
			g_Log.AddC(TColor::Red, "[%s][%s][%s] ClientHack Detected : Editing AttackSpeed [%d][%d] [%d][%d]", gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_PlayerData->Ip_addr, gObj[aIndex].m_AttackSpeed, lpClientTime->AttackSpeed, gObj[aIndex].m_MagicSpeed, lpClientTime->MagicSpeed);	
			AntiHackLog->Output("[%s][%s][%s] ClientHack Detected : Editing AttackSpeed [%d][%d] [%d][%d]", gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_PlayerData->Ip_addr, gObj[aIndex].m_AttackSpeed, lpClientTime->AttackSpeed, gObj[aIndex].m_MagicSpeed, lpClientTime->MagicSpeed);	

			gObj[aIndex].m_AttackSpeedHackDetectedCount++;

			if ( gObj[aIndex].m_AttackSpeedHackDetectedCount >= 3)
			{		
				this->GCSendDisableReconnect(aIndex);
				//IOCP.CloseClient(aIndex);
				return;
			}
		}

		else
		{
			gObj[aIndex].m_AttackSpeedHackDetectedCount = 0;
		}
	}

	if (g_ConfigRead.antihack.EnabledAgiCheck == false)
	{
		return;
	}

	EnterCriticalSection(&gObj[aIndex].m_PlayerData->AgiCheckCriti);

	if ( GetTickCount() - gObj[aIndex].m_PlayerData->AgilityCheckDelay < g_ConfigRead.antihack.AgilityDelayCheckTime ) // agility can be checked x seconds after add agility / use bless etc
	{
		LeaveCriticalSection(&gObj[aIndex].m_PlayerData->AgiCheckCriti);
		return;
	}

	if ( gObj[aIndex].Connected == PLAYER_PLAYING )
	{
		if ( lpClientTime->Agility > (gObj[aIndex].m_PlayerData->Dexterity+gObj[aIndex].AddDexterity) )
		{
			gObj[aIndex].m_PlayerData->AgilityCheckCount++;
			g_Log.AddC(TColor::Red, "[ANTI-HACK] [%s][%s][%s] Agility check FAILED - Server:%d Client:%d (Count:%d/3)", gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_PlayerData->Ip_addr, gObj[aIndex].m_PlayerData->Dexterity + gObj[aIndex].AddDexterity, lpClientTime->Agility, gObj[aIndex].m_PlayerData->AgilityCheckCount);
			AntiHackLog->Output("[ANTI-HACK] [%s][%s][%s] Agility check FAILED - Server:%d Client:%d (Count:%d/3)", gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_PlayerData->Ip_addr, gObj[aIndex].m_PlayerData->Dexterity + gObj[aIndex].AddDexterity, lpClientTime->Agility, gObj[aIndex].m_PlayerData->AgilityCheckCount);
		
			if (gObj[aIndex].m_PlayerData->AgilityCheckCount >= 3)
			{
				this->GCSendDisableReconnect(aIndex);
				//IOCP.CloseClient(aIndex);
			}
		}

		else
		{
			gObj[aIndex].m_PlayerData->AgilityCheckCount = 0;
		}
	}

	else
	{
		gObj[aIndex].m_PlayerData->AgilityCheckCount = 0;
	}

	LeaveCriticalSection(&gObj[aIndex].m_PlayerData->AgiCheckCriti);
}


struct PMSG_CHECK_MAINEXE
{
	PBMSG_HEAD h;	// C1:03
	WORD m_wKey;	// 4
};


void GameProtocol::GCCheckMainExeKeySend(int aIndex)
{
	PMSG_CHECK_MAINEXE pMsg;

	gObj[aIndex].CheckSumTableNum = rand() % MAX_CHECKSUM_KEY;
	gObj[aIndex].CheckSumTime = GetTickCount();
	PHeadSetB((LPBYTE)&pMsg, 0x03, sizeof(pMsg));
	pMsg.m_wKey = EncryptCheckSumKey(gObj[aIndex].CheckSumTableNum);

	//IOCP.DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
}





WORD GameProtocol::EncryptCheckSumKey(WORD wSource)
{
	WORD wRandom = rand() % 64;
	WORD wAcc = ((wSource & 0x3F0) * 64) | (wRandom *16) | (wSource & 0x0F);
	return wAcc ^ 0xB479;
}




void GameProtocol::CGCheckMainRecv(PMSG_CHECK_MAINEXE_RESULT * lpMsg, int aIndex)
{
	if(gObj[aIndex].Type != OBJ_USER)
	{
		return;
	}

	if ( !gCheckSumSwitch )
	{
		gObj[aIndex].CheckSumTime = 0;
		return;
	}

	if ( gObj[aIndex].CheckSumTableNum < 0 || gObj[aIndex].CheckSumTableNum > MAX_CHECKSUM_KEY-1 )
	{
		g_Log.Add("error-L1 : [%s][%s] CheckSum-Exe error", gObj[aIndex].AccountID, gObj[aIndex].Name);
		this->GCSendDisableReconnect(aIndex);
		//IOCP.CloseClient(aIndex);
		return;
	}

	if ( dwgCheckSum[gObj[aIndex].CheckSumTableNum] != lpMsg->m_dwKey)
	{
		g_Log.Add("error-L1 : CheckSum-Exe error %d %d %d [%s]", dwgCheckSum[gObj[aIndex].CheckSumTableNum],
			lpMsg->m_dwKey, gObj[aIndex].CheckSumTableNum, gObj[aIndex].AccountID);
		this->GCSendDisableReconnect(aIndex);
		//IOCP.CloseClient(aIndex);
		return;
	}

	gObj[aIndex].CheckSumTime = 0;
}





void GameProtocol::PEchoProc(unsigned char * aMsg, int aLen, short aIndex)
{
	for ( int n = 0 ; n< g_ConfigRead.server.GetObjectMax() ; n++)
	{
		if ( gObj[n].Connected >= PLAYER_CONNECTED )
		{
			IOCP.DataSend(n, aMsg, aLen);
		}
	}
}







void GameProtocol::GCResultSend(int aIndex, BYTE headcode, BYTE result)
{
	PMSG_DEFRESULT pResult;

	PHeadSetB((LPBYTE)&pResult, headcode, sizeof(pResult));
	pResult.result = result;
	
	IOCP.DataSend(aIndex, (UCHAR*)&pResult, pResult.h.size);
}



void GameProtocol::ChatSend(LPOBJ lpObj, char* szChat)
{
	int len = strlen(szChat);

	if ( len < 1 )
		return;

	if ( len > MAX_CHAT_LEN-1 )
		return;

	PMSG_CHATDATA_NUMBER pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x01, len+6);
	strcpy(pMsg.chatmsg, szChat);
	pMsg.NumberH = SET_NUMBERH(lpObj->m_Index);
	pMsg.NumberL = SET_NUMBERL(lpObj->m_Index);

	this->MsgSendV2(lpObj, (UCHAR*)&pMsg, pMsg.h.size);
}



void GameProtocol::vChatSend(LPOBJ lpObj, char* szChat, ...)
{
	char szBuffer[256] = "";
	va_list pArguments;

	va_start(pArguments, szChat);
	vsprintf(szBuffer, szChat, pArguments);
	va_end(pArguments);

	int len = strlen(szBuffer);

	if ( len < 1 )
		return;

	if ( len > MAX_CHAT_LEN-1 )
		return;

/*	char szTempMsg[4096] = {0};
	MultiByteToWideChar(1258, 0, szChat, len, (unsigned short*)szTempMsg, sizeof(szTempMsg));
	memcpy(&pMsg.szChatMsg, szTempMsg, sizeof(pMsg.szChatMsg));
	pMsg.szChatMsg[MAX_CHAT_LEN-2]=0;
	pMsg.szChatMsg[MAX_CHAT_LEN-1]=0;
	PHeadSetB((LPBYTE)&pMsg, 0x01, wcslen((unsigned short*)pMsg.szChatMsg)*2 + (sizeof(GC_CHATSEND)-sizeof(pMsg.szChatMsg)));
	pMsg.sIndex[0] = lpObj->m_Index >> 8;
	pMsg.sIndex[1] = lpObj->m_Index & 0xFF;*/

	PMSG_CHATDATA_NUMBER pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x01, len+6);
	strcpy(pMsg.chatmsg, szBuffer);
	pMsg.NumberH = SET_NUMBERH(lpObj->m_Index);
	pMsg.NumberL = SET_NUMBERL(lpObj->m_Index);

	this->MsgSendV2(lpObj, (UCHAR*)&pMsg, pMsg.h.size);
}





void GameProtocol::AllSendServerMsg( char* chatmsg)
{
	PMSG_NOTICE pNotice;
	
	TNotice::MakeNoticeMsg((TNotice*)&pNotice, 0x00,  chatmsg);

	for ( int n = g_ConfigRead.server.GetObjectStartUserIndex() ; n < g_ConfigRead.server.GetObjectMax() ; n++)
	{
		if ( gObj[n].Connected == PLAYER_PLAYING )
		{
			if ( gObj[n].Type == OBJ_USER )
			{
				IOCP.DataSend(n, (unsigned char*)&pNotice , pNotice.h.size );
			}
		}
	}
}



void GameProtocol::DataSendAll(unsigned char* lpMsg, int iMsgSize)
{			
	for ( int n = g_ConfigRead.server.GetObjectStartUserIndex() ; n < g_ConfigRead.server.GetObjectMax() ; n++)
	{
		if ( gObj[n].Connected == PLAYER_PLAYING )
		{
			if ( gObj[n].Type == OBJ_USER )
			{
				IOCP.DataSend(n, (unsigned char*)lpMsg , iMsgSize );
			}
		}
	}
}




void GameProtocol::ChatTargetSend(LPOBJ lpObj, char * szChat, int senduser)
{
	int len = strlen(szChat);

	if ( len < 1 || len > MAX_CHAT_LEN-1)
		return;

	PMSG_CHATDATA_NUMBER pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x01, len+6);
	strcpy(pMsg.chatmsg, szChat);
	pMsg.NumberH = SET_NUMBERH(lpObj->m_Index);
	pMsg.NumberL = SET_NUMBERL(lpObj->m_Index);
	pMsg.chatmsg[len+1] = 0;

	IOCP.DataSend(senduser, (UCHAR*)&pMsg, pMsg.h.size);
}



struct CHAT_LOG_DATA
{
	PBMSG_HEAD h;	// C1:02
	WORD wServer;	// 4
	char AccountID[11];	// 6
	char Name[11];	// 11
	BYTE btType;	// 1C
	char szChatMsg[90];	// 1D
	char szTargetName[5][11];	// 5A
};

void GameProtocol::PChatProc(PMSG_CHATDATA * lpChat, short aIndex)
{
	if(!ObjectMaxRange(aIndex))
		return;

	int n;
	LPOBJ lpObj = &gObj[aIndex];
	int number;
	int slen = strlen(lpChat->chatmsg);

	if(lpObj->Type != OBJ_USER)
	{
		return;
	}

	if ( slen < 1 )
		return;

	if ( slen > MAX_CHAT_LEN-1 )
	{
		g_Log.Add("[Anti-HACK][PChatProc][%s][%s] Chat Message Len : %d", lpObj->AccountID, lpObj->Name, slen);
		return;
	}

	if(GetTickCount() -  lpObj->m_ChatFloodTime < 1000)
	{
		lpObj->m_ChatFloodCount++;

		if(lpObj->m_ChatFloodCount > 6)
		{
			g_Log.AddC(TColor::Red, "lpObj->m_ChatFloodCOunt > 6");
			this->GCSendDisableReconnect(aIndex);
			//IOCP.CloseClient(aIndex);
			return;
		}
	}

	else
	{
		lpObj->m_ChatFloodCount = 0;
	}

	lpObj->m_ChatFloodTime = GetTickCount();

	char szId[MAX_ACCOUNT_LEN+1];
	szId[MAX_ACCOUNT_LEN] = 0;
	CHAT_LOG_DATA pChatMsg;

	if (g_ConfigRead.server.GetServerType() != SERVER_BATTLECORE)
	{
		memcpy(szId, gObj[aIndex].Name, MAX_ACCOUNT_LEN);
	}

	else
	{
		memcpy(szId, gObj[aIndex].m_PlayerData->m_RealNameOfUBF, MAX_ACCOUNT_LEN);
	}

	if (g_ConfigRead.server.GetServerType() == SERVER_BATTLECORE)
	{
		char szBuff[100];
		memset(szBuff, 0x00, sizeof(szBuff));

		if (lpChat->chatmsg[0] != '!' &&
			lpChat->chatmsg[0] != '`' &&
			lpChat->chatmsg[0] != '@' &&
			lpChat->chatmsg[0] != '$' &&
			lpChat->chatmsg[0] != '~')
		{
			if (lpChat->chatmsg[0] == '/')
			{
				sprintf(szBuff, "%s", lpChat->chatmsg);
			}

			else
			{
				sprintf(szBuff, "[#%d]%s", gObj[aIndex].m_PlayerData->m_nServerCodeOfHomeWorld, lpChat->chatmsg);
			}
		}

		else
		{
			sprintf(szBuff, "%c[#%d]%s", lpChat->chatmsg[0], gObj[aIndex].m_PlayerData->m_nServerCodeOfHomeWorld, lpChat->chatmsg);
		}

		int nSize = strlen(szBuff) - slen;
		lpChat->h.size += nSize;
		memcpy(lpChat->chatmsg, szBuff, lpChat->h.size);
	}

	memcpy(lpChat->chatid, szId, MAX_ACCOUNT_LEN);
	int szTargetNameCount = 0;

	if ( gWriteChatLog )
	{
		pChatMsg.h.c = 0xC1;
		pChatMsg.h.headcode = 0x02;
		memcpy(pChatMsg.AccountID, gObj[aIndex].AccountID, MAX_ACCOUNT_LEN);
		memcpy(pChatMsg.Name, gObj[aIndex].Name, MAX_ACCOUNT_LEN);
		pChatMsg.AccountID[MAX_ACCOUNT_LEN] = 0;
		pChatMsg.Name[MAX_ACCOUNT_LEN] = 0;
		pChatMsg.wServer = g_ConfigRead.server.GetGameServerCode();
		pChatMsg.btType = 0xFF;
	}

	MsgLog->Output("[%s][%s]: %s",gObj[aIndex].AccountID,gObj[aIndex].Name,lpChat->chatmsg);

	if ( (lpObj->Penalty&2)==2)
		return;

	if ( lpObj->m_PlayerData->m_bSecurityCheck == false )
	{
		char seps[2] = " ";
		char string[256];
		char * szCmdToken;
		strcpy(string, lpChat->chatmsg);
		szCmdToken = strtok(string, seps);

		if ( lpChat->chatmsg[0] == '/' && cManager.GetCmd(szCmdToken) == 431 )
		{

		}

		else
		{
			return;
		}
	}
	
	switch ( lpChat->chatmsg[0] )
	{
		case '!':	// Global Announcement
			if ( slen > 2 )
			{
				if ( (lpObj->AuthorityCode&1)==1)
				{
					if(lpChat->chatmsg[1] == '!')
					{
						char Msg[128];
						sprintf(Msg, "[%s][%s]: %s", g_ConfigRead.server.GetServerName(), lpObj->Name, &lpChat->chatmsg[2]);
						GS_GDReqMapSvrMsgMultiCast(g_MapServerManager.GetMapSvrGroup(), Msg);

						g_Log.Add("Announcement: %s %s %s", gObj[aIndex].AccountID, gObj[aIndex].Name, &lpChat->chatmsg[2]);

						if ( gWriteChatLog )
						{
							memcpy(pChatMsg.szChatMsg, &lpChat->chatmsg[2], MAX_CHAT_LEN-1);
							pChatMsg.szChatMsg[MAX_CHAT_LEN] = 0;
							pChatMsg.btType = 0x03;
						}
						return;
					}
					else
					{

						PMSG_NOTICE pNotice;
						TNotice::MakeNoticeMsgEx(&pNotice, 0, "%s : %s", lpObj->Name, &lpChat->chatmsg[1]);
						TNotice::SendNoticeToAllUser(&pNotice);

						g_Log.Add("Announcement: %s %s %s", gObj[aIndex].AccountID, gObj[aIndex].Name, &lpChat->chatmsg[1]);

						if ( gWriteChatLog )
						{
							memcpy(pChatMsg.szChatMsg, &lpChat->chatmsg[1], MAX_CHAT_LEN-1);
							pChatMsg.szChatMsg[MAX_CHAT_LEN] = 0;
							pChatMsg.btType = 0x03;
						}
						return;
					}
				}
			}
			break;
		case '/':	// Command
			if ( slen > 2 )
			{
				cManager.ManagementProc(lpObj, lpChat->chatmsg, aIndex);
				return;
			}
			break;
	}

	// Other Commands
	if ( lpObj->ChatLimitTime > 0 )
	{
		MsgOutput(lpObj->m_Index, Lang.GetText(0,139), lpObj->ChatLimitTime);
		return;
	}
	
	// Party Message
	if ( lpChat->chatmsg[0] == '~' || lpChat->chatmsg[0] == ']' )
	{
		if (lpObj->m_PlayerData->m_bUsePartyMatching == false || lpObj->m_PlayerData->m_nPartyMatchingIndex < 0)
		{
			if (lpObj->PartyNumber >= 0)
			{
				int partycount = gParty.GetPartyCount(lpObj->PartyNumber);

				if (partycount >= 0)
				{
					if (gWriteChatLog)
					{
						memcpy(pChatMsg.szChatMsg, &lpChat->chatmsg[1], MAX_CHAT_LEN - 1);
						pChatMsg.szChatMsg[MAX_CHAT_LEN] = 0;
						pChatMsg.btType = 0x01;
					}

					for (n = 0; n < MAX_USER_IN_PARTY; n++)
					{
						number = gParty.m_PartyS[lpObj->PartyNumber].Number[n];

						if (number >= 0)
						{
							IOCP.DataSend(number, (LPBYTE)lpChat, lpChat->h.size);

							if (gWriteChatLog)
							{
								strcpy(pChatMsg.szTargetName[szTargetNameCount], gObj[number].Name);
								szTargetNameCount++;
							}
						}
					}
				}
			}
		}

		else
		{
			if (gWriteChatLog)
			{
				memcpy(pChatMsg.szChatMsg, &lpChat->chatmsg[1], MAX_CHAT_LEN - 1);
				pChatMsg.szChatMsg[MAX_CHAT_LEN] = 0;
				pChatMsg.btType = 0x01;
			}

			GDSendChatMsgPartyMatching(lpChat->chatmsg, lpObj->Name, lpObj->m_PlayerData->m_nPartyMatchingIndex);
		}
	}
	
	else if ( lpChat->chatmsg[0] == '$' )
	{
		if ( lpObj->m_PlayerData->m_GensInfluence == DUPRIAN_INFLUENCE || lpObj->m_PlayerData->m_GensInfluence == VANERT_INFLUENCE )
		{
			for (n= g_ConfigRead.server.GetObjectStartUserIndex();n<g_ConfigRead.server.GetObjectMax();n++ )
			{
				if ( gObjIsConnected(n) && gObj[n].m_PlayerData->m_GensInfluence == lpObj->m_PlayerData->m_GensInfluence )
				{
					IOCP.DataSend(n, (LPBYTE)lpChat, lpChat->h.size);
				}
			}

			GDGensServerGroupChattingSend(lpObj->m_PlayerData->m_GensInfluence, lpChat);
		}
	}
	
	// Guild
	else if ( lpChat->chatmsg[0] == '@' )
	{
		if ( lpObj->m_PlayerData->GuildNumber > 0 )
		{
			// Notice
			if ( lpChat->chatmsg[1] == '>' )
			{
				if ( lpObj->Name[0] == lpObj->m_PlayerData->lpGuild->Names[0][0] )
				{
					if ( !strcmp(lpObj->Name, lpObj->m_PlayerData->lpGuild->Names[0] ) )
					{
						for(int i=0;i<strlen(lpChat->chatmsg);i++)
						{
							if(lpChat->chatmsg[i] == '%')
							{
								MsgOutput(aIndex,Lang.GetText(0,514));
								return;
							}
						}
						GDGuildNoticeSave(lpObj->m_PlayerData->lpGuild->Name, &lpChat->chatmsg[2]);
						g_Log.Add("Guild announcement %s %s", lpObj->m_PlayerData->lpGuild->Name, lpChat->chatmsg);

						if ( gWriteChatLog )
						{
							memcpy(pChatMsg.szChatMsg, &lpChat->chatmsg[2], MAX_CHAT_LEN-2);
							pChatMsg.szChatMsg[MAX_CHAT_LEN] = 0;
							pChatMsg.btType = 0x04;
						}
					}
				}
			}
			// Aliances
			else if ( lpChat->chatmsg[1] == '@' && lpObj->m_PlayerData->lpGuild->iGuildUnion )
			{
				int iGuildCount = 0;
				int iGuildList[MAX_UNION_GUILD] ={0};

				if ( UnionManager.GetGuildUnionMemberList(lpObj->m_PlayerData->lpGuild->iGuildUnion, iGuildCount, iGuildList) == TRUE )
				{
					for ( int i=0;i<iGuildCount;i++)
					{
						_GUILD_INFO_STRUCT *lpGuildInfo = Guild.SearchGuild_Number(iGuildList[i]);

						if ( !lpGuildInfo )
							continue;

						for (n=0;n<MAX_USER_GUILD;n++)
						{
							if ( lpGuildInfo->Use[n] )
							{
								number = lpGuildInfo->Index[n];

								if ( number >= 0 )
								{
									if ( lpGuildInfo->Names[n][0] == gObj[number].Name[0] )
									{
										if ( !strcmp(lpGuildInfo->Names[n], gObj[number].Name ))
										{
											IOCP.DataSend(number, (LPBYTE)lpChat, lpChat->h.size);
										}
									}
								}
							}
						}
					}
				}

				if ( g_iServerGroupUnionChatting == TRUE )
					GDUnionServerGroupChattingSend(lpObj->m_PlayerData->lpGuild->iGuildUnion, lpChat);
			}
			// Just Guild
			else
			{
				int count = lpObj->m_PlayerData->lpGuild->Count;

				if ( count >= 0 )
				{
					for ( n=0;n<MAX_USER_GUILD;n++)
					{
						if ( lpObj->m_PlayerData->lpGuild->Use[n] )
							{
								number = lpObj->m_PlayerData->lpGuild->Index[n];

								if ( number >= 0 )
								{
									if ( lpObj->m_PlayerData->lpGuild->Names[n][0] == gObj[number].Name[0] )
									{
										if ( !strcmp(lpObj->m_PlayerData->lpGuild->Names[n], gObj[number].Name ) )
										{
											IOCP.DataSend(number, (LPBYTE)lpChat, lpChat->h.size);
										}
									}
								}
							}
						}
				}

				if ( g_iServerGroupGuildChatting == TRUE )
				{
					if ( lpObj->m_PlayerData->lpGuild->Count > 1 )
					{
						GDGuildServerGroupChattingSend(lpObj->m_PlayerData->lpGuild->Number, lpChat);
					}
				}

				if ( gWriteChatLog )
				{
					memcpy(pChatMsg.szChatMsg, &lpChat->chatmsg[1], MAX_CHAT_LEN-1);
					pChatMsg.szChatMsg[MAX_CHAT_LEN] = 0;
					pChatMsg.btType = 0x02;
				}
			}

			if ( gWriteChatLog )
			{
				strcpy(pChatMsg.szTargetName[szTargetNameCount], lpObj->m_PlayerData->GuildName);
				szTargetNameCount++;
			}
		}
	}
	else
	{	
		if ( ( gObj[aIndex].Authority & 0x20 ) == 0x20 && gObjCheckUsedBuffEffect(&gObj[aIndex], BUFFTYPE_INVISABLE) == TRUE )
		{
			return;
		}

		if(!g_prohibitedSymbols.Validate(lpChat->chatmsg,strlen(lpChat->chatmsg),TYPE_CHAT))
		{
			MsgOutput(aIndex,Lang.GetText(0,545));
			return;
		}

		if(SwearFilter.CompareText(lpChat->chatmsg) == true && lpChat->chatmsg[0] != '@' && lpChat->chatmsg[0] != '~')
		{
			++lpObj->m_PlayerData->SwearWarning;

			if(lpObj->m_PlayerData->SwearWarning >= 3)
			{
				MsgOutput(aIndex, Lang.GetText(0,515));
				this->GCSendDisableReconnect(aIndex);
				//IOCP.CloseClient(aIndex);
			}
			else
			{
				MsgOutput(aIndex, Lang.GetText(0,516), lpObj->m_PlayerData->SwearWarning);
			}
			return;
		}

		

		if ( g_NewPVP.IsDuel(*lpObj) || g_NewPVP.IsObserver(*lpObj) )
		{
			g_NewPVP.ChatMsgSend(*lpObj, (LPBYTE)lpChat, lpChat->h.size);
		}
		
		else
		{
			IOCP.DataSend(aIndex, (LPBYTE)lpChat, lpChat->h.size);

			if ( g_GensSystem.IsMapBattleZone(lpObj->MapNumber) )
			{
				g_GensSystem.BattleZoneChatMsgSend(lpObj, (LPBYTE)lpChat, lpChat->h.size);
			}

			else
			{
				this->MsgSendV2(lpObj, (LPBYTE)lpChat, lpChat->h.size);
			}
		}

		if ( gWriteChatLog )
		{
			memcpy(pChatMsg.szChatMsg, lpChat->chatmsg, MAX_CHAT_LEN);
			pChatMsg.szChatMsg[MAX_CHAT_LEN] = 0;
			pChatMsg.btType = 0x00;

		}
	}

	if ( gWriteChatLog )
	{
		if ( pChatMsg.btType != 0xFF )
		{
			pChatMsg.btType |= szTargetNameCount<<4;
			pChatMsg.h.size = sizeof(pChatMsg) - (5 - szTargetNameCount) - 1;
		}
	}
}							
			

void GameProtocol::CGChatRecv(PMSG_CHATDATA_NUMBER * lpMsg, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if ( PacketCheckTime(aIndex) == FALSE )
	{
		return;
	}

	IOCP.DataSend(aIndex, (LPBYTE)lpMsg, lpMsg->h.size);
	MsgSendV2(lpObj, (LPBYTE)lpMsg, lpMsg->h.size);
}



struct PMSG_SERVERMSG
{
	PBMSG_HEAD h;	// C1:0C
	BYTE MsgNumber;	// 3
};

void GameProtocol::GCServerMsgSend(BYTE msg, int aIndex)
{
	PMSG_SERVERMSG pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x0C, sizeof(pMsg));
	pMsg.MsgNumber = msg;
	IOCP.DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
}




void GameProtocol::GCServerMsgStringSend(LPSTR  szMsg, int aIndex, BYTE type) 
{
	PMSG_NOTICE pNotice;
	
	TNotice::MakeNoticeMsg((TNotice*)&pNotice, type, szMsg);
	IOCP.DataSend(aIndex, (UCHAR*)&pNotice, pNotice.h.size);
}

void GameProtocol::GCServerMsgStringSendAll(LPSTR  szMsg, BYTE type) 
{
	PMSG_NOTICE pNotice;
	
	TNotice::MakeNoticeMsg((TNotice*)&pNotice, type, szMsg);
	this->DataSendAll((UCHAR*)&pNotice, pNotice.h.size);
}

void GameProtocol::GCServerMsgStringSendAllEx(BYTE type, LPSTR szMsg, ...)
{
	va_list pArguments;
	char szBuffer[512];

	va_start(pArguments, szMsg);
	vsprintf(szBuffer, szMsg, pArguments);

	this->GCServerMsgStringSendAll(szBuffer, type);
}

void GameProtocol::GCServerMsgStringSendEx(int aIndex, BYTE type, LPSTR szMsg, ...)
{
	va_list pArguments;
	char szBuffer[512];

	va_start(pArguments, szMsg);
	vsprintf(szBuffer, szMsg, pArguments);

	this->GCServerMsgStringSend(szBuffer, aIndex, type);
}


void GameProtocol::GCServerMsgStringSendGuild(_GUILD_INFO_STRUCT* lpNode, LPSTR szMsg, BYTE type)
{
	if ( lpNode == NULL )
	{
		return;
	}

	for ( int n = 0; n<MAX_USER_GUILD ; n++)
	{
		if (lpNode->Use[n] > 0 && lpNode->Index[n] >= 0 )
		{
			GCServerMsgStringSend((char*)szMsg, lpNode->Index[n], type);
		}
	}
}



struct PMSG_EVENT
{
	PBMSG_HEAD h;	// C1:0B
	BYTE State;	// 3
	BYTE Event;	// 4
};

void GameProtocol::GCEventStateSend(int aIndex, BYTE state, BYTE event)
{
	PMSG_EVENT pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x0B, sizeof(pMsg));
	pMsg.Event = event;
	pMsg.State = state;

	IOCP.DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
}



void GameProtocol::GCMapEventStateSend(int map, BYTE state, BYTE event)
{
	PMSG_EVENT pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x0B, sizeof(pMsg));
	pMsg.Event = event;
	pMsg.State = state;

	for ( int n=0; n<g_ConfigRead.server.GetObjectMax() ; n++ )
	{
		if ( gObj[n].Connected == PLAYER_PLAYING && gObj[n].Type == OBJ_USER && map == gObj[n].MapNumber )
		{
			IOCP.DataSend(n, (UCHAR*)&pMsg, pMsg.h.size);
		}
	}
}



void GameProtocol::CGChatWhisperRecv(PMSG_CHATDATA_WHISPER* lpMsg, int aIndex)
{
	char tid[11];
	PMSG_CHATDATA_WHISPER pWhisper;
	int len;
	LPOBJ lpObj = &gObj[aIndex];
	int index;

	if ( PacketCheckTime(aIndex) == FALSE )
	{
		return;
	}

	if ( lpObj->ChatLimitTime > 0 )
	{
		MsgOutput(lpObj->m_Index, Lang.GetText(0,139), lpObj->ChatLimitTime);
		return;
	}

	if ( (lpObj->Penalty & 2) == 2 )
	{
		return;
	}

	if(g_ConfigRead.data.common.SavePrivateChat == true)
	{
		MsgLog->Output(Lang.GetText(0,517), gObj[aIndex].AccountID, lpMsg->id, lpMsg->chatmsg);
	}

	tid[10]=0;
	memcpy(tid, lpMsg->id, sizeof(lpMsg->id));
	index = WhisperCash.AddCash(tid);

	if ( index < 0 )
	{
		if(g_ConfigRead.data.common.joinmuMultiChannelWhisper == true)
		{
			//ifjoinmu
			gObj[aIndex].m_Whispering.iWhisperSent = true;
			gObj[aIndex].m_Whispering.wResponseCount = 0;	
			PMSG_RECV_CHATDATA_WHISPER pDSMsg;
			pDSMsg.h.set((LPBYTE)&pDSMsg,0xC2,sizeof(pDSMsg));
			memcpy(pDSMsg.fromId,gObj[aIndex].Name,sizeof(pDSMsg.fromId));
			pDSMsg.wMapSvrNum = g_MapServerManager.GetMapSvrGroup();
			memcpy(pDSMsg.id,lpMsg->id,sizeof(lpMsg->id));
			memcpy(pDSMsg.chatmsg,lpMsg->chatmsg,sizeof(lpMsg->chatmsg));
			pDSMsg.OriginPlayerIndex = aIndex;
			wsDataCli.DataSend((char *)&pDSMsg, pDSMsg.h.size);
			return;
		}
		else
		{
			GCServerMsgSend(0, aIndex);
			return;
		}
	}

	PHeadSetB((LPBYTE)&pWhisper, 0x02, sizeof(pWhisper));
	strcpy(pWhisper.id, gObj[aIndex].Name);
	memcpy(pWhisper.chatmsg, lpMsg->chatmsg, sizeof(lpMsg->chatmsg));

	//pWhisper.szChatMsg[MAX_CHAT_LEN-2] = 0;
	//pWhisper.chatmsg[MAX_CHAT_LEN-1] = 0;
	len = strlen(pWhisper.chatmsg);

	if (gObj[index].m_PlayerData->bActiveSetParty == true)
	{
		if (len >= 4)
		{
			if (!strcmp(pWhisper.chatmsg, gObj[index].m_PlayerData->PartyPassword))
			{
				gObjAutoPartySet(&gObj[index], &gObj[aIndex]);
			}
		}
	}
	if ( len > 0 )
	{
		pWhisper.h.size -= sizeof(pWhisper.chatmsg);
		pWhisper.h.size += len + 1;

		if ( g_GensSystem.IsMapBattleZone(lpObj->MapNumber) )
		{
			int OwnInfluence = g_GensSystem.GetGensInfluence(lpObj);
			int TarInfluence = g_GensSystem.GetGensInfluence(&gObj[index]);

			if ( OwnInfluence == TarInfluence || CheckAuthorityCondition(98, lpObj) || CheckAuthorityCondition(98, &gObj[index]) )
			{
				IOCP.DataSend(index, (UCHAR*)&pWhisper, pWhisper.h.size);
			}
		}

		else
		{
			IOCP.DataSend(index, (UCHAR*)&pWhisper, pWhisper.h.size);
		}
	}
}



#pragma pack (1)
struct PMSG_JOINRESULT
{
	PBMSG_HEAD h;	// C1:F1
	BYTE scode;	// 3
	BYTE result;	// 4
	BYTE NumberH;	// 5
	BYTE NumberL;	// 6
	BYTE CliVersion[5];	// 7
};
#pragma pack ()

void GameProtocol::SCPJoinResultSend(int aIndex, BYTE result)
{
	PMSG_JOINRESULT pResult;

	memset(&pResult, 0, sizeof(pResult));

	pResult.h.size = sizeof(pResult);
	pResult.h.c = 0xC1;
	pResult.h.headcode = 0xF1;
	pResult.scode = 0x00;
	pResult.result = result;
	pResult.NumberH = SET_NUMBERH(aIndex);
	pResult.NumberL = SET_NUMBERL(aIndex);
	pResult.CliVersion[0] = szClientVersion[0];
	pResult.CliVersion[1] = szClientVersion[1];
	pResult.CliVersion[2] = szClientVersion[2];
	pResult.CliVersion[3] = szClientVersion[3];
	pResult.CliVersion[4] = szClientVersion[4];

	IOCP.DataSend(aIndex, (unsigned char*)&pResult, pResult.h.size);
	gObj[aIndex].ConnectCheckTime = GetTickCount();
}




void GameProtocol::CSPJoinIdPassRequest(PMSG_IDPASS* lpMsg, int aIndex)
{
	if(aIndex < g_ConfigRead.server.GetObjectStartUserIndex() || aIndex > g_ConfigRead.server.GetObjectMax())
	{
		return;
	}

	char serial[17];
	char id[11];
	char hwid[25];

	if ( lpMsg->CliVersion[0] != szClientVersion[0] ||
		 lpMsg->CliVersion[1] != szClientVersion[1] ||
		 lpMsg->CliVersion[2] != szClientVersion[2] ||
		 lpMsg->CliVersion[3] != szClientVersion[3] ||
		 lpMsg->CliVersion[4] != szClientVersion[4] )
	{
		GCJoinResult(JS_BAD_CLIENT_VERSION, aIndex);
		IOCP.CloseClient(aIndex);
		return;
	}

	

	id[10]=0;
	memcpy(id, lpMsg->Id, sizeof(lpMsg->Id));
	BuxConvert(id, MAX_ACCOUNT_LEN);
	ProcessClientHWID(lpMsg->HWID, hwid);

	if (PercentSyntaxCheck(id) == FALSE)
	{
		g_Log.AddC(TColor::Red, "[%d] Possible Crash attempt IP: %s ", aIndex, gObj[aIndex].m_PlayerData->Ip_addr);
		this->GCJoinResult(JS_BAD_CLIENT_VERSION, aIndex);
		IOCP.CloseClient(aIndex);
		return;
	}
	
	serial[16] = 0;
	memcpy(serial, lpMsg->CliSerial, sizeof(lpMsg->CliSerial));
	
	if ( strcmp(serial, szGameServerExeSerial) != 0 )
	{
		g_Log.AddC(TColor::Red,  "error-L1: Serial error [%s] [%s]", id, serial);
		this->GCJoinResult(JS_BAD_CLIENT_VERSION, aIndex);
		IOCP.CloseClient(aIndex);
		return;
	}

	if (lpMsg->HWID == NULL)
	{
		g_Log.AddC(TColor::Red, "[LOGIN ERROR] HWID is EMPTY, [Index]: %d [IP]: %s", aIndex, gObj[aIndex].m_PlayerData->Ip_addr);
		IOCP.CloseClient(aIndex);
		return;
	}

	if ( g_ConfigRead.server.IsConnectMemberUse() == TRUE )
	{
		if ( ConMember.IsMember(id) == FALSE )
		{
			this->GCJoinResult(JS_ONLY_VIP_MEMBERS, aIndex);
			return;
		}
	}

	if ( g_ConfigRead.server.IsTestServer() == TRUE )
	{
		if ( ConMember.IsMember(id) == FALSE )
		{
			this->GCJoinResult(2, aIndex);
			this->GCServerMsgStringSend(Lang.GetText(0,518), aIndex, 0);
			return;
		}
	}

	LPOBJ lpObj = &gObj[aIndex];
	
	if (PacketCheckTime(aIndex) == FALSE )
	{
		g_Log.Add("error-L2 : Login time is over [%d][%s]", aIndex, id);
		IOCP.CloseClient(aIndex);
		return;
	}

	if ( gObj[aIndex].Connected != PLAYER_CONNECTED )
	{
		g_Log.Add("error-L2: Authentication is allowed after connected [%d][%s]", aIndex, id);
		IOCP.CloseClient(aIndex);
		return;
	}

	if ( gObj[aIndex].LoginMsgSnd != FALSE )
	{
		if ( gObj[aIndex].LoginMsgSnd == TRUE )
		{
			g_Log.Add("error: It is after authentication information is sended [%d][%s][%s]", aIndex, id, gObj[aIndex].m_PlayerData->Ip_addr );
		}
		else
		{
			g_Log.Add("error: %d %s %d", gObj[aIndex].LoginMsgSnd, __FILE__, __LINE__);
		}

		return;
	}

	SDHP_IDPASS spMsg;

	PHeadSetB((LPBYTE)&spMsg, 0x01, sizeof(spMsg));
	spMsg.Number = aIndex;
	memcpy(spMsg.Id, lpMsg->Id, sizeof(spMsg.Id));
	memcpy(spMsg.Pass, lpMsg->Pass, sizeof(spMsg.Pass));
	memcpy(spMsg.HWID, hwid, sizeof(hwid));
	strcpy(spMsg.IpAddress, gObj[aIndex].m_PlayerData->Ip_addr);
	gObj[aIndex].CheckTick = lpMsg->TickCount;
	gObj[aIndex].CheckTick2 = GetTickCount();
	gObj[aIndex].ConnectCheckTime = GetTickCount();
	gObj[aIndex].CheckSpeedHack = true;
	gObj[aIndex].ConnectCheckTime = GetTickCount();
	gObj[aIndex].AgiCheckTime = GetTickCount();
	gObj[aIndex].LoginMsgSnd = 1;
	gObj[aIndex].LoginMsgCount++;
	gObj[aIndex].AccountID[MAX_ACCOUNT_LEN] = 0;
	memcpy(gObj[aIndex].AccountID, id, MAX_ACCOUNT_LEN);
	memcpy(gObj[aIndex].m_PlayerData->HWID, hwid, sizeof(hwid));
	gObj[aIndex].m_cAccountItemBlock = 0;
	gObj[aIndex].m_PlayerData->VipType = 0;
	gObj[aIndex].m_PlayerData->VipEffect = VIP_EFFECT_NONE;
	gObj[aIndex].m_PlayerData->AntiHackCheckTime = GetTickCount();
	gObj[aIndex].m_PlayerData->dwLastHitHackTick = GetTickCount64();
	wsJServerCli.DataSend((char*)&spMsg, spMsg.h.size);
	g_Log.Add("Attempt to Login : (%d)%s (MachineID:%s)", aIndex, gObj[aIndex].AccountID, lpMsg->HWID);
	lpObj->m_bMapSvrMoveReq = false;
	lpObj->m_bMapSvrMoveReq_2 = false;
	lpObj->m_sPrevMapSvrCode = -1; 
	lpObj->m_sDestMapNumber = -1;
	lpObj->m_btDestX = 0;
	lpObj->m_btDestY = 0;
}

struct SDHP_BILLSEARCH
{
	PBMSG_HEAD h;	// C1:06
	char Id[10];	// 3
	short Number;	// E
};

void GameProtocol::GCJoinBillCheckSend(LPSTR AccountId, int aIndex)
{
	SDHP_BILLSEARCH pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x06, sizeof(pMsg));
	memcpy(pMsg.Id, AccountId, sizeof(pMsg.Id));
	pMsg.Number = aIndex;

	wsJServerCli.DataSend((char*)&pMsg, pMsg.h.size);
}




void GameProtocol::GCJoinResult(BYTE result, int aIndex)
{
	PMSG_RESULT  pResult;

	PHeadSubSetB((LPBYTE)&pResult, 0xF1, 0x01, sizeof(pResult));
	pResult.result = result;
	IOCP.DataSend(aIndex, (UCHAR*)&pResult, pResult.h.size);
}



void GameProtocol::GCJoinSocketResult(BYTE result, SOCKET Socket)
{
	PMSG_RESULT pResult;

	PHeadSubSetB((LPBYTE)&pResult , 0xF1, 0x01, sizeof(pResult) );
	pResult.result= result;

	wsGServer.DataSocketSend(Socket, (char*)&pResult, pResult.h.size);
}




void GameProtocol::CGClientCloseMsg(PMSG_CLIENTCLOSE * lpMsg, int aIndex)
{
	if ( aIndex < 0 || aIndex > g_ConfigRead.server.GetObjectMax()-1)
	{
		return;
	}

	switch ( lpMsg->Flag )
	{
		case 0:	// Close Game
			gObj[aIndex].m_bOff = 0;
			gObjCloseSet(aIndex, 0);

			g_Log.Add("[UserSelClose] Go Exit [%s][%s] Map:[%d][%d/%d]",
				gObj[aIndex].AccountID, gObj[aIndex].Name,
				gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y);
			break;

		case 2:	// ServerList
			gObj[aIndex].m_bOff = 0;
			gObjCloseSet(aIndex, 2);

			g_Log.Add("[UserSelClose] Go ServerList [%s][%s] Map:[%d][%d/%d]",
				gObj[aIndex].AccountID, gObj[aIndex].Name,
				gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y);
			break;

		case 1: // Select Character
			gObj[aIndex].m_bOff = 0;
			gObjCloseSet(aIndex, 1);

			g_Log.Add("[UserSelClose] Go CharSelect [%s][%s] Map:[%d][%d/%d]",
				gObj[aIndex].AccountID, gObj[aIndex].Name,
				gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y);
			break;
	}
}




void GameProtocol::GCCloseMsgSend(int aIndex, BYTE result)
{
	PMSG_RESULT pMsg;

	PHeadSubSetBE((LPBYTE)&pMsg, 0xF1, 0x02, sizeof(pMsg));
	pMsg.result = result;
	
	IOCP.DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
}



void GameProtocol::CGClientMsg(PMSG_CLIENTMSG* lpMsg, int aIndex)
{
	if ( lpMsg->Flag )
	{
		g_Log.AddC(TColor::Red, "error-L1 : HackCheck [%s][%s] 0x%x 0x%x",
			gObj[aIndex].AccountID, gObj[aIndex].Name,
			lpMsg->Flag, lpMsg->subFlag);
	}
	else 
	{
		if ( gObj[aIndex].m_ClientHackLogCount < 100 )
		{
			g_Log.AddC(TColor::Red,  "error-L1 : HackCheck [%s][%s] 0x%x 0x%x",
				gObj[aIndex].AccountID, gObj[aIndex].Name,
				lpMsg->Flag, lpMsg->subFlag);
		}

		gObj[aIndex].m_ClientHackLogCount++;

		if ( gObj[aIndex].m_ClientHackLogCount >= 100 )
			gObj[aIndex].m_ClientHackLogCount = 100;
	}

	char msg[255];

	wsprintf(msg, "Client HackCheck %d", lpMsg->Flag);
}

struct SDHP_CREATECHAR
{
	PBMSG_HEAD h;	// C1:04
	int UserNumber;	// 4
	int DBNumber;	// 8
	short Number;	// C
	char AccountId[11];	// E
	char Name[11];	// 18
	BYTE ClassSkin;	// 22
};

void GameProtocol::CGPCharacterCreate( PMSG_CHARCREATE * lpMsg, int aIndex)
{
	if ( !PacketCheckTime(aIndex) )
	{
		JGCharacterCreateFailSend(aIndex, lpMsg->Name);
		return;
	}

	if ( aIndex < g_ConfigRead.server.GetObjectStartUserIndex() || aIndex > g_ConfigRead.server.GetObjectMax() )
	{
		return;
	}

	if ( gObj[aIndex].Connected < PLAYER_LOGGED )
	{
		g_Log.Add("Requested to create character before authentication [%d][%s]", aIndex, gObj[aIndex].m_PlayerData->Ip_addr);
		IOCP.CloseClient(aIndex);

		return;
	}

	if ( gObj[aIndex].Connected == PLAYER_PLAYING )
	{
		g_Log.Add("[Anti-HACK][CGPCharacterCreate] Attempted Character Create during GamePlay [%s][%s]",
			gObj[aIndex].AccountID, gObj[aIndex].Name);
		this->GCSendDisableReconnect(aIndex);
		//IOCP.CloseClient(aIndex);

		return;
	}

	if ( g_ConfigRead.server.GetServerType() == SERVER_BATTLECORE )
	{
		this->GCServerMsgStringSend(Lang.GetText(0,620), aIndex, 1);
		JGCharacterCreateFailSend(aIndex, lpMsg->Name);
		return;
	}

	if(gObj[aIndex].m_PlayerData->m_bSecurityCheck == false)
	{
		GCServerMsgStringSend(Lang.GetText(0,512), aIndex, 1);
		JGCharacterCreateFailSend(aIndex, lpMsg->Name);
		return;
	}

	if ( !gCreateCharacter )
	{
		GCServerMsgStringSend(Lang.GetText(0,513), aIndex, 1);
		JGCharacterCreateFailSend(aIndex, lpMsg->Name);
		return;
	}
	/*
		0x00 - Dark Wizard
		0x10 - Dark Knight
		0x20 - Elf
		0x30 - Magic Gladiator
		0x40 - Dark Lord
		0x50 - Summoner
		0x60 - Rage Fighter
	*/

	if ( lpMsg->ClassSkin == 0x00 || lpMsg->ClassSkin == 0x10 || lpMsg->ClassSkin == 0x20 || lpMsg->ClassSkin == 0x30 || lpMsg->ClassSkin == 0x40 || lpMsg->ClassSkin == 0x50 || lpMsg->ClassSkin == 0x60 || lpMsg->ClassSkin == 0x70 )
	{

	}
	else
	{
		JGCharacterCreateFailSend(aIndex, lpMsg->Name);
		return;
	}

	if ( lpMsg->ClassSkin == 0x30 ) // MG
	{
		if ( (gObj[aIndex].EnableCharacterCreate & 4) != 4 )
		{
			g_Log.AddC(TColor::Red,  "error-L1: Magumsa Character create error [%s]", gObj[aIndex].AccountID);
			JGCharacterCreateFailSend(aIndex, lpMsg->Name);
			return;
		}
	}

	if ( lpMsg->ClassSkin == 0x40 ) // DL
	{
		if ( (gObj[aIndex].EnableCharacterCreate & 2) != 2 )
		{
			g_Log.AddC(TColor::Red,  "error-L1: Darklord Character create error [%s]", gObj[aIndex].AccountID);
			JGCharacterCreateFailSend(aIndex, lpMsg->Name);
			return;
		}
	}

	if ( lpMsg->ClassSkin == 0x50 )
	{
		if ( (gObj[aIndex].EnableCharacterCreate & 1) != 1 )
		{
			g_Log.AddC(TColor::Red,  "error-L1: Summoner Character create error [%s]", gObj[aIndex].AccountID);
			JGCharacterCreateFailSend(aIndex, lpMsg->Name);
			return;
		}
	}

	if ( lpMsg->ClassSkin == 0x60 )
	{
		if ( (gObj[aIndex].EnableCharacterCreate & 8) != 8 )
		{
			g_Log.AddC(TColor::Red,  "error-L1: RageFighter Character create error [%s]", gObj[aIndex].AccountID);
			JGCharacterCreateFailSend(aIndex, lpMsg->Name);
			return;
		}
	}

	if ( lpMsg->ClassSkin == 0x70 )
	{
		if ( (gObj[aIndex].EnableCharacterCreate & 0x10) != 0x10 )
		{
			g_Log.AddC(TColor::Red,  "error-L1: GrowLancer Character create error [%s]", gObj[aIndex].AccountID);
			JGCharacterCreateFailSend(aIndex, lpMsg->Name);
			return;
		}
	}

	if(!g_prohibitedSymbols.Validate(lpMsg->Name,strlen(lpMsg->Name),TYPE_NAME))
	{
		JGCharacterCreateFailSend(aIndex, lpMsg->Name);
		return;
	}

	if ( SwearFilter.CompareText(lpMsg->Name) )
	{
		JGCharacterCreateFailSend(aIndex, lpMsg->Name);
		return;
	}

	if ( strstr(lpMsg->Name, "[A]") != 0 )
	{
		JGCharacterCreateFailSend(aIndex, lpMsg->Name);
		return;
	}

	SDHP_CREATECHAR sCreate;

	memset(&sCreate, 0, sizeof(sCreate));
	PHeadSetB((LPBYTE)&sCreate, 0x04, sizeof(sCreate));
	sCreate.ClassSkin = lpMsg->ClassSkin;
	sCreate.Number = aIndex;
	memcpy(sCreate.AccountId, gObj[aIndex].AccountID, MAX_ACCOUNT_LEN);
	memcpy(sCreate.Name, lpMsg->Name, MAX_ACCOUNT_LEN);
	char szTemp[11];
	char szTemp2[11];
	szTemp[10]=0;
	szTemp2[10]=0;
	memcpy(szTemp, sCreate.Name, MAX_ACCOUNT_LEN);
	memcpy(szTemp2, sCreate.AccountId, MAX_ACCOUNT_LEN);

	g_Log.Add("Character Created : %s,%s", szTemp2, szTemp);

	wsDataCli.DataSend((char*)&sCreate, sCreate.h.size);
}




struct SDHP_CHARDELETE
{
	PBMSG_HEAD h;	// C1:05
	short Number;	// 4
	char AccountID[11];	// 6
	char Name[11];	// 10
	BYTE Guild;	// [0:NoGuild] [1:Master] [2:Member] 1A
	char GuildName[8];	// 1B
};



void GameProtocol::CGPCharDel(PMSG_CHARDELETE * lpMsg,int aIndex)
{
	if ( !PacketCheckTime(aIndex) )
		return;

	if ( gObj[aIndex].Connected < PLAYER_LOGGED )
	{
		g_Log.AddC(TColor::Red, "[ERROR] Trying to delete character being not logged");
		IOCP.CloseClient(aIndex);
		return;
	}

	if ( gObj[aIndex].Connected == PLAYER_PLAYING )
	{
		g_Log.Add("[Anti-HACK][CGPCharDel] Attempted Character Delete during GamePlay. [%s][%s]",
			gObj[aIndex].AccountID, gObj[aIndex].Name);
		this->GCSendDisableReconnect(aIndex);
		//IOCP.CloseClient(aIndex);
		return;
	}

	if ( gObj[aIndex].bEnableDelCharacter == FALSE )
	{
		g_Log.Add("[Anti-HACK][CGPCharDel] Attempted Character Delete during Prevented Time. [%s][%s]",
			gObj[aIndex].AccountID, gObj[aIndex].Name);
		this->GCSendDisableReconnect(aIndex);
	//	IOCP.CloseClient(aIndex);
		return;
	}

	if (gObj[aIndex].m_PlayerData->GuildNumber > 0 && gObj[aIndex].m_PlayerData->lpGuild != NULL)
	{
		g_Log.Add("[Anti-HACK][CGPCharDel] Attempted Character Delete which belongs to guild. [%s][%s] [%s]", gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_PlayerData->lpGuild->Name);
		this->GCSendDisableReconnect(aIndex);
		//IOCP.CloseClient(aIndex);
		return;
	}

	SDHP_CHARDELETE pCDel;
	PMSG_RESULT pResult;

	PHeadSubSetB((LPBYTE)&pResult, 0xF3, 0x02, sizeof(pResult));
	pResult.result = 0;

	if ( gObj[aIndex].m_cAccountItemBlock )
	{
		pResult.result = 3;
		IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( g_ConfigRead.server.GetServerType() == SERVER_BATTLECORE )
	{
		pResult.result = 0;
		IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if(gObj[aIndex].m_PlayerData->m_bSecurityCheck == false)
	{
		this->GCServerMsgStringSend(Lang.GetText(0,513), aIndex, 1);
		pResult.result = 3;
		IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	char szJoomin[21];
	char szTemp[20];
	char szTemp2[20];

	memset(szJoomin, 0, sizeof(szJoomin));
	memcpy(szJoomin, lpMsg->LastJoominNumber, sizeof(lpMsg->LastJoominNumber));

	if ( gObjPasswordCheck(aIndex, szJoomin) == FALSE )
	{
		pResult.result = 2;
		IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	PHeadSetB((LPBYTE)&pCDel, 0x05, sizeof(pCDel));
	pCDel.Number = aIndex;
	memcpy(pCDel.AccountID, gObj[aIndex].AccountID, sizeof(pCDel.AccountID));
	memcpy(pCDel.Name, lpMsg->Name, sizeof( lpMsg->Name));
	pCDel.Guild = 0;

	g_UnityBattleField.GDReqCancelUnityBattleField(aIndex, 2, lpMsg->Name);

	szTemp[10]=0;
	szTemp2[10]=0;
	memcpy(szTemp, pCDel.Name, MAX_ACCOUNT_LEN);
	memcpy(szTemp2, pCDel.AccountID, MAX_ACCOUNT_LEN);

	g_Log.Add("Character Deleted : %s,%s", szTemp2, szTemp);

	wsDataCli.DataSend((PCHAR)&pCDel, pCDel.h.size);
	gObj[aIndex].Level = 0;
}



void GameProtocol::CGPCharacterMapJoinRequest( PMSG_CHARMAPJOIN * lpMsg, int aIndex)
{
	if ( !PacketCheckTime(aIndex) )
	{
		//JGCharacterCreateFailSend(aIndex, lpMsg->Name);
		return;
	}

	if ( !gObjIsAccontConnect(aIndex, gObj[aIndex].AccountID))
		return;

	if ( gObj[aIndex].Connected == PLAYER_PLAYING )
		return;

	gObj[aIndex].bEnableDelCharacter = FALSE;

	char _name[MAX_ACCOUNT_LEN+1];
	SDHP_DBCHARINFOREQUEST pCRequest;

	PHeadSetB((LPBYTE)&pCRequest, 0x06, sizeof(pCRequest));
	memset(_name, 0, MAX_ACCOUNT_LEN);

	if (g_ConfigRead.server.GetServerType() == SERVER_BATTLECORE)
	{
		std::string tmpCmpName;
		tmpCmpName.insert(0, lpMsg->Name, MAX_ACCOUNT_LEN);

		if (gObj[aIndex].m_PlayerData->m_NameConvertOfUBF.IsDuplicationOfName() == TRUE)
		{
			memcpy(_name, gObj[aIndex].m_PlayerData->m_NameConvertOfUBF.ConvertSlotIndexToName(lpMsg->btSlotIndex), MAX_ACCOUNT_LEN);
		}

		else
		{
			memcpy(_name, gObj[aIndex].m_PlayerData->m_NameConvertOfUBF.ConvertRealNameToName(tmpCmpName.c_str()), MAX_ACCOUNT_LEN);
		}
	}

	else
	{
		memcpy(_name, lpMsg->Name, MAX_ACCOUNT_LEN);
	}

	memcpy(pCRequest.Name, _name, MAX_ACCOUNT_LEN);
	strcpy(pCRequest.AccountID, gObj[aIndex].AccountID);
	pCRequest.Number = aIndex;
	pCRequest.IsSeasonX = g_ConfigRead.SeasonX;

	wsDataCli.DataSend((char*)&pCRequest, pCRequest.h.size);
}


struct PMSG_LEVELUP
{
	PBMSG_HEAD h;	// C1:F3:05
	BYTE subcode;	// 3
	WORD Level;	// 4
	WORD LevelUpPoint;	// 6
	WORD MaxLife;	// 8
	WORD MaxMana;	// A
	WORD wMaxShield;	// C
	WORD MaxBP;	// E
	short AddPoint;	// 10
	short MaxAddPoint;	// 12
	short MinusPoint;	// 14
	short MaxMinusPoint;	// 16
	int IGCMaxLife;
	int IGCMaxMana;
};


void GameProtocol::GCLevelUpMsgSend(int aIndex, int iSendEffect)
{
	if ( !ObjectMaxRange(aIndex))
		return;

	short AddPoint=0;
	short MaxAddPoint=0;
	short MinusPoint=0;
	short MaxMinusPoint=0;

	gObjGetStatPointState(gObj[aIndex].m_Index, AddPoint, MaxAddPoint, MinusPoint, MaxMinusPoint);
	
	PMSG_LEVELUP pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0xF3, 0x05, sizeof(pMsg));
	pMsg.Level = gObj[aIndex].Level;
	pMsg.LevelUpPoint = gObj[aIndex].m_PlayerData->LevelUpPoint;
	pMsg.MaxLife = gObj[aIndex].MaxLife+gObj[aIndex].AddLife;
	pMsg.MaxMana = gObj[aIndex].MaxMana+gObj[aIndex].AddMana;
	pMsg.MaxBP = gObj[aIndex].MaxBP + gObj[aIndex].AddBP;
	pMsg.wMaxShield = gObj[aIndex].iMaxShield+gObj[aIndex].iAddShield;
	pMsg.AddPoint = AddPoint;
	pMsg.MaxAddPoint = MaxAddPoint;
	pMsg.MinusPoint = MinusPoint;
	pMsg.MaxMinusPoint = MaxMinusPoint;
	pMsg.IGCMaxLife = gObj[aIndex].MaxLife + gObj[aIndex].AddLife;
	pMsg.IGCMaxMana = gObj[aIndex].MaxMana + gObj[aIndex].AddMana;

	IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

	if ( iSendEffect == 1 )
		GCSendEffectInfo(aIndex, 0x10);
}

void GameProtocol::CGLevelUpPointAdd(PMSG_LVPOINTADD * lpMsg, int aIndex)
{
	if ( !gObjIsConnected(aIndex))
	{
		g_Log.AddC(TColor::Red, "[ERROR] (CGLevelUpPointAdd) gObjIsConnected(aIndex)  == FALSE");
		this->GCSendDisableReconnect(aIndex);
		//IOCP.CloseClient(aIndex);
		return;
	}

	if (gObj[aIndex].Type != OBJ_USER)
	{
		g_Log.AddC(TColor::Red, "[ERROR] (CGLevelUpPointAdd) (%d) Index is not USER", aIndex);
		this->GCSendDisableReconnect(aIndex);
		//IOCP.CloseClient(aIndex);
		return;
	}

	EnterCriticalSection(&gObj[aIndex].m_PlayerData->AgiCheckCriti);

	gObj[aIndex].m_PlayerData->AgilityCheckDelay = GetTickCount();

	PMSG_LVPOINTADDRESULT pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0xF3, 0x06, sizeof(pMsg));
	pMsg.ResultType = 0;
	
	if (::gObjLevelUpPointAdd(lpMsg->Type, &gObj[aIndex]) == TRUE )
	{
		pMsg.ResultType = 0x10;
		pMsg.ResultType += lpMsg->Type;

		switch ( lpMsg->Type )
		{
			case 2:	// Vit
				pMsg.MaxLifeAndMana = gObj[aIndex].MaxLife + gObj[aIndex].AddLife;
				pMsg.IGCMaxLifeAndMana = gObj[aIndex].MaxLife + gObj[aIndex].AddLife;
				break;

			case 3:	// Energy
				pMsg.MaxLifeAndMana = gObj[aIndex].MaxMana + gObj[aIndex].AddMana;
				pMsg.IGCMaxLifeAndMana = gObj[aIndex].MaxMana + gObj[aIndex].AddMana;
				break;
		}

		pMsg.wMaxShield = gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield;
		gObjSetBP(aIndex);
		pMsg.MaxBP = gObj[aIndex].MaxBP + gObj[aIndex].AddBP;
	}

	IOCP.DataSend(aIndex, (UCHAR *)&pMsg, pMsg.h.size);
	LeaveCriticalSection(&gObj[aIndex].m_PlayerData->AgiCheckCriti);
}


struct PMSG_INVENTORYITEMMODIFY
{
	PBMSG_HEAD h;	// C1:F3:14
	BYTE subcode;	// 3
	BYTE Pos;	// 4
	BYTE ItemInfo[MAX_ITEM_INFO];	// 5

};

void GameProtocol::GCInventoryItemOneSend(int aIndex, int pos)
{
	if ( !gObj[aIndex].pInventory[pos].IsItem())
		return;

	PMSG_INVENTORYITEMMODIFY pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0xF3, 0x14, sizeof(pMsg));
	pMsg.Pos = pos;
	ItemByteConvert(pMsg.ItemInfo, gObj[aIndex].pInventory[pos]);

	IOCP.DataSend(aIndex, (UCHAR *)&pMsg, pMsg.h.size);
}


struct PMSG_PKLEVEL
{
	PBMSG_HEAD h;	// C1:F3:08
	BYTE subcode;	// 3
	BYTE NumberH;	// 4
	BYTE NumberL;	// 5
	BYTE PkLevel;	// 6
};

void GameProtocol::GCPkLevelSend(int aIndex, BYTE pklevel)
{
	PMSG_PKLEVEL pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0xF3, 0x08, sizeof(pMsg));
	pMsg.NumberH = SET_NUMBERH(aIndex);
	pMsg.NumberL = SET_NUMBERL(aIndex);
	pMsg.PkLevel = pklevel;

	IOCP.DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
	MsgSendV2(&gObj[aIndex], (UCHAR*)&pMsg, pMsg.h.size);
}



#pragma pack(1)
struct PMSG_MAGICLIST	//Revised to 1.01.03
{
	char Pos;	// 0
	WORD wSkillNum; //3
	BYTE btSkillLevel;	// 5
};
#pragma pack()

struct PMSG_MAGICLISTCOUNT_S9
{
	PBMSG_HEAD h;	// C1:F3:11
	BYTE subcode;	// 3
	BYTE Count;	// [0xFE:AddOne] [0xFF:DelOne] 4
	BYTE btListType;	// 5
};

struct PMSG_MAGICLISTCOUNT_SX
{
	PWMSG_HEAD h;
	BYTE subcode;
	BYTE Count;
	BYTE btListType;
};

void GameProtocol::GCMagicListOneSend(int aIndex, char Pos, WORD type, BYTE level, WORD skill, BYTE btListType)
{
	if (g_ConfigRead.SeasonX == false)
	{
		PMSG_MAGICLISTCOUNT_S9 pCount;
		PMSG_MAGICLIST pList;
		BYTE sendbuf[1000];

		int lOfs = sizeof(pCount);
		PHeadSubSetB((LPBYTE)&pCount, 0xF3, 0x11, 0);

		pCount.Count = 0xFE;
		pCount.btListType = btListType;

		pList.Pos = Pos;
		pList.wSkillNum = type;
		pList.btSkillLevel = level << 3;
		pList.btSkillLevel |= (BYTE)skill & 0x07;

		memcpy(&sendbuf[lOfs], &pList, sizeof(pList));
		lOfs += sizeof(pList);

		pCount.h.size = lOfs;
		memcpy(sendbuf, &pCount, sizeof(pCount));

		IOCP.DataSend(aIndex, sendbuf, lOfs);
	}

	else
	{
		PMSG_MAGICLISTCOUNT_SX pCount;
		PMSG_MAGICLIST pList;
		BYTE sendbuf[1000];

		int lOfs = sizeof(pCount);
		PHeadSubSetW((LPBYTE)&pCount, 0xF3, 0x11, 0);

		pCount.Count = 0xFE;
		pCount.btListType = btListType;

		pList.Pos = Pos;
		pList.wSkillNum = type;
		pList.btSkillLevel = level << 3;
		pList.btSkillLevel |= (BYTE)skill & 0x07;

		memcpy(&sendbuf[lOfs], &pList, sizeof(pList));
		lOfs += sizeof(pList);

		pCount.h.sizeH = SET_NUMBERH(lOfs);
		pCount.h.sizeL = SET_NUMBERL(lOfs);
		memcpy(sendbuf, &pCount, sizeof(pCount));

		IOCP.DataSend(aIndex, sendbuf, lOfs);
	}
}


void GameProtocol::GCMagicListOneDelSend(int aIndex, char Pos, WORD type, BYTE level, WORD skill, BYTE btListType)
{
	if (g_ConfigRead.SeasonX == false)
	{
		PMSG_MAGICLISTCOUNT_S9 pCount;
		PMSG_MAGICLIST pList;
		BYTE sendbuf[1000];

		int lOfs = sizeof(pCount);
		PHeadSubSetB((LPBYTE)&pCount, 0xF3, 0x11, 0);

		pCount.h.size = 0;
		pCount.Count = 0xFF;
		pCount.btListType = btListType;

		pList.Pos = Pos;
		pList.wSkillNum = type;
		pList.btSkillLevel = level << 3;
		pList.btSkillLevel |= (BYTE)skill & 0x07;

		memcpy(&sendbuf[lOfs], &pList, sizeof(pList));
		lOfs += sizeof(pList);

		pCount.h.size = lOfs;
		memcpy(sendbuf, &pCount, sizeof(pCount));

		IOCP.DataSend(aIndex, sendbuf, lOfs);
	}

	else
	{
		PMSG_MAGICLISTCOUNT_SX pCount;
		PMSG_MAGICLIST pList;
		BYTE sendbuf[1000];

		int lOfs = sizeof(pCount);
		PHeadSubSetW((LPBYTE)&pCount, 0xF3, 0x11, 0);

		pCount.h.sizeH = 0;
		pCount.h.sizeL = 0;
		pCount.Count = 0xFF;
		pCount.btListType = btListType;

		pList.Pos = Pos;
		pList.wSkillNum = type;
		pList.btSkillLevel = level << 3;
		pList.btSkillLevel |= (BYTE)skill & 0x07;

		memcpy(&sendbuf[lOfs], &pList, sizeof(pList));
		lOfs += sizeof(pList);

		pCount.h.sizeH = SET_NUMBERH(lOfs);
		pCount.h.sizeL = SET_NUMBERL(lOfs);
		memcpy(sendbuf, &pCount, sizeof(pCount));

		IOCP.DataSend(aIndex, sendbuf, lOfs);
	}
}


void GameProtocol::GCMagicListMultiSend(LPOBJ lpObj, BYTE btListType)
{
	if (g_ConfigRead.SeasonX == false)
	{
		WORD BrandSkill;
		PMSG_MAGICLISTCOUNT_S9 pCount;
		PMSG_MAGICLIST pList;
		BYTE sendbuf[1000];

		int lOfs = sizeof(pCount);
		int Count = 0;
		pCount.btListType = btListType;

		PHeadSubSetB((LPBYTE)&pCount, 0xF3, 0x11, 0);

		for (int n = 0; n < MAX_MAGIC; n++)
		{
			if (lpObj->Magic[n].IsMagic() == TRUE)
			{
				if (MagicDamageC.GetSkillUseType(lpObj->Magic[n].m_Skill) == 3)
				{
					continue;
				}

				if (!g_MasterLevelSkillTreeSystem.CheckUsableWeaponSkill(lpObj, n))
				{
					continue;
				}

				if (g_MasterLevelSkillTreeSystem.IsExistMLSBasedSkill(lpObj, lpObj->Magic[n].m_Skill))
				{
					continue;
				}

				if (lpObj->Magic[n].m_Pass == true)
				{
					continue;
				}

				pList.Pos = n;
				pList.wSkillNum = (WORD)lpObj->Magic[n].m_Skill;
				pList.btSkillLevel = lpObj->Magic[n].m_Level << 3;
				pList.btSkillLevel |= lpObj->Magic[n].m_Skill & 0x07;

				memcpy(&sendbuf[lOfs], &pList, sizeof(pList));
				lOfs += sizeof(pList);

				Count++;

				if (MagicDamageC.IsItemAttachedSkill(lpObj->Magic[n].m_Skill))
				{
					BrandSkill = MagicDamageC.GetNormalBrandOfSkill(lpObj->Magic[n].m_Skill);

					if (!lpObj->pInventory[1].IsItem() || lpObj->pInventory[1].m_Special[0] != BrandSkill)
					{
						continue;
					}

					if (!lpObj->pInventory[0].IsItem() || lpObj->pInventory[0].m_Special[0] != BrandSkill)
					{
						continue;
					}

					pList.Pos = n + 1;
					pList.wSkillNum = LOWORD(lpObj->Magic[n].m_Skill);
					pList.btSkillLevel = lpObj->Magic[n].m_Level << 3;
					pList.btSkillLevel |= lpObj->Magic[n].m_Skill & 0x07;

					memcpy(&sendbuf[lOfs], &pList, sizeof(pList));
					lOfs += sizeof(pList);

					Count++;
				}
			}
		}

		pCount.Count = Count;
		pCount.h.size = lOfs;
		memcpy(sendbuf, &pCount, sizeof(pCount));

		IOCP.DataSend(lpObj->m_Index, sendbuf, lOfs);
	}

	else
	{
		WORD BrandSkill;
		PMSG_MAGICLISTCOUNT_SX pCount;
		PMSG_MAGICLIST pList;
		BYTE sendbuf[1000];

		int lOfs = sizeof(pCount);
		int Count = 0;
		pCount.btListType = btListType;

		PHeadSubSetW((LPBYTE)&pCount, 0xF3, 0x11, 0);

		for (int n = 0; n < MAX_MAGIC; n++)
		{
			if (lpObj->Magic[n].IsMagic() == TRUE)
			{
				if (MagicDamageC.GetSkillUseType(lpObj->Magic[n].m_Skill) == 3)
				{
					continue;
				}

				if (!g_MasterLevelSkillTreeSystem.CheckUsableWeaponSkill(lpObj, n))
				{
					continue;
				}

				if (g_MasterLevelSkillTreeSystem.IsExistMLSBasedSkill(lpObj, lpObj->Magic[n].m_Skill))
				{
					continue;
				}

				if (lpObj->Magic[n].m_Pass == true)
				{
					continue;
				}

				pList.Pos = n;
				pList.wSkillNum = (WORD)lpObj->Magic[n].m_Skill;
				pList.btSkillLevel = lpObj->Magic[n].m_Level << 3;
				pList.btSkillLevel |= lpObj->Magic[n].m_Skill & 0x07;

				memcpy(&sendbuf[lOfs], &pList, sizeof(pList));
				lOfs += sizeof(pList);

				Count++;

				if (MagicDamageC.IsItemAttachedSkill(lpObj->Magic[n].m_Skill))
				{
					BrandSkill = MagicDamageC.GetNormalBrandOfSkill(lpObj->Magic[n].m_Skill);

					if (!lpObj->pInventory[1].IsItem() || lpObj->pInventory[1].m_Special[0] != BrandSkill)
					{
						continue;
					}

					if (!lpObj->pInventory[0].IsItem() || lpObj->pInventory[0].m_Special[0] != BrandSkill)
					{
						continue;
					}

					pList.Pos = n + 1;
					pList.wSkillNum = LOWORD(lpObj->Magic[n].m_Skill);
					pList.btSkillLevel = lpObj->Magic[n].m_Level << 3;
					pList.btSkillLevel |= lpObj->Magic[n].m_Skill & 0x07;

					memcpy(&sendbuf[lOfs], &pList, sizeof(pList));
					lOfs += sizeof(pList);

					Count++;
				}
			}
		}

		pCount.Count = Count;
		pCount.h.sizeH = SET_NUMBERH(lOfs);
		pCount.h.sizeL = SET_NUMBERL(lOfs);
		memcpy(sendbuf, &pCount, sizeof(pCount));

		IOCP.DataSend(lpObj->m_Index, sendbuf, lOfs);
	}
}



struct PMSG_EQUIPMENTLIST
{
	PBMSG_HEAD h;	// C1:F3:13
	BYTE subcode;	// 3
	BYTE NumberH;	// 4
	BYTE NumberL;	// 5
	BYTE Equipment[CHAR_SET_SIZE];	// 6
};

void GameProtocol::GCEquipmentSend(int aIndex)
{
	PMSG_EQUIPMENTLIST pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0xF3, 0x13, sizeof(pMsg));
	pMsg.NumberH = SET_NUMBERH(aIndex);
	pMsg.NumberL = SET_NUMBERL(aIndex);
	gObjMakePreviewCharSet(aIndex);
	LPOBJ lpObj = &gObj[aIndex];
	memcpy(pMsg.Equipment, lpObj->CharSet, sizeof(lpObj->CharSet));

	MsgSendV2(lpObj, (LPBYTE)&pMsg, pMsg.h.size);
}


struct PMSG_RECALLMONLIFE
{
	PBMSG_HEAD h;	// C1:F3:20
	BYTE subcode;	// 3
	BYTE Life;	// 4
};

void GameProtocol::GCRecallMonLife(int aIndex, int maxlife, int life)
{
	if ( maxlife <= 0 )
		return;
	
	PMSG_RECALLMONLIFE pMsg;
	BYTE per = life * 100 / maxlife;
	PHeadSubSetB((LPBYTE)&pMsg, 0xF3, 0x20, sizeof(pMsg));
	pMsg.Life = per;

	IOCP.DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
}



struct PMSG_TIMEVIEW
{
	PBMSG_HEAD h;	// C1:F3:22
	BYTE subcode;	// 3
	WORD Second;	// 4
};

void GameProtocol::GCTimeViewSend(int aIndex, int second)
{
	PMSG_TIMEVIEW pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0xF3, 0x22, sizeof(pMsg));
	pMsg.Second = second;

	IOCP.DataSend(aIndex, (UCHAR *)&pMsg, pMsg.h.size);
}



struct PMSG_GOALSEND
{
	PBMSG_HEAD h;	// C1:F3:23
	BYTE subcode;	// 3
	char RedTeamName[8];	// 4
	BYTE RedTeamScore;	// C
	char BlueTeamName[8];	// D
	BYTE BlueTeamScore;	// 15
};

void GameProtocol::GCGoalSend(int aIndex, char* Name1, BYTE score1, char* Name2, BYTE score2)
{
	PMSG_GOALSEND pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0xF3, 0x23, sizeof(pMsg));
	memcpy(pMsg.RedTeamName, Name1, sizeof(pMsg.RedTeamName));
	pMsg.RedTeamScore = score1;
	memcpy(pMsg.BlueTeamName, Name2, sizeof(pMsg.BlueTeamName));
	pMsg.BlueTeamScore = score2;

	IOCP.DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
}



void GameProtocol::GCSkillKeyRecv(PMSG_SKILLKEY * lpMsg, int aIndex)
{
	if ( !gObjIsGamePlaing(&gObj[aIndex]))
		return;

	if (gObj[aIndex].Type != OBJ_USER)
	{
		return;
	}

	DGOptionDataSend(aIndex, gObj[aIndex].Name,
		lpMsg->SkillKey, lpMsg->GameOption,
		lpMsg->QkeyDefine, lpMsg->WkeyDefine, lpMsg->EkeyDefine, lpMsg->ChatWindow, lpMsg->RkeyDefine, ntohl(lpMsg->QWERLevel), gObj[aIndex].m_PlayerData->m_EnableUseChangeSkin);
}





void GameProtocol::GCSkillKeySend(int aIndex, LPBYTE keybuffer, BYTE GO, BYTE Qk, BYTE Wk, BYTE Ek, BYTE ChatWnd, BYTE Rk, int QWER)
{
	PMSG_SKILLKEY pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0xF3, 0x30, sizeof(pMsg));
	memcpy(pMsg.SkillKey, keybuffer, sizeof(pMsg.SkillKey));
	pMsg.GameOption = GO;
	pMsg.QkeyDefine = Qk;
	pMsg.WkeyDefine = Wk;
	pMsg.EkeyDefine = Ek;
	pMsg.ChatWindow = ChatWnd;
	pMsg.RkeyDefine = Rk;
	pMsg.QWERLevel = QWER;

	IOCP.DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
}


struct PMSG_ITEMGETRESULT
{
	PBMSG_HEAD h;	// C1:22
	BYTE result;	// [0xFE:Money] 3
	BYTE Data[MAX_ITEM_INFO];	// 4
};

void GameProtocol::GCMoneySend(int aIndex, DWORD money)
{
	PMSG_ITEMGETRESULT pMsg;

	pMsg.h.c = 0xC3;
	pMsg.h.headcode = 0x22;
	pMsg.h.size = sizeof(pMsg);
	pMsg.result = 0xFE;

	WORD hiWord = SET_NUMBERHW(money);
	WORD loWord = SET_NUMBERLW(money);
	pMsg.Data[0] = SET_NUMBERH(hiWord);
	pMsg.Data[1] = SET_NUMBERL(hiWord);
	pMsg.Data[2] = SET_NUMBERH(loWord);
	pMsg.Data[3] = SET_NUMBERL(loWord);

	IOCP.DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
}


void GameProtocol::GCItemInventoryPutSend(int aIndex,BYTE result, BYTE iteminfo1, BYTE iteminfo2)
{
	PMSG_ITEMGETRESULT pResult;

	pResult.h.c = 0xC3;
	pResult.h.headcode = 0x22;
	pResult.h.size = sizeof(pResult);
	pResult.result = result;
	pResult.Data[0] = iteminfo1;
	pResult.Data[1] = iteminfo2;
	pResult.h.size -= 2;

	IOCP.DataSend(aIndex, (UCHAR*)&pResult, pResult.h.size);
}



void GameProtocol::CGItemGetRequest(PMSG_ITEMGETREQUEST * lpMsg, int aIndex)
{
	int item_num, map_num;
	PMSG_ITEMGETRESULT pResult;
	int type;
	int level;
	int special;
	int NOption;
	char szItemName[50];
	CMapItem * lpItem;

	pResult.h.c = 0xC3;
	pResult.h.headcode = 0x22;
	pResult.h.size = sizeof(pResult);
	pResult.result = -1;

	if ( !gObjIsConnected(aIndex))
	{
		g_Log.AddC(TColor::Red, "[ERROR] (CGItemGetRequest) gObjIsConnected(aIndex)  == FALSE");
		IOCP.CloseClient(aIndex);
		return;
	}

	if ( gObj[aIndex].Type != OBJ_USER )
	{
		return;
	}

	if ( gObj[aIndex].CloseType != -1 )
		return;

	if ( gObj[aIndex].m_PlayerData->m_bSecurityCheck == false )
	{
		pResult.result = -1;
		pResult.h.size -= sizeof(pResult.Data);
		IOCP.DataSend(aIndex, (UCHAR*)&pResult, pResult.h.size);

		return;
	}

	if ( gObj[aIndex].DieRegen != 0 )
	{
		pResult.result = -1;
		pResult.h.size -= sizeof(pResult.Data);
		IOCP.DataSend(aIndex, (UCHAR*)&pResult, pResult.h.size);

		return;
	}

	if ( g_NewPVP.IsObserver(gObj[aIndex]) == TRUE )
	{
		pResult.result = -1;
		pResult.h.size -= sizeof(pResult.Data);
		IOCP.DataSend(aIndex, (UCHAR*)&pResult, pResult.h.size);

		return;
	}

	if ( gObj[aIndex].m_IfState.use != 0 )
	{
		if ( gObj[aIndex].m_IfState.type != 3 )
		{
			this->GCServerMsgStringSend(Lang.GetText(0,31), aIndex, 1);
			pResult.result = -1;
			pResult.h.size -= sizeof(pResult.Data);
			IOCP.DataSend(aIndex, (UCHAR*)&pResult, pResult.h.size);

			return;
		}
	}

	if ( !::gObjFixInventoryPointer(aIndex))
		g_Log.Add("[Fix Inv.Ptr] False Location - %s, %d", __FILE__, __LINE__);

	if ( gObj[aIndex].pTransaction == TRUE )
	{
		g_Log.Add("[%s][%s] CGItemGetRequest() Failed : Transaction == 1, IF_TYPE : %d",
			gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_IfState.type);
		return;

	}

	CMuRummyInfo * pMuRummyInfo = gObj[aIndex].m_PlayerData->m_pCMuRummyInfo;

	if (!pMuRummyInfo)
	{
		g_Log.Add("[MuRummy][Error] pCMuRummyInfo is NULL [%s][%s] [%s, %d]",
			gObj[aIndex].AccountID, gObj[aIndex].Name, __FILE__, __LINE__);
		return;
	}

	if (pMuRummyInfo->IsWaitReward() == true)
	{
		pResult.result = -1;
		pResult.h.size -= sizeof(pResult.Data);
		IOCP.DataSend(aIndex, (UCHAR*)&pResult, pResult.h.size);

		g_Log.Add("[MuRummy][DebugLog][%s][%s] Waiting Item Reward(CGItemGetReq)", gObj[aIndex].AccountID, gObj[aIndex].Name);
		this->GCServerMsgStringSend(Lang.GetText(0,561), aIndex, 1);
		return;
	}

	item_num = MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);

	if ( MaxItemTypeRange(item_num) == FALSE )
	{
		g_Log.Add("error-L3 : %s %d", __FILE__, __LINE__);
		pResult.result = -1;
		pResult.h.size -= sizeof(pResult.Data);
		IOCP.DataSend(aIndex, (UCHAR*)&pResult, pResult.h.size);

		return;
	}

	map_num = gObj[aIndex].MapNumber;

	if ( MAX_MAP_RANGE(map_num) == FALSE )
	{
		g_Log.Add("error-L3 : %s %d", __FILE__, __LINE__);
		pResult.result = -1;
		pResult.h.size -= sizeof(pResult.Data);
		IOCP.DataSend(aIndex, (UCHAR*)&pResult, pResult.h.size);

		return;
	}

	lpItem = &MapC[map_num].m_cItem[item_num];

	if ( lpItem->IsItem() == TRUE && lpItem->Give == false && lpItem->live == true)
	{
		if (IT_MAP_RANGE(map_num) == TRUE)
		{
			if (lpItem->m_Type == ITEMGET(14, 223))
			{
				if (g_ConfigRead.server.GetServerType() == SERVER_MARKET)
				{
					if (g_IT_Event.CheckRelics(aIndex) == TRUE)
					{
						pResult.result = -1;
						pResult.h.size -= sizeof(pResult.Data);
						IOCP.DataSend(aIndex, (UCHAR*)&pResult, pResult.h.size);

						return;
					}
				}
			}
		}

		if ( lpItem->m_QuestItem != false )
		{
			bool bIsGetItem = g_QuestInfo.GetQuestItem(aIndex, lpItem->m_Type, lpItem->m_Level);
			bool bIsQuestExpItem = g_QuestExpProgMng.IsQuestDropItem(aIndex, lpItem->m_Type, lpItem->m_Level);
			
			if (bIsGetItem == false && bIsQuestExpItem == false)
			{
				pResult.result = -1;
				pResult.h.size -= sizeof(pResult.Data);
				IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
				return;
			}
		}

		if (IsJumpingEventItem(lpItem->m_Type))
		{
			pResult.result = -1;
			pResult.h.size -= sizeof(pResult.Data);
			IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
			return;
		}

		LPITEM_ATTRIBUTE pItemAttribute = GetItemAttr(lpItem->m_Type);

		if (!pItemAttribute)
		{
			pResult.result = -1;
			pResult.h.size -= sizeof(pResult.Data);
			IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
			return;
		}

		if (pItemAttribute->ItemKindA == 12)
		{
			pResult.result = -1;
			pResult.h.size -= sizeof(pResult.Data);
			IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
			return;
		}

		if ( lpItem->m_Type == ITEMGET(13,20) )	// Wizard Ring
		{
			switch ( lpItem->m_Level )
			{
				case 0:
				{
					int iWRCount = gObjGetItemCountInIventory(aIndex, lpItem->m_Type/MAX_SUBTYPE_ITEMS, lpItem->m_Type%MAX_SUBTYPE_ITEMS, lpItem->m_Level);

					if ( iWRCount > 0 )
					{
						g_Log.Add("[Ring Event] Too many have Magician's Ring [%s][%s] (Name:%s, Count:%d)",
							gObj[aIndex].AccountID, gObj[aIndex].Name, lpItem->GetName(), iWRCount);

						pResult.result = -1;
						pResult.h.size -= sizeof(pResult.Data);

						IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
						this->GCServerMsgStringSend(Lang.GetText(0,105), aIndex, 1);

						return;
					}
					break;
				}
				case 1:

					pResult.result = -1;
					pResult.h.size -= sizeof(pResult.Data);

					IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
					return;
					break;
				
			}
		}

		if ( lpItem->m_Type == ITEMGET(13,20) && lpItem->m_Level == 2 )	// Wizard Ring
		{
			pResult.result = -1;
			pResult.h.size -= sizeof(pResult.Data);

			IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);			

			return;
		}

		if ( lpItem->m_Type == ITEMGET(13,38) )	// MoonStonePendant
		{
			int count = gObjGetItemCountInIventory(aIndex, lpItem->m_Type/MAX_SUBTYPE_ITEMS,
				lpItem->m_Type % MAX_SUBTYPE_ITEMS, lpItem->m_Level);

			if ( count > 0 )
			{
				g_Log.Add("[MoonStonePendant] Too many have MoonStonePendant [%s][%s] ( Name:%s, Count:%d )",
					gObj[aIndex].AccountID, gObj[aIndex].Name, lpItem->GetName(), count);

				pResult.result = -1;
				pResult.h.size -= sizeof(pResult.Data);

				IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);	
				GCServerMsgStringSend(Lang.GetText(0,285), aIndex, 1);

				return;
			}
		}

		if ( lpItem->m_Type == ITEMGET(13,39) )	// ChangeRing
		{
			int count = gObjGetItemCountInIventory(aIndex, lpItem->m_Type/MAX_SUBTYPE_ITEMS,
				lpItem->m_Type % MAX_SUBTYPE_ITEMS, lpItem->m_Level);

			if ( count > 0 )
			{
				g_Log.Add("[ChangeRing] Too many have ChangeRing [%s][%s] ( Name:%s, Count:%d )",
					gObj[aIndex].AccountID, gObj[aIndex].Name, lpItem->GetName(), count);

				pResult.result = -1;
				pResult.h.size -= sizeof(pResult.Data);

				IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);	
				GCServerMsgStringSend(Lang.GetText(0,287), aIndex, 1);

				return;
			}
		}

		type = lpItem->m_Type;
		level = lpItem->m_Level;
		special = lpItem->m_Special[0];
		NOption = lpItem->m_NewOption;
		strcpy(szItemName, lpItem->GetName());

		if ( lpItem->m_Type == ITEMGET(14,15) ) // Zen
		{
			if ( MapC[map_num].ItemGive(aIndex, item_num, false) == TRUE )
			{
				if ( !gObjCheckMaxZen(aIndex, lpItem->m_BuyMoney))
				{
					if ( gObj[aIndex].m_PlayerData->Money < MAX_ZEN )
					{
						gObj[aIndex].m_PlayerData->Money = MAX_ZEN;
						pResult.result = -2;
						WORD hiWord = SET_NUMBERHW(gObj[aIndex].m_PlayerData->Money);
						WORD loWord = SET_NUMBERLW(gObj[aIndex].m_PlayerData->Money);
						pResult.Data[0] = SET_NUMBERH(hiWord);
						pResult.Data[1] = SET_NUMBERL(hiWord);
						pResult.Data[2] = SET_NUMBERH(loWord);
						pResult.Data[3] = SET_NUMBERL(loWord);

						IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

						return;
					}

					pResult.result = -1;
					pResult.h.size -= sizeof(pResult.Data);

					IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);			

					return;
				}

				gObj[aIndex].m_PlayerData->Money += lpItem->m_BuyMoney;
				pResult.result = -2;

				WORD hiWord = SET_NUMBERHW(gObj[aIndex].m_PlayerData->Money);
				WORD loWord = SET_NUMBERLW(gObj[aIndex].m_PlayerData->Money);
				pResult.Data[0] = SET_NUMBERH(hiWord);
				pResult.Data[1] = SET_NUMBERL(hiWord);
				pResult.Data[2] = SET_NUMBERH(loWord);
				pResult.Data[3] = SET_NUMBERL(loWord);
				pResult.h.size -= 3;
			}

			IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

		}
		else
		{
			if ( lpItem->m_Type == ITEMGET(13,32) )
			{
				int pos = gObjOverlapItemUsingDur((lpItem)?((CItem *)&lpItem->m_Number):NULL, map_num, item_num, aIndex, IsOverlapItem(lpItem->m_Type), ITEMGET(13,32), 0);

				if ( MAIN_INVENTORY_RANGE(pos) != FALSE )
				{
					pResult.result = -3;
					IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
					gObj[aIndex].pInventory[pos].m_Durability += lpItem->m_Durability;
					GCItemDurSend(aIndex, pos, gObj[aIndex].pInventory[pos].m_Durability, 0);

					return;
				}
			}

			if ( lpItem->m_Type == ITEMGET(13,33) )
			{
				int pos = gObjOverlapItemUsingDur((lpItem)?((CItem *)&lpItem->m_Number):NULL, map_num, item_num, aIndex, IsOverlapItem(lpItem->m_Type), ITEMGET(13,33), 0);

				if ( MAIN_INVENTORY_RANGE(pos) != FALSE )
				{
					pResult.result = -3;
					IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
					gObj[aIndex].pInventory[pos].m_Durability += lpItem->m_Durability;
					GCItemDurSend(aIndex, pos, gObj[aIndex].pInventory[pos].m_Durability, 0);

					return;
				}
			}

			if ( lpItem->m_Type == ITEMGET(13,34) )
			{
				int pos = gObjOverlapItemUsingDur((lpItem)?((CItem *)&lpItem->m_Number):NULL, map_num, item_num, aIndex, IsOverlapItem(lpItem->m_Type), ITEMGET(13,34), 0);

				if ( MAIN_INVENTORY_RANGE(pos) != FALSE )
				{
					pResult.result = -3;
					IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
					gObj[aIndex].pInventory[pos].m_Durability += lpItem->m_Durability;
					GCItemDurSend(aIndex, pos, gObj[aIndex].pInventory[pos].m_Durability, 0);

					return;
				}
			}

			if ( lpItem->m_Type == ITEMGET(13,147) ) // Trophy of Battle
			{
				int pos = gObjOverlapItemUsingDur((lpItem)?((CItem *)&lpItem->m_Number):NULL, map_num, item_num, aIndex, IsOverlapItem(lpItem->m_Type), ITEMGET(13,147), 0);

				if ( MAIN_INVENTORY_RANGE(pos) != FALSE )
				{
					pResult.result = -3;
					IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
					gObj[aIndex].pInventory[pos].m_Durability += lpItem->m_Durability;
					GCItemDurSend(aIndex, pos, gObj[aIndex].pInventory[pos].m_Durability, 0);

					return;
				}
			}
			
			if ( lpItem->m_Type == ITEMGET(14,29) ) // Symbol of Kundun
			{
				for ( int n=0;n<MAIN_INVENTORY_SIZE;n++)
				{ 
					int pos = ::g_KalimaGate.CheckOverlapKundunMark(aIndex, lpItem->m_Level);

					if ( MAIN_INVENTORY_RANGE(pos) )
					{
						if ( MapC[map_num].ItemGive(aIndex, item_num, true) == TRUE )
						{
							BYTE NewOption[MAX_EXOPTION_SIZE];

							::ItemIsBufExOption(NewOption, (lpItem != NULL)?(CItem*)&lpItem->m_Number:NULL);

							g_Log.Add("[%s][%s][%d]%d/%d Get serial:%I64d [%s][%d][%d][%d][%d][%d] dur:[%d]Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d] 380:[%d] HO:[%d,%d] E:[%d]", gObj[aIndex].AccountID, gObj[aIndex].Name, map_num,
								gObj[aIndex].X, gObj[aIndex].Y, lpItem->m_Number, szItemName, type,
								level, lpItem->m_Option1, lpItem->m_Option2, lpItem->m_Option3, (int)lpItem->m_Durability,
								NewOption[0], NewOption[1], NewOption[2], NewOption[3], NewOption[4], NewOption[5],
								NewOption[6], lpItem->m_SetOption, lpItem->m_ItemOptionEx>>7,
								g_kJewelOfHarmonySystem.GetItemStrengthenOption((lpItem)?((CItem *)&lpItem->m_Number):NULL), g_kJewelOfHarmonySystem.GetItemOptionLevel((lpItem)?((CItem *)&lpItem->m_Number):NULL),
								lpItem->m_BonusSocketOption);

							pResult.result = -3;

							IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);			

							gObj[aIndex].pInventory[pos].m_Durability += lpItem->m_Durability;

							if ( gObj[aIndex].pInventory[pos].m_Durability >= IsOverlapItem(lpItem->m_Type) )
							{
								int NewDur = gObj[aIndex].pInventory[pos].m_Durability - IsOverlapItem(lpItem->m_Type);
								::gObjInventoryItemSet(aIndex, pos, -1);
								gObj[aIndex].pInventory[pos].Clear();
								this->GCInventoryItemDeleteSend(aIndex, pos, 1);
								::ItemSerialCreateSend(aIndex, 235, gObj[aIndex].X, gObj[aIndex].Y, ItemGetNumberMake(14, 28),
									lpItem->m_Level, 0, 0, 0, 0, aIndex, 0, 0, 0, 0, 0);

								g_Log.Add("[Kalima] [%s][%s] Make Lost Kalima Map (Left Kundun Mark:%d)",
									gObj[aIndex].AccountID, gObj[aIndex].Name, NewDur);

								if ( NewDur > 0 )
								{
									lpItem->m_State = 2;
									lpItem->Give = false;
									lpItem->live = true;
									lpItem->m_Durability = NewDur;

									continue;

								}
							}
							else
							{
								GCItemDurSend(aIndex,pos, gObj[aIndex].pInventory[pos].m_Durability, 0);
								
							}
							return;
						}
					}
					else
					{
						break;
					}
				}
			}
			if ( lpItem->m_Type == ITEMGET(14,110) ) // Double Goer
			{
					int pos = gObjOverlapItemUsingDur((lpItem)?((CItem *)&lpItem->m_Number):NULL, map_num, item_num, aIndex, IsOverlapItem(lpItem->m_Type), ITEMGET(14,110), 0);
					if ( MAIN_INVENTORY_RANGE(pos) )
					{
						pResult.result = -3;

						IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);			
						gObj[aIndex].pInventory[pos].m_Durability += lpItem->m_Durability;

						if ( gObj[aIndex].pInventory[pos].m_Durability >= IsOverlapItem(lpItem->m_Type))
							{
								int NewDur = gObj[aIndex].pInventory[pos].m_Durability - IsOverlapItem(lpItem->m_Type);
								::gObjInventoryItemSet(aIndex, pos, -1);
								gObj[aIndex].pInventory[pos].Clear();
								this->GCInventoryItemDeleteSend(aIndex, pos, 1);
								::ItemSerialCreateSend(aIndex, 235, gObj[aIndex].X, gObj[aIndex].Y, ItemGetNumberMake(14, 111),
									lpItem->m_Level, 0, 0, 0, 0, aIndex, 0, 0, 0, 0, 0);

								g_Log.Add("[DoppelGanger] [%s][%s] Make Mirror of Dimension (Left Marker %d)",
									gObj[aIndex].AccountID, gObj[aIndex].Name, NewDur);

								if ( NewDur > 0 )
								{
									lpItem->m_State = 2;
									lpItem->Give = false;
									lpItem->live = true;
									lpItem->m_Durability = NewDur;

								//	continue;

								}
							}
							else
							{
								GCItemDurSend(aIndex,pos, gObj[aIndex].pInventory[pos].m_Durability, 0);
								
							}
						return;
					}
					else
					{
						
					}
			}
			if ( lpItem->m_Type == ITEMGET(12,144) || lpItem->m_Type == ITEMGET(12,146) ) // Mithril & Elixir
			{
				for (int j = 0; j < MAIN_INVENTORY_SIZE; j++)
				{
					BYTE btMainAtribute = lpItem->m_BonusSocketOption & 0x0F;
					int iInventoryPos = g_PentagramSystem.CheckOverlapMythrilPiece(aIndex, lpItem->m_Type, btMainAtribute);

					if (!MAIN_INVENTORY_RANGE(iInventoryPos))
					{
						break;
					}

					int Dur = gObj[aIndex].pInventory[iInventoryPos].m_Durability + lpItem->m_Durability;

					if (Dur <= IsOverlapItem(lpItem->m_Type) && MapC[map_num].ItemGive(aIndex, item_num, 1) == TRUE)
					{
						BYTE NewOption[MAX_EXOPTION_SIZE];

						::ItemIsBufExOption(NewOption, (lpItem != NULL) ? (CItem*)&lpItem->m_Number : NULL);

						g_Log.Add("[%s][%s][%d]%d/%d Get serial:%I64d [%s][%d][%d][%d][%d][%d] dur:[%d]Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d] 380:[%d] HO:[%d,%d] E:[%d]", gObj[aIndex].AccountID, gObj[aIndex].Name, map_num,
							gObj[aIndex].X, gObj[aIndex].Y, lpItem->m_Number, szItemName, type,
							level, lpItem->m_Option1, lpItem->m_Option2, lpItem->m_Option3, (int)lpItem->m_Durability,
							NewOption[0], NewOption[1], NewOption[2], NewOption[3], NewOption[4], NewOption[5],
							NewOption[6], lpItem->m_SetOption, lpItem->m_ItemOptionEx >> 7,
							g_kJewelOfHarmonySystem.GetItemStrengthenOption((lpItem) ? ((CItem *)&lpItem->m_Number) : NULL), g_kJewelOfHarmonySystem.GetItemOptionLevel((lpItem) ? ((CItem *)&lpItem->m_Number) : NULL),
							lpItem->m_BonusSocketOption);

						pResult.result = -3;
						IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

						gObj[aIndex].pInventory[iInventoryPos].m_Durability += lpItem->m_Durability;

						if (gObj[aIndex].pInventory[iInventoryPos].m_Durability < IsOverlapItem(lpItem->m_Type))
						{
							this->GCItemDurSend(aIndex, iInventoryPos, gObj[aIndex].pInventory[iInventoryPos].m_Durability, 0);
							return;
						}

						int NewDur = gObj[aIndex].pInventory[iInventoryPos].m_Durability - IsOverlapItem(lpItem->m_Type);
						::gObjInventoryItemSet(aIndex, iInventoryPos, -1);
						gObj[aIndex].pInventory[iInventoryPos].Clear();
						this->GCInventoryItemDeleteSend(aIndex, iInventoryPos, 1);

						if (lpItem->m_Type == ITEMGET(12, 144))
						{
							ItemSerialCreateSend(aIndex, 233, gObj[aIndex].X, gObj[aIndex].Y, ItemGetNumberMake(12, 148), 0, 0, 0, 0, 0,
								aIndex, 0, 0, 0, 0, lpItem->m_BonusSocketOption);

							g_Log.Add("[Pentagram] [%s][%s] Make Mythril (Left:%d)", gObj[aIndex].AccountID, gObj[aIndex].Name, NewDur);
						}

						else if (lpItem->m_Type == ITEMGET(12, 146))
						{
							ItemSerialCreateSend(aIndex, 233, gObj[aIndex].X, gObj[aIndex].Y, ItemGetNumberMake(12, 149), 0, 0, 0, 0, 0,
								aIndex, 0, 0, 0, 0, 0);

							g_Log.Add("[Pentagram] [%s][%s] Make elixir (Left:%d)", gObj[aIndex].AccountID, gObj[aIndex].Name, NewDur);
						}

						if (NewDur <= 0)
							return;

						lpItem->m_State = 2;
						lpItem->Give = 0;
						lpItem->live = 1;
						lpItem->m_Durability = NewDur;
					}
				}
			}

			if ( lpItem->m_Type == ITEMGET(13,145) ) // Spirit Map
			{
				int pos = gObjOverlapItemUsingDur((lpItem)?((CItem *)&lpItem->m_Number):NULL, map_num, item_num, aIndex, IsOverlapItem(lpItem->m_Type), ITEMGET(13,145), 0);
					if ( MAIN_INVENTORY_RANGE(pos) )
					{
						pResult.result = -3;

						IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);			
						gObj[aIndex].pInventory[pos].m_Durability += lpItem->m_Durability;

						if ( gObj[aIndex].pInventory[pos].m_Durability >= IsOverlapItem(lpItem->m_Type))
							{
								int NewDur = gObj[aIndex].pInventory[pos].m_Durability - IsOverlapItem(lpItem->m_Type);
								::gObjInventoryItemSet(aIndex, pos, -1);
								gObj[aIndex].pInventory[pos].Clear();
								this->GCInventoryItemDeleteSend(aIndex, pos, 1);
								::ItemSerialCreateSend(aIndex, 235, gObj[aIndex].X, gObj[aIndex].Y, ItemGetNumberMake(13, 146),
									lpItem->m_Level, 0, 0, 0, 0, aIndex, 0, 0, 0, 0, 0);

								g_Log.Add("[Attribute System] [%s][%s] Make Spirit Map (Left Fragments %d)",
									gObj[aIndex].AccountID, gObj[aIndex].Name, NewDur);

								if ( NewDur > 0 )
								{
									lpItem->m_State = 2;
									lpItem->Give = false;
									lpItem->live = true;
									lpItem->m_Durability = NewDur;

								//	continue;

								}
							}
							else
							{
								GCItemDurSend(aIndex,pos, gObj[aIndex].pInventory[pos].m_Durability, 0);
								
							}
						return;
					}
					else
					{
						
					}
			}
			if ( lpItem->m_Type == ITEMGET(14,101) ) // Imperial Fort
			{
					int pos = gObjOverlapItemUsingDur((lpItem)?((CItem *)&lpItem->m_Number):NULL, map_num, item_num, aIndex, IsOverlapItem(lpItem->m_Type), ITEMGET(14,101), 0);
					if ( MAIN_INVENTORY_RANGE(pos) )
					{
						pResult.result = -3;

						IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);			
						gObj[aIndex].pInventory[pos].m_Durability += lpItem->m_Durability;

						if ( gObj[aIndex].pInventory[pos].m_Durability >= IsOverlapItem(lpItem->m_Type))
							{
								int NewDur = gObj[aIndex].pInventory[pos].m_Durability - IsOverlapItem(lpItem->m_Type);
								::gObjInventoryItemSet(aIndex, pos, -1);
								gObj[aIndex].pInventory[pos].Clear();
								this->GCInventoryItemDeleteSend(aIndex, pos, 1);
								::ItemSerialCreateSend(aIndex, 235, gObj[aIndex].X, gObj[aIndex].Y, ItemGetNumberMake(14, 102),
									lpItem->m_Level, 0, 0, 0, 0, aIndex, 0, 0, 0, 0, 0);

								g_Log.Add("[Imperial] [%s][%s] Make Gaion's order letter (Left Scraps %d)",
									gObj[aIndex].AccountID, gObj[aIndex].Name, NewDur);

								if ( NewDur > 0 )
								{
									lpItem->m_State = 2;
									lpItem->Give = false;
									lpItem->live = true;
									lpItem->m_Durability = NewDur;

								//	continue;

								}
							}
							else
							{
								GCItemDurSend(aIndex,pos, gObj[aIndex].pInventory[pos].m_Durability, 0);
								
							}
						return;
					}
					else
					{
						
					}
			}

			if ( lpItem->m_Type == ITEMGET(14,100) ) // Lucky Coin
			{
					int pos = gObjOverlapItemUsingDur((lpItem)?((CItem *)&lpItem->m_Number):NULL, map_num, item_num, aIndex, IsOverlapItem(lpItem->m_Type), ITEMGET(14,100), 0);
					if ( MAIN_INVENTORY_RANGE(pos) )
					{
						pResult.result = -3;

						IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);			
						gObj[aIndex].pInventory[pos].m_Durability += lpItem->m_Durability;

						GCItemDurSend(aIndex,pos, gObj[aIndex].pInventory[pos].m_Durability, 0);
								
						return;
					}
					else
					{
						
					}
			}

			if ( lpItem->m_Type == ITEMGET(14,21) && lpItem->m_Level == 3) // Sign of Lord
			{
				for ( int n=0;n<MAIN_INVENTORY_SIZE;n++)
				{
					int pos = ::g_CastleSiegeSync.CheckOverlapCsMarks(aIndex);

					if ( MAIN_INVENTORY_RANGE(pos) )
					{
						int Dur = gObj[aIndex].pInventory[pos].m_Durability + lpItem->m_Durability;

						if ( Dur <= IsOverlapItem(lpItem->m_Type))
						{
							if ( MapC[map_num].ItemGive(aIndex, item_num, true) == TRUE )
							{
								pResult.result = -3;
								IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
								gObj[aIndex].pInventory[pos].m_Durability += lpItem->m_Durability;
								GCItemDurSend( aIndex, pos, gObj[aIndex].pInventory[pos].m_Durability,0);
								return;
							}
						}
						else
						{
							lpItem->m_Durability = Dur - IsOverlapItem(lpItem->m_Type);
							gObj[aIndex].pInventory[pos].m_Durability = IsOverlapItem(lpItem->m_Type);
							GCItemDurSend(aIndex, pos, gObj[aIndex].pInventory[pos].m_Durability, 0);
						}

					}
					else
					{
						break;
					}
				}
			}

			if ( lpItem->m_Type == ITEMGET(14,90)) // Golden cherry blossom
			{
				int pos = gObjOverlapItemUsingDur((lpItem)?((CItem *)&lpItem->m_Number):NULL, map_num, item_num, aIndex, IsOverlapItem(lpItem->m_Type), ITEMGET(14,90), 0);
				if ( MAIN_INVENTORY_RANGE(pos) )
				{
					pResult.result = -3;
				
					IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);			
				
					gObj[aIndex].pInventory[pos].m_Durability += lpItem->m_Durability;
					
					GCItemDurSend(aIndex,pos, gObj[aIndex].pInventory[pos].m_Durability, 0);
								
					return;
				}

				else
				{
						
				}
			}

			if ( lpItem->m_Type == ITEMGET(14,89)) // Red cherry blossom
			{
				int pos = gObjOverlapItemUsingDur((lpItem)?((CItem *)&lpItem->m_Number):NULL, map_num, item_num, aIndex, IsOverlapItem(lpItem->m_Type), ITEMGET(14,89), 0);
				if ( MAIN_INVENTORY_RANGE(pos) )
				{
					pResult.result = -3;
				
					IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);			
				
					gObj[aIndex].pInventory[pos].m_Durability += lpItem->m_Durability;
					
					GCItemDurSend(aIndex,pos, gObj[aIndex].pInventory[pos].m_Durability, 0);
								
					return;
				}

				else
				{
						
				}
			}

			if ( lpItem->m_Type == ITEMGET(14,88)) // White cherry blossom
			{
				int pos = gObjOverlapItemUsingDur((lpItem)?((CItem *)&lpItem->m_Number):NULL, map_num, item_num, aIndex, IsOverlapItem(lpItem->m_Type), ITEMGET(14,88), 0);
				if ( MAIN_INVENTORY_RANGE(pos) )
				{
					pResult.result = -3;
				
					IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);			
				
					gObj[aIndex].pInventory[pos].m_Durability += lpItem->m_Durability;
					
					GCItemDurSend(aIndex,pos, gObj[aIndex].pInventory[pos].m_Durability, 0);
								
					return;
				}

				else
				{
						
				}
			}

			if ( lpItem->m_Type == ITEMGET(14,153)) // White cherry blossom
			{
				int pos = gObjOverlapItemUsingDur((lpItem)?((CItem *)&lpItem->m_Number):NULL, map_num, item_num, aIndex, IsOverlapItem(lpItem->m_Type), ITEMGET(14,153), 0);
				if ( MAIN_INVENTORY_RANGE(pos) )
				{
					pResult.result = -3;
				
					IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);			
				
					gObj[aIndex].pInventory[pos].m_Durability += lpItem->m_Durability;
					
					GCItemDurSend(aIndex,pos, gObj[aIndex].pInventory[pos].m_Durability, 0);
								
					return;
				}

				else
				{
						
				}
			}

			if ( lpItem->m_Type == ITEMGET(14,154)) // White cherry blossom
			{
				int pos = gObjOverlapItemUsingDur((lpItem)?((CItem *)&lpItem->m_Number):NULL, map_num, item_num, aIndex, IsOverlapItem(lpItem->m_Type), ITEMGET(14,154), 0);
				if ( MAIN_INVENTORY_RANGE(pos) )
				{
					pResult.result = -3;
				
					IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);			
				
					gObj[aIndex].pInventory[pos].m_Durability += lpItem->m_Durability;
					
					GCItemDurSend(aIndex,pos, gObj[aIndex].pInventory[pos].m_Durability, 0);
								
					return;
				}

				else
				{
						
				}
			}

			if ( lpItem->m_Type == ITEMGET(14,155)) // White cherry blossom
			{
				int pos = gObjOverlapItemUsingDur((lpItem)?((CItem *)&lpItem->m_Number):NULL, map_num, item_num, aIndex, IsOverlapItem(lpItem->m_Type), ITEMGET(14,155), 0);
				if ( MAIN_INVENTORY_RANGE(pos) )
				{
					pResult.result = -3;
				
					IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);			
				
					gObj[aIndex].pInventory[pos].m_Durability += lpItem->m_Durability;
					
					GCItemDurSend(aIndex,pos, gObj[aIndex].pInventory[pos].m_Durability, 0);
								
					return;
				}

				else
				{
						
				}
			}

			if ( lpItem->m_Type == ITEMGET(14,156)) // White cherry blossom
			{
				int pos = gObjOverlapItemUsingDur((lpItem)?((CItem *)&lpItem->m_Number):NULL, map_num, item_num, aIndex, IsOverlapItem(lpItem->m_Type), ITEMGET(14,156), 0);
				if ( MAIN_INVENTORY_RANGE(pos) )
				{
					pResult.result = -3;
				
					IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);			
				
					gObj[aIndex].pInventory[pos].m_Durability += lpItem->m_Durability;
					
					GCItemDurSend(aIndex,pos, gObj[aIndex].pInventory[pos].m_Durability, 0);
								
					return;
				}

				else
				{
						
				}
			}

			if (g_LuckyItemManager.IsLuckyItemEquipment(lpItem->m_Type) && g_ConfigRead.data.common.AllowGetLuckyItemGround == false)
			{
				char szSetItemName[48] = { 0 };
				int tmpSetOption = 0;
				BYTE NewOption[8] = { 0 };

				ItemIsBufExOption(NewOption, (lpItem != NULL) ? (CItem*)&lpItem->m_Number : NULL);

				if (gSetItemOption.IsSetItem(lpItem->m_Type))
				{
					if (lpItem->m_SetOption & 1)
					{
						tmpSetOption = 1;
					}
				
					else
					{
						if (lpItem->m_SetOption & 2)
						{
							tmpSetOption = 2;
						}
					}

					strcpy(szSetItemName, gSetItemOption.GetSetOptionName(type, tmpSetOption));
				}

				g_Log.Add("[%s][%s][%d]%d/%d Try Get LuckyItem serial:%I64d [%s][%d][%d][%d][%d][%d] dur:[%d]Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d] 380:[%d] HO:[%d,%d]",
					gObj[aIndex].AccountID, gObj[aIndex].Name, map_num, gObj[aIndex].X, gObj[aIndex].Y, lpItem->m_Number, szSetItemName,
					type, level, lpItem->m_Option1, lpItem->m_Option2, lpItem->m_Option3, (INT)lpItem->m_Durability,
					NewOption[0], NewOption[1], NewOption[2], NewOption[3], NewOption[4], NewOption[5], NewOption[6],
					lpItem->m_SetOption, lpItem->m_ItemOptionEx >> 7,
					g_kJewelOfHarmonySystem.GetItemStrengthenOption((lpItem != NULL) ? (CItem*)&lpItem->m_Number : NULL),
					g_kJewelOfHarmonySystem.GetItemOptionLevel((lpItem != NULL) ? (CItem*)&lpItem->m_Number : NULL));

					pResult.result = -1;

				IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
				this->GCServerMsgStringSend(Lang.GetText(0,519), aIndex, 1);
				return;
			}

			if (lpItem->m_QuestItem == true && g_QuestExpManager.IsQuestItemAtt(lpItem->m_Type, 1) == true)
			{
				for (int n = 12; n < INVENTORY_SIZE; n++)
				{
					int pos = -1;

					if (gObj[aIndex].pInventory[n].m_Type == lpItem->m_Type &&
						gObj[aIndex].pInventory[n].m_Durability < IsOverlapItem(lpItem->m_Type))
					{
						pos = n;
					}

					if (INVENTORY_RANGE(pos) == true)
					{
						if (gObj[aIndex].pInventory[pos].m_Durability + lpItem->m_Durability > IsOverlapItem(lpItem->m_Type))
						{
							lpItem->m_Durability = gObj[aIndex].pInventory[pos].m_Durability + lpItem->m_Durability - IsOverlapItem(lpItem->m_Type);
							gObj[aIndex].pInventory[pos].m_Durability = IsOverlapItem(lpItem->m_Type);
							this->GCItemDurSend(aIndex, pos, gObj[aIndex].pInventory[pos].m_Durability, 0);
						}

						else
						{
							if (MapC[map_num].ItemGive(aIndex, item_num, 1) == TRUE)
							{
								pResult.result = -3;
								IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

								gObj[aIndex].pInventory[pos].m_Durability += lpItem->m_Durability;
								GCItemDurSend(aIndex, pos, gObj[aIndex].pInventory[pos].m_Durability, 0);
								return;
							}
						}
					}
				}
			}

			pResult.result = ::gObjInventoryInsertItemTemp(&gObj[aIndex], lpItem);
			
			if ( pResult.result != 0xFF )
			{
				ItemByteConvert((LPBYTE)&pResult.Data[0], lpItem->m_Type, lpItem->m_Option1, lpItem->m_Option2,
					lpItem->m_Option3, lpItem->m_Level, lpItem->m_Durability, lpItem->m_NewOption,
					lpItem->m_SetOption, lpItem->m_JewelOfHarmonyOption, lpItem->m_ItemOptionEx, lpItem->m_SocketOption, lpItem->m_BonusSocketOption, 0);

				if ( MapC[map_num].ItemGive(aIndex, item_num, false) == TRUE )
				{
					BYTE pos = ::gObjInventoryInsertItem(aIndex, lpItem);

					if ( pos == 0xFF )
						pResult.result = -1;

					IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

					if ( pos == 0xFF )
					{
						::GCItemListSend(aIndex);
						g_PeriodItemEx.OnRequestPeriodItemList(&gObj[aIndex]);
					}
					else
					{
						BYTE NewOption[MAX_EXOPTION_SIZE];

						::ItemIsBufExOption(NewOption, (lpItem != NULL)?(CItem*)&lpItem->m_Number:NULL);

						g_Log.Add("[%s][%s][%d]%d/%d Get serial:%I64d [%s][%d][%d][%d][%d][%d] dur:[%d]Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d] 380:[%d] HO:[%d,%d] E:[%d]", gObj[aIndex].AccountID, gObj[aIndex].Name, map_num,
							gObj[aIndex].X, gObj[aIndex].Y, lpItem->m_Number, szItemName, type,
							level, lpItem->m_Option1, lpItem->m_Option2, lpItem->m_Option3, (INT)lpItem->m_Durability,
							NewOption[0], NewOption[1], NewOption[2], NewOption[3], NewOption[4], NewOption[5],
							NewOption[6], lpItem->m_SetOption, lpItem->m_ItemOptionEx>>7,
							g_kJewelOfHarmonySystem.GetItemStrengthenOption((lpItem)?((CItem *)&lpItem->m_Number):NULL), g_kJewelOfHarmonySystem.GetItemOptionLevel((lpItem)?((CItem *)&lpItem->m_Number):NULL),
							lpItem->m_BonusSocketOption);
						this->GCSendGetItemInfoForParty(aIndex, lpItem);

						if ( BC_MAP_RANGE(map_num) )
						{
							int iBridgeIndex = g_BloodCastle.GetBridgeIndex(map_num);

							if ( g_BloodCastle.CheckQuestItemSerial(iBridgeIndex, lpItem) )
							{
								g_BloodCastle.CatchQuestItemByUser(iBridgeIndex, aIndex, lpItem->m_Level);
							}
						}

						if (IT_MAP_RANGE(map_num) && lpItem->m_Type == ITEMGET(14, 223))
						{
							if (g_ConfigRead.server.GetServerType() == SERVER_MARKET)
							{
								g_IT_Event.SetRelicsInventoryPos(gObj[aIndex].MapNumber, aIndex, pos);
								g_IT_Event.SendRelicsUserInfo(&gObj[aIndex]);

								g_Log.Add("[ ITR ] (%d) (Account:%s, Name:%s) picked up Relics Item(serial:%I64d)",
									map_num - 44, gObj[aIndex].AccountID, gObj[aIndex].Name, lpItem->m_Number);
							}
						}

						if ( g_ConfigRead.server.GetServerType() == SERVER_BATTLECORE )
						{
							if (gObj[aIndex].MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL)
							{
								g_ChaosCastleFinal.SetUBFGetReward(aIndex, lpItem->m_Type, lpItem->m_Number, pos);
							}

							else if (gObj[aIndex].MapNumber == MAP_INDEX_DEVILSQUARE_FINAL)
							{
								g_DevilSquareFinal.SetUBFGetReward(aIndex, lpItem->m_Type, lpItem->m_Number, pos);
							}

							else if (CC_MAP_RANGE(gObj[aIndex].MapNumber) == TRUE)
							{
								g_ChaosCastle.SetUBFGetReward(aIndex, lpItem->m_Type, lpItem->m_Number, pos);
							}
						}

						if ( g_ArcaBattle.IsArcaBattleServer() == TRUE && lpItem->m_Type == ITEMGET(13,147) )
						{
							g_ArcaBattle.BootyItemGetCnt(&gObj[aIndex]);
						}

						if ( lpItem->m_PeriodItemDuration > 0 )
						{
							g_PeriodItemEx.SetPeriodItemInfo(&gObj[aIndex], lpItem->m_Type, lpItem->m_Number, lpItem->m_PeriodItemDuration);

							this->GCInventoryItemOneSend(aIndex, pos);
							g_PeriodItemEx.SendPeriodItemInfoOnce(&gObj[aIndex], &gObj[aIndex].pInventory[pos]);
						}
					}
				}
				else
				{
					pResult.result = -1;
					pResult.h.size -= sizeof(pResult.Data);

					IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
				}
			}
			else
			{
				pResult.result = -1;
				pResult.h.size -= sizeof(pResult.Data);

				IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
			}
		}
	}
	else
	{
		pResult.h.size -= sizeof(pResult.Data);
		IOCP.DataSend(aIndex, (UCHAR*)&pResult, pResult.h.size);
	}
}

bool GameProtocol::CGItemDropRequest(PMSG_ITEMTHROW * lpMsg, int aIndex, BOOL drop_type)
{
	BOOL CopyItem = FALSE;
	BYTE NewOption[MAX_EXOPTION_SIZE];

	if ( !PacketCheckTime(aIndex) )
		return FALSE;

	if ( gObj[aIndex].Type != OBJ_USER )
	{
		return FALSE;
	}

	PMSG_ITEMTHROW_RESULT pResult;

	PHeadSetB((LPBYTE)&pResult, 0x23, sizeof(pResult));
	pResult.Result = true;
	pResult.Ipos = lpMsg->Ipos;

	if ( gObj[aIndex].m_IfState.use != 0 )
	{
		if ( gObj[aIndex].m_IfState.type != 3 )
		{
			pResult.Result = false;
			return FALSE;
		}
	}

	if ( gObj[aIndex].m_PlayerData->m_bSecurityCheck == false )
	{
		MsgOutput(aIndex, Lang.GetText(0,822));
		pResult.Result = false;
	}

	if ( gObj[aIndex].CloseType != -1 )
		pResult.Result = false;

	if ( gObj[aIndex].m_bMapSvrMoveQuit == true )
	{
		g_Log.Add("[ANTI-HACK][Item Duplication] - Item Drop during MapServer Move [%s][%s]",
			gObj[aIndex].AccountID, gObj[aIndex].Name);
		pResult.Result = false;
	}

	if ( !::gObjFixInventoryPointer(aIndex))
		g_Log.Add("[Fix Inv.Ptr] False Location - %s, %d", __FILE__, __LINE__);

	if ( gObj[aIndex].pTransaction == 1 )
	{
		g_Log.Add("[%s][%s] CGItemDropRequest() Failed : Transaction == 1, IF_TYPE : %d",
			gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_IfState.type);
		pResult.Result = false;
		
		return false;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if ( lpMsg->Ipos == 0xFF )	// Null Position
	{
		g_Log.Add("error:%s %d", __FILE__, __LINE__);
		pResult.Result = false;
	}

	if ( lpMsg->Ipos < 0 || lpMsg->Ipos > MAIN_INVENTORY_SIZE-1)	// Out of Bounds
		pResult.Result = false;

	if (!gObjCanItemTouch(lpObj, 0))
		pResult.Result = false;

	if (!IsDumpItem(lpObj->pInventory[lpMsg->Ipos].m_Type) && (lpObj->pInventory[lpMsg->Ipos].m_Type != ITEMGET(13, 20) || !lpObj->pInventory[lpMsg->Ipos].m_Level))
		pResult.Result = false;

	if (g_LuckyItemManager.IsLuckyItemEquipment(lpObj->pInventory[lpMsg->Ipos].m_Type) || g_LuckyItemManager.IsLuckyItemTicket(lpObj->pInventory[lpMsg->Ipos].m_Type))
	{
		pResult.Result = false;
	}

	// Icarus Preventions about Dinorant
	if ( gObj[aIndex].MapNumber == MAP_INDEX_ICARUS)
	{
		if ( lpMsg->Ipos == lpObj->m_btInvenPetPos )
		{
			if ( lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(13,3) ) // Dinorant
			{
				if( lpObj->pInventory[7].IsItem() == FALSE )
				{
					pResult.Result = false;
				}
			}
		}
		else if ( lpMsg->Ipos == 7 )
		{
			if ( lpObj->pInventory[lpObj->m_btInvenPetPos].m_Type != ITEMGET(13,3) ) // Dinorant 
			{
				pResult.Result = false;
			}
		}
	}

	if ( lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(12,30) ||
		 lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(12,31) ||
		 lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(13,36) ||
		 lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(13,37) )
	{
		pResult.Result = false;
	}

	if (GetWingType(lpObj->pInventory[lpMsg->Ipos].m_Type, WCF_ITEMNUMBER) != NONE_WING && g_ConfigRead.pk.bPkPenaltyDisable== FALSE)
	{
		pResult.Result = false;
	}
	if( g_ConfigRead.data.common.joinmuRemoveItemSellDropRestriction == false)
	{
		if ( (lpObj->pInventory[lpMsg->Ipos].m_Type >= ITEMGET(13,0) && lpObj->pInventory[lpMsg->Ipos].m_Type <= ITEMGET(13,3))
			||  lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(14,13) ||
			lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(14,14) ||
			lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(14,16) ||
			(lpObj->pInventory[lpMsg->Ipos].m_Type >= ITEMGET(12,0)
			&& lpObj->pInventory[lpMsg->Ipos].m_Type <= ITEMGET(12,6)) 	||
			(lpObj->pInventory[lpMsg->Ipos].m_Type >= ITEMGET(12,36)
			&& lpObj->pInventory[lpMsg->Ipos].m_Type <= ITEMGET(12,40)) 
			|| lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(12,15) 
			|| (lpObj->pInventory[lpMsg->Ipos].m_Level > g_ConfigRead.pk.iPkMaxLevelItemDrop 
			&& lpObj->pInventory[lpMsg->Ipos].m_Type < ITEMGET(12,0)) 
			||  (lpObj->pInventory[lpMsg->Ipos].IsSetItem() != FALSE && !g_ConfigRead.pk.bPkDropExpensiveItems)
			||  (lpObj->pInventory[lpMsg->Ipos].IsExtItem() != FALSE && !g_ConfigRead.pk.bPkDropExpensiveItems))
		{
			if ( g_ConfigRead.pk.bPkPenaltyDisable == FALSE )
			{
				pResult.Result = false;
			}
		}
	}
	if ( g_kJewelOfHarmonySystem.IsStrengthenByJewelOfHarmony(&lpObj->pInventory[lpMsg->Ipos]) == TRUE )
	{
		pResult.Result = false;
		this->GCServerMsgStringSend(Lang.GetText(0,272), lpObj->m_Index, 1);
	}

	if ( g_kJewelOfHarmonySystem.IsJewelOfHarmonyOriginal(lpObj->pInventory[lpMsg->Ipos].m_Type) ||
		 g_kJewelOfHarmonySystem.IsJewelOfHarmonyPurity(lpObj->pInventory[lpMsg->Ipos].m_Type)   ||
		 g_kJewelOfHarmonySystem.IsJewelOfHarmonySmeltingItems(lpObj->pInventory[lpMsg->Ipos].m_Type) )
	{
		pResult.Result = false;
	}

	if ( lpObj->pInventory[lpMsg->Ipos].m_Type == ITEMGET(13,39) )
	{
		pResult.Result = false;
	}

	if ( lpObj->pInventory[lpMsg->Ipos].IsPeriodItem() == TRUE )
	{
		pResult.Result = false;
	} 

	if ( g_NewPVP.IsDuel(*lpObj) )
	{
		pResult.Result = false;
	}

	if ( IsJumpingEventItem(lpObj->pInventory[lpMsg->Ipos].m_Type) == TRUE)
	{
		pResult.Result = false;
	}

	if ( pResult.Result == 1 )
	{
		::ItemIsBufExOption(NewOption, &lpObj->pInventory[lpMsg->Ipos]);

		int map = lpObj->MapNumber;
		int type = lpObj->pInventory[lpMsg->Ipos].m_Type;
		int level = lpObj->pInventory[lpMsg->Ipos].m_Level;
		float dur = lpObj->pInventory[lpMsg->Ipos].m_Durability;
		BOOL bIsItem = lpObj->pInventory[lpMsg->Ipos].IsItem();
		BYTE Option1 = lpObj->pInventory[lpMsg->Ipos].m_Option1;
		BYTE Option2 = lpObj->pInventory[lpMsg->Ipos].m_Option2;
		BYTE Option3 = lpObj->pInventory[lpMsg->Ipos].m_Option3;
		BYTE NOption = lpObj->pInventory[lpMsg->Ipos].m_NewOption;
		UINT64 serial = lpObj->pInventory[lpMsg->Ipos].m_Number;
		int PetLevel = lpObj->pInventory[lpMsg->Ipos].m_PetItem_Level;
		UINT64 PetExp = lpObj->pInventory[lpMsg->Ipos].m_PetItem_Exp;
		BYTE SOption = lpObj->pInventory[lpMsg->Ipos].m_SetOption;
		BYTE btItemEffectEx = lpObj->pInventory[lpMsg->Ipos].m_ItemOptionEx;
		UINT64 serial2 = lpObj->pInventory[lpMsg->Ipos].m_Number;
		char szItemName[50] = "Item";
		BYTE JOHOption = g_kJewelOfHarmonySystem.GetItemStrengthenOption(&lpObj->pInventory[lpMsg->Ipos]);
		BYTE JOHOptionLevel = g_kJewelOfHarmonySystem.GetItemOptionLevel(&lpObj->pInventory[lpMsg->Ipos]);

		//Season 4 add-on
		BYTE SocketOption[5];
		BYTE SocketIndex;
		g_SocketOptionSystem.GetSocketOption(&lpObj->pInventory[lpMsg->Ipos], SocketOption, SocketIndex);

		if ( gObjCheckSerial0ItemList(&lpObj->pInventory[lpMsg->Ipos]) != FALSE )
		{
			MsgOutput(lpObj->m_Index, Lang.GetText(0,259));
			g_Log.Add("[ANTI-HACK][Serial 0 Item] [ItemDrop] (%s)(%s) Item(%s) Pos(%d)",
				lpObj->AccountID, lpObj->Name, lpObj->pInventory[lpMsg->Ipos].GetName(),
				lpMsg->Ipos);

			CopyItem = TRUE;
			pResult.Result = false;
			bIsItem = FALSE;
		}

		
		if ( !::gObjInventorySearchSerialNumber(lpObj, serial2))	// ANTI DUPE SYSTEM
		{
			CopyItem = TRUE;
			pResult.Result = false;
			bIsItem = FALSE;
		}

		if ( bIsItem == TRUE )
		{
			strcpy(szItemName, lpObj->pInventory[lpMsg->Ipos].GetName());

			if (g_BagManager.SearchAndUseBag(lpObj->m_Index, BAG_COMMON, type, level) == true)
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				g_Log.Add("[%s][%s][Used Item Bag] : (%d)(X:%d/Y:%d)",
					lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y);
			}

			else if ( type == ITEMGET(14,51) )
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				FireworksOpenEven(&gObj[aIndex]);

				g_Log.Add("[%s][%s][Used Star Fireworks Effect] : (%d)(X:%d/Y:%d)",
				lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y);

			}

			else if ( type == ITEMGET(14,63) )
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				FireworksOpenEven(&gObj[aIndex]);

				g_Log.Add("[%s][%s][Used Pounch Fireworks Effect] : (%d)(X:%d/Y:%d)",
				lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y);

			}

			else if (type == ITEMGET(14, 210))
			{
				if (NewYearSummonMonster(lpObj, 647) == FALSE)
				{
					pResult.Result = false;
					IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

					return FALSE;
				}

				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
			}

			else if (type == ITEMGET(14, 211))
			{
				if (NewYearSummonMonster(lpObj, 648) == FALSE)
				{
					pResult.Result = false;
					IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

					return FALSE;
				}

				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
			}

			else if (type == ITEMGET(14, 212))
			{
				if (NewYearSummonMonster(lpObj, 649) == FALSE)
				{
					pResult.Result = false;
					IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

					return FALSE;
				}

				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
			}

			else if (type == ITEMGET(14, 213))
			{
				if (NewYearSummonMonster(lpObj, 650) == FALSE)
				{
					pResult.Result = false;
					IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

					return FALSE;
				}

				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
			}

			else if (type == ITEMGET(14, 217))
			{
				if (SummonGoldColossusMonster(lpObj, 653, 1, 0) == FALSE)
				{
					pResult.Result = false;
					IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

					return FALSE;
				}

				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
			}

			else if (type == ITEMGET(14, 218))
			{
				if (SummonGoldColossusMonster(lpObj, 654, 1, 0) == FALSE)
				{
					pResult.Result = false;
					IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

					return FALSE;
				}

				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
			}

			else if (type == ITEMGET(14, 219))
			{
				if (SummonGoldColossusMonster(lpObj, 655, 1, 0) == FALSE)
				{
					pResult.Result = false;
					IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

					return FALSE;
				}

				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
			}

			else if (type == ITEMGET(14, 220))
			{
				if (SummonGoldColossusMonster(lpObj, 656, 1, 0) == FALSE)
				{
					pResult.Result = false;
					IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

					return FALSE;
				}

				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
			}

			else if (type == ITEMGET(14, 221))
			{
				if (SummonGoldColossusMonster(lpObj, 657, 1, 0) == FALSE)
				{
					pResult.Result = false;
					IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

					return FALSE;
				}
				
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
			}

			else if (type == ITEMGET(14, 234))
			{
				if (SummonPetEventMonster(lpObj) == FALSE)
				{
					pResult.Result = false;
					IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

					return FALSE;
				}

				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
			}

			else if ( type == ITEMGET(12,26) )
			{
				switch ( level )
				{
					case 0:
						{
							gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
							pResult.Result = true;
							int ltype = ITEMGET(12,26);
							int randor = rand()%100;
							int Level;

							if ( randor < 10 )
							{
								Level = 3;
							}
							else if ( randor < 45 )
							{
								Level = 1;
							}
							else
							{
								Level = 2;
							}

							ItemSerialCreateSend(aIndex, lpObj->MapNumber, lpObj->X, lpObj->Y,
								ltype, Level, 1, 0, 0, 0, lpObj->m_Index, 0, 0, 0, 0, 0);
							
							g_Log.Add("[Hidden TreasureBox Event] [%s][%s] Throw Mysterious Bead : Drop (%d) - serial:%I64d",
								lpObj->AccountID, lpObj->Name, Level, serial);
						}
						break;
					case 1: case 2: case 3:
						{
							gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
							pResult.Result = true;

							PMSG_SERVERCMD ServerCmd;

							PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));
							ServerCmd.CmdType = 0;
							ServerCmd.X = lpObj->X;
							ServerCmd.Y = lpObj->Y;

							MsgSendV2(lpObj, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
							IOCP.DataSend(lpObj->m_Index, (LPBYTE)&ServerCmd, sizeof(ServerCmd));
													
							g_Log.Add("[Hidden TreasureBox Event] [%s][%s] Throw Crystal (%d) - serial:%I64d",
								lpObj->AccountID, lpObj->Name, level, serial);
						}
						break;
					case 4:

						break;
					case 5:
						g_Log.Add("[Hidden TreasureBox Event] [%s][%s] Throw Surprise Treasure Box - serial:%I64d",
							lpObj->AccountID, lpObj->Name,  serial);
						gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
						pResult.Result = true;

						if ( (rand()%10000) < g_iHiddenTreasureBoxOfflineRate )
						{
							EGReqRegHTOfflineGift(lpObj->m_Index);
						}
						else
						{
							int money = 1000000;
							MapC[lpObj->MapNumber].MoneyItemDrop(money, lpObj->X, lpObj->Y);
							
							g_Log.Add("[Hidden TreasureBox Event] [%s][%s] Event ZenDrop : %d : (%d)(%d/%d)",
								lpObj->AccountID, lpObj->Name, money, lpObj->MapNumber, lpObj->X, lpObj->Y);
						}
						break;
				}
			}
			else if ( type == ITEMGET(13,20) && level == 1 ) // Wizard Ring A
			{
				if ( gObj[aIndex].Level >= 40 )
				{
					::gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
					g_BagManager.SearchAndUseBag(aIndex, BAG_EVENT, EVENTBAG_WARRIORRING_1, aIndex);
				}
				else
				{
					PMSG_NOTICE pNotice;

					TNotice::MakeNoticeMsg((TNotice *)&pNotice, 10, Lang.GetText(0,106));
					TNotice::SetNoticeProperty((TNotice *)&pNotice, 10, _ARGB(255, 128, 149, 196), 1, 0, 20);
					TNotice::SendNoticeToUser(aIndex, (TNotice *)&pNotice);
					pResult.Result = false;
					IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

					return FALSE;
				}
			}
			else if ( type == ITEMGET(13,20) && level == 2 ) // Wizard Ring B
			{
				if ( gObj[aIndex].Level >= 80 )
				{
					::gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
					g_BagManager.SearchAndUseBag(aIndex, BAG_EVENT, EVENTBAG_WARRIORRING_2, aIndex);
				}
				else
				{
					PMSG_NOTICE pNotice;

					TNotice::MakeNoticeMsg((TNotice *)&pNotice, 10, Lang.GetText(0,109));
					TNotice::SetNoticeProperty((TNotice *)&pNotice, 10, _ARGB(255, 128, 149, 196), 1, 0, 20);
					TNotice::SendNoticeToUser(aIndex, (TNotice *)&pNotice);
					pResult.Result = false;
					IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

					return FALSE;
				}
			}
			else if ( type == ITEMGET(14,28) ) // Lost Map
			{
				if ( g_KalimaGate.CreateKalimaGate(aIndex, level, lpMsg->px, lpMsg->py))
				{
					g_Log.Add("[Kalima] [%s][%s] Success to Make Kalima Gate (Lost Kalima Map Serial:%I64d)",
						gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].pInventory[lpMsg->Ipos].m_Number);

					gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				}
				else
				{
					pResult.Result = false;
					IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

					return FALSE;
				}
			}
			else if(type == ITEMGET(14,64) && drop_type != 1)
			{
				pResult.Result = false;
				IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
				return FALSE;
			}
			else if ( type == ITEMGET(14,20) && ( ((level-1<0)?FALSE:(level-1>4)?FALSE:TRUE) != FALSE ) ) // Remedy Of Love
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
			}
			else if ( type == ITEMGET(13,7) ) // Contract (Summon)
			{
				int iMonsterID = 0;

				if ( level == 0 )
					iMonsterID = 286;
				else if ( level == 1 )
					iMonsterID = 287;

				if ( g_CsNPC_Mercenary.CreateMercenary(aIndex, iMonsterID, lpMsg->px, lpMsg->py) )
				{
					::gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				}
				else
				{
					pResult.Result = false;
					IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

					return FALSE;
				}
			}
			else if ( type == ITEMGET(13,11) ) // Guardian / LifeStone
			{
				BOOL bResult = FALSE;

				if (level == 0)
					bResult = g_CsNPC_Guardian.CreateGuardian(aIndex);

				else if ( g_ConfigRead.server.GetServerType() == SERVER_CASTLE && level == 1 )
					bResult = g_CsNPC_LifeStone.CreateLifeStone(aIndex);

				if ( bResult == TRUE )
					::gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
				else
				{
					pResult.Result = false;
					IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

					return FALSE;
				}
			}

			else if ((type >= ITEMGET(14, 236) && type <= ITEMGET(14, 241)) && level == 0)
			{
				gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);

				switch (type)
				{
				case ITEMGET(14, 236):
					g_BagManager.SearchAndUseBag(aIndex, BAG_EVENT, EVENTBAG_ITR_1, aIndex);
					break;
				case ITEMGET(14, 237):
					g_BagManager.SearchAndUseBag(aIndex, BAG_EVENT, EVENTBAG_ITR_2, aIndex);
					break;
				case ITEMGET(14, 238):
					g_BagManager.SearchAndUseBag(aIndex, BAG_EVENT, EVENTBAG_ITR_3, aIndex);
					break;
				case ITEMGET(14, 239):
					g_BagManager.SearchAndUseBag(aIndex, BAG_EVENT, EVENTBAG_ITR_4, aIndex);
					break;
				case ITEMGET(14, 240):
					g_BagManager.SearchAndUseBag(aIndex, BAG_EVENT, EVENTBAG_ITR_5, aIndex);
					break;
				case ITEMGET(14, 241):
					g_BagManager.SearchAndUseBag(aIndex, BAG_EVENT, EVENTBAG_ITR_6, aIndex);
					break;
				}

				g_Log.Add("[%s][%s][%d]%d/%d Illusion Pot Item Serial:%I64d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
					lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y, serial, szItemName, type, level, Option1, Option2, Option3);
			}

			else
			{

				int OwnerIndex = aIndex;

				if ( (type >= ITEMGET(14,0) && type <= ITEMGET(14,10)) || (type == ITEMGET(4,7) ) || type ==ITEMGET(4,15) || type == ITEMGET(14,64) )
					OwnerIndex = -1;

				if ( MapC[map].ItemDrop( type, level, dur, lpMsg->px, lpMsg->py, Option1, Option2, Option3,
					NOption, SOption, serial2, OwnerIndex, PetLevel, PetExp, btItemEffectEx, SocketOption, SocketIndex, 0) == TRUE )
				{
					gObjInventoryDeleteItem(aIndex, lpMsg->Ipos);
					pResult.Result = true;

					if ( drop_type == FALSE )
					{
						g_Log.Add("[%s][%s][%d]%d/%d Throw away Serial:%I64d [%s][%d][%d][%d][%d][%d] dur:[%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d] 380:[%d] HO:[%d,%d] E:[%d]", lpObj->AccountID, lpObj->Name, lpObj->MapNumber,
							lpObj->X, lpObj->Y, serial, szItemName, type, level, Option1, Option2,
							Option3, (int)dur, NewOption[0], NewOption[1], NewOption[2], NewOption[3], NewOption[4],
							NewOption[5], NewOption[6], SOption, btItemEffectEx>>7, JOHOption, JOHOptionLevel,
							gObj[aIndex].pInventory[lpMsg->Ipos].m_BonusSocketOption);
					}
					else
					{
						g_Log.Add("[%s][%s][%d]%d/%d Throw away (Drop) Serial:%I64d [%s][%d][%d][%d][%d][%d] dur:[%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d] 380:[%d] HO:[%d,%d] E:[%d]", lpObj->AccountID, lpObj->Name, lpObj->MapNumber,
							lpObj->X, lpObj->Y, serial, szItemName, type, level, Option1, Option2,
							Option3, (int)dur, NewOption[0], NewOption[1], NewOption[2], NewOption[3], NewOption[4],
							NewOption[5], NewOption[6], SOption, btItemEffectEx, JOHOption, JOHOptionLevel,
							gObj[aIndex].pInventory[lpMsg->Ipos].m_BonusSocketOption);
					}
				}
				else
				{
					pResult.Result = false;
				}
			}
		}
		else	// bIsItem
		{
			g_Log.Add("Does not have item to throw away");
			pResult.Result = false;
		}
	}

	IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

	if ( CopyItem != FALSE )
	{
		::GCItemListSend(aIndex);
		g_PeriodItemEx.OnRequestPeriodItemList(lpObj);
	}

	if ( pResult.Result == 1 )
	{
		if ( lpMsg->Ipos < 12 )
		{
			if ( lpMsg->Ipos == 10 || lpMsg->Ipos == 11 )
			{
				gObjUseSkill.SkillChangeUse(aIndex);
			}

			::gObjMakePreviewCharSet(aIndex);

			PMSG_USEREQUIPMENTCHANGED pChange;

			PHeadSetB((LPBYTE)&pChange, 0x25, sizeof(pChange));
			pChange.NumberH = SET_NUMBERH(aIndex);
			pChange.NumberL = SET_NUMBERL(aIndex);
			ItemByteConvert(pChange.ItemInfo, lpObj->pInventory[lpMsg->Ipos]);
			pChange.ItemInfo[I_OPTION] = lpMsg->Ipos <<  4;
			pChange.ItemInfo[I_OPTION] |= LevelSmallConvert(aIndex, lpMsg->Ipos) & 0x0F;
			pChange.Element = lpObj->m_iPentagramMainAttribute;

			IOCP.DataSend(aIndex, (LPBYTE)&pChange, pChange.h.size);
			MsgSendV2(lpObj, (LPBYTE)&pChange, pChange.h.size);
		}
	}

	return pResult.Result;
}






struct PMSG_INVENTORYITEMMOVE_RESULT
{
	PBMSG_HEAD h;	// C3:24
	BYTE result;	// 3
	BYTE Pos;	// 4
	BYTE ItemInfo[MAX_ITEM_INFO];	// 5
};


void GameProtocol::GCItemMoveResultSend(int aIndex, BYTE result, BYTE pos, LPBYTE const ItemInfo)
{
	PMSG_INVENTORYITEMMOVE_RESULT pMsg;

	pMsg.h.c = 0xC3;
	pMsg.h.headcode = 0x24;
	pMsg.h.size = sizeof(pMsg);
	pMsg.result = result;
	pMsg.Pos = pos;
	pMsg.ItemInfo[I_TYPE] = ItemInfo[I_TYPE];
	pMsg.ItemInfo[I_OPTION] = ItemInfo[I_OPTION];
	pMsg.ItemInfo[I_DUR] = ItemInfo[I_DUR];
	pMsg.ItemInfo[I_NOPTION] = ItemInfo[I_NOPTION];
	pMsg.ItemInfo[I_SOPTION] = ItemInfo[I_SOPTION];
	pMsg.ItemInfo[I_380OPTION] = ItemInfo[I_380OPTION];
	pMsg.ItemInfo[I_JOHOPTION] = ItemInfo[I_JOHOPTION];
	pMsg.ItemInfo[I_SOCKET1] = ItemInfo[I_SOCKET1];
	pMsg.ItemInfo[I_SOCKET2] = ItemInfo[I_SOCKET2];
	pMsg.ItemInfo[I_SOCKET3] = ItemInfo[I_SOCKET3];
	pMsg.ItemInfo[I_SOCKET4] = ItemInfo[I_SOCKET4];
	pMsg.ItemInfo[I_SOCKET5] = ItemInfo[I_SOCKET5];

	IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}




void GameProtocol::CGInventoryItemMove(PMSG_INVENTORYITEMMOVE * lpMsg, int aIndex)
{
	BYTE result;
	BYTE ItemInfo[MAX_ITEM_INFO];

	if ( !PacketCheckTime(aIndex) )
	{
		this->GCItemMoveResultSend(aIndex, -1, 0, ItemInfo);
		return;
	}

	if ( !::gObjIsConnectedGP(aIndex))
	{
		g_Log.AddC(TColor::Red, "[%s][%d] error-L3", __FILE__, __LINE__);
		this->GCItemMoveResultSend(aIndex, -1, 0, ItemInfo);
		return;
	}

	
	if ( gObj[aIndex].m_PlayerData->m_bSecurityCheck == false )
	{
		MsgOutput(aIndex, Lang.GetText(0,822));
		this->GCItemMoveResultSend(aIndex, -1, 0, ItemInfo);
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];
	int it_type = 0;

	if ( lpObj->m_PlayerData->m_ReqWarehouseOpen != false )
	{
		g_Log.AddC(TColor::Red, "[%s][%d] error-L3", __FILE__, __LINE__);
		this->GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);

		return;
	}

	int source = lpMsg->source_item_num;
	int target = lpMsg->target_item_num;

	if ( lpMsg->tFlag == 2 || lpMsg->sFlag == 2 )
		it_type = 6;

	if ( lpMsg->tFlag == 0 && lpMsg->sFlag == 0 )
		it_type = 8;

	if ( (lpMsg->tFlag == 0 && lpMsg->sFlag == 4) ||
		(lpMsg->tFlag == 4 && lpMsg->sFlag == 0) ||
		(lpMsg->tFlag == 4 && lpMsg->sFlag == 4) )
		it_type = 8;

	if (gObjCanItemTouch(lpObj, it_type) == FALSE)
	{
		if ( lpMsg->sItemInfo[I_TYPE] == ITEMGET(4,7) || lpMsg->sItemInfo[I_TYPE] == ITEMGET(4,15) )
		{
			if ( target >= 12 || source < 12 )
			{
				this->GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
				return;
			}
		}
		else
		{
			this->GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
			return;
		}
	}

	if ( lpObj->DieRegen != FALSE )
	{
		this->GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
		return;
	}

	if ( lpObj->Teleport != FALSE )
	{
		this->GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
		return;
	}

	ItemInfo[I_TYPE] = lpMsg->sItemInfo[I_TYPE];
	ItemInfo[I_OPTION] = lpMsg->sItemInfo[I_OPTION];
	ItemInfo[I_DUR] = lpMsg->sItemInfo[I_DUR];
	ItemInfo[I_NOPTION] = lpMsg->sItemInfo[I_NOPTION];
	ItemInfo[I_SOPTION] = lpMsg->sItemInfo[I_SOPTION];
	ItemInfo[I_380OPTION] = lpMsg->sItemInfo[I_380OPTION];
	ItemInfo[I_JOHOPTION] = lpMsg->sItemInfo[I_JOHOPTION];
	ItemInfo[I_SOCKET1] = lpMsg->sItemInfo[I_SOCKET1];
	ItemInfo[I_SOCKET2] = lpMsg->sItemInfo[I_SOCKET2];
	ItemInfo[I_SOCKET3] = lpMsg->sItemInfo[I_SOCKET3];
	ItemInfo[I_SOCKET4] = lpMsg->sItemInfo[I_SOCKET4];
	ItemInfo[I_SOCKET5] = lpMsg->sItemInfo[I_SOCKET5];

	if ( lpMsg->sFlag == 3 || lpMsg->tFlag == 3 )
	{
		if ( lpObj->m_IfState.use < 1 || lpObj->m_IfState.type != 7 )
		{
			this->GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
			g_Log.Add("[%s][%s] error-L1 : used not Chaosbox", gObj[aIndex].AccountID, gObj[aIndex].Name);

			return;
		}

		if ( lpObj->ChaosLock == TRUE )
		{
			g_Log.Add("[%s][%s] error-L3 : CBMixing", gObj[aIndex].AccountID, gObj[aIndex].Name);
			this->GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
		
			return;
		}
	}

	if ( lpMsg->sFlag == 5 || lpMsg->tFlag == 5 )
	{
		if ( lpObj->m_IfState.use < 1 || lpObj->m_IfState.type != 13 )
		{
			this->GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
			g_Log.Add("[%s][%s] error-L1 : used not DarkTrainerBox", gObj[aIndex].AccountID, gObj[aIndex].Name);

			return;
		}

		if ( lpObj->ChaosLock == TRUE )
		{
			g_Log.Add("[%s][%s] error-L3 : DarkTrainerBoxMixing", gObj[aIndex].AccountID, gObj[aIndex].Name);
			this->GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
		
			return;
		}
	}

	if ( lpMsg->sFlag == 6 || lpMsg->sFlag == 7 || lpMsg->sFlag == 8 ||
		 lpMsg->tFlag == 6 || lpMsg->tFlag == 7 || lpMsg->tFlag == 8 )
	{
		if ( lpObj->m_IfState.use < 1 || lpObj->m_IfState.type !=7 )
		{
			this->GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
			g_Log.Add("[%s][%s] error-L1 : used not ChaosBox",
				gObj[aIndex].AccountID, gObj[aIndex].Name);

			return;
		}

		if ( lpObj->ChaosLock == TRUE )
		{
			g_Log.Add("[%s][%s] error-L3 : JewelOfHarmony Mixing",
				gObj[aIndex].AccountID, gObj[aIndex].Name);
			this->GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);

			return;
		}
	}

	if ( lpMsg->sFlag == 9 || lpMsg->tFlag == 9 )
	{
		if ( lpObj->m_IfState.use	 < 1 || lpObj->m_IfState.type !=7 )
		{
			this->GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
			g_Log.Add("[%s][%s] error-L1 : used not ChaosBox",
				gObj[aIndex].AccountID, gObj[aIndex].Name);

			return;
		}

		if ( lpObj->ChaosLock == TRUE )
		{
			g_Log.Add("[%s][%s] error-L3 : JewelOfHarmony Mixing",
				gObj[aIndex].AccountID, gObj[aIndex].Name);
			this->GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);

			return;
		}
	}

	if ( lpMsg->sFlag == 11 || lpMsg->sFlag == 12 || lpMsg->sFlag == 13 || lpMsg->sFlag == 14 || lpMsg->sFlag == 15 || lpMsg->sFlag == 16 || lpMsg->sFlag == 17 ||
		lpMsg->tFlag == 11 || lpMsg->tFlag == 12 || lpMsg->tFlag == 13 || lpMsg->tFlag == 14 || lpMsg->tFlag == 15 || lpMsg->tFlag == 16 || lpMsg->tFlag == 17 || lpMsg->tFlag == 19 )
	{
		if ( lpObj->m_IfState.use < 1 || lpObj->m_IfState.type !=7 )
		{
			this->GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
			g_Log.Add("[%s][%s] error-L1 : used not ChaosBox",
				gObj[aIndex].AccountID, gObj[aIndex].Name);

			return;
		}

		if ( lpObj->ChaosLock == TRUE )
		{
			g_Log.Add("[%s][%s] error-L3 : SocketItem Mixing",
				gObj[aIndex].AccountID, gObj[aIndex].Name);
			this->GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);

			return;
		}
	}


	if ( lpMsg->sFlag == 2 || lpMsg->tFlag == 2 )
	{
		if ( lpObj->m_IfState.use < 1 || lpObj->m_IfState.type != 6 )
		{
			this->GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
			g_Log.Add("[%s][%s] error-L1 : used not Warehouse", gObj[aIndex].AccountID, gObj[aIndex].Name);

			return;
		}
	}

	if ( lpMsg->sFlag == 1 && lpMsg->tFlag == 0 )
	{
		if ( lpObj->m_IfState.use < 1 || lpObj->m_IfState.type != 1 )
		{
			this->GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
			g_Log.Add("[%s][%s] error-L1 : used not Trade", gObj[aIndex].AccountID, gObj[aIndex].Name);

			return;
		}
	}

	if ( lpMsg->sFlag == 2 && lpMsg->tFlag == 0 )
	{
		int money = ::GetWarehouseUsedHowMuch(lpObj->Level, lpObj->WarehousePW);

		if ( (lpObj->m_PlayerData->Money - money ) < 1 && (lpObj->WarehouseMoney - money) < 1)
		{
			this->GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
			char szTemp[256];
			wsprintf(szTemp, Lang.GetText(0,154), money);
			this->GCServerMsgStringSend(szTemp, lpObj->m_Index, 1);

			return;
		}

		if ( ::bCanWarehouseLock == TRUE )
		{
			if ( gObj[aIndex].WarehouseLock == 1 )
			{
				this->GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
				this->GCServerMsgStringSend(Lang.GetText(0,155), lpObj->m_Index, 1);

				return;
			}
		}
	}
	int type = (ItemInfo[I_TYPE] + ((ItemInfo[I_NOPTION] & 0x80 )*2)) + ((ItemInfo[I_380OPTION] & 0xF0)<<5);

	if( it_type != 8 )
	{
		LPITEM_ATTRIBUTE lpItemAttr = GetItemAttr(type);

		if ( lpItemAttr == NULL )
		{
			this->GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
			return;
		}

		if ( lpItemAttr->QuestItem != false && !g_QuestExpManager.IsQuestItemAtt(type, 4) )
		{
			this->GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
			return;
		}

		if ( type == ITEMGET(14,64) ) // cursed castle water
		{
			this->GCItemMoveResultSend(aIndex, -1, 0, (LPBYTE)&ItemInfo);
			return;
		}
	}

	if (lpMsg->sFlag == 21 && lpMsg->tFlag != 21 && lpMsg->tFlag != 1)
	{
		this->GCItemMoveResultSend(aIndex, -1, 0, ItemInfo);
		return;
	}

	if (lpMsg->sFlag == 1)
	{
		if (lpMsg->tFlag != 21)
		{
			if (lpMsg->tFlag != 1)
			{
				if (IsEventItem(lpObj->Trade[source]) == TRUE)
				{
					GCItemMoveResultSend(aIndex, -1, 0, ItemInfo);
					return;
				}
			}
		}
	}

	if (lpMsg->tFlag == 21)
	{
		int type = (ItemInfo[I_TYPE] + ((ItemInfo[I_NOPTION] & 0x80) * 2)) + ((ItemInfo[I_380OPTION] & 0xF0) << 5);

		if (IsEventItem(type) == TRUE)
		{
			this->GCItemMoveResultSend(aIndex, -1, 0, ItemInfo);
			return;
		}
	}

	if (lpMsg->sFlag == 22 && lpMsg->tFlag != 22)
	{
		this->GCItemMoveResultSend(aIndex, -1, 0, ItemInfo);
		return;
	}

	if ( (lpMsg->sFlag == 0 && lpMsg->tFlag == 0) ||
 		 (lpMsg->sFlag == 2 && lpMsg->tFlag == 0) ||
		 (lpMsg->sFlag == 0 && lpMsg->tFlag == 2) ||
 		 (lpMsg->sFlag == 2 && lpMsg->tFlag == 2) ||
		 (lpMsg->sFlag == 0 && lpMsg->tFlag == 3) ||
		 (lpMsg->sFlag == 0 && lpMsg->tFlag == 5) ||
		 (lpMsg->sFlag == 0 && lpMsg->tFlag == 6) ||
		 (lpMsg->sFlag == 0 && lpMsg->tFlag == 7) ||
		 (lpMsg->sFlag == 0 && lpMsg->tFlag == 8) ||
		 (lpMsg->sFlag == 0 && lpMsg->tFlag == 9) ||
		 (lpMsg->sFlag == 9 && lpMsg->tFlag == 9) ||
		 (lpMsg->sFlag == 9 && lpMsg->tFlag == 0) ||
		 (lpMsg->sFlag == 0 && lpMsg->tFlag == 4) ||
		 (lpMsg->sFlag == 4 && lpMsg->tFlag == 0) ||
		 (lpMsg->sFlag == 4 && lpMsg->tFlag == 4) ||
		 (lpMsg->sFlag == 5 && lpMsg->tFlag == 0) ||
		 (lpMsg->sFlag == 5 && lpMsg->tFlag == 5) ||
		 (lpMsg->sFlag == 6 && lpMsg->tFlag == 0) ||
		 (lpMsg->sFlag == 6 && lpMsg->tFlag == 6) ||
		 (lpMsg->sFlag == 7 && lpMsg->tFlag == 0) ||
		 (lpMsg->sFlag == 7 && lpMsg->tFlag == 7) ||
		 (lpMsg->sFlag == 8 && lpMsg->tFlag == 0) ||
		 (lpMsg->sFlag == 8 && lpMsg->tFlag == 8) ||
		 (lpMsg->sFlag == 3 && lpMsg->tFlag == 0) ||
		 (lpMsg->sFlag == 3 && lpMsg->tFlag == 3) ||
		 (lpMsg->sFlag == 0 && lpMsg->tFlag == 10) ||
		 (lpMsg->sFlag == 10 && lpMsg->tFlag == 0) ||
		 (lpMsg->sFlag == 0 && lpMsg->tFlag == 11) ||
		 (lpMsg->sFlag == 11 && lpMsg->tFlag == 0) ||
		 (lpMsg->sFlag == 0 && lpMsg->tFlag == 12) ||
		 (lpMsg->sFlag == 12 && lpMsg->tFlag == 0) ||
		 (lpMsg->sFlag == 0 && lpMsg->tFlag == 13) ||
		 (lpMsg->sFlag == 13 && lpMsg->tFlag == 0) ||
		 (lpMsg->sFlag == 0 && lpMsg->tFlag == 14) ||
		 (lpMsg->sFlag == 14 && lpMsg->tFlag == 0) ||
		 (lpMsg->sFlag == 0 && lpMsg->tFlag == 15) ||
		 (lpMsg->sFlag == 15 && lpMsg->tFlag == 0) ||
		 (lpMsg->sFlag == 0 && lpMsg->tFlag == 16) ||
		 (lpMsg->sFlag == 16 && lpMsg->tFlag == 0) ||
		 (lpMsg->sFlag == 0 && lpMsg->tFlag == 17) ||
		 (lpMsg->sFlag == 17 && lpMsg->tFlag == 0) ||
		 (lpMsg->sFlag == 11 && lpMsg->tFlag == 11) ||
		 (lpMsg->sFlag == 12 && lpMsg->tFlag == 12) ||
		 (lpMsg->sFlag == 13 && lpMsg->tFlag == 13) ||
		 (lpMsg->sFlag == 14 && lpMsg->tFlag == 14) ||
		 (lpMsg->sFlag == 15 && lpMsg->tFlag == 15) ||
		 (lpMsg->sFlag == 16 && lpMsg->tFlag == 16) ||
		 (lpMsg->sFlag == 17 && lpMsg->tFlag == 17) ||
		 (lpMsg->sFlag == 0 && lpMsg->tFlag == 19) ||
		 (lpMsg->sFlag == 19 && lpMsg->tFlag == 0) ||
		 (lpMsg->sFlag == 19 && lpMsg->tFlag == 19) ||
		 (lpMsg->sFlag == 0 && lpMsg->tFlag == 20) ||
		 (lpMsg->sFlag == 20 && lpMsg->tFlag == 0) ||
		 (lpMsg->sFlag == 20 && lpMsg->tFlag == 20))
	{
		BOOL DurSSend;
		BOOL DurTSend;

		result = gObjInventoryMoveItem(aIndex, source, target, DurSSend, DurTSend, lpMsg->sFlag, lpMsg->tFlag, (LPBYTE)&ItemInfo);

		if (lpObj->m_IfState.use == 1 && lpObj->m_IfState.type == 3) // Shop
		{
			this->GCPriceSend(lpObj->m_Index, 1, 0);
		}

		this->GCItemMoveResultSend(aIndex, result, target, (LPBYTE)&ItemInfo);

		if ( DurSSend != FALSE )
			this->GCItemDurSend(aIndex, source, lpObj->pInventory[source].m_Durability, FALSE);

		if ( DurTSend != FALSE )
			this->GCItemDurSend(aIndex, target, lpObj->pInventory[target].m_Durability, FALSE);

		if ( result != 0xFF && lpMsg->sFlag == 2 && lpMsg->tFlag == 0)
		{
			int money = ::GetWarehouseUsedHowMuch(lpObj->Level, lpObj->WarehousePW);

			if ( (lpObj->m_PlayerData->Money - money) > 0 )
			{
				int iZen = lpObj->m_PlayerData->Money;
				lpObj->m_PlayerData->Money -= money;
				this->GCMoneySend(lpObj->m_Index, lpObj->m_PlayerData->Money);

				g_Log.Add("Pay WareHouse Money(In Inventory) : %d - %d = %d",
					iZen, money, lpObj->m_PlayerData->Money);
			}
			else if ( (lpObj->WarehouseMoney - money) > 0 )
			{
				int iZen = lpObj->WarehouseMoney;
				lpObj->WarehouseMoney -= money;
	
				g_Log.Add("Pay WareHouse Money(In WareHouse) : %d - %d = %d",
					iZen, money, lpObj->WarehouseMoney);

				this->GCWarehouseInventoryMoneySend(aIndex, 1, lpObj->m_PlayerData->Money, lpObj->WarehouseMoney);
			}
		}

		g_PeriodItemEx.SendPeriodItemInfoOnce(lpObj, &lpObj->pInventory[target]);

		return;
	}

	if (lpMsg->sFlag == 22 && lpMsg->tFlag == 22)
	{
		result = gObjMuunInvenMove(lpObj, 0, 0, source, target, (LPBYTE)ItemInfo);
		GCItemMoveResultSend(aIndex, result, target, (LPBYTE)ItemInfo);
	}

	if (lpMsg->sFlag == 1 && lpMsg->tFlag == 21)
	{
		result = gObjTradeEventInventoryMove(lpObj, source, target);

		if (result == 255)
		{
			this->GCItemMoveResultSend(aIndex, -1, target, ItemInfo);
			this->CGTradeCancelButtonRecv(aIndex);
		}

		else
		{
			this->GCItemMoveResultSend(aIndex, result, target, ItemInfo);
		}

		if (result == 255)
		{
			lpObj->m_IfState.state = 0;
			this->GCEventInventoryItemListSend(aIndex);
		}

		else
		{
			lpObj->TradeOk = 0;
			gObj[gObj[aIndex].TargetNumber].TradeOk = 0;
			GCTradeOkButtonSend(lpObj->TargetNumber, 2);
			GCTradeOkButtonSend(aIndex, 0);
		}

		return;
	}

	if (lpMsg->sFlag == 21 && lpMsg->tFlag == 1)
	{
		result = gObjEventInventoryTradeMove(lpObj, source, target);
		GCItemMoveResultSend(aIndex, result, target, ItemInfo);

		return;
	}

	if (lpMsg->sFlag == 21 && lpMsg->tFlag == 21)
	{
		BOOL DurSSend;
		BOOL DurTSend;

		result = gObjEventInvenMove(lpObj, &DurSSend, &DurTSend, source, target);
		this->GCItemMoveResultSend(aIndex, result, target, ItemInfo);

		if (result == 255)
		{
			g_Log.Add("Move Event Inventory error s: %d, t: %d", source, target);
			return;
		}

		if (DurSSend)
		{
			this->GCEventItemDurSend(aIndex, source, lpObj->pEventInventory[source].m_Durability);
		}

		if (DurTSend)
		{
			this->GCEventItemDurSend(aIndex, target, lpObj->pEventInventory[source].m_Durability);
		}

		if (lpObj->pEventInventory[target].m_Durability == 0.0)
		{
			GCEventInventoryItemDeleteSend(aIndex, target, TRUE);
		}

		return;
	}

	if ( lpMsg->sFlag == 1 && lpMsg->tFlag == 1 )
	{
		result = gObjTradeTradeMove(lpObj, source, target);
		this->GCItemMoveResultSend(aIndex, result, target, (LPBYTE)&ItemInfo);

		return;
	}

	if ( lpMsg->sFlag == 1 && lpMsg->tFlag == 0 )
	{
		result = gObjTradeInventoryMove(lpObj, source, target);

		if ( result == 0xFF )
		{
			this->GCItemMoveResultSend(aIndex, 0xFF, target, (LPBYTE)&ItemInfo);
			this->CGTradeCancelButtonRecv(aIndex);
		}
		else
		{
			this->GCItemMoveResultSend(aIndex, result, target, (LPBYTE)&ItemInfo);
		}

		if ( result == 0xFF )
		{
			lpObj->m_IfState.state = 0;
			GCItemListSend(aIndex);
			g_PeriodItemEx.OnRequestPeriodItemList(lpObj);
		}
		
		if ( lpObj->TargetNumber >= 0 )
		{
			lpObj->TradeOk = false;
			gObj[gObj[aIndex].TargetNumber].TradeOk = false;
			this->GCTradeOkButtonSend(lpObj->TargetNumber, 2);
			this->GCTradeOkButtonSend(aIndex, 0);
		}

		return;
	}

	if ( lpMsg->sFlag == 0 && lpMsg->tFlag == 1 )
	{
		result = gObjInventoryTradeMove(lpObj, source, target);
		this->GCItemMoveResultSend(aIndex, result, target, (LPBYTE)&ItemInfo);
	}
}



void GameProtocol::GCEquipmentChange(int aIndex, int pos)
{
	PMSG_USEREQUIPMENTCHANGED pChange;
	LPOBJ lpObj = &gObj[aIndex];

	PHeadSetB((LPBYTE)&pChange, 0x25, sizeof(PMSG_USEREQUIPMENTCHANGED));
	pChange.NumberH = SET_NUMBERH(aIndex);
	pChange.NumberL = SET_NUMBERL(aIndex);
	ItemByteConvert(pChange.ItemInfo, lpObj->pInventory[pos]);
	pChange.ItemInfo[I_OPTION] = pos << 4;
	pChange.ItemInfo[I_OPTION] |= LevelSmallConvert(aIndex, pos) & MAX_ITEM_LEVEL;
	pChange.Element = lpObj->m_iPentagramMainAttribute;

	MsgSendV2(lpObj, (LPBYTE)&pChange, pChange.h.size);
}

void GameProtocol::CGTalkRequestRecv(PMSG_TALKREQUEST * lpMsg, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	int DealerNumber ;
	PMSG_TALKRESULT pResult;
	PMSG_SHOPITEMCOUNT pShopItemCount;

	if ( !PacketCheckTime(aIndex))
		return;

	if ( !gObjIsConnectedGP(aIndex) )
	{
		g_Log.Add("[ANTI-HACK][NPC Talk] Couldn't talk.. (%s)(%s)",
			gObj[aIndex].AccountID, gObj[aIndex].Name);

		return;
	}

	if ( gObj[aIndex].CloseType != -1 )
		return;

	if ( gObj[aIndex].m_PlayerData->m_bSecurityCheck == false )
	{
		MsgOutput(aIndex, Lang.GetText(0,822));
		return;
	}

	BYTE SendByte[4096];
	int lOfs = 0;

	if ( lpObj->m_IfState.use > 0 )
		return;

	DealerNumber = MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);
	
	if ( DealerNumber < 0 || DealerNumber > g_ConfigRead.server.GetObjectMax()-1)
		return;

	if ( lpObj->MapNumber != gObj[DealerNumber].MapNumber )
		return;

	if (g_ConfigRead.server.GetServerType() != SERVER_CASTLE)
	{
		if (gObj[DealerNumber].Class == 367)
		{
			if ((lpObj->X < (gObj[DealerNumber].X - 5)) || (lpObj->X>(gObj[DealerNumber].X + 5)) || (lpObj->Y < (gObj[DealerNumber].Y - 10)) || (lpObj->Y >(gObj[DealerNumber].Y + 5)))
			{
				return;
			}
		}
		else if ((lpObj->X < (gObj[DealerNumber].X - 5)) || (lpObj->X>(gObj[DealerNumber].X + 5)) || (lpObj->Y < (gObj[DealerNumber].Y - 5)) || (lpObj->Y >(gObj[DealerNumber].Y + 5)))
		{
			return;
		}

	}
	else
	{
		if ((lpObj->X < (gObj[DealerNumber].X - 5)) || (lpObj->X>(gObj[DealerNumber].X + 5)) || (lpObj->Y < (gObj[DealerNumber].Y - 5)) || (lpObj->Y >(gObj[DealerNumber].Y + 5)))
		{
			return;
		}
	}

	if ( gObj[DealerNumber].Type == OBJ_NPC )
	{
		if (NpcTalk(&gObj[DealerNumber], lpObj) == TRUE)
		{
			this->GCAnsCsMapSvrTaxInfo(lpObj->m_Index, 1, g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index));
			return;
		}
	}

	if ( ::gObjFixInventoryPointer(aIndex) == false )
	{
		g_Log.Add("[Fix Inv.Ptr] False Location - %s, %d", __FILE__, __LINE__);
	}

	if (gObj[aIndex].pTransaction == 1)
	{
		g_Log.Add("[%s][%s] CGTalkRequestRecv() Failed : Transaction == 1, IF_TYPE : %d",
			gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_IfState.type);

		return;
	}

	if ( gObj[DealerNumber].Class == 234 )	// Npc Server Division
	{
		pResult.h.c = 0xC3;
		pResult.h.headcode = 0x30;
		pResult.h.size = sizeof(pResult);
		pResult.result = 5;

		IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( gObj[DealerNumber].Type == OBJ_NPC && gObj[DealerNumber].m_btNpcType == NPC_TYPES::NPC_NONE)
		return;

	if ( gObj[DealerNumber].m_btNpcType == NPC_TYPES::NPC_SHOP )
	{
		SHOP_DATA * lpShopData = g_ShopMng.GetShop(lpObj, &gObj[DealerNumber]);

		if (!lpShopData)
		{
			return;
		}
	}

	if ( gMerryXMasNpcEvent == TRUE )
	{
		if ( (rand()%6) == 0 )
		{
			ChatTargetSend(&gObj[DealerNumber], Lang.GetText(0,55), aIndex);
		}
	}

	if ( ::gHappyNewYearNpcEvent == TRUE )
	{
		if ( (rand()%6) == 0 )
		{
			ChatTargetSend(&gObj[DealerNumber], Lang.GetText(0,56), aIndex);
		}
	}

	SHOP_DATA * lpShopData = g_ShopMng.GetShop(lpObj, &gObj[DealerNumber]);

	lpObj->TargetNpcNumber = DealerNumber;
	lpObj->m_IfState.use = 1;
	lpObj->m_IfState.type = 3;
	lpObj->m_ShopTime = 0;
	pResult.h.c = 0xC3;
	pResult.h.headcode = 0x30;
	pResult.h.size = sizeof(pResult);

	if (gObj[DealerNumber].m_btNpcType == NPC_TYPES::NPC_SHOP && lpShopData->btIsMossMerchant == TRUE)
	{
		pResult.result = 0x22;
	}
	else
	{
		pResult.result = 0x00;
	}

	if (gObj[DealerNumber].m_btNpcType == NPC_TYPES::NPC_SHOP && lpShopData->btIsMossMerchant == TRUE && !g_GamblingItemBag.IsEnabled())	return;

	if (gObj[DealerNumber].m_btNpcType == NPC_TYPES::NPC_WAREHOUSE)
	{		
		if (gObj[aIndex].m_PlayerData->m_JoinUnityBattle == true || g_ConfigRead.server.GetServerType() == SERVER_BATTLECORE)
		{
			this->GCServerMsgStringSend(Lang.GetText(0,619), aIndex, 1);
			return;
		}

		if (g_ConfigRead.server.GetServerType() == SERVER_MARKET || gObj[aIndex].MapNumber == MAP_INDEX_LORENMARKET)
		{
			g_Log.Add("[Error][%s][%s] Try open Warehouse in Market Server or lorenmarket map.",
				lpObj->AccountID, lpObj->Name);
			return;
		}

		if (gObj[aIndex].m_PlayerData->m_ReqWarehouseOpen != false)
		{
			g_Log.Add("[%s][%s] is Already Opening Warehouse (ReqWarehouseOpen), open warehouse failed",
				lpObj->AccountID, lpObj->Name);
			return;
		}

		if (gObj[aIndex].m_PlayerData->m_bSecurityCheck == false)
		{
			return;
		}

		if (gObj[aIndex].m_btOpenWarehouse == TRUE)
		{
			g_Log.Add("[%s][%s] is Already Opening Warehouse, open warehouse failed",
				lpObj->AccountID, lpObj->Name);
			return;
		}

		lpObj->m_PlayerData->m_ReqWarehouseOpen = true;
		lpObj->m_IfState.type = 6;
		lpObj->m_IfState.state = 0;
	}

	else if (gObj[DealerNumber].m_btNpcType == NPC_TYPES::NPC_CHAOSMIX)
	{
		if (bCanChaosBox == TRUE)
		{
			if (lpObj->m_bPShopOpen == true)
			{
				g_Log.Add("[%s][%s] is Already Opening PShop, ChaosBox Failed",
					lpObj->AccountID, lpObj->Name);
				GCServerMsgStringSend(Lang.GetText(0,112), lpObj->m_Index, 1);

				return;
			}

			lpObj->m_IfState.type = 7;
			lpObj->m_IfState.state = 0;
			pResult.result = 3;
			if (gObj[DealerNumber].Class == 450)
				pResult.result = 22;
			else if (gObj[DealerNumber].Class == 478)
				pResult.result = 32;
			lpObj->bIsChaosMixCompleted = false;
			pResult.level1 = gDQChaosSuccessRateLevel1;
			pResult.level2 = gDQChaosSuccessRateLevel2;
			pResult.level3 = gDQChaosSuccessRateLevel3;
			pResult.level4 = gDQChaosSuccessRateLevel4;
			pResult.level5 = gDQChaosSuccessRateLevel5;
			pResult.level6 = gDQChaosSuccessRateLevel6;
			pResult.level7 = gDQChaosSuccessRateLevel7;

			IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
			gObjInventoryTrans(lpObj->m_Index);

			g_Log.Add("[%s][%s] Open Chaos Box", lpObj->AccountID, lpObj->Name);
			gObjItemTextSave(lpObj);
			gObjWarehouseTextSave(lpObj);
			GCAnsCsMapSvrTaxInfo(lpObj->m_Index, 1, ::g_CastleSiegeSync.GetTaxRateChaos(lpObj->m_Index));
		}
	}

	else if(gObj[DealerNumber].Class == 492)
	{
		pResult.result = 34;
		IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
	}
	else
	{
		IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
	}

	if (gObj[DealerNumber].m_btNpcType == NPC_TYPES::NPC_WAREHOUSE)
	{
		if (gObj[aIndex].m_bMapSvrMoveReq == true)
		{
			gObj[aIndex].m_PlayerData->m_ReqWarehouseOpen = false;
			g_Log.Add("[CGTalkRequestRecv] MapServerMove User. Can't Open WhareHouse. return!! [%s], IP [%s] ",
				gObj[aIndex].AccountID, gObj[aIndex].m_PlayerData->Ip_addr);
			return;
		}

		if (gObj[aIndex].m_State == 32)
		{
			gObj[aIndex].m_PlayerData->m_ReqWarehouseOpen = false;
			g_Log.Add("[CGTalkRequestRecv] MapServerMove User. Can't Open WhareHouse. return!! [%s], IP [%s] ",
				gObj[aIndex].AccountID, gObj[aIndex].m_PlayerData->Ip_addr);
			return;
		}

		if (gObj[aIndex].m_bMapSvrMoveQuit == true)
		{
			gObj[aIndex].m_PlayerData->m_ReqWarehouseOpen = false;
			g_Log.Add("[CGTalkRequestRecv] MapServerMove User. Can't Open WhareHouse. return!! [%s], IP [%s] ",
				gObj[aIndex].AccountID, gObj[aIndex].m_PlayerData->Ip_addr);
			return;
		}

		if (gObj[aIndex].m_IfState.use && gObj[aIndex].m_IfState.type == 19)
		{
			g_Log.Add("[CGTalkRequestRecv][SHOPITEMCOPY][InGameShop Interface Using].[Can't Open WhareHouse] return!! AccID:[%s], IP:[%s], Name :[%s]",
				gObj[aIndex].AccountID, gObj[aIndex].m_PlayerData->Ip_addr, gObj[aIndex].Name);
			return;
		}

		if (gObj[aIndex].WarehouseSaveLock == TRUE)
		{
			gObj[aIndex].m_PlayerData->m_ReqWarehouseOpen = false;
			return;
		}

		gObj[aIndex].WarehouseCount = 0;
		GDGetWarehouseList(aIndex, gObj[aIndex].AccountID);
		return;
	}

	if (gObj[DealerNumber].m_btNpcType == NPC_TYPES::NPC_CHAOSMIX)
		return;

	else if (gObj[DealerNumber].m_btNpcType == NPC_TYPES::NPC_SHOP)
	{
		lOfs = 0;

		lOfs += sizeof(pShopItemCount);

		int size = lOfs + lpShopData->m_ShopData.SendItemDataLen;
		PHeadSetW((LPBYTE)&pShopItemCount, 0x31, size);
		pShopItemCount.Type = 0;
		pShopItemCount.count = lpShopData->m_ShopData.ItemCount;
		memcpy(SendByte, &pShopItemCount, sizeof(pShopItemCount));
		memcpy(&SendByte[lOfs], lpShopData->m_ShopData.SendItemData, lpShopData->m_ShopData.SendItemDataLen);

		IOCP.DataSend(aIndex, SendByte, size);
		GCAnsCsMapSvrTaxInfo(lpObj->m_Index, 2, ::g_CastleSiegeSync.GetTaxRateStore(lpObj->m_Index));
	}
}




void GameProtocol::GCUserWarehouseSend(LPOBJ lpObj)
{
	if ( lpObj->m_IfState.type != 6 )
		return;

	if ( lpObj->m_IfState.type == 6 && lpObj->m_IfState.state == 1 )
		return;

	if ( lpObj->m_btOpenWarehouse == TRUE )
	{
		g_Log.Add("[%s][%s] is Already Opening Warehouse, open warehouse failed",
			lpObj->AccountID, lpObj->Name);
		return;
	}

	PMSG_SHOPITEMCOUNT pMsg;
	BYTE SendByte[4096];
	int lOfs = sizeof(pMsg);

	PHeadSetW((LPBYTE)&pMsg, 0x31, 0);
	pMsg.count = 0;
	pMsg.Type = 0;

	for ( int n=0;n<WAREHOUSE_SIZE;n++)
	{
		if ( lpObj->pWarehouse[n].IsItem() == TRUE )
		{
			SendByte[lOfs] = n;
			lOfs++;
			ItemByteConvert(&SendByte[lOfs], lpObj->pWarehouse[n] );
			lOfs += MAX_ITEM_INFO;
			pMsg.count ++;
		}
	}

	pMsg.h.sizeH = SET_NUMBERH(lOfs);
	pMsg.h.sizeL = SET_NUMBERL(lOfs);
	memcpy(SendByte, &pMsg, sizeof(pMsg));

	IOCP.DataSend(lpObj->m_Index, SendByte, lOfs);
	this->GCWarehouseInventoryMoneySend(lpObj->m_Index, 1, lpObj->m_PlayerData->Money, lpObj->WarehouseMoney);

	if ( lpObj->m_IfState.type == 6 )
	{
		lpObj->m_IfState.state = 1;
	}

	lpObj->m_btOpenWarehouse = TRUE;
}

void GameProtocol::CGBuyRequestRecv(PMSG_BUYREQUEST * lpMsg, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	PMSG_BUYRESULT pResult;

	PHeadSetB((LPBYTE)&pResult, 0x32, sizeof(pResult));
	pResult.Result = -1;

	if ( !PacketCheckTime(aIndex))
	{
		IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( gObj[aIndex].Type != OBJ_USER )
	{
		return;
	}

	if ( gObj[aIndex].CloseType != -1 )
	{
		IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if (gObj[aIndex].TargetNpcNumber == -1)
	{
		IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	LPOBJ lpShopObj = &gObj[gObj[aIndex].TargetNpcNumber];

	SHOP_DATA * lpShopData = g_ShopMng.GetShop(lpObj, lpShopObj);

	if (!lpShopData)
	{
		IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( !::gObjFixInventoryPointer(aIndex))
	{
		g_Log.Add("[Fix Inv.Ptr] False Location - %s, %d", __FILE__, __LINE__);
	}

	if ( gObj[aIndex].m_ShopTime == 0 )
		gObj[aIndex].m_ShopTime = 1;

	if (lpShopObj->Class == 492 && lpShopData->btIsMossMerchant == TRUE)
	{
		if (gObj[aIndex].m_ShopTime > g_GamblingItemBag.GetGamblingShopTime())
		{
			if (lpObj->m_IfState.use && lpObj->m_IfState.type == 3)
			{
				lpObj->TargetNpcNumber = -1;
				pResult.Result = 0xFE;
				lpObj->m_IfState.use = 0; 
				lpObj->m_IfState.type = 0;
			}

			IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
			GCServerMsgStringSend(Lang.GetText(0,520), aIndex, 1);
			return;
		}
	}

	if ( gObj[aIndex].m_ShopTime > 	60 )
	{
		if ( lpObj->m_IfState.use != 0 && lpObj->m_IfState.type == 3 )
		{
			lpObj->TargetNpcNumber = -1;
			lpObj->m_IfState.use = 0;
			lpObj->m_IfState.type = 0;
		}

		IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( lpObj->m_IfState.use > 0 )
	{
		if ( lpObj->m_IfState.type != 3 )
		{
			IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
			return;
		}
	}

	if ( lpMsg->Pos > MAX_ITEM_IN_SHOP-1 )
	{
		IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( lpObj->m_PlayerData->m_bIsCancelItemSale == true )
	{
		IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}
	
	if ( lpShopData->m_ShopData.m_item[lpMsg->Pos].IsItem() == TRUE )
	{
		int iStoreTaxMoney = lpShopData->m_ShopData.m_item[lpMsg->Pos].m_BuyMoney + (int)((__int64)lpShopData->m_ShopData.m_item[lpMsg->Pos].m_BuyMoney * (__int64)g_CastleSiegeSync.GetTaxRateStore(lpObj->m_Index) / (__int64)100);
		
		if ( iStoreTaxMoney < 0 )
			iStoreTaxMoney  = 0;

		int iStoreTaxMoney2 = (int)((__int64)lpShopData->m_ShopData.m_item[lpMsg->Pos].m_BuyMoney * (__int64)g_CastleSiegeSync.GetTaxRateStore(lpObj->m_Index) / (__int64)100);

		if ( iStoreTaxMoney2 < 0 )
			iStoreTaxMoney2  = 0;

		if ( g_ArcaBattle.IsArcaBattleServer() == 1 && iStoreTaxMoney > 0 )
		{
			iStoreTaxMoney *= 3;
		}

		/*if (gObj[aIndex].m_PlayerData->VipType != 0) // not used for now
		{
			iStoreTaxMoney *= g_VipSystem.GetShopBuyPriceMultipler(lpObj);
		}*/

		if (lpShopData->btIsMossMerchant == TRUE)
		{
			int iTaxGambling = g_GamblingItemBag.GetGamblingValue() * g_CastleSiegeSync.GetTaxRateStore(lpObj->m_Index) / 100;
			iStoreTaxMoney = g_GamblingItemBag.GetGamblingValue() + iTaxGambling;
		}

		if ( lpObj->m_PlayerData->Money < iStoreTaxMoney )
			pResult.Result = -1;
		else
		{
			BOOL bNoItem = TRUE;

			if ((lpShopData->m_ShopData.m_item[lpMsg->Pos].m_Type >= ITEMGET(14, 0) && lpShopData->m_ShopData.m_item[lpMsg->Pos].m_Type <= ITEMGET(14, 8)) ||
				(lpShopData->m_ShopData.m_item[lpMsg->Pos].m_Type >= ITEMGET(14, 35) && lpShopData->m_ShopData.m_item[lpMsg->Pos].m_Type <= ITEMGET(14, 40)))
			{
				int dur = lpShopData->m_ShopData.m_item[lpMsg->Pos].m_Durability;

				if ( dur == 0 )
					dur = 1;

				if (::gObjSearchItem(&gObj[aIndex], lpShopData->m_ShopData.m_item[lpMsg->Pos].m_Type, dur, lpShopData->m_ShopData.m_item[lpMsg->Pos].m_Level) == TRUE)
				{
					bNoItem = FALSE;
					lpObj->m_PlayerData->Money -= iStoreTaxMoney;
					::g_CastleSiegeSync.AddTributeMoney(iStoreTaxMoney2);

					if ( lpObj->m_PlayerData->Money < 0 )
						lpObj->m_PlayerData->Money = 0;

		
					this->GCMoneySend(aIndex, lpObj->m_PlayerData->Money);
				}
			}

			if (lpShopData->btIsMossMerchant == TRUE)
			{
				if (!g_GamblingItemBag.IsEnabled())
				{
					pResult.Result = 0xFF;
					IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
					return;
				}

				if (bNoItem)
				{
					int iCount = 0;

					while (iCount <= lpShopData->m_ShopData.ItemCount)
					{
						if (lpMsg->Pos == lpShopData->m_ShopData.SendItemData[iCount + (12 * iCount)])
						{
							pResult.Result = g_GamblingItemBag.GetGamblingSystemItem(aIndex, 229, 0, 0, iCount);
							break;
						}
						iCount++;
					}

					pResult.Result = 0xFF;
					IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
					return;
				}
			}

			if (bNoItem)
			{
				if (lpShopData->m_ShopData.m_item[lpMsg->Pos].m_Type == ITEMGET(13,239)) // Muun Energy Converter
				{
					BYTE btRet = gObjMuunInventoryInsertItem(aIndex, lpShopData->m_ShopData.m_item[lpMsg->Pos]);

					if (btRet == 0xFF)
					{
						pResult.Result = 0xFF;
						IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
						return;
					}

					ItemByteConvert(pResult.ItemInfo, lpShopData->m_ShopData.m_item[lpMsg->Pos]);
					int buymoney = lpShopData->m_ShopData.m_item[lpMsg->Pos].m_BuyMoney;
					lpObj->m_PlayerData->Money -= iStoreTaxMoney;
					g_CastleSiegeSync.AddTributeMoney(iStoreTaxMoney2);
					if (lpObj->m_PlayerData->Money < 0) lpObj->m_PlayerData->Money = 0;
					GCMoneySend(aIndex, lpObj->m_PlayerData->Money);
					int iTaxRate = g_CastleSiegeSync.GetTaxRateStore(lpObj->m_Index);
					g_Log.Add("[%s][%s] Shop buy [MAP:%d][X:%d][Y:%d] [%d][%d][%d] [%s] LEV:%d, DUR:%d, OP:[%d][%d][%d]", lpObj->AccountID, lpObj->Name, lpShopData->btMapNumber, lpShopData->btPosX, lpShopData->btPosY, buymoney, iTaxRate, buymoney + iStoreTaxMoney2, lpShopData->m_ShopData.m_item[lpMsg->Pos].GetName(), lpShopData->m_ShopData.m_item[lpMsg->Pos].m_Level, (int)lpShopData->m_ShopData.m_item[lpMsg->Pos].m_Durability, lpShopData->m_ShopData.m_item[lpMsg->Pos].m_Option1, lpShopData->m_ShopData.m_item[lpMsg->Pos].m_Option2, lpShopData->m_ShopData.m_item[lpMsg->Pos].m_Option3);
					
					this->GCMuunInventoryItemOneSend(aIndex, btRet);

					pResult.Result = 0xFF;
					IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
					return;
				}

				if (lpShopData->m_ShopData.m_item[lpMsg->Pos].m_serial == 0)
				{
					LPITEM_ATTRIBUTE p = GetItemAttr(lpShopData->m_ShopData.m_item[lpMsg->Pos].m_Type);

					if (!p)
					{
						pResult.Result = 0xFF;
						IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
						return;
					}

					pResult.Result = gObjShopBuyInventoryInsertItem(aIndex, lpShopData->m_ShopData.m_item[lpMsg->Pos]);

					if (pResult.Result != 0xFF)
					{
						ItemByteConvert(pResult.ItemInfo, lpShopData->m_ShopData.m_item[lpMsg->Pos]);
						int buymoney = lpShopData->m_ShopData.m_item[lpMsg->Pos].m_BuyMoney;
						lpObj->m_PlayerData->Money -= iStoreTaxMoney;
						g_CastleSiegeSync.AddTributeMoney(iStoreTaxMoney2);
						if (lpObj->m_PlayerData->Money < 0) lpObj->m_PlayerData->Money = 0;
						GCMoneySend(aIndex, lpObj->m_PlayerData->Money);
						int iTaxRate = g_CastleSiegeSync.GetTaxRateStore(lpObj->m_Index);
						g_Log.Add("[%s][%s] Shop buy [MAP:%d][X:%d][Y:%d] [%d][%d][%d] [%s] LEV:%d, DUR:%d, OP:[%d][%d][%d]", lpObj->AccountID, lpObj->Name, lpShopData->btMapNumber, lpShopData->btPosX, lpShopData->btPosY, buymoney, iTaxRate, buymoney + iStoreTaxMoney2, lpShopData->m_ShopData.m_item[lpMsg->Pos].GetName(), lpShopData->m_ShopData.m_item[lpMsg->Pos].m_Level, (int)lpShopData->m_ShopData.m_item[lpMsg->Pos].m_Durability, lpShopData->m_ShopData.m_item[lpMsg->Pos].m_Option1, lpShopData->m_ShopData.m_item[lpMsg->Pos].m_Option2, lpShopData->m_ShopData.m_item[lpMsg->Pos].m_Option3);
					}
				}

				else
				{
					LPITEM_ATTRIBUTE p = GetItemAttr(lpShopData->m_ShopData.m_item[lpMsg->Pos].m_Type);

					if (!p)
					{
						pResult.Result = 0xFF;
						IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
						return;
					}

					if (CheckInventoryEmptySpace(&gObj[aIndex], p->Height, p->Width) == FALSE)
					{
						pResult.Result = 0xFF;
						IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
						return;
					}

					int buymoney = lpShopData->m_ShopData.m_item[lpMsg->Pos].m_BuyMoney;
					lpObj->m_PlayerData->Money -= iStoreTaxMoney;
					g_CastleSiegeSync.AddTributeMoney(iStoreTaxMoney2);
					if (lpObj->m_PlayerData->Money < 0) lpObj->m_PlayerData->Money = 0;
					GCMoneySend(aIndex, lpObj->m_PlayerData->Money);
					int iTaxRate = g_CastleSiegeSync.GetTaxRateStore(lpObj->m_Index);

					ItemSerialCreateSend(aIndex, 222, 0, 0, lpShopData->m_ShopData.m_item[lpMsg->Pos].m_Type, lpShopData->m_ShopData.m_item[lpMsg->Pos].m_Level,
						lpShopData->m_ShopData.m_item[lpMsg->Pos].m_Durability, lpShopData->m_ShopData.m_item[lpMsg->Pos].m_Option1,
						lpShopData->m_ShopData.m_item[lpMsg->Pos].m_Option2, lpShopData->m_ShopData.m_item[lpMsg->Pos].m_Option3,
						aIndex, lpShopData->m_ShopData.m_item[lpMsg->Pos].m_NewOption, lpShopData->m_ShopData.m_item[lpMsg->Pos].m_SetOption,
						0, lpShopData->m_ShopData.m_item[lpMsg->Pos].m_SocketOption, lpShopData->m_ShopData.m_item[lpMsg->Pos].m_BonusSocketOption);

					g_Log.Add("[%s][%s] Request to Shop buy (Serial Item) [MAP:%d][X:%d][Y:%d] [%d][%d][%d] [%s] LEV:%d, DUR:%d, OP:[%d][%d][%d]", lpObj->AccountID, lpObj->Name, lpShopData->btMapNumber, lpShopData->btPosX, lpShopData->btPosY, buymoney, iTaxRate, buymoney + iStoreTaxMoney2, lpShopData->m_ShopData.m_item[lpMsg->Pos].GetName(), lpShopData->m_ShopData.m_item[lpMsg->Pos].m_Level, lpShopData->m_ShopData.m_item[lpMsg->Pos].m_Durability, lpShopData->m_ShopData.m_item[lpMsg->Pos].m_Option1, lpShopData->m_ShopData.m_item[lpMsg->Pos].m_Option2, lpShopData->m_ShopData.m_item[lpMsg->Pos].m_Option3);
				}
			}
		}
	}

	IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
}



struct PMSG_SELLRESULT
{
	PBMSG_HEAD h;	// C1:33
	BYTE Result;	// 3
	DWORD Money;	// 4
};



void GameProtocol::CGSellRequestRecv(PMSG_SELLREQUEST * lpMsg, int aIndex)
{
	PMSG_SELLRESULT pResult;
	LPOBJ lpObj = &gObj[aIndex];

	PHeadSetB((LPBYTE)&pResult, 0x33, sizeof(pResult));
	pResult.Result = false;
	pResult.Money = 0;

	if ( gObj[aIndex].Type != OBJ_USER )
	{
		return;
	}

	if ( gObj[aIndex].CloseType != -1 )
	{
		IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( lpObj->m_IfState.use > 0 && lpObj->m_IfState.type != 3 )
	{
		IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( !PacketCheckTime(aIndex))
	{
		IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( !::gObjFixInventoryPointer(aIndex))
	{
		g_Log.Add("[Fix Inv.Ptr] False Location - %s, %d", __FILE__, __LINE__);
	}

	if ( gObj[aIndex].pTransaction == 1 )
	{
		g_Log.Add("[%s][%s] CGSellRequestRecv() Failed : Transaction == 1, IF_TYPE : %d",
			gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_IfState.type);

		return;
	}

	if (gObj[aIndex].TargetNpcNumber == -1)
	{
		IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	LPOBJ lpShopObj = &gObj[gObj[aIndex].TargetNpcNumber];

	SHOP_DATA * lpShopData = g_ShopMng.GetShop(lpObj, lpShopObj);

	if (!lpShopData)
	{
		IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( !gObjCanItemTouch(lpObj, 3))
	{
		IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( lpMsg->Pos > MAIN_INVENTORY_SIZE-1 && lpMsg->Pos != 236 )
	{
		IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		g_Log.Add("Item location value exceeds: %d", lpMsg->Pos);
		return;
	}

	if ( gObjCheckSerial0ItemList(&lpObj->pInventory[lpMsg->Pos]) != FALSE )
	{
		MsgOutput(aIndex, Lang.GetText(0,259));
		IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

		g_Log.Add("[ANTI-HACK][Serial 0 Item] [Sell Item] (%s)(%s) Item(%s) Pos(%d)",
			lpObj->AccountID, lpObj->Name, lpObj->pInventory[lpMsg->Pos].GetName(), lpMsg->Pos);

		return;
	}

	if (g_ConfigRead.data.common.ShopExcItemLimit == FALSE)
	{
		if (lpObj->pInventory[lpMsg->Pos].m_NewOption == 63)
		{
			IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
			return;
		}
	}

	if(g_ConfigRead.data.common.EnableSellAllItems == false)
	{
		if ( lpObj->pInventory[lpMsg->Pos].m_Type == ITEMGET(14,11) ) // Box of Luck
		{
			IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
			return;
		}

		if ( lpObj->pInventory[lpMsg->Pos].m_QuestItem != false && !g_QuestExpManager.IsQuestItemAtt(lpObj->pInventory[lpMsg->Pos].m_Type, 8) ) 
		{
			IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
			return;
		}
	
		if ( lpObj->pInventory[lpMsg->Pos].m_Type == ITEMGET(13,20) && lpObj->pInventory[lpMsg->Pos].m_Level == 1) // Wizards Ring +1
		{
			IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
			return;
		}

		if ( lpObj->pInventory[lpMsg->Pos].m_Type == ITEMGET(13,20) && lpObj->pInventory[lpMsg->Pos].m_Level == 2) // Wizards Ring +2
		{
			IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
			return;
		}

		if ( lpObj->pInventory[lpMsg->Pos].m_Type == ITEMGET(12,26) &&
			(lpObj->pInventory[lpMsg->Pos].m_Level == 1 ||
			lpObj->pInventory[lpMsg->Pos].m_Level == 2 || 
			lpObj->pInventory[lpMsg->Pos].m_Level == 3 || 
			lpObj->pInventory[lpMsg->Pos].m_Level == 4 || 
			lpObj->pInventory[lpMsg->Pos].m_Level == 5 ))
		{
			IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
			return;
		}

		if ( g_kJewelOfHarmonySystem.IsStrengthenByJewelOfHarmony(&lpObj->pInventory[lpMsg->Pos]) == TRUE && g_ConfigRead.data.common.joinmuRemoveItemSellDropRestriction == false)
		{
			GCServerMsgStringSend(Lang.GetText(0,267), lpObj->m_Index, 1);
			IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
			return;
		}

		if (lpObj->pInventory[lpMsg->Pos].IsPeriodItem() == TRUE && lpObj->pInventory[lpMsg->Pos].IsPeriodItemExpire() == FALSE)
		{
			IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
			return;
		}

		if ( lpObj->pInventory[lpMsg->Pos].IsPeriodItem() == TRUE )
		{
			if ( lpObj->pInventory[lpMsg->Pos].m_Type == ITEMGET(12,266) ||
				lpObj->pInventory[lpMsg->Pos].m_Type == ITEMGET(12,267) )
			{
				lpObj->pInventory[lpMsg->Pos].SetPeriodItemExpire();
				g_PeriodItemEx.RemovePeriodItemDataByForce(lpObj, lpObj->pInventory[lpMsg->Pos].m_Type, lpObj->pInventory[lpMsg->Pos].m_Number);
			}

			else if ( lpObj->pInventory[lpMsg->Pos].IsPeriodItemExpire() == FALSE )
			{
				IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
				return;
			}
		}
	}

	if ( lpObj->pInventory[lpMsg->Pos].m_Type == ITEMGET(14,12) && lpObj->pInventory[lpMsg->Pos].m_Level == 1) // Heart+1
	{
		pResult.Result = true;
		pResult.Money = lpObj->m_PlayerData->Money;
		gObjInventoryItemSet(aIndex, lpMsg->Pos, -1);
		lpObj->pInventory[lpMsg->Pos].Clear();
		IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

		return;
	}

	if (!IsSellToNPCItem(lpObj->pInventory[lpMsg->Pos].m_Type))
	{
		IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( lpObj->pInventory[lpMsg->Pos].IsItem() == FALSE )
	{
		pResult.Result = false;
		pResult.Money = lpObj->m_PlayerData->Money;
		g_Log.Add("Invalid item index %s %d (%s) Pos: %d", __FILE__, __LINE__, lpObj->Name, lpMsg->Pos);
		g_Log.Add("ID: %s CharID: %s ", lpObj->AccountID, lpObj->Name);
		IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

	}
	else
	{
		BYTE NewOption[MAX_EXOPTION_SIZE];
		int iAddZen = 0;
		int type = lpObj->pInventory[lpMsg->Pos].m_Type;

		if ( type == ITEMGET(13,4) || type == ITEMGET(13,5)) // Pets
			lpObj->pInventory[lpMsg->Pos].PetValue();
		else
			lpObj->pInventory[lpMsg->Pos].Value();

		iAddZen = lpObj->pInventory[lpMsg->Pos].m_SellMoney;

		/*if (lpObj->m_PlayerData->VipType != 0) // not used for now
		{
			iAddZen *= g_VipSystem.GetShopSellPriceMultipler(lpObj);
		}*/

		if ( gObjCheckMaxZen(aIndex, iAddZen) == FALSE )
		{
			IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
			return;
		}

		lpObj->m_PlayerData->Money += iAddZen;
		pResult.Result = true;
		pResult.Money = lpObj->m_PlayerData->Money;

		

		ItemIsBufExOption(NewOption, &lpObj->pInventory[lpMsg->Pos]);

		g_Log.Add("[%s][%s] Shop [MAP:%d][X:%d][Y:%d] Item Sell Money:%d %s:%d/%d/%d/%d/%I64d Dur:%d Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d] 380:[%d] HO:[%d,%d] E:[%d]", 
			lpObj->AccountID, lpObj->Name,
			lpShopData->btMapNumber, lpShopData->btPosX, lpShopData->btPosY,
			iAddZen, lpObj->pInventory[lpMsg->Pos].GetName(), 
			lpObj->pInventory[lpMsg->Pos].m_Level, lpObj->pInventory[lpMsg->Pos].m_Option1,
			lpObj->pInventory[lpMsg->Pos].m_Option2, lpObj->pInventory[lpMsg->Pos].m_Option3,
			lpObj->pInventory[lpMsg->Pos].m_Number, (int)lpObj->pInventory[lpMsg->Pos].m_Durability,
			NewOption[0], NewOption[1], NewOption[2], NewOption[3], NewOption[4], NewOption[5], NewOption[6], 
			lpObj->pInventory[lpMsg->Pos].m_SetOption, lpObj->pInventory[lpMsg->Pos].m_ItemOptionEx >> 7,
			g_kJewelOfHarmonySystem.GetItemStrengthenOption(&lpObj->pInventory[lpMsg->Pos]),
			g_kJewelOfHarmonySystem.GetItemOptionLevel(&lpObj->pInventory[lpMsg->Pos]),
			lpObj->pInventory[lpMsg->Pos].m_BonusSocketOption);

		if (g_PentagramSystem.IsPentagramItem(&lpObj->pInventory[lpMsg->Pos]))
		{
			g_PentagramSystem.DelPentagramJewelInfo(aIndex, &lpObj->pInventory[lpMsg->Pos]);
		}

		bool bSendEquip = false;

		if ((lpObj->pInventory[lpMsg->Pos].m_Type == ITEMGET(13,169) || lpObj->pInventory[lpMsg->Pos].m_Type == ITEMGET(13,170)) && (lpMsg->Pos == 10 || lpMsg->Pos == 11))
		{
			bSendEquip = true;
		}

		if (IsExpensiveItem(&lpObj->pInventory[lpMsg->Pos]) == TRUE && lpObj->pInventory[lpMsg->Pos].IsPeriodItem() == FALSE)
		{
			g_CancelItemSale.GDReqAddItemToList(aIndex, lpObj->pInventory[lpMsg->Pos], iAddZen);
		}

		::gObjInventoryDeleteItem(aIndex, lpMsg->Pos);

		if (bSendEquip == true)
		{
			this->GCEquipmentSend(aIndex);
		}

		if ( lpMsg->Pos == 10 || lpMsg->Pos == 11 )
		{
			::gObjUseSkill.SkillChangeUse(aIndex);
		}

		::gObjMakePreviewCharSet(aIndex);

		//GCItemListSend(lpObj->m_Index); // with this, user CAN NOT bug uniria and dinorant
		//g_PeriodItemEx.OnRequestPeriodItemList(lpObj); // cause visual bug while selling items to shop using rmb
	}

	IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
}


int GameProtocol::GetNeedMoneyItemDurRepaire(/*int aIndex, */CItem *  DurItem, BOOL RequestPos)
{
	if (g_ConfigRead.server.GetServerType() == SERVER_BATTLECORE)
	{
		return 1;
	}

	int iMoney = 0;
	int type = DurItem->m_Type;
	float ItemDurability = DurItem->m_Durability;

	if ( type == ITEMGET(13,10) ) // Transformation Ring
		return 0;

	if ( ::HasItemDurability(type ) != FALSE )
	{
		float lc4;
		float lc5 = 0;
		lc4 = DurItem->m_BaseDurability;

		if ( ItemDurability == lc4 )
			return 0;

		float lc6 = 1.0f - ItemDurability / lc4;
		int lc7;

		if ( type == ITEMGET(13,4)|| type == ITEMGET(13,5) ) // Pets
		{
			lc7 = DurItem->m_BuyMoney;
		}
		else
		{
			lc7 = DurItem->m_BuyMoney/3;

			/* switch (lpObj->m_PlayerData->VipType) // temporary disabled as requires display price fix on client side
			{
				case 1:
					lc7 = DurItem->m_BuyMoney * BronzeVipBuyPriceMultiplier;
					break;
				case 2:
					lc7 = DurItem->m_BuyMoney * SilverVipBuyPriceMultiplier;
					break;
				case 3:
					lc7 = DurItem->m_BuyMoney * GoldVipBuyPriceMultiplier;
					break;
				case 4:
					lc7 = DurItem->m_BuyMoney * PlatinumVipBuyPriceMultiplier;
					break;
				default:
					lc7 = DurItem->m_BuyMoney * gItemBuyPriceMultiplier;
					break;
			} */
		}

		if ( lc7 > 400000000 ) lc7 = 400000000;

		if ( lc7 >= 1000 )
			lc7 = lc7/100*100;
		else if ( lc7 >= 100 )
			lc7 = lc7/10*10;

		float lc8 = sqrt(static_cast<float>(lc7));
		float lc9 = sqrt(sqrt(static_cast<float>(lc7))); 
		lc5 = 3.0f * lc8 * lc9;
		lc5 *= lc6;
		lc5 += 1.0f;

		if ( ItemDurability <= 0.0f )
		{
			if ( type == ITEMGET(13,4)|| type ==ITEMGET(13,5) ) // Pets
				lc5 *= 2;
			else
				lc5 *= ::GetAllRepairItemRate(type);

		}

		if ( RequestPos == TRUE )
		{
			lc5 += lc5 * 0.05f;
		}
		iMoney = lc5;

		if ( iMoney >= 1000 )
			iMoney = iMoney/100*100;
		else if ( iMoney >= 100 )	// Deathway fix Here -> Must be 100 and >=
			iMoney = iMoney/10*10;
		
	}
	else
	{
		iMoney = -1;
	}

	return iMoney;
}



struct PMSG_ITEMDURREPAIR_RESULT
{
	PBMSG_HEAD h;	// C1:34
	int Money;	// 4
};



void GameProtocol::ItemDurRepaire(LPOBJ lpObj, /*int aIndex, */CItem * DurItem, int pos, int RequestPos)
{
	PMSG_ITEMDURREPAIR_RESULT pResult;
	int result = TRUE;

	PHeadSetB((LPBYTE)&pResult, 0x34, sizeof(pResult));
	int itemtype = DurItem->m_Type;

	if ( lpObj->m_IfState.type != 13 )
	{
		if ( itemtype ==ITEMGET(13,4)  || itemtype == ITEMGET(13,5))
		{
			pResult.Money = 0;
			IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);

			return;
		}
	}

	if ( DurItem->IsPeriodItem() == TRUE )
	{
		pResult.Money = 0;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);

		return;
	}

	if ( itemtype >= ITEMGET(14,0) || ( itemtype >= ITEMGET(13,0) &&itemtype  < ITEMGET(13,4) ) ||itemtype == ITEMGET(13,10) || (itemtype >= ITEMGET(12,7) && itemtype <= ITEMGET(12,35) ) ||itemtype ==ITEMGET(4,7) ||itemtype ==ITEMGET(4,15)  )
	{
		pResult.Money = 0;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);

		return;
	}

	if ( itemtype == ITEMGET(13,38) )
	{
		pResult.Money = 0;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);

		return;
	}

	if ( itemtype == ITEMGET(13,39) )
	{
		pResult.Money = 0;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);

		return;
	}

	if ( itemtype == ITEMGET(13,40) )
	{
		pResult.Money = 0;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);

		return;
	}

	if ( itemtype == ITEMGET(13,41) )
	{
		pResult.Money = 0;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);

		return;
	}

	if ( itemtype == ITEMGET(13,42) )
	{
		pResult.Money = 0;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);

		return;
	}

	pResult.Money = GetNeedMoneyItemDurRepaire(/*aIndex, */DurItem, RequestPos);

	if ( pResult.Money <= 0 )
	{
		pResult.Money = 0;
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);

		return;
	}

	if ( (lpObj->m_PlayerData->Money - pResult.Money) < 1 )
		pResult.Money = 0;
	else
	{
		lpObj->m_PlayerData->Money -= pResult.Money;
		pResult.Money = lpObj->m_PlayerData->Money;
		DurItem->m_Durability = (float)((int)DurItem->m_BaseDurability);
		DurItem->Convert(DurItem->m_Type, DurItem->m_Option1, DurItem->m_Option2, DurItem->m_Option3,
			DurItem->m_NewOption, DurItem->m_SetOption,DurItem->m_ItemOptionEx,0, -1, 0, CURRENT_DB_VERSION);
		GCItemDurSend(lpObj->m_Index, pos, DurItem->m_Durability, FALSE);
	}

	IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
}




void GameProtocol::CGModifyRequestItem(PMSG_ITEMDURREPAIR * lpMsg, int aIndex)
{
	if(!ObjectMaxRange(aIndex))
		return;

	LPOBJ lpObj = &gObj[aIndex];
	PMSG_ITEMDURREPAIR_RESULT pResult;

	PHeadSetB((LPBYTE)&pResult, 0x34, sizeof(pResult));
	pResult.Money = 0;

	if ( gObj[aIndex].Type != OBJ_USER )
	{
		return;
	}

	if ( gObj[aIndex].CloseType != -1 )
	{
		IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( !PacketCheckTime(aIndex))
	{
		IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if (lpMsg->Requestpos == 1 && gObj[aIndex].Level < 50 )
	{
		pResult.Money = 0;
		IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( lpMsg->Position == 0xFF )
	{
		for ( int n=0;n<MAIN_INVENTORY_SIZE;n++)
		{
			if ( lpObj->pInventory[n].IsItem())
			{
				if ( IsCashItem(lpObj->pInventory[n].m_Type) == TRUE )
					continue;

				if (IsPremiumItem(lpObj->pInventory[n].m_Type) == TRUE)
					continue;

				if ( lpObj->pInventory[n].IsPeriodItem() == TRUE )
					continue;

				if ( lpObj->pInventory[n].IsPeriodItemExpire() == TRUE )
					continue;

				if (g_LuckyItemManager.IsLuckyItemEquipment(lpObj->pInventory[n].m_Type))	//season 6.1 add-on
					continue;

				if ( lpObj->pInventory[n].m_Type == ITEMGET(13,20) && (lpObj->pInventory[n].m_Level == 0 ||lpObj->pInventory[n].m_Level == 1 ))
					continue;

				if ( lpObj->pInventory[n].m_Type == ITEMGET(13,20) && lpObj->pInventory[n].m_Level == 2 )
					continue;

				if ( lpObj->pInventory[n].m_Type == ITEMGET(13,4) || lpObj->pInventory[n].m_Type == ITEMGET(13,5) )
					continue;
				
				if ( lpObj->pInventory[n].m_Type == ITEMGET(13,37) )
					continue;

				if ( lpObj->pInventory[n].m_Type == ITEMGET(13,38) )
					continue;

				if ( lpObj->pInventory[n].m_Type == ITEMGET(13,40) )
					continue;

				if ( lpObj->pInventory[n].m_Type == ITEMGET(13,41) )
					continue;

				if ( lpObj->pInventory[n].m_Type == ITEMGET(13,42) )
					continue;

				if ( lpObj->pInventory[n].m_Type == ITEMGET(13,51) )
					continue;

				if ( lpObj->pInventory[n].m_Type == ITEMGET(13,64) )
					continue;

				if ( lpObj->pInventory[n].m_Type == ITEMGET(13,65) )
					continue;

				if ( lpObj->pInventory[n].m_Type == ITEMGET(13,67) )
					continue;

				if ( lpObj->pInventory[n].m_Type == ITEMGET(13,70) )
					continue;

				if ( lpObj->pInventory[n].m_Type == ITEMGET(13,76) )
					continue;

				if ( lpObj->pInventory[n].m_Type == ITEMGET(13,77) )
					continue;

				if ( lpObj->pInventory[n].m_Type == ITEMGET(13,78) )
					continue;

				if ( lpObj->pInventory[n].m_Type == ITEMGET(13,80) )
					continue;

				if ( lpObj->pInventory[n].m_Type == ITEMGET(13,106) )
					continue;

				if ( lpObj->pInventory[n].m_Type == ITEMGET(13,123) )
					continue;

				if ( lpObj->pInventory[n].m_Type == ITEMGET(13,39) )
					continue;

				if ( lpObj->pInventory[n].m_Type == ITEMGET(12,26) &&
					(lpObj->pInventory[n].m_Level == 1 ||
					 lpObj->pInventory[n].m_Level == 2 || 
					 lpObj->pInventory[n].m_Level == 3 || 
					 lpObj->pInventory[n].m_Level == 4 || 
					 lpObj->pInventory[n].m_Level == 5 ))
					 continue;

				if (!IsRepairItem(lpObj->pInventory[n].m_Type))
					continue;
					
				ItemDurRepaire(lpObj,/*aIndex,*/ &lpObj->pInventory[n], n, lpMsg->Requestpos);
			}
		}

		gObjCalCharacter.CalcCharacter(lpObj->m_Index);
		return;
	}

	if ( lpMsg->Position > MAIN_INVENTORY_SIZE-1)
	{
		pResult.Money = 0;
		return;
	}

	if ( lpObj->pInventory[lpMsg->Position].IsPeriodItem() == TRUE )
		return;

	if ( IsCashItem(lpObj->pInventory[lpMsg->Position].m_Type) == TRUE )
		return;

	if (IsPremiumItem(lpObj->pInventory[lpMsg->Position].m_Type) == TRUE)
		return;

	if ( lpObj->pInventory[lpMsg->Position].IsPeriodItemExpire() == TRUE )
		return;

	if (g_LuckyItemManager.IsLuckyItemEquipment(lpObj->pInventory[lpMsg->Position].m_Type))	//season 6.1 add-on
		return;

	if ( lpObj->pInventory[lpMsg->Position].m_Type == ITEMGET(13,20) && (lpObj->pInventory[lpMsg->Position].m_Level == 0 ||lpObj->pInventory[lpMsg->Position].m_Level == 1 ))
		return;

	if ( lpObj->pInventory[lpMsg->Position].m_Type == ITEMGET(13,20) && lpObj->pInventory[lpMsg->Position].m_Level == 2 )
		return;

	if ( lpObj->pInventory[lpMsg->Position].m_Type == ITEMGET(13,37) )
		return;

	if ( lpObj->pInventory[lpMsg->Position].m_Type == ITEMGET(13,38) )
		return;

	if ( lpObj->pInventory[lpMsg->Position].m_Type == ITEMGET(13,40) )
		return;

	if ( lpObj->pInventory[lpMsg->Position].m_Type == ITEMGET(13,41) )
		return;

	if ( lpObj->pInventory[lpMsg->Position].m_Type == ITEMGET(13,42) )
		return;

	if ( lpObj->pInventory[lpMsg->Position].m_Type == ITEMGET(13,51) )
		return;

	if ( lpObj->pInventory[lpMsg->Position].m_Type == ITEMGET(13,70) )
		return;

	if ( lpObj->pInventory[lpMsg->Position].m_Type == ITEMGET(13,66) )
		return;

	if ( lpObj->pInventory[lpMsg->Position].m_Type == ITEMGET(13,76) )
		return;
	
	if ( lpObj->pInventory[lpMsg->Position].m_Type == ITEMGET(13,77) )
		return;

	if ( lpObj->pInventory[lpMsg->Position].m_Type == ITEMGET(13,78) )
		return;

	if ( lpObj->pInventory[lpMsg->Position].m_Type == ITEMGET(13,80) )
		return;

	if ( lpObj->pInventory[lpMsg->Position].m_Type == ITEMGET(13,106) )
		return;

	if ( lpObj->pInventory[lpMsg->Position].m_Type == ITEMGET(13,123) )
		return;

	if ( lpObj->pInventory[lpMsg->Position].m_Type == ITEMGET(13,39) )
		return;

	if (!IsRepairItem(lpObj->pInventory[lpMsg->Position].m_Type))
		return;

	ItemDurRepaire(lpObj,/*aIndex,*/ &lpObj->pInventory[lpMsg->Position], lpMsg->Position, lpMsg->Requestpos);
	gObjCalCharacter.CalcCharacter(lpObj->m_Index);
}



struct PMSG_TRADE_REQUESTSEND
{
	PBMSG_HEAD h;	// C3:36
	char szId[10];	// 3
};


void GameProtocol::CGTradeRequestSend(PMSG_TRADE_REQUEST * lpMsg, int aIndex)
{
	if(!ObjectMaxRange(aIndex))
		return;

	int number;

	if (bCanTrade == FALSE )
	{
		this->GCServerMsgStringSend(Lang.GetText(0,57), aIndex, 1);
		this->GCServerMsgStringSend(Lang.GetText(0,58), aIndex, 1);
		return;
	}

	if ( gObj[aIndex].CloseType != -1 )
		return;

	LPOBJ lpObj = &gObj[aIndex];

	if ( !PacketCheckTime(aIndex))
		return;

	number = MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);

	if ( ObjectMaxRange(number) == FALSE )
	{
		g_Log.Add("error : %s %d (%d)", __FILE__, __LINE__, number);
		return;
	}

	if ( number == aIndex )
		return;

	if ( !gObjIsConnected(number))
		return;

	if ( gObj[number].Type != OBJ_USER || gObj[aIndex].Type != OBJ_USER ) // ANTI_CRASH
	{
		g_Log.Add("[ANTI-HACK][Trade] (%s)(%s): Attempting to trade with NON Player object: (%d)", gObj[aIndex].AccountID, gObj[aIndex].Name, number);
		return;
	}

	if ( gObj[aIndex].m_PlayerData->m_bSecurityCheck == false )
	{
		this->GCServerMsgStringSend(Lang.GetText(0,521), aIndex, 1);
		return;
	}

	if ( gObj[number].m_PlayerData->m_bSecurityCheck == false )
	{
		this->GCServerMsgStringSend(Lang.GetText(0,606), aIndex, 1);
		return;
	}

	if ( gObj[number].m_bMapSvrMoveReq == true || gObj[aIndex].m_bMapSvrMoveReq == true )
	{
		g_Log.Add("[CGTradeRequestSend] MapServerMove User. Can't Trade. return!! [%s], IP [%s] -> [%s], IP [%s]",
			gObj[number].AccountID, gObj[number].m_PlayerData->Ip_addr, gObj[aIndex].AccountID, gObj[aIndex].m_PlayerData->Ip_addr);

		return;
	}

	if ( gObj[number].m_bMapSvrMoveQuit == true || gObj[aIndex].m_bMapSvrMoveQuit == true )
	{
		g_Log.Add("[CGTradeRequestSend] m_bMapSvrMoveQuit is true . Can't Trade. return!! [%s], IP [%s] -> [%s], IP [%s]",
			gObj[number].AccountID, gObj[number].m_PlayerData->Ip_addr, gObj[aIndex].AccountID, gObj[aIndex].m_PlayerData->Ip_addr);

		return;
	}

	if ( gObj[number].CloseCount >= 0 )
		return;

	if ( DS_MAP_RANGE(gObj[number].MapNumber) != FALSE )
	{
		this->GCServerMsgStringSend(Lang.GetText(0,12), aIndex, 1);
		return;
	}

	if ( gObj[aIndex].m_bPShopOpen == true )
		return;

	if ( gObj[number].m_bPShopOpen == true )
		return;

	if ( CC_MAP_RANGE(gObj[number].MapNumber) != FALSE )
	{
		this->GCServerMsgStringSend(Lang.GetText(0,114), aIndex, 1);
		return;
	}

	if (gObj[number].MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL)
	{
		this->GCServerMsgStringSend(Lang.GetText(0,572), aIndex, 1);
		return;
	}

	if (gObj[aIndex].m_PlayerData->m_JoinUnityBattle == true ||
		gObj[number].m_PlayerData->m_JoinUnityBattle == true ||
		g_ConfigRead.server.GetServerType() == SERVER_BATTLECORE)
	{
		this->GCServerMsgStringSend(Lang.GetText(0,618), aIndex, 1);
		return;
	}

	if ( IT_MAP_RANGE(gObj[number].MapNumber) != FALSE )
	{
		GCServerMsgStringSend(Lang.GetText(0,290), aIndex, 1);
		return;
	}

	if ( DG_MAP_RANGE(gObj[number].MapNumber) != FALSE )
	{
		GCServerMsgStringSend(Lang.GetText(0,303), aIndex, 1);
		return;
	}

	if ( IMPERIAL_MAP_RANGE(gObj[number].MapNumber) != FALSE )
	{
		GCServerMsgStringSend(Lang.GetText(0,304), aIndex, 1);
		return;
	}

	if ( BC_MAP_RANGE(gObj[aIndex].MapNumber) != FALSE )
	{
		int iBridgeIndex = g_BloodCastle.GetBridgeIndex(gObj[aIndex].MapNumber);

		if ( g_BloodCastle.GetCurrentState(iBridgeIndex) != 1 || g_BloodCastle.CheckCanEnter(iBridgeIndex) == false )
		{
			this->GCServerMsgStringSend(Lang.GetText(0,107), aIndex, 1);
			return;
		}	
	}
	if (gObj[number].m_PlayerData->ISBOT == true && g_BotSystem.GetBotType(number) == BOT_TRADE)
	{
		g_BotSystem.AlchemistTradeOpen(aIndex,number);
		return;
	}
	if (gObj[number].m_PlayerData->ISBOT == true && g_BotSystem.GetBotType(number) == BOT_BUFFER)
	{
		gObj[number].BuffPlayerIndex = aIndex;
		g_BotSystem.BuffPlayer(gObj[number].BufferIndex,aIndex);
		g_Log.Add("[%s][%s] Use buff bot",gObj[aIndex].AccountID,gObj[aIndex].Name);
		return;
	}

	if ( (gObj[number].m_Option &1) != 1 )
	{
		this->GCTradeResponseSend(0, aIndex, gObj[number].Name, 0, 0);
		return;
	}

	if ( lpObj->m_IfState.use > 0 )
	{
		this->GCTradeResponseSend(3, aIndex, gObj[number].Name, 0, 0);
		return;
	}

	if (gObj[number].m_IfState.use > 0 )
	{
		this->GCTradeResponseSend(2, aIndex, gObj[number].Name, 0, 0);
		return;
	}

	if (g_NewPVP.IsDuel(gObj[aIndex]) || g_NewPVP.IsDuel(gObj[number]))
	{
		this->GCServerMsgStringSend(Lang.GetText(0,311), aIndex, 1);
		return;
	}

	if (g_NewPVP.IsObserver(gObj[aIndex]) || g_NewPVP.IsObserver(gObj[number]))
	{
		this->GCServerMsgStringSend(Lang.GetText(0,312), aIndex, 1);
		return;
	}

	if ( ::gObjFixInventoryPointer(aIndex) == false )
	{
		g_Log.Add("[Fix Inv.Ptr] False Location - %s, %d", __FILE__, __LINE__);
	}

	if ( gObj[aIndex].pTransaction == 1 )
	{
		g_Log.Add("[%s][%s] CGTradeRequestSend() Failed : Transaction == 1, IF_TYPE : %d",
			gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_IfState.type);

		return;
	}

	lpObj->m_IfState.use = 1;
	lpObj->m_IfState.state = 0;
	lpObj->m_IfState.type = 1;
	lpObj->TargetNumber = number;
	gObj[number].m_IfState.use = 1;
	gObj[number].m_IfState.state = 0;
	gObj[number].m_IfState.type = 1;
	gObj[number].TargetNumber = aIndex;
	lpObj->m_InterfaceTime = GetTickCount();
	gObj[number].m_InterfaceTime = GetTickCount();
/*	if (gObj[number].m_PlayerData->ISBOT == true && g_BotSystem.GetBotType(number) == BOT_TRADE)
	{
		
		g_Log.Add("[%s][%s] Use Trade bot",gObj[aIndex].AccountID,gObj[aIndex].Name);
		PMSG_TRADE_RESPONSE tr;
		tr.Response = true;
		CGTradeResponseRecv(&tr,number);
	//	gObj[
		for(int i=11;i<INVENTORY_SIZE;i++)
		{
			if(gObj[number].pInventory[i].IsItem())
			{

				PMSG_INVENTORYITEMMOVE trM;
				trM.sItemInfo[I_TYPE] = gObj[number].pInventory[i].m_Type;
				trM.sItemInfo[I_OPTION] = gObj[number].pInventory[i].m_Option1;
				trM.sItemInfo[I_DUR] = gObj[number].pInventory[i].m_Durability;
				trM.sItemInfo[I_NOPTION] = gObj[number].pInventory[i].m_NewOption;
				trM.sItemInfo[I_SOPTION] = gObj[number].pInventory[i].m_SetOption;
				trM.sItemInfo[I_380OPTION] = gObj[number].pInventory[i].m_ItemOptionEx;
				trM.sItemInfo[I_JOHOPTION] = gObj[number].pInventory[i].m_JewelOfHarmonyOption;
				trM.sItemInfo[I_SOCKET1] = gObj[number].pInventory[i].m_SocketOption[0];
				trM.sItemInfo[I_SOCKET2] = gObj[number].pInventory[i].m_SocketOption[1];
				trM.sItemInfo[I_SOCKET3] = gObj[number].pInventory[i].m_SocketOption[2];
				trM.sItemInfo[I_SOCKET4] = gObj[number].pInventory[i].m_SocketOption[3];
				trM.sItemInfo[I_SOCKET5] = gObj[number].pInventory[i].m_SocketOption[4];
				trM.sFlag = 0;
				trM.tFlag = 1;
				trM.source_item_num =i;
				trM.target_item_num =1;
				CGInventoryItemMove(&trM,number);
				GCTradeOkButtonSend(aIndex, 1);
			}
		}
	}*/

	PMSG_TRADE_REQUESTSEND pMsg;

	pMsg.h.c = 0xC3;
	pMsg.h.headcode = 0x36;
	pMsg.h.size = sizeof(pMsg);
	memcpy(pMsg.szId, gObj[aIndex].Name, sizeof(pMsg.szId));

	IOCP.DataSend(number, (LPBYTE)&pMsg, pMsg.h.size);

	g_Log.Add("Request to trade [%s][%s][%s] to [%s][%s][%s]", gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_PlayerData->Ip_addr,
		gObj[number].AccountID, gObj[number].Name, gObj[number].m_PlayerData->Ip_addr);

	TradeLog->Output("Request to trade [%s][%s][%s] to [%s][%s][%s]", gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_PlayerData->Ip_addr,
		gObj[number].AccountID, gObj[number].Name, gObj[number].m_PlayerData->Ip_addr);
}



void GameProtocol::CGTradeResponseRecv(PMSG_TRADE_RESPONSE * lpMsg, int aIndex)
{	
	if ( ObjectMaxRange(aIndex) == FALSE )
	{
		g_Log.Add("error-L1 : %d %s %d", aIndex, __FILE__, __LINE__);
		return;
	}

	BOOL Result = TRUE;
	int number = gObj[aIndex].TargetNumber;

	if ( ObjectMaxRange(number) == FALSE )
	{
		if ( gObj[aIndex].m_IfState.use != FALSE )
		{
			if ( gObj[aIndex].m_IfState.type == 1 )
			{
				gObj[aIndex].m_IfState.use = FALSE;
				gObj[aIndex].TargetNumber = -1;
			}
		}

		g_Log.Add("error-L1 : target:%d (A_ID:%s) %s %d", number, gObj[aIndex].AccountID, __FILE__, __LINE__);
		lpMsg->Response = false;
		GCTradeResponseSend(lpMsg->Response, aIndex, gObj[aIndex].Name, 0, 0);
		return;
	}

	if ( gObjIsConnectedGP(aIndex) == FALSE )
	{
		Result = FALSE;
		lpMsg->Response = false;
	}

	if ( gObjIsConnectedGP(number) == FALSE )
	{
		Result = FALSE;
		lpMsg->Response = false;
	}
	
	if ( gObj[aIndex].CloseType != -1 )
	{
		Result = FALSE;
		lpMsg->Response = false;
	}

	if ( gObj[aIndex].Type == OBJ_USER && gObj[aIndex].m_bPShopOpen == true )
	{
		Result = FALSE;
		lpMsg->Response = false;
	}

	if ( gObj[number].Type == OBJ_USER && gObj[number].m_bPShopOpen == true )
	{
		Result = FALSE;
		lpMsg->Response = false;
	}

	if ( gObj[aIndex].X < (gObj[number].X -2 ) || gObj[aIndex].X > (gObj[number].X +2 ) || gObj[aIndex].Y < (gObj[number].Y -2 ) || gObj[aIndex].Y > (gObj[number].Y +2 ) )
	{
		Result = FALSE;
		lpMsg->Response = false;
	}

	if ( gObj[aIndex].m_PlayerData->ISBOT == FALSE)
	{
		if(gObj[aIndex].m_IfState.use == FALSE || gObj[aIndex].m_IfState.type != 1 || gObj[aIndex].m_IfState.state != 0  )
		{
			g_Log.Add("(%s)(%s) Trade Interface State Error : use:%d type:%d", gObj[aIndex].AccountID, gObj[aIndex].Name, 
				gObj[aIndex].m_IfState.use, gObj[aIndex].m_IfState.type);

			return;
		}
	}

	if ( gObj[number].m_IfState.use == FALSE || gObj[number].m_IfState.type != 1 || gObj[number].m_IfState.state != 0 )
	{
		g_Log.Add("(%s)(%s) Trade Interface State Error : use:%d type:%d", gObj[number].AccountID, gObj[number].Name, 
			gObj[number].m_IfState.use, gObj[number].m_IfState.type);

		return;
	}

	if ( gObjFixInventoryPointer(aIndex) == false )
	{
		g_Log.Add("[Fix Inv.Ptr] False Location - %s, %d", __FILE__, __LINE__);
	}

	if ( gObj[aIndex].pTransaction == 1 && gObj[aIndex].m_PlayerData->ISBOT == FALSE )
	{
		g_Log.Add("[%s][%s] CGTradeResponseRecv() Failed : Transaction == 1, IF_TYPE : %d",
			gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_IfState.type);

		return;
	}

	if ( lpMsg->Response == false )
	{
		GCTradeResponseSend(lpMsg->Response, number, gObj[aIndex].Name, 0, 0);
		Result = FALSE;
	}
	else
	{
		Result = TRUE;

		for ( int i=0;i<TRADE_BOX_SIZE;i++)
		{
			gObj[aIndex].Trade[i].Clear();
			gObj[number].Trade[i].Clear();
		}

		memset(gObj[aIndex].TradeMap, (BYTE)-1, TRADE_BOX_MAP_SIZE );
		memset(gObj[number].TradeMap, (BYTE)-1, TRADE_BOX_MAP_SIZE );
		gObj[aIndex].m_IfState.state = 1;
		gObj[number].m_IfState.state = 1;
		gObj[aIndex].TradeMoney = 0;
		gObj[number].TradeMoney = 0;

		if (::gObjInventoryTrans(aIndex) == FALSE )
			Result = 2;

		if (::gObjInventoryTrans(number) == FALSE )
			Result = 3;

		if ( Result != TRUE )
		{
			lpMsg->Response = false;
			GCTradeResponseSend(lpMsg->Response, number, gObj[aIndex].Name, 0, 0);
			GCTradeResponseSend(lpMsg->Response, aIndex, gObj[number].Name, 0, 0);

			if ( Result == 2 )
			{
				g_Log.Add("[%s][%s] Made Trade Error (in Transaction) [%s][%s]",
					gObj[aIndex].AccountID, gObj[aIndex].Name,
					gObj[number].AccountID, gObj[number].Name);
			}
			else if ( Result == 3 )
			{
				g_Log.Add("[%s][%s] Made Trade Error (in Transaction) [%s][%s]",
					gObj[number].AccountID, gObj[number].Name,
					gObj[aIndex].AccountID, gObj[aIndex].Name);
			}

			Result = FALSE;
		}
		else
		{
			lpMsg->Response = true;
			GCTradeResponseSend(lpMsg->Response, number, gObj[aIndex].Name, gObj[aIndex].Level, gObj[aIndex].m_PlayerData->GuildNumber);
			GCTradeResponseSend(lpMsg->Response, aIndex, gObj[number].Name, gObj[number].Level, gObj[number].m_PlayerData->GuildNumber);

			g_Log.Add("[%s][%s] Trade Ready [%s][%s]", 
				gObj[number].AccountID, gObj[number].Name,
				gObj[aIndex].AccountID, gObj[aIndex].Name);
		}
	}

	if ( Result == FALSE )
	{
		if ( gObj[aIndex].m_IfState.use != FALSE && gObj[aIndex].m_IfState.type == 1 )
		{
			gObj[aIndex].m_IfState.use = FALSE;
			gObj[aIndex].TargetNumber = -1;
			g_Log.Add("Interface State : %d", gObj[aIndex].m_IfState.use);
		}

		if ( gObj[number].TargetNumber == aIndex )
		{
			if ( gObj[number].m_IfState.use != FALSE && gObj[number].m_IfState.type == 1 )
			{
				gObj[number].m_IfState.use = FALSE;
				gObj[number].TargetNumber = -1;
				g_Log.Add("Interface State : %d", gObj[number].m_IfState.use);
			}
		}
	}
}




bool GameProtocol::GCTradeResponseSend(BYTE response, int aIndex, LPSTR id, WORD level, int GuildNumber)
{
	PMSG_TRADE_RESPONSE pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x37, sizeof(pMsg));
	pMsg.Response = response;
	pMsg.Level = level;
	pMsg.GuildNumber = GuildNumber;
	memcpy(pMsg.Id, id, sizeof(pMsg.Id));

	return IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

struct PMSG_TRADE_OTHER_DEL
{
	PBMSG_HEAD h;	// C1:38
	BYTE TradeItemIndex;	// 3
};

int GameProtocol::GCTradeOtherDel(int aIndex, BYTE tradeindex)
{
	PMSG_TRADE_OTHER_DEL pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x38, sizeof(pMsg ));
	pMsg.TradeItemIndex = tradeindex;

	return IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

struct PMSG_TRADE_OTHER_MOVE
{
	PBMSG_HEAD h;	// C1:39
	BYTE TradeItemIndex;	// 3
	BYTE ItemInfo[MAX_ITEM_INFO];	// 4
};


int GameProtocol::GCTradeOtherAdd(int aIndex, BYTE tradeindex, LPBYTE iteminfo)
{
	PMSG_TRADE_OTHER_MOVE pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x39, sizeof(pMsg));
	pMsg.TradeItemIndex = tradeindex;
	pMsg.ItemInfo[I_TYPE] = iteminfo[I_TYPE];
	pMsg.ItemInfo[I_OPTION] = iteminfo[I_OPTION];
	pMsg.ItemInfo[I_DUR] = iteminfo[I_DUR];
	pMsg.ItemInfo[I_NOPTION] = iteminfo[I_NOPTION];
	pMsg.ItemInfo[I_SOPTION] = iteminfo[I_SOPTION];
	pMsg.ItemInfo[5] = iteminfo[5];
	pMsg.ItemInfo[6] = iteminfo[6];
	pMsg.ItemInfo[7] = iteminfo[7];
	pMsg.ItemInfo[8] = iteminfo[8];
	pMsg.ItemInfo[9] = iteminfo[9];
	pMsg.ItemInfo[10] = iteminfo[10];
	pMsg.ItemInfo[11] = iteminfo[11];
	

	return IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}



void GameProtocol::CGTradeMoneyRecv(PMSG_TRADE_GOLD * lpMsg, int aIndex)
{
	PMSG_DEFRESULT pMsg;
	int money = 0;
	int number;

	if ( !PacketCheckTime(aIndex) )
		return;

	if ( !ObjectMaxRange(aIndex) )
	{
		g_Log.Add("error: %s %d (%d)", __FILE__, __LINE__, aIndex);
		return;
	}

	if ( gObj[aIndex].CloseType != -1 )
	{
		g_Log.Add("error: [%s][%s] Trade broken by disconect", gObj[aIndex].AccountID, gObj[aIndex].Name);
		return;
	}

	number = gObj[aIndex].TargetNumber;

	if ( !ObjectMaxRange(number) )
	{
		g_Log.Add("error: Trade failed because the opposite does not exist [%s][%s][%d]", gObj[aIndex].AccountID, gObj[aIndex].Name, number);
		return;
	}

	if ( gObj[number].CloseType != -1 )
	{
		g_Log.Add("error: Trade failed because the opposite is disconnecting [%s][%s][%s][%s]", gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[number].AccountID, gObj[number].Name);
		return;
	}

	if ( gObj[aIndex].Type != OBJ_USER )
	{
		return;
	}

	if ( gObj[number].Type != OBJ_USER )
	{
		return;
	}

	if ( gObjIsConnectedGP(number) == FALSE )
	{
		return;
	}

	if ( gObj[aIndex].m_IfState.use == FALSE && gObj[aIndex].m_IfState.type != 1 )
		return;

	if ( gObjCanItemTouch(&gObj[aIndex], 1) == FALSE )
		return;

	if ( lpMsg->Money > 1000000000 )
		return;

	money = lpMsg->Money;

	if ( (gObj[aIndex].m_PlayerData->Money - money) < 0 )
		return;

	GCMoneySend(aIndex, gObj[aIndex].m_PlayerData->Money - money);

	PHeadSetB((LPBYTE)&pMsg, 0x3A, sizeof(pMsg));
	pMsg.result = 1;

	IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

	if ( number >= 0)
	{
		gObj[aIndex].TradeMoney = money;
		gObj[aIndex].TradeOk  = false;
		gObj[number].TradeOk  = false;

		GCTradeOkButtonSend(number, 2);
		GCTradeOkButtonSend(aIndex, 0);
		GCTradeMoneyOther(number, gObj[aIndex].TradeMoney);
	}

	g_Log.Add("[%s][%s][%s] -> [%s][%s][%s]: Trade Money %d",gObj[aIndex].AccountID,gObj[aIndex].Name,gObj[aIndex].m_PlayerData->Ip_addr,gObj[number].AccountID,gObj[number].Name,gObj[number].m_PlayerData->Ip_addr,money);
	TradeLog->Output("[%s][%s][%s] -> [%s][%s][%s]: Trade Money %d",gObj[aIndex].AccountID,gObj[aIndex].Name,gObj[aIndex].m_PlayerData->Ip_addr,gObj[number].AccountID,gObj[number].Name,gObj[number].m_PlayerData->Ip_addr,money);
}

struct PMSG_TRADE_OTHER_GOLD
{
	PBMSG_HEAD h;	// C1:3B
	DWORD Money;	// 4
};


bool GameProtocol::GCTradeMoneyOther(int aIndex, DWORD money)
{
	PMSG_TRADE_OTHER_GOLD pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x3B, sizeof(pMsg));
	pMsg.Money = money;

	return IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);	
}



bool GameProtocol::GCTradeOkButtonSend(int aIndex, BYTE flag)
{
	PMSG_TRADE_OKBUTTON pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x3C, sizeof(pMsg));
	pMsg.Flag = flag;
	/*if(gObj[gObj[aIndex].TargetNumber].m_PlayerData->ISBOT == TRUE && g_BotSystem.GetBotType(gObj[aIndex].TargetNumber) == BOT_TRADE)
	{
		g_BotSystem.AlchemistTradeOk(aIndex,gObj[aIndex].TargetNumber);
		return true;
	}*/
	return IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}


void GameProtocol::CGTradeOkButtonRecv(PMSG_TRADE_OKBUTTON * lpMsg, int aIndex)
{
	int number = gObj[aIndex].TargetNumber;

	if ( number < 0 || number > g_ConfigRead.server.GetObjectMax()-1 )
		return;

	if(gObj[number].Type == OBJ_USER && gObj[number].m_PlayerData->ISBOT == 1)
	{
		if(lpMsg->Flag == 1)
		{
			g_BotSystem.AlchemistTradeOk(aIndex,number);
			return;
		}
	}

	if ( !gObjIsConnected(&gObj[number]))
		return;

	if ( gObj[aIndex].Type != OBJ_USER )
		return;

	if ( gObj[number].Type != OBJ_USER )
		return;

	if ( gObj[aIndex].m_IfState.use < 1 || gObj[aIndex].m_IfState.state != 1 )
		return;

	if(!gObj[number].m_PlayerData->ISBOT)
	{
		if ( gObj[number].m_IfState.use < 1 || gObj[number].m_IfState.state != 1 )
			return;
	}
	if ( gObj[aIndex].CloseType != -1 )
		return;

	if ( gObj[number].CloseType != -1 )
		return;
	
	if ( lpMsg->Flag == 1 )
	{
		if ( gObj[aIndex].TradeOk == false )
		{
			gObj[aIndex].TradeOk = true;
			g_Log.Add("[%s][%s] Trade Accept", gObj[aIndex].AccountID, gObj[aIndex].Name);
			GCTradeOkButtonSend(number, 1);
		}
	}
	else if ( lpMsg->Flag == 0 )
	{
		if ( gObj[aIndex].TradeOk  == true )
		{
			GCTradeOkButtonSend(number, 0);
			gObj[aIndex].TradeOk = false;
		}
	}

	gObjTradeOkButton(aIndex);
	g_Log.Add("[%s][%s][%s] -> [%s][%s][%s]: Trade Accept!",gObj[aIndex].AccountID,gObj[aIndex].Name,gObj[aIndex].m_PlayerData->Ip_addr,gObj[number].AccountID,gObj[number].Name,gObj[number].m_PlayerData->Ip_addr);
	TradeLog->Output("[%s][%s][%s] -> [%s][%s][%s]: Trade Accept!",gObj[aIndex].AccountID,gObj[aIndex].Name,gObj[aIndex].m_PlayerData->Ip_addr,gObj[number].AccountID,gObj[number].Name,gObj[number].m_PlayerData->Ip_addr);
}


void GameProtocol::CGTradeCancelButtonRecv(int aIndex)
{
	if (gObj[aIndex].Type != OBJ_USER)
	{
		return;
	}

	int number = gObj[aIndex].TargetNumber;
	gObjTradeCancel(aIndex);
	CGTradeResult(aIndex, 0);

	if ( number >= 0 )
	{
		gObjTradeCancel(number);
		CGTradeResult(number, 0);
	}

	g_Log.Add("[%s][%s][%s]: Trade Cancel Button Press",gObj[aIndex].AccountID,gObj[aIndex].Name,gObj[aIndex].m_PlayerData->Ip_addr);
	TradeLog->Output("[%s][%s][%s]: Trade Cancel Button Press",gObj[aIndex].AccountID,gObj[aIndex].Name,gObj[aIndex].m_PlayerData->Ip_addr);
}


struct PMSG_TRADE_RESULT
{
	PBMSG_HEAD h;	// C1:3D
	BYTE Result;	// 3
};


void GameProtocol::CGTradeResult(int aIndex , BYTE result )
{
	PMSG_TRADE_RESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x3D, sizeof(pMsg));
	pMsg.Result = result;

	IOCP.DataSend(aIndex, (unsigned char*)&pMsg, pMsg.h.size);
}

struct PMSG_PARTYREQUESTSEND
{
	PBMSG_HEAD h;	// C1:40
	BYTE NumberH;	// 3
	BYTE NumberL;	// 4
};

void GameProtocol::CGPartyRequestRecv(PMSG_PARTYREQUEST * lpMsg, int aIndex)
{
	int number =  MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);
	PMSG_PARTYREQUESTSEND pMsg;

	if ( number < 0 || number > g_ConfigRead.server.GetObjectMax()-1)
		return;

	if (!PacketCheckTime(aIndex))
		return;

	if ( !gObjIsConnected(&gObj[number]))
	{
		this->GCResultSend(aIndex, 0x41, 0x03);
		return;
	}

	if ( gObj[aIndex].CloseCount >= 0 || gObj[number].CloseCount >= 0 )
	{
		this->GCResultSend(aIndex, 0x41, 0x00);
		return;
	}

	if (gObj[number].m_PlayerData->m_bUsePartyMatching)
	{
		GCResultSend(aIndex, 0x41, 0x09);
		return;
	}

	if ( gObj[number].Type != OBJ_USER )
	{
		g_Log.AddC(TColor::Red, "[HACKTOOL] : NPC-PartyRequest npc:%d ip:%s account:%s name:%s State:%d",
			gObj[number].Class, gObj[aIndex].m_PlayerData->Ip_addr, gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].Connected);
		this->GCSendDisableReconnect(aIndex);
		//IOCP.CloseClient(aIndex);
		return;
	}

	if ( g_NewPVP.IsDuel(gObj[aIndex]) || g_NewPVP.IsDuel(gObj[number]) )
	{
		this->GCServerMsgStringSend(Lang.GetText(0,315), aIndex, 1);
		return;
	}

	if ( g_NewPVP.IsObserver(gObj[aIndex]) )
	{
		this->GCServerMsgStringSend(Lang.GetText(0,316), aIndex, 1);
		return;
	}

	if (CC_MAP_RANGE(gObj[aIndex].MapNumber))
	{
		this->GCServerMsgStringSend(Lang.GetText(0,116), aIndex, 1);
		return;
	}

	if (gObj[aIndex].MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL)
	{
		this->GCServerMsgStringSend(Lang.GetText(0,573), aIndex, 1);
		return;
	}

	if ( IT_MAP_RANGE(gObj[aIndex].MapNumber) )
	{
		this->GCServerMsgStringSend(Lang.GetText(0,291), aIndex, 1);
		return;
	}

	g_Log.Add("Party result : %d %d %d %d", gObj[aIndex].CloseCount, gObj[number].CloseCount, gObj[aIndex].Connected, gObj[number].Connected);
	g_Log.Add("%s %s", gObj[aIndex].Name, gObj[number].Name);

	if ( gObj[aIndex].m_IfState.use > 0 )
	{
		this->GCResultSend(aIndex, 0x41, 0x00);
		return;
	}
		
	if ( gObj[number].m_IfState.use > 0 )
	{
		this->GCResultSend(aIndex, 0x41, 0x00);
		return;
	}

	if ( (gObj[number].m_Option &1) != TRUE )
	{
		this->GCResultSend(aIndex, 0x41, 0x01);
		return;
	}

	if ( gObj[aIndex].PartyNumber >= 0 )
	{
		int iUserIndex;
		int iDbNumber;
		gParty.GetIndexUser(gObj[aIndex].PartyNumber, 0, iUserIndex, iDbNumber);
		
		if ( iUserIndex < 0 || ( iUserIndex != aIndex && gObj[aIndex].DBNumber != iDbNumber )  )
		{
			this->GCResultSend(aIndex, 0x41, 0x00);
			return;

		}
	}

	if ( gObj[aIndex].PartyTargetUser >= 0 )
		return;

	if ( gObj[number].PartyNumber >= 0 )
	{
		this->GCResultSend(aIndex, 0x41, 0x04);
		return;
	}

	if ( gObj[number].PartyTargetUser >= 0 )
	{
		this->GCResultSend(aIndex, 0x41, 0x00);
		return;
	}

	if ( g_GensSystem.GetGensInfluence(&gObj[aIndex]) != g_GensSystem.GetGensInfluence(&gObj[number]) && g_GensSystem.CanGensJoinPartyWhileOppositeGens() == FALSE )
	{
		this->GCResultSend(aIndex, 0x41, 0x06);
		return;
	}

	if ( g_GensSystem.IsMapBattleZone(gObj[aIndex].MapNumber) == TRUE && g_GensSystem.CanGensCreatePartyOnBattleZone() == FALSE )
	{
		this->GCResultSend(aIndex, 0x41, 0x07);
		return;
	}

	if ( g_ArcaBattle.IsArcaBattleServer() &&  gObj[aIndex].m_PlayerData->GuildNumber != gObj[number].m_PlayerData->GuildNumber) 
	{
		if (g_AcheronGuardianEvent.IsPlayStart() == false)
		{
			GCServerMsgStringSend(Lang.GetText(0,522), aIndex, 1);
			return;
		}
	}

	if(gObj[aIndex].m_PlayerData->RegisterdLMS == 1)
	{
		MsgOutput(aIndex,Lang.GetText(0,523));
		return;
	}

	if(gObj[number].m_PlayerData->RegisterdLMS == 1)
	{
		MsgOutput(aIndex,Lang.GetText(0,524));
		return;
	}
	/*
	int maxlevel = 0;
	int minlevel = 0;

	if ( gObj[number].PartyNumber >= 0 )
	{
		if ( gParty.GetLevel( gObj[number].PartyNumber, maxlevel, minlevel) == TRUE )
		{
			int limmaxlevel;
			int limmaxlevel2;

			if ( maxlevel > gObj[number].Level )
				limmaxlevel = maxlevel;
			else
				limmaxlevel = gObj[number].Level;

			if ( maxlevel < gObj[number].Level )
				limmaxlevel2 = maxlevel;
			else
				limmaxlevel2 = gObj[number].Level;

			if ( (limmaxlevel - limmaxlevel2) > MAX_PARTY_LEVEL_DIFFERENCE )
			{
				this->GCResultSend(aIndex, 0x41, 0x05);
				// Restricted level for the party is %d
				// DEATHWAY FIX HERE
				g_Log.Add("Restricted level for the party is %d [%s:%d]-[%s:%d]", 130, gObj[aIndex].Name, gObj[aIndex].Level, gObj[number].Name, gObj[number].Level);
				return;
			}

			int limminlevel;
			int limminlevel2;

			if ( maxlevel > gObj[number].Level )
				limminlevel = minlevel;
			else
				limminlevel = gObj[number].Level;

			if ( maxlevel < gObj[number].Level )
				limminlevel2 = minlevel;
			else
				limminlevel2 = gObj[number].Level;

			if ( (limminlevel - limminlevel2) > MAX_PARTY_LEVEL_DIFFERENCE )
			{
				this->GCResultSend(aIndex, 0x41, 0x05);
				// Restricted level for the party is %d
				// DEATHWAY FIX HERE
				g_Log.Add("Restricted level for the party is %d [%s:%d]-[%s:%d]", 130, gObj[aIndex].Name, gObj[aIndex].Level, gObj[number].Name, gObj[number].Level);
				return;
			}
		}
	}
	else
	{
		short sMaxMinLevel[2];

		if ( gObj[aIndex].Level > gObj[number].Level )
			sMaxMinLevel[1] = gObj[aIndex].Level;
		else
			sMaxMinLevel[1] = gObj[number].Level;
		
		if ( gObj[aIndex].Level < gObj[number].Level )
			sMaxMinLevel[0] = gObj[aIndex].Level;
		else
			sMaxMinLevel[0] = gObj[number].Level;
		
		if ( (sMaxMinLevel[1]- sMaxMinLevel[0]) >MAX_PARTY_LEVEL_DIFFERENCE)
		{
			this->GCResultSend(aIndex, 0x41, 0x05);
			// Restricted level for the party is %d
			// DEATHWAY FIX HERE
			g_Log.Add("Restricted level for the party is %d [%s:%d]-[%s:%d]", 130, gObj[aIndex].Name, gObj[aIndex].Level, gObj[number].Name, gObj[number].Level);
			return;
		}
	}
	*/

	gObj[aIndex].m_IfState.use = TRUE;
	gObj[aIndex].m_IfState.type = 2;
	gObj[aIndex].m_IfState.state = 0;
	gObj[number].m_IfState.use = TRUE;
	gObj[number].m_IfState.type = 2;
	gObj[number].m_IfState.state = 0;
	gObj[aIndex].TargetNumber = number;
	gObj[number].TargetNumber = aIndex;
	gObj[aIndex].m_InterfaceTime = GetTickCount();
	gObj[number].m_InterfaceTime = GetTickCount();
	gObj[aIndex].PartyTargetUser = number;
	
	PHeadSetB((LPBYTE)&pMsg, 0x40, sizeof(pMsg));
	pMsg.NumberH = SET_NUMBERH(aIndex);
	pMsg.NumberL = SET_NUMBERL(aIndex);

	IOCP.DataSend(number, (LPBYTE)&pMsg, pMsg.h.size);
}



void GameProtocol::CGPartyRequestResultRecv(PMSG_PARTYREQUESTRESULT * lpMsg, int aIndex)
{
	int number;
	BYTE result = false;
	char szTemp[256];
	int pnumber = -1;

	if ( !PacketCheckTime(aIndex))
		return;

	number = MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);

	if ( number < 0 || number > g_ConfigRead.server.GetObjectMax()-1)
		return;

	if ( !gObjIsConnected(&gObj[aIndex]))
		return;

	if ( !gObjIsConnected(&gObj[number]))
	{
		GCResultSend(aIndex, 0x41, 0x04);
	}
	else
	{
		result = true;
	}

	if (gObj[aIndex].m_PlayerData->m_bUsePartyMatching)
	{
		GCResultSend(aIndex, 0x41, 0x09);
		return;
	}

	if ( gObj[number].MapNumber != gObj[aIndex].MapNumber )
	{
		result = false;
		GCResultSend(number, 0x41, 0x00);
	}

	if (gObj[aIndex].MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL)
	{
		this->GCServerMsgStringSend(Lang.GetText(0,573), aIndex, 1);
		result = false;
		GCResultSend(number, 0x41, 0x00);
	}

	if ( CC_MAP_RANGE(gObj[aIndex].MapNumber) )
	{
		GCServerMsgStringSend(Lang.GetText(0,116), aIndex, 1);
		result = false;
		this->GCResultSend(number, 0x41, 0x00);
	}

	if ( lpMsg->Result == 0 )
	{
		result = false;
		this->GCResultSend(number, 0x41, 0x01);
	}

	if ( gObj[number].m_IfState.use == 0 || gObj[number].m_IfState.type != 2)
	{
		result = false;
		this->GCResultSend(number, 0x41, 0x00);
	}	
	
	if ( gObj[aIndex].m_IfState.use == 0 || gObj[aIndex].m_IfState.type != 2)
	{
		result = false;
		this->GCResultSend(number, 0x41, 0x00);
	}

	if ( result == 1 )
	{
		if ( gObj[number].PartyNumber < 0 )
		{
			gObj[number].PartyNumber = gParty.Create(number, gObj[number].DBNumber, gObj[number].Level);
		}

		if ( gObj[number].PartyNumber >= 0  )
		{
			pnumber = gObj[number].PartyNumber;
			int iPartyPos = gParty.Add(gObj[number].PartyNumber, aIndex, gObj[aIndex].DBNumber, gObj[aIndex].Level);

			if ( iPartyPos >= 0 )
			{
				gObj[aIndex].PartyNumber = gObj[number].PartyNumber;
				result = true;

				if (g_ConfigRead.server.GetServerType() == SERVER_BATTLECORE)
				{
					wsprintf(szTemp, Lang.GetText(0,20), gObj[aIndex].m_PlayerData->m_RealNameOfUBF);
					this->GCServerMsgStringSend(szTemp, number, 1);
					wsprintf(szTemp, Lang.GetText(0,20), gObj[number].m_PlayerData->m_RealNameOfUBF);
					this->GCServerMsgStringSend(szTemp, aIndex, 1);
				}

				else
				{
					wsprintf(szTemp, Lang.GetText(0,20), gObj[aIndex].Name);
					this->GCServerMsgStringSend(szTemp, number, 1);
					wsprintf(szTemp, Lang.GetText(0,20), gObj[number].Name);
					this->GCServerMsgStringSend(szTemp, aIndex, 1);
				}

				gParty.Paint(pnumber);

				if (gObj[number].m_PlayerData->m_bUsePartyMatching == true)
				{
					char szLeaderName[MAX_ACCOUNT_LEN + 1];
					memcpy(szLeaderName, gObj[number].Name, MAX_ACCOUNT_LEN + 1);
					GDReqAcceptMemberJoin(number, 1, szLeaderName, gObj[aIndex].Name, 1);
				}
			}
			else if ( iPartyPos == -1 )
			{
				GCResultSend(number, 0x41, 2);
				GCResultSend(aIndex, 0x41, 2);
			}
			else if ( iPartyPos == -2 )
			{
				result = 2;
				wsprintf(szTemp, Lang.GetText(0,21), gObj[aIndex].Name);
				GCServerMsgStringSend(szTemp, aIndex, 1);
			}
		}
	}

	if ( gObj[aIndex].m_IfState.use !=  0 && gObj[aIndex].m_IfState.type == 2)
	{
		gObj[aIndex].m_IfState.use = 0;
		gObj[aIndex].PartyTargetUser = -1;
	}

	if ( gObj[number].m_IfState.use !=  0 && gObj[number].m_IfState.type == 2)
	{
		gObj[number].m_IfState.use = 0;
		gObj[number].PartyTargetUser = -1;
	}

	if ( pnumber >= 0 )
	{
		CGPartyListAll(pnumber);
	}
}

void GameProtocol::CGPartyList(int aIndex)
{
	PMSG_PARTYLISTCOUNT pCount;
	PMSG_PARTYLIST pList;
	BYTE sendbuf[512];
	int lOfs = 0;

	if ( !this->PacketCheckTime(aIndex))
		return;

	PHeadSetB((LPBYTE)&pCount, 0x42, sizeof(pCount));
	pCount.Count = 0;
	pCount.Result = false;

	int number;
	int pnumber = gObj[aIndex].PartyNumber ;

	if ( pnumber >= 0 )
		pCount.Count = gParty.GetCount(pnumber);

	lOfs += sizeof(pCount);

	if ( pCount.Count != 0 )
	{
		for ( int n=0;n<MAX_USER_IN_PARTY;n++)
		{
			number = gParty.m_PartyS[pnumber].Number[n];

			if ( number >= 0 )
			{
				if (gObjIsConnected(&gObj[number], gParty.m_PartyS[pnumber].DbNumber[n]) == TRUE)
				{
					memset(&pList, 0, sizeof(pList));
					memcpy(pList.szId, gObj[number].Name, MAX_ACCOUNT_LEN);
					pList.Number = n;
					pList.MapNumber = gObj[number].MapNumber;
					pList.X = gObj[number].X;
					pList.Y = gObj[number].Y;
					pList.Life = gObj[number].Life;
					pList.MaxLife = gObj[number].MaxLife + gObj[number].AddLife;
					pList.nServerChannel = gObj[number].m_PlayerData->m_nServerChannel + 1;
					pList.nMana = gObj[number].Mana;
					pList.nMaxMana = gObj[number].MaxMana;

					memcpy(&sendbuf[lOfs], &pList, sizeof(pList));
					lOfs += sizeof(pList);
				}
			}
		}

		pCount.Result = true;
	}

	pCount.h.size = lOfs;
	memcpy(sendbuf, &pCount, sizeof(pCount));
	
	IOCP.DataSend(aIndex, (LPBYTE)&sendbuf, lOfs);
}



void GameProtocol::CGPartyListAll(int pnumber)
{
	PMSG_PARTYLISTCOUNT pCount;
	PMSG_PARTYLIST pList;
	BYTE sendbuf[512];
	int lOfs = 0;
	int number;

	if ( pnumber < 0 )
		return;

	if ( gParty.IsParty(pnumber) == FALSE )
		return;

	PHeadSetB((LPBYTE)&pCount, 0x42, sizeof(pCount));
	pCount.Result = false;
	pCount.Count = gParty.GetCount(pnumber);

	if ( pCount.Count == 0 )
		return;

	pCount.Result = true;
	lOfs += sizeof(pCount);

	for ( int n=0;n<MAX_USER_IN_PARTY;n++)
	{
		number = gParty.m_PartyS[pnumber].Number[n];

		if ( number >= 0 )
		{
			if (gObjIsConnected(&gObj[number], gParty.m_PartyS[pnumber].DbNumber[n]) == TRUE)
			{
				memset(&pList, 0, sizeof(pList));

				if (g_ConfigRead.server.GetServerType() == SERVER_BATTLECORE)
				{
					memcpy(pList.szId, gObj[number].m_PlayerData->m_RealNameOfUBF, MAX_ACCOUNT_LEN);
				}

				else
				{
					memcpy(pList.szId, gObj[number].Name, MAX_ACCOUNT_LEN);
				}

				pList.Number = n;
				pList.MapNumber = gObj[number].MapNumber;
				pList.X = gObj[number].X;
				pList.Y = gObj[number].Y;
				pList.Life = gObj[number].Life;
				pList.MaxLife =  gObj[number].MaxLife;
				pList.nServerChannel = gObj[number].m_PlayerData->m_nServerChannel + 1;
				pList.nMana = gObj[number].Mana;
				pList.nMaxMana = gObj[number].MaxMana;

				memcpy(&sendbuf[lOfs], &pList, sizeof(pList));
				lOfs += sizeof(pList);
			}
		}
	}

	pCount.h.size = lOfs;
	memcpy(sendbuf, &pCount, sizeof(pCount));

	for (int n=0;n<MAX_USER_IN_PARTY;n++)
	{
		number = gParty.m_PartyS[pnumber].Number[n];

		if ( number >= 0 )
		{
			IOCP.DataSend(number, sendbuf, lOfs);
		}
	}

	for (int n = 0; n < MAX_USER_IN_PARTY; n++)
	{
		number = gParty.m_PartyS[pnumber].Number[n];

		if (number >= 0)
		{
			this->GCDisplayBuffeffectPartyMember(number);
		}
	}
}


void GameProtocol::CGPartyDelUser(PMSG_PARTYDELUSER * lpMsg, int aIndex, int nType)
{
	int usernumber = -1;
	int dbnumber = -1;

	if (gObj[aIndex].Type != OBJ_USER)
	{
		return;
	}

	if (gObj[aIndex].m_PlayerData->m_bUsePartyMatching == true)
	{
		char szTargetName[MAX_ACCOUNT_LEN + 1];
		memcpy(szTargetName, lpMsg->szTargetName, MAX_ACCOUNT_LEN + 1);

		if (nType == 0)
		{
			memcpy(szTargetName, gObj[aIndex].Name, MAX_ACCOUNT_LEN + 1);
		}

		if (nType == 1)
		{
			GDReqDeletePartyUser(aIndex, szTargetName);
		}

		if (gObj[aIndex].PartyNumber < 0)
		{
			return;
		}

		if (gParty.RevisionIndexUser(gObj[aIndex].PartyNumber, &lpMsg->Number, szTargetName) == FALSE)
		{
			return;
		}

	}

	if ( gObj[aIndex].PartyNumber < 0 )
	{
		g_Log.Add("No Party %s %s", gObj[aIndex].AccountID, gObj[aIndex].Name);
		return;
	}

	if ( !gParty.GetIndexUser(gObj[aIndex].PartyNumber, lpMsg->Number, usernumber, dbnumber))
	{
		g_Log.Add("User Not found");
		return;
	}

	if ( usernumber < 0 )
	{
		g_Log.Add("User Not found");
		return;
	}

	int count;
	int number;
	int pnumber = gObj[aIndex].PartyNumber;

	if ( pnumber < 0 )
	{
		g_Log.Add("%s %d", __FILE__, __LINE__);
		return;
	}

	count = gParty.GetCount(pnumber);

	if ( count < 1 )
	{
		g_Log.Add("%s %d", __FILE__, __LINE__);
		return;
	}

	if (!DG_MAP_RANGE(gObj[usernumber].MapNumber))
	{
		if (!lpMsg->Number)
		{
			BOOL bDPUser = FALSE;

			for (int n = 1; n < 5; n++)
			{
				int iIndex = gParty.m_PartyS[pnumber].Number[n];

				if (iIndex != -1)
				{
					if (DG_MAP_RANGE(gObj[iIndex].MapNumber))
					{
						bDPUser = TRUE;
						break;
					}
				}
			}

			if (bDPUser == TRUE && count > 2)
			{
				gParty.Delete(pnumber, lpMsg->Number);
				count = gParty.GetCount(gObj[aIndex].PartyNumber);
				gObj[usernumber].PartyNumber = -1;
				gObj[usernumber].PartyTargetUser = -1;
				gParty.UpdatePKPartyPanalty(pnumber);
				gParty.ChangeLeader(pnumber);
				this->GCPartyDelUserSend(usernumber);
				this->CGPartyListAll(pnumber);
				g_Log.Add("PartyMember Count(%d) : Party Out. [%s][%s]", count, gObj[aIndex].AccountID, gObj[aIndex].Name);
				return;
			}
		}

		if (!IMPERIAL_MAP_RANGE(gObj[usernumber].MapNumber))
		{
			if (!lpMsg->Number)
			{
				BOOL bIGUser = FALSE;

				for (int i = 1; i < 5; i++)
				{
					int iIndex = gParty.m_PartyS[pnumber].Number[i];
					if (iIndex != -1)
					{
						if (IMPERIAL_MAP_RANGE(gObj[iIndex].MapNumber))
						{
							bIGUser = TRUE;
							break;
						}
					}
				}

				if (bIGUser == TRUE && count > 2)
				{
					gParty.Delete(pnumber, lpMsg->Number);
					count = gParty.GetCount(gObj[aIndex].PartyNumber);
					gObj[usernumber].PartyNumber = -1;
					gObj[usernumber].PartyTargetUser = -1;
					gParty.UpdatePKPartyPanalty(pnumber);
					gParty.ChangeLeader(pnumber);
					this->GCPartyDelUserSend(usernumber);
					this->CGPartyListAll(pnumber);
					return;
				}
			}

			if (gObj[usernumber].m_PlayerData->lpGuild)
			{
				if (gObj[usernumber].m_PlayerData->lpGuild->WarState == 1 && gObj[usernumber].IsInBattleGround)
				{
					this->GCServerMsgStringSend(Lang.GetText(0,574), aIndex, TRUE);
					return;
				}

				if (usernumber == gParty.m_PartyS[pnumber].Number[0])
				{
					for (int j = 0; j < 5; j++)
					{
						int number = gParty.m_PartyS[pnumber].Number[j];
						if (number >= 0
							&& gObj[number].m_PlayerData->lpGuild
							&& gObj[number].m_PlayerData->lpGuild->WarState == TRUE
							&& gObj[number].IsInBattleGround)
						{
							this->GCServerMsgStringSend(Lang.GetText(0,574), aIndex, TRUE);
							return;
						}
					}
				}
			}

			if (lpMsg->Number && count > 2)
			{
				gParty.Delete(pnumber, lpMsg->Number);
				count = gParty.GetCount(gObj[aIndex].PartyNumber);
				gObj[usernumber].PartyNumber = -1;
				gObj[usernumber].PartyTargetUser = -1;
				gParty.UpdatePKPartyPanalty(pnumber);
				this->GCPartyDelUserSend(usernumber);
				CGPartyListAll(pnumber);
			}
			else
			{
				if (!IT_MAP_RANGE(gObj[usernumber].MapNumber) || lpMsg->Number || count <= 2)
				{
					if (IT_MAP_RANGE(gObj[usernumber].MapNumber) && count <= 2)
					{
						gParty.Delete(pnumber, lpMsg->Number);
						count = gParty.GetCount(gObj[aIndex].PartyNumber);
						gObj[usernumber].PartyNumber = -1;
						gObj[usernumber].PartyTargetUser = -1;
						gParty.UpdatePKPartyPanalty(pnumber);
						if (!lpMsg->Number)
							gParty.ChangeLeader(pnumber);
						this->GCPartyDelUserSend(usernumber);
						this->CGPartyListAll(pnumber);
					}
					else
					{
						if (!ITL_MAP_RANGE(gObj[usernumber].MapNumber) || lpMsg->Number || count <= 2)
						{
							if (ITL_MAP_RANGE(gObj[usernumber].MapNumber) && count <= 2)
							{
								gParty.Delete(pnumber, lpMsg->Number);
								count = gParty.GetCount(gObj[aIndex].PartyNumber);
								gObj[usernumber].PartyNumber = -1;
								gObj[usernumber].PartyTargetUser = -1;
								gParty.UpdatePKPartyPanalty(pnumber);
								if (!lpMsg->Number)
									gParty.ChangeLeader(pnumber);
								this->GCPartyDelUserSend(usernumber);
								this->CGPartyListAll(pnumber);
							}
							else
							{
								for (int index = 0; index < 5; index++)
								{
									number = gParty.m_PartyS[pnumber].Number[index];

									if (number >= 0)
									{
										gParty.Delete(pnumber, index);
										gObj[number].PartyNumber = -1;
										gObj[number].PartyTargetUser = -1;
										if (gObj[number].m_PlayerData->m_bUsePartyMatching == true)
											this->GCPartyDelUserSendNoMessage(number);
										else
											this->GCPartyDelUserSend(number);
									}
								}

								gParty.Destroy(pnumber);
							}
						}
						else
						{
							gParty.Delete(pnumber, lpMsg->Number);
							count = gParty.GetCount(gObj[aIndex].PartyNumber);
							gObj[usernumber].PartyNumber = -1;
							gObj[usernumber].PartyTargetUser = -1;
							gParty.UpdatePKPartyPanalty(pnumber);
							gParty.ChangeLeader(pnumber);
							this->GCPartyDelUserSend(usernumber);
							this->CGPartyListAll(pnumber);
							if (!count)
								gParty.Destroy(pnumber);
						}
					}
				}
				else
				{
					gParty.Delete(number, lpMsg->Number);
					count = gParty.GetCount(gObj[aIndex].PartyNumber);
					gObj[usernumber].PartyNumber = -1;
					gObj[usernumber].PartyTargetUser = -1;
					gParty.UpdatePKPartyPanalty(pnumber);
					gParty.ChangeLeader(pnumber);
					this->GCPartyDelUserSend(usernumber);
					this->CGPartyListAll(pnumber);
					if (!count)
						gParty.Destroy(pnumber);
				}
			}
		}
	}
}

struct PMSG_PARTYDELUSERSEND
{
	PBMSG_HEAD h;	// C1:43
	BYTE btSendMessage;
};


void GameProtocol::GCPartyDelUserSend(int aIndex)
{
	PMSG_PARTYDELUSERSEND pMsg;

	pMsg.btSendMessage = FALSE;
	PHeadSetB((LPBYTE)&pMsg, 0x43, sizeof(pMsg));

	IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void GameProtocol::GCPartyDelUserSendNoMessage(int aIndex)
{
	PMSG_PARTYDELUSERSEND pMsg;

	pMsg.btSendMessage = TRUE;
	PHeadSetB((LPBYTE)&pMsg, 0x43, sizeof(pMsg));

	IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

struct PMSG_GUILDQUESTSEND
{
	PBMSG_HEAD h;	// C1:50
	BYTE NumberH;	// 3
	BYTE NumberL;	// 4
};

void GameProtocol::CGGuildRequestRecv(PMSG_GUILDJOINQ * lpMsg, int aIndex)
{
	PMSG_GUILDQUESTSEND pMsg;
	int number = MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);

	if ( number < g_ConfigRead.server.GetObjectStartUserIndex() || number > g_ConfigRead.server.GetObjectMax()-1 )
		return;

	if ( !PacketCheckTime(aIndex))
		return;

	if ( !gObjIsConnected(&gObj[number]))
	{
		GCResultSend(aIndex, 0x51, 0x03);
		return;
	}

	if (g_ConfigRead.server.GetServerType() == SERVER_CASTLE)
	{
		if (g_CastleSiegeSync.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
		{
			this->GCServerMsgStringSend(Lang.GetText(0,195), aIndex, 1);
			return;
		}
	}

	if ( gObj[number].Type != OBJ_USER )
	{
		g_Log.AddC(TColor::Red, "[HACKTOOL] : NPC-GuildRequest npc:%d ip:%s account:%s name:%s State:%d", gObj[number].Class, gObj[aIndex].m_PlayerData->Ip_addr, gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].Connected);
		this->GCSendDisableReconnect(aIndex);
		//IOCP.CloseClient(aIndex);
		return;
	}

	if(gObj[aIndex].m_PlayerData->m_bSecurityCheck == false)
	{
		GCResultSend(aIndex, 0x51, 0x06);
		return;
	}

	if ( gObj[aIndex].m_IfState.use > 0 )
	{
		GCResultSend(aIndex, 0x51, 0x06);
		return;
	}
	
	if ( gObj[number].m_IfState.use > 0 )
	{
		GCResultSend(aIndex, 0x51, 0x06);
		return;
	}

	if ( IT_MAP_RANGE(gObj[aIndex].MapNumber) )
	{
		this->GCServerMsgStringSend(Lang.GetText(0,292), aIndex, 1);
		return;
	}

	if ( g_NewPVP.IsDuel(gObj[aIndex]) || g_NewPVP.IsDuel(gObj[number]) )
	{
		this->GCServerMsgStringSend(Lang.GetText(0,317), aIndex, 1);
		return;
	}

	if ( g_NewPVP.IsObserver(gObj[aIndex]) )
	{
		this->GCServerMsgStringSend(Lang.GetText(0,318), aIndex, 1);
		return;
	}

	if ( gObj[aIndex].Level < 6 )
	{
		GCResultSend(aIndex, 0x51, 0x07);
		return;
	}

	if ( gObj[aIndex].m_PlayerData->GuildNumber > 0 )
	{
		GCResultSend(aIndex, 0x51, 0x05);
		return;
	}

	if ( (gObj[number].m_Option&1 ) != 1 )
	{
		GCResultSend(aIndex, 0x51, 0x00);
		return;
	}

	if ( gObj[number].m_PlayerData->GuildNumber > 0 )
	{
		if ( strcmp(gObj[number].m_PlayerData->lpGuild->Names[0], gObj[number].Name))
		{
			if ( gObj[number].m_PlayerData->GuildStatus != 64 )
			{
				GCResultSend(aIndex, 0x51, 0x04);
				return;
			}
		}

		if ( gObj[number].m_PlayerData->lpGuild->WarState )
		{
			GCResultSend(aIndex, 0x51, 0x06);
			return;
		}

		int MaxGuildMember;

		if ( gObj[number].Class == CLASS_DARKLORD )
			MaxGuildMember = (gObj[number].Level+gObj[number].m_PlayerData->MasterLevel) / 10 + gObj[number].Leadership / 10;
		else
			MaxGuildMember = (gObj[number].Level+gObj[number].m_PlayerData->MasterLevel) / 10;

		if (MaxGuildMember > g_ConfigRead.data.common.MaxGuildMember)
			MaxGuildMember = g_ConfigRead.data.common.MaxGuildMember;

		if ( gObj[number].m_PlayerData->lpGuild->TotalCount >= MaxGuildMember )
		{
			GCResultSend(aIndex, 0x51, 0x02);
			return;
		}

		int OwnInfluence = g_GensSystem.GetGensInfluence(&gObj[aIndex]);
		int TarInfluence = g_GensSystem.GetGensInfluence(&gObj[number]);

		int iArcaBattleState = g_ArcaBattle.GetState();

		if (iArcaBattleState > 2 && iArcaBattleState < 9)
		{
			this->GCResultSend(aIndex, 0x51, 0xA4);
			return;
		}

		if(OwnInfluence <= NONE_INFLUENCE)
		{
			g_Log.Add("[GensSystem] [%s][%s] Guild Joining Failed to Not Join Gens", gObj[aIndex].AccountID, gObj[aIndex].Name);
			GCResultSend(aIndex, 0x51, 0xA3);
			return;
		}

		if(TarInfluence <= NONE_INFLUENCE)
		{
			g_Log.Add("[GensSystem] [%s][%s] Guild Joining Failed to Not Join Gens GuildMaster", gObj[aIndex].AccountID, gObj[aIndex].Name);
			GCResultSend(aIndex, 0x51, 0xA1);
			return;
		}

		if(OwnInfluence != TarInfluence && g_GensSystem.CanGensJoinGuildWhileOppositeGens() == FALSE)
		{
			g_Log.Add("[GensSystem] [%s][%s] [%s] Guild Joining Failed to Gens be Different : [%s][%s] [%s]", gObj[aIndex].AccountID, gObj[aIndex].Name,
				g_GensSystem.GetGensInfluenceName(&gObj[aIndex]), gObj[number].AccountID, gObj[number].Name, g_GensSystem.GetGensInfluenceName(&gObj[number]));
			GCResultSend(aIndex, 0x51, 0xA2);
			return;
		}
	}
	else
	{
		GCResultSend(aIndex, 0x51, 0x04);
		return;
	}

	gObj[aIndex].m_IfState.use = 1;
	gObj[aIndex].m_IfState.type = 4;
	gObj[aIndex].m_IfState.state = 0;

	gObj[number].m_IfState.use = 1;
	gObj[number].m_IfState.type = 4;
	gObj[number].m_IfState.state = 0;

	gObj[aIndex].TargetNumber = number;
	gObj[number].TargetNumber = aIndex;
	gObj[aIndex].m_InterfaceTime = GetTickCount();
	gObj[number].m_InterfaceTime = GetTickCount();

	PHeadSetB((LPBYTE)&pMsg, 0x50, sizeof(pMsg));
	pMsg.NumberH = SET_NUMBERH(aIndex);
	pMsg.NumberL = SET_NUMBERL(aIndex);

	IOCP.DataSend(number, (LPBYTE)&pMsg, pMsg.h.size);

	g_Log.Add("[%s][%s] Guild Request - Join : %s",
		gObj[aIndex].AccountID, gObj[aIndex].Name,
		gObj[number].m_PlayerData->lpGuild->Name);
}



void GameProtocol::CGGuildRequestResultRecv(PMSG_GUILDQRESULT * lpMsg, int aIndex) //GS-CS Decompiled 100%
{
	int number;
	BYTE result=0;
	int pnumber=-1;

	if ( !PacketCheckTime(aIndex))
		return;

	if(g_CastleSiegeSync.GetCastleState() == 7) //Good
	{
		MsgOutput(aIndex,(Lang.GetText(0,195)));
		return;
	}

	number = MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);

	if ( number < 0 || number > g_ConfigRead.server.GetObjectMax()-1)
		return;

	if ( !gObjIsConnected(&gObj[number]))
	{
		GCResultSend(aIndex, 0x51, 0x02);
	}
	else
	{
		result = 1;
	}

	if ( lpMsg->Result == 0 )
	{
		result = 0;
		GCResultSend(number, 0x51, 0x00);
	}

	if ( result == 1 )
	{
		if ( gObj[aIndex].m_PlayerData->GuildNumber > 0 )
		{
			GDGuildMemberAdd(number, gObj[aIndex].m_PlayerData->lpGuild->Name, gObj[number].Name);
			GDReqCancelJoinGuildMatching(number, gObj[aIndex].Name, 1);
		}
	}

	if ( gObj[aIndex].m_IfState.use && gObj[aIndex].m_IfState.type == 4)
		gObj[aIndex].m_IfState.use = 0;
		
	if ( gObj[number].m_IfState.use && gObj[number].m_IfState.type == 4)
		gObj[number].m_IfState.use = 0;
}

struct PMSG_GUILDLIST
{
	char Name[10];	// 0
	BYTE Number;	// A
	BYTE ConnectAServer;	// B
	BYTE btGuildStatus;	// C
};

struct PMSG_GUILDLISTCOUNT_EX700
{
	PWMSG_HEAD h;	// C2:52
	BYTE Result;	// 4
	BYTE Count;	// 5
	int TotalScore;	// 8
	BYTE Score;	// C
	char szRivalGuild[5][8];	// D
};

struct PMSG_GUILDLISTCOUNT_S6
{
	PWMSG_HEAD h;	// C2:52
	BYTE Result;	// 4
	BYTE Count;	// 5
	int TotalScore;	// 8
	BYTE Score;	// C
	char szRivalGuild[8];
};

void GameProtocol::CGGuildListAll(int pnumber)
{
	if ( gObj[pnumber].Type != OBJ_USER )
	{
		return;
	}

	if ( gObj[pnumber].m_PlayerData->lpGuild == NULL )
		return;

	PMSG_GUILDLISTCOUNT_EX700 pCount;
	PMSG_GUILDLIST pList;
	char sendbuf[1500];
	int lOfs = 0;
	int number;
	int n;
	int guildmaster = 0;
	
	PHeadSetW((LPBYTE)&pCount, 0x52, 0);
	pCount.Result = 0;
	pCount.Count = 0;
	lOfs += sizeof(pCount);
	pCount.h.sizeH = SET_NUMBERH(lOfs);
	pCount.h.sizeL = SET_NUMBERL(lOfs);

	if ( gObj[pnumber].m_PlayerData->GuildNumber < 1 )
	{
		IOCP.DataSend(pnumber, (LPBYTE)&pCount, lOfs);
		return;
	}

	pCount.Count = gObj[pnumber].m_PlayerData->lpGuild->Count;
	pCount.TotalScore = gObj[pnumber].m_PlayerData->lpGuild->TotalScore;
	pCount.Score = gObj[pnumber].m_PlayerData->lpGuild->PlayScore;
	memset(pCount.szRivalGuild, 0, sizeof(pCount.szRivalGuild));

	if ( gObj[pnumber].m_PlayerData->lpGuild->iGuildRival )
	{
		int iGuildCount = 0;
		int iGuildList[MAX_UNION_GUILD] ={0};

		if ( UnionManager.GetGuildUnionMemberList(gObj[pnumber].m_PlayerData->lpGuild->iGuildRival, iGuildCount, iGuildList) == TRUE )
		{
			for ( int i=0;i<iGuildCount;i++)
			{
				_GUILD_INFO_STRUCT *lpGuildInfo = Guild.SearchGuild_Number(iGuildList[i]);
			
				if ( !lpGuildInfo )
					continue;

				memcpy(pCount.szRivalGuild[i], lpGuildInfo->Name, MAX_GUILD_LEN);
			}
		}

		else
		{
			memcpy(pCount.szRivalGuild[0], gObj[pnumber].m_PlayerData->lpGuild->szGuildRivalName, MAX_GUILD_LEN);
		}
	}

	pCount.Result = 1;
	guildmaster = 1;

	if ( pCount.Count == 0 )
		return;

	pCount.Count = 0;
	
	for ( n=0;n<MAX_USER_GUILD;n++)
	{
		number = gObj[pnumber].m_PlayerData->lpGuild->Use[n];

		if ( number > 0 )
		{
			memset(&pList, 0, sizeof(pList));
			memcpy(pList.Name, gObj[pnumber].m_PlayerData->lpGuild->Names[n], sizeof(pList.Name));
			pList.Number = gObj[pnumber].m_PlayerData->lpGuild->pServer[n];
			pList.ConnectAServer = (short)gObj[pnumber].m_PlayerData->lpGuild->pServer[n] & 0x7F;

			if ( gObj[pnumber].m_PlayerData->lpGuild->pServer[n] >= 0 )
				pList.ConnectAServer |= 0x80;

			pList.btGuildStatus = gObj[pnumber].m_PlayerData->lpGuild->GuildStatus[n];
			memcpy(&sendbuf[lOfs], &pList, sizeof(pList));
			lOfs+= sizeof(pList);
			pCount.Count++;
		}
	}
	
	pCount.h.sizeH = SET_NUMBERH(lOfs);
	pCount.h.sizeL = SET_NUMBERL(lOfs);
	memcpy(sendbuf, (LPBYTE)&pCount, sizeof(pCount));
	
	IOCP.DataSend(pnumber, (LPBYTE)sendbuf, lOfs);
}

struct PMSG_GUILDDELUSER_RESULT
{
	PBMSG_HEAD h;	// C1:53
	BYTE Result;	// 3
};

void GameProtocol::CGGuildDelUser(PMSG_GUILDDELUSER * lpMsg, int aIndex)
{
	if ( !PacketCheckTime(aIndex))
		return;

	if ( aIndex < g_ConfigRead.server.GetObjectStartUserIndex() || aIndex > g_ConfigRead.server.GetObjectMax()-1 )
		return;

	if (g_ConfigRead.server.GetServerType() == SERVER_CASTLE)
	{
		if (g_CastleSiegeSync.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE) //Good
		{
			MsgOutput(aIndex, Lang.GetText(0,196));
			return;
		}
	}

	if ( gObj[aIndex].m_PlayerData->GuildNumber < 1 )
		return;

	char memberid[11];
	char guildname[11];
	char joomin[21];
	memset(joomin, 0, sizeof(joomin));
	PMSG_GUILDDELUSER_RESULT pMsg;
	
	PHeadSetB((LPBYTE)&pMsg, 0x53, sizeof(pMsg));
	pMsg.Result = 3;

	if (!g_ConfigRead.data.common.GuildDestroy)
	{
		IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	int iArcaBattleState = g_ArcaBattle.GetState();

	if ( iArcaBattleState > 2 && iArcaBattleState < 9 )
	{
		pMsg.Result = 6;

		IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	if(gObj[aIndex].m_PlayerData->m_bSecurityCheck == false)
	{
		IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	memset(memberid, 0, sizeof(memberid));
	memset(guildname, 0, sizeof(guildname));
	memcpy(memberid, lpMsg->Name, MAX_ACCOUNT_LEN);
	strcpy(guildname, gObj[aIndex].m_PlayerData->lpGuild->Name);

	if ( !strcmp(gObj[aIndex].m_PlayerData->lpGuild->Names[0], gObj[aIndex].Name ))
	{
		memset(joomin, 0, sizeof(joomin));
		memcpy(joomin, lpMsg->Password, sizeof(lpMsg->Password));

		if ( gObjPasswordCheck(aIndex, joomin) == FALSE )
		{
			pMsg.Result = 0;
			IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
			return;
		}

		if ( !strcmp(memberid, gObj[aIndex].Name ))
		{
			if ( g_ConfigRead.server.GetServerType() == SERVER_CASTLE && g_ConfigRead.data.common.CastleGuildDestroyLimit == TRUE )
			{
				if ( strcmp(gObj[aIndex].m_PlayerData->lpGuild->Name, g_CastleSiegeSync.GetCastleOwnerGuild()) == 0 )
				{
					MsgOutput(aIndex, Lang.GetText(0,194));
					return;
				}
			}

			gObjGuildWarMasterClose(&gObj[aIndex]);
			GDGuildDestroySend(aIndex, gObj[aIndex].m_PlayerData->lpGuild->Name, gObj[aIndex].Name);
			g_ArcaBattle.ReqRemoveRewardGuildBuff(gObj[aIndex].m_PlayerData->lpGuild->Name);
			g_ArcaBattle.GDReqMarkRegDel(aIndex, gObj[aIndex].m_PlayerData->GuildNumber);
		}
		else
		{
			if ( Guild.GetGuildMemberStatus(gObj[aIndex].m_PlayerData->lpGuild->Name, memberid) == -1 )
			{
				pMsg.Result = 0;
				IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
				return;
			}

			g_ArcaBattle.ReqGuildMemberDelBuffDel(memberid, guildname);
			GDGuildMemberDel(aIndex, guildname, memberid);
		}

		g_Log.Add("[%s][%s] Guild Request - Dismiss All : %s",
				gObj[aIndex].AccountID,gObj[aIndex].Name, guildname);
	}
	else
	{
		if ( strcmp(memberid, gObj[aIndex].Name) )
		{
			pMsg.Result = 0;
			IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
			return;
		}

		memset(joomin, 0, sizeof(joomin));
		memcpy(joomin, lpMsg->Password, sizeof(lpMsg->Password));

		if ( gObjPasswordCheck(aIndex, joomin) == FALSE )
		{
			MsgOutput(aIndex, Lang.GetText(0,525));
		}

		else
		{
			g_ArcaBattle.ReqGuildMemberDelBuffDel( &gObj[aIndex], guildname);
			GDGuildMemberDel(aIndex, guildname, memberid);
		}

		g_Log.Add("[%s][%s] Guild Request - Leave : %s",
			gObj[aIndex].AccountID, gObj[aIndex].Name, guildname);
	}
}



void GameProtocol::GCGuildDelUserResult(int aIndex, BYTE Result)
{
	PMSG_GUILDDELUSER_RESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x53, sizeof(pMsg));
	pMsg.Result = Result;

	IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}



struct PMSG_GUILDMASTERQUESTION
{
	PBMSG_HEAD h;	// C1:54
};

void GameProtocol::GCGuildMasterQuestionSend(int aIndex)
{
	if ( gObj[aIndex].m_IfState.use > 0 )
	{
		return;
	}

	PMSG_GUILDMASTERQUESTION pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x54, sizeof(pMsg));
	gObj[aIndex].m_IfState.use = 1;
	gObj[aIndex].m_IfState.type = 5;
	gObj[aIndex].m_IfState.state = 0;

	IOCP.DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
}



void GameProtocol::CGGuildMasterAnswerRecv(PMSG_GUILDMASTERANSWER * lpMsg, int aIndex)
{
	if ( lpMsg->Result == 1 )
	{
		if ( !gObjGuildMasterCapacityTest(&gObj[aIndex]))
		{
			if ( gObj[aIndex].m_IfState.use && gObj[aIndex].m_IfState.type == 5 )
			{
				gObj[aIndex].m_IfState.use = 0;
			}

			return;
		}

		GCGuildMasterManagerRun(aIndex);
		return;
	}

	if ( gObj[aIndex].m_IfState.use && gObj[aIndex].m_IfState.type == 5 )
	{
		gObj[aIndex].m_IfState.use = 0;
	}
}


struct PMSG_GUILDMASTERMANAGER_RUN
{
	PBMSG_HEAD h;	// C1:55
};

void GameProtocol::GCGuildMasterManagerRun(int aIndex)
{
	PMSG_GUILDMASTERMANAGER_RUN pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x55, sizeof(pMsg));

	IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}



void GameProtocol::CGGuildMasterInfoSave(int aIndex, PMSG_GUILDINFOSAVE * lpMsg)
{
	char GuildName[MAX_GUILD_LEN+1];
	GuildName[MAX_GUILD_LEN] ='\0';
	memcpy(GuildName, lpMsg->GuildName, MAX_GUILD_LEN);
	int len = strlen(GuildName);

	if ( len <= 2 || len > MAX_GUILD_LEN)
	{
		PMSG_GUILDCREATED_RESULT pMsg;

		PHeadSetB((LPBYTE)&pMsg, 0x56, sizeof(pMsg));
		pMsg.Result = 2;
		
		IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

		if ( gObj[aIndex].m_IfState.use && gObj[aIndex].m_IfState.type == 5 )
		{
			gObj[aIndex].m_IfState.use = 0;
		}

		return;
	}

	if(!g_prohibitedSymbols.Validate(GuildName,len,TYPE_GUILDNAME))
	{
		MsgOutput(aIndex,Lang.GetText(0,546));
		return;
	}

	GDReqCancelJoinGuildMatching(aIndex, gObj[aIndex].Name, 1);
	GDGuildCreateSend(aIndex, GuildName, gObj[aIndex].Name, lpMsg->Mark, lpMsg->btGuildType);

	if ( gObj[aIndex].m_IfState.use && gObj[aIndex].m_IfState.type == 5 )
	{
		gObj[aIndex].m_IfState.use = 0;
	}
}



void GameProtocol::CGGuildMasterCreateCancel(int aIndex)
{
	if ( gObj[aIndex].Type != OBJ_USER )
		return;

	if ( gObj[aIndex].m_PlayerData->GuildNumber > 0 )
		return;

	if ( gObj[aIndex].m_IfState.use && gObj[aIndex].m_IfState.type == 5 )
	{
		gObj[aIndex].m_IfState.use = 0;
	}
}


void GameProtocol::GCGuildViewportNowPaint(int aIndex, char* guildname, BYTE* mark, BOOL isGuildMaster)
{
	_GUILD_INFO_STRUCT * lpGuild = Guild.SearchGuild(guildname);

	if ( lpGuild == NULL )
		return;

	LPOBJ lpObj = &gObj[aIndex];
	BYTE _GuildInfoBuf[256] = {0};
	int _GuildInfoOfs = 5;

	if ( lpObj->m_PlayerData->lpGuild == NULL )
		return;

	PMSG_SIMPLE_GUILDVIEWPORT pMsg;
	pMsg.GuildNumber = lpObj->m_PlayerData->lpGuild->Number;
	pMsg.NumberH = SET_NUMBERH(aIndex)&0x7F;
	pMsg.NumberL = SET_NUMBERL(aIndex);

	if ( isGuildMaster != FALSE )
		pMsg.NumberH |= 0x80;

	if ( isGuildMaster != FALSE )
		pMsg.btGuildStatus = 0x80;
	else
		pMsg.btGuildStatus = lpObj->m_PlayerData->GuildStatus;

	pMsg.btGuildType = lpObj->m_PlayerData->lpGuild->btGuildType;
	pMsg.btGuildRelationShip = 0;
	lpObj->m_PlayerData->iGuildUnionTimeStamp = 0;
	memcpy(&_GuildInfoBuf[_GuildInfoOfs], &pMsg, sizeof(pMsg));
	_GuildInfoOfs += sizeof(pMsg);

	PMSG_SIMPLE_GUILDVIEWPORT_COUNT pGVCount;

	pGVCount.h.c = 0xC2;
	pGVCount.h.headcode = 0x65;
	pGVCount.h.sizeH = SET_NUMBERH(_GuildInfoOfs);
	pGVCount.h.sizeL = SET_NUMBERL(_GuildInfoOfs);
	pGVCount.Count = 1;
	memcpy(_GuildInfoBuf, &pGVCount, sizeof(pGVCount));

	if ( !CC_MAP_RANGE(gObj[aIndex].MapNumber) && gObj[aIndex].MapNumber != MAP_INDEX_CHAOSCASTLE_SURVIVAL )
		IOCP.DataSend(aIndex, _GuildInfoBuf, _GuildInfoOfs);

	MsgSendV2(lpObj, _GuildInfoBuf, _GuildInfoOfs);
}



struct PMSG_GUILDDEL_VIEWPORT_NOW
{
	PBMSG_HEAD h;	// C1:5D
	BYTE NumberH;	// 3
	BYTE NumberL;	// 4
};



void GameProtocol::GCGuildViewportDelNow(int aIndex, BOOL isGuildMaster)
{
	PMSG_GUILDDEL_VIEWPORT_NOW pMsg;

	LPOBJ lpObj = &gObj[aIndex];

	PHeadSetB((LPBYTE)&pMsg, 0x5D, sizeof(pMsg));
	pMsg.NumberH = SET_NUMBERH(aIndex) & 0x7F;
	pMsg.NumberL = SET_NUMBERL(aIndex);

	if ( isGuildMaster != FALSE )
	{
		pMsg.NumberH |= 0x80;
	}

	IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
	MsgSendV2(lpObj, (LPBYTE)&pMsg, pMsg.h.size);
}



void GameProtocol::GCManagerGuildWarEnd(char * GuildName)
{
	_GUILD_INFO_STRUCT * lpNode = Guild.SearchGuild(GuildName);

	if ( lpNode == NULL )
	{
		return;
	}

	int n=0;
	int warmaster = -1;

	while ( true )
	{
		if ( gObj[n].Type == OBJ_USER )
		{
			if ( gObj[n].Connected > PLAYER_LOGGED )
			{
				if ( gObj[n].Name[0] == lpNode->Names[0][0] )
				{
					if ( strcmp(gObj[n].Name, lpNode->Names[0]) == 0 )
					{
						warmaster = n;
						break;
					}
				}
			}
		}

		if ( n < g_ConfigRead.server.GetObjectMax()-1 )
		{
			n++;
		}
		else
		{
			break;
		}
	}

	if ( warmaster >= 1 )
	{
		BYTE Result1 = 0;
		BYTE Result2 = 0;

		if ( lpNode != NULL )
		{
			if ( lpNode->lpTargetGuildNode != NULL )
			{
				if ( lpNode->PlayScore == 0 && lpNode->lpTargetGuildNode->PlayScore == 0 )
				{
					Result1 = 6;
					Result2 = 6;
				}
				else
				{
					Result1 = 0;
					Result2 = 1;
				}
			}
		}

		if ( lpNode->WarType == 1 )
		{
			gBattleGroundEnable(lpNode->BattleGroundIndex, FALSE );
			gObjGuildWarEndSend(lpNode, lpNode->lpTargetGuildNode, Result1, Result2);
			gObjGuildWarEnd(lpNode, lpNode->lpTargetGuildNode);

			cManager.BattleInfoSend(::GetBattleTeamName(0, 0), -1, ::GetBattleTeamName(0, 1), -1);
		}
		else
		{
			gObjGuildWarEndSend(lpNode, lpNode->lpTargetGuildNode, Result1, Result2);
			gObjGuildWarEnd(lpNode, lpNode->lpTargetGuildNode);
		}
	}
}



void GameProtocol::GCManagerGuildWarSet(LPSTR GuildName1, LPSTR GuildName2, int type)
{
	_GUILD_INFO_STRUCT * lpNode = Guild.SearchGuild(GuildName1);

	if ( lpNode == NULL )
	{
		return;
	}

	int n=0;
	int warmaster = -1;

	while ( true )
	{
		if ( gObj[n].Type == OBJ_USER )
		{
			if ( gObj[n].Connected > PLAYER_LOGGED )
			{
				if ( gObj[n].Name[0] == lpNode->Names[0][0] )
				{
					if ( strcmp(gObj[n].Name, lpNode->Names[0]) == 0 )
					{
						warmaster = n;
						break;
					}
				}
			}
		}

		if ( n < g_ConfigRead.server.GetObjectMax()-1 )
		{
			n++;
		}
		else
		{
			break;
		}
	}

	if ( warmaster >= 1 )
	{
		this->GCGuildWarRequestResult(GuildName2, warmaster, type);
	}
}


struct PMSG_GUILDWARREQUEST_RESULT
{
	PBMSG_HEAD h;	// C1:60
	BYTE Result;	// 3
};

void GameProtocol::GCGuildWarRequestResult(LPSTR GuildName, int aIndex, int type)
{
	if (g_ConfigRead.server.GetServerType() == SERVER_CASTLE)
	{
		return;
	}

	if ( gObj[aIndex].m_IfState.use > 0 && gObj[aIndex].m_IfState.type != 12)
	{
		g_Log.AddC(TColor::Red, "[ANTI-HACK][Trade & War] (%s)(%s)",
			gObj[aIndex].AccountID, gObj[aIndex].Name);
		return;
	}

	PMSG_GUILDWARREQUEST_RESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x60, sizeof(pMsg));
	pMsg.Result = 3;

	if ( gObj[aIndex].m_PlayerData->GuildNumber < 1)
	{
		IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	_GUILD_INFO_STRUCT * lpMyGuild = gObj[aIndex].m_PlayerData->lpGuild;

	if ( !lpMyGuild )
	{
		IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	if ( lpMyGuild->WarState == 1 || lpMyGuild->WarDeclareState == 1 )
	{
		pMsg.Result = 4;

		//Error on declaring war : %d %d %s
		g_Log.Add("Error on declaring war : %d %d %s", lpMyGuild->WarState,
			lpMyGuild->WarDeclareState, lpMyGuild->Name);
		IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

		return;
	}

	if ( g_ConfigRead.pk.bPkPenaltyDisable == FALSE && gObj[aIndex].m_PK_Level >= 6)
	{
		pMsg.Result = 4;

		g_Log.Add("Error on declaring war: GuildMaster Is Murderer 0x04 %s %s",
			lpMyGuild->Names[0], gObj[aIndex].Name);

		IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

		return;
	}

	if ( strcmp(lpMyGuild->Names[0], gObj[aIndex].Name ) )
	{
		pMsg.Result = 5;

		// Error on declaring war : 0x05 %s %s
		g_Log.Add("Error on declaring war: 0x05 %s %s",
			lpMyGuild->Names[0], gObj[aIndex].Name);
		IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

		return;
	}

	char _guildname[MAX_GUILD_LEN+1];
	memset(_guildname, 0, sizeof(_guildname));
	memcpy(_guildname, GuildName, MAX_GUILD_LEN);

	if ( !strncmp(lpMyGuild->Name, GuildName, MAX_GUILD_LEN))
		return;

	_GUILD_INFO_STRUCT * lpNode = Guild.SearchGuild(_guildname);

	if ( lpNode )
	{
		if ( lpNode->WarState == 1 || lpNode->WarDeclareState == 1 )
		{
			pMsg.Result = 4;

			//Error on declaring war : %d %d %s
			g_Log.Add("Error on declaring war : %d %d %s", lpNode->WarState,
				lpNode->WarDeclareState, lpNode->Name);
			IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

			return;
		}

		if ( lpMyGuild->iGuildUnion != 0 && lpMyGuild->iGuildUnion == lpNode->iGuildUnion )
		{
			g_Log.Add("[U.System][Guildwar] Cannot declare guildwar between Same Union (%s / %d) <-> (%s / %d)",
				lpMyGuild->Name, lpMyGuild->iGuildUnion, lpNode->Name, lpMyGuild->iGuildUnion);

			return;
		}

		int n= g_ConfigRead.server.GetObjectStartUserIndex();	// #warning Change this 0 to ObjectStartUserIndex
		int warmaster=-1;

		while ( true )
		{
			if ( gObj[n].Type == OBJ_USER )
			{
				if ( gObj[n].Connected > PLAYER_LOGGED )
				{
					if ( gObj[n].Name[0] == lpNode->Names[0][0] )
					{
						if ( !strcmp(gObj[n].Name, lpNode->Names[0]))
						{
							if ( g_ConfigRead.pk.bPkPenaltyDisable == FALSE && gObj[n].m_PK_Level >= 6)
							{
								pMsg.Result = 4;

								g_Log.Add("Error on declaring war : Target GuildMaster Is Murderer 0x04 %s %s",
									lpMyGuild->Names[0], gObj[aIndex].Name);

								IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

								return;
							}

							warmaster = n;
							break;
						}
					}
				}
			}

			if ( n < g_ConfigRead.server.GetObjectMax()-1 )
				n++;
			else
				break;
		}
		
		if ( warmaster >= 1 )
		{
			if ( CC_MAP_RANGE(gObj[aIndex].MapNumber) || CC_MAP_RANGE(gObj[warmaster].MapNumber) )
			{
				GCServerMsgStringSend(Lang.GetText(0,117), aIndex, 1);
				return;
			}

			if (gObj[aIndex].MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL || gObj[warmaster].MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL)
			{
				GCServerMsgStringSend(Lang.GetText(0,575), aIndex, 1);
				return;
			}

			if ( (gObj[warmaster].m_Option&1) != 1 )
			{
				pMsg.Result = 4;
				IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

				return;
			}

			pMsg.Result = 1;
			IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
			GCGuildWarRequestSend(lpMyGuild->Name, warmaster, type);
			lpMyGuild->WarDeclareState = 1;
			lpNode->WarDeclareState = 1;
			lpMyGuild->WarType = type;
			lpNode->WarType = type;

			// Declared guild war [%s][%s] sGuild:(%s) TargetGuild:(%s)
			g_Log.Add("Declared guild war [%s][%s] sGuild:[%s] TargetGuild:[%s]", gObj[aIndex].AccountID, gObj[aIndex].Name,
				lpMyGuild->Name, lpNode->Name);

			strcpy(lpMyGuild->TargetGuildName, lpNode->Name);
			strcpy(lpNode->TargetGuildName, lpMyGuild->Name);
			lpMyGuild->lpTargetGuildNode = lpNode;
			lpNode->lpTargetGuildNode = lpMyGuild;
		}
		else
		{
			pMsg.Result = 2;
			IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

			//return;
		}
	}
	else
	{
		pMsg.Result = 0;
		IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

		return;
	}
}


struct PMSG_GUILDWARSEND
{
	PBMSG_HEAD h;	// C1:61
	char GuildName[8];	// 3
	BYTE Type;	// B
};

void GameProtocol::GCGuildWarRequestSend(LPSTR GuildName, int aIndex, int type) //GS-CS OK
{
	if (g_ConfigRead.server.GetServerType() != SERVER_CASTLE)
	{
		PMSG_GUILDWARSEND pMsg;

		PHeadSetB((LPBYTE)&pMsg, 0x61, sizeof(pMsg));
		pMsg.Type = type;
		memcpy(pMsg.GuildName, GuildName, MAX_GUILD_LEN);

		IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

		// (%s) guild declared guild war
		g_Log.Add("%s guild declared guild war", GuildName);
	}
}


struct PMSG_GUILDWAR_DECLARE
{
	PBMSG_HEAD h;	// C1:62
	char GuildName[8];	// 3
	BYTE Type;	// B
	BYTE TeamCode;	// C
};


void GameProtocol::GCGuildWarRequestSendRecv(PMSG_GUILDWARSEND_RESULT * lpMsg, int aIndex)
{
	if (g_ConfigRead.server.GetServerType() == SERVER_CASTLE)
	{
		return;
	}

	if (gObj[aIndex].Connected != 3)
		return;

	PMSG_GUILDWAR_DECLARE pMsg;
	int count=0;
	int g_call=0;
	
	PHeadSetB((LPBYTE)&pMsg, 0x62, sizeof(pMsg));
	pMsg.Type = 0;
	
	_GUILD_INFO_STRUCT * lpMyNode = gObj[aIndex].m_PlayerData->lpGuild;

	if ( !lpMyNode )
	{
		PMSG_GUILDWARREQUEST_RESULT pResult;

		pResult.h.c = 0xC1;
		pResult.h.headcode = 0x60;
		pResult.h.size = sizeof(pResult);	// #error Change fro pResult
		pResult.Result = 0;

		IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( lpMyNode->lpTargetGuildNode && lpMsg->Result )
	{
		int iTarGetIndex = lpMyNode->lpTargetGuildNode->Index[0];

		if(!ObjectMaxRange(iTarGetIndex))
		{
			g_Log.Add("Object index out of range");
			return;
		}

		if ( BC_MAP_RANGE(gObj[iTarGetIndex].MapNumber) ||
			 CC_MAP_RANGE(gObj[iTarGetIndex].MapNumber) ||
			 DS_MAP_RANGE(gObj[iTarGetIndex].MapNumber) ||
			 DG_MAP_RANGE(gObj[iTarGetIndex].MapNumber) ||
			 IT_MAP_RANGE(gObj[iTarGetIndex].MapNumber) ||
			 IMPERIAL_MAP_RANGE(gObj[iTarGetIndex].MapNumber) )
		{
			PMSG_NOTICE pNotice;

			TNotice::MakeNoticeMsgEx(&pNotice, 1, Lang.GetText(0,117));

			IOCP.DataSend(aIndex, (LPBYTE)&pNotice, pNotice.h.size);

			lpMsg->Result = 0;
		}

		if (gObj[iTarGetIndex].MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL)
		{
			PMSG_NOTICE pNotice;

			TNotice::MakeNoticeMsgEx(&pNotice, 1, Lang.GetText(0,575));

			IOCP.DataSend(aIndex, (LPBYTE)&pNotice, pNotice.h.size);

			lpMsg->Result = 0;
		}
	}

	pMsg.Type = lpMyNode->WarType;

	if ( lpMsg->Result == 0 )
	{
		if ( lpMyNode->lpTargetGuildNode )
		{
			if ( lpMyNode->lpTargetGuildNode->WarDeclareState == 1 )
			{
				lpMyNode->lpTargetGuildNode->WarDeclareState = 0;
				lpMyNode->WarDeclareState = 0;

				PMSG_GUILDWARREQUEST_RESULT pResult;

				PHeadSetB((LPBYTE)&pResult, 0x60, sizeof(pResult));
				pResult.Result = 6;

				if ( lpMyNode->lpTargetGuildNode->Index[0] >= 0 )
				{
					IOCP.DataSend(lpMyNode->lpTargetGuildNode->Index[0], (LPBYTE)&pResult, pResult.h.size);
				}
			}
		}
	}
	else //if ( lpMsg->Result == 1 )
	{
		if ( lpMyNode->WarDeclareState == 1 )
		{
			if ( lpMyNode->lpTargetGuildNode )
			{
				if (lpMyNode->lpTargetGuildNode->WarDeclareState == 1 )
				{
					PMSG_TELEPORT pTeleportMsg;

					if ( lpMyNode->WarType == 1 )
					{
						lpMyNode->BattleGroundIndex = gCheckBlankBattleGround();

						switch ( lpMyNode->BattleGroundIndex )
						{
							case 0xFF:
								lpMyNode->WarDeclareState = 0;
								lpMyNode->WarState = 0;
								lpMyNode->lpTargetGuildNode->WarDeclareState = 0;
								lpMyNode->lpTargetGuildNode->WarState = 0;

								PMSG_GUILDWARREQUEST_RESULT pResult;

								PHeadSetB((LPBYTE)&pResult, 0x60, sizeof(pResult));
								pResult.Result = 4;

								IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
								return;
						}
	
						pTeleportMsg.MoveNumber = 51;
						lpMyNode->lpTargetGuildNode->BattleGroundIndex = lpMyNode->BattleGroundIndex;

						if ( gBSGround[0]->m_BallIndex >= 0 )
							gObjMonsterRegen(&gObj[gBSGround[0]->m_BallIndex]);

						BattleSoccerGoalEnd(0);
						lpMyNode->PlayScore = 0;
						lpMyNode->lpTargetGuildNode->PlayScore = 0;
						gBattleGroundEnable(lpMyNode->BattleGroundIndex, TRUE);
						gSetBattleTeamMaster(lpMyNode->BattleGroundIndex, 0, lpMyNode->Name, lpMyNode);
						gSetBattleTeamMaster(lpMyNode->BattleGroundIndex, 1, lpMyNode->lpTargetGuildNode->Name, lpMyNode->lpTargetGuildNode);
					}
					
					lpMyNode->WarDeclareState = 2;
					lpMyNode->WarState = 1;
					lpMyNode->lpTargetGuildNode->WarDeclareState = 2;
					lpMyNode->lpTargetGuildNode->WarState = 1;
					lpMyNode->PlayScore = 0;
					lpMyNode->lpTargetGuildNode->PlayScore = 0;
					lpMyNode->BattleTeamCode = 0;
					lpMyNode->lpTargetGuildNode->BattleTeamCode = 1;
					memset(pMsg.GuildName, 0, sizeof(pMsg.GuildName));
					memcpy(pMsg.GuildName, lpMyNode->lpTargetGuildNode->Name, sizeof(pMsg.GuildName));
					pMsg.TeamCode = lpMyNode->BattleTeamCode;
					count = 0;

					for ( int n=0;n<MAX_USER_GUILD;n++)
					{
						if ( lpMyNode->Use[n] )
						{
							if ( lpMyNode->Index[n] >= 0 )
							{
								g_call = 0;

								if ( n > 0 )
								{
									if ( lpMyNode->WarType == 1 )
									{
										if ( gObj[lpMyNode->Index[0]].PartyNumber  >= 0 )
										{
											if ( gObj[lpMyNode->Index[0]].PartyNumber == gObj[lpMyNode->Index[n]].PartyNumber )
											{
												g_call = 1;
											}
										}
									}
									else 
									{
										g_call = 1;
									}
								}
								else if ( n== 0 )
								{
									g_call = 1;
								}

								if ( g_call )
								{
									IOCP.DataSend(lpMyNode->Index[n], (LPBYTE)&pMsg, pMsg.h.size);

									// Declared guild war against (%s)
									g_Log.Add("Declared guild war against %s", lpMyNode->Name);
									GCGuildWarScore(lpMyNode->Index[n]);
									int x = 60;

									if ( lpMyNode->WarType == 1 )
									{
										gObj[lpMyNode->Index[n]].IsInBattleGround = true;
										pTeleportMsg.MoveNumber = 51;

										if ( g_ConfigRead.pk.bPkPenaltyDisable || gObj[lpMyNode->Index[n]].m_PK_Level < 6 )
										{
											gObjTeleport(lpMyNode->Index[n], 6, x++, 153);
											count++;
										}
									}
								}
							}
						}
					}

					memset(pMsg.GuildName, 0, sizeof(pMsg.GuildName));
					memcpy(pMsg.GuildName, lpMyNode->Name, sizeof(pMsg.GuildName));

					pMsg.TeamCode = lpMyNode->lpTargetGuildNode->BattleTeamCode;
					count = 0;

					for (int n=0;n<MAX_USER_GUILD;n++)
					{
						if ( lpMyNode->lpTargetGuildNode->Use[n] )
						{
							if ( lpMyNode->lpTargetGuildNode->Index[n] >= 0 )
							{
								g_call = 0;

								if ( n > 0 )
								{
									if ( lpMyNode->WarType == 1 )
									{
										if ( gObj[lpMyNode->lpTargetGuildNode->Index[0]].PartyNumber  >= 0 )
										{
											if ( gObj[lpMyNode->lpTargetGuildNode->Index[0]].PartyNumber == gObj[lpMyNode->lpTargetGuildNode->Index[n]].PartyNumber )
											{
												g_call = 1;
											}
										}
									}
									else 
									{
										g_call = 1;
									}
								}
								else if ( n== 0 )
								{
									g_call = 1;
								}

								if ( g_call )
								{
									IOCP.DataSend(lpMyNode->lpTargetGuildNode->Index[n], (LPBYTE)&pMsg, pMsg.h.size);

									// Declared guild war against (%s)
									g_Log.Add("Declared guild war against %s", lpMyNode->lpTargetGuildNode->Name);
									GCGuildWarScore(lpMyNode->lpTargetGuildNode->Index[n]);
									int x = 59;

									if ( lpMyNode->lpTargetGuildNode->WarType == 1 )
									{

										if ( g_ConfigRead.pk.bPkPenaltyDisable != 0 || gObj[lpMyNode->lpTargetGuildNode->Index[n]].m_PK_Level < 6 )
										{
											gObj[lpMyNode->lpTargetGuildNode->Index[n]].IsInBattleGround = true;
											gObjTeleport(lpMyNode->lpTargetGuildNode->Index[n], 6, x++, 164);
											count++;
										}
									}
								}
							}
						}
					}

					cManager.BattleInfoSend(GetBattleTeamName(0,0),
											GetBattleTeamScore(0,0),
											GetBattleTeamName(0, 1),
											GetBattleTeamScore(0,1));

					if ( lpMyNode->WarType == 1 )
					{
						gObjAddMsgSendDelay(&gObj[aIndex], 5, aIndex, 10000, 0);
						GCServerMsgStringSendGuild(lpMyNode->lpTargetGuildNode,Lang.GetText(0,62), 1);
						GCServerMsgStringSendGuild(lpMyNode,Lang.GetText(0,62), 1);
					}
				}
			}
		}
	}
}



void GameProtocol::GCGuildWarDeclare(int aIndex, LPSTR _guildname)
{
	if (g_ConfigRead.server.GetServerType() != SERVER_CASTLE)
	{
		PMSG_GUILDWAR_DECLARE pMsg;

		PHeadSetB((LPBYTE)&pMsg, 0x62, sizeof(pMsg));
		memcpy(pMsg.GuildName, _guildname, sizeof(pMsg.GuildName));

		IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
	}
}


struct PMSG_GUILDWAR_END
{
	PBMSG_HEAD h;	// C1:63
	BYTE Result;	// 3
	char GuildName[8];	// 4
};

void GameProtocol::GCGuildWarEnd(int aIndex, BYTE result, char* _guildname)
{
	if (g_ConfigRead.server.GetServerType() != SERVER_CASTLE)
	{
		PMSG_GUILDWAR_END pMsg;

		PHeadSetB((LPBYTE)&pMsg, 0x63, sizeof(pMsg));
		pMsg.Result = result;
		memcpy(pMsg.GuildName, _guildname, sizeof(pMsg.GuildName));

		IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
	}
}


struct PMSG_GUILDSCORE
{
	PBMSG_HEAD h;	// C1:64
	BYTE Score1;	// 3
	BYTE Score2;	// 4
	BYTE Type;	// 5
};


void GameProtocol::GCGuildWarScore(int aIndex)
{
	if (g_ConfigRead.server.GetServerType() != SERVER_CASTLE)
	{
		if (gObj[aIndex].Type != OBJ_USER)
			return;

		if (gObj[aIndex].m_PlayerData->GuildNumber < 1)
			return;

		PMSG_GUILDSCORE pMsg;

		PHeadSetB((LPBYTE)&pMsg, 0x64, sizeof(pMsg));
		pMsg.Score1 = 0;
		pMsg.Score2 = 0;
		pMsg.Type = 0;

		if (gObj[aIndex].m_PlayerData->lpGuild != NULL)
		{
			pMsg.Score1 = gObj[aIndex].m_PlayerData->lpGuild->PlayScore;
			g_Log.Add("Score %s %d", gObj[aIndex].Name, gObj[aIndex].m_PlayerData->lpGuild->PlayScore);
		}

		if (gObj[aIndex].m_PlayerData->lpGuild->lpTargetGuildNode != NULL)
		{
			pMsg.Score2 = gObj[aIndex].m_PlayerData->lpGuild->lpTargetGuildNode->PlayScore;
			g_Log.Add("Target Score %s %d", gObj[aIndex].Name, gObj[aIndex].m_PlayerData->lpGuild->lpTargetGuildNode->PlayScore);
		}

		IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
	}
}	



void GameProtocol::CGWarehouseMoneyInOut(int aIndex, PMSG_WAREHOUSEMONEYINOUT* lpMsg)
{
	int money = 0;

	if ( !gObjIsConnected(aIndex))
	{
		g_Log.AddC(TColor::Red,  "error-L3 [%s][%d]", __FILE__, __LINE__);
		GCWarehouseInventoryMoneySend(aIndex, 0, 0, 0);
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];
	money = lpMsg->Money;
	
	if ( lpObj->m_IfState.state == 0 )
	{
		GCWarehouseInventoryMoneySend(aIndex, 0, 0, 0);
		return;
	}

	if ( lpObj->m_IfState.use < 1 || lpObj->m_IfState.type != 6 )
	{
		GCWarehouseInventoryMoneySend(aIndex, 0, 0, 0);
		g_Log.Add("[%s][%s] error-L1 : used not Warehouse",
			gObj[aIndex].AccountID, gObj[aIndex].Name);

		return;
	}

	switch ( lpMsg->Type )
	{
		case 0x00:
			if ( money <= 0 || money > MAX_WAREHOUSE_ZEN )
			{
				GCWarehouseInventoryMoneySend(aIndex, 0, 0, 0);
				return;
			}

			if ( (lpObj->WarehouseMoney+money) > MAX_WAREHOUSE_ZEN )
			{
				GCWarehouseInventoryMoneySend(aIndex, 0, 0, 0);
				return;
			}

			if ( money > lpObj->m_PlayerData->Money )
			{
				GCWarehouseInventoryMoneySend(aIndex, 0, 0, 0);
				return;
			}

			lpObj->m_PlayerData->Money-=money;
			lpObj->WarehouseMoney+=money;
			break;
		case 0x01:
			{
				if ( bCanWarehouseLock == TRUE)
				{
					if ( lpObj->WarehouseLock == 1)
					{
						GCWarehouseInventoryMoneySend(aIndex, 0, 0, 0);
						GCServerMsgStringSend(Lang.GetText(0,155), lpObj->m_Index,1);
						return;
					}
				}

				if ( money <= 0 || money > MAX_WAREHOUSE_ZEN )
				{
					GCWarehouseInventoryMoneySend(aIndex, 0, 0, 0);
					return;
				}
				
				if ( money > lpObj->WarehouseMoney )
				{
					GCWarehouseInventoryMoneySend(aIndex, 0, 0, 0);
					return;
				}

				lpObj->WarehouseMoney -= money;
				lpObj->m_PlayerData->Money += money;
				int rZen = GetWarehouseUsedHowMuch(lpObj->Level, lpObj->WarehousePW);

				if ( (lpObj->m_PlayerData->Money-rZen) > 0 )
				{
					int oldmoney = lpObj->m_PlayerData->Money;
					lpObj->m_PlayerData->Money -= rZen;
					GCMoneySend(lpObj->m_Index, lpObj->m_PlayerData->Money);
					
					g_Log.Add("Get WareHouse Money(In Inventory) : %d - %d = %d",
						oldmoney, rZen, lpObj->m_PlayerData->Money);
				}
				else if ( (lpObj->WarehouseMoney-rZen) > 0 )
				{
					int oldmoney = lpObj->WarehouseMoney;
					lpObj->WarehouseMoney -= rZen;

					g_Log.Add("Get WareHouse Money(In WareHouse) : %d - %d = %d",
						oldmoney, rZen, lpObj->WarehouseMoney);

					GCWarehouseInventoryMoneySend(aIndex, 1, lpObj->m_PlayerData->Money, lpObj->WarehouseMoney);
				}
				else
				{
					lpObj->WarehouseMoney +=money;
					lpObj->m_PlayerData->Money -= money;
					GCWarehouseInventoryMoneySend(aIndex, 0, 0, 0);
					char msg[255];

					wsprintf(msg, Lang.GetText(0,154), rZen);
					GCServerMsgStringSend(msg, lpObj->m_Index, 1);
					return;
				}
			}
			break;

		default:
			GCWarehouseInventoryMoneySend(aIndex, 0, 0, 0);
			return;
	}

	lpObj->WarehouseCount++;
	GCWarehouseInventoryMoneySend(aIndex, 1, lpObj->m_PlayerData->Money, lpObj->WarehouseMoney);
}

void GameProtocol::GCWarehouseInventoryMoneySend(int aIndex, BYTE result, int money, int wmoney)
{
	PMSG_MONEY pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x81, sizeof(pMsg));
	pMsg.Result = result;
	pMsg.iMoney = money;
	pMsg.wMoney = wmoney;

	IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}



void GameProtocol::CGWarehouseUseEnd(int aIndex)
{
	if ( !gObjIsConnectedGP(aIndex))
	{
		g_Log.AddC(TColor::Red,  "error-L3 [%s][%d]", __FILE__, __LINE__);
		return;
	}

	
	LPOBJ lpObj = &gObj[aIndex];

	PMSG_DEFAULT pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x82, sizeof(pMsg));
	IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
	bool bCheck = false;
	Check_SameSerialItem(aIndex, 2, bCheck);

	if( lpObj->m_btOpenWarehouse == FALSE )
	{
		AntiHackLog->Output("[%s][%s] Attempt Multi-Ware dupe", lpObj->AccountID,lpObj->Name);
		return;
	}

	gObjItemTextSave(lpObj);
	GDSetWarehouseList(aIndex, TRUE);
	GDUserItemSave(lpObj);
	g_PentagramSystem.DBREQ_SetPentagramJewel(aIndex, 0);
	g_PentagramSystem.DBREQ_SetPentagramJewel(aIndex, 1);
}



struct PMSG_WAREHOUSESTATE
{
	PBMSG_HEAD h;	// C1:83
	BYTE State;	// 3
};


void GameProtocol::GCWarehouseStateSend(int aIndex, BYTE state)
{
	PMSG_WAREHOUSESTATE pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x83, sizeof(pMsg));
	pMsg.State = state;

	IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}



void GameProtocol::GCWarehouseRecivePassword(int aIndex, PMSG_WAREHOUSEPASSSEND* lpMsg)
{
	if ( bCanWarehouseLock == FALSE )
		return;

	int pw = lpMsg->Pass;

	switch ( lpMsg->Type )
	{
		case 0x00:
			if ( gObj[aIndex].WarehouseLock )
			{
				if ( gObj[aIndex].WarehousePW == pw )
				{
					gObj[aIndex].WarehouseLock = FALSE;
					GCWarehouseStateSend(aIndex, 0x0C);
				}
				else
				{
					GCWarehouseStateSend(aIndex, 0x0A);
				}
			}
			break;
		case 0x01:
			if ( !gObj[aIndex].WarehouseLock || gObj[aIndex].WarehouseLock == 0xFF )
			{
				char szJoomin[21];

				memset(szJoomin, 0, sizeof(szJoomin));
				memcpy(szJoomin, lpMsg->LastJoominNumber, sizeof(lpMsg->LastJoominNumber));

				if ( gObjPasswordCheck(aIndex, szJoomin) == FALSE )
				{
					GCWarehouseStateSend(aIndex, 0x0D);
					return;
				}

				gObj[aIndex].WarehousePW = pw;
				gObj[aIndex].WarehouseLock = FALSE;
				GCWarehouseStateSend(aIndex, 0x0C);
			}
			else
			{
				GCWarehouseStateSend(aIndex, 0x0B);
			}
			break;
		case 0x02:
			{
				char szJoomin[21];

				memset(szJoomin, 0, sizeof(szJoomin));
				memcpy(szJoomin, lpMsg->LastJoominNumber, sizeof(lpMsg->LastJoominNumber));

				pw = atoi(szJoomin);

				if ( gObjPasswordCheck(aIndex, szJoomin) == FALSE )
				{
					GCWarehouseStateSend(aIndex, 0x0D);
					return;
				}

				gObj[aIndex].WarehouseLock = FALSE;
				gObj[aIndex].WarehousePW = 0;
				GCWarehouseStateSend(aIndex, gObj[aIndex].WarehouseLock);
			}
			break;
	}
}	



void GameProtocol::GCUserChaosBoxSend(LPOBJ lpObj, int iChaosBoxType)
{
	if ( iChaosBoxType == 1  )
	{
		if ( lpObj->m_IfState.type != 13 )
		{
			return;
		}

		if ( lpObj->m_IfState.type == 13 && lpObj->m_IfState.state == 1 )
		{
			return;
		}
	}
	else 
	{
		if ( lpObj->m_IfState.type != 7 )
		{
			return;
		}

		if ( lpObj->m_IfState.type == 7 && lpObj->m_IfState.state == 1 )
		{
			return;
		}
	}

	PMSG_SHOPITEMCOUNT pMsg;
	BYTE SendByte[2048];
	int lOfs = sizeof(pMsg);
	PHeadSetW((LPBYTE)&pMsg, 0x31, 0);
	pMsg.count = 0;

	if ( iChaosBoxType == 1 )
	{
		pMsg.Type = 5;
	}
	else
	{
		pMsg.Type = 3;
	}

	for ( int n=0;n<CHAOS_BOX_SIZE;n++)
	{
		if ( lpObj->pChaosBox[n].IsItem() == TRUE )
		{
			SendByte[lOfs] = n;
			lOfs++;
			ItemByteConvert(&SendByte[lOfs], lpObj->pChaosBox[n] );
			lOfs += MAX_ITEM_INFO;
			pMsg.count ++;
		}
	}

	pMsg.h.sizeH = SET_NUMBERH(lOfs);
	pMsg.h.sizeL = SET_NUMBERL(lOfs);
	memcpy(SendByte, &pMsg, sizeof(pMsg));

	IOCP.DataSend(lpObj->m_Index, SendByte, lOfs);
}



void GameProtocol::CGChaosBoxItemMixButtonClick(PMSG_CHAOSMIX* aRecv, int aIndex)
{
	if ( ObjectMaxRange(aIndex) == false )
	{
		return;
	}

	if ( !gObjIsConnectedGP(aIndex))
	{
		g_Log.AddC(TColor::Red,  "error-L3 [%s][%d]", __FILE__, __LINE__);
		return;
	}

	if (g_ConfigRead.SeasonX == false)
	{
		aRecv->MixCount = 1; // Disable Mass Combine
	}

	LPOBJ lpObj = &gObj[aIndex];

	if ( lpObj->ChaosLock == TRUE )
	{
		g_Log.Add("[%s][%s] Already Used Chaos",
			lpObj->AccountID, lpObj->Name);

		GCServerMsgStringSend(Lang.GetText(0,11), lpObj->m_Index, 1);
		return;
	}

	if ( lpObj->m_PlayerData->m_bSecurityCheck == false )
	{
		g_Log.AddC(TColor::Red,  "[SECURITY LOCK] WARNING: (%s)(%s) ILLEGAL USE CHAOSBOX WITH SECURITY LOCK ON", lpObj->AccountID, lpObj->Name);
		return;
	}

	if ( lpObj->m_bPShopOpen == true )
	{
		g_Log.Add("[%s][%s] is Already Opening PShop, ChaosBox Failed",
			lpObj->AccountID, lpObj->Name);

		GCServerMsgStringSend(Lang.GetText(0,112), lpObj->m_Index, 1);
		return;
	}

	//season4 add-on start
	PMSG_CHAOSMIXRESULT pMsg;
	PHeadSetB((LPBYTE)&pMsg.h, 0x86, sizeof(PMSG_CHAOSMIXRESULT));
	pMsg.Result = CB_ERROR;

	if (aRecv->Type == 3 ||
		aRecv->Type == 4 ||
		aRecv->Type == 22 ||
		aRecv->Type == 23)
	{
		int pos = 0;
		int validitemcount = 0;

		for (int n = 0; n<CHAOS_BOX_SIZE; n++) //loc8
		{
			if (lpObj->pChaosBox[n].IsItem() == TRUE)
			{
				if (lpObj->pChaosBox[n].m_Level == 9)
				{
					pos = n;
					validitemcount++;
				}
				else if (lpObj->pChaosBox[n].m_Level == 10)
				{
					pos = n;
					validitemcount++;
				}
				else if (lpObj->pChaosBox[n].m_Level == 11)
				{
					pos = n;
					validitemcount++;
				}
				else if (lpObj->pChaosBox[n].m_Level == 12)
				{
					pos = n;
					validitemcount++;
				}
				else if (lpObj->pChaosBox[n].m_Level == 13) //new
				{
					pos = n;
					validitemcount++;
				}
				else if (lpObj->pChaosBox[n].m_Level == 14)
				{
					pos = n;
					validitemcount++;
				}
			}
		}

		if (validitemcount != 1)
		{
			pMsg.Result = 0;
			IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
			return;
		}

		int iItemWidth = 0;
		int iItemHeight = 0;

		lpObj->pChaosBox[pos].GetSize(iItemWidth, iItemHeight);

		if (!CheckInventoryEmptySpace(lpObj, iItemHeight, iItemWidth))
		{
			pMsg.Result = 0;
			IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
			return;
		}

		if (g_LuckyItemManager.IsLuckyItemEquipment(lpObj->pChaosBox[pos].m_Type) == TRUE)
		{
			pMsg.Result = 7;
			IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
			return;
		}
	}
	//season4 add-on end

	lpObj->bIsChaosMixCompleted = true;
	int iMixType = aRecv->Type;

	switch ( iMixType )	// Season X Main ENG
	{
		case CHAOS_TYPE_DEVILSQUARE:
			if (aRecv->MixCount == 1)
			{
				g_MixSystem.DevilSquareItemChaosMix(lpObj);
			}

			else
			{
				g_MixSystem.DevilSquareItemChaosMix_Multi(lpObj, aRecv->MixCount);
			}
			break;
		case CHAOS_TYPE_UPGRADE_10:
			g_MixSystem.PlusItemLevelChaosMix(lpObj, CHAOS_TYPE_UPGRADE_10);
			break;
		case CHAOS_TYPE_UPGRADE_11:
			g_MixSystem.PlusItemLevelChaosMix(lpObj, CHAOS_TYPE_UPGRADE_11);
			break;
		case CHAOS_TYPE_UPGRADE_12:
			g_MixSystem.PlusItemLevelChaosMix(lpObj, CHAOS_TYPE_UPGRADE_12);
			break;
		case CHAOS_TYPE_UPGRADE_13:
			g_MixSystem.PlusItemLevelChaosMix(lpObj, CHAOS_TYPE_UPGRADE_13);
			break;
		case CHAOS_TYPE_UPGRADE_14:
			g_MixSystem.PlusItemLevelChaosMix(lpObj, CHAOS_TYPE_UPGRADE_14);
			break;
		case CHAOS_TYPE_UPGRADE_15:
			g_MixSystem.PlusItemLevelChaosMix(lpObj, CHAOS_TYPE_UPGRADE_15);
			break;
		case CHAOS_TYPE_DINORANT:
			if (aRecv->MixCount == 1)
			{
				g_MixSystem.PegasiaChaosMix(lpObj);
			}

			else
			{
				g_MixSystem.PegasiaChaosMix_Multi(lpObj, aRecv->MixCount);
			}
			break;
		case CHAOS_TYPE_FRUIT:
			if (aRecv->MixCount == 1)
			{
				g_MixSystem.CircleChaosMix(lpObj);
			}

			else
			{
				g_MixSystem.CircleChaosMix_Multi(lpObj, aRecv->MixCount);
			}
			break;
		case CHAOS_TYPE_SECOND_WING:
		case CHAOS_TYPE_CLOAK:
			g_MixSystem.WingChaosMix(lpObj);
			break;
		case CHAOS_TYPE_BLOODCATLE:
			if (aRecv->MixCount == 1)
			{
				g_MixSystem.BloodCastleItemChaosMix(lpObj);
			}

			else
			{
				g_MixSystem.BloodCastleItemChaosMix_Multi(lpObj, aRecv->MixCount);
			}
			break;
		case CHAOS_TYPE_DEFAULT:
		case CHAOS_TYPE_FIRST_WING:
			g_MixSystem.DefaultChaosMix(lpObj);
			break;
		case CHAOS_TYPE_SETITEM:	// #warning mix domyslnie wylaczony w s2 i wyzej
			g_MixSystem.SetItemChaosMix(lpObj);
			break;
		case CHAOS_TYPE_DARKHORSE:
			g_MixSystem.DarkHorseChaosMix(lpObj);
			break;
		case CHAOS_TYPE_DARKSPIRIT:
			g_MixSystem.DarkSpiritChaosMix(lpObj);
			break;
		case CHAOS_TYPE_BLESS_POTION:
			if (aRecv->MixCount == 1)
			{
				g_MixSystem.BlessPotionChaosMix(lpObj);
			}

			else
			{
				g_MixSystem.BlessPotionChaosMix_Multi(lpObj, aRecv->MixCount);
			}
			break;
		case CHAOS_TYPE_SOUL_POTION:
			if (aRecv->MixCount == 1)
			{
				g_MixSystem.SoulPotionChaosMix(lpObj);
			}

			else
			{
				g_MixSystem.SoulPotionChaosMix_Multi(lpObj, aRecv->MixCount);
			}
			break;
		case CHAOS_TYPE_LIFE_STONE:
			g_MixSystem.LifeStoneChaosMix(lpObj);
			break;
		case CHAOS_TYPE_CASTLE_SPECIAL_ITEM_MIX:
			g_MixSystem.CastleSpecialItemMix(lpObj);
			break;
		case CHAOS_TYPE_HT_BOX:
			g_MixSystem.HiddenTreasureBoxItemMix(lpObj);
			break;
		case CHAOS_TYPE_FENRIR_01:
			g_MixSystem.Fenrir_01Level_Mix(lpObj);
			break;
		case CHAOS_TYPE_FENRIR_02:
			g_MixSystem.Fenrir_02Level_Mix(lpObj);
			break;
		case CHAOS_TYPE_FENRIR_03:
			g_MixSystem.Fenrir_03Level_Mix(lpObj);
			break;
		case CHAOS_TYPE_FENRIR_04:
			g_MixSystem.Fenrir_04Upgrade_Mix(lpObj);
			break;
		case CHAOS_TYPE_COMPOUNDPOTION_LV1:
			if (aRecv->MixCount == 1)
			{
				g_MixSystem.ShieldPotionLv1_Mix(lpObj);
			}

			else
			{
				g_MixSystem.ShieldPotionLv1_Mix_Multi(lpObj, aRecv->MixCount);
			}
			break;
		case CHAOS_TYPE_COMPOUNTPOTION_LV2:
			if (aRecv->MixCount == 1)
			{
				g_MixSystem.ShieldPotionLv2_Mix(lpObj);
			}

			else
			{
				g_MixSystem.ShieldPotionLv2_Mix_Multi(lpObj, aRecv->MixCount);
			}
			break;
		case CHAOS_TYPE_COMPOUNTPOTION_LV3:
			if (aRecv->MixCount == 1)
			{
				g_MixSystem.ShieldPotionLv3_Mix(lpObj);
			}

			else
			{
				g_MixSystem.ShieldPotionLv3_Mix_Multi(lpObj, aRecv->MixCount);
			}
			break;
		case CHAOS_TYPE_JEWELOFHARMONY_PURITY:
			if (aRecv->MixCount == 1)
			{
				g_kJewelOfHarmonySystem.PurityJewelOfHarmony(lpObj);
			}

			else
			{
				g_kJewelOfHarmonySystem.PurityJewelOfHarmony_MultiMix(lpObj, aRecv->MixCount);
			}
			break;
		case CHAOS_TYPE_JEWELOFHARMONY_MIX_SMELTINGITEM:
			if (aRecv->MixCount == 1)
			{
				g_kJewelOfHarmonySystem.MakeSmeltingStoneItem(lpObj);
			}

			else
			{
				g_kJewelOfHarmonySystem.MakeSmeltingStoneItem_MultiMix(lpObj, aRecv->MixCount);
			}
			break;
		case CHAOS_TYPE_JEWELOFHARMONY_RESTORE_ITEM:
			g_kJewelOfHarmonySystem.RestoreStrengthenItem(lpObj);
			break;
		case CHAOS_TYPE_380_OPTIONITEM:
			g_kItemSystemFor380.ChaosMix380ItemOption(lpObj);
			break;
		case CHAOS_TYPE_LOTTERY_MIX:
			g_MixSystem.LotteryItemMix(lpObj);
			break;
		case CHAOS_TYPE_OLD_PAPER:
			g_MixSystem.IllusionTempleItemChaosMix(lpObj);
			break;
		case CHAOS_TYPE_CONDOR_FEATHER:
			g_MixSystem.ThirdWingLevel1ChaosMix(lpObj);
			break;
		case CHAOS_TYPE_THIRD_WING:	
			g_MixSystem.ThirdWingLevel2ChaosMix(lpObj);
			break;
		case CHAOS_TYPE_BLOSSOM_MIX:
			g_MixSystem.CherryBlossomMix(lpObj);
			break;
		case CHAOS_TYPE_SEED_EXTRACT:
			g_MixSystem.SeedExtractMix(lpObj);
			break;
		case CHAOS_TYPE_SEED_SPHERE_COMPOSITE:
			g_MixSystem.SeedSphereCompositeMix(lpObj);
			break;
		case CHAOS_TYPE_SET_SEED_SPHERE:
			g_MixSystem.SetSeedSphereMix(lpObj, aRecv->Pos);
			break;
		case CHAOS_TYPE_SEED_SPHERE_REMOVE:
			g_MixSystem.SeedSphereRemoveMix(lpObj, aRecv->Pos);
			break;
		case CHAOS_TYPE_SECROMICON:
			g_MixSystem.SecromiconMix(lpObj);
			break;
		case CHAOS_TYPE_PREMIUM_BOX:
			if (aRecv->MixCount == 1)
			{
				g_MixSystem.PremiumBoxMix(lpObj);
			}

			else
			{
				g_MixSystem.PremiumBoxMix_Multi(lpObj, aRecv->MixCount);
			}
			break;
		case CHAOS_TYPE_SUMMONS_MIX:
			g_MixSystem.SummonsMix(lpObj);
			break;
		case CHAOS_TYPE_LUCKYITEM_EXCHANGE:
			g_LuckyItemManager.LuckyItemTicketExchange(lpObj);
			break;
		case CHAOS_TYPE_LUCKYITEM_REFINE:
			g_LuckyItemManager.LuckyItemSmelting(lpObj);
			break;
		case CHAOS_TYPE_SOCKETWEAPON:
			g_MixSystem.ItemRefineMix(lpObj);
			break;
		case CHAOS_TYPE_ARCABATTLE_BOOTYMIX:
			g_ArcaBattle.BootyExchange(lpObj);
			break;
		case CHAOS_TYPE_ARCABATTLE_SIGNOFLORD:
			g_ArcaBattle.CGReqMarkRegButtonClick(lpObj);
			break;
		case CHAOS_TYPE_MONSTERWING:
			g_MixSystem.MonsterWingMix(lpObj);
			break;
		case CHAOS_TYPE_CCF_REWARD:
			g_MixSystem.CCFRewardMix(lpObj);
			break;
		case CHAOS_TYPE_DSF_REWARD:
			g_MixSystem.DSFRewardMix(lpObj);
			break;
		case CHAOS_TYPE_ADVANCED_WING:
			g_MixSystem.AdvancedWingMix(lpObj);
			break;
		default:
			g_Log.Add("[%s][%s] Undefine chaosmix type detect %d",
				lpObj->AccountID, lpObj->Name, iMixType);
			break;
	}
}



void GameProtocol::CGChaosBoxUseEnd(int aIndex)
{
	if ( !gObjIsConnectedGP(aIndex))
	{
		g_Log.AddC(TColor::Red,  "error-L3 [%s][%d]", __FILE__, __LINE__);
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	PMSG_DEFAULT pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x87, sizeof(pMsg));
	IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

	if ( lpObj->m_IfState.use && lpObj->m_IfState.type == 7 )
	{
		lpObj->m_IfState.use = 0;
		lpObj->m_IfState.state = 0;
	}

	g_MixSystem.ChaosBoxInit(lpObj);
	g_PentagramMixSystem.PentagramMixBoxInit(lpObj);
	gObjInventoryCommit(lpObj->m_Index);
	lpObj->m_bIsCastleNPCUpgradeCompleted = false;
}





void GameProtocol::PMoveProc(PMSG_MOVE* lpMove, int aIndex)
{
	if ( !ObjectMaxRange(aIndex))
	{
		g_Log.Add("error-L3 : move protocol index error %s %d", __FILE__, __LINE__);
		return;
	}

	if(gObj[aIndex].m_bOffLevel == true)
	{
		return;
		}

	PMSG_RECVMOVE pMove;
	short n;
	short pathtable;
	short ax;
	short ay;
	int sx;
	int sy;
	int tick = GetTickCount();
	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Connected != PLAYER_PLAYING)
	{
		return; // for reconnect incase player is using MuBot
	}
	if ( MAX_MAP_RANGE(lpObj->MapNumber) == FALSE ) 
	{
		return;
	}

	if (lpObj->Type == OBJ_USER && lpObj->m_PlayerData->m_bSecurityCheck == false )
	{
		gObjSetPosition(aIndex, lpObj->X, lpObj->Y);
		return;
	}

	if ( lpObj->RegenOk > 0 )
		return;

	if ( lpObj->m_bMapSvrMoveReq == true )
		return;

	if ( lpObj->m_bMapSvrMoveReq_2 == true )
		return;

	if ( lpObj->m_SkillInfo.LordSummonTime )
	{
		lpObj->m_SkillInfo.LordSummonTime = 0;
		char msg[255];
		wsprintf(msg, Lang.GetText(0,146));
		GCServerMsgStringSend(msg, lpObj->m_Index, 1);
	}

	if ( (GetTickCount()-lpObj->m_LastMoveTime) < 100 )
		return;

	if ( lpObj->Teleport )
		return;

	if ( gObjCheckUsedBuffEffect(&gObj[aIndex], BUFFTYPE_STONE) || gObjCheckUsedBuffEffect(&gObj[aIndex], BUFFTYPE_STUN) || gObjCheckUsedBuffEffect(&gObj[aIndex], BUFFTYPE_SLEEP)
		|| gObjCheckUsedBuffEffect(&gObj[aIndex], BUFFTYPE_FREEZE_2) || gObjCheckUsedBuffEffect(&gObj[aIndex], BUFFTYPE_EARTH_BINDS))
	{
		gObjSetPosition(lpObj->m_Index, lpObj->X, lpObj->Y);
		return;
	}

	if ( lpObj->Type == OBJ_USER && gObjCheckMoveArea(aIndex, lpMove->X, lpMove->Y) == false)
	{
		gObjSetPosition(lpObj->m_Index, lpObj->X, lpObj->Y);
		g_Log.Add("[ANTI-HACK] (%s)(%s) Distance is over 30 tiles.", lpObj->AccountID, lpObj->Name);
		return;
	}

	lpObj->m_LastMoveTime = GetTickCount();
	lpObj->m_Rest = 0;
	lpObj->PathCur = 0;
	lpObj->Dir = lpMove->Path[0] >> 4;
	lpObj->PathCount = lpMove->Path[0] & 0x0F;

	if ( lpObj->PathCount > 15 )
	{
		g_Log.Add("error-L3 : Path Count error %d id:%s %s %d",
			lpObj->PathCount, lpObj->AccountID, __FILE__, __LINE__);
		return;
	}

	for ( n=0;n<15;n++)
	{
		lpObj->PathX[n]=0;
		lpObj->PathY[n]=0;
		lpObj->PathOri[n]=0;
	}

	sx = lpMove->X;
	sy = lpMove->Y;

	if ( DG_MAP_RANGE(lpObj->MapNumber) == TRUE )
	{
		if ( g_DoppelGanger.GetDoppelgangerState() == 1 && g_DoppelGanger.CheckMapTile(lpObj->MapNumber, lpObj->X, lpObj->Y) == FALSE )
		{
			gObjSetPosition(lpObj->m_Index, lpObj->X, lpObj->Y);
			return;
		}
	}

	/*
	if ( gObjCheckXYMapTile(lpObj, 1) == TRUE )
	{
		lpObj->PathCount = 0;
		lpObj->PathCur = 0;
		lpObj->PathStartEnd = 0;
		gObjSetPosition(lpObj->m_Index, lpObj->X, lpObj->Y);
		return;
	}*/

	lpObj->PathX[0] = lpMove->X;
	ax = lpObj->PathX[0];
	lpObj->PathY[0] = lpMove->Y;
	ay = lpObj->PathY[0];
	lpObj->PathDir[0] = lpObj->Dir;
	lpObj->PathStartEnd = 1;

	if ( lpObj->PathCount > 0 )
	{
		lpObj->PathCur = 1;
		lpObj->PathCount++;
	}

	for (n=1;n<lpObj->PathCount;n++)
	{
		if ( (n%2) == 1 )
		{
			pathtable = lpMove->Path[(n+1) / 2] >> 4;
		}
		else
		{
			pathtable = lpMove->Path[(n+1) / 2] & 0x0F;
		}

		ax += RoadPathTable[pathtable*2];
		ay += RoadPathTable[pathtable*2+1];
		
		lpObj->PathOri[n-1] = pathtable;
		lpObj->PathDir[n]= pathtable;
		lpObj->PathX[n] = ax;
		lpObj->PathY[n] = ay;
	}

	if ( lpObj->PathCount > 0 )
	{
		int nextX;
		int nextY;
		BYTE mapnumber;
		BYTE attr;
		nextX = lpObj->PathX[1];
		nextY = lpObj->PathY[1];
		mapnumber = lpObj->MapNumber;
		attr = MapC[mapnumber].GetAttr(nextX, nextY);

		if ( (((BYTE)attr & (BYTE)4) == (BYTE)4) || (((BYTE)attr & (BYTE)8) == (BYTE)8) )
		{
			for ( n=0 ; n<15 ; n++)
			{
				lpObj->PathX[n] = 0;
				lpObj->PathY[n] = 0;
				lpObj->PathOri[n] = 0;
			}

			lpObj->PathCount = 0;
			lpObj->PathCur = 0;
			lpObj->PathStartEnd = 0;
			gObjSetPosition(lpObj->m_Index, lpObj->X, lpObj->Y);
			return;
		}
	}

	lpObj->TX = (BYTE)ax;
	lpObj->TY = (BYTE)ay;

	if ( lpObj->Type == OBJ_USER && IMPERIAL_MAP_RANGE(lpObj->MapNumber) )
	{
		int iCX = lpObj->X;
		int iCY = lpObj->Y;
		BOOL bSetStand = FALSE;
		WORD wMapAttr = 0;
		
		for ( int iCOUNT = 0; iCOUNT < 256; iCOUNT++ )
		{
			if ( iCX > lpObj->TX )
				iCX--;
			if ( iCX < lpObj->TX )
				iCX++;
			if ( iCY > lpObj->TY )
				iCY--;
			if ( iCY < lpObj->TY )
				iCY++;

			wMapAttr = MapC[lpObj->MapNumber].GetAttr(iCX, iCY);
                  
			if ( wMapAttr & 0x10 )
			{
				bSetStand = TRUE;
				break;
			}

			if ( iCX == lpObj->TX && iCY == lpObj->TY )
			{
				break;
			}

			if ( bSetStand == 1 )
			{
				lpObj->m_Rest = 1;
				lpObj->PathCur = 0;
				lpObj->PathCount = 0;
				lpObj->PathStartEnd = 0;
				gObjSetPosition(lpObj->m_Index, lpObj->X, lpObj->Y);
				
				PMSG_ACTIONRESULT pActionResult;
				
				PHeadSetB((LPBYTE)&pActionResult, 0x18, sizeof(pActionResult));
				pActionResult.NumberH = SET_NUMBERH(aIndex);
				pActionResult.NumberL = SET_NUMBERL(aIndex);
				pActionResult.ActionNumber = 0x7A;
				pActionResult.Dir = lpObj->Dir;
				pActionResult.TargetNumberH = 0;
				pActionResult.TargetNumberL = 0;

				IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pActionResult, sizeof(pActionResult));
				return;
			}
		}
	}

	if (g_ConfigRead.server.GetServerType() == SERVER_CASTLE)
	{
		if (lpObj->Type == OBJ_USER)
		{
			int nextX = lpObj->X;
			int nextY = lpObj->Y;
			int iResult = 0; //should be BOOL
			WORD attr = 0;

			for (int i = 0; i < 256; i++)
			{
				if (nextX > lpObj->TX)
				{
					nextX -= 1;
				}

				if (nextX < lpObj->TX)
				{
					nextX += 1;
				}

				if (nextY > lpObj->TY)
				{
					nextY -= 1;
				}

				if (nextY < lpObj->TY)
				{
					nextY += 1;
				}

				attr = MapC[lpObj->MapNumber].GetAttr(nextX, nextY);

				if ((attr & 16) == 16)
				{
					iResult = 1;
					break;
				}

				if (nextX == lpObj->TX)
				{
					if (nextY == lpObj->TY)
					{
						break;
					}
				}
			}

			if (iResult == 1)
			{
				lpObj->m_Rest = 1;
				lpObj->PathCur = 0;
				lpObj->PathCount = 0;
				lpObj->PathStartEnd = 0;

				gObjSetPosition(lpObj->m_Index, lpObj->X, lpObj->Y);

				PMSG_ACTIONRESULT pActionResult;

				PHeadSetB((LPBYTE)&pActionResult, 0x18, sizeof(pActionResult));
				pActionResult.NumberH = SET_NUMBERH(aIndex);
				pActionResult.NumberL = SET_NUMBERL(aIndex);
				pActionResult.ActionNumber = 0x7A;
				pActionResult.Dir = lpObj->Dir;
				pActionResult.TargetNumberH = 0;
				pActionResult.TargetNumberL = 0;

				IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pActionResult, sizeof(pActionResult));
				return;
			}
		}
	}

	if ( BC_MAP_RANGE(lpObj->MapNumber) && lpObj->Type == OBJ_USER )
	{
		int iBridgeIndex = g_BloodCastle.GetBridgeIndex(lpObj->MapNumber);
		switch ( g_BloodCastle.GetCurrentState(iBridgeIndex) )
		{
			case 0x01:
				if ( lpObj->TY > 15 )
				{
					lpObj->m_Rest = 1;
					lpObj->PathCur = 0;
					lpObj->PathCount = 0;
					lpObj->PathStartEnd = 0;
					gObjSetPosition(lpObj->m_Index, lpObj->X, lpObj->Y);

					PMSG_ACTIONRESULT pActionResult;

					PHeadSetB((LPBYTE)&pActionResult, 0x18, sizeof(pActionResult));
					pActionResult.NumberH = SET_NUMBERH(aIndex);
					pActionResult.NumberL = SET_NUMBERL(aIndex);
					pActionResult.ActionNumber = 0x7A;
					pActionResult.Dir = lpObj->Dir;
					pActionResult.TargetNumberH = 0;
					pActionResult.TargetNumberL = 0;

					IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pActionResult, sizeof(pActionResult));
					return;
				}
				break;
			case 0x02:
				if ( lpObj->Y > 17 && g_BloodCastle.CheckPlayStart(iBridgeIndex) == false)
				{
					gObjMoveGate(lpObj->m_Index, iBridgeIndex+66);
					return;
				}

				if ( lpObj->TY > 15 && g_BloodCastle.CheckPlayStart(iBridgeIndex) == false )
				{
					lpObj->m_Rest = 1;
					lpObj->PathCur = 0;
					lpObj->PathCount = 0;
					lpObj->PathStartEnd = 0;
					gObjSetPosition(lpObj->m_Index, lpObj->X, lpObj->Y);

					PMSG_ACTIONRESULT pActionResult;

					PHeadSetB((LPBYTE)&pActionResult, 0x18, sizeof(pActionResult));
					pActionResult.NumberH = SET_NUMBERH(aIndex);
					pActionResult.NumberL = SET_NUMBERL(aIndex);
					pActionResult.ActionNumber = 0x7A;
					pActionResult.Dir = lpObj->Dir;
					pActionResult.TargetNumberH = 0;
					pActionResult.TargetNumberL = 0;

					IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pActionResult, sizeof(pActionResult));
					return;
				}

				if ( lpObj->TY > 76 &&  g_BloodCastle.CheckCastleDoorLive(iBridgeIndex) == true )
				{
					lpObj->m_Rest = 1;
					lpObj->PathCur = 0;
					lpObj->PathCount = 0;
					lpObj->PathStartEnd = 0;
					gObjSetPosition(lpObj->m_Index, lpObj->X, lpObj->Y);

					PMSG_ACTIONRESULT pActionResult;

					PHeadSetB((LPBYTE)&pActionResult, 0x18, sizeof(pActionResult));
					pActionResult.NumberH = SET_NUMBERH(aIndex);
					pActionResult.NumberL = SET_NUMBERL(aIndex);
					pActionResult.ActionNumber = 0x7A;
					pActionResult.Dir = lpObj->Dir;
					pActionResult.TargetNumberH = 0;
					pActionResult.TargetNumberL = 0;

					IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pActionResult, sizeof(pActionResult));
					return;
				}
				break;
		}
	}

	PHeadSetB((LPBYTE)&pMove, 0xD4, sizeof(pMove));
	pMove.NumberH = SET_NUMBERH(aIndex);
	pMove.NumberL = SET_NUMBERL(aIndex);
	pMove.X = ax;
	pMove.Y = ay;
	pMove.Path = lpObj->Dir << 4;

	if ( lpObj->Type == OBJ_USER )
	{
		if (lpObj->m_byCCF_CanEnter == 0xFF)
		{
			lpObj->m_byCCF_CanEnter = 0;

			if (lpObj->m_bCCF_UI_Using == TRUE)
			{
				lpObj->m_bCCF_UI_Using = FALSE;
			}
		}

		if (g_ConfigRead.server.GetServerType() == SERVER_MARKET)
		{
			if (IT_MAP_RANGE(lpObj->MapNumber))
			{
				if (lpObj->m_nITR_RelicsTick > 0 || lpObj->m_wITR_NpcType > 0 || lpObj->m_byITR_StoneState != 99 || lpObj->m_bITR_RegisteringRelics == true || lpObj->m_bITR_GettingRelics == true)
				{
					g_IT_Event.CancleStoneState(lpObj->m_wITR_NpcType, lpObj->m_byITR_StoneState, lpObj->MapNumber);
					lpObj->m_nITR_RelicsTick = 0;
					lpObj->m_wITR_NpcType = 0;
					lpObj->m_byITR_StoneState = 99;
					lpObj->m_bITR_RegisteringRelics = false;
					lpObj->m_bITR_GettingRelics = false;
				}
			}
		}

		if ( lpObj->m_IfState.use )
		{
			if ( lpObj->m_IfState.type == 3 )
			{
				lpObj->TargetNpcNumber = -1;
				lpObj->m_IfState.type = 0;
				lpObj->m_IfState.use = 0;
			}
		}

		if ( !gObjPositionCheck(lpObj))
		{
			lpObj->PathCur = 0;
			lpObj->PathCount = 0;
			ax = lpObj->X;
			ay = lpObj->Y;
			lpObj->TX = (BYTE)ax;
			lpObj->TY = (BYTE)ay;
			pMove.X = ax;
			pMove.Y = ay;
		}

		IOCP.DataSend(aIndex, (LPBYTE)&pMove, pMove.h.size);
	}

	int MVL = MAX_VIEWPORT;

	if ( lpObj->Type == OBJ_MONSTER )
		MVL = MaxViewportMonster;
		
	for (n=0;n<MVL;n++)
	{
		if ( lpObj->VpPlayer2[n].state == TRUE )
		{
			int number = lpObj->VpPlayer2[n].number;
			
			if ( number >= 0 )
			{
				if ( gObj[number].Connected > PLAYER_CONNECTED )
				{
					if ( gObj[number].Live )
					{
						if ( gObj[number].Type == OBJ_USER )
						{
							IOCP.DataSend(lpObj->VpPlayer2[n].number, (LPBYTE)&pMove, pMove.h.size);
						}
					}
				}
				else
				{
					lpObj->VpPlayer2[n].number= -1;
					lpObj->VpPlayer2[n].state = 0;
					lpObj->VPCount2--;
				}
			}
		}
	}

	MapC[lpObj->MapNumber].ClearStandAttr(lpObj->m_OldX, lpObj->m_OldY);
	MapC[lpObj->MapNumber].SetStandAttr(lpObj->TX, lpObj->TY);
	lpObj->m_OldX = lpObj->TX;
	lpObj->m_OldY = lpObj->TY;
	lpObj->X = sx;
	lpObj->Y = sy;
	lpObj->m_ViewState = 0;

	if (lpObj->Type == OBJ_USER) // Recv Check
	{
		if (rand() % 5 == 0)
		{
			this->GCSendRecvCheck(lpObj->m_Index);
		}
	}
}



void GameProtocol::RecvPositionSetProc(PMSG_POSISTION_SET * lpMove, int aIndex)
{
	short n;

	if (  ObjectMaxRange(aIndex) == FALSE)
	{
		g_Log.Add("error : move protocol index error %s %d", __FILE__, __LINE__);
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if ( this->PacketCheckTime(aIndex) == FALSE )
	{
		return;
	}

	if ( lpObj->Teleport != 0 )
	{
		return;
	}

	if ( MAX_MAP_RANGE(lpObj->MapNumber) == FALSE )
	{
		return;
	}

	lpObj->X = lpMove->X;
	lpObj->Y = lpMove->Y;

	if ( CC_MAP_RANGE(lpObj->MapNumber) != FALSE )
	{
		if ( (GetTickCount() - lpObj->m_iChaosCastleBlowTime ) < 1000 )
		{
			return;
		}
	}

	if (lpObj->MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL)
	{
		if ((GetTickCount() - lpObj->m_nCCF_BlowTime) < 1000)
		{
			return;
		}
	}

	if ( gObjCheckXYMapTile(lpObj, 2) == TRUE)
	{
		return;
	}

	PMSG_RECV_POSISTION_SET pMove;

	PHeadSetB((LPBYTE)&pMove, 0x15, sizeof(pMove));
	pMove.NumberH = SET_NUMBERH(aIndex);
	pMove.NumberL = SET_NUMBERL(aIndex);
	pMove.X = lpMove->X;
	pMove.Y = lpMove->Y;
	lpObj->TX = lpMove->X;
	lpObj->TY = lpMove->Y;

	if ( ::gObjPositionCheck(lpObj) == FALSE )
	{
		return;
	}

	CreateFrustrum(lpObj->X, lpObj->Y, aIndex);

	if ( lpObj->Type == OBJ_USER )
	{
		IOCP.DataSend(aIndex, (UCHAR *)&pMove, pMove.h.size);
	}

	int MVL = MAX_VIEWPORT;

	if ( lpObj->Type == OBJ_MONSTER )
	{
		MVL = MaxViewportMonster;
	}

	for ( n=0;n<MVL;n++)
	{
		if ( lpObj->VpPlayer2[n].type == OBJ_USER )
		{
			if ( lpObj->VpPlayer2[n].state == 1 )
			{
				int Index = lpObj->VpPlayer2[n].number;

				if (ObjectMaxRange(Index) == false)
				{
					continue;
				}

				if ( gObj[Index].Connected > PLAYER_CONNECTED && gObj[Index].Live != FALSE)
				{
					IOCP.DataSend(lpObj->VpPlayer2[n].number, (UCHAR *)&pMove, pMove.h.size);
				}
				else
				{
					lpObj->VpPlayer2[n].number = -1;
					lpObj->VpPlayer2[n].state = FALSE;
					lpObj->VPCount2--;
				}
			}
		}
	}

	MapC[lpObj->MapNumber].ClearStandAttr(lpObj->m_OldX, lpObj->m_OldY);
	MapC[lpObj->MapNumber].SetStandAttr(lpObj->TX, lpObj->TY);

	lpObj->m_OldX = lpObj->TX;
	lpObj->m_OldY = lpObj->TY;

}



void GameProtocol::CGAttack(PMSG_ATTACK* lpMsg, int aIndex)
{
	if(aIndex < 0 || aIndex > g_ConfigRead.server.GetObjectMax()-1)
	{
		return;
	}

	LPOBJ lpObj;
	LPOBJ lpTargetObj;
	int usernumber = MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);

	if ( usernumber < 0 || usernumber > g_ConfigRead.server.GetObjectMax()-1)
	{
		g_Log.Add("[CGAttack] [UserIndex Error] :%s %d %d", __FILE__, __LINE__, usernumber);
		return;
	}

	if ( !gObj[usernumber].Live )
		return;

	if ( gObj[aIndex].Type == OBJ_USER  && gObj[aIndex].m_PlayerData->m_bSecurityCheck == false)
		return;

	lpObj = &gObj[aIndex];

	int iTimeCalc = GetTickCount() - lpObj->m_LastAttackTime;

	if (iTimeCalc < lpObj->m_DetectSpeedHackTime) //season4 changed (fix)
	{
		lpObj->m_DetectCount++;
		lpObj->m_SumLastAttackTime += iTimeCalc;

		if (lpObj->m_DetectCount > gHackCheckCount)
		{
			lpObj->m_DetectedHackKickCount++;
			lpObj->m_SpeedHackPenalty = gSpeedHackPenalty;

			if (gIsKickDetecHackCountLimit)
			{
				if (lpObj->m_DetectedHackKickCount > gDetectedHackKickCount)
				{
					g_Log.Add("[%s][%s] Kick DetecHackCountLimit Over User (%d)", lpObj->AccountID, lpObj->Name, lpObj->m_DetectedHackKickCount);
					this->GCSendDisableReconnect(aIndex);
					//IOCP.CloseClient(aIndex);
					return;
				}

				g_Log.Add("[%s][%s] Attack Speed Is Wrong Normal (%d)(%d) Penalty %d", lpObj->AccountID, lpObj->Name, lpObj->m_DetectSpeedHackTime, lpObj->m_SumLastAttackTime / lpObj->m_DetectCount, lpObj->m_SpeedHackPenalty);
			}

		}

		lpObj->m_LastAttackTime = GetTickCount();
	}
	else
	{
		lpObj->m_SumLastAttackTime = 0;
		lpObj->m_DetectCount = 0;
	}

	lpObj->m_LastAttackTime = GetTickCount();

	if ( bIsIgnorePacketSpeedHackDetect )
	{
		if ( lpObj->m_SpeedHackPenalty > 0 )
		{
			lpObj->m_SpeedHackPenalty--;
			
			g_Log.Add("[%s][%s] %s Apply Attack Speed Penalty (%d left)",
				lpObj->AccountID, lpObj->Name, Lang.GetText(0,202+lpObj->Class),
				lpObj->m_SpeedHackPenalty);

			return;
		}
	}
	
	lpTargetObj = &gObj[usernumber];
	lpObj->Dir = lpMsg->DirDis;
	GCActionSend(lpObj, lpMsg->AttackAction, aIndex, usernumber);
	gComboAttack.CheckCombo(aIndex, 0);
	gObjAttack(lpObj, lpTargetObj, NULL, FALSE, FALSE, 0, FALSE, 0, 0);
	
	lpObj->UseMagicNumber = 0;
}

void GameProtocol::GCDamageSend(int aIndex, int TargetIndex, int AttackDamage, int MSBFlag, int MSBDamage, int iShieldDamage)
{
	PMSG_ATTACKRESULT pResult;

	PHeadSetB((LPBYTE)&pResult, 0x11, sizeof(pResult));
	pResult.NumberH = SET_NUMBERH(TargetIndex);
	pResult.NumberL = SET_NUMBERL(TargetIndex);
	pResult.DamageH = SET_NUMBERH(AttackDamage);
	pResult.DamageL = SET_NUMBERL(AttackDamage);
	pResult.btShieldDamageH = SET_NUMBERH(iShieldDamage);
	pResult.btShieldDamageL = SET_NUMBERL(iShieldDamage);
	pResult.IGCDamage = AttackDamage;

	if ( MSBFlag != FALSE )
	{
		pResult.NumberH &= 0x7F;
		pResult.NumberH |= 0x80;
	}

	pResult.DamageTypeH = HIBYTE(MSBDamage);
	pResult.DamageTypeL = LOBYTE(MSBDamage);

	if ( gObj[TargetIndex].Type == OBJ_USER )
	{
		IOCP.DataSend(TargetIndex, (LPBYTE)&pResult, pResult.h.size);
	}

	if ( cManager.WatchTargetIndex == TargetIndex || cManager.WatchTargetIndex == aIndex )
	{
		cManager.DataSend((LPBYTE)&pResult, pResult.h.size);
	}

	if ( gObj[aIndex].Type == OBJ_USER )
	{
		IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
	}
}


struct PMSG_DAMAGE
{
	PBMSG_HEAD h;	// C1:F3:07
	BYTE subcode;	// 3
	BYTE DamageH;	// 4
	BYTE DamageL;	// 5
	BYTE btShieldDamageH;	// 6
	BYTE btShieldDamageL;	// 7
};

void GameProtocol::GCDamageSendPoison(int aIndex, int damage, int iShieldDamage)
{
	if ( gObj[aIndex].Type != OBJ_USER )
	{
		return;
	}

	PMSG_DAMAGE pDamage;

	PHeadSubSetB((LPBYTE)&pDamage, 0xF3, 0x07, sizeof(pDamage));
	pDamage.DamageH = SET_NUMBERH(damage);
	pDamage.DamageL = SET_NUMBERL(damage);
	pDamage.btShieldDamageH = SET_NUMBERH(iShieldDamage);
	pDamage.btShieldDamageL = SET_NUMBERL(iShieldDamage);

	IOCP.DataSend(aIndex, (LPBYTE)&pDamage, pDamage.h.size);
}


struct PMSG_KILLPLAYER
{
	PBMSG_HEAD h;	// C1:16
	BYTE NumberH;	// 3
	BYTE NumberL;	// 4
	BYTE ExpH;	// 5
	BYTE ExpL;	// 6
	BYTE DamageH;	// 7
	BYTE DamageL;	// 8
};


void GameProtocol::GCKillPlayerExpSend(int aIndex, int TargetIndex, int exp, int AttackDamage, BOOL MSBFlag)
{
	PMSG_KILLPLAYER_EXT pkillMsg;

	PHeadSetBE((LPBYTE)&pkillMsg, 0x9c, sizeof(pkillMsg));
	pkillMsg.NumberH = SET_NUMBERH(TargetIndex);
	pkillMsg.NumberL = SET_NUMBERL(TargetIndex);
	pkillMsg.ExpH = SET_NUMBERHW(exp);
	pkillMsg.ExpL = SET_NUMBERLW(exp);
	pkillMsg.DamageH = SET_NUMBERH(AttackDamage);
	pkillMsg.DamageL = SET_NUMBERL(AttackDamage);

	if ( MSBFlag != FALSE )
	{
		pkillMsg.NumberH &= 0x7F;
		pkillMsg.NumberH |= 0x80;
	}

	if (  BC_MAP_RANGE(gObj[aIndex].MapNumber) != FALSE )
	{
		g_BloodCastle.AddExperience(aIndex, exp);
	}

	IOCP.DataSend(aIndex, (UCHAR*)&pkillMsg, pkillMsg.h.size);
}

void GameProtocol::GCKillPlayerMasterExpSend(int aIndex, int TargetIndex, int exp, int AttackDamage, BOOL MSBFlag)
{
	PMSG_KILLPLAYER_EXT pkillMsg;

	PHeadSetBE((LPBYTE)&pkillMsg, 0x9C, sizeof(pkillMsg));
	pkillMsg.NumberH = SET_NUMBERH(TargetIndex);
	pkillMsg.NumberL = SET_NUMBERL(TargetIndex);
	pkillMsg.ExpH = SET_NUMBERHW(exp);
	pkillMsg.ExpL = SET_NUMBERLW(exp);
	pkillMsg.DamageH = SET_NUMBERH(AttackDamage);
	pkillMsg.DamageL = SET_NUMBERL(AttackDamage);

	if ( MSBFlag != FALSE )
	{
		pkillMsg.NumberH &= 0x7F;
		pkillMsg.NumberH |= 0x80;
	}

	if (  BC_MAP_RANGE(gObj[aIndex].MapNumber) != FALSE )
	{
		g_BloodCastle.AddExperience(aIndex, exp);
	}

	IOCP.DataSend(aIndex, (UCHAR*)&pkillMsg, pkillMsg.h.size);
}



struct PMSG_DIEPLAYER
{
	PBMSG_HEAD h;	// C1:17
	BYTE NumberH;	// 3
	BYTE NumberL;	// 4
	BYTE Skill;	// 5
	BYTE KillerNumberH;	// 6
	BYTE KillerNumberL;	// 7
};


void GameProtocol::GCDiePlayerSend(LPOBJ lpObj, int TargetIndex, BYTE skill, int KillerIndex)
{
	PMSG_DIEPLAYER pDieMsg;

	PHeadSetB((LPBYTE)&pDieMsg, 0x17, sizeof(pDieMsg));
	pDieMsg.NumberH = SET_NUMBERH(TargetIndex);
	pDieMsg.NumberL = SET_NUMBERL(TargetIndex);
	pDieMsg.Skill = skill;
	pDieMsg.KillerNumberH = SET_NUMBERH(KillerIndex);
	pDieMsg.KillerNumberL = SET_NUMBERL(KillerIndex);
	int iSize = pDieMsg.h.size;

	MsgSendV2(lpObj, (LPBYTE)&pDieMsg, iSize);

	if ( lpObj->Type == OBJ_USER )
	{
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pDieMsg, iSize);
	}
}



void GameProtocol::GCActionSend(LPOBJ lpObj, BYTE ActionNumber, int aIndex, int aTargetIndex)
{
	PMSG_ACTIONRESULT pActionResult;

	PHeadSetB((LPBYTE)&pActionResult, 0x18, sizeof(pActionResult));
	pActionResult.NumberH = SET_NUMBERH(aIndex);
	pActionResult.NumberL = SET_NUMBERL(aIndex);
	pActionResult.ActionNumber = ActionNumber;
	pActionResult.Dir = lpObj->Dir;
	pActionResult.TargetNumberH = SET_NUMBERH(aTargetIndex);
	pActionResult.TargetNumberL = SET_NUMBERL(aTargetIndex);

	MsgSendV2(lpObj, (LPBYTE)&pActionResult, pActionResult.h.size);
}



void GameProtocol::CGActionRecv(PMSG_ACTION * lpMsg, int aIndex)
{
	if ( !ObjectMaxRange(aIndex))
	{
		g_Log.Add("error : action protocol index error %s %d", __FILE__, __LINE__);
		return;
	}

	if(lpMsg->Dir > 7 || lpMsg->h.size > 5)
	{
		g_Log.AddC(TColor::Red,"[%s][%s]DC Hack detected user",gObj[aIndex].AccountID,gObj[aIndex].Name);
		this->GCSendDisableReconnect(aIndex);
		//IOCP.CloseClient(aIndex);
	}
	

	PMSG_ACTIONRESULT pResult;
	LPOBJ lpObj = &gObj[aIndex];

	PHeadSetB((LPBYTE)&pResult, 0x18, sizeof(pResult));
	pResult.ActionNumber = lpMsg->ActionNumber;
	pResult.NumberH = SET_NUMBERH(aIndex);
	pResult.NumberL = SET_NUMBERL(aIndex);
	lpObj->m_ActionNumber = lpMsg->ActionNumber;
	lpObj->Dir = lpMsg->Dir;
	pResult.Dir = lpObj->Dir;
	pResult.TargetNumberH = lpMsg->iTargetIndexH;
	pResult.TargetNumberL = lpMsg->iTargetIndexL;

	if (g_ConfigRead.SeasonX == true)
	{
		if ( lpObj->m_ActionNumber == 0x80)
		{
			lpObj->m_ViewState = 1;
			lpObj->m_Rest = lpObj->m_ActionNumber;
		}
		else if ( lpObj->m_ActionNumber == 0x81 )
		{
			lpObj->m_ViewState = 2;
			lpObj->m_Rest = lpObj->m_ActionNumber;
		}
		else if ( lpObj->m_ActionNumber == 0x82 )
		{
			lpObj->m_ViewState = 3;
			lpObj->m_Rest = lpObj->m_ActionNumber;
		}
	}

	else
	{
		if ( lpObj->m_ActionNumber == 0x80)
		{
			lpObj->m_ViewState = 2;
			lpObj->m_Rest = lpObj->m_ActionNumber;
		}
		else if ( lpObj->m_ActionNumber == 0x81 )
		{
			lpObj->m_ViewState = 3;
			lpObj->m_Rest = lpObj->m_ActionNumber;
		}
		else if ( lpObj->m_ActionNumber == 0x82 )
		{
			lpObj->m_ViewState = 4;
			lpObj->m_Rest = lpObj->m_ActionNumber;
		}
	}

	if(GetTickCount() -  lpObj->m_ActionTime < 500)
	{
		lpObj->m_ActionCount++;

		if(lpObj->m_ActionCount > 9)
		{
			g_Log.AddC(TColor::Red, "[ERROR] lpObj->m_ActionCount > 9");
			this->GCSendDisableReconnect(aIndex);
			//IOCP.CloseClient(aIndex);
			return;
		}
	}

	else
	{
		lpObj->m_ActionCount = 0;
	}

	lpObj->m_ActionTime = GetTickCount();

	
	int MVL = MAX_VIEWPORT;

	if ( lpObj->Type == OBJ_MONSTER )
		MVL = MaxViewportMonster;

	for (int n=0;n<MVL;n++)
	{
		if ( lpObj->VpPlayer2[n].type == OBJ_USER )
		{
			if ( lpObj->VpPlayer2[n].state != 0 && lpObj->VpPlayer2[n].state != 0x10 && lpObj->VpPlayer2[n].state != 0x08)
			{
				IOCP.DataSend(lpObj->VpPlayer2[n].number, (LPBYTE)&pResult, pResult.h.size);
			}
		}
	}
}



void GameProtocol::CGMagicAttack(LPBYTE lpRecv, int aIndex)
{
	if(aIndex < 0 || aIndex > g_ConfigRead.server.GetObjectMax()-1)
	{
		return;
	}

	PMSG_MAGICATTACK * lpMsg = (PMSG_MAGICATTACK *)(lpRecv);

	LPOBJ lpObj;
	LPOBJ lpTargetObj;
	WORD MagicNumber = MAKE_NUMBERW(lpMsg->MagicNumberH, lpMsg->MagicNumberL);
	int usernumber = MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);

	if ( usernumber < 0 || usernumber > g_ConfigRead.server.GetObjectMax()-1 )
	{
		g_Log.Add("error :%s %d %d", __FILE__, __LINE__, usernumber);
		return;
	}

	if ( gObj[usernumber].Live == 0 || gObj[usernumber].Connected  < PLAYER_LOGGED )
		return;

	if ( gObj[aIndex].CloseCount >= 0 )
		return;

	if ( gObj[aIndex].Type == OBJ_USER && gObj[aIndex].m_PlayerData->m_bSecurityCheck == false  )
		return;

	lpObj = &gObj[aIndex];
	lpTargetObj = &gObj[usernumber];
	
	CMagicInf * lpMagic;
	int attackret = gObjCheckAttackArea(aIndex, usernumber);

	if ( attackret )
	{
		g_Log.Add("[%s][%s] Try Attack In Not Attack Area [Protocol] (%s:%d,%d) errortype = %d",
			lpObj->AccountID, lpObj->Name, Lang.GetMap(0,0+lpObj->MapNumber), lpObj->X, lpObj->Y,
			attackret);

		if ( lpObj->Class == 275 )	// Kundun
			lpObj->TargetNumber = -1;

		if ( bIsIgnorePacketSpeedHackDetect )
			return;
	}

	if ( lpObj->Type == OBJ_USER )
	{

		if(MagicNumber == 58) // Nova cast
			usernumber = 58;

		if(MagicNumber == 395)
			usernumber = 395;

		lpMagic = gObjGetMagicSearch(lpObj, MagicNumber);

		if ( !lpMagic )
			return;

		if ( lpMagic->m_Skill == AT_SKILL_RECALL_PARTY )
		{
			if ( g_NewPVP.IsDuel(*lpObj) || g_NewPVP.IsDuel(*lpTargetObj) )
			{
				return;
			}
		}

		if ( MagicDamageC.SkillGetRequireClass(lpObj->Class, lpObj->m_PlayerData->ChangeUP, lpMagic->m_Skill) < 1 )
			return;

		if ( MagicDamageC.CheckStatus(lpMagic->m_Skill, lpObj->m_PlayerData->GuildStatus) == 0 )
		{
			g_Log.AddC(TColor::Red,  "[0x19] CGMagicAttack() - Invalid Status");
			return;
		}

		if ( MagicDamageC.CheckKillCount(lpMagic->m_Skill, lpObj->m_btKillCount) < 0 )
		{
			g_Log.AddC(TColor::Red,  "[0x19] CGMagicAttack() - Invalid KillCount");
			return;
		}

		if (lpObj->Teleport && gObjUseSkill.m_SkillData.EnableWizardSkillWhileTeleport == false)
		{
			return;
		}
	}
	else
	{
		lpMagic = gObjGetMagic(lpObj, MagicNumber);

		if ( !lpMagic )
			return;
	}

	if ( lpMagic->m_Skill != 24 && gObjUseSkill.SpeedHackCheck(aIndex) == FALSE )
		return;

	if ( !gCheckSkillDistance(aIndex, usernumber, lpMagic->m_Skill))
	{
		DWORD dwTick = GetTickCount() - lpObj->m_dwSkillDistanceErrorTick;
		
		if ( dwTick > (g_iSkillDiatanceKickCheckTime*1000) )
		{
			lpObj->m_iSkillDistanceErrorCount = 0;
			lpObj->m_dwSkillDistanceErrorTick = GetTickCount();
		}

		lpObj->m_iSkillDistanceErrorCount++;

		if ( lpObj->m_iSkillDistanceErrorCount > g_iSkillDistanceKickCount )
		{
			if ( g_iSkillDistanceKick )
			{
				g_Log.Add("[SKILL DISTANCE CHECK] [%s][%s] Kick Invalid Skill Area User. count(%d)",
					lpObj->AccountID, lpObj->Name, lpObj->m_iSkillDistanceErrorCount);

				this->GCSendDisableReconnect(lpObj->m_Index);
				//IOCP.CloseClient(lpObj->m_Index);
				return;
			}

			lpObj->m_iSkillDistanceErrorCount=0;
		}

		return;
	}
	
	gObjUseSkill.UseSkill(aIndex, usernumber, lpMagic);
}

void GameProtocol::CGRageAttack(LPBYTE lpRecv, int aIndex)
{
	LPOBJ lpObj;
	LPOBJ lpTargetObj;

	PMSG_RAGEATTACK_REQ * lpMsg = (PMSG_RAGEATTACK_REQ *)(lpRecv);

	WORD MagicNumber = MAKE_NUMBERW(lpMsg->MagicNumberH, lpMsg->MagicNumberL);
	int usernumber = MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);
	CMagicInf * lpMagic;

	lpObj = &gObj[aIndex];
	lpTargetObj = &gObj[usernumber];

	if ( lpObj->Type != OBJ_USER )
	{
		return;
	}

	if (lpObj->Teleport && gObjUseSkill.m_SkillData.EnableWizardSkillWhileTeleport == false)
	{
		return;
	}

	if ( gObj[aIndex].CloseCount >= 0 )
	{
		return;
	}

	if ( MagicNumber == 263 || MagicNumber == 559 || MagicNumber == 563 )
	{
		this->GCMonkMagicAttack(&gObj[aIndex], MagicNumber, gObj[aIndex].m_PlayerData->DarkSideTarget[0], TRUE);
		return;
	}

	if ( usernumber < 0 || usernumber > g_ConfigRead.server.GetObjectMax()-1 )
	{
		g_Log.Add("error :%s %d %d", __FILE__, __LINE__, usernumber);
		return;
	}

	if ( gObj[usernumber].Live == 0 || gObj[usernumber].Connected  < PLAYER_LOGGED )
		return;

	if ( lpObj->Type == OBJ_USER )
	{
		lpMagic = gObjGetMagicSearch(lpObj, MagicNumber);

		if ( !lpMagic )
			return;

		if ( lpMagic->m_Skill == AT_SKILL_RECALL_PARTY )
		{
			if ( g_NewPVP.IsDuel(*lpObj) || g_NewPVP.IsDuel(*lpTargetObj) )
			{
				return;
			}
		}

		if ( MagicDamageC.SkillGetRequireClass(lpObj->Class, lpObj->m_PlayerData->ChangeUP, lpMagic->m_Skill) < 1 )
			return;

		if ( MagicDamageC.CheckStatus(lpMagic->m_Skill, lpObj->m_PlayerData->GuildStatus) == 0 )
		{
			g_Log.AddC(TColor::Red,  "[0x4A] CGRageAttack() - Invalid Status");
			return;
		}

		if ( MagicDamageC.CheckKillCount(lpMagic->m_Skill, lpObj->m_btKillCount) < 0 )
		{
			g_Log.AddC(TColor::Red,  "[0x4A] CGRageAttack() - Invalid KillCount");
			return;
		}

		if ( lpObj->m_PlayerData->m_bSecurityCheck == false )
		{
			return;
		}
	}

	if ( !gCheckSkillDistance(aIndex, usernumber, lpMagic->m_Skill))
	{
		DWORD dwTick = GetTickCount() - lpObj->m_dwSkillDistanceErrorTick;
		
		if ( dwTick > (g_iSkillDiatanceKickCheckTime*1000) )
		{
			lpObj->m_iSkillDistanceErrorCount = 0;
			lpObj->m_dwSkillDistanceErrorTick = GetTickCount();
		}

		lpObj->m_iSkillDistanceErrorCount++;

		if ( lpObj->m_iSkillDistanceErrorCount > g_iSkillDistanceKickCount )
		{
			if ( g_iSkillDistanceKick )
			{
				g_Log.Add("[SKILL DISTANCE CHECK] [%s][%s] Kick Invalid Skill Area User. count(%d)",
					lpObj->AccountID, lpObj->Name, lpObj->m_iSkillDistanceErrorCount);

				this->GCSendDisableReconnect(lpObj->m_Index);
				//IOCP.CloseClient(lpObj->m_Index);
				return;
			}

			lpObj->m_iSkillDistanceErrorCount=0;
		}

		return;
	}

	this->GCMonkMagicAttack(lpObj, MagicNumber, usernumber, TRUE);
}

void GameProtocol::CGRageAttackRange(PMSG_RAGE_ATTACK_RANGE *lpMsg, int aIndex)
{
	if ( ObjectMaxRange(aIndex) == FALSE )
	{
		return;
	}

	if ( gObj[aIndex].Type != OBJ_USER )
	{
		return;
	}

	if (gObj[aIndex].Teleport)
	{
		return;
	}

	CMagicInf * lpMagic;
	LPOBJ lpObj = &gObj[aIndex];

	WORD MagicNumber = MAKE_NUMBERW(lpMsg->MagicNumberH, lpMsg->MagicNumberL);
	WORD TargetIndex = MAKE_NUMBERW(lpMsg->TargetIndexH, lpMsg->TargetIndexL);
	WORD Target[5];

	for(int i = 0; i < 5; i++)
	{
		Target[i] = 10000;
	}

	lpMagic = gObjGetMagicSearch(lpObj, MagicNumber);

	if ( lpMagic )
	{
		if ( gObjUseSkill.SkillMonkDarkSideGetTargetIndex(aIndex, TargetIndex, lpMagic, Target) == TRUE)
		{
			PMSG_RAGE_ATTACK_RANGE_ANS pMsg;
			PHeadSetBE((LPBYTE)&pMsg, 0x4B, sizeof(pMsg));

			memcpy(&pMsg.Target, Target, sizeof(Target));
			pMsg.MagicNumber = lpMagic->m_Skill;;

			gObj[aIndex].m_PlayerData->DarkSideCount = 0;

			for(int i = 0; i < 5; i++)
			{
				if ( Target[i] != 10000 )
				{
					gObj[aIndex].m_PlayerData->DarkSideCount++;
				}
			}

			memcpy(gObj[aIndex].m_PlayerData->DarkSideTarget, Target, sizeof(Target));
			IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		}
	}
}

void GameProtocol::GCMagicAttackNumberSend(LPOBJ lpObj, WORD MagicNumber, int usernumber,  unsigned char skillsuccess)
{
	PMSG_MAGICATTACK_RESULT pAttack;

	PHeadSetBE((LPBYTE)&pAttack, 0x19, sizeof(pAttack));
	pAttack.MagicNumberH = SET_NUMBERH(MagicNumber);
	pAttack.MagicNumberL = SET_NUMBERL(MagicNumber);
	pAttack.SourceNumberH = SET_NUMBERH(lpObj->m_Index);
	pAttack.SourceNumberL = SET_NUMBERL(lpObj->m_Index);
	pAttack.TargetNumberH = SET_NUMBERH(usernumber);
	pAttack.TargetNumberL = SET_NUMBERL(usernumber);
	pAttack.TargetNumberH &= 0x7F;
	
	if ( skillsuccess == 1 )
		pAttack.TargetNumberH |= 0x80;
		
	if ( lpObj->Type == OBJ_USER )
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pAttack, pAttack.h.size);

	if ( CC_MAP_RANGE(lpObj->MapNumber) || lpObj->MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL )
	{
		if (g_MasterLevelSkillTreeSystem.CheckMasterLevelSkill(MagicNumber) == true)
		{
			switch (MagicNumber)
			{
				case 356:
				case 403:
				case 413:
				case 417:
				case 420:
					return;
				default:
					break;
			}
		}

		else
		{
			switch (MagicNumber)
			{
				case 16:
				case 26:
				case 27:
				case 28:
				case 48:
					return;
				default:
					break;
			}
		}
	}
	
	MsgSendV2(lpObj, (LPBYTE)&pAttack, pAttack.h.size);
}

void GameProtocol::GCMonkMagicAttack(LPOBJ lpObj, WORD MagicNumber, int usernumber,  unsigned char skillsuccess)
{
	PMSG_RAGEATTACK pAttack;

	PHeadSetBE((LPBYTE)&pAttack, 0x4A, sizeof(pAttack));
	pAttack.MagicNumberH = SET_NUMBERH(MagicNumber);
	pAttack.MagicNumberL = SET_NUMBERL(MagicNumber);
	pAttack.SourceNumberH = SET_NUMBERH(lpObj->m_Index);
	pAttack.SourceNumberL = SET_NUMBERL(lpObj->m_Index);
	pAttack.TargetNumberH = SET_NUMBERH(usernumber);
	pAttack.TargetNumberL = SET_NUMBERL(usernumber);
	pAttack.TargetNumberH &= 0x7F;

	if ( skillsuccess == 1 )
		pAttack.TargetNumberH |= 0x80;		

	if ( lpObj->Type == OBJ_USER )
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pAttack, pAttack.h.size);

	MsgSendV2(lpObj, (LPBYTE)&pAttack, pAttack.h.size);
}

void GameProtocol::GCMagicCancelSend(LPOBJ lpObj, WORD MagicNumber)
{
	PMSG_MAGICCANCEL pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x1B, sizeof(pMsg));
	pMsg.MagicNumberL = SET_NUMBERL(MagicNumber);
	pMsg.MagicNumberH = SET_NUMBERH(MagicNumber);
	pMsg.NumberH = SET_NUMBERH(lpObj->m_Index);
	pMsg.NumberL = SET_NUMBERL(lpObj->m_Index);

	if ( lpObj->Type == OBJ_USER )
		IOCP.DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);
	
	MsgSendV2(lpObj, (UCHAR*)&pMsg, pMsg.h.size);
}

void GameProtocol::GCUseEffectItem(LPOBJ lpObj, int iEffectUseOption, int iOptionType, int iEffectType, int iLeftTime)
{
	PMSG_USEEFFECTITEM pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x2D, sizeof(pMsg));
	pMsg.byEffectOption = iEffectUseOption;
	pMsg.wOptionType = iOptionType;
	pMsg.wEffectType = iEffectType;
	pMsg.iLeftTime = iLeftTime;
	pMsg.btBuffEffectType = 0;
//	pMsg.dwEffectValue = 0;

	if ( lpObj->Type == OBJ_USER )
		IOCP.DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);
}

void GameProtocol::CGMagicCancel(PMSG_MAGICCANCEL * lpMsg, int aIndex)
{
	if ( !ObjectMaxRange(aIndex))
		return;

	LPOBJ lpObj = &gObj[aIndex];

	WORD MagicNumber = MAKE_NUMBERW(lpMsg->MagicNumberL, lpMsg->MagicNumberH);

	switch ( MagicNumber )
	{
		case 77:
			gObjRemoveBuffEffect(lpObj, 6);
			break;
		case 233:
			gObjRemoveBuffEffect(lpObj, 82);
			break;
		case 380:
			gObjRemoveBuffEffect(lpObj, 138);
			break;
		case 383:
			gObjRemoveBuffEffect(lpObj, 139);
			break;
		case 441:
			gObjRemoveBuffEffect(lpObj, 143);
			break;
	}
}

struct PMSG_USE_MONSTERSKILL
{
	PBMSG_HEAD2 h;	// C1:69
	WORD btMonsterSkillNumber;	// 3
	WORD wObjIndex;	// 4
	WORD wTargetObjIndex;	// 6
};

void GameProtocol::GCUseMonsterSkillSend(LPOBJ lpObj, LPOBJ lpTargetObj, int iSkillNumber)
{	
	PMSG_USE_MONSTERSKILL pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0x69, 0x00, sizeof(pMsg));
	pMsg.btMonsterSkillNumber = iSkillNumber;
	pMsg.wObjIndex = lpObj->m_Index;
	pMsg.wTargetObjIndex = lpTargetObj->m_Index;

	if ( lpObj->Type == OBJ_USER )
		IOCP.DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);

	MsgSendV2(lpObj, (UCHAR*)&pMsg, pMsg.h.size);
}
	

struct PMSG_VIEWSKILLSTATE
{
	PBMSG_HEAD h;	// C1:07
	BYTE State;	// 3
	BYTE NumberH;	// 4
	BYTE NumberL;	// 5
	BYTE BuffEffect; // 6
};

void GameProtocol::GCStateInfoSend(LPOBJ lpObj, BYTE state, int BuffViewport)
{
	PMSG_VIEWSKILLSTATE pMsg;

	BYTE BuffCount = 0;

	PHeadSetB((LPBYTE)&pMsg, 0x07, sizeof(pMsg));

	pMsg.State = state;
	
	pMsg.NumberH = HIBYTE(lpObj->m_Index);
	pMsg.NumberL = LOBYTE(lpObj->m_Index);

	pMsg.BuffEffect = BuffViewport;
	
	if( lpObj->Type == OBJ_USER )
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);

	if( CC_MAP_RANGE(lpObj->MapNumber) || lpObj->MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL )	return;

	MsgSendV2(lpObj, (LPBYTE)&pMsg, pMsg.h.size);
}

void GameProtocol::CGTeleportRecv(PMSG_TELEPORT* lpMsg, int aIndex)
{
	if ( !ObjectMaxRange(aIndex))
		return;

	if ( gObjCheckUsedBuffEffect(&gObj[aIndex], BUFFTYPE_STUN) == TRUE || gObjCheckUsedBuffEffect(&gObj[aIndex], BUFFTYPE_SLEEP) == TRUE 
		|| gObjCheckUsedBuffEffect(&gObj[aIndex], BUFFTYPE_FREEZE_2) || gObjCheckUsedBuffEffect(&gObj[aIndex], BUFFTYPE_EARTH_BINDS) )
	{
		PMSG_TELEPORT_RESULT pTeleportResult;

		pTeleportResult.h.c = 0xC3;
		pTeleportResult.h.size = sizeof(pTeleportResult);
		pTeleportResult.h.headcode = 0x1C;
		pTeleportResult.Type = 0;
		pTeleportResult.Unk = 0;
		pTeleportResult.MapNumber = gObj[aIndex].MapNumber;
		pTeleportResult.MapX = gObj[aIndex].X;
		pTeleportResult.MapY = gObj[aIndex].Y;
		pTeleportResult.Dir = gObj[aIndex].Dir;

		IOCP.DataSend(aIndex, (LPBYTE)&pTeleportResult, pTeleportResult.h.size);
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->m_bMapSvrMoveReq == true || lpObj->m_bMapSvrMoveReq_2 == true || lpObj->m_bMapSvrMoveQuit == true || lpObj->m_State == 32)
	{
		g_Log.Add("[CGTeleportRecv] MapServerMove User. Can't Move. return!! [%s], IP [%s] ",
			lpObj->AccountID, lpObj->m_PlayerData->Ip_addr);
		return;
	}

	if (IT_MAP_RANGE(gObj[aIndex].MapNumber))
	{
		if (g_IT_Event.GetIllusionTempleState(gObj[aIndex].MapNumber) == 2)
		{
			if (g_IT_Event.CheckRelics(aIndex) != FALSE)
			{
				PMSG_TELEPORT_RESULT pTeleportResult;

				pTeleportResult.h.c = 0xC3;
				pTeleportResult.h.size = sizeof(pTeleportResult);
				pTeleportResult.h.headcode = 0x1C;
				pTeleportResult.Type = 0;
				pTeleportResult.Unk = 0;
				pTeleportResult.MapNumber = gObj[aIndex].MapNumber;
				pTeleportResult.MapX = gObj[aIndex].X;
				pTeleportResult.MapY = gObj[aIndex].Y;
				pTeleportResult.Dir = gObj[aIndex].Dir;

				IOCP.DataSend(aIndex, (LPBYTE)&pTeleportResult, pTeleportResult.h.size);
				return;
			}
		
			if (g_IT_Event.CheckSkillRestraint(gObj[aIndex].m_nITR_Index, gObj[aIndex].MapNumber) != FALSE)
			{
				PMSG_TELEPORT_RESULT pTeleportResult;

				pTeleportResult.h.c = 0xC3;
				pTeleportResult.h.size = sizeof(pTeleportResult);
				pTeleportResult.h.headcode = 0x1C;
				pTeleportResult.Type = 0;
				pTeleportResult.Unk = 0;
				pTeleportResult.MapNumber = gObj[aIndex].MapNumber;
				pTeleportResult.MapX = gObj[aIndex].X;
				pTeleportResult.MapY = gObj[aIndex].Y;
				pTeleportResult.Dir = gObj[aIndex].Dir;
						
				IOCP.DataSend(aIndex, (LPBYTE)&pTeleportResult, pTeleportResult.h.size);
				return;
			}
		}
	}

	if (IT_MAP_RANGE(gObj[aIndex].MapNumber))
	{
		if (g_IT_Event.GetIllusionTempleState(gObj[aIndex].MapNumber) == 1)
		{
			PMSG_TELEPORT_RESULT pTeleportResult;
			
			pTeleportResult.h.c = 0xC3;
			pTeleportResult.h.size = sizeof(pTeleportResult);
			pTeleportResult.h.headcode = 0x1C;
			pTeleportResult.Type = 0;
			pTeleportResult.Unk = 0;
			pTeleportResult.MapNumber = gObj[aIndex].MapNumber;
			pTeleportResult.MapX = gObj[aIndex].X;
			pTeleportResult.MapY = gObj[aIndex].Y;
			pTeleportResult.Dir = gObj[aIndex].Dir;

			IOCP.DataSend(aIndex, (LPBYTE)&pTeleportResult, pTeleportResult.h.size);
			return;
		}
	}

	if(CC_MAP_RANGE(gObj[aIndex].MapNumber) != FALSE) //season 4.0 add-on (fix)
	{
		if(g_ChaosCastle.GetCurrentState(gObj[aIndex].m_cChaosCastleIndex) == 3)
		{
			PMSG_TELEPORT_RESULT pTeleportResult;

			pTeleportResult.h.c = 0xC3;
			pTeleportResult.h.size = sizeof(pTeleportResult);
			pTeleportResult.h.headcode = 0x1C;
			pTeleportResult.Type = 0;
			pTeleportResult.Unk = 0;
			pTeleportResult.MapNumber = gObj[aIndex].MapNumber;
			pTeleportResult.MapX = gObj[aIndex].X;
			pTeleportResult.MapY = gObj[aIndex].Y;
			pTeleportResult.Dir = gObj[aIndex].Dir;
			IOCP.DataSend(aIndex, (LPBYTE)&pTeleportResult, pTeleportResult.h.size);
			return;
		}
	}

	if (gObj[aIndex].MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL && g_ConfigRead.server.GetServerType() == SERVER_BATTLECORE)
	{
		if (g_ChaosCastleFinal.GetCurrentState() == 3)
		{
			PMSG_TELEPORT_RESULT pTeleportResult;

			pTeleportResult.h.c = 0xC3;
			pTeleportResult.h.size = sizeof(pTeleportResult);
			pTeleportResult.h.headcode = 0x1C;
			pTeleportResult.Type = 0;
			pTeleportResult.Unk = 0;
			pTeleportResult.MapNumber = gObj[aIndex].MapNumber;
			pTeleportResult.MapX = gObj[aIndex].X;
			pTeleportResult.MapY = gObj[aIndex].Y;
			pTeleportResult.Dir = gObj[aIndex].Dir;
			IOCP.DataSend(aIndex, (LPBYTE)&pTeleportResult, pTeleportResult.h.size);
			return;
		}
	}

	if(DG_MAP_RANGE(gObj[aIndex].MapNumber) != FALSE) 
	{
		if(g_DoppelGanger.GetDoppelgangerState() != 2)
		{
			PMSG_TELEPORT_RESULT pTeleportResult;

			pTeleportResult.h.c = 0xC3;
			pTeleportResult.h.size = sizeof(pTeleportResult);
			pTeleportResult.h.headcode = 0x1C;
			pTeleportResult.Type = 0;
			pTeleportResult.Unk = 0;
			pTeleportResult.MapNumber = gObj[aIndex].MapNumber;
			pTeleportResult.MapX = gObj[aIndex].X;
			pTeleportResult.MapY = gObj[aIndex].Y;
			pTeleportResult.Dir = gObj[aIndex].Dir;
			IOCP.DataSend(aIndex, (LPBYTE)&pTeleportResult, pTeleportResult.h.size);
			return;
		}
	}

	if ( lpMsg->MoveNumber == 0 )
	{
		BYTE x = lpMsg->MapX;
		BYTE y = lpMsg->MapY;

		if ( gObjCheckTeleportArea(aIndex, x, y) == FALSE )
		{
			g_Log.AddC(TColor::Red,  "[%s][%s] Try Teleport Not Move Area [%d,%d]",
				gObj[aIndex].AccountID, gObj[aIndex].Name,
				x, y);

			PMSG_TELEPORT_RESULT pTeleportResult;

			pTeleportResult.h.c = 0xC3;
			pTeleportResult.h.size = sizeof(pTeleportResult);
			pTeleportResult.h.headcode = 0x1C;
			pTeleportResult.Type = 0;
			pTeleportResult.Unk = 0;
			pTeleportResult.MapNumber = gObj[aIndex].MapNumber;
			pTeleportResult.MapX = gObj[aIndex].X;
			pTeleportResult.MapY = gObj[aIndex].Y;
			pTeleportResult.Dir = gObj[aIndex].Dir;

			IOCP.DataSend(aIndex, (LPBYTE)&pTeleportResult, pTeleportResult.h.size);
			return;
		}

		if (g_ConfigRead.server.GetServerType() == SERVER_CASTLE)
		{
			if (gObj[aIndex].MapNumber == MAP_INDEX_CASTLESIEGE)
			{
				if (g_CastleSiege.CheckTeleportMagicAxisY(gObj[aIndex].Y, x, y) == FALSE)
				{
					y = gObj[aIndex].Y;
				}
			}
		}

		CMagicInf * lpMagic;
		PMSG_MAGICATTACK_RESULT pAttack;
		WORD MagicNumber = 6;
		PHeadSetBE((LPBYTE)&pAttack, 0x19, sizeof(pAttack));
		pAttack.MagicNumberH = SET_NUMBERH(MagicNumber);
		pAttack.MagicNumberL = SET_NUMBERL(MagicNumber);
		pAttack.SourceNumberH = SET_NUMBERH(aIndex);
		pAttack.SourceNumberL = SET_NUMBERL(aIndex);
		pAttack.TargetNumberH = SET_NUMBERH(aIndex);
		pAttack.TargetNumberL = SET_NUMBERL(aIndex);

		lpMagic = gObjGetMagicSearch(&gObj[aIndex], 6);

		if ( lpMagic )
		{
			int usemana = gObjMagicManaUse(&gObj[aIndex], lpMagic);
			
			if ( usemana >= 0 )
			{
				int usebp = gObjMagicBPUse(&gObj[aIndex], lpMagic);
			
				if ( usebp >= 0 )
				{
					if ( gObj[aIndex].Type == OBJ_USER )
						IOCP.DataSend(aIndex, (LPBYTE)&pAttack, pAttack.h.size);

					MsgSendV2(&gObj[aIndex], (LPBYTE)&pAttack, pAttack.h.size);
					gObjTeleportMagicUse(aIndex, x, y);
					gObj[aIndex].Mana = usemana;
					gObj[aIndex].BP = usebp;
					GCManaSend(gObj[aIndex].m_Index, gObj[aIndex].Mana, -1, 0, gObj[aIndex].BP);
				}
			}
		}
	}

	if ( IMPERIAL_MAP_RANGE(gObj[aIndex].MapNumber) == TRUE )
	{
		int nCurrentZoneIndex = g_ImperialGuardian.GetCurrentZoneIndex(aIndex);

		if ( nCurrentZoneIndex >= 0 )
		{
			g_ImperialGuardian.CGEnterPortal(aIndex, nCurrentZoneIndex+1);
		}
	}

	if (lpObj->m_IfState.use == 1 && lpObj->m_IfState.type == 6)
	{
		return;
	}

	if ( gGateC.IsInGate(aIndex, lpMsg->MoveNumber) )
	{
		gObjMoveGate(aIndex, lpMsg->MoveNumber);
		return;
	}

	gObjClearViewport(&gObj[aIndex]);

	if (g_ConfigRead.server.GetServerType() == SERVER_CASTLE)
	{
		if (gObj[aIndex].MapNumber == MAP_INDEX_CASTLESIEGE)
		{
			if (g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
			{
				g_CastleSiege.NotifySelfCsJoinSide(aIndex);
				g_CastleSiege.NotifyCsSelfLeftTime(aIndex);
			}
		}
	}
}

void GameProtocol::CGReqMapMove(PMSG_REQ_MAPMOVE *lpMsg, int iIndex)
{
	if ( !ObjectMaxRange(iIndex))
		return;

	LPOBJ lpObj = &gObj[iIndex];

	PMSG_ANS_MAPMOVE pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0x8E, 0x03, sizeof(pMsg));

	pMsg.btResult = 0;

	if (lpObj->Type != OBJ_USER)
	{
		IOCP.DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	if (g_KeyGenerater.CheckKeyValue(lpObj->m_PlayerData->m_dwMapMoveKeyValue, lpMsg->dwBlockKey) == FALSE)
	{
		pMsg.btResult = 12;
		IOCP.DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	if (lpObj->Connected < PLAYER_PLAYING)
	{
		pMsg.btResult = 11;
		IOCP.DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	if (lpObj->Teleport != 0)
	{
		pMsg.btResult = 2;
		IOCP.DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	if (lpObj->m_PlayerData->m_bSecurityCheck == false)
	{
		IOCP.DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	if (lpObj->m_bMapSvrMoveReq == true || lpObj->m_bMapSvrMoveReq_2 == true || lpObj->m_bMapSvrMoveQuit == true || lpObj->m_State == 32)
	{
		g_Log.Add("[CGReqMapMove] MapServerMove User. Can't Move. return!! [%s], IP [%s] ",
			lpObj->AccountID, lpObj->m_PlayerData->Ip_addr);
		return;
	}

	if (lpObj->m_bPShopOpen == true)
	{
		if (lpObj->MapNumber == MAP_INDEX_CASTLESIEGE && lpMsg->wMapIndex == MAP_INDEX_RORENCIA)
		{
			pMsg.btResult = 3;
			IOCP.DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);
			return;
		}

		if (lpObj->MapNumber == MAP_INDEX_RORENCIA && lpMsg->wMapIndex == MAP_INDEX_CASTLESIEGE)
		{
			pMsg.btResult = 3;
			IOCP.DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);
			return;
		}
	}

	if (lpObj->m_IfState.use == 1 && lpObj->m_IfState.type == 6)
	{
		pMsg.btResult = 2;
		IOCP.DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	if (lpObj->DieRegen == TRUE)
	{
		pMsg.btResult = 2;
		IOCP.DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	if (lpObj->RegenOk != FALSE)
	{
		pMsg.btResult = 2;
		IOCP.DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	if (pMsg.btResult)
	{
		IOCP.DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	IOCP.DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);
	gMoveCommand.Move(lpObj, lpMsg->wMapIndex);
}

void GameProtocol::GCSendMapMoveCheckSum(LPOBJ lpObj)
{
	PMSG_MAPMOVE_CHECKSUM pMsg;

	pMsg.dwKeyValue = lpObj->m_PlayerData->m_dwMapMoveKeyValue;
	PHeadSubSetB((LPBYTE)&pMsg, 0x8E, 0x01, sizeof(pMsg));

	IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, pMsg.h.size);
}

void GameProtocol::CGTargetTeleportRecv(PMSG_TARGET_TELEPORT * lpMsg, int aIndex)
{
	if ( !ObjectMaxRange(aIndex))
		return;

	if ( gObjCheckUsedBuffEffect(&gObj[aIndex], BUFFTYPE_STONE) || gObjCheckUsedBuffEffect(&gObj[aIndex], BUFFTYPE_STUN) || gObjCheckUsedBuffEffect(&gObj[aIndex], BUFFTYPE_SLEEP) ||
		gObjCheckUsedBuffEffect(&gObj[aIndex], BUFFTYPE_FREEZE_2) || gObjCheckUsedBuffEffect(&gObj[aIndex], BUFFTYPE_EARTH_BINDS))
		return;

	CMagicInf * lpMagic;
	BYTE x;
	BYTE y;
	x = lpMsg->MapX;
	y = lpMsg->MapY;
	int iTargetIndex = MAKE_NUMBERW(lpMsg->NumberL, lpMsg->NumberH);	// #error? parameters inverse?

	if ( !gObjIsConnectedGP(iTargetIndex) )
	{
		g_Log.AddC(TColor::Red,  "[%s][%s] Try Target Teleport Not Move Area [%d,%d]",
			gObj[aIndex].AccountID, gObj[aIndex].Name, x, y);

		return;
	}

	if ( IT_MAP_RANGE(gObj[iTargetIndex].MapNumber))
	{
		if (g_IT_Event.CheckRelics(iTargetIndex))
		{
			return;
		}
	}

	if ( gObj[aIndex].PartyNumber != gObj[iTargetIndex].PartyNumber ||
		 gObj[aIndex].PartyNumber == -1 ||
		 gObj[iTargetIndex].PartyNumber == -1)
	{
		return;
	}

	if ( !gObjCheckTeleportArea(iTargetIndex, x, y))
	{
		g_Log.AddC(TColor::Red,  "[%s][%s] Try Target Teleport Not Move Area [%d,%d]",
			gObj[aIndex].AccountID, gObj[aIndex].Name, x, y);

		return;
	}

	PMSG_MAGICATTACK_RESULT pAttack;

	PHeadSetBE((LPBYTE)&pAttack, 0x19, sizeof(pAttack));
	WORD MagicNumber = AT_SKILL_TARGET_TELEPORT;
	pAttack.MagicNumberH = SET_NUMBERH(MagicNumber);
	pAttack.MagicNumberL = SET_NUMBERL(MagicNumber);
	pAttack.SourceNumberH = SET_NUMBERH(aIndex);
	pAttack.SourceNumberL = SET_NUMBERL(aIndex);
	pAttack.TargetNumberH = SET_NUMBERH(iTargetIndex);	// #error? parameters inverse?
	pAttack.TargetNumberL = SET_NUMBERL(iTargetIndex);	// #error? parameters inverse?
	lpMagic = gObjGetMagicSearch(&gObj[aIndex], AT_SKILL_TARGET_TELEPORT);

	if ( lpMagic )
	{
		int usemana = gObjMagicManaUse(&gObj[aIndex], lpMagic);

		if ( usemana >= 0 )
		{
			int usebp = gObjMagicBPUse(&gObj[aIndex], lpMagic);
			
			if ( usebp >= 0 )
			{
				if ( gObj[aIndex].Type == OBJ_USER )
					IOCP.DataSend(aIndex, (LPBYTE)&pAttack, pAttack.h.size);

				MsgSendV2(&gObj[aIndex], (LPBYTE)&pAttack, pAttack.h.size);
				gObjTeleportMagicUse(iTargetIndex, x, y);
				gObj[aIndex].Mana = usemana;
				gObj[aIndex].BP = usebp;
				GCManaSend(gObj[aIndex].m_Index, gObj[aIndex].Mana, -1, 0, gObj[aIndex].BP);
				
				return;
			}
		}
	}
}




void GameProtocol::GCTeleportSend(LPOBJ lpObj, WORD MoveNumber, BYTE MapNumber, BYTE MapX, BYTE MapY, BYTE Dir)
{
	PMSG_TELEPORT_RESULT pMsg;

	if ( lpObj->Type != OBJ_USER )
		return;

	pMsg.h.c = 0xC3;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0x1C;
	if(MoveNumber > 0)
	{
		pMsg.Type = 256; // Warp
	}
	else
	{
		pMsg.Type = 0; // Teleportacja
	}
	pMsg.MapNumber = MapNumber;
	pMsg.Unk = 0;
	pMsg.MapX = MapX;
	pMsg.MapY = MapY;
	pMsg.Dir = Dir;
	// mapserver

	if ( MoveNumber == 0 )
		IOCP.DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);
	else
		IOCP.DataSend(lpObj->m_Index, (UCHAR*)&pMsg, pMsg.h.size);
}

void GameProtocol::CGBeattackRecv(unsigned char* lpRecv, int aIndex, int magic_send)
{
	if (aIndex < 0 || aIndex > g_ConfigRead.server.GetObjectMax() - 1)
	{
		return;
	}

	// Set the lpRecv to a Predeterminated Packet
	PMSG_BEATTACK_COUNT * lpCount = (PMSG_BEATTACK_COUNT *)lpRecv;

	// Check the Protocol
	if (lpCount->h.headcode != 0xDF)
	{
		g_Log.Add("error-L3 %s %d", __FILE__, __LINE__);
		return;
	}

	// Check if the count is leess than 1
	if (lpCount->Count < 1)
	{
		g_Log.Add("error-L3 %s %d", __FILE__, __LINE__);
		return;
	}

	// Set the Max limitation to 5
	if (lpCount->Count > 5)
	{
		lpCount->Count = 5;
	}

	// Set the Start of the Offset
	int lOfs = sizeof(PMSG_BEATTACK_COUNT);
	CMagicInf * lpMagic;
	int tNumber;
	PMSG_BEATTACK * lpMsg;

	// Set lpMagic according
	// if aIndex is OBJ_USER
	// or if it is OBJ_MONSTER
	if (gObj[aIndex].Type == OBJ_USER)
	{
		BYTE attr = MapC[gObj[aIndex].MapNumber].GetAttr(gObj[aIndex].X, gObj[aIndex].Y);

		if ((attr & 1) == 1) //safezone
		{
			gObjSetPosition(aIndex, gObj[aIndex].X, gObj[aIndex].Y);
			return;
		}
		//season 4.5 add-on end

		WORD MagicNumber = MAKE_NUMBERW(lpCount->MagicNumberH, lpCount->MagicNumberL);

		lpMagic = gObjGetMagicSearch(&gObj[aIndex], MagicNumber);
	}
	else
	{
		WORD MagicNumber = MAKE_NUMBERW(lpCount->MagicNumberH, lpCount->MagicNumberL);

		lpMagic = gObjGetMagic(&gObj[aIndex], MagicNumber);
	}

	if (gObj[aIndex].Type == OBJ_USER && gObj[aIndex].m_PlayerData->m_bSecurityCheck == false)
		return;

	if (gObj[aIndex].Teleport)
		return;

	// Check if there is Magic
	if (lpMagic == NULL)
	{
		return;
	}

	// Avoid use Skill of the following types
	if (lpMagic->m_Skill == 42 ||
		lpMagic->m_Skill == 43 ||
		lpMagic->m_Skill == 41 ||
		g_MasterLevelSkillTreeSystem.GetBaseMasterLevelSkill(lpMagic->m_Skill) == 336 ||
		g_MasterLevelSkillTreeSystem.GetBaseMasterLevelSkill(lpMagic->m_Skill) == 331 ||
		lpMagic->m_Skill == 333 ||
		lpMagic->m_Skill == 339 ||
		lpMagic->m_Skill == 342 ||
		lpMagic->m_Skill == 332 ||
		lpMagic->m_Skill == 330)
	{
		return;
	}

	if (gObj[aIndex].Type == OBJ_USER)
	{
		// Check if Player can use Magic
		// Acording to its Class
		if (MagicDamageC.SkillGetRequireClass(gObj[aIndex].Class,
			gObj[aIndex].m_PlayerData->ChangeUP,
			lpMagic->m_Skill) < 1)
		{
			// It's not his magic [%s][%s] .
			g_Log.AddC(TColor::Red, "It is not his magic [%s][%s][%d]",
				gObj[aIndex].AccountID,
				gObj[aIndex].Name,
				lpMagic->m_Skill);

			gObjUserKill(aIndex);
			return;
		}

		// Check the time cast Spell
		// No longer that 8 seconds
		if ((GetTickCount() - gObj[aIndex].UseMagicTime) > 8000)
		{
			g_Log.AddC(TColor::Red, "Too long time passed after casting magic [%s][%s] (%d)(%d)",
				gObj[aIndex].AccountID,
				gObj[aIndex].Name,
				lpMagic->m_Skill,
				GetTickCount() - gObj[aIndex].UseMagicTime);

			return;
		}

		// Check if user cast to much Spells
		// Npo longer thant 4
		if (gObj[aIndex].UseMagicCount > 4)
		{
			return;
		}

		gObj[aIndex].UseMagicCount++;
	}

	// Avoid Attack out of the predeterminated
	// Magic need to be one of the followingla
	//	
	//	8	"Twister"
	//	9	"Evil Spirit"
	//	10	"Hellfire"
	//	13	"Blast"
	//	14	"Inferno"
	//	5	"Flame"
	//	24	"Triple Shot"
	//	50	"Flame of Evil(Monster)"
	//	12	"Aquaflash"
	//	41	"Twisting Slash"
	//	47	"Impale"
	//	43	"Death Stab"
	//	42	"Rageful Blow"
	//	52	"Penetration"
	//	55	"Fire Slash"
	//	78	"#Unknown"
	//  Master Triple Shot
	//  Master CometFall
	//  Master FireSlash
	//  235 "Five Shot"

	if (g_MasterLevelSkillTreeSystem.CheckMasterLevelSkill(lpMagic->m_Skill))
	{
		int iBaseMLS = g_MasterLevelSkillTreeSystem.GetBaseMasterLevelSkill(lpMagic->m_Skill);

		if (iBaseMLS != 403
			&& iBaseMLS != 388
			&& iBaseMLS != 330
			&& iBaseMLS != 413
			&& iBaseMLS != 414
			&& iBaseMLS != 481
			&& iBaseMLS != 356
			&& iBaseMLS != 382
			&& iBaseMLS != 512
			&& iBaseMLS != 508
			&& iBaseMLS != 385
			&& iBaseMLS != 391
			&& iBaseMLS != 336
			&& iBaseMLS != 331
			&& iBaseMLS != 417
			&& iBaseMLS != 420
			&& iBaseMLS != 490
			&& iBaseMLS != 482
			&& iBaseMLS != 518
			&& iBaseMLS != 487
			&& iBaseMLS != 326
			&& iBaseMLS != 327
			&& iBaseMLS != 328
			&& iBaseMLS != 329
			&& iBaseMLS != 332
			&& iBaseMLS != 333
			&& iBaseMLS != 337
			&& iBaseMLS != 339
			&& iBaseMLS != 340
			&& iBaseMLS != 342
			&& iBaseMLS != 343
			&& iBaseMLS != 344
			&& iBaseMLS != 346
			&& iBaseMLS != 360
			&& iBaseMLS != 363
			&& iBaseMLS != 378
			&& iBaseMLS != 379
			&& iBaseMLS != 380
			&& iBaseMLS != 381
			&& iBaseMLS != 383
			&& iBaseMLS != 384
			&& iBaseMLS != 386
			&& iBaseMLS != 387
			&& iBaseMLS != 389
			&& iBaseMLS != 392
			&& iBaseMLS != 393
			&& iBaseMLS != 404
			&& iBaseMLS != 406
			&& iBaseMLS != 415
			&& iBaseMLS != 416
			&& iBaseMLS != 418
			&& iBaseMLS != 419
			&& iBaseMLS != 425
			&& iBaseMLS != 426
			&& iBaseMLS != 427
			&& iBaseMLS != 428
			&& iBaseMLS != 429
			&& iBaseMLS != 430
			&& iBaseMLS != 432
			&& iBaseMLS != 433
			&& iBaseMLS != 441
			&& iBaseMLS != 459
			&& iBaseMLS != 460
			&& iBaseMLS != 461
			&& iBaseMLS != 462
			&& iBaseMLS != 466
			&& iBaseMLS != 468
			&& iBaseMLS != 469
			&& iBaseMLS != 470
			&& iBaseMLS != 472
			&& iBaseMLS != 479
			&& iBaseMLS != 480
			&& iBaseMLS != 483
			&& iBaseMLS != 484
			&& iBaseMLS != 486
			&& iBaseMLS != 488
			&& iBaseMLS != 489
			&& iBaseMLS != 492
			&& iBaseMLS != 493
			&& iBaseMLS != 494
			&& iBaseMLS != 495
			&& iBaseMLS != 496
			&& iBaseMLS != 497
			&& iBaseMLS != 509
			&& iBaseMLS != 510
			&& iBaseMLS != 514
			&& iBaseMLS != 515
			&& iBaseMLS != 516
			&& iBaseMLS != 519
			&& iBaseMLS != 520
			&& iBaseMLS != 523
			&& iBaseMLS != 539
			&& iBaseMLS != 551
			&& iBaseMLS != 552
			&& iBaseMLS != 554
			&& iBaseMLS != 555
			&& iBaseMLS != 556
			&& iBaseMLS != 558
			&& iBaseMLS != 559
			&& iBaseMLS != 560
			&& iBaseMLS != 561
			&& iBaseMLS != 562
			&& iBaseMLS != 563
			&& iBaseMLS != 564
			&& iBaseMLS != 565
			&& iBaseMLS != 566
			&& iBaseMLS != 567
			&& iBaseMLS != 575
			&& iBaseMLS != 577
			&& iBaseMLS != 411
			&& iBaseMLS != 424
			&& iBaseMLS != 431
			&& iBaseMLS != 463
			&& iBaseMLS != 491
			&& iBaseMLS != 522
			&& iBaseMLS != 521
			&& iBaseMLS != 524
			&& iBaseMLS != 551
			&& iBaseMLS != 552
			&& iBaseMLS != 554
			&& iBaseMLS != 555
			&& iBaseMLS != 556
			&& iBaseMLS != 558
			&& iBaseMLS != 559
			&& iBaseMLS != 560
			&& iBaseMLS != 561
			&& iBaseMLS != 562
			&& iBaseMLS != 563
			&& iBaseMLS != 564
			&& iBaseMLS != 565
			&& iBaseMLS != 566
			&& iBaseMLS != 567
			&& iBaseMLS != 575
			&& iBaseMLS != 577)
		{
			g_Log.Add("error-L3 %s %d, Skill %d, BaseML %d", __FILE__, __LINE__, lpMagic->m_Skill, iBaseMLS);
			return;
		}
	}

	else
	{
		if (lpMagic->m_Skill != 8
			&& lpMagic->m_Skill != 9
			&& lpMagic->m_Skill != 10
			&& lpMagic->m_Skill != 13
			&& lpMagic->m_Skill != 14
			&& lpMagic->m_Skill != 5
			&& lpMagic->m_Skill != 24
			&& lpMagic->m_Skill != 50
			&& lpMagic->m_Skill != 12
			&& lpMagic->m_Skill != 41
			&& lpMagic->m_Skill != 47
			&& lpMagic->m_Skill != 43
			&& lpMagic->m_Skill != 42
			&& lpMagic->m_Skill != 52
			&& lpMagic->m_Skill != 55
			&& lpMagic->m_Skill != 78
			&& lpMagic->m_Skill != 277) // Penetration
		{
			g_Log.Add("Magic Attack Error: Invalid Skill: %d (%s)(%d)", lpMagic->m_Skill, __FILE__, __LINE__);
			return;
		}
	}

	if (lpMagic->m_Skill == 24 || lpMagic->m_Skill == 78 || g_MasterLevelSkillTreeSystem.GetBaseMasterLevelSkill(lpMagic->m_Skill) == 414 || g_MasterLevelSkillTreeSystem.GetBaseMasterLevelSkill(lpMagic->m_Skill) == 518
		|| lpMagic->m_Skill == 418 || lpMagic->m_Skill == 520)
	{
		int NSAttackSerial = lpCount->Serial;

		if (gObj[aIndex].OSAttackSerial >= 255 && (gObj[aIndex].OSAttackSerial - lpCount->Serial) > 50)
		{
			gObj[aIndex].OSAttackSerial = -1;
		}

		if (NSAttackSerial > gObj[aIndex].OSAttackSerial)
		{
			gObj[aIndex].OSAttackSerial = NSAttackSerial;
			LPOBJ lpObj = &gObj[aIndex];
		}
	}

	if (lpMagic->m_Skill == 9 ||
		g_MasterLevelSkillTreeSystem.GetBaseMasterLevelSkill(lpMagic->m_Skill) == 385 ||
		g_MasterLevelSkillTreeSystem.GetBaseMasterLevelSkill(lpMagic->m_Skill) == 487)
	{
		if (gObj[aIndex].Class != CLASS_WIZARD && gObj[aIndex].Class != CLASS_MAGUMSA)
		{
			return;
		}
	}

	if (bIsIgnorePacketSpeedHackDetect)
	{
		LPOBJ lpObj = &gObj[aIndex];

		if (lpObj->m_SpeedHackPenalty > 0)
		{
			lpObj->m_SpeedHackPenalty--;
			return;
		}
	}

	LPOBJ lpObj = &gObj[aIndex];

	int lOfs2 = lOfs;
	int pTargetNumber[128];

	for (int i = 0; i<lpCount->Count; i++)
	{
		lpMsg = (PMSG_BEATTACK *)&lpRecv[lOfs2];
		pTargetNumber[i] = MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);
		lOfs2 += sizeof(PMSG_BEATTACK);

		if (i >= 1)
		{
			int nTargetDistance = gObjCalDistanceTX(&gObj[pTargetNumber[0]], &gObj[pTargetNumber[i]]);

			if (gObj[pTargetNumber[i]].Type == OBJ_MONSTER && nTargetDistance > 10)
			{
				g_Log.Add("User Hack Detected [%s][%s] Skill: %d, Monster Distance: %d, Cnt:%d",
					gObj[aIndex].AccountID, gObj[aIndex].Name, lpMagic->m_Skill, nTargetDistance, lpCount->Count);
			}
		}
	}

	for (int n = 0; n<lpCount->Count; n++)
	{
		lpMsg = (PMSG_BEATTACK *)&lpRecv[lOfs];
		tNumber = MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);
		LPOBJ lpObj = &gObj[aIndex];

		if (!lpMsg->MagicKey && (lpMagic->m_Skill == 9 || g_MasterLevelSkillTreeSystem.GetBaseMasterLevelSkill(lpMagic->m_Skill) == 385 || g_MasterLevelSkillTreeSystem.GetBaseMasterLevelSkill(lpMagic->m_Skill) == 487))	// "Evil Spirit"
		{
			return;
		}

		if (lpMsg->MagicKey && (lpMagic->m_Skill == 9 || g_MasterLevelSkillTreeSystem.GetBaseMasterLevelSkill(lpMagic->m_Skill) == 385 || g_MasterLevelSkillTreeSystem.GetBaseMasterLevelSkill(lpMagic->m_Skill) == 487))	// "Evil Spirit"
		{
			if (lpObj->DurMagicKeyChecker->IsValidDurationTime(lpMsg->MagicKey) == FALSE)
			{
				g_Log.AddC(TColor::Red, "Invalid DurationTime Key = %d ( Time : %d) [%s][%s]",
					lpMsg->MagicKey, lpObj->DurMagicKeyChecker->GetValidDurationTime(lpMsg->MagicKey),
					lpObj->AccountID, lpObj->Name);
				lOfs += sizeof(PMSG_BEATTACK);

				continue;
			}

			if (lpObj->DurMagicKeyChecker->IsValidCount(lpMsg->MagicKey) == FALSE)
			{
				g_Log.AddC(TColor::Red, "Invalid ValidCount = %d ( Count : %d) [%s][%s]",
					lpMsg->MagicKey, lpObj->DurMagicKeyChecker->GetValidCount(lpMsg->MagicKey),
					lpObj->AccountID, lpObj->Name);
				lOfs += sizeof(PMSG_BEATTACK);

				continue;
			}
		}

		if (gWriteSkillLog)
		{
			g_Log.Add("Magic Attack3 : %d, serial = %d, Tgt =  %d, cnt = %d",
				lpMagic->m_Skill, lpCount->Serial, tNumber, lpCount->Count);
		}

		//season4 add-on start
		if (ObjectMaxRange(tNumber) == false)
		{
			continue;
		}

		int DistanceCheck = gObjCalDistance(lpObj, &gObj[tNumber]);

		if (DistanceCheck > 13)
		{
			lOfs += sizeof(PMSG_BEATTACK);
			continue;
		}
		//season4 add-on end

		for (int i = 0; i<lpCount->Count; i++)
		{
			if (n != i)
			{
				if (pTargetNumber[i] == tNumber)
				{
					g_Log.AddC(TColor::Red, "[ERROR] - pTargetNumber[i] == tNumber");
					this->GCSendDisableReconnect(gObj[aIndex].m_Index);
					//IOCP.CloseClient(gObj[aIndex].m_Index);
					return;
				}
			}
		}

		if (tNumber >= 0 && tNumber < g_ConfigRead.server.GetObjectMax() - 1)
		{
			if (lpMagic->m_Skill == 55)
			{
				gObjUseSkill.MaGumSkillDefenseDown(aIndex, tNumber, lpMagic->m_Level);
				gObjAttack(&gObj[aIndex], &gObj[tNumber], lpMagic, TRUE, 1, 0, FALSE, 0, 0);
			}

			else if (lpMagic->m_Skill == 78)
			{
				gObjUseSkill.SkillFireScream(aIndex, tNumber, lpMagic);
			}

			else if (g_MasterLevelSkillTreeSystem.GetBaseMasterLevelSkill(lpMagic->m_Skill) == 490)
			{
				g_MasterLevelSkillTreeSystem.MLS_MaGumSkillDefenseDown(aIndex, tNumber);
				gObjAttack(&gObj[aIndex], &gObj[tNumber], lpMagic, TRUE, 1, 0, FALSE, 0, 0);
			}

			else if (g_MasterLevelSkillTreeSystem.GetBaseMasterLevelSkill(lpMagic->m_Skill) == 493)
			{
				g_MasterLevelSkillTreeSystem.MLS_SkillMasteryDefenseDown(aIndex, tNumber, lpMagic);
				gObjAttack(&gObj[aIndex], &gObj[tNumber], lpMagic, TRUE, 1, 0, FALSE, 0, 0);
			}

			else if (lpMagic->m_Skill == 518 || lpMagic->m_Skill == 520)
			{
				g_MasterLevelSkillTreeSystem.MLS_FireScream(aIndex, tNumber, lpMagic);
			}

			else if (lpMagic->m_Skill == AT_SKILL_SHINING_PEAK)
			{
				gObjUseSkill.SkillShiningPeak(aIndex, lpMagic, tNumber);
			}

			else if (magic_send)
			{
				gObjAttack(&gObj[aIndex], &gObj[tNumber], lpMagic, TRUE, 1, 0, FALSE, 0, 0);
			}
			else
			{
				gObjAttack(&gObj[aIndex], &gObj[tNumber], lpMagic, FALSE, 1, 0, FALSE, 0, 0);
			}
		}

		lOfs += sizeof(PMSG_BEATTACK);
	}
}

void GameProtocol::CGDurationMagicRecv(LPBYTE lpRecv, int aIndex)
{
	if(aIndex < 0 || aIndex > g_ConfigRead.server.GetObjectMax()-1)
	{
		return;
	}

	PMSG_DURATION_MAGIC_RECV * lpMsg = (PMSG_DURATION_MAGIC_RECV *)(lpRecv);

	CMagicInf * lpMagic;
	LPOBJ lpObj = &gObj[aIndex];

	if ( lpObj->Teleport )
	{
		return;
	}

	WORD MagicNumber = MAKE_NUMBERW(lpMsg->MagicNumberH, lpMsg->MagicNumberL);
	if ( gObj[aIndex].Type == OBJ_USER )
	{
		lpMagic = gObjGetMagicSearch(lpObj, MagicNumber);
	}
	else
	{
		lpMagic = gObjGetMagic( lpObj, MagicNumber);
	}

	if ( lpMagic == NULL )
	{
		return;
	}
	
	if ( lpObj->Type == OBJ_USER )
	{
		if ( MagicDamageC.CheckStatus(lpMagic->m_Skill, lpObj->m_PlayerData->GuildStatus) == FALSE )
		{
			g_Log.AddC(TColor::Red,  "[0x1E] CGDurationMagicRecv() - Invalid Status");	// #warning maybe protocol here change
			return;
		}

		if ( MagicDamageC.CheckKillCount(lpMagic->m_Skill, lpObj->m_btKillCount) < 0 )
		{
			g_Log.AddC(TColor::Red,  "[0x1E] CGDurationMagicRecv() - Invalid KillCount");
			return;
		}

		if (lpObj->m_PlayerData->m_bSecurityCheck == false)
		{
			return;
		}
	}

	lpObj->UseMagicNumber = 0;

	if ( lpMagic->m_Skill != 24 )
	{
		if ( !gObjUseSkill.SpeedHackCheck(aIndex))
		{
			return;
		}
	}

	if ( !gObjUseSkill.EnableSkill(lpMagic->m_Skill) )
	{
		return;
	}

	int aTargetIndex = MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);

	gObjUseSkill.UseSkill(aIndex, lpMagic,lpMsg->X, lpMsg->Y, lpMsg->Dir, lpMsg->TargetPos, aTargetIndex);

	if ( lpMsg->MagicKey )
		lpObj->DurMagicKeyChecker->SetDurationTime(lpMsg->MagicKey, GetTickCount());
}

void GameProtocol::CGUseItemRecv(PMSG_USEITEM* lpMsg, int aIndex)
{
	int pos;
	CItem * citem;
	int iItemUseType = lpMsg->btItemUseType;

	if ( gObj[aIndex].Type != OBJ_USER )
	{
		return;
	}

	// Check User States
	if ( gObj[aIndex].m_IfState.use && gObj[aIndex].m_IfState.type != 3 )
	{
		GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);

		g_Log.Add("[%s][%s] CGUseItemRecv()_If return %s %d",
			gObj[aIndex].AccountID,
			gObj[aIndex].Name,
			__FILE__, __LINE__);

		return;
	}

	if ( gObj[aIndex].m_PlayerData->m_bSecurityCheck == false )
	{
		GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);
		MsgOutput(aIndex, Lang.GetText(0,822));
		return;
	}

	if ( (GetTickCount() - gObj[aIndex].m_PlayerData->PotionTime) < g_ConfigRead.data.common.PotionTime )
	{
		GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);
		return;
	}

	gObj[aIndex].m_PlayerData->PotionTime = GetTickCount();

	// Check if User is exiting
	if ( gObj[aIndex].CloseType != -1 )
	{
		GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);

		g_Log.Add("[%s][%s] CGUseItemRecv()_CloseType return %s %d",
			gObj[aIndex].AccountID, 
			gObj[aIndex].Name,
			__FILE__, __LINE__);

		return;
	}

	// Fix Inventory Pointer
	if ( gObjFixInventoryPointer(aIndex) == false )
	{
		g_Log.Add("[Fix Inv.Ptr] False Location - %s, %d",
			__FILE__, __LINE__);
		//return;
	}

	// Check if there is a Transaction
	// Dupe
	if ( gObj[aIndex].pTransaction == 1 )
	{
		g_Log.Add("[%s][%s] CGUseItemRecv() Failed : Transaction == 1, IF_TYPE : %d",
			gObj[aIndex].AccountID,
			gObj[aIndex].Name,
			gObj[aIndex].m_IfState.type);

		return;
	}

	if ( g_NewPVP.IsObserver(gObj[aIndex]) )
	{
		this->GCServerMsgStringSend(Lang.GetText(0,319), aIndex, 1);
		return;
	}

	// Set Var Pos
	pos = lpMsg->inventoryPos;

	// Check if the pos is in bounds
	if ( pos > MAIN_INVENTORY_SIZE-1 )
	{
		// error : Location is over the range. %s/%d
		g_Log.Add("error: Location is over the range %s/%d", __FILE__, __LINE__);

		GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);

		g_Log.Add("[%s][%s] CGUseItemRecv()_Inventory return %s %d",
			gObj[aIndex].AccountID,
			gObj[aIndex].Name,
			__FILE__, __LINE__);

		return;
	}

	// check if the item
	// have the same position
	// as the new position
	if ( lpMsg->inventoryPos == lpMsg->invenrotyTarget )
	{
		GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);

		g_Log.Add("error-L1 : [%s][%s] CGUseItemRecv()_Pos return %s %d",
			gObj[aIndex].AccountID,
			gObj[aIndex].Name,
			__FILE__, __LINE__);

		return;
	}

	// Set Pointer
	citem = &gObj[aIndex].pInventory[pos];

	// The item is ITEM
	if (citem->IsItem())
	{
		// Check if item have 0 as Serial
		if (gObjCheckSerial0ItemList(citem))
		{
			MsgOutput(aIndex, Lang.GetText(0,259));

			GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);

			g_Log.Add("[ANTI-HACK][Serial 0 Item] [UseItem] (%s)(%s) Item(%s) Pos(%d)",
				gObj[aIndex].AccountID,
				gObj[aIndex].Name,
				citem->GetName(),
				pos);

			return;
		}

		// Check the item Serial
		if (gObjInventorySearchSerialNumber(&gObj[aIndex],
			citem->GetNumber()) == FALSE)
		{
			GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);
			g_Log.Add("error-L2 : CopyItem [%s][%s] return %s %d",
				gObj[aIndex].AccountID,
				gObj[aIndex].Name,
				__FILE__, __LINE__);

			return;
		}

		if (citem->m_serial && !gObjCanItemTouch(&gObj[aIndex], 1))
		{
			GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);
			return;
		}

		if (citem->m_Type == ITEMGET(14, 0) || // Apple
			citem->m_Type == ITEMGET(14, 1) ||
			citem->m_Type == ITEMGET(14, 2) ||
			citem->m_Type == ITEMGET(14, 3))
		{
			int tLife = (citem->m_Value * 10) - (gObj[aIndex].Level * 2);	// #formula

			if (tLife < 0)
			{
				tLife = 0;
			}

			int nAddRate = 0;

			switch (citem->m_Type)
			{
			case ITEMGET(14, 0):	// Apple
				nAddRate = 10;
				break;
			case ITEMGET(14, 1):	// 
				nAddRate = 20;
				break;
			case ITEMGET(14, 2):	// 
				nAddRate = 30;
				break;
			case ITEMGET(14, 3):	// 
				nAddRate = 40;
				break;
			}

			if (citem->m_Level == 1)	// #formula
			{
				nAddRate += 5;
			}

			tLife += ((int)gObj[aIndex].MaxLife * nAddRate) / 100;	// #formula

			if (citem->m_Type == ITEMGET(14, 0))
			{
				if (citem->m_Level < 2)
				{
					gObj[aIndex].FillLife += tLife;
					tLife = 0;
				}
			}

			if (gObj[aIndex].FillLife > 0.0f)
			{
				gObj[aIndex].Life += gObj[aIndex].FillLife;

				if (gObj[aIndex].Life > (gObj[aIndex].MaxLife + gObj[aIndex].AddLife))
				{
					gObj[aIndex].Life = gObj[aIndex].MaxLife + gObj[aIndex].AddLife;
					gObj[aIndex].FillLife = 0;
				}

				GCReFillSend(aIndex, gObj[aIndex].Life, 0xFF, 0, gObj[aIndex].iShield);
			}

			gObj[aIndex].FillLifeMax = tLife;
			gObj[aIndex].FillLife = tLife;

			if (citem->m_Type == ITEMGET(14, 0) && citem->m_Level < 2)
			{
				gObj[aIndex].FillLifeCount = 0;
			}
			else if (citem->m_Level == 1)
			{
				gObj[aIndex].FillLifeCount = 2;
			}
			else
			{
				gObj[aIndex].FillLifeCount = 3;
			}

			if (!gObjSearchItemMinus(&gObj[aIndex], pos, 1))
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
		}
		else if (citem->m_Type == ITEMGET(14, 4) ||	// Small Mana Potion
			citem->m_Type == ITEMGET(14, 5) ||
			citem->m_Type == ITEMGET(14, 6))
		{
			int tMana = (citem->m_Value * 10) - (gObj[aIndex].Level);	// #formula

			if (tMana < 0)
			{
				tMana = 0;
			}

			switch (citem->m_Type)
			{
			case ITEMGET(14, 4):	// Small Mana Potion
				tMana += ((int)(gObj[aIndex].MaxMana + gObj[aIndex].AddMana)) * 20 / 100;
				break;
			case ITEMGET(14, 5):	// Mana Potion
				tMana += ((int)(gObj[aIndex].MaxMana + gObj[aIndex].AddMana)) * 30 / 100;
				break;
			case ITEMGET(14, 6):	// Large Mana Potion
				tMana += ((int)(gObj[aIndex].MaxMana + gObj[aIndex].AddMana)) * 40 / 100;
				break;
			}

			gObj[aIndex].Mana += tMana;

			if (gObj[aIndex].Mana > (gObj[aIndex].MaxMana + gObj[aIndex].AddMana - 1.0f))
				gObj[aIndex].Mana = gObj[aIndex].MaxMana + gObj[aIndex].AddMana;

			GCManaSend(aIndex, gObj[aIndex].Mana, 0xFF, 0, gObj[aIndex].BP);

			if (!gObjSearchItemMinus(&gObj[aIndex], pos, 1))
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
		}
		else if (citem->m_Type == ITEMGET(14, 38))	// Small Compound Potion 
		{
			int iShieldGage = (gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield) * 5 / 100;	// #formula
			int iHPGage = (int)((gObj[aIndex].MaxLife + gObj[aIndex].AddLife) * 10.0f / 100.0f);	// #formula

			if (gObj[aIndex].iShield < 0)
				gObj[aIndex].iShield = 0;

			if (gObj[aIndex].FillLife > 0.0f)
			{
				gObj[aIndex].Life += gObj[aIndex].FillLife;

				if (gObj[aIndex].Life > (gObj[aIndex].MaxLife + gObj[aIndex].AddLife))
					gObj[aIndex].Life = gObj[aIndex].MaxLife + gObj[aIndex].AddLife;
			}

			if (gObj[aIndex].iFillShield > 0)
			{
				gObj[aIndex].iShield += gObj[aIndex].iFillShield;

				if (gObj[aIndex].iShield > (gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield))
					gObj[aIndex].iShield = gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield;
			}

			gObj[aIndex].FillLife = iHPGage;
			gObj[aIndex].FillLifeMax = iHPGage;
			gObj[aIndex].FillLifeCount = 4;
			gObj[aIndex].iFillShield = iShieldGage;
			gObj[aIndex].iFillShieldMax = iShieldGage;
			gObj[aIndex].iFillShieldCount = 4;

			GCSendEffectInfo(aIndex, 3);

			if (!gObjSearchItemMinus(&gObj[aIndex], pos, 1))
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}

			g_Log.Add("[%s][%s]Use Compound Potion Lv1 - SD[%d] HP[%f] -> SD[%d] HP[%f]",
				gObj[aIndex].AccountID, gObj[aIndex].Name,
				gObj[aIndex].iShield, gObj[aIndex].Life,
				gObj[aIndex].iShield + iShieldGage, (double)(gObj[aIndex].Life + (float)iHPGage));
		}
		else if (citem->m_Type == ITEMGET(14, 39))	// Compound Potion 
		{
			int iShieldGage = (gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield) * 10 / 100;	// #formula
			int iHPGage = (int)((gObj[aIndex].MaxLife + gObj[aIndex].AddLife) * 25.0f / 100.0f);	// #formula

			if (gObj[aIndex].iShield < 0)
				gObj[aIndex].iShield = 0;

			if (gObj[aIndex].FillLife > 0.0f)
			{
				gObj[aIndex].Life += gObj[aIndex].FillLife;

				if (gObj[aIndex].Life > (gObj[aIndex].MaxLife + gObj[aIndex].AddLife))
					gObj[aIndex].Life = gObj[aIndex].MaxLife + gObj[aIndex].AddLife;
			}

			if (gObj[aIndex].iFillShield > 0)
			{
				gObj[aIndex].iShield += gObj[aIndex].iFillShield;

				if (gObj[aIndex].iShield > (gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield))
					gObj[aIndex].iShield = gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield;
			}

			gObj[aIndex].FillLife = iHPGage;
			gObj[aIndex].FillLifeMax = iHPGage;
			gObj[aIndex].FillLifeCount = 4;
			gObj[aIndex].iFillShield = iShieldGage;
			gObj[aIndex].iFillShieldMax = iShieldGage;
			gObj[aIndex].iFillShieldCount = 4;

			GCSendEffectInfo(aIndex, 3);

			if (!gObjSearchItemMinus(&gObj[aIndex], pos, 1))
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}

			g_Log.Add("[%s][%s]Use Compound Potion Lv2 - SD[%d] HP[%f] -> SD[%d] HP[%f]",
				gObj[aIndex].AccountID, gObj[aIndex].Name,
				gObj[aIndex].iShield, gObj[aIndex].Life,
				gObj[aIndex].iShield + iShieldGage, (double)(gObj[aIndex].Life + (float)iHPGage));
		}
		else if (citem->m_Type == ITEMGET(14, 40))	// Large Compound Potion 
		{
			int iShieldGage = (gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield) * 20 / 100;	// #formula
			int iHPGage = (int)((gObj[aIndex].MaxLife + gObj[aIndex].AddLife) * 45.0f / 100.0f);	// #formula

			if (gObj[aIndex].iShield < 0)
				gObj[aIndex].iShield = 0;

			if (gObj[aIndex].FillLife > 0.0f)
			{
				gObj[aIndex].Life += gObj[aIndex].FillLife;

				if (gObj[aIndex].Life > (gObj[aIndex].MaxLife + gObj[aIndex].AddLife))
					gObj[aIndex].Life = gObj[aIndex].MaxLife + gObj[aIndex].AddLife;
			}

			if (gObj[aIndex].iFillShield > 0)
			{
				gObj[aIndex].iShield += gObj[aIndex].iFillShield;

				if (gObj[aIndex].iShield > (gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield))
					gObj[aIndex].iShield = gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield;
			}

			gObj[aIndex].FillLife = iHPGage;
			gObj[aIndex].FillLifeMax = iHPGage;
			gObj[aIndex].FillLifeCount = 4;
			gObj[aIndex].iFillShield = iShieldGage;
			gObj[aIndex].iFillShieldMax = iShieldGage;
			gObj[aIndex].iFillShieldCount = 4;

			GCSendEffectInfo(aIndex, 3);

			if (!gObjSearchItemMinus(&gObj[aIndex], pos, 1))
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}

			g_Log.Add("[%s][%s]Use Compound Potion Lv3 - SD[%d] HP[%f] -> SD[%d] HP[%f]",
				gObj[aIndex].AccountID, gObj[aIndex].Name,
				gObj[aIndex].iShield, gObj[aIndex].Life,
				gObj[aIndex].iShield + iShieldGage, (double)(gObj[aIndex].Life + (float)iHPGage));
		}
		else if (citem->m_Type == ITEMGET(14, 35))
		{
			int iShieldGage = (gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield) * 25 / 100;	// #formula

			if (gObj[aIndex].iShield < 0)
				gObj[aIndex].iShield = 0;

			if (gObj[aIndex].iFillShield > 0)
			{
				gObj[aIndex].iShield += gObj[aIndex].iFillShield;

				if (gObj[aIndex].iShield > (gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield))
					gObj[aIndex].iShield = gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield;
			}

			gObj[aIndex].iFillShieldCount = 2;
			gObj[aIndex].iFillShield = iShieldGage;
			gObj[aIndex].iFillShieldMax = iShieldGage;

			GCSendEffectInfo(aIndex, 3);

			if (!gObjSearchItemMinus(&gObj[aIndex], pos, 1))
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
		}
		else if (citem->m_Type == ITEMGET(14, 36))
		{
			int iShieldGage = (gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield) * 35 / 100;	// #formula

			if (gObj[aIndex].iShield < 0)
				gObj[aIndex].iShield = 0;

			if (gObj[aIndex].iFillShield > 0)
			{
				gObj[aIndex].iShield += gObj[aIndex].iFillShield;

				if (gObj[aIndex].iShield > (gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield))
					gObj[aIndex].iShield = gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield;
			}

			gObj[aIndex].iFillShieldCount = 2;
			gObj[aIndex].iFillShield = iShieldGage;
			gObj[aIndex].iFillShieldMax = iShieldGage;

			GCSendEffectInfo(aIndex, 3);

			if (!gObjSearchItemMinus(&gObj[aIndex], pos, 1))
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
		}
		else if (citem->m_Type == ITEMGET(14, 37))
		{
			int iShieldGage = (gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield) * 45 / 100; // #formula

			if (gObj[aIndex].iShield < 0)
				gObj[aIndex].iShield = 0;

			if (gObj[aIndex].iFillShield > 0)
			{
				gObj[aIndex].iShield += gObj[aIndex].iFillShield;

				if (gObj[aIndex].iShield > (gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield))
					gObj[aIndex].iShield = gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield;
			}

			gObj[aIndex].iFillShieldCount = 2;
			gObj[aIndex].iFillShield = iShieldGage;
			gObj[aIndex].iFillShieldMax = iShieldGage;

			GCSendEffectInfo(aIndex, 3);

			if (!gObjSearchItemMinus(&gObj[aIndex], pos, 1))
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
		}
		else if (citem->m_Type == ITEMGET(14, 8)) // Antidote
		{
			if (!gObjSearchItemMinus(&gObj[aIndex], pos, 1))
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}

			if (gObjCheckUsedBuffEffect(&gObj[aIndex], BUFFTYPE_POISON) == TRUE)
			{
				gObjRemoveBuffEffect(&gObj[aIndex], BUFFTYPE_POISON);
				gObj[aIndex].lpAttackObj = 0;
				GCMagicCancelSend(&gObj[aIndex], 1);
			}

			if (gObjCheckUsedBuffEffect(&gObj[aIndex], BUFFTYPE_FREEZE) == TRUE)
			{
				gObjRemoveBuffEffect(&gObj[aIndex], BUFFTYPE_FREEZE);
				gObj[aIndex].DelayActionTime = 0;
				gObj[aIndex].DelayLevel = 0;
				gObj[aIndex].lpAttackObj = 0;
				GCMagicCancelSend(&gObj[aIndex], 7);
			}
		}
		else if (citem->m_Type == ITEMGET(14, 46))
		{
			if (!gObjSearchItemMinus(&gObj[aIndex], pos, 1))
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}

			gObjAddBuffEffect(&gObj[aIndex], BUFFTYPE_HALLOWEEN_BLESS);
		}
		else if (citem->m_Type == ITEMGET(14, 47))
		{
			if (!gObjSearchItemMinus(&gObj[aIndex], pos, 1))
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}

			gObjAddBuffEffect(&gObj[aIndex], BUFFTYPE_HALLOWEEN_ANGER);
		}
		else if (citem->m_Type == ITEMGET(14, 48))
		{
			if (!gObjSearchItemMinus(&gObj[aIndex], pos, 1))
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}

			gObjAddBuffEffect(&gObj[aIndex], BUFFTYPE_HALLOWEEN_CRY);
		}
		else if (citem->m_Type == ITEMGET(14, 49))
		{
			if (!gObjSearchItemMinus(&gObj[aIndex], pos, 1))
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}

			gObjAddBuffEffect(&gObj[aIndex], BUFFTYPE_HALLOWEEN_FOOD);
		}
		else if (citem->m_Type == ITEMGET(14, 50))
		{
			if (!gObjSearchItemMinus(&gObj[aIndex], pos, 1))
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}

			gObjAddBuffEffect(&gObj[aIndex], BUFFTYPE_HALLOWEEN_DRINK);
		}
		else if (citem->m_Type == ITEMGET(14, 85))
		{
			if (!gObjSearchItemMinus(&gObj[aIndex], pos, 1))
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}

			gObjAddBuffEffect(&gObj[aIndex], BUFFTYPE_CHERRYBLOSSOM_DRINK);
		}
		else if (citem->m_Type == ITEMGET(14, 86))
		{
			if (!gObjSearchItemMinus(&gObj[aIndex], pos, 1))
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}

			gObjAddBuffEffect(&gObj[aIndex], BUFFTYPE_CHERRYBLOSSOM_DUMPLING);
		}
		else if (citem->m_Type == ITEMGET(14, 87))
		{
			if (!gObjSearchItemMinus(&gObj[aIndex], pos, 1))
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}

			gObjAddBuffEffect(&gObj[aIndex], BUFFTYPE_CHERRYBLOSSOM_PETAL);
		}
		else if (citem->m_Type == ITEMGET(13, 81))
		{
			if (!gObjSearchItemMinus(&gObj[aIndex], pos, 1))
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}

			gObjAddBuffEffect(&gObj[aIndex], BUFFTYPE_CHARM_GUARDIAN);
		}
		else if (citem->m_Type == ITEMGET(13, 82))
		{
			if (!gObjSearchItemMinus(&gObj[aIndex], pos, 1))
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}

			gObjAddBuffEffect(&gObj[aIndex], BUFFTYPE_CHARM_PROTECTITEM);
		}

		else if (citem->m_Type == ITEMGET(13, 43))
		{
			if (!gObjSearchItemMinus(&gObj[aIndex], pos, 1))
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}

			gObjAddBuffEffect(&gObj[aIndex], BUFFTYPE_PCS_MARK1);
		}

		else if (citem->m_Type == ITEMGET(13, 44))
		{
			if (!gObjSearchItemMinus(&gObj[aIndex], pos, 1))
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}

			gObjAddBuffEffect(&gObj[aIndex], BUFFTYPE_PCS_MARK2);
		}

		else if (citem->m_Type == ITEMGET(13, 45))
		{
			if (!gObjSearchItemMinus(&gObj[aIndex], pos, 1))
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}

			gObjAddBuffEffect(&gObj[aIndex], BUFFTYPE_PCS_MARK3);
		}

		else if (g_kJewelOfHarmonySystem.IsJewelOfHarmonyPurity(citem->m_Type) == TRUE && g_SocketOptionSystem.IsSocketItem(&gObj[aIndex].pInventory[lpMsg->invenrotyTarget]) == FALSE) //season4 add-on)
		{
			if (g_kJewelOfHarmonySystem.StrengthenItemByJewelOfHarmony(&gObj[aIndex], lpMsg->inventoryPos, lpMsg->invenrotyTarget) == TRUE)
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemOneSend(aIndex, lpMsg->invenrotyTarget);
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
			else
			{
				GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);
			}
		}
		else if (g_kJewelOfHarmonySystem.IsJewelOfHarmonySmeltingItems(citem->m_Type) == TRUE)
		{
			if (g_kJewelOfHarmonySystem.SmeltItemBySmeltingStone(&gObj[aIndex], lpMsg->inventoryPos, lpMsg->invenrotyTarget) == TRUE)
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemOneSend(aIndex, lpMsg->invenrotyTarget);
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
			else
			{
				GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);
			}
		}
		else if (citem->m_Type == ITEMGET(14, 13)) // Jewel Of Bless
		{
			if (gObjItemLevelUp(&gObj[aIndex], lpMsg->inventoryPos, lpMsg->invenrotyTarget) == TRUE)
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemOneSend(aIndex, lpMsg->invenrotyTarget);
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
		}
		else if (citem->m_Type == ITEMGET(12, 30))
		{
			if (UseBundleOfBlessJewel(&gObj[aIndex], lpMsg->inventoryPos, lpMsg->invenrotyTarget) == TRUE)
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemOneSend(aIndex, lpMsg->invenrotyTarget);
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
		}
		else if (citem->m_Type == ITEMGET(14, 14)) // Jewel Of Soul
		{
			if (gObjItemRandomLevelUp(&gObj[aIndex], lpMsg->inventoryPos, lpMsg->invenrotyTarget) == TRUE)
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemOneSend(aIndex, lpMsg->invenrotyTarget);
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
		}
		else if (citem->m_Type == ITEMGET(14, 160))	//1.01.00 update
		{
			if (g_LuckyItemManager.LuckyItemRepaire(&gObj[aIndex], lpMsg->inventoryPos, lpMsg->invenrotyTarget))
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemOneSend(aIndex, lpMsg->invenrotyTarget);
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
			else
			{
				GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);
			}
		}

		else if (citem->m_Type == ITEMGET(14, 161))	//1.01.00 update
		{
			if (g_kJewelOfHarmonySystem.StrengthenItemByJewelOfRise(&gObj[aIndex], lpMsg->inventoryPos, lpMsg->invenrotyTarget))
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemOneSend(aIndex, lpMsg->invenrotyTarget);
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
			else
			{
				GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);
			}
		}
		else if (citem->m_Type == ITEMGET(14, 209)) // Tradeable Seal Season 8
		{
			if (g_PentagramSystem.AddTradeCount(&gObj[aIndex], lpMsg->inventoryPos, lpMsg->invenrotyTarget) == TRUE)
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemOneSend(aIndex, lpMsg->invenrotyTarget);
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}

			else
			{
				GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);
			}
		}

		else if (citem->m_Type == ITEMGET(14, 224))
		{
			if (!gObjSearchItemMinus(&gObj[aIndex], pos, 1))
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}

			gObjAddBuffEffect(&gObj[aIndex], BUFFTYPE_LIGHT_BLESSING);
		}

		else if (citem->m_Type == ITEMGET(14, 263))
		{
			if (!gObjSearchItemMinus(&gObj[aIndex], pos, 1))
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}

			gObjAddBuffEffect(&gObj[aIndex], BUFFTYPE_LIGHT_BLESSING_LOW);
		}

		else if (citem->m_Type == ITEMGET(14, 264))
		{
			if (!gObjSearchItemMinus(&gObj[aIndex], pos, 1))
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}

			gObjAddBuffEffect(&gObj[aIndex], BUFFTYPE_LIGHT_BLESSING_MIDDLE);
		}

		else if (citem->m_Type == ITEMGET(14, 16)) // Jewel Of Life
		{
			if (gObjItemRandomOption3Up(&gObj[aIndex], lpMsg->inventoryPos, lpMsg->invenrotyTarget) == TRUE)
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemOneSend(aIndex, lpMsg->invenrotyTarget);
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
			else
			{
				GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);
			}
		}
		else if (citem->m_Type >= ITEMGET(15, 0) ||	// Group 15 - Scrolls
			citem->m_Type == ITEMGET(12, 7) || // Orb of Twisting Slash
			(citem->m_Type >= ITEMGET(12, 8) && citem->m_Type <= ITEMGET(12, 24)) ||	// Orbs
			citem->m_Type == ITEMGET(12, 35) ||
			(citem->m_Type >= ITEMGET(12, 44) && citem->m_Type <= ITEMGET(12, 48)) ||	// Scroll of Fire Scream
			(citem->m_Type >= ITEMGET(12,271) && citem->m_Type <= ITEMGET(12, 277))) // Grow Lancer Skills
		{
			if ((gObj[aIndex].m_PlayerData->Strength + gObj[aIndex].AddStrength) < citem->m_RequireStrength)
			{
				GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);
				return;
			}

			if ((gObj[aIndex].m_PlayerData->Dexterity + gObj[aIndex].AddDexterity) < citem->m_RequireDexterity)
			{
				GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);
				return;
			}

			if (citem->m_Type == ITEMGET(15, 18))	// Scroll of HellBurst
			{
				if (g_QuestInfo.GetQuestState(&gObj[aIndex], 2) != 2)
				{
					GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);
					return;
				}
			}

			if (citem->m_Type >= ITEMGET(12, 8) && citem->m_Type <= ITEMGET(12, 24))
			{
				if (gObj[aIndex].Level < citem->m_RequireLevel)	// Orbs
				{
					GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);
					return;
				}
			}

			unsigned short skillnumber;

			if (citem->IsClass(gObj[aIndex].Class, gObj[aIndex].m_PlayerData->ChangeUP) == FALSE)
			{
				GCInventoryItemDeleteSend(aIndex, -1, 1);
			}
			else
			{
				int addskill = gObjMagicAdd(&gObj[aIndex], ITEM_GET_TYPE(citem->m_Type), ITEM_GET_INDEX(citem->m_Type), citem->m_Level, skillnumber);

				if (addskill >= 0)
				{
					gObjInventoryItemSet(aIndex, pos, -1);
					GCMagicListOneSend(aIndex, addskill, skillnumber, citem->m_Level, 0, 0);
					gObj[aIndex].pInventory[pos].Clear();
					GCInventoryItemDeleteSend(aIndex, pos, 1);
				}
				else
				{
					GCInventoryItemDeleteSend(aIndex, -1, 1);
				}
			}
		}
		else if (citem->m_Type == ITEMGET(14, 10)) // Town Portal Scroll
		{
			if (g_ConfigRead.server.GetServerType() == SERVER_BATTLECORE)
			{
				g_Log.Add("[UBF][Return Scroll] [%s][%s][%s] - No Available",
					gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_PlayerData->m_RealNameOfUBF);

				return;
			}

			g_Log.Add("[Using Item] [Return Scroll] [%s][%s] - Current Map:[%d]",
				gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].MapNumber);

			if (BC_MAP_RANGE(gObj[aIndex].MapNumber))
			{
				int iBridgeIndex = g_BloodCastle.GetBridgeIndex(gObj[aIndex].MapNumber);

				if (g_BloodCastle.GetCurrentState(iBridgeIndex) == 2)
				{
					g_BloodCastle.SearchUserDropQuestItem(aIndex);
				}
				else
				{
					g_BloodCastle.SearchUserDeleteQuestItem(aIndex);
				}
			}

			if (IT_MAP_RANGE(gObj[aIndex].MapNumber))
			{
				g_IT_Event.DropRelicsItem(gObj[aIndex].MapNumber, aIndex);
			}

			if (gObj[aIndex].m_IfState.use && gObj[aIndex].m_IfState.type == 3)
			{
				gObj[aIndex].TargetNpcNumber = -1;
				gObj[aIndex].m_IfState.type = 0;
				gObj[aIndex].m_IfState.use = 0;
			}

			gObjInventoryItemSet(aIndex, pos, -1);
			gObj[aIndex].pInventory[pos].Clear();
			GCInventoryItemDeleteSend(aIndex, pos, 1);

			if (gObj[aIndex].MapNumber == MAP_INDEX_DEVIAS)
			{
				gObjMoveGate(aIndex, 22);
			}
			else if (gObj[aIndex].MapNumber == MAP_INDEX_NORIA)
			{
				gObjMoveGate(aIndex, 27);
			}
			else if (gObj[aIndex].MapNumber == MAP_INDEX_ELBELAND)
			{
				gObjMoveGate(aIndex, 267);
			}
			else if (gObj[aIndex].MapNumber == MAP_INDEX_LOSTTOWER)
			{
				gObjMoveGate(aIndex, 42);
			}
			else if (gObj[aIndex].MapNumber == MAP_INDEX_ATHLANSE)
			{
				gObjMoveGate(aIndex, 49);
			}
			else if (gObj[aIndex].MapNumber == MAP_INDEX_TARKAN)
			{
				gObjMoveGate(aIndex, 57);
			}
			else if (BC_MAP_RANGE(gObj[aIndex].MapNumber) != FALSE)
			{
				gObjMoveGate(aIndex, 22);
			}
			else if (CC_MAP_RANGE(gObj[aIndex].MapNumber) != FALSE)
			{
				gObjMoveGate(aIndex, 22);
			}
			else if (gObj[aIndex].MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL)
			{
				gObjMoveGate(aIndex, 333);
			}
			else if (KALIMA_MAP_RANGE(gObj[aIndex].MapNumber) != FALSE)
			{
				gObjMoveGate(aIndex, 22);
			}
			else if (IT_MAP_RANGE(gObj[aIndex].MapNumber) != FALSE)
			{
				gObjMoveGate(aIndex, 267);
			}
			else if (gObj[aIndex].MapNumber == MAP_INDEX_AIDA)
			{
				gObjMoveGate(aIndex, 27);
			}
			else if (gObj[aIndex].MapNumber == MAP_INDEX_CRYWOLF_FIRSTZONE)
			{
				gObjMoveGate(aIndex, 27);
			}
			else if (gObj[aIndex].MapNumber == MAP_INDEX_BARRACKS)
			{
				gObjMoveGate(aIndex, 114);
			}
			else if (gObj[aIndex].MapNumber == MAP_INDEX_REFUGEE)
			{
				gObjMoveGate(aIndex, 114);
			}
			else if (gObj[aIndex].MapNumber == MAP_INDEX_CALMNESS)
			{
				gObjMoveGate(aIndex, 273);
			}
			else if (gObj[aIndex].MapNumber == MAP_INDEX_RAKLION || gObj[aIndex].MapNumber == MAP_INDEX_HATCHERY)
			{
				gObjMoveGate(aIndex, 287);
			}
			else if (gObj[aIndex].MapNumber == MAP_INDEX_SANTAVILLAGE)
			{
				gObjMoveGate(aIndex, 22);
			}
			else if (DG_MAP_RANGE(gObj[aIndex].MapNumber) != FALSE)
			{
				gObjMoveGate(aIndex, 267);
			}
			else if (IMPERIAL_MAP_RANGE(gObj[aIndex].MapNumber) != FALSE)
			{
				gObjMoveGate(aIndex, 22);
			}
			else if (gObj[aIndex].MapNumber == MAP_INDEX_LORENMARKET)
			{
				gObjMoveGate(aIndex, 333);
			}

			else if (g_ConfigRead.server.GetServerType() == SERVER_CASTLE && gObj[aIndex].MapNumber == MAP_INDEX_CASTLESIEGE)
			{
				if (g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
				{
					if (gObj[aIndex].m_btCsJoinSide == 1)
					{
						gObjMoveGate(aIndex, 101);
					}
					else
					{
						gObjMoveGate(aIndex, 100);
					}
				}
				else
				{
					if (g_CastleSiege.CheckCastleOwnerMember(aIndex) == TRUE || g_CastleSiege.CheckCastleOwnerUnionMember(aIndex) == TRUE)
					{
						gObjMoveGate(aIndex, 101);
					}
					else
					{
						gObjMoveGate(aIndex, 100);
					}
				}
			}

			else if ( g_NewPVP.IsVulcanusMap(gObj[aIndex].MapNumber) )
			{
				gObjMoveGate(aIndex, 294);
			}

			else if ( g_NewPVP.IsPKFieldMap(gObj[aIndex].MapNumber) )
			{
				g_NewPVP.Reset(gObj[aIndex]);
				gObjMoveGate(aIndex, 17);
			}

			else if ( gObj[aIndex].MapNumber == MAP_INDEX_BATTLESOCCER )
			{
			}

			else
			{
				gObjMoveGate(aIndex, 17);
			}

		}
		else if ( citem->m_Type == ITEMGET(13,66) )
		{
			if ( DG_MAP_RANGE(gObj[aIndex].MapNumber) == TRUE )
			{
				g_DoppelGanger.LeaveDoppelganger(aIndex);

				if ( g_DoppelGanger.GetDoppelgangerState() == 2 )
				{
					g_DoppelGanger.SendDoppelgangerResult(&gObj[aIndex], 1);
				}
			}

			gObjTeleport(aIndex, MAP_INDEX_SANTAVILLAGE, 220, 20);

			if ( !gObjSearchItemMinus(&gObj[aIndex], pos, 1) )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
		}

		else if ( citem->m_Type == ITEMGET(14,162) )
		{
			if(gObj[aIndex].m_PlayerData->m_InventoryExpansion < 2)
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
				gObj[aIndex].m_PlayerData->m_InventoryExpansion++;
				GDSetExpWare(aIndex, 1, gObj[aIndex].m_PlayerData->m_InventoryExpansion);

				PMSG_USEEXPANSIONITEM pMsg;
				PHeadSetB((LPBYTE)&pMsg, 0x2B, sizeof(pMsg));
				pMsg.Result = 1;
				IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
			}
		}

		else if ( citem->m_Type == ITEMGET(14,163) )
		{
			if(gObj[aIndex].m_PlayerData->m_WarehouseExpansion < 1)
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
				GDSetExpWare(aIndex, 2, 1);
				gObj[aIndex].m_PlayerData->m_WarehouseExpansion = 1;

				PMSG_USEEXPANSIONITEM pMsg;
				PHeadSetB((LPBYTE)&pMsg, 0x2B, sizeof(pMsg));
				pMsg.Result = 1;
				IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
			}
		}

		else if ( citem->m_Type == ITEMGET(13,152) || citem->m_Type == ITEMGET(13,156) )
		{
			if (g_MasterLevelSkillTreeSystem.CheckMasterSkillPoint(aIndex, 1) == TRUE)
			{
				g_MasterLevelSkillTreeSystem.ResetMasterSkill(aIndex, 1);
				g_Log.Add("[MasterLevelSkill Reset] - Passive (%d)", aIndex);
				gObjCloseSet(aIndex, 1);
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}

			else
			{
				this->GCServerMsgStringSend(Lang.GetText(0,526), aIndex, 1);
			}
		}

		else if (citem->m_Type == ITEMGET(13, 153) || citem->m_Type == ITEMGET(13,157))
		{
			if (g_MasterLevelSkillTreeSystem.CheckMasterSkillPoint(aIndex, 2) == TRUE)
			{
				g_MasterLevelSkillTreeSystem.ResetMasterSkill(aIndex, 2);
				g_Log.Add("[MasterLevelSkill Reset] - Strengthen Skill (%d)", aIndex);
				gObjCloseSet(aIndex, 1);
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}

			else
			{
				this->GCServerMsgStringSend(Lang.GetText(0,526), aIndex, 1);
			}
		}

		else if (citem->m_Type == ITEMGET(13, 154) || citem->m_Type == ITEMGET(13,158))
		{
			if (g_MasterLevelSkillTreeSystem.CheckMasterSkillPoint(aIndex, 3) == TRUE)
			{
				g_MasterLevelSkillTreeSystem.ResetMasterSkill(aIndex, 3);
				g_Log.Add("[MasterLevelSkill Reset] - Enhance Attack/Defense (%d)", aIndex);
				gObjCloseSet(aIndex, 1);
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}

			else
			{
				this->GCServerMsgStringSend(Lang.GetText(0,526), aIndex, 1);
			}
		}

		else if (citem->m_Type == ITEMGET(13, 155) || citem->m_Type == ITEMGET(13,159))
		{
			if (g_MasterLevelSkillTreeSystem.CheckMasterSkillPoint(aIndex, 0) == TRUE)
			{
				g_MasterLevelSkillTreeSystem.ResetMasterSkill(aIndex, 0);
				g_Log.Add("[MasterLevelSkill Reset] - All (%d)", aIndex);
				gObjCloseSet(aIndex, 1);
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}

			else
			{
				this->GCServerMsgStringSend(Lang.GetText(0,526), aIndex, 1);
			}
		}

		else if ( citem->m_Type == ITEMGET(14,9) ) // Ale
		{
			int level = citem->m_Level;

			gObjInventoryItemSet(aIndex, pos, -1);
			gObj[aIndex].pInventory[pos].Clear();
			GCInventoryItemDeleteSend(aIndex, pos, 1);
			gObjUseDrink(&gObj[aIndex], level);
		}
		else if ( citem->m_Type == ITEMGET(14,20) ) // Remedy of Love
		{
			if ( citem->m_Level == 0 )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
				gObjUseDrink(&gObj[aIndex], 2);
			}
		}
		else if ( citem->m_Type == ITEMGET(13,15) ) // Fruits
		{
			if ( iItemUseType == 0 )
			{
				gObjUsePlusStatFruit(aIndex, pos);
			}
			else if ( iItemUseType == 1 )
			{
				gObjUseMinusStatFruit(aIndex, pos);
			}
		}
		else if ( citem->m_Type == ITEMGET(13,70) )
		{
			if(gObj[aIndex].SaveMapNumber != -1)
			{
				bool bMove = gMoveCommand.CheckEquipmentToMove(&gObj[aIndex], gObj[aIndex].SaveMapNumber);

				if(bMove == true && MapNumberCheck(gObj[aIndex].SaveMapNumber))
				{
					gObjTeleport(aIndex, gObj[aIndex].SaveMapNumber, gObj[aIndex].SaveX, gObj[aIndex].SaveY);
					gObjInventoryItemSet(aIndex, pos, -1);
					gObj[aIndex].pInventory[pos].Clear();
					GCInventoryItemDeleteSend(aIndex, pos, 1);
					gObj[aIndex].SaveMapNumber = -1;
					GCServerCmd(aIndex, 61, 0, 0);
				}
			}
			else
			{
				if(gObjSearchItemMinus(&gObj[aIndex], pos, 1))
				{
					gObj[aIndex].SaveMapNumber = gObj[aIndex].MapNumber;
					gObj[aIndex].SaveX = gObj[aIndex].X;
					gObj[aIndex].SaveY = gObj[aIndex].Y;
				}
				else
				{
					gObjInventoryItemSet(aIndex, pos, -1);
					gObj[aIndex].pInventory[pos].Clear();
					GCInventoryItemDeleteSend(aIndex, pos, 1);
				}
			}
		}
		else if ( citem->m_Type == ITEMGET(13,69) )
		{
			if(gObj[aIndex].DieMapNumber != -1)
			{
				bool bMove = gMoveCommand.CheckEquipmentToMove(&gObj[aIndex], gObj[aIndex].DieMapNumber);

				bool bGens = g_GensSystem.IsMapBattleZone(gObj[aIndex].MapNumber);

				if(bMove == true && bGens == false && MapNumberCheck(gObj[aIndex].DieMapNumber))
				{
					gObjTeleport(aIndex, gObj[aIndex].DieMapNumber, gObj[aIndex].DieX, gObj[aIndex].DieY);
					gObjInventoryItemSet(aIndex, pos, -1);
					gObj[aIndex].pInventory[pos].Clear();
					GCInventoryItemDeleteSend(aIndex, pos, 1);
					gObj[aIndex].DieMapNumber = -1;
					GCServerCmd(aIndex, 60, 0, 0);
				}
			}
		}
		else if ( citem->m_Type >= ITEMGET(13,54) && citem->m_Type <= ITEMGET(13,58) )
		{
			CashShopExMinusStatFruit(aIndex, pos);
		}
		else if ( citem->m_Type == ITEMGET(14,70) ) // CSHOP HP Potions
		{
			int tLife = (citem->m_Value*10) - (gObj[aIndex].Level);	// #formula

			if ( tLife <  0 )
			{
				tLife = 0;
			}

			tLife += ((int)(gObj[aIndex].MaxLife + gObj[aIndex].AddLife));	// #formula
			gObj[aIndex].Life += tLife;

			if ( gObj[aIndex].Life > (gObj[aIndex].MaxLife+gObj[aIndex].AddLife-1.0f) )
				gObj[aIndex].Life = gObj[aIndex].MaxLife+gObj[aIndex].AddLife;

			GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);

			gObj[aIndex].FillLifeMax = tLife;
			gObj[aIndex].FillLife = tLife;

			if ( !gObjSearchItemMinus(&gObj[aIndex], pos, 1) )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
		}
		else if ( citem->m_Type == ITEMGET(14,71) ) // CSHOP MP Potions
		{
			int tMana = (citem->m_Value*10) - (gObj[aIndex].Level);	// #formula

			if ( tMana < 0 )
			{
				tMana=0;
			}

			tMana += ((int)(gObj[aIndex].MaxMana + gObj[aIndex].AddMana));
			gObj[aIndex].Mana += tMana;

			if ( gObj[aIndex].Mana > (gObj[aIndex].MaxMana+gObj[aIndex].AddMana-1.0f) )
				gObj[aIndex].Mana = gObj[aIndex].MaxMana+gObj[aIndex].AddMana;
			
			GCManaSend(aIndex, gObj[aIndex].Mana, 0xFF, 0, gObj[aIndex].BP);

			if ( !gObjSearchItemMinus(&gObj[aIndex], pos, 1) )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
		}
		else if ( citem->m_Type == ITEMGET(14,133) )
		{
			float fSD = (gObj[aIndex].iMaxShield+gObj[aIndex].iAddShield) * 65 / 100;

			if(gObj[aIndex].iShield < fSD)
				gObj[aIndex].iShield = fSD;

			GCReFillSend(aIndex, gObj[aIndex].Life, 0xFF, 1, gObj[aIndex].iShield);

			if ( !gObjSearchItemMinus(&gObj[aIndex], pos, 1) )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
		}

		else if ( citem->m_Type == ITEMGET(14,78) ) // CSHOP Arcanum For?a
		{
			LPITEMEFFECT lpEffect = g_ItemAddOption.GetItemEffectData(citem->m_Type);

			if (lpEffect == NULL)
			{
				GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);
				return;
			}

			g_PeriodItemEx.SetPeriodItemInfo(&gObj[aIndex], citem->m_Type, 0, lpEffect->iEffectValidTime);
			if ( !gObjSearchItemMinus(&gObj[aIndex], pos, 1) )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
		}
		else if ( citem->m_Type == ITEMGET(14,79) ) // CSHOP Arcanum Agilidade
		{
			LPITEMEFFECT lpEffect = g_ItemAddOption.GetItemEffectData(citem->m_Type);

			if (lpEffect == NULL)
			{
				GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);
				return;
			}

			g_PeriodItemEx.SetPeriodItemInfo(&gObj[aIndex], citem->m_Type, 0, lpEffect->iEffectValidTime);
			if ( !gObjSearchItemMinus(&gObj[aIndex], pos, 1) )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
		}
		else if ( citem->m_Type == ITEMGET(14,80) ) // CSHOP Arcanum Vitalidade
		{
			LPITEMEFFECT lpEffect = g_ItemAddOption.GetItemEffectData(citem->m_Type);

			if (lpEffect == NULL)
			{
				GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);
				return;
			}

			g_PeriodItemEx.SetPeriodItemInfo(&gObj[aIndex], citem->m_Type, 0, lpEffect->iEffectValidTime);
			if ( !gObjSearchItemMinus(&gObj[aIndex], pos, 1) )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
		}
		else if ( citem->m_Type == ITEMGET(14,81) ) // CSHOP Arcanum Energia
		{
			LPITEMEFFECT lpEffect = g_ItemAddOption.GetItemEffectData(citem->m_Type);

			if (lpEffect == NULL)
			{
				GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);
				return;
			}

			g_PeriodItemEx.SetPeriodItemInfo(&gObj[aIndex], citem->m_Type, 0, lpEffect->iEffectValidTime);
			if ( !gObjSearchItemMinus(&gObj[aIndex], pos, 1) )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
		}
		else if ( citem->m_Type == ITEMGET(14,82) ) // CSHOP Arcanum Comando
		{
			LPITEMEFFECT lpEffect = g_ItemAddOption.GetItemEffectData(citem->m_Type);

			if (lpEffect == NULL)
			{
				GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);
				return;
			}

			g_PeriodItemEx.SetPeriodItemInfo(&gObj[aIndex], citem->m_Type, 0, lpEffect->iEffectValidTime);
			if ( !gObjSearchItemMinus(&gObj[aIndex], pos, 1) )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
		}
		else if ( citem->m_Type == ITEMGET(13,60) ) // CSHOP PK Free Ticket
		{
			if ( gObj[aIndex].m_PK_Level == 6 )
				{
					gObj[aIndex].m_PK_Level = 5;
				}
				else if ( gObj[aIndex].m_PK_Level == 5 )
				{
					gObj[aIndex].m_PK_Level = 4;
				}
				else if ( gObj[aIndex].m_PK_Level == 4 )
				{
					gObj[aIndex].m_PK_Level = 3;
				}
				else if ( gObj[aIndex].m_PK_Level <= 3 )
				{
					gObj[aIndex].m_PK_Level = gObj[aIndex].m_PK_Level;
				}

				GCPkLevelSend(aIndex, gObj[aIndex].m_PK_Level);

				if ( !gObjSearchItemMinus(&gObj[aIndex], pos, 1) )
				{
					gObjInventoryItemSet(aIndex, pos, -1);
					gObj[aIndex].pInventory[pos].Clear();
					GCInventoryItemDeleteSend(aIndex, pos, 1);
				}
				g_Log.Add("[PCS] [%s][%s] Use PK Clear Item", gObj[aIndex].AccountID, gObj[aIndex].Name);
		}
		else if ( citem->m_Type == ITEMGET(14,7) )	// Siege Potion
		{
			if ( !gObjSearchItemMinus(&gObj[aIndex], pos, 1) )
			{
				gObjInventoryItemSet(aIndex, pos, -1);
				gObj[aIndex].pInventory[pos].Clear();
				GCInventoryItemDeleteSend(aIndex, pos, 1);
			}
			
			int iLevel = citem->m_Level;
			int iSerial = citem->GetNumber();
			int iDur = citem->m_Durability;

			if ( gObj[aIndex].m_PlayerData->lpGuild )
			{
				g_Log.Add("[CastleSiege] Using CastleSiege Potion Lv[%d]/Si[%d]/Dur[%d] - [%s][%s] - (Guild : %s)",
					iLevel, iSerial, iDur, gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_PlayerData->lpGuild->Name);
			}
			else
			{
				g_Log.Add("[CastleSiege] Using CastleSiege Potion Lv[%d]/Si[%d]/Dur[%d] - [%s][%s]",
					iLevel, iSerial, iDur, gObj[aIndex].AccountID, gObj[aIndex].Name);
			}

			if ( iLevel == 0 )
			{
				gObjUseBlessAndSoulPotion(aIndex, iLevel);
			}
			else if ( iLevel == 1 )
			{
				gObjUseBlessAndSoulPotion(aIndex, iLevel);
			}
		}
		else if ( citem->m_Type == ITEMGET(13,48) )
		{
			if ( (gObj[aIndex].MapNumber < MAP_INDEX_KALIMA1 || gObj[aIndex].MapNumber > MAP_INDEX_KALIMA6 ) && gObj[aIndex].MapNumber != MAP_INDEX_KALIMA7 )
			{
				int iLevel = g_KalimaGate.GetKalimaGateLevel2(aIndex);
				int iSerial = citem->GetNumber();
				int iDuration = citem->m_Durability;
				BOOL bKalimaGateCreateResult = FALSE;
				int iKalimaGateX = 0;
				int iKalimaGateY = 0;

				iLevel++;
				iKalimaGateX = gObj[aIndex].X + rand()%6 - 2;
				iKalimaGateY = gObj[aIndex].Y + rand()%6 - 2;

				bKalimaGateCreateResult = g_KalimaGate.CreateKalimaGate(aIndex, iLevel, iKalimaGateX, iKalimaGateY);

				if ( bKalimaGateCreateResult == TRUE )
				{
					if ( !gObjSearchItemMinus(&gObj[aIndex], pos, 1) )
					{
						gObjInventoryItemSet(aIndex, pos, -1);
						gObj[aIndex].pInventory[pos].Clear();
						GCInventoryItemDeleteSend(aIndex, pos, 1);
					}

					g_Log.Add("[PCS] Use Free Kalima Ticket Success");
				}
				else
				{
					g_Log.Add("[PCS] Use Free Kalima Ticket Failed");
				}
			}
		}

		else if (g_QuestExpManager.IsQuestItemAtt(citem->m_Type, 64) == true)
		{
			int Ep = g_QuestExpManager.GetQuestItemEp(citem->m_Type);
			int iResult = 0;
			lua_State *L = g_MuLuaQuestExp.GetLua();

			if (!L)
			{
				g_Log.Add("[QuestExp] - Error - [%s] [%d]", __FILE__, __LINE__);
			}

			else
			{
				if (Ep)
				{
					g_Generic_Call(L, "ItemUseQuest", "ii>i", aIndex, Ep, &iResult);

					if (iResult)
					{
						gObjInventoryItemSet(aIndex, pos, -1);
						gObj[aIndex].pInventory[pos].Clear();
						GCInventoryItemDeleteSend(aIndex, pos, 1);
					}
				}
			}
		}
		else
		{
			g_Log.Add("error-L3 : %s return %s %d %d %s",
				gObj[aIndex].Name, __FILE__,__LINE__, pos, gObj[aIndex].pInventory[pos].GetName() );

			GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);
		}
	}
	else
	{
		GCReFillSend(aIndex, gObj[aIndex].Life, 0xFD, 1, gObj[aIndex].iShield);
		g_Log.Add("error-L3 : %s return %s %d %d",
			gObj[aIndex].Name, __FILE__,__LINE__, pos);
	}
}


struct PMSG_REFILL
{
	PBMSG_HEAD h;	// C1:26
	BYTE IPos;	// 3
	BYTE LifeH;	// 4
	BYTE LifeL;	// 5
	BYTE Flag;	// 6
	BYTE btShieldH;	// 7
	BYTE btShieldL;	// 8
	int IGCLife;
};



void GameProtocol::GCReFillSend(int aIndex, int Life, BYTE Ipos, unsigned char flag,  WORD wShield)
{
	PMSG_REFILL pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x26, sizeof(pMsg));
	pMsg.IPos = Ipos;
	pMsg.LifeH = SET_NUMBERH(Life);
	pMsg.LifeL = SET_NUMBERL(Life);
	pMsg.btShieldH = SET_NUMBERH(wShield);
	pMsg.btShieldL = SET_NUMBERL(wShield);
	pMsg.Flag = flag;	// #error Flag is Disabled
	pMsg.IGCLife = Life;
	IOCP.DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
}


struct PMSG_MANASEND
{
	PBMSG_HEAD h;	// C1:27
	BYTE IPos;	// 3
	BYTE ManaH;	// 4
	BYTE ManaL;	// 5
	BYTE BPH;	// 6
	BYTE BPL;	// 7
	int IGCMana;
};

void GameProtocol::GCManaSend(int aIndex, int Mana, BYTE Ipos, unsigned char flag,  WORD BP)
{
	if ( aIndex < 0 || aIndex > g_ConfigRead.server.GetObjectMax()-1)
	{
		g_Log.Add("GCManaSend() return %s %d", __FILE__, __LINE__);
		return;
	}

	if ( gObj[aIndex].Type != OBJ_USER )
	{
		return;
	}

	PMSG_MANASEND pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x27, sizeof(pMsg));
	pMsg.IPos = Ipos;
	pMsg.ManaH = SET_NUMBERH(Mana);
	pMsg.ManaL = SET_NUMBERL(Mana);
	pMsg.BPH = SET_NUMBERH(BP);
	pMsg.BPL = SET_NUMBERL(BP);
	pMsg.IGCMana = Mana;
	IOCP.DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
}


struct PMSG_INVENTORYDELETE
{
	PBMSG_HEAD h;	// C1:28
	BYTE IPos;	// 3
	BYTE Flag;	// 4
};

void GameProtocol::GCInventoryItemDeleteSend(int aIndex, BYTE pos, unsigned char flag)
{
	PMSG_INVENTORYDELETE pMsg;
	
	PHeadSetB((LPBYTE)&pMsg, 0x28, sizeof(pMsg));
	pMsg.IPos = pos;
	pMsg.Flag = flag;

	IOCP.DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
}

struct PMSG_ITEMUSESPECIALTIME
{
	PBMSG_HEAD h;	// C1:29
	BYTE Number;	// 3
	WORD Time;	// 4
};

void GameProtocol::GCItemUseSpecialTimeSend(int aIndex, unsigned char number, int time)
{
	if ( time > 65535 )
	{
		time = 65535;
	}

	PMSG_ITEMUSESPECIALTIME pMsg;

	PHeadSetBE((LPBYTE)&pMsg, 0x29, sizeof(pMsg));
	pMsg.Number = number;
	pMsg.Time = time;

	IOCP.DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
}

struct PMSG_ITEMDUR
{
	PBMSG_HEAD h;	// C1:2A
	BYTE IPos;	// 3
	BYTE Dur;	// 4
	BYTE Flag;	// 5
};


void GameProtocol::GCItemDurSend(int aIndex, BYTE pos, BYTE dur, unsigned char flag)
{
	PMSG_ITEMDUR pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x2A, sizeof(pMsg));
	pMsg.IPos = pos;
	pMsg.Dur = dur;
	pMsg.Flag = flag;

	IOCP.DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
}



void GameProtocol::GCItemDurSend2(int aIndex, unsigned char pos, unsigned char dur, unsigned char flag)
{
	PMSG_ITEMDUR pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x2A, sizeof(pMsg));
	pMsg.IPos = pos;
	pMsg.Dur = dur;
	pMsg.Flag = flag;

	IOCP.DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
}


struct PMSG_WEATHER
{
	PBMSG_HEAD h;	// C1:0F
	BYTE Weather;	// 3
};

void GameProtocol::CGWeatherSend(int aIndex, BYTE weather)
{
	PMSG_WEATHER pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0x0F, sizeof(pMsg));
	pMsg.Weather = weather;

	IOCP.DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
}



void GameProtocol::GCServerCmd(int aIndex, BYTE type, BYTE Cmd1, BYTE Cmd2)
{
	PMSG_SERVERCMD ServerCmd;

	PHeadSubSetB((LPBYTE)&ServerCmd, 0xF3, 0x40, sizeof(ServerCmd));
	ServerCmd.CmdType = type;
	ServerCmd.X = Cmd1;
	ServerCmd.Y = Cmd2;

	IOCP.DataSend(aIndex, (UCHAR*)&ServerCmd, ServerCmd.h.size);
}


struct PMSG_RESULT_MOVEDEVILSQUARE
{
	PBMSG_HEAD h;	// C1:90
	BYTE Result;	// 3
};

void GameProtocol::GCReqmoveDevilSquare(PMSG_REQ_MOVEDEVILSQUARE* lpMsg, int aIndex)
{
	if ( aIndex < 0 || aIndex > g_ConfigRead.server.GetObjectMax()-1 )
	{
		g_Log.Add("return %s %d", __FILE__, __LINE__);
		return;
	}

	BYTE cSquareNumber = lpMsg->SquareNumber;
	BYTE cInvitationItemPos = lpMsg->InvitationItemPos - INVENTORY_BAG_START;

	if ( gObj[aIndex].m_IfState.use && gObj[aIndex].m_IfState.type != 12 )
		return;

	PMSG_RESULT_MOVEDEVILSQUARE pResult;

	PHeadSetB((LPBYTE)&pResult, 0x90, sizeof(pResult));
	pResult.Result = 0;

	LPOBJ lpObj = &gObj[aIndex];

	g_Log.Add("[DevilSquare] [%s][%s] Request Move DevilSquare [%d][%d][%d]",
		lpObj->AccountID, lpObj->Name, lpObj->Class, lpObj->Level, cSquareNumber+1);

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

	if ( g_ConfigRead.data.common.PKEnterEventAllow == true )
	{
		bPlayerKiller = FALSE;
	}

	if ( bPlayerKiller == TRUE ) //season 2.5 changed
	{
		pResult.Result = 6;
		IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( MAIN_INVENTORY_RANGE(cInvitationItemPos) == FALSE )
	{
		pResult.Result = 1;
		IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

		g_Log.Add("[DevilSquare] [%s][%s] Move DevilSquare Fail [%d][%d][%d]",
			lpObj->AccountID, lpObj->Name, lpObj->Class, lpObj->Level, cSquareNumber+1);

		return;
	}

	if ( DS_LEVEL_RANGE(cSquareNumber) == FALSE )
	{
		pResult.Result = 1;
		IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

		g_Log.Add("[DevilSquare] [%s][%s] Move DevilSquare Fail [%d][%d][%d]",
			lpObj->AccountID, lpObj->Name, lpObj->Class, lpObj->Level, cSquareNumber+1);

		return;
	}

	if ( g_DevilSquare.GetState() != DevilSquare_OPEN )
	{
		pResult.Result = 2;
		IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

		g_Log.Add("[DevilSquare] [%s][%s] Move DevilSquare Fail [%d][%d][%d]",
			lpObj->AccountID, lpObj->Name, lpObj->Class, lpObj->Level, cSquareNumber+1);

		return;
	}

	if ( g_DevilSquare.GetObjCount(cSquareNumber) >= g_DevilSquare.GetMaxUserInSquare() )
	{
		pResult.Result = 5;
		IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

		g_Log.Add("[DevilSquare] [%s][%s] Move DevilSquare Fail [%d][%d][%d]",
			lpObj->AccountID, lpObj->Name, lpObj->Class, lpObj->Level, cSquareNumber+1);

		return;
	}

	CItem * sitem = &lpObj->pInventory[cInvitationItemPos];

	if ( sitem->m_Type != ITEMGET(14,19) && ( sitem->m_Type != ITEMGET(13,46) || sitem->m_Durability <= 0.0f ) )	// Devil's Invitation
	{
		pResult.Result = 1;
		IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

		g_Log.Add("[DevilSquare] [%s][%s] Move DevilSquare Fail [%d][%d][%d]",
			lpObj->AccountID, lpObj->Name, lpObj->Class, lpObj->Level, cSquareNumber+1);

		return;
	}

	int ItemLevel = sitem->m_Level;

	if ( sitem->m_Type == ITEMGET(13,46) )
		ItemLevel = 10;

	if ( ItemLevel != 0 )
	{
		if ( ItemLevel != 10 )
		{
			if ( ItemLevel != (cSquareNumber+1) )
			{
				pResult.Result = 1;
				IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

				g_Log.Add("[DevilSquare] [%s][%s] Move DevilSquare Invitation LevelError [%d][%d][%d][%d]",
					lpObj->AccountID, lpObj->Name, lpObj->Class, lpObj->Level, cSquareNumber+1, ItemLevel);

				return;
			}
		}
	}

	BOOL bEnterCheck = FALSE;
	WORD movegateindex = 61;	// #gate
	int iENTER_LEVEL = g_DevilSquare.GetUserLevelToEnter(aIndex, movegateindex);

	if ( DS_LEVEL_RANGE(iENTER_LEVEL) == FALSE )
	{
		g_Log.Add("[DevilSquare] [%s][%s] GetUserLevelToEnter() failed",
			lpObj->AccountID, lpObj->Name);

		return;
	}

	if ( ItemLevel == 10 )
		cSquareNumber = iENTER_LEVEL;

	if ( cSquareNumber != iENTER_LEVEL )
	{
		if ( cSquareNumber > iENTER_LEVEL )
		{
			bEnterCheck = 2;
		}
		else
		{
			bEnterCheck = 1;
		}
	}

	if ( bEnterCheck == 1 )
	{
		pResult.Result = 3;
		IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

		g_Log.Add("[DevilSquare] [%s][%s] Move DevilSquare Fail [%d][%d][%d][%d]",
			lpObj->AccountID, lpObj->Name, lpObj->Class, lpObj->Level, cSquareNumber, ItemLevel);

		return;
	}

	if ( bEnterCheck == 2 )
	{
		pResult.Result = 4;
		IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

		g_Log.Add("[DevilSquare] [%s][%s] Move DevilSquare Fail [%d][%d][%d][%d]",
			lpObj->AccountID, lpObj->Name, lpObj->Class, lpObj->Level, cSquareNumber, ItemLevel);

		return;
	}

	g_Log.Add("[DevilSquare] [%s][%s] Move DevilSquare success [%d][%d] RemoveItem[%s][%d][%d][%d]",
		lpObj->AccountID, lpObj->Name, lpObj->Level, cSquareNumber, lpObj->pInventory[cInvitationItemPos].GetName(),
		cInvitationItemPos, ItemLevel, lpObj->pInventory[cInvitationItemPos].m_Number);

	if ( sitem->m_Type == ITEMGET(14,19) || (sitem->m_Type == ITEMGET(13,46) && sitem->m_Durability == 1.0f) )
	{
		gObjInventoryDeleteItem(aIndex, cInvitationItemPos);
		GCInventoryItemDeleteSend(aIndex, cInvitationItemPos, 1);
		IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
	}
	else if ( sitem->m_Type == ITEMGET(13,46) && sitem->m_Durability > 1.0f )
	{
		sitem->m_Durability -= 1.0f;
		GCItemDurSend2(lpObj->m_Index, cInvitationItemPos, sitem->m_Durability, 0);
	}

	lpObj->m_nEventExp = 0;
	lpObj->m_nEventScore = 0;
	lpObj->m_nEventMoney = 0;
	lpObj->m_bDevilSquareIndex = g_DevilSquare.GetDevilSquareIndex(movegateindex);
	lpObj->m_bDevilSquareAuth = true;
	lpObj->m_IfState.state = 0;
	lpObj->m_IfState.type = 0;
	lpObj->m_IfState.use = 0;

	gObjMoveGate(lpObj->m_Index, movegateindex);
	g_DevilSquare.IncObjCount(cSquareNumber);
}

struct PMSG_RESULT_DEVILSQUARE_REMAINTIME
{
	PBMSG_HEAD h;	// C1:91
	BYTE hEventType;	// 3
	BYTE RemainTime;	// 4
	BYTE EnteredUser;	// 5
	BYTE RemainTime_LOW;	// 6
};


void GameProtocol::GCReqDevilSquareRemainTime(PMSG_REQ_DEVILSQUARE_REMAINTIME* lpMsg, int aIndex)
{
	PMSG_RESULT_DEVILSQUARE_REMAINTIME pResult;

	PHeadSetB((LPBYTE)&pResult, 0x91, sizeof(pResult));
	pResult.RemainTime = 0;
	int iENTER_LEVEL;

	switch ( lpMsg->hEventType )
	{
		case 1:
			if ( gObj[aIndex].m_PK_Level >= 4 && g_ConfigRead.data.common.PKEnterEventAllow == false )
			{
				GCServerCmd(aIndex, 0x37, 0, 0);

				g_Log.Add("[PK User][DevilSquare] [%s][%s] Move Fail [PK Level:%d]",
					gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_PK_Level);

				return;
			}

			pResult.hEventType = 1;

			if ( g_DevilSquare.GetState() == DevilSquare_CLOSE )
			{
				pResult.RemainTime = g_DevilSquare.GetRemainTime();
			}
			else if ( g_DevilSquare.GetState() == DevilSquare_OPEN )
			{
				pResult.RemainTime = 0;
			}
			else
			{
				pResult.RemainTime = g_DevilSquare.GetRemainTime() + g_DevilSquare.GetCloseTime();
			}
			break;

		case 2:
			if ( gObj[aIndex].m_PK_Level >= 4 && g_ConfigRead.data.common.PKEnterEventAllow == false )
			{
				GCServerCmd(aIndex, 0x38, 0, 0);

				g_Log.Add("[PK User][BloodCastle] [%s][%s] Move Fail [PK Level:%d]",
					gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_PK_Level);

				return;
			}

			pResult.hEventType = 2;

			if ( g_BloodCastle.GetCurrentState(lpMsg->btItemLevel) == 1 )
			{
				if ( g_BloodCastle.CheckCanEnter(lpMsg->btItemLevel) != false )
				{
					pResult.RemainTime = 0;
				}
				else
				{
					pResult.RemainTime = g_BloodCastle.GetRemainTime(lpMsg->btItemLevel);
				}
			}
			else
			{
				pResult.RemainTime = g_BloodCastle.GetRemainTime(lpMsg->btItemLevel);
			}
			break;

		case 4:
			if ( gObj[aIndex].m_PK_Level >= 4 && g_ConfigRead.data.common.PKEnterEventAllow == false )
			{
				GCServerCmd(aIndex, 0x39, 0, 0);

				g_Log.Add("[PK User][ChaosCastle] [%s][%s] Move Fail [PK Level:%d]",
					gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_PK_Level);

				return;
			}

			iENTER_LEVEL =  g_ChaosCastle.GetUserLevelToEnter(aIndex);

			if ( iENTER_LEVEL == -1 )
				return;

			pResult.hEventType = 4;

			if ( g_ChaosCastle.GetCurrentState(iENTER_LEVEL) == 1 )
			{
				if ( g_ChaosCastle.CheckCanEnter(iENTER_LEVEL) != false )
				{
					pResult.RemainTime = 0;
					pResult.RemainTime_LOW = 0;
					pResult.EnteredUser = g_ChaosCastle.GetCurEnteredUser(iENTER_LEVEL);
				}
				else
				{
					WORD wREMAIN_TIME = g_ChaosCastle.GetRemainTime(iENTER_LEVEL);
					pResult.RemainTime = SET_NUMBERH(wREMAIN_TIME);
					pResult.RemainTime_LOW = SET_NUMBERL(wREMAIN_TIME);
					pResult.EnteredUser = g_ChaosCastle.GetCurEnteredUser(iENTER_LEVEL);
				}
			}
			else
			{
				WORD wREMAIN_TIME = g_ChaosCastle.GetRemainTime(iENTER_LEVEL);
				pResult.RemainTime = SET_NUMBERH(wREMAIN_TIME);
				pResult.RemainTime_LOW = SET_NUMBERL(wREMAIN_TIME);
				pResult.EnteredUser = g_ChaosCastle.GetCurEnteredUser(iENTER_LEVEL);
			}
			break;
		case 5:
			pResult.hEventType = 5;
			pResult.RemainTime = g_IT_Event.GetRemainTime();
			pResult.RemainTime_LOW = 0;
			pResult.EnteredUser = 0;
			break;
		case 6:
			if (gObj[aIndex].m_PK_Level < 4)
			{
				pResult.hEventType = 6;
				IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
			}
			else
			{
				GCServerCmd(aIndex, 0x39, 0, 0);
				g_Log.Add("[PK User][Chaos Castle Survival] [%s][%s] Move Fail [PK Level:%d]",
					gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_PK_Level);

				return;
			}
	}

	IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
}



void GameProtocol::AllSendMsg(LPBYTE Msg, int size)
{
	for ( int n= g_ConfigRead.server.GetObjectStartUserIndex();n<g_ConfigRead.server.GetObjectMax();n++)
	{
		if ( gObj[n].Connected == PLAYER_PLAYING )
		{
			if ( gObj[n].Type == OBJ_USER )
			{
				IOCP.DataSend(n, Msg, size);
			}
		}
	}
}



void GameProtocol::AllSendSameMapMsg(UCHAR * Msg, int size, BYTE mapnumber)
{
	for ( int n=0;n<g_ConfigRead.server.GetObjectMax();n++)
	{
		if ( gObj[n].Connected == PLAYER_PLAYING )
		{
			if ( gObj[n].Type == OBJ_USER )
			{
				if ( gObj[n].MapNumber == mapnumber )
				{
					IOCP.DataSend(n, Msg, size);
				}
			}
		}
	}
}


struct PMSG_PING
{
	PBMSG_HEAD h;	// C1:71
};


//#unused
void GameProtocol::GCSendPing(int aIndex)
{
	PMSG_PING pMsgPing;

	PHeadSetB((LPBYTE)&pMsgPing, 0x71, sizeof(pMsgPing));
	gObj[aIndex].iPingTime = GetTickCount();

	IOCP.DataSend(aIndex, (LPBYTE)&pMsgPing, pMsgPing.h.size);
}


//#unused
void GameProtocol::GCPingSendRecv(PMSG_PING_RESULT* aRecv, int aIndex)
{
	int PingTime = GetTickCount() - gObj[aIndex].iPingTime;

	MsgOutput(aIndex, "Ping: %d ms", PingTime);
}



struct PMSG_REQ_REGISTER_EVENTCHIP
{
	PBMSG_HEAD2 h;	// C1:02
	int iINDEX;	// 4
	BYTE Pos;	// 8
	char szUID[11];	// 9
};

struct PMSG_REQ_REGISTER_STONES
{
	PBMSG_HEAD2 h;	// C1:06
	int iINDEX;	// 4
	BYTE iPosition;	// 8
	char szUID[11];	// 9
};


void GameProtocol::GCRegEventChipRecv(PMSG_REGEVENTCHIP* lpMsg, int aIndex)
{
	if ( !EVENCHIP_TYPE_RANGE(lpMsg->Type) )
		return;

	LPOBJ lpObj = &gObj[aIndex];

	if ( lpObj->m_IfState.type == 1 )
	{
		g_Log.Add("[EventChip] [%s][%s] Attempted ItemCopy using Trade Window",
			lpObj->AccountID, lpObj->Name);

		return;
	}

	if ( lpObj->m_IfState.type == 7 )
	{
		g_Log.Add("[EventChip] [%s][%s] Attempted ItemCopy using ChaosBox Window",
			lpObj->AccountID, lpObj->Name);

		return;
	}

	if ( lpObj->UseEventServer != FALSE )
	{
		PMSG_REGEVENTCHIP_RESULT Result;

		PHeadSetB((LPBYTE)&Result, 0x95, sizeof(Result));
		Result.ChipCount = -1;
		Result.Type = lpMsg->Type;

		g_Log.Add("[EventChip] [%s][%s] Not Found EventChip (RegEventchip) #1 %d",
			lpObj->AccountID, lpObj->Name, lpMsg->ChipPos);

		IOCP.DataSend(aIndex, (LPBYTE)&Result, Result.h.size);

		return;
	}

	int Pos = lpMsg->ChipPos + INVENTORY_BAG_START;
	CItem * sitem = &lpObj->pInventory[Pos];

	if ( !sitem->IsItem() )
	{
		PMSG_REGEVENTCHIP_RESULT Result;

		PHeadSetB((LPBYTE)&Result, 0x95, sizeof(Result));
		Result.ChipCount = -1;
		Result.Type = lpMsg->Type;

		g_Log.Add("[EventChip] [%s][%s] Not Found EventChip (RegEventchip) #2 %d",
			lpObj->AccountID, lpObj->Name, lpMsg->ChipPos);

		IOCP.DataSend(aIndex, (LPBYTE)&Result, Result.h.size);

		return;
	}

	lpObj->UseEventServer = TRUE;

	switch ( lpMsg->Type )
	{
		case 0x00:
			if ( sitem->m_Type == ITEMGET(14,21) && sitem->m_Level == 0 ) // Rena
			{
				PMSG_REQ_REGISTER_EVENTCHIP pMsg;

				PHeadSubSetB((LPBYTE)&pMsg, 0xBE, 0x02, sizeof(pMsg));
				pMsg.iINDEX = aIndex;
				pMsg.Pos = Pos;
				strcpy(pMsg.szUID, lpObj->AccountID);

				wsDataCli.DataSend((PCHAR)&pMsg, sizeof(pMsg));
			}
			else
			{
				PMSG_REGEVENTCHIP_RESULT Result;

				PHeadSetB((LPBYTE)&Result, 0x95, sizeof(Result));
				Result.ChipCount = -1;
				Result.Type = 0x00;

				g_Log.Add("[EventChip] [%s][%s] Not Found EventChip (RegEventchip) #3 %d",
					lpObj->AccountID, lpObj->Name, lpMsg->ChipPos);

				IOCP.DataSend(aIndex, (LPBYTE)&Result, Result.h.size);
				lpObj->UseEventServer = FALSE;
			}
			break;
		case 0x01:
			if ( sitem->m_Type == ITEMGET(14,21) && sitem->m_Level == 1 ) // Stone
			{
				PMSG_REQ_REGISTER_STONES pMsg;

				PHeadSubSetB((LPBYTE)&pMsg, 0xBE, 0x06, sizeof(pMsg));
				pMsg.iINDEX = aIndex;
				pMsg.iPosition = Pos;
				strcpy(pMsg.szUID, lpObj->AccountID);

				wsDataCli.DataSend((PCHAR)&pMsg, sizeof(pMsg));

				g_Log.Add("[Stone] [%s][%s] Register Stone (Stone Pos:%d, Serial:%I64d)",
					lpObj->AccountID, lpObj->Name, lpMsg->ChipPos, sitem->m_Number);
			}
			else
			{
				PMSG_REGEVENTCHIP_RESULT Result;

				PHeadSetB((LPBYTE)&Result, 0x95, sizeof(Result));
				Result.ChipCount = -1;
				Result.Type = 0x01;

				g_Log.Add("[Stone] [%s][%s] Not Found EventChip (Stone Pos: %d)",
					lpObj->AccountID, lpObj->Name, lpMsg->ChipPos);

				IOCP.DataSend(aIndex, (LPBYTE)&Result, Result.h.size);
				lpObj->UseEventServer = FALSE;
			}
			break;
		default:
			lpObj->UseEventServer = FALSE;
			break;
	}
}

struct PMSG_REQ_REGISTER_MUTONUM
{
	PBMSG_HEAD2 h;	// C1:03
	int iINDEX;	// 4
	char szUID[11];	// 8
};

void GameProtocol::GCGetMutoNumRecv(PMSG_GETMUTONUMBER* lpMsg, int aIndex)
{
	if ( gObj[aIndex].MutoNumber != 0 )
	{
		char msg[255];
		wsprintf(msg, Lang.GetText(0,527));
		GCServerMsgStringSend(msg, aIndex, 1);
		return;
	}

	if ( gObj[aIndex].UseEventServer != FALSE )
		return;

	gObj[aIndex].UseEventServer = TRUE;

	if ( !gObjFind10EventChip(aIndex) )
	{
		PMSG_GETMUTONUMBER_RESULT Result;

		PHeadSetB((LPBYTE)&Result, 0x96, sizeof(Result));
		Result.MutoNum[0] = -1;
		Result.MutoNum[1] = 0;
		Result.MutoNum[2] = 0;

		IOCP.DataSend(aIndex, (LPBYTE)&Result, Result.h.size);
		gObj[aIndex].UseEventServer = FALSE;

		return;
	}

	PMSG_REQ_REGISTER_MUTONUM pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0xBE, 0x03, sizeof(pMsg));
	pMsg.iINDEX = aIndex;
	strcpy(pMsg.szUID, gObj[aIndex].AccountID);

	wsDataCli.DataSend((PCHAR)&pMsg, sizeof(pMsg));

	g_Log.Add("[EventChip] [%s][%s] Request MutoNumber",
		gObj[aIndex].AccountID, gObj[aIndex].Name);
}



void GameProtocol::GCUseEndEventChipRescv(int aIndex)
{
	if ( !gObjIsConnectedGP(aIndex))
	{
		g_Log.AddC(TColor::Red,  "error-L3 [%s][%d]", __FILE__, __LINE__);
		return;
	}

	if ( gObj[aIndex].m_IfState.use && gObj[aIndex].m_IfState.type == 9 )
	{
		gObj[aIndex].m_IfState.state = 0;
		gObj[aIndex].m_IfState.type = 0;
		gObj[aIndex].m_IfState.use = 0;
	}
}

struct PMSG_REQ_RESET_EVENTCHIP
{
	PBMSG_HEAD2 h;	// C1:[04:Rena] [09:Stone]
	int iINDEX;	// 4
	char szUID[11];	// 8
};

void GameProtocol::GCUseRenaChangeZenRecv(PMSG_EXCHANGE_EVENTCHIP* lpMsg, int aIndex)
{
	if ( gObj[aIndex].UseEventServer )
		return;

	gObj[aIndex].UseEventServer = TRUE;

	PMSG_REQ_RESET_EVENTCHIP pMsg;

	if ( lpMsg->btType == 1 )	// Stone?
		PHeadSubSetB((LPBYTE)&pMsg, 0xBE, 0x09, sizeof(pMsg));
	else
		PHeadSubSetB((LPBYTE)&pMsg, 0xBE, 0x04, sizeof(pMsg));

	pMsg.iINDEX = aIndex;
	strcpy(pMsg.szUID, gObj[aIndex].AccountID);

	wsDataCli.DataSend((PCHAR)&pMsg, sizeof(pMsg));

	if ( lpMsg->btType == 0x01 )
		g_Log.Add("[EventChip] [%s][%s] Request Change Stones", gObj[aIndex].AccountID, gObj[aIndex].Name);
	else
		g_Log.Add("[EventChip] [%s][%s] Request Change Rena", gObj[aIndex].AccountID, gObj[aIndex].Name);
}

struct PMSG_SEND_QEUSTINFO
{
	PBMSG_HEAD h;	// C1:A0
	BYTE Count;	// 3
	BYTE State[50];	// 4
};

void GameProtocol::CGRequestQuestInfo(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Type != OBJ_USER)
	{
		return;
	}

	if ( lpObj->m_PlayerData->m_SendQuestInfo != 0 )
		return;

	int questcount = g_QuestInfo.GetQeustCount();
	int foundquest = 0;

	PMSG_SEND_QEUSTINFO pMsg;
	LPQUEST_INFO lpQuestInfo;

	memcpy(pMsg.State, lpObj->m_PlayerData->m_Quest, sizeof(pMsg.State));
	int i;
	for (i=0;i<MAX_QUEST_INFO;i++)
	{
		lpQuestInfo = g_QuestInfo.GetQuestInfo(i);

		if ( lpQuestInfo )
		{
			foundquest++;

			if ( foundquest == questcount )
			{
				break;
			}
		}
	}

	int iSize = i/4+5;

	PHeadSetB((LPBYTE)&pMsg, 0xA0, iSize);
	pMsg.Count = i;
	IOCP.DataSend(aIndex, (LPBYTE)&pMsg, iSize);
	lpObj->m_PlayerData->m_SendQuestInfo = true;
}

void GameProtocol::GCSendQuestInfo(int aIndex, int QuestIndex) // Third Quest
{
	if(gObj[aIndex].Type != OBJ_USER)
	{
		return;
	}

	CGRequestQuestInfo(aIndex);
	LPOBJ lpObj = &gObj[aIndex];
	PMSG_SETQUEST pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0xA1, sizeof(pMsg));
	pMsg.QuestIndex = QuestIndex;
	pMsg.State = g_QuestInfo.GetQuestStateBYTE(lpObj, QuestIndex);

	if ( pMsg.State != 0 )
		IOCP.DataSend(aIndex, (UCHAR*)&pMsg, sizeof(pMsg));
}


struct PMSG_SETQUEST_RESULT
{
	PBMSG_HEAD h;	// C1:A2
	BYTE QuestIndex;	// 3
	BYTE Result;	// 4
	BYTE State;	// 5
};


void GameProtocol::CGSetQuestState(PMSG_SETQUEST* lpMsg, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	PMSG_SETQUEST_RESULT pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0xA2, sizeof(pMsg));
	pMsg.QuestIndex = lpMsg->QuestIndex;
	pMsg.Result = g_QuestInfo.SetQuestState(lpObj, lpMsg->QuestIndex, lpMsg->State);
	pMsg.State = g_QuestInfo.GetQuestStateBYTE(lpObj, lpMsg->QuestIndex);

	IOCP.DataSend(aIndex, (UCHAR*)&pMsg, sizeof(pMsg));
}


struct PMSG_SETQUEST_PRIZE
{
	PBMSG_HEAD h;	// C1:A3
	BYTE NumberH;	// 3
	BYTE NumberL;	// 4
	BYTE Type;	// 5
	BYTE Count;	// 6
};


void GameProtocol::GCSendQuestPrize(int aIndex, BYTE Type, BYTE Count)
{
	CGRequestQuestInfo(aIndex);
	LPOBJ lpObj = &gObj[aIndex];

	PMSG_SETQUEST_PRIZE pMsg;

	PHeadSetB((LPBYTE)&pMsg, 0xA3, sizeof(pMsg));
	pMsg.NumberH = SET_NUMBERH(lpObj->m_Index);
	pMsg.NumberL = SET_NUMBERL(lpObj->m_Index);
	pMsg.Type = Type;
	pMsg.Count = Count;

	IOCP.DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
	MsgSendV2(lpObj, (LPBYTE)&pMsg, sizeof(pMsg));
}



void GameProtocol::CGCloseWindow(int aIndex)
{
	if ( gObj[aIndex].m_IfState.use && gObj[aIndex].m_IfState.type )
	{
		if ( gObj[aIndex].m_IfState.type == 7 )
			return;

		if ( gObj[aIndex].m_IfState.type == 10)	// Marlon
		{
			if (gObj[aIndex].TargetNpcNumber != -1 && gObj[gObj[aIndex].TargetNpcNumber].Class == 229)
			{
				gQeustNpcTeleport.TalkRefDel();
				gObj[aIndex].TargetNpcNumber = -1;
			}
		}

		if (gObj[aIndex].TargetNpcNumber != -1 && gObj[gObj[aIndex].TargetNpcNumber].Class == 568)
		{
			gQuestExpNpcTeleport.TalkRefDel();
			gObj[aIndex].TargetNpcNumber = -1;
		}

		if ( gObj[aIndex].m_IfState.type == 3 )
		{
			gObj[aIndex].TargetNpcNumber = -1;
		}

		if ( gObj[aIndex].m_IfState.type == 1 )
		{
			CGTradeCancelButtonRecv(aIndex);
		}

		if ( gObj[aIndex].m_IfState.type == 6 )
		{
			CGWarehouseUseEnd(aIndex);
		}

		if ( gObj[aIndex].m_IfState.type == 13 )
		{
			g_MixSystem.ChaosBoxInit(&gObj[aIndex]);
			gObjInventoryCommit(aIndex);
		}

		gObj[aIndex].m_IfState.state = 0;
		gObj[aIndex].m_IfState.type = 0;
		gObj[aIndex].m_IfState.use = 0;
	}
}

struct PMSG_RESULT_MOVEBLOODCASTLE
{
	PBMSG_HEAD h;	// C1:9A
	BYTE Result;	// 3
};

void GameProtocol::CGRequestEnterBloodCastle(PMSG_REQ_MOVEBLOODCASTLE* lpMsg, int iIndex)
{
	if ( !ObjectMaxRange(iIndex))
	{
		g_Log.Add("return %s %d", __FILE__, __LINE__);
		return;
	}
	
	BYTE btBridgeNumber = lpMsg->iBridgeNumber - 1;
	BYTE btInvisibleCourtItemPos = lpMsg->iItemPos;
	int iITEM_LEVEL = 0;
	UINT64 dwITEM_SERIAL = 0;
	PMSG_RESULT_MOVEBLOODCASTLE pResult;

	PHeadSetB((LPBYTE)&pResult, 0x9A, sizeof(pResult));
	pResult.Result = 0;

	if ( gObj[iIndex].Type != OBJ_USER || gObj[iIndex].Connected <= PLAYER_LOGGED )
		return;

	if ( gObj[iIndex].m_IfState.use && gObj[iIndex].m_IfState.type != 12 )
		return;

	BOOL bPlayerKiller = FALSE; //Season 2.5 add-on
	
	if(gObj[iIndex].PartyNumber >= 0) //Season 2.5 add-on
	{
		if(gParty.GetPKPartyPenalty(gObj[iIndex].PartyNumber) >= 5)
		{
			bPlayerKiller = TRUE;
		}
	}
	else if(gObj[iIndex].m_PK_Level >= 4)
	{
		bPlayerKiller = TRUE;
	}

	if ( g_ConfigRead.data.common.PKEnterEventAllow == true )
	{
		bPlayerKiller = FALSE;
	}

	if ( bPlayerKiller == TRUE )
	{
		pResult.Result = 7;
		IOCP.DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( g_BloodCastle.CheckCanEnter(btBridgeNumber) == false )
	{
		pResult.Result = 2;
		IOCP.DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( gObj[iIndex].pInventory[btInvisibleCourtItemPos].IsItem() == TRUE )
	{
		if ( gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Type == ITEMGET(13,18) )
		{
			iITEM_LEVEL = gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Level;
			dwITEM_SERIAL = gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Number;
			
			if ( BC_CLOACK_LEVEL_RANGE(iITEM_LEVEL)==FALSE)	// #error
			{
				pResult.Result = 1;
				IOCP.DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
				return;
			}
		}
		else if ( gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Type == ITEMGET(13,47) &&
				  gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Durability > 0.0f ) //#unk
		{
			for ( int i=0;i<MAX_CLOACK_LEVEL;i++)
			{
				if ( g_BloodCastle.CheckEnterLevel(iIndex, i+1) == 0 )	// #error
				{
					iITEM_LEVEL = i+1;
					break;
				}
			}

			dwITEM_SERIAL = gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Number;

			if ( BC_CLOACK_LEVEL_RANGE(iITEM_LEVEL) == FALSE )
			{
				pResult.Result = 1;
				IOCP.DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
				return;
			}
		}
		else
		{
			pResult.Result = 1;
			IOCP.DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
			return;
		}
	}
	else
	{
		pResult.Result = 1;
		IOCP.DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}
	
	int iRESULT = g_BloodCastle.CheckEnterLevel(iIndex, iITEM_LEVEL);

	if ( iRESULT== 0 )
	{
		pResult.Result = 0;
		IOCP.DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
	}
	else 
	{
		if ( iRESULT == -1 )
		{
			pResult.Result = 4;
			IOCP.DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
			return;
		}

		if ( iRESULT == 1 )
		{
			pResult.Result = 3;
			IOCP.DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
			return;
		}

		return;
	}
		
	int iBC_INDEX = g_BloodCastle.EnterUserBridge(iITEM_LEVEL-1, iIndex);

	if ( iBC_INDEX == -1 )
	{
		pResult.Result = 5;
		IOCP.DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( (gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Type == ITEMGET(13,18) ||
		(gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Type == ITEMGET(13,47)) &&
		gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Durability == 1.0f ))
	{
		gObjInventoryDeleteItem(iIndex, btInvisibleCourtItemPos);
		GCInventoryItemDeleteSend(iIndex, btInvisibleCourtItemPos, 1);
		IOCP.DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
	}
	else if (gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Type == ITEMGET(13,47) && gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Durability > 1.0f )
	{
		gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Durability -= 1.0f;
		GCItemDurSend2(iIndex, btInvisibleCourtItemPos, gObj[iIndex].pInventory[btInvisibleCourtItemPos].m_Durability, 0);
	}

	gObj[iIndex].m_cBloodCastleIndex = iITEM_LEVEL-1;
	gObj[iIndex].m_cBloodCastleSubIndex = iBC_INDEX;
	gObj[iIndex].m_iBloodCastleEXP = 0;
	gObj[iIndex].m_IfState.state = 0;
	gObj[iIndex].m_IfState.type = 0;
	gObj[iIndex].m_IfState.use = 0;

	g_BloodCastle.SearchUserDeleteQuestItem(iIndex);
	char szTemp[256];
	wsprintf(szTemp, Lang.GetText(0,74), iITEM_LEVEL);
	GCServerMsgStringSend(szTemp, iIndex, 1);

	switch ( iITEM_LEVEL )
	{
		case 1:
			gObjMoveGate(iIndex, 66);
			break;
		case 2:
			gObjMoveGate(iIndex, 67);
			break;
		case 3:
			gObjMoveGate(iIndex, 68);
			break;
		case 4:
			gObjMoveGate(iIndex, 69);
			break;
		case 5:
			gObjMoveGate(iIndex, 70);
			break;
		case 6:
			gObjMoveGate(iIndex, 71);
			break;
		case 7:
			gObjMoveGate(iIndex, 80);
			break;
		case 8:
			gObjMoveGate(iIndex, 271);
			break;
	}

	g_Log.Add("[Blood Castle] (%d) (Account:%s, Name:%s) Entered Blood Castle (Invisible Cloak Serial:%I64d)",
		iITEM_LEVEL, gObj[iIndex].AccountID, gObj[iIndex].Name, dwITEM_SERIAL);
}


struct PMSG_RESULT_MOVECHAOSCASTLE
{
	PBMSG_HEAD2 h;	// C1:AF:01
	BYTE Result;	// 4
};


void GameProtocol::CGRequestEnterChaosCastle(PMSG_REQ_MOVECHAOSCASTLE* lpMsg, int iIndex)
{
	if ( !ObjectMaxRange(iIndex))
	{
		g_Log.Add("return %s %d", __FILE__, __LINE__);
		return;
	}

	BYTE btChaosCastleNumber = lpMsg->iChaosCastleNumber-1;
	BYTE btGuardMailSetItemPos = lpMsg->iItemPos;
	int iITEM_LEVEL=0;
	UINT64 dwITEM_SERIAL=0;
	PMSG_RESULT_MOVECHAOSCASTLE pResult;

	PHeadSubSetB((LPBYTE)&pResult, 0xAF, 0x01, sizeof(pResult));
	pResult.Result = 0;

	if ( gObj[iIndex].Type != OBJ_USER || gObj[iIndex].Connected <= PLAYER_LOGGED )
		return;

	if ( BC_MAP_RANGE(gObj[iIndex].MapNumber) ||
	 	 CC_MAP_RANGE(gObj[iIndex].MapNumber) ||
		 DS_MAP_RANGE(gObj[iIndex].MapNumber) ||
		 IT_MAP_RANGE(gObj[iIndex].MapNumber) ||
		 DG_MAP_RANGE(gObj[iIndex].MapNumber) ||
		 IMPERIAL_MAP_RANGE(gObj[iIndex].MapNumber) ||
		 gObj[iIndex].MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL ||
		 g_ConfigRead.server.GetServerType() == SERVER_ARCA ||
		 g_ConfigRead.server.GetServerType() == SERVER_MARKET)
	{
		PMSG_NOTICE pNotice;
		TNotice::MakeNoticeMsgEx(&pNotice, 1, Lang.GetText(0,118));
		IOCP.DataSend(iIndex, (LPBYTE)&pNotice, pNotice.h.size);
		return;
	}

	if ( gObj[iIndex].m_IfState.use && gObj[iIndex].m_IfState.type != 12 )
		return;

	if (gObj[iIndex].m_PlayerData->m_bUsePartyMatching)
	{
		PMSG_ANS_PARTYMATCHING_ERROR pMsg;
		PHeadSubSetB((LPBYTE)&pMsg, 0xEF, 0x09, sizeof(pMsg));
		pMsg.iResult = -1;
		IOCP.DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);
	}

	if ( g_ConfigRead.pk.bPkPenaltyDisable == FALSE )
	{
		BOOL bPlayerKiller = FALSE; //Season 2.5 add-on
	
		if(gObj[iIndex].PartyNumber >= 0) //Season 2.5 add-on
		{
			if(gParty.GetPKPartyPenalty(gObj[iIndex].PartyNumber) >= 6)
			{
				bPlayerKiller = TRUE;
			}
		}
		else if(gObj[iIndex].m_PK_Level >= 6)
		{
			bPlayerKiller = TRUE;
		}

		if ( bPlayerKiller == TRUE )
		{
			PMSG_NOTICE pNotice;
			TNotice::MakeNoticeMsgEx(&pNotice, 1, Lang.GetText(0,119));
			IOCP.DataSend(iIndex, (LPBYTE)&pNotice, pNotice.h.size);
			return;
		}
	}

	if ( gObjDuelCheck(&gObj[iIndex]) == TRUE )
	{
		PMSG_NOTICE pNotice;
		TNotice::MakeNoticeMsgEx(&pNotice, 1, Lang.GetText(0,120));
		IOCP.DataSend(iIndex, (LPBYTE)&pNotice, pNotice.h.size);
		return;
	}

	if ( gObj[iIndex].m_PlayerData->lpGuild )
	{
		if ( gObj[iIndex].m_PlayerData->lpGuild->WarState == 1 )
		{
			PMSG_NOTICE pNotice;
			TNotice::MakeNoticeMsgEx(&pNotice, 1, Lang.GetText(0,121));
			IOCP.DataSend(iIndex, (LPBYTE)&pNotice, pNotice.h.size);
			return;
		}
	}

	if ( gObj[iIndex].pInventory[gObj[iIndex].m_btInvenPetPos].m_Type == ITEMGET(13,3) ||	// Dino
		 gObj[iIndex].pInventory[gObj[iIndex].m_btInvenPetPos].m_Type == ITEMGET(13,2) ||	// Uniria
		 gObj[iIndex].pInventory[gObj[iIndex].m_btInvenPetPos].m_Type == ITEMGET(13,37) )	// Fenrir
	{
		PMSG_NOTICE pNotice;
		TNotice::MakeNoticeMsgEx(&pNotice, 1, Lang.GetText(0,122));
		IOCP.DataSend(iIndex, (LPBYTE)&pNotice, pNotice.h.size);
		return;
	}

	BOOL bPlayerKiller = FALSE; //Season 2.5 add-on
	
	if(gObj[iIndex].PartyNumber >= 0) //Season 2.5 add-on
	{
		if(gParty.GetPKPartyPenalty(gObj[iIndex].PartyNumber) >= 5)
		{
			bPlayerKiller = TRUE;
		}
	}
	else if(gObj[iIndex].m_PK_Level >= 4)
	{
		bPlayerKiller = TRUE;
	}

	if ( bPlayerKiller == TRUE )
	{
		pResult.Result = 8;
		IOCP.DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	int iENTER_LEVEL = g_ChaosCastle.GetUserLevelToEnter(iIndex);

	if ( iENTER_LEVEL == -1 )
	{
		pResult.Result = 1;
		IOCP.DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( g_ChaosCastle.CheckCanEnter(iENTER_LEVEL) == false )
	{
		pResult.Result = 2;
		IOCP.DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( gObj[iIndex].pInventory[btGuardMailSetItemPos].IsItem() == TRUE )
	{
		if ( gObj[iIndex].pInventory[btGuardMailSetItemPos].m_Type == ITEMGET(13,29) ) // Armor of Guardman
		{
			iITEM_LEVEL = gObj[iIndex].pInventory[btGuardMailSetItemPos].m_Level;
			dwITEM_SERIAL = gObj[iIndex].pInventory[btGuardMailSetItemPos].m_Number;
		}
		else
		{
			pResult.Result = 1;
			IOCP.DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
			return;
		}
	}
	else
	{
		pResult.Result = 1;
		IOCP.DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( g_ChaosCastle.CheckWearingMOPH(iIndex) == TRUE )
	{
		GCServerMsgStringSend(Lang.GetText(0,123), iIndex, 1);
		return;
	}

	int iBC_INDEX = -1;

	if ( g_ChaosCastle.CheckUserEnterMoney(iIndex, iENTER_LEVEL) )
	{
		iBC_INDEX = g_ChaosCastle.EnterUserChaosCastle(iENTER_LEVEL, iIndex);

		if ( iBC_INDEX == -1 )
		{
			pResult.Result = 5;
			IOCP.DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
			return;
		}
	}
	else
	{
		pResult.Result = 7;
		IOCP.DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	BOOL bRES = g_ChaosCastle.PayUserEnterMoney(iIndex, iENTER_LEVEL);

	if ( bRES == FALSE )
	{
		g_ChaosCastle.LeaveUserChaosCastle(iENTER_LEVEL, iIndex);
		pResult.Result = 5;
		IOCP.DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	gObjInventoryDeleteItem(iIndex, btGuardMailSetItemPos);
	GCInventoryItemDeleteSend(iIndex, btGuardMailSetItemPos, 1);

	IOCP.DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);

	gObj[iIndex].m_cChaosCastleIndex = iENTER_LEVEL;
	gObj[iIndex].m_cChaosCastleSubIndex = iBC_INDEX;
	gObj[iIndex].m_IfState.state = 0;
	gObj[iIndex].m_IfState.type = 0;
	gObj[iIndex].m_IfState.use = 0;

	if ( gObj[iIndex].m_bPShopOpen == true )
		g_PersonalStore.CGPShopReqClose(iIndex);

	if ( gObj[iIndex].PartyNumber >= 0 )
	{
		PMSG_PARTYDELUSER pMsg;
		int pindex = gParty.GetIndex(gObj[iIndex].PartyNumber, iIndex, gObj[iIndex].DBNumber);

		if ( pindex >= 0 )
		{
			pMsg.Number = pindex;
			CGPartyDelUser(&pMsg, iIndex, 0);
		}
	}

	if ( gObj[iIndex].Class == CLASS_ELF && gObj[iIndex].m_RecallMon >= 0)
	{
		GCRecallMonLife(iIndex, 60, 0);
		gObjMonsterCallKill(iIndex);
	}

	char szTemp[256];
	wsprintf(szTemp, Lang.GetText(0,124), iENTER_LEVEL+1);
	GCServerMsgStringSend(szTemp, iIndex, 1);

	switch ( iENTER_LEVEL+1 )
	{
		case 1:
			gObjMoveGate(iIndex, 82);
			break;
		case 2:
			gObjMoveGate(iIndex, 83);
			break;
		case 3:
			gObjMoveGate(iIndex, 84);
			break;
		case 4:
			gObjMoveGate(iIndex, 85);
			break;
		case 5:
			gObjMoveGate(iIndex, 86);
			break;
		case 6:
			gObjMoveGate(iIndex, 87);
			break;
		case 7:
			gObjMoveGate(iIndex, 272);
			break;
	}

	g_Log.Add("[Chaos Castle] (%d) (Account:%s, Name:%s) Entered Chaos Castle (Guard Mail Set Serial:%I64d)",
		iENTER_LEVEL+1, gObj[iIndex].AccountID, gObj[iIndex].Name, dwITEM_SERIAL);
}



void GameProtocol::CGRequestRepositionUserInChaosCastle(PMSG_REQ_REPOSUSER_IN_CC * lpMsg, int aIndex)
{
	if ( !gObjIsConnected(aIndex) )
		return;

	if ( !CC_MAP_RANGE(gObj[aIndex].MapNumber) )
		return;

	if ( g_ChaosCastle.ObjSetPosition(aIndex, gObj[aIndex].X, gObj[aIndex].Y) == TRUE )
		gObj[aIndex].m_iChaosCastleBlowTime = GetTickCount();
}



void GameProtocol::CGRequestEventEnterCount(PMSG_REQ_CL_ENTERCOUNT* lpMsg, int aIndex)
{
	if ( !lpMsg )
		return;

	switch ( lpMsg->btEventType )
	{
		case 0x02:
			EGReqBloodCastleEnterCount(aIndex);
			break;
	}
}

struct PMSG_REQ_2ANIV_SERIAL
{
	PBMSG_HEAD2 h;	// C1:08
	int iINDEX;	// 8
	char szUID[MAX_ACCOUNT_LEN+1];	// 8
	char SERIAL1[5];	// 13
	char SERIAL2[5];	// 18
	char SERIAL3[5];	// 1D
	int iMEMB_GUID;	// 24
};

void GameProtocol::CGRequestLottoRegister(PMSG_REQ_2ANV_LOTTO_EVENT* lpMsg, int aIndex)
{
	PMSG_REQ_2ANIV_SERIAL pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0xBE, 0x08, sizeof(pMsg));

	if ( gObj[aIndex].UseEventServer )
		return;

	gObj[aIndex].UseEventServer = TRUE;
	pMsg.iINDEX = aIndex;
	pMsg.iMEMB_GUID = gObj[aIndex].DBNumber;
	memcpy(pMsg.szUID, gObj[aIndex].AccountID, MAX_ACCOUNT_LEN);
	pMsg.szUID[MAX_ACCOUNT_LEN] = 0;
	memcpy(pMsg.SERIAL1, lpMsg->SERIAL1, 4);
	pMsg.SERIAL1[4] = 0;
	memcpy(pMsg.SERIAL2, lpMsg->SERIAL2, 4);
	pMsg.SERIAL2[4] = 0;
	memcpy(pMsg.SERIAL3, lpMsg->SERIAL3, 4);
	pMsg.SERIAL3[4] = 0;

	wsDataCli.DataSend((PCHAR)&pMsg, sizeof(pMsg));

	g_Log.Add("[Mu_2Anv_Event] [%s][%s] Register Lotto Number (Serial: %s-%s-%s)",
		gObj[aIndex].AccountID, gObj[aIndex].Name, pMsg.SERIAL1, pMsg.SERIAL2, pMsg.SERIAL3);
}

struct SDHP_CHARACTER_TRANSFER
{
	PBMSG_HEAD h;	// C1:CF
	char Account[10];	// 3
	short Number;	// E
};


void GameProtocol::CGReqMoveOtherServer(PMSG_REQ_MOVE_OTHERSERVER * lpMsg, int aIndex)
{
	if ( !gEnableServerDivision)
		return;

	if ( !gObjIsConnectedGP(aIndex))
	{
		g_Log.AddC(TColor::Red,  "error-L3 [%s][%d]", __FILE__, __LINE__);
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if ( lpObj->m_MoveOtherServer )
		return;

	lpObj->m_MoveOtherServer = true;

	PMSG_DEFRESULT pResult;
	PHeadSetB((LPBYTE)&pResult, 0x99, sizeof(pResult));
	
	if ( !gObjPasswordCheck(aIndex, lpMsg->LastJoominNumber))
	{
		pResult.result = 0;

		IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		lpObj->m_MoveOtherServer = false;

		g_Log.Add("[CharTrasfer] Fail (JoominNumber) [%s][%s]",
			lpObj->AccountID, lpObj->Name);

		GCServerMsgStringSend(Lang.GetText(0,528), lpObj->m_Index, 1);

		return;
	}

	SDHP_CHARACTER_TRANSFER pCharTransfer;

	GJSetCharacterInfo(lpObj, lpObj->m_Index, FALSE);
	PHeadSetB((LPBYTE)&pCharTransfer, 0xCF, sizeof(pCharTransfer));
	pCharTransfer.Number = aIndex;
	memcpy(pCharTransfer.Account, lpObj->AccountID, sizeof(pCharTransfer.Account));

	wsDataCli.DataSend((PCHAR)&pCharTransfer, sizeof(pCharTransfer));

	g_Log.Add("[CharTrasfer] Request Character Trasfer [%s][%s]",
		lpObj->AccountID, lpObj->Name);
}



void GameProtocol::GCPacketCheckSumRecv(PMSG_PACKETCHECKSUM * aRecv, int aIndex)
{
	gPacketCheckSum.AddCheckSum(aIndex, aRecv->funcindex, aRecv->CheckSum);
}

struct PMSG_GETITEMINFO_FOR_PARTY
{
	PBMSG_HEAD h;	// C1:47
	BYTE NumberH;	// 3
	BYTE NumberL;	// 4
	UINT ItemInfo;	// 8
	BYTE Level;	// C
};

void GameProtocol::GCSendGetItemInfoForParty(int aIndex, CMapItem * lpItem)
{
	LPOBJ lpObj = &gObj[aIndex];

	if(!lpItem)
		return;

	if ( lpObj->PartyNumber >= 0)
	{
		PMSG_GETITEMINFO_FOR_PARTY pMsg;

		pMsg.h.c = 0xC1;
		pMsg.h.headcode = 0x47;
		pMsg.h.size = sizeof(pMsg);
		pMsg.NumberH = SET_NUMBERH(aIndex);
		pMsg.NumberL = SET_NUMBERL(aIndex);
		pMsg.ItemInfo = lpItem->m_Type & (ITEMGET(MAX_TYPE_ITEMS-1, MAX_SUBTYPE_ITEMS-1));

		if ( lpItem->m_Option1 )
			pMsg.ItemInfo |= 0x2000;

		if ( lpItem->m_Option2 )
			pMsg.ItemInfo |= 0x4000;

		if ( lpItem->m_Option3 )
			pMsg.ItemInfo |= 0x8000;

		if ( lpItem->m_NewOption )
			pMsg.ItemInfo |= 0x10000;

		if ( lpItem->m_SetOption )
			pMsg.ItemInfo |= 0x20000;

		if (g_kItemSystemFor380.Is380OptionItem((lpItem != NULL) ? (CItem*)&lpItem->m_Number : NULL))
			pMsg.ItemInfo |= 0x40000;

		pMsg.Level = lpItem->m_Level;
		
		int partycount = gParty.GetPartyCount(lpObj->PartyNumber);

		if ( partycount >= 0 )
		{
			for ( int n=0;n<MAX_USER_IN_PARTY;n++)
			{
				int number = gParty.m_PartyS[lpObj->PartyNumber].Number[n];

				if ( number >= 0 )
				{
					IOCP.DataSend(number, (LPBYTE)&pMsg, pMsg.h.size);
				}
			}
		}
	}
}


struct PMSG_EFFECTINFO
{
	PBMSG_HEAD h;	// C1:48
	BYTE NumberH;	// 3
	BYTE NumberL;	// 4
	BYTE btType;	// 5
};

void GameProtocol::GCSendEffectInfo(int aIndex, BYTE btType)
{
	if ( ObjectMaxRange(aIndex) == FALSE )
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];
	PMSG_EFFECTINFO pMsg;

	pMsg.h.c = 0xC1;
	pMsg.h.headcode = 0x48;
	pMsg.h.size = sizeof(pMsg);
	pMsg.NumberH = SET_NUMBERH(aIndex);
	pMsg.NumberL = SET_NUMBERL(aIndex);
	pMsg.btType = btType;

	if ( btType == 17 )
	{
		IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
	}

	MsgSendV2(lpObj, (LPBYTE)&pMsg, pMsg.h.size);
}



void GameProtocol::CGRequestPetItemCommand(PMSG_REQUEST_PET_ITEM_COMMAND * lpMsg, int aIndex)
{
	if ( !gObjIsConnectedGP(aIndex))
	{
		g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	if ( !ObjectMaxRange(aIndex))
		return;

	LPOBJ lpObj = &gObj[aIndex];

	if ( lpMsg->PetType )
		return;

	int iTargetIndex = MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);
	gDarkSpirit[aIndex- g_ConfigRead.server.GetObjectStartUserIndex()].ChangeCommand(lpMsg->Command, iTargetIndex);
}



void GameProtocol::CGRequestPetItemInfo(PMSG_REQUEST_PET_ITEMINFO * lpMsg, int aIndex)
{
	if ( !gObjIsConnectedGP(aIndex))
	{
		g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	if ( !ObjectMaxRange(aIndex))
		return;

	LPOBJ lpObj = &gObj[aIndex];

	if ( lpMsg->PetType != 0 && lpMsg->PetType != 1 )
		return;
	
	PMSG_SEND_PET_ITEMINFO pMsg;

	pMsg.h.set((LPBYTE)&pMsg, 0xA9, sizeof(pMsg));
	pMsg.PetType = lpMsg->PetType;
	pMsg.InvenType = lpMsg->InvenType;
	pMsg.nPos = lpMsg->nPos;

	if ( lpMsg->InvenType == 0 )	// Inventory
	{
		if ( !INVENTORY_RANGE(lpMsg->nPos))
		{
			g_Log.Add("[PetItem] [%s][%s] RequestPetItem Invalid Iventory nPos %d",
				lpObj->AccountID, lpObj->Name, lpMsg->nPos);

			return;
		}

		if ( lpObj->pInventory[lpMsg->nPos].IsItem() )
		{
			pMsg.Level = lpObj->pInventory[lpMsg->nPos].m_PetItem_Level;
			pMsg.Exp = lpObj->pInventory[lpMsg->nPos].m_PetItem_Exp - gPetItemExp.m_DarkSpiritExpTable[pMsg.Level];
			pMsg.Dur = (BYTE)lpObj->pInventory[lpMsg->nPos].m_Durability;

			IOCP.DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
		}
	}
	else if ( lpMsg->InvenType == 1 )	// Warehouse
	{
		if ( lpObj->LoadWareHouseInfo != false )
		{
			if ( !WAREHOUSE_RANGE(lpMsg->nPos))
			{
				g_Log.Add("[PetItem] [%s][%s] RequestPetItem Invalid WareHouse nPos %d",
					lpObj->AccountID, lpObj->Name, lpMsg->nPos);

				return;
			}

			if ( lpObj->pWarehouse[lpMsg->nPos].IsItem() )
			{
				pMsg.Level = lpObj->pWarehouse[lpMsg->nPos].m_PetItem_Level;
				pMsg.Exp = lpObj->pWarehouse[lpMsg->nPos].m_PetItem_Exp - gPetItemExp.m_DarkSpiritExpTable[pMsg.Level];		
				pMsg.Dur = (BYTE)lpObj->pInventory[lpMsg->nPos].m_Durability;

				IOCP.DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
			}
		}
	}
	else if ( lpMsg->InvenType == 2 )	// Trade
	{
		if ( lpObj->m_IfState.use < 1 || lpObj->m_IfState.type != 1 )
			return;

		if ( !TRADE_BOX_RANGE(lpMsg->nPos))
		{
			g_Log.Add("[PetItem] [%s][%s] RequestPetItem Invalid Trade nPos %d",
				lpObj->AccountID, lpObj->Name, lpMsg->nPos);

			return;
		}

		if ( lpObj->Trade[lpMsg->nPos].IsItem() )
		{
			pMsg.Level = lpObj->Trade[lpMsg->nPos].m_PetItem_Level;
			pMsg.Exp = lpObj->Trade[lpMsg->nPos].m_PetItem_Exp - gPetItemExp.m_DarkSpiritExpTable[pMsg.Level];		
			pMsg.Dur = (BYTE)lpObj->pInventory[lpMsg->nPos].m_Durability;

			IOCP.DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
		}
	}
	else if ( lpMsg->InvenType == 3 )	// Target Trade
	{
		if ( lpObj->m_IfState.use < 1 || lpObj->m_IfState.type != 1 )
			return;

		int iTargetIndex = gObj[aIndex].TargetNumber;

		if ( iTargetIndex < 0 )
			return;

		if ( !ObjectMaxRange(iTargetIndex) )
			return;

		if ( gObj[iTargetIndex].m_IfState.use < 1 || gObj[iTargetIndex].m_IfState.type != 1 )
			return;

		if ( !TRADE_BOX_RANGE(lpMsg->nPos))
		{
			g_Log.Add("[PetItem] [%s][%s] RequestPetItem Invalid TargetTrade nPos %d",
				lpObj->AccountID, lpObj->Name, lpMsg->nPos);

			return;
		}

		if ( gObj[iTargetIndex].Trade[lpMsg->nPos].IsItem() )
		{
			pMsg.Level = gObj[iTargetIndex].Trade[lpMsg->nPos].m_PetItem_Level;
			pMsg.Exp = gObj[iTargetIndex].Trade[lpMsg->nPos].m_PetItem_Exp - gPetItemExp.m_DarkSpiritExpTable[pMsg.Level];
			pMsg.Dur = (BYTE)lpObj->pInventory[lpMsg->nPos].m_Durability;

			IOCP.DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
		}
	}
	else if ( lpMsg->InvenType == 4 )	// Chaos
	{
		if ( !CHAOS_BOX_RANGE(lpMsg->nPos) )
		{
			g_Log.Add("[PetItem] [%s][%s] RequestPetItem Invalid Chaos nPos %d",
				lpObj->AccountID, lpObj->Name, lpMsg->nPos);

			return;
		}

		if ( lpObj->pChaosBox[lpMsg->nPos].IsItem() )
		{
			pMsg.Level = lpObj->pChaosBox[lpMsg->nPos].m_PetItem_Level;
			pMsg.Exp = lpObj->pChaosBox[lpMsg->nPos].m_PetItem_Exp - gPetItemExp.m_DarkSpiritExpTable[pMsg.Level];
			pMsg.Dur = (BYTE)lpObj->pInventory[lpMsg->nPos].m_Durability;

			IOCP.DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
		}
	}
	else if ( lpMsg->InvenType == 5 )	// Personal Shop
	{
		if ( !INVENTORY_RANGE(lpMsg->nPos) )
		{
			g_Log.Add("[PetItem] [%s][%s] RequestPetItem Invalid Iventory nPos %d",
				lpObj->AccountID, lpObj->Name, lpMsg->nPos);

			return;
		}

		if ( lpObj->m_iPShopDealerIndex != -1 )
		{
			if ( ObjectMaxRange(lpObj->m_iPShopDealerIndex))
			{
				if ( gObj[lpObj->m_iPShopDealerIndex].pInventory[lpMsg->nPos].IsItem() )
				{
					pMsg.Level = gObj[lpObj->m_iPShopDealerIndex].pInventory[lpMsg->nPos].m_PetItem_Level;
					pMsg.Exp = gObj[lpObj->m_iPShopDealerIndex].pInventory[lpMsg->nPos].m_PetItem_Exp - gPetItemExp.m_DarkSpiritExpTable[pMsg.Level];
					pMsg.Dur = (BYTE)lpObj->pInventory[lpMsg->nPos].m_Durability;

					IOCP.DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
				}
			}
		}
	}	
}





struct PMSG_ANS_GUILDVIEWPORT
{
	PBMSG_HEAD h;	// C1:66
	int GuildNumber;	// 4
	BYTE btGuildType;	// 8
	BYTE UnionName[8];	// 9
	char GuildName[8];	// 11
	BYTE Mark[32];	// 19
};

void GameProtocol::GCGuildViewportInfo(PMSG_REQ_GUILDVIEWPORT * aRecv, int aIndex)
{
	if ( !gObjIsConnectedGP(aIndex))
	{
		g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	if ( !ObjectMaxRange(aIndex))
		return;

	LPOBJ lpObj = &gObj[aIndex];
	DWORD dwGuildNumber = aRecv->GuildNumber;
	_GUILD_INFO_STRUCT * lpGuildInfo = Guild.SearchGuild_Number(dwGuildNumber);

	if ( lpGuildInfo )
	{
		PMSG_ANS_GUILDVIEWPORT pMsg;

		pMsg.h.c = 0xC1;
		pMsg.h.headcode = 0x66;
		pMsg.h.size = sizeof(pMsg);
		pMsg.GuildNumber = lpGuildInfo->Number;
		pMsg.btGuildType = lpGuildInfo->btGuildType;

		TUnionInfo * pUnionInfo = UnionManager.SearchUnion(lpGuildInfo->iGuildUnion);

		if ( pUnionInfo )
			memcpy(pMsg.UnionName, pUnionInfo->m_szMasterGuild, MAX_GUILD_LEN);
		else
			pMsg.UnionName[0] = '\0';

		memcpy(pMsg.GuildName, lpGuildInfo->Name, MAX_GUILD_LEN);
		memcpy(pMsg.Mark, lpGuildInfo->Mark, sizeof(pMsg.Mark));

		IOCP.DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
	}
	else
	{
		g_Log.Add("GuildViewportInfo Fail : [%s] GuildNumber : %d",
			lpObj->Name, dwGuildNumber);
	}
}





void GameProtocol::CGGuildAssignStatus(PMSG_GUILD_ASSIGN_STATUS_REQ * aRecv, int aIndex)
{
	if ( !gObjIsConnectedGP(aIndex))
	{
		g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	if ( !ObjectMaxRange(aIndex))
		return;

	LPOBJ lpObj = &gObj[aIndex];
	PMSG_GUILD_ASSIGN_STATUS_RESULT pMsg={0};
	
	pMsg.h.set((LPBYTE)&pMsg, 0xE1, sizeof(pMsg));
	pMsg.btType = aRecv->btType;
	memcpy(pMsg.szTagetName, aRecv->szTagetName, sizeof(pMsg.szTagetName));

	if ( lpObj->m_PlayerData->GuildNumber <= 0 || lpObj->m_PlayerData->lpGuild == NULL )
	{
		pMsg.btResult = GUILD_ANS_NOTEXIST_GUILD;
		IOCP.DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
		return;
	}

	if ( gObj[aIndex].m_PlayerData->GuildStatus != G_MASTER )
	{
		pMsg.btResult = GUILD_ANS_NOTEXIST_PERMISSION;
		IOCP.DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
		return;
	}

	char szTargetName[MAX_ACCOUNT_LEN+1]={0};
	memcpy(szTargetName, aRecv->szTagetName, MAX_ACCOUNT_LEN);

	int iArcaBattleState = g_ArcaBattle.GetState();

	if(iArcaBattleState > 2 && iArcaBattleState < 9)
	{
		pMsg.btResult = GUILD_ANS_ARCA_BATTLE_ONGOING;
        IOCP.DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
		return;
	}

	if ( !strcmp(gObj[aIndex].Name, szTargetName))
	{
		pMsg.btResult = GUILD_ANS_NOTEXIST_PERMISSION;
		IOCP.DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
		return;
	}

	if ( lpObj->m_PlayerData->lpGuild == NULL )
		return;

	if ( aRecv->btType == 1 || aRecv->btType == 2 )
	{
		int iSubMasterCount = 0;
		int iBattleMasterCount = 0;

		for ( int n=0;n<MAX_USER_GUILD;n++)
		{
			if ( lpObj->m_PlayerData->lpGuild->Use[n] > 0 )
			{
				if ( lpObj->m_PlayerData->lpGuild->GuildStatus[n] == G_SUB_MASTER )
				{
					iSubMasterCount++;
				}
				else if ( lpObj->m_PlayerData->lpGuild->GuildStatus[n] == G_BATTLE_MASTER )
				{
					iBattleMasterCount++;
				}
			}
		}

		if ( aRecv->btGuildStatus == G_SUB_MASTER )
		{
			if ( iSubMasterCount == 0 )
			{
				GDGuildReqAssignStatus(aIndex, aRecv->btType, aRecv->szTagetName, aRecv->btGuildStatus);
				return;
			}

			pMsg.btResult = GUILD_ANS_NOTEXIST_EXTRA_STATUS;
			IOCP.DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
			return;
		}
		else if ( aRecv->btGuildStatus == G_BATTLE_MASTER )
		{
			if ( iBattleMasterCount < (((lpObj->Level+lpObj->m_PlayerData->MasterLevel) / 200 ) +1) )
			{
				GDGuildReqAssignStatus(aIndex, aRecv->btType, aRecv->szTagetName, aRecv->btGuildStatus);
				return;
			}

			pMsg.btResult = GUILD_ANS_NOTEXIST_EXTRA_STATUS;
			IOCP.DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
			return;
		}
		else if ( aRecv->btGuildStatus == G_PERSON )
		{
			GDGuildReqAssignStatus(aIndex, aRecv->btType, aRecv->szTagetName, aRecv->btGuildStatus);
			return;
		}
		else
		{
			return;
		}
	}
	
	if ( aRecv->btType == 3 )
	{
		GDGuildReqAssignStatus(aIndex, aRecv->btType, aRecv->szTagetName, aRecv->btGuildStatus);
	}
}



void GameProtocol::CGGuildAssignType(PMSG_GUILD_ASSIGN_TYPE_REQ * aRecv, int aIndex)
{
	if ( !gObjIsConnectedGP(aIndex))
	{
		g_Log.Add("error-L2 : Index %s %d", __FILE__, __LINE__);
		return;
	}

	if ( !ObjectMaxRange(aIndex))
		return;

	LPOBJ lpObj = &gObj[aIndex];
	PMSG_GUILD_ASSIGN_TYPE_RESULT pMsg={0};
	
	pMsg.h.set((LPBYTE)&pMsg, 0xE2, sizeof(pMsg));
	pMsg.btGuildType = aRecv->btGuildType;
	
	if ( lpObj->m_PlayerData->GuildNumber <= 0 || lpObj->m_PlayerData->lpGuild == NULL )
	{
		pMsg.btResult = GUILD_ANS_NOTEXIST_GUILD;
		IOCP.DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
		return;
	}

	if ( gObj[aIndex].m_PlayerData->GuildStatus != G_MASTER )
	{
		pMsg.btResult = GUILD_ANS_NOTEXIST_PERMISSION;
		IOCP.DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
		return;
	}
	
	GDGuildReqAssignType(aIndex, aRecv->btGuildType);
}



void GameProtocol::CGRelationShipReqJoinBreakOff(PMSG_RELATIONSHIP_JOIN_BREAKOFF_REQ * aRecv, int aIndex)
{
	if ( g_CastleSiegeSync.GetCastleState() >= 5 && g_CastleSiegeSync.GetCastleState() <= 7 )
	{
		MsgOutput(aIndex, Lang.GetText(0,197));
		return;
	}

	int iTargetUserIndex = MAKE_NUMBERW(aRecv->btTargetUserIndexH, aRecv->btTargetUserIndexL);

	if ( !ObjectMaxRange(aIndex) || !ObjectMaxRange(iTargetUserIndex))
		return;

	if ( !PacketCheckTime(aIndex))
		return;

	if ( gObjIsConnectedGP(iTargetUserIndex) == FALSE )
	{
		GCResultSend(aIndex, 0x51, 3);
		return;
	}
	
	int iArcaBattleState = g_ArcaBattle.GetState();

	if ( iArcaBattleState > 2 && iArcaBattleState < 9 )
	{
		//TRACE_LOG("aIndex, 0x51, 0xA5");
		GCResultSend(aIndex, 0x51, 0xA4);
		return;
	}

	if ( g_NewPVP.IsDuel(gObj[aIndex]) || g_NewPVP.IsDuel(gObj[iTargetUserIndex]) )
	{
		this->GCServerMsgStringSend(Lang.GetText(0,320), aIndex, 1);
		return;
	}

	if ( g_NewPVP.IsObserver(gObj[aIndex]) || g_NewPVP.IsObserver(gObj[iTargetUserIndex]) )
	{
		this->GCServerMsgStringSend(Lang.GetText(0,321), aIndex, 1);
		return;
	}

	if ( !gObj[aIndex].m_PlayerData->lpGuild || !gObj[iTargetUserIndex].m_PlayerData->lpGuild )
	{
		GCResultSend(aIndex, 0x51, 5);
		return;
	}

	if ( gObj[aIndex].m_PlayerData->GuildStatus != G_MASTER && gObj[iTargetUserIndex].m_PlayerData->GuildStatus != G_MASTER)
	{
		GCResultSend(aIndex, 0x51, 4);
		return;
	}

	if ( gObj[aIndex].m_IfState.use > 0 )
	{
		GCResultSend(aIndex, 0x51, 6);
		return;
	}

	if ( gObj[iTargetUserIndex].m_IfState.use > 0 )
	{
		GCResultSend(aIndex, 0x51, 6);
		return;
	}

	PMSG_RELATIONSHIP_JOIN_BREAKOFF_ANS ErrMsg={0};

	PHeadSetB((LPBYTE)&ErrMsg, 0xE6, sizeof(ErrMsg));
	ErrMsg.btResult = 0;
	ErrMsg.btRequestType = aRecv->btRequestType;
	ErrMsg.btRelationShipType = aRecv->btRelationShipType;
	ErrMsg.btTargetUserIndexH = aRecv->btTargetUserIndexH;
	ErrMsg.btTargetUserIndexL = aRecv->btTargetUserIndexL;

	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj = &gObj[iTargetUserIndex];

	if ( lpObj->m_PlayerData->GuildStatus != G_MASTER || lpTargetObj->m_PlayerData->GuildStatus != G_MASTER)
	{
		ErrMsg.btResult = GUILD_ANS_NOTEXIST_PERMISSION;
		IOCP.DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
		return;
	}

	_GUILD_INFO_STRUCT * lpGuildInfo = gObj[aIndex].m_PlayerData->lpGuild;
	_GUILD_INFO_STRUCT * lpTargetGuildInfo = gObj[iTargetUserIndex].m_PlayerData->lpGuild;
	BYTE btRelationShip = gObjGetRelationShip(lpObj, lpTargetObj);

	if ( aRecv->btRequestType == 1 )
	{
		if ( aRecv->btRelationShipType == 1 )
		{
			if ( lpTargetGuildInfo->Count < g_ConfigRead.data.common.AllianceMinGuildMember )
			{
				ErrMsg.btResult = GUILD_ANS_CANNOT_BE_UNION_MASTER;
				IOCP.DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
				return;
			}

			if ( lpTargetGuildInfo->iGuildUnion != 0 && lpTargetGuildInfo->iGuildUnion == lpTargetGuildInfo->Number )	// #error
			{
				if ( UnionManager.GetGuildRelationShipCount(lpTargetGuildInfo->iGuildUnion, 1) >= g_ConfigRead.data.common.AllianceMaxGuilds )
				{
					ErrMsg.btResult = GUILD_ANS_EXCEED_MAX_UNION_MEMBER;
					IOCP.DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
					return;
				}
			}

			char szCastleOwnerGuildName[MAX_GUILD_LEN+1]={0};
			memcpy(szCastleOwnerGuildName, g_CastleSiegeSync.GetCastleOwnerGuild(), MAX_GUILD_LEN);

			if ( !strcmp(lpGuildInfo->Name, szCastleOwnerGuildName))
			{
				ErrMsg.btResult = GUILD_ANS_EXIST_UNION;
				IOCP.DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
				return;
			}

			if ( lpGuildInfo->iGuildUnion )
			{
				ErrMsg.btResult = GUILD_ANS_EXIST_UNION;
				IOCP.DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
				return;
			}

			if ( btRelationShip == 2 )
			{
				ErrMsg.btResult = GUILD_ANS_EXIST_RELATIONSHIP_RIVAL;
				IOCP.DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
				return;
			}

			if ( lpTargetGuildInfo->iGuildUnion && lpTargetGuildInfo->Number != lpTargetGuildInfo->iGuildUnion )	// #error???
			{
				ErrMsg.btResult = GUIDL_ANS_NOT_UNION_MASTER;
				IOCP.DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
				return;
			}

			int UnionMasterInfluence = g_GensSystem.GetGensInfluence(lpTargetObj);
			int GuildMasterInfluence = g_GensSystem.GetGensInfluence(lpObj);

			if(UnionMasterInfluence == NONE_INFLUENCE)
			{
				g_Log.Add("[GensSystem] [%s][%s] Union Joining Failed to Not Join Gens GuildMaster : UnionMaster [%s][%s]", 
					lpObj->AccountID, lpObj->Name, lpTargetObj->AccountID, lpTargetObj->Name);

				ErrMsg.btResult = GUILD_ANS_NOT_UNION_MASTER_GENS;
				IOCP.DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
				return;
			}

			if(GuildMasterInfluence == NONE_INFLUENCE)
			{
				g_Log.Add("[GensSystem] [%s][%s] Union Joining Failed to Not Join Gens GuildMaster", 
					lpObj->AccountID, lpObj->Name);

				ErrMsg.btResult = GUILD_ANS_NOT_GENS;
				IOCP.DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
				return;
			}

			if(UnionMasterInfluence != GuildMasterInfluence && g_GensSystem.CanGensJoinUnionWhileOppositeGens() == FALSE)
			{
				g_Log.Add("[GensSystem] [%s][%s] Union Joining Failed to Gens be Different : UnionMaster [%s][%s] [%s]",
					lpObj->AccountID, lpObj->Name, g_GensSystem.GetGensInfluenceName(lpObj), lpTargetObj->AccountID, lpTargetObj->Name, g_GensSystem.GetGensInfluenceName(lpTargetObj));

				ErrMsg.btResult = GUILD_ANS_WRONG_GENS;
				IOCP.DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
				return;
			}

		}
		else if ( aRecv->btRelationShipType == 2 )
		{
			/*
			if ( lpGuildInfo->iGuildRival || lpTargetGuildInfo->iGuildRival )
			{
				ErrMsg.btResult = GUILD_ANS_EXIST_RIVAL;
				IOCP.DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
				return;
			}*/

			if ( btRelationShip == 1 )
			{
				ErrMsg.btResult = GUILD_ANS_EXIST_RELATIONSHIP_UNION;
				IOCP.DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
				return;
			}
		}
	}
	else if ( aRecv->btRequestType == 2 )
	{
		if ( aRecv->btRelationShipType == 1 )
		{
			if ( lpGuildInfo->iGuildUnion == 0 )
			{
				ErrMsg.btResult = GUILD_ANS_NOTEXIST_UNION;
				IOCP.DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
				return;
			}
		}
		else if ( aRecv->btRelationShipType == 2 )
		{
			if ( !lpGuildInfo->iGuildRival || !lpTargetGuildInfo->iGuildRival )
			{
				ErrMsg.btResult = GUILD_ANS_NOTEXIST_UNION;
				IOCP.DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
				return;
			}

			if ( lpGuildInfo->iGuildRival != lpTargetGuildInfo->Number || lpGuildInfo->Number != lpTargetGuildInfo->iGuildRival )
			{
				ErrMsg.btResult = GUILD_ANS_NOT_GUILD_RIVAL;
				IOCP.DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
				return;
			}
		}
	}		
	
	if ( aRecv->btRequestType == 2 )
	{
		if ( aRecv->btRelationShipType == 1 )
		{
			GDUnionBreakOff(aIndex, lpGuildInfo->iGuildUnion);
			return;
		}
	}

	lpObj->m_IfState.use = 1;
	lpObj->m_IfState.type = 14;
	lpObj->m_IfState.state = 0;
	lpObj->m_InterfaceTime = GetTickCount();

	lpTargetObj->m_IfState.use = 1;
	lpTargetObj->m_IfState.type = 14;
	lpTargetObj->m_IfState.state = 0;
	lpTargetObj->m_InterfaceTime = GetTickCount();

	PMSG_RELATIONSHIP_JOIN_BREAKOFF_REQ pMsg={0};

	PHeadSetB((LPBYTE)&pMsg, 0xE5, sizeof(pMsg));
	pMsg.btTargetUserIndexL = SET_NUMBERL(aIndex);
	pMsg.btTargetUserIndexH = SET_NUMBERH(aIndex);
	pMsg.btRequestType = aRecv->btRequestType;
	pMsg.btRelationShipType = aRecv->btRelationShipType;
	
	IOCP.DataSend(iTargetUserIndex, (LPBYTE)&pMsg, pMsg.h.size);
}



void GameProtocol::CGRelationShipAnsJoinBreakOff(PMSG_RELATIONSHIP_JOIN_BREAKOFF_ANS * aRecv, int aIndex)
{
	int iTargetUserIndex = MAKE_NUMBERW(aRecv->btTargetUserIndexH, aRecv->btTargetUserIndexL);

	if ( !ObjectMaxRange(aIndex) || !ObjectMaxRange(iTargetUserIndex))
		return;

	if ( !PacketCheckTime(aIndex) )
		return;

	if ( gObjIsConnected(&gObj[iTargetUserIndex]) == FALSE )
	{
		GCResultSend(aIndex, 0x51, 3);
		return;
	}

	if ( aRecv->btResult == 1 )
	{
		if ( aRecv->btRequestType == 1 )
		{
			GDRelationShipReqJoin(iTargetUserIndex, aIndex, aRecv->btRelationShipType);
		}
		else if ( aRecv->btRequestType == 2 )
		{
			GDRelationShipReqBreakOff(aIndex, iTargetUserIndex, aRecv->btRelationShipType);
		}
	}
	else
	{
		PMSG_RELATIONSHIP_JOIN_BREAKOFF_ANS ErrMsg ={0};

		PHeadSetB((LPBYTE)&ErrMsg, 0xE6, sizeof(ErrMsg));
		ErrMsg.btResult = 0;
		ErrMsg.btRequestType = aRecv->btRequestType;
		ErrMsg.btRelationShipType = aRecv->btRelationShipType;
		ErrMsg.btTargetUserIndexH = aRecv->btTargetUserIndexH;
		ErrMsg.btTargetUserIndexL = aRecv->btTargetUserIndexL;
		ErrMsg.btResult = GUILD_ANS_CANCLE_REQUEST;

		IOCP.DataSend(iTargetUserIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);
	}

	if ( gObj[aIndex].m_IfState.use )
	{
		if ( gObj[aIndex].m_IfState.type == 14 )
		{
			gObj[aIndex].m_IfState.use = 0;
		}
	}

	if ( gObj[iTargetUserIndex].m_IfState.use )
	{
		if ( gObj[iTargetUserIndex].m_IfState.type == 14 )
		{
			gObj[iTargetUserIndex].m_IfState.use = 0;
		}
	}
}



void GameProtocol::CGUnionList(PMSG_UNIONLIST_REQ * aRecv, int aIndex)
{
	if ( !ObjectMaxRange(aIndex))
		return;

	if ( !PacketCheckTime(aIndex) )
		return;

	LPOBJ lpObj = &gObj[aIndex];

	if ( lpObj == NULL )
	{
		return;
	}

	if ( gObjIsConnected(&gObj[aIndex]) == FALSE )
	{
		GCResultSend(aIndex, 0x51, 3);
		return;
	}

	if ( lpObj->m_PlayerData->lpGuild == NULL )
	{
		GCResultSend(aIndex, 0x51, 3);
		return;
	}

	if ( lpObj->m_PlayerData->lpGuild->iGuildUnion == 0 )
	{
		GCResultSend(aIndex, 0x51, 3);
		return;
	}

	GDUnionListSend(aIndex, lpObj->m_PlayerData->lpGuild->iGuildUnion);

}


void GameProtocol::CGRelationShipReqKickOutUnionMember(PMSG_KICKOUT_UNIONMEMBER_REQ* aRecv, int aIndex)
{
	if ( !ObjectMaxRange(aIndex))
		return;

	if ( this->PacketCheckTime(aIndex) == FALSE )
		return;

	LPOBJ lpObj = &gObj[aIndex];

	if ( lpObj == NULL )
		return;

	if ( gObjIsConnected(&gObj[aIndex]) == FALSE )
	{
		GCResultSend(aIndex, 0x51, 3);
		MsgOutput(aIndex, Lang.GetText(0,529));
		return;
	}

	if ( lpObj->m_PlayerData->lpGuild == NULL )
	{
		GCResultSend(aIndex, 0x51, 3);
		MsgOutput(aIndex, Lang.GetText(0,530));
		return;
	}

	PMSG_RELATIONSHIP_JOIN_BREAKOFF_ANS ErrMsg = {0};

	PHeadSetB((LPBYTE)&ErrMsg, 0xE6, sizeof(ErrMsg));
	ErrMsg.btResult = 0;
	ErrMsg.btRequestType = 2;
	ErrMsg.btRelationShipType = 1;
	ErrMsg.btTargetUserIndexH = 0;
	ErrMsg.btTargetUserIndexL = 0;

	if ( lpObj->m_PlayerData->lpGuild->iGuildUnion == 0 )
	{
		ErrMsg.btResult = GUILD_ANS_NOTEXIST_UNION;
		IOCP.DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);

		return;
	}

	int iArcaBattleState = g_ArcaBattle.GetState();

	if( iArcaBattleState > 2 && iArcaBattleState < 9)
	{
		ErrMsg.btResult = GUILD_ANS_ARCA_BATTLE_ONGOING;
		IOCP.DataSend(aIndex, (LPBYTE)&ErrMsg, sizeof(ErrMsg));
		return;
	}

	if ( lpObj->m_PlayerData->GuildStatus != GUILD_MASTER || lpObj->m_PlayerData->lpGuild->Number != lpObj->m_PlayerData->lpGuild->iGuildUnion)
	{
		ErrMsg.btResult = GUILD_ANS_NOTEXIST_PERMISSION;
		IOCP.DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);

		return;
	}

	char szUnionMemberGuildName[MAX_GUILD_LEN+1] = {0};
	memcpy(szUnionMemberGuildName, aRecv->szTargetGuildName, MAX_GUILD_LEN);

	if ( !strcmp(lpObj->m_PlayerData->lpGuild->Name, szUnionMemberGuildName))
	{
		ErrMsg.btResult = GUILD_ANS_NOTEXIST_PERMISSION;
		IOCP.DataSend(aIndex, (LPBYTE)&ErrMsg, ErrMsg.h.size);

		return;
	}

	GDRelationShipReqKickOutUnionMember(aIndex, szUnionMemberGuildName);
}



void GameProtocol::CGReqMapSvrAuth(PMSG_REQ_MAPSERVERAUTH * lpMsg, int iIndex)
{
	if ( lpMsg->btCliVersion[0] != szClientVersion[0] ||
		 lpMsg->btCliVersion[1] != szClientVersion[1] ||
		 lpMsg->btCliVersion[2] != szClientVersion[2] ||
		 lpMsg->btCliVersion[3] != szClientVersion[3] ||
		 lpMsg->btCliVersion[4] != szClientVersion[4] )
	{
		//GCAnsMapSvrAuth(iIndex, 6);
		//IOCP.CloseClient(iIndex);

		//return;
	}

	char id[MAX_ACCOUNT_LEN+1];
	id[MAX_ACCOUNT_LEN] = 0;
	memcpy(id, lpMsg->szAccountID, MAX_ACCOUNT_LEN);
	BuxConvert(id, MAX_ACCOUNT_LEN);

/*
	BYTE btSerial[17];

	btSerial[16]=0;
	memcpy(btSerial, lpMsg->btCliSerial, sizeof(lpMsg->btCliSerial));

	
	if ( strcmp((char*)btSerial, szGameServerExeSerial) )
	{
		g_Log.AddC(TColor::Red,  "error-L1: Serial error [%s] [%s]", id, btSerial);
		GCAnsMapSvrAuth(iIndex, 6);
		IOCP.CloseClient(iIndex);

		return;
	}*/

	if ( g_ConfigRead.server.IsConnectMemberUse() == TRUE )
	{
		if ( ConMember.IsMember(id) == FALSE )
		{
			GCAnsMapSvrAuth(iIndex, 2);	
			
			return;
		}
	}

	LPOBJ lpObj = &gObj[iIndex];

	if ( PacketCheckTime(iIndex) == FALSE )
	{
		g_Log.Add("error-L2: Login time is over [%d][%s]", iIndex, id);
		IOCP.CloseClient(iIndex);

		return;
	}

	if ( gObj[iIndex].Connected != PLAYER_CONNECTED )
	{
		g_Log.Add("error-L2: Authentication is allowed after connected [%d][%s]", iIndex, id);
		IOCP.CloseClient(iIndex);

		return;
	}

	if ( gObj[iIndex].LoginMsgSnd != 0 )
	{
		if ( gObj[iIndex].LoginMsgSnd == 1 )
		{
			g_Log.Add("error: It is after authentication information is sended [%d][%s][%s]", iIndex, id, gObj[iIndex].m_PlayerData->Ip_addr);
		}
		else
		{
			g_Log.Add("error: %d %s %d", gObj[iIndex].LoginMsgSnd, __FILE__, __LINE__);
		}

		return;
	}

	gObj[iIndex].CheckTick = lpMsg->iTickCount;
	gObj[iIndex].CheckTick2 = GetTickCount();
	gObj[iIndex].ConnectCheckTime = GetTickCount(); 
	gObj[iIndex].CheckSpeedHack = true;
	gObj[iIndex].ConnectCheckTime = GetTickCount();
	gObj[iIndex].LoginMsgSnd = 1;
	gObj[iIndex].LoginMsgCount++;
	gObj[iIndex].AccountID[MAX_ACCOUNT_LEN] = 0;
	memcpy(gObj[iIndex].AccountID, id, MAX_ACCOUNT_LEN);
	memset(gObj[iIndex].Name, 0, MAX_ACCOUNT_LEN);
	memcpy(gObj[iIndex].m_MapServerAuthInfo.szCharName, lpMsg->szCharName, sizeof(lpMsg->szCharName));
	gObj[iIndex].m_MapServerAuthInfo.iJA1 = lpMsg->iJoinAuthCode1;
	gObj[iIndex].m_MapServerAuthInfo.iJA2 = lpMsg->iJoinAuthCode2;
	gObj[iIndex].m_MapServerAuthInfo.iJA3 = lpMsg->iJoinAuthCode3;
	gObj[iIndex].m_MapServerAuthInfo.iJA4 = lpMsg->iJoinAuthCode4;
	gObj[iIndex].m_cAccountItemBlock = 0;
	gObj[iIndex].m_PlayerData->VipType = 0;
	gObj[iIndex].m_PlayerData->VipEffect = VIP_EFFECT_NONE;
	gObj[iIndex].m_PlayerData->AntiHackCheckTime = GetTickCount();
	gObj[iIndex].m_PlayerData->dwLastHitHackTick = GetTickCount64();
	gObjAddMsgSendDelay(lpObj, 1001, iIndex, 5000, 0);
}

struct PMSG_ANS_MAPSERVERAUTH
{
	PBMSG_HEAD2 h;	// C1:B1:01
	BYTE iResult;	// 4
};

void GameProtocol::GCAnsMapSvrAuth(int iIndex, int iResult)
{
	if ( !ObjectMaxRange(iIndex) )
	{
		g_Log.AddC(TColor::Red, "[MapServerMng] Packet Error JG [0x7B] - Index out of bound : %d",iIndex);
		return;
	}

	PMSG_ANS_MAPSERVERAUTH pMsgResult;

	PHeadSubSetB((LPBYTE)&pMsgResult, 0xB1, 0x01, sizeof(pMsgResult));
	pMsgResult.iResult = iResult;

	IOCP.DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
}

void GameProtocol::CGReqCastleSiegeState(PMSG_REQ_CASTLESIEGESTATE * lpMsg, int iIndex) 
{
	GS_GDReqOwnerGuildMaster(g_MapServerManager.GetMapSvrGroup(), iIndex);
}

void GameProtocol::GCAnsCastleSiegeState(int iIndex, int iResult, LPSTR lpszGuildName, LPSTR lpszGuildMaster)
{
	if( (lpszGuildName == NULL) || (lpszGuildMaster == NULL) )
	{
		return;
	}

	PMSG_ANS_CASTLESIEGESTATE pMsgResult;

	pMsgResult.h.set((LPBYTE)&pMsgResult.h, 0xB2, 0x00, sizeof(pMsgResult));

	pMsgResult.btResult = iResult;

	memcpy(pMsgResult.cOwnerGuild, lpszGuildName, sizeof(pMsgResult.cOwnerGuild));
	memcpy(pMsgResult.cOwnerGuildMaster, lpszGuildMaster, sizeof(pMsgResult.cOwnerGuildMaster));

	SYSTEMTIME tmStateStartDate = {0};
	SYSTEMTIME tmStateEndDate = {0};
	SYSTEMTIME tmSiegeStartDate = {0};

	int iCastleState = g_CastleSiege.GetCastleState();
	int bRETVAL = g_CastleSiege.GetCastleStateTerm(&tmStateStartDate, &tmStateEndDate);
	tmSiegeStartDate = g_CastleSiege.GetCastleLeftSiegeDate();

	if( bRETVAL == FALSE )
	{
		iCastleState = -1;
	}

	int iStateLeftSec = g_CastleSiege.GetCurRemainSec();

	// Create Packet
	pMsgResult.cCastleSiegeState = iCastleState;
	// Start Date
	pMsgResult.btStartYearH = SET_NUMBERH(tmStateStartDate.wYear);
	pMsgResult.btStartYearL = SET_NUMBERL(tmStateStartDate.wYear);
	pMsgResult.btStartMonth = tmStateStartDate.wMonth;
	pMsgResult.btStartDay   = tmStateStartDate.wDay;
	pMsgResult.btStartHour  = tmStateStartDate.wHour;
	pMsgResult.btStartMinute= tmStateStartDate.wMinute;
	// End Date
	pMsgResult.btEndYearH = SET_NUMBERH(tmStateEndDate.wYear);
	pMsgResult.btEndYearL = SET_NUMBERL(tmStateEndDate.wYear);
	pMsgResult.btEndMonth = tmStateEndDate.wMonth;
	pMsgResult.btEndDay   = tmStateEndDate.wDay;
	pMsgResult.btEndHour  = tmStateEndDate.wHour;
	pMsgResult.btEndMinute= tmStateEndDate.wMinute;
	// Siege Start Date
	pMsgResult.btSiegeStartYearH = SET_NUMBERH(tmSiegeStartDate.wYear);
	pMsgResult.btSiegeStartYearL = SET_NUMBERL(tmSiegeStartDate.wYear);
	pMsgResult.btSiegeStartMonth = tmSiegeStartDate.wMonth;
	pMsgResult.btSiegeStartDay   = tmSiegeStartDate.wDay;
	pMsgResult.btSiegeStartHour  = tmSiegeStartDate.wHour;
	pMsgResult.btSiegeStartMinute= tmSiegeStartDate.wMinute;
	// Time Left
	pMsgResult.btStateLeftSec1 = SET_NUMBERH(SET_NUMBERHW(iStateLeftSec));
	pMsgResult.btStateLeftSec2 = SET_NUMBERL(SET_NUMBERHW(iStateLeftSec));
	pMsgResult.btStateLeftSec3 = SET_NUMBERH(SET_NUMBERLW(iStateLeftSec));
	pMsgResult.btStateLeftSec4 = SET_NUMBERL(SET_NUMBERLW(iStateLeftSec));

	IOCP.DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
}

void GameProtocol::CGReqRegCastleSiege(PMSG_REQ_REGCASTLESIEGE * lpMsg, int iIndex) 
{
	if( g_CastleSiege.GetCastleState() != CASTLESIEGE_STATE_REGSIEGE )
	{
		GCAnsRegCastleSiege(iIndex, 7, "");
		return;
	}
	if( strcmp(gObj[iIndex].m_PlayerData->GuildName, "") == 0 )
	{
		GCAnsRegCastleSiege(iIndex, 6, "");
		return;
	}
	if( g_CastleSiege.CheckUnionGuildMaster(iIndex) == FALSE )
	{
		GCAnsRegCastleSiege(iIndex, 0, "");
	}
	else if( (g_CastleSiege.CheckCastleOwnerMember(iIndex) != FALSE) || 
		(g_CastleSiege.CheckCastleOwnerUnionMember(iIndex) != FALSE) )
	{
		GCAnsRegCastleSiege(iIndex, 3, "");
	}
	else
	{
		GS_GDReqRegAttackGuild(g_MapServerManager.GetMapSvrGroup(), iIndex);
	}
}

void GameProtocol::GCAnsRegCastleSiege(int iIndex, int iResult, LPSTR lpszGuildName) 
{
	PMSG_ANS_REGCASTLESIEGE pMsgResult;

	if( lpszGuildName == NULL )
	{
		return;
	}

	if( ObjectMaxRange(iIndex) == FALSE )
	{
		return;
	}

	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x01, sizeof(pMsgResult));
	pMsgResult.btResult = iResult;
	memcpy(&pMsgResult.szGuildName, lpszGuildName, sizeof(pMsgResult.szGuildName));

	IOCP.DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);

	if( ::gObjIsConnected(iIndex) )
	{
		g_Log.Add("[CastleSiege] [%s][%s] Registered Castle Siege (GUILD:%s) - Result:%d",	gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].m_PlayerData->GuildName, iResult);
	}
}

void GameProtocol::CGReqGiveUpCastleSiege(PMSG_REQ_GIVEUPCASTLESIEGE * lpMsg, int iIndex) 
{
	if( (g_CastleSiege.GetCastleState() < CASTLESIEGE_STATE_REGSIEGE) || (g_CastleSiege.GetCastleState() > CASTLESIEGE_STATE_REGMARK) ) //Fixed
	{
		GCAnsGiveUpCastleSiege(iIndex, 3, 0, 0, "");
		return;
	}

	if( strcmp(gObj[iIndex].m_PlayerData->GuildName, "") == 0 )
	{
		return;
	}
	
	if( g_CastleSiege.CheckUnionGuildMaster(iIndex) == FALSE )
	{
		return;
	}

	GS_GDReqGuildSetGiveUp(g_MapServerManager.GetMapSvrGroup(), iIndex, lpMsg->btGiveUp);
}

void GameProtocol::GCAnsGiveUpCastleSiege(int iIndex, int iResult, int bGiveUp, int iMarkCount, LPSTR lpszGuildName)
{
	PMSG_ANS_GIVEUPCASTLESIEGE pMsgResult;

	if( lpszGuildName == NULL )
	{
		return;
	}

	if( ObjectMaxRange(iIndex) == FALSE )
	{
		return;
	}

	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x02, sizeof(pMsgResult));
	pMsgResult.btResult = iResult;
	pMsgResult.btIsGiveUp = bGiveUp;

	memcpy(&pMsgResult.szGuildName, lpszGuildName, sizeof(pMsgResult.szGuildName));

	if( (pMsgResult.btResult == 1) && (iMarkCount > 0) )
	{
		gObj[iIndex].m_PlayerData->Money += (iMarkCount*3000);

		if( gObj[iIndex].m_PlayerData->Money > MAX_ZEN ) 
		{
			gObj[iIndex].m_PlayerData->Money = MAX_ZEN;
		}
		GCMoneySend(iIndex, gObj[iIndex].m_PlayerData->Money);
	}

	IOCP.DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);

	if( gObjIsConnected(iIndex) )
	{
		g_Log.Add("[CastleSiege] [%s][%s] GiveUp Castle Siege (GUILD:%s) - Result:%d", gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].m_PlayerData->GuildName, iResult);
	}
}

void GameProtocol::CGReqGuildRegInfo(PMSG_REQ_GUILDREGINFO * lpMsg, int iIndex) 
{
	if( lpMsg == NULL )
	{
		return;
	}

	if( strcmp(gObj[iIndex].m_PlayerData->GuildName, "") == 0 )
	{
		return;
	}

	GS_GDReqGuildMarkRegInfo(g_MapServerManager.GetMapSvrGroup(), iIndex);
}

void GameProtocol::GCAnsGuildRegInfo(int iIndex, int iResult, CSP_ANS_GUILDREGINFO* lpMsgResult)
{
	PMSG_ANS_GUILDREGINFO pMsgResult;

	if( lpMsgResult == NULL )
	{
		return;
	}

	if( ObjectMaxRange(iIndex) == FALSE )
	{
		return;
	}

	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 3, sizeof(pMsgResult));

	pMsgResult.btResult = iResult;
	pMsgResult.btGuildMark1 = SET_NUMBERH(SET_NUMBERHW(lpMsgResult->iRegMarkCount));
	pMsgResult.btGuildMark2 = SET_NUMBERL(SET_NUMBERHW(lpMsgResult->iRegMarkCount));
	pMsgResult.btGuildMark3 = SET_NUMBERH(SET_NUMBERLW(lpMsgResult->iRegMarkCount));
	pMsgResult.btGuildMark4 = SET_NUMBERL(SET_NUMBERLW(lpMsgResult->iRegMarkCount));
	pMsgResult.btRegRank = lpMsgResult->btRegRank;
	pMsgResult.btIsGiveUp = lpMsgResult->bIsGiveUp;
	memcpy(&pMsgResult.szGuildName, lpMsgResult->szGuildName, sizeof(pMsgResult.szGuildName));

	IOCP.DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
}

void GameProtocol::CGReqRegGuildMark(PMSG_REQ_REGGUILDMARK * lpMsg, int iIndex) 
{
	if( lpMsg == NULL )
	{
		return;
	}
	
	if( ObjectMaxRange(iIndex) == FALSE )
	{
		return;
	}
	
	if( gObj[iIndex].UseEventServer == TRUE )
	{
		return;
	}

	gObj[iIndex].UseEventServer = TRUE;

	if( g_CastleSiege.GetCastleState() != CASTLESIEGE_STATE_REGMARK )
	{
		gObj[iIndex].UseEventServer = FALSE;
		return;
	}
	if( strcmp( gObj[iIndex].m_PlayerData->GuildName, "") == 0)
	{
		gObj[iIndex].UseEventServer = FALSE;
		return;
	}
	
	if( CHECK_LIMIT(lpMsg->btItemPos, 64) == FALSE )
	{
		PMSG_ANS_REGGUILDMARK pMsgResult = {0};
		pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x04, sizeof(pMsgResult));
		pMsgResult.btResult = 3;		
		IOCP.DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
		gObj[iIndex].UseEventServer = FALSE;
	}
	else
	{
		int iItemPos = (lpMsg->btItemPos + 12);

		if( gObj[iIndex].pInventory[iItemPos].IsItem() == FALSE )
		{
			PMSG_ANS_REGGUILDMARK pMsgResult = {0};
			pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x04, sizeof(pMsgResult));
			pMsgResult.btResult = 3;		
			IOCP.DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
			gObj[iIndex].UseEventServer = FALSE;
		}
		else
		{
			if( gObj[iIndex].pInventory[iItemPos].m_Type != ITEMGET(14,21) || 
				gObj[iIndex].pInventory[iItemPos].m_Level != 3 || 
				gObj[iIndex].pInventory[iItemPos].m_Durability <= 0.0f )
			{
				PMSG_ANS_REGGUILDMARK pMsgResult = {0};
				pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x04, sizeof(pMsgResult));
				pMsgResult.btResult = 3;		
				IOCP.DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
				gObj[iIndex].UseEventServer = FALSE;
			}
			else
			{
				GS_GDReqRegGuildMark(g_MapServerManager.GetMapSvrGroup(), iIndex, iItemPos);
			}
		}
	}
}

void GameProtocol::GCAnsRegGuildMark(int iIndex, int iResult, CSP_ANS_GUILDREGMARK* lpMsgResult) 
{
	if( lpMsgResult == NULL )
	{
		return;
	}

	if( ObjectMaxRange(iIndex) == FALSE )
	{
		return;
	}

	if( gObj[iIndex].pInventory[lpMsgResult->iItemPos].IsItem() == FALSE )
	{	
		PMSG_ANS_REGGUILDMARK pMsgResult ={0};
		pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x04, sizeof(pMsgResult));
		pMsgResult.btResult = 3;
		IOCP.DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
		gObj[iIndex].UseEventServer = 0;
	}
	else
	{
		if( gObj[iIndex].pInventory[lpMsgResult->iItemPos].m_Type != ITEMGET(14,21) && 
			gObj[iIndex].pInventory[lpMsgResult->iItemPos].m_Level == 3 || 
			gObj[iIndex].pInventory[lpMsgResult->iItemPos].m_Durability <= 0.0f ) 
		{
			PMSG_ANS_REGGUILDMARK pMsgResult = {0};
			pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x04, sizeof(pMsgResult));
			pMsgResult.btResult = 3;
			IOCP.DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
			gObj[iIndex].UseEventServer = FALSE;
		}
		else
		{
			if( (gObj[iIndex].pInventory[lpMsgResult->iItemPos].m_Durability -= 1.0) > 0.0f )
			{
				GCItemDurSend(iIndex, lpMsgResult->iItemPos, gObj[iIndex].pInventory[lpMsgResult->iItemPos].m_Durability, 0);
			}
			else
			{
				gObjInventoryDeleteItem(iIndex, lpMsgResult->iItemPos);
				GCInventoryItemDeleteSend(iIndex,lpMsgResult->iItemPos,1);
			}
			
			PMSG_ANS_REGGUILDMARK pMsgResult;
			pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x04, sizeof(pMsgResult));
			
			pMsgResult.btResult = iResult;
			pMsgResult.btGuildMark1 = SET_NUMBERH(SET_NUMBERHW(lpMsgResult->iRegMarkCount));
			pMsgResult.btGuildMark2 = SET_NUMBERL(SET_NUMBERHW(lpMsgResult->iRegMarkCount));
			pMsgResult.btGuildMark3 = SET_NUMBERH(SET_NUMBERLW(lpMsgResult->iRegMarkCount));
			pMsgResult.btGuildMark4 = SET_NUMBERL(SET_NUMBERLW(lpMsgResult->iRegMarkCount));
			memcpy(&pMsgResult.szGuildName, lpMsgResult->szGuildName, sizeof(pMsgResult.szGuildName));
			IOCP.DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
			gObj[iIndex].UseEventServer = 0;
			
		}
	}
}

void GameProtocol::CGReqNpcBuy(PMSG_REQ_NPCBUY * lpMsg, int iIndex) 
{
	if( lpMsg == NULL )
	{
		return;
	}

	if( ::gObjIsConnected(iIndex) == FALSE )
	{
		return;
	}

	if( strcmp(gObj[iIndex].m_PlayerData->GuildName, "") == 0 )
	{
		return;
	}

	if( g_CastleSiege.CheckGuildOwnCastle(gObj[iIndex].m_PlayerData->GuildName) == FALSE || (gObj[iIndex].m_PlayerData->GuildStatus != G_MASTER && gObj[iIndex].m_PlayerData->GuildStatus != G_SUB_MASTER) )
	{
		g_Log.AddC(TColor::Red, "[CastleSiege] CGReqNpcBuy() ERROR - Authority Fail [%s][%s], Guild:(%s)(%d), Npc:(CLS:%d, IDX:%d)",	gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].m_PlayerData->GuildName, gObj[iIndex].m_PlayerData->GuildStatus,lpMsg->iNpcNumber,lpMsg->iNpcIndex);
	}
	else
	{
		BYTE btResult = 0;

		BOOL bRETVAL = g_CastleSiege.CheckAddDbNPC(iIndex, lpMsg->iNpcNumber, lpMsg->iNpcIndex, btResult);

		if( bRETVAL == FALSE )
		{
			GCAnsNpcBuy(iIndex, btResult, lpMsg->iNpcNumber, lpMsg->iNpcIndex);
			g_Log.AddC(TColor::Red, "[CastleSiege] CGReqNpcBuy() ERROR - CCastleSiege::CheckAddDbNPC() [%s][%s], Guild:(%s)(%d), Npc:(CLS:%d, IDX:%d)", gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].m_PlayerData->GuildName, gObj[iIndex].m_PlayerData->GuildStatus,lpMsg->iNpcNumber,lpMsg->iNpcIndex);
		}
		else
		{
			g_Log.AddC(TColor::Red, "[CastleSiege] CGReqNpcBuy() OK - [%s][%s], Guild:(%s)(%d), Npc:(CLS:%d, IDX:%d)", gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].m_PlayerData->GuildName, gObj[iIndex].m_PlayerData->GuildStatus,lpMsg->iNpcNumber,lpMsg->iNpcIndex);
		}
	}
}

void GameProtocol::GCAnsNpcBuy(int iIndex, int iResult, int iNpcNumber, int iNpcIndex) 
{
	PMSG_ANS_NPCBUY pMsgResult;

	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x05, sizeof(pMsgResult));
	pMsgResult.btResult = iResult;
	pMsgResult.iNpcNumber = iNpcNumber;
	pMsgResult.iNpcIndex = iNpcIndex;
	IOCP.DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
}

void GameProtocol::CGReqNpcRepair(PMSG_REQ_NPCREPAIR * lpMsg, int iIndex) 
{
	if( lpMsg == NULL )
	{
		return;
	}

	if( ::gObjIsConnected(iIndex) == FALSE )
	{
		return;
	}

	if( strcmp(gObj[iIndex].m_PlayerData->GuildName, "") == 0 )
	{
		return;
	}

	if( (g_CastleSiege.CheckGuildOwnCastle(gObj[iIndex].m_PlayerData->GuildName) == FALSE) || (gObj[iIndex].m_PlayerData->GuildStatus != G_MASTER) && (gObj[iIndex].m_PlayerData->GuildStatus != G_SUB_MASTER) )
	{
		g_Log.AddC(TColor::Red, "[CastleSiege] CGReqNpcRepair() ERROR - Authority Fail [%s][%s], Guild:(%s)(%d)", gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].m_PlayerData->GuildName, gObj[iIndex].m_PlayerData->GuildStatus);
		return;
	}

	_CS_NPC_DATA pNpcData;
	BOOL bRETVAL;
	int iNpcIndex;
	int iRepairCost;

	bRETVAL = g_CastleSiege.GetNpcData(lpMsg->iNpcNumber, lpMsg->iNpcIndex,pNpcData);

	if( bRETVAL == FALSE )
	{
		GCAnsNpcRepair(iIndex, 0, lpMsg->iNpcNumber, lpMsg->iNpcIndex, 0, 0);
		g_Log.AddC(TColor::Red, "[CastleSiege] CGReqNpcRepair() ERROR - CL Request Fail [%s][%s], Guild:(%s)(%d), Npc:(CLS:%d, IDX:%d)",	gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].m_PlayerData->GuildName, gObj[iIndex].m_PlayerData->GuildStatus, lpMsg->iNpcNumber,lpMsg->iNpcIndex);
		return;
	}
	else
	{
		iNpcIndex = pNpcData.m_iNPC_OBJINDEX;

		if( gObjIsConnected(iNpcIndex) == FALSE )
		{
			GCAnsNpcRepair(iIndex, 0, lpMsg->iNpcNumber, lpMsg->iNpcIndex, 0, 0);
			g_Log.AddC(TColor::Red, "[CastleSiege] CGReqNpcRepair() ERROR - Npc Alive Data Mismatch [%s][%s], Guild:(%s)(%d), Npc:(CLS:%d, IDX:%d)", gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].m_PlayerData->GuildName, gObj[iIndex].m_PlayerData->GuildStatus, pNpcData.m_iNPC_NUM, pNpcData.m_iNPC_INDEX);
			return;
		}
		else if( gObj[iNpcIndex].Live == 0 )
		{
			GCAnsNpcRepair(iIndex, 0, lpMsg->iNpcNumber, lpMsg->iNpcIndex, 0, 0);
			g_Log.AddC(TColor::Red, "[CastleSiege] CGReqNpcRepair() ERROR - Npc Alive Data Mismatch [%s][%s], Guild:(%s)(%d), Npc:(CLS:%d, IDX:%d)",	gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].m_PlayerData->GuildName, gObj[iIndex].m_PlayerData->GuildStatus, pNpcData.m_iNPC_NUM, pNpcData.m_iNPC_INDEX);
			return;
		}
		else
		{
			if( (gObj[iNpcIndex].MaxLife == gObj[iNpcIndex].Life) || (gObj[iNpcIndex].MaxLife < gObj[iNpcIndex].Life) )
			{
				gObj[iNpcIndex].Life = gObj[iNpcIndex].MaxLife;
				GCAnsNpcRepair(iIndex, 0, lpMsg->iNpcNumber, lpMsg->iNpcIndex, 0, 0);
				return;
			}
			else
			{
				iRepairCost = 0;

				switch( lpMsg->iNpcNumber )
				{
					case 277:
							iRepairCost	= float((gObj[iNpcIndex].MaxLife - gObj[iNpcIndex].Life) * 0.5f) + float(gObj[iNpcIndex].m_btCsNpcDfLevel * 1000000);
						break;
					case 283:
							iRepairCost	= (gObj[iNpcIndex].MaxLife - gObj[iNpcIndex].Life)* 0.3f + float((gObj[iNpcIndex].m_btCsNpcDfLevel + gObj[iNpcIndex].m_btCsNpcRgLevel) * 1000000);
						break;
					default:
						GCAnsNpcRepair(iIndex, 0, lpMsg->iNpcNumber, lpMsg->iNpcIndex, 0, 0);
						return;
				}

				if( gObj[iIndex].m_PlayerData->Money < iRepairCost )
				{
					GCAnsNpcRepair(iIndex, 3, lpMsg->iNpcNumber, lpMsg->iNpcIndex, 0, 0);
					g_Log.AddC(TColor::Red, "[CastleSiege] CGReqNpcRepair() ERROR - Money isn't enough [%s][%s], Guild:(%s)(%d), Npc:(CLS:%d, IDX:%d)", gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].m_PlayerData->GuildName, gObj[iIndex].m_PlayerData->GuildStatus, pNpcData.m_iNPC_NUM, pNpcData.m_iNPC_INDEX);
					return;
				}
				else
				{
					g_Log.AddC(TColor::Red, "[CastleSiege] CGReqNpcRepair() OK - [%s][%s], Guild:(%s)(%d), Npc:(CLS:%d, IDX:%d)", gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].m_PlayerData->GuildName, gObj[iIndex].m_PlayerData->GuildStatus, pNpcData.m_iNPC_NUM, pNpcData.m_iNPC_INDEX);	
					GS_GDReqCastleNpcRepair(g_MapServerManager.GetMapSvrGroup(), iIndex, lpMsg->iNpcNumber, lpMsg->iNpcIndex, iRepairCost);
					//return;
				}
			}
		}
	}	
}

void GameProtocol::GCAnsNpcRepair(int iIndex, int iResult, int iNpcNumber, int iNpcIndex, int iNpcHP, int iNpcMaxHP) 
{
	PMSG_ANS_NPCREPAIR pMsgResult;

	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x06, sizeof(pMsgResult));
	pMsgResult.btResult = iResult;
	pMsgResult.iNpcNumber = iNpcNumber;
	pMsgResult.iNpcIndex = iNpcIndex;
	pMsgResult.iNpcHP = iNpcHP;
	pMsgResult.iNpcMaxHP = iNpcMaxHP;

	IOCP.DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
}



void GameProtocol::CGReqNpcUpgrade(PMSG_REQ_NPCUPGRADE * lpMsg, int iIndex) 
{
	if( lpMsg == NULL )
	{
		return;
	}

	if( ::gObjIsConnected(iIndex) == FALSE )
	{
		return;
	}

	int iNEED_GEMOFDEFEND = 0; 
	int	iNEED_MONEY = 0;
	int	iCUR_UPLVL = 0;
	int	iNXT_UPLVL = 0; 
	int	iNXT_UPVALUE = 0;
	int	bENABLE_UPGRADE = FALSE;

	if( strcmp(gObj[iIndex].m_PlayerData->GuildName, "") == 0 )
		return;

	if( (g_CastleSiege.CheckGuildOwnCastle(gObj[iIndex].m_PlayerData->GuildName) == FALSE) || (gObj[iIndex].m_PlayerData->GuildStatus != G_MASTER && gObj[iIndex].m_PlayerData->GuildStatus != G_SUB_MASTER) )
	{
		g_Log.Add("[CastleSiege] CGReqNpcUpgrade() ERROR - Authority Fail [%s][%s], Guild:(%s)(%d)", gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].m_PlayerData->GuildName, gObj[iIndex].m_PlayerData->GuildStatus);
		return;
	}

	switch( lpMsg->iNpcNumber )
	{
		case 277://Castle Gate
		{
			_CS_NPC_DATA m_NpcData;
			BOOL bResult = g_CastleSiege.GetNpcData(lpMsg->iNpcNumber, lpMsg->iNpcIndex, m_NpcData);

			if( bResult )
			{
				switch( lpMsg->iNpcUpType )
				{
					case 1:
					{
						iCUR_UPLVL = m_NpcData.m_iNPC_DF_LEVEL;
						iNXT_UPLVL = lpMsg->iNpcUpValue;

						if( (iNXT_UPLVL >= 1) && (iNXT_UPLVL <= 3) && (iNXT_UPLVL == (iCUR_UPLVL+1)) )
						{
							bENABLE_UPGRADE = TRUE;
							iNXT_UPVALUE = iNXT_UPLVL;
							iNEED_GEMOFDEFEND =  g_iNpcUpDfLevel_CGATE[(iNXT_UPLVL-1)][1];
							iNEED_MONEY = g_iNpcUpDfLevel_CGATE[(iNXT_UPLVL-1)][2];
						}
					}
					break;

					case 3:
					{
						iCUR_UPLVL = 0;

						for( int i = 0; i < 3; i++ )
						{
							if( m_NpcData.m_iNPC_MAXHP >= *g_iNpcUpMaxHP_CGATE[i] )
							{
								iCUR_UPLVL = (i+1);
							}
						}
						iNXT_UPLVL = 0;

						for( int i = 0; i < 3; i++ )
						{
							if( lpMsg->iNpcUpValue >= *g_iNpcUpMaxHP_CGATE[i] )
							{
								iNXT_UPLVL = (i+1);
							}
						}

						if( (iNXT_UPLVL >= 1) && (iNXT_UPLVL <= 3) && (iNXT_UPLVL == (iCUR_UPLVL+1)) )
						{
							bENABLE_UPGRADE = TRUE;
							iNXT_UPVALUE = g_iNpcUpMaxHP_CGATE[(iNXT_UPLVL-1)][0];
							iNEED_GEMOFDEFEND = g_iNpcUpMaxHP_CGATE[(iNXT_UPLVL-1)][1];
							iNEED_MONEY = g_iNpcUpMaxHP_CGATE[(iNXT_UPLVL-1)][2];
						}
					}
					break;
					default:
						{
							GCAnsNpcUpgrade(iIndex, 5, lpMsg->iNpcNumber, lpMsg->iNpcIndex, lpMsg->iNpcUpType, lpMsg->iNpcUpValue);
							return;
						}
						break;
				}
			}
			else
			{
				GCAnsNpcUpgrade(iIndex, 7, lpMsg->iNpcNumber, lpMsg->iNpcIndex, lpMsg->iNpcUpType, lpMsg->iNpcUpValue);
				return;	
			}
		}
		break;
		case 283://Statue
		{
			_CS_NPC_DATA m_NpcData;

			BOOL bResult;
			
			bResult	= g_CastleSiege.GetNpcData(lpMsg->iNpcNumber, lpMsg->iNpcIndex, m_NpcData);

			if( bResult )
			{
				switch( lpMsg->iNpcUpType )
				{
					case 1:
						{
							iCUR_UPLVL = m_NpcData.m_iNPC_DF_LEVEL;
							iNXT_UPLVL = lpMsg->iNpcUpValue;

							if( (iNXT_UPLVL >= 1) && (iNXT_UPLVL <= 3) && (iNXT_UPLVL == (iCUR_UPLVL+1)) ) //Fixed
							{
								bENABLE_UPGRADE = TRUE;
								iNXT_UPVALUE = iNXT_UPLVL;
								iNEED_GEMOFDEFEND = g_iNpcUpDfLevel_CSTATUE[(iNXT_UPLVL-1)][1];
								iNEED_MONEY = g_iNpcUpDfLevel_CSTATUE[(iNXT_UPLVL-1)][2];
							}
						}
						break;
					case 2:
						{
							iCUR_UPLVL = m_NpcData.m_iNPC_RG_LEVEL;
							iNXT_UPLVL = lpMsg->iNpcUpValue;
							
							if( (iNXT_UPLVL >= 1) && (iNXT_UPLVL <= 3) && (iNXT_UPLVL == (iCUR_UPLVL+1)) )
							{
								bENABLE_UPGRADE = TRUE;
								iNXT_UPVALUE = iNXT_UPLVL;
								iNEED_GEMOFDEFEND = g_iNpcUpRgLevel_CSTATUE[(iNXT_UPLVL-1)][1];
								iNEED_MONEY = g_iNpcUpRgLevel_CSTATUE[(iNXT_UPLVL-1)][2];
							}
						}
						break;
					case 3:
						{
							iCUR_UPLVL = 0;

							for( int i = 0; i < 3; i++ )
							{
								if( m_NpcData.m_iNPC_MAXHP >= *g_iNpcUpMaxHP_CSTATUE[i] )
								{
									iCUR_UPLVL = (i+1);
								}
							}

							iNXT_UPLVL = 0;

							for( int i = 0; i < 3; i++ )
							{
								if( lpMsg->iNpcUpValue >= *g_iNpcUpMaxHP_CSTATUE[i] )
								{
									iNXT_UPLVL = (i+1);
								}
							}

							if( (iNXT_UPLVL >= 1) && (iNXT_UPLVL <= 3) && (iNXT_UPLVL == (iCUR_UPLVL+1)) )
							{
								bENABLE_UPGRADE = TRUE;
								iNXT_UPVALUE = g_iNpcUpMaxHP_CSTATUE[(iNXT_UPLVL-1)][0];
								iNEED_GEMOFDEFEND = g_iNpcUpMaxHP_CSTATUE[(iNXT_UPLVL-1)][1];
								iNEED_MONEY = g_iNpcUpMaxHP_CSTATUE[(iNXT_UPLVL-1)][2];
							}
						}
						break;
					default:
						{
							GCAnsNpcUpgrade(iIndex, 5, lpMsg->iNpcNumber, lpMsg->iNpcIndex, lpMsg->iNpcUpType, lpMsg->iNpcUpValue);
							g_Log.Add("[CastleSiege] CGReqNpcUpgrade() ERROR - UpType doesn't Exist [%s][%s], Guild:(%s)(%d), NPC CLS:%d, IDX:%d, UPTYPE:%d, UPVAL:%d", gObj[iIndex].AccountID,gObj[iIndex].Name,gObj[iIndex].m_PlayerData->GuildName,gObj[iIndex].m_PlayerData->GuildNumber,lpMsg->iNpcNumber,lpMsg->iNpcIndex,lpMsg->iNpcUpType,lpMsg->iNpcUpValue);
							return;
						}
						break;
				}
			}
			else
			{
				GCAnsNpcUpgrade(iIndex, 7, lpMsg->iNpcNumber, lpMsg->iNpcIndex, lpMsg->iNpcUpType, lpMsg->iNpcUpValue);		
				return;
			}
		}
		break;
	}

	int iGEMOFDEFEND_COUNT;
	int x;
	int bRETVAL;

	if( bENABLE_UPGRADE == FALSE )
	{
		GCAnsNpcUpgrade(iIndex, 6, lpMsg->iNpcNumber, lpMsg->iNpcIndex, lpMsg->iNpcUpType, lpMsg->iNpcUpValue);		
	}
	else 
	{
		if( gObj[iIndex].m_PlayerData->Money < iNEED_MONEY )
		{
			GCAnsNpcUpgrade(iIndex, 3, lpMsg->iNpcNumber, lpMsg->iNpcIndex, lpMsg->iNpcUpType, lpMsg->iNpcUpValue);
			g_Log.Add("[CastleSiege] CGReqNpcUpgrade() ERROR - Money is Low [%s][%s], Guild:(%s)(%d), NPC CLS:%d, IDX:%d, UPTYPE:%d, UPVAL:%d, NXTLV:%d, Money:%d, Need Money:%d", gObj[iIndex].AccountID,gObj[iIndex].Name,gObj[iIndex].m_PlayerData->GuildName,gObj[iIndex].m_PlayerData->GuildNumber,lpMsg->iNpcNumber,lpMsg->iNpcIndex,lpMsg->iNpcUpType,lpMsg->iNpcUpValue,iNXT_UPLVL,gObj[iIndex].m_PlayerData->Money,iNEED_MONEY);
		}
		else
		{
			for( iGEMOFDEFEND_COUNT = 0, x = 0; x < INVENTORY_SIZE; x++ )
			{
				if( gObj[iIndex].pInventory[x].IsItem() == TRUE )
				{
					if( gObj[iIndex].pInventory[x].m_Type == ITEMGET(14,31) )
					{
						iGEMOFDEFEND_COUNT += 1;
					}
				}
			}

			if( iGEMOFDEFEND_COUNT < iNEED_GEMOFDEFEND )
			{
				GCAnsNpcUpgrade(iIndex, 4, lpMsg->iNpcNumber, lpMsg->iNpcIndex, lpMsg->iNpcUpType, lpMsg->iNpcUpValue);
				g_Log.Add("[CastleSiege] CGReqNpcUpgrade() ERROR - Gem is Low [%s][%s], Guild:(%s)(%d), NPC CLS:%d, IDX:%d, UPTYPE:%d, UPVAL:%d, NXTLV:%d, Gem:%d, Need Gem:%d",	gObj[iIndex].AccountID,gObj[iIndex].Name,gObj[iIndex].m_PlayerData->GuildName,gObj[iIndex].m_PlayerData->GuildNumber,lpMsg->iNpcNumber,lpMsg->iNpcIndex,lpMsg->iNpcUpType,lpMsg->iNpcUpValue,iNXT_UPLVL,iGEMOFDEFEND_COUNT,iNEED_GEMOFDEFEND);
			}
			else
			{
				bRETVAL = g_CastleSiege.PayForUpgradeDbNpc(iIndex,lpMsg->iNpcNumber,lpMsg->iNpcIndex,lpMsg->iNpcUpType,lpMsg->iNpcUpValue,(iNXT_UPLVL-1));
				
				if( bRETVAL == FALSE )
				{
				}
				else
				{
					gObj[iIndex].m_bIsCastleNPCUpgradeCompleted = TRUE;
					g_Log.Add("[CastleSiege] [0xB2][0x07] CGReqNpcUpgrade() - Pay For Npc Upgrade (CLS:%d, IDX:%d, UPTYPE:%d, UPVAL:%d)", lpMsg->iNpcNumber,lpMsg->iNpcIndex,lpMsg->iNpcUpType,iNXT_UPVALUE);
					GS_GDReqCastleNpcUpgrade(g_MapServerManager.GetMapSvrGroup(),iIndex,lpMsg->iNpcNumber,lpMsg->iNpcIndex,lpMsg->iNpcUpType,iNXT_UPVALUE,(iNXT_UPLVL-1));
					g_Log.Add("[CastleSiege] CGReqNpcUpgrade() REQ OK - [%s][%s], Guild:(%s)(%d), NPC CLS:%d, IDX:%d, UPTYPE:%d, UPVAL:%d, NXTLV:%d", gObj[iIndex].AccountID,gObj[iIndex].Name,gObj[iIndex].m_PlayerData->GuildName,gObj[iIndex].m_PlayerData->GuildNumber,lpMsg->iNpcNumber,lpMsg->iNpcIndex,lpMsg->iNpcUpType,lpMsg->iNpcUpValue,iNXT_UPLVL);
				}	
			}
		}

	}
}

void GameProtocol::GCAnsNpcUpgrade(int iIndex, int iResult, int iNpcNumber, int iNpcIndex, int iNpcUpType, int iNpcUpValue) 
{
	PMSG_ANS_NPCUPGRADE pMsgResult;

	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x07, sizeof(pMsgResult));
	pMsgResult.btResult = iResult;
	pMsgResult.iNpcNumber = iNpcNumber;
	pMsgResult.iNpcIndex = iNpcIndex;
	pMsgResult.iNpcUpType = iNpcUpType;
	pMsgResult.iNpcUpValue = iNpcUpValue;

	IOCP.DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
}

void GameProtocol::CGReqTaxMoneyInfo(PMSG_REQ_TAXMONEYINFO * lpMsg, int iIndex) 
{
	if( lpMsg == NULL )
	{
		return;
	}

	if( strcmp(gObj[iIndex].m_PlayerData->GuildName, "") == 0 )
	{
		return;
	}
	
	if( (g_CastleSiege.CheckGuildOwnCastle(gObj[iIndex].m_PlayerData->GuildName) == FALSE) || (gObj[iIndex].m_PlayerData->GuildStatus != G_MASTER) )
	{
		g_Log.AddC(TColor::Red, "[CastleSiege] CGReqTaxMoneyInfo() ERROR - Authority Fail [%s][%s], Guild:(%s)(%d)", gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].m_PlayerData->GuildName, gObj[iIndex].m_PlayerData->GuildStatus);
	}
	else
	{
		GS_GDReqTaxInfo(g_MapServerManager.GetMapSvrGroup(), iIndex);
		g_Log.Add("[CastleSiege] CGReqTaxMoneyInfo() REQ OK - [%s][%s], Guild:(%s)(%d)",	gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].m_PlayerData->GuildName, gObj[iIndex].m_PlayerData->GuildStatus);
	}
}

void GameProtocol::GCAnsTaxMoneyInfo(int iIndex, int iResult, BYTE btTaxRateChaos, BYTE btTaxRateStore, __int64 i64Money) 
{
	PMSG_ANS_TAXMONEYINFO pMsgResult;

	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x08, sizeof(pMsgResult));
	pMsgResult.btResult = iResult;
	pMsgResult.btTaxRateChaos = btTaxRateChaos;
	pMsgResult.btTaxRateStore = btTaxRateStore;
	pMsgResult.btMoney1 = SET_NUMBERH(SET_NUMBERHW(HIDWORD(i64Money)));
	pMsgResult.btMoney2 = SET_NUMBERL(SET_NUMBERHW(HIDWORD(i64Money)));
	pMsgResult.btMoney3 = SET_NUMBERH(SET_NUMBERLW(HIDWORD(i64Money)));
	pMsgResult.btMoney4 = SET_NUMBERL(SET_NUMBERLW(HIDWORD(i64Money)));
	pMsgResult.btMoney5 = SET_NUMBERH( SET_NUMBERHW( LODWORD( i64Money ) ) ) ;
	pMsgResult.btMoney6 = SET_NUMBERL( SET_NUMBERHW( LODWORD( i64Money ) ) ) ;
	pMsgResult.btMoney7 = SET_NUMBERH( SET_NUMBERLW( LODWORD( i64Money ) ) ) ;
	pMsgResult.btMoney8 = SET_NUMBERL( SET_NUMBERLW( LODWORD( i64Money ) ) ) ;

	IOCP.DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
}

void GameProtocol::CGReqTaxRateChange(PMSG_REQ_TAXRATECHANGE * lpMsg, int iIndex) 
{
	int iMaxTaxRate;
	int iTaxRate;

	if( lpMsg == NULL )
	{
		return;
	}

	if( strcmp(gObj[iIndex].m_PlayerData->GuildName, "") == 0 )
	{
		return;
	}

	if( (g_CastleSiege.CheckGuildOwnCastle(gObj[iIndex].m_PlayerData->GuildName) == FALSE) || (gObj[iIndex].m_PlayerData->GuildStatus != G_MASTER) )
	{
		g_Log.AddC(TColor::Red, "[CastleSiege] CGReqTaxRateChange() ERROR - Authority Fail [%s][%s], Guild:(%s)(%d)",	gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].m_PlayerData->GuildName, gObj[iIndex].m_PlayerData->GuildStatus);
	}
	else
	{
		iMaxTaxRate = 0;

		switch( lpMsg->btTaxType )
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

		iTaxRate = MAKE_NUMBERDW(MAKE_NUMBERW(lpMsg->btTaxRate1, lpMsg->btTaxRate2), MAKE_NUMBERW(lpMsg->btTaxRate3, lpMsg->btTaxRate4)); 

		if( (iTaxRate < 0) || (iTaxRate > iMaxTaxRate) )
		{
			GCAnsTaxRateChange(iIndex,0,0,0);
			g_Log.Add("[CastleSiege] CGReqTaxRateChange() ERROR - Tax Rate Out of Range [%s][%s], Guild:(%s)(%d), TaxType:%d, TaxRate:%d", gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].m_PlayerData->GuildName, gObj[iIndex].m_PlayerData->GuildStatus, lpMsg->btTaxType, iTaxRate);
		}
		else
		{
			GS_GDReqTaxRateChange(g_MapServerManager.GetMapSvrGroup(), iIndex, lpMsg->btTaxType, iTaxRate);
			g_Log.Add("[CastleSiege] CGReqTaxRateChange() REQ OK - [%s][%s], Guild:(%s)(%d), TaxType:%d, TaxRate:%d", gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].m_PlayerData->GuildName, gObj[iIndex].m_PlayerData->GuildStatus, lpMsg->btTaxType, iTaxRate);
		}
	}
}

void GameProtocol::GCAnsTaxRateChange(int iIndex, int iResult, BYTE btTaxType, int iTaxRate) 
{
	PMSG_ANS_TAXRATECHANGE pMsgResult;
	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x09, sizeof(pMsgResult));
	pMsgResult.btResult = iResult;
	pMsgResult.btTaxType = btTaxType;
	pMsgResult.btTaxRate1 = SET_NUMBERH(SET_NUMBERHW(iTaxRate));
	pMsgResult.btTaxRate2 = SET_NUMBERL(SET_NUMBERHW(iTaxRate));
	pMsgResult.btTaxRate3 = SET_NUMBERH(SET_NUMBERLW(iTaxRate));
	pMsgResult.btTaxRate4 = SET_NUMBERL(SET_NUMBERLW(iTaxRate));
	IOCP.DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
}



void GameProtocol::CGReqMoneyDrawOut(PMSG_REQ_MONEYDRAWOUT * lpMsg, int iIndex) 
{
	int iMoneyChange;
	BOOL bRETVAL;

	if( lpMsg == NULL )
	{
		return;
	}

	if( strcmp(gObj[iIndex].m_PlayerData->GuildName, "") == 0 )
	{
		return;
	}
	
	if( (g_CastleSiege.CheckGuildOwnCastle(gObj[iIndex].m_PlayerData->GuildName) == FALSE) || (gObj[iIndex].m_PlayerData->GuildStatus != G_MASTER) )
	{
		g_Log.AddC(TColor::Red, "[CastleSiege] CGReqMoneyDrawOut() ERROR - Authority Fail [%s][%s], Guild:(%s)(%d)", gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].m_PlayerData->GuildName, gObj[iIndex].m_PlayerData->GuildStatus);
	}
	else
	{
		iMoneyChange = MAKE_NUMBERDW(MAKE_NUMBERW(lpMsg->btMoney4, lpMsg->btMoney3), MAKE_NUMBERW(lpMsg->btMoney2, lpMsg->btMoney1));

		if( iMoneyChange <= 0 )
		{
			GCAnsMoneyDrawOut(iIndex, 0, 0);
			g_Log.Add("[CastleSiege] CGReqMoneyDrawOut() ERROR - Req Money < 0 [%s][%s], Guild:(%s)(%d), Money:%d, TotMoney:%d",	gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].m_PlayerData->GuildName, gObj[iIndex].m_PlayerData->GuildStatus, iMoneyChange, gObj[iIndex].m_PlayerData->Money);
		}
		else
		{
			bRETVAL = g_CastleSiege.CheckCastleHasMoney(iMoneyChange);

			if( bRETVAL == FALSE )
			{
				GCAnsMoneyDrawOut(iIndex, 0, 0);
				g_Log.Add("[CastleSiege] CGReqMoneyDrawOut() ERROR - Castle Money is Low [%s][%s], Guild:(%s)(%d), Money:%d, TotMoney:%d", gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].m_PlayerData->GuildName, gObj[iIndex].m_PlayerData->GuildStatus, iMoneyChange, gObj[iIndex].m_PlayerData->Money);
			}
			else
			{
				if( (gObj[iIndex].m_PlayerData->Money + iMoneyChange) > MAX_ZEN )
				{
					GCAnsMoneyDrawOut(iIndex, 0, 0);
				}
				else
				{
					GS_GDReqCastleMoneyChange(g_MapServerManager.GetMapSvrGroup(), iIndex, -iMoneyChange);
					g_Log.Add("[CastleSiege] CGReqMoneyDrawOut() REQ OK - [%s][%s], Guild:(%s)(%d), Money:%d, TotMoney:%d", gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].m_PlayerData->GuildName, gObj[iIndex].m_PlayerData->GuildStatus, iMoneyChange, gObj[iIndex].m_PlayerData->Money);
				}
			}
		}
	}
}

void GameProtocol::GCAnsMoneyDrawOut(int iIndex, int iResult, __int64 i64Money) 
{
	PMSG_ANS_MONEYDRAWOUT pMsgResult;
	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x10, sizeof(pMsgResult));
	pMsgResult.btResult = iResult;
	pMsgResult.btMoney1 = SET_NUMBERH(SET_NUMBERHW(HIDWORD(i64Money)));
	pMsgResult.btMoney2 = SET_NUMBERL(SET_NUMBERHW(HIDWORD(i64Money)));
	pMsgResult.btMoney3 = SET_NUMBERH(SET_NUMBERLW(HIDWORD(i64Money)));
	pMsgResult.btMoney4 = SET_NUMBERL(SET_NUMBERLW(HIDWORD(i64Money)));
	pMsgResult.btMoney5 = SET_NUMBERH( SET_NUMBERHW( LODWORD( i64Money ) ) ) ;
	pMsgResult.btMoney6 = SET_NUMBERL( SET_NUMBERHW( LODWORD( i64Money ) ) ) ;
	pMsgResult.btMoney7 = SET_NUMBERH( SET_NUMBERLW( LODWORD( i64Money ) ) ) ;
	pMsgResult.btMoney8 = SET_NUMBERL( SET_NUMBERLW( LODWORD( i64Money ) ) ) ;
	
	IOCP.DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
}

void GameProtocol::GCAnsCsGateState(int iIndex, int iResult, int iGateIndex) 
{
	PMSG_ANS_CSGATESTATE pMsgResult;
	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x11, sizeof(pMsgResult));
	pMsgResult.btResult = iResult;
	pMsgResult.btIndex1 = SET_NUMBERH( (iGateIndex &0xffff) );
	pMsgResult.btIndex2 = SET_NUMBERL( (iGateIndex &0xffff) );
	IOCP.DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
}

void GameProtocol::CGReqCsGateOperate(PMSG_REQ_CSGATEOPERATE * lpMsg, int iIndex) 
{
	int iGateIndex;

	if( strcmp(gObj[iIndex].m_PlayerData->GuildName, "") == 0 )
	{
		return;
	}
	
	if( g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE )
	{
		if( gObj[iIndex].m_btCsJoinSide != 1 )
		{
			g_Log.Add("[CastleSiege] CGReqCsGateOperate() ERROR - Authority Fail [%s][%s], Guild:(%s)(%d)", gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].m_PlayerData->GuildName, gObj[iIndex].m_PlayerData->GuildStatus);
			return;
		}
	}
	else
	{
		if( (g_CastleSiege.CheckCastleOwnerMember(iIndex) == FALSE) && (g_CastleSiege.CheckCastleOwnerUnionMember(iIndex) == FALSE) )
		{
			g_Log.Add("[CastleSiege] CGReqCsGateOperate() ERROR - Authority Fail [%s][%s], Guild:(%s)(%d)", gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].m_PlayerData->GuildName, gObj[iIndex].m_PlayerData->GuildStatus);
			return;
		}
	}
	
	iGateIndex = MAKE_NUMBERW(lpMsg->btIndex1,lpMsg->btIndex2);

	if( g_CastleSiege.CheckCsGateAlive(iGateIndex) == TRUE ) 
	{
		int iOperateGate = g_CastleSiege.OperateGate(iGateIndex, gObj[iGateIndex].m_iCsNpcExistVal, lpMsg->btOperate);

		if( iOperateGate == 0 )
		{
			GCAnsCsGateOperate(iIndex, 0, -1, 0);
			g_Log.Add("[CastleSiege] CGReqCsGateOperate() ERROR - Operate Gate Failed [%s][%s], Guild:(%s)(%d)",	gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].m_PlayerData->GuildName, gObj[iIndex].m_PlayerData->GuildStatus);	
		}
		else
		{
			GCAnsCsGateOperate(iIndex, 1, iGateIndex, lpMsg->btOperate);
			g_CastleSiege.SendCsGateStateViewPort(iGateIndex, lpMsg->btOperate);
			g_Log.Add("[CastleSiege] CGReqCsGateOperate() OK - [%s][%s], Guild:(%s)(%d), DOOR:(%d)(X:%d,Y:%d)(STATUE:%d)", gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].m_PlayerData->GuildName, gObj[iIndex].m_PlayerData->GuildNumber,iGateIndex, gObj[iGateIndex].X, gObj[iGateIndex].Y, gObj[iGateIndex].m_btCsGateOpen);
		}
	}
	else
	{
		GCAnsCsGateOperate(iIndex, 2, -1, 0);
		g_Log.Add("[CastleSiege] CGReqCsGateOperate() ERROR - Gate Doesn't Exist [%s][%s], Guild:(%s)(%d)", gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].m_PlayerData->GuildName, gObj[iIndex].m_PlayerData->GuildStatus);	
	}
}

void GameProtocol::GCAnsCsGateOperate(int iIndex, int iResult, int iGateIndex, int iGateOperate) 
{
	PMSG_ANS_CSGATEOPERATE pMsgResult;

	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x12, sizeof(pMsgResult));
	pMsgResult.btResult = iResult;
	pMsgResult.btOperate = iGateOperate;
	pMsgResult.btIndex1 = SET_NUMBERH(iGateIndex & 0xffff); //??
	pMsgResult.btIndex2 = SET_NUMBERL(iGateIndex & 0xffff); //??
	IOCP.DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
}

void GameProtocol::GCAnsCsGateCurState(int iIndex, int iGateIndex, int iGateOperate) 
{
	PMSG_ANS_CSGATECURSTATE pMsgResult;
	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x13, sizeof(pMsgResult));
	pMsgResult.btOperate = iGateOperate;
	pMsgResult.btIndex1 = SET_NUMBERH(iGateIndex & 0xffff); //??
	pMsgResult.btIndex2 = SET_NUMBERL(iGateIndex & 0xffff); //??
	IOCP.DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
}

void GameProtocol::GCAnsCsAccessSwitchState(int iIndex, int iSwitchIndex, int iSwitchUserIndex, BYTE btSwitchState) 
{
	PMSG_ANS_NOTIFYSWITCHPROC pMsgResult;
	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x14, sizeof(pMsgResult));
	pMsgResult.btIndex1 = SET_NUMBERH(iSwitchIndex & 0xffff); //??
	pMsgResult.btIndex2 = SET_NUMBERL(iSwitchIndex & 0xffff); //??
	pMsgResult.btUserIndex1 = SET_NUMBERH(iSwitchUserIndex & 0xffff); //??
	pMsgResult.btUserIndex2 = SET_NUMBERL(iSwitchUserIndex & 0xffff); //??
	pMsgResult.btSwitchState = btSwitchState;
	IOCP.DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
}

void GameProtocol::GCAnsCsAccessCrownState(int iIndex, BYTE btCrownState) 
{
	PMSG_ANS_NOTIFYCROWNPROC pMsgResult;
	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x15, sizeof(pMsgResult));
	pMsgResult.btCrownState = btCrownState;

	if( btCrownState == 0 )
	{
		if( gObj[iIndex].m_iAccumulatedCrownAccessTime < 0 || gObj[iIndex].m_iAccumulatedCrownAccessTime > 61000 ) //season4.0 changed
		{
			gObj[iIndex].m_iAccumulatedCrownAccessTime = 0;
		}

		g_Log.Add("[CastleSiege] [Reg. Attempt] Accumulated Crown AccessTime : %d [%s](%s)(%s)",	gObj[iIndex].m_iAccumulatedCrownAccessTime,gObj[iIndex].m_PlayerData->GuildName,gObj[iIndex].AccountID,gObj[iIndex].Name);
	}

	if( btCrownState == 1 )
	{
		g_Log.Add("[CastleSiege] [Reg. Success] Accumulated Crown AccessTime : %d [%s](%s)(%s)",	gObj[iIndex].m_iAccumulatedCrownAccessTime,gObj[iIndex].m_PlayerData->GuildName,gObj[iIndex].AccountID,gObj[iIndex].Name);
		gObj[iIndex].m_iAccumulatedCrownAccessTime = 0;
	}

	if( btCrownState == 2 )
	{
		gObj[iIndex].m_iAccumulatedCrownAccessTime += GetTickCount() - g_CastleSiege.GetCrownAccessTickCount();
		
		if( gObj[iIndex].m_iAccumulatedCrownAccessTime < 0 || gObj[iIndex].m_iAccumulatedCrownAccessTime > 61000 ) //season4.0 changed
		{
			gObj[iIndex].m_iAccumulatedCrownAccessTime = 0;
		}

		if (gObj[iIndex].Live == FALSE)
		{
			if (g_CastleSiege.CastleSiegeResetAccumulationAfterKill == TRUE)
			{
				gObj[iIndex].m_iAccumulatedCrownAccessTime = 0;
			}
		}

		g_Log.Add("[CastleSiege] [Reg. Fail] Accumulated Crown AccessTime : %d [%s](%s)(%s)", gObj[iIndex].m_iAccumulatedCrownAccessTime, gObj[iIndex].m_PlayerData->GuildName, gObj[iIndex].AccountID, gObj[iIndex].Name);
	}

	pMsgResult.dwAccumulatedCrownAccessTime = gObj[iIndex].m_iAccumulatedCrownAccessTime; //??

	IOCP.DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
}
void GameProtocol::GCAnsCsNotifyStart(int iIndex, BYTE btStartState) 
{
	PMSG_ANS_NOTIFYCSSTART pMsgResult;
	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x17, sizeof(pMsgResult));
	pMsgResult.btStartState = btStartState;
	IOCP.DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
}

void GameProtocol::GCAnsCsNotifyProgress(int iIndex, BYTE btCastleSiegeState, LPCSTR lpszGuildName) 
{
	if ( lpszGuildName == NULL )
	{
		return;
	}

	PMSG_ANS_NOTIFYCSPROGRESS pMsgResult;

	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x18, sizeof(pMsgResult));
	pMsgResult.btCastleSiegeState = btCastleSiegeState;
	memset(pMsgResult.szGuildName, 0, sizeof(pMsgResult.szGuildName));
	memcpy(pMsgResult.szGuildName, lpszGuildName, sizeof(pMsgResult.szGuildName));

	IOCP.DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
}

void GameProtocol::GCAnsCsMapSvrTaxInfo(int iIndex, BYTE btTaxType, BYTE btTaxRate) 
{
	PMSG_ANS_MAPSVRTAXINFO pMsgResult;
	
	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x1A, sizeof(pMsgResult));
	pMsgResult.btTaxType = btTaxType;
	pMsgResult.btTaxRate = btTaxRate;

	IOCP.DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);

}

void GameProtocol::CGReqCsMiniMapData(PMSG_REQ_MINIMAPDATA * lpMsg, int iIndex) 
{
	if( gObjIsConnected(iIndex) == FALSE )
	{
		return;
	}

	if( (gObj[iIndex].m_btCsJoinSide <= 0) || ( g_CastleSiege.CheckUnionGuildMaster(iIndex) == FALSE ) )
	{
		GCAnsCsMiniMapData(iIndex, 3);
	}
	else if( g_CastleSiege.GetCastleState() != CASTLESIEGE_STATE_STARTSIEGE )
	{
		GCAnsCsMiniMapData(iIndex, 2);
	}
	else
	{
		g_CastleSiege.AddMiniMapDataReqUser(iIndex);
		GCAnsCsMiniMapData(iIndex, 1);
	}
}

void GameProtocol::GCAnsCsMiniMapData(int iIndex, BYTE btResult) 
{
	PMSG_ANS_MINIMAPDATA pMsgResult;

	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x1B, sizeof(pMsgResult));
	pMsgResult.btResult = btResult;

	IOCP.DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
}


void GameProtocol::CGReqStopCsMiniMapData(PMSG_REQ_STOPMINIMAPDATA * lpMsg, int iIndex) 
{
	if( gObjIsConnected(iIndex) == FALSE )
	{
		return;
	}

}


void GameProtocol::CGReqCsSendCommand(PMSG_REQ_CSCOMMAND * lpMsg, int iIndex) 
{
	if( gObjIsConnected(iIndex) == FALSE )
	{
		return;
	}

	if( gObj[iIndex].m_btCsJoinSide <= 0 ||	g_CastleSiege.CheckUnionGuildMaster(iIndex) == FALSE )
	{
		return;
	}

	if( g_CastleSiege.GetCastleState() != CASTLESIEGE_STATE_STARTSIEGE )
	{
		return;
	}

	GCAnsCsSendCommand(gObj[iIndex].m_btCsJoinSide, lpMsg->btTeam, lpMsg->btX, lpMsg->btY, lpMsg->btCommand);
}

void GameProtocol::GCAnsCsSendCommand(int iCsJoinSize, BYTE btTeam, BYTE btX, BYTE btY, BYTE btCommand) 
{
	PMSG_ANS_CSCOMMAND pMsgResult;

	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x1D, sizeof(pMsgResult));
	
	pMsgResult.btTeam = btTeam;
	pMsgResult.btX = btX;
	pMsgResult.btY = btY;
	pMsgResult.btCommand = btCommand;
	
	for( int iIndex = g_ConfigRead.server.GetObjectStartUserIndex(); iIndex < g_ConfigRead.server.GetObjectMax(); iIndex++ )
	{
		if( gObjIsConnected(iIndex) == FALSE ) continue;

		if( (gObj[iIndex].m_btCsJoinSide == iCsJoinSize) && (gObj[iIndex].MapNumber == MAP_INDEX_CASTLESIEGE) )
		{
			IOCP.DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
		}
	}
}

void GameProtocol::GCAnsCsLeftTimeAlarm(BYTE btHour, BYTE btMinute) 
{
	PMSG_ANS_CSLEFTTIMEALARM pMsgResult;

	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x1E, sizeof(pMsgResult));
	pMsgResult.btHour = btHour;
	pMsgResult.btMinute = btMinute;

	for (int iIndex = g_ConfigRead.server.GetObjectStartUserIndex();iIndex < g_ConfigRead.server.GetObjectMax();iIndex++)
	{
		if ( !gObjIsConnected(iIndex) )
		{
			continue;
		}

		IOCP.DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
	}

}

void GameProtocol::GCAnsSelfCsLeftTimeAlarm(int iIndex, BYTE btHour, BYTE btMinute) 
{
	PMSG_ANS_CSLEFTTIMEALARM pMsgResult;

	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x1E, sizeof(pMsgResult));
	pMsgResult.btHour = btHour;
	pMsgResult.btMinute = btMinute;

	if ( !gObjIsConnected(iIndex))
	{
		return;
	}

	IOCP.DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
}

void GameProtocol::CGReqCsSetEnterHuntZone(PMSG_REQ_CSHUNTZONEENTER * lpMsg, int iIndex) 
{
	if( gObjIsConnected(iIndex) == FALSE )
	{
		return;
	}
	
	if( g_CastleSiege.CheckCastleOwnerMember(iIndex) == FALSE )
	{
		GCAnsCsSetEnterHuntZone(iIndex, 2, lpMsg->btHuntZoneEnter);
	}
	else if( g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE )
	{
		GCAnsCsSetEnterHuntZone(iIndex, 0, lpMsg->btHuntZoneEnter);
	}
	else
	{
		g_CastleSiege.SetHuntZoneEnter(lpMsg->btHuntZoneEnter);
		GCAnsCsSetEnterHuntZone(iIndex, 1, lpMsg->btHuntZoneEnter);
	}
}

void GameProtocol::GCAnsCsSetEnterHuntZone(int iIndex, BYTE btResult, BYTE btEnterHuntZone) 
{
	PMSG_ANS_CSHUNTZONEENTER pMsgResult;

	pMsgResult.h.set((LPBYTE)&pMsgResult, 0xB2, 0x1F, sizeof(pMsgResult));
	pMsgResult.btResult = btResult;
	pMsgResult.btHuntZoneEnter = btEnterHuntZone;

	IOCP.DataSend(iIndex, (LPBYTE)&pMsgResult, pMsgResult.h.size);
}

void GameProtocol::CGReqNpcDbList(PMSG_REQ_NPCDBLIST * lpMsg, int iIndex) 
{
	PMSG_ANS_NPCDBLIST pResult;

	if( lpMsg == NULL )
	{
		return;
	}

	if( strcmp(gObj[iIndex].m_PlayerData->GuildName, "") == 0 )
	{
		return;
	}
	
	if( g_CastleSiege.CheckGuildOwnCastle(gObj[iIndex].m_PlayerData->GuildName) == FALSE || ( (gObj[iIndex].m_PlayerData->GuildStatus != 128) && (gObj[iIndex].m_PlayerData->GuildStatus != 64) ) )
	{
		g_Log.AddC(TColor::Red, "[CastleSiege] CGReqNpcDbList() ERROR - Authority Fail [%s][%s], Guild:(%s)(%d)", gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].m_PlayerData->GuildName, gObj[iIndex].m_PlayerData->GuildStatus);
		pResult.h.set((LPBYTE)&pResult, 0xB3, sizeof(pResult));
		pResult.iCount = 0;
		pResult.btResult = 2;
		IOCP.DataSend(iIndex, (LPBYTE)&pResult, sizeof(pResult));
	}
	else
	{
		switch( lpMsg->btMonsterCode )
		{
		case 1:
			g_CastleSiege.SendNpcStateList(iIndex, 277);
			break;
		case 2:
			g_CastleSiege.SendNpcStateList(iIndex, 283);
			break;
		}
	}
}

void GameProtocol::CGReqCsRegGuildList(PMSG_REQ_CSREGGUILDLIST * lpMsg, int iIndex) 
{
	if( lpMsg == NULL ) return;

	GS_GDReqAllGuildMarkRegInfo(g_MapServerManager.GetMapSvrGroup(), iIndex);
}

void GameProtocol::CGReqCsAttkGuildList(PMSG_REQ_CSATTKGUILDLIST * lpMsg, int iIndex) 
{
	char cBUFFER[1625]; //season 2.5 changed old -> 1022
	PMSG_ANS_CSATTKGUILDLIST* lpMsgSend;
	PMSG_CSATTKGUILDLIST* lpMsgSendBody;
	int iCount;

	if( lpMsg == NULL )	return;
	
	lpMsgSend = (PMSG_ANS_CSATTKGUILDLIST*)(cBUFFER);
	lpMsgSendBody = (PMSG_CSATTKGUILDLIST*)(cBUFFER+sizeof(PMSG_ANS_CSATTKGUILDLIST));

	iCount = 0;

	lpMsgSend->btResult = g_CastleSiege.GetCsAttkGuildList(lpMsgSendBody, iCount);
	lpMsgSend->iCount = iCount;

	if( lpMsgSend->iCount < 0 )	 lpMsgSend->iCount = 0;
	if( lpMsgSend->iCount > 100 )lpMsgSend->iCount = 100;

	lpMsgSend->h.set((LPBYTE)lpMsgSend, 0xB5, ((lpMsgSend->iCount*sizeof(PMSG_CSATTKGUILDLIST))+sizeof(PMSG_ANS_CSATTKGUILDLIST)));
	IOCP.DataSend(iIndex, (LPBYTE)cBUFFER, ((lpMsgSend->iCount*sizeof(PMSG_CSATTKGUILDLIST))+sizeof(PMSG_ANS_CSATTKGUILDLIST)));
}

void GameProtocol::CGReqWeaponUse(PMSG_REQ_USEWEAPON * aRecv, int iIndex) 
{
	WORD wObjIndex = MAKE_NUMBERW(aRecv->btObjIndexH, aRecv->btObjIndexL);

	if ( !ObjectMaxRange(wObjIndex))
	{
		return;
	}

	if ( !ObjectMaxRange(iIndex))
	{
		return;
	}

	LPOBJ lpOwnerObj = &gObj[iIndex];
	LPOBJ lpWeaponObj = &gObj[wObjIndex];

	if ( lpOwnerObj->Type != OBJ_USER )
	{
		return;
	}

	if ( lpWeaponObj->Type != OBJ_NPC )
	{
		return;
	}

	PMSG_ANS_USEWEAPON pMsg = {0};

	PHeadSubSetB((LPBYTE)&pMsg, 0xB7, 0x01, sizeof(pMsg));
	pMsg.btResult = 1;

	if ( lpWeaponObj->Class == 221 )	// Slingshot attack
	{
		pMsg.btWeaponType = 1;
		pMsg.btObjIndexH = SET_NUMBERH(wObjIndex);
		pMsg.btObjIndexL = SET_NUMBERL(wObjIndex);
		g_CsNPC_Weapon.GetTargetPointXY(lpWeaponObj->Class, aRecv->btTargetIndex-1, pMsg.btPointX, pMsg.btPointY, TRUE);
	}
	else if( lpWeaponObj->Class == 222 )	// Slingshot defense
	{
		pMsg.btWeaponType = 2;
		pMsg.btObjIndexH = SET_NUMBERH(wObjIndex);
		pMsg.btObjIndexL = SET_NUMBERL(wObjIndex);
		g_CsNPC_Weapon.GetTargetPointXY(lpWeaponObj->Class, aRecv->btTargetIndex-1, pMsg.btPointX, pMsg.btPointY, TRUE);
	}

	IOCP.DataSend(iIndex, (LPBYTE)&pMsg, sizeof(pMsg));

	pMsg.btResult = 2;

	for ( int n=0;n<MAX_VIEWPORT;n++)
	{
		if ( lpWeaponObj->VpPlayer2[n].type == OBJ_USER )
		{
			if ( lpWeaponObj->VpPlayer2[n].state )
			{
				if ( iIndex != lpWeaponObj->VpPlayer2[n].number )
				{
					IOCP.DataSend(lpWeaponObj->VpPlayer2[n].number, (LPBYTE)&pMsg, sizeof(pMsg));
				}
			}
		}
	}

	gObjNotifyUseWeaponV1(lpOwnerObj, lpWeaponObj, pMsg.btPointX, pMsg.btPointY);
	g_CsNPC_Weapon.SetWeaponCalDamageInfo(wObjIndex, pMsg.btPointX, pMsg.btPointY, 10000);
}

void GameProtocol::CGReqWeaponDamageValue(PMSG_REQ_WEAPON_DAMAGE_VALUE * aRecv, int iIndex) 
{
	if ( !ObjectMaxRange(iIndex) )
	{
		return;
	}

	BYTE btTargetX = 0;
	BYTE btTargetY = 0;
	WORD wObjIndex = MAKE_NUMBERW(aRecv->btObjIndexH, aRecv->btObjIndexL);

	if ( g_CsNPC_Weapon.GetWeaponCalDamageInfo(wObjIndex, btTargetX, btTargetY) == TRUE )
	{
		gObjNotifyUseWeaponDamage(&gObj[wObjIndex], btTargetX, btTargetY);
	}
	else
	{
		MsgOutput(iIndex, Lang.GetText(0,181));
	}
}

void GameProtocol::GCSendObjectCreationState(int iObjectIndex) 
{
	if ( !ObjectMaxRange(iObjectIndex) )
	{
		return;
	}

	LPOBJ lpObj = &gObj[iObjectIndex];
	PMSG_NOTIFY_OBJECT_CREATION_STATE	pMsg = {0};

	if(lpObj->Class == 278)
	{
		PHeadSubSetB((LPBYTE)&pMsg, 0xB9, 0x01, sizeof(pMsg));
		pMsg.btObjIndexH = SET_NUMBERH(iObjectIndex);
		pMsg.btObjIndexL = SET_NUMBERL(iObjectIndex);
		pMsg.btCreationState = lpObj->m_btCreationState;
		MsgSendV2(lpObj, (LPBYTE)&pMsg, sizeof(pMsg)); 
	}
}

void GameProtocol::CGReqGuildMarkOfCastleOwner(PMSG_REQ_GUILDMARK_OF_CASTLEOWNER * aRecv, int iIndex) 
{
	if ( !ObjectMaxRange(iIndex) )
	{
		return;
	}

	LPOBJ lpObj = &gObj[iIndex];
	char szGuildName[MAX_GUILD_LEN+1] = {0};

	memcpy(szGuildName, g_CastleSiegeSync.GetCastleOwnerGuild(), MAX_GUILD_LEN);

	if ( szGuildName )
	{
		_GUILD_INFO_STRUCT * lpGuild = Guild.SearchGuild(szGuildName);

		if ( lpGuild != NULL )
		{
			PMSG_ANS_GUILDMARK_OF_CASTLEOWNER pMsg = {0};

			PHeadSubSetB((LPBYTE)&pMsg, 0xB9, 0x02, sizeof(pMsg));
			memcpy(pMsg.GuildMarkOfCastleOwner, lpGuild->Mark, sizeof(pMsg.GuildMarkOfCastleOwner));

			IOCP.DataSend(iIndex, (LPBYTE)&pMsg, sizeof(pMsg));
		}
	}
}

void GameProtocol::CGReqCastleHuntZoneEntrance(PMSG_REQ_MOVE_TO_CASTLE_HUNTZONE * aRecv, int iIndex) 
{
	if( !ObjectMaxRange(iIndex) )
	{
		return;
	}
	
	LPOBJ lpObj = &gObj[iIndex];

	PMSG_ANS_MOVE_TO_CASTLE_HUNTZONE pMsg = {0};

	PHeadSubSetB((LPBYTE)&pMsg, 0xB9, 0x05, sizeof(pMsg));

	pMsg.btResult = 0;

	int iEnterTaxMoney = 0;

	iEnterTaxMoney = g_CastleSiegeSync.GetTaxHuntZone(lpObj->m_Index, TRUE);

	if( iEnterTaxMoney < 0)
	{
		iEnterTaxMoney = 0;
	}

	BOOL bPermission = FALSE;

	if( g_CastleSiege.GetHuntZoneEnter() )
	{
		bPermission = TRUE;
	}

	if( lpObj->m_PlayerData->lpGuild )
	{
		if( g_CastleSiege.CheckCastleOwnerMember(lpObj->m_Index) )
		{
			bPermission = TRUE;
		}

		if( g_CastleSiege.CheckCastleOwnerUnionMember(lpObj->m_Index) )
		{
			bPermission = TRUE;
		}
	}
	else
	{
		if( g_CastleSiege.GetHuntZoneEnter() )
		{
			bPermission = TRUE;
		}
	}

	if( bPermission == TRUE )
	{
		if( lpObj->m_PlayerData->Money >= iEnterTaxMoney )
		{
			if( lpObj->MapNumber == MAP_INDEX_CASTLESIEGE )
			{
				if( gObjMoveGate(lpObj->m_Index, 95) == TRUE )
				{
					pMsg.btResult = 1;
					int iOldMoney = lpObj->m_PlayerData->Money;

					lpObj->m_PlayerData->Money -= iEnterTaxMoney;

					g_CastleSiegeSync.AddTributeMoney(iEnterTaxMoney);
					
					if( lpObj->m_PlayerData->Money < 0)
					{
						lpObj->m_PlayerData->Money = 0;
					}
					
					GCMoneySend(lpObj->m_Index, lpObj->m_PlayerData->Money);

					g_Log.Add("[Castle HuntZone] [%s][%s] - Entrance TAX : %d - %d = %d", lpObj->AccountID, lpObj->Name, iOldMoney, iEnterTaxMoney, lpObj->m_PlayerData->Money);
				}
			}
		}
	}

	IOCP.DataSend(iIndex, (LPBYTE)&pMsg, sizeof(pMsg));
}

void GameProtocol::CGReqJewelMix(PMSG_REQ_JEWEL_MIX * lpMsg, int iIndex)
{
	if ( lpMsg == NULL )
		return;

	bool bCheck = false;
	Check_SameSerialItem(iIndex, 3, bCheck);

	if (bCheck == true)
		return;
	CJewelMixSystem::MixJewel(iIndex, lpMsg->btJewelType, lpMsg->btJewelMix);
}

void GameProtocol::GCAnsJewelMix(int iIndex, int iResult)
{
	PMSG_ANS_JEWEL_UNMIX pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0xBC, 0x00, sizeof(pMsg));
	pMsg.btResult = iResult;

	IOCP.DataSend(iIndex, (LPBYTE)&pMsg, sizeof(pMsg));
}


void GameProtocol::CGReqJewelUnMix(PMSG_REQ_JEWEL_UNMIX * lpMsg, int iIndex)
{
	if ( lpMsg == NULL )
		return;

	bool bCheck = false;
	Check_SameSerialItem(iIndex, 3, bCheck);

	if (bCheck == true)
		return;
	CJewelMixSystem::UnMixJewel(iIndex, lpMsg->btJewelType, lpMsg->btJewelLevel, lpMsg->btJewelPos);
}

void GameProtocol::GCAnsJewelUnMix(int iIndex, int iResult)
{
	PMSG_ANS_JEWEL_UNMIX pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0xBC, 0x01, sizeof(pMsg));
	pMsg.btResult = iResult;

	IOCP.DataSend(iIndex, (LPBYTE)&pMsg, sizeof(pMsg));
}

void GameProtocol::CGReqCrywolfInfo(PMSG_REQ_CRYWOLF_INFO* lpMsg, int iIndex)
{
	if ( !ObjectMaxRange(iIndex))
		return;

	LPOBJ lpObj = &gObj[iIndex];

	if (g_ConfigRead.server.GetServerType() == SERVER_CASTLE)
	{
		GCAnsCrywolfInfo(iIndex, g_Crywolf.GetOccupationState(), g_Crywolf.GetCrywolfState());
	}

	else
	{
		GCAnsCrywolfInfo(iIndex, g_CrywolfSync.GetOccupationState(), g_CrywolfSync.GetCrywolfState());
	}
}

void GameProtocol::GCAnsCrywolfInfo(int iIndex, BYTE btOccupationState, BYTE btCrywolfState)
{
	PMSG_ANS_CRYWOLF_INFO pMsg={0};

	PHeadSubSetB((LPBYTE)&pMsg, 0xBD, 0x00, sizeof(pMsg));
	pMsg.btOccupationState = btOccupationState;
	pMsg.btCrywolfState = btCrywolfState;
	
	IOCP.DataSend(iIndex, (LPBYTE)&pMsg, sizeof(pMsg));
}


struct PMSG_ANS_CRYWOLF_ALTAR_CONTRACT
{
	PBMSG_HEAD2 h;	// C1:BD:03
	BYTE btResult;	// 4
	BYTE btAltarState;	// 5
	BYTE btObjIndexH;	// 6
	BYTE btObjIndexL;	// 7
};

void GameProtocol::CGReqAlatrContract(PMSG_REQ_CRYWOLF_ALTAR_CONTRACT* lpMsg, int iIndex)
{
	int iAltarIndex = MAKE_NUMBERW(lpMsg->btObjIndexH, lpMsg->btObjIndexL);

	if ( !ObjectMaxRange(iIndex))
		return;

	if ( !ObjectMaxRange(iAltarIndex))
		return;

	LPOBJ lpObj = &gObj[iIndex];
	LPOBJ lpAltarObj = &gObj[iAltarIndex];

	if ( !CRYWOLF_ALTAR_CLASS_RANGE(lpAltarObj->Class))
		return;

	if ( !gObjIsConnected(lpObj))
		return;

	PMSG_ANS_CRYWOLF_ALTAR_CONTRACT pMsg={0};

	PHeadSubSetB((LPBYTE)&pMsg, 0xBD, 0x03, sizeof(pMsg));
	pMsg.btResult = 0;
	pMsg.btAltarState = g_CrywolfNPC_Altar.GetAltarState(lpAltarObj->Class);
	pMsg.btObjIndexH = lpMsg->btObjIndexH;
	pMsg.btObjIndexL = lpMsg->btObjIndexL;

	if ( pMsg.btAltarState == 0 && 
		 lpObj->Type == OBJ_USER &&
		 lpObj->Class == CLASS_ELF &&
		 lpObj->Level >= MIN_ELF_LEVEL_ALTAR )
	{
		if ( g_CrywolfNPC_Altar.SetAltarUserIndex(iAltarIndex, lpAltarObj->Class, iIndex) != FALSE )
		{
			pMsg.btResult = 1;
		}
	}
	else
	{
		MsgOutput(iIndex, Lang.GetText(0,256), (int)MIN_ELF_LEVEL_ALTAR);	
	}

	IOCP.DataSend(iIndex, (LPBYTE)&pMsg, sizeof(pMsg));
}

struct PMSG_ANS_CRYWOLF_BENEFIT_PLUS_CHAOSRATE
{
	PBMSG_HEAD2 h;	// C1:BD:09
	BYTE btPlusChaosRate;	// 4
};


void GameProtocol::CGReqPlusChaosRate(PMSG_REQ_CRYWOLF_BENEFIT_PLUS_CHAOSRATE* lpMsg, int iIndex)
{
	if ( ObjectMaxRange(iIndex) == FALSE )
		return;

	PMSG_ANS_CRYWOLF_BENEFIT_PLUS_CHAOSRATE pMsg={0};

	PHeadSubSetB((LPBYTE)&pMsg, 0xBD, 0x09, sizeof(pMsg));
	pMsg.btPlusChaosRate = g_CrywolfSync.GetPlusChaosRate();

	IOCP.DataSend(iIndex, (LPBYTE)&pMsg, sizeof(pMsg));
}

void GameProtocol::CGReqKanturuStateInfo(PMSG_REQ_KANTURU_STATE_INFO* lpMsg, int iIndex)
{
	if ( ObjectMaxRange(iIndex) == FALSE )
		return;

	if (g_ConfigRead.server.GetServerType() != SERVER_CASTLE)
	{
		g_KanturuEntranceNPC.NotifyEntranceInfo(iIndex);
	}
}


void GameProtocol::GCReqEnterKanturuBossMap(PMSG_REQ_ENTER_KANTURU_BOSS_MAP* lpMsg, int iIndex)
{
	if ( ObjectMaxRange(iIndex) == FALSE )
		return;

	if (g_ConfigRead.server.GetServerType() != SERVER_CASTLE)
	{
		g_KanturuEntranceNPC.NotifyResultEnterKanturuBossMap(iIndex);
	}
}

void GameProtocol::WereWolfQuarrelCheck(int aIndex)
{
	if ( ObjectMaxRange(aIndex) == FALSE )
	{
		return;
	}

	if ( gObj[aIndex].Type != OBJ_USER || gObj[aIndex].Connected <= PLAYER_LOGGED )
	{
		return;
	}

	g_QuestInfo.CheckQuestMapEnterOnWerwolf(aIndex);
}

void GameProtocol::GateKeeperCheck(int aIndex)
{
	if ( ObjectMaxRange(aIndex) == FALSE )
	{
		return;
	}

	if ( gObj[aIndex].Type != OBJ_USER || gObj[aIndex].Connected <= PLAYER_LOGGED )
	{
		return;
	}

	g_QuestInfo.CheckQuestMapEnterOnGateKeeper(aIndex);
}

struct PMSG_ILLUSIONTEMPLE_KILLPOINT
{
	PBMSG_HEAD2 h;
	BYTE btKillPoint;
};

void GameProtocol::GCSendIllusionTempleKillPoint(int aIndex, BYTE KillPoint)
{
	PMSG_ILLUSIONTEMPLE_KILLPOINT pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xBF, 0x06, sizeof(pMsg));
	pMsg.btKillPoint = KillPoint;

	IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void GameProtocol::CGReqIllusionTempleUseSkill(PMSG_REQ_USEILLUSIONTEMPLESKILL * aRecv, int aIndex)
{
	if(ObjectMaxRange(aIndex) == false)
	{
		g_Log.Add("return %s %d", __FILE__, __LINE__);
		return;
	}

	g_IT_Event.ITR_USeSkill(aIndex, MAKE_NUMBERW(aRecv->MagicNumberH, aRecv->MagicNumberL), MAKE_NUMBERW(aRecv->btTargetObjIndexH, aRecv->btTargetObjIndexL), aRecv->btDis);
}

void GameProtocol::GCIllusionTempleSendReward(int aIndex)
{
	if(ObjectMaxRange(aIndex) == false)
	{
		g_Log.Add("return %s %d", __FILE__, __LINE__);
		return;
	}

	g_IT_Event.ReqEventReward(aIndex);
}

void GameProtocol::GCMasterLevelUpMsgSend(int aIndex)
{
	if ( !ObjectMaxRange(aIndex) )
		return;

	PMSG_MASTER_LEVEL_UP_SEND pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xF3, 0x51, sizeof(pMsg));

	pMsg.MasterLevel = gObj[aIndex].m_PlayerData->MasterLevel;
	pMsg.MLPoint = gObj[aIndex].m_PlayerData->MasterPoint;
	pMsg.GainPoint = g_ConfigRead.data.common.MLPointPerLevel;
	pMsg.MaxPoint = g_ConfigRead.data.common.MLUserMaxLevel;
	pMsg.MaxLife = gObj[aIndex].MaxLife+gObj[aIndex].AddLife;
	pMsg.MaxMana = gObj[aIndex].MaxMana+gObj[aIndex].AddMana;
	pMsg.MaxBP = gObj[aIndex].MaxBP + gObj[aIndex].AddBP;
	pMsg.MaxShield = gObj[aIndex].iMaxShield + gObj[aIndex].iAddShield;
	pMsg.IGCMaxLife = gObj[aIndex].MaxLife + gObj[aIndex].AddLife;
	pMsg.IGCMaxMana = gObj[aIndex].MaxMana + gObj[aIndex].AddMana;
	IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

	GCSendEffectInfo(aIndex, 16);
}

struct PMSG_REQ_REGISTER_LUCKYCOIN
{
	PBMSG_HEAD2 h;
	int iIndex;
	char szAccountID[11];
	BYTE btPos;
};

void GameProtocol::CGReqLuckyCoinInfo(int aIndex)
{
	if(gObj[aIndex].UseEventServer == FALSE)
	{
		PMSG_REQ_LUCKYCOIN pMsg = {0};
		PHeadSubSetB((LPBYTE)&pMsg, 0xBE, 0x19, sizeof(pMsg));
	
		memcpy(pMsg.szUID, gObj[aIndex].AccountID, 10);
		pMsg.szUID[10] = 0;
		pMsg.iIndex = aIndex;

		wsDataCli.DataSend((PCHAR)&pMsg, pMsg.h.size);

		gObj[aIndex].UseEventServer = TRUE;
	}
}

void GameProtocol::CGReqLuckyCoinRegister(PMSG_REQ_LUCKYCOIN_REGISTER *aRecv, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == false)
	{
		return;
	}

	if ( lpObj->m_IfState.type == 1 )
	{
		g_Log.Add("[Lucky Coin] [%s][%s] Attempted ItemCopy using Trade Window",
			lpObj->AccountID, lpObj->Name);

		return;
	}

	if ( lpObj->m_IfState.type != 7 )
	{
		g_Log.Add("[Lucky Coin] [%s][%s] User is NOT in Lucky Coin Window",
			lpObj->AccountID, lpObj->Name);

		return;
	}

	if(lpObj->UseEventServer == FALSE)
	{
		BYTE btPos = 0xFF;

		for(int i=INVENTORY_BAG_START;i<MAIN_INVENTORY_SIZE;i++)
		{
			if(lpObj->pInventory[i].IsItem() == TRUE)
			{
				if(lpObj->pInventory[i].m_Type == ITEMGET(14,100))
				{
					btPos = i;
					break;
				}
			}
		}
	
		if(btPos == 0xFF)
		{
			PMSG_ANS_LUCKYCOIN_REGISTER pMsg;
			PHeadSubSetB((LPBYTE)&pMsg, 0xBF, 0x0C, sizeof(pMsg));

			pMsg.btResult = 0;
			pMsg.iLuckyCoin = 0;

			IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);

			g_Log.Add("[Lucky Coin] [%s][%s] User doesn't have Lucky Coin in inventory",
				lpObj->AccountID, lpObj->Name);

			return;
		}

		lpObj->UseEventServer = TRUE;

		PMSG_REQ_REGISTER_LUCKYCOIN pMsg = {0};
		PHeadSubSetB((LPBYTE)&pMsg, 0xBE, 0x18, sizeof(pMsg));

		pMsg.btPos = btPos;
		pMsg.iIndex = aIndex;
		memcpy(pMsg.szAccountID, lpObj->AccountID, 10);

		wsDataCli.DataSend((char *)&pMsg, pMsg.h.size);

		g_Log.Add("[Lucky Coin] [%s][%s] Request to Register Lucky Coin",
			lpObj->AccountID, lpObj->Name);
	}
	return;
}

void GameProtocol::CGReqLuckyCoinTrade(PMSG_REQ_LUCKYCOIN_TRADE * aRecv, int aIndex)
{
	//CItemBagEx * Bag = NULL;
	LPOBJ lpObj = &gObj[aIndex];

	if(gObjIsConnectedGP(aIndex) == false)
	{
		return;
	}

	if ( lpObj->m_IfState.type == 1 )
	{
		g_Log.Add("[Lucky Coin] [%s][%s] Attempted ItemCopy using Trade Window",
			lpObj->AccountID, lpObj->Name);

		return;
	}

	if ( lpObj->m_IfState.type != 7 )
	{
		g_Log.Add("[Lucky Coin] [%s][%s] User is NOT in Lucky Coin Window",
			lpObj->AccountID, lpObj->Name);

		return;
	}

	int Count = 0;
	CItem Item;
	time_t Duration;

	switch ( aRecv->iLuckyCoinTradeCount )
	{
		case 10:
			g_BagManager.GetItemFromBag(aIndex, BAG_EVENT, EVENTBAG_LUCKYCOIN10, aIndex, Item, Duration);
			break;
		case 20:
			g_BagManager.GetItemFromBag(aIndex, BAG_EVENT, EVENTBAG_LUCKYCOIN20, aIndex, Item, Duration);
			break;
		case 30:
			g_BagManager.GetItemFromBag(aIndex, BAG_EVENT, EVENTBAG_LUCKYCOIN30, aIndex, Item, Duration);
			break;
		default:
			GCAnsLuckyCoinTrade(aIndex, 4);
			return;
	}

	for (int i=INVENTORY_BAG_START;i<MAIN_INVENTORY_SIZE;++i)
	{
		if(lpObj->pInventory[i].IsItem() == TRUE && lpObj->pInventory[i].m_Type == ITEMGET(14,100))
			Count += lpObj->pInventory[i].m_Durability;
	}

	if(Count < aRecv->iLuckyCoinTradeCount)
	{
		GCAnsLuckyCoinTrade(aIndex, 0);
		return;
	}

	if(!CheckInventoryEmptySpace(lpObj, 4, 2))
	{
		GCAnsLuckyCoinTrade(aIndex, 2);
		return;
	}

	Count = aRecv->iLuckyCoinTradeCount;

	for (int i=INVENTORY_BAG_START;i<MAIN_INVENTORY_SIZE;++i)
	{
		if(lpObj->pInventory[i].IsItem() == TRUE && lpObj->pInventory[i].m_Type == ITEMGET(14,100))
		{
			if(lpObj->pInventory[i].m_Durability > Count)
			{
				lpObj->pInventory[i].m_Durability -= Count;
				GCItemDurSend2(aIndex, i, lpObj->pInventory[i].m_Durability, 0);
				Count = 0;
			}
			else
			{
				Count -= lpObj->pInventory[i].m_Durability;
				gObjInventoryDeleteItem(aIndex, i);
				GCInventoryItemDeleteSend(aIndex, i, 1);
			}
			if(Count == 0)
				break;
		}
	}

	GJSetCharacterInfo(lpObj, aIndex, FALSE);
	ItemSerialCreateSend(aIndex, 235, 0, 0, Item.m_Type, Item.m_Level, Item.m_Durability, Item.m_Option1, Item.m_Option2, Item.m_Option3, aIndex, Item.m_NewOption, Item.m_SetOption, Duration, 0, 0);

	g_Log.Add("[Lucky Coin] [%s][%s] Trade Lucky Coin [%d]",
		lpObj->AccountID, lpObj->Name, aRecv->iLuckyCoinTradeCount);
}

void GameProtocol::GCAnsLuckyCoinTrade(int aIndex, BYTE Result)
{
	PMSG_ANS_LUCKYCOIN_TRADE pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xBF, 0x0D, sizeof(pMsg));

	pMsg.btResult = Result;

	IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void GameProtocol::CGReqMuBotUse(int aIndex, PMSG_MUBOT_SWITCH *aRecv)
{
	if(!ObjectMaxRange(aIndex))
		return;

	if(!gObjIsConnectedGP(aIndex))
		return;

	if(g_ConfigRead.data.mubot.enable == false)
		return;

	LPOBJ lpObj = &gObj[aIndex];
	DWORD Money = (lpObj->Level+lpObj->m_PlayerData->MasterLevel) * g_ConfigRead.data.mubot.cost;

	if(lpObj->Type != OBJ_USER)
		return;

	if(lpObj->Level < g_ConfigRead.data.mubot.minlevel)
	{
		MsgOutput(aIndex, Lang.GetText(0,419), g_ConfigRead.data.mubot.minlevel);
		this->GCAnsMuBotUse(aIndex, 1, 0, 0);
		return;
	}

	if(lpObj->m_PlayerData->Money < Money)
	{
		MsgOutput(aIndex, Lang.GetText(0,420), Money);
		this->GCAnsMuBotUse(aIndex, 1, 0, 0);
		return;
	}

	if(lpObj->m_PlayerData->VipType < g_ConfigRead.data.mubot.viplevel)
	{
		MsgOutput(aIndex, Lang.GetText(0,421));
		this->GCAnsMuBotUse(aIndex, 1, 0, 0);
		return;
	}

	if(lpObj->m_PlayerData->VipType == 0 && g_ConfigRead.data.mubot.viplevel > 0)
	{
		MsgOutput(aIndex, Lang.GetText(0,422));
		this->GCAnsMuBotUse(aIndex, 1, 0, 0);
		return;
	}

	if (g_NewPVP.IsDuel(*lpObj) == true || g_NewPVP.IsObserver(*lpObj) == true)
	{
		this->GCAnsMuBotUse(aIndex, 1, 0, 0);
		return;
	}

	if(aRecv->Status == 0)
	{
		lpObj->m_PlayerData->m_MuBotPayTime = 0;
		lpObj->m_PlayerData->m_MuBotTotalTime = 0;
		lpObj->m_PlayerData->m_MuBotTick = GetTickCount();
		lpObj->m_PlayerData->m_MuBotEnable = true;

		this->GCAnsMuBotUse(aIndex, 0, 0, 0);
	}
	else
	{
		lpObj->m_PlayerData->m_MuBotPayTime = 0;
		lpObj->m_PlayerData->m_MuBotTotalTime = 0;
		lpObj->m_PlayerData->m_MuBotTick = 0;
		lpObj->m_PlayerData->m_MuBotEnable = false;

		this->GCAnsMuBotUse(aIndex, 1, 0, 0);
	}

}

void GameProtocol::GCAnsMuBotUse(int aIndex, BYTE Status, DWORD Time, DWORD Money)
{
	PMSG_MUBOT_USE pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xBF, 0x51, sizeof(pMsg));

	pMsg.Status = Status;
	pMsg.Money = Money;
	pMsg.Time = Time % 0xFF;
	pMsg.TimeMultipler = Time / 0xFF;
	
	IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void GameProtocol::CGReqMuBotSaveData(int aIndex, PMSG_MUBOT_DATASAVE *aRecv)
{
	if(!ObjectMaxRange(aIndex))
		return;

	if(!gObjIsConnected(aIndex))
		return;

	if(gObj[aIndex].Type != OBJ_USER)
		return;

	GDReqMuBotSave(aIndex, aRecv->MuBotData);
}

void GameProtocol::GCAnsMuBotData(int aIndex, BYTE MuBotData[257])
{
	PMSG_MUBOT_DATASAVE pMsg;
	PHeadSetW((LPBYTE)&pMsg, 0xAE, sizeof(pMsg));

	memcpy(pMsg.MuBotData, MuBotData, 257);

	IOCP.DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
}

void GameProtocol::GCSendObjectHP(int aIndex, int aTargetIndex)
{
	PMSG_TARGET_HP pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xFA, 0x05, sizeof(pMsg));

	pMsg.m_Index = aTargetIndex;
	pMsg.m_Hp = gObj[aTargetIndex].Life;
	pMsg.m_MaxHp = gObj[aTargetIndex].MaxLife + gObj[aTargetIndex].AddLife;
	pMsg.m_Level = gObj[aTargetIndex].Level;
	pMsg.m_bIceEffect = gObjCheckUsedBuffEffect(&gObj[aTargetIndex], BUFFTYPE_FREEZE);
	pMsg.m_bPoisonEffect = gObjCheckUsedBuffEffect(&gObj[aTargetIndex], BUFFTYPE_POISON);

	IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
	this->MsgSendV2(&gObj[aTargetIndex], (LPBYTE)&pMsg, pMsg.h.size);
}

void GameProtocol::CGReqAntiCheatRecv(int aIndex, PMSG_SEND_AH_INFO *aRecv)
{
	if(!ObjectMaxRange(aIndex))
	{
		return;
	}

	if(!gObjIsConnected(aIndex))
	{
		return;
	}

	if(gObj[aIndex].Type != OBJ_USER)
	{
		return;
	}

	if(gObj[aIndex].Connected != PLAYER_PLAYING)
	{
		return;
	}

	if(gObj[aIndex].m_PlayerData->LivePacketTime == 0 || g_ConfigRead.antihack.EnablePacketTimeCheck == FALSE)
	{
		gObj[aIndex].m_PlayerData->LivePacketTime = GetTickCount();
	}

	else
	{
		DWORD tick = GetTickCount() -  gObj[aIndex].m_PlayerData->LivePacketTime;

		if(tick < g_ConfigRead.antihack.PacketTimeMinTime)
		{
			g_Log.AddC(TColor::Red, "[ANTI-HACK] (%s)(%s)(%s) (Map:%d)(X:%d)(Y:%d) SpeedHack detected -> Tick (Current:%d/Minimum:%d)",
				gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_PlayerData->Ip_addr, gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y, tick, g_ConfigRead.antihack.PacketTimeMinTime);

			AntiHackLog->Output("[ANTI-HACK] (%s)(%s)(%s) (Map:%d)(X:%d)(Y:%d) SpeedHack detected -> Tick (Current:%d/Minimum:%d)",
				gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_PlayerData->Ip_addr, gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y, tick, g_ConfigRead.antihack.PacketTimeMinTime);

			gObj[aIndex].m_PlayerData->LiveCheckDetectCount++;

			if(gObj[aIndex].m_PlayerData->LiveCheckDetectCount >= 3)
			{

				if(g_ConfigRead.antihack.EnableHackDetectMessage == TRUE)
				{
					MsgOutput(aIndex, (char *)g_ConfigRead.antihack.HackDetectMessage.c_str());
				}

				if(g_ConfigRead.antihack.EnableAutoBanUserAccount == TRUE)
				{
					GDReqBanUser(aIndex, 1, 1);
				}
				
				this->GCSendDisableReconnect(aIndex);
				//IOCP.CloseClient(aIndex);

				return;
			}
		}

		else
		{
			gObj[aIndex].m_PlayerData->LivePacketTime = GetTickCount();
		}
	}

	if(g_ConfigRead.antihack.EnabledAgiCheck == false)
	{
		gObj[aIndex].AgiCheckTime = GetTickCount();
		return;
	}

	if(gObj[aIndex].m_PlayerData->BlessSkillCast == true)
	{
		gObj[aIndex].AgiCheckTime = GetTickCount();
		return;
	}

	WORD Agility = gObj[aIndex].m_PlayerData->Dexterity + gObj[aIndex].AddDexterity;

	int AgilityDiff = (gObj[aIndex].m_PlayerData->Dexterity + gObj[aIndex].AddDexterity) - aRecv->Agi;

	if(aRecv->type == 1)
	{
		if(AgilityDiff <= -30 || AgilityDiff >= 30)
		{
			g_Log.AddC(TColor::Red,  "[ANTI-HACK] (%s)(%s)(%s) Dexterity check error: %d/%d", gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_PlayerData->Ip_addr,
				Agility, aRecv->Agi);

			AntiHackLog->Output("[ANTI-HACK] (%s)(%s)(%s) Dexterity check error: %d/%d", gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_PlayerData->Ip_addr,
				Agility, aRecv->Agi);

			if(g_ConfigRead.antihack.EnableHackDetectMessage == TRUE)
			{
				MsgOutput(aIndex, (char *)g_ConfigRead.antihack.HackDetectMessage.c_str());
			}

			if(g_ConfigRead.antihack.EnableAutoBanUserAccount == TRUE)
			{
				GDReqBanUser(aIndex, 1, 1);
			}

			this->GCSendDisableReconnect(aIndex);
			//IOCP.CloseClient(aIndex);
		}

		else
		{
			gObj[aIndex].AgiCheckTime = GetTickCount();
		}
	}
	else
	{
		g_Log.AddC(TColor::Red,  "[ANTI-HACK] (%s)(%s)(%s) Cheat detected: %s", gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_PlayerData->Ip_addr,
			aRecv->Agi == 0 ? "SpeedHack" : "Proxy");

		AntiHackLog->Output("[ANTI-HACK] (%s)(%s)(%s) Cheat detected: %s", gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_PlayerData->Ip_addr,
			aRecv->Agi == 0 ? "SpeedHack" : "Proxy");

		if(g_ConfigRead.antihack.EnableHackDetectMessage == TRUE)
		{
			MsgOutput(aIndex, (char *)g_ConfigRead.antihack.HackDetectMessage.c_str());
		}

		if(g_ConfigRead.antihack.EnableAutoBanUserAccount == TRUE)
		{
			GDReqBanUser(aIndex, 1, 1);
		}

		this->GCSendDisableReconnect(aIndex);
		//IOCP.CloseClient(aIndex);

	}
}

void GameProtocol::CGReqFileCrcRecv(int aIndex, PMSG_REQ_CRCDATA *aRecv)
{
	if(!ObjectMaxRange(aIndex))
	{
		return;
	}

	if(!gObjIsConnectedGP(aIndex))
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if(g_ConfigRead.antihack.MainCRC == 0)
	{
		lpObj->CrcCheckTime = 0;
		return;
	}

	if(aRecv->MainExe == g_ConfigRead.antihack.MainCRC &&
		aRecv->IGCDLL == g_ConfigRead.antihack.DLLCRC &&
		aRecv->PlayerBMD == g_ConfigRead.antihack.PlayerCRC &&
		aRecv->SkillCRC == g_ConfigRead.antihack.SkillCRC
		)
	{
		lpObj->CrcCheckTime = 0;
	}

	else
	{
		g_Log.Add("[ANTI-HACK] (%s)(%s)(%s) Invalid Game-Client files (Recv/Valid) (Main:%X/%X) (IGC:%X/%X) (Player:%X/%X) (SkillCRC: %X/%X)", lpObj->AccountID, lpObj->Name, lpObj->m_PlayerData->Ip_addr, aRecv->MainExe, g_ConfigRead.antihack.MainCRC,
		aRecv->IGCDLL, g_ConfigRead.antihack.DLLCRC, aRecv->PlayerBMD, g_ConfigRead.antihack.PlayerCRC, aRecv->SkillCRC, g_ConfigRead.antihack.SkillCRC);

		AntiHackLog->Output("[ANTI-HACK] (%s)(%s)(%s) Invalid Game-Client files (Recv/Valid) (Main:%X/%X) (IGC:%X/%X) (Player:%X/%X) (SkillCRC: %X/%X)", lpObj->AccountID, lpObj->Name, lpObj->m_PlayerData->Ip_addr, aRecv->MainExe, g_ConfigRead.antihack.MainCRC,
			aRecv->IGCDLL, g_ConfigRead.antihack.DLLCRC, aRecv->PlayerBMD, g_ConfigRead.antihack.PlayerCRC,  aRecv->SkillCRC, g_ConfigRead.antihack.SkillCRC);

		if(g_ConfigRead.antihack.EnableHackDetectMessage == TRUE)
		{
			MsgOutput(aIndex, (char *)g_ConfigRead.antihack.HackDetectMessage.c_str());
		}

		if(g_ConfigRead.antihack.EnableAutoBanUserAccount == TRUE)
		{
			GDReqBanUser(aIndex, 1, 1);
		}

		this->GCSendDisableReconnect(aIndex);
		//IOCP.CloseClient(aIndex);
		
	}
}

void GameProtocol::GCElementalDamageSend(int aIndex, int TargetIndex, int Damage, int Elemental)
{
	PMSG_ELEMENTALDAMAGE pResult;
	PHeadSetB((LPBYTE)&pResult, 0xD8, sizeof(pResult));

	pResult.NumberH = SET_NUMBERH(TargetIndex);
	pResult.NumberL = SET_NUMBERL(TargetIndex);
	pResult.Damage = Damage;
	pResult.btElement = Elemental;

	pResult.btTargetH = SET_NUMBERH(aIndex);
	pResult.btTargetL = SET_NUMBERL(aIndex);

	if ( gObj[TargetIndex].Type == OBJ_USER )
	{
		IOCP.DataSend(TargetIndex, (LPBYTE)&pResult, pResult.h.size);
	}

	if ( cManager.WatchTargetIndex == TargetIndex || cManager.WatchTargetIndex == aIndex )
	{
		cManager.DataSend((LPBYTE)&pResult, pResult.h.size);
	}

	if ( gObj[aIndex].Type == OBJ_USER )
	{
		IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
	}
}

void GameProtocol::CGReqEnterAcheron(int aIndex)
{
	if(!ObjectMaxRange(aIndex))
	{
		return;
	}

	if(gObj[aIndex].Type != OBJ_USER)
	{
		return;
	}

	int iTargetNumber = gObj[aIndex].TargetNumber;

	if(iTargetNumber == -1)
	{
		return;
	}

	if(gObj[iTargetNumber].Class != 580)
	{
		return;
	}

	BYTE pos = 0xFF;

	for(int i=12;i<MAIN_INVENTORY_SIZE;i++)
	{
		if(gObj[aIndex].pInventory[i].m_Type == ITEMGET(13,146))
		{
			pos = i;
			break;
		}
	}

	if(pos == 0xFF)
	{
		PMSG_RESULT pMsg;
		PHeadSubSetB((LPBYTE)&pMsg, 0xF8, 0x21, sizeof(pMsg));

		pMsg.result = 0x01;
		IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	gObjMoveGate(aIndex, 417);
	g_Log.AddC(TColor::Blue, "[%s][%s] Entered to Acheron Map (Spirit Map serial: %I64d)", gObj[aIndex].AccountID, gObj[aIndex].Name,
		gObj[aIndex].pInventory[pos].m_Number);

	gObjInventoryDeleteItem(aIndex, pos);
	this->GCInventoryItemDeleteSend(aIndex, pos, 1);
}

void GameProtocol::GCObjectLifeInfo(int iIndex, short sObjNum, int iMaxLife, int iCurLife)
{
	PMSG_OBJECT_LIFE_INFO pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xEC, 0x10, sizeof(pMsg));

	pMsg.ObjNumberH = HIBYTE(sObjNum);
	pMsg.ObjNumberL = LOBYTE(sObjNum);

	pMsg.MaxLifeHH = iMaxLife >> 24;
	pMsg.MaxLifeHL = iMaxLife >> 8;
	pMsg.MaxLifeLH = iMaxLife >> 16;
	pMsg.MaxLifeLL = iMaxLife;

	pMsg.CurLifeHH = iCurLife >> 24;
	pMsg.CurLifeHL = iCurLife >> 8;
	pMsg.CurLifeLH = iCurLife >> 16;
	pMsg.CurLifeLL = iCurLife;

	IOCP.DataSend(iIndex, (LPBYTE)&pMsg, sizeof(pMsg));
}

void GameProtocol::GCChaosMachinePriceSend(int aIndex)
{
	if(gObj[aIndex].Type != OBJ_USER)
	{
		g_Log.AddC(TColor::Red,"[ERROR] Index %d not USER", aIndex);
		return;
	}

	BYTE SendData[65535];
	PMSG_PRICE_INFO pMsg;
	PMSG_PRICE_ITEM_LIST pList;
	pMsg.h.c = 0xC2;
	pMsg.h.headcode = 0xFA;
	pMsg.subc = 0x96;
	int lOfs = 0;
	lOfs += sizeof(pMsg );

		pMsg.count = 0;
		pMsg.Type = 1;
		for(short i = 0 ; i < CHAOS_BOX_SIZE; ++i)
		{
			if(gObj[aIndex].pChaosBox[i].IsItem())
			{
				gObj[aIndex].pChaosBox[i].Value();
				pList.BuyPrice = gObj[aIndex].pChaosBox[i].m_BuyMoney;
				pList.SellPrice = gObj[aIndex].pChaosBox[i].m_SellMoney;
				pList.Pos = i;
				memcpy(&SendData[lOfs],&pList,sizeof(pList));
				lOfs += sizeof(pList);
				pMsg.count++;
			}
		}
		int size = lOfs + pMsg.count*sizeof(PMSG_PRICE_ITEM_LIST);
		pMsg.h.sizeL = SET_NUMBERL(size);
		pMsg.h.sizeH = SET_NUMBERH(size);
		memcpy(&SendData[0], &pMsg, sizeof(pMsg));
		IOCP.DataSend(aIndex,SendData,size);
	
}
void GameProtocol::GCPriceSend(int aIndex, BYTE type, SHOP_DATA *lpShopData)
{
	if(gObj[aIndex].Type != OBJ_USER)
	{
		g_Log.AddC(TColor::Red,"[ERROR] Index %d not USER", aIndex);
		return;
	}

	BYTE SendData[65535];
	PMSG_PRICE_INFO pMsg;
	PMSG_PRICE_ITEM_LIST pList;
	pMsg.h.c = 0xC2;
	pMsg.h.headcode = 0xFA;
	pMsg.subc = 0x95;
	int lOfs = 0;
	lOfs += sizeof(pMsg );

	if(type == 0) // shop
	{
		if (!lpShopData)
		{
			return;
		}

		pMsg.count = lpShopData->m_ShopData.ItemCount;
		int size = lOfs + pMsg.count*sizeof(PMSG_PRICE_ITEM_LIST);
		pMsg.h.sizeL = SET_NUMBERL(size);
		pMsg.h.sizeH = SET_NUMBERH(size);
		pMsg.Type = 0;
		memcpy(SendData, &pMsg, sizeof(pMsg));

		for (int i = 0; i < 120; i++)
		{
			if (lpShopData->m_ShopData.m_item[i].IsItem())
			{
				lpShopData->m_ShopData.m_item[i].Value();
				pList.BuyPrice = lpShopData->m_ShopData.m_item[i].m_BuyMoney;
				pList.SellPrice = lpShopData->m_ShopData.m_item[i].m_SellMoney;
				pList.Pos = i;
				memcpy(&SendData[lOfs], &pList, sizeof(pList));
				lOfs += sizeof(pList);
			}
		}

		IOCP.DataSend(aIndex,SendData,size);
	}

	if (type == 1) // inventory
	{
		pMsg.count = 0;
		pMsg.Type = 1;

		for (int i = 0; i < INVENTORY_SIZE; ++i)
		{
			if (gObj[aIndex].pInventory[i].IsItem())
			{
				gObj[aIndex].pInventory[i].Value();
				pList.BuyPrice = gObj[aIndex].pInventory[i].m_BuyMoney;
				pList.SellPrice = gObj[aIndex].pInventory[i].m_SellMoney;
				pList.Pos = i;
				memcpy(&SendData[lOfs], &pList, sizeof(pList));
				lOfs += sizeof(pList);
				pMsg.count++;
			}
		}

		int size = lOfs + pMsg.count*sizeof(PMSG_PRICE_ITEM_LIST);
		pMsg.h.sizeL = SET_NUMBERL(size);
		pMsg.h.sizeH = SET_NUMBERH(size);
		memcpy(&SendData[0], &pMsg, sizeof(pMsg));
		IOCP.DataSend(aIndex, SendData, size);
	}
}

int GameProtocol::OnCGInviteDuel(LPPMSG_REQ_DUEL_INVITE lpMsg, int aIndex)
{
	if (g_ConfigRead.server.GetServerType() == SERVER_CASTLE)
	{
		return ENEWPVP::E_NOT_DUEL_CSSERVER;
	}

	int nResponsor = MAKEWORD(lpMsg->NumberL, lpMsg->NumberH);
	
	LPOBJECTSTRUCT lpRequester = (LPOBJECTSTRUCT)&gObj[aIndex];

	if( !PacketCheckTime(aIndex) ) return ENEWPVP::E_LIMIT_PACKETTIME;
	if( nResponsor == aIndex )			return ENEWPVP::E_INVITE_MYSELF;
	if( g_ConfigRead.server.IsNonPvP() )
	{
		GCServerMsgStringSend(Lang.GetText(0,97), aIndex, 1);
		return ENEWPVP::E_NON_PKSERVER;
	}
	if( gObj[aIndex].CloseType != -1 )	return ENEWPVP::E_CONNECTION_CLOSING;
	if( g_NewPVP.CheckPKPenalty(lpRequester) )
	{
		GCServerMsgStringSend(Lang.GetText(0,98), aIndex, 1);
		return ENEWPVP::E_NOT_DUEL_MURDERER;
	}
	if( !gObjIsConnectedEx(nResponsor) )
	{
		g_Log.Add("error: %s %d (%d)", __FILE__, __LINE__, nResponsor);
		return ENEWPVP::E_DISCONNECT;
	}
	LPOBJECTSTRUCT lpResponsor = (LPOBJECTSTRUCT)&gObj[nResponsor];
	if( g_NewPVP.CheckPKPenalty(lpResponsor) )
	{
		GCServerMsgStringSend(Lang.GetText(0,99), aIndex, 1);
		return ENEWPVP::E_NOT_DUEL_MURDERER;
	}

	char szDuelName[MAX_ACCOUNT_LEN+1] = {0};
	char szDuelName2[MAX_ACCOUNT_LEN+1] = {0};

	memcpy(szDuelName, lpResponsor->Name, MAX_ACCOUNT_LEN);
	szDuelName[MAX_ACCOUNT_LEN] = '\0';

	memcpy(szDuelName2, lpMsg->szName, MAX_ACCOUNT_LEN);
	szDuelName2[MAX_ACCOUNT_LEN] = '\0';

	if( strcmp(szDuelName, szDuelName2) != 0 )
	{
		GCServerMsgStringSend(Lang.GetText(0,85), aIndex, 1);
		return ENEWPVP::E_FAILED_RESULT;
	}
	if( !IsDuelEnable(lpResponsor->m_Index) )
	{
		GCServerMsgStringSend(Lang.GetText(0,86), aIndex, 1);
		return ENEWPVP::E_FAILED_RESULT;
	}
	if( g_NewPVP.IsGuildWar(lpRequester) )
	{
		GCServerMsgStringSend(Lang.GetText(0,87), aIndex, 1);
		return ENEWPVP::E_GUILDWAR;
	}
	if( g_NewPVP.IsGuildWar(lpResponsor) )
	{
		GCServerMsgStringSend(Lang.GetText(0,88), aIndex, 1);
		return ENEWPVP::E_GUILDWAR;
	}
	if( g_NewPVP.IsSelfDefense(lpRequester) || g_NewPVP.IsSelfDefense(lpResponsor) )
	{
		GCServerMsgStringSend(Lang.GetText(0,108), aIndex, 1);
		return ENEWPVP::E_SELFDEFENSE;
	}
	if( CC_MAP_RANGE(lpRequester->MapNumber) || CC_MAP_RANGE(lpResponsor->MapNumber) )
	{
		GCServerMsgStringSend(Lang.GetText(0,125), aIndex, 1);
		return ENEWPVP::E_INVALID_MAP;
	}
	if( DS_MAP_RANGE(lpRequester->MapNumber) || DS_MAP_RANGE(lpResponsor->MapNumber) )
	{
		GCServerMsgStringSend(Lang.GetText(0,100), aIndex, 1);
		return ENEWPVP::E_INVALID_MAP;
	}
	if( BC_MAP_RANGE(lpRequester->MapNumber) || BC_MAP_RANGE(lpResponsor->MapNumber) )
	{
		GCServerMsgStringSend(Lang.GetText(0,89), aIndex, 1);
		return ENEWPVP::E_INVALID_MAP;
	}
	if( IT_MAP_RANGE(lpRequester->MapNumber) || IT_MAP_RANGE(lpResponsor->MapNumber) )
	{
		GCServerMsgStringSend(Lang.GetText(0,300), aIndex, 1);
		return ENEWPVP::E_INVALID_MAP;
	}
	if( DG_MAP_RANGE(lpRequester->MapNumber) || DG_MAP_RANGE(lpResponsor->MapNumber) )
	{
		GCServerMsgStringSend(Lang.GetText(0,301), aIndex, 1);
		return ENEWPVP::E_INVALID_MAP;
	}
	if( IMPERIAL_MAP_RANGE(lpRequester->MapNumber) || IMPERIAL_MAP_RANGE(lpResponsor->MapNumber) )
	{
		GCServerMsgStringSend(Lang.GetText(0,302), aIndex, 1);
		return ENEWPVP::E_INVALID_MAP;
	}
	if( lpRequester->m_IfState.use > 0 )
	{
		GCServerMsgStringSend(Lang.GetText(0,90), aIndex, 1);
		return ENEWPVP::E_FAILED_RESULT;
	}
	if( lpResponsor->m_IfState.use > 0 )
	{
		GCServerMsgStringSendEx(aIndex, 1, Lang.GetText(0,91), lpResponsor->Name);
		return ENEWPVP::E_FAILED_RESULT;
	}
	int nRet = g_NewPVP.Reserve(*lpRequester, *lpResponsor);
	if( nRet != ENEWPVP::E_NO_ERROR )
	{
		switch( nRet )
		{
		case ENEWPVP::E_ALREADY_DUELRESERVED:
			GCServerMsgStringSendEx(aIndex, 1, Lang.GetText(0,93), lpResponsor->Name);
			GCServerMsgStringSend(Lang.GetText(0,84), aIndex, 1);
			break;
		case ENEWPVP::E_ALREADY_DUELLING:
			GCServerMsgStringSend(Lang.GetText(0,83), aIndex, 1);
			GCServerMsgStringSend(Lang.GetText(0,84), aIndex, 1);
			break;
		case ENEWPVP::E_ALREADY_DUELREQUESTED_1:
			GCServerMsgStringSendEx(aIndex, 1, Lang.GetText(0,92), lpResponsor->Name);
			break;
		case ENEWPVP::E_ALREADY_DUELRESERVED_1:			
			GCServerMsgStringSendEx(aIndex, 1, Lang.GetText(0,93), lpResponsor->Name);
			break;
		case ENEWPVP::E_ALREADY_DUELLING_1:
			GCServerMsgStringSendEx(aIndex, 1, Lang.GetText(0,94), lpResponsor->Name);
			break;
		}
		return nRet;
	}
	
	GCServerMsgStringSendEx(aIndex, 1, Lang.GetText(0,95), lpResponsor->Name);
	g_Log.Add("[NewPVP] [%s][%s] Requested to Start Duel to [%s][%s]", lpRequester->AccountID, lpRequester->Name, lpResponsor->AccountID, lpResponsor->Name);
	return ENEWPVP::E_NO_ERROR;
}

int GameProtocol::OnCGAnswerDuel(LPPMSG_ANS_DUEL_ANSWER lpMsg, int aIndex)
{
	if (g_ConfigRead.server.GetServerType() == SERVER_CASTLE)
	{
		return ENEWPVP::E_NOT_DUEL_CSSERVER;
	}

	LPOBJECTSTRUCT lpRequester = NULL;
	LPOBJECTSTRUCT lpResponsor = (LPOBJECTSTRUCT)&gObj[aIndex];
	int nRequester = MAKEWORD(lpMsg->NumberL, lpMsg->NumberH);

	if( !gObjIsConnectedEx(nRequester) )	return ENEWPVP::E_DISCONNECT;
	lpRequester = (LPOBJECTSTRUCT)&gObj[nRequester];

	PMSG_ANS_DUEL_INVITE res = {0};
	res.h.c = 0xC1;
	res.h.size = sizeof(PMSG_ANS_DUEL_INVITE);
	res.h.headcode = 0xAA;
	res.h.subcode = 0x01;	
	memcpy(res.szName, lpResponsor->Name, MAX_ACCOUNT_LEN);
	if( lpResponsor->CloseType != -1 )
	{
		g_NewPVP.Cancel(*lpRequester, *lpResponsor, FALSE);
		res.nResult = ENEWPVP::E_CONNECTION_CLOSING;
		IOCP.DataSend(aIndex, (LPBYTE)&res, res.h.size);
		return ENEWPVP::E_CONNECTION_CLOSING;
	}
	if( !lpMsg->bDuelOK )
	{
		g_NewPVP.Cancel(*lpRequester, *lpResponsor, FALSE);
		res.nResult = ENEWPVP::E_REFUSE_INVATE;
		IOCP.DataSend(aIndex, (LPBYTE)&res, res.h.size);
		return ENEWPVP::E_REFUSE_INVATE;
	}
	if( BC_MAP_RANGE(lpRequester->MapNumber) ||
		CC_MAP_RANGE(lpRequester->MapNumber) ||
		DS_MAP_RANGE(lpRequester->MapNumber) ||
		IT_MAP_RANGE(lpRequester->MapNumber) ||
		DG_MAP_RANGE(lpRequester->MapNumber) ||
		IMPERIAL_MAP_RANGE(lpRequester->MapNumber) ||
		lpRequester->MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL)
	{
		g_NewPVP.Cancel(*lpRequester, *lpResponsor, FALSE);
		GCServerMsgStringSend(Lang.GetText(0,125), lpResponsor->m_Index, 1);
		GCServerMsgStringSend(Lang.GetText(0,125), lpRequester->m_Index, 1);
		res.nResult = ENEWPVP::E_INVALID_MAP;
		IOCP.DataSend(lpResponsor->m_Index, (LPBYTE)&res, res.h.size);
		IOCP.DataSend(lpRequester->m_Index, (LPBYTE)&res, res.h.size);
		return ENEWPVP::E_INVALID_MAP;
	}
	int nRet = g_NewPVP.Join(*lpRequester, *lpResponsor);
	if( nRet != ENEWPVP::E_NO_ERROR )
	{
		switch( nRet )
		{
		case ENEWPVP::E_ALREADY_DUELRESERVED_1:
			GCServerMsgStringSend(Lang.GetText(0,96), aIndex, 1);
			break;
		case ENEWPVP::E_ALREADY_DUELLING_1:
			GCServerMsgStringSend(Lang.GetText(0,83), aIndex, 1);
			GCServerMsgStringSend(Lang.GetText(0,84), aIndex, 1);
			break;
		}
		res.nResult = nRet;
		IOCP.DataSend(lpRequester->m_Index, (LPBYTE)&res, res.h.size);
		IOCP.DataSend(lpResponsor->m_Index, (LPBYTE)&res, res.h.size);
		g_NewPVP.Cancel(*lpRequester, *lpResponsor, FALSE);
		return nRet;
	}
	return ENEWPVP::E_NO_ERROR;
}

int GameProtocol::OnCGLeaveDuel(LPPMSG_REQ_DUEL_EXIT lpMsg, int aIndex)
{
	if (g_ConfigRead.server.GetServerType() == SERVER_CASTLE)
	{
		return ENEWPVP::E_NOT_DUEL_CSSERVER;
	}

	if( gObj[aIndex].CloseType != -1 )	return ENEWPVP::E_DISCONNECT;
	int nRet = g_NewPVP.Leave(gObj[aIndex]);
	if( nRet != ENEWPVP::E_NO_ERROR )	return nRet;
	return ENEWPVP::E_NO_ERROR;
}

int GameProtocol::OnDuelChannelJoin(LPPMSG_REQ_DUEL_JOINCNANNEL lpMsg, int aIndex)
{
	if( gObj[aIndex].CloseType != -1 )	return ENEWPVP::E_DISCONNECT;
	int nRet = g_NewPVP.JoinChannel(lpMsg->nChannelId, gObj[aIndex]);
	if( nRet != ENEWPVP::E_NO_ERROR )	return nRet;
	return ENEWPVP::E_NO_ERROR;
}

int GameProtocol::OnDuelChannelLeave(LPPMSG_REQ_DUEL_LEAVECNANNEL lpMsg, int aIndex)
{
	if( gObj[aIndex].CloseType != -1 )	return ENEWPVP::E_DISCONNECT;
	int nRet = g_NewPVP.LeaveChannel(lpMsg->nChannelId, gObj[aIndex]);
	if( nRet != ENEWPVP::E_NO_ERROR )	return nRet;
	return ENEWPVP::E_NO_ERROR;
}

void GameProtocol::CGReqRegGensMember(_tagPMSG_REQ_REG_GENS_MEMBER *lpMsg, int iIndex)
{
	unsigned char bRet;

	LPOBJ lpObj = &gObj[iIndex];

	_GUILD_INFO_STRUCT *lpGuildInfo = gObj[iIndex].m_PlayerData->lpGuild;

	if (!ObjectMaxRange(iIndex)) return;

	if (gObjIsConnectedGP(iIndex))
	{
		_tagPMSG_ANS_REG_GENS_MEMBER_EXDB pMsg;	

		pMsg.bInfluence = lpMsg->bResult;

		//[GensSystem] [%s][%s] Gens Request - Join [%s]
		if (!g_GensSystem.GetGensInfluence(lpObj))
		{
			pMsg.bResult = 0; bRet = 1; 
		}
		
		//GensSystem] [%s][%s] Gens Joining Failed to Gens be Same : [%s]
		if (g_GensSystem.GetGensInfluence(lpObj) > 0)
		{
			pMsg.bResult = 1; bRet = 0;
		}

		//[GensSystem] [%s][%s] Gens Joining Failed to Below Gens leaving Time
		//if (g_GensAbusingResetTime < 60)
		//{
		//	pMsg.bResult = 2; bRet = 1;
		//}

		//[GensSystem] [%s][%s] Gens Joining Failed to Below 50Lv
		if (lpObj->Level < 50)
		{
			pMsg.bResult = 3; bRet = 0;
		}

		//[GensSystem] [%s][%s] Gens Joining Failed to Gens be Different Guild Leader [%s]
		if (lpObj->m_PlayerData->GuildStatus >= 0)
		{
			pMsg.bResult = 4; bRet = 0;
		}

		//[GensSystem] [%s][%s] Gens Joining Failed to Not Register Guild Leader GuildName [%s]
		if (lpObj->m_PlayerData->GuildStatus == GUILD_MASTER)
		{
			pMsg.bResult = 5; bRet = 0;
		}

		//[GensSystem] [%s][%s] Gens Joining Failed to Already Partymember
		if (lpObj->PartyNumber >= 0)
		{
			pMsg.bResult = 6; bRet = 0; 
		}

		//[GensSystem] [%s][%s] Gens Joining Failed to Union GuildMaster
		if (lpGuildInfo != NULL && lpGuildInfo->iGuildUnion)
		{
			pMsg.bResult = 7; bRet = 0;
		}

		if (lpObj->m_PlayerData->m_bUsePartyMatching)
		{
			pMsg.bResult = 8; bRet = 0;
		}

		if (bRet)
		{
			g_GensSystem.ReqRegGensMember(lpObj, lpMsg->bResult);
		}
		else
		{
			pMsg.bIndexH = SET_NUMBERH(iIndex);
			pMsg.bIndexL = SET_NUMBERL(iIndex);
			DGAnsRegGensMember((_tagPMSG_ANS_REG_GENS_MEMBER_EXDB*)&pMsg);
		}
	}
}

//-> Completed
void GameProtocol::CGReqSecedeGensMember(_tagPMSG_REQ_SEGEDE_GENS_MEMBER *lpMsg, int iIndex)
{
	if (!ObjectMaxRange(iIndex))
		return;

	LPOBJ lpObj = &gObj[iIndex];

	unsigned char bRet = 1;

	if (gObjIsConnectedGP(iIndex))
	{
		_tagPMSG_ANS_SECEDE_GENS_MEMBER_EXDB pMsg;	

		if (!g_GensSystem.IsInfluenceNPC(lpObj))
		{
			pMsg.bResult = 3; bRet = 0;
		}

		else if (lpObj->m_PlayerData->GuildStatus == GUILD_MASTER)
		{
			pMsg.bResult = 2; bRet = 0;
		}

		//[GensSystem] [%s][%s] Gens Leaving Failed to Not Register Gens
		else if (!g_GensSystem.GetGensInfluence(lpObj))
		{
			pMsg.bResult = 1; bRet = 0;
		}

		else if (lpObj->m_PlayerData->m_bUsePartyMatching)
		{
			pMsg.bResult = 4; bRet = 0;
		}

		else
		{
			pMsg.bResult = 0; bRet = 1;
		}

		if (bRet)
		{
			g_GensSystem.ReqSecedeGensMember(lpObj);
		}

		else
		{
			pMsg.bIndexH = SET_NUMBERH(iIndex); 
			pMsg.bIndexL = SET_NUMBERL(iIndex);
			DGAnsSecedeGensMember((_tagPMSG_ANS_SECEDE_GENS_MEMBER_EXDB*)&pMsg);
		}
	}
}

//-> Completed
void GameProtocol::CGReqGensReward(_tagPMSG_GENS_REWARD_CODE *lpMsg, int iIndex)
{
	if (!ObjectMaxRange(iIndex))
		return;

	unsigned char bRet = 0; //Not Used

	LPOBJ lpObj = &gObj[iIndex];

	if (gObjIsConnectedGP(iIndex))
	{
		g_GensSystem.ReqExDBGensRewardCheck(lpObj, lpMsg->bReward);
	}
}

//-> Completed
void GameProtocol::CGReqGensMemberInfo(_tagPMSG_REQ_GENS_INFO *lpMsg, int iIndex)
{
	if (!ObjectMaxRange(iIndex)) return;

	LPOBJ lpObj = &gObj[iIndex];

	if (gObjIsConnectedGP(iIndex))
	{
		if (g_GensSystem.IsRegGensInfluence(lpObj))
		{
			g_GensSystem.ReqExDBGensInfo(lpObj);
		}
	}
}
void GameProtocol::CGInventoryEquipment(_tagPMSG_REQ_INVENTORY_EQUIPMENT_ITEM *lpMsg, int iIndex)
{
	if ( ObjectMaxRange(iIndex) == FALSE )
	{
		return;
	}

	if ( gObj[iIndex].Type != OBJ_USER )
	{
		return;
	}

	if (!gObjIsConnected(iIndex))
	{
		return;
	}

	LPOBJ lpObj = &gObj[iIndex];

	_tagPMSG_ANS_INVENTORY_EQUIPMENT_ITEM pMsg = {0};

	pMsg.btResult = 0;
	PHeadSubSetB((LPBYTE)&pMsg, 0xBF, 0x20, sizeof(pMsg));

	pMsg.btItemPos = lpMsg->btItemPos;

	if (!IsInvenPet(lpObj->pInventory[lpMsg->btItemPos].m_Type) &&
		lpObj->pInventory[lpMsg->btItemPos].m_Durability == 0.0)
	{
		pMsg.btResult = -1;
		IOCP.DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	switch ( lpObj->pInventory[lpMsg->btItemPos].m_Type )
	{
		case ITEMGET(13,128):
		case ITEMGET(13,129):
			if (lpObj->m_PlayerData->m_btSculptPos == 0)
			{
				if (lpMsg->btValue == (BYTE)-2 && lpObj->pInventory[lpMsg->btItemPos].m_Durability == 255.0)
				{
					lpObj->m_PlayerData->m_btSculptPos = lpMsg->btItemPos;
					lpObj->pInventory[lpMsg->btItemPos].m_Durability = 254.0;
					pMsg.btResult = -2;
				}

				goto DEF_SWITCH;
			}

			if (lpObj->m_PlayerData->m_btSculptPos < INVETORY_WEAR_SIZE || lpObj->m_PlayerData->m_btSculptPos >= MAIN_INVENTORY_SIZE)
			{
				goto DEF_SWITCH;
			}

			if (lpMsg->btValue == (BYTE)-1 && lpObj->m_PlayerData->m_btSculptPos == lpMsg->btItemPos)
			{
				if (lpObj->pInventory[lpMsg->btItemPos].m_Durability == 254.0)
				{
					lpObj->pInventory[lpMsg->btItemPos].m_Durability = 255.0;
					pMsg.btResult = -1;
				}

				goto DEF_SWITCH;
			}

			this->GCServerMsgStringSend(Lang.GetText(0,548), iIndex, 1);
			IOCP.DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);
			break;
		case ITEMGET(13,130):
		case ITEMGET(13,131):
		case ITEMGET(13,132):
		case ITEMGET(13,133):
			if (lpObj->m_PlayerData->m_btCharmPos == 0)
			{
				if (lpMsg->btValue == (BYTE)-2 && lpObj->pInventory[lpMsg->btItemPos].m_Durability == 255.0)
				{
					lpObj->m_PlayerData->m_btCharmPos = lpMsg->btItemPos;
					lpObj->pInventory[lpMsg->btItemPos].m_Durability = 254.0;
					pMsg.btResult = -2;
				}

				goto DEF_SWITCH;
			}

			if (lpObj->m_PlayerData->m_btCharmPos < INVETORY_WEAR_SIZE || lpObj->m_PlayerData->m_btCharmPos >= MAIN_INVENTORY_SIZE)
			{
				goto DEF_SWITCH;
			}

			if (lpMsg->btValue == (BYTE)-1 && lpObj->m_PlayerData->m_btCharmPos == lpMsg->btItemPos)
			{
				if (lpObj->pInventory[lpMsg->btItemPos].m_Durability == 254.0)
				{
					lpObj->pInventory[lpMsg->btItemPos].m_Durability = 255.0;
					pMsg.btResult = -1;
				}

				goto DEF_SWITCH;
			}

			this->GCServerMsgStringSend(Lang.GetText(0,548), iIndex, 1);
			IOCP.DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);
			break;
		case ITEMGET(13,134):
			if (lpObj->m_PlayerData->m_btArtifactPos == 0)
			{
				if (lpMsg->btValue == (BYTE)-2 && lpObj->pInventory[lpMsg->btItemPos].m_Durability == 255.0)
				{
					lpObj->m_PlayerData->m_btArtifactPos = lpMsg->btItemPos;
					lpObj->pInventory[lpMsg->btItemPos].m_Durability = 254.0;
					pMsg.btResult = -2;
				}

				goto DEF_SWITCH;
			}

			if (lpObj->m_PlayerData->m_btArtifactPos < INVETORY_WEAR_SIZE || lpObj->m_PlayerData->m_btArtifactPos >= MAIN_INVENTORY_SIZE)
			{
				goto DEF_SWITCH;
			}

			if (lpMsg->btValue == (BYTE)-1 && lpObj->m_PlayerData->m_btArtifactPos == lpMsg->btItemPos)
			{
				if (lpObj->pInventory[lpMsg->btItemPos].m_Durability == 254.0)
				{
					lpObj->pInventory[lpMsg->btItemPos].m_Durability = 255.0;
					pMsg.btResult = -1;
				}

				goto DEF_SWITCH;
			}

			this->GCServerMsgStringSend(Lang.GetText(0,548), iIndex, 1);
			IOCP.DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);
			break;
		case ITEMGET(13,177):
		case ITEMGET(13,178):
		case ITEMGET(13,179):
			if (lpObj->m_PlayerData->m_btExpUpCharmPos == 0)
			{
				if (lpMsg->btValue == (BYTE)-2 && lpObj->pInventory[lpMsg->btItemPos].m_Durability == 255.0)
				{
					lpObj->m_PlayerData->m_btExpUpCharmPos = lpMsg->btItemPos;
					lpObj->pInventory[lpMsg->btItemPos].m_Durability = 254.0;
					pMsg.btResult = -2;
				}

				goto DEF_SWITCH;
			}

			if (lpObj->m_PlayerData->m_btExpUpCharmPos < INVETORY_WEAR_SIZE || lpObj->m_PlayerData->m_btExpUpCharmPos >= MAIN_INVENTORY_SIZE)
			{
				goto DEF_SWITCH;
			}

			if (lpMsg->btValue == (BYTE)-1 && lpObj->m_PlayerData->m_btExpUpCharmPos == lpMsg->btItemPos)
			{
				if (lpObj->pInventory[lpMsg->btItemPos].m_Durability == 254.0)
				{
					lpObj->pInventory[lpMsg->btItemPos].m_Durability = 255.0;
					pMsg.btResult = -1;
				}

				goto DEF_SWITCH;
			}

			this->GCServerMsgStringSend(Lang.GetText(0,548), iIndex, 1);
			IOCP.DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);
			break;
		case ITEMGET(13,2):
		case ITEMGET(13,3):
		case ITEMGET(13,4):
		case ITEMGET(13,37):
			if (lpObj->m_btInvenPetPos != 0)
			{
				if (lpObj->m_btInvenPetPos >= INVETORY_WEAR_SIZE && lpObj->m_btInvenPetPos < MAIN_INVENTORY_SIZE)
				{
					if (lpMsg->btValue != (BYTE)-1 || lpObj->m_btInvenPetPos != lpMsg->btItemPos)
					{
						this->GCServerMsgStringSend(Lang.GetText(0,615), iIndex, 1);
						return;
					}

					g_Log.Add("[CRenewal][InvenPet][ReqTakeOffPet][%s][%s]", lpObj->AccountID, lpObj->Name);

					if (gObjUseInvenPetCheck(lpObj, &lpObj->pInventory[lpMsg->btItemPos], 0) == FALSE)
					{
						g_Log.Add("[CRenewal][InvenPet][ReqTakeOffPet][%s][%s] PetIndex:[%d],Pos:[%d], Map:[%d] [Fail] Cannot Take Off InvenPet.",
							lpObj->AccountID, lpObj->Name, lpObj->pInventory[lpMsg->btItemPos].m_Type, lpMsg->btItemPos, lpObj->MapNumber);

						this->GCServerMsgStringSend(Lang.GetText(0,616), iIndex, 1);
						return;
					}

					if (lpObj->pInventory[lpMsg->btItemPos].m_JewelOfHarmonyOption == 1)
					{
						g_Log.Add("[CRenewal][InvenPet][ReqTakeOffPet][%s][%s] PetIndex:[%d],Pos:[%d], Map:[%d] [Success] Take Off InvenPet.",
							lpObj->AccountID, lpObj->Name, lpObj->pInventory[lpMsg->btItemPos].m_Type, lpMsg->btItemPos, lpObj->MapNumber);

						lpObj->pInventory[lpMsg->btItemPos].m_JewelOfHarmonyOption = 0;
						lpObj->m_wInvenPet = -1;
						pMsg.btResult = -1;
					}
				}
			}

			else if (lpMsg->btValue == (BYTE)-2)
			{
				g_Log.Add("[CRenewal][InvenPet][ReqRidePet][%s][%s]", lpObj->AccountID, lpObj->Name);

				if (lpObj->m_wInvenPet != (WORD)-1 || lpObj->pInventory[lpMsg->btItemPos].m_JewelOfHarmonyOption == 1)
				{
					g_Log.Add("[CRenewal][InvenPet][ReqRidePet][%s][%s] [Fail] Already Ride InvenPet",
						lpObj->AccountID, lpObj->Name);

					this->GCServerMsgStringSend(Lang.GetText(0,615), iIndex, 1);
					return;
				}

				if (gObjUseInvenPetCheck(lpObj, &lpObj->pInventory[lpMsg->btItemPos], 1) == FALSE)
				{
					g_Log.Add("[CRenewal][InvenPet][ReqRidePet][%s][%s] Map:[%d] [Fail] Cannot Ride InvenPet",
						lpObj->AccountID, lpObj->Name, lpObj->MapNumber);

					this->GCServerMsgStringSend(Lang.GetText(0,617), iIndex, 1);
					return;
				}

				lpObj->m_btInvenPetPos = lpMsg->btItemPos;
				lpObj->pInventory[lpMsg->btItemPos].m_JewelOfHarmonyOption = 1;
				pMsg.btResult = (BYTE)-2;

				g_Log.Add("[CRenewal][InvenPet][ReqRidePet][%s][%s] PetIndex:[%d], Pos:[%d], Map:[%d]  [Success] Ride InvenPet",
					lpObj->AccountID, lpObj->Name, lpObj->pInventory[lpMsg->btItemPos].m_Type, lpMsg->btItemPos, lpObj->MapNumber);
			}

			gObjMakePreviewCharSet(iIndex);
			this->GCEquipmentChange(iIndex, lpMsg->btItemPos);
			IOCP.DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);
			break;
		default:
DEF_SWITCH:
			gObjCalCharacter.CalcCharacter(iIndex);
			IOCP.DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);
			break;
	}
}

void GameProtocol::CGReqUseTransformationRing(PMSG_REQ_USETRANSFORMATIONRING * lpMsg, int aIndex)
{
	if ( ObjectMaxRange(aIndex) == false )
	{
		return;
	}

	if ( gObj[aIndex].Type != OBJ_USER )
	{
		return;
	}

	if (lpMsg->btState == 1)
	{
		if (IsTransformCharacterSkin(gObj[aIndex].m_Change) == TRUE)
		{
			gObj[aIndex].m_PlayerData->m_EnableUseChangeSkin = 0;
		}
	}

	else
	{
		gObj[aIndex].m_PlayerData->m_EnableUseChangeSkin = 1;
	}

	PMSG_MOVE pMove;

	pMove.h.c = 0xC1;
	pMove.h.headcode = 0xD4;
	pMove.h.size = sizeof(pMove);
	pMove.X = gObj[aIndex].X;
	pMove.Y = gObj[aIndex].Y;

	PMoveProc(&pMove, aIndex);
	gObjViewportListProtocolCreate(&gObj[aIndex]);
}

void GameProtocol::GCSendAttackSpeed(int aIndex)
{
	PMSG_ATTACKSPEEDSEND pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0xEC, 0x30, sizeof(pMsg));

	pMsg.AttackSpeed = gObj[aIndex].m_AttackSpeed;
	pMsg.MagicSpeed = gObj[aIndex].m_MagicSpeed;

	IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void GameProtocol::CGReqArcaBattleGuildMasterJoin(_tagPMSG_REQ_ARCA_BATTLE_JOIN *lpMsg, int iIndex)
{
	if (ObjectMaxRange(iIndex) == false)
	{
		return;
	}

	LPOBJ lpObj = &gObj[iIndex];

	if (gObjIsConnected(iIndex) == false)
	{
		return;
	}

	_GUILD_INFO_STRUCT * lpGuildInfo = lpObj->m_PlayerData->lpGuild;
	_tagPMSG_ANS_ARCA_BATTLE_GUILD_JOIN_DS pMsg;

	pMsg.wNumber = iIndex;

	if (!lpGuildInfo)
	{
		pMsg.btResult = 1;
		DGAnsArcaBattleGuildJoinSelect((_tagPMSG_ANS_ARCA_BATTLE_GUILD_JOIN_DS *)&pMsg);
		return;
	}

	int iArcaBattleState = g_ArcaBattle.GetState();

	if (iArcaBattleState != 3)
	{
		pMsg.btResult = 5;
		DGAnsArcaBattleGuildJoinSelect((_tagPMSG_ANS_ARCA_BATTLE_GUILD_JOIN_DS *)&pMsg);
		return;
	}

	if (lpObj->m_PlayerData->GuildStatus != GUILD_MASTER)
	{
		pMsg.btResult = 1;
		DGAnsArcaBattleGuildJoinSelect((_tagPMSG_ANS_ARCA_BATTLE_GUILD_JOIN_DS *)&pMsg);
		return;
	}

	if (lpGuildInfo->Count < g_ArcaBattle.GetJoinMemberCnt())
	{
		pMsg.btResult = 2;
		DGAnsArcaBattleGuildJoinSelect((_tagPMSG_ANS_ARCA_BATTLE_GUILD_JOIN_DS *)&pMsg);
		return;
	}

	g_ArcaBattle.GDReqArcaBattleIsTopRank(lpObj->m_Index, lpObj->m_PlayerData->GuildNumber);
}

void GameProtocol::CGReqArcaBattleGuildMemberJoin(_tagPMSG_REQ_ARCA_BATTLE_JOIN *lpMsg, int iIndex)
{
	_tagPMSG_ANS_ARCA_BATTLE_GUILD_MEMBER_JOIN_DS pMsg;

	if (ObjectMaxRange(iIndex) == false)
	{
		return;
	}

	LPOBJ lpObj = &gObj[iIndex];

	if (gObjIsConnected(iIndex) == false)
	{
		return;
	}

	pMsg.wNumber = iIndex;

	if (lpObj->m_PlayerData->lpGuild == NULL)
	{
		pMsg.btResult = 7;
		DGAnsArcaBattleGuildMemberJoin((_tagPMSG_ANS_ARCA_BATTLE_GUILD_MEMBER_JOIN_DS *)&pMsg);
		return;
	}

	if (lpObj->m_PlayerData->GuildStatus == GUILD_MASTER)
	{
		pMsg.btResult = 13;
		DGAnsArcaBattleGuildMemberJoin((_tagPMSG_ANS_ARCA_BATTLE_GUILD_MEMBER_JOIN_DS *)&pMsg);
		return;
	}

	int iArcaBattleState = g_ArcaBattle.GetState();

	if (iArcaBattleState < 4)
	{
		pMsg.btResult = 14;
		DGAnsArcaBattleGuildMemberJoin((_tagPMSG_ANS_ARCA_BATTLE_GUILD_MEMBER_JOIN_DS *)&pMsg);
		return;
	}

	if (iArcaBattleState >= 7)
	{
		pMsg.btResult = 10;
		DGAnsArcaBattleGuildMemberJoin((_tagPMSG_ANS_ARCA_BATTLE_GUILD_MEMBER_JOIN_DS *)&pMsg);
		return;
	}

	GDReqArcaBattleGuildMemberJoin(lpObj);
}

void GameProtocol::CGReqArcaBattleEnter(_tagPMSG_REQ_ARCA_BATTLE_ENTER *lpMsg, int iIndex)
{
	_tagPMSG_ANS_ARCA_BATTLE_ENTER_DS pMsg; 

	if ( ObjectMaxRange(iIndex) == false )
	{
		return;
	}

	LPOBJ lpObj = &gObj[iIndex];

	if ( gObjIsConnected(iIndex) )
	{
		int iArcaBattleState = g_ArcaBattle.GetState();

		if ( iArcaBattleState == 6 || iArcaBattleState == 7 )
		{
			if (lpObj->m_PlayerData->m_bUsePartyMatching)
			{
				PMSG_ANS_PARTYMATCHING_ERROR pMsg;
				PHeadSubSetB((LPBYTE)&pMsg, 0xEF, 0x09, sizeof(pMsg));
				pMsg.iResult = -1;

				IOCP.DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);
				return;
			}

			if ( lpObj->Teleport )
			{
				return;
			}

			BYTE btAttr = MapC[lpObj->MapNumber].GetAttr(lpObj->X,lpObj->Y);

			if ( btAttr & 1 )
				GDReqArcaBattleEnter(lpObj, lpMsg->btEnterSeq);
		}

		else
		{
			pMsg.wNumber = iIndex;
			pMsg.btResult = 12;
			DGAnsArcaBattleEnter((_tagPMSG_ANS_ARCA_BATTLE_ENTER_DS *)&pMsg);
		}
	}
}

void GameProtocol::CGReqAcheronEnter(_tagPMSG_REQ_ACHERON_ENTER *lpMsg, int iIndex)
{
	_tagPMSG_ANS_ACHERON_ENTER pMsg;

	if (ObjectMaxRange(iIndex) == false)
	{
		return;
	}

	LPOBJ lpObj = &gObj[iIndex];

	if (gObjIsConnected(iIndex) == false)
	{
		return;
	}

	for (int pos = INVETORY_WEAR_SIZE; pos < MAIN_INVENTORY_SIZE; pos++)
	{
		if (lpObj->pInventory[pos].IsItem() && lpObj->pInventory[pos].m_Type == ITEMGET(13, 146))
		{
			gObjInventoryDeleteItem(iIndex, pos);
			this->GCInventoryItemDeleteSend(iIndex, pos, 1);
			gObjMoveGate(iIndex, 417);
			return;
		}
	}

	pMsg.h.c = 0xC1;
	pMsg.h.size = sizeof(pMsg);
	pMsg.h.headcode = 0xF8;
	pMsg.h.subcode = 0x21;
	pMsg.btResult = 1;
	IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pMsg, sizeof(pMsg));
}

void GameProtocol::CGReqArcaBattleBootyExchange(int iIndex)
{
	if (ObjectMaxRange(iIndex) == false)
	{
		return;
	}
	LPOBJ lpObj = &gObj[iIndex];

	if (gObjIsConnected(iIndex) == false)
	{
		return;
	}

	if (bCanChaosBox == TRUE && !lpObj->m_IfState.use)
	{
		if (lpObj->m_bPShopOpen == TRUE)
		{
			g_Log.Add("[%s][%s] is Already Opening PShop, ChaosBox Failed", lpObj->AccountID, lpObj->Name);
			this->GCServerMsgStringSend(Lang.GetText(0,531), iIndex, 1);
			return;
		}

		lpObj->m_IfState.use = 1;
		lpObj->m_IfState.type = 7;
		gObjInventoryTrans(lpObj->m_Index);
		g_Log.Add("[ArcaBattle][BootyItemMix] [%s][%s]  Booty Item Mix Start", lpObj->AccountID, lpObj->Name);
		gObjItemTextSave(lpObj);
		gObjWarehouseTextSave(lpObj);
	}
}

void GameProtocol::CGReqSpritemapExchange(int iIndex)
{
	if ( ObjectMaxRange(iIndex) == false )
	{
		return;
	}

    LPOBJ lpObj = &gObj[iIndex];

	if ( gObjIsConnected(iIndex))
	{
		if ( bCanChaosBox == 1 && !(lpObj->m_IfState.type & 3) )
		{
			if ( lpObj->m_bPShopOpen == 1 )
			{
				g_Log.Add("[%s][%s] is Already Opening PShop, ChaosBox Failed", lpObj->AccountID, lpObj->Name);
				this->GCServerMsgStringSend(Lang.GetText(0,531), iIndex, 1);
			}

			else
			{
				lpObj->m_IfState.use = 1;
				lpObj->m_IfState.type = 7;

				gObjInventoryTrans(lpObj->m_Index);
				g_Log.Add("[ArcaBattle][BootyItemMix] [%s][%s]  Booty Item Mix Start", lpObj->AccountID, lpObj->Name);
				gObjItemTextSave((OBJECTSTRUCT *)lpObj);
				gObjWarehouseTextSave((OBJECTSTRUCT *)lpObj);
			}
		}
	}
}

void GameProtocol::CGReqRegisteredMemberCnt(int aIndex)
{
	_tagPMSG_REQ_AB_REG_MEMBER_CNT_DS pMsg; 
	_tagPMSG_ANS_AB_REG_MEMBER_CNT pAnsMsg; 

	if ( ObjectMaxRange(aIndex) )
	{
		LPOBJ lpObj = &gObj[aIndex];

		if ( gObjIsConnected(aIndex) )
		{
			pAnsMsg.h.c = 0xC1;
			pAnsMsg.h.size = sizeof(pMsg);
			pAnsMsg.h.headcode = 0xF8;
			pAnsMsg.h.subcode = 0x42;
			pAnsMsg.btRegMemCnt = 0;

			if ( lpObj->m_PlayerData->lpGuild )
			{
				pMsg.h.c = 0xC1;
				pMsg.h.size = sizeof(pMsg);
				pMsg.h.headcode = 0xF8;
				pMsg.h.subcode = 0x45;
				pMsg.iIndex = aIndex;
				pMsg.iGuildNumber = lpObj->m_PlayerData->GuildNumber;
				wsDataCli.DataSend((char*)&pMsg, sizeof(pMsg));
			}

			else
			{
				pAnsMsg.btResult = 1;
				IOCP.DataSend(aIndex, (LPBYTE)&pAnsMsg, sizeof(pMsg));
			}
		}
	}
}

void GameProtocol::CGReqEnterDoppelGanger(PMSG_REQ_ENTER_DOPPELGANGER *lpMsg, int iIndex)
{
	if ( ObjectMaxRange(iIndex) == false )
	{
		g_Log.Add("return %s %d", __FILE__, __LINE__);
		return;
	}

	g_DoppelGanger.EnterDoppelgangerEvent(iIndex, lpMsg->btPos);
}

void GameProtocol::CGReqEnterZone(PMSG_REQ_ENTER_ZONE * lpMsg, int iIndex)
{
	if ( ObjectMaxRange(iIndex) == false )
	{
		g_Log.Add("return %s %d", __FILE__, __LINE__);
		return;
	}

	if ( lpMsg->btResult == TRUE )
	{
		g_ImperialGuardian.CGEnterPortal(iIndex, 0);
	}
}

void GameProtocol::GCSendRecvCheck(int aIndex)
{
	PMSG_RECV_TRAP pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xFA, 0x9F, sizeof(pMsg));

	pMsg.wUserIndex = aIndex;
	pMsg.CurrTime = time(NULL);

	IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void GameProtocol::CGReqAntihackBreach(int aIndex, PMSG_ANTIHACK_BREACH *lpMsg)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Type != OBJ_USER)
	{
		return;
	}

	g_Log.AddC(TColor::Red, "[%s][%s][%s][%s] AntiHack breach -> Code:%d",
		lpObj->AccountID, lpObj->Name, lpObj->m_PlayerData->Ip_addr, lpObj->m_PlayerData->HWID, lpMsg->dwErrorCode);

	AntiHackLog->Output("[%s][%s][%s][%s] AntiHack breach -> Code: %d",
		lpObj->AccountID, lpObj->Name, lpObj->m_PlayerData->Ip_addr, lpObj->m_PlayerData->HWID, lpMsg->dwErrorCode);

	if (g_ConfigRead.antihack.AntiHackBreachDisconnectUser == true)
	{
		this->GCSendDisableReconnect(aIndex);
		//IOCP.CloseClient(aIndex);
	}
}

void GameProtocol::CGReqAntihackCheck(int aIndex, PMSG_ANTIHACK_CHECK *lpMsg)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Type != OBJ_USER)
	{
		return;
	}

	BYTE MainRecvHeader[] = { 0x8B, 0xFF, 0x55, 0x8B, 0xEC };

	if (memcmp(lpMsg->checkdata, MainRecvHeader, 5) != 0 && g_ConfigRead.antihack.AntiHackRecvHookProtect == true) // data is wrong, recv is probably hooked
	{
		g_Log.AddC(TColor::Red, "[%s][%s][%s][%s] AntiHack breach -> Recv header data is wrong",
			lpObj->AccountID, lpObj->Name, lpObj->m_PlayerData->Ip_addr, lpObj->m_PlayerData->HWID);

		AntiHackLog->Output("[%s][%s][%s][%s] AntiHack breach -> Recv header data is wrong",
			lpObj->AccountID, lpObj->Name, lpObj->m_PlayerData->Ip_addr, lpObj->m_PlayerData->HWID);
	}

	else
	{
		lpObj->m_PlayerData->AntiHackCheckTime = GetTickCount();
	}
}

void GameProtocol::CGReqInJewelPentagramItem(PMSG_REQ_IN_PENTAGRAM_JEWEL *lpMsg, int aIndex)
{
	if (!lpMsg)
	{
		return;
	}

	if (!g_PentagramSystem.PentagramJewel_IN(aIndex, lpMsg->iPentagramPos, lpMsg->iJewelPos))
		this->GCAnsInJewelPentagramItem(aIndex, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
}

void GameProtocol::GCAnsInJewelPentagramItem(int aIndex, int iResult, BYTE btJewelPos, BYTE btJewelIndex, BYTE btItemType, WORD wItemIndex, BYTE btMainAttribute, BYTE btLevel, BYTE btRank1OptionNum, BYTE btRank1Level, BYTE btRank2OptionNum, BYTE btRank2Level, BYTE btRank3OptionNum, BYTE btRank3Level, BYTE btRank4OptionNum, BYTE btRank4Level, BYTE btRank5OptionNum, BYTE btRank5Level)
{
	PMSG_ANS_IN_PENTAGRAM_JEWEL pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xEC, 0x00, sizeof(pMsg));

	pMsg.btResult = iResult;
	pMsg.btJewelPos = btJewelPos;
	pMsg.btJewelIndex = btJewelIndex;
	pMsg.btItemType = btItemType;
	pMsg.wItemIndex = wItemIndex;
	pMsg.btMainAttribute = btMainAttribute;
	pMsg.btLevel = btLevel;
	pMsg.btRank1OptionNum = btRank1OptionNum;
	pMsg.btRank1Level = btRank1Level;
	pMsg.btRank2OptionNum = btRank2OptionNum;
	pMsg.btRank2Level = btRank2Level;
	pMsg.btRank3OptionNum = btRank3OptionNum;
	pMsg.btRank3Level = btRank3Level;
	pMsg.btRank4OptionNum = btRank4OptionNum;
	pMsg.btRank4Level = btRank4Level;
	pMsg.btRank5OptionNum = btRank5OptionNum;
	pMsg.btRank5Level = btRank5Level;

	IOCP.DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
}

void GameProtocol::CGReqOutJewelPentagramItem(PMSG_REQ_OUT_PENTAGRAM_JEWEL *lpMsg, int aIndex)
{
	if (!lpMsg)
	{
		return;
	}

	BYTE btJewelPos;
	BYTE btJewelDBIndex;

	int iResult = g_PentagramSystem.PentagramJewel_OUT(aIndex, lpMsg->iPentagramPos, lpMsg->btSocketIndex, &btJewelPos, &btJewelDBIndex);
	this->GCAnsOutJewelPentagramItem(aIndex, iResult, btJewelPos, btJewelDBIndex);

	if (iResult != 1)
	{
		this->GCAnsPentagramJewelInOut(aIndex, iResult);
	}
}

void GameProtocol::GCAnsOutJewelPentagramItem(int iIndex, int iResult, BYTE btJewelPos, BYTE btJewelDBIndex)
{
	PMSG_ANS_OUT_PENTAGRAM_JEWEL pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xEC, 0x01, sizeof(pMsg));

	pMsg.Result = iResult;
	pMsg.btJewelPos = btJewelPos;
	pMsg.btJewelDBIndex = btJewelDBIndex;

	IOCP.DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void GameProtocol::CGReqRefinePentagramJewel(PMSG_REQ_REFINE_PENTAGRAM_JEWEL *lpMsg, int aIndex)
{
	if (!lpMsg)
	{
		return;
	}

	if (gObj[aIndex].Type != OBJ_USER)
	{
		return;
	}

	gObj[aIndex].m_PlayerData->m_bIsPentagramMixCompleted = true;
	BYTE btRefineResult = g_PentagramMixSystem.PentagramJewelRefine(aIndex, lpMsg->btRefineKind);

	if (btRefineResult != 1)
	{
		this->GCAnsRefinePentagramJewel(aIndex, btRefineResult);
	}
}

void GameProtocol::GCAnsRefinePentagramJewel(int iIndex, BYTE btResult)
{
	PMSG_CHAOSMIXRESULT pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x86, sizeof(pMsg));

	pMsg.Result = btResult;
	IOCP.DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void GameProtocol::CGReqUpgradePentagramJewel(PMSG_REQ_UPGRADE_PENTAGRAM_JEWEL *lpMsg, int aIndex)
{
	if (!lpMsg)
	{
		return;
	}

	if (gObj[aIndex].Type != OBJ_USER)
	{
		return;
	}

	gObj[aIndex].m_PlayerData->m_bIsPentagramMixCompleted = true;
	BYTE btUpgradeResult = g_PentagramMixSystem.PentagramJewel_Upgrade(aIndex, lpMsg->btUpgradeType, lpMsg->btTargetValue);

	if (btUpgradeResult != 1)
	{
		this->GCAnsUpgradePentagramJewel(aIndex, btUpgradeResult);
	}
}

void GameProtocol::GCAnsUpgradePentagramJewel(int iIndex, BYTE btResult)
{
	PMSG_CHAOSMIXRESULT pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x86, sizeof(pMsg));

	pMsg.Result = btResult;
	IOCP.DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void GameProtocol::GCAnsPentagramJewelInOut(int iIndex, BYTE btResult)
{
	PMSG_PENTAGRAM_JEWEL_INOUT pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xEC, 0x04, sizeof(pMsg));

	pMsg.btResult = btResult;
	IOCP.DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void GameProtocol::CGReqGuildMatchingList(PMSG_REQ_GUILDMATCHINGLIST *lpMsg, int nUserIndex)
{
	if (ObjectMaxRange(nUserIndex) == false)
	{
		return;
	}

	if (gObjIsConnected(nUserIndex) == false)
	{
		return;
	}

	GDReqGuildMatchingList(nUserIndex, lpMsg->nPage);
}

void GameProtocol::CGReqGuildMatchingListSearchWord(PMSG_REQ_GUILDMATCHINGLISTSEARCHWORD *lpMsg, int nUserIndex)
{
	if (ObjectMaxRange(nUserIndex) == false)
	{
		return;
	}

	if (gObjIsConnected(nUserIndex) == false)
	{
		return;
	}

	int nPage = lpMsg->nPage;
	char szSearchWord[11];

	memcpy(szSearchWord, lpMsg->szSearchWord, 11);
	GDReqGuildMatchingListSearchWord(nUserIndex, nPage, szSearchWord);
}

void GameProtocol::CGReqRegGuildMatchingList(PMSG_REQ_REGGUILDMATCHINGDATA *lpMsg, int nUserIndex)
{
	if (ObjectMaxRange(nUserIndex) == false)
	{
		return;
	}

	if (gObjIsConnected(nUserIndex) == false)
	{
		return;
	}

	LPOBJ lpObj = &gObj[nUserIndex];

	if (lpObj->Type != OBJ_USER) 
	{
		return;
	}

	int nGuildNumber = lpObj->m_PlayerData->GuildNumber;
	_GUILD_INFO_STRUCT * lpstGuildInfo = Guild.SearchGuild_Number(nGuildNumber);
	_stGuildMatchingList stGuildMatchingList;

	PMSG_ANS_REGGUILDMATCHINGDATA pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xED, 0x02, sizeof(pMsg));

	if (!lpstGuildInfo || Guild.GetGuildMemberStatus(lpstGuildInfo->Name, lpObj->Name) != GUILD_MASTER)
	{
		pMsg.nResult = -4;
		g_Log.Add("[GMATCHING][GC][0xED][0x02] - %s ResultCode:%d ", lpObj->Name, -4);
		IOCP.DataSend(nUserIndex, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	if (lpObj->m_PK_Level > 3)
	{
		pMsg.nResult = -2;
		g_Log.Add("[GMATCHING][GC][0xED][0x02] - %s ResultCode:%d ", lpObj->Name, -2);
		IOCP.DataSend(nUserIndex, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	memcpy(stGuildMatchingList.szMemo, lpMsg->szMemo, 41);
	stGuildMatchingList.btInterestType = lpMsg->btInterestType;
	stGuildMatchingList.btLevelRange = lpMsg->btLevelRange;
	stGuildMatchingList.btClassType = lpMsg->btClassType;
	stGuildMatchingList.btGuildMasterClass = lpObj->Class;

	if (lpObj->m_PlayerData->ChangeUP == 1)
	{
		stGuildMatchingList.btGuildMasterClass |= 0x08;
	}

	if (lpObj->m_PlayerData->ChangeUP == 2)
	{
		stGuildMatchingList.btGuildMasterClass |= 0x08;
		stGuildMatchingList.btGuildMasterClass |= 0x10;
	}

	memcpy(stGuildMatchingList.szGuildName, lpObj->m_PlayerData->GuildName, MAX_GUILD_LEN + 1);
	memcpy(stGuildMatchingList.szRegistrant, lpObj->Name, MAX_ACCOUNT_LEN + 1);
	stGuildMatchingList.btGuildMemberCnt = lpstGuildInfo->Count;
	stGuildMatchingList.nGuildNumber = lpstGuildInfo->Number;
	stGuildMatchingList.nGuildMasterLevel = lpObj->Level + lpObj->m_PlayerData->MasterLevel;
	stGuildMatchingList.btGensType = g_GensSystem.GetGensInfluence(lpObj);
	
	GDReqRegGuildMatchingList(nUserIndex, stGuildMatchingList);
}

void GameProtocol::CGReqCancelGuildMatchingList(PMSG_REQ_CANCEL_GUILDMATCHINGLIST * lpMsg, int nUserIndex)
{
	if (ObjectMaxRange(nUserIndex) == false)
	{
		return;
	}

	LPOBJ lpObj = &gObj[nUserIndex];

	if (gObjIsConnected(nUserIndex) == false)
	{
		return;
	}

	int nGuildNumber = lpObj->m_PlayerData->GuildNumber;
	_GUILD_INFO_STRUCT * lpstGuildInfo = Guild.SearchGuild_Number(nGuildNumber);

	PMSG_ANS_CANCEL_GUILDMATCHINGLIST pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xED, 0x03, sizeof(pMsg));

	if (!lpstGuildInfo || Guild.GetGuildMemberStatus(lpstGuildInfo->Name, lpObj->Name) != GUILD_MASTER)
	{
		pMsg.nResult = -2;
		g_Log.Add("[GMATCHING][GC][0xED][0x03] - %s ResultCode:%d ", lpObj->Name, -2);
		IOCP.DataSend(nUserIndex, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	GDReqDelMatchingList(nUserIndex, nGuildNumber);
}

void GameProtocol::CGReqJoinGuildMatching(PMSG_REQ_JOIN_GUILDMATCHING *lpMsg, int nUserIndex)
{
	if (ObjectMaxRange(nUserIndex) == false)
	{
		return;
	}

	LPOBJ lpObj = &gObj[nUserIndex];

	if (gObjIsConnected(nUserIndex) == false)
	{
		return;
	}

	int nGuildNumber = lpObj->m_PlayerData->GuildNumber;
	int nTargetGuildNumber = lpMsg->nGuildNumber;

	PMSG_ANS_JOIN_GUILDMATCHING pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xED, 0x04, sizeof(pMsg));

	if (nGuildNumber > 0)
	{
		pMsg.nResult = -3;
		g_Log.Add("[GMATCHING][GC][0xED][0x04] - %s ResultCode:%d ", lpObj->Name, -3);
		IOCP.DataSend(nUserIndex, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	if (gObj[nUserIndex].m_PK_Level > 3)
	{
		pMsg.nResult = -2;
		g_Log.Add("[GMATCHING][GC][0xED][0x04] - %s ResultCode:%d ", lpObj->Name, -2);
		IOCP.DataSend(nUserIndex, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	_stGuildMatchingAllowListDB stAllowList;

	stAllowList.nGuildNumber = nTargetGuildNumber;
	memcpy(stAllowList.szApplicantName, lpObj->Name, MAX_ACCOUNT_LEN + 1);
	stAllowList.btApplicantClass = lpObj->Class;
	stAllowList.btState = 0;
	stAllowList.nApplicantLevel = lpObj->Level + lpObj->m_PlayerData->MasterLevel;

	if (lpObj->m_PlayerData->ChangeUP == 1)
	{
		stAllowList.btApplicantClass |= 0x08;
	}

	else if (lpObj->m_PlayerData->ChangeUP == 2)
	{
		stAllowList.btApplicantClass |= 0x08;
		stAllowList.btApplicantClass |= 0x10;
	}
	
	GDReqJoinGuildMatchingList(nUserIndex, stAllowList);
}

void GameProtocol::CGReqCancelJoinGuildMatching(int nUserIndex)
{
	if (ObjectMaxRange(nUserIndex) == false)
	{
		return;
	}

	LPOBJ lpObj = &gObj[nUserIndex];

	if (gObjIsConnected(nUserIndex) == false)
	{
		return;
	}

	GDReqCancelJoinGuildMatching(nUserIndex, lpObj->Name, 0);
}

void GameProtocol::CGReqAllowJoinGuildMatching(PMSG_REQ_ALLOW_JOIN_GUILDMATCHING *lpMsg, int nUserIndex)
{
	if (ObjectMaxRange(nUserIndex) == false)
	{
		return;
	}

	LPOBJ lpObj = &gObj[nUserIndex];

	if (gObjIsConnected(nUserIndex) == false)
	{
		return;
	}

	char szMemberName[MAX_ACCOUNT_LEN + 1];
	memcpy(szMemberName, lpMsg->szName, MAX_ACCOUNT_LEN + 1);

	if (gObjIsConnected(lpObj))
	{
		GDReqAllowJoinGuildMatching(nUserIndex, lpMsg->nAllowType, szMemberName, lpObj->m_PlayerData->GuildName);
	}
}

void GameProtocol::CGReqGetAllowListJoinGuildMatching(PMSG_REQ_ALLOWLIST_GUILDMATCHING *lpMsg, int nUserIndex)
{
	if (ObjectMaxRange(nUserIndex) == false)
	{
		return;
	}

	LPOBJ lpObj = &gObj[nUserIndex];

	if (gObjIsConnected(nUserIndex) == false)
	{
		return;
	}

	int nGuildNumber = lpObj->m_PlayerData->GuildNumber;
	_GUILD_INFO_STRUCT * lpstGuildInfo = Guild.SearchGuild_Number(nGuildNumber);

	PMSG_ANS_WAIT_GUILDMATCHING pMsg;
	PHeadSubSetW((LPBYTE)&pMsg, 0xED, 0x07, sizeof(pMsg));

	if (!lpstGuildInfo || Guild.GetGuildMemberStatus(lpstGuildInfo->Name, lpObj->Name) != GUILD_MASTER)
	{
		pMsg.nResult = -2;
		g_Log.Add("[GMATCHING][GC][0xED][0x07] - %s ResultCode:%d ", lpObj->Name, -2);
		IOCP.DataSend(nUserIndex, (LPBYTE)&pMsg, sizeof(pMsg));
		return;
	}

	GDReqWaitGuildMatchingList(nGuildNumber, nUserIndex);
}

void GameProtocol::CGReqGetWaitStateListGuildMatching(int nUserIndex)
{
	if (ObjectMaxRange(nUserIndex) == false)
	{
		return;
	}

	LPOBJ lpObj = &gObj[nUserIndex];

	if (gObjIsConnected(nUserIndex) == false)
	{
		return;
	}

	char szName[MAX_ACCOUNT_LEN + 1];
	memcpy(szName, lpObj->Name, MAX_ACCOUNT_LEN + 1);

	GDReqGetWaitStateListGuildMatching(nUserIndex, szName);
}

void GameProtocol::CGReqRegWantedPartyMember(PMSG_REQ_REG_WANTED_PARTYMEMBER *lpMsg, int nUserIndex)
{
	if (ObjectMaxRange(nUserIndex) == false)
	{
		return;
	}

	LPOBJ lpObj = &gObj[nUserIndex];

	if (gObjIsConnected(nUserIndex) == false)
	{
		return;
	}

	BOOL bAlreadyParty = FALSE;
	int nPartyNumber = -1;

	if (lpObj->PartyNumber != -1)
	{
		bAlreadyParty = TRUE;
		nPartyNumber = lpObj->PartyNumber;
	}

	_PARTY_INFO_LISTDB stList;

	if (bAlreadyParty)
	{
		stList.btCurPartyMemberCnt = gParty.GetCount(nPartyNumber);
	}

	else
	{
		stList.btCurPartyMemberCnt = 1;
	}

	stList.btUsePassword = lpMsg->btUsePassword;
	stList.btWantedClass = lpMsg->btWantedClass;
	stList.nHuntingGround = lpMsg->nHuntingGround;
	stList.nMaxLevel = lpMsg->nMaxLevel;
	stList.nMinLevel = lpMsg->nMinLevel;
	memcpy(stList.szLeaderName, lpObj->Name, MAX_ACCOUNT_LEN + 1);
	memcpy(stList.szPassword, lpMsg->szPassword, 5);
	memcpy(stList.szTitle, lpMsg->szTitle, 41);
	memcpy(stList.btWantedClassDetailInfo, lpMsg->btWantedClassDetailInfo, 7);
	stList.btGensType = g_GensSystem.GetGensInfluence(&gObj[nUserIndex]);
	stList.nLeaderLevel = gObj[nUserIndex].Level + gObj[nUserIndex].m_PlayerData->MasterLevel;
	stList.btLeaderClass = gObj[nUserIndex].Class;
	stList.btApprovalType = lpMsg->btApprovalType;

	if (gObj[nUserIndex].m_PlayerData->ChangeUP == 1)
	{
		stList.btLeaderClass |= 0x08;
	}

	else if (gObj[nUserIndex].m_PlayerData->ChangeUP == 2)
	{
		stList.btLeaderClass |= 0x08;
		stList.btLeaderClass |= 0x10;
	}

	if (gObj[nUserIndex].m_IfState.use != 0 && gObj[nUserIndex].m_IfState.type == 2)
	{
		PMSG_ANS_REG_WANTED_PARTYMEMBER pMsg;
		PHeadSubSetB((LPBYTE)&pMsg, 0xEF, 0x00, sizeof(pMsg));
		pMsg.nResult = -2;

		IOCP.DataSend(nUserIndex, (LPBYTE)&pMsg, sizeof(pMsg));
	}

	else
	{
		GDReqRegWantedPartyMember(stList, nUserIndex);
	}
}

void GameProtocol::CGReqGetPartyMatchingList(PMSG_REQ_GET_PARTYMATCHINGLIST * lpMsg, int nUserIndex)
{
	if (ObjectMaxRange(nUserIndex) == false)
	{
		return;
	}

	LPOBJ lpObj = &gObj[nUserIndex];

	if (gObjIsConnected(nUserIndex) == false)
	{
		return;
	}

	char szSearchWord[11];
	memset(szSearchWord, 0x00, sizeof(szSearchWord));

	if (lpMsg->btUseSearchWord == 0)
	{
		GDReqGetPartyMatchingList(lpMsg->nPage, szSearchWord, 0, nUserIndex);
	}

	else if (lpMsg->btUseSearchWord == 1)
	{
		memcpy(szSearchWord, lpMsg->szSearchWord, 10);
		GDReqGetPartyMatchingList(lpMsg->nPage, szSearchWord, 1, nUserIndex);
	}

	else if (lpMsg->btUseSearchWord == 2)
	{
		GDReqGetPartyMatchingList(lpMsg->nPage, szSearchWord, 2, nUserIndex);
	}
}

void GameProtocol::CGReqJoinMemberPartyMatching(PMSG_REQ_MEMBERJOIN_PARTYMATCHINGLIST *lpMsg, int nUserIndex)
{
	if (ObjectMaxRange(nUserIndex) == false)
	{
		return;
	}

	LPOBJ lpObj = &gObj[nUserIndex];

	if (gObjIsConnected(nUserIndex) == false)
	{
		return;
	}

	char szMemberName[MAX_ACCOUNT_LEN + 1];
	char szLeaderName[MAX_ACCOUNT_LEN + 1];
	char szPassword[5];

	memset(szMemberName, 0x00, MAX_ACCOUNT_LEN + 1);
	memset(szLeaderName, 0x00, MAX_ACCOUNT_LEN + 1);
	memset(szPassword, 0x00, 5);

	memcpy(szMemberName, lpObj->Name, MAX_ACCOUNT_LEN);
	memcpy(szLeaderName, lpMsg->szLeaderName, MAX_ACCOUNT_LEN);
	memcpy(szPassword, lpMsg->szPassword, 4);

	int nTotalLevel = lpObj->Level + lpObj->m_PlayerData->MasterLevel;
	BYTE btChangeUpClass = lpObj->Class;

	if (lpObj->m_PlayerData->ChangeUP == 1)
	{
		btChangeUpClass |= 0x08;
	}

	if (lpObj->m_PlayerData->ChangeUP == 2)
	{
		btChangeUpClass |= 0x08;
		btChangeUpClass |= 0x18;
	}

	BYTE btClass = lpObj->Class;

	switch (lpObj->Class)
	{
		case CLASS_WIZARD:
			btClass = 4;
			break;
		case CLASS_KNIGHT:
			btClass = 1;
			break;
		case CLASS_ELF:
			btClass = 2;
			break;
		case CLASS_MAGUMSA:
			btClass = 8;
			break;
		case CLASS_DARKLORD:
			btClass = 16;
			break;
		case CLASS_SUMMONER:
			btClass = 32;
			break;
		case CLASS_RAGEFIGHTER:
			btClass = 64;
			break;
		default:
			btClass = 0;
			break;
	}

	if (gObj[nUserIndex].m_IfState.use != 0 && gObj[nUserIndex].m_IfState.type == 2)
	{
		PMSG_ANS_MEMBERJOIN_PARTYMATCHINGLIST pMsg;
		PHeadSubSetB((LPBYTE)&pMsg, 0xEF, 0x02, sizeof(pMsg));
		pMsg.nResult = -4;

		IOCP.DataSend(nUserIndex, (LPBYTE)&pMsg, sizeof(pMsg));
	}

	else
	{
		BYTE btGensType = g_GensSystem.GetGensInfluence(&gObj[nUserIndex]);
		GDReqJoinMemberPartyMatching(szMemberName, szLeaderName,
			szPassword, nUserIndex, lpObj->DBNumber,
			nTotalLevel, btClass, lpMsg->btUseRandomParty,
			btGensType, btChangeUpClass);
	}
}

void GameProtocol::CGReqMemberJoinStateList(PMSG_REQ_MEMBERJOIN_STATELIST_PARTYMATCHING *lpMsg, int nUserIndex)
{
	if (ObjectMaxRange(nUserIndex) == false)
	{
		return;
	}

	LPOBJ lpObj = &gObj[nUserIndex];

	if (gObjIsConnected(nUserIndex) == false)
	{
		return;
	}

	GDReqMemberJoinStateList(lpObj->Name, nUserIndex);
}

void GameProtocol::CGReqMemberJoinStateListLeader(PMSG_REQ_WAITLIST_PARTYMATCHING *lpMsg, int nUserIndex)
{
	if (ObjectMaxRange(nUserIndex) == false)
	{
		return;
	}

	LPOBJ lpObj = &gObj[nUserIndex];

	if (gObjIsConnected(nUserIndex) == false)
	{
		return;
	}

	GDReqMemberJoinStateListLeader(lpObj->Name, nUserIndex);
}

void GameProtocol::CGReqAcceptJoinMemberPartyMatching(PMSG_REQ_ACCEPTMEMBER_PARTYMATCHING *lpMsg, int nUserIndex)
{
	if (ObjectMaxRange(nUserIndex) == false)
	{
		return;
	}

	LPOBJ lpObj = &gObj[nUserIndex];

	if (gObjIsConnected(nUserIndex) == false)
	{
		return;
	}

	char szMemberName[MAX_ACCOUNT_LEN + 1];
	char szLeaderName[MAX_ACCOUNT_LEN + 1];

	memset(szMemberName, 0x00, MAX_ACCOUNT_LEN + 1);
	memset(szLeaderName, 0x00, MAX_ACCOUNT_LEN + 1);

	memcpy(szMemberName, lpMsg->szMemberName, MAX_ACCOUNT_LEN);
	memcpy(szLeaderName, lpObj->Name, MAX_ACCOUNT_LEN);

	GDReqAcceptMemberJoin(nUserIndex, lpMsg->btType, szLeaderName, szMemberName, 0);
}

void GameProtocol::CGReqCancelPartyMatching(PMSG_REQ_CANCEL_JOIN_PARTYMATCHING *lpMsg, int nUserIndex)
{
	if (ObjectMaxRange(nUserIndex) == false)
	{
		return;
	}

	LPOBJ lpObj = &gObj[nUserIndex];

	if (gObjIsConnected(nUserIndex) == false)
	{
		return;
	}

	GDReqCancelPartyMatching(nUserIndex, lpMsg->btType);
}

#pragma pack (1)
struct stBuffList
{
	stBuffList()
	{
		this->btBuffIndex = 0;
		this->nBuffDuration = 0;
	}

	BYTE btBuffIndex;
	int nBuffDuration;
};

struct PMSG_BUFFLIST_SEND_PARTY
{
	PBMSG_HEAD h;
	char Name[MAX_ACCOUNT_LEN + 1];
	BYTE btBuffCount;
	stBuffList stBuffList[32];
};
#pragma pack ()

void GameProtocol::GCDisplayBuffeffectPartyMember(int aIndex)
{
	if (ObjectMaxRange(aIndex) == false)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (gObjIsConnected(aIndex) == false)
	{
		return;
	}

	if (lpObj->PartyNumber < 0)
	{
		return;
	}

	PMSG_BUFFLIST_SEND_PARTY pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x2E, sizeof(pMsg));

	pMsg.btBuffCount = lpObj->m_BuffEffectCount;

	if (g_ConfigRead.server.GetServerType() == SERVER_BATTLECORE)
	{
		memcpy(pMsg.Name, lpObj->m_PlayerData->m_RealNameOfUBF, MAX_ACCOUNT_LEN+1);
	}

	else
	{
		memcpy(pMsg.Name, lpObj->Name, MAX_ACCOUNT_LEN+1);
	}

	int nPartyIndex = lpObj->PartyNumber;

	if (pMsg.btBuffCount > MAX_BUFFEFFECT)
	{
		pMsg.btBuffCount = MAX_BUFFEFFECT;
	}

	for (int nBuffCnt = 0; nBuffCnt < pMsg.btBuffCount; nBuffCnt++)
	{
		pMsg.stBuffList[nBuffCnt].btBuffIndex = lpObj->m_BuffEffectList[nBuffCnt].BuffIndex;
		pMsg.stBuffList[nBuffCnt].nBuffDuration = lpObj->m_BuffEffectList[nBuffCnt].EffectDuration;
	}

	for (int i = 0; i < MAX_USER_IN_PARTY; i++)
	{
		if (gObjIsConnected(gParty.m_PartyS[nPartyIndex].Number[i]))
		{
			IOCP.DataSend(gParty.m_PartyS[nPartyIndex].Number[i], (LPBYTE)&pMsg, pMsg.h.size);
		}
	}
}

void GameProtocol::GCPartyMemberPosition(int nIndex)
{
	if (ObjectMaxRange(nIndex) == false)
	{
		return;
	}

	if (!gObjIsConnected(nIndex))
	{
		return;
	}

	if (gObj[nIndex].m_PlayerData->m_bSendMemberPos == FALSE)
	{
		return;
	}

	if (gObj[nIndex].PartyNumber < 0)
	{
		return;
	}

	PMSG_MEMBER_POS_INFO_SEND pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xE7, 0x01, sizeof(pMsg));

	int nPartyNumber = gObj[nIndex].PartyNumber;

	if (ObjectMaxRange(nPartyNumber))
	{
		pMsg.btCount = 0;

		for (int i = 0; i < 5; i++)
		{
			int nTargetIndex = gParty.m_PartyS[nPartyNumber].Number[i];

			if (ObjectMaxRange(nTargetIndex) == false)
			{
				continue;
			}

			if (g_ConfigRead.server.GetServerType() == SERVER_BATTLECORE)
			{
				memcpy(pMsg.stPosInfo[pMsg.btCount].szName, gObj[nTargetIndex].m_PlayerData->m_RealNameOfUBF, MAX_ACCOUNT_LEN + 1);
			}

			else
			{
				memcpy(pMsg.stPosInfo[pMsg.btCount].szName, gObj[nTargetIndex].Name, MAX_ACCOUNT_LEN + 1);
			}

			pMsg.stPosInfo[pMsg.btCount].btMapNumber = gObj[nTargetIndex].MapNumber;
			pMsg.stPosInfo[pMsg.btCount].btPosX = gObj[nTargetIndex].X;
			pMsg.stPosInfo[pMsg.btCount].btPosY = gObj[nTargetIndex].Y;

			pMsg.btCount++;
		}

		IOCP.DataSend(nIndex, (LPBYTE)&pMsg, pMsg.h.size);
	}
}

void GameProtocol::CGReqSendMemberPosInfoStart(int nIndex)
{
	if (ObjectMaxRange(nIndex) == false)
	{
		return;
	}

	if (!gObjIsConnected(nIndex))
	{
		return;
	}

	if (gObj[nIndex].Type != OBJ_USER)
	{
		return;
	}

	if (gObj[nIndex].PartyNumber < 0)
	{
		return;
	}

	gObj[nIndex].m_PlayerData->m_bSendMemberPos = TRUE;
}

void GameProtocol::CGReqSendMemberPosInfoStop(int nIndex)
{
	if (ObjectMaxRange(nIndex) == false)
	{
		return;
	}

	if (!gObjIsConnected(nIndex))
	{
		return;
	}

	if (gObj[nIndex].Type != OBJ_USER)
	{
		return;
	}

	if (gObj[nIndex].PartyNumber < 0)
	{
		return;
	}

	gObj[nIndex].m_PlayerData->m_bSendMemberPos = FALSE;
}

void GameProtocol::GCReqSendNpcPosInfo(PMSG_REQ_NPC_POSITION *lpMsg, int nIndex)
{
	if (ObjectMaxRange(nIndex) == false)
	{
		return;
	}

	if (!gObjIsConnected(nIndex))
	{
		return;
	}

	if (gObj[nIndex].Type != OBJ_USER)
	{
		return;
	}

	if (gObj[nIndex].PartyNumber < 0)
	{
		return;
	}

	if (lpMsg->btType)
	{
		gObj[nIndex].m_PlayerData->m_bSendNpcPos = FALSE;
	}

	else
	{
		gObj[nIndex].m_PlayerData->m_bSendNpcPos = TRUE;
	}
}

void GameProtocol::CGReqCCF_DayTime(int iIndex)
{
	if (ObjectMaxRange(iIndex) == false)
	{
		return;
	}

	if (gObjIsConnected(iIndex) == false)
	{
		return;
	}

	if (g_ConfigRead.server.GetServerType() != SERVER_BATTLECORE)
	{
		return;
	}

	gObj[iIndex].m_bCCF_UI_Using = true;
	g_ChaosCastleFinal.Send_CCF_Schedule(iIndex);
}

void GameProtocol::CGReqCCF_EnterCheck(int iIndex)
{
	if (ObjectMaxRange(iIndex) == false)
	{
		return;
	}

	if (gObj[iIndex].Type != OBJ_USER || gObj[iIndex].Connected <= PLAYER_LOGGED || g_ConfigRead.server.GetServerType() != SERVER_BATTLECORE)
	{
		return;
	}

	PMSG_RESULT_CCF_ENTERCHECK pResult;
	PHeadSubSetB((LPBYTE)&pResult, 0xAF, 0x05, sizeof(pResult));

	if (gObj[iIndex].MapNumber != MAP_INDEX_LORENMARKET)
	{
		PMSG_NOTICE pNotice;
		TNotice::MakeNoticeMsgEx(&pNotice, 1, Lang.GetText(0,593));
		IOCP.DataSend(iIndex, (LPBYTE)&pNotice, pNotice.h.size);

		pResult.byResult = 11;
		pResult.byRemainTime = 0;
		pResult.byRemainTime_LOW = 0;
		IOCP.DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if (!gObj[iIndex].m_bCCF_UI_Using)
	{
		PMSG_NOTICE pNotice;
		TNotice::MakeNoticeMsgEx(&pNotice, 1, Lang.GetText(0,593));
		IOCP.DataSend(iIndex, (LPBYTE)&pNotice, pNotice.h.size);

		pResult.byResult = 6;
		pResult.byRemainTime = 0;
		pResult.byRemainTime_LOW = 0;
		IOCP.DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if (gObj[iIndex].m_IfState.use && gObj[iIndex].m_IfState.type != 12)
	{
		pResult.byResult = 12;
		pResult.byRemainTime = 0;
		pResult.byRemainTime_LOW = 0;
		IOCP.DataSend(iIndex, &pResult.h.c, pResult.h.size);
		return;
	}

	if (gObj[iIndex].m_byCCF_CanEnter)
	{
		g_Log.Add("[Chaos Castle Survival] [ CGReqCCF_EnterCheck ] ACC:%s, NAME:%s m_byCCF_CanEnter :%d ", gObj[iIndex].AccountID, gObj[iIndex].Name, gObj[iIndex].m_byCCF_CanEnter);
		gObj[iIndex].m_byCCF_CanEnter = 0;

		pResult.byResult = 13;
		pResult.byRemainTime = 0;
		pResult.byRemainTime_LOW = 0;
		IOCP.DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if (!g_ConfigRead.pk.bPkPenaltyDisable)
	{
		BOOL bPlayerKiller = FALSE; //Season 2.5 add-on

		if (gObj[iIndex].PartyNumber >= 0) //Season 2.5 add-on
		{
			if (gParty.GetPKPartyPenalty(gObj[iIndex].PartyNumber) >= 6)
			{
				bPlayerKiller = TRUE;
			}
		}
		else if (gObj[iIndex].m_PK_Level >= 6)
		{
			bPlayerKiller = TRUE;
		}

		if (bPlayerKiller == TRUE)
		{
			PMSG_NOTICE pNotice;
			TNotice::MakeNoticeMsgEx(&pNotice, 1, Lang.GetText(0,593));
			IOCP.DataSend(iIndex, (LPBYTE)&pNotice, pNotice.h.size);

			pResult.byResult = 14;
			pResult.byRemainTime = 0;
			pResult.byRemainTime_LOW = 0;
			IOCP.DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
			return;
		}
	}

	if (gObjDuelCheck(&gObj[iIndex]) == TRUE)
	{
		PMSG_NOTICE pNotice;
		TNotice::MakeNoticeMsgEx(&pNotice, 1, Lang.GetText(0,594));
		IOCP.DataSend(iIndex, (LPBYTE)&pNotice, pNotice.h.size);

		pResult.byResult = 15;
		pResult.byRemainTime = 0;
		pResult.byRemainTime_LOW = 0;
		IOCP.DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if (gObj[iIndex].m_PlayerData->lpGuild && gObj[iIndex].m_PlayerData->lpGuild->WarState == 1)
	{
		PMSG_NOTICE pNotice;
		TNotice::MakeNoticeMsgEx(&pNotice, 1, Lang.GetText(0,595));
		IOCP.DataSend(iIndex, (LPBYTE)&pNotice, pNotice.h.size);

		pResult.byResult = 16;
		pResult.byRemainTime = 0;
		pResult.byRemainTime_LOW = 0;
		IOCP.DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if (gObj[iIndex].pInventory[gObj[iIndex].m_btInvenPetPos].m_Type == ITEMGET(13, 3) ||
		gObj[iIndex].pInventory[gObj[iIndex].m_btInvenPetPos].m_Type == ITEMGET(13, 2) ||
		gObj[iIndex].pInventory[gObj[iIndex].m_btInvenPetPos].m_Type == ITEMGET(13, 37))
	{
		PMSG_NOTICE pNotice;
		TNotice::MakeNoticeMsgEx(&pNotice, 1, Lang.GetText(0,596));
		IOCP.DataSend(iIndex, (LPBYTE)&pNotice, pNotice.h.size);

		pResult.byResult = 17;
		pResult.byRemainTime = 0;
		pResult.byRemainTime_LOW = 0;
		IOCP.DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	BOOL bPlayerKiller = FALSE; //Season 2.5 add-on

	if (gObj[iIndex].PartyNumber >= 0) //Season 2.5 add-on
	{
		if (gParty.GetPKPartyPenalty(gObj[iIndex].PartyNumber) >= 6)
		{
			bPlayerKiller = TRUE;
		}
	}
	else if (gObj[iIndex].m_PK_Level >= 6)
	{
		bPlayerKiller = TRUE;
	}

	if (bPlayerKiller == TRUE)
	{
		pResult.byResult = 8;
		pResult.byRemainTime = 0;
		pResult.byRemainTime_LOW = 0;
		IOCP.DataSend(iIndex, &pResult.h.c, pResult.h.size);
		return;
	}

	if (g_ChaosCastleFinal.m_nCCF_Level > gObj[iIndex].Level)
	{
		pResult.byResult = 1;
		pResult.byRemainTime = 0;
		pResult.byRemainTime_LOW = 0;
		IOCP.DataSend(iIndex, &pResult.h.c, pResult.h.size);
		return;
	}

	if (g_ChaosCastleFinal.CheckWearingMOPH(iIndex) == TRUE)
	{
		GCServerMsgStringSend(Lang.GetText(0,576), iIndex, 1);

		pResult.byResult = 18;
		pResult.byRemainTime = 0;
		pResult.byRemainTime_LOW = 0;
		IOCP.DataSend(iIndex, &pResult.h.c, pResult.h.size);
		return;
	}
	if ((MapC[gObj[iIndex].MapNumber].GetAttr(gObj[iIndex].X, gObj[iIndex].Y) & 1) != 1)
	{
		pResult.byResult = 19;
		pResult.byRemainTime = 0;
		pResult.byRemainTime_LOW = 0;
		IOCP.DataSend(iIndex, &pResult.h.c, pResult.h.size);
		return;
	}

	int nBattleType = g_ChaosCastleFinal.Check_CCF_DayType();

	if (!nBattleType)
	{
		g_Log.Add("[Chaos Castle Survival][CGReqCCF_EnterCheck] No OpenDay , Index:%d,Name:%s", iIndex, gObj[iIndex].Name);

		pResult.byResult = 10;
		pResult.byRemainTime = 0;
		pResult.byRemainTime_LOW = 0;
		IOCP.DataSend(iIndex, &pResult.h.c, pResult.h.size);
		return;
	}
	if (nBattleType == 3 && g_ChaosCastleFinal.CheckOpenOrNotOpenCCF_Final(3) == TRUE)
	{
		g_Log.Add("[Chaos Castle Survival][CGReqCCF_EnterCheck] Not Final Time, Index:%d,Name:%s", iIndex, gObj[iIndex].Name);

		pResult.byResult = 2;
		int nRemainTime = g_ChaosCastleFinal.GetFinalRemainTime(iIndex);
		pResult.byRemainTime = HIBYTE(nRemainTime);
		pResult.byRemainTime_LOW = LOBYTE(nRemainTime);
		IOCP.DataSend(iIndex, &pResult.h.c, pResult.h.size);
		return;
	}
	if (g_ChaosCastleFinal.CheckCanEnter() == FALSE)
	{
		pResult.byResult = 2;
		int nRemainTime = g_ChaosCastleFinal.GetRemainTime(iIndex);
		pResult.byRemainTime = HIBYTE(nRemainTime);
		pResult.byRemainTime_LOW = LOBYTE(nRemainTime);
		IOCP.DataSend(iIndex, &pResult.h.c, pResult.h.size);
		return;
	}

	int nCheckIndex = g_ChaosCastleFinal.EnterCheck_CCF_User(iIndex);

	if (nCheckIndex == -1)
	{
		pResult.byResult = 5;
		pResult.byRemainTime = 0;
		pResult.byRemainTime_LOW = 0;
		IOCP.DataSend(iIndex, &pResult.h.c, pResult.h.size);
		return;
	}

	if (nCheckIndex == -2)
	{
		pResult.byResult = 9;
		pResult.byRemainTime = 0;
		pResult.byRemainTime_LOW = 0;
		IOCP.DataSend(iIndex, &pResult.h.c, pResult.h.size);
		return;
	}

	if (!nBattleType)
	{
		pResult.byResult = 10;
		pResult.byRemainTime = 0;
		pResult.byRemainTime_LOW = 0;
		IOCP.DataSend(iIndex, &pResult.h.c, pResult.h.size);
		return;
	}

	if (nBattleType == 1)
	{
		if (!g_ChaosCastleFinal.CheckUserEnterMoney(iIndex, 1))
		{
			pResult.byResult = 7;
			pResult.byRemainTime = 0;
			pResult.byRemainTime_LOW = 0;
			IOCP.DataSend(iIndex, &pResult.h.c, pResult.h.size);
			return;
		}

		gObj[iIndex].m_byCCF_CanEnter = 1;

		int nReturn = this->CGReqEnterChaosCastleFinal(iIndex);

		if (nReturn)
		{
			g_Log.Add("[Chaos Castle Survival][gObj_EnterChaosCastleFinal] ACC :%s , NAME :%s return :%d ", gObj[iIndex].AccountID, gObj[iIndex].Name, nReturn);
		}

		return;
	}

	if (nBattleType == 2)
	{
		if (g_ChaosCastleFinal.CheckUserEnterMoney(iIndex, 2))
		{
			gObj[iIndex].m_byCCF_CanEnter = -1;
			g_ChaosCastleFinal.GD_Req_Get_Permission(&gObj[iIndex]);
			gObj[iIndex].m_nCCF_CertiTick = GetTickCount();
		}
		else
		{
			pResult.byResult = 7;
			pResult.byRemainTime = 0;
			pResult.byRemainTime_LOW = 0;
			IOCP.DataSend(iIndex, &pResult.h.c, pResult.h.size);
		}
	}
	else
	{
		if (g_ChaosCastleFinal.CheckUserEnterMoney(iIndex, 3))
		{
			gObj[iIndex].m_byCCF_CanEnter = -1;
			g_ChaosCastleFinal.GD_Req_Get_Permission(&gObj[iIndex]);
			gObj[iIndex].m_nCCF_CertiTick = GetTickCount();
		}
		else
		{
			pResult.byResult = 7;
			pResult.byRemainTime = 0;
			pResult.byRemainTime_LOW = 0;
			IOCP.DataSend(iIndex, &pResult.h.c, pResult.h.size);
		}
	}
}

int GameProtocol::CGReqEnterChaosCastleFinal(int iIndex)
{
	if (ObjectMaxRange(iIndex) == false)
	{
		return -4;
	}

	if (gObj[iIndex].Type == OBJ_USER && gObj[iIndex].Connected <= PLAYER_LOGGED)
	{
		return -5;
	}

	PMSG_RESULT_CCF_ENTERCHECK pResult;
	PHeadSubSetB((LPBYTE)&pResult, 0xAF, 0x05, sizeof(pResult));

	if (g_ChaosCastleFinal.PayUserEnterMoney(iIndex, gObj[iIndex].m_byCCF_CanEnter) == FALSE)
	{
		pResult.byResult = 7;
		pResult.byRemainTime = 0;
		pResult.byRemainTime_LOW = 0;
		IOCP.DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
		return -3;
	}

	int nCCFUserIndex = 0;
	int nEnterCheck = g_ChaosCastleFinal.EnterCCF(iIndex, nCCFUserIndex);

	if (nEnterCheck)
	{
		g_Log.Add("[Chaos Castle Survival] (ACC:%s,NAME:%s) EnterCCF return ERROR :%d ", gObj[iIndex].AccountID, gObj[iIndex].Name, nEnterCheck);

		pResult.byResult = 20;
		pResult.byRemainTime = 0;
		pResult.byRemainTime_LOW = 0;
		IOCP.DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
		return nEnterCheck;
	}

	else
	{
		pResult.byResult = 0;
		pResult.byRemainTime = 0;
		pResult.byRemainTime_LOW = 0;
		IOCP.DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);

		gObj[iIndex].m_nCCF_UserIndex = nCCFUserIndex;
		gObj[iIndex].m_byCCF_CanEnter = 0;
		gObj[iIndex].m_IfState.state = 0;
		gObj[iIndex].m_IfState.type = 0;
		gObj[iIndex].m_IfState.use = 0;
		gObj[iIndex].m_byKillMobCount = 0;
		gObj[iIndex].m_byKillUserCount = 0;
		gObj[iIndex].m_bCCF_Quit_Msg = 0;

		if (gObj[iIndex].m_bPShopOpen == true)
			g_PersonalStore.CGPShopReqClose(iIndex);

		if (gObj[iIndex].PartyNumber >= 0)
		{
			PMSG_PARTYDELUSER pMsg;

			int pindex = gParty.GetIndex(gObj[iIndex].PartyNumber, iIndex, gObj[iIndex].DBNumber);

			if (pindex >= 0)
			{
				pMsg.Number = pindex;
				CGPartyDelUser(&pMsg, iIndex, 0);
			}
		}

		if (gObj[iIndex].Class == CLASS_ELF)
		{
			if (gObj[iIndex].m_RecallMon >= 0)
			{
				GCRecallMonLife(iIndex, 60, 0);
				gObjMonsterCallKill(iIndex);
			}
		}

		GCServerMsgStringSend(Lang.GetText(0,577), iIndex, 1);
		gObj[iIndex].MapNumber = MAP_INDEX_CHAOSCASTLE_SURVIVAL;
		gObjMoveGate(iIndex, 443);

		g_Log.Add("[Chaos Castle Survival] (ACC:%s,NAME:%s) Enter CCF_Tryout", gObj[iIndex].AccountID, gObj[iIndex].Name);
		return 0;
	}
}

void GameProtocol::CGRequestRepositionUserInCCF(PMSG_REQ_REPOSUSER_IN_CCF *lpMsg, int aIndex)
{
	if (gObj[aIndex].MapNumber != MAP_INDEX_CHAOSCASTLE_SURVIVAL)
	{
		return;
	}

	if (!gObjIsConnected(aIndex))
	{
		return;
	}

	if (g_ChaosCastleFinal.ObjSetPosition(aIndex, gObj[aIndex].X, gObj[aIndex].Y) == TRUE)
	{
		gObj[aIndex].m_nCCF_BlowTime = GetTickCount();
	}
}

void GameProtocol::CG_CCF_UI_OnOff(PMSG_CCF_UI_ONOFF *lpMsg, int nUserIndex)
{
	if (ObjectMaxRange(nUserIndex) == false)
	{
		return;
	}

	if (gObj[nUserIndex].Type != OBJ_USER || gObj[nUserIndex].Connected <= PLAYER_LOGGED)
	{
		return;
	}

	if (gObj[nUserIndex].m_byCCF_CanEnter == 0xFF)
		gObj[nUserIndex].m_byCCF_CanEnter = 0;

	if (g_ConfigRead.server.GetServerType() == SERVER_BATTLECORE)
		gObj[nUserIndex].m_bCCF_UI_Using = 0;
}

void GameProtocol::CGReq_CCF_Ranking(PMSG_REQ_CCF_RANKING *lpMsg, int aIndex)
{
	if (ObjectMaxRange(aIndex) == false)
	{
		return;
	}

	if (g_ConfigRead.server.GetServerType() != SERVER_BATTLECORE)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->m_bCCF_UI_Using == false)
	{
		return;
	}

	if (lpObj->MapNumber != MAP_INDEX_LORENMARKET)
	{
		return;
	}

	if (g_ChaosCastleFinal.Check_CCF_DayType() == FALSE)
	{
		_stPMSG_CCF_RANK_INFO pMsg;

		PHeadSubSetW((LPBYTE)&pMsg, 0xAF, 0x07, sizeof(pMsg));
		pMsg.btResult = 2;
		pMsg.btCnt = 0;
		pMsg.h.sizeH = HIBYTE(sizeof(pMsg));
		pMsg.h.sizeL = LOBYTE(sizeof(pMsg));
		IOCP.DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
	}

	if (g_ChaosCastleFinal.m_FirstRankingInfoLoad)
	{
		g_ChaosCastleFinal.Send_RankingInfo(lpMsg->byRankingType, aIndex);
	}

	else
	{
		g_ChaosCastleFinal.GD_Load_CCF_RankingList(g_ChaosCastleFinal.Check_CCF_DayType());

		_stPMSG_CCF_RANK_INFO pMsg;

		PHeadSubSetW((LPBYTE)&pMsg, 0xAF, 0x07, sizeof(pMsg));
		pMsg.btResult = 3;
		pMsg.btCnt = 0;
		pMsg.h.sizeH = HIBYTE(sizeof(pMsg));
		pMsg.h.sizeL = LOBYTE(sizeof(pMsg));
		IOCP.DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));

		g_Log.Add("[Chaos Castle Survival][CGReq_CCF_Ranking] GD_Load_CCF_RankingList Send ");
	}
}
void GameProtocol::GCMuunItemDurSend(int aIndex, BYTE pos, BYTE dur)
{
	PMSG_MUUNITEM_DUR pMsg; 

	PHeadSubSetB(&pMsg.h.c, 0x4E, 0x03, sizeof(pMsg));
	pMsg.IPos = pos;
	pMsg.Dur = dur;
	IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void GameProtocol::GCMuunEquipmentChange(int aIndex, int pos)
{
	OBJECTSTRUCT *lpObj;
	PMSG_USER_MUUN_EQUIPMENT_CHANGED pChange; 

	lpObj = &gObj[aIndex];
	PHeadSubSetB((LPBYTE)&pChange, 0x4E, 0x06, sizeof(pChange));

	pChange.NumberH = SET_NUMBERH(aIndex);
	pChange.NumberL = SET_NUMBERL(aIndex);
	ItemByteConvert(pChange.ItemInfo, lpObj->pMuunInventory[pos]);
	pChange.ItemInfo[1] = 16 * pos;
	pChange.ItemInfo[1] = lpObj->pMuunInventory[pos].m_Level & 0xF | 16 * pos;
	this->MsgSendV2(lpObj, (LPBYTE)&pChange, (unsigned __int8)pChange.h.size);
}

void GameProtocol::GCMuunInventoryItemDeleteSend(int aIndex, BYTE pos, BYTE flag)
{
	PMSG_PETINVENTORY_DELETE pMsg; 

	PHeadSubSetB((LPBYTE)&pMsg, 0x4E, 0x04, sizeof(pMsg));
	pMsg.IPos = pos;
	pMsg.Flag = flag;
	IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void GameProtocol::GCMuunInventoryItemOneSend(int aIndex, int pos)
{
	PMSG_MUUN_INVEN_ITEM_MOD pMsg; 

	if (gObj[aIndex].pMuunInventory[pos].IsItem())
	{
		PHeadSubSetB((LPBYTE)&pMsg, 0x4E,0x05, sizeof(pMsg));
		pMsg.Pos = pos;
		ItemByteConvert(pMsg.ItemInfo, gObj[aIndex].pMuunInventory[pos]);
		IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
	}
}

void GameProtocol::CGMuunItemGetRequest(PMSG_MUUNITEM_GETREQUEST *lpMsg, int aIndex)
{
	int item_num, map_num;
	PMSG_MUUNITEM_GETRESULT pResult; 
	int type;
	int level;
	int special;
	int NOption;
	char szItemName[50];
	CMapItem * lpItem;

	pResult.h.c = 0xC3;
	pResult.h.headcode = 0x4E;
	pResult.h.subcode = 0x00;
	pResult.h.size = sizeof(pResult);
	pResult.result = -1;

	if (!gObjIsConnected(aIndex))
	{
		g_Log.AddC(TColor::Red, "[ERROR] (CGMuunItemGetRequest) gObjIsConnected(aIndex)  == FALSE");
		this->GCSendDisableReconnect(aIndex);
		//IOCP.CloseClient(aIndex);
		return;
	}

	if (gObj[aIndex].Type != OBJ_USER)
	{
		return;
	}

	if (gObj[aIndex].CloseType != -1)
		return;

	if (gObj[aIndex].m_PlayerData->m_bSecurityCheck == false)
	{
		pResult.result = -1;
		pResult.h.size -= sizeof(pResult.Data);
		IOCP.DataSend(aIndex, (UCHAR*)&pResult, pResult.h.size);

		return;
	}

	if (gObj[aIndex].DieRegen != 0)
	{
		pResult.result = -1;
		pResult.h.size -= sizeof(pResult.Data);
		IOCP.DataSend(aIndex, &pResult.h.c, (unsigned __int8)pResult.h.size);

		return;
	}

	if (g_NewPVP.IsObserver(gObj[aIndex]))
	{
		pResult.result = -1;
		pResult.h.size -= sizeof(pResult.Data);
		IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

		return;
	}

	if (gObj[aIndex].m_IfState.use == 1)
	{
		GCServerMsgStringSend(Lang.GetText(0,597), aIndex, 1);

		pResult.result = -1;
		pResult.h.size -= sizeof(pResult.Data);
		IOCP.DataSend(aIndex, (LPBYTE)&pResult, (pResult.h.size - 12));
			
		return;
	}

	if (!::gObjFixMuunInventoryPointer(aIndex))
		g_Log.Add("[Fix PetInv.Ptr] False Location -  %d",__LINE__);

	if (gObj[aIndex].pTransaction == TRUE)
	{
		g_Log.Add("[%s][%s] CGMuunItemGetRequest() Failed : Transaction == 1, IF_TYPE : %d",gObj[aIndex].AccountID,gObj[aIndex].Name,gObj[aIndex].m_IfState.type);
		return;
	}

	item_num = MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);

	if (MaxItemTypeRange(item_num) == FALSE)
	{
		g_Log.Add("error-L3 : %s %d", __FILE__, __LINE__);
		pResult.result = -1;
		pResult.h.size -= sizeof(pResult.Data);
		IOCP.DataSend(aIndex, (UCHAR*)&pResult, pResult.h.size);

		return;
	}

	map_num = gObj[aIndex].MapNumber;

	if (MAX_MAP_RANGE(map_num) == FALSE)
	{
		g_Log.Add("error-L3 : %s %d", __FILE__, __LINE__);
		pResult.result = -1;
		pResult.h.size -= sizeof(pResult.Data);
		IOCP.DataSend(aIndex, (UCHAR*)&pResult, pResult.h.size);

		return;
	}

	lpItem = &MapC[map_num].m_cItem[item_num];

	if (lpItem->IsItem() == TRUE && lpItem->Give == false && lpItem->live == true)
	{
		type = lpItem->m_Type;
		level = lpItem->m_Level;
		special = lpItem->m_Special[0];
		NOption = lpItem->m_NewOption;
		LPITEM_ATTRIBUTE pItemAttribute = GetItemAttr(type);

		if (!pItemAttribute)
		{
			return;
		}

		if (pItemAttribute->ItemKindA != 12)
		{
			pResult.result = -1;
			pResult.h.size -= sizeof(pResult.Data);
			IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

			return;
		}

		pResult.result = gObjChkMuunInventoryEmpty(&gObj[aIndex]);

		if (pResult.result == 255)
		{
			pResult.result = -1;
			pResult.h.size -= sizeof(pResult.Data);
			IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

			return;
		}

		if (g_CMuunSystem.IsMuunExpireDate(lpItem->m_Type) == TRUE)
		{
			lpItem->SetMuunItemPeriodExpire();
		}

		ItemByteConvert((LPBYTE)&pResult.Data[0], lpItem->m_Type, lpItem->m_Option1, lpItem->m_Option2,
			lpItem->m_Option3, lpItem->m_Level, lpItem->m_Durability, lpItem->m_NewOption,
			lpItem->m_SetOption, lpItem->m_JewelOfHarmonyOption, lpItem->m_ItemOptionEx, lpItem->m_SocketOption, lpItem->m_BonusSocketOption, 0);

		if (MapC[map_num].ItemGive(aIndex, item_num, 0) == 1)
		{
			BYTE Ret = ::gObjMuunInventoryInsertItem(aIndex, lpItem);

			if (Ret == 0xFF)
				pResult.result = -1;

			IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

			if (Ret == 0xFF)
			{
				GCMuunInventoryItemListSend(aIndex);
			}

			else
			{
				BYTE NewOption[MAX_EXOPTION_SIZE];

				::ItemIsBufExOption(NewOption, (lpItem != NULL) ? (CItem*)&lpItem->m_Number : NULL);

				if (g_CMuunSystem.IsStoneofEvolution(lpItem))
				{
					WORD wPetItemNumber = (lpItem->m_SocketOption[0] << 8) | lpItem->m_SocketOption[1];
					LPITEM_ATTRIBUTE PetItemAttr = GetItemAttr(wPetItemNumber);

					if (!PetItemAttr)
					{
						g_Log.Add("[MuunSystem][Error] pItemAttribute is NULL : %s %d", __FILE__, __LINE__);
						this->GCSendGetItemInfoForParty(aIndex, lpItem);

						return;
					}

					g_Log.Add("[%s][%s][%d]%d/%d Get serial:%I64d [%s][%d][%d][%d][%d][%d] dur:[%d]Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d] 380:[%d] HO:[%d,%d] E:[%d]", gObj[aIndex].AccountID, gObj[aIndex].Name, map_num,
						gObj[aIndex].X, gObj[aIndex].Y, lpItem->m_Number, szItemName, type,
						level, lpItem->m_Option1, lpItem->m_Option2, lpItem->m_Option3, (INT)lpItem->m_Durability,
						NewOption[0], NewOption[1], NewOption[2], NewOption[3], NewOption[4], NewOption[5],
						NewOption[6], lpItem->m_SetOption, lpItem->m_ItemOptionEx >> 7,
						g_kJewelOfHarmonySystem.GetItemStrengthenOption((lpItem) ? ((CItem *)&lpItem->m_Number) : NULL), g_kJewelOfHarmonySystem.GetItemOptionLevel((lpItem) ? ((CItem *)&lpItem->m_Number) : NULL),
						lpItem->m_BonusSocketOption);
				}

				else
				{
					g_Log.Add("[%s][%s][%d]%d/%d Get serial:%I64d [%s][%d][%d][%d][%d][%d] dur:[%d]Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d] 380:[%d] HO:[%d,%d] E:[%d]", gObj[aIndex].AccountID, gObj[aIndex].Name, map_num,
						gObj[aIndex].X, gObj[aIndex].Y, lpItem->m_Number, szItemName, type,
						level, lpItem->m_Option1, lpItem->m_Option2, lpItem->m_Option3, (INT)lpItem->m_Durability,
						NewOption[0], NewOption[1], NewOption[2], NewOption[3], NewOption[4], NewOption[5],
						NewOption[6], lpItem->m_SetOption, lpItem->m_ItemOptionEx >> 7,
						g_kJewelOfHarmonySystem.GetItemStrengthenOption((lpItem) ? ((CItem *)&lpItem->m_Number) : NULL), g_kJewelOfHarmonySystem.GetItemOptionLevel((lpItem) ? ((CItem *)&lpItem->m_Number) : NULL),
						lpItem->m_BonusSocketOption);
				}

				this->GCSendGetItemInfoForParty(aIndex, lpItem);
			}
		}

		else
		{
			pResult.result = -1;
			pResult.h.size -= sizeof(pResult.Data);

			IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
			return;
		}
	}

	else
	{

		pResult.result = -1;
		pResult.h.size -= sizeof(pResult.Data);

		IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}
}

void GameProtocol::CGMuunItemSellRequestRecv(PMSG_MUUN_ITEM_SELLREQUEST *lpMsg, int aIndex)
{
	PMSG_MUUN_ITEM_SELLRESULT pResult;
	LPOBJ lpObj = &gObj[aIndex];

	PHeadSubSetB((LPBYTE)&pResult, 0x4E, 0x09, sizeof(pResult));

	pResult.Result = 0;
	pResult.Money = 0;

	if ( gObj[aIndex].Type != OBJ_USER )
	{
		return;
	}

	if ( gObj[aIndex].CloseType != -1 )
	{
		IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( lpObj->m_IfState.use > 0 && lpObj->m_IfState.type != 3 )
	{
		IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( !PacketCheckTime(aIndex))
	{
		IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( !::gObjFixMuunInventoryPointer(aIndex))
	{
		g_Log.Add("[Fix Inv.Ptr] False Location - %s, %d", __FILE__, __LINE__);
	}

	if ( gObj[aIndex].pTransaction == 1 )
	{
		g_Log.Add("[%s][%s] CGMuunSellRequestRecv() Failed : Transaction == 1, IF_TYPE : %d",
			gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_IfState.type);

		return;
	}

	if (gObj[aIndex].TargetNpcNumber == -1)
	{
		IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	LPOBJ lpShopObj = &gObj[gObj[aIndex].TargetNpcNumber];

	SHOP_DATA * lpShopData = g_ShopMng.GetShop(lpObj, lpShopObj);

	if (!lpShopData)
	{
		IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( !gObjCanItemTouch(lpObj, 3))
	{
		IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( lpMsg->Pos > MUUN_INVENTORY_SIZE-1 )
	{
		IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		g_Log.Add("Item location value exceeds: %d", lpMsg->Pos);
		return;
	}

	if ( gObjCheckSerial0ItemList(&lpObj->pMuunInventory[lpMsg->Pos]) != FALSE )
	{
		MsgOutput(aIndex, Lang.GetText(0,259));
		IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

		g_Log.Add("[ANTI-HACK][Serial 0 Item] [Sell Item] (%s)(%s) Item(%s) Pos(%d)",
			lpObj->AccountID, lpObj->Name, lpObj->pMuunInventory[lpMsg->Pos].GetName(), lpMsg->Pos);

		return;
	}

	if (!IsSellToNPCItem(lpObj->pMuunInventory[lpMsg->Pos].m_Type))
	{
		IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if ( lpObj->pMuunInventory[lpMsg->Pos].IsItem() == FALSE )
	{
		pResult.Result = false;
		pResult.Money = lpObj->m_PlayerData->Money;
		g_Log.Add("Invalid item index %s %d (%s) Pos: %d", __FILE__, __LINE__, lpObj->Name, lpMsg->Pos);
		g_Log.Add("ID: %s CharID: %s ", lpObj->AccountID, lpObj->Name);
		IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

	}
	else
	{
		int sellmoney = 0;

		if (g_CMuunSystem.IsMuunItem(lpObj->pMuunInventory[lpMsg->Pos].m_Type) == TRUE)
		{
			sellmoney = 500000;
		}

		else if (g_CMuunSystem.IsStoneofEvolution(lpObj->pMuunInventory[lpMsg->Pos].m_Type) == TRUE)
		{
			sellmoney = 200000;
		}

		else if (lpObj->pMuunInventory[lpMsg->Pos].m_Type == ITEMGET(13,239))
		{
			sellmoney = 300000;
		}

		else if (lpObj->pMuunInventory[lpMsg->Pos].m_Type == ITEMGET(13,240))
		{
			sellmoney = 1000000;
		}

		else
		{
			IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
			return;
		}

		if ( gObjCheckMaxZen(aIndex, sellmoney) == FALSE )
		{
			IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
			return;
		}

		lpObj->m_PlayerData->Money += sellmoney;
		pResult.Result = 1;
		pResult.Money = lpObj->m_PlayerData->Money;

		BYTE NewOption[MAX_EXOPTION_SIZE];
		ItemIsBufExOption(NewOption, &lpObj->pMuunInventory[lpMsg->Pos]);

		g_Log.Add("[%s][%s] Shop [MAP:%d][X:%d][Y:%d] Item Sell Money:%d %s:%d/%d/%d/%d/%I64d Dur:%d Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d] 380:[%d] HO:[%d,%d] E:[%d]", lpObj->AccountID, lpObj->Name,
			lpShopData->btMapNumber, lpShopData->btPosX, lpShopData->btPosY,
			sellmoney, lpObj->pMuunInventory[lpMsg->Pos].GetName(), 
			lpObj->pMuunInventory[lpMsg->Pos].m_Level, lpObj->pMuunInventory[lpMsg->Pos].m_Option1,
			lpObj->pMuunInventory[lpMsg->Pos].m_Option2, lpObj->pMuunInventory[lpMsg->Pos].m_Option3,
			lpObj->pMuunInventory[lpMsg->Pos].m_Number, (int)lpObj->pMuunInventory[lpMsg->Pos].m_Durability,
			NewOption[0], NewOption[1], NewOption[2], NewOption[3], NewOption[4], NewOption[5], NewOption[6], 
			lpObj->pMuunInventory[lpMsg->Pos].m_SetOption, lpObj->pMuunInventory[lpMsg->Pos].m_ItemOptionEx >> 7,
			g_kJewelOfHarmonySystem.GetItemStrengthenOption(&lpObj->pMuunInventory[lpMsg->Pos]),
			g_kJewelOfHarmonySystem.GetItemOptionLevel(&lpObj->pMuunInventory[lpMsg->Pos]),
			lpObj->pMuunInventory[lpMsg->Pos].m_BonusSocketOption);

		if (lpMsg->Pos == 0 || lpMsg->Pos == 1)
		{
			WORD nItemCode = lpObj->pMuunInventory[lpMsg->Pos].m_Type;
			UINT64 nSerial = lpObj->pMuunInventory[lpMsg->Pos].m_Number;

			if (lpMsg->Pos == 0)
			{
				lpObj->m_wMuunItem = -1;
			}

			else if (lpMsg->Pos == 1)
			{
				lpObj->m_wMuunSubItem = -1;
			}

			g_CMuunSystem.RemoveUserMuunEffect(lpObj, lpMsg->Pos);
			this->GCMuunEquipmentChange(aIndex, lpMsg->Pos);
		}

		gObjMuunInventoryDeleteItem(aIndex, lpMsg->Pos);
		gObjMakePreviewCharSet(aIndex);

		IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
	}
}

void GameProtocol::CGReqEventInvenOpen(LPBYTE lpRecv, int aIndex)
{
	if (g_CMuRummyMng.IsMuRummyEventOn() == true)
	{
		g_CMuRummyMng.CGReqMuRummyEventOpen((_tagPMSG_REQ_MURUMMY_EVENT_OPEN *)lpRecv, aIndex);
		return;
	}

	if (g_ConfigRead.data.common.EventInventoryOpen)
	{
		_tagPMSG_ANS_MURUMMY_EVENT_OPEN pMsg;
		PHeadSubSetB((LPBYTE)&pMsg, 0x4D, 0x0F, sizeof(pMsg));

		pMsg.btResult = 3;

		time_t tmEndTime;
		DWORD dw32EndTime;
		SystemTimeToUnixTime(&g_ConfigRead.data.common.EventEndDate, &tmEndTime);
		dw32EndTime = (DWORD)tmEndTime;

		pMsg.btEventTime1 = SET_NUMBERH(SET_NUMBERHW(dw32EndTime));
		pMsg.btEventTime2 = SET_NUMBERL(SET_NUMBERHW(dw32EndTime));
		pMsg.btEventTime3 = SET_NUMBERH(SET_NUMBERLW(dw32EndTime));
		pMsg.btEventTime4 = SET_NUMBERL(SET_NUMBERLW(dw32EndTime));

		IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
	}
}

void GameProtocol::CGEventItemGetRequest(PMSG_EVENTITEM_GETREQUEST *lpMsg, int aIndex)
{
	int item_num, map_num;
	PMSG_EVENTITEM_GETRESULT pResult;
	int type;
	int level;
	int special;
	int NOption;
	char szItemName[50];
	CMapItem * lpItem;

	pResult.h.c = 0xC3;
	pResult.h.headcode = 0x4D;
	pResult.h.subcode = 0x00;
	pResult.h.size = sizeof(pResult);
	pResult.result = -1;

	if (!gObjIsConnected(aIndex))
	{
		g_Log.AddC(TColor::Red, "[ERROR] (CGEventItemGetRequest) gObjIsConnected(aIndex)  == FALSE");
		this->GCSendDisableReconnect(aIndex);
		//IOCP.CloseClient(aIndex);
		return;
	}

	if (gObj[aIndex].Type != OBJ_USER)
	{
		return;
	}

	if (gObj[aIndex].CloseType != -1)
		return;

	if (g_CMuRummyMng.IsMuRummyEventOn() == false && g_ConfigRead.data.common.EventInventoryOpen == false)
	{
		pResult.result = -1;
		pResult.h.size -= sizeof(pResult.Data);
		IOCP.DataSend(aIndex, (UCHAR*)&pResult, pResult.h.size);

		return;
	}

	if (gObj[aIndex].m_PlayerData->m_bSecurityCheck == false)
	{
		pResult.result = -1;
		pResult.h.size -= sizeof(pResult.Data);
		IOCP.DataSend(aIndex, (UCHAR*)&pResult, pResult.h.size);

		return;
	}

	if (gObj[aIndex].DieRegen != 0)
	{
		pResult.result = -1;
		pResult.h.size -= sizeof(pResult.Data);
		IOCP.DataSend(aIndex, (UCHAR*)&pResult, pResult.h.size);

		return;
	}

	if (g_NewPVP.IsObserver(gObj[aIndex]) == TRUE)
	{
		pResult.result = -1;
		pResult.h.size -= sizeof(pResult.Data);
		IOCP.DataSend(aIndex, (UCHAR*)&pResult, pResult.h.size);

		return;
	}

	if (!::gObjFixEventInventoryPointer(aIndex))
		g_Log.Add("[Fix EventInv.Ptr] False Location - %s, %d", __FILE__, __LINE__);

	if (gObj[aIndex].pTransaction == TRUE)
	{
		g_Log.Add("[%s][%s] CGEventItemGetRequest() Failed : Transaction == 1, IF_TYPE : %d",
			gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_IfState.type);
		return;

	}

	item_num = MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);

	if (MaxItemTypeRange(item_num) == FALSE)
	{
		g_Log.Add("error-L3 : %s %d", __FILE__, __LINE__);
		pResult.result = -1;
		pResult.h.size -= sizeof(pResult.Data);
		IOCP.DataSend(aIndex, (UCHAR*)&pResult, pResult.h.size);

		return;
	}

	map_num = gObj[aIndex].MapNumber;

	if (MAX_MAP_RANGE(map_num) == FALSE)
	{
		g_Log.Add("error-L3 : %s %d", __FILE__, __LINE__);
		pResult.result = -1;
		pResult.h.size -= sizeof(pResult.Data);
		IOCP.DataSend(aIndex, (UCHAR*)&pResult, pResult.h.size);

		return;
	}

	lpItem = &MapC[map_num].m_cItem[item_num];

	if (lpItem->IsItem() == TRUE && lpItem->Give == false && lpItem->live == true)
	{
		type = lpItem->m_Type;
		level = lpItem->m_Level;
		special = lpItem->m_Special[0];
		NOption = lpItem->m_NewOption;
		LPITEM_ATTRIBUTE pItemAttribute = GetItemAttr(type);

		if (!pItemAttribute)
		{
			return;
		}

		if (pItemAttribute->ItemKindA != 11)
		{
			pResult.result = -1;
			pResult.h.size -= sizeof(pResult.Data);
			IOCP.DataSend(aIndex, (UCHAR*)&pResult, pResult.h.size);

			return;
		}

		if (lpItem->m_Type == ITEMGET(14, 215))
		{
			int iInventoryIndex;

			for (int iLoop = 0; iLoop < 32; iLoop++)
			{
				iInventoryIndex = -1;

				if (gObj[aIndex].pEventInventory[iLoop].m_Type == ITEMGET(14, 215) &&
					gObj[aIndex].pEventInventory[iLoop].m_Durability < 24.0)
				{
					iInventoryIndex = iLoop;
				}

				if (iInventoryIndex < 0 || iInventoryIndex > EVENT_INVENTORY_SIZE - 1)
				{
					continue;
				}

				if (MapC[map_num].ItemGive(aIndex, item_num, 1) == TRUE)
				{
					BYTE NewOption[MAX_EXOPTION_SIZE];

					::ItemIsBufExOption(NewOption, (lpItem != NULL) ? (CItem*)&lpItem->m_Number : NULL);

					g_Log.Add("[%s][%s][%d]%d/%d Get serial:%I64d [%s][%d][%d][%d][%d][%d] dur:[%d]Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d] 380:[%d] HO:[%d,%d] E:[%d]", gObj[aIndex].AccountID, gObj[aIndex].Name, map_num,
						gObj[aIndex].X, gObj[aIndex].Y, lpItem->m_Number, lpItem->GetName(), type,
						level, lpItem->m_Option1, lpItem->m_Option2, lpItem->m_Option3, (int)lpItem->m_Durability,
						NewOption[0], NewOption[1], NewOption[2], NewOption[3], NewOption[4], NewOption[5],
						NewOption[6], lpItem->m_SetOption, lpItem->m_ItemOptionEx >> 7,
						g_kJewelOfHarmonySystem.GetItemStrengthenOption((lpItem) ? ((CItem *)&lpItem->m_Number) : NULL), g_kJewelOfHarmonySystem.GetItemOptionLevel((lpItem) ? ((CItem *)&lpItem->m_Number) : NULL),
						lpItem->m_BonusSocketOption);

					pResult.result = -3;
					IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

					gObj[aIndex].pEventInventory[iInventoryIndex].m_Durability += lpItem->m_Durability;

					if (gObj[aIndex].pEventInventory[iInventoryIndex].m_Durability < 24.0)
					{
						this->GCEventItemDurSend(aIndex, iInventoryIndex, gObj[aIndex].pEventInventory[iInventoryIndex].m_Durability);
						return;
					}

					int iDurGap = gObj[aIndex].pEventInventory[iInventoryIndex].m_Durability - 24.0;
					BYTE btRet = gObjEventInventoryInsertItemTemp(&gObj[aIndex], lpItem);
					gObjEventInventoryItemSet(aIndex, iInventoryIndex, -1);
					gObj[aIndex].pEventInventory[iInventoryIndex].Clear();
					this->GCEventInventoryItemDeleteSend(aIndex, iInventoryIndex, 1);

					ItemSerialCreateSend(aIndex, 226, gObj[aIndex].X, gObj[aIndex].Y, ITEMGET(14, 216), lpItem->m_Level, 0, 0, 0, 0, aIndex, 0, 0, 0, 0, 0);

					if (iDurGap <= 0)
					{
						return;
					}

					if (btRet == 255)
					{
						ItemSerialCreateSend(aIndex, gObj[aIndex].MapNumber, gObj[aIndex].X, gObj[aIndex].Y, ItemGetNumberMake(14, 215), lpItem->m_Level, 0, 0, 0, 0, aIndex, 0, 0, 0, 0, 0);
						return;
					}

					lpItem->m_State = 2;
					lpItem->Give = 0;
					lpItem->live = 1;
					lpItem->m_Durability = iDurGap;
				}
			}
		}

		pResult.result = gObjEventInventoryInsertItemTemp(&gObj[aIndex], lpItem);

		if (pResult.result == 255)
		{
			pResult.result = -1;
			pResult.h.size -= sizeof(pResult.Data);

			IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
			return;
		}

		ItemByteConvert((LPBYTE)&pResult.Data[0], lpItem->m_Type, lpItem->m_Option1, lpItem->m_Option2,
			lpItem->m_Option3, lpItem->m_Level, lpItem->m_Durability, lpItem->m_NewOption,
			lpItem->m_SetOption, lpItem->m_JewelOfHarmonyOption, lpItem->m_ItemOptionEx, lpItem->m_SocketOption, lpItem->m_BonusSocketOption, 0);

		if (MapC[map_num].ItemGive(aIndex, item_num, false) == TRUE)
		{
			BYTE Ret = ::gObjEventInventoryInsertItem(aIndex, lpItem);

			if (Ret == 0xFF)
				pResult.result = -1;

			IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

			if (Ret == 0xFF)
			{
				this->GCEventInventoryItemListSend(aIndex);
			}
			else
			{
				BYTE NewOption[MAX_EXOPTION_SIZE];

				::ItemIsBufExOption(NewOption, (lpItem != NULL) ? (CItem*)&lpItem->m_Number : NULL);

				g_Log.Add("[%s][%s][%d]%d/%d Get serial:%I64d [%s][%d][%d][%d][%d][%d] dur:[%d]Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d] 380:[%d] HO:[%d,%d] E:[%d]", gObj[aIndex].AccountID, gObj[aIndex].Name, map_num,
					gObj[aIndex].X, gObj[aIndex].Y, lpItem->m_Number, szItemName, type,
					level, lpItem->m_Option1, lpItem->m_Option2, lpItem->m_Option3, (INT)lpItem->m_Durability,
					NewOption[0], NewOption[1], NewOption[2], NewOption[3], NewOption[4], NewOption[5],
					NewOption[6], lpItem->m_SetOption, lpItem->m_ItemOptionEx >> 7,
					g_kJewelOfHarmonySystem.GetItemStrengthenOption((lpItem) ? ((CItem *)&lpItem->m_Number) : NULL), g_kJewelOfHarmonySystem.GetItemOptionLevel((lpItem) ? ((CItem *)&lpItem->m_Number) : NULL),
					lpItem->m_BonusSocketOption);

				this->GCSendGetItemInfoForParty(aIndex, lpItem);
			}
		}

		else
		{
			pResult.result = -1;
			pResult.h.size -= sizeof(pResult.Data);

			IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
			return;
		}
	}

	else
	{
		pResult.result = -1;
		pResult.h.size -= sizeof(pResult.Data);

		IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}
}

bool GameProtocol::CGEventItemDropRequest(PMSG_EVENTITEM_THROW *lpMsg, int aIndex)
{
	BOOL CopyItem = FALSE;
	BYTE NewOption[MAX_EXOPTION_SIZE];

	if (!PacketCheckTime(aIndex))
		return FALSE;

	if (gObj[aIndex].Type != OBJ_USER)
	{
		return FALSE;
	}

	PMSG_EVENTITEM_THROW_RESULT pResult;

	PHeadSubSetB((LPBYTE)&pResult, 0x4D, 0x01, sizeof(pResult));
	pResult.Result = true;
	pResult.Ipos = lpMsg->Ipos;

	if (gObj[aIndex].m_IfState.use != 0)
	{
		if (gObj[aIndex].m_IfState.type != 3)
		{
			pResult.Result = false;
			return FALSE;
		}
	}

	if (gObj[aIndex].m_PlayerData->m_bSecurityCheck == false)
	{
		MsgOutput(aIndex, Lang.GetText(0,394));
		pResult.Result = false;
	}

	if (gObj[aIndex].CloseType != -1)
		pResult.Result = false;

	if (gObj[aIndex].m_bMapSvrMoveQuit == true)
	{
		g_Log.Add("[ANTI-HACK][Item Duplication] - Item Drop during MapServer Move [%s][%s]",
			gObj[aIndex].AccountID, gObj[aIndex].Name);
		pResult.Result = false;
	}

	if (!::gObjFixEventInventoryPointer(aIndex))
		g_Log.Add("[Fix EventInv.Ptr] False Location - %s, %d", __FILE__, __LINE__);

	if (gObj[aIndex].pTransaction == 1)
	{
		g_Log.Add("[%s][%s] CGEventItemDropRequest() Failed : Transaction == 1, IF_TYPE : %d",
			gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].m_IfState.type);
		pResult.Result = false;

		return false;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (lpMsg->Ipos == 0xFF)
	{
		g_Log.Add("error:%s %d", __FILE__, __LINE__);
		pResult.Result = false;
	}

	if (lpMsg->Ipos < 0 || lpMsg->Ipos > MAIN_INVENTORY_SIZE - 1)
		pResult.Result = false;

	if (!gObjCanItemTouch(lpObj, 0))
		pResult.Result = false;

	if (!IsDumpItem(lpObj->pEventInventory[lpMsg->Ipos].m_Type))
		pResult.Result = false;

	LPITEM_ATTRIBUTE pItemAttribute = GetItemAttr(lpObj->pEventInventory[lpMsg->Ipos].m_Type);

	if (!pItemAttribute)
	{
		return false;
	}

	if (pItemAttribute->ItemKindA != 11)
		pResult.Result = false;

	if (pResult.Result == 1)
	{
		::ItemIsBufExOption(NewOption, &lpObj->pEventInventory[lpMsg->Ipos]);

		int map = lpObj->MapNumber;
		int type = lpObj->pEventInventory[lpMsg->Ipos].m_Type;
		int level = lpObj->pEventInventory[lpMsg->Ipos].m_Level;
		float dur = lpObj->pEventInventory[lpMsg->Ipos].m_Durability;
		BOOL bIsItem = lpObj->pEventInventory[lpMsg->Ipos].IsItem();
		BYTE Option1 = lpObj->pEventInventory[lpMsg->Ipos].m_Option1;
		BYTE Option2 = lpObj->pEventInventory[lpMsg->Ipos].m_Option2;
		BYTE Option3 = lpObj->pEventInventory[lpMsg->Ipos].m_Option3;
		BYTE NOption = lpObj->pEventInventory[lpMsg->Ipos].m_NewOption;
		UINT64 serial = lpObj->pEventInventory[lpMsg->Ipos].m_Number;
		int PetLevel = lpObj->pEventInventory[lpMsg->Ipos].m_PetItem_Level;
		UINT64 PetExp = lpObj->pEventInventory[lpMsg->Ipos].m_PetItem_Exp;
		BYTE SOption = lpObj->pEventInventory[lpMsg->Ipos].m_SetOption;
		BYTE btItemEffectEx = lpObj->pEventInventory[lpMsg->Ipos].m_ItemOptionEx;
		UINT64 serial2 = lpObj->pEventInventory[lpMsg->Ipos].m_Number;
		char szItemName[50] = "Item";
		BYTE JOHOption = g_kJewelOfHarmonySystem.GetItemStrengthenOption(&lpObj->pEventInventory[lpMsg->Ipos]);
		BYTE JOHOptionLevel = g_kJewelOfHarmonySystem.GetItemOptionLevel(&lpObj->pEventInventory[lpMsg->Ipos]);

		//Season 4 add-on
		BYTE SocketOption[5];
		BYTE SocketIndex;
		g_SocketOptionSystem.GetSocketOption(&lpObj->pEventInventory[lpMsg->Ipos], SocketOption, SocketIndex);

		if (gObjCheckSerial0ItemList(&lpObj->pEventInventory[lpMsg->Ipos]) != FALSE)
		{
			MsgOutput(lpObj->m_Index, Lang.GetText(0,259));
			g_Log.Add("[ANTI-HACK][Serial 0 Item] [ItemDrop] (%s)(%s) Item(%s) Pos(%d)",
				lpObj->AccountID, lpObj->Name, lpObj->pEventInventory[lpMsg->Ipos].GetName(),
				lpMsg->Ipos);

			CopyItem = TRUE;
			pResult.Result = false;
			bIsItem = FALSE;
		}

		if (!::gObjEventInventorySearchSerialNum(lpObj, serial2))
		{
			CopyItem = TRUE;
			pResult.Result = false;
			bIsItem = FALSE;
		}

		if (bIsItem == TRUE)
		{
			strcpy(szItemName, lpObj->pEventInventory[lpMsg->Ipos].GetName());

			if (g_BagManager.SearchAndUseBag(lpObj->m_Index, BAG_COMMON, type, level) == true)
			{
				gObjEventInventoryDeleteItem(aIndex, lpMsg->Ipos);
				g_Log.Add("[%s][%s][Used Item Bag (Event Inventory)] : (%d)(X:%d/Y:%d)",
					lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y);
			}
	
			else if (type == ITEMGET(13, 210) && level == 0)
			{
				gObjEventInventoryDeleteItem(aIndex, lpMsg->Ipos);
				g_BagManager.SearchAndUseBag(aIndex, BAG_EVENT, EVENTBAG_EGG_PETITEM, aIndex);

				g_Log.Add("[%s][%s][%d]%d/%d Egg of PetItem Item Serial:%I64d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
					lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y, serial, szItemName, type, level, Option1, Option2, Option3);
			}

			else if (type == ITEMGET(13, 220) && level == 0)
			{
				gObjEventInventoryDeleteItem(aIndex, lpMsg->Ipos);
				g_BagManager.SearchAndUseBag(aIndex, BAG_EVENT, EVENTBAG_EGG_LUCKYBAG, aIndex);

				g_Log.Add("[%s][%s][%d]%d/%d Egg of LuckyBag Item Serial:%I64d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
					lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y, serial, szItemName, type, level, Option1, Option2, Option3);
			}

			else if (type == ITEMGET(13, 221) && level == 0)
			{
				gObjEventInventoryDeleteItem(aIndex, lpMsg->Ipos);
				g_BagManager.SearchAndUseBag(aIndex, BAG_EVENT, EVENTBAG_EGG_FIREGHOST, aIndex);

				g_Log.Add("[%s][%s][%d]%d/%d Egg of FireGhost Item Serial:%I64d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
					lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y, serial, szItemName, type, level, Option1, Option2, Option3);
			}

			else if (type == ITEMGET(13, 222) && level == 0)
			{
				gObjEventInventoryDeleteItem(aIndex, lpMsg->Ipos);
				g_BagManager.SearchAndUseBag(aIndex, BAG_EVENT, EVENTBAG_EGG_GOLDGOBLIN, aIndex);

				g_Log.Add("[%s][%s][%d]%d/%d Egg of GoldGoblin Item Serial:%I64d (%s:%d/level:%d/skill:%d/op2:%d/op3:%d)",
					lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y, serial, szItemName, type, level, Option1, Option2, Option3);
			}

			else
			{
				if (MapC[map].ItemDrop(type, level, dur, lpMsg->px, lpMsg->py, Option1, Option2, Option3,
					NOption, SOption, serial2, aIndex, PetLevel, PetExp, btItemEffectEx, SocketOption, SocketIndex, 0) == TRUE)
				{
					gObjEventInventoryDeleteItem(aIndex, lpMsg->Ipos);
					pResult.Result = true;

					g_Log.Add("[%s][%s][%d]%d/%d Throw away Serial:%I64d [%s][%d][%d][%d][%d][%d] dur:[%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d] 380:[%d] HO:[%d,%d] E:[%d]", lpObj->AccountID, lpObj->Name, lpObj->MapNumber,
						lpObj->X, lpObj->Y, serial, szItemName, type, level, Option1, Option2,
						Option3, (int)dur, NewOption[0], NewOption[1], NewOption[2], NewOption[3], NewOption[4],
						NewOption[5], NewOption[6], SOption, btItemEffectEx >> 7, JOHOption, JOHOptionLevel,
						gObj[aIndex].pEventInventory[lpMsg->Ipos].m_BonusSocketOption);
				}

				else
				{
					pResult.Result = false;
				}
			}
		}

		else
		{
			pResult.Result = false;
		}
	}

	IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

	if (CopyItem != FALSE)
	{
		this->GCEventInventoryItemListSend(aIndex);
	}
}

void GameProtocol::GCEventInventoryItemListSend(int aIndex)
{
	PMSG_INVENTORYLISTCOUNT pMsgILC;
	PMSG_INVENTORYLIST pMsgIL;
	int sOfs = sizeof(PMSG_INVENTORYLISTCOUNT);
	int pMsgILSize = sizeof(PMSG_INVENTORYLIST);
	BYTE sendBuf[4096];
	LPOBJ lpObj = &gObj[aIndex];
	int itemcount = 0;

	for (int n = 0; n<EVENT_INVENTORY_SIZE; n++)
	{
		if (lpObj->pEventInventory[n].IsItem() == TRUE)
		{
			if (lpObj->pEventInventory[n].m_bItemExist == false)
			{
				continue;
			}

			pMsgIL.Pos = n;
			ItemByteConvert(pMsgIL.ItemInfo, lpObj->pEventInventory[n]);
			memcpy(&sendBuf[sOfs], &pMsgIL, pMsgILSize);
			itemcount++;
			sOfs += pMsgILSize;
		}
	}

	pMsgILC.h.c = 0xC4;
	pMsgILC.h.headcode = 0x4D;
	pMsgILC.subcode = 0x02;
	pMsgILC.h.sizeH = SET_NUMBERH(sOfs);
	pMsgILC.h.sizeL = SET_NUMBERL(sOfs);
	pMsgILC.Count = itemcount;
	memcpy(sendBuf, &pMsgILC, sizeof(PMSG_INVENTORYLISTCOUNT));

	IOCP.DataSend(aIndex, sendBuf, sOfs);
}

void GameProtocol::GCEventItemDurSend(int aIndex, BYTE pos, BYTE dur)
{
	PMSG_EVENTITEM_DUR pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0x4D, 0x03, sizeof(pMsg));
	pMsg.IPos = pos;
	pMsg.Dur = dur;

	IOCP.DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
}

void GameProtocol::GCEventInvenItemOneSend(int aIndex, int pos)
{
	if (!gObj[aIndex].pEventInventory[pos].IsItem())
		return;

	PMSG_EVENT_INVEN_ITEM_MOD pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0x4D, 0x05, sizeof(pMsg));
	pMsg.Pos = pos;
	ItemByteConvert(pMsg.ItemInfo, gObj[aIndex].pEventInventory[pos]);

	IOCP.DataSend(aIndex, (UCHAR *)&pMsg, pMsg.h.size);
}

void GameProtocol::GCEventInventoryItemDeleteSend(int aIndex, BYTE pos, unsigned char flag)
{
	PMSG_EVENTINVENTORY_DELETE pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0x4D, 0x04, sizeof(pMsg));
	pMsg.IPos = pos;
	pMsg.Flag = flag;

	IOCP.DataSend(aIndex, (UCHAR*)&pMsg, pMsg.h.size);
}

void GameProtocol::GCResSendExpEventInfo(int aIndex)
{
	PMSG_RES_EXPEVENTINFO pMsg;

	PHeadSubSetB((LPBYTE)&pMsg, 0xBF, 0x52, sizeof(pMsg));

	pMsg.wPCBangRate = 0;
	pMsg.wExpRate = g_ExpManager.GetEventExp() / g_ExpManager.GetStaticExp() * 100.0;

	IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void GameProtocol::CGReq_ITL_Relics(PMSG_REQ_ITL_RELATE_RELICS *lpMsg, int iIndex)
{
	if (!ObjectMaxRange(iIndex))
	{
		return;
	}

	if (!gObjIsConnected(iIndex))
	{
		return;
	}

	if (!lpMsg)
	{
		return;
	}

	WORD NpcNumber = MAKE_NUMBERW(lpMsg->NumberH, lpMsg->NumberL);

	if (NpcNumber >= g_ConfigRead.server.GetObjectMax())
	{
		return;
	}

	LPOBJ lpObj = &gObj[iIndex];

	if (!lpObj)
	{
		return;
	}

	if (lpObj->MapNumber != gObj[NpcNumber].MapNumber)
	{
		return;
	}

	g_Log.Add("[ ITL ][ ITR ][ActRelicsGetOrRegister] NpcIndex:%d,byPrePacket:%d,byAct:%d,wNpcType:%d",
		NpcNumber, lpMsg->byPrePacket, lpMsg->byAct, lpMsg->wNpcType);

	if (IT_MAP_RANGE(lpObj->MapNumber))
	{
		g_IT_Event.ActRelicsGetOrRegister(&gObj[NpcNumber], lpObj, lpObj->MapNumber, lpMsg->byPrePacket, lpMsg->byAct, lpMsg->wNpcType);
	}
}

void GameProtocol::CG_Req_Enter_ITR(PMSG_REQ_ENTER_ITR *lpMsg, int iIndex)
{
	if (!ObjectMaxRange(iIndex))
	{
		return;
	}

	_ITL_AnswerEnter pResult;
	PHeadSubSetB((LPBYTE)&pResult, 0xBF, 0x70, sizeof(pResult));
	memset(pResult.Name, 0x00, sizeof(pResult.Name));

	if (g_IT_Event.IsCanEnterTiming() == false)
	{
		pResult.bReason = 8;
		IOCP.DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if (gObj[iIndex].PartyNumber < 0)
	{
		pResult.bReason = 1;
		IOCP.DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if (gParty.Isleader(gObj[iIndex].PartyNumber, iIndex, gObj[iIndex].DBNumber) == false)
	{
		pResult.bReason = 3;
		IOCP.DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if (gParty.GetCount(gObj[iIndex].PartyNumber) < g_IT_Event.m_iITR_MinUserInParty)
	{
		pResult.bReason = 4;
		IOCP.DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if (gParty.GetPKPartyPenalty(gObj[iIndex].PartyNumber) >= 5)
	{
		pResult.bReason = 12;
		IOCP.DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	int PartyUsersIndex = 0;
	BOOL bResult = this->EnterITRCheckUser(iIndex, PartyUsersIndex);

	if (bResult != 0)
	{
		pResult.bReason = bResult;
		memcpy(pResult.Name, gObj[PartyUsersIndex].Name, MAX_ACCOUNT_LEN + 1);
		IOCP.DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);

		if (iIndex != PartyUsersIndex)
		{
			IOCP.DataSend(PartyUsersIndex, (LPBYTE)&pResult, pResult.h.size);
		}

		return;
	}

	BYTE bySlotNum = -1;
	int nEnterIllusionIndex = g_IT_Event.CheckEnterLevel(iIndex);
	BYTE byMapNumber = g_IT_Event.Find_EmptySlot(gObj[iIndex].PartyNumber, bySlotNum, nEnterIllusionIndex);

	if (byMapNumber > 6)
	{
		pResult.bReason = 14;
		IOCP.DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	BOOL bUpdateOk = g_IT_Event.Update_PartyInfo(byMapNumber, bySlotNum, gObj[iIndex].PartyNumber, iIndex);

	if (bUpdateOk != 0)
	{
		g_Log.Add("[ITR][Update_PartyInfo] ERRORcode:%d,NAME:%s,partyindex:%d",
			bUpdateOk, gObj[iIndex].Name, gObj[iIndex].PartyNumber);

		pResult.bReason = -1;
		memcpy(pResult.Name, gObj[iIndex].Name, MAX_ACCOUNT_LEN + 1);
		IOCP.DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	int dbnumber = 0;
	int useridx;

	for (int i = 0; i < MAX_USER_IN_PARTY; i++)
	{
		if (gParty.GetIndexUser(gObj[iIndex].PartyNumber, i, useridx, dbnumber) == FALSE)
		{
			pResult.bReason = 13;
			IOCP.DataSend(iIndex, (LPBYTE)&pResult, pResult.h.size);
			return;
		}

		if (useridx > 0)
		{
			pResult.bReason = 0;
			memcpy(pResult.Name, gObj[iIndex].Name, MAX_ACCOUNT_LEN + 1);
			IOCP.DataSend(useridx, (LPBYTE)&pResult, pResult.h.size);
		}
	}
}

void GameProtocol::CG_ReqAcceptEnterITR(PMSG_REQ_ACCEPTENTER_ITR *lpMsg, int iIndex)
{
	if (!ObjectMaxRange(iIndex))
	{
		return;
	}

	LPOBJ lpObj = &gObj[iIndex];

	if (!lpObj)
	{
		return;
	}

	if (!gObjIsConnected(lpObj))
	{
		return;
	}

	_ITR_AcceptEnter pResult;
	PHeadSubSetB((LPBYTE)&pResult, 0xBF, 0x71, sizeof(pResult));
	memset(pResult.Name, 0x00, sizeof(pResult.Name));

	int byOkCount = 0;
	int UserIndex;
	int dbnumber;
	BYTE byEnterOK = lpMsg->byEnterOK;

	if (gParty.IsParty(lpObj->PartyNumber) == FALSE)
	{
		g_Log.Add("[ ITR ][CGReqAcceptEnterITR] reqman is not party Error Name :%s,", lpObj->Name);
		g_IT_Event.Delete_PartyInfo(lpObj->PartyNumber);
		gParty.ClearPartyAuthITR(lpObj->PartyNumber);
		return;
	}

	if (byEnterOK == TRUE)
	{
		g_Log.Add("[ ITR ][CGReqAcceptEnterITR] reqman opposite Name :%s,", lpObj->Name);
		g_IT_Event.Delete_PartyInfo(lpObj->PartyNumber);
		gParty.ClearPartyAuthITR(lpObj->PartyNumber);

		pResult.byError = -1;
		memcpy(pResult.Name, lpObj->Name, MAX_ACCOUNT_LEN + 1);
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if (gParty.EnterITR_PartyAuth(lpObj->PartyNumber, lpObj->m_Index) == FALSE)
	{
		g_Log.Add("[ ITR ][CGReqAcceptEnterITR][EnterITR_PartyAuth] Internal Error Name :%s", lpObj->Name);
		g_IT_Event.Delete_PartyInfo(lpObj->PartyNumber);
		gParty.ClearPartyAuthITR(lpObj->PartyNumber);

		pResult.byError = -1;
		memcpy(pResult.Name, lpObj->Name, MAX_ACCOUNT_LEN + 1);
		IOCP.DataSend(lpObj->m_Index, (LPBYTE)&pResult, pResult.h.size);
		return;
	}

	if (gParty.AllAgreeEnterITR(lpObj->PartyNumber) == FALSE)
	{
		g_Log.Add("[ ITR ] [CGReqAcceptEnterITR] Enter Agree NAME:%s, G.NAME:%s,AgreeCnt:%d",
			lpObj->Name, lpObj->m_PlayerData->GuildName, gParty.ReturnOKCountITR(lpObj->PartyNumber));
		return;
	}

	int TroubleUserIndex = -1;
	BOOL bResult = EnterITRCheckUser(iIndex, TroubleUserIndex);

	if (bResult != 0)
	{
		pResult.byError = bResult;
		memcpy(pResult.Name, gObj[TroubleUserIndex].Name, MAX_ACCOUNT_LEN + 1);
		int nAllPartyUserIndex;

		for (int j = 0; j < MAX_USER_IN_PARTY; j++)
		{
			gParty.GetIndexUser(lpObj->PartyNumber, j, nAllPartyUserIndex, dbnumber);
			IOCP.DataSend(nAllPartyUserIndex, (LPBYTE)&pResult, pResult.h.size);
		}

		g_IT_Event.Delete_PartyInfo(lpObj->PartyNumber);
		gParty.ClearPartyAuthITR(lpObj->PartyNumber);
		return;
	}

	for (int i = 0; i < MAX_USER_IN_PARTY; i++)
	{
		gParty.GetIndexUser(lpObj->PartyNumber, i, UserIndex, dbnumber);

		if (UserIndex != -1)
		{
			if (gParty.m_PartyS[lpObj->PartyNumber].bITRWithYouOk[i] == FALSE)
			{
				continue;
			}

			BYTE byITRMapIndex = -1;
			BYTE bySlotNum = -1;
			BOOL bReturn = g_IT_Event.Find_ExistingPartyInfo(lpObj->PartyNumber, byITRMapIndex, bySlotNum);

			if (bReturn)
			{
				g_Log.Add("[ ITR ] [Find_ExistingPartyInfo]ERROR return:%d Name:%s, mapnumber:%d, ITLMapIndex:%d,bySlotNum:%d ",
					bReturn, lpObj->Name, lpObj->MapNumber, byITRMapIndex, bySlotNum);

				return;
			}

			if (g_IT_Event.Enter_ITR(UserIndex, byITRMapIndex, bySlotNum) == TRUE)
			{
				pResult.byError = 0;
				memcpy(pResult.Name, gObj[UserIndex].Name, MAX_ACCOUNT_LEN + 1);
				IOCP.DataSend(UserIndex, (LPBYTE)&pResult, pResult.h.size);

				gObj[UserIndex].MapNumber = byITRMapIndex + 45;
				gObjMoveGate(UserIndex, byITRMapIndex + 142);
			}
		}
	}
}

BYTE GameProtocol::EnterITRCheckUser(int iIndex, int &TroubleUserIndex)
{
	int PartyLeaderITRIndex = g_IT_Event.CheckEnterLevel(iIndex);

	if (PartyLeaderITRIndex == -1)
	{
		return 5;
	}

	int useridx = 0;
	int dbnumber = 0;

	for (int i = 0; i < MAX_USER_IN_PARTY; i++)
	{
		BYTE bReturn = gParty.GetIndexUser(gObj[iIndex].PartyNumber, i, useridx, dbnumber);
		TroubleUserIndex = useridx;

		if (bReturn == FALSE)
		{
			return 13;
		}

		if (useridx > 0)
		{
			if (gObj[useridx].MapNumber != MAP_INDEX_LORENMARKET &&
				IT_MAP_RANGE(gObj[useridx].MapNumber) == FALSE)
			{
				return 10;
			}

			if (gObj[useridx].PartyNumber != gObj[iIndex].PartyNumber)
			{
				return 13;
			}

			bReturn = g_IT_Event.CheckEnterLevel(useridx);

			if (PartyLeaderITRIndex != bReturn)
			{
				return 5;
			}

			if (g_IT_Event.CheckWearingMOPH(useridx) == TRUE)
			{
				return 7;
			}

			if (gObj[useridx].m_IfState.use && gObj[useridx].m_IfState.type != 12)
			{
				return 11;
			}

			if (gObj[useridx].m_PK_Level >= 4)
			{
				return 12;
			}

			if (gObjDuelCheck(&gObj[useridx]) == TRUE)
			{
				return 15;
			}

			if (gObj[useridx].m_PlayerData->lpGuild != NULL &&
				gObj[useridx].m_PlayerData->lpGuild->WarState == TRUE)
			{
				return 16;
			}

			if (gObj[useridx].pInventory[gObj[useridx].m_btInvenPetPos].m_Type == ITEMGET(13, 3) ||
				gObj[useridx].pInventory[gObj[useridx].m_btInvenPetPos].m_Type == ITEMGET(13, 2) ||
				gObj[useridx].pInventory[gObj[useridx].m_btInvenPetPos].m_Type == ITEMGET(13, 37))
			{
				return 17;
			}
		}
	}

	return 0;
}

void GameProtocol::GCSendRareItemPriceInfo(int aIndex)
{
	PMSG_RAREITEMPIRCESEND pMsg;
	pMsg.h.set((LPBYTE)&pMsg, 0xFA, 0xA4, sizeof(pMsg));

	pMsg.btItemSellPriceDivisor = g_ConfigRead.itemPrices.btItemSellPriceDivisor;
	pMsg.iJewelOfBlessPrice = g_ConfigRead.itemPrices.iJewelOfBlessPrice;
	pMsg.iJewelOfSoulPrice = g_ConfigRead.itemPrices.iJewelOfSoulPrice;;
	pMsg.iJewelOfChaosPrice = g_ConfigRead.itemPrices.iJewelOfChaosPrice;
	pMsg.iCrestOfMonarchPrice = g_ConfigRead.itemPrices.iCrestOfMonarchPrice;
	pMsg.iJewelOfCreationPrice = g_ConfigRead.itemPrices.iJewelOfCreationPrice;
	pMsg.iLochFeatherPrice = g_ConfigRead.itemPrices.iLochFeatherPrice;
	pMsg.iWereRabbitEggPrice = g_ConfigRead.itemPrices.iWereRabbitEggPrice;
	pMsg.iJewelOfGuardianPrice = g_ConfigRead.itemPrices.iJewelOfGuardianPrice;
	pMsg.iJewelOfLifePrice = g_ConfigRead.itemPrices.iJewelOfLifePrice;

	IOCP.DataSend(aIndex, (LPBYTE)&pMsg, sizeof(pMsg));
}

void GameProtocol::GCSendDisableReconnect(int aIndex)
{
	PMSG_DISABLE_RECONNECT pMsg;

	pMsg.h.set((LPBYTE)&pMsg,0xFA,sizeof(pMsg));
	pMsg.subcode = 0xA5;

	for(int i=0;i<100;i++)
	{
		pMsg.Trash[i] = rand()%0xFF;
	}

	gObj[aIndex].m_dwDCTimer = GetTickCount64();

	IOCP.DataSend(aIndex,(LPBYTE)&pMsg,sizeof(pMsg));

}
void GameProtocol::GCSendDisableReconnectSystem(int aIndex)
{
	PMSG_DISABLE_RECONNECT pMsg;

	pMsg.h.set((LPBYTE)&pMsg,0xFA,sizeof(pMsg));
	pMsg.subcode = 0xA7;

	for(int i=0;i<100;i++)
	{
		pMsg.Trash[i] = rand()%0xFF;
	}

	//gObj[aIndex].m_dwDCTimer = GetTickCount64();

	IOCP.DataSend(aIndex,(LPBYTE)&pMsg,sizeof(pMsg));

}
void GameProtocol::CGReqUBFMyCharacterInfo(PMSG_REQ_UBF_INFO *lpMsg, int iIndex)
{
	if (!ObjectMaxRange(iIndex))
	{
		return;
	}

	if (gObj[iIndex].Type != OBJ_USER)
	{
		return;
	}

	if (lpMsg->btUBFKind == 0)
	{
		g_UnityBattleField.GDReqCheckJoinedUnityBattleField(iIndex, g_ConfigRead.server.GetServerType() == SERVER_BATTLECORE, 0);
	}
}

void GameProtocol::CGReqUBFJoin(PMSG_REQ_UBF_JOIN *lpMsg, int iIndex)
{
	if (!ObjectMaxRange(iIndex))
	{
		return;
	}

	if (gObj[iIndex].Type != OBJ_USER)
	{
		return;
	}

	if (lpMsg->btUBFKind == 0)
	{
		g_UnityBattleField.GDReqJoinUnityBattleField(iIndex);
	}
}

void GameProtocol::CGReqUBFCancel(PMSG_REQ_UBF_CANCEL *lpMsg, int iIndex)
{
	if (!ObjectMaxRange(iIndex))
	{
		return;
	}

	if (gObj[iIndex].Type != OBJ_USER)
	{
		return;
	}

	if (lpMsg->btCanceled == 1)
	{
		g_UnityBattleField.GDReqCancelUnityBattleField(iIndex, 1);
	}
}

void GameProtocol::CGReqUBFGetRealName(PMSG_REQ_UBF_REAL_NAME *lpMsg, int iIndex)
{
	if (!ObjectMaxRange(iIndex))
	{
		return;
	}

	if (gObj[iIndex].Type != OBJ_USER)
	{
		return;
	}

	if (lpMsg->btRequest == 1)
	{
		g_UnityBattleField.GDReqGetRealNameAndServerCode(iIndex);
	}
}

void GameProtocol::CGReqUBFGetReward(PMSG_REQ_GET_REWARD *lpMsg, int iIndex)
{
	if (!ObjectMaxRange(iIndex))
	{
		return;
	}

	if (gObj[iIndex].Type != OBJ_USER)
	{
		return;
	}

	g_UnityBattleField.GDReqUBFGetReward(iIndex, lpMsg->btBattleKind);
}

void GameProtocol::CGReqDSFSchedule(int iIndex)
{
	if (!ObjectMaxRange(iIndex))
	{
		return;
	}

	if (gObj[iIndex].Type != OBJ_USER)
	{
		return;
	}

	if (!gObjIsConnected(iIndex))
	{
		return;
	}

	if (g_ConfigRead.server.GetServerType() != SERVER_BATTLECORE)
	{
		return;
	}

	if (gObj[iIndex].MapNumber != MAP_INDEX_LORENMARKET)
	{
		return;
	}

	g_DevilSquareFinal.ReqDSFSchedule(iIndex);
}

void GameProtocol::CGReqDSFEnter(int iIndex)
{
	if (!ObjectMaxRange(iIndex))
	{
		return;
	}

	if (gObj[iIndex].Type != OBJ_USER)
	{
		return;
	}

	PMSG_ANS_DSF_ENTER pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xDB, 0x01, sizeof(pMsg));

	memset(pMsg.Name, 0x00, sizeof(pMsg.Name));
	pMsg.btDSFType = g_DevilSquareFinal.m_nDSF_TYPE;

	if (g_DevilSquareFinal.m_btDSFCurState != 0)
	{
		pMsg.btResult = 8;
		IOCP.DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	if (g_DevilSquareFinal.m_bEntrance == false)
	{
		pMsg.btResult = 8;
		IOCP.DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	if (gParty.GetLorenMarketUserCount(gObj[iIndex].PartyNumber) != 2)
	{
		g_Log.Add("[DSF][CGReqDSFEnter][%s][%s][%s][%d] EnterError : Party User is not in RorenMarket",
			gObj[iIndex].AccountID, gObj[iIndex].m_PlayerData->m_RealNameOfUBF, gObj[iIndex].Name, gObj[iIndex].m_PlayerData->m_nServerCodeOfHomeWorld);

		pMsg.btResult = 10;
		IOCP.DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	if (gParty.GetPKPartyPenalty(gObj[iIndex].PartyNumber) >= 5)
	{
		pMsg.btResult = 12;
		IOCP.DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	int PartyUsersIndex = 0;
	BOOL bResult = this->EnterDSFCheckUser(iIndex, PartyUsersIndex);

	if (bResult != 0)
	{
		pMsg.btResult = bResult;
		memcpy(pMsg.Name, gObj[PartyUsersIndex].Name, MAX_ACCOUNT_LEN+1);
		IOCP.DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);

		if (PartyUsersIndex != iIndex)
		{
			IOCP.DataSend(PartyUsersIndex, (LPBYTE)&pMsg, pMsg.h.size);
		}

		return;
	}

	BOOL bExistAlready = g_DevilSquareFinal.IsAlreadyExistUserInDSF(iIndex);

	if (bExistAlready != FALSE)
	{
		g_DevilSquareFinal.DeletePartyByIndex(iIndex);
	}

	if (g_DevilSquareFinal.m_nDSF_TYPE == 1)
	{
		BYTE btSlotNum = -1;
		BYTE btRet = g_DevilSquareFinal.Find_EmptyPartySlot(gObj[iIndex].PartyNumber, btSlotNum);

		if (btRet >= 0xFE)
		{
			pMsg.btResult = 14;
			memcpy(pMsg.Name, gObj[iIndex].Name, sizeof(pMsg.Name));
			IOCP.DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);

			return;
		}

		BOOL bUpdateOk = g_DevilSquareFinal.SetParty(btSlotNum, 1, gObj[iIndex].PartyNumber, iIndex);

		if (bUpdateOk != FALSE)
		{
			g_Log.Add("[DSF][CGReqDSFEnter][%s][%s][%s][%d] ERROR code:%d",
				gObj[iIndex].AccountID, gObj[iIndex].m_PlayerData->m_RealNameOfUBF, gObj[iIndex].Name, gObj[iIndex].m_PlayerData->m_nServerCodeOfHomeWorld, bUpdateOk);

			pMsg.btResult = -1;
			memcpy(pMsg.Name, gObj[iIndex].Name, sizeof(pMsg.Name));
			IOCP.DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);

			return;
		}

		int dbnumber = 0, useridx = -1;

		for (int i = 0; i < 2; i++)
		{
			BOOL bError = gParty.GetIndexUser(gObj[iIndex].PartyNumber, i, useridx, dbnumber);

			if (bError == FALSE)
			{
				pMsg.btResult = 13;
				IOCP.DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);
				return;
			}

			if (useridx > 0)
			{
				pMsg.btResult = 0;
				memcpy(pMsg.Name, gObj[iIndex].Name, sizeof(pMsg.Name));
				IOCP.DataSend(useridx, (LPBYTE)&pMsg, pMsg.h.size);

				g_Log.Add("[DSF][CGReqDSFEnter][%s][%s][%s][%d] Request DSF Enter Success And Ask To Party Member, DSFType : %d",
					gObj[iIndex].AccountID, gObj[iIndex].m_PlayerData->m_RealNameOfUBF, 
					gObj[iIndex].Name, gObj[iIndex].m_PlayerData->m_nServerCodeOfHomeWorld,
					g_DevilSquareFinal.m_nDSF_TYPE);
			}
		}
	}

	else if (g_DevilSquareFinal.m_nDSF_TYPE == 2)
	{
		int dbnumber = 0;
		int useridx = -1;
		WORD wServerCode1 = -1;
		WORD wServerCode2 = -1;
		char szUserName1[MAX_ACCOUNT_LEN+1];
		char szUserName2[MAX_ACCOUNT_LEN+1];

		for (int i = 0; i < 2; i++)
		{
			BOOL bError = gParty.GetIndexUser(gObj[iIndex].PartyNumber, i, useridx, dbnumber);

			if (bError == FALSE)
			{
				pMsg.btResult = 13;
				IOCP.DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);
				return;
			}

			if (useridx > 0)
			{
				if (i == 0)
				{
					memcpy(szUserName1, gObj[useridx].m_PlayerData->m_NameConvertOfUBF.ConvertNameToRealName(gObj[useridx].Name), MAX_ACCOUNT_LEN+1);
					wServerCode1 = gObj[useridx].m_PlayerData->m_NameConvertOfUBF.FindServerCodeAtCharacterName(gObj[useridx].Name);
				}

				else if (i == 1)
				{
					memcpy(szUserName2, gObj[useridx].m_PlayerData->m_NameConvertOfUBF.ConvertNameToRealName(gObj[useridx].Name), MAX_ACCOUNT_LEN+1);
					wServerCode2 = gObj[useridx].m_PlayerData->m_NameConvertOfUBF.FindServerCodeAtCharacterName(gObj[useridx].Name);
				}
			}
		}

		BOOL bGoFinal = g_DevilSquareFinal.IsGoSemiFinalParty(szUserName1, szUserName2, wServerCode1, wServerCode2);

		if (bGoFinal == FALSE)
		{
			pMsg.btResult = 18;
			memcpy(pMsg.Name, gObj[iIndex].Name, sizeof(pMsg.Name));
			IOCP.DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);

			return;
		}
					
		BYTE btSlotNum = -1;
		BYTE btRet = g_DevilSquareFinal.Find_EmptyPartySlot(gObj[iIndex].PartyNumber, btSlotNum);

		if (btRet >= 0xFE)
		{
			pMsg.btResult = 14;
			memcpy(pMsg.Name, gObj[iIndex].Name, sizeof(pMsg.Name));
			IOCP.DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);

			return;
		}

		BOOL bUpdateOk = g_DevilSquareFinal.SetParty(btSlotNum, 2, gObj[iIndex].PartyNumber, iIndex);

		if (bUpdateOk != FALSE)
		{
			g_Log.Add("[DSF][CGReqDSFEnter][%s][%s][%s][%d] ERROR code:%d",
				gObj[iIndex].AccountID, gObj[iIndex].m_PlayerData->m_RealNameOfUBF, gObj[iIndex].Name, gObj[iIndex].m_PlayerData->m_nServerCodeOfHomeWorld, bUpdateOk);

			pMsg.btResult = -1;
			memcpy(pMsg.Name, gObj[iIndex].Name, sizeof(pMsg.Name));
			IOCP.DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);

			return;
		}

		for (int i = 0; i < 2; i++)
		{
			BOOL bError = gParty.GetIndexUser(gObj[iIndex].PartyNumber, i, useridx, dbnumber);

			if (bError == FALSE)
			{
				pMsg.btResult = 13;
				IOCP.DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);
				return;
			}

			if (useridx > 0)
			{
				pMsg.btResult = 0;
				memcpy(pMsg.Name, gObj[iIndex].Name, sizeof(pMsg.Name));
				IOCP.DataSend(useridx, (LPBYTE)&pMsg, pMsg.h.size);

				g_Log.Add("[DSF][CGReqDSFEnter][%s][%s][%s][%d] Request DSF Enter Success And Ask To Party Member, DSFType : %d",
					gObj[iIndex].AccountID, gObj[iIndex].m_PlayerData->m_RealNameOfUBF, 
					gObj[iIndex].Name, gObj[iIndex].m_PlayerData->m_nServerCodeOfHomeWorld,
					g_DevilSquareFinal.m_nDSF_TYPE);
			}
		}
	}

	else if (g_DevilSquareFinal.m_nDSF_TYPE == 3)
	{
		int dbnumber = 0;
		int useridx = -1;
		WORD wServerCode1 = -1;
		WORD wServerCode2 = -1;
		char szUserName1[MAX_ACCOUNT_LEN+1];
		char szUserName2[MAX_ACCOUNT_LEN+1];

		for (int i = 0; i < 2; i++)
		{
			BOOL bError = gParty.GetIndexUser(gObj[iIndex].PartyNumber, i, useridx, dbnumber);

			if (bError == FALSE)
			{
				pMsg.btResult = 13;
				IOCP.DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);
				return;
			}

			if (useridx > 0)
			{
				if (i == 0)
				{
					memcpy(szUserName1, gObj[useridx].m_PlayerData->m_NameConvertOfUBF.ConvertNameToRealName(gObj[useridx].Name), MAX_ACCOUNT_LEN+1);
					wServerCode1 = gObj[useridx].m_PlayerData->m_NameConvertOfUBF.FindServerCodeAtCharacterName(gObj[useridx].Name);
				}

				else if (i == 1)
				{
					memcpy(szUserName2, gObj[useridx].m_PlayerData->m_NameConvertOfUBF.ConvertNameToRealName(gObj[useridx].Name), MAX_ACCOUNT_LEN+1);
					wServerCode2 = gObj[useridx].m_PlayerData->m_NameConvertOfUBF.FindServerCodeAtCharacterName(gObj[useridx].Name);
				}
			}
		}

		BOOL bGoFinal = g_DevilSquareFinal.IsGoFinalParty(szUserName1, szUserName2, wServerCode1, wServerCode2);

		if (bGoFinal == FALSE)
		{
			pMsg.btResult = 18;
			memcpy(pMsg.Name, gObj[iIndex].Name, sizeof(pMsg.Name));
			IOCP.DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);

			return;
		}
					
		BYTE btSlotNum = -1;
		BYTE btRet = g_DevilSquareFinal.Find_EmptyPartySlot(gObj[iIndex].PartyNumber, btSlotNum);

		if (btRet >= 0xFE)
		{
			pMsg.btResult = 14;
			memcpy(pMsg.Name, gObj[iIndex].Name, sizeof(pMsg.Name));
			IOCP.DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);

			return;
		}

		BOOL bUpdateOk = g_DevilSquareFinal.SetParty(btSlotNum, 3, gObj[iIndex].PartyNumber, iIndex);

		if (bUpdateOk != FALSE)
		{
			g_Log.Add("[DSF][CGReqDSFEnter][%s][%s][%s][%d] ERROR code:%d",
				gObj[iIndex].AccountID, gObj[iIndex].m_PlayerData->m_RealNameOfUBF, gObj[iIndex].Name, gObj[iIndex].m_PlayerData->m_nServerCodeOfHomeWorld, bUpdateOk);

			pMsg.btResult = -1;
			memcpy(pMsg.Name, gObj[iIndex].Name, sizeof(pMsg.Name));
			IOCP.DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);

			return;
		}

		for (int i = 0; i < 2; i++)
		{
			BOOL bError = gParty.GetIndexUser(gObj[iIndex].PartyNumber, i, useridx, dbnumber);

			if (bError == FALSE)
			{
				pMsg.btResult = 13;
				IOCP.DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);
				return;
			}

			if (useridx > 0)
			{
				pMsg.btResult = 0;
				memcpy(pMsg.Name, gObj[iIndex].Name, sizeof(pMsg.Name));
				IOCP.DataSend(useridx, (LPBYTE)&pMsg, pMsg.h.size);

				g_Log.Add("[DSF][CGReqDSFEnter][%s][%s][%s][%d] Request DSF Enter Success And Ask To Party Member, DSFType : %d",
					gObj[iIndex].AccountID, gObj[iIndex].m_PlayerData->m_RealNameOfUBF, 
					gObj[iIndex].Name, gObj[iIndex].m_PlayerData->m_nServerCodeOfHomeWorld,
					g_DevilSquareFinal.m_nDSF_TYPE);
			}
		}
	}
}

int GameProtocol::EnterDSFCheckUser(int iIndex, int & TroubleUserIndex)
{
	int useridx = -1;
	int dbnumber = 0;
	int nLorenMarketUserCount = 0;

	for (int i = 0; i < 2; i++)
	{
		BOOL bError = gParty.GetIndexUser(gObj[iIndex].PartyNumber, i, useridx, dbnumber);
		TroubleUserIndex = useridx;

		if (bError == FALSE)
		{
			return 13;
		}

		if (useridx > 0)
		{
			if (gObj[useridx].MapNumber == MAP_INDEX_LORENMARKET || gObj[useridx].MapNumber == MAP_INDEX_DEVILSQUARE_FINAL)
			{
				nLorenMarketUserCount++;
			}

			if (gObj[useridx].PartyNumber != gObj[iIndex].PartyNumber)
			{
				return 13;
			}

			if (gObj[useridx].Level < g_DevilSquareFinal.m_DSFUnitInfo.DSFMinLevel)
			{
				return 5;
			}

			if (g_DevilSquareFinal.CheckWearingMOPH(useridx) == TRUE)
			{
				return 7;
			}

			if (gObj[useridx].m_IfState.use && gObj[useridx].m_IfState.type != 12)
			{
				return 11;
			}

			if (gObj[useridx].m_PK_Level >= 4)
			{
				return 12;
			}

			if (gObjDuelCheck(&gObj[useridx]) == TRUE)
			{
				return 15;
			}

			if (gObj[useridx].m_PlayerData->lpGuild != NULL &&
				gObj[useridx].m_PlayerData->lpGuild->WarState == TRUE)
			{
				return 16;
			}
		}
	}

	return 0;
}

void GameProtocol::CGReqAcceptEnterDSF(PMSG_REQ_DSF_ACCEPTENTER *lpMsg, int iIndex)
{
	if (!ObjectMaxRange(iIndex))
	{
		return;
	}

	if (gObj[iIndex].Type != OBJ_USER)
	{
		return;
	}

	if (!gObjIsConnected(&gObj[iIndex]))
	{
		return;
	}

	LPOBJ lpObj = &gObj[iIndex];

	PMSG_ANS_DSF_ACCEPTENTER pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xDB, 0x02, sizeof(pMsg));
	memset(pMsg.Name, 0x00, sizeof(pMsg.Name));

	int useridx = -1, dbnumber = 0;

	if (gParty.IsParty(lpObj->PartyNumber) == FALSE)
	{
		g_Log.Add("[DSF][CGReqAcceptEnterDSF][%s][%s][%s][%d] reqman is not party Error",
			lpObj->AccountID, lpObj->m_PlayerData->m_RealNameOfUBF,
			lpObj->Name, lpObj->m_PlayerData->m_nServerCodeOfHomeWorld);

		g_DevilSquareFinal.DeleteParty(lpObj->PartyNumber);
		gParty.ClearPartyAuthDSF(lpObj->PartyNumber);
		return;
	}

	if (lpMsg->btEnterOK == TRUE)
	{
		g_Log.Add("[DSF][CGReqAcceptEnterDSF][%s][%s][%s][%d] Enter Cancel...",
			lpObj->AccountID, lpObj->m_PlayerData->m_RealNameOfUBF,
			lpObj->Name, lpObj->m_PlayerData->m_nServerCodeOfHomeWorld);

		g_DevilSquareFinal.DeleteParty(lpObj->PartyNumber);
		gParty.ClearPartyAuthDSF(lpObj->PartyNumber);

		pMsg.btError = 24;
		memcpy(pMsg.Name, lpObj->Name, MAX_ACCOUNT_LEN+1);

		for (int i = 0; i < 2; i++)
		{
			gParty.GetIndexUser(lpObj->PartyNumber, i, useridx, dbnumber);

			if (useridx > 0)
			{
				IOCP.DataSend(useridx, (LPBYTE)&pMsg, pMsg.h.size);
			}
		}

		return;
	}

	g_Log.Add("[DSF][CGReqAcceptEnterDSF][%s][%s][%s][%d] Enter OK...",
		lpObj->AccountID, lpObj->m_PlayerData->m_RealNameOfUBF,
		lpObj->Name, lpObj->m_PlayerData->m_nServerCodeOfHomeWorld);

	if (gParty.EnterDSF_PartyAuth(lpObj->PartyNumber, iIndex) == FALSE)
	{
		g_Log.Add("[DSF][CGReqAcceptEnterDSF][%s][%s][%s][%d] Internal Error!!!",
			lpObj->AccountID, lpObj->m_PlayerData->m_RealNameOfUBF,
			lpObj->Name, lpObj->m_PlayerData->m_nServerCodeOfHomeWorld);

		pMsg.btError = -1;
		memcpy(pMsg.Name, lpObj->Name, sizeof(pMsg.Name));
		IOCP.DataSend(iIndex, (LPBYTE)&pMsg, pMsg.h.size);
		return;
	}

	if (gParty.AllAgreeEnterDSF(lpObj->PartyNumber) == FALSE)
	{
		int AgreeCnt = gParty.DSFReturnOKCount(lpObj->PartyNumber);

		g_Log.Add("[DSF[CGReqAcceptEnterDSF][%s][%s][%s][%d] Party Enter Agree AgreeCnt:%d",
			lpObj->AccountID, lpObj->m_PlayerData->m_RealNameOfUBF,
			lpObj->Name, lpObj->m_PlayerData->m_nServerCodeOfHomeWorld, AgreeCnt);

		return;
	}

	int TroubleUserIndex = -1;
	BOOL bResult = this->EnterDSFCheckUser(iIndex, TroubleUserIndex);

	if (bResult != FALSE)
	{
		pMsg.btError = bResult;
		memcpy(pMsg.Name, gObj[TroubleUserIndex].Name, sizeof(pMsg.Name));

		int nAllPartyUserIndex;

		for (int i = 0; i < 2; i++)
		{
			gParty.GetIndexUser(lpObj->PartyNumber, i, nAllPartyUserIndex, dbnumber);
			IOCP.DataSend(nAllPartyUserIndex, (LPBYTE)&pMsg, pMsg.h.size);
		}

		g_DevilSquareFinal.DeleteParty(lpObj->PartyNumber);
		gParty.ClearPartyAuthDSF(lpObj->PartyNumber);
		return;
	}

	for (int i = 0; i < 2; i++)
	{
		gParty.GetIndexUser(lpObj->PartyNumber, i, useridx, dbnumber);

		if (useridx != -1)
		{
			BYTE btSlotNum = -1;
			BOOL bReturn = g_DevilSquareFinal.Find_PartySlot(lpObj->PartyNumber, btSlotNum);

			if (bReturn != FALSE)
			{
				g_Log.Add("[DSF][Find_ExistingPartyInfo][%s][%s][%s][%d] ERROR bReturn:%d, mapnumber:%d, btSlotNum:%d ",
					lpObj->AccountID, lpObj->m_PlayerData->m_RealNameOfUBF, lpObj->Name, lpObj->m_PlayerData->m_nServerCodeOfHomeWorld,
					bReturn, lpObj->MapNumber, btSlotNum);

				return;
			}

			BOOL bEnterOK = g_DevilSquareFinal.Enter_DSF(useridx, btSlotNum);

			if (bEnterOK == FALSE)
			{
				g_DevilSquareFinal.DeleteParty(lpObj->PartyNumber);
				
				g_Log.Add("[DSF][CGReqAcceptEnterDSF][%s][%s][%s][%d] Enter Error bReturn:%d, mapnumber:%d, btSlotNum:%d ",
					lpObj->AccountID, lpObj->m_PlayerData->m_RealNameOfUBF, lpObj->Name, lpObj->m_PlayerData->m_nServerCodeOfHomeWorld,
					bReturn, lpObj->MapNumber, btSlotNum);
			}

			else
			{
				pMsg.btError = 0;
				memcpy(pMsg.Name, gObj[useridx].Name, sizeof(pMsg.Name));
				IOCP.DataSend(useridx, (LPBYTE)&pMsg, pMsg.h.size);

				gObj[useridx].MapNumber = MAP_INDEX_DEVILSQUARE_FINAL;
				gObjMoveGate(useridx, 474);

				g_Log.Add("[DSF][CGReqAcceptEnterDSF][%s][%s][%s][%d] Enter DSF - PartyNo:%d, btSlotNum:%d ",
						gObj[useridx].AccountID, gObj[useridx].m_PlayerData->m_RealNameOfUBF,
					gObj[useridx].Name, gObj[useridx].m_PlayerData->m_nServerCodeOfHomeWorld,
					gObj[useridx].PartyNumber, btSlotNum);
			}
		}
	}
}

void GameProtocol::CGReqDSFGoFinalParty(PMSG_REQ_DSF_GO_FINAL_PARTY_INFO *lpMsg, int iIndex)
{
	if (!ObjectMaxRange(iIndex))
	{
		return;
	}

	if (gObj[iIndex].Type != OBJ_USER)
	{
		return;
	}

	if (!gObjIsConnected(&gObj[iIndex]))
	{
		return;
	}

	if (g_ConfigRead.server.GetServerType() != SERVER_BATTLECORE)
	{
		return;
	}

	BYTE sendBuf[4096];
	memset(sendBuf, 0x00, sizeof(sendBuf));

	PMSG_ANS_DSF_GO_FINAL_PARTY_INFO pMsg;
	DSF_GO_FINAL_PARTY pParty;

	int iSize = sizeof(PMSG_ANS_DSF_GO_FINAL_PARTY_INFO);
	int iCount = 0;

	if (lpMsg->btDSFType == 2)
	{
		for (int i = 0; i < 40; i++)
		{
			if (g_DevilSquareFinal.m_GoSemiFinalParty[i].btRank > 0)
			{
				memcpy(pParty.szUserName1, g_DevilSquareFinal.m_GoSemiFinalParty[i].szUserName1, MAX_ACCOUNT_LEN+1);
				memcpy(pParty.szUserName2, g_DevilSquareFinal.m_GoSemiFinalParty[i].szUserName2, MAX_ACCOUNT_LEN+1);
				pParty.btRank = g_DevilSquareFinal.m_GoSemiFinalParty[i].btRank;
				pParty.btDSFType = g_DevilSquareFinal.m_GoSemiFinalParty[i].btDSFType;
				pParty.iPoint = g_DevilSquareFinal.m_GoSemiFinalParty[i].iPoint;

				memcpy(&sendBuf[iSize], &pParty, sizeof(pParty));
				iSize += sizeof(pParty);
				iCount++;
			}
		}
	}

	if (lpMsg->btDSFType == 3)
	{
		for (int i = 0; i < 10; i++)
		{
			if (g_DevilSquareFinal.m_GoFinalParty[i].btRank > 0)
			{
				memcpy(pParty.szUserName1, g_DevilSquareFinal.m_GoFinalParty[i].szUserName1, MAX_ACCOUNT_LEN+1);
				memcpy(pParty.szUserName2, g_DevilSquareFinal.m_GoFinalParty[i].szUserName2, MAX_ACCOUNT_LEN+1);
				pParty.btRank = g_DevilSquareFinal.m_GoFinalParty[i].btRank;
				pParty.btDSFType = g_DevilSquareFinal.m_GoFinalParty[i].btDSFType;
				pParty.iPoint = g_DevilSquareFinal.m_GoFinalParty[i].iPoint;

				memcpy(&sendBuf[iSize], &pParty, sizeof(pParty));
				iSize += sizeof(pParty);
				iCount++;
			}
		}
	}

	if (lpMsg->btDSFType == 4)
	{
		if (g_DevilSquareFinal.m_WinnerParty.btRank > 0)
		{
			memcpy(pParty.szUserName1, g_DevilSquareFinal.m_WinnerParty.szUserName1, MAX_ACCOUNT_LEN+1);
			memcpy(pParty.szUserName2, g_DevilSquareFinal.m_WinnerParty.szUserName2, MAX_ACCOUNT_LEN+1);
			pParty.btRank = g_DevilSquareFinal.m_WinnerParty.btRank;
			pParty.btDSFType = g_DevilSquareFinal.m_WinnerParty.btDSFType;
			pParty.iPoint = g_DevilSquareFinal.m_WinnerParty.iPoint;

			memcpy(&sendBuf[iSize], &pParty, sizeof(pParty));
			iSize += sizeof(pParty);
			iCount++;
		}
	}

	PHeadSubSetW((LPBYTE)&pMsg, 0xDB, 0x03, iSize);
	pMsg.btCnt = iCount;
	pMsg.btDSFType = lpMsg->btDSFType;

	if (iCount == 0)
	{
		pMsg.btResult = 100;
	}

	else
	{
		pMsg.btResult = 0;
	}

	memcpy(&sendBuf, &pMsg, sizeof(pMsg));
	IOCP.DataSend(iIndex, sendBuf, iSize);

	g_Log.Add("[DSF][CGReqDSFGoFinalParty][%s][%s][%s][%d] ReqType:%d, ResultCount:%d",
		gObj[iIndex].AccountID, gObj[iIndex].m_PlayerData->m_RealNameOfUBF,
        gObj[iIndex].Name, gObj[iIndex].m_PlayerData->m_nServerCodeOfHomeWorld,
		lpMsg->btDSFType, iCount);
}

void GameProtocol::CGReqDSFGetReward(PMSG_REQ_DSF_GET_REWARD *lpMsg, int iIndex)
{
	if (!ObjectMaxRange(iIndex))
	{
		return;
	}

	if (gObj[iIndex].Type != OBJ_USER)
	{
		return;
	}

	if (!gObjIsConnected(&gObj[iIndex]))
	{
		return;
	}

	if (g_ConfigRead.server.GetServerType() != SERVER_BATTLECORE)
	{
		return;
	}

	if (lpMsg->btDSFType == 4)
	{
		g_DevilSquareFinal.GDReqGetReward(iIndex);
	}
}

void GameProtocol::CGReqDoUsePopupType(int aIndex)
{
	if (!ObjectMaxRange(aIndex))
	{
		return;
	}

	if (gObj[aIndex].Type != OBJ_USER)
	{
		return;
	}

	if (!gObjIsConnected(&gObj[aIndex]))
	{
		return;
	}

	PMSG_ANS_USE_POPUP pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xF3, 0x26, sizeof(pMsg));

	pMsg.nPopupType = g_UnityBattleField.GetUBFPopupType();
	IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void GameProtocol::GCSendEventBanner(int aIndex, int iBannerType)
{
	PMSG_SEND_EVENT_BANNER pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0x4D, 0x18, sizeof(pMsg));

	pMsg.iBannerType = iBannerType;
	IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void GameProtocol::GCAlterPShopVault(int aIndex, BYTE btValue)
{
	PMSG_ALTERPSHOPVAULT pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xFA, 0xA8, sizeof(pMsg));
	pMsg.type = btValue;
	IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void GameProtocol::GCSetCharColors(int aIndex)
{
	PMSG_SET_CHAT_COLOR pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xFA, 0x12, sizeof(pMsg));

	memcpy(pMsg.btInfoMsg, g_ConfigRead.data.chatcolor.btInfoMsg, sizeof(pMsg.btInfoMsg));
	memcpy(pMsg.btErrorMsg, g_ConfigRead.data.chatcolor.btErrorMsg, sizeof(pMsg.btErrorMsg));
	memcpy(pMsg.btChatMsg, g_ConfigRead.data.chatcolor.btChatMsg, sizeof(pMsg.btChatMsg));
	memcpy(pMsg.btWhisperMsg, g_ConfigRead.data.chatcolor.btWhisperMsg, sizeof(pMsg.btWhisperMsg));
	memcpy(pMsg.btPartyMsg, g_ConfigRead.data.chatcolor.btPartyMsg, sizeof(pMsg.btPartyMsg));
	memcpy(pMsg.btGuildMsg, g_ConfigRead.data.chatcolor.btGuildMsg, sizeof(pMsg.btGuildMsg));
	memcpy(pMsg.btAllianceMsg, g_ConfigRead.data.chatcolor.btAllianceMsg, sizeof(pMsg.btAllianceMsg));
	memcpy(pMsg.btGensMsg, g_ConfigRead.data.chatcolor.btGensMsg, sizeof(pMsg.btGensMsg));
	memcpy(pMsg.btGMChatMsg, g_ConfigRead.data.chatcolor.btGMChatMsg, sizeof(pMsg.btGMChatMsg));

	IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
}

void GameProtocol::CGRecvHitHackValues(int aIndex, PMSG_SEND_HITHACK_INFO * lpMsg)
{
	if(!g_ConfigRead.data.common.bHitHackDetection)
	{
		return;
	}

	if(!ObjectMaxRange(aIndex))
	{
		return;
	}
	
	if(!gObjIsConnectedGP(aIndex))
	{
		return;
	}

	bool bMismatchFound = false;
	bool bIsAllValues1Equal = false;;
	bool bIsAllValues2Equal = false;;
	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->m_PlayerData->Dexterity > g_ConfigRead.data.common.iHitHackMaxAgility)
	{
		lpObj->m_PlayerData->dwLastHitHackTick = GetTickCount64();
		return;
	}

	DWORD dwCount = lpObj->m_PlayerData->dwHitHackCounter1;

	if((dwCount+1) % 7 != 0)
	{
		lpObj->m_PlayerData->dwLastHitHackValue1[dwCount] = lpMsg->dwHitHackVal1;
		lpObj->m_PlayerData->dwLastHitHackValue2[dwCount] = lpMsg->dwHitHackVal2;
		lpObj->m_PlayerData->dwHitHackCounter1++;
	}
	else
	{
		lpObj->m_PlayerData->dwHitHackCounter1 = 0;
		dwCount = 0;

		lpObj->m_PlayerData->dwLastHitHackValue1[6] = lpMsg->dwHitHackVal1;
		lpObj->m_PlayerData->dwLastHitHackValue2[6] = lpMsg->dwHitHackVal2;	

	}
  
	for (int i = 0; i < 7-1; i++)      
    {         
        if (lpObj->m_PlayerData->dwLastHitHackValue1[i] != lpObj->m_PlayerData->dwLastHitHackValue1[i + 1])
		{
			bMismatchFound = true;
		}
    }
    
	if(!bMismatchFound)
	{
		if (lpObj->m_PlayerData->dwLastHitHackValue1[0] != 0)
		{
			bIsAllValues1Equal = true;
		}
	}

	bMismatchFound = false;
   
	for (int i = 0; i < 7-1; i++)      
    {         
        if (lpObj->m_PlayerData->dwLastHitHackValue2[i] != lpObj->m_PlayerData->dwLastHitHackValue2[i + 1])
		{
			bMismatchFound = true;
		}
    }

	if(!bMismatchFound)
	{
		if (lpObj->m_PlayerData->dwLastHitHackValue2[0] =! 0)
		{
			bIsAllValues2Equal = true;
		}
	}

	if(bIsAllValues1Equal || bIsAllValues2Equal)
	{
		g_Log.AddC(TColor::Red,"[AntiHack][%s][%s][%s] Hit Hack detected", lpObj->m_PlayerData->Ip_addr, lpObj->AccountID, lpObj->Name);
		AntiHackLog->Output("[AntiHack][%s][%s][%s] Hit Hack detected", lpObj->m_PlayerData->Ip_addr, lpObj->AccountID, lpObj->Name);
		GSProtocol.GCSendDisableReconnect(aIndex);
	}

	lpObj->m_PlayerData->dwLastHitHackTick = GetTickCount64();

}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

