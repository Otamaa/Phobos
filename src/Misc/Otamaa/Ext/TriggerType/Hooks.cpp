#include "Body.h"

#include <TagClass.h>
#include <TagTypeClass.h>
#include <TriggerClass.h>
#include <HouseClass.h>
#include <HouseTypeClass.h>
#include <TActionClass.h>

DEFINE_HOOK(0x684E44, GameInitialize_AddTagsForHouse, 0x5)
{
	GET(TagClass*, pThis, EAX);

	if (auto pTriggerType = pThis->Type->FirstTrigger)
	{
		if (const auto& pTType = TriggerTypeExt::ExtMap.Find(pTriggerType))
		{
			auto HouseIdx = pTType->HouseIdx;

			if (HouseIdx == -1 || HouseIdx == 0x2325)
				return 0;

			auto pHouse = (HouseClass::Index_IsMP(HouseIdx)) ? HouseClass::FindByIndex(HouseIdx) : HouseClass::FindByCountryIndex(HouseIdx);

			if (!pHouse)
				return 0;

			pHouse->RelatedTags.AddItem(pThis);
			return 0x684EA2;
		}
	}

	return 0;
}

DEFINE_HOOK(0x72652D, TriggerClass_RegisterEvent_PlayerX, 0x6)
{
	GET(TriggerClass*, pThis, ESI);

	if (const auto& pTType = TriggerTypeExt::ExtMap.Find(pThis->Type))
	{
		auto HouseIdx = pTType->HouseIdx;

		if (HouseIdx == -1 || HouseIdx == 0x2325)
			return 0;

		auto pHouse = (HouseClass::Index_IsMP(HouseIdx)) ? HouseClass::FindByIndex(HouseIdx) : HouseClass::FindByCountryIndex(HouseIdx);

		if (!pHouse)
			return 0;

		R->EAX(pHouse);
		return 0x726538;
	}

	return 0;
}

DEFINE_HOOK(0x7265E7, TriggerClass_FireActions, 0x7)
{
	GET(TriggerClass*, pThis, EDI);
	GET(TActionClass*, pTAction, ESI);
	GET(ObjectClass*, pObj, EBP);
	GET_STACK(CellStruct, loc, 0x18);

	auto HouseIdx = TriggerTypeExt::ExtMap.Find(pThis->Type)->HouseIdx;
	HouseClass* pHouse = nullptr;

	if (HouseIdx == -1)
		return 0;

	pHouse = (HouseClass::Index_IsMP(HouseIdx)) ? HouseClass::FindByIndex(HouseIdx) : HouseClass::FindByCountryIndex(HouseIdx);

	if (HouseIdx == 0x2325 || !pHouse)
		pHouse = HouseClass::FindByCountryIndex(pThis->House->ArrayIndex);

	if (!pTAction->Execute(pHouse, pObj, pThis, loc))
		return 0x726610;

	R->BL(1);
	return 0x72660E;
}

DEFINE_HOOK(0x7272AE, TriggerTypeClass_LoadFromINI_CountryName, 0x7)
{
	GET(char*, pHouseID, ESI);
	GET(TriggerTypeClass*, pThis, EBP);

	auto nIdx = HouseTypeClass::FindIndexOfName(pHouseID);

	if (nIdx != -1)
	{
		R->EDX(HouseTypeClass::Array.get()->GetItem(nIdx));
		return 0x7272C1;
	}

	TriggerTypeExt::ExtMap.Find(pThis)->HouseIdx = CRT::atoi(pHouseID);
	return 0x7272A4;
}