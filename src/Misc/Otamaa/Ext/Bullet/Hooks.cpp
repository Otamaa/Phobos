#include <Ext/Bullet/Body.h>
#include <Utilities/Macro.h>

static DamageAreaResult __fastcall _DamageArea
(
	CoordStruct* pCoord,
	int nDamage,
	TechnoClass* pSource,
	WarheadTypeClass* pWarhead,
	bool AffectTiberium, //true
	HouseClass* pSourceHouse //nullptr
)
{
	if (auto const BulletOwner = pSource ? pSource->GetOwningHouse() : HouseClass::FindCivilianSide())
		pSourceHouse = BulletOwner;

	auto const nCoord = *pCoord;
	return Map.DamageArea(nCoord, nDamage, pSource, pWarhead, pWarhead->Tiberium , pSourceHouse);
}

DEFINE_POINTER_CALL(0x469A83, &_DamageArea);