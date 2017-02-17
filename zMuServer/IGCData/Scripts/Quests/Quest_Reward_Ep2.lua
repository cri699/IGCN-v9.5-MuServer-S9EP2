--[[
Quest_Reward_Window_Ep2.lua
[ Start  ] 	2009.08.
[ Latest ]	2009-12-09 16:45 PM
[ Version ]	0.0.0.1
[ Text ]									
[ Desc ]									
--]]

function Quest_Reward_Window_Ep2(iObjIndex)

	--local LuaQuestClass = QuestExpLuaBind()


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
-- 카오스 캐슬 1지역
---------------------------------------------------------------------------------------------------------------------

-- 설명 듣기
	LuaQuestClass:SetQuestProgress(ASK1, Ep2, 1)--대화 진행상 완료창이 필요한 경우 사용
	

	ItemType = 13 ItemIndex = 29 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep2, 8, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--근위병의 갑옷 셋트

-- 몬스터 잡기
	LuaQuestClass:SetQuestProgress(ASK1, Ep2, 10)--대화 진행상 완료창이 필요한 경우 사용
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep2, 11, 12, 40)--유충
	LuaQuestClass:SetQuestRewardRandom(Ep2, 11, 2, 1, 2000, 2, 2000, 3, 4000)
	ItemType = 14 ItemIndex = 2 ItemLevel = 0 dur = 3 ItemSkill = 0 ItemCnt = 10 Overlap = 1
	LuaQuestClass:SetQuestRewardItem(Ep2, 11, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt, Overlap)--중간 치료 물약 30개
	ItemType = 14 ItemIndex = 5 ItemLevel = 0 dur = 3 ItemSkill = 0 ItemCnt = 10 Overlap = 1
	LuaQuestClass:SetQuestRewardItem(Ep2, 11, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt, Overlap)--중간 마나 물약 30개
	ItemType = 14 ItemIndex = 35 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 4
	LuaQuestClass:SetQuestRewardItem(Ep2, 11, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--SD 물약 4개

-- 캐슬에서 몬스터 잡기-- 테스트 필요
	LuaQuestClass:SetQuestProgress(ASK1, Ep2, 13)--대화 진행상 완료창이 필요한 경우 사용

	LuaQuestClass:SetQuestEventMapKillPoint(ASK1, Ep2, 14, EVENT_MAP_MON_KILL, 1, 1)
	LuaQuestClass:SetQuestRewardZen(Ep2, 14, 100000)
	
-- 캐슬에서 살아남기-- 테스트 필요
	LuaQuestClass:SetQuestProgress(ASK1, Ep2, 16)--대화 진행상 완료창이 필요한 경우 사용

	LuaQuestClass:SetQuestEventMapClear(ASK1, Ep2, 17, EVENT_MAP_CLEAR_CHAOS, 1)
	LuaQuestClass:SetQuestRewardZen(Ep2, 17, 200000)
	LuaQuestClass:SetQuestRewardExp(Ep2, 17, 100000)

---------------------------------------------------------------------------------------------------------------------
-- 카오스 캐슬 2지역
---------------------------------------------------------------------------------------------------------------------

-- 설명 듣기
	LuaQuestClass:SetQuestProgress(ASK1, Ep2, 19)--대화 진행상 완료창이 필요한 경우 사용
	

	ItemType = 13 ItemIndex = 29 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep2, 26, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--근위병의 갑옷 셋트

-- 몬스터 잡기
	LuaQuestClass:SetQuestProgress(ASK1, Ep2, 28)--대화 진행상 완료창이 필요한 경우 사용
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep2, 29, 13, 60)--헬 스파이더
	LuaQuestClass:SetQuestRewardRandom(Ep2, 29, 2, 1, 2000, 2, 2000, 3, 4000)
	ItemType = 14 ItemIndex = 3 ItemLevel = 0 dur = 3 ItemSkill = 0 ItemCnt = 5 Overlap = 1
	LuaQuestClass:SetQuestRewardItem(Ep2, 29, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt, Overlap)--큰 치료 물약 15개
	ItemType = 14 ItemIndex = 6 ItemLevel = 0 dur = 3 ItemSkill = 0 ItemCnt = 5 Overlap = 1
	LuaQuestClass:SetQuestRewardItem(Ep2, 29, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt, Overlap)--큰 마나 물약 15개
	ItemType = 14 ItemIndex = 35 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 8
	LuaQuestClass:SetQuestRewardItem(Ep2, 29, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--SD 물약 8개

-- 캐슬에서 몬스터 잡기-- 테스트 필요
	LuaQuestClass:SetQuestProgress(ASK1, Ep2, 31)--대화 진행상 완료창이 필요한 경우 사용

	LuaQuestClass:SetQuestEventMapKillPoint(ASK1, Ep2, 32, EVENT_MAP_MON_KILL, 2, 3)
	LuaQuestClass:SetQuestRewardZen(Ep2, 32, 200000)
	
-- 캐슬에서 살아남기-- 테스트 필요
	LuaQuestClass:SetQuestProgress(ASK1, Ep2, 34)--대화 진행상 완료창이 필요한 경우 사용

	LuaQuestClass:SetQuestEventMapClear(ASK1, Ep2, 35, EVENT_MAP_CLEAR_CHAOS, 2)
	LuaQuestClass:SetQuestRewardZen(Ep2, 35, 300000)
	LuaQuestClass:SetQuestRewardExp(Ep2, 35, 400000)
	
---------------------------------------------------------------------------------------------------------------------
-- 카오스 캐슬 3지역
---------------------------------------------------------------------------------------------------------------------

-- 몬스터 잡기
	LuaQuestClass:SetQuestProgress(ASK1, Ep2, 38)--대화 진행상 완료창이 필요한 경우 사용
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep2, 39, 10, 80)--다크나이트
	LuaQuestClass:SetQuestRewardRandom(Ep2, 39, 2, 1, 2000, 2, 2000, 3, 4000)
	ItemType = 14 ItemIndex = 3 ItemLevel = 0 dur = 3 ItemSkill = 0 ItemCnt = 10 Overlap = 1
	LuaQuestClass:SetQuestRewardItem(Ep2, 39, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt, Overlap)--큰 치료 물약 30개
	ItemType = 14 ItemIndex = 6 ItemLevel = 0 dur = 3 ItemSkill = 0 ItemCnt = 10 Overlap = 1
	LuaQuestClass:SetQuestRewardItem(Ep2, 39, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt, Overlap)--큰 마나 물약 30개
	ItemType = 14 ItemIndex = 38 ItemLevel = 0 dur = 3 ItemSkill = 0 ItemCnt = 5 Overlap = 1
	LuaQuestClass:SetQuestRewardItem(Ep2, 39, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt, Overlap)--작은복합물약 15개

-- 캐슬에서 몬스터 잡기-- 테스트 필요
	LuaQuestClass:SetQuestProgress(ASK1, Ep2, 41)--대화 진행상 완료창이 필요한 경우 사용

	LuaQuestClass:SetQuestEventMapKillPoint(ASK1, Ep2, 42, EVENT_MAP_MON_KILL, 3, 5)
	LuaQuestClass:SetQuestRewardZen(Ep2, 42, 500000)
	LuaQuestClass:SetQuestRewardExp(Ep2, 42, 800000)
	
-- 캐슬에서 살아남기-- 테스트 필요
	LuaQuestClass:SetQuestProgress(ASK1, Ep2, 44)--대화 진행상 완료창이 필요한 경우 사용

	LuaQuestClass:SetQuestEventMapClear(ASK1, Ep2, 45, EVENT_MAP_CLEAR_CHAOS, 3)
	LuaQuestClass:SetQuestRewardExp(Ep2, 45, 1000000)
	ItemType = 12 ItemIndex = 15 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep2, 45, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--혼석

---------------------------------------------------------------------------------------------------------------------
-- 카오스 캐슬 4지역
---------------------------------------------------------------------------------------------------------------------

-- 몬스터 잡기
	LuaQuestClass:SetQuestProgress(ASK1, Ep2, 47)--대화 진행상 완료창이 필요한 경우 사용
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep2, 48, 37, 100)--데빌
	LuaQuestClass:SetQuestRewardRandom(Ep2, 48, 1, 1, 4000, 2, 4000)
	ItemType = 14 ItemIndex = 36 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 15
	LuaQuestClass:SetQuestRewardItem(Ep2, 48, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--SD 물약 15개
	ItemType = 14 ItemIndex = 39 ItemLevel = 0 dur = 3 ItemSkill = 0 ItemCnt = 5 Overlap = 1
	LuaQuestClass:SetQuestRewardItem(Ep2, 48, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt, Overlap)--작은복합물약 15개


-- 캐슬에서 몬스터 잡기-- 테스트 필요
	LuaQuestClass:SetQuestProgress(ASK1, Ep2, 50)--대화 진행상 완료창이 필요한 경우 사용

	LuaQuestClass:SetQuestEventMapKillPoint(ASK1, Ep2, 51, EVENT_MAP_USER_KILL, 4, 1)
	LuaQuestClass:SetQuestRewardZen(Ep2, 51, 300000)
	ItemType = 12 ItemIndex = 15 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep2, 51, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--혼석
-- 캐슬에서 살아남기-- 테스트 필요
	LuaQuestClass:SetQuestProgress(ASK1, Ep2, 53)--대화 진행상 완료창이 필요한 경우 사용

	LuaQuestClass:SetQuestEventMapClear(ASK1, Ep2, 54, EVENT_MAP_CLEAR_CHAOS, 4)
	ItemType = 14 ItemIndex = 14 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep2, 54, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--영석

---------------------------------------------------------------------------------------------------------------------
-- 카오스 캐슬 5지역
---------------------------------------------------------------------------------------------------------------------

-- 몬스터 잡기
	LuaQuestClass:SetQuestProgress(ASK1, Ep2, 56)--대화 진행상 완료창이 필요한 경우 사용
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep2, 57, 70, 100, PartyPlay)--퀸 레이너
	LuaQuestClass:SetQuestRewardRandom(Ep2, 57, 1, 1, 4000, 2, 4000)
	ItemType = 14 ItemIndex = 37 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 10
	LuaQuestClass:SetQuestRewardItem(Ep2, 57, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--큰 SD 물약 10개
	ItemType = 14 ItemIndex = 39 ItemLevel = 0 dur = 3 ItemSkill = 0 ItemCnt = 10 Overlap = 1
	LuaQuestClass:SetQuestRewardItem(Ep2, 57, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt, Overlap)--중간복합물약 30개

-- 캐슬에서 몬스터 잡기-- 테스트 필요
	LuaQuestClass:SetQuestProgress(ASK1, Ep2, 59)--대화 진행상 완료창이 필요한 경우 사용

	LuaQuestClass:SetQuestEventMapKillPoint(ASK1, Ep2, 60, EVENT_MAP_USER_KILL, 5, 3)
	ItemType = 14 ItemIndex = 14 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep2, 60, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--영석
	
-- 캐슬에서 살아남기-- 테스트 필요
	LuaQuestClass:SetQuestProgress(ASK1, Ep2, 62)--대화 진행상 완료창이 필요한 경우 사용

	LuaQuestClass:SetQuestEventMapClear(ASK1, Ep2, 63, EVENT_MAP_CLEAR_CHAOS, 5)
	ItemType = 14 ItemIndex = 13 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep2, 63, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--축석	

---------------------------------------------------------------------------------------------------------------------
-- 카오스 캐슬 6지역
---------------------------------------------------------------------------------------------------------------------

-- 몬스터 잡기
	LuaQuestClass:SetQuestProgress(ASK1, Ep2, 65)--대화 진행상 완료창이 필요한 경우 사용
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep2, 66, 59, 30, PartyPlay)--자이칸
	LuaQuestClass:SetQuestRewardRandom(Ep2, 66, 1, 1, 4000, 2, 4000)
	ItemType = 14 ItemIndex = 37 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 20
	LuaQuestClass:SetQuestRewardItem(Ep2, 66, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--큰 SD 물약 20개
	ItemType = 14 ItemIndex = 40 ItemLevel = 0 dur = 3 ItemSkill = 0 ItemCnt = 10 Overlap = 1
	LuaQuestClass:SetQuestRewardItem(Ep2, 66, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt, Overlap)--큰복합물약 30개

-- 캐슬에서 몬스터 잡기-- 테스트 필요
	LuaQuestClass:SetQuestProgress(ASK1, Ep2, 68)--대화 진행상 완료창이 필요한 경우 사용

	LuaQuestClass:SetQuestEventMapKillPoint(ASK1, Ep2, 69, EVENT_MAP_USER_KILL, 6, 5)
	ItemType = 14 ItemIndex = 13 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep2, 69, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--축석
	
-- 캐슬에서 살아남기-- 테스트 필요
	LuaQuestClass:SetQuestProgress(ASK1, Ep2, 71)--대화 진행상 완료창이 필요한 경우 사용

	LuaQuestClass:SetQuestEventMapClear(ASK1, Ep2, 72, EVENT_MAP_CLEAR_CHAOS, 6)
	ItemType = 14 ItemIndex = 16 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep2, 72, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--생석
	
---------------------------------------------------------------------------------------------------------------------
-- 카오스 캐슬 7지역
---------------------------------------------------------------------------------------------------------------------

-- 몬스터 잡기
	LuaQuestClass:SetQuestProgress(ASK1, Ep2, 74)--대화 진행상 완료창이 필요한 경우 사용
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep2, 75, 444, 100, PartyPlay)--쉐도우 폰
	LuaQuestClass:SetQuestRewardRandom(Ep2, 75, 1, 1, 4000, 2, 4000)
	ItemType = 14 ItemIndex = 37 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 30
	LuaQuestClass:SetQuestRewardItem(Ep2, 75, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--큰 SD 물약 30개
	ItemType = 14 ItemIndex = 40 ItemLevel = 0 dur = 3 ItemSkill = 0 ItemCnt = 15 Overlap = 1
	LuaQuestClass:SetQuestRewardItem(Ep2, 75, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt, Overlap)--큰복합물약 45개

-- 캐슬에서 몬스터 잡기-- 테스트 필요
	LuaQuestClass:SetQuestProgress(ASK1, Ep2, 77)--대화 진행상 완료창이 필요한 경우 사용

	LuaQuestClass:SetQuestEventMapKillPoint(ASK1, Ep2, 78, EVENT_MAP_USER_KILL, 7, 10)
	ItemType = 14 ItemIndex = 14 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 2
	LuaQuestClass:SetQuestRewardItem(Ep2, 78, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--영석
	
-- 캐슬에서 살아남기-- 테스트 필요
	LuaQuestClass:SetQuestProgress(ASK1, Ep2, 80)--대화 진행상 완료창이 필요한 경우 사용

	LuaQuestClass:SetQuestEventMapClear(ASK1, Ep2, 81, EVENT_MAP_CLEAR_CHAOS, 7)
	ItemType = 14 ItemIndex = 13 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 2
	LuaQuestClass:SetQuestRewardItem(Ep2, 81, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--축석	




	
end


	
	