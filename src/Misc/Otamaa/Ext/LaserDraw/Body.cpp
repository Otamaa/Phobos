#include "Body.h"

#include <Ext/Techno/Body.h>

std::map<TechnoClass*, LaserDrawClass*> LaserDrawExt::WhoCalledMe;
template<> const DWORD Extension<LaserDrawExt::base_type>::Canary = 0x11111221;

void LaserDrawExt::ExtData::InvalidatePointer(void* ptr, bool bRemoved)
{
	AnnounceInvalidPointer(Target, ptr);
	AnnounceInvalidPointer(Owner, ptr);
}

void LaserDrawExt::ExtData::SaveToStream(PhobosStreamWriter& Stm)
{
	Extension<base_type>::SaveToStream(Stm);
	Serialize(Stm);
}

void LaserDrawExt::ExtData::LoadFromStream(PhobosStreamReader& Stm)
{
	Extension<base_type>::LoadFromStream(Stm);
	Serialize(Stm);
}

bool LaserDrawExt::LoadGlobals(PhobosStreamReader& Stm)
{
	return Stm
		.Process(WhoCalledMe)
		.Success();
}

bool LaserDrawExt::SaveGlobals(PhobosStreamWriter& Stm)
{
	return Stm
		.Process(WhoCalledMe)
		.Success();
}

double LaserDrawExt::CalculateColor(int nPass, int nFrame)
{
	//auto nR = (BYTE)(CalculateColor(15, Unsorted::CurrentFrame + 30) * 255.0);
	//auto nB = (BYTE)(CalculateColor(15, Unsorted::CurrentFrame + 60) * 255.0);
	//auto nG = (BYTE)(CalculateColor(15, Unsorted::CurrentFrame) * 255.0);

	int v4 = nFrame % (6 * nPass);
	auto const v2 = v4 / nPass;

	if (v2 < 0)
	{
		if (v2 < 1)
			return 0.0;
	}
	else if (v2 < 1)
	{
		return (double)v2;
	}
	if (v2 <= 3)
		return 1.0;

	return 0.0;
}

std::array<ColorStruct, 5> TestColor
= { { { 0,50,0 },{ 40,0,0 },{10,0,50 } ,{ 30,10,10 },{ 30,10,10 } } };

void LaserDrawExt::Exec(LaserDrawClass* pLaser)
{
	if (auto pExt = LaserDrawExt::ExtMap.Find(pLaser))
	{
		if (pExt->Laser_MultiColors)
		{
			pLaser->Blinks = false;
			pLaser->Fades = false;
			pLaser->StartIntensity = 1.0f;
			pLaser->EndIntensity = 1.0f;

			int nTransDur = 2;
			int transitionCycle = (pExt->FramePasses / nTransDur)
				% TestColor.size();

			int currentColorIndex = transitionCycle;
			int nextColorIndex = (transitionCycle + 1) % TestColor.size();

			double blendingCoef = (pExt->FramePasses % nTransDur)
				/ (double)nTransDur;
			ColorStruct color = {
				(BYTE)(TestColor[currentColorIndex].R * (1 - blendingCoef) + TestColor[nextColorIndex].R * blendingCoef),
				(BYTE)(TestColor[currentColorIndex].G * (1 - blendingCoef) + TestColor[nextColorIndex].G * blendingCoef),
				(BYTE)(TestColor[currentColorIndex].B * (1 - blendingCoef) + TestColor[nextColorIndex].B * blendingCoef)
			};

			pLaser->InnerColor = color;
			pLaser->OuterColor = color;
			pLaser->OuterSpread = { 0,0,0 };

			++pExt->FramePasses;
		}

		if (pExt->Laser_RealTimePosition)
		{
			if (auto const pTarget = pExt->Target)
			{
				if (!pTarget->IsDead())
				{
					auto const nCoord = pTarget->GetCoords();
					pLaser->Target = nCoord;
				}
			}
		}

		if (auto const pTechOwner = pExt->Owner)
		{
			if (!pTechOwner->IsDead())
			{
				auto const nOverride = pExt->Overrider;
				auto const nFLH = TechnoExt::GetFLHAbsoluteCoords(pTechOwner, pExt->FLH, pTechOwner->GetTechnoType()->Turret, nOverride);
				pLaser->Source = nFLH;
			}
		}
	}
}

LaserDrawExt::ExtContainer LaserDrawExt::ExtMap;
LaserDrawExt::ExtContainer::ExtContainer() : Container("LaserDrawClass") { }
LaserDrawExt::ExtContainer::~ExtContainer() = default;

DEFINE_HOOK(0x54FE60, LaserDrawClass_CTOR, 0x5)
{
	GET(LaserDrawClass* const, pThis, ECX);

	LaserDrawExt::ExtMap.FindOrAllocate(pThis);
	return 0x0;
}

DEFINE_HOOK(0x54FFB0, LaserDrawClass_DTOR, 0x7)
{
	GET(LaserDrawClass* const, pThis, ECX);

	LaserDrawExt::ExtMap.Remove(pThis);
	return 0x0;
}

DEFINE_HOOK(0x550016, LaserDrawClass_DVC_DTOR, 0x7)
{
	GET(LaserDrawClass* const, pThis, ESI);

	LaserDrawExt::ExtMap.Remove(pThis);
	return 0x0;
}

DEFINE_HOOK(0x5501D7, LaserDrawClass_DTOR_Inline, 0x9)
{
	GET(LaserDrawClass* const, pThis, ESI);

	LaserDrawExt::ExtMap.Remove(pThis);
	return 0x0;
}