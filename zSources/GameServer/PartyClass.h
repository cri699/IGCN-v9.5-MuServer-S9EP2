//////////////////////////////////////////////////////////////////////
// PartyClass.h
#ifndef PARTYCLASS_H
#define PARTYCLASS_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_USER_IN_PARTY 5
#define MAX_PARTY_DISTANCE_EFFECT	10

struct PARTY_STRUCT
{
	int		Count;	// 0
	int		Number[MAX_USER_IN_PARTY];	// 4
	int		DbNumber[MAX_USER_IN_PARTY];	// 18
	short	m_MaxLevel;	// 2C
	short	m_MinLevel;	// 2E
	char	m_UserPKLevel[5];
	char	m_PKPartyLevel;
	int		m_nPKUserCount;
	BOOL	bUsePartyMatching;
	int		nMatchingPartyCount;
	BYTE	bITLEnterOK[MAX_USER_IN_PARTY];
	BYTE	bITRWithYouOk[MAX_USER_IN_PARTY];
	BYTE	bDSFEnterOK[MAX_USER_IN_PARTY];

	PARTY_STRUCT()
	{
		this->Count = 0;
		memset(this->Number, 0x00, sizeof(this->Number));
		memset(this->DbNumber, 0x00, sizeof(this->DbNumber));
		this->m_MaxLevel = 0;
		this->m_MinLevel = 0;
		memset(this->m_UserPKLevel, 0x00, sizeof(this->m_UserPKLevel));
		this->m_PKPartyLevel = 0;
		this->m_nPKUserCount = 0;
		memset(this->bITLEnterOK, 0x00, sizeof(this->bITLEnterOK));
		memset(this->bITRWithYouOk, 0x00, sizeof(this->bITRWithYouOk));
		memset(this->bDSFEnterOK, 0x00, sizeof(this->bDSFEnterOK));
	}
};

class PartyClass
{

public:

	PartyClass();
	virtual ~PartyClass();

	void Init();
	void Delete();

	BOOL IsParty(int party_number);
	int Create(int usernumber, int dbnumber, int level);
	BOOL Destroy(int party_number);
	int Add(int party_number, int usernumber, int dbnumber,  int level);
	void Delete(int party_number, int index);
	int Delete(int party_number, int usernumber, int dbnumber);
	int GetCount(int party_number);
	int RevisionIndexUser(int party_number, BYTE *index, char *szName);
	int GetIndexUser(int party_number, int index, int& usernumber,  int& dbnumber);
	int GetPartyCount(int party_number);
	int GetIndex(int party_number, int usernumber, int dbnumber);
	BOOL SetLevel(int party_number, int level);
	BOOL GetLevel(int party_number, int& maxlevel, int& minlevel);
	BOOL Isleader(int party_number, int usernumber, int dbnumber);
	void Paint(int party_number);
	void PartyMemberLifeSend(int party_number);
	void PartyMemberLifeSendS8E3(int party_number);
	char GetPKPartyPenalty(int party_number);
	void UpdatePKUserInfo(int party_number, int usernumber, int dbnumber, char userPKLevel);
	void UpdatePKPartyPanalty(int party_number);
	void ChangeLeader(int party_number);
	BOOL EnterITL_PartyAuth(int partynumber, int userindex);
	char ReturnOKCount(int partynumber);
	void ClearPartyAuthITL(int partynumber);
	bool AllAgreeEnterITL(int partynumber);
	BOOL EnterITR_PartyAuth(int partynumber, int userindex);
	char ReturnOKCountITR(int partynumber);
	void ClearPartyAuthITR(int partynumber);
	bool AllAgreeEnterITR(int partynumber);
	BOOL EnterDSF_PartyAuth(int partynumber, int userindex);
	char DSFReturnOKCount(int partynumber);
	void ClearPartyAuthDSF(int partynumber);
	bool AllAgreeEnterDSF(int partynumber);
	char GetLorenMarketUserCount(int partynumber);

public:

	int m_PartyCount;	// 4
	PARTY_STRUCT * m_PartyS;	// 8
	char m_szTempPaint[256];	// 56B88

};

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

