#pragma once

#include <Utilities/TemplateDef.h>

class AircraftDiveData
{
public:

	Valueable<bool> Enable;
	Valueable<int> Distance;
	Valueable<int> Speed;
	Valueable<int> Delay;
	Valueable<int> FlightLevel;
	Valueable<bool> PullUpAfterFire;

	AircraftDiveData() :
		Enable { false }
		, Distance { 0 }
		, Speed { 1 }
		, Delay { 0 }
		, FlightLevel { 300 }
		, PullUpAfterFire { false }
	{ }

	~AircraftDiveData() = default;

	bool Load(PhobosStreamReader & Stm, bool RegisterForChange)
	{ Debug::Log("Loading Element From AircraftDiveData ! \n"); return Serialize(Stm); }

	bool Save(PhobosStreamWriter & Stm)
	{ return Serialize(Stm); }


	void Read(INI_EX & parser, const char* pSection, bool Allocate = false)
	{
		Enable.Read(parser, pSection, "Dive");
		Distance.Read(parser, pSection, "Dive.Distance");
		Speed.Read(parser, pSection, "Dive.Speed");
		Delay.Read(parser, pSection, "Dive.Delay");
		FlightLevel.Read(parser, pSection, "Dive.FlightLevel");
		PullUpAfterFire.Read(parser, pSection, "Dive.PullUpAfterFire");
	}

private:
	template <typename T>
	bool Serialize(T & Stm)
	{
		return Stm
			.Process(Enable)
			.Process(Distance)
			.Process(Speed)
			.Process(Delay)
			.Process(FlightLevel)
			.Process(PullUpAfterFire)
			.Success()
			;
	}
};