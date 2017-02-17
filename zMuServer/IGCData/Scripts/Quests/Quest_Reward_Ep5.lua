--[[
Quest_Reward_Ep5.lua
[ Start  ] 	2009.08.
[ Latest ]	2009-12-14 16:45 PM
[ Version ]	0.0.0.1
[ Text ]									
[ Desc ]									
--]]

function Quest_Reward_Window_Ep5(iObjIndex)

	local ItemType = 0
	local ItemIndex = 0 
	local ItemLevel = 0 
	local dur = 0 
	local ItemSkill = 0 
	local ItemCnt = 0
	local Overlap = 0 -- 아이템이 겹쳐지는가?
	local Opt	= 0
	local ExOpt	= 0
	
---------------------------------------------------------------------------------------------------------------------
-- 환영사원 1지역
---------------------------------------------------------------------------------------------------------------------

-- 설명 듣기
	LuaQuestClass:SetQuestProgress(ASK1, Ep5, 1)--대화 진행상 완료창이 필요한 경우 사용
	
	LuaQuestClass:SetQuestRewardExp(Ep5, 8, 10000)


-- 몬스터 잡기
	LuaQuestClass:SetQuestProgress(ASK1, Ep5, 10)--대화 진행상 완료창이 필요한 경우 사용
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep5, 11, 60, 50, PartyPlay)--블러디 울프
	ItemType = 13 ItemIndex = 51 ItemLevel = 1 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep5, 11, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--피의두루마리 + 1

-- 환영사원에서 살아남기-- 테스트 필요
	LuaQuestClass:SetQuestProgress(ASK1, Ep5, 13)--대화 진행상 완료창이 필요한 경우 사용

	LuaQuestClass:SetQuestEventMapClear(ASK1, Ep5, 14, EVENT_MAP_CLEAR_ILLUSION, 1)
	ItemType = 14 ItemIndex = 14 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep5, 14, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--영혼의 보석

---------------------------------------------------------------------------------------------------------------------
-- 환영사원 2지역
---------------------------------------------------------------------------------------------------------------------

-- 설명 듣기
	LuaQuestClass:SetQuestProgress(ASK1, Ep5, 16)--대화 진행상 완료창이 필요한 경우 사용
	

	LuaQuestClass:SetQuestRewardExp(Ep5, 23, 20000)

-- 몬스터 잡기
	LuaQuestClass:SetQuestProgress(ASK1, Ep5, 25)--대화 진행상 완료창이 필요한 경우 사용
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep5, 26, 57, 70, PartyPlay)--아이언 휠
	ItemType = 13 ItemIndex = 51 ItemLevel = 2 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep5, 26, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--피의두루마리 + 2

	
-- 사원에서 살아남기- 차후 기능 구현
	LuaQuestClass:SetQuestProgress(ASK1, Ep5, 28)--대화 진행상 완료창이 필요한 경우 사용

	LuaQuestClass:SetQuestEventMapClear(ASK1, Ep5, 29, EVENT_MAP_CLEAR_ILLUSION, 2)
	ItemType = 14 ItemIndex = 13 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep5, 29, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--축복의 보석
	
---------------------------------------------------------------------------------------------------------------------
-- 환영사원 3지역
---------------------------------------------------------------------------------------------------------------------

-- 몬스터 잡기
	LuaQuestClass:SetQuestProgress(ASK1, Ep5, 32)--대화 진행상 완료창이 필요한 경우 사용
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep5, 33, 73, 90, PartyPlay)--드라칸
	ItemType = 13 ItemIndex = 51 ItemLevel = 3 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep5, 33, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--피의두루마리 + 3

-- 사원에서 살아남기- 차후 기능 구현
	LuaQuestClass:SetQuestProgress(ASK1, Ep5, 35)--대화 진행상 완료창이 필요한 경우 사용

	LuaQuestClass:SetQuestEventMapClear(ASK1, Ep5, 36, EVENT_MAP_CLEAR_ILLUSION, 3)
	ItemType = 14 ItemIndex = 16 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep5, 36, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--생명의 보석

---------------------------------------------------------------------------------------------------------------------
-- 환영사원 4지역
---------------------------------------------------------------------------------------------------------------------

-- 몬스터 잡기
	LuaQuestClass:SetQuestProgress(ASK1, Ep5, 38)--대화 진행상 완료창이 필요한 경우 사용
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep5, 39, 63, 30, PartyPlay)--데스빔나이트
	ItemType = 13 ItemIndex = 51 ItemLevel = 4 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep5, 39, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--피의두루마리 + 4

-- 사원에서 살아남기- 차후 기능 구현
	LuaQuestClass:SetQuestProgress(ASK1, Ep5, 41)--대화 진행상 완료창이 필요한 경우 사용

	LuaQuestClass:SetQuestEventMapClear(ASK1, Ep5, 42, EVENT_MAP_CLEAR_ILLUSION, 4)
	ItemType = 12 ItemIndex = 15 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep5, 42, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--혼돈의 보석
	ItemType = 14 ItemIndex = 14 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep5, 42, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--영혼의 보석
---------------------------------------------------------------------------------------------------------------------
-- 환영사원 5지역
---------------------------------------------------------------------------------------------------------------------

-- 몬스터 잡기
	LuaQuestClass:SetQuestProgress(ASK1, Ep5, 44)--대화 진행상 완료창이 필요한 경우 사용
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep5, 45, 75, 130, PartyPlay)--자이언트드라칸
	ItemType = 13 ItemIndex = 51 ItemLevel = 5 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep5, 45, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--피의두루마리 + 5

-- 사원에서 몬스터 잡기-- 테스트 필요
	LuaQuestClass:SetQuestProgress(ASK1, Ep5, 47)--대화 진행상 완료창이 필요한 경우 사용

	LuaQuestClass:SetQuestEventMapClear(ASK1, Ep5, 48, EVENT_MAP_CLEAR_ILLUSION, 5)
	ItemType = 14 ItemIndex = 14 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep5, 48, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--영혼의 보석
	ItemType = 14 ItemIndex = 13 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep5, 48, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--축복의 보석

---------------------------------------------------------------------------------------------------------------------
-- 환영사원 6지역
---------------------------------------------------------------------------------------------------------------------

-- 몬스터 잡기
	LuaQuestClass:SetQuestProgress(ASK1, Ep5, 50)--대화 진행상 완료창이 필요한 경우 사용
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep5, 51, 446, 150, PartyPlay)--쉐도우 룩
	ItemType = 13 ItemIndex = 51 ItemLevel = 6 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep5, 51, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--피의두루마리 + 6

-- 사원에서 몬스터 잡기-- 테스트 필요
	LuaQuestClass:SetQuestProgress(ASK1, Ep5, 53)--대화 진행상 완료창이 필요한 경우 사용

	LuaQuestClass:SetQuestEventMapClear(ASK1, Ep5, 54, EVENT_MAP_CLEAR_ILLUSION, 6)
	ItemType = 14 ItemIndex = 13 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep5, 54, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--축복의 보석
	ItemType = 14 ItemIndex = 16 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep5, 54, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--생명의 보석	

end


	
	