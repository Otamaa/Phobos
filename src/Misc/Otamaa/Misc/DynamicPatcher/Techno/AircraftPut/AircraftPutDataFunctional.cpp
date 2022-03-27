#include "AircraftPutDataFunctional.h"
#include <Ext/Rules/Body.h>
#include <Misc/Otamaa/Misc/DynamicPatcher/Helpers/Helpers.h>

void AircraftPutDataFunctional::OnPut(TechnoExt::ExtData* pExt, TechnoTypeExt::ExtData* pTypeExt, CoordStruct* pCoord)
{
	if (!pExt)
		return;

	if (pExt->OwnerObject()->WhatAmI() != AbstractType::Aircraft || !pExt->OwnerObject()->Owner)
		return;

	auto const pTechno = (AircraftClass*)pExt->OwnerObject();

	if (!pTechno->Spawned)
	{
		auto const pType = pTechno->Type;
		auto const Iter = make_iterator(RulesGlobal->PadAircraft);

		if (!Iter.empty() && Iter.contains(pType))
		{
			bool const bRemoveIfNoDock = RemoveIfNoDock(pTypeExt->AnotherData.MuPutData);
			// remove extra pad Aircraft
			if (pType->AirportBound && bRemoveIfNoDock)
			{
				int const count = Helpers_DP::CountAircraft(pTechno->Owner,Iter);
				if (pTechno->Owner->AirportDocks <= 0 ||
					pTechno->Owner->AirportDocks < count)
				{
					pTechno->Owner->TransactMoney(pType->Cost);
					pTechno->Limbo();
					pTechno->UnInit();
					return;
				}
			}

			auto const nOffset = GetOffset(pTypeExt->AnotherData.MuPutData);

			// move location
			if (!pExt->AnotherData.aircraftPutOffsetFlag && nOffset)
			{
				pExt->AnotherData.aircraftPutOffsetFlag = true;
				pExt->AnotherData.aircraftPutOffset = true;
				if (!IsForceOffset(pTypeExt->AnotherData.MuPutData))
				{
					// check Building has Helipad
					if (auto pCell = MapClass::Instance->TryGetCellAt(*pCoord))
					{
						auto const pBuilding = pCell->GetBuilding();
						if (pBuilding && pBuilding->Type->Helipad)
							pExt->AnotherData.aircraftPutOffset = false;
					}
				}

				if (pExt->AnotherData.aircraftPutOffset)
					*pCoord += nOffset;

			}
		}
	}
}

void AircraftPutDataFunctional::AI(TechnoExt::ExtData* pExt, TechnoTypeExt::ExtData* pTypeExt)
{
	if (!pExt)
		return;

	if (auto const nOffset = GetOffset(pTypeExt->AnotherData.MuPutData))
	{
		pExt->AnotherData.aircraftPutOffset = false;
		auto const pTechno = pExt->OwnerObject();

		CoordStruct location = pTechno->Location;
		CoordStruct pos = location + nOffset;
		pTechno->SetLocation(pos);

		if (auto const pCell = MapClass::Instance->TryGetCellAt(location))
			pTechno->SetDestination(pCell, true);

		pTechno->QueueMission(Mission::Enter, false);
	}
}

CoordStruct AircraftPutDataFunctional::GetOffset(AircraftPutData& nData)
{
	auto const nOffset = nData.PosOffset.Get(RulesExt::Global()->AnotherData.MyPutData.PosOffset.Get());

	return { nOffset.X * 256 ,nOffset.Y * 256 , nOffset.Z * 256 };
}

bool AircraftPutDataFunctional::IsForceOffset(AircraftPutData& nData)
{
	return nData.ForceOffset.Get(RulesExt::Global()->AnotherData.MyPutData.ForceOffset.Get());
}

bool AircraftPutDataFunctional::RemoveIfNoDock(AircraftPutData& nData)
{
	return nData.RemoveIfNoDocks.Get(RulesExt::Global()->AnotherData.MyPutData.RemoveIfNoDocks.Get());
}