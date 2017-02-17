//////////////////////////////////////////////////////////////////////
// LuaClass.cpp
#include "stdafx.h"
#include "LuaClass.h"
#include "TLog.h"
#include "configread.h"

bool g_Generic_Call(lua_State *L, const char* func, const char* sig, ...)
{
	va_list vl;
	int narg, nres;
    
	va_start(vl, sig);
	lua_getglobal(L, func);
    
	narg = 0;

	while (*sig) 
	{
		switch (*sig++) 
		{
			case 'd':  /* double argument */
				lua_pushnumber(L, va_arg(vl, double));
				break;
    
			case 'i':  /* int argument */
				lua_pushnumber(L, va_arg(vl, int));
				break;
    
			case 's':  /* string argument */
				lua_pushstring(L, va_arg(vl, char *));
				break;

			case 'w':
				lua_pushnumber(L, va_arg(vl, unsigned short));
				break;
    
			case '>':
				goto endwhile;
    
			default:
				g_Log.AddC(0, __FILE__, __FUNCTION__, TColor::Red, "[LUA] invalid option (%d)", *(sig - 1));
				return false;
        }

		narg++;
		luaL_checkstack(L, 1, "too many arguments");
      } 

endwhile:
    
	nres = strlen(sig);

	if (lua_pcall(L, narg, nres, 0) != 0)  /* do the call */
	{
		g_Log.AddC(0, __FILE__, __FUNCTION__, TColor::Red, "[LUA] error running function '%s': '%s", func, lua_tostring(L, -1));
		return false;
	}
    
	nres = -nres;

	while (*sig) 
	{
		switch (*sig++) 
		{ 
			case 'd':  /* double result */
				if (!lua_isnumber(L, nres))
				{
					g_Log.AddC(0, __FILE__, __FUNCTION__, TColor::Red, "[LUA] wrong result type");
					return false;
				}

				*va_arg(vl, double *) = lua_tonumber(L, nres);
				break;
    
			case 'i':  /* int result */
				if (!lua_isnumber(L, nres))
				{
					g_Log.AddC(0, __FILE__, __FUNCTION__, TColor::Red, "[LUA] wrong result type");
					return false;
				}

				*va_arg(vl, int *) = (int)lua_tonumber(L, nres);
				break;
    
			case 's':  /* string result */
				if (!lua_isstring(L, nres))
				{
					g_Log.AddC(0, __FILE__, __FUNCTION__, TColor::Red, "[LUA] wrong result type");
					return false;
				}

				*va_arg(vl, const char **) = lua_tostring(L, nres);
				break;
    
			default:
				g_Log.AddC(0, __FILE__, __FUNCTION__, TColor::Red, "[LUA] invalid option (%c)", *(sig - 1));
				break;
		}

		nres++;
	}

	va_end(vl);
}

BOOL Require(lua_State * L)
{
	const char * szFile;
	const char * szPath;

	static void * sentinel_ = 0;

	szFile = luaL_checklstring(L, 1, 0);
	szPath = g_ConfigRead.GetPath((LPSTR)szFile);

	lua_settop(L, 1);
	lua_getfield(L, -10000, "_LOADED");
	lua_getfield(L, 2, szPath);

	bool bcheck = lua_toboolean(L, -1);

	if ( bcheck )
	{
		if ( lua_touserdata(L, -1) == sentinel_ )
		{
			luaL_error(L, "loop or previous error loading module '%s'", szPath);
		}

		return TRUE;
	}

	if ( luaL_loadfile(L, szPath) )
	{
		lua_tolstring(L, -1, 0);
		const char * szModule = lua_tolstring(L, 1, 0);
		luaL_error(L, "error loading module '%s' from file '%s':\n\t%s", szModule, szFile, szPath);
	}

	lua_pushlightuserdata(L, &sentinel_);
	lua_setfield(L, 2, szPath);
	lua_pushstring(L, szPath);
	lua_call(L, 1, 1);

	if ( lua_type(L, -1) )
		lua_setfield(L, 2, szPath);

	lua_getfield(L, 2, szPath);

	if ( lua_touserdata(L, -1) == &sentinel_ )
	{
		lua_pushboolean(L, 1);
		lua_pushvalue(L, -1);
		lua_setfield(L, 2, szPath);
	}

	return TRUE;
}

CLuaClass::CLuaClass(void)
{
	this->m_pLuaState = NULL;
	this->Create();
}

CLuaClass::~CLuaClass(void)
{
	this->Release();
}

bool CLuaClass::DoFile(const char* szFileName)
{
	if ( luaL_loadfile(this->m_pLuaState, szFileName) != 0 )
	{
		g_Log.MsgBox("Error: %s", lua_tolstring(this->m_pLuaState, -1, 0));
		return false;
	}

	if ( lua_pcall(this->m_pLuaState, 0, -1, 0) != 0 )
	{
		g_Log.MsgBox("Error: %s", lua_tolstring(this->m_pLuaState, -1, 0));
		return false;
	}

	return true;
}

lua_State * CLuaClass::GetLua()
{
	return this->m_pLuaState;
}

bool CLuaClass::Create()
{
	this->m_pLuaState = luaL_newstate();
	luaL_openlibs(this->m_pLuaState);
	lua_pushcclosure(this->m_pLuaState, Require, 0);
	lua_setfield(this->m_pLuaState, -10002, "murequire");
	lua_gc(this->m_pLuaState, 2, 0);

	return true;
}

bool CLuaClass::Release()
{
	if ( this->m_pLuaState )
	{
		lua_close(this->m_pLuaState);
		this->m_pLuaState = 0;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

