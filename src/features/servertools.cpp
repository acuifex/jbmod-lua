#include "servertools.h"

#include "luabaseentity.h"
#include "interfaces.h"

static int CreateEntityByName(lua_State* L) {
    const char* name = luaL_checkstring(L, 1);
    CBaseEntity* ent = serverTools->CreateEntityByName(name);
    luabaseentity.create(L, ent);
    return 1;
}

static int DispatchSpawn(lua_State* L) {
    void* ent = luabaseentity.get(L);
    serverTools->DispatchSpawn((CBaseEntity*)ent);
    return 0;
}

static const struct luaL_Reg lib [] = {
    {"CreateEntityByName", CreateEntityByName},
    {"DispatchSpawn", DispatchSpawn},
    {NULL, NULL} /* end of array */
};

int luaopen_servertools (lua_State *L) {
  luaL_newlib(L, lib);
  return 1;
}