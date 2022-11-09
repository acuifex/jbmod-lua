#include "luabaseentity.h"

#include "cbase.h"

static int KeyValue(lua_State* L) {
    CBaseEntity* ent = (CBaseEntity*) luabaseentity.get(L);

    const char* k = luaL_checkstring(L, 2);
    const char* v = luaL_checkstring(L, 3);

    ent->KeyValue(k, v);
    return 0;
}


static int Precache(lua_State* L) {
    CBaseEntity* ent = (CBaseEntity*) luabaseentity.get(L);
    ent->Precache();
    return 0;
}


static int Activate(lua_State* L) {
    CBaseEntity* ent = (CBaseEntity*) luabaseentity.get(L);
    ent->Activate();
    return 0;
}


static const struct luaL_Reg baseentity_m [] = {
    {"KeyValue", KeyValue},
    {"Precache", Precache},
    {"Activate", Activate},
    {NULL, NULL}
};

luaReference luabaseentity("BaseEntity", baseentity_m);
