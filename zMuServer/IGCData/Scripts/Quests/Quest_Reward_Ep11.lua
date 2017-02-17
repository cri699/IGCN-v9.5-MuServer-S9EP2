--[[
Quest_Reward_Ep11.lua
[ Start  ] 	2011.07.18
[ Latest ]	2011-09-28 4:02 PM
[ Version ]	0.0.0.2
[ Text ]	거절 시 요구사항이 출력되는 현상 수정
[ Line ]	47 - 57
--]]
--================================================================================================================
function Quest_Reward_Window_Ep11(iObjIndex)

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
	local Overlap = 0 	-- 아이템이 겹쳐지는가?
	local Opt	= 0
	local ExOpt	= 0
	
---------------------------------------------------------------------------------------------------------------------

LuaQuestClass:SetQuestProgress(ASK1, Ep11, 2)

	ItemType = 13 ItemIndex = 150 ItemLevel = 0 ItemCnt = 1 							-- 붉은색 잉크
	LuaQuestClass:SetQuestDropItem(Ep11, 1, ASK1, 549, 100, ItemType, ItemIndex, ItemLevel, ItemCnt)		-- 블러디오크
	
	ItemType = 13 ItemIndex = 150 ItemLevel = 0 ItemSkill = 0 ItemCnt = 10 Opt = 0 ExOpt = 0
	LuaQuestClass:SetQuestGetItem(ASK1, Ep11, 1, ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt)	-- 붉은색 잉크 10개
	
	LuaQuestClass:SetQuestRewardZen(Ep11, 1, 1000000)

LuaQuestClass:SetQuestProgress(ASK1, Ep11, 5)	
	
	ItemType = 13 ItemIndex = 151 ItemLevel = 0 ItemCnt = 1 							-- 정신의 양피지
	LuaQuestClass:SetQuestDropItem(Ep11, 4, ASK1, 562, 100, ItemType, ItemIndex, ItemLevel, ItemCnt)		-- 다크매머드
	
	ItemType = 13 ItemIndex = 151 ItemLevel = 0 ItemSkill = 0 ItemCnt = 1 Opt = 0 ExOpt = 0
	LuaQuestClass:SetQuestGetItem(ASK1, Ep11, 4, ItemType, ItemIndex, ItemLevel, ItemSkill, ItemCnt, Opt, ExOpt)	-- 정신의 양피지
	
	ItemType = 13 ItemIndex = 158 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1 
	LuaQuestClass:SetQuestRewardItem(Ep11, 4, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)		-- 봉인된 붉은 망각의 두루마리
end
--================================================================================================================	
	