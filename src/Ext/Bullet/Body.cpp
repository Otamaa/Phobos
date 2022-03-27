#include "Body.h"

#include <Ext/RadSite/Body.h>
#include <Ext/WeaponType/Body.h>
#include <Ext/BulletType/Body.h>

template<> const DWORD Extension<BulletClass>::Canary = 0x2A2A2A2A;
BulletExt::ExtContainer BulletExt::ExtMap;

void BulletExt::ExtData::InitializeConstants() { }
void BulletExt::ExtData::ApplyRadiationToCell(CellStruct Cell, int Spread, int RadLevel)
{
	auto const pThis = this->OwnerObject();
	auto const& Instances = RadSiteExt::Array;
	auto const pWeapon = pThis->GetWeaponType();
	auto const pWeaponExt = WeaponTypeExt::ExtMap.FindOrAllocate(pWeapon);
	auto const pRadType = pWeaponExt->RadType;
	auto const pThisHouse = pThis->Owner ? pThis->Owner->Owner : nullptr;
	bool CreateNew = true;

	if (Instances.Count > 0)
	{
		auto const it = std::find_if(Instances.begin(), Instances.end(),
			[=](RadSiteExt::ExtData* const pSite) // Lambda
			{// find
				return pSite->Type == pRadType &&
					pSite->OwnerObject()->BaseCell == Cell &&
					Spread == pSite->OwnerObject()->Spread;
			});

		if (it != Instances.end())
		{
			auto const pRadExt = *it;
			auto const pRadSite = pRadExt->OwnerObject();
			auto const nRadLevel = (int)RadSiteExt::GetRadLevelAt(pRadSite, Cell);
			auto nAmount = RadLevel;

			if (nRadLevel + RadLevel > pRadType->GetLevelMax())
				nAmount = pRadType->GetLevelMax() - nRadLevel;

			if (nAmount > 0)
			{
				CreateNew = false;
				RadSiteExt::Add(pRadSite, nAmount, pThisHouse);
			}
		}
	}

	if (CreateNew)
		RadSiteExt::CreateInstance(Cell, Spread, RadLevel, pWeaponExt, pThisHouse);
}

void BulletExt::InitializeLaserTrails(BulletClass* pThis)
{
	auto pExt = BulletExt::ExtMap.Find(pThis);

	if (pExt->LaserTrails.size())
		return;

	size_t nTotal = 0;
	if (auto pTypeExt = BulletTypeExt::ExtMap.Find(pThis->Type))
	{
		auto pOwner = pThis->Owner ? pThis->Owner->Owner : HouseClass::FindCivilianSide();

		for (auto const& idxTrail: pTypeExt->LaserTrail_Types)
		{
			if (auto const pLaserType = LaserTrailTypeClass::Array[idxTrail].get())
			{
				pExt->LaserTrails.push_back(
					std::make_unique<LaserTrailClass>(pLaserType, pOwner->LaserColor));
				++nTotal;
			}
		}
	}

	if (nTotal > 0)
		pExt->LaserTrails.resize(nTotal);
	else
		pExt->LaserTrails.clear();
}

void BulletExt::UpdateOwner(BulletClass* pThis)
{
	if (pThis->Owner && pThis->WeaponType)
	{
		auto const pWeaponExt = WeaponTypeExt::ExtMap.Find(pThis->WeaponType);

		if (pWeaponExt && pWeaponExt->DetachedFromOwner.Get())
			pThis->Owner = nullptr;
	}
}

// =============================
// load / save

template <typename T>
void BulletExt::ExtData::Serialize(T& Stm)
{
	Stm
		.Process(this->Intercepted)
		.Process(this->ShouldIntercept)
		.Process(this->LaserTrails)
		.Process(this->AnotherData)
		;
}

void BulletExt::ExtData::LoadFromStream(PhobosStreamReader& Stm)
{
	Extension<BulletClass>::LoadFromStream(Stm);
	this->Serialize(Stm);
}

void BulletExt::ExtData::SaveToStream(PhobosStreamWriter& Stm)
{
	Extension<BulletClass>::SaveToStream(Stm);
	this->Serialize(Stm);
}

void BulletExt::ExtContainer::InvalidatePointer(void* ptr, bool bRemoved) { }

bool BulletExt::LoadGlobals(PhobosStreamReader& Stm)
{
	return Stm
		.Success();
}

bool BulletExt::SaveGlobals(PhobosStreamWriter& Stm)
{
	return Stm
		.Success();
}

// =============================
// container

BulletExt::ExtContainer::ExtContainer() : Container("BulletClass") { }

BulletExt::ExtContainer::~ExtContainer() = default;

// =============================
// container hooks

DEFINE_HOOK(0x4664BA, BulletClass_CTOR, 0x5)
{
	GET(BulletClass*, pItem, ESI);

	BulletExt::ExtMap.FindOrAllocate(pItem);
	return 0;
}

DEFINE_HOOK(0x4665E9, BulletClass_DTOR, 0xA)
{
	GET(BulletClass*, pItem, ESI);

	BulletExt::ExtMap.Remove(pItem);
	return 0;
}

DEFINE_HOOK_AGAIN(0x46AFB0, BulletClass_SaveLoad_Prefix, 0x8)
DEFINE_HOOK(0x46AE70, BulletClass_SaveLoad_Prefix, 0x5)
{
	GET_STACK(BulletClass*, pItem, 0x4);
	GET_STACK(IStream*, pStm, 0x8);

	BulletExt::ExtMap.PrepareStream(pItem, pStm);

	return 0;
}

DEFINE_HOOK_AGAIN(0x46AF97, BulletClass_Load_Suffix, 0x7)
DEFINE_HOOK(0x46AF9E, BulletClass_Load_Suffix, 0x7)
{
	BulletExt::ExtMap.LoadStatic();
	return 0;
}

DEFINE_HOOK(0x46AFC4, BulletClass_Save_Suffix, 0x3)
{
	BulletExt::ExtMap.SaveStatic();
	return 0;
}
