#include <DiskLaserClass.h>
#include <BulletClass.h>
#include <BulletTypeClass.h>
#include <WeaponTypeClass.h>
#include <WarheadTypeClass.h>

#include <Ext/BulletType/Body.h>
#include <Ext/Anim/Body.h>
#include <Ext/AnimType/Body.h>
#include <Ext/TechnoType/Body.h>

DEFINE_HOOK(0x4A7755, DiskLaserClass_Update_ChargedUpSound, 0xB)
{
	GET(DiskLaserClass* const, pThis, ESI);

	R->ECX(TechnoTypeExt::ExtMap.Find(pThis->Owner->GetTechnoType())->AnotherData.DiskLaserChargeUp.Get(RulesClass::Instance->DiskLaserChargeUp));

	return 0x4A7760;
}

DEFINE_HOOK(0x70FDC2, TechnoClass_Drain_LocalDrainAnim, 0x7)
{
	GET(TechnoClass*, Drainer, ESI);
	GET(TechnoClass*, pVictim, EDI);

	AnimClass* pDrainAnim = nullptr;

	if (auto pAnimType = TechnoTypeExt::ExtMap.Find(Drainer->GetTechnoType())->AnotherData.DrainAnimationType.Get(RulesClass::Instance->DrainAnimationType))
	{
		auto nCoord = Drainer->GetCoords();
		if (auto pDrainAnimCreated = GameCreate<AnimClass>(pAnimType, nCoord, 0, 1, 0x600, 0, false))
		{
			AnimExt::SetAnimOwnerHouseKind(pDrainAnimCreated, Drainer->Owner, pVictim->Owner, false);
			pDrainAnim = pDrainAnimCreated;
		}
	}

	R->EAX(pDrainAnim);
	return 0x70FE07;
}

DEFINE_HOOK(0x4A7696, DiskLaserClass_Update_CreateBulletInsteadDamageArea, 0x6)
{
	GET(DiskLaserClass* const, pThis, ESI);

	if (auto pWeapon = pThis->Weapon)
	{
		GET_STACK(CoordStruct, nCoord, STACK_OFFS(0x50,0x18));

		HouseClass* pHouse = pThis->Owner ? pThis->Owner->Owner : nullptr;
		MapClass::DamageArea(nCoord, pThis->Damage, pThis->Owner, pWeapon->Warhead, pWeapon->Warhead->Tiberium, pHouse);
	}

	return 0x4A76B4;
}