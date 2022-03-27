#include <Ext/Anim/Body.h>

#include <TechnoClass.h>
#include <ScenarioClass.h>
#include <HouseClass.h>
#include <WarheadTypeClass.h>
#include <ParticleSystemClass.h>
#include <ParticleTypeClass.h>
#include <MapClass.h>
#include <SuperClass.h>
#include <CellClass.h>

#include <Ext/AnimType/Body.h>

#include <Ext/Tiberium/Body.h>

#include <Ext/BulletType/Body.h>
#include <Ext/Bullet/Body.h>
#include <Misc/Otamaa/Helpers.h>

#include <Utilities/Macro.h>

DEFINE_HOOK(0x685078, Generate_OreTwinkle_Anims, 0x7)
{
	GET(CellClass* const, location, ESI);

	if (location->GetContainedTiberiumValue() > 0)
	{
		auto const tib = TiberiumClass::Array->GetItem(location->GetContainedTiberiumIndex());

		if (!ScenarioClass::Instance->Random(0, TiberiumExt::ExtMap.Find(tib)->GetTwinkleChance() - 1))
			if (auto pAnimtype = TiberiumExt::ExtMap.Find(tib)->GetTwinkleAnim())
				if (auto pAnim = GameCreate<AnimClass>(pAnimtype, location->GetCoords()))
					AnimExt::SetAnimOwnerHouseKind(pAnim, nullptr, nullptr, false);
	}

	return 0x6850E5;
}

DEFINE_HOOK(0x423CD5, AnimClass_Expired_Extra_OnWater, 0x6)
{
	GET(AnimClass*, pThis, ESI);

	if (!pThis->Type)
		return 0x423EFD;

	auto const nLocation = pThis->GetCenterCoord();
	auto const pOwner = pThis->Owner;
	auto const pAnimTypeExt = AnimTypeExt::ExtMap.Find(pThis->Type);
	auto const& nData = pAnimTypeExt->AnotherData;

	DWORD flags = 0x600u;
	int ForceZAdjust = 0;

	//Default
	AnimTypeClass* pSplashAnim = nullptr;

	if (pAnimTypeExt)
	{
		if (nData.ExplodeOnWater.Get())
		{
			if (auto const pWarhead = pThis->Type->Warhead)
			{
				auto const nDamage = Game::F2I(pThis->Damage);
				TechnoClass* pTechOwner = abstract_cast<TechnoClass*>(pThis->OwnerObject);

				MapClass::DamageArea(nLocation, nDamage, pTechOwner, pWarhead, pWarhead->Tiberium, pOwner);
				MapClass::FlashbangWarheadAt(nDamage, pWarhead, nLocation);
				auto nLand = pThis->GetCell() ? pThis->GetCell()->LandType : LandType::Clear;
				pSplashAnim = MapClass::SelectDamageAnimation(nDamage, pWarhead, nLand, nLocation);
				flags = 0x2600u;
				ForceZAdjust = -30;
			}

			if (auto pExpireAnim = pThis->Type->ExpireAnim)
				if (auto pAnim = GameCreate<AnimClass>(pExpireAnim, nLocation, 0, 1, 0x2600u, -30, 0))
					AnimExt::SetAnimOwnerHouseKind(pAnim, pOwner, nullptr, false);
		}
		else
		{
			if (pThis->Type->IsMeteor)
			{
				auto const splash = nData.SplashList.GetElements(RulesClass::Instance->SplashList);

				if (splash.size() > 0)
				{
					auto nIndexR = (splash.size() - 1);
					auto nIndex = nData.SplashIndexRandom.Get() ?
						ScenarioClass::Instance->Random(0, nIndexR) : nData.SplashIndex.Get(nIndexR);

					pSplashAnim = splash.at(nIndex);
				}
			}
			else
			{
				pSplashAnim = nData.WakeAnim.Get(RulesClass::Instance->Wake);
			}
		}
	}

	if (pSplashAnim)
		if (auto const pSplashAnimCreated = GameCreate<AnimClass>(pSplashAnim, nLocation, 0, 1, flags, ForceZAdjust))
			AnimExt::SetAnimOwnerHouseKind(pSplashAnimCreated, pOwner, nullptr, false);

	return 0x423EFD;
}

DEFINE_HOOK(0x423DE7, AnimClass_Expired_Extra_OnLand_DamageArea, 0x6)
{
	GET(AnimClass*, pThis, ESI);

	if (auto const pType = pThis->Type)
	{
		auto const pOwner = pThis->Owner;
		auto nCoords = pThis->Bounce.GetCoords();

		if (auto pExpireAnim = pType->ExpireAnim)
			if (auto pAnim = GameCreate<AnimClass>(pExpireAnim, nCoords, 0, 1, 0x2600u, -30, 0))
				AnimExt::SetAnimOwnerHouseKind(pAnim, pOwner, nullptr, false);

		if (auto const pWarhead = pType->Warhead)
		{
			auto const nDamage = Game::F2I(pType->Damage);
			MapClass::DamageArea(nCoords, nDamage, abstract_cast<TechnoClass*>(pThis->OwnerObject), pWarhead, pWarhead->Tiberium, pOwner);
			MapClass::FlashbangWarheadAt(nDamage, pWarhead, nCoords, false);
		}
	}

	return 0x423EFD;
}

DEFINE_HOOK(0x424FE8, AnimClass_Middle_SpawnParticle, 0xC)
{
	GET(AnimClass* const, pThis, ESI);

	if (auto const pType = pThis->Type)
	{
		auto const pAnimTypeExt = AnimTypeExt::ExtMap.Find(pType);
		//auto const pObject = pThis->OwnerObject;
		auto const pHouse = Otamaa::Helpers::GetHouseOwner(pThis);

		if (!pAnimTypeExt)
			return 0;

		Otamaa::Helpers::SpawnMultiple(
			pAnimTypeExt->AnotherData.SpawnsMultiple,
			pAnimTypeExt->AnotherData.SpawnsMultiple_amouts,
			pThis->GetCenterCoord(), pHouse, pAnimTypeExt->AnotherData.SpawnsMultiple_Random.Get());

		auto const& nData = pAnimTypeExt->AnotherData;

		if (pType->SpawnsParticle != -1)
		{
			auto const pParticleType = ParticleTypeClass::Array.get()->GetItem(pType->SpawnsParticle);
			auto const nCoord = pThis->GetCenterCoord();

			if (pType->NumParticles > 0 && pParticleType)
			{
				for (int i = 0; i < pType->NumParticles; ++i)
				{
					CoordStruct nDestCoord;
					if (nData.ParticleChance.isset() ?
						(ScenarioClass::Instance->Random(0, 99) < abs(nData.ParticleChance.Get())) : true)
					{
						nDestCoord = Otamaa::Helpers::GetRandomCoordsInsideLoops(nData.ParticleRangeMin.Get(), nData.ParticleRangeMax.Get(), nCoord, i);
						ParticleSystemClass::Instance->SpawnParticle(pParticleType, &nDestCoord);
					}
				}
			}
		}

		Otamaa::Helpers::LauchSW(
			nData.LaunchSW.Get() && nData.LaunchWhat.isset(),
			nData.LaunchWhat.Get(), pHouse,
			pThis->GetCenterCoord(), nData.LaunchWaitcharge.Get(),
			nData.LaunchResetCharge.Get());
	}

	return 0x42504D;
}

DEFINE_HOOK(0x423991, AnimClass_BounceAI_BounceAnim, 0xA)
{
	//GET(AnimTypeClass*, pType, EAX);
	GET(AnimTypeClass*, pBounceAnim, ECX);
	GET(AnimClass*, pThis, EBP);

	auto nCoord = pThis->GetCenterCoord();
	if (auto pAnim = GameCreate<AnimClass>(pBounceAnim, nCoord, 0, 1, 0x600, 0, 0))
		AnimExt::SetAnimOwnerHouseKind(pAnim, pThis->Owner, nullptr, false);

	return 0x4239D3;
}
