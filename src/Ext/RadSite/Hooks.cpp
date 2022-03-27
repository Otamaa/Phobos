#include "Body.h"

#include <BulletClass.h>
#include <HouseClass.h>
#include <InfantryClass.h>
#include <WarheadTypeClass.h>
#include <ScenarioClass.h>
#include <FootClass.h>

#include <Ext/BuildingType/Body.h>
#include <Ext/Bullet/Body.h>
#include <Ext/Rules/Body.h>
#include <Ext/Techno/Body.h>

#include <Utilities/Macro.h>
/*
	Custom Radiations
	Worked out from old uncommented Ares RadSite Hook , adding some more hook
	and rewriting some in order to make this working perfecly
	Credit : Ares Team , for unused/uncommented source of Hook.RadSite
						,RulesData_LoadBeforeTypeData Hook
			 Alex-B : GetRadSiteAt ,Helper that used at FootClass_AI & BuildingClass_AI
					  Radiate , Uncommented
			 me(Otamaa) adding some more stuffs and rewriting hook that cause crash

*/

DEFINE_HOOK(0x469150, BulletClass_Detonate_ApplyRadiation, 0x5)
{
	enum { Handled = 0x46920B, Continue = 0x0 };

	if (!Phobos::Config::DisableCustomRadSite)
	{
		GET(BulletClass* const, pThis, ESI);
		GET_BASE(CoordStruct const*, pCoords, 0x8);

		auto const pWeapon = pThis->GetWeaponType();

		if (pWeapon && pWeapon->RadLevel > 0)
		{
			auto const pExt = BulletExt::ExtMap.Find(pThis);
			pExt->ApplyRadiationToCell(
				CellClass::Coord2Cell(*pCoords),
				 (int)(pWeapon->Warhead->CellSpread),
				pWeapon->RadLevel);
		}

		return Handled;
	}

	return Continue;
}

//unused function , safeguard
DEFINE_HOOK(0x46ADE0, BulletClass_ApplyRadiation_NoBullet, 0x5)
{
	enum { Handled = 0x46AE5E, Continue = 0x0 };

	if (!Phobos::Config::DisableCustomRadSite)
	{
		GET_STACK(CellStruct, location, 0x4);
		GET_STACK(int, spread, 0x8);
		GET_STACK(int, amount, 0xC);
		auto const& Instances = RadSiteExt::Array;
		auto const pDefault = RadTypeClass::FindOrAllocate("Radiation");

		bool CreateNew = true;
		if (Instances.Count > 0)
		{
			auto const it = std::find_if(Instances.begin(), Instances.end(),
				[=](RadSiteExt::ExtData* const pSite) // Lambda
				{// find
					return pSite->Type == pDefault &&
						pSite->OwnerObject()->BaseCell == location &&
						spread == pSite->OwnerObject()->Spread;
				});

			if (it != Instances.end())
			{
				auto const pRadExt = *it;
				auto const pRadSite = pRadExt->OwnerObject();
				auto const nRadLevel = (int)RadSiteExt::GetRadLevelAt(pRadSite, location);
				auto nAmount = amount;

				if (nRadLevel + amount >= pDefault->GetLevelMax())
					nAmount = pDefault->GetLevelMax() - nRadLevel;

				if (nAmount > 0)
				{
					CreateNew = false;
					// Handle It
					RadSiteExt::Add(pRadSite, nAmount, nullptr);
				}
			}
		}

		if(CreateNew)
			RadSiteExt::CreateInstance(location, spread, amount, nullptr, nullptr);

		return Handled;
	}

	return Continue;
}

// Fix for desolator
DEFINE_HOOK(0x5213B4, InfantryClass_AIDeployment_CheckRad, 0x7)
{
	enum { FireCheck = 0x5213F4, SetMissionRate = 0x521484, Continue = 0x0 };

	if (!Phobos::Config::DisableCustomRadSite)
	{
		GET(InfantryClass*, pThis, ESI);
		int radLevel = 0;
		int weaponRadLevel = 0;

		if (auto pWeaponStruct = pThis->GetDeployWeapon())
		{
			if (auto pWeapon = pWeaponStruct->WeaponType)
			{
				auto const pWeaponExt = WeaponTypeExt::ExtMap.FindOrAllocate(pWeapon);
				auto currentCoord = Map.GetCellAt(pThis->GetCoords())->MapCoords;

				if (RadSiteExt::Array.Count > 0)
				{
					auto const it = std::find_if(RadSiteExt::Array.begin(), RadSiteExt::Array.end(),
						[=](RadSiteExt::ExtData* const pSite)
					{
						return
							pSite->Type == pWeaponExt->RadType &&
							pSite->OwnerObject()->BaseCell == currentCoord &&
							pSite->OwnerObject()->Spread == (int)(pWeapon->Warhead->CellSpread)
							;
					});

					if (it != RadSiteExt::Array.end())
					{
						auto pRadExt = *it;
						radLevel = (int)RadSiteExt::GetRadLevelAt(pRadExt->OwnerObject(), currentCoord);
					}
				}

				weaponRadLevel = pWeapon->RadLevel;
			}
		}

		return (!radLevel || (radLevel < (weaponRadLevel / 3))) ? FireCheck : SetMissionRate;
	}

	return Continue;
}

// Fix for desolator unable to fire his deploy weapon when cloaked
DEFINE_HOOK(0x521478, InfantryClass_AIDeployment_FireNotOKCloakFix, 0x4)
{
	GET(InfantryClass* const, pThis, ESI);

	AbstractClass* pTarget = nullptr; //default WWP nullptr

	auto const pWeapon = pThis->GetDeployWeapon()->WeaponType;

	if (pWeapon
		&& pWeapon->DecloakToFire
		&& (pThis->CloakState == CloakState::Cloaked || pThis->CloakState == CloakState::Cloaking))
	{
		// FYI this are hack to immedietely stop the Cloaking
		// since this function is always failing to decloak and set target when cell is occupied
		// something is wrong somewhere  # Otamaa
		pThis->CloakDelayTimer.Start((int)(pThis->Type->Sequence->GetSequence(DoType::DeployedFire).CountFrames * 900.0));

		if (auto pCell = Map.GetCellAt(pThis->GetCoords()))
			pTarget = pCell;

	}

	pThis->SetTarget(pTarget); //Here we go

	return 0x521484;
}

// Too OP, be aware
DEFINE_HOOK(0x43FB23, BuildingClass_AI, 0x5)
{
	if (!Phobos::Config::DisableCustomRadSite)
	{
		GET(BuildingClass* const, pBuilding, ECX);

		if (pBuilding->IsIronCurtained() ||
			pBuilding->Type->ImmuneToRadiation ||
			pBuilding->InLimbo ||
			pBuilding->BeingWarpedOut ||
			pBuilding->TemporalTargetingMe) {
			return 0;
		}

		auto buildingCoords = pBuilding->GetMapCoords();
		for (auto pFoundation = pBuilding->GetFoundationData(false);
			*pFoundation != CellStruct { 0x7FFF, 0x7FFF }; ++pFoundation)
		{
			CellStruct nCurrentCoord = buildingCoords + *pFoundation;

			for (auto& pRadExt : RadSiteExt::Array)
			{
				RadSiteClass* pRadSite = pRadExt->OwnerObject();
				RadTypeClass* pType = pRadExt->Type;

				// Check the distance, if not in range, just skip this one
				double orDistance = pRadSite->BaseCell.DistanceFrom(nCurrentCoord);
				if ((double)(pRadSite->Spread) < orDistance - 0.5)
					continue;

				int delay = pType->GetBuildingApplicationDelay();
				if ((delay == 0) || (Unsorted::CurrentFrame % delay != 0))
					continue;

				auto nRadLevel = RadSiteExt::GetRadLevelAt(pRadSite, nCurrentCoord);
				if (nRadLevel <= 0.0 || !pType->GetWarhead())
					continue;

				auto pWarhead = pType->GetWarhead();

				if (!pWarhead)
					Debug::Log("RadSite[%x] Has Missing Warhead ! \n", pRadSite);

				bool absolute = pWarhead->WallAbsoluteDestroyer;
				bool ignore = pBuilding->Type->Wall && absolute;
				auto damage = (int)((nRadLevel / 2) * pType->GetLevelFactor());

				if (pBuilding->IsAlive) // simple fix for previous issues
					if (pBuilding->ReceiveDamage(&damage, (int)(orDistance), pWarhead, nullptr, ignore, absolute, pRadExt->RadHouse) == DamageState::NowDead)
						break; //dont continue , meaningless
			}
		}
	}

	return 0;
}

// skip Frame % RadApplicationDelay
//DEFINE_LJMP(0x4DA554, 0x4DA56E);
DEFINE_HOOK(0x4DA554, FootClass_AI_SkipForCustomRad, 0x5)
{
	return !Phobos::Config::DisableCustomRadSite ? 0x4DA56E : 0x0;
}

// Hook Adjusted to support Ares RadImmune Ability check
DEFINE_HOOK(0x4DA59F, FootClass_AI_Radiation, 0x6)
{
	enum { CheckOtherState = 0x4DA63B, SkipEverything = 0x4DAF00, Continue = 0x0 };

	if (!Phobos::Config::DisableCustomRadSite)
	{
		GET(FootClass* const, pFoot, ESI);
		//RadSiteExt::ReceiveRadDamage(pFoot, , 0.7, false, pFoot->IsAlive && (!pFoot->IsSinking || !pFoot->IsCrashing));

		if (!pFoot->IsIronCurtained() && !pFoot->TemporalTargetingMe)
		{
			auto nCurrentCoord = pFoot->GetCell()->MapCoords;
			for (auto& pRadExt : RadSiteExt::Array)
			{
				RadSiteClass* pRadSite = pRadExt->OwnerObject();
				auto pRadType = pRadExt->Type;
				double orDistance = pRadSite->BaseCell.DistanceFrom(nCurrentCoord);

				if ((double)(pRadSite->Spread) < orDistance - 0.7)
					continue;

				int RadApplicationDelay = pRadType->GetApplicationDelay();

				if ((RadApplicationDelay <= 0) || (Unsorted::CurrentFrame % RadApplicationDelay != 0))
					continue;

				auto pWarhead = pRadType->GetWarhead();
				double nRadLevel = RadSiteExt::GetRadLevelAt(pRadSite, nCurrentCoord);

				if (nRadLevel <= 0.0)
					continue;

				int nDamage = (int)((nRadLevel)*pRadType->GetLevelFactor());

				if (!pWarhead)
					Debug::Log("RadSite[%x] Has Missing Warhead ! \n", pRadSite);

				if (pFoot->IsAlive && (!pFoot->IsSinking || !pFoot->IsCrashing))
					if (pFoot->ReceiveDamage(&nDamage, (int)(orDistance), pWarhead, nullptr, false, pWarhead->WallAbsoluteDestroyer, pRadExt->RadHouse) == DamageState::NowDead)
						break; //dont continue , meaningless
			}
		}
		//else
		//	if(pFoot->WhatAmI() != AbstractType::Aircraft)
		//		Debug::Log("Cannot Giving Affect to [%s] because OnMap[%s]Spawned[%s] ! \n",pFoot->get_ID(),pFoot->IsInPlayfield ? "True":"False",pFoot->Spawned ? "True":"False");

		return pFoot->IsAlive ? CheckOtherState : SkipEverything;
	}

	return Continue;
}

#define GET_RADSITE(reg, value)\
	GET(RadSiteClass* const, pThis, reg);\
	auto pExt = RadSiteExt::ExtMap.Find(pThis);\
	auto output = pExt->Type->## value ##;

DEFINE_HOOK(0x65B843, RadSiteClass_AI_LevelDelay, 0x6)
{
	enum { SetTimer = 0x65B849, Continue = 0x0 };

	if (!Phobos::Config::DisableCustomRadSite)
	{
		GET_RADSITE(ESI, GetLevelDelay());
		R->ECX(output);
		return SetTimer;
	}

	return Continue;
}

DEFINE_HOOK(0x65B8B9, RadSiteClass_AI_LightDelay, 0x6)
{
	enum { SetTimer = 0x65B8BF, Continue = 0x0 };

	if (!Phobos::Config::DisableCustomRadSite)
	{
		GET_RADSITE(ESI, GetLightDelay());
		R->ECX(output);
		return SetTimer;
	}

	return Continue;
}

DEFINE_HOOK(0x65BB67, RadSite_Deactivate, 0x6)
{
	enum { DevideValue = 0x65BB6D, Continue = 0x0 };

	if (!Phobos::Config::DisableCustomRadSite)
	{
		GET_RADSITE(ECX, GetLevelDelay());
		GET(int, val, EAX);

		R->EAX(val / output);
		R->EDX(val % output);

		return DevideValue;
	}

	return Continue;
}