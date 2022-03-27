#include "Body.h"
#include <ParticleClass.h>
#include <ParticleSystemClass.h>
#include <Ext/Anim/Body.h>
#include <Ext/AnimType/Body.h>

DEFINE_HOOK(0x62C361, ParticleClass_SmokeAI_DisOnWater, 0x6)
{
	GET(ParticleClass*, pThis, EBP);
	GET(ParticleTypeClass*, pType, EDI);

	if (pType->WindEffect == -1)
	{
		if (auto pCell = pThis->GetCell())
		{
			auto nLand = pCell->LandType;
			if (nLand != LandType::Water || nLand != LandType::Beach)
				return 0;
		}
	}
	else
	{
		if (auto pCell = pThis->GetCell())
		{
			auto nLand = pCell->LandType;
			if (nLand != LandType::Water || nLand != LandType::Beach)
				return 0;
			else
			{
				if (auto pExt = ParticleTypeExt::ExtMap.Find(pType))
				{
					if (!pExt->DeleteWhenReachWater.Get())
					{
						return 0;
					}
				}
			}
		}
	}

	pThis->hasremaining = 1;
	pThis->UnInit();

	return 0x62C394;
}

DEFINE_HOOK(0x62BD69, ParticleClass_SmokeAI_NoWind, 0x6)
{
	return (R->EBP<ParticleClass*>()->Type->WindEffect == -1) ? 0x62C200 : 0x0;
}

DEFINE_HOOK(0x62CDD9, ParticleClass_FireAI, 0x5)
{
	GET(ParticleClass*, pThis, ESI);
	GET(ParticleTypeClass*, pType, EAX);
	GET(ObjectClass*, pObject, EDI);

	TechnoClass* pOwner = nullptr;
	HouseClass* pHouse = nullptr;
	bool damaging = false;

	if (auto pSystem = pThis->ParticleSystem)
	{
		pOwner = pSystem->Owner;
		pHouse = pSystem->OwnerHouse;

		if (pOwner && (pOwner->AbstractFlags == AbstractFlags::Techno))
		{
			if (!pSystem->OwnerHouse)
				pHouse = pOwner->Owner;
		}
	}

	if (auto pWarhead = pType->Warhead)
	{
		GET(int, nDistance, ECX);
		GET_STACK(int, nDamage, 0x10);

		pObject->ReceiveDamage(&nDamage, nDistance / 10, pWarhead, pOwner, false, false, pHouse);

		damaging = true;
	}

	if (damaging)
	{
		auto const pExt = ParticleTypeExt::ExtMap.Find(pType);

		if (auto pDamagingAnim = pExt->DamagingAnim.Get())
		{
			auto nCoord = pObject->GetCenterCoords();
			if (auto pAnim = GameCreate<AnimClass>(pDamagingAnim, nCoord))
				AnimExt::SetAnimOwnerHouseKind(pAnim, pHouse, pObject->GetOwningHouse(), false);
		}

		if (pExt->ExpireAfterDamaging.Get())
		{
			//set remaining iteration to 1
			return 0x62CE26;
		}

	}

	return 0x62CE09;
}