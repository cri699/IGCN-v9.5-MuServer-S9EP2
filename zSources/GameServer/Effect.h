//////////////////////////////////////////////////////////////////////
// Effect.h
#pragma once

#define MAX_PLAYER_BUFF 16

class CEffect
{
public:
	CEffect(void);
	virtual ~CEffect(void);

	bool IsEffect();
	bool Set(int effect);
	bool Set(int effect, int item, int option1, int option2, int option3, int value1, int value2, int value3, int time, DWORD tick);
	void Clear();

	BYTE m_effect;
	int m_count;
	int m_value1;
	int m_value2;
	int m_value3;
	int m_effecttype1;
	int m_effecttype2;
	int m_effecttype3;
	int m_tick;
	DWORD m_tickcount;
};



//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

