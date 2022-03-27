#include "Body.h"

template<> const DWORD Extension<AITriggerTypeClass>::Canary = 0xAAAAAAAA;
AITriggerTypeExt::ExtContainer AITriggerTypeExt::ExtMap;

// =============================
// load / save
template <typename T>
void AITriggerTypeExt::ExtData::Serialize(T& Stm)
{
		Stm 
			;
}

bool AITriggerTypeExt::LoadGlobals(PhobosStreamReader& Stm)
{
	return Stm
		.Success();
}

bool AITriggerTypeExt::SaveGlobals(PhobosStreamWriter& Stm)
{
	return Stm
		.Success();
}

void AITriggerTypeExt::ExtData::LoadFromStream(PhobosStreamReader& Stm)
{
	Extension<AITriggerTypeClass>::LoadFromStream(Stm);
	this->Serialize(Stm);
}

void AITriggerTypeExt::ExtData::SaveToStream(PhobosStreamWriter& Stm)
{
	Extension<AITriggerTypeClass>::SaveToStream(Stm);
	this->Serialize(Stm);
}

void AITriggerTypeExt::ExtContainer::InvalidatePointer(void* ptr, bool bRemoved) {}

// =============================
// container
AITriggerTypeExt::ExtContainer::ExtContainer() : Container("AITriggerTypeClass") {}
AITriggerTypeExt::ExtContainer::~ExtContainer() = default;

/*
DEFINE_HOOK(0x41E3FB, AITriggerTypeClass_CTOR, 0xC)
{
	GET(AITriggerTypeClass*, pThis, ESI);

	AITriggerTypeExt::ExtMap.FindOrAllocate(pThis);
	return 0;
}

DEFINE_HOOK(0x41E4E8, AITriggerTypeClass_DTOR, 0x7)
{

	GET(AITriggerTypeClass*, pThis, ESI);

	AITriggerTypeExt::ExtMap.Remove(pThis);
	return 0;
}

DEFINE_HOOK_AGAIN(0x41E540, AITriggerTypeClass_SaveLoad_Prefix, 0x5)
DEFINE_HOOK(0x41E5C0, AITriggerTypeClass_SaveLoad_Prefix, 0x8)
{
	GET_STACK(AITriggerTypeClass*, pItem, 0x4);
	GET_STACK(IStream*, pStm, 0x8);

	AITriggerTypeExt::ExtMap.PrepareStream(pItem, pStm);

	return 0;
}

DEFINE_HOOK(0x41E5A1, AITriggerTypeClass_Load_Suffix, 0x6)
{
	AITriggerTypeExt::ExtMap.LoadStatic();

	return 0;
}

DEFINE_HOOK(0x41E5DA, AITriggerTypeClass_Save_Suffix, 0x5)
{

	AITriggerTypeExt::ExtMap.SaveStatic();
	return 0;
}*/