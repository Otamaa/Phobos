#include "GiftBoxFunctional.h"
#include <Misc/Otamaa/Misc/DynamicPatcher/Helpers/Helpers.h>
#include "GiftBox.h"

const bool OpenDisallowed(TechnoClass* const pTechno)
{
	if (pTechno)
	{
		bool bIsOnWarfactory = false;
		if (pTechno->WhatAmI() == AbstractType::Unit)
		{
			if (auto const pCell = pTechno->GetCell())
			{
				if (auto const pBuildingBelow = pCell->GetBuilding())
				{
					if (auto const pLinkedBuilding = specific_cast<BuildingClass*>(*pTechno->RadioLinks.Items))
					{
						bIsOnWarfactory = pLinkedBuilding->Type->WeaponsFactory && !pLinkedBuilding->Type->Naval && pBuildingBelow == pLinkedBuilding;
					}
				}
			}
		}

		return pTechno->Absorbed ||
			pTechno->InOpenToppedTransport ||
			bIsOnWarfactory ||
			pTechno->TemporalTargetingMe;
	}

	return false;
}

void GiftBoxFunctional::Init(TechnoExt::ExtData* pExt, TechnoTypeExt::ExtData* pTypeExt)
{
	if (!pExt || !pTypeExt || !pTypeExt->AnotherData.MyGiftBoxData.Enable)
		return;

	if (!pExt->AnotherData.MyGiftBox.get())
	{
		auto nDelay = pTypeExt->AnotherData.MyGiftBoxData.DelayMax == 0 ?
			pTypeExt->AnotherData.MyGiftBoxData.Delay :
			ScenarioClass::Instance->Random(
				pTypeExt->AnotherData.MyGiftBoxData.DelayMin,
				pTypeExt->AnotherData.MyGiftBoxData.DelayMax);

		pExt->AnotherData.MyGiftBox.reset(GameCreate<GiftBox>(pExt->OwnerObject(),nDelay));
	}
}

void GiftBoxFunctional::Destroy(TechnoExt::ExtData* pExt, TechnoTypeExt::ExtData* pTypeExt)
{
	if (!pExt || !pTypeExt)
		return;

	auto const pGBox = pExt->AnotherData.MyGiftBox.get();

	if( !pGBox || OpenDisallowed(pExt->OwnerObject()))
		return;

	if (pTypeExt->AnotherData.MyGiftBoxData.OpenWhenDestoryed && !pGBox->IsOpen)
	{
		pGBox->Release(pTypeExt->AnotherData.MyGiftBoxData);
		pGBox->IsOpen = true;
	}
}

void GiftBoxFunctional::AI(TechnoExt::ExtData* pExt, TechnoTypeExt::ExtData* pTypeExt)
{
	if (!pExt || !pTypeExt)
		return;

	auto const pGBox = pExt->AnotherData.MyGiftBox.get();

	if (!pTypeExt || !pGBox || OpenDisallowed(pExt->OwnerObject()))
		return;

	if (!pTypeExt->AnotherData.MyGiftBoxData.OpenWhenDestoryed && pGBox->CanOpen())
	{
		pGBox->Release(pTypeExt->AnotherData.MyGiftBoxData);

		if (pTypeExt->AnotherData.MyGiftBoxData.Remove)
		{
			pExt->OwnerObject()->Limbo();
			pExt->OwnerObject()->UnInit();

			return;
		}

		if (pTypeExt->AnotherData.MyGiftBoxData.Destroy)
		{
			auto nDamage = (pExt->OwnerObject()->Health + 1);
			pExt->OwnerObject()->ReceiveDamage(&nDamage, 0, RulesGlobal->C4Warhead, nullptr, true,
				!pTypeExt->OwnerObject()->Crewed, nullptr);

			return;
		}

		{
			auto nDelay = pTypeExt->AnotherData.MyGiftBoxData.DelayMax == 0 ?
				pTypeExt->AnotherData.MyGiftBoxData.Delay :
				ScenarioClass::Instance->Random(
					pTypeExt->AnotherData.MyGiftBoxData.DelayMin,
					pTypeExt->AnotherData.MyGiftBoxData.DelayMax);

			pGBox->Reset(nDelay);
		}
	}
}

CellClass* GiftBox::GetCell(CellClass* pIn, CoordStruct& InOut , size_t nSpread , bool EmptyCell)
{
	CellStruct cell = CellClass::Coord2Cell(InOut);
	auto const nDummy = GeneralUtils::AdjacentCellsInRange(nSpread);

	size_t const max = nDummy.size();
	for (size_t i = 0; i < max; i++)
	{
		int index = ScenarioGlobal->Random(0, max - 1);
		CellStruct const offset = nDummy[index];

		if (offset == CellStruct::Empty)
			continue;

		if (auto pNewCell = MapClass::Instance->TryGetCellAt(cell + offset))
		{
			if (pNewCell->LandType != pIn->LandType || (EmptyCell && pNewCell->GetContent()))
				continue;

			InOut = pNewCell->GetCoordsWithBridge();
			return pNewCell;
			break;
		}
	}

	return nullptr;
}

void GiftBox::Release(GiftBoxData& nData)
{
	auto const pHouse = OwnerObject->Owner;
	CoordStruct location = OwnerObject->GetCoords();

	if (auto pCell = MapClass::Instance->TryGetCellAt(location))
	{
		AbstractClass* pDest = nullptr;
		AbstractClass* pFocus = nullptr;

		if (OwnerObject->WhatAmI() != AbstractType::Building)
		{
			if (auto pFoot = generic_cast<FootClass*>(OwnerObject))
				pDest = pFoot->Destination;

			pFocus = OwnerObject->Focus;
		}

		std::vector<TechnoTypeClass*> nOut;

		if (nData.RandomType)
		{
			auto const nIdx = ScenarioGlobal->Random.RandomRanged(0,nData.Gifts.size() - 1);
			for (int i = 0; i < nData.Nums[nIdx]; ++i)
				nOut.push_back(nData.Gifts[nIdx]);
		}
		else
		{
			for (size_t i = 0; i < (nData.Gifts.size()); ++i)
			{
				for (int a = 0; a < nData.Nums[i] ; ++a)
				{
					nOut.push_back(nData.Gifts[i]);
				}
			}
		}

		if (!nOut.empty())
		{
			for (auto const& pTech : nOut)
			{
				if (nData.RandomRange > 0)
					if(auto const pNewCell = GetCell(pCell, location, (size_t)(nData.RandomRange.Get()), nData.EmptyCell))
						pCell = pNewCell;

				if (auto const pGift = Helpers_DP::CreateAndPutTechno(pTech, pHouse, location, pCell))
				{
					pGift->IsInPlayfield = true;

					if (!pDest && !pFocus)
					{
						pGift->Scatter(CoordStruct::Empty, true, false);
					}
					else
					{
						if (pGift->WhatAmI() != AbstractType::Building)
						{
							CoordStruct des = pDest ? location : pDest->GetCoords();

							if (pFocus)
							{
								pGift->SetFocus(pFocus);
								if (pGift->WhatAmI() == AbstractType::Unit)
								{
									des = pFocus->GetCoords();
								}
							}

							if (auto pTargetCell = MapClass::Instance->TryGetCellAt(des))
							{
								pGift->SetDestination(pTargetCell, true);
								pGift->QueueMission(Mission::Move, false);
							}
						}
					}
				}
				else
				{
					Debug::Log("Gift box release gift failed ,pType [%s]", pTech->get_ID());
				}
			}
		}
	}
}