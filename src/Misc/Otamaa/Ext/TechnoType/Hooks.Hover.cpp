//#include <New/Type/HoverTypeClass.h>

#include <Ext/TechnoType/Body.h>
#include <Ext/Anim/Body.h>
#include <Ext/AnimType/Body.h>
#include <HoverLocomotionClass.h>
#include <DriveLocomotionClass.h>
#include <DropPodLocomotionClass.h>

/*; hover vehicle characteristics

1484    HoverHeight=120         ; height of hovering vehicles
1520    HoverDampen=40%         ; dampening effect on hover vehicle bounciness
1488    HoverBob=.04            ; time between hover 'bobs'
1496    HoverBoost=150%         ; hover speed when traveling on straight away
1504    HoverAcceleration=.02   ; time to accelerate to full speed
1512    HoverBrake=.03          ; time to decelerate to full stop
*/

namespace Temp_513D20
{
	HoverTypeClass* pType;
}

// we fetch the hover type here , reducing look-ups needed
// until we got new Ext system , this will do the job fine !
DEFINE_HOOK(0x513D3A, HoverLocomotionClass_513D20_FetchHovertype, 0x8)
{
	GET(HoverLocomotionClass* const, pLoco, ESI);

	if (auto const Linked = pLoco->Owner ? pLoco->Owner : pLoco->LinkedTo)
	{
		if (auto const pType = Linked->GetTechnoType())
		{
			Temp_513D20::pType = TechnoTypeExt::GetMyHover(pType);
			return 0x0;
		}
	}

	Temp_513D20::pType = HoverTypeClass::Array[0].get();
	return 0x0;
}

DEFINE_HOOK(0x513DD6, HoverLocomotionClass_513D20_HoverHeight1, 0x6)
{
	GET_STACK(int, heightGet, STACK_OFFS(0x28, 0x18));

	auto height = Temp_513D20::pType->GetHeight();

	R->EAX(heightGet - height);

	return 0x513DDC;
}

DEFINE_HOOK(0x513E8F, HoverLocomotionClass_513D20_HoverHeight2, 0xC)
{
	enum { ContinueCheck = 0x513E9D, SetHoverDampen = 0x513F0E };

	GET(int, comparator, EDI);

	auto const height = Temp_513D20::pType->GetHeight();

	if (comparator < height)
		return ContinueCheck;

	R->ECX(RulesClass::Instance());
	return SetHoverDampen;
}

//513EAA
DEFINE_HOOK(0x513EAA, HoverLocomotionClass_513D20_HoverHeight3, 0x5)
{
	auto const height = Temp_513D20::pType->GetHeight();

	_asm fild dword ptr[height];

	R->EAX(RulesClass::Instance());
	return 0x513EB5;
}

DEFINE_HOOK(0x513ECD, HoverLocomotionClass_513D20_HoverHeight4, 0x6)
{;
	auto const height = Temp_513D20::pType->GetHeight();

	R->ECX(RulesClass::Instance());
	R->EAX(height);

	return 0x513ED9;
}

DEFINE_HOOK(0x513F1B, HoverLocomotionClass_513D20_HoverDampen, 0x6)
{
	auto const dampen = Temp_513D20::pType->GetDampen();

	_asm fmul qword ptr[dampen];

	return 0x513F27;
}

DEFINE_HOOK(0x513DE0, HoverLocomotionClass_513D20_HeightCalc, 0x7)
{
	GET(HoverLocomotionClass* const, pLoco, ESI);
	GET_STACK(int, nVal, STACK_OFFS(0x28, 0x18));

	auto const nDVal = (double)nVal + pLoco->__Gravity_HoverDampen;
	auto const Linked = pLoco->Owner ? pLoco->Owner : pLoco->LinkedTo;

	if (!Linked)
		Debug::FatalErrorAndExit("Cannot Find Owner for HoverLocomotion !");

	auto const nFlags = Linked->WhatAmI();
	auto const nMult_A = nFlags == AbstractType::Unit ? 1.0 : 1.1;
	auto const nResult = Math::sin((double)((Unsorted::CurrentFrame + 2 * (int)nFlags) % ((int)(nMult_A * Temp_513D20::pType->GetBob() * 900.0))) * 6.283185307179586 / (nMult_A * Temp_513D20::pType->GetBob() * 900.0));
	R->EAX(Game::F2I(nResult + nResult + nDVal));

	return 0x513E62;
}
/*
DEFINE_HOOK(0x513E1A, HoverLocomotionClass_513D20_HoverBob, 0x6)
{
	auto const nBob = Temp_513D20::pType->GetBob();
	__asm  fmul qword ptr[nBob];
	return 0x513E20;
}
*/
DEFINE_HOOK(0x514A32, HoverLocomotionClass_Process_Anim, 0xB)
{
	GET(HoverLocomotionClass* const, pLoco, ESI);

	if (!(Unsorted::CurrentFrame % 10))
	{
		auto const Linked = pLoco->Owner ? pLoco->Owner : pLoco->LinkedTo;

		if (!Linked)
			Debug::FatalErrorAndExit("Cannot Find Owner for HoverLocomotion !");

		auto const pType = Linked->GetTechnoType();
		auto const pHover = TechnoTypeExt::GetMyHover(pType);
		auto pAnimType = pHover->GetAboveWaterAnim();
		bool bIsCloked = (Linked->CloakState == CloakState::Cloaked || Linked->CloakState == CloakState::Cloaking);

		if (!Linked->IsOnBridge() && Linked->GetCell()->LandType == LandType::Water)
		{
			if (pAnimType && !bIsCloked)
			{
				auto nCoord = Linked->GetCoords();
				if (auto pAnim = GameCreate<AnimClass>(pAnimType, nCoord))
					AnimExt::SetAnimOwnerHouseKind(pAnim, Linked->Owner, nullptr, false);
			}
		}
	}

	return 0x514AC8;
}

namespace Temp_515ED0
{
	HoverTypeClass* pType;
}

DEFINE_HOOK(0x515ED0, HoverLocomotionClass_515ED0_FetchHovertype, 0x6)
{
	GET(HoverLocomotionClass* const, pLoco, ECX);

	if (auto const Linked = pLoco->Owner ? pLoco->Owner : pLoco->LinkedTo)
	{
		if (auto const pType = Linked->GetTechnoType())
		{
			Temp_515ED0::pType = TechnoTypeExt::GetMyHover(pType);
			return 0x0;
		}
	}

	Temp_515ED0::pType = HoverTypeClass::Array[0].get();
	return 0x0;
}

//
DEFINE_HOOK(0x516179, HoverLocomotionClass_515ED0_HoverAccel, 0x6)
{
	auto const accel = Temp_515ED0::pType->GetAccel();

	_asm fld qword ptr[accel];

	return 0x516185;
}

DEFINE_HOOK(0x5161B1, HoverLocomotionClass_515ED0_HoverBrake, 0x6)
{
	auto const brake = Temp_515ED0::pType->GetBrake();

	_asm fld qword ptr[brake];

	return 0x5161BD;
}

DEFINE_HOOK(0x5167FC, HoverLocomotionClass_515ED0_ScoldSound, 0x5)
{
	auto const sound = Temp_515ED0::pType->GetScoldSound();

	if (sound != -1)
		VocClass::PlayGlobal(sound, 0x2000, 1.0);

	return 0x516818;
}

DEFINE_HOOK(0x51613B, HoverLocomotionClass_515ED0_HoverBoost, 0xC)
{
	auto const nBoostExt = Temp_515ED0::pType->GetBoost();

	R->ECX(&nBoostExt);

	return 0x516147;
}

DEFINE_HOOK(0x514349, HoverLocomotionClass_Process_515ED0_ClearContext, 0x9)
{
	Temp_515ED0::pType = nullptr;
	return 0x0;
}

DEFINE_HOOK(0x514AD0, HoverLocomotionClass_Process_513D20_ClearContext, 0x5)
{
	Temp_513D20::pType = nullptr;
	return 0x0;
}