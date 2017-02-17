//////////////////////////////////////////////////////////////////////
// DataSendProtocolCore.cpp
#include "stdafx.h"
#include "protocol.h"
#include "DataSendProtocolCore.h"
#include "ReconnectSystem.h"
#include "EncDec.h"

// # NOT IN USE ATM

bool DSProtocolCore(BYTE* buff, BYTE protoNum, int len, int enc, int unk1)
{
	switch (protoNum)
	{
		case 0xF1:
			{
				PMSG_DEFAULT2 * lpMsg2 = (PMSG_DEFAULT2 *)buff;
				switch (lpMsg2->subcode)
				{
					case 0x01:
						CGJoinIdPassRequest((PMSG_IDPASS_OLD*)buff, buff, len);
						enc = 1;
						break;
				}
			}
			break;

		case 0x0E:
			CGLiveSend((PMSG_CLIENTTIME_OLD*)buff, buff, len);
			break;
	}
	return true;
}

void CGLiveSend(PMSG_CLIENTTIME_OLD *lpMsg, BYTE* buff, int len)
{
	
	BYTE NewClientTime[200];
	memset(NewClientTime, 0x00, sizeof(NewClientTime));

	PMSG_CLIENTTIME_NEW * lpMsg2 = (PMSG_CLIENTTIME_NEW *)NewClientTime;

	lpMsg2->h.c = 0xC1;
	lpMsg2->h.size = sizeof(PMSG_CLIENTTIME_NEW);
	lpMsg2->h.headcode = 0x0E;

	lpMsg2->TimeH = HIBYTE(lpMsg->Time);
	lpMsg2->TimeL = LOBYTE(lpMsg->Time);
	lpMsg2->Agility = *(WORD*)((*(DWORD*)0x851ACC4) + 0x11A) + *(WORD*)((*(DWORD*)0x851ACC4) + 0x134);
	lpMsg2->AttackSpeed = lpMsg->AttackSpeed;
	lpMsg2->MagicSpeed = lpMsg2->MagicSpeed;
	memcpy(lpMsg2->Version, DLL_VERSION, strlen(DLL_VERSION));

	buff = NewClientTime;
	len = lpMsg2->h.size;

}
void CGJoinIdPassRequest(PMSG_IDPASS_OLD *lpMsg, BYTE* buff, int len)
{
	BYTE NewIDPass[200];
	memset(NewIDPass, 0x00, sizeof(NewIDPass));

	PMSG_IDPASS_NEW * lpMsg2 = (PMSG_IDPASS_NEW *)NewIDPass;

	lpMsg2->h.c = 0xC1;
	lpMsg2->h.size = sizeof(PMSG_IDPASS_NEW);
	lpMsg2->h.headcode = 0xF1;
	lpMsg2->subcode = 0x01;
	lpMsg2->TickCount = lpMsg->TickCount;

	memcpy(lpMsg2->Id, lpMsg->Id, 10);
	memcpy(lpMsg2->Pass, lpMsg->Pass, 20);
	memcpy(lpMsg2->CliSerial, lpMsg->CliSerial, 16);
	memcpy(lpMsg2->CliVersion, lpMsg->CliVersion, 5);
	lpMsg2->ServerSeason = 0x40;

	memcpy(Login, (LPBYTE)lpMsg,sizeof(lpMsg));
	CEncDec::EncXor32(&Login[3], 3, sizeof(lpMsg) - 3);
	buff = NewIDPass;
	len = lpMsg2->h.size;

	// Fix account id save for s9
	DWORD OldProtect;
	VirtualProtect((LPVOID)0x08B97990, 12, PAGE_EXECUTE_READWRITE, &OldProtect);

	char AccountID[11];
	AccountID[10] = 0;
	memcpy(AccountID, lpMsg2->Id, 10);
	CEncDec::BuxConvert(AccountID, 10);

	memcpy((void*)0x08B97990, AccountID, 11);

}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

