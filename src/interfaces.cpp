#include "interfaces.h"

#include "toolframework/itoolentity.h"

void FindInterfaces(CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory)
{
	getInterface<IServerTools>::value = static_cast<IServerTools*>(gameServerFactory(VSERVERTOOLS_INTERFACE_VERSION, NULL));

}


