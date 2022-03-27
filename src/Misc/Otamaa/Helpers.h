#pragma once

#include <Utilities/TemplateDef.h>

class SuperWeaponTypeClass;
namespace Otamaa
{
	struct Helpers
	{
		static void LauchSW(bool Lauch, SuperWeaponTypeClass* LaunchWhat, HouseClass* pOwner, CoordStruct Where, bool WaitForCharge, bool ResetChargeAfterLauch);
		static HouseClass* GetHouseOwner(AnimClass* pWho, HouseClass* pFallback = nullptr);
		static HouseClass* GetHouseOwner(HouseClass* pIn, HouseClass* pFallback = nullptr);
		static void SpawnMultiple(std::vector<AnimTypeClass*>& nAnims, DynamicVectorClass<int>& nAmount, CoordStruct Where, HouseClass* pOwner, bool bRandom);
		static bool CheckMinMax(double nMin, double nMax, int& nOutMin, int& nOutMax);
		static CoordStruct GetRandomCoordsInsideLoops(double nMin, double nMax, CoordStruct nPos, int Increment);

	};
}