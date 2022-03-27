#include "Body.h"

template<> const DWORD Extension<EBoltExt::base_type>::Canary = 0x87654121;
EBoltExt::ExtContainer EBoltExt::ExtMap;

void EBoltExt::ExtData::InitializeConstants()
{
}

// ============================ =
// load / save

template <typename T>
void EBoltExt::ExtData::Serialize(T & Stm)
{
	Stm
		;
}

void EBoltExt::ExtData::LoadFromStream(PhobosStreamReader& Stm)
{
	Extension<EBoltExt::base_type>::LoadFromStream(Stm);
	this->Serialize(Stm);
}

void EBoltExt::ExtData::SaveToStream(PhobosStreamWriter& Stm)
{
	Extension<EBoltExt::base_type>::SaveToStream(Stm);
	this->Serialize(Stm);
}

void EBoltExt::ExtContainer::InvalidatePointer(void* ptr, bool bRemoved) { }

bool EBoltExt::LoadGlobals(PhobosStreamReader& Stm)
{
	return Stm
		.Success();
}

bool EBoltExt::SaveGlobals(PhobosStreamWriter& Stm)
{
	return Stm
		.Success();
}
// =============================
// container

EBoltExt::ExtContainer::ExtContainer() : Container("EBolt") { };
EBoltExt::ExtContainer::~ExtContainer() = default;

// =============================
// container hooks

DEFINE_HOOK(0x4C1E42, EBolt_CTOR, 0x5)
{
	GET(EBolt*, pItem, EAX);
	EBoltExt::ExtMap.FindOrAllocate(pItem);

	return 0;
}

DEFINE_HOOK(0x4C2951, EBolt_DTOR, 0x5)
{
	GET(EBolt*, pItem, ECX);
	EBoltExt::ExtMap.Remove(pItem);
	return 0;
}

DEFINE_HOOK(0x4C24BE, EBolt_Draw_Color1, 0x5)
{
	GET_STACK(EBolt*, pItem, 0x40);
	auto ext = EBoltExt::ExtMap.Find(pItem);
	if (ext && ext->Color_1 != ColorStruct::Empty)
	{
		R->EAX(Drawing::Color16bit(ext->Color_1));
		return 0x4C24E4;
	}
	return 0;
}

DEFINE_HOOK(0x4C25CB, EBolt_Draw_Color2, 0x5)
{
	GET_STACK(EBolt*, pItem, 0x40);
	auto ext = EBoltExt::ExtMap.Find(pItem);
	if (ext && ext->Color_2 != ColorStruct::Empty)
	{
		R->Stack<int>(0x18, Drawing::Color16bit(ext->Color_2));
		return 0x4C25FD;
	}
	return 0;
}

DEFINE_HOOK(0x4C26C7, EBolt_Draw_Color3, 0x5)
{
	GET_STACK(EBolt*, pItem, 0x40);
	auto ext = EBoltExt::ExtMap.Find(pItem);
	if (ext && ext->Color_3 != ColorStruct::Empty)
	{
		R->EBX(R->EBX() - 2);
		R->EAX(Drawing::Color16bit(ext->Color_1));
		return 0x4C26EE;
	}

	return 0;
}