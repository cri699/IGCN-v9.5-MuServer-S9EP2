//////////////////////////////////////////////////////////////////////
// KeyGenerater.h
#ifndef KEYGENERATER_H
#define KEYGENERATER_H

static int KEY_GENERATE_FILTER[10][4] =
{
	321, 37531879, 8734, 32,
	873, 64374332, 3546, 87,
	537, 24798765, 5798, 32,
	654, 32498765, 3573, 73,
	546, 98465432, 6459, 12,
	987, 24654876, 5616, 54,
	357, 34599876, 8764, 98,
	665, 78641332, 6547, 54,
	813, 85132165, 8421, 98,
	454, 57684216, 6875, 45
};

#pragma once
class CKeyGenerater
{
public:
	CKeyGenerater();
	virtual ~CKeyGenerater();

	DWORD GenerateSeedValue();
	DWORD GenerateKeyValue(DWORD dwKeyValue);
	bool CheckKeyValue(DWORD &dwOldKeyValue, DWORD dwReceiveKeyValue);
};

extern CKeyGenerater g_KeyGenerater;

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

