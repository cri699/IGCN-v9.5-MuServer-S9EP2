//////////////////////////////////////////////////////////////////////
// MuunInfoMng.cpp
#include "StdAfx.h"
#include "MuunInfoMng.h"
#include "user.h"
#include "TLog.h"
#include "winutil.h"
#include "MuunOpt.h"

CMuunInfoMng::CMuunInfoMng()
{

}

CMuunInfoMng::~CMuunInfoMng()
{

}

bool CMuunInfoMng::LoadScriptMuunSystemInfo(char *lpszFileName)
{
	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(lpszFileName);

	if (res.status != pugi::status_ok)
	{
		g_Log.MsgBox("Error loading %s file (%s)", lpszFileName, res.description());
		return false;
	}

	for (int i = 0; i < MAX_MUUN_ITEM; i++)
	{
		this->m_CMuunInfo[i].Clear();
	}

	char szAddOptStartDate[13];
	char szAddOptEndDate[13];

	pugi::xml_node muunsys = file.child("MuunSystem");

	for (pugi::xml_node muun = muunsys.child("Muun"); muun; muun = muun.next_sibling())
	{
		int nIndex = muun.attribute("ID").as_int();
		int nItemType = muun.attribute("ItemCat").as_int();
		int nItemIndex = muun.attribute("ItemIndex").as_int();
		int nItemNum = ITEMGET(nItemType, nItemIndex);
		int nMuunType = muun.attribute("Type").as_int();
		int nMuunRank = muun.attribute("Rank").as_int();
		int	nOptIndex = muun.attribute("OptionIndex").as_int();
		int	nAddOptType = muun.attribute("AddOptionType").as_int();
		int nAddOptVal = muun.attribute("AddOptionValue").as_int();
		int nSkillDelayTime = muun.attribute("SkillDelayTime").as_int();

		strcpy(szAddOptStartDate, muun.attribute("OptionStartDate").as_string());
		strcpy(szAddOptEndDate, muun.attribute("OptionEndDate").as_string());

		int iEvolutionMuunType = muun.attribute("EvolutionItemCat").as_int();
		int iEvolutionMuunIndex = muun.attribute("EvolutionItemIndex").as_int();

		int iEvolutionMuunNum = -1;

		if (iEvolutionMuunType != -1)
			iEvolutionMuunNum = ITEMGET(iEvolutionMuunType, iEvolutionMuunIndex);

		if (nIndex < 0 || nIndex > MAX_MUUN_ITEM -1)
		{
			g_Log.Add("[MuunSystem][LoadScriptMuunSystemInfo] Invalid Index");
			g_Log.MsgBox("[MuunSystem] Invalid Index : %d", nIndex);

			return FALSE;
		}

		this->m_CMuunInfo[nIndex].SetIndex(nIndex);
		this->m_CMuunInfo[nIndex].SetItemNum(nItemNum);
		this->m_CMuunInfo[nIndex].SetMuunType(nMuunType);
		this->m_CMuunInfo[nIndex].SetMuunRank(nMuunRank);
		this->m_CMuunInfo[nIndex].SetOptIndex(nOptIndex);
		this->m_CMuunInfo[nIndex].SetAddOptType(nAddOptType);
		this->m_CMuunInfo[nIndex].SetAddOptVal(nAddOptVal);
		time_t nAddOptStart = this->ConvertStringToTime(szAddOptStartDate);
		time_t nAddOptEnd = this->ConvertStringToTime(szAddOptEndDate);
		this->m_CMuunInfo[nIndex].SetAddOptStart(nAddOptStart);
		this->m_CMuunInfo[nIndex].SetAddOptEnd(nAddOptEnd);
		this->m_CMuunInfo[nIndex].SetEvolutionMuunNum(iEvolutionMuunNum);
		this->m_CMuunInfo[nIndex].SetDelayTime(nSkillDelayTime);
	}

	return true;
}

bool CMuunInfoMng::LoadScriptMuunSystemOption(const char *lpszFileName)
{
	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(lpszFileName);

	if (res.status != pugi::status_ok)
	{
		g_Log.MsgBox("Error loading %s file (%s)", lpszFileName, res.description());
		return false;
	}

	pugi::xml_node muunsys = file.child("MuunSystem");
	CMuunOpt MuunOpt[60];

	for (pugi::xml_node muun = muunsys.child("Option"); muun; muun = muun.next_sibling())
	{
		int nIndex = muun.attribute("Index").as_int();
		int nOptType = muun.attribute("Type").as_int();

		int nMuunLvVal[6];
		nMuunLvVal[0] = muun.attribute("Value0").as_int();
		nMuunLvVal[1] = muun.attribute("Value1").as_int();
		nMuunLvVal[2] = muun.attribute("Value2").as_int();
		nMuunLvVal[3] = muun.attribute("Value3").as_int();
		nMuunLvVal[4] = muun.attribute("Value4").as_int();
		nMuunLvVal[5] = muun.attribute("Value5").as_int();

		int nConditionType = muun.attribute("ConditionType").as_int();
		int nConditionVal1 = muun.attribute("ConditionValue1").as_int();
		int nConditionVal2 = muun.attribute("ConditionValue2").as_int();

		if (nIndex < 0 || nIndex > 60 - 1)
		{
			g_Log.Add("[MuunSystem][LoadScriptMuunSystemOption] Invalid Index");
			g_Log.MsgBox("[MuunSystem] Invalid Index : %s %d", lpszFileName, nIndex);

			return 0;
		}

		MuunOpt[nIndex].SetOptType(nOptType);
		MuunOpt[nIndex].SetConditionType(nConditionType);
		MuunOpt[nIndex].SetConditionVal1(nConditionVal1);
		MuunOpt[nIndex].SetConditionVal2(nConditionVal2);
		MuunOpt[nIndex].SetMuunLvVal(nMuunLvVal);
	}

	this->InsertMuunOptionInfo(MuunOpt);
	return true;
}

bool CMuunInfoMng::InsertMuunOptionInfo(CMuunOpt *pCMuunOpt)
{
	int nMuunLvVal[6] = { 0, 0, 0, 0, 0, 0 };
	
	for (int i = 0; i < MAX_MUUN_ITEM; i++)
	{
		int nOptIndex = this->m_CMuunInfo[i].GetOptIndex();
		if (nOptIndex >= 0)
		{
			int nOptType = pCMuunOpt[nOptIndex].GetOptType();
			pCMuunOpt[nOptIndex].GetMuunLvVal(nMuunLvVal);

			this->m_CMuunInfo[i].SetOptType(nOptType);
			this->m_CMuunInfo[i].SetConditionType(pCMuunOpt[nOptIndex].GetConditionType());
			this->m_CMuunInfo[i].SetConditionVal1(pCMuunOpt[nOptIndex].GetConditionVal1());
			this->m_CMuunInfo[i].SetConditionVal2(pCMuunOpt[nOptIndex].GetConditionVal2());
			this->m_CMuunInfo[i].SetMuunLvVal(nMuunLvVal);
		}
	}

	return TRUE;
}

time_t CMuunInfoMng::ConvertStringToTime(const char *pchTime)
{
	int year, month, day;
	SYSTEMTIME stTime;
	memset(&stTime, 0x00, sizeof(stTime));

	sscanf(pchTime, "%d-%d-%d", &year, &month, &day);
	stTime.wYear = year;
	stTime.wMonth = month;
	stTime.wDay = day;

	time_t result;
	SystemTimeToUnixTime(&stTime, &result);

	return result;
}

CMuunInfo * CMuunInfoMng::GetMuunInfo(int iIndex)
{
	if (iIndex < 0 && iIndex > MAX_MUUN_ITEM - 1)
	{
		return NULL;
	}

	if (this->m_CMuunInfo[iIndex].GetIndex() == -1)
	{
		return NULL;
	}

	return &this->m_CMuunInfo[iIndex];
}

CMuunInfo * CMuunInfoMng::GetMuunItemNumToMuunInfo(int iMuunItemNum)
{
	for (int i = 0; i < MAX_MUUN_ITEM; i++)
	{
		if (this->m_CMuunInfo[i].GetItemNum() == iMuunItemNum)
		{
			return &this->m_CMuunInfo[i];
		}
	}

	return NULL;
}

int CMuunInfoMng::GetBeforeEvolutionMuunItemIndex(int iItemNum)
{
	for (int i = 0; i < MAX_MUUN_ITEM; i++)
	{
		if (this->m_CMuunInfo[i].GetEvolutionMuunNum() == iItemNum)
		{
			return this->m_CMuunInfo[i].GetItemNum();
		}
	}

	return -1;
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

