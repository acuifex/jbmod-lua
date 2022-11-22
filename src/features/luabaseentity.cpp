#include <type_traits>
#include "cbase.h"

#include "luaReference.h"

// FIXME: probably should store EHANDLE

static int KeyValue(lua_State* L) {
    // hack to specify a function overload
    return luaReference<CBaseEntity>::callmethod(L, (bool (CBaseEntity::*)(const char *, const char *))&CBaseEntity::KeyValue);
}

static int SetModel(lua_State* L) {
    return luaReference<CBaseEntity>::callmethod(L, &CBaseEntity::SetModel);
}


static int IsMoving(lua_State* L) {
    return luaReference<CBaseEntity>::callmethod(L, &CBaseEntity::IsMoving);
}


static int Precache(lua_State* L) {
    CBaseEntity* ent = luaReference<CBaseEntity>::get(L);
    ent->Precache();
    return 0;
}


static int Activate(lua_State* L) {
    CBaseEntity* ent = luaReference<CBaseEntity>::get(L);
    ent->Activate();
    return 0;
}

template<>
const char* luaReference<CBaseEntity>::name = "CBaseEntity";
template<>
const struct luaL_Reg luaReference<CBaseEntity>::methods [] = {
    {"KeyValue", KeyValue},
    // {"SetRenderColor", SetRenderColor}, // we can't call non virtual functions unfortunately. use keyvalue lol
    {"IsMoving", IsMoving},
    {"SetModel", SetModel},
    {"Precache", Precache},
    {"Activate", Activate},
    {NULL, NULL}
};
