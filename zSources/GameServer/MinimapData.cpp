//////////////////////////////////////////////////////////////////////
// MinimapData.cpp
#include "StdAfx.h"
#include "MinimapData.h"
#include "GameMain.h"
#include "ReadScript.h"
#include "TLog.h"
#include "configread.h"

CMinimapData g_MinimapData;

CMinimapData::CMinimapData(void)
{
	this->m_vtMinimapData.clear();
	this->m_bFileLoad = false;
}

CMinimapData::~CMinimapData(void)
{
}

bool CMinimapData::LoadFile(char * szFile)
{
	this->m_vtMinimapData.clear();
	this->m_bFileLoad = false;

	if(g_ConfigRead.data.common.Version < 3)
	{
		return true;
	}

	SMDFile = fopen(szFile, "r");

	if(SMDFile == NULL)
	{
		g_Log.MsgBox("Cannot load %s file", szFile);
		return false;
	}

	int Token;
	int Type;
	int Count = 0;
	MINIMAP_DATA m_MinimapData;

	while ( true )
	{
		Token = GetToken();

		if ( Token == END )
			break;

		if ( Token == NAME )
		{
			if(!strcmp("end", TokenString)) break;
		}

		if ( Token == NUMBER )
		{
			Type = TokenNumber;

			if ( Type == 0 )
			{
				while ( true )
				{
					Token = GetToken();

					if ( strcmp(TokenString, "end") == 0 )
						break;

					m_MinimapData.btMapNumber = TokenNumber;

					Token = GetToken();
					m_MinimapData.btID = TokenNumber;

					Token = GetToken();
					m_MinimapData.btObjectType = TokenNumber;

					Token = GetToken();
					m_MinimapData.btObjectID = TokenNumber;

					Token = GetToken();
					m_MinimapData.btIsDisabled = TokenNumber;

					Token = GetToken();
					m_MinimapData.btX = TokenNumber;

					Token = GetToken();
					m_MinimapData.btY = TokenNumber;

					Token = GetToken();
					strcpy(m_MinimapData.szName, TokenString);

					this->m_vtMinimapData.push_back(m_MinimapData);
					Count++;
				}
			}
		}
	}

	fclose(SMDFile);
	this->m_bFileLoad = true;

	g_Log.Add(0, __FILE__, __FUNCTION__, "[Minimap Data] Loaded %d positions", Count);
}

void CMinimapData::SendData(int aIndex)
{
	if(g_ConfigRead.data.common.Version < 3)
	{
		return;
	}

	LPOBJ lpObj = &gObj[aIndex];
	PMSG_MINIMAP_DATA pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xE7, 0x03, sizeof(pMsg));

	for ( std::vector<MINIMAP_DATA>::iterator It = this->m_vtMinimapData.begin(); It != this->m_vtMinimapData.end(); It++ )
	{
		if ( It->btMapNumber == lpObj->MapNumber )
		{
			memset(pMsg.szName, 0x00, sizeof(pMsg.szName));
			pMsg.btID = It->btID;
			pMsg.btObjectType = It->btObjectType;
			pMsg.btObjectID = It->btObjectID;
			pMsg.btIsDisabled = It->btIsDisabled;
			pMsg.btX = It->btX;
			pMsg.btY = It->btY;
			strcpy(pMsg.szName, It->szName);

			IOCP.DataSend(aIndex, (LPBYTE)&pMsg, pMsg.h.size);
		}
	}
}

void CMinimapData::SendPartyData(int party_number)
{
	if(g_ConfigRead.data.common.Version < 3)
	{
		return;
	}

	if ( party_number < 0 )
		return;

	PMSG_MINIMAP_PARTY_USER pMsg;
	PHeadSubSetB((LPBYTE)&pMsg, 0xE7, 0x01, sizeof(pMsg));

	pMsg.btCount = 0;
	memset(pMsg.m_User, 0x00, sizeof(pMsg.m_User));

	for(int i=0;i<MAX_USER_IN_PARTY;i++)
	{
		int Number = gParty.m_PartyS[party_number].Number[i];

		if ( Number >= 0 )
		{
			strcpy(pMsg.m_User[pMsg.btCount].szName, gObj[Number].Name);
			pMsg.m_User[pMsg.btCount].szName[10] = 0;
			pMsg.m_User[pMsg.btCount].btMapNumber = gObj[Number].MapNumber;
			pMsg.m_User[pMsg.btCount].btX = gObj[Number].X;
			pMsg.m_User[pMsg.btCount].btY = gObj[Number].Y;
			pMsg.btCount++;
		}
	}

	for(int i=0;i<MAX_USER_IN_PARTY;i++)
	{
		int Number = gParty.m_PartyS[party_number].Number[i];

		if ( Number >= 0 )
		{
			IOCP.DataSend(Number, (LPBYTE)&pMsg, pMsg.h.size);
		}
	}
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

