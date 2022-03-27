#include "Body.h"

#include <TemporalClass.h>
#include <Ext/BuildingType/Body.h>

//building
DEFINE_HOOK(0x6FE3F5, TechnoClass_Fireat_OccupyDamageBonus, 0xB)
{
	GET(TechnoClass* const, pThis, ESI);

	if (auto Building = specific_cast<BuildingClass*>(pThis))
	{
		if (auto TypeExt = BuildingTypeExt::ExtMap.Find(Building->Type))
		{
			Debug::Log(__FUNCTION__" Executed ! \n");

			GET_STACK(int, nDamage, 0x2C);

			auto Mult = TypeExt->BuildingOccupyDamageMult.Get(RulesClass::Instance->OccupyDamageMultiplier);

			R->EAX(Game::F2I(nDamage * Mult));

			return 0x6FE405;
		}
	}

	return 0;
}

DEFINE_HOOK(0x6FE421, TechnoClass_Fireat_BunkerDamageBonus, 0xB)
{
	GET(TechnoClass* const, pThis, ESI);

	if (auto TypeExt = TechnoTypeExt::ExtMap.Find(pThis->Type()))
	{
		Debug::Log(__FUNCTION__" Executed ! \n");
		GET_STACK(int, nDamage, 0x2C);

		auto Mult = TypeExt->BunkerDamageMultiplier.Get(RulesClass::Instance->BunkerDamageMultiplier);

		R->EAX(Game::F2I(nDamage * Mult));

		return 0x6FE435;
	}

	return 0;
}

//
DEFINE_HOOK(0x6F7294, TechnoClass_FireAt_OccupyRange, 0xB)
{
	//GET(TechnoClass* const, pThis, ESI);
	GET(int, nAddRange, EDI);
	Debug::Log(__FUNCTION__" Executed ! \n");
	auto nRange = RulesGlobal->OccupyWeaponRange;
	R->EDI(((nRange + nAddRange) << 8));
	return 0x6F72A2;
}

DEFINE_HOOK(0x6F9193, TechnoClass_GreatestThreat_OccupyRange, 0x6)
{
	//GET(TechnoClass* const, pThis, ESI);
	//GET(int, nAddRange, EAX);
	Debug::Log(__FUNCTION__" Executed ! \n");
	auto nRange = RulesGlobal->OccupyWeaponRange;
	R->EDX(&nRange); //lea stack eax 
	return 0x6F919F;
}