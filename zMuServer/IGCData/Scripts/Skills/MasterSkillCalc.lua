-- // ============================================================
-- // == INTERNATIONAL GAMING CENTER NETWORK
-- // == www.igc-network.com
-- // == (C) 2010-2015 IGC-Network (R)
-- // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
-- // == File is a part of IGCN Group MuOnline Server files.
-- // ============================================================

-- MasterSkillCalc Control Script, Lua v5.2
-- Skill Tree Skill Configuration (Result, Time) - Can be modified to adjust for own needs

-- SkillID refers to Index of skill in \Data\Skills\SkillList.txt

-- Character Classes
CLASS_WIZARD                                          = 0
CLASS_KNIGHT                                          = 1
CLASS_ELF                                             = 2
CLASS_GLADIATOR                                       = 3
CLASS_DARKLORD                                        = 4
CLASS_SUMMONER                                        = 5
CLASS_RAGEFIGHTER                                     = 6

-- SkillID: 403, Soul Barrier Strengthener - (Grand Master)
function WizardMagicDefense_Level1(Index, TargetIndex, Dexterity, Energy)
	local SkillEffect = Dexterity / 50 + Energy / 200 + 10
	local SkillTime = Energy / 40 + 60
	
	if (Index == TargetIndex and SkillEffect > 60) then -- casting spell on yourself
		SkillEffect = 60
	elseif (Index ~= TargetIndex and SkillEffect > 50) then -- casting spell on others
		SkillEffect = 50
	end
	
	return SkillEffect, SkillTime
end

-- SkillID: 404, Soul Barrier Proficiency - (Grand Master)
function WizardMagicDefense_Level2(Index, TargetIndex, Dexterity, Energy)
	local SkillEffect = Dexterity / 50 + Energy / 200 + 10
	local SkillTime = Energy / 40 + 60
	
	if (Index == TargetIndex and SkillEffect > 70) then -- casting spell on yourself
		SkillEffect = 70
	elseif (Index ~= TargetIndex and SkillEffect > 50) then -- casting spell on others
		SkillEffect = 50
	end
	
	return SkillEffect, SkillTime
end

-- SkillID: 406, Soul Barrier Mastery (Grand Master)
function WizardMagicDefense_Level3(Index, TargetIndex, Dexterity, Energy)
	local SkillEffect = Dexterity / 50 + Energy / 200 + 10
	local SkillTime = Energy / 40 + 60
	
	if (Index == TargetIndex and SkillEffect > 75) then -- casting spell on yourself
		SkillEffect = 75
	elseif (Index ~= TargetIndex and SkillEffect > 50) then -- casting spell on others
		SkillEffect = 60
	end
	
	return SkillEffect, SkillTime
end

-- SkillID: 326, Cyclone Strengthener - (Blade Master)
function CycloneCalc_Master_Knight(InDamage, Energy)
	local OutDamage = InDamage * (Energy / 10 + 200) / 100
	
	return OutDamage
end

-- SkillID: 479, Cyclone Strengthener - (Blade Master)
function CycloneCalc_Master_Gladiator(InDamage, Energy)
	local OutDamage = InDamage * 2
	
	return OutDamage
end

-- SkillID: 327, Slash Strengthener - (Blade Master)
function SlashCalc_Master_Knight(InDamage, Energy)
	local OutDamage = InDamage * (Energy / 10 + 200) / 100
	
	return OutDamage
end

-- SkillID: 328, Falling Slash Strengthener - (Blade Master)
function FallingSlashCalc_Master_Knight(InDamage, Energy)
	local OutDamage = InDamage * (Energy / 10 + 200) / 100
	
	return OutDamage
end

-- SkillID: 329, Lunge Strengthener - (Blade Master)
function LungeCalc_Master_Knight(InDamage, Energy)
	local OutDamage = InDamage * (Energy / 10 + 200) / 100
	
	return OutDamage
end

-- SkillID: 332, Twisting Slash Mastery - (Blade Master)
function TwistingSlashCalc_Master_Knight(InDamage, Energy)
	local OutDamage = InDamage * (Energy / 10 + 200) / 100
	
	return OutDamage
end

-- SkillID: 333, Rageful Blow Mastery - (Blade Master)
function RagefulBlow_Master_Knight(InDamage, Energy)
	local OutDamage = InDamage * (Energy / 10 + 200) / 100
	
	return OutDamage
end

-- SkillID: 336, Slash Strengthener - (Blade Master)
function DeathStab_MasterLevel1_Knight(InDamage, Energy)
	local OutDamage = InDamage * (Energy / 10 + 200) / 100
	
	return OutDamage
end

-- SkillID: 337, Strike of Destruction - (Blade Master)
function StrikeOfDestructionCalc_MasterLevel1_Knight(InDamage, Energy)
	local OutDamage = InDamage * (Energy / 10 + 200) / 100
	
	return OutDamage
end

-- SkillID: 339, Death Stab Proficiency - (Blade Master)
function DeathStab_MasterLevel2_Knight(InDamage, Energy)
	local OutDamage = InDamage * (Energy / 10 + 200) / 100
	
	return OutDamage
end

-- SkillID: 342, Death Stab Mastery - Blade Master)
function DeathStab_MasterLevel3_Knight(InDamage, Energy)
	local OutDamage = InDamage * (Energy / 10 + 200) / 100
	
	return OutDamage
end

-- SkillID: 340, Strike of Destruction Proficiency - Blade Master)
function StrikeOfDestructionCalc_MasterLevel2_Knight(InDamage, Energy)
	local OutDamage = InDamage * (Energy / 10 + 200) / 100
	
	return OutDamage
end

-- SkillID: 343, Strike of Destruction Mastery - Blade Master)
function StrikeOfDestructionCalc_MasterLevel3_Knight(InDamage, Energy)
	local OutDamage = InDamage * (Energy / 10 + 200) / 100
	
	return OutDamage
end

-- SkillID: 344, Blood Storm - (Blade Master)
function BloodStormCalc_MasterLevel1_Knight(InDamage, Energy)
	local OutDamage = InDamage * (Energy / 10 + 200) / 100
	
	return OutDamage
end

-- SkillID: 346, Blood Storm Strengthener
function BloodStormCalc_MasterLevel2_Knight(InDamage, Energy)
	local OutDamage = InDamage * (Energy / 10 + 200) / 100
	
	return OutDamage
end

-- SkillID: 356, Swell Life Strengthener - (Blade Master)
function KnightSkillAddLife_Level1(Vitality, Energy, PartyBonus)
	local SkillEffect = Vitality / 100 + 12 + Energy / 20 + PartyBonus
	local SkillTime = Energy / 10 + 60
	
	return SkillEffect, SkillTime
end

-- SkillID: 360, Swell Life Proficiency - (Blade Master)
function KnightSkillAddLife_Level2(Vitality, Energy, PartyBonus)
	local SkillEffect = Vitality / 100 + 12 + Energy / 20 + PartyBonus
	local SkillTime = Energy / 10 + 60
	
	return SkillEffect, SkillTime
end

-- SkillID: 363, Swell Life Mastery - (Blade Master)
function KnightSkillAddLife_Level3(Vitality, Energy, PartyBonus)
	local SkillEffect = Vitality / 100 + 12 + Energy / 20 + PartyBonus
	local SkillTime = Energy / 10 + 60
	
	return SkillEffect, SkillTime
end

-- SkillID: 413, Heal Strengthener - (High Elf)
function ElfHeal(Class, Index, TargetIndex, Energy)
	local SkillEffect = 0
	
	if (Index ~= TargetIndex) then
		if (Class == CLASS_WIZARD) then
			SkillEffect = Energy / 5 + 5
		elseif (Class == CLASS_KNIGHT) then
			SkillEffect = Energy / 5 + 5
		elseif (Class == CLASS_ELF) then
			SkillEffect = Energy / 5 + 5
		elseif (Class == CLASS_GLADIATOR) then
			SkillEffect = Energy / 5 + 5
		elseif (Class == CLASS_DARKLORD) then
			SkillEffect = Energy / 5 + 5
		elseif (Class == CLASS_SUMMONER) then
			SkillEffect = Energy / 5 + 5
		elseif (Class == CLASS_RAGEFIGHTER) then
			SkillEffect = Energy / 5 + 5
		end
	elseif (Index == TargetIndex) then
		SkillEffect = Energy / 5 + 5
	end
	
	return SkillEffect
end

-- SkillID: 416, Penetration Strengthener - (High Elf)
function PenetrationCalc_Master_Elf(InDamage, Energy)
	local OutDamage = InDamage * 2
	
	return OutDamage
end

-- SkillID: 417, Defense Increase Strengthener - (High Elf)
function ElfDefense_Level1(Class, Index, TargetIndex, Energy)
	local SkillEffect = 0
	local SkillTime = 60
	
	if (Index ~= TargetIndex) then
		if (Class == CLASS_WIZARD) then
			SkillEffect = 2 + Energy / 8
		elseif (Class == CLASS_KNIGHT) then
			SkillEffect = 2 + Energy / 8
		elseif (Class == CLASS_ELF) then
			SkillEffect = 2 + Energy / 8
		elseif (Class == CLASS_GLADIATOR) then
			SkillEffect = 2 + Energy / 8
		elseif (Class == CLASS_DARKLORD) then
			SkillEffect = 2 + Energy / 8
		elseif (Class == CLASS_SUMMONER) then
			SkillEffect = 2 + Energy / 8
		elseif (Class == CLASS_RAGEFIGHTER) then
			SkillEffect = 2 + Energy / 8
		end
	elseif (Index == TargetIndex) then
		SkillEffect = 2 + Energy / 8
	end
	
	return SkillEffect, SkillTime
end

-- SkillID: 423, Defense Increase Mastery - (High Elf)
function ElfDefense_Level2(Class, Index, TargetIndex, Energy)
	local SkillEffect = 0
	local SkillTime = 60
	
	if (Index ~= TargetIndex) then
		if (Class == CLASS_WIZARD) then
			SkillEffect = 2 + Energy / 8
		elseif (Class == CLASS_KNIGHT) then
			SkillEffect = 2 + Energy / 8
		elseif (Class == CLASS_ELF) then
			SkillEffect = 2 + Energy / 8
		elseif (Class == CLASS_GLADIATOR) then
			SkillEffect = 2 + Energy / 8
		elseif (Class == CLASS_DARKLORD) then
			SkillEffect = 2 + Energy / 8
		elseif (Class == CLASS_SUMMONER) then
			SkillEffect = 2 + Energy / 8
		elseif (Class == CLASS_RAGEFIGHTER) then
			SkillEffect = 2 + Energy / 8
		end
	elseif (Index == TargetIndex) then
		SkillEffect = 2 + Energy / 8
	end
	
	return SkillEffect, SkillTime
end

-- SkillID: 420, Attack Increase Strengthener - (High Elf)
function ElfAttack_Level1(Class, Index, TargetIndex, Energy)
	local SkillEffect = 0
	local SkillTime = 60
	
	if (Index ~= TargetIndex) then
		if (Class == CLASS_WIZARD) then
			SkillEffect = 3 + Energy / 7
		elseif (Class == CLASS_KNIGHT) then
			SkillEffect = 3 + Energy / 7
		elseif (Class == CLASS_ELF) then
			SkillEffect = 3 + Energy / 7
		elseif (Class == CLASS_GLADIATOR) then
			SkillEffect = 3 + Energy / 7
		elseif (Class == CLASS_DARKLORD) then
			SkillEffect = 3 + Energy / 7
		elseif (Class == CLASS_SUMMONER) then
			SkillEffect = 3 + Energy / 7
		elseif (Class == CLASS_RAGEFIGHTER) then
			SkillEffect = 3 + Energy / 7
		end
	elseif (Index == TargetIndex) then
		SkillEffect = 3 + Energy / 7
	end
	
	return SkillEffect, SkillTime
end

-- SkillID: 422, Attack Increase Mastery - (High Elf)
function ElfAttack_Level2(Class, Index, TargetIndex, Energy)
	local SkillEffect = 0
	local SkillTime = 60
	
	if (Index ~= TargetIndex) then
		if (Class == CLASS_WIZARD) then
			SkillEffect = 3 + Energy / 7
		elseif (Class == CLASS_KNIGHT) then
			SkillEffect = 3 + Energy / 7
		elseif (Class == CLASS_ELF) then
			SkillEffect = 3 + Energy / 7
		elseif (Class == CLASS_GLADIATOR) then
			SkillEffect = 3 + Energy / 7
		elseif (Class == CLASS_DARKLORD) then
			SkillEffect = 3 + Energy / 7
		elseif (Class == CLASS_SUMMONER) then
			SkillEffect = 3 + Energy / 7
		elseif (Class == CLASS_RAGEFIGHTER) then
			SkillEffect = 3 + Energy / 7
		end
	elseif (Index == TargetIndex) then
		SkillEffect = 3 + Energy / 7
	end
	
	return SkillEffect, SkillTime
end

-- SkillID: 424, Ice Arrow Strengthener - (High Elf)
function IceArrowCalc_Master_Elf(InDamage, Energy)
	local OutDamage = InDamage * 2
	
	return OutDamage
end

-- SkillID: 427, Poison Arrow - (Damage over time) - (High Elf)
function PoisonArrow_MasterLevel1_DotDamage(InDamage)
	local DotDamage = InDamage / 10
	local Time = 10
	
	return DotDamage, Time
end

-- SkillID: 436, Poison Arrow Strengthener - (Damage over time) - (High Elf)
function PoisonArrow_MasterLevel2_DotDamage(InDamage, MasterEffect)
	local DotDamage = (InDamage / 10) + MasterEffect
	local Time = 10
	
	return DotDamage, Time
end

-- SkillID: 429, Party Healing Strengthener - (High Elf)
function ElfPartyHealing(Energy)
	local HP = Energy / 6 + 6
	
	return HP
end

-- SkillID: 430, Bless - (High ELf)
function ElfBless_Level1(Energy)
	local SkillEffect = Energy / 100
	
	return SkillEffect
end

-- SkillID: 433, Bless Strengthener - (High Elf)
function ElfBless_Level2(Energy)
	local SkillEffect = Energy / 100
	
	return SkillEffect
end

-- SkillID: 481, Twisting Slash Strengthener - (Duel Master)
function TwistingSlashCalc_Master_Gladiator(InDamage, Energy)
	local OutDamage = InDamage * 2
	
	return OutDamage
end

-- SkillID: 482, Power Slash Strengthener - (Duel Master)
function PowerSlash_Master_Gladiator(InDamage, Energy)
	local OutDamage = InDamage * 2
	
	return OutDamage
end

-- SkillID: 490, Blood Attack Strengthener - (Duel Master)
function FireSlash_MasterLevel1_Gladiator(InDamage, Energy)
	local OutDamage = InDamage * 2
	
	return OutDamage
end

-- SkillID: 492, Flame Strike Strengthener - (Duel Master)
function FlameStrike_MasterLevel1_Gladiator(InDamage, Energy)
	local OutDamage = InDamage * 2
	
	return OutDamage
end

-- SkillID: 494, Flame Strike Mastery - (Duel Master)
function FlameStrike_MasterLevel2_Gladiator(InDamage, Energy)
	local OutDamage = InDamage * 2
	
	return OutDamage
end

-- SkillID: 493, Fire Slash Mastery - (Duel Master)
function FireSlash_MasterLevel2_Gladiator(InDamage, Energy)
	local OutDamage = InDamage * 2
	
	return OutDamage
end

-- SkillID: 495, Earth Prison - (Duel Master)
function EarthPrison_Level1()
 local DebuffSuccessRate = 10
 local DebuffTime = 5
 
 return DebuffSuccessRate, DebuffTime
end

-- SkillID: 497, Earth Prison Strengthener - (Duel Master)
function EarthPrison_Level2()
 local DebuffSuccessRate = 10
 local DebuffTime = 5
 
 return DebuffSuccessRate, DebuffTime
end

-- SkillID: 508, Fire Burst Strengthener - (Lord Emperor)
function FireBurst_MasterLevel1_Lord(InDamage, Energy)
	local OutDamage = (InDamage * (((Energy) / 20 + 200))) / 100;
	
	return OutDamage
end

-- SkillID: 514, Fire Burst Mastery - (Lord Emperor)
function FireBurst_MasterLevel2_Lord(InDamage, Energy)
	local OutDamage = (InDamage * (((Energy) / 20 + 200))) / 100;
	
	return OutDamage
end

-- SkillID: 509, Force Wave Strengthener - (Lord Emperor)
function ForceWave_Master_Lord(InDamage, Energy)
	local OutDamage = (InDamage * (((Energy) / 20 + 200))) / 100;
	
	return OutDamage
end

-- SkillID: 511, Critical DMG Increase PowUp - (Lord Emperor)
function DarkLordCriticalDamage_Level1(Command, Energy)
	local SkillEffect = Command / 25 + Energy / 30
	local SkillTime = Energy / 10 + 60
	
	return SkillEffect, SkillTime
end

-- SkillID: 512, Earthshake Strengthener - (Lord Emperor)
function EarthShake_MasterLevel1_Lord(InDamage, Energy)
	local OutDamage = (InDamage * (((Energy) / 20 + 200))) / 100;
	
	return OutDamage
end

-- SkillID: 516, Earthshake Mastery - (Lord Emperor)
function EarthShake_MasterLevel2_Lord(InDamage, Energy)
	local OutDamage = (InDamage * (((Energy) / 20 + 200))) / 100;
	
	return OutDamage
end

-- SkillID: 515, Critical DMG Increase PowUp (2) - (Lord Emperor)
function DarkLordCriticalDamage_Level2(Command, Energy)
	local SkillEffect = Command / 25 + Energy / 30
	local SkillTime = Energy / 10 + 60
	
	return SkillEffect, SkillTime
end

-- SkillID: 517, Critical DMG Increase PowUp (3) - (Lord Emperor)
function DarkLordCriticalDamage_Level3(Command, Energy)
	local SkillEffect = Command / 25 + Energy / 30
	local SkillTime = Energy / 10 + 60
	
	return SkillEffect, SkillTime
end

-- SkillID: 522, Critical Damage Increase Master - (Lord Emperor)
function DarkLordCriticalDamage_Level4(Command, Energy)
	local SkillEffect = Command / 25 + Energy / 30
	local SkillTime = Energy / 10 + 60
	
	return SkillEffect, SkillTime
end

-- SkillID: 518, Fire Scream Strengthener - (Lord Emperor)
function FireScream_MasterLevel1_Lord(InDamage, Energy)
	local OutDamage = (InDamage * (((Energy) / 20 + 200))) / 100;
	
	return OutDamage
end

-- SkillID: 520, Fire Scream Mastery - (Lord Emperor)
function FireScream_MasterLevel2_Lord(InDamage, Energy)
	local OutDamage = (InDamage * (((Energy) / 20 + 200))) / 100;
	
	return OutDamage
end

-- SkillID: 519, Electric Spark Strengthener - (Lord Emperor)
function ElectricSpark_Master_Lord(InDamage, Energy)
	local OutDamage = (InDamage * (((Energy) / 20 + 200))) / 100;
	
	return OutDamage
end

-- SkillID: 523, Chaotic Diseier Strengthener - (Lord Emperor)
function ChaoticDiseier_Master_Lord(InDamage, Energy)
	local OutDamage = InDamage * (Energy / 25 + 200) / 100
	
	return OutDamage
end

-- SkillID: 454, Sleep Strengthener - MvP - (Dimension Master)
function Sleep_Monster(Energy, Curse, MonsterLevel)
	local SkillSuccessRate = Energy / 30 + Curse / 6 + 20
	local SkillTime = Energy / 100 + 5 - MonsterLevel / 20
	
	return SkillSuccessRate, SkillTime
end

-- SkillID: 454, Sleep Strengthener - PvP - (Dimension Master)
function Sleep_PvP(Energy, Curse, PlayerLevel, TargetLevel)
	local SkillSuccessRate = Energy / 37 + Curse / 6 + 15
	local SkillTime = Energy / 250 + (PlayerLevel - TargetLevel) / 100 + 4
	
	return SkillSuccessRate, SkillTime
end

-- SkillID: 455, Chain Lightning Strengthener - (Dimension Master)
function ChainLightning_Master_Summoner(InDamage, TargetNumber)
	local DamagePercent = 0
	
	if(TargetNumber == 1) then
		DamagePercent = 100
	elseif(TargetNumber == 2) then
		DamagePercent = 70
	elseif(TargetNumber == 3) then
		DamagePercent = 50
	else
		DamagePercent = 0
	end
		
	local OutDamage = InDamage * DamagePercent / 100
	
	return OutDamage
end

-- SkillID: 459, Weakness Strengthener - MvP - (Dimension Master)
function SummonerWeakness_Monster(Energy, Curse, MonsterLevel)
	local SkillSuccessRate = Energy / 50 + Curse / 6 + 32
	local SkillEffect = Energy / 58 + 4
	local SkillTime = Energy / 100 + 4 - MonsterLevel / 20
	
	return SkillSuccessRate, SkillEffect, SkillTime
end

-- SkillID: 459, Weakness Strengthener - PvP - (Dimension Master)
function SummonerWeakness_PvP(Energy, Curse, PlayerLevel, TargetLevel)
	local SkillSuccessRate =  Energy / 50 + Curse / 6 + 17
	local SkillEffect = Energy / 93 + 3
	local SkillTime = Energy / 300 + (PlayerLevel - TargetLevel) / 150 + 5
	
	return SkillSuccessRate, SkillEffect, SkillTime
end

-- SkillID: 460, Innovation Strengthener - MvP - (Dimension Master)
function SummonerInnovation_Monster(Energy, Curse, MonsterLevel)
	local SkillSuccessRate = Energy / 50 + Curse / 6 + 32
	local SkillEffect = Energy / 90 + 20
	local SkillTime = Energy / 100 + 4 - MonsterLevel / 20
	
	return SkillSuccessRate, SkillEffect, SkillTime
end

-- SkillID: 460, Innovation Strengthener - PvP - (Dimension Master)
function SummonerInnovation_PvP(Energy, Curse, PlayerLevel, TargetLevel)
	local SkillSuccessRate =  Energy / 50 + Curse / 6 + 17
	local SkillEffect = Energy / 110 + 12
	local SkillTime = Energy / 300 + (PlayerLevel - TargetLevel) / 150 + 5
	
	return SkillSuccessRate, SkillEffect, SkillTime
end

-- SkillID: 458, Drain Life Strengthener - MvP - (Dimension Master)
function SummonerDrainLife_Monster_Level1(Energy, MonsterLevel)
	local AddHP = (Energy / 15) + MonsterLevel / 2.5
	
	return AddHP
end

-- SkillID: 458, Drain Life Skill - PvP - (Dimension Master)
function SummonerDrainLife_PvP_Level1(Energy, Damage)
	local AddHP = Energy / 23 + 10 * Damage / 100
	
	return AddHP
end

-- SkillID: 462, Drain Life Master - MvP - (Dimension Master)
function SummonerDrainLife_Monster_Level2(Energy, MonsterLevel)
	local AddHP = (Energy / 15) + MonsterLevel / 2.5
	
	return AddHP
end

-- SkillID: 462, Drain Life Skill - PvP - (Dimension Master)
function SummonerDrainLife_PvP_Level2(Energy, Damage)
	local AddHP = Energy / 23 + 10 * Damage / 100
	
	return AddHP
end

-- SkillID: 469, Berserker Strengthener - (Dimension Master)
function SummonerBerserker_Level1(Energy)
	local SkillEffectUP = Energy / 30
	local SkillEffectDOWN = Energy / 60
	local SkillTime = Energy / 20 + 30
	
	return SkillEffectUP, SkillEffectDOWN, SkillTime
end

-- SkillID: 470, Berserker Proficiency - (Dimension Master)
function SummonerBerserker_Level2(Energy)
	local SkillEffectUP = Energy / 30
	local SkillEffectDOWN = Energy / 60
	local SkillTime = Energy / 20 + 30
	
	return SkillEffectUP, SkillEffectDOWN, SkillTime
end

-- SkillID: 472, Berserker Mastery - (Dimension Master)
function SummonerBerserker_Level3(Energy)
	local SkillEffectUP = Energy / 30
	local SkillEffectDOWN = Energy / 60
	local SkillTime = Energy / 20 + 30
	
	return SkillEffectUP, SkillEffectDOWN, SkillTime
end

-- SkillID: 554, Killing Blow Mastery - (Fist Master)
function KillingBlow_Master(InDamage, Vitality)
 local OutDamage = InDamage * (Vitality / 10 + 50) / 100.0
 
 return OutDamage
end

-- SkillID: 555, Beast Uppercut Mastery - (Fist Master)
function BeastUppercut_Master(InDamage, Vitality)
 local OutDamage = InDamage * (Vitality / 10 + 50) / 100.0
 
 return OutDamage
end

-- SkillID: 562, Chain Drive Mastery - (Fist Master)
function ChainDrive_Master(InDamage, Vitality)
 local OutDamage = (InDamage + Vitality / 10) * (Vitality / 10 + 50) / 100.0
 
 return OutDamage
end

-- SkillID: 563, Dark Side Strengthener - (Fist Master)
function RageFighterDarkSideIncDamage(InDamage, Dexterity, Energy)
 local OutDamage = (Dexterity / 8 + InDamage + Energy / 10) * (Dexterity / 8 + Energy / 10 + 100) / 100.0
 
 return OutDamage
end

-- SkillID: 561: Dragon Roar Mastery - (Fist Master)
function DragonRoar_Master(InDamage, Energy)
 local OutDamage = (InDamage + Energy / 10) * (Energy / 10 + 50) / 100.0
 
 return OutDamage
end

-- SkillID: 564, Dragon Slasher Strengthener - (Fist Master)
function DragonSlasher_Level1(InDamage, SkillBonus, Energy, TargetType)
 local OutDamage = 0
 
 if (TargetType == 1) then -- User
 	OutDamage = (InDamage + SkillBonus) * (Energy / 10 + 50) / 100.0
 else -- Monster
 	OutDamage = ((InDamage + SkillBonus) * (Energy / 10 + 50) / 100.0 + 100.0) * 3.0
 end
 
 return OutDamage
end

-- SkillID: 566, Dragon Slasher Mastery - (Fist Master)
function DragonSlasher_Level2(InDamage, SkillBonus, Energy, TargetType)
 local OutDamage = 0
 
 if (TargetType == 1) then -- User
 	OutDamage = (InDamage + SkillBonus) * (Energy / 10 + 50) / 100.0
 else -- Monster
 	OutDamage = ((InDamage + SkillBonus) * (Energy / 10 + 50) / 100.0 + 100.0) * 3.0
 end
 
 return OutDamage
end

-- SkillID: 565, Blood Howling - (Fist Master)
function BloodHowl_Level1(TargetHP)
 local SkillEffect = TargetHP * 5 / 100
 local SkillSuccessRate = 10
 local SkillTime = 10

 return SkillEffect, SkillSuccessRate, SkillTime
end

-- SkillID: 567, Blood Howling Strengthener - (Fist Master)
function BloodHowl_Level2(TargetHP)
 local SkillEffect = TargetHP * 5 / 100
 local SkillSuccessRate = 10
 local SkillTime = 10

 return SkillEffect, SkillSuccessRate, SkillTime
end