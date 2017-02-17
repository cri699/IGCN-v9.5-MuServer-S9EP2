--[[
Quest_Ep20.lua
[ Start  ] 	2010.06.22
[ Latest ]	2010-11-01 7:12 PM
[ Version ]	0.0.0.3
[ Text ]	일일 퀘스트 완료 후 다시 반복적으로 의뢰완료가 출력되는 현상
[ Line ]	415 - 416								
--]]
QuestInclude("Quest_Define.lua")
--================================================================================================================
function Ep20_Progress(SelectSyntactic, objIndex)

	local QS = LuaQuestClass:GetQuestSwitch( Ep20, objIndex)
		
	if QS == 0 then
		if SelectSyntactic == 1 then -- 수락 
			LuaQuestClass:SendQuestProgress( Ep20, 1, objIndex)-- 
			return 
		end
	elseif QS == 1 then
		if SelectSyntactic == 1 then -- 수락 
			LuaQuestClass:SendQuestProgress( Ep20, 2, objIndex)-- 
			return 
		end
	elseif QS == 2 then
		if SelectSyntactic == 1 then -- 수락 
			LuaQuestClass:SendQuestProgress( Ep20, 4, objIndex)-- 
			return 
		elseif SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress( Ep20, 3, objIndex)-- 
			return 
		end
	elseif QS == 5 then
		if SelectSyntactic == 1 then -- 수락 
			LuaQuestClass:SendQuestProgress( Ep20, 7, objIndex)-- 
			return 
		elseif SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress( Ep20, 6, objIndex)-- 
			return 
		end
	elseif QS == 10 then
		if SelectSyntactic == 1 then -- 수락 
			LuaQuestClass:SendQuestProgress( Ep20, 12, objIndex)-- 
			return 
		elseif SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress( Ep20, 11, objIndex)-- 
			return 
		end	
	elseif QS == 13 then
		if SelectSyntactic == 1 then -- 수락 
			LuaQuestClass:SendQuestProgress( Ep20, 15, objIndex)-- 
			return 
		elseif SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress( Ep20, 14, objIndex)-- 
			return 
		end	
	elseif QS == 17 then
		if SelectSyntactic == 1 then -- 수락 
			LuaQuestClass:SendQuestProgress( Ep20, 19, objIndex)-- 
			return 
		elseif SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress( Ep20, 18, objIndex)-- 
			return 
		end	
	elseif QS == 20 then
		if SelectSyntactic == 1 then -- 수락 
			LuaQuestClass:SendQuestProgress( Ep20, 22, objIndex)-- 
			return 
		elseif SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress( Ep20, 21, objIndex)-- 
			return 
		end
	elseif QS == 24 then
		if SelectSyntactic == 1 then -- 수락 
			LuaQuestClass:SendQuestProgress( Ep20, 26, objIndex)-- 
			return 
		elseif SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress( Ep20, 25, objIndex)-- 
			return 
		end
	elseif QS == 27 then
		if SelectSyntactic == 1 then -- 수락 
			LuaQuestClass:SendQuestProgress( Ep20, 29, objIndex)-- 
			return 
		elseif SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress( Ep20, 28, objIndex)-- 
			return 
		end
	elseif QS == 31 then
		if SelectSyntactic == 1 then -- 수락 
			LuaQuestClass:SendQuestProgress( Ep20, 33, objIndex)-- 
			return 
		elseif SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress( Ep20, 32, objIndex)-- 
			return 
		end
	elseif QS == 34 then
		if SelectSyntactic == 1 then -- 수락 
			LuaQuestClass:SendQuestProgress( Ep20, 36, objIndex)-- 
			return 
		elseif SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress( Ep20, 35, objIndex)-- 
			return 
		end		
	elseif QS == 38 then
		if SelectSyntactic == 1 then -- 수락 
			LuaQuestClass:SendQuestProgress( Ep20, 40, objIndex)-- 
			return 
		elseif SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress( Ep20, 39, objIndex)-- 
			return 
		end		
	elseif QS == 41 then
		if SelectSyntactic == 1 then -- 수락 
			LuaQuestClass:SendQuestProgress( Ep20, 43, objIndex)-- 
			return 
		elseif SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress( Ep20, 42, objIndex)-- 
			return 
		end		
	elseif QS == 45 then
		if SelectSyntactic == 1 then -- 수락 
			LuaQuestClass:SendQuestProgress( Ep20, 47, objIndex)-- 
			return 
		elseif SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress( Ep20, 46, objIndex)-- 
			return 
		end							
	elseif QS == 48 then
		if SelectSyntactic == 1 then -- 수락 
			LuaQuestClass:SendQuestProgress( Ep20, 50, objIndex)-- 
			return 
		elseif SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress( Ep20, 49, objIndex)-- 
			return 
		end							
	elseif QS == 52 then
		if SelectSyntactic == 1 then -- 수락 
			LuaQuestClass:SendQuestProgress( Ep20, 54, objIndex)-- 
			return 
		elseif SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress( Ep20, 53, objIndex)-- 
			return 
		end					
	
	elseif QS == 54 then
		if SelectSyntactic == 1 then -- 수락 
			LuaQuestClass:SendQuestProgress( Ep20, 55, objIndex)-- 
			return 
		end		
	elseif QS == 56 then
		if SelectSyntactic == 1 then -- 수락 
			LuaQuestClass:SendQuestProgress( Ep20, 58, objIndex)-- 
			return 
		elseif SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress( Ep20, 57, objIndex)-- 
			return 
		end		
	elseif QS == 59 then
		if SelectSyntactic == 1 then -- 수락 
			LuaQuestClass:SendQuestProgress( Ep20, 60, objIndex)-- 
			return 
		end		
	elseif QS == 63 then
		if SelectSyntactic == 1 then -- 수락 
			LuaQuestClass:SendQuestProgress( Ep20, 64, objIndex)-- 
			return 
		end		

-- 2레벨 의뢰서 일일퀘스트		
	elseif QS == 70 then 				-- 2레벨 의회서 받아라
		if SelectSyntactic == 1 then -- 수락 
			LuaQuestClass:SendQuestProgress( Ep20, 72, objIndex)-- 
			
			return 
		elseif SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress( Ep20, 71, objIndex)-- 
			return 
		end
	elseif QS == 75 then				-- 월요일
		if SelectSyntactic == 1 then -- 수락 
			LuaQuestClass:SendQuestProgress( Ep20, 77, objIndex)-- 
			return 
		elseif SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress( Ep20, 76, objIndex)-- 
			return 
		end		
	elseif QS == 79 then				-- 화요일
		if SelectSyntactic == 1 then -- 수락 
			LuaQuestClass:SendQuestProgress( Ep20, 81, objIndex)-- 
			return 
		elseif SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress( Ep20, 80, objIndex)-- 
			return 
		end		
	elseif QS == 83 then				-- 수요일
		if SelectSyntactic == 1 then -- 수락 
			LuaQuestClass:SendQuestProgress( Ep20, 85, objIndex)-- 
			return 
		elseif SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress( Ep20, 84, objIndex)-- 
			return 
		end		
	elseif QS == 87 then				-- 목요일
		if SelectSyntactic == 1 then -- 수락 
			LuaQuestClass:SendQuestProgress( Ep20, 89, objIndex)-- 
			return 
		elseif SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress( Ep20, 88, objIndex)-- 
			return 
		end		
	elseif QS == 91 then				-- 금요일
		if SelectSyntactic == 1 then -- 수락 
			LuaQuestClass:SendQuestProgress( Ep20, 93, objIndex)-- 
			return 
		elseif SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress( Ep20, 92, objIndex)-- 
			return 
		end		
	elseif QS == 95 then				-- 토요일
		if SelectSyntactic == 1 then -- 수락 
			LuaQuestClass:SendQuestProgress( Ep20, 97, objIndex)-- 
			return 
		elseif SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress( Ep20, 96, objIndex)-- 
			return 
		end		
	elseif QS == 99 then				-- 일요일
		if SelectSyntactic == 1 then -- 수락 
			LuaQuestClass:SendQuestProgress( Ep20, 101, objIndex)-- 
			return 
		elseif SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress( Ep20, 100, objIndex)-- 
			return 
		end
	elseif QS == 102 then
		if SelectSyntactic == 1 then	-- 수락
			LuaQuestClass:SendQuestProgress( Ep20, 103, objIndex)
		end
	end
end
--================================================================================================================
function Ep20_ChkCompleteQS(QS)
	
--	local QS = LuaQuestClass:GetQuestSwitch( Ep20, objIndex)

	if QS == 4 or QS == 7 or QS == 9
	or QS == 12 or QS == 15 or QS == 16
	or QS == 19 or QS == 22 or QS == 23
	or QS == 26 or QS == 29 or QS == 30
	or QS == 33 or QS == 36 or QS == 37
	or QS == 40 or QS == 43 or QS == 44
	or QS == 47 or QS == 50 or QS == 51
	or QS == 55 or QS == 58	or QS == 60
	or QS == 64 or QS == 72 or QS == 74
	or QS == 77 or QS == 78	or QS == 81
	or QS == 82 or QS == 85 or QS == 86
	or QS == 89 or QS == 90 or QS == 93
	or QS == 94 or QS == 97 or QS == 98
 	or QS == 101 or QS == 103 or QS == 107 
	then
		
		return 1
	end
	
	return 0
end
--================================================================================================================
function Ep20_SendCompleteQS(QS, objIndex)
	
--	local QS = LuaQuestClass:GetQuestSwitch(Ep20, objIndex)
	local UserLv = LuaQuestClass:GetUserLv(objIndex)

--여기에 분기 만들고
	if QS == 4 then
		if UserLv > 300 and UserLv <= 400 then
			LuaQuestClass:SubQuestZen(1000000, objIndex)
			LuaQuestClass:SetQuestSwitch(Ep20, 5, objIndex)
			
		elseif UserLv > 400 then
			LuaQuestClass:SubQuestZen(1000000, objIndex)
			LuaQuestClass:SetQuestSwitch(Ep20, 70, objIndex)
			
		end

	DebugPrint('QS4:', QS)
-- 400 레벨 이하 요일 퀘스트 1레벨 의뢰서
	elseif QS == 7 then
		LuaQuestClass:SubQuestZen(500000, objIndex)
		LuaQuestClass:SetQuestSwitch(Ep20, 8, objIndex)

	-- 월요일 퀘스트 시작
	elseif QS == 9 then
		LuaQuestClass:SetQuestSwitch(Ep20, 10, objIndex)
		
	elseif QS == 12 then
		LuaQuestClass:SetQuestSwitch(Ep20, 13, objIndex)

	elseif QS == 15 then
		LuaQuestClass:SetQuestSwitch(Ep20, 59, objIndex)
	-- 화요일 퀘스트 시작
	elseif QS == 16 then
		LuaQuestClass:SetQuestSwitch(Ep20, 17, objIndex)
		
	elseif QS == 19 then
		ItemType = 14 ItemIndex = 153 ItemLevel = 0 ItemSkill = 0 ItemCnt = 20 Opt = 0 ExOpt = 0
		LuaQuestClass:DeleteInvenItem(ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt, objIndex)--스타더스트

		LuaQuestClass:SetQuestSwitch(Ep20, 20, objIndex)

	elseif QS == 22 then
		ItemType = 14 ItemIndex = 154 ItemLevel = 0 ItemSkill = 0 ItemCnt = 10 Opt = 0 ExOpt = 0
		LuaQuestClass:DeleteInvenItem(ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt, objIndex)--칼트석
		LuaQuestClass:SetQuestSwitch(Ep20, 59, objIndex)
	-- 수요일 퀘스트 시작
	elseif QS == 23 then
		DebugPrint('여기?')
		LuaQuestClass:SetQuestSwitch(Ep20, 24, objIndex)

	elseif QS == 26 then
		LuaQuestClass:SetQuestSwitch(Ep20, 27, objIndex)

	elseif QS == 29 then
		LuaQuestClass:SetQuestSwitch(Ep20, 59, objIndex)
	-- 목요일 퀘스트 시작
	elseif QS == 30 then
		LuaQuestClass:SetQuestSwitch(Ep20, 31, objIndex)
		
	elseif QS == 33 then
		LuaQuestClass:SetQuestSwitch(Ep20, 34, objIndex)

	elseif QS == 36 then
		LuaQuestClass:SetQuestSwitch(Ep20, 59, objIndex)
	-- 금요일 퀘스트 시작
	elseif QS == 37 then
		LuaQuestClass:SetQuestSwitch(Ep20, 38, objIndex)

	elseif QS == 40 then
		ItemType = 14 ItemIndex = 155 ItemLevel = 0 ItemSkill = 0 ItemCnt = 20 Opt = 0 ExOpt = 0
		LuaQuestClass:DeleteInvenItem(ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt, objIndex)--탄탈로스의 갑옷
		LuaQuestClass:SetQuestSwitch(Ep20, 41, objIndex)

	elseif QS == 43 then
		ItemType = 14 ItemIndex = 156 ItemLevel = 0 ItemSkill = 0 ItemCnt = 10 Opt = 0 ExOpt = 0
		LuaQuestClass:DeleteInvenItem(ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt, objIndex)--도살자의 몽둥이
		LuaQuestClass:SetQuestSwitch(Ep20, 59, objIndex)
	-- 토요일 퀘스트 시작
	elseif QS == 44 then
		LuaQuestClass:SetQuestSwitch(Ep20, 45, objIndex)

	elseif QS == 47 then
		LuaQuestClass:SetQuestSwitch(Ep20, 48, objIndex)

	elseif QS == 50 then
		LuaQuestClass:SetQuestSwitch(Ep20, 59, objIndex)
	-- 일요일 퀘스트 시작
	elseif QS == 51 then
		LuaQuestClass:SetQuestSwitch(Ep20, 52, objIndex)

	elseif QS == 55 then
		LuaQuestClass:SetQuestSwitch(Ep20, 56, objIndex)

	elseif QS == 58 then
		LuaQuestClass:SetQuestSwitch(Ep20, 61, objIndex)
		LuaQuestClass:SetQuestTimeLimit(Ep20, 61, objIndex, 5)

	elseif QS == 60 then
		ItemType = 14 ItemIndex = 152 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
		LuaQuestClass:DeleteInvenItem(ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt, objIndex)--1레벨 의뢰 완료 확인서
		LuaQuestClass:SetQuestSwitch(Ep20, 61, objIndex)
		LuaQuestClass:SetQuestTimeLimit(Ep20, 61, objIndex, 1440)
	elseif QS == 64 then
		ItemType = 14 ItemIndex = 151 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
		LuaQuestClass:DeleteInvenItem(ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt, objIndex)--1레벨 의뢰서
		LuaQuestClass:SetQuestSwitch(Ep20, 65, objIndex)

-- 아케론 일일 퀘스트(2레벨 의뢰서)

	elseif QS == 72 then
		LuaQuestClass:SubQuestZen(500000, objIndex)
		LuaQuestClass:SetQuestSwitch(Ep20, 73, objIndex)
		
	elseif QS == 74 then
		LuaQuestClass:SetQuestSwitch(Ep20, 75, objIndex)
	elseif QS == 77 then
		LuaQuestClass:SetQuestSwitch(Ep20, 102, objIndex)
	elseif QS == 78 then
		LuaQuestClass:SetQuestSwitch(Ep20, 79, objIndex)
	elseif QS == 81 then
		LuaQuestClass:SetQuestSwitch(Ep20, 102, objIndex)
	elseif QS == 82 then
		LuaQuestClass:SetQuestSwitch(Ep20, 83, objIndex)
	elseif QS == 85 then
		LuaQuestClass:SetQuestSwitch(Ep20, 102, objIndex)
	elseif QS == 86 then
		LuaQuestClass:SetQuestSwitch(Ep20, 87, objIndex)
	elseif QS == 89 then
		LuaQuestClass:SetQuestSwitch(Ep20, 102, objIndex)
	elseif QS == 90 then
		LuaQuestClass:SetQuestSwitch(Ep20, 91, objIndex)
	elseif QS == 93 then
		LuaQuestClass:SetQuestSwitch(Ep20, 102, objIndex)
	elseif QS == 94 then
		LuaQuestClass:SetQuestSwitch(Ep20, 95, objIndex)
	elseif QS == 97 then
		LuaQuestClass:SetQuestSwitch(Ep20, 102, objIndex)
	elseif QS == 98 then
		LuaQuestClass:SetQuestSwitch(Ep20, 99, objIndex)
	elseif QS == 101 then
		LuaQuestClass:SetQuestSwitch(Ep20, 102, objIndex)
	elseif QS == 103 then
		ItemType = 14 ItemIndex = 171 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
		LuaQuestClass:DeleteInvenItem(ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt, objIndex)	-- 2레벨 의뢰 완료 확인서
		LuaQuestClass:SetQuestSwitch(Ep20, 104, objIndex)
		LuaQuestClass:SetQuestTimeLimit(Ep20, 104, objIndex, 1440)
	elseif QS == 107 then
		ItemType = 14 ItemIndex = 151 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
		LuaQuestClass:DeleteInvenItem(ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt, objIndex)--1레벨 의뢰서
		LuaQuestClass:SetQuestSwitch(Ep20, 65, objIndex)
	end

	return
end
--================================================================================================================
function Ep20_Complete(objIndex)
	
	local LimitLv = 400
		
	
	local QS = LuaQuestClass:GetQuestSwitch( Ep20, objIndex)
	
	if QS == 3 then
		LuaQuestClass:SetQuestSwitch( Ep20, 2, objIndex)
		LuaQuestClass:SendQuestComplete( Ep20, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif QS == 6 then
		LuaQuestClass:SetQuestSwitch( Ep20, 5, objIndex)
		LuaQuestClass:SendQuestComplete( Ep20, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif QS == 11 then
		LuaQuestClass:SetQuestSwitch( Ep20, 10, objIndex)
		LuaQuestClass:SendQuestComplete( Ep20, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
						
	elseif QS == 14 then
		LuaQuestClass:SetQuestSwitch( Ep20, 13, objIndex)
		LuaQuestClass:SendQuestComplete( Ep20, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	
	elseif QS == 18 then
		LuaQuestClass:SetQuestSwitch( Ep20, 17, objIndex)
		LuaQuestClass:SendQuestComplete( Ep20, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
		
	elseif QS == 21 then
		LuaQuestClass:SetQuestSwitch( Ep20, 20, objIndex)
		LuaQuestClass:SendQuestComplete( Ep20, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
		
	elseif QS == 25 then
		LuaQuestClass:SetQuestSwitch( Ep20, 24, objIndex)
		LuaQuestClass:SendQuestComplete( Ep20, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
		
	elseif QS == 28 then
		LuaQuestClass:SetQuestSwitch( Ep20, 27, objIndex)
		LuaQuestClass:SendQuestComplete( Ep20, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
		
	elseif QS == 32 then
		LuaQuestClass:SetQuestSwitch( Ep20, 31, objIndex)
		LuaQuestClass:SendQuestComplete( Ep20, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif QS == 35 then
		LuaQuestClass:SetQuestSwitch( Ep20, 34, objIndex)
		LuaQuestClass:SendQuestComplete( Ep20, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
			
	elseif QS == 39 then
		LuaQuestClass:SetQuestSwitch( Ep20, 38, objIndex)
		LuaQuestClass:SendQuestComplete( Ep20, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
			
	elseif QS == 42 then
		LuaQuestClass:SetQuestSwitch( Ep20, 41, objIndex)
		LuaQuestClass:SendQuestComplete( Ep20, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
			
	elseif QS == 46 then
		LuaQuestClass:SetQuestSwitch( Ep20, 45, objIndex)
		LuaQuestClass:SendQuestComplete( Ep20, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
			
	elseif QS == 49 then
		LuaQuestClass:SetQuestSwitch( Ep20, 48, objIndex)
		LuaQuestClass:SendQuestComplete( Ep20, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
			
	elseif QS == 53 then
		LuaQuestClass:SetQuestSwitch( Ep20, 52, objIndex)
		LuaQuestClass:SendQuestComplete( Ep20, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
			
	elseif QS == 57 then
		LuaQuestClass:SetQuestSwitch( Ep20, 56, objIndex)
		LuaQuestClass:SendQuestComplete( Ep20, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
--	elseif QS == 61 then
--		LuaQuestClass:SendQuestConfirmBtn( Ep20, 61, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
--		return
	elseif QS == 62 then
		LuaQuestClass:SetQuestSwitch( Ep20, 63, objIndex)
		LuaQuestClass:SendQuestComplete( Ep20, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return

	elseif QS == 71 then
		LuaQuestClass:SetQuestSwitch( Ep20, 70, objIndex)
		LuaQuestClass:SendQuestComplete( Ep20, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif QS == 76 then
		LuaQuestClass:SetQuestSwitch( Ep20, 75, objIndex)
		LuaQuestClass:SendQuestComplete( Ep20, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif QS == 80 then
		LuaQuestClass:SetQuestSwitch( Ep20, 79, objIndex)
		LuaQuestClass:SendQuestComplete( Ep20, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return			
	elseif QS == 84 then
		LuaQuestClass:SetQuestSwitch( Ep20, 83, objIndex)
		LuaQuestClass:SendQuestComplete( Ep20, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	
	elseif QS == 88 then
		LuaQuestClass:SetQuestSwitch( Ep20, 87, objIndex)
		LuaQuestClass:SendQuestComplete( Ep20, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
		
	elseif QS == 92 then
		LuaQuestClass:SetQuestSwitch( Ep20, 91, objIndex)
		LuaQuestClass:SendQuestComplete( Ep20, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
		
	elseif QS == 96 then
		LuaQuestClass:SetQuestSwitch( Ep20, 95, objIndex)
		LuaQuestClass:SendQuestComplete( Ep20, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
		
	elseif QS == 100 then
		LuaQuestClass:SetQuestSwitch( Ep20, 99, objIndex)
		LuaQuestClass:SendQuestComplete( Ep20, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return

--	elseif QS == 104 then
--		LuaQuestClass:SendQuestConfirmBtn( Ep20, 104, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
--		return
	elseif QS == 105 then
		LuaQuestClass:SetQuestSwitch( Ep20, 104, objIndex)
		LuaQuestClass:SendQuestComplete( Ep20, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return

	end	

	local bComplete = LuaQuestClass:IsQuestComplete( Ep20, objIndex)--완료 체크
	
	DebugPrint('bComplete:', bComplete)
	if bComplete == false then
		return
	end

	
	local ChkCompleteQS = Ep20_ChkCompleteQS(QS)
	
	if ChkCompleteQS == 1 then
		
		local UserLv = LuaQuestClass:GetUserLv(objIndex)
		
		if UserLv >= 350 and UserLv <= 400 then
			local Result = LuaQuestClass:SendQuestReward(Ep20, objIndex)
			
			if Result == 1 then
				Ep20_SendCompleteQS(QS, objIndex)
			end

			return
			
		elseif UserLv > 400 then
			local Result = LuaQuestClass:SendQuestReward(Ep20, objIndex)
			
			if Result == 1 then
				Ep20_SendCompleteQS(QS, objIndex)
			end

			return
		end
		
		return
	end

end
--================================================================================================================