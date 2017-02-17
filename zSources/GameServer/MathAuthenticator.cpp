//////////////////////////////////////////////////////////////////////
// MathAuthenticator.cpp
#include "stdafx.h"/*
#include "MathAuthenticator.h"
#include "user.h"
#include "GameMain.h"
#include "winutil.h"
#include "configread.h"

// It's class used by eX700 and later clients to authorize... something?
// I dont know, but I think it is used to disable some traps in main
// Based on 1.01.04 CHN GS. Coded by [IGC] Dudas
// Status: 100%

MathAuthenticator g_MathAuthen;

MathAuthenticator::MathAuthenticator()
{
	InitializeCriticalSection(&this->m_criti);
}

MathAuthenticator::~MathAuthenticator()
{
	DeleteCriticalSection(&this->m_criti);
}

void MathAuthenticator::Update()
{
	DWORD elapsed = GetTickCount() - this->m_dwTick;
	this->m_dwTick = GetTickCount();

	for (int i = g_ConfigRead.server.GetObjectStartUserIndex(); i < g_ConfigRead.server.GetObjectMax(); i++)
	{
		if (gObj[i].Connected == PLAYER_PLAYING && gObj[i].Type == OBJ_USER && gObj[i].m_PlayerData->ISBOT == false)
		{
			gObj[i].m_PlayerData->MathAuthenElapsed += elapsed;

			if (gObj[i].m_PlayerData->MathAuthenElapsed > 5000)
			{
				this->authen(i);
				gObj[i].m_PlayerData->MathAuthenElapsed = 0;
			}
		}
	}
}

void MathAuthenticator::authen(int objIndex)
{
	int fc = rand() % 6;
	EnterCriticalSection(&this->m_criti);

	switch (fc)
	{
	case 0:
		this->authenSin(objIndex);
		break;
	case 1:
		this->authenCos(objIndex);
		break;
	case 2:
		this->authenTan(objIndex);
		break;
	case 3:
		this->authenSqrt(objIndex);
		break;
	case 4:
		this->authenLog(objIndex);
		break;
	case 5:
		this->authenExp(objIndex);
		break;
	default:
		this->authenCos(objIndex);
		break;
	}

	LeaveCriticalSection(&this->m_criti);
}

void MathAuthenticator::authenSin(int objIndex)
{
	double iv = rand() % 10;

	MAPM func = iv;
	MAPM r = func.sin();

	this->send(objIndex, 0, iv, &r);
}

void MathAuthenticator::authenCos(int objIndex)
{
	double iv = rand() % 10;

	MAPM func = iv;
	MAPM r = func.cos();

	this->send(objIndex, 1, iv, &r);
}

void MathAuthenticator::authenTan(int objIndex)
{
	double iv = rand() % 10;

	MAPM func = iv;
	MAPM r = func.tan();

	this->send(objIndex, 2, iv, &r);
}

void MathAuthenticator::authenSqrt(int objIndex)
{
	double iv = rand() % 10;

	MAPM func = iv;
	MAPM r = func.sqrt();

	this->send(objIndex, 3, iv, &r);
}

void MathAuthenticator::authenLog(int objIndex)
{
	double iv = rand() % 10;

	MAPM func = iv;
	MAPM r = func.log();

	this->send(objIndex, 4, iv, &r);
}

void MathAuthenticator::authenExp(int objIndex)
{
	double iv = rand() % 10;

	MAPM func = iv;
	MAPM r = func.exp();

	this->send(objIndex, 5, iv, &r);
}

struct PMSG_MATH_AUTHEN
{
	PBMSG_HEAD2 h;
	int function;
	float input;
	char result[64];
};

void MathAuthenticator::send(int objIndex, int fc, double iv, MAPM *r)
{
	PMSG_MATH_AUTHEN pMsg;

	memset(&pMsg, 0x00, sizeof(pMsg));
	PHeadSubSetB((LPBYTE)&pMsg, 0xF3, 0x32, sizeof(pMsg));

	pMsg.function = fc;
	pMsg.input = iv;
	r->toString(pMsg.result, 5);

	IOCP.DataSend(objIndex, (LPBYTE)&pMsg, pMsg.h.size);
}*/

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

