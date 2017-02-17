//////////////////////////////////////////////////////////////////////
// DoppelgangerLua.cpp
// ------------------------------
// Decompiled by Hybrid
// 1.01.00
// ------------------------------

#include "stdafx.h"
#include "DoppelgangerLua.h"
#include "Doppelganger.h"

const char CDoppelgangerLua::className[] = "CDoppelgangerLua";
const Luna<CDoppelgangerLua>::RegType  CDoppelgangerLua::Register[] =
{

{ "LuaGetMaxUserLevel",         &CDoppelgangerLua::LuaGetMaxUserLevel },
{ "LuaGetMinUserLevel",         &CDoppelgangerLua::LuaGetMinUserLevel },
{ "LuaGetUserCount",            &CDoppelgangerLua::LuaGetUserCount },
{ "LuaSetReadyTime",            &CDoppelgangerLua::LuaSetReadyTime },
{ "LuaSetPlayTime",             &CDoppelgangerLua::LuaSetPlayTime },
{ "LuaSetEndTime",              &CDoppelgangerLua::LuaSetEndTime },
{ "LuaGetLocalTime",            &CDoppelgangerLua::LuaGetLocalTime },
{ "LuaGetStateTime",            &CDoppelgangerLua::LuaGetStateTime },
{ "LuaSetHerdStartPosInfo",     &CDoppelgangerLua::LuaSetHerdStartPosInfo },
{ "LuaSetHerdEndPosInfo",       &CDoppelgangerLua::LuaSetHerdEndPosInfo },
{ "LuaAddMonsterHerd",          &CDoppelgangerLua::LuaAddMonsterHerd },
{ "LuaAddMonsterNormal",        &CDoppelgangerLua::LuaAddMonsterNormal },
{ "LuaIceworkerRegen",          &CDoppelgangerLua::LuaIceworkerRegen },
{ "LuaGetMonsterHerdIndex",     &CDoppelgangerLua::LuaGetMonsterHerdIndex },
{ "LuaIncMonsterHerdIndex",     &CDoppelgangerLua::LuaIncMonsterHerdIndex },
{ "LuaMonsterHerdStart",        &CDoppelgangerLua::LuaMonsterHerdStart },
{ "LuaGetMapNumber",            &CDoppelgangerLua::LuaGetMapNumber },
{ "LuaGetRandomValue",          &CDoppelgangerLua::LuaGetRandomValue },
{ "LuaSetAddHerdMonsterTime",   &CDoppelgangerLua::LuaSetAddHerdMonsterTime },
{ "LuaGetAddHerdMonsterTime",   &CDoppelgangerLua::LuaGetAddHerdMonsterTime },
{ "LuaSetAddBossMonsterTime",   &CDoppelgangerLua::LuaSetAddBossMonsterTime },
{ "LuaGetAddBossMonsterTime",   &CDoppelgangerLua::LuaGetAddBossMonsterTime },
{ "LuaGetBossRegenOrder",       &CDoppelgangerLua::LuaGetBossRegenOrder },
{ "LuaSetBossRegenOrder",       &CDoppelgangerLua::LuaSetBossRegenOrder },
{ "LuaGetBossMonsterState",     &CDoppelgangerLua::LuaGetBossMonsterState },
{ "LuaSetBossMonsterState",     &CDoppelgangerLua::LuaSetBossMonsterState },
{ "LuaGetLastBossMonsterState", &CDoppelgangerLua::LuaGetLastBossMonsterState },
{ "LuaSetLastBossMonsterState", &CDoppelgangerLua::LuaSetLastBossMonsterState },
{ 0 }

};

CDoppelgangerLua::CDoppelgangerLua(void)
{
}

CDoppelgangerLua::~CDoppelgangerLua(void)
{
}

int CDoppelgangerLua::LuaGetMaxUserLevel(lua_State* L)
{
    int max_user_level = g_DoppelGanger.GetUserMaxLevel();
    lua_pushnumber(L, max_user_level);

    return 1;
}

int CDoppelgangerLua::LuaGetMinUserLevel(lua_State* L)
{
    int max_user_level = g_DoppelGanger.GetUserMaxLevel();
    lua_pushnumber(L, max_user_level);

    return 1;
}

int CDoppelgangerLua::LuaGetUserCount(lua_State *L)
{
    int user_count = g_DoppelGanger.GetStartUserCount();
    lua_pushnumber(L, user_count);

    return 1;
}

int CDoppelgangerLua::LuaSetReadyTime(lua_State *L)
{
    ULONGLONG ready_time = luaL_checkint(L, 2);
    g_DoppelGanger.SetReadyTime(ready_time);
    lua_settop(L, -2);

    return 0;
}

int CDoppelgangerLua::LuaSetPlayTime(lua_State *L)
{
    ULONGLONG play_time = luaL_checkinteger(L, 2);
    g_DoppelGanger.SetPlayTime(play_time);
    lua_settop(L, -2);

    return 0;
}

int CDoppelgangerLua::LuaSetEndTime(lua_State *L)
{
    ULONGLONG end_time = luaL_checkinteger(L, 2);
    g_DoppelGanger.SetEndTime(end_time);
    lua_settop(L, -2);

    return 0;
}

int CDoppelgangerLua::LuaGetLocalTime(lua_State *L)
{
    ULONGLONG current_time = GetTickCount64();
    lua_pushnumber(L, current_time);

    return 1;
}

int CDoppelgangerLua::LuaGetStateTime(lua_State *L)
{
    ULONGLONG state_time = g_DoppelGanger.GetStateTime();
    lua_pushnumber(L, state_time);

    return 1;
}

int CDoppelgangerLua::LuaSetHerdStartPosInfo(lua_State *L)
{
    int herd_index = luaL_checkinteger(L, 2);
    int pos_info = luaL_checkinteger(L, 3);
    int should_move = luaL_checkinteger(L, 4);

    g_DoppelGanger.SetHerdStartPosInfo(herd_index, pos_info, should_move);

    lua_settop(L, -4);

    return 0;
}

int CDoppelgangerLua::LuaSetHerdEndPosInfo(lua_State *L)
{
    int herd_index = luaL_checkinteger(L, 2);
    int pos_info = luaL_checkinteger(L, 3);

    g_DoppelGanger.SetHerdEndPosInfo(herd_index, pos_info);

    lua_settop(L, -3);

    return 0;
}

int CDoppelgangerLua::LuaAddMonsterHerd(lua_State *L)
{
    int herd_index = luaL_checkinteger(L, 2);
    int monster_class = luaL_checkinteger(L, 3);
    int should_attack_first = luaL_checkinteger(L, 4);

    g_DoppelGanger.AddMonsterHerd(herd_index, monster_class, should_attack_first);

    lua_settop(L, -4);

    return 0;
}

int CDoppelgangerLua::LuaAddMonsterNormal(lua_State *L)
{
    int monster_class = luaL_checkinteger(L, 2);
    int monster_pos_x = luaL_checkinteger(L, 3);
    int monster_pos_y = luaL_checkinteger(L, 4);

    g_DoppelGanger.AddMonsterNormal(monster_class, monster_pos_x, monster_pos_y);

    lua_settop(L, -4);

    return 0;
}

int CDoppelgangerLua::LuaIceworkerRegen(lua_State *L)
{
    int pos_info = luaL_checkinteger(L, 2);

    g_DoppelGanger.SetIceWorkerRegen(pos_info);

    lua_settop(L, -2);

    return 0;
}

int CDoppelgangerLua::LuaGetMonsterHerdIndex(lua_State *L)
{
    lua_pushnumber(L, g_DoppelGanger.GetMonsterHerdIndex());

    return 1;
}

int CDoppelgangerLua::LuaIncMonsterHerdIndex(lua_State *L)
{
	g_DoppelGanger.IncMonsterHerdIndex();

    return 0;
}

int CDoppelgangerLua::LuaMonsterHerdStart(lua_State *L)
{
    int herd_index = luaL_checkinteger(L, 2);

    g_DoppelGanger.MonsterHerdStart(herd_index);

    lua_settop(L, -2);

    return 0;
}

int CDoppelgangerLua::LuaGetMapNumber(lua_State *L)
{
    lua_pushnumber(L, g_DoppelGanger.GetMapNumber());

    return 1;
}

int CDoppelgangerLua::LuaGetRandomValue(lua_State *L)
{
    int range = luaL_checkinteger(L, 2);
    int rand_value = g_DoppelGanger.GetRandomValue(range);

    lua_settop(L, -2);
    lua_pushnumber(L, rand_value);

    return 1;
}

int CDoppelgangerLua::LuaSetAddHerdMonsterTime(lua_State *L)
{
    ULONGLONG add_monster_time = luaL_checknumber(L, 2);

    g_DoppelGanger.SetAddHerdMonsterTime(add_monster_time);

    lua_settop(L, -2);

    return 0;
}

int CDoppelgangerLua::LuaGetAddHerdMonsterTime(lua_State *L)
{
    ULONGLONG add_monster_time = g_DoppelGanger.GetAddHerdMonsterTime();

    lua_pushnumber(L, add_monster_time);

    return 1;
}

int CDoppelgangerLua::LuaSetAddBossMonsterTime(lua_State *L)
{
    ULONGLONG add_monster_time = luaL_checknumber(L, 2);

    g_DoppelGanger.SetAddBossMonsterTime(add_monster_time);

    lua_settop(L, -2);

    return 0;
}

int CDoppelgangerLua::LuaGetAddBossMonsterTime(lua_State *L)
{
    ULONGLONG add_monster_time = g_DoppelGanger.GetAddBossMonsterTime();

    lua_pushnumber(L, add_monster_time);

    return 1;
}

int CDoppelgangerLua::LuaGetBossRegenOrder(lua_State *L)
{
    int regen_order = g_DoppelGanger.GetBossRegenOrder();
    lua_pushnumber(L, regen_order);

    return 1;
}

int CDoppelgangerLua::LuaSetBossRegenOrder(lua_State *L)
{
    int regen_order = luaL_checkinteger(L, 2);

    g_DoppelGanger.SetBossRegenOrder(regen_order);

    lua_settop(L, -2);

    return 0;
}

int CDoppelgangerLua::LuaGetBossMonsterState(lua_State *L)
{
    int state = g_DoppelGanger.GetKillerState();

    lua_pushnumber(L, state);

    return 1;
}

int CDoppelgangerLua::LuaSetBossMonsterState(lua_State *L)
{
    int state = luaL_checkinteger(L, 2);

    g_DoppelGanger.SetKillerState(state);

    lua_settop(L, -2);

    return 0;
}

int CDoppelgangerLua::LuaGetLastBossMonsterState(lua_State *L)
{
    int state = g_DoppelGanger.GetAngerKillerState();

    lua_pushnumber(L, state);

    return 1;
}

int CDoppelgangerLua::LuaSetLastBossMonsterState(lua_State *L)
{
    int state = luaL_checkinteger(L, 2);

    g_DoppelGanger.SetAngerKillerState(state);

    lua_settop(L, -2);

    return 0;
}

//////////////////////////////////////////////////////////////////////
// iDev.Games - MuOnline S9EP2 IGC9.5 - TRONG.WIN - DAO VAN TRONG     
//////////////////////////////////////////////////////////////////////

