#include "Body.h"

template<> const DWORD Extension<TriggerTypeClass>::Canary = 0xAAAAAAAA;
TriggerTypeExt::ExtContainer TriggerTypeExt::ExtMap;

// =============================
// load / save
template <typename T>
void TriggerTypeExt::ExtData::Serialize(T& Stm)
{
		Stm
			//.Process(HouseIdx)
			;
}

bool TriggerTypeExt::LoadGlobals(PhobosStreamReader& Stm)
{
	return Stm
		.Success();
}

bool TriggerTypeExt::SaveGlobals(PhobosStreamWriter& Stm)
{
	return Stm
		.Success();
}

void TriggerTypeExt::ExtData::LoadFromStream(PhobosStreamReader& Stm)
{
	Extension<TriggerTypeClass>::LoadFromStream(Stm);
	this->Serialize(Stm);
}

void TriggerTypeExt::ExtData::SaveToStream(PhobosStreamWriter& Stm)
{
	Extension<TriggerTypeClass>::SaveToStream(Stm);
	this->Serialize(Stm);
}

void TriggerTypeExt::ExtContainer::InvalidatePointer(void* ptr, bool bRemoved) {}

// =============================
// container

TriggerTypeExt::ExtContainer::ExtContainer() : Container("TriggerTypeClass") {}
TriggerTypeExt::ExtContainer::~ExtContainer() = default;

/*
DEFINE_HOOK(0x726DD7, TriggerTypeClass_CTOR, 0x6)
{
	GET(TriggerTypeClass*, pThis, ESI);

	TriggerTypeExt::ExtMap.FindOrAllocate(pThis);
	return 0;
}

DEFINE_HOOK(0x726F46, TriggerTypeClass_DTOR, 0x5)
{

	GET(TriggerTypeClass*, pThis, ESI);

	TriggerTypeExt::ExtMap.Remove(pThis);
	return 0;
}

DEFINE_HOOK_AGAIN(0x727BF0, TriggerTypeClass_SaveLoad_Prefix, 0x5)
DEFINE_HOOK(0x727C80, TriggerTypeClass_SaveLoad_Prefix, 0x8)
{
	GET_STACK(TriggerTypeClass*, pItem, 0x4);
	GET_STACK(IStream*, pStm, 0x8);

	TriggerTypeExt::ExtMap.PrepareStream(pItem, pStm);

	return 0;
}

DEFINE_HOOK(0x727C75, TriggerTypeClass_Load_Suffix, 0x5)
{
	TriggerTypeExt::ExtMap.LoadStatic();

	return 0;
}

DEFINE_HOOK(0x727C9A, TriggerTypeClass_Save_Suffix, 0x5)
{

	TriggerTypeExt::ExtMap.SaveStatic();
	return 0;
}*/