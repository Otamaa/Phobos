#include <TechnoClass.h>

#include <Ext/WeaponType/Body.h>
//#include <Misc/DynamicPatcher/Techno/ExtraFire/ExtraFirefunctional.h>
#include  "../Techno/SpawnSupport/SpawnSupportFunctional.h"
#include "../Techno/GiftBox/GiftBoxFunctional.h"
#include <Misc/Otamaa/Misc/DynamicPatcher/Techno/JumjetFaceTarget/JJFacingToTargetFunctional.h>
#include <Misc/Otamaa/Misc/DynamicPatcher/Techno/Passengers/PassengersFunctional.h>
#include <Misc/Otamaa/Misc/DynamicPatcher/Techno/FireSW/FireSWFunctional.h>
#include <Misc/Otamaa/Misc/DynamicPatcher/Techno/AircraftDive/AircraftDiveFunctional.h>
#include <Misc/Otamaa/Misc/DynamicPatcher/Techno/AircraftPut/AircraftPutDataFunctional.h>

//OnInit
DEFINE_HOOK(0x6F42ED, TechnoClass_Init_DP, 0xA)
{
	GET(TechnoClass*, pThis, ESI);

	auto const pTypeExt = TechnoTypeExt::ExtMap.Find(pThis->GetTechnoType());
	auto const pExt = TechnoExt::ExtMap.Find(pThis);

	GiftBoxFunctional::Init(pExt, pTypeExt);
	AircraftDiveFunctional::Init(pExt, pTypeExt);

	if (pTypeExt->AnotherData.VirtualUnit.Get())
	{
		pThis->UpdatePlacement(PlacementType::Remove);
		pThis->IsOnMap = false;
		pThis->GetTechnoType()->DontScore = true;
		pThis->GetTechnoType()->Selectable = false;
		pThis->GetTechnoType()->Immune = true;
	}

	return 0x0;
}

DEFINE_HOOK(0x5F45A0, TechnoClass_Selectable_DP, 0x5)
{
	GET(TechnoClass*, pThis, EDI);

	bool Slectable = true;

	if (pThis)
	{
		auto pTypeExt = TechnoTypeExt::ExtMap.Find(pThis->GetTechnoType());

		Slectable |= pTypeExt->AnotherData.VirtualUnit.Get();
	}

	return Slectable ? 0x0 : 0x5F45A9;
}

DEFINE_HOOK(0x6FC339, TechnoClass_CanFire_DP, 0x6)
{
	GET(TechnoClass*, pThis, ESI);
	GET(WeaponTypeClass*, pWeapon, EDI);
	GET_STACK(AbstractClass*, pTarget, STACK_OFFS(0x20, -0x4));
	bool bCeaseFire = false;

	PassengersFunctional::CanFire(pThis, bCeaseFire);

	return bCeaseFire ? 0x6FCB7E : 0x0;
}

namespace CalculatePinch
{
	void Calc(TechnoClass* pFirer, int nWeaponIdx)
	{
		auto pWeapon = pFirer->GetWeapon(nWeaponIdx);
		if (pWeapon && pWeapon->WeaponType)
		{
			auto ext = WeaponTypeExt::ExtMap.Find(pWeapon->WeaponType);

			if (ext && (ext->AnotherData.RockerPitch.Get() > 0.0f))
			{
				double halfPI = Math::PI / 2;
				double theta = 0;

				if (pFirer->HasTurret())
				{
					double turretRad = pFirer->GetRealFacing().current().radians() - halfPI;
					double bodyRad = pFirer->PrimaryFacing.current().radians() - halfPI;
					Matrix3D matrix3D = Matrix3D(true);
					matrix3D.RotateZ((float)turretRad);
					matrix3D.RotateZ((float)-bodyRad);
					theta = matrix3D.GetZRotation();
				}

				double gamma = (double)ext->AnotherData.RockerPitch.Get();
				int lrSide = 1;
				int fbSide = 1;
				if (theta < 0)
				{
					lrSide *= -1;
				}
				if (theta >= halfPI || theta <= -halfPI)
				{
					fbSide *= -1;
				}

				double pitch = gamma;
				double roll = 0.0;
				if (theta != 0)
				{
					if (Math::sin(halfPI - theta) == 0.0)
					{
						pitch = 0.0;
						roll = gamma * lrSide;
					}
					else
					{
						double l = Math::cos(gamma);
						double y = l / Math::sin(halfPI - theta);
						double z = Math::sin(gamma);
						double lyz = Math::sqrt(std::pow(y, 2) + std::pow(z, 2));
						pitch = Math::acos(std::abs(y) / lyz) * fbSide;
						roll = (gamma - std::abs(pitch)) * lrSide;
					}
				}
				pFirer->RockingForwardsPerFrame = -(float)pitch;
				pFirer->RockingSidewaysPerFrame = (float)roll;
			}
		}
	}
}

DEFINE_HOOK(0x6FDD50, TechnoClass_Fire_DP, 0x6)
{
	GET(TechnoClass*, pThis, ECX);
	GET_STACK(int, nWeapon, 0x8);
	GET_STACK(AbstractClass*, pTarget, 0x4);

	//Debug::Log_WithBool(true, __FUNCTION__" Executed For[%s] WeaponIdx[%d] aveTarget[%s] \n", pThis->get_ID(), nWeapon, pTarget ? "Yes" : "No");
	CalculatePinch::Calc(pThis, nWeapon);
	//ExtraFirefunctional::GetWeapon(pThis, pTarget ,nWeapon);
	auto const pType = pThis->GetTechnoType();
	auto const pTypeExt = TechnoTypeExt::ExtMap.Find(pType);
	auto const pExt = TechnoExt::ExtMap.Find(pThis);

	FireSWFunctional::OnFire(pThis, pTarget, nWeapon);
	SpawnSupportFunctional::OnFire(pThis);
	AircraftDiveFunctional::OnFire(pExt, pTypeExt, pTarget, nWeapon);

	return 0x0;
}

DEFINE_HOOK(0x6F6CA0, TechnoClass_Put_DP, 0x7)
{
	GET(TechnoClass*, pThis, ECX);
	GET_STACK(CoordStruct*, pCoord, 0x4);
	GET_STACK(Direction, faceDir, 0x8);

	auto const pType = pThis->GetTechnoType();
	auto const pTypeExt = TechnoTypeExt::ExtMap.Find(pType);
	auto const pExt = TechnoExt::ExtMap.Find(pThis);

	AircraftPutDataFunctional::OnPut(pExt, pTypeExt, pCoord);

	return 0;
}

DEFINE_HOOK(0x6F9039, TechnoClass_Greatest_Threat_HealWeaponRange, 0x5)
{
	GET(TechnoClass*, pTechno, ESI);
	int guardRange = pTechno->GetTechnoType()->GuardRange;
	auto pirmary = pTechno->GetWeapon(0);

	if (pirmary && pirmary->WeaponType)
	{
		int range = pirmary->WeaponType->Range;
		if (range > guardRange)
		{
			guardRange = range;
		}
	}
	auto secondary = pTechno->GetWeapon(1);
	if (secondary && secondary->WeaponType)
	{
		int range = secondary->WeaponType->Range;
		if (range > guardRange)
		{
			guardRange = range;
		}
	}

	R->EDI(guardRange);
	return 0x6F903E;
}