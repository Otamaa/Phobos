#include "SpawnSupportFunctional.h"
#include "SpawnSupportData.h"

#include <Ext/Techno/Body.h>
#include <Ext/TechnoType/Body.h>

#include "../../Helpers/Helpers.h"

void SpawnSupportFunctional::FireSupportWeaponToSpawn(TechnoClass* pThis ,bool InUpdateFunc , bool useROF)
{
	if (!pThis)
		return;

	auto const pSpawnOwner = pThis->SpawnOwner;

	if (!pSpawnOwner)
		return;

	auto const pExt = TechnoExt::ExtMap.Find(pSpawnOwner);
	auto const pTypeExt = TechnoTypeExt::ExtMap.Find(pSpawnOwner->GetTechnoType());

	if (!pExt || !pTypeExt)
		return;

	if (pTypeExt->AnotherData.MySpawnSupportDatas.FireOnce)
	{
		if (!pExt->AnotherData.MySpawnSuport.spawnFireFlag)
		{
			pExt->AnotherData.MySpawnSuport.spawnFireOnceDelay.Start(pTypeExt->AnotherData.MySpawnSupportDatas.Delay);
			pExt->AnotherData.MySpawnSuport.spawnFireFlag = true;
		}
		else
		{
			if (!pExt->AnotherData.MySpawnSuport.spawnFireOnceDelay.InProgress())
			{
				pSpawnOwner->SpawnManager->Target = nullptr;
				pSpawnOwner->SpawnManager->SetTarget(nullptr);
			}
		}
	}

	if (auto pSpawn = pSpawnOwner->SpawnManager)
	{
		if (!pSpawn->Target && !pSpawn->NewTarget)
		{
			pExt->AnotherData.MySpawnSuport.spawnFireFlag = false;
		}
	}

	if (pTypeExt->AnotherData.MySpawnSupportDatas.Enable && pTypeExt->AnotherData.MySpawnSupportDatas.SupportWeapon)
	{
		if (InUpdateFunc && !pTypeExt->AnotherData.MySpawnSupportDatas.Always.Get())
			return;

		if (!InUpdateFunc && pTypeExt->AnotherData.MySpawnSupportDatas.Always.Get())
			return;

		CoordStruct nFLH { 0,0,0 };

		SpawnSupportFLHData nFLHData = pTypeExt->AnotherData.MySpawnSupportFLH;

		if (auto const pTransporter = pSpawnOwner->Transporter)
		{
			if(auto const pTransportExt = TechnoTypeExt::ExtMap.Find(pTransporter->GetTechnoType()))
			{
				nFLHData = pTransportExt->AnotherData.MySpawnSupportFLH;
			}
		}

		auto const pSupportWeapon = pSpawnOwner->Veterancy.IsElite() ?
			pTypeExt->AnotherData.MySpawnSupportDatas.EliteSupportWeapon : pTypeExt->AnotherData.MySpawnSupportDatas.SupportWeapon;

		nFLH = pSpawnOwner->Veterancy.IsElite() ? nFLHData.EliteSpawnSupportFLH : nFLHData.SpawnSupportFLH;

		if (pTypeExt->AnotherData.MySpawnSupportDatas.SwitchFLH)
		{
			nFLH.Y *= pExt->AnotherData.MySpawnSuport.supportFLHMult;
			pExt->AnotherData.MySpawnSuport.supportFLHMult *= -1;
		}

		if (useROF && pExt->AnotherData.MySpawnSuport.supportFireROF.InProgress())
			return;

		auto const nSourcePos = Helpers_DP::GetFLHAbsoluteCoords(pSpawnOwner, nFLH, true);
		auto const nTargetPos = Helpers_DP::GetFLHAbsoluteCoords(pThis, nFLH, true);
		BulletVelocity nVel = Helpers_DP::GetBulletVelocity(nSourcePos, nTargetPos);

		Helpers_DP::FireBulletTo(pSpawnOwner, pThis, pSupportWeapon, nSourcePos, nTargetPos, nVel);

		if (useROF)
			pExt->AnotherData.MySpawnSuport.supportFireROF.Start(pSupportWeapon->ROF);

	}
}

void SpawnSupportFunctional::AI(TechnoClass* pThis)
{
	SpawnSupportFunctional::FireSupportWeaponToSpawn(pThis,true,true);
}

void SpawnSupportFunctional::OnFire(TechnoClass* pThis)
{
	SpawnSupportFunctional::FireSupportWeaponToSpawn(pThis);
}