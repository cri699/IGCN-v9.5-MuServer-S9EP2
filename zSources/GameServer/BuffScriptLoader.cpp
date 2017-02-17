// BuffScriptLoader.cpp: implementation of the CBuffScriptLoader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ArcaBattle.h"
#include "BuffScriptLoader.h"
#include "zzzitem.h"

CBuffScriptLoader	g_BuffScript;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBuffScriptLoader::CBuffScriptLoader()
{

}

CBuffScriptLoader::~CBuffScriptLoader()
{

}

void CBuffScriptLoader::Initialize()
{
	iBuffCount = 0;
	iPeriodBuffCount = 0;

	for(int i = 0; i < MAX_BUFF_EFFECT-1; i++)
	{
		BuffData[i].BuffIndex = 0xFF;
		BuffData[i].BuffType = 0;
		BuffData[i].ClearType = 0;
		BuffData[i].NoticeType = 0;
		BuffData[i].BuffDescript[0] = 0;
	}

	for(int i = 0; i < MAX_BUFF_EFFECT-1; i++)
	{
		PeriodBuffInfo[i].wBuffIndex = 0xFF;
		PeriodBuffInfo[i].dwDuration = 0;
		PeriodBuffInfo[i].btEffectType1 = 0;
		PeriodBuffInfo[i].wEffectValue1 = 0;
		PeriodBuffInfo[i].btEffectType2 = 0;
		PeriodBuffInfo[i].wEffectValue2 = 0;
	}
}

bool CBuffScriptLoader::Load(char* pchFileName)
{
	Initialize();

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(pchFileName);

	if (res.status != pugi::status_ok)
	{
		return false;
	}
	
	BYTE btBuffIndex = 0;
	BYTE btBuffEffectType = 0;
	BYTE ItemType = 0;
	WORD ItemIndex = 0;
	BYTE btBuffType = 0;
	BYTE btNoticeType = 0;
	BYTE btClearType = 0;

	char chBuffName[60];
	char chBuffDescript[300];

	BYTE btEffectType1 = 0;
	WORD wEffectValue1 = 0;
	BYTE btEffectType2 = 0;
	WORD wEffectValue2 = 0;
	DWORD dwDuration = 0;

	pugi::xml_node main = file.child("BuffEffectManager");
	pugi::xml_node general = main.child("General");
	pugi::xml_node period = main.child("Period");

	for (pugi::xml_node buff = general.child("Buff"); buff; buff = buff.next_sibling())
	{
		btBuffIndex			= buff.attribute("Index").as_int();
		btBuffEffectType	= buff.attribute("EffectType").as_int();
		ItemType			= buff.attribute("ItemType").as_int();
		ItemIndex			= buff.attribute("ItemIndex").as_int();
		btBuffType			= buff.attribute("Type").as_int();
		btNoticeType		= buff.attribute("NoticeType").as_int();
		btClearType			= buff.attribute("ClearType").as_int();

		strcpy(chBuffName, buff.attribute("Name").as_string());
		strcpy(chBuffDescript, buff.attribute("Description").as_string());

		AddBuffEffectData(btBuffIndex, btBuffEffectType, ItemType, ItemIndex, chBuffName, btBuffType, btNoticeType, btClearType, chBuffDescript);
	}

	for (pugi::xml_node buff = period.child("Buff"); buff; buff = buff.next_sibling())
	{
		btBuffIndex			= buff.attribute("Index").as_int();
		btEffectType1		= buff.attribute("EffectType1").as_int();
		wEffectValue1		= buff.attribute("EffectValue1").as_int();
		btEffectType2		= buff.attribute("EffectType2").as_int();
		wEffectValue2		= buff.attribute("EffectValue2").as_int();
		dwDuration			= buff.attribute("Duration").as_int();

		AddPeriodBuffEffectInfo(btBuffIndex, btEffectType1, wEffectValue1, btEffectType2, wEffectValue2, dwDuration);
	}

	return true;
}

bool CBuffScriptLoader::AddBuffEffectData(BYTE btBuffIndex, BYTE btBuffEffectType, BYTE btItemType, WORD wItemIndex, char* pchName, BYTE btBuffType, BYTE btNoticeType, BYTE btClearType, char* pchDescript)
{
	BuffData[iBuffCount].BuffIndex = btBuffIndex;
	BuffData[iBuffCount].BuffEffectType = btBuffEffectType;
	BuffData[iBuffCount].ItemType = btItemType;
	BuffData[iBuffCount].ItemIndex = wItemIndex;
	BuffData[iBuffCount].BuffType = btBuffType;
	BuffData[iBuffCount].NoticeType = btNoticeType;
	BuffData[iBuffCount].ClearType = btClearType;

	memcpy(BuffData[iBuffCount].BuffName, pchName, sizeof(BuffData[iBuffCount].BuffName));
	memcpy(BuffData[iBuffCount].BuffDescript, pchDescript, sizeof(BuffData[iBuffCount].BuffDescript));
	
	iBuffCount++;
	return true;
}

bool CBuffScriptLoader::CheckVaildBuffEffect(BYTE btBuffIndex)
{
	if ( btBuffIndex < 0 || btBuffIndex > MAX_BUFF_EFFECT-1 )	return false;
	return true;
}

LPBUFF_EFFECT_DATE CBuffScriptLoader::GetBuffData(BYTE btBuffIndex)
{
	LPBUFF_EFFECT_DATE lpBuffData = NULL;

	if ( CheckVaildBuffEffect(btBuffIndex) == false )	return NULL;

	for ( int i = 0; i < MAX_BUFF_EFFECT-1; i++ )
	{
		if ( BuffData[i].BuffIndex == btBuffIndex )	return &BuffData[i];
	}

	return NULL;
}

LPBUFF_EFFECT_DATE CBuffScriptLoader::GetBuffDataFromItemCode(WORD wItemCode)
{
	for ( int i = 0; i < MAX_BUFF_EFFECT-1; i++ )
	{
		if ( ITEMGET(BuffData[i].ItemType, BuffData[i].ItemIndex) == wItemCode)	return &BuffData[i];
	}

	return NULL;
}

LPPERIOD_BUFF_EFFECT_INFO CBuffScriptLoader::GetPeriodBuffInfo(WORD wBuffIndex)
{
	for ( int i = 0; i < 254; ++i )
	{
		if ( this->PeriodBuffInfo[i].wBuffIndex == wBuffIndex )
			return &this->PeriodBuffInfo[i];
	}

	return 0;
}

void CBuffScriptLoader::AddPeriodBuffEffectInfo(WORD wBuffIndex, BYTE btEffectType1, WORD wEffectValue1, BYTE btEffectType2, WORD wEffectValue2, DWORD dwDuration)
{
	this->PeriodBuffInfo[this->iPeriodBuffCount].wBuffIndex = wBuffIndex;
	this->PeriodBuffInfo[this->iPeriodBuffCount].btEffectType1 = btEffectType1;
	this->PeriodBuffInfo[this->iPeriodBuffCount].wEffectValue1 = wEffectValue1;
	this->PeriodBuffInfo[this->iPeriodBuffCount].btEffectType2 = btEffectType1;
	this->PeriodBuffInfo[this->iPeriodBuffCount].wEffectValue2 = wEffectValue2;
	this->PeriodBuffInfo[this->iPeriodBuffCount].dwDuration = dwDuration;

	this->iPeriodBuffCount++;
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

