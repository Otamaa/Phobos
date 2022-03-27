#include <Ext/TechnoType/Body.h>
#include <Utilities/Macro.h>

#include <TerrainTypeClass.h>

DEFINE_HOOK(0x7466D8, UnitClass_DesguiseAs_AsAnotherUnit, 0xA)
{
	GET(UnitClass*, pTarget, ESI);
	GET(UnitClass*, pThis, EDI);

	auto const pTypeExt = TechnoTypeExt::ExtMap.Find(pThis->Type);
	auto const& nData = pTypeExt->AnotherData;

	if (pTarget->IsDisguised() || !nData.TankDisguiseAsTank.Get())
		return 0x0;

	pThis->Disguise = pTarget->Type;
	pThis->DisguisedAsHouse = pTarget->GetOwningHouse();

	return 0x746712;
}

static bool Allowed(const Otamaa::TTyExt::ExtDataB& pThis, TechnoTypeClass* pThat)
{
	if (pThis.DisguiseDisAllowed.empty())
		return true;

	if (!pThis.DisguiseDisAllowed.Contains(pThat))
		return true;

	return false;
}

DEFINE_HOOK(0x746670, UnitClass_DisguiseAs_Override, 0x5)
{
	GET(UnitClass*, pThis, ECX);
	GET_STACK(ObjectClass*, pTarget, 0x4);

	if (!pTarget || pTarget->WhatAmI() == AbstractType::Infantry)
		return 0x746714;

	auto const pExt = TechnoTypeExt::ExtMap.Find(pThis->Type);
	auto const& nData = pExt->AnotherData;

	if (!pTarget->IsDisguised())
	{
		if (!nData.TankDisguiseAsTank.Get())
		{
			if (pThis->CanDisguiseAs(pTarget))
			{
				if (const auto pOverlay = specific_cast<OverlayClass*>(pTarget))
				{
					pThis->Disguise = pOverlay->Type;
					pThis->DisguisedAsHouse = nullptr;
					pThis->Techno_70E280(pTarget);
					return 0x746714;
				}

				if (const auto pTerrain = specific_cast<TerrainClass*>(pTarget))
				{
					pThis->Disguise = pTerrain->Type;
					pThis->DisguisedAsHouse = nullptr;
					pThis->Techno_70E280(pTarget);
					return 0x746714;
				}
			}
		}
		else
		{
			if (const auto pUnit = specific_cast<UnitClass*>(pTarget))
			{
				if (Allowed(nData, pUnit->Type))
				{
					pThis->Disguise = pUnit->Type;
					pThis->DisguisedAsHouse = pUnit->GetOwningHouse();
					pThis->Techno_70E280(pTarget);
					return 0x746714;
				}
			}
		}
	}
	else
	{
		if (const auto pUnit = specific_cast<UnitClass*>(pTarget))
		{
			if (Allowed(nData, pUnit->Type))
			{
				if (pUnit->Owner)
				{
					if (pUnit->Owner->IsAlliedWith(pThis->Owner))
					{
						pThis->Disguise = pUnit->Type;
						pThis->DisguisedAsHouse = pUnit->GetOwningHouse();
						pThis->Techno_70E280(pTarget);
						return 0x746714;
					}
				}

				pThis->Disguise = pUnit->Disguise;
				pThis->DisguisedAsHouse = pUnit->DisguisedAsHouse;
				pThis->Techno_70E280(pTarget);
				return 0x746714;
			}
		}
		else
		{
			pThis->Disguise = pTarget->GetDisguise(true);
			pThis->DisguisedAsHouse = pTarget->GetDisguiseHouse(true);
			pThis->Techno_70E280(pTarget);
		}

	}

	return 0x746714;
}

static void __fastcall UnitClass_DisguiseAI_(UnitClass* pThis, void* _)
{
	auto const pExt = TechnoTypeExt::ExtMap.Find(pThis->Type);
	auto const& nData = pExt->AnotherData;

	if (!nData.TankDisguiseAsTank.Get())
		pThis->UpdateDisguise();
}

DEFINE_POINTER_CALL(0x73649C, &UnitClass_DisguiseAI_);


DEFINE_HOOK(0x746A30, UnitClass_Disguise_AI_UnitAsUnit, 0xB)
{
	GET(UnitClass*, pThis, ESI);

	auto const pTypeExt = TechnoTypeExt::ExtMap.Find(pThis->Type);
	auto const& nData = pTypeExt->AnotherData;

	if (!nData.TankDisguiseAsTank.Get())
		return 0x0;

	R->EAX(pThis->Type);
	return 0x746A6C;
}

//522700
DEFINE_HOOK(0x522718, InfantryClass_DisguiseAs_Allowed, 0x5)
{
	GET(InfantryClass*, pThis, EDI);
	GET(InfantryClass*, pThat, ESI);

	auto const pExt = TechnoTypeExt::ExtMap.Find(pThis->Type);
	auto const& nData = pExt->AnotherData;

	return Allowed(nData, pThat->Type) ? 0x0 : 0x522772;
}
