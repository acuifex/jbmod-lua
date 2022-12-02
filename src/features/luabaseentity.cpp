#include <type_traits>
#include "cbase.h"

#include "luaReference.h"

// FIXME: probably should store EHANDLE

DEFINE_FUNCTION_TYPE(CBaseEntity, Activate)
DEFINE_FUNCTION_TYPE(CBaseEntity, Precache)
DEFINE_FUNCTION_TYPE(CBaseEntity, IsMoving)
DEFINE_FUNCTION_TYPE(CBaseEntity, SetModel)
DEFINE_FUNCTION_TYPE(CBaseEntity, GetClassname)

DEFINE_FUNCTION_TYPE_CUSTOM(CBaseEntity, KeyValue) {
    // hack to specify a function overload
    return luaReference<CBaseEntity>::callmethod(L, (bool (CBaseEntity::*)(const char *, const char *))&CBaseEntity::KeyValue);
}

BEGIN_TYPE(CBaseEntity)
    APPEND_FUNCTION_TYPE(CBaseEntity, Activate)
    APPEND_FUNCTION_TYPE(CBaseEntity, Precache)
    APPEND_FUNCTION_TYPE(CBaseEntity, IsMoving)
    APPEND_FUNCTION_TYPE(CBaseEntity, SetModel)
    APPEND_FUNCTION_TYPE(CBaseEntity, KeyValue)
    APPEND_FUNCTION_TYPE(CBaseEntity, GetClassname)
END_TYPE(CBaseEntity)