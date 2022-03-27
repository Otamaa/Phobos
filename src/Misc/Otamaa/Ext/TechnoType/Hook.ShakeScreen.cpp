#include <BuildingClass.h>
#include <ScenarioClass.h>
#include <UnitClass.h>

#include <Utilities/GeneralUtils.h>
#include <Ext/TechnoType/Body.h>

//Local shakescreen that handled with TechnoShaketags
//OnlyShakeWhenOnView

//handle everything ourself
DEFINE_HOOK(0x441C0C, BuildingClass_Destroyed_Shake, 0x5)
{
	GET(BuildingClass* const, pBld, ESI);

	auto const cost = pBld->Type->GetActualCost(pBld->Owner);
	auto const stength = pBld->Type->Strength;
	auto const ShakeRules = RulesClass::Instance->ShakeScreen;
	auto const pTypeExt = TechnoTypeExt::ExtMap.Find(pBld->GetTechnoType());

	//and can be disabled manually
	if (ShakeRules
		&& !pTypeExt->AnotherData.DontShake.Get()
		&& cost
		&& cost > ShakeRules
		&& stength
		//&& pBld->IsOnMyView()
		)
	{
		int costfactor = cost / RulesClass::Instance->ShakeScreen;
		int strfactor = stength / RulesClass::Instance->ShakeScreen;

		if (costfactor)
			GScreenClass::Instance()->ScreenShakeX = ScenarioClass::Instance->Random.Random() % costfactor;

		if (strfactor)
			GScreenClass::Instance()->ScreenShakeY = ScenarioClass::Instance->Random.Random() % strfactor;

	}

	return 0x441C39; //return 0 causing crash
}

//7387DD , 5
//handle everything ourself
DEFINE_HOOK(0x7387DD, UnitClass_Destroyed_Shake, 0x5)
{
	GET(UnitClass* const, pUnit, ESI); //forEXT
	GET(int const, UnitStreght, ECX);
	GET(int const, Rules_Shake, EAX);
	auto const pTypeExt = TechnoTypeExt::ExtMap.Find(pUnit->GetTechnoType());

	if (UnitStreght
		&& Rules_Shake
		&& UnitStreght > Rules_Shake
		&& !pTypeExt->AnotherData.DontShake.Get()
		//&& pUnit->IsOnMyView()
		)
	{
		//original calculation
		auto  nMin = Math::max((UnitStreght % (Rules_Shake / 2)) , 6);
		auto  nMax = Math::max((UnitStreght / (Rules_Shake / 2) + 3), 6);

		if (nMin || nMax)
			GScreenClass::Instance()->ScreenShakeX = abs(ScenarioClass::Instance->Random(nMin, nMax));

		if (nMin || nMax)
			GScreenClass::Instance()->ScreenShakeY = abs(ScenarioClass::Instance->Random(nMin, nMax));
	}

	return 0x738801;
}