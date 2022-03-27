#include "Helpers.h"

#include <SuperWeaponTypeClass.h>
#include <SuperClass.h>
#include <AnimClass.h>
#include <HouseClass.h>
#include <Ext/Anim/Body.h>
#include <ScenarioClass.h>

void Otamaa::Helpers::LauchSW(bool Lauch, SuperWeaponTypeClass* LaunchWhat, HouseClass* pOwner, CoordStruct Where , bool WaitForCharge, bool ResetChargeAfterLauch)
{
	if (Lauch && pOwner)
	{
		if (LaunchWhat)
		{
			if (auto const pSuper = pOwner->Supers.Items[LaunchWhat->ArrayIndex])
			{
				bool const lauch = (WaitForCharge) && (!pSuper->IsCharged || (pSuper->IsPowered() && pOwner->HasLowPower())) ? false : true;

				if (lauch)
				{
					pSuper->Launch(CellClass::Coord2Cell(Where), true);

					if (ResetChargeAfterLauch)
						pSuper->Reset();
				}
			}
		}
	}
}

HouseClass* Otamaa::Helpers::GetHouseOwner(AnimClass* pWho, HouseClass* pFallback)
{
	auto const pObject = pWho->OwnerObject;
	auto const pHouse = pWho->Owner ? pWho->Owner : ((pObject) ? pObject->GetOwningHouse() : pFallback);

	return !pHouse ? HouseClass::FindCivilianSide() : pHouse;
}

HouseClass* Otamaa::Helpers::GetHouseOwner(HouseClass* pIn, HouseClass* pFallback)
{
	auto const pFirstPhaseHouse = pIn ? pIn : nullptr;
	auto const pSecondPhaseHouse = !pFirstPhaseHouse && pFallback ? pFallback : pFirstPhaseHouse;

	return !pSecondPhaseHouse ? HouseClass::FindCivilianSide() : pSecondPhaseHouse;
}

void Otamaa::Helpers::SpawnMultiple(std::vector<AnimTypeClass*>& nAnims, DynamicVectorClass<int>& nAmount, CoordStruct Where, HouseClass* pOwner , bool bRandom)
{
	if (!nAnims.empty())
	{
		const auto nCreateAnim = [&](int nIndex)
		{
			if (auto const pMultipleSelected = nAnims[nIndex])
			{
				if ((size_t)nAmount[nIndex] > 0)
				{
					for (size_t k = (size_t)nAmount[nIndex]; k > 0; --k)
					{
						if (auto pAnimCreated = GameCreate<AnimClass>(pMultipleSelected, Where))
						{
							AnimExt::SetAnimOwnerHouseKind(pAnimCreated, pOwner, nullptr, false);
						}
					}
				}
			}
		};

		if (!bRandom)
		{
			for (size_t i = 0; i< nAnims.size(); i++)
			{
				nCreateAnim(i);
			}
		}
		else
		{
			nCreateAnim(nAnims.size() == 1 ?
				0 : ScenarioClass::Instance->Random.RandomRanged(0, nAnims.size() - 1));
		}
	}
}

bool Otamaa::Helpers::CheckMinMax(double nMin, double nMax, int& nOutMin, int& nOutMax)
{
	int nMinL = Game::F2I(abs(nMin) * 256.0);
	int nMaxL = Game::F2I(abs(nMax) * 256.0);

	if (!nMinL || !nMaxL)
		return false;

	if (nMinL > nMaxL)
		std::swap(nMinL, nMaxL);

	nOutMin = nMinL;
	nOutMax = nMaxL;

	return true;
}

CoordStruct Otamaa::Helpers::GetRandomCoordsInsideLoops(double nMin, double nMax, CoordStruct nPos, int Increment)
{
	CoordStruct nBuff = nPos;
	int nMinL = 0;
	int nMaxL = 0;

	if (CheckMinMax(nMin, nMax , nMinL,nMaxL))
	{
		auto nRandomCoords = MapClass::GetRandomCoordsNear(nPos,
			(abs(ScenarioClass::Instance->Random.RandomRanged(nMinL, nMaxL)) * Math::min(Increment, 1)),
			ScenarioClass::Instance->Random.RandomRanged(0,1));

		nRandomCoords.Z = MapClass::Instance->GetCellFloorHeight(nRandomCoords);;
		nBuff = nRandomCoords;
	}

	return nBuff;
}