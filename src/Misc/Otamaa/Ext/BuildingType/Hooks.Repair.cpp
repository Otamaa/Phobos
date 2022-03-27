#include <Ext/BuildingType/Body.h>

DEFINE_HOOK(0x450821, BuildingClass_Repair_AI_Step, 0x5)
{
	GET(BuildingClass* const, pThis, ESI);
	auto const RepairStep = BuildingTypeExt::ExtMap.Find(pThis->Type)->AnotherTypeData.RepairRate.Get(RulesGlobal->RepairRate);
	R->EAX(Game::F2I(RepairStep * 900.0));

	return 0x450837;
}

DEFINE_HOOK(0x70BEF9, TechnoClass_canHealRepair_Building, 0xB)
{
	GET(TechnoClass*, pThis, ESI);

	if (auto const pBuilding = specific_cast<BuildingClass*>(pThis))
	{
		auto const RepairStep = BuildingTypeExt::ExtMap.Find(pBuilding->Type)->AnotherTypeData.RepairRate.Get(RulesGlobal->RepairRate);
		R->EAX(Game::F2I(RepairStep * 900.0));
		return 0x70BF0F;
	}

	return 0;
}

//https://modenc.renegadeprojects.com/RepairStep
DEFINE_HOOK(0x712120, TechnoTypeClass_GetRepairStep_Building, 0x5)
{
	GET(TechnoTypeClass*, pThis, ECX);

	auto nStep = RulesGlobal->RepairStep;
	if (auto const pBuildingType = specific_cast<BuildingTypeClass*>(pThis))
		nStep = BuildingTypeExt::ExtMap.Find(pBuildingType)->AnotherTypeData.RepairStep.Get(nStep);

	R->EAX(nStep);

	return 0x71212B;
}

DEFINE_HOOK(0x7120D1, TechnoTypeClass_GetRepairCost_Building, 0x5)
{
	GET(TechnoTypeClass*, pThis, ECX);

	if (pThis)
	{
		auto nStep = RulesGlobal->RepairStep;

		if (auto const pBuildingType = specific_cast<BuildingTypeClass*>(pThis))
			nStep = BuildingTypeExt::ExtMap.Find(pBuildingType)->AnotherTypeData.RepairStep.Get(nStep);

		R->EAX(Math::MoreOrEqualTo(((int)((pThis->GetCost() / (pThis->Strength / nStep) * RulesGlobal->RepairPercent))), 1));
	}

	R->EAX(1);

	return 0x712118;
}