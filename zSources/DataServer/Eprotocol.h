// Eprotocol.h: interface for the Eprotocol class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EPROTOCOL_H__25F5CC2C_9902_45AA_904D_F6DC2D9B01EA__INCLUDED_)
#define AFX_EPROTOCOL_H__25F5CC2C_9902_45AA_904D_F6DC2D9B01EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

void EProtocolCore(int aIndex, BYTE HeadCode, LPBYTE aRecv, int iSize);
void InitEventDB();

struct PMSG_REQ_VIEW_EC_MN {
  /*<thisrel this+0x0>*/ /*|0x3|*/ struct PBMSG_HEAD h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iINDEX;
  /*<thisrel this+0x8>*/ /*|0xb|*/ char szUID[11];
};
// <size 0x14>

struct PMSG_REQ_REGISTER_EVENTCHIP {
  /*<thisrel this+0x0>*/ /*|0x3|*/ struct PBMSG_HEAD h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iINDEX;
  /*<thisrel this+0x8>*/ /*|0x1|*/ unsigned char Pos;
  /*<thisrel this+0x9>*/ /*|0xb|*/ char szUID[11];
};
// <size 0x14>

struct PMSG_REQ_RESET_EVENTCHIP {
  /*<thisrel this+0x0>*/ /*|0x3|*/ struct PBMSG_HEAD h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iINDEX;
  /*<thisrel this+0x8>*/ /*|0xb|*/ char szUID[11];
};
// <size 0x14>

struct PMSG_REQ_REGISTER_STONES {
  /*<thisrel this+0x0>*/ /*|0x3|*/ struct PBMSG_HEAD h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iINDEX;
  /*<thisrel this+0x8>*/ /*|0x1|*/ unsigned char iPosition;
  /*<thisrel this+0x9>*/ /*|0xb|*/ char szUID[11];
};
// <size 0x14>

struct PMSG_REQ_DELETE_STONES {
  /*<thisrel this+0x0>*/ /*|0x3|*/ struct PBMSG_HEAD h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iINDEX;
  /*<thisrel this+0x8>*/ /*|0xb|*/ char szUID[11];
  /*<thisrel this+0x13>*/ /*|0x1|*/ char bSUCCESS;
  /*<thisrel this+0x14>*/ /*|0x4|*/ int iStoneCount;
};
// <size 0x18>

struct PMSG_REQ_2ANIV_SERIAL {
  /*<thisrel this+0x0>*/ /*|0x3|*/ struct PBMSG_HEAD h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iINDEX;
  /*<thisrel this+0x8>*/ /*|0xb|*/ char szUID[11];
  /*<thisrel this+0x13>*/ /*|0x5|*/ char SERIAL1[5];
  /*<thisrel this+0x18>*/ /*|0x5|*/ char SERIAL2[5];
  /*<thisrel this+0x1d>*/ /*|0x5|*/ char SERIAL3[5];
  /*<thisrel this+0x24>*/ /*|0x4|*/ int iMEMB_GUID;
};
// <size 0x28>

struct PMSG_REQ_REG_RINGGIFT {
  /*<thisrel this+0x0>*/ /*|0x3|*/ struct PBMSG_HEAD h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iINDEX;
  /*<thisrel this+0x8>*/ /*|0xb|*/ char szUID[11];
  /*<thisrel this+0x13>*/ /*|0x1|*/ unsigned char btGiftKind;
};
// <size 0x14>

struct PMSG_REQ_REG_CC_OFFLINE_GIFT {
  /*<thisrel this+0x0>*/ /*|0x3|*/ struct PBMSG_HEAD h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iINDEX;
  /*<thisrel this+0x8>*/ /*|0xb|*/ char szUID[11];
  /*<thisrel this+0x14>*/ /*|0x2|*/ unsigned short wServerCode;
  /*<thisrel this+0x16>*/ /*|0xb|*/ char szNAME[11];
};
// <size 0x24>

struct PMSG_REQ_REG_DL_OFFLINE_GIFT {
  /*<thisrel this+0x0>*/ /*|0x3|*/ struct PBMSG_HEAD h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iINDEX;
  /*<thisrel this+0x8>*/ /*|0xb|*/ char szUID[11];
  /*<thisrel this+0x14>*/ /*|0x2|*/ unsigned short wServerCode;
  /*<thisrel this+0x16>*/ /*|0xb|*/ char szNAME[11];
};
// <size 0x24>

struct PMSG_REQ_REG_HT_OFFLINE_GIFT {
  /*<thisrel this+0x0>*/ /*|0x3|*/ struct PBMSG_HEAD h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iINDEX;
  /*<thisrel this+0x8>*/ /*|0xb|*/ char szUID[11];
  /*<thisrel this+0x14>*/ /*|0x2|*/ unsigned short wServerCode;
  /*<thisrel this+0x16>*/ /*|0xb|*/ char szNAME[11];
};
// <size 0x24>

/**************************************************************************/

struct PMSG_ANS_VIEW_EC_MN {
  /*<thisrel this+0x0>*/ /*|0x3|*/ struct PBMSG_HEAD h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iINDEX;
  /*<thisrel this+0x8>*/ /*|0xb|*/ char szUID[11];
  /*<thisrel this+0x13>*/ /*|0x1|*/ char bSUCCESS;
  /*<thisrel this+0x14>*/ /*|0x2|*/ short nEVENT_CHIPS;
  /*<thisrel this+0x18>*/ /*|0x4|*/ int iMUTO_NUM;
};
// <size 0x1c>

struct PMSG_ANS_REGISTER_EVENTCHIP {
  /*<thisrel this+0x0>*/ /*|0x3|*/ struct PBMSG_HEAD h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iINDEX;
  /*<thisrel this+0x8>*/ /*|0x1|*/ unsigned char Pos;
  /*<thisrel this+0x9>*/ /*|0xb|*/ char szUID[11];
  /*<thisrel this+0x14>*/ /*|0x1|*/ char bSUCCESS;
  /*<thisrel this+0x16>*/ /*|0x2|*/ short nEVENT_CHIPS;
};
// <size 0x18>

struct PMSG_ANS_RESET_EVENTCHIP {
  /*<thisrel this+0x0>*/ /*|0x3|*/ struct PBMSG_HEAD h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iINDEX;
  /*<thisrel this+0x8>*/ /*|0xb|*/ char szUID[11];
  /*<thisrel this+0x13>*/ /*|0x1|*/ char bSUCCESS;
};
// <size 0x14>

struct PMSG_ANS_VIEW_STONES {
  /*<thisrel this+0x0>*/ /*|0x3|*/ struct PBMSG_HEAD h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iINDEX;
  /*<thisrel this+0x8>*/ /*|0xb|*/ char szUID[11];
  /*<thisrel this+0x13>*/ /*|0x1|*/ char bSUCCESS;
  /*<thisrel this+0x14>*/ /*|0x4|*/ int iStoneCount;
};
// <size 0x18>

struct PMSG_ANS_REGISTER_STONES {
  /*<thisrel this+0x0>*/ /*|0x3|*/ struct PBMSG_HEAD h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iINDEX;
  /*<thisrel this+0x8>*/ /*|0x1|*/ unsigned char iPosition;
  /*<thisrel this+0x9>*/ /*|0xb|*/ char szUID[11];
  /*<thisrel this+0x14>*/ /*|0x1|*/ char bSUCCESS;
  /*<thisrel this+0x18>*/ /*|0x4|*/ int iStoneCount;
};
// <size 0x1c>

struct PMSG_ANS_DELETE_STONES {
  /*<thisrel this+0x0>*/ /*|0x3|*/ struct PBMSG_HEAD h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iINDEX;
  /*<thisrel this+0x8>*/ /*|0xb|*/ char szUID[11];
  /*<thisrel this+0x13>*/ /*|0x1|*/ char bSUCCESS;
  /*<thisrel this+0x14>*/ /*|0x4|*/ int iStoneCount;
};
// <size 0x18>

struct PMSG_ANS_2ANIV_SERIAL {
  /*<thisrel this+0x0>*/ /*|0x3|*/ struct PBMSG_HEAD h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iINDEX;
  /*<thisrel this+0x8>*/ /*|0xb|*/ char szUID[11];
  /*<thisrel this+0x13>*/ /*|0x1|*/ unsigned char btIsRegistered;
  /*<thisrel this+0x14>*/ /*|0x4|*/ int iGiftNumber;
};
// <size 0x18>

struct PMSG_ANS_REG_RINGGIFT {
  /*<thisrel this+0x0>*/ /*|0x3|*/ struct PBMSG_HEAD h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iINDEX;
  /*<thisrel this+0x8>*/ /*|0xb|*/ char szUID[11];
  /*<thisrel this+0x13>*/ /*|0x1|*/ unsigned char btIsRegistered;
  /*<thisrel this+0x14>*/ /*|0x1|*/ unsigned char btGiftSection;
  /*<thisrel this+0x15>*/ /*|0x1|*/ unsigned char btGiftKind;
};
// <size 0x18>

struct PMSG_ANS_REG_CC_OFFLINE_GIFT {
  /*<thisrel this+0x0>*/ /*|0x3|*/ struct PBMSG_HEAD h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iINDEX;
  /*<thisrel this+0x8>*/ /*|0xb|*/ char szUID[11];
  /*<thisrel this+0x14>*/ /*|0x2|*/ unsigned short wServerCode;
  /*<thisrel this+0x16>*/ /*|0xb|*/ char szNAME[11];
  /*<thisrel this+0x24>*/ /*|0x4|*/ int iResultCode;
  /*<thisrel this+0x28>*/ /*|0x32|*/ char szGIFT_NAME[50];
};
// <size 0x5c>

struct PMSG_ANS_REG_DL_OFFLINE_GIFT {
  /*<thisrel this+0x0>*/ /*|0x3|*/ struct PBMSG_HEAD h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iINDEX;
  /*<thisrel this+0x8>*/ /*|0xb|*/ char szUID[11];
  /*<thisrel this+0x14>*/ /*|0x2|*/ unsigned short wServerCode;
  /*<thisrel this+0x16>*/ /*|0xb|*/ char szNAME[11];
  /*<thisrel this+0x24>*/ /*|0x4|*/ int iResultCode;
  /*<thisrel this+0x28>*/ /*|0x32|*/ char szGIFT_NAME[50];
};
// <size 0x5c>

struct PMSG_ANS_REG_HT_OFFLINE_GIFT {
  /*<thisrel this+0x0>*/ /*|0x3|*/ struct PBMSG_HEAD h;
  /*<thisrel this+0x4>*/ /*|0x4|*/ int iINDEX;
  /*<thisrel this+0x8>*/ /*|0xb|*/ char szUID[11];
  /*<thisrel this+0x14>*/ /*|0x2|*/ unsigned short wServerCode;
  /*<thisrel this+0x16>*/ /*|0xb|*/ char szNAME[11];
  /*<thisrel this+0x24>*/ /*|0x4|*/ int iResultCode;
  /*<thisrel this+0x28>*/ /*|0x32|*/ char szGIFT_NAME[50];
};
// <size 0x5c>

struct PMSG_ANS_REG_LUCKYCOIN
{
	PBMSG_HEAD h;
	int iIndex;
	char szUID[11];
	BYTE Result;
	BYTE Pos;
	int LuckyCoins;
};

struct PMSG_REQ_REGISTER_LUCKYCOIN
{
	PBMSG_HEAD h;
	int iIndex;
	char szAccountID[11];
	BYTE btPos;
};

struct PMSG_REQ_LUCKYCOIN
{
	PBMSG_HEAD h;
	int iIndex;
	char szUID[11];
};

struct PMSG_ANS_LUCKYCOIN
{
	PBMSG_HEAD h;
	int iIndex;
	char szUID[11];
	int LuckyCoins;
};

void EGAnsEventChipInfo(int aIndex,PMSG_REQ_VIEW_EC_MN * lpMsg);
void EGAnsRegEventChipInfo(int aIndex,PMSG_REQ_REGISTER_EVENTCHIP * lpMsg);
void EGAnsResetEventChip(int aIndex,PMSG_REQ_RESET_EVENTCHIP * lpMsg);
void EGAnsEventStoneInfo(int aIndex,PMSG_REQ_VIEW_EC_MN * lpMsg);
void EGAnsRegEventStoneInfo(int aIndex,PMSG_REQ_REGISTER_STONES * lpMsg);
void EGAnsResetStoneInfo(int aIndex,PMSG_REQ_RESET_EVENTCHIP * lpMsg);
void EGAns2AnivRegSerial(int aIndex,PMSG_REQ_2ANIV_SERIAL * lpMsg);
void EGAnsDeleteStones(int aIndex,PMSG_REQ_DELETE_STONES * lpMsg);
void EGAnsRegCCOfflineGift(int aIndex,PMSG_REQ_REG_CC_OFFLINE_GIFT * lpMsg);
void EGAnsRegDLOfflineGift(int aIndex,PMSG_REQ_REG_DL_OFFLINE_GIFT * lpMsg);
void EGAnsRegHTOfflineGift(int aIndex,PMSG_REQ_REG_HT_OFFLINE_GIFT * lpMsg);
void EGAnsLuckyCoinInfo(int aIndex,PMSG_REQ_LUCKYCOIN * lpMsg);
void EGAnsRegLuckyCoin(int aIndex,PMSG_REQ_REGISTER_LUCKYCOIN * lpMsg);

#endif // !defined(AFX_EPROTOCOL_H__25F5CC2C_9902_45AA_904D_F6DC2D9B01EA__INCLUDED_)


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

