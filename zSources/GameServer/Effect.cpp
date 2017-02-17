//////////////////////////////////////////////////////////////////////
// Effect.cpp
#include "stdafx.h"
#include "Effect.h"

CEffect::CEffect(void)
{
	this->Clear();
}

CEffect::~CEffect(void)
{
}

bool CEffect::IsEffect()
{
    if(this->m_effect == 0xFF)
	{
	    return 0;
	}

	return 1;
}

bool CEffect::Set(int effect)
{
    if(this->m_effect != 0xFF)
	{
        return 0;
	}

	this->m_effect = effect;
	this->m_count = 0;
	this->m_value1 = 0;
	this->m_value2 = 0;
	this->m_value3 = 0;
	this->m_effecttype1 = 0;
	this->m_effecttype2 = 0;
	this->m_effecttype3 = 0;
	this->m_tick = 0;
	return 1;
}

bool CEffect::Set(int effect, int item, int option1, int option2, int option3, int value1, int value2, int value3, int time, DWORD tick)
{
	if(this->m_effect != 0xFF)
	{
		return 0;
	}
	this->m_effect = effect;
	this->m_count = time;
	this->m_value1 = value1;
	this->m_value2 = value2;
	this->m_value3 = value3;
	this->m_effecttype1 = option1;
	this->m_effecttype2 = option2;
	this->m_effecttype3 = option3;
	this->m_tick = GetTickCount();
	return 1;
}

void CEffect::Clear()
{
    this->m_effect = 0xFF;
	this->m_count = 0;
	this->m_value1 = 0;
	this->m_value2 = 0;
	this->m_value3 = 0;
	this->m_effecttype1 = 0;
	this->m_effecttype2 = 0;
	this->m_effecttype3 = 0;
	this->m_tick = 0;
}


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

