#include "luacontext.h"

#include "tier0/dbg.h"
#include "cbase.h"

#include "features/hook.h"
#include "features/keyvalues.h"
#include "luaReference.h"

luaContext* g_pLuaContext = 0;

static int Lua_print(lua_State* L) {
    int nargs = lua_gettop(L);

    for (int i=1; i <= nargs; i++) {
        Msg("%s", luaL_tolstring(L, i, NULL));
    }

    return 0;
}

static const struct luaL_Reg printlib [] = {
    {"print", Lua_print},
    {NULL, NULL} /* end of array */
};


luaContext::luaContext()
{
    state = luaL_newstate();
    // luaL_openlibs(state); // ok let's not open all libs. some are dangerous
    // TODO: remove this or change it's functions. currently needed for _G and other misc stuff (did you know that i'm bad at coding?)
    luaL_requiref(state, LUA_GNAME, luaopen_base, 1);
    luaL_requiref(state, LUA_STRLIBNAME, luaopen_string, 1);
    luaL_requiref(state, LUA_MATHLIBNAME, luaopen_math, 1);
    luaL_requiref(state, LUA_TABLIBNAME, luaopen_table, 1);

    lua_getglobal(state, LUA_GNAME);
    luaL_setfuncs(state, printlib, 0);
    lua_pop(state, 1);

    lua_newtable(state);  // create table for functions
    hooktable = luaL_ref(state,LUA_REGISTRYINDEX); // store said table in pseudo-registry

    luaL_requiref(state, "hook", luaopen_hook, 1);
    luaL_requiref(state, "KeyValues", luaopen_keyvalues, 1);
    LOAD_INTERFACE(IServerTools, state);
    luaReference<CBaseEntity>::registerType(state);
}

// debug function
static void dumpstack (lua_State *L) {
    int top=lua_gettop(L);
    for (int i=1; i <= top; i++) {
        Msg("%d\t%s\t", i, luaL_typename(L,i));
        switch (lua_type(L, i)) {
            case LUA_TNUMBER:
            Msg("%g\n",lua_tonumber(L,i));
            break;
            case LUA_TSTRING:
            Msg("%s\n",lua_tostring(L,i));
            break;
            case LUA_TBOOLEAN:
            Msg("%s\n", (lua_toboolean(L, i) ? "true" : "false"));
            break;
            case LUA_TNIL:
            Msg("%s\n", "nil");
            break;
            default:
            Msg("%p\n",lua_topointer(L,i));
            break;
        }
    }
}

void luaContext::callHook(const char* name, int nargs)
{
    lua_rawgeti(state,LUA_REGISTRYINDEX,hooktable);
    int type = lua_getfield(state, -1, name); // get function from hooktable 
    if (type == LUA_TNIL) {
        lua_pop(state, 2+nargs); // pop table and nill and arguments
        return;
    }
    lua_insert(state, -(nargs+2)); // place before arguments AND table
    lua_pop(state, 1); // pop hooktable
    int error = lua_pcall(state, nargs, 0, 0); // will output errors on garbage values. this is kinda intended
    if (error) {
        Warning("%s\n", lua_tostring(state, -1));
        lua_pop(state, 1);  /* pop error message from the stack */
    }
}

luaContext::~luaContext()
{
    lua_close(state);
}