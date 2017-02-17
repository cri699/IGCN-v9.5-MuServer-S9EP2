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


function SkillSpec_GetBonusValue(Class, SkillID, Strength, Dexterity, Vitality, Energy)
	local BonusVal = 0
	
	if (Class == CLASS_KNIGHT) then
		if (SkillID == 21 or SkillID == 22 or SkillID == 19 or SkillID == 20 or SkillID == 23 or SkillID == 326 or SkillID == 327 or SkillID == 328 or SkillID == 329) then
			BonusVal = SkillSpec_WeaponSkill_Knight(Strength)
			
		elseif (SkillID == 41 or SkillID == 330 or SkillID == 332) then
			BonusVal = SkillSpec_TwistingSlash_Knight(Strength)
		
		elseif (SkillID == 47) then
			BonusVal = SkillSpec_Impale_Knight(Strength)
		
		elseif (SkillID == 43 or SkillID == 336 or SkillID == 339 or SkillID == 342) then
			BonusVal = SkillSpec_DeathStab_Knight(Strength)
			
		elseif (SkillID == 42 or SkillID == 331 or SkillID == 333) then
			BonusVal = SkillSpec_RagefulBlow_Knight(Strength)
			
		elseif (SkillID == 232 or SkillID == 337 or SkillID == 340 or SkillID == 343) then
			BonusVal = SkillSpec_Destruction_Knight(Strength)
			
		elseif (SkillID == 344 or SkillID == 346) then
			BonusVal = SkillSpec_BloodStorm_Knight(Strength)
		
		end
	
	elseif (Class == CLASS_WIZARD) then
		if (SkillID == 17) then
			BonusVal = SkillSpec_EnergyBall_Wizard(Energy)
		
		elseif (SkillID == 4) then
			BonusVal = SkillSpec_FireBall_Wizard(Energy)
		
		elseif (SkillID == 11) then
			BonusVal = SkillSpec_PowerWave_Wizard(Energy)
			
		elseif (SkillID == 3 or SkillID == 379) then
			BonusVal = SkillSpec_Lightning_Wizard(Energy)
		
		elseif (SkillID == 2 or SkillID == 390 or SkillID == 394) then
			BonusVal = SkillSpec_Meteorite_Wizard(Energy)
			
		elseif (SkillID == 7 or SkillID == 389) then
			BonusVal = SkillSpec_Ice_Wizard(Energy)
			
		elseif (SkillID == 1 or SkillID == 384) then
			BonusVal = SkillSpec_Poison_Wizard(Energy)
			
		elseif (SkillID == 5 or SkillID == 378) then
			BonusVal = SkillSpec_Flame_Wizard(Energy)
			
		elseif (SkillID == 8) then
			BonusVal = SkillSpec_Twister_Wizard(Energy)
			
		elseif (SkillID == 9 or SkillID == 385) then
			BonusVal = SkillSpec_EvilSpirit_Wizard(Energy)
			
		elseif (SkillID == 10 or SkillID == 388) then
			BonusVal = SkillSpec_HellFire_Wizard(Energy)
			
		elseif (SkillID == 12) then
			BonusVal = SkillSpec_AquaBeam_Wizard(Energy)
			
		elseif (SkillID == 13 or SkillID == 382) then
			BonusVal = SkillSpec_CometFall_Wizard(Energy)
			
		elseif (SkillID == 14 or SkillID == 381) then
			BonusVal = SkillSpec_Inferno_Wizard(Energy)
			
		elseif (SkillID == 38 or SkillID == 387) then
			BonusVal = SkillSpec_DeathPoison_Wizard(Energy)
			
		elseif (SkillID == 39 or SkillID == 391 or SkillID == 393) then
			BonusVal = SkillSpec_IceStorm_Wizard(Energy)
			
		elseif (SkillID == 495 or SkillID == 497) then
			BonusVal = SkillSpec_FreezeEarth_Wizard(Energy)
			
		end
	
	elseif (Class == CLASS_ELF) then
		if (SkillID == 24 or SkillID == 414 or SkillID == 418) then
			BonusVal = SkillSpec_TripleShot_Elf(Dexterity)
			
		elseif (SkillID == 52 or SkillID == 416) then
			BonusVal = SkillSpec_Penetration_Elf(Dexterity)
			
		elseif (SkillID == 51 or SkillID == 424) then
			BonusVal = SkillSpec_IceArrow_Elf(Dexterity)
			
		elseif (SkillID == 235 or SkillID == 411 or SkillID == 431) then
			BonusVal = SkillSpec_MultiShot_Elf(Dexterity)
			
		elseif (SkillID == 427 or SkillID == 434) then
			BonusVal = SkillSpec_PoisonArrow_Elf(Dexterity)
			
		end
		
	elseif (Class == CLASS_GLADIATOR) then
		if (SkillID == 21 or SkillID == 22 or SkillID == 19 or SkillID == 20 or SkillID == 23 or SkillID == 479) then
			BonusVal = SkillSpec_WeaponSkill_Gladiator(Strength)	

		elseif (SkillID == 41 or SkillID == 481) then
			BonusVal = SkillSpec_TwistingSlash_Gladiator(Strength)
			
		elseif (SkillID == 55 or SkillID == 490 or SkillID == 493) then
			BonusVal = SkillSpec_BloodAttack_Gladiator(Strength)
			
		elseif (SkillID == 56 or SkillID == 482) then
			BonusVal = SkillSpec_PowerSlash_Gladiator(Strength)
			
		elseif (SkillID == 236 or SkillID == 492 or SkillID == 494) then
			BonusVal = SkillSpec_FlameStrike_Gladiator(Strength)
			
		elseif (SkillID == 17) then
			BonusVal = SkillSpec_EnergyBall_Gladiator(Energy)
		
		elseif (SkillID == 4) then
			BonusVal = SkillSpec_FireBall_Gladiator(Energy)
		
		elseif (SkillID == 11) then
			BonusVal = SkillSpec_PowerWave_Gladiator(Energy)
			
		elseif (SkillID == 3 or SkillID == 480) then
			BonusVal = SkillSpec_Lightning_Gladiator(Energy)
		
		elseif (SkillID == 2) then
			BonusVal = SkillSpec_Meteorite_Gladiator(Energy)
			
		elseif (SkillID == 7 or SkillID == 489 or SkillID == 491) then
			BonusVal = SkillSpec_Ice_Gladiator(Energy)
			
		elseif (SkillID == 1) then
			BonusVal = SkillSpec_Poison_Gladiator(Energy)
			
		elseif (SkillID == 5 or SkillID == 483) then
			BonusVal = SkillSpec_Flame_Gladiator(Energy)
			
		elseif (SkillID == 8) then
			BonusVal = SkillSpec_Twister_Gladiator(Energy)
			
		elseif (SkillID == 9 or SkillID == 487) then
			BonusVal = SkillSpec_EvilSpirit_Gladiator(Energy)
			
		elseif (SkillID == 10) then
			BonusVal = SkillSpec_HellFire_Gladiator(Energy)
			
		elseif (SkillID == 12) then
			BonusVal = SkillSpec_AquaBeam_Gladiator(Energy)
			
		elseif (SkillID == 13 or SkillID == 484) then
			BonusVal = SkillSpec_CometFall_Gladiator(Energy)
			
		elseif (SkillID == 14 or SkillID == 486) then
			BonusVal = SkillSpec_Inferno_Gladiator(Energy)
			
		elseif (SkillID == 495 or SkillID == 497) then
			BonusVal = SkillSpec_FreezeEarth_Gladiator(Energy)
			
		elseif (SkillID == 496) then
			BonusVal = SkillSpec_GiganticStorm_Gladiator(Energy)
		
		end
		
	elseif (Class == CLASS_DARKLORD) then
		if (SkillID == 21 or SkillID == 22 or SkillID == 19 or SkillID == 20) then
			BonusVal = SkillSpec_WeaponSkill_Lord(Strength)
			
		elseif (SkillID == 60) then
			BonusVal = SkillSpec_Force_Lord(Strength)
			
		elseif (SkillID == 66 or SkillID == 509) then
			BonusVal = SkillSpec_ForceWave_Lord(Strength)
			
		elseif (SkillID == 61 or SkillID == 508 or SkillID == 514) then
			BonusVal = SkillSpec_FireBurst_Lord(Strength)
			
		elseif (SkillID == 65 or SkillID == 519) then
			BonusVal = SkillSpec_ElectricSpark_Lord(Strength)
		
		elseif (SkillID == 78 or SkillID == 518 or SkillID == 520) then
			BonusVal = SkillSpec_FireScream_Lord(Strength)
			
		elseif (SkillID == 238 or SkillID == 523) then
			BonusVal = SkillSpec_ChaoticDiseier_Lord(Strength)
			
		end

	elseif (Class == CLASS_SUMMONER) then
		if (SkillID == 214 or SkillID == 458 or SkillID == 462) then
			BonusVal = SkillSpec_DrainLife_Summoner(Energy)
			
		elseif (SkillID == 215 or SkillID == 455) then
			BonusVal = SkillSpec_ChainLightning_Summoner(Energy)
			
		elseif (SkillID == 223) then
			BonusVal = SkillSpec_Explosion_Summoner(Energy)
			
		elseif (SkillID == 224) then
			BonusVal = SkillSpec_Requiem_Summoner(Energy)
			
		elseif (SkillID == 4) then
			BonusVal = SkillSpec_FireBall_Summoner(Energy)
		
		elseif (SkillID == 11) then
			BonusVal = SkillSpec_PowerWave_Summoner(Energy)
		
		elseif (SkillID == 2) then
			BonusVal = SkillSpec_Meteorite_Summoner(Energy)			
			
		elseif (SkillID == 225) then
			BonusVal = SkillSpec_Pollution_Summoner(Energy)
			
		elseif (SkillID == 230 or SkillID == 456) then
			BonusVal = SkillSpec_LightningShock_Summoner(Energy)
			
		end
		
	elseif (Class == CLASS_RAGEFIGHTER) then
		if (SkillID == 260 or SkillID == 551 or SkillID == 554) then
			BonusVal = SkillSpec_KillingBlow_Fighter(Vitality)
			
		elseif (SkillID == 261 or SkillID == 552 or SkillID == 555) then
			BonusVal = SkillSpec_BeastUppercut_Fighter(Vitality)
			
		elseif (SkillID == 270) then
			BonusVal = SkillSpec_PhoenixShot_Fighter(Vitality)
			
		end
	
	end
	
	return BonusVal
end

function SkillSpec_WeaponSkill_Knight(Strength)
	local BonusVal = Strength / 30
	return BonusVal
end

function SkillSpec_TwistingSlash_Knight(Strength)
	local BonusVal = Strength / 40
	return BonusVal
end

function SkillSpec_Impale_Knight(Strength)
	local BonusVal = 15 + (Strength / 35)
	return BonusVal
end

function SkillSpec_DeathStab_Knight(Strength)
	local BonusVal = 70 + (Strength / 150)
	return BonusVal
end

function SkillSpec_RagefulBlow_Knight(Strength)
	local BonusVal = 60 + (Strength / 150)
	return BonusVal
end

function SkillSpec_Destruction_Knight(Strength)
	local BonusVal = 110 + (Strength / 150)
	return BonusVal
end

function SkillSpec_BloodStorm_Knight(Strength)
	local BonusVal = 170 + (Strength / 200)
	return BonusVal
end

function SkillSpec_EnergyBall_Wizard(Energy)
	local BonusVal = 3 + (Energy / 30)
	return BonusVal
end

function SkillSpec_FireBall_Wizard(Energy)
	local BonusVal = 8 + (Energy / 30)
	return BonusVal
end

function SkillSpec_PowerWave_Wizard(Energy)
	local BonusVal = 14 + (Energy / 30)
	return BonusVal
end

function SkillSpec_Lightning_Wizard(Energy)
	local BonusVal = 17 + (Energy / 40)
	return BonusVal
end

function SkillSpec_Meteorite_Wizard(Energy)
	local BonusVal = 21 + (Energy / 45)
	return BonusVal
end

function SkillSpec_Ice_Wizard(Energy)
	local BonusVal = 10 + (Energy / 45)
	return BonusVal
end

function SkillSpec_Poison_Wizard(Energy)
	local BonusVal = 12 + (Energy / 50)
	return BonusVal
end

function SkillSpec_Flame_Wizard(Energy)
	local BonusVal = 25 + (Energy / 100)
	return BonusVal
end

function SkillSpec_Twister_Wizard(Energy)
	local BonusVal = 35 + (Energy / 90)
	return BonusVal
end

function SkillSpec_EvilSpirit_Wizard(Energy)
	local BonusVal = 45 + (Energy / 150)
	return BonusVal
end

function SkillSpec_HellFire_Wizard(Energy)
	local BonusVal = 120 + (Energy / 200)
	return BonusVal
end

function SkillSpec_AquaBeam_Wizard(Energy)
	local BonusVal = 80 + (Energy / 200)
	return BonusVal
end

function SkillSpec_CometFall_Wizard(Energy)
	local BonusVal = 70 + (Energy / 150)
	return BonusVal
end

function SkillSpec_Inferno_Wizard(Energy)
	local BonusVal = 100 + (Energy / 90)
	return BonusVal
end

function SkillSpec_IceStorm_Wizard(Energy)
	local BonusVal = 80 + (Energy / 90)
	return BonusVal
end

function SkillSpec_DeathPoison_Wizard(Energy)
	local BonusVal = 95 + (Energy / 100)
	return BonusVal
end

function SkillSpec_FreezeEarth_Wizard(Energy)
	local BonusVal = 150 + (Energy / 200)
	return BonusVal
end

function SkillSpec_TripleShot_Elf(Dexterity)
	local BonusVal = Dexterity / 100
	return BonusVal
end

function SkillSpec_Penetration_Elf(Dexterity)
	local BonusVal = 70 + (Dexterity / 200)
	return BonusVal
end

function SkillSpec_IceArrow_Elf(Dexterity)
	local BonusVal = 105 + (Dexterity / 150)
	return BonusVal
end

function SkillSpec_MultiShot_Elf(Dexterity)
	local BonusVal = 40 + (Dexterity / 200)
	return BonusVal
end

function SkillSpec_PoisonArrow_Elf(Dexterity)
	local BonusVal = 130 + (Dexterity / 100)
	return BonusVal
end

function SkillSpec_DrainLife_Summoner(Energy)
	local BonusVal = 35
	return BonusVal
end

function SkillSpec_ChainLightning_Summoner(Energy)
	local BonusVal = 70 + (Energy / 200)
	return BonusVal
end

function SkillSpec_Explosion_Summoner(Energy)
	local BonusVal = 40 + (Energy / 150)
	return BonusVal
end

function SkillSpec_Requiem_Summoner(Energy)
	local BonusVal = 65 + (Energy / 150)
	return BonusVal
end

function SkillSpec_FireBall_Summoner(Energy)
	local BonusVal = 8 + (Energy / 30)
	return BonusVal
end

function SkillSpec_PowerWave_Summoner(Energy)
	local BonusVal = 14 + (Energy / 30)
	return BonusVal
end

function SkillSpec_Meteorite_Summoner(Energy)
	local BonusVal = 21 + (Energy / 45)
	return BonusVal
end

function SkillSpec_Ice_Summoner(Energy)
	local BonusVal = 10 + (Energy / 45)
	return BonusVal
end

function SkillSpec_Pollution_Summoner(Energy)
	local BonusVal = 80 + (Energy / 150)
	return BonusVal
end

function SkillSpec_LightningShock_Summoner(Energy)
	local BonusVal = 95 + (Energy / 200)
	return BonusVal
end

function SkillSpec_WeaponSkill_Gladiator(Strength)
	local BonusVal = Strength / 30
	return BonusVal
end

function SkillSpec_TwistingSlash_Gladiator(Strength)
	local BonusVal = Strength / 40
	return BonusVal
end

function SkillSpec_BloodAttack_Gladiator(Strength)
	local BonusVal = 80 + (Strength / 150)
	return BonusVal
end

function SkillSpec_PowerSlash_Gladiator(Strength)
	local BonusVal = Strength / 100
	return BonusVal
end

function SkillSpec_FlameStrike_Gladiator(Strength)
	local BonusVal = 140 + (Strength / 200)
	return BonusVal
end

function SkillSpec_BloodStorm_Gladiator(Strength)
	local BonusVal = 170 + (Strength / 200)
	return BonusVal
end

function SkillSpec_EnergyBall_Gladiator(Energy)
	local BonusVal = 3 + (Energy / 30)
	return BonusVal
end

function SkillSpec_FireBall_Gladiator(Energy)
	local BonusVal = 8 + (Energy / 30)
	return BonusVal
end

function SkillSpec_PowerWave_Gladiator(Energy)
	local BonusVal = 14 + (Energy / 30)
	return BonusVal
end

function SkillSpec_Lightning_Gladiator(Energy)
	local BonusVal = 17 + (Energy / 40)
	return BonusVal
end

function SkillSpec_Meteorite_Gladiator(Energy)
	local BonusVal = 21 + (Energy / 45)
	return BonusVal
end

function SkillSpec_Ice_Gladiator(Energy)
	local BonusVal = 10 + (Energy / 45)
	return BonusVal
end

function SkillSpec_Poison_Gladiator(Energy)
	local BonusVal = 12 + (Energy / 50)
	return BonusVal
end

function SkillSpec_Flame_Gladiator(Energy)
	local BonusVal = 25 + (Energy / 100)
	return BonusVal
end

function SkillSpec_Twister_Gladiator(Energy)
	local BonusVal = 35 + (Energy / 90)
	return BonusVal
end

function SkillSpec_EvilSpirit_Gladiator(Energy)
	local BonusVal = 45 + (Energy / 150)
	return BonusVal
end

function SkillSpec_HellFire_Gladiator(Energy)
	local BonusVal = 120 + (Energy / 200)
	return BonusVal
end

function SkillSpec_AquaBeam_Gladiator(Energy)
	local BonusVal = 80 + (Energy / 200)
	return BonusVal
end

function SkillSpec_CometFall_Gladiator(Energy)
	local BonusVal = 70 + (Energy / 150)
	return BonusVal
end

function SkillSpec_Inferno_Gladiator(Energy)
	local BonusVal = 100 + (Energy / 90)
	return BonusVal
end

function SkillSpec_FreezeEarth_Gladiator(Energy)
	local BonusVal = 150 + (Energy / 200)
	return BonusVal
end

function SkillSpec_GiganticStorm_Gladiator(Energy)
	local BonusVal = 110 + (Energy / 250)
	return BonusVal
end

function SkillSpec_WeaponSkill_Lord(Strength)
	local BonusVal = Strength / 30
	return BonusVal
end

function SkillSpec_Force_Lord(Strength)
	local BonusVal = 10 + (Strength / 40)
	return BonusVal
end

function SkillSpec_ForceWave_Lord(Strength)
	local BonusVal = 50 + (Strength / 40)
	return BonusVal
end

function SkillSpec_FireBurst_Lord(Strength)
	local BonusVal = 100 + (Strength / 100)
	return BonusVal
end

function SkillSpec_ElectricSpark_Lord(Strength)
	local BonusVal = 250 + (Strength / 100)
	return BonusVal
end

function SkillSpec_FireScream_Lord(Strength)
	local BonusVal = 130 + (Strength / 150)
	return BonusVal
end

function SkillSpec_ChaoticDiseier_Lord(Strength)
	local BonusVal = 190 + (Strength / 150)
	return BonusVal
end

function SkillSpec_BeastUppercut_Fighter(Vitality)
	local BonusVal = Vitality / 200
	return BonusVal
end

function SkillSpec_KillingBlow_Fighter(Vitality)
	local BonusVal = Vitality / 200
	return BonusVal
end

function SkillSpec_PhoenixShot_Fighter(Vitality)
	local BonusVal = Vitality / 200
	return BonusVal
end