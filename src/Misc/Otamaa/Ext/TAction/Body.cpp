#include <Ext/TAction/Body.h>
#include <Ext/Techno/Body.h>

#include <TagClass.h>

enum class CnpTriggerAction : unsigned int
{
	SetVeterancy = 700,
	SpendMoney,
	RunSuperWeaponAtWaypoint, //lol
	ActivateAIMode,
	PlayAnimOnScreen,
	SetFootDestionation,
	DoWeaponDetonate

};

namespace  TActionExt_Temp
{
	static bool CNP_TAction_700(TActionClass* pThis, HouseClass* pHouse, ObjectClass* pObject,
		TriggerClass* pTrigger, CellStruct const& location)
	{
		if (pTrigger)
		{
			auto const nVal = Math::clamp(pThis->Value, 0, 2);
			for (auto const pTech : *TechnoClass::Array)
			{
				if (TechnoExt::IsReallyAlive(pTech))
				{
					if (auto const pTag = pTech->AttachedTag)
					{
						if (pTag->ContainsTrigger(pTrigger))
						{
							switch (nVal)
							{
							case 1:
								pTech->Veterancy.SetElite();
								return true;
							case 2:
								pTech->Veterancy.SetVeteran();
								return true;
							default:
								pTech->Veterancy.SetRookie();
								return true;
							}
						}
					}
				}
			}
		}

		return false;
	}

	static bool CNP_TAction_701(TActionClass* pThis, HouseClass* pHouse, ObjectClass* pObject,
		TriggerClass* pTrigger, CellStruct const& location)
	{
		if (pTrigger)
		{
			if (auto const pOwner = pThis->FindHouseByIndex(pTrigger, pThis->Bounds.Y))
			{
				if (pThis->Bounds.X)
				{
					pOwner->TakeMoney(pOwner->Available_Money());
					pOwner->GiveMoney(abs(pThis->Value));
					return true;
				}
				else
				{
					pOwner->TransactMoney(pThis->Value);
					return true;
				}
			}
		}

		return false;
	}

	static bool CNP_TAction_703(TActionClass* pThis, HouseClass* pHouse, ObjectClass* pObject,
	TriggerClass* pTrigger, CellStruct const& location)
	{
		if (pTrigger)
		{
			if (auto const pOwner = pThis->FindHouseByIndex(pTrigger, pThis->Bounds.X))
			{
				pOwner->AIMode = (AIMode)pThis->Value;
				return true;
			}
		}

		return false;
	}

	static bool CNP_TAction_704(TActionClass* pThis, HouseClass* pHouse, ObjectClass* pObject,
	TriggerClass* pTrigger, CellStruct const& location)
	{
		if (pTrigger)
		{
			if (auto const pAnim = AnimTypeClass::Find(pThis->Text))
			{
				if (auto const pImage = pAnim->Image)
				{
					auto const nHeight = pImage->Height;
					auto const nWidth = pImage->Width;
					auto const MapRect_A = Make_Global<int>(0x87F8DC);

					int const n_B30 = MapRect_A * 30;
					int nWidth_ = nWidth / 2 - (n_B30);

					if (!(nWidth_ >= n_B30))
					{
						auto const MapRect_B = Make_Global<int>(0x87F8E0);

						int const n_B45 = 45 * MapRect_B;
						int nHeight_ = 15 * MapRect_B + nHeight / 2;

						do
						{
							if (nHeight_ < n_B45)
							{
								do
								{
									Vector3D<float> nMtxF = { (float)nWidth_,(float)nHeight_,0.0f };
									auto const nOut = Matrix3D::MatrixMultiply(TacticalGlobal->IsoTransformMatrix, nMtxF);
									CoordStruct nCood = { (int)nOut.X,(int)nOut.Y,0 };
									GameCreate<AnimClass>(pAnim, nCood);
									nHeight_ += nHeight;
								}
								while (nHeight_ < n_B45);
							}
							nWidth_ += nWidth;
						}
						while (nWidth_, n_B30);
					}
					return true;
				}
			}
		}

		return false;
	}

	static bool CNP_TAction_705(TActionClass* pThis, HouseClass* pHouse, ObjectClass* pObject,
	TriggerClass* pTrigger, CellStruct const& location)
	{
		if (pTrigger)
		{
			if (auto const pOwner = pThis->FindHouseByIndex(pTrigger, pThis->Value))
			{
				auto const nCoords = ScenarioGlobal->GetWaypointCoords(pThis->Waypoint);
				if (auto const  pCell = Map[nCoords])
				{
					for (auto const pFoot : *FootClass::Array)
					{
						if (TechnoExt::IsReallyAlive(pFoot) && pFoot->Owner == pOwner)
						{
							pFoot->SetDestination(pCell,false);
						}
					}

					return true;
				}
			}
		}

		return false;
	}

	bool CNP_TAction_706(TActionClass* pThis, HouseClass* pHouse, ObjectClass* pObject,
	TriggerClass* pTrigger, CellStruct const& location)
	{
		//use SW fire code
		return true;
	}

	bool Execute(TActionClass* pThis, HouseClass* pHouse, ObjectClass* pObject,
	TriggerClass* pTrigger, CellStruct const& location, bool& bHandled)
	{
		switch (static_cast<CnpTriggerAction>(pThis->ActionKind))
		{
		case CnpTriggerAction::SetVeterancy:
			return CNP_TAction_700(pThis, pHouse, pObject, pTrigger, location);
		case CnpTriggerAction::SpendMoney:
			return CNP_TAction_701(pThis, pHouse, pObject, pTrigger, location);
		case CnpTriggerAction::ActivateAIMode:
			return CNP_TAction_703(pThis, pHouse, pObject, pTrigger, location);
		case CnpTriggerAction::PlayAnimOnScreen:
			return CNP_TAction_704(pThis, pHouse, pObject, pTrigger, location);
		case CnpTriggerAction::SetFootDestionation:
			return CNP_TAction_705(pThis, pHouse, pObject, pTrigger, location);
		case CnpTriggerAction::DoWeaponDetonate:
			return CNP_TAction_706(pThis, pHouse, pObject, pTrigger, location);
		}
		return nullptr;
	}

}

enum class CnpTriggerEvent : unsigned int
{
	CheckTechno = 700,
	ObjectExist,
	ObjectHealthTreshold,
};

namespace  TEventExt_Temp
{
	bool CheckTechno(TEventClass* pThis, HouseClass* pHouse)
	{
		AbstractType nOpt = (AbstractType)atoi(pThis->String);
		auto count = pThis->Value;

		switch (nOpt)
		{
		case AbstractType::Aircraft:
		{
			for (auto const pAir : *AircraftClass::Array)
			{
				if (pAir->Owner == pHouse)
				{
					count--;

					if (count <= 0)
					{
						return true;
					}
				}
			}
		}
		break;
		case AbstractType::Unit:
		{
			for (auto const pAir : *UnitClass::Array)
			{
				if (pAir->Owner == pHouse)
				{
					count--;

					if (count <= 0)
					{
						return true;
					}
				}
			}
		}
		break;
		case AbstractType::Building:
		{
			for (auto const pAir : *BuildingClass::Array)
			{
				if (pAir->Owner == pHouse)
				{
					count--;

					if (count <= 0)
					{
						return true;
					}
				}
			}
		}
		break;
		case AbstractType::Infantry:
		{
			for (auto const pAir : *InfantryClass::Array)
			{
				if (pAir->Owner == pHouse)
				{
					count--;

					if (count <= 0)
					{
						return true;
					}
				}
			}

		}
		break;
		default:
			break;
		}
		return false;
	}

	bool Execute(TEventClass* pThis, int iEvent, HouseClass* pHouse, ObjectClass* pObject,
		TimerStruct* pTimer, bool* isPersitant, TechnoClass* pSource, bool& bHandled)
	{
		bHandled = true;
		switch (static_cast<CnpTriggerEvent>(pThis->EventKind))
		{
		case CnpTriggerEvent::CheckTechno:
			return CheckTechno(pThis, pHouse);
		case CnpTriggerEvent::ObjectExist:
			return iEvent == (int)CnpTriggerEvent::ObjectExist && pObject;
		case CnpTriggerEvent::ObjectHealthTreshold:
		{
			if (iEvent == (int)CnpTriggerEvent::ObjectHealthTreshold)
				if (auto pTech = generic_cast<TechnoClass*>(pObject))
					return ((double)pThis->Value) >= (pTech->GetHealthPercentage() * 100.0);
		}
		default:
			return false;
		}

	}

}
