#include "Body.h"
#include <WeaponTypeClass.h>
#include <BulletClass.h>

#include "../WeaponType/Body.h"
#include "../../Enum/RadTypes.h"
#include"../WarheadType/Body.h"

template<> const DWORD Extension<RadSiteClass>::Canary = 0x87654321;
RadSiteExt::ExtContainer RadSiteExt::ExtMap;

DynamicVectorClass<RadSiteExt::ExtData*> RadSiteExt::RadSiteInstance;
/*
		TODO : RadTypes Enum Save games 
				-Cell markings
				-Rewriting some stuffs 

*/
void RadSiteExt::CreateInstance(CellStruct location, int spread, int amount, WeaponTypeExt::ExtData *pWeaponExt) {
	auto const pRadSite = GameCreate<RadSiteClass>(location, spread, amount);

	//This one is special, because it replace the hook in "CTOR"
	auto pRadExt = RadSiteExt::ExtMap.FindOrAllocate(pRadSite);

	if (pRadExt->Weapon != pWeaponExt->OwnerObject()) {
		pRadExt->Weapon = pWeaponExt->OwnerObject();
		pRadExt->Type = &pWeaponExt->RadType;
	}

	pRadExt->Type->DebugLog("RadSite CreateInstance");
	pRadSite->SetBaseCell(&location);
	pRadSite->SetSpread(spread);

	RadSiteExt::SetRadLevel(pRadSite, pRadExt->Type, amount);

	pRadSite->Activate();

	if (RadSiteExt::RadSiteInstance.FindItemIndex(pRadExt) == -1) {
		RadSiteExt::RadSiteInstance.AddItem(pRadExt);
	}
}

//Rewrite because Of crashing crazy
void RadSiteExt::RadSiteAdd(RadSiteClass* pRad, int lvmax, int amount) {

	int value = pRad->RadLevel * pRad->RadTimeLeft / pRad->RadDuration;
	pRad->Deactivate();
	pRad->RadLevel = value + amount;
	int breaka = (value + amount) * lvmax;
	pRad->RadDuration = breaka;
	pRad->RadTimeLeft = breaka;
	pRad->Activate();
}


//write handler myself -Otamaa
//pRadSite->SetRadLevel(amount);
void RadSiteExt::SetRadLevel(RadSiteClass* pRad, RadType* Type , int amount) {
	
	int mult = Type->DurationMultiple;

	pRad->RadDuration = mult * amount;
	pRad->RadTimeLeft = mult * amount;
}

//helper function provided by AlexB
double RadSiteExt::GetRadLevelAt(RadSiteClass* pThis, CellStruct const& cell) {
	auto const base = MapClass::Instance->GetCellAt(pThis->BaseCell)->GetCoords();
	auto const coords = MapClass::Instance->GetCellAt(cell)->GetCoords();

	auto const dist = coords.DistanceFrom(base);
	auto const max = pThis->SpreadInLeptons;

	return dist > max ? 0.0 : (max - dist) / static_cast<double>(max)* pThis->RadLevel;
}

// =============================
// container

void RadSiteExt::ExtData::LoadFromStream(IStream* Stm) {

	char weaponID[0x18];
	PhobosStreamReader::Process(Stm, weaponID);
	Weapon = WeaponTypeClass::Find(weaponID);

	Debug::Log("RadSiteExt::LoadFromStream Load Weapon %s \n", Weapon ? Weapon->ID : NONE_STR);
	
	auto pWeaponTypeExt = WeaponTypeExt::ExtMap.FindOrAllocate(Weapon);
	if (pWeaponTypeExt) {
		Type = &pWeaponTypeExt->RadType;
		Type->DebugLog("RadSiteExt::LoadFromStream");
	}
}

void RadSiteExt::ExtData::SaveToStream(IStream* Stm) {
	PhobosStreamWriter::Process(Stm, this->Weapon->ID);

	Debug::Log("RadSiteExt::SaveToStream %s \n", this->Weapon->ID);
}

RadSiteExt::ExtContainer::ExtContainer() : Container("RadSiteClass") {};
RadSiteExt::ExtContainer::~ExtContainer() = default;

DEFINE_HOOK(65B28D, RadSiteClass_CTOR, 6) {
	GET(RadSiteClass*, pThis, ESI);
	GET_STACK(BulletClass *, pBullet, 0xC);
	if (!pBullet) return 0;

	bool isAllocate;
	
	auto pRadExt = RadSiteExt::ExtMap.FindOrAllocate(pThis, &isAllocate);
	if (isAllocate) {
		RadSiteExt::RadSiteInstance.AddItem(pRadExt);

		auto pWeaponType = pBullet->WeaponType;
		auto pWeaponTypeExt = WeaponTypeExt::ExtMap.FindOrAllocate(pWeaponType);

		pRadExt->Weapon = pWeaponType;
		pRadExt->Type = &pWeaponTypeExt->RadType;
		pRadExt->Type->DebugLog("RadSiteClass_CTOR");
	}
	else {
		Debug::Log("RadSiteClass_CTOR");
	}
	
	return 0;
}

DEFINE_HOOK(65B2F4, RadSiteClass_DTOR, 5) {
	GET(RadSiteClass *, pThis, ECX);
	auto pRadExt = RadSiteExt::ExtMap.Find(pThis);
	
	RadSiteExt::ExtMap.Remove(pThis);
	
	RadSiteExt::RadSiteInstance.Remove(pRadExt);
	return 0;
}

DEFINE_HOOK_AGAIN(65B3D0, RadSiteClass_SaveLoad_Prefix, 5)
DEFINE_HOOK(65B450, RadSiteClass_SaveLoad_Prefix, 8)
{
	GET_STACK(RadSiteClass*, pItem, 0x4);
	GET_STACK(IStream*, pStm, 0x8);

	RadSiteExt::ExtMap.PrepareStream(pItem, pStm);

	return 0;
}

 DEFINE_HOOK(65B43F, RadSiteClass_Load_Suffix, 7) {

	 auto pItem = RadSiteExt::ExtMap.Find(RadSiteExt::ExtMap.SavingObject);
	 IStream* pStm = RadSiteExt::ExtMap.SavingStream;

	 pItem->LoadFromStream(pStm);
	 return 0;
 }

DEFINE_HOOK(65B464, RadSiteClass_Save_Suffix, 5) {

	auto pItem = RadSiteExt::ExtMap.Find(RadSiteExt::ExtMap.SavingObject);
	IStream* pStm = RadSiteExt::ExtMap.SavingStream;

	pItem->SaveToStream(pStm);

	return 0;
}
