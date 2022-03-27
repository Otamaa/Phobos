#include "FireSWFunctional.h"
#include <Ext/Techno/Body.h>
#include <Ext/TechnoType/Body.h>
#include <Misc/Otamaa/Helpers.h>

void FireSWFunctional::OnFire(TechnoClass* pThis, AbstractClass* pTarget, int nWeaponIDx)
{
	if (auto const pTypeExt = TechnoTypeExt::ExtMap.Find(pThis->GetTechnoType()))
	{
		if (!pTypeExt->AnotherData.SWFireData.SuperWeapons.empty() && (pTypeExt->AnotherData.SWFireData.AnyWeapon.Get()
			|| pTypeExt->AnotherData.SWFireData.WeaponIndex.Get() == nWeaponIDx))
		{
			if (auto const pHouse = pThis->Owner)
			{
				for (auto const& pSuperType : pTypeExt->AnotherData.SWFireData.SuperWeapons)
				{
					if (pSuperType)
					{
						if (SuperClass* pSuperDecided = pHouse->FindSuperWeapon(pSuperType->Type))
						{
							if (pSuperDecided->IsCharged || !pTypeExt->AnotherData.SWFireData.RealLaunch)
							{
								CoordStruct targetPos = pTarget && pTypeExt->AnotherData.SWFireData.ToTarget.Get() ? pTarget->GetCoords() : pThis->GetCoords();
								CellStruct cell = CellClass::Coord2Cell(targetPos);
								pSuperDecided->IsCharged = true;
								pSuperDecided->Launch(cell, true);
								pSuperDecided->IsCharged = false;
								pSuperDecided->Reset();
							}
						}
					}
				}
			}
		}
	}
}