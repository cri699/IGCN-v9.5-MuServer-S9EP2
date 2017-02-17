--[[
Quest_Ep18.lua
[ Start  ] 	2009.08.
[ Latest ]	2009-09-15 15:50 PM
[ Version ]	0.0.0.0
[ Text ]									
[ Desc ]									
--]]
QuestInclude("Quest_Define.lua")

function Ep18_Progress(SelectSyntactic, objIndex)

	local QS = LuaQuestClass:GetQuestSwitch( Ep18, objIndex)
		
	if QS == 0 then
		if SelectSyntactic == 1 then -- 수락 
			LuaQuestClass:SendQuestProgress( Ep18, 1, objIndex)-- 
			return 
		end
		if SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress( Ep18, 2, objIndex)-- 
			return 
		end
	elseif QS == 3 then
		if SelectSyntactic == 1 then -- 수락 
			local CharClass = LuaQuestClass:GetCharClass(objIndex)
		
			if CharClass == CLASS_KNIGHT or CharClass == CLASS_MAGUMSA or CharClass == CLASS_DARKLORD or CharClass == CLASS_MONK then--흑기사, 마검사, 다크로드
				LuaQuestClass:SendQuestProgress(Ep18, 4, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_WIZARD then--흑마법사
				LuaQuestClass:SendQuestProgress(Ep18, 5, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_ELF then--요정
				LuaQuestClass:SendQuestProgress(Ep18, 6, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_SUMMONER then--소환술사
				LuaQuestClass:SendQuestProgress(Ep18, 7, objIndex)-- 
				return
			end
		end
		if SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress( Ep18, 8, objIndex)-- 
			return 
		end
	elseif QS == 9 then
		if SelectSyntactic == 1 then -- 수락 
			local CharClass = LuaQuestClass:GetCharClass(objIndex)
		
			if CharClass == CLASS_KNIGHT or CharClass == CLASS_MAGUMSA or CharClass == CLASS_DARKLORD or CharClass == CLASS_MONK then--흑기사, 마검사, 다크로드
				LuaQuestClass:SendQuestProgress(Ep18, 10, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_WIZARD then--흑마법사
				LuaQuestClass:SendQuestProgress(Ep18, 11, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_ELF then--요정
				LuaQuestClass:SendQuestProgress(Ep18, 12, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_SUMMONER then--소환술사
				LuaQuestClass:SendQuestProgress(Ep18, 13, objIndex)-- 
				return
			end
		end
		if SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress( Ep18, 14, objIndex)-- 
			return 
		end
	elseif QS == 15 then
		if SelectSyntactic == 1 then -- 수락 
			local CharClass = LuaQuestClass:GetCharClass(objIndex)
		
			if CharClass == CLASS_KNIGHT or CharClass == CLASS_MAGUMSA or CharClass == CLASS_DARKLORD or CharClass == CLASS_ELF or CharClass == CLASS_MONK then--흑기사, 마검사, 다크로드, 엘프
				LuaQuestClass:SendQuestProgress(Ep18, 16, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_WIZARD or CharClass == CLASS_SUMMONER then--흑마법사, 소환술사
				LuaQuestClass:SendQuestProgress(Ep18, 17, objIndex)-- 
				return
			end
			
		end
		if SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress( Ep18, 18, objIndex)-- 
			return 
		end
	elseif QS == 19 then
		if SelectSyntactic == 1 then -- 수락 
			local CharClass = LuaQuestClass:GetCharClass(objIndex)
		
			if CharClass == CLASS_KNIGHT or CharClass == CLASS_MAGUMSA or CharClass == CLASS_DARKLORD or CharClass == CLASS_ELF or CharClass == CLASS_MONK then--흑기사, 마검사, 다크로드, 엘프
				LuaQuestClass:SendQuestProgress(Ep18, 20, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_WIZARD or CharClass == CLASS_SUMMONER then--흑마법사, 소환술사
				LuaQuestClass:SendQuestProgress(Ep18, 21, objIndex)-- 
				return
			end
			
		end
		if SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress( Ep18, 22, objIndex)-- 
			return 
		end
	elseif QS == 23 then
		if SelectSyntactic == 1 then -- 수락 
			local CharClass = LuaQuestClass:GetCharClass(objIndex)
		
			if CharClass == CLASS_KNIGHT or CharClass == CLASS_MAGUMSA or CharClass == CLASS_DARKLORD or CharClass == CLASS_ELF or CharClass == CLASS_MONK then--흑기사, 마검사, 다크로드, 엘프
				LuaQuestClass:SendQuestProgress(Ep18, 24, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_WIZARD or CharClass == CLASS_SUMMONER then--흑마법사, 소환술사
				LuaQuestClass:SendQuestProgress(Ep18, 25, objIndex)-- 
				return
			end
			
		end
		if SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress( Ep18, 26, objIndex)-- 
			return 
		end
	elseif QS == 27 then
		if SelectSyntactic == 1 then -- 수락 
			local CharClass = LuaQuestClass:GetCharClass(objIndex)
		
			if CharClass == CLASS_KNIGHT or CharClass == CLASS_MAGUMSA or CharClass == CLASS_DARKLORD or CharClass == CLASS_ELF or CharClass == CLASS_MONK then--흑기사, 마검사, 다크로드, 엘프
				LuaQuestClass:SendQuestProgress(Ep18, 28, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_WIZARD or CharClass == CLASS_SUMMONER then--흑마법사, 소환술사
				LuaQuestClass:SendQuestProgress(Ep18, 29, objIndex)-- 
				return
			end
			
		end
		if SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress( Ep18, 30, objIndex)-- 
			return 
		end
	elseif QS == 31 then
		if SelectSyntactic == 1 then -- 수락 
			local CharClass = LuaQuestClass:GetCharClass(objIndex)
		
			if CharClass == CLASS_KNIGHT or CharClass == CLASS_MAGUMSA or CharClass == CLASS_DARKLORD or CharClass == CLASS_ELF or CharClass == CLASS_MONK then--흑기사, 마검사, 다크로드, 엘프
				LuaQuestClass:SendQuestProgress(Ep18, 32, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_WIZARD or CharClass == CLASS_SUMMONER then--흑마법사, 소환술사
				LuaQuestClass:SendQuestProgress(Ep18, 33, objIndex)-- 
				return
			end
			
		end
		if SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress( Ep18, 34, objIndex)-- 
			return 
		end
	elseif QS == 35 then
		if SelectSyntactic == 1 then -- 수락 
			local CharClass = LuaQuestClass:GetCharClass(objIndex)
		
			if CharClass == CLASS_KNIGHT or CharClass == CLASS_MAGUMSA or CharClass == CLASS_DARKLORD or CharClass == CLASS_ELF or CharClass == CLASS_MONK then--흑기사, 마검사, 다크로드, 엘프
				LuaQuestClass:SendQuestProgress(Ep18, 36, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_WIZARD or CharClass == CLASS_SUMMONER then--흑마법사, 소환술사
				LuaQuestClass:SendQuestProgress(Ep18, 37, objIndex)-- 
				return
			end
			
		end
		if SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress( Ep18, 38, objIndex)-- 
			return 
		end
	elseif QS == 39 then
		if SelectSyntactic == 1 then -- 수락 
			local CharClass = LuaQuestClass:GetCharClass(objIndex)
		
			if CharClass == CLASS_KNIGHT or CharClass == CLASS_MAGUMSA or CharClass == CLASS_DARKLORD or CharClass == CLASS_ELF or CharClass == CLASS_MONK then--흑기사, 마검사, 다크로드, 엘프
				LuaQuestClass:SendQuestProgress(Ep18, 40, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_WIZARD or CharClass == CLASS_SUMMONER then--흑마법사, 소환술사
				LuaQuestClass:SendQuestProgress(Ep18, 41, objIndex)-- 
				return
			end
			
		end
		if SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress( Ep18, 42, objIndex)-- 
			return 
		end
	elseif QS == 43 then
		if SelectSyntactic == 1 then -- 수락 
			local CharClass = LuaQuestClass:GetCharClass(objIndex)
		
			if CharClass == CLASS_KNIGHT or CharClass == CLASS_MAGUMSA or CharClass == CLASS_DARKLORD or CharClass == CLASS_ELF or CharClass == CLASS_MONK then--흑기사, 마검사, 다크로드, 엘프
				LuaQuestClass:SendQuestProgress(Ep18, 44, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_WIZARD or CharClass == CLASS_SUMMONER then--흑마법사, 소환술사
				LuaQuestClass:SendQuestProgress(Ep18, 45, objIndex)-- 
				return
			end
			
		end
		if SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress( Ep18, 46, objIndex)-- 
			return 
		end
	elseif QS == 47 then
		if SelectSyntactic == 1 then -- 수락 
			local CharClass = LuaQuestClass:GetCharClass(objIndex)
		
			if CharClass == CLASS_KNIGHT or CharClass == CLASS_MAGUMSA or CharClass == CLASS_DARKLORD or CharClass == CLASS_ELF or CharClass == CLASS_MONK then--흑기사, 마검사, 다크로드, 엘프
				LuaQuestClass:SendQuestProgress(Ep18, 48, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_WIZARD or CharClass == CLASS_SUMMONER then--흑마법사, 소환술사
				LuaQuestClass:SendQuestProgress(Ep18, 49, objIndex)-- 
				return
			end
			
		end
		if SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress( Ep18, 50, objIndex)-- 
			return 
		end
	elseif QS == 51 then
		if SelectSyntactic == 1 then -- 수락 
			local CharClass = LuaQuestClass:GetCharClass(objIndex)
		
			if CharClass == CLASS_KNIGHT or CharClass == CLASS_MAGUMSA or CharClass == CLASS_DARKLORD or CharClass == CLASS_ELF or CharClass == CLASS_MONK then--흑기사, 마검사, 다크로드, 엘프
				LuaQuestClass:SendQuestProgress(Ep18, 52, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_WIZARD or CharClass == CLASS_SUMMONER then--흑마법사, 소환술사
				LuaQuestClass:SendQuestProgress(Ep18, 53, objIndex)-- 
				return
			end
			
		end
		if SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress( Ep18, 54, objIndex)-- 
			return 
		end
	elseif QS == 55 then
		if SelectSyntactic == 1 then -- 수락 
			local CharClass = LuaQuestClass:GetCharClass(objIndex)
		
			if CharClass == CLASS_KNIGHT or CharClass == CLASS_MAGUMSA or CharClass == CLASS_DARKLORD or CharClass == CLASS_ELF or CharClass == CLASS_MONK then--흑기사, 마검사, 다크로드, 엘프
				LuaQuestClass:SendQuestProgress(Ep18, 56, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_WIZARD or CharClass == CLASS_SUMMONER then--흑마법사, 소환술사
				LuaQuestClass:SendQuestProgress(Ep18, 57, objIndex)-- 
				return
			end
			
		end
		if SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress( Ep18, 58, objIndex)-- 
			return 
		end
	elseif QS == 59 then
		if SelectSyntactic == 1 then -- 수락 
			local CharClass = LuaQuestClass:GetCharClass(objIndex)
		
			if CharClass == CLASS_KNIGHT or CharClass == CLASS_MAGUMSA or CharClass == CLASS_DARKLORD or CharClass == CLASS_ELF or CharClass == CLASS_MONK then--흑기사, 마검사, 다크로드, 엘프
				LuaQuestClass:SendQuestProgress(Ep18, 60, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_WIZARD or CharClass == CLASS_SUMMONER then--흑마법사, 소환술사
				LuaQuestClass:SendQuestProgress(Ep18, 61, objIndex)-- 
				return
			end
			
		end
		if SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress( Ep18, 62, objIndex)-- 
			return 
		end
	elseif QS == 63 then
		if SelectSyntactic == 1 then -- 수락 
			local CharClass = LuaQuestClass:GetCharClass(objIndex)
		
			if CharClass == CLASS_KNIGHT or CharClass == CLASS_MAGUMSA or CharClass == CLASS_DARKLORD or CharClass == CLASS_ELF or CharClass == CLASS_MONK then--흑기사, 마검사, 다크로드, 엘프
				LuaQuestClass:SendQuestProgress(Ep18, 64, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_WIZARD or CharClass == CLASS_SUMMONER then--흑마법사, 소환술사
				LuaQuestClass:SendQuestProgress(Ep18, 65, objIndex)-- 
				return
			end
			
		end
		if SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress( Ep18, 66, objIndex)-- 
			return 
		end
	elseif QS == 67 then
		if SelectSyntactic == 1 then -- 수락 
			local CharClass = LuaQuestClass:GetCharClass(objIndex)
		
			if CharClass == CLASS_KNIGHT or CharClass == CLASS_MAGUMSA or CharClass == CLASS_DARKLORD or CharClass == CLASS_ELF or CharClass == CLASS_MONK then--흑기사, 마검사, 다크로드, 엘프
				LuaQuestClass:SendQuestProgress(Ep18, 68, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_WIZARD or CharClass == CLASS_SUMMONER then--흑마법사, 소환술사
				LuaQuestClass:SendQuestProgress(Ep18, 69, objIndex)-- 
				return
			end
			
		end
		if SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress( Ep18, 70, objIndex)-- 
			return 
		end
	elseif QS == 71 then
		if SelectSyntactic == 1 then -- 수락 
			local CharClass = LuaQuestClass:GetCharClass(objIndex)
		
			if CharClass == CLASS_KNIGHT or CharClass == CLASS_MAGUMSA or CharClass == CLASS_DARKLORD or CharClass == CLASS_ELF or CharClass == CLASS_MONK then--흑기사, 마검사, 다크로드, 엘프
				LuaQuestClass:SendQuestProgress(Ep18, 72, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_WIZARD or CharClass == CLASS_SUMMONER then--흑마법사, 소환술사
				LuaQuestClass:SendQuestProgress(Ep18, 73, objIndex)-- 
				return
			end
			
		end
		if SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress( Ep18, 74, objIndex)-- 
			return 
		end
	elseif QS == 75 then
		if SelectSyntactic == 1 then -- 수락 
			local CharClass = LuaQuestClass:GetCharClass(objIndex)
		
			if CharClass == CLASS_KNIGHT or CharClass == CLASS_MAGUMSA or CharClass == CLASS_DARKLORD or CharClass == CLASS_ELF or CharClass == CLASS_MONK then--흑기사, 마검사, 다크로드, 엘프
				LuaQuestClass:SendQuestProgress(Ep18, 76, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_WIZARD or CharClass == CLASS_SUMMONER then--흑마법사, 소환술사
				LuaQuestClass:SendQuestProgress(Ep18, 77, objIndex)-- 
				return
			end
			
		end
		if SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress( Ep18, 78, objIndex)-- 
			return 
		end
	elseif QS == 79 then
		if SelectSyntactic == 1 then -- 수락 
			local CharClass = LuaQuestClass:GetCharClass(objIndex)
		
			if CharClass == CLASS_KNIGHT or CharClass == CLASS_MAGUMSA or CharClass == CLASS_DARKLORD or CharClass == CLASS_ELF or CharClass == CLASS_MONK then--흑기사, 마검사, 다크로드, 엘프
				LuaQuestClass:SendQuestProgress(Ep18, 80, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_WIZARD or CharClass == CLASS_SUMMONER then--흑마법사, 소환술사
				LuaQuestClass:SendQuestProgress(Ep18, 81, objIndex)-- 
				return
			end
			
		end
		if SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress( Ep18, 82, objIndex)-- 
			return 
		end
	elseif QS == 83 then
		if SelectSyntactic == 1 then -- 수락 
			local CharClass = LuaQuestClass:GetCharClass(objIndex)
		
			if CharClass == CLASS_KNIGHT or CharClass == CLASS_MAGUMSA or CharClass == CLASS_DARKLORD or CharClass == CLASS_ELF or CharClass == CLASS_MONK then--흑기사, 마검사, 다크로드, 엘프
				LuaQuestClass:SendQuestProgress(Ep18, 84, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_WIZARD or CharClass == CLASS_SUMMONER then--흑마법사, 소환술사
				LuaQuestClass:SendQuestProgress(Ep18, 85, objIndex)-- 
				return
			end
			
		end
		if SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress( Ep18, 86, objIndex)-- 
			return 
		end
	elseif QS == 87 then
		if SelectSyntactic == 1 then -- 수락 
			local CharClass = LuaQuestClass:GetCharClass(objIndex)
		
			if CharClass == CLASS_KNIGHT or CharClass == CLASS_MAGUMSA or CharClass == CLASS_DARKLORD or CharClass == CLASS_ELF or CharClass == CLASS_MONK then--흑기사, 마검사, 다크로드, 엘프
				LuaQuestClass:SendQuestProgress(Ep18, 88, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_WIZARD or CharClass == CLASS_SUMMONER then--흑마법사, 소환술사
				LuaQuestClass:SendQuestProgress(Ep18, 89, objIndex)-- 
				return
			end
			
		end
		if SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress( Ep18, 90, objIndex)-- 
			return 
		end
	elseif QS == 91 then
		if SelectSyntactic == 1 then -- 수락 
			local CharClass = LuaQuestClass:GetCharClass(objIndex)
		
			if CharClass == CLASS_KNIGHT or CharClass == CLASS_MAGUMSA or CharClass == CLASS_DARKLORD or CharClass == CLASS_ELF or CharClass == CLASS_MONK then--흑기사, 마검사, 다크로드, 엘프
				LuaQuestClass:SendQuestProgress(Ep18, 92, objIndex)-- 
				return
			end
			
			if CharClass == CLASS_WIZARD or CharClass == CLASS_SUMMONER then--흑마법사, 소환술사
				LuaQuestClass:SendQuestProgress(Ep18, 93, objIndex)-- 
				return
			end
			
		end
		if SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress( Ep18, 94, objIndex)-- 
			return 
		end
	elseif QS == 95 then
		if SelectSyntactic == 1 then -- 수락 
			LuaQuestClass:SendQuestProgress( Ep18, 96, objIndex)-- 
			return 
		end
		if SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress( Ep18, 97, objIndex)-- 
			return 
		end
	elseif QS == 98 then
		if SelectSyntactic == 1 then -- 수락 
			LuaQuestClass:SendQuestProgress( Ep18, 99, objIndex)-- 
			return 
		end
		if SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress( Ep18, 100, objIndex)-- 
			return 
		end
	elseif QS == 101 then
		if SelectSyntactic == 1 then -- 수락 
			LuaQuestClass:SendQuestProgress( Ep18, 102, objIndex)-- 
			return 
		end
		if SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress( Ep18, 103, objIndex)-- 
			return 
		end
	elseif QS == 104 then
		if SelectSyntactic == 1 then -- 수락 
			LuaQuestClass:SendQuestProgress( Ep18, 105, objIndex)-- 
			return 
		end
		if SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress( Ep18, 106, objIndex)-- 
			return 
		end
	elseif QS == 107 then
		if SelectSyntactic == 1 then -- 수락 
			LuaQuestClass:SendQuestProgress( Ep18, 108, objIndex)-- 
			return 
		end
		if SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress( Ep18, 109, objIndex)-- 
			return 
		end
	elseif QS == 110 then
		if SelectSyntactic == 1 then -- 수락 
			LuaQuestClass:SendQuestProgress( Ep18, 111, objIndex)-- 
			return 
		end
		if SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress( Ep18, 112, objIndex)-- 
			return 
		end
	elseif QS == 113 then
		if SelectSyntactic == 1 then -- 수락 
			LuaQuestClass:SendQuestProgress( Ep18, 114, objIndex)-- 
			return 
		end
		if SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress( Ep18, 115, objIndex)-- 
			return 
		end
	elseif QS == 116 then
		if SelectSyntactic == 1 then -- 수락 
			LuaQuestClass:SendQuestProgress( Ep18, 117, objIndex)-- 
			return 
		end
		if SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress( Ep18, 118, objIndex)-- 
			return 
		end
	elseif QS == 119 then
		if SelectSyntactic == 1 then -- 수락 
			LuaQuestClass:SendQuestProgress( Ep18, 120, objIndex)-- 
			return 
		end
		if SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress( Ep18, 121, objIndex)-- 
			return 
		end
	elseif QS == 122 then
		if SelectSyntactic == 1 then -- 수락 
			LuaQuestClass:SendQuestProgress( Ep18, 123, objIndex)-- 
			return 
		end
		if SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress( Ep18, 124, objIndex)-- 
			return 
		end
	elseif QS == 125 then
		if SelectSyntactic == 1 then -- 수락 
			LuaQuestClass:SendQuestProgress( Ep18, 126, objIndex)-- 
			return 
		end
		if SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress( Ep18, 127, objIndex)-- 
			return 
		end
	elseif QS == 128 then
		if SelectSyntactic == 1 then -- 수락 
			LuaQuestClass:SendQuestProgress( Ep18, 129, objIndex)-- 
			return 
		end
		if SelectSyntactic == 2 then -- 거절
			LuaQuestClass:SendQuestProgress( Ep18, 130, objIndex)-- 
			return 
		end


	end
	
-- 선택문 모두 입력 시키기~

end

function Ep18_ChkCompleteQS(QS)

--	local QS = LuaQuestClass:GetQuestSwitch( Ep18, objIndex)

	if QS == 1
	or QS == 4 or QS == 5 or QS == 6 or QS == 7
	or QS == 10 or QS == 11 or QS == 12 or QS == 13
	or QS == 16 or QS == 17 
	or QS == 20 or QS == 21
	or QS == 24 or QS == 25
	or QS == 28 or QS == 29
	or QS == 32 or QS == 33
	or QS == 36 or QS == 37
	or QS == 40 or QS == 41
	or QS == 44 or QS == 45
	or QS == 48 or QS == 49
	or QS == 52 or QS == 53
	or QS == 56 or QS == 57
	or QS == 60 or QS == 61	
	or QS == 64 or QS == 65
	or QS == 68 or QS == 69
	or QS == 72 or QS == 73	
	or QS == 76 or QS == 77
	or QS == 80 or QS == 81
	or QS == 84 or QS == 85
	or QS == 88 or QS == 89
	or QS == 92 or QS == 93
	or QS == 96 or QS == 99
	or QS == 102 or QS == 105
	or QS == 108 or QS == 111
	or QS == 114 or QS == 117
	or QS == 120 or QS == 123
	or QS == 126 or QS == 129
	then
		return 1
	end
	
	return 0
end

function Ep18_SendCompleteQS(QS, objIndex)

--	local QS = LuaQuestClass:GetQuestSwitch(Ep18, objIndex)
	local UserLv = LuaQuestClass:GetUserLv(objIndex)
	if QS == 1 then
		if UserLv < 15 then
			LuaQuestClass:SetQuestSwitch(Ep18, 0, objIndex)
		elseif UserLv >= 15 then
			LuaQuestClass:SetQuestSwitch(Ep18, 3, objIndex)
		end
	elseif QS == 4 or QS == 5 or QS == 6 or QS == 7 then	
		if UserLv < 26 then
			LuaQuestClass:SetQuestSwitch(Ep18, 3, objIndex)
		elseif UserLv >= 26 then
			LuaQuestClass:SetQuestSwitch(Ep18, 9, objIndex)
		end
------------------------------------
	elseif QS == 10 or  QS == 11 or QS == 12 or QS == 13 then
		if UserLv < 36 then
			LuaQuestClass:SetQuestSwitch(Ep18, 9, objIndex)
		elseif UserLv >= 36 then
			LuaQuestClass:SetQuestSwitch(Ep18, 15, objIndex)
		end

-----------------------------------------								
	elseif QS == 16 or  QS == 17 then
		if UserLv < 46 then
			LuaQuestClass:SetQuestSwitch(Ep18, 15, objIndex)
		elseif UserLv >= 46 then
			LuaQuestClass:SetQuestSwitch(Ep18, 19, objIndex)
		end

--------------------------------------
	elseif QS == 20 or  QS == 21 then
		if UserLv < 56 then
			LuaQuestClass:SetQuestSwitch(Ep18, 19, objIndex)
		elseif UserLv >= 56 then
			LuaQuestClass:SetQuestSwitch(Ep18, 23, objIndex)
		end

--------------------------------------	
	elseif QS == 24 or  QS == 25 then
		if UserLv < 66 then
			LuaQuestClass:SetQuestSwitch(Ep18, 23, objIndex)
		elseif UserLv >= 56 then
			LuaQuestClass:SetQuestSwitch(Ep18, 27, objIndex)
		end

	
-----------------------------------------	
	elseif QS == 28  or  QS == 29 then
		if UserLv < 80 then
			LuaQuestClass:SetQuestSwitch(Ep18, 27, objIndex)
		elseif UserLv >= 80 then
			LuaQuestClass:SetQuestSwitch(Ep18, 31, objIndex)
		end

	
-------------------------------------------
	elseif QS == 32  or  QS == 33 then
		if UserLv < 91 then
			LuaQuestClass:SetQuestSwitch(Ep18, 31, objIndex)
		elseif UserLv >= 91 then
			LuaQuestClass:SetQuestSwitch(Ep18, 35, objIndex)
		end


---------------------------
	elseif QS == 36  or  QS == 37 then
		if UserLv < 101 then
			LuaQuestClass:SetQuestSwitch(Ep18, 35, objIndex)
		elseif UserLv >= 101 then
			LuaQuestClass:SetQuestSwitch(Ep18, 39, objIndex)
		end


----------------------------
	elseif QS == 40  or  QS == 41 then
		if UserLv < 111 then
			LuaQuestClass:SetQuestSwitch(Ep18, 39, objIndex)
		elseif UserLv >= 111 then
			LuaQuestClass:SetQuestSwitch(Ep18, 43, objIndex)
		end


-------------------------------
	elseif QS == 44  or  QS == 45 then
		if UserLv < 121 then
			LuaQuestClass:SetQuestSwitch(Ep18, 43, objIndex)
		elseif UserLv >= 121 then
			LuaQuestClass:SetQuestSwitch(Ep18, 47, objIndex)
		end


--------------------------------
	elseif QS == 48  or  QS == 49 then
		if UserLv < 131 then
			LuaQuestClass:SetQuestSwitch(Ep18, 47, objIndex)
		elseif UserLv >= 131 then
			LuaQuestClass:SetQuestSwitch(Ep18, 51, objIndex)
		end


--------------------------------------
	elseif QS == 52  or  QS == 53 then
		if UserLv < 141 then
			LuaQuestClass:SetQuestSwitch(Ep18, 51, objIndex)
		elseif UserLv >= 56 then
			LuaQuestClass:SetQuestSwitch(Ep18, 55, objIndex)
		end


---------------------------------------
	elseif QS == 56  or  QS == 57 then
		if UserLv < 161 then
			LuaQuestClass:SetQuestSwitch(Ep18, 55, objIndex)
		elseif UserLv >= 56 then
			LuaQuestClass:SetQuestSwitch(Ep18, 59, objIndex)
		end


-------------------------------------------
	elseif QS == 60  or  QS == 61 then
		if UserLv < 166 then
			LuaQuestClass:SetQuestSwitch(Ep18, 59, objIndex)
		elseif UserLv >= 166 then
			LuaQuestClass:SetQuestSwitch(Ep18, 63, objIndex)
		end


----------------------------------
	elseif QS == 64  or  QS == 65 then
		if UserLv < 171 then
			LuaQuestClass:SetQuestSwitch(Ep18, 63, objIndex)
		elseif UserLv >= 171 then
			LuaQuestClass:SetQuestSwitch(Ep18, 67, objIndex)
		end


-----------------------------------
	elseif QS == 68  or  QS == 69 then
		if UserLv < 180 then
			LuaQuestClass:SetQuestSwitch(Ep18, 67, objIndex)
		elseif UserLv >= 180 then
			LuaQuestClass:SetQuestSwitch(Ep18, 71, objIndex)
		end


-------------------------------------
	elseif QS == 72  or  QS == 73 then
		if UserLv < 190 then
			LuaQuestClass:SetQuestSwitch(Ep18, 71, objIndex)
		elseif UserLv >= 190 then
			LuaQuestClass:SetQuestSwitch(Ep18, 75, objIndex)
		end


----------------------------------------
	elseif QS == 76  or  QS == 77 then
		if UserLv < 200 then
			LuaQuestClass:SetQuestSwitch(Ep18, 75, objIndex)
		elseif UserLv >= 200 then
			LuaQuestClass:SetQuestSwitch(Ep18, 79, objIndex)
		end


--------------------------------------	

	elseif QS == 80  or  QS == 81 then
		if UserLv < 210 then
			LuaQuestClass:SetQuestSwitch(Ep18, 79, objIndex)
		elseif UserLv >= 210 then
			LuaQuestClass:SetQuestSwitch(Ep18, 83, objIndex)
		end


--------------------------------------	
	elseif QS == 84  or  QS == 85 then
		if UserLv < 220 then
			LuaQuestClass:SetQuestSwitch(Ep18, 83, objIndex)
		elseif UserLv >= 220 then
			LuaQuestClass:SetQuestSwitch(Ep18, 87, objIndex)
		end


--------------------------------------	
	elseif QS == 88  or  QS == 89 or QS == 92  or  QS == 93 then
		if UserLv < 230 then
			LuaQuestClass:SetQuestSwitch(Ep18, 91, objIndex)
		elseif UserLv >= 230 then
			LuaQuestClass:SetQuestSwitch(Ep18, 95, objIndex)
		end

--------------------------------------	
	elseif QS == 96 or QS == 99 then
		if UserLv < 235 then
			LuaQuestClass:SetQuestSwitch(Ep18, 98, objIndex)
		elseif UserLv >= 235 then
			LuaQuestClass:SetQuestSwitch(Ep18, 101, objIndex)
		end


--------------------------------------	
	elseif QS == 102 or QS == 105 then
		if UserLv < 240 then
			LuaQuestClass:SetQuestSwitch(Ep18, 104, objIndex)
		elseif UserLv >= 240 then
			LuaQuestClass:SetQuestSwitch(Ep18, 107, objIndex)
		end


--------------------------------------	
	elseif QS == 108 or QS == 111 then
		if UserLv < 245 then
			LuaQuestClass:SetQuestSwitch(Ep18, 110, objIndex)
		elseif UserLv >= 245 then
			LuaQuestClass:SetQuestSwitch(Ep18, 113, objIndex)
		end


--------------------------------------	
	elseif QS == 114 or QS == 117 then
		if UserLv < 250 then
			LuaQuestClass:SetQuestSwitch(Ep18, 116, objIndex)
		elseif UserLv >= 250 then
			LuaQuestClass:SetQuestSwitch(Ep18, 119, objIndex)
		end


--------------------------------------	
	elseif QS == 120 or QS == 123 then
		if UserLv < 255 then
			LuaQuestClass:SetQuestSwitch(Ep18, 122, objIndex)
		elseif UserLv >= 255 then
			LuaQuestClass:SetQuestSwitch(Ep18, 125, objIndex)
		end


--------------------------------------	
	elseif QS == 126 or QS == 129 then
		if UserLv < 260 then
			LuaQuestClass:SetQuestSwitch(Ep18, 128, objIndex)
		elseif UserLv >= 260 then
			LuaQuestClass:SetQuestSwitch(Ep18, 131, objIndex)
		end
	end

--------------------------------------	
	return
end

function Ep18_Complete(objIndex)

	local LimitLv = 259
		
	
	local QS = LuaQuestClass:GetQuestSwitch( Ep18, objIndex)
	
	if QS == 2 then
		LuaQuestClass:SetQuestSwitch( Ep18, 0, objIndex)
		LuaQuestClass:SendQuestComplete( Ep18, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif QS == 8 then
		LuaQuestClass:SetQuestSwitch( Ep18, 3, objIndex)
		LuaQuestClass:SendQuestComplete( Ep18, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif  QS == 14 then
		LuaQuestClass:SetQuestSwitch( Ep18, 9, objIndex)
		LuaQuestClass:SendQuestComplete( Ep18, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif QS == 18 then
		LuaQuestClass:SetQuestSwitch( Ep18, 15, objIndex)
		LuaQuestClass:SendQuestComplete( Ep18, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif  QS == 22 then
		LuaQuestClass:SetQuestSwitch( Ep18, 19, objIndex)
		LuaQuestClass:SendQuestComplete( Ep18, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif  QS == 26 then
		LuaQuestClass:SetQuestSwitch( Ep18, 23, objIndex)
		LuaQuestClass:SendQuestComplete( Ep18, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif  QS == 30 then
		LuaQuestClass:SetQuestSwitch( Ep18, 27, objIndex)
		LuaQuestClass:SendQuestComplete( Ep18, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif  QS == 34 then
		LuaQuestClass:SetQuestSwitch( Ep18, 31, objIndex)
		LuaQuestClass:SendQuestComplete( Ep18, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif  QS == 38 then
		LuaQuestClass:SetQuestSwitch( Ep18, 35, objIndex)
		LuaQuestClass:SendQuestComplete( Ep18, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif  QS == 42 then
		LuaQuestClass:SetQuestSwitch( Ep18, 39, objIndex)
		LuaQuestClass:SendQuestComplete( Ep18, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif  QS == 46 then
		LuaQuestClass:SetQuestSwitch( Ep18, 43, objIndex)
		LuaQuestClass:SendQuestComplete( Ep18, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif  QS == 50 then
		LuaQuestClass:SetQuestSwitch( Ep18, 47, objIndex)
		LuaQuestClass:SendQuestComplete( Ep18, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif  QS == 54 then
		LuaQuestClass:SetQuestSwitch( Ep18, 51, objIndex)
		LuaQuestClass:SendQuestComplete( Ep18, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif  QS == 58 then
		LuaQuestClass:SetQuestSwitch( Ep18, 55, objIndex)
		LuaQuestClass:SendQuestComplete( Ep18, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif  QS == 62 then
		LuaQuestClass:SetQuestSwitch( Ep18, 59, objIndex)
		LuaQuestClass:SendQuestComplete( Ep18, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif  QS == 66 then
		LuaQuestClass:SetQuestSwitch( Ep18, 63, objIndex)
		LuaQuestClass:SendQuestComplete( Ep18, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif  QS == 70 then
		LuaQuestClass:SetQuestSwitch( Ep18, 67, objIndex)
		LuaQuestClass:SendQuestComplete( Ep18, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif  QS == 74 then
		LuaQuestClass:SetQuestSwitch( Ep18, 71, objIndex)
		LuaQuestClass:SendQuestComplete( Ep18, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif  QS == 78 then
		LuaQuestClass:SetQuestSwitch( Ep18, 75, objIndex)
		LuaQuestClass:SendQuestComplete( Ep18, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif  QS == 82 then
		LuaQuestClass:SetQuestSwitch( Ep18, 79, objIndex)
		LuaQuestClass:SendQuestComplete( Ep18, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif  QS == 86 then
		LuaQuestClass:SetQuestSwitch( Ep18, 83, objIndex)
		LuaQuestClass:SendQuestComplete( Ep18, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif  QS == 90 then
		LuaQuestClass:SetQuestSwitch( Ep18, 87, objIndex)
		LuaQuestClass:SendQuestComplete( Ep18, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif  QS == 94 then
		LuaQuestClass:SetQuestSwitch( Ep18, 91, objIndex)
		LuaQuestClass:SendQuestComplete( Ep18, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif  QS == 97 then
		LuaQuestClass:SetQuestSwitch( Ep18, 95, objIndex)
		LuaQuestClass:SendQuestComplete( Ep18, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif  QS == 100 then
		LuaQuestClass:SetQuestSwitch( Ep18, 98, objIndex)
		LuaQuestClass:SendQuestComplete( Ep18, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif  QS == 103 then
		LuaQuestClass:SetQuestSwitch( Ep18, 101, objIndex)
		LuaQuestClass:SendQuestComplete( Ep18, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif  QS == 106 then
		LuaQuestClass:SetQuestSwitch( Ep18, 104, objIndex)
		LuaQuestClass:SendQuestComplete( Ep18, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif  QS == 109 then
		LuaQuestClass:SetQuestSwitch( Ep18, 107, objIndex)
		LuaQuestClass:SendQuestComplete( Ep18, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif  QS == 112 then
		LuaQuestClass:SetQuestSwitch( Ep18, 110, objIndex)
		LuaQuestClass:SendQuestComplete( Ep18, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif  QS == 115 then
		LuaQuestClass:SetQuestSwitch( Ep18, 113, objIndex)
		LuaQuestClass:SendQuestComplete( Ep18, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif  QS == 118 then
		LuaQuestClass:SetQuestSwitch( Ep18, 116, objIndex)
		LuaQuestClass:SendQuestComplete( Ep18, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif  QS == 121 then
		LuaQuestClass:SetQuestSwitch( Ep18, 119, objIndex)
		LuaQuestClass:SendQuestComplete( Ep18, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif  QS == 124 then
		LuaQuestClass:SetQuestSwitch( Ep18, 122, objIndex)
		LuaQuestClass:SendQuestComplete( Ep18, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif  QS == 127 then
		LuaQuestClass:SetQuestSwitch( Ep18, 125, objIndex)
		LuaQuestClass:SendQuestComplete( Ep18, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
	elseif  QS == 130 then
		LuaQuestClass:SetQuestSwitch( Ep18, 128, objIndex)
		LuaQuestClass:SendQuestComplete( Ep18, QS, objIndex)--진행상 보상 없이 확인 버튼 눌렀을때.
		return
							
	end	

	
	local bComplete = LuaQuestClass:IsQuestComplete( Ep18, objIndex)--완료 체크
	
	if bComplete == false then
		return
	end


	local ChkCompleteQS = Ep18_ChkCompleteQS(QS)
	
	if ChkCompleteQS == 1 then
	
		local UserLv = LuaQuestClass:GetUserLv(objIndex)
		
		if UserLv <= LimitLv then
			local Result = LuaQuestClass:SendQuestReward(Ep18, objIndex)
			if Result == 1 then
				--LuaQuestClass:SetQuestSwitch(Ep18, 5, objIndex)
				Ep18_SendCompleteQS(QS, objIndex)
			end
			return
		elseif UserLv > LimitLv then
			local Result = LuaQuestClass:SendQuestReward(Ep18, objIndex)
			if Result == 1 then
				LuaQuestClass:SetQuestSwitch(Ep18, 131, objIndex)
			end

			return
		end
		
		return
	end

end

