#include "Body.h"
#include <Ext/WeaponType/Body.h>
#include "..\Scenario\Body.h"

// TODO: Implement proper extended AircraftClass.

void AircraftExt::FireBurst(AircraftClass* pThis, AbstractClass* pTarget, int shotNumber = 0)
{
	int weaponIndex = pThis->SelectWeapon(pTarget);

	if (auto pWeaponStuct = pThis->GetWeapon(weaponIndex))
	{
		if (auto weaponType = pWeaponStuct->WeaponType)
		{
			auto pWeaponTypeExt = WeaponTypeExt::ExtMap.Find(weaponType);

			if (weaponType->Burst > 0)
			{
				for (int i = 0; i < weaponType->Burst; i++)
				{
					if (pWeaponTypeExt && weaponType->Burst < 2 && pWeaponTypeExt->Strafing_SimulateBurst)
						pThis->CurrentBurstIndex = shotNumber;

					pThis->AircraftClass_Fire(pThis->Target, weaponIndex);
				}
			}
		}
	}
}
