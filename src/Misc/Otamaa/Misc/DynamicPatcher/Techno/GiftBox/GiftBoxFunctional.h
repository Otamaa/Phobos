#pragma once
#include <Ext/TechnoType/Body.h>
#include <Ext/Techno/Body.h>

class GiftBoxData;
struct GiftBoxFunctional
{
	static void Init(TechnoExt::ExtData* pExt  , TechnoTypeExt::ExtData* pTypeExt);
	static void AI(TechnoExt::ExtData* pExt, TechnoTypeExt::ExtData* pTypeExt);
	static void Destroy(TechnoExt::ExtData* pExt, TechnoTypeExt::ExtData* pTypeExt);

};