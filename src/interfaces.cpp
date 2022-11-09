#include "interfaces.h"

IServerTools* serverTools = nullptr;
void FindInterfaces(CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory)
{
	serverTools = (IServerTools *)gameServerFactory(VSERVERTOOLS_INTERFACE_VERSION, NULL);
}


