#include <Ext/Anim/Body.h>
#include <Ext/AnimType/Body.h>
#include <Ext/WeaponType/Body.h>
#include <Ext/Scenario/Body.h>

/*	Replace this with BulletDetonate insted
*   this should able to make this more usable

DEFINE_HOOK(0x6E2443, TActionClass_DoExplosionAt_Override, 0xB)
{
	enum { Skip = 0x6E2510, Continue = 0x0 };

	GET(WeaponTypeClass*, pWeapon, ESI);
	GET(CoordStruct, nCoord, ECX);
	//GET_STACK(ObjectClass*, pObj, STACK_OFFS(0x24, -0x8));

	if (Map[nCoord] && pWeapon && pWeapon->Projectile)
		if(WeaponTypeExt::DetonateAt(pWeapon, nCoord))
			return Skip;

	return Continue;
}*/