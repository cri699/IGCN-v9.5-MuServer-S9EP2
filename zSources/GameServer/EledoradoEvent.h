//////////////////////////////////////////////////////////////////////
// EledoradoEvent.h
// ------------------------------
// Decompiled by Deathway
// Date : 2007-05-09
// ------------------------------
#ifndef ELEDORADOEVENT_H
#define ELEDORADOEVENT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct ELEDORARO_MONSTER_POS
{
	WORD m_Type;	// 2
	BYTE m_MapNumber[4];	// 4
	BYTE m_Dis;	// 5
	BYTE m_X;	// 6
	BYTE m_Y;	// 7
	BYTE m_Dir;	// 8
	BYTE m_W;	// 9
	BYTE m_H;	// A
	WORD m_Count;
};

struct ELEDORADO_MONSTER_INFO
{
	int m_Index;

	ELEDORADO_MONSTER_INFO()
	{
		this->m_Index = -1;
	}
};

struct ELEDORADO_REGEN_TIME
{
	WORD m_Type;
	int m_RegenTime;
};

class CEledoradoEvent
{

public:

	CEledoradoEvent();
	virtual ~CEledoradoEvent();

	void ClearMonster();

	void Init();
	void SetEventState(int State);
	void Run();
	void RegenGoldGoblen();
	void RegenTitan();
	void RegenGoldDercon();
	void RegenDevilLizardKing();
	void RegenKantur();
	void RegenRabbit();
	void RegenDarkKnight();
	void RegenDevil();
	void RegenDarkKnightAida();
	void RegenCrust();
	void RegenSatiros();
	void RegenTwinTail();
	void RegenIronKnight();
	void RegenNeipin();
	void RegenGreatDragon();
	void CheckGoldDercon(int MapNumber);
	void SetMenualStart(BOOL bMenualStart){this->m_bMenualStart = bMenualStart;}	// line : 57
	void Start_Menual();
	void End_Menual();

	bool IsEledoradoMonster(int iIndex);

	int m_BossGoldDerconMapNumber[4];	// 44

	ELEDORARO_MONSTER_POS * GetMonsterPos(int iMonsterIndex);

private:

	BOOL GetBoxPosition(int mapnumber, int ax, int ay, int aw, int ah, short &mx, short &my);
	BYTE GetMapNumber(std::map<int, ELEDORARO_MONSTER_POS>::iterator Iter);

	int EventState;	// 4

	int m_BossTitanMapNumber;	// 1C
	int m_BossTitanMapX;	// 20
	int m_BossTitanMapY;	// 24
	int m_BossDevilLizardKingMapNumber;	// 28
	int m_BossDevilLizardKingMapX;	// 2C
	int m_BossDevilLizardKingMapY;	// 30
	int m_BossKanturMapNumber;	// 34
	int m_BossKanturMapX;	// 38
	int m_BossKanturMapY;	// 3C
	int m_bMenualStart;	// 40

	std::map<int, ELEDORARO_MONSTER_POS> m_vtMonsterPos;
	std::map<int, ELEDORADO_MONSTER_INFO> m_mapMonsterInfo;

	CRITICAL_SECTION m_MonCriti;

};

extern CEledoradoEvent gEledoradoEvent;

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

