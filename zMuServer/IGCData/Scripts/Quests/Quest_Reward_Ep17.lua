--[[
Quest_Reward_Ep17.lua
[ Start  ] 	2009.08.
[ Latest ]	2009-12-09 16:45 PM
[ Version ]	0.0.0.1
[ Text ]									
[ Desc ]									
--]]

function Quest_Reward_Window_Ep17(iObjIndex)

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
	
-- 겐스퀘스트(1)(150~219레벨)
	LuaQuestClass:SetQuestProgress(ASK1, Ep17, 1)--대화 진행상 완료창이 필요한 경우 사용
	

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep17, 2, 52, 50)--실버 발키리
	LuaQuestClass:SetQuestRewardExp(Ep17, 2, 150000)
	ItemType = 14 ItemIndex = 38 ItemLevel = 0 dur = 3 ItemSkill = 0 ItemCnt = 1 Overlap = 1
	LuaQuestClass:SetQuestRewardItem(Ep17, 2, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt, Overlap)--작은SD물약
	LuaQuestClass:SetQuestRewardContribute(Ep17, 2, 5)	

-- 겐스퀘스트(2)(220~249레벨)
	LuaQuestClass:SetQuestProgress(ASK1, Ep17, 4)--대화 진행상 완료창이 필요한 경우 사용
	

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep17, 5, 306, 50)--데스라이더
	LuaQuestClass:SetQuestRewardExp(Ep17, 5, 250000)
	ItemType = 14 ItemIndex = 38 ItemLevel = 0 dur = 3 ItemSkill = 0 ItemCnt = 1 Overlap = 1
	LuaQuestClass:SetQuestRewardItem(Ep17, 5, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt, Overlap)--작은SD물약	
	LuaQuestClass:SetQuestRewardContribute(Ep17, 5, 5)
-- 겐스퀘스트(3)(250~299레벨)
	LuaQuestClass:SetQuestProgress(ASK1, Ep17, 7)--대화 진행상 완료창이 필요한 경우 사용
	

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep17, 8, 352, 50)--아이언 라이더
	LuaQuestClass:SetQuestRewardExp(Ep17, 8, 350000)
	ItemType = 14 ItemIndex = 39 ItemLevel = 0 dur = 3 ItemSkill = 0 ItemCnt = 1 Overlap = 1
	LuaQuestClass:SetQuestRewardItem(Ep17, 8, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt, Overlap)--중간SD물약	
	LuaQuestClass:SetQuestRewardContribute(Ep17, 8, 5)
-- 겐스퀘스트(4)(300~379레벨)
	LuaQuestClass:SetQuestProgress(ASK1, Ep17, 10)--대화 진행상 완료창이 필요한 경우 사용
	

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep17, 11, 489, 50)--불타는 용암 거인
	LuaQuestClass:SetQuestRewardExp(Ep17, 11, 450000)
	ItemType = 14 ItemIndex = 40 ItemLevel = 0 dur = 3 ItemSkill = 0 ItemCnt = 1 Overlap = 1
	LuaQuestClass:SetQuestRewardItem(Ep17, 11, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt, Overlap)--큰SD물약	
	LuaQuestClass:SetQuestRewardContribute(Ep17, 11, 5)
-- 겐스퀘스트(5)(3800~레벨)
	LuaQuestClass:SetQuestProgress(ASK1, Ep17, 13)--대화 진행상 완료창이 필요한 경우 사용
	

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep17, 14, 448, 50)--고스트 네이핀
	LuaQuestClass:SetQuestRewardExp(Ep17, 14, 500000)
	ItemType = 14 ItemIndex = 40 ItemLevel = 0 dur = 3 ItemSkill = 0 ItemCnt = 1 Overlap = 1
	LuaQuestClass:SetQuestRewardItem(Ep17, 14, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt, Overlap)--큰 SD물약		
	LuaQuestClass:SetQuestRewardContribute(Ep17, 14, 5)

end
	
	