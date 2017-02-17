//////////////////////////////////////////////////////////////////////
// ItemSerial.h
#ifndef ITEMSERIAL_H
#define ITEMSERIAL_H

#pragma once
class CItemSerial
{
public:
	CItemSerial(void);
	virtual ~CItemSerial(void);

	bool Init();
	UINT64 GenerateSerial();
	UINT64 GetSerialCount();

private:
	UINT64 MakeSerial();
	BOOL GetSerialFromDB(UINT64 & serial, int MakeSerialCount);

	CRITICAL_SECTION m_ItemSerialCriti;
	UINT64 m_dwItemSerial;
	UINT64 m_dwLastSerial;
	CQuery m_Query;
};

extern CItemSerial g_ItemSerial;
#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

