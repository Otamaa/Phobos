#pragma once

#include <Utilities/SavegameDef.h>

class AircraftDive
{
public:

	int ZOffset;
	int Delay;
	bool CanDive;

	int Speed;
	int DataDelay;

	AircraftDive() :
		Speed { 0 }
		, DataDelay { 0 }
		, ZOffset { 0 }
		, Delay { 0 }
		, CanDive { false }
	{ }

	AircraftDive(int nSpeed, int nDataDelay) :
		Speed { nSpeed }
		, DataDelay { nDataDelay }
		, ZOffset { 0 }
		, Delay { 0 }
		, CanDive { false }

	{ }

	~AircraftDive() = default;

	bool Load(PhobosStreamReader& Stm, bool RegisterForChange)
	{ Debug::Log("Loading Element From PassengersData ! \n"); return Serialize(Stm); }

	bool Save(PhobosStreamWriter& Stm)
	{ return Serialize(Stm); }


	int Diving()
	{
		if (--Delay < 0)
		{
			ZOffset += Speed;
			Delay = DataDelay;
		}
		return ZOffset;
	}

	void Reset()
	{
		ZOffset = 0;
		Delay = DataDelay;
		CanDive = true;
	}

private:
	template <typename T>
	bool Serialize(T& Stm)
	{
		return Stm
			.Process(Speed)
			.Process(DataDelay)
			.Process(ZOffset)
			.Process(Delay)
			.Process(CanDive)
			.Success()
			;
	}
};