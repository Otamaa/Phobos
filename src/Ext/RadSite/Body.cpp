#include "Body.h"
#include <WeaponTypeClass.h>

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
void RadSiteExt::CreateInstance(CellStruct location, int spread, int amount, WeaponTypeExt::ExtData *pWeaponExt ,HouseClass* pOwner) {
	//auto const pCell = MapClass::Instance->TryGetCellAt(location);
	auto& Instances = RadSiteExt::RadSiteInstance;
	auto const pRadSite = GameCreate<RadSiteClass>(location, spread, amount);
	//This one is special , because it replace the hook in "CTOR"
	auto pRadExt = RadSiteExt::ExtMap.FindOrAllocate(pRadSite);
	
	pRadExt->TypeIndex = pWeaponExt->RadTypeIndex;
	Debug::Log("Instance created Rad IDX is %d \n" ,pWeaponExt->RadTypeIndex);
	pRadExt->Type = RadType::Array[pWeaponExt->RadTypeIndex].get();
	Debug::Log("Instance created Rad is %s \n", pRadExt->Type->Name);
//	Debug::Log("Instance Rad Color = %d,%d,%d\n", pRadExt->Type->RadSiteColor.Get().R, pRadExt->Type->RadSiteColor.Get().G, pRadExt->Type->RadSiteColor.Get().B);
	pRadExt->Owner = pOwner;
	pRadSite->SetBaseCell(&location);
	pRadSite->SetSpread(spread);

	RadSiteExt::SetRadLevel(pRadSite, pRadExt->Type, amount);

	pRadSite->Activate();
	//pCell->SetRadSite(pRadSite); //hmm 

	if (Instances.FindItemIndex(pRadExt) == -1) {
		Instances.AddItem(pRadExt);
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

void RadSiteExt::SetRadLevel(RadSiteClass* pRad, RadType* Type , int amount) {
	
	int mult = Type->GetDurationMultiple();
//Commented out because the hook keep crashing 
//write handler myself -Otamaa
	//pRadSite->SetRadLevel(amount);
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

/*
void RadSiteExt::RadSiteClass_Radiate(RadSiteClass* pThis) {
	auto const spread = pThis->Spread;

	for (auto y = -spread; y <= spread; ++y) {
		for (auto x = -spread; x < spread; ++x) {
			auto const cell = pThis->BaseCell + CellStruct{ x, y };
			auto const amount = GetRadLevelAt(pThis, cell);

			auto const pCell = MapClass::Instance->GetCellAt(cell);
			pCell->RadLevel_Increase(amount);

		}
	}
}*/

// =============================
// container

void RadSiteExt::ExtData::LoadFromStream(IStream* Stm) {
	this->TypeIndex.Load(Stm);
	this->Type.Load(Stm);
	this->Owner.Load(Stm);
}

void RadSiteExt::ExtData::SaveToStream(IStream* Stm) {
   this->TypeIndex.Save(Stm);
	this->Type.Save(Stm);
	this->Owner.Save(Stm);
}

RadSiteExt::ExtContainer::ExtContainer() : Container("RadSiteClass") {
}

RadSiteExt::ExtContainer::~ExtContainer() = default;


/*
DEFINE_HOOK(65B28D, RadSiteClass_CTOR, 6) {
	GET(RadSiteClass*, pThis, ESI);
	GET_STACK(BulletClass *, pBullet, 0xC);
	auto pRadExt = RadSiteExt::ExtMap.FindOrAllocate(pThis);
	Debug::Log("RadSiteClass_CTOR: Allocated\n");
	
	if (pBullet) {
		auto pWeapon = pBullet->WeaponType;
		if (pWeapon) {
			auto pWeaponExt = WeaponTypeExt::ExtMap.Find(pWeapon);
			pRadExt->RadIndex = pWeaponExt->RadTypeIndex;
			pRadExt->Type = RadType::Array[pWeaponExt->RadTypeIndex].get();
			Debug::Log("RadSiteClass_CTOR:Current RadSite TypeIndex=%d, Section name = %s\n", pWeaponExt->RadTypeIndex, pRadExt->Type->Name);
		}
	}

	return 0;
}
*/


DEFINE_HOOK(65B2F4, RadSiteClass_DTOR, 5) {
	GET(RadSiteClass *, pThis, ECX);
	auto pRadExt = RadSiteExt::ExtMap.Find(pThis);
	
	RadSiteExt::ExtMap.Remove(pThis);
	
	RadSiteExt::RadSiteInstance.Remove(pRadExt);
	return 0;
}

//todo add more
//Enumerable save load
//Save load 
//65B3D0

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

