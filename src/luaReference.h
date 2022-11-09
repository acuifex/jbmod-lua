#pragma once
#include "lua/lua.hpp"

class luaReference {
public:
    luaReference(const char* name, const struct luaL_Reg* methods);
    const struct luaL_Reg* methods;
    const char* name;
    // TODO: properly initialize and expose meta
    const struct luaL_Reg meta[2] = {
        {"__index", NULL},
        // {"__gc", keyvalues_gc},
        {NULL, NULL}
    };

    void* get (lua_State *L);
    void create (lua_State *L, void* data);
    void registerType (lua_State *L);
};