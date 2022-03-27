#include <Ext/WeaponType/Body.h>

DEFINE_HOOK(0x6FF660, TechnoClass_FireBullet_Shake, 0x6)
{
	GET(WeaponTypeClass* const, pWeaponType, EBX);
	GET(TechnoClass* const, pSource, ESI);
	//Shake Screen
	if (pWeaponType)
	{
		if (auto const pWeaponExt = WeaponTypeExt::ExtMap.Find(pWeaponType))
		{
			if (pWeaponExt->AnotherData.ShakeLocal.Get() && !pSource->IsOnMyView())
				return 0x0;

			if (pWeaponExt->AnotherData.Xhi || pWeaponExt->AnotherData.Xlo)
				Map.ScreenShakeX = abs(ScenarioClass::Instance->Random(pWeaponExt->AnotherData.Xlo, pWeaponExt->AnotherData.Xhi));

			if (pWeaponExt->AnotherData.Yhi || pWeaponExt->AnotherData.Ylo)
				Map.ScreenShakeY = abs(ScenarioClass::Instance->Random(pWeaponExt->AnotherData.Ylo, pWeaponExt->AnotherData.Yhi));
		}
	}

	return 0x0;
}