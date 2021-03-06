#include "Body.h"
#include <RadSiteClass.h>
#include <BulletClass.h>
#include <HouseClass.h>
#include <MapClass.h>

#include "../../Enum/_Enumerator.hpp"
#include "../RadSite/Body.h"
#include "../../Enum/RadTypes.h"
#include "../BuildingType/Body.h"
#include "../Building/Body.h"
/*
				Custom Radiations 

	Worked out from old uncommented Ares RadSite Hook , adding some more hook 
	and rewriting some in order to make this working perfecly 

	Credit : Ares Team , for unused/uncommented source of Hook.RadSite
						,RulesData_LoadBeforeTypeData Hook

			 Alex-B : GetRadSiteAt ,Helper that used at FootClass_Update & BuildingClass_Update
					  Radiate , Uncommented 
			 me(Otamaa) adding some more stuffs and rewriting hook that cause crash
			
	TODO : -Enumerable Save/Load
			-Cell markings 
			-Testings 


	// some Ares Hook that need to be disabled to use this
	//4DA584 = FootClass_Update_RadImmune, 7
*/

//From Ares,Ext/Rules/hook.cpp
DEFINE_HOOK(679A15, RulesData_LoadBeforeTypeData, 6)
{
	GET_STACK(CCINIClass*, pINI, 0x4);
	RadType::AddDefaults(); //try initilize default items
	RadType::LoadListSection(pINI); //load the custom items
	
	return 0;
}

//5
//yeah , no
DEFINE_HOOK(469150, B_Detonate_ApplyRad , 5) {
	GET(BulletClass* const, pThis, ESI);
	GET_BASE(CoordStruct const*, pCoords, 0x8);

	auto const pWeapon = pThis->GetWeaponType();
	auto const pWH = pThis->WH;

	if (pWeapon) {
		if (pWeapon->RadLevel > 0) {
			auto const cell = CellClass::Coord2Cell(*pCoords);
			auto const spread = static_cast<int>(pWH->CellSpread);
			pThis->ApplyRadiationToCell(cell, spread, pWeapon->RadLevel);
		}
	}

	return 0x46920B;
}

//hack it here so we can use this globally if needed
DEFINE_HOOK(46ADE0, BulletClass_ApplyRadiation, 5)
{
	GET(BulletClass* const, pThis, ESI);
	GET_STACK(CellStruct, location, 0x4);
	GET_STACK(int, spread, 0x8);
	GET_STACK(int, amount, 0xC);

	auto const pWeapon = pThis->GetWeaponType();
	auto pWeaponExt = WeaponTypeExt::ExtMap.Find(pWeapon);
	auto pRadType = RadType::Array[0].get();
	auto& Instances = RadSiteExt::RadSiteInstance;
	//adding owner to RadSite ?
	// not tested working as intended or not 
	//but , why not ?
	auto const pThisHouse = pThis->Owner ? pThis->Owner->Owner : nullptr; 

	if (Instances.Count > 0) {
		auto const it = std::find_if(Instances.begin(), Instances.end(), [=](RadSiteExt::ExtData* const pSite) {
			return pSite->TypeIndex == unsigned(pWeaponExt->RadTypeIndex) &&
				   pSite->OwnerObject()->BaseCell == location &&
					pSite->Owner  == pThisHouse				&&
				   spread == pSite->OwnerObject()->Spread; 
		});
		if (it == Instances.end()) {
		
			RadSiteExt::CreateInstance(location, spread, amount, pWeaponExt , pThisHouse);
		}
		else {
			pRadType = RadType::Array[(*it)->TypeIndex].get();
			auto pRadSite = (*it)->OwnerObject();
			if (pRadSite->GetRadLevel() + amount > pRadType->GetLevelMax()) {
				amount = pRadType->GetLevelMax() - pRadSite->GetRadLevel();
			}
			int lvmax = pRadType->GetDurationMultiple();
			//Handle It 
			RadSiteExt::RadSiteAdd(pRadSite, lvmax, amount);
		
		}
	}
	else {
		
		RadSiteExt::CreateInstance(location, spread, amount, pWeaponExt , pThisHouse);
	}

	return 0x46AE5E;
}

//Too OP , be aware
DEFINE_HOOK(43FB23, BuildingClass_Update, 5) {
	GET(BuildingClass * const, pBld, ECX);
	
	auto MainCoords = pBld->GetMapCoords();

	for (auto pFoundation = pBld->GetFoundationData(false); *pFoundation != CellStruct{ 0x7FFF, 0x7FFF }; ++pFoundation) {
		CellStruct CurrentCoord = MainCoords + *pFoundation;

		for (auto pRadExt : RadSiteExt::RadSiteInstance) {
			RadSiteClass* pRadSite = pRadExt->OwnerObject();

			auto OrDistance = pRadSite->BaseCell.DistanceFrom(CurrentCoord);
			if (pRadSite->Spread < OrDistance - 0.5) {//Check the distance , if not in range , just skip this one
				continue;
			}

			auto const pType = pRadExt->Type;
			auto RadApplicationDelay = pType->GetBuildingApplicationDelay();
			if (Unsorted::CurrentFrame %  RadApplicationDelay == 0) {
					if (!pBld->IsIronCurtained() && !pBld->Type->ImmuneToRadiation && !pBld->InLimbo) {
				//	auto const RadLevel = pRadSite->GetRadLevel();
						auto RadLevel = RadSiteExt::GetRadLevelAt(pRadSite,CurrentCoord);//for more precise dmg calculation
					if (RadLevel > 0) {

					//	int first = pBld->Type->Strength;
						double RadLevelFactor = pType->GetLevelFactor();
						int Damage = static_cast<int>((RadLevel /2) *  RadLevelFactor );
					//	if (first > Damage) {Damage = first - Damage;} else if (Damage > first) { Damage = Damage - first;}
						int Distance = static_cast<int>(OrDistance);
						auto Warhead = pType->GetWarhead();
						if (Warhead) {
							//Prevent crash when bullet has no House Owner
							auto const pThisHouse = pRadExt->Owner ? pRadExt->Owner : HouseClass::FindCivilianSide();

							bool absolute = false; //will prevent passanger escapes
							bool ignore = false; //will ignore verses

							if (pBld->Type->Wall && Warhead->WallAbsoluteDestroyer) {
								 absolute =true ;
								 ignore = true;
							}
							else if (!pBld->Type->Wall && Warhead->WallAbsoluteDestroyer) {
								 absolute = true;
								 ignore = false;

							}
							

							pBld->ReceiveDamage(&Damage, Distance, Warhead, nullptr, ignore, absolute, pThisHouse);
						}
					}
				}
			}
		}
	}
	return 0;
}


//be aware that this function is updated every frame 
//putting debug log here can become mess because it gonna print bunch of debug line
// need to disable this from ares : 
//4DA584 = FootClass_Update_RadImmune, 7
DEFINE_HOOK(4DA554, FootClass_Update_RadSiteClass, 5)
{
	GET(FootClass* const, pThis, ESI);
	auto pCell = pThis->GetCell();

	for (auto const pRadExt : RadSiteExt::RadSiteInstance) {//Loop for each different radiation stored in the RadSites container


		auto pRadSite = pRadExt->OwnerObject();
		auto OrDistance = pRadSite->BaseCell.DistanceFrom(pCell->MapCoords);
		if (pRadSite->Spread < OrDistance - 0.7) {//Check the distance , if not in range , just skip this one
			continue;
		}
	
		auto const pType = pRadExt->Type;
		auto RadApplicationDelay = pType->GetApplicationDelay();
		if (Unsorted::CurrentFrame %  RadApplicationDelay == 0) {
			if (!pThis->GetTechnoType()->ImmuneToRadiation && !pThis->IsInAir() && !pThis->InLimbo && !pThis->IsIronCurtained() ) {
				//auto const RadLevel = pRadSite->GetRadLevel();
				auto RadLevel = RadSiteExt::GetRadLevelAt(pRadSite, pCell->MapCoords); //for more precise dmg calculation
				if (RadLevel > 0) {
					double RadLevelFactor = pType->GetLevelFactor();
					
					auto pWarhead = pType->GetWarhead();
					
					auto Damage =  static_cast<int>(RadLevel * RadLevelFactor);
					auto Distance = static_cast<int>(OrDistance);

					if (pWarhead) {
						//Prevent crash when bullet has no House Owner
				auto const pThisHouse = pRadExt->Owner ? pRadExt->Owner : HouseClass::FindCivilianSide();
			
					pThis->ReceiveDamage(&Damage, Distance, pWarhead, nullptr, false, true, pThisHouse);
					}
					
				}
			}
		}
	}
	return pThis->IsAlive ? 0x4DA63Bu : 0x4DAF00;
}


//6
//Otamaa :  rename it so it easier to sequence
DEFINE_HOOK(65B593, RadSiteClass_Activate_0, 6) {
	GET(RadSiteClass * const, pThis, ECX);
	auto pExt = RadSiteExt::ExtMap.Find(pThis);
	//for bug handle
	auto CurrentLevel = pThis->GetRadLevel();
	auto LevelDelay = pExt->Type->GetLevelDelay();
	auto LightDelay = pExt->Type->GetLightDelay();
	if (CurrentLevel < LevelDelay) {
		LevelDelay = CurrentLevel;
		LightDelay = LevelDelay;
	}
	//end handling
	R->ECX(LevelDelay);
	R->EAX(LightDelay);
	return 0x65B59F;
}

//
DEFINE_HOOK(65B5CE, RadSiteClass_Activate_1,6) {
	GET(RadSiteClass * const , pThis, ESI);

	auto pExt = RadSiteExt::ExtMap.Find(pThis);
	ColorStruct pRadColor = pExt->Type->GetColor();
	
	R->EAX(0);
	R->EDX(0);
	R->EBX(0);

	R->EDI(RulesClass::Instance); //point out the missing register -Otamaa

	R->DL(pRadColor.G);
	R->EBP(R->EDX());

	R->BL(pRadColor.B);
	R->AL(pRadColor.R);
	return 0x65B604;
}

//6
DEFINE_HOOK(65B63E, RadSiteClass_Activate_2, 6)
{
	GET(RadSiteClass * const , Rad, ESI);
	auto pExt = RadSiteExt::ExtMap.Find(Rad);
	auto factor = pExt->Type->GetLightFactor();

	__asm fmul factor;
	return 0x65B644;
}

//6
DEFINE_HOOK(65B6A0, RadSiteClass_Activate_3, 6)
{
	GET(RadSiteClass * const , Rad, ESI);
	
	auto pRadExt = RadSiteExt::ExtMap.Find(Rad);

	double factor = pRadExt->Type->GetTintFactor();

	__asm fmul factor;
	return 0x65B6A6;
}

//6
DEFINE_HOOK(65B6CA, RadSiteClass_Activate_4, 6)
{
	GET(RadSiteClass * const , Rad, ESI);

	auto pRadExt = RadSiteExt::ExtMap.Find(Rad);

	double factor = pRadExt->Type->GetTintFactor();

	__asm fmul factor;
	return 0x65B6D0;
}

//6
DEFINE_HOOK(65B6F2, RadSiteClass_Activate_5, 6)
{
	GET(RadSiteClass * const , Rad, ESI);

	auto pRadExt = RadSiteExt::ExtMap.Find(Rad);

	double factor = pRadExt->Type->GetTintFactor();

	__asm fmul factor;
	return 0x65B6F8;
}


//6
DEFINE_HOOK(65B843, RadSiteClass_Update_1, 6)
{
	GET(RadSiteClass * const, Rad, ESI);
	
	auto pRadExt = RadSiteExt::ExtMap.Find(Rad);
	
	auto delay = pRadExt->Type->GetLevelDelay();
	R->ECX(delay);
	return 0x65B849;
}
//6
DEFINE_HOOK(65B8B9, RadSiteClass_Update_2, 6)
{
	GET(RadSiteClass * const, Rad, ESI);

	auto pRadExt = RadSiteExt::ExtMap.Find(Rad);

	auto delay = pRadExt->Type->GetLightDelay();

	R->ECX(delay);
	return 0x65B8BF;
}

//Additional Hook below 
//some working , some cant fint the EXT cause crash
//65BB67  
DEFINE_HOOK(65BB67, RadSite_Deactivate, 6) {

	GET(RadSiteClass* const, Rad, ECX);
//	GET(RulesClass*, Rules, ESI);

	auto pRadExt = RadSiteExt::ExtMap.Find(Rad);

	auto delay = pRadExt->Type->GetLevelDelay();
	R->ESI(delay);//
	__asm xor edx  , edx; //fixing integer overflow crash
	__asm idiv esi;//
	return 0x65BB6D;
}

/*Cant find the Rad Ext 
//commented out ,Rewrited on RadsiteExt
//65B4F7
DEFINE_HOOK(65B4FC,RadSite_SetRadLevel , 6) {
	GET(RadSiteClass*, pRad, ECX);
	auto pRadExt = RadSiteExt::ExtMap.Find(pRad);

	auto delay = pRadExt->Type->GetDurationMultiple();

	R->EAX(delay);
	return 0x65B502;
}

//65B55F
DEFINE_HOOK(65B55F, RadSite_Add, 6) {

	GET(RadSiteClass*, pRad, ECX);
	auto pRadExt = RadSiteExt::ExtMap.Find(pRad);

	auto delay = pRadExt->Type->GetDurationMultiple();

	R->EAX(delay);

	return 0x65B565;
}*/
