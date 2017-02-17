// ItemAddOption.h: interface for the CItemAddOption class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEMADDOPTION_H__96FEC421_39C3_40DD_9D47_EE1D86DD8A21__INCLUDED_)
#define AFX_ITEMADDOPTION_H__96FEC421_39C3_40DD_9D47_EE1D86DD8A21__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_ITEM_ADD_OPTION	100

#define ADD_OPTION_SPEED	1
#define ADD_OPTION_ATTACK_DAMAGE	2
#define ADD_OPTION_DEFENSE	3
#define ADD_OPTION_LIFE	4
#define ADD_OPTION_MANA	5
#define ADD_OPTION_EXPERIENCE	6
#define ADD_OPTION_DROP_RATE	7
#define ADD_OPTION_DISABLE_EXPERIENCE 8
#define ADD_OPTION_STRENGTH	9
#define ADD_OPTION_DEXTERITY	10
#define ADD_OPTION_VITALITY	11
#define ADD_OPTION_ENERGY	12
#define ADD_OPTION_LEADERSHIP	13
#define ADD_OPTION_FORCE	14
#define ADD_OPTION_MAGIC	15
#define ADD_OPTION_MOVEABLE	16
#define ADD_OPTION_DARKLORD_CRITICALDMG 17
#define ADD_OPTION_DOT 18
#define ADD_OPTION_POISON 19
#define ADD_OPTION_ICE 20
#define ADD_OPTION_WIZARDDEFENSE 21
#define ADD_OPTION_MAGICDEF 22
#define ADD_OPTION_REDUCE_DEFENSE 23
#define ADD_OPTION_REFLECT 24
#define ADD_OPTION_SLEEP 25
#define ADD_OPTION_BLIND 26
#define ADD_OPTION_WIZARDDEFENSE2 28
#define ADD_OPTION_WEAKNESS 29
#define ADD_OPTION_INNOVATION 30
#define ADD_OPTION_BERSERKERMANA 31
#define ADD_OPTION_BERSERKERHP 32
#define ADD_OPTION_MAGICCIRCLE 33
#define ADD_OPTION_MAGICCIRCLE_STRENGTH 34
#define ADD_OPTION_ALLDAMAGE 51
#define ADD_OPTION_ALLDEFENSE 52
#define ADD_OPTION_LIFE_RECOVERY 53
#define ADD_OPTION_MANA_RECOVERY 54
#define ADD_OPTION_CRITICALDMG 55
#define ADD_OPTION_EXCDMG 56
#define ADD_OPTION_SANTA_DMG 57
#define ADD_OPTION_SANTA_DEF 58
#define ADD_OPTION_VULCANEXP 60
#define ADD_OPTION_VULCANDROP 61
#define ADD_OPTION_GUARDIAN 62
#define ADD_OPTION_ITEMPROTECT 63
#define ADD_OPTION_ZEN_DROP 64
#define ADD_OPTION_SD	66
#define ADD_OPTION_AG	67
#define ADD_OPTION_PARTYEXP 68
#define ADD_OPTION_VITALITY2 69
#define ADD_OPTION_ADDBP 70
#define ADD_OPTION_INCSTAT 71
#define ADD_OPTION_WEAKEN 72
#define ADD_OPTION_INNOVATE 73
#define ADD_OPTION_FIRE 74
#define ADD_OPTION_FROST 75
#define ADD_OPTION_TORNADO 76
#define ADD_OPTION_BIND 77
#define ADD_OPTION_DARKNESS 78

typedef struct ITEMEFFECT
{
	WORD wOptionType;
	WORD wEffectType1;
	WORD wEffectType2;
	int iItemNumber;
	int iItemType;
	int iItemIndex;
	int iEffectValue1;
	int iEffectValue2;
	int iEffectValidTime;

} ITEMEFFECT, * LPITEMEFFECT;

class CItemAddOption  
{
public:

	CItemAddOption();
	virtual ~CItemAddOption();

	void Load(PCHAR chFileName);
	void Initialize();

	bool IsValidEffect(int iItemNumber);
	LPITEMEFFECT GetItemEffectData(int iItemNumber);

private:

	LPITEMEFFECT SearchItemEffect(int iItemNumber);

	ITEMEFFECT m_ItemAddOption[100];
};

extern CItemAddOption	g_ItemAddOption;

#endif // !defined(AFX_ITEMADDOPTION_H__96FEC421_39C3_40DD_9D47_EE1D86DD8A21__INCLUDED_)


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

