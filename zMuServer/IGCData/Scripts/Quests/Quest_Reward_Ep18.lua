--[[
Quest_Reward_Ep18.lua
[ Start  ] 	2009.08.
[ Latest ]	2009-12-09 16:45 PM
[ Version ]	0.0.0.1
[ Text ]									
[ Desc ]									
--]]

function Quest_Reward_Window_Ep18(iObjIndex)

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
-- 반복 퀘스트 (1~259레벨)
---------------------------------------------------------------------------------------------------------------------

-- 거미 사냥(1~14레벨)
	LuaQuestClass:SetQuestProgress(ASK1, Ep18, 2)--대화 진행상 완료창이 필요한 경우 사용
	

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep18, 1, 3, 10)--거미
	LuaQuestClass:SetQuestRewardExp(Ep18, 1, 2000)

-- 약탈자 처리(15~25레벨)
	LuaQuestClass:SetQuestProgress(ASK1, Ep18, 8)--대화 진행상 완료창이 필요한 경우 사용
	

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep18, 4, 7, 15)--자이언트
	LuaQuestClass:SetQuestRewardExp(Ep18, 4, 6000)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep18, 5, 6, 15)--리치
	LuaQuestClass:SetQuestRewardExp(Ep18, 5, 6000)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep18, 6, 31, 15)--아곤
	LuaQuestClass:SetQuestRewardExp(Ep18, 6, 6000)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep18, 7, 421, 15)--오염된 나비
	LuaQuestClass:SetQuestRewardExp(Ep18, 7, 6000)
	
-- 약탈자 두목 처리(26~35레벨)
	LuaQuestClass:SetQuestProgress(ASK1, Ep18, 14)--대화 진행상 완료창이 필요한 경우 사용
	

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep18, 10, 14, 20)--해골전사
	LuaQuestClass:SetQuestRewardExp(Ep18, 10, 10000)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep18, 11, 7, 20)--자이언트
	LuaQuestClass:SetQuestRewardExp(Ep18, 11, 10000)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep18, 12, 32, 20)--돌괴물
	LuaQuestClass:SetQuestRewardExp(Ep18, 12, 10000)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep18, 13, 419, 20)--흉측한 토끼
	LuaQuestClass:SetQuestRewardExp(Ep18, 13, 10000)
	
	
-- 보급로 처리(36~45레벨)
	LuaQuestClass:SetQuestProgress(ASK1, Ep18, 18)--대화 진행상 완료창이 필요한 경우 사용
	

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep18, 16, 22, 25)--얼음괴물
	LuaQuestClass:SetQuestRewardExp(Ep18, 16, 14000)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep18, 17, 24, 25)--웜
	LuaQuestClass:SetQuestRewardExp(Ep18, 17, 14000)


-- 보급부대 처리(46~55레벨)
	LuaQuestClass:SetQuestProgress(ASK1, Ep18, 22)--대화 진행상 완료창이 필요한 경우 사용
	

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep18, 20, 21, 30)--암살자
	LuaQuestClass:SetQuestRewardExp(Ep18, 20, 21000)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep18, 21, 22, 30)--얼음괴물
	LuaQuestClass:SetQuestRewardExp(Ep18, 21, 21000)


-- 보급부대 처리(56~65레벨)
	LuaQuestClass:SetQuestProgress(ASK1, Ep18, 26)--대화 진행상 완료창이 필요한 경우 사용
	

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep18, 24, 19, 30)--설인
	LuaQuestClass:SetQuestRewardExp(Ep18, 24, 28000)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep18, 25, 23, 30)--호머드
	LuaQuestClass:SetQuestRewardExp(Ep18, 25, 28000)


	
-- 던전 청소(66~79레벨)
	LuaQuestClass:SetQuestProgress(ASK1, Ep18, 30)--대화 진행상 완료창이 필요한 경우 사용
	

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep18, 28, 17, 40)--싸이클롭스
	LuaQuestClass:SetQuestRewardExp(Ep18, 28, 36000)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep18, 29, 12, 40)--유충
	LuaQuestClass:SetQuestRewardExp(Ep18, 29, 36000)


-- 엘베란드 장로의 부탁(80~90레벨)
	LuaQuestClass:SetQuestProgress(ASK1, Ep18, 34)--대화 진행상 완료창이 필요한 경우 사용
	

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep18, 32, 423, 50)--토템골렘
	LuaQuestClass:SetQuestRewardExp(Ep18, 32, 38000)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep18, 33, 422, 50)--저주받은 리치
	LuaQuestClass:SetQuestRewardExp(Ep18, 33, 38000)

		
-- 던전 상황 파악(1)(91~100레벨)
	LuaQuestClass:SetQuestProgress(ASK1, Ep18, 38)--대화 진행상 완료창이 필요한 경우 사용
	

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep18, 36, 5, 50)--헬하운드
	LuaQuestClass:SetQuestRewardExp(Ep18, 36, 43000)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep18, 37, 17, 50)--싸이클롭스
	LuaQuestClass:SetQuestRewardExp(Ep18, 37, 43000)

	

-- 던전 상황 파악(2)(101~110레벨)
	LuaQuestClass:SetQuestProgress(ASK1, Ep18, 42)--대화 진행상 완료창이 필요한 경우 사용
	

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep18, 40, 13, 50)--헬스파이더
	LuaQuestClass:SetQuestRewardExp(Ep18, 40, 50000)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep18, 41, 15, 50)--해골궁수
	LuaQuestClass:SetQuestRewardExp(Ep18, 41, 50000)

	

-- 던전 상황 파악(3)(111~120레벨)
	LuaQuestClass:SetQuestProgress(ASK1, Ep18, 46)--대화 진행상 완료창이 필요한 경우 사용
	

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep18, 44, 9, 50)--썬더리치
	LuaQuestClass:SetQuestRewardExp(Ep18, 44, 54000)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep18, 45, 5, 50)--헬하운드
	LuaQuestClass:SetQuestRewardExp(Ep18, 45, 54000)

	

-- 던전 상황 파악(4)(121~130레벨)
	LuaQuestClass:SetQuestProgress(ASK1, Ep18, 50)--대화 진행상 완료창이 필요한 경우 사용
	

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep18, 48, 8, 50)--포이즌 소뿔전사
	LuaQuestClass:SetQuestRewardExp(Ep18, 48, 60000)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep18, 49, 13, 50)--헬스파이더
	LuaQuestClass:SetQuestRewardExp(Ep18, 49, 60000)

	

-- 던전 상황 파악(5)(131~140레벨)
	LuaQuestClass:SetQuestProgress(ASK1, Ep18, 54)--대화 진행상 완료창이 필요한 경우 사용
	

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep18, 52, 10, 50)--다크 나이트
	LuaQuestClass:SetQuestRewardExp(Ep18, 52, 66000)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep18, 53, 9, 50)--썬더 리치
	LuaQuestClass:SetQuestRewardExp(Ep18, 53, 66000)

	

-- 던전 상황 파악(6)(141~160레벨)
	LuaQuestClass:SetQuestProgress(ASK1, Ep18, 58)--대화 진행상 완료창이 필요한 경우 사용
	

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep18, 56, 18, 15)--고르곤
	LuaQuestClass:SetQuestRewardExp(Ep18, 56, 72000)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep18, 57, 8, 50)--포이즌 소뿔전사
	LuaQuestClass:SetQuestRewardExp(Ep18, 57, 72000)

	

-- 척후부대의 지속적인 도움 요청(1)(161~165레벨)
	LuaQuestClass:SetQuestProgress(ASK1, Ep18, 62)--대화 진행상 완료창이 필요한 경우 사용
	

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep18, 60, 36, 50)--쉐도우
	LuaQuestClass:SetQuestRewardExp(Ep18, 60, 80000)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep18, 61, 25, 3)--아이스 퀸
	LuaQuestClass:SetQuestRewardExp(Ep18, 61, 80000)

	

-- 척후부대의 지속적인 도움 요청(2)(166~170레벨)
	LuaQuestClass:SetQuestProgress(ASK1, Ep18, 66)--대화 진행상 완료창이 필요한 경우 사용
	

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep18, 64, 39, 50)--포이즌 쉐도우
	LuaQuestClass:SetQuestRewardExp(Ep18, 64, 82000)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep18, 65, 424, 50)--괴수 우씨
	LuaQuestClass:SetQuestRewardExp(Ep18, 65, 82000)
	
	
-- 척후부대의 지속적인 도움 요청(3)(171~179레벨)
	LuaQuestClass:SetQuestProgress(ASK1, Ep18, 70)--대화 진행상 완료창이 필요한 경우 사용
	

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep18, 68, 34, 50)--저주받은 법사
	LuaQuestClass:SetQuestRewardExp(Ep18, 68, 84000)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep18, 69, 425, 50)--괴수우씨 대장
	LuaQuestClass:SetQuestRewardExp(Ep18, 69, 84000)


	
	
-- 척후부대의 지속적인 도움 요청(4)(180~189레벨)
	LuaQuestClass:SetQuestProgress(ASK1, Ep18, 74)--대화 진행상 완료창이 필요한 경우 사용
	

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep18, 72, 41, 50)--데쓰카우
	LuaQuestClass:SetQuestRewardExp(Ep18, 72, 95000)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep18, 73, 36, 50)--쉐도우 
	LuaQuestClass:SetQuestRewardExp(Ep18, 73, 95000)


	
-- 결계수호 (190~199레벨)
	LuaQuestClass:SetQuestProgress(ASK1, Ep18, 78)--대화 진행상 완료창이 필요한 경우 사용
	

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep18, 76, 37, 50)--데빌
	LuaQuestClass:SetQuestRewardExp(Ep18, 76, 110000)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep18, 77, 34, 50)--저주받은 법사
	LuaQuestClass:SetQuestRewardExp(Ep18, 77, 110000)


-- 결계수호 (200~209레벨)
	LuaQuestClass:SetQuestProgress(ASK1, Ep18, 82)--대화 진행상 완료창이 필요한 경우 사용
	

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep18, 80, 40, 50)--데스나이트
	LuaQuestClass:SetQuestRewardExp(Ep18, 80, 120000)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep18, 81, 41, 50)--데쓰카우
	LuaQuestClass:SetQuestRewardExp(Ep18, 81, 120000)


	
-- 결계수호 (210~219레벨)
	LuaQuestClass:SetQuestProgress(ASK1, Ep18, 86)--대화 진행상 완료창이 필요한 경우 사용
	

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep18, 84, 35, 60)--데스고르곤
	LuaQuestClass:SetQuestRewardExp(Ep18, 84, 130000)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep18, 85, 37, 60)--데빌
	LuaQuestClass:SetQuestRewardExp(Ep18, 85, 130000)

-- 이카루스 진입로 (220~229레벨)
	LuaQuestClass:SetQuestProgress(ASK1, Ep18, 90)--대화 진행상 완료창이 필요한 경우 사용
	

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep18, 88, 38, 15)--발록
	LuaQuestClass:SetQuestRewardExp(Ep18, 88, 160000)
	ItemType = 12 ItemIndex = 15 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep18, 88, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--혼석
	
	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep18, 89, 40, 60)--데스나이트
	LuaQuestClass:SetQuestRewardExp(Ep18, 89, 160000)
	ItemType = 12 ItemIndex = 15 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep18, 89, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--혼석

	
	LuaQuestClass:SetQuestProgress(ASK1, Ep18, 94)--대화 진행상 완료창이 필요한 경우 사용
	

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep18, 92, 38, 15)--발록
	LuaQuestClass:SetQuestRewardExp(Ep18, 92, 140000)

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep18, 93, 40, 60)--데스나이트
	LuaQuestClass:SetQuestRewardExp(Ep18, 93, 140000)			
	
	
	
-- 척후 부대의 새로운 임무(1) (230~234레벨)
	LuaQuestClass:SetQuestProgress(ASK1, Ep18, 97)--대화 진행상 완료창이 필요한 경우 사용
	

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep18, 96, 62, 60)--뮤턴트
	LuaQuestClass:SetQuestRewardExp(Ep18, 96, 170000)
	ItemType = 12 ItemIndex = 15 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep18, 96, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--혼석
	
	
	LuaQuestClass:SetQuestProgress(ASK1, Ep18, 100)--대화 진행상 완료창이 필요한 경우 사용
	

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep18, 99, 62, 60)--뮤턴트
	LuaQuestClass:SetQuestRewardExp(Ep18, 99, 150000)


	
-- 척후 부대의 새로운 임무(2) (235~239레벨)
	LuaQuestClass:SetQuestProgress(ASK1, Ep18, 103)--대화 진행상 완료창이 필요한 경우 사용
	

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep18, 102, 60, 60)--블러디울프
	LuaQuestClass:SetQuestRewardExp(Ep18, 102, 180000)
	ItemType = 14 ItemIndex = 14 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep18, 102, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--영석
	
	
	LuaQuestClass:SetQuestProgress(ASK1, Ep18, 106)--대화 진행상 완료창이 필요한 경우 사용
	

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep18, 105, 60, 60)--블러디울프
	LuaQuestClass:SetQuestRewardExp(Ep18, 105, 160000)


-- 척후 부대의 새로운 임무(3) (240~244레벨)
	LuaQuestClass:SetQuestProgress(ASK1, Ep18, 109)--대화 진행상 완료창이 필요한 경우 사용
	

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep18, 108, 57, 60)--아이언휠
	LuaQuestClass:SetQuestRewardExp(Ep18, 108, 190000)
	ItemType = 14 ItemIndex = 14 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep18, 108, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--영석
	
	
	LuaQuestClass:SetQuestProgress(ASK1, Ep18, 112)--대화 진행상 완료창이 필요한 경우 사용
	

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep18, 111, 57, 60)--아이언휠
	LuaQuestClass:SetQuestRewardExp(Ep18, 111, 170000)

-- 천공성의로 진출(1) (245~249레벨)
	LuaQuestClass:SetQuestProgress(ASK1, Ep18, 115)--대화 진행상 완료창이 필요한 경우 사용
	

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep18, 114, 69, 80)--알카모스
	LuaQuestClass:SetQuestRewardExp(Ep18, 114, 200000)
	ItemType = 14 ItemIndex = 13 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep18, 114, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--축석
	
	
	LuaQuestClass:SetQuestProgress(ASK1, Ep18, 118)--대화 진행상 완료창이 필요한 경우 사용
	

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep18, 117, 69, 80)--알카모스
	LuaQuestClass:SetQuestRewardExp(Ep18, 117, 180000)


			
	
-- 천공성의로 진출(2) (250~254레벨)
	LuaQuestClass:SetQuestProgress(ASK1, Ep18, 121)--대화 진행상 완료창이 필요한 경우 사용
	

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep18, 120, 71, 80)--메가크러스트
	LuaQuestClass:SetQuestRewardExp(Ep18, 120, 210000)
	ItemType = 14 ItemIndex = 13 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep18, 120, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--축석
	
	
	LuaQuestClass:SetQuestProgress(ASK1, Ep18, 124)--대화 진행상 완료창이 필요한 경우 사용
	

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep18, 123, 71, 80)--메가크러스트
	LuaQuestClass:SetQuestRewardExp(Ep18, 123, 190000)


			
-- 천공성의로 진출(3) (255~259레벨)
	LuaQuestClass:SetQuestProgress(ASK1, Ep18, 127)--대화 진행상 완료창이 필요한 경우 사용
	

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep18, 126, 70, 80)--퀸레이너
	LuaQuestClass:SetQuestRewardExp(Ep18, 126, 220000)
	ItemType = 14 ItemIndex = 13 ItemLevel = 0 dur = 0 ItemSkill = 0 ItemCnt = 1
	LuaQuestClass:SetQuestRewardItem(Ep18, 126, ItemType, ItemIndex, ItemLevel, dur, ItemSkill, ItemCnt)--축석
	
	
	LuaQuestClass:SetQuestProgress(ASK1, Ep18, 130)--대화 진행상 완료창이 필요한 경우 사용
	

	LuaQuestClass:SetQuestMonsterKill(ASK1, Ep18, 129, 70, 80)--퀸레이너
	LuaQuestClass:SetQuestRewardExp(Ep18, 129, 200000)


	
	
---------------------------------------------------------------------------------------------------------------------	

end
	
	