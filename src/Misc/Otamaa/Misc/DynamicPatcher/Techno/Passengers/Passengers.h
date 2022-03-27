#pragma once

#include <Utilities/TemplateDef.h>

class PassengersData
{
public:

	Valueable<bool> PassiveAcquire;
	Valueable<bool> ForceFire;
	Valueable<bool> MobileFire;
	Valueable<bool> SameFire;

	PassengersData()
		: PassiveAcquire { true }
		, ForceFire { false }
		, MobileFire { true }
		, SameFire { true }
	{ }

	~PassengersData() = default;

	bool Load(PhobosStreamReader& Stm, bool RegisterForChange)
	{ Debug::Log("Loading Element From PassengersData ! \n"); return Serialize(Stm); }

	bool Save(PhobosStreamWriter& Stm)
	{ return Serialize(Stm); }


	void Read(INI_EX& parser, const char* pSection, bool Allocate = false)
	{
		PassiveAcquire.Read(parser, pSection, "Passengers.PassiveAcquire");
		ForceFire.Read(parser, pSection, "Passengers.ForceFire");
		MobileFire.Read(parser, pSection, "Passengers.MobileFire");
		SameFire.Read(parser, pSection, "Passengers.SameFire");
	}

private:
	template <typename T>
	bool Serialize(T& Stm)
	{
		return Stm
			.Process(PassiveAcquire)
			.Process(ForceFire)
			.Process(MobileFire)
			.Process(SameFire)
			.Success()
			;
	}
};