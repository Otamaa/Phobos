#include "Body.h"

#include <Ext/Anim/Body.h>
#include <Ext/AnimType/Body.h>
#include <Utilities/Macro.h>

static void __fastcall _AnimClass_SetOwner_Bomb(AnimClass* pAnim, BombClass* pThisBomb)
{
	auto const& pAnimExt = AnimTypeExt::ExtMap.Find(pAnim->Type);
	if (pAnimExt && pAnimExt->CreateUnit.Get())
		AnimExt::SetAnimOwnerHouseKind(pAnim, pThisBomb->OwnerHouse,
		pThisBomb->Target ? pThisBomb->Target->GetOwningHouse() : nullptr, false);
	else
		pAnim->Owner = pThisBomb->OwnerHouse;
}

static DamageAreaResult __fastcall _DamageArea
(
	CoordStruct* pCoord,
	int nDamage,
	TechnoClass* pSource,
	WarheadTypeClass* pWarhead,
	bool AffectTiberium, //false
	HouseClass* pSourceHouse //nullptr
)
{
	GET_REGISTER_STATIC_TYPE(BombClass*, pThisBomb, esi);

	auto const pExt = BombExt::ExtMap.Find(pThisBomb);

	if ((pExt && Phobos::Config::MoreDetailSLDebugLog))
		Debug::Log("Extension for Bomb[%x] found ! \n", pExt);

	auto const nCoord = *pCoord;
	auto const nResult = Map.DamageArea(nCoord, nDamage, pSource, pWarhead, pWarhead->Tiberium, pThisBomb->OwnerHouse);
	auto const pCell = Map[*pCoord];

	auto const nLandType = pCell ? pCell->LandType:LandType::Clear;
	if (auto const pAnimType = Map.SelectDamageAnimation(nDamage, pWarhead, nLandType, nCoord))
	{
		if (auto const pAnim = GameCreate<AnimClass>(pAnimType, nCoord, 0, 1, 0x2600, -15, false))
		{
			_AnimClass_SetOwner_Bomb(pAnim, pThisBomb);
		}
	}

	return nResult;
}

// skip the Explosion Anim block
DEFINE_LJMP(0x4387A8, 0x438857);
DEFINE_POINTER_CALL(0x4387A3, _DamageArea);