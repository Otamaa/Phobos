/*
		separate this , mixing up them causing game to desync which result of "undefine" behaviour
*/

#include <Ext/Techno/Body.h>
#include <Ext/TechnoType/Body.h>
#include <Ext/Building/Body.h>
#include <Ext/BulletType/Body.h>
#include <Utilities/Macro.h>

#include <Misc/Otamaa/Misc/DynamicPatcher/Techno/GiftBox/GiftBoxFunctional.h>
//#include <Misc/Otamaa/Misc/DynamicPatcher/Techno/JumjetFaceTarget/JJFacingToTargetFunctional.h>
//#include <Misc/Otamaa/Misc/DynamicPatcher/Techno/DriveData/DriveDataFunctional.h>
//#include <Misc/Otamaa/Misc/DynamicPatcher/Techno/Passengers/PassengersFunctional.h>
//#include <Misc/Otamaa/Misc/DynamicPatcher/Techno/AircraftDive/AircraftDiveFunctional.h>
//#include <Misc/Otamaa/Misc/DynamicPatcher/Techno/AircraftPut/AircraftPutDataFunctional.h>

/**/
DEFINE_HOOK(0x6F9E50, TechnoClass_AI_, 0x5)
{
	GET(TechnoClass*, pThis, ECX);

	auto const pExt = TechnoExt::ExtMap.Find(pThis);
	auto const pTypeExt = TechnoTypeExt::ExtMap.Find(pThis->GetTechnoType());

	//if (!pExt || !pTypeExt)
	//	return 0;

	TechnoExt::UpdateMindControlAnim(pThis);
	TechnoExt::ApplyMindControlRangeLimit(pThis);
	TechnoExt::ApplyInterceptor(pThis);
	TechnoExt::ApplySpawn_LimitRange(pThis);
	TechnoExt::CheckDeathConditions(pThis);
	TechnoExt::EatPassengers(pThis);
	GiftBoxFunctional::AI(pExt, pTypeExt);

	return 0;
}

static void __fastcall AircraftClass_AI_(AircraftClass* pThis, void* _)
{

	if (pThis->Type->OpenTopped)
	{
		for (NextObject object(pThis->Passengers.GetFirstPassenger()); object; ++object)
		{
			if (auto const pInf = generic_cast<FootClass*>(*object))
			{
				if (!pInf->Transporter || !pInf->InOpenToppedTransport)
				{
					pThis->EnteredOpenTopped(pInf);
					pInf->Transporter = pThis;
					pInf->Undiscover();
				}
			}
		}
	}

	auto const pFoot = static_cast<FootClass*>(pThis);
	auto const pExt = TechnoExt::ExtMap.Find(pThis);
	auto const pTypeExt = TechnoTypeExt::ExtMap.Find(pThis->GetTechnoType());

	if (pExt && pTypeExt)
	{
		//DriveDataFunctional::AI(pExt);
		//AircraftDiveFunctional::AI(pExt, pTypeExt);

	}

	pFoot->AI();
}

DEFINE_POINTER_CALL(0x414DA3, &AircraftClass_AI_);

DEFINE_HOOK(0x4DA63B, FootClass_AI_AfterRadSite, 0x6)
{
	GET(FootClass*, pThis, ESI);

	auto const pExt = TechnoExt::ExtMap.Find(pThis);
	auto const pTypeExt = TechnoTypeExt::ExtMap.Find(pThis->GetTechnoType());

	//if (pThis->GetTechnoType()->RevealToAll)
	//{
	//	pThis->UpdateSight(false, 0, true, HouseClass::Player(), 0);
	//	CoordStruct nBuff;
	//	pThis->GetPosition_0(&nBuff);
	//	MapClass::Instance->RevealArea3(&nBuff, 0, pThis->LastSightRange + 3, false);
	//}

	if (const auto pTargetTech = abstract_cast<TechnoClass*>(pThis->Target))
	{
		//Spawnee trying to chase Aircraft that go out of map until it reset
		//fix this , so reset immedietely if target is not on map
		if (pThis->SpawnOwner && (!pTargetTech->IsOnMap || !pTargetTech->IsInPlayfield
			|| pTargetTech->TemporalTargetingMe))
		{
			pThis->SpawnOwner->SetTarget(nullptr);
			pThis->SpawnOwner->SpawnManager->ResetTarget();
		}
	}

	if (pExt && pTypeExt)
	{
		//PassengersFunctional::AI(pThis);
		//JJFacingFunctional::AI(pExt, pTypeExt);
		//TrailsManager::AI(pThis);
	}

	return pThis->IsLocked ? 0x4DA677 : 0x4DA643;
}

DEFINE_HOOK(0x4DA698, FootClass_AI_IsMovingNow, 0x8)
{
	GET(FootClass*, pThis, ESI);
	bool const IsMovingNow = R->AL();

	if (IsMovingNow)
	{
		auto const pExt = TechnoExt::ExtMap.Find(pThis);
		//auto const pTypeExt = TechnoTypeExt::ExtMap.Find(pThis->GetTechnoType());

		// LaserTrails update routine is in TechnoClass::AI hook because TechnoClass::Draw
		// doesn't run when the object is off-screen which leads to visual bugs - Kerbiter
		if (pExt && !pExt->LaserTrails.empty())
		{
			for (auto const& trail : pExt->LaserTrails)
			{
				if (pThis->WhatAmI() == AbstractType::Aircraft && !pThis->IsInAir() && trail->LastLocation.isset())
					trail->LastLocation.Reset();

				trail->Update(TechnoExt::GetFLHAbsoluteCoords(pThis, trail->FLH, trail->IsOnTurret));
			}
		}

		return 0x4DA6A0;
	}

	return 0x4DA7B0;
}

// this updated after TechnoClass::AI
// then check if the techno itself is still active/alive/present
DEFINE_HOOK(0x43FE69, BuildingClass_AI_Add, 0xA)
{
	GET(BuildingClass*, pThis, ESI);

	auto const pExt = BuildingExt::ExtMap.Find(pThis);
	auto const pTypeExt = BuildingTypeExt::ExtMap.Find(pThis->Type);
	auto const pTypeData = TechnoTypeExt::ExtMap.Find(pThis->Type);

	if (pTypeExt && pExt && pTypeData)
	{
		//TechnoExt::ApplyPowered_KillSpawns
		if (pTypeData->Powered_KillSpawns && pThis->Type->Powered && !pThis->IsPowerOnline())
		{
			if (auto pManager = pThis->SpawnManager)
			{
				pManager->ResetTarget();
				for (auto pItem : pManager->SpawnedNodes)
				{
					if (pItem->Status == SpawnNodeStatus::Attacking || pItem->Status == SpawnNodeStatus::Returning)
					{
						if (pItem->Unit)
							pItem->Unit->ReceiveDamage(&pItem->Unit->Health, 0,
								RulesClass::Instance()->C4Warhead, nullptr, false, false, nullptr);
					}
				}
			}
		}

		auto const pRulesExt = RulesExt::Global();
		auto const nMission = pThis->GetCurrentMission();

		if (pTypeExt->AnotherTypeData.AutoSellTime.Get(0.0f) > 0.0f && nMission != Mission::Selling && !pThis->Type->Unsellable)
		{
			if (pExt->AnotherData.AutoSellTimer.StartTime == -1 || nMission == Mission::Attack)
				pExt->AnotherData.AutoSellTimer.Start((int)(pTypeExt->AnotherTypeData.AutoSellTime.Get() * 900.0));
			else
				if (pExt->AnotherData.AutoSellTimer.Completed())
					pThis->Sell(-1);
		}

		if (!pRulesExt->AnotherData.AI_AutoSellHealthRatio.empty() && pRulesExt->AnotherData.AI_AutoSellHealthRatio.size() <= 3 && (nMission != Mission::Selling && !pThis->Type->Unsellable))
			if (pThis->Owner && !pThis->Owner->IsPlayer() && !pThis->Owner->Type->MultiplayPassive && pRulesExt->AnotherData.AI_AutoSellHealthRatio.at(pThis->Owner->GetCorrectAIDifficultyIndex()) > 0.0f)
				if ((pRulesExt->AnotherData.AI_AutoSellHealthRatio.at(pThis->Owner->GetCorrectAIDifficultyIndex()) > pThis->GetHealthPercentage()) && (pThis->EstimatedHealth < 0))
					pThis->Sell(-1);
	}

	return 0x0;
}

DEFINE_HOOK(0x702050, TechnoClass_Destroy, 6)
{
	GET(TechnoClass*, pThis, ESI);
	auto const pTypeExt = TechnoTypeExt::ExtMap.Find(pThis->GetTechnoType());
	auto const pExt = TechnoExt::ExtMap.Find(pThis);

	GiftBoxFunctional::Destroy(pExt, pTypeExt);
	return 0;
}

DEFINE_HOOK(0x6F42ED, TechnoClass_Init_DP, 0xA)
{
	GET(TechnoClass*, pThis, ESI);

	auto const pTypeExt = TechnoTypeExt::ExtMap.Find(pThis->GetTechnoType());
	auto const pExt = TechnoExt::ExtMap.Find(pThis);

	GiftBoxFunctional::Init(pExt, pTypeExt);

	return 0x0;
}