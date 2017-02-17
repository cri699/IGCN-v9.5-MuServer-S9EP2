//////////////////////////////////////////////////////////////////////
// classdef.cpp
// ------------------------------
// Decompiled by Deathway
// Date : 2007-05-09
// ------------------------------

#include "StdAfx.h"
#include "classdef.h"
#include "user.h"
#include "GameMain.h"
#include "winutil.h"
// GS-N 0.99.60T 0x004A36F0
//	GS-N	1.00.18	JPN	0x004C14B0	-	Completed

classdef::classdef()
{

}

classdef::~classdef()
{
	return;
}

void classdef::GDReqClassDefData()
{
	PMSG_REQ_CLASSDEF pMsg;
	PHeadSetB((LPBYTE)&pMsg, 0x02, sizeof(pMsg));

	wsDataCli.DataSend((char *)&pMsg, pMsg.h.size);
}

void classdef::DGAnsClassDefData(PMSG_ANS_CLASSDEF *lpMsg)
{
	for (int i = 0; i < lpMsg->iCount; i++)
	{
		if (lpMsg->m_ClassData[i].Class != -1)
		{
			this->SetCharacter(lpMsg->m_ClassData[i].Class, lpMsg->m_ClassData[i].Str, lpMsg->m_ClassData[i].Dex, lpMsg->m_ClassData[i].Vit, lpMsg->m_ClassData[i].Energy,
				lpMsg->m_ClassData[i].Life, lpMsg->m_ClassData[i].Mana, lpMsg->m_ClassData[i].LevelLife, lpMsg->m_ClassData[i].LevelMana,
				lpMsg->m_ClassData[i].VitalityToLife, lpMsg->m_ClassData[i].EnergyToMana, lpMsg->m_ClassData[i].Leadership);
		}
	}
}

void classdef::SetCharacter(int Class,  int Str, int Dex, int Vit, int Energy, float Life, float Mana, float LevelLife, float LevelMana, float VitalityToLife, float EnergyToMana, int Leadership)
{
	this->DefClass[Class].Experience = 0;

	this->DefClass[Class].Strength = Str;
	this->DefClass[Class].Dexterity = Dex;
	this->DefClass[Class].Vitality = Vit;
	this->DefClass[Class].Energy = Energy;

	this->DefClass[Class].Life = Life;
	this->DefClass[Class].MaxLife = Life;
	this->DefClass[Class].LevelLife = LevelLife;
	this->DefClass[Class].Mana = Mana;
	this->DefClass[Class].MaxMana = Mana;
	this->DefClass[Class].LevelMana = LevelMana;

	this->DefClass[Class].VitalityToLife = VitalityToLife;
	this->DefClass[Class].EnergyToMana = EnergyToMana;

	this->DefClass[Class].Leadership = Leadership;

	this->SetEquipment(Class);
}

void classdef::SetEquipment(int Class)
{
	int def_type=0;
	int right_type=0;
	int n;

	for (n=0;n<MAX_PLAYER_EQUIPMENT+1;n++)
	{
		this->DefClass[Class].Equipment[n].Clear();
	}

	switch (Class)
	{
		case CLASS_KNIGHT:
			def_type = ItemGetNumberMake(1, 0);
			break;

		case CLASS_ELF:
			def_type = ItemGetNumberMake(4, 0);
			right_type = ItemGetNumberMake(4, 15);
			break;

		case CLASS_DARKLORD:
			def_type = ItemGetNumberMake(6, 0);
			right_type = ItemGetNumberMake(0, 1);
			break;

		case CLASS_MAGUMSA:
			def_type = ItemGetNumberMake(6, 0);
			right_type = ItemGetNumberMake(0, 1);
			break;
	}

	if ( Class != 0 && Class != 5 && Class != 6 )
	{
		if ( Class == CLASS_DARKLORD )
		{
			this->DefClass[Class].Equipment[0].m_Level = 0;
			this->DefClass[Class].Equipment[1].m_Level = 0;
			this->DefClass[Class].Equipment[0].Convert(right_type, 0, 0, 0, 0, 0, 0, 0, -1, 0, CURRENT_DB_VERSION);
			this->DefClass[Class].Equipment[1].Convert(def_type, 0, 0, 0, 0, 0, 0, 0, -1, 0, CURRENT_DB_VERSION);
		}
		else if ( Class == CLASS_ELF || Class == CLASS_MAGUMSA )
		{
			this->DefClass[Class].Equipment[0].m_Level = 0;
			this->DefClass[Class].Equipment[1].m_Level = 0;
			this->DefClass[Class].Equipment[0].Convert(right_type, 0, 0, 0, 0, 0, 0, 0, -1, 0, CURRENT_DB_VERSION);
			this->DefClass[Class].Equipment[1].Convert(def_type, 0, 0, 0, 0, 0, 0, 0, -1, 0, CURRENT_DB_VERSION);
		}
		else
		{
			this->DefClass[Class].Equipment[0].m_Level = 0;
			this->DefClass[Class].Equipment[0].Convert(right_type, 0, 0, 0, 0, 0, 0, 0, -1, 0, CURRENT_DB_VERSION);
		}
	}
}

int classdef::GetDefPoint(int char_class)
{
	if ( char_class < 0 || char_class > MAX_TYPE_PLAYER-1 )
	{
		return 0;
	}

	return this->DefClass[char_class].Strength + this->DefClass[char_class].Dexterity + this->DefClass[char_class].Vitality + this->DefClass[char_class].Energy + this->DefClass[char_class].Leadership;
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

