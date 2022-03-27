#include "Body.h"

#include <Ext/BuildingType/Body.h>
#include <Ext/Building/Body.h>

void Otamaa::BExt::ExtData::Init(BuildingClass* pThis)
{
	if (!pThis || !pThis->Type)
		return;

	auto const& pExt = BuildingExt::ExtMap.Find(pThis);

	if (!pExt)
		return;

	//YR doing similar thing , by memset the 8 Point2D array
	if (auto const& pTypeExt = BuildingTypeExt::ExtMap.Find(pThis->Type))
		if (pTypeExt->AnotherTypeData.DamageFire_Offs.Count > 0)
			pExt->AnotherData.DamageFireAnims.reserve((size_t)pTypeExt->AnotherTypeData.DamageFire_Offs.Count);
}