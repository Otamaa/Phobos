//std::vector<std::vector<int>> number_list::recycling;

//DEFINE_HOOK(0x74D80A, veins_forceActivate, 0x6)
//{
	//	GET(VeinholeMonsterClass*, pThis, ESI);
//	R->AL(true);
	//	Debug::Log_Masselist(__FUNCTION__"[%x]Executed !", pThis);
//	return 0x74D810;
//}

/* hmm , empty shadowdrawstruct ?

DEFINE_HOOK(0x756860, VoxelLibraryClass_Draw_Shadow, 0x5)
{
	//GET(VoxLib*, pLib, ECX);
	//GET_STACK(VoxelShadowDrawStruct*, pShadowDrawer, 0x4);

	//Debug::Log(__FUNCTION__" : pLib [%p] pShadowDrawerSurface[%p] pShadowDraw [%p] ! \n", pLib, pShadowDrawer->SurfacePtr, pShadowDrawer);
	return 0x0;
}*/

/* wrong techno pointer wtf !
DEFINE_HOOK(0x707280, TechnoClass_Voxel_Shadow, 0x6)
{
	GET(TechnoClass*, pThis, EBX);
	GET_STACK(VoxelShadowDrawStruct*, pShadowDrawer, 0x114);
	GET(int, nIdx, EBP);

	Debug::Log(__FUNCTION__": pThis = %p[%s] , at %d,%d , VoxelIndex = %d , pVox = %p\n", pThis, pThis->get_ID(), pThis->GetCoords().X, pThis->GetCoords().Y, nIdx, pShadowDrawer);
	return 0x0;
}
*/
/*
DEFINE_HOOK(0x6D70BC, TacticalClass_Overlay_CheckFog2, 0xA)
{
	enum { Draw = 0x6D70C6, Skip = 0x6D71A4 };

	GET(CellClass*, pCell, EAX);

	return (pCell->OverlayTypeIndex == -1 || pCell->IsFogged()) ? Skip : Draw;
}

DEFINE_HOOK(0x6D6EDA, TacticalClass_Overlay_CheckFog1, 0xA)
{
	enum { Draw = 0x6D6EE4, Skip = 0x6D7006 };

	GET(CellClass*, pCell, EAX);

	return (pCell->OverlayTypeIndex == -1 || pCell->IsFogged()) ? Skip : Draw;
}

DEFINE_HOOK(0x48049E, CellClass_DrawTileAndSmudge_CheckFog, 0x6)
{
	enum { Draw = 0x4804A4, Skip = 0x4804FB };

	GET(CellClass*, pCell, ESI);

	return (pCell->SmudgeTypeIndex == -1 || pCell->IsFogged()) ? Skip : Draw;
}*/


/* not really working
* // combined with OnmiFire= causing jumpjet spinning uncontrolled
namespace JJHelper
{
	DirStruct RadiansToDir(double radians)
	{
		short d = (short)(radians / Math::BINARY_ANGLE_MAGIC);
		return DirStruct(d);
	}

	DirStruct CoordToDir(CoordStruct sourcePos, CoordStruct targetPos)
	{
		// get angle
		double angle = Math::atan2(double(sourcePos.Y - targetPos.Y), double(targetPos.X - sourcePos.X));
		// Magic form tomsons26
		angle -= Math::deg2rad(90);
		return RadiansToDir(angle);
	}

	short GetValFromBits(DirStruct& dir, size_t Bits = 16, int offset = 0)
	{
		//Bits = Bits > 16 ? 16 : Bits;
		Bits = Bits < 0 ? 0 : Bits;

		return (short)(TranslateFixedPoint(16, Bits, dir.value(), offset));
	}

	int Dir2FacingIndex(DirStruct& dir, int facing)
	{
		auto nfacing = std::sqrt(facing);
		auto bits = (size_t)std::round(nfacing);
		double face = GetValFromBits(dir, bits);
		double x = (face / (1 << (int)bits)) * facing;
		int index = (int)std::round((x));
		return index;
	}

	void TurnToTarget(UnitClass* pThis)
	{
		auto const pExt = TechnoExt::ExtMap.Find(pThis);
		auto const pTypeExt = TechnoTypeExt::ExtMap.Find(pThis->GetTechnoType());

		bool bTurnToTarget = true;//pTypeExt->Jumpjet_FacingToTarget.Get();

		if (bTurnToTarget && pThis->IsInAir() && !pExt->JumpjetNeedToFace)
		{
			if (auto pTarget = pThis->Target)
			{
				{
					auto const nToDir = pThis->GetDirectionOverObject(pTarget);
					auto const nCurDir = pThis->PrimaryFacing.current();
					if (nToDir != nCurDir)
					{
						pExt->JumpjetNeedToFace = true;
						pExt->JumpjetTargetDir = nToDir;
					}
				}
			}
		}
	}

	void AI_TurnToTarget(TechnoClass* pThis)
	{
		if (auto const pUnit = specific_cast<UnitClass*>(pThis))
		{
			auto const pExt = TechnoExt::ExtMap.Find(pThis);

			if (pExt->JumpjetNeedToFace)
			{
				auto const pLoco = pUnit->Locomotor.get();
				if (!pLoco->Is_Moving_Now() && !pUnit->PrimaryFacing.in_motion())
					pLoco->Do_Turn(pExt->JumpjetTargetDir);

				pExt->JumpjetNeedToFace = false;
			}
		}
	}
}


DEFINE_HOOK(0x736E40, UnitClass_FiringAI_JumpjetTurning, 0x6)
{
	GET(FireError, nFireError, EAX);
	GET(UnitClass*, pThis, ESI);

	R->EBP(nFireError);

	if (pThis->Type->JumpJet)
	{
		if (nFireError == FireError::OK
			|| nFireError == FireError::REARM
			|| nFireError == FireError::FACING
			|| nFireError == FireError::ROTATING)
		{
			if (!pThis->Locomotor.get()->Is_Moving_Now())
				JJHelper::TurnToTarget(pThis);
		}
	}

	return  (nFireError == FireError::OK) ? 0x736E4B : 0x736E46;
}*/

/*
DEFINE_HOOK(0x42465D, AnimCalss_AI_Midpoint_CheckAvail, 0x8)
{
	GET(AnimClass*, pThis, ESI);

	if (!pThis->Type)
	{
		Debug::Log("Anim[0x%x] with corrupted Type attempting to do MiddlePoint_AI !\n", pThis);
		if (!pThis->IsBuildingAnim)
		{
			pThis->RemainingIterations = 0;
			pThis->UnInit();
			pThis->AnnounceExpiredPointer();
			GameDelete(pThis);

			return 0x424B42;
		}
	}

	return  0x0;
}*/

/*
DEFINE_HOOK(0x42468C, AnimClass_AI_PipngPong_crash, 0x6)
{
	GET(AnimClass*, pThis, ESI);

	R->EAX(pThis->Type);
	R->CL(pThis->Type && pThis->Type->PingPong);

	return 0x424698;
}

// Clean fog !
DEFINE_HOOK(0x577EBF, MapClass_Reveal, 0x6)
{
	GET(CellClass*, pCell, EAX);

	pCell->AltFlags |= AltCellFlags::NoFog | AltCellFlags::Mapped;
	pCell->Flags |= CellFlags::Revealed;
	pCell->ShroudCounter = 0;
	pCell->GapsCoveringThisCell = 0;
	pCell->CleanFog();

	return 0x577EE9;
}*/

/*!

*/
/*
DEFINE_HOOK(0x6F9E50, TechnoClass_AI_RepairSpawn, 0x5)
{
	GET(TechnoClass*, pThis, ECX);

	// [Vanilla Bug] Fix spawned AircraftTypes's targeting (enable Repair Drones) #222

	if (const auto pTargetTech = abstract_cast<TechnoClass*>(pThis->Target))
	{
		if (const auto pSpawnManager = pThis->SpawnManager)
		{
			const auto DoReset = [&pSpawnManager, &pThis]()
			{
				pThis->SetTarget(nullptr);
				pSpawnManager->SetTarget(nullptr);
				pSpawnManager->ResetTarget();
			};

			const auto DoCheckWithWeapon = [&](TechnoClass* pOwner, bool bCheckSpawner = false, SpawnNode* pNode = nullptr)
			{
				WeaponTypeClass* pWeapon = nullptr;
				WeaponTypeClass* pWeapon_2 = nullptr;
				WeaponStruct nWeaponStruct;

				if (!pOwner->GetTechnoType()->IsGattling)
				{
					nWeaponStruct = pOwner->GetTechnoType()->GetWeapon(0, pOwner->Veterancy.IsElite());

					if (WeaponStruct::IsValid(&nWeaponStruct))
						pWeapon = nWeaponStruct.WeaponType;

					nWeaponStruct = pOwner->GetTechnoType()->GetWeapon(1, pOwner->Veterancy.IsElite());
					if (WeaponStruct::IsValid(&nWeaponStruct))
						pWeapon_2 = nWeaponStruct.WeaponType;

					if (!pWeapon && pWeapon_2)
						pWeapon = pWeapon_2;
				}
				else
				{
					nWeaponStruct = pOwner->GetTechnoType()->GetWeapon(pOwner->CurrentWeaponNumber, pOwner->Veterancy.IsElite());

					if (WeaponStruct::IsValid(&nWeaponStruct))
						pWeapon = nWeaponStruct.WeaponType;
				}

				if (!pWeapon)
					return;

				if (bCheckSpawner && !pWeapon->Spawner)
					return;

				if (GeneralUtils::GetWarheadVersusArmor(pWeapon->Warhead, pTargetTech->GetTechnoType()->Armor) * pWeapon->Damage < 0.0 &&
					pTargetTech->IsGreenHP())
				{
					DoReset();
				}
			};

			//check owner
			DoCheckWithWeapon(pThis, true);

			//check spawnee
			for (const auto& pNode : pSpawnManager->SpawnedNodes)
			{
				auto pSpawnee = pNode->Unit;
				// check more status , so it wont reset everytime not needed
				if (pSpawnee && !pSpawnee->GetTechnoType()->MissileSpawn && pSpawnee->Target == pThis->Target && (pNode->Status == SpawnNodeStatus::Attacking || pNode->Status == SpawnNodeStatus::Preparing || pNode->Status == SpawnNodeStatus::TakeOff))
					DoCheckWithWeapon(pSpawnee, false, pNode);
			}
		}
	}

	return 0;
}
*/
/*
DEFINE_HOOK(0x447FED, BuildingClass_GetFireError_Facing, 0x7)
{
	enum { Continue = 0x0, Skip = 0x448052 };

	GET(BuildingClass*, pThis, ESI);

	return pThis->Type->TurretAnimIsVoxel || !pThis->Type->IsAnimDelayedFire || pThis->Anims[(int)BuildingAnimSlot::SpecialThree] ? Continue : Skip;
}

DEFINE_HOOK(0x44404D, BuildingClass_KickOutUnit_AircraftType_ZCoord, 0x6)
{
	GET(AircraftClass*, pProduct, EBP);
	//GET(BuildingClass*, pThis, ESI);

	if (pProduct->GetHeight() > 0)
		pProduct->Tracker_4134A0();

	return 0;
}
*/

/*
DEFINE_HOOK(0x74A904, VoxelAnimClass_AI_DamageArea_FixNoHouseOwner, 0x6)
{
	GET(VoxelAnimClass*, pThis, EBX);
	GET(ObjectClass*, pVictim, EDI);
	GET(WarheadTypeClass*, pWarhead, EDX);
	GET(int, nDistance, ECX);

	Debug::Log("VoxelAnim DamageArea for Victim [%x]=%s ! \n", pVictim, pVictim->GetTechnoType()->get_ID());
	pVictim->ReceiveDamage(&pThis->Type->Damage, Game::AdjustForZ(nDistance), pWarhead, nullptr, false, false, pThis->OwnerHouse);

	return 0x74A929;
}

DEFINE_HOOK(0x687AF4, CCINIClass_InitializeStuffOnMap_AdjustAircrafts, 0x5)
{
	//Debug::Log_WithBool(false, __FUNCTION__" Executed! \n");
	for (auto const& pAircraft : *AircraftClass::Array())
	{
		const auto& pCell = pAircraft->GetCell();

		if (!pCell || !pAircraft->Type->AirportBound || pAircraft->DockedTo)
			continue;

		if (const auto& pCellBuilding = pCell->GetBuilding())
		{
			if (pCellBuilding->Type->Helipad && pCellBuilding->Owner == pAircraft->Owner)
			{
				pCellBuilding->SendCommand(RadioCommand::RequestLink, pAircraft);
				pCellBuilding->SendCommand(RadioCommand::RequestTether, pAircraft);
				pAircraft->SetLocation(pCellBuilding->GetDockCoords(pAircraft));
				pAircraft->DockedTo = pCellBuilding;

				if (pAircraft->GetHeight() > 0)
					pAircraft->Tracker_4134A0();

				pCellBuilding->Owner->RecheckTechTree = true;
			}
		}
	}

	return 0;
}

DEFINE_HOOK(0x534A3C, ScenarioClass_initTheater_TheaterType_DebugLog, 0x6)
{
	GET(TheaterType, nType, EDI);
	R->ECX(TheaterTypeClass::GetIdentifier(nType));
	return 0x534A42;
}*/

/*
DEFINE_HOOK(0x4DB33E, FootClass_Unlimbo_TeleOccupyBug, 0x7)
{
	GET(FootClass*, pFoot, EDI);

	if (pFoot->WhatAmI() == AbstractType::Unit)
	{
		auto const pLoco = static_cast<LocomotionClass*>(pFoot->Locomotor.get());

		CLSID clsid;
		if (SUCCEEDED(pLoco->GetClassID(&clsid))
			&& clsid == LocomotionClass::CLSIDs::Teleport)
		{
			static_cast<TeleportLocomotionClass*>(pLoco)->LastCoords = CoordStruct::Empty;
		}
	}

	return 0x0;
}*/
/*
DEFINE_HOOK(0x710021, FootClass_ImbueLocomotor_SpawnRate, 0x5)
{
	GET(SpawnManagerClass*, pManager, ECX);

	pManager->KillNodes();

	for (const auto& nNodes : pManager->SpawnedNodes)
	{
		nNodes->SpawnTimer.Start(pManager->RegenRate);
	}

	return 0x710026;
}

DEFINE_HOOK(0x4D54DD, FootClass_Mi_Hunt_NoPath, 0x6)
{
	GET(FootClass*, pThis, ESI);

	auto pOwner = pThis->Owner;

	if ((pOwner && !pOwner->ControlledByPlayer()) &&
		!(Unsorted::CurrentFrame % 450) &&
		pThis->CurrentMapCoords == pThis->LastMapCoords)
	{
		pThis->SetDestination(nullptr, true);
		pThis->SetTarget(nullptr);

		if (auto pSpawnManager = pThis->SpawnManager)
			pSpawnManager->ResetTarget();

		auto nLoc = &pThis->Location;
		pThis->TargetAndEstimateDamage((DWORD)nLoc, 0u);

		if (!pThis->Target)
		{
			if (auto pTarget = pOwner->FindTargetOnCoords(pThis->Location))
			{
				pThis->SetDestination(pTarget, true);
				pThis->SetTarget(pTarget);
			}
		}
	}

	return 0;
}

DEFINE_HOOK(0x4D8825, FootClass_UpdatePosition_Decloak, 0xA)
{
	GET(FootClass*, pThis, ESI);

	bool bUncloakWhenMove = true;

	if (bUncloakWhenMove)
		pThis->Reveal();

	return 0x4D882F;
}

DEFINE_HOOK(0x74192E, UnitClass_CrushCell_Decloak, 0x5)
{
	//GET(UnitClass*, pThis, EDI);
	bool bUncloakWhenHit = true;

	return !bUncloakWhenHit ? 0x741939 : 0x0;
}

DEFINE_HOOK(0x4DA64D, FootClass_Update_IsInPlayField, 0x6)
{
	GET(TechnoTypeClass*, pFootType, EAX);

	return pFootType->BalloonHover || pFootType->JumpJet ? 0x4DA655 : 0x4DA677;
}*/


/*
DEFINE_HOOK(0x513E9D, HoverLocomotionClass_DisableDampen_ResourceGatherer, 0x9)
{
	GET(HoverLocomotionClass*, pThis, ESI);

	bool bCalculateDampen = true;
	auto pLinked = pThis->LinkedTo;

	if (auto pUnit = specific_cast<UnitClass*>(pLinked))
	{
		if (pUnit->Type->ResourceGatherer)
		{
			bCalculateDampen = (pLinked->GetCurrentMission() != Mission::Harvest);
			pLinked->InAir = pLinked->GetHeight() > 0;
		}
	}

	return bCalculateDampen && pThis->Is_Powered() ? 0x513EAA : 0x513ECD;
}

DEFINE_HOOK(0x714522, TechnoTypeClass_LoadFromINI_RequiredHouses, 0x9)
{
	GET(CCINIClass*, pINI, ESI);
	GET(TechnoTypeClass*, pTech, EBP);
	GET(const char*, pSection, EBX);
	auto pKey = Make_Pointer<const char>(0x843BB4);
	unsigned int nRet = 0;

	if (pINI->GetString(pSection, pKey, Phobos::readBuffer, sizeof(Phobos::readBuffer)))
	{
		if (!CCINIClass::IsBlank(Phobos::readBuffer))
		{
			char* pContext = nullptr;
			for (char* cur = strtok_s(Phobos::readBuffer, Phobos::readDelims, &pContext);
				cur;
				cur = strtok_s(nullptr, Phobos::readDelims, &pContext))
			{
				nRet |= HouseTypeClass::FindIndexOfNameShiftToTheRightOnce(cur);
			}
		}
	}

	pTech->RequiredHouses = nRet;
	return 0x71453C;
}
*/

/*
DEFINE_HOOK(0x4FD8F7, HouseClass_TryGetEnemies, 0x6)
{
	bool AllowAIToSellAndQuitGame = false;

	return AllowAIToSellAndQuitGame ? 0x0 : 0x4FD907;
}
*/

/*
DEFINE_HOOK(0x468112, BulletClass_Draw_Voxel, 0x9)
{
	GET(BulletClass*, pThis, ESI);
	GET(BulletTypeClass*, pThisType, EAX);
	LEA_STACK(Matrix3D*, pMTX, 0x30);

	auto const& nVox = pThisType->MainVoxel;

	if ((!nVox.VXL || !nVox.HVA))
	{
		Debug::Log("Projectile [%x][%s] has invalid voxel/hva image !\n", pThis, pThisType->get_ID());
		return 0x468258;
	}

	pMTX->MakeIdentity();
	return 0x46811B;
}


DEFINE_HOOK(0x4DA9C9, FootClass_Update_DeployToLand_WalkedFrame, 0xA)
{
	GET(FootClass*, pThis, ESI);
	GET(TechnoTypeClass*, pThisType, EAX);

	return !pThisType->JumpJet || pThis->GetHeight() <= 0 ? 0x4DAA01 : 0x4DA9D7;
}*/
/*
DEFINE_HOOK(0x469CA7, BulletClass_Logics_DoAtomDamage_IsNukeWarhead, 0x6)
{
	GET(WarheadTypeClass*, pWarhead, EDX);

	auto pWarheadExt = WarheadTypeExt::ExtMap.Find(pWarhead);
	return pWarheadExt->AnotherData.IsNukeWarhead.Get() ? 0x469CAF : 0x469D06;
}
*/
/*
namespace MapClass_DoAtomDamage_Context
{
	HouseClass* HouseOwner;
	TechnoClass* TechnoOwner;
	WarheadTypeClass* Warhead;
	Nullable<int> Damage;
}
DEFINE_HOOK(0x469CB5, BulletClass_Logics_DoAtomDamage_Setcontext, 0x5)
{
	GET(BulletClass*, pThis, ESI);
	GET(ObjectClass*, pOwnerObject, EAX);

	if (auto pTechOwner = generic_cast<TechnoClass*>(pOwnerObject))
	{
		MapClass_DoAtomDamage_Context::TechnoOwner = pTechOwner;
		MapClass_DoAtomDamage_Context::HouseOwner = pTechOwner->GetOwningHouse();
	}

	MapClass_DoAtomDamage_Context::Warhead = pThis->WH;
	if (auto const pWeapon = pThis->GetWeaponType())
		MapClass_DoAtomDamage_Context::Damage = pWeapon->Damage;

	return 0x0;
}

static DamageAreaResult __fastcall _MapClass_DoAtomDamage_DamageArea
(
	CoordStruct* pCoord,
	int nDamage,
	TechnoClass* pSource, //nullptr
	WarheadTypeClass* pWarhead,
	bool AffectTiberium, //false
	HouseClass* pSourceHouse //nullptr
)
{
	auto const nCoord = *pCoord;
	auto const pDecidedWarhead = MapClass_DoAtomDamage_Context::Warhead ? MapClass_DoAtomDamage_Context::Warhead : pWarhead;
	auto const nResult = Map.DamageArea
	(nCoord,
		MapClass_DoAtomDamage_Context::Damage.Get(nDamage),
		MapClass_DoAtomDamage_Context::TechnoOwner,
		pDecidedWarhead, pDecidedWarhead->Tiberium,
		MapClass_DoAtomDamage_Context::HouseOwner);

	//clean up context
	MapClass_DoAtomDamage_Context::TechnoOwner = nullptr;
	MapClass_DoAtomDamage_Context::HouseOwner = nullptr;;
	MapClass_DoAtomDamage_Context::Warhead = nullptr;
	MapClass_DoAtomDamage_Context::Damage.Reset();

	return nResult;
}

DEFINE_POINTER_CALL(0x425237, _MapClass_DoAtomDamage_DamageArea);
*/

/* ToDo : fixed direction where the thing will go , instead of random direction
*  it may need to expand how quaterion works unfortunately , i dont  really understand
*
* https://stackoverflow.com/questions/66472309/how-can-i-check-if-an-object-is-facing-a-certain-direction-in-unity-c-sharp
*/



/*
DEFINE_HOOK(0x70F26B, TechnoClass_DrawBehind_invisible, 0x5)
{
	GET(TechnoClass*, pThis, ESI);

	return !GameOptions.ShowHidden || pThis->GetTechnoType()->Invisible ?  0x70F278 :0x70F659;
}

namespace Gatt
{
	void TechnoClass_AI_Gattling(TechnoClass* pThis)
	{
		//timer reach
		auto pType = pThis->GetTechnoType();
		pThis->AdjustGattlingValue(pType->RateDown);
		auto nCurrentGattVal = pThis->GetCurrentGattlingValue();
		auto nCurrentStage = pThis->GetCurrentGattlingStage();

		if (nCurrentGattVal > 0)
		{
			auto const nWeaponStage = pThis->Veterancy.IsElite() ?
				pType->EliteStage[nCurrentStage] : pType->WeaponStage[nCurrentStage];
			if (nCurrentGattVal < nWeaponStage)
				pThis->SetGattlingStage(--nCurrentStage);
		}
	}
};

#include <Constructable.h>

namespace __weaponcontainer
{
	std::vector<ConstructableWeaponStruct> pWeaponData;

	void Delatage(WeaponTypeClass* pWeapon, CoordStruct FLH)
	{
		pWeaponData.push_back({ pWeapon, FLH, 0, 0, false });

		if (!pWeaponData.empty())
		{
			auto nData = pWeaponData[0];
			auto pWeapon___ = nData.WeaponType->get_ID();
			Debug::Log("Weapon data has item with [%s] [%d.%d.%d] [%d] [%d] [%s] \n",
				pWeapon___,
				nData.FLH.X,
				nData.FLH.Y,
				nData.FLH.Z,
				nData.BarrelLength,
				nData.BarrelThickness,
				nData.TurretLocked ? "Yes" : "No"
			);
		}
	}
}

 //size ?
DEFINE_HOOK(0x415F0E, AircraftClass_FireAt_check, 0x6)
{
	enum
	{
		Delete = 0x41659E,
		CheckLoco = 0x415F39,
		ROTOneCalc = 0x4160CF,
		SightCalculation = 0x41631F,
	};

	GET(AircraftClass*, pThis, EDI);
	GET(AbstractClass*, pTarget, EBX);
	GET(int, nWhichSlot, EAX);

	auto pBullet = pThis->FireAt(pTarget, nWhichSlot);
	R->EAX(pBullet);
	R->Stack(0x10, pBullet);

	if (!pBullet)
		return Delete;

	auto pBulletType = pBullet->Type;

	if (!pBulletType->ROT)
		return CheckLoco;

	if (pBulletType->ROT == 1)
		return ROTOneCalc;

	return SightCalculation;
}

namespace Aircraft_ReceiveDamage_Add
{
	Nullable<bool> Crashable { };

	DWORD SendRadioForBuilding(REGISTERS* R, AircraftClass* pThis)
	{
		bool bCrashable = true;
		//if there is building and contact estabilished , dont crash
		if (auto pCell = pThis->GetCell())
			if (auto pBuilding = pCell->GetBuilding())
				if (pBuilding->Owner == pThis->GetOwningHouse())
					if (pThis->SendCommand(RadioCommand::QueryCanEnter, pBuilding) == RadioCommand::AnswerPositive)
						bCrashable = false;

		if (!Crashable.Get(pThis->Locomotor.get()->Is_Moving() && bCrashable))
		{
			R->EAX(0);
			return 0x41669A;
		}

		return 0x0;
	}
};


namespace Context_
{
	AnimClass* pAnim_BounceInit = nullptr;
	VoxelAnimClass* pVoxAnim_BounceInit = nullptr;
	InfantryClass* pInf_ = nullptr;
	TechnoClass* pTechno_1 = nullptr;
	TechnoClass* pTechno_2 = nullptr;
};

DEFINE_HOOK(0x42257E, AnimClass_BounceInit_, 0xA)
{
	GET(AnimClass*, pThis, ESI);
	Context_::pAnim_BounceInit = pThis;
	return 0x0;
}

static void __fastcall BounceReplaceWrapper_Anim(
BounceClassCopy* pThis,
void* _,
CoordStruct* pCoord,
double Els,
double Grav,
double maxVel,
Vector3D<float>* pVel,
double aVel)
{
	double nGravOut = Grav;

	if (Context_::pAnim_BounceInit)
	{
		//Debug::Log(__FUNCTION__" Executed for [%x=%s] Original Graf [%fl]! \n" , pAnim, pAnim->Type->ID , Grav);
		auto pAnimExt = AnimTypeExt::ExtMap.Find(Context_::pAnim_BounceInit->Type);
		nGravOut = pAnimExt->Bounce_Gravity.Get(Grav);
		nGravOut = nGravOut <= 0.0 ? Grav : nGravOut;
	}

	pThis->BounceClassInit_Init(pCoord, Els, nGravOut, maxVel, pVel, aVel);

	Context_::pAnim_BounceInit = nullptr;
}

DEFINE_POINTER_CALL(0x422648, BounceReplaceWrapper_Anim);

DEFINE_HOOK(0x7497D5, VoxelAnimClass_BounceInit_, 0x9)
{
	GET(VoxelAnimClass*, pThis, ESI);
	Context_::pVoxAnim_BounceInit = pThis;
	return 0x0;
}

FUNC_STATIC(void,BounceReplaceWrapper_VoxelAnim,
BounceClassCopy* pThis,
void* _,
CoordStruct* pCoord,
double Els,
double Grav,
double maxVel,
FloatVelocity* pVel,
double aVel)
{
	//if (pAnim)
	//{
		//Debug::Log(__FUNCTION__" Executed for [%x=%s] Original Graf [%fl]! \n", pAnim, pAnim->Type->ID, Grav);
		//Grav = (double) RulesGlobal->Gravity;
	//}

	pThis->BounceClassInit_Init(pCoord, Els, Grav, maxVel, pVel, aVel);
	Context_::pVoxAnim_BounceInit = nullptr;
}

DEFINE_POINTER_CALL(0x74981F, BounceReplaceWrapper_VoxelAnim);

DEFINE_HOOK(0x520C16 , _InfantryClass_DoingAI_Context_,0x8)
{
	GET(InfantryClass*, pThis, ESI);
	Context_::pInf_ = pThis;
	return 0;
}

DEFINE_HOOK(0x70244D, TechnoClass_TakeDamage_AnimCtor_context_A, 0x9)
{
	GET(TechnoClass*, pThis, ESI);
	Context_::pTechno_1 = pThis;
	return 0x0;
}

AnimClass* __fastcall _AnimClass_Ctor_Replace_Techno_A(
AnimClassCopy* pThis,
void* _,
AnimTypeClass* pType,
CoordStruct* pCoord,
int LoopDelay,
int LoopCount, DWORD flags, int ForceZAdjust, bool reverse)
{
	auto nBuff = pThis->_AnimClass_CTOR(pType, pCoord, LoopDelay, LoopCount, flags, ForceZAdjust, reverse);
	if (nBuff)
	{
		if (AnimTypeExt::ExtMap.Find(pType)->CreateUnit.Get())
			AnimExt::SetAnimOwnerHouseKind(nBuff, Context_::pTechno_1->GetOwningHouse(), nullptr, false);
		else
			nBuff->Owner = Context_::pTechno_1->GetOwningHouse();
	}

	Context_::pTechno_1 = nullptr;
	return nBuff;
}

DEFINE_POINTER_CALL(0x7024AA, &_AnimClass_Ctor_Replace_Techno_A);

DEFINE_HOOK(0x7024F3, TechnoClass_TakeDamage_AnimCtor_context_B, 0x6)
{
	GET(TechnoClass*, pThis, ESI);
	Context_::pTechno_2 = pThis;
	return 0x0;
}

AnimClass* __fastcall _AnimClass_Ctor_Replace_Techno_B(
AnimClassCopy* pThis,
void* _,
AnimTypeClass* pType,
CoordStruct* pCoord,
int LoopDelay,
int LoopCount, DWORD flags, int ForceZAdjust, bool reverse)
{
	auto nBuff = pThis->_AnimClass_CTOR(pType, pCoord, LoopDelay, LoopCount, flags, ForceZAdjust, reverse);
	if (nBuff)
	{
		if (AnimTypeExt::ExtMap.Find(pType)->CreateUnit.Get())
			AnimExt::SetAnimOwnerHouseKind(nBuff, Context_::pTechno_2->GetOwningHouse(), nullptr, false);
		else
			nBuff->Owner = Context_::pTechno_2->GetOwningHouse();
	}

	Context_::pTechno_2 = nullptr;
	return nBuff;
}

DEFINE_POINTER_CALL(0x702566, &_AnimClass_Ctor_Replace_Techno_B)

DEFINE_HOOK(0x466CFB, BulletClass_ProjectileMotion_fetchData, 0xB)
{
	GET(BulletClass*, pThis, EBP);
	_fetchCall::ProjMotion_bulletContext = pThis;
	return 0;
}

static double __fastcall ProjectileMotion_Exec(
CoordStruct* pCoord,
BulletVelocity* pVel,
CoordStruct* pSecondCoord,
DirStruct* pDir,
bool bInAir,
bool bAirburs,
bool bVeryHigh,
bool bLevel)
{
	//if (auto pBullet = ProjMotion_bulletContext)
	//	Debug::Log(__FUNCTION__"Exec for [%x][%s] !\n", pBullet, pBullet->Type->get_ID());

	auto nResult = BulletClass::ProjectileMotion(pCoord, pVel, pSecondCoord, pDir, bInAir, bAirburs, bVeryHigh, bLevel);
	_fetchCall::ProjMotion_bulletContext = nullptr;
	return nResult;
}

DEFINE_POINTER_CALL(0x466D31, ProjectileMotion_Exec);
*/
/*
namespace __ProjMotion___
{
	double convert(std::bitset<64> const& bs)
	{
		static_assert(sizeof(uint64_t) == sizeof(double), "Cannot use this!");

		uint64_t const u = bs.to_ullong();
		double d;

		// Aliases to `char*` are explicitly allowed in the Standard (and only them)
		char const* cu = reinterpret_cast<char const*>(&u);
		char* cd = reinterpret_cast<char*>(&d);

		// Copy the bitwise representation from u to d
		memcpy(cd, cu, sizeof(u));

		return d;
	}

 double Process(CoordStruct* pCoordTo,
   VelocityClass* pVel,
   CoordStruct* pCoordFrom,
   DirStruct* pDir,
   bool bInAir,
   bool bAirburs,
   bool bVeryHigh,
   bool bLevel)
	{
		auto nCoord = *pCoordFrom;

		if (nCoord == CoordStruct::Empty)
		{
			auto nXYVel = pVel->DistanceXY();
			DirStruct VelToDir  = DirStruct(pVel->Z, nXYVel);
			DirStruct nBaseDir = DirStruct(0x2000);

			if (VelToDir.CompareToTwoDir(&nBaseDir, pDir))
				VelToDir = nBaseDir;
			else if ((nBaseDir - VelToDir).value8() >= 0)
				VelToDir += *pDir;
			else
				VelToDir -= *pDir;

			DirStruct nNegated = DirStruct(16383);
			auto nVelToDirRes = VelToDir - nNegated;
			auto nRad = nVelToDirRes.radians<8>();

			if (nRad != 0.0)
			{
				//?
				pVel->X /= Math::cos(nRad);
				pVel->Y /= Math::cos(nRad);
			}

			////
		}

		return 0.0;
	}
};

DEFINE_HOOK(0x466CFB, BulletClass_ProjectileMotion_fetchData, 0xB)
{
	GET(BulletClass*, pThis, EBP);
	_fetchCall::ProjMotion_bulletContext = pThis;
	return 0;
}

//we call it then clean up the context
static int __fastcall ProjectileMotion_Exec(
CoordStruct* pCoord,
BulletVelocity* pVel,
CoordStruct* pSecondCoord,
DirStruct* pDir,
bool bInAir,
bool bAirburs,
bool bVeryHigh,
bool bLevel)
{
	auto nResult = BulletClass::ProjectileMotion(pCoord, pVel, pSecondCoord, pDir, bInAir, bAirburs, bVeryHigh, bLevel);
	_fetchCall::ProjMotion_bulletContext = nullptr; //clean up
	return nResult;
}

DEFINE_POINTER_CALL(0x466D31, ProjectileMotion_Exec);

// Raw is unsigned
// if Raw to Direction ((( Raw >> 7 ) + 1 ) >> 1)
// if from Direction to Raw  (Direction <<8 )


// Expanding NomMM limited array
PhobosMap<int, int> NonMarbleArr;
DEFINE_HOOK(0x545CE2, IsoTileType_LoadFromINI_NonMMArray_Add_replace, 0x7)
{
	GET(int, nIdx, EAX);
	GET(int, nCount, EBX);
	NonMarbleArr[nIdx] = nCount;
	return 0x545CE9;
}

DEFINE_HOOK(0x546C72, IsoTileType_LoadFromINI_NonMMArray_Get_replace2, 0x7)
{
	GET(int, nIdx, EDX);
	R->EDX(NonMarbleArr.get_or_default(nIdx));
	return 0x546C79;
}

DEFINE_HOOK(0x546C55, IsoTileType_LoadFromINI_NonMMArray_Get_replace1, 0x7)
{
	GET(int, nIdx, EDX);
	R->EDX(NonMarbleArr.get_or_default(nIdx));
	return 0x546C5C;
}

DEFINE_HOOK(0x546C8B, IsoTileType_LoadFromINI_NonMMArray_clear, 0xA)
{
	NonMarbleArr.clear();
	return 0x0;
}

DEFINE_HOOK(0x6FC339, TechnoClass_CanFire_Early, 0x6)
{
	GET(TechnoClass*, pThis, ESI);
	GET(WeaponTypeClass*, pWeapon, EDI);
	GET_STACK(AbstractClass*, pTarget, STACK_OFFS(0x20, -0x4));
	bool bCeaseFire = false;

	Debug::Log_WithBool(true, __FUNCTION__" Executed For[%s] Weapon[%s] Cease[%s] HaveTarget[%s] \n", pThis->get_ID(), pWeapon->get_ID(), bCeaseFire ? "Yes" : "No", pTarget ? "Yes" : "No");
	return bCeaseFire ? 0x6FCB7E : 0x0;
}

DEFINE_HOOK(0x6FDD50, TechnoClass_Fire_Early, 0x6)
{
	GET(TechnoClass*, pThis, ECX);
	GET_STACK(int, nWeapon, 0x8);
	GET_STACK(AbstractClass*, pTarget, 0x4);

	Debug::Log_WithBool(true, __FUNCTION__" Executed For[%s] WeaponIdx[%d] HaveTarget[%s] \n", pThis->get_ID(), nWeapon, pTarget ? "Yes" : "No");

	return 0x0;
}

DEFINE_HOOK(0x740244, UnitClass_GetCursorOverObject_Armory, 0x6)
{
	GET(BuildingTypeClass*, pThat, ECX);

	R->AL(pThat->Bunker || pThat->Armory);

	return 0x74024A;
}

//char DrawIt_47EC90(CoordStruct& nCoord, RectangleStruct& nRect, bool bBlit)

namespace Reworked
{
	void DrawPips(TechnoClass* pThis, Point2D* pLoc_1, Point2D* pLoc_2, RectangleStruct* pBound)
	{
		int nPipIdx = 4;
		Point2D nPos_Loc_1 = Point2D::Empty;
		SHPStruct* pPipShape = nullptr;
		int nIncrement = 0;

		if (pThis->WhatAmI() == AbstractType::Building)
		{
			nPos_Loc_1 = { pLoc_1->X + 6, pLoc_2->Y - 1 };
			nIncrement = 2;
			pPipShape = FileSystem::PIPS2_SHP();
		}
		else
		{
			nPos_Loc_1 = { pLoc_1->X - 5, pLoc_2->Y };
			nIncrement = 0;
			pPipShape = FileSystem::PIPS_SHP();

			if (pThis->WhatAmI() == AbstractType::Infantry)
				nPos_Loc_1.X += 11;
		}

		auto pType = pThis->GetTechnoType();

		switch (pType->PipScale)
		{
		case PipScale::Ammo:
		case PipScale::MindControl:
		case PipScale::Passengers:
		case PipScale::Power:
		{
			if (pThis->WhatAmI() == AbstractType::Unit)
			{
				//draw number pip , representing power
			}
		}
		break;
		case PipScale::Tiberium:
		default:
			break;
		}
	}
};

DEFINE_HOOK(0x5FDD50, OverlayClass_GetTibType_CheckTibArray, 0x8)
{
	GET(int, nArrayCount, EDX);

	//Debug::Log_WithBool(!Phobos::Config::MoreDetailSLDebugLog, __FUNCTION__"current array count[%d] ! \n", nArrayCount);

	R->EDI(TiberiumClass::Array()->Items);
	return nArrayCount >= 0 ? 0x5FDD5A : 0x5FDDA4;
}

DEFINE_HOOK(0x74A6C6, VoxelAnimClass_IsTiberium_FindIndexPlusRandom_crash, 0x5)
{
	GET(VoxelAnimClass*, pThis, EBX);
	GET(int, nOverlayTypeIdx, ECX);
	GET(int, nRandomAdd, EAX);

	Debug::Log_WithBool(!Phobos::Config::MoreDetailSLDebugLog,"VoxelAnim[%x]=[%s] Trying to spawn tiberium with overlay idx[%d] + [%d] ! \n", pThis->Type->get_ID(), nOverlayTypeIdx, nRandomAdd);

	return 0x0;
}

DEFINE_HOOK(0x44FE51, BuildingClass_ReadINI_AutoDockAircrafts, 0x1)
{
	for (auto& pAircraft : *AircraftClass::Array)
	{
		if (pAircraft->Type->AirportBound)
		{
			if (auto pBld = pAircraft->GetCell()->GetBuilding())
			{
				if (pBld->Type->Helipad)
				{
					pBld->SendCommand(RadioCommand::RequestLink, pAircraft);
					pBld->SendCommand(RadioCommand::RequestTether, pAircraft);
					if (pAircraft->GetHeight() > 0)
						AircraftTrackerClass::Instance->Add(pAircraft);
				}
			}
		}
	}

	return 0;
}

DEFINE_HOOK(0x4CD8C9, FlyLocomotionClass_Movement_AI_DisableTSExp, 0x9)
{
	GET(FootClass*, pFoot, EDX);
	auto const& pTypeExt = TechnoTypeExt::ExtMap.Find(pFoot->GetTechnoType());
	return pTypeExt->AnotherData.Disable_C4WarheadExp.Get() ? 0x4CD9C0 : 0x0;
}

DEFINE_HOOK(0x6FE26E, TechnoClass_Fire_CrawlFHL, 0x6)
{
	GET(TechnoClass* const, pThis, ESI);
	GET(CoordStruct*, pDefaultFLH, EAX);
	GET_BASE(int const, idxWeapon, 0xC);

	if (auto pInf = specific_cast<InfantryClass*>(pThis))
	{
		if (pInf->Crawling)
		{
			auto const pTypeExt = TechnoTypeExt::ExtMap.Find(pThis->GetTechnoType());
			auto const& nFLH = pTypeExt->CrawlFLHData;
			CoordStruct nRet = *pDefaultFLH;

			if (!pThis->Veterancy.IsElite())
			{
				if (idxWeapon == 0)
					nRet = nFLH.PrimaryCrawlFLH.Get(*pDefaultFLH);
				else
					nRet = nFLH.SecondaryCrawlFLH.Get(*pDefaultFLH);
			}
			else
			{
				if (idxWeapon == 0)
					nRet = nFLH.Elite_PrimaryCrawlFLH.Get(*pDefaultFLH);
				else
					nRet = nFLH.Elite_SecondaryCrawlFLH.Get(*pDefaultFLH);
			}
			*pDefaultFLH = { nRet.X,nRet.Y,nRet.Z };
		}
	}

	return 0x0;
}





DEFINE_HOOK(0x43FBEF, BuildingClass_AI_ChangePowerState, 0x6)
{
	GET(BuildingClass*, pThis, ESI);

	auto const pTypeExt = BuildingTypeExt::ExtMap.Find(pThis->Type);

	if (!pThis->IsReadyToCommence || !pThis->Anims[9])
		return 0;

	auto const pRules = RulesClass::Instance();
	bool const bWasOnline = static_cast<bool>(R->BL());

	if (!bWasOnline && pTypeExt->AnotherTypeData.TurretAnim_DamagedLowPower.isset() && pTypeExt->AnotherTypeData.TurretAnim_LowPower.isset())
	{
		pThis->Anims[9]->Type = pThis->GetHealthPercentage() <= pRules->ConditionYellow ? pTypeExt->AnotherTypeData.TurretAnim_DamagedLowPower.Get() : pTypeExt->AnotherTypeData.TurretAnim_LowPower.Get();
		return 0;
	}

	auto const pOriginal_A = pThis->Type->BuildingAnim[9].Anim;
	auto const pOrigilan_D = pThis->Type->BuildingAnim[9].Damaged;
	pThis->Anims[9]->Type = AnimTypeClass::Find((pThis->GetHealthPercentage() <= pRules->ConditionYellow) ? pOrigilan_D : pOriginal_A);

	return 0;
}

DEFINE_HOOK_AGAIN(0x450949, BuildingClass_ReceiveDamage_TurretAnim, 0x5)
DEFINE_HOOK_AGAIN(0x442B14, BuildingClass_ReceiveDamage_TurretAnim, 0x5)
DEFINE_HOOK(0x442BC2, BuildingClass_ReceiveDamage_TurretAnim, 0x5)
{
	GET(BuildingClass*, pThis, ESI);
	GET(BuildingAnimSlot, nSlot, EBP);

	if (nSlot == BuildingAnimSlot::Turret && !pThis->IsPowerOnline())
	{
		auto const pTypeExt = BuildingTypeExt::ExtMap.Find(pThis->Type);
		auto const pAnim = pTypeExt->AnotherTypeData.TurretAnim_LowPower.isset() ? pTypeExt->AnotherTypeData.TurretAnim_LowPower.Get()->ID : (pThis->Type->BuildingAnim[9].Anim);
		auto const pAnim_D = pTypeExt->AnotherTypeData.TurretAnim_DamagedLowPower.isset() ? pTypeExt->AnotherTypeData.TurretAnim_DamagedLowPower.Get()->ID : (pThis->Type->BuildingAnim[9].Damaged);
		auto const pRules = RulesClass::Instance();

		R->EAX(pThis->GetHealthPercentage() <= pRules->ConditionYellow ? pAnim_D : pAnim);
	}

	return 0;
}

DEFINE_HOOK(0x6D5D74, BuildingClass_PlaceWall_DrawTranslucency, 0x5)
{
	//PlaceHolder , not really sure how to deal with this atm
	R->EAX(0x20606);
	return 0x6D5D79;
}

DEFINE_HOOK(0x6D5AD9, BuildingClass_PlaceFirestromWall_DrawTranslucency, 0x5)
{
	//PlaceHolder , not really sure how to deal with this atm
	R->EAX(0x20606);
	return 0x6D5ADE;
}

DEFINE_HOOK(0x6D584F, BuildingClass_PlaceLaserFence_DrawTranslucency, 0x5)
{
	//PlaceHolder , not really sure how to deal with this atm
	R->EDX(0x20606);
	return 0x6D5ADE;
}

//DEFINE_HOOK(0x47EECC, BuildingClass_Place_Transculency, 0x6)
//{
	//PlaceHolder , not really sure how to deal with this atm
//	R->EDX(0x20600);
//	return 0x47EED2;
//}


DEFINE_HOOK_AGAIN(0x6D5EB1, BuildingClass_Place_DrawSHP, 0x6)
DEFINE_HOOK(0x47EF52, BuildingClass_Place_DrawSHP, 0x6)
{
	//PlaceHolder , not really sure how to deal with this atm
	R->EDX(Make_Pointer_B<SHPStruct>(0x8A03FC));
	return R->Origin() + 6;
}

DEFINE_HOOK(0x6A8E2A, SidebarClass_StripClass_AI_Building_EVA_ConstructionComplete, 0x5)
{
	//GET(BuildingClass*, pBuilding, ESI);
	R->ECX(Make_Pointer<char>(0x83FA80));
	return 0x6A8E2F;
}

DEFINE_HOOK(0x4FB63F, HouseClass_PlaceObject_EVA_UnitReady, 0x5)
{
	//GET(FootClass*, pProduct, ESI);
	R->ECX(Make_Pointer<char>(0x8249A0));
	return 0x4FB644;
}




DEFINE_HOOK(0x7ADA6D, TechnoClass_SensorAI_ObserverSkipWarn, 0x6)
{ return (HouseClass::Player->IsObserver()) ? 0x70DADC : 0x0; }

DEFINE_HOOK(0x4870D0, CellClass_SensedByHouses_ObserverAlwaysSensed, 0x6)
{
	GET_STACK(int, nIdx, 0x4);

	auto const pHouseArr = *HouseClass::Array();
	auto const pHouse = pHouseArr[nIdx];

	if (pHouse && pHouse->IsObserver())
	{
		R->AL(true);
		return 0x4870DE;
	}

	return 0;
}*/

/*
DEFINE_HOOK(0x4DBDB6, FootClass_IsCloakable_CloakMove, 0x6)
{
	GET(FootClass*, pThis, ESI);

	if (auto const pTypeExt = TechnoTypeExt::ExtMap.Find(pThis->GetTechnoType()))
	{
		if (pTypeExt->Cloak_WhenMove.Get())
		{
			if (auto pLoco = pThis->Locomotor.get())
			{
				if (!pLoco->Is_Moving())
					return 0x4DBDEB;
				else
					return 0x4DBDE7;
			}
		}
	}

	return 0x0;
}

DEFINE_HOOK(0x4DBDD4, FootClass_IsCloakable_CloakMove, 0x7)
{
	GET(FootClass*, pThis, ESI);

	if (auto const pTypeExt = TechnoTypeExt::ExtMap.Find(pThis->GetTechnoType()))
	{
		if (pTypeExt->Cloak_WhenMove.Get())
		{
			if (!pThis->Locomotor->Is_Moving())
				return 0x4DBDEB;
			else
				return 0x4DBDE7;
		}
	}

	return 0x0;
}

namespace TechnoClass_FireTemp
{
	BulletClass* Bullet;
}

DEFINE_HOOK(0x6FF4CC, TechnoClass_Fire_RecordBullet, 0x6)
{
	GET_STACK(BulletClass*, pBullet, 0x3C);
	TechnoClass_FireTemp::Bullet = pBullet;
	return 0x0;
}

DEFINE_HOOK(0x6FD366, TechnoClass_LaserZap_CoordFix, 0x7)
{
	GET_STACK(AbstractClass*, pVictim, STACK_OFFS(0x48, -0x4));

	R->ECX(pVictim);

	if (pVictim)
	{
		TechnoClass_FireTemp::Bullet = nullptr;
		return ((pVictim->AbstractFlags & AbstractFlags::Object) != AbstractFlags::None)
			? 0x6FD374 : 0x6FD383;
	}
	else
	{
		if (auto const pBullet = TechnoClass_FireTemp::Bullet)
		{
			auto const nBuffer = pBullet->TargetCoords;
			R->EAX(&nBuffer);
		}
	}

	TechnoClass_FireTemp::Bullet = nullptr;
	return 0x6FD38D;
}

DEFINE_HOOK(0x5F54A8, ObjectClass_ReceiveDamage_HalfCheck, 0x6)
{
	GET(int, oldstr, EDX);
	GET(int, maxstr, EBP);
	GET(int, damage, ECX);

	auto const nHealth = (int)(maxstr * RulesGlobal->ConditionYellow);
	auto const nDamage = oldstr - damage;
	bool const nDamage_B = nDamage < nHealth;

	return oldstr <= nHealth || !nDamage_B ? 0x5F54C4 : 0x5F54B8;
}

namespace RulesTemp
{
	bool ShouldRGB;
}

DEFINE_HOOK(0x66D3D2, RulesClass_ReadColors_ShouldRGB, 0x6)
{
	GET(CCINIClass*, pINI, EBP);
	RulesTemp::ShouldRGB = pINI->ReadBool("AudioVisual", "ShouldColorsRGB", false);
	return 0x0;
}

DEFINE_HOOK(0x66D418, RulesClass_ReadColors_ReadRGB, 0x8)
{
	if (RulesTemp::ShouldRGB)
	{
		GET(HSVClass*, pHSV, EAX);
		GET(ColorStruct*, pColor, EAX);
		pColor->ConstructHSV(pHSV);
	}
	return 0x0;
}
*/

/*//need to move the check
DEFINE_HOOK(0x6F7CF7, TechnoClass_EvaluateObject_CanBeAutoTargetedBy, 0x8)
{
	GET(ObjectClass*, pThat, ESI);
	GET(TechnoClass*, pThis, EDI);

	if (auto const pThatT = generic_cast<TechnoClass*>(pThat))
		if (auto const pExt = TechnoTypeExt::ExtMap.Find(pThatT->GetTechnoType()))
			if (!pExt->AnotherData.CanBeAutoTargetedBy.empty() && !pExt->AnotherData.CanBeAutoTargetedBy.Contains(pThis->GetTechnoType()))
				return 0x6F7D19;

	return 0x0;
}

DEFINE_HOOK(0x711F58, TechnoTypeClass_CostOf_AI, 0x5)
{
	GET(HouseClass*, pOwner, EDI);
	GET(int, nVal, EAX);

	if (pOwner && !pOwner->IsPlayer())
		R->EAX(nVal * RulesClass::Instance->AIDiffs[pOwner->GetCorrectAIDifficultyIndex()].Cost);

	return 0x0;
}

static	void __fastcall _Log_NewTeam(const char* pFormat, ...)
{
	Debug::Log(pFormat);
}

DEFINE_POINTER_CALL(0x6F0A48, &_Log_NewTeam);


DEFINE_HOOK(0x4F670B, HouseClass_CanAffordBase_Check_WF, 0x7)
{
	GET(HouseClass*, pThis, ESI);
	auto nBuildWeapons = RulesGlobal->BuildWeapons;

	if (auto pType = pThis->FirstBuildableFromArray(nBuildWeapons))
		R->EAX(pType->GetActualCost(pThis));
	else
	{
		R->EAX(0);
		Debug::Log("Failed To Find Buildable Weapon Factory for House [%s] ! \n", pThis->Type->get_ID());
	}

	return 0x4F6728;
}

DEFINE_HOOK(0x4F6782, HouseClass_CanAffordBase_Check_REF, 0x7)
{
	GET(HouseClass*, pThis, ESI);
	auto nBuildWeapons = RulesGlobal->BuildRefinery;

	if (auto pType = pThis->FirstBuildableFromArray(nBuildWeapons))
		R->EAX(pType->GetActualCost(pThis));
	else
	{
		R->EAX(0);
		Debug::Log("Failed To Find Buildable Refinery for House [%s] ! \n", pThis->Type->get_ID());
	}

	return 0x4F679F;
}


// DEFINE_POINTER_CALL(0x6D49AA, &Timer::DrawTimer);
//DEFINE_POINTER_CALL(0x6D4B08, &Timer::DrawTimer);


void HouseExt::ExtData::SetNextPendingBuilding(int nHouseIdx)
{
	if (MouseClass::Instance->CurrentBuilding)
	{
		if (auto pHouse = MouseClass::Instance->CurrentBuilding->GetOwningHouse())
		{
			if (pHouse->ArrayIndex == nHouseIdx)
			{
				for (auto pFactory : *FactoryClass::Array)
				{
					if (pFactory->Object == MouseClass::Instance->CurrentBuilding && pFactory->IsDone())
					{
						auto const pData = BuildingTypeExt::ExtMap.Find(reinterpret_cast<BuildingTypeClass*>(MouseClass::Instance->CurrentBuildingType));
						if (pData->NextPendingBuilding.isset())
						{
							BuildingClass* pOldBld = (BuildingClass*)pFactory->Object;
							auto pBld = (BuildingClass*)pData->NextPendingBuilding->CreateObject(pHouse);
							pFactory->Object = pBld;

							MouseClass::Instance->CurrentBuilding = pBld;
							MouseClass::Instance->CurrentBuildingType = pBld->Type;

							pBld->Value = pOldBld->Value;
							pBld->ShouldRebuild = pOldBld->ShouldRebuild;
							pBld->SetSidebarTabObject();

							DisplayClass::Instance->SetCursorShape(pBld->GetFoundationData(true));

							auto pExt = HouseExt::ExtMap.Find(pHouse);
							pExt->CurrentBuildingHeapID = pBld->Type->ArrayIndex;
							pExt->CurrentBuildingFactory = pFactory;

							GameDelete(pOldBld);

							break;
						}
					}
				}
			}
		}
	}
}

namespace Test
{
	typedef Delegate<BulletClass*> FindBullet;
	//typedef std::function<BulletClass* ()> FindBulletS;

	void FindBulletTargetHouse(TechnoClass* pThis, FindBullet& nFunc, bool allied = true)
	{
		nFunc.BindLambda([&allied, pThis]()
		{
			auto bullets = *BulletClass::Array();
			for (int i = bullets.Count - 1; i >= 0; i--)
			{
				auto const pBullet = bullets.GetItem(i);
				if (!pBullet->IsAlive
					//|| pBullet->InLimbo
					//|| !pBullet->Owner || pBullet->Owner->Owner == pThis->Owner
					//|| (allied && pBullet->Owner->Owner->IsAlliedWith(pThis))
					)
				{
					continue;
				}
				else
				{
					return pBullet;
					break;
				}
			}

			return (BulletClass*)nullptr;
		});
	}

	BulletClass* FindBulletTargetMe(TechnoClass* pThis, bool allied = true)
	{
		FindBullet nFunc;
		FindBulletTargetHouse(pThis, nFunc, allied);
		if (auto pDummy = nFunc.Execute())
			if (pDummy->Target == pThis)
				return pDummy;

		return (BulletClass*)nullptr;
	}

	void Techno_Update_Decoy(TechnoClass* pThis)
	{
		if (auto pDummy = FindBulletTargetMe(pThis, false))
		{
			if (pDummy)
				Debug::Log("DelegaeExec Bullet Found ! [%x]= %s ", pDummy, pDummy->Type->get_ID());
		}
	}
}

//DEFINE_HOOK(0x6F9E50, TechnoClass_AI_DelegateTest, 0x5)
//{
	//GET(TechnoClass*, pThis, ECX);

	//if (!_strcmpi(pThis->GetTechnoType()->ID, "CMIN"))
	//{
	//	Test::Techno_Update_Decoy(pThis);
	//}

//	return 0x0;
//}


DEFINE_HOOK(0x5D4E3B, DispatchingMessage_ReloadResources, 0x5)
{
	LEA_STACK(tagMSG*, pMsg, STACK_OFFS(0x2C,0x1C));
	GET_STACK(DWORD, nDW, STACK_OFFS(0x2C, 0x40));

	if ((nDW == 0x10 || nDW == 0x2 || nDW == 0x112) && pMsg->wParam == (WPARAM)0xF060)
		ExitProcess(1u);

	if ((nDW == 0x104 || nDW == 0x100) && pMsg->wParam == (WPARAM)0xD && ((pMsg->lParam & 0x20000000) != 0))
	{
		//set critical section here ?
		Debug::Log(__FUNCTION__, " : Ready to manage critical sections  ! \n");
	}

	Imports::TranslateMessage(pMsg);
	Imports::DispatchMessageA(pMsg);

	return 0x5D4E4D;
}

DEFINE_HOOK(0x68B9A0, ExeRun_PreloadLibrary, 0x5)
{
	if (!LoadLibraryA("d3dx9_29.dll"))
	{
		MessageBoxA(0, "d3dx9_29.dll not available! ", "Error", 0x10u);
		ExitProcess(1u);
	}

	return 0x0;
}

DEFINE_HOOK(0x7CD810, ExeStartUp_RegisterCriticalSection, 0x9)
{

	//auto nDDraw = GetModuleHandleA(".\\DDRAW.DLL");
	//if(!nDDraw)
	//	ExitProcess(1u);

	//InitializeCriticalSection

	return 0x0;
}

#include <any>

namespace Test
{
	std::unordered_map<std::string, std::any> mProp;

	template<typename T>
	std::optional<T> GetProperty(std::string_view name)
	{
		const auto& it = std::find_if(mProp.cbegin(), mProp.cend(), [&](const auto& elem) { return std::string_view { elem.first } } == name);

		if (it == mProp.cend()) { return { }; }

		auto* value = std::any_cast<T>(&it->second.value);

		if (!value) { return { }; }

		return *value;

	}

}

DEFINE_HOOK(0x7369F4, UnitClass_Rotation_AI_Omnifire, 0x6)
{
	GET(UnitClass*, pThis, ESI);
	GET(WeaponTypeClass*, pWeapon, EAX);

	bool OmNI = pWeapon->OmniFire;
	if (auto const pExt = TechnoTypeExt::ExtMap.Find(pThis->GetTechnoType()))
	{
		auto& nData = pExt->AnotherData;
		auto nJJ = nData.MyJJData.Enable;
		if (pThis->GetTechnoType()->JumpJet
			&& pThis->IsInAir() && nJJ)
		{
			OmNI = true;
		}
	}

	R->CL(OmNI);
	return 0x7369FA;
}

DEFINE_HOOK(0x74125C, UnitClass_CanFire_OmniFire, 0x6)
{
	GET(UnitClass*, pThis, ESI);
	GET(WeaponTypeClass*, pWeapon, EBX);

	bool OmNI = pWeapon->OmniFire;
	if (auto const pExt = TechnoTypeExt::ExtMap.Find(pThis->GetTechnoType()))
	{
		auto& nData = pExt->AnotherData;
		auto nJJ = nData.MyJJData.Enable;

		if (pThis->GetTechnoType()->JumpJet
			&& pThis->IsInAir() && nJJ)
		{
			OmNI = true;
		}
	}

	R->AL(OmNI);

	return 0x741262;
}

#include <Misc/Otamaa/Misc/DynamicPatcher/Helpers/Helpers.h>

*//*
DEFINE_HOOK(0x419F2C, AircraftClass_MI_Enter_Opentopped, 0x5)
{
	GET(AircraftClass*, pThis, ESI);

	if (auto const pTarget = pThis->Destination)
	{
		CoordStruct DockingCoord = CoordStruct::Empty;

		if (pTarget->WhatAmI() == AbstractType::Building)
			DockingCoord = ((BuildingClass*)pTarget)->GetDockCoords(pThis);     //(v10->vftable->t.r.m.o.Docking_Coord)(v10, v28, this);
		else
			DockingCoord = ((TechnoClass*)pTarget)->GetCenterCoord();

		auto nDiff_X = DockingCoord.X - pThis->Location.X;
		auto nDiff_Y = DockingCoord.Y - pThis->Location.Y;

		if (nDiff_X <= 0)
		{
			if (nDiff_X < -5)
			{
				nDiff_X = -5;
			}
		}
		else if (nDiff_X > 5)
		{
			nDiff_X = 5;
		}

		if (nDiff_Y <= 0)
		{
			if (nDiff_Y < -5)
			{
				nDiff_Y = -5;
			}
		}
		else if (nDiff_Y > 5)
		{
			nDiff_Y = 5;
		}

		auto Res_X = nDiff_X + pThis->Location.X;
		auto Res_Y = nDiff_Y + pThis->Location.Y;
		auto Res_Z = pThis->Location.Z;
		pThis->SetLocation({ Res_X,Res_Y,Res_Z });

	}
	pThis->carrayall6D4 = true;
	R->EAX(true);
	return 0x0;
}

//DEFINE_HOOK(0x6FA4BC, TechnoClass_AI_Opentopped, 0x6)
//{}

DEFINE_HOOK(0x4169A2, AircraftClass_MI_Move_Opentopped, 0x7)
{
	GET(AircraftClass*, pThis, ESI);

	if (pThis->GetTechnoType()->OpenTopped && pThis->Passengers.NumPassengers > 0 && !pThis->Target)
	{
		auto iter = make_iterator(pThis->RadioLinks);
		if (!(iter.size() <=1))
		{
			pThis->ObjectClickedAction(Action::Enter, iter[0], false);
			return 0x4169C0;
		}
	}

	return 0x0;
}*/