--[[
Quest_Ep9.lua
[ Start  ] 	2011.07.18
[ Latest ]	2011-09-14 12:03 PM
[ Version ]	0.0.0.1
[ Text ]	마스터 스킬 트리 초기화 퀘스트(첫번째)								
[ Desc ]									
--]]
--================================================================================================================
QuestInclude("Quest_Define.lua")
--================================================================================================================
function Ep9_Progress(SelectSyntactic, objIndex)
	
	local QS = LuaQuestClass:GetQuestSwitch(Ep9, objIndex)
	
	if QS == 0 then
		if SelectSyntactic == 1 then -- 수락
			LuaQuestClass:SendQuestProgress(Ep9, 1, objIndex)-- 
			return
		end
		
		if SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress(Ep9, 2, objIndex)-- 
			return
		end
		
 	elseif QS == 3 then
		if SelectSyntactic == 1 then -- 수락
			LuaQuestClass:SendQuestProgress(Ep9, 4, objIndex)-- 
			return
		end
		
		if SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress(Ep9, 5, objIndex)-- 
			return
		end
	end
end
--================================================================================================================
function Ep9_ChkCompleteQS(QS)

	if QS == 1 or QS == 4 then
		return 1
	end
	
	return 0
end
--================================================================================================================
function Ep9_SendCompleteQS(QS, objIndex)

	local UserLv = LuaQuestClass:GetUserLv(objIndex)

	if QS == 1 then
		ItemType = 13 ItemIndex = 148 ItemLevel = 0 ItemSkill = 0 ItemCnt = 10 Opt = 0 ExOpt = 0
		LuaQuestClass:DeleteInvenItem(ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt, objIndex)	-- 초록색 잉크
		
		LuaQuestClass:SetQuestSwitch(Ep9, 3, objIndex)
	elseif QS == 4 then
		ItemType = 13 ItemIndex = 151 ItemLevel = 0 ItemSkill = 0 ItemCnt = 1 Opt = 0 ExOpt = 0
		LuaQuestClass:DeleteInvenItem(ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt, objIndex)	-- 정신의 양피지
		
		LuaQuestClass:SetQuestSwitch(Ep9, 6, objIndex)
	return
	end
end
--================================================================================================================
function Ep9_Complete(objIndex)

	local QS = LuaQuestClass:GetQuestSwitch(Ep9, objIndex)
	
	if QS == 2 then
		LuaQuestClass:SetQuestSwitch(Ep9, 0, objIndex)
		LuaQuestClass:SendQuestComplete(Ep9, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif QS == 5 then
		LuaQuestClass:SetQuestSwitch(Ep9, 3, objIndex)
		LuaQuestClass:SendQuestComplete(Ep9, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	end

	local bComplete = LuaQuestClass:IsQuestComplete(Ep9, objIndex)--완료 체크
	DebugPrint('bComplete: ', bComplete)
	if bComplete == false then
		return
	end	local ChkCompleteQS = Ep9_ChkCompleteQS(QS)
	
	
	if ChkCompleteQS == 1 then
		local UserLv = LuaQuestClass:GetUserLv(objIndex)
		local Result = LuaQuestClass:SendQuestReward(Ep9, objIndex)
			if Result == 1 then
					Ep9_SendCompleteQS(QS, objIndex)
					
				
			end
		return
	end
end
--================================================================================================================