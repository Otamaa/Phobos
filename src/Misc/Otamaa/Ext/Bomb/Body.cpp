#include "Body.h"

template<> const DWORD Extension<BombClass>::Canary = 0x87659781;
BombExt::ExtContainer BombExt::ExtMap;

// =============================
// load / save

void BombExt::ExtData::LoadFromStream(PhobosStreamReader& Stm)
{
	Extension<BombClass>::LoadFromStream(Stm);
}

void BombExt::ExtData::SaveToStream(PhobosStreamWriter& Stm)
{
	Extension<BombClass>::SaveToStream(Stm);
}

void BombExt::ExtContainer::InvalidatePointer(void* ptr, bool bRemoved) { }

bool BombExt::LoadGlobals(PhobosStreamReader& Stm)
{
	return Stm
		.Success();
}

bool BombExt::SaveGlobals(PhobosStreamWriter& Stm)
{
	return Stm
		.Success();
}

// =============================
// container

BombExt::ExtContainer::ExtContainer() : Container("BombClass") { };
BombExt::ExtContainer::~ExtContainer() = default;

// =============================
// container hooks

// BombListClass::Plant()
DEFINE_HOOK_AGAIN(0x438EE9, BombClass_CTOR , 0x6)
DEFINE_HOOK(0x4385FC, BombClass_CTOR, 0x6) // is this inline ?
{
	GET(BombClass*, pItem, ESI);
	BombExt::ExtMap.FindOrAllocate(pItem);
	return 0;
}

DEFINE_HOOK(0x4393F2, BombClass_SDDTOR, 0x5)
{
	GET(BombClass *, pItem, ECX);
	BombExt::ExtMap.Remove(pItem);
	return 0;
}

DEFINE_HOOK_AGAIN(0x438B40, BombClass_SaveLoad_Prefix, 0x5)
DEFINE_HOOK(0x438BD0, BombClass_SaveLoad_Prefix, 0x8)
{
	GET_STACK(BombClass*, pItem, 0x4);
	GET_STACK(IStream*, pStm, 0x8);
	if (Phobos::Config::MoreDetailSLDebugLog)
	Debug::Log( __FUNCTION__" Called ! [%x] \n", pItem);
	BombExt::ExtMap.PrepareStream(pItem, pStm);
	return 0;
}

DEFINE_HOOK(0x438BBD, BombClass_Load_Suffix, 0xA)
{
	if (Phobos::Config::MoreDetailSLDebugLog)
	Debug::Log(__FUNCTION__" Called !\n");
	BombExt::ExtMap.LoadStatic();
	return 0;
}

DEFINE_HOOK(0x438BE4, BombClass_Save_Suffix, 0x5)
{
	if (Phobos::Config::MoreDetailSLDebugLog)
	Debug::Log(__FUNCTION__" Called !\n");
	BombExt::ExtMap.SaveStatic();
	return 0;
}