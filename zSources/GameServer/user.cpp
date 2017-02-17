//////////////////////////////////////////////////////////////////////
// user.cpp
// ------------------------------
// Decompiled by Deathway
// Date : 2007-05-09
// ------------------------------
// GS-N 0.99.60T 0x004B1200
/*	GS-N	1.00.18	JPN	0x004CF4C0	-	Completed
	void gObjSpriteDamage(LPOBJ lpObj, int damage)	-	Wierd Compilation - Status OK
	void gObjTradeCancel(int aIndex)	-	Wierd Compilation - Status OK
	BOOL gObjJoominCheck(int aIndex, char* szInJN)	-	strcmpi comflict symbols
	void SkillFrustrum()	-	Wrong Symbol NAmes of the zzmath functions
*/
#include "stdafx.h"
#include "user.h"
#include "TLog.h"
#include "LogToFile.h"
#include "GameServer.h"
#include "GameMain.h"
#include "DoppelGanger.h"
#include "MapServerManager.h"
#include "gObjMonster.h"
#include "ObjCalCharacter.h"
#include "ObjUseSkill.h"
#include "DarkSpirit.h"
#include "DSProtocol.h"
#include "DevilSquare.h"
#include "protocol.h"
#include "ImperialGuardian.h"
#include "MonsterAttr.h"
#include "SendNPCInfo.h"
#include "ChaosBox.h"
#include "PacketChecksum.h"
#include "QuestInfo.h"
#include "EledoradoEvent.h"
#include "TNotice.h"
#include "GMMng.h"
#include "zzzmathlib.h"
#include "Gate.h"
#include "ObjAttack.h"
#include "SProtocol.h"
#include "MultiAttackHAckCheck.h"
#include "BattleSoccer.h"
#include "BattleSoccerManager.h"
#include "TUnion.h"
#include "TMonsterAIElement.h"
#include "winutil.h"
#include "Weapon.h"
#include "CrywolfSync.h"
#include "Kanturu.h"
#include "CashShop.h"
#include "Mercenary.h"
#include "KalimaGate.h"
#include "KanturuBattleUserMng.h"
#include "Guardian.h"
#include "MoveCommand.h"
#include "StatMng.h"
#include "SetItemOption.h"
#include "SkillAdditionInfo.h"
#include "PeriodItemEx.h"
#include "TMonsterSkillManager.h"
#include "IllusionTempleEvent_Renewal.h"
#include "BuffEffect.h"
#include "BuffEffectSlot.h"
#include "MasterSkillSystem.h"
#include "PentagramMixSystem.h"
#include "MasterLevelSkillTreeSystem.h"
//Added by HermeX
#include "Crywolf.h"
#include "CrywolfStatue.h"
#include "CrywolfAltar.h"
#include "LifeStone.h"
#include "GuardianStatue.h"
#include "CannonTower.h"
#include "CastleCrown.h"
#include "CastleCrownSwitch.h"
#include "CastleSiegeSync.h"
#include "Raklion.h"
#include "RaklionUtil.h"
#include "RaklionBattleUserMng.h"
#include "RaklionMonsterMng.h"
#include "ItemSocketOptionSystem.h"
#include "ChaosCastle.h"
#include "configread.h"
#include "LastManStanding.h"
#include "MapAttribute.h"
#include "GensSystem.h"
#include "NewPVP.h"
#include "VipSys.h"
#include "ExpManager.h"
#include "QuestExpProgMng.h"
#include "QuestExpInfo.h"
#include "LuckyItemManager.h"
#include "PentagramSystem.h"
#include "MuunSystem.h"
#include "ChaosCastleFinal.h"// directions
#include "MuRummyMng.h"
#include "MineSystem.h"
#include "Shop.h"
#include "PersonalStore.h"
#include "LargeRand.h"
#include "AcheronGuardianEvent.h"
#include "ItemOptionTypeMng.h"
#include "BonusEvent.h"
#include "CItemDrop.h"
#include "DevilSquareFinal.h"
#include "BotSystem.h"
#include "OfflineLevelling.h"

inline bool ObjectMaxRange(int Index)
{
	if (Index < 0 || Index > g_ConfigRead.server.GetObjectMax() - 1)
		return false;

	return true;
}

USER_DATA::USER_DATA()
{
	InitializeCriticalSection(&this->m_MasterSkillCriti);
	InitializeCriticalSection(&this->m_DarkSideRFCriti);
	InitializeCriticalSection(&this->AgiCheckCriti);
	this->m_pCMuRummyInfo = new CMuRummyInfo;
	this->pPentagramMixBox = new CItem[CHAOS_BOX_SIZE];
	this->pPentagramMixBoxMap = new BYTE[CHAOS_BOX_MAP_SIZE];

	this->Init();
}

USER_DATA::~USER_DATA()
{
	DeleteCriticalSection(&this->m_MasterSkillCriti);
	DeleteCriticalSection(&this->m_DarkSideRFCriti);
	DeleteCriticalSection(&this->AgiCheckCriti);

	if (this->m_pCMuRummyInfo != NULL)
	{
		delete this->m_pCMuRummyInfo;
		this->m_pCMuRummyInfo = NULL;
	}

	if (this->pPentagramMixBox != NULL)
	{
		delete[] this->pPentagramMixBox;
		this->pPentagramMixBox = NULL;
	}

	if (this->pPentagramMixBoxMap != NULL)
	{
		delete[] this->pPentagramMixBoxMap;
		this->pPentagramMixBoxMap = NULL;
	}
}

void USER_DATA::Init(bool VipReset)
{
	memset(this->m_RealNameOfUBF, 0, sizeof (this->m_RealNameOfUBF));
	memset(this->GuildName, 0, sizeof (this->GuildName));
	this->GuildNumber = 0;
	this->iGuildUnionTimeStamp = 0;
	this->lpGuild = NULL;	
	this->GuildStatus = -1;
	this->SantaCount = 0;
	this->RegisterdLMS = 0;
	this->RegisteredLMSRoom = -1;
	this->MoveGate = -1;
	g_kJewelOfHarmonySystem.InitEffectValue(&this->m_JewelOfHarmonyEffect);
	g_kItemSystemFor380.InitEffectValue(&this->m_ItemOptionExFor380);
	this->m_iResets = 0;

	this->LastAuthTime = 0;	
	if (g_ConfigRead.server.GetServerType() != SERVER_CASTLE)
	{
		this->m_bKanturuEntranceByNPC = FALSE;
	}

	this->SetOpAddMaxAttackDamage = 0;
	this->SetOpAddMinAttackDamage = 0;
	this->SetOpAddDamage = 0;
	this->SetOpIncAGValue = 0;
	this->SetOpAddCriticalDamageSuccessRate = 0;
	this->SetOpAddCriticalDamage = 0;
	this->SetOpAddExDamageSuccessRate = 0;
	this->SetOpAddExDamage = 0;
	this->SetOpAddSkillAttack = 0;	
	this->SetOpAddAttackDamage = 0;
	this->SetOpAddDefence = 0;
	this->SetOpAddMagicPower = 0;
	this->SetOpAddDefenceRate = 0;
	this->SetOpIgnoreDefense = 0;
	this->SetOpDoubleDamage = 0;
	this->SetOpTwoHandSwordImproveDamage = 0;
	this->SetOpImproveSuccessAttackRate = 0;
	this->SetOpReflectionDamage = 0;
	this->SetOpImproveSheldDefence = 0;
	this->SetOpDecreaseAG = 0;
	this->SetOpImproveItemDropRate = 0;
	this->IsFullSetItem = false;	
	this->m_ReqWarehouseOpen = 0;	
	this->m_MuBotEnable = false;
	this->m_MuBotPayTime = 0;
	this->m_MuBotTotalTime = 0;
	this->m_WCoinC = 0.0;
	this->m_WCoinP = 0.0;
	this->m_GoblinPoint = 0.0;
	this->m_iGensInfoLoad = false;
	this->m_iQuestInfoLoad = false;
	this->m_iPeriodItemEffectIndex = -1;
	this->SkillHellFire2State = 0;
	this->SkillHellFire2Count = 0;
	this->SkillStrengthenHellFire2State = 0;
	this->SkillStrengthenHellFire2Count = 0;
	this->PacketsPerSecond = 0;
	this->SwearWarning = 0;
	this->PlusStatQuestClear = false;
	this->ComboSkillquestClear = false;
	memset(this->m_Quest, (BYTE)-1, sizeof(this->m_Quest));
	this->m_SendQuestInfo = 0;
	this->BlessSkillCast = false;
	this->RFSkillCount = 0;
	this->DarkSideCount = 0;
	this->m_btSculptPos = 0;
	this->m_btCharmPos = 0;
	this->m_btArtifactPos = 0;
	this->m_btExpUpCharmPos = 0;
	this->MasterLevel = 0;
	this->MasterExperience = 0;
	this->MasterNextExp = 0;
	this->MasterPoint = 0;
	this->m_GensInfluence = NONE_INFLUENCE;
	this->m_GensClass = 0;
	this->m_GensRanking = 0;
	this->m_GensAbusingTick = 0;
	this->m_ContributePoint = 0;
	this->m_NextContributePoint = 0;
	this->PentagramInfoLoad = 0;
	this->AgilityCheckDelay = GetTickCount();
	this->AgilityCheckCount = 0;
	this->ISBOT = false;
	this->wBotIndex = -1;
	for(int i=0;i<5;i++)
	{
		this->DarkSideTarget[i] = 10000;
	}
	
	for (int i=0;i<250;i++)
	{
		this->m_PentagramJewelInfo_Inven[i].Clear();
	}

	for (int i=0;i<250;i++)
	{
		this->m_PentagramJewelInfo_Warehouse[i].Clear();
	}

	this->m_bUserQuestInfoSent = FALSE;

	for (int i = 0; i < MAX_QUESTEXP_USER_INFO; i++)
	{
		this->m_UserQuestInfo[i].Clear();
	}

	this->PotionTime = 0;
	this->m_dwMSBFlagAttackDisable = GetTickCount();
	this->m_bUsePolymorphSkin = false;
	this->LastSkillUseCount = 0;
	this->LastSkillUseNumber = 0;
	this->LastSkillUseTick = GetTickCount64();
	this->PentagramMixMoney = 0;
	this->PentagramMixSuccessRate = 0;
	this->m_PentagramMixLock = FALSE;
	this->m_bIsPentagramMixCompleted = false;
	this->m_bUsePartyMatching = false;
	this->m_bPartyMatchingLeader = false;
	this->m_nServerChannel = -1;
	this->m_nPartyMatchingIndex = -1;
	this->m_nPartyMatchingMemberCount = 0;
	this->m_bUseGuildMatching = false;
	this->m_bSendNpcPos = FALSE;
	this->m_bSendMemberPos = FALSE;
	this->m_Resistance_Perfect = 0;
	this->m_Resistance_SD = 0;
	this->m_Resistance_Stun = 0;
	this->m_Resistance_Double = 0;
	this->m_Resistance_Critical = 0;
	this->m_Resistance_Excellent = 0; 
	this->m_EnableUseChangeSkin = 0;
	this->AntiHackCheckTime = GetTickCount();
	this->m_dwMapMoveKeyValue = 0;
	this->m_bIsCancelItemSale = false;
	this->m_bGremoryCaseDBInfo = false;
	this->m_JoinUnityBattle = false;
	this->m_nServerCodeOfHomeWorld = -1;
	this->SkillDelay.Init();
	this->m_dwGensHitTeleportDelay = 0;
	this->bActiveSetParty = false;
	this->dwOpenWareCnt = 0;
	this->dwCloseWareCnt = 0;
	this->bHasSameSerialItem = false;
	this->Ruud = 0;

	this->dwHitHackCounter1 = 0;
	this->dwHitHackCounter2 = 0;

	memset(this->dwLastHitHackValue1,0,sizeof(this->dwLastHitHackValue1));
	memset(this->dwLastHitHackValue2,0,sizeof(this->dwLastHitHackValue2));

	this->dwLastHitHackTick = GetTickCount64();

	memset(this->PartyPassword, 0x00, sizeof(this->PartyPassword));

	for (int i = 0; i < MAX_CANCEL_ITEMS_SALE; i++)
	{
		this->m_CancelItemSaleList[i].Clear();
	}

	for (int i = 0; i < MAX_GREMORYCASE_STORAGE_TYPES; i++)
	{
		for (int j = 0; j < MAX_GREMORYCASE_STORAGE_ITEMS; j++)
		{
			this->m_GremoryCaseData[i][j].Clear();
		}
	}

	if (VipReset == true)
	{ 
		this->VipType = 0;
		this->VipEffect = VIP_EFFECT_NONE;
	}
}

//Y
//|-------
//||6|5|4|
//||7|9|3|
//||8|1|2|
//|------- X

int ChangeCount; 
int lOfsChange;
int GuildUserCount;
int GuildUserOfs;
int GuildInfoCount;
int GuildInfoOfs;
int GensInfoOfs;
int GensInfoCount;
int gDisconnect;
int gObjTotalUser;
int gObjTotalMonster;
int gItemLoopMax;
int gItemLoop;
int gItemLoop2;

int MaxViewportMonster = 20;

CViewportGuild ViewGuildMng;	// line : 130
OBJECTSTRUCT * gObj;
int gObjCount;
int gObjMonCount;
int gObjCallMonCount;
MessageStateMachine ** gSMMsg;	
ExMessageStateMachine ** gSMAttackProcMsg;
BILL_CLASS * m_ObjBill;	// line : 193
static CRaklionUtil RAKLION_UTIL;
static CLogToFile * CharSaveLog;
CItem * pTempInventory; 
BYTE * pTempInventoryMap;

BYTE GuildInfoBuf[10000];
BYTE GuildUserBuf[10000];
BYTE GensInfoBuf[10000];
BYTE GensUserBuf[10000];
BYTE SendGBufChange[5000];

PMSG_VIEWPORTCREATE pViewportCreate;
PMSG_VIEWPORTCREATE_CHANGE pViewportCreateChange;
PMSG_VIEWPORTDESTROY pViewportDestroy;
PMSG_MONSTER_VIEWPORTCREATE pMonsterViewportCreate;
PMSG_CALLMONSTER_VIEWPORTCREATE pCallMonsterViewportCreate;

#pragma pack(1)

PWMSG_COUNT pItemCount;
PMSG_ITEMVIEWPORTCREATE pItemViewportCreate;
PMSG_VIEWPORTDESTROY pItemViewportDestroy;

#pragma pack()


short RoadPathTable[MAX_ROAD_PATH_TABLE] = { -1, -1, 0, -1, 1, -1, 1, 0, 1, 1, 0, 1, -1, 1, -1, 0 };	// THIS VARIABLE HAS SOME STRANGE THINGS ONT ITS UBICATION
BOOL g_EnergyCheckOff=1; // THIS VARIABLE HAS SOME STRANGE THINGS ONT ITS UBICATION


void gObjSkillUseProc(LPOBJ lpObj)
{
	if(lpObj->m_SkillInfo.RemedyOfLoveTime > 0)
	{
		lpObj->m_SkillInfo.RemedyOfLoveTime--;

		if(lpObj->m_SkillInfo.RemedyOfLoveTime <= 0)
		{
			lpObj->m_SkillInfo.RemedyOfLoveEffect = 0;
			lpObj->m_SkillInfo.RemedyOfLoveTime = 0;
		}
	}

	if(lpObj->m_SkillInfo.LordSummonTime != 0 && lpObj->Type == OBJ_USER)
	{
		lpObj->m_SkillInfo.LordSummonTime--;

		if(lpObj->m_SkillInfo.LordSummonTime <= 0)
		{
			lpObj->m_SkillInfo.LordSummonTime = 0;

			int m_check1 = gMoveCommand.CheckMainToMove(lpObj);
			int m_check2 = gMoveCommand.CheckEquipmentToMove(lpObj,lpObj->m_SkillInfo.LordSummonMapNumber);
			int m_check3 = gMoveCommand.CheckInterfaceToMove(lpObj);

			if(m_check1 == false || m_check2 == false || m_check3 == false)
			{
					char szMsg[256];
					wsprintf(szMsg,Lang.GetText(0,146));
					
					GSProtocol.GCServerMsgStringSend(szMsg,lpObj->m_Index,1);
			}
			else
			{
				gObjRecall(lpObj->m_Index,lpObj->m_SkillInfo.LordSummonMapNumber,lpObj->m_SkillInfo.LordSummonX,lpObj->m_SkillInfo.LordSummonY);
			}
			//season4 add-on (fix)
			lpObj->m_SkillInfo.LordSummonMapNumber = 0;
			lpObj->m_SkillInfo.LordSummonTime = 0;
			lpObj->m_SkillInfo.LordSummonX = 0;
			lpObj->m_SkillInfo.LordSummonY = 0;
		}
		else if(lpObj->m_SkillInfo.LordSummonTime <= 5 && lpObj->m_SkillInfo.LordSummonTime != 0)
		{
			char szMsg[256];
			wsprintf(szMsg,Lang.GetText(0,135),lpObj->m_SkillInfo.LordSummonTime);
				
			GSProtocol.GCServerMsgStringSend(szMsg,lpObj->m_Index,1);
		}
	}

	TMonsterSkillElementInfo::CheckSkillElementInfoProc(lpObj);

	if(IT_MAP_RANGE(lpObj->MapNumber)) //season 2.5 add-on
	{
		if (lpObj->m_nITR_Index != -1)
		{
			g_IT_Event.EventSkillProc(lpObj);
		}
	}

	if(lpObj->m_SkillNumber < 1)
	{
		return;
	}

	if(lpObj->m_SkillNumber == 18)
	{
		if(GetTickCount() > lpObj->m_SkillTime)
		{
			lpObj->m_SkillNumber = 0;
		}
	}
}

void gObjSkillUseProcTime500(LPOBJ lpObj)
{
	if(lpObj->Type != OBJ_USER)
		return;

	if ( lpObj->m_PlayerData->SkillHellFire2State != 0 )
	{
		unsigned long dwCurrentTick = GetTickCount();
		lpObj->m_PlayerData->SkillHellFire2Count = (dwCurrentTick - lpObj->m_PlayerData->SkillHellFire2Time) / 500;

		if ( lpObj->m_PlayerData->SkillHellFire2Count > 12 )
		{
			lpObj->m_PlayerData->SkillHellFire2Count = 12;
		}

		if(dwCurrentTick >= (lpObj->m_PlayerData->SkillHellFire2Time + 6000))
		{
			CMagicInf * lpMagic = gObjGetMagicSearch(lpObj,40);

			if(lpMagic)
			{
				gObjUseSkill.RunningSkill(lpObj->m_Index,0,lpMagic,0);
			}
		}
		else
		{
			int mana = MagicDamageC.SkillGetMana(40);
			mana = mana * 20 / 100;
			BOOL bSuccess = TRUE;

			if ( mana > lpObj->Mana )
			{
				bSuccess = FALSE;
				int count = 0;

				while ( true )
				{
					count++;

					if ( gObjIsConnected(lpObj) == FALSE )
					{
						g_Log.Add("[CHECK_LOG_INFINITY] gObjIsConnected() error %s %d", __FILE__, __LINE__);
						break;
					}

					if ( count > 100 )
					{
						g_Log.Add("[CHECK_LOG_INFINITY] ( _count > 100 ) error %s %d", __FILE__, __LINE__);
						break;
					}

					int pos = gObjGetManaItemPos(lpObj->m_Index);

					if ( pos == -1 )
					{
						break;
					}

					PMSG_USEITEM pMsg;
					pMsg.inventoryPos = pos;
					pMsg.invenrotyTarget = 0;
					GSProtocol.CGUseItemRecv(&pMsg, lpObj->m_Index);

					if ( mana <= lpObj->Mana )
					{
						bSuccess = TRUE;
						break;
					}
				}
			}
			
			if ( bSuccess == FALSE )
			{
				CMagicInf * lpMagic = gObjGetMagicSearch(lpObj, 40);
				
				if (lpMagic)
				{
					gObjUseSkill.RunningSkill(lpObj->m_Index, 0, lpMagic, 0);
				}
			}
			else
			{
				lpObj->Mana -= mana;

				if ( lpObj->Mana < 0.0f )
				{
					lpObj->Mana = 0.0f;
				}

				GSProtocol.GCManaSend(lpObj->m_Index, lpObj->Mana, -1, 0, lpObj->BP);
				PMSG_EX_SKILL_COUNT pMsg;
				PHeadSetB( (LPBYTE)&pMsg, 0xBA, sizeof(pMsg));
				pMsg.Type = 40;
				pMsg.Count = lpObj->m_PlayerData->SkillHellFire2Count;
				pMsg.NumberH = SET_NUMBERH(lpObj->m_Index);
				pMsg.NumberL = SET_NUMBERL(lpObj->m_Index);

				IOCP.DataSend(lpObj->m_Index, (UCHAR *)&pMsg, pMsg.h.size);
				GSProtocol.MsgSendV2(lpObj, (UCHAR *)&pMsg, pMsg.h.size);
			}
		}
	}

	if (lpObj->m_PlayerData->SkillStrengthenHellFire2State != 0)
	{
		unsigned long dwCurrentTick = GetTickCount();
		lpObj->m_PlayerData->SkillStrengthenHellFire2Count = (dwCurrentTick - lpObj->m_PlayerData->SkillStrengthenHellFire2Time) / 500;

		if (lpObj->m_PlayerData->SkillStrengthenHellFire2Count > 12)
		{
			lpObj->m_PlayerData->SkillStrengthenHellFire2Count = 12;
		}

		if (dwCurrentTick >= (lpObj->m_PlayerData->SkillStrengthenHellFire2Time + 6000))
		{
			CMagicInf * lpMagic = gObjGetMagicSearch(lpObj, 392);

			if (lpMagic)
			{
				gObjUseSkill.RunningSkill(lpObj->m_Index, 0, lpMagic, 0);
			}
		}
		else
		{
			int mana = MagicDamageC.SkillGetMana(392);
			mana = mana * 20 / 100;
			BOOL bSuccess = TRUE;

			if (mana > lpObj->Mana)
			{
				bSuccess = FALSE;
				int count = 0;

				while (true)
				{
					count++;

					if (gObjIsConnected(lpObj) == FALSE)
					{
						g_Log.Add("[CHECK_LOG_INFINITY] gObjIsConnected() error %s %d", __FILE__, __LINE__);
						break;
					}

					if (count > 100)
					{
						g_Log.Add("[CHECK_LOG_INFINITY] ( _count > 100 ) error %s %d", __FILE__, __LINE__);
						break;
					}

					int pos = gObjGetManaItemPos(lpObj->m_Index);

					if (pos == -1)
					{
						break;
					}

					PMSG_USEITEM pMsg;
					pMsg.inventoryPos = pos;
					pMsg.invenrotyTarget = 0;
					GSProtocol.CGUseItemRecv(&pMsg, lpObj->m_Index);

					if (mana <= lpObj->Mana)
					{
						bSuccess = TRUE;
						break;
					}
				}
			}

			if (bSuccess == FALSE)
			{
				CMagicInf * lpMagic = gObjGetMagicSearch(lpObj, 392);

				if (lpMagic)
				{
					gObjUseSkill.RunningSkill(lpObj->m_Index, 0, lpMagic, 0);
				}
			}
			else
			{
				lpObj->Mana -= mana;

				if (lpObj->Mana < 0.0f)
				{
					lpObj->Mana = 0.0f;
				}

				GSProtocol.GCManaSend(lpObj->m_Index, lpObj->Mana, -1, 0, lpObj->BP);
				PMSG_EX_SKILL_COUNT pMsg;
				PHeadSetB((LPBYTE)&pMsg, 0xBA, sizeof(pMsg));
				pMsg.Type = 392;
				pMsg.Count = lpObj->m_PlayerData->SkillStrengthenHellFire2Count;
				pMsg.NumberH = SET_NUMBERH(lpObj->m_Index);
				pMsg.NumberL = SET_NUMBERL(lpObj->m_Index);

				IOCP.DataSend(lpObj->m_Index, (UCHAR *)&pMsg, pMsg.h.size);
				GSProtocol.MsgSendV2(lpObj, (UCHAR *)&pMsg, pMsg.h.size);
			}
		}
	}
}

void MonsterAndMsgProc()
{
	LPOBJ lpObj;
	int n;
	int aIndex;

	for ( n=0;n<g_ConfigRead.server.GetObjectMax();n++)
	{
		lpObj = &gObj[n];

		if ( lpObj->Connected == PLAYER_PLAYING  )
		{
			if ( lpObj->Type == OBJ_MONSTER || lpObj->Type == OBJ_NPC) 
			{
				if(lpObj->m_iCurrentAI != 0)
				{
					continue;
				}
				gObjMonsterProcess(lpObj);
			}

			else if ( lpObj->Type == OBJ_USER )
			{
				gObjSkillUseProcTime500(lpObj);
				gObjMsgProc(lpObj);
				CreateFrustrum(lpObj->X, lpObj->Y, n);
			}

			if ( lpObj->Type == OBJ_USER && lpObj->m_PlayerData->ISBOT == false && n >= g_ConfigRead.server.GetObjectStartUserIndex() )
			{
				gDarkSpirit[n- g_ConfigRead.server.GetObjectStartUserIndex()].Run();
			}
		}
		else if ( lpObj->Connected >= PLAYER_LOGGED )
		{
			if ( lpObj->Type == OBJ_USER )
			{
				gObjMsgProc(lpObj);

				if ( lpObj->Connected == PLAYER_PLAYING )
				{
					CreateFrustrum(lpObj->X, lpObj->Y, n);
				}
			}
		}

		else
		{
			gObjMsgProc(lpObj);
		}
	}

	for ( n=0;n<g_ConfigRead.server.GetObjectMax();n++)
	{
		lpObj = &gObj[n];

		if ( lpObj->Connected == PLAYER_PLAYING  )
		{
			aIndex = lpObj->m_Index;

			for ( int i=0;i<MAX_MONSTER_SEND_ATTACK_MSG;i++)
			{
				if ( gSMAttackProcMsg[n][i].MsgCode >= 0 )
				{
					if ( GetTickCount64() > gSMAttackProcMsg[n][i].MsgTime )
					{
						gObjStateAttackProc(lpObj, gSMAttackProcMsg[n][i].MsgCode, gSMAttackProcMsg[n][i].SendUser,
							gSMAttackProcMsg[n][i].SubCode, gSMAttackProcMsg[n][i].SubCode2);
						gSMAttackProcMsg[n][i].MsgCode = -1;
					}
				}
			}
		}
	}
	TMonsterSkillManager::MonsterSkillProc();
}

void MoveMonsterProc()
{
	DWORD MoveTime;
	int DelayTime;
	LPOBJ lpObj;

	for(int n = 0; n < g_ConfigRead.server.GetObjectMax();n++)
	{
		lpObj = &gObj[n];

		if(lpObj->m_iCurrentAI != 0)
		{
			if(lpObj->Type == OBJ_MONSTER || lpObj->Type == OBJ_NPC)
			{
				continue;
			}
		}
		
		if ( DG_MAP_RANGE(gObj[n].MapNumber) )
		{
			continue;
		}

		if(lpObj->Connected == PLAYER_PLAYING)
		{
			if(lpObj->Type == OBJ_MONSTER)
			{
				if (g_ConfigRead.server.GetServerType() != SERVER_CASTLE)
				{
					if(ATTRIBUTE_RANGE(lpObj->m_Attribute) != FALSE)
					{
						CreateFrustrum(lpObj->X, lpObj->Y, n);
						continue;
					}
				}
				
				else
				{
					if (ATTRIBUTE_RANGE(lpObj->m_Attribute) != FALSE || lpObj->Class == 277 || lpObj->Class == 283 || lpObj->Class == 288 || lpObj->Class == 278 || lpObj->Class == 216 || lpObj->Class == 217 || lpObj->Class == 218 || lpObj->Class == 219 || (CRYWOLF_ALTAR_CLASS_RANGE(lpObj->Class) != FALSE || CRYWOLF_STATUE_CHECK(lpObj->Class) != FALSE))
					{
						CreateFrustrum(lpObj->X, lpObj->Y, n);
						continue;
					}
				}
			}
			
			if(lpObj->m_State == 2)
			{
				if(lpObj->PathCount != 0)
				{
					if(lpObj->DelayLevel != 0)
					{
						DelayTime = 300;
					}
					else
					{
						DelayTime = 0;
					}
	
					if(lpObj->Type == OBJ_MONSTER && lpObj->m_RecallMon >= 100)
					{
						lpObj->m_MoveSpeed = 200;
					}
					else
					{
						lpObj->m_MoveSpeed = 400;
					}
	
					if(lpObj->PathDir[lpObj->PathCur]%2 == 0)
					{
						MoveTime = (lpObj->m_MoveSpeed + DelayTime)*(double)1.3;
					}
					else
					{
						MoveTime = lpObj->m_MoveSpeed + DelayTime;
					}
	
					if( (GetTickCount() - lpObj->PathTime) > MoveTime && lpObj->PathCur < 14)
					{
						int nextX = lpObj->PathX[lpObj->PathCur];
						int nextY = lpObj->PathY[lpObj->PathCur];
						BYTE mapnumber = lpObj->MapNumber;
						BYTE attr = MapC[mapnumber].GetAttr(nextX,nextY);
	
						if( lpObj->Type == OBJ_USER &&
							( (attr & 4) == 4 || (attr & 8) == 8) )
						{
							g_Log.Add("[ CHECK POSITION ] MoveMosterProc [%s][%s] Map[%d]-(%d,%d) User(%d,%d) Can not Move Position Attr[%d]",
								lpObj->AccountID,lpObj->Name,lpObj->MapNumber,nextX,nextY,lpObj->X,lpObj->Y,attr);
							for(int n = 0; n < 15;n++)
							{
								lpObj->PathX[n] = 0;
								lpObj->PathY[n] = 0;
								lpObj->PathOri[n] = 0;
							}
	
							lpObj->PathCount = 0;
							lpObj->PathCur = 0;
							lpObj->PathTime = GetTickCount();

							if(lpObj->Type == OBJ_MONSTER || lpObj->Type == OBJ_NPC)
							{
								lpObj->PathStartEnd = 0;
							}
	
							if(lpObj->Type == OBJ_USER)
							{
								gObjSetPosition(lpObj->m_Index,lpObj->X,lpObj->Y);
							}
						}
						else
						{
							lpObj->X = lpObj->PathX[lpObj->PathCur];
							lpObj->Y = lpObj->PathY[lpObj->PathCur];
							lpObj->Dir = lpObj->PathDir[lpObj->PathCur];

							lpObj->PathTime = GetTickCount();
							lpObj->PathCur++;

							if(lpObj->PathCur >= lpObj->PathCount)
							{
								lpObj->PathCur = 0;
								lpObj->PathCount = 0;

								if(lpObj->Type == OBJ_MONSTER || lpObj->Type == OBJ_NPC)
								{
									lpObj->PathStartEnd = 0;
								}
							}
						}
					}
				}
				CreateFrustrum(lpObj->X,lpObj->Y,n);
			}
		}
	}
}

void gObjInit()
{
	try
	{
		if (g_ConfigRead.server.GetServerType() == SERVER_CASTLE)
		{
			MaxViewportMonster = 75;
		}

		gObj = new OBJECTSTRUCT[g_ConfigRead.server.GetObjectMax()];
		m_ObjBill = new BILL_CLASS[g_ConfigRead.server.GetObjectMaxUser()];
		gMultiAttackHackCheck = new CMultiAttackHackCheck[g_ConfigRead.server.GetObjectMax()];

		gDarkSpirit = new CDarkSpirit[g_ConfigRead.server.GetObjectMaxUser()];
		gObjCount = g_ConfigRead.server.GetObjectStartUserIndex();
		gObjMonCount = 0;
		gObjCallMonCount = g_ConfigRead.server.GetObjectMaxMonster();

		gSMMsg = new MessageStateMachine *[g_ConfigRead.server.GetObjectMax()];
		gSMAttackProcMsg = new ExMessageStateMachine *[g_ConfigRead.server.GetObjectMax()];

		for (int n = 0; n < g_ConfigRead.server.GetObjectMax(); n++)
		{
			gSMMsg[n] = new MessageStateMachine[MAX_MONSTER_SEND_MSG];
			gSMAttackProcMsg[n] = new ExMessageStateMachine[MAX_MONSTER_SEND_ATTACK_MSG];
		}

		pTempInventory = new CItem[INVENTORY_SIZE];
		pTempInventoryMap = new BYTE[INVENTORY_MAP_SIZE];

		for (int n = 0; n < g_ConfigRead.server.GetObjectMax(); n++)
		{
			memset(&gObj[n], 0, sizeof(OBJECTSTRUCT));
			gObj[n].Type = (BYTE)OBJ_EMPTY;
			gObj[n].m_socket = INVALID_SOCKET;
			gObj[n].PathTime = GetTickCount();
			gObj[n].m_RecallMon = -1;
			gObj[n].m_lpMagicBack = new CMagicInf[MAGIC_SIZE];
			gObj[n].Magic = new CMagicInf[MAGIC_SIZE];
			gObj[n].VpPlayer = new VIEWPORT_STRUCT[MAX_VIEWPORT];
			gObj[n].VpPlayer2 = new VIEWPORT_PLAYER_STRUCT[MAX_VIEWPORT];
			gObj[n].DurMagicKeyChecker = new TDurMagicKeyChecker;
			gObj[n].sHD = new HITDAMAGE_STRUCT[MAX_ST_HIT_DAMAGE];
			gObj[n].m_Agro = new TMonsterAIAgro;
			gObj[n].MapNumber = MAP_INDEX_RORENCIA;

			if (n >= g_ConfigRead.server.GetObjectStartUserIndex())
			{
				gObj[n].m_PlayerData = new USER_DATA();
				gObj[n].Inventory1 = new CItem[INVENTORY_SIZE];
				gObj[n].Inventory2 = new CItem[INVENTORY_SIZE];
				gObj[n].InventoryMap1 = new BYTE[INVENTORY_MAP_SIZE];
				gObj[n].InventoryMap2 = new BYTE[INVENTORY_MAP_SIZE];
				gObj[n].pMuunInventory1 = new CItem[MUUN_INVENTORY_SIZE];
				gObj[n].pMuunInventory2 = new CItem[MUUN_INVENTORY_SIZE];
				gObj[n].pEventInventory1 = new CItem[EVENT_INVENTORY_SIZE];
				gObj[n].pEventInventory2 = new CItem[EVENT_INVENTORY_SIZE];
				gObj[n].pEventInventoryMap1 = new BYTE[EVENT_INVENTORY_MAP_SIZE];
				gObj[n].pEventInventoryMap2 = new BYTE[EVENT_INVENTORY_MAP_SIZE];
			}

			else
			{
				gObj[n].Inventory1 = pTempInventory;
				gObj[n].Inventory2 = pTempInventory;
				gObj[n].pMuunInventory1 = pTempInventory;
				gObj[n].pMuunInventory2 = pTempInventory;
				gObj[n].pEventInventory1 = pTempInventory;
				gObj[n].pEventInventory2 = pTempInventory;
				gObj[n].InventoryMap1 = pTempInventoryMap;
				gObj[n].InventoryMap2 = pTempInventoryMap;
				gObj[n].pEventInventoryMap1 = pTempInventoryMap;
				gObj[n].pEventInventoryMap2 = pTempInventoryMap;
			}

			if (n >= g_ConfigRead.server.GetObjectStartUserIndex())
			{
				gObj[n].Trade = new CItem[TRADE_BOX_SIZE];
				gObj[n].TradeMap = new BYTE[TRADE_BOX_MAP_SIZE];
				gObj[n].pWarehouse = new CItem[WAREHOUSE_SIZE];
				gObj[n].pWarehouseMap = new BYTE[WAREHOUSE_SIZE];
				gObj[n].pChaosBox = new CItem[CHAOS_BOX_SIZE];
				gObj[n].pChaosBoxMap = new BYTE[CHAOS_BOX_MAP_SIZE];
			}

			gObj[n].WarehouseCount = 0;
			gObj[n].m_bGMSummon = false;
			gObj[n].m_btOpenWarehouse = FALSE;

			if (n >= g_ConfigRead.server.GetObjectStartUserIndex())
			{
				gObj[n].PerSocketContext = new _PER_SOCKET_CONTEXT;
			}

			InitializeCriticalSection(&gObj[n].m_critPShopTrade);
		}
	}

	catch (std::exception e)
	{
		g_Log.MsgBox("%s", e.what());
		return;
	}

	InitFrustrumEx();
}

void gObjEnd()
{
	gObjAllDisconnect();
	gObjAllLogOut();

	for (int n=0; n<g_ConfigRead.server.GetObjectMax(); n++)
	{
		delete [] gSMMsg[n];
		delete [] gSMAttackProcMsg[n];
	}

	delete [] gSMMsg;
	delete [] gSMAttackProcMsg;
	delete [] m_ObjBill;
	delete [] gDarkSpirit;
	delete [] gMultiAttackHackCheck;
	delete [] pTempInventory;
	delete [] pTempInventoryMap;
	delete CharSaveLog;
	
	for (int n = 0; n < g_ConfigRead.server.GetObjectMax(); n++)
	{
		delete[] gObj[n].m_lpMagicBack;
		delete[] gObj[n].Magic;
		delete[] gObj[n].VpPlayer;
		delete[] gObj[n].VpPlayer2;
		delete[] gObj[n].DurMagicKeyChecker;
		delete[] gObj[n].sHD;
		delete[] gObj[n].m_Agro;

		if (gObj[n].m_PlayerData != NULL)
		{
			delete gObj[n].m_PlayerData;
			gObj[n].m_PlayerData = NULL;
		}

		if (n >= g_ConfigRead.server.GetObjectStartUserIndex())
		{
			delete[] gObj[n].pChaosBox;
			delete[] gObj[n].pChaosBoxMap;
			delete[] gObj[n].Inventory1;
			delete[] gObj[n].Inventory2;
			delete[] gObj[n].InventoryMap1;
			delete[] gObj[n].InventoryMap2;
			delete gObj[n].PerSocketContext;
			delete[] gObj[n].Trade;
			delete[] gObj[n].TradeMap;
			delete[] gObj[n].pWarehouse;
			delete[] gObj[n].pWarehouseMap;
			delete[] gObj[n].pMuunInventory1;
			delete[] gObj[n].pMuunInventory2;
			delete[] gObj[n].pEventInventory1;
			delete[] gObj[n].pEventInventory2;
			delete[] gObj[n].pEventInventoryMap1;
			delete[] gObj[n].pEventInventoryMap2;
		}

		DeleteCriticalSection(&gObj[n].m_critPShopTrade);
	}
	delete [] gObj;
}

void gObjClearViewportOfMine(LPOBJ lpObj)
{
	for (int i=0;i<MAX_VIEWPORT;i++)
	{
		lpObj->VpPlayer[i].state =0;
		lpObj->VpPlayer[i].number =-1;
	}

	lpObj->VPCount=0;
}

void gObjClearViewport(LPOBJ lpObj)
{
	for (int i=0;i<MAX_VIEWPORT;i++)
	{
		lpObj->VpPlayer[i].state=0;
		lpObj->VpPlayer[i].number =-1;
		lpObj->VpPlayer2[i].state=0;
		lpObj->VpPlayer2[i].number=-1;
	}

	lpObj->VPCount = 0;
	lpObj->VPCount2 = 0;
}
void gObjCloseSet(int aIndex, int Flag)
{
	if (aIndex < 0 || aIndex > g_ConfigRead.server.GetObjectMax()-1)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->CloseCount > 0)
	{ 
		return;
	}

	if (lpObj->Type != OBJ_USER)
	{
		return;
	}

	if ( lpObj->Connected == PLAYER_PLAYING )
	{
		
		if ( BC_MAP_RANGE(lpObj->MapNumber) )
		{
			g_BloodCastle.SearchUserDropQuestItem(aIndex);
		}

		if (IT_MAP_RANGE(lpObj->MapNumber))
		{
			g_IT_Event.DropRelicsItem(lpObj->MapNumber, lpObj->m_Index);
			g_IT_Event.Leave_ITR(lpObj->m_Index, lpObj->MapNumber);

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

		if (lpObj->MapNumber == MAP_INDEX_DEVILSQUARE_FINAL)
		{
			g_DevilSquareFinal.Leave_DSF(lpObj->m_Index);
		}

		if ( DG_MAP_RANGE(lpObj->MapNumber) )
		{
			g_DoppelGanger.LeaveDoppelganger(lpObj->m_Index);

			if ( g_DoppelGanger.GetDoppelgangerState() == 2 )
			{
				g_DoppelGanger.SendDoppelgangerResult(lpObj, 1);
			}
		}

		if ( (GetTickCount() - lpObj->MySelfDefenseTime) < 60000)
		{
			GSProtocol.GCServerMsgStringSend(Lang.GetText(0,42), lpObj->m_Index, 1);
			GSProtocol.GCCloseMsgSend(lpObj->m_Index, -1);
			return;
		}

		if ( (lpObj->m_IfState.use != 0 ) && ( ( lpObj->m_IfState.type == 1 ) || (lpObj->m_IfState.type == 6) || (lpObj->m_IfState.type == 13) || (lpObj->m_IfState.type == 7)  ) ) 
		{
			gObjSaveChaosBoxItemList(lpObj);
			switch ( lpObj->m_IfState.type )
			{
				case 1:
					GSProtocol.GCServerMsgStringSend(Lang.GetText(0,43), lpObj->m_Index, 1);
					break;

				case 6:
					GSProtocol.GCServerMsgStringSend(Lang.GetText(0,44), lpObj->m_Index, 1);
					break;

				case 7:
					GSProtocol.GCServerMsgStringSend(Lang.GetText(0,59), lpObj->m_Index, 1);
					break;

				case 13:
					GSProtocol.GCServerMsgStringSend(Lang.GetText(0,145), lpObj->m_Index, 1);
					break;
			}
			return;
		}
	}

	lpObj->CloseCount = 6;
	lpObj->CloseType = Flag;
	lpObj->bEnableDelCharacter = 1;
	
}

void gObjCharTradeClear(LPOBJ lpObj)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return;
	}

	if ( lpObj->m_PlayerData->ISBOT == true && lpObj->m_PlayerData->bt_BotType != 1 )
	{
		return;
	}

	memset(lpObj->TradeMap, 0xFF , TRADE_BOX_SIZE);

	for ( int i=0 ; i< TRADE_BOX_SIZE ; i++)
	{
		lpObj->Trade[i].Clear();
	}

	lpObj->TradeMoney = 0;
	lpObj->TradeOk = false;
}

void gObjCharZeroSet(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	memset( lpObj->Name, 0, MAX_ACCOUNT_LEN);

	if ( lpObj->Type == OBJ_USER )
	{
		g_SocketOptionSystem.ClearUserData(lpObj);
		g_MasterLevelSkillTreeSystem.InitPassiveSkillData(lpObj);
	}

	lpObj->m_Option = 3;
	lpObj->m_iScriptMaxLife = 0;
	lpObj->Level = 0;

	memset( lpObj->BackName,0,sizeof(lpObj->BackName));

	lpObj->Life = 0;
	lpObj->MaxLife = 0;
	lpObj->AddLife = 0;
	lpObj->AddMana = 0;
	lpObj->Live = FALSE;
	lpObj->Dir = 0;
	lpObj->X = 0;
	lpObj->Y = 0;
	lpObj->TX = 0;
	lpObj->TY = 0;
	lpObj->MTX = 0;
	lpObj->MTY = 0;
	lpObj->MapNumber = MAP_INDEX_RORENCIA;
	lpObj->RegenTime = 0;
	lpObj->RegenOk = 0;
	lpObj->lpAttackObj = 0;
	lpObj->PartyTargetUser = -1;
	lpObj->CloseCount = -1;
	lpObj->CloseType = -1;
	lpObj->m_IfState.state = -1;
	lpObj->m_IfState.type = -1;
	lpObj->m_IfState.use = 0;
	lpObj->m_InterfaceTime = 0;
	lpObj->m_ActionNumber = 0;
	lpObj->m_State = 0;
	lpObj->m_Rest = 0;
	lpObj->m_ViewState = 0;
	lpObj->DieRegen = 0;
	lpObj->iMaxShield = 0;
	lpObj->iShield = 0;
	lpObj->iAddShield = 0;
	lpObj->iFillShield = 0;
	lpObj->iFillShieldMax = 0;
	lpObj->iFillShieldCount = 0;
	lpObj->dwShieldAutoRefillTimer = 0;
	lpObj->WinDuels = 0;
	lpObj->LoseDuels = 0;
	lpObj->m_SkillInfo.Clear();
	lpObj->m_Attribute = 0;
	lpObj->m_iMonsterBattleDelay = 0;
	lpObj->m_cKalimaGateExist = FALSE;
	lpObj->m_iKalimaGateIndex = -1;
	lpObj->m_cKalimaGateEnterCount = 0;
	lpObj->TradeOk = false;
	lpObj->sHDCount = 0;
	lpObj->PathCount = 0;
	lpObj->PathCur = 0;
	lpObj->PathStartEnd = 0;
	lpObj->VPCount = 0;
	lpObj->VPCount2 = 0;
	lpObj->m_ShopTime = 0;
	lpObj->m_ManaFillCount = 0;
	lpObj->FillLife = 0;
	lpObj->iObjectSecTimer = 0;
	lpObj->m_FriendServerOnline = FALSE;
	lpObj->AddStrength = 0;
	lpObj->AddDexterity = 0;
	lpObj->AddVitality = 0;
	lpObj->AddEnergy = 0;

	lpObj->m_CurseSpell = 0;
	memset( lpObj->m_AddResistance, 0, sizeof(lpObj->m_AddResistance));
	lpObj->DamageMinus = 0;
	lpObj->DamageReflect = 0;
	lpObj->MonsterDieGetMoney = 0;
	lpObj->MonsterDieGetLife = 0;
	lpObj->MonsterDieGetMana = 0;
	lpObj->m_CriticalDamage = 0;
	lpObj->m_ExcelentDamage = 0;
	lpObj->UseMagicNumber = 0;
	lpObj->UseMagicTime = 0;
	lpObj->UseMagicCount = 0;
	lpObj->OSAttackSerial = -1;
	lpObj->SASCount = 0;
	lpObj->SkillAttackTime = GetTickCount();
	lpObj->m_LastAttackTick = GetTickCount();
	lpObj->m_LastAttackNumber = 0;
	lpObj->m_AttackSpeed = 0;
	lpObj->m_MagicSpeed = 0;
	lpObj->m_TotalAttackTime = GetTickCount();
	lpObj->m_TotalAttackCount = 0;
	lpObj->CheckSumTime = GetTickCount() + 10000;
	lpObj->CheckSumTableNum = -1;
	if(g_ConfigRead.antihack.MainCRC != 0)
	{
		lpObj->CrcCheckTime = GetTickCount();
	}
	else
	{
		lpObj->CrcCheckTime = 0 ;
	}
	lpObj->m_SumLastAttackTime = 0;
	lpObj->m_DetectCount = 0;
	lpObj->m_DetectSpeedHackTime = 0.00;
	lpObj->m_SpeedHackPenalty = 0;
	lpObj->m_CurseDamageMin = 0;
	lpObj->m_CurseDamageMax = 0;
	lpObj->m_DetectedHackKickCount = 0;
	lpObj->IsInBattleGround = false;
	lpObj->m_dwPKTimer = GetTickCount();
	lpObj->CheckTickCount = 0;
	lpObj->m_AttackSpeedHackDetectedCount = 0;

	for (int n =0;n<MAX_VIEWPORT ;n++)
	{
		lpObj->VpPlayer[n].state = 0;
		lpObj->VpPlayer2[n].state = 0;
		lpObj->VpPlayer[n].number = -1;
		lpObj->VpPlayer2[n].number = -1;
	}

	lpObj->PartyNumber = -1;
	lpObj->PartyTargetUser = -1;
	lpObj->Authority = 0;
	lpObj->AuthorityCode = 0;
	lpObj->Penalty = 0;
	lpObj->m_RecallMon = -1;
	lpObj->m_Change = -1;

	lpObj->LastAttackerID = -1;
	lpObj->m_cChaosCastleIndex= -1;
	lpObj->m_cChaosCastleSubIndex = -1;
	lpObj->m_iChaosCastleBlowTime = 0;
	lpObj->m_cKillUserCount = 0;
	lpObj->m_cKillMonsterCount = 0;
	memset( lpObj->CharSet, 0, sizeof(lpObj->CharSet));

	for (int i =0; i<MAX_SELF_DEFENSE;i++)
	{
		lpObj->SelfDefense[i] = -1;
		lpObj->SelfDefenseTime[i] = 0;
	}

	lpObj->MySelfDefenseTime = 0;
	lpObj->m_MagicDefense = 0;
	lpObj->LoadWareHouseInfo = false;
	lpObj->m_bAttackerKilled = false;
	lpObj->m_MoveOtherServer = 0;
	lpObj->m_BossGoldDerconMapNumber = -1;
	lpObj->TargetNumber =	0;
	lpObj->TargetNpcNumber = -1;
	lpObj->WarehouseMoney = 0;
	lpObj->WarehouseLock = -1;
	lpObj->WarehouseCount = 0;
	lpObj->WarehouseSaveLock = 0;
	lpObj->WarehousePW = 0;
	lpObj->WarehouseID = 0;
	lpObj->WarehouseTick = 0;
	lpObj->WarehouseUnfailLock = 0;
	lpObj->m_bDevilSquareAuth = false;
	lpObj->m_TimeCount = 0;
	lpObj->EventChipCount = 0;
	lpObj->MutoNumber = 0;
	lpObj->HaveWeaponInHand = false;

	lpObj->iStoneCount = 0;
	lpObj->m_iDuelUserReserved = -1;
	lpObj->m_iDuelUserRequested = -1;
	lpObj->m_iDuelUser = -1;
	lpObj->m_btDuelScore = 0;
	lpObj->m_iDuelTickCount = 0;
	lpObj->m_MaxLifePower = 0;
	lpObj->AddBP = 0;
	lpObj->m_CheckLifeTime = 0;
	gPacketCheckSum.ClearCheckSum(lpObj->m_Index);
	lpObj->m_LastTeleportTime = 0;
	lpObj->m_ClientHackLogCount = 0;
	lpObj->UseEventServer = FALSE;
	lpObj->SaveX = 0;
	lpObj->SaveY = 0;
	lpObj->SaveMapNumber = -1;
	lpObj->DieX = 0;
	lpObj->DieY = 0;
	lpObj->DieMapNumber = -1;
	lpObj->m_wMineCount = 0;
	lpObj->m_btMiningLevel = 0;
	lpObj->ChatBlockTime = 0;
	lpObj->m_PostCMDLastUseTick = GetTickCount()-10000; // so you can post as soon as you enter game instead waiting 10 sec
	gObjClearBuffEffect(lpObj, CLEAR_TYPE_LOGOUT);

	if ( lpObj->Type == OBJ_USER && (lpObj->m_PlayerData->ISBOT == false && lpObj->m_PlayerData->bt_BotType != 1) )
	{
		memset(lpObj->pWarehouseMap, (BYTE)-1, WAREHOUSE_SIZE);
		memset(lpObj->TradeMap, (BYTE)-1, TRADE_BOX_SIZE);
		memset(lpObj->pChaosBoxMap, (BYTE)-1, CHAOS_BOX_MAP_SIZE);
	}

	for (int i=0;i<MAX_MAGIC ; i++)
	{
		lpObj->Magic[i].Clear();
	}

	lpObj->MagicCount = 0;

	for (int i=0;i<MAX_MONSTER_SEND_MSG;i++)
	{
		gSMMsg[lpObj->m_Index][i].MsgCode = -1;
	}

	lpObj->m_bIsInMonsterHerd = false;
	lpObj->m_bIsMonsterAttackFirst = true;
	lpObj->m_lpMonsterHerd = NULL;

	if ( (lpObj->m_Index- g_ConfigRead.server.GetObjectStartUserIndex()) >= 0 )
	{
		for (int i=0;i<MAX_MONSTER_SEND_ATTACK_MSG;i++)
		{
			gSMAttackProcMsg[lpObj->m_Index- g_ConfigRead.server.GetObjectStartUserIndex()][i].MsgCode = -1;
		}
	}

	lpObj->Leadership = 0;
	lpObj->AddLeadership = 0;
	lpObj->ChatLimitTime = 0;
	lpObj->ChatLimitTimeSec = 0;
	lpObj->SkillLongSpearChange = false;

	lpObj->bEnableDelCharacter = 1;

	if ( lpObj->Type == OBJ_USER && (lpObj->m_PlayerData->ISBOT == false && lpObj->m_PlayerData->bt_BotType != 1) )
	{
		for (int i=0;i<WAREHOUSE_SIZE;i++)
		{
			lpObj->pWarehouse[i].Clear();
		}

		for (int i=0;i<CHAOS_BOX_SIZE;i++)
		{
			lpObj->pChaosBox[i].Clear();
		}
	}

	::gMultiAttackHackCheck[lpObj->m_Index].Init();
	lpObj->pTransaction = 0;
	::gObjMonsterHitDamageInit(lpObj);
	::gObjSetInventory1Pointer(lpObj);
	::gObjSetEventInventory1Pointer(lpObj);
	::gObjSetMuunInventory1Pointer(lpObj);

	if ( lpObj->Type == OBJ_USER && (lpObj->m_PlayerData->ISBOT == false && lpObj->m_PlayerData->bt_BotType != 1) )
	{
		memset(lpObj->pInventoryMap, (BYTE)-1, INVENTORY_MAP_SIZE);
	}

	gObjCharTradeClear(lpObj);

	if ( lpObj->Type == OBJ_USER && (lpObj->m_PlayerData->ISBOT == false && lpObj->m_PlayerData->bt_BotType != 1) )
	{
		for (int i=0;i<WAREHOUSE_SIZE;i++)
		{
			lpObj->pWarehouse[i].Clear();
		}

		for (int i=0;i<INVENTORY_SIZE;i++)
		{
			lpObj->Inventory1[i].Clear();
		}

		for (int i=0;i<INVENTORY_SIZE;i++)
		{
			lpObj->Inventory2[i].Clear();
		}

		for (int i=0;i<TRADE_BOX_SIZE;i++)
		{
			lpObj->Trade[i].Clear();
		}

		for (int i = 0; i < MUUN_INVENTORY_SIZE; i++)
		{
			lpObj->pMuunInventory1[i].Clear();
			lpObj->pMuunInventory2[i].Clear();
		}

		for (int i = 0; i < 2; i++)
		{
			lpObj->m_MuunEffectList[i].Clear();
		}

		lpObj->bMuunInventoryLoad = false;
		lpObj->dwCheckMuunItemTime = 0;
		memset(lpObj->pEventInventoryMap1, -1, EVENT_INVENTORY_MAP_SIZE);
		memset(lpObj->pEventInventoryMap2, -1, EVENT_INVENTORY_MAP_SIZE);

		for (int i = 0; i < EVENT_INVENTORY_SIZE; i++)
		{
			lpObj->pEventInventory1[i].Clear();
			lpObj->pEventInventory2[i].Clear();
		}

		lpObj->EventInventoryLoad = false;

		CMuRummyInfo * pCMuRummyInfo = lpObj->m_PlayerData->m_pCMuRummyInfo;

		if (!pCMuRummyInfo)
		{
			g_Log.Add("[MuRummy][Error] pCMuRummyInfo is NULL [%s][%s] [%s, %d]", lpObj->AccountID, lpObj->Name, __FILE__, __LINE__);
		}

		else
		{
			pCMuRummyInfo->Clear();
		}

		lpObj->m_wMuunItem = -1;
		lpObj->m_wMuunSubItem = -1;
		lpObj->m_wMuunRideItem = -1;
		lpObj->m_wInvenPet = -1;
		lpObj->m_btInvenPetPos = 0;
		lpObj->bSubEquip = 0;
		lpObj->m_nDSFIndex = -1;
		lpObj->m_PlayerData->Init(false);
	}

	lpObj->bIsChaosMixCompleted = false;
	lpObj->m_bMapSvrMoveQuit = false;
	lpObj->m_iCsNpcExistVal = 0;
	lpObj->m_btCsNpcType = 0;
	lpObj->m_btCsGateOpen = 0;
	lpObj->m_iCsGateLeverLinkIndex = -1;
	lpObj->m_btCsNpcDfLevel = 0;
	lpObj->m_btCsNpcRgLevel = 0;
	lpObj->m_btCsJoinSide = 0;
	lpObj->m_bCsGuildInvolved = false;
	lpObj->m_bIsCastleNPCUpgradeCompleted = 0;
	lpObj->m_btWeaponState = 0;
	lpObj->m_bCsGuildInvolved = false;
	lpObj->m_iAccumulatedDamage = 0;
	lpObj->m_iAccumulatedCrownAccessTime = 0;
	lpObj->m_MonsterSkillElementInfo.Reset();
	lpObj->m_iCrywolfMVPScore = 0;
	lpObj->m_iSkillDistanceErrorCount = 0;
	lpObj->m_dwSkillDistanceErrorTick = 0;
	memset(lpObj->MarryName, 0x00, sizeof(lpObj->MarryName));
	lpObj->Married = 0;
	lpObj->MarryRequested = 0;
	lpObj->MarryRequestIndex = -1;
	lpObj->MarryRequestTime = 0;
	lpObj->BufferIndex = -1;
	lpObj->BuffId = -1;
	lpObj->BuffPlayerIndex = -1;
	
	lpObj->m_ActionTime = 0;
	lpObj->m_ActionCount = 0;
	lpObj->m_ChatFloodCount = 0;
	lpObj->m_ChatFloodTime = 0;
	lpObj->m_bOff = false;
	lpObj->m_bOffLevel = false;
	lpObj->m_iPentagramAttributePattern = 0;
	lpObj->m_iPentagramDefense = 0;
	lpObj->m_iPentagramDamageMax = 0;
	lpObj->m_iPentagramDamageMin = 0;
	lpObj->m_iPentagramDamageOrigin = 1;

	lpObj->m_bBlind = false;

	lpObj->m_iCreatedActivationTime = 0;
	lpObj->m_btLifeStoneCount = 0;
	lpObj->m_btNpcType = 0;
	lpObj->m_bGMSummon = false;
	lpObj->m_Disappear_Monster = 0;
	lpObj->m_Disappear_NewYearMonster = 0;
	lpObj->m_SummonCharDBNum = 0;
	lpObj->m_bIsHelpMon = false;

	lpObj->m_nITR_Index = -1;
	lpObj->m_nITR_RelicsTick = 0;
	lpObj->m_bITR_GettingRelics = 0;
	lpObj->m_bITR_RegisteringRelics = 0;
	lpObj->m_wITR_NpcType = 0;
	lpObj->m_byITR_StoneState = 99;
	lpObj->m_dwDCTimer = 0;
	lpObj->m_dwOffLevelTime = 0;
	memset(&lpObj->m_BotBuffs,0,sizeof(lpObj->m_BotBuffs));
	lpObj->m_nPhaseLordSilvester = 0;
	lpObj->m_btOpenWarehouse = FALSE;
	lpObj->m_Whispering.iWhisperSent = false;
	lpObj->m_Whispering.wResponseCount = 0;
	for (int k = 0; k < 3; k++)
		lpObj->m_nRecallMonIndex[k] = -1;

	gObjClearViewport(lpObj);
}

int gObjGetSocket(SOCKET socket)
{
	for (int n= g_ConfigRead.server.GetObjectStartUserIndex();n<g_ConfigRead.server.GetObjectMax();n++)
	{
		if (gObj[n].Connected != PLAYER_EMPTY)
		{
			if (gObj[n].m_socket == socket)
			{
				return n;
			}
		}
	}
	return -1;
}

int gObjGetHWIDUseCount(LPTSTR HWID)
{
	int count = 0;

	for (int n = g_ConfigRead.server.GetObjectStartUserIndex(); n < g_ConfigRead.server.GetObjectMax(); n++)
	{
		if (gObj[n].Connected != PLAYER_EMPTY)
		{
			if (strcmp(gObj[n].m_PlayerData->HWID, HWID) == 0)
			{
				if (gObj[n].m_bOff == false)
				{
					count++;
				}
			}
		}
	}

	return count;
}

void gObjSetTradeOption( int aIndex, int option)
{
	if ( gObjIsConnected(aIndex) == TRUE )
	{
		if ( option == 0 )
		{
			gObj[aIndex].m_Option = 0;
			GSProtocol.GCServerMsgStringSend(Lang.GetText(0,33), aIndex, 1);
		}
		else
		{
			gObj[aIndex].m_Option |= 1;
			GSProtocol.GCServerMsgStringSend(Lang.GetText(0,32), aIndex, 1);
		}
	}
}

bool IsDuelEnable(int aIndex)
{
	if ( ObjectMaxRange(aIndex ) == FALSE)
	{
		return false;
	}

	if ( gObjIsConnected(aIndex) == TRUE )
	{
		if ( (gObj[aIndex].m_Option&2) == 2 )
		{
			return true;
		}
	}

	return false;
}

bool IsOnDuel(int aIndex1, int aIndex2)
{
	if( !CHECK_LIMIT(aIndex1, g_ConfigRead.server.GetObjectMax())  || !CHECK_LIMIT(aIndex2, g_ConfigRead.server.GetObjectMax())  )	return false;
	if( gObj[aIndex1].Type != OBJ_USER || gObj[aIndex2].Type != OBJ_USER ) return false;
	if( gObj[aIndex1].m_iDuelUser == aIndex2 && gObj[aIndex2].m_iDuelUser == aIndex1 ) return true;
	return false;
}

void gObjSetDuelOption(int aIndex, int option)
{
	if ( gObjIsConnected(aIndex) == TRUE )
	{
		if ( option == 0 )
		{
			gObj[aIndex].m_Option = 0;
		}
		else
		{
			gObj[aIndex].m_Option |= 2;
		}
	}
}

void DbItemSetInByte(LPOBJ lpObj, struct SDHP_DBCHAR_INFORESULT* lpMsg, bool* bAllItemExist)
{
	int itype;
	int _type;
	CItem item;
	BYTE OptionData;
	WORD hiWord;
	WORD loWord;
	int check;
	bool bIsItemExist = true;

	for(int n = 0; n < INVENTORY_SIZE;n++)
	{
		bIsItemExist = true;
		lpObj->pInventory[n].Clear();
		itype = lpMsg->dbInventory[n*MAX_DBITEM_INFO];

		if(lpMsg->dbInventory[n*MAX_DBITEM_INFO] == 0xFF
			&& (lpMsg->dbInventory[n*MAX_DBITEM_INFO+DBI_NOPTION_DATA] & 0x80) == 0x80
			&& (lpMsg->dbInventory[n*MAX_DBITEM_INFO+DBI_OPTION380_DATA] & 0xF0) == 0xF0)
		{
			continue;
		}
		

		itype |= (lpMsg->dbInventory[n*MAX_DBITEM_INFO+DBI_OPTION380_DATA] & 0xF0) * 32;
		itype |= (lpMsg->dbInventory[n*MAX_DBITEM_INFO+DBI_NOPTION_DATA] & 0x80) * 2;
		_type = itype;

		if(lpMsg->dbInventory[n*MAX_DBITEM_INFO] == 0xFF
			&& (lpMsg->dbInventory[n*MAX_DBITEM_INFO+DBI_NOPTION_DATA] & 0x80) == 0x80
			&& (lpMsg->dbInventory[n*MAX_DBITEM_INFO+DBI_OPTION380_DATA] & 0xF0) == 0xF0)
		{
			itype = -1;
		}

		if(_type == -1)
		{
			continue;
		}

		if(IsItem(_type) == false)
		{
			bIsItemExist = false;

			if(bAllItemExist != 0)
			{
				*bAllItemExist = 0;
			}
		}

		if(bIsItemExist == false)
		{
			lpObj->pInventory[n].m_bItemExist = 0;
			
			g_Log.Add("error-L2 : Unknown Item found [%s][%s] (Type:%d, LEV:%d, DUR:%d OP1:%d, OP2:%d, OP3:%d, NEWOP:%d, SET:%d)",
				lpObj->AccountID,lpObj->Name,lpObj->pInventory[n].m_Type,lpObj->pInventory[n].m_Level,
				lpObj->pInventory[n].m_Durability,lpObj->pInventory[n].m_Option1,
				lpObj->pInventory[n].m_Option2,lpObj->pInventory[n].m_Option3,
				lpObj->pInventory[n].m_NewOption,lpObj->pInventory[n].m_SetOption);

			lpObj->pInventory[n].Clear();
			continue;
		}

		if(_type == ITEMGET(14,17) || _type == ITEMGET(14,18))
		{
			if(lpMsg->dbInventory[n*MAX_DBITEM_INFO+DBI_DUR]==0)
			{
				itype = -1;
			}
		}

		if(_type == ITEMGET(13,49) || _type == ITEMGET(13,50))
		{
			if(lpMsg->dbInventory[n*MAX_DBITEM_INFO+DBI_DUR]==0)
			{
				itype = -1;
			}
		}

		if(_type == ITEMGET(14,19))
		{
			if(lpMsg->dbInventory[n*MAX_DBITEM_INFO+DBI_DUR]==(BYTE)-1)
			{
				itype = -1;
			}
		}

		if(itype != -1)
		{
			item.m_Level = DBI_GET_LEVEL(lpMsg->dbInventory[n*MAX_DBITEM_INFO+DBI_OPTION_DATA]);

			if(_type != ITEMGET(14,11))
			{
				if(item.m_Level > 15)
				{
					item.m_Level = 0;
				}
			}
			
			OptionData = lpMsg->dbInventory[n*MAX_DBITEM_INFO+DBI_OPTION_DATA];
			item.m_Option1 = DBI_GET_SKILL(OptionData);
			item.m_Option2 = DBI_GET_LUCK(OptionData);
			item.m_Option3 = DBI_GET_OPTION(OptionData);
			item.m_Option3 |= DBI_GET_OPTION16(lpMsg->dbInventory[n*MAX_DBITEM_INFO+DBI_NOPTION_DATA]) >> 4;

			item.m_Durability = lpMsg->dbInventory[n*MAX_DBITEM_INFO+DBI_DUR];
			item.m_JewelOfHarmonyOption = lpMsg->dbInventory[n*MAX_DBITEM_INFO+DBI_JOH_DATA];
			item.m_ItemOptionEx = DBI_GET_380OPTION(lpMsg->dbInventory[n*MAX_DBITEM_INFO+DBI_OPTION380_DATA]);

			if(item.m_ItemOptionEx)
			{
				item.m_Type = itype;

				if(g_kItemSystemFor380.Is380Item(&item)==false)
				{
					item.m_ItemOptionEx = 0;
					g_Log.Add("[380Item][%s][%s] Invalid 380 Item Option in Inventory pos[%d]",
						lpObj->AccountID,lpObj->Name,n);
				}
			}

			BYTE SocketOption[5];
			BYTE SocketOptionIndex = 0xFF;
			memset(&SocketOption, 0xFF, 5);

			if (g_SocketOptionSystem.IsEnableSocketItem(_type) == 1 || g_PentagramSystem.IsPentagramItem(_type) || g_PentagramSystem.IsPentagramJewel(_type))
			{
				for (int i = 0; i < 5; i++)
				{
					SocketOption[i] = lpMsg->dbInventory[((n*MAX_DBITEM_INFO) + DBI_SOCKET_1) + i];
				}

				SocketOptionIndex = lpMsg->dbInventory[n*MAX_DBITEM_INFO + DBI_JOH_DATA];
			}

			else
			{
				for (int i = 0; i < 5; i++)
				{
					SocketOption[i] = 0xFF;
				}

				SocketOptionIndex = 0;
			}

			item.m_PeriodItemOption = (lpMsg->dbInventory[n*MAX_DBITEM_INFO + DBI_OPTION380_DATA] & 6) >> 1;

			item.Convert(itype,item.m_Option1,item.m_Option2,item.m_Option3,DBI_GET_NOPTION(lpMsg->dbInventory[n*MAX_DBITEM_INFO+DBI_NOPTION_DATA]),lpMsg->dbInventory[n*MAX_DBITEM_INFO+DBI_SOPTION_DATA],item.m_ItemOptionEx, SocketOption, SocketOptionIndex, item.m_PeriodItemOption, CURRENT_DB_VERSION);

			if(_type == ITEMGET(14,7))
			{

			}

			else
			{
				if (_type >= ITEMGET(14, 0) && _type <= ITEMGET(14, 8)
					|| _type >= ITEMGET(14, 38) && _type <= ITEMGET(14, 40))
				{
					if (item.m_Durability == 0.0f)
					{
						item.m_Durability = 1.0f;
					}
				}
				//Begin CashShop Items
				else if (_type == ITEMGET(14, 70) || _type == ITEMGET(14, 71))//cashshop
				{
					if (item.m_Durability == 0.0f)
					{
						item.m_Durability = 1.0f;
					}
				}

				if (_type == ITEMGET(14, 78) || _type == ITEMGET(14, 79)
					|| _type == ITEMGET(14, 80) || _type == ITEMGET(14, 81)
					|| _type == ITEMGET(14, 82))
				{
					if (item.m_Durability == 0.0f)
					{
						item.m_Durability = 1.0f;
					}
				}

				if (_type == ITEMGET(14, 63) || _type == ITEMGET(14, 64))
				{
					if (item.m_Durability == 0.0f)
					{
						item.m_Durability = 1.0f;
					}
				}
				//End CashShop Fucntions
				else if (_type >= ITEMGET(14, 46) && _type <= ITEMGET(14, 50))
				{
					if (item.m_Durability == 0.0f)
					{
						item.m_Durability = 1.0f;
					}
				}
				else if (_type >= ITEMGET(14, 35) && _type <= ITEMGET(14, 37))
				{
					if (item.m_Durability == 0.0f)
					{
						item.m_Durability = 1.0f;
					}
				}

				else if (_type == ITEMGET(13, 51))
				{
					if (item.m_Durability == 0.0f)
					{
						item.m_Durability = 1.0f;
					}
				}

				else if (_type != ITEMGET(13, 10) && _type != ITEMGET(14, 29) && _type != ITEMGET(14, 21))
				{
					if (item.m_Level == 3)
					{
						if (item.m_Durability > item.m_BaseDurability && bIsItemExist == 1)
						{
							item.m_Durability = item.m_BaseDurability;
						}
					}
				}
			}

			int iMaxOverlap = IsOverlapItem(item.m_Type);

			if (iMaxOverlap != 0)
			{
				if (item.m_Durability > iMaxOverlap)
				{
					item.m_Durability = iMaxOverlap;
				}
			}

			lpObj->pInventory[n].m_Option1 = item.m_Option1;
			lpObj->pInventory[n].m_Option2 = item.m_Option2;
			lpObj->pInventory[n].m_Option3 = item.m_Option3;
			lpObj->pInventory[n].m_JewelOfHarmonyOption = item.m_JewelOfHarmonyOption;
			lpObj->pInventory[n].m_ItemOptionEx = item.m_ItemOptionEx;

			DWORD hidword = MAKE_NUMBERDW(MAKE_NUMBERW(lpMsg->dbInventory[n*MAX_DBITEM_INFO+DBI_SERIAL1], lpMsg->dbInventory[n*MAX_DBITEM_INFO+DBI_SERIAL2]), MAKE_NUMBERW(lpMsg->dbInventory[n*MAX_DBITEM_INFO+DBI_SERIAL3], lpMsg->dbInventory[n*MAX_DBITEM_INFO+DBI_SERIAL4]));
			DWORD lodword = MAKE_NUMBERDW(MAKE_NUMBERW(lpMsg->dbInventory[n*MAX_DBITEM_INFO+DBI_SERIAL5], lpMsg->dbInventory[n*MAX_DBITEM_INFO+DBI_SERIAL6]), MAKE_NUMBERW(lpMsg->dbInventory[n*MAX_DBITEM_INFO+DBI_SERIAL7], lpMsg->dbInventory[n*MAX_DBITEM_INFO+DBI_SERIAL8]));
			item.m_Number = MAKEQWORD(hidword, lodword);

			gObjInventoryInsertItemPos(lpObj->m_Index,item,n,0);

			if(item.m_Type == ITEMGET(14,64))
			{
				gObjInventoryDeleteItem(lpObj->m_Index, n);
			}
			if(item.m_Type == ITEMGET(13,107) && item.m_Durability <= 0.0f)
			{
				gObjInventoryDeleteItem(lpObj->m_Index, n);
			}
		}
	}
	gObjRequestPetItemInfo(lpObj->m_Index, 0);
}

void gObjSetBP(int aIndex)
{
	if ( gObj[aIndex].Type != OBJ_USER )
	{
		return;
	}

	int Strength = gObj[aIndex].m_PlayerData->Strength + gObj[aIndex].AddStrength;
	int Dexterity = gObj[aIndex].m_PlayerData->Dexterity + gObj[aIndex].AddDexterity;
	int Vitality = gObj[aIndex].m_PlayerData->Vitality + gObj[aIndex].AddVitality;
	int Energy = gObj[aIndex].m_PlayerData->Energy + gObj[aIndex].AddEnergy;
	int Leadership = gObj[aIndex].Leadership + gObj[aIndex].AddLeadership;

	switch ( gObj[aIndex].Class )
	{
		case CLASS_WIZARD:
			gObj[aIndex].MaxBP = (Strength * 0.2) + (Dexterity * 0.4) + (Vitality * 0.3) + (Energy * 0.2);
			break;

		case CLASS_KNIGHT:
			gObj[aIndex].MaxBP = (Strength * 0.15) + (Dexterity * 0.2) + (Vitality * 0.3) + (Energy * 1.0);
			break;

		case CLASS_ELF:
			gObj[aIndex].MaxBP = (Strength * 0.3) + (Dexterity * 0.2) + (Vitality * 0.3) + (Energy * 0.2);
			break;

		case CLASS_MAGUMSA:
			gObj[aIndex].MaxBP = (Strength * 0.2) + (Dexterity * 0.25) + (Vitality * 0.3) + (Energy * 0.15);
			break;

		case CLASS_DARKLORD:
			gObj[aIndex].MaxBP = (Strength * 0.3) + (Dexterity * 0.2) + (Vitality * 0.1) + (Energy * 0.15) + (Leadership * 0.3);
			break;
		
		case CLASS_SUMMONER:
			gObj[aIndex].MaxBP = (Strength * 0.2) + (Dexterity * 0.25) + (Vitality * 0.3) + (Energy * 0.15);
			break;

		case CLASS_RAGEFIGHTER:
			gObj[aIndex].MaxBP = (Strength * 0.15) + (Dexterity * 0.2) + (Vitality * 0.3) + (Energy * 1.0);
			break;

		case CLASS_GROWLANCER:
			gObj[aIndex].MaxBP = (Strength * 0.15) + (Dexterity * 0.2) + (Vitality * 0.3) + (Energy * 1.0);
			break;
	}
}

bool gObjSetCharacter(LPBYTE lpdata, int aIndex)
{
	WORD itype;
	int n;
	char szCharName[MAX_ACCOUNT_LEN + 1];
	SDHP_DBCHAR_INFORESULT * lpMsg = (SDHP_DBCHAR_INFORESULT *)lpdata;

	// Check in position
	if (ObjectMaxRange(aIndex) == FALSE)
	{
		g_Log.Add("error : %s %d", __FILE__, __LINE__);
		return FALSE;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Connected < PLAYER_LOGGED)
	{
		return FALSE;
	}

	if (lpObj->Connected == PLAYER_PLAYING)
	{
		g_Log.AddC(TColor::Red, "error-L1: (%s/%s)Received reconnect message while playing game. Forced to disconnect", lpObj->AccountID, lpObj->Name);
		return FALSE;
	}

	if (lpMsg->Class == 0 || lpMsg->Class == 1 || lpMsg->Class == 2 || lpMsg->Class == 16 || lpMsg->Class == 17 || lpMsg->Class == 18 || lpMsg->Class == 32 || lpMsg->Class == 33 || lpMsg->Class == 34 || lpMsg->Class == 48 || lpMsg->Class == 50 || lpMsg->Class == 64 || lpMsg->Class == 66 || lpMsg->Class == 80 || lpMsg->Class == 81 || lpMsg->Class == 82 || lpMsg->Class == 96 || lpMsg->Class == 98 || lpMsg->Class == 112 || lpMsg->Class == 114)
	{

	}
	else
	{
		return FALSE;
	}

	gObjCharZeroSet(aIndex);

	if (g_MixSystem.ChaosBoxInit(lpObj) == FALSE)
	{
		g_Log.Add("error-L1 : ChaosBox Init error %s %d", __FILE__, __LINE__);
	}

	if (g_PentagramMixSystem.PentagramMixBoxInit(lpObj) == FALSE)
	{
		g_Log.Add("error-L1 : PentagramMixBox Init error %s %d", __FILE__, __LINE__);
	}

	lpObj->m_PosNum = 0;
	lpObj->Level = lpMsg->Level;
	lpObj->m_PlayerData->LevelUpPoint = lpMsg->LevelUpPoint;
	lpObj->m_PlayerData->DbClass = lpMsg->Class;
	lpObj->Class = lpMsg->Class >> 4;
	lpObj->m_PlayerData->ChangeUP = lpMsg->Class & 0x07;	// Set Second Type of Character

	szCharName[MAX_ACCOUNT_LEN] = 0;
	memcpy(szCharName, lpMsg->Name, MAX_ACCOUNT_LEN);
	strcpy(lpObj->Name, szCharName);

	if (g_ConfigRead.server.GetServerType() == SERVER_BATTLECORE)
	{
		std::string tmpCmpName;
		tmpCmpName.insert(0, lpObj->Name, MAX_ACCOUNT_LEN);

		strncpy(lpObj->m_PlayerData->m_RealNameOfUBF, lpObj->m_PlayerData->m_NameConvertOfUBF.ConvertNameToRealName(tmpCmpName.c_str()), MAX_ACCOUNT_LEN);
		lpObj->m_PlayerData->m_nServerCodeOfHomeWorld = lpObj->m_PlayerData->m_NameConvertOfUBF.FindServerCodeAtCharacterName(tmpCmpName.c_str());
	}

	if (lpObj->Level < 1)
	{
		g_Log.AddC(TColor::Red, "error : %s Level Zero", lpMsg->Name);
		return FALSE;
	}

	lpObj->X = lpMsg->MapX;
	lpObj->Y = lpMsg->MapY;
	lpObj->MapNumber = lpMsg->MapNumber;
	lpObj->StartX = lpObj->X;
	lpObj->StartY = lpObj->Y;

	if (MAX_MAP_RANGE(lpObj->MapNumber) == FALSE)
	{
		g_Log.Add("error : Map Number max over. %s %d", __FILE__, __LINE__);
		lpObj->MapNumber = MAP_INDEX_RORENCIA;
		MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y);
	}

	if (lpObj->Level < 6)
	{
		if (lpObj->Class == 2)
		{
			lpObj->MapNumber = MAP_INDEX_NORIA;
			MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y);
		}
		else if (lpObj->Class == 5)
		{
			short sX;
			short sY;
			short sLVL;
			unsigned char btMAPNUM = lpObj->MapNumber;
			unsigned char btDIR;

			int iMapNumber = gGateC.GetGate(267, (short &)sX, (short &)sY, (BYTE &)btMAPNUM, (BYTE &)btDIR, (short &)sLVL);

			if (iMapNumber >= 0)
			{
				lpObj->MapNumber = btMAPNUM;
				lpObj->X = sX;
				lpObj->Y = sY;
			}
		}
		else
		{
			lpObj->MapNumber = MAP_INDEX_RORENCIA;
			MapC[lpObj->MapNumber].GetLevelPos(lpObj->Level, lpObj->X, lpObj->Y);
		}
	}

	if (DS_MAP_RANGE(lpObj->MapNumber) != FALSE)
	{
		lpObj->MapNumber = MAP_INDEX_RORENCIA;
		MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y);
	}

	if (BC_MAP_RANGE(lpObj->MapNumber) != FALSE)
	{
		lpObj->MapNumber = MAP_INDEX_DEVIAS;
		MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y);
		lpObj->m_cBloodCastleIndex = -1;
		lpObj->m_cBloodCastleSubIndex = -1;
		lpObj->m_iBloodCastleEXP = 0;
		lpObj->m_bBloodCastleComplete = 0;
	}

	else
	{
		lpObj->m_cBloodCastleIndex = -1;
		lpObj->m_cBloodCastleSubIndex = -1;
		lpObj->m_iBloodCastleEXP = 0;
		lpObj->m_bBloodCastleComplete = 0;
	}

	if (lpObj->MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL)
	{
		if (g_ConfigRead.server.GetServerType() == SERVER_BATTLECORE)
			lpObj->MapNumber = MAP_INDEX_LORENMARKET;
		else
			lpObj->MapNumber = MAP_INDEX_DEVIAS;

		MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y);
	}

	lpObj->m_bCCF_UI_Using = 0;
	lpObj->m_nCCF_BlowTime = 0;
	lpObj->m_byCCF_CanEnter = 0;
	lpObj->m_nCCF_CertiTick = 0;
	lpObj->m_nCCF_UserIndex = -1;
	lpObj->m_nCCF_BlowTime = 0;
	lpObj->m_byKillUserCount = 0;
	lpObj->m_byKillMobCount = 0;
	lpObj->m_bCCF_Quit_Msg = 0;

	if (lpObj->MapNumber == MAP_INDEX_REFUGEE)
	{
		lpObj->MapNumber = MAP_INDEX_BARRACKS;
		MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y);
	}

	if (lpObj->MapNumber == MAP_INDEX_ACHERON || lpObj->MapNumber == MAP_INDEX_DEBENTER)
	{
		short sX;
		short sY;
		short sLVL;
		unsigned char btMAPNUM = MAP_INDEX_ACHERON;
		unsigned char btDIR;

		int iMapNumber = gGateC.GetGate(417, (short &)sX, (short &)sY, (BYTE &)btMAPNUM, (BYTE &)btDIR, (short &)sLVL);

		if (iMapNumber >= 0)
		{
			lpObj->MapNumber = btMAPNUM;
			lpObj->X = sX;
			lpObj->Y = sY;
		}
	}

	if (lpObj->MapNumber == MAP_INDEX_SANTAVILLAGE && gGateC.CheckGateLevel(aIndex, 22) == false) //season4 add-on
	{
		if (lpObj->Class == CLASS_ELF)
		{
			lpObj->MapNumber = MAP_INDEX_NORIA;
			MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y);
		}
		else if (lpObj->Class == CLASS_SUMMONER)
		{
			lpObj->MapNumber = MAP_INDEX_ELBELAND;
			MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y);
		}
		else
		{
			lpObj->MapNumber = MAP_INDEX_RORENCIA;
			MapC[lpObj->MapNumber].GetLevelPos(lpObj->Level, lpObj->X, lpObj->Y);
		}
	}

	if (lpObj->MapNumber == MAP_INDEX_ARCA_WAR || lpObj->MapNumber == MAP_INDEX_DEBENTER_ARCA_WAR)
	{
		short sX;
		short sY;
		short sLVL;
		unsigned char btMAPNUM = MAP_INDEX_NORIA;
		unsigned char btDIR;

		int iMapNumber = gGateC.GetGate(27, (short &)sX, (short &)sY, (BYTE &)btMAPNUM, (BYTE &)btDIR, (short &)sLVL);

		if (iMapNumber >= 0)
		{
			lpObj->MapNumber = btMAPNUM;
			lpObj->X = sX;
			lpObj->Y = sY;
		}
	}

	if (IMPERIAL_MAP_RANGE(lpObj->MapNumber) != FALSE)
	{
		lpObj->MapNumber = MAP_INDEX_DEVIAS;
		MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y);
	}

	if (CC_MAP_RANGE(lpObj->MapNumber) != FALSE)
	{
		lpObj->MapNumber = MAP_INDEX_DEVIAS;
		MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y);
	}

	lpObj->m_cChaosCastleIndex = -1;
	lpObj->m_cChaosCastleSubIndex = -1;
	lpObj->m_iChaosCastleBlowTime = 0;
	lpObj->m_cKillUserCount = 0;
	lpObj->m_cKillMonsterCount = 0;

	if (IT_MAP_RANGE(lpObj->MapNumber) != FALSE)
	{
		if (g_ConfigRead.server.GetServerType() == SERVER_MARKET)
		{
			lpObj->MapNumber = MAP_INDEX_LORENMARKET;
		}

		else
		{
			lpObj->MapNumber = MAP_INDEX_ELBELAND;
		}

		MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y);
		lpObj->m_cIllusionTempleIndex = -1;
	}

	else
	{
		lpObj->m_cIllusionTempleIndex = -1;
	}

	if (DG_MAP_RANGE(lpObj->MapNumber) != FALSE)
	{
		lpObj->MapNumber = MAP_INDEX_ELBELAND;
		MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y);
	}

	if (g_ConfigRead.server.GetServerType() != SERVER_CASTLE)
	{
		if (lpObj->MapNumber == MAP_INDEX_KANTURU_BOSS)
		{
			short sX;
			short sY;
			short sLVL;
			BYTE btMAPNUM = lpObj->MapNumber;
			BYTE btDIR;

			int iMapNumber = gGateC.GetGate(137, (short &)sX, (short &)sY, (BYTE &)btMAPNUM, (BYTE &)btDIR, (short &)sLVL);

			if (iMapNumber >= 0)
			{
				lpObj->MapNumber = btMAPNUM;
				lpObj->X = sX;
				lpObj->Y = sY;
			}
		}
	}

	else
	{
		if (lpObj->MapNumber == MAP_INDEX_CASTLESIEGE)
		{
			if (g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
			{
				short sX;
				short sY;
				short sLVL;
				BYTE btMAPNUM = lpObj->MapNumber;
				BYTE btDIR;

				int iMapNumber = gGateC.GetGate(100, (short &)sX, (short &)sY, (BYTE &)btMAPNUM, (BYTE &)btDIR, (short &)sLVL);

				if (iMapNumber >= 0)
				{
					lpObj->MapNumber = btMAPNUM;
					lpObj->X = sX;
					lpObj->Y = sY;
					lpObj->m_sDestMapNumber = btMAPNUM;
					lpObj->m_btDestX = sX;
					lpObj->m_btDestY = sY;
				}
			}
		}
	}

	if (lpObj->MapNumber == MAP_INDEX_CASTLEHUNTZONE)
	{
		short sX;
		short sY;
		short sLVL;
		BYTE btMAPNUM = lpObj->MapNumber;
		BYTE btDIR;

		int iMapNumber = gGateC.GetGate(106, (short &)sX, (short &)sY, (BYTE &)btMAPNUM, (BYTE &)btDIR, (short &)sLVL);

		if (iMapNumber >= 0)
		{
			lpObj->MapNumber = btMAPNUM;
			lpObj->X = sX;
			lpObj->Y = sY;
			lpObj->m_sDestMapNumber = btMAPNUM;
			lpObj->m_btDestX = sX;
			lpObj->m_btDestY = sY;
		}
	}

	if (lpObj->m_bMapSvrMoveReq == true)
	{
		short sSVR_CODE = ::g_MapServerManager.CheckMoveMapSvr(lpObj->m_Index, lpObj->m_sDestMapNumber, lpObj->m_sPrevMapSvrCode);

		if (sSVR_CODE == g_ConfigRead.server.GetGameServerCode())
		{
			lpObj->MapNumber = lpObj->m_sDestMapNumber;
			lpObj->X = lpObj->m_btDestX;
			lpObj->Y = lpObj->m_btDestY;
			lpObj->StartX = lpObj->X;
			lpObj->StartY = lpObj->Y;
		}

		else
		{
			g_Log.AddC(TColor::Red, "[MapServerMng] Dest Map Server doesn't have Map [%s][%s] : %d",
				lpObj->AccountID, lpObj->Name, lpObj->m_sDestMapNumber);
			gObjDel(lpObj->m_Index);

			return FALSE;
		}
	}

	if (lpObj->MapNumber == MAP_INDEX_LORENMARKET)
	{
		BYTE btAttr = MapC[lpObj->MapNumber].GetAttr(lpObj->X, lpObj->Y);

		if ((btAttr & 1) != 1)
		{
			MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y);
			lpObj->StartX = lpObj->X;
			lpObj->StartY = lpObj->Y;
		}
	}

	if (g_ConfigRead.server.GetServerType() == SERVER_BATTLECORE)
	{
		lpObj->MapNumber = MAP_INDEX_LORENMARKET;
		BYTE btAttr = MapC[lpObj->MapNumber].GetAttr(lpObj->X, lpObj->Y);

		if ((btAttr & 1) != 1)
		{
			MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y);
			lpObj->StartX = lpObj->X;
			lpObj->StartY = lpObj->Y;
		}
	}

	if (g_MapAttr.CheckVIP(lpObj->MapNumber) > lpObj->m_PlayerData->VipType)
	{
		lpObj->MapNumber = MAP_INDEX_RORENCIA;
		MapC[lpObj->MapNumber].GetLevelPos(lpObj->Level, lpObj->X, lpObj->Y);
	}

	BYTE attr = MapC[lpObj->MapNumber].GetAttr(lpObj->X, lpObj->Y);
	int result;
	short x;
	short y;
	BYTE mapNumber;
	BYTE dir;
	short level;
	int gt;
	int userlevel;

	if ((attr & 4) == 4 || (attr & 8) == 8)
	{
		g_Log.Add("error: Invalid place to be located causes modifying location");
		MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y);
	}

	mapNumber = MAP_INDEX_RORENCIA;
	userlevel = lpObj->Level;
	x = lpObj->X;
	y = lpObj->Y;
	dir = lpObj->Dir;
	level = lpObj->Level;
	mapNumber = lpObj->MapNumber;
	gt = 0;

	if (g_MapAttr.CheckBlockEntry(lpObj->MapNumber) == true)
	{
		g_Log.Add("[%s][Forced Move From %s to Lorencia] (%s)(%s) (BeforeMap:%d(%d/%d))",
			Lang.GetMap(0, lpObj->MapNumber), Lang.GetMap(0, lpObj->MapNumber), lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y);

		lpObj->MapNumber = MAP_INDEX_RORENCIA;
		MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, (short &)lpObj->X, (short &)lpObj->Y);
	}

	if (g_ConfigRead.server.IsNonPvP() == TRUE && (g_NewPVP.IsVulcanusMap(lpObj->MapNumber) || g_NewPVP.IsPKFieldMap(lpObj->MapNumber)))
	{
		short sX;
		short sY;
		short sLVL;
		unsigned char btMAPNUM = lpObj->MapNumber;
		unsigned char btDIR;

		int iMapNumber = gGateC.GetGate(17, (short &)sX, (short &)sY, (BYTE &)btMAPNUM, (BYTE &)btDIR, (short &)sLVL);

		if (iMapNumber >= 0)
		{
			lpObj->MapNumber = btMAPNUM;
			lpObj->X = sX;
			lpObj->Y = sY;
		}
	}

	if (g_ConfigRead.server.IsNonPvP() == FALSE && g_NewPVP.IsPKFieldMap(lpObj->MapNumber))
	{
		short sX;
		short sY;
		short sLVL;
		unsigned char btMAPNUM = lpObj->MapNumber;
		unsigned char btDIR;

		int iMapNumber = gGateC.GetGate(17, (short &)sX, (short &)sY, (BYTE &)btMAPNUM, (BYTE &)btDIR, (short &)sLVL);

		if (iMapNumber >= 0)
		{
			lpObj->MapNumber = btMAPNUM;
			lpObj->X = sX;
			lpObj->Y = sY;
		}
	}

	if (lpObj->MapNumber == MAP_INDEX_HATCHERY &&
		(g_Raklion.GetRaklionState() == RAKLION_STATE_CLOSE_DOOR ||
			g_Raklion.GetRaklionState() == RAKLION_STATE_ALL_USER_DIE ||
			g_Raklion.GetRaklionState() == RAKLION_STATE_NOTIFY_4 ||
			g_Raklion.GetRaklionState() == RAKLION_STATE_END)) //season4 add-on
	{
		gt = 286;
	}

	if (lpObj->MapNumber == MAP_INDEX_RAKLION) //season4 add-on
	{
		g_RaklionUtil.NotifyRaklionCurrentState(lpObj->m_Index, g_Raklion.GetRaklionState(), g_Raklion.GetRaklionStateDetail());
	}

	if (lpObj->MapNumber == MAP_INDEX_DEVIAS)
	{
		gt = 22;
	}
	else if (lpObj->MapNumber == MAP_INDEX_NORIA)
	{
		gt = 27;
	}
	else if (lpObj->MapNumber == MAP_INDEX_LOSTTOWER)
	{
		gt = 42;
	}

	if (gt > 0)
	{
		result = gGateC.GetGate(gt, x, y, mapNumber, dir, level);

		if (result >= 0)
		{
			if (lpObj->Class == CLASS_DARKLORD || lpObj->Class == CLASS_MAGUMSA ||
				lpObj->Class == CLASS_RAGEFIGHTER || lpObj->Class == CLASS_GROWLANCER)
			{
				if (level > 0)
				{
					level = level / 3 * 2;
				}
			}

			if (level != 0 && lpObj->Level < level)
			{
				if (lpObj->Class == CLASS_ELF) // Elf
				{
					lpObj->MapNumber = MAP_INDEX_NORIA;
					MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, lpObj->X, lpObj->Y);
				}
				else if (lpObj->Class == CLASS_SUMMONER)
				{
					lpObj->MapNumber = MAP_INDEX_ELBELAND;
					MapC[lpObj->MapNumber].GetLevelPos(lpObj->Level, lpObj->X, lpObj->Y);
				}
				else
				{
					lpObj->MapNumber = MAP_INDEX_RORENCIA;
					MapC[lpObj->MapNumber].GetLevelPos(lpObj->Level, lpObj->X, lpObj->Y);
				}
			}
		}
		else
		{
			g_Log.Add("error : %d (%s %d)", result, __FILE__, __LINE__);
		}
	}

	lpMsg->MapX = lpObj->X;
	lpMsg->MapY = lpObj->Y;
	lpObj->TX = lpMsg->MapX;
	lpObj->TY = lpMsg->MapY;
	lpObj->m_OldX = lpObj->X;
	lpObj->m_OldY = lpObj->Y;
	lpObj->m_PlayerData->Experience = lpMsg->Exp;

	if (lpMsg->mlNextExp == NULL && lpMsg->mLevel == NULL && lpMsg->mlExp == NULL)
	{
		g_MasterLevelSkillTreeSystem.InitData(lpObj);
	}

	else
	{
		lpObj->m_PlayerData->MasterLevel = lpMsg->mLevel;
		lpObj->m_PlayerData->MasterExperience = lpMsg->mlExp;
		lpObj->m_PlayerData->MasterNextExp = lpMsg->mlNextExp;
		lpObj->m_PlayerData->MasterPoint = lpMsg->mlPoint;
	}

	lpObj->m_PlayerData->Strength = lpMsg->Str;
	lpObj->m_PlayerData->Dexterity = lpMsg->Dex;
	lpObj->m_PlayerData->Vitality = lpMsg->Vit;
	lpObj->m_PlayerData->Energy = lpMsg->Energy;
	lpObj->Life = lpMsg->Life;
	//lpObj->Life /= 10.0f;
	lpObj->MaxLife = lpMsg->MaxLife;
	//lpObj->MaxLife /= 10.0f;
	lpObj->Mana = lpMsg->Mana;
	//lpObj->Mana /= 10.0f;
	lpObj->MaxMana = lpMsg->MaxMana;
	//lpObj->MaxMana /= 10.0f;
	lpObj->m_PlayerData->Money = lpMsg->Money;
	lpObj->Leadership = lpMsg->Leadership;
	lpObj->ChatLimitTime = lpMsg->ChatLitmitTime;
	lpObj->m_PlayerData->iFruitPoint = lpMsg->iFruitPoint;
	lpObj->m_PlayerData->m_iResets = lpMsg->resets;
	lpObj->WinDuels = lpMsg->WinDuels;
	lpObj->LoseDuels = lpMsg->LoseDuels;
	lpObj->GameMaster = lpMsg->GmCode;
	lpObj->PenaltyMask = lpMsg->PenaltyMask;
	lpObj->m_PlayerData->AgilityCheckDelay = GetTickCount();
	lpObj->m_PlayerData->AgilityCheckCount = 0;

	if (lpObj->m_PlayerData->Strength > g_MaxStrength)
	{
		lpObj->m_PlayerData->Strength = g_MaxStrength;
		lpMsg->Str = g_MaxStrength;
	}

	if (lpObj->m_PlayerData->Dexterity > g_MaxAgility)
	{
		lpObj->m_PlayerData->Dexterity = g_MaxAgility;
		lpMsg->Dex = g_MaxAgility;
	}

	if (lpObj->m_PlayerData->Vitality > g_MaxVitality)
	{
		lpObj->m_PlayerData->Vitality = g_MaxVitality;
		lpMsg->Vit = g_MaxVitality;
	}

	if (lpObj->m_PlayerData->Energy > g_MaxEnergy)
	{
		lpObj->m_PlayerData->Energy = g_MaxEnergy;
		lpMsg->Energy = g_MaxEnergy;
	}

	if (lpObj->Leadership > g_MaxCommand)
	{
		lpObj->Leadership = g_MaxCommand;
		lpMsg->Leadership = g_MaxCommand;
	}

	if (lpObj->Mana < 1.0f)
	{
		lpObj->Mana = 1.0f;
	}

	if (lpObj->Life < 1.0f)
	{
		lpObj->Life = 1.0f;
	}

	lpObj->MaxLife = DCInfo.DefClass[lpObj->Class].Life + ((lpObj->Level + lpObj->m_PlayerData->MasterLevel) - 1) * DCInfo.DefClass[lpObj->Class].LevelLife + ((lpObj->m_PlayerData->Vitality - DCInfo.DefClass[lpObj->Class].Vitality) * DCInfo.DefClass[lpObj->Class].VitalityToLife);
	lpObj->MaxMana = DCInfo.DefClass[lpObj->Class].Mana + ((lpObj->Level + lpObj->m_PlayerData->MasterLevel) - 1) * DCInfo.DefClass[lpObj->Class].LevelMana + ((lpObj->m_PlayerData->Energy - DCInfo.DefClass[lpObj->Class].Energy) * DCInfo.DefClass[lpObj->Class].EnergyToMana);

	if (lpObj->Life > lpObj->MaxLife)
	{
		lpObj->Life = lpObj->MaxLife;
	}

	if (lpObj->Mana > lpObj->MaxMana)
	{
		lpObj->Mana = lpObj->MaxMana;
	}

	lpObj->VitalityToLife = DCInfo.DefClass[lpObj->Class].VitalityToLife;
	lpObj->EnergyToMana = DCInfo.DefClass[lpObj->Class].EnergyToMana;

	gObjSetBP(aIndex);
	lpObj->BP = lpObj->MaxBP / 2;
	::gObjCalcMaxLifePower(lpObj->m_Index);
	lpObj->m_PK_Count = lpMsg->PkCount;
	lpObj->m_PK_Level = lpMsg->PkLevel;
	lpObj->m_PK_Time = lpMsg->PkTime;

	lpObj->m_PlayerData->m_InventoryExpansion = lpMsg->InventoryExpansion;
	lpObj->m_PlayerData->m_WarehouseExpansion = lpMsg->WarehouseExpansion;

	if (lpObj->m_PK_Level == 3)
	{
		lpObj->m_PK_Count = 0;
		lpObj->m_PK_Time = 0;
	}

	lpObj->MaxRegenTime = 4000;	// Resurrection of the player if die
	lpObj->m_MoveSpeed = 1000;

	memcpy(lpObj->m_PlayerData->m_Quest, lpMsg->dbQuest, sizeof(lpObj->m_PlayerData->m_Quest));

	if (lpObj->m_PlayerData->m_Quest[0] == 0)
	{
		memset(lpObj->m_PlayerData->m_Quest, (BYTE)-1, sizeof(lpObj->m_PlayerData->m_Quest));
	}

	if (::g_QuestInfo.GetQuestState(lpObj, 2) == 2)	// Ring of Honor
	{
		if (lpObj->Level < /*QUEST_MINLEVEL_PLUSSTAT*/1)	// Repair Bug of Marlon Before
		{
			::g_QuestInfo.ReSetQuestState(lpObj, 2);
			g_Log.Add("[%s][%s] Find Invalid QuestInfo (%d)",
				lpObj->AccountID, lpObj->Name, 2);
		}
		else
		{
			lpObj->m_PlayerData->PlusStatQuestClear = true;
		}
	}

	if (::g_QuestInfo.GetQuestState(lpObj, 3) == 2)	// Dark Stone
	{
		if (lpObj->Level < /*QUEST_MINLEVEL_PLUSSTAT*/1)	// Repair Bug of Marlon After
		{
			::g_QuestInfo.ReSetQuestState(lpObj, 3);
			g_Log.Add("[%s][%s] Find Invalid QuestInfo (%d)",
				lpObj->AccountID, lpObj->Name, 3);
		}
		else
		{
			lpObj->m_PlayerData->ComboSkillquestClear = true;
		}
	}

	if (lpObj->m_PlayerData->m_Quest[21] > 20)
	{
		lpObj->m_PlayerData->m_Quest[21] = 0;
	}

	if (lpObj->m_PlayerData->m_Quest[22] > 20)
	{
		lpObj->m_PlayerData->m_Quest[22] = 0;
	}

	if (lpObj->m_PlayerData->m_Quest[24] > 20)
	{
		lpObj->m_PlayerData->m_Quest[24] = 0;
	}

	::gObjSetInventory1Pointer(lpObj);
	lpObj->m_PlayerData->GuildNumber = 0;
	lpObj->m_PlayerData->lpGuild = NULL;

	for (int i = 0; i < MAX_MAGIC; i++)
	{
		lpObj->Magic[i].Clear();
	}

	if (lpObj->Class == CLASS_WIZARD || lpObj->Class == CLASS_ELF ||
		lpObj->Class == CLASS_KNIGHT || lpObj->Class == CLASS_DARKLORD ||
		lpObj->Class == CLASS_MAGUMSA || lpObj->Class == CLASS_SUMMONER ||
		lpObj->Class == CLASS_RAGEFIGHTER || lpObj->Class == CLASS_GROWLANCER)
	{
		BOOL bEnergyBall = TRUE;
		BOOL bForceSkill = TRUE;
		BOOL IsGetInitinityArrowSkill = FALSE;

		gObjMagicAddEnergyCheckOnOff(0);

		for (n = 0; n < MAX_MAGIC; n++)
		{
			itype = lpMsg->dbMagicList[n * MAX_MAGIC_INFO];

			if ((lpMsg->dbMagicList[n * MAX_MAGIC_INFO + DBM_LEVEL] & 0x7) > 0) //Season3 add-on
			{
				itype = itype * (lpMsg->dbMagicList[n * MAX_MAGIC_INFO + DBM_LEVEL] & 0x7) + lpMsg->dbMagicList[n * MAX_MAGIC_INFO + DBM_EXTRA];
			}

			if (itype != 0xFF)
			{
				if (g_MasterLevelSkillTreeSystem.CheckMasterLevelSkill(itype))
				{
					BYTE level = lpMsg->dbMagicList[n * MAX_MAGIC_INFO + DBM_LEVEL] >> 3;

					if (gObjMagicAdd(lpObj, itype, level) > -1)
					{
						lpObj->m_PlayerData->MasterPointUsed += level;
						g_MasterLevelSkillTreeSystem.CalcPassiveSkillData(lpObj, itype, level);
					}
				}

				else
				{

					if (itype == 17)
					{
						bEnergyBall = FALSE;
					}

					if (itype == 60)
					{
						bForceSkill = FALSE;
					}

					if (itype == 77)
					{
						IsGetInitinityArrowSkill = 1;
						gObjMagicAdd(lpObj, itype, lpMsg->dbMagicList[n * MAX_MAGIC_INFO + DBM_LEVEL] & 0xF);
					}

					else if (itype != 18 && itype != 19 && itype != 20 && itype != 21 && itype != 22 && itype != 23 && itype != 24 && itype != 25 && itype != 49 && itype != 56 && itype != 62 && itype != 76 && itype != 54 && itype != 223 && itype != 224 && itype != 225 && itype != 260 && itype != 261 && itype != 270)
					{
						gObjMagicAdd(lpObj, itype, lpMsg->dbMagicList[n * MAX_MAGIC_INFO + DBM_LEVEL] & 0xF);
					}
				}
			}
		}

		if (lpObj->Class == CLASS_WIZARD)	// Wizard
		{
			if (bEnergyBall == TRUE)
			{
				gObjMagicAdd(lpObj, 17, 0);
				bEnergyBall = FALSE;
			}
		}

		if (lpObj->Class == CLASS_DARKLORD)	// Dark Lord
		{
			if (bForceSkill == TRUE)
			{
				gObjMagicAdd(lpObj, 60, 0);
				bForceSkill = FALSE;
			}
		}

		if (IsGetInitinityArrowSkill == 0 && lpObj->Class == CLASS_ELF)
		{
			if (lpObj->Level >= g_SkillAdditionInfo.GetInfinityArrowUseLevel())
			{
				if (lpObj->m_PlayerData->ChangeUP >= 1)
				{
					int iAddSkillPosition = gObjMagicAdd(lpObj, 77, 0);

					if (iAddSkillPosition > 0)
					{
						g_Log.Add("[%s][%s] Add Infinity Arrow Skill (Character Level : %d)(ChangeUp: %d)",
							lpObj->AccountID, lpObj->Name, lpObj->Level, lpObj->m_PlayerData->ChangeUP);
					}
				}
			}
		}

		if (lpObj->Class == CLASS_WIZARD || lpObj->Class == CLASS_SUMMONER)
		{
			gObjMagicAdd(lpObj, 45, 0);
		}
		else if (lpObj->Class == CLASS_KNIGHT)
		{
			gObjMagicAdd(lpObj, 44, 0);
		}
		else if (lpObj->Class == CLASS_ELF)
		{
			gObjMagicAdd(lpObj, 46, 0);
		}
		else if (lpObj->Class == CLASS_MAGUMSA)	// Magic Gladiator
		{
			gObjMagicAdd(lpObj, 57, 0);
			gObjMagicAdd(lpObj, 73, 0);
		}

		else if (lpObj->Class == CLASS_DARKLORD)	// Dark Lord
		{
			gObjMagicAdd(lpObj, 74, 0);	// Fire Blast
			//gObjMagicAdd(lpObj, 75, 0);	// Sacrifice - BUG Apply Fix in Skill.vtm coz is not created
		}

		else if (lpObj->Class == CLASS_RAGEFIGHTER)
		{
			gObjMagicAdd(lpObj, 269, 0);
		}

		else if (lpObj->Class == CLASS_GROWLANCER)
		{
			gObjMagicAdd(lpObj, 275, 0);
		}

		gObjMagicAdd(lpObj, 67, 0);
		gObjMagicAdd(lpObj, 68, 0);
		gObjMagicAdd(lpObj, 69, 0);
		gObjMagicAdd(lpObj, 70, 0);
		gObjMagicAdd(lpObj, 71, 0);
		gObjMagicAdd(lpObj, 72, 0);

		::gObjMagicAddEnergyCheckOnOff(TRUE);
	}

	bool bAllItemExist = true;

	DbItemSetInByte(lpObj, lpMsg, &bAllItemExist);

	lpObj->Live = TRUE;
	lpObj->Type = OBJ_USER;
	lpObj->TargetNumber = -1;
	lpObj->Connected = PLAYER_PLAYING;
	lpObj->m_PlayerData->AgilityCheckDelay = GetTickCount();
	lpObj->m_PlayerData->AgilityCheckCount = 0;
	lpObj->AgiCheckTime = GetTickCount();
	gObjFindInventoryEquipment(aIndex);
	::gObjMakePreviewCharSet(lpObj->m_Index);
	CreateFrustrum(lpObj->X, lpObj->Y, lpObj->m_Index);
	MapC[lpObj->MapNumber].SetStandAttr(lpObj->X, lpObj->Y);
	lpObj->m_OldX = lpObj->X;
	lpObj->m_OldY = lpObj->Y;
	lpMsg->NextExp = lpObj->m_PlayerData->NextExp;
	lpObj->m_PlayerData->LivePacketTime = 0;
	lpObj->m_PlayerData->LiveCheckDetectCount = 0;
	lpObj->m_PlayerData->Ruud = lpMsg->Ruud;

	if (lpObj->Life == 0.0f)
	{
		lpObj->Live = TRUE;
		lpObj->m_State = 4;
		lpObj->DieRegen = TRUE;
		lpObj->RegenTime = GetTickCount();
	}
	else
	{
		lpObj->m_State = 1;
	}

	if ((lpMsg->CtlCode & 8) == 8 && lpMsg->GmCode != 0)
	{
		lpObj->Authority = 2;
		gObjAddBuffEffect(lpObj, BUFFTYPE_GM_MARK, 0, 0, 0, 0, -10);
		g_Log.AddC(TColor::Red, "[%s][%s] Administrator setting", lpObj->AccountID, lpObj->Name);
		cManager.ManagerAdd(lpObj->Name, lpObj->m_Index);
		MsgOutput(lpObj->m_Index, Lang.GetText(0, 408), lpMsg->GmExpDays);
	}

	else
	{
		lpObj->Authority = 1;
	}

	if ((lpMsg->CtlCode & 0x20) == 0x20 && lpMsg->GmCode != 0)
	{
		lpObj->Authority = 0x20;
		gObjAddBuffEffect(lpObj, BUFFTYPE_GM_MARK, 0, 0, 0, 0, -10);
		g_Log.AddC(TColor::Red, "(%s)(%s) Set Event GM", lpObj->AccountID, lpObj->Name);
		MsgOutput(lpObj->m_Index, Lang.GetText(0, 409), lpMsg->GmExpDays);
	}

	lpObj->Penalty = 0;

	if (lpObj->m_cAccountItemBlock != 0)
	{
		lpObj->Penalty |= 8;
	}

	if ((lpMsg->CtlCode & 2) == 2)
	{
		lpObj->Penalty |= 4;
		g_Log.AddC(TColor::Blue, "Penalty : Item Don't touch  %s %s", lpObj->AccountID, lpObj->Name);
	}

	if ((lpObj->PenaltyMask & 1) == 1)
	{
		lpObj->Penalty |= 2;
		lpObj->ChatBlockTime = lpMsg->ChatBlockTime;
		MsgOutput(aIndex, Lang.GetText(0, 410));
	}

	gObjAuthorityCodeSet(lpObj);
	g_Log.Add("(%d)(%s)(%s) Character select", lpObj->m_Index, lpObj->AccountID, lpObj->Name);

	BYTE weather = MapC[lpObj->MapNumber].GetWeather();

	if ((weather >> 4) > 0)
	{
		GSProtocol.CGWeatherSend(lpObj->m_Index, weather);
	}

	if (DragonEvent->GetState() > 0)
	{
		if (DragonEvent->GetMapNumber() == lpObj->MapNumber)
		{
			::GSProtocol.GCMapEventStateSend(lpObj->MapNumber, 1, 1);
		}
	}

	if (g_ConfigRead.server.GetServerType() == SERVER_CASTLE && lpObj->MapNumber == MAP_INDEX_CASTLESIEGE)
	{
		GSProtocol.GCAnsCsNotifyStart(lpObj->m_Index, CASTLESIEGEREADY(g_CastleSiege.GetCastleState()));

		if (g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
		{
			g_CastleSiege.NotifyCsSelfLeftTime(lpObj->m_Index);
		}
	}

	::gEledoradoEvent.CheckGoldDercon(lpObj->MapNumber);
	//::GSProtocol.GCCheckMainExeKeySend(lpObj->m_Index);

	if (lpMsg->Married == 1)
	{
		memcpy(lpObj->MarryName, lpMsg->MarryName, 10);
		lpObj->Married = 1;
	}
	else
	{
		lpObj->Married = 0;
	}

	if (m_ObjBill[lpObj->m_Index - g_ConfigRead.server.GetObjectStartUserIndex()].GetCertify() >= 0 && m_ObjBill[lpObj->m_Index - g_ConfigRead.server.GetObjectStartUserIndex()].GetCertify() < 3)
	{
		gLCount[m_ObjBill[lpObj->m_Index - g_ConfigRead.server.GetObjectStartUserIndex()].GetCertify()].Add();
	}

	if (bAllItemExist == false)
	{
		PMSG_NOTICE pNotice;
		TNotice::MakeNoticeMsgEx((TNotice *)&pNotice, 1, Lang.GetText(0, 411));
		IOCP.DataSend(lpObj->m_Index, (UCHAR *)&pNotice, pNotice.h.size);
	}

	if (lpObj->Level <= g_iCharacterRecuperationMaxLevel)
	{
		lpObj->m_iAutoRecuperationTime = GetTickCount();
	}
	else
	{
		lpObj->m_iAutoRecuperationTime = 0;
	}

	lpObj->dwShieldAutoRefillTimer = GetTickCount();

	g_Log.Add("[ShieldSystem][CalcSDPoint] [%s][%s] user SD Gage : %d",
		lpObj->AccountID, lpObj->Name, lpObj->iMaxShield + lpObj->iAddShield);

	if (g_ConfigRead.SeasonX == true)
	{
		g_Log.Add("[Ruud Currency] [%s][%s] Ruud:%d", lpObj->AccountID, lpObj->Name, lpObj->m_PlayerData->Ruud);
	}

	g_CashShop.AddUser(lpObj);
	g_PeriodItemEx.AddPeriodInfo(lpObj);
	g_CMuunSystem.AddMuunItemPeriodInfo(lpObj);

	if (lpObj->Class == CLASS_DARKLORD && lpObj->m_Index >= g_ConfigRead.server.GetObjectStartUserIndex())
	{
		gDarkSpirit[lpObj->m_Index - g_ConfigRead.server.GetObjectStartUserIndex()].SetMode(CDarkSpirit::ePetItem_Mode::PetItem_Mode_Normal, -1);
	}

	return TRUE;
}

int gObjCanItemTouch(LPOBJ lpObj, int type)
{
	if ( (lpObj->Penalty &4) == 4 )
	{
		return false;
	}

	if ( (lpObj->Penalty &8) == 8 )
	{
		if ( type == 6 || type == 8 )
		{
			return true;
		}
		return false;
	}
	return true;
}

void gObjMagicTextSave(LPOBJ lpObj)
{
	int total=0;

	for ( int n=0;n<MAGIC_SIZE;n++)
	{
		if ( lpObj->Magic[n].IsMagic() == TRUE )
		{
			CharSaveLog->Output("[%s][%s] Skill Save Complete %d %d", lpObj->AccountID, lpObj->Name, n, lpObj->Magic[n].m_Skill);
			total++;
		}
	}
}

void ItemIsBufExOption(BYTE * buf, CItem * lpItem)
{
	for ( int i=0;i<MAX_EXOPTION_SIZE;i++ )
	{
		buf[i] = 0;
	}

	if ( (lpItem->m_NewOption & 0x20) == 0x20 )
	{
		buf[0] = TRUE;
	}

	if ( (lpItem->m_NewOption & 0x10) == 0x10 )
	{
		buf[1] = TRUE;
	}

	if ( (lpItem->m_NewOption & 8) == 8 )
	{
		buf[2] = TRUE;
	}

	if ( (lpItem->m_NewOption & 4) == 4 )
	{
		buf[3] = TRUE;
	}

	if ( (lpItem->m_NewOption & 2) == 2 )
	{
		buf[4] = TRUE;
	}

	if ( (lpItem->m_NewOption & 1) == 1 )
	{
		buf[5] = TRUE;
	}

}

void gObjStatTextSave(LPOBJ lpObj)
{
	CharSaveLog->Output("[%s][%s] CharInfoSave : Class=%d Level=%d LVPoint=%d Exp=%d Str=%d Dex=%d Vit=%d Energy=%d Leadership:%d Map=%d Pk=%d",
		lpObj->AccountID, lpObj->Name, lpObj->m_PlayerData->DbClass,lpObj->Level, lpObj->m_PlayerData->LevelUpPoint, lpObj->m_PlayerData->Experience, 
		lpObj->m_PlayerData->Strength, lpObj->m_PlayerData->Dexterity, lpObj->m_PlayerData->Vitality, lpObj->m_PlayerData->Energy, lpObj->Leadership, lpObj->MapNumber,
		lpObj->m_PK_Level);
}
 
void gObjItemTextSave(LPOBJ lpObj)
{
	BYTE NewOption[MAX_EXOPTION_SIZE];
	int n;

	for ( n=0;n<INVENTORY_SIZE;n++)
	{
		if ( lpObj->pInventory[n].IsItem() == TRUE )
		{
			if ( lpObj->pInventory[n].m_serial != 0 )
			{
				ItemIsBufExOption(NewOption, &lpObj->pInventory[n]);
				CharSaveLog->Output("[%s][%s] SI[%d,%s,%d,%d,%d,%d]serial:[%I64d] dur:[%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set[%d] 380:[%d] HO:[%d,%d] E:[%d]", lpObj->AccountID, lpObj->Name, n, lpObj->pInventory[n].GetName(),
					lpObj->pInventory[n].m_Level, lpObj->pInventory[n].m_Option1,
					lpObj->pInventory[n].m_Option2, lpObj->pInventory[n].m_Option3,
					lpObj->pInventory[n].m_Number, (BYTE)lpObj->pInventory[n].m_Durability,
					NewOption[0], NewOption[1], NewOption[2], NewOption[3], NewOption[4], NewOption[5], NewOption[6],
					lpObj->pInventory[n].m_SetOption,lpObj->pInventory[n].m_ItemOptionEx >> 7,g_kJewelOfHarmonySystem.GetItemStrengthenOption(&lpObj->pInventory[n]),g_kJewelOfHarmonySystem.GetItemOptionLevel(&lpObj->pInventory[n]),
					lpObj->pInventory[n].m_BonusSocketOption);
			}
		}
	}

	g_Log.Add("[%s][%s] Item Save Complete. Money [ %d ]", lpObj->AccountID, lpObj->Name, lpObj->m_PlayerData->Money);
}

void gObjWarehouseTextSave(LPOBJ lpObj)
{
	BYTE NewOption[MAX_EXOPTION_SIZE];
	int n;

	for ( n=0;n<WAREHOUSE_SIZE;n++)
	{
		if ( lpObj->pWarehouse[n].IsItem() == TRUE )
		{
			if ( lpObj->pWarehouse[n].m_serial != 0 )
			{
				ItemIsBufExOption(NewOption, &lpObj->pWarehouse[n]);
				CharSaveLog->Output("[%s][%s] SW [%d,%s,%d,%d,%d,%d]serial:[%I64d] dur:[%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d] 380:[%d] HO:[%d,%d] E:[%d]", lpObj->AccountID, lpObj->Name, n, lpObj->pWarehouse[n].GetName(),
					lpObj->pWarehouse[n].m_Level, lpObj->pWarehouse[n].m_Option1,
					lpObj->pWarehouse[n].m_Option2, lpObj->pWarehouse[n].m_Option3,
					lpObj->pWarehouse[n].m_Number, (BYTE)lpObj->pWarehouse[n].m_Durability,
					NewOption[0], NewOption[1], NewOption[2], NewOption[3], NewOption[4], NewOption[5], NewOption[6],
					lpObj->pWarehouse[n].m_SetOption, lpObj->pWarehouse[n].m_ItemOptionEx>>7,
					g_kJewelOfHarmonySystem.GetItemStrengthenOption(&lpObj->pWarehouse[n]),
					g_kJewelOfHarmonySystem.GetItemOptionLevel(&lpObj->pWarehouse[n]),
					lpObj->pWarehouse[n].m_BonusSocketOption);
			}
		}
	}

	g_Log.Add("[%s][%s] Store Save Complete. wMoney[%d]", lpObj->AccountID, lpObj->Name, lpObj->WarehouseMoney);
}

void gObjAuthorityCodeSet(LPOBJ lpObj)
{
	if ( (lpObj->Authority&1) == 1 )
	{
		return;
	}

	if ( (lpObj->Authority&2)== 2 )
	{
		lpObj->AuthorityCode = -1;
		return;
	}

	if ( (lpObj->Authority&4) == 4 )
	{
		return;
	}

	if ( (lpObj->Authority&8) == 8 )
	{
		lpObj->AuthorityCode |= 1;
		lpObj->AuthorityCode |= 2;
		lpObj->AuthorityCode |= 4;
		lpObj->AuthorityCode |= 8;
		lpObj->AuthorityCode |= 16;
		lpObj->AuthorityCode |= 32;

		return;
	}

	if ( (lpObj->Authority&16) == 16 )
	{
		return;
	}

	if ( (lpObj->Authority&32) == 32 )
	{
		lpObj->AuthorityCode |= 1;
		lpObj->AuthorityCode |= 2;
		lpObj->AuthorityCode |= 4;
		lpObj->AuthorityCode |= 8;
		lpObj->AuthorityCode |= 16;
		lpObj->AuthorityCode |= 32;

		return;
	}

	if ( (lpObj->PenaltyMask&1) == 1 )
	{
		lpObj->Penalty |= 2;
	}

}

int gObjSetPosMonster(int aIndex, int PosTableNum)
{
	
	if ( (  (aIndex<0)? FALSE: (aIndex > g_ConfigRead.server.GetObjectMax()-1)? FALSE: TRUE ) == FALSE )
	{
		g_Log.Add("error : %s %d", __FILE__, __LINE__);
		return FALSE;
	}

	
	if ( (  (PosTableNum<0)? FALSE: (PosTableNum > g_ConfigRead.server.GetObjectMaxMonster()-1)? FALSE: TRUE ) == FALSE )
	{
		g_Log.Add("error : %s %d", __FILE__, __LINE__);
		return FALSE;
	}

	LPOBJ lpObj = &gObj[aIndex];

	lpObj->m_PosNum = PosTableNum;
	lpObj->X = gMSetBase.m_Mp[PosTableNum].m_X;
	lpObj->Y = gMSetBase.m_Mp[PosTableNum].m_Y;
	lpObj->MapNumber = gMSetBase.m_Mp[PosTableNum].m_MapNumber;
	lpObj->TX = lpObj->X;
	lpObj->TY = lpObj->Y;
	lpObj->m_OldX = lpObj->X;
	lpObj->m_OldY = lpObj->Y;
	lpObj->Dir = gMSetBase.m_Mp[PosTableNum].m_Dir;
	lpObj->StartX = lpObj->X;
	lpObj->StartY = lpObj->Y;

	if ( gMSetBase.GetPosition(PosTableNum, lpObj->MapNumber, lpObj->X, lpObj->Y) == FALSE )
	{
		g_Log.Add("[Invalid Location]: MAP: %d X: %d Y: %d POSTABNUM: %d", lpObj->MapNumber, gMSetBase.m_Mp[PosTableNum].m_X, gMSetBase.m_Mp[PosTableNum].m_Y, PosTableNum);
		g_Log.Add("error: Monster location setting error %s %d", __FILE__, __LINE__);
		g_Log.Add("error-L2: %d class: %d", PosTableNum, lpObj->Class);
		return FALSE;
	}

	lpObj->TX = lpObj->X;
	lpObj->TY = lpObj->Y;
	lpObj->m_OldX = lpObj->X;
	lpObj->m_OldY = lpObj->Y;
	lpObj->Dir = gMSetBase.m_Mp[PosTableNum].m_Dir;
	lpObj->StartX = lpObj->X;
	lpObj->StartY = lpObj->Y;
	int iPentagramMainAttribute = 0;
	gMSetBase.GetPentagramMainAttribute(PosTableNum, &iPentagramMainAttribute);

	if ( iPentagramMainAttribute )
	{
		lpObj->m_iPentagramMainAttribute = iPentagramMainAttribute;
	}

	return true;
}

int gObjSetMonster(int aIndex, int MonsterClass)
{
	int Level;

	if ( ObjectMaxRange(aIndex) == FALSE )
	{
		g_Log.Add("error : %s %d", __FILE__, __LINE__);
		return false;
	}

	LPOBJ lpObj = &gObj[aIndex];

	lpObj->ConnectCheckTime = GetTickCount();

	if (g_ConfigRead.server.GetServerType() == SERVER_BATTLECORE)
	{
		if (MonsterClass > 547)
		{
			if (MonsterClass == 651 || MonsterClass == 669)
			{
				return FALSE;
			}
		}

		else if (MonsterClass >= 545 || (MonsterClass == 240 || MonsterClass == 385 || MonsterClass == 540))
		{
			return FALSE;
		}
	}

	else
	{
		if (MonsterClass >= 683 && MonsterClass <= 688)
		{
			return FALSE;
		}
	}

	lpObj->TargetNumber = -1;
	lpObj->m_RecallMon = -1;

	if (MonsterClass == 200)
	{
		gBSGround[0]->m_BallIndex = lpObj->m_Index;
	}

	if (MonsterClass == 240)
	{
		lpObj->m_btNpcType = NPC_TYPES::NPC_WAREHOUSE;
	}

	else if (MonsterClass == 238 || MonsterClass == 450 || MonsterClass == 478 || MonsterClass == 368 || MonsterClass == 369 || MonsterClass == 370 || MonsterClass == 452 || MonsterClass == 453)
	{
		lpObj->m_btNpcType = NPC_TYPES::NPC_CHAOSMIX;
	}

	else if (MonsterClass == 236)
	{
		lpObj->m_btNpcType == NPC_TYPES::NPC_GOLDARCHER;
	}

	else if (MonsterClass == 582)
	{
		lpObj->m_btNpcType = NPC_TYPES::NPC_PENTAGRAMMIX;
	}

	else
	{
		lpObj->m_btNpcType = NPC_TYPES::NPC_NONE;
	}

	lpObj->Connected = PLAYER_PLAYING;
	lpObj->Live = TRUE;
	lpObj->m_State = 1;

	if ( (MonsterClass >= 204 && MonsterClass <= 259)
		|| (MonsterClass >= 368 && MonsterClass <= 370) ||
		MonsterClass == 367 ||
		MonsterClass == 375 || MonsterClass == 376 || MonsterClass == 377 || MonsterClass == 379 ||  MonsterClass == 406 || MonsterClass == 407 || MonsterClass == 408 ||
		MonsterClass == 415 || MonsterClass == 416 || MonsterClass == 417 || MonsterClass == 465 || 
		MonsterClass == 467 || MonsterClass == 468 || MonsterClass == 469 || MonsterClass == 470 ||
		MonsterClass == 471 || MonsterClass == 472 || MonsterClass == 473 || MonsterClass == 474 ||
		MonsterClass == 475 || MonsterClass == 478 || MonsterClass == 371 || MonsterClass == 479 || MonsterClass == 540 || MonsterClass == 492 ||
		MonsterClass == 522 || MonsterClass == 464 || MonsterClass == 450 || MonsterClass == 451 || MonsterClass == 452 || MonsterClass == 453 ||
		MonsterClass == 543 || MonsterClass == 544 || MonsterClass == 385 || MonsterClass == 381 || MonsterClass == 382 || MonsterClass == 380 || MonsterClass == 383 || MonsterClass == 384 || MonsterClass == 541 || MonsterClass == 542 || MonsterClass == 546 || MonsterClass == 545 || MonsterClass == 547 || MonsterClass == 566 || MonsterClass == 567 || MonsterClass == 568 || MonsterClass == 577 || MonsterClass == 578 || MonsterClass == 579 ||
		MonsterClass == 583 || MonsterClass == 580 || MonsterClass == 581 || MonsterClass == 582 || MonsterClass == 584 || MonsterClass == 603 || MonsterClass == 604 || MonsterClass == 643 || MonsterClass == 651 || MonsterClass == 658 || MonsterClass == 659 || MonsterClass == 660 || MonsterClass == 661 || MonsterClass == 662 || MonsterClass == 663 || MonsterClass == 664 || MonsterClass == 665 || MonsterClass == 666 || MonsterClass == 667 ||
		MonsterClass == 668 || MonsterClass == 682 || MonsterClass == 683 || MonsterClass == 684 || MonsterClass == 685 || MonsterClass == 686 || MonsterClass == 687 || MonsterClass == 688)
	{
		lpObj->Type = OBJ_NPC;
	}
	else
	{
		lpObj->Type = OBJ_MONSTER;
	}

	if ( MonsterClass == 385 )
	{
		g_IT_Event.SetMirageIndex(lpObj->m_Index);
	}

	if ( MonsterClass >= 523 && MonsterClass <= 528 )
		lpObj->Type = OBJ_MONSTER;

	lpObj->Class = MonsterClass;
	Level = lpObj->Level;
	LPMONSTER_ATTRIBUTE lpm = gMAttr.GetAttr( lpObj->Class);

	if ( lpm == NULL )
	{
		g_Log.MsgBox("Warning! Cannot find monster attribute %d (%s %d)", lpObj->Class, __FILE__, __LINE__);
		return false;
	}

	strncpy(lpObj->Name, lpm->m_Name , 10 );

	if (lpm->m_Level == 0)
	{
		g_Log.Add("Monster level can't be 0, Class: %d, %s %d", lpObj->Class, __FILE__, __LINE__);
	}

	lpObj->Level = lpm->m_Level;
	lpObj->m_AttackSpeed = lpm->m_AttackSpeed;
	lpObj->m_AttackDamageMin = lpm->m_DamageMin;
	lpObj->m_AttackDamageMax = lpm->m_DamageMax;
	lpObj->m_Defense = lpm->m_Defense;
	lpObj->m_MagicDefense = lpm->m_MagicDefense;
	lpObj->m_AttackRating = lpm->m_AttackRating;
	lpObj->m_SuccessfulBlocking = lpm->m_Successfulblocking;
	lpObj->Life = lpm->m_Hp;
	lpObj->MaxLife = lpm->m_Hp;
	lpObj->Mana = lpm->m_Mp;
	lpObj->MaxMana = lpm->m_Mp;
	lpObj->m_MoveRange = lpm->m_MoveRange;
	lpObj->m_AttackSpeed = lpm->m_AttackSpeed;
	lpObj->m_MoveSpeed = lpm->m_MoveSpeed;
	lpObj->MaxRegenTime = lpm->m_RegenTime * 1000;
	lpObj->m_AttackRange = lpm->m_AttackRange;
	lpObj->m_ViewRange = lpm->m_ViewRange;
	lpObj->m_Attribute = lpm->m_Attribute;
	lpObj->m_AttackType = lpm->m_AttackType;
	lpObj->m_ItemRate = lpm->m_ItemRate;
	lpObj->m_MoneyRate = lpm->m_MoneyRate;

	lpObj->m_iScriptMaxLife = lpm->m_iScriptHP;
	lpObj->m_iBasicAI = lpm->m_iAINumber;
	lpObj->m_iCurrentAI = lpm->m_iAINumber;
	lpObj->m_iCurrentAIState = 0;
	lpObj->m_iLastAIRunTime = 0;
	lpObj->m_iGroupNumber = 0;
	lpObj->m_iSubGroupNumber = 0;
	lpObj->m_iGroupMemberGuid = -1;
	lpObj->m_iRegenType = 0;

	lpObj->m_Agro->ResetAll();

	lpObj->m_iLastAutomataRuntime = 0;
	lpObj->m_iLastAutomataDelay = 0;

	lpObj->m_btExpType = lpm->m_btExpType;

	if ( lpm->m_PentagramMainAttribute == 6 )
	{
		switch ( rand()%5 )
		{
			case 0:
				lpObj->m_iPentagramMainAttribute = EL_FIRE;
				break;
			case 1:
				lpObj->m_iPentagramMainAttribute = EL_WATER;
				break;
			case 2:
				lpObj->m_iPentagramMainAttribute = EL_EARTH;
				break;
			case 3:
				lpObj->m_iPentagramMainAttribute = EL_WIND;
				break;
			case 4:
				lpObj->m_iPentagramMainAttribute = EL_DARKNESS;
				break;
		}
	}

	else if ( lpm->m_PentagramMainAttribute > 0 )
	{
		lpObj->m_iPentagramMainAttribute = lpm->m_PentagramMainAttribute;
	}

	lpObj->m_iPentagramAttributePattern = lpm->m_PentagramAttributePattern;
	lpObj->m_iPentagramDefense = lpm->m_PentagramDefense;
	lpObj->m_iPentagramAttackMin = lpm->m_PentagramAttackMin;
	lpObj->m_iPentagramAttackMax = lpm->m_PentagramAttackMax;
	lpObj->m_iPentagramAttackRating = lpm->m_PentagramAttackRating;
	lpObj->m_iPentagramDefense = lpm->m_PentagramDefense;

	lpObj->pInventoryCount[0] = 0;
	memcpy(lpObj->m_Resistance, lpm->m_Resistance, MAX_RESISTENCE_TYPE );
	gObjSetInventory1Pointer(lpObj);

	if ( lpObj->m_AttackType != 0 )
	{
		if ( DG_MAP_RANGE(lpObj->MapNumber) == TRUE && lpObj->Type == OBJ_MONSTER )
		{
			gObjMonsterMagicAdd(lpObj, lpObj->m_AttackType, 1);
		}

		else if ( lpObj->m_AttackType >= 100 )
		{
			gObjMonsterMagicAdd(lpObj, lpObj->m_AttackType - 100, 1);
		}
		else
		{
			gObjMonsterMagicAdd(lpObj, lpObj->m_AttackType, 1);
		}

		if ( lpObj->Class == 144 || lpObj->Class == 174 || lpObj->Class == 182 || lpObj->Class == 190 || lpObj->Class == 260 || lpObj->Class == 268 || lpObj->Class == 334 )
		{
			gObjMonsterMagicAdd(lpObj, 17, 1);
		}

		if ( lpObj->Class == 145 
			|| lpObj->Class == 175 
			|| lpObj->Class == 183 
			|| lpObj->Class == 191 
			|| lpObj->Class == 261 
			|| lpObj->Class == 269 
			|| lpObj->Class == 146 
			|| lpObj->Class == 176 
			|| lpObj->Class == 184 
			|| lpObj->Class == 192 
			|| lpObj->Class == 262 
			|| lpObj->Class == 270 
			|| lpObj->Class == 147 
			|| lpObj->Class == 177 
			|| lpObj->Class == 185 
			|| lpObj->Class == 193 
			|| lpObj->Class == 263 
			|| lpObj->Class == 271 
			|| lpObj->Class == 148
			|| lpObj->Class == 178
			|| lpObj->Class == 186
			|| lpObj->Class == 194
			|| lpObj->Class == 264
			|| lpObj->Class == 272
			|| lpObj->Class == 160
			|| lpObj->Class == 180
			|| lpObj->Class == 188
			|| lpObj->Class == 196
			|| lpObj->Class == 266
			|| lpObj->Class == 274
			|| lpObj->Class == 332
			|| lpObj->Class == 333
			|| lpObj->Class == 336
			|| lpObj->Class == 337)
		{
			gObjMonsterMagicAdd(lpObj, 17, 1);
		}

		if ( lpObj->Class == 161 || lpObj->Class == 181 || lpObj->Class == 189 || lpObj->Class == 197 || lpObj->Class == 267 || lpObj->Class == 275 )
		{
			gObjMonsterMagicAdd(lpObj, 200, 1);
			gObjMonsterMagicAdd(lpObj, 17, 1);
			gObjMonsterMagicAdd(lpObj, 1, 1);
			gObjMonsterMagicAdd(lpObj, 201, 1);
			gObjMonsterMagicAdd(lpObj, 202, 1);
			gObjMonsterMagicAdd(lpObj, 55, 1);
		}

		if (  lpObj->Class == 149 || lpObj->Class == 179 || lpObj->Class == 187 || lpObj->Class == 195 || lpObj->Class == 265 || lpObj->Class == 273 || lpObj->Class == 335 )
		{
			gObjMonsterMagicAdd(lpObj, 17, 1);
			gObjMonsterMagicAdd(lpObj, 1, 1);
		}

		if ( lpObj->Class == 66 || lpObj->Class == 73 || lpObj->Class == 77 || lpObj->Class == 89 || lpObj->Class == 95 || lpObj->Class == 112 || lpObj->Class == 118 || lpObj->Class == 124 || lpObj->Class == 130 || lpObj->Class == 143 || lpObj->Class == 163 || lpObj->Class == 165 || lpObj->Class == 167 || lpObj->Class == 169 || lpObj->Class == 171 || lpObj->Class == 173 || lpObj->Class == 427 )
		{
			gObjMonsterMagicAdd(lpObj, 17, 1);
		}
		
		if( lpObj->Class == 89 || lpObj->Class == 95 || lpObj->Class == 112 || lpObj->Class == 118 || lpObj->Class == 124 || lpObj->Class == 130 || lpObj->Class == 143 || lpObj->Class == 433 )
		{
			gObjMonsterMagicAdd(lpObj, 3, 1);
		}

		if ( lpObj->Class == 561 )
		{
			gObjMonsterMagicAdd(lpObj, 238, 1);
			gObjMonsterMagicAdd(lpObj, 237, 1);
			gObjMonsterMagicAdd(lpObj, 38, 1);
			gObjMonsterMagicAdd(lpObj, 9, 1);
		}

		if ( lpObj->Class == 673 )
		{
			gObjMonsterMagicAdd(lpObj, 622, 1);
		}
	}

	CreateFrustrum(lpObj->X, lpObj->Y, lpObj->m_Index);

	MapC[lpObj->MapNumber].SetStandAttr(lpObj->X, lpObj->Y);
	lpObj->m_OldX = lpObj->X;
	lpObj->m_OldY = lpObj->Y;
	lpObj->LastAttackerID = -1;

	return true;
}

short gObjAddSearch(SOCKET aSocket, char* ip)
{
	int count;
	int totalcount = 0;

	if ( gDisconnect == 1 )
	{
		return -1;
	}

	if (gObjTotalUser > g_ConfigRead.server.GetObjectMaxUser())
	{
		GSProtocol.GCJoinSocketResult(4, aSocket);
		return -1;
	}

	count = gObjCount;
	
	while ( true )
	{
		if ( gObj[count].Connected == PLAYER_EMPTY )
		{
			return count;
		}

		count++;

		if ( count >= g_ConfigRead.server.GetObjectMax() )
		{
			count = g_ConfigRead.server.GetObjectStartUserIndex();
		}
		totalcount++;
		if ( totalcount >= g_ConfigRead.server.GetObjectMaxUser() )
		{
			break;
		}
	}
	return -1;
}

short gObjAdd(SOCKET aSocket, char* ip, int aIndex)
{
	if ( ObjectMaxRange(aIndex) == FALSE )
	{
		return -1;
	}

	if ( gObj[aIndex].Connected != PLAYER_EMPTY )
	{
		return -1;
	}
	
	gObjCharZeroSet(aIndex);
	gObj[aIndex].LoginMsgSnd = FALSE;
	gObj[aIndex].LoginMsgCount = 0;
	memset(gObj[aIndex].AccountID, 0, sizeof(gObj[0].AccountID)-1);
	gObj[aIndex].m_Index = aIndex;
	gObj[aIndex].m_socket = aSocket;
	gObj[aIndex].ConnectCheckTime = GetTickCount();
	gObj[aIndex].AutoSaveTime = gObj[aIndex].ConnectCheckTime;
	gObj[aIndex].Connected = PLAYER_CONNECTED;
	gObj[aIndex].CheckSpeedHack = false;
	gObj[aIndex].LoginMsgCount = 0;
	gObj[aIndex].EnableCharacterCreate = 0;
	gObj[aIndex].Type = OBJ_USER;
	m_ObjBill[aIndex- g_ConfigRead.server.GetObjectStartUserIndex()].Init();
	strcpy(gObj[aIndex].m_PlayerData->Ip_addr, ip);
	g_Log.Add("Connection Accept : [%d][%s]", aIndex, ip);
	gObjCount++;
	
	if ( gObjCount >= g_ConfigRead.server.GetObjectMax() )
	{
		gObjCount = g_ConfigRead.server.GetObjectStartUserIndex();
	}

	return aIndex;
}

short gObjMonsterRecall(int iMapNumber)
{
	int number = gObjAddMonster(iMapNumber);

	if ( number < 0 )
	{
		return -1;
	}
	return -1;
}

short gObjAddMonster(int iMapNumber)
{
	bool bCheckCanMove = true;

	if (g_ConfigRead.server.GetServerType() == SERVER_MARKET || g_ConfigRead.server.GetServerType() == SERVER_BATTLECORE)
	{
		if (IT_MAP_RANGE(iMapNumber) || ITL_MAP_RANGE(iMapNumber) || iMapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL || iMapNumber == MAP_INDEX_DEVILSQUARE_FINAL || CC_MAP_RANGE(iMapNumber))
		{
			bCheckCanMove = false;
		}
	}

	if (bCheckCanMove == true)
	{
		if (g_MapServerManager.CheckMapCanMove(iMapNumber) == FALSE)
		{
			return -1;
		}
	}

	int count;
	int totalcount=0;

	count = gObjMonCount;

	while ( true )
	{
		if ( gObj[count].Connected == PLAYER_EMPTY )
		{
			gObjCharZeroSet(count);
			gObj[count].m_Index = count;
			gObj[count].Connected = PLAYER_CONNECTED;
			gObj[count].MapNumber = iMapNumber;
			gObjMonCount++;

			if ( gObjMonCount >= g_ConfigRead.server.GetObjectMaxMonster() )
			{
				gObjMonCount = 0;
			}
			
		return count;

		}

		count ++;
		
		if ( count >= g_ConfigRead.server.GetObjectMaxMonster() )
		{
			count = 0;
		}

		totalcount++;

		if ( totalcount >= g_ConfigRead.server.GetObjectMaxMonster() )
		{
			g_Log.Add("error: Cannnot add any more. %s %d", __FILE__, __LINE__ );
			return -1;
		}
	}
	return -1;
}





short gObjAddCallMon()
{
	int count;
	int totalcount = 0;

	count = gObjCallMonCount;

	while ( true )
	{
		if ( gObj[count].Connected == PLAYER_EMPTY )
		{
			gObjCharZeroSet(count);
			gObj[count].m_Index = count;
			gObj[count].Connected = PLAYER_CONNECTED;
			gObjCallMonCount++;

			if ( gObjCallMonCount >= g_ConfigRead.server.GetObjectStartUserIndex() )
			{
				gObjCallMonCount = g_ConfigRead.server.GetObjectMaxMonster();

			}
			return count;
		}

		count++;

		if ( count >= g_ConfigRead.server.GetObjectStartUserIndex() )
		{
			count = g_ConfigRead.server.GetObjectMaxMonster();
		}

		totalcount++;

		if ( totalcount >= g_ConfigRead.server.GetObjectMaxCallMonster() )
		{
			g_Log.Add("error: Cannnot add any more %s %d", __FILE__, __LINE__);
			return -1;
		}
	}

	return -1;

}





void gObjUserKill(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if ( lpObj->CloseCount < 1 )
	{
		lpObj->CloseCount = 6;
		lpObj->CloseType = 0;
	}
}




void gObjAllLogOut()
{
	gDisconnect = 1;
	gObjAllDisconnect();
	Sleep(1000);
	g_Log.AddC(TColor::Red, "Whole connection closed");
}




void gObjAllDisconnect()
{
	for ( int n= g_ConfigRead.server.GetObjectStartUserIndex(); n< g_ConfigRead.server.GetObjectMax(); n++)
	{
		if ( gObj[n].Connected > PLAYER_EMPTY )
		{
			if ( gObj[n].Type == OBJ_USER )
			{
				if(gObj[n].m_bOff != false)
				{
					GJSetCharacterInfo(&gObj[n], n, FALSE);
					gObj[n].m_bOff = false;
					gObjDel(n);
					
				}
				
				if (gDisconnect == TRUE)
				{
					GSProtocol.GCSendDisableReconnect(n);
				}
				else
				{
					IOCP.CloseClient(n);
				}
			}
		}
	}
}




void gObjTradeSave(LPOBJ lpObj, int index)
{
	if ( lpObj->m_IfState.use != 0 && lpObj->m_IfState.type == 1 )
	{
		int tObjNum = lpObj->TargetNumber;

		if ( tObjNum > 0 )
		{
			char szTemp[256];

			GSProtocol.CGTradeResult(tObjNum, 0);
			wsprintf(szTemp, Lang.GetText(0,14), lpObj->Name);
			GSProtocol.GCServerMsgStringSend((char*)szTemp, tObjNum, 1);
			gObjTradeCancel(tObjNum);
			g_Log.Add("Opposite canceled trade: [%s][%s]", gObj[tObjNum].AccountID, gObj[tObjNum].Name);
		}

		gObjTradeCancel( index );
		g_Log.Add("Cancel trade: [%s][%s]", lpObj->AccountID, lpObj->Name);
	}
}




void gObjBillRequest(LPOBJ lpObj)
{
	char szMsg[128];

	if ( m_ObjBill[lpObj->m_Index- g_ConfigRead.server.GetObjectStartUserIndex()].GetPayCode() == 0 )
	{
		if ( m_ObjBill[lpObj->m_Index- g_ConfigRead.server.GetObjectStartUserIndex()].GetCertify() == 0 )
		{
			wsprintf(szMsg, Lang.GetText(0,22));
		}

		else if ( m_ObjBill[lpObj->m_Index- g_ConfigRead.server.GetObjectStartUserIndex()].GetCertify() == 1 )
		{
			if ( m_ObjBill[lpObj->m_Index- g_ConfigRead.server.GetObjectStartUserIndex()].GetEndTime() != 0 )
			{
				wsprintf(szMsg, Lang.GetText(0,23), m_ObjBill[lpObj->m_Index- g_ConfigRead.server.GetObjectStartUserIndex()].GetEndTime(), g_VipSystem.GetVipName(lpObj->m_PlayerData->VipType));
			}
		}
	}
	else if ( m_ObjBill[lpObj->m_Index- g_ConfigRead.server.GetObjectStartUserIndex()].GetPayCode() == 1 )
	{
		char szTemp[20];
		szTemp[4] = 0;
		strncpy(szTemp, m_ObjBill[lpObj->m_Index- g_ConfigRead.server.GetObjectStartUserIndex()].GetEndsDays(), 4);
		int Day = atoi(szTemp);

		strncpy(szTemp, m_ObjBill[lpObj->m_Index- g_ConfigRead.server.GetObjectStartUserIndex()].GetEndsDays()+4, 2);
		szTemp[2] = 0;
		int Month = atoi(szTemp);

		strncpy(szTemp, m_ObjBill[lpObj->m_Index- g_ConfigRead.server.GetObjectStartUserIndex()].GetEndsDays()+6, 2);
		szTemp[2] = 0;
		int Year = atoi(szTemp);

		if ( m_ObjBill[lpObj->m_Index- g_ConfigRead.server.GetObjectStartUserIndex()].GetCertify() == 0 )	// Account Based
		{
			wsprintf(szMsg, Lang.GetText(0,24), Day, Month, Year, g_VipSystem.GetVipName(lpObj->m_PlayerData->VipType));
		}
		else if ( m_ObjBill[lpObj->m_Index- g_ConfigRead.server.GetObjectStartUserIndex()].GetCertify() == 1 )	// IP Based
		{
			wsprintf(szMsg, Lang.GetText(0,25), Day, Month, Year, g_VipSystem.GetVipName(lpObj->m_PlayerData->VipType));
		}
	}
	else if ( m_ObjBill[lpObj->m_Index- g_ConfigRead.server.GetObjectStartUserIndex()].GetPayCode() == 3)
	{
		char szYear[5] = "";
		char szMonth[3] = "";
		char szDay[3] = "";
		char szHour[3] = "";
		char szMin[3] = "";

		strncpy(szYear, m_ObjBill[lpObj->m_Index- g_ConfigRead.server.GetObjectStartUserIndex()].GetEndsDays(), 4);
		strncpy(szMonth, m_ObjBill[lpObj->m_Index- g_ConfigRead.server.GetObjectStartUserIndex()].GetEndsDays()+4, 2);
		strncpy(szDay, m_ObjBill[lpObj->m_Index- g_ConfigRead.server.GetObjectStartUserIndex()].GetEndsDays()+6, 2);
		strncpy(szHour, m_ObjBill[lpObj->m_Index- g_ConfigRead.server.GetObjectStartUserIndex()].GetEndsDays()+8, 2);
		strncpy(szMin, m_ObjBill[lpObj->m_Index- g_ConfigRead.server.GetObjectStartUserIndex()].GetEndsDays()+10, 2);

		wsprintf(szMsg, Lang.GetText(0,148), m_ObjBill[lpObj->m_Index- g_ConfigRead.server.GetObjectStartUserIndex()].GetEndTime(), szYear, szMonth,
			szDay, szHour, szMin);

		g_Log.Add("[%s][%s] BillType : (Time) RemainPoint : (%d)",
			lpObj->AccountID, lpObj->Name, m_ObjBill[lpObj->m_Index- g_ConfigRead.server.GetObjectStartUserIndex()].GetEndTime(), g_VipSystem.GetVipName(lpObj->m_PlayerData->VipType));
	}
	else if ( m_ObjBill[lpObj->m_Index- g_ConfigRead.server.GetObjectStartUserIndex()].GetPayCode() == 4)
	{
		char szYear[5] = "";
		char szMonth[3] = "";
		char szDay[3] = "";

		strncpy(szYear, m_ObjBill[lpObj->m_Index- g_ConfigRead.server.GetObjectStartUserIndex()].GetEndsDays(), 4);
		strncpy(szMonth, m_ObjBill[lpObj->m_Index- g_ConfigRead.server.GetObjectStartUserIndex()].GetEndsDays()+4, 2);
		strncpy(szDay, m_ObjBill[lpObj->m_Index- g_ConfigRead.server.GetObjectStartUserIndex()].GetEndsDays()+6, 2);

		wsprintf(szMsg, Lang.GetText(0,149),  szYear, szMonth,	szDay, g_VipSystem.GetVipName(lpObj->m_PlayerData->VipType));

		g_Log.Add("[%s][%s] BillType : (Date) RemainDate : (%s-%s-%s)",
			lpObj->AccountID, lpObj->Name, szYear, szMonth, szDay);
	}
	else if ( m_ObjBill[lpObj->m_Index-g_ConfigRead.server.GetObjectStartUserIndex()].GetPayCode() == 5)		// FREE
	{
		wsprintf(szMsg, Lang.GetText(0,150));
		g_Log.Add("[%s][%s] BillType : (NoCharge)", lpObj->AccountID, lpObj->Name);
	}
	else	// Pospaid Account
	{
		wsprintf(szMsg, Lang.GetText(0,26), g_VipSystem.GetVipName(lpObj->m_PlayerData->VipType));
	}

	g_Log.Add(szMsg);
	GSProtocol.GCServerMsgStringSend(szMsg, lpObj->m_Index, 1);
}




short gObjMemFree(int index)
{
	int i = 0;
	LPOBJ lpObj;

	if ( gDisconnect == TRUE )
	{
		return -1;
	}

	if ( index < 0 || index > g_ConfigRead.server.GetObjectMax() )
	{
		g_Log.Add("(%s)(%d) = index over error (%d)", __FILE__, __LINE__, index);
		return false;
	}

	lpObj = &gObj[index];

	if ( lpObj->Connected < PLAYER_CONNECTED )
	{
		return false;
	}

	gObjTradeSave(lpObj, index);

	if ( lpObj->Connected >= PLAYER_LOGGED )
	{
		if ( lpObj->Type == OBJ_USER )
		{
			if ( lpObj->Connected == PLAYER_PLAYING )
			{
				if ( BC_MAP_RANGE(lpObj->MapNumber) )
				{
					g_BloodCastle.SearchUserDropQuestItem(lpObj->m_Index);
				}

				if (IT_MAP_RANGE(lpObj->MapNumber))
				{
					g_IT_Event.DropRelicsItem(lpObj->MapNumber, lpObj->m_Index);
					g_IT_Event.Leave_ITR(lpObj->m_Index, lpObj->MapNumber);
					
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

				if (lpObj->MapNumber == MAP_INDEX_DEVILSQUARE_FINAL)
				{
					g_DevilSquareFinal.Leave_DSF(lpObj->m_Index);
				}

				if ( DG_MAP_RANGE(lpObj->MapNumber) )
				{
					g_DoppelGanger.LeaveDoppelganger(lpObj->m_Index);

					if ( g_DoppelGanger.GetDoppelgangerState() == 2 )
					{
						g_DoppelGanger.SendDoppelgangerResult(lpObj, 1);
					}
				}

				GJSetCharacterInfo(lpObj, index, 0);
			}
			
			GJPUserClose(lpObj);
		}
	}

	gObjViewportClose(lpObj);
	lpObj->Connected = PLAYER_EMPTY;
	return 1;
}





BOOL gObjGameClose(int aIndex)
{
	if ( aIndex < g_ConfigRead.server.GetObjectStartUserIndex() || aIndex > g_ConfigRead.server.GetObjectMax() )	// Deathway fix
	{
		g_Log.Add("(%s)(%d) = index over error (%d)", __FILE__, __LINE__, aIndex);

		return FALSE;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if ( lpObj->Connected != PLAYER_PLAYING )
	{
		return FALSE;
	}

	if ( lpObj->Type != OBJ_USER )
	{
		g_Log.Add("Invalid character type causes failure to close (%s/%s)", lpObj->AccountID, lpObj->Name);
		return FALSE;
	}

	if ( lpObj->m_RecallMon >= 0 )
	{
		gObjMonsterCallKill(lpObj->m_Index);
	}

	gObjMagicTextSave(lpObj);

	if ( (   (lpObj->m_IfState.use != 0 )
		&&  (lpObj->m_IfState.type == 7 )) 
		|| ((lpObj->m_IfState.use != 0)  
		&& (lpObj->m_IfState.type == 13)))
	{
		if ( lpObj->bIsChaosMixCompleted == true || lpObj->m_bIsCastleNPCUpgradeCompleted == true)
		{
			gObjInventoryCommit(aIndex);
		}
		else
		{
			gObjInventoryRollback(aIndex);
		}
	}

	gObjTradeSave(lpObj, aIndex);

	if ( (lpObj->m_IfState.use != 0 ) && (lpObj->m_IfState.type == 10))
	{
		if (lpObj->TargetNpcNumber != -1 && gObj[lpObj->TargetNpcNumber].Class == 229)
		{
			gQeustNpcTeleport.TalkRefDel();
		}
	}

	if ( lpObj->m_IfState.use != 0)
	{
		if (lpObj->TargetNpcNumber != -1 && gObj[lpObj->TargetNpcNumber].Class == 568)
		{
			gQuestExpNpcTeleport.TalkRefDel();
		}
	}

	if ( (lpObj->Authority&2) == 2 )
	{
		cManager.ManagerDel(lpObj->Name);
	}

	g_NewPVP.Reset(*lpObj);
	gObjResetDuel(lpObj);

	if ( lpObj->PartyNumber >= 0 )
	{
		int iPartyIndex= gParty.GetIndex(lpObj->PartyNumber, lpObj->m_Index, lpObj->DBNumber);

		if ( iPartyIndex >= 0 )
		{
			PMSG_PARTYDELUSER pMsg;

			pMsg.Number = iPartyIndex;
			GSProtocol.CGPartyDelUser(&pMsg, lpObj->m_Index, 0);
		}
	}

	if ( lpObj->m_PlayerData->GuildNumber > 0 )
	{
		if ( lpObj->m_PlayerData->lpGuild->WarState == 1 )
		{
			gObjGuildWarMasterClose(lpObj);
		}

		GDCharClose(0, lpObj->m_PlayerData->lpGuild->Name, lpObj->Name);
	}
	else
	{
		GDCharClose(1, NULL, lpObj->Name);
	}

	if (lpObj->MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL)
	{
		lpObj->m_bCCF_UI_Using = false;
		lpObj->m_byCCF_CanEnter = 0;
		lpObj->m_nCCF_CertiTick = 0;
		lpObj->m_nCCF_UserIndex = -1;
		lpObj->m_nCCF_BlowTime = 0;
		lpObj->m_byKillUserCount = 0;
		lpObj->m_byKillMobCount = 0;
		lpObj->m_bCCF_Quit_Msg = false;
	}

	if ( DS_MAP_RANGE(lpObj->MapNumber) )
	{
		g_DevilSquare.DelUser(lpObj->m_bDevilSquareIndex, aIndex);	
	}

	if ( BC_MAP_RANGE(lpObj->MapNumber) )
	{
		g_BloodCastle.SearchUserDropQuestItem(lpObj->m_Index);
	}

	if ( IT_MAP_RANGE(lpObj->MapNumber) )
	{
		g_IT_Event.DropRelicsItem(lpObj->MapNumber, lpObj->m_Index);
		g_IT_Event.Leave_ITR(lpObj->m_Index, lpObj->MapNumber);

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

	if (lpObj->MapNumber == MAP_INDEX_DEVILSQUARE_FINAL)
	{
		g_DevilSquareFinal.Leave_DSF(lpObj->m_Index);
	}

	if ( DG_MAP_RANGE(lpObj->MapNumber) )
	{
		g_DoppelGanger.LeaveDoppelganger(aIndex);

		if ( g_DoppelGanger.GetDoppelgangerState() == 2 )
		{
			g_DoppelGanger.SendDoppelgangerResult(lpObj, 1);
		}
	}

	if( lpObj->Type == OBJ_USER && lpObj->MapNumber == g_LastManStanding.m_Cfg.iPVPMap && lpObj->m_PlayerData->RegisterdLMS == 1)
	{
		g_LastManStanding.UserQuit(lpObj->m_Index);
	}

	gObjSaveChaosBoxItemList(lpObj);

	if ( gObjFixInventoryPointer(aIndex) == false )
	{
		g_Log.Add("[Fix Inv.Ptr] False Location - %s, %d", __FILE__, __LINE__);
	}

	g_PeriodItemEx.RemovePeriodInfo(lpObj);
	g_CMuunSystem.RemoveMuunItemPeriodInfo(lpObj);
	g_CMuRummyMng.GDReqMuRummyInfoUpdate(lpObj);

	if (lpObj->m_PlayerData->m_bIsMining)
	{
		g_MineSystem.CheckIsUPTUserWhenDisconnected(aIndex);
	}

	GJSetCharacterInfo(lpObj, aIndex, 0);
	gObjViewportClose(lpObj);
	g_Log.Add("(%d)(%s)(%s) Character closed", lpObj->m_Index, lpObj->AccountID, lpObj->Name);

	if(lpObj->Name)
	{
		WhisperCash.DelCash(lpObj->Name);
	}

	gObjClearBuffEffect(lpObj, CLEAR_TYPE_LOGOUT);

	memcpy(lpObj->BackName, lpObj->Name, sizeof(lpObj->Name)-1);
	lpObj->BackName[10] = 0;

	lpObj->Connected = PLAYER_LOGGED;
	lpObj->m_PlayerData->m_bUsePartyMatching = false;
	lpObj->m_PlayerData->m_bPartyMatchingLeader = false;
	lpObj->m_PlayerData->m_bUseGuildMatching = false;
	lpObj->m_PlayerData->m_bSendNpcPos = FALSE;

	if ( m_ObjBill[aIndex- g_ConfigRead.server.GetObjectStartUserIndex()].GetCertify() >= 0 && m_ObjBill[aIndex- g_ConfigRead.server.GetObjectStartUserIndex()].GetCertify() < 3)
	{
		gLCount[m_ObjBill[aIndex- g_ConfigRead.server.GetObjectStartUserIndex()].GetCertify()].Delete();
	}

	lpObj->m_nPhaseLordSilvester = 0;

	for (int k = 0; k < 3; k++)
		lpObj->m_nRecallMonIndex[k] = -1;

	return TRUE;
}





short gObjDel(int index)
{
	int i = 0;

	if ( index < 0 || index > g_ConfigRead.server.GetObjectMax() )
	{
		g_Log.Add("(%s)(%d) = index over error (%d)", __FILE__, __LINE__, index);
		return 0;
	}

	LPOBJ lpObj = &gObj[index];

	if ( lpObj->Connected < PLAYER_CONNECTED )
	{
		return 0;
	}

	if ( MAX_MAP_RANGE(lpObj->MapNumber) )
	{
		MapC[lpObj->MapNumber].ClearStandAttr(lpObj->m_OldX, lpObj->m_OldY);
		MapC[lpObj->MapNumber].ClearStandAttr(lpObj->X, lpObj->Y);
	}

	if ( IMPERIAL_MAP_RANGE(lpObj->MapNumber) == TRUE )
	{
		g_Log.Add("[IMPERIALGUARDIAN_TEST] gObjDel() => Class:%d, Index:%d", lpObj->Class, index);
	}

	int UserConnectState = lpObj->Connected;

	if ( lpObj->Type == OBJ_USER && lpObj->m_PlayerData->ISBOT == false && lpObj->m_bOff == false)
	{
		gObjGameClose(index);

		if ( UserConnectState >= PLAYER_LOGGED )
		{
			if ( lpObj->m_bMapSvrMoveQuit == false && lpObj->m_bOff == false )
			{
				GJPUserClose(lpObj);
				g_Log.Add("(%d)Logout Request : %s [%s]", index, lpObj->AccountID, lpObj->m_PlayerData->Ip_addr);
			}
			else
			{
				g_Log.Add("[MapServerMng] User Quit without JoinServer logout-send [%s][%s]",
					lpObj->AccountID, lpObj->Name);
			}

			memset(lpObj->AccountID, 0, sizeof(lpObj->AccountID) -1);
			memset(lpObj->m_PlayerData->Password, 0, sizeof(lpObj->m_PlayerData->Password));
			memset(lpObj->m_PlayerData->HWID, 0, sizeof(lpObj->m_PlayerData->HWID));
		}
		g_Log.Add("(%d)Disconnected : %s [%s]", index, lpObj->AccountID, lpObj->m_PlayerData->Ip_addr);
	}

	if ( lpObj->Type == OBJ_USER && lpObj->m_PlayerData->ISBOT == false && lpObj->m_bOff == false )
	{
		lpObj->Connected = PLAYER_EMPTY;
	}

	else
	{
		lpObj->Connected = PLAYER_EMPTY;
	}

	if (lpObj->Type == OBJ_USER)
	{
		lpObj->m_PlayerData->m_bUsePartyMatching = false;
		lpObj->m_PlayerData->m_bPartyMatchingLeader = false;
		lpObj->m_PlayerData->m_nServerChannel = -1;
		lpObj->m_PlayerData->m_nPartyMatchingIndex = -1;
		lpObj->m_PlayerData->m_bSendNpcPos = FALSE;
		lpObj->m_PlayerData->VipType = 0;
		lpObj->m_bPShopOpen = false;
		if(g_OffLevel.FindUser(i) != -1)
		{
			g_OffLevel.DeleteUser(lpObj->m_Index);
		}
	}

	lpObj->m_bGMSummon = false;
	lpObj->m_nPhaseLordSilvester = 0;

	for (int k = 0; k < 3; k++)
		lpObj->m_nRecallMonIndex[k] = -1;
	
	return 1;
}




short gObjSometimeClose(int index)
{
	if ( gDisconnect == 1 )
	{
		return -1;
	}

	if ( index < 0 || index > g_ConfigRead.server.GetObjectMax() )	// deathway fix needed
	{
		g_Log.Add("(%s)(%d) = index over error (%d)", __FILE__, __LINE__, index);
		return 0;
	}

	LPOBJ lpObj = &gObj[index];
	gObjTradeSave(lpObj, index);

	if ( lpObj->Connected > PLAYER_CONNECTED )
	{
		if ( lpObj->Type == OBJ_USER )
		{
			if ( lpObj->Connected == PLAYER_PLAYING )
			{
				if ( lpObj->PartyNumber >= 0 )
				{
					PMSG_PARTYDELUSER pMsg;
					int iPartyIndex = gParty.GetIndex(lpObj->PartyNumber, lpObj->m_Index, lpObj->DBNumber);

					if ( iPartyIndex >= 0 )
					{
						pMsg.Number = iPartyIndex;
						GSProtocol.CGPartyDelUser(&pMsg, lpObj->m_Index, 0);
					}
				}
			}
		}
	}

	return 1;
}





BOOL gObjIsGamePlaing(LPOBJ lpObj)
{
	if ( lpObj->Connected != PLAYER_PLAYING )
	{
		return false;
	}

	return TRUE;
}



BOOL gObjIsConnectedGP(int aIndex)
{
	if (aIndex < 0 || aIndex > g_ConfigRead.server.GetObjectMax()-1 )
	{
		return FALSE;
	}
	
	if ( gObj[aIndex].Type != OBJ_USER )
	{
		return FALSE;
	}

	if ( gObj[aIndex].Connected != PLAYER_PLAYING )
	{
		return FALSE;
	}

	if ( gObj[aIndex].CloseCount > 0 )
	{
		return FALSE;
	}

	return TRUE;
}



BOOL gObjIsConnectedGP(int aIndex, char * CharName)
{
	if (aIndex < 0 || aIndex > g_ConfigRead.server.GetObjectMax()-1 )
	{
		return FALSE;
	}
	
	if ( gObj[aIndex].Type != OBJ_USER )
	{
		return FALSE;
	}

	if ( gObj[aIndex].Connected != PLAYER_PLAYING )
	{
		return FALSE;
	}

	if ( gObj[aIndex].CloseCount > 0 )
	{
		return FALSE;
	}

	if ( strcmp(gObj[aIndex].Name, CharName) != 0 )
	{
		return FALSE;
	}

	return TRUE;
}





BOOL gObjIsConnected(LPOBJ lpObj)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return FALSE;
	}

	if ( lpObj->Connected != PLAYER_PLAYING )
	{
		return FALSE;
	}

	if ( lpObj->CloseCount > 0 )
	{
		return FALSE;
	}

	return TRUE;
}





BOOL gObjIsConnected(int aIndex) 
{
	if ( aIndex < 0 || aIndex > g_ConfigRead.server.GetObjectMax()-1)
	{
		return FALSE;
	}
	if ( gObj[aIndex].Connected != PLAYER_PLAYING )
	{
		return FALSE;
	}

	return TRUE;
}




BOOL gObjIsConnected(LPOBJ lpObj, int dbnumber)
{
	if (lpObj->Connected < PLAYER_PLAYING)
	{
		return 0;
	}

	if (lpObj->DBNumber != dbnumber)
	{
		return 0;
	}

	return 1;
}

BOOL gObjIsConnectedEx(int aIndex)
{
	if( aIndex < 0 || aIndex > g_ConfigRead.server.GetObjectMax()-1 ) return FALSE;
	LPOBJ lpObj = &gObj[aIndex];
	if( lpObj->Type != OBJ_USER)	return FALSE;
	if( lpObj->Connected != PLAYER_PLAYING ) return FALSE;
	if( lpObj->CloseCount >= 0 )	return FALSE;
	return TRUE;
}

BOOL gObjIsAccontConnect(int aIndex, char* accountid)
{
	if (aIndex < g_ConfigRead.server.GetObjectStartUserIndex() || aIndex > g_ConfigRead.server.GetObjectMax()-1)
	{
		return 0;
	}

	if (gObj[aIndex].Connected < PLAYER_CONNECTED)
	{
		return 0;
	}

	if (strlen(&gObj[aIndex].AccountID[0]) < 1 )
	{
		return 0;
	}

	if ( strcmp(accountid, &gObj[aIndex].AccountID[0]) != 0)
	{
		return 0;
	}
	return 1;
}






BOOL gObjPasswordCheck(int aIndex, char* szInPassword)
{
	if(gObj[aIndex].Type != OBJ_USER)
	{
		return FALSE;
	}

	if(strcmp(gObj[aIndex].m_PlayerData->Password, szInPassword) == 0)
	{
		return TRUE;
	}

	else
	{
		return FALSE;
	}
}

BOOL gObjCheckXYMapTile(LPOBJ lpObj, int iDbgName)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return false;
	}

	if ( (lpObj->Authority&2 ) == 2 )
	{
		return FALSE;
	}

	if ( CC_MAP_RANGE(lpObj->MapNumber) )
	{
		if ( g_ChaosCastle.GetCurrentState(g_ChaosCastle.GetChaosCastleIndex(lpObj->MapNumber)) == 2 )
		{
			return FALSE;
		}
	}

	if (lpObj->MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL)
	{
		if (g_ConfigRead.server.GetServerType() == SERVER_BATTLECORE)
		{
			if (g_ChaosCastleFinal.GetCurrentState() == 2)
			{
				return FALSE;
			}
		}
	}

	int x = lpObj->X;
	int y = lpObj->Y;
	int mapnumber = lpObj->MapNumber;

	if ( mapnumber > MAX_NUMBER_MAP-1 )
	{
		mapnumber = MAP_INDEX_RORENCIA;
	}

	if ( lpObj->MapNumber != MAP_INDEX_LORENMARKET && (lpObj->MapNumber != MAP_INDEX_ARCA_WAR || lpObj->MapNumber != MAP_INDEX_DEBENTER_ARCA_WAR || g_ArcaBattle.IsArcaBattleServer() == FALSE) && lpObj->MapNumber != MAP_INDEX_ACHERON && lpObj->MapNumber != MAP_INDEX_DEBENTER )
	{
		if ( lpObj->Class == 2 && lpObj->Level < 10 )
		{
			if ( lpObj->MapNumber != MAP_INDEX_NORIA )
			{
				g_Log.Add("%s %d %d %d Invalid location causes to force to move", lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y);
				PMSG_TELEPORT pMsg;
				pMsg.MoveNumber = 27;
				gObjMoveGate(lpObj->m_Index, pMsg.MoveNumber);
			}
		}
		
		else
		{
			if ( lpObj->Level < 0 )
			{
				g_Log.Add("%s %d %d %d Invalid location causes to force to move", lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y);
				PMSG_TELEPORT pMsg;
				pMsg.MoveNumber = 17;
				gObjMoveGate(lpObj->m_Index, pMsg.MoveNumber);
			}
		}
	}

	BYTE attr = MapC[mapnumber].GetAttr(x, y);

	if ( (attr&4) == 4 || (attr&8) == 8 )
	{
		BYTE attr2 = MapC[mapnumber].GetAttr(x+2, y);
		BYTE attr3 = MapC[mapnumber].GetAttr(x, y+2);
		BYTE attr4 = MapC[mapnumber].GetAttr(x-2, y);
		BYTE attr5 = MapC[mapnumber].GetAttr(x, y-2);

		if ( attr2 > 1 && attr3 > 1 && attr4 > 1 && attr5 > 1 )
		{
			g_Log.Add("[ CHECK POSITION ] DbgName[%d] [%s][%s] Map[%d]-(%d,%d) Invalid location causes to force to move",iDbgName,lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y);
			
			PMSG_TELEPORT pMsg;

			if (IT_MAP_RANGE(lpObj->MapNumber))
			{
				if (g_IT_Event.GetIllusionTempleState(lpObj->MapNumber) == 2)
				{
					BYTE btTeam = g_IT_Event.GetUserTeam(lpObj->MapNumber, lpObj->m_Index);

					if (btTeam == 0xFF)
					{
						pMsg.MoveNumber = 17;
					}

					else if (btTeam == 1)
					{
						pMsg.MoveNumber = lpObj->MapNumber + 109;
					}

					else if (btTeam == 0)
					{
						pMsg.MoveNumber = lpObj->MapNumber + 103;
					}

					else
					{
						pMsg.MoveNumber = 17;
					}
				}

				else
				{
					pMsg.MoveNumber = 17;
				}

				gObjMoveGate(lpObj->m_Index, pMsg.MoveNumber);
				return TRUE;
			}

			else if (lpObj->MapNumber == MAP_INDEX_DEVILSQUARE_FINAL && g_DevilSquareFinal.m_btDSFCurState == 2)
			{
				if (g_DevilSquareFinal.GetUserTeam(lpObj->m_Index) == -1)
				{
					pMsg.MoveNumber = 17;
				}

				else
				{
					pMsg.MoveNumber = 474;
				}

				gObjMoveGate(lpObj->m_Index, pMsg.MoveNumber);
				return TRUE;
			}

			else
			{
				gObjMoveGate(lpObj->m_Index, 17);
				return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL gObjSetAccountLogin(int aIndex, char * szId, int aUserNumber, int aDBNumber, char* Password)
{
	if ( !ObjectMaxRange(aIndex) )
	{
		g_Log.Add("error: %s %d", __FILE__, __LINE__);
		return 0;
	}

	if ( gObj[aIndex].Connected != PLAYER_CONNECTED )
	{
		g_Log.Add("error-L2: [%d] User(%s) is not connected. DBNumber: %d %s %d", aIndex, szId, aDBNumber, __FILE__, __LINE__);
		return 0;
	}

	if ( strcmp(gObj[aIndex].AccountID, szId) != 0 )
	{
		g_Log.Add("error: %s did not request to authenticate.", szId);
		return 0;
	}

	for(int i=g_ConfigRead.server.GetObjectStartUserIndex();i<g_ConfigRead.server.GetObjectMax();i++)
	{
		if(!strcmpi(szId,gObj[i].AccountID))
		{
			if(gObj[i].m_bOff)
			{
				gObj[i].m_bOff = false;
				gObjDel(i);
				break;
			}
		}
	}

	gObj[aIndex].UserNumber = aUserNumber;
	gObj[aIndex].DBNumber = aDBNumber;
	gObj[aIndex].Connected = PLAYER_LOGGED;
	strcpy(gObj[aIndex].m_PlayerData->Password, Password);

	GSProtocol.GCJoinBillCheckSend(gObj[aIndex].AccountID, aIndex);
	return TRUE;

}





BOOL gObjGetUserNumber(int aIndex)
{
	int usernumber;

	if ( !ObjectMaxRange(aIndex))
	{
		g_Log.Add("error : %s %d",__FILE__, __LINE__);
		return 0;
	}

	usernumber=gObj[aIndex].UserNumber;
	return usernumber;
}





BOOL gObjGetNumber(int aIndex, int& UN, int& DBN)
{
	if ( aIndex < 0 || aIndex > g_ConfigRead.server.GetObjectMax()-1)
	{
		g_Log.Add("error : %s %d",__FILE__, __LINE__);
		return 0;
	}
	UN=gObj[aIndex].UserNumber;
	DBN=gObj[aIndex].DBNumber;
	return 1;
}





// gObj Address 653DBF8
char* gObjGetAccountId(int aIndex)
{
	if ( aIndex < 0 || aIndex > g_ConfigRead.server.GetObjectMax()-1)
	{
		g_Log.Add("error : %s %d",__FILE__, __LINE__);
		return 0;
	}

	if ( gObj[aIndex].Connected == 2)
	{
		return &gObj[aIndex].AccountID[0];
	}
	return 0;
}





int gObjGetIndex(char* szId)
{
	for (int n = g_ConfigRead.server.GetObjectStartUserIndex(); n < g_ConfigRead.server.GetObjectMax(); n++)
	{
		if (gObj[n].Connected >= PLAYER_PLAYING)
		{
			if (gObj[n].Name[0] == *szId)
			{
				if (strcmp(&gObj[n].Name[0], szId) == 0)
				{
					return n;
				}
			}
		}
	}
	return -1;
}





BOOL gObjUserIdConnectCheck(char* szId,int index)
{
	if ( index < 0 || index > g_ConfigRead.server.GetObjectMax())	// Deathway Fix Here
	{
		g_Log.Add("error : index error !! %s %d",__FILE__, __LINE__);
		return 0;
	}

	if ( gObj[index].Connected != PLAYER_PLAYING )
	{
		return 0;
	}

	if (gObj[index].Name[0] != *szId)
	{
		return 0;
	}

	if ( strcmp(gObj[index].Name , szId) == 0)
	{
		return 1;
	}
	return 0;
}





BYTE GetPathPacketDirPos(int px, int py)
{
	BYTE pos = 0;

	if (px <= -1 && py <= -1)
	{
		pos=0;
	}
	else if (px <= -1 && py == 0)
	{
		pos=7;
	}
	else if ( px <= -1 && py >= 1)
	{
		pos=6;
	}
	else if ( px == 0 && py <= -1)
	{
		pos= 1;
	}
	else if ( px == 0 && py >= 1)
	{
		pos = 5;
	}
	else if ( px >= 1 && py <=-1)
	{
		pos=2;
	}
	else if ( px >= 1 && py == 0)
	{
		pos = 3;
	}
	else if ( px >=1  && py >= 1)
	{
		pos = 4;
	}

	return pos;
}





int  gObjCalDistance(LPOBJ lpObj1, LPOBJ lpObj2)
{
	if ( lpObj1 == NULL || lpObj2 == NULL )
	{
		return 0;
	}

	if ( lpObj1->X == lpObj2->X && lpObj1->Y == lpObj2->Y )
	{
		return 0;
	}

	float tx = lpObj1->X - lpObj2->X;
	float ty = lpObj1->Y - lpObj2->Y;

	return sqrt( (tx*tx)+(ty*ty) );
}





BOOL gObjPositionCheck(LPOBJ lpObj)
{
	int ix = lpObj->m_OldX - lpObj->TX;
	int iy = lpObj->m_OldY - lpObj->TY;

	if ( ix <= -15 ||  ix >= 15 )
	{
		return FALSE;
	}

	if ( iy <= -15 ||  iy >= 15 )
	{
		return FALSE;
	}

	return TRUE;
}





BOOL gObjCheckTileArea(int aIndex, int x, int y, int dis)
{
	LPOBJ lpObj = & gObj[aIndex];

	if ( x < (lpObj->X - dis) || x > (lpObj->X + dis) || y < (lpObj->Y - dis) || y > (lpObj->Y + dis) )
	{
		return FALSE;
	}
	return TRUE;
}





BOOL ExtentCheck(int x, int y, int w, int h)
{
	if ( (x >=0) && (x<w) && (y >=0) && (y<h) )
	{
		return 1;
	}
	return 0;
}

//00566240	-> 100% (1.01.00) [NEW]
BOOL InventoryExtentCheck(int x, int y, int w, int h)
{
	int nEndOfRange, nStartOfRange;
	// ----
	if (y < 0)
	{
		return false;
	}
	// ----
	if (y >= 8)	//-> Need rewrite too)
	{
		if (y >= 12)
		{
			if (y >= 16)
			{
				if (y >= 20)
				{
					if (y >= 24)
					{
						if (y >= 28)
						{
							return false;
						}
						// ----
						nStartOfRange = 24;
						nEndOfRange = 28;
					}
					else
					{
						nStartOfRange = 20;
						nEndOfRange = 24;
					}
				}
				else
				{
					nStartOfRange = 16;
					nEndOfRange = 20;
				}
			}
			else
			{
				nStartOfRange = 12;
				nEndOfRange = 16;
			}
		}
		else
		{
			nStartOfRange = 8;
			nEndOfRange = 12;
		}
	}
	else
	{
		nStartOfRange = 0;
		nEndOfRange = 8;
	}
	// ----
	if (nEndOfRange > h)
	{
		nEndOfRange = h;
	}
	// ----
	return x >= 0 && x < w && y >= nStartOfRange && y < nEndOfRange;
}

//00566370	-> 100% (1.01.00) [NEW]
BOOL CheckOutOfInventory(int aIndex, int sy, int height)
{
	int nEndofItemY = sy + height - 1;
	// ----

	if ( sy >= 4 * gObj[aIndex].m_PlayerData->m_InventoryExpansion + 8 && (sy < 24 || sy > 27) )
	{
		return -1;
	}

	if (sy < 0)
	{
		return false;
	}
	// ----
	if (sy >= 8) //-> Need rewrite xD
	{
		if (sy >= 12)
		{
			if (sy >= 16)
			{
				if (sy >= 20)
				{
					if (sy >= 24)
					{
						if (sy >= 28)
						{
							return false;
						}
						// ----
						if (nEndofItemY >= 28)
						{
							return false;
						}
					}
					else
					{
						if (nEndofItemY >= 24)
						{
							return false;
						}
					}
				}
				else
				{
					if (nEndofItemY >= 20)
					{
						return false;
					}
				}
			}
			else
			{
				if (nEndofItemY >= 16)
				{
					return false;
				}
			}
		}
		else
		{
			if (nEndofItemY >= 12)
			{
				return false;
			}
		}
	}
	else
	{
		if (nEndofItemY >= 8)
		{
			return false;
		}
	}
	// ----
	return true;
}

//005664f0	-> 100% (1.01.00) [NEW]
BOOL CheckOutOfWarehouse(int aIndex, int sy, int height)
{
	if (sy >= 15 * gObj[aIndex].m_PlayerData->m_WarehouseExpansion + 15)
	{
		return false;
	}
	// ----
	int nEndofItemY = sy + height - 1;
	// ----
	if (sy < 0)
	{
		return false;
	}
	// ----
	if (sy >= 15)
	{
		if (sy >= 30)
		{
			return false;
		}
		// ----
		if (nEndofItemY >= 30)
		{
			return false;
		}
	}
	else
	{
		if (nEndofItemY >= 15)
		{
			return false;
		}
	}
	// ----
	return true;
}

//00566440	-> 100% (1.01.00) [NEW]
BOOL WarehouseExtentCheck(int x, int y, int w, int h)
{
	int nEndOfRange, nStartOfRange;
	// ----
	if (y < 0)
	{
		return false;
	}
	// ----
	if (y >= 15)
	{
		if (y >= 30)
		{
			return false;
		}
		// ----
		nStartOfRange = 15;
		nEndOfRange = 30;
	}
	else
	{
		nStartOfRange = 0;
		nEndOfRange = 15;
	}
	// ----
	if (nEndOfRange > h)
	{
		nEndOfRange = h;
	}
	// ----
	return x >= 0 && x < w && y >= nStartOfRange && y < nEndOfRange;
}



void gObjSetInventory1Pointer(LPOBJ lpObj)
{
	lpObj->pInventory = lpObj->Inventory1;
	lpObj->pInventoryMap = lpObj->InventoryMap1;
	lpObj->pInventoryCount  = &lpObj->InventoryCount1;
}



void gObjSetInventory2Pointer(LPOBJ lpObj)
{
	lpObj->pInventory = lpObj->Inventory2;
	lpObj->pInventoryMap = lpObj->InventoryMap2;
	lpObj->pInventoryCount  = &lpObj->InventoryCount2;
}

void gObjAddMsgSend(LPOBJ lpObj, int aMsgCode, int aIndex, int  SubCode)
{
	for ( int n=0;n<MAX_MONSTER_SEND_MSG;n++)
	{
		if ( gSMMsg[lpObj->m_Index][n].MsgCode < 0 )
		{
			gSMMsg[lpObj->m_Index][n].MsgCode = aMsgCode;
			gSMMsg[lpObj->m_Index][n].MsgTime = GetTickCount64();
			gSMMsg[lpObj->m_Index][n].SendUser = aIndex;
			gSMMsg[lpObj->m_Index][n].SubCode = SubCode;

			return;
		}
	}

	for ( int n=0;n<MAX_MONSTER_SEND_MSG;n++)
	{
		if ( gSMMsg[lpObj->m_Index][n].MsgCode == 2 || gSMMsg[lpObj->m_Index][n].MsgCode == 6 || gSMMsg[lpObj->m_Index][n].MsgCode == 7 )
		{
			gSMMsg[lpObj->m_Index][n].MsgCode = aMsgCode;
			gSMMsg[lpObj->m_Index][n].MsgTime = GetTickCount64();
			gSMMsg[lpObj->m_Index][n].SendUser = aIndex;
			gSMMsg[lpObj->m_Index][n].SubCode = SubCode;
			
			return;
		}
	}
}




void gObjAddMsgSendDelay(LPOBJ lpObj, int aMsgCode, int aIndex, int delay, int SubCode)
{
	for ( int n=0;n<MAX_MONSTER_SEND_MSG;n++)
	{
		if ( gSMMsg[lpObj->m_Index][n].MsgCode < 0 )
		{
			gSMMsg[lpObj->m_Index][n].MsgCode = aMsgCode;
			gSMMsg[lpObj->m_Index][n].MsgTime = GetTickCount64() + delay;
			gSMMsg[lpObj->m_Index][n].SendUser = aIndex;
			gSMMsg[lpObj->m_Index][n].SubCode = SubCode;

			return;
		}
	}

	for (int n=0;n<MAX_MONSTER_SEND_MSG;n++)
	{
		if ( gSMMsg[lpObj->m_Index][n].MsgCode == 2 || gSMMsg[lpObj->m_Index][n].MsgCode == 6 || gSMMsg[lpObj->m_Index][n].MsgCode == 7 )
		{
			gSMMsg[lpObj->m_Index][n].MsgCode = aMsgCode;
			gSMMsg[lpObj->m_Index][n].MsgTime = GetTickCount64();
			gSMMsg[lpObj->m_Index][n].SendUser= aIndex;
			gSMMsg[lpObj->m_Index][n].SubCode = SubCode;
			
			return;
		}
	}

}



void gObjAddMsgSendDelayInSpecificQPos(LPOBJ lpObj, int aMsgCode, int aIndex, int delay, int SubCode, int iQPosition)
{
	if ( iQPosition >= 0 &&  iQPosition < MAX_MONSTER_SEND_MSG )
	{
		gSMMsg[lpObj->m_Index][iQPosition].MsgCode = aMsgCode;
		gSMMsg[lpObj->m_Index][iQPosition].MsgTime = GetTickCount64();
		gSMMsg[lpObj->m_Index][iQPosition].SendUser = aIndex;
		gSMMsg[lpObj->m_Index][iQPosition].SubCode = SubCode;
	}

}




void gObjAddAttackProcMsgSendDelay(LPOBJ lpObj, int aMsgCode, int aIndex, int delay, int SubCode, int SubCode2)
{
	for ( int n=0;n<MAX_MONSTER_SEND_ATTACK_MSG;n++)
	{
		int iIndex = lpObj->m_Index;

		if ( gSMAttackProcMsg[iIndex][n].MsgCode < 0 )
		{
			gSMAttackProcMsg[iIndex][n].MsgCode = aMsgCode;
			gSMAttackProcMsg[iIndex][n].MsgTime = GetTickCount64() + delay;
			gSMAttackProcMsg[iIndex][n].SendUser = aIndex;
			gSMAttackProcMsg[iIndex][n].SubCode = SubCode;
			gSMAttackProcMsg[iIndex][n].SubCode2 = SubCode2;

			return;
		}
	}

}



void gObjMsgProc(LPOBJ lpObj)
{
	if(!lpObj)
		return;

	int aIndex = lpObj->m_Index;

	if(!ObjectMaxRange(aIndex))
		return;

	for ( int n=0;n<MAX_MONSTER_SEND_MSG;n++)
	{
		if ( gSMMsg[aIndex][n].MsgCode >= 0 )
		{
			if ( GetTickCount64() > gSMMsg[aIndex][n].MsgTime )
			{
				if ( lpObj->Type == OBJ_MONSTER || lpObj->Type == OBJ_NPC )
				{
					gObjMonsterStateProc(lpObj, gSMMsg[aIndex][n].MsgCode, gSMMsg[aIndex][n].SendUser, gSMMsg[aIndex][n].SubCode);
				}
				else
				{
					gObjStateProc(lpObj, gSMMsg[aIndex][n].MsgCode, gSMMsg[aIndex][n].SendUser, gSMMsg[aIndex][n].SubCode);
				}

				gSMMsg[aIndex][n].MsgCode = -1;
			}
		}
	}
}




void gObjStateProc(LPOBJ lpObj, int aMsgCode, int aIndex, int SubCode)
{
	if ( lpObj == NULL )
	{
		g_Log.Add("error : %s %d", __FILE__, __LINE__ );
		return;
	}

	if ( ObjectMaxRange(aIndex) == FALSE )
	{
		g_Log.Add("error : %s %d", __FILE__, __LINE__ );
		return;
	}

	switch ( aMsgCode )
	{
		case  2:
			gObjBackSpring(lpObj, &gObj[aIndex]);
			break;

		case  3:
			gObjMonsterDieLifePlus(lpObj, &gObj[aIndex]);
			break;

		case 1000:
			gObjBillRequest(lpObj);
			break;

		case 1001:
			gObjReqMapSvrAuth(lpObj);
			break;

		case  4:
			if(lpObj->Type == OBJ_USER && lpObj->Connected == 3)
			{
				if ( lpObj->m_PlayerData->lpGuild != NULL && lpObj->m_PlayerData->lpGuild->lpTargetGuildNode != NULL )
				{
					gObjGuildWarEnd(lpObj->m_PlayerData->lpGuild, lpObj->m_PlayerData->lpGuild->lpTargetGuildNode);
				}
			}
			break;

		case  5:
			if(lpObj->Type == OBJ_USER && lpObj->Connected == 3)
			{
				if ( lpObj->m_PlayerData->lpGuild != NULL && lpObj->m_PlayerData->lpGuild->WarState != NULL && lpObj->m_PlayerData->lpGuild->WarType == 1 )
				{
					BattleSoccerGoalStart(0);
				}
			}
			break;

		case  7:
			if(lpObj->Type == OBJ_USER && lpObj->Connected == 3)
			{
				if ( lpObj->m_PlayerData->lpGuild != NULL && lpObj->m_PlayerData->lpGuild->WarState != NULL && lpObj->m_PlayerData->lpGuild->WarType == 1)
				{
					if ( lpObj->m_PlayerData->lpGuild->BattleGroundIndex >= 0 )
					{
						GSProtocol.GCManagerGuildWarEnd(lpObj->m_PlayerData->lpGuild->Name);
					}
				}
			}
			break;

		case 10:
			if ( lpObj->DamageReflect != 0 )
			{
				gObjAttack(lpObj, &gObj[aIndex], 0, 0, 0, SubCode, 0, 0, 1);
			}
			break;
		case 12:
			gObjAttack(lpObj, &gObj[aIndex], 0, 0, 0, SubCode, 0, 0, 0);
			break;
		case 13:
			lpObj->Life = lpObj->MaxLife + lpObj->AddLife;
			GSProtocol.GCReFillSend(lpObj->m_Index, lpObj->Life, -1, 0, lpObj->iShield);
			break;
		case 14:
			lpObj->Mana = lpObj->MaxMana + lpObj->AddMana;
			GSProtocol.GCManaSend(lpObj->m_Index, lpObj->Mana, -1, 0, lpObj->BP);
			break;
		case 15:
			lpObj->iShield = lpObj->iMaxShield + lpObj->iAddShield;
			GSProtocol.GCReFillSend(lpObj->m_Index, lpObj->Life, -1, 0, lpObj->iShield);
			break;
		case 16:
			lpObj->Life += SubCode;
			if (lpObj->Life > lpObj->AddLife + lpObj->MaxLife)
			{
				lpObj->Life = lpObj->AddLife + lpObj->MaxLife;
			}
			GSProtocol.GCReFillSend(lpObj->m_Index, lpObj->Life, 0xFF, 0, lpObj->iShield);
			break;
		case 54:
			if( lpObj->Live )
			{
				CMagicInf * lpMagic = &DefMagicInf[79];
				gObjAttack(lpObj, &gObj[aIndex], lpMagic, 1, 0, SubCode, 0, 0, 0);
			}
		case 58: //Eletric Surge
			if (lpObj->Live)
			{
				CMagicInf * lpMagic = &DefMagicInf[216]; //Eletrict Surge
				gObjAttack(lpObj, &gObj[aIndex], lpMagic, 1, 0, SubCode, 0, 0, 0);
			}
			break;
		case 59:
			if ( lpObj->Live )
			{
				CMagicInf * lpMagic = gObjGetMagicSearch(lpObj, 225);
				if(lpMagic)
				{
					BYTE PosX = LOBYTE(LOWORD(SubCode));
					BYTE PosY = HIBYTE(LOWORD(SubCode));
					BYTE Msg = HIWORD(SubCode);
					gObjUseSkill.NewSkillProc(lpObj->m_Index, lpMagic, PosX, PosY, aIndex, Msg+1, 0);
				}
			}
			break;
		case 65:
			if ( lpObj->Live )
			{
				CMagicInf * lpMagic = gObjGetMagicSearch(lpObj, 339);
				if(lpMagic)
				{
					gObjUseSkill.NewSkillProc(lpObj->m_Index, lpMagic, 0, 0, aIndex, SubCode+1, 0);
				}
			}
			break;
	}
}





void gObjStateAttackProc(LPOBJ lpObj, int aMsgCode, int aIndex, int SubCode, int SubCode2)
{
	if ( ObjectMaxRange(aIndex) == FALSE )
	{
		g_Log.Add("error : %s %d", __FILE__, __LINE__ );
		return;
	}

	switch ( aMsgCode )
	{
		case 50:
			if ( lpObj->Live != FALSE )
			{
				CMagicInf * lpMagic = gObjGetMagicSearch(lpObj, SubCode);

				if (!lpMagic)
					return;

				gObjAttack(lpObj, &gObj[aIndex], lpMagic, 0, 1, 0, SubCode2, 0, 0);

				if (lpMagic->m_Skill == AT_SKILL_SUICIDE)
				{
					lpObj->m_PosNum = -1;
					lpObj->Life = 0.0;

					gObjLifeCheck(lpObj, &gObj[aIndex], 0, 1, 0, 0, 0, 0, 0);
				}
			}
			break;
		case 53:
			if( lpObj->Live )
			{
				CMagicInf * lpMagic = gObjGetMagicSearch(lpObj, SubCode);

				if (lpMagic != NULL)
				{
					gObjAttack(lpObj, &gObj[aIndex], lpMagic, 1, 1, 0, 0, 0, 0);
				}
			}
			break;
		case 51:
			if (lpObj->Type == OBJ_USER && lpObj->m_PlayerData->ISBOT == false && lpObj->m_Index >= g_ConfigRead.server.GetObjectStartUserIndex())
			{
				gDarkSpirit[lpObj->m_Index- g_ConfigRead.server.GetObjectStartUserIndex()].Attack(lpObj, &gObj[aIndex], NULL, SubCode, SubCode2);
			}
			break;
		case 60:
			if (lpObj->Live != FALSE)
			{
				CMagicInf * lpMagic = gObjGetMagicSearch(lpObj, SubCode);

				if (lpMagic != NULL)
				{
					gObjAttack(lpObj, &gObj[aIndex], lpMagic, 1, 1, 0, SubCode2, 0, 0);
				}
			}
			break;
		case 61:
			{
				CMagicInf * lpMagic = gObjGetMagicSearch(lpObj, SubCode);

				if (lpMagic != NULL)
				{
					if (lpMagic->m_Skill == 263)
					{
						gObjUseSkill.SkillMonkBarrageJustOneTarget(lpObj->m_Index, lpMagic, SubCode2);
					}

					else if (lpMagic->m_Skill == 559 || lpMagic->m_Skill == 563)
					{
						g_MasterLevelSkillTreeSystem.MLS_SkillMonkBarrageJustOneTarget(lpObj->m_Index, lpMagic, SubCode2);
					}
				}
			}
			break;
		case 63:
			g_CMuunSystem.Attack(lpObj, &gObj[aIndex], 0, SubCode, SubCode2);
			break;
	}

}




BOOL gObjBackSpring(LPOBJ lpObj, LPOBJ lpTargetObj)
{
	int tdir;
	int x;
	int y;
	BYTE attr;
	PMSG_POSISTION_SET pMove;

	if (MAX_MAP_RANGE(lpObj->MapNumber) == FALSE)
	{
		g_Log.Add("error : %s %d", __FILE__, __LINE__);
		return FALSE;
	}

	if (lpObj->Type == OBJ_USER)
	{
		if (lpObj->m_LastTeleportTime > 0)
		{
			return FALSE;
		}
	}

	if (g_ConfigRead.server.GetServerType() == SERVER_CASTLE)
	{
		if (lpObj->Type == OBJ_USER)
		{
			if ((lpObj->Authority & 0x02) == 2)
			{
				return FALSE;
			}
		}
	}

	if (lpObj->Class >= 131 && lpObj->Class <= 134)
	{
		return FALSE;
	}

	if (lpObj->Class >= 523 && lpObj->Class <= 528)
	{
		return FALSE;
	}

	if (lpObj->Class >= 598 && lpObj->Class <= 602)
	{
		return FALSE;
	}

	if (lpObj->Class >= 627 && lpObj->Class <= 631)
	{
		return FALSE;
	}

	if (lpObj->Class == 275 || lpObj->Class == 657 || lpObj->Class == 673)
	{
		return FALSE;
	}

	if (g_ConfigRead.server.GetServerType() == SERVER_CASTLE)
	{
		if (lpObj->Class == 277 || //castle siege ??
			lpObj->Class == 283 ||
			lpObj->Class == 288 ||
			lpObj->Class == 278 ||
			lpObj->Class == 215 ||
			lpObj->Class == 216 ||
			lpObj->Class == 217 ||
			lpObj->Class == 218 ||
			lpObj->Class == 219)
		{
			return FALSE;
		}

		if (g_CastleSiege.GetCrownUserIndex() == lpObj->m_Index)
		{
			if (lpObj->pInventory[lpObj->m_btInvenPetPos].m_Type == ITEMGET(13, 4))
			{
				return FALSE;
			}
		}

		if (g_Crywolf.GetCrywolfState() == 4 || g_Crywolf.GetCrywolfState() == 3)
		{
			if (lpObj->Type == OBJ_USER)
			{
				for (int i = 205; i <= 209; i++)//crywolf altar
				{
					int iAltarIndex = g_CrywolfNPC_Altar.GetAltarUserIndex(i);

					if (iAltarIndex != -1)
					{
						if (iAltarIndex == lpObj->m_Index)
						{
							return FALSE;
						}
					}
				}
			}
		}

		if (CRYWOLF_ALTAR_CLASS_RANGE(lpObj->Class) || CHECK_CLASS(204, lpObj->Class) != FALSE) //HermeX Fix
		{
			return FALSE;
		}
		if (lpObj->Class == 348)//balista
		{
			return FALSE;
		}
	}

	if (lpObj->Class == 204)
	{
		return FALSE;
	}

	//Season 4.0 add-on
	if (lpObj->Class == 459 ||
		lpObj->Class == 460 ||
		lpObj->Class == 461 ||
		lpObj->Class == 462)
	{
		return FALSE;
	}

	//Season 4.0 add-on
	if (lpObj->Class == 459 ||
		lpObj->Class == 460 ||
		lpObj->Class == 461 ||
		lpObj->Class == 462)
	{
		return FALSE;
	}

	int EffectID = -1;
	for (int i = 0; i < MAX_BUFFEFFECT; i++) //season4 add-on
	{
		EffectID = lpObj->m_BuffEffectList[i].BuffIndex;

		if (EffectID == BUFFTYPE_STONE ||
			EffectID == BUFFTYPE_STUN ||
			EffectID == BUFFTYPE_RESTRICTION ||
			EffectID == BUFFTYPE_FREEZE_2 ||
			EffectID == BUFFTYPE_EARTH_BINDS)
		{
			return FALSE;
		}
	}

	if ((rand() % 3) == 0)
	{
		if (lpTargetObj->Dir < 4)
		{
			tdir = lpTargetObj->Dir + 4;
		}
		else
		{
			tdir = lpTargetObj->Dir - 4;
		}

		tdir *= 2;
	}
	else
	{
		tdir = lpTargetObj->Dir * 2;
	}

	x = lpObj->X;
	y = lpObj->Y;
	x += RoadPathTable[tdir];
	y += RoadPathTable[1 + tdir];
	attr = MapC[lpObj->MapNumber].GetAttr(x, y);

	if ((attr & 1) == 1 ||
		(attr & 2) == 2 ||
		(attr & 4) == 4 ||
		(attr & 8) == 8 || 
		(attr&16)== 16 
		)
	{
		return FALSE;
	}

	pMove.h.c = 0xC1;
	pMove.h.headcode = 0x11;
	pMove.h.size = sizeof(pMove);
	pMove.X = x;
	pMove.Y = y;

	lpObj->m_Rest = 0;
	MapC[lpObj->MapNumber].ClearStandAttr(lpObj->m_OldX, lpObj->m_OldY);
	MapC[lpObj->MapNumber].SetStandAttr(x, y);
	lpObj->m_OldX = x;
	lpObj->m_OldY = y;
	GSProtocol.RecvPositionSetProc(&pMove, lpObj->m_Index);

	if ( lpObj->Type == OBJ_USER )
	{
		lpObj->PathCount = 0;
	}

	return TRUE;
}





BOOL BackSpringCheck(int & x, int & y, int & dir, BYTE map)
{
	BYTE attr;
	int tx = x;
	int ty = y;
	int tdir = dir / 2;

	tx = tx + RoadPathTable[dir];
	ty = ty + RoadPathTable[1 + dir];

	attr = MapC[map].GetAttr(tx, ty);

	if ((attr & 1) == 1 ||
		(attr & 4) == 4 ||
		(attr & 8) == 8 || 
		(attr & 16) == 16
		)
	{
		tdir += 4;

		if (tdir > 7)
		{
			tdir -= 8;
		}

		dir = tdir * 2;
		return FALSE;
	}

	x = tx;
	y = ty;

	return TRUE;
}

BOOL gObjBackSpring2(LPOBJ lpObj, LPOBJ lpTargetObj, int count)
{
	int tdir;

	if ( MAX_MAP_RANGE(lpObj->MapNumber) == FALSE )
	{
		g_Log.Add("error : %s %d", __FILE__, __LINE__);
		return FALSE;
	}

	if (g_ConfigRead.server.GetServerType() == SERVER_CASTLE)
	{
		if (lpObj->Type == OBJ_USER)
		{
			if ((lpObj->Authority & 0x02) == 2)
			{
				return FALSE;
			}
		}
	}

	if ( lpObj->Class == 287 || lpObj->Class == 286 )
	{
		return FALSE;
	}

	if ( lpObj->Class >= 131 && lpObj->Class <= 134 )
	{
		return FALSE;
	}

	if ( lpObj->Class >= 523 && lpObj->Class <= 528 )
	{
		return FALSE;
	}

	if ( lpObj->Class >= 598 && lpObj->Class <= 602 )
	{
		return FALSE;
	}

	if (lpObj->Class >= 627 && lpObj->Class <= 631)
	{
		return FALSE;
	}

	if (lpObj->Class == 275 || lpObj->Class == 657 || lpObj->Class == 673)
	{
		return FALSE;
	}

	if (g_ConfigRead.server.GetServerType() == SERVER_CASTLE)
	{
		if (lpObj->Class == 278)
		{
			return FALSE;
		}

		if (lpObj->Class == 277 ||//castle siege ??
			lpObj->Class == 283 ||
			lpObj->Class == 288 ||
			lpObj->Class == 278 ||
			lpObj->Class == 215 ||
			lpObj->Class == 216 ||
			lpObj->Class == 217 ||
			lpObj->Class == 218 ||
			lpObj->Class == 219)
		{
			return FALSE;
		}

		if (g_CastleSiege.GetCrownUserIndex() == lpObj->m_Index)
		{
			if (lpObj->pInventory[lpObj->m_btInvenPetPos].m_Type == ITEMGET(13, 4))
			{
				return FALSE;
			}
		}

		if (g_Crywolf.GetCrywolfState() == 4 || g_Crywolf.GetCrywolfState() == 3)
		{
			if (lpObj->Type == OBJ_USER)
			{
				for (int i = 205; i <= 209; i++)//crywolf altar
				{
					int iAltarIndex = g_CrywolfNPC_Altar.GetAltarUserIndex(i);

					if (iAltarIndex != -1)
					{
						if (iAltarIndex == lpObj->m_Index)
						{
							return FALSE;
						}
					}
				}
			}
		}

		if (CRYWOLF_ALTAR_CLASS_RANGE(lpObj->Class) || CHECK_CLASS(204, lpObj->Class) != FALSE) //HermeX Fix
		{
			return FALSE;
		}
		if (lpObj->Class == 348)//balista
		{
			return FALSE;
		}
	}
	
	if(lpObj->Class == 275) //Season 2.5 add-on (Kundun Fix)
	{
		return FALSE;
	}

	if( lpObj->Class == 204 )
	{
		return FALSE;
	}

	//Season 4.0 add-on
	if(lpObj->Class == 459 ||
		lpObj->Class == 460 ||
		lpObj->Class == 461 ||
		lpObj->Class == 462)
	{
		return FALSE;
	}

	int EffectID = -1;

	for (int i = 0; i < MAX_BUFFEFFECT; i++) //season4 add-on
	{
		EffectID = lpObj->m_BuffEffectList[i].BuffIndex;

		if (EffectID == BUFFTYPE_STONE ||
			EffectID == BUFFTYPE_STUN ||
			EffectID == BUFFTYPE_RESTRICTION ||
			EffectID == BUFFTYPE_FREEZE_2 ||
			EffectID == BUFFTYPE_EARTH_BINDS)
		{
			return FALSE;
		}
	}

	tdir = GetPathPacketDirPos(lpObj->X - lpTargetObj->X, lpObj->Y - lpTargetObj->Y) * 2;

	int x = lpObj->X;
	int y = lpObj->Y;

	for ( int n=0;n<count;n++)
	{
		if ( n >= 2 )
		{
			if ( lpObj->Class == 4 || lpObj->Class == 3 || lpObj->Class == 1 )
			{
				BackSpringCheck(x, y, tdir, lpObj->MapNumber);
			}
		}
		else
		{
			BackSpringCheck(x, y, tdir, lpObj->MapNumber);
		}
	}

	PMSG_POSISTION_SET pMove;

	pMove.h.c = 0xC1;
	pMove.h.headcode = 0x11;
	pMove.h.size = sizeof(pMove);
	pMove.X = x;
	pMove.Y = y;

	lpObj->m_Rest = 0;
	MapC[lpObj->MapNumber].ClearStandAttr(lpObj->m_OldX, lpObj->m_OldY);
	MapC[lpObj->MapNumber].SetStandAttr(x, y);
	lpObj->m_OldX = x;
	lpObj->m_OldY = y;
	GSProtocol.RecvPositionSetProc(&pMove, lpObj->m_Index);

	return TRUE;
}





bool gObjLevelUp(LPOBJ lpObj, UINT64 addexp, int iMonsterType, const char * szEventType)
{
	if(g_MasterLevelSkillTreeSystem.IsMasterLevelUser(lpObj) == true)
	{
		return g_MasterLevelSkillTreeSystem.MasterLevelUp(lpObj, addexp, iMonsterType, szEventType);
	}

	g_Log.Add("[%s] Experience : Map[%s]-(%d,%d) [%s][%s](%d) Experience: %I64d + %I64d  MonsterClass : %d",
		szEventType, Lang.GetMap(0, lpObj->MapNumber),lpObj->X,lpObj->Y,lpObj->AccountID,lpObj->Name,lpObj->Level,lpObj->m_PlayerData->Experience - addexp,addexp,iMonsterType);

	if ( lpObj->Level >= g_ConfigRead.data.common.UserMaxLevel && lpObj->m_PlayerData->Experience >= gLevelExperience[lpObj->Level] )
	{
		lpObj->m_PlayerData->Experience = gLevelExperience[lpObj->Level];
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,45), lpObj->m_Index, 1);
		return false;
	}

	gObjSetExpPetItem(lpObj->m_Index, addexp);
	if ( lpObj->m_PlayerData->Experience < lpObj->m_PlayerData->NextExp )
	{
		return true;
	}

	lpObj->m_PlayerData->Experience = lpObj->m_PlayerData->NextExp;
	lpObj->Level ++;

	if ( g_ConfigRead.data.reset.iBlockLevelUpPointAfterResets == -1 || lpObj->m_PlayerData->m_iResets < g_ConfigRead.data.reset.iBlockLevelUpPointAfterResets )
	{
		if ( lpObj->Class == 4 )
		{
			lpObj->m_PlayerData->LevelUpPoint += gLevelUpPointMGDL;
		}

		else if ( lpObj->Class == 3 )
		{
			lpObj->m_PlayerData->LevelUpPoint += gLevelUpPointMGDL;
		}

		else if ( lpObj->Class == 6 )
		{
			lpObj->m_PlayerData->LevelUpPoint += gLevelUpPointMGDL;
		}

		else if ( lpObj->Class == 7 )
		{
			lpObj->m_PlayerData->LevelUpPoint += gLevelUpPointMGDL;
		}

		else
		{	
			lpObj->m_PlayerData->LevelUpPoint += gLevelUpPointNormal;
		}

		if ( lpObj->m_PlayerData->PlusStatQuestClear != false )
		{	
			lpObj->m_PlayerData->LevelUpPoint += 1;
			g_Log.Add("[%s][%s] LevelUp PlusStatQuest Clear AddStat %d",
				lpObj->AccountID, lpObj->Name, lpObj->m_PlayerData->LevelUpPoint);
		}
	}

	gObjCalCharacter.CalcCharacter(lpObj->m_Index);
	g_CMuunSystem.CheckMuunItemConditionLevelUp(lpObj);
	lpObj->MaxLife += DCInfo.DefClass[ lpObj->Class ].LevelLife;
	lpObj->MaxMana += DCInfo.DefClass[ lpObj->Class ].LevelMana;
	lpObj->Life = lpObj->MaxLife;
	lpObj->Mana = lpObj->MaxMana;

	lpObj->Life = lpObj->MaxLife + lpObj->AddLife;
	lpObj->Mana = lpObj->MaxMana + lpObj->AddMana;
	
	gObjCalCharacter.CalcShieldPoint(lpObj);
	lpObj->iShield = lpObj->iMaxShield + lpObj->iAddShield;
	GSProtocol.GCReFillSend(lpObj->m_Index, lpObj->Life, -1, 0, lpObj->iShield);

	gObjNextExpCal(lpObj);	
	g_Log.Add("Level Up [%s][%s][%d]", lpObj->AccountID, lpObj->Name, lpObj->Level);

	if(lpObj->Class == CLASS_ELF)
	{
		if(lpObj->Level >= g_SkillAdditionInfo.GetInfinityArrowUseLevel() && lpObj->m_PlayerData->ChangeUP >= 1)
		{
			int iAddSkillPosition = gObjMagicAdd(lpObj,0x4D,0);

			if(iAddSkillPosition >= 0)
			{
				GSProtocol.GCMagicListOneSend(lpObj->m_Index,iAddSkillPosition,0x4D,0xDC,0,0);
				g_Log.Add("[%s][%s] Add Infinity Arrow Skill (Character Level : %d)(ChangeUp: %d)",
					lpObj->AccountID,lpObj->Name,lpObj->Level,lpObj->m_PlayerData->ChangeUP);
			}
			else
			{
				g_Log.Add("[%s][%s] Fail - Add Infinity Arrow Skill (Character Level : %d)(ChangeUp: %d)",
					lpObj->AccountID,lpObj->Name,lpObj->Level,lpObj->m_PlayerData->ChangeUP);
			}
		}
	}

	//GJSetCharacterInfo(lpObj, lpObj->m_Index, 0);
	gObjSetBP(lpObj->m_Index);
	GSProtocol.GCLevelUpMsgSend(lpObj->m_Index, 1);
	gObjCalcMaxLifePower(lpObj->m_Index);
	
	return false;
}





BOOL gObjLevelUpPointAdd(BYTE type, LPOBJ lpObj)	// type : [1:Strength][2:Agility][3:Vitality][4:Energy][5:Leadership]
{
	if ( lpObj->Type != OBJ_USER )
	{
		g_Log.AddC(TColor::Red, "error-L1: LevelUp point value error %s %s %s %d", lpObj->AccountID, lpObj->Name, __FILE__, __LINE__);
		return FALSE;
	}

	if ( lpObj->m_PlayerData->LevelUpPoint < 1 )
	{
		return FALSE;
	}

	switch ( type )
	{
		case 0:
			if(lpObj->m_PlayerData->Strength < g_MaxStrength)
			{
				lpObj->m_PlayerData->Strength++;
			}
			else
				return false;
			break;

		case 1:
			if(lpObj->m_PlayerData->Dexterity < g_MaxAgility)
			{
				lpObj->m_PlayerData->Dexterity++;
			}
			else
				return false;
			break;

		case 2:
			if(lpObj->m_PlayerData->Vitality < g_MaxVitality)
			{
				lpObj->m_PlayerData->Vitality++;

				lpObj->MaxLife += lpObj->VitalityToLife;
				gObjCalcMaxLifePower(lpObj->m_Index);
			}
			else
				return false;
			break;

		case 3:
			if(lpObj->m_PlayerData->Energy < g_MaxEnergy)
			{
				lpObj->m_PlayerData->Energy++;
				lpObj->MaxMana += lpObj->EnergyToMana;
			}
			else
				return false;
			break;

		case 4:
			if(lpObj->Leadership < g_MaxCommand)
			{
				lpObj->Leadership++;
			}
			else
				return false;
			break;
	}

	lpObj->m_PlayerData->LevelUpPoint--;
	gObjCalCharacter.CalcCharacter(lpObj->m_Index);

	return TRUE;
}

BOOL gObjGuildMasterCapacityTest(LPOBJ lpObj)
{
	if (lpObj->Level < g_ConfigRead.data.common.GuildCreateLevel)
	{
		return 0;
	}
	return 1;
}

void gObjNextExpCal(LPOBJ lpObj)
{
	if(lpObj->Type == OBJ_USER)
	{
		lpObj->m_PlayerData->NextExp = gLevelExperience[lpObj->Level];
	}
}

int  retResistance(LPOBJ lpObj, int Resistance_Type)
{
	BYTE r = lpObj->m_Resistance[Resistance_Type];

	if( r == 0xFF)
	{
		return 1;
	}

	if ( r > 0 && gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_SOUL_POTION) != NULL && (Resistance_Type == 2 || Resistance_Type == 0 ))
	{
		r += r * 50 / 100;
	}

	if ( r  == 0 )
	{
		return 0;
	}

	if (  (rand()%(r+1)) == 0 )
	{
		return 0;
	}

	return 1;
}




BOOL retCalcSkillResistance(LPOBJ lpTargetObj, BOOL isDouble)
{
	if ( isDouble != FALSE )
	{
		if ( (rand()%100) < 45 )
		{
			return TRUE;
		}
	}
	else
	{
		if ( (rand()%100) < 40 )
		{
			return TRUE;
		}
	}

	return FALSE;
}




BOOL gObjAttackQ(LPOBJ lpObj)
{
	if ( ATTRIBUTE_RANGE(lpObj->m_Attribute) )
	{
		return FALSE;
	}

	if ( lpObj->Class == 221 || lpObj->Class == 222 )
	{
		return FALSE;
	}

	if ( g_MineSystem.IsTwinkle(lpObj->Class) == TRUE )
	{
		return FALSE;
	}

	if (g_ConfigRead.server.GetServerType() == SERVER_CASTLE)
	{
		if (lpObj->Class == 277 || lpObj->Class == 283)
		{
			if (g_CastleSiege.GetCastleState() != CASTLESIEGE_STATE_STARTSIEGE)
			{
				return FALSE;
			}
		}

		if (lpObj->Class == 221 || lpObj->Class == 222) //HermeX Decompilation
		{
			return FALSE;
		}

		if (lpObj->Class == 277)
		{
			if (lpObj->m_btCsGateOpen == 1)
			{
				return FALSE;
			}
		}
	}

	if ( BC_MAP_RANGE(lpObj->MapNumber) )
	{
		if ( lpObj->Type >= OBJ_MONSTER )
		{
			int iBridgeIndex = g_BloodCastle.GetBridgeIndex(lpObj->MapNumber);

			if ( lpObj->Class == 131 && g_BloodCastle.CheckMonsterKillSuccess(iBridgeIndex) == false)
			{
				return FALSE;
			}

			if ( (((lpObj->Class - 132) < 0)?FALSE:((lpObj->Class-132) > 2)?FALSE:TRUE) !=FALSE && g_BloodCastle.CheckBossKillSuccess(iBridgeIndex) == false )
			{
				return FALSE;
			}

		}
	}

	if ((lpObj->Class >= 100 && lpObj->Class <= 110) || lpObj->Class == 523 || lpObj->Class == 247 || lpObj->Class == 249)
	{
		return FALSE;
	}

	if (lpObj->Class == 689)
	{
		return FALSE;
	}
 
	if (lpObj->m_State != 2 && lpObj->m_bOffLevel == false)
	{
		return FALSE;
	}

	if (lpObj->Live == false)
	{
		return false;
	}

	if (lpObj->Teleport == 1)
	{
		return FALSE;
	}

	if (g_ConfigRead.antihack.EnableAttackBlockInSafeZone == TRUE)
	{
		BYTE btAttr = MapC[lpObj->MapNumber].GetAttr(lpObj->X, lpObj->Y);

		if ((btAttr & 1) == 1)
		{
			return FALSE;
		}
	}

	return TRUE;
}

void gObjReady4Relife(LPOBJ lpObj)
{
	gObjCalCharacter.CalcCharacter(lpObj->m_Index);
	GSProtocol.GCReFillSend(lpObj->m_Index, lpObj->MaxLife + lpObj->AddLife, 0xFE, 0, lpObj->iMaxShield + lpObj->iAddShield);
}

void gObjPlayerKiller(LPOBJ lpObj, LPOBJ lpTargetObj, int MSBDamage)
{
	if (lpObj->Type != OBJ_USER)
	{
		return;
	}

	ReqSavePlayerKiller(lpObj->m_Index, lpTargetObj->m_Index);

	for (int r = 0; r < LMS_MAX_ROOMS; r++)
	{
		if (g_LastManStanding.m_Rooms[r].bState == 3 || g_LastManStanding.m_Rooms[r].bState == 4)
		{
			for (int i = 0; i < LMS_MAX_USER_PER_ROOM; i++)
			{
				if (g_LastManStanding.m_Rooms[r].m_Data[i].iIndex == lpObj->m_Index)
				{
					return;
				}
			}
		}
	}

	if (gObjGetRelationShip(lpObj, lpTargetObj) == 2)
	{
		if (lpObj->m_PlayerData->lpGuild != NULL && lpTargetObj->m_PlayerData->lpGuild != NULL)
		{
			g_Log.Add("[U.System][Rival][Player Kill] ( Killer [%s][%s] / %s / U:%d / R:%d ) vs ( Victim [%s][%s] / %s / U:%d / R:%d )",
				lpObj->AccountID, lpObj->Name, lpObj->m_PlayerData->lpGuild->Name, lpObj->m_PlayerData->lpGuild->iGuildUnion, lpObj->m_PlayerData->lpGuild->iGuildRival,
				lpTargetObj->AccountID, lpTargetObj->Name, lpTargetObj->m_PlayerData->lpGuild->Name, lpTargetObj->m_PlayerData->lpGuild->iGuildUnion, lpTargetObj->m_PlayerData->lpGuild->iGuildRival);
		}
		else
		{
			g_Log.Add("[U.System][Rival][Player Kill][ERROR : Can't find GuildInfo] (  [%s][%s] ) vs ( [%s][%s] )",
				lpObj->AccountID, lpObj->Name, lpTargetObj->AccountID, lpTargetObj->Name);
		}
		return;
	}

	if (CC_MAP_RANGE(lpTargetObj->MapNumber))
	{
		if (g_ChaosCastle.GetCurrentState(g_ChaosCastle.GetChaosCastleIndex(lpTargetObj->MapNumber)) == 2)
		{
			return;
		}
	}

	if (lpTargetObj->MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL)
	{
		if (g_ChaosCastleFinal.GetCurrentState() == 2)
		{
			return;
		}
	}

	if (lpObj->MapNumber == MAP_INDEX_DEVILSQUARE_FINAL || lpTargetObj->MapNumber == MAP_INDEX_DEVILSQUARE_FINAL)
	{
		return;
	}

	if (g_ConfigRead.server.GetServerType() == SERVER_CASTLE)
	{
		if (lpTargetObj->MapNumber == MAP_INDEX_CASTLESIEGE)
		{
			if (g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
			{
				if (lpObj->m_btCsJoinSide != 0)
				{
					return;
				}
			}

			if (lpTargetObj->m_PK_Level >= 6)
			{
				return;
			}
		}
	}

	if((lpObj->Authority & 16) || (lpTargetObj->Authority & 16))
	{
		return;
	}

	if ( IT_MAP_RANGE(lpObj->MapNumber) || IT_MAP_RANGE(lpTargetObj->MapNumber) )
	{
		return;
	}

	if ( g_GensSystem.IsMapBattleZone(lpObj->MapNumber) && g_GensSystem.IsMapBattleZone(lpTargetObj->MapNumber) )
	{
		return;
	}

	if (g_ArcaBattle.IsArcaBattleServer() == TRUE)
	{
		return;
	}

	if (g_MapAttr.CheckPkInc(lpObj->MapNumber) == FALSE)
	{
		MsgOutput(lpObj->m_Index, Lang.GetText(0,412));
		return;
	}

	g_GensSystem.SendPKPenaltyDebuff(lpObj);

	for ( int n=0;n<MAX_SELF_DEFENSE;n++)
	{
		if ( lpTargetObj->SelfDefense[n] >= 0 )
		{
			if ( lpTargetObj->SelfDefense[n] == lpObj->m_Index )
			{
				return;
			}
		}
	}

	if(lpTargetObj->m_PK_Level >= 5)
	{
		return;
	}

	int iOriginPkLevel = lpObj->m_PK_Level;
	int iOriginPkCount = lpObj->m_PK_Count;

	if(MSBDamage == 4) //season3 add-on
	{
		return;
	}

	if(g_ConfigRead.pk.bPkLevelIncreaseOff == true)
	{
		return;
	}

	BYTE btDefaultPartyPkLevel = 3; //Season 2.5 add-on
	
	if(lpTargetObj->PartyNumber >= 0) //Season 2.5 add-on
	{
		if( gParty.GetPKPartyPenalty(lpTargetObj->PartyNumber) >= 5 )
		{
			btDefaultPartyPkLevel = gParty.GetPKPartyPenalty(lpTargetObj->PartyNumber);
		}
		else
		{
			btDefaultPartyPkLevel = lpTargetObj->m_PK_Level;
		}
	}
	else
	{
		btDefaultPartyPkLevel = lpTargetObj->m_PK_Level;
	}

	if ( lpObj->m_PK_Level == 3 )
	{
		if( ((char)btDefaultPartyPkLevel) == 3 ) //Season 2.5 changed
		{
			lpObj->m_PK_Count = 1;
		}
		else if(((char)btDefaultPartyPkLevel) < 3) //Season 2.5 changed
		{
			lpObj->m_PK_Count = 1;
		}
		else if(((char)btDefaultPartyPkLevel) == 4) //Season 2.5 changed
		{
			lpObj->m_PK_Count = 1;
		}
		else
		{
			if(((char)btDefaultPartyPkLevel) >= 6 && lpTargetObj->Level > 20) //Season 2.5 changed
			{
				lpObj->m_PK_Count--;

				if(lpObj->m_PK_Count < -3)
				{
					lpObj->m_PK_Count = (BYTE)-3;
					return;
				}
			}
			else
			{
				return;
			}
		}
	}
	else if( lpObj->m_PK_Level > 3)
	{
		if(((char)btDefaultPartyPkLevel) <= 4) //Season 2.5 changed
		{
			lpObj->m_PK_Count++;
		//	if(lpObj->m_PK_Count > 100)
		//	{
			//	lpObj->m_PK_Count = 100;
		//	}
		}
		else
		{
			return;
		}
	}
	else if( lpObj->m_PK_Level < 3)
	{
		if(((char)btDefaultPartyPkLevel) == 3) //Season 2.5 changed
		{
			lpObj->m_PK_Count = 1;
		}
		else if(((char)btDefaultPartyPkLevel) < 3) //Season 2.5 changed
		{
			lpObj->m_PK_Count = 1;
		}
		else if(((char)btDefaultPartyPkLevel) == 4) //Season 2.5 changed
		{
			lpObj->m_PK_Count = 1;
		}
		else
		{
			if(((char)btDefaultPartyPkLevel) >= 6 && lpTargetObj->Level > 20) //Season 2.5 changed
			{
				lpObj->m_PK_Count--;

				if(lpObj->m_PK_Count < -3)
				{
					lpObj->m_PK_Count = -3;
				}
			}
			else
			{
				return;
			}
		}
	}

	if(lpObj->m_PK_Count == 0)
	{
		lpObj->m_PK_Level = 3;
		lpObj->m_PK_Time = 0;
	}
	else if(lpObj->m_PK_Count == 1)
	{
		lpObj->m_PK_Level = 4;
		lpObj->m_PK_Time = 0;
	}
	else if(lpObj->m_PK_Count == 2)
	{
		lpObj->m_PK_Level = 5;
		lpObj->m_PK_Time = 0;

		if(lpObj->PartyNumber >= 0) //Season 2.5 add-on
		{
			gParty.UpdatePKUserInfo(lpObj->PartyNumber,lpObj->m_Index,lpObj->DBNumber,lpObj->m_PK_Level);
			gParty.UpdatePKPartyPanalty(lpObj->PartyNumber);
		}
	}
	else if(lpObj->m_PK_Count >= 3)
	{
		lpObj->m_PK_Level = 6;
		lpObj->m_PK_Time = 0;

		if(lpObj->PartyNumber >= 0) //Season 2.5 add-on
		{
			gParty.UpdatePKUserInfo(lpObj->PartyNumber,lpObj->m_Index,lpObj->DBNumber,lpObj->m_PK_Level);
			gParty.UpdatePKPartyPanalty(lpObj->PartyNumber);
		}
	}
	else if(lpObj->m_PK_Count <= -3)
	{
		lpObj->m_PK_Level = 1;
		lpObj->m_PK_Time = 0;
	}
	
	GSProtocol.GCPkLevelSend(lpObj->m_Index, lpObj->m_PK_Level);

	if ( lpObj->m_PK_Level == 4 )
	{
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,46), lpObj->m_Index, 1);
	}

	if (lpObj->m_PlayerData->m_bUseGuildMatching)
	{
		if (lpObj->m_PlayerData->GuildNumber > 0)
			GDReqDelMatchingList(lpObj->m_Index, lpObj->m_PlayerData->GuildNumber);
		else
			GDReqCancelJoinGuildMatching(lpObj->m_Index, lpObj->Name, 0);
	}

	if (lpObj->m_PlayerData->m_bUsePartyMatching)
	{
		if (lpObj->m_PlayerData->m_bPartyMatchingLeader)
		{
			GDReqCancelPartyMatching(lpObj->m_Index, 0);
		}

		else
		{
			if (lpObj->PartyNumber >= 0)
			{
				gParty.Delete(lpObj->PartyNumber, lpObj->m_Index, lpObj->DBNumber);
				lpObj->PartyNumber = -1;
			}

			GDReqCancelPartyMatching(lpObj->m_Index, 1);
		}
	}

	g_Log.Add("[%s][%s] PK Info Set (Killed Player) Origin(PkLevel:%d, PkCount:%d) Changed(PkLevel:%d, PkCount:%d)",
		lpObj->AccountID, lpObj->Name, iOriginPkLevel, iOriginPkCount, lpObj->m_PK_Level, lpObj->m_PK_Count);
	

}

void gObjUserDie(LPOBJ lpObj, LPOBJ lpTargetObj)
{
	int count = 76;
	int itemdrop = 0;
	int number = 0;
	int dropresult = 0;

	if(lpObj->Type != OBJ_USER)
	{
		return;
	}

	gObjSetKillCount(lpObj->m_Index,0);

	gObjRemoveBuffEffect(lpObj, CLEAR_TYPE_NON_PCS_ITEM_EFFECT);

	gObjUseSkill.RemoveAllCharacterInvalidMagicAndSkillState(lpObj);

	lpObj->m_PlayerData->m_MuBotEnable = false;
	GSProtocol.GCAnsMuBotUse(lpObj->m_Index, 1, lpObj->m_PlayerData->m_MuBotTotalTime, 0);

	if(gObjTargetGuildWarCheck(lpObj,lpTargetObj) == 1)
	{
		return;
	}

	if(lpObj->m_bOffLevel == true)
	{
		lpObj->m_bOff = false;
		lpObj->m_bOffLevel = false;
	}

	if(DS_MAP_RANGE(lpObj->MapNumber))
	{
		if(lpTargetObj->Type == OBJ_MONSTER)
		{

			LPMONSTER_ATTRIBUTE mAttr = gMAttr.GetAttr(lpTargetObj->Class);

			if(mAttr != 0)
			{
				g_Log.Add("[DevilSquare] Dead In DevilSquare [%s][%s][%s]",lpObj->AccountID,lpObj->Name,mAttr->m_Name);
			}
		}

		return;
	}

	if(lpObj->MapNumber == MAP_INDEX_DEVILSQUARE_FINAL)
	{
		g_DevilSquareFinal.DSFUserDie(lpObj);
	}
	
	if(BC_MAP_RANGE(lpObj->MapNumber))
	{
		g_BloodCastle.SetUserState(lpObj->m_Index,1);

		int iBridgeIndex = g_BloodCastle.GetBridgeIndex(lpObj->MapNumber);

		if(g_BloodCastle.GetCurrentState(iBridgeIndex+1) == BC_STATE_PLAYING)
		{
			g_Log.Add("[Blood Castle] (%d) Try to drop Ultimate Weapon [%s][%s]",iBridgeIndex+1,lpObj->AccountID,lpObj->Name);
			g_BloodCastle.SearchUserDropQuestItem(lpObj->m_Index);
		}
		else
		{
			g_Log.Add("[Blood Castle] (%d) Try to delete Ultimate Weapon [%s][%s]",iBridgeIndex+1,lpObj->AccountID,lpObj->Name);
			g_BloodCastle.SearchUserDeleteQuestItem(lpObj->m_Index);
		}

		if(lpTargetObj->Type == OBJ_MONSTER)
		{
			LPMONSTER_ATTRIBUTE mAttr = gMAttr.GetAttr(lpTargetObj->Class);

			if(mAttr != 0)
			{
				g_Log.Add("[Blood Castle] (%d) Dead In Blood Castle, Killed by Monster [%s][%s][%s]",iBridgeIndex+1,lpObj->AccountID,lpObj->Name,mAttr->m_Name);
				g_BloodCastle.SearchUserDropQuestItem(lpObj->m_Index);
			}
		}
		else if(lpTargetObj->Type == OBJ_USER)
		{
			g_Log.Add("[Blood Castle] (%d) Dead In Blood Castle, Killed by Other User [%s][%s][%s]",iBridgeIndex+1,lpObj->AccountID,lpObj->Name,lpTargetObj->Name);
			g_BloodCastle.SearchUserDropQuestItem(lpObj->m_Index);
		}
		return;
	}
	
	if (lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER)
	{
		if (lpObj->MapNumber == g_LastManStanding.m_Cfg.iPVPMap)
		{
			g_LastManStanding.UserDie(lpObj->m_Index, lpTargetObj->m_Index);
		}
	}

	if (g_AcheronGuardianEvent.IsPlayStart() == true)
	{
		return;
	}

	if(g_ArcaBattle.IsArcaBattleServer() == TRUE && g_ArcaBattle.GetState() == 7) 
	{
		g_ArcaBattle.AddContributePoint(lpTargetObj, lpObj);
		g_ArcaBattle.AddKillPoint(lpTargetObj, lpObj);
		return;
	}

	if(IT_MAP_RANGE(lpObj->MapNumber))
	{
		g_IT_Event.IllusionTempleUserDie(lpObj);
	}

	else if(CC_MAP_RANGE(lpObj->MapNumber))
	{
		return;
	}

	else if (lpObj->MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL)
	{
		return;
	}

	if (g_ConfigRead.server.GetServerType() == SERVER_CASTLE)
	{
		if (g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE && lpObj->MapNumber == MAP_INDEX_CASTLESIEGE)
		{
			if (g_CastleSiege.CastleSiegeResetAccumulationAfterKill == TRUE)
			{
				lpObj->m_iAccumulatedCrownAccessTime = 0;
			}

			return;
		}
	}

	if((lpObj->Authority & 0x10) || (lpTargetObj->Authority & 0x10))
	{
		return;
	}

	if(lpTargetObj->Type == OBJ_MONSTER)
	{
		GSProtocol.ChatTargetSend(lpTargetObj, Lang.GetText(0,335), lpObj->m_Index);
		g_Log.Add("[%s][%s] Killed [%s][%s] Pos:(%d,%d,%d)","Monster", lpTargetObj->Name, lpObj->AccountID, lpObj->Name, lpObj->MapNumber, lpObj->X, lpObj->Y);
	}
	else
	{
		g_Log.Add("[%s][%s] (PkLevel = %d) Killed [%s][%s] (PkLevel = %d) Pos:(%d,%d,%d)",lpTargetObj->AccountID,lpTargetObj->Name,lpTargetObj->m_PK_Level,lpObj->AccountID,lpObj->Name,lpObj->m_PK_Level,lpObj->MapNumber,lpObj->X,lpObj->Y);
		g_Log.Add("Killed by User State Victim:[%s][%s], Murderer:[%s][%s], Dead State (Duel:%d, GuildWar:%d, SeldDefense:%d, GensWar:%d)",lpObj->AccountID,lpObj->Name,lpTargetObj->AccountID,lpTargetObj->Name,!!IsOnDuel(lpTargetObj->m_Index,lpObj->m_Index),!!gObjTargetGuildWarCheck(lpObj,lpTargetObj),!!gObjIsSelfDefense(lpTargetObj,lpObj->m_Index), g_GensSystem.IsMapBattleZone(lpObj->MapNumber));
	}

	g_GensSystem.CalcContributePoint(lpTargetObj, lpObj);

	lpObj->DieMapNumber = lpObj->MapNumber;
	lpObj->DieX = lpObj->X;
	lpObj->DieY = lpObj->Y;

	int iItemDropRate = g_ConfigRead.pk.GetItemDropRate(lpObj->m_PK_Level, (lpTargetObj->Type == 1 && lpObj->Type == 1));

	if (rand()%100 < iItemDropRate)
	{
		itemdrop = TRUE;
	}

	if(gObjCanItemTouch(lpObj, 0)==0)
	{
		itemdrop = FALSE;
	}

	if(lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER)
	{
		if(IsOnDuel(lpObj->m_Index,lpTargetObj->m_Index))
		{
			itemdrop = FALSE;
		}
	}

	if(gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_CHARM_PROTECTITEM) != 0)
	{
		itemdrop = FALSE;
	}

	if (g_ConfigRead.server.GetServerType() == SERVER_CASTLE)
	{
		if (g_Crywolf.GetCrywolfState() == 4)
		{
			if (CRYWOLF_MAP_RANGE(lpObj->MapNumber))
			{
				itemdrop = FALSE;
			}
		}
	}

	if (lpObj->Type == OBJ_USER && lpObj->m_PlayerData->RegisterdLMS == TRUE) // port to x9
	{
		itemdrop = FALSE;
	}

	if ( lpTargetObj->Type == OBJ_USER )
	{
		if ( g_GensSystem.IsMapBattleZone(lpTargetObj->MapNumber) && 
			g_GensSystem.IsMapBattleZone(lpObj->MapNumber) )
		{
			return;
		}

		else
		{
			g_GensSystem.PkPenaltyDropInvenItem(lpObj);
		}
	}

	if(itemdrop == TRUE)
	{
		if(lpObj->m_PK_Level >= 4)
		{
			count = 24;

			while(count-- != 0)
			{
				number = rand()%12;

				if(lpObj->pInventory[number].IsItem()==1 && lpObj->pInventory[number].m_Level < g_ConfigRead.pk.iPkMaxLevelItemDrop)
				{
					if(g_kJewelOfHarmonySystem.IsStrengthenByJewelOfHarmony(&lpObj->pInventory[number])==0 && IsCashItem(lpObj->pInventory[number].m_Type)==0)
					{
						PMSG_ITEMTHROW lpMsg;
						lpMsg.Ipos = number;
						lpMsg.px = lpObj->X;
						lpMsg.py = lpObj->Y;
						if(GSProtocol.CGItemDropRequest(&lpMsg,lpObj->m_Index,1)==1)
						{
							dropresult = 1;
							break;
						}
					}
				}
			}
		}

		if(dropresult == 0)
		{
			count = INVENTORY_MAP_SIZE;

			while(count-- != 0)
			{
				number = rand()%INVENTORY_MAP_SIZE + 12;

				if(lpObj->pInventory[number].IsItem()==1 && lpObj->pInventory[number].m_Level <= g_ConfigRead.pk.iPkMaxLevelItemDrop)
				{
					if(g_kJewelOfHarmonySystem.IsStrengthenByJewelOfHarmony(&lpObj->pInventory[number])==0 && IsCashItem(lpObj->pInventory[number].m_Type)==0)
					{
						if(lpObj->pInventory[number].m_Type == ITEMGET(13,20) && (lpObj->pInventory[number].m_Level >= 1 && lpObj->pInventory[number].m_Level <= 2 ))
						{
							continue;
						}

						if (g_PentagramSystem.IsPentagramItem(lpObj->pInventory[number].m_Type) == true ||
							g_PentagramSystem.IsPentagramJewel(lpObj->pInventory[number].m_Type) == true)
						{
							continue;
						}

						PMSG_ITEMTHROW lpMsg;
						lpMsg.Ipos = number;
						lpMsg.px = lpObj->X;
						lpMsg.py = lpObj->Y;

						if(GSProtocol.CGItemDropRequest(&lpMsg,lpObj->m_Index,1)==1)
						{
							dropresult = 1;
							g_Log.Add("[%s][%s] User Die Item Drop, ItemName:%s",lpObj->AccountID,lpObj->Name,lpObj->pInventory[number].GetName());
							break;
						}
					}
				}
			}
		}
	}

	g_GensSystem.PkPenaltyDropZen(lpObj);

	if(lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER ) 
	{
		if ( IsOnDuel(lpObj->m_Index, lpTargetObj->m_Index))
		{
			g_NewPVP.SetScore(*lpTargetObj);
		}

		return;
	}

	UINT64 minexp = gLevelExperience[lpObj->Level - 1];
	UINT64 maxexp = gLevelExperience[lpObj->Level];

	if(g_MasterLevelSkillTreeSystem.IsMasterLevelUser(lpObj) == TRUE)
	{
		minexp = g_MasterLevelSkillTreeSystem.gMasterExperience[lpObj->m_PlayerData->MasterLevel];
		maxexp = g_MasterLevelSkillTreeSystem.gMasterExperience[lpObj->m_PlayerData->MasterLevel+1];
	}

	boost::multiprecision::int128_t subexp = 0;
	boost::multiprecision::int128_t decexp = 0;
	DWORD decexprate = 0;

	BYTE btDefaultPartyPkLevel = 3; //Season 2.5 add-on

	if(lpObj->PartyNumber >= 0) //Season 2.5 add-on
	{
		if( (gParty.GetPKPartyPenalty(lpObj->PartyNumber)) >= 5 )
		{
			btDefaultPartyPkLevel = gParty.GetPKPartyPenalty(lpObj->PartyNumber);
		}
		else
		{
			btDefaultPartyPkLevel = lpObj->m_PK_Level;
		}
	}
	else
	{
		btDefaultPartyPkLevel = lpObj->m_PK_Level;
	}

	if (lpObj->Level > 10)
	{
		decexprate = g_ConfigRead.pk.GetExpDecreaseRate(btDefaultPartyPkLevel, lpObj->Level+lpObj->m_PlayerData->MasterLevel);
	}

	decexp = 0;

	if(decexprate > 0)
	{
		decexp = ((maxexp - minexp)*decexprate)/1000;
	}	

	if (g_ConfigRead.server.GetServerType() == SERVER_CASTLE)
	{
		if (g_Crywolf.GetCrywolfState() == 4)
		{
			if (lpObj->MapNumber == MAP_INDEX_CRYWOLF_FIRSTZONE)
			{
				decexp = 0;
			}
		}
	}

	if(gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_CHARM_GUARDIAN)  == 0)
	{
		if(g_MasterLevelSkillTreeSystem.IsMasterLevelUser(lpObj) != FALSE) //Season3 add-on
		{
			subexp = lpObj->m_PlayerData->MasterExperience - decexp;
		}
		else
		{
			subexp = lpObj->m_PlayerData->Experience - decexp;
		}

		if(subexp < minexp)
		{
			subexp = minexp;
		}
	}

	g_Log.Add("[%s][%s] Death reduces Experience %I64d - %I64d",lpObj->AccountID,lpObj->Name,subexp,decexp);

	DWORD submoney = 0;

	if(lpObj->m_PlayerData->Money > 0 && gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_CHARM_GUARDIAN) == 0)
	{
		int iMoneyLevel = 0;

		if (lpObj->Level > 10)
		{
			iMoneyLevel = g_ConfigRead.pk.GetZenLossRate(lpObj->m_PK_Level, (lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER));
		}

		if (iMoneyLevel > 0)
		{
			submoney =  (DWORD)(lpObj->m_PlayerData->Money * iMoneyLevel) / 100;
		}

		lpObj->m_PlayerData->Money = lpObj->m_PlayerData->Money - submoney;
		
		if(lpObj->m_PlayerData->Money < 0)
		{
			lpObj->m_PlayerData->Money = 0;
		}

		g_Log.Add("[%s][%s] Death reduces money %d - %d",lpObj->AccountID,lpObj->Name,lpObj->m_PlayerData->Money,submoney);
	}

	if(lpObj->WarehouseMoney > 0 && gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_CHARM_GUARDIAN) == 0)
	{
		submoney = 0;

		int iMoneyLevel = 0;

		if (lpObj->Level > 10)
		{
			iMoneyLevel = g_ConfigRead.pk.GetZenLossRate(lpObj->m_PK_Level, (lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER));
		}

		if(iMoneyLevel > 0)
		{
			submoney = (DWORD)(lpObj->WarehouseMoney * iMoneyLevel) / 100;
		}

		lpObj->WarehouseMoney = lpObj->WarehouseMoney - submoney;
		
		if(lpObj->WarehouseMoney < 0)
		{
			lpObj->WarehouseMoney = 0;
		}

		GDSetWarehouseMoney(lpObj->m_Index);

		g_Log.Add("[%s][%s] Death reduces money in Warehouse %d - %d",lpObj->AccountID,lpObj->Name,lpObj->WarehouseMoney,submoney);
	}

	if(g_MasterLevelSkillTreeSystem.IsMasterLevelUser(lpObj) != FALSE) //Season3 add-on
	{
		lpObj->m_PlayerData->MasterExperience = subexp.convert_to<UINT64>();
	}
	else
	{
		lpObj->m_PlayerData->Experience = subexp.convert_to<UINT64>();
	}
}




void gObjInterfaceTimeCheck(LPOBJ lpObj)
{
	int tObjNum;
	BOOL TimeCheck = FALSE;

	if ( lpObj->m_IfState.use == 0 )
	{
		return;
	}

	if ( (GetTickCount() - lpObj->m_InterfaceTime) < 5000 )
	{
		return;
	}

	if ( lpObj->m_IfState.state == 1 )
	{
		return;
	}

	tObjNum = lpObj->TargetNumber;

	if ( tObjNum < 0 )
	{
		return;
	}

	if ( lpObj->m_IfState.type == 1 )
	{
		GSProtocol.CGTradeResult(lpObj->m_Index, 3);
		GSProtocol.CGTradeResult(tObjNum, 3);

		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,47), tObjNum, 1);
		TimeCheck = TRUE;
	}
	else
	{
		if ( lpObj->m_IfState.type == 2 )
		{
			lpObj->PartyTargetUser = -1;
			gObj[tObjNum].PartyTargetUser = -1;
			GSProtocol.GCResultSend(tObjNum, 0x41, FALSE );
			TimeCheck = TRUE;
		}
	}

	if ( TimeCheck != FALSE )
	{
		lpObj->m_IfState.use = 0;
		lpObj->TargetNumber = -1;
		gObj[tObjNum].m_IfState.use = 0;
		gObj[tObjNum].TargetNumber = -1;
	}

}




void gObjPkDownTimeCheck(LPOBJ lpObj, int TargetLevel)
{
	if (lpObj->m_PK_Level == 3)
	{
		return;
	}

	if (TargetLevel >= 2)
	{
		lpObj->m_PK_Time += TargetLevel / g_ConfigRead.pk.iPKPointDeductionDivisor;
	}

	else
	{
		lpObj->m_PK_Time++;
	}

	if (lpObj->m_PK_Level >= 3)
	{
		if (lpObj->m_PK_Level > 3)
		{
			if (lpObj->m_PK_Count <= 3)
			{
				if (lpObj->m_PK_Count == 3)
				{
					if (lpObj->m_PK_Time > g_ConfigRead.pk.GetPKReqPoint(5))
					{
						lpObj->m_PK_Time = 0;
						lpObj->m_PK_Count--;

						if (lpObj->m_PK_Count > 0)
						{
							if (lpObj->m_PK_Count == 2)
							{
								lpObj->m_PK_Level = 5;
							}

							else if (lpObj->m_PK_Count == 1)
							{
								lpObj->m_PK_Level = 4;
							}
						}

						else
						{
							lpObj->m_PK_Count = 0;
							lpObj->m_PK_Level = 3;
						}

						if (lpObj->m_PK_Level <= 5)
						{
							GSProtocol.GCPkLevelSend(lpObj->m_Index, lpObj->m_PK_Level);
						}

						if (lpObj->m_PK_Level == 4)
						{
							GSProtocol.GCServerMsgStringSend(Lang.GetText(0,46), lpObj->m_Index, 1);

							if (lpObj->PartyNumber >= 0)
							{
								gParty.UpdatePKUserInfo(lpObj->PartyNumber, lpObj->m_Index, lpObj->DBNumber, lpObj->m_PK_Level);
								gParty.UpdatePKPartyPanalty(lpObj->PartyNumber);
								GSProtocol.CGPartyListAll(lpObj->PartyNumber);
							}
						}
					}
				}

				else if (lpObj->m_PK_Time > g_ConfigRead.pk.GetPKReqPoint(4))
				{
					lpObj->m_PK_Time = 0;
					lpObj->m_PK_Count--;

					if (lpObj->m_PK_Count > 0)
					{
						if (lpObj->m_PK_Count == 2)
						{
							lpObj->m_PK_Level = 5;
						}

						else if (lpObj->m_PK_Count == 1)
						{
							lpObj->m_PK_Level = 4;
						}
					}

					else
					{
						lpObj->m_PK_Count = 0;
						lpObj->m_PK_Level = 3;
					}

					if (lpObj->m_PK_Level <= 5)
					{
						GSProtocol.GCPkLevelSend(lpObj->m_Index, lpObj->m_PK_Level);
					}

					if (lpObj->m_PK_Level == 4)
					{
						GSProtocol.GCServerMsgStringSend(Lang.GetText(0,46), lpObj->m_Index, 1);

						if (lpObj->PartyNumber >= 0)
						{
							gParty.UpdatePKUserInfo(lpObj->PartyNumber, lpObj->m_Index, lpObj->DBNumber, lpObj->m_PK_Level);
							gParty.UpdatePKPartyPanalty(lpObj->PartyNumber);
							GSProtocol.CGPartyListAll(lpObj->PartyNumber);
						}
					}
				}
			}

			else if (lpObj->m_PK_Time > g_ConfigRead.pk.iPKPointIncForMurderer)
			{
				lpObj->m_PK_Time = 0;
				lpObj->m_PK_Count--;

				if (lpObj->m_PK_Count > 0)
				{
					if (lpObj->m_PK_Count == 2)
					{
						lpObj->m_PK_Level = 5;
					}

					else if (lpObj->m_PK_Count == 1)
					{
						lpObj->m_PK_Level = 4;
					}
				}

				else
				{
					lpObj->m_PK_Count = 0;
					lpObj->m_PK_Level = 3;
				}

				if (lpObj->m_PK_Level <= 5)
				{
					GSProtocol.GCPkLevelSend(lpObj->m_Index, lpObj->m_PK_Level);
				}

				if (lpObj->m_PK_Level == 4)
				{
					GSProtocol.GCServerMsgStringSend(Lang.GetText(0,46), lpObj->m_Index, 1);
					
					if (lpObj->PartyNumber >= 0)
					{
						gParty.UpdatePKUserInfo(lpObj->PartyNumber, lpObj->m_Index, lpObj->DBNumber, lpObj->m_PK_Level);
						gParty.UpdatePKPartyPanalty(lpObj->PartyNumber);
						GSProtocol.CGPartyListAll(lpObj->PartyNumber);
					}
				}
			}
		}
	}

	else if (lpObj->m_PK_Time > g_ConfigRead.pk.iPKPointIncForMurderer)
	{
		lpObj->m_PK_Time = 0;
		lpObj->m_PK_Count++;
		lpObj->m_PK_Level++;

		GSProtocol.GCPkLevelSend(lpObj->m_Index, lpObj->m_PK_Level);
	}
}




BOOL gObjAngelSprite(LPOBJ lpObj)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return FALSE;
	}

	if ( lpObj->pInventory[8].IsItem() == FALSE )
	{
		return FALSE;
	}

	CItem * Angel = &lpObj->pInventory[8];

	if ( Angel->m_Type == ITEMGET(13,0) && Angel->m_Durability > 0.0f )
	{
		return TRUE;
	}

	return FALSE;
}

BOOL gObjSatanSprite(LPOBJ lpObj)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return FALSE;
	}

	if ( lpObj->pInventory[8].IsItem() == FALSE )
	{
		return FALSE;
	}

	CItem * Satan = &lpObj->pInventory[8];

	if ( Satan->m_Type == ITEMGET(13,1) && Satan->m_Durability > 0.0f )
	{
		return TRUE;
	}

	return FALSE;
}

BOOL gObjDemonSprite(LPOBJ lpObj)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return FALSE;
	}

	if ( lpObj->pInventory[8].IsItem() == FALSE )
	{
		return FALSE;
	}

	CItem * Demon = &lpObj->pInventory[8];

	if ( Demon->m_Type == ITEMGET(13,64) && Demon->m_Durability > 0.0f )
	{
		return TRUE;
	}

	return FALSE;
}

BOOL gObjSafeGuardSprite(LPOBJ lpObj)
{
	if (lpObj->Type != OBJ_USER)
	{
		return FALSE;
	}

	if (lpObj->pInventory[8].IsItem() == FALSE)
	{
		return FALSE;
	}

	CItem * SafeGuard = &lpObj->pInventory[8];

	if (SafeGuard->m_Type == ITEMGET(13, 65) && SafeGuard->m_Durability > 0.0f)
	{
		return TRUE;
	}

	return FALSE;
}

BOOL gObjFindInventoryEquipment(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if ( lpObj->Type != OBJ_USER )
	{
		return FALSE;
	}

	for ( int i = INVETORY_WEAR_SIZE; i < MAIN_INVENTORY_SIZE; i++ )
	{
		gObjSetInventoryEquipment(lpObj, i);
	}

	return TRUE;
}

BOOL gObjSetInventoryEquipment(LPOBJ lpObj, BYTE btPos)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return FALSE;
	}

	if ( lpObj->pInventory[btPos].IsItem() == FALSE )
	{
		return FALSE;
	}

	switch ( lpObj->pInventory[btPos].m_Type )
	{
		case ITEMGET(13,128):
		case ITEMGET(13,129):
			if ( lpObj->pInventory[btPos].m_Durability == 254.0 && (lpObj->m_PlayerData->m_btSculptPos < INVETORY_WEAR_SIZE || lpObj->m_PlayerData->m_btSculptPos >= MAIN_INVENTORY_SIZE) )
				lpObj->m_PlayerData->m_btSculptPos = btPos;
			break;
		case ITEMGET(13,130):
		case ITEMGET(13,131):
		case ITEMGET(13,132):
		case ITEMGET(13,133):
			if ( lpObj->pInventory[btPos].m_Durability == 254.0 && (lpObj->m_PlayerData->m_btCharmPos < INVETORY_WEAR_SIZE || lpObj->m_PlayerData->m_btCharmPos >= MAIN_INVENTORY_SIZE) )
				lpObj->m_PlayerData->m_btCharmPos = btPos;
			break;
		case ITEMGET(13, 134):
			if ( lpObj->pInventory[btPos].m_Durability == 254.0 && (lpObj->m_PlayerData->m_btArtifactPos < INVETORY_WEAR_SIZE || lpObj->m_PlayerData->m_btArtifactPos >= MAIN_INVENTORY_SIZE) )
				lpObj->m_PlayerData->m_btArtifactPos = btPos;
			break;
		case ITEMGET(13,177):
		case ITEMGET(13,178):
		case ITEMGET(13,179):
			if ( lpObj->pInventory[btPos].m_Durability == 254.0 && (lpObj->m_PlayerData->m_btExpUpCharmPos < INVETORY_WEAR_SIZE || lpObj->m_PlayerData->m_btExpUpCharmPos >= MAIN_INVENTORY_SIZE) )
				lpObj->m_PlayerData->m_btExpUpCharmPos = btPos;
			break;
		case ITEMGET(13,2):
		case ITEMGET(13,3):
		case ITEMGET(13,4):
		case ITEMGET(13,37):
			if ( lpObj->pInventory[btPos].m_JewelOfHarmonyOption == 1 && (lpObj->m_btInvenPetPos < 12 || lpObj->m_btInvenPetPos >= 204) )
			{
				lpObj->m_btInvenPetPos = btPos;
				g_Log.Add("[CRenewal][InvenPet][SearchRidePet][%s][%s] [Success] PetIndex:[%d], Pos:[%d]",
					lpObj->AccountID, lpObj->Name, lpObj->pInventory[btPos].m_Type, btPos);
			}

			else
			{
				g_Log.Add("[CRenewal][InvenPet][SearchRidePet][%s][%s] [Fail] Not Use InvenPet",
					lpObj->AccountID, lpObj->Name);
			}
			break;
	}

	return TRUE;
}

BOOL gObjInventoryEquipment(LPOBJ lpObj)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return FALSE;
	}

	if ( lpObj->m_PlayerData->m_btSculptPos >= INVETORY_WEAR_SIZE && lpObj->m_PlayerData->m_btSculptPos < MAIN_INVENTORY_SIZE )
	{
		LPBUFF_EFFECT_DATE lpEffect = g_BuffEffectSlot.GetEffectDataFromItemCode(lpObj->pInventory[lpObj->m_PlayerData->m_btSculptPos].m_Type);

		if(lpEffect == NULL) return FALSE;

		if ( lpObj->pInventory[lpObj->m_PlayerData->m_btSculptPos].m_Durability == 254.0 )
		{
			gObjAddBuffEffect(lpObj, lpEffect->BuffIndex);
		}

		else
		{
			lpObj->m_PlayerData->m_btSculptPos = 0;
			gObjRemoveBuffEffect(lpObj, lpEffect->BuffIndex);
		}
	}

	if ( lpObj->m_PlayerData->m_btCharmPos >= INVETORY_WEAR_SIZE && lpObj->m_PlayerData->m_btCharmPos < MAIN_INVENTORY_SIZE )
	{
		LPBUFF_EFFECT_DATE lpEffect = g_BuffEffectSlot.GetEffectDataFromItemCode(lpObj->pInventory[lpObj->m_PlayerData->m_btCharmPos].m_Type);

		if(lpEffect == NULL) return FALSE;

		if ( lpObj->pInventory[lpObj->m_PlayerData->m_btCharmPos].m_Durability == 254.0 )
		{
			gObjAddBuffEffect(lpObj, lpEffect->BuffIndex);
		}

		else
		{
			lpObj->m_PlayerData->m_btCharmPos = 0;
			gObjRemoveBuffEffect(lpObj, lpEffect->BuffIndex);
		}
	}

	if ( lpObj->m_PlayerData->m_btArtifactPos >= INVETORY_WEAR_SIZE && lpObj->m_PlayerData->m_btArtifactPos < MAIN_INVENTORY_SIZE )
	{
		LPBUFF_EFFECT_DATE lpEffect = g_BuffEffectSlot.GetEffectDataFromItemCode(lpObj->pInventory[lpObj->m_PlayerData->m_btArtifactPos].m_Type);

		if(lpEffect == NULL) return FALSE;

		if ( lpObj->pInventory[lpObj->m_PlayerData->m_btArtifactPos].m_Durability == 254.0 )
		{
			gObjAddBuffEffect(lpObj, lpEffect->BuffIndex);
		}

		else
		{
			lpObj->m_PlayerData->m_btArtifactPos = 0;
			gObjRemoveBuffEffect(lpObj, lpEffect->BuffIndex);
		}
	}

	if ( lpObj->m_PlayerData->m_btExpUpCharmPos >= INVETORY_WEAR_SIZE && lpObj->m_PlayerData->m_btExpUpCharmPos < MAIN_INVENTORY_SIZE )
	{
		LPBUFF_EFFECT_DATE lpEffect = g_BuffEffectSlot.GetEffectDataFromItemCode(lpObj->pInventory[lpObj->m_PlayerData->m_btExpUpCharmPos].m_Type);

		if(lpEffect == NULL) return FALSE;

		if ( lpObj->pInventory[lpObj->m_PlayerData->m_btExpUpCharmPos].m_Durability == 254.0 )
		{
			gObjAddBuffEffect(lpObj, lpEffect->BuffIndex);
		}

		else
		{
			lpObj->m_PlayerData->m_btExpUpCharmPos = 0;
			gObjRemoveBuffEffect(lpObj, lpEffect->BuffIndex);
		}
	}

	if ( lpObj->m_btInvenPetPos >= INVETORY_WEAR_SIZE && lpObj->m_btInvenPetPos < MAIN_INVENTORY_SIZE )
	{
		LPBUFF_EFFECT_DATE lpEffect = g_BuffEffectSlot.GetEffectDataFromItemCode(lpObj->pInventory[lpObj->m_btInvenPetPos].m_Type);

		g_Log.Add("[CRenewal][InvenPet][InvenPetBuff][%s][%s] PetIndex:[%d], Pos:[%d]",
			lpObj->AccountID, lpObj->Name, lpObj->pInventory[lpObj->m_btInvenPetPos].m_Type, lpObj->m_btInvenPetPos);

		if(lpEffect == NULL) 
		{
			g_Log.Add("[CRenewal][InvenPet][InvenPetBuff][%s][%s] PetIndex:[%d], Pos:[%d] [Fail] lpBuffData is Null",
				lpObj->AccountID, lpObj->Name, lpObj->pInventory[lpObj->m_btInvenPetPos].m_Type, lpObj->m_btInvenPetPos);
			return FALSE;
		}

		if ( lpObj->pInventory[lpObj->m_btInvenPetPos].m_JewelOfHarmonyOption == 1 )
		{
			if ( lpObj->m_wInvenPet != (WORD)-1 )
			{
				return FALSE;
			}

			gObjAddBuffEffect(lpObj, lpEffect->BuffIndex);

			g_Log.Add("[CRenewal][InvenPet][ReqInvenPetBuff][%s][%s] PetIndex:[%d], Pos:[%d] [Success] Register InvenPet Buff",
				lpObj->AccountID, lpObj->Name, lpObj->pInventory[lpObj->m_btInvenPetPos].m_Type, lpObj->m_btInvenPetPos);

			CItem * lpItem = &lpObj->pInventory[lpObj->m_btInvenPetPos];

			if ( lpItem->m_Option1 )
			{
				int addskill = gObjWeaponMagicAdd(lpObj, lpItem->m_Special[0], lpItem->m_Level);

				if ( addskill >= 0)
				{
					GSProtocol.GCMagicListOneSend(lpObj->m_Index, addskill, lpItem->m_Special[0], lpItem->m_Level, 0, 0);
				}
			}

			lpObj->m_wInvenPet = lpObj->pInventory[lpObj->m_btInvenPetPos].m_Type;
		}

		else
		{
			gObjRemoveBuffEffect(lpObj, lpEffect->BuffIndex);

			g_Log.Add("[CRenewal][InvenPet][RemoveInvenPetBuff][%s][%s] PetIndex:[%d], Pos:[%d] [Success] Remove InvenPet Buff",
				lpObj->AccountID, lpObj->Name, lpObj->pInventory[lpObj->m_btInvenPetPos].m_Type, lpObj->m_btInvenPetPos);

			int result = gObjMagicDel(lpObj, lpObj->pInventory[lpObj->m_btInvenPetPos].m_Special[0], lpObj->pInventory[lpObj->m_btInvenPetPos].m_Level);

			if ( result >= 0 )
			{
				GSProtocol.GCMagicListOneDelSend(lpObj->m_Index, result, lpObj->pInventory[lpObj->m_btInvenPetPos].m_Special[0], lpObj->pInventory[lpObj->m_btInvenPetPos].m_Level, 0, 0);
			}

			lpObj->m_btInvenPetPos = 0;
		}
	}

	return TRUE;
}

BOOL gObjWingSprite(LPOBJ lpObj)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return FALSE;
	}

	if ( lpObj->pInventory[7].IsItem() == FALSE )
	{
		return FALSE;
	}

	CItem * Wing = &lpObj->pInventory[7];

	if (GetWingType(lpObj->pInventory[7].m_Type, WCF_ITEMNUMBER) != NONE_WING)
	{
		if ( Wing->m_Durability > 0.0f )
		{
			return TRUE;
		}
	}
	
	return FALSE;
}




BOOL gObjUniriaSprite(LPOBJ lpObj)
{
	if ( lpObj->m_btInvenPetPos == 0 || lpObj->m_wInvenPet == (WORD)-1 )
	{
		return FALSE;
	}

	CItem * Uniria = & lpObj->pInventory[lpObj->m_btInvenPetPos];

	if ( Uniria->m_Type == ITEMGET(13,2) && Uniria->m_Durability > 0.0f )
	{
		return TRUE;
	}

	return FALSE;
}

void gObjSpriteDamage(LPOBJ lpObj, int damage)
{
	if ( lpObj->Type != OBJ_USER )
	{
		return;
	}

	int send_dur = 0;

	if ( lpObj->pInventory[8].IsItem() == FALSE )
	{
		return;
	}

	if ( lpObj->pInventory[8].IsPeriodItem() == TRUE )
	{
		return;
	}

	float fN = 10.0f;
	CItem * sprite = &lpObj->pInventory[8];
	float fdamage = damage;

	if ( sprite->m_Type == ITEMGET(13,64) && g_ConfigRead.pet.DamageDisableForPet[DAMAGE_OFF_DEMON] == false)
	{
		fdamage = (damage*3)/10.0f;
		fdamage /= fN;
		sprite->m_Durability -= fdamage;
		send_dur = 1;
	}

	if ( sprite->m_Type == ITEMGET(13,106) && g_ConfigRead.pet.DamageDisableForPet[DAMAGE_OFF_UNICORN] == false)
	{
		fdamage = (damage*3)/10.0f;
		fdamage /= fN;
		sprite->m_Durability -= fdamage;
		send_dur = 1;
	}

	if ( sprite->m_Type == ITEMGET(13,123) && g_ConfigRead.pet.DamageDisableForPet[DAMAGE_OFF_SKELETON] == false)
	{
		fdamage = (damage*3)/10.0f;
		fdamage /= fN;
		sprite->m_Durability -= fdamage;
		send_dur = 1;
	}

	if ( sprite->m_Type == ITEMGET(13,65) && g_ConfigRead.pet.DamageDisableForPet[DAMAGE_OFF_SPIRIT_ANGEL] == false)
	{
		fdamage = (damage*2)/10.0f;
		fdamage /= fN;
		sprite->m_Durability -= fdamage;
		send_dur = 1;
	}

	if ( sprite->m_Type == ITEMGET(13,67) && g_ConfigRead.pet.DamageDisableForPet[DAMAGE_OFF_RUDOLF] == false)
	{
		if (lpObj->Class == CLASS_RAGEFIGHTER)
		{
			if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsDownDur3_Monk > 0.0)
			{
				fdamage = damage / lpObj->m_PlayerData->m_MPSkillOpt.iMpsDownDur3_Monk;
			}
		}

		else if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsDownDur3 > 0.0)
		{
			fdamage = damage / lpObj->m_PlayerData->m_MPSkillOpt.iMpsDownDur3;
		}

		fdamage = (fdamage*3)/10.0f;
		fdamage /= fN;
		sprite->m_Durability -= fdamage;
		send_dur = 1;
	}

	if ( sprite->m_Type == ITEMGET(13,80) && g_ConfigRead.pet.DamageDisableForPet[DAMAGE_OFF_PANDA] == false)
	{
		fdamage = (damage*2)/10.0f;
		fdamage /= fN;
		sprite->m_Durability -= fdamage;
		send_dur = 1;
	}

	if ( sprite->m_Type == ITEMGET(13,0) && g_ConfigRead.pet.DamageDisableForPet[DAMAGE_OFF_ANGEL] == false) // angel
	{
		if (lpObj->Class == CLASS_RAGEFIGHTER)
		{
			if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsDownDur3_Monk > 0.0)
			{
				fdamage = damage / lpObj->m_PlayerData->m_MPSkillOpt.iMpsDownDur3_Monk;
			}
		}

		else if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsDownDur3 > 0.0)
		{
			fdamage = damage / lpObj->m_PlayerData->m_MPSkillOpt.iMpsDownDur3;
		}

		fdamage = (damage*3)/10.0f;
		fdamage /= fN;
		sprite->m_Durability -= fdamage;
		send_dur = 1;
	}
	else if ( sprite->m_Type == ITEMGET(13,1) && g_ConfigRead.pet.DamageDisableForPet[DAMAGE_OFF_SATAN] == false)
	{
		if (lpObj->Class == CLASS_RAGEFIGHTER)
		{
			if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsDownDur3_Monk > 0.0)
			{
				fdamage = damage / lpObj->m_PlayerData->m_MPSkillOpt.iMpsDownDur3_Monk;
			}
		}

		else if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsDownDur3 > 0.0)
		{
			fdamage = damage / lpObj->m_PlayerData->m_MPSkillOpt.iMpsDownDur3;
		}

		fdamage = (fdamage*2)/10.0f;
		fdamage /= fN;
		sprite->m_Durability -= fdamage;
		send_dur = 1;
	}

	if ( sprite->m_Durability < 0.0f )
	{
		sprite->m_Durability = 0.0f; // fixed durability error (previously gs was sending negative values)
	}

	if ( send_dur != FALSE )
	{
		GSProtocol.GCItemDurSend(lpObj->m_Index, 8, sprite->m_Durability, 0);

		if ( sprite->m_Durability < 1.0f )
		{
			int itemnum = sprite->m_Type;

			g_Log.Add("[%s][%s] %s+%d Item is broken because durability is exhausted", lpObj->AccountID, lpObj->Name,
				lpObj->pInventory[8].GetName(), lpObj->pInventory[8].m_Level);
			gObjInventoryDeleteItem(lpObj->m_Index, 8);
			GSProtocol.GCInventoryItemDeleteSend(lpObj->m_Index, 8, 1); // flag changed ( 0 -> 1 )
			gObjMakePreviewCharSet(lpObj->m_Index);
			GSProtocol.GCEquipmentChange(lpObj->m_Index, 8);

			if ( BC_MAP_RANGE(lpObj->MapNumber) || CC_MAP_RANGE(lpObj->MapNumber) || lpObj->MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL )
			{
				if ( itemnum == ITEMGET(13,0) || itemnum == ITEMGET(13,1))
				{
					int pos = -1;

					for ( int i=MAX_PLAYER_EQUIPMENT;i<MAIN_INVENTORY_SIZE;i++)
					{
						if ( lpObj->pInventory[i].IsItem() == TRUE )
						{
							if ( lpObj->pInventory[i].m_Type == itemnum )
							{
								pos = i;
								break;
							}
						}
					}

					if ( pos != -1 )
					{
						BYTE ItemInfo[MAX_ITEM_INFO];
						BOOL DurSend;
						BOOL DurTargetSend;

						ItemByteConvert(ItemInfo, lpObj->pInventory[pos]);
						gObjInventoryMoveItem(lpObj->m_Index, pos, 8, DurSend, DurTargetSend, 0, 0, ItemInfo);
						GSProtocol.GCItemMoveResultSend(lpObj->m_Index, 0, 8, ItemInfo);
						GSProtocol.GCInventoryItemDeleteSend(lpObj->m_Index, pos, 0);
						GSProtocol.GCServerMsgStringSend(Lang.GetText(0,82), lpObj->m_Index, 1);
					}
				}
			}
				
			if ( lpObj->MapNumber == MAP_INDEX_ICARUS && itemnum == ITEMGET(13,3) || lpObj->MapNumber == MAP_INDEX_ICARUS && itemnum == ITEMGET(13,37))
			{
				int pos = -1;

				for ( int i=MAX_PLAYER_EQUIPMENT;i<MAIN_INVENTORY_SIZE;i++)
				{
					if ( lpObj->pInventory[i].IsItem() == TRUE )
					{
						if ( lpObj->pInventory[i].m_Type == ITEMGET(13,3) )
						{
							pos = i;
							break;
						}
					}
				}

				if ( pos == -1 )
				{
					if ( lpObj->pInventory[7].IsItem() == FALSE )
					{
						gObjMoveGate(lpObj->m_Index, 22);
					}
				}
				else
				{
					BYTE ItemInfo[MAX_ITEM_INFO];
					BOOL DurSend;
					BOOL DurTargetSend;

					ItemByteConvert(ItemInfo, lpObj->pInventory[pos]);
					gObjInventoryMoveItem(lpObj->m_Index, pos, 8, DurSend, DurTargetSend, 0, 0, ItemInfo);
					GSProtocol.GCItemMoveResultSend(lpObj->m_Index, 0, 8, ItemInfo);
					GSProtocol.GCInventoryItemDeleteSend(lpObj->m_Index, pos, 0);
					GSProtocol.GCServerMsgStringSend(Lang.GetText(0,82), lpObj->m_Index, 1);
				}
			}

			GCItemListSend(lpObj->m_Index); // with this, user CAN NOT bug uniria
			g_PeriodItemEx.OnRequestPeriodItemList(lpObj);
		}
	}
}




void gObjSecondDurDown(LPOBJ lpObj)
{
	lpObj->m_TimeCount +=1;

	int ret = 0;
	int reCalCharacter = 0;
	int	item_num[5];
	item_num[0] = 2;
	item_num[1] = 3;
	item_num[2] = 4;
	item_num[3] = 5;
	item_num[4] = 6;
	if(lpObj->m_Change >= 0)
	{
		gObjChangeDurProc(lpObj);
	}

	BYTE attr = MapC[lpObj->MapNumber].GetAttr(lpObj->X,lpObj->Y);

	for (int i = 0; i < 5; i++)
	{
		CItem * DurItem = &lpObj->pInventory[item_num[i]];
		ret = 0;
		// ----
		if (g_LuckyItemManager.IsLuckyItemEquipment(DurItem->m_Type))
		{
			ret = DurItem->LuckyItemArmorDurabilityDown(1, lpObj->m_Index);
			// ----
			if (ret != 0)
			{
				GSProtocol.GCItemDurSend2(lpObj->m_Index, (BYTE)item_num[i], (int)DurItem->m_Durability, 0);
			}
			// ----
			if (ret == 2)
			{
				reCalCharacter = 1;
			}
		}
	}

	if(lpObj->m_TimeCount % 10 != 0)
	{
		return;
	}

	if(lpObj->pInventory[7].IsItem() == 1)
	{
		ret = lpObj->pInventory[7].DurabilityDown(1,lpObj->m_Index);
		
		if(ret != 0)
		{
			GSProtocol.GCItemDurSend2(lpObj->m_Index,7,lpObj->pInventory[7].m_Durability,0);
		}

		if(ret == 2)
		{
			reCalCharacter = 1;
		}
	}

	if((attr&1)==0)
	{
		if(lpObj->pInventory[10].IsItem() == 1 && lpObj->pInventory[10].m_PeriodItemOption != 1)
		{
			if((lpObj->pInventory[10].m_Type == ITEMGET(13,20) && lpObj->pInventory[10].m_Level == 0) ||
			   (lpObj->pInventory[10].m_Type == ITEMGET(13,107) && lpObj->pInventory[10].m_Level == 0))
			{
				ret = lpObj->pInventory[10].DurabilityDown(70,lpObj->m_Index);
				if(ret != 0)
				{
					GSProtocol.GCItemDurSend2(lpObj->m_Index,10,lpObj->pInventory[10].m_Durability,0);
				}

				if(ret == 2)
				{
					reCalCharacter = 1;
				}
			}

			else if(lpObj->pInventory[10].m_Type == ITEMGET(13,38))
			{
				ret = lpObj->pInventory[10].DurabilityDown(63,lpObj->m_Index);
				if(ret != 0)
				{
					GSProtocol.GCItemDurSend2(lpObj->m_Index,10,lpObj->pInventory[10].m_Durability,0);
				}
			}

			else if(lpObj->pInventory[10].m_Type == ITEMGET(13,169))
			{
				ret = lpObj->pInventory[10].DurabilityDown(70,lpObj->m_Index);
				if(ret != 0)
				{
					GSProtocol.GCItemDurSend2(lpObj->m_Index,10,lpObj->pInventory[10].m_Durability,0);
				}

				if(ret == 2)
				{
					reCalCharacter = 1;
				}
			}

			else if(lpObj->pInventory[10].m_Type == ITEMGET(13,170))
			{
				ret = lpObj->pInventory[10].DurabilityDown(70,lpObj->m_Index);
				if(ret != 0)
				{
					GSProtocol.GCItemDurSend2(lpObj->m_Index,10,lpObj->pInventory[10].m_Durability,0);
				}

				if(ret == 2)
				{
					reCalCharacter = 1;
				}
			}

			else if(lpObj->pInventory[10].m_Type == ITEMGET(13,268))
			{
				ret = lpObj->pInventory[10].DurabilityDown(70,lpObj->m_Index);
				if(ret != 0)
				{
					GSProtocol.GCItemDurSend2(lpObj->m_Index,10,lpObj->pInventory[10].m_Durability,0);
				}

				if(ret == 2)
				{
					reCalCharacter = 1;
				}
			}

			else if(lpObj->pInventory[10].m_Type != ITEMGET(13,10))
			{
				ret = lpObj->pInventory[10].DurabilityDown(1,lpObj->m_Index);
				if(ret != 0)
				{
					GSProtocol.GCItemDurSend2(lpObj->m_Index,10,lpObj->pInventory[10].m_Durability,0);
				}

				if(ret == 2)
				{
					reCalCharacter = 1;
				}
			}
		}

		if(lpObj->pInventory[11].IsItem() == 1 && lpObj->pInventory[11].m_PeriodItemOption != 1)
		{
			if((lpObj->pInventory[11].m_Type == ITEMGET(13,20) && lpObj->pInventory[11].m_Level == 0) ||
			   (lpObj->pInventory[11].m_Type == ITEMGET(13,107) && lpObj->pInventory[11].m_Level == 0))
			{
				ret = lpObj->pInventory[11].DurabilityDown(70,lpObj->m_Index);
				if(ret != 0)
				{
					GSProtocol.GCItemDurSend2(lpObj->m_Index,11,lpObj->pInventory[11].m_Durability,0);
				}

				if(ret == 2)
				{
					reCalCharacter = 1;
				}
			}
			else if(lpObj->pInventory[11].m_Type == ITEMGET(13,38))
			{
				ret = lpObj->pInventory[11].DurabilityDown(63,lpObj->m_Index);
				if(ret != 0)
				{
					GSProtocol.GCItemDurSend2(lpObj->m_Index,11,lpObj->pInventory[11].m_Durability,0);
				}
			}

			else if(lpObj->pInventory[11].m_Type == ITEMGET(13,169))
			{
				ret = lpObj->pInventory[11].DurabilityDown(70,lpObj->m_Index);
				if(ret != 0)
				{
					GSProtocol.GCItemDurSend2(lpObj->m_Index,11,lpObj->pInventory[11].m_Durability,0);
				}

				if(ret == 2)
				{
					reCalCharacter = 1;
				}
			}

			else if(lpObj->pInventory[11].m_Type == ITEMGET(13,170))
			{
				ret = lpObj->pInventory[11].DurabilityDown(70,lpObj->m_Index);
				if(ret != 0)
				{
					GSProtocol.GCItemDurSend2(lpObj->m_Index,11,lpObj->pInventory[11].m_Durability,0);
				}

				if(ret == 2)
				{
					reCalCharacter = 1;
				}
			}

			else if(lpObj->pInventory[11].m_Type == ITEMGET(13,268))
			{
				ret = lpObj->pInventory[11].DurabilityDown(70,lpObj->m_Index);
				if(ret != 0)
				{
					GSProtocol.GCItemDurSend2(lpObj->m_Index,11,lpObj->pInventory[11].m_Durability,0);
				}

				if(ret == 2)
				{
					reCalCharacter = 1;
				}
			}

			else if(lpObj->pInventory[11].m_Type != ITEMGET(13,10))
			{
				ret = lpObj->pInventory[11].DurabilityDown(1,lpObj->m_Index);
				if(ret != 0)
				{
					GSProtocol.GCItemDurSend2(lpObj->m_Index,11,lpObj->pInventory[11].m_Durability,0);
				}

				if(ret == 2)
				{
					reCalCharacter = 1;
				}
			}
		}

		if(lpObj->pInventory[9].IsItem() == 1 && lpObj->pInventory[9].m_PeriodItemOption != 1)
		{
			if(lpObj->pInventory[9].m_Type != ITEMGET(13,10))
			{
				ret = lpObj->pInventory[9].DurabilityDown(1,lpObj->m_Index);

				if(ret != 0)
				{
					GSProtocol.GCItemDurSend2(lpObj->m_Index,9,lpObj->pInventory[9].m_Durability,0);
				}

				if(ret == 2)
				{
					reCalCharacter = 1;
				}
			}
		}
	}

	if(reCalCharacter != 0)
	{
		gObjCalCharacter.CalcCharacter(lpObj->m_Index);
	}
}

void gObjChangeDurProc(LPOBJ lpObj) //season 4.5 add-on
{
	if(lpObj->pInventory[10].IsItem() == 1 && lpObj->pInventory[10].IsPeriodItem() == FALSE)
	{
		if(lpObj->pInventory[10].m_Type == ITEMGET(13,10))
		{
			int m_Durability = lpObj->pInventory[10].m_Durability;

			lpObj->pInventory[10].m_Durability -= (float)0.02;

			if(m_Durability > (int)lpObj->pInventory[10].m_Durability)
			{
				if(lpObj->pInventory[10].m_Durability < 1)
				{
					lpObj->pInventory[10].m_Durability = 0;
				}

				BYTE dur = lpObj->pInventory[10].m_Durability;

				GSProtocol.GCItemDurSend(lpObj->m_Index,10,dur,0);

				if(dur == 0)
				{
					gObjUseSkill.SkillChangeUse(lpObj->m_Index);
					g_Log.Add("[%s][%s] %s+%d %d Item broken because durability exhausted",lpObj->AccountID,lpObj->Name,lpObj->pInventory[10].GetName(),lpObj->pInventory[10].m_Level,dur);
					lpObj->pInventory[10].Clear();
					GSProtocol.GCInventoryItemDeleteSend(lpObj->m_Index,10,0);
				}
			}
		}
		else if(lpObj->pInventory[10].m_Type == ITEMGET(13,39))
		{
			int m_Durability = lpObj->pInventory[10].m_Durability;

			lpObj->pInventory[10].m_Durability -= (float)0.02;

			if(m_Durability > (int)lpObj->pInventory[10].m_Durability)
			{
				if(lpObj->pInventory[10].m_Durability < 1)
				{
					lpObj->pInventory[10].m_Durability = 0;
				}

				BYTE dur = lpObj->pInventory[10].m_Durability;

				GSProtocol.GCItemDurSend(lpObj->m_Index,10,dur,0);

				if(dur == 0)
				{
					gObjUseSkill.SkillChangeUse(lpObj->m_Index);
					g_Log.Add("[%s][%s] %s+%d %d Item broken because durability exhausted",lpObj->AccountID,lpObj->Name,lpObj->pInventory[10].GetName(),lpObj->pInventory[10].m_Level,dur);
					lpObj->pInventory[10].Clear();
					GSProtocol.GCInventoryItemDeleteSend(lpObj->m_Index,10,0);
				}
			}
		}
		//Second Edition
		else if(lpObj->pInventory[10].m_Type == ITEMGET(13,40))
		{
			int m_Durability = lpObj->pInventory[10].m_Durability;

			lpObj->pInventory[10].m_Durability -= (float)0.02;

			if(m_Durability > (int)lpObj->pInventory[10].m_Durability)
			{
				if(lpObj->pInventory[10].m_Durability < 1)
				{
					lpObj->pInventory[10].m_Durability = 0;
				}

				BYTE dur = lpObj->pInventory[10].m_Durability;

				GSProtocol.GCItemDurSend(lpObj->m_Index,10,dur,0);

				if(dur == 0)
				{
					gObjUseSkill.SkillChangeUse(lpObj->m_Index);
					g_Log.Add("[%s][%s] %s+%d %d Item broken because durability exhausted",lpObj->AccountID,lpObj->Name,lpObj->pInventory[10].GetName(),lpObj->pInventory[10].m_Level,dur);
					lpObj->pInventory[10].Clear();
					GSProtocol.GCInventoryItemDeleteSend(lpObj->m_Index,10,0);
				}
			}
		}
		//Season 2.5 add-on
		else if(lpObj->pInventory[10].m_Type == ITEMGET(13,41)) //Season 2.5 Santa Girl Ring
		{
			int m_Durability = lpObj->pInventory[10].m_Durability;

			lpObj->pInventory[10].m_Durability -= (float)0.02;

			if(m_Durability > (int)lpObj->pInventory[10].m_Durability)
			{
				if(lpObj->pInventory[10].m_Durability < 1)
				{
					lpObj->pInventory[10].m_Durability = 0;
				}

				BYTE dur = lpObj->pInventory[10].m_Durability;

				GSProtocol.GCItemDurSend(lpObj->m_Index,10,dur,0);

				if(dur == 0)
				{
					gObjUseSkill.SkillChangeUse(lpObj->m_Index);
					g_Log.Add("[%s][%s] %s+%d %d Item broken because durability exhausted",lpObj->AccountID,lpObj->Name,lpObj->pInventory[10].GetName(),lpObj->pInventory[10].m_Level,dur);
					lpObj->pInventory[10].Clear();
					GSProtocol.GCInventoryItemDeleteSend(lpObj->m_Index,10,0);
				}
			}
		}
		else if(lpObj->pInventory[10].m_Type == ITEMGET(13,68)) //Season 4.0 XMas Ring
		{
			int m_Durability = lpObj->pInventory[10].m_Durability;

			lpObj->pInventory[10].m_Durability -= (float)0.02;

			if(m_Durability > (int)lpObj->pInventory[10].m_Durability)
			{
				if(lpObj->pInventory[10].m_Durability < 1)
				{
					lpObj->pInventory[10].m_Durability = 0;
				}

				BYTE dur = lpObj->pInventory[10].m_Durability;

				GSProtocol.GCItemDurSend(lpObj->m_Index,10,dur,0);

				if(dur == 0)
				{
					gObjUseSkill.SkillChangeUse(lpObj->m_Index);
					g_Log.Add("[%s][%s] %s+%d %d Item broken because durability exhausted",lpObj->AccountID,lpObj->Name,lpObj->pInventory[10].GetName(),lpObj->pInventory[10].m_Level,dur);
					lpObj->pInventory[10].Clear();
					GSProtocol.GCInventoryItemDeleteSend(lpObj->m_Index,10,0);
				}
			}
		}
		else if((lpObj->pInventory[10].m_Type == ITEMGET(13,76) || lpObj->pInventory[10].m_Type == ITEMGET(13,77) || lpObj->pInventory[10].m_Type == ITEMGET(13,78))
			&& lpObj->pInventory[10].m_PeriodItemOption == 0) //Season 4.6 Panda Ring
		{
			int m_Durability = lpObj->pInventory[10].m_Durability;

			lpObj->pInventory[10].m_Durability -= (float)0.02;

			if(m_Durability > (int)lpObj->pInventory[10].m_Durability)
			{
				if(lpObj->pInventory[10].m_Durability < 1)
				{
					lpObj->pInventory[10].m_Durability = 0;
				}

				BYTE dur = lpObj->pInventory[10].m_Durability;

				GSProtocol.GCItemDurSend(lpObj->m_Index,10,dur,0);

				if(dur == 0)
				{
					gObjUseSkill.SkillChangeUse(lpObj->m_Index);
					g_Log.Add("[%s][%s] %s+%d %d Item broken because durability exhausted",lpObj->AccountID,lpObj->Name,lpObj->pInventory[10].GetName(),lpObj->pInventory[10].m_Level,dur);
					lpObj->pInventory[10].Clear();
					GSProtocol.GCInventoryItemDeleteSend(lpObj->m_Index,10,0);
				}
			}
		}
		else if(lpObj->pInventory[10].m_Type == ITEMGET(13,122) && lpObj->pInventory[10].m_PeriodItemOption == 0) //Season 5.0 Skeleton Ring
		{
			int m_Durability = lpObj->pInventory[10].m_Durability;

			lpObj->pInventory[10].m_Durability -= (float)0.02;

			if(m_Durability > (int)lpObj->pInventory[10].m_Durability)
			{
				if(lpObj->pInventory[10].m_Durability < 1)
				{
					lpObj->pInventory[10].m_Durability = 0;
				}

				BYTE dur = lpObj->pInventory[10].m_Durability;

				GSProtocol.GCItemDurSend(lpObj->m_Index,10,dur,0);

				if(dur == 0)
				{
					gObjUseSkill.SkillChangeUse(lpObj->m_Index);
					g_Log.Add("[%s][%s] %s+%d %d Item broken because durability exhausted",lpObj->AccountID,lpObj->Name,lpObj->pInventory[10].GetName(),lpObj->pInventory[10].m_Level,dur);
					lpObj->pInventory[10].Clear();
					GSProtocol.GCInventoryItemDeleteSend(lpObj->m_Index,10,0);
				}
			}
		}
	}

	if(lpObj->pInventory[11].IsItem() == 1 && lpObj->pInventory[11].IsPeriodItem() == FALSE)
	{
		if(lpObj->pInventory[11].m_Type == ITEMGET(13,10))
		{
			int m_Durability = lpObj->pInventory[11].m_Durability;

			lpObj->pInventory[11].m_Durability -= (float)0.02;

			if(m_Durability > (int)lpObj->pInventory[11].m_Durability)
			{
				if(lpObj->pInventory[11].m_Durability < 1)
				{
					lpObj->pInventory[11].m_Durability = 0;
				}

				BYTE dur = lpObj->pInventory[11].m_Durability;

				GSProtocol.GCItemDurSend(lpObj->m_Index,11,dur,0);

				if(dur == 0)
				{
					gObjUseSkill.SkillChangeUse(lpObj->m_Index);
					g_Log.Add("[%s][%s] %s+%d %d Item broken because durability exhausted",lpObj->AccountID,lpObj->Name,lpObj->pInventory[10].GetName(),lpObj->pInventory[11].m_Level,dur); //wz bug
					lpObj->pInventory[11].Clear();
					GSProtocol.GCInventoryItemDeleteSend(lpObj->m_Index,11,0);
				}
			}
		}
		else if(lpObj->pInventory[11].m_Type == ITEMGET(13,39))
		{
			int m_Durability = lpObj->pInventory[11].m_Durability;

			lpObj->pInventory[11].m_Durability -= (float)0.02;

			if(m_Durability > (int)lpObj->pInventory[11].m_Durability)
			{
				if(lpObj->pInventory[11].m_Durability < 1)
				{
					lpObj->pInventory[11].m_Durability = 0;
				}

				BYTE dur = lpObj->pInventory[11].m_Durability;

				GSProtocol.GCItemDurSend(lpObj->m_Index,11,dur,0);

				if(dur == 0)
				{
					gObjUseSkill.SkillChangeUse(lpObj->m_Index);
					g_Log.Add("[%s][%s] %s+%d %d Item broken because durability exhausted",lpObj->AccountID,lpObj->Name,lpObj->pInventory[11].GetName(),lpObj->pInventory[11].m_Level,dur);
					lpObj->pInventory[11].Clear();
					GSProtocol.GCInventoryItemDeleteSend(lpObj->m_Index,11,0);
				}
			}
		}
		//Second Edition
		else if(lpObj->pInventory[11].m_Type == ITEMGET(13,40))
		{
			int m_Durability = lpObj->pInventory[11].m_Durability;

			lpObj->pInventory[11].m_Durability -= (float)0.02;

			if(m_Durability > (int)lpObj->pInventory[11].m_Durability)
			{
				if(lpObj->pInventory[11].m_Durability < 1)
				{
					lpObj->pInventory[11].m_Durability = 0;
				}

				BYTE dur = lpObj->pInventory[11].m_Durability;

				GSProtocol.GCItemDurSend(lpObj->m_Index,11,dur,0);

				if(dur == 0)
				{
					gObjUseSkill.SkillChangeUse(lpObj->m_Index);
					g_Log.Add("[%s][%s] %s+%d %d Item broken because durability exhausted",lpObj->AccountID,lpObj->Name,lpObj->pInventory[11].GetName(),lpObj->pInventory[11].m_Level,dur);
					lpObj->pInventory[11].Clear();
					GSProtocol.GCInventoryItemDeleteSend(lpObj->m_Index,11,0);
				}
			}
		}
		//Season 2.5 add-on
		else if(lpObj->pInventory[11].m_Type == ITEMGET(13,41)) //Season 2.5 Santa Girl Ring
		{
			int m_Durability = lpObj->pInventory[11].m_Durability;

			lpObj->pInventory[11].m_Durability -= (float)0.02;

			if(m_Durability > (int)lpObj->pInventory[11].m_Durability)
			{
				if(lpObj->pInventory[11].m_Durability < 1)
				{
					lpObj->pInventory[11].m_Durability = 0;
				}

				BYTE dur = lpObj->pInventory[11].m_Durability;

				GSProtocol.GCItemDurSend(lpObj->m_Index,11,dur,0);

				if(dur == 0)
				{
					gObjUseSkill.SkillChangeUse(lpObj->m_Index);
					g_Log.Add("[%s][%s] %s+%d %d Item broken because durability exhausted",lpObj->AccountID,lpObj->Name,lpObj->pInventory[11].GetName(),lpObj->pInventory[11].m_Level,dur);
					lpObj->pInventory[11].Clear();
					GSProtocol.GCInventoryItemDeleteSend(lpObj->m_Index,11,0);
				}
			}
		}

		else if(lpObj->pInventory[11].m_Type == ITEMGET(13,68)) //Season 4.0 XMas Ring
		{
			int m_Durability = lpObj->pInventory[11].m_Durability;

			lpObj->pInventory[11].m_Durability -= (float)0.02;

			if(m_Durability > (int)lpObj->pInventory[11].m_Durability)
			{
				if(lpObj->pInventory[11].m_Durability < 1)
				{
					lpObj->pInventory[11].m_Durability = 0;
				}

				BYTE dur = lpObj->pInventory[11].m_Durability;

				GSProtocol.GCItemDurSend(lpObj->m_Index,11,dur,0);

				if(dur == 0)
				{
					gObjUseSkill.SkillChangeUse(lpObj->m_Index);
					g_Log.Add("[%s][%s] %s+%d %d Item broken because durability exhausted",lpObj->AccountID,lpObj->Name,lpObj->pInventory[11].GetName(),lpObj->pInventory[11].m_Level,dur);
					lpObj->pInventory[11].Clear();
					GSProtocol.GCInventoryItemDeleteSend(lpObj->m_Index,11,0);
				}
			}
		}

		else if((lpObj->pInventory[11].m_Type == ITEMGET(13,76) || lpObj->pInventory[11].m_Type == ITEMGET(13,77) || lpObj->pInventory[11].m_Type == ITEMGET(13,78))
			&& lpObj->pInventory[11].m_PeriodItemOption == 0) //Season 4.6 Panda Ring
		{
			int m_Durability = lpObj->pInventory[11].m_Durability;

			lpObj->pInventory[11].m_Durability -= (float)0.02;

			if(m_Durability > (int)lpObj->pInventory[11].m_Durability)
			{
				if(lpObj->pInventory[11].m_Durability < 1)
				{
					lpObj->pInventory[11].m_Durability = 0;
				}

				BYTE dur = lpObj->pInventory[11].m_Durability;

				GSProtocol.GCItemDurSend(lpObj->m_Index,11,dur,0);

				if(dur == 0)
				{
					gObjUseSkill.SkillChangeUse(lpObj->m_Index);
					g_Log.Add("[%s][%s] %s+%d %d Item broken because durability exhausted",lpObj->AccountID,lpObj->Name,lpObj->pInventory[11].GetName(),lpObj->pInventory[11].m_Level,dur);
					lpObj->pInventory[11].Clear();
					GSProtocol.GCInventoryItemDeleteSend(lpObj->m_Index,11,0);
				}
			}
		}

		else if(lpObj->pInventory[11].m_Type == ITEMGET(13,122) && lpObj->pInventory[11].m_PeriodItemOption == 0) //Season 5.0 Skeleton Ring
		{
			int m_Durability = lpObj->pInventory[11].m_Durability;

			lpObj->pInventory[11].m_Durability -= (float)0.02;

			if(m_Durability > (int)lpObj->pInventory[11].m_Durability)
			{
				if(lpObj->pInventory[11].m_Durability < 1)
				{
					lpObj->pInventory[11].m_Durability = 0;
				}

				BYTE dur = lpObj->pInventory[11].m_Durability;

				GSProtocol.GCItemDurSend(lpObj->m_Index,11,dur,0);

				if(dur == 0)
				{
					gObjUseSkill.SkillChangeUse(lpObj->m_Index);
					g_Log.Add("[%s][%s] %s+%d %d Item broken because durability exhausted",lpObj->AccountID,lpObj->Name,lpObj->pInventory[11].GetName(),lpObj->pInventory[11].m_Level,dur);
					lpObj->pInventory[11].Clear();
					GSProtocol.GCInventoryItemDeleteSend(lpObj->m_Index,11,0);
				}
			}
		}
	}
}

void gObjWingDurProc(LPOBJ lpObj) 
{
	BYTE send_dur=0;

	// ??? ?????????? ?????? ?? ???..
	return;

	if( lpObj->pInventory[7].IsItem() == TRUE )
	{	
		int dur = (int)lpObj->pInventory[7].m_Durability;
			lpObj->pInventory[7].m_Durability -= (float)0.0002;
			
			if( dur > (int)lpObj->pInventory[7].m_Durability ) 
			{
				if( lpObj->pInventory[7].m_Durability < 1 ) 
					lpObj->pInventory[7].m_Durability = 0;

				send_dur = (BYTE)lpObj->pInventory[7].m_Durability;
				GSProtocol.GCItemDurSend(lpObj->m_Index, 7, send_dur, 0);
				if( send_dur == 0 ) 
				{
					g_Log.Add("[%s][%s] %s+%d %d Item broken because durability exhausted", lpObj->AccountID, lpObj->Name, lpObj->pInventory[7].GetName(), lpObj->pInventory[7].m_Level, send_dur);
					lpObj->pInventory[7].Clear();
					GSProtocol.GCInventoryItemDeleteSend(lpObj->m_Index, 7, 0);
				}
			}
			//g_Log.Add("???? ?????? %f",lpObj->pInventory[7].m_Durability);
			return;
	}
}


void gObjPenaltyDurDown(LPOBJ lpObj, LPOBJ lpTargetObj)	// Functio n Not USed
{
	int ret = 0;
	int reCalCharacter = 0;

	int irand = rand()%2;

	CItem * Weapon = &lpObj->pInventory[irand];
	int decdur = 1;

	switch(lpObj->Class)
	{
	case 0:
		decdur = lpObj->Level - (lpTargetObj->Level * 38) / 20;
		break;
	case 1:
		decdur = lpObj->Level - (lpTargetObj->Level * 37) / 20;
		break;
	case 2:
		decdur = lpObj->Level - (lpTargetObj->Level * 36) / 20;
		break;
	case 3:
	case 4:
		decdur = lpObj->Level - (lpTargetObj->Level * 37) / 20;
		break;
	default : break;
	}

	decdur /= 10;

	if(Weapon->m_Type != ITEMGET(4,7) && Weapon->m_Type != ITEMGET(4,15))
	{
		if(Weapon->m_Type < ITEMGET(6,0) && Weapon->m_Type >= ITEMGET(7,0) )
		{
			if ( Weapon->IsItem() )
			{
				ret = Weapon->DurabilityDown2(decdur,lpObj->m_Index);
			}
		}
		else
		{
			CItem * Weapon = &lpObj->pInventory[irand];

			if(Weapon->IsItem())
			{
				ret = Weapon->DurabilityDown2(decdur,lpObj->m_Index);
			}
		}
	}

	if(ret != 0)
	{
		GSProtocol.GCItemDurSend2(lpObj->m_Index,irand,lpObj->pInventory[irand].m_Durability,0);
	}

	if(ret == 2)
	{
		reCalCharacter = 1;
	}

	int item_num[7] = {0,1,2,3,4,5,6};
	int item_pos = rand()%7;

	CItem * DurItem = &lpObj->pInventory[item_num[item_pos]];

	if(DurItem->m_Type != ITEMGET(4,7) && DurItem->m_Type != ITEMGET(4,15))
	{
		if(item_pos < 2)
		{
			if(DurItem->m_Type >= ITEMGET(6,0) && DurItem->m_Type < ITEMGET(7,0))
			{
				ret = DurItem->DurabilityDown2(decdur,lpObj->m_Index);
	
				if(ret != 0)
				{
					GSProtocol.GCItemDurSend2(lpObj->m_Index,item_num[item_pos],DurItem->m_Durability,0);
	
					if(ret == 2)
					{
						reCalCharacter = 1;
					}
				}
			}
		}
		else
		{
			ret = DurItem->DurabilityDown2(decdur,lpObj->m_Index);

			if(ret != 0)
			{
				GSProtocol.GCItemDurSend2(lpObj->m_Index,item_num[item_pos],DurItem->m_Durability,0);

				if(ret == 2)
				{
					reCalCharacter = 1;
				}
			}
		}
	}

	CItem * Wing = &lpObj->pInventory[7];

	if(Wing->IsItem())
	{
		ret = DurItem->DurabilityDown2(decdur,lpObj->m_Index);

		if(ret != 0)
		{
			GSProtocol.GCItemDurSend2(lpObj->m_Index,item_num[item_pos],DurItem->m_Durability,0);
			if(ret == 2)
			{
				reCalCharacter = 1;
			}
		}
	}

	if(reCalCharacter != 0)
	{
		gObjCalCharacter.CalcCharacter(lpObj->m_Index);
	}
}




void gObjWeaponDurDown(LPOBJ lpObj, LPOBJ lpTargetObj, int type)
{
	if(lpObj->Type == OBJ_MONSTER || lpObj->Type == OBJ_NPC)
	{
		return;
	}

	if(gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_CHARM_PROTECTITEM) != 0)
	{
		return;
	}

	if (g_MineSystem.IsEquipPickax(lpObj->m_Index))
	{
		return;
	}

	int itargetdefence = 0;
	CItem * Right = &lpObj->pInventory[0];
	CItem * Left = &lpObj->pInventory[1];
	int ret = 0;
	int iOption1, idefense = 0;
	if(type == 0)
	{
		itargetdefence = lpTargetObj->m_Defense;

		if((lpObj->Class == 1 || lpObj->Class == 3 || lpObj->Class == 4)
			&& (Right->m_Type >= 0 && Right->m_Type < ITEMGET(4,0) 
				&& Left->m_Type >= 0 && Left->m_Type < ITEMGET(4,0)))
		{
			if(rand()%2==0)
			{
				ret = Left->NormalWeaponDurabilityDown(itargetdefence,lpObj->m_Index);
				
				if(ret != 0)
				{
					GSProtocol.GCItemDurSend(lpObj->m_Index,1,Left->m_Durability,0);
					if(ret == 2)
					{
						gObjCalCharacter.CalcCharacter(lpObj->m_Index);
					}
				}
			}
			else
			{
				ret = Right->NormalWeaponDurabilityDown(itargetdefence,lpObj->m_Index);
				if(ret != 0)
				{
					GSProtocol.GCItemDurSend(lpObj->m_Index,0,Right->m_Durability,0);
					if(ret==2)
					{
						gObjCalCharacter.CalcCharacter(lpObj->m_Index);
					}
				}
			}

			return;
		}
		else if(Left->m_Type >= ITEMGET(4,0) && Left->m_Type < ITEMGET(4,7) || Left->m_Type == ITEMGET(4,20) || Left->m_Type == ITEMGET(4,21))
		{
			ret = Left->BowWeaponDurabilityDown(itargetdefence,lpObj->m_Index);

			if(ret != 0)
			{
				GSProtocol.GCItemDurSend(lpObj->m_Index,1,Left->m_Durability,0);
				if(ret==2)
				{
					gObjCalCharacter.CalcCharacter(lpObj->m_Index);
				}
			}
		}
		else if(Right->m_Type >= ITEMGET(4,8) && Right->m_Type < ITEMGET(4,15) || Right->m_Type >= ITEMGET(4,16) && Right->m_Type < ITEMGET(5,0))
		{
			ret = Right->BowWeaponDurabilityDown(itargetdefence,lpObj->m_Index);

			if(ret != 0)
			{
				GSProtocol.GCItemDurSend(lpObj->m_Index,0,Right->m_Durability,0);
				if(ret==2)
				{
					gObjCalCharacter.CalcCharacter(lpObj->m_Index);
				}
			}
		}
		else if(Right->m_Type >= ITEMGET(0,0) && Right->m_Type < ITEMGET(4,0))
		{
			ret = Right->NormalWeaponDurabilityDown(itargetdefence,lpObj->m_Index);

			if(ret != 0)
			{
				GSProtocol.GCItemDurSend(lpObj->m_Index,0,Right->m_Durability,0);
				if(ret==2)
				{
					gObjCalCharacter.CalcCharacter(lpObj->m_Index);
				}
			}
		}
		else if(Right->m_Type >= ITEMGET(5,0) && Right->m_Type < ITEMGET(6,0))
		{
			ret = Right->StaffWeaponDurabilityDown(itargetdefence,lpObj->m_Index);

			if(ret != 0)
			{
				GSProtocol.GCItemDurSend(lpObj->m_Index,0,Right->m_Durability,0);
				if(ret==2)
				{
					gObjCalCharacter.CalcCharacter(lpObj->m_Index);
				}
			}
		}
	return;
	}

	if(lpTargetObj->Type == OBJ_MONSTER || lpTargetObj->Type == OBJ_NPC)
	{
		itargetdefence = lpTargetObj->m_MagicDefense;
	}
	else
	{
		itargetdefence = lpTargetObj->m_Defense;
	}

	if(Right->m_Type >= ITEMGET(5,0) && Right->m_Type < ITEMGET(6,0))
	{
		ret = Right->StaffWeaponDurabilityDown(itargetdefence,lpObj->m_Index);
		if(ret != 0)
		{
			GSProtocol.GCItemDurSend(lpObj->m_Index,0,Right->m_Durability,0);
			if(ret==2)
			{
				gObjCalCharacter.CalcCharacter(lpObj->m_Index);
			}
		}
	}
}




void gObjArmorRandomDurDown(LPOBJ lpObj, LPOBJ lpAttackObj)
{
	int item_num[7] = {0,1,2,3,4,5,6};
	int item_pos = rand()%7;

	if(lpObj->Type != OBJ_USER)
	{
		return;
	}

	if(gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_CHARM_PROTECTITEM) != 0)
	{
		return;
	}

	CItem * DurItem = &lpObj->pInventory[item_num[item_pos]];
	int damagemin = lpAttackObj->m_AttackDamageMin;
	int ret;

	if(item_pos < 2)
	{
		if(DurItem->m_Type >= ITEMGET(6,0) && DurItem->m_Type < ITEMGET(7,0))
		{
			ret = DurItem->ArmorDurabilityDown(damagemin,lpObj->m_Index);

			if(ret != 0)
			{
				GSProtocol.GCItemDurSend(lpObj->m_Index,item_num[item_pos],DurItem->m_Durability,0);
				if(ret == 2)
				{
					gObjCalCharacter.CalcCharacter(lpObj->m_Index);
				}
			}
			return;
		}
	}
	else
	{
		if(DurItem->m_Type != ITEMGET(4,7) && DurItem->m_Type != ITEMGET(4,15))
		{
			ret = DurItem->ArmorDurabilityDown(damagemin,lpObj->m_Index);

			if(ret != 0)
			{
				GSProtocol.GCItemDurSend(lpObj->m_Index,item_num[item_pos],DurItem->m_Durability,0);
				if(ret == 2)
				{
					gObjCalCharacter.CalcCharacter(lpObj->m_Index);
				}
			}
		}
	}
}




bool gObjIsSelfDefense(LPOBJ lpObj, int aTargetIndex)
{
	if ( !ObjectMaxRange(aTargetIndex) )
	{
		return false;
	}

	for ( int n=0;n<MAX_SELF_DEFENSE;n++)
	{
		if ( lpObj->SelfDefense[n] >= 0 )
		{
			if ( lpObj->SelfDefense[n] == aTargetIndex )
			{
				return true;
			}
		}
	}

	return false;
}




void gObjCheckSelfDefense(LPOBJ lpObj, int aTargetIndex)
{
	if(!ObjectMaxRange(aTargetIndex))
	{
		return;
	}

	if(gObj[aTargetIndex].m_PK_Level > 4)
	{
		return;
	}

	for(int n = 0; n < MAX_SELF_DEFENSE;n++)
	{
		if(gObj[aTargetIndex].SelfDefense[n] >= 0)
		{
			if(gObj[aTargetIndex].SelfDefense[n]==lpObj->m_Index)
			{
				return;
			}
		}
	}

	int blank = -1;

	for(int n = 0; n < MAX_SELF_DEFENSE;n++)
	{
		if(lpObj->SelfDefense[n] >= 0)
		{
			if(lpObj->SelfDefense[n] == aTargetIndex)
			{
				lpObj->SelfDefenseTime[n] = GetTickCount()+60000;
				return;
			}
		}
		else
		{
			blank = n;
		}
	}

	if(blank < 0)
	{
		g_Log.Add("Self-defense count exceeds");
		return;
	}

	lpObj->MySelfDefenseTime = GetTickCount();
	lpObj->SelfDefense[blank] = aTargetIndex;
	lpObj->SelfDefenseTime[blank] = GetTickCount() + 60000;

	char szTemp[64];

	wsprintf(szTemp,Lang.GetText(0,27),lpObj->Name,gObj[aTargetIndex].Name);

	GSProtocol.GCServerMsgStringSend(szTemp,lpObj->m_Index,1);
	GSProtocol.GCServerMsgStringSend(szTemp,aTargetIndex,1);

	g_Log.Add("[%s][%s] Set SelfDefence [%s][%s]",lpObj->AccountID,lpObj->Name,gObj[aTargetIndex].AccountID,gObj[aTargetIndex].Name);
}




void gObjTimeCheckSelfDefense(LPOBJ lpObj)
{
	char szTemp[64];

	for ( int n=0;n<MAX_SELF_DEFENSE;n++)
	{
		if ( lpObj->SelfDefense[n] >= 0 )
		{
			if ( GetTickCount() > lpObj->SelfDefenseTime[n] )
			{
				wsprintf(szTemp, Lang.GetText(0,28), lpObj->Name);
				GSProtocol.GCServerMsgStringSend(szTemp, lpObj->m_Index, 1);
				GSProtocol.GCServerMsgStringSend(szTemp, lpObj->SelfDefense[n], 1);		

				g_Log.Add("[%s][%s] ReSet SelfDefence [%s][%s]",
					lpObj->AccountID, lpObj->Name,
					gObj[lpObj->SelfDefense[n]].AccountID, gObj[lpObj->SelfDefense[n]].Name);

				lpObj->SelfDefense[n] = -1;
				
			}
		}
	}
}




BOOL gObjAttack(LPOBJ lpObj, LPOBJ lpTargetObj, CMagicInf * lpMagic, BOOL magicsend, BYTE MSBFlag, int AttackDamage, BOOL bCombo, BYTE RFAttack, BYTE byReflect, BYTE byPentagramAttack)
{
	return gclassObjAttack.Attack(lpObj, lpTargetObj, lpMagic, magicsend, MSBFlag, AttackDamage, bCombo, RFAttack, byReflect, byPentagramAttack);
}

void gObjMonsterExpDivision(LPOBJ lpMonObj, LPOBJ lpObj, int AttackDamage, int MSBFlag)
{
	gObjMonsterHitDamageUserDel(lpMonObj);
	lpMonObj->MonsterMoneyDrop = 0;

	int tObjNum;
	UINT64 exp;
	UINT64 nDropZen = 0;
	LPOBJ lpTargetObj;

	for(int n = 0; n < MAX_ST_HIT_DAMAGE;n++)
	{
		if(lpMonObj->sHD[n].number >= 0)
		{
			tObjNum = lpMonObj->sHD[n].number;
			lpTargetObj = &gObj[tObjNum];

			if(lpMonObj->sHD[n].HitDamage > 0)
			{
				bool bSendExp = 1;

				exp = gObjMonsterExpSingle(lpTargetObj,lpMonObj,lpMonObj->sHD[n].HitDamage,lpMonObj->MaxLife,bSendExp, nDropZen);

				if(exp > 0)
				{
					if(bSendExp == 1)
					{
						if(lpTargetObj->m_Index == lpObj->m_Index)
						{
							if(g_MasterLevelSkillTreeSystem.IsMasterLevelUser(lpObj))
							{
								GSProtocol.GCKillPlayerMasterExpSend(lpTargetObj->m_Index,lpMonObj->m_Index,exp,AttackDamage,MSBFlag);
							}

							else
							{
								GSProtocol.GCKillPlayerExpSend(lpTargetObj->m_Index,lpMonObj->m_Index,exp,AttackDamage,MSBFlag);
							}
						}
					}
				}

				if (ItemDrop.IsZenDropActive(lpMonObj->MapNumber) == true)
				{
					DWORD dwZenDropMoney = ItemDrop.GetZenAmount(lpMonObj->MapNumber, lpMonObj->Level);

					if (dwZenDropMoney > 0)
					{
						lpMonObj->MonsterMoneyDrop = dwZenDropMoney;
					}
				}

				else
				{
					lpMonObj->MonsterMoneyDrop = nDropZen;
				}

				if ((lpObj->pInventory[10].IsItem() == TRUE && lpObj->pInventory[10].m_Type == ITEMGET(13,76) && lpObj->pInventory[10].IsPeriodItemExpire() == FALSE) ||
					lpObj->pInventory[11].IsItem() == TRUE && lpObj->pInventory[11].m_Type == ITEMGET(13,76) && lpObj->pInventory[11].IsPeriodItemExpire() == FALSE)
				{
					lpMonObj->MonsterMoneyDrop *= 1.5;
				}

				else if ((lpObj->pInventory[10].IsItem() == TRUE && lpObj->pInventory[10].m_Type == ITEMGET(13,77) && lpObj->pInventory[10].IsPeriodItemExpire() == FALSE) ||
					lpObj->pInventory[11].IsItem() == TRUE && lpObj->pInventory[11].m_Type == ITEMGET(13,77) && lpObj->pInventory[11].IsPeriodItemExpire() == FALSE)
				{
					lpMonObj->MonsterMoneyDrop *= 1.5;
				}

				else if ((lpObj->pInventory[10].IsItem() == TRUE && lpObj->pInventory[10].m_Type == ITEMGET(13,78) && lpObj->pInventory[10].IsPeriodItemExpire() == FALSE) ||
					lpObj->pInventory[11].IsItem() == TRUE && lpObj->pInventory[11].m_Type == ITEMGET(13,78) && lpObj->pInventory[11].IsPeriodItemExpire() == FALSE)
				{
					lpMonObj->MonsterMoneyDrop *= 1.5;
				}

				else if ((lpObj->pInventory[10].IsItem() == TRUE && lpObj->pInventory[10].m_Type == ITEMGET(13,166) && lpObj->pInventory[10].IsPeriodItemExpire() == FALSE) ||
					lpObj->pInventory[11].IsItem() == TRUE && lpObj->pInventory[11].m_Type == ITEMGET(13,166) && lpObj->pInventory[11].IsPeriodItemExpire() == FALSE)
				{
					lpMonObj->MonsterMoneyDrop *= 1.5;
				}

				else if (lpObj->pInventory[8].IsItem() == TRUE && lpObj->pInventory[8].m_Type == ITEMGET(13, 106) && lpObj->pInventory[8].m_Durability > 0.0)
				{
					lpMonObj->MonsterMoneyDrop *= 1.5;
				}
			}
		}
	}
}

struct _stHitPartyInfo
{
	_stHitPartyInfo()
	{
		this->nPartyNumber = -1;
		this->nHitDamage = 0;
	}

	int nPartyNumber;
	int nHitDamage;
};

void gObjMonsterExpDivisionRenewal(LPOBJ lpMonObj, LPOBJ lpObj, int AttackDamage, int MSBFlag)
{
	_stHitPartyInfo stHitParty[40];

	bool bSendExp;
	bool bFindPartySlot = false;
	int nZeroIndex = -1;
	bool bIsFindZeroIndex = false;
	bool bIsPartyDamage = false;
	UINT64 nDropZen = 0;

	for (int n = 0; n < MAX_ST_HIT_DAMAGE; n++)
	{
		if (lpMonObj->sHD[n].number >= 0)
		{
			int nTargetNumber = lpMonObj->sHD[n].number;
			LPOBJ lpTargetObj = &gObj[nTargetNumber];

			if (gObj[nTargetNumber].PartyNumber < 0)
			{
				bSendExp = true;
				UINT64 nTempDropZen = gObjMonsterExpSingleRenewal(lpTargetObj, lpMonObj, lpMonObj->sHD[n].HitDamage, lpMonObj->MaxLife, bSendExp);

				if (lpObj->m_Index == lpMonObj->TargetNumber)
				{
					nDropZen = nTempDropZen;
				}
			}

			else
			{
				for (int k = 0; k < 40; k++)
				{
					if (stHitParty[k].nPartyNumber == lpTargetObj->PartyNumber)
					{
						stHitParty[k].nHitDamage += lpMonObj->sHD[n].HitDamage;
						stHitParty[k].nPartyNumber = lpTargetObj->PartyNumber;
						bFindPartySlot = true;
					}

					if (stHitParty[k].nPartyNumber == -1)
					{
						if (bIsFindZeroIndex == false)
						{
							nZeroIndex = k;
							bIsFindZeroIndex = true;
						}
					}
				}

				if (bFindPartySlot == false)
				{
					if (nZeroIndex >= 0 && nZeroIndex < MAX_ST_HIT_DAMAGE)
					{
						stHitParty[nZeroIndex].nHitDamage = lpMonObj->sHD[n].HitDamage;
						stHitParty[nZeroIndex].nPartyNumber = lpTargetObj->PartyNumber;
					}
				}

				bIsPartyDamage = true;
			}
		}
	}

	if (bIsPartyDamage)
	{
		for (int i = 0; i < 40; i++)
		{
			if (stHitParty[i].nPartyNumber >= 0)
			{
				UINT64 nTempDropZen = gObjExpPartyRenewal(stHitParty[i].nPartyNumber, lpObj->m_Index, lpMonObj, stHitParty[i].nHitDamage, lpMonObj->MaxLife, 1);

				if (nTempDropZen > 0)
					nDropZen = nTempDropZen;
			}
		}
	}

	if (lpMonObj->Type == OBJ_MONSTER)
	{
		if (ItemDrop.IsZenDropActive(lpMonObj->MapNumber) == true)
		{
			DWORD dwZenDropMoney = ItemDrop.GetZenAmount(lpMonObj->MapNumber, lpMonObj->Level);

			if (dwZenDropMoney > 0)
			{
				lpMonObj->MonsterMoneyDrop = dwZenDropMoney;
			}
		}

		else
		{
			lpMonObj->MonsterMoneyDrop = nDropZen;
		}

		if ((lpObj->pInventory[10].IsItem() == TRUE && lpObj->pInventory[10].m_Type == ITEMGET(13,76) && lpObj->pInventory[10].IsPeriodItemExpire() == FALSE) ||
			(lpObj->pInventory[11].IsItem() == TRUE && lpObj->pInventory[11].m_Type == ITEMGET(13,76) && lpObj->pInventory[11].IsPeriodItemExpire() == FALSE))
		{
			lpMonObj->MonsterMoneyDrop *= 1.5;
		}

		if ((lpObj->pInventory[10].IsItem() == TRUE && lpObj->pInventory[10].m_Type == ITEMGET(13,77) && lpObj->pInventory[10].IsPeriodItemExpire() == FALSE) ||
			(lpObj->pInventory[11].IsItem() == TRUE && lpObj->pInventory[11].m_Type == ITEMGET(13,77) && lpObj->pInventory[11].IsPeriodItemExpire() == FALSE))
		{
			lpMonObj->MonsterMoneyDrop *= 1.5;
		}

		if ((lpObj->pInventory[10].IsItem() == TRUE && lpObj->pInventory[10].m_Type == ITEMGET(13,78) && lpObj->pInventory[10].IsPeriodItemExpire() == FALSE) ||
			(lpObj->pInventory[11].IsItem() == TRUE && lpObj->pInventory[11].m_Type == ITEMGET(13,78) && lpObj->pInventory[11].IsPeriodItemExpire() == FALSE))
		{
			lpMonObj->MonsterMoneyDrop *= 1.5;
		}
	}
}



UINT64 gObjMonsterExpSingle(LPOBJ lpObj, LPOBJ lpTargetObj, int dmg, int tot_dmg, bool& bSendExp, UINT64& nDropZen)
{
	if(lpObj->Type != OBJ_USER)
	{
		g_Log.AddC(TColor::Red, "[ERROR] lpObj->Type != OBJ_USER (%s)(%d)", __FILE__, __LINE__);
		return 0;
	}

	if (g_ConfigRead.server.GetServerType() == SERVER_BATTLECORE) // exp is disabled on BattleCore
	{
		return 0;
	}

	if (g_MasterLevelSkillTreeSystem.CheckMLGetExp(lpObj, lpTargetObj) == FALSE)
	{
		bSendExp = false;
		return 0;
	}

	if(DS_MAP_RANGE(lpObj->MapNumber))
	{
		nDropZen = g_DevilSquare.gObjMonsterExpSingle(lpObj,lpTargetObj,dmg,tot_dmg);
		return nDropZen;
	}

	UINT64 exp;
	UINT64 maxexp = 0;

	int level = (lpTargetObj->Level+25)*lpTargetObj->Level/3;
	int userlevel = lpObj->Level;
	
	if (g_MasterLevelSkillTreeSystem.IsMasterLevelUser(lpObj) == TRUE)
	{
		userlevel += lpObj->m_PlayerData->MasterLevel;
	}

	if((lpTargetObj->Level+10) < userlevel)
	{
		level = level*(lpTargetObj->Level+10)/userlevel;
	}

	if(lpTargetObj->Level >= 65)
	{
		level += (lpTargetObj->Level-64)*(lpTargetObj->Level/4);
	}

	if(level > 0)
	{
		maxexp = level/2;
	}

	else
	{
		level = 0;
	}

	if(maxexp < 1)
	{
		exp = level;
	}
	else
	{
		exp = maxexp / 2 + level;
	}

	exp = dmg * exp / tot_dmg;

	if (BC_MAP_RANGE(lpObj->MapNumber))
	{
		exp = exp * 50 / 100;
	}

	if(CC_MAP_RANGE(lpObj->MapNumber))
	{
		exp *= g_ChaosCastle.GetExperienceBonus(lpObj->m_cChaosCastleIndex);
	}

	UINT64 dwDefaultExp = exp;
	nDropZen = exp;
	float fVipBonus = g_VipSystem.GetExpBonus(lpObj);
	float fMapBonus = 0.0;
	float fEventBonus = 0.0;
	float fBaseExp = 0.0;

	if(!g_MasterLevelSkillTreeSystem.IsMasterLevelUser(lpObj))
	{
		fEventBonus = g_BonusEvent.GetAddExp();
		fMapBonus = g_MapAttr.GetExpBonus(lpObj->MapNumber);
		fBaseExp = g_ExpManager.GetExpMultiplier(lpObj->Level, lpObj->m_PlayerData->m_iResets, false);
	}

	else
	{
		fEventBonus = g_BonusEvent.GetAddMLExp();
		fMapBonus = g_MapAttr.GetMasterExpBonus(lpObj->MapNumber);
		fBaseExp = g_ExpManager.GetExpMultiplier(lpObj->Level + lpObj->m_PlayerData->MasterLevel, lpObj->m_PlayerData->m_iResets, true);
	}

	exp *= (fBaseExp + fVipBonus + fEventBonus + fMapBonus);

	if (g_CrywolfSync.GetOccupationState() == 1 && g_CrywolfSync.GetApplyPenalty() == TRUE)
	{
		exp =  (exp * g_CrywolfSync.GetGettingExpPenaltyRate()) / 100;
	}

	if (DG_MAP_RANGE(lpObj->MapNumber))
	{
		if (g_MasterLevelSkillTreeSystem.IsMasterLevelUser(lpObj))
		{
			exp *= g_DoppelGanger.GetDoppelGangerMLExpRate();
		}

		else
		{
			exp *= g_DoppelGanger.GetDoppelGangerExpRate();
		}
	}

	if (exp > 0)
	{
		CheckItemOptForGetExpExRenewal(lpObj, lpTargetObj, exp, dwDefaultExp, false);

		if (g_MasterLevelSkillTreeSystem.IsMasterLevelUser(lpObj))
		{
			lpObj->m_PlayerData->MasterExperience += exp;
		}
		else
		{
			lpObj->m_PlayerData->Experience += exp;
		}

		if (gObjLevelUp(lpObj, exp, lpTargetObj->Class, "Single") == 0)
		{
			bSendExp = 0;
		}
	}

	return exp;
}

UINT64 gObjMonsterExpSingleRenewal(LPOBJ lpObj, LPOBJ lpTargetObj, int dmg, int tot_dmg, bool &bSendExp)
{
	if (!lpObj || !lpTargetObj)
	{
		return 0;
	}

	if (lpObj->Type != OBJ_USER)
	{
		g_Log.AddC(TColor::Red, "[ERROR] lpObj->Type != OBJ_USER (%s)(%d)", __FILE__, __LINE__);
		return 0;
	}

	if (g_ConfigRead.server.GetServerType() == SERVER_BATTLECORE) // exp is disabled on BattleCore
	{
		return 0;
	}

	UINT64 nExp = 0;
	int nTotalLevel = lpObj->Level + lpObj->m_PlayerData->MasterLevel;
	int nMonsterLevel = lpTargetObj->Level;

	if (g_MasterLevelSkillTreeSystem.CheckMLGetExp(lpObj, lpTargetObj) == FALSE)
	{
		bSendExp = false;
		return 0;
	}

	nMonsterLevel *= 4;

	if (nTotalLevel > nMonsterLevel)
	{
		nExp = ((15 * nMonsterLevel) + (15 * nTotalLevel) * 0.3 / 200.0 * (nMonsterLevel - nTotalLevel));
	}

	else
	{
		nExp = ((15 * nTotalLevel) + (15 * nTotalLevel) * 0.3 / 200.0 * (nMonsterLevel - nTotalLevel));
	}

	if (nExp <= 0)
	{
		nExp = 1;
	}

	if (dmg > tot_dmg)
	{
		dmg = tot_dmg;
	}

	nExp = dmg * nExp / tot_dmg;

	if (BC_MAP_RANGE(lpObj->MapNumber))
	{
		nExp = nExp * 50 / 100;
	}

	UINT64 dwDefaultExp = nExp;
	UINT64 nDropZen = nExp;
	float fVipBonus = g_VipSystem.GetExpBonus(lpObj);
	float fMapBonus = 0.0;
	float fEventBonus = 0.0;
	float fBaseExp = 0.0;

	if (!g_MasterLevelSkillTreeSystem.IsMasterLevelUser(lpObj))
	{
		fEventBonus = g_BonusEvent.GetAddExp();
		fMapBonus = g_MapAttr.GetExpBonus(lpObj->MapNumber);
		fBaseExp = g_ExpManager.GetExpMultiplier(lpObj->Level, lpObj->m_PlayerData->m_iResets, false);
	}

	else
	{
		fEventBonus = g_BonusEvent.GetAddMLExp();
		fMapBonus = g_MapAttr.GetMasterExpBonus(lpObj->MapNumber);
		fBaseExp = g_ExpManager.GetExpMultiplier(lpObj->Level + lpObj->m_PlayerData->MasterLevel, lpObj->m_PlayerData->m_iResets, true);
	}

	nExp *= (fBaseExp + fVipBonus + fEventBonus + fMapBonus);

	if (g_CrywolfSync.GetOccupationState() == 1 && g_CrywolfSync.GetApplyPenalty() == TRUE)
	{
		nExp = (nExp * g_CrywolfSync.GetGettingExpPenaltyRate()) / 100;
	}

	if (nExp > 0)
	{
		CheckItemOptForGetExpExRenewal(lpObj, lpTargetObj, nExp, dwDefaultExp, false);

		if (g_MasterLevelSkillTreeSystem.IsMasterLevelUser(lpObj))
		{
			lpObj->m_PlayerData->MasterExperience += nExp;
		}
		else
		{
			lpObj->m_PlayerData->Experience += nExp;
		}

		if (gObjLevelUp(lpObj, nExp, lpTargetObj->Class, "Single") == 0)
		{
			bSendExp = 0;
		}
	}

	if (bSendExp)
	{
		GSProtocol.GCKillPlayerMasterExpSend(lpObj->m_Index, lpTargetObj->m_Index, nExp, dmg, bSendExp);
	}

	return nDropZen;
}

void gObjExpParty(LPOBJ lpObj, LPOBJ lpTargetObj, int AttackDamage, int MSBFlag)
{
	if (DS_MAP_RANGE(lpObj->MapNumber))
	{
		g_DevilSquare.gObjExpParty(lpObj, lpTargetObj, AttackDamage, MSBFlag);
		return;
	}

	if (g_ConfigRead.server.GetServerType() == SERVER_BATTLECORE) // exp is disabled on BattleCore
	{
		return;
	}

	UINT64 exp;
	UINT64 maxexp = 0;
	UINT64 totalexp;
	int level = (lpTargetObj->Level + 25)*lpTargetObj->Level / 3;
	int number;
	int partynum = lpObj->PartyNumber;
	int totallevel = 0;
	int partylevel;
	int partycount;
	int dis[5];
	int viewplayer = 0;
	int viewpercent = 100;
	int bApplaySetParty = 0;
	BYTE bCheckSetParty[MAX_TYPE_PLAYER];
	int toplevel = 0;

	for (int n = 0; n < MAX_USER_IN_PARTY; n++)
	{
		number = gParty.m_PartyS[partynum].Number[n];

		if (number >= 0)
		{
			LPOBJ lpPartyObj = &gObj[number];

			int dis = gObjCalDistance(lpTargetObj, lpPartyObj); //Season 2.5 add-on

			if (dis < 10) //Season 2.5 add-on
			{
				if (lpPartyObj->Level > toplevel)
				{
					toplevel = lpPartyObj->Level;
				}
			}

			if (dis < 10) //Season 3.0 add-on
			{
				if (lpPartyObj->Level + lpPartyObj->m_PlayerData->MasterLevel > toplevel)
				{
					toplevel = lpPartyObj->Level + lpPartyObj->m_PlayerData->MasterLevel;
				}
			}
		}
	}

	if (!ObjectMaxRange(partynum))
	{
		g_Log.Add("error : %s %d", __FILE__, __LINE__);
		return;
	}

	memset(bCheckSetParty, 0, sizeof(bCheckSetParty));

	partycount = gParty.m_PartyS[partynum].Count;

	for (int n = 0; n < MAX_USER_IN_PARTY; n++)
	{
		number = gParty.m_PartyS[partynum].Number[n];

		if (number >= 0)
		{
			LPOBJ lpPartyObj = &gObj[number];

			if (lpTargetObj->MapNumber == lpPartyObj->MapNumber)
			{
				dis[n] = gObjCalDistance(lpTargetObj, &gObj[number]);

				if (dis[n] < 10)
				{
					lpPartyObj = &gObj[number];

					if (toplevel >= (lpPartyObj->Level + lpPartyObj->m_PlayerData->MasterLevel + 200))
					{
						totallevel += lpPartyObj->Level + lpPartyObj->m_PlayerData->MasterLevel + 200;
					}
					else
					{
						totallevel += lpPartyObj->Level + lpPartyObj->m_PlayerData->MasterLevel;
					}
					viewplayer += 1;
					bCheckSetParty[lpPartyObj->Class] = 1;
				}
			}
		}
	}

	if ((bCheckSetParty[0] != 0 && bCheckSetParty[1] != 0 && bCheckSetParty[2] != 0) ||
		(bCheckSetParty[3] != 0 && bCheckSetParty[4] != 0 && bCheckSetParty[5] != 0) ||
		(bCheckSetParty[4] != 0 && bCheckSetParty[5] != 0 && bCheckSetParty[6] != 0) ||
		(bCheckSetParty[0] != 0 && bCheckSetParty[2] != 0 && bCheckSetParty[7] != 0))
	{
		bApplaySetParty = 1;
	}

	if (viewplayer > 1)
	{
		if (bApplaySetParty != 0)
		{
			if (viewplayer == 3)
			{
				viewpercent = gSetPartyExp3;
			}
			else if (viewplayer == 4)
			{
				viewpercent = gSetPartyExp4;
			}
			else if (viewplayer >= 5)
			{
				viewpercent = gSetPartyExp5;
			}
			else
			{
				viewpercent = gSetPartyExp2;
			}
		}
		else
		{
			if (viewplayer == 2)
			{
				viewpercent = gPartyExp2;
			}
			else if (viewplayer == 3)
			{
				viewpercent = gPartyExp3;
			}
			else if (viewplayer == 4)
			{
				viewpercent = gPartyExp4;
			}
			else if (viewplayer >= 5)
			{
				viewpercent = gPartyExp5;
			}
			else
			{
				viewpercent = gPartyExp1;
			}
		}

		partylevel = totallevel / viewplayer;
	}
	else
	{
		partylevel = totallevel;
	}

	if ((lpTargetObj->Level + 10) < partylevel)
	{
		level = level * (lpTargetObj->Level + 10) / partylevel;
	}

	if (lpTargetObj->Level >= 65)
	{
		level = level + (lpTargetObj->Level - 64)*(lpTargetObj->Level / 4);
	}

	if (level > 0)
	{
		maxexp = level / 2;
	}
	else
	{
		level = 0;
	}

	if (maxexp < 1)
	{
		totalexp = level;
	}
	else
	{
		totalexp = maxexp / 2 + level;
	}

	if (lpTargetObj->Type == OBJ_MONSTER)
	{
		if (ItemDrop.IsZenDropActive(lpTargetObj->MapNumber) == true)
		{
			DWORD dwZenDropMoney = ItemDrop.GetZenAmount(lpTargetObj->MapNumber, lpTargetObj->Level);
			
			if (dwZenDropMoney > 0)
			{
				lpTargetObj->MonsterMoneyDrop = dwZenDropMoney;
			}
		}

		else
		{
			lpTargetObj->MonsterMoneyDrop = totalexp;
		}

		if ((lpObj->pInventory[10].IsItem() == TRUE && lpObj->pInventory[10].m_Type == ITEMGET(13,76) && lpObj->pInventory[10].IsPeriodItemExpire() == FALSE) ||
			lpObj->pInventory[11].IsItem() == TRUE && lpObj->pInventory[11].m_Type == ITEMGET(13,76) && lpObj->pInventory[11].IsPeriodItemExpire() == FALSE)
		{
			lpTargetObj->MonsterMoneyDrop *= 1.5;
		}

		else if ((lpObj->pInventory[10].IsItem() == TRUE && lpObj->pInventory[10].m_Type == ITEMGET(13,77) && lpObj->pInventory[10].IsPeriodItemExpire() == FALSE) ||
			lpObj->pInventory[11].IsItem() == TRUE && lpObj->pInventory[11].m_Type == ITEMGET(13,77) && lpObj->pInventory[11].IsPeriodItemExpire() == FALSE)
		{
			lpTargetObj->MonsterMoneyDrop *= 1.5;
		}

		else if ((lpObj->pInventory[10].IsItem() == TRUE && lpObj->pInventory[10].m_Type == ITEMGET(13,78) && lpObj->pInventory[10].IsPeriodItemExpire() == FALSE) ||
			lpObj->pInventory[11].IsItem() == TRUE && lpObj->pInventory[11].m_Type == ITEMGET(13,78) && lpObj->pInventory[11].IsPeriodItemExpire() == FALSE)
		{
			lpTargetObj->MonsterMoneyDrop *= 1.5;
		}

		else if ((lpObj->pInventory[10].IsItem() == TRUE && lpObj->pInventory[10].m_Type == ITEMGET(13, 166) && lpObj->pInventory[10].IsPeriodItemExpire() == FALSE) ||
			lpObj->pInventory[11].IsItem() == TRUE && lpObj->pInventory[11].m_Type == ITEMGET(13, 166) && lpObj->pInventory[11].IsPeriodItemExpire() == FALSE)
		{
			lpTargetObj->MonsterMoneyDrop *= 1.5;
		}
	}

	for (int n = 0; n < MAX_USER_IN_PARTY; n++)
	{
		number = gParty.m_PartyS[partynum].Number[n];

		if (number >= 0)
		{
			LPOBJ lpPartyObj = &gObj[number];

			if (lpTargetObj->MapNumber == lpPartyObj->MapNumber && dis[n] < 10)
			{
				UINT64 mymaxexp;

				if (!g_MasterLevelSkillTreeSystem.IsMasterLevelUser(lpPartyObj))
				{
					mymaxexp = gLevelExperience[lpPartyObj->Level];
				}

				else
				{
					mymaxexp = lpPartyObj->m_PlayerData->MasterNextExp;
				}

				exp = (totalexp * viewpercent * (lpPartyObj->Level + lpPartyObj->m_PlayerData->MasterLevel)) / totallevel / 100;

				if (g_MasterLevelSkillTreeSystem.CheckMLGetExp(lpPartyObj, lpTargetObj) == FALSE)
				{
					exp = 0;
				}

				if (BC_MAP_RANGE(lpPartyObj->MapNumber))
				{
					exp = exp * 50 / 100;
				}

				if (exp > mymaxexp)
				{
					exp = mymaxexp;
				}

				if (lpPartyObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER)
				{
					exp = 0;
				}

				UINT64 dwDefaultExp = exp;
				float fVipBonus = g_VipSystem.GetExpBonus(lpPartyObj);
				float fMapBonus = 0.0;
				float fEventBonus = 0.0;
				float fBaseExp = 0.0;

				if (!g_MasterLevelSkillTreeSystem.IsMasterLevelUser(lpPartyObj))
				{
					fEventBonus = g_BonusEvent.GetAddExp();
					fMapBonus = g_MapAttr.GetExpBonus(lpPartyObj->MapNumber);
					fBaseExp = g_ExpManager.GetExpMultiplier(lpPartyObj->Level, lpPartyObj->m_PlayerData->m_iResets, false);					
				}

				else
				{
					fEventBonus = g_BonusEvent.GetAddMLExp();
					fMapBonus = g_MapAttr.GetMasterExpBonus(lpPartyObj->MapNumber);
					fBaseExp = g_ExpManager.GetExpMultiplier(lpPartyObj->Level + lpPartyObj->m_PlayerData->MasterLevel, lpPartyObj->m_PlayerData->m_iResets, true);
				}

				exp *= (fBaseExp + fVipBonus + fEventBonus + fMapBonus);

				if (g_CrywolfSync.GetOccupationState() == 1 && g_CrywolfSync.GetApplyPenalty() == TRUE)
				{
					exp = (exp * g_CrywolfSync.GetGettingExpPenaltyRate()) / 100;
				}

				if (DG_MAP_RANGE(lpPartyObj->MapNumber))
				{
					if (g_MasterLevelSkillTreeSystem.IsMasterLevelUser(lpPartyObj))
					{
						exp *= g_DoppelGanger.GetDoppelGangerMLExpRate();
					}

					else
					{
						exp *= g_DoppelGanger.GetDoppelGangerExpRate();
					}
				}

				if (exp > 0)
				{
					CheckItemOptForGetExpExRenewal(lpPartyObj, lpTargetObj, exp, dwDefaultExp, false);

					if (g_MasterLevelSkillTreeSystem.IsMasterLevelUser(lpPartyObj))
					{
						lpPartyObj->m_PlayerData->MasterExperience += exp;
					}

					else
					{
						lpPartyObj->m_PlayerData->Experience += exp;
					}

					if (gObjLevelUp(lpPartyObj, exp, lpTargetObj->Class, "Party") == 0)
					{
						continue;
					}

					GSProtocol.GCKillPlayerMasterExpSend(lpPartyObj->m_Index, lpTargetObj->m_Index, exp, AttackDamage, MSBFlag);
				}
			}
		}
	}
}

UINT64 gObjExpPartyRenewal(int nPartyNumber, int nLastAttackUserIndex, LPOBJ lpTargetObj, int AttackDamage, int nTotalDamage, int MSBFlag)
{
	if (lpTargetObj == NULL)
	{
		return 0;
	}

	if (g_ConfigRead.server.GetServerType() == SERVER_BATTLECORE) // exp is disabled on BattleCore
	{
		return 0;
	}

	int nMonsterLevel = lpTargetObj->Level;
	bool bApplaySetParty = 0;
	int nNumber = 0;
	int nTotalLevel = 0;
	int nTopLevel = 0;
	int nPartyCount = 0;
	int nViewPlayer = 0;
	UINT64 nDropZen = 0;
	UINT64 nTempExp = 0;
	BYTE bCheckSetParty[MAX_TYPE_PLAYER];
	int dis[5];

	if (!lpTargetObj)
	{
		return 0;
	}

	if (!ObjectMaxRange(nPartyNumber))
	{
		return 0;
	}

	memset(bCheckSetParty, 0, sizeof(bCheckSetParty));

	for (int n = 0; n < MAX_USER_IN_PARTY; n++)
	{
		nNumber = gParty.m_PartyS[nPartyNumber].Number[n];

		if (nNumber >= 0)
		{
			if (gObj[nNumber].Connected == PLAYER_PLAYING)
			{
				if (lpTargetObj->MapNumber == gObj[nNumber].MapNumber)
				{
					dis[n] = gObjCalDistance(lpTargetObj, &gObj[nNumber]);

					if (dis[n] < 10)
					{
						LPOBJ lpPartyObj = &gObj[nNumber];

						nViewPlayer++;
						bCheckSetParty[gObj[nNumber].Class] = 1;
						nTotalLevel += lpPartyObj->m_PlayerData->MasterLevel + lpPartyObj->Level;

						if (lpPartyObj->m_PlayerData->MasterLevel + lpPartyObj->Level > nTopLevel)
						{
							nTopLevel = lpPartyObj->m_PlayerData->MasterLevel + lpPartyObj->Level;
						}
					}
				}
			}
		}
	}

	nMonsterLevel *= 4;

	if (nTopLevel > nMonsterLevel)
	{
		nTempExp = ((15 * nMonsterLevel) + (15 * nTopLevel) * 0.3 / 200.0 * (nMonsterLevel - nTopLevel));
	}

	else
	{
		nTempExp = ((15 * nTopLevel) + (15 * nTopLevel) * 0.3 / 200.0 * (nMonsterLevel - nTopLevel));
	}

	if (nTempExp <= 0)
	{
		nTempExp = 1;
	}

	if (AttackDamage > nTotalDamage)
	{
		AttackDamage = nTotalDamage;
	}

	nTempExp = AttackDamage * nTempExp / nTotalDamage;

	nPartyCount = gParty.m_PartyS[nPartyNumber].Count;
	float fPartyCntAddExp;

	if ((bCheckSetParty[0] != 0 && bCheckSetParty[1] != 0 && bCheckSetParty[2] != 0) ||
		(bCheckSetParty[3] != 0 && bCheckSetParty[4] != 0 && bCheckSetParty[5] != 0) ||
		(bCheckSetParty[4] != 0 && bCheckSetParty[5] != 0 && bCheckSetParty[6] != 0) ||
		(bCheckSetParty[0] != 0 && bCheckSetParty[2] != 0 && bCheckSetParty[7] != 0))
	{
		bApplaySetParty = true;
	}

	if (bApplaySetParty)
	{
		switch (nViewPlayer)
		{
		case 3:
			fPartyCntAddExp = 1.05;
			break;
		case 4:
			fPartyCntAddExp = 1.06;
			break;
		case 5:
			fPartyCntAddExp = 1.07;
			break;
		default:
			fPartyCntAddExp = 1.00;
			break;
		}
	}

	else
	{
		switch (nViewPlayer)
		{
		case 2:
			fPartyCntAddExp = 1.02;
			break;
		case 3:
			fPartyCntAddExp = 1.03;
			break;
		case 4:
			fPartyCntAddExp = 1.04;
			break;
		case 5:
			fPartyCntAddExp = 1.05;
			break;
		default:
			fPartyCntAddExp = 1.00;
			break;
		}
	}

	for (int i = 0; i < MAX_USER_IN_PARTY; i++)
	{
		nNumber = gParty.m_PartyS[nPartyNumber].Number[i];

		if (nNumber >= 0)
		{
			if (gObj[nNumber].Connected == PLAYER_PLAYING)
			{
				LPOBJ lpPartyObj = &gObj[nNumber];
				int nMemberLevel = lpPartyObj->m_PlayerData->MasterLevel + lpPartyObj->Level;

				if (lpTargetObj->MapNumber == lpPartyObj->MapNumber)
				{
					if (dis[i] < 10)
					{
						UINT64 nExp;

						if (g_MasterLevelSkillTreeSystem.CheckMLGetExp(lpPartyObj, lpTargetObj) == FALSE)
						{
							nExp = 0;
						}

						else
						{
							UINT64 i64MyMaxExp;

							if (!g_MasterLevelSkillTreeSystem.IsMasterLevelUser(lpPartyObj))
							{
								i64MyMaxExp = gLevelExperience[lpPartyObj->Level];
							}

							else
							{
								i64MyMaxExp = lpPartyObj->m_PlayerData->MasterNextExp;
							}

							nExp = nTempExp * fPartyCntAddExp * nMemberLevel / nTotalLevel;

							if (BC_MAP_RANGE(lpPartyObj->MapNumber))
							{
								nExp = nExp * 50 / 100;
							}

							if (lpPartyObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER)
							{
								nExp = 0;
							}

							UINT64 dwDefaultExp = nExp;
							float fVipBonus = g_VipSystem.GetExpBonus(lpPartyObj);
							float fMapBonus = 0.0;
							float fEventBonus = 0.0;
							float fBaseExp = 0.0;

							if (nLastAttackUserIndex == lpPartyObj->m_Index)
							{
								nDropZen = nExp;
							}

							if (!g_MasterLevelSkillTreeSystem.IsMasterLevelUser(lpPartyObj))
							{
								fEventBonus = g_BonusEvent.GetAddExp();
								fMapBonus = g_MapAttr.GetExpBonus(lpPartyObj->MapNumber);
								fBaseExp = g_ExpManager.GetExpMultiplier(lpPartyObj->Level, lpPartyObj->m_PlayerData->m_iResets, false);								
							}

							else
							{
								fEventBonus = g_BonusEvent.GetAddMLExp();
								fMapBonus = g_MapAttr.GetMasterExpBonus(lpPartyObj->MapNumber);
								fBaseExp = g_ExpManager.GetExpMultiplier(lpPartyObj->Level + lpPartyObj->m_PlayerData->MasterLevel, lpPartyObj->m_PlayerData->m_iResets, true);
							}

							nExp *= (fBaseExp + fVipBonus + fMapBonus + fEventBonus);

							if (g_CrywolfSync.GetOccupationState() == 1 && g_CrywolfSync.GetApplyPenalty() == TRUE)
							{
								nExp = (nExp * g_CrywolfSync.GetGettingExpPenaltyRate()) / 100;
							}

							if (DG_MAP_RANGE(lpPartyObj->MapNumber))
							{
								if (g_MasterLevelSkillTreeSystem.IsMasterLevelUser(lpPartyObj))
								{
									nExp *= g_DoppelGanger.GetDoppelGangerMLExpRate();
								}

								else
								{
									nExp *= g_DoppelGanger.GetDoppelGangerExpRate();
								}
							}

							if (nExp > 0)
							{
								CheckItemOptForGetExpExRenewal(lpPartyObj, lpTargetObj, nExp, dwDefaultExp, false);

								if (g_MasterLevelSkillTreeSystem.IsMasterLevelUser(lpPartyObj))
								{
									lpPartyObj->m_PlayerData->MasterExperience += nExp;
								}

								else
								{
									lpPartyObj->m_PlayerData->Experience += nExp;
								}

								if (gObjLevelUp(lpPartyObj, nExp, lpTargetObj->Class, "Party") == 0)
								{
									continue;
								}
							}

							GSProtocol.GCKillPlayerMasterExpSend(lpPartyObj->m_Index, lpTargetObj->m_Index, nExp, AttackDamage, MSBFlag);
						}
					}
				}
			}
		}
	}

	return nDropZen;
}

void gObjMonsterDieLifePlus(LPOBJ lpObj, LPOBJ lpTartObj)
{
	if(lpTartObj->Level < 0 || lpTartObj->Level > 255)
	{
		return;
	}

	if(lpObj->Type != OBJ_USER)
	{
		return;
	}

	lpObj->Life += lpTartObj->Level;

	COMMON_EXT_OPTION_TYPE * lpDieLifeOption = g_ItemOptionTypeMng.GetCommonExcOption(OPTION_INC_LIFE_MONSTER_DIE);
	COMMON_EXT_OPTION_TYPE * lpDieManaOption = g_ItemOptionTypeMng.GetCommonExcOption(OPTION_INC_MANA_MONSTER_DIE);

	if(lpObj->MonsterDieGetLife != 0 && lpDieLifeOption != NULL)
	{
		lpObj->Life += (lpObj->MaxLife + lpObj->AddLife) / lpDieLifeOption->OptionValue * lpObj->MonsterDieGetLife;
	}

	if(lpObj->MonsterDieGetMana != 0 && lpDieManaOption != NULL)
	{
		lpObj->Mana += (lpObj->MaxMana + lpObj->AddMana) / lpDieManaOption->OptionValue * lpObj->MonsterDieGetMana;
	}

	if (lpObj->m_PlayerData->m_wSocketOptionMonsterDieGetHp > 0) //season4 add-on
	{
		lpObj->Life += lpObj->m_PlayerData->m_wSocketOptionMonsterDieGetHp;
	}

	if (lpObj->m_PlayerData->m_wSocketOptionMonsterDieGetMana > 0) //season4 add-on
	{
		lpObj->Mana += lpObj->m_PlayerData->m_wSocketOptionMonsterDieGetMana;
	}

	if (lpObj->Class == CLASS_RAGEFIGHTER)
	{
		if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsMonsterDieGetLife_Monk > 0.0)
		{
			lpObj->Life = lpObj->Life + ((lpObj->MaxLife + lpObj->AddLife) / lpObj->m_PlayerData->m_MPSkillOpt.iMpsMonsterDieGetLife_Monk);
		}
	}

	else if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsMonsterDieGetLife > 0.0)
	{
		lpObj->Life = lpObj->Life + ((lpObj->MaxLife + lpObj->AddLife) / lpObj->m_PlayerData->m_MPSkillOpt.iMpsMonsterDieGetLife);
	}

	if (lpObj->Class == CLASS_RAGEFIGHTER)
	{
		if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsMonsterDieGetMana_Monk > 0.0)
		{
			lpObj->Mana = lpObj->Mana + ((lpObj->MaxMana + lpObj->AddMana) / lpObj->m_PlayerData->m_MPSkillOpt.iMpsMonsterDieGetMana_Monk);
		}
	}

	else if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsMonsterDieGetMana > 0.0)
	{
		lpObj->Mana = lpObj->Mana + ((lpObj->MaxMana + lpObj->AddMana) / lpObj->m_PlayerData->m_MPSkillOpt.iMpsMonsterDieGetMana);
	}

	if (lpObj->Class == CLASS_RAGEFIGHTER)
	{
		if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsMonsterDieGetSD_Monk > 0.0)
		{
			lpObj->iShield = lpObj->iShield + ((lpObj->iMaxShield + lpObj->iAddShield) / lpObj->m_PlayerData->m_MPSkillOpt.iMpsMonsterDieGetSD_Monk);
		}
	}

	else if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsMonsterDieGetSD > 0.0)
	{
		lpObj->iShield = lpObj->iShield + ((lpObj->iMaxShield + lpObj->iAddShield) / lpObj->m_PlayerData->m_MPSkillOpt.iMpsMonsterDieGetSD);
	}
	
	if(lpObj->Life > (lpObj->MaxLife + lpObj->AddLife))
	{
		lpObj->Life = lpObj->MaxLife + lpObj->AddLife;
	}

	if(lpObj->iShield > (lpObj->iMaxShield + lpObj->iAddShield))
	{
		lpObj->iShield = lpObj->iMaxShield + lpObj->iAddShield;
	}

	GSProtocol.GCReFillSend(lpObj->m_Index, lpObj->Life, -1, 0, lpObj->iShield);

	if(lpObj->MonsterDieGetMana != 0 || lpObj->m_PlayerData->m_wSocketOptionMonsterDieGetMana != 0 || lpObj->m_PlayerData->m_MPSkillOpt.iMpsMonsterDieGetMana != 0.0 ||
		lpObj->m_PlayerData->m_MPSkillOpt.iMpsMonsterDieGetMana_Monk != 0.0)
	{
		if(lpObj->Mana > (lpObj->MaxMana + lpObj->AddMana))
		{
			lpObj->Mana = lpObj->MaxMana + lpObj->AddMana;
		}

		GSProtocol.GCManaSend(lpObj->m_Index,lpObj->Mana,255,0,lpObj->BP);
	}
}

void gObjLifeCheck(LPOBJ lpTargetObj, LPOBJ lpObj, int AttackDamage, int DamageSendType, int MSBFlag, int MSBDamage, WORD Skill, int iShieldDamage, int iElementalDamage)
{
	if(lpObj->Connected < PLAYER_PLAYING)
	{
		return;
	}

	if(ATTRIBUTE_RANGE(lpObj->m_Attribute) || ATTRIBUTE_RANGE(lpTargetObj->m_Attribute))
	{
		return;
	}

	if (lpObj->Class == 689) // Uruk Mountain Traps
	{
		return;
	}

	LPOBJ lpCallObj = lpObj;

	if(lpObj->Type == OBJ_MONSTER && lpObj->m_RecallMon >= 0)
	{
		lpCallObj = &gObj[lpObj->m_RecallMon];
	}

	if(AttackDamage != 0 && lpTargetObj->Type == OBJ_MONSTER)
	{
		if(lpTargetObj->m_RecallMon >= 0)
		{
			GSProtocol.GCRecallMonLife(lpTargetObj->m_RecallMon,lpTargetObj->MaxLife,lpTargetObj->Life);
		}

		gObjMonsterSetHitDamage(lpTargetObj,lpCallObj->m_Index,AttackDamage);
	}

	if(lpTargetObj->Life <= 0 && lpTargetObj->Live != 0)
	{
		if(DamageSendType == 0)
		{
			GSProtocol.GCDamageSend(lpCallObj->m_Index,lpTargetObj->m_Index,AttackDamage,MSBFlag,MSBDamage,iShieldDamage);
		}
		else if(DamageSendType == 1)
		{
			GSProtocol.GCDamageSendPoison(lpTargetObj->m_Index,AttackDamage,iShieldDamage);
		}
		else if(DamageSendType == 2)
		{
			GSProtocol.GCDamageSend(lpCallObj->m_Index,lpTargetObj->m_Index,AttackDamage,MSBFlag,5,iShieldDamage);
		}

		if(iElementalDamage != 0)
		{
			GSProtocol.GCElementalDamageSend(lpCallObj->m_Index, lpTargetObj->m_Index, iElementalDamage, lpCallObj->m_iPentagramMainAttribute);
		}
		if(lpCallObj->Type == OBJ_MONSTER)
		{
			gObjAddMsgSend(lpCallObj,3,lpTargetObj->m_Index,0);
			CreateFrustrum(lpCallObj->X,lpCallObj->Y,lpCallObj->m_Index);
			lpTargetObj->KillerType = 1;
		}

		if (lpCallObj->Type == OBJ_USER && lpCallObj->m_PlayerData->ISBOT == false && lpObj->m_Index >= g_ConfigRead.server.GetObjectStartUserIndex())
		{
			gDarkSpirit[lpCallObj->m_Index- g_ConfigRead.server.GetObjectStartUserIndex()].ReSetTarget(lpTargetObj->m_Index);
			g_CMuunSystem.ReSetTarget(lpObj, lpTargetObj->m_Index);
		}

		if(lpCallObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER)
		{
			gObjSetKillCount(lpCallObj->m_Index,1);
		}

		if(IT_MAP_RANGE(lpCallObj->MapNumber))
		{
			if (g_IT_Event.GetIllusionTempleState(lpCallObj->MapNumber) == 2)
			{
				g_IT_Event.IncreaseKillCount(lpCallObj->m_Index, lpCallObj->MapNumber, 1);
				
				g_Log.Add("[ ITR ] [IncreaseKillCount] Map:%d, (%s)(%s) Kill User TargetInfo (%s)(%s)",
					lpCallObj->MapNumber - 44, lpCallObj->AccountID, lpCallObj->Name, lpTargetObj->AccountID, lpTargetObj->Name);
			}
		}

		if(lpTargetObj->Type == OBJ_MONSTER)
		{
			if (g_ConfigRead.server.GetServerType() == SERVER_CASTLE)
			{
				if (lpTargetObj->Class == 275 || lpTargetObj->Class == 295 || lpTargetObj->MapNumber == MAP_INDEX_CRYWOLF_FIRSTZONE 
					|| lpTargetObj->MapNumber == MAP_INDEX_HATCHERY || lpTargetObj->Class == 561 || lpTargetObj->Class == 43
					|| lpTargetObj->Class == 44 || lpTargetObj->Class == 53 || lpTargetObj->Class == 54 || lpTargetObj->Class == 55
					|| lpTargetObj->Class == 56 || lpTargetObj->Class == 78 || lpTargetObj->Class == 79 || lpTargetObj->Class == 80
					|| lpTargetObj->Class == 81 || lpTargetObj->Class == 82 || lpTargetObj->Class == 83 || lpTargetObj->Class == 493
					|| lpTargetObj->Class == 494 || lpTargetObj->Class == 495 || lpTargetObj->Class == 496 || lpTargetObj->Class == 497
					|| lpTargetObj->Class == 498 || lpTargetObj->Class == 499 || lpTargetObj->Class == 500 || lpTargetObj->Class == 501
					|| lpTargetObj->Class == 502)
				{
					gObjAddMsgSendDelayInSpecificQPos(lpTargetObj, 1, lpCallObj->m_Index, 800, 0, 0);
				}
				else
				{
					gObjAddMsgSendDelay(lpTargetObj, 1, lpCallObj->m_Index, 800, 0);
				}

				g_Crywolf.CrywolfMonsterDieProc(lpTargetObj->m_Index, lpCallObj->m_Index);
			}

			else
			{
				if (lpTargetObj->Class == 275 || lpTargetObj->Class == 295 || lpTargetObj->Class == 561 || lpTargetObj->Class == 43
					|| lpTargetObj->Class == 44 || lpTargetObj->Class == 53 || lpTargetObj->Class == 54 || lpTargetObj->Class == 55
					|| lpTargetObj->Class == 56 || lpTargetObj->Class == 78 || lpTargetObj->Class == 79 || lpTargetObj->Class == 80
					|| lpTargetObj->Class == 81 || lpTargetObj->Class == 82 || lpTargetObj->Class == 83 || lpTargetObj->Class == 493
					|| lpTargetObj->Class == 494 || lpTargetObj->Class == 495 || lpTargetObj->Class == 496 || lpTargetObj->Class == 497
					|| lpTargetObj->Class == 498 || lpTargetObj->Class == 499 || lpTargetObj->Class == 500 || lpTargetObj->Class == 501
					|| lpTargetObj->Class == 502 || lpTargetObj->Class == 627 || lpTargetObj->Class == 628 || lpTargetObj->Class == 629
					|| lpTargetObj->Class == 630 || lpTargetObj->Class == 631 || lpTargetObj->Class == 627 || lpTargetObj->Class == 628
					|| lpTargetObj->Class == 629 || lpTargetObj->Class == 630 || lpTargetObj->Class == 631 || lpTargetObj->Class == 459
					|| lpTargetObj->Class == 673) // Boss monsters, fixed for bug which boss sometimes doesn't drop item if he is killed by one hit
				{
					gObjAddMsgSendDelayInSpecificQPos(lpTargetObj, 1, lpCallObj->m_Index, 800, 0, 0);

					if (g_ConfigRead.data.common.BossKillMessage == true)
					{
						PMSG_NOTICE pNotice;
						char szMsg[256];
						LPMONSTER_ATTRIBUTE lpm = gMAttr.GetAttr(lpTargetObj->Class);

						int iMaxHitUser = gObjMonsterTopHitDamageUser(lpTargetObj);

						if (iMaxHitUser == -1)
						{
							iMaxHitUser = lpCallObj->m_Index;
						}

						wsprintf(szMsg, Lang.GetText(0,581), lpm->m_Name, gObj[iMaxHitUser].Name);
						TNotice::MakeNoticeMsg(&pNotice, 0, szMsg);
						TNotice::SendNoticeToAllUser(&pNotice);
					}
				}
				else
				{
					gObjAddMsgSendDelay(lpTargetObj, 1, lpCallObj->m_Index, 800, 0);
				}
			}

			if (g_ConfigRead.server.GetServerType() == SERVER_CASTLE)
			{
				if (lpTargetObj->m_Attribute == 62)
				{
					char szMsg[256];

					switch (lpTargetObj->Class)
					{
						case 300: // Hero Mobs
						case 301:
						case 302:
						case 303:
							{
								PMSG_NOTICE pNotice;

								ItemSerialCreateSend(lpCallObj->m_Index, lpCallObj->MapNumber, lpCallObj->X, lpCallObj->Y, ItemGetNumberMake(14, 13), 0, 0, 0, 0, 0, lpCallObj->m_Index, 0, 0, 0, 0, 0);

								if (gObjIsConnected(lpCallObj->m_Index))
								{
									wsprintf(szMsg, Lang.GetText(0,216), lpTargetObj->Name, lpCallObj->Name);
									g_Log.Add("[CastleDeep Event] [%s][%s] Boss Monster Killed : %s", lpCallObj->AccountID, lpCallObj->Name, lpTargetObj->Name);
								}
								else
								{
									wsprintf(szMsg, Lang.GetText(0,217), lpTargetObj->Name);
									g_Log.Add("[CastleDeep Event] Boss Monster Killed : %s", lpTargetObj->Name);
								}

								TNotice::MakeNoticeMsg(&pNotice, 0, szMsg);

								for (int i = g_ConfigRead.server.GetObjectStartUserIndex(); i < g_ConfigRead.server.GetObjectMax(); i++)
								{
									if (gObj[i].Connected == PLAYER_PLAYING && gObj[i].Type == OBJ_USER)
									{
										IOCP.DataSend(i, (LPBYTE)&pNotice, pNotice.h.size);
									}
								}
							}
							break;

						case 295: // Erohim
							{
								PMSG_NOTICE pNotice;

								if (gObjIsConnected(lpCallObj->m_Index))
								{
									wsprintf(szMsg, Lang.GetText(0,218), lpCallObj->Name);
									g_Log.Add("[CastleDeep Event] [%s][%s] Boss Monster Killed : %s", lpCallObj->AccountID, lpCallObj->Name, lpTargetObj->Name);
								}
								else
								{
									wsprintf(szMsg, Lang.GetText(0,219));
									g_Log.Add("[CastleDeep Event] Boss Monster Killed : %s", lpTargetObj->Name);
								}

								TNotice::MakeNoticeMsg(&pNotice, 0, szMsg);

								for (int i = g_ConfigRead.server.GetObjectStartUserIndex(); i < g_ConfigRead.server.GetObjectMax(); i++)
								{
									if (gObj[i].Connected == PLAYER_PLAYING && gObj[i].Type == OBJ_USER)
									{
										IOCP.DataSend(i, (LPBYTE)&pNotice, pNotice.h.size);
									}
								}
							}
							break;
					}
				}
			}

			else
			{
				g_Kanturu.KanturuMonsterDieProc(lpTargetObj->m_Index, lpCallObj->m_Index);
			}

			if ( lpTargetObj->Class >= 678 && lpTargetObj->Class <= 680 )
			{
				KillLordSilvesterRecallMon(lpTargetObj->m_RecallMon, lpTargetObj->m_Index);
			}
	
			if(BC_MAP_RANGE(lpTargetObj->MapNumber))
			{
				int iBridgeIndex = g_BloodCastle.GetBridgeIndex(lpTargetObj->MapNumber);

				if(lpTargetObj->Class == 131)
				{
					g_BloodCastle.DestroyCastleDoor(iBridgeIndex, lpTargetObj);
				}

				if(BC_STATUE_RANGE(lpTargetObj->Class - 132))
				{
					g_BloodCastle.DestroySaintStatue(iBridgeIndex, lpTargetObj);
				}
			}

			if (g_ArcaBattle.IsArcaBattleServer() == TRUE && lpTargetObj->Type >= OBJ_MONSTER && g_ArcaBattle.GetState() == 7)
			{
				if (lpObj->m_Attribute == 100)
				{
					LPOBJ lpCallObj = &gObj[lpObj->m_RecallMon];

					if (lpCallObj->Type != OBJ_USER)
					{
						g_Log.Add("[ArcaBattle] Error gObjLifeCheck()");
						return;
					}
				}

				g_ArcaBattle.IsOccupyObelisk(lpObj, lpTargetObj->Class);

				if (lpTargetObj->Class == 592 || lpTargetObj->Class == 593 || lpTargetObj->Class == 594 || lpTargetObj->Class == 595 || lpTargetObj->Class == 596
					|| lpTargetObj->Class == 611 || lpTargetObj->Class == 612 || lpTargetObj->Class == 613 || lpTargetObj->Class == 614 || lpTargetObj->Class == 615)
				{
					g_ArcaBattle.AddContributePoint(lpObj, lpTargetObj);
				}

				g_ArcaBattle.AddKillPoint(lpObj, lpTargetObj);
			}

			if(CC_MAP_RANGE(lpTargetObj->MapNumber))
			{
				if(rand()%100 < 50)
				{
					g_ChaosCastle.SearchNBlowObjs(lpTargetObj->MapNumber,lpTargetObj->X,lpTargetObj->Y);
				}

				g_ChaosCastle.DelMonsterList(g_ChaosCastle.GetChaosCastleIndex(lpTargetObj->MapNumber),lpTargetObj->m_Index);

				int TopHitUser = gObjMonsterTopHitDamageUser(lpTargetObj);

				if(TopHitUser != -1)
				{
					if(CC_MAP_RANGE(gObj[TopHitUser].MapNumber))
					{
						if(gObjIsConnected(TopHitUser))
						{
							gObj[TopHitUser].m_cKillMonsterCount += 1;

							int KillMonsterCount = gObj[TopHitUser].m_cKillMonsterCount;
							int CCIndex = g_ChaosCastle.GetChaosCastleIndex(gObj[TopHitUser].MapNumber);
							g_QuestExpProgMng.ChkUserQuestTypeEventMap(QUESTEXP_ASK_CHAOSCASTLE_MON_KILL, lpObj, CCIndex, KillMonsterCount);
						}
					}
				}
			}

			if (lpTargetObj->MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL)
			{
				if (rand() % 100 < 50)
				{
					g_ChaosCastleFinal.SearchNBlowObjs(lpTargetObj->MapNumber, lpTargetObj->X, lpTargetObj->Y);
				}

				g_ChaosCastleFinal.DelMonsterList(lpTargetObj->m_Index);

				int nMaxHitterIndex = gObjMonsterTopHitDamageUser(lpTargetObj);

				if (nMaxHitterIndex != -1 && gObj[nMaxHitterIndex].MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL && gObjIsConnected(nMaxHitterIndex))
					gObj[nMaxHitterIndex].m_byKillMobCount++;
			}

			if(DG_MAP_RANGE(lpTargetObj->MapNumber))
			{
				if ( lpTargetObj->Class == 531 )
				{
					g_Log.Add("[Doppelganger] Iceworker Die");
					g_DoppelGanger.DelIceWorkerIndex(lpTargetObj->m_Index);
				}

				if ( lpTargetObj->Class == 530 )
				{
					g_Log.Add("[Doppelganger] Killer Monster Die MiddleTreasureBox Create");
					g_DoppelGanger.SetKillerState(4);
					g_DoppelGanger.SetMiddleBossIndex(530, lpTargetObj->m_Index);
					g_DoppelGanger.AddMiddleTreasureBoxAll(lpTargetObj->X, lpTargetObj->Y);
				}

				if ( lpTargetObj->Class == 529 )
				{
					g_Log.Add("[Doppelganger] AngerKiller Monster Die MiddleTreasureBox Create");
					g_DoppelGanger.SetAngerKillerState(4);
					g_DoppelGanger.SetMiddleBossIndex(529, lpTargetObj->m_Index);
					g_DoppelGanger.AddMiddleTreasureBoxAll(lpTargetObj->X, lpTargetObj->Y);
				}
			}

			if (lpTargetObj->MapNumber == MAP_INDEX_DEVILSQUARE_FINAL && g_ConfigRead.server.GetServerType() == SERVER_BATTLECORE && lpObj->Type == OBJ_USER)
			{
				int nPoint = g_DevilSquareFinal.MonsterKillPoint(lpTargetObj->Class);
				g_DevilSquareFinal.MonsterKillPointCalc(lpObj->m_Index, nPoint);
				g_DevilSquareFinal.DSFDropReward(lpObj, lpTargetObj);

				g_Log.Add("[DSF][MonsterKill][%s][%s][%s][%d] MonsterType:%d, KillPoint:%d",
					lpObj->AccountID, lpObj->m_PlayerData->m_RealNameOfUBF,
					lpObj->Name, lpObj->m_PlayerData->m_nServerCodeOfHomeWorld,
					lpTargetObj->Class, nPoint);
			}

			gObjPkDownTimeCheck(lpCallObj,lpTargetObj->Level);

			if(lpCallObj->Type == OBJ_USER)
			{
				gObjAddMsgSendDelay(lpCallObj,3,lpTargetObj->m_Index,2000,0);
			}
		}
		else if(lpTargetObj->Type == OBJ_USER)
		{
			if(BC_MAP_RANGE(lpTargetObj->MapNumber))
			{
				g_BloodCastle.SetUserState(lpTargetObj->m_Index,1);
				g_BloodCastle.SearchUserDropQuestItem(lpTargetObj->m_Index);
			}

			if(IT_MAP_RANGE(lpTargetObj->MapNumber))
			{
				g_IT_Event.DropRelicsItem(lpTargetObj->MapNumber, lpTargetObj->m_Index);
			}

			if(CC_MAP_RANGE(lpTargetObj->MapNumber))
			{
				g_ChaosCastle.SetUserState(lpTargetObj->m_Index,1);

				if(CC_MAP_RANGE(lpCallObj->MapNumber))
				{
					if(gObjIsConnected(lpCallObj->m_Index))
					{
						lpCallObj->m_cKillUserCount += 1;

						int KillUserCount = lpObj->m_cKillUserCount;
						int CCIndex = g_ChaosCastle.GetChaosCastleIndex(lpObj->MapNumber);
						g_QuestExpProgMng.ChkUserQuestTypeEventMap(QUESTEXP_ASK_CHAOSCASTLE_USER_KILL, lpObj, CCIndex, KillUserCount);
					}
				}

				g_ChaosCastle.SendFailMessage(g_ChaosCastle.GetChaosCastleIndex(lpTargetObj->MapNumber),lpTargetObj->m_Index);

				g_Log.Add("[Chaos Castle] (%d) [%s][%s] User Dead In Chaos Castle : Killed by [%s][%s]",g_ChaosCastle.GetChaosCastleIndex(lpTargetObj->MapNumber)+1,lpTargetObj->AccountID,lpTargetObj->Name,lpCallObj->AccountID,lpCallObj->Name);
			}

			if (lpTargetObj->MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL)
			{
				g_ChaosCastleFinal.SetUserState(lpTargetObj->m_Index, 1);

				if (lpCallObj->MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL && gObjIsConnected(lpObj) == TRUE)
				{
					lpCallObj->m_byKillUserCount++;
				}

				g_ChaosCastleFinal.SendFailMessage(lpTargetObj->m_Index);

				g_Log.Add("[Chaos Castle Survival] [%s][%s] User Dead In Chaos Castle Survival : Killed by [%s][%s]", lpTargetObj->AccountID, lpTargetObj->Name, lpCallObj->AccountID, lpCallObj->Name);
				g_Log.Add("[Chaos Castle Survival] [gObjLifeCheck] NAME :%s, Index :%d ", lpTargetObj->Name, lpTargetObj->m_Index);
			}

			lpTargetObj->KillerType = 0;

			gObjUserDie(lpTargetObj,lpCallObj);

			int iGuildWar = gObjGuildWarCheck(lpCallObj,lpTargetObj);
			int iDuel = gObjDuelCheck(lpCallObj,lpTargetObj);

			if(iGuildWar == 0 && iDuel == 0)
			{
				if(lpTargetObj->m_bAttackerKilled == 0)
				{
					gObjPlayerKiller(lpCallObj,lpTargetObj,MSBDamage);
				}
			}

			lpTargetObj->m_bAttackerKilled = 0;

			if(lpTargetObj->m_RecallMon >= 0)
			{
				gObjMonsterCallKill(lpTargetObj->m_Index);
			}

			g_QuestExpProgMng.ChkUserQuestTypeEventMap(QUESTEXP_ASK_CONTRIBUTION_POINT, lpObj, lpObj->MapNumber, 0);
		}

		lpTargetObj->Live = 0;
		lpTargetObj->m_State = 4;
		lpTargetObj->RegenTime = GetTickCount();
		lpTargetObj->DieRegen = 1;
		lpTargetObj->PathCount = 0;

		if(gObjTargetGuildWarCheck(lpTargetObj,lpCallObj)==1)
		{
			lpTargetObj->KillerType = 2;
		}

		if(gObjDuelCheck(lpCallObj,lpTargetObj) == TRUE)
		{
			lpTargetObj->KillerType = 3;
			g_NewPVP.CheckScore(*lpCallObj, *lpTargetObj);
		}

		if(lpTargetObj->Teleport == 1)
		{
			lpTargetObj->Teleport = 0;
		}

		GSProtocol.GCDiePlayerSend(lpTargetObj,lpTargetObj->m_Index,Skill,lpCallObj->m_Index);

		if ( lpTargetObj->Class >= 524 && lpTargetObj->Class <= 528 && lpTargetObj->Class != 526 )
		{
			g_ImperialGuardian.DestroyGate(lpTargetObj->m_nZoneIndex, lpTargetObj->m_Index, lpObj->m_Index);
		}

		if ( lpObj->Class == 523 )
		{
			g_Log.Add("[IMPERIALGUARDIAN] Player Killed by Trap [AccountID]:%s, [NAME]:%s, [ZONE]:%d",
				lpTargetObj->AccountID, lpTargetObj->Name, lpObj->m_nZoneIndex+1);
		}

		if ( lpObj->Class >= 504 && lpObj->Class <= 521 )
		{
			g_Log.Add("[IMPERIALGUARDIAN] Player Dead [Zone]:%d [AccountID]:%s, [NAME]:%s by Monster => %s(%d)",
				lpObj->m_nZoneIndex+1, lpTargetObj->AccountID, lpTargetObj->Name, lpObj->Name, lpObj->m_Index);
		}

		if ( lpTargetObj->Class >= 504 && lpTargetObj->Class <= 521 )
		{	
			g_Log.Add("[IMPERIALGUARDIAN] Monster Dead [TYPE]: %d,[NAME]: %s, [ZONE]:%d",
				lpTargetObj->Class, lpTargetObj->Name, lpTargetObj->m_nZoneIndex+1);			
		}

		if ( (lpTargetObj->Class >= 504 && lpTargetObj->Class <= 511) || lpTargetObj->Class == 526 )
		{
			g_ImperialGuardian.DropItem(lpObj->m_Index, lpTargetObj->m_Index);
		}

		if (lpTargetObj->MapNumber == MAP_INDEX_HATCHERY) //season4 add-on
		{
			if (lpTargetObj->Type >= OBJ_MONSTER)
			{
				if (lpTargetObj->Class == 460 ||
					lpTargetObj->Class == 461 ||
					lpTargetObj->Class == 462)
				{
					g_Raklion.BossEggDieDecrease();
					g_RaklionUtil.NotifyRaklionUserMonsterCount(g_Raklion.GetBossEggDieCount(), 0);
				}
			}
		}

		if(lpTargetObj->Class == 79)
		{
			gEledoradoEvent.m_BossGoldDerconMapNumber[lpTargetObj->m_BossGoldDerconMapNumber] = -1;
			gEledoradoEvent.CheckGoldDercon(lpCallObj->MapNumber);
		}

		if(lpTargetObj->m_RecallMon >= 0)
		{
			return;
		}

		if(lpTargetObj->Class == 340 || lpTargetObj->Class == 348 || lpTargetObj->Class == 349)
		{
			return;
		}

		if(lpTargetObj->Class == 287 || lpTargetObj->Class == 286)
		{
			return;
		}

		if(lpTargetObj->Class == 275)
		{
			return;
		}

		if (g_ConfigRead.server.GetServerType() == SERVER_CASTLE)
		{
			if (lpTargetObj->Class == 288)
			{
				return;
			}
		}

		if(lpTargetObj->Class == 295)
		{
			return;
		}

		if (g_ConfigRead.server.GetServerType() != SERVER_CASTLE)
		{
			if (lpTargetObj->Class == 362 || lpTargetObj->Class == 363 || lpTargetObj->Class == 361)
			{
				return;
			}
		}

		if(lpTargetObj->Class == 412) //Season 2.5 Dark Elf Hero
		{
			return;
		}

		if(lpTargetObj->Class >= 524 && lpTargetObj->Class <= 528 && lpTargetObj->Class != 526)
		{
			return;
		}

		if (lpTargetObj->MapNumber == MAP_INDEX_DEVILSQUARE_FINAL)
		{
			return;
		}

		if(lpCallObj->Type == OBJ_USER)
		{
			if (lpTargetObj->m_btExpType == 1)
			{
				gObjMonsterExpDivisionRenewal(lpTargetObj, lpCallObj, AttackDamage, MSBFlag);
			}

			if(lpCallObj->PartyNumber >= 0)
			{
				gObjExpParty(lpCallObj,lpTargetObj,AttackDamage,MSBFlag);
			}
			else if(lpTargetObj->Type == OBJ_MONSTER)
			{
				gObjMonsterExpDivision(lpTargetObj,lpCallObj,AttackDamage,MSBFlag);
			}

			if(lpTargetObj->Type == OBJ_MONSTER && DS_MAP_RANGE(lpTargetObj->MapNumber))
			{
				g_DevilSquare.gObjMonsterScoreDivision(lpTargetObj,lpCallObj,AttackDamage,MSBFlag);
			}
		}

		if(lpTargetObj->Type == OBJ_MONSTER)
		{
			if(lpTargetObj->m_bIsInMonsterHerd != 0 && lpTargetObj->m_lpMonsterHerd != 0)
			{
				lpTargetObj->m_lpMonsterHerd->MonsterDieAction(lpTargetObj);
			}
		}
	}
	else
	{
		if ( lpTargetObj->Class == 673 )
		{
			if ( !lpTargetObj->m_nPhaseLordSilvester && lpTargetObj->MaxLife * 0.7 >= lpTargetObj->Life )
				gObjMonsterSummonSkillForLordSilvester(lpTargetObj);
			if ( lpTargetObj->m_nPhaseLordSilvester == 1 && lpTargetObj->MaxLife * 0.5 >= lpTargetObj->Life )
				gObjMonsterSummonSkillForLordSilvester(lpTargetObj);
			if ( lpTargetObj->m_nPhaseLordSilvester == 2 && lpTargetObj->MaxLife * 0.2 >= lpTargetObj->Life )
				gObjMonsterSummonSkillForLordSilvester(lpTargetObj);
		}

		if(iElementalDamage == TRUE)
		{
			GSProtocol.GCElementalDamageSend(lpCallObj->m_Index,lpTargetObj->m_Index,AttackDamage,MSBDamage);
		}
		
		else if(DamageSendType == 0)
		{
			GSProtocol.GCDamageSend(lpCallObj->m_Index,lpTargetObj->m_Index,AttackDamage,MSBFlag,MSBDamage,iShieldDamage);
		}
		else if(DamageSendType == 1)
		{
			GSProtocol.GCDamageSendPoison(lpTargetObj->m_Index,AttackDamage,iShieldDamage);
		}
		else if(DamageSendType == 2)
		{
			GSProtocol.GCDamageSend(lpCallObj->m_Index,lpTargetObj->m_Index,AttackDamage,MSBFlag,5,iShieldDamage);
		}
		else if(DamageSendType == 3)
		{
			GSProtocol.GCDamageSend(lpCallObj->m_Index,lpTargetObj->m_Index,AttackDamage,MSBFlag,6,iShieldDamage);
		}
	}

	if(lpTargetObj->Type == OBJ_MONSTER)
	{
		GSProtocol.GCSendObjectHP(lpCallObj->m_Index, lpTargetObj->m_Index);
	}

	if (lpTargetObj->Type == OBJ_MONSTER)
	{
		for (int i = 0; i < MAX_VIEWPORT; i++)
		{
			if (lpTargetObj->VpPlayer[i].type == OBJ_USER)
			{
				GSProtocol.GCObjectLifeInfo(lpTargetObj->VpPlayer[i].number, lpTargetObj->m_Index, lpTargetObj->MaxLife + lpTargetObj->AddLife, lpTargetObj->Life);
			}
		}
	}
}



BOOL gObjInventoryTrans(int aIndex)
{
	if ( ((aIndex< 0)?FALSE:(aIndex > g_ConfigRead.server.GetObjectMax()-1)?FALSE:TRUE) == FALSE )
	{
		g_Log.Add("error : %s %d", __FILE__, __LINE__);
		return FALSE;
	}

	if ( gObj[aIndex].pTransaction == 1 && gObj[aIndex].m_PlayerData->ISBOT == FALSE )
	{
		g_Log.Add("[%s][%s] error-L3 : pTransaction(%d) status error %s %d",
			gObj[aIndex].AccountID, gObj[aIndex].Name,
			gObj[aIndex].pTransaction, __FILE__, __LINE__);
		return FALSE;
	}

	for ( int n=0;n<MAX_MAGIC;n++)
	{
		gObj[aIndex].m_lpMagicBack[n] = gObj[aIndex].Magic[n]; 
	}

	for (int n=0;n<INVENTORY_SIZE;n++)
	{
		gObj[aIndex].Inventory2[n] = gObj[aIndex].Inventory1[n];
	}
	
	for (int n = 0; n < EVENT_INVENTORY_SIZE; n++)
	{
		gObj[aIndex].pEventInventory2[n] = gObj[aIndex].pEventInventory1[n];
	}

	memcpy(gObj[aIndex].InventoryMap2, gObj[aIndex].InventoryMap1, INVENTORY_MAP_SIZE); 
	memcpy(gObj[aIndex].pEventInventoryMap2, gObj[aIndex].pEventInventoryMap1, EVENT_INVENTORY_MAP_SIZE);

	gObj[aIndex].InventoryCount2 = gObj[aIndex].InventoryCount1;
	gObjSetInventory2Pointer(&gObj[aIndex]);
	gObjSetEventInventory2Pointer(&gObj[aIndex]);
	gObj[aIndex].pTransaction = 1;
	
	g_Log.Add("Trade Transaction (%s)", gObj[aIndex].Name);

	return TRUE;
}

// CMagicInf::operator= 4c4d90




int gObjInventoryCommit(int aIndex)
{

	if(!ObjectMaxRange(aIndex))
	{
		g_Log.Add("error : gObjInventoryCommit() - aIndex out of bound %s %d",__FILE__,__LINE__);
		return false;
	}

	if(gObj[aIndex].pTransaction != 1)
	{
		g_Log.Add("[%s][%s] error-L3 : pTransaction(%d) status2",gObj[aIndex].AccountID,gObj[aIndex].Name,gObj[aIndex].pTransaction);
		return false;
	}

	for(int n = 0; n < INVENTORY_SIZE; n++)
	{
		gObj[aIndex].Inventory1[n] = gObj[aIndex].Inventory2[n];
	}

	memcpy(gObj[aIndex].InventoryMap1,gObj[aIndex].InventoryMap2,INVENTORY_MAP_SIZE);

	gObj[aIndex].InventoryCount1 = gObj[aIndex].InventoryCount2;

	gObjSetInventory1Pointer(&gObj[aIndex]);

	for (int n = 0; n < EVENT_INVENTORY_SIZE; n++)
	{
		gObj[aIndex].pEventInventory1[n] = gObj[aIndex].pEventInventory2[n];
	}

	memcpy(gObj[aIndex].pEventInventoryMap1, gObj[aIndex].pEventInventoryMap2, EVENT_INVENTORY_MAP_SIZE);

	gObjSetEventInventory1Pointer(&gObj[aIndex]);

	gObj[aIndex].pTransaction = 2;

	g_Log.Add("Trade Commit (%s)",gObj[aIndex].Name);

	return true;
}




int gObjInventoryRollback(int aIndex)
{
	if(!ObjectMaxRange(aIndex))
	{
		g_Log.Add("error : %s %d",__FILE__,__LINE__);
		return false;
	}

	if(gObj[aIndex].pTransaction != 1)
	{
		g_Log.Add("[%s][%s] error-L3 : pTransaction(%d) status2",gObj[aIndex].AccountID,gObj[aIndex].Name,gObj[aIndex].pTransaction);
		return false;
	}

	for(int n = 0; n < MAX_MAGIC; n++)
	{
		gObj[aIndex].Magic[n] = gObj[aIndex].m_lpMagicBack[n];
	}

	for(int n = 0; n < INVENTORY_SIZE; n++)
	{
		gObj[aIndex].Inventory2[n].Clear();
	}

	for (int n = 0; n < EVENT_INVENTORY_SIZE; n++)
	{
		gObj[aIndex].pEventInventory2[n].Clear();
	}

	gObjSetInventory1Pointer(&gObj[aIndex]);
	gObjSetEventInventory1Pointer(&gObj[aIndex]);

	gObj[aIndex].pTransaction = 3;

	g_Log.Add("Trade Rollback (%s)",gObj[aIndex].Name);

	return true;
}




void gObjInventoryItemSet(int aIndex, int itempos, BYTE set_byte)
{
	int width;
	int height;
	if(itempos < INVETORY_WEAR_SIZE || itempos == 236)
	{
		return;
	}
	else if(itempos > (INVENTORY_SIZE-1))
	{
		return;
	}

	if(gObj[aIndex].pInventory[itempos].GetSize((int&)width,(int &)height)==0)
	{
		g_Log.Add("error: Item does not exist %s %d",__FILE__,__LINE__);
		return;
	}

	gObjInventoryItemBoxSet(aIndex,itempos,width,height,set_byte);
}


void gObjInventoryItemBoxSet(int aIndex, int itempos, int xl, int yl, BYTE set_byte)
{
	int itemposx = (itempos - INVETORY_WEAR_SIZE) % 8;
	int itemposy = (itempos - INVETORY_WEAR_SIZE) / 8;

	int xx, yy;

	if (!CheckOutOfInventory(aIndex, itemposy, yl))	//OK
	{
		return;
	}

	for (int y = 0; y < yl; y++)
	{
		yy = itemposy + y;

		for (int x = 0; x < xl; x++)
		{
			xx = itemposx + x;

			if (InventoryExtentCheck(xx, yy, 8, MAX_INVENTORY_H2))
			{
				*(BYTE*)(gObj[aIndex].pInventoryMap + (itemposy + y) * 8 + (itemposx + x)) = set_byte;
			}

			else
			{
				return;
			}
		}
	}
}

bool gObjFixInventoryPointer(int aIndex)
{

	if(gObjIsConnected(aIndex)==0)
	{
		g_Log.Add("[Fix Inv.Ptr] [%s][%s] - disconnected",gObj[aIndex].AccountID,gObj[aIndex].Name);
		return false;
	}

	if(gObj[aIndex].pInventory == gObj[aIndex].Inventory1)
	{
		return true;
	}

	if(gObj[aIndex].pInventory == gObj[aIndex].Inventory2)
	{
		if(gObj[aIndex].pTransaction == 1)
		{
			g_Log.Add("[Fix Inv.Ptr] [%s][%s] - Transaction == 1, Do not change Pointer",gObj[aIndex].AccountID,gObj[aIndex].Name);
			return false;
		}
		else
		{
			g_Log.Add("[Fix Inv.Ptr] [%s][%s] - Inventory Pointer was 2",gObj[aIndex].AccountID,gObj[aIndex].Name);

			for(int n = 0; n < INVENTORY_SIZE; n++)
			{
				gObj[aIndex].Inventory2[n].Clear();
			}
		}
	}
	else
	{
		g_Log.Add("[Fix Inv.Ptr] [%s][%s] - Inventory Pointer was Wrong",gObj[aIndex].AccountID,gObj[aIndex].Name);
	}

	gObjSetInventory1Pointer(&gObj[aIndex]);

	return false;
}

BYTE gObjInventoryDeleteItem(int aIndex ,int itempos)
{
	if(itempos < INVETORY_WEAR_SIZE || itempos == 236)
	{
		int MagicDel = gObjMagicDel(&gObj[aIndex],gObj[aIndex].pInventory[itempos].m_Special[0],gObj[aIndex].pInventory[itempos].m_Level);

		if(MagicDel >= 0)
		{
			GSProtocol.GCMagicListOneDelSend(aIndex,MagicDel,
				gObj[aIndex].pInventory[itempos].m_Special[0],
				gObj[aIndex].pInventory[itempos].m_Level,0,0);
		}

		gObj[aIndex].pInventory[itempos].Clear();
	}
	else
	{
		gObjInventoryItemSet(aIndex,itempos,255);
		gObj[aIndex].pInventory[itempos].Clear();
	}

	return true;
}

BYTE gObjWarehouseDeleteItem(int aIndex, int itempos)
{
	gObjWarehouseItemSet(aIndex, itempos, -1);
	gObj[aIndex].pWarehouse[itempos].Clear();

	return true;
}

BYTE gObjChaosBoxDeleteItem(int aIndex, int itempos)
{
	gObjChaosItemSet(aIndex, itempos, -1);
	gObj[aIndex].pChaosBox[itempos].Clear();

	return true;
}

BYTE gObjInventoryInsertItem(int aIndex, int item_num)
{
	int w,h,map_num,iwidth,iheight;
	BYTE blank;

	blank = 0;
	map_num = gObj[aIndex].MapNumber;

	if(MapC[map_num].m_cItem[item_num].live == 0)
	{
		return -1;
	}

	if(MapC[map_num].m_cItem[item_num].GetSize((int &)iwidth,(int &)iheight) == 0)
	{
		g_Log.Add("error: Item does not exist %s %d",__FILE__,__LINE__);
		return -1;
	}

	for(h = 0; h < 8; h++)
	{
		for(w = 0; w < 8; w++)
		{
			if(*(BYTE*)(gObj[aIndex].pInventoryMap+h*8+w) == 255)
			{
				blank = gObjOnlyInventoryRectCheck(aIndex,w,h,iwidth,iheight);

				if(blank == 254)
				{
					goto GOTO_EndFunc;
				}

				if(blank != 255)
				{
					gObj[aIndex].pInventory[blank] = MapC[map_num].m_cItem[item_num];
					gObjInventoryItemSet(aIndex,blank,gObj[aIndex].pInventory[blank].m_Type);
					return blank;
				}
			}
		}
	}
GOTO_EndFunc:
	return -1;

}

DWORD gGetItemNumber()	
{
	return gItemNumberCount;
}

void gPlusItemNumber() 
{
	gItemNumberCount++;
}

BYTE gObjInventoryInsertItem(int aIndex, CItem item)
{
	int w,h,iwidth,iheight;
	BYTE blank = 0;

	if(item.GetSize((int&)iwidth,(int&)iheight)==0)
	{
		g_Log.Add("error: Item does not exist %s %d",__FILE__,__LINE__);
		return -1;
	}

	for(h = 0; h < MAX_INVENTORY_H; h++)
	{
		for( w = 0; w < 8; w++)
		{
			if(*(BYTE*)(gObj[aIndex].pInventoryMap+h*8+w) == 255)
			{
				blank = gObjInventoryRectCheck(aIndex,w,h,iwidth,iheight);

				if(blank == 254)
				{
					goto GOTO_EndFunc;
				}

				if(blank != 255)
				{
					if(gObjCheckSerial0ItemList(&item)!=0)
					{
						MsgOutput(aIndex,Lang.GetText(0,259));
						return -1;
					}

					if(gObjInventorySearchSerialNumber(&gObj[aIndex],item.m_Number) == 0)
					{
						return -1;
					}

					gObj[aIndex].pInventory[blank] = item;

					gObjInventoryItemSet(aIndex,blank,gObj[aIndex].pInventory[blank].m_Type);
					return blank;
				}
			}
		}
	}

GOTO_EndFunc:
	return -1;
}

BYTE gObjOnlyInventoryInsertItem(int aIndex, CItem item)
{
	int w, h, iwidth, iheight;
	BYTE blank = 0;

	if (item.GetSize((int&)iwidth, (int&)iheight) == 0)
	{
		g_Log.Add("error: Item does not exist %s %d", __FILE__, __LINE__);
		return -1;
	}

	for (h = 0; h < MAX_INVENTORY_H; h++)	//OK
	{
		for (w = 0; w < 8; w++)
		{
			if (*(BYTE*)(gObj[aIndex].pInventoryMap + h * 8 + w) == 255)
			{
				blank = gObjOnlyInventoryRectCheck(aIndex, w, h, iwidth, iheight);

				if (blank == 254)
				{
					goto GOTO_EndFunc;
				}

				if (blank != 255)
				{
					if (gObjCheckSerial0ItemList(&item) != 0)
					{
						MsgOutput(aIndex, Lang.GetText(0,259));
						return -1;
					}

					if (gObjInventorySearchSerialNumber(&gObj[aIndex], item.m_Number) == 0)
					{
						return -1;
					}

					gObj[aIndex].pInventory[blank] = item;

					gObjInventoryItemSet(aIndex, blank, gObj[aIndex].pInventory[blank].m_Type);
					return blank;
				}
			}
		}
	}

GOTO_EndFunc:
	return -1;
}

BYTE gObjInventoryInsertItem(int aIndex, CMapItem * item)
{
	int w, h, iwidth, iheight;
	BYTE blank = 0;

	CItem copyitem;

	if (item->GetSize((int &)iwidth, (int &)iheight) == 0)
	{
		return -1;
	}

	for (h = 0; h < MAX_INVENTORY_H; h++)
	{
		for (w = 0; w < 8; w++)
		{
			if (*(BYTE*)(gObj[aIndex].pInventoryMap + h * 8 + w) == 255)
			{
				blank = gObjOnlyInventoryRectCheck(aIndex, w, h, iwidth, iheight);
				if (blank == 254)
				{
					goto GOTO_EndFunc;
				}

				if (blank != 255)
				{
					copyitem.m_Level = item->m_Level;
					copyitem.m_Durability = item->m_Durability;

					copyitem.Convert(item->m_Type, item->m_Option1, item->m_Option2, item->m_Option3, item->m_NewOption, item->m_SetOption, item->m_ItemOptionEx, item->m_SocketOption, item->m_BonusSocketOption, 0, CURRENT_DB_VERSION);
					copyitem.SetPetItemInfo(item->m_PetItem_Level, item->m_PetItem_Exp);

					copyitem.m_Number = item->m_Number;

					gObj[aIndex].pInventory[blank] = copyitem;

					gObjInventoryItemSet(aIndex, blank, gObj[aIndex].pInventory[blank].m_Type);
					return blank;
				}
			}
		}
	}

GOTO_EndFunc:
	return -1;
}




BOOL gObjSearchItem(LPOBJ lpObj, int item_type, int add_dur, int nLevel)
{
	for(int n = INVETORY_WEAR_SIZE; n < MAIN_INVENTORY_SIZE; n++)
	{
		if(lpObj->pInventory[n].IsItem() == 1)
		{
			if(lpObj->pInventory[n].m_Type == item_type)
			{
				if(lpObj->pInventory[n].m_Level == nLevel)
				{
					if((int)lpObj->pInventory[n].m_Durability + add_dur <= 3)
					{
						lpObj->pInventory[n].m_Durability += add_dur;
						GSProtocol.GCItemDurSend(lpObj->m_Index,n,lpObj->pInventory[n].m_Durability,0);
						return true;
					}
				}
			}
		}
	}
	return false;
}




BOOL gObjSearchItemMinus(LPOBJ lpObj, int pos, int m_dur)
{
	int n = pos;
	if(n < 0 || n > (MAIN_INVENTORY_SIZE-1))
	{
		g_Log.Add("error-L4 : %s %d",__FILE__,__LINE__);
		return 1;
	}

	if(lpObj->pInventory[n].IsItem() == 1)
	{
		if((int)lpObj->pInventory[n].m_Durability - m_dur > 0)
		{
			lpObj->pInventory[n].m_Durability -= m_dur;
			GSProtocol.GCItemDurSend(lpObj->m_Index,n,lpObj->pInventory[n].m_Durability,1);
			return 1;
		}
	}
	return 0;
}



BYTE gObjShopBuyInventoryInsertItem(int aIndex, CItem item)
{
	int w, h, iwidth, iheight;
	BYTE blank = 0;

	if (item.GetSize(iwidth, iheight) == 0)
	{
		g_Log.Add("error: Item does not exist %s %d", __FILE__, __LINE__);
		return -1;
	}

	for (h = 0; h < MAX_INVENTORY_H; h++)	//24
	{
		for (w = 0; w < 8; w++)
		{
			if (*(BYTE*)(gObj[aIndex].pInventoryMap + h * 8 + w) == 255)
			{
				blank = gObjOnlyInventoryRectCheck(aIndex, w, h, iwidth, iheight);

				if (blank == 254)
				{
					goto GOTO_EndFunc;
				}

				if (blank != 255)
				{
					gObj[aIndex].pInventory[blank] = item;
					gObj[aIndex].pInventory[blank].m_Number = 0;

					gObjInventoryItemSet(aIndex, blank, gObj[aIndex].pInventory[blank].m_Type);
					return blank;
				}
			}
		}
	}
GOTO_EndFunc:
	return -1;
}

BYTE gObjShopBuyMuunInventoryInsertItem(int aIndex, CItem item)
{
	int w, h, iwidth, iheight;
	BYTE blank = 0;

	if (item.GetSize(iwidth, iheight) == 0)
	{
		g_Log.Add("error: Item does not exist %s %d", __FILE__, __LINE__);
		return -1;
	}

	return gObjMuunInventoryInsertItem(aIndex, item);
}

BYTE gObjShopBuyEventInventoryInsertItem(int aIndex, CItem item)
{
	int w, h, iwidth, iheight;
	BYTE blank = 0;

	if (item.GetSize(iwidth, iheight) == 0)
	{
		g_Log.Add("error: Item does not exist %s %d", __FILE__, __LINE__);
		return -1;
	}

	for (h = 0; h < 4; h++)	//24
	{
		for (w = 0; w < 8; w++)
		{
			if (*(BYTE*)(gObj[aIndex].pEventInventoryMap + h * 8 + w) == 255)
			{
				blank = gObjEventInventoryRectCheck(aIndex, w, h, iwidth, iheight);

				if (blank == 254)
				{
					goto GOTO_EndFunc;
				}

				if (blank != 255)
				{
					gObj[aIndex].pEventInventory[blank] = item;
					gObj[aIndex].pEventInventory[blank].m_Number = 0;

					gObjEventInventoryItemSet(aIndex, blank, gObj[aIndex].pEventInventory[blank].m_Type);
					return blank;
				}
			}
		}
	}
GOTO_EndFunc:
	return -1;
}

BYTE gObjInventoryInsertItemTemp(LPOBJ lpObj, CMapItem * Item)
{
	CItem item;
	int w, h, iwidth, iheight;
	BYTE blank = 0;

	if (Item->GetSize((int &)iwidth, (int &)iheight) == 0)
	{
		g_Log.Add("error: Item does not exist %s %d", __FILE__, __LINE__);
		return -1;
	}

	for (h = 0; h < MAX_INVENTORY_H; h++)	//24
	{
		for (w = 0; w < 8; w++)
		{
			if (*(BYTE*)(lpObj->pInventoryMap + h * 8 + w) == 255)
			{
				blank = gObjInventoryRectCheck(lpObj->m_Index, w, h, iwidth, iheight);

				if (blank == 254)
				{
					goto GOTO_EndFunc;
				}

				if (blank != 255)
				{
					return blank;
				}
			}
		}
	}
GOTO_EndFunc:
	return -1;
}

BYTE gObjInventoryRectCheck(int aIndex, int sx, int sy, int width, int height)
{
	int x, y;
	int blank = 0;

	if (sx + width > 8)
	{
		return -1;
	}

	if (sy + height > MAX_INVENTORY_H2)	//28
	{
		return -2;
	}
	// ----
	bool bResult = CheckOutOfInventory(aIndex, sy, height);
	// ----
	if (!bResult)
	{
		if (bResult == -1)
		{
			return -2;
		}
		// ----
		return -1;
	}
	// ----
	int xx, yy;

	for (y = 0; y < height; y++)
	{

		yy = sy + y;

		for (x = 0; x < width; x++)
		{
			xx = sx + x;

			if (InventoryExtentCheck(xx, yy, 8, MAX_INVENTORY_H2))
			{
				if (*(BYTE*)(gObj[aIndex].pInventoryMap + (sy + y) * 8 + (sx + x)) != 255)
				{
					blank += 1;
					return -1;
				}
			}
			else
			{
				g_Log.Add("error : %s %d", __FILE__, __LINE__);
				return -1;
			}
		}
	}

	if (blank == 0)
	{
		return sx + sy * 8 + 12;	//strange
	}
	return  -1;
}




BYTE gObjOnlyInventoryRectCheck(int aIndex, int sx, int sy, int width, int height)
{
	int x, y;
	int blank = 0;

	if (sx + width > 8)
	{
		return -1;
	}

	if (sy + height > 4 * gObj[aIndex].m_PlayerData->m_InventoryExpansion + 8)
	{
		return -2;
	}

	int nInventoyHeightofCurUser = CheckOutOfInventory(aIndex, sy, height);
	if (!nInventoyHeightofCurUser)
	{
		if (nInventoyHeightofCurUser == -1)
		{
			return -2;
		}
		return -1;
	}

	int xx, yy;

	for (y = 0; y < height; y++)
	{

		yy = sy + y;

		for (x = 0; x < width; x++)
		{
			xx = sx + x;

			if (InventoryExtentCheck(xx, yy, 8, MAX_INVENTORY_H))	//24
			{
				if (*(BYTE*)(gObj[aIndex].pInventoryMap + (sy + y) * 8 + (sx + x)) != 255)
				{
					blank += 1;
					return -1;
				}
			}
			else
			{
				g_Log.Add("error : %s %d", __FILE__, __LINE__);
				return -1;
			}
		}
	}

	if (blank == 0)
	{
		return sx + sy * 8 + 12;	//strange again
	}
	return  -1;
}

BOOL CheckInventoryEmptySpace(LPOBJ lpObj, int iItemHeight, int iItemWidth)
{
	int h = 0;
	int w = 0;
	BYTE blank = 0;

	for (h = 0; h<MAX_INVENTORY_H; h++)	//24
	{
		for (w = 0; w<8; w++)
		{
			if (*(BYTE*)(lpObj->pInventoryMap + (h)* 8 + (w)) == 255)
			{
				blank = gObjOnlyInventoryRectCheck(lpObj->m_Index, w, h, iItemWidth, iItemHeight);

				if (blank == 0xFE)
					return FALSE;

				if (blank != 0xFF)
					return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL gObjIsItemPut(LPOBJ lpObj, CItem * lpItem, int pos ) // check this
{
	if(lpItem->m_TwoHand == 1)
	{
		if(pos == 0 || pos == 1)
		{
			if(lpObj->pInventory[0].IsItem() == 1)
			{
				if(lpObj->pInventory[0].m_Type == ITEMGET(4,7) || lpObj->pInventory[0].m_Type == ITEMGET(4,15))
				{
					
				}
				else
				{
					return false;
				}
			}

			if(lpObj->pInventory[1].IsItem() == 1)
			{
				if(lpObj->pInventory[1].m_Type == ITEMGET(4,7) || lpObj->pInventory[1].m_Type == ITEMGET(4,15))
				{
					
				}
				else
				{
					return false;
				}
			}
		}
	}

	if(pos == 1)
	{
		if(lpObj->pInventory[0].IsItem() == 1)
		{
			if(lpItem->m_Type == ITEMGET(4,7) || lpItem->m_Type == ITEMGET(4,15))
			{
					
			}
			else
			{
				if(lpObj->pInventory[0].m_TwoHand)
				{
					return false;
				}
			}
		}
	}

	if(lpItem->m_Part != pos)
	{
		if(pos == 1 || pos == 0)
		{
			if(lpItem->m_Part == 1 || lpItem->m_Part == 0)
			{

			}
			else
			{
				return false;
			}
		}
		else if(pos == 11)
		{
			if(lpItem->m_Part == 10)
			{

			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}

	if((lpObj->m_PlayerData->Strength + lpObj->AddStrength) < (lpItem->m_RequireStrength - lpItem->m_HJOpStrength))
	{
		return false;
	}

	if((lpObj->m_PlayerData->Dexterity + lpObj->AddDexterity) < (lpItem->m_RequireDexterity - lpItem->m_HJOpDexterity))
	{
		return false;
	}

	if (lpItem->IsPeriodItemExpire() == TRUE)
	{
		return false;
	}

	if(lpObj->Level+lpObj->m_PlayerData->MasterLevel < lpItem->m_RequireLevel)
	{
		return false;
	}

	if(lpItem->m_Type >= 0 && lpItem->m_Type < ITEMGET(12,0))
	{
		if((lpObj->m_PlayerData->Vitality + lpObj->AddVitality) < lpItem->m_RequireVitality)
		{
			return false;
		}

		if((lpObj->m_PlayerData->Energy + lpObj->AddEnergy) < lpItem->m_RequireEnergy)
		{
			return false;
		}
	}

	if((lpObj->Leadership + lpObj->AddLeadership) < lpItem->m_RequireLeaderShip)
	{
		return false;
	}

	if (GetWingType(lpItem->m_Type, WCF_ITEMNUMBER) != NONE_WING)
	{
		if (lpObj->Level < lpItem->m_RequireLevel)
		{
			return false;
		}
	}

	else if(lpItem->m_Type >= ITEMGET(13,0) && lpItem->m_Type <= ITEMGET(13,15))
	{
		if(lpItem->m_Type == ITEMGET(13,10))
		{
			if(lpItem->m_Level <= 2)
			{
				if(lpObj->Level < 20)
				{
					return false;
				}
			}
			else
			{
				if(lpObj->Level < 50)
				{
					return false;
				}
			}
		}
		else if(lpItem->m_Type == ITEMGET(13,2))
		{
			if(lpObj->MapNumber == 7 || lpObj->Level < lpItem->m_RequireLevel)
			{
				return false;
			}
		}
		else if(lpItem->m_Type == ITEMGET(13,3))
		{
			if(lpObj->MapNumber == 7 || lpObj->Level < lpItem->m_RequireLevel)
			{
				return false;
			}
		}
		else
		{
			if(lpObj->Level < lpItem->m_RequireLevel)
			{
				return false;
			}
		}
	}

	if(lpItem->m_Type == ITEMGET(13,20) && lpItem->m_Level == 0)
	{
		int count = gObjGetItemCountInEquipment(lpObj->m_Index,13,20,0);

		if(count != 0)
		{
			return false;
		}
	}

	if(lpItem->m_Type == ITEMGET(13,107) && lpItem->m_Level == 0)
	{
		int count = gObjGetItemCountInEquipment(lpObj->m_Index,13,107,0);

		if(count != 0)
		{
			return false;
		}
	}

	if(lpItem->m_Type == ITEMGET(13,10))
	{
		if (CC_MAP_RANGE(lpObj->MapNumber) || lpObj->MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL)
		{
			return false;
		}
	}

	if(lpItem->m_Type == ITEMGET(13,37))
	{
		if (CC_MAP_RANGE(lpObj->MapNumber) || lpObj->MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL)
		{
			return false;
		}
	}

	if(lpItem->m_Type == ITEMGET(13,37))
	{
		if (CC_MAP_RANGE(lpObj->MapNumber) || lpObj->MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL)
		{
			return false;
		}
	}

	if(lpItem->m_Type == ITEMGET(13,76) || lpItem->m_Type == ITEMGET(13,77) || lpItem->m_Type == ITEMGET(13,78))
	{
		if (CC_MAP_RANGE(lpObj->MapNumber) || lpObj->MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL)
		{
			return false;
		}
	}

	if (IT_MAP_RANGE(lpObj->MapNumber))
	{
		if (g_IT_Event.CheckChangeRing(lpItem->m_Type))
		{
			return false;
		}
	}

	if(lpItem->m_Type == ITEMGET(13,122))
	{
		if(CC_MAP_RANGE(lpObj->MapNumber) || lpObj->MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL)
		{
			return false;
		}
	}

	if(lpItem->m_Type == ITEMGET(13,38))
	{
		int count = gObjGetItemCountInEquipment(lpObj->m_Index,13,38,0);

		if(count != 0)
		{
			return false;
		}
	}

	if(lpItem->m_Type == ITEMGET(13,39))
	{
		if (CC_MAP_RANGE(lpObj->MapNumber) || lpObj->MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL)
		{
			return false;
		}

		int count = 0;

		for(int n = 0; n < 7; n++)
		{
			count = gObjGetItemCountInEquipment(lpObj->m_Index,13,10,n);

			if(count != 0)
			{
				return false;
			}
		}

		count = gObjGetItemCountInEquipment(lpObj->m_Index,13,39,0);

		if(count != 0)
		{
			return false;
		}

		count = gObjGetItemCountInEquipment(lpObj->m_Index,13,40,0);

		if(count != 0)
		{
			return false;
		}

		count = gObjGetItemCountInEquipment(lpObj->m_Index,13,41,0);

		if(count != 0)
		{
			return false;
		}

		count = gObjGetItemCountInEquipment(lpObj->m_Index,13,42,0);

		if(count != 0)
		{
			return false;
		}
	}

	if(lpItem->m_Type == ITEMGET(13,40))
	{
		if (CC_MAP_RANGE(lpObj->MapNumber) || lpObj->MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL)
		{
			return false;
		}

		int count = 0;

		for(int n = 0; n < 7; n++)
		{
			count = gObjGetItemCountInEquipment(lpObj->m_Index,13,10,n);

			if(count != 0)
			{
				return false;
			}
		}

		count = gObjGetItemCountInEquipment(lpObj->m_Index,13,39,0);

		if(count != 0)
		{
			return false;
		}

		count = gObjGetItemCountInEquipment(lpObj->m_Index,13,40,0);

		if(count != 0)
		{
			return false;
		}

		count = gObjGetItemCountInEquipment(lpObj->m_Index,13,41,0);

		if(count != 0)
		{
			return false;
		}

		count = gObjGetItemCountInEquipment(lpObj->m_Index,13,42,0);

		if(count != 0)
		{
			return false;
		}
	}

	if(lpItem->m_Type == ITEMGET(13,130))
	{
		int count = gObjGetItemCountInEquipment(lpObj->m_Index, 13, 132, 0);

		if(count != 0)
		{
			return false;
		}
	}

	if(lpItem->m_Type == ITEMGET(13,131))
	{
		int count = gObjGetItemCountInEquipment(lpObj->m_Index, 13, 133, 0);

		if(count != 0)
		{
			return false;
		}
	}

	if(lpItem->m_Type == ITEMGET(13,132))
	{
		int count = gObjGetItemCountInEquipment(lpObj->m_Index, 13, 130, 0);

		if(count != 0)
		{
			return false;
		}
	}

	if(lpItem->m_Type == ITEMGET(13,133))
	{
		int count = gObjGetItemCountInEquipment(lpObj->m_Index, 13, 131, 0);

		if(count != 0)
		{
			return false;
		}
	}

	if(lpItem->m_Type == ITEMGET(13,128))
	{
		int count = gObjGetItemCountInEquipment(lpObj->m_Index, 13, 129, 0);

		if(count != 0)
		{
			return false;
		}
	}

	if(lpItem->m_Type == ITEMGET(13,129))
	{
		int count = gObjGetItemCountInEquipment(lpObj->m_Index, 13, 128, 0);

		if(count != 0)
		{
			return false;
		}
	}

	if(lpItem->m_Type == ITEMGET(13,41))
	{
		if (CC_MAP_RANGE(lpObj->MapNumber) || lpObj->MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL)
		{
			return false;
		}

		int count = 0;

		for(int n = 0; n < 7; n++)
		{
			count = gObjGetItemCountInEquipment(lpObj->m_Index,13,10,n);

			if(count != 0)
			{
				return false;
			}
		}

		count = gObjGetItemCountInEquipment(lpObj->m_Index,13,39,0);

		if(count != 0)
		{
			return false;
		}

		count = gObjGetItemCountInEquipment(lpObj->m_Index,13,40,0);

		if(count != 0)
		{
			return false;
		}

		count = gObjGetItemCountInEquipment(lpObj->m_Index,13,41,0);

		if(count != 0)
		{
			return false;
		}

		count = gObjGetItemCountInEquipment(lpObj->m_Index,13,42,0);

		if(count != 0)
		{
			return false;
		}
	}

	if(lpItem->m_Type == ITEMGET(13,42))
	{
		if (CC_MAP_RANGE(lpObj->MapNumber) || lpObj->MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL)
		{
			return false;
		}

		int count = 0;

		for(int n = 0; n < 7; n++)
		{
			count = gObjGetItemCountInEquipment(lpObj->m_Index,13,10,n);

			if(count != 0)
			{
				return false;
			}
		}

		count = gObjGetItemCountInEquipment(lpObj->m_Index,13,39,0);

		if(count != 0)
		{
			return false;
		}

		count = gObjGetItemCountInEquipment(lpObj->m_Index,13,40,0);

		if(count != 0)
		{
			return false;
		}

		count = gObjGetItemCountInEquipment(lpObj->m_Index,13,41,0);

		if(count != 0)
		{
			return false;
		}

		count = gObjGetItemCountInEquipment(lpObj->m_Index,13,42,0);

		if(count != 0)
		{
			return false;
		}
	}

	if(lpItem->m_Type == ITEMGET(13,10))
	{
		int count = 0;

		for(int n = 0; n < 7; n ++)
		{
			count = gObjGetItemCountInEquipment(lpObj->m_Index,13,10,n);

			if(count != 0)
			{
				return false;
			}
		}

		count = gObjGetItemCountInEquipment(lpObj->m_Index,13,39,0);
		
		if(count != 0)
		{
			return false;
		}

		count = gObjGetItemCountInEquipment(lpObj->m_Index,13,40,0);
		
		if(count != 0)
		{
			return false;
		}

		count = gObjGetItemCountInEquipment(lpObj->m_Index,13,41,0);
		
		if(count != 0)
		{
			return false;
		}

		count = gObjGetItemCountInEquipment(lpObj->m_Index,13,42,0);
		
		if(count != 0)
		{
			return false;
		}
	}

	if(lpItem->m_Type == ITEMGET(13,169) || lpItem->m_Type == ITEMGET(13,170))
	{
		if (CC_MAP_RANGE(lpObj->MapNumber) || lpObj->MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL)
		{
			return false;
		}

		if (gObjGetItemCountInEquipment(lpObj->m_Index, 13, 169, 0) > 0 ||
			gObjGetItemCountInEquipment(lpObj->m_Index, 13, 170, 0) > 0)
		{
			return false;
		}
	}

	if (g_PentagramSystem.IsPentagramItem(lpItem->m_Type))
	{
		BYTE btMainAttribute = lpItem->m_BonusSocketOption & 0x0F;

		if (btMainAttribute != 1 &&
			btMainAttribute != 2 &&
			btMainAttribute != 3 &&
			btMainAttribute != 4 &&
			btMainAttribute != 5)
		{
			return false;
		}
	}

	else if(lpItem->m_Type >= ITEMGET(14,00))
	{
		return false;
	}

	return true;
}




BYTE gObjWerehouseRectCheck(int aIndex, int sx, int sy, int width, int height)
{
	int x, y;
	int blank = 0;

	if (sx + width > 8)
	{
		return -1;
	}

	if (sx + width < 0)
	{
		return -1;
	}

	if (!CheckOutOfWarehouse(aIndex, sy, height))
	{
		return -1;
	}

	int xx, yy;

	LPOBJ lpObj = &gObj[aIndex];

	for (y = 0; y < height; y++)
	{

		yy = sy + y;

		for (x = 0; x < width; x++)
		{
			xx = sx + x;

			if (WarehouseExtentCheck(xx, yy, 8, MAX_WAREHOUSE_H))
			{
				if (*(BYTE*)(gObj[aIndex].pWarehouseMap + (sy + y) * 8 + (sx + x)) != 255)
				{
					blank += 1;
					return -1;
				}
			}
			else
			{
				g_Log.Add("error : %s %d", __FILE__, __LINE__);
				return -1;
			}
		}
	}

	if (blank == 0)
	{
		return sx + sy * 8;
	}
	return  -1;
}




BYTE gObjMapRectCheck(BYTE * lpMapBuf, int sx, int sy, int ex, int ey, int width, int height)
{
	int x,y;
	int blank = 0;

	if(sx + width > ex || sx + width < 0)
	{
		return -1;
	}

	if(sy + height > ey || sy + height < 0)
	{
		return -1;
	}

	int xx,yy;

	for(y = 0; y < height; y++)
	{
		yy = sy + y;

		for(x = 0; x < width; x++)
		{
			xx = sx + x;

			if(ExtentCheck(xx,yy,ex,ey)==1)
			{
				if(*(BYTE*)(lpMapBuf + (sy + y)*ex + (sx+x)) != 255)
				{
					blank += 1;
					return -1;
				}
			}
			else
			{
				g_Log.Add("error : %s %d",__FILE__,__LINE__);
				return -1;
			}
		}
	}

	if(blank == 0)
	{
		return sy*ex+sx;
	}
	return  -1;

	return -1;
}




void gObjWarehouseItemBoxSet(int aIndex, int itempos, int xl, int yl, BYTE set_byte)
{
	int itemposx = (itempos) % 8;
	int itemposy = (itempos) / 8;

	int xx, yy;

	if (!CheckOutOfWarehouse(aIndex, itemposy, yl))
	{
		return;
	}

	for (int y = 0; y < yl; y++)
	{
		yy = itemposy + y;

		for (int x = 0; x < xl; x++)
		{
			xx = itemposx + x;

			if (WarehouseExtentCheck(xx, yy, 8, MAX_WAREHOUSE_H))
			{
				*(BYTE*)(gObj[aIndex].pWarehouseMap + (itemposy + y) * 8 + (itemposx + x)) = set_byte;
			}
			else
			{
				g_Log.Add("error : %d/%d  %s %d", xx, yy, __FILE__, __LINE__);
				return;
			}
		}
	}
}




void gObjWarehouseItemSet(int aIndex, int itempos, BYTE set_byte)
{
	int width;
	int height;
	if(itempos < 0)
	{
		return;
	}
	else if(itempos > (WAREHOUSE_SIZE-1))
	{
		return;
	}

	if(gObj[aIndex].pWarehouse[itempos].GetSize((int&)width,(int &)height)==0)
	{
		g_Log.Add("error: Item does not exist %s %d",__FILE__,__LINE__);
		return;
	}

	gObjWarehouseItemBoxSet(aIndex,itempos,width,height,set_byte);
}




void gObjChaosItemBoxSet(int aIndex, int itempos, int xl, int yl, BYTE set_byte)
{
	int itemposx = (itempos)%8;
	int itemposy = (itempos)/8;

	int xx,yy;

	for(int y = 0; y < yl; y ++)
	{
		yy = itemposy + y;

		for(int x = 0; x < xl; x++)
		{
			xx = itemposx + x;

			if(ExtentCheck(xx,yy,8,4)==1)
			{
				*(BYTE*)(gObj[aIndex].pChaosBoxMap + (itemposy + y)*8+(itemposx + x)) = set_byte;
			}
			else
			{
				g_Log.Add("error : %d/%d  %s %d",xx,yy,__FILE__,__LINE__);
				return;
			}
		}
	}
}




void gObjChaosItemSet(int aIndex, int itempos, BYTE set_byte)
{
	int width;
	int height;

	if(itempos < 0)
	{
		return;
	}

	else if(itempos > (CHAOS_BOX_SIZE-1))
	{
		return;
	}

	if(gObj[aIndex].pChaosBox[itempos].GetSize((int&)width,(int &)height)==0)
	{
		g_Log.Add("error: Item does not exist %s %d",__FILE__,__LINE__);
		return;
	}

	gObjChaosItemBoxSet(aIndex,itempos,width,height,set_byte);
}




BYTE gObjChaosBoxInsertItemPos(int aIndex, CItem item, int pos, int source)
{

	LPOBJ lpObj;
	BYTE TempMap[CHAOS_BOX_MAP_SIZE];

	int w,h,iwidth,iheight;

	int blank,useClass;

	if(pos < 0 || pos > (CHAOS_BOX_SIZE-1))
	{
		return -1;
	}

	useClass = 0;

	lpObj = &gObj[aIndex];

	if(lpObj->pChaosBox[pos].IsItem() == 1)
	{
		return -1;
	}

	if(item.IsItem() == 0)
	{
		return -1;
	}

	w = pos%8;
	h = pos/8;

	if(ExtentCheck(w,h,8,4) == 0)
	{
		return -1;
	}

	item.GetSize((int &)iwidth,(int &)iheight);

	if(source >= 0)
	{
		memcpy(TempMap,lpObj->pChaosBoxMap,CHAOS_BOX_MAP_SIZE);
		gObjChaosItemBoxSet(lpObj->m_Index,source,iwidth,iheight,255);
	}

	if(*(BYTE*)(lpObj->pChaosBoxMap + h * 8 + w) != 255)
	{
		if(source >= 0)
		{
			memcpy(lpObj->pChaosBoxMap,TempMap,CHAOS_BOX_MAP_SIZE);
		}
		return -1;
	}

	blank = gObjMapRectCheck(lpObj->pChaosBoxMap,w,h,8,4,iwidth,iheight);

	if(blank == 255)
	{
		if(source >= 0)
		{
			memcpy(lpObj->pChaosBoxMap,TempMap,CHAOS_BOX_MAP_SIZE);
		}
		return -1;
	}

	lpObj->pChaosBox[pos] = item;
	return pos;
}




BYTE gObjWarehouseInsertItemPos(int aIndex, CItem item, int pos, int source)
{
	LPOBJ lpObj;
	BYTE TempMap[WAREHOUSE_SIZE];

	int w, h, iwidth, iheight;

	int blank, useClass;

	if (pos < 0 || pos >(WAREHOUSE_SIZE - 1))	//OK
	{
		return -1;
	}

	useClass = 0;

	lpObj = &gObj[aIndex];

	if (lpObj->pWarehouse[pos].IsItem() == 1)
	{
		return -1;
	}

	if (item.IsItem() == 0)
	{
		return -1;
	}

	w = pos % 8;
	h = pos / 8;

	if (!WarehouseExtentCheck(w, h, 8, MAX_WAREHOUSE_H))
	{
		return -1;
	}

	item.GetSize((int &)iwidth, (int &)iheight);

	if (source >= 0)
	{
		memcpy(TempMap, lpObj->pWarehouseMap, WAREHOUSE_SIZE);
		gObjWarehouseItemBoxSet(lpObj->m_Index, source, iwidth, iheight, 255);
	}

	if (*(BYTE*)(lpObj->pWarehouseMap + h * 8 + w) != 255)
	{
		if (source >= 0)
		{
			memcpy(lpObj->pWarehouseMap, TempMap, WAREHOUSE_SIZE);
		}
		return -1;
	}

	blank = gObjWerehouseRectCheck(lpObj->m_Index, w, h, iwidth, iheight);

	if (blank == 255)
	{
		if (source >= 0)
		{
			memcpy(lpObj->pWarehouseMap, TempMap, WAREHOUSE_SIZE);
		}
		return -1;
	}

	lpObj->pWarehouse[pos] = item;
	return pos;
}




BYTE gObjInventoryInsertItemPos(int aIndex, CItem item, int pos, BOOL RequestCheck)
{
	if (pos < 0 || pos > INVENTORY_SIZE)	//236 from 6.2
	{
		return -1;
	}

	LPOBJ lpObj;
	BYTE TempInventoryMap[INVENTORY_MAP_SIZE];

	int w, h, iwidth, iheight;

	int blank, useClass = 0;

	lpObj = &gObj[aIndex];

	if (lpObj->pInventory[pos].IsItem() == 1)
	{
		return -1;
	}

	if (item.IsItem() == 0)
	{
		return -1;
	}

	if (pos < 12 || pos == 236)
	{
		useClass = item.IsClass(lpObj->Class, lpObj->m_PlayerData->ChangeUP);

		if (useClass == 0)
		{
			g_Log.Add("error-L2: It is not his class");
			return -1;
		}

		if (RequestCheck != 0)
		{
			if (gObjIsItemPut(lpObj, &item, pos) == 0)
			{
				return -1;
			}
		}
	}
	else
	{
		w = (pos - 12) % 8;
		h = (pos - 12) / 8;

		item.GetSize((int &)iwidth, (int &)iheight);

		bool bResult = CheckOutOfInventory(aIndex, h, iheight);
		// ----
		if (!bResult)
		{
			if (bResult == -1)
			{
				return -2;
			}
			// ----
			return -1;
		}
		// ----
		if (!InventoryExtentCheck(w, h, 8, MAX_INVENTORY_H2))	//OK
		{
			return -1;
		}

		memcpy(TempInventoryMap, lpObj->pInventoryMap, INVENTORY_MAP_SIZE);	//OK

		if (*(BYTE*)(lpObj->pInventoryMap + h * 8 + w) != 255)
		{
			memcpy(lpObj->pInventoryMap, TempInventoryMap, INVENTORY_MAP_SIZE);	//OK
			return -1;
		}

		blank = gObjInventoryRectCheck(lpObj->m_Index, w, h, iwidth, iheight);

		if (blank >= 254)
		{
			memcpy(lpObj->pInventoryMap, TempInventoryMap, INVENTORY_MAP_SIZE);	//OK
			return -1;
		}
	}

	lpObj->pInventory[pos] = item;

	if (pos > 11)
	{
		gObjInventoryItemSet(aIndex, pos, 1);
	}

	if (pos < 12 || pos == 236)
	{
		CItem * lpItem = &lpObj->pInventory[pos];

		if (lpItem->m_Option1)
		{
			int result = gObjWeaponMagicAdd(lpObj, lpItem->m_Special[0], lpItem->m_Level);

			if (result >= 0)
			{
				GSProtocol.GCMagicListOneSend(lpObj->m_Index, result, lpItem->m_Special[0], lpItem->m_Level, 0, 0);
			}
		}
	}
	return pos;
}

BOOL gObjInventorySearchSerialNumber(LPOBJ lpObj, UINT64 serial)
{
	UINT64 s_num;
	int count = 0;

	if(gItemSerialCheck == 0)
	{
		return true;
	}

	if(serial == 0 || serial == (UINT64)-1)
	{
		return true;
	}

	for(int n = 0; n < INVENTORY_SIZE; n++)
	{
		s_num = lpObj->pInventory[n].GetNumber();

		if (s_num != 0 && s_num == serial && s_num != (UINT64)-1)
		{
			count++;
		}
	}

	if(count <= 1)
	{
		return true;
	}

	for(int n = 0; n < INVENTORY_SIZE; n++)
	{
		s_num = lpObj->pInventory[n].GetNumber();

		if (s_num != 0 && s_num == serial && s_num != (UINT64)-1)
		{
			g_Log.Add("error-L1: CopyItem Id[%s] Char[%s] Item[%s] InventoryPos[%d] serial[%I64d]",lpObj->AccountID,lpObj->Name,lpObj->pInventory[n].GetName(),n,s_num);
			GSProtocol.GCServerMsgStringSend(Lang.GetText(0,15),lpObj->m_Index,1);
			gObjUserKill(lpObj->m_Index);
		}
	}

	return false;
}




BOOL gObjWarehouseSearchSerialNumber(LPOBJ lpObj, UINT64 sirial)
{
	UINT64 s_num;
	int count = 0;

	if(gItemSerialCheck == 0)
	{
		return true;
	}

	if (sirial == 0 || sirial == (UINT64)-1)
	{
		return true;
	}

	for(int n = 0; n < WAREHOUSE_SIZE; n++)
	{
		s_num = lpObj->pWarehouse[n].GetNumber();

		if (s_num != 0 && s_num == sirial && s_num != (UINT64)-1)
		{
			count++;
		}
	}

	if(count <= 1)
	{
		return true;
	}

	for(int n = 0; n < WAREHOUSE_SIZE; n++)
	{
		s_num = lpObj->pWarehouse[n].GetNumber();

		if (s_num != 0 && s_num == sirial && s_num != (UINT64)-1)
		{
			g_Log.Add("error-L1: CopyItem Id[%s] Char[%s] Item[%s] warehousepos[%d] serial[%I64d]",lpObj->AccountID,lpObj->Name,lpObj->pWarehouse[n].GetName(),n,s_num);
			GSProtocol.GCServerMsgStringSend(Lang.GetText(0,15),lpObj->m_Index,1);
			gObjUserKill(lpObj->m_Index);
		}
	}

	return false;
}




BYTE gObjInventoryMoveItem(int aIndex, unsigned char source, unsigned char target, int& durSsend, int& durTsend, unsigned char sFlag, unsigned char tFlag, unsigned char* siteminfo)
{
	LPOBJ lpObj;
	BYTE TempInventoryMap[225];
	int size = 225;
	int w,h;
	int iwidth,iheight;
	int blank;
	UINT64 s_num;

	CItem * sitem;
	CItem * titem;

	int bPersonalShopTrans = 0;
	int bSourceIsPShop = 0;

	durSsend = 0;
	durTsend = 0;

	lpObj = &gObj[aIndex];

	if ( lpObj->m_bMapSvrMoveReq == true || lpObj->m_bMapSvrMoveQuit == true || lpObj->m_State == 32 )
	{
		g_Log.AddC(TColor::Red, "[gObjInventoryMoveItem] Can't move item in inventory - MapServerMove");
		return -1;
	}

	int useClass = 0;
	__try
	{
		if(sFlag == 2 || tFlag == 2)
		{
			if(lpObj->m_IfState.type != 6)
			{
				g_Log.Add("He is not using warehouse [%s][%s]",lpObj->AccountID,lpObj->Name);
				return -1;
			}

			if(lpObj->m_IfState.state == 0)
			{
				g_Log.Add("He is not using warehouse [%s][%s]",lpObj->AccountID,lpObj->Name);
				return -1;
			}
		}

		switch(sFlag)
		{
		case 0:
			if((source < 0 || source > (MAIN_INVENTORY_SIZE-1)) && source != 236)
			{
				g_Log.Add("error-L1 : %s %d",__FILE__,__LINE__);
				return -1;
			}

			if(lpObj->pInventory[source].IsItem() == 0)
			{
				g_Log.Add("Item does not exist [%s][%s] %d",lpObj->AccountID,lpObj->Name,__LINE__);
				return -1;
			}

			sitem = &lpObj->pInventory[source];

			if(lpObj->MapNumber == MAP_INDEX_ICARUS)
			{
				if(source == 8)
				{
					if(sitem->m_Type == ITEMGET(13,3))
					{
						if(lpObj->pInventory[7].IsItem() == 0)
						{
							return -1;
						}
					}
					else if(sitem->m_Type == ITEMGET(13,37))
					{
						if(lpObj->pInventory[7].IsItem() == 0)
						{
							return -1;
						}
					}

					else if (sitem->m_Type == ITEMGET(13, 4)) //season4 add-on
					{
						if (lpObj->pInventory[7].IsItem() == 0)
						{
							return -1;
						}
					}
				}
				else if (source == 7) //OK
				{
					if (lpObj->pInventory[lpObj->m_btInvenPetPos].m_Type != ITEMGET(13, 3) &&
						//season4 add-on
						lpObj->pInventory[lpObj->m_btInvenPetPos].m_Type != ITEMGET(13, 37) &&
						lpObj->pInventory[lpObj->m_btInvenPetPos].m_Type != ITEMGET(13, 4))
					{
						return -1;
					}
				}
			}

			break;
		case 2:
			if(source < 0 || source > (WAREHOUSE_SIZE-1))
			{
				g_Log.Add("error-L1 : %s %d",__FILE__,__LINE__);
				return -1;
			}

			if(lpObj->pWarehouse[source].IsItem() == 0)
			{
				g_Log.Add("Item does not exist [%s][%s] %d",lpObj->AccountID,lpObj->Name,__LINE__);
				return -1;
			}

			sitem = &lpObj->pWarehouse[source];
			break;
		case 3:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		case 15:
		case 16:
		case 19:
		case 20:
			if(source < 0 || source > (CHAOS_BOX_SIZE -1))
			{
				g_Log.Add("error-L1 : %s %d",__FILE__,__LINE__);
				return -1;
			}

			if(lpObj->pChaosBox[source].IsItem() == 0)
			{
				g_Log.Add("Item does not exist [%s][%s] %d",lpObj->AccountID,lpObj->Name,__LINE__);
				return -1;
			}

			sitem = &lpObj->pChaosBox[source];
			break;
		case 4:
			if(gObj[aIndex].m_bPShopOpen == 1)
			{
				return -1;
			}

			EnterCriticalSection(&gObj[aIndex].m_critPShopTrade);

			if(gObj[aIndex].m_bPShopTransaction == 1)
			{
				g_Log.Add("[PShop] [%s][%s] PShop Item Move Request Failed : Already Trade With Other",gObj[aIndex].AccountID,gObj[aIndex].Name);
				LeaveCriticalSection(&lpObj->m_critPShopTrade);
				return -1;
			}

			lpObj->m_bPShopTransaction = 1;
			bPersonalShopTrans = 1;
			bSourceIsPShop = 1;

			LeaveCriticalSection(&lpObj->m_critPShopTrade);

			if(source < MAIN_INVENTORY_SIZE || source > (INVENTORY_SIZE - 1))
			{
				g_Log.Add("error-L1 : %s %d",__FILE__,__LINE__);
				return -1;
			}

			if(lpObj->pInventory[source].IsItem() == 0)
			{
				g_Log.Add("Item does not exist [%s][%s] %d",lpObj->AccountID,lpObj->Name,__LINE__);
				return -1;
			}

			sitem = &lpObj->pInventory[source];

			break;
		case 17:
			if (source < 0 || source >(CHAOS_BOX_SIZE - 1))
			{
				g_Log.Add("error-L1 : %s %d", __FILE__, __LINE__);
				return -1;
			}

			if (lpObj->m_PlayerData->pPentagramMixBox[source].IsItem() == 0)
			{
				g_Log.Add("Item does not exist [%s][%s] %d", lpObj->AccountID, lpObj->Name, __LINE__);
				return -1;
			}

			sitem = &lpObj->m_PlayerData->pPentagramMixBox[source];
			break;
		default: return -1;
		}

		BYTE level;
		BYTE op1;
		BYTE op2;
		BYTE op3;
		BYTE dur;
		int type;

		BufferItemtoConvert3(siteminfo,type,level,op1,op2,op3,dur);

		if(sitem->m_Type != type || sitem->m_Level != level || sitem->m_Option1 != op1 || sitem->m_Option2 != op2 || sitem->m_Option3 != op3)
		{
			g_Log.AddC(TColor::Red, "Item information does not match. %x %x %x %x %x/ %x %x %x %x %x",type,level,op1,op2,op3,sitem->m_Type,sitem->m_Level,sitem->m_Option1,sitem->m_Option2,sitem->m_Option3);
			return -1;
		}

		switch(tFlag)
		{
		case 0:
			if((target < 0 || target > (MAIN_INVENTORY_SIZE - 1)) && target != 236)
			{
				g_Log.Add("error-L1: %s %d",__FILE__,__LINE__);
				return -1;
			}

			titem = &lpObj->pInventory[target];
			break;
		case 2:
			if(target < 0 || target > (WAREHOUSE_SIZE - 1))
			{
				g_Log.Add("error-L1 : %s %d",__FILE__,__LINE__);
				return -1;
			}

			if(sitem->IsPeriodItem() == TRUE)
			{
				return -1;
			}

			titem = &lpObj->pWarehouse[target];
			break;
		case 3:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		case 15:
		case 16:
		case 19:
		case 20:
			if(target < 0 || target > (CHAOS_BOX_SIZE -1))
			{
				g_Log.Add("error-L1 : %s %d",__FILE__,__LINE__);
				return -1;
			}

			titem = &lpObj->pChaosBox[target];

			if(sFlag == 0)
			{
				if(tFlag == 5)
				{
					if(sitem->m_Type == ITEMGET(12,15)	||
						sitem->m_Type == ITEMGET(14,13) ||
						sitem->m_Type == ITEMGET(14,14) ||
						sitem->m_Type == ITEMGET(14,16) ||
						sitem->m_Type == ITEMGET(14,22) ||
						sitem->m_Type == ITEMGET(13,31) ||
						sitem->m_Type == ITEMGET(14,53) ||
						//Season4.5 add-on
						sitem->m_Type == ITEMGET(13, 83) ||
						sitem->m_Type == ITEMGET(13, 84) ||
						sitem->m_Type == ITEMGET(13, 85) ||
						sitem->m_Type == ITEMGET(13, 86) ||
						sitem->m_Type == ITEMGET(13, 87) ||
						sitem->m_Type == ITEMGET(13, 88) ||
						sitem->m_Type == ITEMGET(13, 89) ||
						sitem->m_Type == ITEMGET(13, 90) ||
						sitem->m_Type == ITEMGET(13, 91) ||
						sitem->m_Type == ITEMGET(13, 92) ||
						//6.2
						sitem->m_Type == ITEMGET(14, 103) ||
						sitem->m_Type == ITEMGET(14, 104) ||
						sitem->m_Type == ITEMGET(14, 105) ||
						sitem->m_Type == ITEMGET(14, 106) ||
						sitem->m_Type == ITEMGET(14, 107) ||
						sitem->m_Type == ITEMGET(14, 108) ||
						sitem->m_Type == ITEMGET(14, 121) ||
						sitem->m_Type == ITEMGET(14, 122) ||
						sitem->m_Type == ITEMGET(14, 112) ||
						sitem->m_Type == ITEMGET(14, 113))
					{

					}
					else
					{
						return -1;
					}
				}
				else if(tFlag == 6)
				{
					if(g_kJewelOfHarmonySystem.IsJewelOfHarmonyOriginal(sitem->m_Type) == 0)
					{
						return -1;
					}
				}
				else if(tFlag == 7)
				{
					if(g_kJewelOfHarmonySystem.IsEnableToMakeSmeltingStoneItem(sitem) == 0)
					{
						GSProtocol.GCServerMsgStringSend(Lang.GetText(0,278),lpObj->m_Index,1);
						return -1;
					}

					if(IsJumpingEventItem(sitem->m_Type) == TRUE)
					{
						GSProtocol.GCServerMsgStringSend(Lang.GetText(0,278),lpObj->m_Index,1);
						return -1;
					}
				}
				else if(tFlag == 8)
				{
					if(g_kJewelOfHarmonySystem.IsStrengthenByJewelOfHarmony(sitem) == 0)
					{
						return -1;
					}
				}
				else if(tFlag == 9)
				{
					if(sitem->m_Type != ITEMGET(14,54) && sitem->m_Type != ITEMGET(14,92) && sitem->m_Type != ITEMGET(14,93) && sitem->m_Type != ITEMGET(14,95))
					{
						return -1;
					}
				}
				else if(tFlag == 10)
				{
					if(sitem->m_Type != ITEMGET(14,88) && sitem->m_Type != ITEMGET(14,89) && sitem->m_Type != ITEMGET(14,90))
					{
						return -1;
					}
				}
				else if (tFlag == 11)
				{

				}
				else if (tFlag == 12)
				{

				}
				else if (tFlag == 13)
				{

				}
				else if (tFlag == 14)
				{

				}
				else if(tFlag == 15)
				{
					for (int i=0;i<CHAOS_BOX_SIZE;i++)
					{
						if(gObj[aIndex].pChaosBox[i].IsItem())
						{
							return -1;
						}
					}
					if(sitem->m_Type == ITEMGET(13,137) || sitem->m_Type == ITEMGET(13,142))
					{
						if(lpObj->Class == CLASS_MAGUMSA)
						{
							MsgOutput(aIndex, Lang.GetText(0,608));
							return -1;
						}
					}
					if(sitem->m_Type == ITEMGET(13,143) || sitem->m_Type == ITEMGET(13,138))
					{
						if(lpObj->Class == CLASS_RAGEFIGHTER)
						{
							MsgOutput(aIndex, Lang.GetText(0,609));
							return -1;
						}
					}
				}
				else if(tFlag == 16)
				{
					for (int i=0;i<CHAOS_BOX_SIZE;i++)
					{
						if(gObj[aIndex].pChaosBox[i].IsItem())
						{
							return -1;
						}
					}
				}

				else if (tFlag == 19)
				{

				}

				else if(tFlag == 20)
				{
					if(sitem->m_Type != ITEMGET(14,21) || sitem->m_Level != 3)
					{
						return -1;
					}
				}

				else
				{
					if(sitem->m_Level < 4 && sitem->m_Option3*4 < 4)
					{
						if(sitem->m_Type == ITEMGET(12,15) || sitem->m_Type == ITEMGET(14,13) || sitem->m_Type == ITEMGET(14,14) || sitem->m_Type == ITEMGET(14,16) || sitem->m_Type == ITEMGET(14,22) || sitem->m_Type == ITEMGET(13,14))
						{
							
						}
						else if(sitem->m_Type >= ITEMGET(12,0) && sitem->m_Type <= ITEMGET(12,2))
						{

						}
						else if(sitem->m_Type >= ITEMGET(12,36) && sitem->m_Type <= ITEMGET(12,43))
						{

						}
						else if(sitem->m_Type >= ITEMGET(12,262) && sitem->m_Type <= ITEMGET(12,265))
						{

						}
						else if(sitem->m_Type == ITEMGET(12,49) || sitem->m_Type <= ITEMGET(12,50))
						{

						}
						else if(sitem->m_Type == ITEMGET(14,17) || sitem->m_Type == ITEMGET(14,18))
						{

						}
						else if(sitem->m_Type == ITEMGET(13,49) || sitem->m_Type == ITEMGET(13,50))
						{

						}
						else if(sitem->m_Type == ITEMGET(13,2) && sitem->m_Durability == 255.0f)
						{

						}
						else if(sitem->m_Type == ITEMGET(13,16) || sitem->m_Type == ITEMGET(13,17))
						{

						}

						else if(sitem->m_Type == ITEMGET(12,30) || sitem->m_Type == ITEMGET(12,31))
						{

						}

						else if(sitem->m_Type == ITEMGET(14,31))
						{

						}

						else if(sitem->m_Type == ITEMGET(12,26))
						{

						}

						else if(sitem->m_Type == ITEMGET(12,136))
						{

						}

						else if(sitem->m_Type == ITEMGET(13,276))
						{

						}

						else if(sitem->m_Type == ITEMGET(13,277))
						{

						}

						else if(
							sitem->m_Type == ITEMGET(12,30) ||
							sitem->m_Type == ITEMGET(12,31) ||
							sitem->m_Type == ITEMGET(13,32) ||
							sitem->m_Type == ITEMGET(13,33) ||
							sitem->m_Type == ITEMGET(13,34) ||
							sitem->m_Type == ITEMGET(13,35) ||
							sitem->m_Type == ITEMGET(13,36) ||
							sitem->m_Type == ITEMGET(13,37) ||
							sitem->m_Type == ITEMGET(13,51) ||
							sitem->m_Type == ITEMGET(13,52) ||
							sitem->m_Type == ITEMGET(13,53) ||
							sitem->m_Type == ITEMGET(12,140) ||
							sitem->m_Type == ITEMGET(12,137) ||
							sitem->m_Type == ITEMGET(13,147) 
							)
						{

						}
						else if(sitem->m_Type == ITEMGET(14,3) || sitem->m_Type == ITEMGET(14,38) || sitem->m_Type == ITEMGET(14,39))
						{

						}
						else if(g_kJewelOfHarmonySystem.IsJewelOfHarmonyPurity(sitem->m_Type) == 1)
						{

						}
						else if(sitem->m_Type == ITEMGET(12,30))
						{

						}
						else if(sitem->m_Type == ITEMGET(12,31))
						{

						}
						else if(sitem->m_Type == ITEMGET(13,51))
						{

						}
						else if(sitem->m_Type == ITEMGET(13,52))
						{

						}
						else if(sitem->m_Type == ITEMGET(13,53))
						{

						}
						else if(sitem->m_Type >= ITEMGET(13,83) && sitem->m_Type <= ITEMGET(13,92))
						{

						}
						else if(sitem->m_Type == ITEMGET(12,140))
						{

						}
						else if(sitem->m_Type == ITEMGET(12,137))
						{

						}
						else if(sitem->m_Type == ITEMGET(14,31))
						{

						}
						else if(sitem->m_Type == ITEMGET(14,53))
						{

						}
						else if(sitem->m_Type == ITEMGET(14,96))
						{
						
						}
						else if(sitem->m_Type >= ITEMGET(14,112) && sitem->m_Type <= ITEMGET(14,113))
						{

						}
						else if(sitem->m_Type >= ITEMGET(14,121) && sitem->m_Type <= ITEMGET(14,122))
						{

						}
						else if(sitem->m_Type >= ITEMGET(14,103) && sitem->m_Type <= ITEMGET(14,108))
						{

						}

						else if(sitem->m_Type >= ITEMGET(14,176) && sitem->m_Type <= ITEMGET(14,180))
						{

						}

						else if(sitem->m_Type >= ITEMGET(14,181) && sitem->m_Type <= ITEMGET(14,190))
						{

						}

						else if(sitem->m_Type >= ITEMGET(14,191) && sitem->m_Type <= ITEMGET(14,202))
						{

						}

						else if (sitem->m_Type >= ITEMGET(14, 210) && sitem->m_Type <= ITEMGET(14, 212))
						{

						}
						else
						{
							return -1;
						}
					}
					else if(sitem->m_Type == ITEMGET(14,11))
					{
						return -1;
					}
					else if(IsJumpingEventItem(sitem->m_Type) == TRUE)
					{
						return -1;
					}
				}
			}
			break;
		case 4:
			if(gDoPShopOpen == 0)
			{
				return -1;
			}

			if(gObj[aIndex].m_bPShopOpen == 1)
			{
				return -1;
			}

			if(sitem->m_Type == ITEMGET(14,11) && sitem->m_Level == 13)
			{
				return -1;
			}

			if(g_kJewelOfHarmonySystem.IsStrengthenByJewelOfHarmony(sitem) == 1)
			{
				GSProtocol.GCServerMsgStringSend(Lang.GetText(0,268),lpObj->m_Index,1);
				return -1;
			}

			if(IsJumpingEventItem(sitem->m_Type) == 1)
			{
				return -1;
			}

			if (IsPersonalStoreItem(sitem->m_Type))
			{
				if (sitem->m_Type == ITEMGET(12,26) && sitem->m_Level)
				{
					return -1;
				}
			}

			else
			{
				if (sitem->m_Type != ITEMGET(14,21) || sitem->m_Level != 3)
				{
					return -1;
				}
			}

			if(g_ConfigRead.data.common.StoreExcItemLimit == FALSE)
			{
				if(sitem->m_NewOption == 63)
				{
					return -1;
				}
			}

			if(sitem->m_Type == ITEMGET(13,38))
			{
				GSProtocol.GCServerMsgStringSend(Lang.GetText(0,286),lpObj->m_Index,1);
				return -1;
			}

			if(sitem->m_Type == ITEMGET(13,39))
			{
				return -1;
			}

			EnterCriticalSection(&gObj[aIndex].m_critPShopTrade);

			if(gObj[aIndex].m_bPShopTransaction == 1)
			{
				if(bSourceIsPShop == 1)
				{
					lpObj->m_bPShopTransaction = 1;
					bPersonalShopTrans = 1;
				}
				else
				{
					g_Log.Add("[PShop] [%s][%s] PShop Item Move Request Failed : Already Trade With Other",gObj[aIndex].AccountID,gObj[aIndex].Name);
					LeaveCriticalSection(&lpObj->m_critPShopTrade);
					return -1;
				}
			}
			else
			{
				lpObj->m_bPShopTransaction = 1;
				bPersonalShopTrans = 1;
			}

			LeaveCriticalSection(&lpObj->m_critPShopTrade);

			if(target < MAIN_INVENTORY_SIZE || target > (INVENTORY_SIZE - 1))
			{
				g_Log.Add("error-L1 : %s %d",__FILE__,__LINE__);
				return -1;
			}

			titem = &lpObj->pInventory[target];

			break;
		case 17:
			if (target > CHAOS_BOX_SIZE - 1)
			{
				g_Log.Add("error-L1 : %s %d", __FILE__, __LINE__);
				return -1;
			}

			titem = &lpObj->m_PlayerData->pPentagramMixBox[target];
			break;
		default: return -1;
		}

		s_num = sitem->GetNumber();

		if(gObjCheckSerial0ItemList(sitem))
		{
			MsgOutput(lpObj->m_Index,Lang.GetText(0,259));
			g_Log.Add("[ANTI-HACK][Serial 0 Item] [MoveItem] (%s)(%s) Item(%s) Pos(%d)",lpObj->AccountID,lpObj->Name,sitem->GetName(),source);
			return -1;
		}

		if(gObjInventorySearchSerialNumber(lpObj,s_num) == 0)
		{
			return -1;
		}

		if(gObjWarehouseSearchSerialNumber(lpObj,s_num) == 0)
		{
			return -1;
		}

		if (titem->IsItem() == 1)
		{
			if (sFlag != 0)
			{
				return -1;
			}

			if (sitem->m_Type != titem->m_Type)
			{
				return -1;
			}

			if (sitem->m_Level != titem->m_Level)
			{
				return -1;
			}

			int max_count = IsOverlapItem(titem->m_Type);

			if (max_count == 0)
			{
				return -1;
			}

			int t_dur = titem->m_Durability;

			if (t_dur < max_count)
			{
				int dif_dur = max_count - int(titem->m_Durability);

				if (dif_dur > sitem->m_Durability)
				{
					dif_dur = sitem->m_Durability;
				}

				titem->m_Durability += dif_dur;
				sitem->m_Durability -= dif_dur;

				if (titem->m_Type == ITEMGET(14, 29) && max_count <= titem->m_Durability)
				{
					titem->m_Durability -= max_count;

					if (titem->m_Durability == 0)
					{
						gObjInventoryItemSet(aIndex, target, 255);
						gObj[aIndex].pInventory[target].Clear();
						GSProtocol.GCInventoryItemDeleteSend(aIndex, target, 1);
						durTsend = 0;
					}
					else
					{
						durTsend = 1;
					}

					ItemSerialCreateSend(aIndex, 235, gObj[aIndex].X, gObj[aIndex].Y, ItemGetNumberMake(14, 28), sitem->m_Level, 0, 0, 0, 0, aIndex, 0, 0, 0, 0, 0);
				}

				if (titem->m_Type == ITEMGET(14, 110) && max_count <= titem->m_Durability)
				{
					titem->m_Durability -= max_count;

					if (titem->m_Durability == 0)
					{
						gObjInventoryItemSet(aIndex, target, 255);
						gObj[aIndex].pInventory[target].Clear();
						GSProtocol.GCInventoryItemDeleteSend(aIndex, target, 1);
						durTsend = 0;
					}
					else
					{
						durTsend = 1;
					}

					ItemSerialCreateSend(aIndex, 235, gObj[aIndex].X, gObj[aIndex].Y, ItemGetNumberMake(14, 111), sitem->m_Level, 0, 0, 0, 0, aIndex, 0, 0, 0, 0, 0);
				}

				if (titem->m_Type == ITEMGET(12, 144) && max_count <= titem->m_Durability && titem->m_BonusSocketOption == sitem->m_BonusSocketOption)
				{
					titem->m_Durability -= max_count;

					BYTE btMainAttribute = titem->m_BonusSocketOption;

					if (titem->m_Durability == 0)
					{
						gObjInventoryDeleteItem(aIndex, target);
						GSProtocol.GCInventoryItemDeleteSend(aIndex, target, 1);
						durTsend = 0;
					}
					else
					{
						durTsend = 1;
					}

					ItemSerialCreateSend(aIndex, 235, gObj[aIndex].X, gObj[aIndex].Y, ItemGetNumberMake(12, 148), sitem->m_Level, 0, 0, 0, 0, aIndex, 0, 0, 0, 0, sitem->m_BonusSocketOption);
				}

				if (titem->m_Type == ITEMGET(12, 146) && max_count <= titem->m_Durability)
				{
					titem->m_Durability -= max_count;

					if (titem->m_Durability == 0)
					{
						gObjInventoryItemSet(aIndex, target, 255);
						gObj[aIndex].pInventory[target].Clear();
						GSProtocol.GCInventoryItemDeleteSend(aIndex, target, 1);
						durTsend = 0;
					}
					else
					{
						durTsend = 1;
					}

					ItemSerialCreateSend(aIndex, 235, gObj[aIndex].X, gObj[aIndex].Y, ItemGetNumberMake(12, 149), sitem->m_Level, 0, 0, 0, 0, aIndex, 0, 0, 0, 0, 0);
				}

				if (titem->m_Type == ITEMGET(13, 145) && max_count <= titem->m_Durability)
				{
					titem->m_Durability -= max_count;

					if (titem->m_Durability == 0)
					{
						gObjInventoryItemSet(aIndex, target, 255);
						gObj[aIndex].pInventory[target].Clear();
						GSProtocol.GCInventoryItemDeleteSend(aIndex, target, 1);
						durTsend = 0;
					}
					else
					{
						durTsend = 1;
					}

					ItemSerialCreateSend(aIndex, 235, gObj[aIndex].X, gObj[aIndex].Y, ItemGetNumberMake(13, 146), sitem->m_Level, 0, 0, 0, 0, aIndex, 0, 0, 0, 0, 0);
				}

				if (titem->m_Type == ITEMGET(14, 101) && max_count <= titem->m_Durability)
				{
					titem->m_Durability -= max_count;

					if (titem->m_Durability == 0)
					{
						gObjInventoryItemSet(aIndex, target, 255);
						gObj[aIndex].pInventory[target].Clear();
						GSProtocol.GCInventoryItemDeleteSend(aIndex, target, 1);
						durTsend = 0;
					}
					else
					{
						durTsend = 1;
					}

					ItemSerialCreateSend(aIndex, 235, gObj[aIndex].X, gObj[aIndex].Y, ItemGetNumberMake(14, 102), sitem->m_Level, 0, 0, 0, 0, aIndex, 0, 0, 0, 0, 0);
				}

				if (sitem->m_Durability > 0)
				{
					durSsend = 1;
					durTsend = 1;
				}
				else
				{
					switch (sFlag)
					{
					case 0:
						gObjInventoryDeleteItem(aIndex, source);
						GSProtocol.GCInventoryItemDeleteSend(lpObj->m_Index, source, 1);
						durTsend = 1;
					default:	return 0;
					}
				}
			}
			return -1;
		}

		if (IT_MAP_RANGE(lpObj->MapNumber) == TRUE)
		{
			if (lpObj->pInventory[source].m_Type == ITEMGET(14, 223))
			{
				g_IT_Event.SetRelicsInventoryPos(lpObj->MapNumber, aIndex, target);
				g_Log.Add("[ ITR ][gObjInventoryMoveItem]Name:%s,Index:%d,target:%d", lpObj->Name, aIndex, target);
			}
		}

		switch(tFlag)
		{
		case 0:
		case 4:
			if(target < INVETORY_WEAR_SIZE || target == 236)
			{
				if(lpObj->MapNumber == MAP_INDEX_ICARUS)
				{
					if(target == 10)
					{
						if(lpObj->pInventory[source].m_Type == ITEMGET(13,10))
						{
							return -1;
						}
					}
					else if(target == 11)
					{
						if(lpObj->pInventory[source].m_Type == ITEMGET(13,10))
						{
							return -1;
						}
					}
					if(target == 10)
					{
						if(lpObj->pInventory[source].m_Type == ITEMGET(13,40))
						{
							return -1;
						}
					}
					else if(target == 11)
					{
						if(lpObj->pInventory[source].m_Type == ITEMGET(13,40))
						{
							return -1;
						}
					}
				}

				if(target < INVETORY_WEAR_SIZE || target == 236)
				{
					if(lpObj->MapNumber == MAP_INDEX_ICARUS || lpObj->MapNumber == MAP_INDEX_KANTURU_BOSS)
					{
						if(target == 10)
						{
							if(lpObj->pInventory[source].m_Type == ITEMGET(13,39) || lpObj->pInventory[source].m_Type == ITEMGET(13,40) ||
								lpObj->pInventory[source].m_Type == ITEMGET(13,10))
							{
								return -1;
							}
						}
						else if(target == 11)
						{
							if(lpObj->pInventory[source].m_Type == ITEMGET(13,39) || lpObj->pInventory[source].m_Type == ITEMGET(13,40) ||
								lpObj->pInventory[source].m_Type == ITEMGET(13,10))
							{
								return -1;
							}
						}
					}
				}

				if (g_MineSystem.IsPickax(lpObj->pInventory[source].m_Type) == TRUE && lpObj->m_bPShopOpen)
					return -1;

				useClass = sitem->IsClass(lpObj->Class,lpObj->m_PlayerData->ChangeUP);

				if(useClass == 0)
					return -1;

				if(gObjIsItemPut(lpObj,sitem,target) == 0)
					return -1;

				if(!CheckCanWearResetItem(lpObj->m_Index,lpObj->pInventory[source].m_Type))
					return -1;

			}
			else 
			{
				if(target >= MAIN_INVENTORY_SIZE && target != 236)
				{
					if(lpObj->pInventory[source].m_Type == ITEMGET(12,26))
					{
						if(lpObj->pInventory[source].m_Level == 1 ||
							lpObj->pInventory[source].m_Level == 2 ||
							lpObj->pInventory[source].m_Level == 3 ||
							lpObj->pInventory[source].m_Level == 4 ||
							lpObj->pInventory[source].m_Level == 5)
						{
							return -1;
						}
					}	
				}

				w = (target - INVETORY_WEAR_SIZE)%8;
				h = (target - INVETORY_WEAR_SIZE)/8;

				if (!InventoryExtentCheck(w, h, 8, MAX_INVENTORY_H2))
				{
					return -1;
				}

				sitem->GetSize((int &)iwidth,(int &)iheight);
				memcpy(TempInventoryMap,lpObj->pInventoryMap,size);

				switch(sFlag)
				{
				case 0:
				case 4:
					if(source > (INVETORY_WEAR_SIZE - 1) && source != 236)
					{
						gObjInventoryItemBoxSet(lpObj->m_Index,source,iwidth,iheight,255);
					}
					break;
				default: break;
				}
			
			if(*(BYTE*)(gObj[aIndex].pInventoryMap + h * 8 + w) != 255)
			{
				memcpy(lpObj->pInventoryMap,TempInventoryMap,size);
				return -1;
			}

			blank = gObjInventoryRectCheck(lpObj->m_Index,w,h,iwidth,iheight);

			if(blank >= 254)
			{
				memcpy(lpObj->pInventoryMap,TempInventoryMap,size);
				return -1;
			}
			}

			s_num = sitem->GetNumber();
			titem = sitem;
			BOOL bSendEquip;

				switch(sFlag)
				{
				case 0:
				case 4:
					lpObj->pInventory[target] = lpObj->pInventory[source];

					if (source < 204 || source == 236 || target >= 204 && target != 236)
					{
						if (source >= 204 && source != 236 && target >= 204 && target != 236)
							g_PersonalStore.GDMovePShopItem(lpObj, source, target);
					}

					else
					{
						g_PersonalStore.GDDelPShopItemValue(lpObj, source);
					}

					bSendEquip = FALSE;

					if ((source == 11 || source == 10)
						&& (sitem->m_Type == ITEMGET(13, 169) || sitem->m_Type == ITEMGET(13, 170))
						|| (target == 11 || target == 10)
						&& (titem->m_Type == ITEMGET(13, 169) || titem->m_Type == ITEMGET(13, 170)))
						bSendEquip = TRUE;

					gObjInventoryDeleteItem(aIndex,source);

					if (bSendEquip)
					{
						GSProtocol.GCEquipmentSend(aIndex);
					}

					gObjSetInventoryEquipment(lpObj, target);

					if (target > 12 && g_LuckyItemManager.IsLuckyItemEquipment(lpObj->pInventory[target].m_Type))
					{
						CItem * lpItem = &lpObj->pInventory[target];
						g_LuckyItemManager.GDReqLuckyItemInsert(lpItem, lpObj->m_Index);
					}
					break;
				case 2:
					if(g_PentagramSystem.IsPentagramItem(&lpObj->pWarehouse[source]) == true)
					{
						g_PentagramSystem.SwitchPentagramJewel(aIndex, &lpObj->pWarehouse[source], 0);
						g_PentagramSystem.GCPentagramJewelInfo(aIndex, 0);
						g_PentagramSystem.GCPentagramJewelInfo(aIndex, 1);

						CItem Item;
						memcpy(&Item, &lpObj->pWarehouse[source], sizeof(Item));
						ItemByteConvert(siteminfo, Item);
					}
					lpObj->pInventory[target] = lpObj->pWarehouse[source];
					gObjWarehouseDeleteItem(aIndex,source);

					if (target > 12 && g_LuckyItemManager.IsLuckyItemEquipment(lpObj->pInventory[target].m_Type))
					{
						CItem * lpItem = &lpObj->pInventory[target];
						g_LuckyItemManager.GDReqLuckyItemInsert(lpItem, lpObj->m_Index);
					}
					break;
					break;
				case 17:
					lpObj->pInventory[target] = lpObj->m_PlayerData->pPentagramMixBox[source];
					gObjPentagramMixBoxDeleteItem(aIndex, source);
					break;
				case 3:
				case 5:
				case 6:
				case 7:
				case 8:
				case 9:
				case 10:
				case 11:
				case 12:
				case 13:
				case 14:
				case 15:
				case 16:
				case 19:
				case 20:
					lpObj->pInventory[target] = lpObj->pChaosBox[source];
					gObjChaosBoxDeleteItem(aIndex,source);
					break;
				default : break;
				}

				if(target > (INVETORY_WEAR_SIZE - 1) && target != 236)
				{
					gObjInventoryItemSet(aIndex,target,1);
				}

				if(target < INVETORY_WEAR_SIZE || target == 236)
				{
					CItem * lpItem = &lpObj->pInventory[target];
					if(lpItem->m_Option1)
					{
						int s_pos = gObjWeaponMagicAdd(&gObj[aIndex],lpItem->m_Special[0],lpItem->m_Level);
						if(s_pos >= 0)
						{
							GSProtocol.GCMagicListOneSend(aIndex,s_pos,lpItem->m_Special[0],lpItem->m_Level,0,0);
						}
					}
				}
			break;
		case 2:
			BYTE res_1;
				switch(sFlag)
				{
				case 0:
					if(lpObj->pInventory[source].m_Type == ITEMGET(13,20) || lpObj->pInventory[source].m_Type == ITEMGET(13,107))
					{
						if(lpObj->pInventory[source].m_Level == 0 ||
							lpObj->pInventory[source].m_Level == 1)
						{
							return -1;
						}
					}

					if(lpObj->pInventory[source].m_Type == ITEMGET(14,11))
					{
						if(lpObj->pInventory[source].m_Level == 13)
						{
							return -1;
						}
					}

					if(lpObj->pInventory[source].m_Type == ITEMGET(12,26))
					{
						if(lpObj->pInventory[source].m_Level == 1 ||
							lpObj->pInventory[source].m_Level == 2 ||
							lpObj->pInventory[source].m_Level == 3 ||
							lpObj->pInventory[source].m_Level == 4 ||
							lpObj->pInventory[source].m_Level == 5)
						{
							return -1;
						}
					}

					if(lpObj->pInventory[source].m_Type == ITEMGET(13,39))
					{
						return -1;
					}

					if (lpObj->pInventory[source].m_Type == ITEMGET(13, 42)) //Season 2.5 add-on
					{
						return -1;
					}

					if (lpObj->pInventory[source].m_QuestItem && !g_QuestExpManager.IsQuestItemAtt(lpObj->pInventory[source].m_Type, 4) ) //Season 2.5 add-on
					{
						return -1;
					}

					if (lpObj->pInventory[source].m_Type == ITEMGET(13, 70)) //Season 4 add-on
					{
						if (lpObj->pInventory[source].m_Durability == 1.0f) //??
						{
							return -1;
						}
					}

					if (lpObj->pInventory[source].IsPeriodItem())
					{
						return -1;
					}

					if (!IsStoreWarehouseItem(lpObj->pInventory[source].m_Type) && (lpObj->pInventory[source].m_Type != ITEMGET(14, 21) || lpObj->pInventory[source].m_Level != 3))
					{
						return -1;
					}

					if (lpObj->pInventory[source].m_Type == ITEMGET(14, 163))
					{
						return -1;
					}

					res_1 = gObjWarehouseInsertItemPos(aIndex,lpObj->pInventory[source],target,-1);
					break;
				case 2:
					res_1 = gObjWarehouseInsertItemPos(aIndex,lpObj->pWarehouse[source],target,source);
					break;
				default : break;
				}

				if(res_1 == 255)
				{
					return -1;
				}

				switch(sFlag)
				{
				case 0:
					if(g_PentagramSystem.IsPentagramItem(&lpObj->pInventory[source]) == true)
					{
						g_PentagramSystem.SwitchPentagramJewel(aIndex, &lpObj->pInventory[source], 1);
						g_PentagramSystem.GCPentagramJewelInfo(aIndex, 0);
						g_PentagramSystem.GCPentagramJewelInfo(aIndex, 1);

						CItem Item;
						memcpy(&Item, &lpObj->pInventory[source], sizeof(Item));
						ItemByteConvert(siteminfo, Item);
					}
					lpObj->pWarehouse[target] = lpObj->pInventory[source];
					gObjInventoryDeleteItem(aIndex,source);
					break;
				case 2:
					lpObj->pWarehouse[target] = lpObj->pWarehouse[source];
					gObjWarehouseDeleteItem(aIndex,source);
					break;
				default : break;
				}

				gObjWarehouseItemSet(aIndex,target,1);

				if(sFlag == 0)
				{
					if(source < INVETORY_WEAR_SIZE || source == 236)
					{
						if(source == 10 || source == 11)
						{
							if(lpObj->pWarehouse[target].m_Type == ITEMGET(13,10))
							{
								gObjUseSkill.SkillChangeUse(aIndex);
							}
							if(lpObj->pWarehouse[target].m_Type == ITEMGET(13,40))
							{
								gObjUseSkill.SkillChangeUse(aIndex);
							}
							if(lpObj->pWarehouse[target].m_Type == ITEMGET(13,41))
							{
								gObjUseSkill.SkillChangeUse(aIndex);
							}
							if(lpObj->pWarehouse[target].m_Type == ITEMGET(13,42))
							{
								gObjUseSkill.SkillChangeUse(aIndex);
							}
							if(lpObj->pWarehouse[target].m_Type == ITEMGET(13,68))
							{
								gObjUseSkill.SkillChangeUse(aIndex);
							}
							if(lpObj->pWarehouse[target].m_Type == ITEMGET(13,76))
							{
								gObjUseSkill.SkillChangeUse(aIndex);
							}
							if(lpObj->pWarehouse[target].m_Type == ITEMGET(13,77))
							{
								gObjUseSkill.SkillChangeUse(aIndex);
							}
							if(lpObj->pWarehouse[target].m_Type == ITEMGET(13,78))
							{
								gObjUseSkill.SkillChangeUse(aIndex);
							}
							if(lpObj->pWarehouse[target].m_Type == ITEMGET(13,122))
							{
								gObjUseSkill.SkillChangeUse(aIndex);
							}
							if (lpObj->pWarehouse[target].m_Type == ITEMGET(13, 163))
							{
								gObjUseSkill.SkillChangeUse(aIndex);
							}
							if (lpObj->pWarehouse[target].m_Type == ITEMGET(13, 164))
							{
								gObjUseSkill.SkillChangeUse(aIndex);
							}
							if (lpObj->pWarehouse[target].m_Type == ITEMGET(13, 165))
							{
								gObjUseSkill.SkillChangeUse(aIndex);
							}
							if (lpObj->pWarehouse[target].m_Type == ITEMGET(13, 268))
							{
								gObjUseSkill.SkillChangeUse(aIndex);
							}
						}

						gObjMakePreviewCharSet(aIndex);
						GSProtocol.GCEquipmentChange(aIndex,source);
					}
				}
			return 2;
		case 3:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		case 15:
		case 16:
		case 19:
		case 20:
			BYTE res_2;
				switch(sFlag)
				{
				case 0:
					res_2 = gObjChaosBoxInsertItemPos(aIndex,lpObj->pInventory[source],target,-1);
					break;
				case 3:
				case 5:
				case 6:
				case 7:
				case 8:
				case 9:
				case 10:
				case 11:
				case 12:
				case 13:
				case 14:
				case 15:
				case 16:
				case 19:
				case 20:
					res_2 = gObjChaosBoxInsertItemPos(aIndex,lpObj->pChaosBox[source],target,source);
					break;
				default : break;
				}

				if(res_2 == 255)
				{
					return -1;
				}

				switch(sFlag)
				{
				case 0:
					lpObj->pChaosBox[target] = lpObj->pInventory[source];
					gObjInventoryDeleteItem(aIndex,source);
					break;
				case 3:
				case 5:
				case 6:
				case 7:
				case 8:
				case 9:
				case 10:
				case 11:
				case 12:
				case 13:
				case 14:
				case 15:
				case 16:
				case 19:
				case 20:
					lpObj->pChaosBox[target] = lpObj->pChaosBox[source];
					gObjChaosBoxDeleteItem(aIndex,source);
					break;
				default : break;
				}

				gObjChaosItemSet(aIndex,target,1);
				GSProtocol.GCChaosMachinePriceSend(aIndex);
				if(sFlag == 0)
				{
					if(source < INVETORY_WEAR_SIZE || source == 236)
					{
						if(source == 10 || source == 11)
						{
							if(lpObj->pChaosBox[target].m_Type == ITEMGET(13,10))
							{
								gObjUseSkill.SkillChangeUse(aIndex);
							}

							if(lpObj->pChaosBox[target].m_Type == ITEMGET(13,39))
							{
								gObjUseSkill.SkillChangeUse(aIndex);
							}

							if(lpObj->pChaosBox[target].m_Type == ITEMGET(13,40))
							{
								gObjUseSkill.SkillChangeUse(aIndex);
							}

							if(lpObj->pChaosBox[target].m_Type == ITEMGET(13,41))
							{
								gObjUseSkill.SkillChangeUse(aIndex);
							}

							if(lpObj->pChaosBox[target].m_Type == ITEMGET(13,42))
							{
								gObjUseSkill.SkillChangeUse(aIndex);
							}

							if(lpObj->pChaosBox[target].m_Type == ITEMGET(13,68))
							{
								gObjUseSkill.SkillChangeUse(aIndex);
							}

							if(lpObj->pChaosBox[target].m_Type == ITEMGET(13,76))
							{
								gObjUseSkill.SkillChangeUse(aIndex);
							}

							if(lpObj->pChaosBox[target].m_Type == ITEMGET(13,77))
							{
								gObjUseSkill.SkillChangeUse(aIndex);
							}

							if(lpObj->pChaosBox[target].m_Type == ITEMGET(13,78))
							{
								gObjUseSkill.SkillChangeUse(aIndex);
							}

							if(lpObj->pChaosBox[target].m_Type == ITEMGET(13,122))
							{
								gObjUseSkill.SkillChangeUse(aIndex);
							}
							if (lpObj->pChaosBox[target].m_Type == ITEMGET(13, 163))
							{
								gObjUseSkill.SkillChangeUse(aIndex);
							}
							if (lpObj->pChaosBox[target].m_Type == ITEMGET(13, 164))
							{
								gObjUseSkill.SkillChangeUse(aIndex);
							}
							if (lpObj->pChaosBox[target].m_Type == ITEMGET(13, 165))
							{
								gObjUseSkill.SkillChangeUse(aIndex);
							}
							if (lpObj->pChaosBox[target].m_Type == ITEMGET(13, 268))
							{
								gObjUseSkill.SkillChangeUse(aIndex);
							}
						}

						gObjMakePreviewCharSet(aIndex);
						GSProtocol.GCEquipmentChange(aIndex,source);
					}
				}
			return tFlag;
		case 17:
			BYTE res_3;
				if (sFlag == 17)
				{
					res_3 = gObjPentagramMixBoxInsertItemPos(aIndex, lpObj->m_PlayerData->pPentagramMixBox[source], target, source);
				}
				
				else
				{
					res_3 = gObjPentagramMixBoxInsertItemPos(aIndex, lpObj->pInventory[source], target, -1);
				}

				if (res_3 == 0xFF)
				{
					return -1;
				}

				if (sFlag == 17)
				{
					lpObj->m_PlayerData->pPentagramMixBox[target] = lpObj->m_PlayerData->pPentagramMixBox[source];
					gObjPentagramMixBoxDeleteItem(aIndex, source);
				}

				else
				{
					lpObj->m_PlayerData->pPentagramMixBox[target] = lpObj->pInventory[source];
					gObjInventoryDeleteItem(aIndex, source);
				}

				gObjPentagramMixItemSet(aIndex, target, 1);

				if (sFlag == 0 && source == 236)
				{
					GSProtocol.GCEquipmentChange(aIndex, source);
				}

				return tFlag;
			break;

		default: break;
		}

		if(sFlag == 0 && (source < INVETORY_WEAR_SIZE || source == 236))
		{
			if(lpObj->pInventory[source].IsItem() == 1)
			{
				if(lpObj->pInventory[source].m_Type == ITEMGET(13,10))
				{
					gObjUseSkill.SkillChangeUse(aIndex);
					g_Log.Add("Using transformation ring: %s : %d",gObj[aIndex].Name,lpObj->pInventory[source].m_Level);
				}

				if(lpObj->pInventory[source].m_Type == ITEMGET(13,39))
				{
					gObjUseSkill.SkillChangeUse(aIndex);
					g_Log.Add("Using transformation ring: %s : %d",gObj[aIndex].Name,lpObj->pInventory[source].m_Level);
				}

				if(lpObj->pInventory[source].m_Type == ITEMGET(13,40))
				{
					gObjUseSkill.SkillChangeUse(aIndex);
					g_Log.Add("Using transformation ring: %s : %d",gObj[aIndex].Name,lpObj->pInventory[source].m_Level);
				}

				if(lpObj->pInventory[source].m_Type == ITEMGET(13,41))
				{
					gObjUseSkill.SkillChangeUse(aIndex);
					g_Log.Add("Using transformation ring: %s : %d",gObj[aIndex].Name,lpObj->pInventory[source].m_Level);
				}

				if(lpObj->pInventory[source].m_Type == ITEMGET(13,42))
				{
					gObjUseSkill.SkillChangeUse(aIndex);
					g_Log.Add("Using transformation ring: %s : %d",gObj[aIndex].Name,lpObj->pInventory[source].m_Level);
				}

				if(lpObj->pInventory[source].m_Type == ITEMGET(13,68))
				{
					gObjUseSkill.SkillChangeUse(aIndex);
					g_Log.Add("Using transformation ring: %s : %d",gObj[aIndex].Name,lpObj->pInventory[source].m_Level);
				}

				if(lpObj->pInventory[source].m_Type == ITEMGET(13,76))
				{
					gObjUseSkill.SkillChangeUse(aIndex);
					g_Log.Add("Using transformation ring: %s : %d",gObj[aIndex].Name,lpObj->pInventory[source].m_Level);
				}

				if(lpObj->pInventory[source].m_Type == ITEMGET(13,77))
				{
					gObjUseSkill.SkillChangeUse(aIndex);
					g_Log.Add("Using transformation ring: %s : %d",gObj[aIndex].Name,lpObj->pInventory[source].m_Level);
				}

				if(lpObj->pInventory[source].m_Type == ITEMGET(13,78))
				{
					gObjUseSkill.SkillChangeUse(aIndex);
					g_Log.Add("Using transformation ring: %s : %d",gObj[aIndex].Name,lpObj->pInventory[source].m_Level);
				}

				if(lpObj->pInventory[source].m_Type == ITEMGET(13,122))
				{
					gObjUseSkill.SkillChangeUse(aIndex);
					g_Log.Add("Using transformation ring: %s : %d",gObj[aIndex].Name,lpObj->pInventory[source].m_Level);
				}
				if (lpObj->pInventory[source].m_Type == ITEMGET(13, 163))
				{
					gObjUseSkill.SkillChangeUse(aIndex);
					g_Log.Add("Using transformation ring: %s : %d", gObj[aIndex].Name, lpObj->pInventory[source].m_Level);
				}
				if (lpObj->pInventory[source].m_Type == ITEMGET(13, 164))
				{
					gObjUseSkill.SkillChangeUse(aIndex);
					g_Log.Add("Using transformation ring: %s : %d", gObj[aIndex].Name, lpObj->pInventory[source].m_Level);
				}
				if (lpObj->pInventory[source].m_Type == ITEMGET(13, 165))
				{
					gObjUseSkill.SkillChangeUse(aIndex);
					g_Log.Add("Using transformation ring: %s : %d", gObj[aIndex].Name, lpObj->pInventory[source].m_Level);
				}
				if (lpObj->pInventory[source].m_Type == ITEMGET(13, 268))
				{
					gObjUseSkill.SkillChangeUse(aIndex);
					g_Log.Add("Using transformation ring: %s : %d", gObj[aIndex].Name, lpObj->pInventory[source].m_Level);
				}
			}
			else
			{
				if(source == 10 || source == 11)
				{
					if(lpObj->pInventory[target].m_Type == ITEMGET(13,10))
					{
						gObjUseSkill.SkillChangeUse(aIndex);
					}

					if(lpObj->pInventory[target].m_Type == ITEMGET(13,39))
					{
						gObjUseSkill.SkillChangeUse(aIndex);
					}

					if(lpObj->pInventory[target].m_Type == ITEMGET(13,40))
					{
						gObjUseSkill.SkillChangeUse(aIndex);
					}

					if(lpObj->pInventory[target].m_Type == ITEMGET(13,41))
					{
						gObjUseSkill.SkillChangeUse(aIndex);
					}

					if(lpObj->pInventory[target].m_Type == ITEMGET(13,42))
					{
						gObjUseSkill.SkillChangeUse(aIndex);
					}

					if(lpObj->pInventory[target].m_Type == ITEMGET(13,68))
					{
						gObjUseSkill.SkillChangeUse(aIndex);
					}

					if(lpObj->pInventory[target].m_Type == ITEMGET(13,76))
					{
						gObjUseSkill.SkillChangeUse(aIndex);
					}

					if(lpObj->pInventory[target].m_Type == ITEMGET(13,77))
					{
						gObjUseSkill.SkillChangeUse(aIndex);
					}

					if(lpObj->pInventory[target].m_Type == ITEMGET(13,78))
					{
						gObjUseSkill.SkillChangeUse(aIndex);
					}

					if(lpObj->pInventory[target].m_Type == ITEMGET(13,122))
					{
						gObjUseSkill.SkillChangeUse(aIndex);
					}
					if (lpObj->pInventory[target].m_Type == ITEMGET(13, 163))
					{
						gObjUseSkill.SkillChangeUse(aIndex);
					}
					if (lpObj->pInventory[target].m_Type == ITEMGET(13, 164))
					{
						gObjUseSkill.SkillChangeUse(aIndex);
					}
					if (lpObj->pInventory[target].m_Type == ITEMGET(13, 165))
					{
						gObjUseSkill.SkillChangeUse(aIndex);
					}
					if (lpObj->pInventory[target].m_Type == ITEMGET(13, 268))
					{
						gObjUseSkill.SkillChangeUse(aIndex);
					}
				}
			}

			gObjMakePreviewCharSet(aIndex);
			GSProtocol.GCEquipmentChange(aIndex,source);
		}

		if (tFlag == 0 && (target < INVETORY_WEAR_SIZE || target == 236))
		{
			if (lpObj->pInventory[target].m_Type == ITEMGET(13, 10))
			{
				gObjUseSkill.SkillChangeUse(aIndex);
			}

			if (lpObj->pInventory[target].m_Type == ITEMGET(13, 39))
			{
				gObjUseSkill.SkillChangeUse(aIndex);
			}

			if (lpObj->pInventory[target].m_Type == ITEMGET(13, 40))
			{
				gObjUseSkill.SkillChangeUse(aIndex);
			}

			if (lpObj->pInventory[target].m_Type == ITEMGET(13, 41))
			{
				gObjUseSkill.SkillChangeUse(aIndex);
			}

			if (lpObj->pInventory[target].m_Type == ITEMGET(13, 42))
			{
				gObjUseSkill.SkillChangeUse(aIndex);
			}

			if (lpObj->pInventory[target].m_Type == ITEMGET(13, 68))
			{
				gObjUseSkill.SkillChangeUse(aIndex);
			}

			if (lpObj->pInventory[target].m_Type == ITEMGET(13,76))
			{
				gObjUseSkill.SkillChangeUse(aIndex);
			}

			if (lpObj->pInventory[target].m_Type == ITEMGET(13,77))
			{
				gObjUseSkill.SkillChangeUse(aIndex);
			}

			if (lpObj->pInventory[target].m_Type == ITEMGET(13,78))
			{
				gObjUseSkill.SkillChangeUse(aIndex);
			}

			if (lpObj->pInventory[target].m_Type == ITEMGET(13, 122))
			{
				gObjUseSkill.SkillChangeUse(aIndex);
			}
			if (lpObj->pInventory[target].m_Type == ITEMGET(13, 163))
			{
				gObjUseSkill.SkillChangeUse(aIndex);
			}
			if (lpObj->pInventory[target].m_Type == ITEMGET(13, 164))
			{
				gObjUseSkill.SkillChangeUse(aIndex);
			}
			if (lpObj->pInventory[target].m_Type == ITEMGET(13, 165))
			{
				gObjUseSkill.SkillChangeUse(aIndex);
			}
			if (lpObj->pInventory[target].m_Type == ITEMGET(13, 268))
			{
				gObjUseSkill.SkillChangeUse(aIndex);
			}

			g_Log.Add("Using transformation ring: %s : %d", gObj[aIndex].Name, lpObj->pInventory[target].m_Level);
			gObjMakePreviewCharSet(aIndex);
			GSProtocol.GCEquipmentChange(aIndex, target);
		}
	}
	__finally
	{
		if(bPersonalShopTrans == 1)
		{
			gObj[aIndex].m_bPShopTransaction = 0;
		}
	}
	return false;
}




BYTE gObjTradeRectCheck(int aIndex, int sx, int sy, int width, int height )
{
	int x;
	int y;
	int blank = 0;
	int xx;
	int yy;

	if ( ( sx + width) > 8 )
	{
		return -1;
	}

	if ( (sy + height) > 4 )
	{
		return -1;
	}

	for ( y=0;y<height;y++)
	{
		yy = sy + y;
		
		for ( x=0;x<width;x++)
		{
			xx = sx + x;

			if ( ::ExtentCheck(xx, yy, 8, 4) == TRUE )
			{
				if ( *(BYTE*)(gObj[aIndex].TradeMap + yy * 8 + xx) != (BYTE)-1)
				{
					blank++;
					return -1;
				}
			}
			else
			{
				g_Log.Add("error : %s %d", __FILE__ ,__LINE__);
				return -1;
			}
		}
	}

	if ( blank == 0 )
	{
		return (sy * 8 + sx);
	}

	return -1;
}




BOOL gObjTradeItemBoxSet(int aIndex, int itempos, int xl, int yl, BYTE set_byte)
{
	int itemposx = (itempos)%8;
	int itemposy = (itempos)/8;

	int xx,yy;

	for(int y = 0; y < yl; y ++)
	{
		yy = itemposy + y;

		for(int x = 0; x < xl; x++)
		{
			xx = itemposx + x;

			if(ExtentCheck(xx,yy,8,4)==1)
			{
				*(BYTE*)(gObj[aIndex].TradeMap + yy*8+xx) = set_byte;
			}
			else
			{
				g_Log.Add("error : %s %d",__FILE__,__LINE__);
				return false;
			}
		}
	}
	return true;
}




BYTE gObjTradeInventoryMove(LPOBJ lpObj, BYTE source, BYTE target)
{
	int iwidth, iheight;
	UINT64 s_num;
	BYTE itembuf[13];
	if(source > TRADE_BOX_SIZE)
	{
		g_Log.Add("error : %s %d",__FILE__,__LINE__);
		return -1;
	}

	if(target > MAIN_INVENTORY_SIZE && target != 236)
	{
		g_Log.Add("error : %s %d",__FILE__,__LINE__);
		return -1;
	}

	if(lpObj->TargetNumber < 0)
	{
		return -1;
	}

	if(lpObj->Trade[source].IsItem() == 0)
	{
		return -1;
	}

	if(lpObj->m_IfState.use == 0 || lpObj->m_IfState.type != 1)
	{
		return -1;
	}

	if (IsEventItem(lpObj->Trade[source]) == TRUE)
	{
		return -1;
	}

	if(gObjInventoryInsertItemPos(lpObj->m_Index,lpObj->Trade[source],target,1) == 255)
	{
		return -1;
	}

	s_num = 0;
	s_num = lpObj->Trade[source].m_Number;

	lpObj->Trade[source].GetSize((int &)iwidth,(int &)iheight);

	gObjTradeItemBoxSet(lpObj->m_Index,source,iwidth,iheight,255);
	lpObj->Trade[source].Clear();

	ItemByteConvert(itembuf,lpObj->Trade[source]);

	GSProtocol.GCTradeOtherDel(lpObj->TargetNumber,source);
	if(gObj[lpObj->TargetNumber].m_PlayerData->ISBOT == true && g_BotSystem.GetBotType(lpObj->TargetNumber) == BOT_TRADE)
	{
		g_BotSystem.CheckAlchemist(lpObj->m_Index,gObj[lpObj->TargetNumber].m_PlayerData->wBotIndex);
	}
	if(gObjCheckSerial0ItemList(&lpObj->Trade[source]) != 0)
	{
		MsgOutput(lpObj->m_Index,Lang.GetText(0,259));
		g_Log.Add("[ANTI-HACK][Serial 0 Item] [Trade] (%s)(%s) Item(%s) Pos(%d)",lpObj->AccountID,lpObj->Name,lpObj->Trade[source].GetName(),source);
		return -1;
	}

	if(gObjInventorySearchSerialNumber(lpObj,s_num) == 0)
	{
		return -1;
	}

	if(target < INVETORY_WEAR_SIZE || target == 236)
	{
		if(lpObj->pInventory[target].IsItem() == 1)
		{
			if(lpObj->pInventory[target].m_Type == ITEMGET(13,10))
			{
				gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				g_Log.Add("Using transformation ring: %s : %d",lpObj->Name,lpObj->pInventory[target].m_Level);
			}
			if(lpObj->pInventory[target].m_Type == ITEMGET(13,39))
			{
				gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				g_Log.Add("Using transformation ring: %s : %d",lpObj->Name,lpObj->pInventory[target].m_Level);
			}
			if(lpObj->pInventory[target].m_Type == ITEMGET(13,40))
			{
				gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				g_Log.Add("Using transformation ring: %s : %d",lpObj->Name,lpObj->pInventory[target].m_Level);
			}
			if(lpObj->pInventory[target].m_Type == ITEMGET(13,41))
			{
				gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				g_Log.Add("Using transformation ring: %s : %d",lpObj->Name,lpObj->pInventory[target].m_Level);
			}
			if(lpObj->pInventory[target].m_Type == ITEMGET(13,42))
			{
				gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				g_Log.Add("Using transformation ring: %s : %d",lpObj->Name,lpObj->pInventory[target].m_Level);
			}
			if(lpObj->pInventory[target].m_Type == ITEMGET(13,68))
			{
				gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				g_Log.Add("Using transformation ring: %s : %d",lpObj->Name,lpObj->pInventory[target].m_Level);
			}
			if(lpObj->pInventory[target].m_Type == ITEMGET(13,76))
			{
				gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				g_Log.Add("Using transformation ring: %s : %d",lpObj->Name,lpObj->pInventory[target].m_Level);
			}
			if(lpObj->pInventory[target].m_Type == ITEMGET(13,77))
			{
				gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				g_Log.Add("Using transformation ring: %s : %d",lpObj->Name,lpObj->pInventory[target].m_Level);
			}
			if(lpObj->pInventory[target].m_Type == ITEMGET(13,78))
			{
				gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				g_Log.Add("Using transformation ring: %s : %d",lpObj->Name,lpObj->pInventory[target].m_Level);
			}
			if(lpObj->pInventory[target].m_Type == ITEMGET(13,122))
			{
				gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				g_Log.Add("Using transformation ring: %s : %d",lpObj->Name,lpObj->pInventory[target].m_Level);
			}
			if (lpObj->pInventory[target].m_Type == ITEMGET(13, 163))
			{
				gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				g_Log.Add("Using transformation ring: %s : %d", lpObj->Name, lpObj->pInventory[target].m_Level);
			}
			if (lpObj->pInventory[target].m_Type == ITEMGET(13, 164))
			{
				gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				g_Log.Add("Using transformation ring: %s : %d", lpObj->Name, lpObj->pInventory[target].m_Level);
			}
			if (lpObj->pInventory[target].m_Type == ITEMGET(13, 165))
			{
				gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				g_Log.Add("Using transformation ring: %s : %d", lpObj->Name, lpObj->pInventory[target].m_Level);
			}
			if (lpObj->pInventory[target].m_Type == ITEMGET(13, 268))
			{
				gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				g_Log.Add("Using transformation ring: %s : %d", lpObj->Name, lpObj->pInventory[target].m_Level);
			}
		}

		gObjMakePreviewCharSet(lpObj->m_Index);
		GSProtocol.GCEquipmentChange(lpObj->m_Index,target);
	}

	return false;
}




BYTE gObjInventoryTradeMove(LPOBJ lpObj, BYTE source, BYTE target)
{
	int h,w,iwidth,iheight,blank;
	UINT64 s_num;

	if(source > MAIN_INVENTORY_SIZE)
	{
		g_Log.Add("error : %s %d",__FILE__,__LINE__);
		return -1;
	}

	if(lpObj->pInventory[source].IsItem() == 0)
	{
		return -1;
	}

	if(lpObj->TargetNumber < 0)
	{
		return -1;
	}

	if(lpObj->m_IfState.use == 0 || lpObj->m_IfState.type != 1)
	{
		return -1;
	}

	if(gObj[lpObj->TargetNumber].m_PlayerData->bt_BotType == 1)
	{
		//check for item

	}

	if(lpObj->pInventory[source].m_Type == ITEMGET(13,20))
	{
		if(lpObj->pInventory[source].m_Level == 0 ||
			lpObj->pInventory[source].m_Level == 1)
		{
			return -1;
		}
	}

	if(lpObj->pInventory[source].m_Type == ITEMGET(13,20))
	{
		if(lpObj->pInventory[source].m_Level == 2)
		{
			return -1;
		}
	}

	if(lpObj->pInventory[source].m_Type == ITEMGET(13,107))
	{
		if(lpObj->pInventory[source].m_Level == 0 ||
			lpObj->pInventory[source].m_Level == 1)
		{
			return -1;
		}
	}

	if(lpObj->pInventory[source].m_Type == ITEMGET(13,107))
	{
		if(lpObj->pInventory[source].m_Level == 2)
		{
			return -1;
		}
	}

	if(lpObj->pInventory[source].IsPeriodItem() == TRUE)
	{
		return -1;
	}

	if(lpObj->pInventory[source].m_Type == ITEMGET(14,11))
	{
		if(lpObj->pInventory[source].m_Level == 13)
		{
			return -1;
		}
	}

	if(lpObj->pInventory[source].m_Type == ITEMGET(12,26))
	{
		if(lpObj->pInventory[source].m_Level == 1 ||
			lpObj->pInventory[source].m_Level == 2 ||
			lpObj->pInventory[source].m_Level == 3 ||
			lpObj->pInventory[source].m_Level == 4 ||
			lpObj->pInventory[source].m_Level == 5)
		{
			return -1;
		}
	}

	if(lpObj->pInventory[source].m_Type == ITEMGET(13,38))
	{
		return -1;
	}

	if(lpObj->pInventory[source].m_Type == ITEMGET(13,39))
	{
		return -1;
	}

	if (!IsTransactionItem(lpObj->pInventory[source].m_Type))
	{
		if (!(lpObj->pInventory[source].m_Type == ITEMGET(14, 21) && lpObj->pInventory[source].m_Level == 3))
		{
			return -1;
		}
	}

	lpObj->pInventory[source].GetSize((int &)iwidth,(int &)iheight);
	s_num = lpObj->pInventory[source].GetNumber();

	if(gObjCheckSerial0ItemList(&lpObj->pInventory[source]) != 0)
	{
		MsgOutput(lpObj->m_Index,Lang.GetText(0,259));
		g_Log.Add("[ANTI-HACK][Serial 0 Item] [Trade] (%s)(%s) Item(%s) Pos(%d)",lpObj->AccountID,lpObj->Name,lpObj->pInventory[source].GetName(),source);
		return -1;
	}

	if(gObjInventorySearchSerialNumber(lpObj,s_num) == 0)
	{
		return -1;
	}

	w = target % 8;
	h = target >> 3;

	if(ExtentCheck(w,h,8,4) == 0)
	{
		return -1;
	}

	if(*(BYTE*)(lpObj->TradeMap + h * 8 + w) == 255)
	{
		BYTE itembuf[13];
		blank = gObjTradeRectCheck(lpObj->m_Index,w,h,iwidth,iheight);

		if(blank == 255)
		{
			return -1;
		}

		lpObj->Trade[blank] = lpObj->pInventory[source];
		gObjInventoryDeleteItem(lpObj->m_Index,source);
		gObjTradeItemBoxSet(lpObj->m_Index,blank,iwidth,iheight,lpObj->Trade[blank].m_Type);
		ItemByteConvert(itembuf,lpObj->Trade[blank]);
		GSProtocol.GCTradeOtherAdd(lpObj->TargetNumber,blank,itembuf);
		if(gObj[lpObj->TargetNumber].m_PlayerData->ISBOT == true && g_BotSystem.GetBotType(lpObj->TargetNumber) == BOT_TRADE)
		{
			g_BotSystem.CheckAlchemist(lpObj->m_Index,gObj[lpObj->TargetNumber].m_PlayerData->wBotIndex);
		}
		/*if(lpObj->m_PlayerData->ISBOT == TRUE && g_BotSystem.GetBotType(lpObj->m_Index) == BOT_TRADE)
		{
			g_BotSystem.CheckAlchemist(gObj[aIndex].TargetNumber,aIndex);
		}*/
		g_PentagramSystem.GCTransPentagramJewelViewInfo(lpObj->m_Index, &lpObj->Trade[blank]);

		if(source < INVETORY_WEAR_SIZE || source == 236)
		{
			if(source == 10 || source == 11)
			{
				if(lpObj->Trade[blank].m_Type == ITEMGET(13,10))
				{
					gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				}
				if(lpObj->Trade[blank].m_Type == ITEMGET(13,39))
				{
					gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				}
				if(lpObj->Trade[blank].m_Type == ITEMGET(13,40))
				{
					gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				}
				if(lpObj->Trade[blank].m_Type == ITEMGET(13,41))
				{
					gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				}
				if(lpObj->Trade[blank].m_Type == ITEMGET(13,42))
				{
					gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				}
				if(lpObj->Trade[blank].m_Type == ITEMGET(13,68))
				{
					gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				}
				if(lpObj->Trade[blank].m_Type == ITEMGET(13,76))
				{
					gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				}
				if(lpObj->Trade[blank].m_Type == ITEMGET(13,77))
				{
					gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				}
				if(lpObj->Trade[blank].m_Type == ITEMGET(13,78))
				{
					gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				}
				if(lpObj->Trade[blank].m_Type == ITEMGET(13,122))
				{
					gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				}
				if (lpObj->Trade[blank].m_Type == ITEMGET(13, 163))
				{
					gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				}
				if (lpObj->Trade[blank].m_Type == ITEMGET(13, 164))
				{
					gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				}
				if (lpObj->Trade[blank].m_Type == ITEMGET(13, 165))
				{
					gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				}
				if (lpObj->Trade[blank].m_Type == ITEMGET(13, 268))
				{
					gObjUseSkill.SkillChangeUse(lpObj->m_Index);
				}
			}
			gObjMakePreviewCharSet(lpObj->m_Index);
			GSProtocol.GCEquipmentChange(lpObj->m_Index,source);
		}
		return true;
	}
	return -1;
}




BYTE gObjTradeTradeMove(LPOBJ lpObj, BYTE source, BYTE target)
{
	int h,w;
	int iwidth,iheight;
	BYTE TempTradeMap[TRADE_BOX_MAP_SIZE];
	int blank;

	if(source > TRADE_BOX_SIZE)
	{
		g_Log.Add("error : %s %d",__FILE__,__LINE__);
		return -1;
	}
	if(source == target)
	{
		g_Log.Add("error : %s %d",__FILE__,__LINE__);
		return -1;
	}
	if(lpObj->Trade[source].IsItem() == 0)
	{
		return -1;
	}
	if(lpObj->Trade[target].IsItem() == 1)
	{
		return -1;
	}
	if(lpObj->TargetNumber < 0)
	{
		return -1;
	}
	if(lpObj->m_IfState.use == 0 || lpObj->m_IfState.type != 1)
	{
		return -1;
	}

	lpObj->Trade[source].GetSize((int &)iwidth,(int &)iheight);
	memcpy(TempTradeMap,lpObj->TradeMap,TRADE_BOX_MAP_SIZE);
	gObjTradeItemBoxSet(lpObj->m_Index,source,iwidth,iheight,255);

	w = target % 8;
	h = target / 8;

	if(ExtentCheck(w,h,8,4) == 0)
	{
		memcpy(lpObj->TradeMap,TempTradeMap,TRADE_BOX_MAP_SIZE);
		return -1;
	}

	if(*(BYTE*)(lpObj->TradeMap + h * 8 + w) == 255)
	{
		BYTE itembuf[13];
		blank = gObjTradeRectCheck(lpObj->m_Index,w,h,iwidth,iheight);

		if(blank == 255)
		{
			memcpy(lpObj->TradeMap,TempTradeMap,TRADE_BOX_MAP_SIZE);
			return -1;
		}

		lpObj->Trade[blank] = lpObj->Trade[source];
		lpObj->Trade[source].Clear();
		GSProtocol.GCTradeOtherDel(lpObj->TargetNumber,source);
		gObjTradeItemBoxSet(lpObj->m_Index,blank,iwidth,iheight,lpObj->Trade[blank].m_Type);
		ItemByteConvert(itembuf,lpObj->Trade[blank]);
		GSProtocol.GCTradeOtherAdd(lpObj->TargetNumber,blank,itembuf);
		g_PentagramSystem.GCTransPentagramJewelViewInfo(lpObj->m_Index, &lpObj->Trade[blank]);
		return true;
	}

	memcpy(lpObj->TradeMap,TempTradeMap,TRADE_BOX_MAP_SIZE);
	return -1;
}




void gObjTradeCancel(int aIndex)
{
	if(ObjectMaxRange(aIndex) == 0)
	{
		g_Log.Add("error : index error %s %d", __FILE__, __LINE__);
		return;
	}
	if(gObj[aIndex].Type != OBJ_USER || gObj[aIndex].Connected != PLAYER_PLAYING)
	{
		g_Log.Add("Return, because trade canceled while game is not working [%s][%s]", gObj[aIndex].AccountID, gObj[aIndex].Name);
		return;
	}
	if(gObj[aIndex].m_IfState.use != 1)
	{
		g_Log.Add("Return, because trade canceled while it is not [in-trade1] [%s][%s]", gObj[aIndex].AccountID, gObj[aIndex].Name);
		return;
	}
	if(gObj[aIndex].m_IfState.type != 1)
	{
		g_Log.Add("Return, because trade canceled while it is not [in-trade2] [%s][%s]", gObj[aIndex].AccountID, gObj[aIndex].Name);
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	gObjInventoryRollback(aIndex);
	g_MixSystem.ChaosBoxInit(lpObj);
	g_PentagramMixSystem.PentagramMixBoxInit(lpObj);
	gObj[aIndex].TargetNumber = -1;
	gObj[aIndex].m_IfState.use = 0;
	gObj[aIndex].m_IfState.state = 0; //season4.5 add-on
	gObjCharTradeClear(&gObj[aIndex]);
	GSProtocol.GCMoneySend(aIndex,gObj[aIndex].m_PlayerData->Money);
	GCItemListSend(aIndex);
	GSProtocol.GCEventInventoryItemListSend(aIndex);
	GSProtocol.GCEquipmentSend(aIndex);
	g_PeriodItemEx.OnRequestPeriodItemList(&gObj[aIndex]);
	GSProtocol.GCMagicListMultiSend(&gObj[aIndex],0);
	gObjUseSkill.SkillChangeUse(aIndex);

	if(gObj[aIndex].m_PlayerData->GuildNumber > 0)
	{
		GSProtocol.GCGuildViewportNowPaint(aIndex,gObj[aIndex].m_PlayerData->GuildName,0,0);
		gObjNotifyUpdateUnionV1(&gObj[aIndex]);
		gObjNotifyUpdateUnionV2(&gObj[aIndex]);
	}
	g_Log.Add("Cancel trade: [%s][%s]", gObj[aIndex].AccountID,gObj[aIndex].Name);
}




void gObjTempInventoryItemBoxSet(BYTE * TempMap, int itempos, int xl, int yl, BYTE set_byte)
{
	int itemposx = (itempos - INVETORY_WEAR_SIZE) % 8;
	int itemposy = (itempos - INVETORY_WEAR_SIZE) / 8;

	for(int y = 0 ; y < yl ; y++)
	{
		for(int x = 0; x < xl; x++)
		{
			*(BYTE*)(TempMap + (itemposy +y) * 8 + (itemposx + x)) = set_byte;
		}
	}
}




BYTE gObjTempInventoryRectCheck(int aIndex, BYTE * TempMap, int sx, int sy, int width, int height)
{
	int x, y, blank = 0;

	if (sx + width > 8)
	{
		return -1;
	}
	if (sy + height > MAX_INVENTORY_H)	//24
	{
		return -2;
	}
	// ----
	bool bResult = CheckOutOfInventory(aIndex, sy, height);
	// ----
	if (!bResult)
	{
		if (bResult == -1)
		{
			return -2;
		}
		// ----
		return -1;
	}
	// ----
	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			if (InventoryExtentCheck(sx + x, sy + y, 8, MAX_INVENTORY_H) && *(BYTE*)(TempMap + (sy + y) * 8 + (sx + x)) != 255)
			{
				blank += 1;
				return -1;
			}
		}
	}
	if (blank == 0)
	{
		return sx + sy * 8 + INVETORY_WEAR_SIZE;
	}
	return -1;
}




BYTE gObjTempInventoryInsertItem(LPOBJ lpObj, CItem item, BYTE * TempMap)
{
	int w, h, iwidth, iheight;
	BYTE blank = 0;

	if (item.GetSize(iwidth, iheight) == 0)
	{
		g_Log.Add("Item does not exist [%s][%d] %d", __FILE__, __LINE__, item.m_Type);
		return -1;
	}

	for (h = 0; h < 4 * lpObj->m_PlayerData->m_InventoryExpansion + 8; h++)
	{
		for (w = 0; w < 8; w++)
		{
			if (*(BYTE*)(TempMap + h * 8 + w) == 255)
			{
				blank = gObjTempInventoryRectCheck(lpObj->m_Index, TempMap, w, h, iwidth, iheight);

				if (blank == 254)
				{
					goto GOTO_EndFunc;
				}
				if (blank != 255)
				{
					gObjTempInventoryItemBoxSet(TempMap, blank, iwidth, iheight, item.m_Type);
					return blank;
				}
			}
		}
	}
GOTO_EndFunc:
	return -1;
}




BOOL TradeItemInventoryPutTest(int aIndex)
{
	BYTE TempInventoryMap[INVENTORY_MAP_SIZE];
	BYTE TempEventInventoryMap[EVENT_INVENTORY_MAP_SIZE];

	int number,result = 1;
	number = gObj[aIndex].TargetNumber;

	if(gObjIsConnected(number) == 0)
	{
		return false;
	}

	memcpy(TempInventoryMap, gObj[aIndex].pInventoryMap, INVENTORY_MAP_SIZE);
	memcpy(TempEventInventoryMap, gObj[aIndex].pEventInventoryMap, EVENT_INVENTORY_MAP_SIZE);

	for(int n = 0; n < TRADE_BOX_SIZE; n++)
	{
		if(gObj[number].Trade[n].IsItem() == 1)
		{
			if (IsEventItem(gObj[number].Trade[n].m_Type))
			{
				if (gObjTempEventInventoryInsertItem(&gObj[aIndex], gObj[number].Trade[n], TempEventInventoryMap) == 255)
				{
					return false;
				}
			}

			else
			{
				if (gObjTempInventoryInsertItem(&gObj[aIndex], gObj[number].Trade[n], TempInventoryMap) == 255)
				{
					return false;
				}
			}

			result = 1;
		}
	}

	if(result == 1)
	{
		return true;
	}

	return false;
}




BOOL TradeitemInventoryPut(int aIndex)
{
	int number;
	BYTE insert;
	char szItemName[50] = "Item";
	BYTE ExOption[8];

	number = gObj[aIndex].TargetNumber;

	if(gObjIsConnected(number) == 0)
	{
		return false;
	}

	for(int n = 0; n < TRADE_BOX_SIZE; n++)
	{
		if(gObj[number].Trade[n].IsItem() == 1)
		{
			if (g_PentagramSystem.IsPentagramItem(&gObj[number].Trade[n]) == true)
			{
				gObj[number].Trade[n].m_Durability -= 1.0;
			}

			if (IsEventItem(gObj[number].Trade[n].m_Type))
			{
				insert = gObjEventInventoryInsertItem(aIndex, gObj[number].Trade[n]);
			}

			else
			{
				insert = gObjInventoryInsertItem(aIndex, gObj[number].Trade[n]);
			}

			if(insert != 255)
			{
				if (g_PentagramSystem.IsPentagramItem(&gObj[number].Trade[n]) == true)
				{
					int nJewelCount = 0;
					gTransPentagramJewelInfo(aIndex, insert, number, gObj[number].Trade[n], &nJewelCount);
					g_Log.Add("[Trade Item] [%s][%s] Result Trade Count - %d --> %d, JewelCount = %d",
						gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[number].Trade[n].m_Durability + 1.0, gObj[number].Trade[n].m_Durability, nJewelCount);
				}

				strcpy(szItemName,gObj[number].Trade[n].GetName());
				ItemIsBufExOption(ExOption,&gObj[number].Trade[n]);

				g_Log.Add("[%s][%s][%s] (%d,%d,%d) Trade Item  Opposite :[%s][%s][%s] (%d,%d,%d) (Item:%s/Serial:%I64d [%d][%d][%d][%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d] E:[%d]",
					gObj[number].AccountID,gObj[number].Name,gObj[number].m_PlayerData->Ip_addr,gObj[number].MapNumber,gObj[number].X,gObj[number].Y,
					gObj[aIndex].AccountID,gObj[aIndex].Name,gObj[aIndex].m_PlayerData->Ip_addr,gObj[aIndex].MapNumber,gObj[aIndex].X,gObj[aIndex].Y,
					szItemName,gObj[number].Trade[n].m_Number,gObj[number].Trade[n].m_Level,gObj[number].Trade[n].m_Option1,gObj[number].Trade[n].m_Option2,gObj[number].Trade[n].m_Option3,
					ExOption[0],ExOption[1],ExOption[2],ExOption[3],ExOption[4],ExOption[5],ExOption[6],gObj[number].Trade[n].m_SetOption,gObj[number].Trade[n].m_BonusSocketOption);

				TradeLog->Output("[%s][%s][%s] (%d,%d,%d) Trade Item  Opposite :[%s][%s][%s] (%d,%d,%d) (Item:%s/Serial:%I64d [%d][%d][%d][%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d] E:[%d]",
					gObj[number].AccountID,gObj[number].Name,gObj[number].m_PlayerData->Ip_addr,gObj[number].MapNumber,gObj[number].X,gObj[number].Y,
					gObj[aIndex].AccountID,gObj[aIndex].Name,gObj[aIndex].m_PlayerData->Ip_addr,gObj[aIndex].MapNumber,gObj[aIndex].X,gObj[aIndex].Y,
					szItemName,gObj[number].Trade[n].m_Number,gObj[number].Trade[n].m_Level,gObj[number].Trade[n].m_Option1,gObj[number].Trade[n].m_Option2,gObj[number].Trade[n].m_Option3,
					ExOption[0],ExOption[1],ExOption[2],ExOption[3],ExOption[4],ExOption[5],ExOption[6],gObj[number].Trade[n].m_SetOption,gObj[number].Trade[n].m_BonusSocketOption);

			}
			else
			{
				g_Log.Add("[%s][%s][%s] (%d,%d,%d) Trade Item  Opposite :[%s][%s][%s] (%d,%d,%d) (Item:%s/Serial:%I64d [%d][%d][%d][%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d] E:[%d]",
					gObj[number].AccountID,gObj[number].Name,gObj[number].m_PlayerData->Ip_addr,gObj[number].MapNumber,gObj[number].X,gObj[number].Y,
					gObj[aIndex].AccountID,gObj[aIndex].Name,gObj[aIndex].m_PlayerData->Ip_addr,gObj[aIndex].MapNumber,gObj[aIndex].X,gObj[aIndex].Y,
					szItemName,gObj[number].Trade[n].m_Number,gObj[number].Trade[n].m_Level,gObj[number].Trade[n].m_Option1,gObj[number].Trade[n].m_Option2,gObj[number].Trade[n].m_Option3,
					ExOption[0],ExOption[1],ExOption[2],ExOption[3],ExOption[4],ExOption[5],ExOption[6],gObj[number].Trade[n].m_SetOption,gObj[number].Trade[n].m_BonusSocketOption);
				g_Log.Add("error : Trade FAIL %s %d", __FILE__, __LINE__);

				TradeLog->Output("[%s][%s][%s] (%d,%d,%d) Trade Item  Opposite :[%s][%s][%s] (%d,%d,%d) (Item:%s/Serial:%I64d [%d][%d][%d][%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d] E:[%d]",
					gObj[number].AccountID,gObj[number].Name,gObj[number].m_PlayerData->Ip_addr,gObj[number].MapNumber,gObj[number].X,gObj[number].Y,
					gObj[aIndex].AccountID,gObj[aIndex].Name,gObj[aIndex].m_PlayerData->Ip_addr,gObj[aIndex].MapNumber,gObj[aIndex].X,gObj[aIndex].Y,
					szItemName,gObj[number].Trade[n].m_Number,gObj[number].Trade[n].m_Level,gObj[number].Trade[n].m_Option1,gObj[number].Trade[n].m_Option2,gObj[number].Trade[n].m_Option3,
					ExOption[0],ExOption[1],ExOption[2],ExOption[3],ExOption[4],ExOption[5],ExOption[6],gObj[number].Trade[n].m_SetOption,gObj[number].Trade[n].m_BonusSocketOption);
				TradeLog->Output("error : Trade FAIL %s %d", __FILE__, __LINE__);
				return false;
			}
		}
	}
	return true;
}

bool gObjTradeFullExcItemCheck(int aIndex)
{
	if(g_ConfigRead.data.common.TradeExcItemLimit == true)
		return false;

	for(int i=0;i<TRADE_BOX_SIZE;i++)
	{
		if(gObj[aIndex].Trade[i].IsItem())
		{
			if(gObj[aIndex].Trade[i].m_NewOption == 63)
				return true;
		}
	}

	return false;
}

bool gObjTradeItemSerialFFFFFFFFCheck(int aIndex)
{
	if(g_ConfigRead.data.common.BlockTradeFFFFFFFFItems == true)
		return false;

	for(int i=0;i<TRADE_BOX_SIZE;i++)
	{
		if(gObj[aIndex].Trade[i].IsItem())
		{
			if (gObj[aIndex].Trade[i].m_Number == (UINT64)-1)
				return true;
		}
	}

	return false;
}



void gObjTradeOkButton(int aIndex)
{
	BYTE result = 1;
	int number = gObj[aIndex].TargetNumber;

	if(number < 0)
	{
		return;
	}

	if(gObjIsConnected(number) == 0)
	{
		return;
	}

	if(gObj[number].TargetNumber != aIndex)
	{
		g_Log.Add("error: The opposite to trade is not me [%s][%s] Target:[%d] [%d]" ,gObj[aIndex].AccountID,gObj[aIndex].Name,gObj[number].TargetNumber,gObj[number].TargetNumber);
		return;
	}

	if(gObj[aIndex].TradeOk != 1 || gObj[number].TradeOk != 1)
	{
		return;
	}

	if(TradeItemInventoryPutTest(aIndex) == 1)
	{
		if(TradeItemInventoryPutTest(number) == 1)
		{
			result = 1;
		}
		else
		{
			result = 2;
		}
	}
	else
	{
		result = 2;
	}

	if(gObjCheckMaxZen(aIndex,gObj[number].TradeMoney) == 0)
	{
		result = 5;
	}

	if(gObjCheckMaxZen(number,gObj[aIndex].TradeMoney) == 0)
	{
		result = 5;
	}

	if(g_kJewelOfHarmonySystem.IsEnableToTrade(&gObj[number]) == 0 || g_kJewelOfHarmonySystem.IsEnableToTrade(&gObj[aIndex]) == 0)
	{
		result = 4;
		gObjTradeCancel(aIndex);
		gObjTradeCancel(number);
		GSProtocol.CGTradeResult(aIndex,result);
		GSProtocol.CGTradeResult(number,result);
		return;
	}

	if (g_PentagramSystem.IsEnableToTradePentagramItem(&gObj[number]) == 0 || g_PentagramSystem.IsEnableToTradePentagramItem(&gObj[aIndex]) == 0)
	{
		result = 6;
		gObjTradeCancel(aIndex);
		gObjTradeCancel(number);
		GSProtocol.CGTradeResult(aIndex, result);
		GSProtocol.CGTradeResult(number, result);
		return;
	}

	if (g_PentagramSystem.IsEnableTransPentagramJewelInfo(aIndex, number) == false)
	{
		result = 7;
		gObjTradeCancel(aIndex);
		gObjTradeCancel(number);
		GSProtocol.CGTradeResult(aIndex, result);
		GSProtocol.CGTradeResult(number, result);
		return;
	}

	if(gObjTradeFullExcItemCheck(number) == true || gObjTradeFullExcItemCheck(aIndex) == true)
	{
		result = 4;
		gObjTradeCancel(aIndex);
		gObjTradeCancel(number);
		GSProtocol.CGTradeResult(aIndex,result);
		GSProtocol.CGTradeResult(number,result);
		return;
	}

	if(gObjTradeItemSerialFFFFFFFFCheck(number) == true || gObjTradeItemSerialFFFFFFFFCheck(aIndex) == true)
	{
		result = 4;
		gObjTradeCancel(aIndex);
		gObjTradeCancel(number);
		GSProtocol.CGTradeResult(aIndex,result);
		GSProtocol.CGTradeResult(number,result);
		return;
	}

	if(result == 1)
	{
		TradeitemInventoryPut(aIndex);
		TradeitemInventoryPut(number);
		gObjInventoryCommit(aIndex);
		gObjInventoryCommit(number);
		gObjMakePreviewCharSet(aIndex);
		gObjMakePreviewCharSet(number);

		gObj[aIndex].m_PlayerData->Money -= gObj[aIndex].TradeMoney;
		if(gObj[aIndex].m_PlayerData->Money < 0)
		{
			gObj[aIndex].m_PlayerData->Money = 0;
		}
		gObj[number].m_PlayerData->Money -= gObj[number].TradeMoney;
		if(gObj[number].m_PlayerData->Money < 0)
		{
			gObj[number].m_PlayerData->Money = 0;
		}

		gObj[aIndex].m_PlayerData->Money += gObj[number].TradeMoney;
		gObj[number].m_PlayerData->Money += gObj[aIndex].TradeMoney;

		g_Log.Add("[%s][%s],[%s][%s] Money Trade (%d) (%d)", gObj[aIndex].AccountID,gObj[aIndex].Name,gObj[number].AccountID,gObj[number].Name,gObj[aIndex].TradeMoney,gObj[number].TradeMoney);
		GJSetCharacterInfo(&gObj[aIndex],aIndex,0);
		GJSetCharacterInfo(&gObj[number],number,0);
	}
	else
	{
		result = 2;
		gObjTradeCancel(aIndex);
		gObjTradeCancel(number);
		GSProtocol.CGTradeResult(aIndex,result);
		GSProtocol.CGTradeResult(number,result);
		return;
	}

	gObj[aIndex].TargetNumber = -1;
	gObj[aIndex].m_IfState.use = 0;
	gObj[number].TargetNumber = -1;
	gObj[number].m_IfState.use = 0;
	gObj[aIndex].TradeOk = 0;
	gObj[number].TradeOk = 0;
	gObj[aIndex].TradeMoney = 0;
	gObj[number].TradeMoney = 0;

	GSProtocol.GCMoneySend(aIndex,gObj[aIndex].m_PlayerData->Money);
	GSProtocol.GCMoneySend(number,gObj[number].m_PlayerData->Money);
	GSProtocol.CGTradeResult(aIndex,result);
	GSProtocol.CGTradeResult(number,result);

	GCItemListSend(aIndex);
	GCItemListSend(number);

	GSProtocol.GCEventInventoryItemListSend(aIndex);
	GSProtocol.GCEventInventoryItemListSend(number);

	g_PeriodItemEx.OnRequestPeriodItemList(&gObj[aIndex]);
	g_PeriodItemEx.OnRequestPeriodItemList(&gObj[number]);

	for(int n = 0; n < TRADE_BOX_SIZE; n++)
	{
		gObj[aIndex].Trade[n].Clear();
		gObj[number].Trade[n].Clear();
	}

	gObjNotifyUpdateUnionV1(&gObj[aIndex]);
	gObjNotifyUpdateUnionV2(&gObj[aIndex]);
}




BYTE LevelSmallConvert(int aIndex, int inventoryindex)
{
	int level = gObj[aIndex].pInventory[inventoryindex].m_Level;

	if(level == 15)
	{
		return 7;
	}

	if(level >= 12 && level <= 14)
	{
		return 6;
	}

	if(level == 11)
	{
		return 5;
	}

	if(level >= 9 && level <= 10)
	{
		return 4;
	}

	if(level >= 7 && level <= 8)
	{
		return 3;
	}

	if(level >= 5 && level <= 6)
	{
		return 2;
	}

	if(level >= 3 && level <= 4)
	{
		return 1;
	}
	
	return 0;
}




BYTE LevelSmallConvert(int level)
{
	if(level == 15)
	{
		return 7;
	}

	if(level >= 12 && level <= 14)
	{
		return 6;
	}

	if(level == 11)
	{
		return 5;
	}

	if(level >= 9 && level <= 10)
	{
		return 4;
	}

	if(level >= 7 && level <= 8)
	{
		return 3;
	}

	if(level >= 5 && level <= 6)
	{
		return 2;
	}

	if(level >= 3 && level <= 4)
	{
		return 1;
	}
	
	return 0;
}




void gObjAttackDamageCalc(int aIndex) 
{
	return;	// Here goes a Macro
}




void gObjMakePreviewCharSet(int aIndex)
{
	if ( !ObjectMaxRange(aIndex ))
	{
		g_Log.Add("error : %s %d", __FILE__, __LINE__);
		return;
	}

	BYTE index;
	int itemindex;
	LPOBJ lpObj = &gObj[aIndex];
	memset(lpObj->CharSet, 0, sizeof(lpObj->CharSet));

	if (g_ConfigRead.SeasonX == false)
	{
		lpObj->CharSet[CS_CLASS] = CS_SET_CLASS(lpObj->Class);

		if (lpObj->m_PlayerData->ChangeUP == 1)
		{
			lpObj->CharSet[CS_CLASS] |= CS_SET_CHANGEUP;
		}

		if (lpObj->m_PlayerData->ChangeUP == 2)
		{
			lpObj->CharSet[CS_CLASS] |= CS_SET_CHANGEUP;
			lpObj->CharSet[CS_CLASS] |= CS_SET_CHANGEUP2;
		}
	}

	else
	{
		lpObj->CharSet[CS_CLASS] = CS_SET_CLASS_SX(lpObj->Class);

		if (lpObj->m_PlayerData->ChangeUP == 1)
		{
			lpObj->CharSet[CS_CLASS] |= CS_SET_CHANGEUP_SX;
		}

		if (lpObj->m_PlayerData->ChangeUP == 2)
		{
			lpObj->CharSet[CS_CLASS] |= CS_SET_CHANGEUP_SX;
			lpObj->CharSet[CS_CLASS] |= CS_SET_CHANGEUP2_SX;
		}
	}

	// Set Class
	if (g_ConfigRead.SeasonX == false)
	{
		if ( lpObj->m_ActionNumber == 0x80 )
		{
			lpObj->CharSet[CS_CLASS] |= 2;
		}
		else if ( lpObj->m_ActionNumber == 0x81 )
		{
			lpObj->CharSet[CS_CLASS] |= 3;
		}

		else
		{
			lpObj->CharSet[CS_CLASS] = lpObj->CharSet[CS_CLASS];
		}
	}

	else
	{
		if ( lpObj->m_ActionNumber == 0x80 )
		{
			lpObj->CharSet[CS_CLASS] |= 1;
		}
		else if ( lpObj->m_ActionNumber == 0x81 )
		{
			lpObj->CharSet[CS_CLASS] |= 2;
		}

		else
		{
			lpObj->CharSet[CS_CLASS] = lpObj->CharSet[CS_CLASS];
		}
	}

	// Set Righth HAnd Item
	if ( lpObj->pInventory[0].m_Type < ITEMGET(0,0) )
	{
		lpObj->CharSet[CS_WEAPON1_DATA] |= (BYTE)DBI_GET_TYPE(-1);
		lpObj->CharSet[CS_WEAPON1_TYPE] = (BYTE)DBI_GET_INDEX(-1);
	}
	else
	{
		lpObj->CharSet[CS_WEAPON1_DATA] |= DBI_GET_TYPE(lpObj->pInventory[0].m_Type);
		lpObj->CharSet[CS_WEAPON1_TYPE] = DBI_GET_INDEX(lpObj->pInventory[0].m_Type);
	}

	// Set Left Hand
	if ( lpObj->pInventory[1].m_Type < ITEMGET(0,0) )
	{
		lpObj->CharSet[CS_WEAPON2_DATA] |= (BYTE)DBI_GET_TYPE(-1);
		lpObj->CharSet[CS_WEAPON2_TYPE] = (BYTE)DBI_GET_INDEX(-1);
	}
	else
	{
		lpObj->CharSet[CS_WEAPON2_DATA] |= DBI_GET_TYPE(lpObj->pInventory[1].m_Type);
		lpObj->CharSet[CS_WEAPON2_TYPE] = DBI_GET_INDEX(lpObj->pInventory[1].m_Type);
	}

	// Set Helmet
	if ( lpObj->pInventory[2].m_Type < ITEMGET(0,0) && lpObj->m_PlayerData->ISBOT == false)
	{
		lpObj->CharSet[13] |= CS_SET_HELMET1(-1);
		lpObj->CharSet[9]  |= CS_SET_HELMET2(-1);
		lpObj->CharSet[3]  |= CS_SET_HELMET3(-1);
	}
	else if (lpObj->m_PlayerData->ISBOT == true)
	{
		lpObj->CharSet[13] |= CS_SET_HELMET1(ITEMGET(7,59));
		lpObj->CharSet[9]  |= CS_SET_HELMET2(ITEMGET(7,59));
		lpObj->CharSet[3]  |= CS_SET_HELMET3(ITEMGET(7,59));
	}
	else
	{
		lpObj->CharSet[13] |= CS_SET_HELMET1(lpObj->pInventory[2].m_Type);
		lpObj->CharSet[9]  |= CS_SET_HELMET2(lpObj->pInventory[2].m_Type);
		lpObj->CharSet[3]  |= CS_SET_HELMET3(lpObj->pInventory[2].m_Type);
	}
	// Set Armor
	if ( lpObj->pInventory[3].m_Type < ITEMGET(0,0) && lpObj->m_PlayerData->ISBOT == false )
	{
		lpObj->CharSet[14] |= CS_SET_ARMOR1(-1);
		lpObj->CharSet[9]  |= CS_SET_ARMOR2(-1);
		lpObj->CharSet[3]  |= CS_SET_ARMOR3(-1);
	}
	else if (lpObj->m_PlayerData->ISBOT == true)
	{
		lpObj->CharSet[14] |= CS_SET_ARMOR1(ITEMGET(8,59));
		lpObj->CharSet[9]  |= CS_SET_ARMOR2(ITEMGET(8,59));
		lpObj->CharSet[3]  |= CS_SET_ARMOR3(ITEMGET(8,59));
	}
	else
	{
		lpObj->CharSet[14] |= CS_SET_ARMOR1(lpObj->pInventory[3].m_Type);
		lpObj->CharSet[9]  |= CS_SET_ARMOR2(lpObj->pInventory[3].m_Type);
		lpObj->CharSet[3]  |= CS_SET_ARMOR3(lpObj->pInventory[3].m_Type);
	}

	// Set Pants
	if ( lpObj->pInventory[4].m_Type < ITEMGET(0,0) && lpObj->m_PlayerData->ISBOT == false )
	{
		lpObj->CharSet[14] |= CS_SET_PANTS1(-1);
		lpObj->CharSet[9]  |= CS_SET_PANTS2(-1);
		lpObj->CharSet[4]  |= CS_SET_PANTS3(-1);
	}
	else if (lpObj->m_PlayerData->ISBOT == true)
	{
		lpObj->CharSet[14] |= CS_SET_PANTS1(ITEMGET(9,59));
		lpObj->CharSet[9]  |= CS_SET_PANTS2(ITEMGET(9,59));
		lpObj->CharSet[4]  |= CS_SET_PANTS3(ITEMGET(9,59));
	}
	else
	{
		lpObj->CharSet[14] |= CS_SET_PANTS1(lpObj->pInventory[4].m_Type);
		lpObj->CharSet[9]  |= CS_SET_PANTS2(lpObj->pInventory[4].m_Type);
		lpObj->CharSet[4]  |= CS_SET_PANTS3(lpObj->pInventory[4].m_Type);
	}

	// Set Gloves
	if ( lpObj->pInventory[5].m_Type < ITEMGET(0,0) && lpObj->m_PlayerData->ISBOT == false )
	{
		lpObj->CharSet[15] |= CS_SET_GLOVES1(-1);
		lpObj->CharSet[9]  |= CS_SET_GLOVES2(-1);
		lpObj->CharSet[4]  |= CS_SET_GLOVES3(-1);
	}
	else if (lpObj->m_PlayerData->ISBOT == true)
	{
		lpObj->CharSet[15] |= CS_SET_GLOVES1(ITEMGET(10,59));
		lpObj->CharSet[9]  |= CS_SET_GLOVES2(ITEMGET(10,59));
		lpObj->CharSet[4]  |= CS_SET_GLOVES3(ITEMGET(10,59));
	}
	else
	{
		lpObj->CharSet[15] |= CS_SET_GLOVES1(lpObj->pInventory[5].m_Type);
		lpObj->CharSet[9]  |= CS_SET_GLOVES2(lpObj->pInventory[5].m_Type);
		lpObj->CharSet[4]  |= CS_SET_GLOVES3(lpObj->pInventory[5].m_Type);
	}

	// Set Boots
	if ( lpObj->pInventory[6].m_Type < ITEMGET(0,0) && lpObj->m_PlayerData->ISBOT == false )
	{
		lpObj->CharSet[15] |= CS_SET_BOOTS1(-1);
		lpObj->CharSet[9]  |= CS_SET_BOOTS2(-1);
		lpObj->CharSet[5]  |= CS_SET_BOOTS3(-1);
	}
	else if (lpObj->m_PlayerData->ISBOT == true)
	{
		lpObj->CharSet[15] |= CS_SET_BOOTS1(ITEMGET(11,59));
		lpObj->CharSet[9]  |= CS_SET_BOOTS2(ITEMGET(11,59));
		lpObj->CharSet[5]  |= CS_SET_BOOTS3(ITEMGET(11,59));
	}
	else
	{
		lpObj->CharSet[15] |= CS_SET_BOOTS1(lpObj->pInventory[6].m_Type);
		lpObj->CharSet[9]  |= CS_SET_BOOTS2(lpObj->pInventory[6].m_Type);
		lpObj->CharSet[5]  |= CS_SET_BOOTS3(lpObj->pInventory[6].m_Type);
	}

	index = 0;
	
	// Set Part onf Wings
	if ( lpObj->pInventory[7].m_Type < ITEMGET(0,0) )
	{
		index = 0;
	}
		
	index |= 3;
	
	if ( lpObj->pInventory[8].m_Type > 0 )
	{
		if ( lpObj->pInventory[8].m_Type == ITEMGET(13,67) || lpObj->pInventory[8].m_Type == ITEMGET(13,123) )
		{
			index = 2;
		}
		else
		{
			index = lpObj->pInventory[8].m_Type & 3;
		}
	}

	int levelindex;
	lpObj->CharSet[5] |= index;

	if(lpObj->m_PlayerData->ISBOT == false)
	{
		levelindex = CS_SET_SMALLLEVEL_RH(LevelSmallConvert(aIndex, 0));
		levelindex |= CS_SET_SMALLLEVEL_LH(LevelSmallConvert(aIndex, 1));
		levelindex |= CS_SET_SMALLLEVEL_HELMET(LevelSmallConvert(aIndex, 2));
		levelindex |= CS_SET_SMALLLEVEL_ARMOR(LevelSmallConvert(aIndex, 3));
		levelindex |= CS_SET_SMALLLEVEL_PANTS(LevelSmallConvert(aIndex, 4));
		levelindex |= CS_SET_SMALLLEVEL_GLOVES(LevelSmallConvert(aIndex, 5));
		levelindex |= CS_SET_SMALLLEVEL_BOOTS(LevelSmallConvert(aIndex, 6));
	}

	else
	{
		levelindex = CS_SET_SMALLLEVEL_RH(LevelSmallConvert(aIndex, 0));
		levelindex |= CS_SET_SMALLLEVEL_LH(LevelSmallConvert(aIndex, 1));
		levelindex |= CS_SET_SMALLLEVEL_HELMET(7);
		levelindex |= CS_SET_SMALLLEVEL_ARMOR(7);
		levelindex |= CS_SET_SMALLLEVEL_PANTS(7);
		levelindex |= CS_SET_SMALLLEVEL_GLOVES(7);
		levelindex |= CS_SET_SMALLLEVEL_BOOTS(7);
	}

	lpObj->CharSet[6] = CS_SET_SMALLLEVEL1(levelindex);
	lpObj->CharSet[7] = CS_SET_SMALLLEVEL2(levelindex);
	lpObj->CharSet[8] = CS_SET_SMALLLEVEL3(levelindex);

	/*

	if ( (lpObj->pInventory[7].m_Type >= ITEMGET(12,3) && lpObj->pInventory[7].m_Type <= ITEMGET(12,6)) || (lpObj->pInventory[7].m_Type == ITEMGET(13,30)) || (lpObj->pInventory[7].m_Type == ITEMGET(12,41)) || (lpObj->pInventory[7].m_Type == ITEMGET(12,42))) 
	{

		lpObj->CharSet[5] |= CS_SET_WING1(-1); 
		
		if ( lpObj->pInventory[7].m_Type == ITEMGET(13,30) ) 
		{
			lpObj->CharSet[9] |= 5;
		}
		else if ( lpObj->pInventory[7].m_Type == ITEMGET(12,41) )
		{
			lpObj->CharSet[9] |= 6;
		}
		else if ( lpObj->pInventory[7].m_Type == ITEMGET(12,42) )
		{
			lpObj->CharSet[9] |= 7;
		}
		else
		{
			lpObj->CharSet[9] |= (lpObj->pInventory[7].m_Type - 2) & 0x07; 
		}

	}*/

	// Set Excellent Items
	lpObj->CharSet[10] = 0;

	if ( lpObj->pInventory[2].IsExtItem() != FALSE )
		lpObj->CharSet[10] = (char)0x80;

	if ( lpObj->pInventory[3].IsExtItem() != FALSE )
		lpObj->CharSet[10] |= 0x40;

	if ( lpObj->pInventory[4].IsExtItem() != FALSE )
		lpObj->CharSet[10] |= 0x20;

	if ( lpObj->pInventory[5].IsExtItem() != FALSE )
		lpObj->CharSet[10] |= 0x10;

	if ( lpObj->pInventory[6].IsExtItem() != FALSE )
		lpObj->CharSet[10] |= 0x8;

	if ( lpObj->pInventory[0].IsExtItem() != FALSE )
		lpObj->CharSet[10] |= 0x04;

	if ( lpObj->pInventory[1].IsExtItem() != FALSE )
		lpObj->CharSet[10] |= 0x02;

	// Set Set Items
	lpObj->CharSet[11] = 0;

	if ( lpObj->pInventory[2].IsSetItem() != FALSE )
		lpObj->CharSet[11] = (char)0x80;

	if ( lpObj->pInventory[3].IsSetItem() != FALSE )
		lpObj->CharSet[11] |= 0x40;

	if ( lpObj->pInventory[4].IsSetItem() != FALSE )
		lpObj->CharSet[11] |= 0x20;

	if ( lpObj->pInventory[5].IsSetItem() != FALSE )
		lpObj->CharSet[11] |= 0x10;

	if ( lpObj->pInventory[6].IsSetItem() != FALSE )
		lpObj->CharSet[11] |= 0x8;

	if ( lpObj->pInventory[0].IsSetItem() != FALSE )
		lpObj->CharSet[11] |= 0x04;

	if ( lpObj->pInventory[1].IsSetItem() != FALSE )
		lpObj->CharSet[11] |= 0x02;

	gObjCalCharacter.CalcCharacter(aIndex);

	if ( lpObj->m_PlayerData->IsFullSetItem != false )
		lpObj->CharSet[11] |= 0x01;

/*
13
	0	"Guardian Angel"
	1	"Imp"
	2	"Horn of Uniria"
	3	"Horn of Dinorant"
	4	"Horn of Dark Horse"
*/
	if ( (lpObj->pInventory[8].m_Type & 0x03) != 0 )
	{
		if ( lpObj->pInventory[8].m_Type > 0 )
		{
			lpObj->CharSet[10] |= 0x01;
		}
	}

	if ( lpObj->pInventory[lpObj->m_btInvenPetPos].m_Type == ITEMGET(13,4) )	// Dark Horse
	{
		lpObj->CharSet[12] |= 0x01;
	}
	
	//Hermex Fix for Charset 16 to prevent Wings Level 3 Visual Bug
	//lpObj->CharSet[16] = 0;

	/*
	
	if( lpObj->pInventory[7].m_Type >= ITEMGET(12,36) && lpObj->pInventory[7].m_Type <= ITEMGET(12,43) ) //Third Wings
	{
		lpObj->CharSet[16] |= (lpObj->pInventory[7].m_Type - 35) * 32; // * 4
	}*/

	if ( (lpObj->pInventory[10].IsItem() == TRUE && lpObj->pInventory[10].m_Type == ITEMGET(13,169) && lpObj->pInventory[10].m_Durability > 0.0) ||
		(lpObj->pInventory[11].IsItem() == TRUE && lpObj->pInventory[11].m_Type == ITEMGET(13,169) && lpObj->pInventory[11].m_Durability > 0.0))
	{
		lpObj->CharSet[12] |= 0x08;
	}

	if ( (lpObj->pInventory[10].IsItem() == TRUE && lpObj->pInventory[10].m_Type == ITEMGET(13,170) && lpObj->pInventory[10].m_Durability > 0.0) ||
		(lpObj->pInventory[11].IsItem() == TRUE && lpObj->pInventory[11].m_Type == ITEMGET(13,170) && lpObj->pInventory[11].m_Durability > 0.0))
	{
		lpObj->CharSet[12] |= 0x08;
	}

	if ( lpObj->pInventory[lpObj->m_btInvenPetPos].m_Type == ITEMGET(13,37) )	// Fenrir
	{
		lpObj->CharSet[10] &= 0xFE;
		lpObj->CharSet[12] &= 0xFE;
		lpObj->CharSet[12] |= 0x04;
		lpObj->CharSet[16] = 0;
		lpObj->CharSet[17] = 0;

		if ( lpObj->pInventory[lpObj->m_btInvenPetPos].IsFenrirIncLastAttackDamage() != FALSE )
		{
			lpObj->CharSet[16] |= 0x01; //1
		}

		if ( lpObj->pInventory[lpObj->m_btInvenPetPos].IsFenrirDecLastAttackDamage() != FALSE )
		{
			lpObj->CharSet[16] |= 0x02; //2
		}

		if ( lpObj->pInventory[lpObj->m_btInvenPetPos].IsFenrirIllusion() != FALSE )
		{
			lpObj->CharSet[17] |= 0x01; //4
		}
	}

	/*

	if((lpObj->pInventory[7].m_Type >= ITEMGET(12,36) && lpObj->pInventory[7].m_Type <= ITEMGET(12,40)) || lpObj->pInventory[7].m_Type == ITEMGET(12,43))
	{
		lpObj->CharSet[5] |= 0xC;
		lpObj->CharSet[16] |= ((lpObj->pInventory[7].m_Type - 35) & 0x07 ); // 4 *

		if(lpObj->pInventory[7].m_Type == ITEMGET(12, 43)) 
		{
			lpObj->CharSet[16] |= 0x18;
		}
	}*/

	if (g_ConfigRead.SeasonX == false)
	{
		if ((lpObj->pInventory[7].m_Type >= ITEMGET(12, 36) && lpObj->pInventory[7].m_Type <= ITEMGET(12, 40)) || lpObj->pInventory[7].m_Type == ITEMGET(12, 43) || lpObj->pInventory[7].m_Type == ITEMGET(12, 50))
		{
			lpObj->CharSet[5] |= CS_SET_WING1(-1);

			if (lpObj->pInventory[7].m_Type == ITEMGET(12, 43))
			{
				lpObj->CharSet[9] |= 6;
			}
			else if (lpObj->pInventory[7].m_Type == ITEMGET(12, 50))
			{
				lpObj->CharSet[9] |= 7;
			}
			else if (lpObj->pInventory[7].m_Type == ITEMGET(12, 51))
			{
				lpObj->CharSet[9] |= 8;
			}

			else
			{
				lpObj->CharSet[9] |= (lpObj->pInventory[7].m_Type - 35) & 7;
			}
		}
		else if ((lpObj->pInventory[7].m_Type >= ITEMGET(12, 130) && lpObj->pInventory[7].m_Type <= ITEMGET(12, 135)))
		{
			lpObj->CharSet[5] |= 12;

			lpObj->CharSet[17] |= 32 * ((lpObj->pInventory[7].m_Type + 127) & 7);
		}
		else if ((lpObj->pInventory[7].m_Type >= ITEMGET(12, 3) && lpObj->pInventory[7].m_Type <= ITEMGET(12, 6)) || lpObj->pInventory[7].m_Type == ITEMGET(13, 30) || lpObj->pInventory[7].m_Type == ITEMGET(12, 42) || lpObj->pInventory[7].m_Type == ITEMGET(12, 49))
		{
			lpObj->CharSet[5] |= 8;

			if (lpObj->pInventory[7].m_Type == ITEMGET(13, 30))
			{
				lpObj->CharSet[9] |= 5;
			}
			else if (lpObj->pInventory[7].m_Type == ITEMGET(12, 42))
			{
				lpObj->CharSet[9] |= 6;
			}
			else if (lpObj->pInventory[7].m_Type == ITEMGET(12, 49))
			{
				lpObj->CharSet[9] |= 7;
			}

			else
			{
				lpObj->CharSet[9] |= (lpObj->pInventory[7].m_Type - 2) & 7;
			}
		}
		else if ((lpObj->pInventory[7].m_Type >= ITEMGET(12, 0) && lpObj->pInventory[7].m_Type <= ITEMGET(12, 2)) || lpObj->pInventory[7].m_Type == ITEMGET(12, 41))
		{
			lpObj->CharSet[5] |= 4;

			if (lpObj->pInventory[7].m_Type == ITEMGET(12, 41))
			{
				lpObj->CharSet[9] |= 4;
			}
			else
			{
				lpObj->CharSet[9] |= (lpObj->pInventory[7].m_Type + 1) & 7;
			}
		}
		else if (lpObj->pInventory[7].m_Type >= ITEMGET(12, 51) && lpObj->pInventory[7].m_Type <= ITEMGET(12, 57))
		{
			lpObj->CharSet[5] |= CS_SET_WING1(3);
			if (lpObj->pInventory[7].m_Type == ITEMGET(12, 51))
			{
				lpObj->CharSet[16] |= 8;
			}
			else if (lpObj->pInventory[7].m_Type == ITEMGET(12, 52))
			{
				lpObj->CharSet[16] |= 9;
			}
			else if (lpObj->pInventory[7].m_Type == ITEMGET(12, 53))
			{
				lpObj->CharSet[16] |= 10;
			}
			else if (lpObj->pInventory[7].m_Type == ITEMGET(12, 54))
			{
				lpObj->CharSet[16] |= 11;
			}
			else if (lpObj->pInventory[7].m_Type == ITEMGET(12, 55))
			{
				lpObj->CharSet[16] |= 12;
			}
			else if (lpObj->pInventory[7].m_Type == ITEMGET(12, 56))
			{
				lpObj->CharSet[16] |= 13;
			}
			else if (lpObj->pInventory[7].m_Type == ITEMGET(12, 57))
			{
				lpObj->CharSet[16] |= 14;
			}
		}

		else if (lpObj->pInventory[7].m_Type >= ITEMGET(12, 262) && lpObj->pInventory[7].m_Type <= ITEMGET(12, 267))
		{
			if (lpObj->pInventory[7].m_Type == ITEMGET(12, 262))
			{
				lpObj->CharSet[16] |= 0x04;
				lpObj->CharSet[5] |= 8;
			}

			else if (lpObj->pInventory[7].m_Type == ITEMGET(12, 263))
			{
				lpObj->CharSet[16] |= 0x08;
				lpObj->CharSet[5] |= 8;
			}

			else if (lpObj->pInventory[7].m_Type == ITEMGET(12, 264))
			{
				lpObj->CharSet[16] |= 0x0C;
				lpObj->CharSet[5] |= 8;
			}

			else if (lpObj->pInventory[7].m_Type == ITEMGET(12, 265))
			{
				lpObj->CharSet[16] |= 0x10;
				lpObj->CharSet[5] |= 8;
			}

			else if (lpObj->pInventory[7].m_Type == ITEMGET(12, 266) || lpObj->pInventory[7].m_Type == ITEMGET(12, 268))
			{
				lpObj->CharSet[5] |= 4;
				lpObj->CharSet[9] |= 5;
			}

			else if (lpObj->pInventory[7].m_Type == ITEMGET(12, 267))
			{
				lpObj->CharSet[5] |= 4;
				lpObj->CharSet[9] |= 6;
			}
		}

		else if (lpObj->pInventory[7].m_Type >= ITEMGET(12, 266) && lpObj->pInventory[7].m_Type <= ITEMGET(12, 268))
		{
			lpObj->CharSet[5] |= 4;

			if (lpObj->pInventory[7].m_Type == ITEMGET(12, 266) || lpObj->pInventory[7].m_Type == ITEMGET(12, 268))
			{
				lpObj->CharSet[9] |= 5;
			}

			else if (lpObj->pInventory[7].m_Type == ITEMGET(12, 267))
			{
				lpObj->CharSet[9] |= 6;
			}
		}

		else if (lpObj->pInventory[7].m_Type == ITEMGET(12, 269) || lpObj->pInventory[7].m_Type == ITEMGET(12, 270))
		{
			lpObj->CharSet[9] |= 3;

			if (lpObj->pInventory[7].m_Type == ITEMGET(12, 269))
			{
				lpObj->CharSet[16] |= 28;
			}

			else if (lpObj->pInventory[7].m_Type == ITEMGET(12, 270))
			{
				lpObj->CharSet[16] |= 32;
			}
		}
	}

	else
	{
		WORD ItemIndex = ITEM_GET_INDEX(lpObj->pInventory[7].m_Type);

		if (ItemIndex >= 0 && ItemIndex <= 6)
		{
			lpObj->CharSet[16] |= (0x04 * (ItemIndex + 1));
		}

		else if (ItemIndex >= 36 && ItemIndex <= 43)
		{
			lpObj->CharSet[9] |= 0x01;
			lpObj->CharSet[16] |= (0x04 * (ItemIndex - 36));
		}

		else if ((ItemIndex >= 49 && ItemIndex <= 50) || (ItemIndex >= 130 && ItemIndex <= 135))
		{
			lpObj->CharSet[9] |= 0x02;

			if (ItemIndex == 49 || ItemIndex == 50)
			{
				lpObj->CharSet[16] |= (0x04 * (ItemIndex - 49));
			}

			else if (ItemIndex >= 130 && ItemIndex <= 135)
			{
				lpObj->CharSet[16] |= (0x04 * (0x02 + (ItemIndex - 130)));
			}
		}

		else if (ItemIndex >= 262 && ItemIndex <= 269)
		{
			lpObj->CharSet[9] |= 0x03;

			if (ItemIndex >= 262 && ItemIndex <= 265)
			{
				lpObj->CharSet[16] |= (0x04 * (ItemIndex - 262));
			}

			if (ItemIndex == 266 || ItemIndex == 268)
			{
				lpObj->CharSet[16] |= 0x10;
			}

			if (ItemIndex == 267)
			{
				lpObj->CharSet[16] |= 0x14;
			}

			if (ItemIndex == 269)
			{
				lpObj->CharSet[16] |= 0x1C;
			}
		}

		else if (ItemIndex == 30)
		{
			lpObj->CharSet[9] |= 0x03;
			lpObj->CharSet[16] |= 0x18;
		}

		else if (ItemIndex == 270)
		{
			lpObj->CharSet[9] |= 0x04;
		}

		else if (ItemIndex == 278)
		{
			lpObj->CharSet[9] |= 0x04;
			lpObj->CharSet[16] |= 0x04;
		}
	}

	itemindex = lpObj->pInventory[8].m_Type;

	switch ( itemindex )
	{
		case ITEMGET(13,64):
			lpObj->CharSet[16] |= 0x20;
			break;
		case ITEMGET(13,65):
			lpObj->CharSet[16] |= 0x40;
			break;
		case ITEMGET(13,67):
			lpObj->CharSet[10] |= 0x01;
			lpObj->CharSet[16] |= 0x80;
			break;
		case ITEMGET(13,80):
			lpObj->CharSet[16] |= 0xE0;
			break;
		case ITEMGET(13,106):
			lpObj->CharSet[16] |= 0xA0;
			break;
		case ITEMGET(13,123):
			lpObj->CharSet[16] |= 0x60;
			break;
		default:
			break;
	}
}

void gObjViewportPaint(HWND hWnd)
{

	int n;
	HDC hdc;
	char szTemp[256];
	int count = 0;
	int count2 = 0;
	int count3 = 0;
	int playerc = 0;
	int totalplayer = 0;

	hdc = GetDC(hWnd);

	for ( n=0;n<g_ConfigRead.server.GetObjectMax();n++)
	{
		if ( gObj[n].Live != FALSE )
		{
			if ( gObj[n].Connected != PLAYER_EMPTY )
			{
				if ( gCurPaintMapNumber == gObj[n].MapNumber )
				{
					if ( gObj[n].Type == OBJ_USER && gObj[n].m_PlayerData->ISBOT == false )
					{
						playerc++;
					}
				}
			}
		}

		if ( gObj[n].Type == OBJ_USER && gObj[n].Connected != PLAYER_EMPTY && gObj[n].m_PlayerData->ISBOT == false )
		{
			totalplayer++;
		}
		else if ( gObj[n].Connected != PLAYER_EMPTY )
		{
			count++;
		}
	}				

	if ( gCurPaintType == 1 )
	{
		RECT rect;
		int iStartX = 150;
		int iStartY = 100;
		int iWidth	= 1;
		int iHeight	= 1;
		int iMagnifying = 3;
		int x;
		int y;

		hdc = GetDC(hWnd);

		HBRUSH hCharacterBrush = CreateSolidBrush(RGB(255, 0, 0));
		HBRUSH hMonsterBrush = CreateSolidBrush(RGB(255, 128, 0));
		HBRUSH hNpcBrush = CreateSolidBrush(RGB(0, 255, 255));
		HBRUSH hItemBrush = CreateSolidBrush(RGB(0, 0, 255));
		HBRUSH hCrywolfMovePath = CreateSolidBrush(RGB(0, 255, 0));
		int iOldBkMode = SetBkMode(hdc,TRANSPARENT);
		
		// Set a Gray backgraound on non-walkable areas
		if ( MapC[gCurPaintMapNumber].m_attrbuf != NULL )
		{
			for ( y=0;y<255;y++)
			{
				for (x=0;x<255;x++)
				{
					if ( (MapC[gCurPaintMapNumber].m_attrbuf[y*256+x] & 4) ==4 || (MapC[gCurPaintMapNumber].m_attrbuf[y*256+x]&8) == 8)
					{
						rect.left = iStartX+y*iWidth*iMagnifying;
						rect.right = (iStartX+y*iWidth*iMagnifying) + (iWidth*iMagnifying);
						rect.top = iStartY+x*iHeight*iMagnifying;
						rect.bottom = (iStartY+x*iHeight*iMagnifying) + (iHeight*iMagnifying);

						FillRect(hdc, &rect, (HBRUSH)GetStockObject(LTGRAY_BRUSH));
					}
				}
			}
		}
//#error message("put else if gObjViewportPaint")
		//27F1C
		if ( TMonsterAIElement::s_MonsterAIMovePath[MAP_INDEX_CRYWOLF_FIRSTZONE].m_bDataLoad != FALSE && gCurPaintMapNumber == 34)
		{
			TMonsterAIMovePath & MovePath = TMonsterAIElement::s_MonsterAIMovePath[MAP_INDEX_CRYWOLF_FIRSTZONE];

			for ( int i=0;i<MovePath.m_iMovePathSpotCount;i++)
			{
				rect.left = iStartX+MovePath.m_MovePathInfo[i].m_iPathY*iWidth*iMagnifying;
				rect.right = (iStartX+MovePath.m_MovePathInfo[i].m_iPathY*iWidth*iMagnifying) + (iWidth*iMagnifying);
				rect.top = iStartY+MovePath.m_MovePathInfo[i].m_iPathX*iHeight*iMagnifying;
				rect.bottom = (iStartY+MovePath.m_MovePathInfo[i].m_iPathX*iHeight*iMagnifying) + (iHeight*iMagnifying);

				FillRect(hdc, &rect, hCrywolfMovePath);

				wsprintf(szTemp, "(%d,%d)", MovePath.m_MovePathInfo[i].m_iPathX, MovePath.m_MovePathInfo[i].m_iPathY);
			}
		}

		for ( n=0;n<g_ConfigRead.server.GetObjectMax();n++)
		{
			if ( gObj[n].Live != FALSE )
			{
				if ( gObj[n].Connected != PLAYER_EMPTY )
				{
					if ( gCurPaintMapNumber == gObj[n].MapNumber )
					{
						if ( gObj[n].Type == OBJ_USER )
						{
							rect.left = iStartX+gObj[n].Y*iWidth*iMagnifying;
							rect.right = (iStartX+gObj[n].Y*iWidth*iMagnifying) + ((iWidth+2)*iMagnifying);
							rect.top = iStartY+gObj[n].X*iHeight*iMagnifying;
							rect.bottom = (iStartY+gObj[n].X*iHeight*iMagnifying) + ((iHeight+2)*iMagnifying);

							FillRect(hdc, &rect, hCharacterBrush);

							wsprintf(szTemp, "%s (%d,%d)", gObj[n].Name, gObj[n].X, gObj[n].Y); 
							TextOut(hdc, rect.left, rect.bottom, szTemp, strlen(szTemp));
						}
						else if ( gObj[n].Type == OBJ_MONSTER )
						{
							rect.left = iStartX+gObj[n].Y*iWidth*iMagnifying;
							rect.right = (iStartX+gObj[n].Y*iWidth*iMagnifying) + ((iWidth)*iMagnifying);
							rect.top = iStartY+gObj[n].X*iHeight*iMagnifying;
							rect.bottom = (iStartY+gObj[n].X*iHeight*iMagnifying) + ((iHeight)*iMagnifying);

							FillRect(hdc, &rect, hMonsterBrush);

							if ( gObj[n].m_iCurrentAI != 0 )
							{
								if ( gObj[n].m_iGroupMemberGuid == 0 )
								{
									FillRect(hdc, &rect, hCharacterBrush);

									wsprintf(szTemp, "%s (%d,%d) HP(%f/%f)", gObj[n].Name, gObj[n].m_iGroupNumber, gObj[n].m_iGroupMemberGuid, gObj[n].Life, gObj[n].MaxLife );
									TextOut(hdc, rect.left, rect.bottom, szTemp, strlen(szTemp));
								}
							}
						}
					}
				}
			}
		}

		for ( int n=0;n<g_ConfigRead.server.GetObjectMaxItem();n++)
		{
			if ( MapC[gCurPaintMapNumber].m_cItem[n].IsItem() )
			{
				rect.left = iStartX+MapC[gCurPaintMapNumber].m_cItem[n].py*iWidth*iMagnifying;
				rect.right = (iStartX+MapC[gCurPaintMapNumber].m_cItem[n].py*iWidth*iMagnifying) + (iWidth*iMagnifying);
				rect.top = iStartY+MapC[gCurPaintMapNumber].m_cItem[n].px*iHeight*iMagnifying;
				rect.bottom = (iStartY+MapC[gCurPaintMapNumber].m_cItem[n].px*iHeight*iMagnifying) + (iHeight*iMagnifying);

				FillRect(hdc, &rect, hItemBrush);

				wsprintf(szTemp, "%s", MapC[gCurPaintMapNumber].m_cItem[n].GetName());
				TextOut(hdc, rect.left, rect.bottom, szTemp, strlen(szTemp));
			}
		}

		DeleteObject((HGDIOBJ)hCharacterBrush);
		DeleteObject((HGDIOBJ)hMonsterBrush);
		DeleteObject((HGDIOBJ)hNpcBrush);
		DeleteObject((HGDIOBJ)hItemBrush);
		DeleteObject((HGDIOBJ)hCrywolfMovePath);
		SetBkMode(hdc, iOldBkMode);
	}

	gObjTotalUser = totalplayer;
	gObjTotalMonster = count;

	ReleaseDC(hWnd, hdc);
}
		


static int FrustrumX[MAX_ARRAY_FRUSTRUM];
static int FrustrumY[MAX_ARRAY_FRUSTRUM];

void InitFrustrum()
{
	float CameraViewFar    = 2400.f;	// ???? ??
	float CameraViewNear   = CameraViewFar*0.19f;// ??? ??
	float CameraViewTarget = CameraViewFar*0.53f;// ??????? ??? ??g
	float WidthFar  = 1190.f;// ?????? ???? ??
	float WidthNear = 550.f;// ??????? ???? ??
		
	vec3_t p[4];
	Vector(-WidthFar ,CameraViewFar -CameraViewTarget,0.f,p[0]);
	Vector( WidthFar ,CameraViewFar -CameraViewTarget,0.f,p[1]);
	Vector( WidthNear,CameraViewNear-CameraViewTarget,0.f,p[2]);
	Vector(-WidthNear,CameraViewNear-CameraViewTarget,0.f,p[3]);
	vec3_t Angle;
	float Matrix[3][4];
	Vector(0.f,0.f,45.f,Angle);
	AngleMatrix(Angle,Matrix);
    vec3_t Frustrum[4];
	for(int i=0;i<4;i++)
	{
		VectorRotate(p[i],Matrix,Frustrum[i]);
		FrustrumX[i] = (int)(Frustrum[i][0]*0.01f);
		FrustrumY[i] = (int)(Frustrum[i][1]*0.01f);
	}
}

void InitFrustrumEx() // 1.01.10B GS KOR, changed values
{
	float CameraViewFar = 3200.f;
	float CameraViewNear = CameraViewFar*0.19f;
	float CameraViewTarget = CameraViewFar*0.53f;
	float WidthFar = 1390.f;
	float WidthNear = 750.f;

	vec3_t p[4];
	Vector(-WidthFar, CameraViewFar - CameraViewTarget, 0.f, p[0]);
	Vector(WidthFar, CameraViewFar - CameraViewTarget, 0.f, p[1]);
	Vector(WidthNear, CameraViewNear - CameraViewTarget, 0.f, p[2]);
	Vector(-WidthNear, CameraViewNear - CameraViewTarget, 0.f, p[3]);
	vec3_t Angle;
	float Matrix[3][4];
	Vector(0.f, 0.f, 45.f, Angle);
	AngleMatrix(Angle, Matrix);
	vec3_t Frustrum[4];
	for (int i = 0; i<4; i++)
	{
		VectorRotate(p[i], Matrix, Frustrum[i]);
		FrustrumX[i] = (int)(Frustrum[i][0] * 0.01f);
		FrustrumY[i] = (int)(Frustrum[i][1] * 0.01f);
	}
}




void CreateFrustrum(int x, int y, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	for ( int i=0;i<MAX_ARRAY_FRUSTRUM;i++)
	{
		lpObj->FrustrumX[i] = FrustrumX[i] + x;
		lpObj->FrustrumY[i] = FrustrumY[i] + y;
	}
}





bool TestFrustrum2(int x, int y, int aIndex)
{
	int j = 3;
	LPOBJ lpObj = &gObj[aIndex];

	for(int i = 0; i < MAX_ARRAY_FRUSTRUM;j = i,i++)
	{
		if((lpObj->FrustrumX[i]- x) * (lpObj->FrustrumY[j]-y) - (lpObj->FrustrumX[j]-x) * (lpObj->FrustrumY[i]-y) < 0.0f)
		{
			return false;
		}
	}
	return true;
}

BOOL gObjCheckViewport(int aIndex, int x, int y)
{
	LPOBJ lpObj = &gObj[aIndex];

	if (x < lpObj->X - 15 || x > lpObj->X + 15 || y < lpObj->Y - 15 || y > lpObj->Y + 15)
	{
		return false;
	}

	if (lpObj->m_bBlind == true)
	{
		if (x < lpObj->X - 2 || x > lpObj->X + 2 || y < lpObj->Y - 2 || y > lpObj->Y + 2)
		{
			return 2;
		}
	}

	int j = 3;

	for (int i = 0; i < MAX_ARRAY_FRUSTRUM; j = i, i++)
	{
		int frustrum = (lpObj->FrustrumX[i] - x) * (lpObj->FrustrumY[j] - y) - (lpObj->FrustrumX[j] - x) * (lpObj->FrustrumY[i] - y);
		if (frustrum < 0)
		{
			return false;
		}
	}
	return true;
}


void gObjViewportClose(LPOBJ lpObj)
{
	int tObjNum,i;
	int MVL = MAX_VIEWPORT;

	if(lpObj->Type == OBJ_MONSTER)
	{
		MVL = MaxViewportMonster;
	}

	if(lpObj->Connected == PLAYER_PLAYING)
	{
		for(int n = 0; n < MVL; n++)
		{
			if(lpObj->VpPlayer2[n].state != 0)
			{
				if(lpObj->VpPlayer2[n].type != 5)
				{
					tObjNum = lpObj->VpPlayer2[n].number;

					i = 0;

					while( true )
					{
						if(gObj[tObjNum].VpPlayer[i].number == lpObj->m_Index)
						{
							if(gObj[tObjNum].VpPlayer[i].type == lpObj->Type)
							{
								gObj[tObjNum].VpPlayer[i].state = 3;
								break;
							}
						}

						if(i < MVL-1)
						{
							i++;
						}
						else
						{
							break;
						}
					}

					i = 0;

					while( true )
					{
						if(gObj[tObjNum].VpPlayer2[i].number == lpObj->m_Index)
						{
							if(gObj[tObjNum].VpPlayer2[i].type == lpObj->Type)
							{
								gObj[tObjNum].VpPlayer2[i].state = 0;
								break;
							}
						}

						if(i < MVL-1)
						{
							i++;
						}
						else
						{
							break;
						}
					}
				}
			}
		}
	}
	gObjClearViewport(lpObj);
}



#define MP 1

inline void gObjViewportListCreate(short aIndex)
{
	int result;
	LPOBJ lpObj;
	int mapnum;

	if(ObjectMaxRange(aIndex) == 0)
	{
		return;
	}

	lpObj = &gObj[aIndex];

	if(lpObj->Connected != PLAYER_PLAYING)
	{
		return;
	}

	if(lpObj->RegenOk > 0)
	{
		return;
	}

	mapnum = lpObj->MapNumber;
	gItemLoop2 = 0;

	if(lpObj->Type == OBJ_USER)
	{
		MapClass * lpMap = &MapC[mapnum];

		for(int n = 0; n < g_ConfigRead.server.GetObjectMaxItem(); n++)
		{
			if(lpMap->m_cItem[n].live)
			{
				gItemLoop2++;

				if(lpMap->m_cItem[n].m_State == 1 || lpMap->m_cItem[n].m_State == 2)
				{
					if(gObjCheckViewport(aIndex,lpMap->m_cItem[n].px,lpMap->m_cItem[n].py) == 1)
					{
						result = ViewportAdd(aIndex,n,5);
					}
				}
			}
		}
	}

	if(aIndex == 0)
	{
		gItemLoopMax = gItemLoop2;
		gItemLoop = gItemLoop2;
	}

	LPOBJ lpTempObj;

	if(lpObj->Type == OBJ_USER 	)
	{

#if (MP==1)
#pragma omp parallel for
#endif
		for(int n = 0; n < g_ConfigRead.server.GetObjectMax(); n++)
		{
			lpTempObj = &gObj[n];

			if(lpTempObj->Connected == PLAYER_PLAYING && aIndex != n)
			{
				if(lpTempObj->m_State == 1 || lpTempObj->m_State == 2)
				{
					if(mapnum == lpTempObj->MapNumber)
					{
						if(gObjCheckViewport(aIndex,lpTempObj->X,lpTempObj->Y) == 1)
						{
							result = ViewportAdd(aIndex,n,lpTempObj->Type);
							result = ViewportAdd2(n,aIndex,gObj[aIndex].Type);
						}
					}
				}
			}
		}
	}
	else if(lpObj->Type == OBJ_MONSTER || lpObj->Type == OBJ_NPC)
	{

#if (MP==1)
#pragma omp parallel for
#endif
		for(int n = g_ConfigRead.server.GetObjectMaxMonster(); n < g_ConfigRead.server.GetObjectMax(); n++)
		{
			lpTempObj = &gObj[n];

			if(lpTempObj->Connected == PLAYER_PLAYING && aIndex != n)
			{
				if(lpTempObj->m_State == 1 || lpTempObj->m_State == 2)
				{
					if(mapnum == lpTempObj->MapNumber)
					{
						if(gObjCheckViewport(aIndex,lpTempObj->X,lpTempObj->Y) == 1)
						{
							result = ViewportAdd(aIndex,n,gObj[n].Type);
							result = ViewportAdd2(n,aIndex,gObj[aIndex].Type);
						}
					}
				}
			}
		}
	}
}



int  ViewportAdd(int aIndex, int aAddIndex, int aType)
{
	int blank = -1;
	int count = 0;
	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj = &gObj[aAddIndex];

	if ( lpTargetObj->Class == 523 || lpTargetObj->Class == 603 )
	{
		return -1;
	}

	int MVL = MAX_VIEWPORT;

	if(lpObj->Type == OBJ_MONSTER)
	{
		MVL = MaxViewportMonster;
	}

	for(int n = 0; n < MVL; n++)
	{
		if(lpObj->VpPlayer[n].state != 0)
		{
			if(lpObj->VpPlayer[n].number == aAddIndex)
			{
				if(lpObj->VpPlayer[n].type == aType)
				{
					return -1;
				}
			}
		}
		else
		{
			if(blank < 0)
			{
				blank = n;
			}
		}
	}

	if(blank < 0)
	{
		return -1;
	}

	lpObj->VpPlayer[blank].state = 1;
	lpObj->VpPlayer[blank].number = aAddIndex;
	lpObj->VpPlayer[blank].type = aType;
	lpObj->VPCount ++;

	if(lpObj->Type == OBJ_MONSTER || lpObj->Type == OBJ_NPC)
	{
		if(aType == 1)
		{
			if(lpObj->m_iCurrentAI != 0)
			{
				lpObj->m_Agro->SetAgro(aAddIndex,1000);
			}
		}
	}
	return blank;
}




int  ViewportAdd2(int aIndex, int aAddIndex, int aType)
{
	int blank = -1;
	int count = 0;
	LPOBJ lpObj = &gObj[aIndex];
	int MVL = MAX_VIEWPORT;

	if(lpObj->Type == OBJ_MONSTER)
	{
		MVL = MaxViewportMonster;
	}

	for(int n = 0; n < MVL; n++)
	{
		if(lpObj->VpPlayer2[n].state != 0)
		{
			if(lpObj->VpPlayer2[n].number == aAddIndex)
			{
				if(lpObj->VpPlayer2[n].type == aType)
				{
					return -1;
				}
			}
		}
		else
		{
			if(blank < 0)
			{
				blank = n;
			}
		}
	}

	if(blank < 0)
	{
		return -1;
	}

	lpObj->VpPlayer2[blank].state = 1;
	lpObj->VpPlayer2[blank].number = aAddIndex;
	lpObj->VpPlayer2[blank].type = aType;
	lpObj->VPCount2 ++;

	return blank;
}




void ViewportDel(short aIndex, int aDelIndex)
{
	if(gObjIsConnected(aIndex) == 0)
	{
		return;
	}

	if(gObjIsConnected(aDelIndex) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];
	int MVL = MAX_VIEWPORT;

	if(lpObj->Type == OBJ_MONSTER)
	{
		MVL = MaxViewportMonster;
	}

	for(int n = 0; n < MVL; n++)
	{
		if(lpObj->VpPlayer[n].state == 1 || lpObj->VpPlayer[n].state == 2)
		{
			if(lpObj->VpPlayer[n].number == aDelIndex)
			{
				lpObj->VpPlayer[n].state = 0;
				lpObj->VpPlayer[n].number = -1;
				lpObj->VPCount --;
			}
		}
	}
}




void Viewport2Del(short aIndex, int aDelIndex)
{
	if(gObjIsConnected(aIndex) == 0)
	{
		return;
	}

	if(gObjIsConnected(aDelIndex) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];
	int MVL = MAX_VIEWPORT;

	if(lpObj->Type == OBJ_MONSTER)
	{
		MVL = MaxViewportMonster;
	}

	for(int n = 0; n < MVL; n++)
	{
		if(lpObj->VpPlayer2[n].state == 1 || lpObj->VpPlayer2[n].state == 2)
		{
			if(lpObj->VpPlayer2[n].number == aDelIndex)
			{
				lpObj->VpPlayer2[n].state = 0;
				lpObj->VpPlayer2[n].number = -1;
				lpObj->VPCount2 --;
			}
		}
	}
}




void gObjViewportAllDel(short aIndex)
{
	if(gObjIsConnected(aIndex) == 0)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];
	int MVL = MAX_VIEWPORT;

	if(lpObj->Type == OBJ_MONSTER)
	{
		MVL = MaxViewportMonster;
	}

	for(int n = 0; n < MVL; n++)
	{
		if(lpObj->VpPlayer[n].state == 1 || lpObj->VpPlayer[n].state == 2)
		{
			Viewport2Del(lpObj->VpPlayer[n].number,aIndex);
		}

		if(lpObj->VpPlayer2[n].state == 1 || lpObj->VpPlayer2[n].state == 2)
		{
			ViewportDel(lpObj->VpPlayer2[n].number,aIndex);
		}
	}
}




void gObjViewportListDestroy(short aIndex)
{
	short tObjNum;
	int n;
	LPOBJ lpObj;
	int MVL;

	if(ObjectMaxRange(aIndex) == 0)
	{
		return;
	}

	if(gObj[aIndex].Connected != PLAYER_PLAYING)
	{
		return;
	}

	lpObj = &gObj[aIndex];
	MVL = MAX_VIEWPORT;

	if(lpObj->Type == OBJ_MONSTER)
	{
		MVL = MaxViewportMonster;
	}

	for(n = 0; n < MVL; n++)
	{
		int mapnum;
		if(lpObj->VpPlayer[n].state == 1 || lpObj->VpPlayer[n].state == 2)
		{
			tObjNum = lpObj->VpPlayer[n].number;

			if (ObjectMaxRange(tObjNum) == false)
			{
				continue;
			}

			switch(lpObj->VpPlayer[n].type)
			{
			case 5:
				mapnum = lpObj->MapNumber;

				if(MapC[mapnum].m_cItem[tObjNum].IsItem() == 1)
				{
					if(MapC[mapnum].m_cItem[tObjNum].m_State == 8)
					{
						lpObj->VpPlayer[n].state = 3;
					}
					else
					{
						if(gObjCheckViewport(aIndex,MapC[mapnum].m_cItem[tObjNum].px,MapC[mapnum].m_cItem[tObjNum].py) == 0)
						{
							lpObj->VpPlayer[n].state = 3;
						}
					}
				}
				else
				{
					if(MapC[mapnum].m_cItem[tObjNum].live == 0 || MapC[mapnum].m_cItem[tObjNum].Give == 0)
					{
						lpObj->VpPlayer[n].state = 3;
					}
					else
					{
						lpObj->VpPlayer[n].state = 3;
					}
				}
				break;
			default : 
				if(gObj[tObjNum].m_State == 1)
				{
					if(gObj[tObjNum].Live == 0)
					{
						lpObj->VpPlayer[n].state = 3;
					}
				}

				if(gObj[tObjNum].Connected  == PLAYER_EMPTY ||
					gObj[tObjNum].m_State == 8 ||
					gObj[tObjNum].Teleport != 0 ||
					gObj[tObjNum].m_State == 32)
				{
					lpObj->VpPlayer[n].state = 3;
				}
				else
				{
					bool bDelete = false;

					if (lpObj->m_bBlind == true)
					{
						if (gObjCheckViewport(aIndex, gObj[tObjNum].X, gObj[tObjNum].Y) != 1)
						{
							bDelete = true;
						}
					}

					else if (!gObjCheckViewport(aIndex, gObj[tObjNum].X, gObj[tObjNum].Y))
					{
						bDelete = true;
					}

					if(bDelete)
					{
						lpObj->VpPlayer[n].state = 3;

						if(lpObj->Type == OBJ_MONSTER || lpObj->Type == OBJ_NPC)
						{
							if(gObj[tObjNum].Type == OBJ_USER)
							{
								if(lpObj->m_iCurrentAI != 0)
								{
									lpObj->m_Agro->DelAgro(tObjNum);
								}
							}
						}
					}
				}
				break;
			}
		}

		if(lpObj->VpPlayer2[n].state == 1 || lpObj->VpPlayer2[n].state == 2)
		{
			tObjNum = lpObj->VpPlayer2[n].number;

			if (ObjectMaxRange(tObjNum) == false)
			{
				continue;
			}

			if(gObj[tObjNum].Connected < PLAYER_PLAYING)
			{
				lpObj->VpPlayer2[n].state = 0;
				lpObj->VpPlayer2[n].number = -1;
				lpObj->VPCount2 --;
			}
			else
			{
				if(gObjCheckViewport(tObjNum,lpObj->X,lpObj->Y) == 0)
				{
					lpObj->VpPlayer2[n].state = 0;
					lpObj->VpPlayer2[n].number = -1;
					lpObj->VPCount2 --;
				}
			}
		}
	}
}

void gObjStateSetCreate(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Connected < PLAYER_PLAYING)
	{
		return;
	}

	DWORD dwNowTick = GetTickCount();
	
	if(lpObj->DieRegen == 1)
	{
		if(dwNowTick - lpObj->RegenTime > lpObj->MaxRegenTime + 1000)
		{
			gObjClearBuffEffect(lpObj, CLEAR_TYPE_NON_PCS_ITEM_EFFECT);
			lpObj->DieRegen = 2;
			lpObj->m_State = 8;
		}
	}

	if(lpObj->Type == OBJ_USER)
	{
		if(lpObj->m_State == 8)
		{
			gObjReady4Relife(lpObj);
		}
		else
		{
			gObjCheckTimeOutValue(lpObj,(DWORD &)dwNowTick);
		}
	}

	if(lpObj->Teleport == 1)
	{
		if(lpObj->DieRegen != 0)
		{
			lpObj->Teleport = 0;
		}
		else if(GetTickCount() - lpObj->TeleportTime > 100)
		{
			lpObj->Teleport = 2;
		}
	}

	else if(lpObj->Teleport == 2)
	{
		lpObj->Teleport = 3;

		if(lpObj->Type == OBJ_USER)
		{
			lpObj->TX = lpObj->X;
			lpObj->TY = lpObj->Y;

			CreateFrustrum(lpObj->X,lpObj->Y,aIndex);
			GSProtocol.GCTeleportSend(&gObj[aIndex],0,lpObj->MapNumber,lpObj->X,lpObj->Y,lpObj->Dir);
			gObjViewportListProtocolCreate(lpObj);
			GCItemListSend(aIndex); // avoid making copy of item in inventory after teleport
				
			if (g_ConfigRead.server.GetServerType() == SERVER_CASTLE)
			{
				if(lpObj->MapNumber == MAP_INDEX_CASTLESIEGE)
				{
					if(g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
					{
						g_CastleSiege.NotifySelfCsJoinSide(aIndex);
						g_CastleSiege.NotifyCsSelfLeftTime(aIndex);
					}
				}
			}

			lpObj->Teleport = 0;
		}

		else if(lpObj->Type == OBJ_NPC && lpObj->Class == 229)
		{
			int x = lpObj->X;
			int y = lpObj->Y;
			
			lpObj->X = x;
			lpObj->Y = y;

			lpObj->TX = lpObj->X;
			lpObj->TY = lpObj->Y;

			CreateFrustrum(lpObj->X,lpObj->Y,aIndex);

			lpObj->m_State = 1;
			lpObj->PathStartEnd = 0;
		}
		else if(lpObj->Type == OBJ_MONSTER)
		{
			lpObj->TX = lpObj->X;
			lpObj->TY = lpObj->Y;

			CreateFrustrum(lpObj->X,lpObj->Y,aIndex);

			gObjViewportListCreate(lpObj->m_Index);
			gObjViewportListProtocolCreate(lpObj);

			lpObj->Teleport = 0;
			lpObj->PathStartEnd = 0;
			lpObj->PathCur = 0;
			lpObj->PathCount = 0;
		}
		else 
		{
			int x,y;

			if(lpObj->m_RecallMon >= 0)
			{
				x = lpObj->X;
				y = lpObj->Y;
			}
			else
			{
				x = lpObj->X + rand()%6 - 3;
				y = lpObj->Y + rand()%6 - 3;
			}

			lpObj->X = x;
			lpObj->Y = y;

			lpObj->TX = lpObj->X;
			lpObj->TY = lpObj->Y;

			if(lpObj->m_RecallMon >= 0)
			{
				gObjViewportListProtocolCreate(lpObj);
				lpObj->PathStartEnd = 0;
			}
		}
	}

	if(lpObj->RegenOk == 2 && lpObj->Type == OBJ_USER)
	{
		lpObj->m_State = 1;
		lpObj->RegenOk = 3;
		lpObj->X = lpObj->RegenMapX;
		lpObj->Y = lpObj->RegenMapY;
		lpObj->MapNumber = lpObj->RegenMapNumber;

		lpObj->TX = lpObj->X;
		lpObj->TY = lpObj->Y;

		CreateFrustrum(lpObj->X,lpObj->Y,aIndex);

		if(DragonEvent->GetState() > 0)
		{
			if(DragonEvent->GetMapNumber() == lpObj->MapNumber)
			{
				GSProtocol.GCMapEventStateSend(lpObj->MapNumber,1,1);
			}
		}

		if (g_ConfigRead.server.GetServerType() == SERVER_CASTLE)
		{
			if (lpObj->MapNumber == MAP_INDEX_CASTLESIEGE)
			{
				GSProtocol.GCAnsCsNotifyStart(lpObj->m_Index, CASTLESIEGECHECKSTATE(g_CastleSiege.GetCastleState()));

				if (g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
				{
					g_CastleSiege.NotifySelfCsJoinSide(lpObj->m_Index);
					g_CastleSiege.NotifyCsSelfLeftTime(lpObj->m_Index);
				}
			}
		}

		gEledoradoEvent.CheckGoldDercon(lpObj->MapNumber);

	}
}
#pragma pack (1)
struct PMSG_CHARREGEN
{
	struct PBMSG_HEAD h;
	BYTE subcode;
	BYTE MapX;
	BYTE MapY;
	BYTE MapNumber;
	BYTE Dir;
	WORD Life;
	WORD Mana;
	WORD wShield;
	WORD BP;

	BYTE ExpHHH; //
	BYTE ExpHHL; //
	BYTE ExpHLH; //
	BYTE ExpHLL; //
	BYTE ExpLHH; //
	BYTE ExpLHL; //
	BYTE ExpLLH; //
	BYTE ExpLLL; //

	DWORD Money;
	int IGCLife;
	int IGCMana;
};
#pragma pack ()

void gObjSetState()
{
	int n;
	LPOBJ lpObj;

	for(int n = 0; n < g_ConfigRead.server.GetObjectMax(); n++)
	{
		lpObj = &gObj[n];

		if(lpObj->Connected > PLAYER_LOGGED)
		{
			if(lpObj->m_State == 1)
			{
				lpObj->m_State = 2;
				if(lpObj->RegenOk == 3)
				{
					lpObj->RegenOk = 0;
				}
			}
	
			if(lpObj->Teleport == 3)
			{
				lpObj->Teleport = 0;
			}
	
			if(lpObj->DieRegen == 2)
			{
				if (lpObj->m_bGMSummon)
				{
					gObjDel(lpObj->m_Index);
					continue;
				}

				int mapnum = lpObj->MapNumber;
				int x = lpObj->X;
				int y = lpObj->Y;
	
				lpObj->m_bAttackerKilled = 0;
	
				if(DS_MAP_RANGE(lpObj->MapNumber))
				{
					if(lpObj->Type >= OBJ_MONSTER)
					{
						g_DevilSquare.gDevilSquareMonsterRegen(lpObj);
						continue;
					}
				}

				if(lpObj->MapNumber == MAP_INDEX_DEVILSQUARE_FINAL)
				{
					if(lpObj->Type >= OBJ_MONSTER)
					{
						g_DevilSquareFinal.DSFMonsterRegen(lpObj);
						continue;
					}
				}

				if(lpObj->m_iCurrentAI != 0 && lpObj->m_iRegenType != 0)
					continue;

				if ( lpObj->MapNumber == MAP_INDEX_CRYWOLF_FIRSTZONE )
				{
					if ( g_CrywolfSync.GetCrywolfState() == 3 && lpObj->Type >= OBJ_MONSTER )
						continue;
				}

				if (g_ConfigRead.server.GetServerType() != SERVER_CASTLE)
				{
					if (lpObj->MapNumber == MAP_INDEX_KANTURU_BOSS && lpObj->Type == OBJ_MONSTER)
					{
						continue;
					}
				}
	
				if(lpObj->m_iCurrentAI != 0 && lpObj->m_iRegenType != 0)
				{
					continue;
				}

				if ( lpObj->Type >= 586 && lpObj->Type <= 602 )
				{
					if ( g_ArcaBattle.IsArcaBattleServer() == TRUE )
					{
						continue;
					}
				}

				if (lpObj->Type >= 627 && lpObj->Type <= 641)
				{
					if (g_ArcaBattle.IsArcaBattleServer() == TRUE)
					{
						continue;
					}
				}

				if ( lpObj->Type >= 504 && lpObj->Type <= 521 )
				{
					continue;
				}

				if ( lpObj->Type >= 523 && lpObj->Type <= 528 )
				{
					continue;
				}

				if (g_ConfigRead.server.GetServerType() == SERVER_CASTLE)
				{
					if (lpObj->MapNumber == MAP_INDEX_CRYWOLF_FIRSTZONE)
					{
						if (g_CrywolfSync.GetCrywolfState() == 3)
						{
							if (lpObj->Type >= OBJ_MONSTER)
							{
								continue;
							}
						}
					}
				}
	
				if(lpObj->m_bIsInMonsterHerd != 0)
				{
					if(lpObj->m_lpMonsterHerd != 0)
					{
						lpObj->m_lpMonsterHerd->MonsterRegenAction(lpObj);
					}
					continue;
				}
	
				if(BC_MAP_RANGE(lpObj->MapNumber) && lpObj->Type >= OBJ_MONSTER)
				{
					int iBridgeIndex = g_BloodCastle.GetBridgeIndex(lpObj->MapNumber);

					if(lpObj->Type == OBJ_MONSTER)
					{
						if(lpObj->Class != 131 && BC_STATUE_RANGE(lpObj->Class - 132)== 0)
						{
							g_BloodCastle.ChangeMonsterState(iBridgeIndex,lpObj->m_Index);
						}

						else
						{
							gObjDel(lpObj->m_Index);
							continue;
						}
					}
				}

				if (lpObj->Class == 541 || lpObj->Class == 542)
				{
					g_DoppelGanger.DelTreasureBox(lpObj->m_Index, lpObj->Class);
					continue;
				}

				if(lpObj->m_Attribute == 60)
				{
					gObjDel(lpObj->m_Index);
					continue;
				}

				if(lpObj->Class >= 678 && lpObj->Class <= 680)
				{
					gObjDel(lpObj->m_Index);
					continue;
				}

				if (g_ConfigRead.server.GetServerType() == SERVER_CASTLE)
				{
					if (lpObj->m_Attribute == 61)
					{
						gObjDel(lpObj->m_Index);
						continue;
					}

					if(lpObj->m_Attribute == 62)
					{
						switch(lpObj->Class)
						{
							case 295:
							case 300:
							case 301:
							case 302:
							case 303:
								gObjDel(lpObj->m_Index);
								continue;
						}

						if(lpObj->m_dwLastCheckTick + 600000 < GetTickCount())
						{
							gObjDel(lpObj->m_Index);
							continue;
						}
					}
				}

				if(lpObj->Type >= OBJ_MONSTER)
				{
					if(lpObj->Class == 287 || lpObj->Class == 286)
					{
						g_CsNPC_Mercenary.DeleteMercenary(lpObj->m_Index);
						gObjDel(lpObj->m_Index);
						continue;
					}
				}

				if (g_ConfigRead.server.GetServerType() == SERVER_CASTLE)
				{
					if (lpObj->Type >= OBJ_MONSTER)
					{
						if (lpObj->Class == 278)
						{
							g_CsNPC_LifeStone.DeleteLifeStone(lpObj->m_Index);
							gObjDel(lpObj->m_Index);
							continue;
						}
					}
				}

				int State = 1;

				if(lpObj->Type >= OBJ_MONSTER)
				{
					if((lpObj->Class >= 504 && lpObj->Class <= 521) || (lpObj->Class >= 523 && lpObj->Class <= 528))
					{
						State = 0;
						lpObj->Life = lpObj->MaxLife;
						lpObj->DieRegen = 0;
					}

					if ( lpObj->Class >= 586 && lpObj->Class <= 602 && g_ArcaBattle.IsArcaBattleServer() == TRUE )
					{
						State = 0;
						lpObj->DieRegen = 0;
					}		

					if (lpObj->Class >= 627 && lpObj->Class <= 641 && g_ArcaBattle.IsArcaBattleServer() == TRUE)
					{
						State = 0;
						lpObj->DieRegen = 0;
					}

					if(lpObj->Class >= 560 && lpObj->Class <= 561)
					{
						State = 0;
						lpObj->DieRegen = 0;
					}
				}

				PMSG_CHARREGEN pMsg;

				if(State != 0)
				{
					lpObj->Life = lpObj->MaxLife + lpObj->AddLife;
					lpObj->Mana = lpObj->MaxMana + lpObj->AddMana;
					lpObj->iShield = lpObj->iMaxShield + lpObj->iAddShield;

					lpObj->Live = 1;

					gObjRemoveBuffEffect(lpObj, BUFFTYPE_POISON);
					gObjRemoveBuffEffect(lpObj, BUFFTYPE_FREEZE);
					gObjRemoveBuffEffect(lpObj, BUFFTYPE_STONE);

					lpObj->Teleport = 0;
					lpObj->m_ViewState = 0;

					for(int i = 0; i < MAX_SELF_DEFENSE; i++)
					{
						lpObj->SelfDefenseTime[i] = 0;
					}

					gObjTimeCheckSelfDefense(lpObj);

					gObjClearBuffEffect(lpObj, CLEAR_TYPE_NON_PCS_ITEM_EFFECT);				
					gObjClearViewport(lpObj);

					if(lpObj->Type == OBJ_USER)
					{
						if(g_iUseCharacterAutoRecuperationSystem != 0 && lpObj->Level <= g_iCharacterRecuperationMaxLevel)
						{
							lpObj->m_iAutoRecuperationTime = GetTickCount();
						}
						else
						{
							lpObj->m_iAutoRecuperationTime = 0;
						}

						lpObj->dwShieldAutoRefillTimer = GetTickCount();

						if (lpObj->m_PlayerData->ISBOT == false && lpObj->m_Index >= g_ConfigRead.server.GetObjectStartUserIndex())
						{
							gDarkSpirit[lpObj->m_Index - g_ConfigRead.server.GetObjectStartUserIndex()].SetMode(CDarkSpirit::ePetItem_Mode::PetItem_Mode_Normal, -1);
						}

						if(lpObj->KillerType == 3)
						{
							if(!g_NewPVP.IsDuel(*lpObj))
							{
								MapC[lpObj->MapNumber].GetRandomLengthPos(lpObj->X, lpObj->Y, 18);
							}
						}

						else if(lpObj->KillerType == 2)
						{
							int w_Type = 0;

							if(lpObj->m_PlayerData->lpGuild != 0)
							{
								if(lpObj->m_PlayerData->lpGuild->WarType == 1)
								{
									w_Type = 1;
								}
							}

							if(w_Type == 0)
							{
								MapC[lpObj->MapNumber].GetRandomLengthPos((short &)lpObj->X,(short &)lpObj->Y,18);
							}
							else if(lpObj->m_PlayerData->lpGuild->WarState)
							{
								int BattleGroundIndex = lpObj->m_PlayerData->lpGuild->BattleGroundIndex;
								gBattleGetTeamPosition(BattleGroundIndex,lpObj->m_PlayerData->lpGuild->BattleTeamCode,(short &)lpObj->X,(short &)lpObj->Y);
							}
						}

						else if(lpObj->MapNumber == 6 && lpObj->m_PlayerData->RegisterdLMS == TRUE)
						{
							int room = lpObj->m_PlayerData->RegisteredLMSRoom;

							for(int i=0;i<g_LastManStanding.m_Rooms[room].regCount;i++)
							{
								if(lpObj->m_Index == g_LastManStanding.m_Rooms[room].m_Data[i].iIndex)
								{
									int xrand = g_LastManStanding.m_Rooms[room].X2 - g_LastManStanding.m_Rooms[room].X1;
									int yrand = g_LastManStanding.m_Rooms[room].Y2 - g_LastManStanding.m_Rooms[room].Y1;
									lpObj->MapNumber = 6;
									lpObj->X = g_LastManStanding.m_Rooms[room].X1+rand()%xrand ;
									lpObj->Y = g_LastManStanding.m_Rooms[room].Y1+rand()%yrand;
									MsgOutput(lpObj->m_Index, Lang.GetText(0,413));
								}
							}
						}

						else if (g_MapAttr.CheckUserRegen(lpObj->MapNumber) == true)
						{
							
						}

						else if(lpObj->MapNumber == MAP_INDEX_RORENCIA || lpObj->MapNumber == MAP_INDEX_DUNGEON)
						{
							lpObj->MapNumber = MAP_INDEX_RORENCIA;
							MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber,(short &)lpObj->X,(short &)lpObj->Y);
						}
						else if(DS_MAP_RANGE(lpObj->MapNumber))
						{
							g_DevilSquare.DieProcDevilSquare(lpObj);
							lpObj->MapNumber = MAP_INDEX_NORIA;
							MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber,(short &)lpObj->X,(short &)lpObj->Y);
						}
						else if(lpObj->MapNumber == MAP_INDEX_ICARUS)
						{
							lpObj->MapNumber = MAP_INDEX_DEVIAS;
							MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber,(short &)lpObj->X,(short &)lpObj->Y);
						}

						else if(BC_MAP_RANGE(lpObj->MapNumber))
						{
							lpObj->MapNumber = MAP_INDEX_DEVIAS;
							MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber,(short &)lpObj->X,(short &)lpObj->Y);
						}
						
						else if(CC_MAP_RANGE(lpObj->MapNumber) && g_ConfigRead.server.GetServerType() == SERVER_BATTLECORE)
						{
							lpObj->MapNumber = MAP_INDEX_LORENMARKET;
							MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber,(short &)lpObj->X,(short &)lpObj->Y);
						}

						else if(CC_MAP_RANGE(lpObj->MapNumber))
						{
							lpObj->MapNumber = MAP_INDEX_DEVIAS;
							MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber,(short &)lpObj->X,(short &)lpObj->Y);
						}
						else if (lpObj->MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL)
						{
							lpObj->MapNumber = MAP_INDEX_LORENMARKET;
							MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, (short &)lpObj->X, (short &)lpObj->Y);
						}
						else if(KALIMA_MAP_RANGE(lpObj->MapNumber))
						{
							lpObj->MapNumber = MAP_INDEX_DEVIAS;
							MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber,(short &)lpObj->X,(short &)lpObj->Y);
						}
						else if(lpObj->MapNumber == MAP_INDEX_AIDA)
						{
							MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber,(short &)lpObj->X,(short &)lpObj->Y);
						}

						else if(lpObj->MapNumber == MAP_INDEX_BARRACKS || lpObj->MapNumber == MAP_INDEX_REFUGEE)
						{
							int mgt = 118;
							short x,y,level;
							BYTE map = lpObj->MapNumber,dir;
							int result = gGateC.GetGate(mgt,(short &)x,(short &)y,(BYTE &)map,(BYTE &)dir,(short &)level);

							if(result >= 0)
							{
								lpObj->MapNumber = map;
								lpObj->X = x;
								lpObj->Y = y;
							}
						}

						else if(lpObj->MapNumber == MAP_INDEX_BATTLESOCCER && lpObj->IsInBattleGround == false)
						{
							int mgt = 50;
							short x,y,level;
							BYTE map = lpObj->MapNumber,dir;
							int result = gGateC.GetGate(mgt,(short &)x,(short &)y,(BYTE &)map,(BYTE &)dir,(short &)level);

							if(result >= 0)
							{
								lpObj->MapNumber = map;
								lpObj->X = x;
								lpObj->Y = y;
							}
						}

						else if (g_ConfigRead.server.GetServerType() == SERVER_CASTLE && lpObj->MapNumber == MAP_INDEX_CASTLEHUNTZONE)
						{
							int mgt = 106;
							short x, y, level;
							BYTE map = lpObj->MapNumber, dir;

							int result = gGateC.GetGate(mgt, (short &)x, (short &)y, (BYTE &)map, (BYTE &)dir, (short &)level);

							if (result >= 0)
							{
								lpObj->MapNumber = map;
								lpObj->X = x;
								lpObj->Y = y;
							}
						}

						else if (g_ConfigRead.server.GetServerType() == SERVER_CASTLE && lpObj->MapNumber == MAP_INDEX_CASTLESIEGE)
						{
							int mgt;
							short x, y, level;

							if (!g_CsNPC_LifeStone.SetReSpawnUserXY(lpObj->m_Index))
							{
								mgt = 100;

								if (lpObj->m_btCsJoinSide == 1)
								{
									mgt = 105;
								}
								else
								{
									mgt = 100;
								}

								BYTE map = lpObj->MapNumber, dir;
								int result = gGateC.GetGate(mgt, (short &)x, (short &)y, (BYTE &)map, (BYTE &)dir, (short &)level);

								if (result >= 0)
								{
									lpObj->MapNumber = map;
									lpObj->X = x;
									lpObj->Y = y;
								}
							}
						}

						else if(IT_MAP_RANGE(lpObj->MapNumber))
						{
							BYTE btTeam = g_IT_Event.GetUserTeam(lpObj->MapNumber, lpObj->m_Index);

							if (btTeam == 0xFF)
							{
								lpObj->MapNumber = MAP_INDEX_LORENMARKET;
								MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, (short &)lpObj->X, (short &)lpObj->Y);
							}

							else if (btTeam == 0)
							{
								int mgt = lpObj->MapNumber + 103;
								short x, y, level;
								BYTE map = lpObj->MapNumber, dir;
								int result = gGateC.GetGate(mgt, (short &)x, (short &)y, (BYTE &)map, (BYTE &)dir, (short &)level);

								if (result >= 0)
								{
									lpObj->MapNumber = map;
									lpObj->X = x;
									lpObj->Y = y;
								}
								
								g_IT_Event.IllusionTempleUserDie(lpObj);
							}

							else if (btTeam == 1)
							{
								int mgt = lpObj->MapNumber + 109;
								short x, y, level;
								BYTE map = lpObj->MapNumber, dir;
								int result = gGateC.GetGate(mgt, (short &)x, (short &)y, (BYTE &)map, (BYTE &)dir, (short &)level);

								if (result >= 0)
								{
									lpObj->MapNumber = map;
									lpObj->X = x;
									lpObj->Y = y;
								}
								
								g_IT_Event.IllusionTempleUserDie(lpObj);
							}

							else
							{
								lpObj->MapNumber = MAP_INDEX_LORENMARKET;
								MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, (short &)lpObj->X, (short &)lpObj->Y);
							}

							if (lpObj->m_nITR_RelicsTick > 0 || lpObj->m_wITR_NpcType > 0 || lpObj->m_byITR_StoneState != 99 || lpObj->m_bITR_RegisteringRelics == true || lpObj->m_bITR_GettingRelics == true)
							{
								g_IT_Event.CancleStoneState(lpObj->m_wITR_NpcType, lpObj->m_byITR_StoneState, lpObj->MapNumber);
								lpObj->m_nITR_RelicsTick = 0;
								lpObj->m_wITR_NpcType = 0;
								lpObj->m_byITR_StoneState = 99;
								lpObj->m_bITR_RegisteringRelics = 0;
								lpObj->m_bITR_GettingRelics = 0;
							}
						}

						else if (lpObj->MapNumber == MAP_INDEX_DEVILSQUARE_FINAL)
						{
							lpObj->MapNumber = MAP_INDEX_LORENMARKET;
							MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, (short &)lpObj->X, (short &)lpObj->Y);
						}

						else if (IMPERIAL_MAP_RANGE(lpObj->MapNumber))
						{
							BYTE btMapNumber = lpObj->MapNumber, dir;
							short x,y,level;
							int result = gGateC.GetGate(22, x, y, btMapNumber, dir, level);

							if(result >= 0)
							{
								lpObj->MapNumber = btMapNumber;
								lpObj->X = x;
								lpObj->Y = y;
							}

							g_ImperialGuardian.GCMissionFailUserDie(lpObj->m_Index);
						}

						else if(lpObj->MapNumber == MAP_INDEX_ARCA_WAR || lpObj->MapNumber == MAP_INDEX_DEBENTER_ARCA_WAR)
						{
							if (g_ArcaBattle.IsArcaBattleServer() == TRUE && g_AcheronGuardianEvent.GetProcState() == 1)
							{
								_tagPeriodBuffInfo BuffInfo;
								LPPERIOD_BUFF_EFFECT_INFO lpPeriBuff = g_BuffEffectSlot.GetPeriodBuffInfo(184);

								if (!lpPeriBuff)
									return;

								if (gObjAddPeriodBuffEffect(lpObj, lpPeriBuff, lpPeriBuff->dwDuration) == TRUE)
								{
									BuffInfo.wBuffIndex = 184;
									BuffInfo.btEffectType1 = 20;
									BuffInfo.btEffectType2 = 0;
									BuffInfo.lDuration = lpPeriBuff->dwDuration;

									g_BuffEffect.RequestPeriodBuffInsert(lpObj, &BuffInfo);
									g_Log.Add("[ArcaBattle] ArcaBattle Die Penalty [%s][%s]", lpObj->AccountID, lpObj->Name);
								}
							}
                                                   
							int mgt = 426;													  
							BYTE map = lpObj->MapNumber;													  
							BYTE dir;													  
							short x,y,level;
							int result = gGateC.GetGate(mgt,(short &)x,(short &)y,(BYTE &)map,(BYTE &)dir,(short &)level);

							if ( result >= 0 )
							{
								lpObj->MapNumber = map;
								lpObj->X = x;
								lpObj->Y = y;
							}
						}
						else if(lpObj->MapNumber == MAP_INDEX_CRYWOLF_FIRSTZONE)
						{
							if (g_ConfigRead.server.GetServerType() != SERVER_CASTLE)
							{
								MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, (short &)lpObj->X, (short &)lpObj->Y);
							}

							else
							{
								if (g_Crywolf.GetCrywolfState() == 4)
								{
									int mgt = 118;
									short x, y, level;
									BYTE map = lpObj->MapNumber, dir;

									int result = gGateC.GetGate(mgt, (short &)x, (short &)y, (BYTE &)map, (BYTE &)dir, (short &)level);

									if (result >= 0)
									{
										lpObj->MapNumber = map;
										lpObj->X = x;
										lpObj->Y = y;
									}
									else //1st JMP
									{
										MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, (short &)lpObj->X, (short &)lpObj->Y);

									}
								}
								else // 2nd JMP
								{
									MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber, (short &)lpObj->X, (short &)lpObj->Y);

								}
							}
						}

						else if(g_ConfigRead.server.GetServerType() != SERVER_CASTLE && lpObj->MapNumber == MAP_INDEX_KANTURU_BOSS)
						{
							int mgt = 137;
							short x,y,level;
							BYTE map = lpObj->MapNumber,dir;
							int result = gGateC.GetGate(mgt,(short &)x,(short &)y,(BYTE &)map,(BYTE &)dir,(short &)level);
							
							if(result >= 0)
							{
								lpObj->MapNumber = map;
								lpObj->X = x;
								lpObj->Y = y;
							}
							
							g_KanturuBattleUserMng.DeleteUserData(lpObj->m_Index);
						}

						else if(lpObj->MapNumber == MAP_INDEX_KANTURU1)
						{
							int mgt = 138;
							short x,y,level;
							BYTE map = lpObj->MapNumber,dir;

							int result = gGateC.GetGate(mgt,(short &)x,(short &)y,(BYTE &)map,(BYTE &)dir,(short &)level);

							if(result >= 0)
							{
								lpObj->MapNumber = map;
								lpObj->X = x;
								lpObj->Y = y;
							}
						}
						else if(lpObj->MapNumber == MAP_INDEX_KANTURU2)
						{
							int mgt = 139;
							short x,y,level;
							BYTE map = lpObj->MapNumber,dir;

							int result = gGateC.GetGate(mgt,(short &)x,(short &)y,(BYTE &)map,(BYTE &)dir,(short &)level);

							if(result >= 0)
							{
								lpObj->MapNumber = map;
								lpObj->X = x;
								lpObj->Y = y;
							}
						}

						else if(lpObj->MapNumber == MAP_INDEX_CALMNESS)
						{
							int mgt = 273;
							short x,y,level;
							BYTE map = lpObj->MapNumber,dir;

							int result = gGateC.GetGate(mgt,(short &)x,(short &)y,(BYTE &)map,(BYTE &)dir,(short &)level);

							if(result >= 0)
							{
								lpObj->MapNumber = map;
								lpObj->X = x;
								lpObj->Y = y;
							}
						}

						else if(lpObj->MapNumber == MAP_INDEX_RAKLION)
						{
							int mgt = 287;
							short x,y,level;
							BYTE map = lpObj->MapNumber,dir;

							int result = gGateC.GetGate(mgt,(short &)x,(short &)y,(BYTE &)map,(BYTE &)dir,(short &)level);

							if(result >= 0)
							{
								lpObj->MapNumber = map;
								lpObj->X = x;
								lpObj->Y = y;
							}
						}

						else if(lpObj->MapNumber == MAP_INDEX_HATCHERY)
						{
							int mgt = 287;
							short x,y,level;
							BYTE map = lpObj->MapNumber,dir;

							int result = gGateC.GetGate(mgt,(short &)x,(short &)y,(BYTE &)map,(BYTE &)dir,(short &)level);

							if(result >= 0)
							{
								lpObj->MapNumber = map;
								lpObj->X = x;
								lpObj->Y = y;
							}
							g_RaklionBattleUserMng.DeleteUserData(lpObj->m_Index);
						}

						else if(DG_MAP_RANGE(lpObj->MapNumber))
						{
							g_Log.Add("[Doppelganger] UserDie MoveMap [%s][%s]",
								lpObj->AccountID, lpObj->Name);

							g_DoppelGanger.LeaveDoppelganger(lpObj->m_Index);

							int mgt = 267;
							short x,y,level;
							BYTE map = lpObj->MapNumber,dir;

							int result = gGateC.GetGate(mgt,(short &)x,(short &)y,(BYTE &)map,(BYTE &)dir,(short &)level);

							if(result >= 0)
							{
								lpObj->MapNumber = map;
								lpObj->X = x;
								lpObj->Y = y;
							}

							if ( g_DoppelGanger.GetDoppelgangerState() == 2 )
							{
								g_DoppelGanger.SendDoppelgangerResult(lpObj, 1);
							}
						}

						else if(lpObj->MapNumber == MAP_INDEX_VULCAN)
						{
							int mgt = 294;
							short x,y,level;
							BYTE map = lpObj->MapNumber,dir;

							int result = gGateC.GetGate(mgt,(short &)x,(short &)y,(BYTE &)map,(BYTE &)dir,(short &)level);

							if(result >= 0)
							{
								lpObj->MapNumber = map;
								lpObj->X = x;
								lpObj->Y = y;
							}
						}

						else if(lpObj->MapNumber == MAP_INDEX_KALRUTAN1 || lpObj->MapNumber == MAP_INDEX_KALRUTAN2 )
						{
							int mgt = 335;
							short x,y,level;
							BYTE map = lpObj->MapNumber,dir;

							int result = gGateC.GetGate(mgt,(short &)x,(short &)y,(BYTE &)map,(BYTE &)dir,(short &)level);

							if(result >= 0)
							{
								lpObj->MapNumber = map;
								lpObj->X = x;
								lpObj->Y = y;
							}
						}

						else if(lpObj->MapNumber == MAP_INDEX_ACHERON || lpObj->MapNumber == MAP_INDEX_DEBENTER || lpObj->MapNumber == MAP_INDEX_URUK_MOUNTAIN)
						{
							int mgt = 417;
							short x,y,level;
							BYTE map = lpObj->MapNumber,dir;

							int result = gGateC.GetGate(mgt,(short &)x,(short &)y,(BYTE &)map,(BYTE &)dir,(short &)level);

							if(result >= 0)
							{
								lpObj->MapNumber = map;
								lpObj->X = x;
								lpObj->Y = y;
							}
						}

						else
						{
							MapC[lpObj->MapNumber].GetMapPos(lpObj->MapNumber,(short &)lpObj->X,(short &)lpObj->Y);
						}

						short m_Result = g_MapServerManager.CheckMoveMapSvr(lpObj->m_Index,lpObj->MapNumber,lpObj->m_sPrevMapSvrCode);

						if (m_Result != g_ConfigRead.server.GetGameServerCode())
						{
							if (m_Result == -1)
							{
								g_Log.AddC(TColor::Red, "[MapServerMng] Map Server Move Fail : CheckMoveMapSvr() == -1 [%s][%s] (%d)", lpObj->AccountID, lpObj->Name, lpObj->m_Index);
								continue;
							}

							int _map = lpObj->MapNumber;
							int _x = lpObj->X;
							int _y = lpObj->Y;

							lpObj->MapNumber = mapnum;
							lpObj->X = x;
							lpObj->Y = y;

							GJReqMapSvrMove(lpObj->m_Index, m_Result, _map, _x, _y);
							g_Log.Add("[MapServerMng] Request to Move Map Server : (%d) - [%s][%s] (%d)", m_Result, lpObj->AccountID, lpObj->Name, lpObj->m_Index);
							continue;
						}

						lpObj->TX = lpObj->X;
						lpObj->TY = lpObj->Y;

						CreateFrustrum(lpObj->X,lpObj->Y,n);

						pMsg.h.c = 0xC3;
						pMsg.h.headcode = 0xF3;
						pMsg.h.size = sizeof(pMsg);
						pMsg.subcode = 0x04;
						pMsg.Dir = lpObj->Dir;
						pMsg.MapX = lpObj->X;
						pMsg.MapY = lpObj->Y;
						pMsg.Life = lpObj->Life;
						pMsg.Mana = lpObj->Mana;
						UINT64 Experience = lpObj->m_PlayerData->Experience;

						if(g_MasterLevelSkillTreeSystem.IsMasterLevelUser(lpObj) == TRUE)
						{
							Experience = lpObj->m_PlayerData->MasterExperience;
						}

						pMsg.ExpHHH = SET_NUMBERH(SET_NUMBERHW(HIDWORD(Experience)));
						pMsg.ExpHHL = SET_NUMBERL(SET_NUMBERHW(HIDWORD(Experience)));
						pMsg.ExpHLH = SET_NUMBERH(SET_NUMBERLW(HIDWORD(Experience)));
						pMsg.ExpHLL = SET_NUMBERL(SET_NUMBERLW(HIDWORD(Experience)));
						pMsg.ExpLHH = SET_NUMBERH(SET_NUMBERHW(LODWORD(Experience)));
						pMsg.ExpLHL = SET_NUMBERL(SET_NUMBERHW(LODWORD(Experience)));
						pMsg.ExpLLH = SET_NUMBERH(SET_NUMBERLW(LODWORD(Experience)));
						pMsg.ExpLLL = SET_NUMBERL(SET_NUMBERLW(LODWORD(Experience)));

						pMsg.MapNumber = lpObj->MapNumber;
						pMsg.Money = lpObj->m_PlayerData->Money;
						pMsg.wShield = lpObj->iShield;
						pMsg.BP = lpObj->BP;

						pMsg.IGCLife = lpObj->Life;
						pMsg.IGCMana = lpObj->Mana;

						lpObj->PathCount = 0;

						IOCP.DataSend(n,(unsigned char *)&pMsg,pMsg.h.size);

						gObjViewportListProtocolCreate(lpObj);

						g_SendNPCInfo.SendNpcCoordinate(lpObj->m_Index, lpObj->MapNumber, 0);
						g_SendNPCInfo.SendPortalCoordinate(lpObj->m_Index, lpObj->MapNumber);

						if(DragonEvent->GetState())
						{
							if(DragonEvent->GetMapNumber() == lpObj->MapNumber)
							{
								GSProtocol.GCMapEventStateSend(lpObj->MapNumber,1,1);
							}
						}

						gEledoradoEvent.CheckGoldDercon(lpObj->MapNumber);

						if (g_DevilSquareFinal.m_bEVENT_ENABLE == TRUE && lpObj->MapNumber == MAP_INDEX_LORENMARKET &&
							g_DevilSquareFinal.m_nDSF_TYPE != 0 && g_DevilSquareFinal.m_nUserCount == 0)
						{
							g_DevilSquareFinal.SendDSFResult(lpObj->m_Index);
						}

						if(lpObj->Authority == 32) //season4 add-on
						{
							gObjAddBuffEffect(lpObj, BUFFTYPE_GM_MARK, 0, 0, 0, 0, -10);
						}

						if(g_ConfigRead.server.GetServerType() == SERVER_CASTLE && lpObj->MapNumber == MAP_INDEX_CASTLESIEGE)
						{	
							GSProtocol.GCAnsCsNotifyStart(lpObj->m_Index,CASTLESIEGEREADY(g_CastleSiege.GetCastleState()));
							
							if(g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
							{
								g_CastleSiege.NotifySelfCsJoinSide(lpObj->m_Index);
								g_CastleSiege.NotifyCsSelfLeftTime(lpObj->m_Index);
							}
							
						}
					}
					else
					{
						if(gObjMonsterRegen(lpObj)==0)
						{
							continue;
						}

						CreateFrustrum(lpObj->X,lpObj->Y,n);

						if(IT_MAP_RANGE(lpObj->MapNumber) && lpObj->Class == 380)
						{
							g_Log.Add("[Illusion Temple] (%d) Status Regen OK (%d: %d/%d)",
								lpObj->MapNumber-44, lpObj->MapNumber,
								lpObj->X, lpObj->Y);
						}
					}

					lpObj->DieRegen = 0;
					lpObj->m_State = 1;
				}
			}
		}
	}

	for(int n = 0; n < MAX_NUMBER_MAP; n++)
	{
		MapC[n].StateSetDestroy();
	}
}

void gObjSecondProc()
{
	if(CharSaveLog == NULL)
	{
		CharSaveLog = new CLogToFile("CHAR_SAVE_LOG", g_ConfigRead.server.GetUserSaveLogPath(), 256);
	}

	if(gDisconnect == 1)
		return;
	
	LPOBJ lpObj;

	for(int n = 0; n < g_ConfigRead.server.GetObjectMax();n++)
	{
		lpObj = &gObj[n];

		if(!ObjectMaxRange(lpObj->m_Index))
		{
			gObjDel(n);
			continue;
		}

		if(lpObj->Connected > PLAYER_LOGGED)
		{
			if(lpObj->m_bMapSvrMoveQuit == 1)
			{
				if(GetTickCount() - lpObj->m_dwMapSvrQuitTick > 30000)
				{
					gObjDel(lpObj->m_Index);
					continue;
				}
			}

			gObjSkillUseProc(lpObj);
			gObjCheckBuffEffectList(lpObj);

			if (lpObj->Type == OBJ_USER)
			{
				g_CMuunSystem.CheckMuunItemPeriodData(lpObj);
			}

			if(lpObj->Type == OBJ_NPC && lpObj->Class == 229)
			{
				gQeustNpcTeleport.Run(n);
			}

			if (lpObj->Type == OBJ_NPC && lpObj->Class == 568)
			{
				gQuestExpNpcTeleport.Run(n);
			}

			if(lpObj->Type == OBJ_MONSTER)
			{
				if(lpObj->Class == 275)
				{
					gObjRefillMonsterHP(lpObj,giKundunRefillHPSec);
				}

				if (g_ConfigRead.server.GetServerType() == SERVER_CASTLE)
				{
					if (lpObj->Class == 283)
					{
						g_CsNPC_GuardianStatue.GuardianStatueAct(lpObj->m_Index);
						continue;
					}

					if (lpObj->Class == 278)
					{
						g_CsNPC_LifeStone.LifeStoneAct(lpObj->m_Index);
						continue;
					}
				}

				if(lpObj->Class == 285)
				{
					g_CsNPC_Guardian.GuardianAct(lpObj->m_Index);
					continue;
				}

				if (lpObj->Class == 647 || lpObj->Class == 648 || lpObj->Class == 649 || lpObj->Class == 650)
				{
					if (lpObj->Live == TRUE && GetTickCount() - lpObj->m_Disappear_NewYearMonster > 600000)
					{
						lpObj->Live = FALSE;
						lpObj->m_State = 4;
						lpObj->RegenTime = GetTickCount();
						lpObj->DieRegen = 1;
						lpObj->PathCount = 0;
						lpObj->m_Disappear_NewYearMonster = 0;
						gObjDel(lpObj->m_Index);

					}
				}

				if (lpObj->Class == 653 || lpObj->Class == 654 || lpObj->Class == 655 || lpObj->Class == 656 || lpObj->Class == 657)
				{
					if (lpObj->Live == TRUE && GetTickCount() - lpObj->m_Disappear_Monster > 1800000)
					{
						lpObj->Live = FALSE;
						lpObj->m_State = 4;
						lpObj->RegenTime = GetTickCount();
						lpObj->DieRegen = 1;
						lpObj->PathCount = 0;
						lpObj->m_Disappear_Monster = 0;

						g_Log.Add("[ColossusEvent][GoldMonsterTimeout] MonsterClass : [%d], MonsterIndex : [%d], Map[%d], X[%d], Y[%d]",
							lpObj->Class, lpObj->m_Index, lpObj->MapNumber, lpObj->X, lpObj->Y);

						gObjDel(lpObj->m_Index);
					}
				}

				if (lpObj->Class == 674 || lpObj->Class == 675 || lpObj->Class == 676 || lpObj->Class == 677)
				{
					if (lpObj->Live == TRUE && lpObj->m_Attribute == 60 && GetTickCount() - lpObj->m_Disappear_Monster > 1800000)
					{
						lpObj->Live = FALSE;
						lpObj->m_State = 4;
						lpObj->RegenTime = GetTickCount();
						lpObj->DieRegen = 1;
						lpObj->PathCount = 0;
						lpObj->m_Disappear_Monster = 0;

						g_Log.Add("[PetEvent][GoldMonsterTimeout] MonsterClass : [%d], MonsterIndex : [%d], Map[%d], X[%d], Y[%d]",
							lpObj->Class, lpObj->m_Index, lpObj->MapNumber, lpObj->X, lpObj->Y);

						gObjDel(lpObj->m_Index);
					}
				}

				if (g_ConfigRead.server.GetServerType() == SERVER_CASTLE)
				{
					if (lpObj->Class == 288)
					{
						g_CsNPC_CannonTower.CannonTowerAct(lpObj->m_Index);
						continue;
					}
					g_Crywolf.CrywolfMonsterAct(lpObj->m_Index);
				}
			}

			if(lpObj->Type == OBJ_NPC)
			{
				if (g_ConfigRead.server.GetServerType() == SERVER_CASTLE)
				{
					if ((lpObj->Class < 204) ? FALSE : (lpObj->Class > 209) ? FALSE : TRUE)
					{
						g_Crywolf.CrywolfNpcAct(lpObj->m_Index);
					}

					if (lpObj->Class == 216)
					{
						g_CsNPC_CastleCrown.CastleCrownAct(lpObj->m_Index);
						continue;
					}

					if (CASTLE_CROWNSWITCH_CLASS_RANGE(lpObj->Class))
					{
						g_CsNPC_CastleCrownSwitch.CastleCrownSwitchAct(lpObj->m_Index);
						continue;
					}
				}

				if(lpObj->Class == 221 || lpObj->Class == 222)
				{
					g_CsNPC_Weapon.WeaponAct(lpObj->m_Index);
				}
			}

			if(lpObj->Type == OBJ_USER)
			{
				gObjManaPotionFill(lpObj);
				gObjRestPotionFill(lpObj);
				
				//gObjCheckXYMapTile(lpObj,3);

				if(lpObj->ChatLimitTime > 0)
				{
					lpObj->ChatLimitTimeSec++;

					if(lpObj->ChatLimitTimeSec > 60)
					{
						lpObj->ChatLimitTimeSec = 0;
						lpObj->ChatLimitTime--;

						if(lpObj->ChatLimitTime < 1)
						{
							lpObj->ChatLimitTime = 0;
							MsgOutput(lpObj->m_Index,Lang.GetText(0,49));
						}
					}
				}
				if(lpObj->m_dwDCTimer > 0)
				{
					if((GetTickCount64() - lpObj->m_dwDCTimer) > 5000)
					{
						BYTE btKillClient[] = { 0xC1, 0x04, 0xFA, 0xA6};
						IOCP.DataSend(lpObj->m_Index, btKillClient, sizeof(btKillClient));
						g_Log.AddC(TColor::Red,"(%d)[%s][%s][%s] User did not disconnect upon request. Modified client?", lpObj->m_Index, lpObj->m_PlayerData->Ip_addr,lpObj->AccountID, lpObj->Name);
					}
				}
				if(g_iUseCharacterAutoRecuperationSystem != 0)
				{
					if(lpObj->Level <= g_iCharacterRecuperationMaxLevel)
					{
						gProcessAutoRecuperation(lpObj);
					}
				}
	
				if(lpObj->Type == OBJ_USER)
				{
					gObjShieldAutoRefill(lpObj);
				}
	
				if(lpObj->Type == OBJ_USER && lpObj->m_LastTeleportTime > 0)
				{
					lpObj->m_LastTeleportTime--;
				}

				if((GetTickCount() - lpObj->m_PlayerData->PacketTime) > 1000)
				{
					lpObj->m_PlayerData->PacketsPerSecond = 0;
					lpObj->m_PlayerData->PacketTime = GetTickCount();
				}
				
				gObjDelayLifeCheck(n);
				gObjSecondDurDown(lpObj);
	
				if(lpObj->PartyNumber >= 0)
				{
					gParty.PartyMemberLifeSend(lpObj->PartyNumber);

					if (lpObj->Type == OBJ_USER)
					{
						if (lpObj->m_PlayerData->m_bSendMemberPos)
						{
							GSProtocol.GCPartyMemberPosition(n);
						}
					}
				}
	
				if(lpObj->m_ShopTime >= 1 && lpObj->m_ShopTime < 255)
				{
					lpObj->m_ShopTime++;
				}
	
				int BattleGround = 1;
				int m_BattleTimer = gCheckBattleGroundTimer();
	
				if((lpObj->Authority & 2) == 2)
				{
					BattleGround = 0;
					GSProtocol.GCTimeViewSend(lpObj->m_Index,m_BattleTimer);
				}

				if ((lpObj->Penalty & 2) == 2)
				{

				}

				if(lpObj->MapNumber == MAP_INDEX_BATTLESOCCER && BattleGround == 1)
				{
					BattleGround = 1;
	
					if(lpObj->m_PlayerData->lpGuild != 0)
					{
						if(lpObj->m_PlayerData->lpGuild->WarState != 0)
						{
							int CheckBattleGround = gCheckBattleGround(lpObj);
	
							if(CheckBattleGround != lpObj->m_PlayerData->lpGuild->BattleGroundIndex)
							{
								if(lpObj->m_PlayerData->lpGuild->WarType == 1)
								{
									BattleGround = 0;
								}
								else
								{
									BattleGround = 0;
								}
							}
							else
							{
								BattleGround = 0;
							}
						}
					}
					else
					{
						BattleGround = 1;
					}
	
					if(BattleGround != 0)
					{
						int CheckBattleGround = gCheckBattleGround(lpObj);
		
						if(CheckBattleGround >= 0)
						{
							if (lpObj->MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL && g_ConfigRead.server.GetServerType() == SERVER_BATTLECORE)
							{
								gObjMoveGate(lpObj->m_Index, 333);
							}

							else
							{
								gObjMoveGate(lpObj->m_Index, 17);
							}
						}	
					}
				}
			}
		}

		if(lpObj->Connected >= PLAYER_LOGGED && 
				lpObj->Type == OBJ_USER &&
				lpObj->CloseCount > 0)
		{
			if(lpObj->CloseCount == 1)
			{
				if(lpObj->CloseType == 1)
				{
					if(gObjGameClose(lpObj->m_Index) == 1)
					{
						GSProtocol.GCCloseMsgSend(lpObj->m_Index,1);
						GJUpdateMatchDBUserCharacters(lpObj);
					}
				}
				else if(lpObj->CloseType == 0)
				{
					GSProtocol.GCCloseMsgSend(lpObj->m_Index,0);
					IOCP.CloseClient(lpObj->m_Index);
				}
				else if(lpObj->CloseType == 2)
				{
					GSProtocol.GCCloseMsgSend(lpObj->m_Index,2);
				}
			}
			else
			{
				char Msg[128];
				wsprintf(Msg,Lang.GetText(0,29),lpObj->CloseCount-1);
				GSProtocol.GCServerMsgStringSend(Msg,lpObj->m_Index,1);
			}
			lpObj->CloseCount--;
		}
		if(lpObj->Connected > PLAYER_LOGGED &&
			lpObj->Type == OBJ_USER && lpObj->m_PlayerData->ISBOT == false)
		{
			if(GetTickCount() - lpObj->AutoSaveTime > 600000)
			{
				GJSetCharacterInfo(lpObj,n,0);
				lpObj->AutoSaveTime = GetTickCount();
			}

			if ((lpObj->Penalty & 2) == 2 && lpObj->ChatBlockTime > 0)
			{
				time_t currtime = time(NULL);

				if (currtime > lpObj->ChatBlockTime)
				{
					lpObj->PenaltyMask &= ~1;
					lpObj->Penalty &= ~2;
					lpObj->ChatBlockTime = 0;
				}
			}

			if (lpObj->m_PlayerData->AntiHackCheckTime > 0 && GetTickCount() - lpObj->m_PlayerData->AntiHackCheckTime > 180000 && lpObj->m_bOff == false)
			{
				g_Log.AddC(TColor::Red, "[%s][%s][%s][%s] AntiHack breach -> No response",
					lpObj->AccountID, lpObj->Name, lpObj->m_PlayerData->Ip_addr, lpObj->m_PlayerData->HWID);

				AntiHackLog->Output("[%s][%s][%s][%s] AntiHack breach -> No response",
					lpObj->AccountID, lpObj->Name, lpObj->m_PlayerData->Ip_addr, lpObj->m_PlayerData->HWID);

				if (g_ConfigRead.antihack.AntiHackBreachDisconnectUser == true)
				{
					GSProtocol.GCSendDisableReconnect(lpObj->m_Index);
					//IOCP.CloseClient(lpObj->m_Index);
				}
			}

			if(g_ConfigRead.data.common.bHitHackDetection)
			{
				if (lpObj->m_PlayerData->dwLastHitHackTick > 0 && GetTickCount64() - lpObj->m_PlayerData->dwLastHitHackTick > 180000 && lpObj->m_bOff == false)
				{
					g_Log.AddC(TColor::Red, "[%s][%s][%s][%s] AntiHack breach #2 -> No response",
						lpObj->AccountID, lpObj->Name, lpObj->m_PlayerData->Ip_addr, lpObj->m_PlayerData->HWID);

					if (g_ConfigRead.antihack.AntiHackBreachDisconnectUser == true)
					{
						GSProtocol.GCSendDisableReconnect(lpObj->m_Index);
						//IOCP.CloseClient(lpObj->m_Index);
					}
				}
			}

			if(!lpObj->m_bOff)
			{
				if(lpObj->CheckSumTime > 0 && GetTickCount() - lpObj->CheckSumTime > 3000)
				{
					//IOCP.CloseClient(n);
				}

				if(lpObj->CrcCheckTime > 0 && GetTickCount() - lpObj->CrcCheckTime > 30000)
				{
					g_Log.AddC(TColor::Red, "[CRC] [%s][%s] No response - DC", lpObj->AccountID, lpObj->Name);
					GSProtocol.GCSendDisableReconnect(n);
					//IOCP.CloseClient(n);
				}

				if(lpObj->AgiCheckTime > 0 && GetTickCount() - lpObj->AgiCheckTime > 180000)
				{
					if(g_ConfigRead.antihack.EnabledAgiCheck == true)
					{
						//IOCP.CloseClient(lpObj->m_Index);
					}

					else
					{
						lpObj->AgiCheckTime = GetTickCount();
					}
				}

				gObjInterfaceTimeCheck(lpObj);
				gObjTimeCheckSelfDefense(lpObj);
			}
		}

		if(lpObj->Connected == PLAYER_CONNECTED ||
			lpObj->Connected == PLAYER_LOGGED ||
			lpObj->Connected == PLAYER_PLAYING)
		{
			if(lpObj->Type == OBJ_USER && lpObj->m_PlayerData->ISBOT == false && lpObj->m_bOff == false)
			{

				if(lpObj->Connected >= PLAYER_LOGGED )
				{
					if(GetTickCount() - lpObj->ConnectCheckTime > 60000)
					{
						IOCP.ResponErrorCloseClient(n);
						g_Log.Add("Game working response error causes conclusion [%d][%s][%s][%s]",lpObj->m_Index,lpObj->AccountID,lpObj->Name,lpObj->m_PlayerData->Ip_addr);
					}
				}
				else
				{
					if(GetTickCount() - lpObj->ConnectCheckTime > 30000)
					{
						IOCP.ResponErrorCloseClient(n);
						g_Log.Add("Response error after connection causes conclusion [%d][%s][%s][%s]",lpObj->m_Index,lpObj->AccountID,lpObj->Name,lpObj->m_PlayerData->Ip_addr);
					}
				}

			}

			if(lpObj->Connected == PLAYER_PLAYING && lpObj->Type == OBJ_USER &&
				lpObj->m_PlayerData->m_MuBotEnable == true)
			{
				if(GetTickCount() - lpObj->m_PlayerData->m_MuBotTick > 60000)
				{
					lpObj->m_PlayerData->m_MuBotPayTime++;
					lpObj->m_PlayerData->m_MuBotTotalTime++;
					lpObj->m_PlayerData->m_MuBotTick = GetTickCount();
				}

				if (lpObj->m_PlayerData->m_MuBotTotalTime >= g_ConfigRead.data.mubot.autodisabletime &&
					g_ConfigRead.data.mubot.autodisabletime > 0)
				{
					lpObj->m_PlayerData->m_MuBotEnable = false;
					GSProtocol.GCAnsMuBotUse(lpObj->m_Index, 1, lpObj->m_PlayerData->m_MuBotTotalTime, 0);
				}

				if(lpObj->m_PlayerData->m_MuBotPayTime == 5)
				{
					gObjMuBotPayForUse(lpObj);
				}
			}
		}
	}
}




void gObjManaPotionFill(LPOBJ lpObj)
{
	if(lpObj->Type != OBJ_USER)
	{
		return;
	}

	lpObj->m_ManaFillCount++;

	float tmana;
	int ChangeMana = 0;

	if(lpObj->m_ManaFillCount > 2)
	{
		lpObj->m_ManaFillCount = 0;

		int BP,AddBP;
		BYTE MapAttr;
		int rBP,CurBP;

		if(lpObj->MaxMana + lpObj->AddMana > lpObj->Mana)
		{
			float percent = 3.7f;

			if (!(BC_MAP_RANGE(lpObj->MapNumber) ||
				CC_MAP_RANGE(lpObj->MapNumber) ||
				KALIMA_MAP_RANGE(lpObj->MapNumber) ||
				DS_MAP_RANGE(lpObj->MapNumber) ||
				IT_MAP_RANGE(lpObj->MapNumber) ||
				lpObj->MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL) &&
				!(lpObj->MapNumber == MAP_INDEX_CASTLESIEGE ||
				lpObj->MapNumber == MAP_INDEX_CASTLEHUNTZONE))
			{
				int iValue = gObjGetTotalValueOfEffect(lpObj, EFFECTTYPE_RECOVERY_MANA); //season3 add-on

				if (iValue > 0)
				{
					percent += percent * iValue / 100.0;	//1.01.00 fix
				}
			}

			if (lpObj->Class == CLASS_RAGEFIGHTER)
			{
				if (percent > 0.0 && lpObj->m_PlayerData->m_MPSkillOpt.iMpsAutoRecoverMana_Monk > 0.0)
				{
					percent += lpObj->m_PlayerData->m_MPSkillOpt.iMpsAutoRecoverMana_Monk;
				}
			}

			else if (percent > 0.0 && lpObj->m_PlayerData->m_MPSkillOpt.iMpsAutoRecoverMana > 0.0)
			{
				percent += lpObj->m_PlayerData->m_MPSkillOpt.iMpsAutoRecoverMana;
			}

			tmana = lpObj->Mana;
			tmana += (lpObj->MaxMana + lpObj->AddMana)*percent / 100.0f;

			tmana += lpObj->m_PlayerData->m_JewelOfHarmonyEffect.HJOpAddRefillMP;

			if (lpObj->m_PlayerData->m_btRefillMPSocketOption > 0) //season4 add-on
			{
				tmana += lpObj->m_PlayerData->m_btRefillMPSocketOption;
			}

			if(lpObj->MaxMana + lpObj->AddMana < tmana)
			{
				tmana = lpObj->MaxMana + lpObj->AddMana;
			}

			lpObj->Mana = tmana;
			ChangeMana = 1;
		}

		BP = lpObj->MaxBP + lpObj->AddBP;

		if(lpObj->BP < BP)
		{
			MapAttr = MapC[lpObj->MapNumber].GetAttr((short &)lpObj->X,(short &)lpObj->Y);
			rBP = 2;

			if((MapAttr & 1) != 0)
			{
				rBP = 5;
			}

			if(gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_SANTA_LUCKY))
			{
				rBP = 5;
				if((MapAttr & 1) != 0)
				{
					rBP = 10;
				}
			}

			if(lpObj->Class == CLASS_KNIGHT)
			{
				AddBP = rBP +(BP * 5 / 100);
			}
			else
			{
				AddBP = rBP +(BP * 3 / 100);
			}

			if (lpObj->Class == CLASS_RAGEFIGHTER)
			{
				if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsAutoRecoverAG_Monk > 0.0)
				{
					AddBP += BP * lpObj->m_PlayerData->m_MPSkillOpt.iMpsAutoRecoverAG_Monk;
				}
			}

			else if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsAutoRecoverAG > 0.0)
			{
				AddBP += BP * lpObj->m_PlayerData->m_MPSkillOpt.iMpsAutoRecoverAG;
			}

			if(gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_SOUL_POTION) != 0)
			{
				AddBP += 8;
			}

			AddBP += lpObj->m_PlayerData->SetOpIncAGValue;

			if(AddBP < 1)
			{
				AddBP = 1;
			}

			CurBP = lpObj->BP;
			CurBP += AddBP;

			if(CurBP > BP)
			{
				CurBP = BP;
			}

			lpObj->BP = CurBP;

			ChangeMana = 1;
		}

		if(ChangeMana != 0)
		{
			GSProtocol.GCManaSend(lpObj->m_Index,lpObj->Mana,255,0,lpObj->BP);
		}
	}
}




void gObjRestPotionFill(LPOBJ lpObj)
{
	if(lpObj->Type != OBJ_USER)
	{
		return;
	}

	if(lpObj->m_Rest != 0)
	{
		float tlife;
		float tmana;
		float percent = 3.0;
		lpObj->m_LifeFillCount++;

		if(lpObj->m_LifeFillCount > 4)
		{
			lpObj->m_LifeFillCount = 0;

			tlife = lpObj->Life;

			if(lpObj->pInventory[10].IsItem() == 1)
			{
				if(lpObj->pInventory[10].m_Option3 != 0 && lpObj->pInventory[10].m_Type != ITEMGET(13,24))
				{
					percent += lpObj->pInventory[10].m_Option3;
				}
			}
			else if(lpObj->pInventory[11].IsItem() == 1)
			{
				if(lpObj->pInventory[11].m_Option3 != 0 && lpObj->pInventory[11].m_Type != ITEMGET(13,24))
				{
					percent += lpObj->pInventory[11].m_Option3;
				}
			}
			else if(lpObj->pInventory[9].IsItem() == 1)
			{
				if(lpObj->pInventory[9].m_Option3 != 0 && lpObj->pInventory[9].m_Type != ITEMGET(13,28))
				{
					percent += lpObj->pInventory[9].m_Option3;
				}
			}

			if (lpObj->Class == CLASS_RAGEFIGHTER)
			{
				if (percent > 0.0 && lpObj->m_PlayerData->m_MPSkillOpt.iMpsAutoRecoverLife_Monk > 0.0)
				{
					percent += lpObj->m_PlayerData->m_MPSkillOpt.iMpsAutoRecoverLife_Monk;
				}
			}

			else if (percent > 0.0 && lpObj->m_PlayerData->m_MPSkillOpt.iMpsAutoRecoverLife > 0.0)
			{
				percent += lpObj->m_PlayerData->m_MPSkillOpt.iMpsAutoRecoverLife;
			}

			percent += gObjGetTotalValueOfEffect(lpObj, EFFECTTYPE_RECOVERY_HP);
			percent += lpObj->AutoHPRecovery;

			tlife += (lpObj->MaxLife + lpObj->AddLife)*percent / 100.0f;
			tlife += lpObj->m_PlayerData->m_JewelOfHarmonyEffect.HJOpAddRefillHP;

			if (lpObj->m_PlayerData->m_btRefillHPSocketOption > 0) //season4 add-on
			{
				tlife += lpObj->m_PlayerData->m_btRefillHPSocketOption;
			}
			
			if((lpObj->MaxLife + lpObj->AddLife) < tlife)
			{
				tlife = lpObj->MaxLife + lpObj->AddLife;
			}

			lpObj->Life = tlife;

			GSProtocol.GCReFillSend(lpObj->m_Index, lpObj->Life, -1, 0, lpObj->iShield);
		}

		percent = 3.0f;
		percent += gObjGetTotalValueOfEffect(lpObj, EFFECTTYPE_RECOVERY_MANA);

		tmana = lpObj->Mana + lpObj->AddMana;
		tmana += (lpObj->MaxMana + lpObj->AddMana) * percent / 100.0f;
		tmana += lpObj->m_PlayerData->m_JewelOfHarmonyEffect.HJOpAddRefillMP;

		if((lpObj->MaxMana + lpObj->AddMana) < tmana)
		{
			tmana = lpObj->MaxMana + lpObj->AddMana;
		}

		lpObj->Mana = tmana;

		GSProtocol.GCManaSend(lpObj->m_Index,lpObj->Mana,255,0,lpObj->BP);
	}

	else
	{
		float percent = 0;
		float tlife;
		if(lpObj->pInventory[10].IsItem() == 1)
		{
			if(lpObj->pInventory[10].m_Option3 != 0 && lpObj->pInventory[10].m_Durability != 0.0f && lpObj->pInventory[10].m_Type != ITEMGET(13,24))
			{
				percent += lpObj->pInventory[10].m_Option3;
			}
		}

		if(lpObj->pInventory[11].IsItem() == 1)
		{
			if(lpObj->pInventory[11].m_Option3 != 0 && lpObj->pInventory[11].m_Durability != 0.0f && lpObj->pInventory[11].m_Type != ITEMGET(13,24))
			{
				percent += lpObj->pInventory[11].m_Option3;
			}
		}

		if(lpObj->pInventory[9].IsItem() == 1)
		{
			if(lpObj->pInventory[9].m_Option3 != 0 && lpObj->pInventory[9].m_Durability != 0.0f && lpObj->pInventory[9].m_Type != ITEMGET(13,28))
			{
				percent += lpObj->pInventory[9].m_Option3;
			}
		}

		if(lpObj->pInventory[7].IsItem() == 1)
		{
			if(lpObj->pInventory[7].m_Option3 != 0 && lpObj->pInventory[7].m_Special[2] == 85 && lpObj->pInventory[7].m_Durability != 0.0f)
			{
				percent += lpObj->pInventory[7].m_Option3;
			}
		}

		if (!(BC_MAP_RANGE(lpObj->MapNumber) ||
			CC_MAP_RANGE(lpObj->MapNumber) ||
			KALIMA_MAP_RANGE(lpObj->MapNumber) ||
			DS_MAP_RANGE(lpObj->MapNumber) ||
			IT_MAP_RANGE(lpObj->MapNumber) ||
			lpObj->MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL) &&
			!(lpObj->MapNumber == MAP_INDEX_CASTLESIEGE ||
			lpObj->MapNumber == MAP_INDEX_CASTLEHUNTZONE))
		{
			int iValue = gObjGetTotalValueOfEffect(lpObj, EFFECTTYPE_RECOVERY_HP);

			if (iValue > 0)
			{
				percent += iValue;
			}
		}

		if (lpObj->Class == CLASS_RAGEFIGHTER)
		{
			if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsAutoRecoverLife_Monk > 0.0)
			{
				percent += lpObj->m_PlayerData->m_MPSkillOpt.iMpsAutoRecoverLife_Monk;
			}
		}

		else if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsAutoRecoverLife > 0.0)
		{
			percent += lpObj->m_PlayerData->m_MPSkillOpt.iMpsAutoRecoverLife;
		}

		percent += lpObj->AutoHPRecovery;

		if(percent != 0.0)
		{
			lpObj->m_LifeFillCount++;

			if(lpObj->m_LifeFillCount > 6)
			{
				lpObj->m_LifeFillCount = 0;

				tlife = lpObj->Life;

				tlife += (lpObj->MaxLife +lpObj->AddLife) * percent / 100.f;
				tlife += lpObj->m_PlayerData->m_JewelOfHarmonyEffect.HJOpAddRefillHP;

				if (lpObj->m_PlayerData->m_btRefillHPSocketOption > 0) //season4 add-on
				{
					tlife += lpObj->m_PlayerData->m_btRefillHPSocketOption;
				}

				if((lpObj->MaxLife + lpObj->AddLife) < tlife)
				{
					tlife = lpObj->MaxLife + lpObj->AddLife;
				}

				lpObj->Life = tlife;

				GSProtocol.GCReFillSend(lpObj->m_Index, lpObj->Life, -1, 0, lpObj->iShield);
			}
		}
	}

	if(lpObj->iFillShield > 0)
	{
		int iFillSDPoint = 0;
		if(lpObj->iFillShieldCount <= 0)
		{
			lpObj->iFillShieldCount = 1;
		}

		iFillSDPoint = lpObj->iFillShieldMax / lpObj->iFillShieldCount;

		lpObj->iShield += iFillSDPoint;

		lpObj->iFillShield -= iFillSDPoint;

		if(lpObj->iFillShield < iFillSDPoint)
		{
			iFillSDPoint = lpObj->iFillShield;
			lpObj->iFillShield = 0;

			if(iFillSDPoint < 0)
			{
				iFillSDPoint = 0;
			}
		}

		if(lpObj->iShield > (lpObj->iMaxShield + lpObj->iAddShield))
		{
			lpObj->iShield = lpObj->iMaxShield + lpObj->iAddShield;

			lpObj->iFillShield = 0;
			lpObj->iFillShieldMax = 0;
		}

		GSProtocol.GCReFillSend(lpObj->m_Index, lpObj->Life, -1, 0, lpObj->iShield);
	}

	if(lpObj->FillLife > 0)
	{
		float cur = lpObj->FillLifeMax * 34.0f / 100.0f;

		if(lpObj->FillLifeCount == 2)
		{
			cur = lpObj->FillLifeMax * 54.0f / 100.0f;
		}

		if(lpObj->FillLifeCount == 4)
		{
			cur = lpObj->FillLifeMax * 25.0f / 100.0f;
		}

		if(lpObj->FillLife < cur)
		{
			cur = lpObj->FillLife;
			lpObj->FillLife = 0;

			if(cur < 0)
			{
				cur = 0;
			}
		}
		else
		{
			lpObj->FillLife -= cur;
		}

		lpObj->Life += cur;

		if((lpObj->MaxLife + lpObj->AddLife) < lpObj->Life)
		{
			lpObj->Life = lpObj->MaxLife + lpObj->AddLife;
			lpObj->FillLife = 0;
		}

		GSProtocol.GCReFillSend(lpObj->m_Index, lpObj->Life, -1, 0, lpObj->iShield);
	}
}




void gObjUseDrink(LPOBJ lpObj, int level)
{
	int time;

	if ( level == 2 )
	{
		time = 90;
		GSProtocol.GCItemUseSpecialTimeSend(lpObj->m_Index, 1, time);
		lpObj->m_SkillInfo.RemedyOfLoveEffect = 1;
		lpObj->m_SkillInfo.RemedyOfLoveTime = 90;
	}
	else if ( level == 1 )
	{
		time = 180;
		GSProtocol.GCItemUseSpecialTimeSend(lpObj->m_Index, 0, time);
	}
	else if ( level == 0x4D )
	{
		time = 60;
		GSProtocol.GCItemUseSpecialTimeSend(lpObj->m_Index, 2, time);
	}
	else
	{
		time = 80;
		GSProtocol.GCItemUseSpecialTimeSend(lpObj->m_Index, 0, time);
	}
}

void gObjViewportListProtocolDestroy(LPOBJ lpObj)
{
	BYTE sendBuf[255];
	int lOfs, count;
	count = 0;
	lOfs = 4;

	PBMSG_COUNT pCount;
	PBMSG_COUNT2 pCount2;

	pViewportDestroy.NumberH = SET_NUMBERH(lpObj->m_Index);
	pViewportDestroy.NumberL = SET_NUMBERL(lpObj->m_Index);

	memcpy(&sendBuf[lOfs],&pViewportDestroy,2);

	lOfs += 2;
	count += 1;

	pCount.h.c = 0xC1;
	pCount.h.headcode = 0x14;
	pCount.h.size = lOfs;
	pCount.count = count;

	memcpy(sendBuf,&pCount,sizeof(pCount));

	if(count < 1)	
	{
		return;
	}

	GSProtocol.MsgSendV2(lpObj,sendBuf,lOfs);
}

void gObjViewportListProtocolCreate(LPOBJ lpObj)
{
	int n,tObjNum;
	BYTE sendBuf[5000];
	int lOfs,count,moncount;
	PWMSG_COUNT pwCount;

	count = 0;
	moncount = 0;
	int iViewportSize = 0;
	lOfs = sizeof(pwCount);

	int MVL = MAX_VIEWPORT;

	if(lpObj->Type == OBJ_MONSTER)
	{
		MVL = MaxViewportMonster;
	}

	if(lpObj->Type == OBJ_USER)
	{
		if(lpObj->m_Change >= 0 && (lpObj->m_PlayerData->m_EnableUseChangeSkin == true || IsTransformCharacterSkin(lpObj->m_Change) == false))
		{	
			pViewportCreateChange.NumberH	= HIBYTE(lpObj->m_Index);
			pViewportCreateChange.NumberL	= LOBYTE(lpObj->m_Index);
			
			pViewportCreateChange.NumberH |= (0x01<<7);

			pViewportCreateChange.X			= (BYTE)lpObj->X;
			pViewportCreateChange.Y			= (BYTE)lpObj->Y;
			pViewportCreateChange.TX		= (BYTE)lpObj->TX;
			pViewportCreateChange.TY		= (BYTE)lpObj->TY;
			pViewportCreateChange.SkinH		= HIBYTE(LOWORD(lpObj->m_Change));
			pViewportCreateChange.SkinL		= LOBYTE(LOWORD(lpObj->m_Change));
			if(gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_GM_MARK) == true)	gObjAddBuffEffect(lpObj, BUFFTYPE_GM_MARK, 0, 0, 0, 0, -10);
			pViewportCreateChange.DirAndPkLevel  = lpObj->Dir<<4;

			if(lpObj->m_PlayerData->RegisterdLMS == 1 && lpObj->MapNumber == g_LastManStanding.m_Cfg.iPVPMap && g_LastManStanding.m_Rooms[lpObj->m_PlayerData->RegisteredLMSRoom].bState >= 2)
			{
				pViewportCreateChange.DirAndPkLevel |= (6&0x0F);
			}
			else
			{
				pViewportCreateChange.DirAndPkLevel |= (lpObj->m_PK_Level&0x0F);
			}

			pViewportCreateChange.PentagramMainAttribute = lpObj->m_iPentagramMainAttribute;
			pViewportCreateChange.MuunItemH = SET_NUMBERH(lpObj->m_wMuunItem);
			pViewportCreateChange.MuunItemL = SET_NUMBERL(lpObj->m_wMuunItem);
			pViewportCreateChange.MuunSubItemH = SET_NUMBERH(lpObj->m_wMuunSubItem);
			pViewportCreateChange.MuunSubItemL = SET_NUMBERL(lpObj->m_wMuunSubItem);
			pViewportCreateChange.MuunRideItemH = SET_NUMBERH(lpObj->m_wMuunRideItem);
			pViewportCreateChange.MuunRideItemL = SET_NUMBERL(lpObj->m_wMuunRideItem);

			if (lpObj->m_wMuunRideItem == (WORD)-1)
			{
				pViewportCreateChange.MuunRideItemH = SET_NUMBERH(lpObj->m_wInvenPet);
				pViewportCreateChange.MuunRideItemL = SET_NUMBERL(lpObj->m_wInvenPet);
			}

			pViewportCreateChange.LevelH = SET_NUMBERH(lpObj->Level);
			pViewportCreateChange.LevelL = SET_NUMBERL(lpObj->Level);
			pViewportCreateChange.MaxLifeHH = (int)(lpObj->AddLife + lpObj->MaxLife) >> 24;
			pViewportCreateChange.MaxLifeHL = (int)(lpObj->AddLife + lpObj->MaxLife) >> 8;
			pViewportCreateChange.MaxLifeLH = (int)(lpObj->AddLife + lpObj->MaxLife) >> 16;
			pViewportCreateChange.MaxLifeLL = (int)(lpObj->AddLife + lpObj->MaxLife);
			pViewportCreateChange.CurLifeHH = (int)lpObj->Life >> 24;
			pViewportCreateChange.CurLifeHL = (int)lpObj->Life >> 8;
			pViewportCreateChange.CurLifeLH = (int)lpObj->Life >> 16;
			pViewportCreateChange.CurLifeLL = (int)lpObj->Life;
			
			if (g_ConfigRead.SeasonX == false)
			{
				lpObj->CharSet[0] &= 0xF8;
				lpObj->CharSet[0] |= lpObj->m_ViewState&0x07;
			}

			else
			{
				lpObj->CharSet[0] &= 0xFC;
				lpObj->CharSet[0] |= lpObj->m_ViewState&0x03;
			}
			
			memcpy(&pViewportCreateChange.CharSet, lpObj->CharSet, 18);

			if (g_ConfigRead.server.GetServerType() == SERVER_BATTLECORE)
			{
				memcpy(pViewportCreateChange.Id, lpObj->m_PlayerData->m_RealNameOfUBF, MAX_ACCOUNT_LEN);
				pViewportCreateChange.nServerCodeOfHomeWorld = lpObj->m_PlayerData->m_nServerCodeOfHomeWorld;
			}

			else
			{
				memcpy(pViewportCreateChange.Id, lpObj->Name, MAX_ACCOUNT_LEN);
				pViewportCreateChange.nServerCodeOfHomeWorld = 0;
			}

			if (strlen(pViewportCreateChange.Id) < 1)
			{
				g_Log.Add("[UBF][gObjViewportListProtocolCreate][Name Setting Failed] (%d) ID : %s, Name: %s)",
					lpObj->m_Index, lpObj->AccountID, lpObj->Name);
			}

			pViewportCreateChange.BuffEffectCount = iViewportSize = gObjMakeViewportState(lpObj, pViewportCreateChange.BuffEffectList);

			memcpy((sendBuf+lOfs), (char*)&pViewportCreateChange, sizeof(PMSG_VIEWPORTCREATE_CHANGE) - (32-iViewportSize));
			lOfs += sizeof(PMSG_VIEWPORTCREATE_CHANGE) - (32-iViewportSize);
			count++;

			if( count < 1 )
			{
				return;
			}
			pwCount.h.headcode  = 0x45;
		}
		else
		{
			pViewportCreate.NumberH = HIBYTE(lpObj->m_Index);
			pViewportCreate.NumberL = LOBYTE(lpObj->m_Index);
			
			if (g_ConfigRead.SeasonX == false)
			{
				lpObj->CharSet[0] &= 0xF8;
				lpObj->CharSet[0] |= lpObj->m_ViewState&0x07;
			}

			else
			{
				lpObj->CharSet[0] &= 0xFC;
				lpObj->CharSet[0] |= lpObj->m_ViewState&0x03;
			}

			if (lpObj->m_Change < 0)
			{
				lpObj->m_PlayerData->m_EnableUseChangeSkin = false;
			}

			else
			{
				if (gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_GM_MARK) == TRUE)
				{
					gObjAddBuffEffect(lpObj, BUFFTYPE_GM_MARK, 0, 0, 0, 0, -10);
				}
			}

			if( lpObj->m_State == 1 )
			{
				if( !lpObj->Teleport )
				{
					pViewportCreate.NumberH |= (0x01<<7);
				}
			}

			pViewportCreate.X       = (BYTE)lpObj->X;
			pViewportCreate.Y       = (BYTE)lpObj->Y;
			pViewportCreate.TX		= (BYTE)lpObj->TX;
			pViewportCreate.TY		= (BYTE)lpObj->TY;
			pViewportCreate.DirAndPkLevel = lpObj->Dir<<4;

			if(lpObj->m_PlayerData->RegisterdLMS == 1 && lpObj->MapNumber == g_LastManStanding.m_Cfg.iPVPMap && g_LastManStanding.m_Rooms[lpObj->m_PlayerData->RegisteredLMSRoom].bState >= 2)
			{
				pViewportCreate.DirAndPkLevel |= (6&0x0F);
			}
			else
			{
				pViewportCreate.DirAndPkLevel |= (lpObj->m_PK_Level&0x0F);
			}

			memcpy(&pViewportCreate.CharSet, lpObj->CharSet, 18);

			if (g_ConfigRead.server.GetServerType() == SERVER_BATTLECORE)
			{
				memcpy(pViewportCreate.Id, lpObj->m_PlayerData->m_RealNameOfUBF, MAX_ACCOUNT_LEN);
				pViewportCreate.nServerCodeOfHomeWorld = lpObj->m_PlayerData->m_nServerCodeOfHomeWorld;
			}

			else
			{
				memcpy(pViewportCreate.Id, lpObj->Name, MAX_ACCOUNT_LEN);
				pViewportCreate.nServerCodeOfHomeWorld = 0;
			}

			if (strlen(pViewportCreate.Id) < 1)
			{
				g_Log.Add("[UBF][gObjViewportListProtocolCreate_2][Name Setting Failed] (%d) ID : %s, Name: %s)",
					lpObj->m_Index, lpObj->AccountID, lpObj->Name);
			}

			pViewportCreate.PentagramMainAttribute = lpObj->m_iPentagramMainAttribute;
			pViewportCreate.MuunItemH = SET_NUMBERH(lpObj->m_wMuunItem);
			pViewportCreate.MuunItemL = SET_NUMBERL(lpObj->m_wMuunItem);
			pViewportCreate.MuunSubItemH = SET_NUMBERH(lpObj->m_wMuunSubItem);
			pViewportCreate.MuunSubItemL = SET_NUMBERL(lpObj->m_wMuunSubItem);
			pViewportCreate.MuunRideItemH = SET_NUMBERH(lpObj->m_wMuunRideItem);
			pViewportCreate.MuunRideItemL = SET_NUMBERL(lpObj->m_wMuunRideItem);

			if (lpObj->m_wMuunRideItem == (WORD)-1)
			{
				pViewportCreate.MuunRideItemH = SET_NUMBERH(lpObj->m_wInvenPet);
				pViewportCreate.MuunRideItemL = SET_NUMBERL(lpObj->m_wInvenPet);
			}

			pViewportCreate.LevelH = SET_NUMBERH(lpObj->Level);
			pViewportCreate.LevelL = SET_NUMBERL(lpObj->Level);
			pViewportCreate.MaxLifeHH = (int)(lpObj->AddLife + lpObj->MaxLife) >> 24;
			pViewportCreate.MaxLifeHL = (int)(lpObj->AddLife + lpObj->MaxLife) >> 8;
			pViewportCreate.MaxLifeLH = (int)(lpObj->AddLife + lpObj->MaxLife) >> 16;
			pViewportCreate.MaxLifeLL = (int)(lpObj->AddLife + lpObj->MaxLife);
			pViewportCreate.CurLifeHH = (int)lpObj->Life >> 24;
			pViewportCreate.CurLifeHL = (int)lpObj->Life >> 8;
			pViewportCreate.CurLifeLH = (int)lpObj->Life >> 16;
			pViewportCreate.CurLifeLL = (int)lpObj->Life;
			pViewportCreate.BuffEffectCount = iViewportSize = gObjMakeViewportState(lpObj, pViewportCreate.BuffEffectList);

			memcpy((sendBuf+lOfs), (char*)&pViewportCreate, sizeof(PMSG_VIEWPORTCREATE));
			lOfs += sizeof(PMSG_VIEWPORTCREATE) - (32-iViewportSize);
			count++;

			if( count < 1 )
			{
				return;
			}

			pwCount.h.headcode  = 0x12;
		}

		pwCount.h.c = 0xC2;
		pwCount.h.sizeH = SET_NUMBERH(lOfs);
		pwCount.h.sizeL = SET_NUMBERL(lOfs);
		pwCount.count = count;

		memcpy(sendBuf,&pwCount,sizeof(pwCount));

		IOCP.DataSend(lpObj->m_Index,sendBuf,lOfs);

		if(lpObj->m_PlayerData->GuildNumber != 0)
		{
			GuildInfoOfs = 0;
			GuildInfoOfs = 5;

			PMSG_SIMPLE_GUILDVIEWPORT pGuild;

			pGuild.GuildNumber = lpObj->m_PlayerData->lpGuild->Number;
			pGuild.NumberH = SET_NUMBERH(lpObj->m_Index) & 0x7F;
			pGuild.NumberL = SET_NUMBERL(lpObj->m_Index);

			if(strcmp(lpObj->m_PlayerData->lpGuild->Names[0],lpObj->Name) == 0)
			{
				pGuild.NumberH |= 0x80;
			}

			pGuild.btGuildType = lpObj->m_PlayerData->lpGuild->btGuildType;
			pGuild.btGuildStatus = lpObj->m_PlayerData->GuildStatus;

			if (g_CastleSiegeSync.CheckCastleOwnerMember(lpObj->m_Index) == TRUE || //season 4 add-on
				g_CastleSiegeSync.CheckCastleOwnerUnionMember(lpObj->m_Index) == TRUE)
			{
				pGuild.btOwnerStatus = 1;
			}
			else
			{
				pGuild.btOwnerStatus = 0;
			}

			memcpy(&GuildInfoBuf[GuildInfoOfs],&pGuild,sizeof(pGuild));

			GuildInfoOfs += sizeof(pGuild);

			PMSG_SIMPLE_GUILDVIEWPORT_COUNT pwCount;

			pwCount.h.c = 0xC2;
			pwCount.h.headcode = 0x65;
			pwCount.h.sizeH = SET_NUMBERH(GuildInfoOfs);
			pwCount.h.sizeL = SET_NUMBERL(GuildInfoOfs);
			pwCount.Count = 1;

			memcpy(GuildInfoBuf,&pwCount,sizeof(pwCount));

			IOCP.DataSend(lpObj->m_Index,GuildInfoBuf,GuildInfoOfs);
		}

		g_GensSystem.SendGensInfo(lpObj);

		for(int n = 0; n < MVL; n++)
		{
			if(lpObj->VpPlayer2[n].state != 0)
			{
				tObjNum = lpObj->VpPlayer2[n].number;

				if(tObjNum >= 0)
				{
					if(lpObj->VpPlayer2[n].type == OBJ_USER)
					{
						IOCP.DataSend(tObjNum,sendBuf,lOfs);

						if(GuildInfoCount != 0)
						{
							IOCP.DataSend(tObjNum,GuildInfoBuf,GuildInfoOfs);
						}

						if(GuildUserCount != 0)
						{
							IOCP.DataSend(tObjNum,GuildUserBuf,GuildUserOfs);
						}
					}
				}
			}
		}

		g_GensSystem.GensViewportListProtocol(lpObj);
	}
	else
	{
		count = 0;
	
		if(lpObj->m_RecallMon >= 0 && (lpObj->Class < 678 || lpObj->Class > 680))
		{
			pCallMonsterViewportCreate.NumberH = HIBYTE(lpObj->m_Index);
			pCallMonsterViewportCreate.NumberL = LOBYTE(lpObj->m_Index);
			if( lpObj->m_State == 1 )
				pCallMonsterViewportCreate.NumberH |= (0x01<<7);
								
			pCallMonsterViewportCreate.Type_HI    = HIBYTE(lpObj->Class);
			pCallMonsterViewportCreate.Type_LO    = LOBYTE(lpObj->Class);
			pCallMonsterViewportCreate.X       = (BYTE)lpObj->X;
			pCallMonsterViewportCreate.Y       = (BYTE)lpObj->Y;
			pCallMonsterViewportCreate.TX      = (BYTE)lpObj->TX;
			pCallMonsterViewportCreate.TY      = (BYTE)lpObj->TY;
			pCallMonsterViewportCreate.Path    = lpObj->Dir<<4;
			pCallMonsterViewportCreate.PentagramMainAttribute = lpObj->m_iPentagramMainAttribute;
			pCallMonsterViewportCreate.LevelH = SET_NUMBERH(lpObj->Level);
			pCallMonsterViewportCreate.LevelL = SET_NUMBERL(lpObj->Level);
			pCallMonsterViewportCreate.MaxLifeHH = (int)(lpObj->AddLife + lpObj->MaxLife) >> 24;
			pCallMonsterViewportCreate.MaxLifeHL = (int)(lpObj->AddLife + lpObj->MaxLife) >> 8;
			pCallMonsterViewportCreate.MaxLifeLH = (int)(lpObj->AddLife + lpObj->MaxLife) >> 16;
			pCallMonsterViewportCreate.MaxLifeLL = (int)(lpObj->AddLife + lpObj->MaxLife);
			pCallMonsterViewportCreate.CurLifeHH = (int)lpObj->Life >> 24;
			pCallMonsterViewportCreate.CurLifeHL = (int)lpObj->Life >> 8;
			pCallMonsterViewportCreate.CurLifeLH = (int)lpObj->Life >> 16;
			pCallMonsterViewportCreate.CurLifeLL = (int)lpObj->Life;

			if( lpObj->m_RecallMon >= 0 && lpObj->m_RecallMon < g_ConfigRead.server.GetObjectMax()-1 )
			{
				memcpy(pCallMonsterViewportCreate.Id, gObj[lpObj->m_RecallMon].Name, MAX_ACCOUNT_LEN);
			}
			else memset(pCallMonsterViewportCreate.Id, 0, MAX_ACCOUNT_LEN);								
									
			pCallMonsterViewportCreate.BuffEffectCount = iViewportSize = gObjMakeViewportState(lpObj, pCallMonsterViewportCreate.BuffEffectList);

			memcpy((sendBuf+lOfs), (char*)&pCallMonsterViewportCreate, sizeof(PMSG_CALLMONSTER_VIEWPORTCREATE));
			lOfs += sizeof(PMSG_CALLMONSTER_VIEWPORTCREATE) - (32-iViewportSize);
			count++;
		}
		else
		{
			pMonsterViewportCreate.NumberH = HIBYTE(lpObj->m_Index);
			pMonsterViewportCreate.NumberL = LOBYTE(lpObj->m_Index);
			if( lpObj->m_State == 1 && lpObj->Teleport == 0)
			{
				pMonsterViewportCreate.NumberH |= (0x01<<7);
				pMonsterViewportCreate.NumberH |= (0x01<<6);
			}
			pMonsterViewportCreate.Type_HI    = HIBYTE(lpObj->Class);
			pMonsterViewportCreate.Type_LO    = LOBYTE(lpObj->Class);
			pMonsterViewportCreate.X       = (BYTE)lpObj->X;
			pMonsterViewportCreate.Y       = (BYTE)lpObj->Y;
			pMonsterViewportCreate.TX      = (BYTE)lpObj->TX;
			pMonsterViewportCreate.TY      = (BYTE)lpObj->TY;
			pMonsterViewportCreate.Path    = lpObj->Dir<<4;
			pMonsterViewportCreate.PentagramMainAttribute = lpObj->m_iPentagramMainAttribute;
			pMonsterViewportCreate.LevelH = SET_NUMBERH(lpObj->Level);
			pMonsterViewportCreate.LevelL = SET_NUMBERL(lpObj->Level);
			pMonsterViewportCreate.MaxLifeHH = (int)(lpObj->AddLife + lpObj->MaxLife) >> 24;
			pMonsterViewportCreate.MaxLifeHL = (int)(lpObj->AddLife + lpObj->MaxLife) >> 8;
			pMonsterViewportCreate.MaxLifeLH = (int)(lpObj->AddLife + lpObj->MaxLife) >> 16;
			pMonsterViewportCreate.MaxLifeLL = (int)(lpObj->AddLife + lpObj->MaxLife);
			pMonsterViewportCreate.CurLifeHH = (int)lpObj->Life >> 24;
			pMonsterViewportCreate.CurLifeHL = (int)lpObj->Life >> 8;
			pMonsterViewportCreate.CurLifeLH = (int)lpObj->Life >> 16;
			pMonsterViewportCreate.CurLifeLL = (int)lpObj->Life;
			pMonsterViewportCreate.BuffEffectCount = iViewportSize = gObjMakeViewportState(lpObj, pMonsterViewportCreate.BuffEffectList);
			memcpy((sendBuf+lOfs), (char*)&pMonsterViewportCreate, sizeof(PMSG_MONSTER_VIEWPORTCREATE));
			lOfs += sizeof(PMSG_MONSTER_VIEWPORTCREATE) - (32-iViewportSize);
			moncount++;
		}
	
		if(count > 0)
		{
			PWMSG_COUNT pwCount;
	
			pwCount.h.c = 0xC2;
			pwCount.h.headcode = 0x1F;
			pwCount.h.sizeH = SET_NUMBERH(lOfs);
			pwCount.h.sizeL = SET_NUMBERL(lOfs);
			pwCount.count = count;
	
			memcpy(sendBuf,&pwCount,sizeof(pwCount));
			GSProtocol.MsgSendV2(lpObj,sendBuf,lOfs);
		}
	
		if(moncount > 0)
		{
			PWMSG_COUNT pwCount;
	
			pwCount.h.c = 0xC2;
			pwCount.h.headcode = 0x13;
			pwCount.h.sizeH = SET_NUMBERH(lOfs);
			pwCount.h.sizeL = SET_NUMBERL(lOfs);
			pwCount.count = moncount;
	
			memcpy(sendBuf,&pwCount,sizeof(pwCount));
			GSProtocol.MsgSendV2(lpObj,sendBuf,lOfs);
		}
	}
}

void gObjViewportListProtocol(short aIndex)
{
	if (gObj[aIndex].Connected < PLAYER_PLAYING)
	{
		return;
	}

	PBMSG_COUNT pCount;
	PBMSG_COUNT2 pCount2;
	LPOBJ lpTargetObj, lpObj = &gObj[aIndex];
	int lOfs_Item, count_Item, ItemStructSize = sizeof(PMSG_ITEMVIEWPORTCREATE), lOfs, MonlOfs, callMonlOfs;
	int pMsgSize;
	short tObjNum;
	BYTE count, monstercount, callmonstercount;

	BYTE sendBuf[5000];
	BYTE MonstersendBuf[5000];
	BYTE callMonstersendBuf[5000];
	BYTE ItemBuf[5000];

	GuildInfoOfs = 0;
	GuildInfoCount = 0;
	GensInfoOfs = 0;
	GensInfoCount = 0;
	GuildUserOfs = 0;
	GuildUserCount = 0;
	ChangeCount = 0;
	GuildInfoOfs = 5;
	GuildUserOfs = 5;
	GensInfoOfs = 6;
	lOfsChange = 5;

	lOfs = 4;
	lOfs_Item = 5;
	count = 0;
	int iViewportSize = 0;
	count_Item = 0;

	if (lpObj->Type == OBJ_USER)
	{
		for (int n = 0; n < MAX_VIEWPORT; n++)
		{
			if (lpObj->VpPlayer[n].state == 3)
			{
				tObjNum = lpObj->VpPlayer[n].number;

				if (tObjNum >= 0)
				{
					switch (lpObj->VpPlayer[n].type)
					{
					case 1:
					case 2:
					case 3:
						pViewportDestroy.NumberH = SET_NUMBERH(tObjNum);
						pViewportDestroy.NumberL = SET_NUMBERL(tObjNum);

						memcpy(&sendBuf[lOfs], &pViewportDestroy, sizeof(pViewportDestroy));
						lOfs += sizeof(pViewportDestroy);
						count += 1;
						break;
					case 5:
						pItemViewportDestroy.NumberH = SET_NUMBERH(tObjNum);
						pItemViewportDestroy.NumberL = SET_NUMBERL(tObjNum);

						memcpy(&ItemBuf[lOfs_Item], &pItemViewportDestroy, sizeof(pItemViewportDestroy));

						lOfs_Item += sizeof(pItemViewportDestroy);
						count_Item += 1;
						break;
					}
				}

				lpObj->VpPlayer[n].state = 0;
				lpObj->VpPlayer[n].number = -1;
				lpObj->VPCount -= 1;
			}
		}
	}

	else if (lpObj->Type == OBJ_MONSTER || lpObj->Type == OBJ_NPC)
	{
		for (int n = 0; n < MaxViewportMonster; n++)
		{
			if (lpObj->VpPlayer[n].state == 3)
			{
				lpObj->VpPlayer[n].state = 0;
				lpObj->VpPlayer[n].number = -1;
				lpObj->VPCount -= 1;
			}
		}
	}

	if (lpObj->Type == OBJ_USER)
	{
		if (count > 0)
		{
			pCount.h.c = 0xC1;
			pCount.h.headcode = 0x14;
			pCount.h.size = lOfs;
			pCount.count = count;

			memcpy(sendBuf, &pCount, sizeof(pCount));
			IOCP.DataSend(aIndex, sendBuf, lOfs);
		}

		if (count_Item > 0)
		{
			pItemCount.h.c = 0xC2;
			pItemCount.h.headcode = 0x21;
			pItemCount.h.sizeH = SET_NUMBERH(lOfs_Item);
			pItemCount.h.sizeL = SET_NUMBERL(lOfs_Item);
			pItemCount.count = count_Item;

			memcpy(ItemBuf, &pItemCount, sizeof(pItemCount));
			IOCP.DataSend(aIndex, ItemBuf, lOfs_Item);
		}
	}

	if (lpObj->Type == OBJ_MONSTER || lpObj->Type == OBJ_NPC)
	{
		for (int n = 0; n < MaxViewportMonster; n++)
		{
			if (lpObj->VpPlayer[n].state == 1)
			{
				lpObj->VpPlayer[n].state = 2;
			}
		}
	}
	else
	{
		count = 0;
		monstercount = 0;
		count_Item = 0;
		callmonstercount = 0;
		lOfs = 5;
		MonlOfs = lOfs;
		callMonlOfs = MonlOfs;
		lOfs_Item = 5;
		pMsgSize = 44;

		if (lpObj->Type == OBJ_USER)
		{
			for (int n = 0; n < MAX_VIEWPORT; n++)
			{
				if (lpObj->VpPlayer[n].state == 1)
				{
					tObjNum = lpObj->VpPlayer[n].number;

					if (tObjNum >= 0)
					{
						if ((gObj[tObjNum].Authority & 0x20) == 0x20 && gObjCheckUsedBuffEffect(&gObj[tObjNum], BUFFTYPE_INVISABLE))
							continue;

						if (g_NewPVP.IsObserver(gObj[tObjNum]) == TRUE)
						{
							continue;
						}

						switch (lpObj->VpPlayer[n].type)
						{
						case 1:
							lpTargetObj = &gObj[tObjNum];

							if (lpTargetObj->m_Change >= 0 && (lpTargetObj->m_PlayerData->m_EnableUseChangeSkin == true || IsTransformCharacterSkin(lpTargetObj->m_Change) == false))
							{
								pViewportCreateChange.NumberH = SET_NUMBERH(tObjNum);
								pViewportCreateChange.NumberL = SET_NUMBERL(tObjNum);

								if (g_ConfigRead.SeasonX == false)
								{
									lpTargetObj->CharSet[0] &= 0xF8;
									lpTargetObj->CharSet[0] |= lpTargetObj->m_ViewState&0x07;
								}

								else
								{
									lpTargetObj->CharSet[0] &= 0xFC;
									lpTargetObj->CharSet[0] |= lpTargetObj->m_ViewState&0x03;
								}

								if ((lpTargetObj->Authority & 0x20) == 0x20)
								{
									gObjAddBuffEffect(lpTargetObj, BUFFTYPE_GM_MARK, 0, 0, 0, 0, -10);
								}

								if (lpTargetObj->m_State == 1 && lpTargetObj->Teleport == 0)
								{
									pViewportCreateChange.NumberH |= 0x80;
								}

								pViewportCreateChange.X = lpTargetObj->X;
								pViewportCreateChange.Y = lpTargetObj->Y;
								pViewportCreateChange.TX = lpTargetObj->TX;
								pViewportCreateChange.TY = lpTargetObj->TY;
								pViewportCreateChange.SkinH = SET_NUMBERH((lpTargetObj->m_Change & 0xFFFF) & 0xFFFF);
								pViewportCreateChange.SkinL = SET_NUMBERL((lpTargetObj->m_Change & 0xFFFF) & 0xFFFF);
								pViewportCreateChange.DirAndPkLevel = lpTargetObj->Dir << 4;
								
								if (lpTargetObj->m_PlayerData->RegisterdLMS == 1 && lpObj->MapNumber == g_LastManStanding.m_Cfg.iPVPMap && g_LastManStanding.m_Rooms[lpObj->m_PlayerData->RegisteredLMSRoom].bState >= 2)
								{
									pViewportCreateChange.DirAndPkLevel |= (6 & 0x0F);
								}
								else
								{
									pViewportCreateChange.DirAndPkLevel |= (lpTargetObj->m_PK_Level & 0x0F);
								}

								if (g_ConfigRead.SeasonX == false)
								{
									lpObj->CharSet[0] &= 0xF8;
									lpObj->CharSet[0] |= lpObj->m_ViewState&0x07;
								}

								else
								{
									lpObj->CharSet[0] &= 0xFC;
									lpObj->CharSet[0] |= lpObj->m_ViewState&0x03;
								}

								pViewportCreateChange.PentagramMainAttribute = lpTargetObj->m_iPentagramMainAttribute;
								pViewportCreateChange.MuunItemH = SET_NUMBERH(lpTargetObj->m_wMuunItem);
								pViewportCreateChange.MuunItemL = SET_NUMBERL(lpTargetObj->m_wMuunItem);
								pViewportCreateChange.MuunSubItemH = SET_NUMBERH(lpTargetObj->m_wMuunSubItem);
								pViewportCreateChange.MuunSubItemL = SET_NUMBERL(lpTargetObj->m_wMuunSubItem);
								pViewportCreateChange.MuunRideItemH = SET_NUMBERH(lpTargetObj->m_wMuunRideItem);
								pViewportCreateChange.MuunRideItemL = SET_NUMBERL(lpTargetObj->m_wMuunRideItem);

								if (lpTargetObj->m_wMuunRideItem == (WORD)-1)
								{
									pViewportCreateChange.MuunRideItemH = SET_NUMBERH(lpTargetObj->m_wInvenPet);
									pViewportCreateChange.MuunRideItemL = SET_NUMBERL(lpTargetObj->m_wInvenPet);
								}

								pViewportCreateChange.LevelH = SET_NUMBERH(lpTargetObj->Level);
								pViewportCreateChange.LevelL = SET_NUMBERL(lpTargetObj->Level);
								pViewportCreateChange.MaxLifeHH = (int)(lpTargetObj->AddLife + lpTargetObj->MaxLife) >> 24;
								pViewportCreateChange.MaxLifeHL = (int)(lpTargetObj->AddLife + lpTargetObj->MaxLife) >> 8;
								pViewportCreateChange.MaxLifeLH = (int)(lpTargetObj->AddLife + lpTargetObj->MaxLife) >> 16;
								pViewportCreateChange.MaxLifeLL = (int)(lpTargetObj->AddLife + lpTargetObj->MaxLife);
								pViewportCreateChange.CurLifeHH = (int)lpTargetObj->Life >> 24;
								pViewportCreateChange.CurLifeHL = (int)lpTargetObj->Life >> 8;
								pViewportCreateChange.CurLifeLH = (int)lpTargetObj->Life >> 16;
								pViewportCreateChange.CurLifeLL = (int)lpTargetObj->Life;

								memcpy(pViewportCreateChange.CharSet, lpTargetObj->CharSet, sizeof(pViewportCreateChange.CharSet)); //New for Fix Rings of Transformation

								if (g_ConfigRead.server.GetServerType() == SERVER_BATTLECORE)
								{
									memcpy(pViewportCreateChange.Id, lpTargetObj->m_PlayerData->m_RealNameOfUBF, MAX_ACCOUNT_LEN);
									pViewportCreateChange.nServerCodeOfHomeWorld = lpTargetObj->m_PlayerData->m_nServerCodeOfHomeWorld;
								}

								else
								{
									memcpy(pViewportCreateChange.Id, lpTargetObj->Name, sizeof(pViewportCreateChange.Id));
									pViewportCreateChange.nServerCodeOfHomeWorld = 0;
								}

								if (strlen(pViewportCreateChange.Id) < 1)
								{
									g_Log.Add("[UBF][gObjViewportListProtocol][Name Setting Failed] (%d) ID : %s, Name: %s)",
										lpObj->m_Index, lpObj->AccountID, lpObj->Name);
								}

								pViewportCreateChange.BuffEffectCount = iViewportSize = gObjMakeViewportState(lpTargetObj, pViewportCreateChange.BuffEffectList);

								memcpy((SendGBufChange + lOfsChange), (char*)&pViewportCreateChange, sizeof(PMSG_VIEWPORTCREATE_CHANGE));
								lOfsChange += sizeof(PMSG_VIEWPORTCREATE_CHANGE) - (MAX_BUFFEFFECT - iViewportSize);
								ChangeCount++;
							}
							else
							{
								pViewportCreate.NumberH = SET_NUMBERH(tObjNum);
								pViewportCreate.NumberL = SET_NUMBERL(tObjNum);

								if (g_ConfigRead.SeasonX == false)
								{
									lpTargetObj->CharSet[0] &= 0xF8;
									lpTargetObj->CharSet[0] |= lpTargetObj->m_ViewState&0x07;
								}

								else
								{
									lpTargetObj->CharSet[0] &= 0xFC;
									lpTargetObj->CharSet[0] |= lpTargetObj->m_ViewState&0x03;
								}

								if (lpTargetObj->m_Change < 0)
								{
									lpTargetObj->m_PlayerData->m_EnableUseChangeSkin = false;
								}

								else
								{
									if (gObjCheckUsedBuffEffect(lpTargetObj, BUFFTYPE_GM_MARK) == TRUE)
									{
										gObjAddBuffEffect(lpTargetObj, BUFFTYPE_GM_MARK, 0, 0, 0, 0, -10);
									}
								}

								if (lpTargetObj->m_State == 1 && lpTargetObj->Teleport == 0)
								{
									pViewportCreate.NumberH |= 0x80;
								}

								pViewportCreate.X = lpTargetObj->X;
								pViewportCreate.Y = lpTargetObj->Y;
								pViewportCreate.TX = lpTargetObj->TX;
								pViewportCreate.TY = lpTargetObj->TY;

								pViewportCreate.DirAndPkLevel = lpTargetObj->Dir << 4;
								if (lpObj->m_PlayerData->RegisterdLMS == 1 && lpObj->MapNumber == g_LastManStanding.m_Cfg.iPVPMap && g_LastManStanding.m_Rooms[lpObj->m_PlayerData->RegisteredLMSRoom].bState >= 2)
								{
									pViewportCreate.DirAndPkLevel |= (6 & 0x0F);
								}
								else
								{
									pViewportCreate.DirAndPkLevel |= (lpTargetObj->m_PK_Level & 0x0F);
								}

								pViewportCreate.PentagramMainAttribute = lpTargetObj->m_iPentagramMainAttribute;
								pViewportCreate.MuunItemH = SET_NUMBERH(lpTargetObj->m_wMuunItem);
								pViewportCreate.MuunItemL = SET_NUMBERL(lpTargetObj->m_wMuunItem);
								pViewportCreate.MuunSubItemH = SET_NUMBERH(lpTargetObj->m_wMuunSubItem);
								pViewportCreate.MuunSubItemL = SET_NUMBERL(lpTargetObj->m_wMuunSubItem);
								pViewportCreate.MuunRideItemH = SET_NUMBERH(lpTargetObj->m_wMuunRideItem);
								pViewportCreate.MuunRideItemL = SET_NUMBERL(lpTargetObj->m_wMuunRideItem);

								if (lpTargetObj->m_wMuunRideItem == (WORD)-1)
								{
									pViewportCreate.MuunRideItemH = SET_NUMBERH(lpTargetObj->m_wInvenPet);
									pViewportCreate.MuunRideItemL = SET_NUMBERL(lpTargetObj->m_wInvenPet);
								}
								pViewportCreate.LevelH = SET_NUMBERH(lpTargetObj->Level);
								pViewportCreate.LevelL = SET_NUMBERL(lpTargetObj->Level);
								pViewportCreate.MaxLifeHH = (int)(lpTargetObj->AddLife + lpTargetObj->MaxLife) >> 24;
								pViewportCreate.MaxLifeHL = (int)(lpTargetObj->AddLife + lpTargetObj->MaxLife) >> 8;
								pViewportCreate.MaxLifeLH = (int)(lpTargetObj->AddLife + lpTargetObj->MaxLife) >> 16;
								pViewportCreate.MaxLifeLL = (int)(lpTargetObj->AddLife + lpTargetObj->MaxLife);
								pViewportCreate.CurLifeHH = (int)lpTargetObj->Life >> 24;
								pViewportCreate.CurLifeHL = (int)lpTargetObj->Life >> 8;
								pViewportCreate.CurLifeLH = (int)lpTargetObj->Life >> 16;
								pViewportCreate.CurLifeLL = (int)lpTargetObj->Life;

								memcpy(pViewportCreate.CharSet, lpTargetObj->CharSet, sizeof(pViewportCreate.CharSet));

								if (g_ConfigRead.server.GetServerType() == SERVER_BATTLECORE)
								{
									memcpy(pViewportCreate.Id, lpTargetObj->m_PlayerData->m_RealNameOfUBF, MAX_ACCOUNT_LEN);
									pViewportCreate.nServerCodeOfHomeWorld = lpTargetObj->m_PlayerData->m_nServerCodeOfHomeWorld;
								}

								else
								{
									memcpy(pViewportCreate.Id, lpTargetObj->Name, sizeof(pViewportCreate.Id));
									pViewportCreate.nServerCodeOfHomeWorld = 0;
								}

								if (strlen(pViewportCreate.Id) < 1)
								{
									g_Log.Add("[UBF][gObjViewportListProtocol_2][Name Setting Failed] (%d) ID : %s, Name: %s)",
										lpObj->m_Index, lpObj->AccountID, lpObj->Name);
								}

								pViewportCreate.BuffEffectCount = iViewportSize = gObjMakeViewportState(lpTargetObj, pViewportCreate.BuffEffectList);

								memcpy((sendBuf + lOfs), (char*)&pViewportCreate, sizeof(PMSG_VIEWPORTCREATE));
								lOfs += sizeof(PMSG_VIEWPORTCREATE) - (MAX_BUFFEFFECT - iViewportSize);
								count++;
							}

							if (lpObj->Type == OBJ_USER)
							{
								if (lpTargetObj->m_PlayerData->lpGuild != 0)
								{
									PMSG_SIMPLE_GUILDVIEWPORT pGuild;

									pGuild.GuildNumber = lpTargetObj->m_PlayerData->lpGuild->Number;
									pGuild.NumberH = SET_NUMBERH(lpTargetObj->m_Index) & 0x7F;
									pGuild.NumberL = SET_NUMBERL(lpTargetObj->m_Index);

									pGuild.btGuildStatus = lpTargetObj->m_PlayerData->GuildStatus;
									pGuild.btGuildType = lpTargetObj->m_PlayerData->lpGuild->btGuildType;

									if (lpObj->m_PlayerData->lpGuild != 0)
									{
										pGuild.btGuildRelationShip = gObjGetRelationShip(lpObj, lpTargetObj);
									}
									else
									{
										pGuild.btGuildRelationShip = 0;
									}

									if (strcmp(lpTargetObj->m_PlayerData->lpGuild->Names[0], lpTargetObj->Name) == 0)
									{
										pGuild.NumberH |= 0x80;
									}

									if (g_CastleSiegeSync.CheckCastleOwnerMember(lpTargetObj->m_Index) == TRUE || //season 4 add-on
										g_CastleSiegeSync.CheckCastleOwnerUnionMember(lpTargetObj->m_Index) == TRUE)
									{
										pGuild.btOwnerStatus = 1;
									}
									else
									{
										pGuild.btOwnerStatus = 0;
									}

									memcpy(&GuildInfoBuf[GuildInfoOfs], &pGuild, sizeof(pGuild));
									GuildInfoOfs += sizeof(pGuild);
									GuildInfoCount += 1;
								}

								if (false)
								{

									PMSG_GUILDVIEWPORT_USER pGuildUserViewport;

									if (ViewGuildMng.Add(lpTargetObj->m_PlayerData->GuildNumber, lpTargetObj->m_Index) == 1 && lpTargetObj->m_PlayerData->lpGuild != 0)
									{
										PMSG_GUILDVIEWPORT pGuildViewport;

										pGuildViewport.NumberH = SET_NUMBERH(lpTargetObj->m_PlayerData->lpGuild->Number);
										pGuildViewport.NumberL = SET_NUMBERL(lpTargetObj->m_PlayerData->lpGuild->Number);

										strcpy(pGuildViewport.GuildName, lpTargetObj->m_PlayerData->lpGuild->Name);
										memcpy(pGuildViewport.Mark, lpTargetObj->m_PlayerData->lpGuild->Mark, sizeof(pGuildViewport.Mark));
										memcpy(&GuildInfoBuf[GuildInfoOfs], &pGuildViewport, sizeof(pGuildViewport));

										GuildInfoOfs += sizeof(pGuildViewport);
										GuildInfoCount += 1;
									}

									pGuildUserViewport.NumberH = SET_NUMBERH(lpTargetObj->m_Index) & 0x7F;
									pGuildUserViewport.NumberL = SET_NUMBERL(lpTargetObj->m_Index);

									if (lpTargetObj->m_PlayerData->lpGuild != 0 && strcmp(lpTargetObj->m_PlayerData->lpGuild->Names[0], lpTargetObj->Name) == 0)
									{
										pGuildUserViewport.NumberH |= 0x80;
									}

									if (lpTargetObj->m_PlayerData->lpGuild != 0)
									{
										pGuildUserViewport.GNumberH = SET_NUMBERH(lpTargetObj->m_PlayerData->lpGuild->Number);
										pGuildUserViewport.GNumberL = SET_NUMBERL(lpTargetObj->m_PlayerData->lpGuild->Number);

										memcpy(&GuildUserBuf[GuildUserOfs], &pGuildUserViewport, sizeof(pGuildUserViewport));

										GuildUserOfs += sizeof(pGuildUserViewport);
										GuildUserCount += 1;
									}
								}

								if (g_GensSystem.GetGensInfluence(lpTargetObj))
								{
									_tagPMSG_GENS_MEMBER_VIEWPORT_INFO pGensMsg;

									pGensMsg.btInfluence = g_GensSystem.GetGensInfluence(lpTargetObj);
									pGensMsg.NumberH = SET_NUMBERH(lpTargetObj->m_Index);
									pGensMsg.NumberL = SET_NUMBERL(lpTargetObj->m_Index);
									pGensMsg.iGensRanking = lpTargetObj->m_PlayerData->m_GensRanking;
									pGensMsg.iGensClass = lpTargetObj->m_PlayerData->m_GensClass;
									pGensMsg.iContributePoint = lpTargetObj->m_PlayerData->m_ContributePoint;

									memcpy(&GensInfoBuf[GensInfoOfs], &pGensMsg, sizeof(pGensMsg));

									GensInfoOfs += sizeof(pGensMsg);
									GensInfoCount += 1;
								}
							}
							break;
						case 2:
						case 3:
							if (lpObj->Type == OBJ_USER)
							{
								lpTargetObj = &gObj[tObjNum];

								if (lpTargetObj->m_RecallMon >= 0  && (lpTargetObj->Class < 678 || lpTargetObj->Class > 680))
								{
									pCallMonsterViewportCreate.NumberH = SET_NUMBERH(tObjNum);
									pCallMonsterViewportCreate.NumberL = SET_NUMBERL(tObjNum);

									if (lpTargetObj->m_State == 1)
									{
										pCallMonsterViewportCreate.NumberH |= 0x80;
									}

									pCallMonsterViewportCreate.Type_HI = SET_NUMBERH(lpTargetObj->Class);
									pCallMonsterViewportCreate.Type_LO = SET_NUMBERL(lpTargetObj->Class);

									pCallMonsterViewportCreate.X = lpTargetObj->X;
									pCallMonsterViewportCreate.Y = lpTargetObj->Y;
									pCallMonsterViewportCreate.TX = lpTargetObj->TX;
									pCallMonsterViewportCreate.TY = lpTargetObj->TY;
									pCallMonsterViewportCreate.Path = lpTargetObj->Dir << 4;

									pCallMonsterViewportCreate.PentagramMainAttribute = lpTargetObj->m_iPentagramMainAttribute;
									pCallMonsterViewportCreate.LevelH = SET_NUMBERH(lpTargetObj->Level);
									pCallMonsterViewportCreate.LevelL = SET_NUMBERL(lpTargetObj->Level);
									pCallMonsterViewportCreate.MaxLifeHH = (int)(lpTargetObj->AddLife + lpTargetObj->MaxLife) >> 24;
									pCallMonsterViewportCreate.MaxLifeHL = (int)(lpTargetObj->AddLife + lpTargetObj->MaxLife) >> 8;
									pCallMonsterViewportCreate.MaxLifeLH = (int)(lpTargetObj->AddLife + lpTargetObj->MaxLife) >> 16;
									pCallMonsterViewportCreate.MaxLifeLL = (int)(lpTargetObj->AddLife + lpTargetObj->MaxLife);
									pCallMonsterViewportCreate.CurLifeHH = (int)lpTargetObj->Life >> 24;
									pCallMonsterViewportCreate.CurLifeHL = (int)lpTargetObj->Life >> 8;
									pCallMonsterViewportCreate.CurLifeLH = (int)lpTargetObj->Life >> 16;
									pCallMonsterViewportCreate.CurLifeLL = (int)lpTargetObj->Life;

									if (lpTargetObj->m_RecallMon >= 0 && lpTargetObj->m_RecallMon < g_ConfigRead.server.GetObjectMax() - 1)
									{
										memcpy(pCallMonsterViewportCreate.Id, gObj[lpTargetObj->m_RecallMon].Name, sizeof(pCallMonsterViewportCreate.Id));
									}
									else
									{
										memset(pCallMonsterViewportCreate.Id, 0x00, sizeof(pCallMonsterViewportCreate.Id));
									}

									pCallMonsterViewportCreate.BuffEffectCount = iViewportSize = gObjMakeViewportState(lpTargetObj, pCallMonsterViewportCreate.BuffEffectList);

									memcpy((callMonstersendBuf + callMonlOfs), (char*)&pCallMonsterViewportCreate, sizeof(PMSG_CALLMONSTER_VIEWPORTCREATE));
									callMonlOfs += sizeof(PMSG_CALLMONSTER_VIEWPORTCREATE) - (MAX_BUFFEFFECT - iViewportSize);
									callmonstercount++;;
								}
								else
								{
									pMonsterViewportCreate.NumberH = SET_NUMBERH(tObjNum);
									pMonsterViewportCreate.NumberL = SET_NUMBERL(tObjNum);

									if (lpTargetObj->m_State == 1)
									{
										pMonsterViewportCreate.NumberH |= 0x80;

										if (lpTargetObj->Teleport != 0)
										{
											pMonsterViewportCreate.NumberH |= 0x40;
										}
									}

									pMonsterViewportCreate.Type_HI = SET_NUMBERH(lpTargetObj->Class);
									pMonsterViewportCreate.Type_LO = SET_NUMBERL(lpTargetObj->Class);

									pMonsterViewportCreate.PentagramMainAttribute = lpTargetObj->m_iPentagramMainAttribute;
									pMonsterViewportCreate.LevelH = SET_NUMBERH(lpTargetObj->Level);
									pMonsterViewportCreate.LevelL = SET_NUMBERL(lpTargetObj->Level);
									pMonsterViewportCreate.MaxLifeHH = (int)(lpTargetObj->AddLife + lpTargetObj->MaxLife) >> 24;
									pMonsterViewportCreate.MaxLifeHL = (int)(lpTargetObj->AddLife + lpTargetObj->MaxLife) >> 8;
									pMonsterViewportCreate.MaxLifeLH = (int)(lpTargetObj->AddLife + lpTargetObj->MaxLife) >> 16;
									pMonsterViewportCreate.MaxLifeLL = (int)(lpTargetObj->AddLife + lpTargetObj->MaxLife);
									pMonsterViewportCreate.CurLifeHH = (int)lpTargetObj->Life >> 24;
									pMonsterViewportCreate.CurLifeHL = (int)lpTargetObj->Life >> 8;
									pMonsterViewportCreate.CurLifeLH = (int)lpTargetObj->Life >> 16;
									pMonsterViewportCreate.CurLifeLL = (int)lpTargetObj->Life;

									if (g_ConfigRead.server.GetServerType() == SERVER_CASTLE)
									{
										if (lpTargetObj->Class == 278)
										{
											if (g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
											{
												if (lpObj->m_btCsJoinSide == lpTargetObj->m_btCsJoinSide)
													pMonsterViewportCreate.Type_HI |= 0x80;
											}

											if (lpObj->m_PlayerData->lpGuild && lpTargetObj->m_PlayerData->lpGuild)
											{
												if (lpObj->m_PlayerData->lpGuild == lpTargetObj->m_PlayerData->lpGuild)
												{
													pMonsterViewportCreate.Type_HI |= 0x80;
												}
											}

											pMonsterViewportCreate.Type_HI |= ((lpTargetObj->m_btCreationState << 4) & 0x70);
										}
									}

									pMonsterViewportCreate.X = lpTargetObj->X;
									pMonsterViewportCreate.Y = lpTargetObj->Y;
									pMonsterViewportCreate.TX = lpTargetObj->TX;
									pMonsterViewportCreate.TY = lpTargetObj->TY;
									pMonsterViewportCreate.Path = lpTargetObj->Dir << 4;

									if (g_ConfigRead.server.GetServerType() == SERVER_CASTLE)
									{
										if (lpTargetObj->Class == 277)
										{
											if (lpTargetObj->m_btCsGateOpen == 0)
											{
												gObjRemoveBuffEffect(lpTargetObj, BUFFTYPE_CASTLE_DOOR_STATE);
											}
											else
											{
												gObjAddBuffEffect(lpTargetObj, BUFFTYPE_CASTLE_DOOR_STATE);
											}
										}

										if (lpTargetObj->Class == 216)
										{
											if (g_CastleSiege.GetRegCrownAvailable() == FALSE)
											{
												gObjRemoveBuffEffect(lpTargetObj, 20);
											}
											else
											{
												gObjAddBuffEffect(lpTargetObj, 20);
											}
										}
									}

									pMonsterViewportCreate.BuffEffectCount = iViewportSize = gObjMakeViewportState(lpTargetObj, pMonsterViewportCreate.BuffEffectList);

									memcpy((MonstersendBuf + MonlOfs), (char*)&pMonsterViewportCreate, sizeof(PMSG_MONSTER_VIEWPORTCREATE));
									MonlOfs += sizeof(PMSG_MONSTER_VIEWPORTCREATE) - (MAX_BUFFEFFECT - iViewportSize);
									monstercount++;
								}
							}
							break;
						case 5:
							if (lpObj->Type == OBJ_USER)
							{
								pItemViewportCreate.NumberH = SET_NUMBERH(tObjNum);
								pItemViewportCreate.NumberL = SET_NUMBERL(tObjNum);
								if(lpObj->m_bOffLevel && g_OffLevel.m_PickItemSettings.bPickUpItems)
								{
									g_OffLevel.CheckAndPickUpItem(aIndex,&MapC[lpObj->MapNumber].m_cItem[tObjNum],tObjNum);
								}
								if (MapC[lpObj->MapNumber].m_cItem[tObjNum].m_State == 1)
								{
									pItemViewportCreate.NumberH |= 0x80;
								}

								pItemViewportCreate.px = MapC[lpObj->MapNumber].m_cItem[tObjNum].px;
								pItemViewportCreate.py = MapC[lpObj->MapNumber].m_cItem[tObjNum].py;

								if (MapC[lpObj->MapNumber].m_cItem[tObjNum].m_Type == ITEMGET(14, 15))
								{
									WORD MoneyHW = SET_NUMBERHW(MapC[lpObj->MapNumber].m_cItem[tObjNum].m_BuyMoney);
									WORD MoneyLW = SET_NUMBERLW(MapC[lpObj->MapNumber].m_cItem[tObjNum].m_BuyMoney);

									pItemViewportCreate.ItemInfo[0] = BYTE(MapC[lpObj->MapNumber].m_cItem[tObjNum].m_Type) % 255;
									pItemViewportCreate.ItemInfo[1] = SET_NUMBERL(MoneyHW);
									pItemViewportCreate.ItemInfo[2] = SET_NUMBERH(MoneyLW);
									pItemViewportCreate.ItemInfo[4] = SET_NUMBERL(MoneyLW);
									pItemViewportCreate.ItemInfo[3] = 0;
									pItemViewportCreate.ItemInfo[5] = (MapC[lpObj->MapNumber].m_cItem[tObjNum].m_Type & 0x1E00) >> 5;
									pItemViewportCreate.ItemInfo[6] = 0;
									pItemViewportCreate.ItemInfo[7] = BYTE(MapC[lpObj->MapNumber].m_cItem[tObjNum].m_SocketOption[0]);
									pItemViewportCreate.ItemInfo[8] = BYTE(MapC[lpObj->MapNumber].m_cItem[tObjNum].m_SocketOption[1]);
									pItemViewportCreate.ItemInfo[9] = BYTE(MapC[lpObj->MapNumber].m_cItem[tObjNum].m_SocketOption[2]);
									pItemViewportCreate.ItemInfo[10] = BYTE(MapC[lpObj->MapNumber].m_cItem[tObjNum].m_SocketOption[3]);
									pItemViewportCreate.ItemInfo[11] = BYTE(MapC[lpObj->MapNumber].m_cItem[tObjNum].m_SocketOption[4]);
									memcpy(&ItemBuf[lOfs_Item], &pItemViewportCreate, sizeof(pItemViewportCreate));
									lOfs_Item += ItemStructSize;
								}
								else
								{
									ItemByteConvert(pItemViewportCreate.ItemInfo, (MapC[lpObj->MapNumber].m_cItem[tObjNum]));
									memcpy(&ItemBuf[lOfs_Item], &pItemViewportCreate, sizeof(pItemViewportCreate));
									lOfs_Item += ItemStructSize;
								}

								count_Item += 1;
							}
							break;
						}
					}
					lpObj->VpPlayer[n].state = 2;
				}
			}
		}
		if (lpObj->Type == OBJ_USER)
		{
			if (count > 0)
			{
				PWMSG_COUNT pCount;

				pCount.h.c = 0xC2;
				pCount.h.headcode = 0x12;
				pCount.h.sizeH = SET_NUMBERH(lOfs);
				pCount.h.sizeL = SET_NUMBERL(lOfs);
				pCount.count = count;

				memcpy(sendBuf, &pCount, sizeof(pCount));
				IOCP.DataSend(aIndex, (unsigned char *)&sendBuf, lOfs);
			}

			if (ChangeCount > 0)
			{
				PWMSG_COUNT pCount;

				pCount.h.c = 0xC2;
				pCount.h.headcode = 0x45;
				pCount.h.sizeH = SET_NUMBERH(lOfsChange);
				pCount.h.sizeL = SET_NUMBERL(lOfsChange);
				pCount.count = ChangeCount;

				memcpy(SendGBufChange, &pCount, sizeof(pCount));
				IOCP.DataSend(aIndex, (unsigned char *)&SendGBufChange, lOfsChange);
			}

			if (monstercount > 0)
			{
				PWMSG_COUNT pCount;

				pCount.h.c = 0xC2;
				pCount.h.headcode = 0x13;
				pCount.h.sizeH = SET_NUMBERH(MonlOfs);
				pCount.h.sizeL = SET_NUMBERL(MonlOfs);
				pCount.count = monstercount;

				memcpy(MonstersendBuf, &pCount, sizeof(pCount));
				IOCP.DataSend(aIndex, (unsigned char *)&MonstersendBuf, MonlOfs);
			}

			if (callmonstercount > 0)
			{
				PWMSG_COUNT pCount;

				pCount.h.c = 0xC2;
				pCount.h.headcode = 0x1F;
				pCount.h.sizeH = SET_NUMBERH(callMonlOfs);
				pCount.h.sizeL = SET_NUMBERL(callMonlOfs);
				pCount.count = callmonstercount;

				memcpy(callMonstersendBuf, &pCount, sizeof(pCount));
				IOCP.DataSend(aIndex, (unsigned char *)&callMonstersendBuf, callMonlOfs);
			}

			if (count_Item > 0)
			{
				PWMSG_COUNT pCount;

				pCount.h.c = 0xC2;
				pCount.h.headcode = 0x20;
				pCount.count = count_Item;
				pCount.h.sizeH = SET_NUMBERH(lOfs_Item);
				pCount.h.sizeL = SET_NUMBERL(lOfs_Item);

				memcpy(ItemBuf, &pCount, sizeof(pCount));
				IOCP.DataSend(aIndex, (unsigned char *)&ItemBuf, lOfs_Item);
			}

			if (GuildInfoCount != 0)
			{
				PWMSG_COUNT pCount;

				pCount.h.headcode = 0x65;
				pCount.h.c = 0xC2;
				pCount.h.sizeH = SET_NUMBERH(GuildInfoOfs);
				pCount.h.sizeL = SET_NUMBERL(GuildInfoOfs);
				pCount.count = GuildInfoCount;

				memcpy(GuildInfoBuf, &pCount, sizeof(pCount));
				IOCP.DataSend(aIndex, (unsigned char *)&GuildInfoBuf, GuildInfoOfs);
			}

			if (GensInfoCount != 0)
			{
				_tagPMSG_SEND_GENS_MEMBER_VIEWPORT pGensCount;
				pGensCount.h.c = 0xC2;
				pGensCount.h.headcode = 0xF8;
				pGensCount.h.subcode = 0x05;
				pGensCount.h.sizeH = SET_NUMBERH(GensInfoOfs);
				pGensCount.h.sizeL = SET_NUMBERL(GensInfoOfs);
				pGensCount.Count = GensInfoCount;

				memcpy(GensInfoBuf, &pGensCount, sizeof(pGensCount));
				IOCP.DataSend(aIndex, (unsigned char *)&GensInfoBuf, GensInfoOfs);
			}
		}
	}
}

void gObjTeleportMagicUse(int aIndex, BYTE x, BYTE y)	// Check if x and y are BYTE
{

	LPOBJ lpObj = &gObj[aIndex];

	if ( MAX_MAP_RANGE(lpObj->MapNumber) == FALSE )
	{
		return;
	}

	if(lpObj->Teleport != 0)
	{
		return;
	}

	lpObj->TeleportTime = GetTickCount();
	lpObj->PathCount = 0;
	lpObj->Teleport = 1;
	lpObj->m_ViewState = 1;

	lpObj->X = x;
	lpObj->Y = y;
	lpObj->TX = x;
	lpObj->TY = y;

	MapC[lpObj->MapNumber].ClearStandAttr(lpObj->m_OldX,lpObj->m_OldY);
	MapC[lpObj->MapNumber].SetStandAttr(lpObj->TX,lpObj->TY);

	lpObj->m_OldX = lpObj->TX;
	lpObj->m_OldY = lpObj->TY;

	gObjViewportListProtocolDestroy(lpObj);
}




BOOL gObjMoveGate(int aIndex, int gt)
{
	if(!ObjectMaxRange(aIndex))
		return FALSE;

	LPOBJ lpObj = &gObj[aIndex];
	int result;
	short x,y;
	BYTE mapNumber = 0,dir;
	short level;
	int userlevel = gObj[aIndex].Level;
	int movefail = 0;

	if(gObj[aIndex].m_SkillInfo.LordSummonTime != 0)
	{
		char szMsg[255];
		gObj[aIndex].m_SkillInfo.LordSummonTime = 0;
		wsprintf(szMsg,Lang.GetText(0,146));
		GSProtocol.GCServerMsgStringSend(szMsg,aIndex,1);
	}

	if (lpObj->Type == OBJ_USER && lpObj->m_PlayerData->ISBOT == false && lpObj->m_Index >= g_ConfigRead.server.GetObjectStartUserIndex())
	{
		gDarkSpirit[lpObj->m_Index- g_ConfigRead.server.GetObjectStartUserIndex()].SetMode(CDarkSpirit::ePetItem_Mode::PetItem_Mode_Normal,-1);
	}

	if(BC_MAP_RANGE(lpObj->MapNumber))
	{
		int iBridgeIndex = g_BloodCastle.GetBridgeIndex(lpObj->MapNumber);

		if(g_BloodCastle.GetCurrentState(iBridgeIndex) == BC_STATE_PLAYING)
		{
			g_BloodCastle.SearchUserDropQuestItem(lpObj->m_Index);
		}
		else
		{
			g_BloodCastle.SearchUserDeleteQuestItem(lpObj->m_Index);
		}
	}

	if (lpObj->Type == OBJ_USER && lpObj->m_PlayerData->m_bIsMining)
	{
		g_MineSystem.CheckMoveMapWhileMining(aIndex);
	}

	if(gObj[aIndex].RegenOk != 0)
	{
		g_Log.Add("Regen OK %s %d",__FILE__,__LINE__);
		x = lpObj->X;
		y = lpObj->Y;
		mapNumber = lpObj->MapNumber;
		dir = lpObj->Dir;
		movefail = 1;

		lpObj->RegenMapNumber = lpObj->MapNumber;
		lpObj->RegenMapX = x;
		lpObj->RegenMapY = y;

		gObjClearViewport(lpObj);

		GSProtocol.GCTeleportSend(&gObj[aIndex],gt,mapNumber,gObj[aIndex].X,gObj[aIndex].Y,gObj[aIndex].Dir);

		if(lpObj->m_Change >= 0)
		{
			gObjViewportListProtocolCreate(&gObj[aIndex]);
		}

		gObj[aIndex].RegenOk = 1;

		return false;
	}

	if(gGateC.IsGate(gt) == false)
	{

		g_Log.Add("Cannnot find gate number %d (%s %d)",gt,__FILE__,__LINE__);

		x = lpObj->X;
		y = lpObj->Y;
		mapNumber = lpObj->MapNumber;
		dir = lpObj->Dir;
		movefail = 1;

		lpObj->RegenMapNumber = lpObj->MapNumber;
		lpObj->RegenMapX = x;
		lpObj->RegenMapY = y;

		gObjClearViewport(lpObj);

		GSProtocol.GCTeleportSend(&gObj[aIndex],gt,mapNumber,gObj[aIndex].X,gObj[aIndex].Y,gObj[aIndex].Dir);

		if(lpObj->m_Change >= 0)
		{
			gObjViewportListProtocolCreate(&gObj[aIndex]);
		}

		gObj[aIndex].RegenOk = 1;

		return false;
	}

	if(gt >= 58 && gt <= 61)
	{
		if(gObj[aIndex].m_bDevilSquareAuth == 0)
		{
			g_Log.Add("[DevilSquare] [%s][%s] Not Used DevilSquare Invitation In DevilSquare",gObj[aIndex].AccountID,gObj[aIndex].Name);
			return false;
		}
		else
		{
			gObj[aIndex].m_bDevilSquareAuth = 0;
		}
	}

	result = gGateC.GetGate(gt,(short &)x,(short &)y,(BYTE &)mapNumber,(BYTE &)dir,(short &)level);

	if(result < 0)
	{
		g_Log.Add("error : %d (%s %d)",result,__FILE__,__LINE__);

		x = lpObj->X;
		y = lpObj->Y;
		mapNumber = lpObj->MapNumber;
		dir = lpObj->Dir;

		movefail = 1;
	}

	if ( mapNumber == MAP_INDEX_ACHERON || mapNumber == MAP_INDEX_DEBENTER )
	{
		BOOL bIsABOccupyZone = g_ArcaBattle.IsArcaBattleOccupyZone(lpObj, gt);

		if ( bIsABOccupyZone == TRUE )
		{
			if ( gObjCheckUsedBuffEffect(lpObj, 179) || gObjCheckUsedBuffEffect(lpObj, 180) || gObjCheckUsedBuffEffect(lpObj, 181) || gObjCheckUsedBuffEffect(lpObj, 182) || gObjCheckUsedBuffEffect(lpObj, 183) )
			{
				gObjAddBuffEffect(lpObj, 187, 0, 0, 0, 0, -10);
			}

			else
			{
				if(gObjCheckUsedBuffEffect(lpObj, 187))
				{
					gObjRemoveBuffEffect(lpObj, 187);
				}
			}
		}

		g_ArcaBattle.SendArcaBattleOccupyZone(lpObj, gt);
	}

	else
	{
		if(gObjCheckUsedBuffEffect(lpObj, 187))
		{
			gObjRemoveBuffEffect(lpObj, 187);
		}
	}

	if ( g_GensSystem.IsMapBattleZone(lpObj->MapNumber) == TRUE && lpObj->Type == OBJ_USER && GetTickCount() - lpObj->m_PlayerData->m_dwGensHitTeleportDelay < (g_GensSystem.GetGensAttackWarpDelay() * 1000) )
	{
		x = lpObj->X;
		y = lpObj->Y;
		mapNumber = lpObj->MapNumber;
		dir = lpObj->Dir;

		movefail = 1;

		lpObj->RegenMapNumber = lpObj->MapNumber;
		lpObj->RegenMapX = x;
		lpObj->RegenMapY = y;

		gObjClearViewport(lpObj);
		GSProtocol.GCTeleportSend(&gObj[aIndex],gt,mapNumber,gObj[aIndex].X,gObj[aIndex].Y,gObj[aIndex].Dir);
	
		if(lpObj->m_Change >= 0)
		{
			gObjViewportListProtocolCreate(&gObj[aIndex]);
		}
	
		gObj[aIndex].RegenOk = 1;

		return FALSE;
	}

	else
	{
		if (lpObj->Type == OBJ_USER)
		{
			lpObj->m_PlayerData->m_dwGensHitTeleportDelay = 0;
		}
	}

	if ( g_GensSystem.IsMapBattleZone(mapNumber) && (g_GensSystem.IsUserBattleZoneEnable(lpObj) == FALSE || g_GensSystem.GetEntryAllowType(lpObj, mapNumber) == false) )
	{
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,414), aIndex, 1);

		x = lpObj->X;
		y = lpObj->Y;
		mapNumber = lpObj->MapNumber;
		dir = lpObj->Dir;

		movefail = 1;

		lpObj->RegenMapNumber = lpObj->MapNumber;
		lpObj->RegenMapX = x;
		lpObj->RegenMapY = y;

		gObjClearViewport(lpObj);
		GSProtocol.GCTeleportSend(&gObj[aIndex],gt,mapNumber,gObj[aIndex].X,gObj[aIndex].Y,gObj[aIndex].Dir);
	
		if(lpObj->m_Change >= 0)
		{
			gObjViewportListProtocolCreate(&gObj[aIndex]);
		}
	
		gObj[aIndex].RegenOk = 1;

		return FALSE;
	}

	if(lpObj->m_bPShopOpen == 1)
	{
		int bMove = 1;

		if(lpObj->MapNumber == MAP_INDEX_CASTLESIEGE && mapNumber == MAP_INDEX_RORENCIA)
		{
			bMove = 0;
		}

		if(lpObj->MapNumber == MAP_INDEX_RORENCIA && mapNumber == MAP_INDEX_CASTLESIEGE)
		{
			bMove = 0;
		}

		if(bMove == 0)
		{
			g_Log.Add("[PSHOP ITEM CLONNING] Move Failed : %d (%s %s)",result,lpObj->AccountID,lpObj->Name);

			x = lpObj->X;
			y = lpObj->Y;
			mapNumber = lpObj->MapNumber;
			dir = lpObj->Dir;
			movefail = 1;

			lpObj->RegenMapX = x;
			lpObj->RegenMapY = y;

			lpObj->RegenMapNumber = mapNumber;
			lpObj->RegenMapNumber = lpObj->MapNumber;

			lpObj->RegenMapX = x;
			lpObj->RegenMapY = y;
	
			gObjClearViewport(&gObj[aIndex]);
	
			GSProtocol.GCTeleportSend(&gObj[aIndex],gt,mapNumber,gObj[aIndex].X,gObj[aIndex].Y,gObj[aIndex].Dir);
	
			if(lpObj->m_Change >= 0)
			{
				gObjViewportListProtocolCreate(&gObj[aIndex]);
			}
	
			gObj[aIndex].RegenOk = 1;
		return false;
		}
	}

	if(BC_MAP_RANGE(mapNumber)
		|| CC_MAP_RANGE(mapNumber)
		|| DS_MAP_RANGE(mapNumber)
		|| IT_MAP_RANGE(mapNumber)
		|| DG_MAP_RANGE(mapNumber)
		|| IMPERIAL_MAP_RANGE(mapNumber)
		|| mapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL
		|| mapNumber == MAP_INDEX_DEVILSQUARE_FINAL)
	{
		if(gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_ATTDEF_POWER_INC) != NULL)
		{
			gObjRemoveBuffEffect(lpObj, BUFFTYPE_ATTDEF_POWER_INC);
		}

		if (gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_ATTACK_POWER_INC) != NULL)
		{
			gObjRemoveBuffEffect(lpObj, BUFFTYPE_ATTACK_POWER_INC);
		}

		if (gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_DEFENSE_POWER_INC) != NULL)
		{
			gObjRemoveBuffEffect(lpObj, BUFFTYPE_DEFENSE_POWER_INC);
		}

		if (gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_CRITICAL_DMG_INC) != NULL)
		{
			gObjRemoveBuffEffect(lpObj, BUFFTYPE_CRITICAL_DMG_INC);
		}

		if (gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_HP_INC) != NULL)
		{
			gObjRemoveBuffEffect(lpObj, BUFFTYPE_HP_INC);
		}

		if (gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_MELEE_DEFENSE_INC) != NULL)
		{
			gObjRemoveBuffEffect(lpObj, BUFFTYPE_MELEE_DEFENSE_INC);
		}

		if (gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_HP_INC_STR) != NULL)
		{
			gObjRemoveBuffEffect(lpObj, BUFFTYPE_HP_INC_STR);
		}
		
		if (gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_HP_INC_MAS) != NULL)
		{
			gObjRemoveBuffEffect(lpObj, BUFFTYPE_HP_INC_MAS);
		}

		if (gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_MELEE_DEFENSE_INC_STR) != NULL)
		{
			gObjRemoveBuffEffect(lpObj, BUFFTYPE_MELEE_DEFENSE_INC_STR);
		}
	}

	if(IT_MAP_RANGE(lpObj->MapNumber))
	{
		if (g_ConfigRead.server.GetServerType() == SERVER_MARKET && mapNumber == MAP_INDEX_LORENMARKET)
		{
			g_IT_Event.DropRelicsItem(lpObj->MapNumber, lpObj->m_Index);
			g_IT_Event.Leave_ITR(lpObj->m_Index, lpObj->MapNumber);

			if (lpObj->m_nITR_RelicsTick > 0 || lpObj->m_wITR_NpcType > 0 || lpObj->m_byITR_StoneState != 99 || lpObj->m_bITR_RegisteringRelics == true || lpObj->m_bITR_GettingRelics == true)
			{
				g_IT_Event.CancleStoneState(lpObj->m_wITR_NpcType, lpObj->m_byITR_StoneState, lpObj->MapNumber);
				lpObj->m_nITR_RelicsTick = 0;
				lpObj->m_wITR_NpcType = 0;
				lpObj->m_byITR_StoneState = 99;
				lpObj->m_bITR_RegisteringRelics = 0;
				lpObj->m_bITR_GettingRelics = 0;
			}
		}
	}

	if (lpObj->MapNumber == MAP_INDEX_DEVILSQUARE_FINAL && g_ConfigRead.server.GetServerType() == SERVER_BATTLECORE && mapNumber != MAP_INDEX_DEVILSQUARE_FINAL)
	{
		g_DevilSquareFinal.Leave_DSF(lpObj->m_Index);
	}

	if(DG_MAP_RANGE(lpObj->MapNumber) && mapNumber != lpObj->MapNumber)
	{
		g_DoppelGanger.LeaveDoppelganger(lpObj->m_Index);

		if ( g_DoppelGanger.GetDoppelgangerState() == 2 )
		{
			g_DoppelGanger.SendDoppelgangerResult(lpObj, 1);
		}

		if ( lpObj->PartyNumber >= 0 )
		{
			int nPartyNumber = lpObj->PartyNumber;

			if ( gParty.GetCount(nPartyNumber) <= 1 )
			{
				for ( int i=0;i<5;i++ )
				{
					int index = gParty.m_PartyS[nPartyNumber].Number[i];

					if ( index >= 0 )
					{
						gParty.Delete(nPartyNumber, index);

						gObj[index].PartyNumber = -1;
						gObj[index].PartyTargetUser = -1;

						if ( gObj[index].Connected == PLAYER_PLAYING )
							GSProtocol.GCPartyDelUserSend(index);
					}
				}
			}

			else
			{
				GSProtocol.CGPartyListAll(nPartyNumber);
			}
		}
	}

	if(IMPERIAL_MAP_RANGE(lpObj->MapNumber) && mapNumber != lpObj->MapNumber && lpObj->PartyNumber >= 0)
	{
		int party_number = lpObj->PartyNumber;
		
		if(gParty.GetCount(party_number) <= 1)
		{
			for (int index=0;index<MAX_USER_IN_PARTY;index++)
			{
				int userindex = gParty.m_PartyS[party_number].Number[index];

				if (userindex >= 0)
				{
					gParty.Delete(party_number, index);
					gObj[userindex].PartyNumber = -1;
					gObj[userindex].PartyTargetUser = -1;
					
					if(gObj[userindex].Connected == PLAYER_PLAYING)
					{
						GSProtocol.GCPartyDelUserSend(userindex);
					}
				}
			}

			gParty.Destroy(party_number);
		}

		else
		{
			GSProtocol.CGPartyListAll(party_number);
		}
	}

	if (g_ConfigRead.server.GetServerType() == SERVER_CASTLE)
	{
		if (result == 97)
		{
			if (g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
			{
				if (lpObj->m_btCsJoinSide != 1 && g_CastleSiege.GetCastleTowerAccessable() == FALSE)
				{
					GSProtocol.GCServerMsgStringSend(Lang.GetText(0,167), lpObj->m_Index, 1);

					x = lpObj->X;
					y = lpObj->Y;
					mapNumber = lpObj->MapNumber;
					dir = lpObj->Dir;
					movefail = 1;

					lpObj->RegenMapNumber = lpObj->MapNumber;
					lpObj->RegenMapX = x;
					lpObj->RegenMapY = y;

					gObjClearViewport(&gObj[aIndex]);

					GSProtocol.GCTeleportSend(&gObj[aIndex], gt, mapNumber, gObj[aIndex].X, gObj[aIndex].Y, gObj[aIndex].Dir);

					if (lpObj->m_Change >= 0)
					{
						gObjViewportListProtocolCreate(&gObj[aIndex]);
					}

					gObj[aIndex].RegenOk = 1;
					return false;
				}

				if (lpObj->m_btCsJoinSide == 0)
				{
					GSProtocol.GCServerMsgStringSend(Lang.GetText(0,168), lpObj->m_Index, 1);

					x = lpObj->X;
					y = lpObj->Y;
					mapNumber = lpObj->MapNumber;
					dir = lpObj->Dir;
					movefail = 1;

					lpObj->RegenMapNumber = lpObj->MapNumber;
					lpObj->RegenMapX = x;
					lpObj->RegenMapY = y;

					gObjClearViewport(&gObj[aIndex]);

					GSProtocol.GCTeleportSend(&gObj[aIndex], gt, mapNumber, gObj[aIndex].X, gObj[aIndex].Y, gObj[aIndex].Dir);

					if (lpObj->m_Change >= 0)
					{
						gObjViewportListProtocolCreate(&gObj[aIndex]);
					}

					gObj[aIndex].RegenOk = 1;
					return false;
				}
			}
			else if (g_CastleSiege.CheckCastleOwnerUnionMember(lpObj->m_Index) == FALSE && g_CastleSiege.m_TowerEntryWithoutAlliance == false)
			{
				GSProtocol.GCServerMsgStringSend(Lang.GetText(0,169), lpObj->m_Index, 1);

				x = lpObj->X;
				y = lpObj->Y;
				mapNumber = lpObj->MapNumber;
				dir = lpObj->Dir;
				movefail = 1;

				lpObj->RegenMapNumber = lpObj->MapNumber;
				lpObj->RegenMapX = x;
				lpObj->RegenMapY = y;

				gObjClearViewport(&gObj[aIndex]);

				GSProtocol.GCTeleportSend(&gObj[aIndex], gt, mapNumber, gObj[aIndex].X, gObj[aIndex].Y, gObj[aIndex].Dir);

				if (lpObj->m_Change >= 0)
				{
					gObjViewportListProtocolCreate(&gObj[aIndex]);
				}

				gObj[aIndex].RegenOk = 1;
				return false;
			}
		}
	}

	if(mapNumber == MAP_INDEX_ATHLANSE)
	{
		if(lpObj->m_btInvenPetPos != 0 && lpObj->m_wInvenPet != (WORD)-1)
		{
			if(lpObj->pInventory[lpObj->m_btInvenPetPos].m_Type == ITEMGET(13,2))
			{
				GSProtocol.GCServerMsgStringSend(Lang.GetText(0,3),lpObj->m_Index,1);

				x = lpObj->X;
				y = lpObj->Y;
				mapNumber = lpObj->MapNumber;
				dir = lpObj->Dir;
				movefail = 1;

				lpObj->RegenMapNumber = lpObj->MapNumber;
				lpObj->RegenMapX = x;
				lpObj->RegenMapY = y;
				gObjClearViewport(&gObj[aIndex]);
				GSProtocol.GCTeleportSend(&gObj[aIndex],gt,mapNumber,gObj[aIndex].X,gObj[aIndex].Y,gObj[aIndex].Dir);
				if(lpObj->m_Change >= 0)
				{
					gObjViewportListProtocolCreate(&gObj[aIndex]);
				}
				gObj[aIndex].RegenOk = 1;
				return false;
			}

			if(lpObj->pInventory[lpObj->m_btInvenPetPos].m_Type == ITEMGET(13,3))
			{
				GSProtocol.GCServerMsgStringSend(Lang.GetText(0,153),lpObj->m_Index,1);

				x = lpObj->X;
				y = lpObj->Y;
				mapNumber = lpObj->MapNumber;
				dir = lpObj->Dir;
				movefail = 1;

				lpObj->RegenMapNumber = lpObj->MapNumber;
				lpObj->RegenMapX = x;
				lpObj->RegenMapY = y;
				gObjClearViewport(&gObj[aIndex]);
				GSProtocol.GCTeleportSend(&gObj[aIndex],gt,mapNumber,gObj[aIndex].X,gObj[aIndex].Y,gObj[aIndex].Dir);
				if(lpObj->m_Change >= 0)
				{
					gObjViewportListProtocolCreate(&gObj[aIndex]);
				}
				gObj[aIndex].RegenOk = 1;
				return false;
			}
		}
	}

	if(mapNumber == MAP_INDEX_ICARUS)
	{
		if(!(lpObj->pInventory[lpObj->m_btInvenPetPos].m_Type == ITEMGET(13,3)
			|| lpObj->pInventory[7].m_Type == ITEMGET(13,30)
			|| lpObj->pInventory[lpObj->m_btInvenPetPos].m_Type == ITEMGET(13,37)
			|| lpObj->pInventory[7].m_Type == ITEMGET(12,49)
			|| lpObj->pInventory[7].m_Type == ITEMGET(12,50)
			|| (lpObj->pInventory[7].m_Type >= ITEMGET(12,0) && lpObj->pInventory[7].m_Type <=ITEMGET(12,6)) 
			|| (lpObj->pInventory[7].m_Type >= ITEMGET(12,36) && lpObj->pInventory[7].m_Type <=ITEMGET(12,43))
			|| (lpObj->pInventory[7].m_Type >= ITEMGET(12,130) && lpObj->pInventory[7].m_Type <=ITEMGET(12,135))
			|| (lpObj->pInventory[7].m_Type >= ITEMGET(12,262) && lpObj->pInventory[7].m_Type <=ITEMGET(12,265))
			|| lpObj->pInventory[7].m_Type == ITEMGET(12,266)
			|| lpObj->pInventory[7].m_Type == ITEMGET(12,267)
			|| lpObj->pInventory[7].m_Type == ITEMGET(12,268)
			|| lpObj->pInventory[lpObj->m_btInvenPetPos].m_Type == ITEMGET(13,2)
			|| lpObj->pInventory[11].m_Type == ITEMGET(13,10)
			|| lpObj->pInventory[10].m_Type == ITEMGET(13,10)
			|| lpObj->pInventory[10].m_Type == ITEMGET(13,39)
			|| lpObj->pInventory[11].m_Type == ITEMGET(13,39)
			|| lpObj->pInventory[10].m_Type == ITEMGET(13,40)
			|| lpObj->pInventory[11].m_Type == ITEMGET(13,40)))
		{
			GSProtocol.GCServerMsgStringSend(Lang.GetText(0,153),lpObj->m_Index,1);

			x = lpObj->X;
			y = lpObj->Y;
			mapNumber = lpObj->MapNumber;
			dir = lpObj->Dir;
			movefail = 1;
			lpObj->RegenMapNumber = lpObj->MapNumber;
			lpObj->RegenMapX = x;
			lpObj->RegenMapY = y;
			
			GSProtocol.GCTeleportSend(&gObj[aIndex],gt,mapNumber,gObj[aIndex].X,gObj[aIndex].Y,gObj[aIndex].Dir);
			gObjClearViewportOfMine(&gObj[aIndex]);

			if(lpObj->m_Change >= 0)
			{
				gObjViewportListProtocolCreate(&gObj[aIndex]);
			}
			gObj[aIndex].RegenOk = 1;
			return false;
		}

		if(lpObj->m_RecallMon >= 0)
		{
			GSProtocol.GCRecallMonLife(aIndex,60,0);
			gObjMonsterCallKill(aIndex);
		}
	}

	if (mapNumber == MAP_INDEX_HATCHERY) //season4 add-on
	{
		if (g_Raklion.GetRaklionState() >= RAKLION_STATE_CLOSE_DOOR)
		{
			GSProtocol.GCServerMsgStringSend(Lang.GetText(0,297), lpObj->m_Index, 1);
			x = lpObj->X;
			y = lpObj->Y;
			mapNumber = lpObj->MapNumber;
			dir = lpObj->Dir;
			movefail = 1;

			lpObj->RegenMapNumber = lpObj->MapNumber;
			lpObj->RegenMapX = x;
			lpObj->RegenMapY = y;

			GSProtocol.GCTeleportSend(&gObj[aIndex], gt, mapNumber, gObj[aIndex].X, gObj[aIndex].Y, gObj[aIndex].Dir);
			gObjClearViewportOfMine(lpObj);

			if (lpObj->m_Change >= 0)
			{
				gObjViewportListProtocolCreate(&gObj[aIndex]);
			}
			gObj[aIndex].RegenOk = 1;
			return false;
		}

		if (gt == 290)
		{
			g_RaklionBattleUserMng.AddUserData(aIndex);
		}
	}

	if (mapNumber == MAP_INDEX_RAKLION) //season4.0 add-on
	{
		if (gt == 292)
		{
			if (g_Raklion.GetRaklionState() >= RAKLION_STATE_CLOSE_DOOR)
			{
				GSProtocol.GCServerMsgStringSend(Lang.GetText(0,298), lpObj->m_Index, 1);
				x = lpObj->X;
				y = lpObj->Y;
				mapNumber = lpObj->MapNumber;
				dir = lpObj->Dir;
				movefail = 1;

				lpObj->RegenMapNumber = lpObj->MapNumber;
				lpObj->RegenMapX = x;
				lpObj->RegenMapY = y;

				GSProtocol.GCTeleportSend(&gObj[aIndex], gt, mapNumber, gObj[aIndex].X, gObj[aIndex].Y, gObj[aIndex].Dir);
				gObjClearViewportOfMine(lpObj);

				if (lpObj->m_Change >= 0)
				{
					gObjViewportListProtocolCreate(&gObj[aIndex]);
				}
				gObj[aIndex].RegenOk = 1;
				return false;
			}
			g_RaklionBattleUserMng.DeleteUserData(aIndex);
		}
		else
		{
			g_RaklionUtil.NotifyRaklionCurrentState(aIndex, g_Raklion.GetRaklionState(), g_Raklion.GetRaklionStateDetail());
		}
	}

	if(!KALIMA_MAP_RANGE(mapNumber) && KALIMA_MAP_RANGE(lpObj->MapNumber))
	{
		int iKalimaGateIndex = lpObj->m_iKalimaGateIndex;

		if(g_KalimaGate.DeleteKalimaGate(lpObj->m_Index))
		{
			g_Log.Add("[Kalima] [%s][%s] Kalima Gate Vanished - User MoveGate (SummonIndex:%d, EnterCount:%d)",lpObj->AccountID,lpObj->Name,iKalimaGateIndex,gObj[iKalimaGateIndex].m_cKalimaGateEnterCount);
		}
	}

	if(lpObj->Type == OBJ_USER && g_MapAttr.CheckVIP(mapNumber) > lpObj->m_PlayerData->VipType)
	{
		MsgOutput(aIndex, Lang.GetText(0,415));
		x = lpObj->X;
		y = lpObj->Y;
		mapNumber = lpObj->MapNumber;
		dir = lpObj->Dir;
		movefail = 1;

		lpObj->RegenMapNumber = lpObj->MapNumber;
		lpObj->RegenMapX = x;
		lpObj->RegenMapY = y;

		GSProtocol.GCTeleportSend(&gObj[aIndex], gt, mapNumber, gObj[aIndex].X, gObj[aIndex].Y, gObj[aIndex].Dir);
		gObjClearViewportOfMine(lpObj);

		if (lpObj->m_Change >= 0)
		{
			gObjViewportListProtocolCreate(&gObj[aIndex]);
		}

		gObj[aIndex].RegenOk = 1;
		return false;
	}

	if (lpObj->Type == OBJ_USER && lpObj->m_PlayerData->m_bUsePartyMatching == true)
	{
		if (mapNumber == MAP_INDEX_ARCA_WAR ||
			mapNumber == MAP_INDEX_DEBENTER_ARCA_WAR ||
			mapNumber == MAP_INDEX_VULCAN ||
			mapNumber == MAP_INDEX_CHAOSCASTLE1 ||
			mapNumber == MAP_INDEX_CHAOSCASTLE2 ||
			mapNumber == MAP_INDEX_CHAOSCASTLE3 ||
			mapNumber == MAP_INDEX_CHAOSCASTLE4 ||
			mapNumber == MAP_INDEX_CHAOSCASTLE5 ||
			mapNumber == MAP_INDEX_CHAOSCASTLE6 ||
			mapNumber == MAP_INDEX_CHAOSCASTLE7)
		{
			x = lpObj->X;
			y = lpObj->Y;
			mapNumber = lpObj->MapNumber;
			dir = lpObj->Dir;
			movefail = 1;

			lpObj->RegenMapNumber = lpObj->MapNumber;
			lpObj->RegenMapX = x;
			lpObj->RegenMapY = y;

			gObjClearViewport(&gObj[aIndex]);
			GSProtocol.GCTeleportSend(&gObj[aIndex], gt, mapNumber, gObj[aIndex].X, gObj[aIndex].Y, gObj[aIndex].Dir);

			PMSG_ANS_PARTYMATCHING_ERROR pMsg;
			PHeadSubSetB((LPBYTE)&pMsg, 0xEF, 0x09, sizeof(pMsg));

			pMsg.iResult = -1;
			IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
			return FALSE;
		}
	}

	if (g_ConfigRead.server.GetServerType() != SERVER_CASTLE)
	{
		if (mapNumber == MAP_INDEX_KANTURU_BOSS)
		{
			if (!(lpObj->pInventory[lpObj->m_btInvenPetPos].m_Type == ITEMGET(13, 3)
				|| lpObj->pInventory[7].m_Type == ITEMGET(13, 30)
				|| lpObj->pInventory[lpObj->m_btInvenPetPos].m_Type == ITEMGET(13, 37)
				|| lpObj->pInventory[7].m_Type == ITEMGET(12, 49)
				|| lpObj->pInventory[7].m_Type == ITEMGET(12, 50)
				|| (lpObj->pInventory[7].m_Type >= ITEMGET(12, 0) && lpObj->pInventory[7].m_Type <= ITEMGET(12, 6))
				|| (lpObj->pInventory[7].m_Type >= ITEMGET(12, 262) && lpObj->pInventory[7].m_Type <= ITEMGET(12, 265))
				|| lpObj->pInventory[7].m_Type == ITEMGET(12, 266)
				|| lpObj->pInventory[7].m_Type == ITEMGET(12, 267)
				|| (lpObj->pInventory[7].m_Type >= ITEMGET(12,36) && lpObj->pInventory[7].m_Type <=ITEMGET(12,43)) )
				|| lpObj->pInventory[11].m_Type == ITEMGET(13,10)
				|| lpObj->pInventory[10].m_Type == ITEMGET(13,10)
				|| lpObj->pInventory[11].m_Type == ITEMGET(13,39)
				|| lpObj->pInventory[10].m_Type == ITEMGET(13,39)
				|| lpObj->pInventory[11].m_Type == ITEMGET(13,40)
				|| lpObj->pInventory[10].m_Type == ITEMGET(13,40))
			{
				GSProtocol.GCServerMsgStringSend(Lang.GetText(0,153),lpObj->m_Index,1);

				x = lpObj->X;
				y = lpObj->Y;
				mapNumber = lpObj->MapNumber;
				dir = lpObj->Dir;
				movefail = 1;

				lpObj->RegenMapNumber = lpObj->MapNumber;
				lpObj->RegenMapX = x;
				lpObj->RegenMapY = y;
			
				GSProtocol.GCTeleportSend(&gObj[aIndex],gt,mapNumber,gObj[aIndex].X,gObj[aIndex].Y,gObj[aIndex].Dir);
				gObjClearViewportOfMine(&gObj[aIndex]);

				if(lpObj->m_Change >= 0)
				{
					gObjViewportListProtocolCreate(&gObj[aIndex]);
				}

				gObj[aIndex].RegenOk = 1;
				return false;
			}
		}
	}

	x = lpObj->X;
	y = lpObj->Y;
	mapNumber = lpObj->MapNumber;
	dir = lpObj->Dir;
	level = lpObj->Level;

	if(gGateC.GetGate(gt,(short &)x,(short &)y,(BYTE &)mapNumber,(BYTE &)dir,(short &)level) != -1)
	{
		if(g_MapAttr.CheckBlockEntry(mapNumber) == true)
		{
			GSProtocol.GCServerMsgStringSend(Lang.GetText(0,153),lpObj->m_Index,1);
			x = lpObj->X;
			y = lpObj->Y;
			mapNumber = lpObj->MapNumber;
			dir = lpObj->Dir;
			movefail = 1;

			lpObj->RegenMapNumber = lpObj->MapNumber;
			lpObj->RegenMapX = x;
			lpObj->RegenMapY = y;
			
			GSProtocol.GCTeleportSend(&gObj[aIndex],gt,mapNumber,gObj[aIndex].X,gObj[aIndex].Y,gObj[aIndex].Dir);
			gObjClearViewportOfMine(&gObj[aIndex]);

			if(lpObj->m_Change >= 0)
			{
				gObjViewportListProtocolCreate(&gObj[aIndex]);
			}
		
			gObj[aIndex].RegenOk = 1;
			return false;
		}
	}

	if(lpObj->Class == 4 || lpObj->Class == 3 || lpObj->Class == 6)
	{
		if(level > 0)
		{
			level = (level / 3)*2;
		}
	}

	if(level != 0)
	{
		if(userlevel < level)
		{
			char szTemp[256];
			wsprintf(szTemp,Lang.GetText(0,30),level);
			GSProtocol.GCServerMsgStringSend(szTemp,aIndex,1);

			x = lpObj->X;
			y = lpObj->Y;
			mapNumber = lpObj->MapNumber;
			dir = lpObj->Dir;
			movefail = 1;
		}
	}

	if(gObj[aIndex].m_IfState.use != 0)
	{
		x = lpObj->X;
		y = lpObj->Y;
		mapNumber = lpObj->MapNumber;
		dir = lpObj->Dir;
		movefail = 1;
	}

	if(gObj[aIndex].DieRegen != 0)
	{
		x = lpObj->X;
		y = lpObj->Y;
		mapNumber = lpObj->MapNumber;
		dir = lpObj->Dir;
		movefail = 1;
	}

	if(gGateC.GetGate(gt, (short &)x,(short &)y,(BYTE &)mapNumber,(BYTE &)dir,(short &)level) != -1)
	{
		if(lpObj->m_Change == 404 || lpObj->m_Change == 405) // iluzje z IT
		{
			if(!IT_MAP_RANGE(mapNumber))
			{
				lpObj->m_Change = -1;
				gObjClearViewport(lpObj);
				gObjViewportListProtocolCreate(lpObj);
			}
		}
	}

	if(movefail == 0)
	{
		if(gGateC.GetGate(gt,(short &)x,(short &)y,(BYTE &)mapNumber,(BYTE &)dir,(short &)level) != -1)
		{
			if(gObj[aIndex].MapNumber != mapNumber)
			{
				if(MapNumberCheck(mapNumber)== 0)
				{
					g_Log.Add("error-L3 : %s %d",__FILE__,__LINE__);
					return false;
				}

				short sSvrCode = g_MapServerManager.CheckMoveMapSvr(lpObj->m_Index,mapNumber,lpObj->m_sPrevMapSvrCode);

				if (sSvrCode != g_ConfigRead.server.GetGameServerCode())
				{
					if (sSvrCode == -1)
					{
						g_Log.AddC(TColor::Red, "[MapServerMng] Map Server Move Fail : CheckMoveMapSvr() == -1 [%s][%s] (%d)", lpObj->AccountID, lpObj->Name, lpObj->m_Index);
						return false;
					}

					if (gGateC.CheckGateLevel(aIndex, gt) == 0)
					{
						x = lpObj->X;
						y = lpObj->Y;
						mapNumber = lpObj->MapNumber;
						dir = lpObj->Dir;
						movefail = 1;

						lpObj->RegenMapNumber = lpObj->MapNumber;

						lpObj->RegenMapX = x;
						lpObj->RegenMapY = y;

						gObjClearViewport(&gObj[aIndex]);

						GSProtocol.GCTeleportSend(&gObj[aIndex], gt, mapNumber, gObj[aIndex].X, gObj[aIndex].Y, gObj[aIndex].Dir);

						if (lpObj->m_Change >= 0)
						{
							gObjViewportListProtocolCreate(&gObj[aIndex]);
						}

						gObj[aIndex].RegenOk = 1;

						if (g_ConfigRead.server.GetServerType() == SERVER_CASTLE)
						{
							if(gObj[aIndex].MapNumber == MAP_INDEX_CASTLESIEGE)
							{
								if(g_CastleSiege.GetCastleState() == CASTLESIEGE_STATE_STARTSIEGE)
								{
									g_CastleSiege.NotifySelfCsJoinSide(aIndex);
									g_CastleSiege.NotifyCsSelfLeftTime(aIndex);
								}
							}
						}

						return false;
					}

					GJReqMapSvrMove(lpObj->m_Index,sSvrCode,mapNumber,x,y);
					g_Log.Add("[MapServerMng] Request to Move Map Server : (%d) - [%s][%s] (%d)",sSvrCode,lpObj->AccountID,lpObj->Name,lpObj->m_Index);
					return false;
				}
			}
		}

		if ( (mapNumber == MAP_INDEX_ARCA_WAR || mapNumber == MAP_INDEX_DEBENTER_ARCA_WAR) && g_ArcaBattle.IsArcaBattleServer() == TRUE && g_AcheronGuardianEvent.GetProcState() < 3)
		{
			if ( g_ArcaBattle.GetState() < 7 )
			{
				x = lpObj->X;
				y = lpObj->Y;
				mapNumber = lpObj->MapNumber;
				dir = lpObj->Dir;
				movefail = 1;

				lpObj->RegenMapNumber = lpObj->MapNumber;
		
				lpObj->RegenMapX = x;
				lpObj->RegenMapY = y;
				
				gObjClearViewportOfMine(&gObj[aIndex]);
				
				GSProtocol.GCTeleportSend(&gObj[aIndex],gt,mapNumber,gObj[aIndex].X,gObj[aIndex].Y,gObj[aIndex].Dir);
				
				if(lpObj->m_Change >= 0)
				{
					gObjViewportListProtocolCreate(&gObj[aIndex]);
				}
	
				gObj[aIndex].RegenOk = 1;

				return 0;
			}

			if (g_ArcaBattle.GetGuildMasterPos(lpObj, gt, x, y) == FALSE)
			{
				g_ArcaBattle.GetUserPos(gt, x, y);
			}

			g_ArcaBattle.SetUserMapInfo(lpObj, gt);
		}

		if(lpObj->Type == OBJ_USER)
		{
			lpObj->m_PlayerData->MoveGate = gt;
		}

		lpObj->m_State = 32;

		WORD nCurrentMap = gObj[aIndex].MapNumber;

		g_Log.Add("[MoveMap][%s][%s] : (Before:Map:%d X:%d Y:%d) -> (New Map:%d X:%d Y:%d)",lpObj->AccountID,lpObj->Name,lpObj->MapNumber,lpObj->X,lpObj->Y,mapNumber,x,y);

		gObj[aIndex].X = x;
		gObj[aIndex].Y = y;
		gObj[aIndex].TX = x;
		gObj[aIndex].TY = y;
		gObj[aIndex].MapNumber = mapNumber;
		gObj[aIndex].Dir = dir;
		gObj[aIndex].PathCount = 0;
		gObj[aIndex].Teleport = 0;

		if (gObj[aIndex].m_PlayerData->m_bUsePartyMatching)
		{
			if (DG_MAP_RANGE(gObj[aIndex].MapNumber) || IMPERIAL_MAP_RANGE(gObj[aIndex].MapNumber))
			{
				if (gObj[aIndex].m_PlayerData->m_bPartyMatchingLeader)
				{
					GDReqCancelPartyMatching(aIndex, 0);
				}
				else
				{
					PMSG_ANS_PARTYMATCHING_ERROR pMsg;

					PHeadSubSetB((LPBYTE)&pMsg, 0xEFu, 0x09, sizeof(pMsg));
					pMsg.iResult = -2;
					IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
				}
			}
		}

		gObjViewportListProtocolDestroy(&gObj[aIndex]);
		GSProtocol.GCTeleportSend(&gObj[aIndex], gt, mapNumber, gObj[aIndex].X, gObj[aIndex].Y, gObj[aIndex].Dir);
		gObjViewportListProtocolCreate(&gObj[aIndex]);
		gObjClearViewport(&gObj[aIndex]);

		if (lpObj->m_Change >= 0)
			gObjViewportListProtocolCreate(&gObj[aIndex]);

		gObj[aIndex].RegenMapNumber = mapNumber;
		gObj[aIndex].RegenMapX = x;
		gObj[aIndex].RegenMapY = y;
		gObj[aIndex].RegenOk = 1;

		if (nCurrentMap != mapNumber)
		{
			g_SendNPCInfo.SendNpcCoordinate(lpObj->m_Index, lpObj->MapNumber, 0);
			g_SendNPCInfo.SendPortalCoordinate(lpObj->m_Index, lpObj->MapNumber);
		}

		if (g_DevilSquareFinal.m_bEVENT_ENABLE == true && lpObj->MapNumber == MAP_INDEX_DEVILSQUARE_FINAL &&
			g_DevilSquareFinal.m_nDSF_TYPE != 0 && g_DevilSquareFinal.m_bWinnerResultMsgSend == true)
		{
			g_DevilSquareFinal.SendDSFResult(lpObj->m_Index);
		}
	}
	else
	{
		GSProtocol.GCTeleportSend(&gObj[aIndex],gt,mapNumber,gObj[aIndex].X,gObj[aIndex].Y,gObj[aIndex].Dir);
		gObj[aIndex].RegenOk = 1;

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

	if(movefail != 0)
	{
		return false;
	}

	if(gObj[aIndex].Type == OBJ_USER)
	{
		gObj[aIndex].m_LastTeleportTime = 10;

		g_NewPVP.Reset(gObj[aIndex]);

		if(gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_INVISABLE) != 0)
		{
			if(gObj[aIndex].MapNumber != MAP_INDEX_CASTLESIEGE)
			{
				gObjViewportListProtocolDestroy(&gObj[aIndex]);
			}
		}
	}

	return true;
}




void gObjTeleport(int aIndex, int map, int x, int y)
{

	if(aIndex < 0 || aIndex > g_ConfigRead.server.GetObjectMax() - 1)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];
	
	if(MapNumberCheck(map)==0)
	{
		g_Log.Add("error-L3 : %s %d",__FILE__,__LINE__);
		return;
	}

	if(gObj[aIndex].MapNumber != map)
	{
		short sSvrCode = g_MapServerManager.CheckMoveMapSvr(lpObj->m_Index,map,lpObj->m_sPrevMapSvrCode);

		if(sSvrCode != g_ConfigRead.server.GetGameServerCode())
		{
			if(sSvrCode == -1)
			{
				g_Log.AddC(TColor::Red, "[MapServerMng] Map Server Move Fail : CheckMoveMapSvr() == -1 [%s][%s] (%d)",lpObj->AccountID,lpObj->Name,lpObj->m_Index);
				return;
			}

			GJReqMapSvrMove(lpObj->m_Index,sSvrCode,map,x,y);
			g_Log.Add("[MapServerMng] Request to Move Map Server : (%d) - [%s][%s] (%d)",sSvrCode,lpObj->AccountID,lpObj->Name,lpObj->m_Index);
			return;
		}
	}

	if(lpObj->m_IfState.use == 1 && lpObj->m_IfState.type == 1)
	{
		GSProtocol.CGTradeCancelButtonRecv(lpObj->m_Index);
	}

	if(lpObj->m_IfState.use == 1 && lpObj->m_IfState.type == 6)
	{
		GSProtocol.CGWarehouseUseEnd(lpObj->m_Index);
	}

	int nCurrentMapNum = gObj[aIndex].MapNumber;

	lpObj->m_State = 32;
	gObj[aIndex].X = x;
	gObj[aIndex].Y = y;

	gObj[aIndex].TX = x;
	gObj[aIndex].TY = y;

	gObj[aIndex].MapNumber = map;
	gObj[aIndex].PathCount = 0;
	gObj[aIndex].Teleport = 0;

	if (gObj[aIndex].Type == OBJ_USER && gObj[aIndex].m_PlayerData->m_bIsMining)
		g_MineSystem.CheckMoveMapWhileMining(aIndex);

	gObjViewportListProtocolDestroy(&gObj[aIndex]);
	GSProtocol.GCTeleportSend(&gObj[aIndex], 1, map, gObj[aIndex].X, gObj[aIndex].Y, gObj[aIndex].Dir);
	gObjViewportListProtocolCreate(&gObj[aIndex]);
	gObjClearViewport(&gObj[aIndex]);

	if(lpObj->m_Change >= 0)
	{
		gObjViewportListProtocolCreate(&gObj[aIndex]);
	}

	gObj[aIndex].RegenMapNumber = map;
	gObj[aIndex].RegenMapX = x;
	gObj[aIndex].RegenMapY = y;
	gObj[aIndex].RegenOk = 1;
}




void gObjMoveDataLoadingOK(int aIndex)
{
	if(gObj[aIndex].RegenOk == 1)
	{
		gObj[aIndex].RegenOk = 2;
	}
}




class CMagicInf* gObjGetMagic(LPOBJ lpObj, int mIndex)
{
	if ( mIndex < 0 || mIndex > MAX_MAGIC-1 )
	{
		g_Log.Add("Number of magic exceeds (%s %d)", __FILE__, __LINE__);
		return NULL;
	}

	if ( lpObj->Magic[mIndex].IsMagic() == FALSE )
	{
		return NULL;
	}

	return &lpObj->Magic[mIndex];
}

CMagicInf * gObjGetMagicSearch(LPOBJ lpObj, WORD skillnumber)
{

	if(skillnumber == 58)
	{
		skillnumber = 40;
	}

	if(skillnumber == 395)
	{
		skillnumber = 392;
	}

	for(int n = 0; n < MAGIC_SIZE;n++)
	{
		if(lpObj->Magic[n].IsMagic() == 1)
		{
			if(lpObj->Magic[n].m_Skill == skillnumber)
			{
				/*if(gObjMagicEnergyCheck(lpObj,lpObj->Magic[n].m_Skill) == 0)
				{
					GSProtocol.GCServerMsgStringSend(Lang.GetText(0,147),lpObj->m_Index,1);
					return false;
				}
				else
				{
					return &lpObj->Magic[n];
				}*/
				return &lpObj->Magic[n]; //Added by HermeX
			}
		}
	}
	return false;
}




int	gObjMagicManaUse(LPOBJ lpObj, CMagicInf * lpMagic)
{
	float mana = lpObj->Mana;
	float DecreaseMana;

	if(lpObj->Type != OBJ_USER)
	{
		return false;
	}

	if(lpMagic == 0)
	{
		return false;
	}

	if (g_MasterLevelSkillTreeSystem.CheckMasterLevelSkill(lpMagic->m_Skill) == true)
	{
		DecreaseMana = MagicDamageC.SkillGetMana(lpMagic->m_Skill);
		DecreaseMana += DecreaseMana * g_MasterLevelSkillTreeSystem.GetMasterSkillManaIncRate(lpMagic->m_Level) / 100.0;
	}

	else
	{
		DecreaseMana = MagicDamageC.SkillGetMana(lpMagic->m_Skill);
	}

	if (lpObj->Class == CLASS_RAGEFIGHTER)
	{
		DecreaseMana -= DecreaseMana * lpObj->m_PlayerData->m_MPSkillOpt.iMpsDecreaseMana_Monk / 100.0;
	}

	else
	{
		DecreaseMana -= DecreaseMana * lpObj->m_PlayerData->m_MPSkillOpt.iMpsDecreaseMana / 100.0;
	}

	if (DecreaseMana < 1.0)
		DecreaseMana = 1.0;

	mana -= DecreaseMana;

	if(mana < 0)
	{
		return -1;
	}

	return mana;
}




int  gObjMagicBPUse(LPOBJ lpObj, CMagicInf * lpMagic)
{
	int bp = lpObj->BP;

	if(lpObj->Type != OBJ_USER)
	{
		return false;
	}

	if(lpMagic == 0)
	{
		return false;
	}

	int usebp = MagicDamageC.SkillGetBP(lpMagic->m_Skill);

	if (lpObj->m_PlayerData->m_btAGReduceRate > 0)
	{
		usebp -= usebp * lpObj->m_PlayerData->m_btAGReduceRate / 100;
	}

	bp -= usebp;

	if(bp < 0)
	{
		return -1;
	}

	return bp;
}

void gObjMagicAddEnergyCheckOnOff(int flag)
{
	g_EnergyCheckOff=flag;
}

int  gObjMagicAdd(LPOBJ lpObj, WORD Type, WORD Index, BYTE Level, WORD & SkillNumber)
{

	int skill = -1,n,reqeng,reqleadership;

	skill = GetSkillNumberInex(Type,Index,Level);

	if(skill < 0)
	{
		g_Log.Add("error-L2: Does not have the skill");
		return -1;
	}

	if(g_EnergyCheckOff == 1)
	{
		reqeng = MagicDamageC.SkillGetRequireEnergy(skill);

		if(reqeng < 0)
		{
			g_Log.Add("error-L2: Skill energy not enough");
			return -1;
		}

		if((lpObj->m_PlayerData->Energy + lpObj->AddEnergy) < reqeng)
		{
			g_Log.Add("error-L2: Skill energy not enough");
			return -1;
		}
	}

	reqleadership = MagicDamageC.GetskillRequireLeadership(skill);

	if(reqleadership < 0)
	{
		g_Log.Add("error-L2: Skill Leadership not enough");
		return -1;
	}

	if((lpObj->Leadership + lpObj->AddLeadership) < reqleadership)
	{
		g_Log.Add("error-L2: Skill Leadership not enough");
		return -1;
	}

	if(MagicDamageC.SkillGetRequireClass(lpObj->Class,lpObj->m_PlayerData->ChangeUP,skill) < 1)
	{
		g_Log.Add("error-L2: His class is not allowed to use the skill [ID: %d]", skill);
		return -1;
	}

	if(g_MasterLevelSkillTreeSystem.IsExistMLSBasedSkill(lpObj, skill) == TRUE )
	{
		return -1;
	}

	for(int n = 0; n < MAGIC_SIZE; n++)
	{
		if(lpObj->Magic[n].IsMagic() == 1)
		{
			skill = GetSkillNumberInex(Type,Index,Level);

			if(skill < 0)
			{
				g_Log.Add("Skill index error (%s %d)",__FILE__,__LINE__);
				return -1;
			}

			if(lpObj->Magic[n].m_Skill == skill)
			{
				g_Log.Add("Same skill already exists");
				return -1;
			}
		}
	}

	skill = -1;

	for(int n = 0; n < MAGIC_SIZE; n++)
	{
		if(lpObj->Magic[n].IsMagic() == 0)
		{
			skill = lpObj->Magic[n].Set(Type,Index,Level);
			if(skill < 0)
			{
				g_Log.Add("Same magic already exists %s %d",__FILE__,__LINE__);
				return -1;
			}
			else
			{
				lpObj->MagicCount++;
				SkillNumber = skill;
				return n;
			}
		}
	}

	g_Log.Add("No space to add magic %s %d",__FILE__,__LINE__);
	return -1;
}




int  gObjMagicDel(LPOBJ lpObj, WORD aSkill, BYTE Level)
{
	if (g_MasterLevelSkillTreeSystem.MLS_WeaponSkillDel(lpObj, aSkill, Level) != FALSE) //season4 add-on
	{
		return -1;
	}

	for(int n = 0; n < MAGIC_SIZE; n++)
	{
		if(lpObj->Magic[n].IsMagic() == 1)
		{
			if(lpObj->Magic[n].m_Skill == aSkill)
			{
				lpObj->Magic[n].Clear();
				return n;
			}
		}
	}
	return -1;
}




int  gObjMagicAdd(LPOBJ lpObj, WORD aSkill, BYTE Level)
{
	int skill = -1,n,reqeng;

	skill = -1;

	if(g_EnergyCheckOff == 1 && lpObj->Type == OBJ_USER && lpObj->m_PlayerData->ISBOT == false)
	{
		reqeng = MagicDamageC.SkillGetRequireEnergy(aSkill);

		if(reqeng < 0)
		{
			g_Log.Add("error-L2: Skill energy not enough");
			return -1;
		}

		if((lpObj->m_PlayerData->Energy + lpObj->AddEnergy) < reqeng)
		{
			g_Log.Add("error-L2: Skill energy not enough");
			return -1;
		}
	}

	if(MagicDamageC.SkillGetRequireClass(lpObj->Class,lpObj->m_PlayerData->ChangeUP,aSkill) < 1 && lpObj->Type == OBJ_USER && lpObj->m_PlayerData->ISBOT == false)
	{
		g_Log.Add("error-L2: His class is not allowed to use the skill [ID: %d]", aSkill);
		return -1;
	}

	if(g_MasterLevelSkillTreeSystem.IsExistMLSBasedSkill(lpObj, aSkill) == true)
	{
		return -1;
	}

	for(int n = 0; n < MAGIC_SIZE; n++)
	{
		if(lpObj->Magic[n].IsMagic() == 1)
		{
			if(lpObj->Magic[n].m_Skill == aSkill)
			{
				if (g_EnergyCheckOff == 0)
				{
					return -1;
				}

				g_Log.Add("Same skill already exists %d %d", lpObj->Magic[n].m_Skill,aSkill);
				return -1;
			}
		}
	}

	for(int n = 0; n < MAGIC_SIZE; n++)
	{
		if(lpObj->Magic[n].IsMagic() == 0)
		{
			skill = lpObj->Magic[n].Set(aSkill,Level);

			if(skill < 0)
			{
				g_Log.Add("Same magic already exists %s %d",__FILE__,__LINE__);
				return -1;
			}
			else
			{
				lpObj->MagicCount++;
				g_Log.Add("[gObjMagicAdd] (%s) %d - Skill:%d, Level:%d", lpObj->Name, lpObj->MagicCount, aSkill, Level);
				return n;
			}
		}
	}

	g_Log.Add("No space to add magic %s %d",__FILE__,__LINE__);
	return -1;
}




int gObjWeaponMagicAdd(LPOBJ lpObj, WORD aSkill, BYTE Level)
{
	int skill = -1,n;
	skill = -1;

	if(MagicDamageC.SkillGetRequireClass(lpObj->Class,lpObj->m_PlayerData->ChangeUP,aSkill) < 1)
	{
		return -1;
	}

	if(g_MasterLevelSkillTreeSystem.MLS_WeaponSkillAdd(lpObj, aSkill, Level) == true)
	{
		return -1;
	}

	for(int n = 0; n < MAGIC_SIZE; n++)
	{
		if(lpObj->Magic[n].IsMagic() == 0)
		{
			skill = lpObj->Magic[n].Set(aSkill,Level);
			if(skill < 0)
			{
				g_Log.Add("Same magic already exists %s %d",__FILE__,__LINE__);
				return -1;
			}
			else
			{
				lpObj->MagicCount++;
				g_Log.Add("[gObjWeaponMagicAdd] (%s) %d - Skill:%d, Level:%d", lpObj->Name, lpObj->MagicCount, aSkill, Level);
				return n;
			}
		}
	}
	g_Log.Add("No space to add magic %s %d",__FILE__,__LINE__);
	return -1;
}




int gObjMonsterMagicAdd(LPOBJ lpObj, WORD aSkill, BYTE Level)
{
	int skill = -1;
	for (int n=0; n<MAGIC_SIZE ; n++)
	{
		if ( lpObj->Magic[n].IsMagic() == FALSE )
		{
			skill = lpObj->Magic[n].Set(aSkill, Level);

			if ( skill < 0 )
			{
				g_Log.Add("Same magic already exists %s %d",__FILE__, __LINE__);
				return -1;
			}

			lpObj->MagicCount++;
			return n;
		}
	}

	g_Log.Add("No space to add magic %s %d", __FILE__, __LINE__);
	return -1;
}




BOOL gObjMagicEnergyCheck(LPOBJ lpObj , WORD aSkill)
{
	int reqeng,reqlevel;

	reqeng = MagicDamageC.SkillGetRequireEnergy(aSkill);

	if(reqeng < 0)
	{
		return false;
	}

	if((lpObj->m_PlayerData->Energy + lpObj->AddEnergy) < reqeng)
	{
		return false;
	}

	reqlevel = MagicDamageC.SkillGetRequireLevel(aSkill);

	if(lpObj->Level < reqlevel)
	{
		return false;
	}

	return true;
}

BOOL gObjSpecialItemLevelUp(LPOBJ lpObj, int source, int target)
{
	if(source < 0 || source > MAIN_INVENTORY_SIZE -1)
	{
		return false;
	}

	if(target < 0 || target > MAIN_INVENTORY_SIZE -1)
	{
		return false;
	}

	if(lpObj->pInventory[source].IsItem() == 0)
	{
		return false;
	}

	if(lpObj->pInventory[target].IsItem() == 0)
	{
		return false;
	}

	if (g_LuckyItemManager.IsLuckyItemEquipment(lpObj->pInventory[target].m_Type))	//1.01.00
	{
		return false;
	}

	g_Log.Add("[%s][%s]Before using Jewel of Bless : (%s) serial:(%I64d)(%I64d)",lpObj->AccountID,lpObj->Name,lpObj->pInventory[target].GetName(),lpObj->pInventory[target].m_Number,lpObj->pInventory[target].m_Level);

	lpObj->pInventory[target].m_Level++;

	if(lpObj->pInventory[target].m_Level > 15)
	{
		lpObj->pInventory[target].m_Level = 15;
	}

	gObjMakePreviewCharSet(lpObj->m_Index);

	if (g_PentagramSystem.IsPentagramItem(lpObj->pInventory[target].m_Type) == false)
	{
		float levelitemdur = ItemGetDurability(lpObj->pInventory[target].m_Type, lpObj->pInventory[target].m_Level, lpObj->pInventory[target].IsExtItem(), lpObj->pInventory[target].IsSetItem());

		lpObj->pInventory[target].m_Durability = levelitemdur * lpObj->pInventory[target].m_Durability / lpObj->pInventory[target].m_BaseDurability;
	}

	lpObj->pInventory[target].Convert(
			lpObj->pInventory[target].m_Type,
			lpObj->pInventory[target].m_Option1,
			lpObj->pInventory[target].m_Option2,
			lpObj->pInventory[target].m_Option3,
			lpObj->pInventory[target].m_NewOption,
			lpObj->pInventory[target].m_SetOption,
			lpObj->pInventory[target].m_ItemOptionEx,
			lpObj->pInventory[target].m_SocketOption,
			lpObj->pInventory[target].m_BonusSocketOption,
			0,
			CURRENT_DB_VERSION);

	g_Log.Add("[%s][%s]After using Jewel of Bless(%d) : (%s) serial:(%I64d)(%I64d)",lpObj->AccountID,lpObj->Name,lpObj->pInventory[source].m_Number,
			lpObj->pInventory[target].GetName(),lpObj->pInventory[target].m_Number,lpObj->pInventory[target].m_Level);
	return true;
}

BOOL gObjSpecialItemRepair(LPOBJ lpObj, int source, int target)
{
	if(source < 0 || source > MAIN_INVENTORY_SIZE -1)
	{
		return false;
	}

	if(target < 0 || target > MAIN_INVENTORY_SIZE -1)
	{
		return false;
	}

	if(lpObj->pInventory[source].IsItem() == 0)
	{
		return false;
	}

	if(lpObj->pInventory[target].IsItem() == 0)
	{
		return false;
	}

	g_Log.Add("[%s][%s]Before using Jewel of Bless : (%s) serial:(%I64d)(%I64d)",lpObj->AccountID,lpObj->Name,lpObj->pInventory[target].GetName(),lpObj->pInventory[target].m_Number,lpObj->pInventory[target].m_Level);

	lpObj->pInventory[target].m_Durability = 255.0;

	gObjMakePreviewCharSet(lpObj->m_Index);

	lpObj->pInventory[target].Convert(
			lpObj->pInventory[target].m_Type,
			lpObj->pInventory[target].m_Option1,
			lpObj->pInventory[target].m_Option2,
			lpObj->pInventory[target].m_Option3,
			lpObj->pInventory[target].m_NewOption,
			lpObj->pInventory[target].m_SetOption,
			lpObj->pInventory[target].m_ItemOptionEx,
			lpObj->pInventory[target].m_SocketOption,
			lpObj->pInventory[target].m_BonusSocketOption,
			0,
			CURRENT_DB_VERSION);

	g_Log.Add("[%s][%s]After using Jewel of Bless(%d) : (%s) serial:(%I64d)(%I64d)",lpObj->AccountID,lpObj->Name,lpObj->pInventory[source].m_Number,
			lpObj->pInventory[target].GetName(),lpObj->pInventory[target].m_Number,lpObj->pInventory[target].m_Level);
	return true;
}

BOOL gObjItemLevelUp(LPOBJ lpObj, int source, int target)
{
	if(source < 0 || source > MAIN_INVENTORY_SIZE -1)
	{
		return false;
	}

	if(target < 0 || target > MAIN_INVENTORY_SIZE -1)
	{
		return false;
	}

	if(lpObj->pInventory[source].IsItem() == 0)
	{
		return false;
	}

	if(lpObj->pInventory[target].IsItem() == 0)
	{
		return false;
	}

	BOOL bExceptionCheckResult = IsExceptionJewelOfBlessInchant(lpObj, source, target);

	if (bExceptionCheckResult)
	{
		return bExceptionCheckResult;
	}

	if (IsNotApplyJewelToItems(lpObj->pInventory[target].m_Type) == TRUE)
	{
		return false;
	}

	if (g_LuckyItemManager.IsLuckyItemEquipment(lpObj->pInventory[target].m_Type) && g_ConfigRead.data.common.LuckyItemAllowUseJewels == false)	//1.01.00
	{
		return false;
	}

	if(lpObj->pInventory[target].m_Level >= 6)
	{
		return false;
	}

	g_Log.Add("[%s][%s]Before using Jewel of Bless : (%s) serial:(%I64d)(%I64d)",lpObj->AccountID,lpObj->Name,lpObj->pInventory[target].GetName(),lpObj->pInventory[target].m_Number,lpObj->pInventory[target].m_Level);

	lpObj->pInventory[target].m_Level++;
	gObjMakePreviewCharSet(lpObj->m_Index);

	if(g_PentagramSystem.IsPentagramItem(lpObj->pInventory[target].m_Type) == false)
	{
		float levelitemdur = ItemGetDurability(lpObj->pInventory[target].m_Type,lpObj->pInventory[target].m_Level,lpObj->pInventory[target].IsExtItem(),lpObj->pInventory[target].IsSetItem());

		lpObj->pInventory[target].m_Durability = levelitemdur * lpObj->pInventory[target].m_Durability / lpObj->pInventory[target].m_BaseDurability;
	}

	lpObj->pInventory[target].Convert(
			lpObj->pInventory[target].m_Type,
			lpObj->pInventory[target].m_Option1,
			lpObj->pInventory[target].m_Option2,
			lpObj->pInventory[target].m_Option3,
			lpObj->pInventory[target].m_NewOption,
			lpObj->pInventory[target].m_SetOption,
			lpObj->pInventory[target].m_ItemOptionEx,
			lpObj->pInventory[target].m_SocketOption,
			lpObj->pInventory[target].m_BonusSocketOption,
			0,
			CURRENT_DB_VERSION);

	g_Log.Add("[%s][%s]After using Jewel of Bless(%d) : (%s) serial:(%I64d)(%I64d)",lpObj->AccountID,lpObj->Name,lpObj->pInventory[source].m_Number,
			lpObj->pInventory[target].GetName(),lpObj->pInventory[target].m_Number,lpObj->pInventory[target].m_Level);
	return true;
}

BOOL gObjItemRandomLevelUp(LPOBJ lpObj, int source, int target)
{
	if(source < 0 || source > MAIN_INVENTORY_SIZE -1)
	{
		return false;
	}

	if(target < 0 || target > MAIN_INVENTORY_SIZE -1)
	{
		return false;
	}

	if(lpObj->pInventory[source].IsItem() == 0)
	{
		return false;
	}

	if(lpObj->pInventory[target].IsItem() == 0)
	{
		return false;
	}

	if (IsNotApplyJewelToItems(lpObj->pInventory[target].m_Type) == TRUE)
	{
		return false;
	}

	if (g_LuckyItemManager.IsLuckyItemEquipment(lpObj->pInventory[target].m_Type) && g_ConfigRead.data.common.LuckyItemAllowUseJewels == false)	//1.01.00
	{
		return false;
	}

	if(lpObj->pInventory[target].m_Level >= 9)
	{
		return false;
	}

	g_Log.Add("[%s][%s]Before using Jewel of Soul : (%s) serial:(%I64d)(%I64d)",
		lpObj->AccountID,lpObj->Name,lpObj->pInventory[target].GetName(),
		lpObj->pInventory[target].m_Number,lpObj->pInventory[target].m_Level);

	int _r = rand()%100;

	if(lpObj->pInventory[target].m_Option2)
	{
		if(_r < g_ConfigRead.data.common.SoulLuckRate)
		{
			lpObj->pInventory[target].m_Level++;
		}
		else
		{
			if(lpObj->pInventory[target].m_Level >= 7)
			{
				lpObj->pInventory[target].m_Level = 0;
			}
			else
			{
				lpObj->pInventory[target].m_Level--;

				if(lpObj->pInventory[target].m_Level < 0)
				{
					lpObj->pInventory[target].m_Level = 0;
				}
			}
		}
	}
	else
	{
		if(_r >= g_ConfigRead.data.common.SoulNoLuckRate)
		{
			if(lpObj->pInventory[target].m_Level >= 7)
			{
				lpObj->pInventory[target].m_Level = 0;
			}
			else
			{
				lpObj->pInventory[target].m_Level--;

				if(lpObj->pInventory[target].m_Level < 0)
				{
					lpObj->pInventory[target].m_Level = 0;
				}
			}
		}
		else
		{
			lpObj->pInventory[target].m_Level++;
		}
	}

	if(lpObj->pInventory[target].m_Level > 9)
	{
		lpObj->pInventory[target].m_Level = 9;
	}
	else
	{
		gObjMakePreviewCharSet(lpObj->m_Index);
	}

	if (g_PentagramSystem.IsPentagramItem(lpObj->pInventory[target].m_Type) == false)
	{
		float levelitemdur = ItemGetDurability(lpObj->pInventory[target].m_Type,lpObj->pInventory[target].m_Level,lpObj->pInventory[target].IsExtItem(),lpObj->pInventory[target].IsSetItem());

		lpObj->pInventory[target].m_Durability = levelitemdur * lpObj->pInventory[target].m_Durability / lpObj->pInventory[target].m_BaseDurability;
	}

	lpObj->pInventory[target].Convert(
			lpObj->pInventory[target].m_Type,
			lpObj->pInventory[target].m_Option1,
			lpObj->pInventory[target].m_Option2,
			lpObj->pInventory[target].m_Option3,
			lpObj->pInventory[target].m_NewOption,
			lpObj->pInventory[target].m_SetOption,
			lpObj->pInventory[target].m_ItemOptionEx,
			lpObj->pInventory[target].m_SocketOption,
			lpObj->pInventory[target].m_BonusSocketOption,
			0,
			CURRENT_DB_VERSION);

	g_Log.Add("[%s][%s]After using Jewel of Soul (%d) : (%s) serial:(%I64d)(%I64d)",lpObj->AccountID,lpObj->Name,lpObj->pInventory[source].m_Number,
			lpObj->pInventory[target].GetName(),lpObj->pInventory[target].m_Number,lpObj->pInventory[target].m_Level);

	if(g_kJewelOfHarmonySystem.IsStrengthenByJewelOfHarmony(&lpObj->pInventory[target])== 1)
	{
		if(g_kJewelOfHarmonySystem.IsActive(&lpObj->pInventory[target]) == 0)
		{
			GSProtocol.GCServerMsgStringSend(Lang.GetText(0,271),lpObj->m_Index,1);
		}
	}

	return true;
}

BOOL gObjItemRandomOption3Up(LPOBJ lpObj, int source, int target)
{
	if(source < 0 || source > MAIN_INVENTORY_SIZE -1)
	{
		return false;
	}

	if(target < 0 || target > MAIN_INVENTORY_SIZE -1)
	{
		return false;
	}

	if(lpObj->pInventory[source].IsItem() == 0)
	{
		return false;
	}

	if(lpObj->pInventory[target].IsItem() == 0)
	{
		return false;
	}

	if (IsNotApplyJewelToItems(lpObj->pInventory[target].m_Type) == TRUE)
	{
		return false;
	}

	if (lpObj->pInventory[target].m_Type >= ITEMGET(12,200) && lpObj->pInventory[target].m_Type <= ITEMGET(12,220))
	{
		return false;
	}

	g_Log.Add("[%s][%s]Before using Jewel of Bless : (%s) serial:(%I64d)(%I64d)",
		lpObj->AccountID,lpObj->Name,
		lpObj->pInventory[target].GetName(),
		lpObj->pInventory[target].m_Number,
		lpObj->pInventory[target].m_Option3);

	int _r = rand()%100;
	char maxopt = g_ConfigRead.data.common.Is28Opt == true ? 7 : 4;

	if (lpObj->pInventory[target].m_Option3 == 0)
	{
		if ((lpObj->pInventory[target].m_Type >= ITEMGET(12,3) && lpObj->pInventory[target].m_Type <= ITEMGET(12,6)) ||
			lpObj->pInventory[target].m_Type == ITEMGET(12,49) || lpObj->pInventory[target].m_Type == ITEMGET(12,42))
		{
			lpObj->pInventory[target].m_NewOption &= 0xDF;

			if (rand()%2 == 1)
			{
				lpObj->pInventory[target].m_NewOption |= 0x20;
			}
		}
	}

	if (lpObj->pInventory[target].m_Option3 >= maxopt)
	{
		return false;
	}

	if (_r < g_ConfigRead.data.common.LifeRate)
	{
		if (lpObj->pInventory[target].m_Option3 == 0)
		{
			if ((lpObj->pInventory[target].m_Type >= ITEMGET(12,36) && lpObj->pInventory[target].m_Type <= ITEMGET(12,40)) ||
				lpObj->pInventory[target].m_Type == ITEMGET(12,43) || lpObj->pInventory[target].m_Type == ITEMGET(12,50))
			{
				lpObj->pInventory[target].m_NewOption &= 0xEF;
				lpObj->pInventory[target].m_NewOption &= 0xDF;
				
				if (rand()%2 == 0)
				{
					if (rand()%1000 < 400)
					{
						lpObj->pInventory[target].m_NewOption |= 0x10;
					}
				}

				else
				{
					if (rand()%1000 < 300)
					{
						lpObj->pInventory[target].m_NewOption |= 0x20;
					}
				}
			}

			else if (lpObj->pInventory[target].m_Type >= ITEMGET(12,262) && lpObj->pInventory[target].m_Type <= ITEMGET(12,265))
			{
				lpObj->pInventory[target].m_NewOption &= 0xEF;

				if (rand()%2 == 0)
				{
					lpObj->pInventory[target].m_NewOption |= 0x10;
				}
			}
		}

		lpObj->pInventory[target].m_Option3++;
	}

	else
	{
		lpObj->pInventory[target].m_Option3 = 0;
	}

	gObjMakePreviewCharSet(lpObj->m_Index);

	float levelitemdur = ItemGetDurability(lpObj->pInventory[target].m_Type,lpObj->pInventory[target].m_Level,lpObj->pInventory[target].IsExtItem(),lpObj->pInventory[target].IsSetItem());

	lpObj->pInventory[target].m_Durability = levelitemdur * lpObj->pInventory[target].m_Durability / lpObj->pInventory[target].m_BaseDurability;

	lpObj->pInventory[target].Convert(
			lpObj->pInventory[target].m_Type,
			lpObj->pInventory[target].m_Option1,
			lpObj->pInventory[target].m_Option2,
			lpObj->pInventory[target].m_Option3,
			lpObj->pInventory[target].m_NewOption,
			lpObj->pInventory[target].m_SetOption,
			lpObj->pInventory[target].m_ItemOptionEx,
			lpObj->pInventory[target].m_SocketOption,
			lpObj->pInventory[target].m_BonusSocketOption,
			0,
			CURRENT_DB_VERSION);

	g_Log.Add("[%s][%s]After using Jewel of Bless(%d) : (%s) serial:(%I64d)(%I64d)",lpObj->AccountID,lpObj->Name,lpObj->pInventory[source].m_Number,
			lpObj->pInventory[target].GetName(),lpObj->pInventory[target].m_Number,lpObj->pInventory[target].m_Option3);

	unsigned char NewOption[8];

	ItemIsBufExOption(NewOption,&lpObj->pInventory[target]);

	g_Log.Add("[%s][%s] JewelofLife Result [%d,%s,%d,%d,%d,%d]serial:[%I64d] dur:[%d] Ex:[%d,%d,%d,%d,%d,%d,%d]",
			lpObj->AccountID,lpObj->Name,target,lpObj->pInventory[target].GetName(),
			lpObj->pInventory[target].m_Level,lpObj->pInventory[target].m_Option1,lpObj->pInventory[target].m_Option2,
			lpObj->pInventory[target].m_Option3,lpObj->pInventory[target].m_Number,
			BYTE(lpObj->pInventory[target].m_Durability),NewOption[0],NewOption[1],NewOption[2],NewOption[3],
			NewOption[4],NewOption[5],NewOption[6]);

	return true;
}




void gObjAbilityReSet(LPOBJ lpObj)
{
	lpObj->m_PlayerData->LevelUpPoint = (lpObj->Level - 1) * 5;

	lpObj->m_PlayerData->Strength = DCInfo.DefClass[lpObj->Class].Strength;
	lpObj->m_PlayerData->Dexterity = DCInfo.DefClass[lpObj->Class].Dexterity;
	lpObj->m_PlayerData->Vitality = DCInfo.DefClass[lpObj->Class].Vitality;
	lpObj->m_PlayerData->Energy = DCInfo.DefClass[lpObj->Class].Energy;

	lpObj->MaxLife = DCInfo.DefClass[lpObj->Class].MaxLife;
	lpObj->MaxMana = DCInfo.DefClass[lpObj->Class].MaxMana;

	if(lpObj->Class == CLASS_WIZARD)
	{
		lpObj->MaxLife += (lpObj->Level -1)*0.5f;
		lpObj->MaxMana += 2.0f * (lpObj->Level -1);

		lpObj->Life = lpObj->MaxLife;
		lpObj->Mana = lpObj->MaxMana;
	}
	else
	{
		lpObj->MaxLife += 2.0f * (lpObj->Level - 1);
		lpObj->MaxMana += (lpObj->Level - 1)*0.5f;

		lpObj->Life = lpObj->MaxLife;
		lpObj->Mana = lpObj->MaxMana;
	}

	IOCP.CloseClient(lpObj->m_Index);
}




BOOL gObjTargetGuildWarCheck(LPOBJ lpObj, LPOBJ lpTargetObj)
{
	if(lpObj->Type != OBJ_USER)
	{
		return false;
	}

	if(lpTargetObj->Type != OBJ_USER)
	{
		return false;
	}

	if(lpObj->m_PlayerData->GuildNumber < 1 || lpTargetObj->m_PlayerData->GuildNumber < 1)
	{
		return false;
	}

	if(lpObj->m_PlayerData->lpGuild->WarState != 1 || lpTargetObj->m_PlayerData->lpGuild->WarState != 1)
	{
		return false;
	}

	if(strcmp(lpObj->m_PlayerData->lpGuild->TargetGuildName,lpTargetObj->m_PlayerData->lpGuild->Name))
	{
		return false;
	}
	return true;
}




void gObjGuildWarEndSend(LPOBJ lpObj, BYTE Result1, BYTE Result2)
{
	if(lpObj == 0)
	{
		g_Log.Add("error-L3 : %s %d",__FILE__,__LINE__);
		return;
	}

	if(lpObj->m_PlayerData->lpGuild == 0)
	{
		g_Log.Add("error-L3 : %s %d",__FILE__,__LINE__);
		return;
	}

	if(lpObj->m_PlayerData->lpGuild->WarState != 1)
	{
		g_Log.Add("error-L3 : %s %d",__FILE__,__LINE__);
		return;
	}

	if(lpObj->m_PlayerData->lpGuild->lpTargetGuildNode == 0)
	{
		g_Log.Add("error-L3 : %s %d",__FILE__,__LINE__);
		return;
	}

	if(Result1 == 1)
	{
		gObjGuildWarItemGive(lpObj->m_PlayerData->lpGuild,lpObj->m_PlayerData->lpGuild->lpTargetGuildNode);
	}
	else if(Result1 == 3)
	{
		gObjGuildWarItemGive(lpObj->m_PlayerData->lpGuild->lpTargetGuildNode,lpObj->m_PlayerData->lpGuild);
	}
	else if(Result1 == 1)
	{
		gObjGuildWarItemGive(lpObj->m_PlayerData->lpGuild->lpTargetGuildNode,lpObj->m_PlayerData->lpGuild);
	}

	for(int n = 0; n < MAX_USER_GUILD;n++)
	{
		if(lpObj->m_PlayerData->lpGuild->Use[n] && lpObj->m_PlayerData->lpGuild->Index[n] >= 0)
		{
			gObj[lpObj->m_PlayerData->lpGuild->Index[n]].IsInBattleGround = 0;
			GSProtocol.GCGuildWarEnd(lpObj->m_PlayerData->lpGuild->Index[n],Result1,lpObj->m_PlayerData->lpGuild->lpTargetGuildNode->Name);
		}
	}

	for(int n = 0; n < MAX_USER_GUILD;n++)
	{
		if(lpObj->m_PlayerData->lpGuild->lpTargetGuildNode->Use[n] && lpObj->m_PlayerData->lpGuild->lpTargetGuildNode->Index[n] >= 0)
		{
			gObj[lpObj->m_PlayerData->lpGuild->lpTargetGuildNode->Index[n]].IsInBattleGround = 0;
			GSProtocol.GCGuildWarEnd(lpObj->m_PlayerData->lpGuild->lpTargetGuildNode->Index[n],Result2,lpObj->m_PlayerData->lpGuild->Name);
		}
	}

	PMSG_NOTICE pNotice;
	TNotice::MakeNoticeMsgEx(&pNotice, 0, Lang.GetText(0,416), lpObj->m_PlayerData->lpGuild->Name, lpObj->m_PlayerData->lpGuild->lpTargetGuildNode->Name, Result1, Result2);
	TNotice::SendNoticeToAllUser(&pNotice);
}

void gObjGuildWarEndSend(_GUILD_INFO_STRUCT * lpGuild1, _GUILD_INFO_STRUCT * lpGuild2, BYTE Result1, BYTE Result2)
{
	if(lpGuild1 == 0 || lpGuild2 == 0)
	{
		return;
	}

	if(lpGuild1->WarState != 1 || lpGuild2->WarState != 1)
	{
		return;
	}

	for(int n = 0; n < MAX_USER_GUILD;n++)
	{
		if(lpGuild1->Use[n] && lpGuild1->Index[n] >= 0)
		{
			gObj[lpGuild1->Index[n]].IsInBattleGround = 0;
			GSProtocol.GCGuildWarEnd(lpGuild1->Index[n],Result1,lpGuild2->Name);
		}
	}

	for(int n = 0; n < MAX_USER_GUILD;n++)
	{
		if(lpGuild2->Use[n] && lpGuild2->Index[n] >= 0)
		{
			gObj[lpGuild2->Index[n]].IsInBattleGround = 0;
			GSProtocol.GCGuildWarEnd(lpGuild2->Index[n],Result2,lpGuild1->Name);
		}
	}

	PMSG_NOTICE pNotice;
	TNotice::MakeNoticeMsgEx(&pNotice, 0, Lang.GetText(0,416), lpGuild1->Name, lpGuild2->lpTargetGuildNode->Name, Result1, Result2);
	TNotice::SendNoticeToAllUser(&pNotice);
}




void gObjGuildWarEnd(_GUILD_INFO_STRUCT * lpGuild, _GUILD_INFO_STRUCT * lpTargetGuild)
{
	if(lpGuild == 0 || lpTargetGuild == 0)
	{
		return;
	}

	lpGuild->WarDeclareState = 0;
	lpGuild->WarState = 0;

	if(lpTargetGuild != 0)
	{
		lpTargetGuild->WarDeclareState = 0;
		lpTargetGuild->WarState = 0;

		if(lpGuild->lpTargetGuildNode != 0)
		{
			_GUILD_INFO_STRUCT * lpTguild = lpGuild->lpTargetGuildNode;

			lpGuild->lpTargetGuildNode = 0;
			lpTguild->lpTargetGuildNode = 0;
		}
	}
	else
	{
		lpGuild->lpTargetGuildNode = 0;
	}
}




BOOL gObjGuildWarProc(_GUILD_INFO_STRUCT * lpGuild1, _GUILD_INFO_STRUCT * lpGuild2, int score)
{
	int totalscore;
	int n;
	int maxscore;
	char szTemp[0x100];

	if(lpGuild1 == 0 || lpGuild2 == 0)
	{
		return false;
	}

	totalscore = 1;
	maxscore = 20;

	lpGuild1->PlayScore += score;

	wsprintf(szTemp,"%s ( %d ) VS %s ( %d )",lpGuild1->Name,lpGuild1->PlayScore,lpGuild2->Name,lpGuild2->PlayScore);
	g_Log.Add(szTemp);

	if(lpGuild1->WarType == 1)
	{
		SetBattleTeamScore(lpGuild1->BattleGroundIndex,lpGuild1->BattleTeamCode,lpGuild1->PlayScore);
		maxscore = 100;
	}
	else
	{
		maxscore = 20;
	}

	cManager.BattleInfoSend(GetBattleTeamName(0,0),GetBattleTeamScore(0,0),GetBattleTeamName(0,1),GetBattleTeamScore(0,1));

	if(lpGuild1->PlayScore >= maxscore)
	{
		if(lpGuild1->PlayScore > maxscore && lpGuild2->PlayScore == 0)
		{
			totalscore = 3;
		}
		else if(lpGuild1->PlayScore > maxscore && lpGuild2->PlayScore <= 10)
		{
			totalscore = 2;
		}

		lpGuild1->TotalScore += totalscore;
		DGGuildScoreUpdate(lpGuild1->Name,lpGuild1->TotalScore);
		DGGuildScoreUpdate(lpGuild2->Name,lpGuild2->TotalScore);

		lpGuild1->PlayScore = 0;
		lpGuild2->PlayScore = 0;

		return true;
	}

	for(int n = 0; n < MAX_USER_GUILD; n++)
	{
		if(lpGuild1->Use[n] && lpGuild1->Index[n] >= 0)
		{
			GSProtocol.GCGuildWarScore(lpGuild1->Index[n]);
		}
	}

	for(int n = 0; n < MAX_USER_GUILD; n++)
	{
		if(lpGuild2->Use[n] && lpGuild2->Index[n] >= 0)
		{
			GSProtocol.GCGuildWarScore(lpGuild2->Index[n]);
		}
	}
	return false;
}




BOOL gObjGuildWarCheck(LPOBJ lpObj, LPOBJ lpTargetObj)
{
	int score = 1;

	if(gObjTargetGuildWarCheck(lpObj,lpTargetObj)==0)
	{
		return false;
	}

	if(strcmp(lpTargetObj->Name,lpTargetObj->m_PlayerData->lpGuild->Names[0])==0)
	{
		score = 2;
	}

	_GUILD_INFO_STRUCT * lpGuild = lpObj->m_PlayerData->lpGuild;
	_GUILD_INFO_STRUCT * lpTargetGuild = lpTargetObj->m_PlayerData->lpGuild;

	if(gObjGuildWarProc(lpGuild,lpTargetGuild,score)==1)
	{
		if(lpGuild->WarType == 1)
		{
			gObjGuildWarEndSend(lpGuild,lpTargetGuild,1,0);
			gBattleGroundEnable(lpGuild->BattleGroundIndex,0);
			gObjGuildWarEnd(lpGuild,lpTargetGuild);
			cManager.BattleInfoSend(GetBattleTeamName(0,0),255,GetBattleTeamName(0,1),255);
		}
		else
		{
			gObjGuildWarEndSend(lpObj,1,0);
			gObjAddMsgSendDelay(lpObj,4,lpObj->m_Index,2000,0);
		}
	}
	return true;
}




BOOL gObjGuildWarMasterClose(LPOBJ lpObj)
{
	if(lpObj->m_PlayerData->GuildNumber < 1)
	{
		return false;
	}

	if(strcmp(lpObj->m_PlayerData->lpGuild->Names[0],lpObj->Name))
	{
		return false;
	}

	gObjGuildWarEndSend(lpObj,3,2);

	if(lpObj->m_PlayerData->lpGuild->WarType == 1 && lpObj->m_PlayerData->lpGuild->BattleGroundIndex < 1)
	{
		gBattleGroundEnable(lpObj->m_PlayerData->lpGuild->BattleGroundIndex,0);
		cManager.BattleInfoSend(GetBattleTeamName(0,0),255,GetBattleTeamName(0,1),255);
	}
	gObjGuildWarEnd(lpObj->m_PlayerData->lpGuild,lpObj->m_PlayerData->lpGuild->lpTargetGuildNode);
	return true;
}




int gObjGuildWarItemGive(_GUILD_INFO_STRUCT * lpWinGuild, _GUILD_INFO_STRUCT * lpLoseGuild)
{
	return true;
	int n;
	int r_userindex[MAX_USER_GUILD];
	int r_usercount;

	for(int n = 0; n < MAX_USER_GUILD; n++)
	{
		if(lpLoseGuild->Use[n] > 0)
		{
			r_userindex[r_usercount] = lpLoseGuild->Index[n];
			r_usercount++;
		}
	}

	if(r_usercount < 1)
	{
		g_Log.Add("error2: No user in the loser guild  is in game %s %d",__FILE__,__LINE__);
		return false;
	}

	int lose_user = r_userindex[rand()%r_usercount];

	if(lose_user < 0)
	{
		g_Log.Add("error2: No user in the loser guild  is in game %s %d",__FILE__,__LINE__);
		return false;
	}

	if(gObj[lose_user].Connected < PLAYER_PLAYING)
	{
		g_Log.Add("error2: No user in the loser guild  is in game %s %d",__FILE__,__LINE__);
		return false;
	}

	r_usercount = 0;

	for(int n = 0; n < MAX_USER_GUILD;n++)
	{
		if(lpWinGuild->Use[n] > 0)
		{
			r_userindex[r_usercount] = lpWinGuild->Index[n];
			r_usercount++;
		}
	}

	if(r_usercount < 1)
	{
		g_Log.Add("error2: No user in the winner guild is in game");
		return false;
	}

	int win_user = r_userindex[rand()%r_usercount];

	if(win_user < 0)
	{
		g_Log.Add("error2: No user in the winner guild is in game");
		return false;
	}

	if(gObj[win_user].Connected < PLAYER_PLAYING)
	{
		g_Log.Add("error2: No user in the winner guild is in game");
		return false;
	}

	LPOBJ lpObj = &gObj[lose_user];

	int count = 24;

	int number;

	while(count--)
	{
		number = rand()%12;

		if(lpObj->pInventory[number].IsItem()==1)
		{
			if(gObjInventoryInsertItem(win_user,lpObj->pInventory[number]) != 0xFF)
			{
				return true;
			}
			break;
		}
	}

	count = 64;

	while(count--)
	{
		number = rand()%64+12;

		if(lpObj->pInventory[number].IsItem()==1)
		{
			if(gObjInventoryInsertItem(win_user,lpObj->pInventory[number]) != 0xFF)
			{
				return true;
			}
			break;
		}
	}
	return true;
}




int gObjGetPkTime(LPOBJ lpObj, int& hour, int& min)
{
	int pktime = 0;

	if(lpObj->m_PK_Time > 0)
	{
		pktime = lpObj->m_PK_Time / 60;
	}

	if(lpObj->m_PK_Level == 4)
	{
		hour = (180 - pktime) / 60;
		min = (180 - pktime) % 60;
		return true;
	}

	if(lpObj->m_PK_Level == 5)
	{
		hour = (360 - pktime) / 60;
		min = (360 - pktime) % 60;
		return true;
	}

	if(lpObj->m_PK_Level >= 6)
	{
		hour = (((lpObj->m_PK_Count - 3) * 60 + 540) - pktime) / 60;
		min = (((lpObj->m_PK_Count - 3) * 60 + 540) - pktime) % 60;
		return true;
	}

	return false;
}




BOOL gObjMonsterCall(int aIndex, int MonsterType, int x, int y)
{

	if(gObj[aIndex].MapNumber == MAP_INDEX_ICARUS)
	{
		return false;
	}

	if(gObj[aIndex].m_RecallMon >= 0)
	{

		GSProtocol.GCRecallMonLife(aIndex,60,0);
		gObjMonsterCallKill(aIndex);
		return false;
	}

	int result = gObjAddCallMon();

	if(result >= 0)
	{
		gObj[result].X = x;
		gObj[result].Y = y;

		gObj[result].MTX = x;
		gObj[result].MTY = y;
		gObj[result].Dir = 2;

		gObj[result].MapNumber = gObj[aIndex].MapNumber;

		gObjSetMonster(result, MonsterType);

		gObj[result].m_RecallMon = aIndex;
		gObj[result].m_Attribute = 100;

		gObj[result].TargetNumber = -1;
		gObj[result].m_ActState.Emotion = 0;
		gObj[result].m_ActState.Attack = 0;
		gObj[result].m_ActState.EmotionCount = 0;

		gObj[result].PathCount = 0;

		gObj[aIndex].m_RecallMon = result;
		gObj[result].m_MoveRange = 15;

		GSProtocol.GCRecallMonLife(gObj[result].m_RecallMon,gObj[result].MaxLife,gObj[result].Life);
		g_Log.Add("[Summon Monster] [%s][%s] Try to Summon Monster - Succeed (SummonIndex:%d)",gObj[aIndex].AccountID,gObj[aIndex].Name,result);
		return true;
	}
	return false;
}




void gObjMonsterCallKill(int aIndex)
{

	g_Log.Add("[Summon Monster] [%s][%s] Try to Kill Summoned Monster",gObj[aIndex].AccountID,gObj[aIndex].Name);

	if(gObj[aIndex].m_RecallMon < 0)
	{
		return;
	}

	int callmon = gObj[aIndex].m_RecallMon;

	if(callmon < 0 || callmon > g_ConfigRead.server.GetObjectMax() -1 )
	{
		return;
	}

	if(gObj[callmon].Type != OBJ_MONSTER)
	{
		return;
	}

	if(gObj[callmon].m_RecallMon != aIndex)
	{
		return;
	}

	if(gObj[aIndex].m_RecallMon >= 0)
	{
		if(gObjIsConnected(gObj[aIndex].m_RecallMon)==1)
		{
			g_Log.Add("[Summon Monster] [%s][%s] Try to Kill Summoned Monster - Found Summoned Monster (SummonIndex:%d)",gObj[aIndex].AccountID,gObj[aIndex].Name,gObj[aIndex].m_RecallMon);
			gObj[gObj[aIndex].m_RecallMon].m_RecallMon = -1;
		}
	}

	gObj[aIndex].m_RecallMon = -1;
	gObjDel(callmon);

	g_Log.Add("[Summon Monster] [%s][%s] Try to Kill Summoned Monster - Finished to Kill Summoned Monster (SummonIndex:%d)",gObj[aIndex].AccountID,gObj[aIndex].Name,callmon);
}




BOOL gObjCheckTeleportArea(int aIndex, BYTE x, BYTE y)
{

	if(x < (gObj[aIndex].X - 8)
		|| x > (gObj[aIndex].X + 8)
		|| y < (gObj[aIndex].Y - 8)
		|| y > (gObj[aIndex].Y + 8))
	{
		return false;
	}

	int mapnumber = gObj[aIndex].MapNumber;

	if(mapnumber < 0 || mapnumber >= MAX_NUMBER_MAP)
	{
		return false;
	}

	if(IMPERIAL_MAP_RANGE(mapnumber))
	{
		return false;
	}

	unsigned char attr = MapC[mapnumber].GetAttr(x,y);

	if(attr)
	{
		return false;
	}

	attr = MapC[mapnumber].GetAttr(gObj[aIndex].X,gObj[aIndex].Y);

	if(attr & 1)
	{
		return false;
	}

	return true;
}

bool gObjCheckMoveArea(int aIndex, int X, int Y)
{
	if(!ObjectMaxRange(aIndex))
		return false;

	LPOBJ lpObj = &gObj[aIndex];

	int iXDiff = abs(lpObj->X - X);
	int iYDiff = abs(lpObj->Y - Y);

	if (iXDiff > 30 || iYDiff > 30)
	{
		return false;
	}

	return true;
}



BOOL gObjCheckAttackAreaUsedPath(int aIndex, int TarObjIndex)
{
	int X,Y;
	if(gObj[aIndex].PathCount > 1)
	{
		 X = gObj[aIndex].PathX[gObj[aIndex].PathCount-1];
		 Y = gObj[aIndex].PathY[gObj[aIndex].PathCount-1];
	}
	else
	{
		return false;
	}

	if(gObj[TarObjIndex].X < (X - 10)
		|| gObj[TarObjIndex].X > (X + 10)
		|| gObj[TarObjIndex].Y < (Y - 10)
		|| gObj[TarObjIndex].Y > (Y + 10))
	{
		return false;
	}
	return true;
}




BOOL gObjCheckattackAreaUsedViewPort(int aIndex, int TarObjIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj = &gObj[TarObjIndex];

	if(lpTargetObj->m_Index == lpObj->m_Index)
	{
		return true;
	}

	int MVL = MAX_VIEWPORT;

	if(lpObj->Type == OBJ_MONSTER)
	{
		MVL = MaxViewportMonster;
	}

	for(int n = 0; n < MVL;n++)
	{
		if(lpObj->VpPlayer[n].state)
		{
			if(lpObj->VpPlayer[n].number == lpTargetObj->m_Index)
			{
				return true;
			}
		}

		if(lpObj->VpPlayer2[n].state)
		{
			if(lpObj->VpPlayer2[n].number == lpTargetObj->m_Index)
			{
				return true;
			}
		}
	}
	return false;
}




int  gObjCheckAttackArea(int aIndex, int TarObjIndex)
{

	if(gObjCheckattackAreaUsedViewPort(aIndex,TarObjIndex)==0)
	{
		return 1;
	}

	int mapnumber = gObj[aIndex].MapNumber;

	if(mapnumber < 0 || mapnumber >= MAX_NUMBER_MAP)
	{
		return 2;
	}

	int tarmapnumber = gObj[TarObjIndex].MapNumber;

	if(tarmapnumber < 0 || tarmapnumber >= MAX_NUMBER_MAP)
	{
		return 3;
	}

	if(tarmapnumber != mapnumber)
	{
		return 4;
	}

	BYTE attr;

	if (g_ConfigRead.server.GetServerType() != SERVER_CASTLE)
	{
		attr = MapC[mapnumber].GetAttr(gObj[TarObjIndex].X, gObj[TarObjIndex].Y);

		if (attr && !(attr & 2))
		{
			return 5;
		}

	}

	else
	{
		attr = 0;
	}

	attr = MapC[mapnumber].GetAttr(gObj[aIndex].X,gObj[aIndex].Y);

	if(attr && !(attr&2))
	{
		return 6;
	}

	return false;
}




BOOL gUserFindDevilSquareInvitation(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	for(int n = 0; n < MAIN_INVENTORY_SIZE; n++)
	{
		if(lpObj->pInventory[n].IsItem() == 1)
		{
			if(lpObj->pInventory[n].m_Type == ITEMGET(14,19))
			{
				return true;
			}

			if(lpObj->pInventory[n].m_Type == ITEMGET(13,46))
			{
				return true;
			}
		}
	}
	return false;
}




BOOL gUserFindDevilSquareKeyEyes(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	int bFindKey = 0;
	int bFindEyes = 0;

	for(int n = 0; n < MAIN_INVENTORY_SIZE; n++)
	{
		if(lpObj->pInventory[n].IsItem() == 1)
		{
			if(lpObj->pInventory[n].m_Type == ITEMGET(14,17))
			{
				bFindEyes = 1;
			}

			if(lpObj->pInventory[n].m_Type == ITEMGET(14,18))
			{
				bFindKey = 1;
			}
		}
	}

	if(bFindKey != 0 && bFindEyes != 0)
	{
		return true;
	}
	return false;
}




void gObjSendUserStatistic(int aIndex, int startLevel, int endLevel)
{
	if(gDisconnect == 1)
	{
		return;
	}

	if(endLevel < startLevel)
	{
		endLevel = g_ConfigRead.data.common.UserMaxLevel;
	}

	int n;
	LPOBJ lpObj;
	int sClassCount[7];

	sClassCount[CLASS_WIZARD] = 0;
	sClassCount[CLASS_KNIGHT] = 0;
	sClassCount[CLASS_ELF] = 0;
	sClassCount[CLASS_MAGUMSA] = 0;
	sClassCount[CLASS_DARKLORD] = 0;
	sClassCount[CLASS_SUMMONER] = 0;
	sClassCount[CLASS_RAGEFIGHTER] = 0;

	for(int n = g_ConfigRead.server.GetObjectStartUserIndex(); n < g_ConfigRead.server.GetObjectMax(); n++)
	{
		lpObj = &gObj[n];

		if(lpObj->Connected > PLAYER_LOGGED)
		{
			if(lpObj->Level >= startLevel && lpObj->Level <= endLevel)
			{
				sClassCount[lpObj->Class]++;
			}

			sClassCount[7]++;
		}
	}

	MsgOutput(gObj[aIndex].m_Index,Lang.GetText(0,578),startLevel,endLevel,sClassCount[7],
		Lang.GetText(0,202),sClassCount[CLASS_WIZARD],
		Lang.GetText(0,203),sClassCount[CLASS_KNIGHT],
		Lang.GetText(0,204),sClassCount[CLASS_ELF],
		Lang.GetText(0,205),sClassCount[CLASS_MAGUMSA],
		Lang.GetText(0,206),sClassCount[CLASS_DARKLORD],
		Lang.GetText(0,207),sClassCount[CLASS_SUMMONER],
		Lang.GetText(0,208),sClassCount[CLASS_RAGEFIGHTER]);
}




LPOBJ gObjFind(char * targetcharname)
{
	if ( gDisconnect == TRUE )
	{
		return NULL;
	}

	int n;
	LPOBJ lpObj;

	for ( n = g_ConfigRead.server.GetObjectStartUserIndex() ; n<g_ConfigRead.server.GetObjectMax();n++)
	{
		lpObj = &gObj[n];

		if ( lpObj->Connected > PLAYER_LOGGED )
		{
			if ( lpObj->Name[0] == targetcharname[0] )
			{
				if ( strcmp(lpObj->Name, targetcharname) == 0 )
				{
					return lpObj;
				}
			}
		}
	}

	return NULL;

}




BOOL gObjFind10EventChip(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	int count = 0;

	for(int n = 0; n < MAIN_INVENTORY_SIZE;n++)
	{
		if(lpObj->pInventory[n].IsItem() == 1)
		{
			if(lpObj->pInventory[n].m_Type == ITEMGET(14,21))
			{
				count++;

				if(count >= 10)
				{
					return true;
				}
			}
		}
	}
	return false;
}




BOOL gObjDelete10EventChip(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];
	int count = 0;

	for(int n = 0; n < MAIN_INVENTORY_SIZE;n++)
	{
		if(lpObj->pInventory[n].IsItem() == 1)
		{
			if(lpObj->pInventory[n].m_Type == ITEMGET(14,21))
			{
				gObjInventoryDeleteItem(aIndex,n);
				GSProtocol.GCInventoryItemDeleteSend(aIndex,n,1);
				g_Log.Add("[EventChip] [%s][%s] Delete Rena (%d)",lpObj->AccountID,lpObj->Name,n);
				count++;

				if(count >= 10)
				{
					return true;
				}
			}
		}
	}
	return false;
}

void gObjSetPosition(int aIndex, int x, int y)
{
	if ( MAX_MAP_RANGE(gObj[aIndex].MapNumber) == FALSE )
		return;

	LPOBJ lpObj = &gObj[aIndex];
	PMSG_POSISTION_SET pMove;

	pMove.h.c = 0xC1;
	pMove.h.headcode = 0x15;
	pMove.h.size = sizeof(pMove);

	pMove.X = x;
	pMove.Y = y;

	lpObj->m_Rest = 0;

	MapC[lpObj->MapNumber].ClearStandAttr(lpObj->m_OldX,lpObj->m_OldY);
	MapC[lpObj->MapNumber].SetStandAttr(x,y);

	lpObj->m_OldX = x;
	lpObj->m_OldY = y;

	GSProtocol.RecvPositionSetProc(&pMove,lpObj->m_Index);
}




int  gObjGetItemCountInEquipment(int aIndex, int itemtype, int itemindex, int itemlevel)
{
	LPOBJ lpObj = &gObj[aIndex];
	int count = 0;

	for ( int n =0;n<MAX_PLAYER_EQUIPMENT ; n++ )
	{
		if ( lpObj->pInventory[n].IsItem() == TRUE )
		{
			if ( lpObj->pInventory[n].m_Type == ((itemtype * MAX_SUBTYPE_ITEMS) + itemindex) )
			{
				if ( itemlevel != -1 )
				{
					if (  lpObj->pInventory[n].m_Level != itemlevel )
					{
						continue;
					}
				}
				
				count++;
			}
		}
	}

	return count;
}





int  gObjGetItemCountInIventory(int aIndex, int itemtype, int itemindex, int itemlevel)
{
	LPOBJ lpObj = &gObj[aIndex];
	int count = 0;

	for ( int n =0;n<MAIN_INVENTORY_SIZE ; n++ )
	{
		if ( lpObj->pInventory[n].IsItem() == TRUE )
		{
			if ( lpObj->pInventory[n].m_Type == ((itemtype * MAX_SUBTYPE_ITEMS) + itemindex) )
			{
				if ( itemlevel != -1 )
				{
					if (  lpObj->pInventory[n].m_Level != itemlevel )
					{
						continue;
					}
				}
				
				count++;
			}
		}
	}

	return count;
}




int gObjGetItemCountInIventory(int aIndex, int itemnum)
{
	LPOBJ lpObj = &gObj[aIndex];
	int count = 0;

	for ( int n=0;n<MAIN_INVENTORY_SIZE;n++)
	{
		if ( lpObj->pInventory[n].IsItem() == TRUE )
		{
			if ( lpObj->pInventory[n].m_Type == itemnum )
			{
				count++;
			}
		}
	}

	return count;

}




int  gObjGetManaItemPos(int aIndex)	// [Failure : -1]
{
	LPOBJ lpObj = &gObj[aIndex];
	int count = 0;
	
	for ( int n=0;n<MAIN_INVENTORY_SIZE;n++)
	{
		if ( lpObj->pInventory[n].IsItem() == TRUE )
		{
			if ( lpObj->pInventory[n].m_Type == ITEMGET(14,4) || lpObj->pInventory[n].m_Type == ITEMGET(14,5) || lpObj->pInventory[n].m_Type == ITEMGET(14,6) )
			{
				return n;
			}
		}
	}

	return -1;
}




void gObjDelteItemCountInInventory(int aIndex, int itemtype, int itemindex, int count)
{
	LPOBJ lpObj = &gObj[aIndex];
	int delcount = 0;

	for(int n = 0; n < MAIN_INVENTORY_SIZE; n++)
	{
		if(lpObj->pInventory[n].IsItem() == 1)
		{
			if(lpObj->pInventory[n].m_Type == itemtype * 512 + itemindex)
			{
				gObjInventoryDeleteItem(aIndex,n);
				GSProtocol.GCInventoryItemDeleteSend(aIndex,n,1);
				g_Log.Add("[DeleteItem] [%s][%s] Delete (%d,%d)(%d)",lpObj->AccountID,lpObj->Name,itemtype,itemindex,n);
				delcount++;

				if(delcount >= count)
				{
					return;
				}
			}
		}
	}
}




void gObjGetStatPointState(int aIndex, short& AddPoint, short& MaxAddPoint, short& MinusPoint, short& MaxMinusPoint)
{
	if(gObj[aIndex].Level <= 5)
	{
		AddPoint = 0;
		MaxAddPoint = 0;
	}

	if(gObj[aIndex].Type != OBJ_USER)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	int total_point = lpObj->m_PlayerData->Strength + lpObj->m_PlayerData->Dexterity + lpObj ->m_PlayerData->Vitality + lpObj->m_PlayerData->Energy + lpObj->Leadership + lpObj->m_PlayerData->LevelUpPoint;
	int ori_point;

	if(lpObj->Class == CLASS_DARKLORD || lpObj->Class == CLASS_MAGUMSA || lpObj->Class == CLASS_RAGEFIGHTER)
	{
		ori_point = DCInfo.GetDefPoint(lpObj->Class) + (lpObj->Level - 1) * 7;
	}
	else
	{
		ori_point = DCInfo.GetDefPoint(lpObj->Class) + (lpObj->Level - 1) * 5;
	}

	if(g_QuestInfo.GetQuestState(lpObj,0) == 2)
	{
		ori_point+= 10;
	}

	if(g_QuestInfo.GetQuestState(lpObj,1) == 2)
	{
		ori_point+= 10;
	}

	if(g_QuestInfo.GetQuestState(lpObj,2) == 2)
	{
		ori_point+= lpObj->Level - 220;
	}

	//Season3 Quest MaxPoint Calculation
	if(g_QuestInfo.GetQuestState(lpObj,4) == 2)
	{
		ori_point+= 20;
	}

	if(g_QuestInfo.GetQuestState(lpObj,5) == 2)
	{
		ori_point+= 20;
	}

	if(g_QuestInfo.GetQuestState(lpObj,6) == 2)
	{
		ori_point+= 30;
	}

	int addpoint = ori_point;

	addpoint += gStatMng.GetMaxStat(lpObj->Level,lpObj->Class);

	AddPoint = total_point - ori_point;
	MaxAddPoint = gStatMng.GetMaxStat(lpObj->Level,lpObj->Class);
	MinusPoint = lpObj->m_PlayerData->iFruitPoint;
	MaxMinusPoint = gStatMng.GetMaxMinusStat(lpObj->Level,lpObj->Class);
}




BOOL gObjCheckStatPointUp(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Type != OBJ_USER)
	{
		return FALSE;
	}

	int total_point = lpObj->m_PlayerData->Strength + lpObj->m_PlayerData->Dexterity + lpObj->m_PlayerData->Vitality + lpObj->m_PlayerData->Energy + lpObj->Leadership + lpObj->m_PlayerData->LevelUpPoint;
	int ori_point;

	if(lpObj->Class == CLASS_DARKLORD || lpObj->Class == CLASS_MAGUMSA || lpObj->Class == CLASS_RAGEFIGHTER)
	{
		ori_point = DCInfo.GetDefPoint(lpObj->Class) + (lpObj->Level - 1) * 7;
	}
	else
	{
		ori_point = DCInfo.GetDefPoint(lpObj->Class) + (lpObj->Level - 1) * 5;
	}

	if(g_QuestInfo.GetQuestState(lpObj,0) == 2)
	{
		ori_point+= 10;
	}

	if(g_QuestInfo.GetQuestState(lpObj,1) == 2)
	{
		ori_point+= 10;
	}

	if(g_QuestInfo.GetQuestState(lpObj,2) == 2)
	{
		ori_point+= lpObj->Level - 220;
	}

	//Season3 Quest MaxPoint Calculation
	if(g_QuestInfo.GetQuestState(lpObj,4) == 2)
	{
		ori_point+= 20;
	}

	if(g_QuestInfo.GetQuestState(lpObj,5) == 2)
	{
		ori_point+= 20;
	}

	if(g_QuestInfo.GetQuestState(lpObj,6) == 2)
	{
		ori_point+= 30;
	}

	ori_point += gStatMng.GetMaxStat(lpObj->Level,lpObj->Class);

	if(total_point < ori_point)
	{
		return true;
	}
	return false;
}




BOOL gObjCheckStatPointDown(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Type != OBJ_USER)
	{
		return FALSE;
	}

	int total_point = lpObj->m_PlayerData->Strength + lpObj->m_PlayerData->Dexterity + lpObj->m_PlayerData->Vitality + lpObj->m_PlayerData->Energy + lpObj->m_PlayerData->LevelUpPoint;
	int ori_point;

	if(lpObj->Class == CLASS_DARKLORD || lpObj->Class == CLASS_MAGUMSA || lpObj->Class == CLASS_RAGEFIGHTER)
	{
		ori_point = DCInfo.GetDefPoint(lpObj->Class) + (lpObj->Level - 1) * 7;
	}
	else
	{
		ori_point = DCInfo.GetDefPoint(lpObj->Class) + (lpObj->Level - 1) * 5;
	}

	if(g_QuestInfo.GetQuestState(lpObj,0) == 2)
	{
		ori_point+= 10;
	}

	if(g_QuestInfo.GetQuestState(lpObj,1) == 2)
	{
		ori_point+= 10;
	}

	//Aplied Webzen Fix for MaxPoint Calculation
	if(g_QuestInfo.GetQuestState(lpObj,2) == 2)
	{
		ori_point+= lpObj->Level - 220;
	}

	//Season3 Quest MaxPoint Calculation
	if(g_QuestInfo.GetQuestState(lpObj,4) == 2)
	{
		ori_point+= 20;
	}

	if(g_QuestInfo.GetQuestState(lpObj,5) == 2)
	{
		ori_point+= 20;
	}

	if(g_QuestInfo.GetQuestState(lpObj,6) == 2)
	{
		ori_point+= 30;
	}

	ori_point += gStatMng.GetMaxStat(lpObj->Level,lpObj->Class);

	if(total_point > ori_point)
	{
		return true;
	}
	return false;
}




void gObjUseCircle(int aIndex, int pos)
{
	int level = gObj[aIndex].pInventory[pos].m_Level;
	int iSerial = gObj[aIndex].pInventory[pos].m_Number;

	PMSG_DEFRESULT pResult;

	PHeadSetB((LPBYTE)&pResult,0x2C,sizeof(pResult));

	pResult.result = 0;

	if(gObj[aIndex].Level <= 10 || level > 3)
	{
		pResult.result |= 0xC0;
		IOCP.DataSend(aIndex,(unsigned char*)&pResult,pResult.h.size);
		return;
	}

	if(gObjCheckStatPointUp(aIndex)==0)
	{
		pResult.result |= 0xC0;
		IOCP.DataSend(aIndex,(unsigned char*)&pResult,pResult.h.size);
		return;
	}

	g_Log.Add("[StatUp] [%s][%s] Use StatUpItem Level:%d Pos:%d serial:%I64d",gObj[aIndex].AccountID,gObj[aIndex].Name,level,pos,iSerial);

	gObjInventoryItemSet(aIndex,pos,0xFF);
	gObj[aIndex].pInventory[pos].Clear();
	GSProtocol.GCInventoryItemDeleteSend(aIndex,pos,1);

	short AddPoint;
	short MaxAddPoint;
	short MinusPoint;
	short MaxMinusPoint;

	gObjGetStatPointState(aIndex,(short &)AddPoint,(short &)(MaxAddPoint),(short &)MinusPoint,(short &)MaxMinusPoint);

	int iSuccessRate = 0;

	if(AddPoint <= 10)
	{
		iSuccessRate = 100;
	}
	else if((AddPoint - 10) < MaxAddPoint * (double)0.1)
	{
		iSuccessRate = 90;
	}
	else if((AddPoint - 10) < MaxAddPoint * (double)0.3)
	{
		iSuccessRate = 80;
	}
	else if((AddPoint - 10) < MaxAddPoint * (double)0.5)
	{
		iSuccessRate = 70;
	}
	else if((AddPoint - 10) < MaxAddPoint * (double)0.8)
	{
		iSuccessRate = 60;
	}
	else
	{
		iSuccessRate = 50;
	}

	int incvalue = 0;

	if(rand()%100 < iSuccessRate)
	{
		iSuccessRate = rand()%100;

		int AddStat = 0;

		if(iSuccessRate < 70)
		{
			AddStat = 1;
		}
		else if(iSuccessRate < 95)
		{
			AddStat = 2;
		}
		else
		{
			AddStat = 3;
		}

		if((AddStat + AddPoint) > MaxAddPoint)
		{
			AddStat = 1;
		}

		pResult.result |= level * 16;
		pResult.result |= AddStat;

		IOCP.DataSend(aIndex,(unsigned char *)&pResult,pResult.h.size);

		incvalue = AddStat;

		g_Log.Add("[StatUp] [%s][%s] Success [%d][%d] %d",gObj[aIndex].AccountID,gObj[aIndex].Name,level,incvalue,iSerial);
	}
	else
	{
		pResult.result |= 0x40;
		pResult.result |= level * 16;
		pResult.result = pResult.result;

		IOCP.DataSend(aIndex,(unsigned char *)&pResult,pResult.h.size);
		g_Log.Add("[StatUp] [%s][%s] Fail [%d] %d",gObj[aIndex].AccountID,gObj[aIndex].Name,level,iSerial);
		return;
	}

	switch(level)
	{
	case 3:
		gObj[aIndex].m_PlayerData->Strength += incvalue;
		break;
	case 2:
		gObj[aIndex].m_PlayerData->Dexterity += incvalue;
		break;
	case 1:
		gObj[aIndex].m_PlayerData->Vitality += incvalue;
		gObj[aIndex].MaxLife += gObj[aIndex].VitalityToLife * incvalue;
		GSProtocol.GCReFillSend(aIndex,gObj[aIndex].MaxLife+gObj[aIndex].AddLife,0xFE,0,gObj[aIndex].iAddShield+gObj[aIndex].iMaxShield);
		break;
	case 0:
		gObj[aIndex].m_PlayerData->Energy += incvalue;
		gObj[aIndex].MaxMana += gObj[aIndex].EnergyToMana * incvalue;
		gObjSetBP(aIndex);
		GSProtocol.GCManaSend(gObj[aIndex].m_Index,gObj[aIndex].MaxMana + gObj[aIndex].AddMana,0xFE,0,gObj[aIndex].MaxBP);
		break;
	default: break;
	}

	gObjCalCharacter.CalcCharacter(aIndex);
}

void gObjUsePlusStatFruit(int aIndex,int pos)
{
	if(!ObjectMaxRange(aIndex))
	{
		return;
	}

	if(gObj[aIndex].Class > 6 || gObj[aIndex].Class < 0)
	{
		return;
	}

	int iItemLevel = gObj[aIndex].pInventory[pos].m_Level;
	int iItemSerial = gObj[aIndex].pInventory[pos].m_Number;
	int iCharacterClass = gObj[aIndex].Class;

	PMSG_USE_STAT_FRUIT pResult;

	PHeadSetB((LPBYTE)&pResult,0x2C,sizeof(pResult));

	pResult.result = 2;
	pResult.btFruitType = iItemLevel;
	pResult.btStatValue = 0;

	if(gObj[aIndex].Level <= 9 || iItemLevel > 4)
	{
		pResult.result = 2;
		IOCP.DataSend(aIndex,(unsigned char *)&pResult,pResult.h.size);
		return;
	}

	if(iItemLevel == 4 && gObj[aIndex].Class != 4)
	{
		pResult.result = 2;
		IOCP.DataSend(aIndex,(unsigned char *)&pResult,pResult.h.size);
		return;
	}

	for(int iItemIndex = 0; iItemIndex < INVETORY_WEAR_SIZE; iItemIndex++)
	{
		if(gObj[aIndex].pInventory[iItemIndex].IsItem())
		{
			if (g_ConfigRead.data.common.joinmuFruitRequireUnWearItem == true)
			{
				pResult.result = 16;
				IOCP.DataSend(aIndex, (unsigned char *)&pResult, pResult.h.size);
				return;
			}
		}
	}

	if(gObjCheckStatPointUp(aIndex) == false)
	{
		pResult.result = 33;
		IOCP.DataSend(aIndex,(unsigned char *)&pResult,pResult.h.size);
		return;
	}

	g_Log.Add("[StatUp] [%s][%s] Use StatUpItem Level:%d Pos:%d serial:%I64d",
		gObj[aIndex].AccountID,gObj[aIndex].Name,iItemLevel,pos,iItemSerial);

	gObjInventoryItemSet(aIndex,pos,0xFF);
	gObj[aIndex].pInventory[pos].Clear();
	GSProtocol.GCInventoryItemDeleteSend(aIndex,pos,1);

	short AddPoint;
	short MaxAddPoint;
	short MinusPoint;
	short MaxMinusPoint;

	gObjGetStatPointState(aIndex,(short &)AddPoint,(short &)MaxAddPoint,(short &)MinusPoint,(short &)MaxMinusPoint);

	int iSuccessRate = 0;

	if(AddPoint <= 10)
	{
		iSuccessRate = 100;
	}
	else if((AddPoint - 10) < (MaxAddPoint * (double)0.1))
	{
		iSuccessRate = 90;
	}
	else if((AddPoint - 10) < (MaxAddPoint * (double)0.3))
	{
		iSuccessRate = 80;
	}
	else if((AddPoint - 10) < (MaxAddPoint * (double)0.5))
	{
		iSuccessRate = 70;
	}
	else if((AddPoint - 10) < (MaxAddPoint * (double)0.8))
	{
		iSuccessRate = 60;
	}
	else
	{
		iSuccessRate = 50;
	}

	int iIncStat = 0;
	int bSuccess = 0;

	if(rand()%100 < iSuccessRate)
	{
		bSuccess = 1;
	}

	if(bSuccess != 0)
	{
		iSuccessRate = rand()%100;

		int AddStat = 0;

		if(iSuccessRate < 70)
		{
			AddStat = 1;
		}
		else if(iSuccessRate < 95)
		{
			AddStat = 2;
		}
		else
		{
			AddStat = 3;
		}

		if((AddStat + AddPoint) > MaxAddPoint)
		{
			AddStat = 1;
		}

		pResult.result = 0;
		pResult.btStatValue = AddStat;
		pResult.btFruitType = iItemLevel;

		IOCP.DataSend(aIndex,(unsigned char *)&pResult,pResult.h.size);
		iIncStat = AddStat;

		g_Log.Add("[StatUp] [%s][%s] Success [%d][%d] %d",
			gObj[aIndex].AccountID,gObj[aIndex].Name,iItemLevel,iIncStat,iItemSerial);
	}
	else
	{
		pResult.result = 1;
		pResult.btStatValue = 0;
		pResult.btFruitType = iItemLevel;

		IOCP.DataSend(aIndex,(unsigned char *)&pResult,pResult.h.size);

		g_Log.Add("[StatUp] [%s][%s] Fail [%d] %d",
			gObj[aIndex].AccountID,gObj[aIndex].Name,iItemLevel,iItemSerial);
		return;
	}

	switch(iItemLevel)
	{
	case 4: gObj[aIndex].Leadership += iIncStat;	break;
	case 3: gObj[aIndex].m_PlayerData->Strength += iIncStat;	break;
	case 2: gObj[aIndex].m_PlayerData->Dexterity += iIncStat;	break;
	case 1:
		gObj[aIndex].m_PlayerData->Vitality += iIncStat;
		gObj[aIndex].MaxLife += gObj[aIndex].VitalityToLife * iIncStat;
		break;
	case 0:
		gObj[aIndex].m_PlayerData->Energy += iIncStat;
		gObj[aIndex].MaxMana += gObj[aIndex].EnergyToMana * iIncStat;
		gObjSetBP(aIndex);
		break;
	default : break;
	}

	gObjCalCharacter.CalcCharacter(aIndex);
	GSProtocol.GCReFillSend(aIndex,gObj[aIndex].MaxLife+gObj[aIndex].AddLife,0xFE,0,gObj[aIndex].iAddShield+gObj[aIndex].iMaxShield);
	gObjSetBP(aIndex);
	GSProtocol.GCManaSend(gObj[aIndex].m_Index,gObj[aIndex].MaxMana + gObj[aIndex].AddMana,0xFE,0,gObj[aIndex].MaxBP + gObj[aIndex].AddBP);
}


void gObjUseMinusStatFruit(int aIndex, int pos)
{
	if(!ObjectMaxRange(aIndex))
	{
		return;
	}

	if(gObj[aIndex].Class > 6 || gObj[aIndex].Class < 0)
	{
		return;
	}

	int iItemLevel = gObj[aIndex].pInventory[pos].m_Level;
	int iItemSerial = gObj[aIndex].pInventory[pos].m_Number;
	int iCharacterClass = gObj[aIndex].Class;

	PMSG_USE_STAT_FRUIT pResult;

	PHeadSetB((LPBYTE)&pResult,0x2C,sizeof(pResult));

	pResult.result = 5;
	pResult.btFruitType = iItemLevel;
	pResult.btStatValue = 0;

	if(gObj[aIndex].Level <= 9 || iItemLevel > 4)
	{
		pResult.result = 5;
		IOCP.DataSend(aIndex,(unsigned char *)&pResult,pResult.h.size);
		return;
	}

	if(iItemLevel == 4 && gObj[aIndex].Class != 4)
	{
		pResult.result = 5;
		IOCP.DataSend(aIndex,(unsigned char *)&pResult,pResult.h.size);
		return;
	}

	for(int iItemIndex = 0; iItemIndex < INVETORY_WEAR_SIZE; iItemIndex++)
	{
		if(gObj[aIndex].pInventory[iItemIndex].IsItem())
		{
			if (g_ConfigRead.data.common.joinmuFruitRequireUnWearItem == true)
			{
				pResult.result = 16;
				IOCP.DataSend(aIndex, (unsigned char *)&pResult, pResult.h.size);
				return;
			}
		}
	}

	int iDefaultStat = 0;
	int iPresentStat = 0;
	int bEnableUseFruit = 1;

	if(iItemLevel == 0)
	{
		iDefaultStat = DCInfo.DefClass[iCharacterClass].Energy;
		iPresentStat = gObj[aIndex].m_PlayerData->Energy;

		if(iPresentStat <= iDefaultStat)
		{
			bEnableUseFruit = 0;
		}
	}
	else if(iItemLevel == 1)
	{
		iDefaultStat = DCInfo.DefClass[iCharacterClass].Vitality;
		iPresentStat = gObj[aIndex].m_PlayerData->Vitality;

		if(iPresentStat <= iDefaultStat)
		{
			bEnableUseFruit = 0;
		}
	}
	else if(iItemLevel == 2)
	{
		iDefaultStat = DCInfo.DefClass[iCharacterClass].Dexterity;
		iPresentStat = gObj[aIndex].m_PlayerData->Dexterity;

		if(iPresentStat <= iDefaultStat)
		{
			bEnableUseFruit = 0;
		}
	}
	else if(iItemLevel == 3)
	{
		iDefaultStat = DCInfo.DefClass[iCharacterClass].Strength;
		iPresentStat = gObj[aIndex].m_PlayerData->Strength;

		if(iPresentStat <= iDefaultStat)
		{
			bEnableUseFruit = 0;
		}
	}
	else if(iItemLevel == 4)
	{
		iDefaultStat = DCInfo.DefClass[iCharacterClass].Leadership;
		iPresentStat = gObj[aIndex].Leadership;

		if(iPresentStat <= iDefaultStat)
		{
			bEnableUseFruit = 0;
		}
	}

	if(bEnableUseFruit == 0)
	{
		pResult.result = 38;
		IOCP.DataSend(aIndex,(unsigned char *)&pResult,pResult.h.size);
		return;
	}

	short AddPoint;
	short MaxAddPoint;
	short MinusPoint;
	short MaxMinusPoint = 0;

	gObjGetStatPointState(aIndex,(short &)AddPoint,(short &)MaxAddPoint,(short &)MinusPoint,(short &)MaxMinusPoint);

	if(MinusPoint >= MaxMinusPoint || MinusPoint < 0)
	{
		pResult.result = 37;
		IOCP.DataSend(aIndex,(unsigned char *)&pResult,pResult.h.size);
		return;
	}

	g_Log.Add("[StatDown] [%s][%s] Use StatDownItem Level:%d Pos:%d serial:%I64d",
		gObj[aIndex].AccountID,gObj[aIndex].Name,iItemLevel,pos,iItemSerial);

	gObjInventoryItemSet(aIndex,pos,0xFF);
	gObj[aIndex].pInventory[pos].Clear();
	GSProtocol.GCInventoryItemDeleteSend(aIndex,pos,1);

	int iDecStat = 0;
	int bSuccess = 0;

	int iSuccessRate = 100 - gObj[aIndex].Level / 6;

	if(iSuccessRate < 0)
	{
		iSuccessRate = 0;
	}

	if(rand()%100 < iSuccessRate)
	{
		bSuccess = 1;
	}

	if(MinusPoint <= 10)
	{
		bSuccess = 1;
	}

	if(bSuccess != 0)
	{
		iSuccessRate = rand()%100;

		if(iSuccessRate < 50)
		{
			iDecStat = 1;
		}
		else if(iSuccessRate < 75)
		{
			iDecStat = 3;
		}
		else if(iSuccessRate < 91)
		{
			iDecStat = 5;
		}
		else if(iSuccessRate < 98)
		{
			iDecStat = 7;
		}
		else
		{
			iDecStat = 9;
		}

		if((MinusPoint + iDecStat) >= MaxMinusPoint)
		{
			iDecStat = MaxMinusPoint - MinusPoint;
		}

		if((iPresentStat - iDecStat) < iDefaultStat)
		{
			iDecStat = iPresentStat - iDefaultStat;
		}

		pResult.result = 3;
		pResult.btFruitType = iItemLevel;
		pResult.btStatValue = iDecStat;

		IOCP.DataSend(aIndex,(unsigned char *)&pResult,pResult.h.size);
		g_Log.Add("[StatDown] [%s][%s] Success [%d][%d] %d",
			gObj[aIndex].AccountID,gObj[aIndex].Name,iItemLevel,iDecStat,iItemSerial);
	}
	else
	{
		pResult.result = 4;
		pResult.btFruitType = iItemLevel;
		pResult.btStatValue = 0;
		IOCP.DataSend(aIndex,(unsigned char *)&pResult,pResult.h.size);
		g_Log.Add("[StatDown] [%s][%s] Fail [%d] %d",
			gObj[aIndex].AccountID,gObj[aIndex].Name,iItemLevel,iItemSerial);
		return;
	}

	switch(iItemLevel)
	{
	case 4:	gObj[aIndex].Leadership -= iDecStat;	break;
	case 3: gObj[aIndex].m_PlayerData->Strength -= iDecStat;	break;
	case 2: gObj[aIndex].m_PlayerData->Dexterity -= iDecStat;	break;
	case 1:

		gObj[aIndex].m_PlayerData->Vitality -= iDecStat;
		gObj[aIndex].MaxLife -= gObj[aIndex].VitalityToLife *iDecStat;
		break;

	case 0:
		gObj[aIndex].m_PlayerData->Energy -= iDecStat;
		gObj[aIndex].MaxMana -= gObj[aIndex].EnergyToMana * iDecStat;
		gObjSetBP(aIndex);
		break;

	default:
		break;
	}

	int iOldLevelUpPoint = gObj[aIndex].m_PlayerData->LevelUpPoint;
	gObj[aIndex].m_PlayerData->LevelUpPoint += iDecStat;

	int iOldFruitPoint = gObj[aIndex].m_PlayerData->LevelUpPoint;
	gObj[aIndex].m_PlayerData->iFruitPoint += iDecStat;

	g_Log.Add("[StatDown] [%s][%s] MinusStat[%d] -> LevelUpPoint Old(%d)/New(%d)  FruitPoint Old(%d)/New(%d)",
		gObj[aIndex].AccountID,gObj[aIndex].Name,iDecStat,iOldLevelUpPoint,gObj[aIndex].m_PlayerData->LevelUpPoint,iOldFruitPoint,gObj[aIndex].m_PlayerData->iFruitPoint);

	gObjCalCharacter.CalcCharacter(aIndex);
	GSProtocol.GCReFillSend(aIndex,gObj[aIndex].MaxLife+gObj[aIndex].AddLife,0xFE,0,gObj[aIndex].iAddShield+gObj[aIndex].iMaxShield);
	gObjSetBP(aIndex);
	GSProtocol.GCManaSend(gObj[aIndex].m_Index,gObj[aIndex].MaxMana + gObj[aIndex].AddMana,0xFE,0,gObj[aIndex].MaxBP);
}

void CashShopExMinusStatFruit(int aIndex, int pos) //GS-CS Need Decompile
{
	if(gObj[aIndex].Class > 6 || gObj[aIndex].Class < 0)
	{
		return;
	}
	
	int iItemLevel = 0;

	switch(gObj[aIndex].pInventory[pos].m_Type)
	{
		case 6710:
			iItemLevel = 3;
			break;
		case 6711:
			iItemLevel = 2;
			break;
		case 6712:
			iItemLevel = 1;
			break;
		case 6713:
			iItemLevel = 0;
			break;
		case 6714:
			iItemLevel = 4;
			break;
	}

	int iItemSerial = gObj[aIndex].pInventory[pos].m_Number;
	int iCharacterClass = gObj[aIndex].Class;
	int iStatPoint = gObj[aIndex].pInventory[pos].m_Durability * 10.0;

	PMSG_USE_STAT_FRUIT pResult;
	PHeadSetB((LPBYTE)&pResult, 0x2C, sizeof(pResult));

	pResult.result = 8;
	pResult.btFruitType = iItemLevel;
	pResult.btStatValue = 0;

	for(int iItemIndex = 0; iItemIndex < INVETORY_WEAR_SIZE; iItemIndex++)
	{
		if(gObj[aIndex].pInventory[iItemIndex].IsItem())
		{
			if (g_ConfigRead.data.common.joinmuFruitRequireUnWearItem == true)
			{
				pResult.result = 16;
				IOCP.DataSend(aIndex, (unsigned char *)&pResult, pResult.h.size);
				return;
			}
		}
	}

	bool bEnableFruit = true;

	switch ( pResult.btFruitType )
	{
		case 3:
			if(gObj[aIndex].m_PlayerData->Strength == DCInfo.DefClass[iCharacterClass].Strength)
			{
				bEnableFruit = false;
			}
			break;
		case 2:
			if(gObj[aIndex].m_PlayerData->Dexterity == DCInfo.DefClass[iCharacterClass].Dexterity)
			{
				bEnableFruit = false;
			}
			break;
		case 1:
			if(gObj[aIndex].m_PlayerData->Vitality == DCInfo.DefClass[iCharacterClass].Vitality)
			{
				bEnableFruit = false;
			}
			break;
		case 0:
			if(gObj[aIndex].m_PlayerData->Energy == DCInfo.DefClass[iCharacterClass].Energy)
			{
				bEnableFruit = false;
			}
			break;
		case 5:
			if(gObj[aIndex].Leadership = DCInfo.DefClass[iCharacterClass].Leadership)
			{
				bEnableFruit = false;
			}
			break;
	}

	int iDefaultStat, iPresentStat;

	switch ( pResult.btFruitType )
	{
		case 3:
			iDefaultStat = DCInfo.DefClass[iCharacterClass].Strength;
			iPresentStat = gObj[aIndex].m_PlayerData->Strength;
			if(iPresentStat - iStatPoint < iDefaultStat)
				iStatPoint = iPresentStat - iDefaultStat;
			break;
		case 2:
			iDefaultStat = DCInfo.DefClass[iCharacterClass].Dexterity;
			iPresentStat = gObj[aIndex].m_PlayerData->Dexterity;
			if(iPresentStat - iStatPoint < iDefaultStat)
				iStatPoint = iPresentStat - iDefaultStat;
			break;
		case 1:
			iDefaultStat = DCInfo.DefClass[iCharacterClass].Vitality;
			iPresentStat = gObj[aIndex].m_PlayerData->Vitality;
			if(iPresentStat - iStatPoint < iDefaultStat)
				iStatPoint = iPresentStat - iDefaultStat;
			break;
		case 0:
			iDefaultStat = DCInfo.DefClass[iCharacterClass].Energy;
			iPresentStat = gObj[aIndex].m_PlayerData->Energy;
			if(iPresentStat - iStatPoint < iDefaultStat)
				iStatPoint = iPresentStat - iDefaultStat;
			break;
		case 4:
			iDefaultStat = DCInfo.DefClass[iCharacterClass].Leadership;
			iPresentStat = gObj[aIndex].Leadership;
			if(iPresentStat - iStatPoint < iDefaultStat)
				iStatPoint = iPresentStat - iDefaultStat;
			break;
	}

	if(bEnableFruit)
	{
		g_Log.Add("[InGameShop][ExMinusStatPoint] [%s][%s] Use StatDownItem Level:%d,Pos:%d,Dur:%d,Serial:%I64d",
			gObj[aIndex].AccountID, gObj[aIndex].Name, gObj[aIndex].pInventory[pos].m_Level,
			pos, gObj[aIndex].pInventory[pos].m_Durability, iItemSerial);

		gObjInventoryItemSet(aIndex,pos,0xFF);
		gObj[aIndex].pInventory[pos].Clear();
		GSProtocol.GCInventoryItemDeleteSend(aIndex,pos,1);
		
		pResult.result = 6;
		pResult.btFruitType = iItemLevel;
		pResult.btStatValue = iStatPoint;

		IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

		switch(iItemLevel)
		{
			case 4:	gObj[aIndex].Leadership -= iStatPoint;	break;
			case 3: gObj[aIndex].m_PlayerData->Strength -= iStatPoint;	break;
			case 2: gObj[aIndex].m_PlayerData->Dexterity -= iStatPoint;	break;
			case 1:
				gObj[aIndex].m_PlayerData->Vitality -= iStatPoint;
				gObj[aIndex].MaxLife -= gObj[aIndex].VitalityToLife *iStatPoint;
				break;

			case 0:
				gObj[aIndex].m_PlayerData->Energy -= iStatPoint;
				gObj[aIndex].MaxMana -= gObj[aIndex].EnergyToMana * iStatPoint;
				gObjSetBP(aIndex);
				break;
		}
		gObj[aIndex].m_PlayerData->LevelUpPoint += iStatPoint;
		gObjCalCharacter.CalcCharacter(aIndex);
	}
	else
	{
		pResult.result = 38;
		IOCP.DataSend(aIndex, (LPBYTE)&pResult, pResult.h.size);

		g_Log.Add("[InGameShop][ExMinusStatPoint] [%s][%s] MinusStat[%d]",
		gObj[aIndex].AccountID,gObj[aIndex].Name,iStatPoint);

		gObjCalCharacter.CalcCharacter(aIndex);
		GSProtocol.GCReFillSend(aIndex,gObj[aIndex].MaxLife+gObj[aIndex].AddLife,0xFE,0,gObj[aIndex].iAddShield+gObj[aIndex].iMaxShield);
		gObjSetBP(aIndex);
		GSProtocol.GCManaSend(gObj[aIndex].m_Index,gObj[aIndex].MaxMana + gObj[aIndex].AddMana,0xFE,0,gObj[aIndex].MaxBP);
	}
}



void gObjCalcMaxLifePower(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	switch(lpObj->Class)
	{
	case CLASS_WIZARD:
			lpObj->m_MaxLifePower = lpObj->Level + lpObj->m_PlayerData->Vitality + 110;
		break;
	case CLASS_KNIGHT:
			lpObj->m_MaxLifePower = lpObj->Level*2 + lpObj->m_PlayerData->Vitality + 60;
		break;
	case CLASS_ELF:
			lpObj->m_MaxLifePower = lpObj->Level + lpObj->m_PlayerData->Vitality + 80;
		break;
	case CLASS_MAGUMSA:
	case CLASS_DARKLORD:
			lpObj->m_MaxLifePower = lpObj->Level + lpObj->m_PlayerData->Vitality + 110;
		break;
			
	default :
		lpObj->m_MaxLifePower = 0;
		break;
	}
}




void gObjDelayLifeCheck(int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->m_CheckLifeTime > 0)
	{
		lpObj->m_CheckLifeTime--;

		if(lpObj->m_CheckLifeTime <= 0)
		{
			lpObj->m_CheckLifeTime = 0;

			if(lpObj->Life < 0)
			{
				lpObj->Life = 0;
			}

			if(lpObj->lpAttackObj != 0)
			{
				gObjLifeCheck(lpObj,lpObj->lpAttackObj,0,1,0,0,0,0,0);
			}
		}
	}
}

BOOL gObjDuelCheck(LPOBJ lpObj)
{
	BOOL bRetVal = FALSE;
	if( lpObj->Type == OBJ_USER )
	{
		int iDuelIndex = lpObj->m_iDuelUser;
		if( CHECK_LIMIT(iDuelIndex, g_ConfigRead.server.GetObjectMax()) )
		{
			bRetVal = TRUE;
		}
	}
	return bRetVal;
}

BOOL gObjDuelCheck(LPOBJ lpObj, LPOBJ lpTargetObj)
{
	BOOL bRetVal = FALSE;
	if( lpObj->Type == OBJ_USER && lpTargetObj->Type == OBJ_USER)
	{
		if( lpObj->m_iDuelUser == lpTargetObj->m_Index && lpTargetObj->m_iDuelUser == lpObj->m_Index)
		{
			bRetVal = TRUE;
		}
	}
	return bRetVal;
}

void gObjResetDuel(LPOBJ lpObj)
{
	int iDuelIndex = lpObj->m_iDuelUser;

	if ( ObjectMaxRange(iDuelIndex))
	{
		lpObj->m_iDuelUser = -1;
		lpObj->m_iDuelUserReserved = -1;
		lpObj->m_btDuelScore = 0;
		lpObj->m_iDuelTickCount = 0;

		if (lpObj->Type == OBJ_USER && lpObj->m_PlayerData->ISBOT == false && lpObj->m_Index >= g_ConfigRead.server.GetObjectStartUserIndex())
		{
			gDarkSpirit[lpObj->m_Index - g_ConfigRead.server.GetObjectStartUserIndex()].ReSetTarget(iDuelIndex);
			gDarkSpirit[iDuelIndex - g_ConfigRead.server.GetObjectStartUserIndex()].ReSetTarget(lpObj->m_Index);
			g_CMuunSystem.ReSetTarget(lpObj, iDuelIndex);
			g_CMuunSystem.ReSetTarget(&gObj[iDuelIndex], lpObj->m_Index);
		}

		gObj[iDuelIndex].m_iDuelUser = -1;
		gObj[iDuelIndex].m_iDuelUserReserved = -1;
		gObj[iDuelIndex].m_iDuelUserRequested = -1;
		gObj[iDuelIndex].m_iDuelTickCount = 0;
	}
}

void SkillFrustrum(BYTE bangle, int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	vec3_t p[4];
	Vector(-6, 6, 0, p[0]);
	Vector(6, 6, 0, p[1]);
	Vector(1, 0, 0, p[2]);
	Vector(-1, 0, 0, p[3]);

	vec3_t Angle;

	Vector(0.f, 0.f, (bangle * 360 / 255), Angle);

	float Matrix[3][4];

	AngleMatrix(Angle,Matrix);

	vec3_t vFrustrum[4];

	for(int i = 0; i < 4; i++)
	{
		VectorRotate(p[i],Matrix,vFrustrum[i]);

		lpObj->fSkillFrustrumX[i] = (int)vFrustrum[i][0] + lpObj->X;
		lpObj->fSkillFrustrumY[i] = (int)vFrustrum[i][1] + lpObj->Y;
	}
}




BOOL SkillTestFrustrum(int x, int y,int aIndex)
{
	LPOBJ lpObj = &gObj[aIndex];

	int j = 3;

	if(gObj[aIndex].m_bOffLevel)
		return true;

	for(int i = 0; i < MAX_ARRAY_FRUSTRUM; j = i,i++)
	{
		if((lpObj->fSkillFrustrumX[i]- x) * (lpObj->fSkillFrustrumY[j]-y) - (lpObj->fSkillFrustrumX[j]-x) * (lpObj->fSkillFrustrumY[i]-y) < 0.0f)
		{
			return false;
		}
	}
	return true;
}



BOOL gObjCheckMaxZen(int aIndex, int nAddZen)
{
	if ( !ObjectMaxRange(aIndex) )
		return FALSE;

	LPOBJ lpObj = &gObj[aIndex];
	
	__int64 _Money = 0;
	_Money = (__int64)lpObj->m_PlayerData->Money + (__int64)nAddZen;

	if ( _Money > (__int64)MAX_ZEN )
	{
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,142), aIndex, 1);
		return FALSE;
	}

	return TRUE;
}




void MakeRandomSetItem(int aIndex)
{
	int itemnum = gSetItemOption.GenRandomItemNum();

	int SetOption = gSetItemOption.GenSetOption(itemnum);

	int option1rand;
	int option2rand;
	int option3rand;
	int optionc;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;

	option1rand = 6;
	option2rand = 4;

	option3rand = rand()%100;
	optionc = rand()%3;

	if(rand()%100 < option2rand)
	{
		Option2 = 1;
	}

	switch(optionc)
	{
	case 0: 
		if(option3rand < 4)
		{
			Option3 = 3;
		}
		break;
	case 1:
		if(option3rand < 8)
		{
			Option3 = 2;
		}
		break;
	case 2: 
		if(option3rand < 12)
		{
			Option3 = 1;
		}
		break;
	default: break;
	}

	Option1 = 1;

	ItemSerialCreateSend(aIndex,gObj[aIndex].MapNumber,gObj[aIndex].X,gObj[aIndex].Y,itemnum,0,0,Option1,Option2,Option3,aIndex,0,SetOption,0,0,0);
}

void MakeRandomSetItem(int aIndex, _stGremoryCaseItem & stItem)
{
	int itemnum = gSetItemOption.GenRandomItemNum();

	int SetOption = gSetItemOption.GenSetOption(itemnum);

	int option1rand;
	int option2rand;
	int option3rand;
	int optionc;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;

	option1rand = 6;
	option2rand = 4;

	option3rand = rand()%100;
	optionc = rand()%3;

	if(rand()%100 < option2rand)
	{
		Option2 = 1;
	}

	switch(optionc)
	{
	case 0: 
		if(option3rand < 4)
		{
			Option3 = 3;
		}
		break;
	case 1:
		if(option3rand < 8)
		{
			Option3 = 2;
		}
		break;
	case 2: 
		if(option3rand < 12)
		{
			Option3 = 1;
		}
		break;
	default: break;
	}

	Option1 = 1;

	stItem.wItemID = itemnum;
	stItem.btItemLevel = 0;
	stItem.btItemDurability = 0;
	stItem.btItemSkill = Option1;
	stItem.btItemLuck = Option2;
	stItem.btItemOption = Option3;
	stItem.btItemExcOption = 0;
	stItem.btItemSetOption = SetOption;
}



void MakeRewardSetItem(int aIndex, BYTE cDropX, BYTE cDropY, int iRewardType, int iMapnumber)
{
	int itemnum = gSetItemOption.GenRandomItemNum();

	int SetOption = gSetItemOption.GenSetOption(itemnum);

	int option1rand;
	int option2rand;
	int option3rand;
	int optionc;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;

	option1rand = 6;
	option2rand = 4;

	option3rand = rand()%100;
	optionc = rand()%3;

	if(rand()%100 < option2rand)
	{
		Option2 = 1;
	}

	switch(optionc)
	{
	case 0: 
		if(option3rand < 4)
		{
			Option3 = 3;
		}
		break;
	case 1:
		if(option3rand < 8)
		{
			Option3 = 2;
		}
		break;
	case 2: 
		if(option3rand < 12)
		{
			Option3 = 1;
		}
		break;
	default: break;
	}

	Option1 = 1;

	if(cDropX == 0 && cDropY == 0)
	{
		cDropX = gObj[aIndex].X;
		cDropY = gObj[aIndex].Y;
	}

	ItemSerialCreateSend(aIndex,iMapnumber,cDropX,cDropY,itemnum,0,0,Option1,Option2,Option3,aIndex,0,SetOption,0,0,0);

	if(iRewardType == 1)
	{
		g_Log.Add("[Reward][KUNDUN] [%s][%s] Set Item itemnum:[%d] skill:[%d] luck:[%d] option:[%d] SetOption:[%d]",
			gObj[aIndex].AccountID,gObj[aIndex].Name,itemnum,Option1,Option2,Option3,SetOption);
	}
	else
	{
		g_Log.Add("[Reward][Etc] [%s][%s] Set Item itemnum:[%d] skill:[%d] luck:[%d] option:[%d] SetOption:[%d]",
			gObj[aIndex].AccountID,gObj[aIndex].Name,itemnum,Option1,Option2,Option3,SetOption);
	}		
}

void MakeRewardSetItem(int aIndex, BYTE cDropX, BYTE cDropY, int iRewardType, int iMapnumber, _stGremoryCaseItem & stItem)
{
	int itemnum = gSetItemOption.GenRandomItemNum();

	int SetOption = gSetItemOption.GenSetOption(itemnum);

	int option1rand;
	int option2rand;
	int option3rand;
	int optionc;
	int Option1 = 0;
	int Option2 = 0;
	int Option3 = 0;

	option1rand = 6;
	option2rand = 4;

	option3rand = rand()%100;
	optionc = rand()%3;

	if(rand()%100 < option2rand)
	{
		Option2 = 1;
	}

	switch(optionc)
	{
	case 0: 
		if(option3rand < 4)
		{
			Option3 = 3;
		}
		break;
	case 1:
		if(option3rand < 8)
		{
			Option3 = 2;
		}
		break;
	case 2: 
		if(option3rand < 12)
		{
			Option3 = 1;
		}
		break;
	default: break;
	}

	Option1 = 1;

	if(cDropX == 0 && cDropY == 0)
	{
		cDropX = gObj[aIndex].X;
		cDropY = gObj[aIndex].Y;
	}

	stItem.wItemID = itemnum;
	stItem.btItemLevel = 0;
	stItem.btItemDurability = 0;
	stItem.btItemSkill = Option1;
	stItem.btItemLuck = Option2;
	stItem.btItemOption = Option3;
	stItem.btItemExcOption = 0;
	stItem.btItemSetOption = SetOption;

	if(iRewardType == 1)
	{
		g_Log.Add("[Reward][KUNDUN] [%s][%s] Set Item itemnum:[%d] skill:[%d] luck:[%d] option:[%d] SetOption:[%d]",
			gObj[aIndex].AccountID,gObj[aIndex].Name,itemnum,Option1,Option2,Option3,SetOption);
	}
	else
	{
		g_Log.Add("[Reward][Etc] [%s][%s] Set Item itemnum:[%d] skill:[%d] luck:[%d] option:[%d] SetOption:[%d]",
			gObj[aIndex].AccountID,gObj[aIndex].Name,itemnum,Option1,Option2,Option3,SetOption);
	}		
}



void gObjRecall(int aIndex, int mapnumber, int x, int y)
{
	if(gObj[aIndex].MapNumber != mapnumber)
	{
		gObj[aIndex].X = x;
		gObj[aIndex].Y = y;

		gObj[aIndex].TX = x;
		gObj[aIndex].TY = y;

		gObj[aIndex].MapNumber = mapnumber;

		gObj[aIndex].PathCount = 0;
		gObj[aIndex].Teleport = 0;

		gObjClearViewport(&gObj[aIndex]);
		GSProtocol.GCTeleportSend(&gObj[aIndex],-1,mapnumber,gObj[aIndex].X,gObj[aIndex].Y,gObj[aIndex].Dir);

		if(gObj[aIndex].m_Change >= 0)
		{
			gObjViewportListProtocolCreate(&gObj[aIndex]);
		}

		gObj[aIndex].RegenMapNumber = mapnumber;
		gObj[aIndex].RegenMapX = x;
		gObj[aIndex].RegenMapY = y;
		gObj[aIndex].RegenOk = 1;
	}
	else
	{
		PMSG_MAGICATTACK_RESULT pAttack;

		PHeadSetBE((unsigned char *)&pAttack,0x19,sizeof(pAttack));
		WORD MagicNumber = 6;
		
		pAttack.MagicNumberH = SET_NUMBERH(MagicNumber);
		pAttack.MagicNumberL = SET_NUMBERL(MagicNumber);
		pAttack.SourceNumberH = SET_NUMBERH(aIndex);
		pAttack.SourceNumberL = SET_NUMBERL(aIndex);
		pAttack.TargetNumberH = SET_NUMBERH(aIndex);
		pAttack.TargetNumberL = SET_NUMBERL(aIndex);

		IOCP.DataSend(aIndex,(unsigned char *)&pAttack,pAttack.h.size);
		GSProtocol.MsgSendV2(&gObj[aIndex],(unsigned char*)&pAttack,pAttack.h.size);

		gObjTeleportMagicUse(aIndex,x,y);
	}
}




void gObjSetExpPetItem(int aIndex, UINT64 exp)
{
	LPOBJ lpObj = &gObj[aIndex];

	if(lpObj->Class != 4)	
	{
		return;
	}

	exp *= gDarkSpiritAddExperience;

	if(lpObj->pInventory[lpObj->m_btInvenPetPos].m_Type == ITEMGET(13,4) && lpObj->pInventory[1].m_Type == ITEMGET(13,5))
	{
		UINT64 addexp = exp * 10 / 100;

		if(lpObj->pInventory[1].AddPetItemExp(addexp))
		{
			if(gObjIsItemPut(lpObj,&lpObj->pInventory[1],1) == 0)
			{
				lpObj->pInventory[1].PetItemLevelDown(addexp);
				MsgOutput(aIndex,Lang.GetText(0,137));
			}
			else
			{
				g_Log.Add("[%s][%s][PetItemLevelUp] [%s] Level:[%d]Exp:[%I64d]AddExp:[%I64d]",
					lpObj->AccountID,lpObj->Name,lpObj->pInventory[1].GetName(),lpObj->pInventory[1].m_PetItem_Level,lpObj->pInventory[1].m_PetItem_Exp,addexp);
				CDarkSpirit::SendLevelmsg(lpObj->m_Index,1,0,0xFE);
				gObjCalCharacter.CalcCharacter(lpObj->m_Index);
			}
		}

		if(lpObj->pInventory[lpObj->m_btInvenPetPos].AddPetItemExp(addexp))
		{
			if(gObjUseInvenPetCheck(lpObj, &lpObj->pInventory[lpObj->m_btInvenPetPos],1) == 0)
			{
				lpObj->pInventory[lpObj->m_btInvenPetPos].PetItemLevelDown(addexp);
				MsgOutput(aIndex,Lang.GetText(0,138));
			}
			else
			{
				g_Log.Add("[%s][%s][PetItemLevelUp] [%s] Level:[%d]Exp:[%I64d]AddExp:[%I64d]",
					lpObj->AccountID,lpObj->Name,lpObj->pInventory[lpObj->m_btInvenPetPos].GetName(),lpObj->pInventory[lpObj->m_btInvenPetPos].m_PetItem_Level,lpObj->pInventory[lpObj->m_btInvenPetPos].m_PetItem_Exp,addexp);
				CDarkSpirit::SendLevelmsg(lpObj->m_Index,lpObj->m_btInvenPetPos,1,0xFE);
				gObjCalCharacter.CalcCharacter(lpObj->m_Index);
			}
		}
	}
	else if(lpObj->pInventory[lpObj->m_btInvenPetPos].m_Type == ITEMGET(13,4))
	{
		UINT64 addexp = exp * 20 / 100;

		if(lpObj->pInventory[lpObj->m_btInvenPetPos].AddPetItemExp(addexp))
		{
			if(gObjUseInvenPetCheck(lpObj,&lpObj->pInventory[lpObj->m_btInvenPetPos], 1) == 0)
			{
				lpObj->pInventory[lpObj->m_btInvenPetPos].PetItemLevelDown(addexp);
				MsgOutput(aIndex,Lang.GetText(0,138));
			}
			else
			{
				g_Log.Add("[%s][%s][PetItemLevelUp] [%s] Level:[%d]Exp:[%I64d]AddExp:[%I64d]",
					lpObj->AccountID,lpObj->Name,lpObj->pInventory[lpObj->m_btInvenPetPos].GetName(),lpObj->pInventory[lpObj->m_btInvenPetPos].m_PetItem_Level,lpObj->pInventory[lpObj->m_btInvenPetPos].m_PetItem_Exp,addexp);
				CDarkSpirit::SendLevelmsg(lpObj->m_Index,lpObj->m_btInvenPetPos,1,0xFE);
				gObjCalCharacter.CalcCharacter(lpObj->m_Index);
			}
		}
	}
	else if(lpObj->pInventory[1].m_Type == ITEMGET(13,5))
	{
		UINT64 addexp = exp * 20 / 100;

		if(lpObj->pInventory[1].AddPetItemExp(addexp))
		{
			if(gObjIsItemPut(lpObj,&lpObj->pInventory[1],1) == 0)
			{
				lpObj->pInventory[1].PetItemLevelDown(addexp);
				MsgOutput(aIndex,Lang.GetText(0,137));
			}
			else
			{
				g_Log.Add("[%s][%s][PetItemLevelUp] [%s] Level:[%d]Exp:[%I64d]AddExp:[%I64d]",
					lpObj->AccountID,lpObj->Name,lpObj->pInventory[1].GetName(),lpObj->pInventory[1].m_PetItem_Level,lpObj->pInventory[1].m_PetItem_Exp,addexp);
				CDarkSpirit::SendLevelmsg(lpObj->m_Index,1,0,0xFE);
				gObjCalCharacter.CalcCharacter(lpObj->m_Index);
			}
		}
	}
}




BOOL gObjGetRandomItemDropLocation(int iMapNumber, BYTE& cX, BYTE& cY, int iRangeX, int iRangeY, int iLoopCount)
{
	int iUX = cX;
	int iUY = cY;

	if ( iRangeX <= 0 )
	{
		iRangeX = 1;
	}

	if ( iRangeY <= 0 )
	{
		iRangeY = 1;
	}

	if ( iLoopCount <= 0 )
	{
		iLoopCount = 1;
	}

	while ( iLoopCount-- > 0 )
	{
		cX = ( rand() % (iRangeX+1) ) * (((rand()%2==0)?-1:1)) + iUX;
		cY = ( rand() % (iRangeY+1) ) * (((rand()%2==0)?-1:1)) + iUY;

		BYTE attr = MapC[iMapNumber].GetAttr(cX, cY);

		if ( (attr&4) != 4 && (attr&8) != 8)
		{
			return TRUE;
		}
	}

	return FALSE;
}





BOOL gObjGetRandomFreeLocation(int iMapNumber, BYTE &cX, BYTE & cY, int iRangeX, int iRangeY, int iLoopCount)
{
	int iUX = cX;
	int iUY = cY;

	if ( iRangeX <= 0 )
	{
		iRangeX = 1;
	}

	if ( iRangeY <= 0 )
	{
		iRangeY = 1;
	}

	if ( iLoopCount <= 0 )
	{
		iLoopCount = 1;
	}

	while ( iLoopCount-- > 0 )
	{
		cX = ( rand() % (iRangeX+1) ) * (((rand()%2==0)?-1:1)) + iUX;
		cY = ( rand() % (iRangeY+1) ) * (((rand()%2==0)?-1:1)) + iUY;

		BYTE attr = MapC[iMapNumber].GetAttr(cX, cY);

		if ( attr == 0 )
		{
			return TRUE;
		}
	}

	return FALSE;
}





int gObjCheckAttackTypeMagic(int iClass, int iSkill)
{
	if ( iSkill == 0 )
	{
		return 0;
	}

	return MagicDamageC.GetSkillType(iSkill);
}





int gObjGetGuildUnionNumber(LPOBJ lpObj)
{
	int iUnion=0;

	if ( lpObj->m_PlayerData->lpGuild != NULL )
	{
		iUnion = (lpObj->m_PlayerData->lpGuild->iGuildUnion == 0)? lpObj->m_PlayerData->lpGuild->Number : lpObj->m_PlayerData->lpGuild->iGuildUnion;
	}

	return iUnion;

}




void gObjGetGuildUnionName(LPOBJ lpObj, char* szUnionName, int iUnionNameLen)
{
	szUnionName[0] = 0;
	
	if ( lpObj->m_PlayerData->lpGuild == NULL )
	{
		return;
	}

	if ( lpObj->m_PlayerData->lpGuild->iGuildUnion == 0 )
	{
		return;
	}

	TUnionInfo * pUnionInfo = UnionManager.SearchUnion(lpObj->m_PlayerData->lpGuild->iGuildUnion);

	if ( pUnionInfo != NULL )
	{
		memcpy(szUnionName, pUnionInfo->m_szMasterGuild, iUnionNameLen);
	}
}




BOOL gObjCheckRival(LPOBJ lpObj, LPOBJ lpTargetObj)
{
	if(lpTargetObj->Type != OBJ_USER)
	{
		return false;
	}

	if(lpObj->Type != OBJ_USER)
	{
		return false;
	}

	if(lpTargetObj->m_PlayerData->lpGuild == 0 || lpObj->m_PlayerData->lpGuild == 0)
	{
		return false;
	}

	if(lpTargetObj->m_PlayerData->lpGuild->iGuildRival == 0 || lpObj->m_PlayerData->lpGuild->iGuildRival == 0)
	{
		return false;
	}

	int iUnion = !lpObj->m_PlayerData->lpGuild->iGuildUnion ? lpObj->m_PlayerData->lpGuild->Number : lpObj->m_PlayerData->lpGuild->iGuildUnion;

	if(UnionManager.GetGuildRelationShip(iUnion,lpTargetObj->m_PlayerData->lpGuild->Number) == 2)
	{
		return true;
	}
	return false;
}




int gObjGetRelationShip(LPOBJ lpObj, LPOBJ lpTargetObj)
{
	if ( lpObj == NULL || lpTargetObj == NULL )
	{
		return false;
	}

	if ( lpObj->m_PlayerData->lpGuild == NULL || lpTargetObj->m_PlayerData->lpGuild == NULL )
	{
		return 0;
	}

	int iUnion = (!lpObj->m_PlayerData->lpGuild->iGuildUnion) ? lpObj->m_PlayerData->lpGuild->Number : lpObj->m_PlayerData->lpGuild->iGuildUnion;

	return UnionManager.GetGuildRelationShip(iUnion, lpTargetObj->m_PlayerData->lpGuild->Number);
}

struct PMSG_UNION_VIEWPORT_NOTIFY_COUNT {

  // static data ------------------------------------

  // non-static data --------------------------------
  /*<thisrel this+0x0>*/ /*|0x4|*/ struct PWMSG_HEAD h;
  /*<thisrel this+0x4>*/ /*|0x1|*/ unsigned char btCount;

  // base classes -----------------------------------

  // friends ----------------------------------------

  // static functions -------------------------------

  // non-virtual functions --------------------------

  // virtual functions ------------------------------
};
// <size 0x5>

struct PMSG_UNION_VIEWPORT_NOTIFY {

  // static data ------------------------------------

  // non-static data --------------------------------
  /*<thisrel this+0x0>*/ /*|0x1|*/ unsigned char btNumberH;
  /*<thisrel this+0x1>*/ /*|0x1|*/ unsigned char btNumberL;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iGuildNumber;
  /*<thisrel this+0x8>*/ /*|0x1|*/ unsigned char btGuildRelationShip;
  /*<thisrel this+0x9>*/ /*|0x8|*/ char szUnionName[8];

  // base classes -----------------------------------

  // friends ----------------------------------------

  // static functions -------------------------------

  // non-virtual functions --------------------------

  // virtual functions ------------------------------
};
// <size 0x14>

void gObjNotifyUpdateUnionV1(LPOBJ lpObj)
{
	if(lpObj == 0)
	{
		return;
	}

	char cBUFFER_V1[6000];
	int iVp1Count = 0;

	memset(cBUFFER_V1,0x00,sizeof(cBUFFER_V1));

	PMSG_UNION_VIEWPORT_NOTIFY_COUNT * lpMsg = (PMSG_UNION_VIEWPORT_NOTIFY_COUNT *)&cBUFFER_V1;
	PMSG_UNION_VIEWPORT_NOTIFY * lpMsgBody = (PMSG_UNION_VIEWPORT_NOTIFY * )&cBUFFER_V1[sizeof(PMSG_UNION_VIEWPORT_NOTIFY_COUNT)];

	for(int n = 0; n < MAX_VIEWPORT; n++)
	{
		if(!ObjectMaxRange(lpObj->VpPlayer[n].number))
		{
			continue;
		}

		if(lpObj->VpPlayer[n].state == 2 && lpObj->VpPlayer[n].type == OBJ_USER)
		{
			LPOBJ lpTargetObj = &gObj[lpObj->VpPlayer[n].number];

			if(lpTargetObj == 0)
			{
				continue;
			}

			if(lpTargetObj->m_PlayerData->lpGuild == 0)
			{
				continue;
			}

			gObjGetGuildUnionName(lpTargetObj,lpMsgBody[iVp1Count].szUnionName,sizeof(lpMsgBody[iVp1Count].szUnionName));

			lpMsgBody[iVp1Count].btGuildRelationShip = gObjGetRelationShip(lpObj,lpTargetObj);
			lpMsgBody[iVp1Count].btNumberL = SET_NUMBERL(WORD(lpTargetObj->m_Index));
			lpMsgBody[iVp1Count].btNumberH = SET_NUMBERH(WORD(lpTargetObj->m_Index));
			lpMsgBody[iVp1Count].iGuildNumber = lpTargetObj->m_PlayerData->lpGuild->Number;
			iVp1Count++;
		}
	}

	if(iVp1Count > 0 && iVp1Count <= MAX_VIEWPORT)
	{
		lpMsg->btCount = iVp1Count;

		PHeadSetW((unsigned char *)lpMsg,0x67,iVp1Count * sizeof(PMSG_UNION_VIEWPORT_NOTIFY) + sizeof(PMSG_UNION_VIEWPORT_NOTIFY_COUNT));

		IOCP.DataSend(lpObj->m_Index,(unsigned char *)lpMsg,((lpMsg->h.sizeL & 0xFF) & 0xFF | ((lpMsg->h.sizeH & 0xFF) & 0xFF) << 8) & 0xFFFF);
	}
	else
	{
		if(iVp1Count != 0)
		{
			g_Log.Add("[Union ViewPort] ERROR : iVp1Count is OUT of BOUND: %d",iVp1Count);
		}
	}
}





void gObjNotifyUpdateUnionV2(LPOBJ lpObj)
{
	if(lpObj == 0)
	{
		return;
	}

	if(lpObj->Type != OBJ_USER)
	{
		return;
	}

	if(lpObj->m_PlayerData->lpGuild == 0)
	{
		return;
	}

	char cBUFFER_V2[100] = {0};

	PMSG_UNION_VIEWPORT_NOTIFY_COUNT * lpMsg2 = (PMSG_UNION_VIEWPORT_NOTIFY_COUNT *)&cBUFFER_V2;
	PMSG_UNION_VIEWPORT_NOTIFY * lpMsgBody2 = (PMSG_UNION_VIEWPORT_NOTIFY *)&cBUFFER_V2[sizeof(PMSG_UNION_VIEWPORT_NOTIFY_COUNT)];

	gObjGetGuildUnionName(lpObj,lpMsgBody2->szUnionName,sizeof(lpMsgBody2->szUnionName));
	lpMsgBody2->btNumberL = SET_NUMBERL(WORD(lpObj->m_Index));
	lpMsgBody2->btNumberH = SET_NUMBERH(WORD(lpObj->m_Index));

	lpMsgBody2->iGuildNumber = lpObj->m_PlayerData->lpGuild->Number;
	lpMsgBody2->btGuildRelationShip = 0;
	lpMsg2->btCount = 1;

	PHeadSetW((unsigned char *)lpMsg2,0x67,sizeof(lpMsg2) + sizeof(lpMsgBody2[0])+1);

	IOCP.DataSend(lpObj->m_Index,(unsigned char *)lpMsg2,((lpMsg2->h.sizeL & 0xFF) & 0xFF | ((lpMsg2->h.sizeH & 0xFF) & 0xFF) << 8) & 0xFFFF);

	for(int n = 0; n < MAX_VIEWPORT; n++)
	{
		if(lpObj->VpPlayer2[n].type == OBJ_USER && lpObj->VpPlayer2[n].state != 0)
		{
			LPOBJ lpTargetObj = &gObj[lpObj->VpPlayer2[n].number];

			if(lpTargetObj->m_PlayerData->lpGuild != 0)
			{
				lpMsgBody2->btGuildRelationShip = gObjGetRelationShip(lpTargetObj,lpObj);
			}

			if(lpMsgBody2->btGuildRelationShip != 1)
			{
				IOCP.DataSend(lpObj->VpPlayer2[n].number,(unsigned char *)lpMsg2,((lpMsg2->h.sizeL & 0xFF) & 0xFF | ((lpMsg2->h.sizeH & 0xFF) & 0xFF) << 8) & 0xFFFF);
			}
		}
	}
}





void gObjUnionUpdateProc(int iIndex)
{

	if ( gObjIsConnected(iIndex) == FALSE )
	{
		return;
	}

	LPOBJ lpObj = &gObj[iIndex];

	if ( lpObj->Type != OBJ_USER )
	{
		return;
	}

	if ( lpObj->RegenOk > 0 )
	{
		return;
	}

	if ( lpObj->CloseCount > -1 )
		return;

	if ( lpObj->m_PlayerData->lpGuild == NULL )
	{
		return;
	}

	if ( lpObj->m_PlayerData->lpGuild->CheckTimeStamp(lpObj->m_PlayerData->iGuildUnionTimeStamp) != FALSE )
	{
		return;
	}

	lpObj->m_PlayerData->iGuildUnionTimeStamp = lpObj->m_PlayerData->lpGuild->GetTimeStamp();
	gObjNotifyUpdateUnionV1(lpObj);
	gObjNotifyUpdateUnionV2(lpObj);

}

// _GUILD_INFO_STRUCT 0x4e0a10





void gObjSetKillCount(int aIndex, int iOption)	// Option : [0 : SetToZero] [1 : Increase] [2 : Decrease]
{
	if ( gObjIsConnected(aIndex) == FALSE )
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];

	if ( iOption== 1 )
	{
		if ( lpObj->m_btKillCount <= 254 )
		{
			lpObj->m_btKillCount++;
		}
	}
	else if ( iOption == 2 )
	{
		if ( lpObj->m_btKillCount > 0 )
		{
			lpObj->m_btKillCount--;
		}
	}
	else if ( iOption == 0 )
	{
		lpObj->m_btKillCount = 0;
	}

	PMSG_KILLCOUNT pMsg = {0};
	PHeadSubSetB((LPBYTE)&pMsg, 0xB8,0x01, sizeof(pMsg));
	pMsg.btKillCount = lpObj->m_btKillCount;
	
	IOCP.DataSend( aIndex, (UCHAR*)&pMsg, sizeof(pMsg));
}

struct PMSG_NOTIFY_REGION_OF_WEAPON
{
	struct PBMSG_HEAD2 h;
	BYTE btWeaponType;
	BYTE btPointX;
	BYTE btPointY;
};

struct PMSG_NOTIFY_TARGET_OF_WEAPON
{
	struct PBMSG_HEAD2 h;
	BYTE NumberH;
	BYTE NumberL;
	BYTE btWeaponType;
};

struct PMSG_WEAPON_OWNER_VIEWPORT_NOTIFY_COUNT
{
	struct PWMSG_HEAD h;
	BYTE btCount;
};

struct PMSG_WEAPON_OWNER_VIEWPORT_NOTIFY
{
	BYTE btObjType;
	BYTE btObjClassH;
	BYTE btObjClassL;
	BYTE btObjIndexH;
	BYTE btObjIndexL;
	BYTE btX;
	BYTE btY;
	BYTE CharSet[17]; //lol
	BYTE btViewSkillStateCount;
	BYTE btViewSkillState[32];
};

void gObjNotifyUseWeaponV1(LPOBJ lpOwnerObj, LPOBJ lpWeaponObj, int iTargetX, int iTargetY)
{
	if(lpOwnerObj == 0)
	{
		return;
	}

	if(lpWeaponObj == 0)
	{
		return;
	}

	PMSG_NOTIFY_REGION_OF_WEAPON pNotifyRegionMsg = {0};

	PHeadSubSetB((LPBYTE)&pNotifyRegionMsg,0xB7,2,sizeof(pNotifyRegionMsg));

	pNotifyRegionMsg.btPointX = iTargetX;
	pNotifyRegionMsg.btPointY = iTargetY;

	PMSG_NOTIFY_TARGET_OF_WEAPON pNotifyTargetMsg = {0};

	PHeadSubSetB((LPBYTE)&pNotifyTargetMsg,0xB7,3,sizeof(pNotifyTargetMsg));

	if(lpWeaponObj->Class == 221)
	{
		pNotifyRegionMsg.btWeaponType = 1;
		pNotifyTargetMsg.btWeaponType = 1;
	}
	else if(lpWeaponObj->Class == 222)
	{
		pNotifyRegionMsg.btWeaponType = 2;
		pNotifyTargetMsg.btWeaponType = 2;
	}

	char cBUFFER_V1[6000];
	int iVp1Count = 0;

	memset(cBUFFER_V1,0x00,sizeof(cBUFFER_V1));

	PMSG_WEAPON_OWNER_VIEWPORT_NOTIFY_COUNT * lpMsg = (PMSG_WEAPON_OWNER_VIEWPORT_NOTIFY_COUNT *)&cBUFFER_V1;
	PMSG_WEAPON_OWNER_VIEWPORT_NOTIFY * lpMsgBody = (PMSG_WEAPON_OWNER_VIEWPORT_NOTIFY *)&cBUFFER_V1[sizeof(PMSG_WEAPON_OWNER_VIEWPORT_NOTIFY_COUNT)];

	for(int n = 0; n < g_ConfigRead.server.GetObjectMax();n++)
	{
		LPOBJ lpTargetObj = &gObj[n];

		if(gObjIsConnected(n) == 0)
		{
			continue;
		}

		if(lpOwnerObj->MapNumber != lpTargetObj->MapNumber)
		{
			continue;
		}

		if(abs(lpTargetObj->X - iTargetX) > 6)
		{
			continue;
		}

		if(abs(lpTargetObj->Y - iTargetY) > 6)
		{
			continue;
		}

		lpMsgBody[iVp1Count].btObjClassH = SET_NUMBERH(lpTargetObj->Class);
		lpMsgBody[iVp1Count].btObjClassL = SET_NUMBERL(lpTargetObj->Class);
		lpMsgBody[iVp1Count].btObjIndexH = SET_NUMBERH(lpTargetObj->m_Index);
		lpMsgBody[iVp1Count].btObjIndexH = SET_NUMBERL(lpTargetObj->m_Index);
		lpMsgBody[iVp1Count].btX = lpTargetObj->X;
		lpMsgBody[iVp1Count].btY = lpTargetObj->Y;
		lpMsgBody[iVp1Count].btViewSkillStateCount = gObjMakeViewportState(lpTargetObj, lpMsgBody[iVp1Count].btViewSkillState); //Season3 changed

		if(lpTargetObj->Type == OBJ_USER)
		{
			lpMsgBody[iVp1Count].btObjType = OBJ_USER;
			lpMsgBody[iVp1Count].btObjClassH = CS_SET_CLASS(lpTargetObj->Class);
			lpMsgBody[iVp1Count].btObjClassL = 0;

			memcpy(lpMsgBody[iVp1Count].CharSet,&lpTargetObj->CharSet[1],sizeof(lpMsgBody[iVp1Count].CharSet));
		}
		else if(lpTargetObj->Type == OBJ_MONSTER)
		{
			lpMsgBody[iVp1Count].btObjType = OBJ_MONSTER;
			memset(lpMsgBody[iVp1Count].CharSet,0x00,sizeof(lpMsgBody[iVp1Count].CharSet));
		}
		else
		{
			lpMsgBody[iVp1Count].btObjType = OBJ_NPC;
			memset(lpMsgBody[iVp1Count].CharSet,0x00,sizeof(lpMsgBody[iVp1Count].CharSet));
		}

		iVp1Count++;

		if(lpTargetObj->Type == OBJ_USER)
		{
			IOCP.DataSend(lpTargetObj->m_Index,(LPBYTE)&pNotifyRegionMsg,sizeof(pNotifyRegionMsg));

			if(abs(lpTargetObj->X - iTargetX) > 3)
			{
				continue;
			}

			if(abs(lpTargetObj->Y - iTargetY) > 3)
			{
				continue;
			}

			pNotifyTargetMsg.NumberH = SET_NUMBERH(lpTargetObj->m_Index);
			pNotifyTargetMsg.NumberL = SET_NUMBERL(lpTargetObj->m_Index);
		}
	}

	if(iVp1Count > 0 && iVp1Count <= 100)
	{
		lpMsg->btCount = iVp1Count;
		PHeadSetW((LPBYTE)lpMsg,0x68,iVp1Count * sizeof(PMSG_WEAPON_OWNER_VIEWPORT_NOTIFY) + sizeof(PMSG_WEAPON_OWNER_VIEWPORT_NOTIFY_COUNT));

		IOCP.DataSend(lpOwnerObj->m_Index,(LPBYTE)lpMsg,((lpMsg->h.sizeL & 0xFF) & 0xFF | ((lpMsg->h.sizeH & 0xFF) & 0xFF) << 8) & 0xFFFF);
	}
}





void gObjNotifyUseWeaponDamage(LPOBJ lpWeaponObj, int iTargetX, int iTargetY)
{
	if ( lpWeaponObj->Class == 221 || lpWeaponObj->Class == 222)
	{
		for ( int i=0;i<g_ConfigRead.server.GetObjectMax();i++)
		{
			LPOBJ lpObj = &gObj[i];

			if ( lpWeaponObj->MapNumber != lpObj->MapNumber )
			{
				continue;
			}

			if ( abs(lpObj->X - iTargetX) > 3 )
			{
				continue;
			}

			if ( abs(lpObj->Y - iTargetY) > 3 )
			{
				continue;
			}

			g_CsNPC_Weapon.AddWeaponDamagedTargetInfo(lpWeaponObj->m_Index, i, 100);
		}
	}

}





void gObjUseBlessAndSoulPotion(int aIndex, int iItemLevel)
{
	LPOBJ lpObj = &gObj[aIndex];

	if ( iItemLevel == 0 )	// Bless Potion
	{
		gObjAddBuffEffect(lpObj, BUFFTYPE_BLESS_POTION, 0, 0, 0, 0, 60);
	}
	else if ( iItemLevel == 1 ) // Soul Potion
	{
		gObjAddBuffEffect(lpObj, BUFFTYPE_SOUL_POTION, 0, 0, 0, 0, 60);
		gObjUseDrink(lpObj, 0x4D);
	}
}





void gObjWeaponDurDownInCastle(LPOBJ lpObj, LPOBJ lpTargetObj, int iDecValue)
{
	if(lpObj->Type == OBJ_MONSTER || lpObj->Type == OBJ_NPC)
	{
		return;
	}

	if (g_MineSystem.IsEquipPickax(lpObj->m_Index))
	{
		return;
	}

	int itargetdefence = 0;

	CItem * Right = &lpObj->pInventory[0];
	CItem * Left = &lpObj->pInventory[1];

	int bIsRightDurDown = 0;
	int bIsLeftDurDown = 0;

	if(lpObj->Class == 1 || lpObj->Class == 3 || lpObj->Class == 4 || lpObj->Class == 6)
	{
		if(Right->m_Type >= ITEMGET(0,0) && Right->m_Type < ITEMGET(4,0)
			&& Left->m_Type >= ITEMGET(0,0) && Left->m_Type < ITEMGET(4,0))
		{
			bIsRightDurDown = 1;
			bIsLeftDurDown = 1;
		}
	}

	if(Left->m_Type >= ITEMGET(4,0) && Left->m_Type < ITEMGET(4,7)
		|| Left->m_Type == ITEMGET(4,20)
		|| Left->m_Type == ITEMGET(4,21))
	{
		bIsLeftDurDown = 1;
	}
	else if(Right->m_Type >= ITEMGET(4,8) && Right->m_Type < ITEMGET(4,15) || Right->m_Type >= ITEMGET(4,16) && Right->m_Type < ITEMGET(5,0))
	{
		bIsRightDurDown = 1;
	}
	else if(Right->m_Type >= 0 && Right->m_Type < ITEMGET(4,0))
	{
		bIsRightDurDown = 1;
	}
	else if(Right->m_Type >= ITEMGET(5,0) && Right->m_Type < ITEMGET(6,0))
	{
		bIsRightDurDown = 1;
	}

	if(bIsRightDurDown != 0)
	{
		int iRet = Right->SimpleDurabilityDown(iDecValue);

		if(iRet != 0)
		{
			GSProtocol.GCItemDurSend(lpObj->m_Index,0,Right->m_Durability,0);

			if(iRet == 2)
			{
				gObjCalCharacter.CalcCharacter(lpObj->m_Index);
				gObjCalCharacter.CalcMLSkillItemOption(lpObj);
			}
		}
	}

	if(bIsLeftDurDown != 0)
	{
		int iRet = Left->SimpleDurabilityDown(iDecValue);

		if(iRet != 0)
		{
			GSProtocol.GCItemDurSend(lpObj->m_Index,1,Left->m_Durability,0);

			if(iRet == 2)
			{
				gObjCalCharacter.CalcCharacter(lpObj->m_Index);
				gObjCalCharacter.CalcMLSkillItemOption(lpObj);
			}
		}
	}
}

void gObjCheckTimeOutValue(LPOBJ lpObj, DWORD& rNowTick)
{
	long lTick;
	long lSecond;

	lTick = rNowTick - lpObj->m_dwLastCheckTick;
	lSecond = lTick / 1000;

	if(lSecond == 0)
	{
		return;
	}

	lpObj->m_dwLastCheckTick = rNowTick;
}



void MsgOutput(int aIndex, char* msg, ...) 
{
	char szBuffer[512]="";
	va_list pArguments;

	va_start(pArguments, msg );
	vsprintf(&szBuffer[0], msg, pArguments);
	va_end(pArguments);
	
	GSProtocol.GCServerMsgStringSend(&szBuffer[0], aIndex, 1);
}





void gProcessAutoRecuperation(LPOBJ lpObj)
{
	if(lpObj->Life == (lpObj->MaxLife + lpObj->AddLife)
		&& lpObj->Mana == (lpObj->MaxMana + lpObj->AddMana)
		&& lpObj->BP == (lpObj->MaxBP + lpObj->AddBP) )
	{
		lpObj->m_iAutoRecuperationTime = GetTickCount();
		return;
	}

	if(( GetTickCount() - lpObj->m_iAutoRecuperationTime ) >= 10000 && ( GetTickCount() - lpObj->m_iAutoRecuperationTime ) < 15000)
	{
		if(lpObj->Life < ( lpObj->MaxLife + lpObj->AddLife ) )
		{
			lpObj->Life += 1.0f;

			if(lpObj->Life > (lpObj->MaxLife + lpObj->AddLife))
			{
				lpObj->Life = lpObj->MaxLife + lpObj->AddLife;
			}

			GSProtocol.GCReFillSend(lpObj->m_Index, lpObj->Life, -1, 0, lpObj->iShield);
		}

		if(lpObj->Mana < ( lpObj->MaxMana + lpObj->AddMana ) || lpObj->BP < ( lpObj->MaxBP + lpObj->AddBP) )
		{
			lpObj->Mana += 1.0f;

			if(lpObj->Mana > (lpObj->MaxMana + lpObj->AddMana))
			{
				lpObj->Mana = lpObj->MaxMana + lpObj->AddMana;
			}

			lpObj->BP += 1;

			if(lpObj->BP > (lpObj->MaxBP + lpObj->AddBP))
			{
				lpObj->BP = lpObj->MaxBP + lpObj->AddBP;
			}

			GSProtocol.GCManaSend(lpObj->m_Index,lpObj->Mana,0xFF,0,lpObj->BP);
		}
	}
	else if(( GetTickCount() - lpObj->m_iAutoRecuperationTime ) >= 15000 && ( GetTickCount() - lpObj->m_iAutoRecuperationTime ) < 25000)
	{
		if(lpObj->Life < ( lpObj->MaxLife + lpObj->AddLife ) )
		{
			lpObj->Life += 5.0f;

			if(lpObj->Life > (lpObj->MaxLife + lpObj->AddLife))
			{
				lpObj->Life = lpObj->MaxLife + lpObj->AddLife;
			}

			GSProtocol.GCReFillSend(lpObj->m_Index, lpObj->Life, -1, 0, lpObj->iShield);
		}

		if(lpObj->Mana < ( lpObj->MaxMana + lpObj->AddMana ) || lpObj->BP < ( lpObj->MaxBP + lpObj->AddBP) )
		{
			lpObj->Mana += 5.0f;

			if(lpObj->Mana > (lpObj->MaxMana + lpObj->AddMana))
			{
				lpObj->Mana = lpObj->MaxMana + lpObj->AddMana;
			}

			lpObj->BP += 5;

			if(lpObj->BP > (lpObj->MaxBP + lpObj->AddBP))
			{
				lpObj->BP = lpObj->MaxBP + lpObj->AddBP;
			}

			GSProtocol.GCManaSend(lpObj->m_Index,lpObj->Mana,0xFF,0,lpObj->BP);
		}
	}
	else if(( GetTickCount() - lpObj->m_iAutoRecuperationTime ) >= 25000)
	{
		if(lpObj->Life < ( lpObj->MaxLife + lpObj->AddLife ) )
		{
			lpObj->Life += 10.0f;

			if(lpObj->Life > (lpObj->MaxLife + lpObj->AddLife))
			{
				lpObj->Life = lpObj->MaxLife + lpObj->AddLife;
			}

			GSProtocol.GCReFillSend(lpObj->m_Index, lpObj->Life, -1, 0, lpObj->iShield);
		}

		if(lpObj->Mana < ( lpObj->MaxMana + lpObj->AddMana ) || lpObj->BP < ( lpObj->MaxBP + lpObj->AddBP) )
		{
			lpObj->Mana += 10.0f;

			if(lpObj->Mana > (lpObj->MaxMana + lpObj->AddMana))
			{
				lpObj->Mana = lpObj->MaxMana + lpObj->AddMana;
			}

			lpObj->BP += 10;

			if(lpObj->BP > (lpObj->MaxBP + lpObj->AddBP))
			{
				lpObj->BP = lpObj->MaxBP + lpObj->AddBP;
			}

			GSProtocol.GCManaSend(lpObj->m_Index,lpObj->Mana,0xFF,0,lpObj->BP);
		}
	}
}

void gObjShieldAutoRefill(LPOBJ lpObj)
{
	int iRefillPoint = 0;
	int iShieldRefillOption = 0;

	if(g_ShieldSystemOn == 0)
	{
		return;
	}

	if(g_ShieldAutoRefillOn == 0)
	{
		return;
	}

	if(lpObj->Type != OBJ_USER)
	{
		return;
	}

	if (MAX_MAP_RANGE(lpObj->MapNumber) == FALSE)
	{
		return;
	}

	if(g_ShieldAutoRefillOnSafeZone == 1)
	{
		unsigned char btMapAttribute = MapC[lpObj->MapNumber].GetAttr(lpObj->X,lpObj->Y);

		if((btMapAttribute & 1) != 1 && lpObj->m_PlayerData->m_ItemOptionExFor380.OpRefillOn == 0)
		{
			lpObj->dwShieldAutoRefillTimer = GetTickCount();
			return;
		}
	}

	if(lpObj->iShield >= (lpObj->iMaxShield + lpObj->iAddShield))
	{
		lpObj->dwShieldAutoRefillTimer = GetTickCount();
		return;
	}

	iShieldRefillOption = lpObj->m_PlayerData->m_ItemOptionExFor380.OpAddRefillSD;

	int iRefillExpression = 0;

	float fRefillExpressionA = (lpObj->iMaxShield + lpObj->iAddShield) / 30;
	float fRefillExpressionB = iShieldRefillOption + 100;

	iRefillExpression = ((fRefillExpressionA * fRefillExpressionB) / 100.0f) / 25.0f;

	unsigned long dwTick = GetTickCount() - lpObj->dwShieldAutoRefillTimer;

	if(dwTick >= 25000)
	{
		iRefillPoint = iRefillExpression * 3; 
	}
	else if(dwTick >= 15000)
	{
		iRefillPoint = (iRefillExpression * 25) / 10;
	}
	else if(dwTick >= 10000)
	{
		iRefillPoint = iRefillExpression * 2;
	}
	else
	{
		return;
	}

	iRefillPoint++; //season4 changed (fix)

	if(iRefillPoint == 0)
	{
		return;
	}

	if (lpObj->Class == CLASS_RAGEFIGHTER)
	{
		iRefillPoint += iRefillPoint * lpObj->m_PlayerData->m_MPSkillOpt.iMpsSDSpeed_Monk / 100.0;
	}

	else
	{
		iRefillPoint += iRefillPoint * lpObj->m_PlayerData->m_MPSkillOpt.iMpsSDSpeed / 100.0;
	}

	lpObj->iShield += iRefillPoint;

	if(lpObj->iShield > (lpObj->iMaxShield + lpObj->iAddShield))
	{
		lpObj->iShield = lpObj->iMaxShield + lpObj->iAddShield;
	} 

	GSProtocol.GCReFillSend(lpObj->m_Index, lpObj->Life, -1, 0, lpObj->iShield);
}

int gObjCheckOverlapItemUsingDur(int iUserIndex, int iMaxOverlapped, int iItemType, int iItemLevel)
{
	for(int x = INVETORY_WEAR_SIZE; x < MAIN_INVENTORY_SIZE; x++)
	{
		if(gObj[iUserIndex].pInventory[x].IsItem() == 1
			&& gObj[iUserIndex].pInventory[x].m_Type == (short)iItemType
			&& gObj[iUserIndex].pInventory[x].m_Level == (short)iItemLevel)
		{
			int iITEM_DUR = gObj[iUserIndex].pInventory[x].m_Durability;

			if((((iITEM_DUR)<0)?FALSE:((iITEM_DUR)>iMaxOverlapped-1)?FALSE:TRUE ))
			{
				return x;
			}
		}
	}
return -1;
}

int gObjOverlapItemUsingDur(class CItem* lpItem, int iMapNumber, int iItemNumber, int iUserIndex, int iMaxOverlapped, int iItemType, int iItemLevel)
{
	for(int iLoop = 0; iLoop < MAIN_INVENTORY_SIZE; iLoop ++)
	{
		int iInventoryIndex = gObjCheckOverlapItemUsingDur(iUserIndex,iMaxOverlapped,iItemType,iItemLevel);
		if(MAIN_INVENTORY_RANGE(iInventoryIndex))
		{
			int iItemDur = gObj[iUserIndex].pInventory[iInventoryIndex].m_Durability + lpItem->m_Durability;

			if(iItemDur <= iMaxOverlapped)
			{
				if(MapC[iMapNumber].ItemGive(iUserIndex,iItemNumber,1) == 1)
				{
					return iInventoryIndex;
				}
			}
			else
			{
				lpItem->m_Durability = iItemDur - iMaxOverlapped;
				gObj[iUserIndex].pInventory[iInventoryIndex].m_Durability = iMaxOverlapped;

				GSProtocol.GCItemDurSend(iUserIndex,iInventoryIndex,gObj[iUserIndex].pInventory[iInventoryIndex].m_Durability,0);
			}
		}
		else
		{
			return -1;
		}
	}
	return -1;
}

int gObjCheckSerial0ItemList(class CItem* lpItem)
{
	if(gSerialZeroCheck == false)
	{
		return false;
	}

	if(lpItem->m_Type == ITEMGET(14,13)
		|| lpItem->m_Type == ITEMGET(14,14)
		|| lpItem->m_Type == ITEMGET(14,16)
		|| lpItem->m_Type == ITEMGET(14,22)
		|| lpItem->m_Type == ITEMGET(12,15)
		|| lpItem->m_Type == ITEMGET(12,30)
		|| lpItem->m_Type == ITEMGET(12,31)
		|| lpItem->m_Type == ITEMGET(14,31))
	{
		if(lpItem->GetNumber() == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

int gObjCheckInventorySerial0Item(LPOBJ lpObj)
{
	UINT64 iItemSerial;
	int iCount;
	int i;

	if(gItemSerialCheck == 0)
	{
		return true;
	}

	iCount = 0;

	for(int i = 0; i < INVENTORY_SIZE; i++)
	{
		if(lpObj->pInventory[i].IsItem() == 0)
		{
			continue;
		}

		if(gObjCheckSerial0ItemList(&lpObj->pInventory[i])==0)
		{
			continue;
		}

		iItemSerial = lpObj->pInventory[i].GetNumber();

		if(iItemSerial == 0)
		{
			g_Log.Add("[ANTI-HACK][Serial 0 Item] (%s)(%s) Item(%s) Pos(%d)",
				lpObj->AccountID,lpObj->Name,lpObj->pInventory[i].GetName(),i);
			iCount++;
		}
	}

	if(iCount > 0)
	{
		return true;
	}

	return false;
}



BOOL gCheckSkillDistance(int aIndex, int aTargetIndex, int iSkillNum)
{
	if ( !g_iSkillDistanceCheck  )
		return TRUE;

	if ( iSkillNum == 40 )
		return TRUE;

	if ( iSkillNum == 392 )
		return TRUE;

	int iSkillDistance = MagicDamageC.GetSkillDistance(iSkillNum);

	if ( iSkillDistance == -1 )
		return FALSE;

	iSkillDistance += g_iSkillDistanceCheckTemp;
	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj = &gObj[aTargetIndex];

	if ( abs(lpObj->X - lpTargetObj->X) > iSkillDistance ||
		 abs(lpObj->Y - lpTargetObj->Y) > iSkillDistance )
	{
		g_Log.Add("[SKILL DISTANCE CHECK] [%s][%s] Invalid Skill attacker(%d, %d), defender(%d, %d), skillnum:%d skilldistance:%d",
			lpObj->AccountID, lpObj->Name,
			lpObj->X, lpObj->Y, 
			lpTargetObj->X, lpTargetObj->Y,
			iSkillNum, iSkillDistance);

		return FALSE;
	}

	return TRUE;
}

void gObjSaveChaosBoxItemList(LPOBJ lpObj)
{
	unsigned char ExOption[MAX_EXOPTION_SIZE];

	for (int n = 0; n < CHAOS_BOX_SIZE; n++)
	{
		if (lpObj->pChaosBox[n].IsItem() == TRUE)
		{
			ItemIsBufExOption(ExOption, &lpObj->pChaosBox[n]);

			g_Log.Add("[ChaosBoxItemList][Lost ItemList] [%s][%s] [%d,%s,%d,%d,%d,%d] Serial:[%I64d][%d] Ex:[%d,%d,%d,%d,%d,%d,%d] Set:[%d] 380:[%d] HO:[%d,%d] E:[%d]",
				lpObj->AccountID, lpObj->Name, n, lpObj->pChaosBox[n].GetName(), lpObj->pChaosBox[n].m_Level, lpObj->pChaosBox[n].m_Option1, lpObj->pChaosBox[n].m_Option2, lpObj->pChaosBox[n].m_Option3, lpObj->pChaosBox[n].m_Number,
				(int)lpObj->pChaosBox[n].m_Durability, ExOption[0], ExOption[1], ExOption[2], ExOption[3], ExOption[4], ExOption[5], ExOption[6], lpObj->pChaosBox[n].m_SetOption,
				lpObj->pChaosBox[n].m_ItemOptionEx >> 7, g_kJewelOfHarmonySystem.GetItemStrengthenOption(&lpObj->pChaosBox[n]), g_kJewelOfHarmonySystem.GetItemOptionLevel(&lpObj->pChaosBox[n]),
				lpObj->pChaosBox[n].m_BonusSocketOption);
		}
	}

	//DSMSG_CHAOSMACHINE_LOSTITEMS pMsg;
	//PHeadSetW((LPBYTE)&pMsg, 0xC4, sizeof(pMsg));

	//memcpy(pMsg.szAccountID, lpObj->AccountID, MAX_ACCOUNT_LEN + 1);
	//memcpy(pMsg.szName, lpObj->Name, MAX_ACCOUNT_LEN + 1);
	//ItemByteConvert32(pMsg.btLostItems, lpObj->pChaosBox, CHAOS_BOX_SIZE);

	//wsDataCli.DataSend((char *)&pMsg, sizeof(pMsg));
}

void gObjMuBotPayForUse(LPOBJ lpObj)
{
	if(lpObj->Type != OBJ_USER)
		return;

	if(!lpObj->m_PlayerData->m_MuBotEnable)
		return;

	if(lpObj->m_PlayerData->m_MuBotPayTime == 5)
	{
		int iMoney = (lpObj->Level+lpObj->m_PlayerData->MasterLevel) * g_ConfigRead.data.mubot.cost;

		if(iMoney > lpObj->m_PlayerData->Money)
		{
			lpObj->m_PlayerData->m_MuBotEnable = false;
			GSProtocol.GCAnsMuBotUse(lpObj->m_Index, 1, lpObj->m_PlayerData->m_MuBotTotalTime, 0);
			return;
		}

		lpObj->m_PlayerData->Money -= iMoney;
		GSProtocol.GCMoneySend(lpObj->m_Index, lpObj->m_PlayerData->Money);

		lpObj->m_PlayerData->m_MuBotPayTime = 0;

		GSProtocol.GCAnsMuBotUse(lpObj->m_Index, 0, lpObj->m_PlayerData->m_MuBotTotalTime, iMoney);
	}
}

BOOL gObjGetRandomFreeArea(int iMapNumber, BYTE &cX, BYTE &cY, int iSX, int iSY, int iDX, int iDY, int iLoopCount)
{
	if (MAX_MAP_RANGE(iMapNumber) == FALSE)
	{
		return FALSE;
	}

	if( iSX <= 0 ) iSX = 1;
	if( iSY <= 0 ) iSY = 1;
	if( iDX <= 0 ) iDX = 1;
	if( iDY <= 0 ) iDY = 1;
	
	if( iSX >= 256 ) iSX = 255;
	if( iSY >= 256 ) iSY = 255;
	if( iDX >= 256 ) iDX = 255;
	if( iDY >= 256 ) iDY = 255;

	if( iLoopCount <= 0 ) iLoopCount = 1;

	while( iLoopCount-- > 0 )
	{
		cX = (rand() % (iDX-iSX)) + iSX;
		cY = (rand() % (iDY-iSY)) + iSY;

		BYTE btMapAttr = MapC[iMapNumber].GetAttr(cX, cY);

		if( btMapAttr == 0 )
			return TRUE;
	}
	return FALSE;
}

void gObjAddSpecificSkillsForSiege(LPOBJ lpObj)
{

}

int gObjCalcHPPercent(double Life, double MaxLife)
{
	if ( Life <= 0 ) 
	{
		return 0;
	}

	return (int)Life * 100 / (int)MaxLife;
}

bool CheckAuthorityCondition(int AuthorityCode, LPOBJ lpObj) //0043F3E0 identical season4.5 add-on
{
	return ((lpObj->Authority&AuthorityCode) == lpObj->Authority)?true:false;
}

void gObjReqMapSvrAuth(LPOBJ lpObj)
{
	if ( lpObj->Connected != PLAYER_CONNECTED )
	{
		g_Log.Add("%s is not connected", lpObj->AccountID);
		IOCP.CloseClient(lpObj->m_Index);
		return;
	}

	GJReqMapSvrAuth(lpObj->m_Index, lpObj->AccountID, lpObj->m_MapServerAuthInfo.szCharName, 
		lpObj->m_MapServerAuthInfo.iJA1, lpObj->m_MapServerAuthInfo.iJA2,
		lpObj->m_MapServerAuthInfo.iJA3, lpObj->m_MapServerAuthInfo.iJA4);

	g_Log.Add("[MapServerMng] Map Server Join Send : [%s][%s](%d)",
		lpObj->AccountID, lpObj->m_MapServerAuthInfo.szCharName, lpObj->m_Index);

	memset(lpObj->m_MapServerAuthInfo.szCharName, 0x00, MAX_ACCOUNT_LEN+2);
	lpObj->m_MapServerAuthInfo.iJA1 = 0;
	lpObj->m_MapServerAuthInfo.iJA2 = 0;
	lpObj->m_MapServerAuthInfo.iJA3 = 0;
	lpObj->m_MapServerAuthInfo.iJA4 = 0;
}

int gGetPartyMaxLevel(int nPartyNumber)
{
	int nResult = -1;

	for(int i=0; i<MAX_USER_IN_PARTY; i++)
	{
		int nUserIndex = gParty.m_PartyS[nPartyNumber].Number[i];

		if ( nUserIndex <= 0 )
		{
			continue;
		}

		if ( gObj[nUserIndex].Type != OBJ_USER )
		{
			continue;
		}

		if ( nResult < (gObj[nUserIndex].Level+gObj[nUserIndex].m_PlayerData->MasterLevel) )
		{
			nResult = gObj[nUserIndex].Level+gObj[nUserIndex].m_PlayerData->MasterLevel;
		}
	}

	return nResult;
}

int gGetLowHPMonster(int nZoneIndex, int nIndex, int nDistance)
{
	LPOBJ lpObj = &gObj[nIndex];
	int nTargetIndex = -1;
	int nTempTargetLife = 0;
	int nTargetLife = 0;

	_stZoneInfo * result = g_ImperialGuardian.GetZoneInfo(result, nZoneIndex);

	DWORD nMonsterCount = result->m_vtMonsterIndexInfo.size();

	for (int i=0;i<nMonsterCount;i++)
	{
		_stMonsterIndexInfo monsterIndexInfo = result->m_vtMonsterIndexInfo.at(i);

		if ( gObjCalDistance(lpObj, &gObj[monsterIndexInfo.m_nIndex]) < nDistance )
		{
			if ( monsterIndexInfo.m_bLive == true )
			{
				nTempTargetLife = gObj[monsterIndexInfo.m_nIndex].Life / gObj[monsterIndexInfo.m_nIndex].MaxLife * 100;

				if ( nTempTargetLife < nTargetLife || !nTargetLife )
				{
					if ( nTempTargetLife != 100 )
					{
						if ( nTempTargetLife > 0 )
						{
							nTargetLife = nTempTargetLife;
							nTargetIndex = monsterIndexInfo.m_nIndex;
						}
					}
				}
			}
		}
	}

	return nTargetIndex;
}

BOOL gObjGamblingInventoryCheck(int aIndex, int iWidth, int iHeight)
{
	for (int h = 0; h < MAX_INVENTORY_H; h++)
	{
		for (int w = 0; w < 8; w++)
		{
			if (*(BYTE*)(gObj[aIndex].pInventoryMap + h * 8 + w) == 0xFF)
			{
				BYTE blank = gObjOnlyInventoryRectCheck(aIndex, w, h, iWidth, iHeight);

				if (blank == 0xFE)
					return FALSE;
				if (blank != 0xFF)
					return TRUE;
			}
		}
	}

	return FALSE;
}

void gObjPentagramMixBoxSet(int aIndex, int itempos, int xl, int yl, BYTE set_byte)
{
	int itemposx = (itempos) % 8;
	int itemposy = (itempos) / 8;

	int xx, yy;

	for (int y = 0; y < yl; y++)
	{
		yy = itemposy + y;

		for (int x = 0; x < xl; x++)
		{
			xx = itemposx + x;

			if (ExtentCheck(xx, yy, 8, 4) == 1)
			{
				*(BYTE*)(gObj[aIndex].m_PlayerData->pPentagramMixBoxMap + (itemposy + y) * 8 + (itemposx + x)) = set_byte;
			}
			else
			{
				g_Log.Add("error : %d/%d  %s %d", xx, yy, __FILE__, __LINE__);
				return;
			}
		}
	}
}

void gObjPentagramMixItemSet(int aIndex, int itempos, BYTE set_byte)
{
	int width;
	int height;
	if (itempos < 0)
	{
		return;
	}
	else if (itempos >(CHAOS_BOX_SIZE - 1))
	{
		return;
	}

	if (gObj[aIndex].m_PlayerData->pPentagramMixBox[itempos].GetSize((int&)width, (int &)height) == 0)
	{
		g_Log.Add("error: Item does not exist %s %d", __FILE__, __LINE__);
		return;
	}

	gObjPentagramMixBoxSet(aIndex, itempos, width, height, set_byte);
}

BYTE gObjPentagramMixBoxInsertItemPos(int aIndex, CItem item, int pos, int source)
{
	LPOBJ lpObj;
	BYTE TempMap[CHAOS_BOX_MAP_SIZE];

	int w, h, iwidth, iheight;

	int blank, useClass;

	if (pos < 0 || pos >(CHAOS_BOX_SIZE - 1))
	{
		return -1;
	}

	useClass = 0;

	lpObj = &gObj[aIndex];

	if (lpObj->m_PlayerData->pPentagramMixBox[pos].IsItem() == 1)
	{
		return -1;
	}

	if (item.IsItem() == 0)
	{
		return -1;
	}

	w = pos % 8;
	h = pos / 8;

	if (ExtentCheck(w, h, 8, 4) == 0)
	{
		return -1;
	}

	item.GetSize((int &)iwidth, (int &)iheight);

	if (source >= 0)
	{
		memcpy(TempMap, lpObj->m_PlayerData->pPentagramMixBoxMap, CHAOS_BOX_MAP_SIZE);
		gObjPentagramMixBoxSet(lpObj->m_Index, source, iwidth, iheight, 255);
	}

	if (*(BYTE*)(lpObj->m_PlayerData->pPentagramMixBoxMap + h * 8 + w) != 255)
	{
		if (source >= 0)
		{
			memcpy(lpObj->m_PlayerData->pPentagramMixBoxMap, TempMap, CHAOS_BOX_MAP_SIZE);
		}
		return -1;
	}

	blank = gObjMapRectCheck(lpObj->m_PlayerData->pPentagramMixBoxMap, w, h, 8, 4, iwidth, iheight);

	if (blank == 255)
	{
		if (source >= 0)
		{
			memcpy(lpObj->m_PlayerData->pPentagramMixBoxMap, TempMap, CHAOS_BOX_MAP_SIZE);
		}
		return -1;
	}

	lpObj->m_PlayerData->pPentagramMixBox[pos] = item;
	return pos;
}

BYTE gObjPentagramMixBoxInsertItem(int aIndex, CItem item)
{
	if (item.IsItem() == FALSE)
	{
		return -1;
	}

	int iwidth, iheight;
	item.GetSize(iwidth, iheight);
	BYTE blank = 0;

	for (int h = 0; h < 4; h++)
	{
		for (int w = 0; w < 8; w++)
		{
			if (*(BYTE*)(gObj[aIndex].m_PlayerData->pPentagramMixBoxMap + h * 8 + w) == 255)
			{
				blank = gObjPentagramMixBoxRectCheck(aIndex, w, h, iwidth, iheight);

				if (blank == 254)
				{
					goto GOTO_EndFunc;
				}

				if (blank != 255)
				{
					gObjPentagramMixBoxInsertItemPos(aIndex, item, blank, -1);
					return blank;
				}
			}
		}
	}
GOTO_EndFunc:
	return -1;
}

BYTE gObjPentagramMixBoxRectCheck(int aIndex, int sx, int sy, int width, int height)
{
	int x;
	int y;
	int xx;
	int yy;

	if ((sx + width) > 8)
	{
		return -1;
	}

	if ((sy + height) > 4)
	{
		return -2;
	}

	for (y = 0; y<height; y++)
	{
		yy = sy + y;

		for (x = 0; x<width; x++)
		{
			xx = sx + x;

			if (::ExtentCheck(xx, yy, 8, 4) == TRUE)
			{
				if (*(BYTE*)(gObj[aIndex].m_PlayerData->pPentagramMixBoxMap + yy * 8 + xx) != (BYTE)-1)
				{
					return -1;
				}
			}
			else
			{
				g_Log.Add("error : %s %d", __FILE__, __LINE__);
				return -1;
			}
		}
	}

	return (sy * 8 + sx);
}

BYTE gObjPentagramMixBoxDeleteItem(int aIndex, int itempos)
{
	gObjPentagramMixItemSet(aIndex, itempos, -1);
	gObj[aIndex].m_PlayerData->pPentagramMixBox[itempos].Clear();
	return TRUE;
}

BOOL gTransPentagramJewelInfoTest(int aIndex, int targetIndex, CItem sourceItem)
{
	BYTE iTargetDBJewelIndex = 0xFF;

	for (int i = 0;; i++)
	{
		if (i >= 5)
			return TRUE;

		if (sourceItem.m_SocketOption[i] < 0xFE)
			break;
	}

	for (int index = 0; index < 250; index++)
	{
		BOOL bIndexExist = FALSE;

		for (int j = 0; j < 250; j++)
		{
			if (gObj[aIndex].m_PlayerData->m_PentagramJewelInfo_Inven[j].btJewelIndex != 0xFF)
			{
				if (gObj[aIndex].m_PlayerData->m_PentagramJewelInfo_Inven[j].btJewelIndex == index)
				{
					bIndexExist = TRUE;
				}
			}
		}

		if (!bIndexExist)
		{
			iTargetDBJewelIndex = index;
			break;
		}
	}

	if (iTargetDBJewelIndex == 0xFF)
	{
		g_Log.Add("[ gTransPentagramJewelInfo ] PentagramJewelIndex Area Not Exist!!");
		return FALSE;
	}

	return TRUE;
}

int gTransPentagramJewelInfo(int aIndex, BYTE madeItemPos, int targetIndex, CItem sourceItem, int *nJewelCount)
{
	LPOBJ lpObj = &gObj[aIndex];
	LPOBJ lpTargetObj = &gObj[targetIndex];

	if (lpObj->Type != OBJ_USER)
	{
		return FALSE;
	}

	if (lpTargetObj->Type != OBJ_USER)
	{
		return FALSE;
	}

	BYTE btJewelPos;
	BYTE btItemType;
	WORD wItemIndex;
	BYTE btMainAttribute;
	BYTE btLevel;
	BYTE btRank1OptionNum;
	BYTE btRank1Level;
	BYTE btRank2OptionNum;
	BYTE btRank2Level;
	BYTE btRank3OptionNum;
	BYTE btRank3Level;
	BYTE btRank4OptionNum;
	BYTE btRank4Level;
	BYTE btRank5OptionNum;
	BYTE btRank5Level;
	BYTE btTargetDBJewelIndex = 0xFF;
	BOOL bSave = FALSE;
	*nJewelCount = 0;

	for (int i = 0; i < 5; i++)
	{
		if (sourceItem.m_SocketOption[i] < 0xFE)
		{
			for (int index = 0; index < 250; index++)
			{
				BOOL bIndexExist = FALSE;

				for (int j = 0; j < 250; j++)
				{
					if (lpObj->m_PlayerData->m_PentagramJewelInfo_Inven[j].btJewelIndex != 0xFF)
					{
						if (lpObj->m_PlayerData->m_PentagramJewelInfo_Inven[j].btJewelIndex == index)
						{
							bIndexExist = TRUE;
						}
					}
				}

				if (!bIndexExist)
				{
					btTargetDBJewelIndex = index;
					break;
				}
			}

			if (btTargetDBJewelIndex == 0xFF)
			{
				g_Log.Add("[TransPentagramJewelInfo][%s][%s] PentagramJewelIndex Area Not Exist!!", lpObj->AccountID, lpObj->Name);
				return FALSE;
			}

			bSave = TRUE;
			btJewelPos = 0xFF;
			*nJewelCount++;
			lpObj->pInventory[madeItemPos].m_SocketOption[i] = btTargetDBJewelIndex;

			for (int k = 0; k < 250; k++)
			{
				if (lpTargetObj->m_PlayerData->m_PentagramJewelInfo_Inven[k].btJewelIndex == sourceItem.m_SocketOption[i])
				{
					btJewelPos = 0;
					btItemType = lpTargetObj->m_PlayerData->m_PentagramJewelInfo_Inven[k].btItemType;
					wItemIndex = lpTargetObj->m_PlayerData->m_PentagramJewelInfo_Inven[k].wItemIndex;
					btMainAttribute = lpTargetObj->m_PlayerData->m_PentagramJewelInfo_Inven[k].btMainAttribute;
					btLevel = lpTargetObj->m_PlayerData->m_PentagramJewelInfo_Inven[k].btLevel;
					btRank1OptionNum = lpTargetObj->m_PlayerData->m_PentagramJewelInfo_Inven[k].btRank1OptionNum;
					btRank1Level = lpTargetObj->m_PlayerData->m_PentagramJewelInfo_Inven[k].btRank1Level;
					btRank2OptionNum = lpTargetObj->m_PlayerData->m_PentagramJewelInfo_Inven[k].btRank2OptionNum;
					btRank2Level = lpTargetObj->m_PlayerData->m_PentagramJewelInfo_Inven[k].btRank2Level;
					btRank3OptionNum = lpTargetObj->m_PlayerData->m_PentagramJewelInfo_Inven[k].btRank3OptionNum;
					btRank3Level = lpTargetObj->m_PlayerData->m_PentagramJewelInfo_Inven[k].btRank3Level;
					btRank4OptionNum = lpTargetObj->m_PlayerData->m_PentagramJewelInfo_Inven[k].btRank4OptionNum;
					btRank4Level = lpTargetObj->m_PlayerData->m_PentagramJewelInfo_Inven[k].btRank4Level;
					btRank5OptionNum = lpTargetObj->m_PlayerData->m_PentagramJewelInfo_Inven[k].btRank5OptionNum;
					btRank5Level = lpTargetObj->m_PlayerData->m_PentagramJewelInfo_Inven[k].btRank5Level;
					break;
				}
			}

			if (btJewelPos)
				return FALSE;

			for (int l = 0; l < 250; l++)
			{
				if (lpObj->m_PlayerData->m_PentagramJewelInfo_Inven[l].btJewelIndex == 0xFF)
				{
					lpObj->m_PlayerData->m_PentagramJewelInfo_Inven[l].btJewelPos = 0;
					lpObj->m_PlayerData->m_PentagramJewelInfo_Inven[l].btJewelIndex = btTargetDBJewelIndex;
					lpObj->m_PlayerData->m_PentagramJewelInfo_Inven[l].btItemType = btItemType;
					lpObj->m_PlayerData->m_PentagramJewelInfo_Inven[l].wItemIndex = wItemIndex;
					lpObj->m_PlayerData->m_PentagramJewelInfo_Inven[l].btMainAttribute = btMainAttribute;
					lpObj->m_PlayerData->m_PentagramJewelInfo_Inven[l].btLevel = btLevel;
					lpObj->m_PlayerData->m_PentagramJewelInfo_Inven[l].btRank1OptionNum = btRank1OptionNum;
					lpObj->m_PlayerData->m_PentagramJewelInfo_Inven[l].btRank1Level = btRank1Level;
					lpObj->m_PlayerData->m_PentagramJewelInfo_Inven[l].btRank2OptionNum = btRank2OptionNum;
					lpObj->m_PlayerData->m_PentagramJewelInfo_Inven[l].btRank2Level = btRank2Level;
					lpObj->m_PlayerData->m_PentagramJewelInfo_Inven[l].btRank3OptionNum = btRank3OptionNum;
					lpObj->m_PlayerData->m_PentagramJewelInfo_Inven[l].btRank3Level = btRank3Level;
					lpObj->m_PlayerData->m_PentagramJewelInfo_Inven[l].btRank4OptionNum = btRank4OptionNum;
					lpObj->m_PlayerData->m_PentagramJewelInfo_Inven[l].btRank4Level = btRank4Level;
					lpObj->m_PlayerData->m_PentagramJewelInfo_Inven[l].btRank5OptionNum = btRank5OptionNum;
					lpObj->m_PlayerData->m_PentagramJewelInfo_Inven[l].btRank5Level = btRank5Level;
					break;
				}
			}

			g_PentagramSystem.DBREQ_InsertPentagramJewel(aIndex, btJewelPos, btTargetDBJewelIndex,
				btItemType, wItemIndex, btMainAttribute, btLevel, btRank1OptionNum, btRank1Level,
				btRank2OptionNum, btRank2Level, btRank3OptionNum, btRank3Level,
				btRank4OptionNum, btRank4Level, btRank5OptionNum, btRank5Level);

			g_Log.Add("[TransPentagramJewelInfo][%s][%s] PentagramJewel DB Save : DBIndex = %d, ItemType = %d, ItemIndex = %d, MainAttribute = %d, 1RankN/L = %d/%d, 2RankN/L = %d/%d, 3RankN/L = %d/%d, 4RankN/L = %d/%d, 5RankN/L = %d/%d ",
				lpObj->AccountID, lpObj->Name, btTargetDBJewelIndex, btItemType, wItemIndex, btMainAttribute, btRank1OptionNum, btRank1Level,
				btRank2OptionNum, btRank2Level, btRank3OptionNum, btRank3Level, btRank4OptionNum, btRank4Level, btRank5OptionNum, btRank5Level);

			for (int m = 0; m < 250; m++)
			{
				if (lpTargetObj->m_PlayerData->m_PentagramJewelInfo_Inven[m].btJewelIndex == sourceItem.m_SocketOption[i])
				{
					lpTargetObj->m_PlayerData->m_PentagramJewelInfo_Inven[m].btJewelPos = 0;
					lpTargetObj->m_PlayerData->m_PentagramJewelInfo_Inven[m].btJewelIndex = -1;
					lpTargetObj->m_PlayerData->m_PentagramJewelInfo_Inven[m].btItemType = -1;
					lpTargetObj->m_PlayerData->m_PentagramJewelInfo_Inven[m].wItemIndex = -1;
					lpTargetObj->m_PlayerData->m_PentagramJewelInfo_Inven[m].btMainAttribute = -1;
					lpTargetObj->m_PlayerData->m_PentagramJewelInfo_Inven[m].btLevel = 0;
					lpTargetObj->m_PlayerData->m_PentagramJewelInfo_Inven[m].btRank1OptionNum = -1;
					lpTargetObj->m_PlayerData->m_PentagramJewelInfo_Inven[m].btRank1Level = -1;
					lpTargetObj->m_PlayerData->m_PentagramJewelInfo_Inven[m].btRank2OptionNum = -1;
					lpTargetObj->m_PlayerData->m_PentagramJewelInfo_Inven[m].btRank2Level = -1;
					lpTargetObj->m_PlayerData->m_PentagramJewelInfo_Inven[m].btRank3OptionNum = -1;
					lpTargetObj->m_PlayerData->m_PentagramJewelInfo_Inven[m].btRank3Level = -1;
					lpTargetObj->m_PlayerData->m_PentagramJewelInfo_Inven[m].btRank4OptionNum = -1;
					lpTargetObj->m_PlayerData->m_PentagramJewelInfo_Inven[m].btRank4Level = -1;
					lpTargetObj->m_PlayerData->m_PentagramJewelInfo_Inven[m].btRank5OptionNum = -1;
					lpTargetObj->m_PlayerData->m_PentagramJewelInfo_Inven[m].btRank5Level = -1;
					break;
				}
			}

			g_PentagramSystem.DBREQ_DelPentagramJewel(targetIndex, 0, sourceItem.m_SocketOption[i]);
		}
	}

	if (bSave)
	{
		g_PentagramSystem.GCPentagramJewelInfo(aIndex, 0);
	}
}

WING_TYPE GetWingType(WORD type, WING_CHECK_FLAG eFlag)
{
	if (eFlag == WING_CHECK_FLAG::WCF_ITEMNUMBER)
	{
		if (type == ITEMGET(12, 0) || type == ITEMGET(12, 1) || type == ITEMGET(12, 2) || type == ITEMGET(12, 41))
		{
			return WING_TYPE::FIRST_WING;
		}

		else if (type == ITEMGET(12, 3) || type == ITEMGET(12, 4) || type == ITEMGET(12, 5) || type == ITEMGET(12, 6)
			|| type == ITEMGET(13, 30) || type == ITEMGET(12, 42) || type == ITEMGET(12, 49) || type == ITEMGET(12,269))
		{
			return WING_TYPE::SECOND_WING;
		}

		else if (type == ITEMGET(12, 36) || type == ITEMGET(12, 37) || type == ITEMGET(12, 38) || type == ITEMGET(12, 39) || type == ITEMGET(12, 40) ||
			type == ITEMGET(12, 43) || type == ITEMGET(12, 50) || type == ITEMGET(12,268) || type == ITEMGET(12,270))
		{
			return WING_TYPE::THIRD_WING;
		}

		else if ((type >= ITEMGET(12, 130) && type <= ITEMGET(12, 135)) || type == ITEMGET(12,278))
		{
			return WING_TYPE::LITTLE_WING;
		}

		else if (type >= ITEMGET(12, 262) && type <= ITEMGET(12, 265))
		{
			return WING_TYPE::MONSTER_WRING;
		}

		else if (type == ITEMGET(12, 266))
		{
			return WING_TYPE::CHAOSCASTLE_WING;
		}

		else if (type == ITEMGET(12, 267))
		{
			return WING_TYPE::GOODANDDEVIL_WING;
		}

		else
		{
			return WING_TYPE::NONE_WING;
		}
	}

	else
	{
		if (type == 0 || type == 1 || type == 2 || type == 41)
		{
			return WING_TYPE::FIRST_WING;
		}

		else if (type == 3 || type == 4 || type == 5 || type == 6 || type == 30 || type == 42 || type == 49 || type == 269)
		{
			return WING_TYPE::SECOND_WING;
		}

		else if (type == 36 || type == 37 || type == 38 || type == 39 || type == 40 || type == 43 || type == 50 || type == 268 || type == 270)
		{
			return WING_TYPE::THIRD_WING;
		}

		else if ((type >= 130 && type <= 135) || type == 278)
		{
			return WING_TYPE::LITTLE_WING;
		}

		else if (type >= 262 && type <= 265)
		{
			return WING_TYPE::MONSTER_WRING;
		}

		else if (type == 266)
		{
			return WING_TYPE::CHAOSCASTLE_WING;
		}

		else if (type == 267)
		{
			return WING_TYPE::GOODANDDEVIL_WING;
		}

		else
		{
			return WING_TYPE::NONE_WING;
		}
	}

	return WING_TYPE::NONE_WING;
}

void GetBundleCount(int count,int div,int& itemcount,int& left)
{
	itemcount = count/div;
	left = count%div;
}
int gObjGetJewelCountInInventory(int aIndex, BYTE type)
{
	LPOBJ lpObj = &gObj[aIndex];
	int returnCount = 0;
	switch(type)
	{
		case 1: //Jewel of Bless
			{
				for(int i=0;i<INVENTORY_SIZE;i++)
				{
					if(lpObj->pInventory[i].m_Type == ITEMGET(12,30))
					{
						switch(lpObj->pInventory[i].m_Level)
						{
						case 0:
							returnCount += 10;
							break;
						case 1:
							returnCount += 20;
							break;
						case 2:
							returnCount += 30;
							break;
						}
					}
					if(lpObj->pInventory[i].m_Type == ITEMGET(14,13))
					{
						returnCount++;
					}
				}
				return returnCount;
			}
			break;
		case 2: // jewel of soul
			{
				for(int i=0;i<INVENTORY_SIZE;i++)
				{
					if(lpObj->pInventory[i].m_Type == ITEMGET(12,31))
					{
						switch(lpObj->pInventory[i].m_Level)
						{
						case 0:
							returnCount += 10;
							break;
						case 1:
							returnCount += 20;
							break;
						case 2:
							returnCount += 30;
							break;
						}
					}
					if(lpObj->pInventory[i].m_Type == ITEMGET(14,14))
					{
						returnCount++;
					}
				}
				return returnCount;
			}
			break;
		case 3: // Jewel of Chaos
			{
				for(int i=0;i<INVENTORY_SIZE;i++)
				{
					if(lpObj->pInventory[i].m_Type == ITEMGET(12,141))
					{
						switch(lpObj->pInventory[i].m_Level)
						{
						case 0:
							returnCount += 10;
							break;
						case 1:
							returnCount += 20;
							break;
						case 2:
							returnCount += 30;
							break;
						}
					}
					if(lpObj->pInventory[i].m_Type == ITEMGET(12,15))
					{
						returnCount++;
					}
				}
				return returnCount;
			}
			break;
	}
}

void gObjAutoPartySet(LPOBJ lpObj /* master */, LPOBJ lpTargetObj /*requestor*/)
{
	if (lpObj->Type != OBJ_USER || lpTargetObj->Type != OBJ_USER)
	{
		g_Log.AddC(TColor::Red, "error %d", __LINE__);
		return;
	}

	if ( !gObjIsConnected(lpTargetObj) || !gObjIsConnected(lpObj) )
	{
		GSProtocol.GCResultSend(lpTargetObj->m_Index, 0x41, 0x03);
		return;
	}

	if ( lpObj->CloseCount >= 0 || lpTargetObj->CloseCount >= 0 )
	{
		GSProtocol.GCResultSend(lpTargetObj->m_Index, 0x41, 0x00);
		return;
	}

	if (lpTargetObj->m_PlayerData->m_bUsePartyMatching)
	{
		GSProtocol.GCResultSend(lpTargetObj->m_Index, 0x41, 0x09);
		return;
	}

	if ( g_NewPVP.IsDuel(*lpObj) || g_NewPVP.IsDuel(*lpTargetObj) )
	{
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,315), lpTargetObj->m_Index, 1);
		return;
	}

	if ( g_NewPVP.IsObserver(*lpTargetObj) || g_NewPVP.IsObserver(*lpObj) )
	{
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,316), lpTargetObj->m_Index, 1);
		return;
	}

	if (CC_MAP_RANGE(lpObj->m_Index) || CC_MAP_RANGE(lpTargetObj->m_Index))
	{
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,116), lpTargetObj->m_Index, 1);
		return;
	}

	if (lpObj->MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL)
	{
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,573), lpTargetObj->m_Index, 1);
		return;
	}

	if ( IT_MAP_RANGE(lpObj->MapNumber) || IT_MAP_RANGE(lpTargetObj->MapNumber)  )
	{
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,291), lpTargetObj->m_Index, 1);
		return;
	}

	if ( g_ArcaBattle.IsArcaBattleServer() && lpObj->m_PlayerData->GuildNumber != lpObj->m_PlayerData->GuildNumber) 
	{
		if (g_AcheronGuardianEvent.IsPlayStart() == false)
		{
			GSProtocol.GCServerMsgStringSend(Lang.GetText(0,522), lpTargetObj->m_Index, 1);
			return;
		}
	}

	if (g_GensSystem.IsMapBattleZone(lpObj->MapNumber) || g_GensSystem.IsMapBattleZone(lpTargetObj->MapNumber) )
	{
		if (g_GensSystem.CanGensCreatePartyOnBattleZone() == FALSE)
		{
			GSProtocol.GCServerMsgStringSend(Lang.GetText(0,533), lpTargetObj->m_Index, 1);
			return;
		}
	}

	if (g_GensSystem.GetGensInfluence(lpObj) != g_GensSystem.GetGensInfluence(lpTargetObj))
	{
		if (g_GensSystem.CanGensJoinPartyWhileOppositeGens() == FALSE)
		{
			GSProtocol.GCServerMsgStringSend(Lang.GetText(0,288), lpTargetObj->m_Index, 1);
			return;
		}
	}

	if (lpTargetObj->PartyNumber >= 0)
	{
		return;
	}

	char szTemp[250];
	int pnumber = -1;
	if (lpObj->PartyNumber < 0)
	{
		lpObj->PartyNumber = gParty.Create(lpObj->m_Index, lpObj->DBNumber, lpObj->Level);
	}

	if (lpObj->PartyNumber >= 0)
	{
		pnumber = lpObj->PartyNumber;
		int iPartyPos = gParty.Add(lpObj->PartyNumber, lpTargetObj->m_Index, lpTargetObj->DBNumber, lpTargetObj->Level);

		if (iPartyPos >= 0)
		{
			lpTargetObj->PartyNumber = lpObj->PartyNumber;
			wsprintf(szTemp, Lang.GetText(0,20), lpTargetObj->Name);
			GSProtocol.GCServerMsgStringSend(szTemp, lpObj->m_Index, 1);
			wsprintf(szTemp, Lang.GetText(0,20), lpObj->Name);
			GSProtocol.GCServerMsgStringSend(szTemp, lpTargetObj->m_Index, 1);
			gParty.Paint(pnumber);
		}
		else if (iPartyPos == -1)
		{
			GSProtocol.GCResultSend(lpObj->m_Index, 0x41, 2);
			GSProtocol.GCResultSend(lpTargetObj->m_Index, 0x41, 2);
		}
		else if (iPartyPos == -2)
		{
			//result = 2;
			wsprintf(szTemp, Lang.GetText(0,21), lpTargetObj->Name);
			GSProtocol.GCServerMsgStringSend(szTemp, lpTargetObj->m_Index, 1);
		}
	}

	if (pnumber >= 0)
	{
		GSProtocol.CGPartyListAll(pnumber);
	}
}

BYTE gObjMuunInventoryInsertItemPos(int aIndex, CItem item, int pos)
{
	if (pos < 0 || pos > MUUN_INVENTORY_SIZE)
	{
		return -1;
	}
	
	LPOBJ lpObj = &gObj[aIndex];
		
	if (gObj[aIndex].pMuunInventory[pos].IsItem() == TRUE)
	{
		return -1;
	}

	if (item.IsItem() == FALSE)
	{
		return -1;
	}

	lpObj->pMuunInventory[pos] = item;
	return pos;
}

bool gObjFixMuunInventoryPointer(int aIndex)
{
	if (!gObjIsConnected(aIndex))
	{
		g_Log.Add("[Fix Muun Inv.Ptr] [%s][%s] - disconnected", gObj[aIndex].AccountID, gObj[aIndex].Name);
		return false;
	}

	if (gObj[aIndex].pMuunInventory == gObj[aIndex].pMuunInventory1)
	{
		return true;
	}

	if (gObj[aIndex].pMuunInventory == gObj[aIndex].pMuunInventory2)
	{
		if (gObj[aIndex].pTransaction == 1)
		{
			g_Log.Add("[Fix Muun Inv.Ptr] [%s][%s] - Transaction == 1, Do not change Pointer", gObj[aIndex].AccountID, gObj[aIndex].Name);
			return false;
		}

		g_Log.Add("[Fix Muun Inv.Ptr] [%s][%s] - Muun Inventory Pointer was 2", gObj[aIndex].AccountID, gObj[aIndex].Name);

		for (int n = 0; n < MUUN_INVENTORY_SIZE; n++)
		{
			gObj[aIndex].pMuunInventory2[n].Clear();
		}
	}

	else
	{
		g_Log.Add("[Fix Muun Inv.Ptr] [%s][%s] - Muun Inventory Pointer was Wrong", gObj[aIndex].AccountID, gObj[aIndex].Name);
	}

	gObjSetMuunInventory1Pointer(&gObj[aIndex]);
	return false;
}

void gObjSetMuunInventory1Pointer(LPOBJ lpObj)
{
	lpObj->pMuunInventory = lpObj->pMuunInventory1;
}

void gObjSetMuunInventory2Pointer(LPOBJ lpObj)
{
	lpObj->pMuunInventory = lpObj->pMuunInventory2;
}

BYTE gObjChkMuunInventoryEmpty(OBJECTSTRUCT *lpObj)
{
	for (int i = 2; i < MUUN_INVENTORY_SIZE; ++i)
	{
		if (!lpObj->pMuunInventory[i].IsItem())
			return i;
	}
	return -1;
}

BYTE gObjMuunInventoryInsertItem(int aIndex, CItem item)
{
	CItem copyitem;
	int iwidth, iheight;

	if (item.GetSize(iwidth, iheight) == 0)
	{
		g_Log.Add("error: Item does not exist %s %d", __FILE__, __LINE__);
		return -1;
	}

	BYTE Ret = gObjChkMuunInventoryEmpty(&gObj[aIndex]);

	if (Ret == (BYTE)-1)
	{
		return (BYTE)-1;
	}

	if (gObj[aIndex].pMuunInventory[Ret].IsItem() == TRUE)
	{
		return -1;
	}

	memcpy(&gObj[aIndex].pMuunInventory[Ret], &item, sizeof(gObj[aIndex].pMuunInventory[Ret]));

	if (g_CMuunSystem.IsMuunItem(item.m_Type) == TRUE && g_CMuunSystem.IsMuunExpireDate(item.m_Type) == FALSE)
	{
		g_CMuunSystem.SetMuunItemAddPeriodData(&gObj[aIndex], item.m_Type, item.m_Number);
	}

	return Ret;
}

BYTE gObjMuunInventoryInsertItem(int aIndex, CMapItem *item)
{
	int iwidth, iheight;
	BYTE blank = 0;

	CItem copyitem;

	if (item->GetSize((int &)iwidth, (int &)iheight) == 0)
	{
		return -1;
	}

	BYTE Ret = gObjChkMuunInventoryEmpty(&gObj[aIndex]);

	if (Ret == 0xFF)
	{
		return -1;
	}

	if (gObj[aIndex].pMuunInventory[Ret].IsItem() == TRUE)
	{
		return -1;
	}

	copyitem.m_Level = item->m_Level;
	copyitem.m_Durability = item->m_Durability;
				
	copyitem.Convert(item->m_Type, item->m_Option1, item->m_Option2, item->m_Option3, item->m_NewOption, item->m_SetOption, item->m_ItemOptionEx, item->m_SocketOption, item->m_BonusSocketOption, 0, 3);
				
	copyitem.m_Number = item->m_Number;
				
	gObj[aIndex].pMuunInventory[Ret] = copyitem;
				
	if (g_CMuunSystem.IsMuunItem(item->m_Type) == TRUE && g_CMuunSystem.IsMuunExpireDate(item->m_Type) == FALSE)
	{
		g_CMuunSystem.SetMuunItemAddPeriodData(&gObj[aIndex], item->m_Type, item->m_Number);
	}

	return Ret;
}

BYTE gObjMuunInventoryDeleteItem(int aIndex, int itempos)
 {
	 g_CMuunSystem.ClearPeriodMuunItemData(&gObj[aIndex],gObj[aIndex].pMuunInventory[itempos].m_Type,gObj[aIndex].pMuunInventory[itempos].m_Number);
	 gObj[aIndex].pMuunInventory[itempos].Clear();
	 return 1;
 }

BYTE gObjMuunInvenMove(OBJECTSTRUCT *lpObj, int *durSsend, int *durTsend, BYTE source, BYTE target, BYTE *siteminfo)
{
	if (source < 0 > source > MUUN_INVENTORY_SIZE-1)
	{
		g_Log.Add("error : %d", __LINE__);
		return -1;
	}

	if (target < 0 > target > MUUN_INVENTORY_SIZE - 1)
	{
		g_Log.Add("error : %d", __LINE__);
		return -1;
	}

	if (source == target)
	{
		g_Log.Add("error : %d", __LINE__);
		return -1;
	}

	if (lpObj->pMuunInventory[source].IsItem() == FALSE)
	{
		return -1;
	}

	if (lpObj->pMuunInventory[target].IsItem() == TRUE)
	{
		return -1;
	}

	if (target < 2)
	{
		if (g_CMuunSystem.IsMuunItem(lpObj->pMuunInventory[source].m_Type) == FALSE)
		{
			return -1;
		}

		for (int i = 0; i < 2; i++)
		{
			if (lpObj->pMuunInventory[i].IsItem() == TRUE && source != i)
			{
				int EvolutionMuunItemIndex = g_CMuunSystem.GetEvolutionMuunItemIndex(lpObj->pMuunInventory[i].m_Type);

				if (EvolutionMuunItemIndex == -1)
				{
					int nBeforeMuunIndex = g_CMuunSystem.GetBeforeEvolutionMuunItemIndex(lpObj->pMuunInventory[i].m_Type);

					if (lpObj->pMuunInventory[i].m_Type == lpObj->pMuunInventory[source].m_Type || 
						nBeforeMuunIndex == lpObj->pMuunInventory[source].m_Type)
					{
						return -1;
					}
				}

				else if (lpObj->pMuunInventory[i].m_Type == lpObj->pMuunInventory[source].m_Type || 
					EvolutionMuunItemIndex == lpObj->pMuunInventory[source].m_Type)
				{
					return -1;
				}
			}
		}
	}

	lpObj->pMuunInventory[target] = lpObj->pMuunInventory[source];
	lpObj->pMuunInventory[source].Clear();

	ItemByteConvert(siteminfo, lpObj->pMuunInventory[target]);

	if (target < 2 || source < 2)
	{
		LPITEM_ATTRIBUTE pItemAttribute = GetItemAttr(lpObj->pMuunInventory[target].m_Type);

		if (!pItemAttribute)
		{
			return -1;
		}

		if (pItemAttribute->ItemKindA != 12)
		{
			return -1;
		}

		if (pItemAttribute->ItemKindA == 12 && !pItemAttribute->ItemKindB)
		{
			return -1;
		}

		g_CMuunSystem.MuunItemEquipment(lpObj->m_Index, target, source);

		if (target && !source)
		{
			GSProtocol.GCMuunEquipmentChange(lpObj->m_Index, 0);
		}

		else
		{
			GSProtocol.GCMuunEquipmentChange(lpObj->m_Index, 0);
		}
	}

	return 22;
}

int gObjMuunInventorySearchSerialNumber(OBJECTSTRUCT *lpObj, UINT64 serial)
{
	int result;
	int count; 

	UINT64 s_num; 
	UINT64 s_numa;

	count = 0;
	if (gItemSerialCheck)
	{
		if (serial)
		{
			for (int n = 0; n < MUUN_INVENTORY_SIZE; ++n)
			{
				s_num = lpObj->pMuunInventory[n].GetNumber();
				if (s_num)
				{
					if (s_num == serial)
						++count;
				}
			}
			if (count > 1)
			{
				for (int na = 0; na < MUUN_INVENTORY_SIZE; ++na)
				{
					s_numa = lpObj->pMuunInventory[na].GetNumber();
					if (s_numa && s_numa == serial)
					{
						GSProtocol.GCServerMsgStringSend(Lang.GetText(0,565) , lpObj->m_Index, 1);
						g_Log.Add("MuunInventory CopyItem %s", lpObj->AccountID);
						gObjUserKill(lpObj->m_Index);
					}
				}
				result = 0;
			}
			else
			{
				result = 1;
			}
		}
		else
		{
			result = 1;
		}
	}
	else
	{
		result = 1;
	}
	return result;
}

void gObjEventInventoryItemSet(int aIndex, int itempos, BYTE set_byte)
{
	int width, height;

	if (itempos < 0 || itempos > EVENT_INVENTORY_SIZE - 1)
	{
		return;
	}

	if (gObj[aIndex].pEventInventory[itempos].GetSize(width, height) == 0)
	{
		g_Log.Add("error %s %d", __FILE__, __LINE__);
		return;
	}

	gObjEventInventoryItemBoxSet(aIndex, itempos, width, height, set_byte);
}

void gObjEventInventoryItemBoxSet(int aIndex, int itempos, int xl, int yl, BYTE set_byte)
{
	int itemposx = itempos % 8;
	int itemposy = itempos / 8;

	for (int y = 0; y < yl; y++)
	{
		for (int x = 0; x < xl; x++)
		{
			if (ExtentCheck(x + itemposx, y + itemposy, 8, 4) == TRUE)
			{
				*(BYTE*)(gObj[aIndex].pEventInventoryMap + (itemposy + y) * 8 + (itemposx + x)) = set_byte;
			}

			else
			{
				g_Log.Add("error %s %d", __FILE__, __LINE__);
				return;
			}
		}
	}
}

bool gObjFixEventInventoryPointer(int aIndex)
{
	if (gObjIsConnected(aIndex) == 0)
	{
		g_Log.Add("[Fix Event Inv.Ptr] [%s][%s] - disconnected", gObj[aIndex].AccountID, gObj[aIndex].Name);
		return false;
	}

	if (gObj[aIndex].pEventInventory == gObj[aIndex].pEventInventory1)
	{
		return true;
	}

	if (gObj[aIndex].pEventInventory == gObj[aIndex].pEventInventory2)
	{
		if (gObj[aIndex].pTransaction == 1)
		{
			g_Log.Add("[Fix Event Inv.Ptr] [%s][%s] - Transaction == 1, Do not change Pointer", gObj[aIndex].AccountID, gObj[aIndex].Name);
			return false;
		}
		else
		{
			g_Log.Add("[Fix Event Inv.Ptr] [%s][%s] - Event Inventory Pointer was 2", gObj[aIndex].AccountID, gObj[aIndex].Name);

			for (int n = 0; n < INVENTORY_SIZE; n++)
			{
				gObj[aIndex].pEventInventory2[n].Clear();
			}
		}
	}
	else
	{
		g_Log.Add("[Fix Event Inv.Ptr] [%s][%s] - Event Inventory Pointer was Wrong", gObj[aIndex].AccountID, gObj[aIndex].Name);
	}

	gObjSetEventInventory1Pointer(&gObj[aIndex]);

	return false;
}

bool gObjEventInventoryDeleteItem(int aIndex, int itempos)
{
	gObjEventInventoryItemSet(aIndex, itempos, -1);
	gObj[aIndex].pEventInventory[itempos].Clear();

	return TRUE;
}

BYTE gObjEventInventoryInsertItemTemp(LPOBJ lpObj, CMapItem * Item)
{
	CItem item;
	int w, h, iwidth, iheight;
	BYTE blank = 0;

	if (Item->GetSize((int &)iwidth, (int &)iheight) == 0)
	{
		g_Log.Add("error: Item does not exist %s %d", __FILE__, __LINE__);
		return -1;
	}

	for (h = 0; h < 4; h++)
	{
		for (w = 0; w < 8; w++)
		{
			if (*(BYTE*)(lpObj->pEventInventoryMap + h * 8 + w) == 255)
			{
				blank = gObjEventInventoryRectCheck(lpObj->m_Index, w, h, iwidth, iheight);

				if (blank == 254)
				{
					goto GOTO_EndFunc;
				}

				if (blank != 255)
				{
					return blank;
				}
			}
		}
	}
GOTO_EndFunc:
	return -1;
}

BYTE gObjEventInventoryInsertItem(int aIndex, CMapItem * item)
{
	int w, h, iwidth, iheight;
	BYTE blank = 0;

	CItem copyitem;

	if (item->GetSize((int &)iwidth, (int &)iheight) == 0)
	{
		return -1;
	}

	for (h = 0; h < 4; h++)
	{
		for (w = 0; w < 8; w++)
		{
			if (*(BYTE*)(gObj[aIndex].pEventInventoryMap + h * 8 + w) == 255)
			{
				blank = gObjEventInventoryRectCheck(aIndex, w, h, iwidth, iheight);
				if (blank == 254)
				{
					goto GOTO_EndFunc;
				}

				if (blank != 255)
				{
					copyitem.m_Level = item->m_Level;
					copyitem.m_Durability = item->m_Durability;

					copyitem.Convert(item->m_Type, item->m_Option1, item->m_Option2, item->m_Option3, item->m_NewOption, item->m_SetOption, item->m_ItemOptionEx, item->m_SocketOption, item->m_BonusSocketOption, 0, CURRENT_DB_VERSION);
					copyitem.SetPetItemInfo(item->m_PetItem_Level, item->m_PetItem_Exp);

					copyitem.m_Number = item->m_Number;

					gObj[aIndex].pEventInventory[blank] = copyitem;

					gObjEventInventoryItemSet(aIndex, blank, gObj[aIndex].pEventInventory[blank].m_Type);
					return blank;
				}
			}
		}
	}

GOTO_EndFunc:
	return -1;
}

BYTE gObjEventInventoryInsertItem(int aIndex, CItem item)
{
	int w, h, iwidth, iheight;
	BYTE blank = 0;

	if (item.GetSize((int&)iwidth, (int&)iheight) == 0)
	{
		g_Log.Add("error: Item does not exist %s %d", __FILE__, __LINE__);
		return -1;
	}

	for (h = 0; h < 4; h++)
	{
		for (w = 0; w < 8; w++)
		{
			if (*(BYTE*)(gObj[aIndex].pEventInventoryMap + h * 8 + w) == 255)
			{
				blank = gObjEventInventoryRectCheck(aIndex, w, h, iwidth, iheight);

				if (blank == 254)
				{
					goto GOTO_EndFunc;
				}

				if (blank != 255)
				{
					if (gObjCheckSerial0ItemList(&item) != 0)
					{
						MsgOutput(aIndex, Lang.GetText(0,259));
						return -1;
					}

					if (gObjEventInventorySearchSerialNum(&gObj[aIndex], item.m_Number) == 0)
					{
						return -1;
					}

					gObj[aIndex].pEventInventory[blank] = item;

					gObjEventInventoryItemSet(aIndex, blank, gObj[aIndex].pEventInventory[blank].m_Type);
					return blank;
				}
			}
		}
	}

GOTO_EndFunc:
	return -1;
}

BYTE gObjEventInvenItemOverlap(LPOBJ lpObj, int *durSsend, int *durTsend, BYTE source, BYTE target)
{
	CItem* sitem = &lpObj->pEventInventory[source];
	CItem* titem = &lpObj->pEventInventory[target];

	int max_count = 0;

	if (titem->IsItem() == FALSE)
	{
		return 0;
	}

	LPITEM_ATTRIBUTE pSItemAttribute = GetItemAttr(sitem->m_Type);
	LPITEM_ATTRIBUTE pTItemAttribute = GetItemAttr(titem->m_Type);

	if (!pSItemAttribute || !pTItemAttribute)
	{
		return 1;
	}

	if (pSItemAttribute->ItemKindA != 11 || pTItemAttribute->ItemKindA != 11)
	{
		return 1;
	}

	max_count = IsOverlapItem(sitem->m_Type);

	if (max_count && titem->m_Type == sitem->m_Type && titem->m_Level == sitem->m_Level && titem->m_Durability < max_count)
	{
		int sdur = max_count - titem->m_Durability;

		if (sitem->m_Durability < titem->m_Durability)
		{
			sdur = titem->m_Durability;
		}

		titem->m_Durability = sdur + titem->m_Durability;
		sitem->m_Durability -= sdur;

		if (titem->m_Type == ITEMGET(14, 215) && titem->m_Durability >= max_count)
		{
			titem->m_Durability -= max_count;

			if (titem->m_Durability == 0.0)
			{
				gObjEventInventoryItemSet(lpObj->m_Index, target, -1);
				lpObj->pEventInventory[target].Clear();
				GSProtocol.GCEventInventoryItemDeleteSend(lpObj->m_Index, target, TRUE);

				*durTsend = FALSE;
			}

			else
			{
				*durTsend = TRUE;
			}

			ItemSerialCreateSend(lpObj->m_Index, 226, lpObj->X, lpObj->Y, ITEMGET(14, 216), sitem->m_Level, 0, 0, 0, 0, lpObj->m_Index, 0, 0, 0, 0, 0);
		}

		if (sitem->m_Durability <= 0.0)
		{
			gObjEventInventoryItemSet(lpObj->m_Index, source, -1);
			sitem->Clear();
			GSProtocol.GCEventInventoryItemDeleteSend(lpObj->m_Index, source, FALSE);
			*durSsend = TRUE;
			return 21;
		}

		else
		{
			*durSsend = TRUE;
		}
	}

	return -1;
}

BYTE gObjEventInvenMove(LPOBJ lpObj, int *durSsend, int *durTsend, BYTE source, BYTE target)
{
	BYTE TempEventInventoryMap[32];

	if (source > EVENT_INVENTORY_SIZE - 1)
	{
		return -1;
	}

	if (target > EVENT_INVENTORY_SIZE - 1)
	{
		return -1;
	}

	if (source == target)
	{
		return -1;
	}

	BYTE btRet = gObjEventInvenItemOverlap(lpObj, durSsend, durTsend, source, target);

	if (btRet == 1)
	{
		return -1;
	}

	else if (btRet == 21 || btRet == 0xFF)
	{
		return btRet;
	}

	if (lpObj->pEventInventory[source].IsItem() == FALSE || lpObj->pEventInventory[target].IsItem() == TRUE)
	{
		return -1;
	}

	int width, height;

	lpObj->pEventInventory[source].GetSize(width, height);
	memcpy(&TempEventInventoryMap, lpObj->pEventInventoryMap, EVENT_INVENTORY_MAP_SIZE);
	gObjEventInventoryItemBoxSet(lpObj->m_Index, source, width, height, -1);

	int w = target % 8;
	int h = target / 8;

	if (ExtentCheck(w, h, 8, 4) == FALSE)
	{
		memcpy(lpObj->pEventInventory, TempEventInventoryMap, EVENT_INVENTORY_MAP_SIZE);
		return -1;
	}

	if (*(BYTE*)(lpObj->pEventInventoryMap + h * 8 + w) != 0xFF)
	{
		memcpy(lpObj->pEventInventoryMap, TempEventInventoryMap, EVENT_INVENTORY_MAP_SIZE);
		return -1;
	}

	BYTE blank = gObjEventInventoryRectCheck(lpObj->m_Index, w, h, width, height);

	if (blank == 0xFF)
	{
		memcpy(lpObj->pEventInventoryMap, TempEventInventoryMap, EVENT_INVENTORY_MAP_SIZE);
		return -1;
	}

	memcpy(&lpObj->pEventInventory[blank], &lpObj->pEventInventory[source], sizeof(lpObj->pEventInventory[blank]));
	lpObj->pEventInventory[source].Clear();
	gObjEventInventoryItemBoxSet(lpObj->m_Index, blank, width, height, lpObj->pEventInventory[blank].m_Type);

	return 21;
}

BYTE gObjEventInventoryTradeMove(LPOBJ lpObj, BYTE source, BYTE target)
{
	int h, w, iwidth, iheight, s_num, blank;

	if (lpObj->m_bMapSvrMoveReq == true || lpObj->m_bMapSvrMoveQuit == true || lpObj->m_State == 32)
	{
		g_Log.AddC(TColor::Red, "[gObjEventInventoryTradeMove] Can't move item in inventory - MapServerMove");
		return -1;
	}

	if (source > EVENT_INVENTORY_SIZE)
	{
		g_Log.Add("error : %s %d", __FILE__, __LINE__);
		return -1;
	}

	if (lpObj->pEventInventory[source].IsItem() == 0)
	{
		return -1;
	}

	if (lpObj->TargetNumber < 0)
	{
		return -1;
	}

	if (lpObj->m_IfState.use == 0 || lpObj->m_IfState.type != 1)
	{
		return -1;
	}

	LPITEM_ATTRIBUTE pItemAttribute = GetItemAttr(lpObj->pEventInventory[source].m_Type);

	if (pItemAttribute == NULL)
	{
		return -1;
	}

	if (pItemAttribute->ItemKindA != 11)
	{
		return -1;
	}

	if (!IsTransactionItem(lpObj->pEventInventory[source].m_Type))
	{
		return -1;
	}

	lpObj->pEventInventory[source].GetSize((int &)iwidth, (int &)iheight);
	s_num = lpObj->pEventInventory[source].GetNumber();

	if (gObjCheckSerial0ItemList(&lpObj->pEventInventory[source]) != 0)
	{
		MsgOutput(lpObj->m_Index, Lang.GetText(0,259));
		g_Log.Add("[ANTI-HACK][Serial 0 Item] [Trade] (%s)(%s) Item(%s) Pos(%d)", lpObj->AccountID, lpObj->Name, lpObj->pEventInventory[source].GetName(), source);
		return -1;
	}

	if (gObjEventInventorySearchSerialNum(lpObj, s_num) == 0)
	{
		return -1;
	}

	w = target % 8;
	h = target / 8;

	if (ExtentCheck(w, h, 8, 4) == 0)
	{
		return -1;
	}

	if (*(BYTE*)(lpObj->TradeMap + h * 8 + w) == 255)
	{
		BYTE itembuf[13];
		blank = gObjTradeRectCheck(lpObj->m_Index, w, h, iwidth, iheight);

		if (blank == 255)
		{
			return -1;
		}

		lpObj->Trade[blank] = lpObj->pEventInventory[source];
		gObjEventInventoryDeleteItem(lpObj->m_Index, source);
		gObjTradeItemBoxSet(lpObj->m_Index, blank, iwidth, iheight, lpObj->Trade[blank].m_Type);
		ItemByteConvert(itembuf, lpObj->Trade[blank]);
		GSProtocol.GCTradeOtherAdd(lpObj->TargetNumber, blank, itembuf);
		return true;
	}

	return -1;
}

BYTE gObjTradeEventInventoryMove(LPOBJ lpObj, BYTE source, BYTE target)
{
	int iwidth, iheight;
	UINT64 s_num;
	BYTE itembuf[13];

	if (source > TRADE_BOX_SIZE)
	{
		g_Log.Add("error : %s %d", __FILE__, __LINE__);
		return -1;
	}

	if (target > EVENT_INVENTORY_SIZE)
	{
		g_Log.Add("error : %s %d", __FILE__, __LINE__);
		return -1;
	}

	if (lpObj->TargetNumber < 0)
	{
		return -1;
	}

	if (lpObj->Trade[source].IsItem() == 0)
	{
		return -1;
	}

	if (IsEventItem(lpObj->Trade[source].m_Type) == FALSE)
	{
		return -1;
	}

	if (lpObj->m_IfState.use == 0 || lpObj->m_IfState.type != 1)
	{
		return -1;
	}

	if (gObjEventInventoryInsertItemPos(lpObj->m_Index, lpObj->Trade[source], target, 1) == 255)
	{
		return -1;
	}

	s_num = 0;
	s_num = lpObj->Trade[source].m_Number;

	lpObj->Trade[source].GetSize((int &)iwidth, (int &)iheight);

	gObjTradeItemBoxSet(lpObj->m_Index, source, iwidth, iheight, 255);
	lpObj->Trade[source].Clear();

	ItemByteConvert(itembuf, lpObj->Trade[source]);

	GSProtocol.GCTradeOtherDel(lpObj->TargetNumber, source);

	if (gObjCheckSerial0ItemList(&lpObj->Trade[source]) != 0)
	{
		MsgOutput(lpObj->m_Index, Lang.GetText(0,259));
		g_Log.Add("[ANTI-HACK][Serial 0 Item] [Trade] (%s)(%s) Item(%s) Pos(%d)", lpObj->AccountID, lpObj->Name, lpObj->Trade[source].GetName(), source);
		return -1;
	}

	if (gObjEventInventorySearchSerialNum(lpObj, s_num) == 0)
	{
		return -1;
	}

	return 21;
}

void gObjTempEventInventoryItemBoxSet(BYTE * TempMap, int itempos, int xl, int yl, BYTE set_byte)
{
	int itemposx = itempos % 8;
	int itemposy = itempos/ 8;

	for (int y = 0; y < yl; y++)
	{
		for (int x = 0; x < xl; x++)
		{
			*(BYTE*)(TempMap + (itemposy + y) * 8 + (itemposx + x)) = set_byte;
		}
	}
}

BYTE gObjTempEventInventoryRectCheck(int aIndex, BYTE * TempMap, int sx, int sy, int width, int height)
{
	int x, y, blank = 0;

	if (sx + width > 8)
	{		
		return -1;
	}

	if (sy + height > 4)
	{
		return -2;
	}

	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			if (ExtentCheck(sx + x, sy + y, 8, 4) && *(BYTE*)(TempMap + (sy + y) * 8 + (sx + x)) != 255)
			{
				return -1;
			}
		}
	}

	if (blank == 0)
	{
		return sx + sy * 8;
	}

	return -1;
}

BYTE gObjTempEventInventoryInsertItem(LPOBJ lpObj, CItem item, BYTE * TempMap)
{
	int w, h, iwidth, iheight;
	BYTE blank = 0;

	if (item.GetSize(iwidth, iheight) == 0)
	{
		g_Log.Add("Item does not exist [%s][%d] %d", __FILE__, __LINE__, item.m_Type);
		return -1;
	}

	for (h = 0; h < 4; h++)
	{
		for (w = 0; w < 8; w++)
		{
			if (*(BYTE*)(TempMap + h * 8 + w) == 255)
			{
				blank = gObjTempEventInventoryRectCheck(lpObj->m_Index, TempMap, w, h, iwidth, iheight);

				if (blank == 254)
				{
					goto GOTO_EndFunc;
				}
				if (blank != 255)
				{
					gObjTempEventInventoryItemBoxSet(TempMap, blank, iwidth, iheight, item.m_Type);
					return blank;
				}
			}
		}
	}
GOTO_EndFunc:
	return -1;
}

BOOL CheckEventInventoryEmptySpace(LPOBJ lpObj, int iItemHeight, int iItemWidth)
{
	int h = 0;
	int w = 0;
	BYTE blank = 0;

	for (h = 0; h<4; h++)
	{
		for (w = 0; w<8; w++)
		{
			if (*(BYTE*)(lpObj->pEventInventoryMap + (h)* 8 + (w)) == 255)
			{
				blank = gObjEventInventoryRectCheck(lpObj->m_Index, w, h, iItemWidth, iItemHeight);

				if (blank == 0xFE)
					return FALSE;

				if (blank != 0xFF)
					return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL IsEventItem(CItem item)
{
	LPITEM_ATTRIBUTE pItemAttribute = GetItemAttr(item.m_Type);

	if (!pItemAttribute)
	{
		return FALSE;
	}

	return pItemAttribute->ItemKindA == 11;
}

BOOL IsEventItem(int iType)
{
	LPITEM_ATTRIBUTE pItemAttribute = GetItemAttr(iType);

	if (!pItemAttribute)
	{
		return FALSE;
	}

	return pItemAttribute->ItemKindA == 11;
}

BYTE gObjEventInventoryRectCheck(int aIndex, int sx, int sy, int width, int height)
{
	int x, y;
	int blank = 0;

	if (sx + width > 8)
	{
		return -1;
	}

	if (sy + height > 4)
	{
		return -2;
	}

	int xx, yy;

	for (y = 0; y < height; y++)
	{
		yy = sy + y;

		for (x = 0; x < width; x++)
		{
			xx = sx + x;

			if (ExtentCheck(xx, yy, 8, 4))
			{
				if (*(BYTE*)(gObj[aIndex].pEventInventoryMap + (sy + y) * 8 + (sx + x)) != 255)
				{
					return -1;
				}
			}
			else
			{
				g_Log.Add("error : %s %d", __FILE__, __LINE__);
				return -1;
			}
		}
	}

	if (blank == 0)
	{
		return sx + sy * 8;
	}

	return  -1;
}

BYTE gObjEventInventoryInsertItemPos(int aIndex, CItem item, int pos, BOOL RequestCheck)
{
	if (pos < 0 || pos > EVENT_INVENTORY_SIZE)
	{
		return -1;
	}

	LPOBJ lpObj;
	BYTE TempInventoryMap[EVENT_INVENTORY_MAP_SIZE];

	int w, h, iwidth, iheight;

	int blank, useClass = 0;

	lpObj = &gObj[aIndex];

	if (lpObj->pEventInventory[pos].IsItem() == 1)
	{
		return -1;
	}

	if (item.IsItem() == 0)
	{
		return -1;
	}

	w = pos % 8;
	h = pos / 8;

	item.GetSize((int &)iwidth, (int &)iheight);

	if (!ExtentCheck(w, h, 8, 4))
	{
		return -1;
	}

	memcpy(TempInventoryMap, lpObj->pEventInventoryMap, EVENT_INVENTORY_MAP_SIZE);

	if (*(BYTE*)(lpObj->pEventInventoryMap + h * 8 + w) != 255)
	{
		memcpy(lpObj->pEventInventoryMap, TempInventoryMap, EVENT_INVENTORY_MAP_SIZE);
		return -1;
	}

	blank = gObjEventInventoryRectCheck(lpObj->m_Index, w, h, iwidth, iheight);

	if (blank >= 254)
	{
		memcpy(lpObj->pEventInventoryMap, TempInventoryMap, EVENT_INVENTORY_MAP_SIZE);
		return false;
	}

	lpObj->pEventInventory[pos] = item;
	gObjEventInventoryItemSet(aIndex, pos, 1);

	return pos;
}

BOOL gObjEventInventorySearchSerialNum(LPOBJ lpObj, UINT64 serial)
{
	UINT64 s_num;
	int count = 0;

	if (gItemSerialCheck == 0)
	{
		return true;
	}

	if (serial == 0 || serial == (UINT64)-1)
	{
		return true;
	}

	for (int n = 0; n < EVENT_INVENTORY_SIZE; n++)
	{
		s_num = lpObj->pEventInventory[n].GetNumber();

		if (s_num != 0 && s_num == serial && s_num != (UINT64)-1)
		{
			count++;
		}
	}

	if (count <= 1)
	{
		return true;
	}

	for (int n = 0; n < EVENT_INVENTORY_SIZE; n++)
	{
		s_num = lpObj->pEventInventory[n].GetNumber();

		if (s_num != 0 && s_num == serial && s_num != (UINT64)-1)
		{
			g_Log.Add("error-L1: CopyItem Id[%s] Char[%s] Item[%s] InventoryPos[%d] serial[%I64d]", lpObj->AccountID, lpObj->Name, lpObj->pEventInventory[n].GetName(), n, s_num);
			GSProtocol.GCServerMsgStringSend(Lang.GetText(0,15), lpObj->m_Index, 1);
			gObjUserKill(lpObj->m_Index);
		}
	}

	return false;
}

void gObjSetEventInventory1Pointer(LPOBJ lpObj)
{
	lpObj->pEventInventory = lpObj->pEventInventory1;
	lpObj->pEventInventoryMap = lpObj->pEventInventoryMap1;
}

void gObjSetEventInventory2Pointer(LPOBJ lpObj)
{
	lpObj->pEventInventory = lpObj->pEventInventory2;
	lpObj->pEventInventoryMap = lpObj->pEventInventoryMap2;
}

BOOL IsNotApplyJewelToItems(WORD sItemType)
{
	if (sItemType == ITEMGET(4, 7) || sItemType == ITEMGET(4, 15))
	{
		return TRUE;
	}

	if (sItemType == ITEMGET(0, 41))
	{
		return TRUE;
	}

	WING_TYPE enWingType = GetWingType(sItemType, WCF_ITEMNUMBER);

	if (enWingType != NONE_WING)
	{
		if (enWingType == CHAOSCASTLE_WING || enWingType == GOODANDDEVIL_WING)
		{
			return TRUE;
		}

		else
		{
			return FALSE;
		}
	}

	if (sItemType >= ITEMGET(12, 200) && sItemType <= ITEMGET(12, 220))
	{
		return FALSE;
	}

	if (sItemType < ITEMGET(12, 0))
	{
		return FALSE;
	}
	
	return TRUE;
}

BOOL UseBundleOfBlessJewel(LPOBJ lpObj, int source, int target)
{
	if (source < 0 || source > MAIN_INVENTORY_SIZE)
	{
		return FALSE;
	}

	if (target < 0 || target > MAIN_INVENTORY_SIZE)
	{
		return FALSE;
	}

	if (lpObj->pInventory[source].IsItem() == FALSE)
	{
		return FALSE;
	}

	if (lpObj->pInventory[target].IsItem() == FALSE)
	{
		return FALSE;
	}

	if (g_MineSystem.IsPickax(lpObj->pInventory[target].m_Type) == FALSE)
	{
		return FALSE;
	}

	CItem * ItemPickax = &lpObj->pInventory[target];
	short sRepairValue = g_MineSystem.GetRepairValueOfJewel(lpObj->pInventory[source].m_Type, lpObj->pInventory[source].m_Level);

	if (sRepairValue < 0)
	{
		ItemPickax->m_Durability = 255.0;
	}

	if (ItemPickax->m_Durability + sRepairValue > 255.0)
	{
		return FALSE;
	}

	ItemPickax->m_Durability += sRepairValue;
	g_Log.Add("[MineSystem][PICKAX][BUNDLE_REPAIR][SUCCESS][%s][%s][Type:%d][Index:%d][Level:%d][Serial:%I64d] - %d/255 (+%d)",
		lpObj->AccountID, lpObj->Name, lpObj->pInventory[source].m_Type / 512, lpObj->pInventory[source].m_Type % 512, lpObj->pInventory[source].m_Level,
		lpObj->pInventory[source].m_Number, (int)lpObj->pInventory[source].m_Durability, sRepairValue);

	return TRUE;
}

BOOL IsExceptionJewelOfBlessInchant(LPOBJ lpObj, int source, int target)
{
	if (lpObj->pInventory[target].m_Type == ITEMGET(13, 37))
	{
		CItem * ItemFenrir = &lpObj->pInventory[target];

		if (ItemFenrir->m_Durability >= 255)
		{
			return false;
		}

		if (ItemFenrir->IsPeriodItem() == TRUE)
		{
			return false;
		}

		if (rand() % 10000 < g_iFenrirRepairRate)
		{
			int iAddDur = rand() % 150 + 50;

			if ((ItemFenrir->m_Durability + iAddDur) > 255)
			{
				ItemFenrir->m_Durability = 255.0f;
			}
			else
			{
				ItemFenrir->m_Durability += iAddDur;
			}

			MsgOutput(lpObj->m_Index, Lang.GetText(0,257), int(ItemFenrir->m_Durability));

			g_Log.Add("[FENRIR REPAIR][SUCCESS] [%s][%s] - %d/255 (+%d)", lpObj->AccountID, lpObj->Name, (int)ItemFenrir->m_Durability, iAddDur);
		}
		else
		{
			MsgOutput(lpObj->m_Index, Lang.GetText(0,258));

			g_Log.Add("[FENRIR REPAIR][FAILED] [%s][%s] - %d/255", lpObj->AccountID, lpObj->Name, (int)ItemFenrir->m_Durability);
		}

		return true;
	}

	else if (g_MineSystem.IsPickax(lpObj->pInventory[target].m_Type) == TRUE)
	{
		CItem * ItemPickax = &lpObj->pInventory[target];
		short sRepairValue = g_MineSystem.GetRepairValueOfJewel(lpObj->pInventory[source].m_Type, lpObj->pInventory[source].m_Level);

		if (sRepairValue < 0)
		{
			return FALSE;
		}

		if (ItemPickax->m_Durability + sRepairValue > 255.0)
		{
			return FALSE;
		}

		ItemPickax->m_Durability += sRepairValue;
		g_Log.Add("[MineSystem][PICKAX][REPAIR][SUCCESS][%s][%s][Type:%d][Index:%d][Level:%d][Serial:%I64d] - %d/255 (+%d)",
			lpObj->AccountID, lpObj->Name, lpObj->pInventory[source].m_Type / 512, lpObj->pInventory[source].m_Type % 512, lpObj->pInventory[source].m_Level,
			lpObj->pInventory[source].m_Number, (int)lpObj->pInventory[source].m_Durability, sRepairValue);

		return TRUE;
	}

	else if (lpObj->pInventory[target].m_Type == ITEMGET(13, 268))
	{
		CItem * DSFRing = &lpObj->pInventory[target];

		if (DSFRing->m_Durability >= 255)
		{
			return false;
		}

		if ((DSFRing->m_Durability + 50) > 255)
		{
			DSFRing->m_Durability = 255.0f;
		}
		else
		{
			DSFRing->m_Durability += 50;
		}

		g_Log.Add("[DSF_REWARD][SUCCESS] [%s][%s] - %d/255 (+%d)", lpObj->AccountID, lpObj->Name, (int)DSFRing->m_Durability, 50);

		return TRUE;
	}

	return FALSE;
}

BOOL IsTransformCharacterSkin(int pChange)
{
	switch (pChange)
	{
		case 2:
		case 7:
		case 8:
		case 9:
		case 14:
		case 41:
		case 372:
		case 373:
		case 374:
		case 378:
		case 477:
		case 503:
		case 548:
		case 616:
		case 617:
		case 625:
		case 626:
		case 642:
		case 646:
		case 706:
			return TRUE;
		default:
			return FALSE;
	}
	return FALSE;
}

BOOL gCountBlessSoulChaosJewel(int aIndex, short & sBlessJewelCount, short & s10BlessJewelCount, short & s20BlessJewelCount, short & s30BlessJewelCount, short & sSoulJewelCount, short & s10SoulJewelCount, short & s20SoulJewelCount, short & s30SoulJewelCount, short & sChaosJewelCount, short & s10ChaosJewelCount, short & s20ChaosJewelCount, short & s30ChaosJewelCount)
{
	if (!gObjIsConnected(aIndex))
	{
		return FALSE;
	}

	if (!ObjectMaxRange(aIndex))
	{
		return FALSE;
	}

	LPOBJ lpObj = &gObj[aIndex];

	for (int n = 0; n < MAIN_INVENTORY_SIZE; n++)
	{
		if (lpObj->pInventory[n].IsItem() == TRUE)
		{
			switch (lpObj->pInventory[n].m_Type)
			{
				case ITEMGET(14,13):
					sBlessJewelCount++;
					break;
				case ITEMGET(12,30):
					if (lpObj->pInventory[n].m_Level == 0)
					{
						s10BlessJewelCount++;
					}
					else if (lpObj->pInventory[n].m_Level == 1)
					{
						s20BlessJewelCount++;
					}
					else if (lpObj->pInventory[n].m_Level == 2)
					{
						s30BlessJewelCount++;
					}
					break;
				case ITEMGET(14,14):
					sSoulJewelCount++;
					break;
				case ITEMGET(12, 31):
					if (lpObj->pInventory[n].m_Level == 0)
					{
						s10SoulJewelCount++;
					}
					else if (lpObj->pInventory[n].m_Level == 1)
					{
						s20SoulJewelCount++;
					}
					else if (lpObj->pInventory[n].m_Level == 2)
					{
						s30SoulJewelCount++;
					}
					break;
				case ITEMGET(12,15):
					sChaosJewelCount++;
					break;
				case ITEMGET(12, 141):
					if (lpObj->pInventory[n].m_Level == 0)
					{
						s10ChaosJewelCount++;
					}
					else if (lpObj->pInventory[n].m_Level == 1)
					{
						s20ChaosJewelCount++;
					}
					else if (lpObj->pInventory[n].m_Level == 2)
					{
						s30ChaosJewelCount++;
					}
					break;
			}
		}
	}

	return TRUE;
}

BOOL gJewelBuyItemValueTest(int aBuyerIndex, int aSellerIndex, short sBlessJewelPrice, short sSoulJewelPrice, short sChaosJewelPrice, short sBlessJewelCount, short s10BlessJewelCount, short s20BlessJewelCount, short s30BlessJewelCount, short sSoulJewelCount, short s10SoulJewelCount, short s20SoulJewelCount, short s30SoulJewelCount, short sChaosJewelCount, short s10ChaosJewelCount, short s20ChaosJewelCount, short s30ChaosJewelCount,
	short &sBlessJewelNeedCount, short &s10BlessJewelNeedCount, short &s20BlessJewelNeedCount, short &s30BlessJewelNeedCount, short &sSoulJewelNeedCount, short &s10SoulJewelNeedCount, short &s20SoulJewelNeedCount, short &s30SoulJewelNeedCount, short &sChaosJewelNeedCount, short &s10ChaosJewelNeedCount, short &s20ChaosJewelNeedCount, short &s30ChaosJewelNeedCount)
{
	if (!gObjIsConnected(aBuyerIndex))
	{
		return FALSE;
	}

	if (!gObjIsConnected(aSellerIndex))
	{
		return FALSE;
	}

	if (!ObjectMaxRange(aBuyerIndex))
	{
		return FALSE;
	}

	if (!ObjectMaxRange(aSellerIndex))
	{
		return FALSE;
	}

	LPOBJ lpBuyerObj = &gObj[aBuyerIndex];
	LPOBJ lpSellerObj = &gObj[aSellerIndex];

	short sBPrice = sBlessJewelPrice;
	short sSPrice = sSoulJewelPrice;
	short sCPrice = sChaosJewelPrice;

	BOOL bMinus30BJ = FALSE;
	BOOL bMinus30SJ = FALSE;
	BOOL bMinus30CJ = FALSE;

	short sChaosNeedCount;
	short s10ChaosNeedCount;
	short s20ChaosNeedCount;
	short s30ChaosNeedCount;
	short sSoulNeedCount;
	short s10SoulNeedCount;
	short s20SoulNeedCount;
	short s30SoulNeedCount;
	short sBlessNeedCount;
	short s10BlessNeedCount;
	short s20BlessNeedCount;
	short s30BlessNeedCount;

	s30BlessNeedCount = sBlessJewelPrice / 30;
	if (s30BlessNeedCount >= s30BlessJewelCount)
	{
		s30BlessNeedCount = s30BlessJewelCount;
	}

	while (true)
	{
		sBPrice = sBPrice - 30 * s30BlessNeedCount;
		s20BlessNeedCount = sBPrice / 20;

		if (s20BlessNeedCount >= s20BlessJewelCount)
			s20BlessNeedCount = s20BlessJewelCount;

		sBPrice = sBPrice - 20 * s20BlessNeedCount;
		s10BlessNeedCount = sBPrice / 10;

		if (s10BlessNeedCount >= s10BlessJewelCount)
			s10BlessNeedCount = s10BlessJewelCount;

		sBPrice = sBPrice - 10 * s10BlessNeedCount;
		sBlessNeedCount = sBPrice;

		if (sBlessNeedCount >= sBlessJewelCount)
			sBlessNeedCount = sBlessJewelCount;

		if ((sBPrice - sBlessNeedCount) <= 0)
			break;

		if (s30BlessNeedCount <= 0 || bMinus30BJ)
			return 14;

		s30BlessNeedCount--;
		sBPrice = sBlessJewelPrice;
		bMinus30BJ = TRUE;
	}

	s30SoulNeedCount = sSoulJewelPrice / 30;
	if (s30SoulNeedCount >= s30SoulJewelCount)
	{
		s30SoulNeedCount = s30SoulJewelCount;
	}

	while (true)
	{
		sSPrice = sSPrice - 30 * s30SoulNeedCount;
		s20SoulNeedCount = sSPrice / 20;

		if (s20SoulNeedCount >= s20SoulJewelCount)
			s20SoulNeedCount = s20SoulJewelCount;

		sSPrice = sSPrice - 20 * s20SoulNeedCount;
		s10SoulNeedCount = sSPrice / 10;

		if (s10SoulNeedCount >= s10SoulJewelCount)
			s10SoulNeedCount = s10SoulJewelCount;

		sSPrice = sSPrice - 10 * s10SoulNeedCount;
		sSoulNeedCount = sSPrice;

		if (sSoulNeedCount >= sSoulJewelCount)
			sSoulNeedCount = sSoulJewelCount;

		if ((sSPrice - sSoulNeedCount) <= 0)
			break;

		if (s30SoulNeedCount <= 0 || bMinus30SJ)
			return 15;

		s30SoulNeedCount--;
		sSPrice = sSoulJewelPrice;
		bMinus30SJ = TRUE;
	}

	s30ChaosNeedCount = sChaosJewelPrice / 30;
	if (s30ChaosNeedCount >= s30ChaosJewelCount)
	{
		s30ChaosNeedCount = s30ChaosJewelCount;
	}

	while (true)
	{
		sCPrice = sCPrice - 30 * s30ChaosNeedCount;
		s20ChaosNeedCount = sCPrice / 20;

		if (s20ChaosNeedCount >= s20ChaosJewelCount)
			s20ChaosNeedCount = s20ChaosJewelCount;

		sCPrice = sCPrice - 20 * s20ChaosNeedCount;
		s10ChaosNeedCount = sCPrice / 10;

		if (s10ChaosNeedCount >= s10ChaosJewelCount)
			s10ChaosNeedCount = s10ChaosJewelCount;

		sCPrice = sCPrice - 10 * s10ChaosNeedCount;
		sChaosNeedCount = sCPrice;

		if (sChaosNeedCount >= sChaosJewelCount)
			sChaosNeedCount = sChaosJewelCount;

		if ((sCPrice - sChaosNeedCount) <= 0)
			break;

		if (s30ChaosNeedCount <= 0 || bMinus30CJ)
			return 16;

		s30ChaosNeedCount--;
		sCPrice = sChaosJewelPrice;
		bMinus30CJ = TRUE;
	}

	sBlessJewelNeedCount = sBlessNeedCount;
	s10BlessJewelNeedCount = s10BlessNeedCount;
	s20BlessJewelNeedCount = s20BlessNeedCount;
	s30BlessJewelNeedCount = s30BlessNeedCount;
	sSoulJewelNeedCount = sSoulNeedCount;
	s10SoulJewelNeedCount = s10SoulNeedCount;
	s20SoulJewelNeedCount = s20SoulNeedCount;
	s30SoulJewelNeedCount = s30SoulNeedCount;
	sChaosJewelNeedCount = sChaosNeedCount;
	s10ChaosJewelNeedCount = s10ChaosNeedCount;
	s20ChaosJewelNeedCount = s20ChaosNeedCount;
	s30ChaosJewelNeedCount = s30ChaosNeedCount;

	return FALSE;
}

BOOL gJewelInventoryPutTest(int aBuyerIndex, int aSellerIndex, short sBlessJewelPrice, short sSoulJewelPrice, short sChaosJewelPrice)
{
	if (!gObjIsConnected(aBuyerIndex))
	{
		return FALSE;
	}

	if (!gObjIsConnected(aSellerIndex))
	{
		return FALSE;
	}

	if (!ObjectMaxRange(aBuyerIndex))
	{
		return FALSE;
	}

	if (!ObjectMaxRange(aSellerIndex))
	{
		return FALSE;
	}

	LPOBJ lpBuyerObj = &gObj[aBuyerIndex];
	LPOBJ lpSellerObj = &gObj[aSellerIndex];

	int iBJCommisionCount = round(sBlessJewelPrice * 0.03);
	int iSJCommisionCount = round(sSoulJewelPrice * 0.03);
	int iCJCommisionCount = round(sChaosJewelPrice * 0.03);

	if (iBJCommisionCount > 50)
		iBJCommisionCount = 50;

	if (iSJCommisionCount > 50)
		iSJCommisionCount = 50;

	if (iCJCommisionCount > 50)
		iCJCommisionCount = 50;

	if (CheckInventoryEmptySpaceCount(lpSellerObj, 1, 1) >=
		((sChaosJewelPrice - iCJCommisionCount) % 30 % 20 % 10
		+ (sChaosJewelPrice - iCJCommisionCount) % 30 % 20 / 10
		+ (sChaosJewelPrice - iCJCommisionCount) % 30 / 20
		+ (sChaosJewelPrice - iCJCommisionCount) / 30
		+ (sSoulJewelPrice - iSJCommisionCount) % 30 % 20 % 10
		+ (sSoulJewelPrice - iSJCommisionCount) % 30 % 20 / 10
		+ (sSoulJewelPrice - iSJCommisionCount) % 30 / 20
		+ (sSoulJewelPrice - iSJCommisionCount) / 30
		+ (sBlessJewelPrice - iBJCommisionCount) % 30 % 20 % 10
		+ (sBlessJewelPrice - iBJCommisionCount) % 30 % 20 / 10
		+ (sBlessJewelPrice - iBJCommisionCount) % 30 / 20
		+ (sBlessJewelPrice - iBJCommisionCount) / 30))
	{
		return TRUE;
	}

	else
	{
		return 100;
	}
}

BOOL gJewelInventoryPut(int aBuyerIndex, int aSellerIndex, short sBlessJewelPrice, short sSoulJewelPrice, short sChaosJewelPrice, short sBlessJewelNeedCount, short s10BlessJewelNeedCount, short s20BlessJewelNeedCount, short s30BlessJewelNeedCount, short sSoulJewelNeedCount, short s10SoulJewelNeedCount, short s20SoulJewelNeedCount, short s30SoulJewelNeedCount, short sChaosJewelNeedCount, short s10ChaosJewelNeedCount, short s20ChaosJewelNeedCount, short s30ChaosJewelNeedCount)
{
	if (!gObjIsConnected(aBuyerIndex))
	{
		return FALSE;
	}

	if (!gObjIsConnected(aSellerIndex))
	{
		return FALSE;
	}

	if (!ObjectMaxRange(aBuyerIndex))
	{
		return FALSE;
	}

	if (!ObjectMaxRange(aSellerIndex))
	{
		return FALSE;
	}

	LPOBJ lpBuyerObj = &gObj[aBuyerIndex];
	LPOBJ lpSellerObj = &gObj[aSellerIndex];

	short sBJNeedCount = sBlessJewelNeedCount;
	short s10BJNeedCount = s10BlessJewelNeedCount;
	short s20BJNeedCount = s20BlessJewelNeedCount;
	short s30BJNeedCount = s30BlessJewelNeedCount;
	short sSJNeedCount = sSoulJewelNeedCount;
	short s10SJNeedCount = s10SoulJewelNeedCount;
	short s20SJNeedCount = s20SoulJewelNeedCount;
	short s30SJNeedCount = s30SoulJewelNeedCount;
	short sCJNeedCount = sChaosJewelNeedCount;
	short s10CJNeedCount = s10ChaosJewelNeedCount;
	short s20CJNeedCount = s20ChaosJewelNeedCount;
	short s30CJNeedCount = s30ChaosJewelNeedCount;

	for (int n = 0; n < 204; n++)
	{
		if (lpBuyerObj->pInventory[n].IsItem() == TRUE)
		{
			if (lpBuyerObj->pInventory[n].m_Type == ITEMGET(12, 30))
			{
				if (lpBuyerObj->pInventory[n].m_Level == 2)
				{
					if (s30BJNeedCount > 0)
					{
						gObjInventoryItemSet(lpBuyerObj->m_Index, n, -1);
						lpBuyerObj->pInventory[n].Clear();
						GSProtocol.GCInventoryItemDeleteSend(lpBuyerObj->m_Index, n, TRUE);
						s30BJNeedCount--;
					}
				}
			}

			if (lpBuyerObj->pInventory[n].m_Type == ITEMGET(12, 31))
			{
				if (lpBuyerObj->pInventory[n].m_Level == 2)
				{
					if (s30SJNeedCount > 0)
					{
						gObjInventoryItemSet(lpBuyerObj->m_Index, n, -1);
						lpBuyerObj->pInventory[n].Clear();
						GSProtocol.GCInventoryItemDeleteSend(lpBuyerObj->m_Index, n, TRUE);
						s30SJNeedCount--;
					}
				}
			}

			if (lpBuyerObj->pInventory[n].m_Type == ITEMGET(12, 141))
			{
				if (lpBuyerObj->pInventory[n].m_Level == 2)
				{
					if (s30CJNeedCount > 0)
					{
						gObjInventoryItemSet(lpBuyerObj->m_Index, n, -1);
						lpBuyerObj->pInventory[n].Clear();
						GSProtocol.GCInventoryItemDeleteSend(lpBuyerObj->m_Index, n, TRUE);
						s30CJNeedCount--;
					}
				}
			}
		}
	}

	for (int n = 0; n < 204; n++)
	{
		if (lpBuyerObj->pInventory[n].IsItem() == TRUE)
		{
			if (lpBuyerObj->pInventory[n].m_Type == ITEMGET(12, 30))
			{
				if (lpBuyerObj->pInventory[n].m_Level == 1)
				{
					if (s20BJNeedCount > 0)
					{
						gObjInventoryItemSet(lpBuyerObj->m_Index, n, -1);
						lpBuyerObj->pInventory[n].Clear();
						GSProtocol.GCInventoryItemDeleteSend(lpBuyerObj->m_Index, n, TRUE);
						s20BJNeedCount--;
					}
				}
			}

			if (lpBuyerObj->pInventory[n].m_Type == ITEMGET(12, 31))
			{
				if (lpBuyerObj->pInventory[n].m_Level == 1)
				{
					if (s20SJNeedCount > 0)
					{
						gObjInventoryItemSet(lpBuyerObj->m_Index, n, -1);
						lpBuyerObj->pInventory[n].Clear();
						GSProtocol.GCInventoryItemDeleteSend(lpBuyerObj->m_Index, n, TRUE);
						s20SJNeedCount--;
					}
				}
			}

			if (lpBuyerObj->pInventory[n].m_Type == ITEMGET(12, 141))
			{
				if (lpBuyerObj->pInventory[n].m_Level == 1)
				{
					if (s20CJNeedCount > 0)
					{
						gObjInventoryItemSet(lpBuyerObj->m_Index, n, -1);
						lpBuyerObj->pInventory[n].Clear();
						GSProtocol.GCInventoryItemDeleteSend(lpBuyerObj->m_Index, n, TRUE);
						s20CJNeedCount--;
					}
				}
			}
		}
	}

	for (int n = 0; n < 204; n++)
	{
		if (lpBuyerObj->pInventory[n].IsItem() == TRUE)
		{
			if (lpBuyerObj->pInventory[n].m_Type == ITEMGET(12, 30))
			{
				if (lpBuyerObj->pInventory[n].m_Level == 0)
				{
					if (s10BJNeedCount > 0)
					{
						gObjInventoryItemSet(lpBuyerObj->m_Index, n, -1);
						lpBuyerObj->pInventory[n].Clear();
						GSProtocol.GCInventoryItemDeleteSend(lpBuyerObj->m_Index, n, TRUE);
						s10BJNeedCount--;
					}
				}
			}

			if (lpBuyerObj->pInventory[n].m_Type == ITEMGET(12, 31))
			{
				if (lpBuyerObj->pInventory[n].m_Level == 0)
				{
					if (s10SJNeedCount > 0)
					{
						gObjInventoryItemSet(lpBuyerObj->m_Index, n, -1);
						lpBuyerObj->pInventory[n].Clear();
						GSProtocol.GCInventoryItemDeleteSend(lpBuyerObj->m_Index, n, TRUE);
						s10SJNeedCount--;
					}
				}
			}

			if (lpBuyerObj->pInventory[n].m_Type == ITEMGET(12, 141))
			{
				if (lpBuyerObj->pInventory[n].m_Level == 0)
				{
					if (s10CJNeedCount > 0)
					{
						gObjInventoryItemSet(lpBuyerObj->m_Index, n, -1);
						lpBuyerObj->pInventory[n].Clear();
						GSProtocol.GCInventoryItemDeleteSend(lpBuyerObj->m_Index, n, TRUE);
						s10CJNeedCount--;
					}
				}
			}
		}
	}

	for (int n = 0; n < 204; n++)
	{
		if (lpBuyerObj->pInventory[n].IsItem() == TRUE)
		{
			if (lpBuyerObj->pInventory[n].m_Type == ITEMGET(14, 13))
			{
				if (sBJNeedCount > 0)
				{
					gObjInventoryItemSet(lpBuyerObj->m_Index, n, -1);
					lpBuyerObj->pInventory[n].Clear();
					GSProtocol.GCInventoryItemDeleteSend(lpBuyerObj->m_Index, n, TRUE);
					sBJNeedCount--;
				}
			}

			if (lpBuyerObj->pInventory[n].m_Type == ITEMGET(14, 14))
			{
				if (sSJNeedCount > 0)
				{
					gObjInventoryItemSet(lpBuyerObj->m_Index, n, -1);
					lpBuyerObj->pInventory[n].Clear();
					GSProtocol.GCInventoryItemDeleteSend(lpBuyerObj->m_Index, n, TRUE);
					sSJNeedCount--;
				}
			}

			if (lpBuyerObj->pInventory[n].m_Type == ITEMGET(12, 15))
			{
				if (sCJNeedCount > 0)
				{
					gObjInventoryItemSet(lpBuyerObj->m_Index, n, -1);
					lpBuyerObj->pInventory[n].Clear();
					GSProtocol.GCInventoryItemDeleteSend(lpBuyerObj->m_Index, n, TRUE);
					sCJNeedCount--;
				}
			}
		}
	}

	int iBJCommisionCount = round(sBlessJewelPrice * 0.03);
	int iSJCommisionCount = round(sSoulJewelPrice * 0.03);
	int iCJCommisionCount = round(sChaosJewelPrice * 0.03);

	if (iBJCommisionCount > 50)
		iBJCommisionCount = 50;

	if (iSJCommisionCount > 50)
		iSJCommisionCount = 50;

	if (iCJCommisionCount > 50)
		iCJCommisionCount = 50;

	short s30BlessCount = (sBlessJewelPrice - iBJCommisionCount) / 30;
	short s20BlessCount = (sBlessJewelPrice - iBJCommisionCount) % 30 / 20;
	short s10BlessCount = (sBlessJewelPrice - iBJCommisionCount) % 30 % 20 / 10;
	short sBlessCount = (sBlessJewelPrice - iBJCommisionCount) % 30 % 20 % 10;

	short s30SoulCount = (sSoulJewelPrice - iSJCommisionCount) / 30;
	short s20SoulCount = (sSoulJewelPrice - iSJCommisionCount) % 30 / 20;
	short s10SoulCount = (sSoulJewelPrice - iSJCommisionCount) % 30 % 20 / 10;
	short sSoulCount = (sSoulJewelPrice - iSJCommisionCount) % 30 % 20 % 10;

	short s30ChaosCount = (sChaosJewelPrice - iCJCommisionCount) / 30;
	short s20ChaosCount = (sChaosJewelPrice - iCJCommisionCount) % 30 / 20;
	short s10ChaosCount = (sChaosJewelPrice - iCJCommisionCount) % 30 % 20 / 10;
	short sChaosCount = (sChaosJewelPrice - iCJCommisionCount) % 30 % 20 % 10;

	if (s30BlessCount > 0)
	{
		for (int i30B = 0; i30B < s30BlessCount; i30B++)
		{
			ItemSerialCreateSend(lpSellerObj->m_Index, 235, lpSellerObj->X, lpSellerObj->Y, ITEMGET(12, 30), 2, 0, 0, 0, 0, lpSellerObj->m_Index, 0, 0, 0, 0, 0);

			g_Log.Add("[PShop][Create Jewel Put Seller Inven] - User(ID:%s,Name:%s) Item(Name:%s,Code:%d,level:%d)",
				lpSellerObj->AccountID, lpSellerObj->Name, ItemAttribute[ITEMGET(12, 30)].Name, ITEMGET(12, 30), 2);
		}
	}

	if (s20BlessCount > 0)
	{
		for (int i20B = 0; i20B < s20BlessCount; i20B++)
		{
			ItemSerialCreateSend(lpSellerObj->m_Index, 235, lpSellerObj->X, lpSellerObj->Y, ITEMGET(12, 30), 1, 0, 0, 0, 0, lpSellerObj->m_Index, 0, 0, 0, 0, 0);

			g_Log.Add("[PShop][Create Jewel Put Seller Inven] - User(ID:%s,Name:%s) Item(Name:%s,Code:%d,level:%d)",
				lpSellerObj->AccountID, lpSellerObj->Name, ItemAttribute[ITEMGET(12, 30)].Name, ITEMGET(12, 30), 1);
		}
	}

	if (s10BlessCount > 0)
	{
		for (int i10B = 0; i10B < s10BlessCount; i10B++)
		{
			ItemSerialCreateSend(lpSellerObj->m_Index, 235, lpSellerObj->X, lpSellerObj->Y, ITEMGET(12, 30), 0, 0, 0, 0, 0, lpSellerObj->m_Index, 0, 0, 0, 0, 0);

			g_Log.Add("[PShop][Create Jewel Put Seller Inven] - User(ID:%s,Name:%s) Item(Name:%s,Code:%d,level:%d)",
				lpSellerObj->AccountID, lpSellerObj->Name, ItemAttribute[ITEMGET(12, 30)].Name, ITEMGET(12, 30), 0);
		}
	}

	if (sBlessCount > 0)
	{
		for (int iB = 0; iB < sBlessCount; iB++)
		{
			ItemSerialCreateSend(lpSellerObj->m_Index, 235, lpSellerObj->X, lpSellerObj->Y, ITEMGET(14, 13), 0, 0, 0, 0, 0, lpSellerObj->m_Index, 0, 0, 0, 0, 0);

			g_Log.Add("[PShop][Create Jewel Put Seller Inven] - User(ID:%s,Name:%s) Item(Name:%s,Code:%d,level:%d)",
				lpSellerObj->AccountID, lpSellerObj->Name, ItemAttribute[ITEMGET(14, 13)].Name, ITEMGET(14, 13), 0);
		}
	}

	if (s30SoulCount > 0)
	{
		for (int i30S = 0; i30S < s30SoulCount; i30S++)
		{
			ItemSerialCreateSend(lpSellerObj->m_Index, 235, lpSellerObj->X, lpSellerObj->Y, ITEMGET(12, 31), 2, 0, 0, 0, 0, lpSellerObj->m_Index, 0, 0, 0, 0, 0);

			g_Log.Add("[PShop][Create Jewel Put Seller Inven] - User(ID:%s,Name:%s) Item(Name:%s,Code:%d,level:%d)",
				lpSellerObj->AccountID, lpSellerObj->Name, ItemAttribute[ITEMGET(12, 31)].Name, ITEMGET(12, 31), 2);
		}
	}

	if (s20SoulCount > 0)
	{
		for (int i20S = 0; i20S < s20SoulCount; i20S++)
		{
			ItemSerialCreateSend(lpSellerObj->m_Index, 235, lpSellerObj->X, lpSellerObj->Y, ITEMGET(12, 31), 1, 0, 0, 0, 0, lpSellerObj->m_Index, 0, 0, 0, 0, 0);

			g_Log.Add("[PShop][Create Jewel Put Seller Inven] - User(ID:%s,Name:%s) Item(Name:%s,Code:%d,level:%d)",
				lpSellerObj->AccountID, lpSellerObj->Name, ItemAttribute[ITEMGET(12, 31)].Name, ITEMGET(12, 31), 1);
		}
	}

	if (s10SoulCount > 0)
	{
		for (int i10S = 0; i10S < s10SoulCount; i10S++)
		{
			ItemSerialCreateSend(lpSellerObj->m_Index, 235, lpSellerObj->X, lpSellerObj->Y, ITEMGET(12, 31), 0, 0, 0, 0, 0, lpSellerObj->m_Index, 0, 0, 0, 0, 0);

			g_Log.Add("[PShop][Create Jewel Put Seller Inven] - User(ID:%s,Name:%s) Item(Name:%s,Code:%d,level:%d)",
				lpSellerObj->AccountID, lpSellerObj->Name, ItemAttribute[ITEMGET(12, 31)].Name, ITEMGET(12, 31), 0);
		}
	}

	if (sSoulCount > 0)
	{
		for (int iS = 0; iS < sSoulCount; iS++)
		{
			ItemSerialCreateSend(lpSellerObj->m_Index, 235, lpSellerObj->X, lpSellerObj->Y, ITEMGET(14, 14), 0, 0, 0, 0, 0, lpSellerObj->m_Index, 0, 0, 0, 0, 0);

			g_Log.Add("[PShop][Create Jewel Put Seller Inven] - User(ID:%s,Name:%s) Item(Name:%s,Code:%d,level:%d)",
				lpSellerObj->AccountID, lpSellerObj->Name, ItemAttribute[ITEMGET(14, 14)].Name, ITEMGET(14, 14), 0);
		}
	}

	if (s30ChaosCount > 0)
	{
		for (int i30C = 0; i30C < s30ChaosCount; i30C++)
		{
			ItemSerialCreateSend(lpSellerObj->m_Index, 235, lpSellerObj->X, lpSellerObj->Y, ITEMGET(12, 141), 2, 0, 0, 0, 0, lpSellerObj->m_Index, 0, 0, 0, 0, 0);

			g_Log.Add("[PShop][Create Jewel Put Seller Inven] - User(ID:%s,Name:%s) Item(Name:%s,Code:%d,level:%d)",
				lpSellerObj->AccountID, lpSellerObj->Name, ItemAttribute[ITEMGET(12, 141)].Name, ITEMGET(12, 141), 2);
		}
	}

	if (s20ChaosCount > 0)
	{
		for (int i20C = 0; i20C < s20ChaosCount; i20C++)
		{
			ItemSerialCreateSend(lpSellerObj->m_Index, 235, lpSellerObj->X, lpSellerObj->Y, ITEMGET(12, 141), 1, 0, 0, 0, 0, lpSellerObj->m_Index, 0, 0, 0, 0, 0);

			g_Log.Add("[PShop][Create Jewel Put Seller Inven] - User(ID:%s,Name:%s) Item(Name:%s,Code:%d,level:%d)",
				lpSellerObj->AccountID, lpSellerObj->Name, ItemAttribute[ITEMGET(12, 141)].Name, ITEMGET(12, 141), 1);
		}
	}

	if (s10ChaosCount > 0)
	{
		for (int i10C = 0; i10C < s10ChaosCount; i10C++)
		{
			ItemSerialCreateSend(lpSellerObj->m_Index, 235, lpSellerObj->X, lpSellerObj->Y, ITEMGET(12, 141), 0, 0, 0, 0, 0, lpSellerObj->m_Index, 0, 0, 0, 0, 0);

			g_Log.Add("[PShop][Create Jewel Put Seller Inven] - User(ID:%s,Name:%s) Item(Name:%s,Code:%d,level:%d)",
				lpSellerObj->AccountID, lpSellerObj->Name, ItemAttribute[ITEMGET(12, 141)].Name, ITEMGET(12, 141), 0);
		}
	}

	if (sChaosCount > 0)
	{
		for (int iC = 0; iC < sChaosCount; iC++)
		{
			ItemSerialCreateSend(lpSellerObj->m_Index, 235, lpSellerObj->X, lpSellerObj->Y, ITEMGET(12, 15), 0, 0, 0, 0, 0, lpSellerObj->m_Index, 0, 0, 0, 0, 0);

			g_Log.Add("[PShop][Create Jewel Put Seller Inven] - User(ID:%s,Name:%s) Item(Name:%s,Code:%d,level:%d)",
				lpSellerObj->AccountID, lpSellerObj->Name, ItemAttribute[ITEMGET(12, 15)].Name, ITEMGET(12, 15), 0);
		}
	}

	return FALSE;
}

int CheckInventoryEmptySpaceCount(LPOBJ lpObj, int iItemHeight, int iItemWidth)
{
	int nCount = 0;

	for (int h = 0; h < MAX_INVENTORY_H; h++)
	{
		for (int w = 0; w < 8; w++)
		{
			if (*(BYTE*)(lpObj->pInventoryMap + h * 8 + w) == 255)
			{
				BYTE blank = gObjOnlyInventoryRectCheck(lpObj->m_Index, w, h, iItemWidth, iItemHeight);

				if (blank != 255 && blank != 254)
				{
					nCount++;
				}
			}
		}
	}

	return nCount;
}

BOOL SummonGoldColossusMonster(LPOBJ lpObj, int monsterIndex, int nCount, int bIsHelpMon)
{
	if (nCount < 1)
	{
		nCount = 1;
	}

	if ((MapC[lpObj->MapNumber].GetAttr(lpObj->X, lpObj->Y) & 1) == 1)
	{
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,566), lpObj->m_Index, TRUE);
		return FALSE;
	}

	if (BC_MAP_RANGE(lpObj->MapNumber))
	{
		return FALSE;
	}

	if (CC_MAP_RANGE(lpObj->MapNumber))
	{
		return FALSE;
	}

	if (DS_MAP_RANGE(lpObj->MapNumber))
	{
		return FALSE;
	}

	if (DG_MAP_RANGE(lpObj->MapNumber))
	{
		return FALSE;
	}

	if (IMPERIAL_MAP_RANGE(lpObj->MapNumber))
	{
		return FALSE;
	}

	if (KALIMA_MAP_RANGE(lpObj->MapNumber))
	{
		return FALSE;
	}

	if (lpObj->MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL)
	{
		return FALSE;
	}

	if (lpObj->MapNumber == MAP_INDEX_HATCHERY)
	{
		return FALSE;
	}

	if (lpObj->MapNumber == MAP_INDEX_ICARUS)
	{
		return FALSE;
	}

	if (lpObj->MapNumber == MAP_INDEX_VULCANROOM)
	{
		return FALSE;
	}

	if (lpObj->MapNumber == MAP_INDEX_ARCA_WAR || lpObj->MapNumber == MAP_INDEX_DEBENTER_ARCA_WAR)
	{
		return FALSE;
	}

	if (lpObj->MapNumber == MAP_INDEX_BARRACKS || lpObj->MapNumber == MAP_INDEX_REFUGEE)
	{
		return FALSE;
	}

	if (lpObj->MapNumber == MAP_INDEX_VULCAN)
	{
		return FALSE;
	}

	if (ITL_MAP_RANGE(lpObj->MapNumber))
	{
		return FALSE;
	}

	LPMONSTER_ATTRIBUTE lpMonsterAttr = 0;

	if (monsterIndex)
	{
		lpMonsterAttr = gMAttr.GetAttr(monsterIndex);
	}

	if (!lpMonsterAttr)
	{
		return FALSE;
	}

	for (int i = 0; i < nCount; i++)
	{
		int nResult = gObjAddMonster(lpObj->MapNumber);

		if (nResult < 0)
		{
			return FALSE;
		}

		gObj[nResult].m_Disappear_Monster = GetTickCount();
		gObjSetMonster(nResult, lpMonsterAttr->m_Index);
		gObj[nResult].DieRegen = 0;
		gObj[nResult].m_bGMSummon = 1;
		gObj[nResult].m_PosNum = -1;
		gObj[nResult].X = lpObj->X;
		gObj[nResult].Y = lpObj->Y;
		gObj[nResult].MapNumber = lpObj->MapNumber;
		gObj[nResult].TX = gObj[nResult].X;
		gObj[nResult].TY = gObj[nResult].Y;
		gObj[nResult].m_OldX = gObj[nResult].X;
		gObj[nResult].m_OldY = gObj[nResult].Y;
		gObj[nResult].StartX = LOBYTE(gObj[nResult].X);
		gObj[nResult].StartY = LOBYTE(gObj[nResult].Y);
		gObj[nResult].Level = LOWORD(lpMonsterAttr->m_Level);
		gObj[nResult].m_Attribute = 60;
		gObj[nResult].MaxRegenTime = 5000;
		gObj[nResult].Dir = rand() % 8;
		gObj[nResult].m_SummonCharDBNum = lpObj->DBNumber;

		if (bIsHelpMon == 1)
			gObj[nResult].m_bIsHelpMon = 1;

		g_Log.Add("[ColossusEvent][SummonMonster][%s][%s] MonsterClass : [%d], MonsterIndex : [%d], Map[%d], X[%d], Y[%d]",
			lpObj->AccountID, lpObj->Name, monsterIndex, nResult, gObj[nResult].MapNumber, gObj[nResult].X, gObj[nResult].Y);
		
	return TRUE;
	}
}

BOOL SummonPetEventMonster(LPOBJ lpObj)
{
	if ((MapC[lpObj->MapNumber].GetAttr(lpObj->X, lpObj->Y) & 1) == 1)
	{
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,566), lpObj->m_Index, TRUE);
		return FALSE;
	}

	if (BC_MAP_RANGE(lpObj->MapNumber))
	{
		return FALSE;
	}

	if (CC_MAP_RANGE(lpObj->MapNumber))
	{
		return FALSE;
	}

	if (DS_MAP_RANGE(lpObj->MapNumber))
	{
		return FALSE;
	}

	if (DG_MAP_RANGE(lpObj->MapNumber))
	{
		return FALSE;
	}

	if (IMPERIAL_MAP_RANGE(lpObj->MapNumber))
	{
		return FALSE;
	}

	if (KALIMA_MAP_RANGE(lpObj->MapNumber))
	{
		return FALSE;
	}

	if (lpObj->MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL)
	{
		return FALSE;
	}

	if (lpObj->MapNumber == MAP_INDEX_HATCHERY)
	{
		return FALSE;
	}

	if (lpObj->MapNumber == MAP_INDEX_ICARUS)
	{
		return FALSE;
	}

	if (lpObj->MapNumber == MAP_INDEX_VULCANROOM)
	{
		return FALSE;
	}

	if (lpObj->MapNumber == MAP_INDEX_ARCA_WAR || lpObj->MapNumber == MAP_INDEX_DEBENTER_ARCA_WAR)
	{
		return FALSE;
	}

	if (lpObj->MapNumber == MAP_INDEX_BARRACKS || lpObj->MapNumber == MAP_INDEX_REFUGEE)
	{
		return FALSE;
	}

	if (lpObj->MapNumber == MAP_INDEX_VULCAN)
	{
		return FALSE;
	}

	if (ITL_MAP_RANGE(lpObj->MapNumber))
	{
		return FALSE;
	}

	int nMonsterRate = GetLargeRand() % 10000;
	int monsterIndex;

	if (nMonsterRate >= g_ConfigRead.data.common.PetEventBookMoonRabbitSpawnRate)
	{
		if (nMonsterRate >= g_ConfigRead.data.common.PetEventBookLuckyBagSpawnRate + g_ConfigRead.data.common.PetEventBookMoonRabbitSpawnRate)
		{
			if (nMonsterRate >= g_ConfigRead.data.common.PetEventBookFireFlameSpawnRate + g_ConfigRead.data.common.PetEventBookLuckyBagSpawnRate + g_ConfigRead.data.common.PetEventBookMoonRabbitSpawnRate)
			{
				if (nMonsterRate >= g_ConfigRead.data.common.PetEventBookGoldGoblinSpawnRate + g_ConfigRead.data.common.PetEventBookFireFlameSpawnRate + g_ConfigRead.data.common.PetEventBookLuckyBagSpawnRate + g_ConfigRead.data.common.PetEventBookMoonRabbitSpawnRate)
				{
					return 0;
				}

				monsterIndex = 677;
			}

			else
			{
				monsterIndex = 676;
			}
		}

		else
		{
			monsterIndex = 675;
		}
	}

	else
	{
		monsterIndex = 674;
	}

	LPMONSTER_ATTRIBUTE lpMonsterAttr = 0;

	if (monsterIndex)
	{
		lpMonsterAttr = gMAttr.GetAttr(monsterIndex);
	}

	if (!lpMonsterAttr)
	{
		return FALSE;
	}

	int nResult = gObjAddMonster(lpObj->MapNumber);

	if (nResult < 0)
	{
		return FALSE;
	}

	gObj[nResult].m_Disappear_Monster = GetTickCount();
	gObjSetMonster(nResult, lpMonsterAttr->m_Index);
	gObj[nResult].DieRegen = 0;
	gObj[nResult].m_bGMSummon = 1;
	gObj[nResult].m_PosNum = -1;
	gObj[nResult].X = lpObj->X;
	gObj[nResult].Y = lpObj->Y;
	gObj[nResult].MapNumber = lpObj->MapNumber;
	gObj[nResult].TX = gObj[nResult].X;
	gObj[nResult].TY = gObj[nResult].Y;
	gObj[nResult].m_OldX = gObj[nResult].X;
	gObj[nResult].m_OldY = gObj[nResult].Y;
	gObj[nResult].StartX = LOBYTE(gObj[nResult].X);
	gObj[nResult].StartY = LOBYTE(gObj[nResult].Y);
	gObj[nResult].Level = LOWORD(lpMonsterAttr->m_Level);
	gObj[nResult].m_Attribute = 60;
	gObj[nResult].MaxRegenTime = 5000;
	gObj[nResult].Dir = rand() % 8;

	g_Log.Add("[PetEvent][SummonMonster][%s][%s] MonsterClass : [%d], MonsterIndex : [%d], Map[%d], X[%d], Y[%d]",
		lpObj->AccountID, lpObj->Name, monsterIndex, nResult, gObj[nResult].MapNumber, gObj[nResult].X, gObj[nResult].Y);

	return TRUE;
}

BOOL NewYearSummonMonster(LPOBJ lpObj, int monsterIndex)
{
	if ((MapC[lpObj->MapNumber].GetAttr(lpObj->X, lpObj->Y) & 1) == 1)
	{
		GSProtocol.GCServerMsgStringSend(Lang.GetText(0,566), lpObj->m_Index, TRUE);
		return FALSE;
	}

	if (BC_MAP_RANGE(lpObj->MapNumber))
	{
		return FALSE;
	}

	if (CC_MAP_RANGE(lpObj->MapNumber))
	{
		return FALSE;
	}

	if (DS_MAP_RANGE(lpObj->MapNumber))
	{
		return FALSE;
	}

	if (DG_MAP_RANGE(lpObj->MapNumber))
	{
		return FALSE;
	}

	if (IMPERIAL_MAP_RANGE(lpObj->MapNumber))
	{
		return FALSE;
	}

	if (KALIMA_MAP_RANGE(lpObj->MapNumber))
	{
		return FALSE;
	}

	if (lpObj->MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL)
	{
		return FALSE;
	}

	if (lpObj->MapNumber == MAP_INDEX_HATCHERY)
	{
		return FALSE;
	}

	if (lpObj->MapNumber == MAP_INDEX_ICARUS)
	{
		return FALSE;
	}

	if (lpObj->MapNumber == MAP_INDEX_VULCANROOM)
	{
		return FALSE;
	}

	if (lpObj->MapNumber == MAP_INDEX_ARCA_WAR || lpObj->MapNumber == MAP_INDEX_DEBENTER_ARCA_WAR)
	{
		return FALSE;
	}

	if (lpObj->MapNumber == MAP_INDEX_BARRACKS || lpObj->MapNumber == MAP_INDEX_REFUGEE)
	{
		return FALSE;
	}

	if (lpObj->MapNumber == MAP_INDEX_VULCAN)
	{
		return FALSE;
	}

	if (ITL_MAP_RANGE(lpObj->MapNumber))
	{
		return FALSE;
	}

	LPMONSTER_ATTRIBUTE lpMonsterAttr = 0;

	if (monsterIndex)
	{
		lpMonsterAttr = gMAttr.GetAttr(monsterIndex);
	}

	if (!lpMonsterAttr)
	{
		return FALSE;
	}

	int nResult = gObjAddMonster(lpObj->MapNumber);

	if (nResult < 0)
	{
		return FALSE;
	}

	gObj[nResult].m_Disappear_NewYearMonster = GetTickCount();
	gObjSetMonster(nResult, lpMonsterAttr->m_Index);
	gObj[nResult].DieRegen = 0;
	gObj[nResult].m_bGMSummon = 1;
	gObj[nResult].m_PosNum = -1;
	gObj[nResult].X = lpObj->X;
	gObj[nResult].Y = lpObj->Y;
	gObj[nResult].MapNumber = lpObj->MapNumber;
	gObj[nResult].TX = gObj[nResult].X;
	gObj[nResult].TY = gObj[nResult].Y;
	gObj[nResult].m_OldX = gObj[nResult].X;
	gObj[nResult].m_OldY = gObj[nResult].Y;
	gObj[nResult].StartX = LOBYTE(gObj[nResult].X);
	gObj[nResult].StartY = LOBYTE(gObj[nResult].Y);
	gObj[nResult].Level = LOWORD(lpMonsterAttr->m_Level);
	gObj[nResult].m_Attribute = 60;
	gObj[nResult].MaxRegenTime = 5000;
	gObj[nResult].Dir = rand() % 8;

	return TRUE;
}

int gObjGetAutoPartyUserCount()
{
	int counter = 0;
	for(int i=g_ConfigRead.server.GetObjectStartUserIndex();i<g_ConfigRead.server.GetObjectMax();i++)
	{
		if(gObj[i].Connected == PLAYER_PLAYING && gObj[i].Type == OBJ_USER)
		{
			if(gObj[i].m_PlayerData->bActiveSetParty)
				counter++;
		}
	}
	return counter;
}

int gObjGetOffTradeUsercount()
{
	int counter = 0;
	for(int i=g_ConfigRead.server.GetObjectStartUserIndex();i<g_ConfigRead.server.GetObjectMax();i++)
	{
		if(gObj[i].Connected == PLAYER_PLAYING && gObj[i].Type == OBJ_USER)
		{
			if(gObj[i].m_bOff && !gObj[i].m_bOffLevel)
				counter++;
		}
	}
	return counter;
}

void gObjDisconnectOffTraders()
{
	for ( int n= g_ConfigRead.server.GetObjectStartUserIndex(); n< g_ConfigRead.server.GetObjectMax(); n++)
	{
		if ( gObj[n].Connected > PLAYER_EMPTY )
		{
			if ( gObj[n].Type == OBJ_USER && gObj[n].m_bOff && !gObj[n].m_bOffLevel)
			{
					GJSetCharacterInfo(&gObj[n], n, FALSE);
					gObj[n].m_bOff = false;
					gObjDel(n);
					IOCP.CloseClient(n);
			}
		}
	}
}

void gObjDisconnectOffLevelers()
{
	for ( int n= g_ConfigRead.server.GetObjectStartUserIndex(); n< g_ConfigRead.server.GetObjectMax(); n++)
	{
		if ( gObj[n].Connected > PLAYER_EMPTY )
		{
			if ( gObj[n].Type == OBJ_USER && gObj[n].m_bOff && gObj[n].m_bOffLevel)
			{
					GJSetCharacterInfo(&gObj[n], n, FALSE);
					gObj[n].m_bOff = false;
					gObjDel(n);
					IOCP.CloseClient(n);
			}
		}
	}
}
void CNameConvert::InitData()
{
	for (int i = 0; i < 5; i++)
	{
		this->m_perCharacterInfo[i].Clear();
	}
}

void CNameConvert::InputData(int cnt, const char *name, const char *realNameOfUbf, WORD serverCode, int Length)
{
	if ( cnt > -1 && cnt < 5 )
	{
		memcpy(this->m_perCharacterInfo[cnt].szCharacterName, name, Length);
		this->m_perCharacterInfo[cnt].szCharacterName[Length] = 0;
		memcpy(this->m_perCharacterInfo[cnt].szCharacterNameOfUBF, realNameOfUbf, Length);
		this->m_perCharacterInfo[cnt].szCharacterNameOfUBF[Length] = 0;
		this->m_perCharacterInfo[cnt].nServerCodeOfHomeWorld = serverCode;
	}
}

char * CNameConvert::ConvertNameToRealName(const char *name)
{
	for (int i = 0; i < 5; i++)
	{
		if (!strcmp(this->m_perCharacterInfo[i].szCharacterName, name))
		{
			return this->m_perCharacterInfo[i].szCharacterNameOfUBF;
		}
	}

	return this->m_perCharacterInfo[0].szCharacterNameOfUBF;
}

char * CNameConvert::ConvertRealNameToName(const char *realName)
{
	for (int i = 0; i < 5; i++)
	{
		if (!strcmp(this->m_perCharacterInfo[i].szCharacterNameOfUBF, realName))
		{
			return this->m_perCharacterInfo[i].szCharacterName;
		}
	}

	return this->m_perCharacterInfo[0].szCharacterName;
}

WORD CNameConvert::FindServerCodeAtRealName(const char *realName)
{
	for (int i = 0; i < 5; i++)
	{
		if (!strcmp(this->m_perCharacterInfo[i].szCharacterNameOfUBF, realName))
		{
			return this->m_perCharacterInfo[i].nServerCodeOfHomeWorld;
		}
	}

	return 0;
}

WORD CNameConvert::FindServerCodeAtCharacterName(const char *name)
{
	for (int i = 0; i < 5; i++)
	{
		if (!strcmp(this->m_perCharacterInfo[i].szCharacterName, name))
		{
			return this->m_perCharacterInfo[i].nServerCodeOfHomeWorld;
		}
	}

	return 0;
}

char * CNameConvert::ConvertSlotIndexToName(int slotindex)
{
	int SlotNumber = slotindex;

	if ( slotindex > 2 && slotindex > -1 )
	{
		SlotNumber = 0;
		g_Log.Add("[UBF][CNAMECONVERT][ConvertSlotIndexToName] Map Join Request Fail, Wrong SlotIndex :%d. Default (ChName,RealName): (%s,%s)",
			slotindex, this->m_perCharacterInfo[0].szCharacterName, this->m_perCharacterInfo[0].szCharacterNameOfUBF);
	}

	return this->m_perCharacterInfo[SlotNumber].szCharacterName;
}

int CNameConvert::IsDuplicationOfName()
{
	for ( int pivotIndex = 0; pivotIndex < 5; pivotIndex++ )
	{
		for ( int searchIndex = 0; searchIndex < 5; searchIndex++ )
		{
			if ( pivotIndex != searchIndex )
			{
				if ( !strcmp(this->m_perCharacterInfo[pivotIndex].szCharacterNameOfUBF, this->m_perCharacterInfo[searchIndex].szCharacterNameOfUBF))
				{
					return TRUE;
				}
			}
		}
	}

	return FALSE;
}

void gObjInvenPetDamage(LPOBJ lpObj, int damage)
{
	if (lpObj->Type != OBJ_USER)
	{
		return;
	}

	BOOL send_dur = FALSE;

	if (lpObj->m_btInvenPetPos == 0)
	{
		return;
	}

	if (lpObj->m_wInvenPet == (WORD)-1)
	{
		return;
	}

	if (lpObj->pInventory[lpObj->m_btInvenPetPos].IsItem() == FALSE)
	{
		return;
	}

	if (lpObj->pInventory[lpObj->m_btInvenPetPos].IsPeriodItem() == TRUE)
	{
		return;
	}

	float fN = 10.0f;
	CItem * sprite = &lpObj->pInventory[lpObj->m_btInvenPetPos];
	float fdamage = damage;

	if ( sprite->m_Type == ITEMGET(13,2) && g_ConfigRead.pet.DamageDisableForPet[DAMAGE_OFF_UNIRIA] == false)
	{
		if (lpObj->Class == CLASS_RAGEFIGHTER)
		{
			if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsDownDur3_Monk > 0.0)
			{
				fdamage = damage / lpObj->m_PlayerData->m_MPSkillOpt.iMpsDownDur3_Monk;
			}
		}

		else if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsDownDur3 > 0.0)
		{
			fdamage = damage / lpObj->m_PlayerData->m_MPSkillOpt.iMpsDownDur3;
		}

		fdamage = (fdamage)/10.0f;
		fdamage /= 10.0f;
		sprite->m_Durability -= fdamage;
		send_dur = 1;
	}

	else if ( sprite->m_Type == ITEMGET(13,3) && g_ConfigRead.pet.DamageDisableForPet[DAMAGE_OFF_DINORANT] == false)
	{
		if (lpObj->Class == CLASS_RAGEFIGHTER)
		{
			if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsDownDur3_Monk > 0.0)
			{
				fdamage = damage / lpObj->m_PlayerData->m_MPSkillOpt.iMpsDownDur3_Monk;
			}
		}

		else if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsDownDur3 > 0.0)
		{
			fdamage = damage / lpObj->m_PlayerData->m_MPSkillOpt.iMpsDownDur3;
		}

		fdamage = (fdamage)/10.0f;
		fdamage /= 10.0f;
		sprite->m_Durability -= fdamage;
		send_dur = 1;
	}

	else if ( sprite->m_Type == ITEMGET(13,4) && g_ConfigRead.pet.DamageDisableForPet[DAMAGE_OFF_DLHORSE] == false)
	{
		if ( sprite->m_Durability < 1.0f )
		{
			return;
		}

		fdamage = (damage*2)/10.0f;
		fdamage /= 10.0f;
		fdamage += 1.0f;
		sprite->m_DurabilitySmall += (int)fdamage;
		int DurabilityVal = 1500 + lpObj->m_PlayerData->m_MPSkillOpt.iMpsPetDurDownSpeed;

		if ( sprite->m_DurabilitySmall > DurabilityVal )
		{
			sprite->m_Durability -= 1.0f;
			sprite->m_DurabilitySmall = 0;
			send_dur = 1;

			if ( sprite->m_Durability < 1.0f )
			{
				sprite->m_Durability = 0;

				UINT64 iPetExperience = lpObj->pInventory[1].m_PetItem_Exp;

				if ( sprite->DecPetItemExp(10) != FALSE )
				{
					g_Log.Add("[%s][%s][PetItemExpDown] [%s] Level:[%d]Exp:[%I64d]DecExp[%I64d]",
						lpObj->AccountID, lpObj->Name, lpObj->pInventory[lpObj->m_btInvenPetPos].GetName(),
						lpObj->pInventory[lpObj->m_btInvenPetPos].m_PetItem_Level, lpObj->pInventory[lpObj->m_btInvenPetPos].m_PetItem_Exp,
						iPetExperience - lpObj->pInventory[1].m_PetItem_Exp);
					CDarkSpirit::SendLevelmsg(lpObj->m_Index, lpObj->m_btInvenPetPos, 1, (BYTE)-1);
				}

				g_Log.Add("[%s][%s] Dark Lord Pet is Broken because durability is exhausted [%I64d]",
						lpObj->AccountID,lpObj->Name,lpObj->pInventory[lpObj->m_btInvenPetPos].m_Number);
			}

			GSProtocol.GCItemDurSend(lpObj->m_Index, lpObj->m_btInvenPetPos, sprite->m_Durability, 0);
		}
	
		return;
	}

	else if(sprite->m_Type == ITEMGET(13,37) && g_ConfigRead.pet.DamageDisableForPet[DAMAGE_OFF_FENRIR] == false)
	{
		if ( sprite->m_Durability < 1.0f )
		{
			return;
		}

		int iMaxDurSmall = g_iFenrirDefaultMaxDurSmall;
		int iMultipleDmg = 2;

		if(lpObj->Class == CLASS_WIZARD)
		{

		}

		else if(lpObj->Class == CLASS_KNIGHT)
		{

		}

		else if(lpObj->Class == CLASS_ELF)
		{
			iMaxDurSmall = g_iFenrirElfMaxDurSmall;
			iMultipleDmg = 2;
		}

		if (sprite->IsFenrirIllusion() == TRUE)
		{
			iMaxDurSmall = g_ConfigRead.pet.GoldFenrirDurabilityRate;
		}

		if (lpObj->Class == CLASS_RAGEFIGHTER)
		{
			if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsDownDur3_Monk > 0.0)
			{
				fdamage = damage / lpObj->m_PlayerData->m_MPSkillOpt.iMpsDownDur3_Monk;
			}
		}

		else if (lpObj->m_PlayerData->m_MPSkillOpt.iMpsDownDur3 > 0.0)
		{
			fdamage = damage / lpObj->m_PlayerData->m_MPSkillOpt.iMpsDownDur3;
		}

		fdamage = fdamage * iMultipleDmg / 10.0f;
		fdamage /= 10.0f;
		fdamage += 1.0f;

		sprite->m_DurabilitySmall += (int)fdamage;

		if(sprite->m_DurabilitySmall > iMaxDurSmall)
		{
			sprite->m_Durability -= 1.0f;
			sprite->m_DurabilitySmall = 0;

			if(sprite->m_Durability < 1.0f)
			{
				sprite->m_Durability = 0;
			}

			send_dur = 1;
		}
	}

	if ( sprite->m_Durability < 0.0f )
	{
		sprite->m_Durability = 0.0f; // fixed durability error (previously gs was sending negative values)
	}

	if ( send_dur == TRUE )
	{
		GSProtocol.GCItemDurSend(lpObj->m_Index, lpObj->m_btInvenPetPos, sprite->m_Durability, 0);

		if ( sprite->m_Durability < 1.0 )
		{
			int itemnum = sprite->m_Type;
			int iInvenPetPos = lpObj->m_btInvenPetPos;

			g_Log.Add("[%s][%s] %s+%d Item is broken because durability is exhausted", lpObj->AccountID, lpObj->Name,
				lpObj->pInventory[iInvenPetPos].GetName(), lpObj->pInventory[iInvenPetPos].m_Level);

			g_Log.Add("[CRenewal][InvenPet][ZeroDur][%s][%s] PetIndex:[%d], Pos:[%d] InvenPet Durability is Zero. => Remove Inven Pet",
				lpObj->AccountID, lpObj->Name, sprite->m_Type, iInvenPetPos);

			_tagPMSG_REQ_INVENTORY_EQUIPMENT_ITEM pMsg;
			pMsg.btItemPos = iInvenPetPos;
			pMsg.btValue = (BYTE)-1;

			GSProtocol.CGInventoryEquipment(&pMsg, lpObj->m_Index);
			gObjInventoryDeleteItem(lpObj->m_Index, iInvenPetPos);
			GSProtocol.GCInventoryItemDeleteSend(lpObj->m_Index, iInvenPetPos, 0);

			if ( lpObj->MapNumber == MAP_INDEX_ICARUS && itemnum == ITEMGET(13,3) || lpObj->MapNumber == MAP_INDEX_ICARUS && itemnum == ITEMGET(13,37))
			{
				int pos = -1;

				for ( int i=MAX_PLAYER_EQUIPMENT;i<MAIN_INVENTORY_SIZE;i++)
				{
					if ( lpObj->pInventory[i].IsItem() == TRUE )
					{
						if ( lpObj->pInventory[i].m_Type == ITEMGET(13,3) || lpObj->pInventory[i].m_Type == ITEMGET(13,37) )
						{
							if ( lpObj->pInventory[i].m_Durability > 0.0 )
							{
								pos = i;
								break;
							}
						}
					}
				}

				if ( pos == -1 )
				{
					if ( lpObj->pInventory[7].IsItem() == FALSE )
					{
						gObjMoveGate(lpObj->m_Index, 22);
					}
				}

				else
				{
					_tagPMSG_REQ_INVENTORY_EQUIPMENT_ITEM pMsg;
					pMsg.btItemPos = iInvenPetPos;
					pMsg.btValue = (BYTE)-2;

					GSProtocol.CGInventoryEquipment(&pMsg, lpObj->m_Index);
					GSProtocol.GCServerMsgStringSend(Lang.GetText(0,82), lpObj->m_Index, 1);
				}
			}
		}
	}
}

BOOL gObjUseInvenPetCheck(LPOBJ lpObj, CItem *lpItem, int flag)
{
	if (flag == 1)
	{
		if (!gObjInventorySearchSerialNumber(lpObj, lpItem->m_Number))
		{
			return FALSE;
		}

		if (!gObjWarehouseSearchSerialNumber(lpObj, lpItem->m_Number))
		{
			return FALSE;
		}

		if (lpItem->m_Durability <= 0.0)
		{
			return FALSE;
		}

		if (lpObj->m_Rest != 0)
		{
			return FALSE;
		}

		if (CC_MAP_RANGE(lpObj->MapNumber) == TRUE)
		{
			return FALSE;
		}

		if (lpObj->MapNumber == MAP_INDEX_CHAOSCASTLE_SURVIVAL)
		{
			return FALSE;
		}

		int useClass = lpItem->IsClass(lpObj->Class, lpObj->m_PlayerData->ChangeUP);

		if (useClass == FALSE)
		{
			return FALSE;
		}

		if (gObjCheckUsedBuffEffect(lpObj, BUFFTYPE_BLESS) == TRUE)
		{
			int nEffectValue = 0;
			gObjGetValueOfBuffIndex(lpObj, BUFFTYPE_BLESS, &nEffectValue, NULL);

			int nTmpLeadership = 0;

			if (lpObj->Class == CLASS_DARKLORD)
			{
				nTmpLeadership = lpObj->Leadership - nEffectValue;
			}

			if (lpObj->m_PlayerData->Strength - nEffectValue + lpObj->AddStrength < lpItem->m_RequireStrength - lpItem->m_HJOpStrength)
			{
				return FALSE;
			}

			if (lpObj->m_PlayerData->Dexterity - nEffectValue + lpObj->AddDexterity < lpItem->m_RequireDexterity - lpItem->m_HJOpDexterity)
			{
				return FALSE;
			}

			if (lpItem->m_Type >= ITEMGET(0,0) && lpItem->m_Type < ITEMGET(12,0))
			{
				if (lpObj->m_PlayerData->Vitality - nEffectValue + lpObj->AddVitality < lpItem->m_RequireVitality)
				{
					return FALSE;
				}

				if (lpObj->m_PlayerData->Energy - nEffectValue + lpObj->AddEnergy < lpItem->m_RequireEnergy)
				{
					return FALSE;
				}
			}

			if (nTmpLeadership + lpObj->AddLeadership < lpItem->m_RequireLeaderShip)
			{
				return FALSE;
			}
		}

		else
		{
			if (lpObj->m_PlayerData->Strength + lpObj->AddStrength < lpItem->m_RequireStrength - lpItem->m_HJOpStrength)
			{
				return FALSE;
			}

			if (lpObj->m_PlayerData->Dexterity + lpObj->AddDexterity < lpItem->m_RequireDexterity - lpItem->m_HJOpDexterity)
			{
				return FALSE;
			}

			if (lpItem->m_Type >= ITEMGET(0,0) && lpItem->m_Type < ITEMGET(12,0))
			{
				if (lpObj->m_PlayerData->Vitality + lpObj->AddVitality < lpItem->m_RequireVitality)
				{
					return FALSE;
				}

				if (lpObj->m_PlayerData->Energy + lpObj->AddEnergy < lpItem->m_RequireEnergy)
				{
					return FALSE;
				}
			}

			if (lpObj->Leadership + lpObj->AddLeadership < lpItem->m_RequireLeaderShip)
			{
				return FALSE;
			}
		}

		if (lpObj->Level < lpItem->m_RequireLevel)
		{
			return FALSE;
		}

		if (lpItem->m_Type >= ITEMGET(13,0) && lpItem->m_Type < ITEMGET(13,15))
		{
			if (lpItem->m_Type == ITEMGET(13,2))
			{
				if (lpObj->MapNumber == MAP_INDEX_ICARUS || lpObj->MapNumber == MAP_INDEX_ATHLANSE)
				{
					return FALSE;
				}
			}
				
			else if (lpItem->m_Type == ITEMGET(13,3))
			{
				if (lpObj->MapNumber == MAP_INDEX_ICARUS)
				{
					return FALSE;
				}
			}
		}

		return TRUE;
	}

	else
	{
		if (lpObj->MapNumber == MAP_INDEX_ICARUS)
		{
			if ((lpItem->m_Type == ITEMGET(13,3) || lpItem->m_Type == ITEMGET(13,37)) && lpItem->m_Durability == 0.0)
			{
				return TRUE;
			}

			if (lpItem->m_Type == ITEMGET(13,3))
			{
				if (lpObj->pInventory[7].IsItem() == FALSE)
				{
					return FALSE;
				}
			}

			else if (lpItem->m_Type == ITEMGET(13,37))
			{
				if (lpObj->pInventory[7].IsItem() == FALSE)
				{
					return FALSE;
				}
			}

			else if (lpItem->m_Type == ITEMGET(13,4))
			{
				if (lpObj->pInventory[7].IsItem() == FALSE)
				{
					return FALSE;
				}
			}
		}

		return TRUE;
	}
}

BOOL IsInvenPet(WORD sIndex)
{
	return sIndex >= ITEMGET(13,2) && (sIndex <= ITEMGET(13,4) || sIndex == ITEMGET(13,37));
}

BOOL IsInvenItem(WORD sIndex)
{
	switch ( sIndex )
	{
		case ITEMGET(13,128):
		case ITEMGET(13,129):
		case ITEMGET(13,130):
		case ITEMGET(13,131):
		case ITEMGET(13,132):
		case ITEMGET(13,133):
		case ITEMGET(13,134):
		case ITEMGET(13,177):
		case ITEMGET(13,178):
		case ITEMGET(13,179):
			return TRUE;
		default:
			return FALSE;
	}
}

int gObjCalDistanceTX(LPOBJ lpObj1, LPOBJ lpObj2)
{
	if ( lpObj1 == NULL || lpObj2 == NULL )
	{
		return 0;
	}

	if ( lpObj1->TX == lpObj2->TX && lpObj1->TY == lpObj2->TY )
	{
		return 0;
	}

	float tx = lpObj1->TX - lpObj2->TX;
	float ty = lpObj1->TY - lpObj2->TY;

	return sqrt( (tx*tx)+(ty*ty) );
}

void Check_SameSerialItem(int aIndex, BYTE bCase, bool & bCheckFail)
{
	if (!ObjectMaxRange(aIndex))
	{
		return;
	}
	
	LPOBJ lpObj = &gObj[aIndex];

	if (lpObj->Type != OBJ_USER)
	{
		return;
	}

	DWORD dwSameSerialItemCount = 0;
	UINT64 i64Serial = 0LL;

	std::set<UINT64> UserAllItemList;

	if (bCase == 1)
	{
		lpObj->m_PlayerData->dwOpenWareCnt++;
	}

	else if (bCase == 2)
	{
		lpObj->m_PlayerData->dwCloseWareCnt++;
	}

	DWORD dwInvenDeleteCnt = 0;
	DWORD dwWareDeleteCnt = 0;
	bool bNeedFullItemLog = false;

	if (lpObj->m_PlayerData->dwOpenWareCnt == 1 || lpObj->m_PlayerData->dwCloseWareCnt == 1)
	{
		bNeedFullItemLog = true;

		SerialCheck->Output("[SERIAL CHECK] Performing Serial Checking due to Open/Close WareHouse - ACC:[%s], NAME:[%s],Open:[%d], Close:[%d], case:[%d]",
			lpObj->AccountID, lpObj->Name, lpObj->m_PlayerData->dwOpenWareCnt, lpObj->m_PlayerData->dwCloseWareCnt, bCase);
	}

	if (bNeedFullItemLog == true)
	{
		DWORD dwAllWareCount = 0;
		DWORD dwNoneZeroItemCount = 0;

		for (int iPos = 0; iPos < WAREHOUSE_SIZE; iPos++)
		{
			if (lpObj->pWarehouse[iPos].IsItem() == false)
			{
				continue;
			}

			dwAllWareCount++;

			if (lpObj->pWarehouse[iPos].GetNumber())
			{
				SerialCheck->Output("[SERIAL CHECK] Warehouse Item Acc:[%s],Name:[%s],SR:[%I64d],POS:[%d],LVL:[%d],ItemName:[%s]",
					lpObj->AccountID, lpObj->Name, lpObj->pWarehouse[iPos].GetNumber(), iPos, lpObj->pWarehouse[iPos].m_Level, lpObj->pWarehouse[iPos].GetName());

				dwNoneZeroItemCount++;
			}
		}

		SerialCheck->Output("[SERIAL CHECK] Warehouse Item List Complete Acc:[%s],Name:[%s], CNT:[%d] NON-ZERO-CNT:[%d]",
			lpObj->AccountID, lpObj->Name, dwAllWareCount, dwNoneZeroItemCount);

		DWORD dwInvenCnt = 0;
		DWORD dwInvenNonZero = 0;

		for (int iPos = 0; iPos < INVENTORY_SIZE; iPos++)
		{
			if (lpObj->pInventory[iPos].IsItem() == false)
			{
				continue;
			}

			dwInvenCnt++;

			if (lpObj->pInventory[iPos].GetNumber())
			{
				SerialCheck->Output("[SERIAL CHECK] Inventory Item Acc:[%s],Name:[%s],SR:[%I64d],POS:[%d],LVL:[%d],ItemName:[%s]",
					lpObj->AccountID, lpObj->Name, lpObj->pInventory[iPos].GetNumber(), iPos, lpObj->pInventory[iPos].m_Level, lpObj->pInventory[iPos].GetName());

				dwInvenNonZero++;
			}
		}

		SerialCheck->Output("[SERIAL CHECK] Inventory Item List Complete Acc:[%s],Name:[%s], CNT:[%d] NON-ZERO-CNT:[%d]",
			lpObj->AccountID, lpObj->Name, dwInvenCnt, dwInvenNonZero);

		for (int iPos = 0; iPos < WAREHOUSE_SIZE; iPos++)
		{
			if (lpObj->pWarehouse[iPos].IsItem() == false)
			{
				continue;
			}

			UINT64 i64Serial = lpObj->pWarehouse[iPos].GetNumber();

			if (i64Serial)
			{
				std::pair<std::set<UINT64>::iterator, bool> ret = UserAllItemList.insert(i64Serial);

				if (ret.second == false)
				{
					SerialCheck->Output("[SERIAL CHECK] ~~~~~~ DUPLICATED ITEM FOUND [WAREHOUSE] - ITEM WILL BE DELETED ~~~~~~ ACC:[%s],Name:[%s],SERIAL:[%I64d],POS:[%d],LVL:[%d],ItemName:[%s]",
						lpObj->AccountID, lpObj->Name, i64Serial, iPos, lpObj->pWarehouse[iPos].m_Level, lpObj->pWarehouse[iPos].GetName());

					gObjWarehouseDeleteItem(lpObj->m_Index, iPos);
					dwWareDeleteCnt++;
					dwSameSerialItemCount++;
				}
			}
		}

		for (int iPos = 0; iPos < INVENTORY_SIZE; iPos++)
		{
			if (lpObj->pInventory[iPos].IsItem() == false)
			{
				continue;
			}

			UINT64 i64Serial = lpObj->pInventory[iPos].GetNumber();

			if (i64Serial)
			{
				std::pair<std::set<UINT64>::iterator, bool> ret = UserAllItemList.insert(i64Serial);

				if (ret.second == false)
				{
					SerialCheck->Output("[SERIAL CHECK] ~~~~~~ DUPLICATED ITEM FOUND [INVENTORY] - ITEM WILL BE DELETED ~~~~~~ ACC:[%s],Name:[%s],SERIAL:[%I64d],POS:[%d],LVL:[%d],ItemName:[%s]",
						lpObj->AccountID, lpObj->Name, i64Serial, iPos, lpObj->pInventory[iPos].m_Level, lpObj->pInventory[iPos].GetName());

					gObjInventoryDeleteItem(lpObj->m_Index, iPos);
					dwInvenDeleteCnt++;
					dwSameSerialItemCount++;
				}
			}
		}

		if (dwSameSerialItemCount)
		{
			lpObj->m_PlayerData->bHasSameSerialItem = true;
			bCheckFail = true;

			SerialCheck->Output("[SERIAL CHECK][RESULT] ~~~~~~~ FAILED ~~~~~~ ACC : [%s], CharName :[%s], AllCnt:[%d], Case : [%d] InvenCnt:[%d], WareCnt:[%d] , IP :[%s]",
				lpObj->AccountID, lpObj->Name, dwSameSerialItemCount, bCase, dwInvenDeleteCnt, dwWareDeleteCnt, lpObj->m_PlayerData->Ip_addr);

			SerialCheck->Output("[SERIAL CHECK] ACC:[%s],NAME:[%s],IP:[%s] - DISCONNECT",
				lpObj->AccountID, lpObj->Name, lpObj->m_PlayerData->Ip_addr);

			g_Log.Add("[SERIAL CHECK] ACC:[%s],NAME:[%s],IP:[%s] - DISCONNECT",
				lpObj->AccountID, lpObj->Name, lpObj->m_PlayerData->Ip_addr);

			GSProtocol.GCSendDisableReconnect(aIndex);
		}

		else
		{
			SerialCheck->Output("[SERIAL CHECK][RESULT] ~~~~~~~ SUCCESS ~~~~~~ ACC : [%s], CharName :[%s], Case : [%d], IP :[%s]",
				lpObj->AccountID, lpObj->Name, bCase, lpObj->m_PlayerData->Ip_addr);
		}
	}

	else if (bCase == 3)
	{
		SerialCheck->Output("[SERIAL CHECK] Performing Serial Checking due to JewelMixSystem - ACC:[%s], NAME:[%s], case:[%d]",
			lpObj->AccountID, lpObj->Name, bCase);

		DWORD dwInvenCnt = 0;
		DWORD dwInvenNonZero = 0;

		for (int iPos = 0; iPos < INVENTORY_SIZE; iPos++)
		{
			if (lpObj->pInventory[iPos].IsItem() == false)
			{
				continue;
			}

			dwInvenCnt++;

			if (lpObj->pInventory[iPos].GetNumber())
			{
				SerialCheck->Output("[SERIAL CHECK] Inventory Item Acc:[%s],Name:[%s],SR:[%I64d],POS:[%d],LVL:[%d],ItemName:[%s]",
					lpObj->AccountID, lpObj->Name, lpObj->pInventory[iPos].GetNumber(), iPos, lpObj->pInventory[iPos].m_Level, lpObj->pInventory[iPos].GetName());

				dwInvenNonZero++;
			}
		}

		SerialCheck->Output("[SERIAL CHECK] Inventory Item List Complete Acc:[%s],Name:[%s], CNT:[%d] NON-ZERO-CNT:[%d]",
			lpObj->AccountID, lpObj->Name, dwInvenCnt, dwInvenNonZero);

		for (int iPos = 0; iPos < INVENTORY_SIZE; iPos++)
		{
			if (lpObj->pInventory[iPos].IsItem() == false)
			{
				continue;
			}

			UINT64 i64Serial = lpObj->pInventory[iPos].GetNumber();

			if (i64Serial)
			{
				std::pair<std::set<UINT64>::iterator, bool> ret = UserAllItemList.insert(i64Serial);

				if (ret.second == false)
				{
					SerialCheck->Output("[SERIAL CHECK] ~~~~~~ DUPLICATED ITEM FOUND [INVENTORY] - ITEM WILL BE DELETED ~~~~~~ ACC:[%s],Name:[%s],SERIAL:[%I64d],POS:[%d],LVL:[%d],ItemName:[%s]",
						lpObj->AccountID, lpObj->Name, i64Serial, iPos, lpObj->pInventory[iPos].m_Level, lpObj->pInventory[iPos].GetName());

					gObjInventoryDeleteItem(lpObj->m_Index, iPos);
					dwInvenDeleteCnt++;
					dwSameSerialItemCount++;
				}
			}
		}

		if (dwSameSerialItemCount)
		{
			lpObj->m_PlayerData->bHasSameSerialItem = true;
			bCheckFail = true;

			SerialCheck->Output("[SERIAL CHECK][RESULT] ~~~~~~~ FAILED ~~~~~~ ACC : [%s], CharName :[%s], AllCnt:[%d], Case : [%d] InvenCnt:[%d], IP :[%s]",
				lpObj->AccountID, lpObj->Name, dwSameSerialItemCount, bCase, dwInvenDeleteCnt, lpObj->m_PlayerData->Ip_addr);

			SerialCheck->Output("[SERIAL CHECK] ACC:[%s],NAME:[%s],IP:[%s] - DISCONNECT",
				lpObj->AccountID, lpObj->Name, lpObj->m_PlayerData->Ip_addr);

			g_Log.Add("[SERIAL CHECK] ACC:[%s],NAME:[%s],IP:[%s] - DISCONNECT",
				lpObj->AccountID, lpObj->Name, lpObj->m_PlayerData->Ip_addr);

			GSProtocol.GCSendDisableReconnect(aIndex);
		}

		else
		{
			SerialCheck->Output("[SERIAL CHECK][RESULT] ~~~~~~~ SUCCESS ~~~~~~ ACC : [%s], CharName :[%s], Case : [%d], IP :[%s]",
				lpObj->AccountID, lpObj->Name, bCase, lpObj->m_PlayerData->Ip_addr);
		}
	}
}

// Season X Mass Combination System
// Check for Empty space in inventory for multiple items
bool gObjChaosBoxPutItemTest(int aIndex, CItem Item, BYTE btCount)
{
	BYTE TempInventoryMap[INVENTORY_MAP_SIZE];
	BYTE TempEventInventoryMap[EVENT_INVENTORY_MAP_SIZE];

	int result = 1;

	memcpy(TempInventoryMap, gObj[aIndex].pInventoryMap, INVENTORY_MAP_SIZE);
	memcpy(TempEventInventoryMap, gObj[aIndex].pEventInventoryMap, EVENT_INVENTORY_MAP_SIZE);

	for (int n = 0; n < btCount; n++)
	{
		if (Item.IsItem() == 1)
		{
			if (IsEventItem(Item.m_Type))
			{
				if (gObjTempEventInventoryInsertItem(&gObj[aIndex], Item, TempEventInventoryMap) == 255)
				{
					return false;
				}
			}

			else
			{
				if (gObjTempInventoryInsertItem(&gObj[aIndex], Item, TempInventoryMap) == 255)
				{
					return false;
				}
			}

			result = 1;
		}
	}

	if (result == 1)
	{
		return true;
	}

	return false;
}

BYTE gObjChaosBoxInsertItemTemp(LPOBJ lpObj, CItem * Item)
{
	CItem item;
	int w, h, iwidth, iheight;
	BYTE blank = 0;

	if (Item->GetSize((int &)iwidth, (int &)iheight) == 0)
	{
		g_Log.Add("error: Item does not exist %s %d", __FILE__, __LINE__);
		return -1;
	}

	for (h = 0; h < 4; h++)
	{
		for (w = 0; w < 8; w++)
		{
			if (*(BYTE*)(lpObj->pChaosBoxMap + h * 8 + w) == 255)
			{
				blank = gObjMapRectCheck(lpObj->pChaosBoxMap, w, h, 8, 4, iwidth, iheight);

				if (blank != 255)
				{
					return blank;
				}
			}
		}
	}

	return -1;
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

