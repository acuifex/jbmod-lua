#pragma once
#include "lua/lua.hpp"


template <int... Is>
struct indices {};

template <int N, int... Is>
struct build_indices : build_indices<N-1, N-1, Is...> {};

template <int... Is>
struct build_indices<0, Is...> : indices<Is...> {};

template<typename T>
static T getFromStack(lua_State* L, int index) = delete;

template<>
const char* getFromStack<const char*>(lua_State* L, int index) {
    return luaL_checkstring(L, index);
}

// template<>
// byte getFromStack<byte>(lua_State* L, int index) {
//     return luaL_checkinteger(L, index);
// }

template<typename T>
static void appendToStack(lua_State* L, T value) = delete;

template<>
void appendToStack<bool>(lua_State* L, bool value) {
    lua_pushboolean(L, value);
}

// i hate this
template <typename ret>
class superUglyHack {
public:
    template <typename t, typename... params, int... i>
    static int callmethod_impl(lua_State* L, t* that, ret (t::*f)(params...), indices<i...>) {
        // +2 is because it starts from zero, and we have this pointer on the stack
        appendToStack(L,(that->*f)(getFromStack<params>(L, i + 2)...));
        return 1;
    }
};
// doing all of this bullshit because of void
template <>
class superUglyHack<void> {
public:
    template <typename t, typename... params, int... i>
    static int callmethod_impl(lua_State* L, t* that, void (t::*f)(params...), indices<i...>) {
        (that->*f)(getFromStack<params>(L, i + 2)...);
        return 0;
    }
};
    // template<typename ret, typename t, typename... params>
    // static int callmethod(lua_State* L, luaReference<t> r, ret (T::*f)(params...)) {
    //     t* that = r.get(L);
    //     return superUglyHack<ret>::callmethod_impl(L, that, f, build_indices<sizeof...(params)>{});
    // }

template<typename T>
class luaReference {
private:
    luaReference() {} // disable creation
public:
    static const struct luaL_Reg methods [];
    static const char* name;

    static T* get (lua_State *L);
    static void create (lua_State *L, T* data);
    static void registerType (lua_State *L);

    template<typename ret, typename... params>
    static int callmethod(lua_State* L, ret (T::*f)(params...)) {
        return superUglyHack<ret>::callmethod_impl(L, get(L), f, build_indices<sizeof...(params)>{});
    }
};

template<typename T>
T* luaReference<T>::get(lua_State *L){
    void *ud = luaL_checkudata(L, 1, name);
    // TODO: output name
    luaL_argcheck(L, ud != NULL, 1, "custom type expected");
    return reinterpret_cast<T*>(ud);
}

template<typename T>
void luaReference<T>::create (lua_State *L, T* data) {
    lua_pushlightuserdata(L, static_cast<void*>(data));

    luaL_getmetatable(L, name);
    lua_setmetatable(L, -2);
}

template<typename T>
void luaReference<T>::registerType (lua_State *L) {
    luaL_newmetatable(L, name); // create metatable
    lua_newtable(L); // create table
    luaL_setfuncs(L,methods,0); // assign methods
    lua_setfield(L, -2, "__index"); // assign method table to metatable
    lua_pop(L, 1); // pop metatable
}