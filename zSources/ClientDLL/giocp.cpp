//////////////////////////////////////////////////////////////////////
// giocp.cpp
#include "stdafx.h"
#include "giocp.h"
#include "PacketEncrypt.h"
#include "protocol.h"
#include "EncDec.h"
#include "ReconnectSystem.h"
#include "DataSendProtocolCore.h"
#include "offsets.h"

SOCKET* g_MuSocket = (SOCKET*)(PROTOCOL_SOCKET_ADDR);

void ParsePacket(void* PackStream, int unk1, int unk2)
{
	BYTE* buff;
	while (true)
	{
		__asm {
			MOV ECX, PackStream;
			MOV EDX, PARSE_PACKET_STREAM;
			CALL EDX;
			MOV buff, EAX;
		}
		if (!buff)
			break;

		BYTE DecBuff[7024];
		unsigned int DecSize;

		int proto;
		int size;
		int enc;

		switch (buff[0])
		{
			case 0xC1:
				proto = buff[2];
				size = buff[1];
				enc = 0;
				break;
			case 0xC2:
				proto = buff[3];
				size = *(WORD*)&buff[1];
				enc = 0;
				break;
			case 0xC3:
				enc = 1;
				size = buff[1];
				DecSize = g_PacketEncrypt.Decrypt(&DecBuff[1], &buff[2], size - 2);
				DecBuff[0] = 0xC1;
				DecBuff[1] = DecSize + 2;
				size = DecSize + 2;
				buff = DecBuff;
				proto = DecBuff[2];
				break;
			case 0xC4:
				enc = 1;
				size = MAKEWORD(buff[2], buff[1]);
				DecSize = g_PacketEncrypt.Decrypt(&DecBuff[2], &buff[3], size - 3);
				DecBuff[0] = 0xC2;
				DecBuff[2] = LOBYTE(DecSize + 3);
				DecBuff[1] = HIBYTE(DecSize + 3);
				size = DecSize + 3;
				buff = DecBuff;
				proto = buff[3];
				break;
		}

			if (unk1 == 1)
			{
				typedef int(*tProtocolCore2)(int, int, BYTE*, int, int);
				tProtocolCore2 ProtocolCore2 = (tProtocolCore2)PROTOCOL_CORE1;
				ProtocolCore2(unk2, proto, buff, size, enc);
			}
			else
			{
				typedef int(*tProtocolCore)(int, BYTE*, int, int);
				tProtocolCore ProtocolCore = (tProtocolCore)PROTOCOL_CORE2;
				bool bUseClientProtocolCore = CliProtocolCore(buff, proto, size, enc); // DLL protocolcore
				if (bUseClientProtocolCore)
				{
					ProtocolCore(proto, buff, size, enc); // Main.exe protocolcore
				}
				
			}

		
	}
}

void DataSend(void* pData, int size)
{
	BYTE* pack = (BYTE*)pData;

	if (pack[0] == 0xC1)
	{
		CEncDec::EncXor32(&pack[3], 3, size - 3);
	}

	else if (pack[0] == 0xC2)
	{
		CEncDec::EncXor32(&pack[4], 4, size - 4);
	}

	SendPacket((BYTE*)pData, size, 0, 0);
}

void __declspec(naked) muSendPacket(BYTE* buff, int len)
{
	__asm
	{
		PUSH EBP;
		MOV EBP, ESP;
		MOV EAX, len;
		PUSH EAX;
		PUSH buff;
		MOV ECX, DWORD PTR DS : [MU_SENDER_CLASS];
		MOV EDX, MU_SEND_PACKET;
		CALL EDX;
		MOV ESP, EBP;
		POP EBP;
		RETN;
	}
}

void SendPacket(BYTE* buff, int len, int enc, int unk1)
{
	if (buff[0] == 0xC1)
	{
		CEncDec::DecXor32(&buff[3], 3, len - 3);
	}

	else if (buff[0] == 0xC2)
	{
		CEncDec::DecXor32(&buff[4], 4, len - 4);
	}

	if (buff[2] == 0xF3 && buff[3] == 0x03)
	{
		memcpy(bCharacter, buff, buff[1]);
		CEncDec::EncXor32(&bCharacter[3], 3, buff[1] - 3);
	}

	if(buff[2] == 0xB1 && buff[3] == 0x01)
	{
		if (g_ReconnectProcess >= 2 && g_ReconnectProcess != 6)
			return;
	}

	if (buff[2] == 0xF1 && buff[3] == 0x01) // ban by hwid
	{
		BYTE NewIDPass[200];
		memset(NewIDPass, 0x00, sizeof(NewIDPass));

		PMSG_IDPASS_OLD * lpMsg1 = (PMSG_IDPASS_OLD *)buff;
		PMSG_IDPASS_NEW * lpMsg2 = (PMSG_IDPASS_NEW *)NewIDPass;

		lpMsg2->h.c = 0xC1;
		lpMsg2->h.size = sizeof(PMSG_IDPASS_NEW);
		lpMsg2->h.headcode = 0xF1;
		lpMsg2->subcode = 0x01;
		lpMsg2->TickCount = lpMsg1->TickCount;

		memcpy(lpMsg2->Id, lpMsg1->Id, 10);
		memcpy(lpMsg2->Pass, lpMsg1->Pass, 20);
		memcpy(lpMsg2->CliSerial, lpMsg1->CliSerial, 16);
		memcpy(lpMsg2->CliVersion, lpMsg1->CliVersion, 5);
		lpMsg2->ServerSeason = 0x40;

		memcpy(Login, buff, buff[1]);
		buff = NewIDPass;
		len = lpMsg2->h.size;
		CEncDec::EncXor32(&Login[3], 3, len - 3);

		// Fix account id save for s9
		DWORD OldProtect;
		VirtualProtect((LPVOID)0x08B97990, 12, PAGE_EXECUTE_READWRITE, &OldProtect); // S9

		char AccountID[11];
		AccountID[10] = 0;
		memcpy(AccountID, lpMsg2->Id, 10);
		CEncDec::BuxConvert(AccountID, 10);

		memcpy((void*)0x08B97990, AccountID, 11);

		enc = 1;
	}

	else if (buff[2] == 0x0E)
	{
		
		BYTE NewClientTime[200];
		memset(NewClientTime, 0x00, sizeof(NewClientTime));

		PMSG_CLIENTTIME_OLD * lpMsg1 = (PMSG_CLIENTTIME_OLD *)buff;
		PMSG_CLIENTTIME_NEW * lpMsg2 = (PMSG_CLIENTTIME_NEW *)NewClientTime;

		lpMsg2->h.c = 0xC1;
		lpMsg2->h.size = sizeof(PMSG_CLIENTTIME_NEW);
		lpMsg2->h.headcode = 0x0E;

		lpMsg2->TimeH = HIBYTE(lpMsg1->Time);
		lpMsg2->TimeL = LOBYTE(lpMsg1->Time);
		lpMsg2->Agility = *(WORD*)((*(DWORD*)0x851ACC4) + 0x11A) + *(WORD*)((*(DWORD*)0x851ACC4) + 0x134);
		lpMsg2->AttackSpeed = lpMsg1->AttackSpeed;
		lpMsg2->MagicSpeed = lpMsg1->MagicSpeed;
		memcpy(lpMsg2->Version, DLL_VERSION, strlen(DLL_VERSION));

		buff = NewClientTime;
		len = lpMsg2->h.size;

		enc = 1;

	}

	if (buff[0] == 0xC1)
	{
		CEncDec::EncXor32(&buff[3], 3, len - 3);
	}

	else if (buff[0] == 0xC2)
	{
		CEncDec::EncXor32(&buff[4], 4, len - 4);
	}

	if (enc)
	{
		BYTE EncBuff[7024];
		unsigned int EncSize;

		if (buff[0] == 0xC1)
		{
			EncSize = g_PacketEncrypt.Encrypt(&EncBuff[2], &buff[2], len - 2);
			EncBuff[0] = 0xC3;
			EncBuff[1] = EncSize + 2;
			buff = EncBuff;
			len = EncSize + 2;
		}

		else if (buff[0] == 0xC2)
		{
			EncSize = g_PacketEncrypt.Encrypt(&EncBuff[3], &buff[3], len - 3);
			EncBuff[0] = 0xC4;
			EncBuff[1] = HIBYTE(EncSize + 3);
			EncBuff[2] = LOBYTE(EncSize + 3);
			buff = EncBuff;
			len = EncSize + 3;
		}
	}

	muSendPacket(buff, len);
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

