USE [MuOnline] -- 9.5.1.13
GO
/****** Object:  StoredProcedure [dbo].[IGC_ArcaBattleAllJoinUserSelect]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE   PROC [dbo].[IGC_ArcaBattleAllJoinUserSelect]
AS  
SET NOCOUNT ON
SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED

BEGIN
	SELECT G_Name, Number, CharName
	FROM	dbo.IGC_ARCA_BATTLE_MEMBER_JOIN_INFO
END

SET XACT_ABORT OFF  
SET NOCOUNT OFF













GO
/****** Object:  StoredProcedure [dbo].[IGC_ArcaBattleAllMarkCntSelect]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE  procedure [dbo].[IGC_ArcaBattleAllMarkCntSelect]
AS     
	SET NOCOUNT ON
	SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED

	SELECT G_Name, MarkCnt FROM dbo.IGC_ARCA_BATTLE_GUILDMARK_REG













GO
/****** Object:  StoredProcedure [dbo].[IGC_ArcaBattleGuildGroupNumSelect]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE  procedure [dbo].[IGC_ArcaBattleGuildGroupNumSelect]  
 @CharName varchar(10)
as         
BEGIN      
    
 DECLARE @GuildName varchar(8)
 DECLARE @return int    
 SET @return = 0      
         
 Set  nocount on    

set @GuildName = (SELECT G_Name FROM IGC_ARCA_BATTLE_MEMBER_JOIN_INFO where CharName = @CharName)

 SELECT GroupNum FROM IGC_ARCA_BATTLE_GUILD_JOIN_INFO
  WHERE G_Name =  @GuildName

 SET XACT_ABORT OFF    
 Set nocount off    
      
END













GO
/****** Object:  StoredProcedure [dbo].[IGC_ArcaBattleGuildInsert]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER OFF
GO

CREATE  procedure [dbo].[IGC_ArcaBattleGuildInsert]
 @GuildName varchar(8),      
 @CharName varchar(10),      
 @Number int
as     
BEGIN  

 DECLARE @return int
 DECLARE @GuildNum int
 DECLARE @GuildGroupNum tinyint
 SET @return = 0  
     
 Set  nocount on

SELECT @GuildNum = Number FROM IGC_ARCA_BATTLE_GUILD_JOIN_INFO
set @GuildGroupNum = @@ROWCOUNT
IF( @GuildGroupNum >= 6 )
BEGIN
   SET  @return = 3
   GOTO EndProc
END

IF NOT EXISTS ( SELECT Number FROM IGC_ARCA_BATTLE_GUILD_JOIN_INFO WITH ( READUNCOMMITTED )       
    WHERE Number =  @Number )  
  BEGIN 
   begin transaction      
  INSERT INTO IGC_ARCA_BATTLE_GUILD_JOIN_INFO (G_Name, G_Master, Number, JoinDate, GroupNum) VALUES      
   (  @GuildName, @CharName, @Number, GetDate(), @GuildGroupNum+1 )

  INSERT INTO IGC_ARCA_BATTLE_MEMBER_JOIN_INFO (G_Name, Number, CharName, JoinDate) VALUES      
   (  @GuildName, @Number, @CharName, GetDate() )
   goto EndProcTran
   END
ELSE
 BEGIN
   SET  @return = 4
   GOTO EndProc
 END

EndProcTran:
	IF ( @@Error  <> 0 )
	BEGIN	
		rollback transaction
		SET @return = -1
		SELECT @return
	END 
	ELSE
	BEGIN
		commit transaction
		SELECT @return
	END

EndProc:

	SET	XACT_ABORT OFF
	Set	nocount off
	
	SELECT @return
END













GO
/****** Object:  StoredProcedure [dbo].[IGC_ArcaBattleGuildMarkInfoAllDel]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE PROCEDURE [dbo].[IGC_ArcaBattleGuildMarkInfoAllDel]

AS BEGIN
	DECLARE @ErrorCode int

	SET @ErrorCode = 0

	SET nocount on

	DELETE dbo.IGC_ARCA_BATTLE_GUILDMARK_REG

	IF ( @@Error <> 0 ) BEGIN
		SET @ErrorCode = -1
	END

	SELECT @ErrorCode

	SET nocount off
END













GO
/****** Object:  StoredProcedure [dbo].[IGC_ArcaBattleGuildMemberInsert]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE  procedure [dbo].[IGC_ArcaBattleGuildMemberInsert]
 @GuildName varchar(8),      
 @CharName varchar(10),      
 @Number int
as     
BEGIN  

 DECLARE @return int
 DECLARE @GuildNum int
 SET @return = 0  
     
 Set  nocount on

SELECT @GuildNum = Number FROM IGC_ARCA_BATTLE_GUILD_JOIN_INFO WHERE Number =  @Number
IF( @@ROWCOUNT  < 1 )
BEGIN
   SET  @return = 7
   GOTO EndProc
END

SELECT @GuildNum = Number FROM IGC_ARCA_BATTLE_MEMBER_JOIN_INFO WHERE Number =  @Number
IF( @@ROWCOUNT  >= 30 )
BEGIN
   SET  @return = 9
   GOTO EndProc
END

IF NOT EXISTS ( SELECT CharName FROM IGC_ARCA_BATTLE_MEMBER_JOIN_INFO WITH ( READUNCOMMITTED )       
    WHERE CharName =  @CharName )  
  BEGIN
   begin transaction 
  INSERT INTO IGC_ARCA_BATTLE_MEMBER_JOIN_INFO (G_Name, Number, CharName, JoinDate) VALUES      
   (  @GuildName, @Number, @CharName, GetDate() )
   goto EndProcTran
   END
ELSE
 BEGIN
   SET  @return = 8
   GOTO EndProc
 END

EndProcTran:
	IF ( @@Error  <> 0 )
	BEGIN	
		rollback transaction
		SET @return = -1
		SELECT @return
	END 
	ELSE
	BEGIN
		commit transaction
		SELECT @return
	END

EndProc:

	SET	XACT_ABORT OFF
	Set	nocount off
	
	SELECT @return
END













GO
/****** Object:  StoredProcedure [dbo].[IGC_ArcaBattleGuildMemberSelect]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE PROCEDURE [dbo].[IGC_ArcaBattleGuildMemberSelect]
	@G_Number int
AS BEGIN
	DECLARE @ErrorCode int

	SET @ErrorCode = 0

	SET nocount on

	SET @ErrorCode = (select count (Number)  from IGC_ARCA_BATTLE_MEMBER_JOIN_INFO where Number = @G_Number)

	IF ( @@Error <> 0 ) BEGIN
		SET @ErrorCode = -1
	END

	SELECT @ErrorCode

	SET nocount off
END













GO
/****** Object:  StoredProcedure [dbo].[IGC_ArcaBattleGuildNamesSelect]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE  procedure [dbo].[IGC_ArcaBattleGuildNamesSelect]
as         
BEGIN      
           
 Set  nocount on    

 SELECT G_Name FROM IGC_ARCA_BATTLE_GUILD_JOIN_INFO

 SET XACT_ABORT OFF    
 Set nocount off    
      
END













GO
/****** Object:  StoredProcedure [dbo].[IGC_ArcaBattleGuildSelect]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE  procedure [dbo].[IGC_ArcaBattleGuildSelect]
 @CharName varchar(10)

as     
BEGIN  

 DECLARE @return int
 DECLARE @GuildNum int
 DECLARE @GuildGroupNum tinyint
 SET @return = 0  
     
 Set  nocount on

SELECT @GuildNum = Number FROM IGC_ARCA_BATTLE_GUILD_JOIN_INFO
set @GuildGroupNum = @@ROWCOUNT
IF( @GuildGroupNum >= 6 )
BEGIN
   SET  @return = 3
   GOTO EndProc
END

IF NOT EXISTS ( SELECT Number FROM IGC_ARCA_BATTLE_GUILD_JOIN_INFO WITH ( READUNCOMMITTED )       
    WHERE G_Master = @CharName )  
  BEGIN    
   
   goto EndProcTran
   END
ELSE
 BEGIN
   SET  @return = 4
   GOTO EndProc
 END

EndProcTran:
	IF ( @@Error  <> 0 )
	BEGIN	
		SET @return = -1
		SELECT @return
	END 
	ELSE
	BEGIN
		SELECT @return
	END

EndProc:

	SET	XACT_ABORT OFF
	Set	nocount off
	
	SELECT @return
END













GO
/****** Object:  StoredProcedure [dbo].[IGC_ArcaBattleInfoDelete]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE procedure [dbo].[IGC_ArcaBattleInfoDelete]
as       
BEGIN  
 DECLARE @return int
 SET @return = 0  
 Set  nocount on

 begin transaction  

delete IGC_ARCA_BATTLE_MEMBER_JOIN_INFO
delete IGC_ARCA_BATTLE_GUILD_JOIN_INFO
delete IGC_ARCA_BATTLE_WIN_GUILD_INFO


IF ( @@Error  <> 0 )
 BEGIN	
	rollback transaction
	SET @return = -1
	SELECT @return
 END 
ELSE
 BEGIN
	commit transaction
	SELECT @return
 END
END













GO
/****** Object:  StoredProcedure [dbo].[IGC_ArcaBattleMarkCntSelect]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE  procedure [dbo].[IGC_ArcaBattleMarkCntSelect]
 @G_Number int
as     
BEGIN  

 DECLARE @return int
 DECLARE @GuildRegRank bigint

 SET @return = 0  
     
 Set  nocount on

IF NOT EXISTS ( SELECT G_Number FROM IGC_ARCA_BATTLE_GUILDMARK_REG WITH ( READUNCOMMITTED )       
    WHERE G_Number =  @G_Number)  
  BEGIN

	SET @GuildRegRank = (SELECT count (*) FROM IGC_ARCA_BATTLE_GUILDMARK_REG)
	IF( @GuildRegRank >= 250 )
	 BEGIN
	   SET  @return = -1
	   GOTO EndProc
	 END	  
   END
ELSE
  BEGIN
	set @return = (SELECT MarkCnt FROM IGC_ARCA_BATTLE_GUILDMARK_REG WHERE G_Number =  @G_Number)
  END

EndProc:
	SET	XACT_ABORT OFF
	Set	nocount off	
	select @return
	RETURN
END













GO
/****** Object:  StoredProcedure [dbo].[IGC_ArcaBattleMarkInsert]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE  procedure [dbo].[IGC_ArcaBattleMarkInsert]
 @G_Name varchar(8),      
 @G_Number int,
 @G_Master varchar(10),      
 @MarkCnt bigint
as     
BEGIN  

 DECLARE @return int
 DECLARE @GuildRegRank int
 DECLARE @CurrMarkCnt int

 SET @return = 0  
     
 Set  nocount on


IF NOT EXISTS ( SELECT G_Number FROM IGC_ARCA_BATTLE_GUILDMARK_REG WITH ( READUNCOMMITTED )       
    WHERE G_Number =  @G_Number )  
  BEGIN

	SET @GuildRegRank = (SELECT count (*) FROM IGC_ARCA_BATTLE_GUILDMARK_REG)
	IF( @GuildRegRank >= 250 )
	BEGIN
	   SET  @return = 3
	   GOTO EndProc
	END

   begin transaction      
  INSERT INTO IGC_ARCA_BATTLE_GUILDMARK_REG (G_Name, G_Number, G_Master, RegDate, GuildRegRank, MarkCnt ) VALUES      
   (  @G_Name, @G_Number, @G_Master, GetDate(), (@GuildRegRank+1), @MarkCnt )
   goto EndProcTran

   END
ELSE
 BEGIN
	SET @CurrMarkCnt  = (SELECT MarkCnt from IGC_ARCA_BATTLE_GUILDMARK_REG WHERE G_Number = @G_Number AND G_Name = @G_Name)
	 begin transaction
	 UPDATE IGC_ARCA_BATTLE_GUILDMARK_REG SET MarkCnt = (@MarkCnt + @CurrMarkCnt)  WHERE G_Number = @G_Number AND G_Name = @G_Name
	 SET @return = 1
	goto EndProcTran
 END

EndProcTran:
	IF ( @@Error  <> 0 )
	BEGIN	
		rollback transaction
		SET @return = -1
		select @return
	END 
	ELSE
	BEGIN
		commit transaction
		select @return
	END
	RETURN

EndProc:
	SET	XACT_ABORT OFF
	Set	nocount off	
	select @return
	RETURN
END













GO
/****** Object:  StoredProcedure [dbo].[IGC_ArcaBattleMarkRankSelect]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE PROC [dbo].[IGC_ArcaBattleMarkRankSelect]
	@G_Number		INT
AS
    SET NOCOUNT ON
	SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED
	
	DECLARE @return int
	DECLARE @GuildRegRank int

	SET @return = 0  
	SET @GuildRegRank = 0
	 
	IF NOT EXISTS ( SELECT G_Number FROM dbo.IGC_ARCA_BATTLE_GUILDMARK_REG WHERE G_Number =  @G_Number)  
	BEGIN
		SELECT @return, @GuildRegRank
		RETURN
	END

	DECLARE @Tbl_Rank TABLE 
	(
		mRank		INT IDENTITY(1,1) primary key,
		G_Number	INT,
		mMarkCnt	BIGINT
	)
	
	INSERT INTO @Tbl_Rank (G_Number, mMarkCnt) 
	SELECT G_Number, MarkCnt FROM dbo.IGC_ARCA_BATTLE_GUILDMARK_REG ORDER BY MarkCnt DESC, GuildRegRank ASC
	
	SELECT mRank, mMarkCnt  FROM @Tbl_Rank WHERE G_Number = @G_Number
	
	
	RETURN













GO
/****** Object:  StoredProcedure [dbo].[IGC_ArcaBattleMarkRegDel]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE procedure  [dbo].[IGC_ArcaBattleMarkRegDel]
 @G_Number int
as       
BEGIN  
 DECLARE @return int
 SET @return = 0  
 Set  nocount on

 begin transaction  

 delete dbo.IGC_ARCA_BATTLE_GUILDMARK_REG WHERE G_Number =  @G_Number

IF ( @@Error  <> 0 )
 BEGIN	
	rollback transaction
	SET @return = -1
	SELECT @return
 END 
ELSE
 BEGIN
	commit transaction
	SELECT @return
 END
END













GO
/****** Object:  StoredProcedure [dbo].[IGC_ArcaBattleMemberSelect]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
    
CREATE  procedure [dbo].[IGC_ArcaBattleMemberSelect]  
 @CharName varchar(10)  
as         
BEGIN      
    
 DECLARE @return int    
 SET @return = 0      
         
 Set  nocount on    
      
IF NOT EXISTS ( SELECT CharName FROM IGC_ARCA_BATTLE_MEMBER_JOIN_INFO WITH ( READUNCOMMITTED )           
    WHERE CharName =  @CharName )      
  BEGIN 
 SET  @return = 11    
   END    
    
 SET XACT_ABORT OFF    
 Set nocount off    
     
 SELECT @return    
END













GO
/****** Object:  StoredProcedure [dbo].[IGC_ArcaBattleMemberUnderSelect]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE  procedure [dbo].[IGC_ArcaBattleMemberUnderSelect]
 @GuildName varchar(8)
as         
BEGIN      
 
 Set  nocount on    

 SELECT count (*) FROM IGC_ARCA_BATTLE_MEMBER_JOIN_INFO WHERE G_Name = @GuildName

 SET XACT_ABORT OFF    
 Set nocount off    
      
END













GO
/****** Object:  StoredProcedure [dbo].[IGC_ArcaBattleMinGuildDelete]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE  procedure [dbo].[IGC_ArcaBattleMinGuildDelete]
  @G_Name varchar(8)
as     
BEGIN

 DECLARE @return int
 SET @return = 0

 Set  nocount on
 begin transaction  

 delete IGC_ARCA_BATTLE_GUILD_JOIN_INFO WHERE G_Name = @G_Name
 delete IGC_ARCA_BATTLE_MEMBER_JOIN_INFO WHERE G_Name = @G_Name

IF ( @@Error  <> 0 )
 BEGIN	
	rollback transaction
	SET @return = -1
	SELECT @return
 END 
ELSE
 BEGIN
	commit transaction
	SELECT @return
 END
END













GO
/****** Object:  StoredProcedure [dbo].[IGC_ArcaBattleMinGuildSelect]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE  procedure [dbo].[IGC_ArcaBattleMinGuildSelect]
 @G_Name varchar(8),
 @nMinGuildMemCnt int
as     
BEGIN

 DECLARE @return int
 DECLARE @GuildNum int
 DECLARE @GuildMemCnt int
 SET @return = -1       

 Set  nocount on

 SELECT @GuildNum = Number FROM IGC_ARCA_BATTLE_MEMBER_JOIN_INFO WHERE G_Name = @G_Name
 SET @GuildMemCnt = @@ROWCOUNT
 IF( @GuildMemCnt < @nMinGuildMemCnt )
 BEGIN
   SELECT @return
 END
ELSE
 BEGIN
   SELECT @GuildNum
 END
END













GO
/****** Object:  StoredProcedure [dbo].[IGC_ArcaBattleProcInsert]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE  procedure [dbo].[IGC_ArcaBattleProcInsert]
 @nProcState tinyint
as     
BEGIN  

 DECLARE @return int
 SET @return = 0  
 Set  nocount on

IF NOT EXISTS ( SELECT Proc_State FROM IGC_ARCA_BATTLE_PROC_STATE WITH ( READUNCOMMITTED ) )  
  BEGIN
   begin transaction      
  INSERT INTO IGC_ARCA_BATTLE_PROC_STATE (Proc_State) VALUES  ( @nProcState )
   END
ELSE
 begin transaction      
  UPDATE IGC_ARCA_BATTLE_PROC_STATE SET  Proc_State = @nProcState

IF ( @@Error  <> 0 )
 BEGIN	
	rollback transaction
	SET @return = -1
	SELECT @return
 END 
ELSE
 BEGIN
	commit transaction
	SELECT @return
 END
END













GO
/****** Object:  StoredProcedure [dbo].[IGC_ArcaBattleProcSelect]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE procedure [dbo].[IGC_ArcaBattleProcSelect]    

as         
BEGIN      
      
 SELECT Proc_State FROM IGC_ARCA_BATTLE_PROC_STATE
    
END













GO
/****** Object:  StoredProcedure [dbo].[IGC_ArcaBattleTopRankSelect]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE proc [dbo].[IGC_ArcaBattleTopRankSelect]
AS
	SELECT top 6 G_Name, MarkCnt FROM dbo.IGC_ARCA_BATTLE_GUILDMARK_REG ORDER BY MarkCnt DESC, GuildRegRank ASC













GO
/****** Object:  StoredProcedure [dbo].[IGC_ArcaBattleWinGuildInsert]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
    
CREATE  procedure [dbo].[IGC_ArcaBattleWinGuildInsert]    
 @GuildName varchar(8),        
 @Number int,    
 @nOuccupyObelisk tinyint,    
 @nObeliskGroup tinyint,    
 @nLeftTime bigint    
    
as         
BEGIN      
         
 Set  nocount on    
    
 INSERT INTO IGC_ARCA_BATTLE_WIN_GUILD_INFO (G_Name, G_Number, WinDate, OuccupyObelisk, ObeliskGroup, LeftTime) VALUES         
   (@GuildName, @Number, GetDate(), @nOuccupyObelisk,@nObeliskGroup,  @nLeftTime)    
      
 Set nocount off      
    
END













GO
/****** Object:  StoredProcedure [dbo].[IGC_ArcaBattleWinGuildSelect]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE procedure [dbo].[IGC_ArcaBattleWinGuildSelect]    
as         
BEGIN      
      
 SELECT G_Name, OuccupyObelisk, ObeliskGroup FROM IGC_ARCA_BATTLE_WIN_GUILD_INFO
    
END













GO
/****** Object:  StoredProcedure [dbo].[IGC_BattleCore_CancelUser]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE PROCEDURE [dbo].[IGC_BattleCore_CancelUser]
	@AccountID varchar(10),
	@Name varchar(10),
	@ServerCode int
AS
BEGIN
	SET NOCOUNT ON;

	IF NOT EXISTS (SELECT * FROM [BattleCore].[dbo].BattleCoreInfo WHERE AccountID = @AccountID AND UBFName = @Name AND ServerCode = @ServerCode)
	BEGIN
		SELECT 0 AS Result
		RETURN
	END

	DECLARE @KeyName varchar(10)
	SELECT @KeyName = Name FROM [BattleCore].[dbo].BattleCoreInfo WHERE AccountID = @AccountID AND UBFName = @Name AND ServerCode = @ServerCode

	DECLARE @Slot1 varchar(10), @Slot2 varchar(10), @CurSlot varchar(10)
	SELECT @Slot1 = GameID1, @Slot2 = GameID2 FROM [BattleCore].[dbo].AccountCharacter WHERE Id = @AccountID

	IF (@Slot1 = @KeyName)
	BEGIN
		UPDATE [BattleCore].[dbo].AccountCharacter SET GameID1 = NULL WHERE Id = @AccountID
		PRINT 'Slot1 clear'
	END
	ELSE IF (@Slot2 = @KeyName)
	BEGIN
		UPDATE [BattleCore].[dbo].AccountCharacter SET GameID2 = NULL WHERE Id = @AccountID
		PRINT 'Slot2 clear'
	END

	UPDATE [BattleCore].[dbo].BattleCoreInfo SET RegisterState = 0, RegisterDay = 0, RegisterMonth = 0, LeftTime = GETDATE() WHERE
		AccountID = @AccountID AND UBFName = @Name AND ServerCode = @ServerCode

	SELECT 1 AS Result
END





GO
/****** Object:  StoredProcedure [dbo].[IGC_BattleCore_CopyCharacter_Normal]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE PROCEDURE [dbo].[IGC_BattleCore_CopyCharacter_Normal]
	@AccountID varchar(10),
	@Name varchar(10),
	@ServerCode int
AS
BEGIN
	SET NOCOUNT ON;

	-- prepare data

	IF NOT EXISTS (SELECT * FROM Character WHERE AccountID = @AccountID AND Name = @Name)
	BEGIN
		SELECT 2 AS Result
		RETURN
	END

	DECLARE @Slot1 varchar (10), @Slot2 varchar (10)
	DECLARE @SlotNumber int

	SET @SlotNumber = 0

	SELECT @Slot1 = GameID1, @Slot2 = GameID2 FROM [BattleCore].[dbo].AccountCharacter WHERE Id = @AccountID

	IF (@Slot1 IS NULL)
	BEGIN
		SET @SlotNumber = 1
	END
	ELSE IF (@Slot2 IS NULL)
	BEGIN
		SET @SlotNumber = 2
	END
	ELSE
	BEGIN
		SELECT 3 AS Result
		RETURN
	END
	
	DECLARE @KeyName varchar(10)
	SELECT @KeyName = Name FROM [BattleCore].[dbo].BattleCoreInfo WHERE AccountID = @AccountID AND UBFName = @Name AND ServerCode = @ServerCode

	-- step 1
	-- copy character data

	DECLARE @cLevel int
	DECLARE @LevelUpPoint int
	DECLARE @Class tinyint
	DECLARE @Experience bigint
	DECLARE @Strength int
	DECLARE @Dexterity int
	DECLARE @Vitality int
	DECLARE @Energy int
	DECLARE @MagicList varbinary(450)
	DECLARE @Money int
	DECLARE @Life real
	DECLARE @MaxLife real
	DECLARE @Mana real
	DECLARE @MaxMana real
	DECLARE @MapNumber smallint
	DECLARE @MapPosX smallint
	DECLARE @MapPosY smallint
	DECLARE @MapDir tinyint
	DECLARE @PkCount int
	DECLARE @PkLevel int
	DECLARE @PkTime int
	DECLARE @MDate smalldatetime
	DECLARE @LDate smalldatetime
	DECLARE @CtlCode tinyint
	DECLARE @Quest varbinary(100)
	DECLARE @Leadership int
	DECLARE @ChatLimitTime smallint
	DECLARE @FruitPoint int
	DECLARE @RESETS int
	DECLARE @Inventory varbinary(7584)
	DECLARE @Married int
	DECLARE @MarryName varchar(10)
	DECLARE @MuBotData varbinary(257)
	DECLARE @mLevel int
	DECLARE @mlPoint int
	DECLARE @mlExperience bigint
	DECLARE @mlNextExp bigint
	DECLARE @InventoryExpansion tinyint
	DECLARE @WinDuels int
	DECLARE @LoseDuels int
	DECLARE @PenaltyMask int
	DECLARE @BlockChatTime bigint

	DECLARE CUR CURSOR FOR SELECT [cLevel], [LevelUpPoint], [Class], [Experience], [Strength], [Dexterity], [Vitality], [Energy], [MagicList], 
		[Money], [Life], [MaxLife], [Mana], [MaxMana], [MapNumber], [MapPosX], [MapPosY], [MapDir], [PkCount], [PkLevel], [PkTime], 
		[MDate], [LDate], [CtlCode], [Quest], [Leadership], [ChatLimitTime], [FruitPoint], [RESETS], [Inventory], [Married],
		[MarryName], [MuBotData], [mLevel], [mlPoint], [mlExperience], [mlNextExp], [InventoryExpansion], [WinDuels], [LoseDuels], 
		[PenaltyMask], [BlockChatTime] FROM Character WHERE AccountID = @AccountID AND Name = @Name
	OPEN CUR
	FETCH NEXT FROM CUR INTO @cLevel, @LevelUpPoint, @Class, @Experience, @Strength, @Dexterity, @Vitality, @Energy, @MagicList,
		@Money, @Life, @MaxLife, @Mana, @MaxMana, @MapNumber, @MapPosX, @MapPosY, @MapDir, @PkCount, @PkLevel, @PkTime,
		@MDate, @LDate, @CtlCode, @Quest, @Leadership, @ChatLimitTime, @FruitPoint, @RESETS, @Inventory, @Married,
		@MarryName, @MuBotData, @mLevel, @mlPoint, @mlExperience, @mlNextExp, @InventoryExpansion, @WinDuels, @LoseDuels,
		@PenaltyMask, @BlockChatTime

	INSERT INTO [BattleCore].[dbo].Character (AccountID, Name, cLevel, LevelUpPoint, Class, Experience, Strength, Dexterity, Vitality, Energy, MagicList,
		Money, Life, MaxLife, Mana, MaxMana, MapNumber, MapPosX, MapPosY, MapDir, PkCount, PkLevel, PkTime, MDate, LDate, CtlCode, Quest,
		Leadership, ChatLimitTime, FruitPoint, RESETS, Inventory, Married, MarryName, MuBotData, mLevel, mlPoint, mlExperience, mlNextExp,
		InventoryExpansion, WinDuels, LoseDuels, PenaltyMask, BlockChatTime) 
		VALUES (@AccountID, @KeyName, @cLevel, @LevelUpPoint, @Class, @Experience, @Strength, @Dexterity, @Vitality, @Energy, @MagicList,
		100000000, @Life, @MaxLife, @Mana, @MaxMana, @MapNumber, @MapPosX, @MapPosY, @MapDir, @PkCount, @PkLevel, @PkTime, @MDate, @LDate, @CtlCode, @Quest,
		@Leadership, @ChatLimitTime, @FruitPoint, @RESETS, @Inventory, @Married, @MarryName, @MuBotData, @mLevel, @mlPoint, @mlExperience, @mlNextExp,
		@InventoryExpansion, @WinDuels, @LoseDuels, @PenaltyMask, @BlockChatTime)

	CLOSE CUR
	DEALLOCATE CUR

	-- step 2
	-- copy event inventory

	DECLARE @EventInventory varbinary(1024)
	SELECT @EventInventory = Inventory FROM T_Event_Inventory WHERE AccountID = @AccountID AND Name = @Name

	INSERT INTO [BattleCore].[dbo].T_Event_Inventory (AccountID, Name, Inventory) VALUES (@AccountID, @KeyName, @EventInventory)

	-- step 3
	-- copy muun inventory

	DECLARE @MuunInventory varbinary(1984)
	DECLARE @MuunDbVersion tinyint
	DECLARE @MuunSubEquip tinyint

	DECLARE CUR CURSOR FOR SELECT [Items], [DbVersion], [SubEquip] FROM PetWarehouse WHERE AccountID = @AccountID AND Name = @Name
	OPEN CUR
	FETCH NEXT FROM CUR INTO @MuunInventory, @MuunDbVersion, @MuunSubEquip

	INSERT INTO [BattleCore].[dbo].PetWarehouse (AccountID, Name, Items, DbVersion, SubEquip) VALUES (@AccountID, @KeyName, @MuunInventory, @MuunDbVersion, @MuunSubEquip)

	CLOSE CUR
	DEALLOCATE CUR

	-- step 4
	-- copy pentagram item data

	DECLARE @UserGuid int
	DECLARE @JewelPos tinyint
	DECLARE @JewelIndex smallint
	DECLARE @ItemType smallint
	DECLARE @ItemIndex int
	DECLARE @MainAttribute smallint
	DECLARE @JewelLevel smallint
	DECLARE @Rank1 tinyint
	DECLARE @Rank1Level tinyint
	DECLARE @Rank2 tinyint
	DECLARE @Rank2Level tinyint
	DECLARE @Rank3 tinyint
	DECLARE @Rank3Level tinyint
	DECLARE @Rank4 tinyint
	DECLARE @Rank4Level tinyint
	DECLARE @Rank5 tinyint
	DECLARE @Rank5Level tinyint
	DECLARE @RegDate datetime

	DECLARE CUR CURSOR FOR SELECT [UserGuid], [JewelPos], [JewelIndex], [ItemType], [ItemIndex], [MainAttribute], [JewelLevel], 
		[Rank1], [Rank1Level], [Rank2], [Rank2Level], [Rank3], [Rank3Level], [Rank4], [Rank4Level], [Rank5], [Rank5Level] FROM T_PentagramInfo WHERE
		AccountID = @AccountID AND Name = @Name
	OPEN CUR
	FETCH NEXT FROM CUR INTO @UserGuid, @JewelPos, @JewelIndex, @ItemType, @ItemIndex, @MainAttribute, @JewelLevel,
		@Rank1, @Rank1Level, @Rank2, @Rank2Level, @Rank3, @Rank3Level, @Rank4, @Rank4Level, @Rank5, @Rank5Level

	WHILE( @@fetch_status <> -1 )
	BEGIN
		IF( @@fetch_status <> -2 )
		BEGIN
			INSERT INTO [BattleCore].[dbo].T_PentagramInfo (UserGuid, AccountID, Name, JewelPos, JewelIndex, ItemType, ItemIndex, MainAttribute, JewelLevel,
				Rank1, Rank1Level, Rank2, Rank2Level, Rank3, Rank3Level, Rank4, Rank4Level, Rank5, Rank5Level) VALUES (@UserGuid, @AccountID, @KeyName,
				@JewelPos, @JewelIndex, @ItemType, @ItemIndex, @MainAttribute, @JewelLevel, @Rank1, @Rank1Level, @Rank2, @Rank2Level, @Rank3,
				@Rank3Level, @Rank4, @Rank4Level, @Rank5, @Rank5Level)
		END
		FETCH NEXT FROM CUR INTO @UserGuid, @JewelPos, @JewelIndex, @ItemType, @ItemIndex, @MainAttribute, @JewelLevel,
			@Rank1, @Rank1Level, @Rank2, @Rank2Level, @Rank3, @Rank3Level, @Rank4, @Rank4Level, @Rank5, @Rank5Level
	END

	CLOSE CUR
	DEALLOCATE CUR

	-- step 5
	-- copy character option data (q,w,e,r etc.)

	DECLARE @SkillKey binary(20)
	DECLARE @GameOption tinyint
	DECLARE @QKey tinyint
	DECLARE @WKey tinyint
	DECLARE @EKey tinyint
	DECLARE @ChatWindow tinyint
	DECLARE @RKey tinyint
	DECLARE @QWERLevel int
	DECLARE @EnableChangeMode tinyint

	DECLARE CUR CURSOR FOR SELECT [SkillKey], [GameOption], [QKey], [WKey], [EKey], [ChatWindow], [RKey], [QWERLevel], [EnableChangeMode] FROM OptionData WHERE Name = @Name
	OPEN CUR
	FETCH NEXT FROM CUR INTO @SkillKey, @GameOption, @QKey, @WKey, @EKey, @ChatWindow, @RKey, @QWERLevel, @EnableChangeMode

	INSERT INTO [BattleCore].[dbo].OptionData (Name, SkillKey, GameOption, Qkey, Wkey, Ekey, ChatWindow, Rkey, QWERLevel, EnableChangeMode) VALUES
		(@KeyName, @SkillKey, @GameOption, @QKey, @WKey, @EKey, @ChatWindow, @RKey, @QWERLevel, @EnableChangeMode)

	CLOSE CUR
	DEALLOCATE CUR

	-- step 6.1
	-- copy period item data (cash-shop)

	DECLARE @UserGUID_Period int
	DECLARE @PeriodItemType smallint
	DECLARE @PeriodItemCode smallint
	DECLARE @PeriodEffectCategory smallint
	DECLARE @PeriodEffectType1 smallint
	DECLARE @PeriodEffectType2 smallint
	DECLARE @PeriodSerial bigint
	DECLARE @PeriodDuration int
	DECLARE @PeriodBuyDate bigint
	DECLARE @PeriodExpireDate bigint
	DECLARE @PeriodBuyDateString smalldatetime
	DECLARE @PeriodExpireDateString smalldatetime
	DECLARE @PeriodUsedInfo int
	DECLARE @PeriodSetExpire int

	DECLARE CUR CURSOR FOR SELECT [UserGUID], [ItemType], [ItemCode], [EffectCategory], [EffectType1], [EffectType2], [Serial], [Duration], [BuyDate], [ExpireDate], [UsedInfo], [BuyDateString], [ExpireDateString], [SetExpire]
	FROM IGC_PeriodItemInfo WHERE CharacterName = @Name
	OPEN CUR
	FETCH NEXT FROM CUR INTO @UserGUID_Period, @PeriodItemType, @PeriodItemCode, @PeriodEffectCategory, @PeriodEffectType1, @PeriodEffectType2, @PeriodSerial, @PeriodDuration,
	@PeriodBuyDate, @PeriodExpireDate, @PeriodUsedInfo, @PeriodBuyDateString, @PeriodExpireDateString, @PeriodSetExpire

	WHILE( @@fetch_status  <> -1 )
	BEGIN
		IF( @@fetch_status  <> -2 )
		BEGIN
			INSERT INTO [BattleCore].[dbo].IGC_PeriodItemInfo (UserGUID, CharacterName, ItemType, ItemCode, EffectCategory, EffectType1, EffectType2, Serial, Duration, BuyDate, ExpireDate, UsedInfo, BuyDateString, ExpireDateString, SetExpire)
			VALUES (@UserGUID_Period, @KeyName, @PeriodItemType, @PeriodItemCode, @PeriodEffectCategory, @PeriodEffectType1, @PeriodEffectType2, @PeriodSerial, @PeriodDuration, @PeriodBuyDate, @PeriodExpireDate, @PeriodUsedInfo, @PeriodBuyDateString, @PeriodExpireDateString, @PeriodSetExpire)
		END

		FETCH NEXT FROM CUR INTO @UserGUID_Period, @PeriodItemType, @PeriodItemCode, @PeriodEffectCategory, @PeriodEffectType1, @PeriodEffectType2, @PeriodSerial, @PeriodDuration,
		@PeriodBuyDate, @PeriodExpireDate, @PeriodUsedInfo, @PeriodBuyDateString, @PeriodExpireDateString, @PeriodSetExpire
	END

	CLOSE CUR
	DEALLOCATE CUR

	-- step 6.2
	-- copy period item data (arca buffs)
	DECLARE @PeriodBuffIndex smallint
	SET @PeriodEffectType1 = 0
	SET @PeriodEffectType2 = 0
	SET @PeriodExpireDate = 0
	SET @PeriodDuration = 0

	DECLARE CUR CURSOR FOR SELECT [BuffIndex], [EffectType1], [EffectType2], [ExpireDate], [Duration] FROM IGC_PeriodBuffInfo WHERE CharacterName = @Name
	OPEN CUR
	FETCH NEXT FROM CUR INTO @PeriodBuffIndex, @PeriodEffectType1, @PeriodEffectType2, @PeriodExpireDate, @PeriodDuration

	WHILE( @@fetch_status <> -1 )
	BEGIN
		IF( @@fetch_status <> -2 )
		BEGIN
			INSERT INTO [BattleCore].[dbo].IGC_PeriodBuffInfo (CharacterName, BuffIndex, EffectType1, EffectType2, ExpireDate, Duration) VALUES
			(@KeyName, @PeriodBuffIndex, @PeriodEffectType1, @PeriodEffectType2, @PeriodExpireDate, @PeriodDuration)
		END

		FETCH NEXT FROM CUR INTO @PeriodBuffIndex, @PeriodEffectType1, @PeriodEffectType2, @PeriodExpireDate, @PeriodDuration
	END

	CLOSE CUR
	DEALLOCATE CUR

	-- step 7
	-- copy lucky item data

	DECLARE @LuckyUserGuid int
	DECLARE @LuckyItemCode int
	DECLARE @LuckyItemSerial bigint
	DECLARE @LuckyDurabilitySmall smallint

	DECLARE CUR CURSOR FOR SELECT [UserGuid], [ItemCode], [ItemSerial], [DurabilitySmall] FROM T_LUCKY_ITEM_INFO WHERE CharName = @Name
	OPEN CUR
	FETCH NEXT FROM CUR INTO @LuckyUserGuid, @LuckyItemCode, @LuckyItemSerial, @LuckyDurabilitySmall

	WHILE( @@fetch_status <> -1 )
	BEGIN
		IF( @@fetch_status <> -2 )
		BEGIN
			INSERT INTO [BattleCore].[dbo].T_LUCKY_ITEM_INFO (UserGuid, CharName, ItemCode, ItemSerial, DurabilitySmall) VALUES
			(@LuckyUserGuid, @KeyName, @LuckyItemCode, @LuckyItemSerial, @LuckyDurabilitySmall)
		END

		FETCH NEXT FROM CUR INTO @LuckyUserGuid, @LuckyItemCode, @LuckyItemSerial, @LuckyDurabilitySmall
	END

	CLOSE CUR
	DEALLOCATE CUR

	-- step 8
	-- copy gm data

	DECLARE @AuthorityMask int
	DECLARE @Expiry smalldatetime

	SELECT @AuthorityMask = AuthorityMask, @Expiry = Expiry FROM T_GMSystem WHERE Name = @Name

	INSERT INTO [BattleCore].[dbo].T_GMSystem (Name, AuthorityMask, Expiry) VALUES (@KeyName, @AuthorityMask, @Expiry)


	-- step 9
	-- set character in AccountCharacter

	IF (@SlotNumber = 1)
	BEGIN
		UPDATE [BattleCore].[dbo].AccountCharacter SET GameID1 = @KeyName WHERE Id = @AccountID
	END
	ELSE IF (@SlotNumber = 2)
	BEGIN
		UPDATE [BattleCore].[dbo].AccountCharacter SET GameID2 = @KeyName WHERE Id = @AccountID
	END

	SELECT 1 AS Result
END





GO
/****** Object:  StoredProcedure [dbo].[IGC_BattleCore_CopyCharacter_Promotion]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE PROCEDURE [dbo].[IGC_BattleCore_CopyCharacter_Promotion]
	@AccountID varchar(10),
	@Name varchar(10),
	@ServerCode int
AS
BEGIN
	SET NOCOUNT ON;

	-- prepare data

	IF NOT EXISTS (SELECT * FROM Character WHERE AccountID = @AccountID AND Name = @Name)
	BEGIN
		SELECT 2 AS Result
		RETURN
	END

	DECLARE @Slot1 varchar (10), @Slot2 varchar (10)
	DECLARE @SlotNumber int

	SET @SlotNumber = 0

	SELECT @Slot1 = GameID1, @Slot2 = GameID2 FROM [BattleCore].[dbo].AccountCharacter WHERE Id = @AccountID

	IF (@Slot1 IS NULL)
	BEGIN
		SET @SlotNumber = 1
	END
	ELSE IF (@Slot2 IS NULL)
	BEGIN
		SET @SlotNumber = 2
	END
	ELSE
	BEGIN
		SELECT 3 AS Result
		RETURN
	END
	
	DECLARE @KeyName varchar(10)
	SELECT @KeyName = Name FROM [BattleCore].[dbo].BattleCoreInfo WHERE AccountID = @AccountID AND UBFName = @Name AND ServerCode = @ServerCode

	-- step 1
	-- copy character data

	DECLARE @cLevel int
	DECLARE @LevelUpPoint int
	DECLARE @Class tinyint
	DECLARE @Experience bigint
	DECLARE @Strength int
	DECLARE @Dexterity int
	DECLARE @Vitality int
	DECLARE @Energy int
	DECLARE @MagicList varbinary(450)
	DECLARE @Money int
	DECLARE @Life real
	DECLARE @MaxLife real
	DECLARE @Mana real
	DECLARE @MaxMana real
	DECLARE @MapNumber smallint
	DECLARE @MapPosX smallint
	DECLARE @MapPosY smallint
	DECLARE @MapDir tinyint
	DECLARE @PkCount int
	DECLARE @PkLevel int
	DECLARE @PkTime int
	DECLARE @MDate smalldatetime
	DECLARE @LDate smalldatetime
	DECLARE @CtlCode tinyint
	DECLARE @Quest varbinary(100)
	DECLARE @Leadership int
	DECLARE @ChatLimitTime smallint
	DECLARE @FruitPoint int
	DECLARE @RESETS int
	DECLARE @Inventory varbinary(7584)
	DECLARE @Married int
	DECLARE @MarryName varchar(10)
	DECLARE @MuBotData varbinary(257)
	DECLARE @mLevel int
	DECLARE @mlPoint int
	DECLARE @mlExperience bigint
	DECLARE @mlNextExp bigint
	DECLARE @InventoryExpansion tinyint
	DECLARE @WinDuels int
	DECLARE @LoseDuels int
	DECLARE @PenaltyMask int
	DECLARE @BlockChatTime bigint

	DECLARE CUR CURSOR FOR SELECT [cLevel], [LevelUpPoint], [Class], [Experience], [Strength], [Dexterity], [Vitality], [Energy], [MagicList], 
		[Money], [Life], [MaxLife], [Mana], [MaxMana], [MapNumber], [MapPosX], [MapPosY], [MapDir], [PkCount], [PkLevel], [PkTime], 
		[MDate], [LDate], [CtlCode], [Quest], [Leadership], [ChatLimitTime], [FruitPoint], [RESETS], [Inventory], [Married],
		[MarryName], [MuBotData], [mLevel], [mlPoint], [mlExperience], [mlNextExp], [InventoryExpansion], [WinDuels], [LoseDuels], 
		[PenaltyMask], [BlockChatTime] FROM Character WHERE AccountID = @AccountID AND Name = @Name
	OPEN CUR
	FETCH NEXT FROM CUR INTO @cLevel, @LevelUpPoint, @Class, @Experience, @Strength, @Dexterity, @Vitality, @Energy, @MagicList,
		@Money, @Life, @MaxLife, @Mana, @MaxMana, @MapNumber, @MapPosX, @MapPosY, @MapDir, @PkCount, @PkLevel, @PkTime,
		@MDate, @LDate, @CtlCode, @Quest, @Leadership, @ChatLimitTime, @FruitPoint, @RESETS, @Inventory, @Married,
		@MarryName, @MuBotData, @mLevel, @mlPoint, @mlExperience, @mlNextExp, @InventoryExpansion, @WinDuels, @LoseDuels,
		@PenaltyMask, @BlockChatTime

	INSERT INTO [BattleCore].[dbo].Character (AccountID, Name, cLevel, LevelUpPoint, Class, Experience, Strength, Dexterity, Vitality, Energy, MagicList,
		Money, Life, MaxLife, Mana, MaxMana, MapNumber, MapPosX, MapPosY, MapDir, PkCount, PkLevel, PkTime, MDate, LDate, CtlCode, Quest,
		Leadership, ChatLimitTime, FruitPoint, RESETS, Inventory, Married, MarryName, MuBotData, mLevel, mlPoint, mlExperience, mlNextExp,
		InventoryExpansion, WinDuels, LoseDuels, PenaltyMask, BlockChatTime) 
		VALUES (@AccountID, @KeyName, @cLevel, @LevelUpPoint, @Class, @Experience, @Strength, @Dexterity, @Vitality, @Energy, @MagicList,
		@Money, @Life, @MaxLife, @Mana, @MaxMana, @MapNumber, @MapPosX, @MapPosY, @MapDir, @PkCount, @PkLevel, @PkTime, @MDate, @LDate, @CtlCode, @Quest,
		@Leadership, @ChatLimitTime, @FruitPoint, @RESETS, @Inventory, @Married, @MarryName, @MuBotData, @mLevel, @mlPoint, @mlExperience, @mlNextExp,
		@InventoryExpansion, @WinDuels, @LoseDuels, @PenaltyMask, @BlockChatTime)

	CLOSE CUR
	DEALLOCATE CUR

	-- step 2
	-- copy event inventory

	DECLARE @EventInventory varbinary(1024)
	SELECT @EventInventory = Inventory FROM T_Event_Inventory WHERE AccountID = @AccountID AND Name = @Name

	INSERT INTO [BattleCore].[dbo].T_Event_Inventory (AccountID, Name, Inventory) VALUES (@AccountID, @KeyName, @EventInventory)

	-- step 3
	-- copy muun inventory

	DECLARE @MuunInventory varbinary(1984)
	DECLARE @MuunDbVersion tinyint
	DECLARE @MuunSubEquip tinyint

	DECLARE CUR CURSOR FOR SELECT [Items], [DbVersion], [SubEquip] FROM PetWarehouse WHERE AccountID = @AccountID AND Name = @Name
	OPEN CUR
	FETCH NEXT FROM CUR INTO @MuunInventory, @MuunDbVersion, @MuunSubEquip

	INSERT INTO [BattleCore].[dbo].PetWarehouse (AccountID, Name, Items, DbVersion, SubEquip) VALUES (@AccountID, @KeyName, @MuunInventory, @MuunDbVersion, @MuunSubEquip)

	CLOSE CUR
	DEALLOCATE CUR

	-- step 4
	-- copy pentagram item data

	DECLARE @UserGuid int
	DECLARE @JewelPos tinyint
	DECLARE @JewelIndex smallint
	DECLARE @ItemType smallint
	DECLARE @ItemIndex int
	DECLARE @MainAttribute smallint
	DECLARE @JewelLevel smallint
	DECLARE @Rank1 tinyint
	DECLARE @Rank1Level tinyint
	DECLARE @Rank2 tinyint
	DECLARE @Rank2Level tinyint
	DECLARE @Rank3 tinyint
	DECLARE @Rank3Level tinyint
	DECLARE @Rank4 tinyint
	DECLARE @Rank4Level tinyint
	DECLARE @Rank5 tinyint
	DECLARE @Rank5Level tinyint
	DECLARE @RegDate datetime

	DECLARE CUR CURSOR FOR SELECT [UserGuid], [JewelPos], [JewelIndex], [ItemType], [ItemIndex], [MainAttribute], [JewelLevel], 
		[Rank1], [Rank1Level], [Rank2], [Rank2Level], [Rank3], [Rank3Level], [Rank4], [Rank4Level], [Rank5], [Rank5Level] FROM T_PentagramInfo WHERE
		AccountID = @AccountID AND Name = @Name
	OPEN CUR
	FETCH NEXT FROM CUR INTO @UserGuid, @JewelPos, @JewelIndex, @ItemType, @ItemIndex, @MainAttribute, @JewelLevel,
		@Rank1, @Rank1Level, @Rank2, @Rank2Level, @Rank3, @Rank3Level, @Rank4, @Rank4Level, @Rank5, @Rank5Level

	WHILE( @@fetch_status <> -1 )
	BEGIN
		IF( @@fetch_status <> -2 )
		BEGIN
			INSERT INTO [BattleCore].[dbo].T_PentagramInfo (UserGuid, AccountID, Name, JewelPos, JewelIndex, ItemType, ItemIndex, MainAttribute, JewelLevel,
				Rank1, Rank1Level, Rank2, Rank2Level, Rank3, Rank3Level, Rank4, Rank4Level, Rank5, Rank5Level) VALUES (@UserGuid, @AccountID, @KeyName,
				@JewelPos, @JewelIndex, @ItemType, @ItemIndex, @MainAttribute, @JewelLevel, @Rank1, @Rank1Level, @Rank2, @Rank2Level, @Rank3,
				@Rank3Level, @Rank4, @Rank4Level, @Rank5, @Rank5Level)
		END
		FETCH NEXT FROM CUR INTO @UserGuid, @JewelPos, @JewelIndex, @ItemType, @ItemIndex, @MainAttribute, @JewelLevel,
			@Rank1, @Rank1Level, @Rank2, @Rank2Level, @Rank3, @Rank3Level, @Rank4, @Rank4Level, @Rank5, @Rank5Level
	END

	CLOSE CUR
	DEALLOCATE CUR

	-- step 5
	-- copy character option data (q,w,e,r etc.)

	DECLARE @SkillKey binary(20)
	DECLARE @GameOption tinyint
	DECLARE @QKey tinyint
	DECLARE @WKey tinyint
	DECLARE @EKey tinyint
	DECLARE @ChatWindow tinyint
	DECLARE @RKey tinyint
	DECLARE @QWERLevel int
	DECLARE @EnableChangeMode tinyint

	DECLARE CUR CURSOR FOR SELECT [SkillKey], [GameOption], [QKey], [WKey], [EKey], [ChatWindow], [RKey], [QWERLevel], [EnableChangeMode] FROM OptionData WHERE Name = @Name
	OPEN CUR
	FETCH NEXT FROM CUR INTO @SkillKey, @GameOption, @QKey, @WKey, @EKey, @ChatWindow, @RKey, @QWERLevel, @EnableChangeMode

	INSERT INTO [BattleCore].[dbo].OptionData (Name, SkillKey, GameOption, Qkey, Wkey, Ekey, ChatWindow, Rkey, QWERLevel, EnableChangeMode) VALUES
		(@KeyName, @SkillKey, @GameOption, @QKey, @WKey, @EKey, @ChatWindow, @RKey, @QWERLevel, @EnableChangeMode)

	CLOSE CUR
	DEALLOCATE CUR

	-- step 6.1
	-- copy period item data (cash-shop)

	DECLARE @UserGUID_Period int
	DECLARE @PeriodItemType smallint
	DECLARE @PeriodItemCode smallint
	DECLARE @PeriodEffectCategory smallint
	DECLARE @PeriodEffectType1 smallint
	DECLARE @PeriodEffectType2 smallint
	DECLARE @PeriodSerial bigint
	DECLARE @PeriodDuration int
	DECLARE @PeriodBuyDate bigint
	DECLARE @PeriodExpireDate bigint
	DECLARE @PeriodBuyDateString smalldatetime
	DECLARE @PeriodExpireDateString smalldatetime
	DECLARE @PeriodUsedInfo int
	DECLARE @PeriodSetExpire int

	DECLARE CUR CURSOR FOR SELECT [UserGUID], [ItemType], [ItemCode], [EffectCategory], [EffectType1], [EffectType2], [Serial], [Duration], [BuyDate], [ExpireDate], [UsedInfo], [BuyDateString], [ExpireDateString], [SetExpire]
	FROM IGC_PeriodItemInfo WHERE CharacterName = @Name
	OPEN CUR
	FETCH NEXT FROM CUR INTO @UserGUID_Period, @PeriodItemType, @PeriodItemCode, @PeriodEffectCategory, @PeriodEffectType1, @PeriodEffectType2, @PeriodSerial, @PeriodDuration,
	@PeriodBuyDate, @PeriodExpireDate, @PeriodUsedInfo, @PeriodBuyDateString, @PeriodExpireDateString, @PeriodSetExpire

	WHILE( @@fetch_status  <> -1 )
	BEGIN
		IF( @@fetch_status  <> -2 )
		BEGIN
			INSERT INTO [BattleCore].[dbo].IGC_PeriodItemInfo (UserGUID, CharacterName, ItemType, ItemCode, EffectCategory, EffectType1, EffectType2, Serial, Duration, BuyDate, ExpireDate, UsedInfo, BuyDateString, ExpireDateString, SetExpire)
			VALUES (@UserGUID_Period, @KeyName, @PeriodItemType, @PeriodItemCode, @PeriodEffectCategory, @PeriodEffectType1, @PeriodEffectType2, @PeriodSerial, @PeriodDuration, @PeriodBuyDate, @PeriodExpireDate, @PeriodUsedInfo, @PeriodBuyDateString, @PeriodExpireDateString, @PeriodSetExpire)
		END

		FETCH NEXT FROM CUR INTO @UserGUID_Period, @PeriodItemType, @PeriodItemCode, @PeriodEffectCategory, @PeriodEffectType1, @PeriodEffectType2, @PeriodSerial, @PeriodDuration,
		@PeriodBuyDate, @PeriodExpireDate, @PeriodUsedInfo, @PeriodBuyDateString, @PeriodExpireDateString, @PeriodSetExpire
	END

	CLOSE CUR
	DEALLOCATE CUR

	-- step 6.2
	-- copy period item data (arca buffs)
	DECLARE @PeriodBuffIndex smallint
	SET @PeriodEffectType1 = 0
	SET @PeriodEffectType2 = 0
	SET @PeriodExpireDate = 0
	SET @PeriodDuration = 0

	DECLARE CUR CURSOR FOR SELECT [BuffIndex], [EffectType1], [EffectType2], [ExpireDate], [Duration] FROM IGC_PeriodBuffInfo WHERE CharacterName = @Name
	OPEN CUR
	FETCH NEXT FROM CUR INTO @PeriodBuffIndex, @PeriodEffectType1, @PeriodEffectType2, @PeriodExpireDate, @PeriodDuration

	WHILE( @@fetch_status <> -1 )
	BEGIN
		IF( @@fetch_status <> -2 )
		BEGIN
			INSERT INTO [BattleCore].[dbo].IGC_PeriodBuffInfo (CharacterName, BuffIndex, EffectType1, EffectType2, ExpireDate, Duration) VALUES
			(@KeyName, @PeriodBuffIndex, @PeriodEffectType1, @PeriodEffectType2, @PeriodExpireDate, @PeriodDuration)
		END

		FETCH NEXT FROM CUR INTO @PeriodBuffIndex, @PeriodEffectType1, @PeriodEffectType2, @PeriodExpireDate, @PeriodDuration
	END

	CLOSE CUR
	DEALLOCATE CUR

	-- step 7
	-- copy lucky item data

	DECLARE @LuckyUserGuid int
	DECLARE @LuckyItemCode int
	DECLARE @LuckyItemSerial bigint
	DECLARE @LuckyDurabilitySmall smallint

	DECLARE CUR CURSOR FOR SELECT [UserGuid], [ItemCode], [ItemSerial], [DurabilitySmall] FROM T_LUCKY_ITEM_INFO WHERE CharName = @Name
	OPEN CUR
	FETCH NEXT FROM CUR INTO @LuckyUserGuid, @LuckyItemCode, @LuckyItemSerial, @LuckyDurabilitySmall

	WHILE( @@fetch_status <> -1 )
	BEGIN
		IF( @@fetch_status <> -2 )
		BEGIN
			INSERT INTO [BattleCore].[dbo].T_LUCKY_ITEM_INFO (UserGuid, CharName, ItemCode, ItemSerial, DurabilitySmall) VALUES
			(@LuckyUserGuid, @KeyName, @LuckyItemCode, @LuckyItemSerial, @LuckyDurabilitySmall)
		END

		FETCH NEXT FROM CUR INTO @LuckyUserGuid, @LuckyItemCode, @LuckyItemSerial, @LuckyDurabilitySmall
	END

	CLOSE CUR
	DEALLOCATE CUR

	-- step 8
	-- copy gm data
	
	DECLARE @AuthorityMask int
	DECLARE @Expiry smalldatetime

	SELECT @AuthorityMask = AuthorityMask, @Expiry = Expiry FROM T_GMSystem WHERE Name = @Name

	INSERT INTO [BattleCore].[dbo].T_GMSystem (Name, AuthorityMask, Expiry) VALUES (@KeyName, @AuthorityMask, @Expiry)


	-- step 9
	-- set character in AccountCharacter

	IF (@SlotNumber = 1)
	BEGIN
		UPDATE [BattleCore].[dbo].AccountCharacter SET GameID1 = @KeyName WHERE Id = @AccountID
	END
	ELSE IF (@SlotNumber = 2)
	BEGIN
		UPDATE [BattleCore].[dbo].AccountCharacter SET GameID2 = @KeyName WHERE Id = @AccountID
	END

	SELECT 1 AS Result
END





GO
/****** Object:  StoredProcedure [dbo].[IGC_BattleCore_CopyPetItemInfo]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE PROCEDURE [dbo].[IGC_BattleCore_CopyPetItemInfo]
	@PetItemSerial bigint,
	@ServerCode int
AS
BEGIN
	SET NOCOUNT ON;

	DECLARE @ItemSerial bigint, @Pet_Level smallint, @Pet_Exp int
	SELECT @ItemSerial = ItemSerial, @Pet_Level = Pet_Level, @Pet_Exp = Pet_Exp FROM T_PetItem_Info WHERE ItemSerial = @PetItemSerial

	IF EXISTS (SELECT * FROM [BattleCore].[dbo].BattleCore_PetItem_Info WHERE ItemSerial = @ItemSerial AND ServerCode = @ServerCode)
	BEGIN
		DELETE FROM [BattleCore].[dbo].BattleCore_PetItem_Info WHERE ItemSerial = @ItemSerial AND ServerCode = @ServerCode
	END

	INSERT INTO [BattleCore].[dbo].BattleCore_PetItem_Info (ItemSerial, ServerCode, Pet_Level, Pet_Exp) VALUES (@PetItemSerial, @ServerCode, @Pet_Level, @Pet_Exp)
END





GO
/****** Object:  StoredProcedure [dbo].[IGC_BattleCore_DeleteCharacter]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE PROCEDURE [dbo].[IGC_BattleCore_DeleteCharacter]
	@AccountID varchar(10),
	@Name varchar(10),
	@ServerCode int
AS
BEGIN
	SET NOCOUNT ON;

	IF NOT EXISTS (SELECT * FROM [BattleCore].[dbo].BattleCoreInfo WHERE AccountID = @AccountID AND UBFName = @Name AND ServerCode = @ServerCode)
	BEGIN
		SELECT 0 AS Result
		RETURN
	END

	DECLARE @KeyName varchar(10)
	SELECT @KeyName = Name FROM [BattleCore].[dbo].BattleCoreInfo WHERE AccountID = @AccountID AND UBFName = @Name AND ServerCode = @ServerCode

	DELETE FROM [BattleCore].[dbo].Character WHERE AccountID = @AccountID AND Name = @KeyName
	DELETE FROM [BattleCore].[dbo].IGC_PeriodBuffInfo WHERE CharacterName = @KeyName
	DELETE FROM [BattleCore].[dbo].IGC_PeriodItemInfo WHERE CharacterName = @KeyName
	DELETE FROM [BattleCore].[dbo].OptionData WHERE Name = @KeyName
	DELETE FROM [BattleCore].[dbo].PetWarehouse WHERE Name = @KeyName
	DELETE FROM [BattleCore].[dbo].T_Event_Inventory WHERE Name = @KeyName
	DELETE FROM [BattleCore].[dbo].T_PentagramInfo WHERE Name = @KeyName
	DELETE FROM [BattleCore].[dbo].T_LUCKY_ITEM_INFO WHERE CharName = @KeyName

	SELECT 1 AS Result
END





GO
/****** Object:  StoredProcedure [dbo].[IGC_BattleCore_GetCCFReward]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE PROCEDURE [dbo].[IGC_BattleCore_GetCCFReward]
	@AccountID varchar(10),
	@Name varchar(10),
	@ServerCode int
AS
BEGIN
	SET NOCOUNT ON;

	IF NOT EXISTS (SELECT * FROM [BattleCore].[dbo].BattleCoreInfo WHERE AccountID = @AccountID AND UBFName = @Name AND ServerCode = @ServerCode)
	BEGIN
		SELECT 2 AS Result
		RETURN
	END

	DECLARE @KeyName varchar(10)
	SELECT @KeyName = Name FROM [BattleCore].[dbo].BattleCoreInfo WHERE AccountID = @AccountID AND UBFName = @Name AND ServerCode = @ServerCode

	SELECT 1 AS Result, * FROM [BattleCore].[dbo].BattleCore_ChaosCastleFinal_Reward WHERE CharName = @KeyName
END





GO
/****** Object:  StoredProcedure [dbo].[IGC_BattleCore_GetCCReward]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO


CREATE PROCEDURE [dbo].[IGC_BattleCore_GetCCReward]
	@AccountID varchar(10),
	@Name varchar(10),
	@ServerCode int
AS
BEGIN
	SET NOCOUNT ON;

	IF NOT EXISTS (SELECT * FROM [BattleCore].[dbo].BattleCoreInfo WHERE AccountID = @AccountID AND UBFName = @Name AND ServerCode = @ServerCode)
	BEGIN
		SELECT 2 AS Result
		RETURN
	END

	DECLARE @KeyName varchar(10)
	SELECT @KeyName = Name FROM [BattleCore].[dbo].BattleCoreInfo WHERE AccountID = @AccountID AND UBFName = @Name AND ServerCode = @ServerCode

	SELECT 1 AS Result, * FROM [BattleCore].[dbo].BattleCore_ChaosCastle_Reward WHERE CharName = @KeyName
END






GO
/****** Object:  StoredProcedure [dbo].[IGC_BattleCore_GetDSFReward]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO



CREATE PROCEDURE [dbo].[IGC_BattleCore_GetDSFReward]
	@AccountID varchar(10),
	@Name varchar(10),
	@ServerCode int
AS
BEGIN
	SET NOCOUNT ON;

	IF NOT EXISTS (SELECT * FROM [BattleCore].[dbo].BattleCoreInfo WHERE AccountID = @AccountID AND UBFName = @Name AND ServerCode = @ServerCode)
	BEGIN
		SELECT 2 AS Result
		RETURN
	END

	DECLARE @KeyName varchar(10)
	SELECT @KeyName = Name FROM [BattleCore].[dbo].BattleCoreInfo WHERE AccountID = @AccountID AND UBFName = @Name AND ServerCode = @ServerCode

	SELECT 1 AS Result, * FROM [BattleCore].[dbo].BattleCore_DevilSquareFinal_Reward WHERE CharName = @KeyName
END







GO
/****** Object:  StoredProcedure [dbo].[IGC_BattleCore_GetRealName]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE PROCEDURE [dbo].[IGC_BattleCore_GetRealName]
	@KeyName varchar(10)
AS
BEGIN
	SET NOCOUNT ON;

	SELECT UBFName AS RealName, ServerCode FROM [BattleCore].[dbo].BattleCoreInfo WHERE Name = @KeyName
END





GO
/****** Object:  StoredProcedure [dbo].[IGC_BattleCore_GetUserInfo]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE PROCEDURE [dbo].[IGC_BattleCore_GetUserInfo]
	@AccountID varchar(10),
	@Name varchar(10),
	@ServerCode int,
	@IsUBFServer int
AS
BEGIN
	SET NOCOUNT ON;

	IF (@IsUBFServer = 1) -- Battle Core GS
	BEGIN
		IF EXISTS (SELECT * FROM [BattleCore].[dbo].BattleCoreInfo WHERE AccountID = @AccountID AND Name = @Name)
		BEGIN
			SELECT 1 AS Result, RegisterState, RegisterMonth, RegisterDay FROM [BattleCore].[dbo].BattleCoreInfo WHERE AccountID = @AccountID AND Name = @Name
			RETURN
		END
		ELSE
		BEGIN
			SELECT 0 AS Result
			RETURN
		END
	END
	ELSE IF (@IsUBFServer = 0) -- Normal GS
	BEGIN
		IF EXISTS (SELECT * FROM [BattleCore].[dbo].BattleCoreInfo WHERE AccountID = @AccountID AND UBFName = @Name AND ServerCode = @ServerCode)
		BEGIN
			SELECT 1 AS Result, RegisterState, RegisterMonth, RegisterDay FROM [BattleCore].[dbo].BattleCoreInfo WHERE AccountID = @AccountID AND UBFName = @Name AND ServerCode = @ServerCode
			RETURN
		END
		ELSE
		BEGIN
			SELECT 0 AS Result
			RETURN
		END
	END
	ELSE
	BEGIN
		SELECT 0 AS Result
		RETURN
	END			
END





GO
/****** Object:  StoredProcedure [dbo].[IGC_BattleCore_JoinUser]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO


CREATE PROCEDURE [dbo].[IGC_BattleCore_JoinUser]
	@AccountID varchar(10),
	@Name varchar(10),
	@UBFName varchar(10),
	@ServerCode int,
	@RegisterState int,
	@RegisterMonth int,
	@RegisterDay int
AS
BEGIN
	SET NOCOUNT ON;

	IF ((SELECT COUNT (*) FROM [BattleCore].[dbo].BattleCoreInfo WHERE AccountID = @AccountID AND RegisterState = 1) >= 2)
	BEGIN
		SELECT 4 AS Result
		RETURN
	END

	IF EXISTS (SELECT * FROM [BattleCore].[dbo].BattleCoreInfo WHERE AccountID = @AccountID AND UBFName = @UBFName AND ServerCode = @ServerCode AND RegisterState = 1)
	BEGIN
		SELECT 2 AS Result
		RETURN
	END

	IF EXISTS (SELECT * FROM [BattleCore].[dbo].BattleCoreInfo WHERE AccountID = @AccountID AND UBFName = @UBFName AND ServerCode = @ServerCode AND RegisterState = 0)
	BEGIN
		DECLARE @LeftTime smalldatetime
		SELECT @LeftTime = LeftTime FROM [BattleCore].[dbo].BattleCoreInfo WHERE AccountID = @AccountID AND UBFName = @UBFName AND ServerCode = @ServerCode AND RegisterState = 0

		IF (DATEDIFF(SECOND, @LeftTime, GETDATE()) < 180)
		BEGIN
			SELECT 3 AS Result, (180 - DATEDIFF(SECOND, @LeftTime, GETDATE())) AS LeftSecond
			RETURN
		END
		ELSE
		BEGIN
			UPDATE [BattleCore].[dbo].BattleCoreInfo SET RegisterState = 1, RegisterMonth = @RegisterMonth, RegisterDay = @RegisterDay WHERE AccountID = @AccountID AND UBFName = @UBFName AND ServerCode = @ServerCode
			SELECT 1 AS Result
			RETURN
		END
	END
	ELSE
	BEGIN

		DECLARE @FreeKeyName int
		SET @FreeKeyName = 0

		WHILE (1 = 1)
		BEGIN
			SET @FreeKeyName = ROUND(((2000000000 - 1 -1) * RAND() + 1), 0)
			IF NOT EXISTS (SELECT * FROM [BattleCore].[dbo].BattleCoreInfo WHERE Name = @FreeKeyName)
			BEGIN
				BREAK
			END
		END

		INSERT INTO [BattleCore].[dbo].BattleCoreInfo (AccountID, Name, UBFName, ServerCode, RegisterState, RegisterMonth, RegisterDay, LeftTime) VALUES
		(@AccountID, @FreeKeyName, @UBFName, @ServerCode, 1, @RegisterMonth, @RegisterDay, NULL)

		INSERT INTO [BattleCore].[dbo].AccountCharacter (Id, GameID1, GameID2, GameID3, GameID4, GameID5, GameIDC, MoveCnt, Summoner, WarehouseExpansion, RageFighter, SecCode) VALUES
		(@AccountID, NULL, NULL, NULL, NULL, NULL, NULL, 0, 1, 1, 1, 0)

		SELECT 0 AS Result
	END
		RETURN


END






GO
/****** Object:  StoredProcedure [dbo].[IGC_BattleCore_SetCCFGainReward]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE PROCEDURE [dbo].[IGC_BattleCore_SetCCFGainReward]
	@AccountID varchar(10),
	@Name varchar(10),
	@ServerCode int
AS
BEGIN
	SET NOCOUNT ON;

	IF NOT EXISTS (SELECT * FROM [BattleCore].[dbo].BattleCoreInfo WHERE AccountID = @AccountID AND UBFName = @Name AND ServerCode = @ServerCode)
	BEGIN
		SELECT 0 AS Result
		RETURN
	END

	DECLARE @KeyName varchar(10)
	SELECT @KeyName = Name FROM [BattleCore].[dbo].BattleCoreInfo WHERE AccountID = @AccountID AND UBFName = @Name AND ServerCode = @ServerCode

	UPDATE [BattleCore].[dbo].BattleCore_ChaosCastleFinal_Reward SET RewardGain = 1 WHERE CharName = @KeyName
	SELECT 1 AS Result
END





GO
/****** Object:  StoredProcedure [dbo].[IGC_BattleCore_SetCCGainReward]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE PROCEDURE [dbo].[IGC_BattleCore_SetCCGainReward]
	@AccountID varchar(10),
	@Name varchar(10),
	@ServerCode int
AS
BEGIN
	SET NOCOUNT ON;

	IF NOT EXISTS (SELECT * FROM [BattleCore].[dbo].BattleCoreInfo WHERE AccountID = @AccountID AND UBFName = @Name AND ServerCode = @ServerCode)
	BEGIN
		SELECT 0 AS Result
		RETURN
	END

	DECLARE @KeyName varchar(10)
	SELECT @KeyName = Name FROM [BattleCore].[dbo].BattleCoreInfo WHERE AccountID = @AccountID AND UBFName = @Name AND ServerCode = @ServerCode

	UPDATE [BattleCore].[dbo].BattleCore_ChaosCastle_Reward SET RewardGain = 1 WHERE CharName = @KeyName
	SELECT 1 AS Result
END





GO
/****** Object:  StoredProcedure [dbo].[IGC_BattleCore_SetDSFGainReward]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO



CREATE PROCEDURE [dbo].[IGC_BattleCore_SetDSFGainReward]
	@AccountID varchar(10),
	@Name varchar(10),
	@ServerCode int
AS
BEGIN
	SET NOCOUNT ON;

	IF NOT EXISTS (SELECT * FROM [BattleCore].[dbo].BattleCoreInfo WHERE AccountID = @AccountID AND UBFName = @Name AND ServerCode = @ServerCode)
	BEGIN
		SELECT 0 AS Result
		RETURN
	END

	DECLARE @KeyName varchar(10)
	SELECT @KeyName = Name FROM [BattleCore].[dbo].BattleCoreInfo WHERE AccountID = @AccountID AND UBFName = @Name AND ServerCode = @ServerCode

	UPDATE [BattleCore].[dbo].BattleCore_DevilSquareFinal_Reward SET RewardGain = 1 WHERE CharName = @KeyName
	SELECT 1 AS Result
END







GO
/****** Object:  StoredProcedure [dbo].[IGC_CancelItemSale_AddItem]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE PROCEDURE [dbo].[IGC_CancelItemSale_AddItem]
	@szAccountID varchar(10),
	@szName varchar(10),
	@SellDate bigint,
	@SellExpireDate bigint,
	@SellPrice int,
	@ItemCode smallint,
	@ItemLevel tinyint,
	@ItemOp1 tinyint,
	@ItemOp2 tinyint,
	@ItemOp3 tinyint,
	@ItemDur tinyint,
	@ItemExcOp tinyint,
	@ItemSetOp tinyint,
	@ItemHarmonyOp smallint,
	@ItemOpEx smallint,
	@ItemSocketOp1 smallint,
	@ItemSocketOp2 smallint,
	@ItemSocketOp3 smallint,
	@ItemSocketOp4 smallint,
	@ItemSocketOp5 smallint,
	@ItemBonusSocketOp smallint,
	@ItemSerial bigint
AS
BEGIN
	SET NOCOUNT ON;
	DECLARE @ItemDataEntry int
	DECLARE @FreeItemNumber int
	SET @ItemDataEntry = 0
	SET @FreeItemNumber = 0

	IF EXISTS (SELECT * FROM IGC_CancelItemSale WHERE AccountID = @szAccountID AND Name = @szName AND
	ItemCode = @ItemCode AND ItemLevel = @ItemLevel AND ItemOp1 = @ItemOp1 AND ItemOp2 = @ItemOp2 AND @ItemOp3 = @ItemOp3
	AND ItemDur = @ItemDur AND ItemExcOp = @ItemExcOp AND ItemSetOp = @ItemSetOp AND ItemHarmonyOp = @ItemHarmonyOp AND
	ItemOpEx = @ItemOpEx AND ItemSocketOp1 = @ItemSocketOp1 AND ItemSocketOp2 = @ItemSocketOp2 AND
	ItemSocketOp3 = @ItemSocketOp3 AND ItemSocketOp4 = @ItemSocketOp4 AND ItemSocketOp5 = @ItemSocketOp5 AND
	ItemBonusSocketOp = @ItemBonusSocketOp AND ItemSerial = @ItemSerial)
	BEGIN
		UPDATE IGC_CancelItemSale SET ItemCount = ItemCount + 1, SellPrice = SellPrice + @SellPrice, SellDate = @SellDate,
		SellExpireDate = @SellExpireDate WHERE AccountID = @szAccountID AND Name = @szName AND
		ItemCode = @ItemCode AND ItemLevel = @ItemLevel AND ItemOp1 = @ItemOp1 AND ItemOp2 = @ItemOp2 AND @ItemOp3 = @ItemOp3
		AND ItemDur = @ItemDur AND ItemExcOp = @ItemExcOp AND ItemSetOp = @ItemSetOp AND ItemHarmonyOp = @ItemHarmonyOp AND
		ItemOpEx = @ItemOpEx AND ItemSocketOp1 = @ItemSocketOp1 AND ItemSocketOp2 = @ItemSocketOp2 AND
		ItemSocketOp3 = @ItemSocketOp3 AND ItemSocketOp4 = @ItemSocketOp4 AND ItemSocketOp5 = @ItemSocketOp5 AND
		ItemBonusSocketOp = @ItemBonusSocketOp AND ItemSerial = @ItemSerial
	END
	ELSE
	BEGIN
		
		SET @ItemDataEntry = (SELECT COUNT(*) FROM IGC_CancelItemSale WHERE AccountID = @szAccountID AND Name = @szName)

		IF (@ItemDataEntry > 4)
		BEGIN
			WHILE (@ItemDataEntry > 4)
			BEGIN
				DELETE FROM IGC_CancelItemSale WHERE AccountID = @szAccountID AND Name = @szName AND SellDate = (SELECT MIN(SellDate) FROM IGC_CancelItemSale)
				SET @ItemDataEntry = @ItemDataEntry - 1
			END
		END

		WHILE (@FreeItemNumber < 5)
		BEGIN
			IF NOT EXISTS (SELECT * FROM IGC_CancelItemSale WHERE AccountID = @szAccountID AND Name = @szName AND ItemNumber = @FreeItemNumber)
			BEGIN
				BREAK
			END
			ELSE
			BEGIN
				SET @FreeItemNumber = @FreeItemNumber + 1
			END
		END
			

		INSERT INTO IGC_CancelItemSale (AccountID, Name, ItemNumber, ItemCount, SellDate, SellExpireDate, SellPrice, ItemCode, 
		ItemLevel, ItemOp1, ItemOp2, ItemOp3, ItemDur, ItemExcOp, ItemSetOp, ItemHarmonyOp, ItemOpEx, 
		ItemSocketOp1, ItemSocketOp2, ItemSocketOp3, ItemSocketOp4, ItemSocketOp5, ItemBonusSocketOp, ItemSerial) VALUES
		(@szAccountID, @szName, @FreeItemNumber, 1, @SellDate, @SellExpireDate, @SellPrice, @ItemCode, @ItemLevel, @ItemOp1, @ItemOp2,
		@ItemOp3, @ItemDur, @ItemExcOp, @ItemSetOp, @ItemHarmonyOp, @ItemOpEx,
		@ItemSocketOp1, @ItemSocketOp2, @ItemSocketOp3, @ItemSocketOp4, @ItemSocketOp5, @ItemBonusSocketOp, @ItemSerial)
	END
END





GO
/****** Object:  StoredProcedure [dbo].[IGC_CancelItemSale_ClearExpiredItems]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE PROCEDURE [dbo].[IGC_CancelItemSale_ClearExpiredItems]
	@szAccountID varchar(10),
	@szName varchar(10),
	@CurrentTime bigint
AS
BEGIN
	SET NOCOUNT ON;

	DELETE FROM IGC_CancelItemSale WHERE AccountID = @szAccountID AND Name = @szName AND (@CurrentTime < SellDate OR @CurrentTime > SellExpireDate)
END





GO
/****** Object:  StoredProcedure [dbo].[IGC_CancelItemSale_DeleteItem]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE PROCEDURE [dbo].[IGC_CancelItemSale_DeleteItem]
	@szAccountID varchar(10),
	@szName varchar(10),
	@btItemNumber tinyint,
	@wItemCode smallint,
	@dwItemPrice int
AS
BEGIN
	SET NOCOUNT ON;

	DELETE FROM IGC_CancelItemSale WHERE AccountID = @szAccountID AND Name = @szName AND ItemNumber = @btItemNumber AND ItemCode = @wItemCode AND SellPrice = @dwItemPrice
END





GO
/****** Object:  StoredProcedure [dbo].[IGC_CancelItemSale_GetItemList]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE PROCEDURE [dbo].[IGC_CancelItemSale_GetItemList]
	@szAccountID varchar(10),
	@szName varchar(10)
AS
BEGIN
	SET NOCOUNT ON;

	SELECT TOP 5 * FROM IGC_CancelItemSale WHERE AccountID = @szAccountID AND Name = @szName
END





GO
/****** Object:  StoredProcedure [dbo].[IGC_CancelItemSale_GetItemToReBuy]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE PROCEDURE [dbo].[IGC_CancelItemSale_GetItemToReBuy]
	@szAccountID varchar(10),
	@szName varchar(10),
	@btItemNumber tinyint,
	@wItemCode smallint,
	@dwItemPrice int
AS
BEGIN
	SET NOCOUNT ON;

	SELECT TOP 1 * FROM IGC_CancelItemSale WHERE AccountID = @szAccountID AND Name = @szName AND ItemNumber = @btItemNumber AND ItemCode = @wItemCode AND SellPrice = @dwItemPrice
END





GO
/****** Object:  StoredProcedure [dbo].[IGC_DeleteAllPartyMatchingList]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE procedure [dbo].[IGC_DeleteAllPartyMatchingList]
as 
BEGIN
	SET NOCOUNT ON
	SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED

       delete from dbo.IGC_PartyMatching
       delete from dbo.IGC_WaitPartyMatching
END












GO
/****** Object:  StoredProcedure [dbo].[IGC_DeleteCharacter]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE PROCEDURE [dbo].[IGC_DeleteCharacter]
	@szCharName varchar(10)
AS
BEGIN
	SET NOCOUNT ON;

	IF EXISTS (SELECT [Name] FROM [dbo].[Character] WHERE [Name] = @szCharName)
	BEGIN
		DELETE FROM [dbo].[Character] WHERE [Name] = @szCharName
	END
	ELSE
	BEGIN
		SELECT 0 AS QueryResult
		RETURN
	END

	IF EXISTS (SELECT [Name] FROM [dbo].[OptionData] WHERE [Name] = @szCharName)
	BEGIN
		DELETE FROM [dbo].[OptionData] WHERE [Name] = @szCharName
	END

	IF EXISTS (SELECT [CHAR_NAME] FROM [dbo].[T_QUEST_EXP_INFO] WHERE [CHAR_NAME] = @szCharName)
	BEGIN
		DELETE FROM [dbo].[T_QUEST_EXP_INFO] WHERE [CHAR_NAME] = @szCharName
	END

	IF EXISTS (SELECT [Name] FROM [dbo].[IGC_Gens] WHERE [Name] = @szCharName)
	BEGIN
		DELETE FROM [dbo].[IGC_Gens] WHERE [Name] = @szCharName
	END

	IF EXISTS (SELECT [Name] FROM [dbo].[IGC_GensAbuse] WHERE [Name] = @szCharName)
	BEGIN
		DELETE FROM [dbo].[IGC_GensAbuse] WHERE [Name] = @szCharName
	END

	IF EXISTS (SELECT [CharacterName] FROM [dbo].[IGC_PeriodItemInfo] WHERE [CharacterName] = @szCharName)
	BEGIN
		DELETE FROM [dbo].[IGC_PeriodItemInfo] WHERE [CharacterName] = @szCharName
	END

	IF EXISTS (SELECT [CharName] FROM [dbo].[T_LUCKY_ITEM_INFO] WHERE [CharName] = @szCharName)
	BEGIN
		DELETE FROM [dbo].[T_LUCKY_ITEM_INFO] WHERE [CharName] = @szCharName
	END

	IF EXISTS (SELECT [Name] FROM [dbo].[T_PentagramInfo] WHERE [Name] = @szCharName)
	BEGIN
		DELETE FROM [dbo].[T_PentagramInfo] WHERE [Name] = @szCharName
	END

	IF EXISTS (SELECT [CharacterName] FROM [dbo].[IGC_PeriodBuffInfo] WHERE [CharacterName] = @szCharName)
	BEGIN
		DELETE FROM [dbo].[IGC_PeriodBuffInfo] WHERE [CharacterName] = @szCharName
	END

	IF EXISTS (SELECT [Name] FROM [dbo].[T_Event_Inventory] WHERE [Name] = @szCharName)
	BEGIN
		DELETE FROM [dbo].[T_Event_Inventory] WHERE [Name] = @szCharName
	END

	IF EXISTS (SELECT [Name] FROM [dbo].[PetWarehouse] WHERE [Name] = @szCharName)
	BEGIN
		DELETE FROM [dbo].[PetWarehouse] WHERE [Name] = @szCharName
	END

	IF EXISTS (SELECT [Name] FROM [dbo].[IGC_CancelItemSale] WHERE [Name] = @szCharName)
	BEGIN
		DELETE FROM [dbo].[IGC_CancelItemSale] WHERE [Name] = @szCharName
	END

	IF EXISTS (SELECT [Name] FROM [dbo].[IGC_GremoryCase] WHERE [Name] = @szCharName)
	BEGIN
		DELETE FROM [dbo].[IGC_GremoryCase] WHERE [Name] = @szCharName
	END

	IF EXISTS (SELECT [CharacterName] FROM [dbo].[T_MineSystem] WHERE [CharacterName] = @szCharName)
	BEGIN
		DELETE FROM [dbo].[T_MineSystem] WHERE [CharacterName] = @szCharName
	END

	IF EXISTS (SELECT [Name] FROM [dbo].[T_PSHOP_ITEMVALUE_INFO] WHERE [Name] = @szCharName)
	BEGIN
		DELETE FROM [dbo].[T_PSHOP_ITEMVALUE_INFO] WHERE [Name] = @szCharName
	END

	IF EXISTS (SELECT [Name] FROM [dbo].[T_MuRummy] WHERE [Name] = @szCharName)
	BEGIN
		DELETE FROM [dbo].[T_MuRummy] WHERE [Name] = @szCharName
	END

	IF EXISTS (SELECT [Name] FROM [dbo].[T_MuRummyInfo] WHERE [Name] = @szCharName)
	BEGIN
		DELETE FROM [dbo].[T_MuRummyInfo] WHERE [Name] = @szCharName
	END

	IF EXISTS (SELECT [Name] FROM [dbo].[T_MuRummyLog] WHERE [Name] = @szCharName)
	BEGIN
		DELETE FROM [dbo].[T_MuRummyLog] WHERE [Name] = @szCharName
	END

	IF EXISTS (SELECT [Name] FROM [dbo].[T_GMSystem] WHERE [Name] = @szCharName)
	BEGIN
		DELETE FROM [dbo].[T_GMSystem] WHERE [Name] = @szCharName
	END

	IF EXISTS (SELECT [name] FROM [dbo].[C_Monster_KillCount] WHERE [name] = @szCharName)
	BEGIN
		DELETE FROM [dbo].[C_Monster_KillCount] WHERE [name] = @szCharName
	END

	SELECT 1 AS QueryResult

END
GO
/****** Object:  StoredProcedure [dbo].[IGC_DeleteGuildMatchingData]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE    PROCEDURE [dbo].[IGC_DeleteGuildMatchingData]
   @GuildNum varchar(8)
as 
BEGIN

	SET NOCOUNT ON
	SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED

	IF EXISTS (SELECT TOP 1 * FROM dbo.IGC_GuildMatching  WITH (READUNCOMMITTED)
		where GuildNum = @GuildNum)
	              BEGIN
	                   delete dbo.IGC_GuildMatching where GuildNum = @GuildNum 
			     select 0 as Result
	              END
       ELSE
	              BEGIN
	       	     select -3 as Result
	              END
	       
END













GO
/****** Object:  StoredProcedure [dbo].[IGC_DeletePartyMatchingList]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE procedure [dbo].[IGC_DeletePartyMatchingList]
   @PartyLeaderName varchar(10)
as 
BEGIN
	SET NOCOUNT ON
	SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED

      	delete dbo.IGC_PartyMatching where PartyLeaderName = @PartyLeaderName
   
END













GO
/****** Object:  StoredProcedure [dbo].[IGC_DeleteWaitPartyMatchingList]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE procedure [dbo].[IGC_DeleteWaitPartyMatchingList]
   @MemberName varchar(10)
as 
BEGIN
	SET NOCOUNT ON
	SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED

      	delete dbo.IGC_WaitPartyMatching where MemberName = @MemberName
END













GO
/****** Object:  StoredProcedure [dbo].[IGC_DelGuildMatchingWaitState]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE    PROCEDURE [dbo].[IGC_DelGuildMatchingWaitState]
   @Name varchar(10)
as 
BEGIN
	SET NOCOUNT ON
	SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED
    delete  dbo.IGC_WaitGuildMatching where ApplicantName = @Name 
   
END













GO
/****** Object:  StoredProcedure [dbo].[IGC_GensAbuseLoad]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

-- =============================================
-- Author:  <Andrzej Erenc @ awHost.pl>
-- Create date: <28.02.2013>
-- Description: <Gens Abuse Load SP>
-- =============================================
CREATE PROCEDURE [dbo].[IGC_GensAbuseLoad]
 @szCharName varchar(10)
AS
BEGIN
 -- SET NOCOUNT ON added to prevent extra result sets from
 -- interfering with SELECT statements.
 SET NOCOUNT ON;
 BEGIN TRANSACTION

SELECT TOP 10 [Name]
      ,[KillName]
      ,[KillCount]
  FROM [dbo].[IGC_GensAbuse]
  WHERE Name = @szCharName
 
 IF(@@Error <> 0 )
  ROLLBACK TRANSACTION
 ELSE 
  COMMIT TRANSACTION
 SET NOCOUNT OFF 
END














GO
/****** Object:  StoredProcedure [dbo].[IGC_GensAbuseSave]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

-- =============================================
-- Author:  <Andrzej Erenc>
-- Create date: <28.02.2013>
-- Description: <Gens Abuse Save SP>
-- =============================================
CREATE PROCEDURE [dbo].[IGC_GensAbuseSave]
 @szCharName varchar(10),
 @szKillName varchar(10),
 @iKillCount smallint
AS
BEGIN
 -- SET NOCOUNT ON added to prevent extra result sets from
 -- interfering with SELECT statements.
 SET NOCOUNT ON;
 BEGIN TRANSACTION

    IF NOT EXISTS ( SELECT Name FROM IGC_GensAbuse  WITH ( READUNCOMMITTED ) 
    WHERE Name = @szCharName AND KillName = @szKillName )
 BEGIN
  INSERT INTO [dbo].[IGC_GensAbuse]
           ([Name]
           ,[KillName]
           ,[KillCount])
     VALUES
           (@szCharName,
           @szKillName,
           @iKillCount)
 END
 ELSE
 BEGIN
  UPDATE [dbo].[IGC_GensAbuse]
   SET [Name] = @szCharName,
      [KillName] = @szKillName,
      [KillCount] = @iKillCount
   WHERE Name = @szCharName
 END
 
 IF(@@Error <> 0 )
  ROLLBACK TRANSACTION
 ELSE 
  COMMIT TRANSACTION
 SET NOCOUNT OFF 
END


















GO
/****** Object:  StoredProcedure [dbo].[IGC_GensAdd]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO


-- =============================================
-- Author:  <Andrzej Erenc, Dudas (IGC)>
-- Create date: <28.02.2013>
-- Update date: <02.06.2015>
-- Description: <Gens Add SP>
-- =============================================
CREATE PROCEDURE [dbo].[IGC_GensAdd]
	@szCharName varchar(10),
	@Influence tinyint,
	@LeaveDayDifference smallint
AS
BEGIN

	SET NOCOUNT ON;
	BEGIN TRANSACTION;

	IF NOT EXISTS ( SELECT Name FROM IGC_Gens  WITH ( READUNCOMMITTED ) WHERE Name = @szCharName )
	BEGIN
		INSERT INTO IGC_Gens (Name, Influence, Rank, LeaveDate)  VALUES (@szCharName, @Influence, 0, NULL)
		SELECT 0x00
	END
	ELSE
	BEGIN
		DECLARE @CurInfluence tinyint
		SELECT @CurInfluence = Influence FROM IGC_Gens WHERE Name = @szCharName

		IF (@CurInfluence > 0)
		BEGIN
			SELECT 0x01
		END
		ELSE
		BEGIN
			DECLARE @GensLeaveDate smalldatetime
			SELECT @GensLeaveDate = LeaveDate FROM IGC_Gens WHERE Name = @szCharName
			IF (DATEDIFF(DAY, @GensLeaveDate, GETDATE()) < @LeaveDayDifference AND @LeaveDayDifference != 0)
			BEGIN
				SELECT 0x02
			END
			ELSE
			BEGIN
				UPDATE IGC_Gens SET Influence = @Influence, Rank = 0, Reward = 0, Points = 0, LeaveDate = NULL WHERE Name = @szCharName
				SELECT 0x00
			END
		END
	END
 
	IF(@@Error <> 0 )
		ROLLBACK TRANSACTION
	ELSE 
		COMMIT TRANSACTION
	SET NOCOUNT OFF 
END

















GO
/****** Object:  StoredProcedure [dbo].[IGC_GensDelete]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO


-- =============================================
-- Author:  <Andrzej Erenc, Dudas (IGC)>
-- Create date: <28.02.2013>
-- Update date: <02.06.2015>
-- Description: <Gens Delete SP>
-- =============================================

CREATE PROCEDURE [dbo].[IGC_GensDelete]
	@szCharName varchar(10)
AS
BEGIN

	SET NOCOUNT ON;
	BEGIN TRANSACTION;

    IF NOT EXISTS ( SELECT Name FROM IGC_Gens  WITH ( READUNCOMMITTED ) WHERE Name = @szCharName )
	BEGIN
		SELECT 0x01
	END
	ELSE IF EXISTS ( SELECT G_Name FROM Guild WHERE G_Master = @szCharName)
	BEGIN
		SELECT 0x02
	END
	ELSE
	BEGIN
		UPDATE IGC_Gens SET Influence = 0, Points = 0, Rank = 0, Reward = 0, LeaveDate = GETDATE() WHERE Name = @szCharName
		IF EXISTS (SELECT Name from IGC_GensAbuse WHERE Name = @szCharName)
		BEGIN
			DELETE FROM IGC_GensAbuse WHERE Name = @szCharName
		END
		SELECT 0x00
	END
 
	IF(@@Error <> 0 )
		ROLLBACK TRANSACTION
	ELSE 
		COMMIT TRANSACTION
	SET NOCOUNT OFF 
END

















GO
/****** Object:  StoredProcedure [dbo].[IGC_GensLoad]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

-- =============================================
-- Author:  <Andrzej Erenc awHost.pl>
-- Create date: <28.02.2013>
-- Description: <Gens Load SP>
-- =============================================
CREATE PROCEDURE [dbo].[IGC_GensLoad]
 @szCharName varchar(10)
AS
BEGIN
 SET NOCOUNT ON;
 BEGIN TRANSACTION

 SELECT [Name]
      ,[Points]
      ,[Class]
	  ,[Influence]
  FROM [dbo].[IGC_Gens]
  WHERE Name = @szCharName
 
 IF(@@Error <> 0 )
  ROLLBACK TRANSACTION
 ELSE 
  COMMIT TRANSACTION
 SET NOCOUNT OFF 
END


















GO
/****** Object:  StoredProcedure [dbo].[IGC_GensReward]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO


-- =============================================
-- Create date: <28.02.2013>
-- Description: <Gens Reward SP>
-- =============================================
CREATE PROCEDURE [dbo].[IGC_GensReward]
 @szCharName varchar(10)
AS
BEGIN
 -- SET NOCOUNT ON added to prevent extra result sets from
 -- interfering with SELECT statements.
 SET NOCOUNT ON;
 BEGIN TRANSACTION

    IF NOT EXISTS ( SELECT Name FROM IGC_Gens  WITH ( READUNCOMMITTED ) 
    WHERE Name = @szCharName )
 BEGIN
  SELECT 0x00
 END
 ELSE
 BEGIN
  UPDATE IGC_Gens SET Reward = 2 WHERE Name = @szCharName
  SELECT 0x01
 END
 
 IF(@@Error <> 0 )
  ROLLBACK TRANSACTION
 ELSE 
  COMMIT TRANSACTION
 SET NOCOUNT OFF 
END


GO
/****** Object:  StoredProcedure [dbo].[IGC_GensRewardCheck]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO


-- =============================================
-- Create date: <28.02.2013>
-- Description: <Gens Check Reward SP>
-- =============================================

CREATE PROCEDURE [dbo].[IGC_GensRewardCheck]
 @szCharName varchar(10)
AS
BEGIN
 -- SET NOCOUNT ON added to prevent extra result sets from
 -- interfering with SELECT statements.
 SET NOCOUNT ON;
 BEGIN TRANSACTION

    IF NOT EXISTS ( SELECT Name FROM IGC_Gens  WITH ( READUNCOMMITTED ) 
    WHERE Name = @szCharName )
 BEGIN
  SELECT 0 AS Reward, 0 as Class
 END
 ELSE
 BEGIN
  SELECT Reward, Class FROM IGC_Gens WHERE Name = @szCharName
 END
 
 IF(@@Error <> 0 )
  ROLLBACK TRANSACTION
 ELSE 
  COMMIT TRANSACTION
 SET NOCOUNT OFF 
END


GO
/****** Object:  StoredProcedure [dbo].[IGC_GensSave]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

-- =============================================
-- Author:  <Andrzej Erenc>
-- Create date: <28.02.2013>
-- Description: <Gens Save SP>
-- =============================================
CREATE PROCEDURE [dbo].[IGC_GensSave]
 @szCharName varchar(10),
 @Points int,
 @Class tinyint
AS
BEGIN
 -- SET NOCOUNT ON added to prevent extra result sets from
 -- interfering with SELECT statements.
 SET NOCOUNT ON;
 BEGIN TRANSACTION;

    IF NOT EXISTS ( SELECT Name FROM IGC_Gens  WITH ( READUNCOMMITTED ) 
    WHERE Name = @szCharName )
 BEGIN
  SELECT 0x00;
 END
 ELSE
 BEGIN
  UPDATE IGC_Gens SET Points = @Points, Class = @Class WHERE Name = @szCharName
  SELECT 0x01
 END
 
 IF(@@Error <> 0 )
  ROLLBACK TRANSACTION
 ELSE 
  COMMIT TRANSACTION
 SET NOCOUNT OFF 
END


















GO
/****** Object:  StoredProcedure [dbo].[IGC_GetGuildMatchingAcceptNRejectInfo]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE    PROCEDURE [dbo].[IGC_GetGuildMatchingAcceptNRejectInfo]
   @Name varchar(10)
as 
BEGIN

	SET NOCOUNT ON
	SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED

	select top 1 State  from dbo.IGC_WaitGuildMatching where ApplicantName = @Name and ( State = 1 or State = 2 )
   
END













GO
/****** Object:  StoredProcedure [dbo].[IGC_GetGuildMatchingList]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE procedure [dbo].[IGC_GetGuildMatchingList]
   @page int,
   @pageSize int
as 
BEGIN

	SET NOCOUNT ON
	SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED

	DECLARE @nTemp int, @nPageSize int
       DECLARE @lastIdentNo int

       set @nTemp = (((@page - 1) * @pageSize) + 1) 
       set @nPageSize = @pageSize

       set ROWCOUNT @nTemp

       select @lastIdentNo = identNo from dbo.IGC_GuildMatching order by identNo desc

       set ROWCOUNT @nPageSize
  
       select  
	   identNo, GuildName, GuildNum, GuildMasterName, GuildMasterLevel, GuildMasterClass, GuildMemberCnt, Memo, InterestType, LevelRange, ClassType, GensType, RegDate
	   from dbo.IGC_GuildMatching where identNo <= @lastIdentNo 
       order by identNo desc  
    
 end













GO
/****** Object:  StoredProcedure [dbo].[IGC_GetGuildMatchingListCount]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE   PROCEDURE [dbo].[IGC_GetGuildMatchingListCount]
AS
	SET NOCOUNT ON
	SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED

       select count(*) as Count  from dbo.IGC_GuildMatching












GO
/****** Object:  StoredProcedure [dbo].[IGC_GetGuildMatchingListKeyword]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE procedure [dbo].[IGC_GetGuildMatchingListKeyword]
   @page int,
   @pageSize int,
   @keyword varchar(10)
as 
BEGIN

	SET NOCOUNT ON
	SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED

	DECLARE @nTemp int, @nPageSize int
       DECLARE @lastIdentNo int


       set @nTemp = (((@page - 1) * @pageSize) + 1) 
       set @nPageSize = @pageSize

       set ROWCOUNT @nTemp

       set @keyword = rtrim(replace(@keyword,' ',''))
       set @keyword = '%' + @keyword + '%'

       select @lastIdentNo = identNo from dbo.IGC_GuildMatching  where replace(Memo,' ','')  like @keyword order by identNo desc

       set ROWCOUNT @nPageSize
  
       select  
	   identNo, GuildName, GuildNum, GuildMasterName, GuildMasterLevel, GuildMasterClass, GuildMemberCnt, Memo, InterestType, LevelRange, ClassType, GensType, RegDate
	   from dbo.IGC_GuildMatching where identNo <= @lastIdentNo and replace(Memo,' ','') like @keyword
       order by identNo desc  
    
 end












GO
/****** Object:  StoredProcedure [dbo].[IGC_GetGuildMatchingRegCheck]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE    PROCEDURE [dbo].[IGC_GetGuildMatchingRegCheck]
   @GuildNum int
as 
BEGIN

	SET NOCOUNT ON
	SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED

       select count(*) as count from IGC_GuildMatching where GuildNum = @GuildNum
END













GO
/****** Object:  StoredProcedure [dbo].[IGC_GetGuildMatchingWaitState]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE    PROCEDURE [dbo].[IGC_GetGuildMatchingWaitState]
   @Name varchar(10),
   @State tinyint
as 
BEGIN

	SET NOCOUNT ON
	SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED

    select top 1 State  from dbo.IGC_WaitGuildMatching where ApplicantName = @Name and State = @State
   
END













GO
/****** Object:  StoredProcedure [dbo].[IGC_GetGuildMatchingWaitStateList]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE    PROCEDURE [dbo].[IGC_GetGuildMatchingWaitStateList]
   @Name	 varchar(10)
as 
BEGIN

	SET NOCOUNT ON
	SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED

       select top 1 GuildName, GuildMasterName  from dbo.IGC_WaitGuildMatching where ApplicantName = @Name and State = 0
   
END













GO
/****** Object:  StoredProcedure [dbo].[IGC_GetLoadEventInventory]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE PROC [dbo].[IGC_GetLoadEventInventory] (
	@AccountID	VARCHAR(10)
,	@Name		VARCHAR(10)
)
AS  
	SET NOCOUNT ON
	SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED

	DECLARE @Inventory VARBINARY(1024)    
	DECLARE @OldAccountID VARCHAR(10)
	
	SELECT @OldAccountID = AccountID FROM dbo.T_Event_Inventory WHERE Name = @Name
	IF @OldAccountID <> @AccountID 
	BEGIN
		DELETE FROM T_Event_Inventory WHERE AccountID = @OldAccountID and Name = @Name
	END
	
	
	SELECT @Inventory = Inventory FROM dbo.T_Event_Inventory WHERE AccountID = @AccountID AND Name = @Name
	IF @@ROWCOUNT = 0
	BEGIN
		SET @Inventory = 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
		
		INSERT INTO dbo.T_Event_Inventory ( AccountID, Name, Inventory)
		VALUES (@AccountID,@Name,@Inventory);
	END

	SELECT @Inventory













GO
/****** Object:  StoredProcedure [dbo].[IGC_GetPartyMatchingList]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE procedure [dbo].[IGC_GetPartyMatchingList]
   @page int,
   @pageSize int
as 
BEGIN

	SET NOCOUNT ON
	SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED

	DECLARE @nTemp int, @nPageSize int
       DECLARE @lastIdentNo int

       set @nTemp = (((@page - 1) * @pageSize) + 1) 
       set @nPageSize = @pageSize

       set ROWCOUNT @nTemp

       select @lastIdentNo = IdentNo from dbo.IGC_PartyMatching order by IdentNo desc

       set ROWCOUNT @nPageSize
  
       select  
	   IdentNo, PartyLeaderName, Title, MinLevel, MaxLevel, HuntingGround, WantedClass, CurMemberCount, AcceptType, UsePassWord, 
	   PassWord, WantedClassDetailInfo1, WantedClassDetailInfo2, WantedClassDetailInfo3, WantedClassDetailInfo4, WantedClassDetailInfo5, 
	   WantedClassDetailInfo6, WantedClassDetailInfo7, LeaderChannel, GensType, LeaderLevel, LeaderClass, RegDate
	   from dbo.IGC_PartyMatching where IdentNo <= @lastIdentNo 
       order by IdentNo desc  
    
 end













GO
/****** Object:  StoredProcedure [dbo].[IGC_GetPartyMatchingListCount]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE    PROCEDURE [dbo].[IGC_GetPartyMatchingListCount]
AS
	SET NOCOUNT ON
	SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED

       select count(*) as Count  from dbo.IGC_PartyMatching













GO
/****** Object:  StoredProcedure [dbo].[IGC_GetPartyMatchingListJoinAble]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER OFF
GO

CREATE procedure [dbo].[IGC_GetPartyMatchingListJoinAble]
   @UserLevel smallint,
   @Class tinyint,
   @GensType tinyint,
   @page int,
   @pageSize int
as 
BEGIN
	SET NOCOUNT ON
	SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED

       DECLARE @nTemp int, @nPageSize int
       DECLARE @lastIdentNo int

       set @nTemp = (((@page - 1) * @pageSize) + 1) 
       set @nPageSize = @pageSize

       set ROWCOUNT @nTemp

       select @lastIdentNo = IdentNo from dbo.IGC_PartyMatching order by IdentNo desc

       set ROWCOUNT @nPageSize
  
       select  
	   IdentNo, PartyLeaderName, Title, MinLevel, MaxLevel, HuntingGround, WantedClass, CurMemberCount, AcceptType, UsePassWord, 
	   PassWord, WantedClassDetailInfo1, WantedClassDetailInfo2, WantedClassDetailInfo3, WantedClassDetailInfo4, WantedClassDetailInfo5, 
	   WantedClassDetailInfo6, WantedClassDetailInfo7, LeaderChannel, GensType, LeaderLevel, LeaderClass, RegDate
	   from dbo.IGC_PartyMatching where  MinLevel <= @UserLevel and (MaxLevel >= @UserLevel  or MaxLevel = 0)
       and  (WantedClass = WantedClass |  @Class or WantedClass = 0 )  and CurMemberCount < 5 and IdentNo <= @lastIdentNo 
       and UsePassWord = 0 and GensType = @GensType
       order by IdentNo desc  
SET NOCOUNT OFF
    
 end













GO
/****** Object:  StoredProcedure [dbo].[IGC_GetPartyMatchingListJoinAbleTotalCount]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE PROCEDURE [dbo].[IGC_GetPartyMatchingListJoinAbleTotalCount]
   @UserLevel smallint,
   @Class tinyint,
   @Gens tinyint
as 
  BEGIN
SET NOCOUNT ON
       select count(*) as nCount  from IGC_PartyMatching where  MinLevel <= @UserLevel and (MaxLevel >= @UserLevel  or MaxLevel = 0)
       and  (WantedClass = WantedClass |  @Class or WantedClass = 0 )  and CurMemberCount < 5 and UsePassWord = 0 
       and GensType = @Gens 
SET NOCOUNT OFF
    
 end
















GO
/****** Object:  StoredProcedure [dbo].[IGC_GetPartyMatchingListKeyword]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER OFF
GO

CREATE procedure [dbo].[IGC_GetPartyMatchingListKeyword]
   @page int,
   @pageSize int,
   @keyword varchar(10)
as 
BEGIN

	SET NOCOUNT ON
	SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED

	DECLARE @nTemp int, @nPageSize int
       DECLARE @lastIdentNo int


       set @nTemp = (((@page - 1) * @pageSize) + 1) 
       set @nPageSize = @pageSize

       set ROWCOUNT @nTemp

       set @keyword = rtrim(replace(@keyword,' ',''))
       set @keyword = '%' + @keyword + '%'

       select @lastIdentNo = IdentNo from dbo.IGC_PartyMatching  where replace(Title,' ','')  like @keyword order by IdentNo desc

       set ROWCOUNT @nPageSize
  
       select  
	   IdentNo, PartyLeaderName, Title, MinLevel, MaxLevel, HuntingGround, WantedClass, CurMemberCount, AcceptType, UsePassWord, 
	   PassWord, WantedClassDetailInfo1, WantedClassDetailInfo2, WantedClassDetailInfo3, WantedClassDetailInfo4, WantedClassDetailInfo5,
	   WantedClassDetailInfo6, WantedClassDetailInfo7, LeaderChannel, GensType, LeaderLevel, LeaderClass, RegDate
	   from dbo.IGC_PartyMatching where IdentNo <= @lastIdentNo and replace(Title,' ','') like @keyword
       order by IdentNo desc  
    
 end













GO
/****** Object:  StoredProcedure [dbo].[IGC_GetPartyMatchingPassWordInfo]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE    PROCEDURE [dbo].[IGC_GetPartyMatchingPassWordInfo]
   @Name	 varchar(10)
as 
BEGIN

	SET NOCOUNT ON
	SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED

       select top 1 PassWord,UsePassWord,AcceptType  from IGC_PartyMatching where PartyLeaderName = @Name

END













GO
/****** Object:  StoredProcedure [dbo].[IGC_GetPartyMatchingRandom]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE procedure [dbo].[IGC_GetPartyMatchingRandom]
   @UserLevel smallint,
   @Class         tinyint,
   @GensType    tinyint
as 
BEGIN

           SET NOCOUNT ON
           SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED

           select top 1 
           PartyLeaderName
           from dbo.IGC_PartyMatching where MinLevel <= @UserLevel and (MaxLevel >= @UserLevel  or MaxLevel = 0)
       and  (WantedClass = WantedClass |  @Class  or WantedClass = 0)   and CurMemberCount < 5   and UsePassWord = 0 and GensType = @GensType
           order by newid()
  END












GO
/****** Object:  StoredProcedure [dbo].[IGC_GetPartyMatchingWaitList]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER OFF
GO


CREATE    PROCEDURE [dbo].[IGC_GetPartyMatchingWaitList]
   @Name	 varchar(10)
as 
BEGIN

	SET NOCOUNT ON
	SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED

	select top 1 LeaderName from dbo.IGC_WaitPartyMatching where MemberName = @Name
   
END















GO
/****** Object:  StoredProcedure [dbo].[IGC_GetPartyMatchingWaitListForLeader]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE    PROCEDURE [dbo].[IGC_GetPartyMatchingWaitListForLeader]
   @Name	 varchar(10)
as 
BEGIN

	SET NOCOUNT ON
	SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED

	select top 80 MemberName, Class, MemberLevel from dbo.IGC_WaitPartyMatching where LeaderName = @Name
   
END













GO
/****** Object:  StoredProcedure [dbo].[IGC_GetTotalOnline]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE PROCEDURE [dbo].[IGC_GetTotalOnline]
AS
BEGIN
	SET NOCOUNT ON;

	DECLARE @Server1 int
	DECLARE @Server2 int

	SET @Server1 = 0
	SET @Server2 = 0

	SELECT @Server1 = COUNT(ConnectStat) FROM [MuOnline].[dbo].[MEMB_STAT] WHERE ConnectStat = 1
	SELECT @Server2 = COUNT(ConnectStat) FROM [MuOnline2].[dbo].[MEMB_STAT] WHERE ConnectStat = 1

	SELECT (@Server1 + @Server2) AS TotalOnline
END





GO
/****** Object:  StoredProcedure [dbo].[IGC_GetWaitGuildMatching]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO


CREATE    PROCEDURE [dbo].[IGC_GetWaitGuildMatching] 
   @GuildNumber int, 
   @State tinyint
as 
BEGIN

	SET NOCOUNT ON
	SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED

    select top 80   
	identNo, GuildNumber, GuildName, GuildMasterName, ApplicantName, ApplicantClass, ApplicantLevel, State, RegDate
	from dbo.IGC_WaitGuildMatching where GuildNumber = @GuildNumber and State = @State
    order by identNo desc
   
END













GO
/****** Object:  StoredProcedure [dbo].[IGC_GremoryCase_AddItem]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE PROCEDURE [dbo].[IGC_GremoryCase_AddItem]
	@szAccountID varchar(10),
	@szName	varchar(10),
	@StorageType tinyint,
	@RewardSource tinyint,
	@ItemID smallint,
	@ItemLevel tinyint,
	@ItemDurability smallint,
	@ItemOp1 tinyint,
	@ItemOp2 tinyint,
	@ItemOp3 tinyint,
	@ItemExcOption smallint,
	@ItemSetOption smallint,
	@ItemSocketCount tinyint,
	@ItemMainAttribute tinyint,
	@ItemMuunEvoItemType smallint,
	@ItemMuunEvoItemIndex smallint,
	@ReceiveDate bigint,
	@ExpireDate bigint
AS
BEGIN
	SET NOCOUNT ON;

	DECLARE @ItemsInStorage int
	SET @ItemsInStorage = (SELECT COUNT(*) FROM IGC_GremoryCase WHERE AccountID = @szAccountID AND ((Name = @szName AND StorageType = 2) OR StorageType = 1))

	IF (@ItemsInStorage >= 50)
	BEGIN
		WHILE (@ItemsInStorage >= 50)
		BEGIN
			DELETE FROM IGC_GremoryCase WHERE AccountID = @szAccountID AND ((Name = @szName AND StorageType = 2) OR StorageType = 1) AND ReceiveDate = 
			(SELECT MIN(ReceiveDate) FROM IGC_GremoryCase WHERE AccountID = @szAccountID AND ((Name = @szName AND StorageType = 2) OR StorageType = 1))
			SET @ItemsInStorage = @ItemsInStorage - 1
		END
	END

	DECLARE @FreeItemGUID int
	SET @FreeItemGUID = 0

	WHILE (1 = 1)
	BEGIN
		IF NOT EXISTS (SELECT * FROM IGC_GremoryCase WHERE AccountID = @szAccountID AND ((Name = @szName AND StorageType = 2) OR StorageType = 1) AND ItemGUID = @FreeItemGUID)
		BEGIN
			BREAK
		END
		SET @FreeItemGUID = @FreeItemGUID + 1
	END

	DECLARE @FreeAuthCode int
	SET @FreeAuthCode = 0

	WHILE (1 = 1)
	BEGIN
		SET @FreeAuthCode = ROUND(((2000000000 - 1 -1) * RAND() + 1), 0)
		IF NOT EXISTS (SELECT * FROM IGC_GremoryCase WHERE AuthCode = @FreeAuthCode)
		BEGIN
			BREAK
		END
	END

	INSERT INTO IGC_GremoryCase (AccountID, Name, StorageType, RewardSource, ItemGUID, AuthCode, ItemID, ItemLevel, ItemDurability, ItemOp1, ItemOp2, ItemOp3, ItemExcOption, ItemSetOption, ItemSocketCount, ItemMainAttribute, ItemMuunEvoItemType, ItemMuunEvoItemIndex, ReceiveDate, ExpireDate) VALUES
	(@szAccountID, @szName, @StorageType, @RewardSource, @FreeItemGUID, @FreeAuthCode, @ItemID, @ItemLevel, @ItemDurability, @ItemOp1, @ItemOp2, @ItemOp3, @ItemExcOption, @ItemSetOption, @ItemSocketCount, @ItemMainAttribute, @ItemMuunEvoItemType, @ItemMuunEvoItemIndex, @ReceiveDate, @ExpireDate)

	SELECT @FreeItemGUID AS ItemGUID, @FreeAuthCode AS AuthCode
END





GO
/****** Object:  StoredProcedure [dbo].[IGC_GremoryCase_CheckUseItem]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE PROCEDURE [dbo].[IGC_GremoryCase_CheckUseItem]
	@ItemID smallint,
	@ItemGUID int,
	@AuthCode int
AS
BEGIN
	SET NOCOUNT ON;

	IF EXISTS (SELECT * FROM IGC_GremoryCase WHERE ItemID = @ItemID AND ItemGUID = @ItemGUID AND AuthCode = @AuthCode)
	BEGIN
		SELECT 1 AS ResultCode
	END
	ELSE
	BEGIN
		SELECT 0 AS ResultCode
	END
END





GO
/****** Object:  StoredProcedure [dbo].[IGC_GremoryCase_DeleteItem]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE PROCEDURE [dbo].[IGC_GremoryCase_DeleteItem]
	@ItemID smallint,
	@ItemGUID int,
	@AuthCode int
AS
BEGIN
	SET NOCOUNT ON;

	DELETE FROM IGC_GremoryCase WHERE ItemID = @ItemID AND ItemGUID = @ItemGUID AND AuthCode = @AuthCode
END





GO
/****** Object:  StoredProcedure [dbo].[IGC_GremoryCase_GetItemList]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE PROCEDURE [dbo].[IGC_GremoryCase_GetItemList]
	@szAccountID varchar(10),
	@szName varchar(10)
AS
BEGIN
	SET NOCOUNT ON;

	SELECT * FROM IGC_GremoryCase WHERE AccountID = @szAccountID AND ((Name = @szName AND StorageType = 2) OR StorageType = 1) ORDER BY StorageType ASC
END





GO
/****** Object:  StoredProcedure [dbo].[IGC_IBS_CheckGiftTarget]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE PROCEDURE [dbo].[IGC_IBS_CheckGiftTarget]
	@Target					varchar(10)	
AS
BEGIN
	SET NOCOUNT ON;

    IF NOT EXISTS(SELECT AccountID FROM Character WHERE Name = @Target) 
	BEGIN
		SELECT 3 AS Result
	END
	ELSE
	BEGIN
		SELECT 0 AS Result
	END
	
	SET NOCOUNT OFF;
END





GO
/****** Object:  StoredProcedure [dbo].[IGC_InsertGuildMatchingList]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO


CREATE procedure [dbo].[IGC_InsertGuildMatchingList]
   @GuildName varchar(8),
   @GuildMasterName varchar(10),
   @GuildNum	int,
   @GuildMasterLevel smallint,
   @GuildMasterClass tinyint,
   @GuildMemberCnt smallint,
   @Memo varchar(40),
   @InterestType tinyint,
   @LevelRange tinyint,
   @ClassType tinyint,
   @GensType tinyint
as 
BEGIN

	SET NOCOUNT ON
	SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED

	DECLARE @aErrNo INT, 
			@aRowCnt INT

	SELECT @aErrNo = 0, @aRowCnt = 0

	IF EXISTS (SELECT TOP 1 * FROM dbo.IGC_GuildMatching where GuildNum = @GuildNum)
    BEGIN
		BEGIN TRANSACTION
		BEGIN
			delete dbo.IGC_GuildMatching where GuildNum= @GuildNum
			SELECT @aErrNo = @@ERROR,  @aRowCnt = @@ROWCOUNT
			IF  @aErrNo <> 0 OR @aRowCnt = 0
			BEGIN
				ROLLBACK TRAN
				RETURN
			END		

			insert into dbo.IGC_GuildMatching(GuildName,GuildNum,GuildMasterName,GuildMasterLevel,GuildMasterClass,
			GuildMemberCnt,Memo,InterestType,LevelRange,ClassType,GensType,RegDate)
			values(@GuildName,@GuildNum,@GuildMasterName,@GuildMasterLevel, @GuildMasterClass, @GuildMemberCnt,
			@Memo,@InterestType,@LevelRange,@ClassType,@GensType,GetDate())
			SELECT @aErrNo = @@ERROR,  @aRowCnt = @@ROWCOUNT
			IF  @aErrNo <> 0 OR @aRowCnt = 0
			BEGIN
				ROLLBACK TRAN
				RETURN
			END		

		END
		COMMIT TRANSACTION

    END
    ELSE
    BEGIN
		insert into dbo.IGC_GuildMatching(GuildName,GuildNum,GuildMasterName,GuildMasterLevel,GuildMasterClass,
		GuildMemberCnt,Memo,InterestType,LevelRange,ClassType,GensType,RegDate)
		values(@GuildName,@GuildNum,@GuildMasterName,@GuildMasterLevel, @GuildMasterClass, @GuildMemberCnt,
		@Memo,@InterestType,@LevelRange,@ClassType,@GensType,GetDate())

    END
       
   
END













GO
/****** Object:  StoredProcedure [dbo].[IGC_InsertGuildMatchingWaitList]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE  procedure [dbo].[IGC_InsertGuildMatchingWaitList]

   @GuildNumber	int,
   @ApplicantName varchar(10),
   @ApplicantClass tinyint,
   @ApplicantLevel smallint,
   @State tinyint
as 
BEGIN
	SET NOCOUNT ON
	SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED

	DECLARE @aErrNo INT, 
			@aRowCnt INT

	SELECT @aErrNo = 0, @aRowCnt = 0

       declare @GuildName varchar(8)
       declare  @GuildMasterName varchar(10)

       set  @GuildName = (select top 1  GuildName from dbo.IGC_GuildMatching where GuildNum = @GuildNumber)
       set  @GuildMasterName = (select top 1  GuildMasterName from dbo.IGC_GuildMatching where GuildNum = @GuildNumber)

	IF EXISTS (SELECT TOP 1 * FROM dbo.IGC_WaitGuildMatching where ApplicantName = @ApplicantName)
    BEGIN
		BEGIN TRANSACTION
		BEGIN

				delete dbo.IGC_WaitGuildMatching where ApplicantName = @ApplicantName
				SELECT @aErrNo = @@ERROR,  @aRowCnt = @@ROWCOUNT
				IF  @aErrNo <> 0 OR @aRowCnt = 0
				BEGIN
					ROLLBACK TRAN
					RETURN
				END		

				insert into dbo.IGC_WaitGuildMatching (GuildName, GuildMasterName, GuildNumber, ApplicantName, ApplicantClass,
				ApplicantLevel,State,RegDate) values(@GuildName, @GuildMasterName,@GuildNumber,@ApplicantName,@ApplicantClass,
				@ApplicantLevel,@State,GetDate())
				SELECT @aErrNo = @@ERROR,  @aRowCnt = @@ROWCOUNT
				IF  @aErrNo <> 0 OR @aRowCnt = 0
				BEGIN
					ROLLBACK TRAN
					RETURN
				END		

		END
		COMMIT TRANSACTION

    END
    ELSE
    BEGIN
            insert into dbo.IGC_WaitGuildMatching (GuildName, GuildMasterName, GuildNumber, ApplicantName, ApplicantClass,
            ApplicantLevel,State,RegDate) values(@GuildName, @GuildMasterName,@GuildNumber,@ApplicantName,@ApplicantClass,
			@ApplicantLevel,@State,GetDate())

    END
       
   
END













GO
/****** Object:  StoredProcedure [dbo].[IGC_InsertPartyMatchingList]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE procedure [dbo].[IGC_InsertPartyMatchingList]
   @PartyLeaderName varchar(10),
   @Title varchar(40),
   @PassWord varchar(4),
   @MinLevel	smallint,
   @MaxLevel smallint,
   @HuntingGround smallint,
   @WantedClass tinyint,
   @CurMemberCount tinyint,
   @UsePassWord tinyint,
   @AcceptType tinyint,
   @WantedClassDetailInfo1 tinyint,
   @WantedClassDetailInfo2 tinyint,
   @WantedClassDetailInfo3 tinyint,
   @WantedClassDetailInfo4 tinyint,
   @WantedClassDetailInfo5 tinyint,
   @WantedClassDetailInfo6 tinyint,
   @WantedClassDetailInfo7 tinyint,
   @LeaderChannel tinyint,
   @GensType tinyint,
   @LeaderLevel smallint,
   @LeaderClass tinyint
as 
BEGIN

	SET NOCOUNT ON
	SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED

	DECLARE @aErrNo INT, 
			@aRowCnt INT

	SELECT @aErrNo = 0, @aRowCnt = 0


	IF EXISTS (SELECT TOP 1 * FROM dbo.IGC_PartyMatching where PartyLeaderName = @PartyLeaderName)
    BEGIN

		BEGIN TRANSACTION
		BEGIN

			delete IGC_PartyMatching where PartyLeaderName= @PartyLeaderName
			SELECT @aErrNo = @@ERROR,  @aRowCnt = @@ROWCOUNT
			IF  @aErrNo <> 0 OR @aRowCnt = 0
			BEGIN
				ROLLBACK TRAN
				RETURN
			END		

			insert into dbo.IGC_PartyMatching(PartyLeaderName,Title,MinLevel,MaxLevel,HuntingGround,WantedClass,CurMemberCount,PassWord,UsePassWord,AcceptType,WantedClassDetailInfo1,
			WantedClassDetailInfo2,WantedClassDetailInfo3,WantedClassDetailInfo4,WantedClassDetailInfo5,WantedClassDetailInfo6,WantedClassDetailInfo7
			,LeaderChannel,GensType,  LeaderLevel ,LeaderClass,RegDate)
			values(@PartyLeaderName,@Title,@MinLevel,@MaxLevel,@HuntingGround,@WantedClass,@CurMemberCount,@PassWord,@UsePassWord,@AcceptType,@WantedClassDetailInfo1
			,@WantedClassDetailInfo2,@WantedClassDetailInfo3,@WantedClassDetailInfo4,@WantedClassDetailInfo5,@WantedClassDetailInfo6,@WantedClassDetailInfo7
				,@LeaderChannel,@GensType, @LeaderLevel ,   @LeaderClass,GetDate())
			SELECT @aErrNo = @@ERROR,  @aRowCnt = @@ROWCOUNT
			IF  @aErrNo <> 0 OR @aRowCnt = 0
			BEGIN
				ROLLBACK TRAN
				RETURN
			END		

		END
		COMMIT TRANSACTION

		select 1 as result

    END
    ELSE
    BEGIN

	insert into dbo.IGC_PartyMatching(PartyLeaderName,Title,MinLevel,MaxLevel,HuntingGround,WantedClass,CurMemberCount,PassWord,UsePassWord,AcceptType,WantedClassDetailInfo1,
	WantedClassDetailInfo2,WantedClassDetailInfo3,WantedClassDetailInfo4,WantedClassDetailInfo5,WantedClassDetailInfo6,WantedClassDetailInfo7
	,LeaderChannel,GensType,  LeaderLevel ,LeaderClass,RegDate)

	values(@PartyLeaderName,@Title,@MinLevel,@MaxLevel,@HuntingGround,@WantedClass,@CurMemberCount,@PassWord,@UsePassWord,@AcceptType,@WantedClassDetailInfo1
	,@WantedClassDetailInfo2,@WantedClassDetailInfo3,@WantedClassDetailInfo4,@WantedClassDetailInfo5,@WantedClassDetailInfo6,@WantedClassDetailInfo7
		,@LeaderChannel,@GensType,   @LeaderLevel , @LeaderClass,GetDate())
	select 0 as result

    END
       
   
END













GO
/****** Object:  StoredProcedure [dbo].[IGC_InsertWaitPartyMatching]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO


CREATE procedure [dbo].[IGC_InsertWaitPartyMatching]
   @PartyLeaderName varchar(10),
   @MemberName varchar(10),
   @Class tinyint,
   @MemberLevel smallint,
   @MemberDBNumber int
as 
BEGIN

	SET NOCOUNT ON
	SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED

	IF EXISTS (SELECT TOP 1 * FROM IGC_WaitPartyMatching where MemberName = @MemberName)
    BEGIN
            select -1 as result

    END
    ELSE
    BEGIN
	insert into dbo.IGC_WaitPartyMatching(LeaderName,MemberName,Class,MemberLevel,MemberDBNumber,RegDate)
	values(@PartyLeaderName,@MemberName,@Class,@MemberLevel,@MemberDBNumber,GetDate())
        select 0 as result
    END
       
END













GO
/****** Object:  StoredProcedure [dbo].[IGC_IsApplicantListGuildMatching]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE    PROCEDURE [dbo].[IGC_IsApplicantListGuildMatching]
   @GuildNumber	int
as 
BEGIN

	SET NOCOUNT ON
	SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED

	IF EXISTS (SELECT TOP 1 * FROM IGC_WaitGuildMatching where GuildNumber = @GuildNumber and State = 0)
    BEGIN
        select 1 as result

    END
    ELSE
    BEGIN
        select 0 as result
    END
   
END













GO
/****** Object:  StoredProcedure [dbo].[IGC_LuckyItemDelete]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE  PROC  [dbo].[IGC_LuckyItemDelete]
	@CharacterName varchar(10)
	,@ItemCode	int
	,@ItemSerial	bigint
	AS  
	DELETE	T_LUCKY_ITEM_INFO 
	WHERE	CharName = @CharacterName AND ItemCode = @ItemCode AND ItemSerial = @ItemSerial












GO
/****** Object:  StoredProcedure [dbo].[IGC_LuckyItemInsert]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE  PROC  [dbo].[IGC_LuckyItemInsert]  
	@UserGuid 			int
	,@CharName			varchar(10)
	,@ItemCode			int
	,@ItemSerial		bigint
	,@DurabilitySmall	smallint
	AS         
	DECLARE @ErrorCode int        
	SET	@ErrorCode = 0        
	
	SET NOCOUNT ON    
	SET	XACT_ABORT ON
	        
	BEGIN TRAN
	
	IF NOT EXISTS (SELECT ItemCode FROM T_LUCKY_ITEM_INFO  WITH ( READUNCOMMITTED )         
	WHERE CharName = @CharName AND ItemCode = @ItemCode AND ItemSerial  = @ItemSerial
	and	UserGuid = @UserGuid)
	BEGIN            
		INSERT INTO T_LUCKY_ITEM_INFO (UserGuid, CharName, ItemCode, ItemSerial, DurabilitySmall) 
		VALUES  (@UserGuid, @CharName, @ItemCode, @ItemSerial, @DurabilitySmall)    
	END       
	
	ELSE        
	BEGIN    
		UPDATE T_LUCKY_ITEM_INFO 
		SET	DurabilitySmall = @DurabilitySmall
		WHERE CharName = @CharName AND ItemCode = @ItemCode AND ItemSerial  = @ItemSerial 
		and	UserGuid = @UserGuid
	END        
	
	IF( @@Error <> 0 )        
	BEGIN        
	SET @ErrorCode = 2        
	END        
	
	IF ( @ErrorCode  <> 0 ) 
	ROLLBACK TRAN       
	ELSE        
	COMMIT TRAN
	
	SELECT @ErrorCode        
	
	SET	XACT_ABORT OFF        
	SET NOCOUNT OFF












GO
/****** Object:  StoredProcedure [dbo].[IGC_LuckyItemSelect]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE  PROC [dbo].[IGC_LuckyItemSelect]
	@CharacterName varchar(10)  
	AS  
	SELECT	ItemCode, ItemSerial, DurabilitySmall 
	FROM	T_LUCKY_ITEM_INFO 
	WHERE	CharName = @CharacterName












GO
/****** Object:  StoredProcedure [dbo].[IGC_MineSystem_Delete_UPTUserInfo]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE PROCEDURE [dbo].[IGC_MineSystem_Delete_UPTUserInfo]
	@CharacterName	varchar(10)	
AS
    SET NOCOUNT ON
	SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED

	DELETE dbo.T_MineSystem WHERE CharacterName = @CharacterName















GO
/****** Object:  StoredProcedure [dbo].[IGC_MineSystem_Insert_UPTUserInfo]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE PROCEDURE [dbo].[IGC_MineSystem_Insert_UPTUserInfo]
	@CharacterName	varchar(10),
	@QuestionType	int,
	@CurrentType	int
AS
    SET NOCOUNT ON
	SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED

	INSERT dbo.T_MineSystem ( CharacterName, TwinkleType, CurrentStage )
	VALUES(@CharacterName, @QuestionType, @CurrentType)















GO
/****** Object:  StoredProcedure [dbo].[IGC_MineSystem_Select_UPTUserInfo]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

-- =============================================
CREATE PROCEDURE [dbo].[IGC_MineSystem_Select_UPTUserInfo]
	@CharacterName	varchar(10)	
AS
    SET NOCOUNT ON
	SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED

	SELECT CharacterName, TwinkleType, CurrentStage FROM dbo.T_MineSystem WHERE CharacterName = @CharacterName














GO
/****** Object:  StoredProcedure [dbo].[IGC_Monster_KillCount_Save]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE PROCEDURE [dbo].[IGC_Monster_KillCount_Save]
	@szCharName VARCHAR(10), --Name of character who kill the monster
	@iMonsterID INT --Monster ID that was killed
AS
	BEGIN
		SET NOCOUNT ON

		IF EXISTS (SELECT 0 FROM dbo.C_Monster_KillCount WHERE Name = @szCharName AND MonsterID = @iMonsterID)
			BEGIN
				UPDATE dbo.C_Monster_KillCount SET Count = Count +1 WHERE Name = @szCharName AND MonsterID = @iMonsterID
			END
		ELSE
			BEGIN
				INSERT INTO dbo.C_Monster_KillCount (Name, MonsterID, Count) VALUES (@szCharName, @iMonsterID, 1)
			END


		IF (@@ERROR <> 0)
			SELECT 0 as ResultCode --No error, return success.
		ELSE
			SELECT 1 as ResultCode --We got an error error, return fail.
	END














GO
/****** Object:  StoredProcedure [dbo].[IGC_PentagramInfoDelete]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE   PROC [dbo].[IGC_PentagramInfoDelete]
	@UserGuid	INT,
	@AccountID	VARCHAR(10),
	@Name		VARCHAR(10),
	@JewelPos	TINYINT,
	@JewelIndex	smallint
	AS  
	SET NOCOUNT ON
	SET XACT_ABORT ON 
	
	IF (@JewelPos = 0)
	BEGIN
	
		DELETE	T_PentagramInfo 
		WHERE
			AccountID	= @AccountID	
			AND	Name		= @Name		
			AND	JewelPos	= @JewelPos
			AND	JewelIndex	= @JewelIndex
	
	END
	ELSE
	BEGIN
	
		DELETE	T_PentagramInfo 
		WHERE
			AccountID	= @AccountID	
			AND	JewelPos	= @JewelPos
			AND	JewelIndex	= @JewelIndex
	
	END
	
	SET XACT_ABORT OFF
	SET NOCOUNT OFF












GO
/****** Object:  StoredProcedure [dbo].[IGC_PentagramInfoSelect]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE  PROC [dbo].[IGC_PentagramInfoSelect]
	@UserGuid	INT,
	@AccountID	VARCHAR(10),
	@Name		VARCHAR(10),
	@JewelPos	TINYINT
	AS  
	SET NOCOUNT ON
	SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED
	
	IF (@JewelPos = 0)
	BEGIN
	
		SELECT	JewelIndex,ItemType,ItemIndex,MainAttribute,JewelLevel,Rank1, Rank1Level, Rank2, Rank2Level, Rank3, Rank3Level, Rank4, Rank4Level, Rank5, Rank5Level
		FROM	dbo.T_PentagramInfo
		WHERE	AccountID	= @AccountID	
		AND	Name		= @Name		
		AND	JewelPos	= @JewelPos	
	
	END
	ELSE
	BEGIN
	
		SELECT	JewelIndex,ItemType,ItemIndex,MainAttribute,JewelLevel,Rank1, Rank1Level, Rank2, Rank2Level, Rank3, Rank3Level, Rank4, Rank4Level, Rank5, Rank5Level
		FROM	dbo.T_PentagramInfo
		WHERE	AccountID	= @AccountID	
		AND	JewelPos	= @JewelPos	
	
	END
	
	
	SET NOCOUNT OFF












GO
/****** Object:  StoredProcedure [dbo].[IGC_PentagramInfoUpdate]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE   PROC [dbo].[IGC_PentagramInfoUpdate]
	@UserGuid	INT,
	@AccountID	VARCHAR(10),
	@Name		VARCHAR(10),
	@JewelPos	TINYINT,
	@JewelIndex	SMALLINT,
	@ItemType	SMALLINT,
	@ItemIndex	INT,
	@MainAttribute	SMALLINT,
	@JewelLevel	SMALLINT,
	@Rank1		TINYINT,
	@Rank1Level	TINYINT,
	@Rank2		TINYINT,
	@Rank2Level	TINYINT,
	@Rank3		TINYINT,
	@Rank3Level	TINYINT,
	@Rank4		TINYINT,
	@Rank4Level	TINYINT,
	@Rank5		TINYINT,
	@Rank5Level	TINYINT
	AS  
	SET NOCOUNT ON
	SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED
	SET XACT_ABORT ON
	
	
	DECLARE  @Result  INT
	SET 	   @Result = 0        
	
	BEGIN TRAN 
	
	IF EXISTS ( SELECT TOP 1 JewelIndex FROM dbo.T_PentagramInfo  
	WHERE 	AccountID = @AccountID AND JewelPos  = @JewelPos AND JewelIndex = @JewelIndex )
	BEGIN
	
		IF (@JewelPos = 0)
		BEGIN
	
			IF EXISTS ( SELECT TOP 1 JewelIndex FROM dbo.T_PentagramInfo  
			WHERE 	AccountID = @AccountID AND Name = @Name 
			AND JewelPos  = @JewelPos AND JewelIndex = @JewelIndex )
			BEGIN
				UPDATE	dbo.T_PentagramInfo
				SET	JewelIndex = @JewelIndex
				,ItemType	= @ItemType
				,ItemIndex	= @ItemIndex
				,MainAttribute	= @MainAttribute
				,JewelLevel = @JewelLevel
				,Rank1 = @Rank1
				,Rank1Level = @Rank1Level	
				,Rank2 = @Rank2
				,Rank2Level = @Rank2Level
				,Rank3 = @Rank3
				,Rank3Level = @Rank3Level
				,Rank4 = @Rank4
				,Rank4Level = @Rank4Level
				,Rank5 = @Rank5
				,Rank5Level = @Rank5Level 
				WHERE
					AccountID = @AccountID	
				AND	Name	   = @Name		
				AND	JewelPos  = @JewelPos
				AND JewelIndex = @JewelIndex	
	
			END
			ELSE
			BEGIN
				INSERT	dbo.T_PentagramInfo
						(UserGuid,AccountID,Name,JewelPos,JewelIndex,ItemType,ItemIndex,MainAttribute,JewelLevel,Rank1, Rank1Level, Rank2, Rank2Level, Rank3, Rank3Level, Rank4, Rank4Level, Rank5, Rank5Level)
				SELECT	@UserGuid,@AccountID,@Name,@JewelPos,@JewelIndex,@ItemType,@ItemIndex,@MainAttribute,@JewelLevel,@Rank1,@Rank1Level,@Rank2,@Rank2Level,@Rank3,@Rank3Level,@Rank4,@Rank4Level,@Rank5,@Rank5Level
			END		
		END
		ELSE
		BEGIN
	
			UPDATE	dbo.T_PentagramInfo
			SET	JewelIndex = @JewelIndex
			,ItemType	= @ItemType
			,ItemIndex	= @ItemIndex
			,MainAttribute	= @MainAttribute
			,JewelLevel = @JewelLevel
			,Rank1 = @Rank1
			,Rank1Level = @Rank1Level	
			,Rank2 = @Rank2
			,Rank2Level = @Rank2Level
			,Rank3 = @Rank3
			,Rank3Level = @Rank3Level
			,Rank4 = @Rank4
			,Rank4Level = @Rank4Level
			,Rank5 = @Rank5
			,Rank5Level = @Rank5Level
			WHERE	
				AccountID = @AccountID	
			AND	JewelPos  = @JewelPos
			AND JewelIndex = @JewelIndex	
	
		END	
	
		IF @@ERROR <> 0 GOTO ERROR_HANDLING        
	  
	END
	ELSE
	BEGIN
		INSERT	dbo.T_PentagramInfo
				(UserGuid,AccountID,Name,JewelPos,JewelIndex,ItemType,ItemIndex,MainAttribute,JewelLevel,Rank1, Rank1Level, Rank2, Rank2Level, Rank3, Rank3Level, Rank4, Rank4Level, Rank5, Rank5Level)
		SELECT	@UserGuid,@AccountID,@Name,@JewelPos,@JewelIndex,@ItemType,@ItemIndex,@MainAttribute,@JewelLevel,@Rank1,@Rank1Level,@Rank2,@Rank2Level,@Rank3,@Rank3Level,@Rank4,@Rank4Level,@Rank5,@Rank5Level
		
		IF @@ERROR <> 0 GOTO ERROR_HANDLING        		   
	 
	END
	
	COMMIT TRAN
	RETURN
	
	ERROR_HANDLING:      
	ROLLBACK TRAN    
	SET @Result = 1        
	RETURN          
	        
	
	SET XACT_ABORT OFF
	SET NOCOUNT OFF


GO
/****** Object:  StoredProcedure [dbo].[IGC_PeriodBuffDelete]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO


-- =============================================
-- Author:		<Dudas [IGCN Team]>
-- Create date: <28.02.2014>
-- Description:	<PeriodBuffSystem delete procedure>
-- =============================================
CREATE PROCEDURE [dbo].[IGC_PeriodBuffDelete]
	-- Add the parameters for the stored procedure here
	@CharacterName varchar(10),
	@BuffIndex int
AS
BEGIN
	SET NOCOUNT ON;

	DELETE FROM IGC_PeriodBuffInfo WHERE [CharacterName] = @CharacterName AND [BuffIndex] = @BuffIndex

	SELECT 0 AS QueryResult

END

















GO
/****** Object:  StoredProcedure [dbo].[IGC_PeriodBuffDeleteGuild]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO


-- =============================================
-- Author:		<Dudas [IGCN Team]>
-- Create date: <28.02.2014>
-- Description:	<PeriodBuffSystem delete procedure for guilds>
-- =============================================
CREATE PROCEDURE [dbo].[IGC_PeriodBuffDeleteGuild]
	-- Add the parameters for the stored procedure here
	@BuffIndex int
AS
BEGIN
	SET NOCOUNT ON;

	DELETE FROM IGC_PeriodBuffInfo WHERE [BuffIndex] = @BuffIndex

	SELECT 0 AS QueryResult

END

















GO
/****** Object:  StoredProcedure [dbo].[IGC_PeriodBuffInsert]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO


-- =============================================
-- Author:  <Dudas>
-- Create date: <28.02.2014>
-- Description: <PeriodBuffSystem insert procedure>
-- =============================================
CREATE PROCEDURE [dbo].[IGC_PeriodBuffInsert]
 -- Add the parameters for the stored procedure here
 @CharacterName varchar(10),
 @BuffIndex smallint,
 @EffectType1 smallint,
 @EffectType2 smallint,
 @Duration int,
 @ExpireDate bigint
 
AS
BEGIN
 SET NOCOUNT ON;

    INSERT INTO [dbo].[IGC_PeriodBuffInfo]
           ([CharacterName], [BuffIndex], [EffectType1], [EffectType2], [Duration], [ExpireDate])
     VALUES (@CharacterName, @BuffIndex, @EffectType1, @EffectType2, @Duration, @ExpireDate)

	 SELECT 0 AS QueryResult
END

















GO
/****** Object:  StoredProcedure [dbo].[IGC_PeriodBuffSelect]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO


-- =============================================
-- Author:		<Dudas [IGCN Team]>
-- Create date: <28.02.2014>
-- Description:	<PeriodBuffSystem select procedure>
-- =============================================
CREATE PROCEDURE [dbo].[IGC_PeriodBuffSelect]
	-- Add the parameters for the stored procedure here
	@CharacterName varchar(10)
AS
BEGIN

	SET NOCOUNT ON;

	SELECT * FROM IGC_PeriodBuffInfo WHERE [CharacterName] = @CharacterName


END

















GO
/****** Object:  StoredProcedure [dbo].[IGC_PeriodItemDeleteEx]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

-- =============================================
-- Author:		<Dudas [IGCN Team]>
-- Create date: <04.07.2013>
-- Description:	<PeriodItemSystemEx delete procedure>
-- =============================================
CREATE PROCEDURE [dbo].[IGC_PeriodItemDeleteEx]
	-- Add the parameters for the stored procedure here
	@UserGUID int,
	@CharacterName varchar(10),
	@ItemType tinyint,
	@ItemCode smallint,
	@Serial bigint
AS
BEGIN
	DECLARE @ErrorCode int
	DECLARE @ItemInfoCount int

	SET @ErrorCode = 0
	SET @ItemInfoCount = 0

	SET NOCOUNT ON

	IF ( @ItemType = 1 ) BEGIN
		SELECT @ItemInfoCount = COUNT(*) FROM IGC_PeriodItemInfo WHERE UserGUID = @UserGUID AND ItemCode = @ItemCode
		IF ( @ItemInfoCount < 1 ) BEGIN
			SET @ErrorCode = 1
		END
		ELSE BEGIN
			UPDATE IGC_PeriodItemInfo SET UsedInfo = 0 WHERE UserGUID = @UserGUID AND CharacterName = @CharacterName AND ItemCode = @ItemCode
		END
	END
	ELSE IF ( @ItemType = 2 ) BEGIN
		SELECT @ItemInfoCount = COUNT(*) FROM IGC_PeriodItemInfo WHERE UserGUID = @UserGUID AND CharacterName = @CharacterName AND ItemCode = @ItemCode AND Serial = @Serial
		IF ( @ItemInfoCount < 1 ) BEGIN
			SET @ErrorCode = 1
		END
		ELSE BEGIN
			UPDATE IGC_PeriodItemInfo SET UsedInfo = 0 WHERE UserGUID = @UserGUID AND ItemCode = @ItemCode AND Serial = @Serial
		END
	END

	SELECT @ErrorCode

	SET NOCOUNT OFF
END




GO
/****** Object:  StoredProcedure [dbo].[IGC_PeriodItemExpiredItemSelectEx]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

-- =============================================
-- Author:		<Dudas [IGCN Team]>
-- Create date: <04.07.2013>
-- Description:	<PeriodItemSystemEx select for expired items procedure>
-- =============================================
CREATE PROCEDURE [dbo].[IGC_PeriodItemExpiredItemSelectEx]
 -- Add the parameters for the stored procedure here
 @UserGUID int,
 @CharacterName varchar(10)

AS
BEGIN
	DECLARE @ErrorCode int

	SET @ErrorCode = 0

	SET NOCOUNT ON

	UPDATE IGC_PeriodItemInfo SET SetExpire = 2 WHERE UserGUID = @UserGUID AND CharacterName = @CharacterName AND ItemType = 2 AND [ExpireDateString] < GETDATE() AND SetExpire = 1

	UPDATE IGC_PeriodItemInfo SET SetExpire = 1 WHERE UserGUID = @UserGUID AND CharacterName = @CharacterName AND ItemType = 2 AND [ExpireDateString] < GETDATE() AND SetExpire = 0

	SELECT * FROM IGC_PeriodItemInfo WHERE UserGUID = @UserGUID AND CharacterName = @CharacterName AND ItemType = 2 AND SetExpire = 1
	IF ( @@Error <> 0 ) BEGIN
		SET @ErrorCode = -1
	END

	SET NOCOUNT OFF
END




GO
/****** Object:  StoredProcedure [dbo].[IGC_PeriodItemInsertEx]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

-- =============================================
-- Author:  <Dudas (IGC)>
-- Create date: <31.08.2015>
-- Description: <PeriodItemSystemEx insert procedure (update)>
-- =============================================
CREATE PROCEDURE [dbo].[IGC_PeriodItemInsertEx]
 @UserGUID int,
 @CharacterName varchar(10),
 @ItemType smallint,
 @ItemCode smallint,
 @EffectCategory smallint,
 @EffectType1 smallint,
 @EffectType2 smallint,
 @Serial bigint,
 @Duration int,
 @BuyDate bigint,
 @ExpireDate bigint
 
AS
BEGIN
	DECLARE @ErrorCode int
	DECLARE @PeriodItemIndex int

	SET @ErrorCode = 0
	SET @PeriodItemIndex = 0

	SET XACT_ABORT ON
	SET NOCOUNT ON		
	BEGIN TRANSACTION

	DECLARE @CalcExpireDate smalldatetime

	SELECT @CalcExpireDate = DATEADD( second, @Duration, GETDATE() )

	IF ( @ItemType = 1 ) BEGIN
		SELECT @PeriodItemIndex = id FROM IGC_PeriodItemInfo WHERE UserGUID = @UserGUID AND CharacterName = @CharacterName AND UsedInfo = 1 AND EffectCategory = @EffectCategory
		IF( @PeriodItemIndex <> 0 ) BEGIN
			UPDATE IGC_PeriodItemInfo SET UsedInfo = 0 WHERE id = @PeriodItemIndex
		END

		INSERT INTO IGC_PeriodItemInfo ( UserGUID, CharacterName, ItemCode, ItemType, EffectType1, EffectType2, Duration, BuyDateString, ExpireDateString, UsedInfo, EffectCategory, Serial, BuyDate, ExpireDate )
		VALUES
		( @UserGUID, @CharacterName, @ItemCode, @ItemType, @EffectType1, @EffectType2, @Duration, GETDATE(), @CalcExpireDate, 1, @EffectCategory, @Serial, @BuyDate, @ExpireDate )
		iF ( @@Error <> 0 ) BEGIN
			SET @ErrorCode = 2
		END
	END
	ELSE IF ( @ItemType = 2 ) BEGIN
		INSERT INTO IGC_PeriodItemInfo ( UserGUID, CharacterName, ItemCode, ItemType, EffectType1, EffectType2, Duration, BuyDateString, ExpireDateString, UsedInfo, EffectCategory, Serial, BuyDate, ExpireDate )
		VALUES
		( @UserGUID, @CharacterName, @ItemCode, @ItemType, 0, 0, @Duration, GETDATE(), @CalcExpireDate, 1, 0, @Serial, @BuyDate, @ExpireDate )
		iF ( @@Error <> 0 ) BEGIN
			SET @ErrorCode = 2
		END
	END

	IF ( @ErrorCode <> 0 )
		ROLLBACK TRANSACTION
	ELSE
		COMMIT TRANSACTION

	SELECT @ErrorCode

	SET NOCOUNT OFF
	SET XACT_ABORT OFF
END




GO
/****** Object:  StoredProcedure [dbo].[IGC_PeriodItemSelectEx]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

-- =============================================
-- Author:		<Dudas [IGCN Team]>
-- Create date: <04.07.2013>
-- Description:	<PeriodItemSystemEx select procedure>
-- =============================================
CREATE PROCEDURE [dbo].[IGC_PeriodItemSelectEx]
	-- Add the parameters for the stored procedure here
	@UserGUID int,
	@CharacterName varchar(10)
AS
BEGIN
	DECLARE @ErrorCode int

	SET @ErrorCode = 0

	SET NOCOUNT ON

	UPDATE IGC_PeriodItemInfo SET UsedInfo = 0 WHERE UserGUID = @UserGUID AND CharacterName = @CharacterName AND UsedInfo = 1 AND ExpireDateString < GetDate()

	SELECT * FROM IGC_PeriodItemInfo WHERE UserGUID = @UserGUID AND CharacterName = @CharacterName AND UsedInfo = 1
	IF ( @@Error <> 0 ) BEGIN
		SET @ErrorCode = -1
	END

	SET NOCOUNT OFF
END




GO
/****** Object:  StoredProcedure [dbo].[IGC_PShopItemMove]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE PROC [dbo].[IGC_PShopItemMove] (
	@AccountID	VARCHAR(10)
,	@Name		VARCHAR(10)

,	@OldInvenNum		int
,	@NewInvenNum		int
)
AS  
	SET NOCOUNT ON
	SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED

	IF EXISTS ( SELECT * FROM T_PSHOP_ITEMVALUE_INFO WHERE AccountID = @AccountID AND Name = @Name AND ItemInvenNum = @NewInvenNum )
	BEGIN
		DELETE FROM T_PSHOP_ITEMVALUE_INFO WHERE AccountID = @AccountID AND Name = @Name AND ItemInvenNum = @NewInvenNum
	END

	UPDATE dbo.T_PSHOP_ITEMVALUE_INFO
	SET 
			ItemInvenNum = @NewInvenNum
	WHERE AccountID = @AccountID AND Name = @Name AND ItemInvenNum = @OldInvenNum














GO
/****** Object:  StoredProcedure [dbo].[IGC_PShopItemValueInfoDel]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE PROC [dbo].[IGC_PShopItemValueInfoDel] (
	@AccountID	VARCHAR(10)
,	@Name		VARCHAR(10)

,	@ItemInvenNum		int
)
AS  
	SET NOCOUNT ON
	SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED

	DELETE FROM dbo.T_PSHOP_ITEMVALUE_INFO WHERE AccountID = @AccountID AND Name = @Name AND ItemInvenNum = @ItemInvenNum














GO
/****** Object:  StoredProcedure [dbo].[IGC_PShopItemValueInfoLoad]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE PROC [dbo].[IGC_PShopItemValueInfoLoad] (
	@AccountID varchar(10)
,	@Name		VARCHAR(10)
)
AS  
	SET NOCOUNT ON
	SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED

	SELECT 
		ItemInvenNum, ItemSerial, Money, BlessJewelValue, SoulJewelValue, ChaosJewelValue
	FROM dbo.T_PSHOP_ITEMVALUE_INFO WHERE AccountID = @AccountID AND Name = @Name














GO
/****** Object:  StoredProcedure [dbo].[IGC_PShopItemValueInfoSave]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE PROC [dbo].[IGC_PShopItemValueInfoSave] (
	@AccountID	VARCHAR(10)
,	@Name		VARCHAR(10)

,	@ItemInvenNum		int
,	@ItemSerial			bigint
,	@Money				int
,	@BlessJewelValue	int
,	@SoulJewelValue		int
,	@ChaosJewelValue	int
)
AS  
	SET NOCOUNT ON
	SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED

	UPDATE dbo.T_PSHOP_ITEMVALUE_INFO
	SET 
			ItemInvenNum = @ItemInvenNum
		,	ItemSerial = @ItemSerial
		,	Money = @Money
		,	BlessJewelValue = @BlessJewelValue
		,	SoulJewelValue = @SoulJewelValue
		,	ChaosJewelValue = @ChaosJewelValue
	WHERE AccountID = @AccountID AND Name = @Name AND ItemInvenNum = @ItemInvenNum
	IF @@ROWCOUNT = 0 
	BEGIN
		INSERT INTO dbo.T_PSHOP_ITEMVALUE_INFO 
		( AccountID, Name, ItemInvenNum, ItemSerial, Money, BlessJewelValue, SoulJewelValue, ChaosJewelValue )
		VALUES ( @AccountID,@Name, @ItemInvenNum,@ItemSerial,@Money,@BlessJewelValue,@SoulJewelValue,@ChaosJewelValue );
	END
	














GO
/****** Object:  StoredProcedure [dbo].[IGC_QuestExpUserInfoLoad]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS OFF
GO
SET QUOTED_IDENTIFIER OFF
GO


CREATE  proc [dbo].[IGC_QuestExpUserInfoLoad]
 @CharacterName varchar(10)
AS
SELECT EPISODE, QUEST_SWITCH, PROG_STATE, ASK_INDEX, ASK_VALUE, ASK_STATE 
FROM T_QUEST_EXP_INFO WHERE CHAR_NAME = @CharacterName














GO
/****** Object:  StoredProcedure [dbo].[IGC_QuestExpUserInfoLoad_Fir]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO


CREATE PROC [dbo].[IGC_QuestExpUserInfoLoad_Fir]
@CharacterName VARCHAR(10)  
AS  
SELECT  CHAR_NAME, EPISODE, QUEST_SWITCH, PROG_STATE
,StartDateConvert,EndDateConvert  
FROM T_QUEST_EXP_INFO
WHERE CHAR_NAME = @CharacterName














GO
/****** Object:  StoredProcedure [dbo].[IGC_QuestExpUserInfoLoad_Sec]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO


CREATE PROCEDURE [dbo].[IGC_QuestExpUserInfoLoad_Sec]
 @CharacterName varchar(10),
 @Episode	int,
 @Ask_Index	varbinary(5)		OUTPUT,
 @Ask_Value	varbinary(5)		OUTPUT,
 @Ask_State	varbinary(5)		OUTPUT
AS

SELECT @Ask_Index = ASK_INDEX, @Ask_Value = ASK_VALUE, @Ask_State = ASK_STATE  FROM T_QUEST_EXP_INFO  WHERE CHAR_NAME =  @CharacterName AND EPISODE = @Episode














GO
/****** Object:  StoredProcedure [dbo].[IGC_QuestExpUserInfoSave]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO


CREATE  PROC [dbo].[IGC_QuestExpUserInfoSave]
@CharacterName	varchar(10)
,@Episode		smallint      
,@Quest_Switch	smallint     
,@ProgState		smallint
,@StartDate		bigint	--StartDateConvert
,@EndDate 		bigint	--EndDateConvert  
,@AskIndex		varbinary(5)  
,@AskValue		varbinary(5)  
,@AskState		varbinary(5)  
AS       
SET NOCOUNT ON

DECLARE	@ErrorCode	int      
SET	@ErrorCode = 0      
      
BEGIN TRANSACTION

IF NOT EXISTS ( SELECT EPISODE FROM T_QUEST_EXP_INFO  WITH ( READUNCOMMITTED )       
WHERE EPISODE = @Episode AND CHAR_NAME = @CharacterName )  
BEGIN        
INSERT T_QUEST_EXP_INFO 
(CHAR_NAME,EPISODE,QUEST_SWITCH,PROG_STATE
,StartDateConvert,EndDateConvert 
,ASK_INDEX,ASK_VALUE,ASK_STATE) 
SELECT	
@CharacterName,@Episode,@Quest_Switch,@ProgState
,@StartDate,@EndDate
,@AskIndex,@AskValue,@AskState  
END      

ELSE      
BEGIN 
UPDATE	T_QUEST_EXP_INFO 
SET	QUEST_SWITCH = @Quest_Switch,PROG_STATE = @ProgState
,StartDateConvert = @StartDate
,EndDateConvert = @EndDate
,ASK_INDEX = @AskIndex,ASK_VALUE = @AskValue,ASK_STATE = @AskState
WHERE	EPISODE = @Episode AND CHAR_NAME = @CharacterName  
END      

IF(@@Error <> 0)      
BEGIN      
SET @ErrorCode = 2      
END      

IF(@@Error <> 0)
ROLLBACK TRANSACTION
ELSE
COMMIT TRANSACTION
   
SELECT @ErrorCode      

SET NOCOUNT OFF














GO
/****** Object:  StoredProcedure [dbo].[IGC_SetDeleteMuRummy]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE PROC [dbo].[IGC_SetDeleteMuRummy] (
	@AccountID	VARCHAR(10)
,	@Name		VARCHAR(10)
)
AS  
	SET NOCOUNT ON
	SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED

	DELETE FROM dbo.T_MuRummyInfo WHERE AccountID = @AccountID AND Name = @Name
	DELETE FROM dbo.T_MuRummy WHERE AccountID = @AccountID AND Name = @Name














GO
/****** Object:  StoredProcedure [dbo].[IGC_SetGuildMatchingWaitState]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE    PROCEDURE [dbo].[IGC_SetGuildMatchingWaitState]
   @Name varchar(10),
   @State	  tinyint
as 
BEGIN

	SET NOCOUNT ON
	SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED

    IF EXISTS ( select Name from dbo.Character where Name = @Name)
     BEGIN
	 update  dbo.IGC_WaitGuildMatching set State = @State where ApplicantName = @Name 
	 select  0 as result
     END
     ELSE
     BEGIN
	 delete  dbo.IGC_WaitGuildMatching  where ApplicantName = @Name 
	 select -1 as result
     END
    SET NOCOUNT OFF
END













GO
/****** Object:  StoredProcedure [dbo].[IGC_SetInsertMuRummy]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE PROC [dbo].[IGC_SetInsertMuRummy] (
	@AccountID	VARCHAR(10)
,	@Name		VARCHAR(10)
,	@Number		INT
,	@Color		INT
,	@Position	INT
,	@Status		INT
,	@Sequence	INT
)
AS  
	SET NOCOUNT ON
	SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED

	INSERT INTO dbo.T_MuRummy ( AccountID, Name, mNumber, mColor, mPosition, mStatus, mSequence)
	VALUES (@AccountID,@Name,@Number,@Color,@Position,@Status,@Sequence);













GO
/****** Object:  StoredProcedure [dbo].[IGC_SetSaveEventInventory]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE PROC [dbo].[IGC_SetSaveEventInventory] (
	@AccountID	VARCHAR(10)
,	@Name		VARCHAR(10)
,	@Inventory	VARBINARY(1024)	-- 32 * 32
)
AS  
	SET NOCOUNT ON
	SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED

	UPDATE dbo.T_Event_Inventory SET Inventory = @Inventory WHERE AccountID = @AccountID AND Name = @Name
	IF @@ROWCOUNT = 0
	BEGIN
		SET @Inventory = 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
		
		INSERT INTO dbo.T_Event_Inventory ( AccountID, Name, Inventory)
		VALUES (@AccountID,@Name,@Inventory);
	END














GO
/****** Object:  StoredProcedure [dbo].[IGC_SetSaveMuRummyInfo]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE PROC [dbo].[IGC_SetSaveMuRummyInfo] (
	@AccountID	VARCHAR(10)
,	@Name		VARCHAR(10)
,	@TotalScore	INT
)
AS  
	SET NOCOUNT ON
	SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED

	UPDATE dbo.T_MuRummyInfo SET mTotalScore = @TotalScore WHERE AccountID = @AccountID AND Name = @Name
	IF @@ROWCOUNT = 0
	BEGIN
		INSERT INTO dbo.T_MuRummyInfo ( AccountID, Name, mTotalScore)
		VALUES (@AccountID,@Name,0);
	END













GO
/****** Object:  StoredProcedure [dbo].[IGC_SetSaveMuRummyLog]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE PROC [dbo].[IGC_SetSaveMuRummyLog] (
	@AccountID	VARCHAR(10)
,	@Name		VARCHAR(10)
,	@Score	INT
)
AS  
	SET NOCOUNT ON
	SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED

	INSERT INTO dbo.T_MuRummyLog ( mDate, AccountID, Name, mScore)	VALUES (GETDATE(), @AccountID,@Name,@Score);














GO
/****** Object:  StoredProcedure [dbo].[IGC_SetSelectMuRummy]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE PROC [dbo].[IGC_SetSelectMuRummy] (
	@AccountID	VARCHAR(10)
,	@Name		VARCHAR(10)
)
AS  
	SET NOCOUNT ON
	SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED

	SELECT a.AccountID,a.Name,a.mTotalScore,b.mColor,b.mNumber,b.mPosition,b.mStatus,b.mSequence
	FROM dbo.T_MuRummyInfo a INNER JOIN dbo.T_MuRummy b ON a.Name = b.Name
	WHERE a.AccountID = @AccountID AND a.Name = @Name















GO
/****** Object:  StoredProcedure [dbo].[IGC_SetUpdateMuRummy]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE PROC [dbo].[IGC_SetUpdateMuRummy] (
	@AccountID	VARCHAR(10)
,	@Name		VARCHAR(10)
,	@Position	INT
,	@Status		INT
,	@Sequence	INT
)
AS  
	SET NOCOUNT ON
	SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED

	UPDATE dbo.T_MuRummy SET mPosition = @Position, mStatus = @Status 
	WHERE AccountID = @AccountID AND Name = @Name AND mSequence = @Sequence














GO
/****** Object:  StoredProcedure [dbo].[IGC_UpdateGuildMatchingMemberCount]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER OFF
GO

CREATE    PROCEDURE [dbo].[IGC_UpdateGuildMatchingMemberCount]
   @GuildName	 varchar(8),
   @MemberCount tinyint
as 
BEGIN

       SET NOCOUNT ON
       SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED
       UPDATE IGC_GuildMatching SET GuildMemberCnt = @MemberCount where GuildName = @GuildName
   
END













GO
/****** Object:  StoredProcedure [dbo].[IGC_UpdatePartyMatchingMemberCount]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE    PROCEDURE [dbo].[IGC_UpdatePartyMatchingMemberCount]
   @LeaderName	 varchar(10),
   @MemberCount tinyint
as 
BEGIN
	SET NOCOUNT ON
	SET TRANSACTION ISOLATION LEVEL READ UNCOMMITTED

     UPDATE dbo.IGC_PartyMatching SET CurMemberCount = @MemberCount where PartyLeaderName = @LeaderName
   
END













GO
/****** Object:  StoredProcedure [dbo].[IGC_VipAdd]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

-- =============================================
-- Author:  Andrzej Erenc @ awHost.pl
-- Create date: 31.05.2013
-- Description: Vip Add Procedure
-- =============================================
CREATE PROCEDURE [dbo].[IGC_VipAdd]
 -- Add the parameters for the stored procedure here
 @szCharName varchar(10),
 @DayAdd int,
 @VipType smallint
AS
BEGIN
 -- SET NOCOUNT ON added to prevent extra result sets from
 -- interfering with SELECT statements.
 SET NOCOUNT ON;
BEGIN TRANSACTION;

IF NOT EXISTS (SELECT * FROM T_VIPList  WITH ( READUNCOMMITTED ) WHERE AccountID = @szCharName)
 INSERT INTO T_VIPList (AccountID,Date,Type) VALUES (@szCharName,DateAdd(dd, @DayAdd, GETDATE()),@VipType)
ELSE IF EXISTS (SELECT * FROM T_VIPList  WITH ( READUNCOMMITTED ) WHERE AccountID = @szCharName AND Date > GETDATE())
 BEGIN
  UPDATE T_VIPList SET Date = DateAdd(dd, @DayAdd, Date) WHERE AccountID = @szCharName
  UPDATE T_VIPList SET Type = @VipType WHERE AccountID = @szCharName AND Type < @VipType
 END
ELSE UPDATE T_VIPList SET Date = DateAdd(dd, @DayAdd, GETDATE()), Type = @VipType WHERE AccountID = @szCharName

 
 IF(@@Error <> 0 )
  ROLLBACK TRANSACTION
 ELSE 
  COMMIT TRANSACTION
 SET NOCOUNT OFF 
END

















GO
/****** Object:  StoredProcedure [dbo].[MD5CREATEACC]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE PROCEDURE [dbo].[MD5CREATEACC]
@accountid varchar(10),
@pass varchar(10),
@mail_addr varchar(50),
@personalid char(13)

as
begin
DECLARE @HashThis varbinary(16)
DECLARE @Hash varbinary(16)
DECLARE @salt varchar(32)
SET @salt = '1234567890' -- your own salt, change this
set @HashThis = HashBytes('MD5',@accountid+@pass+@salt);
IF EXISTS(SELECT * from MEMB_INFO WHERE memb___id = @accountid)
BEGIN
	SELECT 0 AS Result
END
ELSE
BEGIN
	INSERT INTO MEMB_INFO(memb___id, memb__pwd, mail_addr, sno__numb, ctl1_code, bloc_code, memb_name) VALUES (@accountid, @HashThis, @mail_addr, @personalid, 0, 0, @accountid)
	select 1 as Result
END
SET NOCOUNT OFF;
end

















GO
/****** Object:  StoredProcedure [dbo].[MD5HASH3]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE PROCEDURE [dbo].[MD5HASH3]
@accountid varchar(10),
@pass_salted varchar(50)
as
begin
DECLARE @HashThis varbinary(16)
DECLARE @Hash varbinary(16)
set @HashThis = HashBytes('MD5',@accountid+@pass_salted);
set @Hash = (select memb__pwd from MEMB_INFO where memb___id=@accountid);
if
@HashThis = @Hash
SELECT 1 AS QueryResult
else
SELECT 0 AS QueryResult
 SET NOCOUNT OFF;
end

















GO
/****** Object:  StoredProcedure [dbo].[VIPSystem_CheckAccount]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE PROCEDURE [dbo].[VIPSystem_CheckAccount]
 -- Add the parameters for the stored procedure here
 @Account varchar(10)
AS
BEGIN
 -- SET NOCOUNT ON added to prevent extra result sets from
 -- interfering with SELECT statements.
 SET NOCOUNT ON;

    -- Insert statements for procedure here
 DECLARE @DateInfo smalldatetime
 DECLARE @VipType tinyint
 
 
 SELECT @DateInfo = Date, @VipType = Type FROM T_VIPList WHERE AccountID = @Account
 
 IF (DATEDIFF(MINUTE, GETDATE(), @DateInfo) > 0)
 BEGIN
  SELECT 1 AS Result, @DateInfo AS DateTime, @VipType AS Type
 END
 ELSE
 BEGIN
  SELECT 0 AS Result
 END
END
















GO
/****** Object:  StoredProcedure [dbo].[WZ_CharMoveReset]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE Procedure [dbo].[WZ_CharMoveReset] 

	@AccountID		varchar(10),
	@Name			varchar(10)
AS
BEGIN
	SET NOCOUNT ON
	SET XACT_ABORT ON	
	
	DECLARE	@Result			tinyint	
	DECLARE	@ResultLowCount		int
	DECLARE	@Class				tinyint
	DECLARE	@Ctl1_Code			tinyint
	DECLARE	@SQLEXEC			varchar(1000)
	DECLARE	@ErrorCheck			INT
	DECLARE 	@g1 				varchar(10), @g2 varchar(10), @g3 varchar(10), @g4 varchar(10), @g5 varchar(10)
	DECLARE 	@MoveCnt			 tinyint		
	DECLARE 	@ChangeMoveCnt		tinyint		
	DECLARE	@SqlStmt			VARCHAR(700)		
	DECLARE	@SqlStmt2			VARCHAR(700)		

	SET LOCK_TIMEOUT	1000
	SET @Result = 0x00	
	SET @ErrorCheck = 0x00

	SELECT @Class = Class, @Ctl1_Code = CtlCode FROM Character WHERE Name = @Name
	
	SELECT @ResultLowCount = @@rowcount, @ErrorCheck = @@error
					
	IF @ResultLowCount = 0 
	BEGIN
		SET @Result	= 0x02			
		GOTO ON_ERROR						
	END

	IF @ErrorCheck  <> 0 GOTO ON_ERROR

	IF  ( (@Ctl1_Code & 127 ) > 0 )
	BEGIN
		SET @Result	= 0x03			
		GOTO ON_ERROR						
	END 

	SELECT  @g1=GameID1, @g2=GameID2, @g3=GameID3, @g4=GameID4, @g5=GameID5, @MoveCnt = MoveCnt 
	FROM dbo.AccountCharacter WHERE Id = @AccountID 		
	
	SELECT @ResultLowCount = @@rowcount, @ErrorCheck = @@error

	IF @ResultLowCount = 0 
	BEGIN
		SET @Result	= 0x02			
		GOTO ON_ERROR						
	END

	IF @ErrorCheck  <> 0 GOTO ON_ERROR

	SET @MoveCnt =  0

	SET @SqlStmt = 'UPDATE AccountCharacter  '

	IF ( @g1 = @Name )
		SET @SqlStmt = @SqlStmt + ' SET  '
	ELSE IF ( @g2 = @Name )
		SET @SqlStmt = @SqlStmt + ' SET  '
	ELSE IF ( @g3 = @Name )
		SET @SqlStmt = @SqlStmt + ' SET  '
	ELSE IF ( @g4 = @Name )
		SET @SqlStmt = @SqlStmt + ' SET  '
	ELSE IF ( @g5 = @Name )
		SET @SqlStmt = @SqlStmt + ' SET  '
	ELSE 				
		SET @Result	= 0x05

	IF ( @Result <> 0 )
		GOTO ON_ERROR

	SET @SqlStmt = @SqlStmt + ' MoveCnt =  ' + CONVERT(VARCHAR, @MoveCnt )					
	SET @SqlStmt = @SqlStmt + ' WHERE Id =  ''' + @AccountID	 + ''''				
	SET @SqlStmt2 = 'UPDATE Character '
	SET @SqlStmt2 = @SqlStmt2 + 'SET  '
	SET @SqlStmt2 = @SqlStmt2 + 'CtlCode = ' + CONVERT(VARCHAR, @Ctl1_Code & 127)
	SET @SqlStmt2 = @SqlStmt2 + ' WHERE Name = ''' +  @Name + ''''

	BEGIN TRANSACTION 

	EXEC(@SqlStmt)
	SELECT @ResultLowCount = @@rowcount,  @ErrorCheck = @@error
	IF  @ResultLowCount = 0  GOTO ON_TRN_ERROR
	IF  @ErrorCheck  <> 0 GOTO ON_TRN_ERROR

	EXEC(@SqlStmt2)
	SELECT @ResultLowCount = @@rowcount,  @ErrorCheck = @@error
	IF  @ResultLowCount = 0  GOTO ON_TRN_ERROR
	IF  @ErrorCheck  <> 0 GOTO ON_TRN_ERROR


ON_TRN_ERROR:
	IF ( @Result  <> 0 ) OR (@ErrorCheck <> 0)
	BEGIN
		IF @Result = 0 
			SET @Result = 0x09

		ROLLBACK TRAN
	END
	ELSE
		COMMIT TRAN

ON_ERROR:
	IF @ErrorCheck <> 0
	BEGIN
		SET @Result = 0x09
	END 

	SELECT @Result	

	SET NOCOUNT OFF
	SET XACT_ABORT OFF
END


















GO
/****** Object:  StoredProcedure [dbo].[WZ_CONNECT_MEMB]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE Procedure [dbo].[WZ_CONNECT_MEMB]

	@memb___id 	varchar(10),
	@ServerName  	varchar(20),
	@IP 		varchar(15)	
AS
BEGIN

SET NOCOUNT ON
	DECLARE @find_id 		varchar(10)	
	DECLARE @ConnectStat	 tinyint
	SET @find_id = 'NOT'
	SET @ConnectStat = 1

	SELECT @find_id = S.memb___id FROM MEMB_STAT S INNER JOIN MEMB_INFO I ON S.memb___id = I.memb___id 
	      WHERE I.memb___id = @memb___id

	IF( @find_id = 'NOT' )
	BEGIN
		INSERT INTO MEMB_STAT (memb___id,ConnectStat,ServerName,IP,ConnectTM)
		values(@memb___id,  @ConnectStat, @ServerName, @IP, getdate())
	END
	ELSE	
		UPDATE MEMB_STAT SET ConnectStat = @ConnectStat,
					 ServerName = @ServerName,IP = @IP,
					 ConnectTM = getdate()
       	WHERE memb___id = @memb___id
END


















GO
/****** Object:  StoredProcedure [dbo].[WZ_CreateCharacter]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER OFF
GO


CREATE Procedure [dbo].[WZ_CreateCharacter] 

	@AccountID		varchar(10),
	@Name			varchar(10),
	@Class			tinyint
AS
BEGIN

	SET NOCOUNT ON
	SET	XACT_ABORT ON
	DECLARE		@Result		tinyint
	SET @Result = 0x00

	IF EXISTS ( SELECT Name FROM Character WHERE Name = @Name )
	BEGIN
		SET @Result	= 0x01
		GOTO ProcEnd						
	END

	BEGIN TRAN

	IF NOT EXISTS ( SELECT Id FROM  AccountCharacter WHERE Id = @AccountID )
		BEGIN
			INSERT INTO dbo.AccountCharacter(Id, GameID1, GameID2, GameID3, GameID4, GameID5, GameIDC) 
			VALUES(@AccountID, @Name, NULL, NULL, NULL, NULL, NULL)

			SET @Result  = @@Error
		END
	ELSE
		BEGIN
			Declare @g1 varchar(10), @g2 varchar(10), @g3 varchar(10), @g4 varchar(10), @g5 varchar(10)						
			SELECT @g1=GameID1, @g2=GameID2, @g3=GameID3, @g4=GameID4, @g5=GameID5 FROM dbo.AccountCharacter Where Id = @AccountID 			

			IF( ( @g1 Is NULL) OR (Len(@g1) = 0))
				BEGIN
					UPDATE AccountCharacter SET  GameID1 = @Name
					WHERE Id = @AccountID
										
					SET @Result  = @@Error
				END
			ELSE IF( @g2  Is NULL OR Len(@g2) = 0)
				BEGIN
					UPDATE AccountCharacter SET  GameID2 = @Name
					WHERE Id = @AccountID

					SET @Result  = @@Error
				END
			ELSE IF( @g3  Is NULL OR Len(@g3) = 0)
				BEGIN	
					UPDATE AccountCharacter SET  GameID3 = @Name
					WHERE Id = @AccountID

					SET @Result  = @@Error
				END
			ELSE IF( @g4 Is NULL OR Len(@g4) = 0)
				BEGIN
					UPDATE AccountCharacter SET  GameID4 = @Name
					WHERE Id = @AccountID

					SET @Result  = @@Error
				END
			ELSE IF( @g5 Is NULL OR Len(@g5) = 0)
				BEGIN
					UPDATE AccountCharacter SET  GameID5 = @Name
					WHERE Id = @AccountID

					SET @Result  = @@Error
				END	
			ELSE
				BEGIN		
					SET @Result	= 0x03							
					GOTO TranProcEnd								
				END	 
		END

	IF( @Result <> 0 )
		BEGIN
			GOTO TranProcEnd		
		END
	ELSE
		BEGIN
			INSERT INTO dbo.Character(AccountID, Name, cLevel, LevelUpPoint, Class, Strength, Dexterity, Vitality, Energy, Inventory,MagicList,
					Life, MaxLife, Mana, MaxMana, MapNumber, MapPosX, MapPosY,  MDate, LDate, Quest, Leadership )
				
			SELECT @AccountID As AccountID, @Name As Name, Level, LevelUpPoint, @Class As Class, 
				Strength, Dexterity, Vitality, Energy, Inventory,MagicList, Life, MaxLife, Mana, MaxMana, MapNumber, MapPosX, MapPosY,
				getdate() As MDate, getdate() As LDate, Quest, Leadership
			FROM  DefaultClassType WHERE Class = @Class

			SET @Result  = @@Error
		END
		
		 IF NOT EXISTS ( SELECT AccountID FROM T_InGameShop_Point WHERE AccountID = @AccountID )
            BEGIN
               INSERT INTO dbo.T_InGameShop_Point (AccountID, WCoinC, WCoinP, GoblinPoint)
               VALUES (@AccountID, 0, 0, 0)
            END

TranProcEnd:
	IF ( @Result  <> 0 )
		ROLLBACK TRAN
	ELSE
		COMMIT TRAN

ProcEnd:
	SET NOCOUNT OFF
	SET XACT_ABORT OFF

	SELECT
	   CASE @Result
	      WHEN 0x00 THEN 0x01
	      WHEN 0x01 THEN 0x00
	      WHEN 0x03 THEN 0x03
	      ELSE 0x02
	   END AS Result 
END


GO
/****** Object:  StoredProcedure [dbo].[WZ_CreateCharacter_GetVersion]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE Procedure [dbo].[WZ_CreateCharacter_GetVersion]
AS
BEGIN
	SELECT 1
END


















GO
/****** Object:  StoredProcedure [dbo].[WZ_CS_CheckSiegeGuildList]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS OFF
GO
SET QUOTED_IDENTIFIER OFF
GO
CREATE Procedure [dbo].[WZ_CS_CheckSiegeGuildList]

	@szGuildName		varchar(8)
AS
BEGIN
	BEGIN TRANSACTION
	
	SET NOCOUNT ON

	DECLARE @iEnd INT

	SELECT @iEnd = SIEGE_ENDED FROM MuCastle_DATA

	IF @iEnd = 1
	BEGIN
		SELECT 0 As QueryResult
	END
	ELSE IF EXISTS ( SELECT GUILD_NAME FROM MuCastle_SIEGE_GUILDLIST  WITH (READUNCOMMITTED) 
				WHERE GUILD_NAME = @szGuildName)
	BEGIN
		SELECT 1 As QueryResult	
	END
	ELSE
	BEGIN
		IF EXISTS ( SELECT REG_SIEGE_GUILD FROM MuCastle_REG_SIEGE WITH (READUNCOMMITTED) 
				WHERE REG_SIEGE_GUILD = @szGuildName AND IS_GIVEUP = 0)
		BEGIN
			SELECT 1 As QueryResult
		END
		ELSE
		BEGIN
			SELECT 0 As QueryResult	
		END
	END


	IF(@@Error <> 0 )
		ROLLBACK TRANSACTION
	ELSE	
		COMMIT TRANSACTION

	SET NOCOUNT OFF	
END


















GO
/****** Object:  StoredProcedure [dbo].[WZ_CS_GetAllGuildMarkRegInfo]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS OFF
GO
SET QUOTED_IDENTIFIER OFF
GO
CREATE Procedure [dbo].[WZ_CS_GetAllGuildMarkRegInfo]

	@iMapSvrGroup		SMALLINT
AS
BEGIN
	BEGIN TRANSACTION
	
	SET NOCOUNT ON	

	SELECT TOP 100 * FROM MuCastle_REG_SIEGE WITH (READUNCOMMITTED)
	WHERE MAP_SVR_GROUP = @iMapSvrGroup
	ORDER BY SEQ_NUM DESC

	IF(@@Error <> 0 )
		ROLLBACK TRANSACTION
	ELSE	
		COMMIT TRANSACTION

	SET NOCOUNT OFF	
END


















GO
/****** Object:  StoredProcedure [dbo].[WZ_CS_GetCalcRegGuildList]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS OFF
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE Procedure [dbo].[WZ_CS_GetCalcRegGuildList]

	@iMapSvrGroup		SMALLINT
AS
BEGIN
	BEGIN TRANSACTION
	
	SET NOCOUNT ON	

	DECLARE T_CURSOR CURSOR FAST_FORWARD
	FOR SELECT TOP 100 * FROM MuCastle_REG_SIEGE	WHERE MAP_SVR_GROUP = @iMapSvrGroup AND IS_GIVEUP = 0 ORDER BY SEQ_NUM DESC
	
	OPEN T_CURSOR
	
	DECLARE	@iMapSvrNum			INT
	DECLARE	@szRegGuild			VARCHAR(8)
	DECLARE	@iRegMarks			INT
	DECLARE	@iIsGiveUp			INT
	DECLARE	@iSeqNum			INT
	DECLARE	@iGuildMemberCount		INT
	DECLARE	@iGuildMasterLevel		INT

	CREATE TABLE #T_REG_GUILDLIST  (
		[REG_SIEGE_GUILD] [varchar] (8) NOT NULL ,
		[REG_MARKS] [int] NOT NULL ,
		[GUILD_MEMBER] [int] NOT NULL ,
		[GM_LEVEL] [int] NOT NULL ,
		[SEQ_NUM] [int] NOT NULL 
	) ON [PRIMARY]
	
	FETCH FROM T_CURSOR INTO @iMapSvrNum, @szRegGuild, @iRegMarks, @iIsGiveUp, @iSeqNum
	WHILE (@@FETCH_STATUS = 0)
	BEGIN
		IF EXISTS ( SELECT G_Name FROM Guild  WITH (READUNCOMMITTED) WHERE G_Name = @szRegGuild)
		BEGIN
			DECLARE @szGuildMaster	VARCHAR(10)
			SELECT @szGuildMaster = G_Master FROM Guild  WHERE G_Name = @szRegGuild

			IF EXISTS ( SELECT Name FROM Character WITH (READUNCOMMITTED) WHERE Name = @szGuildMaster)
			BEGIN
				SELECT @iGuildMemberCount = COUNT(*) FROM GuildMember WHERE G_Name = @szRegGuild
				SELECT @iGuildMasterLevel = cLevel FROM Character WHERE Name = @szGuildMaster

				INSERT INTO #T_REG_GUILDLIST VALUES (@szRegGuild, @iRegMarks, @iGuildMemberCount, @iGuildMasterLevel, @iSeqNum)
			END
		END
		
		FETCH FROM T_CURSOR INTO @iMapSvrGroup, @szRegGuild, @iRegMarks, @iIsGiveUp, @iSeqNum
	END
	
	CLOSE T_CURSOR
	
	DEALLOCATE T_CURSOR

	SELECT * FROM #T_REG_GUILDLIST

	DROP TABLE #T_REG_GUILDLIST

	IF(@@Error <> 0 )
		ROLLBACK TRANSACTION
	ELSE	
		COMMIT TRANSACTION

	SET NOCOUNT OFF	
END


















GO
/****** Object:  StoredProcedure [dbo].[WZ_CS_GetCastleMoneySts]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS OFF
GO
SET QUOTED_IDENTIFIER OFF
GO
CREATE Procedure [dbo].[WZ_CS_GetCastleMoneySts]

	@iMapSvrGroup		SMALLINT,
	@iTaxDate		DATETIME
AS
BEGIN
	BEGIN TRANSACTION
	
	SET NOCOUNT ON
	
	DECLARE	@iTaxInc		MONEY
	DECLARE	@iTaxDec		MONEY
	DECLARE	@iYEAR		INT
	DECLARE	@iMONTH		INT
	DECLARE	@iDAY			INT
	DECLARE	@dtLogDateStart	DATETIME
	DECLARE	@dtLogDateEnd	DATETIME
	SELECT	@iYEAR		= DATEPART(YY, @iTaxDate)
	SELECT	@iMONTH		= DATEPART(MM, @iTaxDate)
	SELECT	@iDAY			= DATEPART(DD, @iTaxDate)
	SET		@dtLogDateStart	= CAST(@iYEAR AS VARCHAR(4)) + '-' + CAST(@iMONTH AS VARCHAR(2))  + '-' + CAST(@iDAY AS VARCHAR(4)) + ' 00:00:00'
	SET		@dtLogDateEnd	= CAST(@iYEAR AS VARCHAR(4)) + '-' + CAST(@iMONTH AS VARCHAR(2))  + '-' + CAST(@iDAY AS VARCHAR(4)) + ' 23:59:59'
	
	SELECT @iTaxInc = SUM(MONEY_CHANGE) FROM MuCastle_MONEY_STATISTICS  WITH (READUNCOMMITTED) 
	WHERE MAP_SVR_GROUP = 0 and LOG_DATE BETWEEN @dtLogDateStart AND @dtLogDateEnd and MONEY_CHANGE >= 0
	
	SELECT @iTaxDec = SUM(MONEY_CHANGE) FROM MuCastle_MONEY_STATISTICS  WITH (READUNCOMMITTED) 
	WHERE MAP_SVR_GROUP = 0 and LOG_DATE BETWEEN @dtLogDateStart AND @dtLogDateEnd and MONEY_CHANGE < 0
	
	IF @iTaxInc IS NULL
		SET @iTaxInc = 0
	IF @iTaxDec IS NULL
		SET @iTaxDec = 0

	SELECT @dtLogDateStart As TaxDate, @iTaxInc As TaxInc, @iTaxDec As TaxDec

	IF(@@Error <> 0 )
		ROLLBACK TRANSACTION
	ELSE	
		COMMIT TRANSACTION

	SET NOCOUNT OFF	
END


















GO
/****** Object:  StoredProcedure [dbo].[WZ_CS_GetCastleMoneyStsRange]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS OFF
GO
SET QUOTED_IDENTIFIER OFF
GO
CREATE Procedure [dbo].[WZ_CS_GetCastleMoneyStsRange]

	@iMapSvrGroup		SMALLINT,
	@iTaxDateStart		DATETIME,
	@iTaxDateEnd		DATETIME
AS
BEGIN
	BEGIN TRANSACTION
	
	SET NOCOUNT ON

	CREATE TABLE #T_REG_TAXSTT  (
		[TaxDate] [datetime] NOT NULL ,
		[TaxInc] [money] NOT NULL ,
		[TaxDec] [money] NOT NULL 
	) ON [PRIMARY]
	
	IF (@iTaxDateStart <= @iTaxDateEnd)
	BEGIN
		DECLARE	@iTaxDate		DATETIME
		SET		@iTaxDate		= @iTaxDateStart

		WHILE(@iTaxDate <= @iTaxDateEnd)
		BEGIN
			DECLARE	@dtLogDateStart	DATETIME
			DECLARE	@dtLogDateEnd	DATETIME
			DECLARE	@iTaxInc		MONEY
			DECLARE	@iTaxDec		MONEY
			DECLARE	@iYEAR		INT
			DECLARE	@iMONTH		INT
			DECLARE	@iDAY			INT
			SELECT	@iYEAR		= DATEPART(YY, @iTaxDate)
			SELECT	@iMONTH		= DATEPART(MM, @iTaxDate)
			SELECT	@iDAY			= DATEPART(DD, @iTaxDate)
			SET		@dtLogDateStart	= CAST(@iYEAR AS VARCHAR(4)) + '-' + CAST(@iMONTH AS VARCHAR(2))  + '-' + CAST(@iDAY AS VARCHAR(4)) + ' 00:00:00'
			SET		@dtLogDateEnd	= CAST(@iYEAR AS VARCHAR(4)) + '-' + CAST(@iMONTH AS VARCHAR(2))  + '-' + CAST(@iDAY AS VARCHAR(4)) + ' 23:59:59'
					
			SELECT @iTaxInc = SUM(MONEY_CHANGE) FROM MuCastle_MONEY_STATISTICS  WITH (READUNCOMMITTED) 
			WHERE MAP_SVR_GROUP = 0 and LOG_DATE BETWEEN @dtLogDateStart AND @dtLogDateEnd and MONEY_CHANGE >= 0
			
			SELECT @iTaxDec = SUM(MONEY_CHANGE) FROM MuCastle_MONEY_STATISTICS  WITH (READUNCOMMITTED) 
			WHERE MAP_SVR_GROUP = 0 and LOG_DATE BETWEEN @dtLogDateStart AND @dtLogDateEnd and MONEY_CHANGE < 0

			IF @iTaxInc IS NULL
				SET @iTaxInc = 0
			IF @iTaxDec IS NULL
				SET @iTaxDec = 0
						
			INSERT INTO #T_REG_TAXSTT VALUES (@dtLogDateStart, @iTaxInc, @iTaxDec)

			SET @iTaxDate				= DATEADD(DD, 1, @iTaxDate)
		END
	END
	
	SELECT * FROM #T_REG_TAXSTT

	DROP TABLE #T_REG_TAXSTT

	IF(@@Error <> 0 )
		ROLLBACK TRANSACTION
	ELSE	
		COMMIT TRANSACTION

	SET NOCOUNT OFF	
END


















GO
/****** Object:  StoredProcedure [dbo].[WZ_CS_GetCastleNpcInfo]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS OFF
GO
SET QUOTED_IDENTIFIER OFF
GO
CREATE Procedure [dbo].[WZ_CS_GetCastleNpcInfo]

	@iMapSvrGroup		SMALLINT
AS
BEGIN
	BEGIN TRANSACTION
	
	SET NOCOUNT ON	

	SELECT * FROM MuCastle_NPC WITH (READUNCOMMITTED)
	WHERE MAP_SVR_GROUP = @iMapSvrGroup

	IF(@@Error <> 0 )
		ROLLBACK TRANSACTION
	ELSE	
		COMMIT TRANSACTION

	SET NOCOUNT OFF	
END


















GO
/****** Object:  StoredProcedure [dbo].[WZ_CS_GetCastleTaxInfo]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS OFF
GO
SET QUOTED_IDENTIFIER OFF
GO
CREATE Procedure [dbo].[WZ_CS_GetCastleTaxInfo]

	@iMapSvrGroup		SMALLINT
AS
BEGIN
	BEGIN TRANSACTION
	
	SET NOCOUNT ON	

	SELECT MONEY, TAX_RATE_CHAOS, TAX_RATE_STORE, TAX_HUNT_ZONE FROM MuCastle_DATA  WITH (READUNCOMMITTED) 
	WHERE MAP_SVR_GROUP = @iMapSvrGroup

	IF(@@Error <> 0 )
		ROLLBACK TRANSACTION
	ELSE	
		COMMIT TRANSACTION

	SET NOCOUNT OFF	
END


















GO
/****** Object:  StoredProcedure [dbo].[WZ_CS_GetCastleTotalInfo]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS OFF
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE Procedure [dbo].[WZ_CS_GetCastleTotalInfo]

	@iMapSvrGroup		SMALLINT,
	@iCastleEventCycle	INT
AS
BEGIN
	DECLARE	@iCastleSiegeTerm			INT
	SET		@iCastleSiegeTerm			= @iCastleEventCycle
	DECLARE	@iFirstCreate				INT
	SET		@iFirstCreate				= 0
	
	BEGIN TRANSACTION
	
	SET NOCOUNT ON	

	IF NOT EXISTS ( SELECT MAP_SVR_GROUP FROM MuCastle_DATA  WITH (READUNCOMMITTED) 
				WHERE MAP_SVR_GROUP = @iMapSvrGroup)
	BEGIN
		DECLARE	@dtStartDate			datetime
		DECLARE	@dtEndDate			datetime
		DECLARE	@dtStartDateString		varchar(32)
		DECLARE	@dtEndDateString		varchar(32)

		SET		@dtStartDate			= GetDate()
		SET		@dtEndDate			= DATEADD(dd, @iCastleSiegeTerm, GetDate())
		SET		@dtStartDateString		= CAST(DATEPART(YY, @dtStartDate) AS char(4)) + '-' + CAST(DATEPART(MM, @dtStartDate) AS char(2)) + '-' + CAST(DATEPART(DD, @dtStartDate) AS char(2)) + ' 00:00:00'
		SET		@dtEndDateString		= CAST(DATEPART(YY, @dtEndDate) AS char(4)) + '-' + CAST(DATEPART(MM, @dtEndDate) AS char(2)) + '-' + CAST(DATEPART(DD, @dtEndDate) AS char(2)) + ' 00:00:00'


		INSERT INTO MuCastle_DATA  VALUES (
			@iMapSvrGroup,
			@dtStartDateString,
			@dtEndDateString,
			0,
			0,
			0,
			'',
			0,
			0,
			0,
			0
		)

		SET @iFirstCreate				= 1
	END

	SELECT	 MAP_SVR_GROUP, 
			DATEPART(YY,SIEGE_START_DATE)	As SYEAR, 
			DATEPART(MM,SIEGE_START_DATE)	As SMONTH, 
			DATEPART(DD,SIEGE_START_DATE)	As SDAY, 
			DATEPART(YY,SIEGE_END_DATE)	As EYEAR, 
			DATEPART(MM,SIEGE_END_DATE)	As EMONTH, 
			DATEPART(DD,SIEGE_END_DATE)	As EDAY, 
			SIEGE_GUILDLIST_SETTED, 
			SIEGE_ENDED, 
			CASTLE_OCCUPY, 
			OWNER_GUILD, 
			MONEY, 
			TAX_RATE_CHAOS,
			TAX_RATE_STORE,
			TAX_HUNT_ZONE,
			@iFirstCreate As FIRST_CREATE
	FROM MuCastle_DATA  WITH (READUNCOMMITTED)
	WHERE MAP_SVR_GROUP = @iMapSvrGroup

	IF(@@Error <> 0 )
		ROLLBACK TRANSACTION
	ELSE	
		COMMIT TRANSACTION

	SET NOCOUNT OFF	
END


















GO
/****** Object:  StoredProcedure [dbo].[WZ_CS_GetCsGuildUnionInfo]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS OFF
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE Procedure [dbo].[WZ_CS_GetCsGuildUnionInfo]

	@szGuildName		VARCHAR(8)
AS
BEGIN
	BEGIN TRANSACTION
	
	SET NOCOUNT ON

	DECLARE	@iG_Union	INT
	SET		@iG_Union	= -1
	
	IF EXISTS ( SELECT G_Name FROM Guild  WITH (READUNCOMMITTED) 
				WHERE G_Name = @szGuildName)
	BEGIN
		SELECT @iG_Union = G_Union
		FROM Guild WITH (READUNCOMMITTED) 
		WHERE G_Name = @szGuildName
	END

	IF (@iG_Union = 0)
	BEGIN
		SELECT @szGuildName As GUILD_NAME
	END
	ELSE
	BEGIN
		SELECT G_Name As GUILD_NAME
		FROM Guild WITH (READUNCOMMITTED) 
		WHERE G_Union = @iG_Union
	END
	
	IF(@@Error <> 0 )
		ROLLBACK TRANSACTION
	ELSE	
		COMMIT TRANSACTION

	SET NOCOUNT OFF	
END


















GO
/****** Object:  StoredProcedure [dbo].[WZ_CS_GetGuildMarkRegInfo]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS OFF
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE Procedure [dbo].[WZ_CS_GetGuildMarkRegInfo]

	@iMapSvrGroup		SMALLINT,
	@szGuildName		VARCHAR(8)
AS
BEGIN
	BEGIN TRANSACTION
	
	SET NOCOUNT ON	

	SELECT * FROM MuCastle_REG_SIEGE WITH (READUNCOMMITTED)
	WHERE MAP_SVR_GROUP = @iMapSvrGroup and REG_SIEGE_GUILD = @szGuildName
	ORDER BY SEQ_NUM ASC

	IF(@@Error <> 0 )
		ROLLBACK TRANSACTION
	ELSE	
		COMMIT TRANSACTION

	SET NOCOUNT OFF	
END


















GO
/****** Object:  StoredProcedure [dbo].[WZ_CS_GetOwnerGuildMaster]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS OFF
GO
SET QUOTED_IDENTIFIER OFF
GO
CREATE Procedure [dbo].[WZ_CS_GetOwnerGuildMaster]

	@iMapSvrGroup		SMALLINT
AS
BEGIN
	BEGIN TRANSACTION
	
	SET NOCOUNT ON	

	IF EXISTS ( SELECT MAP_SVR_GROUP FROM MuCastle_DATA  WITH (READUNCOMMITTED) 
				WHERE MAP_SVR_GROUP = @iMapSvrGroup)
	BEGIN
		DECLARE	@iIsCastleOccupied	TINYINT
		DECLARE	@szGuildName		VARCHAR(8)

		SELECT @iIsCastleOccupied = CASTLE_OCCUPY, @szGuildName = OWNER_GUILD FROM MuCastle_DATA WHERE MAP_SVR_GROUP = @iMapSvrGroup

		IF (@iIsCastleOccupied = 1)
		BEGIN
			IF (@szGuildName <> '')			
			BEGIN
				IF EXISTS ( SELECT G_Master FROM Guild  WITH (READUNCOMMITTED)
							WHERE G_Name = @szGuildName)
				BEGIN
					SELECT 1 As QueryResult, @szGuildName As OwnerGuild, G_Master As OwnerGuildMaster FROM Guild  WITH (READUNCOMMITTED) WHERE G_Name = @szGuildName
				END
				ELSE
				BEGIN
					SELECT 4 As QueryResult, '' As OwnerGuild, '' As OwnerGuildMaster
				END
			END
			ELSE
			BEGIN
				SELECT 3 As QueryResult, '' As OwnerGuild, '' As OwnerGuildMaster
			END
		END
		ELSE
		BEGIN
			SELECT 2 As QueryResult, '' As OwnerGuild, '' As OwnerGuildMaster
		END
	END
	ELSE
	BEGIN
		SELECT 0 As QueryResult, '' As OwnerGuild, '' As OwnerGuildMaster
	END


	IF(@@Error <> 0 )
		ROLLBACK TRANSACTION
	ELSE	
		COMMIT TRANSACTION

	SET NOCOUNT OFF	
END


















GO
/****** Object:  StoredProcedure [dbo].[WZ_CS_GetSiegeGuildInfo]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS OFF
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE Procedure [dbo].[WZ_CS_GetSiegeGuildInfo]

	@iMapSvrGroup		SMALLINT
AS
BEGIN
	BEGIN TRANSACTION
	
	SET NOCOUNT ON

	SELECT * 
	FROM MuCastle_SIEGE_GUILDLIST  WITH (READUNCOMMITTED) 
	WHERE MAP_SVR_GROUP = @iMapSvrGroup

	IF(@@Error <> 0 )
		ROLLBACK TRANSACTION
	ELSE	
		COMMIT TRANSACTION

	SET NOCOUNT OFF	
END


















GO
/****** Object:  StoredProcedure [dbo].[WZ_CS_ModifyCastleOwnerInfo]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS OFF
GO
SET QUOTED_IDENTIFIER OFF
GO
CREATE Procedure [dbo].[WZ_CS_ModifyCastleOwnerInfo]

	@iMapSvrGroup		SMALLINT,
	@iCastleOccupied	INT,
	@szOwnGuildName	VARCHAR(8)
AS
BEGIN
	BEGIN TRANSACTION
	
	SET NOCOUNT ON

	IF EXISTS ( SELECT MAP_SVR_GROUP FROM MuCastle_DATA  WITH (READUNCOMMITTED) 
				WHERE MAP_SVR_GROUP = @iMapSvrGroup)
	BEGIN
		UPDATE MuCastle_DATA 
		SET CASTLE_OCCUPY = @iCastleOccupied, OWNER_GUILD = @szOwnGuildName
		WHERE MAP_SVR_GROUP = @iMapSvrGroup

		SELECT 1 As QueryResult
	END
	ELSE
	BEGIN
		SELECT 0 As QueryResult
	END

	IF(@@Error <> 0 )
		ROLLBACK TRANSACTION
	ELSE	
		COMMIT TRANSACTION

	SET NOCOUNT OFF	
END


















GO
/****** Object:  StoredProcedure [dbo].[WZ_CS_ModifyCastleSchedule]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS OFF
GO
SET QUOTED_IDENTIFIER OFF
GO
CREATE Procedure [dbo].[WZ_CS_ModifyCastleSchedule]

	@iMapSvrGroup		SMALLINT,
	@dtStartDate		DATETIME,
	@dtEndDate		DATETIME
AS
BEGIN
	BEGIN TRANSACTION
	
	SET NOCOUNT ON

	IF EXISTS ( SELECT MAP_SVR_GROUP FROM MuCastle_DATA  WITH (READUNCOMMITTED) 
				WHERE MAP_SVR_GROUP = @iMapSvrGroup)
	BEGIN
		UPDATE MuCastle_DATA 
		SET SIEGE_START_DATE = @dtStartDate, SIEGE_END_DATE = @dtEndDate
		WHERE MAP_SVR_GROUP = @iMapSvrGroup

		SELECT 1 As QueryResult
	END
	ELSE
	BEGIN
		SELECT 0 As QueryResult
	END

	IF(@@Error <> 0 )
		ROLLBACK TRANSACTION
	ELSE	
		COMMIT TRANSACTION

	SET NOCOUNT OFF	
END


















GO
/****** Object:  StoredProcedure [dbo].[WZ_CS_ModifyGuildGiveUp]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS OFF
GO
SET QUOTED_IDENTIFIER OFF
GO
CREATE Procedure [dbo].[WZ_CS_ModifyGuildGiveUp]

	@iMapSvrGroup		SMALLINT,
	@szGuildName		VARCHAR(8),
	@iIsGiveUp		INT
AS
BEGIN
	BEGIN TRANSACTION
	
	SET NOCOUNT ON

	IF EXISTS ( SELECT MAP_SVR_GROUP FROM MuCastle_REG_SIEGE  WITH (READUNCOMMITTED) 
				WHERE MAP_SVR_GROUP = @iMapSvrGroup and REG_SIEGE_GUILD = @szGuildName)
	BEGIN
		DECLARE	@iMarkCount	INT
		SELECT @iMarkCount = REG_MARKS FROM MuCastle_REG_SIEGE  WITH (READUNCOMMITTED) 
				WHERE MAP_SVR_GROUP = @iMapSvrGroup and REG_SIEGE_GUILD = @szGuildName

		UPDATE MuCastle_REG_SIEGE 
		SET IS_GIVEUP = @iIsGiveUp, REG_MARKS = 0
		WHERE MAP_SVR_GROUP = @iMapSvrGroup and REG_SIEGE_GUILD = @szGuildName

		SELECT 1 As QueryResult, @iMarkCount As DEL_MARKS
	END
	ELSE
	BEGIN
		SELECT 2 As QueryResult, 0 As DEL_MARKS
	END

	IF(@@Error <> 0 )
		ROLLBACK TRANSACTION
	ELSE	
		COMMIT TRANSACTION

	SET NOCOUNT OFF	
END


















GO
/****** Object:  StoredProcedure [dbo].[WZ_CS_ModifyGuildMarkRegCount]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS OFF
GO
SET QUOTED_IDENTIFIER OFF
GO
CREATE Procedure [dbo].[WZ_CS_ModifyGuildMarkRegCount]

	@iMapSvrGroup		SMALLINT,
	@szGuildName		VARCHAR(8),
	@iMarkCount		INT
AS
BEGIN
	BEGIN TRANSACTION
	
	SET NOCOUNT ON

	IF EXISTS ( SELECT MAP_SVR_GROUP FROM MuCastle_REG_SIEGE  WITH (READUNCOMMITTED) 
				WHERE MAP_SVR_GROUP = @iMapSvrGroup and REG_SIEGE_GUILD = @szGuildName)
	BEGIN
		UPDATE MuCastle_REG_SIEGE 
		SET REG_MARKS = @iMarkCount
		WHERE MAP_SVR_GROUP = @iMapSvrGroup and REG_SIEGE_GUILD = @szGuildName

		SELECT 1 As QueryResult
	END
	ELSE
	BEGIN
		SELECT 0 As QueryResult
	END

	IF(@@Error <> 0 )
		ROLLBACK TRANSACTION
	ELSE	
		COMMIT TRANSACTION

	SET NOCOUNT OFF	
END


















GO
/****** Object:  StoredProcedure [dbo].[WZ_CS_ModifyGuildMarkReset]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS OFF
GO
SET QUOTED_IDENTIFIER OFF
GO
CREATE Procedure [dbo].[WZ_CS_ModifyGuildMarkReset]

	@iMapSvrGroup		SMALLINT,
	@szGuildName		VARCHAR(8)
AS
BEGIN
	BEGIN TRANSACTION

	DECLARE		@iMarkCount	INT
	DECLARE		@bIsGiveUp	INT

	SET NOCOUNT ON

	IF EXISTS ( SELECT MAP_SVR_GROUP FROM MuCastle_REG_SIEGE  WITH (READUNCOMMITTED) 
				WHERE MAP_SVR_GROUP = @iMapSvrGroup and REG_SIEGE_GUILD = @szGuildName)
	BEGIN
		SELECT @iMarkCount = REG_MARKS, @bIsGiveUp = IS_GIVEUP
		FROM MuCastle_REG_SIEGE
		WHERE MAP_SVR_GROUP = @iMapSvrGroup and REG_SIEGE_GUILD = @szGuildName

		IF (@iMarkCount > 0)
		BEGIN
			IF (@bIsGiveUp = 0)
			BEGIN
				UPDATE MuCastle_REG_SIEGE 
				SET REG_MARKS = 0
				WHERE MAP_SVR_GROUP = @iMapSvrGroup and REG_SIEGE_GUILD = @szGuildName
		
				SELECT 1 As QueryResult, @iMarkCount As DEL_MARKS
			END
			ELSE
			BEGIN
				SELECT 2 As QueryResult, 0 As DEL_MARKS
			END
		END
		ELSE
		BEGIN
			SELECT 1 As QueryResult, 0 As DEL_MARKS
		END
	END
	ELSE
	BEGIN
		SELECT 0 As QueryResult, 0 As DEL_MARKS
	END

	IF(@@Error <> 0 )
		ROLLBACK TRANSACTION
	ELSE	
		COMMIT TRANSACTION

	SET NOCOUNT OFF	
END


















GO
/****** Object:  StoredProcedure [dbo].[WZ_CS_ModifyMoney]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS OFF
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE Procedure [dbo].[WZ_CS_ModifyMoney]

	@iMapSvrGroup		SMALLINT,
	@iMoneyChange	MONEY	
AS
BEGIN
	BEGIN TRANSACTION
	
	SET NOCOUNT ON

	IF EXISTS ( SELECT MAP_SVR_GROUP FROM MuCastle_DATA  WITH (READUNCOMMITTED) 
				WHERE MAP_SVR_GROUP = @iMapSvrGroup)
	BEGIN
		DECLARE	@iTotMoney	MONEY
		SELECT @iTotMoney = MONEY FROM MuCastle_DATA
		WHERE MAP_SVR_GROUP = @iMapSvrGroup

		IF (@iTotMoney + @iMoneyChange < 0)
		BEGIN
			SELECT 2 As QueryResult, MONEY
			FROM MuCastle_DATA
			WHERE MAP_SVR_GROUP = @iMapSvrGroup		
		END
		ELSE
		BEGIN
			UPDATE MuCastle_DATA 
			SET MONEY = @iTotMoney + @iMoneyChange
			WHERE MAP_SVR_GROUP = @iMapSvrGroup
	
			SELECT 1 As QueryResult, MONEY
			FROM MuCastle_DATA
			WHERE MAP_SVR_GROUP = @iMapSvrGroup		
		END

		INSERT MuCastle_MONEY_STATISTICS VALUES (@iMapSvrGroup, GetDate(), @iMoneyChange)
	END
	ELSE
	BEGIN
		SELECT 0 As QueryResult, 0 As MONEY
	END

	IF(@@Error <> 0 )
		ROLLBACK TRANSACTION
	ELSE	
		COMMIT TRANSACTION

	SET NOCOUNT OFF	
END


















GO
/****** Object:  StoredProcedure [dbo].[WZ_CS_ModifySiegeEnd]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS OFF
GO
SET QUOTED_IDENTIFIER OFF
GO
CREATE Procedure [dbo].[WZ_CS_ModifySiegeEnd]

	@iMapSvrGroup		SMALLINT,
	@iSiegeEnded		INT
AS
BEGIN
	BEGIN TRANSACTION
	
	SET NOCOUNT ON

	IF EXISTS ( SELECT MAP_SVR_GROUP FROM MuCastle_DATA  WITH (READUNCOMMITTED) 
				WHERE MAP_SVR_GROUP = @iMapSvrGroup)
	BEGIN
		UPDATE MuCastle_DATA 
		SET SIEGE_ENDED = @iSiegeEnded
		WHERE MAP_SVR_GROUP = @iMapSvrGroup

		SELECT 1 As QueryResult
	END
	ELSE
	BEGIN
		SELECT 0 As QueryResult
	END

	IF(@@Error <> 0 )
		ROLLBACK TRANSACTION
	ELSE	
		COMMIT TRANSACTION

	SET NOCOUNT OFF	
END


















GO
/****** Object:  StoredProcedure [dbo].[WZ_CS_ModifyTaxRate]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS OFF
GO
SET QUOTED_IDENTIFIER OFF
GO
CREATE Procedure [dbo].[WZ_CS_ModifyTaxRate]

	@iMapSvrGroup		SMALLINT,
	@iTaxKind		INT,
	@iTaxRate		INT
AS
BEGIN
	BEGIN TRANSACTION
	
	SET NOCOUNT ON

	IF (@iTaxKind = 1)
	BEGIN
		IF EXISTS ( SELECT MAP_SVR_GROUP FROM MuCastle_DATA  WITH (READUNCOMMITTED) 
					WHERE MAP_SVR_GROUP = @iMapSvrGroup)
		BEGIN
			UPDATE MuCastle_DATA 
			SET TAX_RATE_CHAOS = @iTaxRate
			WHERE MAP_SVR_GROUP = @iMapSvrGroup
	
			SELECT @iTaxKind As TaxKind, 1 As QueryResult, TAX_RATE_CHAOS As TaxRate
			FROM MuCastle_DATA
			WHERE MAP_SVR_GROUP = @iMapSvrGroup
		END
		ELSE
		BEGIN
			SELECT @iTaxKind As TaxKind, 0 As QueryResult, 0 As TaxRate
		END
	END
	ELSE IF (@iTaxKind = 2)
	BEGIN
		IF EXISTS ( SELECT MAP_SVR_GROUP FROM MuCastle_DATA  WITH (READUNCOMMITTED) 
					WHERE MAP_SVR_GROUP = @iMapSvrGroup)
		BEGIN
			UPDATE MuCastle_DATA 
			SET TAX_RATE_STORE = @iTaxRate
			WHERE MAP_SVR_GROUP = @iMapSvrGroup
	
			SELECT @iTaxKind As TaxKind, 1 As QueryResult, TAX_RATE_STORE As TaxRate
			FROM MuCastle_DATA
			WHERE MAP_SVR_GROUP = @iMapSvrGroup
		END
		ELSE
		BEGIN
			SELECT @iTaxKind As TaxKind, 0 As QueryResult, 0 As TaxRate
		END
	END
	ELSE IF (@iTaxKind = 3)	
	BEGIN
		IF EXISTS ( SELECT MAP_SVR_GROUP FROM MuCastle_DATA  WITH (READUNCOMMITTED) 
					WHERE MAP_SVR_GROUP = @iMapSvrGroup)
		BEGIN
			UPDATE MuCastle_DATA 
			SET TAX_HUNT_ZONE = @iTaxRate
			WHERE MAP_SVR_GROUP = @iMapSvrGroup
	
			SELECT @iTaxKind As TaxKind, 1 As QueryResult, TAX_HUNT_ZONE As TaxRate
			FROM MuCastle_DATA
			WHERE MAP_SVR_GROUP = @iMapSvrGroup
		END
		ELSE
		BEGIN
			SELECT @iTaxKind As TaxKind, 0 As QueryResult, 0 As TaxRate
		END
	END
	ELSE
	BEGIN
		SELECT @iTaxKind As TaxKind, 0 As QueryResult, 0 As TaxRate
	END

	IF(@@Error <> 0 )
		ROLLBACK TRANSACTION
	ELSE	
		COMMIT TRANSACTION

	SET NOCOUNT OFF	
END


















GO
/****** Object:  StoredProcedure [dbo].[WZ_CS_ReqNpcBuy]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS OFF
GO
SET QUOTED_IDENTIFIER OFF
GO
CREATE Procedure [dbo].[WZ_CS_ReqNpcBuy]

	@iMapSvrGroup		SMALLINT,
	@iNpcNumber		INT,
	@iNpcIndex		INT,
	@iNpcDfLevel		INT,
	@iNpcRgLevel		INT,
	@iNpcMaxHp		INT,
	@iNpcHp		INT,
	@btNpcX		TINYINT,
	@btNpcY		TINYINT,
	@btNpcDIR		TINYINT
AS
BEGIN
	BEGIN TRANSACTION
	
	SET NOCOUNT ON

	IF EXISTS ( SELECT MAP_SVR_GROUP FROM MuCastle_NPC  WITH (READUNCOMMITTED) 
				WHERE MAP_SVR_GROUP = @iMapSvrGroup and NPC_NUMBER = @iNpcNumber and NPC_INDEX = @iNpcIndex)
	BEGIN
		SELECT 4 As QueryResult
	END
	ELSE
	BEGIN
		INSERT INTO MuCastle_NPC VALUES (
			@iMapSvrGroup	,
			@iNpcNumber,
			@iNpcIndex,
			@iNpcDfLevel,
			@iNpcRgLevel,
			@iNpcMaxHp,
			@iNpcHp,
			@btNpcX,
			@btNpcY,
			@btNpcDIR,
			GetDate()
		)
		
		SELECT 1 As QueryResult
	END

	IF(@@Error <> 0 )
		ROLLBACK TRANSACTION
	ELSE	
		COMMIT TRANSACTION

	SET NOCOUNT OFF	
END


















GO
/****** Object:  StoredProcedure [dbo].[WZ_CS_ReqNpcRemove]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS OFF
GO
SET QUOTED_IDENTIFIER OFF
GO
CREATE Procedure [dbo].[WZ_CS_ReqNpcRemove]

	@iMapSvrGroup		SMALLINT,
	@iNpcNumber		INT,
	@iNpcIndex		INT
AS
BEGIN
	BEGIN TRANSACTION
	
	SET NOCOUNT ON

	IF EXISTS ( SELECT MAP_SVR_GROUP FROM MuCastle_NPC  WITH (READUNCOMMITTED) 
				WHERE MAP_SVR_GROUP = @iMapSvrGroup and NPC_NUMBER = @iNpcNumber and NPC_INDEX = @iNpcIndex)
	BEGIN
		DELETE MuCastle_NPC
		WHERE MAP_SVR_GROUP = @iMapSvrGroup and NPC_NUMBER = @iNpcNumber and NPC_INDEX = @iNpcIndex

		SELECT 1 As QueryResult
	END
	ELSE
	BEGIN
		SELECT 2 As QueryResult
	END

	IF(@@Error <> 0 )
		ROLLBACK TRANSACTION
	ELSE	
		COMMIT TRANSACTION

	SET NOCOUNT OFF	
END


















GO
/****** Object:  StoredProcedure [dbo].[WZ_CS_ReqNpcRepair]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS OFF
GO
SET QUOTED_IDENTIFIER OFF
GO
CREATE Procedure [dbo].[WZ_CS_ReqNpcRepair]

	@iMapSvrGroup		SMALLINT,
	@iNpcNumber		INT,
	@iNpcIndex		INT
AS
BEGIN
	BEGIN TRANSACTION
	
	SET NOCOUNT ON

	IF EXISTS ( SELECT MAP_SVR_GROUP FROM MuCastle_NPC  WITH (READUNCOMMITTED) 
				WHERE MAP_SVR_GROUP = @iMapSvrGroup and NPC_NUMBER = @iNpcNumber and NPC_INDEX = @iNpcIndex)
	BEGIN
		UPDATE MuCastle_NPC 
		SET  NPC_HP = NPC_MAXHP
		WHERE MAP_SVR_GROUP = @iMapSvrGroup and NPC_NUMBER = @iNpcNumber and NPC_INDEX = @iNpcIndex

		SELECT 1 As QueryResult, NPC_HP, NPC_MAXHP
		FROM MuCastle_NPC  WITH (READUNCOMMITTED) 
		WHERE MAP_SVR_GROUP = @iMapSvrGroup and NPC_NUMBER = @iNpcNumber and NPC_INDEX = @iNpcIndex
	END
	ELSE
	BEGIN
		SELECT 2 As QueryResult, 0 As NPC_HP, 0 As NPC_MAXHP
	END

	IF(@@Error <> 0 )
		ROLLBACK TRANSACTION
	ELSE	
		COMMIT TRANSACTION

	SET NOCOUNT OFF	
END


















GO
/****** Object:  StoredProcedure [dbo].[WZ_CS_ReqNpcUpdate]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS OFF
GO
SET QUOTED_IDENTIFIER OFF
GO
CREATE Procedure [dbo].[WZ_CS_ReqNpcUpdate]

	@iMapSvrGroup		SMALLINT,
	@iNpcNumber		INT,
	@iNpcIndex		INT,
	@iNpcDfLevel		INT,
	@iNpcRgLevel		INT,
	@iNpcMaxHp		INT,
	@iNpcHp		INT,
	@btNpcX		TINYINT,
	@btNpcY		TINYINT,
	@btNpcDIR		TINYINT
As
Begin
	BEGIN TRANSACTION
	
	SET NOCOUNT ON

	IF EXISTS ( SELECT MAP_SVR_GROUP FROM MuCastle_NPC  WITH (READUNCOMMITTED) 
				WHERE MAP_SVR_GROUP = @iMapSvrGroup and NPC_NUMBER = @iNpcNumber and NPC_INDEX = @iNpcIndex)
	BEGIN
		
		UPDATE MuCastle_NPC
		SET	NPC_NUMBER		= @iNpcNumber, 
			NPC_INDEX		= @iNpcIndex, 
			NPC_DF_LEVEL	= @iNpcDfLevel, 
			NPC_RG_LEVEL	= @iNpcRgLevel, 
			NPC_MAXHP		= @iNpcMaxHp, 
			NPC_HP		= @iNpcHp,
			NPC_X			= @btNpcX,
			NPC_Y			= @btNpcY, 
			NPC_DIR		= @btNpcDIR
		WHERE MAP_SVR_GROUP = @iMapSvrGroup and NPC_NUMBER = @iNpcNumber and NPC_INDEX = @iNpcIndex
	END
	ELSE
	BEGIN

		INSERT INTO MuCastle_NPC VALUES (
			@iMapSvrGroup	,
			@iNpcNumber,
			@iNpcIndex,
			@iNpcDfLevel,
			@iNpcRgLevel,
			@iNpcMaxHp,
			@iNpcHp,
			@btNpcX,
			@btNpcY,
			@btNpcDIR,
			GetDate()
		)
	END

	IF(@@Error <> 0 )
		ROLLBACK TRANSACTION
	ELSE	
		COMMIT TRANSACTION

	SET NOCOUNT OFF	
END


















GO
/****** Object:  StoredProcedure [dbo].[WZ_CS_ReqNpcUpgrade]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS OFF
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE Procedure [dbo].[WZ_CS_ReqNpcUpgrade]

	@iMapSvrGroup		SMALLINT,
	@iNpcNumber		INT,
	@iNpcIndex		INT,
	@iNpcUpType		INT,
	@iNpcUpValue		INT
AS
BEGIN
	BEGIN TRANSACTION
	
	SET NOCOUNT ON

	IF EXISTS ( SELECT MAP_SVR_GROUP FROM MuCastle_NPC  WITH (READUNCOMMITTED) 
				WHERE MAP_SVR_GROUP = @iMapSvrGroup and NPC_NUMBER = @iNpcNumber and NPC_INDEX = @iNpcIndex)
	BEGIN
		IF (@iNpcUpType = 1)
		BEGIN
			UPDATE MuCastle_NPC 
			SET NPC_DF_LEVEL = @iNpcUpValue
			WHERE MAP_SVR_GROUP = @iMapSvrGroup and NPC_NUMBER = @iNpcNumber and NPC_INDEX = @iNpcIndex
	
			SELECT 1 As QueryResult
		END
		ELSE IF (@iNpcUpType = 2)
		BEGIN
			UPDATE MuCastle_NPC 
			SET NPC_RG_LEVEL = @iNpcUpValue
			WHERE MAP_SVR_GROUP = @iMapSvrGroup and NPC_NUMBER = @iNpcNumber and NPC_INDEX = @iNpcIndex
	
			SELECT 1 As QueryResult
		END
		ELSE IF (@iNpcUpType = 3)
		BEGIN
			UPDATE MuCastle_NPC 
			SET NPC_MAXHP = @iNpcUpValue, NPC_HP = @iNpcUpValue
			WHERE MAP_SVR_GROUP = @iMapSvrGroup and NPC_NUMBER = @iNpcNumber and NPC_INDEX = @iNpcIndex
	
			SELECT 1 As QueryResult
		END
		ELSE
		BEGIN
			SELECT 2 As QueryResult
		END
	END
	ELSE
	BEGIN
		SELECT 0 As QueryResult
	END

	IF(@@Error <> 0 )
		ROLLBACK TRANSACTION
	ELSE	
		COMMIT TRANSACTION

	SET NOCOUNT OFF	
END


















GO
/****** Object:  StoredProcedure [dbo].[WZ_CS_ReqRegAttackGuild]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS OFF
GO
SET QUOTED_IDENTIFIER OFF
GO
CREATE Procedure [dbo].[WZ_CS_ReqRegAttackGuild]

	@iMapSvrGroup		SMALLINT,
	@szGuildName		VARCHAR(8)
AS
BEGIN
	BEGIN TRANSACTION
	
	SET NOCOUNT ON

	DECLARE	@iMaxRegGuildCount	INT
	DECLARE	@iCurRegGuildCount	INT
	SET 		@iMaxRegGuildCount	= 100

	SELECT @iCurRegGuildCount = COUNT(*) FROM MuCastle_REG_SIEGE  WITH (READUNCOMMITTED)  WHERE MAP_SVR_GROUP = @iMapSvrGroup
	IF (@iCurRegGuildCount >= @iMaxRegGuildCount)
	BEGIN
			SELECT 6 As QueryResult
	END
	ELSE
	BEGIN
		IF EXISTS ( SELECT MAP_SVR_GROUP FROM MuCastle_REG_SIEGE  WITH (READUNCOMMITTED) 
					WHERE MAP_SVR_GROUP = @iMapSvrGroup and REG_SIEGE_GUILD = @szGuildName)
		BEGIN
			SELECT 2 As QueryResult
		END
		ELSE
		BEGIN
			DECLARE @szOwnGuildName		VARCHAR(8)
			SELECT @szOwnGuildName = OWNER_GUILD FROM MuCastle_DATA WHERE MAP_SVR_GROUP = @iMapSvrGroup
	
			IF (@szOwnGuildName = @szGuildName)
			BEGIN
				SELECT 3 As QueryResult
			END
			ELSE
			BEGIN
				IF NOT EXISTS ( SELECT G_Name FROM Guild  WITH (READUNCOMMITTED) WHERE G_Name = @szGuildName)
				BEGIN
					SELECT 4 As QueryResult
				END
				ELSE
				BEGIN
					DECLARE @szGuildMaster			VARCHAR(10)
					DECLARE @iGuildMasterLevel			INT
					DECLARE @iGuildMemberCount			INT
					
					SELECT @szGuildMaster = G_Master FROM Guild WHERE G_Name = @szGuildName
					SELECT @iGuildMasterLevel = cLevel FROM Character WHERE Name = @szGuildMaster
					SELECT @iGuildMemberCount = COUNT(*) FROM GuildMember WHERE G_Name = @szGuildName
					
					IF (@iGuildMasterLevel < 1)
					BEGIN
						SELECT 5 As QueryResult
					END
					ELSE
					BEGIN
						IF (@iGuildMemberCount < 0)
						BEGIN
							SELECT 8 As QueryResult
						END
						ELSE
						BEGIN
							DECLARE @iMAX_SEQNUM	INT
							DECLARE @iNXT_SEQNUM	INT
							SELECT @iMAX_SEQNUM = MAX(SEQ_NUM) FROM MuCastle_REG_SIEGE  WITH (READUNCOMMITTED)  WHERE MAP_SVR_GROUP = @iMapSvrGroup
							
							IF (@iMAX_SEQNUM IS NULL)
								SET @iNXT_SEQNUM	= 1
							ELSE
								SET @iNXT_SEQNUM	= @iMAX_SEQNUM + 1

							INSERT INTO MuCastle_REG_SIEGE 
							VALUES (@iMapSvrGroup, @szGuildName, 0, 0, @iNXT_SEQNUM)
					
							SELECT 1 As QueryResult
						END
					END
				END
			END
		END
	END

	
	IF(@@Error <> 0 )
		ROLLBACK TRANSACTION
	ELSE	
		COMMIT TRANSACTION

	SET NOCOUNT OFF	
END


















GO
/****** Object:  StoredProcedure [dbo].[WZ_CS_ReqRegGuildMark]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS OFF
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE Procedure [dbo].[WZ_CS_ReqRegGuildMark]

	@iMapSvrGroup		SMALLINT,
	@szGuildName		VARCHAR(8)
AS
BEGIN
	BEGIN TRANSACTION
	
	SET NOCOUNT ON

	IF EXISTS ( SELECT MAP_SVR_GROUP FROM MuCastle_REG_SIEGE  WITH (READUNCOMMITTED) 
				WHERE MAP_SVR_GROUP = @iMapSvrGroup and REG_SIEGE_GUILD = @szGuildName)
	BEGIN
		DECLARE	@bIS_GIVEUP	INT
		SELECT @bIS_GIVEUP = IS_GIVEUP FROM MuCastle_REG_SIEGE  WITH (READUNCOMMITTED) 
				WHERE MAP_SVR_GROUP = @iMapSvrGroup and REG_SIEGE_GUILD = @szGuildName

		IF (@bIS_GIVEUP = 0)
		BEGIN
			UPDATE MuCastle_REG_SIEGE 
			SET REG_MARKS = REG_MARKS + 1
			WHERE MAP_SVR_GROUP = @iMapSvrGroup and REG_SIEGE_GUILD = @szGuildName
	
			SELECT 1 As QueryResult, REG_MARKS
			FROM MuCastle_REG_SIEGE  WITH (READUNCOMMITTED)
			WHERE MAP_SVR_GROUP = @iMapSvrGroup and REG_SIEGE_GUILD = @szGuildName
		END
		ELSE
		BEGIN
			SELECT 0 As QueryResult, 0 As REG_MARKS
		END
	END
	ELSE
	BEGIN
		SELECT 0 As QueryResult, 0 As REG_MARKS
	END

	IF(@@Error <> 0 )
		ROLLBACK TRANSACTION
	ELSE	
		COMMIT TRANSACTION

	SET NOCOUNT OFF	
END


















GO
/****** Object:  StoredProcedure [dbo].[WZ_CS_ResetCastleSiege]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS OFF
GO
SET QUOTED_IDENTIFIER OFF
GO
CREATE Procedure [dbo].[WZ_CS_ResetCastleSiege]

	@iMapSvrGroup		SMALLINT
AS
BEGIN
	BEGIN TRANSACTION
	
	SET NOCOUNT ON

	IF EXISTS ( SELECT MAP_SVR_GROUP FROM MuCastle_DATA  WITH (READUNCOMMITTED) 
				WHERE MAP_SVR_GROUP = @iMapSvrGroup)
	BEGIN
		UPDATE MuCastle_DATA 
		SET 	SIEGE_GUILDLIST_SETTED = 0,
			SIEGE_ENDED = 0
		WHERE MAP_SVR_GROUP = @iMapSvrGroup

		DELETE MuCastle_SIEGE_GUILDLIST
		WHERE MAP_SVR_GROUP = @iMapSvrGroup

		SELECT 1 As QueryResult
	END
	ELSE
	BEGIN
		SELECT 0 As QueryResult
	END

	IF(@@Error <> 0 )
		ROLLBACK TRANSACTION
	ELSE	
		COMMIT TRANSACTION

	SET NOCOUNT OFF	
END


















GO
/****** Object:  StoredProcedure [dbo].[WZ_CS_ResetCastleTaxInfo]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS OFF
GO
SET QUOTED_IDENTIFIER OFF
GO
CREATE Procedure [dbo].[WZ_CS_ResetCastleTaxInfo]

	@iMapSvrGroup		SMALLINT
AS
BEGIN
	BEGIN TRANSACTION
	
	SET NOCOUNT ON

	IF EXISTS ( SELECT MAP_SVR_GROUP FROM MuCastle_DATA  WITH (READUNCOMMITTED) 
				WHERE MAP_SVR_GROUP = @iMapSvrGroup)
	BEGIN
		UPDATE MuCastle_DATA 
		SET MONEY = 0, TAX_RATE_CHAOS = 0, TAX_RATE_STORE = 0, TAX_HUNT_ZONE = 0
		WHERE MAP_SVR_GROUP = @iMapSvrGroup

		SELECT 1 As QueryResult
	END
	ELSE
	BEGIN
		SELECT 0 As QueryResult
	END

	IF(@@Error <> 0 )
		ROLLBACK TRANSACTION
	ELSE	
		COMMIT TRANSACTION

	SET NOCOUNT OFF	
END


















GO
/****** Object:  StoredProcedure [dbo].[WZ_CS_ResetRegSiegeInfo]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS OFF
GO
SET QUOTED_IDENTIFIER OFF
GO
CREATE Procedure [dbo].[WZ_CS_ResetRegSiegeInfo]

	@iMapSvrGroup		SMALLINT
AS
BEGIN
	BEGIN TRANSACTION
	
	SET NOCOUNT ON

	IF EXISTS ( SELECT MAP_SVR_GROUP FROM MuCastle_DATA  WITH (READUNCOMMITTED) 
				WHERE MAP_SVR_GROUP = @iMapSvrGroup)
	BEGIN
		DELETE MuCastle_REG_SIEGE
		WHERE MAP_SVR_GROUP = @iMapSvrGroup

		SELECT 1 As QueryResult
	END
	ELSE
	BEGIN
		SELECT 0 As QueryResult
	END

	IF(@@Error <> 0 )
		ROLLBACK TRANSACTION
	ELSE	
		COMMIT TRANSACTION

	SET NOCOUNT OFF	
END


















GO
/****** Object:  StoredProcedure [dbo].[WZ_CS_ResetSiegeGuildInfo]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS OFF
GO
SET QUOTED_IDENTIFIER OFF
GO
CREATE Procedure [dbo].[WZ_CS_ResetSiegeGuildInfo]

	@iMapSvrGroup		SMALLINT
AS
BEGIN
	BEGIN TRANSACTION
	
	SET NOCOUNT ON

	IF EXISTS ( SELECT MAP_SVR_GROUP FROM MuCastle_DATA  WITH (READUNCOMMITTED) 
				WHERE MAP_SVR_GROUP = @iMapSvrGroup)
	BEGIN
		DELETE MuCastle_SIEGE_GUILDLIST
		WHERE MAP_SVR_GROUP = @iMapSvrGroup

		SELECT 1 As QueryResult
	END
	ELSE
	BEGIN
		SELECT 0 As QueryResult
	END

	IF(@@Error <> 0 )
		ROLLBACK TRANSACTION
	ELSE	
		COMMIT TRANSACTION

	SET NOCOUNT OFF	
END


















GO
/****** Object:  StoredProcedure [dbo].[WZ_CS_SetSiegeGuildInfo]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS OFF
GO
SET QUOTED_IDENTIFIER ON
GO


CREATE PROCEDURE	[dbo].[WZ_CS_SetSiegeGuildInfo]
	@iMapSvrGroup		SMALLINT,
	@szGuildName		VARCHAR(8),
	@iGuildID		INT,
	@iGuildInvolved		INT,
	@iGuildScore		INT
As
Begin
	BEGIN TRANSACTION
	
	SET NOCOUNT ON

	INSERT INTO MuCastle_SIEGE_GUILDLIST
	VALUES (@iMapSvrGroup, @szGuildName, @iGuildID, @iGuildInvolved, @iGuildScore)
	
	IF(@@Error <> 0 )
		ROLLBACK TRANSACTION
	ELSE	
		COMMIT TRANSACTION

	SET NOCOUNT OFF	
End

















GO
/****** Object:  StoredProcedure [dbo].[WZ_CS_SetSiegeGuildOK]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS OFF
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE Procedure [dbo].[WZ_CS_SetSiegeGuildOK]

	@iMapSvrGroup		SMALLINT
AS
BEGIN
	BEGIN TRANSACTION
	
	SET NOCOUNT ON

	IF EXISTS ( SELECT MAP_SVR_GROUP FROM MuCastle_DATA  WITH (READUNCOMMITTED) 
				WHERE MAP_SVR_GROUP = @iMapSvrGroup)
	BEGIN
		UPDATE MuCastle_DATA
		SET SIEGE_GUILDLIST_SETTED = 1
		WHERE MAP_SVR_GROUP = @iMapSvrGroup

		SELECT 1 As QueryResult
	END
	ELSE
	BEGIN
		SELECT 0 As QueryResult
	END
	
	IF(@@Error <> 0 )
		ROLLBACK TRANSACTION
	ELSE	
		COMMIT TRANSACTION

	SET NOCOUNT OFF	
END


















GO
/****** Object:  StoredProcedure [dbo].[WZ_CW_InfoLoad]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER OFF
GO
CREATE Procedure [dbo].[WZ_CW_InfoLoad]

	@iMapSvrGroup		SMALLINT
AS
BEGIN
	BEGIN TRANSACTION
	
	SET NOCOUNT ON
	IF EXISTS ( SELECT MAP_SVR_GROUP FROM MuCrywolf_DATA  WITH (READUNCOMMITTED) 
				WHERE MAP_SVR_GROUP = @iMapSvrGroup)
	BEGIN
		SELECT CRYWOLF_OCCUFY, CRYWOLF_STATE  FROM MuCrywolf_DATA  WHERE MAP_SVR_GROUP = @iMapSvrGroup
	END
	ELSE
	BEGIN
		INSERT MuCrywolf_DATA VALUES ( @iMapSvrGroup, DEFAULT, DEFAULT, DEFAULT, DEFAULT )
		SELECT CRYWOLF_OCCUFY, CRYWOLF_STATE  FROM MuCrywolf_DATA WHERE  MAP_SVR_GROUP = @iMapSvrGroup
	END
	
	IF(@@Error <> 0 )
		ROLLBACK TRANSACTION
	ELSE	
		COMMIT TRANSACTION
	SET NOCOUNT OFF	
END


















GO
/****** Object:  StoredProcedure [dbo].[WZ_CW_InfoSave]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER OFF
GO
CREATE Procedure [dbo].[WZ_CW_InfoSave]

	@iMapSvrGroup		SMALLINT,
	@iCrywolfState		INT,
	@iOccupationState	INT
AS
BEGIN
	BEGIN TRANSACTION
	
	SET NOCOUNT ON
	IF EXISTS ( SELECT MAP_SVR_GROUP FROM MuCrywolf_DATA  WITH (READUNCOMMITTED) 
				WHERE MAP_SVR_GROUP = @iMapSvrGroup)
	BEGIN
		UPDATE MuCrywolf_DATA
		SET CRYWOLF_OCCUFY = @iOccupationState, CRYWOLF_STATE = @iCrywolfState
		WHERE MAP_SVR_GROUP = @iMapSvrGroup
	END
	ELSE
	BEGIN
		INSERT MuCrywolf_DATA VALUES ( @iMapSvrGroup, DEFAULT, DEFAULT, DEFAULT, DEFAULT )

		UPDATE MuCrywolf_DATA
		SET CRYWOLF_OCCUFY = @iOccupationState, CRYWOLF_STATE = @iCrywolfState
		WHERE MAP_SVR_GROUP = @iMapSvrGroup
	END
	
	IF(@@Error <> 0 )
		ROLLBACK TRANSACTION
	ELSE	
		COMMIT TRANSACTION
	SET NOCOUNT OFF	
END


















GO
/****** Object:  StoredProcedure [dbo].[WZ_DelMail]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE Procedure [dbo].[WZ_DelMail]

	@Name 	varchar(10),
	@MemoIndex 	int
AS
BEGIN
	DECLARE @ErrorCode int
	DECLARE @UserGuid  int

	SET XACT_ABORT ON
	SET NOCOUNT ON

	SET @ErrorCode = 0

	IF NOT EXISTS ( SELECT GUID FROM T_FriendMain WHERE Name=@Name)
	  BEGIN
		SET @ErrorCode = 2
		GOTO EndProc

	  END
	ELSE
	  BEGIN
		SELECT @UserGuid = GUID FROM T_FriendMain WHERE Name=@Name
	
		IF ( @@Error  <> 0   )
		BEGIN
			SET @ErrorCode = 3
			GOTO EndProc
		END
	
	  END

	IF NOT EXISTS ( select MemoIndex FROM T_FriendMail  WHERE  MemoIndex=@MemoIndex AND GUID=@UserGuid)
	  BEGIN
		SET @ErrorCode = 4
		GOTO EndProc
	  END

	BEGIN TRAN

	DELETE FROM T_FriendMail WHERE MemoIndex=@MemoIndex AND GUID=@UserGuid
	IF ( @@Error  <> 0 )
		SET @ErrorCode = 5
	ELSE
	  BEGIN
		UPDATE T_FriendMain SET MemoTotal=MemoTotal-1 WHERE GUID = @UserGuid
		IF ( @@Error  <> 0 )
		BEGIN
			SET @ErrorCode = 6
		END
	  END

	IF ( @ErrorCode  <> 0 )
	  BEGIN
		ROLLBACK TRAN
	  END
	ELSE
	  BEGIN
		COMMIT TRAN
		SET @ErrorCode = 1
	  END
	
EndProc:

	SET XACT_ABORT OFF
	SET NOCOUNT OFF
	SELECT @ErrorCode
END


















GO
/****** Object:  StoredProcedure [dbo].[WZ_DISCONNECT_MEMB]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER OFF
GO
CREATE Procedure [dbo].[WZ_DISCONNECT_MEMB]

	@memb___id 	varchar(10)
AS
BEGIN

SET NOCOUNT ON
	DECLARE @find_id		 varchar(10)	
	DECLARE @ConnectStat	 tinyint
	SET @ConnectStat = 0
	SET @find_id = 'NOT'
	SELECT @find_id = S.memb___id FROM MEMB_STAT S INNER JOIN MEMB_INFO I ON S.memb___id = I.memb___id 
	     WHERE I.memb___id = @memb___id

	IF( @find_id <> 'NOT' )
	BEGIN
		UPDATE MEMB_STAT set ConnectStat = @ConnectStat, DisConnectTM = getdate()
		 WHERE memb___id = @memb___id
	END
END


















GO
/****** Object:  StoredProcedure [dbo].[WZ_FriendAdd]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE Procedure [dbo].[WZ_FriendAdd]

	@Name		varchar(10),
	@FriendName 	varchar(10)

AS
BEGIN
	DECLARE @ErrorCode int
	DECLARE @UserGuid  int
	DECLARE @FriendGuid  int

	SET NOCOUNT ON
	SET XACT_ABORT ON


	SET @ErrorCode = 0

	IF NOT EXISTS ( SELECT GUID FROM T_FriendMain WHERE Name=@FriendName )
	  BEGIN
		SET @ErrorCode = 5
		GOTO EndProc
	  END
 	ELSE
	  BEGIN
		SELECT @FriendGuid = GUID FROM T_FriendMain WHERE Name=@FriendName

		IF ( @@Error  <> 0 )
		BEGIN
			SET @ErrorCode = 6
			GOTO EndProc
		END

	END

	IF NOT EXISTS ( SELECT GUID FROM T_FriendMain WHERE Name=@Name)
	  BEGIN
		SET @ErrorCode = 3
		GOTO EndProc

	  END
	ELSE
	  BEGIN
		SELECT @UserGuid = GUID FROM T_FriendMain WHERE Name=@Name
	
		IF ( @@Error  <> 0   )
		BEGIN
			SET @ErrorCode = 4
			GOTO EndProc
		END
	
	  END

	IF EXISTS (SELECT GUID FROM T_FriendList WHERE GUID = @UserGuid AND FriendGuid = @FriendGuid  )
	BEGIN
		SET @ErrorCode = 2
		GOTO EndProc
	END

	BEGIN TRAN

	INSERT INTO T_FriendList (GUID, FriendGuid, FriendName ) 
		VALUES ( @UserGuid, @FriendGuid, @FriendName)
	
	IF ( @@Error  <> 0 )
		SET @ErrorCode = @@Error
	ELSE
	  BEGIN
		DELETE FROM T_WaitFriend where GUID = @UserGuid AND FriendGuid = @FriendGuid
		IF ( @@Error  <> 0 )
		BEGIN
			SET @ErrorCode = @@Error
		END
	  END

	IF ( @ErrorCode  <> 0 )
	  BEGIN
		ROLLBACK TRAN
	  END
	ELSE
	  BEGIN
		COMMIT TRAN
		SET @ErrorCode = 1
	  END
	
EndProc:

	SET XACT_ABORT OFF
	SET NOCOUNT OFF
	SELECT @ErrorCode
END


















GO
/****** Object:  StoredProcedure [dbo].[WZ_FriendDel]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER OFF
GO
CREATE Procedure [dbo].[WZ_FriendDel]

	@Name		varchar(10),
	@FriendName 	varchar(10)
AS
BEGIN
	DECLARE @ErrorCode int
	DECLARE @UserGuid  int
	DECLARE @FriendGuid  int

	SET NOCOUNT ON

	SET @ErrorCode = 0

	IF NOT EXISTS ( SELECT GUID FROM T_FriendMain WHERE Name=@Name)
	  BEGIN
		SET @ErrorCode = 3
		GOTO EndProc

	  END
	ELSE
	  BEGIN
		SELECT @UserGuid = GUID FROM T_FriendMain WHERE Name=@Name
	
		IF ( @@Error  <> 0   )
		BEGIN
			SET @ErrorCode = 4
		END

	  END

	IF NOT EXISTS ( SELECT GUID FROM T_FriendMain WHERE Name=@FriendName )
	  BEGIN
		SET @ErrorCode = 5
		GOTO EndProc
	  END
 	ELSE
	  BEGIN
		SELECT @FriendGuid = GUID FROM T_FriendMain WHERE Name=@FriendName

		IF ( @@Error  <> 0 )
		BEGIN
			SET @ErrorCode = 6
		END
	END

	IF NOT EXISTS (SELECT GUID FROM T_FriendList WHERE GUID = @UserGuid AND FriendGuid = @FriendGuid  )
	BEGIN
		SET @ErrorCode = 2
		GOTO EndProc
	END
	
	DELETE FROM T_FriendList where GUID = @UserGuid AND FriendGuid = @FriendGuid  
	
	IF ( @@Error  <> 0 )
		SET @ErrorCode	= @@Error
	ELSE SET @ErrorCode	= 1

	IF( @ErrorCode = 1 )
	BEGIN
		UPDATE T_FriendList SET Del=1 WHERE GUID=@FriendGuid AND FriendGuid=@UserGuid
	END

EndProc:

	SET NOCOUNT OFF
	SELECT @ErrorCode
END


















GO
/****** Object:  StoredProcedure [dbo].[WZ_Get_DBID]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS OFF
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE Procedure [dbo].[WZ_Get_DBID]

AS
BEGIN	
	SET NOCOUNT ON

	SELECT [DESC] FROM Mu_DBID

	SET NOCOUNT OFF

END


















GO
/****** Object:  StoredProcedure [dbo].[WZ_GetItemSerial]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS OFF
GO
SET QUOTED_IDENTIFIER OFF
GO
CREATE Procedure [dbo].[WZ_GetItemSerial]

AS
BEGIN	
	DECLARE @ItemSerial	bigint
	SET NOCOUNT ON
	BEGIN TRANSACTION

		UPDATE GameServerInfo SET @ItemSerial = ItemCount = ItemCount+1
			
		IF ( @@error  <> 0 )
		BEGIN
			ROLLBACK TRANSACTION
			SELECT -1
		END
		ELSE
		BEGIN
			COMMIT TRANSACTION			
			SELECT @ItemSerial
		END
END


















GO
/****** Object:  StoredProcedure [dbo].[WZ_GetItemSerial2]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE Procedure [dbo].[WZ_GetItemSerial2]

	@iAddSerialCount	int
AS
BEGIN	
	DECLARE @ItemSerial	bigint

	SET NOCOUNT ON
	BEGIN TRANSACTION

	UPDATE GameServerInfo 
	SET @ItemSerial = ItemCount = ItemCount+@iAddSerialCount
		
	IF ( @@error  <> 0 )
	BEGIN
		ROLLBACK TRANSACTION
		SELECT -1
	END
	ELSE
	BEGIN
		COMMIT TRANSACTION	
		SELECT @ItemSerial-@iAddSerialCount+1 AS ItemSerial
	END

	SET NOCOUNT OFF
END



















GO
/****** Object:  StoredProcedure [dbo].[WZ_GuildCreate]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE Procedure [dbo].[WZ_GuildCreate]

	@GuildName	varchar(8),
	@MasterName 	varchar(10)
AS
BEGIN
	DECLARE @ErrorCode int

	SET @ErrorCode = 0
	SET XACT_ABORT ON

	SET NOCOUNT ON
	BEGIN TRANSACTION

	INSERT INTO Guild (G_Name, G_Master, G_Score) VALUES (@GuildName, @MasterName, 0)
	IF ( @@Error  <> 0 )
	BEGIN
		SET @ErrorCode = 1
	END

	IF ( @ErrorCode  =  0 )
	BEGIN
		INSERT GuildMember (Name, G_Name, G_Level) VALUES (@MasterName, @GuildName, 1)
		IF ( @@Error  <> 0 )
		BEGIN
			SET @ErrorCode = 2
		END
	END

	IF ( @ErrorCode  <> 0 )
		ROLLBACK TRANSACTION
	ELSE
		COMMIT TRANSACTION

	SELECT @ErrorCode

	SET NOCOUNT OFF
	SET XACT_ABORT OFF
END


















GO
/****** Object:  StoredProcedure [dbo].[WZ_IBS_AddCoin]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[WZ_IBS_AddCoin]
	-- Add the parameters for the stored procedure here
	@AccountID		varchar(10),
	@Type			int,
	@Coin			float
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;

    -- Insert statements for procedure here
	IF(@Type = 0)
	BEGIN
		UPDATE T_InGameShop_Point SET WCoinC = WCoinC+@Coin WHERE AccountID = @AccountID
	END
	IF(@Type = 1)
	BEGIN
		UPDATE T_InGameShop_Point SET WCoinP = WCoinP+@Coin WHERE AccountID = @AccountID
	END
	IF(@Type = 2)
	BEGIN
		UPDATE T_InGameShop_Point SET GoblinPoint = GoblinPoint+@Coin WHERE AccountID = @AccountID
	END
END




















GO
/****** Object:  StoredProcedure [dbo].[WZ_IBS_AddGift]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

-- =============================================
-- Author:		<Dudas, WOG Team>
-- Create date: <15 July 2011>
-- Description:	<IBS Shop Add Item procedure>
-- =============================================
CREATE PROCEDURE [dbo].[WZ_IBS_AddGift]
	-- Add the parameters for the stored procedure here
	@Target					varchar(10),
	@ID1					int,
	@ID2					int,
	@ID3					int,
	@Type					int,
	@Sender					varchar(10),
	@Message				varchar(200)
	
AS
BEGIN

	SET NOCOUNT ON;
	BEGIN TRANSACTION

	DECLARE @FreeAuthCode int
	SET @FreeAuthCode = 0

	WHILE (1 = 1)
	BEGIN
		SET @FreeAuthCode = ROUND(((2000000000 - 1 -1) * RAND() + 1), 0)
		IF NOT EXISTS (SELECT * FROM T_InGameShop_Items WHERE AuthCode = @FreeAuthCode)
		BEGIN
			BREAK
		END
	END
    
	DECLARE @GiftAccount varchar(10)
	SELECT @GiftAccount = AccountID from Character Where Name = @Target
	INSERT INTO T_InGameShop_Items (AccountID, AuthCode, UniqueID1, UniqueID2, UniqueID3, InventoryType, GiftName, Message, UsedItem) VALUES
	(@GiftAccount, @FreeAuthCode, @ID1, @ID2, @ID3, @Type, @Sender, @Message, 0)
    
	SELECT 1 AS Result
	
	IF(@@Error <> 0 )
		ROLLBACK TRANSACTION
	ELSE	
		COMMIT TRANSACTION
	
	SET NOCOUNT OFF		
END




GO
/****** Object:  StoredProcedure [dbo].[WZ_IBS_AddItem]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

-- =============================================
-- Author:		<Dudas, WOG Team>
-- Create date: <15 July 2011>
-- Description:	<IBS Shop Add Item procedure>
-- =============================================
CREATE PROCEDURE [dbo].[WZ_IBS_AddItem]
	-- Add the parameters for the stored procedure here
	@AccountID				varchar(10),
	@ID1					int,
	@ID2					int,
	@ID3					int,
	@Type					int
	
AS
BEGIN
	SET NOCOUNT ON;
	BEGIN TRANSACTION

	DECLARE @FreeAuthCode int
	SET @FreeAuthCode = 0

	WHILE (1 = 1)
	BEGIN
		SET @FreeAuthCode = ROUND(((2000000000 - 1 -1) * RAND() + 1), 0)
		IF NOT EXISTS (SELECT * FROM T_InGameShop_Items WHERE AuthCode = @FreeAuthCode)
		BEGIN
			BREAK
		END
	END
    
    INSERT INTO T_InGameShop_Items (AccountID, AuthCode, UniqueID1, UniqueID2, UniqueID3, InventoryType, UsedItem) VALUES
    (@AccountID, @FreeAuthCode, @ID1, @ID2, @ID3, @Type, 0)
    
    SELECT 1 AS Result
    
    IF(@@Error <> 0 )
		ROLLBACK TRANSACTION
	ELSE	
		COMMIT TRANSACTION

	SET NOCOUNT OFF		
END




GO
/****** Object:  StoredProcedure [dbo].[WZ_IBS_DeleteItem]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE PROCEDURE [dbo].[WZ_IBS_DeleteItem]
	@AccountID			varchar(10),
	@UniqueCode			int,
	@AuthCode			int
AS
BEGIN
	SET NOCOUNT ON;

	DECLARE @Acc	varchar(10)
	IF EXISTS ( SELECT AccountID, UniqueID1, UniqueID2, UniqueID3 FROM T_InGameShop_Items WHERE UniqueCode = @UniqueCode AND AuthCode = @AuthCode )
	BEGIN
		DELETE FROM T_InGameShop_Items WHERE UniqueCode = @UniqueCode AND AuthCode = @AuthCode AND AccountID = @AccountID
		SELECT 1 AS Result
	END
	ELSE
	BEGIN
		SELECT 0 AS Result
	END
END




GO
/****** Object:  StoredProcedure [dbo].[WZ_IBS_GetItemsList]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

-- =============================================
-- Author:		<Author,,Name>
-- Create date: <Create Date,,>
-- Description:	<Description,,>
-- =============================================
CREATE PROCEDURE [dbo].[WZ_IBS_GetItemsList]
	-- Add the parameters for the stored procedure here
	@AccountID				varchar(10),
	@Type					int																					
AS
BEGIN
	-- SET NOCOUNT ON added to prevent extra result sets from
	-- interfering with SELECT statements.
	SET NOCOUNT ON;

    -- Insert statements for procedure here
    SELECT * FROM T_InGameShop_Items WHERE AccountID = @AccountID AND InventoryType = @Type AND UsedItem = 0
END




GO
/****** Object:  StoredProcedure [dbo].[WZ_IBS_GetItemToUse]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE PROCEDURE [dbo].[WZ_IBS_GetItemToUse]
	@AccountID			varchar(10),
	@UniqueCode			int,
	@AuthCode			int
AS
BEGIN
	SET NOCOUNT ON;

	DECLARE @Acc	varchar(10)
	DECLARE @UsedFlag int

	SET @UsedFlag = 0
	IF EXISTS ( SELECT AccountID, UniqueID1, UniqueID2, UniqueID3 FROM T_InGameShop_Items WHERE UniqueCode = @UniqueCode AND AuthCode = @AuthCode )
	BEGIN
		SELECT @UsedFlag = UsedItem FROM T_InGameShop_Items WHERE UniqueCode = @UniqueCode AND AuthCode = @AuthCode AND AccountID = @AccountID

		IF (@UsedFlag = 1)
		BEGIN
			SELECT 0 AS Result
		END
		ELSE
		BEGIN
			UPDATE T_InGameShop_Items SET UsedItem = 1 WHERE UniqueCode = @UniqueCode AND AuthCode = @AuthCode AND AccountID = @AccountID
			SELECT 1 AS Result, AccountID, UniqueID1, UniqueID2, UniqueID3 FROM T_InGameShop_Items WHERE UniqueCode = @UniqueCode AND AuthCode = @AuthCode AND AccountID = @AccountID
		END
	END
	ELSE
	BEGIN
		SELECT 0 AS Result
	END
END




GO
/****** Object:  StoredProcedure [dbo].[WZ_MoveCharacter]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE Procedure [dbo].[WZ_MoveCharacter] 

	@AccountID			varchar(10),
	@Name				varchar(10),
	@ChangeAccountID		varchar(10),
	@IsOriginalAccountID 		bit
AS
BEGIN

	SET NOCOUNT ON
	SET XACT_ABORT ON
	
	DECLARE	@Result			tinyint	
	DECLARE	@ResultLowCount		int
	DECLARE	@Class				tinyint
	DECLARE	@Ctl1_Code			tinyint
	DECLARE	@SQLEXEC			varchar(1000)
	DECLARE	@ErrorCheck			INT
	DECLARE 	@g1 				varchar(10), @g2 varchar(10), @g3 varchar(10), @g4 varchar(10), @g5 varchar(10)
	DECLARE 	@MoveCnt			 tinyint		
	DECLARE 	@ChangeMoveCnt		tinyint		
	DECLARE	@SqlStmt			VARCHAR(700)		
	DECLARE	@SqlStmt2			VARCHAR(700)		
	DECLARE	@SqlStmt3			VARCHAR(700)		

	SET LOCK_TIMEOUT	1000
	SET @Result = 0x00	
	SET @ErrorCheck = 0x00

	IF EXISTS( SELECT Name FROM GuildMember   WHERE Name = @Name )
	BEGIN
		SET @Result	= 0x10
		GOTO ON_ERROR
	END

	SELECT @Class = Class, @Ctl1_Code = CtlCode FROM Character WHERE Name = @Name
	
	SELECT @ResultLowCount = @@rowcount, @ErrorCheck = @@error
  						
	IF @ResultLowCount = 0 
	BEGIN
		SET @Result	= 0x02			
		GOTO ON_ERROR						
	END

	IF @ErrorCheck  <> 0 GOTO ON_ERROR

	IF ( (@Ctl1_Code & 127 ) > 0 )
	BEGIN
		SET @Result	= 0x03			
		GOTO ON_ERROR						
	END 

	SELECT  @g1=GameID1, @g2=GameID2, @g3=GameID3, @g4=GameID4, @g5=GameID5, @MoveCnt = MoveCnt 
	FROM dbo.AccountCharacter WHERE Id = @AccountID 		
	
	SELECT @ResultLowCount = @@rowcount, @ErrorCheck = @@error

	IF @ResultLowCount = 0 
	BEGIN
		SET @Result	= 0x02			
		GOTO ON_ERROR						
	END

	IF @ErrorCheck  <> 0 GOTO ON_ERROR

	IF @MoveCnt IS NULL 
	BEGIN
		SET @MoveCnt  =0 
	END

	IF  @Class = 48 
		BEGIN
			IF ((@MoveCnt&240) > 0) 
				BEGIN	
					SET @Result	= 0x04
					GOTO ON_ERROR				
				END 
			ELSE
				BEGIN
					SET @MoveCnt =  @MoveCnt | 16	
				END 
		END
	ELSE
		BEGIN
			IF @Class = 64
				BEGIN
					IF ((@MoveCnt&240) > 0) 
						BEGIN	
							SET @Result	= 0x04
						GOTO ON_ERROR				
						END 
					ELSE
						BEGIN
							SET @MoveCnt =  @MoveCnt | 16	
						END 
				END
			ELSE
				BEGIN
					IF  ((@MoveCnt&15) > 0)
						BEGIN	
							SET @Result	= 0x04
							GOTO ON_ERROR				
						END 
					ELSE
						BEGIN
							SET @MoveCnt =  @MoveCnt | 1	
						END 
				END 
		END

	SET @SqlStmt = 'UPDATE AccountCharacter  '

	IF ( @g1 = @Name )
		SET @SqlStmt = @SqlStmt + ' SET  GameID1 = NULL,'
	ELSE IF ( @g2 = @Name )
		SET @SqlStmt = @SqlStmt + ' SET  GameID2 = NULL,'
	ELSE IF ( @g3 = @Name )
		SET @SqlStmt = @SqlStmt + ' SET  GameID3 = NULL,'
	ELSE IF ( @g4 = @Name )
		SET @SqlStmt = @SqlStmt + ' SET  GameID4 = NULL,'
	ELSE IF ( @g5 = @Name )
		SET @SqlStmt = @SqlStmt + ' SET  GameID5 = NULL,'
	ELSE 				
		SET @Result	= 0x05

	IF ( @Result <> 0 )
		GOTO ON_ERROR

	SET @SqlStmt = @SqlStmt + ' MoveCnt =  ' + CONVERT(VARCHAR, @MoveCnt )					
	SET @SqlStmt = @SqlStmt + ' WHERE Id =  ''' + @AccountID	 + ''''				

	SELECT  @g1=GameID1, @g2=GameID2, @g3=GameID3, @g4=GameID4, @g5=GameID5, @ChangeMoveCnt = MoveCnt  
	FROM dbo.AccountCharacter  Where Id = @ChangeAccountID 			

	SELECT @ResultLowCount = @@rowcount, @ErrorCheck = @@error

	IF @ErrorCheck  <> 0 GOTO ON_ERROR

	IF @ResultLowCount = 0 
	BEGIN
		SET @SqlStmt2 ='INSERT INTO dbo.AccountCharacter(Id, GameID1, GameID2, GameID3, GameID4, GameID5, GameIDC) '
		SET @SqlStmt2 = @SqlStmt2 + ' VALUES( ''' +  @ChangeAccountID + ''', '
		SET @SqlStmt2 = @SqlStmt2 + '''' + @Name + ''', '
		SET @SqlStmt2 = @SqlStmt2 +  ' NULL, NULL, NULL, NULL, NULL) '

		PRINT @SqlStmt2
	END
	ELSE
	BEGIN
		SET @SqlStmt2 = 'UPDATE AccountCharacter SET '
	
		IF( ( @g1 Is NULL) OR (Len(@g1) = 0))
			SET @SqlStmt2 = @SqlStmt2 + '  GameID1 = '
		ELSE IF ( @g2  Is NULL OR Len(@g2) = 0)
			SET @SqlStmt2 = @SqlStmt2 + '  GameID2 = '
		ELSE IF ( @g3 Is NULL OR Len(@g3) = 0)
			SET @SqlStmt2 = @SqlStmt2 + '  GameID3 = ' 
		ELSE IF ( @g4 Is NULL OR Len(@g4) = 0)
			SET @SqlStmt2 = @SqlStmt2 + '  GameID4 = '
		ELSE IF ( @g5 Is NULL OR Len(@g5) = 0)
			SET @SqlStmt2 = @SqlStmt2 + '  GameID5 = '
		ELSE 		
			SET @Result	= 0x06			
	
		if( @Result <> 0 )
			GOTO ON_ERROR
		
		SET @SqlStmt2 = @SqlStmt2 +  '''' + @Name + ''''
		SET @SqlStmt2 = @SqlStmt2 + ' WHERE Id =  ''' + @ChangeAccountID + ''''
	END

	SET @SqlStmt3 = 'UPDATE Character '
	SET @SqlStmt3 = @SqlStmt3 + 'SET  AccountID = ''' + @ChangeAccountID + ''''
	
	IF @IsOriginalAccountID = 1
		SET @SqlStmt3 = @SqlStmt3 + ', CtlCode = ' + CONVERT(VARCHAR, @Ctl1_Code & 127	)
	ELSE
		SET @SqlStmt3 = @SqlStmt3 + ', CtlCode = ' + CONVERT(VARCHAR,  @Ctl1_Code | 128	)
	
	SET @SqlStmt3 = @SqlStmt3 + ' WHERE Name = ''' +  @Name + ''''


	BEGIN TRANSACTION 

	EXEC(@SqlStmt)
	SELECT @ResultLowCount = @@rowcount,  @ErrorCheck = @@error
	IF  @ResultLowCount = 0  GOTO ON_TRN_ERROR
	IF  @ErrorCheck  <> 0 GOTO ON_TRN_ERROR

	EXEC(@SqlStmt2)
	SELECT @ResultLowCount = @@rowcount,  @ErrorCheck = @@error
	IF  @ResultLowCount = 0  GOTO ON_TRN_ERROR
	IF  @ErrorCheck  <> 0 GOTO ON_TRN_ERROR
	
	EXEC(@SqlStmt3)
	SELECT @ResultLowCount = @@rowcount,  @ErrorCheck = @@error
	IF  @ResultLowCount = 0  GOTO ON_TRN_ERROR
	IF  @ErrorCheck  <> 0 GOTO ON_TRN_ERROR

ON_TRN_ERROR:
	IF ( @Result  <> 0 ) OR (@ErrorCheck <> 0)
	BEGIN
		IF @Result = 0 
			SET @Result = 0x09

		ROLLBACK TRAN
	END
	ELSE
		COMMIT	TRAN

ON_ERROR:
	IF @ErrorCheck <> 0
	BEGIN
		SET @Result = 0x09
	END 

	SELECT @Result	

	SET NOCOUNT OFF
	SET XACT_ABORT OFF
END


















GO
/****** Object:  StoredProcedure [dbo].[WZ_RenameCharacter]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE Procedure [dbo].[WZ_RenameCharacter] 

	@ServerCode		smallint,
	@AccountID		varchar(10),
	@Name			varchar(10),
	@ChangeName		varchar(10)
AS
BEGIN

	SET NOCOUNT ON
	SET XACT_ABORT ON
	DECLARE @Result	tinyint, 	@GUID int,	@CGUID	int

	SET LOCK_TIMEOUT	5000
	SET @Result = 0x00	

	IF NOT EXISTS ( SELECT Name FROM Character (READUNCOMMITTED) WHERE Name = @Name )
	BEGIN
		SET @Result	= 0x01		
		GOTO ProcEnd						
	END

	IF NOT EXISTS ( SELECT Name FROM Character (READUNCOMMITTED) WHERE Name = @ChangeName )
	BEGIN
		SET @Result	= 0x02					
		GOTO ProcEnd						
	END

	IF EXISTS( SELECT Name FROM GuildMember (READUNCOMMITTED)  WHERE Name = @Name )
	BEGIN
		SET @Result	= 0x03
		GOTO ProcEnd
	END	
	
	SELECT @GUID=GUID FROM T_CGuid WHERE Name=@Name

	DECLARE @cLevel int,@LevelUpPoint int,@Class tinyint  ,@Experience int ,@Strength smallint  ,@Dexterity smallint  ,@Vitality smallint  ,@Energy smallint  ,@Inventory varbinary (1080)  ,@MagicList varbinary (60)  ,@Money int  ,@Life real  ,@MaxLife real  ,@Mana real  ,@MaxMana real  ,@MapNumber smallint  ,@MapPosX smallint  ,@MapPosY smallint  ,@MapDir tinyint,@PkCount int ,@PkLevel int ,@PkTime int ,@MDate smalldatetime  ,@LDate smalldatetime  ,@CtlCode tinyint ,@DbVersion tinyint,@Quest varbinary (50)  ,@Leadership smallint,@ChatLimitTime smallint
	DECLARE @O_Name varchar(10),@O_SkillKey binary(10),@O_GameOption tinyint,@O_Qkey tinyint,@O_Wkey tinyint,@O_Ekey tinyint,@O_ChatWindow tinyint
	DECLARE @F_Name varchar(10),@F_FriendCount tinyint,@F_MemoCount int,@F_MemoTotal int

	SELECT @cLevel=cLevel ,@LevelUpPoint=LevelUpPoint ,@Class=Class,@Experience=Experience  ,@Strength=Strength,@Dexterity=Dexterity,@Vitality=Vitality,@Energy=Energy,
		 @Inventory=Inventory  ,@MagicList=MagicList  ,@Money=Money   ,@Life=Life   ,
		 @MaxLife=MaxLife   ,@Mana=Mana   ,@MaxMana=MaxMana   ,@MapNumber=MapNumber,
		 @MapPosX=MapPosX   ,@MapPosY=MapPosY   ,@MapDir=MapDir ,@PkCount=PkCount  ,
		 @PkLevel=PkLevel  ,@PkTime=PkTime  ,@MDate=MDate   ,@LDate=LDate   ,@CtlCode=CtlCode,
		 @DbVersion=DbVersion ,@Quest=Quest  ,@Leadership=Leadership ,@ChatLimitTime=ChatLimitTime 
		 FROM Character WHERE Name=@Name

	SELECT @O_Name=Name, @O_SkillKey=SkillKey, @O_GameOption=GameOption,  @O_Qkey=Qkey, @O_Wkey=Wkey, @O_Ekey=Ekey ,@O_ChatWindow=ChatWindow 
		 FROM OptionData WHERE Name=@Name

	BEGIN DISTRIBUTED TRAN

	IF NOT EXISTS ( SELECT  Id  FROM  AccountCharacter  WHERE Id = @AccountID )
		BEGIN					
			SET @Result  = 0x04
			GOTO ProcTrnEnd
		END
	ELSE
		BEGIN
			DECLARE @g1 varchar(10), @g2 varchar(10), @g3 varchar(10), @g4 varchar(10), @g5 varchar(10)						
			SELECT @g1=GameID1, @g2=GameID2, @g3=GameID3, @g4=GameID4, @g5=GameID5 FROM dbo.AccountCharacter Where Id = @AccountID 			

			IF( @g1 = @Name )
				BEGIN
					UPDATE AccountCharacter SET  GameID1 = @ChangeName
					WHERE Id = @AccountID
		
					SET @Result  = @@Error
				END
			ELSE IF( @g2 = @Name )
				BEGIN
					UPDATE AccountCharacter SET  GameID2 = @ChangeName
					WHERE Id = @AccountID

					SET @Result  = @@Error
				END
			ELSE IF( @g3 = @Name )
				BEGIN		
					UPDATE AccountCharacter SET  GameID3 = @ChangeName
					WHERE Id = @AccountID

					SET @Result  = @@Error
				END
			ELSE IF( @g4 = @Name )
				BEGIN
					UPDATE AccountCharacter SET  GameID4 = @ChangeName
					WHERE Id = @AccountID

					SET @Result  = @@Error
				END
			ELSE IF( @g5 = @Name )
				BEGIN
					UPDATE AccountCharacter SET  GameID5 = @ChangeName
					WHERE Id = @AccountID

					SET @Result  = @@Error
				END
			ELSE	
				BEGIN			
					SET @Result	= 0x05				
					GOTO ProcTrnEnd								
				END

			IF( @g1 = @ChangeName )
				BEGIN
					UPDATE AccountCharacter SET  GameID1 = ''
					WHERE Id = @AccountID
		
					SET @Result  = @@Error
				END
			ELSE IF( @g2 = @ChangeName )
				BEGIN
					UPDATE AccountCharacter SET  GameID2 = ''
					WHERE Id = @AccountID

					SET @Result  = @@Error
				END
			ELSE IF( @g3 = @ChangeName )
				BEGIN	
					UPDATE AccountCharacter SET  GameID3 = ''
					WHERE Id = @AccountID

					SET @Result  = @@Error
				END
			ELSE IF( @g4 = @ChangeName )
				BEGIN
					UPDATE AccountCharacter SET  GameID4 = ''
					WHERE Id = @AccountID

					SET @Result  = @@Error
				END
			ELSE IF( @g5 = @ChangeName )
				BEGIN
					UPDATE AccountCharacter SET  GameID5 = ''
					WHERE Id = @AccountID

					SET @Result  = @@Error
				END	
			ELSE	
				BEGIN		
					SET @Result	= 0x05			
					GOTO ProcTrnEnd								
				END	
	 		END

	IF ( @Result <> 0 )
	BEGIN
		GOTO ProcTrnEnd	
	END

	IF ( @Result = 0 )
	BEGIN
		
		UPDATE Character	
		SET
		cLevel=@cLevel ,LevelUpPoint=@LevelUpPoint ,Class=@Class,Experience=@Experience  ,
		Strength=@Strength,Dexterity=@Dexterity,Vitality=@Vitality,Energy=@Energy,
		Inventory=@Inventory  ,MagicList=@MagicList  ,Money=@Money   ,Life=@Life   ,
		MaxLife=@MaxLife   ,Mana=@Mana   ,MaxMana=@MaxMana   ,MapNumber=@MapNumber,
		MapPosX=@MapPosX   ,MapPosY=@MapPosY   ,MapDir=@MapDir ,PkCount=@PkCount  ,
		PkLevel=@PkLevel  ,PkTime=@PkTime  ,MDate=@MDate   ,LDate=@LDate   ,CtlCode=@CtlCode,
		DbVersion=@DbVersion ,Quest=@Quest  ,Leadership=@Leadership ,ChatLimitTime=@ChatLimitTime 
		FROM Character WHERE Name=@ChangeName


		UPDATE OptionData SET SkillKey=@O_SkillKey,GameOption= @O_GameOption, Qkey= @O_Qkey, Wkey=@O_Wkey, Ekey=@O_Ekey ,ChatWindow=@O_ChatWindow 
		WHERE Name=@ChangeName
		
		SET @Result =  @@Error
		IF @Result <> 0 
			GOTO ProcTrnEnd	
	END 

ProcTrnEnd:
	IF ( @Result  <> 0 )
		ROLLBACK TRAN
	ELSE
		COMMIT TRAN


ProcEnd:
	SELECT @Result	

	SET NOCOUNT OFF
	SET XACT_ABORT OFF
END


















GO
/****** Object:  StoredProcedure [dbo].[WZ_SetGuildDelete]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE Procedure [dbo].[WZ_SetGuildDelete]

	@GuildName	varchar(10)
AS
BEGIN
	SET NOCOUNT ON
	DECLARE @Result	int
	SET @Result = 1 

	BEGIN TRANSACTION
		DELETE GuildMember WHERE G_Name = @GuildName		
		IF @@Error <> 0 
		BEGIN	
			SET @Result = 0
			GOTO PROBLEM	
		END

		DELETE Guild WHERE G_Name = @GuildName		
		IF @@Error <> 0 
			BEGIN
				SET @Result = 0
				GOTO PROBLEM	
			END
		ELSE GOTO SUCESS

	PROBLEM:
		ROLLBACK TRANSACTION
 
	SUCESS:
   		COMMIT TRANSACTION
				
	SELECT @Result AS Result
	SET NOCOUNT OFF
END


















GO
/****** Object:  StoredProcedure [dbo].[WZ_UserGuidCreate]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER OFF
GO
CREATE Procedure [dbo].[WZ_UserGuidCreate]

	@Name 	varchar(10)
AS
BEGIN
	DECLARE @ErrorCode int
	DECLARE @UserGuid  int

	SET @ErrorCode = 1
	SET @UserGuid = -1

	SET XACT_ABORT ON
	SET NOCOUNT ON

	IF EXISTS ( SELECT GUID FROM T_CGuid WHERE Name=@Name ) 
	  BEGIN
		SET @ErrorCode = 0
		GOTO EndProc
	  END

	BEGIN TRAN

	INSERT INTO T_CGuid (Name) VALUES(@Name)

	IF ( @@Error  <> 0 )
	  BEGIN
		SET @ErrorCode = 2
	  END
	ELSE 
	  BEGIN
		select @UserGuid = GUID FROM T_CGuid where Name=@Name
		IF ( @@Error  <> 0 )
		  BEGIN
			SET @ErrorCode = 3
		  END
		ELSE 
		  BEGIN
			INSERT INTO T_FriendMain ( GUID, Name, FriendCount, MemoCount) VALUES(@UserGuid, @Name,1,10)
			IF ( @@Error  <> 0 )
				SET @ErrorCode = 4
		  END
	  END

EndTranProc:
	IF ( @@Error  <> 0 )
		ROLLBACK TRAN
	ELSE COMMIT TRAN

EndProc:
	SELECT @ErrorCode
	SET XACT_ABORT OFF
	SET NOCOUNT OFF
END


















GO
/****** Object:  StoredProcedure [dbo].[WZ_WaitFriendAdd]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER OFF
GO
CREATE Procedure [dbo].[WZ_WaitFriendAdd]

	@Name 	varchar(10),
	@FriendName 	varchar(10)
AS
BEGIN
	DECLARE @ErrorCode int
	DECLARE @UserGuid  int
	DECLARE @FriendGuid  int
	DECLARE @FriendLevel  int

	SET NOCOUNT ON
	SET XACT_ABORT ON
	

	SET @ErrorCode = 0

	IF EXISTS (SELECT GUID FROM T_FriendMain WHERE Name=@Name)
  	  BEGIN
		select @UserGuid = GUID FROM T_FriendMain WHERE Name=@Name
		IF ( @@Error  <> 0 )
		BEGIN
			SET @ErrorCode = 2
			GOTO EndProc
		END
	  END
	ELSE
	  BEGIN
		SET @ErrorCode = 3
		GOTO EndProc
	  END

	IF EXISTS (SELECT GUID FROM T_FriendMain WHERE Name=@FriendName)
	  BEGIN
		SELECT @FriendGuid = GUID FROM T_FriendMain WHERE Name=@FriendName
		IF ( @@Error  <> 0 )
		BEGIN
			SET @ErrorCode	= 4
			GOTO EndProc
		END
  	  END
	ELSE
	  BEGIN
		SET @ErrorCode	= 5
		GOTO EndProc
  	  END

	SELECT @FriendLevel=cLevel FROM Character WHERE Name=@FriendName
	IF( @@Error <> 0 )
	  BEGIN
		SET @ErrorCode = 5		
		GOTO EndProc
	  END
	ELSE 
	  BEGIN
		IF( @FriendLevel < 6 )
		  BEGIN
			SET @ErrorCode = 6
			GOTO EndProc
		 END
	  END

	BEGIN TRAN

	INSERT INTO T_FriendList (GUID, FriendGuid, FriendName ) 
		VALUES ( @UserGuid, @FriendGuid, @FriendName)
	
	IF ( @@Error  <> 0 )
	BEGIN
		SET @ErrorCode = 7
		GOTO EndTranProc
	END

	IF EXISTS (SELECT GUID FROM T_FriendList WHERE GUID = @FriendGuid AND FriendGuid =  @UserGuid )
	 BEGIN
		UPDATE T_FriendList SET Del=0 WHERE GUID=@FriendGuid AND FriendGuid=@UserGuid
		SET @ErrorCode = 8
		GOTO EndTranProc
	END

	INSERT INTO T_WaitFriend (GUID, FriendName, FriendGuid ) 
				VALUES ( @FriendGuid, @Name, @UserGuid)
	
	IF ( @@Error  <> 0 )
	BEGIN
		SET @ErrorCode = 6
		GOTO EndTranProc
	END

EndTranProc:
	IF ( (@ErrorCode  = 0) OR (@ErrorCode  = 8) )
	  BEGIN
		COMMIT TRAN
	  END
	ELSE
	  BEGIN
		ROLLBACK TRAN
	  END
	
EndProc:

	SET	XACT_ABORT OFF

	SET NOCOUNT OFF

	SELECT @ErrorCode
END


















GO
/****** Object:  StoredProcedure [dbo].[WZ_WaitFriendDel]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE Procedure [dbo].[WZ_WaitFriendDel]

	@Name		 varchar(10), 
	@FriendName 	 varchar(10)
AS
BEGIN
	DECLARE @ErrorCode int
	DECLARE @UserGuid  int
	DECLARE @FriendGuid  int

	SET NOCOUNT ON

	SET @ErrorCode = 0

	IF NOT EXISTS ( select GUID FROM T_FriendMain where Name=@Name)
	  BEGIN
		SET @ErrorCode = 3
		GOTO EndProc

	  END
	ELSE
	  BEGIN
		SELECT @UserGuid = GUID FROM T_FriendMain where Name=@Name
	
		IF ( @@Error  <> 0   )
		BEGIN
			SET @ErrorCode = 4
		END
	  END

	IF NOT EXISTS ( select GUID FROM T_FriendMain where Name=@FriendName )
	  BEGIN
		SET @ErrorCode = 5
		GOTO EndProc
	  END
 	ELSE
	  BEGIN
		SELECT @FriendGuid = GUID FROM T_FriendMain where Name=@FriendName

		IF ( @@Error  <> 0 )
		BEGIN
			SET @ErrorCode = 6
		END

	END

	IF NOT EXISTS (SELECT GUID FROM T_WaitFriend where GUID = @UserGuid AND FriendGuid = @FriendGuid  )
	BEGIN
		SET @ErrorCode = 2
		GOTO EndProc
	END
	
	DELETE FROM T_WaitFriend where GUID = @UserGuid AND FriendGuid = @FriendGuid  
	
	IF ( @@Error  <> 0 )
		SET @ErrorCode	= @@Error

	IF( @ErrorCode = 0 )
	BEGIN
		UPDATE T_FriendList SET Del=1 where GUID=@FriendGuid AND FriendGuid=@UserGuid
	END

EndProc:

	SET NOCOUNT OFF
	SELECT @ErrorCode
END


















GO
/****** Object:  StoredProcedure [dbo].[WZ_WriteMail]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER OFF
GO
CREATE Procedure [dbo].[WZ_WriteMail]

	@SendName 	varchar(10), 
	@RecvName 	varchar(10),
	@Subject	varchar(32),
	@Dir		tinyint,
	@Act		tinyint
AS
BEGIN
	SET NOCOUNT ON

	DECLARE 	@userguid	int
	DECLARE 	@memocount	int
	DECLARE	@return		int
	DECLARE	@MemoTotal	int
	DECLARE 	@FriendLevel  	int

	SET	XACT_ABORT ON

	SET @return	= 0

	SELECT @userguid=GUID, @MemoTotal=MemoTotal FROM T_FriendMain where Name= @RecvName
	IF( @@ROWCOUNT < 1 )
	BEGIN
		SET @return = 2
		GOTO EndProc
	END

	IF( @MemoTotal > 49 )
	BEGIN
		SET @return = 5
		GOTO EndProc
	END

	SELECT @FriendLevel=cLevel FROM Character WHERE Name=@RecvName
	IF( @@Error <> 0 )
	 BEGIN
		SET @return = 2		
		GOTO EndProc
	 END
	ELSE 
	  BEGIN
		IF( @FriendLevel < 6 )
		 BEGIN
			SET @return = 6
			GOTO EndProc
		END
	  END

	BEGIN TRANSACTION

	UPDATE T_FriendMain set @memocount = MemoCount = MemoCount+1, MemoTotal=MemoTotal+1 WHERE Name = @RecvName
	IF( @@error <> 0 )
	BEGIN
		SET @return = 3
		GOTO EndProcTran
	END	

	INSERT INTO T_FriendMail (MemoIndex, GUID, FriendName, wDate, Subject,bRead,  Dir,  Act) VALUES(@memocount,@userguid, @SendName, getdate(), @Subject, 0,  @Dir, @Act)
	IF( @@error <> 0 )
	BEGIN
		SET @return = 4
		GOTO EndProcTran
	END

EndProcTran:
	IF ( @return  <> 0 )
	BEGIN
		ROLLBACK TRANSACTION
	END
	ELSE
	BEGIN
		COMMIT TRANSACTION
		SET @return = @memocount
	END
	
EndProc:
	SET XACT_ABORT OFF
	SET NOCOUNT OFF
	
	SELECT @return, @userguid
END


















GO
/****** Object:  UserDefinedFunction [dbo].[fn_md5]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE FUNCTION [dbo].[fn_md5] (@data VARCHAR(20), @data2 VARCHAR(10))
RETURNS BINARY(16) AS
BEGIN
DECLARE @hash BINARY(16)
EXEC master.dbo.XP_MD5_EncodeKeyVal @data, @data2, @hash OUT
RETURN @hash
END
GO
/****** Object:  UserDefinedFunction [dbo].[IGC_FriendChat_GetMessageLogs]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO


CREATE FUNCTION [dbo].[IGC_FriendChat_GetMessageLogs] (@Name varchar(10), @FriendName varchar(10), @RecordCount	int)
RETURNS @OrderShipperTab TABLE
   (
    Name   nvarchar(10),
    [Text]       nvarchar(135)
   )
AS
BEGIN
DECLARE @LastID int;
SET @LastID = (SELECT TOP 1 ID FROM IGC_FriendChat_MessageLog  WHERE (Name = @Name AND FriendName = @FriendName) OR (Name = @FriendName AND FriendName = @Name) ORDER BY ID DESC);
INSERT INTO @OrderShipperTab SELECT Name, [Text] FROM IGC_FriendChat_MessageLog WHERE (Name = @Name AND FriendName = @FriendName) OR (Name = @FriendName AND FriendName = @Name) AND ID <= @LastID AND ID > (@LastID - @RecordCount);
RETURN
END


















GO
/****** Object:  Table [dbo].[AccountCharacter]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[AccountCharacter](
	[Number] [int] IDENTITY(1,1) NOT NULL,
	[Id] [varchar](10) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[GameID1] [varchar](10) COLLATE SQL_Latin1_General_CP1_CI_AS NULL,
	[GameID2] [varchar](10) COLLATE SQL_Latin1_General_CP1_CI_AS NULL,
	[GameID3] [varchar](10) COLLATE SQL_Latin1_General_CP1_CI_AS NULL,
	[GameID4] [varchar](10) COLLATE SQL_Latin1_General_CP1_CI_AS NULL,
	[GameID5] [varchar](10) COLLATE SQL_Latin1_General_CP1_CI_AS NULL,
	[GameIDC] [varchar](10) COLLATE SQL_Latin1_General_CP1_CI_AS NULL,
	[MoveCnt] [tinyint] NULL,
	[Summoner] [tinyint] NOT NULL,
	[WarehouseExpansion] [tinyint] NOT NULL,
	[RageFighter] [tinyint] NOT NULL,
	[SecCode] [int] NOT NULL,
 CONSTRAINT [PK_AccountCharacter] PRIMARY KEY CLUSTERED 
(
	[Id] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[C_Monster_KillCount]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[C_Monster_KillCount](
	[name] [varchar](10) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[MonsterId] [int] NOT NULL,
	[count] [int] NOT NULL
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[C_PlayerKiller_Info]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[C_PlayerKiller_Info](
	[Victim] [varchar](20) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[Killer] [varchar](20) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[KillDate] [datetime] NOT NULL
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[Character]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[Character](
	[AccountID] [varchar](10) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[Name] [varchar](10) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[cLevel] [int] NULL,
	[LevelUpPoint] [int] NULL,
	[Class] [tinyint] NULL,
	[Experience] [bigint] NULL,
	[Strength] [int] NULL,
	[Dexterity] [int] NULL,
	[Vitality] [int] NULL,
	[Energy] [int] NULL,
	[MagicList] [varbinary](450) NULL,
	[Money] [int] NULL,
	[Life] [real] NULL,
	[MaxLife] [real] NULL,
	[Mana] [real] NULL,
	[MaxMana] [real] NULL,
	[MapNumber] [smallint] NULL,
	[MapPosX] [smallint] NULL,
	[MapPosY] [smallint] NULL,
	[MapDir] [tinyint] NULL,
	[PkCount] [int] NULL,
	[PkLevel] [int] NULL,
	[PkTime] [int] NULL,
	[MDate] [smalldatetime] NULL,
	[LDate] [smalldatetime] NULL,
	[CtlCode] [tinyint] NULL,
	[Quest] [varbinary](100) NULL,
	[Leadership] [int] NULL,
	[ChatLimitTime] [smallint] NOT NULL,
	[FruitPoint] [int] NOT NULL,
	[RESETS] [int] NOT NULL,
	[Inventory] [varbinary](7584) NULL,
	[Married] [int] NULL,
	[MarryName] [varchar](10) COLLATE SQL_Latin1_General_CP1_CI_AS NULL,
	[MuBotData] [varbinary](257) NULL,
	[mLevel] [int] NOT NULL,
	[mlPoint] [int] NOT NULL,
	[mlExperience] [bigint] NOT NULL,
	[mlNextExp] [bigint] NOT NULL,
	[InventoryExpansion] [tinyint] NOT NULL,
	[WinDuels] [int] NOT NULL,
	[LoseDuels] [int] NOT NULL,
	[PenaltyMask] [int] NOT NULL,
	[BlockChatTime] [bigint] NULL
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[ConnectionHistory]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[ConnectionHistory](
	[ID] [int] IDENTITY(1,1) NOT NULL,
	[AccountID] [varchar](10) COLLATE SQL_Latin1_General_CP1_CI_AS NULL,
	[ServerName] [varchar](40) COLLATE SQL_Latin1_General_CP1_CI_AS NULL,
	[IP] [varchar](16) COLLATE SQL_Latin1_General_CP1_CI_AS NULL,
	[Date] [datetime] NULL,
	[State] [varchar](12) COLLATE SQL_Latin1_General_CP1_CI_AS NULL,
	[HWID] [varchar](100) COLLATE SQL_Latin1_General_CP1_CI_AS NULL,
 CONSTRAINT [PK__Connecti__3214EC27FA8B4F22] PRIMARY KEY CLUSTERED 
(
	[ID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[DefaultClassType]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[DefaultClassType](
	[Class] [tinyint] NOT NULL,
	[Strength] [smallint] NULL,
	[Dexterity] [smallint] NULL,
	[Vitality] [smallint] NULL,
	[Energy] [smallint] NULL,
	[MagicList] [varbinary](450) NULL,
	[Life] [real] NULL,
	[MaxLife] [real] NULL,
	[Mana] [real] NULL,
	[MaxMana] [real] NULL,
	[MapNumber] [smallint] NULL,
	[MapPosX] [smallint] NULL,
	[MapPosY] [smallint] NULL,
	[Quest] [varbinary](100) NULL,
	[DbVersion] [tinyint] NULL,
	[Leadership] [smallint] NULL,
	[Level] [smallint] NULL,
	[LevelUpPoint] [smallint] NULL,
	[Inventory] [varbinary](7584) NULL,
	[LevelLife] [real] NULL,
	[LevelMana] [real] NULL,
	[VitalityToLife] [real] NULL,
	[EnergyToMana] [real] NULL,
 CONSTRAINT [PK_DefaultClassType] PRIMARY KEY CLUSTERED 
(
	[Class] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[GameServerInfo]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[GameServerInfo](
	[Number] [int] NOT NULL,
	[ItemCount] [bigint] NULL,
	[ZenCount] [int] NULL,
	[GmItemCount] [int] NULL,
	[AceItemCount] [int] NULL,
	[GensRankingMonth] [int] NULL
) ON [PRIMARY]

GO
/****** Object:  Table [dbo].[Guild]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[Guild](
	[G_Name] [varchar](8) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[G_Mark] [varbinary](32) NULL,
	[G_Score] [int] NOT NULL,
	[G_Master] [varchar](10) COLLATE SQL_Latin1_General_CP1_CI_AS NULL,
	[G_Count] [int] NULL,
	[G_Notice] [varchar](60) COLLATE SQL_Latin1_General_CP1_CI_AS NULL,
	[Number] [int] IDENTITY(1,1) NOT NULL,
	[G_Type] [int] NOT NULL,
	[G_Rival] [int] NOT NULL,
	[G_Union] [int] NOT NULL,
	[G_Warehouse] [varbinary](3840) NULL,
	[G_WHPassword] [int] NULL,
	[G_WHMoney] [int] NULL,
 CONSTRAINT [PK_Guild] PRIMARY KEY CLUSTERED 
(
	[G_Name] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[GuildMember]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[GuildMember](
	[Name] [varchar](10) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[G_Name] [varchar](8) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[G_Level] [tinyint] NULL,
	[G_Status] [tinyint] NOT NULL,
 CONSTRAINT [PK_GuildMember] PRIMARY KEY CLUSTERED 
(
	[Name] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[IGC_ARCA_BATTLE_GUILD_JOIN_INFO]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[IGC_ARCA_BATTLE_GUILD_JOIN_INFO](
	[G_Name] [varchar](8) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[G_Master] [varchar](10) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[Number] [int] NOT NULL,
	[JoinDate] [smalldatetime] NULL,
	[GroupNum] [tinyint] NOT NULL
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[IGC_ARCA_BATTLE_GUILDMARK_REG]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[IGC_ARCA_BATTLE_GUILDMARK_REG](
	[Index] [int] IDENTITY(1,1) NOT NULL,
	[G_Number] [int] NOT NULL,
	[G_Name] [varchar](8) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[G_Master] [varchar](10) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[RegDate] [smalldatetime] NULL,
	[GuildRegRank] [int] NULL,
	[MarkCnt] [bigint] NOT NULL
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[IGC_ARCA_BATTLE_MEMBER_JOIN_INFO]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[IGC_ARCA_BATTLE_MEMBER_JOIN_INFO](
	[G_Name] [varchar](8) COLLATE SQL_Latin1_General_CP1_CI_AS NULL,
	[Number] [int] NOT NULL,
	[CharName] [varchar](10) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[JoinDate] [smalldatetime] NULL
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[IGC_ARCA_BATTLE_PROC_STATE]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[IGC_ARCA_BATTLE_PROC_STATE](
	[Proc_State] [tinyint] NOT NULL
) ON [PRIMARY]

GO
/****** Object:  Table [dbo].[IGC_ARCA_BATTLE_WIN_GUILD_INFO]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[IGC_ARCA_BATTLE_WIN_GUILD_INFO](
	[G_Name] [varchar](8) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[G_Number] [int] NOT NULL,
	[WinDate] [smalldatetime] NOT NULL,
	[OuccupyObelisk] [tinyint] NOT NULL,
	[ObeliskGroup] [tinyint] NOT NULL,
	[LeftTime] [bigint] NOT NULL
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[IGC_CancelItemSale]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[IGC_CancelItemSale](
	[AccountID] [varchar](10) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[Name] [varchar](10) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[ItemNumber] [tinyint] NOT NULL,
	[ItemCount] [int] NOT NULL,
	[SellDate] [bigint] NOT NULL,
	[SellExpireDate] [bigint] NOT NULL,
	[SellPrice] [int] NOT NULL,
	[ItemCode] [smallint] NOT NULL,
	[ItemLevel] [tinyint] NOT NULL,
	[ItemOp1] [tinyint] NOT NULL,
	[ItemOp2] [tinyint] NOT NULL,
	[ItemOp3] [tinyint] NOT NULL,
	[ItemDur] [tinyint] NOT NULL,
	[ItemExcOp] [tinyint] NOT NULL,
	[ItemSetOp] [tinyint] NOT NULL,
	[ItemHarmonyOp] [smallint] NOT NULL,
	[ItemOpEx] [smallint] NOT NULL,
	[ItemSocketOp1] [smallint] NOT NULL,
	[ItemSocketOp2] [smallint] NOT NULL,
	[ItemSocketOp3] [smallint] NOT NULL,
	[ItemSocketOp4] [smallint] NOT NULL,
	[ItemSocketOp5] [smallint] NOT NULL,
	[ItemBonusSocketOp] [smallint] NOT NULL,
	[ItemSerial] [bigint] NOT NULL
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[IGC_FriendChat_BannedCharacters]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[IGC_FriendChat_BannedCharacters](
	[Name] [nvarchar](10) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
 CONSTRAINT [PK_IGC_FriendChat_BannedCharacters] PRIMARY KEY CLUSTERED 
(
	[Name] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
/****** Object:  Table [dbo].[IGC_FriendChat_BannedIPs]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[IGC_FriendChat_BannedIPs](
	[IP] [nvarchar](15) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
 CONSTRAINT [PK_IGC_FriendChat_BannedIPs] PRIMARY KEY CLUSTERED 
(
	[IP] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
/****** Object:  Table [dbo].[IGC_FriendChat_MessageLog]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[IGC_FriendChat_MessageLog](
	[Name] [nvarchar](10) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[FriendName] [nvarchar](2800) COLLATE SQL_Latin1_General_CP1_CI_AS NULL,
	[Text] [nvarchar](135) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[Date] [smalldatetime] NOT NULL,
	[ID] [bigint] IDENTITY(1,1) NOT NULL,
PRIMARY KEY CLUSTERED 
(
	[ID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
/****** Object:  Table [dbo].[IGC_Gens]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[IGC_Gens](
	[Name] [varchar](10) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[Influence] [tinyint] NOT NULL,
	[Rank] [int] NOT NULL,
	[Points] [int] NOT NULL,
	[Reward] [tinyint] NOT NULL,
	[Class] [smallint] NOT NULL,
	[LeaveDate] [smalldatetime] NULL,
 CONSTRAINT [PK_IGC_Gens] PRIMARY KEY CLUSTERED 
(
	[Name] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[IGC_GensAbuse]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[IGC_GensAbuse](
	[Name] [varchar](10) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[KillName] [varchar](10) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[KillCount] [smallint] NULL
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[IGC_GremoryCase]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[IGC_GremoryCase](
	[AccountID] [varchar](10) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[Name] [varchar](10) COLLATE SQL_Latin1_General_CP1_CI_AS NULL,
	[StorageType] [tinyint] NOT NULL,
	[RewardSource] [smallint] NOT NULL,
	[AuthCode] [int] NOT NULL,
	[ItemGUID] [smallint] NOT NULL,
	[ItemID] [int] NOT NULL,
	[ItemLevel] [tinyint] NOT NULL,
	[ItemDurability] [smallint] NOT NULL,
	[ItemOp1] [tinyint] NOT NULL,
	[ItemOp2] [tinyint] NOT NULL,
	[ItemOp3] [tinyint] NOT NULL,
	[ItemExcOption] [smallint] NOT NULL,
	[ItemSetOption] [smallint] NOT NULL,
	[ItemSocketCount] [tinyint] NOT NULL,
	[ItemMainAttribute] [tinyint] NOT NULL,
	[ItemMuunEvoItemType] [smallint] NOT NULL,
	[ItemMuunEvoItemIndex] [smallint] NOT NULL,
	[ReceiveDate] [bigint] NOT NULL,
	[ExpireDate] [bigint] NOT NULL
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[IGC_GuildMatching]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[IGC_GuildMatching](
	[identNo] [bigint] IDENTITY(1,1) NOT NULL,
	[GuildName] [varchar](8) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[GuildNum] [int] NULL,
	[GuildMasterName] [varchar](10) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[GuildMasterLevel] [smallint] NULL,
	[GuildMasterClass] [tinyint] NULL,
	[GuildMemberCnt] [smallint] NULL,
	[Memo] [varchar](40) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[InterestType] [tinyint] NULL,
	[LevelRange] [tinyint] NULL,
	[ClassType] [tinyint] NULL,
	[GensType] [tinyint] NULL,
	[RegDate] [datetime] NULL
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[IGC_MachineID_Banned]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[IGC_MachineID_Banned](
	[HWID] [varchar](100) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[Note] [varchar](200) COLLATE SQL_Latin1_General_CP1_CI_AS NULL,
 CONSTRAINT [PK_IGC_MachineID_Banned] PRIMARY KEY CLUSTERED 
(
	[HWID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[IGC_PartyMatching]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[IGC_PartyMatching](
	[IdentNo] [bigint] IDENTITY(1,1) NOT NULL,
	[PartyLeaderName] [varchar](10) COLLATE SQL_Latin1_General_CP1_CI_AS NULL,
	[Title] [varchar](40) COLLATE SQL_Latin1_General_CP1_CI_AS NULL,
	[MinLevel] [smallint] NULL,
	[MaxLevel] [smallint] NULL,
	[HuntingGround] [smallint] NULL,
	[WantedClass] [tinyint] NULL,
	[CurMemberCount] [tinyint] NULL,
	[AcceptType] [tinyint] NULL,
	[UsePassWord] [tinyint] NULL,
	[PassWord] [varchar](4) COLLATE SQL_Latin1_General_CP1_CI_AS NULL,
	[WantedClassDetailInfo1] [tinyint] NULL,
	[WantedClassDetailInfo2] [tinyint] NULL,
	[WantedClassDetailInfo3] [tinyint] NULL,
	[WantedClassDetailInfo4] [tinyint] NULL,
	[WantedClassDetailInfo5] [tinyint] NULL,
	[WantedClassDetailInfo6] [tinyint] NULL,
	[WantedClassDetailInfo7] [tinyint] NULL,
	[LeaderChannel] [tinyint] NULL,
	[GensType] [tinyint] NULL,
	[LeaderLevel] [smallint] NULL,
	[LeaderClass] [tinyint] NULL,
	[RegDate] [datetime] NULL,
 CONSTRAINT [PK_IGC_PartyMatching] PRIMARY KEY CLUSTERED 
(
	[IdentNo] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[IGC_PeriodBuffInfo]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[IGC_PeriodBuffInfo](
	[id] [int] IDENTITY(1,1) NOT NULL,
	[CharacterName] [varchar](10) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[BuffIndex] [smallint] NOT NULL,
	[EffectType1] [smallint] NOT NULL,
	[EffectType2] [smallint] NULL,
	[ExpireDate] [bigint] NOT NULL,
	[Duration] [int] NOT NULL,
 CONSTRAINT [PK_IGC_PeriodBuffInfo] PRIMARY KEY CLUSTERED 
(
	[id] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[IGC_PeriodItemInfo]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[IGC_PeriodItemInfo](
	[id] [int] IDENTITY(1,1) NOT NULL,
	[UserGUID] [int] NOT NULL,
	[CharacterName] [varchar](10) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[ItemType] [smallint] NOT NULL,
	[ItemCode] [smallint] NOT NULL,
	[EffectCategory] [smallint] NOT NULL,
	[EffectType1] [smallint] NOT NULL,
	[EffectType2] [smallint] NULL,
	[Serial] [bigint] NOT NULL,
	[Duration] [int] NOT NULL,
	[BuyDate] [bigint] NOT NULL,
	[ExpireDate] [bigint] NOT NULL,
	[UsedInfo] [int] NOT NULL,
	[BuyDateString] [smalldatetime] NULL,
	[ExpireDateString] [smalldatetime] NULL,
	[SetExpire] [tinyint] NOT NULL,
 CONSTRAINT [PK_IGC_PeriodItemInfo] PRIMARY KEY CLUSTERED 
(
	[id] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[IGC_WaitGuildMatching]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[IGC_WaitGuildMatching](
	[identNo] [bigint] IDENTITY(1,1) NOT NULL,
	[GuildNumber] [int] NOT NULL,
	[GuildName] [varchar](8) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[GuildMasterName] [varchar](10) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[ApplicantName] [varchar](10) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[ApplicantClass] [tinyint] NULL,
	[ApplicantLevel] [smallint] NULL,
	[State] [tinyint] NULL,
	[RegDate] [datetime] NULL
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[IGC_WaitPartyMatching]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[IGC_WaitPartyMatching](
	[IdentNo] [bigint] IDENTITY(1,1) NOT NULL,
	[LeaderName] [varchar](10) COLLATE SQL_Latin1_General_CP1_CI_AS NULL,
	[MemberName] [varchar](10) COLLATE SQL_Latin1_General_CP1_CI_AS NULL,
	[Class] [tinyint] NULL,
	[MemberLevel] [smallint] NULL,
	[MemberDBNumber] [int] NULL,
	[RegDate] [datetime] NULL,
 CONSTRAINT [PK_IGC_WaitPartyMatching] PRIMARY KEY CLUSTERED 
(
	[IdentNo] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[MEMB_CREDITS]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[MEMB_CREDITS](
	[memb___id] [varchar](10) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[credits] [int] NOT NULL,
	[used] [int] NULL,
 CONSTRAINT [PK_MEMB_CREDITS] PRIMARY KEY CLUSTERED 
(
	[memb___id] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[MEMB_INFO]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[MEMB_INFO](
	[memb_guid] [int] IDENTITY(1,1) NOT NULL,
	[memb___id] [varchar](10) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[memb__pwd] [binary](16) NOT NULL,
	[memb_name] [varchar](10) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[sno__numb] [char](13) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[post_code] [char](6) COLLATE SQL_Latin1_General_CP1_CI_AS NULL,
	[addr_info] [varchar](50) COLLATE SQL_Latin1_General_CP1_CI_AS NULL,
	[addr_deta] [varchar](50) COLLATE SQL_Latin1_General_CP1_CI_AS NULL,
	[tel__numb] [varchar](20) COLLATE SQL_Latin1_General_CP1_CI_AS NULL,
	[phon_numb] [varchar](15) COLLATE SQL_Latin1_General_CP1_CI_AS NULL,
	[mail_addr] [varchar](50) COLLATE SQL_Latin1_General_CP1_CI_AS NULL,
	[fpas_ques] [varchar](50) COLLATE SQL_Latin1_General_CP1_CI_AS NULL,
	[fpas_answ] [varchar](50) COLLATE SQL_Latin1_General_CP1_CI_AS NULL,
	[job__code] [char](2) COLLATE SQL_Latin1_General_CP1_CI_AS NULL,
	[appl_days] [datetime] NULL,
	[modi_days] [datetime] NULL,
	[out__days] [datetime] NULL,
	[true_days] [datetime] NULL,
	[mail_chek] [char](1) COLLATE SQL_Latin1_General_CP1_CI_AS NULL,
	[bloc_code] [char](1) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[ctl1_code] [char](1) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[cspoints] [int] NULL,
	[VipType] [int] NULL,
	[VipStart] [datetime] NULL,
	[VipDays] [datetime] NULL,
	[JoinDate] [varchar](23) COLLATE SQL_Latin1_General_CP1_CI_AS NULL
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[MEMB_STAT]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[MEMB_STAT](
	[memb___id] [varchar](10) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[ConnectStat] [tinyint] NULL,
	[ServerName] [varchar](20) COLLATE SQL_Latin1_General_CP1_CI_AS NULL,
	[IP] [varchar](15) COLLATE SQL_Latin1_General_CP1_CI_AS NULL,
	[ConnectTM] [smalldatetime] NULL,
	[DisConnectTM] [smalldatetime] NULL,
 CONSTRAINT [PK_MEMB_STAT] PRIMARY KEY CLUSTERED 
(
	[memb___id] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[Mu_DBID]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[Mu_DBID](
	[DESC] [varchar](20) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[Version] [int] NOT NULL
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[MuCastle_DATA]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[MuCastle_DATA](
	[MAP_SVR_GROUP] [int] NOT NULL,
	[SIEGE_START_DATE] [datetime] NOT NULL,
	[SIEGE_END_DATE] [datetime] NOT NULL,
	[SIEGE_GUILDLIST_SETTED] [bit] NOT NULL,
	[SIEGE_ENDED] [bit] NOT NULL,
	[CASTLE_OCCUPY] [bit] NOT NULL,
	[OWNER_GUILD] [varchar](8) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[MONEY] [money] NOT NULL,
	[TAX_RATE_CHAOS] [int] NOT NULL,
	[TAX_RATE_STORE] [int] NOT NULL,
	[TAX_HUNT_ZONE] [int] NOT NULL,
 CONSTRAINT [PK_MuCastle_DATA] PRIMARY KEY CLUSTERED 
(
	[MAP_SVR_GROUP] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[MuCastle_MONEY_STATISTICS]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[MuCastle_MONEY_STATISTICS](
	[MAP_SVR_GROUP] [int] NOT NULL,
	[LOG_DATE] [datetime] NOT NULL,
	[MONEY_CHANGE] [money] NOT NULL
) ON [PRIMARY]

GO
/****** Object:  Table [dbo].[MuCastle_NPC]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[MuCastle_NPC](
	[MAP_SVR_GROUP] [int] NOT NULL,
	[NPC_NUMBER] [int] NOT NULL,
	[NPC_INDEX] [int] NOT NULL,
	[NPC_DF_LEVEL] [int] NOT NULL,
	[NPC_RG_LEVEL] [int] NOT NULL,
	[NPC_MAXHP] [int] NOT NULL,
	[NPC_HP] [int] NOT NULL,
	[NPC_X] [tinyint] NOT NULL,
	[NPC_Y] [tinyint] NOT NULL,
	[NPC_DIR] [tinyint] NOT NULL,
	[NPC_CREATEDATE] [datetime] NOT NULL
) ON [PRIMARY]

GO
/****** Object:  Table [dbo].[MuCastle_REG_SIEGE]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[MuCastle_REG_SIEGE](
	[MAP_SVR_GROUP] [int] NOT NULL,
	[REG_SIEGE_GUILD] [varchar](8) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[REG_MARKS] [int] NOT NULL,
	[IS_GIVEUP] [tinyint] NOT NULL,
	[SEQ_NUM] [int] NOT NULL
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[MuCastle_SIEGE_GUILDLIST]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[MuCastle_SIEGE_GUILDLIST](
	[MAP_SVR_GROUP] [int] NOT NULL,
	[GUILD_NAME] [varchar](10) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[GUILD_ID] [int] NOT NULL,
	[GUILD_INVOLVED] [bit] NOT NULL,
	[GUILD_SCORE] [int] NOT NULL
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[MuCrywolf_DATA]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[MuCrywolf_DATA](
	[MAP_SVR_GROUP] [int] NOT NULL,
	[CRYWOLF_OCCUFY] [int] NOT NULL,
	[CRYWOLF_STATE] [int] NOT NULL,
	[CHAOSMIX_PLUS_RATE] [int] NOT NULL,
	[CHAOSMIX_MINUS_RATE] [int] NOT NULL,
 CONSTRAINT [PK_MuCrywolf_DATA] PRIMARY KEY CLUSTERED 
(
	[MAP_SVR_GROUP] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
/****** Object:  Table [dbo].[OptionData]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[OptionData](
	[Name] [varchar](10) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[SkillKey] [binary](20) NULL,
	[GameOption] [tinyint] NULL,
	[Qkey] [tinyint] NULL,
	[Wkey] [tinyint] NULL,
	[Ekey] [tinyint] NULL,
	[ChatWindow] [tinyint] NULL,
	[Rkey] [tinyint] NULL,
	[QWERLevel] [int] NULL,
	[EnableChangeMode] [tinyint] NULL,
 CONSTRAINT [PK_OptionData] PRIMARY KEY CLUSTERED 
(
	[Name] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[PetWarehouse]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[PetWarehouse](
	[AccountID] [varchar](12) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[Name] [varchar](12) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[Items] [varbinary](1984) NULL,
	[DbVersion] [tinyint] NULL,
	[SubEquip] [tinyint] NULL,
 CONSTRAINT [PK_PetWarehouser] PRIMARY KEY CLUSTERED 
(
	[Name] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[T_CGuid]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[T_CGuid](
	[GUID] [int] IDENTITY(1,1) NOT NULL,
	[Name] [varchar](10) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
 CONSTRAINT [PK_T_CGuid] PRIMARY KEY CLUSTERED 
(
	[GUID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[T_CurCharName]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[T_CurCharName](
	[Name] [char](10) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[cDate] [smalldatetime] NOT NULL
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[T_Event_Inventory]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[T_Event_Inventory](
	[AccountID] [varchar](10) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[Name] [varchar](10) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[Inventory] [varbinary](1024) NULL,
 CONSTRAINT [CL_PK_T_Event_Inventory] PRIMARY KEY CLUSTERED 
(
	[Name] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[T_FriendList]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[T_FriendList](
	[GUID] [int] NOT NULL,
	[FriendGuid] [int] NULL,
	[FriendName] [varchar](10) COLLATE SQL_Latin1_General_CP1_CI_AS NULL,
	[Del] [tinyint] NULL
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[T_FriendMail]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[T_FriendMail](
	[MemoIndex] [int] NOT NULL,
	[GUID] [int] NOT NULL,
	[FriendName] [varchar](10) COLLATE SQL_Latin1_General_CP1_CI_AS NULL,
	[wDate] [smalldatetime] NOT NULL,
	[Subject] [varchar](50) COLLATE SQL_Latin1_General_CP1_CI_AS NULL,
	[bRead] [bit] NOT NULL,
	[Memo] [varbinary](1000) NULL,
	[Photo] [binary](18) NULL,
	[Dir] [tinyint] NULL,
	[Act] [tinyint] NULL,
 CONSTRAINT [PK_T_FriendMemo] PRIMARY KEY CLUSTERED 
(
	[GUID] ASC,
	[MemoIndex] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[T_FriendMain]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[T_FriendMain](
	[GUID] [int] NOT NULL,
	[Name] [varchar](10) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[FriendCount] [tinyint] NULL,
	[MemoCount] [int] NULL,
	[MemoTotal] [int] NULL,
 CONSTRAINT [PK_T_FriendMain] PRIMARY KEY CLUSTERED 
(
	[GUID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[T_GMSystem]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[T_GMSystem](
	[Name] [varchar](10) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[AuthorityMask] [int] NOT NULL,
	[Expiry] [smalldatetime] NULL,
 CONSTRAINT [PK_T_GMSystem] PRIMARY KEY CLUSTERED 
(
	[Name] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[T_InGameShop_Items]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[T_InGameShop_Items](
	[AccountID] [varchar](10) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[UniqueCode] [int] IDENTITY(1,1) NOT NULL,
	[AuthCode] [int] NOT NULL,
	[UniqueID1] [int] NOT NULL,
	[UniqueID2] [int] NOT NULL,
	[UniqueID3] [int] NOT NULL,
	[InventoryType] [int] NOT NULL,
	[GiftName] [varchar](10) COLLATE SQL_Latin1_General_CP1_CI_AS NULL,
	[Message] [varchar](200) COLLATE SQL_Latin1_General_CP1_CI_AS NULL,
	[UsedItem] [int] NOT NULL
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[T_InGameShop_Point]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[T_InGameShop_Point](
	[AccountID] [varchar](10) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[WCoinP] [float] NOT NULL,
	[WCoinC] [float] NOT NULL,
	[GoblinPoint] [float] NOT NULL,
 CONSTRAINT [PK_T_InGameShop_Point] PRIMARY KEY CLUSTERED 
(
	[AccountID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[T_LUCKY_ITEM_INFO]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[T_LUCKY_ITEM_INFO](
	[nIndex] [int] IDENTITY(1,1) NOT NULL,
	[UserGuid] [int] NOT NULL,
	[CharName] [varchar](10) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[ItemCode] [int] NOT NULL,
	[ItemSerial] [bigint] NOT NULL,
	[DurabilitySmall] [smallint] NOT NULL,
 CONSTRAINT [PK_T_LUCKY_ITEM_INFO] PRIMARY KEY CLUSTERED 
(
	[CharName] ASC,
	[ItemCode] ASC,
	[ItemSerial] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[T_MineSystem]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[T_MineSystem](
	[CharacterName] [varchar](10) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[TwinkleType] [int] NOT NULL,
	[CurrentStage] [int] NOT NULL,
 CONSTRAINT [PK_T_MineSystem] PRIMARY KEY CLUSTERED 
(
	[CharacterName] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[T_MU2003_EVENT]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[T_MU2003_EVENT](
	[AccountID] [varchar](50) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[EventChips] [smallint] NOT NULL,
	[MuttoIndex] [int] NOT NULL,
	[MuttoNumber] [int] NOT NULL,
	[Check_Code] [char](1) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
 CONSTRAINT [PK_T_MU2003_EVENT_1] PRIMARY KEY CLUSTERED 
(
	[AccountID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[T_MuRummy]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[T_MuRummy](
	[AccountID] [varchar](10) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[Name] [varchar](10) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[mSequence] [int] NULL,
	[mNumber] [int] NULL,
	[mColor] [int] NULL,
	[mPosition] [int] NULL,
	[mStatus] [int] NULL
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[T_MuRummyInfo]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[T_MuRummyInfo](
	[AccountID] [varchar](10) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[Name] [varchar](10) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[mTotalScore] [int] NULL
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[T_MuRummyLog]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[T_MuRummyLog](
	[mDate] [datetime] NOT NULL,
	[AccountID] [varchar](10) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[Name] [varchar](10) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[mScore] [int] NOT NULL
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[T_PentagramInfo]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[T_PentagramInfo](
	[UserGuid] [int] NOT NULL,
	[AccountID] [varchar](10) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[Name] [varchar](10) COLLATE SQL_Latin1_General_CP1_CI_AS NULL,
	[JewelPos] [tinyint] NOT NULL,
	[JewelIndex] [smallint] NOT NULL,
	[ItemType] [smallint] NOT NULL,
	[ItemIndex] [int] NOT NULL,
	[MainAttribute] [smallint] NOT NULL,
	[JewelLevel] [smallint] NOT NULL,
	[Rank1] [tinyint] NOT NULL,
	[Rank1Level] [tinyint] NULL,
	[Rank2] [tinyint] NOT NULL,
	[Rank2Level] [tinyint] NULL,
	[Rank3] [tinyint] NOT NULL,
	[Rank3Level] [tinyint] NULL,
	[Rank4] [tinyint] NOT NULL,
	[Rank4Level] [tinyint] NULL,
	[Rank5] [tinyint] NOT NULL,
	[Rank5Level] [tinyint] NULL,
	[RegDate] [datetime] NULL
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[T_PetItem_Info]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[T_PetItem_Info](
	[ItemSerial] [bigint] NOT NULL,
	[Pet_Level] [smallint] NULL,
	[Pet_Exp] [bigint] NULL,
 CONSTRAINT [PK_T_Pet_Info] PRIMARY KEY CLUSTERED 
(
	[ItemSerial] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
/****** Object:  Table [dbo].[T_PSHOP_ITEMVALUE_INFO]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[T_PSHOP_ITEMVALUE_INFO](
	[AccountID] [varchar](10) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[Name] [varchar](10) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[ItemInvenNum] [int] NOT NULL,
	[ItemSerial] [bigint] NOT NULL,
	[Money] [int] NOT NULL,
	[BlessJewelValue] [int] NOT NULL,
	[SoulJewelValue] [int] NOT NULL,
	[ChaosJewelValue] [int] NOT NULL,
 CONSTRAINT [CL_PK_T_PSHOP_ITEMVALUE_INFO] PRIMARY KEY CLUSTERED 
(
	[Name] ASC,
	[ItemInvenNum] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[T_QUEST_EXP_INFO]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[T_QUEST_EXP_INFO](
	[nINDEX] [int] IDENTITY(1,1) NOT NULL,
	[CHAR_NAME] [varchar](10) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[EPISODE] [smallint] NOT NULL,
	[QUEST_SWITCH] [smallint] NOT NULL,
	[ASK_INDEX] [varbinary](5) NOT NULL,
	[ASK_VALUE] [varbinary](5) NOT NULL,
	[ASK_STATE] [varbinary](5) NOT NULL,
	[PROG_STATE] [smallint] NULL,
	[QUEST_START_DATE] [datetime] NULL,
	[QUEST_END_DATE] [datetime] NULL,
	[StartDateConvert] [bigint] NOT NULL,
	[EndDateConvert] [bigint] NOT NULL,
PRIMARY KEY CLUSTERED 
(
	[nINDEX] ASC,
	[CHAR_NAME] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[T_VIPList]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[T_VIPList](
	[AccountID] [varchar](10) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[Date] [smalldatetime] NULL,
	[Type] [tinyint] NULL,
 CONSTRAINT [PK_T_VIPList] PRIMARY KEY CLUSTERED 
(
	[AccountID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[T_WaitFriend]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[T_WaitFriend](
	[GUID] [int] NOT NULL,
	[FriendGuid] [int] NOT NULL,
	[FriendName] [varchar](10) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[warehouse]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[warehouse](
	[AccountID] [varchar](10) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[Items] [varbinary](7680) NULL,
	[Money] [int] NULL,
	[EndUseDate] [smalldatetime] NULL,
	[pw] [smallint] NULL,
	[Expanded] [tinyint] NOT NULL,
	[WHOpen] [tinyint] NOT NULL,
 CONSTRAINT [PK_warehouse] PRIMARY KEY CLUSTERED 
(
	[AccountID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  Table [dbo].[ZenEvent]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[ZenEvent](
	[Guid] [int] IDENTITY(1,1) NOT NULL,
	[AccountID] [varchar](10) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[Zen] [int] NULL,
 CONSTRAINT [PK_ZenEvent] PRIMARY KEY CLUSTERED 
(
	[Guid] ASC,
	[AccountID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
) ON [PRIMARY]

GO
SET ANSI_PADDING OFF
GO
/****** Object:  View [dbo].[vCharacterPreview]    Script Date: 11/03/2016 14:09:56 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO



	CREATE VIEW [dbo].[vCharacterPreview] AS SELECT Name, cLevel, Class, Inventory, CtlCode FROM dbo.Character















GO
SET ANSI_PADDING ON

GO
/****** Object:  Index [XI_MEMB_INFO_2]    Script Date: 11/03/2016 14:09:56 ******/
CREATE CLUSTERED INDEX [XI_MEMB_INFO_2] ON [dbo].[MEMB_INFO]
(
	[memb___id] DESC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, SORT_IN_TEMPDB = OFF, DROP_EXISTING = OFF, ONLINE = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON, FILLFACTOR = 70) ON [PRIMARY]
GO
/****** Object:  Index [IX_MuCastle_MONEY_STATISTICS]    Script Date: 11/03/2016 14:09:56 ******/
CREATE CLUSTERED INDEX [IX_MuCastle_MONEY_STATISTICS] ON [dbo].[MuCastle_MONEY_STATISTICS]
(
	[MAP_SVR_GROUP] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, SORT_IN_TEMPDB = OFF, DROP_EXISTING = OFF, ONLINE = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
GO
/****** Object:  Index [IX_NPC_PK]    Script Date: 11/03/2016 14:09:56 ******/
CREATE CLUSTERED INDEX [IX_NPC_PK] ON [dbo].[MuCastle_NPC]
(
	[MAP_SVR_GROUP] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, SORT_IN_TEMPDB = OFF, DROP_EXISTING = OFF, ONLINE = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
GO
/****** Object:  Index [IX_ATTACK_GUILD_KEY]    Script Date: 11/03/2016 14:09:56 ******/
CREATE CLUSTERED INDEX [IX_ATTACK_GUILD_KEY] ON [dbo].[MuCastle_REG_SIEGE]
(
	[MAP_SVR_GROUP] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, SORT_IN_TEMPDB = OFF, DROP_EXISTING = OFF, ONLINE = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
GO
/****** Object:  Index [IX_MuCastle_SIEGE_GUILDLIST]    Script Date: 11/03/2016 14:09:56 ******/
CREATE CLUSTERED INDEX [IX_MuCastle_SIEGE_GUILDLIST] ON [dbo].[MuCastle_SIEGE_GUILDLIST]
(
	[MAP_SVR_GROUP] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, SORT_IN_TEMPDB = OFF, DROP_EXISTING = OFF, ONLINE = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
GO
INSERT [dbo].[DefaultClassType] ([Class], [Strength], [Dexterity], [Vitality], [Energy], [MagicList], [Life], [MaxLife], [Mana], [MaxMana], [MapNumber], [MapPosX], [MapPosY], [Quest], [DbVersion], [Leadership], [Level], [LevelUpPoint], [Inventory], [LevelLife], [LevelMana], [VitalityToLife], [EnergyToMana]) VALUES (0, 18, 18, 15, 30, 0xFF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000, 60, 60, 60, 60, 0, 182, 128, 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000, 3, 0, 1, 0, 0x0000140000000000005000FFFFFFFFFF00000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF, 1, 2, 1, 2)
GO
INSERT [dbo].[DefaultClassType] ([Class], [Strength], [Dexterity], [Vitality], [Energy], [MagicList], [Life], [MaxLife], [Mana], [MaxMana], [MapNumber], [MapPosX], [MapPosY], [Quest], [DbVersion], [Leadership], [Level], [LevelUpPoint], [Inventory], [LevelLife], [LevelMana], [VitalityToLife], [EnergyToMana]) VALUES (16, 28, 20, 25, 10, 0xFF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000, 110, 110, 20, 20, 0, 182, 128, 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000, 3, 0, 1, 0, 0x0000120000000000001000FFFFFFFFFF00000000FFFFFFFFFFFFFFFFFFFFFFFF0000160000000000006000FFFFFFFFFF00000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF, 2, 0.5, 3, 1)
GO
INSERT [dbo].[DefaultClassType] ([Class], [Strength], [Dexterity], [Vitality], [Energy], [MagicList], [Life], [MaxLife], [Mana], [MaxMana], [MapNumber], [MapPosX], [MapPosY], [Quest], [DbVersion], [Leadership], [Level], [LevelUpPoint], [Inventory], [LevelLife], [LevelMana], [VitalityToLife], [EnergyToMana]) VALUES (32, 22, 25, 20, 15, 0xFF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000, 80, 80, 30, 30, 0, 182, 128, 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000, 3, 0, 1, 0, 0x0F00FF0000000000004000FFFFFFFFFF00000000FFFFFFFFFFFFFFFFFFFFFFFF0000140000000000004000FFFFFFFFFF00000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF, 1, 1.5, 2, 1.5)
GO
INSERT [dbo].[DefaultClassType] ([Class], [Strength], [Dexterity], [Vitality], [Energy], [MagicList], [Life], [MaxLife], [Mana], [MaxMana], [MapNumber], [MapPosX], [MapPosY], [Quest], [DbVersion], [Leadership], [Level], [LevelUpPoint], [Inventory], [LevelLife], [LevelMana], [VitalityToLife], [EnergyToMana]) VALUES (48, 26, 26, 26, 26, 0xFF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000, 110, 110, 60, 60, 0, 182, 128, 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000, 3, 0, 1, 0, 0x0100160000000000000000FFFFFFFFFF00000000FFFFFFFFFFFFFFFFFFFFFFFF0000160000000000006000FFFFFFFFFF00000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF, 1, 1, 2, 2)
GO
INSERT [dbo].[DefaultClassType] ([Class], [Strength], [Dexterity], [Vitality], [Energy], [MagicList], [Life], [MaxLife], [Mana], [MaxMana], [MapNumber], [MapPosX], [MapPosY], [Quest], [DbVersion], [Leadership], [Level], [LevelUpPoint], [Inventory], [LevelLife], [LevelMana], [VitalityToLife], [EnergyToMana]) VALUES (64, 26, 20, 20, 15, 0xFF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000, 25, 25, 90, 90, 0, 182, 128, 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000, 3, 24, 1, 0, 0x0100160000000000000000FFFFFFFFFF00000000FFFFFFFFFFFFFFFFFFFFFFFF0000160000000000006000FFFFFFFFFF00000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF, 1.5, 1, 2, 1.5)
GO
INSERT [dbo].[DefaultClassType] ([Class], [Strength], [Dexterity], [Vitality], [Energy], [MagicList], [Life], [MaxLife], [Mana], [MaxMana], [MapNumber], [MapPosX], [MapPosY], [Quest], [DbVersion], [Leadership], [Level], [LevelUpPoint], [Inventory], [LevelLife], [LevelMana], [VitalityToLife], [EnergyToMana]) VALUES (80, 21, 21, 18, 23, 0xFF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000, 70, 70, 40, 40, 0, 182, 128, 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000, 3, 0, 1, 0, 0x0000120000000000001000FFFFFFFFFF00000000FFFFFFFFFFFFFFFFFFFFFFFF0000160000000000006000FFFFFFFFFF00000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF, 1, 1.5, 2, 1.5)
GO
INSERT [dbo].[DefaultClassType] ([Class], [Strength], [Dexterity], [Vitality], [Energy], [MagicList], [Life], [MaxLife], [Mana], [MaxMana], [MapNumber], [MapPosX], [MapPosY], [Quest], [DbVersion], [Leadership], [Level], [LevelUpPoint], [Inventory], [LevelLife], [LevelMana], [VitalityToLife], [EnergyToMana]) VALUES (96, 32, 27, 25, 20, 0xFF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000, 100, 100, 40, 40, 0, 182, 128, 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000, 3, 0, 1, 0, 0x0000120000000000001000FFFFFFFFFF00000000FFFFFFFFFFFFFFFFFFFFFFFF0000160000000000006000FFFFFFFFFF00000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF, 1.3, 1, 2, 1.3)
GO
INSERT [dbo].[DefaultClassType] ([Class], [Strength], [Dexterity], [Vitality], [Energy], [MagicList], [Life], [MaxLife], [Mana], [MaxMana], [MapNumber], [MapPosX], [MapPosY], [Quest], [DbVersion], [Leadership], [Level], [LevelUpPoint], [Inventory], [LevelLife], [LevelMana], [VitalityToLife], [EnergyToMana]) VALUES (112, 30, 30, 25, 24, 0xFF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000, 110, 110, 40, 40, 0, 182, 128, 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000, 3, 0, 1, 0, 0x0000120000000000001000FFFFFFFFFF00000000FFFFFFFFFFFFFFFFFFFFFFFF0000160000000000006000FFFFFFFFFF00000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF, 2, 1, 2, 1)
GO
INSERT [dbo].[GameServerInfo] ([Number], [ItemCount], [ZenCount], [GmItemCount], [AceItemCount], [GensRankingMonth]) VALUES (0, 0, 0, 0, 0, NULL)
GO
INSERT [dbo].[Mu_DBID] ([DESC], [Version]) VALUES (N'3', 3)
GO
SET ANSI_PADDING ON

GO
/****** Object:  Index [IX_C_Monster_KillCount]    Script Date: 11/03/2016 14:09:56 ******/
ALTER TABLE [dbo].[C_Monster_KillCount] ADD  CONSTRAINT [IX_C_Monster_KillCount] UNIQUE NONCLUSTERED 
(
	[name] ASC,
	[MonsterId] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, SORT_IN_TEMPDB = OFF, IGNORE_DUP_KEY = OFF, ONLINE = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
GO
SET ANSI_PADDING ON

GO
/****** Object:  Index [PK_Character]    Script Date: 11/03/2016 14:09:56 ******/
ALTER TABLE [dbo].[Character] ADD  CONSTRAINT [PK_Character] PRIMARY KEY NONCLUSTERED 
(
	[Name] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, SORT_IN_TEMPDB = OFF, IGNORE_DUP_KEY = OFF, ONLINE = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
GO
/****** Object:  Index [PK_GameServerInfo]    Script Date: 11/03/2016 14:09:56 ******/
ALTER TABLE [dbo].[GameServerInfo] ADD  CONSTRAINT [PK_GameServerInfo] PRIMARY KEY NONCLUSTERED 
(
	[Number] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, SORT_IN_TEMPDB = OFF, IGNORE_DUP_KEY = OFF, ONLINE = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
GO
/****** Object:  Index [IDX_GUILD_G_RIVAL]    Script Date: 11/03/2016 14:09:56 ******/
CREATE NONCLUSTERED INDEX [IDX_GUILD_G_RIVAL] ON [dbo].[Guild]
(
	[G_Rival] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, SORT_IN_TEMPDB = OFF, DROP_EXISTING = OFF, ONLINE = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
GO
/****** Object:  Index [IDX_GUILD_G_UNION]    Script Date: 11/03/2016 14:09:56 ******/
CREATE NONCLUSTERED INDEX [IDX_GUILD_G_UNION] ON [dbo].[Guild]
(
	[G_Union] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, SORT_IN_TEMPDB = OFF, DROP_EXISTING = OFF, ONLINE = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
GO
/****** Object:  Index [IDX_GUILD_NUMBER]    Script Date: 11/03/2016 14:09:56 ******/
CREATE NONCLUSTERED INDEX [IDX_GUILD_NUMBER] ON [dbo].[Guild]
(
	[Number] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, SORT_IN_TEMPDB = OFF, DROP_EXISTING = OFF, ONLINE = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
GO
SET ANSI_PADDING ON

GO
/****** Object:  Index [IX_IGC_GensAbuse]    Script Date: 11/03/2016 14:09:56 ******/
ALTER TABLE [dbo].[IGC_GensAbuse] ADD  CONSTRAINT [IX_IGC_GensAbuse] UNIQUE NONCLUSTERED 
(
	[Name] ASC,
	[KillName] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, SORT_IN_TEMPDB = OFF, IGNORE_DUP_KEY = OFF, ONLINE = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
GO
/****** Object:  Index [PK_IGC_GuildMatching]    Script Date: 11/03/2016 14:09:56 ******/
ALTER TABLE [dbo].[IGC_GuildMatching] ADD  CONSTRAINT [PK_IGC_GuildMatching] PRIMARY KEY NONCLUSTERED 
(
	[identNo] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, SORT_IN_TEMPDB = OFF, IGNORE_DUP_KEY = OFF, ONLINE = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
GO
/****** Object:  Index [IX_IGC_PeriodItemInfo]    Script Date: 11/03/2016 14:09:56 ******/
ALTER TABLE [dbo].[IGC_PeriodItemInfo] ADD  CONSTRAINT [IX_IGC_PeriodItemInfo] UNIQUE NONCLUSTERED 
(
	[Serial] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, SORT_IN_TEMPDB = OFF, IGNORE_DUP_KEY = OFF, ONLINE = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
GO
/****** Object:  Index [PK_IGC_WaitGuildMatching]    Script Date: 11/03/2016 14:09:56 ******/
ALTER TABLE [dbo].[IGC_WaitGuildMatching] ADD  CONSTRAINT [PK_IGC_WaitGuildMatching] PRIMARY KEY NONCLUSTERED 
(
	[identNo] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, SORT_IN_TEMPDB = OFF, IGNORE_DUP_KEY = OFF, ONLINE = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
GO
/****** Object:  Index [PK_MEMB_INFO_1]    Script Date: 11/03/2016 14:09:56 ******/
ALTER TABLE [dbo].[MEMB_INFO] ADD  CONSTRAINT [PK_MEMB_INFO_1] PRIMARY KEY NONCLUSTERED 
(
	[memb_guid] DESC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, SORT_IN_TEMPDB = OFF, IGNORE_DUP_KEY = OFF, ONLINE = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
GO
SET ANSI_PADDING ON

GO
/****** Object:  Index [IX_MEMB_INFO_1]    Script Date: 11/03/2016 14:09:56 ******/
ALTER TABLE [dbo].[MEMB_INFO] ADD  CONSTRAINT [IX_MEMB_INFO_1] UNIQUE NONCLUSTERED 
(
	[memb___id] DESC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, SORT_IN_TEMPDB = OFF, IGNORE_DUP_KEY = OFF, ONLINE = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
GO
SET ANSI_PADDING ON

GO
/****** Object:  Index [IX_MEMB_DETAIL]    Script Date: 11/03/2016 14:09:56 ******/
CREATE NONCLUSTERED INDEX [IX_MEMB_DETAIL] ON [dbo].[MEMB_INFO]
(
	[sno__numb] DESC,
	[memb_name] DESC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, SORT_IN_TEMPDB = OFF, DROP_EXISTING = OFF, ONLINE = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
GO
/****** Object:  Index [IX_MuCastle_MONEY_STATISTICS_NC]    Script Date: 11/03/2016 14:09:56 ******/
CREATE NONCLUSTERED INDEX [IX_MuCastle_MONEY_STATISTICS_NC] ON [dbo].[MuCastle_MONEY_STATISTICS]
(
	[MAP_SVR_GROUP] ASC,
	[LOG_DATE] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, SORT_IN_TEMPDB = OFF, DROP_EXISTING = OFF, ONLINE = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
GO
/****** Object:  Index [IX_NPC_SUBKEY]    Script Date: 11/03/2016 14:09:56 ******/
ALTER TABLE [dbo].[MuCastle_NPC] ADD  CONSTRAINT [IX_NPC_SUBKEY] UNIQUE NONCLUSTERED 
(
	[MAP_SVR_GROUP] ASC,
	[NPC_NUMBER] ASC,
	[NPC_INDEX] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, SORT_IN_TEMPDB = OFF, IGNORE_DUP_KEY = OFF, ONLINE = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
GO
SET ANSI_PADDING ON

GO
/****** Object:  Index [IX_ATTACK_GUILD_SUBKEY]    Script Date: 11/03/2016 14:09:56 ******/
ALTER TABLE [dbo].[MuCastle_REG_SIEGE] ADD  CONSTRAINT [IX_ATTACK_GUILD_SUBKEY] UNIQUE NONCLUSTERED 
(
	[MAP_SVR_GROUP] ASC,
	[REG_SIEGE_GUILD] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, SORT_IN_TEMPDB = OFF, IGNORE_DUP_KEY = OFF, ONLINE = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
GO
SET ANSI_PADDING ON

GO
/****** Object:  Index [IX_T_CGuid_Name]    Script Date: 11/03/2016 14:09:56 ******/
CREATE NONCLUSTERED INDEX [IX_T_CGuid_Name] ON [dbo].[T_CGuid]
(
	[Name] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, SORT_IN_TEMPDB = OFF, DROP_EXISTING = OFF, ONLINE = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
GO
/****** Object:  Index [IX_T_FriendList]    Script Date: 11/03/2016 14:09:56 ******/
CREATE NONCLUSTERED INDEX [IX_T_FriendList] ON [dbo].[T_FriendList]
(
	[GUID] ASC,
	[FriendGuid] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, SORT_IN_TEMPDB = OFF, DROP_EXISTING = OFF, ONLINE = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
GO
/****** Object:  Index [IX_T_WaitFriend]    Script Date: 11/03/2016 14:09:56 ******/
CREATE NONCLUSTERED INDEX [IX_T_WaitFriend] ON [dbo].[T_WaitFriend]
(
	[GUID] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, SORT_IN_TEMPDB = OFF, DROP_EXISTING = OFF, ONLINE = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
GO
ALTER TABLE [dbo].[AccountCharacter] ADD  CONSTRAINT [DF__AccountCh__MoveC__7A3223E8]  DEFAULT ((0)) FOR [MoveCnt]
GO
ALTER TABLE [dbo].[AccountCharacter] ADD  CONSTRAINT [DF_AccountCharacter_Summoner]  DEFAULT ((0)) FOR [Summoner]
GO
ALTER TABLE [dbo].[AccountCharacter] ADD  CONSTRAINT [DF_AccountCharacter_WarehouseExpansion]  DEFAULT ((0)) FOR [WarehouseExpansion]
GO
ALTER TABLE [dbo].[AccountCharacter] ADD  CONSTRAINT [DF_AccountCharacter_RageFighter]  DEFAULT ((0)) FOR [RageFighter]
GO
ALTER TABLE [dbo].[AccountCharacter] ADD  DEFAULT ((0)) FOR [SecCode]
GO
ALTER TABLE [dbo].[Character] ADD  CONSTRAINT [DF_Character_cLevel]  DEFAULT ((1)) FOR [cLevel]
GO
ALTER TABLE [dbo].[Character] ADD  CONSTRAINT [DF_Character_LevelUpPoint]  DEFAULT ((0)) FOR [LevelUpPoint]
GO
ALTER TABLE [dbo].[Character] ADD  CONSTRAINT [DF_Character_Experience]  DEFAULT ((0)) FOR [Experience]
GO
ALTER TABLE [dbo].[Character] ADD  CONSTRAINT [DF_Character_Money]  DEFAULT ((0)) FOR [Money]
GO
ALTER TABLE [dbo].[Character] ADD  CONSTRAINT [DF_Character_MapDir]  DEFAULT ((0)) FOR [MapDir]
GO
ALTER TABLE [dbo].[Character] ADD  CONSTRAINT [DF_Character_PkCount]  DEFAULT ((0)) FOR [PkCount]
GO
ALTER TABLE [dbo].[Character] ADD  CONSTRAINT [DF_Character_PkLevel]  DEFAULT ((3)) FOR [PkLevel]
GO
ALTER TABLE [dbo].[Character] ADD  CONSTRAINT [DF_Character_PkTime]  DEFAULT ((0)) FOR [PkTime]
GO
ALTER TABLE [dbo].[Character] ADD  CONSTRAINT [DF_Character_CtlCode]  DEFAULT ((0)) FOR [CtlCode]
GO
ALTER TABLE [dbo].[Character] ADD  CONSTRAINT [DF__Character__Quest__40F9A68C]  DEFAULT ((0)) FOR [Quest]
GO
ALTER TABLE [dbo].[Character] ADD  DEFAULT ((0)) FOR [ChatLimitTime]
GO
ALTER TABLE [dbo].[Character] ADD  DEFAULT ((0)) FOR [FruitPoint]
GO
ALTER TABLE [dbo].[Character] ADD  CONSTRAINT [DF_Character_RESETS]  DEFAULT ((0)) FOR [RESETS]
GO
ALTER TABLE [dbo].[Character] ADD  CONSTRAINT [DF_Character_Married]  DEFAULT ((0)) FOR [Married]
GO
ALTER TABLE [dbo].[Character] ADD  DEFAULT ((0)) FOR [mLevel]
GO
ALTER TABLE [dbo].[Character] ADD  DEFAULT ((0)) FOR [mlPoint]
GO
ALTER TABLE [dbo].[Character] ADD  DEFAULT ((0)) FOR [mlExperience]
GO
ALTER TABLE [dbo].[Character] ADD  DEFAULT ((0)) FOR [mlNextExp]
GO
ALTER TABLE [dbo].[Character] ADD  CONSTRAINT [DF_Character_InventoryExpansion]  DEFAULT ((0)) FOR [InventoryExpansion]
GO
ALTER TABLE [dbo].[Character] ADD  CONSTRAINT [DF_Character_WinDuels]  DEFAULT ((0)) FOR [WinDuels]
GO
ALTER TABLE [dbo].[Character] ADD  CONSTRAINT [DF_Character_LoseDuels]  DEFAULT ((0)) FOR [LoseDuels]
GO
ALTER TABLE [dbo].[Character] ADD  DEFAULT ((0)) FOR [PenaltyMask]
GO
ALTER TABLE [dbo].[Character] ADD  DEFAULT ((0)) FOR [BlockChatTime]
GO
ALTER TABLE [dbo].[DefaultClassType] ADD  CONSTRAINT [DF__DefaultCl__Leade__719CDDE7]  DEFAULT ((0)) FOR [Leadership]
GO
ALTER TABLE [dbo].[DefaultClassType] ADD  CONSTRAINT [DF__DefaultCl__Level__72910220]  DEFAULT ((0)) FOR [Level]
GO
ALTER TABLE [dbo].[DefaultClassType] ADD  CONSTRAINT [DF__DefaultCl__Level__73852659]  DEFAULT ((0)) FOR [LevelUpPoint]
GO
ALTER TABLE [dbo].[GameServerInfo] ADD  CONSTRAINT [DF_GameServerInfo_Number]  DEFAULT ((0)) FOR [Number]
GO
ALTER TABLE [dbo].[GameServerInfo] ADD  CONSTRAINT [DF_GameServerInfo_ZenCount]  DEFAULT ((0)) FOR [ZenCount]
GO
ALTER TABLE [dbo].[GameServerInfo] ADD  DEFAULT ((1)) FOR [GensRankingMonth]
GO
ALTER TABLE [dbo].[Guild] ADD  DEFAULT ((0)) FOR [G_Score]
GO
ALTER TABLE [dbo].[Guild] ADD  CONSTRAINT [DF__Guild__G_Type__7EF6D905]  DEFAULT ((0)) FOR [G_Type]
GO
ALTER TABLE [dbo].[Guild] ADD  CONSTRAINT [DF__Guild__G_Rival__7FEAFD3E]  DEFAULT ((0)) FOR [G_Rival]
GO
ALTER TABLE [dbo].[Guild] ADD  CONSTRAINT [DF__Guild__G_Union__00DF2177]  DEFAULT ((0)) FOR [G_Union]
GO
ALTER TABLE [dbo].[GuildMember] ADD  CONSTRAINT [DF__GuildMemb__G_Sta__01D345B0]  DEFAULT ((0)) FOR [G_Status]
GO
ALTER TABLE [dbo].[IGC_Gens] ADD  CONSTRAINT [DF_IGC_Gens_Rank]  DEFAULT ((0)) FOR [Rank]
GO
ALTER TABLE [dbo].[IGC_Gens] ADD  CONSTRAINT [DF_IGC_Gens_Points]  DEFAULT ((0)) FOR [Points]
GO
ALTER TABLE [dbo].[IGC_Gens] ADD  CONSTRAINT [DF_IGC_Gens_Reward]  DEFAULT ((1)) FOR [Reward]
GO
ALTER TABLE [dbo].[IGC_Gens] ADD  CONSTRAINT [DF_IGC_Gens_Class]  DEFAULT ((0)) FOR [Class]
GO
ALTER TABLE [dbo].[IGC_GuildMatching] ADD  CONSTRAINT [DF_IGC_GuildMatching_InterestType]  DEFAULT ((0)) FOR [InterestType]
GO
ALTER TABLE [dbo].[IGC_GuildMatching] ADD  CONSTRAINT [DF_IGC_GuildMatching_LevelRange]  DEFAULT ((0)) FOR [LevelRange]
GO
ALTER TABLE [dbo].[IGC_GuildMatching] ADD  CONSTRAINT [DF_IGC_GuildMatching_ClassType]  DEFAULT ((0)) FOR [ClassType]
GO
ALTER TABLE [dbo].[IGC_PartyMatching] ADD  CONSTRAINT [DF_IGC_PartyMatching_UsePassWord]  DEFAULT ((0)) FOR [UsePassWord]
GO
ALTER TABLE [dbo].[IGC_PeriodItemInfo] ADD  DEFAULT ((0)) FOR [SetExpire]
GO
ALTER TABLE [dbo].[MEMB_CREDITS] ADD  CONSTRAINT [DF_MEMB_CREDITS_credits]  DEFAULT ((0)) FOR [credits]
GO
ALTER TABLE [dbo].[MEMB_INFO] ADD  CONSTRAINT [DF_MEMB_INFO_mail_chek]  DEFAULT ((0)) FOR [mail_chek]
GO
ALTER TABLE [dbo].[Mu_DBID] ADD  DEFAULT ((1)) FOR [Version]
GO
ALTER TABLE [dbo].[MuCastle_DATA] ADD  CONSTRAINT [DF_MuCastle_Data_SEIGE_ENDED]  DEFAULT ((0)) FOR [SIEGE_ENDED]
GO
ALTER TABLE [dbo].[MuCastle_DATA] ADD  CONSTRAINT [DF_MuCastle_Data_CASTLE_OCCUPY]  DEFAULT ((0)) FOR [CASTLE_OCCUPY]
GO
ALTER TABLE [dbo].[MuCastle_DATA] ADD  CONSTRAINT [DF_MuCastle_Data_MONEY]  DEFAULT ((0)) FOR [MONEY]
GO
ALTER TABLE [dbo].[MuCastle_DATA] ADD  CONSTRAINT [DF_MuCastle_Data_TAX_RATE]  DEFAULT ((0)) FOR [TAX_RATE_CHAOS]
GO
ALTER TABLE [dbo].[MuCastle_DATA] ADD  CONSTRAINT [DF_MuCastle_DATA_TAX_RATE_STORE]  DEFAULT ((0)) FOR [TAX_RATE_STORE]
GO
ALTER TABLE [dbo].[MuCastle_DATA] ADD  CONSTRAINT [DF_MuCastle_DATA_TAX_HUNT_ZONE]  DEFAULT ((0)) FOR [TAX_HUNT_ZONE]
GO
ALTER TABLE [dbo].[MuCastle_SIEGE_GUILDLIST] ADD  CONSTRAINT [DF_MuCastle_SIEGE_GUILDLIST_GUILD_SCORE]  DEFAULT ((0)) FOR [GUILD_SCORE]
GO
ALTER TABLE [dbo].[MuCrywolf_DATA] ADD  CONSTRAINT [DF_MuCrywolf_DATA_CRYWOLF_OCCUFY]  DEFAULT ((0)) FOR [CRYWOLF_OCCUFY]
GO
ALTER TABLE [dbo].[MuCrywolf_DATA] ADD  CONSTRAINT [DF_MuCrywolf_DATA_CRYWOLF_STATE]  DEFAULT ((0)) FOR [CRYWOLF_STATE]
GO
ALTER TABLE [dbo].[MuCrywolf_DATA] ADD  CONSTRAINT [DF_MuCrywolf_DATA_CHAOSMIX_PLUS_RATE]  DEFAULT ((0)) FOR [CHAOSMIX_PLUS_RATE]
GO
ALTER TABLE [dbo].[MuCrywolf_DATA] ADD  CONSTRAINT [DF_MuCrywolf_DATA_CHAOSMIX_MINUS_RATE]  DEFAULT ((0)) FOR [CHAOSMIX_MINUS_RATE]
GO
ALTER TABLE [dbo].[OptionData] ADD  CONSTRAINT [DF__OptionDat__ChatW__4A8310C6]  DEFAULT ((255)) FOR [ChatWindow]
GO
ALTER TABLE [dbo].[OptionData] ADD  DEFAULT ((0)) FOR [EnableChangeMode]
GO
ALTER TABLE [dbo].[T_CurCharName] ADD  CONSTRAINT [DF__T_CurChar__cDate__6BE40491]  DEFAULT (getdate()) FOR [cDate]
GO
ALTER TABLE [dbo].[T_FriendList] ADD  CONSTRAINT [DF_T_FriendList_Del]  DEFAULT ((0)) FOR [Del]
GO
ALTER TABLE [dbo].[T_FriendMail] ADD  CONSTRAINT [DF_T_FriendMemo_MemoIndex]  DEFAULT ((10)) FOR [MemoIndex]
GO
ALTER TABLE [dbo].[T_FriendMail] ADD  CONSTRAINT [DF_T_FriendMemo_wDate]  DEFAULT (getdate()) FOR [wDate]
GO
ALTER TABLE [dbo].[T_FriendMail] ADD  CONSTRAINT [DF_T_FriendMemo_MemoRead]  DEFAULT ((0)) FOR [bRead]
GO
ALTER TABLE [dbo].[T_FriendMail] ADD  CONSTRAINT [DF_T_FriendMemo_Dir]  DEFAULT ((0)) FOR [Dir]
GO
ALTER TABLE [dbo].[T_FriendMail] ADD  CONSTRAINT [DF_T_FriendMemo_Action]  DEFAULT ((0)) FOR [Act]
GO
ALTER TABLE [dbo].[T_FriendMain] ADD  CONSTRAINT [DF_T_FriendMain_MemoCount]  DEFAULT ((10)) FOR [MemoCount]
GO
ALTER TABLE [dbo].[T_FriendMain] ADD  CONSTRAINT [DF_T_FriendMain_MemoTotal]  DEFAULT ((0)) FOR [MemoTotal]
GO
ALTER TABLE [dbo].[T_GMSystem] ADD  CONSTRAINT [DF_T_GMSystem_AuthorityMask]  DEFAULT ((0)) FOR [AuthorityMask]
GO
ALTER TABLE [dbo].[T_InGameShop_Items] ADD  CONSTRAINT [DF_T_InGameShop_Items_UsedItem]  DEFAULT ((0)) FOR [UsedItem]
GO
ALTER TABLE [dbo].[T_InGameShop_Point] ADD  CONSTRAINT [DF_T_InGameShop_Point_WCoinP]  DEFAULT ((0.00)) FOR [WCoinP]
GO
ALTER TABLE [dbo].[T_InGameShop_Point] ADD  CONSTRAINT [DF_T_InGameShop_Point_WCoinC]  DEFAULT ((0.00)) FOR [WCoinC]
GO
ALTER TABLE [dbo].[T_InGameShop_Point] ADD  CONSTRAINT [DF_T_InGameShop_Point_GoblinPoint]  DEFAULT ((0.00)) FOR [GoblinPoint]
GO
ALTER TABLE [dbo].[T_MU2003_EVENT] ADD  CONSTRAINT [DF_T_MU2003_EVENT_EventChips_1]  DEFAULT ((0)) FOR [EventChips]
GO
ALTER TABLE [dbo].[T_MU2003_EVENT] ADD  CONSTRAINT [DF_T_MU2003_EVENT_MuttoIndex_1]  DEFAULT ((-1)) FOR [MuttoIndex]
GO
ALTER TABLE [dbo].[T_MU2003_EVENT] ADD  CONSTRAINT [DF_T_MU2003_EVENT_MuttoNumber_1]  DEFAULT ((0)) FOR [MuttoNumber]
GO
ALTER TABLE [dbo].[T_MU2003_EVENT] ADD  CONSTRAINT [DF_T_MU2003_EVENT_Check_Code]  DEFAULT ((0)) FOR [Check_Code]
GO
ALTER TABLE [dbo].[T_PentagramInfo] ADD  CONSTRAINT [DF_T_PentagramInfo_JewelIndex]  DEFAULT ((255)) FOR [JewelIndex]
GO
ALTER TABLE [dbo].[T_PentagramInfo] ADD  CONSTRAINT [DF_T_PentagramInfo_ItemType]  DEFAULT ((255)) FOR [ItemType]
GO
ALTER TABLE [dbo].[T_PentagramInfo] ADD  CONSTRAINT [DF_T_PentagramInfo_ItemIndex]  DEFAULT ((511)) FOR [ItemIndex]
GO
ALTER TABLE [dbo].[T_PentagramInfo] ADD  CONSTRAINT [DF_T_PentagramInfo_MainAttribute]  DEFAULT ((255)) FOR [MainAttribute]
GO
ALTER TABLE [dbo].[T_PentagramInfo] ADD  CONSTRAINT [DF_T_PentagramInfo_Rank1]  DEFAULT ((255)) FOR [Rank1]
GO
ALTER TABLE [dbo].[T_PentagramInfo] ADD  CONSTRAINT [DF_T_PentagramInfo_Rank2]  DEFAULT ((255)) FOR [Rank2]
GO
ALTER TABLE [dbo].[T_PentagramInfo] ADD  CONSTRAINT [DF_T_PentagramInfo_Rank3]  DEFAULT ((255)) FOR [Rank3]
GO
ALTER TABLE [dbo].[T_PentagramInfo] ADD  CONSTRAINT [DF_T_PentagramInfo_Rank4]  DEFAULT ((255)) FOR [Rank4]
GO
ALTER TABLE [dbo].[T_PentagramInfo] ADD  CONSTRAINT [DF_T_PentagramInfo_Rank5]  DEFAULT ((255)) FOR [Rank5]
GO
ALTER TABLE [dbo].[T_PentagramInfo] ADD  CONSTRAINT [DF_T_PentagramInfo_RegDate]  DEFAULT (getdate()) FOR [RegDate]
GO
ALTER TABLE [dbo].[T_PetItem_Info] ADD  CONSTRAINT [DF_T_Pet_Info_Pet_Level]  DEFAULT ((0)) FOR [Pet_Level]
GO
ALTER TABLE [dbo].[T_PetItem_Info] ADD  CONSTRAINT [DF_T_Pet_Info_Pet_Exp]  DEFAULT ((0)) FOR [Pet_Exp]
GO
ALTER TABLE [dbo].[T_PSHOP_ITEMVALUE_INFO] ADD  DEFAULT ((0)) FOR [ItemInvenNum]
GO
ALTER TABLE [dbo].[T_PSHOP_ITEMVALUE_INFO] ADD  DEFAULT ((0)) FOR [ItemSerial]
GO
ALTER TABLE [dbo].[T_PSHOP_ITEMVALUE_INFO] ADD  DEFAULT ((0)) FOR [Money]
GO
ALTER TABLE [dbo].[T_PSHOP_ITEMVALUE_INFO] ADD  DEFAULT ((0)) FOR [BlessJewelValue]
GO
ALTER TABLE [dbo].[T_PSHOP_ITEMVALUE_INFO] ADD  DEFAULT ((0)) FOR [SoulJewelValue]
GO
ALTER TABLE [dbo].[T_PSHOP_ITEMVALUE_INFO] ADD  DEFAULT ((0)) FOR [ChaosJewelValue]
GO
ALTER TABLE [dbo].[T_QUEST_EXP_INFO] ADD  DEFAULT ((0)) FOR [ASK_INDEX]
GO
ALTER TABLE [dbo].[T_QUEST_EXP_INFO] ADD  DEFAULT ((0)) FOR [ASK_VALUE]
GO
ALTER TABLE [dbo].[T_QUEST_EXP_INFO] ADD  DEFAULT ((0)) FOR [ASK_STATE]
GO
ALTER TABLE [dbo].[T_QUEST_EXP_INFO] ADD  DEFAULT ((0)) FOR [QUEST_START_DATE]
GO
ALTER TABLE [dbo].[T_QUEST_EXP_INFO] ADD  DEFAULT ((0)) FOR [QUEST_END_DATE]
GO
ALTER TABLE [dbo].[T_QUEST_EXP_INFO] ADD  CONSTRAINT [DF_T_QUEST_EXP_INFO_StartDateConvert]  DEFAULT ((0)) FOR [StartDateConvert]
GO
ALTER TABLE [dbo].[T_QUEST_EXP_INFO] ADD  CONSTRAINT [DF_T_QUEST_EXP_INFO_EndDateConvert]  DEFAULT ((0)) FOR [EndDateConvert]
GO
ALTER TABLE [dbo].[warehouse] ADD  CONSTRAINT [DF_warehouse_Money]  DEFAULT ((0)) FOR [Money]
GO
ALTER TABLE [dbo].[warehouse] ADD  CONSTRAINT [DF__warehouse__pw__40058253]  DEFAULT ((0)) FOR [pw]
GO
ALTER TABLE [dbo].[warehouse] ADD  CONSTRAINT [DF_warehouse_Expansion]  DEFAULT ((0)) FOR [Expanded]
GO
ALTER TABLE [dbo].[warehouse] ADD  DEFAULT ((0)) FOR [WHOpen]
GO
ALTER TABLE [dbo].[ZenEvent] ADD  CONSTRAINT [DF_ZenEvent_Zen]  DEFAULT ((0)) FOR [Zen]
GO
ALTER TABLE [dbo].[GuildMember]  WITH CHECK ADD  CONSTRAINT [FK_GuildMember_Guild] FOREIGN KEY([G_Name])
REFERENCES [dbo].[Guild] ([G_Name])
ON UPDATE CASCADE
GO
ALTER TABLE [dbo].[GuildMember] CHECK CONSTRAINT [FK_GuildMember_Guild]
GO

print ('Restored database is configured to support MD5 password encryption. Refer to MD5 Installation Instruction due to complete the process.')