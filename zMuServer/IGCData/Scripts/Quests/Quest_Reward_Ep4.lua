--[[
Quest_Reward_Ep4.lua
[ Start  ] 	2009.08.
[ Latest ]	2009-12-14 16:45 PM
[ Version ]	0.0.0.1
[ Text ]									
[ Desc ]									
--]]

function Quest_Reward_Window_Ep4(iObjIndex)

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
-- 악마의광장 1지역
---------------------------------------------------------------------------------------------------------------------

-- 설명 듣기
	LuaQuestClass:SetQuestProgress(ASK1, Ep4, 1)--대화 진행상 완료창이 필요한 경우 사용
	
	LuaQuestClass:SetQuestRewardExp(Ep4, 8, 10000)


-- 몬스터 잡기
	LuaQuestClass:SetQuestProgress(ASK1, Ep4, 10)--대화 진행상 완료창이 필요한 경우 사용
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep4, 11, 422, 40)--저주받은 리치
	ItemType = 14 ItemIndex = 19 ItemLevel = 1 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep4, 11, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--데빌 스퀘어초대권 + 1

-- 광장에서 몬스터 잡기-- 테스트 필요
	LuaQuestClass:SetQuestProgress(ASK1, Ep4, 13)--대화 진행상 완료창이 필요한 경우 사용

	LuaQuestClass:SetQuestEventMapDevilPoint(ASK1, Ep4, 14, 1, 50)
	LuaQuestClass:SetQuestRewardZen(Ep4, 14, 100000)
	
-- 광장에서 살아남기-- 테스트 필요
	LuaQuestClass:SetQuestProgress(ASK1, Ep4, 16)--대화 진행상 완료창이 필요한 경우 사용

	LuaQuestClass:SetQuestEventMapClear(ASK1, Ep4, 17, EVENT_MAP_CLEAR_DEVIL, 1)
	LuaQuestClass:SetQuestRewardZen(Ep4, 17, 200000)
	LuaQuestClass:SetQuestRewardExp(Ep4, 17, 100000)

---------------------------------------------------------------------------------------------------------------------
-- 악마의광장 2지역
---------------------------------------------------------------------------------------------------------------------

-- 설명 듣기
	LuaQuestClass:SetQuestProgress(ASK1, Ep4, 19)--대화 진행상 완료창이 필요한 경우 사용
	

	LuaQuestClass:SetQuestRewardExp(Ep4, 26, 20000)

-- 몬스터 잡기
	LuaQuestClass:SetQuestProgress(ASK1, Ep4, 28)--대화 진행상 완료창이 필요한 경우 사용
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep4, 29, 41, 60)--데스카우
	ItemType = 14 ItemIndex = 19 ItemLevel = 2 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep4, 29, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--데빌 스퀘어초대권 + 2

-- 광장에서 몬스터 잡기-- 테스트 필요
	LuaQuestClass:SetQuestProgress(ASK1, Ep4, 31)--대화 진행상 완료창이 필요한 경우 사용

	LuaQuestClass:SetQuestEventMapDevilPoint(ASK1, Ep4, 32, 2, 100)
	LuaQuestClass:SetQuestRewardZen(Ep4, 32, 200000)
	
-- 광장에서 살아남기-- 테스트 필요
	LuaQuestClass:SetQuestProgress(ASK1, Ep4, 34)--대화 진행상 완료창이 필요한 경우 사용

	LuaQuestClass:SetQuestEventMapClear(ASK1, Ep4, 35, EVENT_MAP_CLEAR_DEVIL, 2)
	LuaQuestClass:SetQuestRewardZen(Ep4, 35, 300000)
	LuaQuestClass:SetQuestRewardExp(Ep4, 35, 400000)
	
---------------------------------------------------------------------------------------------------------------------
-- 악마의광장 3지역
---------------------------------------------------------------------------------------------------------------------

-- 몬스터 잡기
	LuaQuestClass:SetQuestProgress(ASK1, Ep4, 38)--대화 진행상 완료창이 필요한 경우 사용
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep4, 39, 69, 80)--알카모스
	ItemType = 14 ItemIndex = 19 ItemLevel = 3 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep4, 39, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--데빌 스퀘어초대권 + 3

-- 광장에서 몬스터 잡기-- 테스트 필요
	LuaQuestClass:SetQuestProgress(ASK1, Ep4, 41)--대화 진행상 완료창이 필요한 경우 사용

	LuaQuestClass:SetQuestEventMapDevilPoint(ASK1, Ep4, 42, 3, 130)
	LuaQuestClass:SetQuestRewardZen(Ep4, 42, 500000)
	LuaQuestClass:SetQuestRewardExp(Ep4, 42, 800000)
	
-- 광장에서 살아남기-- 테스트 필요
	LuaQuestClass:SetQuestProgress(ASK1, Ep4, 44)--대화 진행상 완료창이 필요한 경우 사용

	LuaQuestClass:SetQuestEventMapClear(ASK1, Ep4, 45, EVENT_MAP_CLEAR_DEVIL, 3)
	LuaQuestClass:SetQuestRewardExp(Ep4, 45, 1000000)
	ItemType = 12 ItemIndex = 15 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep4, 45, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--혼석

---------------------------------------------------------------------------------------------------------------------
-- 악마의광장 4지역
---------------------------------------------------------------------------------------------------------------------

-- 몬스터 잡기
	LuaQuestClass:SetQuestProgress(ASK1, Ep4, 47)--대화 진행상 완료창이 필요한 경우 사용
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep4, 48, 57, 100, PartyPlay)--아이언 휠
	ItemType = 14 ItemIndex = 19 ItemLevel = 4 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep4, 48, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--데빌 스퀘어초대권 + 4

-- 광장에서 몬스터 잡기-- 테스트 필요
	LuaQuestClass:SetQuestProgress(ASK1, Ep4, 50)--대화 진행상 완료창이 필요한 경우 사용

	LuaQuestClass:SetQuestEventMapDevilPoint(ASK1, Ep4, 51, 4, 160)
	LuaQuestClass:SetQuestRewardZen(Ep4, 51, 500000)
	ItemType = 12 ItemIndex = 15 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep4, 51, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--혼석
-- 광장에서 살아남기-- 테스트 필요
	LuaQuestClass:SetQuestProgress(ASK1, Ep4, 53)--대화 진행상 완료창이 필요한 경우 사용

	LuaQuestClass:SetQuestEventMapClear(ASK1, Ep4, 54, EVENT_MAP_CLEAR_DEVIL, 4)
	ItemType = 14 ItemIndex = 14 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep4, 54, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--영석

---------------------------------------------------------------------------------------------------------------------
-- 악마의광장 5지역
---------------------------------------------------------------------------------------------------------------------

-- 몬스터 잡기
	LuaQuestClass:SetQuestProgress(ASK1, Ep4, 56)--대화 진행상 완료창이 필요한 경우 사용
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep4, 57, 61, 100, PartyPlay)--빔나이트
	ItemType = 14 ItemIndex = 19 ItemLevel = 5 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep4, 57, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--데빌 스퀘어초대권 + 5

-- 광장에서 몬스터 잡기-- 테스트 필요
	LuaQuestClass:SetQuestProgress(ASK1, Ep4, 59)--대화 진행상 완료창이 필요한 경우 사용

	LuaQuestClass:SetQuestEventMapDevilPoint(ASK1, Ep4, 60, 5, 190)
	ItemType = 14 ItemIndex = 14 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep4, 60, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--영석
	
-- 광장에서 살아남기-- 테스트 필요
	LuaQuestClass:SetQuestProgress(ASK1, Ep4, 62)--대화 진행상 완료창이 필요한 경우 사용

	LuaQuestClass:SetQuestEventMapClear(ASK1, Ep4, 63, EVENT_MAP_CLEAR_DEVIL, 5)
	ItemType = 14 ItemIndex = 13 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep4, 63, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--축석	

---------------------------------------------------------------------------------------------------------------------
-- 악마의광장 6지역
---------------------------------------------------------------------------------------------------------------------

-- 몬스터 잡기
	LuaQuestClass:SetQuestProgress(ASK1, Ep4, 65)--대화 진행상 완료창이 필요한 경우 사용
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep4, 66, 454, 120, PartyPlay)--아이스워커
	ItemType = 14 ItemIndex = 19 ItemLevel = 6 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep4, 66, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--데빌 스퀘어초대권 + 6

-- 광장에서 몬스터 잡기-- 테스트 필요
	LuaQuestClass:SetQuestProgress(ASK1, Ep4, 68)--대화 진행상 완료창이 필요한 경우 사용

	LuaQuestClass:SetQuestEventMapDevilPoint(ASK1, Ep4, 69, 6, 210)
	ItemType = 14 ItemIndex = 13 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep4, 69, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--축석
	
-- 광장에서 살아남기-- 테스트 필요
	LuaQuestClass:SetQuestProgress(ASK1, Ep4, 71)--대화 진행상 완료창이 필요한 경우 사용

	LuaQuestClass:SetQuestEventMapClear(ASK1, Ep4, 72, EVENT_MAP_CLEAR_DEVIL, 6)
	ItemType = 14 ItemIndex = 16 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep4, 72, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--생석
	
---------------------------------------------------------------------------------------------------------------------
-- 악마의광장 7지역
---------------------------------------------------------------------------------------------------------------------

-- 몬스터 잡기
	LuaQuestClass:SetQuestProgress(ASK1, Ep4, 74)--대화 진행상 완료창이 필요한 경우 사용
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep4, 75, 445, 140, PartyPlay)--쉐도우 나이트
	ItemType = 14 ItemIndex = 19 ItemLevel = 7 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep4, 75, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--데빌 스퀘어초대권 + 7

-- 광장에서 몬스터 잡기-- 테스트 필요
	LuaQuestClass:SetQuestProgress(ASK1, Ep4, 77)--대화 진행상 완료창이 필요한 경우 사용

	LuaQuestClass:SetQuestEventMapDevilPoint(ASK1, Ep4, 78, 7, 240)
	ItemType = 14 ItemIndex = 14 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 2
	LuaQuestClass:SetQuestRewardItem(Ep4, 78, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--영석
	
-- 광장에서 살아남기-- 테스트 필요
	LuaQuestClass:SetQuestProgress(ASK1, Ep4, 80)--대화 진행상 완료창이 필요한 경우 사용

	LuaQuestClass:SetQuestEventMapClear(ASK1, Ep4, 81, EVENT_MAP_CLEAR_DEVIL, 7)
	ItemType = 14 ItemIndex = 13 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 2
	LuaQuestClass:SetQuestRewardItem(Ep4, 81, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--축석	

	
end


	
	