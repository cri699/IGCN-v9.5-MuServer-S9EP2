//////////////////////////////////////////////////////////////////////
// AntiSpeed.h
#ifndef ANTI_SPEED_H
#define ANTI_SPEED_H
#include "user.h"
//#define USE_NEW_AH
#define ATTACK_TOLERANCE 3

#include <queue>

/*
class CAttackBase{
public:
	bool m_InUse;

	virtual void Process() = 0;
};

class CAttackMelee : public CAttackBase{
	OBJECTSTRUCT * m_TargetObj;
	OBJECTSTRUCT * m_Obj;
public:
	CAttackMelee(OBJECTSTRUCT * lpObj, OBJECTSTRUCT * lpTargetObj);
	void Process();
};

class CAttackMagic : public CAttackBase{
	OBJECTSTRUCT * m_Obj;
	BYTE* m_Msg;
public:
	CAttackMagic(OBJECTSTRUCT * lpObj, BYTE* pmsg, int len);
	~CAttackMagic();
	void Process();
};

class CAttackRange : public CAttackBase{
	OBJECTSTRUCT * m_Obj;
	BYTE* m_Msg;
	int m_Type;
public:
	CAttackRange(OBJECTSTRUCT * lpObj, BYTE* pmsg, int len, int type);
	~CAttackRange();

	void Process();
};
*/

enum ATTACK_TYPE{
	ATTACK_MELEE = 0,
	ATTACK_MAGIC = 1,
	ATTACK_RANGE_OLD = 2,
	ATTACK_RANGE_NEW = 3,
	ATTACK_RAGE_FIGHTER = 4,
};

class CAttackMsg{
	OBJECTSTRUCT * m_Obj;
	BYTE* m_Msg;
	int m_Len;
	int m_Type;
public:
	CAttackMsg(int aIndex, BYTE* pmsg, int len, int type);
	~CAttackMsg();

	void Process();
};

class CAttackHandler{
public:
	CAttackMsg* m_pMsg;
};

class CAttackQueue{
private:
	int aIndex;
	std::queue<CAttackMsg*> m_Queue;
	

	void CheckSize();
	void Clear();
public:
	int m_SwordDelay;
	int m_HandDelay;
	int m_MagicDelay;
	
	bool m_LastFrameAttack;

	CAttackQueue(int aIndex);
	~CAttackQueue();
//	void Push(OBJECTSTRUCT * lpTargetObj);
//	void Push(unsigned char* msg, int len);
	void Push(unsigned char* msg, int len, int type);

	void ProcessQueue();
	static bool ThreadActive;
	static VOID AttackQueueProc();
	
	CRITICAL_SECTION m_CritQueue;
};

class CAntiSpeed{
private:
	std::queue<CAttackHandler*> m_Queue;
public:
	void ProcessQueue();

	static bool GlobalThreadActive;
	static VOID GlobalQueueProc();
};

extern CAntiSpeed* g_AntiSpeed;

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

