#include "Body.h"

template<> const DWORD Extension<InfantryClass>::Canary = 0xE1E2E3E4;
InfantryExt::ExtContainer InfantryExt::ExtMap;

void InfantryExt::ExtData::InitializeConstants() { }
//so it can used globally
bool InfantryExt::IsOccupant(InfantryClass* thisTrooper)
{
    if (auto buildingBelow = thisTrooper->GetCell()->GetBuilding())
        return (buildingBelow->Occupants.FindItemIndex(thisTrooper) != -1);

    return false; // if there is no building, he can't occupy one
}

template <typename T>
void InfantryExt::ExtData::Serialize(T& Stm)
{
	//Stm
	//	;
};

void InfantryExt::ExtContainer::InvalidatePointer(void* ptr, bool bRemoved) {}

bool InfantryExt::LoadGlobals(PhobosStreamReader& Stm)
{
	return Stm
		.Success();
}

bool InfantryExt::SaveGlobals(PhobosStreamWriter& Stm)
{
	return Stm
		.Success();
}

void InfantryExt::ExtData::LoadFromStream(PhobosStreamReader& Stm)
{
	Extension<InfantryClass>::LoadFromStream(Stm);
	this->Serialize(Stm);

}

void InfantryExt::ExtData::SaveToStream(PhobosStreamWriter& Stm)
{
	Extension<InfantryClass>::SaveToStream(Stm);
	this->Serialize(Stm);
}


// =============================
// container

InfantryExt::ExtContainer::ExtContainer() : Container("InfantryClass") {}
InfantryExt::ExtContainer::~ExtContainer() = default;

// =============================
// container hooks

DEFINE_HOOK(0x517CB0, InfantryClass_CTOR, 0x5)
{
	GET(InfantryClass*, pItem, ESI);

	InfantryExt::ExtMap.FindOrAllocate(pItem);
	return 0;
}

DEFINE_HOOK(0x517F83, InfantryClass_DTOR, 0x6)
{
	GET(InfantryClass*, pItem, ESI);

	InfantryExt::ExtMap.Remove(pItem);
	return 0;
}

DEFINE_HOOK_AGAIN(0x521B00, InfantryClass_SaveLoad_Prefix, 0x8)
DEFINE_HOOK(0x521960, InfantryClass_SaveLoad_Prefix, 0x6)
{
	GET_STACK(InfantryClass*, pItem, 0x4);
	GET_STACK(IStream*, pStm, 0x8);

	InfantryExt::ExtMap.PrepareStream(pItem, pStm);

	return 0;
}

DEFINE_HOOK(0x521AEC, InfantryClass_Load_Suffix, 0x6)
{
	InfantryExt::ExtMap.LoadStatic();
	return 0;
}

DEFINE_HOOK(0x521B14, InfantryClass_Save_Suffix, 0x3)
{
	InfantryExt::ExtMap.SaveStatic();
	return 0;
}