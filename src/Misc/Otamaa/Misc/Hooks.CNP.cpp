#include "Hooks.BugFixes.h"


namespace BTypeExtTemp
{
	Valueable<int> BuildingSelfHeal;
	Valueable<int> AircraftSelfHeal;
	Nullable<float> AutoSellTime;
	Nullable<float> AIAutoSellHealthRatio;

	ValueableVector<BuildingTypeClass*> RandomBuildList;
	ValueableVector<double> RandomBuildList_Chance;

}
/*
DEFINE_HOOK(0x448AC8, BuildingClass_Capture_RemoveHealAmount, 0x6)
{
	GET(BuildingClass*, pThis, ESI);

	auto pHouseExt = HouseExt::ExtMap.Find(pThis->Owner);

	pHouseExt->BuildingSelfHealTotal -= BTypeExtTemp::BuildingSelfHeal.Get();

	if (pHouseExt->BuildingSelfHealTotal < 0)
		pHouseExt->BuildingSelfHealTotal = 0;

	pHouseExt->AircraftSelfHealTotal -= BTypeExtTemp::AircraftSelfHeal.Get();

	if (pHouseExt->AircraftSelfHealTotal < 0)
		pHouseExt->AircraftSelfHealTotal = 0;

	return 0;
}

DEFINE_HOOK(0x4491F1, BuildingClass_Capture_AddHealAmount, 0x6)
{
	GET(BuildingClass*, pThis, ESI);

	auto pHouseExt = HouseExt::ExtMap.Find(pThis->Owner);

	pHouseExt->BuildingSelfHealTotal += BTypeExtTemp::BuildingSelfHeal.Get();

	if (pHouseExt->BuildingSelfHealTotal < 0)
		pHouseExt->BuildingSelfHealTotal = 0;

	pHouseExt->AircraftSelfHealTotal += BTypeExtTemp::AircraftSelfHeal.Get();

	if (pHouseExt->AircraftSelfHealTotal < 0)
		pHouseExt->AircraftSelfHealTotal = 0;

	return 0;
}
*/


/*
namespace EventExt
{
	bool Handle(void* pEvent)
	{
		//bla \bla , event exr something set it to true
		//0xC8 , repair all
	}
}

DEFINE_HOOK(0x4C6CC4, EventClass_Execute_Add, 0x6)
{
	GET(void*, pEvent, ESI);
	return EventExt::Handle(pEvent) ? 0x4C6CE5 : 0;
}

DEFINE_HOOK(0x4C66CF, EventClass_Ctor_add, 0x7)
{
	GET(int, nEventType, EBX);

	if (nEventType == 0xC8)
	{
		R->ECX("ALL REPAIR");
		R->Origin() + 7;
	}

	return 0;
}
*/

namespace RandomBuildList
{
	BuildingTypeClass* Select(ValueableVector<BuildingTypeClass*>& nList)
	{
		auto const nRand = ScenarioClass::Instance()->Random.RandomDouble();

		if (BTypeExtTemp::RandomBuildList_Chance.size() < nList.size() || BTypeExtTemp::RandomBuildList_Chance.empty())
		{
			auto const nRemain = nList.size() - BTypeExtTemp::RandomBuildList_Chance.size();

			for (size_t i = 0; i < nRemain; ++i)
				BTypeExtTemp::RandomBuildList_Chance.emplace_back((double)ScenarioClass::Instance()->Random(10));

		}

		{
			for (size_t k = 0; k < nList.size(); ++k)
			{
				if (nRand <= BTypeExtTemp::RandomBuildList_Chance[k])
				{
					return nList[k];
				}
			}
		}

		return nullptr;
	}
}

DEFINE_HOOK(0x4C9D95, FactoryClass_Set_CustomWhat, 0x5)
{
	//GET(BuildingTypeClass*, pType, EDI);

	if (!BTypeExtTemp::RandomBuildList.empty())
		if (auto pTypeSelected = RandomBuildList::Select(BTypeExtTemp::RandomBuildList))
			R->EDX(pTypeSelected);

	return 0x0;
}

namespace RulesExtTemp
{
	Valueable<ColorStruct> ActionLinecolor;
	Valueable<ColorStruct> MCLinecolor;
}

DEFINE_HOOK(0x4DC124, FootClass_DrawActionLine_Color, 0x5)
{
	GET(unsigned int, nDefault, EDI);

	if (RulesExtTemp::ActionLinecolor.Get() != ColorStruct::Empty)
	{
		auto nColor = RulesExtTemp::ActionLinecolor.Get();
		nDefault = nColor.ToInit();
	}

	R->EDI(nDefault);
	return 0x0;
}

DEFINE_HOOK(0x4DC2AB, FootClass_DrawActionLine_MindControlLine_Color, 0x5)
{
	GET(unsigned int, nDefault, EDX);

	if (RulesExtTemp::MCLinecolor.Get() != ColorStruct::Empty)
	{
		auto nColor = RulesExtTemp::MCLinecolor.Get();
		nDefault = nColor.ToInit();
	}

	R->EDX(nDefault);
	return 0x0;
}


/*- replace allied check with these
* so it can apply more of their stuffs
DEFINE_HOOK(0x5094F9, HouseClass_AdjustThreats, 0x6)
{
	GET(HouseClass*, pThis, ESI);
	GET(HouseClass*, pThat, EBX);

	return pThat->IsAlliedWith(pThis)  ? 0x5095B6 : 0x509532;
}

DEFINE_HOOK(0x4FBD1C , HouseClass_DoesEnemyBuildingExist, 0x6)
{
	GET(HouseClass*, pThis, ESI);
	GET(HouseClass*, pThat, EAX);

	return pThis->IsAlliedWith(pThat) ? 0x4FBD57 : 0x4FBD47;
}

DEFINE_HOOK(0x5003BA , HouseClass_FindJuicyTarget , 0x6)
{
	GET(HouseClass*, pThis, EDI);
	GET(HouseClass*, pThat, EAX);

	return pThis->IsAlliedWith(pThat) ? 0x5003F7 : 0x5004B1;
}*/

DEFINE_HOOK(0x6FCA81, TechnoClass_CanFire_Parasite, 0x6)
{
	enum
	{
		ContinueCheck = 0x6FCADB,
		ReturnIllegal = 0x6FCAB6,
		ContinueCheckA = 0x6FCAC5,
		ContinueCheckB = 0x6FCAFA
	};

	GET(FootClass*, pThis, ESI);
	GET(AbstractClass*, pTarget, EDI);
	GET(WeaponTypeClass*, pWeapon, EBX);

	if (!pThis || !pTarget || !pWeapon)
		return ReturnIllegal;

	if (pWeapon->Warhead && pWeapon->Warhead->Parasite)
	{
		bool Parasite_Ground = false;
		//auto pWHExt = WarheadTypeExt::ExtMap.Find(pWeapon->Warhead);

		if (pTarget->WhatAmI() == AbstractType::Cell && Parasite_Ground)
			return ContinueCheckB;

		auto pFoot = generic_cast<FootClass*>(pThis);
		auto pTargetFoot = generic_cast<FootClass*>(pTarget);

		if (!pFoot || !pTargetFoot)
			return ReturnIllegal;

		if (!pFoot->ParasiteImUsing->CanInfect(pTargetFoot))
			return ReturnIllegal;

		return ContinueCheck;
	}

	return ContinueCheckB;
}

namespace RulesExtTemp
{
	Valueable<bool> AIAgainstEngi;
	Valueable<int> AIAgainstSpy_Delay;
}

DEFINE_HOOK(0x519B58, InfantryClass_PerCellProcess_AutoSellCheck, 0x6)
{
	if (!RulesExtTemp::AIAgainstEngi.Get())
		return 0x0;

	GET(BuildingClass*, pBuilding, EDI);
	GET(InfantryClass*, pThis, ESI);

	if (!pThis->Owner || !pBuilding->Owner)
		return 0x0;

	if (pThis->Owner->IsAlliedWith(pBuilding)
		|| pBuilding->Owner->IsPlayer()
		|| pBuilding->Owner->Type->MultiplayPassive
		|| pBuilding->Type->Unsellable
		|| pBuilding->GetCurrentMission() == Mission::Selling
		) return 0x0;

	if (!pThis->Type->Engineer)
	{
		if (pThis->Type->Agent)
		{
			auto pBldExt = BuildingExt::ExtMap.Find(pBuilding);

			if (pBldExt->AutoSellTimer.StartTime == -1)
				pBldExt->AutoSellTimer.Start(RulesExtTemp::AIAgainstSpy_Delay.Get());

			if (pBldExt->AutoSellTimer.Completed())
				pBuilding->Sell(-1);

		}

		return 0x0;
	}

	if (pBuilding->Type->ConstructionYard)
		pBuilding->Focus = pBuilding->GetCell();

	pBuilding->Sell(-1);

	return 0x51A03E;
}

DEFINE_HOOK(0x51A002, InfantryClass_PerCellProcess_EventCheck, 0x6)
{
	GET(InfantryClass*, pThis, ESI);
	GET(TechnoClass*, pAnother, EDI);

	if (auto pTag = pAnother->AttachedTag)
	{
		pTag->RaiseEvent(TriggerEvent::SpyAsHouse, pThis, { 0,0 });
		pTag->RaiseEvent(TriggerEvent::SpyAsInfantry, pThis, { 0,0 });
		pTag->RaiseEvent(TriggerEvent(0x2BE), pThis, { 0,0 }); //new
	}

	return 0x0;
}

DEFINE_HOOK(0x4693D3, BulletClass_Logics_Parasite, 0x6)
{
	GET(BulletClass*, pThis, ESI);
	GET(WarheadTypeClass*, pWH, EAX);

	if (!pThis->Owner)
		return 0x0;

	bool Parasite_Ground = false;
	if (Parasite_Ground && pThis->Target->WhatAmI() == AbstractType::Cell)
	{
		auto pTechnoType = pThis->Owner->GetTechnoType();
		auto nCoord = pThis->Target->GetCoords();
		auto pCellTarget = MapClass::Instance()->TryGetCellAt(nCoord);
		auto nCellDest = MapClass::Instance()->NearByLocation(
			pCellTarget->MapCoords,
			pTechnoType->SpeedType,
			-1,
			pTechnoType->MovementZone,
			false, 1, 1, true, false, false, false, { 0,0 }, false, true);
		auto pCellDest = MapClass::Instance()->TryGetCellAt(nCellDest);
		auto nDestCoord = pCellDest->GetCoords();

		auto nDir = pThis->Owner->GetRealFacing();
		if (pThis->Owner->Unlimbo(nDestCoord, nDir.value8()))
		{
			pThis->Owner->SetTarget(nullptr);
			pThis->Owner->SetDestination(nullptr, true);
			pThis->Owner->QueueMission(Mission::Guard, false);
			pThis->Owner->EnterIdleMode(false, 1);
			pThis->Location = nDestCoord;
		}
		else
		{
			pThis->Owner->UnInit();
			GameDelete(pThis->Owner);
		}


		return 0x469AA4;
	}
	else
	{
		if (pWH->MakesDisguise)
			pThis->Owner->DisguiseAs(pThis->Target);
	}

	return 0x0;
}

/* size , too small*/

#include <Ext/Sidebar/Body.h>


/*
static void DrawGClock(int nFrame, Point2D* pPoint, RectangleStruct* pRect, SHPStruct* pGClock, ConvertClass* pGClockConvert, int nTranculency)
{
	if (!pGClock)
		pGClock = SidebarExt::DefaultClockSHp;

	if (!pGClockConvert)
		pGClockConvert = FileSystem::SIDEBAR_PAL();

	auto nFlag = BlitterFlags::bf_400;
	auto const nFlag_ = Math::clamp(nTranculency, 0, 3);

	switch (nFlag_)
	{
	case 1:
		nFlag = nFlag | BlitterFlags::TransLucent25;
		break;
	case 2:
		nFlag = nFlag | BlitterFlags::TransLucent50;
		break;
	case 3:
		nFlag = nFlag | BlitterFlags::TransLucent75;
		break;
	default:
		break;
	}

	DSurface::Sidebar->
	DrawSHP(pGClockConvert, pGClock, nFrame + 1, pPoint, pRect, nFlag, 0, 0, ZGradient::Ground, 1000, 0, nullptr, 0, 0, 0);
}*/


/*
DEFINE_HOOK(0x6A9E4A, StripClass_DrawIt_GCLOCK2SHPChange, 0x5)
{
	GET_STACK(int, nFrame, 0x38);
	REF_STACK(Point2D, nPoint, 0x64);
	GET(int, nX, ESI);
	GET(int, nY, EBP);
	REF_STACK(RectangleStruct, nRect, 0xAC);

	nPoint.X = nX;
	nPoint.Y = nY;


		DrawGClock(nFrame, &nPoint, &nRect, SidebarExt::ClockSHP[0], SidebarExt::ClockPAL[0], 0);

	GClockTemp::SuperType = nullptr;
	GClockTemp::Tech = nullptr;
	return 0x6A9E9C;
}*/

/*

DEFINE_HOOK(0x6FF660, TechnoClass_FireBullet, 0x6)
{
	return 0x0;
}

DEFINE_HOOK(0x6FD2C9, TechnoClass_FireLaser_AroundTarget, 0x5)
{
	return 0x0;
}

DEFINE_HOOK(0x6FDE0E, TechnoClass_FireAt_Strafe, 0x6)
{

}

template<enum E>
auto to_string(E value)
{
	switch(value)
	{
		constexpr {
			for(auto o : $E.variables())
			->{ case o.value(): return o.name(); }
		}
	}
}

namespace TestSS
{
	void Print()
	{
		Debug::Log("Value %s", to_string(AbstractType));
	}
}*/

#include <Ext/SWType/Body.h>


// New CSF Entries:
// TXT_TIMER_FORMAT_HMS "{0:d}:{1:02d}:{2:02d}"
// TXT_TIMER_FORMAT_MS "{0:02d}:{1:02d}"
/*
DEFINE_HOOK(0x6D4941, TacticalClass_Draw_Timers, 0x6)
{
	int nTimerIndex = 0;

	enum TimerStyle
	{
		STYLE_TIMER = 0,
		STYLE_PERCENTAGE,
		STYLE_VARIABLE,
	};

	auto drawTimer = [&nTimerIndex](const wchar_t* lpText, const wchar_t* lpTimeFormat, const wchar_t* lpTimeFormatNoHour,
		ColorScheme* pScheme, int nTime,
		LARGE_INTEGER* pBlinkTimer = nullptr, bool* pBlinkState = nullptr,
		TimerStyle eTimerStyle = STYLE_TIMER, int nTotalTime = 0,
		ColorScheme* pBlinkScheme = ColorScheme::Array->GetItem(ColorScheme::White)) -> void
	{
		std::wstring lpTime;
		std::wstring lpDisplay = lpText;
		lpDisplay += L"  ";

		switch (eTimerStyle)
		{
		case STYLE_TIMER:
		default:
			do
			{
				int nHour = nTime / 60 / 60;
				int nMinute = nTime / 60 % 60;
				int nSecond = nTime % 60 % 60;

				if (nHour)
					lpTime = std::move(std::format(lpTimeFormat, nHour, nMinute, nSecond));
				else
					lpTime = std::move(std::format(lpTimeFormatNoHour, nMinute, nSecond));
			}
			while (false);
			break;

		case STYLE_PERCENTAGE:
			do
			{
				double nPercentage = nTotalTime ? nTime * 100.0 / nTotalTime : 100.0;
				lpTime = std::move(std::format(lpTimeFormat, nPercentage));
			}
			while (false);
			break;

		case STYLE_VARIABLE:
			do
			{
				lpTime = std::format(lpTimeFormatNoHour, nTime);
			}
			while (false);
			break;
		}

		int nTimeWidth;
		BitFont::Instance->GetTextDimension(lpTime.c_str(), &nTimeWidth, nullptr, DSurface::ViewBounds->Width);

		ColorScheme* pTimeScheme = pScheme;
		// Blink
		if (!nTime && pBlinkTimer && pBlinkState)
		{
			auto currentTime = Game::AudioGetTime();
			if (pBlinkTimer->QuadPart <= currentTime.QuadPart)
			{
				pBlinkTimer->QuadPart = currentTime.QuadPart + 1000;
				*pBlinkState = !*pBlinkState;
			}
			if (*pBlinkState)
				pTimeScheme = pBlinkScheme;
		}

		Point2D pntbuffer;
		++nTimerIndex;

		Point2D location;
		location.X = DSurface::ViewBounds->Width - nTimeWidth - 3;
		location.Y = DSurface::ViewBounds->Height - nTimerIndex * (BitFont::Instance->field_1C + 2);
		Fancy_Text_Print_Wide(pntbuffer, lpDisplay.c_str(), DSurface::Composite, DSurface::ViewBounds,
			location, pScheme, nullptr, (TextPrintType)0x4618);

		location.X += nTimeWidth;
		Fancy_Text_Print_Wide(pntbuffer, lpTime.c_str(), DSurface::Composite, DSurface::ViewBounds,
			location, pTimeScheme, nullptr, (TextPrintType)0x4618);
	};

	if (ScenarioClass::Instance->MissionTimer.InProgress())
	{
		const int nTimeLeft = ScenarioClass::Instance->MissionTimer.GetTimeLeft();
		drawTimer(ScenarioClass::Instance->MissionTimerTextCSF,
			StringTable::TryFetchString("TXT_TIMER_FORMAT_HMS", L"{0:d}:{1:02d}:{2:02d}"),
			StringTable::TryFetchString("TXT_TIMER_FORMAT_MS", L"{0:02d}:{1:02d}"),
			ColorScheme::Array->GetItem(HouseClass::Player->ColorSchemeIndex), nTimeLeft / 15);
	}

	for (auto& timer : ScenarioExt::Extension->VariableTimers)
	{
		const wchar_t* lpText = timer.Label.get_text();
		const int nValue = ScenarioExt::Extension->Variables[timer.IsGlobal].FetchIndex(timer.VariableIndex).Value;
		ColorScheme* pScheme = ColorScheme::Array->GetItem(timer.ColorIndex);

		drawTimer(lpText, L"{0:d}", nullptr, pScheme, nValue, nullptr, nullptr, STYLE_VARIABLE);
	}

	for (auto pSuper : SuperClass::ShowTimers())
	{
		if (!SuperWeaponTypeExt::IsVisible(pSuper))
			continue;

		const int nTimeLeft = pSuper->RechargeTimer.GetTimeLeft();
		const int nTotalTime = pSuper->GetRechargeTime();
		if (!SessionClass::IsCampaign() || !pSuper->IsOnHold || nTotalTime != nTimeLeft)
		{
			auto const pData = SuperWeaponTypeExt::GetExtData(pSuper->Type);
			if (pData->ShowTimer_Percentage)
			{
				drawTimer(pSuper->Type->UIName, StringTable::TryFetchString(pData->ShowTimer_PercentageFormat, L"{0:n2}%"), nullptr,
					ColorScheme::Array->GetItem(pSuper->Owner->ColorSchemeIndex), nTimeLeft,
					&pSuper->BlinkTimer, &pSuper->BlinkState, STYLE_PERCENTAGE, nTotalTime);
			}
			else
			{
				drawTimer(pSuper->Type->UIName,
					StringTable::TryFetchString("TXT_TIMER_FORMAT_HMS", L"{0:d}:{1:02d}:{2:02d}"),
					StringTable::TryFetchString("TXT_TIMER_FORMAT_MS", L"{0:02d}:{1:02d}"),
					ColorScheme::Array->GetItem(pSuper->Owner->ColorSchemeIndex), nTimeLeft / 15,
					&pSuper->BlinkTimer, &pSuper->BlinkState);
			}
		}
	}

	for (auto pHouse : *HouseClass::Array)
	{
		if (pHouse->PowerBlackoutTimer.InProgress())
		{
			const int nTimeLeft = pHouse->PowerBlackoutTimer.GetTimeLeft();
			drawTimer(StringTable::LoadString("MSG:BlackoutTimer"),
				StringTable::TryFetchString("TXT_TIMER_FORMAT_HMS", L"{0:d}:{1:02d}:{2:02d}"),
				StringTable::TryFetchString("TXT_TIMER_FORMAT_MS", L"{0:02d}:{1:02d}"),
				ColorScheme::Array->GetItem(pHouse->ColorSchemeIndex), nTimeLeft / 15);
		}
	}

	return 0x6D4B25;
}
*/
/*https://stackoverflow.com/questions/36048319/show-the-percentage-of-process-completion-in-a-c-program
template <typename T, typename Func>
void bubblesort(std::vector<T>& v, Func callback)
{
	size_t const len = v.size();
	size_t n = v.size();
	while (n > 0)
	{
		size_t newn = 0;
		for (size_t i = 1; i <= n - 1; ++i)
		{
			if (v[i - 1] > v[i])
			{
				std::swap(v[i - 1], v[i]);
				newn = i;
			}
		}
		n = newn;
		callback(100 - static_cast<int>(n * 100 / len));
	}
}

static void Temp()
{
	std::vector<int> vec;

	char const* prefix = "Bubble sort under progress. Done: ";
	int lastp = -1;
	bubblesort(vec, [&lastp, prefix](int p)
	{
			//if progress has changed, update it
			if (p != lastp)
			{
				lastp = p;
				std::cout << "\r" << prefix << p << "%" << std::flush;
				//std::flush is needed when we don't start a new line
				 // '\r' puts the cursor to the start of the line
			}
	});

	std::cout << "\r" << prefix << "100%" << std::endl;
	//make sure we always end on 100% and end the line
}*/

namespace TTypeExt_Temp
{
	ValueableVector<TechnoTypeClass*> CanBeAutoTargetedBy;
}



enum NewVHPVal : int
{
	None = 0,
	Normal,
	Strong,
	Threat,
	Health,
	Damage,
	Value,
	Locked
};

namespace NewVHPTemp
{
	Nullable<int> VHP_Value;
}
//DEFINE_HOOK(0x4775B0, CCINIClass_ReadVHPScan_ReplaceName, 0x7)

//DEFINE_HOOK(0x6F8721, TechnoClass_CanAutoTargetObject_VHPScan, 0x7)
static DWORD TechnoClass_CanAutoTargetObject_VHPScan(REGISTERS* R)
{
	GET(TechnoClass*, pTargeted, ESI);
	GET(TechnoClass*, pAttacker, EDI);
	GET(int*, pValue, EBP);

	auto const nVal = NewVHPTemp::VHP_Value.Get(2);
	auto const nVHP = (NewVHPVal)pAttacker->GetTechnoType()->VHPScan;

	switch (nVHP)
	{
	case Normal:
	{
		if (pTargeted->EstimatedHealth <= 0)
		{
			*pValue /= nVal;
			break;
		}

		if (auto pTargetType = pTargeted->GetTechnoType())
		{
			if (pTargeted->EstimatedHealth <= pTargetType->Strength / 2)
			{
				*pValue *= nVal;
				break;
			}
		}
		else
		{
			if (pTargeted->EstimatedHealth <= pTargeted->GetType()->Strength / 2)
			{
				*pValue *= nVal;
				break;
			}
		}
	}
	break;
	case Threat:
	{
		*pValue *= *pValue / nVal;
	}
	break;
	default:
	{
		if (nVHP != NewVHPVal::Health)
		{
			if (nVHP == NewVHPVal::Damage)
			{
				*pValue = pTargeted->CombatDamage() / nVal * (*pValue);
				break;

			}

			if (nVHP == NewVHPVal::Value)
			{
				auto const nWeapon = pTargeted->SelectWeapon(pAttacker);
				auto const nFireError = pTargeted->GetFireError(pAttacker, nWeapon, false);
				if (nFireError == FireError::OK &&
					nFireError != FireError::FACING &&
					nFireError != FireError::REARM &&
					nFireError != FireError::ROTATING
					)
				{
					*pValue /= nVal;
					break;
				}

				*pValue *= nVal;
				break;
			}
		}

		if (auto pTargetType = pTargeted->GetTechnoType())
		{
			if (pTargeted->EstimatedHealth > pTargetType->Strength / 2)
			{
				*pValue *= nVal;
				break;
			}
		}
		else
		{
			if (pTargeted->EstimatedHealth > pTargeted->GetType()->Strength / 2)
			{
				*pValue *= nVal;
				break;
			}
		}

		*pValue /= nVal;
	}
	break;
	}

	return  0x6F875F;
}

/* not really sure :s*/
DEFINE_HOOK(0x5F6CD0, ObjectClass_CanCrush_CrushLevel, 0x5)
{
	GET(ObjectClass*, pThis, ECX);
	GET_STACK(FootClass*, pCrusher, 0x4);

	bool IsOmni = false;
	bool CanCrush = true;

	if (!pThis || !pCrusher)
		CanCrush = false;

	if (auto pThisTech = generic_cast<TechnoClass*>(pThis))
	{
		if (pCrusher->GetTechnoType()->OmniCrusher)
		{
			IsOmni = true;
			CanCrush = pThisTech->GetTechnoType()->OmniCrushResistant;
		}

		if (!IsOmni)
		{
			if (!pThisTech->GetTechnoType()->Crushable)
				CanCrush = false;

			if (!pThisTech->Uncrushable)
				CanCrush = false;
		}

		if (pThisTech->WhatAmI() == AbstractType::Building)
			CanCrush = false;

		if (auto pThisInf = specific_cast<InfantryClass*>(pThis))
			if (pThisInf->IsDeployed() && !pThisInf->Type->DeployedCrushable)
				CanCrush = false;

		if (pThisTech->IsIronCurtained())
			CanCrush = false;
	}
	else
	{
		if (!pThis->GetType()->Crushable)
			CanCrush = false;
	}

	if (pCrusher->GetOwningHouse())
		if (pCrusher->GetOwningHouse()->IsAlliedWith(pThis))
			CanCrush = false;


	R->AL(CanCrush);
	return 0x5F6D89;
}

namespace RulesExtTemp_
{
	Nullable<ColorStruct> BuildingLineColor;
}

DEFINE_HOOK(0x6F6186, TechnoClass_DrawLinesOnBuildings_BL, 0x6)
{
	if (RulesExtTemp_::BuildingLineColor.isset() && RulesExtTemp_::BuildingLineColor.Get() != ColorStruct::Empty)
		R->Stack(0x14, RulesExtTemp_::BuildingLineColor.Get().ToInit());

	bool bDontDrawColor = false;

	return bDontDrawColor ? 0x6F6494 : 0x0;
}

DEFINE_HOOK(0x6F542F, TechnoClass_DrawSelectionRelated_BL, 0x7)
{
	if (RulesExtTemp_::BuildingLineColor.isset() && RulesExtTemp_::BuildingLineColor.Get() != ColorStruct::Empty)
		R->Stack(0x10, RulesExtTemp_::BuildingLineColor.Get().ToInit());

	return 0x0;
}

DEFINE_HOOK(0x6F5458, TechnoClass_DrawSelectionRelated_SkipBL, 0x9)
{
	return (R->EAX<int>() == 15 || (RulesExtTemp_::BuildingLineColor.isset() && RulesExtTemp_::BuildingLineColor.Get() != ColorStruct::Empty)) ?
		0x6F5461 : 0x6F5645;
}

DEFINE_HOOK(0x6F569D, TechnoClass_DrawSelectionRelated_SkipBL_FKWW, 0x7)
{
	return  (RulesExtTemp_::BuildingLineColor.isset() && RulesExtTemp_::BuildingLineColor.Get() != ColorStruct::Empty) ? 0x6F5E10 : 0x0;
}

/* this one on the root
* my hook do after this called !
DEFINE_HOOK(0x6F3B2E, TechnoClass_Transform_FLH, 0x6)
{
	GET(WeaponStruct, nWeaponStruct, EAX);
	GET(TechnoClass*, pThis, EBX);
	GET_STACK(int, idxWeapon, 0x8);

	CoordStruct nRet = nWeaponStruct.FLH;

	if (auto pInf = specific_cast<InfantryClass*>(pThis))
	{
		if (pInf->Crawling)
		{
			auto const pTypeExt = TechnoTypeExt::ExtMap.Find(pThis->GetTechnoType());
			auto const& nFLH = pTypeExt->CrawlFLHData;

			if (!pThis->Veterancy.IsElite())
			{
				if (idxWeapon == 0)
					nRet = nFLH.PrimaryCrawlFLH.Get(nWeaponStruct.FLH);
				else
					nRet = nFLH.SecondaryCrawlFLH.Get(nWeaponStruct.FLH);
			}
			else
			{
				if (idxWeapon == 0)
					nRet = nFLH.Elite_PrimaryCrawlFLH.Get(nWeaponStruct.FLH);
				else
					nRet = nFLH.EliteSe_condaryCrawlFLH.Get(nWeaponStruct.FLH);
			}
		}
	}

	R->ECX(nRet.X);
	R->EBP(nRet.Y);
	R->EAX(nRet.Z);

	return 0x6F3B37;
}*/

namespace ROFTemp
{
	Valueable<bool> ROF_Random;
	Nullable<Point2D> Rof_RandomMinMax;
}



namespace PermaDisgue_Temp
{
	Valueable<bool>PermaDisguise_ShowSelf { true };
};

DEFINE_HOOK(0x70ED8E, TechnoClass_GetDisguiseFlags_DisguiseShowSelf, 0x6)
{
	//GET(TechnoClass*, pThis, EDI);
	//auto const pType = pThis->GetTechnoType();
	return PermaDisgue_Temp::PermaDisguise_ShowSelf.Get() ? 0x0 : 0x70EE1B;
}

DEFINE_HOOK(0x70EE87, TechnoClass_IsClearlyVisibleTo_ShowSelf, 0x5)
{	//GET(TechnoClass*, pThis, ESI);
	//auto const pType = pThis->GetTechnoType();
	return PermaDisgue_Temp::PermaDisguise_ShowSelf.Get() ? 0x0 : 0x70EEAF;
}

namespace temp_underwater
{
	Valueable<bool> Underwater_ShowWake;
}

DEFINE_HOOK(0x69FE6E, TechnoClass_Underwater_Change, 0x6)
{
	GET(FootClass*, pThis, ECX);

	return temp_underwater::Underwater_ShowWake.Get() ? 0x69FE76 : 0x0;
}

/*
DEFINE_HOOK(0x69FE92, ShipLocomotionClass_WakeAnim, 0x5)
{
	GET(FootClass*, pThis, ECX);

	return 0x69FEF0;
}*/

namespace TechExt_SmartDeploy
{
	bool IsDeployed { false };
	//AIDeploysSmart ->Rules
}
static DWORD SmartDeploy(UnitClass* pThis, CellStruct* pWhere)
{
	if (!pThis || !pThis->Owner || pThis->Owner->IsPlayer())
		return 0x0;

	auto const nHeight = pThis->Type->DeploysInto->GetFoundationHeight(false);
	auto const nWidth = pThis->Type->DeploysInto->GetFoundationWidth();
	auto nMapCoords = pThis->GetMapCoords();
	auto const nZone = MapClass::Instance->Zone_56D230(&nMapCoords, pThis->Type->MovementZone, false);
	auto const nNear = MapClass::Instance->NearByLocation(nMapCoords, pThis->Type->SpeedType, nZone, pThis->Type->MovementZone, false, nWidth, nHeight, true, false, false, false, nMapCoords, false, true);
	auto const pCell = MapClass::Instance->TryGetCellAt(nNear);

	pThis->Type->DeploysInto->FlushPlacement(pWhere, pThis->Owner);

	auto const pLoco = pThis->Locomotor.get();
	pLoco->Mark_All_Occupation_Bits(1);
	pThis->SetDestination(pCell, true);
	pThis->QueueMission(Mission::Move, true);

	TechExt_SmartDeploy::IsDeployed = true;
}

static void SmartDeploy_AI(TechnoClass* pThis)
{
	if (auto pUnit = specific_cast<UnitClass*>(pThis))
	{
		if (TechExt_SmartDeploy::IsDeployed)
		{
			auto const pLoco = pUnit->Locomotor.get();
			if (!pLoco->Is_Moving())
			{
				TechExt_SmartDeploy::IsDeployed = false;
				pUnit->ClickedEvent(NetworkEvents::Deploy);
			}
		}
	}
}

static void DamageSelfAuto_AI(TechnoClass* pThis)
{
	bool DamageSelf_Auto = false;
	if (auto pInf = specific_cast<InfantryClass*>(pThis))
	{
		if (DamageSelf_Auto && !pThis->Target && pThis->CurrentMission == Mission::Guard)
		{
			if (pInf->SequenceAnim == DoType::Ready)
			{
				pInf->SetTarget(pInf);
			}
		}
	}
}


DEFINE_HOOK(0x7394E0, UnitClass_TryToDeploy_SmarterDeploy, 0x6)
{
	//GET(UnitClass*, pThis, EBP);
	//GET_STACK(CellStruct, nCell, 0x14);

	return 0x739580;
}

DEFINE_HOOK(0x711F60, TechnoTypeClass_GetRefund_NoRefund, 0x9)
{
	GET(TechnoTypeClass*, pThis, ECX);

	bool noRefund = false;

	if (noRefund)
	{
		R->EAX(0);
		return 0x712036;
	}

	return 0x0;
}

DEFINE_HOOK(0x702D6D, TechnoClass_RegisterDestruction_Veterancy, 0x8)
{

	return 0x0;
}
DEFINE_HOOK(0x7EE1B6, TechnoClass_GetPrimaryWeapon_FixWrongGet, 0x6)
{
	GET(TechnoClass*, pThis, ESI);

	if (pThis->GetTechnoType()->IsGattling)
		R->EAX(2 * pThis->CurrentGattlingStage);
	else
		R->EAX(pThis->CurrentWeaponNumber);

	return 0x70E1BC;
}

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
/*
DEFINE_HOOK(0x6D4B62, TacticalClass_PrintTimer_ReplaceForSW, 0xA)
{
	if (!SWTimerTemp::Super || !SWTimerTemp::SuperExt)
		return 0x0;

	GET_STACK(int, nInterval, STACK_OFFS(0x644, -0x4));
	//not sure if this used ? , vanilla does this ,.. so
	LEA_STACK(wchar_t*, pWideBuffer_1, STACK_OFFS(0x644, 0x200)); //size 256
	REF_STACK(wchar_t, nWideBuffer, STACK_OFFS(0x644, 0x400));
	LEA_STACK(wchar_t*, pWideBuffer_2, STACK_OFFS(0x644, 0x600)); //size 256
	GET_STACK(char*, pString, STACK_OFFS(0x644, -0x8));

	auto const pFont = BitFont::BitFontPtr(
		TextPrintType::UseGradPal |
		TextPrintType::Right |
		TextPrintType::NoShadow |
		TextPrintType::Metal12 |
		TextPrintType::unk400);

	R->EBX(pFont);
	std::wstring lpTime;

	if (!SWTimerTemp::SuperExt->PercentTimer.Get())
	{
		auto const nHour = nInterval / 60 / 60;
		auto const nSeconds = nInterval % 60;
		auto const nMinute = (nInterval / 60 % 60);

		if (nHour)
		{
			_snwprintf(pWideBuffer_1, 256, L"%s  %d:%02d:%02d", pString, nHour, nMinute, nSeconds);
			_swprintf(&nWideBuffer, L"%s  ", pString);
			_snwprintf(pWideBuffer_2, 256, L"%d:%02d:%02d", nHour, nMinute, nSeconds);
		}
		else
		{
			_snwprintf(pWideBuffer_1, 256, L"%s  %02d:%02d", pString, nMinute, nSeconds);
			_swprintf(&nWideBuffer, L"%s  ", pString);
			_snwprintf(pWideBuffer_2, 256, L"%02d:%02d", nMinute, nSeconds);
		}
	}
	else
	{
		int const nTimeLeft = SWTimerTemp::Super->RechargeTimer.GetTimeLeft();
		double const nTimePrec = (((nTimeLeft * 1.0) / SWTimerTemp::Super->Type->RechargeTime) * 100.0);
		double const nRec = !SWTimerTemp::SuperExt->PercentTimer_Backwards.Get() ? (100.0 - nTimePrec) : abs(nTimePrec);

		_snwprintf(pWideBuffer_1, 256, L"%s  %.0f %%", pString, nRec);
		_swprintf(&nWideBuffer, L"%s  ", pString);
		_snwprintf(pWideBuffer_2, 256, L"%s  %.0f %%", nRec);
	}

	return 0x6D4C54;
}
*/


DEFINE_HOOK(0x6FE248, TechnoClass_Fire_AroundTarget, 0x5)
{
	return 0x0;
}


/*
DEFINE_HOOK(0x45933D, BuildingClass_BunkerUpSound_1, 0x5)
{
	GET(BuildingClass*, pThis, ESI);
	BunkerPlaySound(pThis, true);
	return 0x459374;
}

DEFINE_HOOK(0x4595D9, BuildingClass_BunkerUpSound_2, 0x5)
{
	GET(BuildingClass*, pThis, EDI);
	BunkerPlaySound(pThis, true);
	return 0x459612;
}

DEFINE_HOOK(0x459494, BuildingClass_BunkerDownSound, 0x5)
{
	GET(BuildingClass*, pThis, ESI);
	BunkerPlaySound(pThis);
	return 0x4594CD;
}
