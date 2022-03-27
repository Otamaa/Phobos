#include "AircraftDiveFunctional.h"

void AircraftDiveFunctional::Init(TechnoExt::ExtData* pExt, TechnoTypeExt::ExtData* pTypeExt)
{
	if (pExt->OwnerObject()->WhatAmI() != AbstractType::Aircraft)
		return;

	if (pTypeExt->AnotherData.MyDiveData.Enable)
	{
		pExt->AnotherData.MyDiveData.reset(
			GameCreate<AircraftDive>(pTypeExt->AnotherData.MyDiveData.Speed, pTypeExt->AnotherData.MyDiveData.Delay));
	}
}

void AircraftDiveFunctional::AI(TechnoExt::ExtData* pExt, TechnoTypeExt::ExtData* pTypeExt)
{
	if (!pExt)
		return;

	auto const pData = pExt->AnotherData.MyDiveData.get();

	if (!pData)
		return;

	auto const pTechno = pExt->OwnerObject();
	auto const pTarget = pTechno->Target;

	if (!pTarget || !pTechno->IsInAir())
	{
		pData->Reset();
		return;
	}

	CoordStruct location = pTechno->Location;
	CoordStruct targetPos = pTarget->GetCoords();

	int distance = pTypeExt->AnotherData.MyDiveData.Distance;
	int nBackupDistance = pTechno->GetTechnoType()->GuardRange;

	if (distance == 0)
	{
		int const weaponIndex = pTechno->SelectWeapon(pTarget);
		auto const pWeaponStruct = pTechno->GetWeapon(weaponIndex);

		if (!pWeaponStruct)
			distance = nBackupDistance;

		auto const pWeaponType = pWeaponStruct->WeaponType;

		if (!pWeaponType)
			distance = nBackupDistance;

		distance = pWeaponType->Range * 2;
	}

	if (location.DistanceFrom(targetPos) < distance && pData->CanDive)
	{
		int max = targetPos.Z + pTypeExt->AnotherData.MyDiveData.FlightLevel;
		int z = location.Z - pData->Diving();
		pTechno->Location.Z = z > max ? z : max;
	}
}

void AircraftDiveFunctional::OnFire(TechnoExt::ExtData* pExt, TechnoTypeExt::ExtData* pTypeExt, AbstractClass* pTarget, int nWeaponIDx)
{
	if (!pExt)
		return;

	if (pExt->OwnerObject()->WhatAmI() != AbstractType::Aircraft)
		return;

	auto const pData = pExt->AnotherData.MyDiveData.get();

	if (!pData)
		return;

	if (pTypeExt->AnotherData.MyDiveData.PullUpAfterFire)
		pData->CanDive = false;

}