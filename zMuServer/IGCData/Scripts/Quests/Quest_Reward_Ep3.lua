--[[
Quest_Reward_Ep3.lua
[ Start  ] 	2009.08.
[ Latest ]	2009-12-14 16:45 PM
[ Version ]	0.0.0.1
[ Text ]									
[ Desc ]									
--]]

function Quest_Reward_Window_Ep3(iObjIndex)

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
-- 블러드 캐슬 1지역
---------------------------------------------------------------------------------------------------------------------

-- 설명 듣기
	LuaQuestClass:SetQuestProgress(ASK1, Ep3, 1)--대화 진행상 완료창이 필요한 경우 사용
	
	LuaQuestClass:SetQuestRewardExp(Ep3, 8, 10000)


-- 몬스터 잡기
	LuaQuestClass:SetQuestProgress(ASK1, Ep3, 10)--대화 진행상 완료창이 필요한 경우 사용
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep3, 11, 5, 40)--헬하운드
	ItemType = 13 ItemIndex = 18 ItemLevel = 1 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep3, 11, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--투명망토 + 1

-- 캐슬에서 몬스터 잡기--- 테스트 필요
	LuaQuestClass:SetQuestProgress(ASK1, Ep3, 13)--대화 진행상 완료창이 필요한 경우 사용

	LuaQuestClass:SetQuestEventMapKillPoint(ASK1, Ep3, 14, EVENT_MAP_BLOOD_GATE, 1, 1)
	LuaQuestClass:SetQuestRewardZen(Ep3, 14, 100000)
	
-- 캐슬에서 살아남기-- 테스트 필요
	LuaQuestClass:SetQuestProgress(ASK1, Ep3, 16)--대화 진행상 완료창이 필요한 경우 사용

	LuaQuestClass:SetQuestEventMapClear(ASK1, Ep3, 17, EVENT_MAP_CLEAR_BLOOD, 1)
	LuaQuestClass:SetQuestRewardZen(Ep3, 17, 200000)
	LuaQuestClass:SetQuestRewardExp(Ep3, 17, 100000)

---------------------------------------------------------------------------------------------------------------------
-- 블러드 캐슬 2지역
---------------------------------------------------------------------------------------------------------------------

-- 설명 듣기
	LuaQuestClass:SetQuestProgress(ASK1, Ep3, 19)--대화 진행상 완료창이 필요한 경우 사용
	

	LuaQuestClass:SetQuestRewardExp(Ep3, 26, 20000)

-- 몬스터 잡기
	LuaQuestClass:SetQuestProgress(ASK1, Ep3, 28)--대화 진행상 완료창이 필요한 경우 사용
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep3, 29, 18, 60)--고르곤
	ItemType = 13 ItemIndex = 18 ItemLevel = 2 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep3, 29, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--투명망토 + 2

-- 캐슬에서 몬스터 잡기--- 테스트 필요
	LuaQuestClass:SetQuestProgress(ASK1, Ep3, 31)--대화 진행상 완료창이 필요한 경우 사용

	LuaQuestClass:SetQuestEventMapKillPoint(ASK1, Ep3, 32, EVENT_MAP_BLOOD_GATE, 2, 1)
	LuaQuestClass:SetQuestRewardZen(Ep3, 32, 200000)
	
-- 캐슬에서 살아남기-- 테스트 필요
	LuaQuestClass:SetQuestProgress(ASK1, Ep3, 34)--대화 진행상 완료창이 필요한 경우 사용

	LuaQuestClass:SetQuestEventMapClear(ASK1, Ep3, 35, EVENT_MAP_CLEAR_BLOOD, 2)
	LuaQuestClass:SetQuestRewardZen(Ep3, 35, 300000)
	LuaQuestClass:SetQuestRewardExp(Ep3, 35, 400000)
	
---------------------------------------------------------------------------------------------------------------------
-- 블러드 캐슬 3지역
---------------------------------------------------------------------------------------------------------------------

-- 몬스터 잡기
	LuaQuestClass:SetQuestProgress(ASK1, Ep3, 38)--대화 진행상 완료창이 필요한 경우 사용
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep3, 39, 40, 80)--데쓰나이트
	ItemType = 13 ItemIndex = 18 ItemLevel = 3 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep3, 39, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--투명망토 + 3

-- 캐슬에서 몬스터 잡기--- 테스트 필요
	LuaQuestClass:SetQuestProgress(ASK1, Ep3, 41)--대화 진행상 완료창이 필요한 경우 사용

	LuaQuestClass:SetQuestEventMapKillPoint(ASK1, Ep3, 42, EVENT_MAP_BLOOD_GATE, 3, 1)
	LuaQuestClass:SetQuestRewardZen(Ep3, 42, 500000)
	LuaQuestClass:SetQuestRewardExp(Ep3, 42, 800000)
	
-- 캐슬에서 살아남기-- 테스트 필요
	LuaQuestClass:SetQuestProgress(ASK1, Ep3, 44)--대화 진행상 완료창이 필요한 경우 사용

	LuaQuestClass:SetQuestEventMapClear(ASK1, Ep3, 45, EVENT_MAP_CLEAR_BLOOD, 3)
	LuaQuestClass:SetQuestRewardExp(Ep3, 45, 1000000)
	ItemType = 12 ItemIndex = 15 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep3, 45, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--혼석

---------------------------------------------------------------------------------------------------------------------
-- 블러드 캐슬 4지역
---------------------------------------------------------------------------------------------------------------------

-- 몬스터 잡기
	LuaQuestClass:SetQuestProgress(ASK1, Ep3, 47)--대화 진행상 완료창이 필요한 경우 사용
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep3, 48, 71, 80, PartyPlay)--메가 크러스트
	ItemType = 13 ItemIndex = 18 ItemLevel = 4 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep3, 48, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--투명망토 + 4

-- 캐슬에서 몬스터 잡기--- 테스트 필요
	LuaQuestClass:SetQuestProgress(ASK1, Ep3, 50)--대화 진행상 완료창이 필요한 경우 사용

	LuaQuestClass:SetQuestEventMapKillPoint(ASK1, Ep3, 51, EVENT_MAP_BLOOD_GATE, 4, 1)
	LuaQuestClass:SetQuestRewardZen(Ep3, 51, 500000)
	ItemType = 12 ItemIndex = 15 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep3, 51, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--혼석
-- 캐슬에서 살아남기-- 테스트 필요
	LuaQuestClass:SetQuestProgress(ASK1, Ep3, 53)--대화 진행상 완료창이 필요한 경우 사용

	LuaQuestClass:SetQuestEventMapClear(ASK1, Ep3, 54, EVENT_MAP_CLEAR_BLOOD, 4)
	ItemType = 14 ItemIndex = 14 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep3, 54, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--영석

---------------------------------------------------------------------------------------------------------------------
-- 블러드 캐슬 5지역
---------------------------------------------------------------------------------------------------------------------

-- 몬스터 잡기
	LuaQuestClass:SetQuestProgress(ASK1, Ep3, 56)--대화 진행상 완료창이 필요한 경우 사용
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep3, 57, 58, 100, PartyPlay)--탄탈로스
	ItemType = 13 ItemIndex = 18 ItemLevel = 5 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep3, 57, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--투명망토 + 5

-- 캐슬에서 몬스터 잡기--- 테스트 필요
	LuaQuestClass:SetQuestProgress(ASK1, Ep3, 59)--대화 진행상 완료창이 필요한 경우 사용

	LuaQuestClass:SetQuestEventMapKillPoint(ASK1, Ep3, 60, EVENT_MAP_BLOOD_GATE, 5, 1)
	ItemType = 14 ItemIndex = 14 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep3, 60, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--영석
	
-- 캐슬에서 살아남기-- 테스트 필요
	LuaQuestClass:SetQuestProgress(ASK1, Ep3, 62)--대화 진행상 완료창이 필요한 경우 사용

	LuaQuestClass:SetQuestEventMapClear(ASK1, Ep3, 63, EVENT_MAP_CLEAR_BLOOD, 5)
	ItemType = 14 ItemIndex = 13 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep3, 63, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--축석	

---------------------------------------------------------------------------------------------------------------------
-- 블러드 캐슬 6지역
---------------------------------------------------------------------------------------------------------------------

-- 몬스터 잡기
	LuaQuestClass:SetQuestProgress(ASK1, Ep3, 65)--대화 진행상 완료창이 필요한 경우 사용
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep3, 66, 72, 100, PartyPlay)--팬텀나이트
	ItemType = 13 ItemIndex = 18 ItemLevel = 6 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep3, 66, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--투명망토 + 6

-- 캐슬에서 몬스터 잡기--- 테스트 필요
	LuaQuestClass:SetQuestProgress(ASK1, Ep3, 68)--대화 진행상 완료창이 필요한 경우 사용

	LuaQuestClass:SetQuestEventMapKillPoint(ASK1, Ep3, 69, EVENT_MAP_BLOOD_GATE, 6, 1)
	ItemType = 14 ItemIndex = 13 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep3, 69, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--축석
	
-- 캐슬에서 살아남기-- 테스트 필요
	LuaQuestClass:SetQuestProgress(ASK1, Ep3, 71)--대화 진행상 완료창이 필요한 경우 사용

	LuaQuestClass:SetQuestEventMapClear(ASK1, Ep3, 72, EVENT_MAP_CLEAR_BLOOD, 6)	
	ItemType = 14 ItemIndex = 16 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep3, 72, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--생석
	
---------------------------------------------------------------------------------------------------------------------
-- 블러드 캐슬 7지역
---------------------------------------------------------------------------------------------------------------------

-- 몬스터 잡기
	LuaQuestClass:SetQuestProgress(ASK1, Ep3, 74)--대화 진행상 완료창이 필요한 경우 사용
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep3, 75, 359, 100, PartyPlay)--트윈테일
	ItemType = 13 ItemIndex = 18 ItemLevel = 7 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep3, 75, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--투명망토 + 7

-- 캐슬에서 몬스터 잡기--- 테스트 필요
	LuaQuestClass:SetQuestProgress(ASK1, Ep3, 77)--대화 진행상 완료창이 필요한 경우 사용

	LuaQuestClass:SetQuestEventMapKillPoint(ASK1, Ep3, 78, EVENT_MAP_BLOOD_GATE, 7, 1)
	ItemType = 14 ItemIndex = 14 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 2
	LuaQuestClass:SetQuestRewardItem(Ep3, 78, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--영석
	
-- 캐슬에서 살아남기-- 테스트 필요
	LuaQuestClass:SetQuestProgress(ASK1, Ep3, 80)--대화 진행상 완료창이 필요한 경우 사용

	LuaQuestClass:SetQuestEventMapClear(ASK1, Ep3, 81, EVENT_MAP_CLEAR_BLOOD, 7)
	ItemType = 14 ItemIndex = 13 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 2
	LuaQuestClass:SetQuestRewardItem(Ep3, 81, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--축석	

---------------------------------------------------------------------------------------------------------------------
-- 블러드 캐슬 8지역
---------------------------------------------------------------------------------------------------------------------

-- 몬스터 잡기
	LuaQuestClass:SetQuestProgress(ASK1, Ep3, 83)--대화 진행상 완료창이 필요한 경우 사용
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep3, 84, 360, 100, PartyPlay)--드레드 피어
	ItemType = 13 ItemIndex = 18 ItemLevel = 8 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep3, 84, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--투명망토 + 8

-- 캐슬에서 몬스터 잡기--- 테스트 필요
	LuaQuestClass:SetQuestProgress(ASK1, Ep3, 86)--대화 진행상 완료창이 필요한 경우 사용

	LuaQuestClass:SetQuestEventMapKillPoint(ASK1, Ep3, 87, EVENT_MAP_BLOOD_GATE, 8, 1)
	ItemType = 14 ItemIndex = 13 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 2
	LuaQuestClass:SetQuestRewardItem(Ep3, 87, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--축석	
	
-- 캐슬에서 살아남기-- 테스트 필요
	LuaQuestClass:SetQuestProgress(ASK1, Ep3, 89)--대화 진행상 완료창이 필요한 경우 사용

	LuaQuestClass:SetQuestEventMapClear(ASK1, Ep3, 90, EVENT_MAP_CLEAR_BLOOD, 8)
	ItemType = 14 ItemIndex = 16 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 2
	LuaQuestClass:SetQuestRewardItem(Ep3, 90, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--생석	





	
end


	
	