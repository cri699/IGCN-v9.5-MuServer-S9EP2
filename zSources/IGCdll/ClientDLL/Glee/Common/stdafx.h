// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include <iostream>
#include <tchar.h>

// this code disables data conversion warnings
#pragma warning(disable:4051)    
#pragma warning(disable:4244)     
#pragma warning(disable:4136)     
#pragma warning(disable:4305)   
#pragma warning(disable:4018)      
#pragma warning(disable:4800)      
#pragma warning(disable:4730)      

#include <limits>
#include <malloc.h>
#include <stdio.h>
#include <assert.h>
#include <algorithm>
#include <xutility>
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <strstream>

const int MAX_INT=std::numeric_limits<int>::max();
const float MAX_FLOAT=std::numeric_limits<float>::max();

typedef unsigned int uint;

#ifndef max 
#define max(X,Y) ((X)>(Y) ? (X) : (Y))
#endif
#ifndef min
#define min(X,Y) ((X)<(Y) ? (X) : (Y))
#endif

#include "arraylist.h"
#include "string.h"
#include "exception.h"

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

