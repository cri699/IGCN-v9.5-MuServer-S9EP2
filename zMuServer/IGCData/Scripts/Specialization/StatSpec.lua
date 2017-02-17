-- // ============================================================
-- // == INTERNATIONAL GAMING CENTER NETWORK
-- // == www.igc-network.com
-- // == (C) 2010-2015 IGC-Network (R)
-- // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
-- // == File is a part of IGCN Group MuOnline Server files.
-- // ============================================================

-- Character Classes
CLASS_WIZARD                                         	= 0	-- Fairy Elf, Muse Elf, High Elf
CLASS_KNIGHT                                         	= 1	-- Dark Wizard, Soul Master, Grand Master
CLASS_ELF                                            	= 2	-- Dark Knight, Blade Knight, Blade Master
CLASS_GLADIATOR                                       	= 3	-- Magic Gladiator, Duel Master
CLASS_DARKLORD                                        	= 4	-- Dark Lord, Lord Emperor
CLASS_SUMMONER                                        	= 5	-- Summoner, Bloody Summoner, Dimension Master
CLASS_RAGEFIGHTER                                     	= 6	-- Rage Fighter, Fist Master
CLASS_GROWLANCER                                    	= 7	-- Grow Lancer, Mirage Lancer

-- Options
STAT_OPTION_INC_ATTACK_POWER							= 1
STAT_OPTION_INC_ATTACK_RATE								= 2
STAT_OPTION_INC_ATTACK_RATE_PVP							= 3
STAT_OPTION_INC_DEFENSE									= 4
STAT_OPTION_INC_ATTACK_SPEED							= 5
STAT_OPTION_INC_DEFENSE_RATE							= 6
STAT_OPTION_INC_DEFENSE_RATE_PVP						= 7
STAT_OPTION_INC_LIFE									= 8 -- disabled at present
STAT_OPTION_INC_MAGIC_DAMAGE							= 9
STAT_OPTION_INC_CURSE_DAMAGE							= 10
STAT_OPTION_INC_MANA									= 11 -- disabled at present
STAT_OPTION_INC_PET_DAMAGE								= 12 -- disabled at present			

function StatSpec_GetPercent(Class, OptionID, Strength, Agility, Vitality, Energy, Leadership)
	local Percent = 0

	-- LogAdd(string.format('Class: %d OptionID: %d', Class, OptionID))
	
	if (Class == CLASS_KNIGHT and OptionID == STAT_OPTION_INC_ATTACK_POWER) then
		Percent = StatSpec_AtkPowerInc_Knight(Strength)
		
	elseif (Class == CLASS_ELF and OptionID == STAT_OPTION_INC_ATTACK_POWER) then
		Percent = StatSpec_AtkPowerInc_Elf(Agility)
		
	elseif (Class == CLASS_GLADIATOR and OptionID == STAT_OPTION_INC_ATTACK_POWER) then
		Percent = StatSpec_AtkPowerInc_Gladiator(Strength)
		
	elseif (Class == CLASS_DARKLORD and OptionID == STAT_OPTION_INC_ATTACK_POWER) then
		Percent = StatSpec_AtkPowerInc_Lord(Strength)
		
	elseif (Class == CLASS_RAGEFIGHTER and OptionID == STAT_OPTION_INC_ATTACK_POWER) then
		Percent = StatSpec_AtkPowerInc_Fighter(Strength)
		
	elseif (Class == CLASS_GROWLANCER and OptionID == STAT_OPTION_INC_ATTACK_POWER) then
		Percent = StatSpec_AtkPowerInc_Lancer(Strength)
		
	elseif (Class == CLASS_WIZARD and OptionID == STAT_OPTION_INC_ATTACK_RATE) then
		Percent = StatSpec_AtkRatePvMInc_Wizard(Agility)
	
	elseif (Class == CLASS_WIZARD and OptionID == STAT_OPTION_INC_ATTACK_RATE_PVP) then
		Percent = StatSpec_AtkRatePvPInc_Wizard(Agility)
		
	elseif (Class == CLASS_KNIGHT and OptionID == STAT_OPTION_INC_DEFENSE) then
		Percent = StatSpec_DefenseInc_Knight(Agility)
		
	elseif (Class == CLASS_DARKLORD and OptionID == STAT_OPTION_INC_DEFENSE) then
		Percent = StatSpec_DefenseInc_Lord(Agility)
		
	elseif (Class == CLASS_GROWLANCER and OptionID == STAT_OPTION_INC_DEFENSE) then
		Percent = StatSpec_DefenseInc_Lancer(Agility)
		
	elseif (Class == CLASS_ELF and OptionID == STAT_OPTION_INC_DEFENSE_RATE) then
		Percent = StatSpec_DefRatePvMInc_Elf(Agility)
		
	elseif (Class == CLASS_ELF and OptionID == STAT_OPTION_INC_DEFENSE_RATE_PVP) then
		Percent = StatSpec_DefRatePvPInc_Elf(Agility)
		
	elseif (Class == CLASS_WIZARD and OptionID == STAT_OPTION_INC_MAGIC_DAMAGE) then
		Percent = StatSpec_MagicPowerInc_Wizard(Energy)
		
	elseif (Class == CLASS_GLADIATOR and OptionID == STAT_OPTION_INC_MAGIC_DAMAGE) then
		Percent = StatSpec_MagicPowerInc_Gladiator(Energy)
		
	elseif (Class == CLASS_SUMMONER and OptionID == STAT_OPTION_INC_MAGIC_DAMAGE) then
		Percent = StatSpec_MagicPowerInc_Summoner(Energy)
	
	elseif (Class == CLASS_SUMMONER and OptionID == STAT_OPTION_INC_CURSE_DAMAGE) then
		Percent = StatSpec_CursePowerInc_Summoner(Energy)
	
	end
	
	return Percent	
end
		
function StatSpec_AtkPowerInc_Knight(Strength)
	local Percent = 0
	
	if (Strength > 1 and Strength < 1501) then
		Percent = Strength / 150
	elseif (Strength >= 1501 and Strength < 2001) then
		Percent = 10 + (Strength - 1500) / 25
	elseif (Strength >= 2001 and Strength < 3000) then
		Percent = 30 + (Strength - 2000) / 66
	end
	
	return Percent
end

function StatSpec_DefenseInc_Knight(Agility)
	local Percent = 0
	
	if (Agility > 1 and Agility < 1501) then
		Percent = Agility / 150
	elseif (Agility >= 1501 and Agility < 2001) then
		Percent = 10 + (Agility - 1500) / 25
	elseif (Agility >= 2001 and Agility < 3000) then
		Percent = 30 + (Agility - 2000) / 66
	end
	
	return Percent
end

function StatSpec_MagicPowerInc_Wizard(Energy)
	local Percent = 0
	
	if (Energy > 1 and Energy < 1501) then
		Percent = Energy / 150
	elseif (Energy >= 1501 and Energy < 2001) then
		Percent = 10 + (Energy - 1500) / 25
	elseif (Energy >= 2001 and Energy < 3000) then
		Percent = 30 + (Energy - 2000) / 66
	end
	
	return Percent
end

function StatSpec_AtkRatePvMInc_Wizard(Agility)
	local Percent = 0
	
	if (Agility > 1 and Agility < 1501) then
		Percent = Agility / 150
	elseif (Agility >= 1501 and Agility < 2001) then
		Percent = 10 + (Agility - 1500) / 25
	elseif (Agility >= 2001 and Agility < 3000) then
		Percent = 30 + (Agility - 2000) / 66
	end
	
	return Percent
end

function StatSpec_AtkRatePvPInc_Wizard(Agility)
	local Percent = 0
	
	if (Agility > 1 and Agility < 1501) then
		Percent = Agility / 150
	elseif (Agility >= 1501 and Agility < 2001) then
		Percent = 10 + (Agility - 1500) / 25
	elseif (Agility >= 2001 and Agility < 3000) then
		Percent = 30 + (Agility - 2000) / 66
	end
	
	return Percent
end

function StatSpec_AtkPowerInc_Elf(Agility)
	local Percent = 0
	
	if (Agility > 1 and Agility < 1801) then
		Percent = Agility / 180
	elseif (Agility >= 1801 and Agility < 2101) then
		Percent = 10 + (Agility - 1800) / 15
	elseif (Agility >= 2101 and Agility < 3000) then
		Percent = 30 + (Agility - 2100) / 45
	end
	
	return Percent
end

function StatSpec_DefRatePvMInc_Elf(Agility)
	local Percent = 0
	
	if (Agility > 1 and Agility < 1801) then
		Percent = Agility / 180
	elseif (Agility >= 1801 and Agility < 2101) then
		Percent = 10 + (Agility - 1800) / 15
	elseif (Agility >= 2101 and Agility < 3000) then
		Percent = 30 + (Agility - 2100) / 45
	end
	
	return Percent
end

function StatSpec_DefRatePvPInc_Elf(Agility)
	local Percent = 0
	
	if (Agility > 1 and Agility < 1801) then
		Percent = Agility / 180
	elseif (Agility >= 1801 and Agility < 2101) then
		Percent = 10 + (Agility - 1800) / 15
	elseif (Agility >= 2101 and Agility < 3000) then
		Percent = 30 + (Agility - 2100) / 45
	end
	
	return Percent
end

function StatSpec_MagicPowerInc_Summoner(Energy)
	local Percent = 0
	
	if (Energy > 1 and Energy < 1501) then
		Percent = Energy / 150
	elseif (Energy >= 1501 and Energy < 2001) then
		Percent = 10 + (Energy - 1500) / 25
	elseif (Energy >= 2001 and Energy < 3000) then
		Percent = 30 + (Energy - 2000) / 66
	end
	
	return Percent
end

function StatSpec_CursePowerInc_Summoner(Energy)
	local Percent = 0
	
	if (Energy > 1 and Energy < 1501) then
		Percent = Energy / 150
	elseif (Energy >= 1501 and Energy < 2001) then
		Percent = 10 + (Energy - 1500) / 25
	elseif (Energy >= 2001 and Energy < 3000) then
		Percent = 30 + (Energy - 2000) / 66
	end
	
	return Percent
end

function StatSpec_AtkPowerInc_Gladiator(Strength)
	local Percent = 0
	
	if (Strength > 1 and Strength < 1501) then
		Percent = Strength / 150
	elseif (Strength >= 1501 and Strength < 2001) then
		Percent = 10 + (Strength - 1500) / 25
	elseif (Strength >= 2001 and Strength < 3000) then
		Percent = 30 + (Strength - 2000) / 66
	end
	
	-- LogAdd(string.format('Percent: %d', Percent))
	
	return Percent
end

function StatSpec_MagicPowerInc_Gladiator(Energy)
	local Percent = 0
	
	if (Energy > 1 and Energy < 1801) then
		Percent = Energy / 180
	elseif (Energy >= 1801 and Energy < 2101) then
		Percent = 10 + (Energy - 1500) / 15
	elseif (Energy >= 2101 and Energy < 3000) then
		Percent = 30 + (Energy - 2000) / 45
	end
	
	return Percent
end

function StatSpec_AtkPowerInc_Lord(Strength)
	local Percent = 0
	
	if (Strength > 1 and Strength < 1501) then
		Percent = Strength / 150
	elseif (Strength >= 1501 and Strength < 2001) then
		Percent = 10 + (Strength - 1500) / 25
	elseif (Strength >= 2001 and Strength < 3000) then
		Percent = 30 + (Strength - 2000) / 66
	end
	
	return Percent
end

function StatSpec_DefenseInc_Lord(Agility)
	local Percent = 0
	
	if (Agility > 1 and Agility < 1501) then
		Percent = Agility / 150
	elseif (Agility >= 1501 and Agility < 2001) then
		Percent = 10 + (Agility - 1500) / 25
	elseif (Agility >= 2001 and Agility < 3000) then
		Percent = 30 + (Agility - 2000) / 66
	end
	
	return Percent
end

function StatSpec_AtkPowerInc_Fighter(Strength)
	local Percent = 0
	
	if (Strength > 1 and Strength < 1501) then
		Percent = Strength / 150
	elseif (Strength >= 1501 and Strength < 2001) then
		Percent = 10 + (Strength - 1500) / 25
	elseif (Strength >= 2001 and Strength < 3000) then
		Percent = 30 + (Strength - 2000) / 66
	end
	
	return Percent
end
function StatSpec_AtkPowerInc_Lancer(Strength)
	local Percent = 0
	
	if (Strength > 1 and Strength < 1501) then
		Percent = Strength / 150
	elseif (Strength >= 1501 and Strength < 2001) then
		Percent = 10 + (Strength - 1500) / 25
	elseif (Strength >= 2001 and Strength < 3000) then
		Percent = 30 + (Strength - 2000) / 66
	end
	
	return Percent
end

function StatSpec_DefenseInc_Lancer(Agility)
	local Percent = 0
	
	if (Agility > 1 and Agility < 1501) then
		Percent = Agility / 150
	elseif (Agility >= 1501 and Agility < 2001) then
		Percent = 10 + (Agility - 1500) / 25
	elseif (Agility >= 2001 and Agility < 3000) then
		Percent = 30 + (Agility - 2000) / 66
	end
	
	return Percent
end	