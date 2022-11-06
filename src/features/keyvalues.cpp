#include "keyvalues.h"

#include "tier1/KeyValues.h"

static int keyvalues_new(lua_State* L) {
    const char* n = luaL_checkstring(L, 1);
    KeyValues* kv = new KeyValues(n);
    lua_pushlightuserdata(L, kv);

    luaL_getmetatable(L, "KeyValues");
    lua_setmetatable(L, -2);
    
    return 1;
}


static KeyValues *checkkeyvalues (lua_State *L) {
    void *ud = luaL_checkudata(L, 1, "KeyValues");
    luaL_argcheck(L, ud != NULL, 1, "`KeyValues' expected");
    return (KeyValues *)ud;
}

static int keyvalues_set(lua_State* L) {
    KeyValues* kv = checkkeyvalues(L);
    const char* n = luaL_checkstring(L, 2);
    switch (lua_type(L, 3)) {
        case LUA_TNUMBER:
        kv->SetInt(n, lua_tointeger(L, 3));
        break;
        case LUA_TSTRING:
        kv->SetString(n, lua_tostring(L, 3));
        break;
        default:
        // TODO: keyvalues can also support colors, booleans, and some other stuff
        luaL_argerror(L, 3, "integer or string expected");
        break;
    }
    return 0;
}

static int keyvalues_test(lua_State* L) {
    KeyValues* kv = checkkeyvalues(L);
    Msg( "root name: %s\n", kv->GetName() );
    for ( KeyValues *pKey = kv->GetFirstSubKey(); pKey; pKey = pKey->GetNextKey() )
    {
        Msg( "Key name: %s\n", pKey->GetName() );
    }
    return 0;
}

static int keyvalues_gc(lua_State* L) {
    KeyValues* kv = (KeyValues *)luaL_checkudata(L, 1, "KeyValues");

    // this feels dangerous.
    kv->deleteThis();
    return 0;
}

static const struct luaL_Reg keyvalueslib_f [] = {
    {"new", keyvalues_new},
    {NULL, NULL}
};

static const struct luaL_Reg keyvalueslib_t [] = {
    {"set", keyvalues_set},
    {"test", keyvalues_test},
    {NULL, NULL}
};

static const struct luaL_Reg keyvalueslib_m [] = {
    {"__index", NULL},
    {"__gc", keyvalues_gc},
    {NULL, NULL}
};

static void createmeta (lua_State *L) {
    luaL_newmetatable(L, "KeyValues"); // create metatable
    luaL_setfuncs(L, keyvalueslib_m, 0); // set meta functions
    luaL_newlib(L, keyvalueslib_t); // create table and assign methods
    lua_setfield(L, -2, "__index"); // assign method table to metatable
    lua_pop(L, 1); // pop metatable
}



int luaopen_keyvalues (lua_State *L) {
    createmeta(L);
    luaL_newlib(L, keyvalueslib_f);
    return 1;
}