--[[
Quest_Main_Quest_GiveUp.lua
[ Start  ] 	2009.08.
[ Latest ]	2012-10-08 5:19 PM
[ Author ]	김정민
[ Version ]	0.0.0.6
[ Text ]	QS 72번 퀘스트 포기 시 QS가 64번으로 초기화 되는 현상 수정
[ Line ]	EP 3		
--]]
--================================================================================================================
function EP1_GiveUp_Level_check(iObjIndex, Ep, QS)
		if QS == 2 then
			LuaQuestClass:SetQuestSwitch(Ep1, 0, iObjIndex)
		elseif QS == 9 then
			LuaQuestClass:SetQuestSwitch(Ep1, 6, iObjIndex)
		elseif QS == 15 then
			LuaQuestClass:SetQuestSwitch(Ep1, 10, iObjIndex)
		elseif QS == 20 then
			LuaQuestClass:SetQuestSwitch(Ep1, 16, iObjIndex)
		elseif QS == 23 or QS == 24 or QS == 25 then
			LuaQuestClass:SetQuestSwitch(Ep1, 21, iObjIndex)
		elseif QS == 29 then
			LuaQuestClass:SetQuestSwitch(Ep1, 26, iObjIndex)
		elseif QS == 31 then
			LuaQuestClass:SetQuestSwitch(Ep1, 30, iObjIndex)
		elseif QS == 33 or QS == 34 or QS == 35 then
			LuaQuestClass:SetQuestSwitch(Ep1, 32, iObjIndex)
		elseif QS > 39 and QS < 60 then
			LuaQuestClass:SetQuestSwitch(Ep1, 60, iObjIndex)
		elseif QS == 62 then
			LuaQuestClass:SetQuestSwitch(Ep1, 60, iObjIndex)
		elseif QS == 67 or QS == 68 then
			LuaQuestClass:SetQuestSwitch(Ep1, 63, iObjIndex)
		elseif QS == 71 or QS == 72 then
			LuaQuestClass:SetQuestSwitch(Ep1, 70, iObjIndex)
		elseif QS == 74 or QS == 75 then
			LuaQuestClass:SetQuestSwitch(Ep1, 73, iObjIndex)
		elseif QS == 77 or QS == 78 then
			LuaQuestClass:SetQuestSwitch(Ep1, 76, iObjIndex)
		elseif QS == 80 or QS == 81 then
			LuaQuestClass:SetQuestSwitch(Ep1, 79, iObjIndex)
		elseif QS == 83 or QS == 84 then
			LuaQuestClass:SetQuestSwitch(Ep1, 82, iObjIndex)
		elseif QS == 90 then
			LuaQuestClass:SetQuestSwitch(Ep1, 255, iObjIndex)
		end
end
--================================================================================================================
function EP2_GiveUp_Level_check(iObjIndex, Ep, QS)
	local CharMaster = LuaQuestClass:IsMasterLevel(iObjIndex) --마스터 레벨인지 판별
	local UserLv = LuaQuestClass:GetUserLv(iObjIndex)
	local CharClass = LuaQuestClass:GetCharClass(iObjIndex)
	
	if CharClass == CLASS_KNIGHT or CharClass == CLASS_WIZARD or CharClass == CLASS_ELF or CharClass == CLASS_SUMMONER then	-- 흑기사, 흑마법사, 요정, 소환술사
		if CharMaster == 0 then
			if UserLv >= 15 and UserLv <= 49 then
				if QS >= 1 and QS <= 8 then
					LuaQuestClass:SetQuestSwitch(Ep2, 0, iObjIndex)
				elseif QS >= 9 and QS <= 11 then
					LuaQuestClass:SetQuestSwitch(Ep2, 9, iObjIndex)
				elseif QS >= 12 and QS <= 14 then
					LuaQuestClass:SetQuestSwitch(Ep2, 12, iObjIndex)
				elseif QS >= 15 and QS <= 17 then
					LuaQuestClass:SetQuestSwitch(Ep2, 15, iObjIndex)
				end
			elseif UserLv >= 50 and UserLv <= 119 then

				if QS < 18 then
					LuaQuestClass:SetQuestSwitch(Ep2, 18, iObjIndex)
				elseif QS >= 18 and QS <= 26 then
					LuaQuestClass:SetQuestSwitch(Ep2, 18, iObjIndex)
				elseif QS >= 27 and QS <= 29 then
					LuaQuestClass:SetQuestSwitch(Ep2, 27, iObjIndex)
				elseif QS >= 30 and QS <= 32 then
					LuaQuestClass:SetQuestSwitch(Ep2, 30, iObjIndex)
				elseif QS >= 33 and QS <= 35 then
					LuaQuestClass:SetQuestSwitch(Ep2, 33, iObjIndex)
				end


			elseif UserLv >= 120 and UserLv <= 179 then

				if QS < 36 then
					LuaQuestClass:SetQuestSwitch(Ep2, 36, iObjIndex)
				elseif QS >= 36 and QS <= 39 then
					LuaQuestClass:SetQuestSwitch(Ep2, 36, iObjIndex)
				elseif QS >= 40 and QS <= 42 then
					LuaQuestClass:SetQuestSwitch(Ep2, 40, iObjIndex)
				elseif QS >= 43 and QS <= 45 then
					LuaQuestClass:SetQuestSwitch(Ep2, 43, iObjIndex)
				end

			elseif UserLv >= 180 and UserLv <= 239 then

				if QS < 46 then
					LuaQuestClass:SetQuestSwitch(Ep2, 46, iObjIndex)
				elseif QS >= 46 and QS <= 48 then
					LuaQuestClass:SetQuestSwitch(Ep2, 46, iObjIndex)
				elseif QS >= 49 and QS <= 51 then
					LuaQuestClass:SetQuestSwitch(Ep2, 49, iObjIndex)
				elseif QS >= 52 and QS <= 54 then
					LuaQuestClass:SetQuestSwitch(Ep2, 52, iObjIndex)
				end					

			elseif UserLv >= 240 and UserLv <= 299 then
				if QS < 55 then
					LuaQuestClass:SetQuestSwitch(Ep2, 55, iObjIndex)
				elseif QS >= 55 and QS <= 57 then
					LuaQuestClass:SetQuestSwitch(Ep2, 55, iObjIndex)
				elseif QS >= 58 and QS <= 60 then
					LuaQuestClass:SetQuestSwitch(Ep2, 58, iObjIndex)
				elseif QS >= 61 and QS <= 63 then
					LuaQuestClass:SetQuestSwitch(Ep2, 61, iObjIndex)
				end

			elseif UserLv >= 300 and UserLv <= 400 then

				if QS < 64 then
					LuaQuestClass:SetQuestSwitch(Ep2, 64, iObjIndex)
				elseif QS >= 64 and QS <= 66 then
					LuaQuestClass:SetQuestSwitch(Ep2, 64, iObjIndex)
				elseif QS >= 67 and QS <= 69 then
					LuaQuestClass:SetQuestSwitch(Ep2, 67, iObjIndex)
				elseif QS >= 70 and QS <= 72 then
					LuaQuestClass:SetQuestSwitch(Ep2, 70, iObjIndex)
				end
			end
		elseif CharMaster == 1 then
			if QS < 73 then
				LuaQuestClass:SetQuestSwitch(Ep2, 73, iObjIndex)
			elseif QS >= 73 and QS <= 75 then
				LuaQuestClass:SetQuestSwitch(Ep2, 73, iObjIndex)
			elseif QS >= 76 and QS <= 78 then
				LuaQuestClass:SetQuestSwitch(Ep2, 76, iObjIndex)
			elseif QS >= 79 and QS <= 81 then
				LuaQuestClass:SetQuestSwitch(Ep2, 79, iObjIndex)
			
			end

		end
	elseif CharClass == CLASS_MAGUMSA or CharClass == CLASS_DARKLORD or CharClass == CLASS_MONK then	-- 다크로드, 마검사
		if CharMaster == 0 then
			if UserLv >= 15 and UserLv <= 29 then
				if QS >= 1 and QS <= 8 then
					LuaQuestClass:SetQuestSwitch(Ep2, 0, iObjIndex)
				elseif QS >= 9 and QS <= 11 then
					LuaQuestClass:SetQuestSwitch(Ep2, 9, iObjIndex)
				elseif QS >= 12 and QS <= 14 then
					LuaQuestClass:SetQuestSwitch(Ep2, 12, iObjIndex)
				elseif QS >= 15 and QS <= 17 then
					LuaQuestClass:SetQuestSwitch(Ep2, 15, iObjIndex)
				end


			elseif UserLv >= 30 and UserLv <= 99 then
				if QS < 18 then
					LuaQuestClass:SetQuestSwitch(Ep2, 18, iObjIndex)
				elseif QS >= 18 and QS <= 26 then
					LuaQuestClass:SetQuestSwitch(Ep2, 18, iObjIndex)
				elseif QS >= 27 and QS <= 29 then
					LuaQuestClass:SetQuestSwitch(Ep2, 27, iObjIndex)
				elseif QS >= 30 and QS <= 32 then
					LuaQuestClass:SetQuestSwitch(Ep2, 30, iObjIndex)
				elseif QS >= 33 and QS <= 35 then
					LuaQuestClass:SetQuestSwitch(Ep2, 33, iObjIndex)
				end


			elseif UserLv >= 100 and UserLv <= 159 then

				if QS < 36 then
					LuaQuestClass:SetQuestSwitch(Ep2, 36, iObjIndex)
				elseif QS >= 36 and QS <= 39 then
					LuaQuestClass:SetQuestSwitch(Ep2, 36, iObjIndex)
				elseif QS >= 40 and QS <= 42 then
					LuaQuestClass:SetQuestSwitch(Ep2, 40, iObjIndex)
				elseif QS >= 43 and QS <= 45 then
					LuaQuestClass:SetQuestSwitch(Ep2, 43, iObjIndex)
				end

			elseif UserLv >= 160 and UserLv <= 219 then

				if QS < 46 then
					LuaQuestClass:SetQuestSwitch(Ep2, 46, iObjIndex)
				elseif QS >= 46 and QS <= 48 then
					LuaQuestClass:SetQuestSwitch(Ep2, 46, iObjIndex)
				elseif QS >= 49 and QS <= 51 then
					LuaQuestClass:SetQuestSwitch(Ep2, 49, iObjIndex)
				elseif QS >= 52 and QS <= 54 then
					LuaQuestClass:SetQuestSwitch(Ep2, 52, iObjIndex)
				end

			elseif UserLv >= 220 and UserLv <= 279 then

				if QS < 55 then
					LuaQuestClass:SetQuestSwitch(Ep2, 55, iObjIndex)
				elseif QS >= 55 and QS <= 57 then
					LuaQuestClass:SetQuestSwitch(Ep2, 55, iObjIndex)
				elseif QS >= 58 and QS <= 60 then
					LuaQuestClass:SetQuestSwitch(Ep2, 58, iObjIndex)
				elseif QS >= 61 and QS <= 63 then
					LuaQuestClass:SetQuestSwitch(Ep2, 61, iObjIndex)
				end

			elseif UserLv >= 280 and UserLv <= 400 then

				if QS < 64 then
					LuaQuestClass:SetQuestSwitch(Ep2, 64, iObjIndex)
				elseif QS >= 64 and QS <= 66 then
					LuaQuestClass:SetQuestSwitch(Ep2, 64, iObjIndex)
				elseif QS >= 67 and QS <= 69 then
					LuaQuestClass:SetQuestSwitch(Ep2, 67, iObjIndex)
				elseif QS >= 70 and QS <= 72 then
					LuaQuestClass:SetQuestSwitch(Ep2, 70, iObjIndex)
				end
			end
		elseif CharMaster == 1 then

			if QS < 73 then
				LuaQuestClass:SetQuestSwitch(Ep2, 73, iObjIndex)
			elseif QS >= 73 and QS <= 75 then
				LuaQuestClass:SetQuestSwitch(Ep2, 73, iObjIndex)
			elseif QS >= 76 and QS <= 78 then
				LuaQuestClass:SetQuestSwitch(Ep2, 76, iObjIndex)
			elseif QS >= 79 and QS <= 81 then
				LuaQuestClass:SetQuestSwitch(Ep2, 79, iObjIndex)
			
			end
		end
	end

end
--================================================================================================================
function EP3_GiveUp_Level_check(iObjIndex, Ep, QS)
	local CharMaster = LuaQuestClass:IsMasterLevel(iObjIndex) --마스터 레벨인지 판별
	local UserLv = LuaQuestClass:GetUserLv(iObjIndex)
	local CharClass = LuaQuestClass:GetCharClass(iObjIndex)
	
	if CharClass == CLASS_KNIGHT or CharClass == CLASS_WIZARD or CharClass == CLASS_ELF or CharClass == CLASS_SUMMONER then	-- 흑기사, 흑마법사, 요정, 소환술사
		if CharMaster == 0 then
			if UserLv >= 15 and UserLv <= 80 then
				if QS >= 1 and QS <= 8 then
					LuaQuestClass:SetQuestSwitch(Ep3, 0, iObjIndex)
				elseif QS >= 9 and QS <= 11 then
					LuaQuestClass:SetQuestSwitch(Ep3, 9, iObjIndex)
				elseif QS >= 12 and QS <= 14 then
					LuaQuestClass:SetQuestSwitch(Ep3, 12, iObjIndex)
				elseif QS >= 15 and QS <= 17 then
					LuaQuestClass:SetQuestSwitch(Ep3, 15, iObjIndex)
				end
			elseif UserLv >= 81 and UserLv <= 130 then

				if QS < 18 then
					LuaQuestClass:SetQuestSwitch(Ep3, 18, iObjIndex)
				elseif QS >= 18 and QS <= 26 then
					LuaQuestClass:SetQuestSwitch(Ep3, 18, iObjIndex)
				elseif QS >= 27 and QS <= 29 then
					LuaQuestClass:SetQuestSwitch(Ep3, 27, iObjIndex)
				elseif QS >= 30 and QS <= 32 then
					LuaQuestClass:SetQuestSwitch(Ep3, 30, iObjIndex)
				elseif QS >= 33 and QS <= 35 then
					LuaQuestClass:SetQuestSwitch(Ep3, 33, iObjIndex)
				end


			elseif UserLv >= 131 and UserLv <= 180 then

				if QS < 36 then
					LuaQuestClass:SetQuestSwitch(Ep3, 36, iObjIndex)
				elseif QS >= 36 and QS <= 39 then
					LuaQuestClass:SetQuestSwitch(Ep3, 36, iObjIndex)
				elseif QS >= 40 and QS <= 42 then
					LuaQuestClass:SetQuestSwitch(Ep3, 40, iObjIndex)
				elseif QS >= 43 and QS <= 45 then
					LuaQuestClass:SetQuestSwitch(Ep3, 43, iObjIndex)
				end

			elseif UserLv >= 181 and UserLv <= 230 then

				if QS < 46 then
					LuaQuestClass:SetQuestSwitch(Ep3, 46, iObjIndex)
				elseif QS >= 46 and QS <= 48 then
					LuaQuestClass:SetQuestSwitch(Ep3, 46, iObjIndex)
				elseif QS >= 49 and QS <= 51 then
					LuaQuestClass:SetQuestSwitch(Ep3, 49, iObjIndex)
				elseif QS >= 52 and QS <= 54 then
					LuaQuestClass:SetQuestSwitch(Ep3, 52, iObjIndex)
				end					

			elseif UserLv >= 231 and UserLv <= 280 then
				if QS < 55 then
					LuaQuestClass:SetQuestSwitch(Ep3, 55, iObjIndex)
				elseif QS >= 55 and QS <= 57 then
					LuaQuestClass:SetQuestSwitch(Ep3, 55, iObjIndex)
				elseif QS >= 58 and QS <= 60 then
					LuaQuestClass:SetQuestSwitch(Ep3, 58, iObjIndex)
				elseif QS >= 61 and QS <= 63 then
					LuaQuestClass:SetQuestSwitch(Ep3, 61, iObjIndex)
				end

			elseif UserLv >= 281 and UserLv <= 330 then

				if QS < 64 then
					LuaQuestClass:SetQuestSwitch(Ep3, 64, iObjIndex)
				elseif QS >= 64 and QS <= 66 then
					LuaQuestClass:SetQuestSwitch(Ep3, 64, iObjIndex)
				elseif QS >= 67 and QS <= 69 then
					LuaQuestClass:SetQuestSwitch(Ep3, 67, iObjIndex)
				elseif QS >= 70 and QS <= 72 then
					LuaQuestClass:SetQuestSwitch(Ep3, 70, iObjIndex)
				end
			elseif UserLv >= 331 and UserLv <= 400 then

				if QS < 73 then
					LuaQuestClass:SetQuestSwitch(Ep3, 73, iObjIndex)
				elseif QS >= 73 and QS <= 75 then
					LuaQuestClass:SetQuestSwitch(Ep3, 73, iObjIndex)
				elseif QS >= 76 and QS <= 78 then
					LuaQuestClass:SetQuestSwitch(Ep3, 76, iObjIndex)
				elseif QS >= 79 and QS <= 81 then
					LuaQuestClass:SetQuestSwitch(Ep3, 79, iObjIndex)
				end
			end
		elseif CharMaster == 1 then
			if QS < 82 then
				LuaQuestClass:SetQuestSwitch(Ep3, 82, iObjIndex)
			elseif QS >= 82 and QS <= 84 then
				LuaQuestClass:SetQuestSwitch(Ep3, 82, iObjIndex)
			elseif QS >= 85 and QS <= 87 then
				LuaQuestClass:SetQuestSwitch(Ep3, 85, iObjIndex)
			elseif QS >= 88 and QS <= 90 then
				LuaQuestClass:SetQuestSwitch(Ep3, 88, iObjIndex)
			
			end

		end
	elseif CharClass == CLASS_MAGUMSA or CharClass == CLASS_DARKLORD or CharClass == CLASS_MONK then	-- 다크로드, 마검사
		if CharMaster == 0 then
			if UserLv >= 10 and UserLv <= 60 then
				if QS >= 1 and QS <= 8 then
					LuaQuestClass:SetQuestSwitch(Ep3, 0, iObjIndex)
				elseif QS >= 9 and QS <= 11 then
					LuaQuestClass:SetQuestSwitch(Ep3, 9, iObjIndex)
				elseif QS >= 12 and QS <= 14 then
					LuaQuestClass:SetQuestSwitch(Ep3, 12, iObjIndex)
				elseif QS >= 15 and QS <= 17 then
					LuaQuestClass:SetQuestSwitch(Ep3, 15, iObjIndex)
				end


			elseif UserLv >= 61 and UserLv <= 110 then
				if QS < 18 then
					LuaQuestClass:SetQuestSwitch(Ep3, 18, iObjIndex)
				elseif QS >= 18 and QS <= 26 then
					LuaQuestClass:SetQuestSwitch(Ep3, 18, iObjIndex)
				elseif QS >= 27 and QS <= 29 then
					LuaQuestClass:SetQuestSwitch(Ep3, 27, iObjIndex)
				elseif QS >= 30 and QS <= 32 then
					LuaQuestClass:SetQuestSwitch(Ep3, 30, iObjIndex)
				elseif QS >= 33 and QS <= 35 then
					LuaQuestClass:SetQuestSwitch(Ep3, 33, iObjIndex)
				end


			elseif UserLv >= 111 and UserLv <= 160 then

				if QS < 36 then
					LuaQuestClass:SetQuestSwitch(Ep3, 36, iObjIndex)
				elseif QS >= 36 and QS <= 39 then
					LuaQuestClass:SetQuestSwitch(Ep3, 36, iObjIndex)
				elseif QS >= 40 and QS <= 42 then
					LuaQuestClass:SetQuestSwitch(Ep3, 40, iObjIndex)
				elseif QS >= 43 and QS <= 45 then
					LuaQuestClass:SetQuestSwitch(Ep3, 43, iObjIndex)
				end

			elseif UserLv >= 161 and UserLv <= 210 then

				if QS < 46 then
					LuaQuestClass:SetQuestSwitch(Ep3, 46, iObjIndex)
				elseif QS >= 46 and QS <= 48 then
					LuaQuestClass:SetQuestSwitch(Ep3, 46, iObjIndex)
				elseif QS >= 49 and QS <= 51 then
					LuaQuestClass:SetQuestSwitch(Ep3, 49, iObjIndex)
				elseif QS >= 52 and QS <= 54 then
					LuaQuestClass:SetQuestSwitch(Ep3, 52, iObjIndex)
				end

			elseif UserLv >= 211 and UserLv <= 260 then

				if QS < 55 then
					LuaQuestClass:SetQuestSwitch(Ep3, 55, iObjIndex)
				elseif QS >= 55 and QS <= 57 then
					LuaQuestClass:SetQuestSwitch(Ep3, 55, iObjIndex)
				elseif QS >= 58 and QS <= 60 then
					LuaQuestClass:SetQuestSwitch(Ep3, 58, iObjIndex)
				elseif QS >= 61 and QS <= 63 then
					LuaQuestClass:SetQuestSwitch(Ep3, 61, iObjIndex)
				end

			elseif UserLv >= 261 and UserLv <= 310 then

				if QS < 64 then
					LuaQuestClass:SetQuestSwitch(Ep3, 64, iObjIndex)
				elseif QS >= 64 and QS <= 66 then
					LuaQuestClass:SetQuestSwitch(Ep3, 64, iObjIndex)
				elseif QS >= 67 and QS <= 69 then
					LuaQuestClass:SetQuestSwitch(Ep3, 67, iObjIndex)
				elseif QS >= 70 and QS <= 72 then
					LuaQuestClass:SetQuestSwitch(Ep3, 70, iObjIndex)
				end
			elseif UserLv >= 311 and UserLv <= 400 then
				if QS < 73 then
					LuaQuestClass:SetQuestSwitch(Ep3, 73, iObjIndex)
				elseif QS >= 73 and QS <= 75 then
					LuaQuestClass:SetQuestSwitch(Ep3, 73, iObjIndex)
				elseif QS >= 76 and QS <= 78 then
					LuaQuestClass:SetQuestSwitch(Ep3, 76, iObjIndex)
				elseif QS >= 79 and QS <= 81 then
					LuaQuestClass:SetQuestSwitch(Ep3, 79, iObjIndex)
				end			
			end
		elseif CharMaster == 1 then

			if QS < 82 then
				LuaQuestClass:SetQuestSwitch(Ep3, 82, iObjIndex)
			elseif QS >= 82 and QS <= 84 then
				LuaQuestClass:SetQuestSwitch(Ep3, 82, iObjIndex)
			elseif QS >= 85 and QS <= 87 then
				LuaQuestClass:SetQuestSwitch(Ep3, 85, iObjIndex)
			elseif QS >= 88 and QS <= 90 then
				LuaQuestClass:SetQuestSwitch(Ep3, 88, iObjIndex)
			
			end
		end
	end
end
--================================================================================================================
function EP4_GiveUp_Level_check(iObjIndex, Ep, QS)

	local CharMaster = LuaQuestClass:IsMasterLevel(iObjIndex) --마스터 레벨인지 판별
	local UserLv = LuaQuestClass:GetUserLv(iObjIndex)
	local CharClass = LuaQuestClass:GetCharClass(iObjIndex)
	
	if CharClass == CLASS_KNIGHT or CharClass == CLASS_WIZARD or CharClass == CLASS_ELF or CharClass == CLASS_SUMMONER then	-- 흑기사, 흑마법사, 요정, 소환술사
		if CharMaster == 0 then
			if UserLv >= 15 and UserLv <= 130 then
				if QS >= 1 and QS <= 8 then
					LuaQuestClass:SetQuestSwitch(Ep4, 0, iObjIndex)
				elseif QS >= 9 and QS <= 11 then
					LuaQuestClass:SetQuestSwitch(Ep4, 9, iObjIndex)
				elseif QS >= 12 and QS <= 14 then
					LuaQuestClass:SetQuestSwitch(Ep4, 12, iObjIndex)
				elseif QS >= 15 and QS <= 17 then
					LuaQuestClass:SetQuestSwitch(Ep4, 15, iObjIndex)
				end
			elseif UserLv >= 131 and UserLv <= 180 then

				if QS < 18 then
					LuaQuestClass:SetQuestSwitch(Ep4, 18, iObjIndex)
				elseif QS >= 18 and QS <= 26 then
					LuaQuestClass:SetQuestSwitch(Ep4, 18, iObjIndex)
				elseif QS >= 27 and QS <= 29 then
					LuaQuestClass:SetQuestSwitch(Ep4, 27, iObjIndex)
				elseif QS >= 30 and QS <= 32 then
					LuaQuestClass:SetQuestSwitch(Ep4, 30, iObjIndex)
				elseif QS >= 33 and QS <= 35 then
					LuaQuestClass:SetQuestSwitch(Ep4, 33, iObjIndex)
				end


			elseif UserLv >= 181 and UserLv <= 230 then

				if QS < 36 then
					LuaQuestClass:SetQuestSwitch(Ep4, 36, iObjIndex)
				elseif QS >= 36 and QS <= 39 then
					LuaQuestClass:SetQuestSwitch(Ep4, 36, iObjIndex)
				elseif QS >= 40 and QS <= 42 then
					LuaQuestClass:SetQuestSwitch(Ep4, 40, iObjIndex)
				elseif QS >= 43 and QS <= 45 then
					LuaQuestClass:SetQuestSwitch(Ep4, 43, iObjIndex)
				end

			elseif UserLv >= 231 and UserLv <= 280 then

				if QS < 46 then
					LuaQuestClass:SetQuestSwitch(Ep4, 46, iObjIndex)
				elseif QS >= 46 and QS <= 48 then
					LuaQuestClass:SetQuestSwitch(Ep4, 46, iObjIndex)
				elseif QS >= 49 and QS <= 51 then
					LuaQuestClass:SetQuestSwitch(Ep4, 49, iObjIndex)
				elseif QS >= 52 and QS <= 54 then
					LuaQuestClass:SetQuestSwitch(Ep4, 52, iObjIndex)
				end					

			elseif UserLv >= 281 and UserLv <= 330 then
				if QS < 55 then
					LuaQuestClass:SetQuestSwitch(Ep4, 55, iObjIndex)
				elseif QS >= 55 and QS <= 57 then
					LuaQuestClass:SetQuestSwitch(Ep4, 55, iObjIndex)
				elseif QS >= 58 and QS <= 60 then
					LuaQuestClass:SetQuestSwitch(Ep4, 58, iObjIndex)
				elseif QS >= 61 and QS <= 63 then
					LuaQuestClass:SetQuestSwitch(Ep4, 61, iObjIndex)
				end

			elseif UserLv >= 331 and UserLv <= 400 then

				if QS >= 64 and QS <= 72 then
					LuaQuestClass:SetQuestSwitch(Ep4, 64, iObjIndex)
				elseif QS >= 64 and QS <= 66 then
					LuaQuestClass:SetQuestSwitch(Ep4, 64, iObjIndex)
				elseif QS >= 67 and QS <= 69 then
					LuaQuestClass:SetQuestSwitch(Ep4, 67, iObjIndex)
				elseif QS >= 70 and QS <= 72 then
					LuaQuestClass:SetQuestSwitch(Ep4, 70, iObjIndex)
				end
			end
		elseif CharMaster == 1 then
			if QS < 73 then
				LuaQuestClass:SetQuestSwitch(Ep4, 73, iObjIndex)
			elseif QS >= 73 and QS <= 75 then
				LuaQuestClass:SetQuestSwitch(Ep4, 73, iObjIndex)
			elseif QS >= 76 and QS <= 78 then
				LuaQuestClass:SetQuestSwitch(Ep4, 76, iObjIndex)
			elseif QS >= 79 and QS <= 81 then
				LuaQuestClass:SetQuestSwitch(Ep4, 79, iObjIndex)
			
			end

		end
	elseif CharClass == CLASS_MAGUMSA or CharClass == CLASS_DARKLORD or CharClass == CLASS_MONK then	-- 다크로드, 마검사
		if CharMaster == 0 then
			if UserLv >= 10 and UserLv <= 110 then
				if QS >= 1 and QS <= 8 then
					LuaQuestClass:SetQuestSwitch(Ep4, 0, iObjIndex)
				elseif QS >= 9 and QS <= 11 then
					LuaQuestClass:SetQuestSwitch(Ep4, 9, iObjIndex)
				elseif QS >= 12 and QS <= 14 then
					LuaQuestClass:SetQuestSwitch(Ep4, 12, iObjIndex)
				elseif QS >= 15 and QS <= 17 then
					LuaQuestClass:SetQuestSwitch(Ep4, 15, iObjIndex)
				end


			elseif UserLv >= 111 and UserLv <= 160 then
				if QS < 18 then
					LuaQuestClass:SetQuestSwitch(Ep4, 18, iObjIndex)
				elseif QS >= 18 and QS <= 26 then
					LuaQuestClass:SetQuestSwitch(Ep4, 18, iObjIndex)
				elseif QS >= 27 and QS <= 29 then
					LuaQuestClass:SetQuestSwitch(Ep4, 27, iObjIndex)
				elseif QS >= 30 and QS <= 32 then
					LuaQuestClass:SetQuestSwitch(Ep4, 30, iObjIndex)
				elseif QS >= 33 and QS <= 35 then
					LuaQuestClass:SetQuestSwitch(Ep4, 33, iObjIndex)
				end


			elseif UserLv >= 161 and UserLv <= 210 then

				if QS < 36 then
					LuaQuestClass:SetQuestSwitch(Ep4, 36, iObjIndex)
				elseif QS >= 36 and QS <= 39 then
					LuaQuestClass:SetQuestSwitch(Ep4, 36, iObjIndex)
				elseif QS >= 40 and QS <= 42 then
					LuaQuestClass:SetQuestSwitch(Ep4, 40, iObjIndex)
				elseif QS >= 43 and QS <= 45 then
					LuaQuestClass:SetQuestSwitch(Ep4, 43, iObjIndex)
				end

			elseif UserLv >= 211 and UserLv <= 260 then

				if QS < 46 then
					LuaQuestClass:SetQuestSwitch(Ep4, 46, iObjIndex)
				elseif QS >= 46 and QS <= 48 then
					LuaQuestClass:SetQuestSwitch(Ep4, 46, iObjIndex)
				elseif QS >= 49 and QS <= 51 then
					LuaQuestClass:SetQuestSwitch(Ep4, 49, iObjIndex)
				elseif QS >= 52 and QS <= 54 then
					LuaQuestClass:SetQuestSwitch(Ep4, 52, iObjIndex)
				end

			elseif UserLv >= 261 and UserLv <= 310 then

				if QS < 55 then
					LuaQuestClass:SetQuestSwitch(Ep4, 55, iObjIndex)
				elseif QS >= 55 and QS <= 57 then
					LuaQuestClass:SetQuestSwitch(Ep4, 55, iObjIndex)
				elseif QS >= 58 and QS <= 60 then
					LuaQuestClass:SetQuestSwitch(Ep4, 58, iObjIndex)
				elseif QS >= 61 and QS <= 63 then
					LuaQuestClass:SetQuestSwitch(Ep4, 61, iObjIndex)
				end

			elseif UserLv >= 311 and UserLv <= 400 then

				if QS >= 64 and QS <= 72 then
					LuaQuestClass:SetQuestSwitch(Ep4, 64, iObjIndex)
				elseif QS >= 64 and QS <= 66 then
					LuaQuestClass:SetQuestSwitch(Ep4, 64, iObjIndex)
				elseif QS >= 67 and QS <= 69 then
					LuaQuestClass:SetQuestSwitch(Ep4, 67, iObjIndex)
				elseif QS >= 70 and QS <= 72 then
					LuaQuestClass:SetQuestSwitch(Ep4, 70, iObjIndex)
				end
			end
		elseif CharMaster == 1 then

			if QS < 73 then
				LuaQuestClass:SetQuestSwitch(Ep4, 73, iObjIndex)
			elseif QS >= 73 and QS <= 75 then
				LuaQuestClass:SetQuestSwitch(Ep4, 73, iObjIndex)
			elseif QS >= 76 and QS <= 78 then
				LuaQuestClass:SetQuestSwitch(Ep4, 76, iObjIndex)
			elseif QS >= 79 and QS <= 81 then
				LuaQuestClass:SetQuestSwitch(Ep4, 79, iObjIndex)
			
			end
		end
	end
end

function EP5_GiveUp_Level_check(iObjIndex, Ep, QS)
	local CharMaster = LuaQuestClass:IsMasterLevel(iObjIndex) --마스터 레벨인지 판별
	local UserLv = LuaQuestClass:GetUserLv(iObjIndex)
	if CharMaster == 0 then	
		if UserLv >= 220 and UserLv <= 270 then
			if QS >= 1 and QS <= 8 then
				LuaQuestClass:SetQuestSwitch(Ep5, 0, iObjIndex)
			elseif QS >= 9 and QS <= 11 then
				LuaQuestClass:SetQuestSwitch(Ep5, 9, iObjIndex)
			elseif QS >= 12 and QS <= 14 then
				LuaQuestClass:SetQuestSwitch(Ep5, 12, iObjIndex)
			end

		elseif UserLv >= 271 and UserLv <= 320 then

			if QS < 15 then
				LuaQuestClass:SetQuestSwitch(Ep5, 15, iObjIndex)
			elseif QS < 15 and QS <= 23 then
				LuaQuestClass:SetQuestSwitch(Ep5, 15, iObjIndex)
			elseif QS >= 24 and QS <= 26 then
				LuaQuestClass:SetQuestSwitch(Ep5, 24, iObjIndex)
			elseif QS >= 27 and QS <= 29 then
				LuaQuestClass:SetQuestSwitch(Ep5, 27, iObjIndex)
			end

		elseif UserLv >= 321 and UserLv <= 350 then

			if QS < 30 then
				LuaQuestClass:SetQuestSwitch(Ep5, 30, iObjIndex)
			elseif QS >= 30 and QS <= 33 then
				LuaQuestClass:SetQuestSwitch(Ep5, 30, iObjIndex)
			elseif QS >= 34 and QS <= 36 then
				LuaQuestClass:SetQuestSwitch(Ep5, 34, iObjIndex)
			end


		elseif UserLv >= 351 and UserLv <= 380 then

			if QS < 37 then
				LuaQuestClass:SetQuestSwitch(Ep5, 37, iObjIndex)
			elseif QS >= 37 and QS <= 39 then
				LuaQuestClass:SetQuestSwitch(Ep5, 37, iObjIndex)
			elseif QS >= 40 and QS <= 42 then
				LuaQuestClass:SetQuestSwitch(Ep5, 40, iObjIndex)	
			end

		elseif UserLv >= 381 and UserLv <= 400 then
	
			if QS < 43 then
				LuaQuestClass:SetQuestSwitch(Ep5, 43, iObjIndex)
			elseif QS >= 43 and QS <= 45 then
				LuaQuestClass:SetQuestSwitch(Ep5, 43, iObjIndex)
			elseif QS >= 46 and QS <= 48 then
				LuaQuestClass:SetQuestSwitch(Ep5, 46, iObjIndex)
			end
		end
	elseif CharMaster == 1 then

		if QS < 49 then
			LuaQuestClass:SetQuestSwitch(Ep5, 49, iObjIndex)
		elseif QS >= 49 and QS <= 51 then
			LuaQuestClass:SetQuestSwitch(Ep5, 49, iObjIndex)
		elseif QS >= 52 and QS <= 54 then
			LuaQuestClass:SetQuestSwitch(Ep5, 52, iObjIndex)
		end
	end
end

function EP6_GiveUp_Level_check(iObjIndex, Ep, QS)
	local CharMaster = LuaQuestClass:IsMasterLevel(iObjIndex) --마스터 레벨인지 판별
	local UserLv = LuaQuestClass:GetUserLv(iObjIndex)
	local CharClass = LuaQuestClass:GetCharClass(iObjIndex)
	if CharMaster == 0 then	
		if CharClass == CLASS_KNIGHT or CharClass == CLASS_WIZARD or CharClass == CLASS_ELF or CharClass == CLASS_SUMMONER then	-- 흑기사, 흑마법사, 요정, 소환술사
			if UserLv >= 240 and UserLv <= 270 then
				if QS >= 0 and QS <= 1 then
					LuaQuestClass:SetQuestSwitch(Ep6, 0, iObjIndex)
				elseif QS >= 2 and QS <= 4 then
					LuaQuestClass:SetQuestSwitch(Ep6, 2, iObjIndex)
				elseif QS >= 5 and QS <= 7 then
					LuaQuestClass:SetQuestSwitch(Ep6, 5, iObjIndex)
				end
			elseif UserLv >= 300 and UserLv <= 320 then

				if QS < 8 then
					LuaQuestClass:SetQuestSwitch(Ep6, 8, iObjIndex)
				elseif QS >= 8 and QS <= 10 then
					LuaQuestClass:SetQuestSwitch(Ep6, 8, iObjIndex)
				elseif QS >= 11 and QS <= 13 then
					LuaQuestClass:SetQuestSwitch(Ep6, 11, iObjIndex)
				end


			elseif UserLv >= 331 and UserLv <= 350 then

				if QS < 14 then
					LuaQuestClass:SetQuestSwitch(Ep6, 14, iObjIndex)
				elseif QS >= 14 and QS <= 16 then
					LuaQuestClass:SetQuestSwitch(Ep6, 14, iObjIndex)
				elseif QS >= 17 and QS <= 19 then
					LuaQuestClass:SetQuestSwitch(Ep6, 17, iObjIndex)
				end

			elseif UserLv >= 351 and UserLv <= 380 then

				if QS < 20 then
					LuaQuestClass:SetQuestSwitch(Ep6, 20, iObjIndex)
				elseif QS >= 20 and QS <= 22 then
					LuaQuestClass:SetQuestSwitch(Ep6, 20, iObjIndex)
				elseif QS >= 23 and QS <= 25 then
					LuaQuestClass:SetQuestSwitch(Ep6, 23, iObjIndex)
				end					

			elseif UserLv >= 381 and UserLv <= 400 then
				if QS < 26 then
					LuaQuestClass:SetQuestSwitch(Ep6, 26, iObjIndex)
				elseif QS >= 26 and QS <= 28 then
					LuaQuestClass:SetQuestSwitch(Ep6, 26, iObjIndex)
				elseif QS >= 29 and QS <= 31 then
					LuaQuestClass:SetQuestSwitch(Ep6, 29, iObjIndex)
				end
			end
		elseif CharClass == CLASS_MAGUMSA or CharClass == CLASS_DARKLORD then	-- 다크로드, 마검사
			if UserLv >= 220 and UserLv <= 260 then
				if QS >= 0 and QS <= 1 then
					LuaQuestClass:SetQuestSwitch(Ep6, 0, iObjIndex)
				elseif QS >= 2 and QS <= 4 then
					LuaQuestClass:SetQuestSwitch(Ep6, 2, iObjIndex)
				elseif QS >= 5 and QS <= 7 then
					LuaQuestClass:SetQuestSwitch(Ep6, 5, iObjIndex)
				end
			elseif UserLv >= 271 and UserLv <= 320 then

				if QS < 8 then
					LuaQuestClass:SetQuestSwitch(Ep6, 8, iObjIndex)
				elseif QS >= 8 and QS <= 10 then
					LuaQuestClass:SetQuestSwitch(Ep6, 8, iObjIndex)
				elseif QS >= 11 and QS <= 13 then
					LuaQuestClass:SetQuestSwitch(Ep6, 11, iObjIndex)
				end


			elseif UserLv >= 331 and UserLv <= 350 then

				if QS < 14 then
					LuaQuestClass:SetQuestSwitch(Ep6, 14, iObjIndex)
				elseif QS >= 14 and QS <= 16 then
					LuaQuestClass:SetQuestSwitch(Ep6, 14, iObjIndex)
				elseif QS >= 17 and QS <= 19 then
					LuaQuestClass:SetQuestSwitch(Ep6, 17, iObjIndex)
				end

			elseif UserLv >= 351 and UserLv <= 380 then

				if QS < 20 then
					LuaQuestClass:SetQuestSwitch(Ep6, 20, iObjIndex)
				elseif QS >= 20 and QS <= 22 then
					LuaQuestClass:SetQuestSwitch(Ep6, 20, iObjIndex)
				elseif QS >= 23 and QS <= 25 then
					LuaQuestClass:SetQuestSwitch(Ep6, 23, iObjIndex)
				end					

			elseif UserLv >= 381 and UserLv <= 400 then
				if QS < 26 then
					LuaQuestClass:SetQuestSwitch(Ep6, 26, iObjIndex)
				elseif QS >= 26 and QS <= 28 then
					LuaQuestClass:SetQuestSwitch(Ep6, 26, iObjIndex)
				elseif QS >= 29 and QS <= 31 then
					LuaQuestClass:SetQuestSwitch(Ep6, 29, iObjIndex)
				end
			end
		end
	elseif CharMaster == 1 then
		if QS < 32 then
			LuaQuestClass:SetQuestSwitch(Ep6, 32, iObjIndex)
		elseif QS >= 32 and QS <= 34 then
			LuaQuestClass:SetQuestSwitch(Ep6, 32, iObjIndex)
		elseif QS >= 35 and QS <= 37 then
			LuaQuestClass:SetQuestSwitch(Ep6, 35, iObjIndex)
		end
	end
end

function EP7_GiveUp_Level_check(iObjIndex, Ep, QS)
	local UserLv = LuaQuestClass:GetUserLv(iObjIndex)
	if QS >= 0 and QS <= 4 then
		LuaQuestClass:SetQuestSwitch(Ep7, 0, iObjIndex)
	elseif QS >= 5 and QS <= 9 then
		LuaQuestClass:SetQuestSwitch(Ep7, 5, iObjIndex)
	elseif QS >= 10 and QS <= 14 then
		LuaQuestClass:SetQuestSwitch(Ep7, 10, iObjIndex)
	end
end
--================================================================================================================
function EP8_GiveUp_Level_check(iObjIndex, Ep, QS)

	local UserLv = LuaQuestClass:GetUserLv(iObjIndex)
	
-- 튜토리얼 퀘스트 포기
	if QS >= 0 and QS <= 5 then						-- QS 0번 ~ 5번 사이에서 포기 시
			LuaQuestClass:SetQuestSwitch(Ep8, 0, iObjIndex)		-- 퀘스트 분기 처음부분으로 QS를 설정한다
	elseif QS >= 6 and QS <= 10 then
			LuaQuestClass:SetQuestSwitch(Ep8, 6, iObjIndex)
	elseif QS >= 11 and QS <= 16 then
			LuaQuestClass:SetQuestSwitch(Ep8, 11, iObjIndex)
	elseif QS >= 17 and QS <= 19 then
			LuaQuestClass:SetQuestSwitch(Ep8, 17, iObjIndex)
	elseif QS >= 20 and QS <= 26 then
			LuaQuestClass:SetQuestSwitch(Ep8, 20, iObjIndex)
			
-- 사냥터1 퀘스트 포기
	elseif QS >= 27 and QS <= 29 then
		if UserLv < 400 then
			LuaQuestClass:SetQuestSwitch(Ep8, 27, iObjIndex)
		elseif UserLv >= 400 then
			LuaQuestClass:SetQuestSwitch(Ep8, 36, iObjIndex)	
		end
	elseif QS >= 30 and QS <= 32 then
		if UserLv < 400 then
			LuaQuestClass:SetQuestSwitch(Ep8, 30, iObjIndex)
		elseif UserLv >= 400 then
			LuaQuestClass:SetQuestSwitch(Ep8, 36, iObjIndex)	
		end
	elseif QS >= 33 and QS <= 35 then
		if UserLv < 400 then
			LuaQuestClass:SetQuestSwitch(Ep8, 33, iObjIndex)
		elseif UserLv >= 400 then
			LuaQuestClass:SetQuestSwitch(Ep8, 36, iObjIndex)	
		end
		
-- 사냥터2 퀘스트 포기
	elseif QS >= 36 and QS <= 38 then
		LuaQuestClass:SetQuestSwitch(Ep8, 36, iObjIndex)	
	elseif QS >= 39 and QS <= 41 then
		LuaQuestClass:SetQuestSwitch(Ep8, 39, iObjIndex)	
	elseif QS >= 42 and QS <= 44 then
		LuaQuestClass:SetQuestSwitch(Ep8, 42, iObjIndex)
		
-- 사냥터1, 2 반복 퀘스트 포기
	elseif QS >= 45 and QS <= 62 then
		if UserLv >= 300 and UserLv < 330 then
			LuaQuestClass:SetQuestSwitch(Ep8, 45, iObjIndex)
		elseif UserLv >= 330 and UserLv < 360 then
			LuaQuestClass:SetQuestSwitch(Ep8, 48, iObjIndex)	
		elseif UserLv >= 360 and UserLv < 400 then
			LuaQuestClass:SetQuestSwitch(Ep8, 51, iObjIndex)
		elseif UserLv >= 400 and UserLv < 420 then
			LuaQuestClass:SetQuestSwitch(Ep8, 54, iObjIndex)
		elseif UserLv >= 420 and UserLv < 440 then
			LuaQuestClass:SetQuestSwitch(Ep8, 57, iObjIndex)
		elseif UserLv >= 440 and UserLv < 460 then
			LuaQuestClass:SetQuestSwitch(Ep8, 60, iObjIndex)
		end
	end
end
--================================================================================================================
function EP9_GiveUp_Level_check(iObjIndex, Ep, QS)

	if QS >= 0 and QS <= 2 then
		LuaQuestClass:SetQuestSwitch(Ep9, 0, iObjIndex)
	elseif QS >= 3 and QS <= 5 then
		LuaQuestClass:SetQuestSwitch(Ep9, 3, iObjIndex)
	end
end
--================================================================================================================
function EP10_GiveUp_Level_check(iObjIndex, Ep, QS)

	if QS >= 0 and QS <= 2 then
		LuaQuestClass:SetQuestSwitch(Ep10, 0, iObjIndex)
	elseif QS >= 3 and QS <= 5 then
		LuaQuestClass:SetQuestSwitch(Ep10, 3, iObjIndex)
	end
end
--================================================================================================================
function EP11_GiveUp_Level_check(iObjIndex, Ep, QS)

	if QS >= 0 and QS <= 2 then
		LuaQuestClass:SetQuestSwitch(Ep11, 0, iObjIndex)
	elseif QS >= 3 and QS <= 5 then
		LuaQuestClass:SetQuestSwitch(Ep11, 3, iObjIndex)
	end
end
--================================================================================================================
function EP12_GiveUp_Level_check(iObjIndex, Ep, QS)

	if QS >= 0 and QS <= 2 then
		LuaQuestClass:SetQuestSwitch(Ep12, 0, iObjIndex)
	elseif QS >= 3 and QS <= 5 then
		LuaQuestClass:SetQuestSwitch(Ep12, 3, iObjIndex)
	elseif QS >= 6 and QS <= 8 then
		LuaQuestClass:SetQuestSwitch(Ep12, 6, iObjIndex)
	elseif QS >= 9 and QS <= 11 then
		LuaQuestClass:SetQuestSwitch(Ep12, 9, iObjIndex)
	end
end
--================================================================================================================
function EP15_GiveUp_Level_check(iObjIndex, Ep, QS)
	local UserLv = LuaQuestClass:GetUserLv(iObjIndex)
			if UserLv < 91 then
				LuaQuestClass:SetQuestSwitch(Ep15, 0, iObjIndex)
			elseif UserLv < 101 then
				LuaQuestClass:SetQuestSwitch(Ep15, 5, iObjIndex)
			elseif UserLv < 111 then
				LuaQuestClass:SetQuestSwitch(Ep15, 11, iObjIndex)
			elseif UserLv < 121 then
				LuaQuestClass:SetQuestSwitch(Ep15, 17, iObjIndex)
			elseif UserLv < 131 then
				LuaQuestClass:SetQuestSwitch(Ep15, 23, iObjIndex)
			elseif UserLv < 141 then
				LuaQuestClass:SetQuestSwitch(Ep15, 29, iObjIndex)
			elseif UserLv < 161 then
				LuaQuestClass:SetQuestSwitch(Ep15, 35, iObjIndex)
			elseif UserLv < 166 then
				LuaQuestClass:SetQuestSwitch(Ep15, 41, iObjIndex)
			elseif UserLv < 171 then
				LuaQuestClass:SetQuestSwitch(Ep15, 47, iObjIndex)
			elseif UserLv < 180 then
				LuaQuestClass:SetQuestSwitch(Ep15, 53, iObjIndex)
			elseif UserLv < 186 then
				LuaQuestClass:SetQuestSwitch(Ep15, 59, iObjIndex)
			elseif UserLv < 190 then
				LuaQuestClass:SetQuestSwitch(Ep15, 65, iObjIndex)
			elseif UserLv < 194 then
				LuaQuestClass:SetQuestSwitch(Ep15, 71, iObjIndex)
			elseif UserLv < 198 then
				LuaQuestClass:SetQuestSwitch(Ep15, 77, iObjIndex)
			elseif UserLv < 202 then
				LuaQuestClass:SetQuestSwitch(Ep15, 83, iObjIndex)
			elseif UserLv < 206 then
				LuaQuestClass:SetQuestSwitch(Ep15, 89, iObjIndex)
			elseif UserLv < 210 then
				LuaQuestClass:SetQuestSwitch(Ep15, 95, iObjIndex)
			elseif UserLv < 701 then
				LuaQuestClass:SetQuestSwitch(Ep15, 254, iObjIndex)
			end
end
--================================================================================================================
function EP16_GiveUp_Level_check(iObjIndex, Ep, QS)

	local UserLv     = LuaQuestClass:GetUserLv(iObjIndex)
	
	if QuestGensMuBlue == 0 then 	
		if UserLv < 220 then
			LuaQuestClass:SetQuestSwitch(Ep16, 0, iObjIndex)
		elseif UserLv < 250 then
			LuaQuestClass:SetQuestSwitch(Ep16, 3, iObjIndex)
		elseif UserLv < 300 then
			LuaQuestClass:SetQuestSwitch(Ep16, 6, iObjIndex)
		elseif UserLv < 380 then
			LuaQuestClass:SetQuestSwitch(Ep16, 9, iObjIndex)
		elseif UserLv < 800 then
			LuaQuestClass:SetQuestSwitch(Ep16, 12, iObjIndex)
		end
		
	elseif QuestGensMuBlue == 1 then 	
		if UserLv < 220 then
			LuaQuestClass:SetQuestSwitch(Ep16, 0, iObjIndex)
		elseif UserLv < 250 then
			LuaQuestClass:SetQuestSwitch(Ep16, 3, iObjIndex)
		elseif UserLv < 300 then
			LuaQuestClass:SetQuestSwitch(Ep16, 6, iObjIndex)
		elseif UserLv < 380 then
			LuaQuestClass:SetQuestSwitch(Ep16, 9, iObjIndex)
		elseif UserLv < 800 then
			LuaQuestClass:SetQuestSwitch(Ep16, 12, iObjIndex)
		end
	end
end
--================================================================================================================
function EP17_GiveUp_Level_check(iObjIndex, Ep, QS)

	local UserLv     = LuaQuestClass:GetUserLv(iObjIndex)
	
	if QuestGensMuBlue == 0 then 	
		if UserLv < 220 then
			LuaQuestClass:SetQuestSwitch(Ep17, 0, iObjIndex)
		elseif UserLv < 250 then
			LuaQuestClass:SetQuestSwitch(Ep17, 3, iObjIndex)
		elseif UserLv < 300 then
			LuaQuestClass:SetQuestSwitch(Ep17, 6, iObjIndex)
		elseif UserLv < 380 then
			LuaQuestClass:SetQuestSwitch(Ep17, 9, iObjIndex)
		elseif UserLv < 800 then
			LuaQuestClass:SetQuestSwitch(Ep17, 12, iObjIndex)
		end
		
	elseif QuestGensMuBlue == 1 then 	
		if UserLv < 220 then
			LuaQuestClass:SetQuestSwitch(Ep17, 0, iObjIndex)
		elseif UserLv < 250 then
			LuaQuestClass:SetQuestSwitch(Ep17, 3, iObjIndex)
		elseif UserLv < 300 then
			LuaQuestClass:SetQuestSwitch(Ep17, 6, iObjIndex)
		elseif UserLv < 380 then
			LuaQuestClass:SetQuestSwitch(Ep17, 9, iObjIndex)
		elseif UserLv < 800 then
			LuaQuestClass:SetQuestSwitch(Ep17, 12, iObjIndex)
		end
	end
end
--================================================================================================================
function EP18_GiveUp_Level_check(iObjIndex, Ep, QS)
	local UserLv = LuaQuestClass:GetUserLv(iObjIndex)
			if UserLv < 15 then
				LuaQuestClass:SetQuestSwitch(Ep18, 0, iObjIndex)
			elseif UserLv < 26 then
				LuaQuestClass:SetQuestSwitch(Ep18, 3, iObjIndex)
			elseif UserLv < 36 then
				LuaQuestClass:SetQuestSwitch(Ep18, 9, iObjIndex)
			elseif UserLv < 46 then
				LuaQuestClass:SetQuestSwitch(Ep18, 15, iObjIndex)
			elseif UserLv < 56 then
				LuaQuestClass:SetQuestSwitch(Ep18, 19, iObjIndex)
			elseif UserLv < 66 then
				LuaQuestClass:SetQuestSwitch(Ep18, 23, iObjIndex)
			elseif UserLv < 80 then
				LuaQuestClass:SetQuestSwitch(Ep18, 27, iObjIndex)
			elseif UserLv < 91 then
				LuaQuestClass:SetQuestSwitch(Ep18, 31, iObjIndex)
			elseif UserLv < 101 then
				LuaQuestClass:SetQuestSwitch(Ep18, 35, iObjIndex)
			elseif UserLv < 111 then
				LuaQuestClass:SetQuestSwitch(Ep18, 39, iObjIndex)				
			elseif UserLv < 121 then
				LuaQuestClass:SetQuestSwitch(Ep18, 43, iObjIndex)				
			elseif UserLv < 131 then
				LuaQuestClass:SetQuestSwitch(Ep18, 47, iObjIndex)				
			elseif UserLv < 141 then
				LuaQuestClass:SetQuestSwitch(Ep18, 51, iObjIndex)				
			elseif UserLv < 161 then
				LuaQuestClass:SetQuestSwitch(Ep18, 55, iObjIndex)				
			elseif UserLv < 166 then
				LuaQuestClass:SetQuestSwitch(Ep18, 59, iObjIndex)
			elseif UserLv < 171 then
				LuaQuestClass:SetQuestSwitch(Ep18, 63, iObjIndex)
			elseif UserLv < 180 then
				LuaQuestClass:SetQuestSwitch(Ep18, 67, iObjIndex)
			elseif UserLv < 190 then
				LuaQuestClass:SetQuestSwitch(Ep18, 71, iObjIndex)
			elseif UserLv < 200 then
				LuaQuestClass:SetQuestSwitch(Ep18, 75, iObjIndex)
			elseif UserLv < 210 then
				LuaQuestClass:SetQuestSwitch(Ep18, 79, iObjIndex)
			elseif UserLv < 220 then
				LuaQuestClass:SetQuestSwitch(Ep18, 83, iObjIndex)
			elseif UserLv < 230 then
				if QS < 87 then
					LuaQuestClass:SetQuestSwitch(Ep18, 87, iObjIndex)
				elseif QS >= 87 and QS <= 94 then
					LuaQuestClass:SetQuestSwitch(Ep18, 91, iObjIndex)
				end
			elseif UserLv < 235 then
				if QS < 95 then
					LuaQuestClass:SetQuestSwitch(Ep18, 95, iObjIndex)
				elseif QS >= 95 and QS <= 100 then
					LuaQuestClass:SetQuestSwitch(Ep18, 98, iObjIndex)
				end
			elseif UserLv < 240 then
				if QS < 101 then
					LuaQuestClass:SetQuestSwitch(Ep18, 101, iObjIndex)
				elseif QS >= 101 and QS <= 106 then
					LuaQuestClass:SetQuestSwitch(Ep18, 104, iObjIndex)
				end
			elseif UserLv < 245 then
				if QS < 107 then
					LuaQuestClass:SetQuestSwitch(Ep18, 107, iObjIndex)
				elseif QS >= 107 and QS <= 112 then
					LuaQuestClass:SetQuestSwitch(Ep18, 110, iObjIndex)
				end
			elseif UserLv < 250 then
				if QS < 113 then
					LuaQuestClass:SetQuestSwitch(Ep18, 113, iObjIndex)
				elseif QS >= 113 and QS <= 118 then
					LuaQuestClass:SetQuestSwitch(Ep18, 116, iObjIndex)
				end
			elseif UserLv < 255 then
				if QS < 119 then
					LuaQuestClass:SetQuestSwitch(Ep18, 119, iObjIndex)
				elseif QS >= 119 and QS <= 124 then
					LuaQuestClass:SetQuestSwitch(Ep18, 122, iObjIndex)
				end
			elseif UserLv < 260 then
				if QS < 125 then
					LuaQuestClass:SetQuestSwitch(Ep18, 125, iObjIndex)
				elseif QS >= 125 and QS <= 130 then
					LuaQuestClass:SetQuestSwitch(Ep18, 128, iObjIndex)
				end
			elseif UserLv >= 260 then
				LuaQuestClass:SetQuestSwitch(Ep18, 131, iObjIndex)
			end
end

function EP19_GiveUp_Level_check(iObjIndex, Ep, QS)
	local UserLv = LuaQuestClass:GetUserLv(iObjIndex)
			if UserLv < 290 then
				if QS < 3 then
					LuaQuestClass:SetQuestSwitch(Ep19, 0, iObjIndex)
				elseif QS >= 3 and QS <= 44 then
					LuaQuestClass:SetQuestSwitch(Ep19, 0, iObjIndex)
				end
			elseif UserLv < 320 then
				if QS < 48 then
					LuaQuestClass:SetQuestSwitch(Ep19, 45, iObjIndex)
				elseif QS >= 48 and QS <= 89 then
					LuaQuestClass:SetQuestSwitch(Ep19, 45, iObjIndex)
				end
			elseif UserLv < 350 then
				if QS < 93 then
					LuaQuestClass:SetQuestSwitch(Ep19, 90, iObjIndex)
				elseif QS >= 93 and QS <= 134 then
					LuaQuestClass:SetQuestSwitch(Ep19, 90, iObjIndex)
				end
			elseif UserLv >= 350 then
				LuaQuestClass:SetQuestSwitch(Ep19, 135, iObjIndex)
			end
end

function EP20_GiveUp_Level_check(iObjIndex, Ep, QS)
	local DayOfWeek = -1
	local UserLv = LuaQuestClass:GetUserLv(iObjIndex)
	local CharMaster = LuaQuestClass:IsMasterLevel(iObjIndex) --마스터 레벨인지 판별

	DayOfWeek = tonumber( os.date("%w") )

	if UserLv < 401 then
		if QS < 5 then
			LuaQuestClass:SetQuestSwitch(Ep20, 0, iObjIndex)

		elseif QS >= 5 and QS <= 7 then
			LuaQuestClass:SetQuestSwitch(Ep20, 5, iObjIndex)
		-- 월요일 퀘스트
		elseif QS >= 9 and QS <= 12 then
			LuaQuestClass:SetQuestSwitch(Ep20, 5, iObjIndex)
		elseif QS >= 13 and QS <= 15 then
		-- 월요일 퀘스트 포기시 같은 요일에 퀘스트 포기 불가
			if DayOfWeek == Mon then
				LuaQuestClass:SetQuestSwitch(Ep20, 13, iObjIndex)
			else
				LuaQuestClass:SetQuestSwitch(Ep20, 5, iObjIndex)
			end
		-- 화요일 퀘스트
		elseif QS >= 16 and QS <= 19 then
			LuaQuestClass:SetQuestSwitch(Ep20, 5, iObjIndex)
		elseif QS >= 20 and QS <= 22 then
		-- 화요일 퀘스트 포기시 같은 요일에 퀘스트 포기 불가
			if DayOfWeek == Tue then
				LuaQuestClass:SetQuestSwitch(Ep20, 20, iObjIndex)
			else
				LuaQuestClass:SetQuestSwitch(Ep20, 5, iObjIndex)
			end
		-- 수요일 퀘스트
		elseif QS >= 23 and QS <= 26 then
			LuaQuestClass:SetQuestSwitch(Ep20, 5, iObjIndex)
		elseif QS >= 27 and QS <= 29 then
		-- 수요일 퀘스트 포기시 같은 요일에 퀘스트 포기 불가
			if DayOfWeek == Wed then
				LuaQuestClass:SetQuestSwitch(Ep20, 27, iObjIndex)
			else
				LuaQuestClass:SetQuestSwitch(Ep20, 5, iObjIndex)
			end
		-- 목요일 퀘스트
		elseif QS >= 30 and QS <= 33 then
			LuaQuestClass:SetQuestSwitch(Ep20, 5, iObjIndex)
		elseif QS >= 34 and QS <= 36 then
		-- 목요일 퀘스트 포기시 같은 요일에 퀘스트 포기 불가
			if DayOfWeek == Thu then
			DebugPrint("목요일 퀘스트 포기 목요일이므로 퀘스트 포기 불가")
				LuaQuestClass:SetQuestSwitch(Ep20, 34, iObjIndex)
			else
			DebugPrint("목요일 퀘스트 포기 목요일이 아니므로 퀘스트 포기 가능")
				LuaQuestClass:SetQuestSwitch(Ep20, 5, iObjIndex)

			end	

		-- 금요일 퀘스트
		elseif QS >= 37 and QS <= 40 then
			LuaQuestClass:SetQuestSwitch(Ep20, 5, iObjIndex)
		elseif QS >= 41 and QS <= 43 then
		-- 금요일 퀘스트 포기시 같은 요일에 퀘스트 포기 불가
			if DayOfWeek == Fri then

				LuaQuestClass:SetQuestSwitch(Ep20, 41, iObjIndex)
			else
				LuaQuestClass:SetQuestSwitch(Ep20, 5, iObjIndex)
			end
		
		-- 토요일 퀘스트
		elseif QS >= 44 and QS <= 47 then
			LuaQuestClass:SetQuestSwitch(Ep20, 5, iObjIndex)
		elseif QS >= 48 and QS <= 50 then
		-- 토요일 퀘스트 포기시 같은 요일에 퀘스트 포기 불가
			if DayOfWeek == Sat then
				LuaQuestClass:SetQuestSwitch(Ep20, 48, iObjIndex)
			else
				LuaQuestClass:SetQuestSwitch(Ep20, 5, iObjIndex)
			end

		-- 일요일 퀘스트
		elseif QS >= 51 and QS <= 55 then
			LuaQuestClass:SetQuestSwitch(Ep20, 5, iObjIndex)
		elseif QS >= 56 and QS <= 58 then
		-- 일요일 퀘스트 포기시 같은 요일에 퀘스트 포기 불가
			if DayOfWeek == Sun then
				LuaQuestClass:SetQuestSwitch(Ep20, 56, iObjIndex)
			else
				LuaQuestClass:SetQuestSwitch(Ep20, 5, iObjIndex)
			end
		-- 완료 퀘스트
		elseif QS >= 59 and QS <= 60 then
			LuaQuestClass:SetQuestSwitch(Ep20, 59, iObjIndex)
		end		

		
	elseif UserLv >= 401 then
		if QS >= 70 and QS <= 72 then
			LuaQuestClass:SetQuestSwitch(Ep20, 70, iObjIndex)
		elseif QS >= 74 and QS <= 77 then				-- 월요일 퀘스트
			LuaQuestClass:SetQuestSwitch(Ep20, 70, iObjIndex)
		elseif QS >= 78 and QS <= 81 then				-- 화요일 퀘스트
			LuaQuestClass:SetQuestSwitch(Ep20, 70, iObjIndex)
		elseif QS >= 82 and QS <= 85 then				-- 수요일 퀘스트
			LuaQuestClass:SetQuestSwitch(Ep20, 70, iObjIndex)
		elseif QS >= 86 and QS <= 89 then				-- 목요일 퀘스트
			LuaQuestClass:SetQuestSwitch(Ep20, 70, iObjIndex)
		elseif QS >= 90 and QS <= 93 then				-- 금요일 퀘스트
			LuaQuestClass:SetQuestSwitch(Ep20, 70, iObjIndex)
		elseif QS >= 94 and QS <= 97 then				-- 토요일 퀘스트
			LuaQuestClass:SetQuestSwitch(Ep20, 70, iObjIndex)
		elseif QS >= 98 and QS <= 101 then				-- 일요일 퀘스트
			LuaQuestClass:SetQuestSwitch(Ep20, 70, iObjIndex)
		elseif QS >= 102 and QS <= 103 then				-- 완료 퀘스트
			LuaQuestClass:SetQuestSwitch(Ep20, 102, iObjIndex)
		end			
	end
end
--================================================================================================================