#include "Body.h"

#include <Utilities/Macro.h>
#include <Ext/Techno/Body.h>

namespace LaserDrawTemp
{
	ColorStruct maxColor;
	TechnoClass* Firer;
}

DEFINE_HOOK(0x6FD22E, TechnoClass_Laser_Zap_FetchTechno, 0x7)
{
	GET(TechnoClass*, pThis, ESI);
	REF_STACK(ColorStruct, ColorBuffer, STACK_OFFS(0x48, 0x38));

	ColorBuffer.R = 0;
	ColorBuffer.G = 0;
	ColorBuffer.B = 0;
	LaserDrawTemp::Firer = pThis;

	return R->CL() ? 0x6FD241 : 0x6FD271;
}

static LaserDrawClass* __fastcall LaserDrawClass_(LaserDrawClass* pThis, void* _,
CoordStruct source,
CoordStruct target,
int zAdjust,
BYTE unknown,
ColorStruct innerColor,
ColorStruct outerColor,
ColorStruct outerSpread,
int duration,
bool blinks,
bool fades,
float startIntensity,
float endIntensity
)
{
	if (pThis)
	{
		if (LaserDrawTemp::Firer)
			LaserDrawExt::WhoCalledMe.insert({ LaserDrawTemp::Firer, pThis });

		LaserDrawTemp::Firer = nullptr;
		return static_cast<LaserDrawClassCopy*>(pThis)->
			_LaserDrawClass_CTOR(source, target, zAdjust, unknown, innerColor,
			outerColor, outerSpread, duration, blinks, fades, startIntensity,
				endIntensity);
	}

	return nullptr;
}

DEFINE_POINTER_CALL(0x6FD42B, &LaserDrawClass_);

DEFINE_HOOK(0x550173, LaserDrawClass_AI, 0x6)
{
	GET(LaserDrawClass* const, pThis, ESI);
	LaserDrawExt::Exec(pThis);
	return 0x0;
}

DEFINE_HOOK(0x6FF660, TechnoClass_Fire_At_SetLaserExt, 0x6)
{
	GET(TechnoClass*, pThis, ESI);
	GET(WeaponTypeClass*, pWeapon, EBX);
	GET_BASE(int, weaponIndex, 0xC);
	GET_BASE(AbstractClass*, pTarget, 0x8);

	if (LaserDrawExt::WhoCalledMe.contains(pThis) && pWeapon->IsLaser)
	{
		--pThis->CurrentBurstIndex;
		if (pThis->CurrentBurstIndex == -1)
			pThis->CurrentBurstIndex = pWeapon->Burst - 1;

		if (auto pLaserDrawExt = LaserDrawExt::ExtMap.Find(LaserDrawExt::WhoCalledMe[pThis]))
		{
			//pLaserDrawExt->Owner = pThis;
			//pLaserDrawExt->Target = pTarget;

			bool bFound = false;
			auto nBurstFLH = TechnoExt::GetBurstFLH(pThis, weaponIndex, bFound);
			pLaserDrawExt->FLH = nBurstFLH;

			if (!bFound || !nBurstFLH)
			{
				pLaserDrawExt->FLH = pThis->GetWeapon(weaponIndex)->FLH;
				pLaserDrawExt->FLH.Y = (int)(pLaserDrawExt->FLH.Y * std::pow(-1.0, pThis->CurrentBurstIndex));
			}

			++pThis->CurrentBurstIndex;
			pThis->CurrentBurstIndex %= pWeapon->Burst;

			pLaserDrawExt->Laser_MultiColors = true;
			pLaserDrawExt->Laser_ApplyColorFix = true;

			//pLaserDrawExt->Laser_RealTimePosition = true;
			//pLaserDrawExt->OwnerObject()->Fades = !pLaserDrawExt->Laser_RealTimePosition;

			bool AroundTarget = false;
			if (AroundTarget && pTarget)
				pLaserDrawExt->Overrider = pTarget->GetCoords();
		}

		LaserDrawExt::WhoCalledMe.erase(pThis);
	}

	return 0;
}

DEFINE_HOOK(0x550D1F, LaserDrawClass_DrawInHouseColor_Context_Set, 0x6)
{
	LaserDrawTemp::maxColor = ColorStruct(*R->lea_Stack<ColorStruct*>(0x14));
	return 0;
}

//Enables proper laser thickness and falloff of it
DEFINE_HOOK(0x550F47, LaserDrawClass_DrawInHouseColor_BetterDrawing, 0x5)
{
	GET(LaserDrawClass*, pThis, EBX);

	auto const pExt = LaserDrawExt::ExtMap.Find(pThis);
	if (pExt && pExt->Laser_ApplyColorFix)
	{// Restore overridden code that's needed - Kerbiter
		GET_STACK(bool, noQuickDraw, 0x13);
		R->ESI(noQuickDraw ? 8u : 64u);
		GET_STACK(int, currentThickness, 0x5C)

			// Map value from range of [1, Thickness] to [0, pi/2]
			double x = 0;
		if (pThis->Thickness > 1)
			x = Math::HalfPi * (currentThickness - 1) / (pThis->Thickness - 1);

		// Cosine function for falloff
		double mult = Math::cos(x);
		unsigned int r = (unsigned int)(mult * LaserDrawTemp::maxColor.R);
		unsigned int g = (unsigned int)(mult * LaserDrawTemp::maxColor.G);
		unsigned int b = (unsigned int)(mult * LaserDrawTemp::maxColor.B);

		R->EAX(r);
		R->ECX(g);
		R->EDX(b);

		return 0x550F9D;
	}

	return 0x0;
}