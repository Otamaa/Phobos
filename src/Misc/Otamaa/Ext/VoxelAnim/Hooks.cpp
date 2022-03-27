#include <HouseClass.h>
#include <AnimClass.h>
#include <AnimTypeClass.h>
#include <ScenarioClass.h>
#include <WarheadTypeClass.h>
#include <Utilities/GeneralUtils.h>
#include <Ext/Anim/Body.h>
#include <Ext/AnimType/Body.h>
#include <Ext/VoxelAnimType/Body.h>
#include <Ext/WeaponType/Body.h>

DEFINE_HOOK(0x74A035, VoxelAnimClass_DamageArea_Water, 0x6)
{
	GET(VoxelAnimClass* const, pThis, EBX);

	auto const pType = pThis->Type;
	auto const TypeExt = VoxelAnimTypeExt::ExtMap.Find(pType);
	auto const nLocation = pThis->GetCoords();
	auto const pOwner = pThis->OwnerHouse;
	AnimTypeClass* pAnimType = nullptr;
	DWORD flags = 0x600;
	int ForceZAdjust = 0;

	if (TypeExt->AnotherData.ExplodeOnWater.Get())
	{
		pAnimType = pType->ExpireAnim;

		auto const nDamage = pType->Damage;

		if (auto pWeapon = TypeExt->AnotherData.Weapon.Get())
		{
			WeaponTypeExt::DetonateAt(pWeapon, pThis->Bounce.GetCoords(), nullptr, nDamage);
		}
		else
			if (auto const pWarhead = pType->Warhead)
			{
				auto nLand = pThis->GetCell() ? pThis->GetCell()->LandType : LandType::Clear;
				pAnimType = MapClass::SelectDamageAnimation(nDamage, pWarhead, nLand, nLocation);
				MapClass::DamageArea(nLocation, nDamage, nullptr, pWarhead, pWarhead->Tiberium, pOwner);
				MapClass::FlashbangWarheadAt(nDamage, pWarhead, nLocation);
				flags = 0x2600;
				ForceZAdjust = -30;

			}
	}
	else
	{
		if (pType->IsMeteor)
		{
			auto const splash = TypeExt->AnotherData.SplashList.GetElements(RulesClass::Instance->SplashList);
			auto const nSplashIdx = ScenarioClass::Instance->Random(0, (splash.size() - 1));
			pAnimType = splash.at(nSplashIdx);
		}
		else
		{
			pAnimType = TypeExt->AnotherData.WakeAnim.Get(RulesClass::Instance->Wake);
		}
	}

	if (pAnimType)
		if (auto const pAnimCreated = GameCreate<AnimClass>(pAnimType, nLocation, 0, 1, flags, ForceZAdjust))
			AnimExt::SetAnimOwnerHouseKind(pAnimCreated, pOwner, nullptr, false);

	return 0x74A22A;
}

DEFINE_HOOK(0x74A13E, VoxelAnimClass_ExpiredOnLand_DamageArea, 0x6)
{
	GET(VoxelAnimClass*, pThis, EBX);

	if (auto const pType = pThis->Type)
	{
		auto const TypeExt = VoxelAnimTypeExt::ExtMap.Find(pType);
		auto const pOwner = pThis->OwnerHouse;
		auto nCoords = pThis->Bounce.GetCoords();

		if (auto pExpireAnim = pType->ExpireAnim)
		{
			if (auto pAnim = GameCreate<AnimClass>(pExpireAnim, nCoords, 0, 1, 0x2600u, -30, 0))
			{
				AnimExt::SetAnimOwnerHouseKind(pAnim, pOwner, nullptr, false);
			}
		}

		auto const nDamage = pType->Damage;

		if (auto pWeapon = TypeExt->AnotherData.Weapon.Get())
		{
			WeaponTypeExt::DetonateAt(pWeapon, pThis->Bounce.GetCoords(), nullptr, nDamage);
		}
		else
			if (auto const pWarhead = pType->Warhead)
			{
				MapClass::DamageArea(nCoords, nDamage, nullptr, pWarhead, pWarhead->Tiberium, pOwner);
				MapClass::FlashbangWarheadAt(nDamage, pWarhead, nCoords, false);
			}

	}

	return 0x74A22A;
}

DEFINE_HOOK(0x74A83C, VoxelAnimClass_BounceAnim, 0xA)
{
	GET(VoxelAnimClass*, pThis, EBX);

	auto nCoords = pThis->GetCoords();
	auto const pOwner = pThis->OwnerHouse;

	if (auto pAnim = GameCreate<AnimClass>(pThis->Type->BounceAnim, nCoords, 0, 1, 0x600, 0, 0))
		AnimExt::SetAnimOwnerHouseKind(pAnim, pOwner, nullptr, false);
	/*
	if (auto const pType = pThis->Type)
	{
		auto const TypeExt = VoxelAnimTypeExt::ExtMap.Find(pType);
		auto const nDamage = pType->Damage;

		if (auto pWeapon = TypeExt->Weapon.Get())
		{
			if (auto pBulletExt = BulletTypeExt::ExtMap.Find(pWeapon->Projectile))
			{
				//GET(ObjectClass*, pObject, EDI); //Unused
				//only damage in range
				//not need spesific object , will be handle by the weapon itself
				//GET(int, nGetDistance, EAX);

				if (auto pBullet = pBulletExt->CreateBullet(pThis->GetCell(), nullptr, pWeapon, nDamage, true))
				{
					pBullet->SetLocation(nCoords);
					pBullet->Explode();
					pBullet->Limbo(); //Limbo
					pBullet->UnInit();
				}
			}
		}
	}*/

	return 0x74A884;
}