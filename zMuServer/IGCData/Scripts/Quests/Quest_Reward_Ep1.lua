--[[
Quest_Reward_Ep1.lua
[ Start  ] 	2009.08.
[ Latest ]	2009-12-09 16:45 PM
[ Version ]	0.0.0.1
[ Text ]									
[ Desc ]									
--]]

function Quest_Reward_Window_Ep1(iObjIndex)

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
	
	LuaQuestClass:SetQuestProgress(ASK1, Ep1, 2)--대화 진행상 완료창이 필요한 경우 사용
	
	LuaQuestClass:SetQuestProgress(ASK1, Ep1, 68)
	
	LuaQuestClass:SetQuestProgress(ASK1, Ep1, 72)
	
	LuaQuestClass:SetQuestProgress(ASK1, Ep1, 75)
	
	LuaQuestClass:SetQuestProgress(ASK1, Ep1, 78)
	
	LuaQuestClass:SetQuestProgress(ASK1, Ep1, 81)
	
	LuaQuestClass:SetQuestProgress(ASK1, Ep1, 84)
		
	ItemType = 13 ItemIndex = 20 ItemLevel = 1 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep1, 4, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)
	ItemType = 13 ItemIndex = 20 ItemLevel = 2 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep1, 4, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)

	LuaQuestClass:SetTutorial(ASK1, Ep1, 9)--c버튼 누르기
	LuaQuestClass:SetQuestRewardExp(Ep1, 9, 50)
	
	LuaQuestClass:SetTutorial(ASK1, Ep1, 15)--i버튼 누르기
	LuaQuestClass:SetQuestRewardExp(Ep1, 15, 50)
	LuaQuestClass:SetQuestRewardZen(Ep1, 15, 500)
	
	--LuaQuestClass:SetQuestGetItem(ASK1, Ep1, 20, 14, 0, 0, 0, 5)--사과 구해오기
	ItemType = 14 ItemIndex = 0 ItemLevel = 0 ItemSkill = 0 ItemCnt = 5 Opt = 0 ExOpt = 0
	LuaQuestClass:SetQuestGetItem(ASK1, Ep1, 20, ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt)--사과 구해오기
	LuaQuestClass:SetQuestRewardExp(Ep1, 20, 100)
	LuaQuestClass:SetQuestRewardZen(Ep1, 20, 1000)
--	LuaQuestClass:SetQuestRewardContribute(Ep1, 20, 11)
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep1, 23, 27, 3)--고리전갈3마리잡아라
	LuaQuestClass:SetQuestRewardExp(Ep1, 23, 250)
	LuaQuestClass:SetQuestRewardZen(Ep1, 23, 1000)
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep1, 24, 2, 3)--버지드래곤3마리
	LuaQuestClass:SetQuestRewardExp(Ep1, 24, 250)
	LuaQuestClass:SetQuestRewardZen(Ep1, 24, 1000)
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep1, 25, 418, 3)--기괴한토끼3마리
	LuaQuestClass:SetQuestRewardExp(Ep1, 25, 250)
	LuaQuestClass:SetQuestRewardZen(Ep1, 25, 1000)
	
	ItemType = 14 ItemIndex = 10 ItemLevel = 0 ItemSkill = 0 ItemCnt = 1 Opt = 0 ExOpt = 0
	LuaQuestClass:SetQuestGetItem(ASK1, Ep1, 29, ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt)--마을 귀환문서 1개
	LuaQuestClass:SetQuestRewardExp(Ep1, 29, 200)
	LuaQuestClass:SetQuestRewardZen(Ep1, 29, 3000)
	
	LuaQuestClass:SetQuestBuff(ASK1, Ep1, 31, 3)--공방 버프 받기.
	LuaQuestClass:SetQuestRewardExp(Ep1, 31, 100)
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep1, 33, 28, 30)--풍뎅이 괴물 30마리
	LuaQuestClass:SetQuestRewardExp(Ep1, 33, 5000)
	LuaQuestClass:SetQuestRewardZen(Ep1, 33, 3000)
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep1, 34, 1, 30)--하운드 30마리
	LuaQuestClass:SetQuestRewardExp(Ep1, 34, 5000)
	LuaQuestClass:SetQuestRewardZen(Ep1, 34, 3000)
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep1, 35, 421, 30)--오염된 나비 30마리
	LuaQuestClass:SetQuestRewardExp(Ep1, 35, 5000)
	LuaQuestClass:SetQuestRewardZen(Ep1, 35, 3000)

	--엑셀런트 옵션 줄때 가변인자로 사용합니다.
	ItemType = 4 ItemIndex = 8 ItemLevel = 3 dur = 0 ItemSkill = 1 ItemCnt = 1 --ExOpt = ExOpt1 + ExOpt2 + ExOpt3 + ExOpt4 + ExOpt5 + ExOpt6
	LuaQuestClass:SetQuestRewardItem(Ep1, 40, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--석궁+3
	LuaQuestClass:SetQuestRewardZen(Ep1, 40, 5000)

	ItemType = 12 ItemIndex = 11 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep1, 41, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--고블린 소환구슬
	
	ItemType = 1 ItemIndex = 2 ItemLevel = 3 dur = 0 ItemSkill = 1 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep1, 49, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--양날도끼+3
	
	LuaQuestClass:SetQuestRewardZen(Ep1, 54, 10000)
	ItemType = 15 ItemIndex = 3 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep1, 54, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--파이어법서
	
	LuaQuestClass:SetQuestRewardZen(Ep1, 69, 10000)
	ItemType = 15 ItemIndex = 3 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep1, 69, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--파이어법서
	
	LuaQuestClass:SetQuestSkillLearn(ASK1, Ep1, 42, 24)--다발 스킬
	LuaQuestClass:SetQuestRewardExp(Ep1, 42, 300)
	
	LuaQuestClass:SetQuestSkillLearn(ASK1, Ep1, 43, 30)--고블린 소환 스킬
	LuaQuestClass:SetQuestRewardExp(Ep1, 43, 300)
	
	LuaQuestClass:SetQuestSkillLearn(ASK1, Ep1, 50, 19)--내려찍기 스킬
	LuaQuestClass:SetQuestRewardExp(Ep1, 50, 300)
	
	LuaQuestClass:SetQuestSkillLearn(ASK1, Ep1, 55, 4)--파이어볼 스킬
	LuaQuestClass:SetQuestRewardExp(Ep1, 55, 300)
	
	LuaQuestClass:SetQuestLevelUp(ASK1, Ep1, 62, 25)--레벨 25 이상 올려와라.
	ItemType = 13 ItemIndex = 2 ItemLevel = 0 dur = 255 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep1, 62, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--유니리아
	
	ItemType = 14 ItemIndex = 3 ItemLevel = 0 dur = 3 ItemSkill = 0 ItemCnt = 5 Overlap = 1
	LuaQuestClass:SetQuestRewardItem(Ep1, 67, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt, Overlap)--큰 치료 물약 15개
	
	ItemType = 14 ItemIndex = 6 ItemLevel = 0 dur = 3 ItemSkill = 0 ItemCnt = 5 Overlap = 1
	LuaQuestClass:SetQuestRewardItem(Ep1, 71, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt, Overlap)--큰 마나 물약 15개
	
	ItemType = 13 ItemIndex = 0 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep1, 74, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--술 5개 대신 수호천사 1개
	
	LuaQuestClass:SetQuestRewardZen(Ep1, 77, 30000)	-- 마을 귀환 주문서 대신 젠 지급
	
	LuaQuestClass:SetQuestRewardExp(Ep1, 80, 3000)
	
	LuaQuestClass:SetQuestRewardZen(Ep1, 83, 10000)
	
	ItemType = 13 ItemIndex = 20 ItemLevel = 2 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep1, 90, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--전사의반지 1개
	ItemType = 13 ItemIndex = 20 ItemLevel = 1 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep1, 90, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--용사의 반지 1개
end
	
	