#include "Body.h"

#include <AirstrikeClass.h>

namespace Targeting
{
	void Override(TechnoClass* pThis, AbstractClass* pTarget)
	{
		pThis->ShouldLoseTargetNow = false;

		AbstractClass* pTargetSet = pTarget;

		if (pThis->Target == pTarget)
			return;

		if (auto pAirstrike = pThis->Airstrike)
		{
			if (pAirstrike->Owner == pThis)
			{
				pAirstrike->ResetTarget();
			}
		}

		if (auto pAir = specific_cast<AircraftClass*>(pThis))
		{
			auto pType = pAir->Type;
			if (!pTarget)
				pTargetSet = nullptr;

			if (pType->Spawned && !pType->MissileSpawn && pThis->Ammo)
			{
				if (pThis->CurrentMission == Mission::Attack)
				{
					auto nMStatus = pThis->MissionStatus;
					if (nMStatus > 4 && nMStatus < 10)
					{
						pThis->Ammo = 0;
						pTargetSet = nullptr;
					}
				}
			}
		}

		if (pTargetSet)
		{
			if (auto pTechTarget = abstract_cast<TechnoClass*>(pTargetSet))
			{
				if (auto pBunkerLinked = pTechTarget->BunkerLinkedItem)
				{
					if (pTechTarget->WhatAmI() != AbstractType::Building)
					{
						if (pThis->TemporalImUsing)
						{
							pTargetSet = pBunkerLinked;
						}

						if (auto pWeapon = pThis->GetWeapon(0)->WeaponType)
						{
							if (auto pWh = pWeapon->Warhead)
							{
								if (pWh->IsLocomotor)
								{
									pTargetSet = pBunkerLinked;
								}
							}
						}

						if (auto pAirStrike = pThis->Airstrike)
						{
							if (pAirStrike->Owner == pThis)
							{
								pTargetSet = pBunkerLinked;
							}
						}

					}
				}
			}

			//
			if (pTargetSet == pThis)
			{
				auto pCoord = pThis->GetCoords();
				auto pCell = MapClass::Instance->TryGetCellAt(pCoord);
				auto pSpawn = pThis->SpawnManager;
				pThis->Target = pTargetSet;
				if (pSpawn && !pTargetSet)
				{
					pSpawn->SetTarget(nullptr);
				}
			}
		}
	}
}