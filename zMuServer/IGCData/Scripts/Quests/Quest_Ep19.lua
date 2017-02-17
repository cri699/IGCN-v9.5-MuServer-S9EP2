--[[
Quest_Ep19.lua
[ Start  ] 	2009.08.
[ Latest ]	2010-02-17 11:15 AM
[ Version ]	0.0.0.0
[ Text ]									
[ Desc ]									
--]]
QuestInclude("Quest_Define.lua")

function Ep19_Progress(SelectSyntactic, objIndex)

	local QS = LuaQuestClass:GetQuestSwitch( Ep19, objIndex)
	local Quest_Random = math.random(1, 16000)
	if QS == 0 then
		if SelectSyntactic == 1 then -- 수락 
			LuaQuestClass:SendQuestProgress( Ep19, 2, objIndex)-- 
			return 
		end
		if SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress( Ep19, 1, objIndex)-- 
			return 
		end
	elseif QS == 3 then
		if SelectSyntactic == 1 then -- 수락 
			local CharClass = LuaQuestClass:GetCharClass(objIndex)
		
			if Quest_Random <= 400 then
				LuaQuestClass:SendQuestProgress(Ep19, 5, objIndex)-- 
				return
			elseif Quest_Random <= 800 then
				LuaQuestClass:SendQuestProgress(Ep19, 6, objIndex)-- 
				return
			elseif Quest_Random <= 1200 then
				LuaQuestClass:SendQuestProgress(Ep19, 7, objIndex)-- 
				return
			elseif Quest_Random <= 1600 then
				LuaQuestClass:SendQuestProgress(Ep19, 8, objIndex)-- 
				return
			elseif Quest_Random <= 2000 then
				LuaQuestClass:SendQuestProgress(Ep19, 9, objIndex)-- 
				return
			elseif Quest_Random <= 2400 then
				LuaQuestClass:SendQuestProgress(Ep19, 10, objIndex)-- 
				return
			elseif Quest_Random <= 2800 then
				LuaQuestClass:SendQuestProgress(Ep19, 11, objIndex)-- 
				return
			elseif Quest_Random <= 3200 then
				LuaQuestClass:SendQuestProgress(Ep19, 12, objIndex)-- 
				return
			elseif Quest_Random <= 3600 then
				LuaQuestClass:SendQuestProgress(Ep19, 13, objIndex)-- 
				return
			elseif Quest_Random <= 4000 then
				LuaQuestClass:SendQuestProgress(Ep19, 14, objIndex)-- 
				return
			elseif Quest_Random <= 4400 then
				LuaQuestClass:SendQuestProgress(Ep19, 15, objIndex)-- 
				return
			elseif Quest_Random <= 4800 then
				LuaQuestClass:SendQuestProgress(Ep19, 16, objIndex)-- 
				return
			elseif Quest_Random <= 5200 then
				LuaQuestClass:SendQuestProgress(Ep19, 17, objIndex)-- 
				return
			elseif Quest_Random <= 5600 then
				LuaQuestClass:SendQuestProgress(Ep19, 18, objIndex)-- 
				return
			elseif Quest_Random <= 6000 then
				LuaQuestClass:SendQuestProgress(Ep19, 19, objIndex)-- 
				return
			elseif Quest_Random <= 6400 then
				LuaQuestClass:SendQuestProgress(Ep19, 20, objIndex)-- 
				return				
			elseif Quest_Random <= 6800 then
				LuaQuestClass:SendQuestProgress(Ep19, 21, objIndex)-- 
				return					
			elseif Quest_Random <= 7200 then
				LuaQuestClass:SendQuestProgress(Ep19, 22, objIndex)-- 
				return	
			elseif Quest_Random <= 7600 then
				LuaQuestClass:SendQuestProgress(Ep19, 23, objIndex)-- 
				return	
			elseif Quest_Random <= 8000 then
				LuaQuestClass:SendQuestProgress(Ep19, 24, objIndex)-- 
				return	
			elseif Quest_Random <= 8400 then
				LuaQuestClass:SendQuestProgress(Ep19, 25, objIndex)-- 
				return	
			elseif Quest_Random <= 8800 then
				LuaQuestClass:SendQuestProgress(Ep19, 26, objIndex)-- 
				return	
			elseif Quest_Random <= 9200 then
				LuaQuestClass:SendQuestProgress(Ep19, 27, objIndex)-- 
				return	
			elseif Quest_Random <= 9600 then
				LuaQuestClass:SendQuestProgress(Ep19, 28, objIndex)-- 
				return	
			elseif Quest_Random <= 10000 then
				LuaQuestClass:SendQuestProgress(Ep19, 29, objIndex)-- 
				return	
			elseif Quest_Random <= 10400 then
				LuaQuestClass:SendQuestProgress(Ep19, 30, objIndex)-- 
				return	
			elseif Quest_Random <= 10800 then
				LuaQuestClass:SendQuestProgress(Ep19, 31, objIndex)-- 
				return	
			elseif Quest_Random <= 11200 then
				LuaQuestClass:SendQuestProgress(Ep19, 32, objIndex)-- 
				return	
			elseif Quest_Random <= 11600 then
				LuaQuestClass:SendQuestProgress(Ep19, 33, objIndex)-- 
				return	
			elseif Quest_Random <= 12000 then
				LuaQuestClass:SendQuestProgress(Ep19, 34, objIndex)-- 
				return					
			elseif Quest_Random <= 12400 then
				LuaQuestClass:SendQuestProgress(Ep19, 35, objIndex)-- 
				return	
			elseif Quest_Random <= 12800 then
				LuaQuestClass:SendQuestProgress(Ep19, 36, objIndex)-- 
				return	
			elseif Quest_Random <= 13200 then
				LuaQuestClass:SendQuestProgress(Ep19, 37, objIndex)-- 
				return	
			elseif Quest_Random <= 13600 then
				LuaQuestClass:SendQuestProgress(Ep19, 38, objIndex)-- 
				return	
			elseif Quest_Random <= 14000 then
				LuaQuestClass:SendQuestProgress(Ep19, 39, objIndex)-- 
				return	
			elseif Quest_Random <= 14400 then
				LuaQuestClass:SendQuestProgress(Ep19, 40, objIndex)-- 
				return	
			elseif Quest_Random <= 14800 then
				LuaQuestClass:SendQuestProgress(Ep19, 41, objIndex)-- 
				return	
			elseif Quest_Random <= 15200 then
				LuaQuestClass:SendQuestProgress(Ep19, 42, objIndex)-- 
				return	
			elseif Quest_Random <= 15600 then
				LuaQuestClass:SendQuestProgress(Ep19, 43, objIndex)-- 
				return	
			elseif Quest_Random <= 16000 then
				LuaQuestClass:SendQuestProgress(Ep19, 44, objIndex)-- 
				return	
			end
			
		end
		if SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress( Ep19, 4, objIndex)-- 
			return 
		end
	elseif QS == 45 then
		if SelectSyntactic == 1 then -- 수락 
			LuaQuestClass:SendQuestProgress( Ep19, 47, objIndex)-- 
			return 
		end
		if SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress( Ep19, 46, objIndex)-- 
			return 
		end
	elseif QS == 48 then
		if SelectSyntactic == 1 then -- 수락 
			local CharClass = LuaQuestClass:GetCharClass(objIndex)
		
			if Quest_Random <= 400 then
				LuaQuestClass:SendQuestProgress(Ep19, 50, objIndex)-- 
				return
			elseif Quest_Random <= 800 then
				LuaQuestClass:SendQuestProgress(Ep19, 51, objIndex)-- 
				return
			elseif Quest_Random <= 1200 then
				LuaQuestClass:SendQuestProgress(Ep19, 52, objIndex)-- 
				return
			elseif Quest_Random <= 1600 then
				LuaQuestClass:SendQuestProgress(Ep19, 53, objIndex)-- 
				return
			elseif Quest_Random <= 2000 then
				LuaQuestClass:SendQuestProgress(Ep19, 54, objIndex)-- 
				return
			elseif Quest_Random <= 2400 then
				LuaQuestClass:SendQuestProgress(Ep19, 55, objIndex)-- 
				return
			elseif Quest_Random <= 2800 then
				LuaQuestClass:SendQuestProgress(Ep19, 56, objIndex)-- 
				return
			elseif Quest_Random <= 3200 then
				LuaQuestClass:SendQuestProgress(Ep19, 57, objIndex)-- 
				return
			elseif Quest_Random <= 3600 then
				LuaQuestClass:SendQuestProgress(Ep19, 58, objIndex)-- 
				return
			elseif Quest_Random <= 4000 then
				LuaQuestClass:SendQuestProgress(Ep19, 59, objIndex)-- 
				return
			elseif Quest_Random <= 4400 then
				LuaQuestClass:SendQuestProgress(Ep19, 60, objIndex)-- 
				return
			elseif Quest_Random <= 4800 then
				LuaQuestClass:SendQuestProgress(Ep19, 61, objIndex)-- 
				return
			elseif Quest_Random <= 5200 then
				LuaQuestClass:SendQuestProgress(Ep19, 62, objIndex)-- 
				return
			elseif Quest_Random <= 5600 then
				LuaQuestClass:SendQuestProgress(Ep19, 63, objIndex)-- 
				return
			elseif Quest_Random <= 6000 then
				LuaQuestClass:SendQuestProgress(Ep19, 64, objIndex)-- 
				return
			elseif Quest_Random <= 6400 then
				LuaQuestClass:SendQuestProgress(Ep19, 65, objIndex)-- 
				return				
			elseif Quest_Random <= 6800 then
				LuaQuestClass:SendQuestProgress(Ep19, 66, objIndex)-- 
				return					
			elseif Quest_Random <= 7200 then
				LuaQuestClass:SendQuestProgress(Ep19, 67, objIndex)-- 
				return	
			elseif Quest_Random <= 7600 then
				LuaQuestClass:SendQuestProgress(Ep19, 68, objIndex)-- 
				return	
			elseif Quest_Random <= 8000 then
				LuaQuestClass:SendQuestProgress(Ep19, 69, objIndex)-- 
				return	
			elseif Quest_Random <= 8400 then
				LuaQuestClass:SendQuestProgress(Ep19, 70, objIndex)-- 
				return	
			elseif Quest_Random <= 8800 then
				LuaQuestClass:SendQuestProgress(Ep19, 71, objIndex)-- 
				return	
			elseif Quest_Random <= 9200 then
				LuaQuestClass:SendQuestProgress(Ep19, 72, objIndex)-- 
				return	
			elseif Quest_Random <= 9600 then
				LuaQuestClass:SendQuestProgress(Ep19, 73, objIndex)-- 
				return	
			elseif Quest_Random <= 10000 then
				LuaQuestClass:SendQuestProgress(Ep19, 74, objIndex)-- 
				return	
			elseif Quest_Random <= 10400 then
				LuaQuestClass:SendQuestProgress(Ep19, 75, objIndex)-- 
				return	
			elseif Quest_Random <= 10800 then
				LuaQuestClass:SendQuestProgress(Ep19, 76, objIndex)-- 
				return	
			elseif Quest_Random <= 11200 then
				LuaQuestClass:SendQuestProgress(Ep19, 77, objIndex)-- 
				return	
			elseif Quest_Random <= 11600 then
				LuaQuestClass:SendQuestProgress(Ep19, 78, objIndex)-- 
				return	
			elseif Quest_Random <= 12000 then
				LuaQuestClass:SendQuestProgress(Ep19, 79, objIndex)-- 
				return					
			elseif Quest_Random <= 12400 then
				LuaQuestClass:SendQuestProgress(Ep19, 80, objIndex)-- 
				return	
			elseif Quest_Random <= 12800 then
				LuaQuestClass:SendQuestProgress(Ep19, 81, objIndex)-- 
				return	
			elseif Quest_Random <= 13200 then
				LuaQuestClass:SendQuestProgress(Ep19, 82, objIndex)-- 
				return	
			elseif Quest_Random <= 13600 then
				LuaQuestClass:SendQuestProgress(Ep19, 83, objIndex)-- 
				return	
			elseif Quest_Random <= 14000 then
				LuaQuestClass:SendQuestProgress(Ep19, 84, objIndex)-- 
				return	
			elseif Quest_Random <= 14400 then
				LuaQuestClass:SendQuestProgress(Ep19, 85, objIndex)-- 
				return	
			elseif Quest_Random <= 14800 then
				LuaQuestClass:SendQuestProgress(Ep19, 86, objIndex)-- 
				return	
			elseif Quest_Random <= 15200 then
				LuaQuestClass:SendQuestProgress(Ep19, 87, objIndex)-- 
				return	
			elseif Quest_Random <= 15600 then
				LuaQuestClass:SendQuestProgress(Ep19, 88, objIndex)-- 
				return	
			elseif Quest_Random <= 16000 then
				LuaQuestClass:SendQuestProgress(Ep19, 89, objIndex)-- 
				return	
			end
			
		end
		if SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress( Ep19, 49, objIndex)-- 
			return 
		end
	elseif QS == 90 then
		if SelectSyntactic == 1 then -- 수락 
			LuaQuestClass:SendQuestProgress( Ep19, 92, objIndex)-- 
			return 
		end
		if SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress( Ep19, 91, objIndex)-- 
			return 
		end
	elseif QS == 93 then
		if SelectSyntactic == 1 then -- 수락 
			local CharClass = LuaQuestClass:GetCharClass(objIndex)
		
			if Quest_Random <= 400 then
				LuaQuestClass:SendQuestProgress(Ep19, 95, objIndex)-- 
				return
			elseif Quest_Random <= 800 then
				LuaQuestClass:SendQuestProgress(Ep19, 96, objIndex)-- 
				return
			elseif Quest_Random <= 1200 then
				LuaQuestClass:SendQuestProgress(Ep19, 97, objIndex)-- 
				return
			elseif Quest_Random <= 1600 then
				LuaQuestClass:SendQuestProgress(Ep19, 98, objIndex)-- 
				return
			elseif Quest_Random <= 2000 then
				LuaQuestClass:SendQuestProgress(Ep19, 99, objIndex)-- 
				return
			elseif Quest_Random <= 2400 then
				LuaQuestClass:SendQuestProgress(Ep19, 100, objIndex)-- 
				return
			elseif Quest_Random <= 2800 then
				LuaQuestClass:SendQuestProgress(Ep19, 101, objIndex)-- 
				return
			elseif Quest_Random <= 3200 then
				LuaQuestClass:SendQuestProgress(Ep19, 102, objIndex)-- 
				return
			elseif Quest_Random <= 3600 then
				LuaQuestClass:SendQuestProgress(Ep19, 103, objIndex)-- 
				return
			elseif Quest_Random <= 4000 then
				LuaQuestClass:SendQuestProgress(Ep19, 104, objIndex)-- 
				return
			elseif Quest_Random <= 4400 then
				LuaQuestClass:SendQuestProgress(Ep19, 105, objIndex)-- 
				return
			elseif Quest_Random <= 4800 then
				LuaQuestClass:SendQuestProgress(Ep19, 106, objIndex)-- 
				return
			elseif Quest_Random <= 5200 then
				LuaQuestClass:SendQuestProgress(Ep19, 107, objIndex)-- 
				return
			elseif Quest_Random <= 5600 then
				LuaQuestClass:SendQuestProgress(Ep19, 108, objIndex)-- 
				return
			elseif Quest_Random <= 6000 then
				LuaQuestClass:SendQuestProgress(Ep19, 109, objIndex)-- 
				return
			elseif Quest_Random <= 6400 then
				LuaQuestClass:SendQuestProgress(Ep19, 110, objIndex)-- 
				return				
			elseif Quest_Random <= 6800 then
				LuaQuestClass:SendQuestProgress(Ep19, 111, objIndex)-- 
				return					
			elseif Quest_Random <= 7200 then
				LuaQuestClass:SendQuestProgress(Ep19, 112, objIndex)-- 
				return	
			elseif Quest_Random <= 7600 then
				LuaQuestClass:SendQuestProgress(Ep19, 113, objIndex)-- 
				return	
			elseif Quest_Random <= 8000 then
				LuaQuestClass:SendQuestProgress(Ep19, 114, objIndex)-- 
				return	
			elseif Quest_Random <= 8400 then
				LuaQuestClass:SendQuestProgress(Ep19, 115, objIndex)-- 
				return	
			elseif Quest_Random <= 8800 then
				LuaQuestClass:SendQuestProgress(Ep19, 116, objIndex)-- 
				return	
			elseif Quest_Random <= 9200 then
				LuaQuestClass:SendQuestProgress(Ep19, 117, objIndex)-- 
				return	
			elseif Quest_Random <= 9600 then
				LuaQuestClass:SendQuestProgress(Ep19, 118, objIndex)-- 
				return	
			elseif Quest_Random <= 10000 then
				LuaQuestClass:SendQuestProgress(Ep19, 119, objIndex)-- 
				return	
			elseif Quest_Random <= 10400 then
				LuaQuestClass:SendQuestProgress(Ep19, 120, objIndex)-- 
				return	
			elseif Quest_Random <= 10800 then
				LuaQuestClass:SendQuestProgress(Ep19, 121, objIndex)-- 
				return	
			elseif Quest_Random <= 11200 then
				LuaQuestClass:SendQuestProgress(Ep19, 122, objIndex)-- 
				return	
			elseif Quest_Random <= 11600 then
				LuaQuestClass:SendQuestProgress(Ep19, 123, objIndex)-- 
				return	
			elseif Quest_Random <= 12000 then
				LuaQuestClass:SendQuestProgress(Ep19, 124, objIndex)-- 
				return					
			elseif Quest_Random <= 12400 then
				LuaQuestClass:SendQuestProgress(Ep19, 125, objIndex)-- 
				return	
			elseif Quest_Random <= 12800 then
				LuaQuestClass:SendQuestProgress(Ep19, 126, objIndex)-- 
				return	
			elseif Quest_Random <= 13200 then
				LuaQuestClass:SendQuestProgress(Ep19, 127, objIndex)-- 
				return	
			elseif Quest_Random <= 13600 then
				LuaQuestClass:SendQuestProgress(Ep19, 128, objIndex)-- 
				return	
			elseif Quest_Random <= 14000 then
				LuaQuestClass:SendQuestProgress(Ep19, 129, objIndex)-- 
				return	
			elseif Quest_Random <= 14400 then
				LuaQuestClass:SendQuestProgress(Ep19, 130, objIndex)-- 
				return	
			elseif Quest_Random <= 14800 then
				LuaQuestClass:SendQuestProgress(Ep19, 131, objIndex)-- 
				return	
			elseif Quest_Random <= 15200 then
				LuaQuestClass:SendQuestProgress(Ep19, 132, objIndex)-- 
				return	
			elseif Quest_Random <= 15600 then
				LuaQuestClass:SendQuestProgress(Ep19, 133, objIndex)-- 
				return	
			elseif Quest_Random <= 16000 then
				LuaQuestClass:SendQuestProgress(Ep19, 134, objIndex)-- 
				return	
			end
			
		end
		if SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress( Ep19, 94, objIndex)-- 
			return 
		end
	end
	
-- 선택문 모두 입력 시키기~

end

function Ep19_ChkCompleteQS(QS)

--	local QS = LuaQuestClass:GetQuestSwitch( Ep19, objIndex)
	if QS == 2 then
		return 1
	elseif QS >= 5 and QS <= 44 then	
		return 1
	elseif QS == 47 then
		return 1
	elseif QS >= 50 and QS <= 89 then	
		return 1
	elseif QS == 92 then
		return 1
	elseif QS >= 95 and QS <= 134 then	
		return 1
	end

	return 0
end

function Ep19_SendCompleteQS(QS, objIndex)

--	local QS = LuaQuestClass:GetQuestSwitch(Ep19, objIndex)
	local UserLv = LuaQuestClass:GetUserLv(objIndex)
	if QS == 2 then
		if UserLv < 290 then
			ItemType = 14 ItemIndex = 28 ItemLevel = 3 ItemSkill = 0 ItemCnt = 3 Opt = 0 ExOpt = 0
			LuaQuestClass:DeleteInvenItem(ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt, objIndex)

			LuaQuestClass:SetQuestSwitch(Ep19, 3, objIndex)
		elseif UserLv >= 290 then
			LuaQuestClass:SetQuestSwitch(Ep19, 45, objIndex)
		end
	elseif QS >= 5 and QS <= 28 then	
		if UserLv < 290 then
			LuaQuestClass:SetQuestSwitch(Ep19, 0, objIndex)
		elseif UserLv >= 290 then
			LuaQuestClass:SetQuestSwitch(Ep19, 45, objIndex)
		end
	elseif QS >= 29 and QS <= 36 then	
		ItemType = 5 ItemIndex = 6 ItemLevel = 0 ItemSkill = 0 ItemCnt = 1 Opt = 0 ExOpt = 0
		LuaQuestClass:DeleteInvenItem(ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt, objIndex)
		
		if UserLv < 290 then
			LuaQuestClass:SetQuestSwitch(Ep19, 0, objIndex)
		elseif UserLv >= 290 then
			LuaQuestClass:SetQuestSwitch(Ep19, 45, objIndex)
		end
	elseif QS >= 37 and QS <= 44 then	
		ItemType = 13 ItemIndex = 2 ItemLevel = 0 ItemSkill = 0 ItemCnt = 5 Opt = 0 ExOpt = 0
		LuaQuestClass:DeleteInvenItem(ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt, objIndex)

		if UserLv < 290 then
			
			LuaQuestClass:SetQuestSwitch(Ep19, 0, objIndex)
		elseif UserLv >= 290 then
			LuaQuestClass:SetQuestSwitch(Ep19, 45, objIndex)
		end
	elseif QS == 47 then
		if UserLv < 320 then
			ItemType = 14 ItemIndex = 28 ItemLevel = 4 ItemSkill = 0 ItemCnt = 2 Opt = 0 ExOpt = 0
			LuaQuestClass:DeleteInvenItem(ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt, objIndex)
		
			LuaQuestClass:SetQuestSwitch(Ep19, 48, objIndex)
		elseif UserLv >= 320 then
			LuaQuestClass:SetQuestSwitch(Ep19, 90, objIndex)
		end
	elseif QS >= 50 and QS <= 73 then	
		if UserLv < 320 then
			LuaQuestClass:SetQuestSwitch(Ep19, 45, objIndex)
		elseif UserLv >= 320 then
			LuaQuestClass:SetQuestSwitch(Ep19, 90, objIndex)
		end
	elseif QS >= 74 and QS <= 81 then	
		ItemType = 0 ItemIndex = 16 ItemLevel = 0 ItemSkill = 0 ItemCnt = 1 Opt = 0 ExOpt = 0
		LuaQuestClass:DeleteInvenItem(ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt, objIndex)

		if UserLv < 320 then
			LuaQuestClass:SetQuestSwitch(Ep19, 45, objIndex)
		elseif UserLv >= 320 then
			LuaQuestClass:SetQuestSwitch(Ep19, 90, objIndex)
		end

	elseif QS >= 82 and QS <= 89 then	
		ItemType = 2 ItemIndex = 5 ItemLevel = 0 ItemSkill = 0 ItemCnt = 1 Opt = 0 ExOpt = 0
		LuaQuestClass:DeleteInvenItem(ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt, objIndex)
		
		if UserLv < 320 then
			LuaQuestClass:SetQuestSwitch(Ep19, 45, objIndex)
		elseif UserLv >= 320 then
			LuaQuestClass:SetQuestSwitch(Ep19, 90, objIndex)
		end
	elseif QS == 92 then
		if UserLv < 350 then
			ItemType = 14 ItemIndex = 28 ItemLevel = 5 ItemSkill = 0 ItemCnt = 2 Opt = 0 ExOpt = 0
			LuaQuestClass:DeleteInvenItem(ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt, objIndex)

			LuaQuestClass:SetQuestSwitch(Ep19, 93, objIndex)
		elseif UserLv >= 350 then
			LuaQuestClass:SetQuestSwitch(Ep19, 135, objIndex)
		end
	elseif QS >= 95 and QS <= 126 then	
		if UserLv < 350 then
			LuaQuestClass:SetQuestSwitch(Ep19, 90, objIndex)
		elseif UserLv >= 350 then
			LuaQuestClass:SetQuestSwitch(Ep19, 135, objIndex)
		end
	elseif QS >= 127 and QS <= 134 then	
		ItemType = 5 ItemIndex = 17 ItemLevel = 0 ItemSkill = 0 ItemCnt = 1 Opt = 0 ExOpt = 0
		LuaQuestClass:DeleteInvenItem(ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt, objIndex)
		
		if UserLv < 350 then
			LuaQuestClass:SetQuestSwitch(Ep19, 90, objIndex)
		elseif UserLv >= 350 then
			LuaQuestClass:SetQuestSwitch(Ep19, 135, objIndex)
		end
	end

--------------------------------------	
	return
end

function Ep19_Complete(objIndex)

	local LimitLv = 349
		
	
	local QS = LuaQuestClass:GetQuestSwitch( Ep19, objIndex)
	
	if QS == 1 then
		LuaQuestClass:SetQuestSwitch( Ep19, 0, objIndex)
		LuaQuestClass:SendQuestComplete( Ep19, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif QS == 4 then
		LuaQuestClass:SetQuestSwitch( Ep19, 3, objIndex)
		LuaQuestClass:SendQuestComplete( Ep19, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif  QS == 46 then
		LuaQuestClass:SetQuestSwitch( Ep19, 45, objIndex)
		LuaQuestClass:SendQuestComplete( Ep19, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif QS == 49 then
		LuaQuestClass:SetQuestSwitch( Ep19, 48, objIndex)
		LuaQuestClass:SendQuestComplete( Ep19, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif  QS == 91 then
		LuaQuestClass:SetQuestSwitch( Ep19, 90, objIndex)
		LuaQuestClass:SendQuestComplete( Ep19, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif  QS == 94 then
		LuaQuestClass:SetQuestSwitch( Ep19, 93, objIndex)
		LuaQuestClass:SendQuestComplete( Ep19, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	end	

	
	local bComplete = LuaQuestClass:IsQuestComplete( Ep19, objIndex)--완료 체크
	
	if bComplete == false then
		return
	end


	local ChkCompleteQS = Ep19_ChkCompleteQS(QS)
	
	if ChkCompleteQS == 1 then
	
		local UserLv = LuaQuestClass:GetUserLv(objIndex)
		
		if UserLv <= LimitLv then
			local Result = LuaQuestClass:SendQuestReward(Ep19, objIndex)
			if Result == 1 then
				--LuaQuestClass:SetQuestSwitch(Ep19, 5, objIndex)
				Ep19_SendCompleteQS(QS, objIndex)
			end
			return
		elseif UserLv > LimitLv then
			local Result = LuaQuestClass:SendQuestReward(Ep19, objIndex)
			if Result == 1 then
				Ep19_SendCompleteQS(QS, objIndex)
				LuaQuestClass:SetQuestSwitch(Ep19, 135, objIndex)
			end

			return
		end
		
		return
	end

end

