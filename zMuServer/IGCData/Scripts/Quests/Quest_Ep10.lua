--[[
Quest_Ep10.lua
[ Start  ] 	2011.07.18
[ Latest ]	2011-09-15 9:24 PM
[ Version ]	0.0.0.1
[ Text ]	마스터 스킬 트리 초기화 퀘스트(두번째)								
[ Desc ]									
--]]
--================================================================================================================
QuestInclude("Quest_Define.lua")
--================================================================================================================
function Ep10_Progress(SelectSyntactic, objIndex)
	
	local QS = LuaQuestClass:GetQuestSwitch(Ep10, objIndex)
	
	if QS == 0 then
		if SelectSyntactic == 1 then -- 수락
			LuaQuestClass:SendQuestProgress(Ep10, 1, objIndex)-- 
			return
		end
		
		if SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress(Ep10, 2, objIndex)-- 
			return
		end
		
 	elseif QS == 3 then
		if SelectSyntactic == 1 then -- 수락
			LuaQuestClass:SendQuestProgress(Ep10, 4, objIndex)-- 
			return
		end
		
		if SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress(Ep10, 5, objIndex)-- 
			return
		end
	end
end
--================================================================================================================
function Ep10_ChkCompleteQS(QS)

	if QS == 1 or QS == 4 then
		return 1
	end
	
	return 0
end
--================================================================================================================
function Ep10_SendCompleteQS(QS, objIndex)

	local UserLv = LuaQuestClass:GetUserLv(objIndex)

	if QS == 1 then
		ItemType = 13 ItemIndex = 149 ItemLevel = 0 ItemSkill = 0 ItemCnt = 10 Opt = 0 ExOpt = 0
		LuaQuestClass:DeleteInvenItem(ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt, objIndex)	-- 파란색 잉크
		
		LuaQuestClass:SetQuestSwitch(Ep10, 3, objIndex)
	elseif QS == 4 then
		ItemType = 13 ItemIndex = 151 ItemLevel = 0 ItemSkill = 0 ItemCnt = 1 Opt = 0 ExOpt = 0
		LuaQuestClass:DeleteInvenItem(ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt, objIndex)	-- 정신의 양피지
		
		LuaQuestClass:SetQuestSwitch(Ep10, 6, objIndex)
	return
	end
end
--================================================================================================================
function Ep10_Complete(objIndex)

	local QS = LuaQuestClass:GetQuestSwitch(Ep10, objIndex)
	
	if QS == 2 then
		LuaQuestClass:SetQuestSwitch(Ep10, 0, objIndex)
		LuaQuestClass:SendQuestComplete(Ep10, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif QS == 5 then
		LuaQuestClass:SetQuestSwitch(Ep10, 3, objIndex)
		LuaQuestClass:SendQuestComplete(Ep10, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	end

	local bComplete = LuaQuestClass:IsQuestComplete(Ep10, objIndex)--완료 체크

	if bComplete == false then
		return
	end	local ChkCompleteQS = Ep10_ChkCompleteQS(QS)
	
	
	if ChkCompleteQS == 1 then
		local UserLv = LuaQuestClass:GetUserLv(objIndex)
		local Result = LuaQuestClass:SendQuestReward(Ep10, objIndex)
			if Result == 1 then
					Ep10_SendCompleteQS(QS, objIndex)
			end
		return
	end
end
--================================================================================================================