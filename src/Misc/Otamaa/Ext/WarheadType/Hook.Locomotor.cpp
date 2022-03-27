#include "Body.h"

#include <Ext/Bullet/Body.h>
#include <Ext/Techno/Body.h>

/*
#include <Utilities/Helpers.Alex.h>
#include <Ext/Bullet/Body.h>

void BulletExt::ExtData::ManipulateLoco(TechnoClass* pThis, FootClass* pTarget, bool area)
{
	auto Target = pTarget;
	if (!Target)
		return;

	auto bullet = this->OwnerObject();
	if (!bullet)
		return;

	if (Target && Target->WhatAmI() == AbstractType::Unit)
	{
		if (auto link = *Target->RadioLinks.Items)
		{
			if (auto building = specific_cast<BuildingClass*>(link))
			{
				if (building->Type->WeaponsFactory)
				{
					if (MapClass::Instance->GetCellAt(Target->Location)->GetBuilding() == building)
					{
						return;
					}

				}
			}
		}

		auto TargetUnit = specific_cast<UnitClass*>(Target);
		if (TargetUnit->Undeploying || TargetUnit->Deploying)
		{
			return;
		}
	}

	if (!area)
	{
		if (pThis->LocomotorTarget)
		{
			pThis->AnnounceInvalidPointer_LocoManipulator(false);
			//Target->LocomotorTarget->AnnounceInvalidPointer_LocoManipulator(false);
		}

		pTarget = abstract_cast<FootClass*>(bullet->Target);
	}
	auto const eligible = pTarget->WhatAmI() == AbstractType::Unit || pTarget->WhatAmI() == AbstractType::Aircraft;

	if (!Target || pTarget->InWhichLayer() != Layer::Ground || pTarget->IsIronCurtained() || !eligible || pTarget->IsAttackedByLocomotor)
		return;

	auto const damage = GeneralUtils::GetDamageWarheadVersusArmor(bullet->GetWeaponType()->Damage, bullet->WH, pTarget->GetTechnoType()->Armor);
	//Debug::Log_Masselist(__FUNCTION__"Attacker[%s] Target[%s] damage[%fl] Area[%d]", pThis->Type()->ID, pTarget->GetTechnoType()->ID, damage, area);
	auto const eligibleSize = //bullet->Health
		Game::F2I(damage) > pTarget->Size();
	if (damage >= 0.1 && eligibleSize)
	{
		//	Debug::Log_Masselist(__FUNCTION__" %x\n", pTarget);
		pThis->ImbueLocomotor(pTarget, bullet->WH->Locomotor);
	}
}

*/

DEFINE_HOOK(0x4694CB, BulletClass_DetonateAt_Locomotor, 0x6)
{
	GET(BulletClass* const, pThis, ESI);
	GET(WarheadTypeClass* const, pWH, EAX);

	// check all
	auto const pBulletOwner = pThis->Owner;

	if (!pWH->IsLocomotor)
		return 0x469705;

	if (!pBulletOwner || !pThis->Target)
		return 0x469AA4;


	auto const pTargetFoot = abstract_cast<FootClass*>(pThis->Target);
	auto const pOwnerFoot = abstract_cast<FootClass*>(pBulletOwner);

	if (!pTargetFoot || (pTargetFoot->Type()->Locomotor == pWH->Locomotor))
		return 0x469AA4;


	if (pOwnerFoot->LocomotorTarget == pTargetFoot)
			return 0x469AA4;	

	if (pBulletOwner->LocomotorSource || !pOwnerFoot || pOwnerFoot->byte6AD)
		return 0x469AA4;
	/*
	auto const Ext = BulletExt::ExtMap.Find(pThis);
	if (pWH->CellSpread > 0.0)
	{
		auto const items = Helpers::Alex::getCellSpreadItems(target->GetCoords(), pWH->CellSpread, false);
		for (auto pTarget : items)
		{
			auto const pTargetFoot = abstract_cast<FootClass*>(pTarget);
			Ext->ManipulateLoco(BulletOwner, pTargetFoot, true);
		}
	}
	else
	{
		Ext->ManipulateLoco(BulletOwner, target, false);
	}
	*/
	R->ECX(pThis->Target);
	R->Stack(0x33, false);
	return 0x46953E;
}

// Yeah , You know , WWp not clearing these attrribute properly causing bug
// fix it 
// Author : Otamaa
namespace ClearAttributes
{
	void EndPiggy(FootClass* pThis, bool more = false)
	{
		if (pThis->byte6AD || pThis->byte6AE)
		{
			pThis->byte6AD = 0;
			pThis->byte6AE = 0;

			if (more)
			{
				pThis->IsBeingManipulated = false;
				pThis->BeingManipulatedBy = nullptr;

			}
		}

		auto pType = pThis->GetTechnoType();
		if (!pThis->Owner->ControlledByHuman() && pType->ResourceGatherer)
			pThis->QueueMission(Mission::Harvest, false);

	}

	bool IsOkToEnd(FootClass* pThis)
	{
		return !pThis->byte6AD || !pThis->byte6AE;
	}
}


DEFINE_HOOK(0x4696CE, BulletClassDetonate_ImblueLoco_SetAffectingWh, 0x6)
{
	GET(BulletClass*, pBullet, ESI);

	Debug::Log_Masselist(__FUNCTION__" For[%s] Affecting LocoWH[%s][%p]", pBullet->Type->ID, pBullet->WH->ID, pBullet->WH);

	if (auto const pTargetExt = TechnoExt::ExtMap.Find(abstract_cast<TechnoClass*>(pBullet->Target)))
		pTargetExt->WarheadAffectingMe = pBullet->WH;

	R->EAX(pBullet->WH);

	return 0x4696D4;
}

//These two were not called what so ever :kekw:
DEFINE_HOOK(0x719EFF, TeleportLocoEndPiggy_LocoWH_Bugfix, 0x9)
{
	GET(FootClass* const, pLocoOwner, ECX);

	Debug::Log_Masselist(__FUNCTION__" For[%s]", pLocoOwner->Type()->ID);

	ClearAttributes::EndPiggy(pLocoOwner);

	return 0;
}

/*
DEFINE_HOOK(0x4AF940, DriveLocoEndPiggy_LocoWH_Bugfix, 0x7)
{
	GET_STACK(ILocomotion * const, Loco, 0x4);
	auto const pLocomotor = static_cast<LocomotionClass*>(Loco);

	if (auto const Linked = pLocomotor->LinkedTo)
	{
		Debug::Log_Masselist(__FUNCTION__" For[%s]", Linked->Type()->ID);
		ClearAttributes::EndPiggy(Linked, true);
	}

	return 0;
}

 Re-Load the value here
DEFINE_HOOK(0x4D952C, FootClass_AssignTarget_ClearJumjetProperties, 0xC)
{
	GET(FootClass* const, pThis, EBP);

	auto pType = pThis->Type();

}*/