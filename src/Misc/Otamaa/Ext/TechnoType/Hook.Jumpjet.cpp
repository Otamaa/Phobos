#include <ExtraHeaders/Locomotion.Macros.h>

/*
DWORD TechnoExt::EvalBalloonHoverInf(TechnoClass* pThis, DWORD Yes, DWORD No, DWORD Dunno)
{
	if (auto Inf = specific_cast<InfantryClass*>(pThis))
	{
		if (Inf->Type->DeployToLand)
		{
			auto pData = TechnoExt::ExtMap.Find(Inf);
			return (pData->InfJumpjet_BalloonHovering)
				? Yes
				: No
				;
		}
		return (Inf->Type->BalloonHover)
			? Yes
			: No
			;
	}
	return Dunno;
}

DWORD TechnoExt::EvalBalloonHoverInf(LocomotionClass* Locomotor, DWORD Yes, DWORD No, DWORD Dunno)
 { return EvalBalloonHoverInf(Locomotor->LinkedTo, Yes, No, Dunno); }

bool TechnoExt::IsBalloonHovering(TechnoClass* pThis)
{
	if (auto Inf = specific_cast<InfantryClass*>(pThis))
	{
		if (Inf->Type->DeployToLand)
		{
			auto pData = TechnoExt::ExtMap.Find(Inf);
			return pData->InfJumpjet_BalloonHovering;
		}
		return Inf->Type->BalloonHover;
	}
	return pThis->GetTechnoType()->BalloonHover;
}

bool TechnoExt::IsBalloonHovering(DWORD Locomotor)
{
	Locomotor += 0xC;

	FootClass** Object = reinterpret_cast<FootClass**>(Locomotor); // pointer voodoo

	return TechnoExt::IsBalloonHovering(*Object);
}*/

/* removing several DeployToLand checks that were for units only */
DEFINE_HOOK(0x54BDA9, JumpjetLocomotionClass_ProcessState2_DTL_AllowInf, 0x7)
{
	GET(FootClass*, Foot, EDI);

	enum canHover { Yes = 0x54BDCD, No = 0x54BE62 };

	switch (Foot->WhatAmI())
	{
	case InfantryClass::AbsID:
		return (reinterpret_cast<InfantryClass*>(Foot)->Type->DeployToLand)
			? Yes
			: No
			;
	case UnitClass::AbsID:
		return (reinterpret_cast<UnitClass*>(Foot)->Type->IsSimpleDeployer)
			? Yes
			: No
			;
	default:
		return No;
	}
}

DEFINE_HOOK(0x54C1F6, JumpjetLocomotionClass_ProcessState3_DTL_AllowInf, 0x8)
{
	GET(UnitClass*, Unit, EDI);

	enum canHover { Yes = 0x54C212, No = 0x54C2DC };

	if (Unit)
	{
		return (Unit->Type->IsSimpleDeployer)
			? Yes
			: No
			;
	}

	GET_LOCO(ESI);
	UNREFERENCED_PARAMETER(pExt);
	UNREFERENCED_PARAMETER(LocoOwner);
	auto Object = Linked;

	if (auto Inf = specific_cast<InfantryClass*>(Object))
	{
		if (Inf->Type->DeployToLand)
		{
			auto pData = TechnoExt::ExtMap.Find(Inf);
			R->EDI<InfantryClass*>(Inf);
			return (pData->InfJumpjet_BalloonHovering)
				? Yes
				: No
				;
		}
	}
	return No;
}

DEFINE_HOOK(0x54CA27, JumpjetLocomotionClass_ProcessState4_DTL_AllowInf, 0x5)
{
	GET(FootClass*, Foot, ECX);
	R->EAX<FootClass*>(Foot);
	return 0x54CA5E;
}

/* BalloonHover checks */
DEFINE_HOOK(0x43C409, BuildingClass_ReceivedRadioCommand_BH_CanEnter, 0x6)
{
	GET(TechnoClass*, T, EDI);

	enum canHover
	{
		Yes = 0x43C413,
		No = 0x43C422,
		Dunno = 0
	};

	if (auto Inf = specific_cast<InfantryClass*>(T))
	{
		if (Inf->Type->DeployToLand)
		{
			auto pData = TechnoExt::ExtMap.Find(Inf);
			return (pData->InfJumpjet_BalloonHovering)
				? Yes
				: No
				;
		}
		return (Inf->Type->BalloonHover)
			? Yes
			: No
			;
	}
	return Dunno;
}

DEFINE_HOOK(0x4C7608, Networking_RespondToEvent_BH_Idle, 0x6)
{
	GET(TechnoClass*, pThis, ESI);
	return TechnoExt::EvalBalloonHoverInf(pThis, 0x4C7612, 0x4C762A, 0);
}

DEFINE_HOOK(0x51EDB4, InfantryClass_GetCursorOverObject_BH_CanEnter, 0x6)
{
	GET(InfantryClass*, pThis, EDI);
	return TechnoExt::EvalBalloonHoverInf(pThis, 0x51EDBE, 0x51EDC5, 0);
}

DEFINE_HOOK(0x522A72, InfantryClass_INotifySink_Notice, 0x6)
{
	GET(DWORD, Ptr, EDI);
	Ptr -= 8; //InfantryClass to TechnoClass

	auto Inf = reinterpret_cast<InfantryClass*>(Ptr);
	return TechnoExt::EvalBalloonHoverInf(Inf, 0x522A86, 0x522A7C, 0);
}

DEFINE_HOOK(0x53B25B, PsyDom_Fire_BH, 0x6)
{
	GET(TechnoClass*, pThis, ESI);
	return TechnoExt::EvalBalloonHoverInf(pThis, 0x53B364, 0x53B267, 0);
}

DEFINE_HOOK(0x53C49F, TechnoClass_CanBePermaMC_BH, 0x6)
{
	GET(TechnoClass*, pThis, ESI);
	return TechnoExt::EvalBalloonHoverInf(pThis, 0x53C4A9, 0x53C4AD, 0);
}

DEFINE_HOOK(0x54C6AD, JumpjetLocomotionClass_ProcessState4_SkipSomethingForUnit, 0x5)
{
	R->AL(1);
	return 0x54C6BA;
}

DEFINE_HOOK(0x54BCEE, JumpjetLocomotionClass_ProcessState1_BH, 0x6)
{
	GET_LOCO(ESI);
	UNREFERENCED_PARAMETER(pExt);
	UNREFERENCED_PARAMETER(LocoOwner);
	return TechnoExt::EvalBalloonHoverInf(pLocomotor, 0x54BD26, 0x54BCF8, 0);
}

DEFINE_HOOK(0x54BE6D, JumpjetLocomotionClass_ProcessState2_BH, 0x6)
{
	GET_LOCO(ESI);
	UNREFERENCED_PARAMETER(pExt);
	UNREFERENCED_PARAMETER(LocoOwner);
	return TechnoExt::EvalBalloonHoverInf(pLocomotor, 0x54BE77, 0x54BEA3, 0);
}

DEFINE_HOOK(0x54C0EB, JumpjetLocomotionClass_ProcessState3_BH, 0x6)
{
	GET_LOCO(ESI);
	UNREFERENCED_PARAMETER(pExt);
	UNREFERENCED_PARAMETER(LocoOwner);
	bool Hovering = TechnoExt::IsBalloonHovering(Linked);

	R->AL(Hovering);
	return 0x54C0F1;
}

DEFINE_HOOK(0x6EBE69, TeamClass_Coordinate_Move_BH_1, 0xA)
{
	GET(FootClass*, F, ESI);
	return TechnoExt::IsBalloonHovering(F)
		? 0x6EBE9C
		: 0x6EBE7D
		;
}

DEFINE_HOOK(0x6EBEDB, TeamClass_Coordinate_Move_BH_2, 0xA)
{
	GET(FootClass*, F, ESI);
	return TechnoExt::IsBalloonHovering(F)
		? 0x6EBEEF
		: 0x6EBEFF
		;
}

DEFINE_HOOK(0x6FC2AD, TechnoClass_GetFireError_BH, 0x5)
{
	GET(TechnoClass*, T, EBP);
	return TechnoExt::IsBalloonHovering(T)
		? 0x6FC2C2
		: 0x6FC2D2
		;
}

DEFINE_HOOK(0x70930D, FootClass_CanFightBack_BH, 0xA)
{
	GET(FootClass*, F, ESI);
	return TechnoExt::IsBalloonHovering(F)
		? 0x709321
		: 0x709366
		;
}

DEFINE_HOOK(0x70B671, TechnoClass_41C, 0x6)
{
	GET(TechnoClass*, T, ESI);
	return TechnoExt::IsBalloonHovering(T)
		? 0x70B683
		: 0x70BCA4
		;
}

DEFINE_HOOK(0x54D32E, JumpjetLocomotionClass_54D0F0, 0x5)
{
	GET(DWORD, Locomotor, ESI);
	return TechnoExt::IsBalloonHovering(Locomotor)
		? 0x54D343
		: 0x54D350
		;
}

/* deploy command */
DEFINE_HOOK(0x51E462, InfantryClass_GetCursorOverObject_SelfDeploy, 0x6)
{
	GET(InfantryClass*, Inf, EDI);
	GET(Action, Act, EBP);

	if (Act == Action::Self_Deploy)
	{
		if (Inf->Type->DeployToLand)
		{
			return 0x51E458; // always show the deploy cursor when hovering over self
		}
	}
	return 0;
}

DEFINE_HOOK(0x51F6EB, InfantryClass_Unload_Hovering, 0x6)
{
	GET(InfantryClass*, Inf, ESI);

	if (Inf->Type->DeployToLand)
	{
		Debug::Log("Deploying hover!\n");
		auto pData = TechnoExt::ExtMap.Find(Inf);
		pData->InfJumpjet_BalloonHovering = !pData->InfJumpjet_BalloonHovering;

		if (!pData->InfJumpjet_BalloonHovering)
			pData->InfJumpjet_PendingDeploy = true;
		else
			pData->InfJumpjet_PendingHover = true;

		Debug::Log("Hover state: BH = %d, PD = %d, PH = %d!\n", pData->InfJumpjet_BalloonHovering, pData->InfJumpjet_PendingDeploy, pData->InfJumpjet_PendingHover);
		Inf->ForceMission(Mission::Guard);
		Inf->SetDestination(Inf->GetCell(), true);
		R->EAX(1);
		return 0x51F7F5;
	}
	return 0;
}

DEFINE_HOOK(0x4DAA68, FootClass_Update_Hovering, 0x6)
{
	GET(FootClass*, pThis, ESI);
	if (auto Inf = specific_cast<InfantryClass*>(pThis))
	{
		if (Inf->Type->DeployToLand)
		{
			auto pData = TechnoExt::ExtMap.Find(Inf);
			if (!pData->InfJumpjet_BalloonHovering)
				return 0x4DAAEE;
		}
	}

	return 0;
}

DEFINE_HOOK(0x4DA87A, InfantryClass_Update_AfterLocomotion, 0x6)
{
	GET(FootClass*, pFoot, ESI);

	if (!pFoot->IsAlive)
		return 0x4DAF00;

	if (auto pThis = specific_cast<InfantryClass*>(pFoot))
	{
		if (pThis->Type->DeployToLand)
		{
			if (pThis->GetHeight() < 1)
			{
				auto pData = TechnoExt::ExtMap.Find(pThis);
				if (pData->InfJumpjet_PendingDeploy)
				{
					Debug::Log("Mutating Locomotor on %s\n", pThis->Type->ID);
					LocomotionClass::ChangeLocomotorTo(pThis, LocomotionClass::CLSIDs::Walk);
					pData->InfJumpjet_PendingDeploy = false;
				}
			}
		}
	}

	return 0x4DA886;
}
#include <WalkLocomotionClass.h>

DEFINE_HOOK(0x75AC87, WalkLocomotionClass_ILocomotion_Process_KickHoverUp, 0x7)
{
	GET(WalkLocomotionClass* const, Loco, ESI);
	auto const Linked = Loco->Owner;

	if (auto pInf = specific_cast<InfantryClass*>(Linked))
	{
		if (pInf->Type->JumpJet)
		{
			Debug::Log("Kickup %s\n", pInf->Type->ID);
			auto pData = TechnoExt::ExtMap.Find(pInf);

			if (pData->InfJumpjet_PendingHover)
			{
				if (auto nDest = pInf->Destination)
				{
					auto nDestCoords = nDest->GetCoords();

					if (nDestCoords.DistanceFrom(pInf->Location) > 100)
					{
						Debug::Log("Swap Locomotor %s\n", pInf->Type->ID);
						auto XYZ = pInf->Location;
						pInf->Limbo();
						Loco->AddRef();
						auto nLoco = YRComPtr<ILocomotion>(Loco);
						LocomotionClass::End_Piggyback(nLoco);

						if (!pInf->Unlimbo(XYZ, 0))
						{
							int Damage = pInf->Health * 8;
							pInf->ReceiveDamage(&Damage, 0, RulesClass::Instance->C4Warhead, nullptr, true, true, nullptr);
							Loco->Release();
						}

						pData->InfJumpjet_PendingHover = false;
					}

				}
			}
		}
	}

	return 0;
}

DEFINE_HOOK(0x54C14B, JumpjetLocomotionClass_UpdateMoving, 0x7)
{
	GET(UnitClass*, pUnit, EDI);

	if (pUnit->GetTechnoType()->Sensors)
		pUnit->UpdatePosition(2);

	return 0;
}

/*
TODO: make it obey the deploy command
	  :check networking

DEFINE_HOOK(730CB6, DeployCommandClass_Execute_ToggleHover, 6)
{
	GET(InfantryClass *, Inf, EDI);
	if(Inf && !Inf->Type->DeployToLand)
	{
		R->ECX<InfantryClass *>(Inf);
		return 0x730CBC;
	}
	return 0x730CE0;
}

DEFINE_HOOK(730B9C, DeployCommandClass_Execute_ToggleHoverToo, 7)
{
	GET(TechnoClass *, T, EDI);
	if(InfantryClass * I = specific_cast<InfantryClass *>(T))
	{
		if(I->Type->DeployToLand)
		{

		}
		return 0x730BA8;
	}
	return 0x730BDC;
}
*/

/*
DEFINE_HOOK(54CA3C, JumpjetLocomotionClass_ProcessState4_L1, 6)
{
	Debug::Log("#" str(__LINE__) "\n");
	return 0;
}

DEFINE_HOOK(54B1C3, JumpjetLocomotionClass_ILocomotion_MoveTo_L1, 6)
{
	Debug::Log("MoveTo\n");
	//	Debug::DumpStack(R, 0xF0);

	GET_STACK(DWORD, Locomotor, 0x14);
	Locomotor += 0x8;

	FootClass ** Object = reinterpret_cast<FootClass **>(Locomotor);
	if (FootClass * O = *Object)
	{
		if (AbstractClass * D = O->Destination)
		{
			CoordStruct XYZ;
			D->GetCoords(&XYZ);
			Debug::Log("Destination is object of type %d at (%d, %d, %d)\n", D->WhatAmI(), XYZ.X, XYZ.Y, XYZ.Z);
		}
	}

	return 0;
}

DEFINE_HOOK(54B424, JumpjetLocomotionClass_ILocomotion_MoveTo_L2, 7)
{
	GET(DWORD, Locomotion, ESI);
	GET(FootClass *, F, ECX);
	Debug::Log("Object %s in state %d\n", F->GetType()->ID, *(int*)(Locomotion + 0x4C));
	return 0;
}

DEFINE_HOOK(54B45D, JumpjetLocomotionClass_ILocomotion_MoveTo_L3, 7)
{
	Debug::Log("Object going to state 1\n");
	return 0;
}*/
/*

DEFINE_HOOK(54D600, JumpjetLocomotionClass_Sub54D0F0_CrashFix, 6)
{
	GET(FootClass*, Foot, ECX);
	GET_STACK(CoordStruct*, pCoord, 0x44);

	if (Foot->WhatAmI() != AbstractType::Aircraft
		|| Foot->Location.X == pCoord->X && Foot->Location.Y == pCoord->Y)
	{
		Foot->SetLocation(*pCoord);
	}
	else
	{
		auto onMap = Foot->IsOnMap;
		Foot->IsOnMap = true;
		Foot->SetLocation(*pCoord);
		Foot->IsOnMap = onMap;

	}
	return 0x54D60D;
}


DEFINE_HOOK(54C78F, JumpjetLocomotionClass_State4_DeployDir, 7)
{

	GET_LOCO(ESI);
	auto dir = pExt->JumpJetDeployDir.Get(RulesClass::Instance->DeployDir);

	for (int i = 3; i >= 0; ++i)
	{
		byte b = (dir >> 8 * i) & 0xFF;
		R->DH(b);
	}

	return 0x54C796;
}

DEFINE_HOOK(54C767, JumpjetLocomotionClass_State4_DeployDir, 9)
{
	GET_SPESIFICFROMESI(FacingStruct*, pFacing, 0x54);
	GET_LOCO(ESI);
	auto dir = (short)pExt->JumpJetDeployDir.Get(RulesClass::Instance->DeployDir);
	auto Cur = pFacing->current().value();

	if (Cur != dir)
	{
		pFacing->set(static_cast<DirStruct>(dir));
	}

	return 0x54C7A3; //
}
DEFINE_HOOK(54C774, JumpjetLocomotionClass_State4_DeployDir_B,5)
{
	GET_LOCO(ESI);
	auto dir = pExt->JumpJetDeployDir.Get(RulesClass::Instance->DeployDir);
	R->EBX(dir);
	return 0x54C779;
}

DEFINE_HOOK(54D820 ,JumpJetLocomotionClass_UpdateHeightAboveObject, 6)
DEFINE_HOOK(54DCD2 , JumpetLocomotionClass_DrawMatrix, 8) //when it change loco and infected  with parasite
*/

#undef GET_LOCO