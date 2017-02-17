//////////////////////////////////////////////////////////////////////
// PacketEncrypt.h
#ifndef PACKETENCRYPT_H
#define PACKETENCRYPT_H

#include "cryptopp\rijndael.h"
#include "cryptopp\rsa.h"

class CPacketEncrypt
{

public:

	CPacketEncrypt();
	virtual ~CPacketEncrypt();

	int Encrypt(BYTE * lpDest, BYTE * lpSource, int iSize);
	int Decrypt(BYTE * lpDest, BYTE * lpSource, int iSize);

private:
	CryptoPP::Rijndael::Encryption enc;
	CryptoPP::Rijndael::Decryption dec;
};

extern CPacketEncrypt g_PacketEncrypt;

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

