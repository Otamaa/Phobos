#include "Body.h"
#include <Locomotion.Macros.h>
#include <TunnelLocomotionClass.h>

/*
DEFINE_HOOK(0x7292B1, TunnelLocomotionClass_7291F0_Speed, 0x6)
{
	GET_LOCO(ESI);
	auto speed = pExt->TunnelSpeed.Get(RulesClass::Instance->TunnelSpeed);
	_asm fdiv qword ptr[speed];
	return 0x7292B7;
}

DEFINE_HOOK(0x729520, TunnelLocomotionClass_7294E0_Speed, 0x7)
{
	GET_LOCO(ESI);
	auto speed = pExt->TunnelSpeed.Get(RulesClass::Instance->TunnelSpeed);
	_asm fmul qword ptr[speed];
	return 0x729527;
}

DEFINE_HOOK(0x729953, TunnelLocomotionClass_7298F0_Speed, 0x7)
{
	GET_LOCO(ESI);
	auto speed = pExt->TunnelSpeed.Get(RulesClass::Instance->TunnelSpeed);
	_asm fmul qword ptr[speed];
	return 0x72995A;
}*/

// Tunnel Locomotor Speed multiplier
// Author : Otamaa
/*
DEFINE_HOOK(0x72929A, TunnelLocomotionClass_7291F0_Speed, 0x5)
{
	GET_LOCO(ESI);

	auto speed = pExt->TunnelSpeed.Get(RulesClass::Instance->TunnelSpeed);
	R->EAX(Game::F2I(64.0 / static_cast<double>(pType->ROT) / speed));

	return 0x7292BF;
}*/

//crash a lot
/*
DEFINE_HOOK(0x7294E6, TunnelLocomotionClass_7294E0_Speed, 0x9)
{
	enum { MarkForRedraw = 0x729565, ReturnAfterSetLocation = 0x729560 };
	GET(TunnelLocomotionClass*, pThis, ESI);

	auto pLinked = pThis->LinkedTo;
	auto& nLoc = pLinked->Location;

	auto nLinked = pThis->LinkedTo ? pThis->LinkedTo->get_ID() : "None";
	Debug::Log(__FUNCTION__" [%x] Owner [%s] \n", pThis, nLinked);

	if (nLoc.Z <= -256)
	{
		R->ECX(pLinked);
		return MarkForRedraw;
	}
	else
	{
		auto nSpeed = pLinked->GetCurrentSpeed();
		auto nTunnelSpeedMult = RulesGlobal->TunnelSpeed;

		auto nSpeedTotal = Game::F2I(nSpeed * nTunnelSpeedMult);
		if (nSpeedTotal <= 5)
			nSpeedTotal = 5;

		nLoc.Z -= nSpeedTotal;

		if (nLoc.Z < -256)
			nLoc.Z = -256;

		pLinked->SetLocation(nLoc);
	}

	return ReturnAfterSetLocation;
}
*/

DEFINE_HOOK(0x72951C, TunnelLocomotionClass_7294E0_Speed, 0x8)
{
	GET(TunnelLocomotionClass*, pThis, ESI);
	GET(RulesClass*, pRules, ECX);
	GET(int, nCurrentMovementSpeed, EAX);

	auto nLinked = pThis->LinkedTo ? pThis->LinkedTo->get_ID() : "None";
	Debug::Log(__FUNCTION__" [%x] Owner [%s] \n", pThis, nLinked);

	R->Stack(0x4, nCurrentMovementSpeed);
	R->EAX(Game::F2I((nCurrentMovementSpeed)*pRules->TunnelSpeed));

	return 0x72952C;
}

DEFINE_HOOK(0x72994F, TunnelLocomotionClass_7298F0_Speed, 0x8)
{
	GET(TunnelLocomotionClass*, pThis, ESI);
	GET(RulesClass*, pRules, ECX);
	GET(int, nCurrentMovementSpeed, EAX);

	auto nLinked = pThis->LinkedTo ? pThis->LinkedTo->get_ID() : "None";
	Debug::Log(__FUNCTION__" [%x] Owner [%s] \n", pThis, nLinked);

	R->Stack(0x12, nCurrentMovementSpeed);
	R->EAX(Game::F2I((nCurrentMovementSpeed)*pRules->TunnelSpeed));

	return 0x72995F;
}