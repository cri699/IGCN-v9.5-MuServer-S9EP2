-- /////////////////////////////////////////////////////////////////////////////////////////////////
-- International Gaming Center, copyright 2010-2015 -- www.igc-network.com
--
-- Script allows to enable creation of Summoner or/and RageFighter without
-- a need to purchase Character Cards from Cash Shop
-- /////////////////////////////////////////////////////////////////////////////////////////////////

USE [MuOnline] -- set a current MuOnline database name here
GO

-- #################################################################################################
-- If set to 1 the option will remove previously added triggers
-- #################################################################################################
DECLARE @RemoveTriggers INT = 0

-- ///// CONFIG SECTION START //////////////////////////////////////////////////////////////////////
-- #################################################################################################
-- SECTION A -- if any value from section A set to 1 then section B must be set to 0, and vice-versa
-- #################################################################################################

-- set to 1 if wanting allow all players creating Summoner from level 1
DECLARE @AllowCreateSummonnerFromLevel1 INT = 1

-- set to 1 if wanting allow all players creating Rage Fighter from level 1
DECLARE @AllowCreateRageFighterFromLevel1 INT = 1

-- ///////////////////////////////////////////////////////////////////////////////////////////////// 
-- #################################################################################################
-- SECTION B -- if any value from section B set to 1 then section A must be set to 0, and vice-versa
-- #################################################################################################

-- set to 1 if wanting restrict players to create Summoner from specified level
DECLARE @AllowCreateSummonerAtDesiredLevel INT = 0

-- set to 1 if wanting reset ability to create Summoner to current players
DECLARE @ResetCurrentPlayersPossibilityToCreateSummoner INT = 0

-- set minimum level of character requiring to create Summoner
DECLARE @SummonerCreateLevel INT = 150


-- set to 1 if wanting restrict players to create Rage Fighter from specified level
DECLARE @AllowCreateRageFighterAtDesiredLevel INT = 0

-- set to 1 if wanting reset ability to create Rage Fighter to current players
DECLARE @ResetCurrentPlayersPossibilityToCreateRageFighter INT = 0

-- set minimum level of character requiring to create Rage Fighter
DECLARE @RageFighterCreateLevel INT = 200

IF (@RemoveTriggers = 1)
 BEGIN
  IF EXISTS (SELECT * FROM sysobjects WHERE name = 'RageFighterActivation' AND type = 'TR')
   BEGIN
	DROP TRIGGER RageFighterActivation
	print (N'INFO: RageFighterActivation deleted succesfully');
   END
   
  IF EXISTS (SELECT * FROM sysobjects WHERE name = 'SummonerActivation' AND type = 'TR')
   BEGIN
	DROP TRIGGER SummonerActivation
	print (N'INFO: SummonerActivation deleted succesfully');
   END
  RETURN
 END
-- ///////////////////////////////////////////////////////////////////////////////////////////////// 
-- ##### CONFIG SECTION END -- DO NOT MODIFY BELOW #################################################

-- CASE 1
IF (@AllowCreateSummonnerFromLevel1 = 1 AND @AllowCreateSummonerAtDesiredLevel = 0)
	BEGIN
		IF EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[DF_AccountCharacter_Summoner]'))
			BEGIN
				ALTER TABLE dbo.AccountCharacter DROP CONSTRAINT DF_AccountCharacter_Summoner
			END

		ALTER TABLE dbo.AccountCharacter ADD CONSTRAINT DF_AccountCharacter_Summoner DEFAULT ((1)) FOR Summoner;
		
		UPDATE dbo.AccountCharacter SET Summoner = 1
		print (N'CASE 1: ALL OK')
	END

ELSE IF (@AllowCreateSummonnerFromLevel1 = 1 AND @AllowCreateSummonerAtDesiredLevel = 1)
	print CHAR(13) + N'Something went wrong, verify configuration' + CHAR(13) + N'CASE 1: AllowCreateSummonnerFromLevel1 or AllowCreateSummonerAtDesiredLevel mis-configured, refer to Section A and B description'


-- CASE 2
IF (@AllowCreateRageFighterFromLevel1 = 1 AND @AllowCreateRageFighterAtDesiredLevel = 0)
	BEGIN
		IF EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[DF_AccountCharacter_RageFighter]'))
			BEGIN
				ALTER TABLE dbo.AccountCharacter DROP CONSTRAINT DF_AccountCharacter_RageFighter
			END

		ALTER TABLE dbo.AccountCharacter ADD CONSTRAINT DF_AccountCharacter_RageFighter DEFAULT ((1)) FOR RageFighter;
			
		UPDATE dbo.AccountCharacter SET RageFighter = 1
		print (N'CASE 2: ALL OK')
	END

ELSE IF (@AllowCreateRageFighterFromLevel1 = 1 AND @AllowCreateRageFighterAtDesiredLevel = 1)
	print CHAR(13) + N'Something went wrong, verify configuration' + CHAR(13) + N'CASE 2: AllowCreateRageFighterFromLevel1 or @AllowCreateRageFighterAtDesiredLevel mis-configured, refer to Section A and B description'


-- CASE 3
IF (@AllowCreateSummonnerFromLevel1 = 0 AND @AllowCreateSummonerAtDesiredLevel = 1)
	BEGIN
		IF EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[DF_AccountCharacter_Summoner]'))
			BEGIN
				ALTER TABLE dbo.AccountCharacter DROP CONSTRAINT DF_AccountCharacter_Summoner
			END

		ALTER TABLE dbo.AccountCharacter ADD CONSTRAINT DF_AccountCharacter_Summoner DEFAULT ((0)) FOR Summoner;
		
		IF (@ResetCurrentPlayersPossibilityToCreateSummoner = 1)
			BEGIN
				UPDATE dbo.AccountCharacter SET Summoner = 0
			END

		IF EXISTS (SELECT * FROM sys.objects WHERE [type] = 'TR' AND [name] = 'SummonerActivation')
		DROP TRIGGER SummonerActivation;
		
		EXEC ('
			CREATE TRIGGER SummonerActivation ON [dbo].[Character]
			FOR UPDATE
			AS
			SET NOCOUNT ON
				DECLARE @AccountID varchar(10);
				DECLARE @cLevel int;
				SELECT @AccountID=i.AccountID FROM inserted i;   
				SELECT @cLevel=i.cLevel FROM inserted i;
   
					IF (UPDATE(cLevel) AND (@cLevel >= ' + @SummonerCreateLevel + '))
					UPDATE dbo.AccountCharacter SET Summoner = 1 WHERE Id = @AccountID
			')
			print (N'CASE 3: ALL OK')
		END

ELSE IF (@AllowCreateSummonnerFromLevel1 = 1 AND @AllowCreateSummonerAtDesiredLevel = 1)
	print CHAR(13) + N'Something went wrong, verify configuration' + CHAR(13) + N'CASE 3: AllowCreateSummonnerFromLevel1 or AllowCreateSummonerAtDesiredLevel mis-configured, refer to Section A and B description'


-- CASE 4
IF (@AllowCreateRageFighterFromLevel1 = 0 AND @AllowCreateRageFighterAtDesiredLevel = 1)
	BEGIN

		IF EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[DF_AccountCharacter_RageFighter]'))
			BEGIN
				ALTER TABLE dbo.AccountCharacter DROP CONSTRAINT DF_AccountCharacter_RageFighter
			END

		ALTER TABLE dbo.AccountCharacter ADD CONSTRAINT DF_AccountCharacter_RageFighter DEFAULT ((0)) FOR RageFighter;
			
			IF (@ResetCurrentPlayersPossibilityToCreateRageFighter = 1)
				BEGIN
					UPDATE dbo.AccountCharacter SET RageFighter = 0
				END

		IF EXISTS (SELECT * FROM sys.objects WHERE [type] = 'TR' AND [name] = 'RageFighterActivation')
		DROP TRIGGER RageFighterActivation;
		
		EXEC ('
			CREATE TRIGGER RageFighterActivation ON [dbo].[Character]
			FOR UPDATE
			AS
			SET NOCOUNT ON
				DECLARE @AccountID varchar(10);
				DECLARE @cLevel int;
				SELECT @AccountID=i.AccountID FROM inserted i;
				SELECT @cLevel=i.cLevel FROM inserted i;
   
					IF (UPDATE(cLevel) AND (@cLevel >= ' + @RageFighterCreateLevel + '))
					UPDATE dbo.AccountCharacter SET RageFighter = 1 WHERE Id = @AccountID
			')
			print (N'CASE 4: ALL OK')
	END

ELSE IF (@AllowCreateRageFighterFromLevel1 = 1 AND @AllowCreateRageFighterAtDesiredLevel = 1)
	print CHAR(13) + N'Something went wrong, verify configuration' + CHAR(13) + N'CASE 4: AllowCreateRageFighterFromLevel1 or AllowCreateRageFighterAtDesiredLevel mis-configured, refer to Section A and B description'
GO