-- // ============================================================
-- // == INTERNATIONAL GAMING CENTER NETWORK
-- // == www.igc-network.com
-- // == (C) 2010-2015 IGC-Network (R)
-- // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
-- // == File is a part of IGCN Group MuOnline Server files.
-- // ============================================================

-- RegularSkillCalc Control Script, Lua v5.2
-- Skill Configuration (Result, Time) - Can be modified to adjust for own needs

-- SkillID refers to Index of skill in \Data\Skills\SkillList.txt
-- BuffID refers to \Data\BuffEffectManager.cfg

-- Character Classes
CLASS_WIZARD                                          = 0
CLASS_KNIGHT                                          = 1
CLASS_ELF                                             = 2
CLASS_GLADIATOR                                       = 3
CLASS_DARKLORD                                        = 4
CLASS_SUMMONER                                        = 5
CLASS_RAGEFIGHTER                                     = 6

-- SkillID: 16, Soul Barrier - (Dark Wizard, Soul Master, Grand Master)
function WizardMagicDefense(Index, TargetIndex, Dexterity, Energy)
	local SkillEffect = Dexterity / 50 + Energy / 200 + 10
	local SkillTime = Energy / 40 + 60
	
	if(Index == TargetIndex and SkillEffect > 50) then -- casting spell on yourself
		SkillEffect = 50
	elseif (Index ~= TargetIndex and SkillEffect > 50)  then -- casting spell on others
		SkillEffect = 50
	end
	
	return SkillEffect, SkillTime
end

-- SkillID: 19, 20, 21, 22, 23, 41, 42, 43, 44, 49, 55, 57, Falling Slash, Lunge, Uppercut,
-- Cyclone, Slash, Twisting Slash, Rageful Blow, Death Stab,Crescent Moon Slash, Fire Breath,
-- Fire Slash, Spiral Slash - (Dark Knight, Blade Knight, Blade Master, Magic Gladiator, Duel Master)
function Knight_Gladiator_CalcSkillBonus(Class, InDamage, Energy)	
	local OutDamage = 0
	
	if (Class == CLASS_GLADIATOR or Class == CLASS_DARKLORD) then
		OutDamage = InDamage * 2
	else
		OutDamage = (InDamage * ( 200 + ( Energy / 10 ) ) ) / 100;
	end
		
	return OutDamage
end

-- SkillID: 48, Swell Life - (Dark Knight, Blade Knight, Blade Master)
function KnightSkillAddLife(Vitality, Energy, PartyBonus)
	local SkillEffect = Vitality / 100 + 12 + Energy / 20 + PartyBonus
	local SkillTime = Energy / 10 + 60
	
	return SkillEffect, SkillTime
end

-- SkillID: 232, Strike of Destruction -(Dark Knight, Blade Knight, Blade Master)
function StrikeOfDestructionCalc(InDamage, Energy)
	local OutDamage = InDamage * (Energy /10 + 200) / 100;
	
	return OutDamage
end

-- SkillID: 46, 51, 52, Starfall, Ice Arrow, Penetration - (Fairy Elf, Muse Elf, High Elf)
function Elf_CalcSkillBonus(InDamage, Energy)
	local OutDamage = InDamage * 2
	
	return OutDamage
end

-- SkillID: 26, Heal - (Fairy Elf, Muse Elf, High Elf)
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

-- SkillID: 28, Greater Damage - (Fairy Elf, Muse Elf, High Elf)
function ElfAttack(Class, Index, TargetIndex, Energy)
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

-- SkillID: 27, Greater Defense - (Fairy Elf, Muse Elf, High Elf)
function ElfDefense(Class, Index, TargetIndex, Energy)
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

-- SkillID: 234, Recovery - (Fairy Elf, Muse Elf, High Elf)
function ElfShieldRecovery(Energy, PlayerLevel)
 local SkillEffect = Energy / 4 + PlayerLevel
 
 return SkillEffect
end

-- SkillID: 47, Impale -(Magic Gladiator, Duel Master, Dark Lord, Lord Emperor)
function ImpaleSkillCalc(Class, InDamage, Energy)
	local OutDamage = 0
	
	if (Class == CLASS_GLADIATOR or Class == CLASS_DARKLORD) then
		OutDamage = InDamage * 2
	else
		OutDamage = ( InDamage * ( Energy / 10 + 200 )  ) / 100;
	end
		
	return OutDamage
end

-- SkillID: 56, Power Slash - (Magic Gladiator, Duel Master)
function GladiatorPowerSlash(InDamage, Energy)
	local OutDamage = InDamage * 2
	
	return OutDamage
end

-- SkillID: 236, Flame Strike - (Magic Gladiator, Duel Master)
function FlameStrikeCalc(InDamage)
	local OutDamage = InDamage * 2
	
	return OutDamage
end

-- SkillID: 60 ,61, 62, 65, 74, 78, Force, Fire Burst, Earthshake, Electric Spike, Fire Blast, Fire Scream - (Dark Lord, Lord Emperor)
function Lord_CalcSkillBonus(InDamage, Energy)
	local OutDamage = ( InDamage * ( ( Energy / 20 + 200 ) ) ) / 100;
	
	return OutDamage
end

-- SkillID: 64, Increase Critical Damage - (Dark Lord, Lord Emperor)
function DarkLordCriticalDamage(Command, Energy)
	local SkillEffect = Command / 25 + Energy / 30
	local SkillTime = Energy / 10 + 60
	
	return SkillEffect, SkillTime
end

-- SkillID: 238, Chaotic Diseier - (Dark Lord, Lord Emperor)
function ChaoticDiseierCalc(InDamage, Energy)
	local OutDamage = InDamage * (Energy / 25 + 200) / 100
	
	return OutDamage
end

-- SkillID: 214, Drain Life - PVP - (Summoner, Bloody Summoner, Dimension Master)
function SummonerDrainLife_PvP(Energy, Damage)
	local AddHP = Energy / 23 + (10 * Damage / 100)
	
	return AddHP
end

-- SkillID: 214, Drain Life - MvP - (Summoner, Bloody Summoner, Dimension Master)
function SummonerDrainLife_Monster(Energy, MonsterLevel)
	local AddHP = (Energy / 15) + (MonsterLevel / 2.5)
	
	return AddHP
end

-- SkillID: 215, Chain Lighting - (Summoner, Bloody Summoner, Dimension Master)
function ChainLightningCalc(InDamage, TargetNumber)
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

-- SkillID: 217, Damage Reflection - (Summoner, Bloody Summoner, Dimension Master)
function SummonerDamageReflect(Energy)
	local Reflect = 30 + (Energy / 42)
	local Time = 30 + (Energy / 25)
	
	if (Reflect > 25) then
		Reflect = 25
	end
	
	return Reflect, Time
end

-- SkillID: 218, Berserker - (Summoner, Bloody Summoner, Dimension Master)
function SummonerBerserker(Energy)
	local SkillEffectUP = Energy / 30
	local SkillEffectDOWN = Energy / 60
	local SkillTime = Energy / 20 + 30
	
	return SkillEffectUP, SkillEffectDOWN, SkillTime
end

-- SkillID: 218, BuffID: 81, 150, 151, 152 - (Summoner, Bloody Summoner, Dimension Master)
function SummonerBerserkerAttackDamage(Strength, Dexterity)
	local AttackMin = (Strength+Dexterity) / 7
	local AttackMax = (Strength+Dexterity) / 4
	
	return AttackMin, AttackMax
end

-- SkillID: 218, Berserker - Magic Damage - (Summoner, Bloody Summoner, Dimension Master)
function SummonerBerserkerMagicDamage(Effect, Energy)
	local MagicMin = (Energy / 9) * Effect / 100
	local MagicMax = (Energy / 4) * Effect / 100
	
	return MagicMin, MagicMax
end

-- SkillID: 218, Berserker - Curse Damage - (Summoner, Bloody Summoner, Dimension Master)
function SummonerBerserkerCurseDamage(Effect, Energy)
	local CurseMin = Effect * (Energy / 9) / 100
	local CurseMax = Effect * ((Energy + 0.015) / 4) / 100
	
	return CurseMin, CurseMax
end

-- SkillID: 219, Sleep - MvP - (Summoner, Bloody Summoner, Dimension Master)
function Sleep_Monster(Energy, Curse, MonsterLevel)
	local SkillSuccessRate = Energy / 30 + Curse / 6 + 20
	local SkillTime = Energy / 100 + 5 - MonsterLevel / 20
	
	return SkillSuccessRate, SkillTime
end

-- SkillID: 219, Sleep - PvP - (Summoner, Bloody Summoner, Dimension Master)
function Sleep_PvP(Energy, Curse, PlayerLevel, TargetLevel)
	local SkillSuccessRate = Energy / 37 + Curse / 6 + 15
	local SkillTime = Energy / 250 + (PlayerLevel - TargetLevel) / 100 + 4
	
	return SkillSuccessRate, SkillTime
end

-- SkillID: 221, Weakness - MvP - (Summoner, Bloody Summoner, Dimension Master)
function SummonerWeakness_Monster(Energy, Curse, MonsterLevel)
	local SkillSuccessRate = Energy / 50 + Curse / 6 + 32
	local SkillEffect = Energy / 58 + 4
	local SkillTime = Energy / 100 + 4 - MonsterLevel / 20
	
	return SkillSuccessRate, SkillEffect, SkillTime
end

-- SkillID: 221, Weakness - PvP - (Summoner, Bloody Summoner, Dimension Master)
function SummonerWeakness_PvP(Energy, Curse, PlayerLevel, TargetLevel)
	local SkillSuccessRate =  Energy / 50 + Curse / 6 + 17
	local SkillEffect = Energy / 93 + 3
	local SkillTime = Energy / 300 + (PlayerLevel - TargetLevel) / 150 + 5
	
	return SkillSuccessRate, SkillEffect, SkillTime
end

-- SkillID: 222, Innovation - MvP - (Summoner, Bloody Summoner, Dimension Master)
function SummonerInnovation_Monster(Energy, Curse, MonsterLevel)
	local SkillSuccessRate = Energy / 50 + Curse / 6 + 32
	local SkillEffect = Energy / 90 + 20
	local SkillTime = Energy / 100 + 4 - MonsterLevel / 20
	
	return SkillSuccessRate, SkillEffect, SkillTime
end

-- SkillID: 222, Innovation - PvP - (Summoner, Bloody Summoner, Dimension Master)
function SummonerInnovation_PvP(Energy, Curse, PlayerLevel, TargetLevel)
	local SkillSuccessRate =  Energy / 50 + Curse / 6 + 17
	local SkillEffect = Energy / 110 + 12
	local SkillTime = Energy / 300 + (PlayerLevel - TargetLevel) / 150 + 5
	
	return SkillSuccessRate, SkillEffect, SkillTime
end

-- SkillID: 223, Explosion - Damage over time - (Summoner, Bloody Summoner, Dimension Master)
function ExplosionDotDamage(Damage, MasterEffect)
	local DotDamage = 0
	local Time = 5
	
	if (MasterEffect <= 0) then
		DotDamage = Damage * 60 / 100
	else
		DotDamage = Damage * (60 + MasterEffect) / 100
	end
	
	return DotDamage, Time
end

-- SkillID: 224, Requiem - Damage over time - (Summoner, Bloody Summoner, Dimension Master)
function RequiemDotDamage(Damage)
	local DotDamage = Damage * 60 / 100
	local Time = 5
	
	return DotDamage, Time
end

-- SkillID: 260, Killing Blow - (Rage Fighter, Fist Master)
function RageFighterKillingBlow(InDamage, Vitality)
	local OutDamage = InDamage * (Vitality / 10 + 50) / 100.0
	
	return OutDamage
end

-- SkillID: 261, Beast Uppercut - (Rage Fighter, Fist Master)
function RageFighterBeastUppercut(InDamage, Vitality)
	local OutDamage = InDamage * (Vitality / 10 + 50) / 100.0
	
	return OutDamage
end

-- SkillID: 262, Chain Drive - (Rage Fighter, Fist Master)
function RageFighterChainDrive(InDamage, Vitality)
	local OutDamage = (InDamage + Vitality / 10) * (Vitality / 10 + 50) / 100.0
	
	return OutDamage
end

-- SkillID: 263, Dark Side - (Rage Fighter, Fist Master)
function RageFighterDarkSideIncDamage(InDamage, Dexterity, Energy)
 local OutDamage = (Dexterity / 8 + InDamage + Energy / 10) * (Dexterity / 8 + Energy / 10 + 100) / 100.0
 
 return OutDamage
end

-- SkillID: 264, Dragon Roar - (Rage Fighter, Fist Master)
function RageFighterDragonRoar(InDamage, Energy)
	local OutDamage = (InDamage + Energy / 10) * (Energy / 10 + 50) / 100.0
	
	return OutDamage
end

-- SkillID: 265, Dragon Slasher - (Rage Fighter, Fist Master)
function RageFighterDragonSlasher(InDamage, Energy, TargetType)
	local OutDamage = 0
	
	if (TargetType == 1) then -- Player
		OutDamage = InDamage * (Energy / 10 + 50) / 100.0
	else -- Monster
		OutDamage = (InDamage * (Energy / 10 + 50) / 100.0 + 100.0) * 3.0
	end
	
	return OutDamage
end

-- SkillID: 265, 564, 566 - Dragon Slasher - SD Reduction - (Rage Fighter, Fist Master)
function DragonSlasher_DecreaseVictimSD(Energy)
	local SuccessRate = 0
	local DecreasePercent = 0
	
	SuccessRate = Energy / 100 + 10
	DecreasePercent = Energy / 30 + 10
	
	if (SuccessRate > 100) then
		SuccessRate = 100
	end
	
	if (DecreasePercent > 100) then
		DecreasePercent = 100
	end
	
	return SuccessRate, DecreasePercent
end

-- SkillID: 269, Charge - (Rage Fighter, Fist Master)
function RageFighterCharge(InDamage, Vitality)
	local OutDamage = InDamage * (Vitality / 10 + 50) / 100.0
	
	return OutDamage
end

-- SkillID: 270, Phoenix Shot - (Rage Fighter, Fist Master)
function RageFighterPhoenixShot(InDamage, Vitality)
	local OutDamage = InDamage * (Vitality / 10 + 200) / 100.0
	
	return OutDamage
end

-- SkillID: X, Combo Skill - (Dark Knight, Blade Knight, Blade Master)
function ComboSkillDamage(Strength, Dexterity, Vitality, Energy)
 local Damage = (Strength * 1.5) + Dexterity + Energy
 
 return Damage
end

-- BuffID: 174, 175, 176, 177, 178 - (Fire, Frost, Tornado, Bind, Darkness)
function ElementalDebuffGetDuration(ID, CharacterLevel)
	local Duration = 0
	
	if (ID == 174) then
		Duration = CharacterLevel / 4 + 20
	elseif (ID == 175) then
		Duration = CharacterLevel / 4 + 20
	elseif (ID == 176) then
		Duration = CharacterLevel / 4 + 20
	elseif (ID == 177) then
		Duration = CharacterLevel / 4 + 20
	elseif (ID == 178) then
		Duration = CharacterLevel / 4 + 20
	end
	
	return Duration
end

-- SkillID: 18, Defense - (Shields Skill)
function ShieldSkill_DecreaseVictimDamage(InDamage)
 local OutDamage = InDamage / 2
 
 return OutDamage
end

-- SkillID: 76, Plasma Storm - (Fenrir Pets)
function FenrirSkillCalc(InDamage, PlayerLevel, PlayerMasterLevel)
	local DamageInc = PlayerLevel - 300 + PlayerMasterLevel
	
	if (DamageInc < 0) then
		DamageInc = 0
	end
	
	DamageInc = DamageInc / 5
	
	local OutDamage = ( InDamage * ( DamageInc + 200 ) ) / 100;
	
	return OutDamage
end