//////////////////////////////////////////////////////////////////////
// MagicInf.cpp
#include "stdafx.h"
#include "MagicInf.h"
#include "TLog.h"
#include "GameMain.h"
#include "MasterLevelSkillTreeSystem.h"
// GS-N 0.99.60T 0x0046DE00
//	GS-N	1.00.18	JPN	0x00484870	-	Completed

CMagicInf DefMagicInf[MAX_SKILL-1];



CMagicInf::CMagicInf()
{
	this->Clear();
}



CMagicInf::~CMagicInf()
{
	this->m_DamageMin  = 0;
	this->m_DamageMax  = 0;
}



void CMagicInf::Clear()
{
	this->m_Skill  = -1;
	this->m_Level  = 0;
	this->m_Pass = 0;
}



BOOL CMagicInf::IsMagic()
{
	if ( this->m_Skill == -1 )
	{
		return false;
	}
	return true;
}



int GetSkillNumberInex(int type, int Index, int level)
{
	int skillnumber = -1;

	if ( type == 15 )
	{
		if ( Index >= 0 && Index < 16 )
		{
			skillnumber = Index + 1;
		}
		else if ( Index == 16 )
		{
			skillnumber = 38;
		}
		else if ( Index == 17 )
		{
			skillnumber = 39;
		}
		else if ( Index == 18 )
		{
			skillnumber = 40;
		}
		else if ( Index == 19 )
		{
			skillnumber = AT_SKILL_CHAIN_LIGHTING;
		}
		else if ( Index == 20 )
		{
			skillnumber = AT_SKILL_DRAIN_LIFE;
		}
		else if ( Index == 21 )
		{
			skillnumber = AT_SKILL_LIGHTING_SHOCK;
		}
		else if ( Index == 22 )
		{
			skillnumber = AT_SKILL_DAMAGE_REFLECT;
		}
		else if ( Index == 23 )
		{
			skillnumber = AT_SKILL_SWORD_POWER;
		}
		else if ( Index == 24 )
		{
			skillnumber = AT_SKILL_SLEEP;
		}
		else if ( Index == 26 )
		{
			skillnumber = AT_SKILL_WEAKNESS;
		}
		else if ( Index == 27 )
		{
			skillnumber = AT_SKILL_INNOVATION;
		}
		else if ( Index == 28 )
		{
			skillnumber = AT_SKILL_MAGIC_CIRCLE;
		}
		else if ( Index == 29 )
		{
			skillnumber = AT_SKILL_GIGANTIC_STORM;
		}
		else if ( Index == 30 )
		{
			skillnumber = AT_SKILL_CHAIN_DRIVE;
		}
		else if ( Index == 31 )
		{
			skillnumber = AT_SKILL_DARK_SIDE;
		}
		else if ( Index == 32 )
		{
			skillnumber = AT_SKILL_DRAGON_ROAR;
		}
		else if ( Index == 33 )
		{
			skillnumber = AT_SKILL_DRAGON_SLASH;
		}
		else if ( Index == 34 )
		{
			skillnumber = AT_SKILL_REDARMOR_IGNORE;
		}
		else if ( Index == 35 )
		{
			skillnumber = AT_SKILL_FITNESS;
		}
		else if ( Index == 36 )
		{
			skillnumber = AT_SKILL_DSRINCREASE;
		}
	}

	else if ( type == 12 )
	{
		switch ( Index )
		{
			case 7:		skillnumber = 41;	break;
			case 8:		skillnumber = 26;	break;
			case 9:		skillnumber = 27;	break;
			case 10:	skillnumber = 28;	break;
			case 12:	skillnumber = 42;	break;
			case 13:	skillnumber = 47;	break;
			case 14:	skillnumber = 48;	break;
			case 19:	skillnumber = 43;	break;
			case 17:	skillnumber = 52;	break;
			case 18:	skillnumber = 51;	break;
			case 16:	skillnumber = 55;	break;
			case 21:	skillnumber = 61;	break;
			case 22:	skillnumber = 63;	break;
			case 23:	skillnumber = 64;	break;
			case 24:	skillnumber = 65;	break;
			case 11:	skillnumber = level + 30;	break;
			case 35:	skillnumber = 78;	break;
			case 44:	skillnumber = 232;	break;
			case 45:	skillnumber = 235;	break;
			case 46:	skillnumber = 234;	break;
			case 47:	skillnumber = 236;	break;
			case 48:	skillnumber = 238;	break;
			case 271:	skillnumber = 271;  break;
			case 272:	skillnumber = 273;	break;
			case 273:	skillnumber = 274;	break;
			case 274:	skillnumber = 276;	break;
			case 275:	skillnumber = 277;	break;
			case 276:	skillnumber = 278;	break;
			case 277:	skillnumber = 279;	break;
		}
	}
		
	return skillnumber;
}



int CMagicInf::Set(BYTE aType, WORD aIndex, BYTE aLevel)
{
	if ( this->m_Skill != -1 )
	{
		g_Log.Add("No space to add %s %d", __FILE__, __LINE__);
		return -1;
	}

	this->m_Level = aLevel;
	this->m_Skill = GetSkillNumberInex(aType, aIndex, aLevel);
	
	if ( this->m_Skill < 0 )
	{
		this->m_Skill = 1;
	}

	int damage = MagicDamageC.SkillGet(this->m_Skill);
	this->m_DamageMin = damage;
	this->m_DamageMax = damage + damage/2;

	return this->m_Skill;
}



int CMagicInf::GetDamage()
{
	int damage = 0;

	if ( this->m_Skill == -1 )
	{
		g_Log.Add("Damage value doesn't exist (%s %d)", __FILE__, __LINE__);
		return 0;
	}

	int subd = this->m_DamageMax - this->m_DamageMin;

	__try
	{
		damage = this->m_DamageMin + (rand() % 1);
	}
	__except(subd=1,1)
	{
		g_Log.Add("error2 : %s %d", __FILE__, __LINE__);
	}

	return damage;
}




int CMagicInf::Set(int aSkill, BYTE aLevel)
{
	if ( this->m_Skill != -1 )
	{
		return -1;
	}

	this->m_Skill = aSkill;
	this->m_Level = aLevel;
	int damage;

	if ( g_MasterLevelSkillTreeSystem.CheckMasterLevelSkill(aSkill) == true )
	{
		if ( MagicDamageC.GetSkillUseType(aSkill) == 4 )
		{
			int nBrandOfSkill = MagicDamageC.GetNormalBrandOfSkill(aSkill);

			if ( nBrandOfSkill <= 0 || nBrandOfSkill == aSkill )
			{
				float fValue = g_MasterLevelSkillTreeSystem.GetMasterSkillValue(MagicDamageC.SkillGet(this->m_Skill), this->m_Level);

				if ( fValue == 0.0 )
				{
					damage = 1;
				}

				else
				{
					damage = fValue;
				}
			}

			else if ( g_MasterLevelSkillTreeSystem.CheckMasterLevelSkill(nBrandOfSkill) == true ) 
			{
				float fValue = g_MasterLevelSkillTreeSystem.GetMasterSkillValue(MagicDamageC.SkillGet(nBrandOfSkill), 10);

				if ( fValue == 0.0 )
				{
					damage = 1;
				}

				else
				{
					damage = fValue;
				}
			}

			else
			{
				damage = MagicDamageC.SkillGet(nBrandOfSkill);
			}
		}

		else
		{
			float fValue = g_MasterLevelSkillTreeSystem.GetMasterSkillValue(MagicDamageC.SkillGet(this->m_Skill), this->m_Level);

			if ( fValue == 0.0 )
			{
				damage = 1;
			}

			else
			{
				damage = fValue;
			}
		}
	}

	else
	{
		damage = MagicDamageC.SkillGet(this->m_Skill);
	}

	this->m_DamageMin = damage;
	this->m_DamageMax = damage + damage/2;

	return this->m_Skill;
}

int CMagicInf::UpdateMasterSkill(int iSkill, BYTE btLevel)
{
	if ( this->m_Skill == -1 )
	{
		g_Log.Add("[MasterSkill] [ERROR!!] Fail - Update Master Skill:%d, Level:%d", iSkill, btLevel);
		return -1;
	}

	this->m_Skill = iSkill;
	this->m_Level = btLevel;

	int damage;

	if ( MagicDamageC.GetSkillUseType(this->m_Skill) == 4 )
	{
		int nBrandOfSkill = MagicDamageC.GetNormalBrandOfSkill(this->m_Skill);

		if ( nBrandOfSkill <= 0 || nBrandOfSkill == this->m_Skill )
		{
			float fValue = g_MasterLevelSkillTreeSystem.GetMasterSkillValue(MagicDamageC.SkillGet(this->m_Skill), this->m_Level);

			if ( fValue == 0.0 )
			{
				damage = 1;
			}

			else
			{
				damage = fValue;
			}
		}

		else if ( g_MasterLevelSkillTreeSystem.CheckMasterLevelSkill(nBrandOfSkill) == true ) 
		{
			float fValue = g_MasterLevelSkillTreeSystem.GetMasterSkillValue(MagicDamageC.SkillGet(nBrandOfSkill), 10);

			if ( fValue == 0.0 )
			{
				damage = 1;
			}

			else
			{
				damage = fValue;
			}
		}

		else
		{
			damage = MagicDamageC.SkillGet(nBrandOfSkill);
		}
	}

	else
	{
		float fValue = g_MasterLevelSkillTreeSystem.GetMasterSkillValue(MagicDamageC.SkillGet(this->m_Skill), this->m_Level);

		if ( fValue == 0.0 )
		{
			damage = 1;
		}

		else
		{
			damage = fValue;
		}
	}

	this->m_DamageMin = damage;
	this->m_DamageMax = damage + damage/2;

	return this->m_Skill;
}

void MagicByteConvert(unsigned char* buf, CMagicInf* const Magici, int maxmagic)
{
	int n=0;

	for (int index = 0;index < maxmagic;index++)
	{
		int iExpendSkillNumber = Magici[index].m_Skill;
		BYTE iExpendSkillCount = 0;
		int iExpendSkillExtraNumber = 0;

		if ( Magici[index].m_Skill > 765 )
		{
			iExpendSkillNumber = -1;
			iExpendSkillCount = 3;
			iExpendSkillExtraNumber = Magici[index].m_Skill - 765;
		}

		else if ( Magici[index].m_Skill > 510 )
		{
			iExpendSkillNumber = -1;
			iExpendSkillCount = 2;
			iExpendSkillExtraNumber = Magici[index].m_Skill - 510;
		}

		else if ( Magici[index].m_Skill > 255 )
		{
			iExpendSkillNumber = -1;
			iExpendSkillCount = 1;
			iExpendSkillExtraNumber = Magici[index].m_Skill - 255;
		}

		buf[n] = iExpendSkillNumber;
		n++;

		buf[n] = Magici[index].m_Level << 3;
		buf[n] |= iExpendSkillCount & 7;
		n++;

		buf[n] = iExpendSkillExtraNumber;
		n++;
	}
}





//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

