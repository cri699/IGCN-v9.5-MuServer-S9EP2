--[[
Quest_Reward_Ep8.lua
[ Start  ] 	2011.07.18
[ Latest ]	2012-04-27 4:51 PM
[ Version ]	0.0.0.3
[ Text ]	해외 버그로 조각묶음 인식이 되지 않는 현상 수정
[ Desc ]									
--]]
--================================================================================================================
function Quest_Reward_Window_Ep8(iObjIndex)

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
	local ItemType  = 0
	local ItemIndex = 0 
	local ItemLevel = 0 
	local dur       = 0 
	local ItemSkill = 0 
	local ItemCnt   = 0
	local Overlap   = 0 -- 아이템이 겹쳐지는가?
	local Opt	= 0
	local ExOpt	= 0
	
-- 튜토리얼

LuaQuestClass:SetQuestProgress(ASK1, Ep8, 4)--대화 진행상 완료창이 필요한 경우 사용

	ItemType = 12 ItemIndex = 200 ItemLevel = 0 ItemSkill = 0 ItemCnt = 1 Opt = 0 ExOpt = 0
	LuaQuestClass:SetQuestGetItem(ASK1, Ep8, 5, ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt)	-- 펜타그램 아이템 체크
	
	LuaQuestClass:SetQuestRewardExp(Ep8, 5, 100000)
	ItemType = 12 ItemIndex = 150 ItemLevel = 0 dur = 1 ItemSkill = 0 ItemCnt = 1 
	LuaQuestClass:SetQuestRewardItem(Ep8, 5, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)		-- 보석 조합틀


LuaQuestClass:SetQuestProgress(ASK1, Ep8, 9)--대화 진행상 완료창이 필요한 경우 사용

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep8, 10, 586, 20)							--데블 페어리, 20
	LuaQuestClass:SetQuestRewardExp(Ep8, 10, 100000)
	ItemType = 12 ItemIndex = 221 ItemLevel = 0 dur = 1 ItemSkill = 0 ItemCnt = 1 
	LuaQuestClass:SetQuestRewardItem(Ep8, 10, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)		-- 분노의 에르텔


LuaQuestClass:SetQuestProgress(ASK1, Ep8, 15)--대화 진행상 완료창이 필요한 경우 사용

	ItemType = 12 ItemIndex = 148 ItemLevel = 0 ItemSkill = 0 ItemCnt = 1 Opt = 0 ExOpt = 0
	LuaQuestClass:SetQuestGetItem(ASK1, Ep8, 16, ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt)	-- 미스릴 조각 묶음 체크
	
	LuaQuestClass:SetQuestRewardExp(Ep8, 16, 100000)
	ItemType = 12 ItemIndex = 151 ItemLevel = 0 dur = 1 ItemSkill = 0 ItemCnt = 1 
	LuaQuestClass:SetQuestRewardItem(Ep8, 16, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)		-- 정령 가루


LuaQuestClass:SetQuestProgress(ASK1, Ep8, 18)--대화 진행상 완료창이 필요한 경우 사용

	ItemType = 12 ItemIndex = 149 ItemLevel = 0 ItemSkill = 0 ItemCnt = 1 Opt = 0 ExOpt = 0
	LuaQuestClass:SetQuestGetItem(ASK1, Ep8, 19, ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt)	-- 엘릭서 조각 묶음 체크
	
	LuaQuestClass:SetQuestRewardExp(Ep8, 19, 100000)
	ItemType = 12 ItemIndex = 150 ItemLevel = 0 dur = 1 ItemSkill = 0 ItemCnt = 1 
	LuaQuestClass:SetQuestRewardItem(Ep8, 19, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)		-- 보석 조합틀


LuaQuestClass:SetQuestProgress(ASK1, Ep8, 25)--대화 진행상 완료창이 필요한 경우 사용

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep8, 26, 587, 20)							-- 엘리멘탈 비스트, 20
	LuaQuestClass:SetQuestRewardExp(Ep8, 26, 100000)
	ItemType = 12 ItemIndex = 231 ItemLevel = 0 dur = 1 ItemSkill = 0 ItemCnt = 1 
	LuaQuestClass:SetQuestRewardItem(Ep8, 26, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)		-- 가호의 에르텔


-- 사냥터 1 퀘스트

LuaQuestClass:SetQuestProgress(ASK1, Ep8, 28)--대화 진행상 완료창이 필요한 경우 사용

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep8, 29, 586, 30)							-- 데블 페어리, 30
	LuaQuestClass:SetQuestRewardExp(Ep8, 29, 100000)
	ItemType = 12 ItemIndex = 144 ItemLevel = 0 dur = 1 ItemSkill = 0 ItemCnt = 1 
	LuaQuestClass:SetQuestRewardItem(Ep8, 29, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)		-- 미스릴 조각


LuaQuestClass:SetQuestProgress(ASK1, Ep8, 31)--대화 진행상 완료창이 필요한 경우 사용

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep8, 32, 587, 30)							-- 엘리멘탈 비스트, 30
	LuaQuestClass:SetQuestRewardExp(Ep8, 32, 100000)
	ItemType = 12 ItemIndex = 144 ItemLevel = 0 dur = 1 ItemSkill = 0 ItemCnt = 1 
	LuaQuestClass:SetQuestRewardItem(Ep8, 32, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)		-- 미스릴 조각


LuaQuestClass:SetQuestProgress(ASK1, Ep8, 34)--대화 진행상 완료창이 필요한 경우 사용

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep8, 35, 588, 30)							-- 엘리멘탈 나이트, 30
	LuaQuestClass:SetQuestRewardExp(Ep8, 35, 100000)
	ItemType = 12 ItemIndex = 146 ItemLevel = 0 dur = 1 ItemSkill = 0 ItemCnt = 1 
	LuaQuestClass:SetQuestRewardItem(Ep8, 35, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)		-- 엘릭서 조각


--사냥터 2 퀘스트

LuaQuestClass:SetQuestProgress(ASK1, Ep8, 37)--대화 진행상 완료창이 필요한 경우 사용

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep8, 38, 589, 30)							-- 우바이드 데블 페어리, 30
	LuaQuestClass:SetQuestRewardExp(Ep8, 38, 100000)
	ItemType = 12 ItemIndex = 145 ItemLevel = 0 dur = 1 ItemSkill = 0 ItemCnt = 1 
	LuaQuestClass:SetQuestRewardItem(Ep8, 38, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)		-- 미스릴


LuaQuestClass:SetQuestProgress(ASK1, Ep8, 40)--대화 진행상 완료창이 필요한 경우 사용

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep8, 41, 590, 30)							-- 우바이드 엘리멘탈 비스트, 30
	LuaQuestClass:SetQuestRewardExp(Ep8, 41, 100000)
	ItemType = 12 ItemIndex = 147 ItemLevel = 0 dur = 1 ItemSkill = 0 ItemCnt = 1 
	LuaQuestClass:SetQuestRewardItem(Ep8, 41, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)		-- 엘릭서


LuaQuestClass:SetQuestProgress(ASK1, Ep8, 43)--대화 진행상 완료창이 필요한 경우 사용

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep8, 44, 591, 30)							-- 우바이드 엘리멘탈 나이트, 30
	LuaQuestClass:SetQuestRewardExp(Ep8, 44, 100000)
	ItemType = 12 ItemIndex = 150 ItemLevel = 0 dur = 1 ItemSkill = 0 ItemCnt = 1 
	LuaQuestClass:SetQuestRewardItem(Ep8, 44, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)		-- 보석 조합틀


-- 사냥터 1 반복 퀘스트

LuaQuestClass:SetQuestProgress(ASK1, Ep8, 46)--대화 진행상 완료창이 필요한 경우 사용

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep8, 47, 586, 50)			-- 데블 페어리, 50
	LuaQuestClass:SetQuestRewardExp(Ep8, 47, 200000)				-- 경험치 200000
	LuaQuestClass:SetQuestRewardZen(Ep8, 47, 100000)				-- 젠 100000


	
LuaQuestClass:SetQuestProgress(ASK1, Ep8, 49)--대화 진행상 완료창이 필요한 경우 사용

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep8, 50, 587, 50)			-- 엘리멘탈 비스트, 50
	LuaQuestClass:SetQuestRewardExp(Ep8, 50, 200000)				-- 경험치 200000
	LuaQuestClass:SetQuestRewardZen(Ep8, 50, 100000)				-- 젠 100000

LuaQuestClass:SetQuestProgress(ASK1, Ep8, 52)--대화 진행상 완료창이 필요한 경우 사용

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep8, 53, 588, 50)			-- 엘리멘탈 나이트, 50
	LuaQuestClass:SetQuestRewardExp(Ep8, 53, 200000)				-- 경험치 200000
	LuaQuestClass:SetQuestRewardZen(Ep8, 53, 100000)				-- 젠 100000


--사냥터 2 반복 퀘스트

LuaQuestClass:SetQuestProgress(ASK1, Ep8, 55)--대화 진행상 완료창이 필요한 경우 사용

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep8, 56, 589, 50)			-- 우바이드 데블 페어리, 50
	LuaQuestClass:SetQuestRewardExp(Ep8, 56, 300000)				-- 경험치 300000
	LuaQuestClass:SetQuestRewardZen(Ep8, 56, 100000)				-- 젠 100000
	
LuaQuestClass:SetQuestProgress(ASK1, Ep8, 58)--대화 진행상 완료창이 필요한 경우 사용

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep8, 59, 590, 50)			-- 우바이드 엘리멘탈 비스트, 50
	LuaQuestClass:SetQuestRewardExp(Ep8, 59, 300000)				-- 경험치 300000
	LuaQuestClass:SetQuestRewardZen(Ep8, 59, 100000)				-- 젠 100000

LuaQuestClass:SetQuestProgress(ASK1, Ep8, 61)--대화 진행상 완료창이 필요한 경우 사용

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep8, 62, 591, 50)			-- 우바이드 엘리멘탈 나이트, 50
	LuaQuestClass:SetQuestRewardExp(Ep8, 62, 300000)				-- 경험치 300000
	LuaQuestClass:SetQuestRewardZen(Ep8, 62, 100000)				-- 젠 100000
end
--================================================================================================================