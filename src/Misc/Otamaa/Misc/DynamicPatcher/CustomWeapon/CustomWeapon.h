#pragma once

#include <queue>
#include "SimulateBurst.h"
#include "DelayFireWeapon.h"

class WeaponTypeClass;
class TechnoClass;
struct CustomWeaponManager
{
	std::queue<SimulateBurst> simulateBurstQueue;

	void SimulateBurstQueueClear(std::queue<SimulateBurst>& q)
	{
		std::queue<SimulateBurst> empty;
		std::swap(q, empty);
	}

	void Update(TechnoClass* pAttacker);
	bool FireCustomWeapon(TechnoClass* pShooter, TechnoClass* pAttacker, AbstractClass* pTarget, WeaponTypeClass* pWeapon, CoordStruct flh, CoordStruct bulletSourcePos, double rofMult);
	void SimulateBurstFire(TechnoClass* pShooter, TechnoClass* pAttacker, AbstractClass* pTarget, WeaponTypeClass* pWeapon, SimulateBurst& burst);
	void SimulateBurstFireOnce(TechnoClass* pShooter, TechnoClass* pAttacker, AbstractClass* pTarget, WeaponTypeClass* pWeapon, SimulateBurst& burst);
	TechnoClass* WhoIsShooter(TechnoClass* pAttacker);

	void UnInitAll()
	{ SimulateBurstQueueClear(simulateBurstQueue); }

	bool Load(PhobosStreamReader& Stm, bool RegisterForChange)
	{ Debug::Log("Loading Element From CustomWeaponManager ! \n");  return Serialize(Stm); }

	bool Save(PhobosStreamWriter& Stm)
	{ Debug::Log("Saving CustomWeaponManager ! \n");  return Serialize(Stm); }

private:
	template <typename T>
	bool Serialize(T& Stm)
	{
		return Stm
			.Process(simulateBurstQueue)
			.Success()
			;
	}
};

struct FireWeaponManager
{
	std::queue<DelayFireWeapon> DelayFires;
	CustomWeaponManager CWeaponManager;

	void FireWeaponManagerClear(std::queue<DelayFireWeapon>& q)
	{
		std::queue<DelayFireWeapon> empty;
		std::swap(q, empty);
	}

	void EnqueueDelayFireWeapon(int weaponIndex, AbstractClass* pTarget, int delay = 0, int count = 1)
	{
		DelayFireWeapon delayFire = DelayFireWeapon(weaponIndex, pTarget, delay, count);
		DelayFires.emplace(delayFire);
	}

	void EnqueueDelayFireWeapon(WeaponTypeClass* pWeapon, AbstractClass* pTarget, int delay = 0, int count = 1)
	{
		DelayFireWeapon delayFire = DelayFireWeapon(pWeapon, pTarget, delay, count);
		DelayFires.emplace(delayFire);
	}

	bool FireCustomWeapon(TechnoClass* pShooter, TechnoClass* pAttacker, AbstractClass* pTarget, WeaponTypeClass* pWeapon, CoordStruct flh, CoordStruct bulletSourcePos, double rofMult = 1)
	{
		return CWeaponManager.FireCustomWeapon(pShooter, pAttacker, pTarget, pWeapon, flh, bulletSourcePos, rofMult);
	}

	void TechnoClass_Update_CustomWeapon(TechnoClass* pAttacker);

	void TechnoClass_UnInit_CustomWeapon()
	{
		FireWeaponManagerClear(DelayFires);
		CWeaponManager.UnInitAll();
	}

	bool Load(PhobosStreamReader& Stm, bool RegisterForChange)
	{ Debug::Log("Loading Element From FireWeaponManager ! \n");  return Serialize(Stm); }

	bool Save(PhobosStreamWriter& Stm)
	{ Debug::Log("Saving FireWeaponManager ! \n"); return Serialize(Stm); }

private:
	template <typename T>
	bool Serialize(T& Stm)
	{
		return Stm
			.Process(DelayFires)
			.Process(CWeaponManager)
			.Success()
			;
	}
};
