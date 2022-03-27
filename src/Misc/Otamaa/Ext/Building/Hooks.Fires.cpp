#include <Ext/Building/Body.h>

#include <SpecificStructures.h>
#include <ScenarioClass.h>
#include <WarheadTypeClass.h>

#include <Ext/BuildingType/Body.h>
#include <Ext/Anim/Body.h>
#include <Utilities/Macro.h>

namespace DamageFireAnims
{
	void HandleRemove(BuildingClass* pThis)
	{
		auto pExt = BuildingExt::ExtMap.Find(pThis);

		if (!pExt || pExt->AnotherData.DamageFireAnims.empty())
			return;

		for (auto& pItem : pExt->AnotherData.DamageFireAnims)
		{
			if (pItem != nullptr)
			{
				pItem->UnInit();
				pItem = nullptr;
			}
		}
	}

	void HandleInvalidPtr(BuildingClass* pThis, void* ptr)
	{
		auto pExt = BuildingExt::ExtMap.Find(pThis);

		if (!pExt || pExt->AnotherData.DamageFireAnims.empty())
			return;

		for (auto& pItem : pExt->AnotherData.DamageFireAnims)
		{
			if (pItem != nullptr)
			{
				if ((void*)pItem == ptr)
				{
					//pItem->UnInit();
					pItem = nullptr;
				}
			}
		}
	}

	void __fastcall Construct(BuildingClass* pThis, void* _)
	{
		auto pType = pThis->Type;
		auto const& pExt = BuildingExt::ExtMap.Find(pThis);
		auto const& pTypeext = BuildingTypeExt::ExtMap.Find(pType);

		if (!pExt || !pTypeext)
			return;

		auto const& pFire = pTypeext->AnotherTypeData.DamageFireTypes.GetElements(RulesGlobal->DamageFireTypes);

		if (!pFire.empty() &&
			!(pTypeext->AnotherTypeData.DamageFire_Offs.Count == 0)
			)
		{
			//check if there is still alive fire anim
			if (!pExt->AnotherData.DamageFireAnims.empty())
				HandleRemove(pThis);

			for (int i = 0; i < pTypeext->AnotherTypeData.DamageFire_Offs.Count; ++i)
			{
				auto nFireOffs = pTypeext->AnotherTypeData.DamageFire_Offs[i];
				auto nPixel = TacticalGlobal->ApplyOffsetPixel(nFireOffs);
				CoordStruct nPixCoord { nPixel.X, nPixel.Y, 0 };
				nPixCoord += pThis->GetCenterCoord();

				if (auto const pFireType = pFire.at(ScenarioGlobal->Random(0, pFire.size() - 1)))
				{
					if (auto pAnim = GameCreate<AnimClass>(pFireType, nPixCoord))
					{
						auto nBuildingHeight = pType->GetFoundationHeight(false);
						auto nWidth = pType->GetFoundationWidth();
						auto nAdjust = ((3 * (nFireOffs.Y - 15 * nWidth + (-15) * nBuildingHeight)) >> 1) - 10;
						pAnim->ZAdjust = nAdjust > 0 ? 0 : nAdjust;
						if (pAnim->Type->End > 0)
							pAnim->Animation.Value = ScenarioGlobal->Random(0, pAnim->Type->End - 1);

						pAnim->Owner = pThis->GetOwningHouse();
						pExt->AnotherData.DamageFireAnims.push_back(pAnim);
					}
				}
			}
		}
	}
};

DEFINE_POINTER_CALL(0x43FC92, &DamageFireAnims::Construct);
DEFINE_LJMP(0x460388, 0x46048E); // no thankyou , we handle it ourself !
DEFINE_LJMP(0x43BA72, 0x43BA7F); //remove memset for buildingFireAnims

DEFINE_HOOK(0x43BDD5, BuildingClass_Dtor_FireHandle, 0x6)
{
	GET(BuildingClass*, pThis, ESI);
	DamageFireAnims::HandleRemove(pThis);
	return 0x43BDF6;
}

DEFINE_HOOK(0x44AB87, BuildingClass_handleRemoveFire1, 0x6)
{
	GET(BuildingClass*, pThis, ESI);
	DamageFireAnims::HandleRemove(pThis);
	return 0x44ABAC;
}

DEFINE_HOOK(0x440076, BuildingClass_handleRemoveFire2, 0x6)
{
	GET(BuildingClass*, pThis, ESI);
	DamageFireAnims::HandleRemove(pThis);
	return 0x44009B;
}

DEFINE_HOOK(0x43FC99, BuildingClass_handleRemoveFire3, 0x6)
{
	GET(BuildingClass*, pThis, ESI);
	DamageFireAnims::HandleRemove(pThis);
	return 0x43FCBE;
}

DEFINE_HOOK(0x4458E4, BuildingClass_handleRemoveFire4, 0x6)
{
	GET(BuildingClass*, pThis, ESI);
	DamageFireAnims::HandleRemove(pThis);
	return 0x445905;
}

DEFINE_HOOK(0x4415F9, BuildingClass_handleRemoveFire5, 0x5)
{
	GET(BuildingClass*, pThis, ESI);
	DamageFireAnims::HandleRemove(pThis);

	R->EBX(0);
	return 0x44161C;
}

DEFINE_HOOK(0x43C2A0, BuildingClass_RemoveFire_handle, 0x5)
{
	GET(BuildingClass*, pThis, ECX);
	DamageFireAnims::HandleRemove(pThis);
	return 0x43C2C9;
}

DEFINE_HOOK(0x44EA1C, BuildingClass_DetachOrInvalidPtr_handle, 0x6)
{
	GET(BuildingClass*, pThis, ESI);
	GET(void*, ptr, EBP);
	DamageFireAnims::HandleInvalidPtr(pThis, ptr);
	return 0x44EA2F;
}

DEFINE_HOOK(0x44270B, BuildingClass_ReceiveDamge_OnFire, 0x9)
{
	enum { Skip = 0x4428FE };

	GET(BuildingClass* const, pThis, ESI);
	GET_STACK(CellStruct*, pCell, 0x10);
	REF_STACK(args_ReceiveDamage const, ReceiveDamageArgs, STACK_OFFS(0x9C, -0x4));

	if (!ReceiveDamageArgs.WH->Sparky)
		return Skip;

	auto const pTypeExt = BuildingTypeExt::ExtMap.Find(pThis->Type);

	bool Onfire = (pThis->GetHealthStatus() == HealthState::Green && pTypeExt->AnotherTypeData.HealthOnfire.GreenOnFire)
		|| (pThis->GetHealthStatus() == HealthState::Yellow && pTypeExt->AnotherTypeData.HealthOnfire.YellowOnFire)
		|| (pThis->GetHealthStatus() == HealthState::Red && pTypeExt->AnotherTypeData.HealthOnfire.RedOnFire);

	auto pFireType = pTypeExt->AnotherTypeData.OnFireTypes.GetElements(RulesClass::Instance->OnFire);

	if (pFireType.size() < 3 || !pFireType.size()) //to prevent crash the game
		Onfire = false;

	if (!Onfire)
		return Skip;

	for (; (pCell->X != 0x7FFF || pCell->Y != 0x7FFF); ++pCell)
	{
		auto nCellAdd = pThis->GetMapCoords() + *pCell;
		auto nDestCoord = CoordStruct { (nCellAdd.X * 256) + 128,(nCellAdd.Y * 256) + 128,0 };
		nDestCoord.Z = MapClass::Instance->GetCellFloorHeight(nDestCoord);

		auto PlayFireAnim = [&](int nLoop = 1, int nFireTypeAt = 2)
		{
			if (auto pAnimType = pFireType.at(nFireTypeAt))
			{
				nDestCoord = MapClass::GetRandomCoordsNear(nDestCoord, 96, false);
				if (auto const pAnim = GameCreate<AnimClass>(pAnimType, nDestCoord, 0, nLoop))
				{
					pAnim->SetOwnerObject(pThis);
					auto pKiller = ReceiveDamageArgs.Attacker;
					auto Invoker = (pKiller) ? pKiller->Owner : ReceiveDamageArgs.SourceHouse;

					AnimExt::SetAnimOwnerHouseKind(pAnim, Invoker, pThis->Owner, false);
				}
			}
		};

		switch (ScenarioClass::Instance->Random.RandomRanged(0, pThis->Type->GetFoundationWidth() + pThis->Type->GetFoundationHeight(false) + 5))
		{
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
			PlayFireAnim(ScenarioClass::Instance->Random(0, pFireType.size() - 1), 0);
			break;
		case 6:
		case 7:
		case 8:
			PlayFireAnim(ScenarioClass::Instance->Random(0, pFireType.size() - 1), 1);
			break;
		case 9:
			PlayFireAnim();
			break;
		default:
			break;
		}
	}

	return Skip;
}