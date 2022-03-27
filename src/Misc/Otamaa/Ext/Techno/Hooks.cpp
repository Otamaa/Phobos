#include <Ext/Techno/Body.h>

DEFINE_HOOK(0x4DECBB, FootClass_Destroy_SpinSpeed, 0x5)
{
	GET(FootClass* const, pThis, ESI);

	auto const& pExt = TechnoTypeExt::ExtMap.Find(pThis->GetTechnoType());

	pThis->RockingSidewaysPerFrame = static_cast<float>((ScenarioClass::Instance->Random.RandomDouble() * 0.15 + 0.1) * pExt->AnotherData.CrashSpinLevelRate.Get());

	if (!ScenarioClass::Instance->Random(0,1))
		pThis->RockingSidewaysPerFrame = -pThis->RockingSidewaysPerFrame;

	pThis->RockingForwardsPerFrame = static_cast<float>(ScenarioClass::Instance->Random.RandomDouble() * 0.1 * pExt->AnotherData.CrashSpinVerticalRate.Get());

	return 0x4DED4B;
}

DEFINE_HOOK(0x4D42C4, FootClass_Mission_Patrol_IsCow, 0x8)
{
	enum { Skip = 0x4D42D2, SetMissionRate = 0x4D4569, Continue = 0x0 };

	GET(FootClass* const, pThis, ESI);

	if (TechnoTypeExt::ExtMap.Find(pThis->GetTechnoType())->AnotherData.Is_Cow.Get())
	{
		pThis->UpdateIdleAction();
		return pThis->Destination ? Skip : SetMissionRate;
	}

	return Continue;
}

DEFINE_HOOK(0x51CE9A, InfantryClass_RandomAnim_IsCow, 0x5)
{
	GET(InfantryClass*, pThis, ESI);

	R->EDI(R->EAX());

	R->BL(TechnoTypeExt::ExtMap.Find(pThis->GetTechnoType())->AnotherData.Is_Cow.Get());

	return 0x51CEAA;
}