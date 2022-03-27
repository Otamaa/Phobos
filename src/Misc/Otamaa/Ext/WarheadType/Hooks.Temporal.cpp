#include <HouseClass.h>
#include <AnimClass.h>
#include <TemporalClass.h>
#include <BuildingClass.h>

#include <Ext/WeaponType/Body.h>
#include <Ext/Techno/Body.h>
#include <Ext/Anim/Body.h>
#include <Ext/AnimType/Body.h>
#include <Ext/WarheadType/Body.h>

DEFINE_HOOK(0x71AB47, TemporalClass_GetHelperDamage_AfterAres, 0x5)
{
	GET(WeaponStruct* const, Weapon, EAX);
	//GET(TemporalClass*, Temp, ESI);

	if (auto const TemporalWeapon = Weapon->WeaponType)
	{
		WarheadTypeExt::Temporal_WH = TemporalWeapon->Warhead;
		WeaponTypeExt::Temporal_WP = TemporalWeapon;
	}

	return 0;
}

DEFINE_HOOK(0x71AC50, TemporalClass_LetItGo_ExpireEffect, 0x5)
{
	GET(TemporalClass* const, pThis, ESI);

	if (auto const pTarget = pThis->Target)
	{
		pTarget->UpdatePlacement(PlacementType::Redraw);

		if (auto const Warhead = WarheadTypeExt::Temporal_WH)
		{
			auto const pTempOwner = pThis->Owner;
			auto const peWHext = WarheadTypeExt::ExtMap.Find(Warhead);

			if (auto pExpireAnim = peWHext->AnotherData.TemporalExpiredAnim.Get())
			{
				auto nCoord = pTarget->GetCenterCoord();
				if (auto const pAnim = GameCreate<AnimClass>(pExpireAnim, nCoord))
				{
					pAnim->ZAdjust = pTarget->GetZAdjustment() - 3;
					AnimExt::SetAnimOwnerHouseKind(pAnim, pTempOwner->Owner, pTarget->GetOwningHouse(), false);
				}
			}

			if (peWHext->AnotherData.TemporalExpiredApplyDamage.Get())
			{
				if (auto const pWeapon = WeaponTypeExt::Temporal_WP)
				{
					auto const pTargetStreght = pTarget->GetTechnoType()->Strength;
					auto const distance = pTempOwner->DistanceFrom(pTarget);
					auto damage = Game::F2I((pTargetStreght * ((1.0 - pThis->WarpRemaining / 10.0 / pTargetStreght)
						* (pWeapon->Damage * peWHext->AnotherData.TemporalDetachDamageFactor.Get()) / 100)));

					if (pTarget->IsAlive)
						pTarget->ReceiveDamage(&damage, distance, Warhead, pTempOwner, false, static_cast<bool>(ScenarioClass::Instance->Random(0, 1)), pTempOwner->Owner);
				}
			}
		}
	}

	return 0x71AC5D;
}

/*
DEFINE_HOOK(0x71ADE0, TemporalClass_Detach_Fix, 0x6)
{
	GET(TemporalClass* const, pThis, ECX);

	if (auto const pTarget = pThis->Target)
	{
		pThis->Target = nullptr;
		pTarget->BeingWarpedOut = false;
		pTarget->TemporalTargetingMe = nullptr;
	}

	if (auto const prevTemp = pThis->PrevTemporal)
	{
		if (prevTemp->NextTemporal == pThis)
			prevTemp->NextTemporal = nullptr;

		prevTemp->Detach();
	}

	if (auto const nextTemp = pThis->NextTemporal)
	{
		if (nextTemp->PrevTemporal == pThis)
			nextTemp->PrevTemporal = nullptr;

		nextTemp->Detach();
	}

	pThis->PrevTemporal = nullptr;
	pThis->NextTemporal = nullptr;
	pThis->SourceSW = nullptr;
	pThis->unknown_pointer_38 = nullptr;

	if (auto const pTech = pThis->Owner)
		pTech->EnterIdleMode(0, 1u);

	return 0x71AE49;
}*/

DEFINE_HOOK(0x71B14E, TemporalClass_Fire_ClearTarget, 0x9)
{
	GET(TemporalClass* const, pThis, ESI);

	if (auto const pTarget = pThis->Target)
	{
		if (auto const pTargetTemp = pTarget->TemporalImUsing)
		{
			if (pTargetTemp && pTargetTemp->Target)
				pTargetTemp->LetGo();

			auto pOwner = pThis->Owner;
			if (pOwner && pOwner->Owner && pOwner->Owner->IsPlayer())
				pTarget->Deselect();
		}
	}

	return 0x71B17B;
}

DEFINE_HOOK(0x71A9EE, TemporalClass_Update_RemoveBuildingTarget, 0x9)
{
	GET(TemporalClass* const, pThis, ESI);

	BuildingClass* BuildingTarget = nullptr;

	if (auto const pTarget = pThis->Target)
	{
		if (pTarget->IsSelected)
			pTarget->Deselect();

		BuildingTarget = specific_cast<BuildingClass*>(pTarget);
		if (BuildingTarget)
		{
			if (BuildingTarget->BunkerLinkedItem)
				BuildingTarget->UnloadBunker();

			if (BuildingTarget->Type->Helipad && BuildingTarget->RadioLinks.IsAllocated)
			{
				//for (auto const& items : make_iterator(BuildingTarget->RadioLinks))
				for (auto i = 0; i < BuildingTarget->RadioLinks.Capacity; ++i)
				{
					if (auto const items = BuildingTarget->GetNthLink(i))
					{
						if (auto const pAir = specific_cast<AircraftClass*>(items))
						{
							if (pAir->IsAlive && !pAir->InLimbo)
							{
								if (pAir->IsInAir() && pAir->Type->Crashable)
								{
									pAir->Crash(pThis->Owner);
								}
								else
								{
									//Ask plane to fly
									BuildingTarget->SendCommand(RadioCommand::AnswerLeave, pAir);
									pAir->DockedTo = nullptr;

								}
							}
						}
					}
				}
			}
		}
	}

	R->ECX(BuildingTarget);
	return 0x71AA1D;
}