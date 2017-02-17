//////////////////////////////////////////////////////////////////////
// ExtraEXP.cpp
#include "StdAfx.h"
#include "ExtraEXP.h"
#include "user.h"
#include "TLog.h"
CExtraEXP g_ExtraEXP; // we must put the externed name here so it initializes
void CExtraEXP::LoadFile(LPSTR file) // file is argument look. now we will call the function so it reads the data, so now to read the file, we must find a place where gs calls all loading functions
{
	memset(&m_ExtraExpData,0,sizeof(this->m_ExtraExpData)); // we set all variables to 0 in the memory before its read or if it gets read bad it wont throw unknown random values
	char Name[8]; // we will store the name here all the time
	int check;
	for(int i=0;i<MAX_NUMBER_MAP;i++)
	{
		StringCchPrintf(Name, 8, "Map%02d", i+1); // map 44 should be 49 it is so file read is succesful

		this->m_ExtraExpData.MapsSingle[i] = GetPrivateProfileIntA("SingleExtraExp",Name,0,file); // it shows it in 2 because i+1 in programming all is started to read from 0 but in real life from 1 so we make user friendly
		this->m_ExtraExpData.MapsParty[i] =  GetPrivateProfileIntA("PartyExtraExp",Name,0,file);
	}
	// now we can access info from file in gs memory.. so we find the function that calculates how much exp player gets... to do that u must research gs abit
	g_Log.AddC(0, __FILE__, __FUNCTION__, TColor::DarkOrange, "CExtraEXP::LoadFile() -> Success!"); // so we put breakpoint here at the end of func before last call and check our struct for info, u saw i put som erandom values in file? yea

}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

