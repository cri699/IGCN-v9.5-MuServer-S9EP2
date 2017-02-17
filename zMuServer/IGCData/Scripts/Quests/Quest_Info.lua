-- // ============================================================
-- // == INTERNATIONAL GAMING CENTER NETWORK
-- // == www.igc-network.com
-- // == (C) 2010-2015 IGC-Network (R)
-- // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
-- // == File is a part of IGCN Group MuOnline Server files.
-- // ============================================================

QuestInclude("Quest_Define.lua")
QuestInclude("Quest_Reward_Ep1.lua")
QuestInclude("Quest_Reward_Ep2.lua")
QuestInclude("Quest_Reward_Ep3.lua")
QuestInclude("Quest_Reward_Ep4.lua")
QuestInclude("Quest_Reward_Ep5.lua")
QuestInclude("Quest_Reward_Ep6.lua")
QuestInclude("Quest_Reward_Ep7.lua")
QuestInclude("Quest_Reward_Ep8.lua")
QuestInclude("Quest_Reward_Ep9.lua")
QuestInclude("Quest_Reward_Ep10.lua")
QuestInclude("Quest_Reward_Ep11.lua")
QuestInclude("Quest_Reward_Ep12.lua")
QuestInclude("Quest_Reward_Ep15.lua")
QuestInclude("Quest_Reward_Ep16.lua")
QuestInclude("Quest_Reward_Ep17.lua")
QuestInclude("Quest_Reward_Ep18.lua")
QuestInclude("Quest_Reward_Ep19.lua")
QuestInclude("Quest_Reward_Ep20.lua")
--================================================================================================================
function SetQuestItemInfo()
--[[
EN_OVERLAP		=	0x0001 -- 겹치기
EN_DROP			=	0x0002 -- 아이템 바닥에 버리기
EN_WAREHOUSE		=	0x0004 -- 창고 이동
EN_SELL_SHOP		=	0x0008 -- 상점 판매
EN_PERSONAL_SHOP	=	0x0010 -- 개인 상점
EN_PERSONAL_TRADE	=	0x0020 -- 개인 거래
EN_QUEST_PROG		=	0x0040 -- 아이템 사용해서 퀘스트 진행 
]]
	-- LuaQuestClass:SetQuestUseItemList(ItemType, ItemIndex, QuestItemAttribute, Ep)
	
	local QuestItemAtt = 0
	
	ItemType = 13 ItemIndex = 148 QuestItemAtt = EN_SELL_SHOP + EN_OVERLAP	-- 초록색 잉크
	LuaQuestClass:SetQuestItemInfo(ItemType, ItemIndex, QuestItemAtt, Ep9)
	
	ItemType = 13 ItemIndex = 149 QuestItemAtt = EN_SELL_SHOP + EN_OVERLAP 	-- 파란색 잉크
	LuaQuestClass:SetQuestItemInfo(ItemType, ItemIndex, QuestItemAtt, Ep10)
	
	ItemType = 13 ItemIndex = 150 QuestItemAtt = EN_SELL_SHOP + EN_OVERLAP	-- 붉은색 잉크
	LuaQuestClass:SetQuestItemInfo(ItemType, ItemIndex, QuestItemAtt, Ep11)
	
	ItemType = 13 ItemIndex = 151 QuestItemAtt = EN_SELL_SHOP + EN_OVERLAP	-- 정신의 양피지
	LuaQuestClass:SetQuestItemInfo(ItemType, ItemIndex, QuestItemAtt, Ep11)
	
	ItemType = 14 ItemIndex = 151 QuestItemAtt = EN_QUEST_PROG 		-- 1레벨 의뢰서
	LuaQuestClass:SetQuestItemInfo(ItemType, ItemIndex, QuestItemAtt, Ep20)
	
	ItemType = 14 ItemIndex = 152 QuestItemAtt = EN_SELL_SHOP + EN_OVERLAP 	-- 1레벨 의뢰 완료 확인
	LuaQuestClass:SetQuestItemInfo(ItemType, ItemIndex, QuestItemAtt)

	ItemType = 14 ItemIndex = 170 QuestItemAtt = EN_QUEST_PROG 		-- 2레벨 의뢰서
	LuaQuestClass:SetQuestItemInfo(ItemType, ItemIndex, QuestItemAtt, Ep20)
	
	ItemType = 14 ItemIndex = 171 QuestItemAtt = EN_SELL_SHOP + EN_OVERLAP 	-- 2레벨 의뢰 완료 확인
	LuaQuestClass:SetQuestItemInfo(ItemType, ItemIndex, QuestItemAtt)
	
	ItemType = 14 ItemIndex = 153 QuestItemAtt = EN_SELL_SHOP + EN_OVERLAP 	-- 스타더스트
	LuaQuestClass:SetQuestItemInfo(ItemType, ItemIndex, QuestItemAtt)
	
	ItemType = 14 ItemIndex = 154 QuestItemAtt = EN_SELL_SHOP + EN_OVERLAP 	-- 칼트석
	LuaQuestClass:SetQuestItemInfo(ItemType, ItemIndex, QuestItemAtt)
	
	ItemType = 14 ItemIndex = 155 QuestItemAtt = EN_SELL_SHOP + EN_OVERLAP 	-- 탄탈로스의 갑옷
	LuaQuestClass:SetQuestItemInfo(ItemType, ItemIndex, QuestItemAtt)
	
	ItemType = 14 ItemIndex = 156 QuestItemAtt = EN_SELL_SHOP + EN_OVERLAP 	-- 잿더미 도살자의 몽둥이
	LuaQuestClass:SetQuestItemInfo(ItemType, ItemIndex, QuestItemAtt)

	ItemType = 14 ItemIndex = 164 QuestItemAtt = EN_SELL_SHOP 		-- 고대의 조각상
	LuaQuestClass:SetQuestItemInfo(ItemType, ItemIndex, QuestItemAtt)	

	ItemType = 14 ItemIndex = 165 QuestItemAtt = EN_SELL_SHOP 		-- 마법의 옷감
	LuaQuestClass:SetQuestItemInfo(ItemType, ItemIndex, QuestItemAtt)	

	ItemType = 14 ItemIndex = 166 QuestItemAtt = EN_SELL_SHOP 		-- 공간의 옷감
	LuaQuestClass:SetQuestItemInfo(ItemType, ItemIndex, QuestItemAtt)	
end
--================================================================================================================
function SetQuestNPCTeleportPos()

	local iDir	=	0
	local iPosX	=	0
	local iPosY	=	0
	local iIndex 	=	0
	local iMapNum	=	0
	
	iIndex = 0 iMapNum = 51 iPosX = 55 iPosY = 199 iDir = 2
	LuaQuestClass:SetQuestNPCTeleportPosInfo(iIndex, iMapNum, iPosX, iPosY, iDir)
	
	iIndex = 1 iMapNum = 0 iPosX = 131 iPosY = 138 iDir = 2
	LuaQuestClass:SetQuestNPCTeleportPosInfo(iIndex, iMapNum, iPosX, iPosY, iDir)
	
	iIndex = 2 iMapNum = 3 iPosX = 186 iPosY = 109 iDir = 2
	LuaQuestClass:SetQuestNPCTeleportPosInfo(iIndex, iMapNum, iPosX, iPosY, iDir)
	
	iIndex = 3 iMapNum = 2 iPosX = 226 iPosY = 52 iDir = 2
	LuaQuestClass:SetQuestNPCTeleportPosInfo(iIndex, iMapNum, iPosX, iPosY, iDir)
end
--================================================================================================================
function SetQuestInfo()

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

	--LuaQuestClass:SetQuestRewardRandom(Ep1, QS, 램덤지급갯수, RewardIndex1,Rate1 RewardIndex2,Rate2, RewardIndex3,Rate3)
	--LuaQuestClass:SetQuestRewardRandom(Ep1, 20, 2, 1, 1000, 2, 2000, 3, 3000)
	--LuaQuestClass:SetQuestRewardExp(Ep1, 20, 1)
	--LuaQuestClass:SetQuestRewardZen(Ep1, 20, 2)
	--LuaQuestClass:SetQuestRewardZen(Ep1, 20, 300)
	--LuaQuestClass:SetQuestRewardZen(Ep1, 20, 4)

	--LuaQuestClass:SetQuestEventMapClear(ASK1, Ep1, 20, QUEST_REQUEST_EVENT_MAP_CLEAR_BLOOD, 블러드캐슬레벨) -- 블러드캐슬 클리어
	--LuaQuestClass:SetQuestEventMapClear(ASK1, Ep1, 20, QUEST_REQUEST_EVENT_MAP_CLEAR_BLOOD, 1)

	-- 악마의 광장에서 포인트 획득
	--LuaQuestClass:SetQuestEventMapDevilPoint(ASK1, Ep1, 20, 이벤트맵 레벨,획득해야할 포인트)
	--LuaQuestClass:SetQuestEventMapDevilPoint(ASK1, Ep1, 20, 1, 10)

	-- 몬스터 킬 포인트, 유저 킬 포인트, 성문 파괴
	--LuaQuestClass:SetQuestEventMapKillPoint(ASK1, Ep1, 20, 퀘스트 타입, 이벤트맵 레벨, 킬카운트)
	--LuaQuestClass:SetQuestEventMapKillPoint(ASK1, Ep2, 10, QUEST_REQUEST_EVENT_MAP_USER_KILL, 1, 1)	
	
	-- LuaQuestClass:SetQuestMonsterKill(ASK1, Ep2, 11, 12, 40, 가변 인자)--유충
	-- 0 : 개인, 1: 파티
	--LuaQuestClass:SetQuestMonsterKill(ASK1, Ep2, 11, 12, 40, 1이면 파티)--유충

	--********************************************--
	SetQuestItemInfo()
	SetQuestNPCTeleportPos()

-- Ep1 보상 내역 정리 함수
	Quest_Reward_Window_Ep1(iObjIndex)

-- Ep2 보상 내역 정리 함수
	Quest_Reward_Window_Ep2(iObjIndex)

-- Ep3 보상 내역 정리 함수
	Quest_Reward_Window_Ep3(iObjIndex)

-- Ep4 보상 내역 정리 함수
	Quest_Reward_Window_Ep4(iObjIndex)

-- Ep5 보상 내역 정리 함수
	Quest_Reward_Window_Ep5(iObjIndex)
	
-- Ep6 보상 내역 정리 함수
	Quest_Reward_Window_Ep6(iObjIndex)

-- Ep7 보상 내역 정리 함수
	Quest_Reward_Window_Ep7(iObjIndex)

-- Ep8 보상 내역 정리 함수
	Quest_Reward_Window_Ep8(iObjIndex)
	
-- Ep9 보상 내역 정리 함수
	Quest_Reward_Window_Ep9(iObjIndex)
	
-- Ep10 보상 내역 정리 함수
	Quest_Reward_Window_Ep10(iObjIndex)
	
-- Ep11 보상 내역 정리 함수
	Quest_Reward_Window_Ep11(iObjIndex)
	
-- Ep12 보상 내역 정리 함수
	Quest_Reward_Window_Ep12(iObjIndex)
	
-- Ep15 보상 내역 정리 함수
	Quest_Reward_Window_Ep15(iObjIndex)	

-- Ep16 보상 내역 정리 함수
	Quest_Reward_Window_Ep16(iObjIndex)

-- Ep17 보상 내역 정리 함수
	Quest_Reward_Window_Ep17(iObjIndex)
	
-- Ep18 보상 내역 정리 함수
	Quest_Reward_Window_Ep18(iObjIndex)

-- Ep19 보상 내역 정리 함수
	Quest_Reward_Window_Ep19(iObjIndex)
	
-- Ep20 보상 내역 정리 함수
	Quest_Reward_Window_Ep20(iObjIndex)
	LuaQuestClass:temp()
end
--================================================================================================================