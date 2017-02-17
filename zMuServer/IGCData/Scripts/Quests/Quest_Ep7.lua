--[[
Quest_Ep7.lua
[ Start  ] 	2009.12.
[ Latest ]	2009-12-14 16:45 PM
[ Version ]	0.0.0.1
[ Text ]									
[ Desc ]									
--]]
QuestInclude("Quest_Define.lua")

function Ep7_Progress(SelectSyntactic, objIndex)


	local QS = LuaQuestClass:GetQuestSwitch(Ep7, objIndex)
	--Print('1', 'qs = ', QS, 'SelectSyntactic = ' , SelectSyntactic)
-- 인벤 확장 퀘스트(창고 확장 설명)
	if QS == 0 then
		if SelectSyntactic == 1 then -- 뭔데
			LuaQuestClass:SendQuestProgress(Ep7, 1, objIndex)-- 
			--Print('qs = ', QS , 'SelectSyntactic = ', SelectSyntactic)
			return
		end
	end
	if QS == 1 then
		if SelectSyntactic == 1 then -- 그렇군
			LuaQuestClass:SendQuestProgress(Ep7, 2, objIndex)-- 
			--Print('qs = ', QS , 'SelectSyntactic = ', SelectSyntactic)
			return
		end
	end
-- 인벤 확장 퀘스트(창고 확장)
	if QS == 2 then
		if SelectSyntactic == 1 then -- 뭔데
			LuaQuestClass:SendQuestProgress(Ep7, 4, objIndex)-- 
			--Print('qs = ', QS , 'SelectSyntactic = ', SelectSyntactic)
			return
		end
		if SelectSyntactic == 2 then -- 지금은 바뻐.
			LuaQuestClass:SendQuestProgress(Ep7, 3, objIndex)-- 
			return
		end
	end
-- 인벤 확장 퀘스트(1차 인벤 확장 설명)
	if QS == 5 then
		if SelectSyntactic == 1 then -- 뭔데
			LuaQuestClass:SendQuestProgress(Ep7, 6, objIndex)-- 
			--Print('qs = ', QS , 'SelectSyntactic = ', SelectSyntactic)
			return
		end
	end
	if QS == 6 then
		if SelectSyntactic == 1 then -- 그렇군
			LuaQuestClass:SendQuestProgress(Ep7, 7, objIndex)-- 
			--Print('qs = ', QS , 'SelectSyntactic = ', SelectSyntactic)
			return
		end
	end
-- 인벤 확장 퀘스트(1차 인벤 확장)
	if QS == 7 then
		if SelectSyntactic == 1 then -- 뭔데
			LuaQuestClass:SendQuestProgress(Ep7, 9, objIndex)-- 
			--Print('qs = ', QS , 'SelectSyntactic = ', SelectSyntactic)
			return
		end
		if SelectSyntactic == 2 then -- 지금은 바뻐.
			LuaQuestClass:SendQuestProgress(Ep7, 8, objIndex)-- 
			return
		end
	end
-- 인벤 확장 퀘스트(2차 인벤 확장 설명)
	if QS == 10 then
		if SelectSyntactic == 1 then -- 뭔데
			LuaQuestClass:SendQuestProgress(Ep7, 11, objIndex)-- 
			--Print('qs = ', QS , 'SelectSyntactic = ', SelectSyntactic)
			return
		end
	end
	if QS == 11 then
		if SelectSyntactic == 1 then -- 그렇군
			LuaQuestClass:SendQuestProgress(Ep7, 12, objIndex)-- 
			--Print('qs = ', QS , 'SelectSyntactic = ', SelectSyntactic)
			return
		end
	end
-- 인벤 확장 퀘스트(2차 인벤 확장)
	if QS == 12 then
		if SelectSyntactic == 1 then -- 뭔데
			LuaQuestClass:SendQuestProgress(Ep7, 14, objIndex)-- 
			--Print('qs = ', QS , 'SelectSyntactic = ', SelectSyntactic)
			return
		end
		if SelectSyntactic == 2 then -- 지금은 바뻐.
			LuaQuestClass:SendQuestProgress(Ep7, 13, objIndex)-- 
			return
		end
	end
-- 인벤 확장 퀘스트(창고 확장 설명)
	if QS == 15 then
		if SelectSyntactic == 1 then -- 뭔데
			LuaQuestClass:SendQuestProgress(Ep7, 16, objIndex)-- 
			--Print('qs = ', QS , 'SelectSyntactic = ', SelectSyntactic)
			return
		end
	end
	if QS == 16 then
		if SelectSyntactic == 1 then -- 그렇군
			LuaQuestClass:SendQuestProgress(Ep7, 17, objIndex)-- 
			--Print('qs = ', QS , 'SelectSyntactic = ', SelectSyntactic)
			return
		end
	end
-- 인벤 확장 퀘스트(창고 확장)
	if QS == 17 then
		if SelectSyntactic == 1 then -- 뭔데
			LuaQuestClass:SendQuestProgress(Ep7, 19, objIndex)-- 
			--Print('qs = ', QS , 'SelectSyntactic = ', SelectSyntactic)
			return
		end
		if SelectSyntactic == 2 then -- 지금은 바뻐.
			LuaQuestClass:SendQuestProgress(Ep7, 18, objIndex)-- 
			return
		end
	end

end

function Ep7_ChkCompleteQS(QS)

	if QS == 4 or QS == 9 or QS == 14 or QS == 19 then
		return 1
	end
	
	return 0
end

function Ep7_SendCompleteQS(QS, objIndex)

	local UserLv = LuaQuestClass:GetUserLv(objIndex)
	local CharClass = LuaQuestClass:GetCharClass(objIndex)

	if QS == 4 then
		ItemType = 14 ItemIndex = 164 ItemLevel = 0 ItemSkill = 0 ItemCnt = 1  Opt = 0 ExOpt = 0 --고대의 조각상
		LuaQuestClass:DeleteInvenItem(ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt, objIndex)--고대의 조각상
		LuaQuestClass:SetQuestSwitch(Ep7, 5, objIndex)
	elseif QS == 9 then
		ItemType = 14 ItemIndex = 165 ItemLevel = 0 ItemSkill = 0 ItemCnt = 1  Opt = 0 ExOpt = 0  --마법의 옷감
		LuaQuestClass:DeleteInvenItem(ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt, objIndex)--마법의 옷감
		LuaQuestClass:SetQuestSwitch(Ep7, 10, objIndex)
	elseif QS == 14 then
		ItemType = 14 ItemIndex = 166 ItemLevel = 0 ItemSkill = 0 ItemCnt = 1  Opt = 0 ExOpt = 0 --공간의 옷감
		LuaQuestClass:DeleteInvenItem(ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt, objIndex)--공간의 옷감
		LuaQuestClass:SetQuestSwitch(Ep7, 15, objIndex)
	elseif QS == 19 then
		ItemType = 14 ItemIndex = 164 ItemLevel = 0 ItemSkill = 0 ItemCnt = 1  Opt = 0 ExOpt = 0 --고대의 조각상
		LuaQuestClass:DeleteInvenItem(ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt, objIndex)--고대의 조각상
		LuaQuestClass:SetQuestSwitch(Ep7, 20, objIndex)
	end

	return
end



function Ep7_Complete(objIndex)

	local LimitLv = 400
	
	
	local QS = LuaQuestClass:GetQuestSwitch(Ep7, objIndex)
	
	if QS == 3 then
		LuaQuestClass:SetQuestSwitch(Ep7, 0, objIndex)
		LuaQuestClass:SendQuestComplete(Ep7, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif QS == 8 then
		LuaQuestClass:SetQuestSwitch(Ep7, 5, objIndex)
		LuaQuestClass:SendQuestComplete(Ep7, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif  QS == 13 then
		LuaQuestClass:SetQuestSwitch(Ep7, 10, objIndex)
		LuaQuestClass:SendQuestComplete(Ep7, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif  QS == 18 then
		LuaQuestClass:SetQuestSwitch(Ep7, 15, objIndex)
		LuaQuestClass:SendQuestComplete(Ep7, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	end	

	
	local bComplete = LuaQuestClass:IsQuestComplete(Ep7, objIndex)--완료 체크
	
	if bComplete == false then
		return
	end
	
	local ChkCompleteQS = Ep7_ChkCompleteQS(QS)
	
	if ChkCompleteQS == 1 then
	
		local UserLv = LuaQuestClass:GetUserLv(objIndex)
		
		local Result = LuaQuestClass:SendQuestReward(Ep7, objIndex)
		if Result == 1 then
			Ep7_SendCompleteQS(QS, objIndex)
		end
	
		return
	end

end