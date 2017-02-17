//////////////////////////////////////////////////////////////////////
// ItemSeedSphere.h
#include "zzzitem.h"

enum
{
	SEED_TYPE_NONE = 0x0,
	SEED_TYPE_FIRE = 0x1,
	SEED_TYPE_WATER = 0x2,
	SEED_TYPE_ICE = 0x3,
	SEED_TYPE_WIND = 0x4,
	SEED_TYPE_LIGHTNING = 0x5,
	SEED_TYPE_EARTH = 0x6,
	SEED_TYPE_UNIQUE = 0x7,
	SEED_TYPE_MAX = 0x8,
};

class CSeedItem  
{
public:
	CSeedItem();
	~CSeedItem();
	
	int GetItemCode(); //function without a body
	bool IsMatched(CItem* lpItem);
	void SetOption(BYTE SeedIndex, BYTE SeedType, BYTE ItemLevel, BYTE SeedOptionValueType, WORD* SeeedOptionValue, int ExtractRate);
	void Clear();

public:
	BYTE m_SeedIndex;
	BYTE m_SeedType;
	BYTE m_ItemLevel;
	BYTE m_SeedOptionValueType;
	WORD m_SeedOptionValue[5];
	int m_ItemCode;
	int m_SeedExtractRate;
};

class CSphereItem  
{
public:
	CSphereItem();
	~CSphereItem();
	
	int GetItemCode(); //function without a body
	void SetOption(BYTE SphereLevel, int ValueIncreaseRate);
	void Clear();

public:
	BYTE m_SphereLevel;
	int m_ValueIncreaseRate;
	int m_ItemCode;
};

class CSeedSphereItem  
{
public:
	CSeedSphereItem();
	~CSeedSphereItem();
	
	bool SetOption(CSeedItem* SeedItem, BYTE SphereLevel);
	void AddOptionValue(WORD btOptionValue);
	void Clear();

	int GetItemCode()
	{
		return m_ItemCode;
	}

public:
	BYTE m_SeedSphereIndex;
	BYTE m_SeedIndex;
	BYTE m_SeedType;
	int m_ItemCode;
	BYTE m_ItemLevel;
	BYTE m_SeedOptionValueType;
	WORD m_SeedOptionValue;
	BYTE m_SphereLevel;
};


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

