#include <Ext/WarheadType/Body.h>
#include <Ext/Anim/Body.h>

DEFINE_HOOK(0x469D3C, BulletClass_Detonate_Debris, 0xA)
{
	GET(BulletClass*, pThis, ESI);
	GET(int, nTotalSpawn, EBX);
	GET(WarheadTypeClass*, pWarhead, EAX);

	HouseClass* const pOWner = pThis->Owner ? pThis->Owner->GetOwningHouse() :HouseClass::FindCivilianSide();
	HouseClass* const Victim = (pThis->Target) ? pThis->Target->GetOwningHouse() : nullptr;
	CoordStruct nCoords { 0,0,0 };

	if (pWarhead->DebrisTypes.Count > 0 && pWarhead->DebrisMaximums.Count > 0)
	{
		nCoords = pThis->GetCoords();
		for (int nCurIdx = 0; nCurIdx < pWarhead->DebrisTypes.Count; ++nCurIdx)
		{
			if (pWarhead->DebrisMaximums[nCurIdx] > 0)
			{
				int nAmountToSpawn = abs(int(ScenarioGlobal->Random.Random())) % pWarhead->DebrisMaximums[nCurIdx] + 1;
				nAmountToSpawn = Math::LessOrEqualTo(nAmountToSpawn, nTotalSpawn);
				nTotalSpawn -= nAmountToSpawn;

				for (; nAmountToSpawn > 0; --nAmountToSpawn)
				{
					if (auto const pVoxelAnimType = pWarhead->DebrisTypes[nCurIdx])
						GameCreate<VoxelAnimClass>(pVoxelAnimType, &nCoords, pOWner);
				}
			}

			if (nTotalSpawn <= 0)
			{
				nTotalSpawn = 0;
				break;
			}
		}
	}

	if (!pWarhead->DebrisTypes.Count && (nTotalSpawn > 0))
	{
		const auto& pWHExt = WarheadTypeExt::ExtMap.Find(pWarhead);
		const auto& AnimDebris = pWHExt->AnotherData.DebrisAnimTypes.GetElements(RulesClass::Instance->MetallicDebris);

		if (!AnimDebris.empty())
		{
			nCoords = pThis->GetCoords();
			nCoords.Z += 20;

			for (int i = 0; i < nTotalSpawn; ++i)
			{
				if (auto const pAnimType = AnimDebris[ScenarioClass::Instance->Random(0, AnimDebris.size() - 1)])
				{
					if (auto pAnim = GameCreate<AnimClass>(pAnimType, nCoords))
						AnimExt::SetAnimOwnerHouseKind(pAnim, pOWner, Victim, false);
				}
			}
		}
	}

	return 0x469EBA;
}