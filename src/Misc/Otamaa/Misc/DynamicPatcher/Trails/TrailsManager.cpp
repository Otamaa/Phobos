#include "TrailsManager.h"

#include <Ext/TechnoType/Body.h>
#include <Ext/Techno/Body.h>
#include <Ext/Bullet/Body.h>
#include <Ext/BulletType/Body.h>
#include <Ext/VoxelAnim/Body.h>
#include <Ext/VoxelAnimType/Body.h>

#pragma region TemplatedStuffs
template<typename T, typename Ttype, typename Tbase, typename TbaseType>
static bool CheckAndContruct(Tbase* pClass, TbaseType* pClassType, bool Clear = false, bool IsTechno = true)
{
	if (!pClassType)
		return false;

	auto const pExt = T::ExtMap.Find(pClass);
	auto const pTypeExt = Ttype::ExtMap.Find(pClassType);

	if (!pExt || !pTypeExt)
		return false;

	if (!pExt->AnotherData.Trails.empty())
	{
		if (Clear)
			pExt->AnotherData.Trails.clear();
		else
			return false;
	}

	if (pTypeExt->AnotherData.Trails.CurrentData.empty())
		return false;

	size_t nTotal = 0;
	for (auto const& pTrails : pTypeExt->AnotherData.Trails.CurrentData)
	{
		if (auto const pType = TrailType::Array[pTrails.CurrentType].get())
		{
			if (pType->Mode != TrailMode::NONE)
			{
				if (auto pTrail = std::make_unique<UniversalTrail>(pType, pTrails.FLHs
					, IsTechno ? pTrails.Onturrents : false))
				{
					pTrail->OnLandTypes = pTrails.OnLand;
					pTrail->OnTileTypes = pTrails.OnTileTypes;
					pExt->AnotherData.Trails.push_back(std::move(pTrail));
					++nTotal;
				}
			}
		}
	}

	if (nTotal > 0)
		pExt->AnotherData.Trails.resize(nTotal);
	else
		pExt->AnotherData.Trails.clear();

	return true;
}

template<typename Text, typename Tbase>
static bool ClearLastLoc(Tbase* pBase)
{
	auto const& pExt = Text::ExtMap.Find(pBase);

	if (!pExt)
		return false;

	if (!pExt->AnotherData.Trails.empty())
	{
		for (auto const& pTrail : pExt->AnotherData.Trails)
		{
			pTrail->ClearLastLocation();
		}
	}

	return true;
}

template <typename Text, typename Tbase>
static bool ClearVector(Tbase* pBase)
{
	auto const& pExt = Text::ExtMap.Find(pBase);

	if (!pExt)
		return false;

	pExt->AnotherData.Trails.clear();

	return true;
}
#pragma endregion

void TrailsManager::Construct(ObjectClass* pOwner, bool IsConverted)
{
	if (!pOwner || pOwner->WhatAmI() == AbstractType::Building || TrailType::Array.empty())
		return;

	switch (pOwner->WhatAmI())
	{
	case AbstractType::Unit:
	case AbstractType::Infantry:
	case AbstractType::Aircraft:
	{
		if (!CheckAndContruct<TechnoExt, TechnoTypeExt>((TechnoClass*)pOwner, pOwner->GetTechnoType(), IsConverted) && Phobos::Config::MoreDetailSLDebugLog)
			Debug::Log("Failed To construct Trails for TechnoType ! \n");
	}
	break;
	case AbstractType::Bullet:
	{
		if (!CheckAndContruct<BulletExt, BulletTypeExt>(((BulletClass*)pOwner), ((BulletClass*)pOwner)->Type, false, false) && Phobos::Config::MoreDetailSLDebugLog)
			Debug::Log("Failed To construct Trails for BulletType ! \n");
	}
	break;
	case AbstractType::VoxelAnim:
	{
		if (!CheckAndContruct<VoxelAnimExt, VoxelAnimTypeExt>(((VoxelAnimClass*)pOwner), ((VoxelAnimClass*)pOwner)->Type, false, false) && Phobos::Config::MoreDetailSLDebugLog)
			Debug::Log("Failed To construct Trails for VoxelAnimType ! \n");
	}
	break;
	default:
		break;
	}
}

void TrailsManager::AI(ObjectClass* pOwner)
{
	if (!pOwner || pOwner->WhatAmI() == AbstractType::Building || TrailType::Array.empty())
		return;

	switch (pOwner->WhatAmI())
	{
	case AbstractType::Unit:
	case AbstractType::Infantry:
	case AbstractType::Aircraft:
	{
		auto const& pExt = TechnoExt::ExtMap.Find((TechnoClass*)pOwner);

		if (!pExt->AnotherData.Trails.empty())
		{
			for (auto const& pTrails : pExt->AnotherData.Trails)
			{
				if (((TechnoClass*)pOwner)->CloakState == CloakState::Cloaking ||
					((TechnoClass*)pOwner)->CloakState == CloakState::Cloaked)
				{
					if (pTrails->Type->HideWhenCloak.Get())
						continue;
				}

				if (pTrails->Type->Mode == TrailMode::ANIM)
				{
					switch (pExt->AnotherData.MyDriveData.nState)
					{
					case DrivingState::Start:
					case DrivingState::Stop:
						pTrails->SetDrivingState(pExt->AnotherData.MyDriveData.nState);
						break;
					}
				}
				auto const nSource = TechnoExt::GetFLHAbsoluteCoords((TechnoClass*)pOwner, pTrails->FLH, pTrails->IsOnTurret);
				pTrails->DrawTrail(pOwner->GetOwningHouse(), nSource);
			}
		}

	}
	break;
	case AbstractType::Bullet:
	{
		auto const& pExt = BulletExt::ExtMap.Find((BulletClass*)pOwner);

		if (!pExt)
			return;

		if (!pExt->AnotherData.Trails.empty())
		{
			for (auto const& pTrails : pExt->AnotherData.Trails)
			{
				CoordStruct location = ((BulletClass*)pOwner)->Location;
				BulletVelocity& velocity = ((BulletClass*)pOwner)->Velocity;

				// We adjust LaserTrails to account for vanilla bug of drawing stuff one frame ahead.
				// Pretty meh solution but works until we fix the bug - Kerbiter
				CoordStruct drawnCoords
				{
					(int)(location.X + velocity.X),
					(int)(location.Y + velocity.Y),
					(int)(location.Z + velocity.Z)
				};

				pTrails->DrawTrail(((BulletClass*)pOwner)->GetOwningHouse(), location);
			}
		}
	}
	break;
	case AbstractType::VoxelAnim:
	{
		auto const& pExt = VoxelAnimExt::ExtMap.Find((VoxelAnimClass*)pOwner);

		if (!pExt)
			return;

		if (!pExt->AnotherData.Trails.empty())
		{
			CoordStruct location = ((VoxelAnimClass*)pOwner)->Bounce.GetCoords();

			for (auto const& pTrails : pExt->AnotherData.Trails)
			{
				pTrails->DrawTrail(((VoxelAnimClass*)pOwner)->GetOwningHouse(), location);
			}
		}
	}
	break;
	default:
		break;
	}
}

void TrailsManager::Hide(ObjectClass* pOwner)
{
	if (!pOwner || pOwner->WhatAmI() == AbstractType::Building || TrailType::Array.empty())
		return;

	switch (pOwner->WhatAmI())
	{
	case AbstractType::Unit:
	case AbstractType::Infantry:
	case AbstractType::Aircraft:
	{
		if (!ClearLastLoc<TechnoExt>((TechnoClass*)pOwner))
			return;
	}
	break;
	case AbstractType::Bullet:
	{
		if (!ClearLastLoc<BulletExt>((BulletClass*)pOwner))
			return;
	}
	break;
	case AbstractType::VoxelAnim:
	{
		if (!ClearLastLoc<VoxelAnimExt>((VoxelAnimClass*)pOwner))
			return;
	}
	break;
	default:
		break;
	}
}

void TrailsManager::CleanUp(ObjectClass* pOwner)
{
	if (!pOwner || pOwner->WhatAmI() == AbstractType::Building || TrailType::Array.empty())
		return;

	switch (pOwner->WhatAmI())
	{
	case AbstractType::Unit:
	case AbstractType::Infantry:
	case AbstractType::Aircraft:
	{
		if (!ClearVector<TechnoExt>((TechnoClass*)pOwner))
			return;
	}
	break;
	case AbstractType::Bullet:
	{
		if (!ClearVector<BulletExt>((BulletClass*)pOwner))
			return;
	}
	break;
	case AbstractType::VoxelAnim:
	{
		if (!ClearVector<VoxelAnimExt>((VoxelAnimClass*)pOwner))
			return;
	}
	break;
	default:
		break;
	}
}