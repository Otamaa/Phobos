#include <Ext/TechnoType/Body.h>
#include <AircraftClass.h>

DEFINE_HOOK(0x415991, AircraftClass_Mission_Paradrop_Overfly_Radius, 0x6)
{
	enum { ConditionMeet = 0x41599F, ConditionFailed = 0x4159C8 };

	GET(AircraftClass* const, pThis, ESI);
	GET(int, comparator, EAX);

	auto const pExt = TechnoTypeExt::ExtMap.Find(pThis->GetTechnoType());
	auto Radius = pExt->AnotherData.ParadropOverflRadius.Get(RulesClass::Instance->ParadropRadius);

	return comparator > Radius ? ConditionMeet : ConditionFailed;
}

DEFINE_HOOK(0x415934, AircraftClass_Mission_Paradrop_Approach_Radius, 0x6)
{
	enum { ConditionMeet = 0x415942, ConditionFailed = 0x415956 };

	GET(AircraftClass* const, pThis, ESI);
	GET(int, comparator, EAX);

	auto const pExt = TechnoTypeExt::ExtMap.Find(pThis->GetTechnoType());
	auto Radius = pExt->AnotherData.ParadropRadius.Get(RulesClass::Instance->ParadropRadius);

	return comparator < Radius ? ConditionMeet : ConditionFailed;
}

DEFINE_HOOK(0x413D74, AircraftClass_CTOR_ParaLeft, 0x7)
{
	GET(AircraftClass*, pThis, ESI);

	auto const pType = pThis->Type;
	auto const pExt = TechnoTypeExt::ExtMap.Find(pType);
	auto const& nData = pExt->AnotherData;

	pThis->___paradrop_attempts = nData.Paradrop_MaxAttempt.Get(5); //5
	return 0x413D7B;
}

DEFINE_HOOK(0x415E93 , AircraftClass_DropCargo_ParaLeft, 0x7)
{
	GET(AircraftClass*, pThis, EDI);

	auto const pType = pThis->Type;
	auto const pExt = TechnoTypeExt::ExtMap.Find(pType);
	auto const& nData = pExt->AnotherData;

	pThis->___paradrop_attempts = nData.Paradrop_MaxAttempt.Get(5); //5
	return 0x415E9A;
}