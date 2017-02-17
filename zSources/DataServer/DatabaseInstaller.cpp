//////////////////////////////////////////////////////////////////////
// DatabaseInstaller.cpp
#include "stdafx.h"
#include "DatabaseInstaller.h"
#include "TLog.h"

CDatabaseInstaller::CDatabaseInstaller(void)
{
	this->m_IsConnected = 0;
}

CDatabaseInstaller::~CDatabaseInstaller(void)
{
}

void CDatabaseInstaller::Init()
{
	this->Database = new CQuery;

	this->Database->Connect(g_MuOnlineDNS, g_UserID, g_Password, g_ServerName);

	this->m_IsConnected = 1;
}

void CDatabaseInstaller::End()
{
	this->Database->Disconnect();

	delete this->Database;

	this->m_IsConnected = 0;
}

int CDatabaseInstaller::GetDBVersion()
{
	if(!this->m_IsConnected)
		return -1;

	int Version;

	this->Database->ExecQuery("SELECT Version FROM Mu_DBID");

	if(this->Database->Fetch() != SQL_SUCCESS)
	{
		this->Database->Close();
		this->Database->ExecQuery("IF NOT EXISTS (SELECT * FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME='Mu_DBID' AND COLUMN_NAME='Version')");
		this->Database->ExecQuery("BEGIN ALTER TABLE Mu_DBID ADD Version INT NULL DEFAULT (1) END");
		Version = 1;
	}

	else
	{
		Version = this->Database->GetAsInteger("Version");
		this->Database->Close();
	}

	return Version;
}

void CDatabaseInstaller::InstallUpdates()
{
	int Version = this->GetDBVersion();

	if(Version == -1)
		return;

	if(Version == 1)
	{
		this->Install7041();
	}
}

bool CDatabaseInstaller::Install7041()
{

	this->Database->ExecQuery("IF EXISTS (SELECT * FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME='Guild' AND COLUMN_NAME='G_Warehouse')");
	this->Database->ExecQuery("BEGIN ALTER TABLE Guild DROP COLUMN G_Warehouse END");
	this->Database->ExecQuery("GO");
	this->Database->ExecQuery("IF EXISTS (SELECT * FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME='Guild' AND COLUMN_NAME='G_WHPassword')");
	this->Database->ExecQuery("BEGIN ALTER TABLE Guild DROP COLUMN G_WHPassword END");
	this->Database->ExecQuery("IF EXISTS (SELECT * FROM INFORMATION_SCHEMA.COLUMNS WHERE TABLE_NAME='Guild' AND COLUMN_NAME='G_WHMoney')");
	this->Database->ExecQuery("BEGIN ALTER TABLE Guild DROP COLUMN G_WHMoney END");
	this->Database->ExecQuery("GO");
	this->Database->ExecQuery("IF  EXISTS (SELECT * FROM dbo.sysobjects WHERE id = OBJECT_ID(N'[DF_T_ExpirationItem_Info_ExpirationDate]') AND type = 'D')");
	this->Database->ExecQuery("BEGIN ALTER TABLE [dbo].[T_ExpirationItem_Info] DROP CONSTRAINT [DF_T_ExpirationItem_Info_ExpirationDate] END");
	this->Database->ExecQuery("GO");
	this->Database->ExecQuery("IF  EXISTS (SELECT * FROM sys.objects WHERE object_id = OBJECT_ID(N'[dbo].[T_ExpirationItem_Info]') AND type in (N'U'))");
	this->Database->ExecQuery("DROP TABLE [dbo].[T_ExpirationItem_Info]");
	this->Database->ExecQuery("GO");
	this->Database->ExecQuery("ALTER TABLE Guild ADD G_Warehouse VARBINARY(3840) NULL");
	this->Database->ExecQuery("ALTER TABLE Guild ADD G_WHPassword int NULL");
	this->Database->ExecQuery("ALTER TABLE Guild ADD G_WHMoney int NULL");
	this->Database->ExecQuery("GO");
	this->Database->ExecQuery("CREATE TABLE T_ExpirationItem_Info( [ItemSerial] [int] NOT NULL, [ExpirationDate] [bigint] NOT NULL, CONSTRAINT [PK_T_ExpirationItem_Info] PRIMARY KEY CLUSTERED ( [ItemSerial] ASC )WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY] ) ON [PRIMARY]");
	this->Database->ExecQuery("GO");
	this->Database->ExecQuery("ALTER TABLE T_ExpirationItem_Info ADD CONSTRAINT [DF_T_ExpirationItem_Info_ExpirationDate]  DEFAULT ((0)) FOR [ExpirationDate]");
	this->Database->ExecQuery("GO");
	this->Database->ExecQuery("INSERT INTO T_ExpirationItem_Info (ItemSerial, ExpirationDate) SELECT ItemSerial, PeriodEndTime FROM T_PeriodItemInfo WHERE EffectType1 = 0");
	this->Database->ExecQuery("GO");
	this->Database->ExecQuery("UPDATE Mu_DBID SET Version=2");
	g_Log.Add("Updated Database to 7.0.5");
	return true;
}


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

