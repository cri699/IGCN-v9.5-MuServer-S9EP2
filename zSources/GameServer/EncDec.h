//////////////////////////////////////////////////////////////////////
// EncDec.h
#ifndef ENCDEC_H
#define ENCDEC_H

class CEncDec{
	bool m_CGInitialized;
	bool m_GCInitialized;

	BYTE m_CGXorKey[256];
	BYTE m_CGSubKey[256];

	BYTE m_GCXorKey[256];
	BYTE m_GCSubKey[256];
public:
	CEncDec();
	void OnConnect();
	void CreateServerKey(DWORD xorSeed, DWORD subSeed);
	void CreateClientKey(DWORD xorSeed, DWORD subSeed);

	void EncCGBuff(BYTE* buff, int len);
	bool DecCGBuff(BYTE* buff, int len);

	void EncGCBuff(BYTE* buff, int len);
	void DecGCBuff(BYTE* buff, int len);
};

extern CEncDec* g_EncDec;
#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

