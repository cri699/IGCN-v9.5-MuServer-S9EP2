--[[
Quest_Ep5.lua
[ Start  ] 	2009.12.
[ Latest ]	2009-12-14 16:45 PM
[ Version ]	0.0.0.1
[ Text ]									
[ Desc ]									
--]]
QuestInclude("Quest_Define.lua")

function Ep5_Progress(SelectSyntactic, objIndex)


	local QS = LuaQuestClass:GetQuestSwitch(Ep5, objIndex)
	--Print('1', 'qs = ', QS, 'SelectSyntactic = ' , SelectSyntactic)
-- 환영사원 1지역: 1차 연계 퀘스트
	if QS == 0 then
		if SelectSyntactic == 1 then -- 뭔데
			LuaQuestClass:SendQuestProgress(Ep5, 2, objIndex)-- 
			--Print('qs = ', QS , 'SelectSyntactic = ', SelectSyntactic)
			return
		end
		if SelectSyntactic == 2 then -- 지금은 바뻐.
			LuaQuestClass:SendQuestProgress(Ep5, 1, objIndex)-- 
			return
		end
	end
-- 환영사원 설명	
	if QS == 2 then
		if SelectSyntactic == 1 then -- 그렇군
			LuaQuestClass:SendQuestProgress(Ep5, 3, objIndex)-- 
			--Print('qs = ', QS , 'SelectSyntactic = ', SelectSyntactic)
			return
		end
	end
	
	if QS == 3 then
		if SelectSyntactic == 1 then -- 그렇군
			LuaQuestClass:SendQuestProgress(Ep5, 4, objIndex)-- 
			--Print('qs = ', QS , 'SelectSyntactic = ', SelectSyntactic)
			return
		end
	end

	if QS == 4 then
		if SelectSyntactic == 1 then -- 그렇군
			LuaQuestClass:SendQuestProgress(Ep5, 5, objIndex)-- 
			--Print('qs = ', QS , 'SelectSyntactic = ', SelectSyntactic)
			return
		end
	end

	if QS == 5 then
		if SelectSyntactic == 1 then -- 그렇군
			LuaQuestClass:SendQuestProgress(Ep5, 6, objIndex)-- 
			--Print('qs = ', QS , 'SelectSyntactic = ', SelectSyntactic)
			return
		end
	end
	if QS == 6 then
		if SelectSyntactic == 1 then -- 그렇군
			LuaQuestClass:SendQuestProgress(Ep5, 7, objIndex)-- 
			--Print('qs = ', QS , 'SelectSyntactic = ', SelectSyntactic)
			return
		end
	end
	if QS == 7 then
		if SelectSyntactic == 1 then -- 그렇군
			LuaQuestClass:SendQuestProgress(Ep5, 8, objIndex)-- 
			--Print('qs = ', QS , 'SelectSyntactic = ', SelectSyntactic)
			return
		end
	end
-- 환영사원 1지역: 2차 연계 퀘스트
	if QS == 9 then
		if SelectSyntactic == 1 then -- 뭔데
			LuaQuestClass:SendQuestProgress(Ep5, 11, objIndex)-- 
			--Print('qs = ', QS , 'SelectSyntactic = ', SelectSyntactic)
			return
		end
		if SelectSyntactic == 2 then -- 지금은 바뻐.
			LuaQuestClass:SendQuestProgress(Ep5, 10, objIndex)-- 
			return
		end
	end
-- 환영사원 1지역: 3차 연계 퀘스트
	if QS == 12 then
		if SelectSyntactic == 1 then -- 뭔데
			LuaQuestClass:SendQuestProgress(Ep5, 14, objIndex)-- 
			--Print('qs = ', QS , 'SelectSyntactic = ', SelectSyntactic)
			return
		end
		if SelectSyntactic == 2 then -- 지금은 바뻐.
			LuaQuestClass:SendQuestProgress(Ep5, 13, objIndex)-- 
			return
		end
	end
-- 환영사원 2지역: 1차 연계 퀘스트
	if QS == 15 then
		if SelectSyntactic == 1 then -- 뭔데
			LuaQuestClass:SendQuestProgress(Ep5, 17, objIndex)-- 
			--Print('qs = ', QS , 'SelectSyntactic = ', SelectSyntactic)
			return
		end
		if SelectSyntactic == 2 then -- 지금은 바뻐.
			LuaQuestClass:SendQuestProgress(Ep5, 16, objIndex)-- 
			return
		end
	end
-- 환영사원 설명	
	if QS == 17 then
		if SelectSyntactic == 1 then -- 그렇군
			LuaQuestClass:SendQuestProgress(Ep5, 18, objIndex)-- 
			--Print('qs = ', QS , 'SelectSyntactic = ', SelectSyntactic)
			return
		end
	end
	if QS == 18 then
		if SelectSyntactic == 1 then -- 그렇군
			LuaQuestClass:SendQuestProgress(Ep5, 19, objIndex)-- 
			--Print('qs = ', QS , 'SelectSyntactic = ', SelectSyntactic)
			return
		end
	end

	if QS == 19 then
		if SelectSyntactic == 1 then -- 그렇군
			LuaQuestClass:SendQuestProgress(Ep5, 20, objIndex)-- 
			--Print('qs = ', QS , 'SelectSyntactic = ', SelectSyntactic)
			return
		end
	end
	if QS == 20 then
		if SelectSyntactic == 1 then -- 그렇군
			LuaQuestClass:SendQuestProgress(Ep5, 21, objIndex)-- 
			--Print('qs = ', QS , 'SelectSyntactic = ', SelectSyntactic)
			return
		end
	end
	if QS == 21 then
		if SelectSyntactic == 1 then -- 그렇군
			LuaQuestClass:SendQuestProgress(Ep5, 22, objIndex)-- 
			--Print('qs = ', QS , 'SelectSyntactic = ', SelectSyntactic)
			return
		end
	end
	if QS == 22 then
		if SelectSyntactic == 1 then -- 그렇군
			LuaQuestClass:SendQuestProgress(Ep5, 23, objIndex)-- 
			--Print('qs = ', QS , 'SelectSyntactic = ', SelectSyntactic)
			return
		end
	end

-- 환영사원 2지역: 2차 연계 퀘스트
	if QS == 24 then
		if SelectSyntactic == 1 then -- 뭔데
			LuaQuestClass:SendQuestProgress(Ep5, 26, objIndex)-- 
			--Print('qs = ', QS , 'SelectSyntactic = ', SelectSyntactic)
			return
		end
		if SelectSyntactic == 2 then -- 지금은 바뻐.
			LuaQuestClass:SendQuestProgress(Ep5, 25, objIndex)-- 
			return
		end
	end
-- 환영사원 2지역: 3차 연계 퀘스트
	if QS == 27 then
		if SelectSyntactic == 1 then -- 뭔데
			LuaQuestClass:SendQuestProgress(Ep5, 29, objIndex)-- 
			--Print('qs = ', QS , 'SelectSyntactic = ', SelectSyntactic)
			return
		end
		if SelectSyntactic == 2 then -- 지금은 바뻐.
			LuaQuestClass:SendQuestProgress(Ep5, 28, objIndex)-- 
			return
		end
	end

-- 환영사원 3지역: 1차 연계 퀘스트
	if QS == 30 then
		if SelectSyntactic == 1 then -- 잘알았삼
			LuaQuestClass:SendQuestProgress(Ep5, 31, objIndex)-- 
			--Print('qs = ', QS , 'SelectSyntactic = ', SelectSyntactic)
			return
		end

	end
	if QS == 31 then
		if SelectSyntactic == 1 then -- 뭔데
			LuaQuestClass:SendQuestProgress(Ep5, 33, objIndex)-- 
			--Print('qs = ', QS , 'SelectSyntactic = ', SelectSyntactic)
			return
		end
		if SelectSyntactic == 2 then -- 지금은 바뻐.
			LuaQuestClass:SendQuestProgress(Ep5, 32, objIndex)-- 
			return
		end

	end
-- 환영사원 3지역: 2차 연계 퀘스트
	if QS == 34 then
		if SelectSyntactic == 1 then -- 뭔데
			LuaQuestClass:SendQuestProgress(Ep5, 36, objIndex)-- 
			--Print('qs = ', QS , 'SelectSyntactic = ', SelectSyntactic)
			return
		end
		if SelectSyntactic == 2 then -- 지금은 바뻐.
			LuaQuestClass:SendQuestProgress(Ep5, 35, objIndex)-- 
			return
		end
	end
-- 환영사원 4지역: 1차 연계 퀘스트
	if QS == 37 then
		if SelectSyntactic == 1 then -- 뭔데
			LuaQuestClass:SendQuestProgress(Ep5, 39, objIndex)-- 
			--Print('qs = ', QS , 'SelectSyntactic = ', SelectSyntactic)
			return
		end
		if SelectSyntactic == 2 then -- 지금은 바뻐.
			LuaQuestClass:SendQuestProgress(Ep5, 38, objIndex)-- 
			return
		end

	end
-- 환영사원 4지역: 2차 연계 퀘스트
	if QS == 40 then
		if SelectSyntactic == 1 then -- 뭔데
			LuaQuestClass:SendQuestProgress(Ep5, 42, objIndex)-- 
			--Print('qs = ', QS , 'SelectSyntactic = ', SelectSyntactic)
			return
		end
		if SelectSyntactic == 2 then -- 지금은 바뻐.
			LuaQuestClass:SendQuestProgress(Ep5, 41, objIndex)-- 
			return
		end
	end

-- 환영사원 5지역: 1차 연계 퀘스트
	if QS == 43 then
		if SelectSyntactic == 1 then -- 뭔데
			LuaQuestClass:SendQuestProgress(Ep5, 45, objIndex)-- 
			--Print('qs = ', QS , 'SelectSyntactic = ', SelectSyntactic)
			return
		end
		if SelectSyntactic == 2 then -- 지금은 바뻐.
			LuaQuestClass:SendQuestProgress(Ep5, 44, objIndex)-- 
			return
		end

	end
-- 환영사원 5지역: 2차 연계 퀘스트
	if QS == 46 then
		if SelectSyntactic == 1 then -- 뭔데
			LuaQuestClass:SendQuestProgress(Ep5, 48, objIndex)-- 
			--Print('qs = ', QS , 'SelectSyntactic = ', SelectSyntactic)
			return
		end
		if SelectSyntactic == 2 then -- 지금은 바뻐.
			LuaQuestClass:SendQuestProgress(Ep5, 47, objIndex)-- 
			return
		end
	end

-- 환영사원 6지역: 1차 연계 퀘스트
	if QS == 49 then
		if SelectSyntactic == 1 then -- 뭔데
			LuaQuestClass:SendQuestProgress(Ep5, 51, objIndex)-- 
			--Print('qs = ', QS , 'SelectSyntactic = ', SelectSyntactic)
			return
		end
		if SelectSyntactic == 2 then -- 지금은 바뻐.
			LuaQuestClass:SendQuestProgress(Ep5, 50, objIndex)-- 
			return
		end

	end
-- 환영사원 6지역: 2차 연계 퀘스트
	if QS == 52 then
		if SelectSyntactic == 1 then -- 뭔데
			LuaQuestClass:SendQuestProgress(Ep5, 54, objIndex)-- 
			--Print('qs = ', QS , 'SelectSyntactic = ', SelectSyntactic)
			return
		end
		if SelectSyntactic == 2 then -- 지금은 바뻐.
			LuaQuestClass:SendQuestProgress(Ep5, 53, objIndex)-- 
			return
		end
	end

end

function Ep5_ChkCompleteQS(QS)

	if QS == 11 or QS == 14
	or QS == 26 or QS == 29
	or QS == 33 or QS == 36
	or QS == 39 or QS == 42
	or QS == 45 or QS == 48
	or QS == 51 or QS == 54 then
		return 1
	end
	
	return 0
end

function Ep5_SendCompleteQS(QS, objIndex)



	if QS == 11 then
		LuaQuestClass:SetQuestSwitch(Ep5, 12, objIndex)
	elseif QS == 14 then 
		LuaQuestClass:SetQuestSwitch(Ep5, 15, objIndex)
	elseif QS == 26 then 
		LuaQuestClass:SetQuestSwitch(Ep5, 27, objIndex)
	elseif QS == 29 then
		LuaQuestClass:SetQuestSwitch(Ep5, 30, objIndex)
	elseif QS == 33 then
		LuaQuestClass:SetQuestSwitch(Ep5, 34, objIndex)
	elseif QS == 36 then
		LuaQuestClass:SetQuestSwitch(Ep5, 37, objIndex)
	elseif QS == 39 then
		LuaQuestClass:SetQuestSwitch(Ep5, 40, objIndex)
	elseif QS == 42 then
		LuaQuestClass:SetQuestSwitch(Ep5, 43, objIndex)
	elseif QS == 45 then
		LuaQuestClass:SetQuestSwitch(Ep5, 46, objIndex)
	elseif QS == 48 then
		LuaQuestClass:SetQuestSwitch(Ep5, 49, objIndex)
	elseif QS == 51 then
		LuaQuestClass:SetQuestSwitch(Ep5, 52, objIndex)
	elseif QS == 54 then
		LuaQuestClass:SetQuestSwitch(Ep5, 55, objIndex)
	end

	return
end



function Ep5_Complete(objIndex)

	local LimitLv = 400
	
	
	local QS = LuaQuestClass:GetQuestSwitch(Ep5, objIndex)
	
	if QS == 1 then
		LuaQuestClass:SetQuestSwitch(Ep5, 9, objIndex)
		LuaQuestClass:SendQuestComplete(Ep5, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif QS == 10 then
		LuaQuestClass:SetQuestSwitch(Ep5, 9, objIndex)
		LuaQuestClass:SendQuestComplete(Ep5, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif  QS == 13 then
		LuaQuestClass:SetQuestSwitch(Ep5, 12, objIndex)
		LuaQuestClass:SendQuestComplete(Ep5, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif  QS == 16 then
		LuaQuestClass:SetQuestSwitch(Ep5, 15, objIndex)
		LuaQuestClass:SendQuestComplete(Ep5, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif  QS == 25 then
		LuaQuestClass:SetQuestSwitch(Ep5, 24, objIndex)
		LuaQuestClass:SendQuestComplete(Ep5, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif  QS == 28 then
		LuaQuestClass:SetQuestSwitch(Ep5, 27, objIndex)
		LuaQuestClass:SendQuestComplete(Ep5, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif  QS == 32 then
		LuaQuestClass:SetQuestSwitch(Ep5, 31, objIndex)
		LuaQuestClass:SendQuestComplete(Ep5, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif  QS == 35 then
		LuaQuestClass:SetQuestSwitch(Ep5, 34, objIndex)
		LuaQuestClass:SendQuestComplete(Ep5, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif  QS == 38 then
		LuaQuestClass:SetQuestSwitch(Ep5, 37, objIndex)
		LuaQuestClass:SendQuestComplete(Ep5, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif  QS == 41 then
		LuaQuestClass:SetQuestSwitch(Ep5, 40, objIndex)
		LuaQuestClass:SendQuestComplete(Ep5, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif  QS == 44 then
		LuaQuestClass:SetQuestSwitch(Ep5, 43, objIndex)
		LuaQuestClass:SendQuestComplete(Ep5, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif  QS == 47 then
		LuaQuestClass:SetQuestSwitch(Ep5, 46, objIndex)
		LuaQuestClass:SendQuestComplete(Ep5, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif  QS == 50 then
		LuaQuestClass:SetQuestSwitch(Ep5, 49, objIndex)
		LuaQuestClass:SendQuestComplete(Ep5, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif  QS == 53 then
		LuaQuestClass:SetQuestSwitch(Ep5, 52, objIndex)
		LuaQuestClass:SendQuestComplete(Ep5, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	end	

	if QS == 8 then
		local Result = LuaQuestClass:SendQuestReward(Ep5, objIndex)
		if Result == 1 then
			LuaQuestClass:SetQuestSwitch(Ep5, 9, objIndex)
		end
		return
	elseif QS == 23 then
		local Result = LuaQuestClass:SendQuestReward(Ep5, objIndex)
		if Result == 1 then
			LuaQuestClass:SetQuestSwitch(Ep5, 24, objIndex)
		end
		return
	end	
	
	local bComplete = LuaQuestClass:IsQuestComplete(Ep5, objIndex)--완료 체크
	
	if bComplete == false then
		return
	end
	
	local ChkCompleteQS = Ep5_ChkCompleteQS(QS)
	
	if ChkCompleteQS == 1 then
	
		local UserLv = LuaQuestClass:GetUserLv(objIndex)
		
		if UserLv <= LimitLv then
			local Result = LuaQuestClass:SendQuestReward(Ep5, objIndex)
			if Result == 1 then
				Ep5_SendCompleteQS(QS, objIndex)
			end
			return
		elseif UserLv > LimitLv then
			local Result = LuaQuestClass:SendQuestReward(Ep5, objIndex)
			if Result == 1 then
				Ep5_SendCompleteQS(QS, objIndex)
				LuaQuestClass:SetQuestSwitch(Ep5, 55, objIndex)
			end

			return
		end
		
		return
	end

end