#include "Body.h"

/* There is check for stuffs here that need to be replicated or else it will cause chaos
DEFINE_HOOK(0x41E88D, AITriggerTypeClass_ConditionMet_SideIndex, 0xA)
{
	GET(AITriggerTypeClass*, pThis, ESI);
	GET(HouseClass *, House, EDI);
	//GET_STACK(int, nVal3, 0x24);
	//GET(void* , pSomePtr , EDX);

	Debug::Log(__FUNCTION__" for [%s] house [%s] trg [%d]\n", pThis->ID, House->Type->ID, pThis->SideIndex);

	if(static_cast<int>(pThis->OwnerHouseType) == 3 && House->Type->ArrayIndex2 == -1 )
		return 0x41E8A1;

	if (pThis->SideIndex)
	{
		--pThis->SideIndex;
		if(pThis->SideIndex != House->SideIndex)
			return 0x41E8A1;
	}

	if(pThis->TechLevel > House->TechLevel)
		return 0x41E8A1;

	enum Eligible { Yes = 0x41E8D7, No = 0x41E8A1 };
	if (!pThis->SideIndex)
		return Yes;

	--pThis->SideIndex;
	return(pThis->SideIndex == House->SideIndex && pThis->TechLevel < House->TechLevel) ? Yes : No;


	return  0x41E8D7;
}

DEFINE_HOOK(0x41F3C6, AITriggerTypeClass_LoadFromINIList_Enable, 0x8)
{
	GET(CCINIClass*, pINI, EBX);
	GET(bool, nBool, EDI);

	if (!nBool)
	{
		if (AITriggerTypeClass::Array.get()->Count > 0)
		{
			for (auto Item : *AITriggerTypeClass::Array)
			{
				Item->IsEnabled = pINI->ReadBool("AITriggerTypesEnable", Item->ID, Item->IsEnabled) || SessionClass::Instance->GameMode == GameMode::Campaign;

				//Debug::Log(__FUNCTION__" for [%s] Enabled [%s] \n", Item->ID, Item->IsEnabled ? "Yes" : "No");
			}
		}
	}

	return 0x41F480;
}

DEFINE_HOOK(0x41F73E, AITriggerTypeClass_LoadFromINI_Conditions, 0x6)
{
	GET(AITriggerTypeClass*, pThis, EBP);
	GET(const char*, commacharpos, EAX);
	//GET_STACK(CCINIClass*, pINI, 0x284);

	auto condType = CRT::atoi(commacharpos);
	if (auto AITTypeExt = AITriggerTypeExt::ExtMap.Find(pThis))
	{
		//AITTypeExt->CondCount = pINI->ReadInteger(pThis->ID, "ConditionCount", 0);
		//AITTypeExt->CoopType = pINI->ReadBool(pThis->ID, "CooperativeType", false);

		//Debug::Log(__FUNCTION__" For[%s][%s] curcondType[%d] \n", pThis->ID, pThis->Name, condType);
	}

	pThis->ConditionType = static_cast<AITriggerCondition>(condType);

	auto result = CRT::strtok(0, ",");
	R->EAX(result);

	return result ? 0x41F760 : 0x41FAF2;
}*/

/*
DEFINE_HOOK(0x41F6EE, AITriggerTypeClass_LoadFromINI_House, 0x9)
{
	GET(AITriggerTypeClass*, pThis, EBP);
	GET_STACK(CCINIClass*, pINI, 0x284);
	GET_STACK(const char*, pSection, 0x1C);
	//GET_STACK(const char*, pSection, 0x29);

	
	if (pINI->ReadString(pSection, "NoneOf", "", Phobos::readBuffer))
	{

		for (auto i = strtok_s(Phobos::readBuffer, ",", nullptr); i; i = strtok_s(0, ",", nullptr))
		{
			int Idx = 0;
			for (auto Array : *HouseClass::Array)
			{
				Idx = Array->FindIndexByName(i);
			}

			if (Idx == -1)
			{
				Debug::FatalErrorAndExit(__FUNCTION__"House Invalid ! [%s]%s=%s\n", pSection,"NoneOf",i);
			}


			pThis->OwnerHouseType = 2;

			return 0x41F70C;
		}
	}
	else
	

	Debug::Log_Masselist(__FUNCTION__" for[%s] pSection[%s]",pThis->ID,pSection);
	{
		auto Idx = HouseTypeClass::FindIndexOfName(pSection);
		pThis->HouseIndex = Idx;

		if (Idx != -1)
		{
			pThis->OwnerHouseType = AITriggerHouseType::Single;
		}
	}

	return 0x41F70C;
}*/