--[[
Quest_Main.lua
[ Start  ] 	2010.06.
[ Latest ]	2011-09-14 3:07 PM
[ Author ]	김정민
[ Version ]	0.0.0.2
[ Text ]	마스터 스킬 트리 초기화 퀘스트 추가								
[ Desc ]									
--]]
--소스에서 호출합니다. 함수명 변경 금지.
--================================================================================================================
function NpcTalkClick(NpcIndex, iObjIndex)
	local GensInfluence = LuaQuestClass:GetGensInfluence(iObjIndex)--겐스세력	
	--local LuaQuestClass = QuestExpLuaBind()	
	--local NpcIndex = g_QuestExpLuaBind:GetNpcIndex()
	--g_QuestExpLuaBind:SetiObjIndex(iObjIndex)
	-------------
	----테스트 코드
	--g_QuestExpLuaBind:AddQuestExpNpcTalk(Episode_1, 1)-- NPC한테 보내줄 퀘스트
	--g_QuestExpLuaBind:SendQuestExpNpcTalk()
	--local iObjIndex = g_QuestExpLuaBind:GetiObjIndex()
	--g_QuestExpLuaBind:SendQuestToNPC(Episode_1, 1, iObjIndex)-- NPC한테 보내줄 퀘스트
	--------------
	
	--npc클릭하고 퀘 npc 이면 
	if NpcIndex == NPC_SHADOW_PHANTOM then
		return NpcTalk_ShadowPhantom(iObjIndex)
		
		--return AcceptQuestEp1(QuestSwitch)
	elseif NpcIndex == NPC_GENS_DUPRIAN then
		if GensInfluence == GENS_DUPRIAN then	
			return NpcTalk_GENS(iObjIndex)
		end
		return NpcTalk_GENS_Null(iObjIndex)
		
	elseif NpcIndex == NPC_GENS_VANERT then
		if GensInfluence == GENS_VANERT then	
			return NpcTalk_GENS(iObjIndex)
		end
		return NpcTalk_GENS_Null(iObjIndex)
		
	elseif NpcIndex == NPC_TERSIA then
		return NpcTalk_Tersia(iObjIndex)
		
	elseif NpcIndex == NPC_VAINA then
		return NpcTalk_Vaina(iObjIndex)
		
	elseif NpcIndex == NPC_ZAIRO then
		return NpcTalk_Zairo(iObjIndex)
		
	elseif NpcIndex == NPC_DERUBISH then
		return NpcTalk_Derubish(iObjIndex)
	end 
end
--================================================================================================================
function NpcTalk_ShadowPhantom(iObjIndex)			-- 처음 대사창 처리 0x0A

	-- local LuaQuestClass = QuestExpLuaBind()
	-- LuaQuestClass:AddQuestExpNpcTalk(0, 1)-- 버프

	local ProgEp = 0

	local QS = LuaQuestClass:GetQuestSwitch(Ep1, iObjIndex)
	local CharClass = LuaQuestClass:GetCharClass(iObjIndex)

	if QS > 4 then
		LuaQuestClass:AddQuestExpNpcTalk(Ep1, QS)	-- NPC한테 보내줄 퀘스트
	end
	
	local UserLv = LuaQuestClass:GetUserLv(iObjIndex)

-- 쉐도우 팬텀 퀘스트 진행

	Ep2_NPCTalkList(iObjIndex)				-- 카오스 캐슬   퀘스트
	Ep3_NPCTalkList(iObjIndex)				-- 블러드 캐슬   퀘스트
	Ep4_NPCTalkList(iObjIndex)				-- 악마의 광장   퀘스트
	Ep5_NPCTalkList(iObjIndex)				-- 환영사원 입장 퀘스트
	Ep6_NPCTalkList(iObjIndex)				-- 일주          퀘스트
	
	if QuestResetMasterSkill == 1 then
		Ep9_NPCTalkList(iObjIndex)				-- 마스터 스킬 트리 초기화 퀘스트(첫번째)
		Ep10_NPCTalkList(iObjIndex)				-- 마스터 스킬 트리 초기화 퀘스트(두번째)
		Ep11_NPCTalkList(iObjIndex)				-- 마스터 스킬 트리 초기화 퀘스트(세번째)
		Ep12_NPCTalkList(iObjIndex)				-- 마스터 스킬 트리 초기화 퀘스트(마지막)
	end
	
	if CharClass == CLASS_KNIGHT or CharClass == CLASS_WIZARD or CharClass == CLASS_ELF or CharClass == CLASS_SUMMONER then
		Ep15_NPCTalkList(iObjIndex)			-- 1차 캐릭터만 진행하는 아이템 지급 퀘스트
	end
	
	Ep18_NPCTalkList(iObjIndex)				-- 반복 퀘스트(1) 1   ~ 259레벨
	Ep19_NPCTalkList(iObjIndex)				-- 반복 퀘스트(2) 260 ~ 349레벨
	
	LuaQuestClass:SendQuestSwitchList(iObjIndex)		-- NPC 한테 보낸다.
end
--================================================================================================================
function NpcTalk_GENS(iObjIndex)

	Ep16_NPCTalkList(iObjIndex)
	Ep17_NPCTalkList(iObjIndex)
	Ep20_NPC_Gen_TalkList(iObjIndex)		-- 1차 일일 퀘스트(월?, 월?) 정보 전달
	
	LuaQuestClass:SendQuestSwitchList(iObjIndex)	-- npc 한테 보낸다.
	
end
--================================================================================================================
-- 아무것도 없는 퀘스트를 줄경우
-- NPC Talk는 꼭 주어야 한다.
function NpcTalk_GENS_Null(iObjIndex)

	LuaQuestClass:SendQuestSwitchList(iObjIndex)	--npc 한테 보낸다.
	
end
--================================================================================================================
function NpcTalk_Tersia(iObjIndex)

	Ep20_NPC_Tersia_TalkList(iObjIndex)		-- 1차 일일 퀘스트(임무수령) 정보 전달
	
	LuaQuestClass:SendQuestSwitchList(iObjIndex)	--npc 한테 보낸다.
	
end
--================================================================================================================
function NpcTalk_Vaina(iObjIndex)

	Ep20_NPC_Vaina_TalkList(iObjIndex)
	
	LuaQuestClass:SendQuestSwitchList(iObjIndex)	--npc 한테 보낸다.
	
end
--================================================================================================================
function NpcTalk_Zairo(iObjIndex)

	Ep7_NPCTalkList(iObjIndex)
	Ep20_NPC_Zairo_TalkList(iObjIndex)
	
	LuaQuestClass:SendQuestSwitchList(iObjIndex)	--npc 한테 보낸다.
	
end
--================================================================================================================
function NpcTalk_Derubish(iObjIndex)

	Ep8_NPC_Derubish_TalkList(iObjIndex)		-- 아케론 퀘스트 정보 전달
	Ep20_NPC_Derubish_TalkList(iObjIndex)		-- 2차 일일 퀘스트(목, 월?) 정보 전달
	LuaQuestClass:SendQuestSwitchList(iObjIndex)	--npc 한테 보낸다.
	
end
--================================================================================================================