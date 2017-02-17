USE [Me_MuOnline] -- 9.5.1.0
GO
/****** Object:  StoredProcedure [dbo].[IGC_VipAdd]    Script Date: 04/05/2015 23:12:11 ******/
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
/****** Object:  StoredProcedure [dbo].[VIPSystem_CheckAccount]    Script Date: 04/05/2015 23:12:11 ******/
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
/****** Object:  StoredProcedure [dbo].[WZ_CONNECT_MEMB]    Script Date: 04/05/2015 23:12:11 ******/
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
/****** Object:  StoredProcedure [dbo].[WZ_DISCONNECT_MEMB]    Script Date: 04/05/2015 23:12:11 ******/
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
/****** Object:  UserDefinedFunction [dbo].[fn_md5]    Script Date: 04/05/2015 23:12:11 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO

CREATE FUNCTION [dbo].[fn_md5] (@data VARCHAR(10), @data2 VARCHAR(10))
RETURNS BINARY(16) AS
BEGIN
DECLARE @hash BINARY(16)
EXEC master.dbo.XP_MD5_EncodeKeyVal @data, @data2, @hash OUT
RETURN @hash
END






GO
/****** Object:  Table [dbo].[IGC_MachineID_Banned]    Script Date: 03/09/2015 12:02:30 ******/
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
/****** Object:  Table [dbo].[ConnectionHistory]    Script Date: 04/05/2015 23:12:11 ******/
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
/****** Object:  Table [dbo].[MEMB_INFO]    Script Date: 04/05/2015 23:12:11 ******/
SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
SET ANSI_PADDING ON
GO
CREATE TABLE [dbo].[MEMB_INFO](
	[memb_guid] [int] IDENTITY(1,1) NOT NULL,
	[memb___id] [varchar](10) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
	[memb__pwd] [varchar](20) COLLATE SQL_Latin1_General_CP1_CI_AS NOT NULL,
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
/****** Object:  Table [dbo].[MEMB_STAT]    Script Date: 04/05/2015 23:12:11 ******/
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
/****** Object:  Table [dbo].[T_VIPList]    Script Date: 04/05/2015 23:12:11 ******/
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
SET ANSI_PADDING ON

GO
/****** Object:  Index [XI_MEMB_INFO_2]    Script Date: 04/05/2015 23:12:11 ******/
CREATE CLUSTERED INDEX [XI_MEMB_INFO_2] ON [dbo].[MEMB_INFO]
(
	[memb___id] DESC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, SORT_IN_TEMPDB = OFF, DROP_EXISTING = OFF, ONLINE = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON, FILLFACTOR = 70) ON [PRIMARY]
GO
/****** Object:  Index [PK_MEMB_INFO_1]    Script Date: 04/05/2015 23:12:11 ******/
ALTER TABLE [dbo].[MEMB_INFO] ADD  CONSTRAINT [PK_MEMB_INFO_1] PRIMARY KEY NONCLUSTERED 
(
	[memb_guid] DESC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, SORT_IN_TEMPDB = OFF, IGNORE_DUP_KEY = OFF, ONLINE = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
GO
SET ANSI_PADDING ON

GO
/****** Object:  Index [IX_MEMB_INFO_1]    Script Date: 04/05/2015 23:12:11 ******/
ALTER TABLE [dbo].[MEMB_INFO] ADD  CONSTRAINT [IX_MEMB_INFO_1] UNIQUE NONCLUSTERED 
(
	[memb___id] DESC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, SORT_IN_TEMPDB = OFF, IGNORE_DUP_KEY = OFF, ONLINE = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
GO
SET ANSI_PADDING ON

GO
/****** Object:  Index [IX_MEMB_DETAIL]    Script Date: 04/05/2015 23:12:11 ******/
CREATE NONCLUSTERED INDEX [IX_MEMB_DETAIL] ON [dbo].[MEMB_INFO]
(
	[sno__numb] DESC,
	[memb_name] DESC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, SORT_IN_TEMPDB = OFF, DROP_EXISTING = OFF, ONLINE = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
GO
ALTER TABLE [dbo].[MEMB_INFO] ADD  CONSTRAINT [DF_MEMB_INFO_mail_chek]  DEFAULT ((0)) FOR [mail_chek]
GO
