#include "Body.h"

/*
DEFINE_HOOK(0x6EF8A1, TeamClass_GatherAtEnemyBase_Distance, 0x6)
{
	GET_BASE(ScriptActionNode*, pMissionType, 0x8);

	auto agrument = pMissionType->Argument;

	if (!agrument)
	{
		agrument = RulesClass::Instance->AISafeDistance;
	}

	R->EDX(agrument);
	return 0x6EF8A7;
}

DEFINE_HOOK(0x6EFB69, TeamClass_GatherAtFriendlyBase_Distance, 0x6)
{
	GET_BASE(ScriptActionNode*, pMissionType, 0x8);

	auto agrument = pMissionType->Argument;

	if (!agrument)
	{
		agrument = RulesClass::Instance->AISafeDistance;
	}

	R->EDX(agrument);
	return 0x6EFB6F;
}

DEFINE_HOOK(0x6EF23A, TeamClass_ExecuteTransportUnload_Transporter, 0x6)
{
	GET(FootClass*, pFoot, ESI);

	return (!pFoot->Passengers.FirstPassenger || pFoot->Transporter) ? 0x6EF2A7 : 0x6EF244;
}*/