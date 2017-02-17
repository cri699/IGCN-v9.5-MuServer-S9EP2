--############################################################
--# -- INTERNATIONAL GAMING CENTER NETWORK
--# -- www.igc-network.com
--# -- (C) 2010-2015 IGC-Network (R)
--# ---------------------------------------------------------
--# -- File is a part of IGCN Group MuOnline Server files.
--############################################################

DECLARE @DropExistingDatabases INT = 1 -- switch, enable/disable existing databases of specified names below
DECLARE @Create_Me_MuOnlineDB INT = 1 -- switch, enables/disables creation of Me_MuOnline DB for purpose of Shared Accounts system

IF (@DropExistingDatabases = 1)
  BEGIN
	IF EXISTS (SELECT name FROM master.dbo.sysdatabases WHERE name = N'Me_MuOnline')
	  DROP DATABASE [Me_MuOnline]
	  
	IF EXISTS (SELECT name FROM master.dbo.sysdatabases WHERE name = N'MuOnline')
	  DROP DATABASE [MuOnline]
	
	IF EXISTS (SELECT name FROM master.dbo.sysdatabases WHERE name = N'Events')
	DROP DATABASE [Events]
	
	IF EXISTS (SELECT name FROM master.dbo.sysdatabases WHERE name = N'Ranking')
	DROP DATABASE [Ranking]
	
	IF EXISTS (SELECT name FROM master.dbo.sysdatabases WHERE name = N'BattleCore')
	DROP DATABASE [BattleCore]
  END

IF (@Create_Me_MuOnlineDB = 1)
  BEGIN
	CREATE DATABASE [Me_MuOnline]  ON 
	(
		NAME = N'Me_MuOnline_Data', 
		FILENAME = N'C:\Program Files\Microsoft SQL Server\MSSQL10_50.MSSQLSERVER\MSSQL\DATA\Me_MuOnline_Data.MDF' , 
		SIZE = 5, FILEGROWTH = 10%
	) 
	LOG ON 
	(
		NAME = N'Me_MuOnline_Log', 
		FILENAME = N'C:\Program Files\Microsoft SQL Server\MSSQL10_50.MSSQLSERVER\MSSQL\DATA\Me_MuOnline_Log.LDF' , 
		SIZE = 5, FILEGROWTH = 10%
	)
	COLLATE SQL_Latin1_General_CP1_CI_AS
  END
GO

CREATE DATABASE [MuOnline]  ON 
(
	NAME = N'MuOnline_Data', 
	FILENAME = N'C:\Program Files\Microsoft SQL Server\MSSQL10_50.MSSQLSERVER\MSSQL\DATA\MuOnline_Data.MDF' , 
	SIZE = 5, FILEGROWTH = 10%
) 
LOG ON 
(
	NAME = N'MuOnline_Log', 
	FILENAME = N'C:\Program Files\Microsoft SQL Server\MSSQL10_50.MSSQLSERVER\MSSQL\DATA\MuOnline_Log.LDF' , 
	SIZE = 5, FILEGROWTH = 10%
)
COLLATE SQL_Latin1_General_CP1_CI_AS
GO



CREATE DATABASE [Events]  ON 
(
	NAME = N'Events_Data', 
	FILENAME = N'C:\Program Files\Microsoft SQL Server\MSSQL10_50.MSSQLSERVER\MSSQL\DATA\Events_Data.MDF' , 
	SIZE = 5, FILEGROWTH = 10%
) 
LOG ON 
(
	NAME = N'Events_Log', 
	FILENAME = N'C:\Program Files\Microsoft SQL Server\MSSQL10_50.MSSQLSERVER\MSSQL\DATA\Events_Log.LDF' , 
	SIZE = 5, FILEGROWTH = 10%
)
COLLATE SQL_Latin1_General_CP1_CI_AS
GO

CREATE DATABASE [Ranking]  ON 
(
	NAME = N'Ranking_Data', 
	FILENAME = N'C:\Program Files\Microsoft SQL Server\MSSQL10_50.MSSQLSERVER\MSSQL\DATA\Ranking_Data.MDF' , 
	SIZE = 5, FILEGROWTH = 10%
) 
LOG ON 
(
	NAME = N'Ranking_Log', 
	FILENAME = N'C:\Program Files\Microsoft SQL Server\MSSQL10_50.MSSQLSERVER\MSSQL\DATA\Ranking_Log.LDF' , 
	SIZE = 5, FILEGROWTH = 10%
)
COLLATE SQL_Latin1_General_CP1_CI_AS
GO

CREATE DATABASE [BattleCore]  ON 
(
	NAME = N'BattleCore_Data', 
	FILENAME = N'C:\Program Files\Microsoft SQL Server\MSSQL10_50.MSSQLSERVER\MSSQL\DATA\BattleCore_Data.MDF' , 
	SIZE = 5, FILEGROWTH = 10%
) 
LOG ON 
(
	NAME = N'BattleCore_Log', 
	FILENAME = N'C:\Program Files\Microsoft SQL Server\MSSQL10_50.MSSQLSERVER\MSSQL\DATA\BattleCore_Log.LDF' , 
	SIZE = 5, FILEGROWTH = 10%
)
COLLATE SQL_Latin1_General_CP1_CI_AS
GO