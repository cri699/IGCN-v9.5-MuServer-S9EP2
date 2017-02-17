//////////////////////////////////////////////////////////////////////
// EncDec.h
#ifndef ENCDEC_H
#define ENCDEC_H

//#include <SRSA.h>
//extern CPrivateRsaKey* g_RsaDec;
//extern CPublicRsaKey* g_RsaEnc;

//extern CPublicRsaKey* g_ClientEnc;
//extern CPrivateRsaKey* g_ServerDec;

static unsigned char bBuxCode[3] = { 0xFC, 0xCF, 0xAB };	// Xox Key for some interesthing things :)
class CEncDec {
	friend class CAntiSpeed;
	bool m_CGInitialized;
	bool m_GCInitialized;

	BYTE* m_CGXorKey;
	BYTE* m_CGSubKey;

	BYTE* m_GCXorKey;
	BYTE* m_GCSubKey;
public:

	DWORD __fastcall ComputeCRC(BYTE* data, int size);
	DWORD __fastcall ComputeFileCRC(char* file);
	static void EncXor32(unsigned char*Buff, int SizeOfHeader, int Len);
	static void DecXor32(unsigned char*Buff, int SizeOfHeader, int Len);
	static void BuxConvert(char* buf, int size);
};
extern CEncDec* g_EncDec;
#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

