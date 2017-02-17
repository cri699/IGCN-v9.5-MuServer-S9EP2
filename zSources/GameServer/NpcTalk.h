//////////////////////////////////////////////////////////////////////
// NpcTalk.h
#ifndef NPCTALK_H
#define NPCTALK_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



struct GC_ANS_NPC_TALK_V1
{
	PHEADB	PHeader;	// C3:30
	BYTE NpcNumber;
	BYTE Param1;
	BYTE Param2;
	BYTE Param3;
	BYTE Param4;
};

struct GC_ANS_NPC_TALK_V2
{
	PHEADB	PHeader;	// C3:30
	BYTE NpcNumber;
	BYTE Param1;
	BYTE Param2;
	BYTE Param3;
	BYTE Param4;
	BYTE Param5;
	BYTE Param6;
};

struct PMSG_ANS_DIG_NPC_CLICK // 1.01.00
{
	PBMSG_HEAD2 h;
	WORD	wNPCIndex;
	DWORD	dwContributePoint;
};

BOOL NpcTalk(LPOBJ lpNpc, LPOBJ lpObj);
bool Npc_Dialog(LPOBJ lpNpc, LPOBJ lpObj);

int NpcBattleAnnouncer(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcFasi(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcGuildMasterTalk(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcRolensiaGuard(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcChaosGoblelin(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcRusipher(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcPosionGirl(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcDeviasWizard(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcDeviasGuard(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcDeviasWareHousemen(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcWarehouse(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcNoriaRara(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcDeviasMadam(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcEventChipNPC(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcRorensiaSmith(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcNoriJangIn(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcQuestCheck(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcServerDivision(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcRoadMerchant(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcAngelKing(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcAngelMessanger(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcRolensiaMadam(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcDeviasWeapon(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcDarkSpiritTrainer(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcJewelMixDealer(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcSiegeMachine_Attack(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcSiegeMachine_Defense(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcElderCircle(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcCastleGuard(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcCastleGateLever(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcCastleCrown(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcCastleSwitch(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcCastleHuntZoneGuard(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcShadowPhantom(int iObjIndex);
BOOL NpcMainatenceMachine(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcReira(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcChaosCardMaster(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcWerewolfQuarrel(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcGateKeeper(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcSeedMaster(LPOBJ lpNpc, LPOBJ lpObj);
BOOL NpcSeedResearcher(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcTitus(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcLugard(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcSilverBox(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcGoldBox(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcJerint(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcGens(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcJulia(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcDavid(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcLeoHelper(LPOBJ lpNpc,LPOBJ lpObj);
bool NpcSantaClause(LPOBJ lpNpc,LPOBJ lpObj);
bool NpcTercia(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcVeina(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcZyro(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcAcheronEntrance(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcElementalMaster(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcArcaWar(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcMarketWall(LPOBJ lpNpc, LPOBJ lpObj);
bool NpcMonicaTalk(LPOBJ lpNpc, LPOBJ lpObj);
#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

