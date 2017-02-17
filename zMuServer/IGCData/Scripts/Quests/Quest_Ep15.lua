--[[
Quest_Ep15.lua
[ Start  ] 	2009.08.
[ Latest ]	2009-08-31 09:50 PM
[ Version ]	0.0.0.0
[ Text ]									
[ Desc ]									
--]]
QuestInclude("Quest_Define.lua")

function Ep15_Progress(SelectSyntactic, objIndex)

	local QS = LuaQuestClass:GetQuestSwitch(Ep15, objIndex)
		
	if QS == 0 then
		if SelectSyntactic == 1 then -- 수락 
			LuaQuestClass:SendQuestProgress(Ep15, 1, objIndex)-- 
			return 
		end
		if SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress(Ep15, 2, objIndex)-- 
			return 
		end
	end
	
	if QS == 1 then
		if SelectSyntactic == 1 then -- 수락 
			local CharClass = LuaQuestClass:GetCharClass(objIndex)
		
			if CharClass == CLASS_KNIGHT then--흑기사
				LuaQuestClass:SendQuestProgress(Ep15, 3, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_WIZARD then--흑마법사
				LuaQuestClass:SendQuestProgress(Ep15, 4, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_ELF then--요정
				LuaQuestClass:SendQuestProgress(Ep15, 3, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_SUMMONER then--소환술사
				LuaQuestClass:SendQuestProgress(Ep15, 4, objIndex)-- 
				return
			end
		end
		if SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress(Ep15, 2, objIndex)-- 
			return 
		end
	end
	
	if QS == 5 then
		if SelectSyntactic == 1 then -- 사냥해올게
			local CharClass = LuaQuestClass:GetCharClass(objIndex)
		
			if CharClass == CLASS_KNIGHT then--흑기사
				LuaQuestClass:SendQuestProgress(Ep15, 6, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_WIZARD then--흑마법사
				LuaQuestClass:SendQuestProgress(Ep15, 7, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_ELF then--요정
				LuaQuestClass:SendQuestProgress(Ep15, 8, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_SUMMONER then--소환술사
				LuaQuestClass:SendQuestProgress(Ep15, 9, objIndex)-- 
				return
			end
		end
		if SelectSyntactic == 2 then -- 싫은걸~
			LuaQuestClass:SendQuestProgress(Ep15, 10, objIndex)-- 
			return
		end
	end

	if QS == 11 then
		if SelectSyntactic == 1 then -- 사냥해올게
			local CharClass = LuaQuestClass:GetCharClass(objIndex)
		
			if CharClass == CLASS_KNIGHT then--흑기사
				LuaQuestClass:SendQuestProgress(Ep15, 12, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_WIZARD then--흑마법사
				LuaQuestClass:SendQuestProgress(Ep15, 13, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_ELF then--요정
				LuaQuestClass:SendQuestProgress(Ep15, 14, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_SUMMONER then--소환술사
				LuaQuestClass:SendQuestProgress(Ep15, 15, objIndex)-- 
				return
			end
		end
		if SelectSyntactic == 2 then -- 싫은걸~
			LuaQuestClass:SendQuestProgress(Ep15, 16, objIndex)-- 
			return
		end
	end


	if QS == 17 then
		if SelectSyntactic == 1 then -- 사냥해올게
			local CharClass = LuaQuestClass:GetCharClass(objIndex)
		
			if CharClass == CLASS_KNIGHT then--흑기사
				LuaQuestClass:SendQuestProgress(Ep15, 18, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_WIZARD then--흑마법사
				LuaQuestClass:SendQuestProgress(Ep15, 19, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_ELF then--요정
				LuaQuestClass:SendQuestProgress(Ep15, 20, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_SUMMONER then--소환술사
				LuaQuestClass:SendQuestProgress(Ep15, 21, objIndex)-- 
				return
			end
		end
		if SelectSyntactic == 2 then -- 싫은걸~
			LuaQuestClass:SendQuestProgress(Ep15, 22, objIndex)-- 
			return
		end
	end

	if QS == 23 then
		if SelectSyntactic == 1 then -- 사냥해올게
			local CharClass = LuaQuestClass:GetCharClass(objIndex)
		
			if CharClass == CLASS_KNIGHT then--흑기사
				LuaQuestClass:SendQuestProgress(Ep15, 24, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_WIZARD then--흑마법사
				LuaQuestClass:SendQuestProgress(Ep15, 25, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_ELF then--요정
				LuaQuestClass:SendQuestProgress(Ep15, 26, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_SUMMONER then--소환술사
				LuaQuestClass:SendQuestProgress(Ep15, 27, objIndex)-- 
				return
			end
		end
		if SelectSyntactic == 2 then -- 싫은걸~
			LuaQuestClass:SendQuestProgress(Ep15, 28, objIndex)-- 
			return
		end
	end

	if QS == 29 then
		if SelectSyntactic == 1 then -- 사냥해올게
			local CharClass = LuaQuestClass:GetCharClass(objIndex)
		
			if CharClass == CLASS_KNIGHT then--흑기사
				LuaQuestClass:SendQuestProgress(Ep15, 30, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_WIZARD then--흑마법사
				LuaQuestClass:SendQuestProgress(Ep15, 31, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_ELF then--요정
				LuaQuestClass:SendQuestProgress(Ep15, 32, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_SUMMONER then--소환술사
				LuaQuestClass:SendQuestProgress(Ep15, 33, objIndex)-- 
				return
			end
		end
		if SelectSyntactic == 2 then -- 싫은걸~
			LuaQuestClass:SendQuestProgress(Ep15, 34, objIndex)-- 
			return
		end
	end

	if QS == 35 then
		if SelectSyntactic == 1 then -- 사냥해올게
			local CharClass = LuaQuestClass:GetCharClass(objIndex)
		
			if CharClass == CLASS_KNIGHT then--흑기사
				LuaQuestClass:SendQuestProgress(Ep15, 36, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_WIZARD then--흑마법사
				LuaQuestClass:SendQuestProgress(Ep15, 37, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_ELF then--요정
				LuaQuestClass:SendQuestProgress(Ep15, 38, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_SUMMONER then--소환술사
				LuaQuestClass:SendQuestProgress(Ep15, 39, objIndex)-- 
				return
			end
		end
		if SelectSyntactic == 2 then -- 싫은걸~
			LuaQuestClass:SendQuestProgress(Ep15, 40, objIndex)-- 
			return
		end
	end

	if QS == 41 then
		if SelectSyntactic == 1 then -- 사냥해올게
			local CharClass = LuaQuestClass:GetCharClass(objIndex)
		
			if CharClass == CLASS_KNIGHT then--흑기사
				LuaQuestClass:SendQuestProgress(Ep15, 42, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_WIZARD then--흑마법사
				LuaQuestClass:SendQuestProgress(Ep15, 43, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_ELF then--요정
				LuaQuestClass:SendQuestProgress(Ep15, 44, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_SUMMONER then--소환술사
				LuaQuestClass:SendQuestProgress(Ep15, 45, objIndex)-- 
				return
			end
		end
		if SelectSyntactic == 2 then -- 싫은걸~
			LuaQuestClass:SendQuestProgress(Ep15, 46, objIndex)-- 
			return
		end
	end

	if QS == 47 then
		if SelectSyntactic == 1 then -- 사냥해올게
			local CharClass = LuaQuestClass:GetCharClass(objIndex)
		
			if CharClass == CLASS_KNIGHT then--흑기사
				LuaQuestClass:SendQuestProgress(Ep15, 48, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_WIZARD then--흑마법사
				LuaQuestClass:SendQuestProgress(Ep15, 49, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_ELF then--요정
				LuaQuestClass:SendQuestProgress(Ep15, 50, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_SUMMONER then--소환술사
				LuaQuestClass:SendQuestProgress(Ep15, 51, objIndex)-- 
				return
			end
		end
		if SelectSyntactic == 2 then -- 싫은걸~
			LuaQuestClass:SendQuestProgress(Ep15, 52, objIndex)-- 
			return
		end
	end


	if QS == 53 then
		if SelectSyntactic == 1 then -- 사냥해올게
			local CharClass = LuaQuestClass:GetCharClass(objIndex)
		
			if CharClass == CLASS_KNIGHT then--흑기사
				LuaQuestClass:SendQuestProgress(Ep15, 54, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_WIZARD then--흑마법사
				LuaQuestClass:SendQuestProgress(Ep15, 55, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_ELF then--요정
				LuaQuestClass:SendQuestProgress(Ep15, 56, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_SUMMONER then--소환술사
				LuaQuestClass:SendQuestProgress(Ep15, 57, objIndex)-- 
				return
			end
		end
		if SelectSyntactic == 2 then -- 싫은걸~
			LuaQuestClass:SendQuestProgress(Ep15, 58, objIndex)-- 
			return
		end
	end

	if QS == 59 then
		if SelectSyntactic == 1 then -- 사냥해올게
			local CharClass = LuaQuestClass:GetCharClass(objIndex)
		
			if CharClass == CLASS_KNIGHT then--흑기사
				LuaQuestClass:SendQuestProgress(Ep15, 60, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_WIZARD then--흑마법사
				LuaQuestClass:SendQuestProgress(Ep15, 61, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_ELF then--요정
				LuaQuestClass:SendQuestProgress(Ep15, 62, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_SUMMONER then--소환술사
				LuaQuestClass:SendQuestProgress(Ep15, 63, objIndex)-- 
				return
			end
		end
		if SelectSyntactic == 2 then -- 싫은걸~
			LuaQuestClass:SendQuestProgress(Ep15, 64, objIndex)-- 
			return
		end
	end

	if QS == 65 then
		if SelectSyntactic == 1 then -- 사냥해올게
			local CharClass = LuaQuestClass:GetCharClass(objIndex)
		
			if CharClass == CLASS_KNIGHT then--흑기사
				LuaQuestClass:SendQuestProgress(Ep15, 66, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_WIZARD then--흑마법사
				LuaQuestClass:SendQuestProgress(Ep15, 67, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_ELF then--요정
				LuaQuestClass:SendQuestProgress(Ep15, 68, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_SUMMONER then--소환술사
				LuaQuestClass:SendQuestProgress(Ep15, 69, objIndex)-- 
				return
			end
		end
		if SelectSyntactic == 2 then -- 싫은걸~
			LuaQuestClass:SendQuestProgress(Ep15, 70, objIndex)-- 
			return
		end
	end

	if QS == 71 then
		if SelectSyntactic == 1 then -- 사냥해올게
			local CharClass = LuaQuestClass:GetCharClass(objIndex)
		
			if CharClass == CLASS_KNIGHT then--흑기사
				LuaQuestClass:SendQuestProgress(Ep15, 72, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_WIZARD then--흑마법사
				LuaQuestClass:SendQuestProgress(Ep15, 73, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_ELF then--요정
				LuaQuestClass:SendQuestProgress(Ep15, 74, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_SUMMONER then--소환술사
				LuaQuestClass:SendQuestProgress(Ep15, 75, objIndex)-- 
				return
			end
		end
		if SelectSyntactic == 2 then -- 싫은걸~
			LuaQuestClass:SendQuestProgress(Ep15, 76, objIndex)-- 
			return
		end
	end

	if QS == 77 then
		if SelectSyntactic == 1 then -- 사냥해올게
			local CharClass = LuaQuestClass:GetCharClass(objIndex)
		
			if CharClass == CLASS_KNIGHT then--흑기사
				LuaQuestClass:SendQuestProgress(Ep15, 78, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_WIZARD then--흑마법사
				LuaQuestClass:SendQuestProgress(Ep15, 79, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_ELF then--요정
				LuaQuestClass:SendQuestProgress(Ep15, 80, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_SUMMONER then--소환술사
				LuaQuestClass:SendQuestProgress(Ep15, 81, objIndex)-- 
				return
			end
		end
		if SelectSyntactic == 2 then -- 싫은걸~
			LuaQuestClass:SendQuestProgress(Ep15, 82, objIndex)-- 
			return
		end
	end

	if QS == 83 then
		if SelectSyntactic == 1 then -- 사냥해올게
			local CharClass = LuaQuestClass:GetCharClass(objIndex)
		
			if CharClass == CLASS_KNIGHT then--흑기사
				LuaQuestClass:SendQuestProgress(Ep15, 84, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_WIZARD then--흑마법사
				LuaQuestClass:SendQuestProgress(Ep15, 85, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_ELF then--요정
				LuaQuestClass:SendQuestProgress(Ep15, 86, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_SUMMONER then--소환술사
				LuaQuestClass:SendQuestProgress(Ep15, 87, objIndex)-- 
				return
			end
		end
		if SelectSyntactic == 2 then -- 싫은걸~
			LuaQuestClass:SendQuestProgress(Ep15, 88, objIndex)-- 
			return
		end
	end

	if QS == 89 then
		if SelectSyntactic == 1 then -- 사냥해올게
			local CharClass = LuaQuestClass:GetCharClass(objIndex)
		
			if CharClass == CLASS_KNIGHT then--흑기사
				LuaQuestClass:SendQuestProgress(Ep15, 90, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_WIZARD then--흑마법사
				LuaQuestClass:SendQuestProgress(Ep15, 91, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_ELF then--요정
				LuaQuestClass:SendQuestProgress(Ep15, 92, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_SUMMONER then--소환술사
				LuaQuestClass:SendQuestProgress(Ep15, 93, objIndex)-- 
				return
			end
		end
		if SelectSyntactic == 2 then -- 싫은걸~
			LuaQuestClass:SendQuestProgress(Ep15, 94, objIndex)-- 
			return
		end
	end

	if QS == 95 then
		if SelectSyntactic == 1 then -- 사냥해올게
			local CharClass = LuaQuestClass:GetCharClass(objIndex)
		
			if CharClass == CLASS_KNIGHT then--흑기사
				LuaQuestClass:SendQuestProgress(Ep15, 96, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_WIZARD then--흑마법사
				LuaQuestClass:SendQuestProgress(Ep15, 97, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_ELF then--요정
				LuaQuestClass:SendQuestProgress(Ep15, 98, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_SUMMONER then--소환술사
				LuaQuestClass:SendQuestProgress(Ep15, 99, objIndex)-- 
				return
			end
		end
		if SelectSyntactic == 2 then -- 싫은걸~
			LuaQuestClass:SendQuestProgress(Ep15, 100, objIndex)-- 
			return
		end
	end

end

function Ep15_ChkCompleteQS(QS)

--	local QS = LuaQuestClass:GetQuestSwitch(Ep15, objIndex)

	if QS == 6 or QS == 7 or QS == 8 or QS == 9 
	or QS == 12 or QS == 13 or QS == 14 or QS == 15 
	or QS == 18 or QS == 19 or QS == 20 or QS == 21 
	or QS == 24 or QS == 25 or QS == 26 or QS == 27 
	or QS == 30 or QS == 31 or QS == 32 or QS == 33 
	or QS == 36 or QS == 37 or QS == 38 or QS == 39 
	or QS == 42 or QS == 43 or QS == 44 or QS == 45 
	or QS == 48 or QS == 49 or QS == 50 or QS == 51 
	or QS == 54 or QS == 55 or QS == 56 or QS == 57 
	or QS == 60 or QS == 61 or QS == 62 or QS == 63 
	or QS == 66 or QS == 67 or QS == 68 or QS == 69 
	or QS == 72 or QS == 73 or QS == 74 or QS == 75 
	or QS == 78 or QS == 79 or QS == 80 or QS == 81 
	or QS == 84 or QS == 85 or QS == 86 or QS == 87 
	or QS == 90 or QS == 91 or QS == 92 or QS == 93 
	or QS == 96 or QS == 97 or QS == 98 or QS == 99 then
		return 1
	end
	
	return 0
end

function Ep15_SendCompleteQS(QS, objIndex)

--	local QS = LuaQuestClass:GetQuestSwitch(Ep15, objIndex)

	if QS == 6 or QS == 7 or QS == 8 or QS == 9 then
		LuaQuestClass:SetQuestSwitch(Ep15, 11, objIndex)
	elseif QS == 12 or QS == 13 or QS == 14 or QS == 15 then
		LuaQuestClass:SetQuestSwitch(Ep15, 17, objIndex)
	elseif QS == 18 or QS == 19 or QS == 20 or QS == 21 then
		LuaQuestClass:SetQuestSwitch(Ep15, 23, objIndex)
	elseif QS == 24 or QS == 25 or QS == 26 or QS == 27 then
		LuaQuestClass:SetQuestSwitch(Ep15, 29, objIndex)
	elseif QS == 30 or QS == 31 or QS == 32 or QS == 33 then
		LuaQuestClass:SetQuestSwitch(Ep15, 35, objIndex)
	elseif QS == 36 or QS == 37 or QS == 38 or QS == 39 then
		LuaQuestClass:SetQuestSwitch(Ep15, 41, objIndex)
	elseif QS == 42 or QS == 43 or QS == 44 or QS == 45 then
		LuaQuestClass:SetQuestSwitch(Ep15, 47, objIndex)
	elseif QS == 48 or QS == 49 or QS == 50 or QS == 51 then
		LuaQuestClass:SetQuestSwitch(Ep15, 53, objIndex)
	elseif QS == 54 or QS == 55 or QS == 56 or QS == 57 then
		LuaQuestClass:SetQuestSwitch(Ep15, 59, objIndex)
	elseif QS == 60 or QS == 61 or QS == 62 or QS == 63 then
		LuaQuestClass:SetQuestSwitch(Ep15, 65, objIndex)
	elseif QS == 66 or QS == 67 or QS == 68 or QS == 69 then
		LuaQuestClass:SetQuestSwitch(Ep15, 71, objIndex)
	elseif QS == 72 or QS == 73 or QS == 74 or QS == 75 then
		LuaQuestClass:SetQuestSwitch(Ep15, 77, objIndex)
	elseif QS == 78 or QS == 79 or QS == 80 or QS == 81 then
		LuaQuestClass:SetQuestSwitch(Ep15, 83, objIndex)
	elseif QS == 84 or QS == 85 or QS == 86 or QS == 87 then
		LuaQuestClass:SetQuestSwitch(Ep15, 89, objIndex)
	elseif QS == 90 or QS == 91 or QS == 92 or QS == 93 then
		LuaQuestClass:SetQuestSwitch(Ep15, 95, objIndex)
	elseif QS == 96 or QS == 97 or QS == 98 or QS == 99 then
		LuaQuestClass:SetQuestSwitch(Ep15, 101, objIndex)
	end
	return
end



function Ep15_Complete(objIndex)

	local Quest1LimitLv = 90
	local Quest2LimitLv = 100
	local Quest3LimitLv = 110
	local Quest4LimitLv = 120
	local Quest5LimitLv = 130
	local Quest6LimitLv = 140
	local Quest7LimitLv = 160
	local Quest8LimitLv = 165
	local Quest9LimitLv = 170
	local Quest10LimitLv = 179
	local Quest11LimitLv = 185
	local Quest12LimitLv = 189
	local Quest13LimitLv = 193
	local Quest14LimitLv = 197
	local Quest15LimitLv = 201
	local Quest16LimitLv = 205

	local LimitLv = 209
	
	
	local QS = LuaQuestClass:GetQuestSwitch(Ep15, objIndex)
	
	if QS == 2 then
		LuaQuestClass:SetQuestSwitch(Ep15, 0, objIndex)
		LuaQuestClass:SendQuestComplete(Ep15, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif QS == 10 then
		LuaQuestClass:SetQuestSwitch(Ep15, 5, objIndex)
		LuaQuestClass:SendQuestComplete(Ep15, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif  QS == 16 then
		LuaQuestClass:SetQuestSwitch(Ep15, 11, objIndex)
		LuaQuestClass:SendQuestComplete(Ep15, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif QS == 22 then
		LuaQuestClass:SetQuestSwitch(Ep15, 17, objIndex)
		LuaQuestClass:SendQuestComplete(Ep15, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif  QS == 28 then
		LuaQuestClass:SetQuestSwitch(Ep15, 23, objIndex)
		LuaQuestClass:SendQuestComplete(Ep15, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif  QS == 34 then
		LuaQuestClass:SetQuestSwitch(Ep15, 29, objIndex)
		LuaQuestClass:SendQuestComplete(Ep15, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif  QS == 40 then
		LuaQuestClass:SetQuestSwitch(Ep15, 35, objIndex)
		LuaQuestClass:SendQuestComplete(Ep15, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif  QS == 46 then
		LuaQuestClass:SetQuestSwitch(Ep15, 41, objIndex)
		LuaQuestClass:SendQuestComplete(Ep15, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif  QS == 52 then
		LuaQuestClass:SetQuestSwitch(Ep15, 47, objIndex)
		LuaQuestClass:SendQuestComplete(Ep15, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif  QS == 58 then
		LuaQuestClass:SetQuestSwitch(Ep15, 53, objIndex)
		LuaQuestClass:SendQuestComplete(Ep15, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif  QS == 64 then
		LuaQuestClass:SetQuestSwitch(Ep15, 59, objIndex)
		LuaQuestClass:SendQuestComplete(Ep15, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif  QS == 70 then
		LuaQuestClass:SetQuestSwitch(Ep15, 65, objIndex)
		LuaQuestClass:SendQuestComplete(Ep15, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif  QS == 76 then
		LuaQuestClass:SetQuestSwitch(Ep15, 71, objIndex)
		LuaQuestClass:SendQuestComplete(Ep15, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif  QS == 82 then
		LuaQuestClass:SetQuestSwitch(Ep15, 77, objIndex)
		LuaQuestClass:SendQuestComplete(Ep15, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif  QS == 88 then
		LuaQuestClass:SetQuestSwitch(Ep15, 83, objIndex)
		LuaQuestClass:SendQuestComplete(Ep15, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif  QS == 94 then
		LuaQuestClass:SetQuestSwitch(Ep15, 89, objIndex)
		LuaQuestClass:SendQuestComplete(Ep15, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif  QS == 100 then
		LuaQuestClass:SetQuestSwitch(Ep15, 95, objIndex)
		LuaQuestClass:SendQuestComplete(Ep15, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	end	

	
	local bComplete = LuaQuestClass:IsQuestComplete(Ep15, objIndex)--완료 체크
	
	if bComplete == false then
		return
	end

	if QS == 3 or QS == 4 then
	
		local UserLv = LuaQuestClass:GetUserLv(objIndex)
		
		if UserLv <= LimitLv then
			local Result = LuaQuestClass:SendQuestReward(Ep15, objIndex)
			if Result == 1 then
				LuaQuestClass:SetQuestSwitch(Ep15, 5, objIndex)
			end
			return
		elseif UserLv > LimitLv then
			local Result = LuaQuestClass:SendQuestReward(Ep15, objIndex)
			if Result == 1 then
				LuaQuestClass:SetQuestSwitch(Ep15, 101, objIndex)
			end
			return
		end
		
		return
	end
	
	
	local ChkCompleteQS = Ep15_ChkCompleteQS(QS)
	
	if ChkCompleteQS == 1 then
	
		local UserLv = LuaQuestClass:GetUserLv(objIndex)
		
		if UserLv <= LimitLv then
			local Result = LuaQuestClass:SendQuestReward(Ep15, objIndex)
			if Result == 1 then
				--LuaQuestClass:SetQuestSwitch(Ep15, 5, objIndex)
				Ep15_SendCompleteQS(QS, objIndex)
			end
			return
		elseif UserLv > LimitLv then
			local Result = LuaQuestClass:SendQuestReward(Ep15, objIndex)
			if Result == 1 then
				LuaQuestClass:SetQuestSwitch(Ep15, 101, objIndex)
			end

			return
		end
		
		return
	end

end

