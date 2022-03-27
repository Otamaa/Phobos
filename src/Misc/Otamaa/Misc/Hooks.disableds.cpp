#include <AnimClass.h>
#include <BuildingClass.h>
#include <TechnoClass.h>
#include <FootClass.h>
#include <UnitClass.h>
#include <ScenarioClass.h>
#include <VoxelAnimClass.h>
#include <BulletClass.h>
#include <TunnelLocomotionClass.h>
#include <JumpjetLocomotionClass.h>

#include <Ext/Anim/Body.h>
#include <Ext/AnimType/Body.h>
#include <Ext/Bullet/Body.h>
#include <Ext/Rules/Body.h>
#include <Ext/Techno/Body.h>
#include <Ext/TechnoType/Body.h>
#include <Ext/BuildingType/Body.h>
#include <Ext/HouseType/Body.h>
#include <Ext/WeaponType/Body.h>

#include <Utilities/Macro.h>
#include <Utilities/Debug.h>
#include <Utilities/TemplateDef.h>
#include <Utilities/GeneralUtils.h>


/*
namespace DamageGroupHande
{
	void AddToVector(DynamicVectorClass<DamageGroup*>& nVec, ObjectClass* pObject, int nRange, int nCounter, int nRangeTresh, REGISTERS* R)
	{
		if (pObject && pObject->IsAlive && pObject->IsOnMap && pObject->Health > 0 && pObject->GetHeight() > 0 && nRange <= nRangeTresh)
		{
			if (nCounter && nRange < 85)
				if (auto const pTechno = generic_cast<TechnoClass*>(pObject))
					if (pTechno->IsIronCurtained() && !pTechno->ForceShielded)
						R->Stack(0x17, 1);

			auto pDamagroup = GameCreate<DamageGroup>(pObject, nRange);

			nVec.AddUnique(pDamagroup);
		}
	}
}

DEFINE_HOOK(0x4893BA, DamageArea_DamageAir_Optimize, 0x9)
{
	GET(const CoordStruct* const, pCoords, EDI);
	GET(const WarheadTypeClass* const, pWarhead, ESI);
	GET(int const, nFloorHeight, EAX);
	//GET_STACK(const CellClass*, pCell, STACK_OFFS(0xE0, 0xC0));
	GET_STACK(int, nCounter, 0x1D);
	GET_STACK(int, nSpreadFactor, 0x68);
	REF_STACK(DynamicVectorClass<DamageGroup*>, nDamageGroupVec, 0x38);

	auto const pWarheadExt = WarheadTypeExt::ExtMap.Find(pWarhead);
	auto nWHDamagingAltitude = Game::F2I(pWarheadExt->DamagingAltitude.Get(RulesExt::Global()->DamagingAltitude.Get()));

	if (nWHDamagingAltitude == -1)
	{
		return (nFloorHeight >= pCoords->Z) ? 0x48955E : 0x4893C3;
	}
	else
	{
		if (nWHDamagingAltitude < -1)
		{
			for (auto const& pFoot : *FootClass::Array)
			{
				Point2D nPointbuffer = Point2D::Empty;
				nPointbuffer.X = nSpreadFactor - pFoot->Location.X;
				nPointbuffer.Y = nCounter - pFoot->Location.Y;
				DamageGroupHande::AddToVector(nDamageGroupVec, pFoot, Game::F2I(Math::sqrt((nPointbuffer * nPointbuffer))), nCounter, nSpreadFactor, R);
			}
		}
		else
		{
			for (auto const& pFoot : *FootClass::Array)
			{
				CoordStruct nCoordBufferResult = CoordStruct::Empty;
				nCoordBufferResult.X = nSpreadFactor - pFoot->Location.X;
				nCoordBufferResult.Y = nCounter = pFoot->Location.Y;
				nCoordBufferResult.Z = Game::F2I(pFoot->Location.Z - pCoords->Z * 256.0 / pWarheadExt->DamagingAltitude.Get(RulesExt::Global()->DamagingAltitude.Get()));
				DamageGroupHande::AddToVector(nDamageGroupVec, pFoot, Game::F2I(Math::sqrt((nCoordBufferResult * nCoordBufferResult))), nCounter, nSpreadFactor, R);
			}
		}
	}

	return 0x48955E;
}

*/
/*
 *	there is "goto" statement around the code , that mean it is repeatable code
 *  to go around with original style repetitife code , use lamba
 *  working not that far from vanilla , but this function do essential calcualtion then creating the DamageGroup later
 *	opposite of the Vanilla do , result it is saving few extra FPS on certain condition
 *	May have negative side affects , please test before ship this ! (broke damage to terrain object)

DEFINE_HOOK(0x48975B, DamageArea_MakeGroup, 0x7)
{
	REF_STACK(DynamicVectorClass<DamageGroup*>, nDamageGroupVec, 0x38);
	GET_STACK(size_t, nCounter, 0x10);
	GET_STACK(bool, bIgnoreDefense, 0x1D);
	GET_STACK(CellClass*, pCell, 0x20);
	GET_BASE(WarheadTypeClass*, pWarhead, 0xC);
	GET(AbstractClass*, pObject, ESI);
	GET(CoordStruct*, pCoords, EDI);

	auto nCoords = *pCoords;

	auto SetAndReturn = [&R, &pWarhead, &pObject, &nDamageGroupVec, &bIgnoreDefense, &nCounter](int nDistance = 0, CoordStruct nCoords = CoordStruct::Empty, bool bModifyCoord = false)
	{
		auto nDistanceBuffer = nDistance;

		if (bModifyCoord)
		{
			if (auto const pWarheadExt = WarheadTypeExt::ExtMap.Find(pWarhead))
			{
				auto nWHDamagingAltitude = pWarheadExt->DamagingAltitude.Get(RulesExt::Global()->DamagingAltitude.Get());

				if (nWHDamagingAltitude > 0.0f && nCoords.Z)
					nCoords.Z = Game::F2I(nCoords.Z * 256.0 / nWHDamagingAltitude);
			}

			nDistanceBuffer = Game::F2I(nCoords.MagnitudeSquared());
		}

		auto pDamagroup = GameCreate<DamageGroup>(pObject, nDistanceBuffer);

		if (bIgnoreDefense && !nCounter)
				if (auto const pTechno = generic_cast<TechnoClass*>(pObject))
					if (pTechno->IsIronCurtained() && !pTechno->ForceShielded && pDamagroup->Distance < 85)
						R->Stack(0x17, 1);

		nDamageGroupVec.AddUnique(pDamagroup);

		return 0x4899B3;
	};

	if (pObject->WhatAmI() == AbstractType::Building)
	{
		if (!nCounter)
		{
			auto nCenterCoord = pCell->GetCenterCoords(); //LeptonCoord
			if (nCoords.Z - nCenterCoord.Z <= 2 * Unsorted::LevelHeight)
				return SetAndReturn();

			CoordStruct nBuffer_1 = (pCell->GetCenterCoords() - nCoords);
			return SetAndReturn(Game::F2I(nBuffer_1.Magnitude() - 2 * Unsorted::LevelHeight));
		}
		else
		{
			CoordStruct nBuffer_2 = (pCell->GetCenterCoords() - nCoords);
			return SetAndReturn(0, nBuffer_2, true);
		}
	}

	CoordStruct nBuffer_3 = (pObject->GetCoords() - nCoords);
	return SetAndReturn(0, nBuffer_3, true);
} */

/*
DEFINE_HOOK(0x469D95, BulletClass_Detonate_DebrisType_Owner, 0x5)
{
	GET(BulletClass* const, pBullet, ESI);
	GET(int, nLoopVal, EDX);

	auto Owner = HouseClass::FindCivilianSide();
	Owner = pBullet->Owner ? pBullet->Owner->Owner : Owner;
	auto nCoords = pBullet->GetCenterCoords();

	for (int i = 0; i < nLoopVal; ++i)
	{
		if (auto pDebrisTypes = pBullet->WH->DebrisTypes[i])
			GameCreate<VoxelAnimClass>(pDebrisTypes, &nCoords, Owner);
		--nLoopVal;
	}

	R->Stack(STACK_OFFS(0xA4, 0x6C), nLoopVal);
	return  0x469DE5;
}*/


namespace Temp
{
	bool bDisableDebug = true;

	/*
	int AdjustWindow_Child(REGISTERS* R)
	{
		auto nEDX = R->EDX();
		auto nESI = R->ESI();

		LPRECT nRect;
		Imports::GetWindowRect(Unsorted::Game_hWnd(), nRect);

		R->ESI(nESI - nRect->top);
		R->EDX(nEDX - nRect->left);

		return 0;
	}
	*/
};
/*

DEFINE_HOOK(0x6E21E0, TAction_DoResizePlayerView_Debug, 0x5)
{
	GET_STACK(TriggerClass*, pTrigger, 0xC);
	Debug::Log_WithBool(DebugC::Enabled, __FUNCTION__" : Trigger = %s[%s]\n", pTrigger->Type->ID, pTrigger->Type->Name);
	return 0;

}

DEFINE_HOOK(0x6DEBF9, TActionClass_AllToHunt_Debug, 0x6)
{
	GET_STACK(TriggerClass*, pTrigger, 0x25C);
	Debug::Log_WithBool(DebugC::Enabled, __FUNCTION__" : Trigger = %s[%s]\n", pTrigger->Type->ID, pTrigger->Type->Name);
	return 0;
}*/

// Duh , WWp take these parameter , but not do check and or input it on DoExplosionDamage and Anim->Owner
// Which cause explosion and animation created from these parameter is `nullptr` altho they shouldnt be !
// Which is baffled me , really WWp ?

/*
namespace HousePointerHelper
{
	HouseClass* pAnimHouse = nullptr;
	HouseClass* pExplHouse = nullptr;
};

DEFINE_HOOK(0x6DEC95, TActionClass_PlayAnimAt_IstheseParamvail, 0x7)
{
	GET(TActionClass*, pThis, ESI);
	GET_STACK(HouseClass*, phouse, 0x254);
	GET_STACK(ObjectClass*, pObj, 0x258);
	GET_STACK(TriggerClass*, ptrig, 0x25C);

	auto nHouse = phouse ? phouse->Type->ID : NONE_STR;
	auto nObj = pObj ? pObj->GetType()->ID : NONE_STR;
	auto nTrig = ptrig ? ptrig->Type->ID : NONE_STR;

	Debug::Log_WithBool(DebugC::Enabled, __FUNCTION__" [%p] House[%s] Object[%s] Trig[%s] .\n", pThis, nHouse, nObj, nTrig);

	return 0;
}

DEFINE_HOOK(0x6DECC2, TActionClass_DoExplosionAt_IstheseParamAvail, 0x7)
{
	GET(TActionClass*, pThis, ESI);
	GET_STACK(HouseClass*, phouse, 0x254);
	GET_STACK(ObjectClass*, pObj, 0x258);
	GET_STACK(TriggerClass*, ptrig, 0x25C);

	auto nHouse = phouse ? phouse->Type->ID : NONE_STR;
	auto nObj = pObj ? pObj->GetType()->ID : NONE_STR;
	auto nTrig = ptrig ? ptrig->Type->ID : NONE_STR;

	Debug::Log_WithBool(DebugC::Enabled, __FUNCTION__" [%p] House[%s] Object[%s] Trig[%s] .\n", pThis, nHouse, nObj, nTrig);

	return 0;
}
*/
/*
//do AE here
DEFINE_HOOK(0x6E2443, TActionClass_DoExplosionAt_Debug, 0x6)
{
	GET(WeaponTypeClass*, pWeapon, ESI);
	GET_STACK(CoordStruct, pCoord, 0x18);

	Debug::Log_WithBool(DebugC::Enabled, __FUNCTION__" WeaponType[%s] detoneted at [%d, %d, %d].\n", pWeapon->ID, pCoord.X, pCoord.Y, pCoord.Z);

	return 0;
}

DEFINE_HOOK(0x6DD913, TActionClass_IonCannon_Override, 0x9)
{
	GET(TActionClass*, pThis, ESI);

	if (auto pCell = MapClass::Instance->TryGetCellAt(ScenarioClass::Instance->GetWaypointCoords(pThis->Waypoint)))
	{
		GET_STACK(HouseClass*, pHouse, STACK_OFFS(0x250, -0x4));
		GET_STACK(ObjectClass*, pObj, STACK_OFFS(0x250, -0x8));

		auto nCoord = pCell->GetCoords();
		auto pOwner = pObj ? pObj : nullptr;
		TechnoClass* pTechOwner = pOwner ? abstract_cast<TechnoClass*>(pOwner) : nullptr;
		auto nIsTechEligible = pTechOwner ? pTechOwner->Owner == pHouse : false;
		pTechOwner = nIsTechEligible ? pTechOwner : nullptr;

		//		if (auto pIon = GameCreate<IonBlastClass>(nCoord))
		//		{
		//			IonBlastExt(pIon, nullptr, pHouse, pTechOwner);
		//		}
	}

	return 0x6DD9BC;
}


*/
/*
DEFINE_HOOK(0x6E2443, TActionClass_DoExplosionAt_Override, 0x5)
{
	GET(WeaponTypeClass*, pWeapon, ESI);

	if (pWeapon)
	{
		GET_STACK(CoordStruct, nCoord, STACK_OFFS(0x24, -0xC));
		GET_BASE(HouseClass*, pHouse, STACK_OFFS(0x24, -0x28));

		auto nDamage = pWeapon->Damage;
		if (auto pCell = MapClass::Instance->TryGetCellAt(nCoord))
		{
			if (auto pAnimType = MapClass::SelectDamageAnimation(nDamage, pWeapon->Warhead, pCell->LandType, nCoord))
				if (auto pAnim = GameCreate<AnimClass>(pAnimType, nCoord, 0, 1, 0x2600, -15, false))
					AnimExt::SetAnimOwnerHouseKind(pAnim, pHouse
						, nullptr, AnimTypeExt::ExtMap.Find(pAnimType)->Anim_Owner.Get(), false);

			if (auto pBulletExt = BulletTypeExt::ExtMap.Find(pWeapon->Projectile))
			{
				if (auto pBullet = pBulletExt->CreateBullet(pCell, nullptr, pWeapon, nDamage))
				{
					pBullet->SetLocation(nCoord);
					pBullet->Explode();
					pBullet->Limbo(); //Limbo
					pBullet->UnInit();

				}
			}
		}
	}

	return 0x6E2510;
}

DEFINE_HOOK(0x65DD30, TeamTypeClass_GenerateTeamMemembers_Debug, 0x5)
{
	GET(TeamTypeClass*, pThis, ECX);

	Debug::Log_WithBool(DebugC::Enabled, "Generating a new team '%s'[%s].\n", pThis->ID, pThis->Name);

	return 0;
}

DEFINE_HOOK(0x6DE189, TActionClass_MakeEnemy_DEBUG, 0x6)
{
	GET_STACK(TriggerClass*, ptrig, 0x25C);

	Debug::Log_WithBool(DebugC::Enabled, __FUNCTION__": Trigger = %s[%s]\n", ptrig->Type->ID, ptrig->Type->Name);

	return 0;
}
// failed to kick out unit for some reason !
DEFINE_HOOK(0x444B83, BuildingClass_KickOutUnit_Offset, 0x7)
{
	enum { CheckGDIOfsset = 0x444B8C, CheckNODOffset = 0x444BD8, Handled = 0x444C7C };

	GET(BuildingTypeClass*, pThis, ECX);

	if (const auto& pTypeExt = BuildingTypeExt::ExtMap.Find(pThis))
	{
		GET_STACK(short, nA_Value, 0x10);
		GET_STACK(short, nB_Value, 0x14);
		GET_STACK(short, nC_Value, 0x12);
		GET_STACK(short, nD_Value, 0x1C);

		CellStruct nDefault = CellStruct::Empty;

		if (pThis->GDIBarracks)
			nDefault = CellStruct{ 1,2 };
		if (pThis->NODBarracks)
			nDefault = CellStruct{ 2,2 };
		if (pThis->YuriBarracks)
			nDefault = CellStruct{ 2,1 };

		CellStruct nFetchedOffset{ nB_Value ,nD_Value };
		nFetchedOffset += pTypeExt->KickOutCell_Offset.Get(nDefault);

		if (nA_Value == nFetchedOffset.X && nC_Value == nFetchedOffset.Y)
		{
			auto nExit = pThis->ExitCoord;
			GET(int, nY_Value, EDX);
			GET(int, nX_Value, EAX);

			R->Stack(0x38, nX_Value + nExit.X);
			R->Stack(0x3C, nY_Value + nExit.Y);
			R->Stack(0x40, nExit.Z);

			return Handled;
		}
	}

	return pThis->GDIBarracks ? CheckGDIOfsset : CheckNODOffset;
}

DEFINE_HOOK(0x44EFD8, BuildingClass_TryGetKickOutCell, 0x6)
{
	GET(BuildingTypeClass*, pThis, EDX);
	enum { CheckOther = 0x44F13B, ReturnAndSetCell = 0x44F12F };

	const auto& pTypeExt = BuildingTypeExt::ExtMap.Find(pThis);

	GET(CellStruct, nCell, EBP);
	GET(TechnoClass*, pTech, ESI);
	GET_STACK(CellStruct*, pCellStructRet, 0x30);

	bool bCheck = false;
	CellStruct nDefault = CellStruct::Empty;

	if (pThis->GDIBarracks)
	{
		bCheck = true;
		nDefault = CellStruct{ 1,2 };
	}

	if (pThis->NODBarracks)
	{
		bCheck = true;
		nDefault = CellStruct{ 2,2 };
	}

	if (pThis->YuriBarracks)
	{
		bCheck = true;
		nDefault = CellStruct{ 2,1 };
	}

	if (bCheck)
	{
		nCell += pTypeExt->KickOutCell_Offset.Get(nDefault);

		if (Map.CoordinatesLegal(nCell))
		{
			if (pTech->IsCellOccupied(Map[nCell], -1, -1, nullptr, false) == Move::OK)
			{
				pCellStructRet = &nCell;
				R->EAX(pCellStructRet);
				R->EDX(&nCell);

				return ReturnAndSetCell;
			}
		}
	}

	return CheckOther;
}
*/

DEFINE_HOOK(0x41A160, AircraftClass_GetDockCell_Debug, 0x6)
{
	GET(AircraftClass* const, pAir, ECX);
	GET_STACK(DWORD, nCaller, 0x0);

	Debug::Log_WithBool(Temp::bDisableDebug, __FUNCTION__" %08x[%s] ,calling address = %x\n", pAir, pAir->Type->ID, nCaller);

	return 0;
}

DEFINE_HOOK(0x456376, BuildingClass_RemoveSpacingAroundArea, 0x6)
{
	GET(BuildingClass* const, pThis, ESI);
	GET(BuildingTypeClass*, pThisType, ECX);
	Debug::Log_WithBool(Temp::bDisableDebug, __FUNCTION__" Executed For [%s] \n", pThis->get_ID());

	if (pThisType->UndeploysInto && pThisType->ResourceGatherer)
		return 0x4563A1;

	return (pThisType->Adjacent == 0) ? 0x4563A1 : 0x45638A;
}

DEFINE_HOOK(0x445A78, BuildingClass_Remove_UpdateSpacing, 0x6)
{
	GET(BuildingClass* const, pThis, ESI);
	GET(BuildingTypeClass*, pThisType, ECX);
	Debug::Log_WithBool(Temp::bDisableDebug, __FUNCTION__" Executed For [%s] \n", pThis->get_ID());

	if (pThisType->UndeploysInto && pThisType->ResourceGatherer)
		return 0x445A9F;

	return (pThisType->Adjacent == 0) ? 0x445A9F : 0x445A8A;
}
/*
namespace Work
{
	bool BuildingClass_458A80(BuildingClass* pThis, CoordStruct* pCoord, int nFacing)
	{
		DynamicVectorClass<std::tuple<TechnoClass*, CoordStruct, bool>> nVectorData;

		auto UnlimboFromVector = [&nVectorData](bool bCheckNerbyLoc = false)
		{
			for (int i = nVectorData.Count - 1; i >= 0; --i)
			{
				auto nDataGet = nVectorData[i];
				auto pVecDataTech = std::get<0>(nDataGet);
				auto nLoc = std::get<1>(nDataGet);
				auto bIsSelected = std::get<2>(nDataGet);

				if (bCheckNerbyLoc)
				{
					auto pTechnoType = pVecDataTech->GetTechnoType();
					auto nCell = CellClass::Coord2Cell(nLoc);
					auto nNearbyLoc = Map.Pathfinding_Find
					(nCell,
					pTechnoType->SpeedType,
					-1, MovementZone::Normal,
					false, 1, 1, false, false,
					false, true,CellStruct::Empty ,
					false, false);
					auto pCell = Map[nNearbyLoc];

					nLoc = pCell->GetCoords();
				}

				++Unsorted::IKnowWhatImDoing;
				bool bSuccess = pVecDataTech->Unlimbo(nLoc, pVecDataTech->PrimaryFacing.current().value256());
				--Unsorted::IKnowWhatImDoing;

				if (!bSuccess)
					pVecDataTech->UnInit();
				else
				{
					pVecDataTech->Scatter(CoordStruct::Empty, true, false);

					if (bIsSelected)
						pVecDataTech->Select();
				}
			}

			nVectorData.SetCapacity(0);
		};

		for (auto pFoundation = pThis->GetFoundationData(false); *pFoundation != CellStruct{ 0x7FFF, 0x7FFF }; ++pFoundation)
		{
			CellStruct nCurrentCellS = CellStruct{ (short)(pCoord->X / 256), (short)(pCoord->Y / 256) } +*pFoundation;
			auto pCell = Map[nCurrentCellS];

			if (!pCell)
				break;

			if (pCell->OverlayTypeIndex != -1)
			{
				pCell->OverlayTypeIndex = -1;
				pCell->Powerup = 0;
				pCell->Rubble = nullptr;
			}

			if (auto  pObject = pCell->FirstObject)
			{
				while (abstract_cast<TechnoClass*>(pObject) && pObject->WhatAmI() != AbstractType::Building)
				{
					auto pTech = abstract_cast<TechnoClass*>(pObject);
					nVectorData.AddUnique(std::make_tuple(pTech, pTech->Location, pTech->IsSelected));
					pTech->Limbo();
					pObject->NextObject;

					if (!pObject)
						break;
				}

				UnlimboFromVector();

			}

			if (!pThis->Unlimbo(*pCoord, nFacing))
			{
				pThis->UnInit();
				UnlimboFromVector();
				return false;
			}
			else
			{
				pThis->QueueMission(Mission::Return, false);
				pThis->DiscoveredBy(pThis->Owner);

					//check Activate state and operator then recheck tech tree !

			}

		}


		UnlimboFromVector(true);
		return true;
	}

}

DEFINE_HOOK(0x458A80, BuildingClass_TryPutAndRemoveObstacles, 0x5)
{
	GET(BuildingClass*, pThis, ECX);
	GET_STACK(CoordStruct*, pCoord, 0x4);
	GET_STACK(int, nFacing, 0x8);

	Debug::Log_WithBool(Temp::bDisableDebug, __FUNCTION__" Executed For [%s] \n", pThis->get_ID());
	R->EAX(Work::BuildingClass_458A80(pThis,pCoord,nFacing));

	return 0x458CB5;
}
*/

/*
DEFINE_HOOK(0x466CE9, BulletClass_AdjustingVelocity_BulletPointer, 0x8)
{
	GET(BulletClass*, pThis, EBP);
	Debug::Log_WithBool(Temp::bDisableDebug, __FUNCTION__" Executed!for[%s] ptr[%x] \n", pThis->Type->get_ID(),pThis);
	return 0;
}

DEFINE_HOOK(0x5B2775, BulletClass_AdjustingVelocity_Ascent, 0xA)
{
	GET_STACK(BulletClass*, pBulletCaller, 0xC0);

	Debug::Log_WithBool(Temp::bDisableDebug, __FUNCTION__" Executed!for[%s] ptr[%x] \n", pBulletCaller->Type->get_ID(), pBulletCaller);

	return 0;
}
*/

/*
namespace Shrapnel
{
	void Exec_(BulletClass* pThis)
	{
		CoordStruct nOwnerCoord = CoordStruct::Empty;
		TechnoClass* pBulletOwner = nullptr;
		AbstractClass* pTargetResult = nullptr;
		HouseClass* pTechOwnerHouse = nullptr;

		auto bAllowShrapnel = [](AbstractClass* pTarget)
		{
			bool bForce = false;
			bool bAllowUnit = true;
			bool bAllowBuilding = false;
			bool bAllowInfantry = true;
			bool bAllowterrain = true;
			bool bShrapnelAir = false;
			bool bSrapnelGround = true;

			{
				if (!bForce)
				{
					if (pTarget)
					{
						if (!pTarget->IsInAir())
						{
							switch (pTarget->WhatAmI())
							{
							case AbstractType::Unit:
							{
								if (!bAllowUnit)
									return false;
							}
							break;
							case AbstractType::Building:
							{
								if (!bAllowBuilding)
									return false;
							}
							break;
							case AbstractType::Infantry:
							{
								if (!bAllowInfantry)
									return false;
							}
							break;
							case AbstractType::Terrain:
							{
								if (!bAllowterrain)
									return false;
							}
							break;
							default:
								return false;
								break;
							}
						}
						else
						{
							if (!bShrapnelAir)
								return false;
						}
					}
					else
					{
						if (!bSrapnelGround)
							return false;
					}
				}
			}

			return true;
		};

		auto nShrapnelCount = [](int &nBaseCount, CoordStruct nFrom, CoordStruct nDest)
		{
			int nShrapMin = 0;
			int nShrapMax = 5;

			if (nBaseCount)
			{
				if (nBaseCount < 0)
				{
					auto nDistance = nDest - nFrom;
					auto nDistanceMag = (int)nDistance.Magnitude()* 0.00390625;

					nBaseCount = -nBaseCount - nDistanceMag ; //optimized /256

					if (nShrapMin && nBaseCount < nShrapMin)
						nBaseCount = nShrapMin;
					else if (nShrapMax && nBaseCount > nShrapMax)
						nBaseCount = nShrapMax;

					if (nBaseCount <= 0)
						return;
				}
			}
			else
			{
				nBaseCount = -1;
			}
		};

		if (pThis->Owner)
		{
			pBulletOwner = pThis->Owner;
			pTechOwnerHouse = pBulletOwner->Owner;
			nOwnerCoord = pBulletOwner->Location;
		}

		auto pThisTarget = pThis->Target;
		auto pBulletCell = pThis->GetCell();
		auto pBulletHeight = pThis->GetHeight();
		auto nBulletCoord = pThis->GetCoords();

		if (pBulletHeight <= 256 && pThisTarget)
		{
			//first phase deciding target!
			auto nTargetCoord = pThisTarget->GetCoords();
			auto nDistance = nTargetCoord - nBulletCoord;
			auto nDistanceMagnitude = nDistance.Magnitude();

			if ((int)nDistanceMagnitude <= 256)
			{
				if (pBulletCell->ContainsBridge() || nTargetCoord.Z + 672 > pBulletHeight)
					pTargetResult = pBulletCell->FirstObject;

				if (pBulletCell->ContainsBridge() || pBulletCell->GetCoords().Z + 672 > pBulletHeight)
					pTargetResult = pBulletCell->AltObject;

				bool bShrapInAir = true;
				bool bVersesAlled = true; //put verses check here !

				if (bShrapInAir)
				{
					//CellStruct nBuffer;
					for (auto const& pFootTarget : *FootClass::Array())
					{
						if (pFootTarget->IsInAir())
						{
							auto nFootMapCoords = pFootTarget->GetMapCoords();
							if (nFootMapCoords == pBulletCell->MapCoords && bVersesAlled)
							{
								pTargetResult = pFootTarget;
								break;
							}
						}
					}
				}

				pThisTarget = pTargetResult;

				if (!bAllowShrapnel(pThisTarget))
					return;
			}
		}


		DynamicVectorClass<AbstractClass*> pAbsTargetVec;
		DynamicVectorClass<TerrainClass*> pTerrainTargetVec;
		DynamicVectorClass<CellClass*> pCellTargetVec;

		auto pShrapnelWeapon = pThis->Type->ShrapnelWeapon;
		auto nShrapnelCountResult = pThis->Type->ShrapnelCount;
		nShrapnelCount(nShrapnelCountResult, nBulletCoord, nOwnerCoord);
		auto nWeaponrange = pShrapnelWeapon->Range * 0.00390625; //optimized /256

		//bool bShrapOnTerrain = true;

		//if (pShrapnelWeapon->Projectile->AG)
		{
			nBulletCoord = pThis->GetCoords();

			// get target on range
			CellRangeIterator<CellClass>{}(CellClass::Coord2Cell(nBulletCoord), nWeaponrange, [&bAllowShrapnel, &pAbsTargetVec, &pTerrainTargetVec, &pCellTargetVec, &nShrapnelCountResult, pShrapnelWeapon, pBulletOwner, &nBulletCoord, pThisTarget, pTechOwnerHouse](CellClass* pCell) -> bool
				{
					bool bSomeBool = false;
					auto pCurrentObject = pCell->GetSomeObject(Point2D{1,1});
					bool bVersesEligible = true;

					auto nMinRange = pShrapnelWeapon->MinimumRange;

					//only iterate enough , so it wont fill memory with unnessesary garbage
					{
						if (!nMinRange || (pCell->GetCoords() - nBulletCoord).Magnitude() >= nMinRange)
						{
							if (!pCurrentObject)
							{
								pAbsTargetVec.AddUnique(pCell);
							}
							else
							{
								bool bSameOwner = pBulletOwner ? pCurrentObject == pBulletOwner : false;
								if (pCurrentObject != pThisTarget && !bSameOwner && bAllowShrapnel(pCurrentObject))
								{
									pAbsTargetVec.AddUnique(pCurrentObject);
								}
							}

							return true;
						}
					}
					/*
					if (bSomeBool || !pCurrentObject)
					{
						if (pAbsTargetVec.Count + pTerrainTargetVec.Count < nShrapnelCountResult)
						{
							auto nMinRange = pShrapnelWeapon->MinimumRange;
							if (!nMinRange || (pCell->GetCoords() - nBulletCoord).Magnitude() >= nMinRange)
							{
								pCellTargetVec.AddUnique(pCell);

							}
						}
						return true;
					}

					if (bVersesEligible)
					{
						if (!pCurrentObject->GetTechnoType() && !bSomeBool && pCurrentObject->WhatAmI() == AbstractType::Terrain && pCurrentObject != pThisTarget)
						{
							pTerrainTargetVec.AddUnique(specific_cast<TerrainClass*>(pCurrentObject));

							if (pAbsTargetVec.Count + pTerrainTargetVec.Count == nShrapnelCountResult)
							{
								bSomeBool = true;
							}

							return true;
						}

						bool bAllyShrap = true;
						if (pTechOwnerHouse && pTechOwnerHouse->IsAlliedWith(pCurrentObject))
						{
							if (bAllyShrap)
							{
								pAbsTargetVec.AddUnique(pCurrentObject);
							}

							return true;
						}

						if (pAbsTargetVec.Count == nShrapnelCountResult)
						{
							bSomeBool = true;
						}

					}

					return true;
				});

			Debug::Log(__FUNCTION__" Result AbsVec[%d] TrrVec[%d] CellVec[%d] count[%d] \n", pAbsTargetVec.Count, pTerrainTargetVec.Count, pCellTargetVec.Count, nShrapnelCountResult);

			if (pAbsTargetVec.Count > 0)
			{
				for (int i = 0; i < nShrapnelCountResult; ++i)
				{
					auto nIndex = ScenarioGlobal->Random(pAbsTargetVec.Count - 1);
					if (auto pnTargat = pAbsTargetVec[nIndex])
					{
						auto nCoord = pnTargat->GetCoords();
						Shrapnel::CreateBullet(pShrapnelWeapon, nCoord, pnTargat, pBulletOwner);
					}

				}
			}
		}


		pAbsTargetVec.Clear();
		pTerrainTargetVec.Clear();
		pCellTargetVec.Clear();
	}
};
*/


/*
DEFINE_HOOK(0x6B7BDD, SpawnManagerClass_Kamikaze_AI_Suicide, 0x6)
{
	GET(AircraftTypeClass*, pSpawned, ECX);
	bool bMissileSpawn = R->DL();

	R->DL(bMissileSpawn || pSpawned-)
}



DEFINE_HOOK(0x451A36, BuildingClass_AnimLogic_AI_AdditionalCheck, 0x6)
{
	GET(BuildingClass*, pThis, ESI);
	//GET_BASE(bool, bIsGarrisoned, STACK_OFFS(0x34, 0x10));

	auto cIsGarrison = pThis->Occupants.Count > 0 ? "YES" : "NO";
	Debug::Log_Masselist(__FUNCTION__" Executed For [%s] IsGarrison [%s]", pThis->get_ID(), cIsGarrison);

	bool bIsPowerDown = !pThis->IsPowerOnline() || !pThis->StuffEnabled;

	return bIsPowerDown ? 0x451A4E : 0x451A70;
}

DEFINE_HOOK(0x458333, BuildingClass_Garrison_Anim_AI_test, 0x8)
{
	GET(BuildingClass*, pThis ,ECX);
	bool bIsGarrisoned = pThis->Occupants.Count > 0;
	auto cIsGarrison = bIsGarrisoned ? "YES" : "NO";

	auto pBuildingExt = BuildingTypeExt::ExtMap.Find(pThis->Type);
	auto nReplace = Replacer::ReplaceOccupantAnimNameTo(pBuildingExt->GarrisonAnim_idle, bIsGarrisoned, pThis->Occupants[0],"None");

	Debug::Log_Masselist(__FUNCTION__" Executed For [%s] IsGarrison [%s] idleNameReplaced[%s]", pThis->get_ID(), cIsGarrison, nReplace);

	return 0;
}*/

/*
DEFINE_HOOK(0x6F9E50, TechnoClass_AI_GeneralHijacker, 0x5)
{
	GET(TechnoClass*, pThis, ECX);

	//TempPromotion::UpgradeVeterancyOnceTime(pThis);

	return 0;
}


// this were ideal place to hook actually
// but the Infantry is deleted by Ares which can cause problem when fecthcing needed stuffs
DEFINE_HOOK(0x5206A1, InfantryClass_UpdateVehicleThief_Hijack_Finalize, 0x5)
{
	GET(InfantryClass*, pThis, ESI);

	if (auto pTargetUnit = specific_cast<UnitClass*>(pThis->Target))
	{

	}

	return 0;
}


// Death Anim hardcoded
// Dehardcode voxcel anim bounce properties
// Custom Droppod Stuffs

// Chrono Sparkle
// Unit = 736240
// Infantry =51BB07
// Building = 4403DA
// Aircraft = 414C17

// Barrel ? wtf ?
// 48A2E9

namespace BounceClass_
{
	void __fastcall InitB(BounceClass* pBounce, CoordStruct* pCoord, double elasticity, double fval1, double fval2, Vector3D<float> *vect3, double angular_velocity)
	{

		Debug::Log(__FUNCTION__" Bounce [%x] Called Gravity [%lf] !\n", pBounce,fval1);
		pBounce->Initialize_B(pCoord, elasticity, fval1, fval2, vect3, angular_velocity);
	}

};

DEFINE_POINTER_CALL(0x74981F, BounceClass_::InitB);


DEFINE_HOOK(0x709C04, TechnoClass_DrawPip_Pipscale_Not2, 0x4)
{
	GET(TechnoClass*, pTechno, EBP);
	GET_STACK(SHPStruct*, pShape, 0x50);

	Debug::Log(__FUNCTION__" For [%s] Shape[%x]\n", pTechno->get_ID(), pShape);

	R->EAX(pShape);
	return 0x709C08;
}

DEFINE_HOOK(0x709CE8, TechnoClass_DrawPip_Pipscale_Not2Building, 0x4)
{
	GET(TechnoClass*, pTechno, EBP);
	GET_STACK(SHPStruct*, pShape, 0x48);

	Debug::Log(__FUNCTION__" For [%s] Shape[%x]\n", pTechno->get_ID(), pShape);

	R->EDX(pShape);
	return 0x709CEC;
}

DEFINE_HOOK(0x709CE8, TechnoClass_DrawPip_Pipscale_5, 0x4)
{
	GET(TechnoClass*, pTechno, EBP);
	GET_STACK(SHPStruct*, pShape, 0x48);

	Debug::Log(__FUNCTION__" For [%s] Shape[%x]\n", pTechno->get_ID(), pShape);

	R->EDX(pShape);
	return 0x70A12B;
}

DEFINE_HOOK(0x70A1C6, TechnoClass_DrawPip_Pipscale_5_Overload, 0x4)
{
	GET(TechnoClass*, pTechno, EBP);
	GET_STACK(SHPStruct*, pShape, 0x48);

	Debug::Log(__FUNCTION__" For [%s] Shape[%x]\n", pTechno->get_ID(), pShape);

	R->ECX(pShape);
	return 0x70A1CA;
}


DEFINE_HOOK(0x70A4B9, TechnoClass_DrawPip_Pipscale_1, 0x4)
{
	GET(TechnoClass*, pTechno, EBP);
	GET_STACK(SHPStruct*, pShape, 0x58);

	Debug::Log(__FUNCTION__" For [%s] Shape[%x]\n", pTechno->get_ID(), pShape);

	R->EAX(pShape);
	return 0x70A4BD;
}

DEFINE_HOOK(0x70A3CE, TechnoClass_DrawPip_Pipscale_1b, 0x4)
{
	GET(TechnoClass*, pTechno, EBP);
	GET_STACK(SHPStruct*, pShape, 0x50);

	Debug::Log(__FUNCTION__" For [%s] Shape[%x]\n", pTechno->get_ID(), pShape);

	R->EDX(pShape);
	return 0x70A3D2;
}

// Logged
DEFINE_HOOK(0x70A31D, TechnoClass_DrawPip_Pipscale_2, 0x7)
{
	GET(TechnoClass*, pTechno, EBP);
	GET_STACK(SHPStruct*, pShape, 0x50);

	Debug::Log(__FUNCTION__" For [%s] Shape[0x%x]\n", pTechno->get_ID(), pShape);

	R->EDX(pShape);
	return 0;
	//return 0x70A31D;
}

DEFINE_HOOK(0x4179AA, AircraftClass_AssumeTaskComplete_AirportBound, 0x6)
{
	GET(AircraftClass*, pThis, ESI);
	GET(ObjectClass*, pThat, EAX);

	R->EDI(pThat);

	return pThis->Destination == pThat ? 0x4179DD : 0x0;
}

/*
DEFINE_HOOK(0x4FC1FF, HouseClass_AcceptDefeat_CleanShroudFog, 0x6)
{
	MapClass::Instance->Reveal(R->ESI<HouseClass*>());
	return 0x4FC214;
}*/



//DEFINE_HOOK(0x70D219 ,TechnoClass_IsRadarVisible, 0x6){}
/*
DEFINE_HOOK(0x4242E1, AnimClass_AI_TrailerAnim, 0x5)
{
	GET(AnimClass*, pThis, ESI);

	auto pExt = AnimExt::ExtMap.Find(pThis);
	auto pAnimTypeExt = AnimTypeExt::ExtMap.Find(pThis->Type);

	if (pExt->TrailerCounter > 0)
		pExt->TrailerCounter--;
	else
	{
		if (pExt->TrailerCounter < 0)
		{
			auto nNum = pThis->Type->NumParticles;
			if (nNum > 0)
			{
				if (pAnimTypeExt->ParticleChance.isset() ? (ScenarioClass::Instance->Random(99) < abs(pAnimTypeExt->ParticleChance.Get())) : true)
					pExt->TrailerCounter = nNum;
			}
			else
			{
				pExt->TrailerCounter = INT_MAX;
			}
		}
		else
		{
			auto nCoord = pThis->GetCenterCoord_();
			if (auto pAnim = GameCreate<AnimClass>(pThis->Type->TrailerAnim, nCoord))
			{
				Debug::Log(__FUNCTION__" Called! \n");
				pAnim->Owner = pThis->Owner;
			}

			pExt->TrailerCounter = -1;
		}
	}

	return 0x424322;
}




DEFINE_HOOK(0x54DCD2, JumpetLocomotionClass_DrawMatrix, 0x8)
{
	GET(JumpjetLocomotionClass*, pLoco, ESI);

	R->ECX(pLoco->Owner);

	return (pLoco->Owner->GetTechnoType()->TiltCrashJumpjet) ? 0x54DCE8 : (pLoco->Is_Ok_To_End() ? 0x54DCE8 : 0x54DF13);
}

DEFINE_HOOK(0x702819, TechnoClass_ReceivedDamage_Uncloak, 0xA)
{
	GET(TechnoClass*, pThis, ESI);

	bool bDamagetDecloak = true;

	if (bDamagetDecloak)
		pThis->Reveal();

	return 0x702823;
}

DEFINE_HOOK(0x5F6BFE, ObjectClass_GetYSort_Adjust, 0x7)
{
	GET(ObjectClass*, pThis, ESI);
	GET(int, nOffset, ECX);

	auto YOffs = 0;
	if (pThis && abstract_cast<TechnoClass*>(pThis))
		R->ECX(nOffset + YOffs);

	return 0;
}

DEFINE_HOOK(0x54D0D0, JumpjetLocomotionClass_IsMovingNow, 0x7)
{
	GET_STACK(JumpjetLocomotionClass* const, pLoco, 0x4);


	if (pLoco->Owner->GetHeight() <= 0)
	{
		R->EAX(pLoco->IsMoving);
	}
	else
	{
		auto nState = pLoco->NextState;
		if (nState == 3 || nState == 1 || nState == 4)
			R->EAX(true);
		else
			R->EAX(false);
	}

	R->EAX(pLoco->NextState && pLoco->NextState != 2);

	return 0x54D0E2;
}

DEFINE_HOOK(0x7102F9, FootClass_ImbueLocomotor_SetDestination, 0x5)
{
	GET(TechnoClass*, pThisVictim, ESI);
	GET(TechnoClass*, pAttakcer, EBX);

	auto pFoot = abstract_cast<FootClass*>(pThisVictim);
	pFoot->LocomotorSource = abstract_cast<FootClass*>(pAttakcer);
	bool UseLastDestinationInstead = true;
	pFoot->Deselect();

	if (!pAttakcer->GetTechnoType()->Insignificant || pFoot->WhatAmI() == AbstractType::Aircraft)
	{
		AbstractClass* pDest = pAttakcer;
		if (UseLastDestinationInstead)
			pDest = MapClass::Instance->TryGetCellAt(pFoot->LastMapCoords);

		pThisVictim->SetDestination(pDest, true);

	}
	else
	{
		//check if it allow to be scatter !
		pThisVictim->Scatter(CoordStruct::Empty, 1, 0);
	}

	return 0x71033A;
}

//need ext
DEFINE_HOOK(0x6FCA9B, TechnoClass_GetFireError_Parasite, 0x9)
{
	GET(ParasiteClass*, pParasite, ECX);
	GET(TechnoClass*, pTech, EDI);

	return pTech->GetFireError() ==FireError::MUST_DEPLOY ?
}

DEFINE_HOOK(0x419CC1, AircraftClass_Mi_Enter_AiportBound, 0x6)
{
	GET(TechnoClass*, pThat, EDI);
	GET(TechnoClass*, pThat_, EAX);

	if (pThat != pThat_)
	{
		GET(TechnoClass*, pThis, EDI);
		if (pThat && pThat->WhatAmI() == AbstractType::Building)
		{
			pThis->SendToFirstLink(RadioCommand::NotifyUnlink);
			pThis->SendCommand(RadioCommand::RequestLink, pThat);
			pThis->SetDestination(pThat, 1);
			return 0x419CFF;
		}
		if (pThis->SendCommand(RadioCommand::QueryCanEnter, pThat_) == RadioCommand::AnswerPositive)
		{
			pThis->SendToFirstLink(RadioCommand::NotifyUnlink);
			pThis->SendToFirstLink(RadioCommand::RequestUntether);
			pThis->SendCommand(RadioCommand::RequestLink, pThat_);
			pThis->SetDestination(pThat_, 1);
		}
	}

	return 0x419D0B;
}



DEFINE_HOOK(0x416748, AircraftClass_NormalAircraftsMove_AirportBound, 0x5)
{
	GET(AircraftClass*, pThis, ESI);

	return pThis->Type->AirportBound ? 0x41675D : 0x0;
}

DEFINE_HOOK(0x4DEFF1, FootClass_FindNearestBuildingOfType_OverFlowFix, 0x8)
{
	GET(BuildingClass*, pThat, EDI);
	GET(TechnoClass*, pThis, ESI);
	GET(int*, pDistance, EBX);

	auto nDistance = pThis ? pThis->DistanceFrom(pThat) : 0;

	if (*pDistance == -1 || nDistance < *pDistance || pThat->IsPrimaryFactory)
	{
		R->Stack(0x14, pThat);
		pDistance = &nDistance;
	}
	return 0x4DF014;
}

#include <FlyLocomotionClass.h>

DEFINE_HOOK(0x4CED12, FlyLocomotionClass_UpdateLanding_Finish, 0x8)
{
	GET(FlyLocomotionClass* const, pThis, ESI);
	GET(int, nHeight, EDI);

	if (nHeight <= 104)
		return 0;

	pThis->IsTakingOff = 0;
	R->Stack(0x13, 1);
	return 0x4CED2D;
}

DEFINE_HOOK(0x41B748, FlyLocomotionClass_Land_Altitude, 0x5)
{
	GET_STACK(FlyLocomotionClass* const, pThis, 0x4);

	int nLandAttitude = 100;

	if (auto pAir = specific_cast<AircraftClass*>(pThis->LinkedTo))
	{
		if (pAir->Spawned && pAir->HasAnyLink())
		{
			if (auto pSpawnOwner = pAir->SpawnOwner)
			{
				for (auto pLink : pAir->RadioLinks)
				{
					if (pLink)
					{
						if (pLink->WhatAmI() != AbstractType::Building)
						{
							if (pSpawnOwner == pLink)
							{
								nLandAttitude = pLink->GetHeight(); //spawned unit cannot land on jumjet spawner fix
							}
						}
					}
				}
			}
		}
	}

	R->EAX(nLandAttitude);
	return 0x41B74D;
}


DEFINE_HOOK(0x437C29, Buffer_To_RLE_Surface_Bound_Fix, 0x7)
{
	GET_STACK(int, nX_Value, 0x30);
	GET_STACK(int, nY_Value, 0x58);
	GET(int, nX_Comparison, EAX);
	GET(int, nY_Comparison, EDX);

	if (nX_Comparison >= nX_Value || nX_Value < 0)
		nX_Comparison = 0;

	if (nY_Comparison >= nY_Value || nY_Comparison < 0)
		nY_Comparison = 0;

	R->EAX(R->ECX<Surface*>()->Lock(nX_Comparison, nY_Comparison));
	return 0x437C30;
}

DEFINE_HOOK(0x44EFD8, BuildingClass_TryGetKickOutCell, 0x6)
{

	GET(TechnoClass*, pProduced, ESI);
	//GET(BuildingTypeClass*, pThisType, EDX);
	GET(CellStruct, nCell, EBP);

	int nCellOffsetX = 1;
	int nCellOffsetY = 1;

	nCell.X += (short)nCellOffsetX;
	nCell.Y += (short)nCellOffsetY;

	if (!MapClass::Instance->CoordinatesLegal(nCell))
		return 0x44F13B;

	if (auto pCell = MapClass::Instance->TryGetCellAt(nCell))
		if (pProduced->IsCellOccupied(pCell, -1, -1, nullptr, false) != Move::OK)
			return 0x44F13B;

	return 0x44F037;
}



DEFINE_HOOK(0x54D600, JumpjetLocomotionClass_Sub54D0F0_CrashFix, 0x6)
{
	GET(FootClass*, pLinked, ECX);
	GET_STACK(CoordStruct, nCoord, 0x44);

	if (pLinked->WhatAmI() != AbstractType::Aircraft || (pLinked->Location.X == nCoord.X && pLinked->Location.Y == nCoord.Y))
	{
		pLinked->SetLocation(nCoord);
	}
	else
	{
		bool v3 = pLinked->IsOnMap;
		pLinked->IsOnMap = 1;
		pLinked->SetLocation(nCoord);
		pLinked->IsOnMap = v3;
	}

	return 0x54D60D;
}

DEFINE_HOOK(0x707B95, TechnoClass_PointerExpired_LocoSource, 0x7)
{
	GET(TechnoClass*, pAttacker, ECX);
	GET(TechnoClass*, pVictim, ESI);

	if (pVictim && abstract_cast<FootClass*>(pVictim))
	{
		if (pAttacker->LocomotorTarget == pVictim)
		{
			pAttacker->LocomotorImblued(true);
			pAttacker->LocomotorTarget = nullptr;
			pVictim->LocomotorSource = nullptr;
		}

		if (pVictim->GetHeight() > 0)
		{
			pVictim->IsCrashing = 1;
			pVictim->IsBeingManipulated = 1;
			pVictim->BeingManipulatedBy = pVictim;
			pVictim->ChronoWarpedByHouse = pAttacker->Owner;
			pVictim->Stun();
			pVictim->SetDestination(nullptr, 1);
		}
	}

	specific_cast<UnitClass*>(pVictim)->unknown_int_6D4 = 1;
	pVictim->SetDestination(nullptr, 1);
	pVictim->LocomotorSource = nullptr;
	return 0x707BB2;
}


DEFINE_HOOK(0x6FDD6F, TechnoClass_Fire_RandomizedWeapon, 0x8)
{
	GET(WeaponTypeClass*, pWeapon, EAX);

	if(!pWeapon)
		return 0x6FDE03;

	R->EBX(pWeapon);
	R->Stack(0x40, pWeapon);

	return R->EBX<WeaponTypeClass*>() ? 0x6FDD7D : 0x6FDE03;
}*/
// hmm , dunno how to implement the nested one better
// will put this here for later
/* Crash a lot  , mostlikely wrong size
DEFINE_HOOK(0x6FDD6F, TechnoClass_FireAt_NestedWeapon_pt1, 0x0)
{
	GET(WeaponStruct*, pWeaponStruct, EAX);

	if (auto pWeapon = pWeaponStruct->WeaponType)
	{
		R->EBX(pWeapon);
		return 0x6FDD73;
	}

	return 0x6FDE03;
}

DEFINE_HOOK(0x6FA467, TechnoClass_AI_AttackAllies, 0x5)
{
	return R->ESI<TechnoClass*>()->Type()->AttackFriendlies ? 0x6FA472 : 0x0;
}

DEFINE_HOOK(0x6FA7B2, TechnoClass_Update_ExtraSelfHeal, 0x7)
{
	GET(TechnoClass* const, pThis, ESI);

	return TechnoTypeExt::ExtMap.Find(pThis->Type())->NoExtraSelfHeal.Get() ? 0x6FA941 : 0x0;
}

DEFINE_HOOK(0x42CC48, AStarClass_PathfindingfailureLog_Disable, 0xA)
{
	auto pRules = RulesExt::Global();
	return pRules->SkipPathFindingFailureLog.Get() ? 0x42CC6D : 0x0;
}

DEFINE_HOOK(0x42CBC9, AstarClass_FindPath_DebugLog_Disable, 0x6)
{
	auto pRules = RulesExt::Global();
	return pRules->SkipFindPathFailureLog.Get() ? 0x42CBE6 : 0x0;
}

DEFINE_HOOK(0x6EBEDB, TeamClass_MoveToFocus_BalloonHover, 0xA)
{
	GET(FootClass* const, pThis, ESI);

	auto const Type = pThis->GetTechnoType();

	return Type->WhatAmI() == AbstractType::UnitType && (Type->BalloonHover || Type->JumpJet) && Type->DeployToLand ?
		0x6EBEEF : 0x6EBEFF;
}

DEFINE_HOOK(0x6EBE69, TeamClass_MoveToFocus_SetDestination, 0xA)
{
	GET(FootClass* const, pThis, ESI);

	auto const Type = pThis->GetTechnoType();

	return  Type->WhatAmI() == AbstractType::UnitType && (Type->BalloonHover || Type->JumpJet) && Type->DeployToLand ?
		0x6EBE9C : 0x6EBE82;
}

DEFINE_HOOK(0x6EBB86, TeamClass_MoveToFocus_IsInStray, 0x6)
{
	GET(FootClass* const, pThis, ESI);
	GET(TeamClass* const, pThisteam, EBP);

	auto Range = pThis->DistanceFrom(pThisteam->SpawnCell);

	if (pThis->GetHeight() > 0 && pThis->WhatAmI() == AbstractType::Unit)
		Range = Game::F2I(pThis->GetCoordPoint().DistanceFromAutoMethod(pThisteam->Focus->GetCoordPoint()));

	R->EAX(Range);
	return 0x6EBB91;
}

DEFINE_HOOK(0x70A504, TechnoClass_DrawPipScalePips_NoRepairHeal, 0x6)
{
	GET(TechnoClass*, pThis, EBP);

	return TechnoTypeExt::ExtMap.Find(pThis->Type())->NoExtraSelfHeal.Get() ? 0x70A6C0 : 0x0;
}

DEFINE_HOOK(0x73C6F5, UnitClass_DrawAsSHP_StandFrame, 0x9)
{
	GET(UnitTypeClass*, pThis, ECX);
	GET(UnitClass*, pThisU, EBP);
	GET(int, nFrameNum, EBX);

	auto nFrame = pThis->StartStandFrame + pThis->StandingFrames * nFrameNum;

	if (pThis->IdleRate > 0)
		nFrame += pThisU->WalkedFramesSoFar % pThis->StandingFrames;

	R->EBX(nFrame);
	return 0x73C725;
}

//new






DEFINE_HOOK(0x6B72FE, SpawnManagerClass_Update_CrashFix, 0x9)
{
	GET(SpawnManagerClass* const, pThis, ESI);

	return pThis->Owner && pThis->Owner->WhatAmI() == AbstractType::Building ? 0x6B735C : 0x0;
}

*/


/*
DEFINE_HOOK(0x6F9039, TechnoClass_GreatesThreat_Deharcode_RepairWeaponGuardRange, 0x5)
{
	GET(TechnoClass*, pThis, ESI);

	int nGuardRange = pThis->GetTechnoType()->GuardRange;

	if (!pThis->GetTechnoType()->IsGattling)
	{
		auto nPrimaryWStruct = pThis->GetTechnoType()->GetWeapon(0, pThis->Veterancy.IsElite());
		if (WeaponStruct::IsValid(&nPrimaryWStruct))
		{
			if (auto pPrimaryWeapon = nPrimaryWStruct.WeaponType)
			{
				auto nPrimaryRange = pPrimaryWeapon->Range;
				if (nPrimaryRange > 0 && nPrimaryRange > nGuardRange)
				{
					nGuardRange = nPrimaryRange;
				}
			}

		}

		auto nSecondaryWStruct = pThis->GetTechnoType()->GetWeapon(1, pThis->Veterancy.IsElite());
		if (WeaponStruct::IsValid(&nSecondaryWStruct))
		{
			if (auto pSecondryWeapon = nSecondaryWStruct.WeaponType)
			{
				auto nSecondaryRange = pSecondryWeapon->Range;
				if (nSecondaryRange > 0 && nSecondaryRange > nGuardRange)
				{
					nGuardRange = nSecondaryRange;
				}
			}
		}

	}
	else
	{
		auto nWeaponStruct = pThis->GetTechnoType()->GetWeapon(pThis->CurrentWeaponNumber, pThis->Veterancy.IsElite());

		if (WeaponStruct::IsValid(&nWeaponStruct))
		{
			if (auto pWeapon = nWeaponStruct.WeaponType)
			{
				auto nGattRange = pWeapon->Range;
				if (nGattRange > 0 && nGattRange > nGuardRange)
				{
					nGuardRange = nGattRange;
				}
			}
		}
	}

	R->EDI(nGuardRange);
	return 0x6F903E;
}
*/