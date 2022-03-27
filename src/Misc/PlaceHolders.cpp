#include <Phobos.h>

/*
DEFINE_HOOK(0x6FD430 , TechnoClass_Laser_Zap_Set, 0x5)
{
	GET(LaserDrawClass*, pLaser, EAX);

	if (pLaser)
	{
		if (LaserDrawTemp::Firer)
			LaserDrawExt::WhoCalledMe.insert(LaserDrawTemp::Firer, pLaser);

		LaserDrawTemp::Firer = nullptr;

		R->EAX(pLaser);
		return 0x6FD434;
	}

	return 0x6FD446;
}
*/

/*
DEFINE_HOOK(0x6FD434, TechnoClass_Laser_Zap_Set, 0xA)
{
	//GET_STACK(WeaponTypeClass* const, pThis, STACK_OFFS(0x48, -0xC));
	GET(LaserDrawClass*, pLaser, EAX);

	if(LaserDrawTemp::Firer)
		LaserDrawExt::WhoCalledMe.insert(LaserDrawTemp::Firer, pLaser);

	LaserDrawTemp::Firer = nullptr;
	return 0x0;
}*/

struct OwnedWeed
{
	float IncreaseAmount(float Amount)
	{
		Storage += Amount;
	}

	float DecreaseAmount(float Amount)
	{
		if (Storage >= Amount)
			Storage = -Amount;
		else
		{
			float nBuff = Storage;
			Storage = -nBuff;
		}
		return Storage;
	}

	float Storage { 0.0f };
};
// #issue 149 : replace power used by power remain
// Failed cause Ares overtaken 70AA60 = TechnoClass_DrawExtraInfo, 6

///
/*
DEFINE_HOOK(0x43C0DB, BuildingClass_UpdateDamageFireAnims_Count, 0x6)
{
	GET(BuildingClass* const, pThis, ECX);
	GET(RulesClass*, pRules, EAX);

	auto const pExt = BuildingTypeExt::ExtMap.Find(pThis->Type);
	auto nFireType = pExt->DamageFireTypes.GetElements(pRules->DamageFireTypes);
	auto nSize = nFireType.size();

	R->EAX(Math::LessOrEqualTo((int)nSize, 8));
	return 0x43C0E1;
}

DEFINE_HOOK(0x43C1CC, BuildingClass_UpdateDamageFireAnims_Type, 0x6)
{
	GET_STACK(int, idxresult, 0x28);
	GET(BuildingClass* const, pThis, ESI);
	GET(RulesClass*, pRules, EDX);

	auto const pExt = BuildingTypeExt::ExtMap.Find(pThis->Type);
	auto nFireType = pExt->DamageFireTypes.GetElements(pRules->DamageFireTypes);

	R->ECX(nFireType.at(idxresult));
	return 0x43C1D9;
}
*/

namespace RadTimer
{
	bool AllowDamaging(TimerStruct& nTimer, int nDelay)
	{
		Debug::Log(__FUNCTION__"Executed ! nDelay [%d] /n", nDelay);
		if (!nTimer.GetTimeLeft())
		{
			nTimer.Start(nDelay);
			return true;
		}

		return false;
	}
}


//DEFINE_HOOK(0x70ABDD, TechnoClass_Draw_Stuff_When_Selected_PowerDisplay, 0x8)
//{
//	LEA_STACK(wchar_t*, pString, STACK_OFFS(0x15C, 0x100));
//	Debug::Log(__FUNCTION__ " string = %ls\n", pString);
//	R->EDX(pString);
//	R->ECX(R->lea_Stack<int>(STACK_OFFS(0x15C, 0x120)));
//
//	return 0x70ABE5;
//}

/*
DEFINE_HOOK(0x62D9BF, ParticleClass_SDDTOR, 0x5)
{
	GET(ParticleClass*, pThis, ESI);

	pThis->InLimbo = false;
	pThis->Remove();

	0x62D9CD;
}*/
/*
DEFINE_HOOK(0x44B63E, BuildingClass_Mi_Attack_AnimDelayedFire, 0x6)
{
	GET(BuildingClass*, pThis, ESI);
	GET(BuildingTypeClass*, pType, EAX);
	GET(int, nWeaponIndex, EBP);
	GET(int, nVal2, EDX);
	GET(int, nVal3, EAX);

	Debug::Log_WithBool(Temp::bDisableDebug, __FUNCTION__" Executed For [%s] \n", pThis->get_ID());

	pThis->PrismStage = PrismChargeState::Master;
	pThis->DelayBeforeFiring = pType->DelayedFireDelay;
	pThis->PrismTargetCoords.X = nWeaponIndex;
	pThis->PrismTargetCoords.Y = nVal2;
	pThis->PrismTargetCoords.Z = nVal3;
	pThis->DestroyNthAnim(BuildingAnimSlot::Active);

	if (!pType->Turret)
	{
		pThis->DestroyNthAnim(BuildingAnimSlot::Special);
		return 0x44B6D6;
	}

	if (pThis->Anims[(int)BuildingAnimSlot::SpecialThree])
		return 0x44B6C4;

	pThis->DestroyNthAnim(BuildingAnimSlot::Special);
	auto pAnim = pThis->Anims[(int)BuildingAnimSlot::Special];

	if (!pAnim)
		return 0x44B6D6;

	auto pAnimType = pAnim->Type;

	auto nHalfEnd = pAnimType->End / 2;
	auto nEnd = pAnimType->End;
	if (!pAnimType->Shadow)
		nHalfEnd = pAnimType->End;

	pThis->Anims[(int)BuildingAnimSlot::SuperFour]->Animation.Value = nHalfEnd - pAnim->Animation.Value;
	pThis->DestroyNthAnim(BuildingAnimSlot::SpecialTwo);

	return 0x44B6D6;
}*/


//
DEFINE_HOOK(0x62A854, ParasiteClass_UnInfect_BugFix, 0xE)
{
	GET(ParasiteClass*, pThis, ESI);

	if (pThis->Owner)
	{
		auto damage = pThis->Owner->Health * 4; //in case there is shield
		pThis->Owner->ReceiveDamage(&damage, 0, RulesClass::Instance->C4Warhead, nullptr, true, true, nullptr);
	}

	return 0x62A862;
}

/*
DEFINE_HOOK(0x692686, DisplayClass_DecideAction_Sensed, 0x6)
{
	GET(TechnoClass*, pTech, EDI);

	auto pRules = RulesExt::Global();

	return (pRules->ReplaceSensedBehaviourOnCampaign.Get()
		? pTech->Owner->IsPlayerControl() : pTech->IsHumanControlled)
		? 0x692690 : 0x6926DB;
}*/

namespace Draw
{
	int PipFrame(TechnoClass* pThis)
	{
		const auto strength = pThis->Type()->Strength;
		const auto nPipPoint = Point3D{ 0,0,0 };

		if (pThis->Health > RulesClass::Instance->ConditionYellow * strength && nPipPoint.X != -1)
			return nPipPoint.X;
		else if (pThis->Health > RulesClass::Instance->ConditionRed * strength && (nPipPoint.Y != -1 || nPipPoint.X != -1))
			return nPipPoint.Y == -1 ? nPipPoint.X : nPipPoint.Y;
		else if (nPipPoint.Z != -1 || nPipPoint.X != -1)
			return nPipPoint.Z == -1 ? nPipPoint.X : nPipPoint.Z;

		return 16;
	}

	int PipAmount(TechnoClass* pThis, int iLength)
	{
		auto nFirstVal = Game::F2I(pThis->GetHealthPercentage()) * iLength;
		auto nClamp = Math::clamp(nFirstVal, 0, iLength);

		return pThis->IsAlive
			? nClamp
			: 0;
	}
}

/* Broken , no!
DEFINE_HOOK(0x70A0C2, TechnoClass_DrawPipScale_Overload, 0x4)
{
	GET(TechnoClass*, pThis, EBP);
	GET(int, nMaxpip, EBX);
//	GET_STACK(int, nPipCount, 0x14);

	//add pip style code
	if (auto pManager = pThis->CaptureManager)
	{
		auto pShp = FileSystem::PIPS_SHP.get();

		GET_STACK(int, nLocX, 0x50);
		GET_STACK(int, nLocY, 0x54);
		GET_STACK(RectangleStruct*, pBound, 0x80);

		Point2D nPoint{ nLocX,nLocY };
		auto nFrame = 1;

		for (int i = 0; i < pManager->ControlNodes.Count; ++i)
		{
			DSurface::Temp->DrawSHP(FileSystem::PALETTE_PAL, pShp, nFrame, &nPoint, pBound, BlitterFlags(0x600), 0, 0, ZGradientDescIndex::Flat, 1000, 0, 0, 0, 0, 0);
			nFrame = !pManager->InfiniteMindControl || pManager->ControlNodes.Count <= pManager->MaxControlNodes ? 4 : 1;
			nPoint.Y += 4;
			nPoint.X += 2;
		}

		return 0x70A1DE;
	}

	R->EDI(0);
	return nMaxpip != 0 ? 7381196 : 7381340;
}*/

//73C68D = UnitClass_DrawAsSHP_IdleRate, 6 ; quite complex with some image swapping and loco check
/*
DEFINE_HOOK(0x70A3E5, TechnoClass_DrawPipScale_Ammo_Idx, 0x7)
{
	//GET(TechnoClass*, pThis, EBP);

	auto nPipIdx = 13;
	R->ESP(nPipIdx);
	return 0;
}

DEFINE_HOOK(0x70A35D, TechnoClass_DrawPipScale_Ammo, 0x5)
{
	GET(TechnoClass*, pThis, EBP);
	GET_STACK(RectangleStruct*, pRect, 0x20);
	GET_STACK(int, nPointX, 0x14);
	GET_STACK(int, nPointY, 0x15);

	SHPStruct* pShpFIle = FileSystem::PIPS_SHP.get(); //

	if (!pShpFIle)
		return 0;

	int nCustOffsetX = 0; //
	int nCustOffsetY = 0; //
	auto nAmmo = pThis->Ammo / pThis->Type()->Ammo;
	nPointX += nCustOffsetX;
	nPointY += nCustOffsetY;
	Point2D nPoint{ nPointX,nPointY };

	auto nFrame = nAmmo * pShpFIle->Frames;

	DSurface::Temp->DrawSHP(FileSystem::PALETTE_PAL, pShpFIle, nFrame, &nPoint, pRect, BlitterFlags(0x600), 0, 0, ZGradientDescIndex::Flat, 1000, 0, 0, 0, 0, 0);

	return 0x70A4EC;
}*/

/*
DEFINE_HOOK(0x54D0D0, JumpjetLocomotionClass_IsMovingNow, 0x7)
{
	GET_STACK(ILocomotion * const, pILoco, 0x4);
	auto const pLocomotor = static_cast<LocomotionClass*>(pILoco);

	GET_STACK(DWORD, nDWLoco, 0x4);

	DWORD dLinkSec = nDWLoco + static_cast<DWORD>(0x8);
	auto pLinked2 = *reinterpret_cast<FootClass **>(dLinkSec);
	auto pLinked = pLocomotor ? pLocomotor->LinkedTo : pLinked2;

	if (!pLinked->Deactivated)
	{
		DWORD dUnknown_A = nDWLoco + (pLocomotor ? static_cast<DWORD>(0x48) : static_cast<DWORD>(0x4C));
		auto nUnknownInt_A = *reinterpret_cast<int*>(dUnknown_A);
		R->EAX(nUnknownInt_A);

	}
	else
	{
		DWORD dUnknown_B = nDWLoco + (pLocomotor ? static_cast<DWORD>(0x4C) : static_cast<DWORD>(0x50));
		auto nUnknownInt_B = *reinterpret_cast<int*>(dUnknown_B);
		if (nUnknownInt_B == 3 || nUnknownInt_B == 1 || nUnknownInt_B == 4)
		{
			R->EAX(1);
		}
		else
		{
			R->EAX(0);
		}
	}

	return 0x54D0E2;
}*/


/*
namespace BulletClass_DrawVXL_Context
{
	Nullable<BulletClass*> pThisBullet {};
}

DEFINE_HOOK(0x46B19B, BulletClass_DrawVXL_FirersPalette_SetContext, 0x8)
{
	GET(BulletClass*, pThis, EAX);

	BulletClass_DrawVXL_Context::pThisBullet = pThis;

	return 0x0;
}

DEFINE_HOOK(0x46B1EC, BulletClass_DrawVXL_FirersPalette, 0xC)
{
	GET(BulletTypeClass*, pThisType, EDX);
	auto pBullet = BulletClass_DrawVXL_Context::pThisBullet;

	if (pThisType->FirersPalette && pBullet && !(pBullet->InheritedColor == -1))
	{
		R->ECX(pBullet->InheritedColor); //replace the index
		return 0x46B1F2;
	}

	BulletClass_DrawVXL_Context::pThisBullet.Reset();
	return 0x0;
}
*/

/*
DEFINE_HOOK(0x502A95, HouseClass_RegisterGain_RecheckTechTree, 0x6)
{
	//GET(TechnoClass*, pTech, EAX);
	GET(HouseClass*, pThis, EDI);

	pThis->RecheckTechTree = true;
	return 0;
}

DEFINE_HOOK(0x502606, HouseClass_RegisterLoss_RecheckTechTree, 0x6)
{
	//GET(TechnoClass*, pTech, EAX);
	GET(HouseClass*, pThis, EDI);

	pThis->RecheckTechTree = true;
	return 0;
}
*/
//DEFINE_LJMP(0x41A6B7, 0x41A6C4); //enable aircraft guard area
//DEFINE_LJMP(0x41A758, 0x41A761);

/*
DEFINE_HOOK(0x6622E0, RocketLocomotionClass_ILocomotion_Process_CustomMissile, 0x9)
{
	GET(AircraftClass*, pThis, ECX);

	R->EDX(pThis->Type);

	if (auto pExt = TechnoTypeExt::ExtMap.Find(pThis->Type))
	{
		if (pExt->IsCustomMissile)
		{
			R->EAX(&pExt->CustomMissileData);
			return 0x66230A;
		}
	}

	return 0;
}

DEFINE_HOOK(0x7C8E17, Game_operator_new, 0x6)
{
	GET_STACK(size_t, nSize, 0x4);
	GET_STACK(DWORD, nCaller, 0x0);

	Debug::Log("Game trying to alloc 0x%X , Caller 0x%X \n", nSize, nCaller);

	void* pPointer = operator new(nSize, std::nothrow);
	R->EAX(pPointer);
	return 0x7C8E24;
}

DEFINE_HOOK(0x7C8B3D, Game_operator_delete, 0x9)
{
	GET_STACK(void*, pMem, 0x4);
	GET_STACK(DWORD, nCaller, 0x0);

	Debug::Log("Game trying to free 0x%X , Caller 0x%X \n", sizeof(pMem), nCaller);

	YRMemory::Free(pMem);
	//delete[] pPointer;
	return 0x7C8B47;
}

*/
/*
namespace _tempFunc
{
	//6FF831 = TechnoClass_Fire_LimboLaunch_Timer, C
	void SetTimer(TechnoClass* pVictim , int nTimerDur)
	{
		TimerStruct nTempTimer;
		nTempTimer.Start(nTimerDur);
		LogicClass::Instance().AddObject(pVictim , false);
	}

	// return 0;
}*/


/*
DEFINE_HOOK(0x471A82, CaptureManagerClass_Overload_Handle, 0x6)
{
	GET(CaptureManagerClass*, pThis, ESI);

		const auto& nOverloadCount = RulesGlobal->OverloadCount;
		constexpr auto nGetItem = [](TypeList<int>& nType, int nCount)
		{
			int nRealSize = nType.Count;
			// if nCount is more than this TypeList count , just use maximum count
			// prevent crash , duh
			if (nCount > nRealSize)
				nCount = nRealSize;

			return nType[nCount];
		};

		bool bDoOverload = false;
		int nCount = 0;
		for (int i = 0; i < nOverloadCount.Count; ++i)
		{
			if (nOverloadCount[i] == pThis->ControlNodes.Count)
			{
				nCount = i;
				bDoOverload = true;
				break;
			}
		}

	R->Stack(0x10,nCount);
	pThis->OverloadDamageDelay = nGetItem(RulesGlobal->OverloadFrames, nCount);
	R->EAX(nGetItem(RulesGlobal->OverloadDamage, nCount));

	return 0x471AD0;
}
*/

/*
DEFINE_HOOK(0x736BF3, UnitClass_Update_TurretFacing, 8)
{
	GET(UnitClass*, pThis, ESI);

	auto pType = pThis->Type;
	bool bInMotion = false;

	//Debug::Log(__FUNCTION__" Exec For [%s] ! \n", pThis->get_ID());

	if (pType->JumpJet)
	{
		if (!pThis->Target && !pType->TurretSpins)
		{
			pThis->SecondaryFacing.set(pThis->PrimaryFacing.current());
		}
	}
	else
	{
		bInMotion = pThis->SecondaryFacing.IsRotating();
	}

	R->EAX(bInMotion);
	R->ECX(&pThis->SecondaryFacing);

	return 0x736BFE;
}
*/
//#include <Ext/ObjectType/Body.h>
struct Ares
{
	bool DamageOccupants(int nHealth, bool bSubjectToTrenches, WarheadTypeClass* pWarhead, AbstractClass* pTarget, TechnoClass* pOwner, HouseClass* pHouse)
	{
		if (!pWarhead || !pTarget)
			return false;

		auto const pBuilding = abstract_cast<BuildingClass*>(pTarget);

		if (!pBuilding)
			return false;

		auto const occupants = pBuilding->Occupants.Count;
		auto const pBldTypeExt = BuildingTypeExt::ExtMap.Find(pBuilding->Type);

		auto const passThrough = 0.20; //UCPassThrough
		auto const fatalRate = 0.20;//UCFatalRate

		if (!occupants || !passThrough || !pBldTypeExt)
			return false;

		auto& Random = ScenarioClass::Instance->Random;

		auto const idxPoorBastard = Random.RandomRanged(0, occupants - 1);
		auto const pPoorBastard = pBuilding->Occupants[idxPoorBastard];

		if (bSubjectToTrenches && Random.RandomDouble() >= passThrough)
			return false;

		// which Occupant is getting it?

		if (fatalRate > 0.0 && Random.RandomDouble() < fatalRate)
		{
			pPoorBastard->Destroyed(pOwner);
			pPoorBastard->UnInit();
			pBuilding->Occupants.RemoveItem(idxPoorBastard);
			pBuilding->UpdateThreatInCell(pBuilding->GetCell());
		}
		else
		{
			auto const multiplier = 0.20; //UCDamageMultiplier
			auto adjustedDamage = static_cast<int>(
				std::ceil(nHealth * multiplier));
			auto const result = pPoorBastard->ReceiveDamage(&adjustedDamage, 0,
				pWarhead, pOwner, false, true, pHouse);
		}

		// fix up the firing index, otherwise building stops to fire
		if (pBuilding->FiringOccupantIndex >= pBuilding->GetOccupantCount())
			pBuilding->FiringOccupantIndex = 0;

		// if the last occupant was killed and this building was raided,
		// it needs to be returned to its owner. (Bug #700)
		//auto const pBuildingExt = BuildingExt::ExtMap.Find(pBuilding);
		//evalRaidStatus(pBuildingExt);

		return true;
	}

	/*
	void evalRaidStatus(BuildingExt::ExtData* pData , bool IgnoreEva = true)
	{
		// if the building is still marked as raided, but unoccupied, return it to its previous owner
		if (pData->isCurrentlyRaided && !pData->OwnerObject()->Occupants.Count)
		{
			// Fix for #838: Only return the building to the previous owner if he hasn't been defeated
			if (!pData->OwnerBeforeRaid->Defeated)
			{
				pData->ignoreNextEVA = IgnoreEva; // #698 - used in BuildingClass_ChangeOwnership_TrenchEVA to override EVA announcement
				pData->OwnerObject()->SetOwningHouse(pData->OwnerBeforeRaid);
			}

			pData->OwnerBeforeRaid = nullptr;
			pData->isCurrentlyRaided = false;
		}
	}
	*/
};

/*
DEFINE_HOOK(0x4387A8, BombClass_Detonate_CreateExplosiveAnim, 0x6)
{
	GET(BombClass*, pThis, ESI);
	GET_STACK(CoordStruct, nCoord, STACK_OFFS(0x38, 0x10));
	// hopefully BombClass Warhead still stored here , lets see
	GET_STACK(WarheadTypeClass*, pWarhead, STACK_OFFS(0x38, 0x4));
	// also the damage
	GET(int, nDamage, EDX);

	auto nLandType = Map[nCoord]->LandType;
	if (auto pAnimType = Map.SelectDamageAnimation(nDamage, pWarhead, nLandType, nCoord))
	{
		if (auto pAnim = GameCreate<AnimClass>(pAnimType, nCoord, 0, 1, 0x2600, -15, false))
		{
			_AnimClass_SetOwner_Bomb(pAnim, pThis);
			Map.DamageArea()
		}
	}

	return 0x438857;
}*/

/*
DEFINE_HOOK(0x438857, BombClass_Detonate_Anim_setOwner, 0x5)
{
	// why ? , EAX is not guarantee to be AnimClass*
	// casting it to make sure we got correct pointer !
	GET(AbstractClass*, pPointer, EAX);
	GET(BombClass*, pThis, ESI);

	if (auto pAnim = specific_cast<AnimClass*>(pPointer))
	{
		_AnimClass_SetOwner_Bomb(pAnim, pThis);
	}

	return 0;
}

FUNC_STATIC(AnimClass*, _BombClass_Detonate_AnimCtor,
AnimClass* pThis,
void* _,
AnimTypeClass* pType,
CoordStruct* pCoord,
int LoopDelay,
int LoopCount,
DWORD flags,
int ForceZAdjust,
bool reverse
)
{
	if (pType)
	{
		GET_REGISTER_STATIC_TYPE(BombClass*, pThisBomb, esi);
		auto pAnim = _FunctionPadded::AnimClass_Ctor_Wrapper(
			pThis, _, pType, pCoord, LoopDelay,
			LoopCount, flags, ForceZAdjust, reverse);

		_AnimClass_SetOwner_Bomb(pAnim, pThisBomb);

		return pAnim;
	}
	else
	{
		//GameDelete(pThis);
		return nullptr;
	}
}
*/


//StandFrameRate int
/*
DEFINE_HOOK(0x73C6FE, UnitClass_Draw_StandFrameRate_test, 0x6)
{
	GET(UnitTypeClass*, pThisType, ECX);
	GET(int, nFrameNumber, EAX);

	const auto& pTypeExt = TechnoTypeExt::ExtMap.Find(pThisType);
	Debug::Log(__FUNCTION__"Executed For [%s] \n", pThisType->get_ID());

	int nFrame = 0;
	if (pTypeExt->StandFrameRate.isset())
	{
		auto nRate = Math::max(1, pTypeExt->StandFrameRate.Get());
		nFrame = nFrameNumber / nRate;
	}
	else
		nFrame = nFrameNumber;

	R->EBX(nFrame);
	return 0x73C700;
}*/

/*
if (!VeinholeMonsterClass::Array()->empty())
{
	for (auto const& pVeinhole : *VeinholeMonsterClass::Array())
	{
		if (pVeinhole)
		{
			auto nCoord = WWMouse->GetCoordsUnderCursor();
			auto nCell = Map.TryGetCellAt(nCoord);

			Debug::Log_Masselist("Vein[%x] str[%d] With OverlayIDx[%d] CurArray[%d,%d] data [%d]", pVeinhole, pVeinhole->Health, nCell->OverlayTypeIndex, nCell->MapCoords.X, nCell->MapCoords.Y, (int)nCell->Powerup);
		}
	}
}*/


/*
//no
DEFINE_HOOK(0x444DC9, BuildingClass_KickOutUnit_Barracks, 0x9)
{
	enum { KickOut_Succeeded = 0x444971 };

	GET(TechnoClass* const, pProduct, EDI);
	GET(RadioCommand, nRadio, EAX);
	GET(BuildingClass* const, pThis, ESI);

	if (nRadio == RadioCommand::AnswerPositive)
	{
		pThis->SendCommand(RadioCommand::RequestUnload, pProduct);
		if (auto pFocus = pProduct->Focus) //Focus is Waypoint
		{
			pProduct->SetDestination(pFocus, 1);
			return KickOut_Succeeded;
		}

		pProduct->Scatter(CoordStruct::Empty, true, false);
	}

	return KickOut_Succeeded;
}

DEFINE_HOOK(0x447E90, BuildingClass_GetDestinationCoord_Helipad, 0x6)
{
	GET(BuildingClass*, pThis, ECX);
	GET_STACK(CoordStruct*, pCoordRet, 0x4);
	GET_STACK(TechnoClass*, pDocker, 0x8);

	auto pType = pThis->Type;
	auto nCoord = CoordStruct::Empty;

	if (pType->Helipad)
	{
		pThis->GetDockCoords(&nCoord, pDocker);
		nCoord.Z = 0;

	}
	else if (pType->UnitRepair || pType->Bunker)
		pThis->GetDockCoords(&nCoord, pDocker);
	else
		pThis->GetCoords(&nCoord);

	pCoordRet = &nCoord;
	R->EAX(&nCoord);
	return 0x447F06;
}
*/

/*	broken , appearing in random cell
*/
namespace Shrapnel
{
	void CreateBullet(WeaponTypeClass* pWeapon, CoordStruct nCoord, AbstractClass* pTarget, TechnoClass* pOwner)
	{
		if (!pWeapon || !pWeapon->Projectile || !pWeapon->Warhead || !pTarget)
			return;

		if (auto pBulletCreated = pWeapon->Projectile->CreateBullet(pTarget, pOwner, pWeapon->Damage, pWeapon->Warhead, pWeapon->Speed, pWeapon->Bright))
		{
			pBulletCreated->SetWeaponType(pWeapon);

			VelocityClass nVelResult { 0.0,0.0,0.0 };
			auto nDegRad = Math::DEG90_AS_RAD;
			auto nBinaryMagic = Math::BINARY_ANGLE_MAGIC_VALUE;

			auto nDest = nCoord;
			auto nTargetCoord = pTarget->GetCoords();

			auto nDistance = Math::atan2((double)(nDest.Y - nTargetCoord.Y), (double)(nDest.X - nTargetCoord.X));

			if (nVelResult.X == 0.0 && nVelResult.Y == 0.0)
				nVelResult.X = 100.0;

			auto nMagnitude = nVelResult.MagnitudeXY();

			auto nDegree = ((nDistance - nDegRad) * nBinaryMagic) - 0x3FFF * -0.00009587672516830327;

			nVelResult.X = Math::cos(nDegree) * nMagnitude;
			nVelResult.Y = Math::sin(nDegree) * nMagnitude;

			nMagnitude = nVelResult.MagnitudeXY();

			auto nVelocity_Z = Math::atan2(nVelResult.Z, nMagnitude);
			nVelocity_Z = ((nVelocity_Z - nDegRad) * nBinaryMagic) - 0x3FFF * -0.00009587672516830327;;

			nMagnitude = nVelResult.MagnitudeXY();

			if (nVelocity_Z != 0.0)
			{
				nVelResult.X = nVelResult.X / Math::cos(nVelocity_Z);
				nVelResult.Y = nVelResult.Y / Math::cos(nVelocity_Z);
			}

			nVelResult.X = Math::cos(0.7853262558535721) * nVelResult.X;
			nVelResult.Y = Math::cos(0.7853262558535721) * nVelResult.Y;
			nVelResult.Z = Math::sin(0.7853262558535721) * nVelocity_Z;

			auto nSpeed = pWeapon->Speed;
			if (nVelResult == VelocityClass::Empty)
				nVelResult.X = 100.0;

			nMagnitude = nVelResult.DistanceXY();
			auto nSpeedDevided = nSpeed / nMagnitude;

			nVelResult.X = nSpeedDevided * nVelResult.X;
			nVelResult.Y = nVelResult.Y * nSpeedDevided;
			nVelResult.Z = nVelocity_Z * nSpeedDevided;

			pBulletCreated->MoveTo(nDest, nVelResult);

			//laser and bolt affects

			if (pWeapon->IsLaser && pOwner)
				pOwner->CreateLaser(pTarget, 0, pWeapon, nDest);

			if (pWeapon->IsElectricBolt && pOwner)
				pOwner->CreateEbolt(pTarget, pWeapon, nDest);
		}
	}

	void Exec(BulletClass* pThis)
	{
		auto pType = pThis->Type;
		auto nShrapnelCount = pType->ShrapnelCount;
		int nShrapnelCountResult = 0;
		CellStruct nBulletCellstruct;

		if (nShrapnelCount >= 0)
		{
			nShrapnelCountResult = nShrapnelCount;
		}
		else
		{
			if (auto pOwner = pThis->Owner)
			{
				nShrapnelCountResult = -nShrapnelCount - (pThis->Location - pOwner->GetCoords()).MagnitudeInt();

				if (nShrapnelCount <= 0)
					return;
			}
			else
			{
				nShrapnelCountResult = 3;
			}
		}

		if (nShrapnelCountResult <= 0)
			return;

		if (auto pObject = pThis->GetCell()->FirstObject)
		{
			if (pObject->WhatAmI() != AbstractType::Building)
			{
				auto pShrapnelWeapn = pType->ShrapnelWeapon;
				auto nShrapnelWeapon_Range_Lepton = pShrapnelWeapn->Range / 256;
				nBulletCellstruct = pThis->GetMapCoords();
				int nCounter_2 = 0;
				int nCounter_3 = 1;

				if (nShrapnelWeapon_Range_Lepton >= 1)
				{
					int nInitialValue = 9;
					do
					{
						auto nInitialValie_substractbyfour = nInitialValue - 4;

						if (nInitialValie_substractbyfour < nInitialValue)
						{
							auto nCellDirection = &CellSpread::GetCell(nInitialValie_substractbyfour);
							do
							{
								auto nCellStructResult = nBulletCellstruct + *nCellDirection;
								if (auto pObject_FornCellStructResult = Map[nCellStructResult]->FirstObject)
								{
									auto pTarget_1 = pThis->Owner;
									if (pObject_FornCellStructResult != pTarget_1
										&& pTarget_1
										&& !pThis->Owner->Owner->IsAlliedWith(pObject_FornCellStructResult)
										)
									{
										CreateBullet(pShrapnelWeapn, pThis->Location, pObject_FornCellStructResult, pThis->Owner);
										if (++nCounter_2 == nShrapnelCountResult)
											return;
									}
								}
								++nCellDirection;

							}
							while (++nInitialValie_substractbyfour < nInitialValue);

						}
						nInitialValue += 4;
						++nCounter_3;
					}
					while (nCounter_3 <= nShrapnelWeapon_Range_Lepton);
				}

				if (nCounter_2 < nShrapnelCountResult)
				{
					auto nShrapLeft = nShrapnelCountResult - nCounter_2;
					do
					{
						nBulletCellstruct = pThis->GetMapCoords();
						auto nRandom_1 = ScenarioGlobal->Random(-2, 2);
						auto nRandom_2 = ScenarioGlobal->Random(-2, 2);
						nBulletCellstruct.X += nRandom_1;
						nBulletCellstruct.Y += nRandom_2;
						CreateBullet(pShrapnelWeapn, pThis->Location, Map[nBulletCellstruct], pThis->Owner);

					}
					while (--nShrapLeft);
				}
			}
		}
	}
};

/* creating random bullet somewhere , idk
* borked
DEFINE_HOOK(0x469A4F, BulletClass_Shrapnel_Override, 0x7)
{
	GET(BulletClass*, pThis, ESI);

	//Shrapnel::Exec_(pThis);
	Shrapnel::Exec(pThis);

	return 0x469A56;
}*/

/*
namespace HandleRemove
{
	void Bullet(BulletExt::ExtData* pData, VelocityClass bVel, CoordStruct nCoords)
	{
		if (pData)
		{
			if (const auto& pBullet = pData->OwnerObject())
			{
				pBullet->Velocity = bVel;
				pBullet->SourceCoords = nCoords;
				pBullet->LastMapCoords = { (short)(nCoords.X / 256) , (short)(nCoords.Y / 256) };
				DisplayClass::Instance->Remove(pBullet);
			}
		}
	}
};
*/

/*
DEFINE_HOOK(0x6FE5C3, TechnoClass_FireAt_ForceBulletInacurateWhenMoving, 0x6)
{
	GET(TechnoTypeClass*, pType, EAX);

	const auto& pTypeExt = TechnoTypeExt::ExtMap.Find(pType);
	R->CL(pTypeExt->InaccurateWhenMoving.Get(pType->JumpJet));

	return 0x6FE5C9;
}*/

/*
//here start
namespace Taunt
{
	void Execute(TechnoClass* pAttacker, TechnoClass* pVictim)
	{
		if (pAttacker && pVictim)
		{
			if (pVictim->Berzerk || pVictim->BerzerkDurationLeft)
				return;

			bool bCanFire = false;
			int nWeaponCount = Math::max(2, pVictim->GetTechnoType()->WeaponCount);

			for (int i = 0; i < nWeaponCount; ++i)
			{
				auto nFireError = pVictim->GetFireErrorWithoutRange(pAttacker, i);
				bCanFire = nFireError != FireError::ILLEGAL && nFireError != FireError::CANT;
			}

			// toDo : check other state ?
			if (bCanFire)
				pVictim->SetTarget(pAttacker);
		}
	}
};

namespace Temp_a
{
	ValueableVector<AnimTypeClass*> DeployAnims;

	void HandleDeployAnim(UnitClass* pFoot)
	{
		auto pDeployANim = pFoot->DeployAnim;
		bool SomeExtCheck_isDeploying = true;
		auto nCoords = pFoot->GetCoords();

		if (pDeployANim && pFoot->Undeploying)
		{
			if (pFoot->Animation.Value >= pDeployANim->Type->End + pDeployANim->Type->Start - 2)
			{
				SomeExtCheck_isDeploying = false;
				pFoot->Undeploying = false;
			}
		}
		else
		{
			//auto pTypeExt = TechnoTypeExt::ExtMap.Find(pFoot->Type);

			if (auto pAninType = GeneralUtils::GetAnimTypeFromFacing(pFoot, DeployAnims))
			{
				if (auto pAnim = GameCreate<AnimClass>(pAninType, nCoords))
				{
					pFoot->DeployAnim = pAnim;
					pAnim->SetOwnerObject(pFoot);
					pFoot->DeployAnim->LightConvert = pFoot->GetRemapColour();
				}
			}

			SomeExtCheck_isDeploying = false;

			int nDeploySound = pFoot->Type->DeploySound;
			if (nDeploySound != -1)
				VocClass::PlayAt(nDeploySound, nCoords);

			pFoot->Animation.Value = pFoot->DeployAnim->Type->Start;
			pFoot->Animation.Start(pFoot->DeployAnim->Type->Rate);
			pFoot->Undeploying = true;
		}

		if (pFoot->IsDisguised())
			pFoot->ClearDisguise();

		int nUnDeploySound = pFoot->Type->UndeploySound;
		if (nUnDeploySound != -1)
			VocClass::PlayAt(nUnDeploySound, nCoords);

	}

	// didnt made this pattern scan - c+p'd from somewhere
	std::vector<int> pattern_to_byte(const char* pattern)
	{
		auto bytes = std::vector<int> {};
		auto start = const_cast<char*>(pattern);
		auto end = const_cast<char*>(pattern) + strlen(pattern);

		for (auto current = start; current < end; ++current)
		{
			if (*current == '?')
			{
				++current;
				if (*current == '?')
					++current;
				bytes.push_back(-1);
			}
			else
			{
				bytes.push_back(strtoul(current, &current, 16));
			}
		}

		return bytes;
	};
};

namespace TempPromotion
{
	void UpgradeVeterancyOnceTime(TechnoClass* pThis)
	{
		if (auto pFoot = generic_cast<FootClass*>(pThis))
		{
			//test
			HouseClass::CreateAnim(pThis->Owner, specific_cast<CellClass*>(pThis->Target), nullptr, "MONEY");

			auto pThisExt = TechnoExt::ExtMap.Find(pThis);
			if (!pThisExt->VeterancyUpgreaded && pThis->GetTechnoType()->Passengers > 0)
			{
				for (NextObject object(pThis->Passengers.GetFirstPassenger()); object; ++object)
				{
					if (auto pInf = specific_cast<InfantryClass*>(*object))
					{
						auto pTechnoTypeExtInf = TechnoTypeExt::ExtMap.Find(pInf->Type);
						if (pTechnoTypeExtInf->IsGeneralPilot && (pInf->Owner == pThis->Owner))
						{
							if (pInf->Veterancy.IsElite() && !pThis->Veterancy.IsElite())
							{
								pThis->Veterancy.SetElite();
								pThisExt->VeterancyUpgreaded = true;
								break;
							}
							else if (pInf->Veterancy.IsVeteran() && !pThis->Veterancy.IsVeteran())
							{
								pThis->Veterancy.SetVeteran();
								pThisExt->VeterancyUpgreaded = true;
								break;
							}
						}
					}
				}
			}
		}
	}
}
*/


namespace MeteorShower
{
	void Execute(AnimTypeClass* pLargeMeteor, AnimTypeClass* pSmallMeteor, CoordStruct nCoords, HouseClass* pOwner = nullptr, HouseClass* pVictim = nullptr, int nSwapMeteorChance = 100, int nSpawnMin = 1, int nSpawnMax = 2, double nDistanceMin = 0.0, double nDistanceMax = 1.0)
	{
		auto pCell = MapClass::Instance->TryGetCellAt(nCoords);

		if (!pCell)
			return;

		auto pLargeMeteorDecided = pLargeMeteor ? pLargeMeteor : AnimTypeClass::FindOrAllocate("METLARGE");
		auto pSmallMeteorDecided = pSmallMeteor ? pSmallMeteor : AnimTypeClass::FindOrAllocate("METSMALL");

		auto nCoords_ = nCoords;
		nCoords_.Z = MapClass::Instance->GetCellFloorHeight(nCoords);

		if (pCell->ContainsBridge() || pCell->ContainsSlide())
			nCoords_.Z += 416;

		auto nValue = ScenarioClass::Instance->Random(nSpawnMin, nSpawnMax);
		auto dMin = Leptons(nDistanceMin);
		auto dMax = Leptons(nDistanceMax);

		if (nValue > 0)
		{
			for (int i = 0; i < nValue; --i)
			{
				auto nDistance = ScenarioGlobal->Random(dMin, dMax); //Distance
				auto nRandomCoords = Map.GetRandomCoordsNear(nCoords_, nDistance, ScenarioClass::Instance->Random(1));
				auto nRandomX = ScenarioGlobal->Random() % i;
				auto nRandomY = ScenarioGlobal->Random() % i;
				CoordStruct nCoordBuffer { nRandomX + nRandomCoords.X ,nRandomY + nRandomCoords.Y ,nRandomCoords.Z };
				AnimTypeClass* pSpawnedMeteor = pSmallMeteorDecided;

				if (ScenarioGlobal->Random(99) < nSwapMeteorChance)
					pSpawnedMeteor = pLargeMeteorDecided;

				if (pSpawnedMeteor)
					if (auto pAnim = GameCreate<AnimClass>(pSpawnedMeteor, nCoordBuffer))
						AnimExt::SetAnimOwnerHouseKind(pAnim, pOwner, pVictim, false);

			}
		}
	}
}
