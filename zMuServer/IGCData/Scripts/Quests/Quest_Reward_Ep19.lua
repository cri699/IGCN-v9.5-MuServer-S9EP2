--[[
Quest_Reward_Ep19.lua
[ Start  ] 	2009.08.
[ Latest ]	2010-02-17 11:15 AM
[ Version ]	0.0.0.2
[ Text ]									
[ Desc ]									
--]]

function Quest_Reward_Window_Ep19(iObjIndex)

	--local LuaQuestClass = QuestExpLuaBind()

	--********************************************--
	--Ask_Index 요구조건 DB에저장 bit연산 후 저장 한다.(여유 있게 하자...)
	-- DB: Name,EP,QS,Ask_index-Value-Complete Ask_index-Complete .....
	--0x00 0 00000000
	--g_LuaBind:SetQuestMonsterKill(EP,QS,Ask_Index ,monsterIndex,KillCnt)
	--g_LuaBind:SetQuestMonsterKill(1, 2, 0x1, 27, 3)--고리전갈3마리잡아라
	
	--아이템이 겹쳐지는가?
	--ItemType = 14 ItemIndex = 3 ItemLevel = 0 dur = 3 ItemSkill = 0 ItemCnt = 5 Overlap = 1
	--LuaQuestClass:SetQuestRewardItem(Ep1, 67, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt, Overlap)--큰 치료 물약 15개
	
	--엑셀런트 옵션 줄때 가변인자로 사용합니다.
	--ItemType = 4 ItemIndex = 8 ItemLevel = 3 dur = 0 ItemSkill = 1 ItemCnt = 1 Overlap = 0 ExOpt = ExOpt1 + ExOpt2 + ExOpt3 + ExOpt4 + ExOpt5 + ExOpt6
	--LuaQuestClass:SetQuestRewardItem(Ep1, 40, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt, Overlap, ExOpt)--석궁+3

	--옵션 추가 가변인자
	--ItemType = 4 ItemIndex = 8 ItemLevel = 3 dur = 0 ItemSkill = 1 ItemCnt = 1 Overlap = 0 Opt = Opt4 / Opt8 / Opt12 / Opt16
	--LuaQuestClass:SetQuestRewardItem(Ep1, 40, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt, Overlap, Opt)--옵션추가

	--********************************************--
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
-- 반복 퀘스트 (260~349레벨)
---------------------------------------------------------------------------------------------------------------------

-- 쿤둔의 감시자(260~289레벨)
	LuaQuestClass:SetQuestProgress(ASK1, Ep19, 1)--대화 진행상 완료창이 필요한 경우 사용
	ItemType = 14 ItemIndex = 28 ItemLevel = 3 ItemSkill = 0 ItemCnt = 3 Opt = 0 ExOpt = 0
	LuaQuestClass:SetQuestGetItem(ASK2, Ep19, 2, ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt)--지도구해오기
	
-- 아이언 휠 1차 보상 (260~289레벨)----------------------------------------------------------------------------------
	LuaQuestClass:SetQuestProgress(ASK1, Ep19, 4)--대화 진행상 완료창이 필요한 경우 사용

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 5, 57, 30)--아이언 휠

	LuaQuestClass:SetQuestRewardRandom(Ep19, 5, 1, 1, 9200, 2, 600, 3, 200)
	LuaQuestClass:SetQuestRewardExp(Ep19, 5, 70000)
	ItemType = 13 ItemIndex = 1 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1 
	LuaQuestClass:SetQuestRewardItem(Ep19, 5, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--사탄
	ItemType = 4 ItemIndex = 16 ItemLevel = 4 dur = 0 ItemSkill = 1 ItemCnt = 1 Overlap = 0 Opt = Opt12
	LuaQuestClass:SetQuestRewardItem(Ep19, 5, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt, Overlap, Opt)--세인트 크로스 석궁

-- 아이언 휠 2차 보상(260~289레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 6, 57, 30)--아이언 휠

	LuaQuestClass:SetQuestRewardRandom(Ep19, 6, 1, 1, 9200, 2, 600, 3, 200)
	LuaQuestClass:SetQuestRewardExp(Ep19, 6, 80000)
	ItemType = 13 ItemIndex = 0 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 2 
	LuaQuestClass:SetQuestRewardItem(Ep19, 6, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--수호천사
	ItemType = 2 ItemIndex = 10 ItemLevel = 4 dur = 0 ItemSkill = 1 ItemCnt = 1 Overlap = 0 Opt = Opt12
	LuaQuestClass:SetQuestRewardItem(Ep19, 6, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt, Overlap, Opt)--그레이트 셉터




-- 아이언 휠 3차 보상(260~289레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 7, 57, 30)--아이언 휠

	LuaQuestClass:SetQuestRewardRandom(Ep19, 7, 1, 1, 9200, 2, 600, 3, 200)
	LuaQuestClass:SetQuestRewardExp(Ep19, 7, 90000)
	LuaQuestClass:SetQuestRewardZen(Ep19, 7, 200000)
	ItemType = 14 ItemIndex = 19 ItemLevel = 4 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 7, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--데빌 스퀘어 입장권

	
-- 아이언 휠 4차 보상(260~289레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 8, 57, 30)--아이언 휠

	LuaQuestClass:SetQuestRewardRandom(Ep19, 8, 1, 1, 9200, 2, 600, 3, 200)
	LuaQuestClass:SetQuestRewardExp(Ep19, 8, 100000)
	ItemType = 13 ItemIndex = 3 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 8, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--디노란트
	ItemType = 13 ItemIndex = 18 ItemLevel = 6 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 8, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--투명망토


-- 아이언 휠 5차 보상(260~289레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 9, 57, 30)--아이언 휠

	LuaQuestClass:SetQuestRewardRandom(Ep19, 9, 1, 1, 9200, 2, 600, 3, 200)
	LuaQuestClass:SetQuestRewardExp(Ep19, 9, 110000)
	ItemType = 14 ItemIndex = 43 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 9, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--하급제련석
	ItemType = 14 ItemIndex = 14 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 9, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--영혼의보석

	
-- 아이언 휠 6차 보상(260~289레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 10, 57, 30)--아이언 휠

	LuaQuestClass:SetQuestRewardRandom(Ep19, 10, 1, 1, 9200, 2, 600, 3, 200)
	LuaQuestClass:SetQuestRewardExp(Ep19, 10, 120000)
	ItemType = 14 ItemIndex = 14 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 10, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--영혼의보석
	ItemType = 14 ItemIndex = 13 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 10, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--축복의보석

-- 아이언 휠 7차 보상(260~289레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 11, 57, 30)--아이언 휠

	LuaQuestClass:SetQuestRewardRandom(Ep19, 11, 1, 1, 9200, 2, 600, 3, 200)
	LuaQuestClass:SetQuestRewardExp(Ep19, 11, 60000)
	ItemType = 13 ItemIndex = 1 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 11, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--사탄
	ItemType = 5 ItemIndex = 17 ItemLevel = 4 dur = 0 ItemSkill = 0 ItemCnt = 1 Overlap = 0 Opt = Opt12
	LuaQuestClass:SetQuestRewardItem(Ep19, 11, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt, Overlap, Opt)--에이션트 스틱

-- 아이언 휠 8차 보상(260~289레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 12, 57, 30)--아이언 휠

	LuaQuestClass:SetQuestRewardRandom(Ep19, 12, 1, 1, 9200, 2, 600, 3, 200)
	LuaQuestClass:SetQuestRewardExp(Ep19, 12, 50000)
	ItemType = 14 ItemIndex = 14 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 12, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--영혼의보석
	ItemType = 14 ItemIndex = 16 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 12, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--생명의보석



-- 탄탈로스&뮤턴트 1차 보상 (260~289레벨)----------------------------------------------------------------------------------

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 13, 62, 20)--뮤턴트
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep19, 13, 58, 20)--탄탈로스
	
	LuaQuestClass:SetQuestRewardRandom(Ep19, 13, 1, 1, 9200, 2, 600, 3, 200)
	LuaQuestClass:SetQuestRewardExp(Ep19, 13, 50000)
	ItemType = 13 ItemIndex = 1 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1 
	LuaQuestClass:SetQuestRewardItem(Ep19, 13, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--사탄
	ItemType = 14 ItemIndex = 19 ItemLevel = 5 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 13, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--데빌 스퀘어 입장권

-- 탄탈로스&뮤턴트 2차 보상(260~289레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 14, 62, 20)--뮤턴트
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep19, 14, 58, 20)--탄탈로스

	LuaQuestClass:SetQuestRewardRandom(Ep19, 14, 1, 1, 9200, 2, 600, 3, 200)
	LuaQuestClass:SetQuestRewardExp(Ep19, 14, 60000)
	ItemType = 13 ItemIndex = 0 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 2 
	LuaQuestClass:SetQuestRewardItem(Ep19, 14, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--수호천사
	ItemType = 14 ItemIndex = 16 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 14, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--생명의보석




-- 탄탈로스&뮤턴트 3차 보상(260~289레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 15, 62, 20)--뮤턴트
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep19, 15, 58, 20)--탄탈로스

	LuaQuestClass:SetQuestRewardRandom(Ep19, 15, 1, 1, 9200, 2, 600, 3, 200)
	LuaQuestClass:SetQuestRewardExp(Ep19, 15, 70000)
	ItemType = 14 ItemIndex = 43 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 15, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--하급제련석
	ItemType = 14 ItemIndex = 19 ItemLevel = 4 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 15, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--데빌 스퀘어 입장권

	
-- 탄탈로스&뮤턴트 4차 보상(260~289레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 16, 62, 20)--뮤턴트
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep19, 16, 58, 20)--탄탈로스

	LuaQuestClass:SetQuestRewardRandom(Ep19, 16, 1, 1, 9200, 2, 600, 3, 200)
	LuaQuestClass:SetQuestRewardExp(Ep19, 16, 80000)
	ItemType = 14 ItemIndex = 14 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 16, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--영혼의보석
	ItemType = 13 ItemIndex = 18 ItemLevel = 6 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 16, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--투명망토


-- 탄탈로스&뮤턴트 5차 보상(260~289레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 17, 62, 20)--뮤턴트
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep19, 17, 58, 20)--탄탈로스

	LuaQuestClass:SetQuestRewardRandom(Ep19, 17, 1, 1, 9200, 2, 600, 3, 200)
	LuaQuestClass:SetQuestRewardExp(Ep19, 17, 90000)
	ItemType = 14 ItemIndex = 22 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 17, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--창조의보석
	ItemType = 14 ItemIndex = 13 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 17, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--축복의보석

	
-- 탄탈로스&뮤턴트 6차 보상(260~289레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 18, 62, 20)--뮤턴트
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep19, 18, 58, 20)--탄탈로스

	LuaQuestClass:SetQuestRewardRandom(Ep19, 18, 1, 1, 9200, 2, 600, 3, 200)
	LuaQuestClass:SetQuestRewardExp(Ep19, 18, 100000)
	ItemType = 14 ItemIndex = 14 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 18, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--영혼의보석
	ItemType = 13 ItemIndex = 18 ItemLevel = 7 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 18, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--투명망토

-- 탄탈로스&뮤턴트 7차 보상(260~289레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 19, 62, 20)--뮤턴트
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep19, 19, 58, 20)--탄탈로스

	LuaQuestClass:SetQuestRewardRandom(Ep19, 19, 1, 1, 9200, 2, 600, 3, 200)
	LuaQuestClass:SetQuestRewardExp(Ep19, 19, 110000)
	ItemType = 14 ItemIndex = 14 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 19, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--영혼의보석
	ItemType = 14 ItemIndex = 16 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 19, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--생명의보석

-- 탄탈로스&뮤턴트 8차 보상(260~289레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 20, 62, 20)--뮤턴트
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep19, 20, 58, 20)--탄탈로스

	LuaQuestClass:SetQuestRewardRandom(Ep19, 20, 1, 1, 9200, 2, 600, 3, 200)
	LuaQuestClass:SetQuestRewardExp(Ep19, 20, 120000)
	ItemType = 14 ItemIndex = 14 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 20, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--영혼의보석
	ItemType = 12 ItemIndex = 15 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 20, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--혼돈의보석



-- 빔나이트&뮤턴트 1차 보상 (260~289레벨)----------------------------------------------------------------------------------

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 21, 62, 20)--뮤턴트
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep19, 21, 61, 10)--빔나이트
	
	LuaQuestClass:SetQuestRewardRandom(Ep19, 21, 1, 1, 9200, 2, 600, 3, 200)
	LuaQuestClass:SetQuestRewardExp(Ep19, 21, 90000)
	ItemType = 13 ItemIndex = 0 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 3 
	LuaQuestClass:SetQuestRewardItem(Ep19, 21, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--수호천사
	ItemType = 14 ItemIndex = 19 ItemLevel = 5 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 21, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--데빌 스퀘어 입장권

-- 빔나이트&뮤턴트 2차 보상(260~289레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 22, 62, 20)--뮤턴트
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep19, 22, 61, 10)--빔나이트

	LuaQuestClass:SetQuestRewardRandom(Ep19, 22, 1, 1, 9200, 2, 600, 3, 200)
	LuaQuestClass:SetQuestRewardExp(Ep19, 22, 100000)
	ItemType = 13 ItemIndex = 1 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 2 
	LuaQuestClass:SetQuestRewardItem(Ep19, 22, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--사탄
	ItemType = 14 ItemIndex = 16 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 22, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--생명의보석




-- 빔나이트&뮤턴트 3차 보상(260~289레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 23, 62, 20)--뮤턴트
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep19, 23, 61, 10)--빔나이트

	LuaQuestClass:SetQuestRewardRandom(Ep19, 23, 1, 1, 9200, 2, 600, 3, 200)
	LuaQuestClass:SetQuestRewardExp(Ep19, 23, 110000)
	ItemType = 14 ItemIndex = 14 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 23, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--영혼의보석
	ItemType = 14 ItemIndex = 19 ItemLevel = 4 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 23, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--데빌 스퀘어 입장권

	
-- 빔나이트&뮤턴트 4차 보상(260~289레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 24, 62, 20)--뮤턴트
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep19, 24, 61, 10)--빔나이트

	LuaQuestClass:SetQuestRewardRandom(Ep19, 24, 1, 1, 9200, 2, 600, 3, 200)
	LuaQuestClass:SetQuestRewardExp(Ep19, 24, 120000)
	ItemType = 14 ItemIndex = 14 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 24, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--영혼의보석
	ItemType = 13 ItemIndex = 18 ItemLevel = 6 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 24, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--투명망토


-- 빔나이트&뮤턴트 5차 보상(260~289레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 25, 62, 20)--뮤턴트
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep19, 25, 61, 10)--빔나이트

	LuaQuestClass:SetQuestRewardRandom(Ep19, 25, 1, 1, 9200, 2, 600, 3, 200)
	LuaQuestClass:SetQuestRewardExp(Ep19, 25, 50000)
	ItemType = 14 ItemIndex = 22 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 25, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--창조의보석
	ItemType = 12 ItemIndex = 31 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 25, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--영혼의보석*10

	
-- 빔나이트&뮤턴트 6차 보상(260~289레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 26, 62, 20)--뮤턴트
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep19, 26, 61, 10)--빔나이트

	LuaQuestClass:SetQuestRewardRandom(Ep19, 26, 1, 1, 9200, 2, 600, 3, 200)
	LuaQuestClass:SetQuestRewardExp(Ep19, 26, 60000)
	ItemType = 14 ItemIndex = 14 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 26, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--영혼의보석
	ItemType = 13 ItemIndex = 18 ItemLevel = 7 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 26, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--투명망토

-- 빔나이트&뮤턴트 7차 보상(260~289레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 27, 62, 20)--뮤턴트
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep19, 27, 61, 10)--빔나이트

	LuaQuestClass:SetQuestRewardRandom(Ep19, 27, 1, 1, 9200, 2, 600, 3, 200)
	LuaQuestClass:SetQuestRewardExp(Ep19, 27, 70000)
	ItemType = 14 ItemIndex = 14 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 27, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--영혼의보석
	ItemType = 5 ItemIndex = 9 ItemLevel = 4 dur = 0 ItemSkill = 0 ItemCnt = 1 Overlap = 0 Opt = Opt12
	LuaQuestClass:SetQuestRewardItem(Ep19, 27, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt, Overlap, Opt)--드레곤소울지팡이

-- 빔나이트&뮤턴트 8차 보상(260~289레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 28, 62, 20)--뮤턴트
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep19, 28, 61, 10)--빔나이트

	LuaQuestClass:SetQuestRewardRandom(Ep19, 28, 1, 1, 9200, 2, 600, 3, 200)
	LuaQuestClass:SetQuestRewardExp(Ep19, 28, 80000)
	ItemType = 14 ItemIndex = 22 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 28, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--창조의보석
	ItemType = 12 ItemIndex = 15 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 28, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--혼돈의보석



-- 아이템수집&뮤턴트 1차 보상 (260~289레벨)----------------------------------------------------------------------------------

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 29, 62, 20)--뮤턴트
	ItemType = 5 ItemIndex = 6 ItemLevel = 0 ItemSkill = 0 ItemCnt = 1 Opt = 0 ExOpt = 0
	LuaQuestClass:SetQuestGetItem(ASK2, Ep19, 29, ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt)--부활의 지팡이
	
	LuaQuestClass:SetQuestRewardRandom(Ep19, 29, 1, 1, 9200, 2, 600, 3, 200)
	LuaQuestClass:SetQuestRewardExp(Ep19, 29, 120000)
	LuaQuestClass:SetQuestRewardZen(Ep19, 29, 100000)
	ItemType = 14 ItemIndex = 43 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 29, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--하급제련석
	
-- 아이템수집&뮤턴트 2차 보상(260~289레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 30, 62, 20)--뮤턴트
	ItemType = 5 ItemIndex = 6 ItemLevel = 0 ItemSkill = 0 ItemCnt = 1 Opt = 0 ExOpt = 0
	LuaQuestClass:SetQuestGetItem(ASK2, Ep19, 30, ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt)--부활의 지팡이

	LuaQuestClass:SetQuestRewardRandom(Ep19, 30, 1, 1, 9200, 2, 600, 3, 200)
	LuaQuestClass:SetQuestRewardExp(Ep19, 30, 110000)
	LuaQuestClass:SetQuestRewardZen(Ep19, 30, 200000)
	ItemType = 13 ItemIndex = 0 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 2 
	LuaQuestClass:SetQuestRewardItem(Ep19, 30, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--수호천사


-- 아이템수집&뮤턴트 3차 보상(260~289레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 31, 62, 20)--뮤턴트
	ItemType = 5 ItemIndex = 6 ItemLevel = 0 ItemSkill = 0 ItemCnt = 1 Opt = 0 ExOpt = 0
	LuaQuestClass:SetQuestGetItem(ASK2, Ep19, 31, ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt)--부활의 지팡이

	LuaQuestClass:SetQuestRewardRandom(Ep19, 31, 1, 1, 9200, 2, 600, 3, 200)
	LuaQuestClass:SetQuestRewardExp(Ep19, 31, 100000)
	LuaQuestClass:SetQuestRewardZen(Ep19, 31, 200000)
	ItemType = 13 ItemIndex = 1 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 31, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--사탄

	
-- 아이템수집&뮤턴트 4차 보상(260~289레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 32, 62, 20)--뮤턴트
	ItemType = 5 ItemIndex = 6 ItemLevel = 0 ItemSkill = 0 ItemCnt = 1 Opt = 0 ExOpt = 0
	LuaQuestClass:SetQuestGetItem(ASK2, Ep19, 32, ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt)--부활의 지팡이

	LuaQuestClass:SetQuestRewardRandom(Ep19, 32, 1, 1, 9200, 2, 600, 3, 200)
	LuaQuestClass:SetQuestRewardExp(Ep19, 32, 90000)
	ItemType = 14 ItemIndex = 14 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 32, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--영혼의보석
	ItemType = 12 ItemIndex = 15 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 32, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--혼돈의보석


-- 아이템수집&뮤턴트 5차 보상(260~289레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 33, 62, 20)--뮤턴트
	ItemType = 5 ItemIndex = 6 ItemLevel = 0 ItemSkill = 0 ItemCnt = 1 Opt = 0 ExOpt = 0
	LuaQuestClass:SetQuestGetItem(ASK2, Ep19, 33, ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt)--부활의 지팡이

	LuaQuestClass:SetQuestRewardRandom(Ep19, 33, 1, 1, 9200, 2, 600, 3, 200)
	LuaQuestClass:SetQuestRewardExp(Ep19, 33, 80000)
	ItemType = 14 ItemIndex = 14 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 33, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--영혼의보석
	ItemType = 14 ItemIndex = 13 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 33, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--축복의보석

	
-- 아이템수집&뮤턴트 6차 보상(260~289레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 34, 62, 20)--뮤턴트
	ItemType = 5 ItemIndex = 6 ItemLevel = 0 ItemSkill = 0 ItemCnt = 1 Opt = 0 ExOpt = 0
	LuaQuestClass:SetQuestGetItem(ASK2, Ep19, 34, ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt)--부활의 지팡이

	LuaQuestClass:SetQuestRewardRandom(Ep19, 34, 1, 1, 9200, 2, 600, 3, 200)
	LuaQuestClass:SetQuestRewardExp(Ep19, 34, 70000)
	ItemType = 14 ItemIndex = 43 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 34, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--하급제련석
	ItemType = 13 ItemIndex = 18 ItemLevel = 7 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 34, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--투명망토

-- 아이템수집&뮤턴트 7차 보상(260~289레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 35, 62, 20)--뮤턴트
	ItemType = 5 ItemIndex = 6 ItemLevel = 0 ItemSkill = 0 ItemCnt = 1 Opt = 0 ExOpt = 0
	LuaQuestClass:SetQuestGetItem(ASK2, Ep19, 35, ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt)--부활의 지팡이

	LuaQuestClass:SetQuestRewardRandom(Ep19, 35, 1, 1, 9200, 2, 600, 3, 200)
	LuaQuestClass:SetQuestRewardExp(Ep19, 35, 60000)
	ItemType = 3 ItemIndex = 10 ItemLevel = 4 dur = 0 ItemSkill = 1 ItemCnt = 1 Overlap = 0 Opt = Opt12
	LuaQuestClass:SetQuestRewardItem(Ep19, 35, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt, Overlap, Opt)--드레곤스피어
	ItemType = 12 ItemIndex = 15 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 35, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--혼돈의보석


-- 아이템수집&뮤턴트 8차 보상(260~289레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 36, 62, 20)--뮤턴트
	ItemType = 5 ItemIndex = 6 ItemLevel = 0 ItemSkill = 0 ItemCnt = 1 Opt = 0 ExOpt = 0
	LuaQuestClass:SetQuestGetItem(ASK2, Ep19, 36, ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt)--부활의 지팡이

	LuaQuestClass:SetQuestRewardRandom(Ep19, 36, 1, 1, 9200, 2, 600, 3, 200)
	LuaQuestClass:SetQuestRewardExp(Ep19, 36, 50000)
	ItemType = 14 ItemIndex = 13 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 36, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--축복의보석
	ItemType = 12 ItemIndex = 15 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 36, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--혼돈의보석

	
-- 아이템수집&뮤턴트 1차 보상 (260~289레벨)----------------------------------------------------------------------------------

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 37, 62, 20)--뮤턴트
	ItemType = 13 ItemIndex = 2 ItemLevel = 0 ItemSkill = 0 ItemCnt = 5 Opt = 0 ExOpt = 0
	LuaQuestClass:SetQuestGetItem(ASK2, Ep19, 37, ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt)--유니리아
	
	LuaQuestClass:SetQuestRewardRandom(Ep19, 37, 1, 1, 9200, 2, 600, 3, 200)
	LuaQuestClass:SetQuestRewardExp(Ep19, 37, 110000)
	LuaQuestClass:SetQuestRewardZen(Ep19, 37, 100000)
	ItemType = 14 ItemIndex = 19 ItemLevel = 4 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 37, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--데빌스퀘어초대권
	
-- 아이템수집&뮤턴트 2차 보상(260~289레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 38, 62, 20)--뮤턴트
	ItemType = 13 ItemIndex = 2 ItemLevel = 0 ItemSkill = 0 ItemCnt = 5 Opt = 0 ExOpt = 0
	LuaQuestClass:SetQuestGetItem(ASK2, Ep19, 38, ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt)--유니리아

	LuaQuestClass:SetQuestRewardRandom(Ep19, 38, 1, 1, 9200, 2, 600, 3, 200)
	LuaQuestClass:SetQuestRewardExp(Ep19, 38, 120000)
	LuaQuestClass:SetQuestRewardZen(Ep19, 38, 200000)
	ItemType = 13 ItemIndex = 0 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 2 
	LuaQuestClass:SetQuestRewardItem(Ep19, 38, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--수호천사


-- 아이템수집&뮤턴트 3차 보상(260~289레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 39, 62, 20)--뮤턴트
	ItemType = 13 ItemIndex = 2 ItemLevel = 0 ItemSkill = 0 ItemCnt = 5 Opt = 0 ExOpt = 0
	LuaQuestClass:SetQuestGetItem(ASK2, Ep19, 39, ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt)--유니리아

	LuaQuestClass:SetQuestRewardRandom(Ep19, 39, 1, 1, 9200, 2, 600, 3, 200)
	LuaQuestClass:SetQuestRewardExp(Ep19, 39, 50000)
	LuaQuestClass:SetQuestRewardZen(Ep19, 39, 200000)
	ItemType = 13 ItemIndex = 1 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 39, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--사탄

	
-- 아이템수집&뮤턴트 4차 보상(260~289레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 40, 62, 20)--뮤턴트
	ItemType = 13 ItemIndex = 2 ItemLevel = 0 ItemSkill = 0 ItemCnt = 5 Opt = 0 ExOpt = 0
	LuaQuestClass:SetQuestGetItem(ASK2, Ep19, 40, ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt)--유니리아

	LuaQuestClass:SetQuestRewardRandom(Ep19, 40, 1, 1, 9200, 2, 600, 3, 200)
	LuaQuestClass:SetQuestRewardExp(Ep19, 40, 60000)
	ItemType = 13 ItemIndex = 3 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 40, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--디노란트
	ItemType = 12 ItemIndex = 15 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 40, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--혼돈의보석


-- 아이템수집&뮤턴트 5차 보상(260~289레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 41, 62, 20)--뮤턴트
	ItemType = 13 ItemIndex = 2 ItemLevel = 0 ItemSkill = 0 ItemCnt = 5 Opt = 0 ExOpt = 0
	LuaQuestClass:SetQuestGetItem(ASK2, Ep19, 41, ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt)--유니리아

	LuaQuestClass:SetQuestRewardRandom(Ep19, 41, 1, 1, 9200, 2, 600, 3, 200)
	LuaQuestClass:SetQuestRewardExp(Ep19, 41, 70000)
	ItemType = 14 ItemIndex = 14 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 41, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--영혼의보석
	ItemType = 14 ItemIndex = 13 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 41, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--축복의보석

	
-- 아이템수집&뮤턴트 6차 보상(260~289레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 42, 62, 20)--뮤턴트
	ItemType = 13 ItemIndex = 2 ItemLevel = 0 ItemSkill = 0 ItemCnt = 5 Opt = 0 ExOpt = 0
	LuaQuestClass:SetQuestGetItem(ASK2, Ep19, 42, ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt)--유니리아

	LuaQuestClass:SetQuestRewardRandom(Ep19, 42, 1, 1, 9200, 2, 600, 3, 200)
	LuaQuestClass:SetQuestRewardExp(Ep19, 42, 80000)
	ItemType = 14 ItemIndex = 43 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 42, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--하급제련석
	ItemType = 13 ItemIndex = 18 ItemLevel = 7 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 42, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--투명망토

-- 아이템수집&뮤턴트 7차 보상(260~289레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 43, 62, 20)--뮤턴트
	ItemType = 13 ItemIndex = 2 ItemLevel = 0 ItemSkill = 0 ItemCnt = 5 Opt = 0 ExOpt = 0
	LuaQuestClass:SetQuestGetItem(ASK2, Ep19, 43, ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt)--유니리아

	LuaQuestClass:SetQuestRewardRandom(Ep19, 43, 1, 1, 9200, 2, 600, 3, 200)
	LuaQuestClass:SetQuestRewardExp(Ep19, 43, 90000)
	ItemType = 12 ItemIndex = 15 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 43, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--혼돈의보석
	ItemType = 2 ItemIndex = 6 ItemLevel = 4 dur = 0 ItemSkill = 0 ItemCnt = 0 Overlap = 0 Opt = Opt12
	LuaQuestClass:SetQuestRewardItem(Ep19, 43, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt, Overlap, Opt)--카오스도끼

-- 아이템수집&뮤턴트 8차 보상(260~289레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 44, 62, 20)--뮤턴트
	ItemType = 13 ItemIndex = 2 ItemLevel = 0 ItemSkill = 0 ItemCnt = 5 Opt = 0 ExOpt = 0
	LuaQuestClass:SetQuestGetItem(ASK2, Ep19, 44, ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt)--유니리아

	LuaQuestClass:SetQuestRewardRandom(Ep19, 44, 1, 1, 9200, 2, 600, 3, 200)
	LuaQuestClass:SetQuestRewardExp(Ep19, 44, 100000)
	ItemType = 14 ItemIndex = 22 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 44, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--창조의보석
	ItemType = 12 ItemIndex = 15 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 44, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--혼돈의보석
		
---------------------------------------------------------------------------------------------------------------------	

---------------------------------------------------------------------------------------------------------------------
-- 반복 퀘스트 (290~319레벨)
---------------------------------------------------------------------------------------------------------------------

-- 쿤둔의 감시자(290~319레벨)
	LuaQuestClass:SetQuestProgress(ASK1, Ep19, 46)--대화 진행상 완료창이 필요한 경우 사용
	ItemType = 14 ItemIndex = 28 ItemLevel = 4 ItemSkill = 0 ItemCnt = 2 Opt = 0 ExOpt = 0
	LuaQuestClass:SetQuestGetItem(ASK2, Ep19, 47, ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt)--지도구해오기
	
-- 퀸레이너 1차 보상 (290~319레벨)----------------------------------------------------------------------------------
	LuaQuestClass:SetQuestProgress(ASK1, Ep19, 49)--대화 진행상 완료창이 필요한 경우 사용

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 50, 70, 30)--퀸레이너

	LuaQuestClass:SetQuestRewardRandom(Ep19, 50, 1, 1, 9000, 2, 700, 3, 300)
	LuaQuestClass:SetQuestRewardExp(Ep19, 50, 70000)
	LuaQuestClass:SetQuestRewardZen(Ep19, 50, 200000)
	ItemType = 13 ItemIndex = 1 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1 
	LuaQuestClass:SetQuestRewardItem(Ep19, 50, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--사탄

-- 퀸레이너 2차 보상(290~319레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 51, 70, 30)--퀸레이너

	LuaQuestClass:SetQuestRewardRandom(Ep19, 51, 1, 1, 9000, 2, 700, 3, 300)
	LuaQuestClass:SetQuestRewardExp(Ep19, 51, 80000)
	LuaQuestClass:SetQuestRewardZen(Ep19, 51, 100000)
	ItemType = 13 ItemIndex = 18 ItemLevel = 6 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 51, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--투명망토




-- 퀸레이너 3차 보상(290~319레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 52, 70, 30)--퀸레이너

	LuaQuestClass:SetQuestRewardRandom(Ep19, 52, 1, 1, 9000, 2, 700, 3, 300)
	LuaQuestClass:SetQuestRewardExp(Ep19, 52, 90000)
	ItemType = 13 ItemIndex = 0 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 2 
	LuaQuestClass:SetQuestRewardItem(Ep19, 52, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--수호천사
	ItemType = 14 ItemIndex = 16 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 52, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--생명의 보석

	
-- 퀸레이너 4차 보상(290~319레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 53, 70, 30)--퀸레이너

	LuaQuestClass:SetQuestRewardRandom(Ep19, 53, 1, 1, 9000, 2, 700, 3, 300)
	LuaQuestClass:SetQuestRewardExp(Ep19, 53, 100000)
	ItemType = 14 ItemIndex = 43 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 53, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--하급제련석
	ItemType = 12 ItemIndex = 15 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 53, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--혼돈의보석


-- 퀸레이너 5차 보상(290~319레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 54, 70, 30)--퀸레이너

	LuaQuestClass:SetQuestRewardRandom(Ep19, 54, 1, 1, 9000, 2, 700, 3, 300)
	LuaQuestClass:SetQuestRewardExp(Ep19, 54, 110000)
	ItemType = 13 ItemIndex = 1 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 54, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--사탄
	ItemType = 14 ItemIndex = 13 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 54, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--축복의보석

	
-- 퀸레이너 6차 보상(290~319레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 55, 70, 30)--퀸레이너

	LuaQuestClass:SetQuestRewardRandom(Ep19, 55, 1, 1, 9000, 2, 700, 3, 300)
	LuaQuestClass:SetQuestRewardExp(Ep19, 55, 120000)
	ItemType = 14 ItemIndex = 14 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 55, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--영혼의보석
	ItemType = 14 ItemIndex = 16 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 55, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--생명의보석

-- 퀸레이너 7차 보상(290~319레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 56, 70, 30)--퀸레이너

	LuaQuestClass:SetQuestRewardRandom(Ep19, 56, 1, 1, 9000, 2, 700, 3, 300)
	LuaQuestClass:SetQuestRewardExp(Ep19, 56, 130000)
	ItemType = 14 ItemIndex = 14 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 56, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--영혼의보석
	ItemType = 3 ItemIndex = 10 ItemLevel = 6 dur = 0 ItemSkill = 1 ItemCnt = 1 Overlap = 0 Opt = Opt12
	LuaQuestClass:SetQuestRewardItem(Ep19, 56, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt, Overlap, Opt)--드라곤스피어

-- 퀸레이너 8차 보상(290~319레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 57, 70, 30)--퀸레이너

	LuaQuestClass:SetQuestRewardRandom(Ep19, 57, 1, 1, 9000, 2, 700, 3, 300)
	LuaQuestClass:SetQuestRewardExp(Ep19, 57, 140000)
	ItemType = 12 ItemIndex = 15 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 57, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--혼돈의보석
	ItemType = 14 ItemIndex = 13 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 57, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--축복의보석



-- 드라칸&알카모스 1차 보상 (290~319레벨)----------------------------------------------------------------------------------

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 58, 69, 20)--알카모스
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep19, 58, 73, 20)--드라칸
	
	LuaQuestClass:SetQuestRewardRandom(Ep19, 58, 1, 1, 9000, 2, 700, 3, 300)
	LuaQuestClass:SetQuestRewardExp(Ep19, 58, 110000)
	LuaQuestClass:SetQuestRewardZen(Ep19, 58, 200000)
	ItemType = 14 ItemIndex = 43 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 58, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--하급 제련석
	
-- 드라칸&알카모스 2차 보상(290~319레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 59, 69, 20)--알카모스
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep19, 59, 73, 20)--드라칸

	LuaQuestClass:SetQuestRewardRandom(Ep19, 59, 1, 1, 9000, 2, 700, 3, 300)
	LuaQuestClass:SetQuestRewardExp(Ep19, 59, 120000)
	ItemType = 13 ItemIndex = 0 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 2 
	LuaQuestClass:SetQuestRewardItem(Ep19, 59, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--수호천사
	ItemType = 2 ItemIndex = 10 ItemLevel = 6 dur = 0 ItemSkill = 1 ItemCnt = 1 Overlap = 0 Opt = Opt12
	LuaQuestClass:SetQuestRewardItem(Ep19, 59, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt, Overlap, Opt)--그레이트셉터




-- 드라칸&알카모스 3차 보상(290~319레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 60, 69, 20)--알카모스
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep19, 60, 73, 20)--드라칸

	LuaQuestClass:SetQuestRewardRandom(Ep19, 60, 1, 1, 9000, 2, 700, 3, 300)
	LuaQuestClass:SetQuestRewardExp(Ep19, 60, 130000)
	ItemType = 13 ItemIndex = 1 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 60, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--사탄
	ItemType = 13 ItemIndex = 18 ItemLevel = 7 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 60, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--투명망토

	
-- 드라칸&알카모스 4차 보상(290~319레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 61, 69, 20)--알카모스
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep19, 61, 73, 20)--드라칸

	LuaQuestClass:SetQuestRewardRandom(Ep19, 61, 1, 1, 9000, 2, 700, 3, 300)
	LuaQuestClass:SetQuestRewardExp(Ep19, 61, 140000)
	ItemType = 14 ItemIndex = 14 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 61, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--영혼의보석
	ItemType = 14 ItemIndex = 13 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 61, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--축복의보석


-- 드라칸&알카모스 5차 보상(290~319레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 62, 69, 20)--알카모스
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep19, 62, 73, 20)--드라칸

	LuaQuestClass:SetQuestRewardRandom(Ep19, 62, 1, 1, 9000, 2, 700, 3, 300)
	LuaQuestClass:SetQuestRewardExp(Ep19, 62, 70000)
	ItemType = 14 ItemIndex = 13 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 62, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--축복의보석
	ItemType = 12 ItemIndex = 15 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 62, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--혼돈의보석

	
-- 드라칸&알카모스 6차 보상(290~319레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 63, 69, 20)--알카모스
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep19, 63, 73, 20)--드라칸

	LuaQuestClass:SetQuestRewardRandom(Ep19, 63, 1, 1, 9000, 2, 700, 3, 300)
	LuaQuestClass:SetQuestRewardExp(Ep19, 63, 80000)
	ItemType = 12 ItemIndex = 15 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 63, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--혼돈의보석
	ItemType = 14 ItemIndex = 16 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 63, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--생명의보석

-- 드라칸&알카모스 7차 보상(290~319레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 64, 69, 20)--알카모스
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep19, 64, 73, 20)--드라칸

	LuaQuestClass:SetQuestRewardRandom(Ep19, 64, 1, 1, 9000, 2, 700, 3, 300)
	LuaQuestClass:SetQuestRewardExp(Ep19, 64, 90000)
	ItemType = 14 ItemIndex = 14 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 64, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--영혼의보석
	ItemType = 14 ItemIndex = 13 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 64, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--축복의보석

-- 드라칸&알카모스 8차 보상(290~319레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 65, 69, 20)--알카모스
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep19, 65, 73, 20)--드라칸

	LuaQuestClass:SetQuestRewardRandom(Ep19, 65, 1, 1, 9000, 2, 700, 3, 300)
	LuaQuestClass:SetQuestRewardExp(Ep19, 65, 100000)
	ItemType = 14 ItemIndex = 13 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 65, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--축복의보석
	ItemType = 4 ItemIndex = 16 ItemLevel = 6 dur = 0 ItemSkill = 1 ItemCnt = 1 Overlap = 0 Opt = Opt12
	LuaQuestClass:SetQuestRewardItem(Ep19, 65, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt, Overlap, Opt)--세인트크로스석궁



-- 알파크러스트&알카모스 1차 보상 (290~319레벨)----------------------------------------------------------------------------------

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 66, 69, 20)--알카모스
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep19, 66, 74, 10)--알파크러스트
	
	LuaQuestClass:SetQuestRewardRandom(Ep19, 66, 1, 1, 9000, 2, 700, 3, 300)
	LuaQuestClass:SetQuestRewardExp(Ep19, 66, 140000)
	ItemType = 13 ItemIndex = 0 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 2 
	LuaQuestClass:SetQuestRewardItem(Ep19, 66, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--수호천사
	ItemType = 5 ItemIndex = 9 ItemLevel = 6 dur = 0 ItemSkill = 0 ItemCnt = 1 Overlap = 0 Opt = Opt12
	LuaQuestClass:SetQuestRewardItem(Ep19, 66, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt, Overlap, Opt)--드레곤 소울 지팡이

-- 알파크러스트&알카모스 2차 보상(290~319레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 67, 69, 20)--알카모스
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep19, 67, 74, 10)--알파크러스트

	LuaQuestClass:SetQuestRewardRandom(Ep19, 67, 1, 1, 9000, 2, 700, 3, 300)
	LuaQuestClass:SetQuestRewardExp(Ep19, 67, 130000)
	ItemType = 13 ItemIndex = 1 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1 
	LuaQuestClass:SetQuestRewardItem(Ep19, 67, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--사탄
	ItemType = 5 ItemIndex = 17 ItemLevel = 6 dur = 0 ItemSkill = 0 ItemCnt = 1 Overlap = 0 Opt = Opt12
	LuaQuestClass:SetQuestRewardItem(Ep19, 67, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt, Overlap, Opt)--에이션트 스틱




-- 알파크러스트&알카모스 3차 보상(290~319레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 68, 69, 20)--알카모스
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep19, 68, 74, 10)--알파크러스트

	LuaQuestClass:SetQuestRewardRandom(Ep19, 68, 1, 1, 9000, 2, 700, 3, 300)
	LuaQuestClass:SetQuestRewardExp(Ep19, 68, 120000)
	ItemType = 14 ItemIndex = 19 ItemLevel = 4 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 68, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--데빌 스퀘어 입장권
	ItemType = 14 ItemIndex = 22 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 68, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--창조의보석


	
-- 알파크러스트&알카모스 4차 보상(290~319레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 69, 69, 20)--알카모스
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep19, 69, 74, 10)--알파크러스트

	LuaQuestClass:SetQuestRewardRandom(Ep19, 69, 1, 1, 9000, 2, 700, 3, 300)
	LuaQuestClass:SetQuestRewardExp(Ep19, 69, 110000)
	ItemType = 14 ItemIndex = 14 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 69, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--영혼의보석
	ItemType = 12 ItemIndex = 15 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 69, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--혼돈의보석


-- 알파크러스트&알카모스 5차 보상(290~319레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 70, 69, 20)--알카모스
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep19, 70, 74, 10)--알파크러스트

	LuaQuestClass:SetQuestRewardRandom(Ep19, 70, 1, 1, 9000, 2, 700, 3, 300)
	LuaQuestClass:SetQuestRewardExp(Ep19, 70, 100000)
	ItemType = 14 ItemIndex = 43 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 70, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--하급제련석
	ItemType = 12 ItemIndex = 30 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 70, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--축복의보석*10

	
-- 알파크러스트&알카모스 6차 보상(290~319레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 71, 69, 20)--알카모스
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep19, 71, 74, 10)--알파크러스트

	LuaQuestClass:SetQuestRewardRandom(Ep19, 71, 1, 1, 9000, 2, 700, 3, 300)
	LuaQuestClass:SetQuestRewardExp(Ep19, 71, 90000)
	ItemType = 14 ItemIndex = 13 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 71, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--축복의보석
	ItemType = 14 ItemIndex = 16 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 71, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--생명의보석

-- 알파크러스트&알카모스 7차 보상(290~319레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 72, 69, 20)--알카모스
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep19, 72, 74, 10)--알파크러스트

	LuaQuestClass:SetQuestRewardRandom(Ep19, 72, 1, 1, 9000, 2, 700, 3, 300)
	LuaQuestClass:SetQuestRewardExp(Ep19, 72, 80000)
	ItemType = 14 ItemIndex = 14 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 72, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--영혼의보석
	ItemType = 14 ItemIndex = 19 ItemLevel = 5 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 72, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--데빌스퀘어초대권

-- 알파크러스트&알카모스 8차 보상(290~319레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 73, 69, 20)--알카모스
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep19, 73, 74, 10)--알파크러스트

	LuaQuestClass:SetQuestRewardRandom(Ep19, 73, 1, 1, 9000, 2, 700, 3, 300)
	LuaQuestClass:SetQuestRewardExp(Ep19, 73, 70000)
	ItemType = 13 ItemIndex = 18 ItemLevel = 6 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 73, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--투명망토
	ItemType = 14 ItemIndex = 16 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 73, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--생명의보석



-- 아이템수집&알카모스 1차 보상 (290~319레벨)----------------------------------------------------------------------------------

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 74, 69, 20)--알카모스
	ItemType = 0 ItemIndex = 16 ItemLevel = 0 ItemSkill = 0 ItemCnt = 1 Opt = 0 ExOpt = 0
	LuaQuestClass:SetQuestGetItem(ASK2, Ep19, 74, ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt)--파괴의검
	
	LuaQuestClass:SetQuestRewardRandom(Ep19, 74, 1, 1, 9000, 2, 700, 3, 300)
	LuaQuestClass:SetQuestRewardExp(Ep19, 74, 90000)
	LuaQuestClass:SetQuestRewardZen(Ep19, 74, 200000)
	ItemType = 13 ItemIndex = 1 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 3
	LuaQuestClass:SetQuestRewardItem(Ep19, 74, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--사탄
	
-- 아이템수집&알카모스 2차 보상(290~319레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 75, 69, 20)--알카모스
	ItemType = 0 ItemIndex = 16 ItemLevel = 0 ItemSkill = 0 ItemCnt = 1 Opt = 0 ExOpt = 0
	LuaQuestClass:SetQuestGetItem(ASK2, Ep19, 75, ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt)--파괴의검

	LuaQuestClass:SetQuestRewardRandom(Ep19, 75, 1, 1, 9000, 2, 700, 3, 300)
	LuaQuestClass:SetQuestRewardExp(Ep19, 75, 100000)
	ItemType = 13 ItemIndex = 0 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 2
	LuaQuestClass:SetQuestRewardItem(Ep19, 75, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--사탄
	ItemType = 13 ItemIndex = 18 ItemLevel = 6 dur = 0 ItemSkill = 0 ItemCnt = 1 
	LuaQuestClass:SetQuestRewardItem(Ep19, 75, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--투명망토


-- 아이템수집&알카모스 3차 보상(290~319레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 76, 69, 20)--알카모스
	ItemType = 0 ItemIndex = 16 ItemLevel = 0 ItemSkill = 0 ItemCnt = 1 Opt = 0 ExOpt = 0
	LuaQuestClass:SetQuestGetItem(ASK2, Ep19, 76, ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt)--파괴의검

	LuaQuestClass:SetQuestRewardRandom(Ep19, 76, 1, 1, 9000, 2, 700, 3, 300)
	LuaQuestClass:SetQuestRewardExp(Ep19, 76, 110000)
	ItemType = 14 ItemIndex = 22 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 76, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--창조의보석
	ItemType = 14 ItemIndex = 16 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 76, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--생명의보석

	
-- 아이템수집&알카모스 4차 보상(290~319레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 77, 69, 20)--알카모스
	ItemType = 0 ItemIndex = 16 ItemLevel = 0 ItemSkill = 0 ItemCnt = 1 Opt = 0 ExOpt = 0
	LuaQuestClass:SetQuestGetItem(ASK2, Ep19, 77, ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt)--파괴의검

	LuaQuestClass:SetQuestRewardRandom(Ep19, 77, 1, 1, 9000, 2, 700, 3, 300)
	LuaQuestClass:SetQuestRewardExp(Ep19, 77, 120000)
	ItemType = 14 ItemIndex = 43 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 77, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--하급제련석
	ItemType = 14 ItemIndex = 13 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 77, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--축복의보석


-- 아이템수집&알카모스 5차 보상(290~319레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 78, 69, 20)--알카모스
	ItemType = 0 ItemIndex = 16 ItemLevel = 0 ItemSkill = 0 ItemCnt = 1 Opt = 0 ExOpt = 0
	LuaQuestClass:SetQuestGetItem(ASK2, Ep19, 78, ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt)--파괴의검

	LuaQuestClass:SetQuestRewardRandom(Ep19, 78, 1, 1, 9000, 2, 700, 3, 300)
	LuaQuestClass:SetQuestRewardExp(Ep19, 78, 130000)
	ItemType = 14 ItemIndex = 19 ItemLevel = 4 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 78, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--데빌스퀘어초대권
	ItemType = 14 ItemIndex = 13 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 78, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--축복의보석

	
-- 아이템수집&알카모스 6차 보상(290~319레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 79, 69, 20)--알카모스
	ItemType = 0 ItemIndex = 16 ItemLevel = 0 ItemSkill = 0 ItemCnt = 1 Opt = 0 ExOpt = 0
	LuaQuestClass:SetQuestGetItem(ASK2, Ep19, 79, ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt)--파괴의검

	LuaQuestClass:SetQuestRewardRandom(Ep19, 79, 1, 1, 9000, 2, 700, 3, 300)
	LuaQuestClass:SetQuestRewardExp(Ep19, 79, 140000)
	ItemType = 12 ItemIndex = 15 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 79, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--혼돈의보석
	ItemType = 14 ItemIndex = 16 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 79, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--생명의보석

-- 아이템수집&알카모스 7차 보상(290~319레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 80, 69, 20)--알카모스
	ItemType = 0 ItemIndex = 16 ItemLevel = 0 ItemSkill = 0 ItemCnt = 1 Opt = 0 ExOpt = 0
	LuaQuestClass:SetQuestGetItem(ASK2, Ep19, 80, ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt)--파괴의검

	LuaQuestClass:SetQuestRewardRandom(Ep19, 80, 1, 1, 9000, 2, 700, 3, 300)
	LuaQuestClass:SetQuestRewardExp(Ep19, 80, 70000)
	ItemType = 14 ItemIndex = 14 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 80, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--영혼의보석
	ItemType = 14 ItemIndex = 13 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 80, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--축복의보석


-- 아이템수집&알카모스 8차 보상(290~319레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 81, 69, 20)--알카모스
	ItemType = 0 ItemIndex = 16 ItemLevel = 0 ItemSkill = 0 ItemCnt = 1 Opt = 0 ExOpt = 0
	LuaQuestClass:SetQuestGetItem(ASK2, Ep19, 81, ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt)--파괴의검

	LuaQuestClass:SetQuestRewardRandom(Ep19, 81, 1, 1, 9000, 2, 700, 3, 300)
	LuaQuestClass:SetQuestRewardExp(Ep19, 81, 80000)
	ItemType = 13 ItemIndex = 1 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 81, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--사탄
	ItemType = 14 ItemIndex = 13 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 81, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--축복의보석

	
-- 아이템수집&알카모스 1차 보상 (290~319레벨)----------------------------------------------------------------------------------

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 82, 69, 20)--알카모스
	ItemType = 2 ItemIndex = 5 ItemLevel = 0 ItemSkill = 0 ItemCnt = 1 Opt = 0 ExOpt = 0
	LuaQuestClass:SetQuestGetItem(ASK2, Ep19, 82, ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt)--크리스탈검
	
	LuaQuestClass:SetQuestRewardRandom(Ep19, 82, 1, 1, 9000, 2, 700, 3, 300)
	LuaQuestClass:SetQuestRewardExp(Ep19, 82, 130000)
	LuaQuestClass:SetQuestRewardZen(Ep19, 82, 100000)
	ItemType = 14 ItemIndex = 43 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 82, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--하급제련석
	
-- 아이템수집&알카모스 2차 보상(290~319레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 83, 69, 20)--알카모스
	ItemType = 2 ItemIndex = 5 ItemLevel = 0 ItemSkill = 0 ItemCnt = 1 Opt = 0 ExOpt = 0
	LuaQuestClass:SetQuestGetItem(ASK2, Ep19, 83, ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt)--크리스탈검

	LuaQuestClass:SetQuestRewardRandom(Ep19, 83, 1, 1, 9000, 2, 700, 3, 300)
	LuaQuestClass:SetQuestRewardExp(Ep19, 83, 140000)
	ItemType = 13 ItemIndex = 0 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 2 
	LuaQuestClass:SetQuestRewardItem(Ep19, 83, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--수호천사
	ItemType = 14 ItemIndex = 14 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 83, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--영혼의보석

-- 아이템수집&알카모스 3차 보상(290~319레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 84, 69, 20)--알카모스
	ItemType = 2 ItemIndex = 5 ItemLevel = 0 ItemSkill = 0 ItemCnt = 1 Opt = 0 ExOpt = 0
	LuaQuestClass:SetQuestGetItem(ASK2, Ep19, 84, ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt)--크리스탈검

	LuaQuestClass:SetQuestRewardRandom(Ep19, 84, 1, 1, 9000, 2, 700, 3, 300)
	LuaQuestClass:SetQuestRewardExp(Ep19, 84, 70000)
	ItemType = 12 ItemIndex = 15 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 84, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--혼돈의보석
	ItemType = 14 ItemIndex = 16 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 84, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--생명의보석

	
-- 아이템수집&알카모스 4차 보상(290~319레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 85, 69, 20)--알카모스
	ItemType = 2 ItemIndex = 5 ItemLevel = 0 ItemSkill = 0 ItemCnt = 1 Opt = 0 ExOpt = 0
	LuaQuestClass:SetQuestGetItem(ASK2, Ep19, 85, ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt)--크리스탈검

	LuaQuestClass:SetQuestRewardRandom(Ep19, 85, 1, 1, 9000, 2, 700, 3, 300)
	LuaQuestClass:SetQuestRewardExp(Ep19, 85, 80000)
	ItemType = 14 ItemIndex = 14 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 85, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--영혼의보석
	ItemType = 14 ItemIndex = 13 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 85, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--축복의보석


-- 아이템수집&알카모스 5차 보상(290~319레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 86, 69, 20)--알카모스
	ItemType = 2 ItemIndex = 5 ItemLevel = 0 ItemSkill = 0 ItemCnt = 1 Opt = 0 ExOpt = 0
	LuaQuestClass:SetQuestGetItem(ASK2, Ep19, 86, ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt)--크리스탈검

	LuaQuestClass:SetQuestRewardRandom(Ep19, 86, 1, 1, 9000, 2, 700, 3, 300)
	LuaQuestClass:SetQuestRewardExp(Ep19, 86, 90000)
	ItemType = 14 ItemIndex = 22 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 86, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--창조의보석
	ItemType = 14 ItemIndex = 13 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 86, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--축복의보석

	
-- 아이템수집&알카모스 6차 보상(290~319레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 87, 69, 20)--알카모스
	ItemType = 2 ItemIndex = 5 ItemLevel = 0 ItemSkill = 0 ItemCnt = 1 Opt = 0 ExOpt = 0
	LuaQuestClass:SetQuestGetItem(ASK2, Ep19, 87, ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt)--크리스탈검

	LuaQuestClass:SetQuestRewardRandom(Ep19, 87, 1, 1, 9000, 2, 700, 3, 300)
	LuaQuestClass:SetQuestRewardExp(Ep19, 87, 100000)
	ItemType = 13 ItemIndex = 1 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 87, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--사탄
	ItemType = 13 ItemIndex = 18 ItemLevel = 7 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 87, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--투명망토

-- 아이템수집&알카모스 7차 보상(290~319레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 88, 69, 20)--알카모스
	ItemType = 2 ItemIndex = 5 ItemLevel = 0 ItemSkill = 0 ItemCnt = 1 Opt = 0 ExOpt = 0
	LuaQuestClass:SetQuestGetItem(ASK2, Ep19, 88, ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt)--크리스탈검

	LuaQuestClass:SetQuestRewardRandom(Ep19, 88, 1, 1, 9000, 2, 700, 3, 300)
	LuaQuestClass:SetQuestRewardExp(Ep19, 88, 110000)
	ItemType = 13 ItemIndex = 1 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 88, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--사탄
	ItemType = 14 ItemIndex = 14 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 88, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--영혼의보석

-- 아이템수집&알카모스 8차 보상(290~319레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 89, 69, 20)--알카모스
	ItemType = 2 ItemIndex = 5 ItemLevel = 0 ItemSkill = 0 ItemCnt = 1 Opt = 0 ExOpt = 0
	LuaQuestClass:SetQuestGetItem(ASK2, Ep19, 89, ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt)--크리스탈검

	LuaQuestClass:SetQuestRewardRandom(Ep19, 89, 1, 1, 9000, 2, 700, 3, 300)
	LuaQuestClass:SetQuestRewardExp(Ep19, 89, 120000)
	ItemType = 14 ItemIndex = 14 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 89, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--영혼의보석
	ItemType = 14 ItemIndex = 16 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 89, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--생명의보석
		
---------------------------------------------------------------------------------------------------------------------	

---------------------------------------------------------------------------------------------------------------------
-- 반복 퀘스트 (320~349레벨)
---------------------------------------------------------------------------------------------------------------------

-- 쿤둔의 감시자(320~349레벨)
	LuaQuestClass:SetQuestProgress(ASK1, Ep19, 91)--대화 진행상 완료창이 필요한 경우 사용
	ItemType = 14 ItemIndex = 28 ItemLevel = 5 ItemSkill = 0 ItemCnt = 2 Opt = 0 ExOpt = 0
	LuaQuestClass:SetQuestGetItem(ASK2, Ep19, 92, ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt)--지도구해오기
	
-- 스플린터 울프&사티로스 1차 보상 (320~349레벨)----------------------------------------------------------------------------------
	LuaQuestClass:SetQuestProgress(ASK1, Ep19, 94)--대화 진행상 완료창이 필요한 경우 사용
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 95, 351, 20)--스플린터 울프
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep19, 95, 353, 20)--사티로스
	
	LuaQuestClass:SetQuestRewardRandom(Ep19, 95, 1, 1, 8800, 2, 800, 3, 400)
	LuaQuestClass:SetQuestRewardExp(Ep19, 95, 90000)
	ItemType = 13 ItemIndex = 0 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 2 
	LuaQuestClass:SetQuestRewardItem(Ep19, 95, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--수호천사
	ItemType = 14 ItemIndex = 13 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 95, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--축복의보석
	
-- 스플린터 울프&사티로스 2차 보상(320~349레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 96, 351, 20)--스플린터 울프
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep19, 96, 353, 20)--사티로스

	LuaQuestClass:SetQuestRewardRandom(Ep19, 96, 1, 1, 8800, 2, 800, 3, 400)
	LuaQuestClass:SetQuestRewardExp(Ep19, 96, 100000)
	ItemType = 13 ItemIndex = 1 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1 
	LuaQuestClass:SetQuestRewardItem(Ep19, 96, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--사탄
	ItemType = 14 ItemIndex = 14 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 96, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--영혼의보석




-- 스플린터 울프&사티로스 3차 보상(320~349레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 97, 351, 20)--스플린터 울프
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep19, 97, 353, 20)--사티로스

	LuaQuestClass:SetQuestRewardRandom(Ep19, 97, 1, 1, 8800, 2, 800, 3, 400)
	LuaQuestClass:SetQuestRewardExp(Ep19, 97, 110000)
	ItemType = 14 ItemIndex = 14 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1 
	LuaQuestClass:SetQuestRewardItem(Ep19, 97, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--영혼의보석
	ItemType = 14 ItemIndex = 19 ItemLevel = 5 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 97, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--데빌스퀘어초대권

	
-- 스플린터 울프&사티로스 4차 보상(320~349레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 98, 351, 20)--스플린터 울프
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep19, 98, 353, 20)--사티로스

	LuaQuestClass:SetQuestRewardRandom(Ep19, 98, 1, 1, 8800, 2, 800, 3, 400)
	LuaQuestClass:SetQuestRewardExp(Ep19, 98, 120000)
	ItemType = 14 ItemIndex = 22 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 98, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--창조의보석
	ItemType = 12 ItemIndex = 15 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 98, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--혼돈의보석


-- 스플린터 울프&사티로스 5차 보상(320~349레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 99, 351, 20)--스플린터 울프
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep19, 99, 353, 20)--사티로스

	LuaQuestClass:SetQuestRewardRandom(Ep19, 99, 1, 1, 8800, 2, 800, 3, 400)
	LuaQuestClass:SetQuestRewardExp(Ep19, 99, 130000)
	ItemType = 14 ItemIndex = 14 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 99, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--영혼의보석
	ItemType = 14 ItemIndex = 13 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 99, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--축복의보석

	
-- 스플린터 울프&사티로스 6차 보상(320~349레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 100, 351, 20)--스플린터 울프
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep19, 100, 353, 20)--사티로스

	LuaQuestClass:SetQuestRewardRandom(Ep19, 100, 1, 1, 8800, 2, 800, 3, 400)
	LuaQuestClass:SetQuestRewardExp(Ep19, 100, 140000)
	ItemType = 14 ItemIndex = 43 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 100, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--혼돈의보석
	ItemType = 0 ItemIndex = 20 ItemLevel = 0 dur = 0 ItemSkill = 1 ItemCnt = 1 Overlap = 0 Opt = Opt12
	LuaQuestClass:SetQuestRewardItem(Ep19, 100, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt, Overlap, Opt)--나이트블레이드

-- 스플린터 울프&사티로스 7차 보상(320~349레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 101, 351, 20)--스플린터 울프
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep19, 101, 353, 20)--사티로스

	LuaQuestClass:SetQuestRewardRandom(Ep19, 101, 1, 1, 8800, 2, 800, 3, 400)
	LuaQuestClass:SetQuestRewardExp(Ep19, 101, 150000)
	LuaQuestClass:SetQuestRewardZen(Ep19, 101, 200000)
	ItemType = 14 ItemIndex = 13 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 101, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--축복의보석

-- 스플린터 울프&사티로스 8차 보상(320~349레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 102, 351, 20)--스플린터 울프
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep19, 102, 353, 20)--사티로스

	LuaQuestClass:SetQuestRewardRandom(Ep19, 102, 1, 1, 8800, 2, 800, 3, 400)
	LuaQuestClass:SetQuestRewardExp(Ep19, 102, 160000)
	ItemType = 12 ItemIndex = 15 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 102, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--혼돈의보석
	ItemType = 14 ItemIndex = 13 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 102, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--축복의보석





-- 스플린터 울프&블레이드헌터 1차 보상 (320~349레벨)----------------------------------------------------------------------------------

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 103, 351, 20)--스플린터 울프
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep19, 103, 354, 10)--블레이드헌터
	
	LuaQuestClass:SetQuestRewardRandom(Ep19, 103, 1, 1, 8800, 2, 800, 3, 400)
	LuaQuestClass:SetQuestRewardExp(Ep19, 103, 110000)
	ItemType = 13 ItemIndex = 1 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1 
	LuaQuestClass:SetQuestRewardItem(Ep19, 103, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--사탄
	ItemType = 12 ItemIndex = 15 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 103, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--혼돈의보석
	
-- 스플린터 울프&블레이드헌터 2차 보상(320~349레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 104, 351, 20)--스플린터 울프
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep19, 104, 354, 10)--블레이드헌터

	LuaQuestClass:SetQuestRewardRandom(Ep19, 104, 1, 1, 8800, 2, 800, 3, 400)
	LuaQuestClass:SetQuestRewardExp(Ep19, 104, 120000)
	ItemType = 14 ItemIndex = 19 ItemLevel = 5 dur = 0 ItemSkill = 0 ItemCnt = 1 
	LuaQuestClass:SetQuestRewardItem(Ep19, 104, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--데빌스퀘어초대권
	ItemType = 12 ItemIndex = 15 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1 
	LuaQuestClass:SetQuestRewardItem(Ep19, 104, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--혼돈의보석




-- 스플린터 울프&블레이드헌터 3차 보상(320~349레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 105, 351, 20)--스플린터 울프
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep19, 105, 354, 10)--블레이드헌터

	LuaQuestClass:SetQuestRewardRandom(Ep19, 105, 1, 1, 8800, 2, 800, 3, 400)
	LuaQuestClass:SetQuestRewardExp(Ep19, 105, 130000)
	ItemType = 14 ItemIndex = 14 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1 
	LuaQuestClass:SetQuestRewardItem(Ep19, 105, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--영혼의보석
	ItemType = 14 ItemIndex = 13 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1 
	LuaQuestClass:SetQuestRewardItem(Ep19, 105, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--축복의보석

	
-- 스플린터 울프&블레이드헌터 4차 보상(320~349레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 106, 351, 20)--스플린터 울프
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep19, 106, 354, 10)--블레이드헌터

	LuaQuestClass:SetQuestRewardRandom(Ep19, 106, 1, 1, 8800, 2, 800, 3, 400)
	LuaQuestClass:SetQuestRewardExp(Ep19, 106, 140000)
	ItemType = 14 ItemIndex = 14 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 106, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--영혼의보석
	ItemType = 14 ItemIndex = 16 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 106, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--생명의보석


-- 스플린터 울프&블레이드헌터 5차 보상(320~349레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 107, 351, 20)--스플린터 울프
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep19, 107, 354, 10)--블레이드헌터

	LuaQuestClass:SetQuestRewardRandom(Ep19, 107, 1, 1, 8800, 2, 800, 3, 400)
	LuaQuestClass:SetQuestRewardExp(Ep19, 107, 150000)
	ItemType = 13 ItemIndex = 0 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 2
	LuaQuestClass:SetQuestRewardItem(Ep19, 107, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--수호천사
	ItemType = 2 ItemIndex = 11 ItemLevel = 0 dur = 0 ItemSkill = 1 ItemCnt = 1 Overlap = 0 Opt = Opt12
	LuaQuestClass:SetQuestRewardItem(Ep19, 107, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt, Overlap, Opt)--로드셉터

	
-- 스플린터 울프&블레이드헌터 6차 보상(320~349레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 108, 351, 20)--스플린터 울프
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep19, 108, 354, 10)--블레이드헌터

	LuaQuestClass:SetQuestRewardRandom(Ep19, 108, 1, 1, 8800, 2, 800, 3, 400)
	LuaQuestClass:SetQuestRewardExp(Ep19, 108, 160000)
	ItemType = 14 ItemIndex = 43 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 108, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--하급제련석
	ItemType = 2 ItemIndex = 6 ItemLevel = 5 dur = 0 ItemSkill = 0 ItemCnt = 1 Overlap = 0 Opt = Opt12
	LuaQuestClass:SetQuestRewardItem(Ep19,108, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt, Overlap, Opt)--카오스도끼

-- 스플린터 울프&블레이드헌터 7차 보상(320~349레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 109, 351, 20)--스플린터 울프
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep19, 109, 354, 10)--블레이드헌터

	LuaQuestClass:SetQuestRewardRandom(Ep19, 109, 1, 1, 8800, 2, 800, 3, 400)
	LuaQuestClass:SetQuestRewardExp(Ep19, 109, 90000)
	ItemType = 14 ItemIndex = 43 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 109, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--하급제련석
	ItemType = 14 ItemIndex = 13 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 109, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--축복의보석

-- 스플린터 울프&블레이드헌터 8차 보상(320~349레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 110, 351, 20)--스플린터 울프
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep19, 110, 354, 10)--블레이드헌터

	LuaQuestClass:SetQuestRewardRandom(Ep19, 110, 1, 1, 8800, 2, 800, 3, 400)
	LuaQuestClass:SetQuestRewardExp(Ep19, 110, 100000)
	ItemType = 14 ItemIndex = 14 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 110, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--영혼의보석
	ItemType = 14 ItemIndex = 16 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 110, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--생명의보석



-- 알파크러스트&알카모스 1차 보상 (320~349레벨)----------------------------------------------------------------------------------

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 111, 351, 20)--스플린터 울프
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep19, 111, 355, 5)--켄타우로스
	
	LuaQuestClass:SetQuestRewardRandom(Ep19, 111, 1, 1, 8800, 2, 800, 3, 400)
	LuaQuestClass:SetQuestRewardExp(Ep19, 111, 160000)
	ItemType = 4 ItemIndex = 22 ItemLevel = 0 dur = 0 ItemSkill = 1 ItemCnt = 1 Overlap = 0 Opt = Opt12
	LuaQuestClass:SetQuestRewardItem(Ep19, 111, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt, Overlap, Opt)--알바트로스보우
	ItemType = 14 ItemIndex = 16 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1 
	LuaQuestClass:SetQuestRewardItem(Ep19, 111, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--생명의보석
	
-- 알파크러스트&알카모스 2차 보상(320~349레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 112, 351, 20)--스플린터 울프
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep19, 112, 355, 5)--켄타우로스

	LuaQuestClass:SetQuestRewardRandom(Ep19, 112, 1, 1, 8800, 2, 800, 3, 400)
	LuaQuestClass:SetQuestRewardExp(Ep19, 112, 150000)
	ItemType = 14 ItemIndex = 43 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1 
	LuaQuestClass:SetQuestRewardItem(Ep19, 112, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--하급제련석
	ItemType = 14 ItemIndex = 13 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1 
	LuaQuestClass:SetQuestRewardItem(Ep19, 112, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--축복의보석




-- 알파크러스트&알카모스 3차 보상(320~349레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 113, 351, 20)--스플린터 울프
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep19, 113, 355, 5)--켄타우로스

	LuaQuestClass:SetQuestRewardRandom(Ep19, 113, 1, 1, 8800, 2, 800, 3, 400)
	LuaQuestClass:SetQuestRewardExp(Ep19, 113, 140000)
	ItemType = 14 ItemIndex = 14 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 113, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--영혼의보석
	ItemType = 14 ItemIndex = 16 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 113, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--생명의보석


	
-- 알파크러스트&알카모스 4차 보상(320~349레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 114, 351, 20)--스플린터 울프
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep19, 114, 355, 5)--켄타우로스

	LuaQuestClass:SetQuestRewardRandom(Ep19, 114, 1, 1, 8800, 2, 800, 3, 400)
	LuaQuestClass:SetQuestRewardExp(Ep19, 114, 130000)
	ItemType = 14 ItemIndex = 22 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 114, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--창조의보석
	ItemType = 14 ItemIndex = 13 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 114, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--축복의보석


-- 알파크러스트&알카모스 5차 보상(320~349레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 115, 351, 20)--스플린터 울프
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep19, 115, 355, 5)--켄타우로스

	LuaQuestClass:SetQuestRewardRandom(Ep19, 115, 1, 1, 8800, 2, 800, 3, 400)
	LuaQuestClass:SetQuestRewardExp(Ep19, 115, 120000)
	ItemType = 5 ItemIndex = 11 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1 Overlap = 0 Opt = Opt12
	LuaQuestClass:SetQuestRewardItem(Ep19, 115, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt, Overlap, Opt)--쿤둔의지팡이
	ItemType = 13 ItemIndex = 18 ItemLevel = 7 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 115, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--투명망토

	
-- 알파크러스트&알카모스 6차 보상(320~349레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 116, 351, 20)--스플린터 울프
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep19, 116, 355, 5)--켄타우로스

	LuaQuestClass:SetQuestRewardRandom(Ep19, 116, 1, 1, 8800, 2, 800, 3, 400)
	LuaQuestClass:SetQuestRewardExp(Ep19, 116, 110000)
	ItemType = 13 ItemIndex = 1 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 116, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--사탄
	ItemType = 12 ItemIndex = 15 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 116, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--혼돈의보석

-- 알파크러스트&알카모스 7차 보상(320~349레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 117, 351, 20)--스플린터 울프
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep19, 117, 355, 5)--켄타우로스

	LuaQuestClass:SetQuestRewardRandom(Ep19, 117, 1, 1, 8800, 2, 800, 3, 400)
	LuaQuestClass:SetQuestRewardExp(Ep19, 117, 100000)
	ItemType = 14 ItemIndex = 43 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 117, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--하급제련석
	ItemType = 14 ItemIndex = 16 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 117, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--생명의보석

-- 알파크러스트&알카모스 8차 보상(320~349레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 118, 351, 20)--스플린터 울프
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep19, 118, 355, 5)--켄타우로스

	LuaQuestClass:SetQuestRewardRandom(Ep19, 118, 1, 1, 8800, 2, 800, 3, 400)
	LuaQuestClass:SetQuestRewardExp(Ep19, 118, 90000)
	ItemType = 14 ItemIndex = 19 ItemLevel = 6 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 118, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--데빌스퀘어초대권
	ItemType = 12 ItemIndex = 30 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 118, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--축복의보석*10



-- 아이언라이더 1차 보상 (320~349레벨)----------------------------------------------------------------------------------

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 119, 352, 30)--아이언라이더

	LuaQuestClass:SetQuestRewardRandom(Ep19, 119, 1, 1, 8800, 2, 800, 3, 400)
	LuaQuestClass:SetQuestRewardExp(Ep19, 119, 100000)
	ItemType = 5 ItemIndex = 18 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1 Overlap = 0 Opt = Opt12
	LuaQuestClass:SetQuestRewardItem(Ep19, 119, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt, Overlap, Opt)--데모닉스틱
	ItemType = 14 ItemIndex = 19 ItemLevel = 6 dur = 0 ItemSkill = 0 ItemCnt = 1 
	LuaQuestClass:SetQuestRewardItem(Ep19, 119, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--데빌스퀘어초대권
	
-- 아이언라이더 2차 보상(320~349레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 120, 352, 30)--아이언라이더

	LuaQuestClass:SetQuestRewardRandom(Ep19, 120, 1, 1, 8800, 2, 800, 3, 400)
	LuaQuestClass:SetQuestRewardExp(Ep19, 120, 110000)
	ItemType = 13 ItemIndex = 0 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 2 
	LuaQuestClass:SetQuestRewardItem(Ep19, 120, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--수호천사
	ItemType = 14 ItemIndex = 13 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 120, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--축복의보석




-- 아이언라이더 3차 보상(320~349레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 121, 352, 30)--아이언라이더

	LuaQuestClass:SetQuestRewardRandom(Ep19, 121, 1, 1, 8800, 2, 800, 3, 400)
	LuaQuestClass:SetQuestRewardExp(Ep19, 121, 120000)
	ItemType = 14 ItemIndex = 14 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1 
	LuaQuestClass:SetQuestRewardItem(Ep19, 121, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--영혼의보석
	ItemType = 14 ItemIndex = 16 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 121, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--생명의보석

	
-- 아이언라이더 4차 보상(320~349레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 122, 352, 30)--아이언라이더

	LuaQuestClass:SetQuestRewardRandom(Ep19, 122, 1, 1, 8800, 2, 800, 3, 400)
	LuaQuestClass:SetQuestRewardExp(Ep19, 122, 130000)
	ItemType = 14 ItemIndex = 43 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 122, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--하급제련석
	ItemType = 12 ItemIndex = 15 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 122, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--혼돈의보석


-- 아이언라이더 5차 보상(320~349레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 123, 352, 30)--아이언라이더

	LuaQuestClass:SetQuestRewardRandom(Ep19, 123, 1, 1, 8800, 2, 800, 3, 400)
	LuaQuestClass:SetQuestRewardExp(Ep19, 123, 140000)
	ItemType = 13 ItemIndex = 0 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 2
	LuaQuestClass:SetQuestRewardItem(Ep19, 123, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--수호천사
	ItemType = 12 ItemIndex = 15 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 123, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--혼돈의보석

	
-- 아이언라이더 6차 보상(320~349레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 124, 352, 30)--아이언라이더

	LuaQuestClass:SetQuestRewardRandom(Ep19, 124, 1, 1, 8800, 2, 800, 3, 400)
	LuaQuestClass:SetQuestRewardExp(Ep19, 124, 150000)
	ItemType = 12 ItemIndex = 15 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 124, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--혼돈의보석
	ItemType = 14 ItemIndex = 16 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 124, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--생명의보석

-- 아이언라이더 7차 보상(320~349레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 125, 352, 30)--아이언라이더

	LuaQuestClass:SetQuestRewardRandom(Ep19, 125, 1, 1, 8800, 2, 800, 3, 400)
	LuaQuestClass:SetQuestRewardExp(Ep19, 125, 160000)
	ItemType = 13 ItemIndex = 1 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 125, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--사탄
	ItemType = 14 ItemIndex = 16 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 125, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--생명의보석

-- 아이언라이더 8차 보상(320~349레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 126, 352, 30)--아이언라이더

	LuaQuestClass:SetQuestRewardRandom(Ep19, 126, 1, 1, 8800, 2, 800, 3, 400)
	LuaQuestClass:SetQuestRewardExp(Ep19, 126, 90000)
	ItemType = 14 ItemIndex = 14 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 126, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--영혼의보석
	ItemType = 14 ItemIndex = 13 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 126, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--축복의보석


	
-- 아이템수집&알카모스 1차 보상 (320~349레벨)----------------------------------------------------------------------------------

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 127, 351, 20)--스플린터 울프
	ItemType = 5 ItemIndex = 17 ItemLevel = 0 ItemSkill = 0 ItemCnt = 1 Opt = 0 ExOpt = 0
	LuaQuestClass:SetQuestGetItem(ASK2, Ep19, 127, ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt)--에이션트스틱
	
	LuaQuestClass:SetQuestRewardRandom(Ep19, 127, 1, 1, 8800, 2, 800, 3, 400)
	LuaQuestClass:SetQuestRewardExp(Ep19, 127, 140000)
	LuaQuestClass:SetQuestRewardZen(Ep19, 127, 200000)
	ItemType = 13 ItemIndex = 1 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 127, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--사탄
	
-- 아이템수집&알카모스 2차 보상(320~349레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 128, 351, 20)--스플린터 울프
	ItemType = 5 ItemIndex = 17 ItemLevel = 0 ItemSkill = 0 ItemCnt = 1 Opt = 0 ExOpt = 0
	LuaQuestClass:SetQuestGetItem(ASK2, Ep19, 128, ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt)--에이션트스틱

	LuaQuestClass:SetQuestRewardRandom(Ep19, 128, 1, 1, 8800, 2, 800, 3, 400)
	LuaQuestClass:SetQuestRewardExp(Ep19, 128, 150000)
	ItemType = 13 ItemIndex = 0 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 2 
	LuaQuestClass:SetQuestRewardItem(Ep19, 128, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--수호천사
	ItemType = 13 ItemIndex = 1 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 128, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--사탄
	
-- 아이템수집&알카모스 3차 보상(320~349레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 129, 351, 20)--스플린터 울프
	ItemType = 5 ItemIndex = 17 ItemLevel = 0 ItemSkill = 0 ItemCnt = 1 Opt = 0 ExOpt = 0
	LuaQuestClass:SetQuestGetItem(ASK2, Ep19, 129, ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt)--에이션트스틱

	LuaQuestClass:SetQuestRewardRandom(Ep19, 129, 1, 1, 8800, 2, 800, 3, 400)
	LuaQuestClass:SetQuestRewardExp(Ep19, 129, 160000)
	ItemType = 13 ItemIndex = 0 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 2 
	LuaQuestClass:SetQuestRewardItem(Ep19, 129, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--수호천사
	ItemType = 14 ItemIndex = 14 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 129, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--영혼의보석

	
-- 아이템수집&알카모스 4차 보상(320~349레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 130, 351, 20)--스플린터 울프
	ItemType = 5 ItemIndex = 17 ItemLevel = 0 ItemSkill = 0 ItemCnt = 1 Opt = 0 ExOpt = 0
	LuaQuestClass:SetQuestGetItem(ASK2, Ep19, 130, ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt)--에이션트스틱

	LuaQuestClass:SetQuestRewardRandom(Ep19, 130, 1, 1, 8800, 2, 800, 3, 400)
	LuaQuestClass:SetQuestRewardExp(Ep19, 130, 90000)
	ItemType = 14 ItemIndex = 14 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 130, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--혼돈의보석
	ItemType = 14 ItemIndex = 13 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 130, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--축복의보석


-- 아이템수집&알카모스 5차 보상(320~349레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 131, 351, 20)--스플린터 울프
	ItemType = 5 ItemIndex = 17 ItemLevel = 0 ItemSkill = 0 ItemCnt = 1 Opt = 0 ExOpt = 0
	LuaQuestClass:SetQuestGetItem(ASK2, Ep19, 131, ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt)--에이션트스틱

	LuaQuestClass:SetQuestRewardRandom(Ep19, 131, 1, 1, 8800, 2, 800, 3, 400)
	LuaQuestClass:SetQuestRewardExp(Ep19, 131, 100000)
	ItemType = 14 ItemIndex = 14 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 131, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--영혼의보석
	ItemType = 13 ItemIndex = 18 ItemLevel = 7 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 131, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--투명망토

	
-- 아이템수집&알카모스 6차 보상(320~349레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 132, 351, 20)--스플린터 울프
	ItemType = 5 ItemIndex = 17 ItemLevel = 0 ItemSkill = 0 ItemCnt = 1 Opt = 0 ExOpt = 0
	LuaQuestClass:SetQuestGetItem(ASK2, Ep19, 132, ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt)--에이션트스틱

	LuaQuestClass:SetQuestRewardRandom(Ep19, 132, 1, 1, 8800, 2, 800, 3, 400)
	LuaQuestClass:SetQuestRewardExp(Ep19, 132, 110000)
	ItemType = 14 ItemIndex = 43 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 132, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--하급제련석
	ItemType = 2 ItemIndex = 6 ItemLevel = 6 dur = 0 ItemSkill = 0 ItemCnt = 1 Overlap = 0 Opt = Opt12
	LuaQuestClass:SetQuestRewardItem(Ep19, 132, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt, Overlap, Opt)--카오스도끼

-- 아이템수집&알카모스 7차 보상(320~349레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 133, 351, 20)--스플린터 울프
	ItemType = 5 ItemIndex = 17 ItemLevel = 0 ItemSkill = 0 ItemCnt = 1 Opt = 0 ExOpt = 0
	LuaQuestClass:SetQuestGetItem(ASK2, Ep19, 133, ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt)--에이션트스틱

	LuaQuestClass:SetQuestRewardRandom(Ep19, 133, 1, 1, 8800, 2, 800, 3, 400)
	LuaQuestClass:SetQuestRewardExp(Ep19, 133, 120000)
	ItemType = 13 ItemIndex = 1 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 133, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--사탄
	ItemType = 14 ItemIndex = 16 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 133, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--생명의보석

-- 아이템수집&알카모스 8차 보상(320~349레벨)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep19, 134, 351, 20)--스플린터 울프
	ItemType = 5 ItemIndex = 17 ItemLevel = 0 ItemSkill = 0 ItemCnt = 1 Opt = 0 ExOpt = 0
	LuaQuestClass:SetQuestGetItem(ASK2, Ep19, 134, ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt)--에이션트스틱

	LuaQuestClass:SetQuestRewardRandom(Ep19, 134, 1, 1, 8800, 2, 800, 3, 400)
	LuaQuestClass:SetQuestRewardExp(Ep19, 134, 130000)
	ItemType = 14 ItemIndex = 14 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 134, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--영혼의보석
	ItemType = 12 ItemIndex = 15 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep19, 134, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--혼돈의보석
	
---------------------------------------------------------------------------------------------------------------------	
end
	
	