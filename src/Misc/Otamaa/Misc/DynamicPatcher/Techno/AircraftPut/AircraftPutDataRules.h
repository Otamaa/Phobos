#pragma once
#include <Utilities/TemplateDef.h>

class AircraftPutDataRules
{
public :

	Valueable<Point3D> PosOffset;
	Valueable<bool> ForceOffset;
	Valueable<bool> RemoveIfNoDocks;

	AircraftPutDataRules():
		PosOffset { {0,0,0} }
		, RemoveIfNoDocks { false  }
		, ForceOffset { false }
	{ }

	~AircraftPutDataRules() = default;

	void Read(INI_EX & parser, const char* pSection)
	{
		PosOffset.Read(parser, pSection, "AircraftNoHelipadPutOffset");
		ForceOffset.Read(parser, pSection, "AircraftForcePutOffset");
		RemoveIfNoDocks.Read(parser, pSection, "RemoveIfNoDocks");
	}

	bool Load(PhobosStreamReader & Stm, bool RegisterForChange)
	{ Debug::Log("Loading Element From AircraftPutDataRules ! \n"); return Serialize(Stm); }

	bool Save(PhobosStreamWriter & Stm)
	{ return Serialize(Stm); }

private:
	template <typename T>
	bool Serialize(T & Stm)
	{
		return Stm
			.Process(PosOffset)
			.Process(RemoveIfNoDocks)
			.Process(ForceOffset)
			.Success()
			;
	}
};