-- // ============================================================
-- // == INTERNATIONAL GAMING CENTER NETWORK
-- // == www.igc-network.com
-- // == (C) 2010-2014 IGC-Network (R)
-- // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
-- // == Modify if you know what you do only!
-- // == File is a part of IGCN Group MuOnline Server files.
-- // ============================================================

-- MasterSkillCalc Control Script, Lua v5.2
-- Regular and Master Experience formulas, - Can be modified to adjust for own needs

-- Regular Exp Table formula - used to calculate exp required for next regular level
function SetExpTable_Normal(Level)
	local Exp = 10 * Level * Level * (Level + 9);

	if ( Level > 255 ) then
		Exp = Exp + ( 1000 * (Level-255) * (Level-255) * ((Level-255) + 9))
	end
	
	return Exp
end

-- Master Exp Table formula - used to calculate exp required for next regular level
MaxNormalLevel = 400 
function SetExpTable_Master(MasterLevel)
	local TotalLevel = MasterLevel + MaxNormalLevel
	local Exp = (TotalLevel + 9) * TotalLevel * TotalLevel * 10
	
	Exp = Exp + ( ((TotalLevel - 255) + 9) * (TotalLevel - 255) * (TotalLevel - 255) * 1000)
	Exp = (Exp - 3892250000) / 2
	
	if(MasterLevel > 200) then
		Exp = Exp * ( (TotalLevel - 600) * (TotalLevel - 600) * 1.2 / 100000.0 + 1.0 )
	end
	
	return Exp
end