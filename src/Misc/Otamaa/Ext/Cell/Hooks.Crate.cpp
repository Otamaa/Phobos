#include <ScenarioClass.h>
#include <HouseClass.h>
#include <SuperClass.h>
#include <WarheadTypeClass.h>
#include <BulletTypeClass.h>
#include <BulletClass.h>
#include <AnimClass.h>

#include "Body.h"

#include <Utilities/Macro.h>
#include <Ext/TechnoType/Body.h>
#include <New/Type/CrateTypeClass.h>
#include <Ext/Techno/Body.h>
#include <Ext/Anim/Body.h>
#include <Ext/AnimType/Body.h>

/*
			Crate Power Up Extended
https://www.modenc.renegadeprojects.com/Powerups
Format Original :
	Crate_Type = Chance ,Animation ,Water Allowed ,	Special Parameter

*/

namespace CrateStuffs
{
	enum ePowerup : unsigned char
	{
		Money,
		Unit,
		HealBase,
		Cloak,
		Explosion,
		Napalm,
		Squad,
		Darkness,
		Reveal,
		Armor,
		Speed,
		Firepower,
		ICBM,
		Invulnerability,
		Veteran,
		IonStorm,
		Gas,
		Tiberium,
		Pod
	};

	unsigned char EnumOfIndex(int i)
	{
		return static_cast<ePowerup>(i);
	}
};
//skip WW Debug log
//481F75
//WW debung call 0x481F7C
//DEFINE_LJMP(0x481F75, 0x481F81);

//Shroud easy to handle without breaking everything else
//jump goes to the end of the function because need to replace the animation
//for more fit with the stuffs
DEFINE_HOOK(0x481F87, CellClass_CrateCollected_Shroud_Override, 0x7)
{
	if (CrateTypeClass::Array.empty())
	{
		Debug::Log("CrateType is empty return 0 \n");
		return 0;
	}

	GET(TechnoClass*, pCollector, EAX);

	bool bPass = false;//return default

	if (auto const pHouse = pCollector->Owner)
	{
		auto& nCreateType = CrateTypeClass::Array;
		auto Powerups_Animarray = Make_Pointer<int>(0x81DAD8);
		CellStruct nCellBuffer = { static_cast<short>(R->EDX()),static_cast<short>(R->ECX()) };
		auto pCell = MapClass::Instance->TryGetCellAt(nCellBuffer);
		auto nHeight = MapClass::Instance->GetCellFloorHeight(CellClass::Cell2Coord(nCellBuffer));
		auto nAnimCoords = CellClass::Cell2Coord(nCellBuffer, 200 + nHeight);
		auto nDice = ScenarioClass::Instance->Random.RandomRanged(0, nCreateType.size() - 1); //Pick  random from array
		auto nPickedUpDice = ScenarioClass::Instance->Random.RandomRanged(0, RulesClass::Instance->CrateMaximum);
		bool bAllowSpawn = abs(nCreateType[nDice]->Chance.Get()) < nPickedUpDice && abs(nCreateType[nDice]->Chance.Get()) > 0;

		if (CellExt::ExtMap.Find(pCell)->NewPowerups >= 0)
		{
			nDice = abs(CellExt::ExtMap.Find(pCell)->NewPowerups);
			//dice = dice - 1;
			Debug::Log("Crate type Check cell which to spawn [%d]\n", nDice);
			bAllowSpawn = true; //forced
		}

		bool bLandTypeEligible = false;
		auto pAnimtype = nCreateType[nDice]->Anim.Get();
		auto nSoundIdx = nCreateType[nDice]->Sound.Get();
		auto nEvaIndex = nCreateType[nDice]->Eva.Get();
		bool bNotObserver = !pHouse->IsObserver() || !pHouse->IsPlayerObserver();
		auto bIsWater = pCell->LandType == LandType::Water && pCell->Tile_Is_Water() && pCell->Tile_Is_Wet();
		bLandTypeEligible = (bIsWater && nCreateType[nDice]->AllowWater.Get()) || !bIsWater;

		if (bAllowSpawn && bLandTypeEligible)
		{
			switch (nCreateType[nDice]->Type.Get())
			{
			case 1: //Super Weapon
			{
				bool bAllow = true;
				if (SessionClass::Instance->GameMode == GameMode::Skirmish)
					bAllow = GameModeOptionsClass::Instance->SWAllowed;

				if (bAllow)
				{
					if (auto pSuperType = nCreateType[nDice]->Super.Get())
					{
						if (auto pSuper = pHouse->Supers.GetItem(pSuperType->ArrayIndex))
						{
							if (nCreateType[nDice]->SuperGrant.Get())
							{
								if (pSuper->Grant(true, bNotObserver, false))
								{
									if (bNotObserver && pHouse->ControlledByPlayer())
									{
										if (MouseClass::Instance->AddCameo(AbstractType::Special, pSuperType->ArrayIndex))
											MouseClass::Instance->RepaintSidebar(1);
									}
								}
							}
							else
							{
								//Abused By AI ?
								pSuper->IsCharged = true;
								pSuper->Launch(pCell->MapCoords, true);
								pSuper->IsCharged = false;

							}
							bPass = true;
						}
					}
				}
			}
			break;
			case 2: //Weapon
			{
				auto pWeapon = nCreateType[nDice]->WeaponType.Get();
				if (pWeapon && (!pWeapon->Warhead->MindControl || !pWeapon->LimboLaunch))
				{
					if (auto pBulletC = pWeapon->Projectile->CreateBullet(pCell, pCollector, pWeapon->Damage, pWeapon->Warhead, pWeapon->Speed, pWeapon->Bright))
					{
						pBulletC->SetWeaponType(pWeapon);
						pBulletC->SetLocation(CellClass::Cell2Coord(nCellBuffer));
						pBulletC->Explode();
						pBulletC->Limbo();
						pBulletC->UnInit();

						bPass = true;
					}
				}
			}
			break;
			case 3: //case 3 is overrided reshroud
			{
				if (!pAnimtype)
					pAnimtype = AnimTypeClass::Array->GetItem(Powerups_Animarray[7]);

				MapClass::Instance->Reshroud(pHouse);
				bPass = true;
			}
			break;
			case 4: //random Unit
			{
				if (auto nUnitList = nCreateType[nDice]->Unit.GetElements())
				{
					if (auto const pUnit = static_cast<TechnoClass*>(nUnitList.at(ScenarioClass::Instance->Random() % nUnitList.size())->CreateObject(pHouse)))
					{
						//   auto TrygeteligibleArea = TechnoExt::GetPutLocation(CellClass::Cell2Coord(BufferCellStruct), 6); //try to not get stuck
						auto facing = static_cast<short>(ScenarioClass::Instance->Random(0, 255));
						++Unsorted::IKnowWhatImDoing;
						auto succes = pUnit->Unlimbo(CellClass::Cell2Coord(nCellBuffer), facing);
						--Unsorted::IKnowWhatImDoing;

						if (!succes && pUnit)
							pUnit->UnInit();
							//GameDelete(pUnit);

						pUnit->QueueMission(Mission::Guard, 1);
						pUnit->NextMission();


						if (bNotObserver && !pHouse->Type->MultiplayPassive)
							pHouse->RecheckTechTree = true;

						bPass = true;
					}
				}
			}
			break;
			case 5: //Money
			{
				if (!pAnimtype)
					pAnimtype = AnimTypeClass::Array->GetItem(Powerups_Animarray[0]);

				if (nSoundIdx < 0)
					nSoundIdx = RulesClass::Instance->CrateMoneySound;

				auto nMoneyMin = abs(nCreateType[nDice]->MoneyMin.Get());
				auto nMoneyMax = abs(nCreateType[nDice]->MoneyMax.Get());
				if (nMoneyMax > 0 && nMoneyMax > nMoneyMin)
				{
					pHouse->GiveMoney(abs(ScenarioClass::Instance->Random.RandomRanged(nMoneyMin, nMoneyMax)));
					bPass = true;
				}
			}
			break;
			default:
				break;
			}

			if (bPass)
			{
				if (pAnimtype)
					if (auto pAnim = GameCreate<AnimClass>(pAnimtype, nAnimCoords))
						AnimExt::SetAnimOwnerHouseKind(pAnim, nullptr, pCollector->Owner);

				if (nSoundIdx >= 0)
					VocClass::PlayAt(nSoundIdx, nAnimCoords, nullptr);

				if (nEvaIndex >= 0)
					if (pHouse->ControlledByPlayer() && bNotObserver)
						VoxClass::PlayAtPos(nEvaIndex, &nAnimCoords);
			}
		}
		else
		{
			//   return 0x481AD3;
			   //reroll it instead
			   //reroll may cause score to be double each re-roll which is undesirable
			   //for online matching

			if (!pAnimtype)
				pAnimtype = AnimTypeClass::Array->GetItem(Powerups_Animarray[0]);

			if (!nSoundIdx)
				nSoundIdx = RulesClass::Instance->CrateMoneySound;

			pHouse->GiveMoney(RulesClass::Instance->SoloCrateMoney);

			bPass = true;
		}
	}

	return bPass ? 0x483389 : 0x0;
}

/*
		Building And Unit Spesific Crates
		ToDo: Infantry Spesific Crate;
			 allow put crate on top of creaters
*/

//force spawn crate for testings
/*disable Scenario checking
DEFINE_HOOK(738390, CellClass_Crate_To_Create, 6)
{
	GET(UnitTypeClass*, pUnit, EAX);
	return pUnit->CarriesCrate ? 0x7383D4 : 0x738457;
}
*/

DEFINE_HOOK(0x73844A, UnitClass_ReceiveDamage_PlaceCrate_override, 0x6)
{
	GET(CellStruct, Where, EAX);
	GET(UnitClass*, pthis, ESI);

	auto nCrateType = TechnoTypeExt::ExtMap.Find(pthis->GetTechnoType())->CrateType;
	nCrateType = abs(nCrateType);

	auto Success = MapClass::Instance->Place_Crate(Where, nCrateType);
	Debug::Log(__FUNCTION__"Unit[%s] to crate [%d] X [%d] Y[%d] succes [%d]\n", pthis->Type->ID, nCrateType, Where.X, Where.Y, Success);
	return 0x738457;
}

DEFINE_HOOK(0x442215, BuildingTypeClass_Destroy_PlaceCrate_override, 0x7)
{
	GET(BuildingTypeClass*, pBldType, EDX);
	GET(BuildingClass *, Building, EBX);

	auto nCrateType = TechnoTypeExt::ExtMap.Find(pBldType)->CrateType;
	nCrateType = abs(nCrateType);

	auto Success = MapClass::Instance->Place_Crate(Building->GetCell()->MapCoords, nCrateType);
	Debug::Log(__FUNCTION__"Building[%s] to crate [%d] succes ? [%d] \n", pBldType->ID, nCrateType, Success);
	R->AL(Success);

	return 0x442226;
}


/*	Important Part to further Extend the Crate Types without rewrite whole functions
	NeedCheck : How Does this react with CellChainReact from Ares ?
*/
/**/
DEFINE_LJMP(0x56BFC2, 0x56BFC7);

DEFINE_HOOK(0x56C1D3, MapClass_RemoveCrate_Override, 0x9)
{
	GET(CellClass*, pThis, EBX);

	CellExt::ExtMap.Find(pThis)->NewPowerups = -1;

	return 0;
}

DEFINE_HOOK(0x56BFF9, MapClass_PlaceCrate_Override, 0x6)
{
	GET(CellClass*, pThis, EAX);
	GET(int, OverlayData, EDX);
	auto pCellExt = CellExt::ExtMap.Find(pThis);

	//custom powerup
	auto nNewPowerUp = OverlayData > 20 ? (OverlayData - 20) : (-1);
	//original powerup
	OverlayData = Math::min(OverlayData, 20);
	OverlayData = OverlayData == 19 ? 0 : OverlayData;

	pThis->OverlayData = CrateStuffs::EnumOfIndex(OverlayData);
	pCellExt->NewPowerups = nNewPowerUp;

	Debug::Log(__FUNCTION__"NewPowerUps [%d] Original PowerUps[%d] cell NewPowerups [%d] \n", nNewPowerUp, OverlayData, pCellExt->NewPowerups);
	return 0x56BFFF;
}

DEFINE_HOOK(0x481ACE, CellHasPowerUp_Override, 0x5)
{
	GET(CellClass*, pThis, ESI);

	enum { keeproll = 0x481AD3, SpawnSpesific = 0x481B22 };

	auto nOverlayData = pThis->OverlayData;
	auto pCellExt = CellExt::ExtMap.Find(pThis);
	auto nNewPowerup = pCellExt->NewPowerups;

	Debug::Log(__FUNCTION__"Original PowerUps [%d] cell NewPowerups [%d] \n", nOverlayData, nNewPowerup);

	if (nNewPowerup > -1 && !CrateTypeClass::Array.empty())
	{
		R->EBX(CrateStuffs::EnumOfIndex(7));//force spawn Darkness
		return SpawnSpesific;
	}
	else
	{
		if (nOverlayData < 19)
		{
			R->EBX(nOverlayData);
			return SpawnSpesific;
		}
	}

	return keeproll;
}

//4A18F0 , CrateImage
//put random crate -> set crate + image after rol
//-> set powerup type/new powerup
//->collected
//->spawn spesific
