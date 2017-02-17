//////////////////////////////////////////////////////////////////////
// PacketEncrypt.h
#ifndef PACKETENCRYPT_H
#define PACKETENCRYPT_H

class CPacketEncrypt
{

public:

	CPacketEncrypt();
	virtual ~CPacketEncrypt();

	int Encrypt(void * lpDest, void * lpSource, int iSize);
	int Decrypt(void * lpDest, void * lpSource, int iSize);

private:

	int EncryptBlock(void *, void *, int);
	int DecryptBlock(void *, void *);
	void DecryptBlock(unsigned long* lpdwFrom,unsigned long* lpdwTo);
	void EncryptBlock(unsigned long* lpdwFrom,unsigned long* lpdwTo);
};

extern CPacketEncrypt g_PacketEncrypt;

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

