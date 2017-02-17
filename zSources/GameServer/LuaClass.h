//////////////////////////////////////////////////////////////////////
// LuaClass.h
#ifndef LUACLASS_H
#define LUACLASS_H

#pragma once
class CLuaClass
{
public:
	CLuaClass(void);
	virtual ~CLuaClass(void);

	bool DoFile(const char* szFileName);
	lua_State * GetLua();
	bool Create();
	bool Release();

private:

	lua_State * m_pLuaState;
};

extern bool g_Generic_Call(lua_State *L, const char* func, const char* sig, ...);

#endif

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

