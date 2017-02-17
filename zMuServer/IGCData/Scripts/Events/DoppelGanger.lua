-- // ============================================================
-- // == INTERNATIONAL GAMING CENTER NETWORK
-- // == www.igc-network.com
-- // == (C) 2010-2015 IGC-Network (R)
-- // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
-- // == File is a part of IGCN Group MuOnline Server files.
-- // ============================================================

-- DoppelGanger Control Script, Lua v5.2
-- DoppelGanger Event - Can be modified to adjust for own needs


-- DoppelGanger Map Information function initialization
---- FN_LuaSetDoppelgangerInit()

-- Get Highest User Level
---- LuaGetMaxUserLevel()

-- Get Lowest User Level
---- LuaGetMinUserLevel()

-- Monster HP Settings
---- LuaSetMonsterHp(monsterIndex, HP)

-- 몬스터 출현 시간 설정
---- LuaSetAddMonsterTime(Second)

-- Monster Attack Possibility
---- LuaSetMonsterAttackRate(Rate)

-- Event Preparation Time
---- LuaSetReadyTime(Minute)

-- Event Processing TIme
---- LuaSetPlayTime(Minute)

-- Time to receive award
---- LuaSetEndTime(Minute)


local classDoppelgangerLua = CDoppelgangerLua()

--//////////////////////////////////////////////////////////////////////////////
-- Doppelganger map information initializations
--//////////////////////////////////////////////////////////////////////////////
function FN_LuaDopplegangerInit()
	-- Preparation time (minutes)
	classDoppelgangerLua:LuaSetReadyTime(1)
	-- processing time (minutes)
	classDoppelgangerLua:LuaSetPlayTime(10)
	-- Time to receive award (minutes)
	classDoppelgangerLua:LuaSetEndTime(1)
end


--//////////////////////////////////////////////////////////////////////////////
-- Callback - invoked periodically
--//////////////////////////////////////////////////////////////////////////////
function FN_LuaDoppelgangerCallback(nCurTime)
	-- Current time
	--local nCurTime 			= classDoppelgangerLua:LuaGetLocalTime()

	-- Event Start time
	local nStateTime		= classDoppelgangerLua:LuaGetStateTime()
	-- Time after last cycle (milliseconds)
	local nPlayTime 		= (nCurTime - nStateTime) / 1000
	-- Monster Appearance time
	local nAddHerdMonsterTime = nCurTime
	--local nAddBossMonsterTime = nCurTime
	-- Ice Walker appearance
	local bIceWorkerRegen	= 0

	nAddHerdMonsterTime 	= classDoppelgangerLua:LuaGetAddHerdMonsterTime()
	--nAddBossMonsterTime 	= classDoppelgangerLua:LuaGetAddBossMonsterTime()
	local nBossRegenOrder	= classDoppelgangerLua:LuaGetBossRegenOrder()

	-- Map Number
	local nMapNumber = classDoppelgangerLua:LuaGetMapNumber()

	local nHerdIndex = -1

	-- Generate bunch of monsters
	if (nCurTime - nAddHerdMonsterTime) >= 3 * 1000 then

		-- Obtain Monster indexes
		nHerdIndex = classDoppelgangerLua:LuaGetMonsterHerdIndex()
		if nHerdIndex == -1 then
			return
		end

		-- Monster bunch Set Start Position
		classDoppelgangerLua:LuaSetHerdStartPosInfo(nHerdIndex, 0, 1)
		-- Monster bunch Set Exit Position
		classDoppelgangerLua:LuaSetHerdEndPosInfo(nHerdIndex, 0)

		-- Add bunch of monsters
		FN_LuaAddHerdMonster(nCurTime, nStateTime, nHerdIndex)

		-- 죽지 않고 도착지점에 들어간 도살자는 다시 생성
		local nKillerBossState = classDoppelgangerLua:LuaGetBossMonsterState()
		if nKillerBossState == 3 then
			FN_LuaAddMiddleBossMonster(nCurTime, nStateTime, nHerdIndex, 0)
		end

		-- 죽지 않고 도착지점에 들어간 분노한 도살자는 다시 생성
		local nAngerKillerBossState = classDoppelgangerLua:LuaGetLastBossMonsterState()
		if nAngerKillerBossState == 3 then
			FN_LuaAddLastBossMonster(nCurTime, nStateTime, nHerdIndex, 0)
		end

		-- 무리 몬스터 생성 시간 저장
		classDoppelgangerLua:LuaSetAddHerdMonsterTime(nCurTime)

	end

	-- 무리번호를 안받았으면 리턴
	if nHerdIndex == -1 then
		return
	end

	--DebugPrint('nBossRegenOrder', nBossRegenOrder)

	-- 보스급 몬스터 생성
	if nBossRegenOrder == 0 and nPlayTime >= 1 * 60 then
		-- 1분 후
		classDoppelgangerLua:LuaSetBossMonsterState(1)
		FN_LuaAddMiddleBossMonster(nCurTime, nStateTime, nHerdIndex, 1)
		classDoppelgangerLua:LuaSetBossRegenOrder(nBossRegenOrder+1)
	elseif nBossRegenOrder == 1 and nPlayTime >= 4 * 60 then
		-- 4분 후
		classDoppelgangerLua:LuaSetBossMonsterState(1)
		FN_LuaAddMiddleBossMonster(nCurTime, nStateTime, nHerdIndex, 1)
		classDoppelgangerLua:LuaSetBossRegenOrder(nBossRegenOrder+1)
	elseif nBossRegenOrder == 2 and nPlayTime >= 6 * 60 - 5 then
		-- 6분 후 아이스 워커를 출현시킨다
		bIceWorkerRegen	= 1
		classDoppelgangerLua:LuaSetBossRegenOrder(nBossRegenOrder+1)
	elseif nBossRegenOrder == 3 and nPlayTime >= 7 * 60 then
		-- 7분 후
		classDoppelgangerLua:LuaSetLastBossMonsterState(1)
		FN_LuaAddLastBossMonster(nCurTime, nStateTime, nHerdIndex, 1)
		classDoppelgangerLua:LuaSetBossRegenOrder(nBossRegenOrder+1)
	end


	-- 실제로 무리 몬스터를 움직임
	classDoppelgangerLua:LuaMonsterHerdStart(nHerdIndex)

	-- 아이스 워커 출현은 여기서
	if bIceWorkerRegen == 1 then
		-- 무리 몬스터 인덱스를 얻어옴
		local nHerdIndex = classDoppelgangerLua:LuaGetMonsterHerdIndex()
		if nHerdIndex == -1 then
			return
		end

		-- 위치를 설정
		local nPosInfo = classDoppelgangerLua:LuaGetRandomValue(16) + 3
		-- 무리 몬스터 시작위치 설정
		classDoppelgangerLua:LuaSetHerdStartPosInfo(nHerdIndex, nPosInfo, 0)
		-- 무리 몬스터 종료위치 설정
		classDoppelgangerLua:LuaSetHerdEndPosInfo(nHerdIndex, nPosInfo)

		-- Ice Walker Regen Notification
		classDoppelgangerLua:LuaIceworkerRegen(nPosInfo)

		-- Ice Walker Regen
		FN_LuaAddIceWorkerMonster(nCurTime, nStateTime, nHerdIndex)

		-- 실제로 무리 몬스터를 움직임
		classDoppelgangerLua:LuaMonsterHerdStart(nHerdIndex)

		bIceWorkerRegen = 0
	end

end

--//////////////////////////////////////////////////////////////////////////////
-- Add Regular Monsters
--//////////////////////////////////////////////////////////////////////////////
function FN_LuaAddHerdMonster(nCurTime, nStateTime, nHerdIndex)
	-- bring the highest level
	--local nMaxUserLevel	= classDoppelgangerLua:LuaGetMaxUserLevel()
	-- Calculate Monster HP basing on Player Level
	--local nMonsterHp = nMaxUserLevel --* 10
	-- Time after last cycle (milliseconds)
	local nPlayTime = nCurTime - nStateTime
	-- Map Number
	local nMapNumber = classDoppelgangerLua:LuaGetMapNumber()

	--Monster Attack (Minimum Damage) = nMonsterAtt * 0.8)
	--local nMonsterAtt 	= nMaxUserLevel + 100
	-- Monster Defense
	--local nMonsterDef 	= nMaxUserLevel + 100

	-- Set number of monsters
	local nMonsterCount = 1

	if nPlayTime < 3 * 60 * 1000 then
		nMonsterCount = 1
	elseif nPlayTime < 6 * 60 * 1000  then
		nMonsterCount = 2
	else
		nMonsterCount = 3
	end

	nMonsterCount = nMonsterCount + classDoppelgangerLua:LuaGetUserCount() - 1

	-- nMonsterCount Added by Monster
	for cnt = 0, nMonsterCount-1 do
		local nMonsterIndex = 533 + classDoppelgangerLua:LuaGetRandomValue(6)
		--local nMonsterIndex = 531 + classDoppelgangerLua:LuaGetRandomValue(9)
		--local nMonsterIndex = 530
		local nAttackFirst	= 0

		if classDoppelgangerLua:LuaGetRandomValue(1000) < 700 then
		--if classDoppelgangerLua:LuaGetRandomValue(10000) < 1000 then
			nAttackFirst = 1
		end

		-- Set Monster to attack first (suicide)
		if nMonsterIndex == 533 then
			nAttackFirst = 1
		end

		-- If Dark Lord then change to Magic Gladiator
		if nMonsterIndex == 538 then
			nMonsterIndex = 539
		end

		classDoppelgangerLua:LuaAddMonsterHerd(nHerdIndex, nMonsterIndex, nAttackFirst)
	end
end

--//////////////////////////////////////////////////////////////////////////////
-- Add Middle Boss Monster
--//////////////////////////////////////////////////////////////////////////////
function FN_LuaAddMiddleBossMonster(nCurTime, nStateTime, nHerdIndex, nFlag)
	-- bring the highest level
	--local nMaxUserLevel	= classDoppelgangerLua:LuaGetMaxUserLevel()
	-- Calculate Monster HP basing on Player Level
	--local nMonsterHp = nMaxUserLevel --* 10
	-- Time after last cycle (milliseconds)
	local nPlayTime = nCurTime - nStateTime
	-- Map Number
	local nMapNumber = classDoppelgangerLua:LuaGetMapNumber()

	-- Monster Attack (Minimum Damage) = nMonsterAtt * 0.8)
	--local nMonsterAtt 	= nMaxUserLevel + 100
	-- Monster Defense
	--local nMonsterDef 	= nMaxUserLevel + 100

	local nMonsterIndex = 530	-- Butcher
	local nAttackFirst	= 0
	classDoppelgangerLua:LuaAddMonsterHerd(nHerdIndex, nMonsterIndex, nAttackFirst)

	if nFlag == 1 then
		nMonsterIndex 	= 538	-- Dark Lord
		nAttackFirst	= 1
		classDoppelgangerLua:LuaAddMonsterHerd(nHerdIndex, nMonsterIndex, nAttackFirst)
	end
end

--//////////////////////////////////////////////////////////////////////////////
-- Add Final Boss Monster
--//////////////////////////////////////////////////////////////////////////////
function FN_LuaAddLastBossMonster(nCurTime, nStateTime, nHerdIndex, nFlag)
	-- bring the highest level
	--local nMaxUserLevel	= classDoppelgangerLua:LuaGetMaxUserLevel()
	-- Calculate Monster HP basing on Player Level
	--local nMonsterHp = nMaxUserLevel --* 10
	-- Time after last cycle (milliseconds)
	local nPlayTime = nCurTime - nStateTime
	-- Map Number
	local nMapNumber = classDoppelgangerLua:LuaGetMapNumber()

	-- Monster Attack (Minimum Damage) = nMonsterAtt * 0.8)
	--local nMonsterAtt 	= nMaxUserLevel + 100
	-- Monster Defense
	--local nMonsterDef 	= nMaxUserLevel + 100

	local nMonsterIndex = 529	-- Butcher
	local nAttackFirst	= 0
	classDoppelgangerLua:LuaAddMonsterHerd(nHerdIndex, nMonsterIndex, nAttackFirst)

	if nFlag == 1 then
		nMonsterIndex 	= 538	-- Dark Lord
		nAttackFirst	= 1
		classDoppelgangerLua:LuaAddMonsterHerd(nHerdIndex, nMonsterIndex, nAttackFirst)
	end
end

--//////////////////////////////////////////////////////////////////////////////
-- Ice Walker Add rule
--//////////////////////////////////////////////////////////////////////////////
function FN_LuaAddIceWorkerMonster(ncurTime, nStateTime, nHerdIndex)
	-- bring the highest level
	--local nMaxUserLevel	= classDoppelgangerLua:LuaGetMaxUserLevel()
	-- Calculate Monster HP basing on Player Level
	--local nMonsterHp = nMaxUserLevel * 10

	-- Monster Attack (Minimum Damage) = nMonsterAtt * 0.8)
	--local nMonsterAtt 	= nMaxUserLevel + 100
	-- Monster Defense
	--local nMonsterDef 	= nMaxUserLevel + 100

	local nMonsterIndex = 531	-- Ice Walker
	local nAttackFirst	= 1

	local nMonsterCount = classDoppelgangerLua:LuaGetUserCount()
	for cnt = 0, nMonsterCount-1 do
		classDoppelgangerLua:LuaAddMonsterHerd(nHerdIndex, nMonsterIndex, nAttackFirst)
	end
end

--//////////////////////////////////////////////////////////////////////////////
-- Get Start position of the monsters
--//////////////////////////////////////////////////////////////////////////////
function FN_LuaGetStartPosition(nMapNumber)
	local StartPosX = nill
	local StartPosY = nill

	if nMapNumber == 65 then
		StartPosX = 225
		StartPosY = 101
	elseif nMapNumber == 66 then
		StartPosX = 114
		StartPosY = 181
	elseif nMapNumber ==67 then
		StartPosX = 110
		StartPosY = 151
	elseif nMapNumber == 68 then
		StartPosX = 43
		StartPosY = 109
	end

	return StartPosX, StartPosY
end

--//////////////////////////////////////////////////////////////////////////////
-- Get End position of the monsters
--//////////////////////////////////////////////////////////////////////////////
function FN_LuaGetEndPosition(nMapNumber)
	local EndPosX = nill
	local EndPosY = nill

	if nMapNumber == 65 then
		EndPosX = 210
		EndPosY = 64
	elseif nMapNumber == 66 then
		EndPosX = 125
		EndPosY = 124
	elseif nMapNumber == 67 then
		EndPosX = 108
		EndPosY = 104
	elseif nMapNumber == 68 then
		EndPosX = 74
		EndPosY = 53
	end

	return EndPosX, EndPosY
end

--//////////////////////////////////////////////////////////////////////////////
-- Get the start position of the Ice Walker
--//////////////////////////////////////////////////////////////////////////////
function FN_LuaGetIceWorkerStartPos(nMapNumber)
	local StartPosX = nill
	local StartPosY = nill

	if nMapNumber == 65 then
		StartPosX = 210
		StartPosY = 64
	elseif nMapNumber == 66 then
		StartPosX = 125
		StartPosY = 124
	elseif nMapNumber ==67 then
		StartPosX = 108
		StartPosY = 104
	elseif nMapNumber == 68 then
		StartPosX = 74
		StartPosY = 53
	end

	return StartPosX, StartPosY
end

















