// CannonTower.cpp: implementation of the CCannonTower class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameServer.h"
#include "CannonTower.h"
#include "user.h"

//#include "..\include\readscript.h"
#include "winutil.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CCannonTower g_CsNPC_CannonTower;
CCannonTower::CCannonTower()
{

}

CCannonTower::~CCannonTower()
{

}

void CCannonTower::CannonTowerAct(int iIndex) // 0x00560940  1.00.19
{
	if ((rand() % 2) != 0) 	// review this
	{
		return;
	}
	LPOBJ lpObj = &gObj[iIndex];
	int count = 0;
	int ASBOfs = 0;
	PMSG_BEATTACK_COUNT pCount;
	PMSG_BEATTACK pAttack;

	pCount.h.c = 0xC1;
	pCount.h.headcode = 0xDB;
	pCount.h.size = 0x00;
	pCount.MagicNumberH = 0x00;
	pCount.MagicNumberL = 0x00;
	pCount.Count = 0x00;
	pCount.X = lpObj->X;
	pCount.Y = lpObj->Y;

	ASBOfs = sizeof(PMSG_BEATTACK_COUNT);

	unsigned char AttackSendBuff[256];

	while (true)
	{
		if (lpObj->VpPlayer2[count].state)
		{
			if (lpObj->VpPlayer2[count].type == OBJ_USER)
			{
				int tObjNum = lpObj->VpPlayer2[count].number;

				if (tObjNum >= 0)
				{
					if (gObj[tObjNum].m_btCsJoinSide)
					{
						if (gObjCalDistance(lpObj, &gObj[tObjNum]) < 7)
						{
							pAttack.NumberH = SET_NUMBERH(tObjNum);
							pAttack.NumberL = SET_NUMBERL(tObjNum);
							memcpy((AttackSendBuff + ASBOfs), (PBYTE)&pAttack, sizeof(PMSG_BEATTACK));
							ASBOfs += sizeof(PMSG_BEATTACK);
							pCount.Count++;
						}
					}
				}
			}
		}
		count++;

		if (count > MAX_VIEWPORT - 1) break;
	}
	if (pCount.Count > 0)
	{
		pCount.h.size = ASBOfs;

		memcpy(AttackSendBuff, (LPBYTE)&pCount, sizeof(PMSG_BEATTACK_COUNT));
		GSProtocol.CGBeattackRecv(AttackSendBuff, lpObj->m_Index, 1);
		WORD MagicNumber = 50;
		PMSG_DURATION_MAGIC_SEND pSend;
		PHeadSetBE((LPBYTE)&pSend, 0x1E, sizeof(pSend));
		pSend.MagicNumberH = SET_NUMBERH(MagicNumber);
		pSend.MagicNumberL = SET_NUMBERL(MagicNumber);
		pSend.X = lpObj->X;
		pSend.Y = lpObj->Y;
		pSend.Dir = 0;
		pSend.NumberH = SET_NUMBERH(iIndex);
		pSend.NumberL = SET_NUMBERL(iIndex);

		GSProtocol.MsgSendV2(lpObj, (PBYTE)&pSend, pSend.h.size);
	}
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

