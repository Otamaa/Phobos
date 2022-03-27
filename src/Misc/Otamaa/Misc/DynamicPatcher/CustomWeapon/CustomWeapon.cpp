#include "CustomWeapon.h"

#include <Ext/Techno/Body.h>
#include <Ext/WeaponType/Body.h>
#include <Ext/BulletType/Body.h>

void CustomWeaponManager::Update(TechnoClass* pAttacker)
{
	if (!pAttacker || !pAttacker->Target || !TechnoExt::IsReallyAlive(pAttacker))
	{
		SimulateBurstQueueClear(simulateBurstQueue);
	}
	else
	{
		for (size_t i = 0; i < simulateBurstQueue.size(); i++)
		{
			SimulateBurst burst = simulateBurstQueue.front();
			simulateBurstQueue.pop();

			if (burst.Index < burst.Burst)
			{
				// 检查延迟
				if (burst.CanFire())
				{
					TechnoClass* pShooter = burst.Shooter;
					AbstractClass* pTarget = burst.Target;
					WeaponTypeClass* pWeaponType = burst.WeaponType;

					auto const pTargetTech = generic_cast<TechnoClass*>(pTarget);

					if (pWeaponType
						&& pShooter && pShooter->IsAlive
						&& pTargetTech && !Helpers_DP::IsDeadOrInvisible(pTargetTech)
						&& (!burst.FireData.CheckRange || pShooter->GetCoords().DistanceFrom(pTarget->GetCoords()) <= burst.Range)
						&& (!pAttacker->Transporter || (pWeaponType->FireInTransport || burst.FireData.OnlyFireInTransport))
					)
					{
						SimulateBurstFire(pShooter, pAttacker, pTarget, pWeaponType, burst);
					}
					else
					{
						continue;
					}
				}

				simulateBurstQueue.emplace(burst);
			}
		}
	}
}

bool CustomWeaponManager::FireCustomWeapon(TechnoClass* pShooter, TechnoClass* pAttacker, AbstractClass* pTarget, WeaponTypeClass* pWeapon, CoordStruct flh, CoordStruct bulletSourcePos, double rofMult)
{
	bool isFire = false;
	pShooter = WhoIsShooter(pShooter);
	if (pWeapon && (!pAttacker->Transporter || pWeapon->FireInTransport))
	{
		if (auto typeExt = WeaponTypeExt::ExtMap.Find(pWeapon))
		{
			AttachFireData fireData = typeExt->WAttachFireData;
			CoordStruct fireFLH = flh;

			if (TechnoClass* pTransporter = pAttacker->Transporter)
			{
				if (fireData.UseAlternateFLH)
				{
					int index = pTransporter->Passengers.IndexOf((FootClass*)pAttacker);
					if (index < 6)
						fireFLH = pTransporter->GetTechnoType()->Weapon[index + 5].FLH;
				}
			}
			else if (fireData.OnlyFireInTransport)
			{
				return isFire;
			}

			int burst = pWeapon->Burst;
			int range = pWeapon->Range;
			if (pTarget->IsInAir())
			{
				range += pShooter->GetTechnoType()->AirRangeBonus;
			}

			if (burst > 1 && fireData.SimulateBurst)
			{

				int flipY = 1;

				if (BulletTypeClass* pBulletType = pWeapon->Projectile)
				{
					auto bulletTypeExt = BulletTypeExt::ExtMap.Find(pBulletType);
					if (bulletTypeExt && bulletTypeExt->MissileData.ReverseVelocity)
						flipY = -1;

				}

				SimulateBurst newBurst = SimulateBurst(pWeapon, pShooter, pTarget, fireFLH, burst, range, fireData, flipY);
				SimulateBurstFire(pShooter, pAttacker, pTarget, pWeapon, newBurst);
				simulateBurstQueue.emplace(newBurst);
				isFire = true;
			}
			else
			{
				if (!fireData.CheckRange || pShooter->GetCoords().DistanceFrom(pTarget->GetCoords()) <= range)
				{
					Helpers_DP::FireWeaponTo(pShooter, pAttacker, pTarget, pWeapon, fireFLH, bulletSourcePos, fireData.RadialFire, fireData.RadialAngle);
					isFire = true;
				}
			}
		}
		else
		{
			Helpers_DP::FireWeaponTo(pShooter, pAttacker, pTarget, pWeapon, flh, bulletSourcePos);
			isFire = true;
		}

	}
	return isFire;
}

void CustomWeaponManager::SimulateBurstFire(TechnoClass* pShooter, TechnoClass* pAttacker, AbstractClass* pTarget, WeaponTypeClass* pWeapon, SimulateBurst& burst)
{
	if (burst.FireData.SimulateBurstMode == 3)
	{
		SimulateBurst b2 = burst.Clone();
		b2.FlipY *= -1;
		SimulateBurstFireOnce(pShooter, pAttacker, pTarget, pWeapon, b2);
	}
	// 单发
	SimulateBurstFireOnce(pShooter, pAttacker, pTarget, pWeapon, burst);

}

void CustomWeaponManager::SimulateBurstFireOnce(TechnoClass* pShooter, TechnoClass* pAttacker, AbstractClass* pTarget, WeaponTypeClass* pWeapon, SimulateBurst& burst)
{
	// Pointer<TechnoClass> pShooter = WhoIsShooter(pShooter);
	CoordStruct sourcePos = Helpers_DP::GetFLHAbsoluteCoords(pShooter, burst.FLH, true, burst.FlipY);
	CoordStruct targetPos = pTarget->GetCoords();
	BulletVelocity bulletVelocity = BulletVelocity::Empty;

	if (burst.FireData.RadialFire)
	{
		RadialFireHelper radialFireHelper = RadialFireHelper(pShooter, burst.Burst, burst.FireData.RadialAngle);
		bulletVelocity = radialFireHelper.GetBulletVelocity(burst.Index);
	}
	else
	{
		bulletVelocity = Helpers_DP::GetBulletVelocity(sourcePos, targetPos);
	}

	Helpers_DP::FireBulletTo(pAttacker, pTarget, pWeapon, sourcePos, targetPos, bulletVelocity);

	burst.CountOne();
}

TechnoClass*  CustomWeaponManager::WhoIsShooter(TechnoClass* pAttacker)
{
	if (TechnoClass* pTransporter = pAttacker->Transporter)
		pAttacker = pTransporter;

	return pAttacker;
}

void FireWeaponManager::TechnoClass_Update_CustomWeapon(TechnoClass* pAttacker)
{
	for (size_t i = 0; i < DelayFires.size(); i++)
	{
		DelayFireWeapon delayFire = DelayFires.front();
		DelayFires.pop();

		if (delayFire.TimesUp())
		{
			if (delayFire.FireOwnWeapon)
			{
				pAttacker->FireAt(delayFire.Target, delayFire.WeaponIndex);
			}
			else
			{
				Helpers_DP::FireWeaponTo(pAttacker, pAttacker, delayFire.Target, delayFire.Weapon, CoordStruct::Empty);
			}
			delayFire.ReduceOnce();
		}
		if (delayFire.NotDone())
		{
			DelayFires.emplace(delayFire);
		}
	}
	// 发射自定义武器
	CWeaponManager.Update(pAttacker);
}