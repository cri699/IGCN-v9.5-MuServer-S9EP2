//////////////////////////////////////////////////////////////////////
// ExtraEXP.h
#include "MapClass.h"
struct MAP_EXP	// this is where we will put the info from file that we will read
{
	int MapsSingle[MAX_NUMBER_MAP]; // each map have each extra exp possibility so we make INTEGER array with max maps size
	int MapsParty[MAX_NUMBER_MAP]; // for party exp
};
// now we must put this struct in class so we can access it from anywhere
class CExtraEXP // most commonly use Candfilename for the name of class
{
	//and all functions that  want to use outside of the .cpp file where u make the class u must put in public:
public: // like this got it? yup
	void LoadFile(LPSTR file); // LPSTR is a string and file is name of argument. we must put the function here in class header so we can access it from anywhere nderstad? yup a
	// so now we put structure  in the class that we made above
	MAP_EXP m_ExtraExpData; // type of variable is MAP_EXP and name is u see
}; extern CExtraEXP g_ExtraEXP; // this is how we will call class later g_ExtraEXP.function()
// now we need a function that will load a file

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

