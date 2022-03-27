#include "DriveDataFunctional.h"
#include <Misc/Otamaa/Misc/DynamicPatcher/Helpers/Helpers.h>

void DriveDataFunctional::AI(TechnoExt::ExtData* pThis)
{
	if (!pThis)
		return;

	auto pTechno = pThis->OwnerObject();

	Mission const mission = pTechno->CurrentMission;
	auto& nDriveData = pThis->AnotherData.MyDriveData;

	switch (mission)
	{
	case Mission::Move:
	case Mission::AttackMove:
		if (Mission::Move != nDriveData.LastMission && Mission::AttackMove != nDriveData.LastMission)
		{
			nDriveData.nState = DrivingState::Start;
		}
		else
		{
			nDriveData.nState = DrivingState::Moving;
		}
		break;
	default:
		if (Mission::Move == nDriveData.LastMission || Mission::AttackMove == nDriveData.LastMission)
		{
			nDriveData.nState = DrivingState::Stop;
		}
		else
		{
			nDriveData.nState = DrivingState::Stand;
		}
		break;
	}

	nDriveData.LastMission = mission;
}