--[[
Quest_Reward_Ep7.lua
[ Start  ] 	2009.08.
[ Latest ]	2009-12-14 16:45 PM
[ Version ]	0.0.0.1
[ Text ]									
[ Desc ]									
--]]

function Quest_Reward_Window_Ep7(iObjIndex)
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
-- 인벤 확장 퀘스트
---------------------------------------------------------------------------------------------------------------------

-- 창고 확장
	LuaQuestClass:SetQuestProgress(ASK1, Ep7, 3)--대화 진행상 완료창이 필요한 경우 사용
	
	ItemType = 14 ItemIndex = 164 ItemLevel = 0 ItemCnt = 1 --고대의 조각상
	LuaQuestClass:SetQuestDropItem(Ep7, 4, ASK1, 40, 5, ItemType, ItemIndex, ItemLevel, ItemCnt)--데쓰나이트
	
	ItemType = 14 ItemIndex = 164 ItemLevel = 0 ItemSkill = 0 ItemCnt = 1 Opt = 0 ExOpt = 0
	LuaQuestClass:SetQuestGetItem(ASK1, Ep7, 4, ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt)--고대의 조각상 체크

	ItemType = 14 ItemIndex = 163 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1 
	LuaQuestClass:SetQuestRewardItem(Ep7, 4, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--창고확장증서

-- 1차 인벤토리
	LuaQuestClass:SetQuestProgress(ASK1, Ep7, 8)--대화 진행상 완료창이 필요한 경우 사용
	
	ItemType = 14 ItemIndex = 165 ItemLevel = 0 ItemCnt = 1 --마법의 옷감
	LuaQuestClass:SetQuestDropItem(Ep7, 9, ASK1, 52, 5, ItemType, ItemIndex, ItemLevel, ItemCnt)--실버발키리
	
	ItemType = 14 ItemIndex = 165 ItemLevel = 0 ItemSkill = 0 ItemCnt = 1 Opt = 0 ExOpt = 0
	LuaQuestClass:SetQuestGetItem(ASK1, Ep7, 9, ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt)--마법의 옷감 체크

	ItemType = 14 ItemIndex = 162 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1 
	LuaQuestClass:SetQuestRewardItem(Ep7, 9, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--마법의배낭
	
-- 2차 인벤토리
	LuaQuestClass:SetQuestProgress(ASK1, Ep7, 13)--대화 진행상 완료창이 필요한 경우 사용
	
	ItemType = 14 ItemIndex = 166 ItemLevel = 0 ItemCnt = 1 --공간의 옷감
	LuaQuestClass:SetQuestDropItem(Ep7, 14, ASK1, 69, 5, ItemType, ItemIndex, ItemLevel, ItemCnt)--알카모스
	
	ItemType = 14 ItemIndex = 166 ItemLevel = 0 ItemSkill = 0 ItemCnt = 1 Opt = 0 ExOpt = 0
	LuaQuestClass:SetQuestGetItem(ASK1, Ep7, 14, ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt)--공간의 옷감 체크

	ItemType = 14 ItemIndex = 162 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1 
	LuaQuestClass:SetQuestRewardItem(Ep7, 14, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--마법의 배낭

-- 창고 확장
	LuaQuestClass:SetQuestProgress(ASK1, Ep7, 18)--대화 진행상 완료창이 필요한 경우 사용
	
	ItemType = 14 ItemIndex = 164 ItemLevel = 0 ItemCnt = 1 --고대의 조각상
	LuaQuestClass:SetQuestDropItem(Ep7, 19, ASK1, 61, 5, ItemType, ItemIndex, ItemLevel, ItemCnt)--빔나이트
	
	ItemType = 14 ItemIndex = 164 ItemLevel = 0 ItemSkill = 0 ItemCnt = 1 Opt = 0 ExOpt = 0
	LuaQuestClass:SetQuestGetItem(ASK1, Ep7, 19, ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt)--고대의 조각상 체크

	ItemType = 14 ItemIndex = 163 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1 
	LuaQuestClass:SetQuestRewardItem(Ep7, 19, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--창고확장증서	
end


	
	