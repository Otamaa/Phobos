#pragma once
#include <Utilities/TemplateDef.h>

enum DrivingState : int
{
	Moving = 0, Stand = 1, Start = 2, Stop = 3
};

class DriveData
{
public:

	DrivingState nState;
	Mission LastMission;

	DriveData() :
		nState { DrivingState::Stop }
		, LastMission { Mission::None }
	{}

	bool Load(PhobosStreamReader& Stm, bool RegisterForChange)
	{ Debug::Log("Loading Element From DriveData ! \n");  return Serialize(Stm); }

	bool Save(PhobosStreamWriter& Stm)
	{ return Serialize(Stm); }

private:
	template <typename T>
	bool Serialize(T& Stm)
	{
		return Stm
			.Process(nState)
			.Process(LastMission)
			.Success()
			;
	}
};