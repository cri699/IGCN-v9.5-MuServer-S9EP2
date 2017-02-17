--[[
Quest_Reward_Ep1.lua
[ Start  ] 	2009.08.
[ Latest ]	2009-12-09 16:45 PM
[ Version ]	0.0.0.1
[ Text ]									
[ Desc ]									
--]]

function Quest_Reward_Window_Ep16(iObjIndex)

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

-- 겐스퀘스트(1)(150~219레벨)
	LuaQuestClass:SetQuestProgress(ASK1, Ep16, 1)--대화 진행상 완료창이 필요한 경우 사용
	

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep16, 2, 46, 50)--베파르
	LuaQuestClass:SetQuestRewardExp(Ep16, 2, 100000)
	ItemType = 14 ItemIndex = 35 ItemLevel = 0 dur = 1 ItemSkill = 0 ItemCnt = 3 
	LuaQuestClass:SetQuestRewardItem(Ep16, 2, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--작은SD물약
	LuaQuestClass:SetQuestRewardContribute(Ep16, 2, 5)	

-- 겐스퀘스트(2)(220~249레벨)
	LuaQuestClass:SetQuestProgress(ASK1, Ep16, 4)--대화 진행상 완료창이 필요한 경우 사용
	

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep16, 5, 308, 50)--데스트리
	LuaQuestClass:SetQuestRewardExp(Ep16, 5, 200000)
	ItemType = 14 ItemIndex = 35 ItemLevel = 0 dur = 1 ItemSkill = 0 ItemCnt = 3 
	LuaQuestClass:SetQuestRewardItem(Ep16, 5, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--작은SD물약	
	LuaQuestClass:SetQuestRewardContribute(Ep16, 5, 5)
-- 겐스퀘스트(3)(250~299레벨)
	LuaQuestClass:SetQuestProgress(ASK1, Ep16, 7)--대화 진행상 완료창이 필요한 경우 사용
	

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep16, 8, 351, 50)--스프린터 울프
	LuaQuestClass:SetQuestRewardExp(Ep16, 8, 300000)
	ItemType = 14 ItemIndex = 36 ItemLevel = 0 dur = 1 ItemSkill = 0 ItemCnt = 3
	LuaQuestClass:SetQuestRewardItem(Ep16, 8, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--중간SD물약	
	LuaQuestClass:SetQuestRewardContribute(Ep16, 8, 5)
-- 겐스퀘스트(4)(300~379레벨)
	LuaQuestClass:SetQuestProgress(ASK1, Ep16, 10)--대화 진행상 완료창이 필요한 경우 사용
	

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep16, 11, 486, 50)--피의암살자
	LuaQuestClass:SetQuestRewardExp(Ep16, 11, 400000)
	ItemType = 14 ItemIndex = 37 ItemLevel = 0 dur = 1 ItemSkill = 0 ItemCnt = 3
	LuaQuestClass:SetQuestRewardItem(Ep16, 11, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--큰SD물약	
	LuaQuestClass:SetQuestRewardContribute(Ep16, 11, 5)
-- 겐스퀘스트(5)(3800~레벨)
	LuaQuestClass:SetQuestProgress(ASK1, Ep16, 13)--대화 진행상 완료창이 필요한 경우 사용
	

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep16, 14, 442, 50)--사피 두오
	LuaQuestClass:SetQuestRewardExp(Ep16, 14, 450000)
	ItemType = 14 ItemIndex = 37 ItemLevel = 0 dur = 1 ItemSkill = 0 ItemCnt = 3
	LuaQuestClass:SetQuestRewardItem(Ep16, 14, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--큰 SD물약		
	LuaQuestClass:SetQuestRewardContribute(Ep16, 14, 5)
---------------------------------------------------------------------------------------------------------------------

end
	
	