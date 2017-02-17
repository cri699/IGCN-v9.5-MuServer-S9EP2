//////////////////////////////////////////////////////////////////////
// MagicDamage.cpp
#include "stdafx.h"
#include "MagicDamage.h"
#include "TLog.h"
#include "GameMain.h"
#include "configread.h"
#include "MasterLevelSkillTreeSystem.h"

// GS-N 0.99.60T 0x0046B890 - Completed
//	GS-N	1.00.18	JPN	0x00482100	-	Completed

CMagicDamage MagicDamageC;

CMagicDamage::CMagicDamage()
{
	return;
}

CMagicDamage::~CMagicDamage()
{
	return;
}

void CMagicDamage::Init()
{
	for ( int n=0; n< MAX_SKILL;n++)
	{
		this->m_Damage[n] = -1;
	}
}

void CMagicDamage::LogSkillList(char * filename)
{
	this->Init();

	pugi::xml_document file;
	pugi::xml_parse_result res = file.load_file(filename);

	if (res.status != pugi::status_ok)
	{
		g_Log.MsgBox("%s load status - fail, error (%s)", filename, res.description());
		return;
	}

	pugi::xml_node main = file.child("SkillList");

	int number;
	char name[50];
	int requireLevel;
	int damage;
	int mana;
	int bp=0;
	int dis;
	int rEnergy;
	int iSkillType;
	int rLeadership;
	int rDelay;
	int attr;
	int iSkillUseType;
	int iSkillBrand;
	int iKillCount;
	int RequireStatus[MAX_REQ_SKILL_STATUS] = {0};
	BYTE RequireClass[MAX_TYPE_PLAYER];
	WORD nSkillRank;
	WORD nSkillGroup;
	WORD nRequireMLPoint;
	WORD nHP;
	WORD nSD;
	WORD wKeepTime;
	WORD nRequireStr;
	WORD nRequireDex;
	BYTE btIconNum;
	BYTE btItemSkill;
	BYTE btIsDamage;
	BYTE btBufIndex;

	for (pugi::xml_node skill = main.child("Skill"); skill; skill = skill.next_sibling())
	{
		number			= skill.attribute("Index").as_int();

		strcpy(name, skill.attribute("Name").as_string());

		requireLevel		= skill.attribute("ReqLevel").as_int();
		damage				= number < 300 ? skill.attribute("Damage").as_int() : skill.attribute("STID").as_int();
		mana				= skill.attribute("ManaUsage").as_int();
		bp					= skill.attribute("BPUsage").as_int();
		dis					= skill.attribute("Distance").as_int();
		rDelay				= skill.attribute("Delay").as_int();
		rEnergy				= skill.attribute("ReqEnergy").as_int();
		rLeadership			= skill.attribute("ReqCommand").as_int();

		if (g_ConfigRead.SeasonX == false)
		{
			attr = skill.attribute("Attribute").as_int();
		}

		else
		{
			attr = skill.attribute("Element").as_int();
		}

		iSkillType			= skill.attribute("Type").as_int();
		iSkillUseType		= skill.attribute("UseType").as_int();
		iSkillBrand			= skill.attribute("Brand").as_int();
		iKillCount			= skill.attribute("KillCount").as_int();

		RequireStatus[0]	= skill.attribute("ReqStatus0").as_int();
		RequireStatus[1]	= skill.attribute("ReqStatus1").as_int();
		RequireStatus[2]	= skill.attribute("ReqStatus2").as_int();

		RequireClass[0]		= skill.attribute("DarkWizard").as_int();
		RequireClass[1]		= skill.attribute("DarkKnight").as_int();
		RequireClass[2]		= skill.attribute("FairyElf").as_int();
		RequireClass[3]		= skill.attribute("MagicGladiator").as_int();
		RequireClass[4]		= skill.attribute("DarkLord").as_int();
		RequireClass[5]		= skill.attribute("Summoner").as_int();
		RequireClass[6]		= skill.attribute("RageFighter").as_int();
		RequireClass[7]		= skill.attribute("GrowLancer").as_int();

		nSkillRank			= skill.attribute("Rank").as_int();
		nSkillGroup			= skill.attribute("Group").as_int();
		nRequireMLPoint		= skill.attribute("ReqMLPoint").as_int();
		nHP					= skill.attribute("HP").as_int();
		nSD					= skill.attribute("SD").as_int();
		wKeepTime			= skill.attribute("Duration").as_int();
		nRequireStr			= skill.attribute("ReqStrength").as_int();
		nRequireDex			= skill.attribute("ReqDexterity").as_int();
		btIconNum			= skill.attribute("IconNumber").as_int();
		btItemSkill			= skill.attribute("ItemSkill").as_int();
		btIsDamage			= skill.attribute("isDamage").as_int();
		btBufIndex			= skill.attribute("BuffIndex").as_int();

		this->Set(name, number,damage, requireLevel, mana, bp, dis, attr, iSkillType, RequireClass, rEnergy, rLeadership, rDelay, btItemSkill, btIsDamage, btBufIndex);
		this->SetEx(number, iSkillUseType, iSkillBrand, iKillCount, RequireStatus);
		this->SetMasterLevelSkillInfo(number, nSkillRank, nSkillGroup, nRequireMLPoint, nHP, nSD, wKeepTime, nRequireStr, nRequireDex);
		DefMagicInf[number].Set(number, 0);
	}

	g_Log.Add("[%s] Skill information data load complete", filename);
}

void CMagicDamage::SetMasterLevelSkillInfo(int iSkill, int iSkillRank, int iSkillGroup, int iRequireMLPoint, int iHP, int iSD, WORD wKeepTime, int iRequireStr, int iRequireDex)
{
	if ( iSkill < 0 || iSkill > MAX_SKILL -1 )
	{
		g_Log.MsgBox("Skill limit error: (%s %d), ID: %d", __FILE__, __LINE__, iSkill);
		return;
	}

	if ( this->m_Damage[iSkill] == -1 )
	{
		g_Log.MsgBox("Damage value is already set (%s %d)", __FILE__, __LINE__);
		return;
	}

	this->m_iSkillRank[iSkill] = iSkillRank;
	this->m_iSkillGroup[iSkill] = iSkillGroup;
	this->m_nRequirePoint[iSkill] = iRequireMLPoint;
	this->m_iHP[iSkill] = iHP;
	this->m_iSD[iSkill] = iSD;
	this->m_wKeepTime[iSkill] = wKeepTime;
	this->m_iRequireStr[iSkill] = iRequireStr;
	this->m_iRequireDex[iSkill] = iRequireDex;
}

void CMagicDamage::SetEx(int iSkill, int iSkillUseType, int iSkillBrand, int iKillCount, int * pReqStatus)
{
	if ( iSkill < 0 || iSkill > MAX_SKILL -1 )
	{
		g_Log.MsgBox("Skill limit error: (%s %d), ID: %d", __FILE__, __LINE__, iSkill);
		return;
	}

	if ( this->m_Damage[iSkill] == -1 )
	{
		g_Log.MsgBox("Damage value is already set (%s %d)", __FILE__, __LINE__);
		return;
	}

	this->m_iSkillUseType[iSkill] = iSkillUseType;
	this->m_iSkillBrand[iSkill] = iSkillBrand;
	this->m_iKillCount[iSkill] = iKillCount;
	memcpy(this->m_iRequireStatus[iSkill], pReqStatus, sizeof(this->m_iRequireStatus[iSkill]));	// #error Deathway Fix
}

void CMagicDamage::Set(char* name, int skill, int damage,  int rlevel,  int mana,  int bp,  int dis, int Attr,  int iSkillType, unsigned char* ReqClass, int rEnergy, int rLeadership, int rdelay, BYTE byItemSkill, BYTE byIsDamage, BYTE byBufIndex)
{
	if ( skill < 0 || skill > MAX_SKILL -1 )
	{
		g_Log.MsgBox("Skill limit error: (%s %d), ID: %d", __FILE__, __LINE__, skill);
		return;
	}

	if ( this->m_Damage[skill] != -1 )
	{
		g_Log.MsgBox("Damage value is already set (%s %d)", __FILE__, __LINE__);
		return;
	}

	strcpy(this->m_Name[skill], name);
	this->m_Damage[skill] = damage;
	this->m_rLevel[skill] = rlevel;
	this->m_Mana[skill] = mana;
	this->m_Distance[skill] = dis;
	this->m_RequireLeadership[skill] = rLeadership;
	this->m_Delay[skill] = rdelay;
	this->m_BrainPower[skill] = bp;
	this->m_Attr[skill] = Attr;
	this->m_iSkillType[skill] = iSkillType;
	memcpy(this->m_RequireClass[skill], ReqClass, sizeof(this->m_RequireClass[0]));
	this->m_RequireEnergy[skill] = (rEnergy * rlevel * 4) / 100 + 20;

	if ( this->m_RequireEnergy[skill] <= 0 )
	{
		this->m_RequireEnergy[skill] = 0;
	}

	this->m_byItemAttachedSkill[skill] = byItemSkill;
	this->m_byIsDamage[skill] = byIsDamage;
	this->m_byBufIndex[skill] = byBufIndex;

	// Set Level Requirements for Skills
	this->m_RequireLevel[skill] = 0;	// Required Level to use the skill

	if ( skill == 41 )	// Twisting Slash
	{
		this->m_RequireLevel[skill] = 80;
	}
	else if ( skill == 42 )	// Rageful Blow
	{
		this->m_RequireLevel[skill] = 170;
	}
	else if ( skill == 43 )	// Death Stab
	{
		this->m_RequireLevel[skill] = 160;
	}
	else if ( skill == 47 )	// Impale
	{
		this->m_RequireLevel[skill] = 28;
	}
	else if ( skill == 48 )	// Inner Strength
	{
		this->m_RequireLevel[skill] = 120;
	}
	else if ( skill == 52 )	
	{
		this->m_RequireLevel[skill] = 130;
	}

	if ( skill == 30 )
	{
		this->m_RequireEnergy[skill] = 30;
	}
	else if ( skill == 31 )
	{
		this->m_RequireEnergy[skill] = 60;
	}
	else if ( skill == 32 )
	{
		this->m_RequireEnergy[skill] = 90;
	}
	else if ( skill == 33 )
	{
		this->m_RequireEnergy[skill] = 130;
	}
	else if ( skill == 34 )
	{
		this->m_RequireEnergy[skill] = 170;
	}
	else if ( skill == 35 )
	{
		this->m_RequireEnergy[skill] = 210;
	}
	else if ( skill == 36 )
	{
		this->m_RequireEnergy[skill] = 300;
	}
	else if ( skill == 41 )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 42 )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 43 )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 47 )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 48 )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 49  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 55  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 51  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 52  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 24  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 17 )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 18 )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 19 )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 20  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 21  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 22  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 23  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 25  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 56  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 60  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 44  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 45  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 46  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 57  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 73  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 74  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 67  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 68  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 69  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 70  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 71  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 72  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 76  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 77  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 78  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 79  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 62  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 214  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 215  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 216  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 217  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 219  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 220  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 223 )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 224  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 221  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 222  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 225  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 230  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 218  )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 232 )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 233 )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 234 )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 235 )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 236 )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 237 )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 238 )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 239 )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 260 )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 261 )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 262 )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 263 )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 264 )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 265 )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 266 )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 267 )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 268 )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 269 )
	{
		this->m_RequireEnergy[skill] = 0;
	}
	else if ( skill == 270 )
	{
		this->m_RequireEnergy[skill] = 0;
	}
}



int CMagicDamage::Get(int skill)
{
	if ( skill < 0 || skill > MAX_SKILL-1 )
	{
		g_Log.Add("Skill limit error: (%s %d), ID: %d", __FILE__, __LINE__, skill);
		return this->m_Damage[0];
	}

	if ( this->m_Damage[skill] == -1 )
	{
		g_Log.Add("Damage value doesn't exist (%s %d)", __FILE__, __LINE__);
		return this->m_Damage[0];
	}

	return this->m_Damage[skill];
}




int CMagicDamage::SkillGet(int skill)
{
	if ( skill < 0 || skill > MAX_SKILL-1 )
	{
		g_Log.Add("Skill limit error: (%s %d), ID: %d", __FILE__, __LINE__, skill);
		return this->m_Damage[0];
	}

	if ( this->m_Damage[skill] == -1 )
	{
		g_Log.Add("Damage value doesn't exist (%s %d)", __FILE__, __LINE__);
		return this->m_Damage[0];
	}

	return this->m_Damage[skill];
}



int CMagicDamage::SkillGetMana(int skill)
{
	if ( skill < 0 || skill > MAX_SKILL-1 )
	{
		g_Log.Add("Skill limit error: (%s %d), ID: %d", __FILE__, __LINE__, skill);
		return this->m_Mana[0];
	}

	if ( this->m_Damage[skill] == -1 )
	{
		g_Log.Add("Damage value doesn't exist (%s %d)", __FILE__, __LINE__);
		return this->m_Damage[0];
	}

	return this->m_Mana[skill];
}




int CMagicDamage::SkillGetBP(int skill)
{
	if ( skill < 0 || skill > MAX_SKILL-1 )
	{
		g_Log.Add("Skill limit error: (%s %d), ID: %d", __FILE__, __LINE__, skill);
		return 0;
	}

	if ( this->m_Damage[skill] == -1 )
	{
		g_Log.Add("Damage value doesn't exist (%s %d)", __FILE__, __LINE__);
		return 0;
	}

	return this->m_BrainPower[skill];
}





int CMagicDamage::SkillGetRequireEnergy(int skill)
{
	if ( skill < 0 || skill > MAX_SKILL-1 )
	{
		g_Log.Add("Skill limit error: (%s %d), ID: %d", __FILE__, __LINE__, skill);
		return -1;
	}

	if ( this->m_RequireEnergy[skill] == -1 )
	{
		g_Log.Add("Damage value doesn't exist (%s %d)", __FILE__, __LINE__);
		return -1;
	}

	return this->m_RequireEnergy[skill];
}





BOOL CMagicDamage::SkillGetRequireClass(int Cclass, int ChangeUP,  int skill)
{
	if ( skill < 0 || skill > MAX_SKILL-1 )
	{
		g_Log.Add("Skill limit error: (%s %d), ID: %d", __FILE__, __LINE__, skill);
		return -1;
	}

	if ( Cclass < 0 || Cclass > MAX_TYPE_PLAYER-1 )
	{
		g_Log.Add("Damage value doesn't exist (%s %d)", __FILE__, __LINE__);
		return -1;
	}

	int requireclass = this->m_RequireClass[skill][Cclass];

	if ( requireclass == 0 )
	{
		return FALSE;
	}

	if ( requireclass > 1 )
	{
		if ( requireclass > (ChangeUP+1) )
		{
			return FALSE;
		}
	}

	return TRUE;
}




int CMagicDamage::GetSkillAttr(int skill)
{
	if ( skill < 0 || skill > MAX_SKILL-1 )
	{
		return -1;
	}

	return this->m_Attr[skill];
}




int CMagicDamage::CheckSkillAttr(int skill, int attr)
{
	if ( skill < 0 || skill > MAX_SKILL-1 )
	{
		return FALSE;
	}

	return this->m_Attr[skill];
}



int CMagicDamage::GetskillRequireLeadership(int skill)
{
	if ( skill < 0 || skill > MAX_SKILL-1 )
	{
		g_Log.Add("Skill limit error: (%s %d), ID: %d", __FILE__, __LINE__, skill);
		return -1;
	}

	if ( this->m_RequireLeadership[skill] == -1 )
	{
		g_Log.Add("Damage value doesn't exist (%s %d)", __FILE__, __LINE__);
		return -1;
	}

	return this->m_RequireLeadership[skill];
}


int CMagicDamage::GetDelayTime(int skill)
{
	if ( skill < 0 || skill > MAX_SKILL-1 )
	{
		g_Log.Add("Skill limit error: (%s %d), ID: %d", __FILE__, __LINE__, skill);
		return 0;
	}

	return this->m_Delay[skill];
}




int CMagicDamage::GetSkillType(int iSkill)
{
	if ( iSkill < 0 || iSkill > MAX_SKILL-1 )
	{
		g_Log.Add("Skill limit error: (%s %d), ID: %d", __FILE__, __LINE__, iSkill);
		return 0;
	}
	
	return this->m_iSkillType[iSkill];
}



int CMagicDamage::SkillGetRequireLevel(int skill)
{
	if ( skill < 0 || skill > MAX_SKILL-1 )
	{
		g_Log.Add("Skill limit error: (%s %d), ID: %d", __FILE__, __LINE__, skill);
		return -1;
	}

	return this->m_RequireLevel[skill];
}




BOOL CMagicDamage::CheckStatus(int iSkill, int iGuildStatus)
{
	if ( iSkill < 0 || iSkill > MAX_SKILL-1 )
	{
		g_Log.Add("Skill limit error: (%s %d), ID: %d", __FILE__, __LINE__, iSkill);
		return 0;
	}

	if ( this->m_iSkillUseType[iSkill] == 0 )
	{
		return TRUE;
	}

	if (g_MasterLevelSkillTreeSystem.CheckMasterLevelSkill(iSkill) == true)
	{
		return TRUE;
	}

	if ( iGuildStatus == 0x80 )
	{
		if ( this->m_iRequireStatus[iSkill][0] == 1 )
		{
			return TRUE;
		}
	}
	else if ( iGuildStatus == 0x40 )
	{
		if ( this->m_iRequireStatus[iSkill][1] == 1 )
		{
			return TRUE;
		}
	}
	else if ( iGuildStatus == 0x20 )
	{
		if ( this->m_iRequireStatus[iSkill][2] == 1 )
		{
			return TRUE;
		}
	}

	return FALSE;
}




BOOL CMagicDamage::CheckBrandOfSkill(int iSkill)
{
	if ( iSkill < 0 || iSkill > MAX_SKILL-1 )
	{
		g_Log.Add("Skill limit error: (%s %d), ID: %d", __FILE__, __LINE__, iSkill);
		return 0;
	}

	if ( this->m_iSkillBrand[iSkill] > 0 )
	{
		return TRUE;
	}

	return FALSE;
}

int CMagicDamage::CheckKillCount(int iSkill, int iKillCount)
{
	if ( iSkill < 0 || iSkill > MAX_SKILL-1 )
	{
		g_Log.Add("Skill limit error: (%s %d), ID: %d", __FILE__, __LINE__, iSkill);
		return -1;
	}

	return iKillCount - this->m_iKillCount[iSkill];
}

int CMagicDamage::GetSkillDistance(int skill)
{
	if ( skill < 0 || skill > MAX_SKILL-1 )
	{
		g_Log.Add("Skill limit error: (%s %d), ID: %d", __FILE__, __LINE__, skill);
		return -1;
	}

	return this->m_Distance[skill];
}

int CMagicDamage::GetRequireMLPoint(int iSkill)
{
	if ( iSkill < 0 || iSkill > MAX_SKILL-1 )
	{
		g_Log.Add("Skill limit error: (%s %d), ID: %d", __FILE__, __LINE__, iSkill);
		return -1;
	}

	return this->m_nRequirePoint[iSkill];
}

int CMagicDamage::GetRequireStrength(int iSkill)
{
	if ( iSkill < 0 || iSkill > MAX_SKILL-1 )
	{
		g_Log.Add("Skill limit error: (%s %d), ID: %d", __FILE__, __LINE__, iSkill);
		return -1;
	}

	return this->m_iRequireStr[iSkill];
}

int CMagicDamage::GetRequireDexterity(int iSkill)
{
	if ( iSkill < 0 || iSkill > MAX_SKILL-1 )
	{
		g_Log.Add("Skill limit error: (%s %d), ID: %d", __FILE__, __LINE__, iSkill);
		return -1;
	}

	return this->m_iRequireDex[iSkill];
}

WORD CMagicDamage::GetBrandOfSkill(int iSkill)
{
	if ( iSkill < 0 || iSkill > MAX_SKILL-1 )
	{
		g_Log.Add("Skill limit error: (%s %d), ID: %d", __FILE__, __LINE__, iSkill);
		return -1;
	}

	return this->m_iSkillBrand[iSkill];
}

WORD CMagicDamage::GetNormalBrandOfSkill(int iSkill)
{
	if ( iSkill < 0 || iSkill > MAX_SKILL-1 )
	{
		g_Log.Add("Skill limit error: (%s %d), ID: %d", __FILE__, __LINE__, iSkill);
		return -1;
	}

	int iCnt = 0;
	WORD iBrandOfSkill = iSkill;
	WORD nBrandOfSkill;

	while ( true )
	{
		nBrandOfSkill = this->GetBrandOfSkill(iBrandOfSkill);

		if ( g_MasterLevelSkillTreeSystem.CheckMasterLevelSkill(nBrandOfSkill) == false )
			break;

		iBrandOfSkill = nBrandOfSkill;
		iCnt++;

		if ( iCnt > 3 )
		{
			g_Log.Add("[GetNormalBrandOfSkill] fail!!! - %d", iSkill);
			return iSkill;
		}
	}

	if ( !nBrandOfSkill && g_MasterLevelSkillTreeSystem.CheckMasterLevelSkill(iBrandOfSkill) )
	{
		return iBrandOfSkill;
	}

	return nBrandOfSkill;
}

BYTE CMagicDamage::IsItemAttachedSkill(int iSkill)
{
	if ( iSkill < 0 || iSkill > MAX_SKILL-1 )
	{
		g_Log.Add("Skill limit error: (%s %d), ID: %d", __FILE__, __LINE__, iSkill);
		return -1;
	}

	return this->m_byItemAttachedSkill[iSkill];
}

int CMagicDamage::GetSkillUseType(int iSkill)
{
	if ( iSkill < 0 || iSkill > MAX_SKILL-1 )
	{
		g_Log.Add("Skill limit error: (%s %d), ID: %d", __FILE__, __LINE__, iSkill);
		return -1;
	}

	return this->m_iSkillUseType[iSkill];
}

int CMagicDamage::GetSkillLevel(int iSkill)
{
	if ( iSkill < 0 || iSkill > MAX_SKILL-1 )
	{
		g_Log.Add("Skill limit error: (%s %d), ID: %d", __FILE__, __LINE__, iSkill);
		return -1;
	}

	return this->m_rLevel[iSkill];
}

int CMagicDamage::GetSkillGroup(int iSkill)
{
	if ( iSkill < 0 || iSkill > MAX_SKILL-1 )
	{
		g_Log.Add("Skill limit error: (%s %d), ID: %d", __FILE__, __LINE__, iSkill);
		return -1;
	}

	return this->m_iSkillGroup[iSkill];
}

int CMagicDamage::GetSkillRank(int iSkill)
{
	if ( iSkill < 0 || iSkill > MAX_SKILL-1 )
	{
		g_Log.Add("Skill limit error: (%s %d), ID: %d", __FILE__, __LINE__, iSkill);
		return -1;
	}

	return this->m_iSkillRank[iSkill];
}

int CMagicDamage::GetSkillKeepTime(int iSkill)
{
	if ( iSkill < 0 || iSkill > MAX_SKILL-1 )
	{
		g_Log.Add("Skill limit error: (%s %d), ID: %d", __FILE__, __LINE__, iSkill);
		return -1;
	}

	return this->m_wKeepTime[iSkill];
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

