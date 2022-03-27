#include "Body.h"
//#include <Misc/Otamaa/Misc/DynamicPatcher/Helpers/Helpers.h>
#include <Ext/Techno/Body.h>

bool Otamaa::TTExt::ExtData::AI(TechnoClass* pThis)
{
	//if (auto const pExt = TechnoExt::ExtMap.Find(pThis))
	//{
	//	if (!pExt->AnotherData.IsDtorEd)
	//		pExt->AnotherData.IsDtorEd = Helpers_DP::IsDead(pThis);
	//
	//	return !pExt->AnotherData.IsDtorEd;
	//}

	return false;
}