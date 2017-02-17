//////////////////////////////////////////////////////////////////////
// giocp.h
#ifndef GIOCP_H
#define GIOCP_H
// defines
#define PARSE_PACKET_HOOK 0x00669EC7 // S9
#define PARSE_PACKET_STREAM 0x0063B42D // S9
#define PROTOCOL_CORE1 0x0067430E // S9
#define PROTOCOL_CORE2 0x0067084A // S9

#define SEND_PACKET_HOOK 0x004393AE // S9
#define MU_SEND_PACKET 0x0043976D // S9
#define MU_SENDER_CLASS 0x120703C // S9

// function headers

void ParsePacket(void* PackStream, int unk1, int unk2);
void DataSend(void* pData, int size);
void SendPacket(BYTE* buff, int len, int enc, int unk1);
extern SOCKET* g_MuSocket;

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

