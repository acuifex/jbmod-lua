#include "interfaces.h"
#include "luaReference.h"

#include "toolframework/itoolentity.h"

DEFINE_FUNCTION_INTERFACE(IServerTools, CreateEntityByName)
DEFINE_FUNCTION_INTERFACE(IServerTools, DispatchSpawn)
DEFINE_FUNCTION_INTERFACE(IServerTools, RemoveEntity)
DEFINE_FUNCTION_INTERFACE(IServerTools, GetBaseEntityByEntIndex)
DEFINE_FUNCTION_INTERFACE(IServerTools, FirstEntity)
DEFINE_FUNCTION_INTERFACE(IServerTools, NextEntity)
DEFINE_FUNCTION_INTERFACE(IServerTools, FindEntityByHammerID)

BEGIN_INTERFACE(IServerTools)
    APPEND_FUNCTION_INTERFACE(IServerTools, CreateEntityByName)
    APPEND_FUNCTION_INTERFACE(IServerTools, DispatchSpawn)
    APPEND_FUNCTION_INTERFACE(IServerTools, RemoveEntity)
    APPEND_FUNCTION_INTERFACE(IServerTools, GetBaseEntityByEntIndex)
    APPEND_FUNCTION_INTERFACE(IServerTools, FirstEntity)
    APPEND_FUNCTION_INTERFACE(IServerTools, NextEntity)
    APPEND_FUNCTION_INTERFACE(IServerTools, FindEntityByHammerID)
END_INTERFACE(IServerTools)