#include "Hooks.Otamaa.h"

DEFINE_HOOK(0x52BA78, _YR_GameInit_Pre, 0x5)
{
	//	MouseCursorTypeClass::AddDefaults();
	return 0;
}

DEFINE_HOOK(0x6BE1C2, _YR_ProgramEnd, 0x8)
{
	return 0x0;
}

DEFINE_HOOK(0x505F6C, HouseClass_GenerateAIBuildList_AIBuildInstead, 0x6)
{
	GET(HouseClass*, pHouse, ESI);

	if (!pHouse->ControlledByHuman() && !pHouse->IsNeutral())
	{
		for (auto& nNodes : pHouse->Base.BaseNodes)
		{
			auto nIdx = nNodes.BuildingTypeIndex;
			if (nIdx >= 0)
			{
				auto const& pBldTypeExt = BuildingTypeExt::ExtMap.Find(BuildingTypeClass::Array->GetItem(nIdx));

				if (!pBldTypeExt->AnotherTypeData.AIBuildInsteadPerDiff.empty() && pBldTypeExt->AnotherTypeData.AIBuildInsteadPerDiff.at(pHouse->GetCorrectAIDifficultyIndex()) != -1)
					nIdx = pBldTypeExt->AnotherTypeData.AIBuildInsteadPerDiff.at(pHouse->GetCorrectAIDifficultyIndex());

				nNodes.BuildingTypeIndex = nIdx;
			}
		}
	}

	return 0;
}

DEFINE_HOOK(0x5D736E, MultiplayGameMode_GenerateInitForces, 0x6)
{
	return (R->EAX<int>() > 0) ? 0x0 : 0x5D743E;
}

namespace DrawHeathData
{
	void DrawNumber(TechnoClass* const pThis, Point2D* pLocation, RectangleStruct* pBounds)
	{
		auto const pTypeExt = TechnoTypeExt::ExtMap.Find(pThis->GetTechnoType());

		if (!pTypeExt->AnotherData.HealthNumber_Show.Get() || !pThis->IsAlive)
			return;

		auto const pShapeData = pTypeExt->AnotherData.HealthNumber_SHP.Get(nullptr);

		if (!pShapeData)
			return;

		SHPStruct* const pShpGreen = pShapeData->GetOrDefaultShape(nullptr);

		if (!pShpGreen)
			return;

		Point2D nCurrentDistance { 0,0 };
		auto const nLocation = *pLocation;
		auto const pType = pThis->GetTechnoType();
		int XOffset = 0;
		int	YOffset = 0;

		auto GetFrame = [pThis, pShpGreen](char nInput)
		{
			int nFrameResult = -1;
			int nInputToFrame = -1;

			switch (nInput)
			{
			case (*" "):
				nInputToFrame = 12;//blank frame
				break;
			case (*"%"):
				nInputToFrame = 10;
				break;
			case (*"/"):
				nInputToFrame = 11;
				break;
			default:
				nInputToFrame = nInput - 48;
				break;
			}

			int const nFrame_Total = Math::clamp((int)pShpGreen->Frames, 12 + 1, 36 + 1);
			nFrameResult = nInputToFrame;

			// blank frames on the end (+1) !
			// 0  1  2  3  4  5  6  7  8  9 10 11

			// 12 13 14 15 16 17 18 19 20 21 22	23
			if (nFrame_Total == 25 && (pThis->IsYellowHP() || pThis->IsRedHP()))
			{
				nFrameResult = nInputToFrame + 12;
			}
			// 24 25 26 27 28 29 30 31 32 33 34 35
			else if (nFrame_Total == 37)
			{
				if (!pThis->IsGreenHP())
				{
					if (!(nInputToFrame == 12) && pThis->IsYellowHP())
						nFrameResult = nInputToFrame + 12;

					nFrameResult = nInputToFrame + 24;
				}
			}

			return nFrameResult;
		};

		char nBuffer[256];

		if (!pTypeExt->AnotherData.HealthNumber_Percent.Get())
			_snprintf_s(nBuffer, _TRUNCATE, "%d/%d", pThis->Health, pThis->GetTechnoType()->Strength);
		else
			_snprintf_s(nBuffer, _TRUNCATE, "%d%s", (int)(pThis->GetHealthPercentage() * 100.0), "%");

		auto const bIsBuilding = pThis->WhatAmI() == AbstractType::Building;

		{
			// coord calculation is not really right !
			if (auto const pBuilding = specific_cast<BuildingClass*>(pThis))
			{
				auto const pBldType = pBuilding->Type;
				CoordStruct nDimension { 0,0,0 };
				auto const nLocTemp = nLocation;
				pBldType->Dimension2(&nDimension);
				CoordStruct nDimension2 { -nDimension.X / 2,nDimension.Y / 2,nDimension.Z / 2 };
				Point2D nDest;
				TacticalGlobal->CoordsToScreen(&nDest, &nDimension2);

				XOffset = nDest.X + nLocTemp.X + pTypeExt->AnotherData.Healnumber_Offset.Get().X + 2;
				YOffset = nDest.Y + nLocTemp.Y + pTypeExt->AnotherData.Healnumber_Offset.Get().Y + pType->PixelSelectionBracketDelta;
			}
			else
			{
				XOffset = nLocation.X + pTypeExt->AnotherData.Healnumber_Offset.Get().X + 2 - 15;
				YOffset = nLocation.Y + pTypeExt->AnotherData.Healnumber_Offset.Get().Y + pType->PixelSelectionBracketDelta - 33;
			}
		}

		Point2D nDistanceFactor = pTypeExt->AnotherData.Healnumber_Decrement.Get(bIsBuilding ? Point2D { 10,-5 } : Point2D { 5 ,0 });

		for (auto const& nCurrentData : nBuffer)
		{
			if (nCurrentData == *"\0")
				break;
			Point2D nOffset { nCurrentDistance.X + XOffset,YOffset + nCurrentDistance.Y };

			{
				auto const nFrameIndex = GetFrame(nCurrentData);

				if ((nFrameIndex >= 0) && Phobos::Config::ShowHealthPercentEnabled)
				{
					DSurface::Temp->DrawSHP
					(FileSystem::PALETTE_PAL,
						pShpGreen,
						nFrameIndex,
						&nOffset,
						pBounds,
						BlitterFlags(0x600),
						0,
						0,
						ZGradient::Ground,
						1000,
						0,
						0,
						0,
						0,
						0
					);
				}
			}

			nCurrentDistance.Y += nDistanceFactor.Y;
			nCurrentDistance.X += nDistanceFactor.X;
		}
	}

	void DrawBar(TechnoClass* pThis, Point2D* pLocation, RectangleStruct* pBound)
	{
		auto const pTypeExt = TechnoTypeExt::ExtMap.Find(pThis->GetTechnoType());
		auto pPipsShape = FileSystem::PIPS_SHP();
		auto pPipsShapeSelected = FileSystem::PIPBRD_SHP();
		auto const pShapeData = pTypeExt->AnotherData.HealthBarSHP.Get(nullptr);
		auto const pSelectedShapeData = pTypeExt->AnotherData.HealthBarSHP_Selected.Get(nullptr);

		if (pShapeData)
			pPipsShape = pShapeData->GetOrDefaultShape(pPipsShape);

		if (pSelectedShapeData)
			pPipsShapeSelected = pSelectedShapeData->GetOrDefaultShape(pPipsShapeSelected);

		ConvertClass* pPalette = pTypeExt->AnotherData.HealthbarRemap.Get() ? pThis->GetRemapColour() :
			pTypeExt->AnotherData.HealthBarSHP_Palette.GetOrDefaultConvert(FileSystem::PALETTE_PAL());

		Point2D nLocation = *pLocation;
		nLocation += pTypeExt->AnotherData.HealthBarSHP_PointOffset.Get();
		Point2D nPoint { nLocation.X,nLocation.Y };
		bool bIsInfantry = pThis->WhatAmI() == AbstractType::Infantry;
		int nBracketDelta = pThis->GetTechnoType()->PixelSelectionBracketDelta + pTypeExt->AnotherData.HealthBarSHPBracketOffset.Get();

		if (pThis->IsSelected)
		{
			nPoint.X += (bIsInfantry ? 11 : 1);
			nPoint.Y += nBracketDelta - (bIsInfantry ? 25 : 26);

			DSurface::Temp->DrawSHP(pPalette, pPipsShapeSelected, (bIsInfantry ? 1 : 0), &nPoint, pBound, BlitterFlags(0xE00), 0, 0, ZGradient::Ground, 1000, 0, 0, 0, 0, 0);
		}

		int const nOffsetX = (bIsInfantry ? -5 : -15);
		int const nLength = (bIsInfantry ? 8 : 17);
		int const nYDelta = nBracketDelta - (bIsInfantry ? 24 : 25);
		int const nDraw = pThis->IsAlive ? Math::clamp((int)(round(pThis->GetHealthPercentage() * nLength)), 1, nLength) : 0;
		CoordStruct const nHealthFrame = pTypeExt->AnotherData.HealthBarSHP_HealthFrame.Get();
		int nHealthFrameResult = nHealthFrame.Y; //Green

		if (pThis->IsYellowHP())
			nHealthFrameResult = nHealthFrame.Z; //Yellow

		if (pThis->IsRedHP() || pThis->Health <= 0 || !pThis->IsAlive)
			nHealthFrameResult = nHealthFrame.X;//Red

		for (int i = 0; i < nDraw; ++i)
		{
			nPoint.Y = nYDelta + nLocation.Y;
			nPoint.X = nOffsetX + nLocation.X + 2 * i;
			DSurface::Temp->DrawSHP(pPalette, pPipsShape, nHealthFrameResult, &nPoint, pBound, BlitterFlags(0x600), 0, 0, ZGradient::Ground, 1000, 0, 0, 0, 0, 0);
		}
	}
}

DEFINE_HOOK(0x6F65D1, TechnoClass_DrawHealthBar_DrawBuildingShieldBar, 0x6)
{
	GET(TechnoClass*, pThis, ESI);
	GET(int, iLength, EBX);
	GET_STACK(Point2D*, pLocation, STACK_OFFS(0x4C, -0x4));
	GET_STACK(RectangleStruct*, pBound, STACK_OFFS(0x4C, -0x8));

	const auto pExt = TechnoExt::ExtMap.Find(pThis);
	if (const auto pShieldData = pExt->Shield.get())
	{
		if (pShieldData->IsAvailable())
			pShieldData->DrawShieldBar(iLength, pLocation, pBound);
	}

	DrawHeathData::DrawNumber(pThis, pLocation, pBound);
	return 0;
}

DEFINE_HOOK(0x6F683C, TechnoClass_DrawHealthBar_Custom, 0x7)
{
	GET(TechnoClass*, pThis, ESI);
	GET_STACK(Point2D*, pLocation, STACK_OFFS(0x4C, -0x4));
	GET_STACK(RectangleStruct*, pBound, STACK_OFFS(0x4C, -0x8));

	const auto pExt = TechnoExt::ExtMap.Find(pThis);
	if (const auto pShieldData = pExt->Shield.get())
	{
		if (pShieldData->IsAvailable())
		{
			const int iLength = pThis->WhatAmI() == AbstractType::Infantry ? 8 : 17;
			pShieldData->DrawShieldBar(iLength, pLocation, pBound);
		}
	}

	DrawHeathData::DrawBar(pThis, pLocation, pBound);
	DrawHeathData::DrawNumber(pThis, pLocation, pBound);

	return 0x6F6A58;
	//return 0;
}

DEFINE_HOOK(0x44D0C3, BuildingClass_Missile_EMPFire_WeaponType, 0x5)
{
	GET(BulletClass*, pBullet, EAX);
	GET(WeaponTypeClass*, pWeapon, EBP);

	if (pBullet && pWeapon && !pBullet->GetWeaponType())
		pBullet->SetWeaponType(pWeapon);

	return 0;
}

DEFINE_HOOK(0x7290AD, TunnelLocomotionClass_Process_Stop, 0x5)
{
	GET(TunnelLocomotionClass* const, pLoco, ESI);

	if (const auto pLinked = pLoco->Owner)
		if (auto const pCell = pLinked->GetCell())
			pCell->CollectCrate(pLinked);

	return 0;
}

DEFINE_HOOK(0x709ACF, TechnoClass_DrawPip_PipShape1_A, 0x6)
{
	GET(TechnoClass*, pThis, EBP);
	GET(SHPStruct*, pPipShape01, ECX);

	const auto pThisExt = TechnoTypeExt::ExtMap.Find(pThis->GetTechnoType());
	auto const pShapeData = pThisExt->AnotherData.PipShapes01.Get(nullptr);

	if (pShapeData)
		pPipShape01 = pShapeData->GetOrDefaultShape(pPipShape01);

	R->ECX(pPipShape01);

	return 0;
}

DEFINE_HOOK(0x709AE3, TechnoClass_DrawPip_PipShape1_B, 0x6)
{
	GET(TechnoClass*, pThis, EBP);
	GET(SHPStruct*, pPipShape01, EAX);

	const auto pThisExt = TechnoTypeExt::ExtMap.Find(pThis->GetTechnoType());
	auto const pShapeData = pThisExt->AnotherData.PipShapes01.Get(nullptr);

	if (pShapeData)
		pPipShape01 = pShapeData->GetOrDefaultShape(pPipShape01);

	R->EAX(pPipShape01);

	return 0;
}

DEFINE_HOOK(0x709AF8, TechnoClass_DrawPip_PipShape2, 0x6)
{
	GET(TechnoClass*, pThis, EBP);
	GET(SHPStruct*, pPipShape02, EBX);

	const auto pThisExt = TechnoTypeExt::ExtMap.Find(pThis->GetTechnoType());
	auto const pShapeData = pThisExt->AnotherData.PipShapes02.Get(nullptr);

	if (pShapeData)
		pPipShape02 = pShapeData->GetOrDefaultShape(pPipShape02);

	R->EBX(pPipShape02);

	return 0;
}

//6F6759 , EDX ,6F675F
DEFINE_HOOK(0x6F6722, TechnoClass_DrawHealth_Building_PipFile_B, 0x6)
{
	GET(BuildingClass*, pThis, ESI);

	const auto pThisExt = TechnoTypeExt::ExtMap.Find(pThis->GetTechnoType());
	auto const pShapeData = pThisExt->AnotherData.PipShapes01.Get(nullptr);
	auto pPipShape01 = FileSystem::PIPS_SHP();

	if (pShapeData)
		pPipShape01 = pShapeData->GetOrDefaultShape(pPipShape01);

	R->EDX(pPipShape01);

	return 0x6F6728;
}

DEFINE_HOOK(0x6F6759, TechnoClass_DrawHealth_Building_PipFile_B_pal, 0x6)
{
	GET(BuildingClass*, pThis, ESI);
	const auto pBuildingTypeExt = BuildingTypeExt::ExtMap.Find(pThis->Type);
	ConvertClass* nPal = nullptr;

	if (pBuildingTypeExt->AnotherTypeData.PipShapes01Remap)
		nPal = pThis->GetRemapColour();
	else
		nPal = pBuildingTypeExt->AnotherTypeData.PipShapes01Palette.GetOrDefaultConvert(FileSystem::THEATER_PAL());

	R->EDX(nPal);
	return 0x6F675F;
}

DEFINE_HOOK(0x6F66B3, TechnoClass_DrawHealth_Building_PipFile_A, 0x6)
{
	GET(BuildingClass*, pThis, ESI);
	GET(SHPStruct*, pDefaultPip, EAX);

	const auto pThisExt = TechnoTypeExt::ExtMap.Find(pThis->GetTechnoType());
	const auto pBuildingTypeExt = BuildingTypeExt::ExtMap.Find(pThis->Type);
	ConvertClass* nPal = nullptr;

	if (pBuildingTypeExt->AnotherTypeData.PipShapes01Remap)
		nPal = pThis->GetRemapColour();
	else
		nPal = pBuildingTypeExt->AnotherTypeData.PipShapes01Palette.GetOrDefaultConvert(FileSystem::THEATER_PAL());

	auto const pShapeData = pThisExt->AnotherData.PipShapes01.Get(nullptr);

	if (pShapeData)
		pDefaultPip = pShapeData->GetOrDefaultShape(pDefaultPip);

	//PipShapes01Palette
	R->EDX(nPal);//
	R->EAX(pDefaultPip);

	return 0x6F66B9;
}

// this hook already inside loop function !
DEFINE_HOOK(0x709C84, TechnoClass_DrawPip_Occupants, 0x6)
{
	struct DrawPipDataStruct
	{
		int nOccupantsCount; int Y; SHPStruct* pShape; int nMaxOccupants;
	};

	GET(BuildingClass*, pThis, EBP);
	GET(int, nOccupantIdx, EDI);
	GET(int, nOffset_X, EBX);
	GET_STACK(int, nOffset_Y, STACK_OFFS(0x74, 0x50));
	GET_STACK(DrawPipDataStruct, nPipDataStruct, STACK_OFFS(0x74, 0x60));
	GET_STACK(Point2D, nDrawOffset, STACK_OFFS(0x74, 0x24));
	GET_STACK(Point2D, nOffsetadd, STACK_OFFS(0x74, 0x1C));
	GET_STACK(RectangleStruct*, pRect, STACK_OFFS(0x74, -0xC));
	GET(int, nOffsetY_Increment, ESI);

	int nPipFrameIndex = 6;
	SHPStruct* pPipFile = nPipDataStruct.pShape;
	ConvertClass* pPalette = FileSystem::THEATER_PAL;

	if (nOccupantIdx < nPipDataStruct.nMaxOccupants)
	{
		if (auto const pInfantry = pThis->Occupants.GetItem(nOccupantIdx))
		{
			const auto pInfTypeExt = TechnoTypeExt::ExtMap.Find(pInfantry->Type);
			auto const pPipData = pInfTypeExt->AnotherData.PipGarrison.Get(nullptr);

			if (pPipData)
			{
				if (auto const pGarrisonPip = pPipData->GetOrDefaultShape(nullptr))
				{
					pPipFile = pGarrisonPip;
					nPipFrameIndex = pInfTypeExt->AnotherData.PipGarrison_FrameIndex.Get();
					nPipFrameIndex = Math::clamp(nPipFrameIndex, 0, (int)pGarrisonPip->Frames);
					pPalette = pInfTypeExt->AnotherData.PipGarrison_Palette.GetOrDefaultConvert(pPalette);
				}
			}
			else
			{
				nPipFrameIndex = (int)pInfantry->Type->OccupyPip;
			}
		}
	}

	Point2D nOffset { nOffset_X + nDrawOffset.X ,nDrawOffset.Y + nOffset_Y };
	if (pPipFile)
	{
		DSurface::Temp->DrawSHP(
			pPalette,
			pPipFile,
			nPipFrameIndex,
			&nOffset,
			pRect,
			BlitterFlags(0x600),
			0,
			0,
			ZGradient::None,
			1000,
			0,
			0,
			0,
			0,
			0);
	}

	++nOccupantIdx;
	nOffset_X += nOffsetadd.X;
	nOffset_Y += nOffsetY_Increment;

	// need to forward the value bacause it is needed for next loop
	R->EBX(nOffset_X);
	R->ECX(nOffset_Y);
	R->EDI(nOccupantIdx);
	R->EAX(nPipDataStruct.nOccupantsCount);

	return 0x709D11;
}

namespace Replacer
{
	const char* ReplaceOccupantAnimNameTo(BuildingClass* pThis, BuildingAnimSlot nSlot, const char* pDefault)
	{
		//pthis check is just in  case
		if (pThis
			&& pThis->IsAlive
			&& (pThis->Occupants.Count > 0)
			&& pThis->Occupants[0]
			)
		{
			bool IsInternet = SessionClass::Instance->GameMode == GameMode::Internet;
			Debug::Log_WithBool(!IsInternet, __FUNCTION__" Executed for[%s] !  O[%d] \n", pThis->get_ID(), pThis->Occupants.Count);
			if (auto pOccupantsOwner = pThis->Occupants[0]->Owner)
			{
				auto nIndex = HouseTypeClass::Array()->FindItemIndex(pOccupantsOwner->Type);
				if (nIndex != -1)
				{
					Debug::Log_WithBool(!IsInternet, __FUNCTION__" Executed for[%s] !  O[%d] house[%d] \n", pThis->get_ID(), pThis->Occupants.Count, nIndex);

					const auto pBuildingExt = BuildingTypeExt::ExtMap.Find(pThis->Type);
					AnimTypeClass* pDecidedAnim = nullptr;

					switch (nSlot)
					{
					case BuildingAnimSlot::Active:
						if (!pBuildingExt->AnotherTypeData.GarrisonAnim_ActiveOne.empty())
							pDecidedAnim = pBuildingExt->AnotherTypeData.GarrisonAnim_ActiveOne[nIndex];
						break;
					case BuildingAnimSlot::ActiveTwo:
						if (!pBuildingExt->AnotherTypeData.GarrisonAnim_ActiveTwo.empty())
							pDecidedAnim = pBuildingExt->AnotherTypeData.GarrisonAnim_ActiveTwo[nIndex];
						break;
					case BuildingAnimSlot::ActiveThree:
						if (!pBuildingExt->AnotherTypeData.GarrisonAnim_ActiveThree.empty())
							pDecidedAnim = pBuildingExt->AnotherTypeData.GarrisonAnim_ActiveThree[nIndex];
						break;
					case BuildingAnimSlot::ActiveFour:
						if (!pBuildingExt->AnotherTypeData.GarrisonAnim_ActiveFour.empty())
							pDecidedAnim = pBuildingExt->AnotherTypeData.GarrisonAnim_ActiveFour[nIndex];
						break;
					case BuildingAnimSlot::Idle:
						if (!pBuildingExt->AnotherTypeData.GarrisonAnim_idle.empty())
							pDecidedAnim = pBuildingExt->AnotherTypeData.GarrisonAnim_idle[nIndex];
						break;
					default:
						break;
					}

					if (pDecidedAnim)
					{
						Debug::Log_WithBool(!IsInternet, __FUNCTION__" Executed for[%s] !  O[%d] house[%d] Decided[%s]\n", pThis->get_ID(), pThis->Occupants.Count, nIndex, pDecidedAnim->get_ID());
						return pDecidedAnim->get_ID();
					}
				}
			}
		}

		return pDefault;
	}
}

DEFINE_HOOK(0x4518CF, BuildingClass_AnimLogic_check, 0x9)
{
	GET(BuildingClass*, pThis, ESI);
	GET_STACK(const char*, pDecidedName, STACK_OFFS(0x34, -0x4));
	GET_STACK(BuildingAnimSlot, nSlot, STACK_OFFS(0x34, -0x8));

	R->EAX(AnimTypeClass::FindIndex(Replacer::ReplaceOccupantAnimNameTo(pThis, nSlot, pDecidedName)));

	return 0x4518D8;
}

DEFINE_HOOK(0x6FF329, TechnoCllass_FireAt_OccupyAnims, 0x6)
{
	GET(WeaponTypeClass*, pWeapon, EBX);

	AnimTypeClass* pDecidedMuzzle = pWeapon->OccupantAnim;
	const auto pWeaponExt = WeaponTypeExt::ExtMap.Find(pWeapon);

	if (pWeaponExt->AnotherData.OccupantAnim_UseMultiple.Get())
	{
		switch (pWeaponExt->AnotherData.OccupantAnims.size())
		{
		case 0:
			break;
		case 1:
			pDecidedMuzzle = pWeaponExt->AnotherData.OccupantAnims[0];
			break;
		default:
			pDecidedMuzzle = pWeaponExt->AnotherData.OccupantAnims[ScenarioGlobal->Random(0, pWeaponExt->AnotherData.OccupantAnims.size() - 1)];
			break;
		}
	}

	R->EDI(pDecidedMuzzle);
	return 0x6FF32F;
}

DEFINE_HOOK(0x46B1D6, BulletClass_DrawVXL_Palette, 0x6)
{
	GET_STACK(BulletClass*, pThis, STACK_OFFS(0xF8, 0xE4));
	GET(BulletTypeClass*, pThisType, EDX);
	GET(Point2D*, pPoint, ECX);
	GET(int, nRect_X, EBP);
	GET(int, nRect_Y, ESI);

	R->Stack(STACK_OFFS(0xF8, 0xE4), Point2D { pPoint->X + nRect_X , pPoint->Y + nRect_Y });
	R->EAX(ColorScheme::Array()->Items);

	int nIdx = pThisType->Color;
	if (pThisType->FirersPalette && !(pThis->InheritedColor == -1))
		nIdx = pThis->InheritedColor;

	R->ECX(nIdx);
	return 0x46B1F2;
}

DEFINE_HOOK(0x706389, TechnoClass_Draw_Object_NormalLight, 0x6)
{
	GET(TechnoClass*, pThis, ESI);

	if (auto pBuilding = specific_cast<BuildingClass*>(pThis))
	{
		if (pBuilding->CurrentMission == Mission::Construction && !pBuilding->BState && pBuilding->Type->Buildup)
		{
			auto const pExt = BuildingTypeExt::ExtMap.Find(pBuilding->Type);
			if (pExt && pExt->AnotherTypeData.BuildUp_UseNormalLIght.Get())
			{
				R->EBP(1000);
			}
		}
	}

	return 0x0;
}

// Draw Tiled !
DEFINE_HOOK(0x4236A7, AnimClass_Draw_Tiled_CustomPalette, 0xA)
{
	GET(AnimClass*, pThis, ESI);
	GET(int, nY_Loc, EDI);
	GET(int, nYadd_Loc, EBP);
	GET(BlitterFlags, nFlags, EBX);
	GET_STACK(int, nFrame, STACK_OFFS(0x110, 0xE4));
	LEA_STACK(Point2D*, nPoint, STACK_OFFS(0x110, 0xE0));
	GET_STACK(int, nSHPHeight, STACK_OFFS(0x110, 0xF0));
	GET_STACK(SHPStruct*, pShp, STACK_OFFS(0x110, 0xE8));
	GET_STACK(int, nX_Loc, STACK_OFFS(0x110, 0xD0));
	GET_STACK(int, nTintColor, STACK_OFFS(0x110, 0xF4));
	GET_STACK(int, nBrightness, STACK_OFFS(0x110, 0xD8));

	if (!pShp)
		return 0x42371B;

	const auto pTypeExt = AnimTypeExt::ExtMap.Find(pThis->Type);
	auto pPal = pTypeExt ? pTypeExt->Palette.GetOrDefaultConvert(FileSystem::ANIM_PAL()) : FileSystem::ANIM_PAL();
	auto Y_Doffs = pThis->Type->YDrawOffset;


	for (; nYadd_Loc >= 0;)
	{
		nPoint->X = nX_Loc;
		nPoint->Y = nYadd_Loc + Y_Doffs;

		DSurface::Temp->DrawSHP(
		pPal,
		pShp,
		nFrame,
		nPoint,
		&Drawing::SurfaceDimensions_Hidden(),
		nFlags,
		0,
		nY_Loc,
		ZGradient::Deg90,
		nBrightness,
		nTintColor,
		0,
		0,
		0,
		0);

		nYadd_Loc -= nSHPHeight;
		nY_Loc -= nSHPHeight + (nSHPHeight / 2);
		nPoint->Y = nYadd_Loc + Y_Doffs;
	}
	/*
	do
	{
		DSurface::Temp->DrawSHP(
			pPal,
			pShp,
			nFrame,
			nPoint,
			&Drawing::SurfaceDimensions_Hidden(),
			nFlags,
			0,
			nY_Loc,
			ZGradient::Deg90,
			nBrightness,
			nTintColor,
			0,
			0,
			0,
			0);

		nYadd_Loc -= nSHPHeight;
		nY_Loc -= nSHPHeight + nSHPHeight / 2;
		nPoint->Y = nYadd_Loc + Y_Doffs;
	}
	while (nYadd_Loc >= 0);*/

	return 0x42371B;
}

DEFINE_HOOK(0x464758, BuildingTypeClass_LoadFromINI_PowerUPZAdjust, 0x8)
{
	GET(int, nIndex, EBX);
	GET(BuildingTypeClass*, pThis, EBP);

	char flag[0x800];
	sprintf_s(flag, "PowerUp%01dZAdjust", nIndex);
	pThis->BuildingAnim[nIndex].ZAdjust = CCINIClass::INI_Art().ReadInteger(pThis->ImageFile, flag, pThis->BuildingAnim[nIndex].ZAdjust);

	return 0;
}

namespace _fetchCall
{
	bool bDisableNotTiberiumLog = true;
	bool bDisableNoDigestLog = true;
	//	BulletClass* ProjMotion_bulletContext = nullptr;

	static	void __fastcall TacticalClass_Render()
	{
		IonBlastClass::DrawAll();
		VeinholeMonsterClass::DrawAll();
	}

	//RepeatableTimerStruct InActiveTimer(10000);

	static	void __fastcall LogicClass_AI()
	{
		IonBlastClass::UpdateAll();
		VeinholeMonsterClass::UpdateAll();

		//if (InActiveTimer.Expired())
		//{
		//	AbstractClass::RemoveAllInactive();
		//	InActiveTimer.Restart();
		//}

		//for (auto const pRadExt : RadSiteExt::Array)
		//{
		//	if (pRadExt)
		//		pRadExt->UpdateTimer();
		//}
	}

	static	int __fastcall SelectParticle(char* pName)
	{
		return RulesExt::Global()->AnotherData.VeinholeParticle.Get(ParticleTypeClass::FindIndex(pName));
	}

	static	void __fastcall Replace_VeinholeShapeLoad(TheaterType nTheater)
	{
		//TheaterTypeClass::GetCharExtension(nTheater)
		char flag[0x100];
		_snprintf_s(flag, _TRUNCATE, "VEINHOLE.%s", Theater::GetTheater(nTheater).Extension);
		if (auto const pImage = FileSystem::LoadSHPFile(flag))
			VeinholeMonsterClass::VeinSHPData = pImage;
	}

	static	void __fastcall _Log_NotTib(const char* pFormat, ...)
	{
		if (!bDisableNotTiberiumLog)
			Debug::Log(pFormat);
	}

	static	void __fastcall _Log_NoDigest(const char* pFormat, ...)
	{
		if (!bDisableNoDigestLog)
			Debug::Log(pFormat);
	}

	static	void __fastcall DisplayClass_ReadINI_add(TheaterType nTheater)
	{
		SmudgeTypeClass::TheaterInit(nTheater);
		Replace_VeinholeShapeLoad(nTheater);
	}

	static	void __fastcall DrawShape_VeinHole
	(Surface* pSurface, ConvertClass* pPal, SHPStruct* SHP, int FrameIndex,
	const Point2D* const Position, const RectangleStruct* const Bounds, BlitterFlags Flags,
	int Remap, int ZAdjust, ZGradient ZGradientDescIndex, int Brightness, int TintColor,
	SHPStruct* ZShape, int ZShapeFrame, int XOffset, int YOffset
	)
	{
		bool bUseTheaterPal = true;
		CC_Draw_Shape(pSurface, bUseTheaterPal ? FileSystem::THEATER_PAL() : pPal, SHP, FrameIndex, Position, Bounds, Flags, Remap, ZAdjust, ZGradientDescIndex, Brightness
		 , TintColor, ZShape, ZShapeFrame, XOffset, YOffset);
	}

	static void __fastcall UnitClass_RotationAI_(UnitClass* pThis)
	{
		auto const TypeExt = TechnoTypeExt::ExtMap.Find(pThis->Type);

		auto nDisableEmp = pThis->EMPLockRemaining && TypeExt->AnotherData.FacingRotation_DisalbeOnEMP.Get();
		auto nDisableDeactivated = pThis->Deactivated && TypeExt->AnotherData.FacingRotation_DisalbeOnDeactivated.Get() && !pThis->EMPLockRemaining;

		if ((!(nDisableEmp || nDisableDeactivated) || !TypeExt->AnotherData.FacingRotation_Disable.Get()))
			pThis->UpdateRotation();
	}
}

DEFINE_HOOK(0x75F415, WaveClass_DamageCell_FixNoHouseOwner, 0x6)
{
	GET(TechnoClass*, pTechnoOwner, EAX);
	GET(ObjectClass*, pVictim, ESI);
	GET_STACK(int, nDamage, STACK_OFFS(0x18, 0x4));
	GET_STACK(WarheadTypeClass*, pWarhead, STACK_OFFS(0x18, 0x8));

	//Debug::Log("Wave Receive Damage for Victim [%x] ! \n", pVictim);
	pVictim->ReceiveDamage(&nDamage, 0, pWarhead, pTechnoOwner, false, false, pTechnoOwner ? pTechnoOwner->GetOwningHouse() : nullptr);

	return 0x75F432;
}

DEFINE_HOOK(0x530277, MixFile_BoostTrap_FixLog, 0x6)
{
	LEA_STACK(char*, pFilename, STACK_OFFS(0x78, 0x40));

	Debug::Log("%s Loaded! \n", pFilename);

	return 0x530289;
}

DEFINE_POINTER_CALL(0x685136, _fetchCall::Replace_VeinholeShapeLoad);
DEFINE_POINTER_CALL(0x4AD0A3, _fetchCall::DisplayClass_ReadINI_add)
DEFINE_POINTER_CALL(0x6D4656, _fetchCall::TacticalClass_Render);
DEFINE_POINTER_CALL(0x55B64B, _fetchCall::LogicClass_AI);
DEFINE_POINTER_CALL(0x74D0DF, _fetchCall::SelectParticle);
DEFINE_POINTER_CALL(0x5FDDB9, _fetchCall::_Log_NotTib);
DEFINE_POINTER_CALL(0x69A79D, _fetchCall::_Log_NoDigest);
DEFINE_POINTER_CALL(0x74D5BC, _fetchCall::DrawShape_VeinHole);
DEFINE_POINTER_CALL(0x7365E8, _fetchCall::UnitClass_RotationAI_);

DEFINE_HOOK(0x74C8FB, VeinholeMonsterClass_CTOR_SetArmor, 0x6)
{
	GET(VeinholeMonsterClass*, pThis, ESI);
	GET(TerrainTypeClass*, pThisTree, EDX);

	pThis->GetType()->Armor = pThisTree->Armor;

	return 0x0;
}

// thse were removed to completely disable vein
DEFINE_HOOK(0x74D376, VeinholeMonsterClass_AI_TSRandomRate_1, 0x6)
{
	GET(RulesClass*, pRules, EAX);

	auto const nRand = pRules->VeinholeShrinkRate > 0 ?
		ScenarioGlobal->Random(0, pRules->VeinholeShrinkRate / 2) : 0;

	R->EAX(pRules->VeinholeShrinkRate + nRand);
	return 0x74D37C;
}

DEFINE_HOOK(0x74D2A4, VeinholeMonsterClass_AI_TSRandomRate_2, 0x6)
{
	GET(RulesClass*, pRules, ECX);

	auto const nRand = pRules->VeinholeGrowthRate > 0 ?
		ScenarioGlobal->Random(0, pRules->VeinholeGrowthRate / 2) : 0;

	R->EAX(pRules->VeinholeGrowthRate + nRand);
	return 0x74D2AA;
}

//Add Occupy and Remove Occupy harcoded to 8
//5687D9 , INI 46142D

// IsJuggernaut Building Type class
// // DeployedFrom.Facing ,
// // DeployedFrom.ClearTarget ,
// // DeployedFrom.AutosetTarget,
// // DeployedFrom.AllowAutoFire,
//
//443C0D ticktank artillary ,set target
//44A93D ticktank artillary ,undeploy clear target
//736D77 ticktank ,deploy to fire
//739801 ticktank artillary , setfacing after deploy

DEFINE_HOOK(0x443C0D, BuildingClass_AssignTarget_Jugger, 0x6)
{
	GET(BuildingTypeClass*, pThis, EAX);

	auto const& pTypeExt = BuildingTypeExt::ExtMap.Find(pThis);
	R->CL(pThis->TickTank || pTypeExt->AnotherTypeData.IsJuggernaut);

	return 0x443C13;
}

DEFINE_HOOK(0x44A93D, BuildingClass_MI_Decontruction_Jugger, 0x6)
{
	GET(BuildingTypeClass*, pThis, EAX);

	auto const& pTypeExt = BuildingTypeExt::ExtMap.Find(pThis);
	R->CL(pThis->TickTank || pTypeExt->AnotherTypeData.IsJuggernaut);

	return 0x44A943;
}

DEFINE_HOOK(0x739801, UnitClass_TryToDeploy_BarrelFacing_Jugger, 0x6)
{
	GET(BuildingTypeClass*, pThis, EAX);

	auto const& pTypeExt = BuildingTypeExt::ExtMap.Find(pThis);
	R->CL(pThis->TickTank || pTypeExt->AnotherTypeData.IsJuggernaut);

	return 0x739807;
}

DEFINE_HOOK(0x6F6D9E, TechnoClass_Unlimbo_BuildingFacing_Jugger, 0x7)
{
	GET(TechnoClass*, pThis, ESI);
	//GET_STACK(int, nDir, STACK_OFFS(0x28, -0x8));

	//nDir = TranslateFixedPoint(16, 16, nDir, 0);
	//nDir = nDir << 8;
	if (auto pBuilding = specific_cast<BuildingClass*>(pThis))
	{
		auto const pBuildingType = pBuilding->Type;
		auto const& pTypeExt = BuildingTypeExt::ExtMap.Find(pBuildingType);
		if (!pBuildingType->LaserFence)
		{
			auto const nDir = pTypeExt->AnotherTypeData.IsJuggernaut ? 0x7FFF : pBuildingType->StartFacing;
			auto nDirOut = DirStruct(nDir);
			R->ECX(&nDirOut);
		}
	}

	return 0x0;
}

DEFINE_HOOK(0x449B04, TechnoClass_MI_Construct_Facing_Jugger, 0x6)
{
	GET(BuildingClass*, pThis, ESI);
	auto const& pTypeExt = BuildingTypeExt::ExtMap.Find(pThis->Type);
	auto nDir = DirStruct(pTypeExt->AnotherTypeData.IsJuggernaut ? 0x7FFF : pThis->Type->StartFacing);
	R->EDX(&nDir);
	return 0x0;
}

namespace _funct_JumpjetHeight
{
	//can add offset function if needed
	int _GetZCoordAdjent(CellClass* pCell, TechnoClass* pTechno, bool bCheckBridje)
	{
		int nZResult = 0;

		if (!pCell)
			return nZResult;

		auto pTechnoExt = TechnoTypeExt::ExtMap.Find(pTechno->GetTechnoType());
		//auto nBool = Make_Global<bool>(0xA8E9A0);

		if (auto pBuilding = pCell->GetBuilding())
		{
			CoordStruct nBuffer;
			pBuilding->Type->Dimension2(&nBuffer);
			nZResult += nBuffer.Z;

		}
		else
			if (pCell->FindTechnoNearestTo(Point2D::Empty, false, nullptr))
			{
				nZResult += 85;
			}

		if (auto nOffset = pTechnoExt->AnotherData.JumpJet_HeightOffset.isset())
			nZResult = Math::min((nZResult - nOffset), 0);

		auto nCellZ = pCell->GetFloorHeight({ 128,128 });
		if (bCheckBridje && pCell->ContainsBridge())
		{
			auto const nOffs = Make_Global<int>(0xABC5DC);
			nCellZ += nOffs;
		}

		return nZResult + nCellZ;
	}

	int _CalculateJumpjetHeight(JumpjetLocomotionClass* pLoco)
	{
		if (auto const pFoot = pLoco->LinkedTo)
		{
			auto nZFirst = _funct_JumpjetHeight::_GetZCoordAdjent(Map.GetCellAt(pFoot->Location), pFoot, true);

			if (pLoco->__currentSpeed > 0.0)
			{
				auto index = pLoco->Facing.current().value8();
				CoordStruct	nCoord = { pFoot->Location.X + AdjacentCoord[index].X, pFoot->Location.Y + AdjacentCoord[index].Y, 0 };
				auto nZAfter = _funct_JumpjetHeight::_GetZCoordAdjent(Map.GetCellAt(nCoord), pFoot, false);
				if (nZAfter <= nZFirst)
				{
					nZAfter = nZFirst + nZAfter / 2;
				}

				nZFirst = nZAfter;
			}

			return nZFirst;
		}

		//fallback to original function just in case !
		return pLoco->GetZCoords();
	}

	int __fastcall _ReplaceHeightCheck(JumpjetLocomotionClass* pThis)
	{
		return _CalculateJumpjetHeight(pThis);
	}
};

DEFINE_POINTER_CALL(0x54D345, _funct_JumpjetHeight::_ReplaceHeightCheck);

static DamageAreaResult __fastcall _RocketLocomotionClass_DamageArea
(
	CoordStruct* pCoord,
	int nDamage,
	TechnoClass* pSource,
	WarheadTypeClass* pWarhead,
	bool AffectTiberium, //false
	HouseClass* pSourceHouse //nullptr
)
{
	HouseClass* pHouseOwner = pSource ? pSource->GetOwningHouse() : nullptr;
	auto nCoord = *pCoord;
	return Map.DamageArea
	(nCoord, nDamage, pSource, pWarhead, pWarhead->Tiberium, pHouseOwner);
}

DEFINE_POINTER_CALL(0x6632C7, _RocketLocomotionClass_DamageArea);

#include <Misc/Otamaa/New/Entity/EboltClass/ElectricBoltManager.h>

static bool Phobos_EndProgHandle()
{
	Debug::Log("Cleaning up phobos ! \n");

	ArmorTypeClass::Clear();
	//MouseCursorTypeClass::Cleanup();
	//TheaterTypeClass::Clear();
	ShapeHandlerEnumerator::Cleanup();
	ElectricBoltManager::Clear_All();

	return true;
}

static DWORD __fastcall DeInt_72AC40()
{ JMP_STD(0x72AC40); }

static DWORD Phobos_EndProgHandle_add()
{
	if (!Phobos_EndProgHandle())
		Debug::Log("Error when trying to shutdown Phobos.dll");

	return DeInt_72AC40();
}

DEFINE_POINTER_CALL(0x6BE118, Phobos_EndProgHandle_add);

static AnimClass* __fastcall _InfantryClass_DoingAI_AnimCtor(
AnimClassCopy* pThis,
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
	GET_REGISTER_STATIC_TYPE(InfantryClass*, pThisInf, esi);
	auto const pAnim = pThis->_AnimClass_CTOR(pType, pCoord, LoopDelay, LoopCount, flags, ForceZAdjust, reverse);

	if (pAnim && pThisInf)
	{
		if (AnimTypeExt::ExtMap.Find(pAnim->Type)->CreateUnit.Get())
			AnimExt::SetAnimOwnerHouseKind(pAnim, pThisInf->GetOwningHouse(),
			pThisInf->Target ? pThisInf->Target->GetOwningHouse() : nullptr, false);
		else
			pAnim->Owner = pThisInf->GetOwningHouse();
	}

	return pAnim;
}

DEFINE_POINTER_CALL(0x520CA4, _InfantryClass_DoingAI_AnimCtor);

//Lunar limitation
DEFINE_LJMP(0x546C8B, 0x546CBF);

static char __fastcall CellClass_DrawPlacement(CellClass* pThis, void* _, CoordStruct* pCoord, RectangleStruct* pRect, bool bBlit)
{
	if (auto nObject = DisplayClass::Instance->CurrentBuildingType)
	{
		if (nObject->WhatAmI() == AbstractType::BuildingType)
			if (auto pBuildingType = static_cast<BuildingTypeClass*>(nObject))
			{
				Point2D nPoint = { pCoord->X, pCoord->Y };
				DSurface::Temp()->DrawSHP(
					FileSystem::PALETTE_PAL(),
					pBuildingType->GetImage(),
					0,
					&nPoint,
					pRect,
					BlitterFlags::TransLucent50 | BlitterFlags::Plain | BlitterFlags::Centered, 0, 0, 0, 0, 0, nullptr, 0, 0, 0);
			}
	}

	return pThis->DrawIt_47EC90((*pCoord), (*pRect), bBlit);
}

DEFINE_HOOK(0x6D528A, TacticalClass_DrawPlacement_PlacementPreview, 0x6)
{
	if (auto const pBuilding = specific_cast<BuildingClass*>(DisplayClass::Instance->CurrentBuilding))
	{
		if (auto const pTypeExt = BuildingTypeExt::ExtMap.Find(pBuilding->Type))
		{
			if (pTypeExt->AnotherTypeData.PlacementPreview_Show.Get(RulesExt::Global()->AnotherData.Building_PlacementPreview.Get()))
			{
				auto pImage = pBuilding->Type->GetImage();
				if (auto const pImageData = pTypeExt->AnotherTypeData.PlacementPreview_Shape.Get(nullptr))
					pImage = pImageData->GetOrDefaultShape(pImage);

				if (!pImage)
					return 0x0;

				auto const nFrame = Math::clamp(pTypeExt->AnotherTypeData.PlacementPreview_ShapeFrame.Get(), 0, (int)pImage->Frames);
				CellStruct const nDisplayCell = *Make_Pointer<CellStruct>(0x88095C);
				CellStruct const nDisplayCell_Offset = *Make_Pointer<CellStruct>(0x880960);
				auto const pCell = MapClass::Instance->TryGetCellAt(nDisplayCell + nDisplayCell_Offset);

				if (!pCell)
					return 0x0;

				auto const nHeight = pCell->GetFloorHeight({ 0,0 });
				auto nPoint = TacticalClass::Instance->CoordsToView(CellClass::Cell2Coord(pCell->MapCoords, nHeight));

				auto const nFlag = BlitterFlags::Centered | BlitterFlags::Nonzero | BlitterFlags::MultiPass | EnumFunctions::GetTranslucentLevel(pTypeExt->AnotherTypeData.PlacementPreview_Transculency.Get());
				auto const nREct = DSurface::Temp()->GetRect();
				auto const nPalette = pTypeExt->AnotherTypeData.PlacementPreview_Remap.Get() ? pBuilding->GetRemapColour() : pTypeExt->AnotherTypeData.PlacementPreview_Palette.GetOrDefaultConvert(FileSystem::UNITx_PAL());
				Point2D nDefault { 0 , -15 };
				nPoint += pTypeExt->AnotherTypeData.PlacementPreview_Offset.Get(nDefault);

				DSurface::Temp()->DrawSHP(
					nPalette,
					pImage,
					nFrame,
					&nPoint,
					&nREct,
					nFlag,
					0,
					0,
					1000, 0, 0, nullptr, 0, 0, 0

				);
			}
		}
	}

	return 0x0;
}

DEFINE_HOOK(0x6EE606, TeamClass_TMission_Move_To_Own_Building_index, 0x7)
{
	GET(TeamClass*, pThis, EBP);
	GET(int, nRawData, EAX);

	auto nBuildingIdx = nRawData & 0xFFFF;
	auto nTypeIdx = nRawData >> 16 & 0xFFFF;
	auto nScript = pThis->CurrentScript;

	//Debug::Log("Team[%x] script [%s]=[%d] , Trying to find type[%d] building at idx[%d] ! \n", pThis, nScript->Type->get_ID(), nScript->CurrentMission, nTypeIdx, nBuildingIdx);
	if (nBuildingIdx < BuildingTypeClass::Array()->Count)
		return 0x0;

	Debug::Log("Team[%x] script [%s]=[%d] , Failed to find type[%d] building at idx[%d] ! \n", pThis, nScript->Type->get_ID(), nScript->CurrentMission, nTypeIdx, nBuildingIdx);
	return 0x6EE7C3;
}

namespace GClockTemp
{
	SuperWeaponTypeClass* Super;
	TechnoTypeClass* Techno;
}

DEFINE_HOOK(0x6A9941, StripClass_DrawIt_GetSWData, 0x7)
{
	GET(SuperWeaponTypeClass*, pThis, EAX);
	GClockTemp::Super = pThis;
	return 0x0;
}

DEFINE_HOOK(0x6A9779, StripClass_DrawIt_GetTechnoData, 0x5)
{
	GET(TechnoTypeClass*, pThis, EAX);
	GClockTemp::Techno = pThis;
	return 0x0;
}

static void __fastcall StripClass_Draw_GClockSHP(Surface* Surface, ConvertClass* Palette, SHPStruct* SHP, int FrameIndex,
	const Point2D* const Position, const RectangleStruct* const Bounds, BlitterFlags Flags,
	int Remap,
	int ZAdjust,
	ZGradient ZGradientDescIndex,
	int Brightness,
	int TintColor, SHPStruct* ZShape, int ZShapeFrame, int XOffset, int YOffset)
{
	int Gclock_int = -1;

	if (GClockTemp::Super)
	{
		if (auto const pExt = SWTypeExt::ExtMap.Find(GClockTemp::Super))
		{
			if (pExt->AnotherData.GClock_Shape.Get(nullptr))
				SHP = pExt->AnotherData.GClock_Shape.Get()->GetOrDefaultShape(SHP);

			Gclock_int = pExt->AnotherData.GClock_Transculency.Get(-1);
			Palette = pExt->AnotherData.GClock_Palette.GetOrDefaultConvert(Palette);
		}

		if (Gclock_int != -1)
			Flags = BlitterFlags::bf_400 | EnumFunctions::GetTranslucentLevel(Gclock_int);

		CC_Draw_Shape(Surface, Palette, SHP, FrameIndex, Position, Bounds, Flags, Remap, ZAdjust, ZGradientDescIndex, Brightness, TintColor, ZShape, ZShapeFrame, XOffset, YOffset);
		return;
	}

	if (GClockTemp::Techno)
	{
		if (auto const pExt = TechnoTypeExt::ExtMap.Find(GClockTemp::Techno))
		{
			if (pExt->AnotherData.GClock_Shape.Get(nullptr))
				SHP = pExt->AnotherData.GClock_Shape.Get()->GetOrDefaultShape(SHP);


			Gclock_int = pExt->AnotherData.GClock_Transculency.Get(-1);
			Palette = pExt->AnotherData.GClock_Palette.GetOrDefaultConvert(Palette);
		}

		if (Gclock_int != -1)
			Flags = BlitterFlags::bf_400 | EnumFunctions::GetTranslucentLevel(Gclock_int);

		CC_Draw_Shape(Surface, Palette, SHP, FrameIndex, Position, Bounds, Flags, Remap, ZAdjust, ZGradientDescIndex, Brightness, TintColor, ZShape, ZShapeFrame, XOffset, YOffset);
		return;
	}

	CC_Draw_Shape(Surface, Palette, SHP, FrameIndex, Position, Bounds, Flags, Remap, ZAdjust, ZGradientDescIndex, Brightness, TintColor, ZShape, ZShapeFrame, XOffset, YOffset);
}

DEFINE_POINTER_CALL(0x6A9E97, &StripClass_Draw_GClockSHP);

DEFINE_HOOK(0x6A9E9C, StripClass_Draw_GClock_ClearContext, 0x6)
{
	GClockTemp::Techno = nullptr;
	GClockTemp::Super = nullptr;
	return 0x0;
}

static void __fastcall _DrawBehindAnim(TechnoClass* pThis, void* _, Point2D* pWhere, RectangleStruct* pBounds)
{
	if (!pThis->GetTechnoType()->Invisible)
		pThis->DrawBehind(pWhere, pBounds);
}

DEFINE_POINTER_CALL(0x6FA2D3, &_DrawBehindAnim)

static	void __fastcall _Log_PathFailere_1(const char* pFormat, ...)
{
	if (!RulesExt::Global()->AnotherData.DisablePathfindFailureLog.Get())
		Debug::Log(pFormat);
}

DEFINE_POINTER_CALL(0x42CBDE, &_Log_PathFailere_1);

static	void __fastcall _Log_PathFailere_2(const char* pFormat, ...)
{
	if (!RulesExt::Global()->AnotherData.DisablePathfindFailureLog.Get())
		Debug::Log(pFormat);
}

DEFINE_POINTER_CALL(0x42CC65, &_Log_PathFailere_2);

namespace SWTimerTemp
{
	SuperClass* Super;
	SWTypeExt::ExtData* SuperExt;
}

DEFINE_HOOK(0x6D4A10, TacticalClass_Render_FetchSW, 0x6)
{
	SWTimerTemp::Super = R->ECX<SuperClass*>();
	SWTimerTemp::SuperExt = SWTypeExt::ExtMap.Find(R->ECX<SuperClass*>()->Type);
	return 0x0;
}

DEFINE_HOOK(0x6D4A71, TacticalClass_Render_ClearSW, 0x5)
{
	SWTimerTemp::Super = nullptr;
	SWTimerTemp::SuperExt = nullptr;
	return 0x0;
}

static void __fastcall TacticalClass_PrintTimer(int arg1, ColorScheme* scheme, int interval, const wchar_t* string, LARGE_INTEGER* pBlinkTimer, bool* pBlinkState)
{
	JMP_STD(0x6D4B50);
}

namespace Timer
{
	void __fastcall DrawTimer(int arg1, ColorScheme* scheme, int interval, const wchar_t* string, LARGE_INTEGER* pBlinkTimer, bool* pBlinkState)
	{
		if (!SWTimerTemp::Super || !SWTimerTemp::SuperExt || !SWTimerTemp::SuperExt->AnotherData.ChargeTimer.Get())
		{
			TacticalClass_PrintTimer(arg1, scheme, interval, string, pBlinkTimer, pBlinkState);
			return;
		}

		auto const pFont = BitFont::BitFontPtr(
		TextPrintType::UseGradPal |
		TextPrintType::Right |
		TextPrintType::NoShadow |
		TextPrintType::Metal12 |
		TextPrintType::unk400);

		std::wstring lpDisplay = string;
		lpDisplay += L"  ";
		wchar_t nBuff[256];

		int const nTimeLeft = SWTimerTemp::Super->RechargeTimer.GetTimeLeft();
		double const nTimePrec = (((nTimeLeft * 1.0) / SWTimerTemp::Super->Type->RechargeTime) * 100.0);
		double const nRec = !SWTimerTemp::SuperExt->AnotherData.ChargeTimer_Backwards.Get() ? (100.0 - nTimePrec) : abs(nTimePrec);
		std::swprintf(nBuff, 256, L"%03d", (int)nRec); //adding % symbol broke things , wtf

		int nTimerIndex = arg1;

		// Most code below were previded by @Secsome !
		int nTimeWidth;
		pFont->GetTextDimension(nBuff, &nTimeWidth, nullptr, DSurface::ViewBounds->Width);

		ColorScheme* pTimeScheme = scheme;

		if (!interval && pBlinkTimer && pBlinkState)
		{
			auto currentTime = Game::AudioGetTime();
			if (pBlinkTimer->QuadPart <= currentTime.QuadPart)
			{
				pBlinkTimer->QuadPart = currentTime.QuadPart + 1000;
				*pBlinkState = !*pBlinkState;
			}

			if (*pBlinkState)
				pTimeScheme = ColorScheme::Array->GetItem(ColorScheme::White);
		}

		Point2D pntbuffer;
		++nTimerIndex;

		Point2D location;
		location.X = DSurface::ViewBounds->Width - nTimeWidth - 3;
		location.Y = DSurface::ViewBounds->Height - nTimerIndex * (pFont->field_1C + 2);
		Fancy_Text_Print_Wide(pntbuffer, lpDisplay.c_str(), DSurface::Composite, DSurface::ViewBounds,
			location, scheme, nullptr, (TextPrintType)0x4618);

		location.X += nTimeWidth;
		Fancy_Text_Print_Wide(pntbuffer, nBuff, DSurface::Composite, DSurface::ViewBounds,
			location, pTimeScheme, nullptr, (TextPrintType)0x4618);

	}
}

DEFINE_POINTER_CALL(0x6D4A6B, &Timer::DrawTimer);

//Make Building placement Grid tranparent
static void __fastcall CellClass_Draw_It_Shape(Surface* Surface, ConvertClass* Palette, SHPStruct* SHP, int FrameIndex,
	const Point2D* const Position, const RectangleStruct* const Bounds, BlitterFlags Flags,
	int Remap,
	int ZAdjust, // + 1 = sqrt(3.0) pixels away from screen
	ZGradient ZGradientDescIndex,
	int Brightness, // 0~2000. Final color = saturate(OriginalColor * Brightness / 1000.0f)
	int TintColor, SHPStruct* ZShape, int ZShapeFrame, int XOffset, int YOffset)
{
	Flags = Flags | EnumFunctions::GetTranslucentLevel(RulesExt::Global()->AnotherData.PlacementGrid_TranslucentLevel.Get());

	CC_Draw_Shape(Surface, Palette, SHP, FrameIndex, Position, Bounds, Flags, Remap, ZAdjust,
		ZGradientDescIndex, Brightness, TintColor, ZShape, ZShapeFrame, XOffset, YOffset);
}

DEFINE_POINTER_CALL(0x47EFB4, &CellClass_Draw_It_Shape);

static	void __fastcall _Log_LocalMix(const char* pFormat, ...)
{
	Debug::Log("LOCAL.MIX\n");
}

DEFINE_POINTER_CALL(0x530439, &_Log_LocalMix);

static	void __fastcall _Log_CacheMix(const char* pFormat, ...)
{
	Debug::Log("CACHE.MIX\n");
}

DEFINE_POINTER_CALL(0x5303B5, &_Log_CacheMix);

static void* __cdecl _YR_Allocate_Localmix(size_t size)
{
	Debug::Log("LOCALMD.MIX\n");
	return YRMemory::Allocate(size);
}

DEFINE_POINTER_CALL(0x5303E8, &_YR_Allocate_Localmix);

static	void __fastcall _Log_CacheMdMix(const char* pFormat, ...)
{
	Debug::Log("CACHEMD.MIX\n");
}

DEFINE_POINTER_CALL(0x530349, &_Log_CacheMdMix);

static	void __fastcall _Log_Ra2Mix(const char* pFormat, ...)
{
	Debug::Log("RA2.MIX\n");
}

DEFINE_POINTER_CALL(0x530307, &_Log_Ra2Mix);

static void* __cdecl _YR_Allocate_Ra2mix(size_t size)
{
	Debug::Log("RA2MD.MIX\n");
	return YRMemory::Allocate(size);
}

DEFINE_POINTER_CALL(0x5302BA, &_YR_Allocate_Ra2mix);

#include <Misc/Otamaa/ExtraHeaders/FlyingStrings.h>

static void __fastcall _eb_DrawAll_Impl()
{ JMP_STD(0x4C2830); }

static void __fastcall ___eb_DrawAll()
{
	_eb_DrawAll_Impl();
	ElectricBoltManager::Draw_All();
	FlyingStrings::UpdateAll();

}

namespace AnnoyingAudioLogSutffs
{
	bool Disable = true;
}

//40A55D
static	void __fastcall _Log_soundFrameOrBufferSize(const char* pFormat, ...)
{
	if (!AnnoyingAudioLogSutffs::Disable)
		Debug::Log(pFormat);
}

DEFINE_POINTER_CALL(0x40A55D, &_Log_soundFrameOrBufferSize);
DEFINE_POINTER_CALL(0x40A5BC, &_Log_soundFrameOrBufferSize);
DEFINE_POINTER_CALL(0x6D466E, &___eb_DrawAll);
/*
DEFINE_IMPORT_PATCH(0x7E1278, Phobos_HeapAlloc);
DEFINE_IMPORT_PATCH(0x7E1298, Phobos_HeapCreate);
DEFINE_IMPORT_PATCH(0x7E1190, Phobos_HeapDestroy);
DEFINE_IMPORT_PATCH(0x7E127C, Phobos_HeapFree);
DEFINE_IMPORT_PATCH(0x7E12A4, Phobos_HeapReAlloc);
DEFINE_IMPORT_PATCH(0x7DCEB0, Phobos_HeapSize);

DEFINE_HOOK(0x65DF8D, TeamTypeClass_GenerateTeamMemembers_OpenTopped, 0x6)
{
	GET(TechnoClass*, pTransporter, ESI);
	GET_STACK(FootClass*, pCargo, 0x14);
	GET_STACK(bool, nBool, 0x12);

	enum { Success = 0x65DF95, Failed = 0x65E004 };

	if (pTransporter->GetTechnoType()->OpenTopped)
	{
		pTransporter->EnteredOpenTopped(pCargo);
		pCargo->Transporter = pTransporter;
	}

	return nBool ? Success : Failed;
}*/

/*
static void store_FPU_DWORD() {
	JMP_STD(0x7C5EE4);
}

#include <cfenv>

static void InitPhobos() {
	store_FPU_DWORD();
	_set_controlfp(_RC_CHOP, _MCW_RC);
	fesetround(FE_TOWARDZERO);
}

DEFINE_POINTER_CALL(0x6BBFC9, &InitPhobos);

//crater chance 425066

DEFINE_HOOK(0x42465D, AnimClass_MidPoint_NoType, 0x8)
{
	GET(AnimClass*, pThis, ESI);
	GET_STACK(DWORD, C, 0x0);

	if (!pThis->Type)
	{
		Debug::Log("Anim [%x] Got Corrupted AnimType after [%x] ! deleting ! \n",pThis,C);
		Debug::DumpStack(__FUNCTION__, 0x200);
		return 0x424B31; //we delete this
	}
	return 0x0;
}*/

DEFINE_HOOK(0x442243, BuildingClass_ReceiveDamage_AddEarly, 0xA)
{
	R->Stack(STACK_OFFS(0x9C, 0x6C), DamageState::Unaffected);

	GET(BuildingClass*, pThis, ESI);
	GET(TechnoClass*, pSource, EBP);

	if (pThis == pSource && !pSource->GetTechnoType()->DamageSelf) {
		return 0x442C06;
	}

	/*
	auto const pTypeExt = BuildingTypeExt::ExtMap.Find(pThis->Type);

	if (pTypeExt->RubbleIntact.Get()) {
		if (pThis->GetCurrentMission() == Mission::Selling || pThis->GetCurrentMission() == Mission::Construction) {
			if (pThis->GetOwningHouse() && !pThis->GetOwningHouse()->Defeated) {
				return 0x442C06;
			}
		}
	}*/

	return 0x442268;
}

DEFINE_HOOK(0x4F8EBD, HouseClass_AI_CheckLimboedStuffs_, 0x7)
{
	GET(HouseClass*, pThis, ESI);

	for (auto const pBuilding : pThis->Buildings)
	{
		if (pBuilding && pBuilding->InLimbo)
		{
			auto const pExt = BuildingTypeExt::ExtMap.Find(pBuilding->Type);
			if (pExt && pExt->RubbleIntact.Get())
			{
				const auto pName = pBuilding->get_ID();
				GameDelete(pBuilding);

				if (pThis->Buildings.Remove(pBuilding)) {
					Debug::Log("Success Remove RubbleBuilding [%s] From House [%s] ! \n", pName, pThis->get_ID());
				}
				else {
					Debug::Log("Failed Remove RubbleBuilding [%s] From House [%s] ! \n", pName, pThis->get_ID());
				}
			}
		}
	}

	return 0x0;
}

DEFINE_HOOK(0x44E85F, BuildingClass_Power_UntieStregth, 0x7)
{
	GET(BuildingClass*, pThis, ESI);
	GET_STACK(int, nPowMult, STACK_OFFS(0xC, 0x4));

	auto const pTypeExt = BuildingTypeExt::ExtMap.Find(pThis->Type);

	R->EAX(Game::F2I(pTypeExt->AnotherTypeData.Power_DegradeWithHealth.Get()
		? (nPowMult * pThis->GetHealthPercentage()) : (nPowMult)));

	return 0x44E86F;
}

DEFINE_HOOK(0x6F7891, TechnoClass_TriggersCellInset_IgnoreVertical, 0x7)
{
	GET(WeaponTypeClass*, pWeapon, EDI);
	GET(TechnoClass*, pThis, ESI);

	auto const pExt = WeaponTypeExt::ExtMap.Find(pWeapon);
	auto const& nData = pExt->AnotherData;

	R->AL(pThis->IsInAir() && !nData.Range_IgnoreVertical.Get());
	return R->Origin() + 0x7;
}
