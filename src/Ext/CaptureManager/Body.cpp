#include "Body.h"

#include <Ext/TechnoType/Body.h>
template<> const DWORD Extension<CaptureManagerClass>::Canary = 0x23232323;
CaptureExt::ExtContainer CaptureExt::ExtMap;

void CaptureExt::ExtData::InitializeConstants()
{
	//init this here 
	if (auto pManagerOwnerExt = TechnoTypeExt::ExtMap.Find(OwnerObject()->Owner->GetTechnoType()))
		this->OverloadCount = pManagerOwnerExt->Overload_Count.Get(RulesClass::Instance->OverloadCount);
}

template <typename T>
void CaptureExt::ExtData::Serialize(T& Stm)
{
	Stm
		.Process(this->OverloadCount)
		;
}
void CaptureExt::ExtData::LoadFromStream(PhobosStreamReader& Stm)
{
	Extension<CaptureManagerClass>::LoadFromStream(Stm);
	this->Serialize(Stm);
}

void CaptureExt::ExtData::SaveToStream(PhobosStreamWriter& Stm)
{
	Extension<CaptureManagerClass>::SaveToStream(Stm);
	this->Serialize(Stm);
}

void CaptureExt::ExtContainer::InvalidatePointer(void* ptr, bool bRemoved) { }

bool CaptureExt::LoadGlobals(PhobosStreamReader& Stm)
{
	return Stm
		.Success();
}

bool CaptureExt::SaveGlobals(PhobosStreamWriter& Stm)
{
	return Stm
		.Success();
}

CaptureExt::ExtContainer::ExtContainer() : Container("CaptureManagerClass") { }
CaptureExt::ExtContainer::~ExtContainer() = default;

DEFINE_HOOK(0x471832, CaptureManagerClass_CTOR, 0x9)
{
	GET(CaptureManagerClass* const, pThis, ESI);

	CaptureExt::ExtMap.FindOrAllocate(pThis);

	return 0;
}

DEFINE_HOOK(0x4729E1, CaptureManagerClass_DTOR, 0xD)
{

	GET(CaptureManagerClass* const, pThis, ESI);

	CaptureExt::ExtMap.Remove(pThis);

	return 0;
}

DEFINE_HOOK_AGAIN(0x4728E0, CaptureManagerClass_SaveLoad_Prefix, 0x5)
DEFINE_HOOK(0x472720, CaptureManagerClass_SaveLoad_Prefix, 0x7)
{
	GET_STACK(CaptureManagerClass*, pThis, 0x4);
	GET_STACK(IStream*, pStm, 0x8);

	CaptureExt::ExtMap.PrepareStream(pThis, pStm);

	return 0;
}

DEFINE_HOOK(0x472781, CaptureManagerClass_Load_Suffix, 0xB)
{
	CaptureExt::ExtMap.LoadStatic();
	return 0;
}

DEFINE_HOOK(0x4728F8, CaptureManagerClass_Save_Suffix, 0x8)
{
	CaptureExt::ExtMap.SaveStatic();
	return 0;
}