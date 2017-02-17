//////////////////////////////////////////////////////////////////////
// SkillUseTime.cpp
#include "stdafx.h"
#include "SkillUseTime.h"
#include "TLog.h"
#include "user.h"

CSkillUseTime g_SkillUseTime;

CSkillUseTime::CSkillUseTime(void)
{
	this->m_bFileLoad = false;
	this->m_bEnabled = false;
}

CSkillUseTime::~CSkillUseTime(void)
{
}

bool CSkillUseTime::LoadConfig(LPSTR lpFile)
{
	CIniReader ReadSkillTime(lpFile);

	this->m_bEnabled					= ReadSkillTime.ReadInt("SkillTimeCheck", "IsEnabled", 0);
	this->m_iIsDC						= ReadSkillTime.ReadInt("SkillTimeCheck", "IsDisconnectUser", 0);
	this->m_iNumberOfBadSkillUseDC		= ReadSkillTime.ReadInt("SkillTimeCheck", "NumberOfCastsToDisconnect", 0);
	this->m_bDebugMode					= ReadSkillTime.ReadInt("SkillTimeCheck", "DebugMode", 0);

	return true;
}

bool CSkillUseTime::LoadFile(LPSTR lpFile)
{
	if ( this->m_bEnabled == false )
	{
		return false;
	}

	this->m_bFileLoad = false;
	this->m_vtSkillTimeInfo.clear();

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(lpFile);

	if (res.status != pugi::status_ok)
	{
		g_Log.MsgBox("%s File Load Error (%s)", lpFile, res.description());
		return false;
	}

	SKILL_TIME_INFO m_SkillInfo;
	
	pugi::xml_node main = file.child("SkillUseTime");

	for (pugi::xml_node skill = main.child("Skill"); skill; skill = skill.next_sibling())
	{
		m_SkillInfo.iSkill = skill.attribute("Number").as_int();
		m_SkillInfo.iMinMagicSpeed = skill.attribute("MinMagicSpeed").as_int();
		m_SkillInfo.iMaxMagicSpeed = skill.attribute("MaxMagicSpeed").as_int();
		m_SkillInfo.iUseTime = skill.attribute("UseTime").as_int();

		this->m_vtSkillTimeInfo.push_back(m_SkillInfo);
	}

	this->m_bFileLoad = true;
	g_Log.Add("Loaded file %s (count:%d)", lpFile, this->m_vtSkillTimeInfo.size());

	return true;
}

bool CSkillUseTime::CheckSkillTime(LPOBJ lpObj, int iSkill)
{
	if ( this->m_bEnabled == false )
	{
		return true;
	}

	if ( this->m_bFileLoad == false )
	{
		return true;
	}

	if ( lpObj->Type != OBJ_USER )
	{
		return true;
	}

	ULONGLONG iTimeDiff = GetTickCount64() - lpObj->m_PlayerData->LastSkillUseTick;

	if ( this->m_bDebugMode )
	{
		g_Log.Add("[DEBUG] UseSkill (%d) Time(%d) MagicSpeed(%d)", iSkill, iTimeDiff, lpObj->m_MagicSpeed);
	}

	for(std::vector<SKILL_TIME_INFO>::iterator It = this->m_vtSkillTimeInfo.begin(); It != this->m_vtSkillTimeInfo.end(); It++)
	{
		if ( lpObj->m_MagicSpeed >= It->iMinMagicSpeed && lpObj->m_MagicSpeed <= It->iMaxMagicSpeed )
		{
			if ( iSkill == It->iSkill )
			{
				if ( iTimeDiff < It->iUseTime )
				{
					lpObj->m_PlayerData->LastSkillUseCount++;

					if ( this->m_iIsDC )
					{
						if ( lpObj->m_PlayerData->LastSkillUseCount >= this->m_iNumberOfBadSkillUseDC )
						{
							g_Log.AddC(TColor::Red, "[ANTI-HACK] [%s][%s][%s] Used skill too fast %d times -> Disconnect", lpObj->AccountID, lpObj->Name, lpObj->m_PlayerData->Ip_addr, lpObj->m_PlayerData->LastSkillUseCount);
							GSProtocol.GCSendDisableReconnect(lpObj->m_Index);
							//IOCP.CloseClient(lpObj->m_Index);
						}
					}

					return false;
				}
			}
		}
	}

	lpObj->m_PlayerData->LastSkillUseCount = 0;
	lpObj->m_PlayerData->LastSkillUseNumber = iSkill;
	lpObj->m_PlayerData->LastSkillUseTick = GetTickCount64();

	return true;
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

