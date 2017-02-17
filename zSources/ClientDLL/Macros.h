//////////////////////////////////////////////////////////////////////
// Macros.h
#define FPTR(Func)	(DWORD)Func			// Interpretacja wskaŸnika na funkcjê jako liczba

#ifndef RGBA
#define RGBA(r,g,b,a)        ((COLORREF)( (((DWORD)(BYTE)(a))<<24) |     RGB(r,g,b) ))
#define AddAValue(rgb, a)	 ((COLORREF)( (((DWORD)(BYTE)(a))<<24) | RGB(GetRValue(rgb), GetGValue(rgb), GetBValue(rgb))))
#define GetAValue(rgb)   ((BYTE) ((rgb) >> 24)) 
#endif

#define CMYK_RGB(c, m, y, k) (RGBA(((255-c)*(255-k))/255, ((255-m)*(255-k))/255, ((255-Y)*(255-K))/255), 255))

//#define ITEM_BASE 0x4C7
//define MU_MONSTER_COUNT 0x25B

//#define MU_ITEM(Sec, Id) (Sec*512)+ITEM_BASE+Id
#define MU_SERVER_ITEM(Sec, Id) (Sec*512)+Id

#define PI 3.14159265
#define E 2.71828183

#define METHOD_PTR(ClassType, MethodName) {(void (ClassType ## :: ## *MethodName##Ptr)() = (ClassType ## :: ## MethodName))}

#ifdef UNICODE
#define tstrlen wcslen
#define tsprintf wsprintf_s
#define tsscanf wscanf_s
#define tvsprintf vswprintf_s
#define tstring std::wstring
#define tfopen _wfopen_s
#else
#define tstrlen strlen
#define tsprintf sprintf_s
#define tsscanf sscanf_s
#define tvsprintf vsprintf_s
#define tstring std::string
#define tfopen fopen_s
#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

