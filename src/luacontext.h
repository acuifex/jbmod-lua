#pragma once
#include "lua.hpp"

class luaContext
{
public:
    luaContext();
    ~luaContext();
    // TODO: handle results
    void callHook(const char* name, int nargs);

    lua_State* state;

    // TODO: this should be private
    int hooktable;
};


extern luaContext* g_pLuaContext;
