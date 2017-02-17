--[[
Quest_Ep16.lua
[ Start  ] 	2009.08.
[ Latest ]	2009-09-15 15:50 PM
[ Version ]	0.0.0.0
[ Text ]									
[ Desc ]									
--]]
QuestInclude("Quest_Define.lua")

function Ep16_Progress(SelectSyntactic, objIndex)

	local QS = LuaQuestClass:GetQuestSwitch(Ep16, objIndex)
		
	if QS == 0 then
		if SelectSyntactic == 1 then -- 수락 
			LuaQuestClass:SendQuestProgress( Ep16, 2, objIndex)-- 
			return 
		end
		if SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress( Ep16, 1, objIndex)-- 
			return 
		end
	elseif QS == 3 then
		if SelectSyntactic == 1 then -- 수락 
			LuaQuestClass:SendQuestProgress( Ep16, 5, objIndex)-- 
			return 
		end
		if SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress( Ep16, 4, objIndex)-- 
			return 
		end
	elseif QS == 6 then
		if SelectSyntactic == 1 then -- 수락 
			LuaQuestClass:SendQuestProgress( Ep16, 8, objIndex)-- 
			return 
		end
		if SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress( Ep16, 7, objIndex)-- 
			return 
		end
	elseif QS == 9 then
		if SelectSyntactic == 1 then -- 수락 
			LuaQuestClass:SendQuestProgress( Ep16, 11, objIndex)-- 
			return 
		end
		if SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress( Ep16, 10, objIndex)-- 
			return 
		end
	elseif QS == 12 then
		if SelectSyntactic == 1 then -- 수락 
			LuaQuestClass:SendQuestProgress( Ep16, 14, objIndex)-- 
			return 
		end
		if SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress( Ep16, 13, objIndex)-- 
			return 
		end
	end
	

-- 선택문 모두 입력 시키기~
end

function Ep16_ChkCompleteQS(QS)

--	local QS = LuaQuestClass:GetQuestSwitch(Ep16, objIndex)

	if QS == 2 or QS == 5 or QS == 8 or QS == 11 or QS == 14 then
		return 1
	end
	
	return 0
end


function Ep16_Complete(objIndex)

	local LimitLv = 209
	
	
	local QS = LuaQuestClass:GetQuestSwitch(Ep16, objIndex)
	
	if QS == 1 then
		LuaQuestClass:SetQuestSwitch(Ep16, 0, objIndex)
		LuaQuestClass:SendQuestComplete(Ep16, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif QS == 4 then
		LuaQuestClass:SetQuestSwitch(Ep16, 3, objIndex)
		LuaQuestClass:SendQuestComplete(Ep16, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif  QS == 7 then
		LuaQuestClass:SetQuestSwitch(Ep16, 6, objIndex)
		LuaQuestClass:SendQuestComplete(Ep16, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif QS == 10 then
		LuaQuestClass:SetQuestSwitch(Ep16, 9, objIndex)
		LuaQuestClass:SendQuestComplete(Ep16, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif  QS == 13 then
		LuaQuestClass:SetQuestSwitch(Ep16, 12, objIndex)
		LuaQuestClass:SendQuestComplete(Ep16, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	end	

	
	local bComplete = LuaQuestClass:IsQuestComplete(Ep16, objIndex)--완료 체크
	
	if bComplete == false then
		return
	end


	local ChkCompleteQS = Ep16_ChkCompleteQS(QS)
	
	if ChkCompleteQS == 1 then
	
		local UserLv = LuaQuestClass:GetUserLv(objIndex)
		
			local Result = LuaQuestClass:SendQuestReward(Ep16, objIndex)
			if Result == 1 then
				--LuaQuestClass:SetQuestSwitch(Ep16, 5, objIndex)
				if QS == 2 then
					if UserLv < 220 then
						LuaQuestClass:SetQuestSwitch(Ep16, 0, objIndex)
					elseif UserLv >= 220 then
						LuaQuestClass:SetQuestSwitch(Ep16, 3, objIndex)
					end
	
				elseif QS == 5 then
					if UserLv < 250 then
						LuaQuestClass:SetQuestSwitch(Ep16, 3, objIndex)
					elseif UserLv >= 250 then
						LuaQuestClass:SetQuestSwitch(Ep16, 6, objIndex)
					end
	
				elseif QS == 8 then
					if UserLv < 300 then
						LuaQuestClass:SetQuestSwitch(Ep16, 6, objIndex)
					elseif UserLv >= 300 then
						LuaQuestClass:SetQuestSwitch(Ep16, 9, objIndex)
					end

				elseif QS == 11 then
					if UserLv < 380 then
						LuaQuestClass:SetQuestSwitch(Ep16, 9, objIndex)
					elseif UserLv >= 380 then
						LuaQuestClass:SetQuestSwitch(Ep16, 12, objIndex)
					end
				elseif QS == 14 then
					LuaQuestClass:SetQuestSwitch(Ep16, 12, objIndex)

				end
			end
			return
	end

end

