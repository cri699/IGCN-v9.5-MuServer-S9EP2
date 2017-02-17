-- // ============================================================
-- // == INTERNATIONAL GAMING CENTER NETWORK
-- // == www.igc-network.com
-- // == (C) 2010-2015 IGC-Network (R)
-- // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
-- // == File is a part of IGCN Group MuOnline Server files.
-- // ============================================================

-- MasterSkillPoint Control Script, Lua v5.2
-- Skill Tree Skill benefits per point - Can be modified to adjust for own needs

-- Formulas applies to Skill Tree Skill of specified SkillID specified at - Damage/STID column in IGC_SkillList.xml file

-- SkillID: 1
-- Index:  304, 306, 307, 308, 310, 311, 313, 323, 332, 333, 343, 354, 355, 380, 383, 393, 394, 431, 441, 452,
--			454, 463, 491, 514, 516, 522, 534, 538, 554, 555, 561, 562, 564, 571, 582, 584, 585, 586, 588, 589, 591
-- FN_LuaGetPercent(nPoint)

-- SkillID: 2
-- Index:	None by Default
-- FN_LuaGetPercent2(nPoint)

-- SkillID: 3
-- Index: 	315, 326, 350, 351, 384, 385, 407, 413, 416, 435, 436, 443, 448, 449, 450, 459, 460, 
--			466, 473, 479, 480, 481, 482, 483, 484, 487, 489, 508, 511, 519, 524, 526, 529, 593, 609
-- FN_LuaGetSkillDamageMin(nPoint)

-- SkillID: 4
-- Index:	327, 328, 329, 330, 348, 378, 379, 382, 390, 398, 509
-- FN_LuaGetSkillDamageMiddle(nPoint)

-- SkillID: 5
-- Index: 	352, 401, 439, 467, 520, 531, 556, 573
-- FN_LuaGetSkillDamageMax(nPoint)

-- SkillID: 6
-- Index:	359, 362, 606, 608
-- FN_LuaGetMonsterKill(nPoint)

-- SkillID: 7
-- Index:	303, 339, 340, 342, 345, 356, 360, 363, 403, 406, 451, 453, 469, 470, 493, 494, 517, 530, 581
-- FN_LuaGetPercent3(nPoint)

-- SkillID: 8
-- Index:	341
-- FN_LuaGetMaxAG(nPoint)

-- SkillID: 9
-- Index: 	334, 338
-- FN_LuaGetMaxLifeMana(nPoint)

-- SkillID: 10
-- Index:	399, 404, 433, 437, 472, 515, 528
-- FN_LuaGetAttack(nPoint)

-- SkillID: 11
-- Index:	358, 605
-- FN_LuaGetSDMonsterKill(nPoint)

-- SkillID: 12
-- Index:	301
-- FN_LuaGetBlockingRate(nPoint)

-- SkillID: 13
-- Index:	302, 325
-- FN_LuaGetAttackSuccessRate(nPoint)

-- SkillID: 14
-- Index:	347
-- FN_LuaGetManAttackRate(nPoint)

-- SkillID: 15
-- Index:	440
-- FN_LuaGetPercent4(nPoint)

-- SkillID: 16
-- Index:	309, 415, 419, 428
-- FN_LuaGetIncreaseBlocking(nPoint)

-- SkillID: 17
-- Index:	300, 305, 312, 317, 318, 319, 320, 322, 324, 375, 377, 402, 410, 412, 446,
--			447, 462, 476, 478, 505, 506, 507, 510, 532, 535, 549, 550, 578, 583, 590
-- FN_LuaGetIncreaseStat(nPoint)

-- SkillID: 18
-- Index:	357, 604
-- FN_LuaGetDecreaseMana(nPoint)

-- SkillID: 19
-- Index:	None by Default
-- FN_LuaGetChangeBlocking(nPoint)

-- SkillID: 20
-- Index:	533, 574
-- FN_LuaGetControlAttack(nPoint)

-- SkillID: 21
-- Index:	None by Default
-- FN_LuaGetManaIncRate(nPoint)

-- SkillID: 22
-- Index:	331, 334, 335, 336, 346, 349, 361, 381, 386, 387, 391, 392, 395, 397, 405, 411, 413, 417,
--			420, 421, 422, 423, 424, 429, 434, 442, 455, 456, 457, 458, 456, 471, 485, 486, 488, 490,
--			492, 496, 497, 511, 512, 518, 523, 527, 551, 552, 557, 558, 559, 560, 568, 569, 573, 607
-- FN_LuaGetSkillDamageMin2(nPoint)

-- SkillID: 23
-- Index:	353, 400, 438, 468, 536
-- FN_LuaGetAttackSpeed(nPoint)

-- SkillID: 24
-- Index:	None by Default
-- FN_LuaGetPercent5(nPoint)

-- SkillID: 25
-- Index:	344
-- FN_LuaGetBloodStorm(nPoint)

-- SkillID: 26
-- Index:	495
-- FN_LuaGetEarthPrison(nPoint)

-- SkillID: 27
-- Index:	427
-- FN_LuaGetPoisonArrow(nPoint)

-- SkillID: 28
-- Index:	521
-- FN_LuaGetIronDefense(nPoint)

-- SkillID: 29
-- Index:	579
-- FN_LuaGetBlockingRate_2(nPoint)

-- SkillID: 30
-- Index:	599
-- FN_LuaGetAttackRate_2(nPoint)

-- SkillID: 31
-- Index:	603
-- FN_LuaGetManAttack_2(nPoint)

-- SkillID: 32
-- Index:	None by Default
-- FN_LuaGetMaxSD_1(nPoint)

-- SkillID: 33
-- Index:	580
-- FN_LuaGetMaxSD_2(nPoint)

-- SkillID: 34
-- Index:	600, 601
-- FN_LuaGetMaxLifeMana_2(nPoint)

-- SkillID: 35
-- Index:	587
-- FN_LuaGetIncBlocking_2(nPoint)

-- SkillID: 36
-- Index:	595, 596, 597, 598
-- FN_LuaGetIncStat_2(nPoint)

-- SkillID: 37
-- Index:	602
-- FN_LuaGetMaxAG_2(nPoint)

-- SkillID: 38
-- Index:	314, 316, 321, 366, 367, 368, 369, 370, 371, 372, 373, 374, 409, 445, 475,
--			504, 539, 548, 566, 567, 592, 594, 610, 611, 612, 613, 614, 615, 616, 617
-- FN_LuaGetRate_6(nPoint)

-- SkillID: 39
-- Index:	None by Default
-- FN_LuaGetRate_6(nPoint)

 MLST_VALUE_NORMAL = 0 -- Regular Calc
 MLST_VALUE_DAMAGE = 1 -- Attack Damage Calc
 MLST_VALUE_MANA_INC = 2 -- Mana Use Calc

function MLS_ValueInit()
	AddToMLSTable(1, 'FN_LuaGetPercent', MLST_VALUE_NORMAL) -- 0
	AddToMLSTable(2, 'FN_LuaGetPercent2', MLST_VALUE_NORMAL) -- 0
	AddToMLSTable(3, 'FN_LuaGetSkillDamageMin', MLST_VALUE_DAMAGE) -- 1 
	AddToMLSTable(4, 'FN_LuaGetSkillDamageMiddle', MLST_VALUE_DAMAGE) -- 1
	AddToMLSTable(5, 'FN_LuaGetSkillDamageMax', MLST_VALUE_DAMAGE) -- 1
	AddToMLSTable(6, 'FN_LuaGetMonsterKill', MLST_VALUE_NORMAL) -- 0
	AddToMLSTable(7, 'FN_LuaGetPercent3', MLST_VALUE_NORMAL) -- 0
	AddToMLSTable(8, 'FN_LuaGetMaxAG', MLST_VALUE_NORMAL) -- 0
	AddToMLSTable(9, 'FN_LuaGetMaxLifeMana', MLST_VALUE_NORMAL) -- 0
	AddToMLSTable(10, 'FN_LuaGetAttack', MLST_VALUE_NORMAL) -- 0
	AddToMLSTable(11, 'FN_LuaGetSDMonsterKill', MLST_VALUE_NORMAL) -- 0
	AddToMLSTable(12, 'FN_LuaGetBlockingRate', MLST_VALUE_NORMAL) -- 0
	AddToMLSTable(13, 'FN_LuaGetAttackSuccessRate', MLST_VALUE_NORMAL) -- 0
	AddToMLSTable(14, 'FN_LuaGetManAttackRate', MLST_VALUE_NORMAL) -- 0
	AddToMLSTable(15, 'FN_LuaGetPercent4', MLST_VALUE_NORMAL) -- 0
	AddToMLSTable(16, 'FN_LuaGetIncreaseBlocking', MLST_VALUE_NORMAL) -- 0
	AddToMLSTable(17, 'FN_LuaGetIncreaseStat', MLST_VALUE_NORMAL) -- 0
	AddToMLSTable(18, 'FN_LuaGetDecreaseMana', MLST_VALUE_NORMAL) -- 0
	AddToMLSTable(19, 'FN_LuaGetChangeBlocking', MLST_VALUE_NORMAL) -- 0
	AddToMLSTable(20, 'FN_LuaGetControlAttack', MLST_VALUE_NORMAL) -- 0
	AddToMLSTable(21, 'FN_LuaGetManaIncRate', MLST_VALUE_MANA_INC) -- 2
	AddToMLSTable(22, 'FN_LuaGetSkillDamageMin2', MLST_VALUE_DAMAGE) -- 1
	AddToMLSTable(23, 'FN_LuaGetAttackSpeed', MLST_VALUE_NORMAL) -- 0
	AddToMLSTable(24, 'FN_LuaGetPercent5', MLST_VALUE_NORMAL) -- 0
	AddToMLSTable(25, 'FN_LuaGetBloodStorm', MLST_VALUE_NORMAL) -- 0
	AddToMLSTable(26, 'FN_LuaGetEarthPrison', MLST_VALUE_NORMAL) -- 0
	AddToMLSTable(27, 'FN_LuaGetPoisonArrow', MLST_VALUE_NORMAL) -- 0
	AddToMLSTable(28, 'FN_LuaGetIronDefense', MLST_VALUE_NORMAL) -- 0
	AddToMLSTable(29, 'FN_LuaGetBlockingRate_2', MLST_VALUE_NORMAL) -- 0
	AddToMLSTable(30, 'FN_LuaGetAttackRate_2', MLST_VALUE_NORMAL) -- 0
	AddToMLSTable(31, 'FN_LuaGetManAttack_2', MLST_VALUE_NORMAL) -- 0
	AddToMLSTable(32, 'FN_LuaGetMaxSD_1', MLST_VALUE_NORMAL) -- 0
	AddToMLSTable(33, 'FN_LuaGetMaxSD_2', MLST_VALUE_NORMAL) -- 0
	AddToMLSTable(34, 'FN_LuaGetMaxLifeMana_2', MLST_VALUE_NORMAL) -- 0
	AddToMLSTable(35, 'FN_LuaGetIncBlocking_2', MLST_VALUE_NORMAL) -- 0
	AddToMLSTable(36, 'FN_LuaGetIncStat_2', MLST_VALUE_NORMAL) -- 0
	AddToMLSTable(37, 'FN_LuaGetMaxAG_2', MLST_VALUE_NORMAL) -- 0
	AddToMLSTable(38, 'FN_LuaGetRate_6', MLST_VALUE_NORMAL) -- 0
	AddToMLSTable(39, 'FN_LuaGetRate_7', MLST_VALUE_NORMAL) -- 0
end

--//////////////////////////////////////////////////////////////////////////////
-- SkillID: 1
--//////////////////////////////////////////////////////////////////////////////
function FN_LuaGetPercent(nPoint)
	local nReturn = 1 + ((((((((nPoint) - 30) ^ 3) + 25000) / 499) / 50) * 100) / 12)
	return nReturn
end

--//////////////////////////////////////////////////////////////////////////////
-- SkillID: 2
--//////////////////////////////////////////////////////////////////////////////
function FN_LuaGetPercent2(nPoint)
	local nReturn = 0.8 + (((((((nPoint - 30) ^ 3) + 25000) / 499) / 6)))
	return nReturn
end

--//////////////////////////////////////////////////////////////////////////////
-- SkillID: 3
--//////////////////////////////////////////////////////////////////////////////
function FN_LuaGetSkillDamageMin(nPoint)
	local nReturn = (0.85 + (((((((nPoint - 30) ^ 3) + 25000) / 499) / 6)))) * 6
	return nReturn

end

--//////////////////////////////////////////////////////////////////////////////
-- SkillID: 4
--//////////////////////////////////////////////////////////////////////////////
function FN_LuaGetSkillDamageMiddle(nPoint)
	local nReturn = (0.9 + (((((((nPoint - 30) ^ 3) + 25000) / 499) / 6)))) * 8
	return nReturn

end

--//////////////////////////////////////////////////////////////////////////////
-- SkillID: 5
--//////////////////////////////////////////////////////////////////////////////
function FN_LuaGetSkillDamageMax(nPoint)
	local nReturn = (0.95 + (((((((nPoint - 30) ^ 3) + 25000) / 499) / 6)))) * 10
	return nReturn

end

--//////////////////////////////////////////////////////////////////////////////
-- SkillID: 6
--//////////////////////////////////////////////////////////////////////////////
function FN_LuaGetMonsterKill(nPoint)
	local nReturn =  52 / (1 + (((((((nPoint - 30) ^ 3) + 25000) / 499) / 6))))
	return nReturn

end

--//////////////////////////////////////////////////////////////////////////////
-- SkillID: 7
--//////////////////////////////////////////////////////////////////////////////
function FN_LuaGetPercent3(nPoint)
	local nReturn =  (1 + (((((((nPoint - 30) ^ 3) + 25000) / 499) / 6)))) * 1.5
	return nReturn
end

--//////////////////////////////////////////////////////////////////////////////
-- SkillID: 8
--//////////////////////////////////////////////////////////////////////////////
function FN_LuaGetMaxAG(nPoint)
	local nReturn = (1 + (((((((nPoint - 30) ^ 3) + 25000) / 499) / 50) * 100) / 12)) * 23
	return nReturn
end

--//////////////////////////////////////////////////////////////////////////////
-- SkillID: 9
--//////////////////////////////////////////////////////////////////////////////
function FN_LuaGetMaxLifeMana(nPoint)
	local nReturn = (1 + (((((((nPoint - 30) ^ 3) + 25000) / 499) / 50) * 100) / 12)) * 85
	return nReturn
end

--//////////////////////////////////////////////////////////////////////////////
-- SkillID: 10
--//////////////////////////////////////////////////////////////////////////////
function FN_LuaGetAttack(nPoint)
	local nReturn = (0.8 + (((((((nPoint - 30) ^ 3) + 25000) / 499) / 50) * 100) / 12)) * 8
	return nReturn
end

--//////////////////////////////////////////////////////////////////////////////
-- SkillID: 11
--//////////////////////////////////////////////////////////////////////////////
function FN_LuaGetSDMonsterKill(nPoint)
	local nReturn = 11 / (1 + (((((((nPoint - 30) ^ 3) + 25000) / 499) / 50) * 100) / 12))
	return nReturn
end


--//////////////////////////////////////////////////////////////////////////////
-- SkillID: 12
--//////////////////////////////////////////////////////////////////////////////
function FN_LuaGetBlockingRate(nPoint)
	local nReturn = (1 + (((((((nPoint - 30) ^ 3) + 25000) / 499) / 50) * 100) / 12)) * 85 * 6
	return nReturn
end

--//////////////////////////////////////////////////////////////////////////////
-- SkillID: 13
--//////////////////////////////////////////////////////////////////////////////
function FN_LuaGetAttackSuccessRate(nPoint)
	local nReturn = (1 + (((((((nPoint - 30) ^ 3) + 25000) / 499) / 50) * 100) / 12)) * 85 * 5
	return nReturn
end

--//////////////////////////////////////////////////////////////////////////////
-- SkillID: 14
--//////////////////////////////////////////////////////////////////////////////
function FN_LuaGetManAttackRate(nPoint)
	local nReturn = (1 + (((((((nPoint - 30) ^ 3) + 25000) / 499) / 50) * 100) / 12)) * 85 * 8
	return nReturn
end

--//////////////////////////////////////////////////////////////////////////////
-- SkillID: 15
--//////////////////////////////////////////////////////////////////////////////
function FN_LuaGetPercent4(nPoint)
	local nReturn = (1 + (((((((nPoint - 30) ^ 3) + 25000) / 499) / 6)))) * 15
	return nReturn
end

--//////////////////////////////////////////////////////////////////////////////
-- SkillID: 16
--//////////////////////////////////////////////////////////////////////////////
function FN_LuaGetIncreaseBlocking(nPoint)
	local nReturn = (1 + (((((((nPoint - 30) ^ 3) + 25000) / 499) / 6)))) * 50
	return nReturn
end

--//////////////////////////////////////////////////////////////////////////////
-- SkillID: 17
--//////////////////////////////////////////////////////////////////////////////
function FN_LuaGetIncreaseStat(nPoint)
	local nReturn = (1 + (((((((nPoint - 30) ^ 3) + 25000) / 499) / 6)))) * 10
	return nReturn
end

--//////////////////////////////////////////////////////////////////////////////
-- SkillID: 18
--//////////////////////////////////////////////////////////////////////////////
function FN_LuaGetDecreaseMana(nPoint)
	local nReturn = (1 + (((((((nPoint - 30) ^ 3) + 25000) / 499) / 6)))) * 6
	return nReturn
end

--//////////////////////////////////////////////////////////////////////////////
-- SkillID: 19
--//////////////////////////////////////////////////////////////////////////////
function FN_LuaGetChangeBlocking(nPoint)
	local nReturn = 50 / (1 + (((((((nPoint - 30) ^ 3) + 25000) / 499) / 6)))) + 20
	return nReturn
end

--//////////////////////////////////////////////////////////////////////////////
-- SkillID: 20
--//////////////////////////////////////////////////////////////////////////////
function FN_LuaGetControlAttack(nPoint)
	local nReturn = 40 / (1 + (((((((nPoint - 30) ^ 3) + 25000) / 499) / 6)))) + 5
	return nReturn
end

--//////////////////////////////////////////////////////////////////////////////
-- SkillID: 21
--//////////////////////////////////////////////////////////////////////////////
function FN_LuaGetManaIncRate(nPoint)
	local nReturn = (2 + (((((((nPoint - 30 ) ^ 3) + 25000) / 499 ) / 6)))) * 1.5
	return nReturn
end


--///////////////////////////////////////////////////////////////////////////////
-- SkillID: 22
--///////////////////////////////////////////////////////////////////////////////
function FN_LuaGetSkillDamageMin2(nPoint)
	local nReturn = (0.8 + (((((((nPoint - 30) ^ 3) + 25000) / 499) / 6)))) * 5
	return nReturn
end

--///////////////////////////////////////////////////////////////////////////////
-- SkillID: 23
--///////////////////////////////////////////////////////////////////////////////
function FN_LuaGetAttackSpeed(nPoint)
	local nReturn = nPoint * 1.0
	return nReturn
end

--///////////////////////////////////////////////////////////////////////////////
-- SkillID: 24
--///////////////////////////////////////////////////////////////////////////////
function FN_LuaGetPercent5(nPoint)
	local nReturn = (0.95 + ( ( ( ( (((nPoint - 30) ^ 3) + 25000) / 499) / 6)))) * 7.8
	return nReturn
end


--///////////////////////////////////////////////////////////////////////////////
-- SkillID: 25
--///////////////////////////////////////////////////////////////////////////////
function FN_LuaGetBloodStorm(nPoint)
	local nReturn = 170
	return nReturn
end

--///////////////////////////////////////////////////////////////////////////////
-- SkillID: 26
--///////////////////////////////////////////////////////////////////////////////
function FN_LuaGetEarthPrison(nPoint)
	local nReturn = 150
	return nReturn
end

--///////////////////////////////////////////////////////////////////////////////
-- SkillID: 27
--///////////////////////////////////////////////////////////////////////////////
function FN_LuaGetPoisonArrow(nPoint)
	local nReturn = 130
	return nReturn
end

--///////////////////////////////////////////////////////////////////////////////
-- SkillID: 28
--///////////////////////////////////////////////////////////////////////////////
function FN_LuaGetIronDefense(nPoint)
	local nReturn = 800
	return nReturn
end

--///////////////////////////////////////////////////////////////////////////////
-- SkillID: 29
--///////////////////////////////////////////////////////////////////////////////
function  FN_LuaGetBlockingRate_2(nPoint)
	local nReturn = (1 + ( ( ( ( ( ( (nPoint - 30) ^ 3) + 25000) / 499) / 50) * 100) / 12)) * 85 * 2.5
	return nReturn
end

--///////////////////////////////////////////////////////////////////////////////
-- SkillID: 30
--///////////////////////////////////////////////////////////////////////////////
function FN_LuaGetAttackRate_2(nPoint)
	local nReturn = (1 + ( ( ( ( ( ( (nPoint - 30) ^ 3) + 25000) / 499) / 50) * 100) / 12) ) * 85 * 2
	return nReturn
end

--///////////////////////////////////////////////////////////////////////////////
-- SkillID: 31
--///////////////////////////////////////////////////////////////////////////////
function FN_LuaGetManAttack_2(nPoint)
	local nReturn = (1 + ( ( ( ( ( ( (nPoint - 30) ^ 3) + 25000) / 499) / 50) * 100) / 12)) * 85 * 3.2
	return nReturn
end

--///////////////////////////////////////////////////////////////////////////////
-- SkillID: 32
--///////////////////////////////////////////////////////////////////////////////
function FN_LuaGetMaxSD_1(nPoint)
	local nReturn = (1 + ( ( ( ( ( ((nPoint - 30) ^ 3) + 25000) / 499) / 50) * 100) / 12)) * 85 * 5
	return nReturn
end

--///////////////////////////////////////////////////////////////////////////////
-- SkillID: 33
--///////////////////////////////////////////////////////////////////////////////
function FN_LuaGetMaxSD_2(nPoint)
	local nReturn = (1 + ( ( ( ( ( ((nPoint - 30) ^ 3) + 25000) / 499) / 50) * 100) / 12)) * 85 * 3
	return nReturn
end

--///////////////////////////////////////////////////////////////////////////////
-- SkillID: 34
--///////////////////////////////////////////////////////////////////////////////
function FN_LuaGetMaxLifeMana_2(nPoint)
	local nReturn = (1 + ( ( ( ( ( ( (nPoint - 30) ^ 3) + 25000) / 499) / 50) * 100) / 12)) * 45
	return nReturn
end

--///////////////////////////////////////////////////////////////////////////////
-- SkillID: 35
--///////////////////////////////////////////////////////////////////////////////
function FN_LuaGetIncBlocking_2(nPoint)
	local nReturn = (1 + ( ( ( ( ( ( (nPoint - 30) ^ 3) + 25000) / 499) / 6) ) ) ) * 28
	return nReturn
end

--///////////////////////////////////////////////////////////////////////////////
-- SkillID: 36
--///////////////////////////////////////////////////////////////////////////////
function FN_LuaGetIncStat_2(nPoint)
	local nReturn = (1 + ( ( ( ( ( ( (nPoint - 30) ^ 3) + 25000) / 499) / 6) ) ) ) * 8
	return nReturn
end

--///////////////////////////////////////////////////////////////////////////////
-- SkillID: 37
--///////////////////////////////////////////////////////////////////////////////
function FN_LuaGetMaxAG_2(nPoint)
	local nReturn = (1 + ( ( ( ( ( ( (nPoint - 30) ^ 3) + 25000) / 499) / 50) * 100) / 12) ) * 15
	return nReturn
end

--///////////////////////////////////////////////////////////////////////////////
-- SkillID: 38
--///////////////////////////////////////////////////////////////////////////////
function FN_LuaGetRate_6(nPoint)
	local nReturn = 0.94 + ( ( ( ( ( ( (nPoint - 30) ^ 3) + 25000) / 499) / 20) ) )
	return nReturn
end

--///////////////////////////////////////////////////////////////////////////////
-- SkillID: 39
--///////////////////////////////////////////////////////////////////////////////
function FN_LuaGetRate_7(nPoint)
	local nReturn = 0.9 + ( ( ( ( ( ( (nPoint - 30) ^ 3) + 25000) / 499) / 12) ) )
	return nReturn
end