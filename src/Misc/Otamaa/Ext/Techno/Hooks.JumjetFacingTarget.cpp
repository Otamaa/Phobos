#include "Body.h"
#include <ExtraHeaders/DynamicPatcherHelper.h>
#include <Ext/TechnoType/Body.h>

// JumjetFacingToTarget 
// https://github.com/ChrisLv-CN/PatcherExtension/blob/main/MyExtension/JumpjetFaceToTarget.cs
// Author : ChrisLv-CN
// Port : Otamaa

DEFINE_HOOK(0x6FC339, UnitClass_CanFire, 0x6)
{
	GET(TechnoClass* const, pThis, ESI);
	GET(WeaponTypeClass* const, pWeapon, EDI);
	GET_STACK(AbstractClass*, pTarget, STACK_OFFS(0x20, -0x4));

	return TechnoExt::Scriptable_CanFire(pThis, pTarget, pWeapon, false) ? 0x6FCB7E : 0x0;
}
/*
DEFINE_HOOK(0x6F6CA0, TechnoClass_Put_Scriptable, 0x7)
{
	GET(TechnoClass* const, pThis, ECX);
	GET_STACK(CoordStruct*, pCoord, 0x4);
	GET_STACK(Direction, Dir, 0x8);

	UNREFERENCED_PARAMETER(pThis);
	UNREFERENCED_PARAMETER(pCoord);
	UNREFERENCED_PARAMETER(Dir);

	return 0;
}

DEFINE_HOOK(0x6FDD50, TechnoClass_Fire_Scriptable, 0x6)
{
	GET(TechnoClass* const, pThis, ECX);
	GET_STACK(AbstractClass*, pTarget, 0x4);
	GET_STACK(int, Wpidx, 0x8);

	UNREFERENCED_PARAMETER(pThis);
	UNREFERENCED_PARAMETER(pTarget);
	UNREFERENCED_PARAMETER(Wpidx);

	return 0;
}*/

CoordStruct ChrisLv_Helper::GetFLHAbsoluteCoords(TechnoClass* pTechno, CoordStruct flh, bool isOnTurret)
{
	auto res = pTechno->GetCoords();

	if (flh)
	{
		// Step 1: get body transform matrix
		Matrix3D matrix3D;
		auto const pFoot = abstract_cast<FootClass*>(pTechno);

		if (pFoot && pFoot->Locomotor)
		{
			// no locomotor means no rotation or transform of any kind (ex. buildings) - Kerbiter
			matrix3D = pFoot->Locomotor->Draw_Matrix(nullptr);
		}
		else
		{
			matrix3D.MakeIdentity();
		}

		// Step 2-3: Turret offset and rotation
		if (isOnTurret && pTechno->HasTurret())
		{
			TechnoTypeExt::ApplyTurretOffset(pTechno->GetTechnoType(), &matrix3D);

			double turretRad = (pTechno->TurretFacing().value32() - 8) * -(Math::PiD / 16);
			double bodyRad = (pTechno->PrimaryFacing.current().value32() - 8) * -(Math::PiD / 16);
			float angle = static_cast<float>(turretRad - bodyRad);

			matrix3D.RotateZ(angle);
		}
		// Step 4: apply FLH offset
		auto offset = GetFLHAbsoluteOffset(matrix3D, flh);
		res += offset;
	}

	return res;
}

bool TechnoExt::Scriptable_CanFire(TechnoClass* pThis, AbstractClass* pTarget, WeaponTypeClass* Weapon, bool checkCoord)
{
	auto const pThisExt = TechnoExt::ExtMap.Find(pThis);
	auto const pType = pThis->Type();
	auto const pTypeExt = TechnoTypeExt::ExtMap.Find(pType);

	if (!pTypeExt->JumpjetFacingToTarget.Get()
		|| pThis->WhatAmI() != AbstractType::Unit
		|| !pType->JumpJet
		|| pThis->Target != pTarget)
	{
		return false;
	}

	if (pThis->IsInAir())
	{
		auto nThiscoord = pThis->GetCoords();
		auto pCell = specific_cast<CellClass*>(pTarget);
		auto nTargetCoord = pCell ? CellClass::Cell2Coord(pCell->MapCoords) : pTarget->GetCoords();
		auto nTargetDir = ChrisLv_Helper::Point2Dir(nThiscoord, nTargetCoord);
		auto const nFaceCount = pTypeExt->JumjetFacingCount.Get();
		auto nTargetIdx = ChrisLv_Helper::Dir2FacingIndex(nTargetDir, nFaceCount);
		auto nThisIdx = ChrisLv_Helper::Dir2FacingIndex(pThis->GetRealFacing(), nFaceCount);
		if (nThisIdx  != nTargetIdx)
		{
			pThisExt->JumjetNeedToTurn = true;
			pThisExt->JumjetTurnTo = ChrisLv_Helper::DirNormalized(nTargetIdx, nFaceCount);

		}
		else
		{
			pThisExt->JumjetNeedToTurn = false;
		}
	}

	return false;
}

void TechnoExt::ApplyJumjetFacing(TechnoClass* pThis)
{
	if (!pThis)
		return;

	auto const pThisExt = TechnoExt::ExtMap.Find(pThis);
	auto const pTypeExt = TechnoTypeExt::ExtMap.Find(pThis->Type());

	if (pTypeExt->JumpjetFacingToTarget.Get() && pThisExt->JumjetNeedToTurn)
	{
		if (auto const pFoot = generic_cast<FootClass*>(pThis))
		{
			if (pFoot->GetCurrentSpeed() == 0)
			{
				pThisExt->JumjetNeedToTurn = false;
				pFoot->StopMoving();
				auto pThisloc = pThis->GetCoords();
				auto nVectorloc = ChrisLv_Helper::Dir2Vector(pThisExt->JumjetTurnTo);
				CoordStruct nBuffer = pThisloc + nVectorloc;
				CoordStruct nBufferLoc = pThisloc - nVectorloc;
				pFoot->Location = nBufferLoc;
				pFoot->MoveTo(&nBuffer);
			}
			else
			{
				pThisExt->JumjetNeedToTurn = false;
			}
		}
	}
}