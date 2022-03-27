#include "Body.h"
#include <Ext/LineTrail/Body.h>

template<> const DWORD Extension<ObjectClass>::Canary = 0x3B3B3B3B;
ObjectExt::ExtContainer ObjectExt::ExtMap;

void ObjectExt::ExtContainer::InvalidatePointer(void* ptr, bool bRemoved) {}

bool ObjectExt::LoadGlobals(PhobosStreamReader& Stm)
{
	return Stm

		.Success();
}

bool ObjectExt::SaveGlobals(PhobosStreamWriter& Stm)
{
	return Stm

		.Success();
}

ObjectExt::ExtContainer::ExtContainer() : Container("ObjectClass") {}
ObjectExt::ExtContainer::~ExtContainer() = default;
/*
DEFINE_HOOK(0x5F39E8, ObjectClass_CTOR, 0xB)//
{
	GET(ObjectClass*, pThis, ESI);

	ObjectExt::ExtMap.FindOrAllocate(pThis);

	return 0;
}

*/