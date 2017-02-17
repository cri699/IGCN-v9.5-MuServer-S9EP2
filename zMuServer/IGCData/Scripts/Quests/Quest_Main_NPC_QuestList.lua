--[[
Quest_Main_NPC_QuestList.lua
[ Start  ] 	2010.06.
[ Latest ]	2010-06-30 11:15 AM
[ Version ]	0.0.0.1
[ Text ]									
[ Desc ]									
--]]


--[[
local CharMaster = LuaQuestClass:IsMasterLevel(objIndex)
리턴값 1이면 마스터 레벨 0 이면 아님
--]]

function Ep19_NPCQuestList(iObjIndex)

	local ProgEp = ChkProgQuest(Ep19, iObjIndex)-- 진행중인 퀘스트가 존재 하냐?
	
	if ProgEp == 1 then
		return
	end		
	
	local QS = LuaQuestClass:GetQuestSwitch(Ep19, iObjIndex) -- 현재 QuestSwitch
	
	local UserLv = LuaQuestClass:GetUserLv(iObjIndex)

	if UserLv >= 290 and UserLv <= 319 then
		QS = LuaQuestClass:GetQuestSwitch(Ep19, iObjIndex)
		if QS < 45 and ProgEp ~= 1 then
			LuaQuestClass:SetQuestSwitch(Ep19, 45, iObjIndex)

		end

	elseif UserLv >= 320 and UserLv <= 349 then
		QS = LuaQuestClass:GetQuestSwitch(Ep19, iObjIndex)

		if QS < 90 and ProgEp ~= 1 then
			LuaQuestClass:SetQuestSwitch(Ep19, 90, iObjIndex)
		end
	end
end
--[[
function Ep20_NPCQuestList(iObjIndex)

	local ProgEp = ChkProgQuest(Ep20, iObjIndex)-- 진행중인 퀘스트가 존재 하냐?
	local CharMaster = LuaQuestClass:IsMasterLevel(iObjIndex) --마스터 레벨인지 판별
	
	if ProgEp == 1 then
		return
	end		
				DebugPrint("마스터 레벨")	
	local QS = LuaQuestClass:GetQuestSwitch(Ep20, iObjIndex) -- 현재 QuestSwitch
	
	local UserLv = LuaQuestClass:GetUserLv(iObjIndex)

		if CharMaster == 0 then
			if UserLv >= 350 and UserLv <= 400 then

				QS = LuaQuestClass:GetQuestSwitch(Ep20, iObjIndex)
				local GetQuestTimeLimitState = 0;

				GetQuestTimeLimitState = LuaQuestClass:GetQuestTimeLimitState(Ep20, iObjIndex)

				if QS == 61 then		
					if GetQuestTimeLimitState == 0 then		

					end
				end
			end
		elseif CharMaster == 1 then
	if UserLv >= 401 then

				QS = LuaQuestClass:GetQuestSwitch(Ep20, iObjIndex)
				local GetQuestTimeLimitState = 0;

				GetQuestTimeLimitState = LuaQuestClass:GetQuestTimeLimitState(Ep20, iObjIndex)

				if QS == 61 and QS == 104 then		
					if GetQuestTimeLimitState == 0 then	
					end				

				end
		end


end
--]]


