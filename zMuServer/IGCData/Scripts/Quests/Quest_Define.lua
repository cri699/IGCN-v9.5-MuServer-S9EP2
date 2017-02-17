--[[
Quest_Define.lua
[ Start  ] 	2009.08.
[ Latest ]	2009-12-08 10:45 AM
[ Author ]	이준희
[ Version ]	0.0.0.5
[ Text ]									
[ Desc ]									
--]]

--루아 바인딩.
LuaQuestClass = QuestExpLuaBind()

--겐스 세력 
GENS_DUPRIAN	=	1	--듀프리언
GENS_VANERT	=	2	--바네르트

--유저 클래스
CLASS_WIZARD                                          = 0
CLASS_KNIGHT                                          = 1
CLASS_ELF                                             = 2
CLASS_MAGUMSA                                         = 3
CLASS_DARKLORD                                        = 4
CLASS_SUMMONER                                        = 5
CLASS_MONK                                            = 6

-- 조건 체크 순서 체크
ASK1 = 1
ASK2 = 2
ASK3 = 3
ASK4 = 4
-- 에피소드 번호 정의
Ep1		=	1
Ep2		=	2
Ep3		=	3
Ep4		=	4
Ep5		=	5
Ep6		=	6
Ep7		=	7
Ep8		=	8
Ep9		=	9
Ep10		=	10
Ep11		=	11
Ep12		=	12
Ep13		=	13
Ep14		=	14
Ep15		=	15
Ep16		=	16
Ep17		=	17
Ep18		=	18
Ep19		=	19
Ep20		=	20

--퀘스트 NPC
NPC_SHADOW_PHANTOM	=	257		-- 쉐도우 팬텀
NPC_GENS_DUPRIAN     	=	543		-- 겐스 듀프리언
NPC_GENS_VANERT	        =	544		-- 겐스 바네르트
NPC_TERSIA	        =	566		-- 용병관리인 테르시아
NPC_VAINA	        =	567		-- 신녀 베이나
NPC_ZAIRO	        =	568		-- 떠돌이 상인 자이로
NPC_DERUBISH		=	581

-- 엑셀런트 옵션 추가 정의
ExOpt1	=	0x20
ExOpt2	=	0x10
ExOpt3	=	0x08
ExOpt4	=	0x04
ExOpt5	=	0x02
ExOpt6	=	0x01

-- 옵션 추가 정의
Opt4	=	1
Opt8	=	2
Opt12	=	3
Opt16	=	4

-- 파티 여부
PartyPlay = 1

-- 신규 조건 내용
	-- 포인트 획득 조건
	EVENT_MAP_USER_KILL			= 0x0040	-- 유저 킬 포인트
	EVENT_MAP_MON_KILL			= 0x0041	-- 몬스터 킬 포인트
	EVENT_MAP_BLOOD_GATE			= 0x0042	-- 블러드 캐슬 성문 파괴
	EVENT_MAP_DEVIL_POINT			= 0x0104	-- 악마의 광장 포인트 획득
	-- 클리어 조건
	EVENT_MAP_CLEAR_BLOOD			= 0x0100	-- 블러드 캐슬 클리어
	EVENT_MAP_CLEAR_CHAOS			= 0x0101	-- 카오스 캐슬 클리어
	EVENT_MAP_CLEAR_DEVIL			= 0x0102	-- 악마의 광장 클리어
	EVENT_MAP_CLEAR_ILLUSION		= 0x0103	-- 환영 사원 클리어

-- Drop Item 속성 관련.
EN_OVERLAP		=	0x0001 -- 겹치기
EN_DROP			=	0x0002 -- 아이템 바닥에 버리기
EN_WAREHOUSE		=	0x0004 -- 창고 이동
EN_SELL_SHOP		=	0x0008 -- 상점 판매
EN_PERSONAL_SHOP	=	0x0010 -- 개인 상점
EN_PERSONAL_TRADE	=	0x0020 -- 개인 거래
EN_QUEST_PROG		=	0x0040 -- 아이템 사용해서 퀘스트 진행 


-- 요일별 퀘스트
Sun	=	0
Mon	=	1
Tue	=	2
Wed	=	3
Thu	=	4
Fri	=	5
Sat	=	6

-- PVP 퀘스트 조건
VULCANUS = 63

-- Quests Configs
QuestResetMasterSkill 	= 1
QuestGensMuBlue			= 1
QuestExtendStorage 		= 1