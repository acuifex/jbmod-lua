#pragma once
#include "tier1/interface.h"

template <typename T>
struct getInterface {
	static T* value;
};

// TODO: this really should be undefined until we need it.
// forward declare without defining so the linker would be angry at us, instead of runtime crashes?
template<typename T>
T* getInterface<T>::value = nullptr;

void FindInterfaces(CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory);