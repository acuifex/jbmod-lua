#pragma once

#include "toolframework/itoolentity.h"

extern IServerTools* serverTools;

void FindInterfaces(CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory);