#include "JJFacingToTargetFunctional.h"
#include <Ext/Techno/Body.h>
#include <Ext/TechnoType/Body.h>
#include "../../Helpers/Helpers.h"

void JJFacingFunctional::AI(TechnoExt::ExtData* pExt, TechnoTypeExt::ExtData* pTypeExt)
{
	if (auto const pFoot = generic_cast<FootClass*>(pExt->OwnerObject()))
	{
		if (pTypeExt->AnotherData.MyJJData.Enable
			&& pFoot->WhatAmI() == AbstractType::Unit
			&& pFoot->GetTechnoType()->JumpJet
			&& pFoot->IsInAir()
		)
		{
			if (pExt->AnotherData.MyJJData.NeedToTurn)
			{

				if (pFoot->GetCurrentSpeed() == 0)
				{
					pExt->AnotherData.MyJJData.Turning();
					//pFoot->Locomotor.get()->Stop_Moving();
					pFoot->StopMoving();
					CoordStruct location = pFoot->Location;

					if (auto const pCell = MapClass::Instance->TryGetCellAt(location))
					{
						CoordStruct nForward = { pTypeExt->AnotherData.MyJJData.Forward,0,0 };
						CoordStruct offsetLocation = Helpers_DP::GetFLHAbsoluteCoords(location, nForward, pExt->AnotherData.MyJJData.ToDir);
						pFoot->SetLocation(offsetLocation);
						pFoot->SetDestination(pCell, true);
						pFoot->QueueMission(Mission::Move, true);
					}
				}
				else
				{
					pExt->AnotherData.MyJJData.Cancel();
				}
					/*
				auto const pLoco = pFoot->Locomotor.get();
				if (!pLoco->Is_Moving_Now() && !pFoot->PrimaryFacing.in_motion())
					pLoco->Do_Turn(pExt->AnotherData.MyJJData.ToDir);

				pExt->AnotherData.MyJJData.Turning();*/
			}
			else if (auto pTarget = pFoot->Target)
			{

				int nWeapon = pFoot->SelectWeapon(pTarget);
				bool canFire = false;

				/*
				int weaponCount = pFoot->GetTechnoType()->WeaponCount;
				if (weaponCount == 0)
					weaponCount = 2;

				for (int i = 0; i < weaponCount; i++)
				{
					switch (pExt->OwnerObject()->GetFireError(pTarget, i, true))
					{
					case FireError::ILLEGAL:
					case FireError::CANT:
					case FireError::MOVING:
					case FireError::RANGE:
						break;
					default:
						canFire = pExt->OwnerObject()->IsCloseEnough(pTarget, i);
						break;
					}

					if (canFire)
						break;
				}*/
				auto const nFireError = pExt->OwnerObject()->GetFireError(pTarget, nWeapon, true);
				switch (nFireError)
				{
				case FireError::ILLEGAL:
				case FireError::CANT:
				case FireError::MOVING:
				case FireError::RANGE:
				{ pExt->AnotherData.MyJJData.Cancel();
					return;
				}

					break;
				default:
					canFire = pExt->OwnerObject()->IsCloseEnough(pTarget, nWeapon);
					break;
				}

				if (canFire)
				{
					CoordStruct sourcePos = pExt->OwnerObject()->Location;
					CoordStruct targetPos = pExt->OwnerObject()->Target->GetCoords();
					DirStruct toDir = Helpers_DP::Point2Dir(sourcePos, targetPos);
					DirStruct selfDir = pFoot->PrimaryFacing.current();
					int facing = pTypeExt->AnotherData.MyJJData.Facing;
					int toIndex = Helpers_DP::Dir2FacingIndex(toDir, facing);
					int selfIndex = Helpers_DP::Dir2FacingIndex(selfDir, facing);

					if (selfIndex != toIndex)
					{
						DirStruct targetDir = Helpers_DP::DirNormalized(toIndex, facing);
						pExt->AnotherData.MyJJData.TurnTo(targetDir, facing);
					}
					else
					{
						pExt->AnotherData.MyJJData.Cancel();
					}
				}
			}
		}
	}
}

/*
#include <JumpjetLocomotionClass.h>

DEFINE_HOOK(0x54B6E0, JumpjetLocomotionClass_DoTurn, 0x8)
{
	GET_STACK(JumpjetLocomotionClass*, pLoco, 0x4);
	GET_STACK(DirStruct, nDir, 0x8);

	auto const pFoot = pLoco->Owner ? pLoco->Owner : pLoco->LinkedTo;
	R->EAX(pLoco->Facing.turn(nDir) && pFoot->PrimaryFacing.turn(nDir));

	return 0x54B6FF;
}

DEFINE_HOOK(0x736E40, UnitClass_FiringAI_JumpjetTurning, 0x6)
{
	GET(FireError, nFireError, EAX);
	GET(UnitClass*, pThis, ESI);

	R->EBP(nFireError);

	if (pThis->Type->JumpJet)
	{
		if (nFireError == FireError::OK
			|| nFireError == FireError::REARM
			|| nFireError == FireError::FACING
			|| nFireError == FireError::ROTATING)
		{
			if (!pThis->Locomotor.get()->Is_Moving_Now())
			{
				auto pTypeExt = TechnoTypeExt::ExtMap.Find(pThis->Type);
				auto pExt = TechnoExt::ExtMap.Find(pThis);

				if (auto pTarget = pThis->Target)
				{
					CoordStruct sourcePos = pThis->Location;
					CoordStruct targetPos = pTarget->GetCoords();
					DirStruct toDir = Helpers_DP::Point2Dir(sourcePos, targetPos);
					DirStruct selfDir = pThis->PrimaryFacing.current();
					int facing = pTypeExt->AnotherData.MyJJData.Facing;
					int toIndex = Helpers_DP::Dir2FacingIndex(toDir, facing);
					int selfIndex = Helpers_DP::Dir2FacingIndex(selfDir, facing);

					if (selfIndex != toIndex)
					{
						DirStruct targetDir = Helpers_DP::DirNormalized(toIndex, facing);
						pExt->AnotherData.MyJJData.TurnTo(targetDir, facing);
					}
					else
					{
						pExt->AnotherData.MyJJData.Cancel();
					}
				}
			}
		}
	}

	return  (nFireError == FireError::OK) ? 0x736E4B : 0x736E46;
}*/