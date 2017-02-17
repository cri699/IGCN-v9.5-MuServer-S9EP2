-- Character Classes
CLASS_WIZARD                                          = 0	-- Fairy Elf, Muse Elf, High Elf
CLASS_KNIGHT                                          = 1	-- Dark Wizard, Soul Master, Grand Master
CLASS_ELF                                             = 2	-- Dark Knight, Blade Knight, Blade Master
CLASS_GLADIATOR                                       = 3	-- Magic Gladiator, Duel Master
CLASS_DARKLORD                                        = 4	-- Dark Lord, Lord Emperor
CLASS_SUMMONER                                        = 5	-- Summoner, Bloody Summoner, Dimension Master
CLASS_RAGEFIGHTER                                     = 6	-- Rage Fighter, Fist Master
CLASS_GROWLANCER									  = 7	-- Grow Lancer, Mirage Lancer



function MakeItemID(ItemType, ItemIndex)
	return ItemType * 512 + ItemIndex;
end

function Wings_CalcAbsorb(AttackDamage, WingID, WingLevel, MLS_WingOption)
	local OutDamage = AttackDamage
	
	if ( (WingID > MakeItemID(12,2) and WingID < MakeItemID(12,7)) or WingID == MakeItemID(12,42)) then
		OutDamage = AttackDamage * (75 - WingLevel * 2) / 100
	elseif (WingID == MakeItemID(12,49)) then
		OutDamage = AttackDamage * (90 - WingLevel * 2) / 100
	elseif (WingID == MakeItemID(13,30)) then
		OutDamage = AttackDamage * (90 - WingLevel) / 100
	elseif ( (WingID > MakeItemID(12,35) and WingID < MakeItemID(12,40)) or WingID == MakeItemID(12,43) or WingID == MakeItemID(12,50)) then
		OutDamage = (AttackDamage * (61 - WingLevel * 2) - MLS_WingOption) / 100
	elseif (WingID == MakeItemID(12,40)) then
		OutDamage = (AttackDamage * (76 - WingLevel * 2) - MLS_WingOption) / 100
	elseif (WingID == MakeItemID(12,264) or WingID == MakeItemID(12,265)) then
		OutDamage = AttackDamage * (71 - WingLevel * 2) / 100
	elseif (WingID == MakeItemID(12,262)) then
		OutDamage = AttackDamage * (87 - WingLevel * 2) / 100
	elseif (WingID == MakeItemID(12,263)) then
		OutDamage = AttackDamage * (70 - WingLevel * 2) / 100
	elseif (WingID == MakeItemID(12,266)) then
		OutDamage = AttackDamage * (20 - WingLevel) / 100
	elseif (WingID == MakeItemID(12,267)) then
		OutDamage = AttackDamage * (25 - WingLevel) / 100
	elseif (WingID == MakeItemID(12,268)) then
		OutDamage = AttackDamage * (61 - WingLevel * 2) / 100
	else
		OutDamage = AttackDamage * (88 - WingLevel * 2) / 100
	end
	
	return OutDamage
end

function Wings_CalcIncAttack(AttackDamage, WingID, WingLevel)
	local OutDamage = AttackDamage
	
	if (WingID == MakeItemID(13,30)) then
		OutDamage = AttackDamage * (WingLevel * 2 + 120) / 100
	elseif (WingID == MakeItemID(12,41)) then
		OutDamage = AttackDamage * (WingLevel * 2 + 112) / 100
	elseif (WingID == MakeItemID(12,42)) then
		OutDamage = AttackDamage * (WingLevel + 132) / 100
	elseif (WingID == MakeItemID(12,43)) then
		OutDamage = AttackDamage * (WingLevel * 2 + 139) / 100
	elseif (WingID > MakeItemID(12,2) and WingID < MakeItemID(12,7)) then
		OutDamage = AttackDamage * (WingLevel + 132) / 100
	elseif (WingID > MakeItemID(12,35) and WingID < MakeItemID(12,41)) then
		OutDamage = AttackDamage * (WingLevel * 2 + 139) / 100
	elseif (WingID == MakeItemID(12,49)) then
		OutDamage = AttackDamage * (WingLevel * 2 + 120) / 100
	elseif (WingID == MakeItemID(12,50)) then
		OutDamage = AttackDamage * (WingLevel * 2 + 139) / 100
	elseif (WingID == MakeItemID(12,264) or WingID == MakeItemID(12,265)) then
		OutDamage = AttackDamage * (WingLevel + 135) / 100
	elseif (WingID == MakeItemID(12,262)) then
		OutDamage = AttackDamage * (WingLevel + 121) / 100
	elseif (WingID == MakeItemID(12,263)) then
		OutDamage = AttackDamage * (WingLevel + 133) / 100
	elseif (WingID == MakeItemID(12,266)) then
		OutDamage = AttackDamage * (WingLevel + 180) / 100
	elseif (WingID == MakeItemID(12,267)) then
		OutDamage = AttackDamage * (WingLevel + 175) / 100
	elseif (WingID == MakeItemID(12,268)) then
		OutDamage = AttackDamage * (WingLevel * 2 + 139) / 100
	else
		OutDamage = AttackDamage * (WingLevel * 2 + 112) / 100
	end
	
	return OutDamage
end

function Wings_DecreaseHP(WingID, WingDur, Class, Skill)
	local HPDec = 0
	
	if (WingID == MakeItemID(12,266) and Class ~= CLASS_RAGEFIGHTER and WingDur > 0) then
		HPDec = 3
	elseif (Class == CLASS_WIZARD or Class == CLASS_ELF or Class == CLASS_SUMMONER) then
		HPDec = 1
	elseif (Class == CLASS_RAGEFIGHTER) then
		if (Skill == 263 or Skill == 559 or Skill == 563 or Skill == 269 or Skill == 262 or Skill == 558 or Skill == 562) then
			HPDec = 4
		elseif (Skill == 265 or Skill == 564 or Skill == 566) then
			HPDec = 100
		elseif (Skill == 260 or Skill == 261 or Skill == 264 or Skill == 551 or Skill == 554 or Skill == 552 or Skill == 555 or Skill == 560 or Skill == 561 or Skill == 270) then
			HPDec = 2
		else
			HPDec = 3
		end
	else
		HPDec = 3
	end
	
	return HPDec
end
	
function DarkSpirit_CalcValues(Command, PetLevel)
	local AttackDamageMin = PetLevel * 15 + Command / 8 + 180
	local AttackDamageMax = PetLevel * 15 + Command / 4 + 200
	
	local CriticalDamage = 30
	local ExcellentDamage = Command / 200 + 3 + PetLevel / 20
	local DoubleDamage = Command / 300 + 2 + PetLevel / 30
	local IgnoreEnemy = Command / 400 + 1 + PetLevel / 40
	
	local AttackSpeed = PetLevel * 4 / 5 + Command / 50 + 20
	local SuccessAtkRate = PetLevel + PetLevel / 15 + 1000
	
	return AttackDamageMin, AttackDamageMax, CriticalDamage, ExcellentDamage, DoubleDamage, IgnoreEnemy, AttackSpeed, SuccessAtkRate
end

function GoldHellMaine_DropWing()
	local WingDropRate = GetLargeRandomValue(1000000)
	local WingCountRate = GetRandomValue(100)
	local WingCount = 0
	local WingExpireTime = 2419200 -- seconds
	
	if (WingDropRate < 50000) then
		if (WingCountRate >= 75) then
			if (WingCountRate >= 90) then
				if (WingCountRate >= 95) then
					if(WingCountRate >= 98) then
						WingCount = 5
					else
						WingCount = 4
					end
				else
					WingCount = 3
				end
			else
				WingCount = 2
			end
		else
			WingCount = 1
		end
	else
		WingCount = 0
	end
	
	return WingCount, WingExpireTime
end

function GoldKundun_DropWing()
	local WingDropRate = GetLargeRandomValue(1000000)
	local WingCountRate = GetRandomValue(100)
	local WingCount = 0
	local WingExpireTime = 2419200 -- seconds
	
	if (WingDropRate < 200000) then
		if (WingCountRate >= 75) then
			if (WingCountRate >= 90) then
				if (WingCountRate >= 95) then
					if(WingCountRate >= 98) then
						WingCount = 5
					else
						WingCount = 4
					end
				else
					WingCount = 3
				end
			else
				WingCount = 2
			end
		else
			WingCount = 1
		end
	else
		WingCount = 0
	end
	
	return WingCount, WingExpireTime
end
		