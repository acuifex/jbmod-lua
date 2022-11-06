#include "hook.h"

#include "luacontext.h"

static int Lua_hook_set(lua_State* L) {
    luaL_checktype(L, 1, LUA_TSTRING);
    luaL_checktype(L, 2, LUA_TFUNCTION);
    
    lua_rawgeti(L,LUA_REGISTRYINDEX,g_pLuaContext->hooktable);
    lua_insert(L, 1);
    lua_settable(L, 1);
    lua_pop(L, 1);
    return 0;
}

static const struct luaL_Reg hooklib [] = {
    {"set", Lua_hook_set},
    {NULL, NULL} /* end of array */
};

int luaopen_hook (lua_State *L) {
  luaL_newlib(L, hooklib);
  return 1;
}