#include "Body.h"
#include <Ext/TechnoType/Body.h>

DEFINE_HOOK(0x471A8D, CaptureManagerClass_Overload_Count_A, 0x6)
{
	GET(CaptureManagerClass*, pThis, ESI);

	// too slow , the function hiccups when these around 
	// init it on CaptureExt then load it here 
	//auto pExt = TechnoTypeExt::ExtMap.Find(pThis->Owner->GetTechnoType());
	//auto nData = pExt->Overload_Count.Get(pRules->OverloadCount);
	//auto nData_2 = nData.Items;

	R->ECX(CaptureExt::ExtMap.Find(pThis)->OverloadCount.Items);
	return 0x471A93;
}

DEFINE_HOOK(0x471A9B, CeptureManagerClass_Overload_Count_B, 0x6)
{
	GET(CaptureManagerClass*, pThis, ESI);

	R->EDI((CaptureExt::ExtMap.Find(pThis)->OverloadCount.Count - 1));
	return 0x471AA2;
}

DEFINE_HOOK(0x471AB5, CeptureManagerClass_Overload_Frames, 0x6)
{
	GET(RulesClass*, pRules, EBX);
	GET(CaptureManagerClass*, pThis, ESI);
	GET(int, nIdx, EAX);

	auto pExt = TechnoTypeExt::ExtMap.Find(pThis->Owner->GetTechnoType());
	auto const& nData = pExt->Overload_Frames.Get(pRules->OverloadFrames);

	R->EDX(nData[nIdx]);
	return 0x471ABE;
}

DEFINE_HOOK(0x471AC7, CeptureManagerClass_Overload_Damage, 0x6)
{
	GET(RulesClass*, pRules, ECX);
	GET(CaptureManagerClass*, pThis, ESI);
	GET(int, nIdx, EAX);

	auto pExt = TechnoTypeExt::ExtMap.Find(pThis->Owner->GetTechnoType());
	auto const& nData = pExt->Overload_Damage.Get(pRules->OverloadDamage);

	R->EAX(nData[nIdx]);
	return 0x471AD0;
}

DEFINE_HOOK(0x471B39, CaptureManagerClass_Overload_DeathSound, 0x6)
{
	GET(RulesClass*, pRules, ECX);
	GET(CaptureManagerClass*, pThis, ESI);

	auto pExt = TechnoTypeExt::ExtMap.Find(pThis->Owner->GetTechnoType());
	R->ECX(pExt->Overload_DeathSound.Get(pRules->MasterMindOverloadDeathSound));

	return 0x471B3F;
}

DEFINE_HOOK(0x471C0C, CaptureManagerClass_Overload_ParticleSys, 0x6)
{
	GET(RulesClass*, pRules, EDX);
	GET(CaptureManagerClass*, pThis, ESI);

	auto pExt = TechnoTypeExt::ExtMap.Find(pThis->Owner->GetTechnoType());
	R->ECX(pExt->Overload_ParticleSys.Get(pRules->DefaultSparkSystem));

	return 0x471C12;
}