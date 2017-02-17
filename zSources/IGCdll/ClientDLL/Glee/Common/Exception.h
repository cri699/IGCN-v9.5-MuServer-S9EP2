//////////////////////////////////////////////////////////////////////
// Exception.h
// Description:		Exception class with filename and line number info
//
// By:				Ben Woodhouse (0145394)
// Date:			12/2003
//////////////////////////////////////////////////////////////////////

#pragma once

#define FATALERROR(errorText) throw Exception(errorText,__FUNCTION__,__FILE__,__LINE__)

namespace Mirage
{
	struct Exception
	{
		Exception(const String &errStr_in)
		{
			lineNum=-1;
			errStr=errStr_in;
		}
		Exception(const String &errStr_in, const char * function_in, const char *filename_in, int lineNum_in)
		{
			lineNum=lineNum_in;
			filename=filename_in;
			function=function_in;
			errStr=errStr_in;
		}
		Exception(const char *errStr_in)
		{
			lineNum=-1;
			errStr=errStr_in;
		}
		Exception(const char *errStr_in, const char * function_in, const char *filename_in, int lineNum_in)
		{
			lineNum=lineNum_in;
			filename=filename_in;
			function=function_in;
			errStr=errStr_in;
		}

		String errStr;
		String filename;
		String function;
		int lineNum;
	};
}


//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

