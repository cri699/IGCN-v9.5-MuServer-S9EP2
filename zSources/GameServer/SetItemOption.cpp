//////////////////////////////////////////////////////////////////////
// SetItemOption.cpp
// ------------------------------
// Decompiled by Deathway
// Date : 2007-05-09
// ------------------------------
// GS-N 0.99.60T 0x004E3170
//	GS-N	1.00.18	JPN	0x0050BB90	-	Completed

#include "stdafx.h"
#include "SetItemOption.h"
#include "TLog.h"
#include "LargeRand.h"
#include "LuckyItemManager.h"

CSetItemOption gSetItemOption;


CSetItemOption::CSetItemOption()
{
	return;
}

CSetItemOption::~CSetItemOption()
{
	return;
}

BOOL CSetItemOption::LoadTypeInfo(char* filename)
{
	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(filename);

	if (res.status != pugi::status_ok)
	{
		g_Log.MsgBox("Failed to load %s file (%s)", filename, res.description());
		return FALSE;
	}

	memset(this->m_SetItemTypeInfo, -1, sizeof(this->m_SetItemTypeInfo) );
	memset(&this->m_SetItemDropInfo, 0, sizeof(this->m_SetItemDropInfo) );
	
	for (int i=0; i<MAX_ITEMS; i++)
	{
		this->m_SetItemTypeInfo[i].IsLoad = FALSE;
	}

	for (int i=0; i<MAX_TYPE_ITEMS; i++)
	{
		this->m_SetItemDropInfo.IsItemFound[i] = FALSE;
	}

	pugi::xml_node main = file.child("SetItemType");
	pugi::xml_node droprate = main.child("DropRate");

	for (pugi::xml_node section = droprate.child("Section"); section; section = section.next_sibling())
	{
		int SectionIdx = section.attribute("Index").as_int();
		this->m_SetItemDropInfo.dwSectionDropRate[SectionIdx] = section.attribute("DropRate").as_int();
		this->m_SetItemDropInfo.TotalDropRate += this->m_SetItemDropInfo.dwSectionDropRate[SectionIdx];
	}

	for (pugi::xml_node section = main.child("Section"); section; section = section.next_sibling())
	{
		int SectionIdx = section.attribute("Index").as_int();

		for (pugi::xml_node item = section.child("Item"); item; item = item.next_sibling())
		{
			int ItemIdx = item.attribute("Index").as_int();
			LPSETITEM_TYPEINFO pointer = &this->m_SetItemTypeInfo[ITEMGET(SectionIdx, ItemIdx)];

			pointer->OptionIndex[0] = item.attribute("TierI").as_int();
			pointer->OptionIndex[1] = item.attribute("TierII").as_int();
			pointer->OptionIndex[2] = item.attribute("TierIII").as_int();
			pointer->OptionIndex[3] = item.attribute("TierIV").as_int();
			pointer->ChaosMixLevel[0] = item.attribute("MixLevelA").as_int();
			pointer->ChaosMixLevel[1] = item.attribute("MixLevelB").as_int();

			pointer->IsLoad = TRUE;
			this->m_SetItemDropInfo.IsItemFound[SectionIdx] = TRUE;
		}

		if (this->m_SetItemDropInfo.IsItemFound[SectionIdx] == FALSE)
		{
			this->m_SetItemDropInfo.TotalDropRate -= this->m_SetItemDropInfo.dwSectionDropRate[SectionIdx];
			this->m_SetItemDropInfo.dwSectionDropRate[SectionIdx] = 0;
		}
	}

	return TRUE;
}

BOOL CSetItemOption::LoadOptionInfo(LPSTR filename)
{
	char szTemp[128];
	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(filename);

	if (res.status != pugi::status_ok)
	{
		g_Log.MsgBox("Failed to load %s file (%s)", filename, res.description());
		return FALSE;
	}

	memset(&this->m_SetItemOptionInfo, 0, sizeof(this->m_SetItemOptionInfo));

	for (int i=0;i<MAX_SETITEM_OPTION;i++)
	{
		memset(&this->m_SetItemOptionInfo[i].SetOptionTable[0], -1, sizeof(this->m_SetItemOptionInfo[0].SetOptionTable) );
		memset(&this->m_SetItemOptionInfo[i].SetExPOptionTable, -1, sizeof(this->m_SetItemOptionInfo[0].SetExPOptionTable) );
		memset(&this->m_SetItemOptionInfo[i].SetFullOtionTable[0], -1, sizeof(this->m_SetItemOptionInfo[0].SetFullOtionTable) );
	}

	pugi::xml_node main = file.child("SetItemOption");

	for (pugi::xml_node setitem = main.child("SetItem"); setitem; setitem = setitem.next_sibling())
	{
		int number = setitem.attribute("Index").as_int();
		LPSETITEM_OPTIONINFO p = &this->m_SetItemOptionInfo[number];

		strcpy(p->Name, setitem.attribute("Name").as_string());

		for (int i=0;i<OPTION_TABLE_SIZE;i++)
		{
			sprintf(szTemp, "OptIdx1_%d", i+1);
			p->SetOptionTable[i][0] = setitem.attribute(szTemp).as_int();

			sprintf(szTemp, "OptVal1_%d", i+1);
			p->SetOptionTableValue[i][0] = setitem.attribute(szTemp).as_int();

			sprintf(szTemp, "OptIdx2_%d", i+1);
			p->SetOptionTable[i][1] = setitem.attribute(szTemp).as_int();
				
			sprintf(szTemp, "OptVal2_%d", i+1);
			p->SetOptionTableValue[i][1] = setitem.attribute(szTemp).as_int();

			if ( (p->SetOptionTable[i][0] != -1) || (p->SetOptionTable[i][1] != -1) )
			{
				p->SetOptionCount++;
			}
		}
			
		// Special Set Item Options

		for (int i=0;i<SPECIAL_OPTION_TABLE_SIZE;i++)
		{
			sprintf(szTemp, "SpecialOptIdx%d", i+1);
			p->SetExPOptionTable[i] = setitem.attribute(szTemp).as_int();

			sprintf(szTemp, "SpecialOptVal%d", i+1);
			p->SetExPOptionTableValue[i] = setitem.attribute(szTemp).as_int();

			if ( p->SetExPOptionTable[i] != -1)
			{
				p->SetOptionCount ++;
			}
		}
			
		// Full Type Set Item

		for (int i=0;i<FULL_OPTION_TABLE_SIZE;i++)
		{
			sprintf(szTemp, "FullOptIdx%d", i+1);
			p->SetFullOtionTable[i] = setitem.attribute(szTemp).as_int();

			sprintf(szTemp, "FullOptVal%d", i+1);
			p->SetFullOtionTableValue[i] = setitem.attribute(szTemp).as_int();
		}

		// Save Character Class Disponibility

		p->RequireClass[CLASS_WIZARD] = setitem.attribute("DarkWizard").as_int();
		p->RequireClass[CLASS_KNIGHT] = setitem.attribute("DarkKnight").as_int();
		p->RequireClass[CLASS_ELF] = setitem.attribute("FairyElf").as_int();
		p->RequireClass[CLASS_MAGUMSA] = setitem.attribute("MagicGladiator").as_int();
		p->RequireClass[CLASS_DARKLORD] = setitem.attribute("DarkLord").as_int();
		p->RequireClass[CLASS_SUMMONER] = setitem.attribute("Summoner").as_int();
		p->RequireClass[CLASS_RAGEFIGHTER] = setitem.attribute("RageFighter").as_int();
		p->RequireClass[CLASS_GROWLANCER] = setitem.attribute("GrowLancer").as_int();
			
		// Active SetItem
		p->IsLoad=TRUE;
	}

	return TRUE;
}

int CSetItemOption::IsSetItem(int itemnum)	
{
	if (itemnum<0 || itemnum > MAX_ITEMS-1)
	{
		g_Log.Add("IsSetItem Check error: (itemnum:%d)", itemnum);
		return 0;
	}
	return this->m_SetItemTypeInfo[itemnum].IsLoad ;
}


int CSetItemOption::GetOptionTableIndex(int itemnum, int setoption)	
{
	if (itemnum<0 || itemnum > MAX_ITEMS-1)
	{
		g_Log.Add("GetOptionTableIndex Check error: (itemnum:%d)", itemnum);
		return -1;
	}

	if (setoption != 1 && setoption != 2)
	{
		g_Log.Add("GetOptionTableIndex setoption Check error: (itemnum:%d)", itemnum);
		return -1;
	}

	if (this->m_SetItemTypeInfo[itemnum].IsLoad == FALSE)
	{
		return -1;
	}

	if (setoption == 0x10)
	{
		return this->m_SetItemTypeInfo[itemnum].OptionIndex[2];
	}

	else if (setoption == 0x20)
	{
		return this->m_SetItemTypeInfo[itemnum].OptionIndex[3];
	}

	return this->m_SetItemTypeInfo[itemnum].OptionIndex[setoption-1];
}


char * CSetItemOption::GetSetOptionName(int optionindex)
{
	if ( optionindex < 0 || optionindex > MAX_SETITEM_OPTION-1)
	{
		g_Log.Add("GetSetOptionName Check error: (itemnum:%d)", optionindex);
		return "";
	}
	if (this->m_SetItemOptionInfo[optionindex].IsLoad   ==0)
	{
		return "";
	}
	return &this->m_SetItemOptionInfo[optionindex].Name[0];
}



	
LPSTR CSetItemOption::GetSetOptionName(int itemnum, int settype)
{
	int optionindex;

	if (itemnum<0 || itemnum > MAX_ITEMS-1)
	{
		g_Log.Add("GetSetOptionName Check error: (itemnum:%d)", itemnum);
		return "";
	}

	if (this->m_SetItemTypeInfo[itemnum].IsLoad == FALSE)
	{
		return "";
	}
	
	optionindex= this->m_SetItemTypeInfo[itemnum].OptionIndex[settype-1];
	
	if (optionindex<0 || optionindex > MAX_SETITEM_OPTION-1)
	{	
		g_Log.Add("GetSetOptionName Check error: (itemnum:%d)", optionindex);
		return "";
	}

	if (this->m_SetItemOptionInfo[optionindex].IsLoad  == 0)
	{
		return "";
	}

	return &this->m_SetItemOptionInfo[optionindex].Name[0];
}

int CSetItemOption::GetSetOption(int optionindex, int optablenum, int& op1, int& op2, int& opvalue1,int& opvalue2, int Class, int ChangeUp)
{
	opvalue1=0;
	opvalue2=0;

	if (optionindex <0 || optionindex > MAX_SETITEM_OPTION-1)
	{
		g_Log.Add("GetSetOption Check error: (itemnum:%d)", optionindex);
		return 0;
	}

	if (this->m_SetItemOptionInfo[optionindex].IsLoad==0)
	{
		return 0;
	}

	if (optablenum >= this->m_SetItemOptionInfo[optionindex].SetOptionCount)
	{
		return 0;
	}
	int isRequireClass;

	isRequireClass=this->IsRequireClass(optionindex, Class, ChangeUp);

	if (optablenum <OPTION_TABLE_SIZE)
	{
		op1=this->m_SetItemOptionInfo[optionindex].SetOptionTable[optablenum][0];
		opvalue1=this->m_SetItemOptionInfo[optionindex].SetOptionTableValue[optablenum][0];

		if ( op1 > 183 )
		{
			if ( isRequireClass == 0 )
			{
				op1 = -1;
			}
		}

		if ( op1 != -1 )
		{
			op1 += 0xA0;
		}

		op2=this->m_SetItemOptionInfo[optionindex].SetOptionTable[optablenum][1];
		opvalue2=this->m_SetItemOptionInfo[optionindex].SetOptionTableValue[optablenum][1];

		if ( op2 > 183 )
		{
			if ( isRequireClass == 0 )
			{
				op2 = -1;
			}
		}

		if ( op2 != -1 )
		{
			op2 += 0xA0;
		}
	}
	else
	{
		op1=this->m_SetItemOptionInfo[optionindex].SetExPOptionTable[optablenum-OPTION_TABLE_SIZE];
		opvalue1=this->m_SetItemOptionInfo[optionindex].SetExPOptionTableValue[optablenum-OPTION_TABLE_SIZE];

		if ( op1 > 183 )
		{
			if ( isRequireClass == 0 )
			{
				op1 = -1;
			}
		}

		if ( op1 != -1 )
		{
			op1 += 0xA0;
		}

		op2 = -1;
	}

	return 1;
}

int CSetItemOption::GetMaxSetOptionCount(int optionindex)
{
	if (optionindex <0 || optionindex > MAX_SETITEM_OPTION-1)
	{
		g_Log.Add("GetMaxOptionCount Check error: (itemnum:%d)", optionindex);
		return -1;
	}

	if (this->m_SetItemOptionInfo[optionindex].IsLoad == 0)
	{
		return -1;
	}

	return this->m_SetItemOptionInfo[optionindex].SetOptionCount;
}

int CSetItemOption::GetGetFullSetOption(int optionindex, int& op1, int& op2, int& op3, int& op4, int& op5, int& opvalue1, int& opvalue2, int& opvalue3, int& opvalue4, int& opvalue5, int Class, int ChangeUp)
{
	opvalue1 = 0;
	opvalue2 = 0;
	opvalue3 = 0;
	opvalue4 = 0;
	opvalue5 = 0;

	if (optionindex <0 || optionindex > MAX_SETITEM_OPTION-1)
	{
		g_Log.Add("GetMaxOptionCount Check error: (itemnum:%d)", optionindex);
		return 0;
	}

	if (this->m_SetItemOptionInfo[optionindex].IsLoad == 0)
	{
		return 0;
	}

	BOOL isRequireClass=this->IsRequireClass(optionindex, Class, ChangeUp);
	op1 = this->m_SetItemOptionInfo[optionindex].SetFullOtionTable[0];
	opvalue1 = this->m_SetItemOptionInfo[optionindex].SetFullOtionTableValue[0];

	if ( op1 > 183 )
	{
		if ( isRequireClass == 0 )
		{
			op1 = -1;
		}
	}

	if ( op1 != -1 )
	{
		op1 += 0xA0;
	}

	op2=this->m_SetItemOptionInfo[optionindex].SetFullOtionTable[1];
	opvalue2=this->m_SetItemOptionInfo[optionindex].SetFullOtionTableValue[1];

	if ( op2 > 183 )
	{
		if ( isRequireClass == 0 )
		{
			op2 = -1;
		}
	}

	if ( op2 != -1 )
	{
		op2 += 0xA0;
	}

	op3=this->m_SetItemOptionInfo[optionindex].SetFullOtionTable[2];
	opvalue3=this->m_SetItemOptionInfo[optionindex].SetFullOtionTableValue[2];

	if ( op3 > 183 )
	{
		if ( isRequireClass == 0 )
		{
			op3 = -1;
		}
	}

	if ( op3 != -1 )
	{
		op3 += 0xA0;
	}

	op4=this->m_SetItemOptionInfo[optionindex].SetFullOtionTable[3];
	opvalue4=this->m_SetItemOptionInfo[optionindex].SetFullOtionTableValue[3];

	if ( op4 > 183 )
	{
		if ( isRequireClass == 0 )
		{
			op4 = -1;
		}
	}

	if ( op4 != -1 )
	{
		op4 += 0xA0;
	}

	op5=this->m_SetItemOptionInfo[optionindex].SetFullOtionTable[4];
	opvalue5=this->m_SetItemOptionInfo[optionindex].SetFullOtionTableValue[4];

	if ( op5 > 183 )
	{
		if ( isRequireClass == 0 )
		{
			op5 = -1;
		}
	}

	if ( op5 != -1 )
	{
		op5 += 0xA0;
	}

	return 1;
}

BOOL CSetItemOption::IsRequireClass(int optionindex, int Class, int ChangeUP)
{
	int requireclass;
	
	if (optionindex <0 || optionindex > MAX_SETITEM_OPTION-1)
	{
		g_Log.Add("GetMaxOptionCount Check error: (itemnum:%d)", optionindex);
		return 0;
	}

	if (this->m_SetItemOptionInfo[optionindex].IsLoad == 0)
	{
		return 0;
	}

	if ( Class <0 || Class > MAX_TYPE_PLAYER-1)
	{
		return 0;
	}

	requireclass= this->m_SetItemOptionInfo[optionindex].RequireClass[Class];

	if (requireclass == 0)
	{
		return 0;
	}

	if (requireclass > 1)
	{
		if (requireclass > ChangeUP+1)
		{
			return 0;
		}
	}
	return 1;
}


int CSetItemOption::CheckMixContition(int itemnum, int mixitemlevel)
{
	if (mixitemlevel == -1)
	{
		return 0;
	}

	if ( itemnum <0 || itemnum > MAX_ITEMS-1)
	{
		g_Log.Add("MakeSetOption Check error: (itemnum:%d)", itemnum);
		return 0;
	}

	if (this->m_SetItemTypeInfo[itemnum].IsLoad == 0)
	{
		return 0;
	}

	if (this->m_SetItemTypeInfo[itemnum].ChaosMixLevel[0] == mixitemlevel || this->m_SetItemTypeInfo[itemnum].ChaosMixLevel[1] == mixitemlevel)
	{
		return 1;
	}

	return 0;
}

int CSetItemOption::MakeSetOption(int itemnum, int mixitemlevel)
{
	if ( itemnum <0 || itemnum > MAX_ITEMS-1)
	{
		g_Log.Add("MakeSetOption Check error: (itemnum:%d)", itemnum);
		return 0;
	}

	if (this->m_SetItemTypeInfo[itemnum].IsLoad == FALSE)
	{
		return 0;
	}

	if (this->m_SetItemTypeInfo[itemnum].ChaosMixLevel[0] == this->m_SetItemTypeInfo[itemnum].ChaosMixLevel[1])
	{
		return (rand() % 2) +1;
	}
	else if ( this->m_SetItemTypeInfo[itemnum].ChaosMixLevel[0] == mixitemlevel )
	{
		return 1;
	}
	else if ( this->m_SetItemTypeInfo[itemnum].ChaosMixLevel[1] == mixitemlevel )
	{
		return 2;
	}

	return 0;
}

int CSetItemOption::GenRandomItemNum()
{
	DWORD dwRandDropRate = GetLargeRand() % this->m_SetItemDropInfo.TotalDropRate;

	int iLeft = this->m_SetItemDropInfo.TotalDropRate;
	int iRight = this->m_SetItemDropInfo.TotalDropRate;
	int iSectionIdx = -1;

	for (int i = 0; i < MAX_TYPE_ITEMS; i++)
	{
		iLeft -= this->m_SetItemDropInfo.dwSectionDropRate[i];

		if (iLeft <= dwRandDropRate && dwRandDropRate < iRight)
		{
			iSectionIdx = i;
			break;
		}
	}

	if (iSectionIdx == -1)
	{
		return FALSE;
	}

	int iItemIndex;

	do
	{
		int maxindex = ( g_MaxItemIndexOfEachItemType[iSectionIdx] <= 0 ) ?  MAX_SUBTYPE_ITEMS : g_MaxItemIndexOfEachItemType[iSectionIdx];
		int index = rand() % maxindex;
		iItemIndex = ITEMGET(iSectionIdx, index);

		if (g_LuckyItemManager.IsLuckyItemEquipment(iItemIndex) == true)
		{
			continue;
		}
		
		if ( this->IsSetItem(iItemIndex) != FALSE )
			return iItemIndex;
	}
	while ( true );

	return FALSE;
}

int CSetItemOption::GenSetOption(int itemnum)
{
	if ( itemnum < 0 || itemnum > MAX_ITEMS-1)
	{
		g_Log.AddC(TColor::Red, "GenSetType Check error: (itemnum:%d)", itemnum);
		return FALSE;
	}

	if ( this->m_SetItemTypeInfo[itemnum].IsLoad == FALSE )
		return FALSE;

	int iLinkCount = 0;
	int SetOption = 0;

	for (int i = 0; i < MAX_SETITEM_OPTION_INDEX; i++)
	{
		if (this->m_SetItemTypeInfo[itemnum].OptionIndex[i] != 0)
		{
			iLinkCount++;
		}
	}

	if (iLinkCount > 0)
	{
		int iLinkNum = rand() % iLinkCount;

		for (int i = 0; i < MAX_SETITEM_OPTION_INDEX; i++)
		{
			if (this->m_SetItemTypeInfo[itemnum].OptionIndex[iLinkNum] == 0)
			{
				if (this->m_SetItemTypeInfo[itemnum].OptionIndex[iLinkNum + 1])
				{
					iLinkNum++;
				}

				else if (this->m_SetItemTypeInfo[itemnum].OptionIndex[iLinkNum + 2])
				{
					iLinkNum += 2;
				}

				else if (this->m_SetItemTypeInfo[itemnum].OptionIndex[iLinkNum + 3])
				{
					iLinkNum += 3;
				}
			}
		}

		if (iLinkNum == 0 || iLinkNum == 1)
		{
			SetOption = 1 << iLinkNum;
		}

		else if (iLinkNum == 2 || iLinkNum == 3)
		{
			SetOption = 1 << (iLinkNum + 2);
		}
	}

	if (rand() % 100 < 80)
	{
		SetOption |= 4;
	}

	else
	{
		SetOption |= 8;
	}

	return SetOption;
}

int CSetItemOption::GenRandomItemNumByCat(int iCat)
{
	if (iCat < 0 || iCat >= MAX_TYPE_ITEMS)
	{
		g_Log.AddC(TColor::Red, "GenSetTypeByOptionIndex Check error: (iCat:%d)", iCat);
		return FALSE;
	}

	if (this->m_SetItemDropInfo.IsItemFound[iCat] == FALSE)
	{
		g_Log.AddC(TColor::Red, "GenSetTypeByOptionIndex Check error: (iCat:%d) - No Set Item found", iCat);
		return FALSE;
	}

	int iItemIndex;

	do
	{
		int maxindex = (g_MaxItemIndexOfEachItemType[iCat] <= 0) ? MAX_SUBTYPE_ITEMS : g_MaxItemIndexOfEachItemType[iCat];
		int index = rand() % maxindex;
		iItemIndex = ITEMGET(iCat, index);

		if (g_LuckyItemManager.IsLuckyItemEquipment(iItemIndex) == true)
		{
			continue;
		}

		if (this->IsSetItem(iItemIndex) != FALSE)
			return iItemIndex;
	}
	while (true);

	return FALSE;
}

int CSetItemOption::LuckyItemGenSetOption(int itemnum, int * piAddSetOptRate)
{
	if (itemnum < 0 || itemnum > MAX_ITEMS - 1)
	{
		g_Log.AddC(TColor::Red, "GenSetType Check error: (itemnum:%d)", itemnum);
		return false;
	}

	if (!this->m_SetItemTypeInfo[itemnum].IsLoad)
	{
		return false;
	}

	int iLinkCount = 0;
	int SetOption = 0;

	for (int i = 0; i < MAX_SETITEM_OPTION_INDEX; i++)
	{
		if (this->m_SetItemTypeInfo[itemnum].OptionIndex[i] != 0)
		{
			iLinkCount++;
		}
	}

	if (iLinkCount > 0)
	{
		int iLinkNum = rand() % iLinkCount;

		for (int i = 0; i < MAX_SETITEM_OPTION_INDEX; i++)
		{
			if (this->m_SetItemTypeInfo[itemnum].OptionIndex[iLinkNum] == 0)
			{
				if (this->m_SetItemTypeInfo[itemnum].OptionIndex[iLinkNum + 1])
				{
					iLinkNum++;
				}

				else if (this->m_SetItemTypeInfo[itemnum].OptionIndex[iLinkNum + 2])
				{
					iLinkNum += 2;
				}

				else if (this->m_SetItemTypeInfo[itemnum].OptionIndex[iLinkNum + 3])
				{
					iLinkNum += 3;
				}
			}
		}

		if (iLinkNum == 0 || iLinkNum == 1)
		{
			SetOption = 1 << iLinkNum;
		}

		else if (iLinkNum == 2 || iLinkNum == 3)
		{
			SetOption = 1 << (iLinkNum + 2);
		}
	}

	DWORD dwRand = GetLargeRand() % 1000000;

	if (dwRand <= piAddSetOptRate[0])
	{
		SetOption |= 4;
	}
	else if (dwRand <= piAddSetOptRate[0] + piAddSetOptRate[1])
	{
		SetOption |= 8;
	}

	return SetOption;
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

