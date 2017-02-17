//////////////////////////////////////////////////////////////////////
// ItemSeedSphere.cpp

#include "stdafx.h"
#include "ItemSeedSphere.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSeedItem::CSeedItem()
{
	Clear();
}

CSeedItem::~CSeedItem()
{

}

void CSeedItem::Clear()
{
	m_SeedIndex = 0;
	m_SeedType = SEED_TYPE_NONE;
	m_ItemLevel = 0;
	m_SeedOptionValueType = 0;

	for(int i = 0; i < 5; i++)
	{
		m_SeedOptionValue[i] = 0;
	}

	m_SeedExtractRate = 0;
	m_ItemCode = 0;
}

bool CSeedItem::IsMatched(CItem *lpItem)
{
	if(lpItem == NULL)	return false;

	if(lpItem->IsItem() == FALSE)	return false;

	if( m_ItemCode == lpItem->m_Type && 
		m_ItemLevel == lpItem->m_Level)	return true;
	
	return false;
}

void CSeedItem::SetOption(BYTE SeedIndex, BYTE SeedType, BYTE ItemLevel, BYTE SeedOptionValueType, WORD* SeedOptionValue, int ExtractRate)
{
	m_SeedIndex = SeedIndex;
	m_SeedType = SeedType;
	m_ItemLevel = ItemLevel;
	m_SeedOptionValueType = SeedOptionValueType;

	for(int i = 0; i < 5; i++)
	{
		m_SeedOptionValue[i] = SeedOptionValue[i];
	}

	m_SeedExtractRate = ExtractRate;

	switch ( SeedType )
	{
		case SEED_TYPE_FIRE:		m_ItemCode = ITEMGET(12,60);break;
		case SEED_TYPE_WATER:		m_ItemCode = ITEMGET(12,61); break;
		case SEED_TYPE_ICE:			m_ItemCode = ITEMGET(12,62); break;
		case SEED_TYPE_WIND:		m_ItemCode = ITEMGET(12,63); break;
		case SEED_TYPE_LIGHTNING:	m_ItemCode = ITEMGET(12,64); break;
		case SEED_TYPE_EARTH:		m_ItemCode = ITEMGET(12,65); break;
		case SEED_TYPE_UNIQUE:		m_ItemCode = ITEMGET(12,66); break;
	}
}

CSphereItem::CSphereItem()
{

}

CSphereItem::~CSphereItem()
{

}

void CSphereItem::Clear()
{
	m_SphereLevel = 0;
	m_ValueIncreaseRate = 0;
	m_ItemCode = 0;
}

void CSphereItem::SetOption(BYTE SphereLevel, int ValueIncreaseRate)
{
	m_SphereLevel = SphereLevel;
	m_ValueIncreaseRate = ValueIncreaseRate;

	switch ( SphereLevel )
	{
		case 1:m_ItemCode = ITEMGET(12,70); break;
		case 2:m_ItemCode = ITEMGET(12,71); break;
		case 3:m_ItemCode = ITEMGET(12,72); break;
		case 4:m_ItemCode = ITEMGET(12,73); break;
		case 5:m_ItemCode = ITEMGET(12,74); break;
	}
}

CSeedSphereItem::CSeedSphereItem()
{
	Clear();
}

CSeedSphereItem::~CSeedSphereItem()
{

}

void CSeedSphereItem::Clear()
{
	m_SeedSphereIndex = 0;
	m_SeedIndex = 0;
	m_SeedType = SEED_TYPE_NONE;
	m_ItemLevel = 0;
	m_SeedOptionValueType = 0;
	m_SeedOptionValue = 0;
	m_ItemCode = 0;
}

bool CSeedSphereItem::SetOption(CSeedItem *SeedItem, BYTE SphereLevel)
{
	if(SeedItem == NULL || SphereLevel == 0)	return false;

	m_SeedSphereIndex = SphereLevel * 50 + SeedItem->m_SeedIndex;
	m_SeedIndex = SeedItem->m_SeedIndex;
	m_SeedType = SeedItem->m_SeedType;
	m_ItemLevel = SeedItem->m_ItemLevel;
	m_SeedOptionValueType = SeedItem->m_SeedOptionValueType;
	m_SphereLevel = SphereLevel;
	m_SeedOptionValue = SeedItem->m_SeedOptionValue[SphereLevel-1];

	switch ( SeedItem->m_SeedType )
	{
		case SEED_TYPE_FIRE:		m_ItemCode = ITEMGET(12,100);	break;
		case SEED_TYPE_WATER:		m_ItemCode = ITEMGET(12,101);	break;
		case SEED_TYPE_ICE:			m_ItemCode = ITEMGET(12,102);	break;
		case SEED_TYPE_WIND:		m_ItemCode = ITEMGET(12,103);	break;
		case SEED_TYPE_LIGHTNING:	m_ItemCode = ITEMGET(12,104);	break;
		case SEED_TYPE_EARTH:		m_ItemCode = ITEMGET(12,105);	break;
		case SEED_TYPE_UNIQUE:										break;
		default:							return false;
	}

	m_ItemCode += (m_SphereLevel - 1) * 6;

	return true;
}

void CSeedSphereItem::AddOptionValue(WORD btOptionValue)
{
	if(m_SeedOptionValue + btOptionValue > (WORD)-2)	return;

	m_SeedOptionValue += btOptionValue;
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

