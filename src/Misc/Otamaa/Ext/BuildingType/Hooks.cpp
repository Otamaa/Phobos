#include <Ext/BuildingType/Body.h>

#include <Utilities/Macro.h>
#include <BuildingClass.h>

DEFINE_HOOK(0x6FE3F1, TechnoClass_Fireat_OccupyDamageBonus, 0xB)
{
	GET(TechnoClass* const, pThis, ESI);

	if (auto const Building = specific_cast<BuildingClass*>(pThis))
	{
		if (auto const TypeExt = BuildingTypeExt::ExtMap.Find(Building->Type))
		{
			GET_STACK(int, nDamage, 0x2C);
			R->EAX(Game::F2I(nDamage * TypeExt->AnotherTypeData.BuildingOccupyDamageMult.Get(RulesGlobal->OccupyDamageMultiplier)));
			return 0x6FE405;
		}
	}

	return 0;
}

DEFINE_HOOK(0x6FE421, TechnoClass_Fireat_BunkerDamageBonus, 0xB)
{
	GET(TechnoClass* const, pThis, ESI);

	if (auto const Building = specific_cast<BuildingClass*>(pThis->BunkerLinkedItem))
	{
		if (auto const TypeExt = BuildingTypeExt::ExtMap.Find(Building->Type))
		{
			GET_STACK(int, nDamage, 0x2C);
			R->EAX(Game::F2I(nDamage * TypeExt->AnotherTypeData.BuildingBunkerDamageMult.Get(RulesGlobal->OccupyDamageMultiplier)));
			return 0x6FE435;
		}
	}

	return 0;
}

DEFINE_HOOK(0x6FD183, TechnoClass_RearmDelay_BuildingOccupyROFMult, 0xC)
{
	GET(TechnoClass*, pThis, ESI);

	if (auto const Building = specific_cast<BuildingClass*>(pThis))
	{
		if (auto const TypeExt = BuildingTypeExt::ExtMap.Find(Building->Type))
		{
			auto const nMult = TypeExt->AnotherTypeData.BuildingOccupyROFMult.Get(RulesGlobal->OccupyROFMultiplier);
			if (nMult > 0.0f)
			{
				GET_STACK(int, nROF, STACK_OFFS(0x10, -0x4));
				R->EAX(Game::F2I(((double)nROF) / nMult));
				return 0x6FD1AB;
			}
			return 0x6FD1B1;
		}
	}

	return 0x0;
}

DEFINE_HOOK(0x6FD1C7, TechnoClass_RearmDelay_BuildingBunkerROFMult, 0xC)
{
	GET(TechnoClass*, pThis, ESI);

	if (auto const Building = specific_cast<BuildingClass*>(pThis->BunkerLinkedItem))
	{
		if (auto const TypeExt = BuildingTypeExt::ExtMap.Find(Building->Type))
		{
			auto const nMult = TypeExt->AnotherTypeData.BuildingBunkerROFMult.Get(RulesGlobal->BunkerROFMultiplier);
			if (nMult > 0.0f)
			{
				GET_STACK(int, nROF, STACK_OFFS(0x10, -0x4));
				R->EAX(Game::F2I(((double)nROF) / nMult));
				return 0x6FD1EF;
			}
			return 0x6FD1F1;
		}
	}

	return 0x0;
}

static void BunkerPlaySound(BuildingClass const* pThis, bool bUp = false)
{
	auto nSound = bUp ? RulesGlobal->BunkerWallsUpSound : RulesGlobal->BunkerWallsDownSound;

	if (auto const TypeExt = BuildingTypeExt::ExtMap.Find(pThis->Type))
	{
		auto const& nData = TypeExt->AnotherTypeData;
		nSound = bUp ? nData.BunkerWallsUpSound.Get(nSound) : nData.BunkerWallsDownSound.Get(nSound);
	}

	if (nSound != -1)
		VocClass::PlayAt(nSound, pThis->Location);
}

DEFINE_HOOK_AGAIN(0x45933D, BuildingClass_BunkerSound, 0x5)
DEFINE_HOOK_AGAIN(0x4595D9, BuildingClass_BunkerSound, 0x5)
DEFINE_HOOK(0x459494, BuildingClass_BunkerSound, 0x5)
{
	BuildingClass const* pThis = R->Origin() == 0x4595D9 ?
		R->EDI<BuildingClass*>() : R->ESI<BuildingClass*>();

	BunkerPlaySound(pThis, R->Origin() != 0x459494);

	switch (R->Origin())
	{
	case 0x45933D:
		return 0x459374;
	case 0x4595D9:
		return 0x459612;
	}

	return 0x4594CD;
}

// not working  ?
// hook itself is fine , but it not play globally as it should :s
DEFINE_HOOK(0x44A86A, BuildingClass_Mi_Selling_PackupSound, 0x6)
{
	GET(BuildingClass* const, pThis, EBP);
	CoordStruct nBuffer;
	pThis->GetCenterCoord(&nBuffer);
	auto const pExt = BuildingTypeExt::ExtMap.Find(pThis->Type);
	VocClass::PlayIndexAtPos(pThis->Type->PackupSound, nBuffer, pExt && pExt->AnotherTypeData.PackupSound_PlayGlobal.Get());
	return 0x44A89E;
}

DEFINE_HOOK_AGAIN(0x4426DB, BuildingClass_ReceiveDamage_DisableDamageSound, 0x8)
DEFINE_HOOK_AGAIN(0x702777, BuildingClass_ReceiveDamage_DisableDamageSound, 0x8)
DEFINE_HOOK(0x70272E, BuildingClass_ReceiveDamage_DisableDamageSound, 0x8)
{
	GET(TechnoClass*, pThis, ESI);

	if (auto const pBuilding = specific_cast<BuildingClass*>(pThis))
	{
		auto const pExt = BuildingTypeExt::ExtMap.Find(pBuilding->Type);
		if (pExt && pExt->AnotherTypeData.DisableDamageSound.Get())
		{
			switch (R->Origin())
			{
			case 0x70272E:
				return 0x702765;
			case 0x702777:
				return 0x7027AE;
			case 0x4426DB:
				return 0x44270B;
			}
		}
	}

	return 0x0;
}

DEFINE_HOOK(0x44E85F, BuildingClass_Power_DegradeWithHealth, 0x7)
{
	GET(BuildingClass*, pThis, ESI);
	GET_STACK(int, nPowMult, STACK_OFFS(0xC, 0x4));

	auto const pTypeExt = BuildingTypeExt::ExtMap.Find(pThis->Type);

	R->EAX(pTypeExt->AnotherTypeData.Power_DegradeWithHealth.Get() ? Game::F2I(nPowMult * pThis->GetHealthPercentage()) : (nPowMult));

	return 0x44E86F;
}