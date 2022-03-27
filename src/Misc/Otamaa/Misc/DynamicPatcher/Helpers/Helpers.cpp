#include "Helpers.h"

#include <Ext/Techno/Body.h>

CoordStruct Helpers_DP::GetFLHAbsoluteCoords(TechnoClass* pTechno, CoordStruct flh, bool isOnTurret, int flipY, bool nextFrame)
{
	CoordStruct turretOffset = CoordStruct::Empty;
	auto const pType = pTechno->GetTechnoType();

	if (isOnTurret)
	{
		if (auto const pTypeExt = TechnoTypeExt::ExtMap.Find(pType))
		{
			turretOffset = pTypeExt->TurretOffset;
		}
		else
		{
			turretOffset.X = pType->TurretOffset;
		}
	}
	return GetFLHAbsoluteCoords(pTechno, flh, isOnTurret, flipY, turretOffset, nextFrame);
}

void Helpers_DP::FireWeaponTo(TechnoClass* pShooter, TechnoClass* pAttacker, AbstractClass* pTarget, WeaponTypeClass* pWeapon, CoordStruct flh, CoordStruct bulletSourcePos, bool radialFire, int splitAngle)
{
	if (!pTarget)
		return;

	CoordStruct targetPos = pTarget->GetCoords();

	// radial fire
	int burst = pWeapon->Burst;
	RadialFireHelper radialFireHelper = RadialFireHelper(pShooter, burst, splitAngle);
	int flipY = -1;
	for (int i = 0; i < burst; i++)
	{
		BulletVelocity bulletVelocity = BulletVelocity::Empty;

		if (radialFire)
		{
			flipY = (i < burst / 2.0f) ? -1 : 1;
			bulletVelocity = radialFireHelper.GetBulletVelocity(i);
		}
		else
		{
			flipY *= -1;
		}

		CoordStruct sourcePos = bulletSourcePos;

		if (!bulletSourcePos)
		{
			// get flh
			sourcePos = GetFLHAbsoluteCoords(pShooter, flh, true, flipY);
		}
		if (!bulletVelocity)
		{
			bulletVelocity = GetBulletVelocity(sourcePos, targetPos);
		}

		FireBulletTo(pAttacker, pTarget, pWeapon, sourcePos, targetPos, bulletVelocity);

	}
}

BulletClass* Helpers_DP::FireBulletTo(TechnoClass* pAttacker, AbstractClass* pTarget, WeaponTypeClass* pWeapon, CoordStruct sourcePos, CoordStruct targetPos, BulletVelocity bulletVelocity)
{
	if (!pTarget)
		return nullptr;

	// Fire weapon
	auto const pBullet = FireBullet(pAttacker, pTarget, pWeapon, sourcePos, targetPos, bulletVelocity);
	// Draw bullet effect
	DrawBulletEffect(pWeapon, sourcePos, targetPos, pAttacker, pTarget);
	// Draw particle system
	AttachedParticleSystem(pWeapon, sourcePos, pTarget, pAttacker, targetPos);
	// Play report sound
	PlayReportSound(pWeapon, sourcePos);
	// Draw weapon anim
	DrawWeaponAnim(pWeapon, sourcePos, targetPos);
	return pBullet;
}

BulletClass* Helpers_DP::FireBullet(TechnoClass* pAttacker, AbstractClass* pTarget, WeaponTypeClass* pWeapon, CoordStruct sourcePos, CoordStruct targetPos, BulletVelocity bulletVelocity)
{
	double fireMult = 1;

	if (pAttacker && pAttacker->IsAlive)
	{
		// check spawner
		auto pSpawn = pAttacker->SpawnManager;
		if (pWeapon->Spawner && pSpawn)
		{
			pSpawn->SetTarget(pTarget);
			return nullptr;
		}

		// check Abilities FIREPOWER
		fireMult = GetDamageMult(pAttacker);
	}

	int damage = (int)(pWeapon->Damage * fireMult);
	auto pWH = pWeapon->Warhead;
	int speed = pWeapon->Speed;
	bool bright = pWeapon->Bright || pWH->Bright;

	BulletClass* pBullet = nullptr;
	pBullet = pWeapon->Projectile->CreateBullet(pTarget, pAttacker, damage, pWH, speed, bright);
	pBullet->WeaponType = pWeapon;

	//double nAdjust = pBullet->Type->Vertical ? 10.0 : 100.0;
	//bulletVelocity.Z -= nAdjust;

	pBullet->MoveTo(sourcePos, bulletVelocity);

	if (pWeapon->Projectile->Inviso && !pWeapon->Projectile->Airburst)
	{
		pBullet->Detonate(targetPos);
		pBullet->UnInit();

		return nullptr;
	}

	return pBullet;
}
