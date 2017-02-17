--[[
Quest_Reward_Ep15.lua
[ Start  ] 	2009.08.
[ Latest ]	2009-12-09 16:45 PM
[ Version ]	0.0.0.1
[ Text ]									
[ Desc ]									
--]]

function Quest_Reward_Window_Ep15(iObjIndex)

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
-- 임무의 시작(80~90레벨)
	LuaQuestClass:SetQuestProgress(ASK1, Ep15, 2)--대화 진행상 완료창이 필요한 경우 사용

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep15, 3, 20, 50)--설인 대장
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep15, 3, 423, 50)--토템 골램
	LuaQuestClass:SetQuestRewardExp(Ep15, 3, 60000)
	LuaQuestClass:SetQuestRewardZen(Ep15, 3, 1500000)
	ItemType = 12 ItemIndex = 15 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep15, 3, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--혼석
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep15, 4, 19, 50)--설인
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep15, 4, 422, 50)--저주 받은 리치
	LuaQuestClass:SetQuestRewardExp(Ep15, 4, 60000)
	LuaQuestClass:SetQuestRewardZen(Ep15, 4, 1500000)
	ItemType = 12 ItemIndex = 15 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep15, 4, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--혼석

-- 던전 정복(1)(91~100레벨)	
	LuaQuestClass:SetQuestProgress(ASK1, Ep15, 10)--대화 진행상 완료창이 필요한 경우 사용
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep15, 6, 15, 50)--해골궁수
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep15, 6, 5, 30)--헬하운드
	ItemType = 10 ItemIndex = 1 ItemLevel = 8 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep15, 6, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--드라곤 장갑
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep15, 7, 17, 50)--싸이클롭스
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep15, 7, 11, 30)--고스트
	ItemType = 10 ItemIndex = 3 ItemLevel = 8 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep15, 7, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--전설의 장갑
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep15, 8, 15, 50)--해골궁수
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep15, 8, 5, 30)--헬하운드
	ItemType = 10 ItemIndex = 14 ItemLevel = 8 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep15, 8, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--수호 장갑
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep15, 9, 17, 50)--싸이클롭스
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep15, 9, 11, 30)--고스트
	ItemType = 10 ItemIndex = 40 ItemLevel = 8 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep15, 9, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--레드윙 장갑
	
-- 던전 정복(2)(101~110레벨)
	LuaQuestClass:SetQuestProgress(ASK1, Ep15, 16)--대화 진행상 완료창이 필요한 경우 사용
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep15, 12, 5, 50)--헬하운드
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep15, 12, 13, 30)--헬스파이더
	ItemType = 11 ItemIndex = 1 ItemLevel = 8 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep15, 12, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--드라곤 부츠
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep15, 13, 15, 50)--해골궁수
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep15, 13, 5, 30)--헬하운드
	ItemType = 11 ItemIndex = 3 ItemLevel = 8 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep15, 13, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--전설의 부츠
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep15, 14, 5, 50)--헬하운드
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep15, 14, 13, 30)--헬스파이더
	ItemType = 11 ItemIndex = 14 ItemLevel = 8 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep15, 14, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--수호 부츠
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep15, 15, 15, 50)--해골궁수
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep15, 15, 5, 30)--헬하운드
	ItemType = 11 ItemIndex = 40 ItemLevel = 8 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep15, 15, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--레드윙 부츠


-- 던전 정복(3)(111~120레벨)
	LuaQuestClass:SetQuestProgress(ASK1, Ep15, 22)--대화 진행상 완료창이 필요한 경우 사용
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep15, 18, 16, 50)--해골전사대장
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep15, 18, 9, 30)--선더리치
	ItemType = 7 ItemIndex = 1 ItemLevel = 8 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep15, 18, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--드라곤 투구
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep15, 19, 5, 50)--헬하운드
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep15, 19, 13, 30)--헬스파이더
	ItemType = 7 ItemIndex = 3 ItemLevel = 8 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep15, 19, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--전설의 투구
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep15, 20, 16, 50)--해골전사대장
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep15, 20, 9, 30)--선더리치
	ItemType = 7 ItemIndex = 14 ItemLevel = 8 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep15, 20, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--수호 투구
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep15, 21, 5, 50)--헬하운드
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep15, 21, 13, 30)--헬스파이더
	ItemType = 7 ItemIndex = 40 ItemLevel = 8 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep15, 21, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--레드윙 투구

-- 던전 정복(4)(121~130레벨)
	LuaQuestClass:SetQuestProgress(ASK1, Ep15, 28)--대화 진행상 완료창이 필요한 경우 사용
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep15, 24, 8, 50)--포이즌 소뿔전사
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep15, 24, 10, 30)--다크나이트
	ItemType = 9 ItemIndex = 1 ItemLevel = 8 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep15, 24, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--드라곤 바지
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep15, 25, 13, 50)--헬스파이더
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep15, 25, 16, 30)--해골전사대장
	ItemType = 9 ItemIndex = 3 ItemLevel = 8 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep15, 25, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--전설의 바지
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep15, 26, 8, 50)--포이즌 소뿔전사
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep15, 26, 10, 30)--다크나이트
	ItemType = 9 ItemIndex = 14 ItemLevel = 8 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep15, 26, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--수호 바지
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep15, 27, 13, 50)--헬스파이더
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep15, 27, 16, 30)--해골전사대장
	ItemType = 9 ItemIndex = 40 ItemLevel = 8 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep15, 27, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--레드윙 바지

-- 던전 정복(5)(131~140레벨)
	LuaQuestClass:SetQuestProgress(ASK1, Ep15, 34)--대화 진행상 완료창이 필요한 경우 사용
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep15, 30, 10, 50)--다크나이트
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep15, 30, 18, 10)--고르곤
	ItemType = 8 ItemIndex = 1 ItemLevel = 8 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep15, 30, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--드라곤 갑옷
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep15, 31, 16, 50)--해골전사대장
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep15, 31, 9, 30)--썬더리치
	ItemType = 8 ItemIndex = 3 ItemLevel = 8 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep15, 31, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--전설의 갑옷
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep15, 32, 10, 50)--다크나이트
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep15, 32, 18, 10)--고르곤
	ItemType = 8 ItemIndex = 14 ItemLevel = 8 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep15, 32, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--수호 갑옷
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep15, 33, 16, 50)--해골전사대장
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep15, 33, 9, 30)--썬더리치
	ItemType = 8 ItemIndex = 40 ItemLevel = 8 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep15, 33, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--레드윙 갑옷

-- 던전 정복(6)(141~160레벨)
	LuaQuestClass:SetQuestProgress(ASK1, Ep15, 40)--대화 진행상 완료창이 필요한 경우 사용
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep15, 36, 8, 50)--포이즌 소뿔전사
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep15, 36, 10, 50)--다크나이트
	LuaQuestClass:SetQuestMonsterKill(ASK3, Ep15, 36, 18, 30)--고르곤
	ItemType = 0 ItemIndex = 13 ItemLevel = 8 dur = 0 ItemSkill = 1 ItemCnt = 2
	LuaQuestClass:SetQuestRewardItem(Ep15, 36, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--더블블레이드
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep15, 37, 8, 50)--포이즌 소뿔전사
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep15, 37, 10, 5)--다크나이트
	LuaQuestClass:SetQuestMonsterKill(ASK3, Ep15, 37, 18, 1)--고르곤
	ItemType = 5 ItemIndex = 5 ItemLevel = 7 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep15, 37, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--전설의지팡이
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep15, 38, 8, 50)--포이즌 소뿔전사
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep15, 38, 10, 50)--다크나이트
	LuaQuestClass:SetQuestMonsterKill(ASK3, Ep15, 38, 18, 30)--고르곤
	ItemType = 4 ItemIndex = 13 ItemLevel = 8 dur = 0 ItemSkill = 1 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep15, 38, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--블루윙석궁
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep15, 39, 8, 50)--포이즌 소뿔전사
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep15, 39, 10, 5)--다크나이트
	LuaQuestClass:SetQuestMonsterKill(ASK3, Ep15, 39, 18, 1)--고르곤
	ItemType = 5 ItemIndex = 16 ItemLevel = 7 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep15, 39, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--레드윙스틱

-- 척후 부대 임무(1)(161~165레벨)
	LuaQuestClass:SetQuestProgress(ASK1, Ep15, 46)--대화 진행상 완료창이 필요한 경우 사용
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep15, 42, 36, 50)--쉐도우
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep15, 42, 39, 50)--포이즌 쉐도우
	ItemType = 10 ItemIndex = 34 ItemLevel = 7 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep15, 42, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--애쉬크로우장갑
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep15, 43, 25, 5)--아이스 퀸
	ItemType = 10 ItemIndex = 35 ItemLevel = 7 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep15, 43, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--이클립스장갑
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep15, 44, 36, 50)--쉐도우
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep15, 44, 39, 50)--포이즌 쉐도우
	ItemType = 10 ItemIndex = 36 ItemLevel = 7 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep15, 44, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--아이리스장갑
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep15, 45, 25, 5)--아이스 퀸
	ItemType = 10 ItemIndex = 41 ItemLevel = 7 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep15, 45, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--에이션트장갑

-- 척후 부대 임무(2)(166~170레벨)
	LuaQuestClass:SetQuestProgress(ASK1, Ep15, 52)--대화 진행상 완료창이 필요한 경우 사용
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep15, 48, 39, 50)--포이즌 쉐도우
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep15, 48, 34, 30)--저주받은 법사
	ItemType = 11 ItemIndex = 34 ItemLevel = 7 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep15, 48, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--애쉬크로우부츠
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep15, 49, 424, 100)--괴수 우씨
	ItemType = 11 ItemIndex = 35 ItemLevel = 7 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep15, 49, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--이클립스부츠
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep15, 50, 39, 50)--포이즌 쉐도우
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep15, 50, 34, 30)--저주받은 법사
	ItemType = 11 ItemIndex = 36 ItemLevel = 7 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep15, 50, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--아이리스부츠
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep15, 51, 424, 100)--괴수 우씨
	ItemType = 11 ItemIndex = 41 ItemLevel = 7 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep15, 51, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--에이션트부츠

-- 척후 부대 임무(3)(171~179레벨)
	LuaQuestClass:SetQuestProgress(ASK1, Ep15, 58)--대화 진행상 완료창이 필요한 경우 사용
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep15, 54, 34, 50)--저주받은 법사
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep15, 54, 41, 50)--데스 카우
	ItemType = 7 ItemIndex = 34 ItemLevel = 7 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep15, 54, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--애쉬크로우헬멧
	ItemType = 9 ItemIndex = 34 ItemLevel = 7 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep15, 54, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--애쉬크로우바지
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep15, 55, 425, 100)--괴수 우씨 대장
	ItemType = 7 ItemIndex = 35 ItemLevel = 7 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep15, 55, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--이클립스헬멧
	ItemType = 9 ItemIndex = 35 ItemLevel = 7 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep15, 55, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--이클립스바지
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep15, 56, 34, 50)--저주받은 법사
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep15, 56, 41, 50)--데스 카우
	ItemType = 7 ItemIndex = 36 ItemLevel = 7 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep15, 56, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--아이리스헬멧
	ItemType = 9 ItemIndex = 36 ItemLevel = 7 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep15, 56, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--아이리스바지
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep15, 57, 425, 100)--괴수 우씨 대장
	ItemType = 7 ItemIndex = 41 ItemLevel = 7 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep15, 57, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--에이션트헬멧
	ItemType = 9 ItemIndex = 41 ItemLevel = 7 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep15, 57, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--에이션트바지


-- 척후 부대 임무(4)(180~185레벨)<카오스 무기류 옵션 추가>
	LuaQuestClass:SetQuestProgress(ASK1, Ep15, 64)--대화 진행상 완료창이 필요한 경우 사용
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep15, 60, 41, 100)--데스카우
	ItemType = 8 ItemIndex = 34 ItemLevel = 7 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep15, 60, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--애쉬크로우헬멧
	ItemType = 2 ItemIndex = 6 ItemLevel = 4 dur = 0 ItemSkill = 0 ItemCnt = 1 Overlap = 0 Opt = Opt4
	LuaQuestClass:SetQuestRewardItem(Ep15, 60, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt, Overlap, Opt)--카오스드라곤도끼
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep15, 61, 36, 100)--쉐도우
	ItemType = 8 ItemIndex = 35 ItemLevel = 7 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep15, 61, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--이클립스헬멧
	ItemType = 2 ItemIndex = 6 ItemLevel = 4 dur = 0 ItemSkill = 0 ItemCnt = 1 Overlap = 0 Opt = Opt4
	LuaQuestClass:SetQuestRewardItem(Ep15, 61, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt, Overlap, Opt)--카오스드라곤도끼
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep15, 62, 41, 100)--데스 카우
	ItemType = 8 ItemIndex = 36 ItemLevel = 7 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep15, 62, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--아이리스헬멧
	ItemType = 2 ItemIndex = 6 ItemLevel = 4 dur = 0 ItemSkill = 0 ItemCnt = 1 Overlap = 0 Opt = Opt4
	LuaQuestClass:SetQuestRewardItem(Ep15, 62, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt, Overlap, Opt)--카오스드라곤도끼
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep15, 63, 36, 100)--쉐도우
	ItemType = 8 ItemIndex = 41 ItemLevel = 7 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep15, 63, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--에이션트헬멧
	ItemType = 2 ItemIndex = 6 ItemLevel = 4 dur = 0 ItemSkill = 0 ItemCnt = 1 Overlap = 0 Opt = Opt4
	LuaQuestClass:SetQuestRewardItem(Ep15, 63, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt, Overlap, Opt)--카오스드라곤도끼


-- 이계 탐사(1)(186~189레벨)
	LuaQuestClass:SetQuestProgress(ASK1, Ep15, 70)--대화 진행상 완료창이 필요한 경우 사용
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep15, 66, 41, 50)--데스 카우
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep15, 66, 37, 30)--데빌
	ItemType = 10 ItemIndex = 34 ItemLevel = 8 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep15, 66, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--애쉬크로우헬멧
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep15, 67, 36, 50)--쉐도우
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep15, 67, 39, 30)--포이즌 쉐도우
	ItemType = 10 ItemIndex = 35 ItemLevel = 8 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep15, 67, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--이클립스헬멧
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep15, 68, 41, 50)--데스 카우
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep15, 68, 37, 30)--데빌
	ItemType = 10 ItemIndex = 36 ItemLevel = 8 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep15, 68, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--아이리스헬멧
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep15, 69, 36, 50)--쉐도우
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep15, 69, 39, 30)--포이즌 쉐도우
	ItemType = 10 ItemIndex = 41 ItemLevel = 8 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep15, 69, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--에이션트헬멧



-- 이계 탐사(2)(190~193레벨)
	LuaQuestClass:SetQuestProgress(ASK1, Ep15, 76)--대화 진행상 완료창이 필요한 경우 사용
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep15, 72, 37, 100)--데빌
	ItemType = 11 ItemIndex = 34 ItemLevel = 8 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep15, 72, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--애쉬크로우부츠
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep15, 73, 39, 50)--포이즌 쉐도우
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep15, 73, 34, 30)--저주받은 법사
	ItemType = 11 ItemIndex = 35 ItemLevel = 8 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep15, 73, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--이클립스부츠
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep15, 74, 37, 100)--데빌
	ItemType = 11 ItemIndex = 36 ItemLevel = 8 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep15, 74, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--아이리스부츠
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep15, 75, 39, 50)--포이즌 쉐도우
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep15, 75, 34, 30)--저주받은 법사
	ItemType = 11 ItemIndex = 41 ItemLevel = 8 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep15, 75, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--에이션트부츠


-- 이계 탐사(3)(194~197레벨)
	LuaQuestClass:SetQuestProgress(ASK1, Ep15, 82)--대화 진행상 완료창이 필요한 경우 사용
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep15, 78, 40, 100)--데스나이트
	ItemType = 7 ItemIndex = 34 ItemLevel = 8 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep15, 78, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--애쉬크로우투구
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep15, 79, 34, 50)--저주받은 법사
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep15, 79, 41, 50)--데스 카우
	ItemType = 7 ItemIndex = 35 ItemLevel = 8 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep15, 79, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--이클립스투구
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep15, 80, 40, 100)--데스나이트
	ItemType = 7 ItemIndex = 36 ItemLevel = 8 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep15, 80, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--아이리스투구
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep15, 81, 34, 50)--저주받은 법사
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep15, 81, 41, 50)--데스 카우
	ItemType = 7 ItemIndex = 41 ItemLevel = 8 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep15, 81, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--에이션트투구

-- 이계 탐사(4)(198~201레벨)
	LuaQuestClass:SetQuestProgress(ASK1, Ep15, 88)--대화 진행상 완료창이 필요한 경우 사용
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep15, 84, 40, 50)--데스나이트
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep15, 84, 35, 30)--데스고르곤
	ItemType = 9 ItemIndex = 34 ItemLevel = 8 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep15, 84, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--애쉬크로우바지
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep15, 85, 41, 100)--데스카우
	ItemType = 9 ItemIndex = 35 ItemLevel = 8 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep15, 85, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--이클립스바지
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep15, 86, 40, 50)--데스나이트
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep15, 86, 35, 30)--데스고르곤
	ItemType = 9 ItemIndex = 36 ItemLevel = 8 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep15, 86, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--아이리스바지
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep15, 87, 41, 100)--데스카우
	ItemType = 9 ItemIndex = 41 ItemLevel = 8 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep15, 87, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--에이션트바지

-- 이계 탐사(5)(202~205레벨)
	LuaQuestClass:SetQuestProgress(ASK1, Ep15, 94)--대화 진행상 완료창이 필요한 경우 사용
	

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep15, 90, 35, 50)--데스고르곤
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep15, 90, 38, 10)--발록
	ItemType = 8 ItemIndex = 34 ItemLevel = 8 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep15, 90, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--애쉬크로우갑옷
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep15, 91, 37, 100)--데빌
	ItemType = 8 ItemIndex = 35 ItemLevel = 8 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep15, 91, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--이클립스갑옷
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep15, 92, 35, 50)--데스고르곤
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep15, 92, 38, 10)--발록
	ItemType = 8 ItemIndex = 36 ItemLevel = 8 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep15, 92, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--아이리스갑옷
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep15, 93, 37, 100)--데빌
	ItemType = 8 ItemIndex = 41 ItemLevel = 8 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep15, 93, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--에이션트갑옷


-- 이계 탐사(6)(206~209레벨)
	LuaQuestClass:SetQuestProgress(ASK1, Ep15, 100)--대화 진행상 완료창이 필요한 경우 사용
	

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep15, 96, 40, 50)--데스나이트
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep15, 96, 35, 30)--데스고르곤
	LuaQuestClass:SetQuestMonsterKill(ASK3, Ep15, 96, 38, 20)--발록
	ItemType = 2 ItemIndex = 5 ItemLevel = 7 dur = 0 ItemSkill = 1 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep15, 96, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--크리스탈검
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep15, 97, 40, 100)--데스나이트
	ItemType = 5 ItemIndex = 5 ItemLevel = 8 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep15, 97, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--전설의지팡이
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep15, 98, 40, 50)--데스나이트
	LuaQuestClass:SetQuestMonsterKill(ASK2, Ep15, 98, 35, 30)--데스고르곤
	LuaQuestClass:SetQuestMonsterKill(ASK3, Ep15, 98, 38, 20)--발록
	ItemType = 4 ItemIndex = 14 ItemLevel = 7 dur = 0 ItemSkill = 1 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep15, 98, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--아쿠아골드석궁
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep15, 99, 40, 100)--데스나이트
	ItemType = 5 ItemIndex = 16 ItemLevel = 8 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep15, 99, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--레드윙스틱
end
	
	