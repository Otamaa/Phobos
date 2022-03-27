#include "Hooks.Otamaa.h"

/**/
DEFINE_HOOK(0x415EEE, AircraftClass_ParadropCargo_Dont, 0x8)
{
	GET(AircraftClass*, pThis, EDI);

	auto const pTypeExt = TechnoTypeExt::ExtMap.Find(pThis->Type);

	if (pThis->Passengers.FirstPassenger)
	{
		if (pTypeExt && pTypeExt->AnotherData.Paradrop_DropPassangers.Get())
		{
			pThis->DropOffParadropCargo();
			return 0x415EFD;
		}
	}

	return 0x415F08;
}

namespace ReplaceParadropWithFire
{
	void __fastcall DropCarGo(AircraftClass* pThis, void* _)
	{
		pThis->Fire(pThis->Target, 0);
	}
};

DEFINE_POINTER_CALL(0x4159FB, &ReplaceParadropWithFire::DropCarGo);

static void __fastcall TriggerCrashWeapon(TechnoClass* pThis, void* _, int nMult)
{
	if (auto pType = pThis->GetTechnoType())
	{
		auto const pTypeExt = TechnoTypeExt::ExtMap.Find(pType);

		if (auto const pWeapon = pTypeExt->AnotherData.CrashWeapon.Get())
		{
			if (BulletClass* pBullet = pWeapon->Projectile->CreateBullet(pThis->GetCell(), pThis,
				pWeapon->Damage, pWeapon->Warhead, 0, pWeapon->Bright))
			{
				const CoordStruct& coords = pThis->GetCoords();

				pBullet->SetWeaponType(pWeapon);
				pBullet->Limbo();
				pBullet->SetLocation(coords);
				pBullet->Explode(true);
				pBullet->UnInit();
			}

		}
		else
			pThis->FireDeathWeapon(nMult);

		if (pType->DestroyAnim.Count > 0)
		{
			auto const facing = pThis->PrimaryFacing.current().value256();
			int idxAnim = 0;

			if (!pTypeExt->DestroyAnim_Random.Get())
			{
				if (pType->DestroyAnim.Count >= 8)
				{
					idxAnim = pType->DestroyAnim.Count;
					if (pType->DestroyAnim.Count % 2 == 0)
						idxAnim *= static_cast<int>(facing / 256.0);
				}
			}
			else
			{
				if(pType->DestroyAnim.Count > 1)
					idxAnim = ScenarioClass::Instance->Random.RandomRanged(0, (pType->DestroyAnim.Count - 1));
			}

			if (AnimTypeClass* pAnimType = pType->DestroyAnim[idxAnim])
			{
				if (auto const pAnim = GameCreate<AnimClass>(pAnimType, pThis->GetCoords()))
				{
					auto const pAnimTypeExt = AnimTypeExt::ExtMap.Find(pAnim->Type);
					auto const pAnimExt = AnimExt::ExtMap.Find(pAnim);

					if (!pAnimTypeExt || !pAnimExt)
						return;

					AnimExt::SetAnimOwnerHouseKind(pAnim, nullptr, pThis->Owner);

					pAnimExt->FromDeathUnit = true;

					if (pAnimTypeExt->CreateUnit_InheritDeathFacings.Get())
						pAnimExt->DeathUnitFacing = facing;

					if (pAnimTypeExt->CreateUnit_InheritTurretFacings.Get())
					{
						if (pThis->HasTurret())
						{
							pAnimExt->DeathUnitHasTurret = true;
							pAnimExt->DeathUnitTurretFacing = pThis->SecondaryFacing.current();
						}
					}
				}
			}
		}
	}
}

DEFINE_POINTER_CALL(0x4CD809, &TriggerCrashWeapon);

DEFINE_HOOK(0x5B2778, BulletClass_ProjectileMotion_AscentAngle, 0x7)
{//auto pBullet = _fetchCall::ProjMotion_bulletContext;
	R->Stack(0x18, (Math::clamp((0x4000 - 0x2000), 0, 0x4000)));

	return 0x5B277F;
}

DEFINE_HOOK(0x5B260B, BulletClass_ProjectileMotion_DescentAngle, 0x7)
{//auto pBullet = _fetchCall::ProjMotion_bulletContext;
	GET_BASE(bool, bIsVeryHigh, 0x18);
	GET_STACK(int, nPointLength, 0x38);

	int DescentAngle = 3;
	return nPointLength <=
		((bIsVeryHigh ? 6 : DescentAngle) << 8)
		? 0x5B289C : 0x5B2627;
}

DEFINE_HOOK(0x5B2721, BulletClass_ProjectileMotion_Cruise, 0x5)
{//auto pBullet = _fetchCall::ProjMotion_bulletContext;
	GET(int, nLepton, EAX);

	bool bLockedOnTrajectory = false;
	int nCruiseLevel = 5;

	if (bLockedOnTrajectory || nLepton >= nCruiseLevel)
		nLepton = nCruiseLevel;

	R->EAX(nLepton);
	return 0x5B2732;
}

DEFINE_HOOK(0x466BBC, BulletClass_AI_MissileROTVar, 0x6)
{
	GET(BulletClass*, pThis, EBP);
	GET(RulesClass*, pRules, ECX);

	auto const pBulletTypeExt = BulletTypeExt::ExtMap.Find(pThis->Type);
	double nVar = pBulletTypeExt->AnotherData.MissileROTVar.Get(pRules->MissileROTVar);
	R->ESI(&nVar);

	return 0x466BC2;
}

DEFINE_HOOK(0x466E9F, BulletClass_AI_MissileSafetyAltitude, 0x6)
{
	GET(BulletClass*, pThis, EBP);
	GET(int, comparator, EAX);

	auto const& pBulletTypeExt = BulletTypeExt::ExtMap.Find(pThis->Type);
	return comparator >= pBulletTypeExt->AnotherData.MissileSafetyAltitude.Get(RulesGlobal->MissileSafetyAltitude)
		? 0x466EAD : 0x466EB6;
}

DEFINE_HOOK(0x416545, AircraftClass_Fire_AttackRangeSight_1, 0x7)
{
	GET(AircraftClass*, pThis, EDI);
	GET(RulesClass*, pRules, EAX);

	R->Stack(STACK_OFFS(0x94, 0x48), R->ECX());
	auto pTypeExt = TechnoTypeExt::ExtMap.Find(pThis->Type);
	R->ECX(pTypeExt->AnotherData.AttackingAircraftSightRange.Get(pRules->AttackingAircraftSightRange));

	return 0x41654C;
}

DEFINE_HOOK(0x416580, AircraftClass_Fire_AttackRangeSight_2, 0x7)
{
	GET(AircraftClass*, pThis, EDI);
	GET(RulesClass*, pRules, ECX);

	R->Stack(STACK_OFFS(0x8C, 0x48), R->EDX());
	auto pTypeExt = TechnoTypeExt::ExtMap.Find(pThis->Type);
	R->EDX(pTypeExt->AnotherData.AttackingAircraftSightRange.Get(pRules->AttackingAircraftSightRange));

	return 0x416587;
}

DEFINE_HOOK(0x4156F1, AircraftClass_Mission_SpyplaneApproach_camerasound, 0x6)
{
	GET(RulesClass* const, pRules, EAX);
	GET(AircraftClass* const, pThis, ESI);

	const auto pTypeExt = TechnoTypeExt::ExtMap.Find(pThis->Type);

	R->ECX(pTypeExt->AnotherData.SpyplaneCameraSound.Get(pRules->SpyPlaneCamera));

	return 0x4156F7;
}

DEFINE_HOOK(0x442A2A, BuildingClass_ReceiveDamage_RotateVsAircraft, 0x8)
{
	GET(BuildingClass* const, pThis, ESI);
	GET(RulesClass* const, Rules, ECX);

	auto const pStructureExt = BuildingTypeExt::ExtMap.Find(pThis->Type);
	auto const& nData = pStructureExt->AnotherTypeData;
	R->AL(nData.PlayerReturnFire.Get(Rules->PlayerReturnFire));

	return 0x442A30;
}

DEFINE_HOOK(0x6634F6, RocketLocomotionClass_ILocomotion_DrawMatrix_CustomMissile, 0x6)
{
	enum { Handled = 0x66351B, Skip = 0x0 };

	GET(AircraftTypeClass*, pType, ECX);
	const auto pExt = TechnoTypeExt::ExtMap.Find(pType);

	if (pExt->AnotherData.IsCustomMissile)
	{
		R->EAX(&pExt->AnotherData.CustomMissileData);
		return Handled;
	}

	return Skip;
}

DEFINE_HOOK(0x662720, RocketLocomotionClass_ILocomotion_Process_Raise, 0x6)
{
	enum { Handled = 0x6624C8, Skip = 0x0 };

	GET(RocketLocomotionClass*, pThis, ESI);

	{
		if (auto pAir = specific_cast<AircraftClass*>(pThis->Owner))
		{
			const auto pExt = TechnoTypeExt::ExtMap.Find(pAir->Type);
			{
				bool bRaise = pAir->Veterancy.IsElite() ? pExt->AnotherData.CustomMissileEliteRaise.Get() : pExt->AnotherData.CustomMissileRaise.Get();
				if (pExt->AnotherData.IsCustomMissile && !bRaise)
				{
					return Handled;
				}
			}
		}
	}

	return Skip;
}

DEFINE_HOOK(0x6FD0A6, TechnoClass_RearmDelay_RandomROF, 0x5)
{
	GET(TechnoClass*, pThis, ESI);

	int nResult = 0;
	auto const pExt = TechnoTypeExt::ExtMap.Find(pThis->GetTechnoType());

	if (pExt && pExt->AnotherData.ROF_Random.Get())
	{
		auto const nData = pExt->AnotherData.Rof_RandomMinMax.Get({ 0,2 });
		Point2D nBuff { 0, 2 };

		if (nData != nBuff)
		{
			if (nData.X && nData.Y)
			{
				nBuff = Point2D { (abs(nData.X), abs(nData.Y)) };

				if (nBuff.X > nBuff.Y)
					std::swap(nBuff.X, nBuff.Y);
			}
		}

		nResult = (ScenarioClass::Instance->Random.RandomRanged(nBuff.X, nBuff.Y));
	}

	R->EAX((double)nResult);
	return 0x6FD0B5;
}

DEFINE_HOOK_AGAIN(0x4DABAB, ObjectClass_WasFallingDown, 0x6)
DEFINE_HOOK(0x4DABBC, ObjectClass_WasFallingDown, 0x6)
{
	GET(ObjectClass*, pThis, ESI);

	if (!pThis || pThis->IsFallingDown)
		return 0x0;

	if (auto const pTechno = generic_cast<TechnoClass*>(pThis))
	{
		if (auto const pTechnoTypeExt = TechnoTypeExt::ExtMap.Find(pTechno->GetTechnoType()))
		{
			auto pDecidedAnim = pTechnoTypeExt->AnotherData.Landing_Anim.Get();
			if (auto const pCell = pTechno->GetCell())
			{
				if (!pCell->ContainsBridge() && pCell->LandType == LandType::Water)
					pDecidedAnim = pTechnoTypeExt->AnotherData.Landing_AnimOnWater.Get();
			}

			if (pDecidedAnim)
			{
				auto const nCoord = pTechno->GetCenterCoord();
				if (auto pAnim = GameCreate<AnimClass>(pDecidedAnim, nCoord, 1, 1, 0x600, 0, 0))
					AnimExt::SetAnimOwnerHouseKind(pAnim, pTechno->GetOwningHouse(), nullptr, false);
			}
		}
	}

	return 0x0;
}

DEFINE_HOOK(0x6F3B2E, TechnoClass_Transform_FLH, 0x6)
{
	GET(WeaponStruct*, nWeaponStruct, EAX);
	GET(TechnoClass*, pThis, EBX);
	GET_STACK(int, idxWeapon, 0x8);

	CoordStruct nRet = nWeaponStruct->FLH;

	if (auto const pInf = specific_cast<InfantryClass*>(pThis))
	{
		if (pInf->Crawling)
		{
			auto const pTypeExt = TechnoTypeExt::ExtMap.Find(pThis->GetTechnoType());
			auto const& nFLH = pTypeExt->AnotherData.CrawlFLHData;

			if (!pThis->Veterancy.IsElite())
			{
				if (idxWeapon == 0)
					nRet = nFLH.PrimaryCrawlFLH.Get(nWeaponStruct->FLH);
				else
					nRet = nFLH.SecondaryCrawlFLH.Get(nWeaponStruct->FLH);
			}
			else
			{
				if (idxWeapon == 0)
					nRet = nFLH.Elite_PrimaryCrawlFLH.Get(nWeaponStruct->FLH);
				else
					nRet = nFLH.Elite_SecondaryCrawlFLH.Get(nWeaponStruct->FLH);
			}
		}
	}

	R->ECX(nRet.X);
	R->EBP(nRet.Y);
	R->EAX(nRet.Z);

	return 0x6F3B37;
}

DEFINE_HOOK(0x5F5A86, ObjectClass_SpawnParachuted_Animation_Bulet, 0x6)
{
	GET(RulesClass*, pRules, ECX);
	GET(BulletClass*, pBullet, ESI);

	auto const pBulletTypeExt = BulletTypeExt::ExtMap.Find(pBullet->Type);

	R->EDX(pBulletTypeExt->AnotherData.Parachute.Get(pRules->BombParachute));
	return 0x5F5A8C;
}

DEFINE_HOOK(0x5F53E5, ObjectClass_ReceiveDamage_HitAnim, 0x8)
{
	// prevent desync because too many animation created each time
	//if (SessionGlobal.GameMode == GameMode::Internet || SessionGlobal.GameMode == GameMode::LAN)
	//	if ((Unsorted::CurrentFrame % 10))
	//		return 0x0;

	GET(ObjectClass*, pThis, ESI);

	//switch (pThis->WhatAmI())
	//{
	//case AbstractType::Unit:
	//case AbstractType::Aircraft:
	//case AbstractType::Terrain:
	//case AbstractType::Overlay:
	//case AbstractType::Infantry:
	//case AbstractType::Building:
	//case AbstractType::VeinholeMonster:
	//	break;
	//default:
	//	return 0x0;
	//	break;
	//}

	GET_STACK(TechnoClass*, pAttacker, STACK_OFFS(0x24, -0x10));
	GET_STACK(WarheadTypeClass*, pWarhead, STACK_OFFS(0x24, -0xC));
	GET_STACK(bool, bIgnoreDefense, STACK_OFFS(0x24, -0x14));
	GET_STACK(HouseClass*, pAttackerHouse, STACK_OFFS(0x24, -0x1C));
	GET(int*, pDamage, EDI);

	if (pWarhead)
	{
		auto const pWarheadExt = WarheadTypeExt::ExtMap.Find(pWarhead);
		auto const pTechno = generic_cast<TechnoClass*>(pThis);
		auto const pType = pThis->GetType();
		bool bImmune_pt2 = false;
		bool const bImmune_pt1 =
			(pThis->IsIronCurtained() && !bIgnoreDefense) ||
			(pType->Immune && !bIgnoreDefense) || pThis->InLimbo
			;

		if (pTechno)
		{
			const auto pExt = TechnoExt::ExtMap.Find(pTechno);
			bImmune_pt2 = (pExt->Shield.get() && pExt->Shield.get()->IsActive())
				|| pTechno->TemporalTargetingMe
				|| (pTechno->ForceShielded && !bIgnoreDefense)
				|| pTechno->BeingWarpedOut
				|| pTechno->IsSinking
				;
		}

		if (!bImmune_pt1 && !bImmune_pt2 && (*pDamage > 0))
		{
			auto const nArmor = pType->Armor;
			auto const pArmor = ArmorTypeClass::FindFromIndex((int)nArmor);

			if (pWarheadExt && (!pWarheadExt->AnotherData.ArmorHitAnim.empty()) && pArmor)
			{
				AnimTypeClass* pAnimTypeDecided = pWarheadExt->AnotherData.ArmorHitAnim[(int)nArmor];

				if (!pAnimTypeDecided && pArmor->DefaultTo != -1)
				{
					pAnimTypeDecided = pWarheadExt->AnotherData.ArmorHitAnim[pArmor->DefaultTo];

					if (!pAnimTypeDecided)
					{
						//Holy shit !
						for (auto pDefArmor = ArmorTypeClass::FindFromIndex(pArmor->DefaultTo);
							pDefArmor && pDefArmor->DefaultTo != -1;
							pDefArmor = ArmorTypeClass::FindFromIndex(pDefArmor->DefaultTo))
						{
							pAnimTypeDecided = pWarheadExt->AnotherData.ArmorHitAnim[pDefArmor->DefaultTo];
							if (pAnimTypeDecided)
								break;
						}
					}
				}

				if (pAnimTypeDecided)
				{
					//if (Phobos::Config::MoreDetailSLDebugLog)
					//	Debug::Log("Hitting Victinm[%s] with[%s] Armor[%s] hitAnim[%s] \n", pType->get_ID(), pWarhead->get_ID(), pArmor->Name.data(), pAnimTypeDecided->get_ID());

					CoordStruct nBuffer { 0, 0 , 0 };
					if (pTechno)
					{
						auto const pTechnoTypeExt = TechnoTypeExt::ExtMap.Find(pTechno->GetTechnoType());

						if (!pTechnoTypeExt->AnotherData.HitCoordOffset.empty())
						{
							if ((pTechnoTypeExt->AnotherData.HitCoordOffset.size() > 1) && pTechnoTypeExt->AnotherData.HitCoordOffset_Random.Get())
								nBuffer = pTechnoTypeExt->AnotherData.HitCoordOffset[ScenarioGlobal->Random(0, pTechnoTypeExt->AnotherData.HitCoordOffset.size() - 1)];
							else
								nBuffer = pTechnoTypeExt->AnotherData.HitCoordOffset[0];
						}
					}

					auto const nCoord = pThis->GetCenterCoord() + nBuffer;
					if (auto pAnimPlayed = GameCreate<AnimClass>(pAnimTypeDecided, nCoord))
						AnimExt::SetAnimOwnerHouseKind(pAnimPlayed, pAttacker ? pAttacker->GetOwningHouse() : pAttackerHouse, pThis->GetOwningHouse(), false);
				}
			}
		}
	}

	return 0;
}