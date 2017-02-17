//////////////////////////////////////////////////////////////////////
// LuaFun.cpp
// ------------------------------
// Decompiled by Hybrid
// 1.01.00
// ------------------------------

#include "stdafx.h"
#include "LuaFun.h"
#include "TLog.h"

static int Debug_Print(lua_State *L)
{
	return 0;
}

void Debug_AddDebugPrintTemp(lua_State *L)
{
	luaL_Reg DebugGlue[] =
	{
		{"DebugPrint", Debug_Print},
		{NULL, NULL}
	};

	for (int i = 0; DebugGlue[i].name; ++i )
	{
		lua_pushcclosure(L, DebugGlue[i].func, 0); 
		lua_setglobal(L, DebugGlue[i].name);
	}
}

bool g_Generic_Call(lua_State * LState, const char * Func, const char * Sig, ...)
{
	if (LState == NULL)
	{
		return false;
	}

	va_list	VA_LIST;
	int nArg = 0;

	va_start(VA_LIST, Sig);
	lua_getglobal(LState, Func);

	while (*Sig)
	{
		switch (*Sig++)
		{
			case 'd':
			{
				lua_pushnumber(LState, va_arg(VA_LIST, double));
			} break;

			case 'i':
			{
				lua_pushnumber(LState, va_arg(VA_LIST, int));
			} break;

			case 's':
			{
				char *pszString = va_arg(VA_LIST, char *);
				lua_pushlstring(LState, pszString, strlen(pszString));
			} break;

			case 'w':
			{
				lua_pushnumber(LState, va_arg(VA_LIST, unsigned int));
			} break;

			case 'l':
			{
				lua_pushnumber(LState, va_arg(VA_LIST, unsigned long long));
			} break;

			case '>':
			{
				goto endwhile;
			}

			default:
			{
				g_Log.Add("luacall_Generic_Call invalid option (%c)", *(Sig - 1));
				return false;
			}
		}

		nArg++;
		luaL_checkstack(LState, 1, "too many arguments");

	} endwhile:

	int nRes = strlen(Sig);

	if (lua_pcall(LState, nArg, nRes, 0) != 0)
	{
		g_Log.Add("luacall_Generic_Call error running function '%s': '%s'", Func, lua_tolstring(LState, -1, 0));
		return false;
	}

	int nRetValCnt = nRes;
	nRes = -nRes;

	while (*Sig)
	{
		switch (*Sig++)
		{
		case 'd':
		{
			if (!lua_isnumber(LState, nRes))
			{
				return false;
			}
			*va_arg(VA_LIST, double *) = lua_tonumber(LState, nRes);

		} break;

		case 'i':
		{
			if (!lua_isnumber(LState, nRes))
			{
				return false;
			}

			*va_arg(VA_LIST, int *) = (int)lua_tonumber(LState, nRes);
		} break;

		case 's':
		{
			if (!lua_isstring(LState, nRes))
			{
				return false;
			}

			*va_arg(VA_LIST, const char **) = lua_tostring(LState, nRes);
		} break;

		case 'w':
		{
			if (!lua_isnumber(LState, nRes))
			{
				return false;
			}

			*va_arg(VA_LIST, unsigned int*) = (unsigned int)lua_tonumber(LState, nRes);
		} break;

		case 'l':
		{
			if (!lua_isnumber(LState, nRes))
			{
				return false;
			}

			*va_arg(VA_LIST, int *) = (unsigned long long)lua_tonumber(LState, nRes);
		} break;

		default: return false;	break;
		}

		nRes++;
	}

	va_end(VA_LIST);

	if (nRetValCnt)
	{
		lua_pop(LState, nRetValCnt);
	}

	return true;
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

