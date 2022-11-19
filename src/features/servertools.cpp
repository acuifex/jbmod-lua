#include "servertools.h"

#include "cbase.h"

#include "interfaces.h"
#include "luaReference.h"

static int CreateEntityByName(lua_State* L) {
    // const char* name = luaL_checkstring(L, 1);
    // CBaseEntity* ent = serverTools->CreateEntityByName(name);
    // luaReference<CBaseEntity>::create(L, ent);
    // return 1;
    return call(L, serverTools, &IServerTools::CreateEntityByName);
}

static int DispatchSpawn(lua_State* L) {
    return call(L, serverTools, &IServerTools::DispatchSpawn);
}

static int GetBaseEntityByEntIndex(lua_State* L) {
    return call(L, serverTools, &IServerTools::GetBaseEntityByEntIndex);
}

static const struct luaL_Reg lib [] = {
    {"CreateEntityByName", CreateEntityByName},
    {"DispatchSpawn", DispatchSpawn},
    {"GetBaseEntityByEntIndex", GetBaseEntityByEntIndex},
    {NULL, NULL} /* end of array */
};

int luaopen_servertools (lua_State *L) {
  luaL_newlib(L, lib);
  return 1;
}