//////////////////////////////////////////////////////////////////////
// TemporaryUserManager.h
#include "user.h"

#pragma once

struct TEMPORARY_USER_MANAGER_DATA
{
	DWORD m_dwTick;
	char m_Name[11];
	int m_Index;
	int m_Change;
	int m_PartyNumber;
	int m_MapNumber;
	int m_BloodCastleIndex;
	int m_BloodCastleSubIndex;
	int m_BloodCastleExp;
	int m_IllusionTempleIndex;
	int m_ImperialEventIndex;
	int m_DevilSquareScore;
	int m_DevilSquareExp;
	int m_DevilSquareMoney;
	char m_DevilSquareIndex;
	int m_DevilSquareAuth;

	void Clear()
	{
		this->m_dwTick = 0;
		memset(m_Name, 0x00, sizeof(m_Name));
		this->m_Index = 0;
		this->m_Change= 0;
		this->m_PartyNumber = 0;
		this->m_MapNumber = 0;
		this->m_BloodCastleIndex = 0;
		this->m_BloodCastleSubIndex = 0;
		this->m_BloodCastleExp = 0;
		this->m_IllusionTempleIndex = 0;
		this->m_ImperialEventIndex = 0;
		this->m_DevilSquareScore = 0;
		this->m_DevilSquareExp = 0;
		this->m_DevilSquareMoney = 0;
		this->m_DevilSquareIndex = 0;
		this->m_DevilSquareAuth = 0;

	}
};

typedef struct
{
	PBMSG_HEAD h;
	char szCharName[MAX_ACCOUNT_LEN+1];
	short sExGameServerCode;
}SDHP_REQ_SET_EXGAMESERVERCODE,*LPSDHP_REQ_SET_EXGAMESERVERCODE;

class CTemporaryUserManager
{
public:
	CTemporaryUserManager(void);
	virtual ~CTemporaryUserManager(void);

	static CTemporaryUserManager *  GetInstance();
	void InitializeUserData();
	static CTemporaryUserManager * m_pInstance;

	int SearchEmptyIndex();
	void ClearSpecificUserData(int Index);
	BOOL AddUserData(char * Name, int aIndex, int PartyNumber);
	BOOL AddToPriviousParty(int aIndex, int Index);
	void TidyUpTempUserDate(int Index);
	void DeleteUserInParty(int Index);
	void DeleteAllPartyUserData(int PartyNumber);
	int CheckMatchCharacter(char* Name);
	int CheckMatchIndex(int aIndex);
	int CheckInEventMap(int Index);
	void LeaveEvent(int Index);
	BOOL ResumeEvent(int Index, int aIndex);
	BOOL ResumeIllusionTemple(int Index, int aIndex);
	BOOL ResumeImperialGuardian(int Index, int aIndex);
	BOOL ResumeBloodCastle(int Index, int aIndex);
	BOOL ResumeDoppelganger(int Index, int aIndex);
	BOOL ResumeDevilSquare(int Index, int aIndex);
	void SendEventMapInfo(int Index, int aIndex);
	void ClearOverTimeUserData(int OverTimeSeconds);

private:
	CRITICAL_SECTION m_criti;
	TEMPORARY_USER_MANAGER_DATA * m_UserData;
};

extern void TidyupTempuserdate(int aIndex, int party_number, BYTE PartyPos);

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

