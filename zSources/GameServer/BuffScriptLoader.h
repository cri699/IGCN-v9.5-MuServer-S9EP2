// BuffScriptLoader.h: interface for the CBuffScriptLoader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BUFFSCRIPTLOADER_H__0639E406_8C5A_4328_808C_D7CEC8A7A711__INCLUDED_)
#define AFX_BUFFSCRIPTLOADER_H__0639E406_8C5A_4328_808C_D7CEC8A7A711__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_BUFF_EFFECT	255

typedef struct
{
	BYTE BuffIndex;
	BYTE BuffEffectType;
	BYTE ItemType;
	WORD ItemIndex;
	char BuffName[60];
	BYTE BuffType;
	BYTE NoticeType;
	BYTE ClearType;
	char BuffDescript[300];
}BUFF_EFFECT_DATE, *LPBUFF_EFFECT_DATE;

typedef struct
{
	WORD wBuffIndex;
	BYTE btEffectType1;
	WORD wEffectValue1;
	BYTE btEffectType2;
	WORD wEffectValue2;
	DWORD dwDuration;
}_tagPeriodBuffEffectInfo, * LPPERIOD_BUFF_EFFECT_INFO;

struct _tagPeriodBuffInfo
{
	WORD wBuffIndex;
	char btEffectType1;
	char btEffectType2;
	int lDuration;
	int lExpireDate;
};

class CBuffScriptLoader  
{
public:
	CBuffScriptLoader();
	virtual ~CBuffScriptLoader();

	bool Load(char* pchFileName);
	void Initialize();
	bool AddBuffEffectData(BYTE btBuffIndex, BYTE btBuffEffectType, BYTE btItemType, WORD wItemIndex, char* pchName, BYTE btBuffType, BYTE btNoticeType, BYTE btClearType, char* pchDescript);
	void AddPeriodBuffEffectInfo(WORD wBuffIndex, BYTE btEffectType1, WORD wEffectValue1, BYTE btEffectType2, WORD wEffectValue2, DWORD dwDuration);
	bool CheckVaildBuffEffect(BYTE btBuffIndex);
	LPBUFF_EFFECT_DATE GetBuffData(BYTE btBuffIndex);
	LPBUFF_EFFECT_DATE GetBuffDataFromItemCode(WORD wItemCode);
	LPPERIOD_BUFF_EFFECT_INFO GetPeriodBuffInfo(WORD wBuffIndex);

private:
	int iBuffCount;
	int iPeriodBuffCount;
	BUFF_EFFECT_DATE BuffData[MAX_BUFF_EFFECT-1];
	_tagPeriodBuffEffectInfo PeriodBuffInfo[MAX_BUFF_EFFECT-1];

};

extern CBuffScriptLoader	g_BuffScript;

#endif // !defined(AFX_BUFFSCRIPTLOADER_H__0639E406_8C5A_4328_808C_D7CEC8A7A711__INCLUDED_)


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

