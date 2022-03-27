#include "Body.h"

#include <New/Type/RadTypeClass.h>
#include <LightSourceClass.h>

template<> const DWORD Extension<RadSiteClass>::Canary = 0x87654321;
RadSiteExt::ExtContainer RadSiteExt::ExtMap;

DynamicVectorClass<RadSiteExt::ExtData*> RadSiteExt::Array;
void RadSiteExt::ExtData::InitializeConstants()
{
	this->Type = RadTypeClass::FindOrAllocate("Radiation");
}

void RadSiteExt::CreateInstance(CellStruct location, int spread, int amount, WeaponTypeExt::ExtData* pWeaponExt, HouseClass* const pOwner)
{
	// use real ctor
	auto const pRadSite = GameCreate<RadSiteClass>();
	auto pRadExt = RadSiteExt::ExtMap.FindOrAllocate(pRadSite);

	//Adding Owner to RadSite, from bullet
	if (pWeaponExt)
	{
		if (!pWeaponExt->Rad_NoOwner && pOwner)
			pRadExt->RadHouse = pOwner;

		pRadExt->Type = pWeaponExt->RadType;
	}

	pRadSite->SetBaseCell(&location);
	pRadSite->SetSpread(spread);
	RadSiteExt::SetRadLevel(pRadSite, amount);
	RadSiteExt::CreateLight(pRadSite);

	Array.AddUnique(pRadExt);
}

//RadSiteClass Activate , Rewritten
void RadSiteExt::CreateLight(RadSiteClass* pThis)
{
	auto const pRadExt = RadSiteExt::ExtMap.Find(pThis);
	auto nLevelDelay = pRadExt->Type->GetLevelDelay();
	auto nLightDelay = pRadExt->Type->GetLightDelay();
	auto nRadcolor = pRadExt->Type->GetColor();

	auto ConvertColor = [&pRadExt](BYTE nByteColor)
	{ return  (int)(Math::min(((1000 * nByteColor) / 255)* pRadExt->Type->GetTintFactor(), 2000.0)); };

	auto nLightFactor = Math::min(pThis->RadLevel * pRadExt->Type->GetLightFactor(), 2000.0);
	auto nDuration = pThis->RadDuration;

	pThis->RadLevelTimer.Start(nLevelDelay);
	pThis->RadLightTimer.Start(nLightDelay);

	//if(pRadExt->Type->GetApplicationDelay() > 0)
	//pRadExt->ApplycationDelay_I.Start(pRadExt->Type->GetApplicationDelay());

	//if(pRadExt->Type->GetBuildingApplicationDelay() > 0)
	//pRadExt->ApplycationDelay_B.Start(pRadExt->Type->GetBuildingApplicationDelay());

	pThis->Intensity = (int)(nLightFactor);
	pThis->LevelSteps = nDuration / nLevelDelay;
	pThis->IntensitySteps = nDuration / nLightDelay;
	pThis->IntensityDecrement = (int)(nLightFactor) / (nDuration / nLightDelay);

	TintStruct nTintBuffer{ ConvertColor(nRadcolor.R) ,ConvertColor(nRadcolor.G) ,ConvertColor(nRadcolor.B) };
	pThis->Tint = nTintBuffer;
	bool update = false;

	if (pThis->LightSource)
	{
		pThis->LightSource->ChangeLevels((int)(nLightFactor), nTintBuffer, update);
		pThis->Radiate();
	}
	else
	{
		auto const pCell = MapClass::Instance->TryGetCellAt(pThis->BaseCell);
		if (auto const pLight = GameCreate<LightSourceClass>(pCell->GetCoords(), pThis->SpreadInLeptons, (int)(nLightFactor), nTintBuffer))
		{
			pThis->LightSource = pLight;
			pLight->DetailLevel = 0;
			pLight->Activate(update);
			pThis->Radiate();
		}
	}
}

// Rewrite because of crashing craziness
void RadSiteExt::Add(RadSiteClass* pThis, int amount, HouseClass* pNewOwner)
{
	auto const pRadExt = RadSiteExt::ExtMap.Find(pThis);
	pRadExt->RadHouse = pNewOwner;
	pThis->Deactivate();
	pThis->RadLevel = ((pThis->RadLevel * pThis->RadTimeLeft) / pThis->RadDuration) + amount;
	pThis->RadDuration = pThis->RadLevel * pRadExt->Type->GetDurationMultiple();
	pThis->RadTimeLeft = pThis->RadDuration;
	RadSiteExt::CreateLight(pThis);
}

void RadSiteExt::SetRadLevel(RadSiteClass* pThis, int amount)
{
	auto const pRadExt = RadSiteExt::ExtMap.Find(pThis);
	const int nMult = pRadExt->Type->GetDurationMultiple();
	pThis->RadLevel = amount;
	pThis->RadDuration = nMult * amount;
	pThis->RadTimeLeft = nMult * amount;
}

// helper function provided by AlexB
double RadSiteExt::GetRadLevelAt(RadSiteClass* pThis, CellStruct const& cell)
{
	auto nMax = (double)pThis->SpreadInLeptons;
	auto nDist = Map.GetCellAt(cell)->GetCoords()
		.DistanceFrom(Map.GetCellAt(pThis->BaseCell)->GetCoords());
	auto nResult = ((nDist > nMax) ? 0.0 : (nMax - nDist) / nMax * pThis->RadLevel);
	auto nRadMax = (double)RadSiteExt::ExtMap.Find(pThis)->Type->GetLevelMax();
	return Math::clamp(nResult, 0.0, nRadMax);
}

void RadSiteExt::ExtData::UpdateTimer()
{
	if (this->Type->GetBuildingApplicationDelay() > 0 && this->ApplycationDelay_B.Completed())
		this->ApplycationDelay_B.Restart();

	if (this->Type->GetApplicationDelay() > 0 && this->ApplycationDelay_I.Completed())
		this->ApplycationDelay_I.Restart();
}

// =============================
// load / save

template <typename T>
void RadSiteExt::ExtData::Serialize(T& Stm)
{
	Stm
		.Process(this->RadHouse)
		.Process(this->Type)
		.Process(this->ApplycationDelay_I)
		.Process(this->ApplycationDelay_B)
		;
}

void RadSiteExt::ExtData::LoadFromStream(PhobosStreamReader& Stm)
{
	Extension<RadSiteClass>::LoadFromStream(Stm);
	this->Serialize(Stm);
}

void RadSiteExt::ExtData::SaveToStream(PhobosStreamWriter& Stm)
{
	Extension<RadSiteClass>::SaveToStream(Stm);
	this->Serialize(Stm);
}

void RadSiteExt::ExtContainer::InvalidatePointer(void* ptr, bool bRemoved) { }

bool RadSiteExt::LoadGlobals(PhobosStreamReader& Stm)
{
	return Stm
		.Success();
}

bool RadSiteExt::SaveGlobals(PhobosStreamWriter& Stm)
{
	return Stm
		.Success();
}

// =============================
// container

RadSiteExt::ExtContainer::ExtContainer() : Container("RadSiteClass") { };
RadSiteExt::ExtContainer::~ExtContainer() = default;

// =============================
// container hooks

DEFINE_HOOK(0x65B28D, RadSiteClass_CTOR, 0x6)
{
	if(!Phobos::Config::DisableCustomRadSite)
	{
		GET(RadSiteClass*, pThis, ESI);
		auto pRadSiteExt = RadSiteExt::ExtMap.FindOrAllocate(pThis);

		RadSiteExt::Array.AddUnique(pRadSiteExt);
	}

	return 0;
}

DEFINE_HOOK(0x65B2F4, RadSiteClass_DTOR, 0x5)
{
	if (!Phobos::Config::DisableCustomRadSite)
	{
		GET(RadSiteClass*, pThis, ECX);

		if (auto pRadSiteExt = RadSiteExt::ExtMap.Find(pThis))
			RadSiteExt::Array.Remove(pRadSiteExt);

		RadSiteExt::ExtMap.Remove(pThis);

	}

	return 0;
}

DEFINE_HOOK_AGAIN(0x65B3D0, RadSiteClass_SaveLoad_Prefix, 0x5)
DEFINE_HOOK(0x65B450, RadSiteClass_SaveLoad_Prefix, 0x8)
{
	if (!Phobos::Config::DisableCustomRadSite)
	{
		GET_STACK(RadSiteClass*, pItem, 0x4);
		GET_STACK(IStream*, pStm, 0x8);

		RadSiteExt::ExtMap.PrepareStream(pItem, pStm);
	}

	return 0;
}

DEFINE_HOOK(0x65B43F, RadSiteClass_Load_Suffix, 0x7)
{
	if (!Phobos::Config::DisableCustomRadSite)
		RadSiteExt::ExtMap.LoadStatic();

	return 0;
}

DEFINE_HOOK(0x65B464, RadSiteClass_Save_Suffix, 0x5)
{

	if (!Phobos::Config::DisableCustomRadSite)
		RadSiteExt::ExtMap.SaveStatic();

	return 0;
}