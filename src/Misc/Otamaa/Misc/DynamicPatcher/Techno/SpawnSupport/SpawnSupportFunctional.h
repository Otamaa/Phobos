#pragma once

class TechnoClass;
class AbstractClass;
struct SpawnSupportFunctional
{
	static void AI(TechnoClass* pThis);
	static void OnFire(TechnoClass* pThis);
	static void FireSupportWeaponToSpawn(TechnoClass* pThis,bool InUpdateFunc = false, bool useROF = false);
};