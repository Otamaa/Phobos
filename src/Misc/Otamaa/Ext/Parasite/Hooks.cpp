#include "Body.h"
#include <TechnoClass.h>
#include <HouseClass.h>
#include <AnimClass.h>
#include <ScenarioClass.h>
#include <SpecificStructures.h>

#include <Ext/WarheadType/Body.h>
#include <Ext/TechnoType/Body.h>
#include <Ext/Anim/Body.h>
#include <Ext/AnimType/Body.h>

#include <Utilities/GeneralUtils.h>


/* ToDo :

	Allow To Parasite Ally *not find the check ?
	Parasite eject when target health full * possible , but ares
*/

//Disable Paraasite Rocking Harcode
DEFINE_HOOK(0x62A20E, ParasiteClass_AI_DisableRocking, 0x4)
{
	GET(ParasiteClass* const, pThis, ESI);
	GET(WeaponTypeClass* const, pWeapon, EDI);
	GET_STACK(DWORD, nVel, STACK_OFFS(0x4C, 0xC));

	auto pWarheadExt = WarheadTypeExt::ExtMap.Find(pWeapon->Warhead);

	if (!pWarheadExt->AnotherData.Parasite_DisableRocking.Get())
			pThis->Victim->RockingAI(nVel);

	return 0x62A222;
}

//Parasite Exit sound
DEFINE_HOOK(0x62A71C, ParasiteClass_ExitUnit_ExitSound, 0x6)
{
	GET(TechnoClass* const, pParasiteOwner, EAX);
	GET_STACK(CoordStruct, nCoord, STACK_OFFS(0x3C, 0x18));

	if (pParasiteOwner)
	{
		auto pOwnerTypeExt = TechnoTypeExt::ExtMap.Find(pParasiteOwner->GetTechnoType());
		VoxClass::PlayAtPos(pOwnerTypeExt->AnotherData.ParasiteExit_Sound.Get(), &nCoord);
	}

	return 0;
}
/*
DEFINE_HOOK(0x6298CC, ParasiteClass_SquiddyGrab_DeharcodeAnim, 0xA)
{
	enum { Handled = 0x62991C, Continue = 0x0 };

	GET_STACK(WeaponTypeClass*, PWeap, STACK_OFFS(0x6C, 0x4C));

	auto const pWarheadExt = WarheadTypeExt::ExtMap.FindOrAllocate(PWeap->Warhead);
	auto nIndex = pWarheadExt->AnotherData.Parasite_GrappleAnimIndex.Get(AnimTypeClass::FindIndex("SQDG"));

	if (nIndex != -1)
	{
		GET(ParasiteClass*, pArasite, ESI);
		GET_STACK(CoordStruct*, pCoord, STACK_OFFS(0x6C, 0x18));

		if (auto pAnimType = AnimTypeClass::Array()->GetItem(nIndex))
		{
			if (auto pAnim = GameCreate<AnimClass>(pAnimType, *pCoord))
			{
				pAnim->Owner = pArasite->Owner->Owner;
				pArasite->GrappleAnim = pAnim;

				return Handled;
			}
		}
	}

	return Continue;
}
*/
DEFINE_HOOK(0x629B50, ParasiteClass_SquiddyGrab_DeharcodeSplash, 0x7)
{
	enum { Handled = 0x629B9C, Continue = 0x0 };

	GET_STACK(WeaponTypeClass* const, pWeapon, STACK_OFFS(0x70, 0x4C));

	if (auto const pWhExt = WarheadTypeExt::ExtMap.Find(pWeapon->Warhead))
	{
		if (auto const AnimType = pWhExt->AnotherData.SquidSplash.GetElements(RulesClass::Instance->SplashList))
		{
			if (auto const pSplashType = AnimType.at(ScenarioClass::Instance->Random(0, (AnimType.size() - 1))))
			{
				GET_STACK(CoordStruct*, pCoord, STACK_OFFS(0x70, 0xC));
				if (auto const pAnim = GameCreate<AnimClass>(pSplashType, *pCoord))
				{
					GET(ParasiteClass* const, pThis, ESI);
					auto pKiller = pThis->Owner;
					auto Invoker = (pKiller) ? pKiller->Owner : pThis->GetOwningHouse();
					AnimExt::SetAnimOwnerHouseKind(pAnim, Invoker, (pThis->Victim) ? pThis->Victim->GetOwningHouse() : nullptr, false);
					return Handled;
				}
			}
		}
	}

	return Continue;
}

/*
DEFINE_HOOK(0x4D73D4, FootClass_TakeDamage_ParasiteDontDeleteSelf, 0x8)
{
	GET(int*, damage, EDI);
	GET(FootClass* const, pThis, ESI);
	REF_STACK(args_ReceiveDamage const, args, STACK_OFFS(0x1C, -0x4));

	if (auto pParasite = pThis->ParasiteEatingMe)
	{
		bool Uninfect = true; //default
		auto const ParasiteClass = pParasite->ParasiteImUsing;

		if (auto Weapon = ParasiteExt::GetParasiteWeapon(ParasiteClass))
		{
			auto pdamage = GeneralUtils::GetDamageWarheadVersusArmor(Weapon->Damage, Weapon->Warhead, pThis->GetTechnoType()->Armor);
			auto const attacker = abstract_cast<FootClass*>(args.Attacker);

			if (pParasite->Owner == pThis->Owner
				|| pParasite->Owner->IsAlliedWith(pThis)
				&& attacker == pParasite
				&& (pdamage < 0.0))
				Uninfect = false;
		}

		if (*damage < 0 && Uninfect)
		{
			ParasiteClass->SuppressionTimer.Start(50); //deharcode this
			ParasiteClass->ExitUnit();
		}
	}

	return 0x4D7413;
}
*/