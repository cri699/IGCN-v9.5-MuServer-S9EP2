//////////////////////////////////////////////////////////////////////
// GensSystemProtocol.h
#ifndef GENSSYSTEMPROTOCOL_H
#define GENSSYSTEMPROTOCOL_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_INFLUENCE_TYPE		3

enum INFLUENCE_TYPE
{
	//-----
	LEAVED_INFLUENCE  = -1, 
	//-----
	NONE_INFLUENCE    =  0,
	//----- 
	DUPRIAN_INFLUENCE  =  1, 
	//-----
	VANERT_INFLUENCE =  2
	//-----
};

enum USER_BATTLE_ZONE 
{	
	//-----
	BATTLE_ZONE_BLOCK   = 0,
	//-----
	BATTLE_ZONE_UNBLOCK = 1
	//-----
};

//--------------------------------------------------

struct _tagPMSG_REQ_REG_GENS_MEMBER
{
	PBMSG_HEAD2 h; //0
	BYTE bResult;  //4
}; 

struct _tagPMSG_REQ_SEGEDE_GENS_MEMBER
{
	PBMSG_HEAD2 h; //0
	BYTE bResult; //3
};

struct _tagPMSG_GENS_REWARD_CODE
{
	PBMSG_HEAD2 h; //0
	BYTE bReward;  //4
};

struct _tagPMSG_REQ_GENS_INFO
{
};

//--------------------------------------------------

struct _tagPMSG_ANS_GENS_INFO_EXDB
{
	PBMSG_HEAD2 h;
	BYTE bInfluence;
	WORD wIndexH;
	WORD wIndexL;
	int iContributePoint;
	int iResult;
	int iGensClass;
	int iGensRanking;
};

struct _tagPMSG_ANS_REG_GENS_MEBMER 
{
	PBMSG_HEAD2 h;   //0
	BYTE bResult;    //4  
	BYTE iInfluence; //5
};

struct _tagPMSG_ANS_SECEDE_GENS_MEMBER_EXDB 
{
	PBMSG_HEAD2 h; //0
	BYTE bResult;  //4
	BYTE bIndexH;  //5
	BYTE bIndexL;  //6
};

struct _tagPMSG_ANS_ABUSING_INFO //9A (154) 
{
	PBMSG_HEAD2 h; //2
	char Name[11]; //13
	char szCharNames[110]; //113
	int iKillUserCount; //114
	BYTE bIndexH; //154
	BYTE bIndexL; //155
	int KillCount[10]; //166
};

struct _tagPMSG_ANS_GENS_REWARD_EXDB
{
	PBMSG_HEAD2 h;
	BYTE bResult; //4
	int iInfluence;
	int iGensClass; //8
	BYTE bIndexH; //12
	BYTE bIndexL; //13
};

struct _tagPMSG_ANS_GENS_MEMBER_COUNT_EXDB //0xA (10)
{
	PBMSG_HEAD2 h;
	int iVanertMemberCount;
	int iDuprianMemberCount;
};

struct _tagPMSG_ANS_GENS_REWARD_DAY_CHECK_EXDB
{
	PBMSG_HEAD2 h;
	BYTE bIndexH;
	BYTE bIndexL;
	int iRewardDay;
};

//--------------------------------------------------

struct _tagPMSG_REQ_REG_GENS_MEMBER_EXDB //0x22 (34)
{
	PBMSG_HEAD2 h; 
	char AccountID[11];
	char Name[11];
	BYTE bInfluence;
	WORD wNumberH;
	WORD wNumberL;
	BYTE bIndexH;
	BYTE bIndexL;

	_tagPMSG_REQ_REG_GENS_MEMBER_EXDB()
	{
		memset(this->AccountID, 0x00, sizeof(this->AccountID));
		memset(this->Name, 0x00, sizeof(this->Name));
		this->bInfluence = 0;
		this->wNumberH = 0;
		this->wNumberL = 0;
		this->bIndexH = 0;
		this->bIndexL = 0;
	}
};

struct _tagPMSG_ANS_REG_GENS_MEMBER_EXDB
{
	PBMSG_HEAD2 h;   //0 
	BYTE bResult;    //4
	BYTE bInfluence; //5
	BYTE bIndexH;    //6
	BYTE bIndexL;    //7
};

struct _tagPMSG_REQ_SECEDE_GENS_MEMBER_EXDB //0x16 (22)
{
	PBMSG_HEAD2 h;
	char Name[11];
	WORD wNumberH;
	WORD wNumberL;
	BYTE bIndexH;
	BYTE bIndexL;

	_tagPMSG_REQ_SECEDE_GENS_MEMBER_EXDB()
	{
		memset(this->Name, 0x00, sizeof(this->Name));
		this->wNumberH = 0;
		this->wNumberL = 0;
		this->bIndexH = 0;
		this->bIndexL = 0;
	}
};

struct _tagPMSG_REQ_ABUSING_INFO //0x10 (16)
{
	PBMSG_HEAD2 h;
	char Name[11];
	BYTE bIndexH;
	BYTE bIndexL;
};

struct _tagPMSG_REQ_SAVE_CONTRIBUTE_POINT_EXDB //0x1A (24)
{
	PBMSG_HEAD2 h;
	char Name[11];
	int iContributePoint;
	int iGensClass;

	_tagPMSG_REQ_SAVE_CONTRIBUTE_POINT_EXDB()
	{
		memset(this->Name, 0x0, sizeof(this->Name));
		this->iContributePoint = 0;
		this->iGensClass = 0;
	}
};

struct _tagPMSG_REQ_SAVE_ABUSING_KILLUSER_EXDB //0xB0 (176)
{
	PBMSG_HEAD2 h; //
	char AccountID[0x0B];
	char Name[0x0B];
	char KillName[110]; 
	int KillCount[10]; 
	//i150
	//i154
	//b154
	//b155
};

struct _tagGENS_RANKING
{
	PBMSG_HEAD2 h;
};

struct GENS_RANKING_DATA
{
	int GensClass;
	int ContributePoint;
	int RankingPosition;
	BYTE RewardItemGroup;
	WORD RewardItemNumber;
	int RewardItemCount;
};

struct _tagPMSG_GENS_MEMBER_VIEWPORT_INFO // 0x10
{
	BYTE btInfluence;     // +0x0(0x1)
	BYTE NumberH;         // +0x1(0x1)
	BYTE NumberL;         // +0x2(0x1)
	int iGensRanking;     // +0x4(0x4)
	int iGensClass;       // +0x8(0x4)
 int iContributePoint; // +0xc(0x4)
};
extern _tagPMSG_GENS_MEMBER_VIEWPORT_INFO pGensMsg;

struct _tagPMSG_SEND_GENS_MEMBER_VIEWPORT // 0x6
{
	struct PWMSG_HEAD2 h; // +0x0(0x5)
	BYTE Count;           // +0x5(0x1)
};
extern _tagPMSG_SEND_GENS_MEMBER_VIEWPORT pGensCount;

//--------------------------------------------------

struct _tagPMSG_SEND_GENS_INFO //0x16 (20)
{
	PBMSG_HEAD2 h; 
	BYTE bInfluence;
	int iGensRanking;
	int iGensClass;
	int iContributePoint;
	int iNextContributePoint;
};

struct _tagPMSG_REQ_GENS_REWARD_CHECK_EXDB //0x18 (24)
{
	PBMSG_HEAD2 h;
	char Name[11];
	WORD wIndexH;
	WORD wIndexL;
	int iInfluence;
};

struct _tagPMSG_REQ_GENS_REWARD_COMPLETE_EXDB //0x14 (20)
{
	PBMSG_HEAD2 h;
	char Name[11];
	WORD wIndexH;
	WORD wIndexL;
};

struct _tagPMSG_SEND_GENS_REWARD //0x8 (8)
{
	PBMSG_HEAD2 h;
	BYTE bItemType; 
};

struct _tagPMSG_REQ_GENS_MEMBER_COUNT //0x14 (20)
{
	PBMSG_HEAD2 h;
	char Name[11];
	WORD wIndexH;
	WORD wIndexL;
};

struct _tagPMSG_SET_GENS_REWARD_DAY_EXDB
{
	PBMSG_HEAD2 h;
};

struct _tagPMSG_REQ_GENS_REWARD_DAY
{
	PBMSG_HEAD2 h;
	BYTE bIndexH, bIndexL;
};

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

