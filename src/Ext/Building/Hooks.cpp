#include "Body.h"
#include <Utilities/Macro.h>

#include <Ext/BuildingType/Body.h>

#include <AnimClass.h>
#include <UnitClass.h>
#include <TacticalClass.h>
#include <ScenarioClass.h>

DEFINE_HOOK(0x7396D2, UnitClass_TryToDeploy_Transfer, 0x5)
{
	GET(UnitClass*, pUnit, EBP);
	GET(BuildingClass*, pStructure, EBX);

	if (pUnit->Type->DeployToFire && pUnit->Target)
		pStructure->LastTarget = pUnit->Target;

	if (auto pStructureExt = BuildingExt::ExtMap.Find(pStructure))
		pStructureExt->DeployedTechno = true;

	return 0;
}

DEFINE_HOOK(0x449ADA, BuildingClass_MissionConstruction_DeployToFireFix, 0x0)
{
	GET(BuildingClass*, pThis, ESI);

	auto pExt = BuildingExt::ExtMap.Find(pThis);
	if (pExt && pExt->DeployedTechno && pThis->LastTarget)
	{
		pThis->Target = pThis->LastTarget;
		pThis->QueueMission(Mission::Attack, false);
	}
	else
	{
		pThis->QueueMission(Mission::Guard, false);
	}

	return 0x449AE8;
}

/* gsl lib , i dont have  it*/
/* https://stackoverflow.com/questions/52863643/understanding-gslnarrow-implementation */
/* better way to check the index */
namespace gsl
{
	// narrow_cast(): a searchable way to do narrowing casts of values
	template <class T, class U>
	constexpr T narrow_cast(U&& u) noexcept
	{
		return static_cast<T>(std::forward<U>(u));
	}
};

// i dont want to edit YRPP , my kraken keep crashing when opening submodule !
class TacticalFake final : public TacticalClass
{
public :
	
	Point2D* ApplyOffsetPixel_(Point2D* pRet, Point2D* pOffset)
	{ JMP_THIS(0x6D2070); }

	Point2D ApplyOffsetPixel(Point2D Input)
	{
		Point2D nBuffer;
		ApplyOffsetPixel_(&nBuffer, &Input);
		return nBuffer;
	}
};

//remove memset for BuildingFireAnims
DEFINE_LJMP(0x43BA72, 0x43BA7F);

namespace DamageFireAnims
{
	template<class T, class I, class = typename std::enable_if<std::is_integral<I>::value>::type>
	void remove(std::vector<T>& v, I index)
	{
		const auto& iter = v.cbegin() + gsl::narrow_cast<typename std::vector<T>::difference_type>(index);
		v.erase(iter);
	}

	void HandleRemove(BuildingClass* pThis)
	{
		if (auto pExt = BuildingExt::ExtMap.Find(pThis))
		{
			for (auto const& pItem : pExt->DamageFireAnims)
				if (pItem != nullptr)
					pItem->UnInit();

			pExt->DamageFireAnims.clear();

		}
	}

	void HandleInvalidPtr(BuildingClass* pThis, void* ptr)
	{
		auto pExt = BuildingExt::ExtMap.Find(pThis);

		if (!pExt || !pExt->DamageFireAnims.empty())
			return;

		for (size_t i = 0; i < pExt->DamageFireAnims.size(); ++i)
		{
			if (pExt->DamageFireAnims[i] != nullptr)
			{
				if (pExt->DamageFireAnims[i] == ptr)
				{
					pExt->DamageFireAnims[i] = nullptr;
					DamageFireAnims::remove(pExt->DamageFireAnims, i);
				}
			}
			else
			{
				DamageFireAnims::remove(pExt->DamageFireAnims, i);
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

		auto const& pFire = pTypeext->DamageFire_Types.GetElements(RulesClass::Instance->DamageFireTypes);

		if (!pFire.empty() &&
			!(pTypeext->DamageFire_Offsets.Count == 0)
			)
		{
			//if still items there , just clear it 
			if (!pExt->DamageFireAnims.empty())
				HandleRemove(pThis);

			for (int i = 0; i < pTypeext->DamageFire_Offsets.Count; ++i)
			{
				auto nFireOffs = pTypeext->DamageFire_Offsets[i];
				auto pTactical = static_cast<TacticalFake*>(TacticalClass::Instance());
				auto nPixel = pTactical->ApplyOffsetPixel(nFireOffs);
				CoordStruct nPixCoord { nPixel.X, nPixel.Y, 0 };
				CoordStruct nBuffer;
				pThis->GetCenterCoord(&nBuffer);
				nPixCoord += nBuffer;

				if (auto const pFireType = pFire.at(ScenarioClass::Instance()->Random.RandomRanged(0,pFire.size() - 1)))
				{
					if (auto pAnim = GameCreate<AnimClass>(pFireType, nPixCoord))
					{
						auto nBuildingHeight = pType->GetFoundationHeight(false);
						auto nWidth = pType->GetFoundationWidth();
						auto nAdjust = ((3 * (nFireOffs.Y - 15 * nWidth + (-15) * nBuildingHeight)) >> 1) - 10;
						pAnim->ZAdjust = nAdjust > 0 ? 0 : nAdjust;
						if (pAnim->Type->End > 0)
							pAnim->Animation.Value = ScenarioClass::Instance()->Random.RandomRanged(0,pAnim->Type->End - 1);

						pAnim->Owner = pThis->GetOwningHouse();
						pExt->DamageFireAnims.emplace_back(pAnim);
					}
				}
			}
		}
	}
};

DEFINE_POINTER_CALL(0x43FC92, &DamageFireAnims::Construct);

DEFINE_HOOK(0x43BDD5, BuildingClass_DTOR_DamageFireAnims, 0x6)
{
	GET(BuildingClass*, pThis, ESI);
	DamageFireAnims::HandleRemove(pThis);
	return 0x43BDF6;
}

DEFINE_HOOK(0x44AB87, BuildingClass_MI_Deconstruct_DamageFireAnims, 0x6)
{
	GET(BuildingClass*, pThis, ESI);
	DamageFireAnims::HandleRemove(pThis);
	return 0x44ABAC;
}

DEFINE_HOOK(0x440076, BuildingClass_AI_DamageFireAnims_1, 0x6)
{
	GET(BuildingClass*, pThis, ESI);
	DamageFireAnims::HandleRemove(pThis);
	return 0x44009B;
}

DEFINE_HOOK(0x43FC99, BuildingClass_AI_DamageFireAnims_2, 0x6)
{
	GET(BuildingClass*, pThis, ESI);
	DamageFireAnims::HandleRemove(pThis);
	return 0x43FCBE;
}

DEFINE_HOOK(0x4458E4, BuildingClass_Limbo_DamageFireAnims, 0x6)
{
	GET(BuildingClass*, pThis, ESI);
	DamageFireAnims::HandleRemove(pThis);
	return 0x445905;
}

DEFINE_HOOK(0x4415F9, BuildingClass_Destroy_DamageFireAnims, 0x5)
{
	GET(BuildingClass*, pThis, ESI);
	DamageFireAnims::HandleRemove(pThis);

	R->EBX(0);
	return 0x44161C;
}

DEFINE_HOOK(0x43C2A0, BuildingClass_RemoveDamageFireAnims, 0x5)
{
	GET(BuildingClass*, pThis, ECX);
	DamageFireAnims::HandleRemove(pThis);
	return 0x43C2C9;
}

DEFINE_HOOK(0x44EA1C, BuildingClass_InvalidPointer_DamageFireAnims, 0x6)
{
	GET(BuildingClass*, pThis, ESI);
	GET(void*, ptr, EBP);
	DamageFireAnims::HandleInvalidPtr(pThis, ptr);
	return 0x44EA2F;
}