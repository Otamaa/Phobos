
//DEFINE_POINTER_CALL(0x6D524F, &CellClass_DrawPlacement);
//DEFINE_POINTER_CALL(0x6D56F3, &CellClass_DrawPlacement);

/*
DEFINE_HOOK(0x6D53B9, BuildingClass_DrawPlacement_ShowbuildingImage, 0x9)
{
	GET_STACK(Point2D, nPoint, STACK_OFFS(0x54, 0x28));
	GET(BuildingClass*, pThis, ECX);
	auto pBound = &DSurface::ViewBounds();
	auto nConvert = pThis->GetRemapColour();

	DSurface::Temp()->DrawSHP(
	nConvert,
	pThis->Type->GetImage(),
	0,
	&nPoint,
	pBound,
	BlitterFlags::TransLucent50 | BlitterFlags::Plain | BlitterFlags::Centered, 0, 0, 0, 0, 0, nullptr, 0, 0, 0);

	return 0x0;
}

static void __fastcall DisplayClass_DrawPlacement(DisplayClass* pThis, void* _, CellStruct* pBuffer, CellStruct* pWhere)
{
	pThis->SetCursorPos(pBuffer, pWhere);

	auto pBuildingType = *reinterpret_cast<BuildingTypeClass**>(0x880990);
	auto pBuilding = *reinterpret_cast<BuildingClass**>(0x88098C);

	if (pBuildingType && pBuilding)
	{
		{
			Point2D nPoint = { pBuffer->X, pBuffer->Y };
			DSurface::Temp()->DrawSHP(
				pBuilding->GetRemapColour(),
				pBuildingType->GetImage(),
				0,
				&nPoint,
				&DSurface::ViewBounds(),
				BlitterFlags::TransLucent50 | BlitterFlags::Plain | BlitterFlags::Centered, 0, 0, 0, 0, 0, nullptr, 0, 0, 0);
		}
	}
}

DEFINE_POINTER_CALL(0x4FB8FE, &DisplayClass_DrawPlacement);
*/

//auto nChar = *reinterpret_cast<char**>(0x83E1AC);
//	auto pPalette = Generate_Color_Spread_Light_Convert(&nScheme->BaseColor, nullptr, &FileSystem::TEMPERAT_PAL(),nullptr, DSurface::Primary(),53,1000,1000,1000, nChar);

/*
DEFINE_HOOK(0x5052F7, HouseClass_FirstBuildableFromArray_ListEmpty_Retnullptr, 0x5)
{
	Debug::Log(__FUNCTION__" ! \n");
	return 0x0;
}

DEFINE_HOOK(0x5052F7, HouseClass_FirstBuildableFromArray_IndexOverflow_Retnullptr, 0x5)
{
	Debug::Log(__FUNCTION__" ! \n");
	return 0x0;
}

DEFINE_HOOK(0x41A125, AircraftClass_MissionEnter_Opentopped, 0x6)
{
	GET(AircraftClass*, pThis, ESI);

	if (auto nRadioContact = specific_cast<BuildingClass*>(pThis->GetRadioContact()))
	{
		if (pThis->IsAlive && !pThis->InLimbo && pThis->IsInAir())
		{
		}
	}
}

// not working ?
// something else already does this
DEFINE_HOOK(0x777CCA, WindowName_ApplyCustom, 0x5)
{

	R->ESP("Yuri's Revenge +P27");
	return 0x0;
}*/

/*
DEFINE_HOOK(0x514A65, HoverLocomotionClass_513D20_AnimUnderWater, 0xB)
{
	GET(HoverLocomotionClass* const, pLoco, ESI);

	auto const Linked = pLoco->Owner;
	auto const pType = Linked->GetTechnoType();
	auto const pExt = TechnoTypeExt::ExtMap.Find(pType);
	auto pAnimType = pExt->HoverType->GetAboveWaterAnim();

	if (pAnimType && (Linked->CloakState != CloakState::Cloaked || Linked->CloakState != CloakState::Cloaking))
	{
		auto nCoord = Linked->GetCenterCoords();
		if (auto pAnim = GameCreate<AnimClass>(pAnimType, nCoord))
			AnimExt::SetAnimOwnerHouseKind(pAnim, Linked->Owner, nullptr, AnimTypeExt::ExtMap.Find(pAnim->Type)->Anim_Owner.Get(), false);
	}

	return 0x514AC8;
}*/


/*
DEFINE_HOOK(0x513E14, HoverLocomotionClass_513D20_HoverBob, 0x6)
{
	R->ECX(Unsorted::CurrentFrame); //Uhh ...
	GET(HoverLocomotionClass* const, pLoco, ESI);

	auto const Linked = pLoco->Owner;
	auto const pType = Linked->GetTechnoType();
	auto const pHover = TechnoTypeExt::GetMyHover(pType);
	auto bob = pHover->GetBob();

	_asm fmul qword ptr[bob];

	return 0x513E20;
}*/
//#223
/*
DEFINE_HOOK(0x70D724, TechnoClass_FireDeathWeapon_Crit, 0x4)
{
	//Weapon that decided before this hook
	GET(WeaponTypeClass*, pDeathWeapon, EDI);
	GET(TechnoClass* const, pThis, ESI);

	auto const ThisExt = TechnoExt::ExtMap.Find(pThis);
	auto const TypeExt = TechnoTypeExt::ExtMap.Find(pThis->GetTechnoType());

	if (ThisExt->CritHitted && TypeExt->CritDeathWeapon.isset())
		pDeathWeapon = TypeExt->CritDeathWeapon.Get();

	R->EBP(R->EAX());
	R->EDI(pDeathWeapon);

	return pDeathWeapon ? 0x70D735 : 0x70D72A;
}*/


/* Ares hookn
DEFINE_HOOK(0x74A8FE, VoxelAnimClass_DamageArea, 0x6)
{
	enum { ContinueLoop = 0x74A929};

	GET(VoxelAnimClass* const, pThis, EAX);
	GET(ObjectClass*, pVictim, EDI);
	GET(int, nDistance, ECX);

	if (auto const pType = pThis->Type)
	{
		const auto& TypeExt = VoxelAnimTypeExt::ExtMap.Find(pType);

		if (auto pWeapon = TypeExt->Weapon.Get())
		{
			auto const nDamage = pType->Damage;
			auto const nCoord = pThis->Bounce.GetCoords();
			if (auto pBulletExt = BulletTypeExt::ExtMap.Find(pWeapon->Projectile))
			{
				if (auto pBullet = pBulletExt->CreateBullet(pVictim, nullptr, pWeapon, nDamage, true))
				{
					pBullet->SetLocation(nCoord);
					pBullet->Explode();
					pBullet->Limbo(); //Limbo
					pBullet->UnInit();
				}
			}
		}
		else
		{
			pVictim->ReceiveDamage(&pThis->Type->Damage, Game::AdjustForZ(nDistance), pThis->Type->Warhead, nullptr, false, false, pThis->OwnerHouse);
		}
	}

	return ContinueLoop;
}*/
/*
DEFINE_HOOK(0x73DDC0, UnitClass_Mi_Unload_DeployIntoPlaceAnywhere, 0x6)
{
	GET(UnitClass* const, pThis, ESI);

	if (R->AL())
	{
		if (pThis->Type->DeploysInto->PlaceAnywhere)
		{
			return 0x73DE20;
		}
		else
		{
			pThis->StopMoving();
			pThis->QueueMission(Mission::Harvest, false);
			pThis->NextMission();

			return 0x73DE3A;
		}
	}

	R->ECX(pThis);
	return  0x73DDC6;
}


//


DEFINE_HOOK(0x73F7DD, UnitClass_IsCellOccupied_Bib, 0x8)
{
	GET(UnitClass* const, pThis, ESI);
	GET(BuildingClass* const, pBuilding, EBX);

	return pThis && pThis->Owner->IsAlliedWith(pBuilding) ? 0x0 : 0x73F823;
}*/

/*
DEFINE_HOOK(0x71C3A0, TerrainClass_Draw_It_replace, 0x8)
{
	GET(TerrainClass*, pThis, ESI);
	auto shapeNum = 0;

	if (pThis->TimeToDie)
	{
		shapeNum = static_cast<int>(pThis->TimeToDie) + pThis->Animation.Value;
	}
	else
		if (auto mCurrentShape = TerrainExt::ExtMap.Find(pThis)->CurrentShape)
		{
			shapeNum = mCurrentShape;

			Debug::Log_Masselist(__FUNCTION__" Shape changed!");
		}

	R->Stack(0x10, shapeNum);

	return 0x71C3C3;
}

DEFINE_HOOK(0x71CD71, TerrainClass_Render_redraw, 0x6)
{
	GET(TerrainClass*, pThis, EDI);
	GET(RectangleStruct*, nRect, ESI);
	GET_STACK(Point2D, nPoint, 0x10);

	pThis->Draw(&nPoint, nRect);

	return 0x71CD81;
}

DEFINE_HOOK(0x71C2BC, TerrainClass_DrawIt_add_damagestate, 0x6)
{
	GET(TerrainTypeClass*, pType, EAX);
	GET(TerrainClass*, pThis, ESI);

	return(pType->IsAnimated || pThis->TimeToDie || TerrainExt::ExtMap.Find(pThis)->CurrentShape)
		? 0x71C2D0 : 0x71C2D7;
}

//Skip pTerrain->NeedsRedraw = false , set
//DEFINE_LJMP(0x6D98EF, 0x6D98F6);
//skip armor limitation
//DEFINE_LJMP(0x71C5D8,0x71C5E5)
DEFINE_HOOK(0x71C2C6, TerrainClass_DrawShape_addCheck, 0x6)
{
	GET(TerrainClass*, pThis, ESI);
	auto pTerreainTypeExt = TerrainTypeExt::ExtMap.Find(pThis->Type);

	R->CL(pThis->TimeToDie || pTerreainTypeExt->HaveDamagedState.Get());
	return 0x71C2C2;
}

DEFINE_HOOK(0x71C1F6, TerrainClass_DrawShape_replace, 0x8)
{
	GET(TerrainClass*, pThis, ESI);

	auto pTerreainTypeExt = TerrainTypeExt::ExtMap.Find(pThis->Type);

	auto nShapeNum = 0;
	if (pThis->Type->IsAnimated)
		nShapeNum = pThis->Animation.Value;
	else if (pThis->TimeToDie)
		nShapeNum = (int)(pThis->TimeToDie) + pThis->Animation.Value;
	else if (pThis->IsRedHP() && pTerreainTypeExt->HaveDamagedState.Get())
		nShapeNum = 1;

	R->EBX(nShapeNum);
	return 0x71C234;
}
*/

/*
DEFINE_HOOK(0x6D9823, TerrainClass_Redraw_CheckDamagedState, 0x6)
{
	enum { DontDraw = 0x6D98F6, Drawn = 0x6D9864 };
	GET(ObjectClass*, pThis, ESI);

	auto pTerrain = specific_cast<TerrainClass*>(pThis);

	if (!pTerrain || !pThis->IsAlive)
		return DontDraw;

	auto pType = pTerrain->Type;
	auto pTerreainTypeExt = TerrainTypeExt::ExtMap.Find(pTerrain->Type);
	//auto pTerrainExt = TerrainExt::ExtMap.Find(pTerrain);
	bool bIsDamage = pTerreainTypeExt->HaveDamagedState.Get();
	bool bDrawn = pType->SpawnsTiberium || pType->IsAnimated || pType->IsVeinhole || pType->IsFlammable || pTerrain->IsBurning || bIsDamage || pTerrain->TimeToDie;

	return bDrawn ? Drawn : DontDraw;
}

DEFINE_HOOK(0x71C73D, TerrainClass_AI_add, 0x8)
{
	GET(TerrainClass*, pThis, ESI);

	if (!pThis->Type->SpawnsTiberium
		|| !pThis->Type->IsFlammable
		|| !pThis->Type->IsVeinhole
		|| !pThis->Type->IsAnimated)
	{
		auto pTerrainExt = TerrainExt::ExtMap.Find(pThis);
		auto pTerreainTypeExt = TerrainTypeExt::ExtMap.Find(pThis->Type);
		if (!pTerrainExt->AlreadyUpdated && pThis->IsRedHP() && pTerreainTypeExt->HaveDamagedState.Get())
		{
			//Debug::Log_Masselist(__FUNCTION__" Executed ! , Tree[%s] ", pThis->Type->get_ID());
			pThis->UpdatePlacement(PlacementType::Redraw);
			pThis->NeedsRedraw = true;
			pTerrainExt->AlreadyUpdated = true;
		}
	}

	return 0;
}

#include <VeinholeMonsterClass.h>

DEFINE_HOOK(0x5F4FEF, ObjectClass_Put_Add, 0x7)
{
	enum { RequiresUpdate = 0x5F501B, NoUpdate = 0x5F5045 , ForceUpdate = 0x5F5038};

	GET(ObjectClass*, pThis, ESI);
	GET(ObjectTypeClass*, pType, EBX);

	if (!pType->IsLogic)
		return NoUpdate;

	// terrain only needs to get Update called when it spawns, or now when
	// it is flammable. if none is set, don't update
	if (auto pTerrainType = abstract_cast<TerrainTypeClass*>(pType))
	{
		auto pTerrain = abstract_cast<TerrainClass*>(pThis);
		auto pTerrainExt = TerrainExt::ExtMap.Find(pTerrain);
		auto pTerreainTypeExt = TerrainTypeExt::ExtMap.Find(pTerrainType);

	  return (pTerrainType->SpawnsTiberium || pTerrainType->IsFlammable
				|| pTerrainType->IsVeinhole || pTerrainType->IsAnimated
					//only update one that needed , skip others !
				|| (pTerreainTypeExt->HaveDamagedState.Get() && !pTerrainExt->AlreadyUpdated))

			 ? RequiresUpdate:NoUpdate;
	}

	if (auto pVeinMonster = abstract_cast<VeinholeMonsterClass*>(pThis))
	{
		R->ESI(pVeinMonster);
		return ForceUpdate;
	}

	return RequiresUpdate;
}*/

//==================================================================================================================================
/*
DEFINE_HOOK(0x469A69, BulletClass_Detonate_DamageArea_Owner, 0x7)
{
	GET(BulletClass*const, pThis, ESI);

	auto ownerHouse = HouseClass::FindCivilianSide();

	if (auto const BulletOwner = pThis->Owner ? pThis->Owner->Owner : nullptr)
		ownerHouse = BulletOwner;

	R->ECX(ownerHouse);

	return 0x469A75;
}*/

//crash ,it seems ares hooked here
DEFINE_HOOK(0x6F7294, TechnoClass_FireAt_OccupyRange, 0xB)
{
	//GET(TechnoClass* const, pThis, ESI);
	GET(int, nAddRange, EDI);
	Debug::Log(__FUNCTION__" Executed ! \n");
	auto nRange = RulesGlobal->OccupyWeaponRange;
	R->EDI(((nRange + nAddRange) << 8));
	return 0x6F72A2;
}

DEFINE_HOOK(0x6F9193, TechnoClass_GreatestThreat_OccupyRange, 0x6)
{
	//GET(TechnoClass* const, pThis, ESI);
	//GET(int, nAddRange, EAX);
	Debug::Log(__FUNCTION__" Executed ! \n");
	auto nRange = RulesGlobal->OccupyWeaponRange;
	R->EDX(&nRange); //lea stack eax
	return 0x6F919F;
}

/*
DEFINE_HOOK(0x7120E5, TechnoTypeClass_GetRepairCost_Building, 0x6)
{
	GET(TechnoTypeClass*, pThis, ESI);
	GET(RulesClass*, pRules, ECX);

	int val = pThis->Strength;

	auto nStep = pRules->RepairStep;
	if (auto pBuildingType = specific_cast<BuildingTypeClass*>(pThis))
		nStep = BuildingTypeExt::ExtMap.Find(pBuildingType)->RepairStep.Get(nStep);

	R->EAX(val / nStep);
	R->EDX(val % nStep);

	return 0x7120F2;
}*/

namespace Ares_TempAnim
{
	DWORD Exect(AnimClass* pThis)
	{
		int nDamageMult = 1;
		auto pOwner = pThis->OwnerObject;
		auto pType = pThis->Type;
		auto pTypeExt = AnimTypeExt::ExtMap.Find(pThis->Type);

		if (!pOwner || pOwner->WhatAmI() == AbstractType::Terrain)
			nDamageMult = 5;

		double dTempDamage = 1.0;
		double dDamageResult;
		auto nTypeDelay = pTypeExt->DamageDelay.Get();
		bool bDamageOccured = false;

		auto nCreateDmageFactor = [pOwner, pThis, pTypeExt, pType](bool bDamaged, double dDamageResultDouble, int nDamageResultInt, bool bImmedieteReturn = false)
		{
			if (bImmedieteReturn || nDamageResultInt <= 0 || pThis->IsPlaying)
				return 0x42465D;

			pThis->Damage = bDamaged ? dDamageResultDouble - static_cast<double>(nDamageResultInt) : 0.0;

			TechnoClass* pTechnoOwner = generic_cast<TechnoClass*>(pOwner);
			HouseClass* pHouseOwner = pThis->Owner ? pThis->Owner : (pTechnoOwner ? pTechnoOwner->GetOwningHouse() : nullptr);

			if (auto pWeapon = pTypeExt->AnimWeapon.Get())
			{
				if (auto pBulletExt = BulletTypeExt::ExtMap.Find(pWeapon->Projectile))
				{
					if (auto pBullet = pBulletExt->CreateBullet(pThis->GetCell(), nullptr, pWeapon, nDamageResultInt, true))
					{
						auto nCoord = pThis->GetCoords();
						pBullet->SetLocation(nCoord);
						pBullet->Explode(true);
						pBullet->Limbo(); //Limbo
						pBullet->UnInit();
					}
				}
			}
			else
			{
				if (auto pWarhead = pType->Warhead ? pType->Warhead : (_strcmpi(pType->get_ID(), "INVISO") ? RulesClass::Instance->FlameDamage2 : RulesClass::Instance->C4Warhead))
				{
					auto nCoords = pThis->GetCoords();
					MapClass::DamageArea(nCoords, nDamageResultInt, nullptr, pWarhead, pWarhead->Tiberium, pHouseOwner);
				}
			}

			return 0x42464C;
		};


		if (nTypeDelay <= 0 || pThis->Type->Damage < 1.0)
		{
			bDamageOccured = true;
			dDamageResult = nDamageMult * pThis->Type->Damage + pThis->Damage;
			pThis->Damage = dDamageResult;

			return !(dDamageResult < dTempDamage) ? nCreateDmageFactor(bDamageOccured, dDamageResult, (int)dDamageResult) : 0x42465D;
		}

		dDamageResult = pThis->Damage + 1.0;
		pThis->Damage = dDamageResult;

		return nCreateDmageFactor(bDamageOccured, dDamageResult, (int)dDamageResult, (dDamageResult < static_cast<double>(nTypeDelay)));
	}
};
/*
DEFINE_HOOK(0x424530, AnimClass_AI_DamageDelay, 0x6)
{
	enum { Delete = 0x424B42, ReturnMidPoint = 0x42465D, ReturnCheckActive = 0x42464C };

	if (R->AL())
		return ReturnCheckActive;
}*/
/*
//Mind that this value is measured in animation frames: depending on Rate,
//the animation might not advance to the next animation frame every game frame.

DEFINE_HOOK(0x424530, AnimClass_AI_DamageDelay, 0x6)
{
	enum {Delete = 0x424B42 , ReturnMidPoint = 0x42465D	, ReturnCheckActive= 0x42464C};

	if (R->AL())
		return ReturnCheckActive;

	GET(AnimClass*, pThis, ESI);
	auto pTypeExt = AnimTypeExt::ExtMap.Find(pThis->Type);
	auto pOwner = pThis->OwnerObject;

	auto nCreateDamagingFactor = [&pThis, &pTypeExt , &pOwner](int nDamage)
	{
		auto pType = pThis->Type;
		TechnoClass* pTechnoOwner = generic_cast<TechnoClass*>(pOwner);
		HouseClass* pHouseOwner = pThis->Owner ? pThis->Owner : (pTechnoOwner ? pTechnoOwner->Owner : nullptr);

		if (auto pWeapon = pTypeExt->AnimWeapon.Get())
		{
			if (auto pBulletExt = BulletTypeExt::ExtMap.Find(pWeapon->Projectile))
			{
				if (auto pBullet = pBulletExt->CreateBullet(pThis->GetCell(), pTechnoOwner, pWeapon, nDamage, true))
				{
					auto nCoord = pThis->GetCenterCoord_();
					pBullet->SetLocation(nCoord);
					pBullet->Explode(true);
					pBullet->Limbo(); //Limbo
					pBullet->UnInit();
				}
			}
		}
		else
		{
			if (auto pWarhead = pType->Warhead ? pType->Warhead : (strcmp(pType->get_ID(), "INVISO") ? RulesClass::Instance->FlameDamage2 : RulesClass::Instance->C4Warhead))
			{
				auto nCoords = pThis->GetCoords();
				MapClass::DamageArea(nCoords, nDamage, pTechnoOwner, pWarhead, pWarhead->Tiberium, pHouseOwner);
			}
		}
	};

	if (pTypeExt->DamageDelay.Get())
	{
		auto pAnimExt = AnimExt::ExtMap.Find(pThis);

		if(!pThis->IsPlaying)
			return ReturnCheckActive;

		if (!pAnimExt->DamageDelayTimer.GetTimeLeft())
		{
			pThis->Damage = pThis->Type->Damage;
			pAnimExt->DamageDelayTimer.Start(pTypeExt->DamageDelay.Get());
		}
		else
		{
			pThis->Damage = 0.0;
		}
	}
	else
	{
		pThis->Damage = pThis->Type->Damage;
	}

	if(pThis->Damage > 0.0)
	nCreateDamagingFactor((int)pThis->Damage);
	R->ESI(pThis);
	return ReturnCheckActive;
}*/

/* ares hook here
DEFINE_HOOK(0x4242CA, AnimClass_AI_AnimTrailer, 0x9)
{
	GET(AnimClass*, pThis, ESI);
	GET(AnimTypeClass*, pType, EAX);

	auto nTrailerSeparation = pType->TrailerSeperation;

	if (nTrailerSeparation == 1 || !(Unsorted::CurrentFrame % nTrailerSeparation))
	{
		auto nCoord = pThis->GetCenterCoord_();
		if (auto pAnim = GameCreate<AnimClass>(pType->TrailerAnim, nCoord, 1, 1, 0x600, 0, 0))
			AnimExt::SetAnimOwnerHouseKind(pAnim, pThis->Owner, nullptr, false);
	}

	return 0x424322;
}*/
