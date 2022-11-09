#include "luaReference.h"

#include "tier0/dbg.h"

luaReference::luaReference(const char* name, const struct luaL_Reg* methods) 
: name(name), methods(methods) {}

void* luaReference::get(lua_State *L){
    void *ud = luaL_checkudata(L, 1, name);
    // TODO: output name
    luaL_argcheck(L, ud != NULL, 1, "custom type expected");
    return ud;
}
void luaReference::create (lua_State *L, void* data) {
    lua_pushlightuserdata(L, data);

    luaL_getmetatable(L, name);
    lua_setmetatable(L, -2);
}
void luaReference::registerType (lua_State *L) {
    luaL_newmetatable(L, name); // create metatable
    luaL_setfuncs(L, meta, 0); // set meta functions
    luaL_newlib(L, methods); // create table and assign methods
    lua_setfield(L, -2, "__index"); // assign method table to metatable
    lua_pop(L, 1); // pop metatable
}