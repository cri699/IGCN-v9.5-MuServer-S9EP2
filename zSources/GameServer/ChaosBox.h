//////////////////////////////////////////////////////////////////////
// ChaosBox.h
#ifndef CHAOSBOX_H
#define CHAOSBOX_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "protocol.h"
#include "zzzitem.h"

#define CHAOS_BOX_SIZE 32
#define MIN_CHAOS_ITEM_LEVEL 4

#define CHAOS_BOX_RANGE(x) (((x)<0)?FALSE:((x)>CHAOS_BOX_SIZE-1)?FALSE:TRUE )

enum CHAOS_TYPE
{
	CHAOS_TYPE_DEFAULT = 0x1,
	CHAOS_TYPE_DEVILSQUARE = 0x2,
	CHAOS_TYPE_UPGRADE_10 = 0x3,
	CHAOS_TYPE_UPGRADE_11 = 0x4,
	CHAOS_TYPE_UPGRADE_12 = 0x16,
	CHAOS_TYPE_UPGRADE_13 = 0x17,
	CHAOS_TYPE_DINORANT = 0x5,
	CHAOS_TYPE_FRUIT = 0x6,
	CHAOS_TYPE_SECOND_WING = 0x7,
	CHAOS_TYPE_BLOODCATLE = 0x8,
	CHAOS_TYPE_FIRST_WING = 0xb,
	CHAOS_TYPE_SETITEM = 0xc,
	CHAOS_TYPE_DARKHORSE = 0xd,
	CHAOS_TYPE_DARKSPIRIT = 0xe,
	CHAOS_TYPE_CLOAK = 0x18,
	CHAOS_TYPE_BLESS_POTION = 0xf,
	CHAOS_TYPE_SOUL_POTION = 0x10,
	CHAOS_TYPE_LIFE_STONE = 0x11,
	CHAOS_TYPE_CASTLE_SPECIAL_ITEM_MIX = 0x12,
	CHAOS_TYPE_HT_BOX = 0x14,
	CHAOS_TYPE_FENRIR_01 = 0x19,
	CHAOS_TYPE_FENRIR_02 = 0x1a,
	CHAOS_TYPE_FENRIR_03 = 0x1b,
	CHAOS_TYPE_FENRIR_04 = 0x1c,
	CHAOS_TYPE_COMPOUNDPOTION_LV1 = 0x1e,
	CHAOS_TYPE_COMPOUNTPOTION_LV2 = 0x1f,
	CHAOS_TYPE_COMPOUNTPOTION_LV3 = 0x20,
	CHAOS_TYPE_JEWELOFHARMONY_PURITY = 0x21,
	CHAOS_TYPE_JEWELOFHARMONY_MIX_SMELTINGITEM = 0x22,
	CHAOS_TYPE_JEWELOFHARMONY_RESTORE_ITEM = 0x23,
	CHAOS_TYPE_380_OPTIONITEM = 0x24,
	CHAOS_TYPE_OLD_PAPER = 0x25,
	CHAOS_TYPE_CONDOR_FEATHER = 0x26,
	CHAOS_TYPE_THIRD_WING = 0x27,
	CHAOS_TYPE_LOTTERY_MIX = 0x28,
	CHAOS_TYPE_BLOSSOM_MIX = 0x29,
	CHAOS_TYPE_SEED_EXTRACT = 0x2a,
	CHAOS_TYPE_SEED_SPHERE_COMPOSITE = 0x2b,
	CHAOS_TYPE_SET_SEED_SPHERE = 0x2c,
	CHAOS_TYPE_SEED_SPHERE_REMOVE = 0x2d,
	CHAOS_TYPE_SECROMICON = 0x2e,
	CHAOS_TYPE_PREMIUM_BOX = 0x2f,
	CHAOS_TYPE_SUMMONS_MIX = 0x30,
	CHAOS_TYPE_UPGRADE_14 = 0x31,
	CHAOS_TYPE_UPGRADE_15 = 0x32,
	CHAOS_TYPE_LUCKYITEM_EXCHANGE = 0x33,
	CHAOS_TYPE_LUCKYITEM_REFINE = 0x34,
	CHAOS_TYPE_ARCABATTLE_BOOTYMIX = 0x36,
	CHAOS_TYPE_ARCABATTLE_SIGNOFLORD = 0x37,
	CHAOS_TYPE_MONSTERWING = 0x38,
	CHAOS_TYPE_SOCKETWEAPON = 0x39,
	CHAOS_TYPE_HEARTBEAT_BOX = 0x3A,
	CHAOS_TYPE_CCF_REWARD = 0x4B,
	CHAOS_TYPE_DSF_REWARD = 0x4C,
	CHAOS_TYPE_ADVANCED_WING = 0xC8

};

struct PMSG_CHAOSMIXRESULT
{
	PBMSG_HEAD h;	// C1:86
	BYTE Result;	// 3
	BYTE ItemInfo[MAX_ITEM_INFO];	// 4
	BYTE Pos; // Season X addon, for Massive Combination
};

#define CB_ERROR					0
#define CB_SUCCESS					1
#define CB_NOT_ENOUGH_ZEN			2
#define CB_TOO_MANY_ITEMS			3
#define CB_LOW_LEVEL_USER			4
#define CB_LACKING_MIX_ITEMS		6
#define CB_INCORRECT_MIX_ITEMS		7
#define CB_INVALID_ITEM_LEVEL		8
#define CB_USER_CLASS_LOW_LEVEL		9
#define CB_NO_BC_CORRECT_ITEMS		10
#define CB_BC_NOT_ENOUGH_ZEN		11
#define CB_MULTIMIX_RESULT			16
#define CB_ARCA_MARK_REG_SUCCESS	0xC1
#define CB_ARCA_MARK_REG_ERROR		0xC2
#define CB_EL_UPGRADEFAIL			0xE1
#define CB_EL_REFINEFAIL			0xE2
#define CB_EL_FAIL_CHAOSASSEMBLY	0xE3
#define CB_NOT_ENOUGH_EMPTY_SPACE	0xF1
#define CB_EL_COMBINATIONFAIL		0xF7
#define CB_EL_SCRIPTERROR			0xF8
#define CB_EL_NOT_ENOUGH_ZEN		0xF9
#define CB_EL_LACKING_MIX_ITEMS		0xFA
#define CB_EL_INCORRECT_MIX_ITEMS	0xFB
#define CB_EL_SCRIPTERROR_2			0xFC
#define CB_EL_PERSONALSTORE			0xFD
#define CB_EL_ALREADY_OPEN			0xFE

class CMixSystem
{
public:
	CMixSystem();
	virtual ~CMixSystem();

	BOOL ChaosBoxCheck(LPOBJ lpObj);
	BOOL ChaosBoxInit(LPOBJ lpObj);
	BOOL ChaosBoxItemDown(LPOBJ lpObj);
	int ChaosBoxMix(LPOBJ lpObj, int & Result2, int & WingNum);
	int  CheckDevilSquareItem(LPOBJ lpObj, int & eventitemcount, int & itemlevel);
	void LogDQChaosItem(LPOBJ lpObj);
	BOOL DevilSquareEventChaosMix(LPOBJ lpObj, BOOL bCheckType, int iItemLevel);
	void LogPlusItemLevelChaosItem(LPOBJ lpObj, int iPlusMixLevel);
	void LogChaosItem(LPOBJ lpObj, LPSTR sLogType);
	BOOL PlusItemLevelChaosMix(LPOBJ lpObj, int mixType);
	void ChaosMixCharmItemUsed(LPOBJ lpObj);
	BOOL IsDeleteItem(int iItemCode);
	BOOL PegasiaChaosMix(LPOBJ lpObj);
	BOOL CircleChaosMix(LPOBJ lpObj);
	BOOL WingChaosMix(LPOBJ lpObj);
	BOOL IllusionTempleItemChaosMix(LPOBJ lpObj);
	BOOL IsMixPossibleItem(int iItemCode);
	BOOL CheckWingItem(int iItemCode);
	BOOL Check2ndWingItem(int iItemCode);
	BOOL Check3rdWingItem(int iItemCode);
	BOOL CheckLevelCondition(CItem * lpItem, short Level, BYTE Op1, BYTE Op2, BYTE Op3, BYTE SetOption, BYTE NewOption);
	BOOL ThirdWingLevel1ChaosMix(LPOBJ lpObj);
	BOOL ThirdWingLevel2ChaosMix(LPOBJ lpObj);
	void ThirdWingMixFail(LPOBJ lpObj);
	void ThirdWingMixFailItemPanalty(CItem * lpItem);
	void DefaultChaosMix(LPOBJ lpObj);
	void DevilSquareItemChaosMix(LPOBJ lpObj);
	void BloodCastleItemChaosMix(LPOBJ lpObj);
	void SetItemChaosMix(LPOBJ lpObj);
	void DarkHorseChaosMix(LPOBJ lpObj);
	void DarkSpiritChaosMix(LPOBJ lpObj);
	void BlessPotionChaosMix(LPOBJ lpObj);
	void SoulPotionChaosMix(LPOBJ lpObj);
	void LifeStoneChaosMix(LPOBJ lpObj);
	void CastleSpecialItemMix(LPOBJ lpObj);
	void HiddenTreasureBoxItemMix(LPOBJ lpObj);
	void Fenrir_01Level_Mix(LPOBJ lpObj);
	void Fenrir_02Level_Mix(LPOBJ lpObj);
	void Fenrir_03Level_Mix(LPOBJ lpObj);
	void Fenrir_04Upgrade_Mix(LPOBJ lpObj);
	void ShieldPotionLv1_Mix(LPOBJ lpObj);
	void ShieldPotionLv2_Mix(LPOBJ lpObj);
	void ShieldPotionLv3_Mix(LPOBJ lpObj);
	void LotteryItemMix(LPOBJ lpObj);
	void SeedExtractMix(LPOBJ lpObj);
	void SeedSphereCompositeMix(LPOBJ lpObj);
	void SetSeedSphereMix(LPOBJ lpObj, BYTE btPos);
	void SeedSphereRemoveMix(LPOBJ lpObj, BYTE btPos);
	void SecromiconMix(LPOBJ lpObj);
	void PremiumBoxMix(LPOBJ lpObj);
	void CherryBlossomMix(LPOBJ lpObj);
	void ItemRefineMix(LPOBJ lpObj);
	void MonsterWingMix(LPOBJ lpObj);
	void SummonsMix(LPOBJ lpObj);
	void CCFRewardMix(LPOBJ lpObj);
	void DSFRewardMix(LPOBJ lpObj);
	BOOL AdvancedWingMix(LPOBJ lpObj);

	void CheckEmptySpace_MultiMix(PMSG_REQ_CHAOS_MULTIMIX_CHECK * aRecv, int aIndex);

	void BlessPotionChaosMix_Multi(LPOBJ lpObj, int iMixCount);
	void SoulPotionChaosMix_Multi(LPOBJ lpObj, int iMixCount);
	void CircleChaosMix_Multi(LPOBJ lpObj, int iMixCount);
	void PegasiaChaosMix_Multi(LPOBJ lpObj, int iMixCount);
	void BloodCastleItemChaosMix_Multi(LPOBJ lpObj, int iMixCount);
	void DevilSquareItemChaosMix_Multi(LPOBJ lpObj, int iMixCount);
	void ShieldPotionLv1_Mix_Multi(LPOBJ lpObj, int iMixCount);
	void ShieldPotionLv2_Mix_Multi(LPOBJ lpObj, int iMixCount);
	void ShieldPotionLv3_Mix_Multi(LPOBJ lpObj, int iMixCount);
	void PremiumBoxMix_Multi(LPOBJ lpObj, int iMixCount);

private:
	int			CHAOS_GEM_ITEMINDEX;
	int			BLESS_GEM_ITEMINDEX;
	int			SOUL_GEM_ITEMINDEX;
	int			CREATURE_GEM_ITEMINDEX;
	int			BLESS_GEM_BUNDLE_ITEMINDEX;
	int			SOUL_GEM_BULDLE_ITEMINDEX;
	int			CONDOR_FEATHER_ITEMINDEX;
	int			CONDOR_FLAME_ITEMINDEX;
	int			WING_OF_STORM_ITEMINDEX;
	int			WING_OF_ETERNAL_ITEMINDEX;
	int			WING_OF_ILLUSION_ITEMINDEX;
	int			WING_OF_RUIN_ITEMINDEX;
	int			CAPE_OF_EMPEROR_ITEMINDEX;
	int			WING_OF_DIMENSION_ITEMINDEX;
	int			CAPE_OF_OVERRULE_ITEMINDEX;
	int			CLOAK_OF_TRANSCEDENCE_ITEMINDEX;
	int			FRUIT_ITEMINDEX;
	int			DINORANT_ITEMINDEX;
	int			BLOODCASTLE_INVITATION_ITEMINDEX;
	int			DEVILSQUARE_INVITATION_ITEMINDEX;
	int			SMALL_SD_POTION_ITEMINDEX;
	int			MEDIUM_SD_POTION_ITEMINDEX;
	int			LARGE_SD_POTION_ITEMINDEX;
	int			BLESS_POTION_ITEMINDEX;
	int			SOUL_POTION_ITEMINDEX;
	int			GOLDEN_BOX_ITEMINDEX;
	int			SILVER_BOX_ITEMINDEX;
	int			HIGH_REFINE_STONE_ITEMINDEX;
	int			LOW_REFINE_STONE_ITEMINDEX;
	int			HARMONY_JEWEL_ITEMINDEX;
	int			m_i3rdWingOpt_LuckRate;
};

extern CMixSystem g_MixSystem;
#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

