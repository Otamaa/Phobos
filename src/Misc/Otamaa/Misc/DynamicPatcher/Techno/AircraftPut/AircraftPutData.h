#pragma once

#include <Utilities/TemplateDef.h>

class AircraftPutData
{
public:

	Nullable<Point3D> PosOffset;
	Nullable<bool> ForceOffset;
	Nullable<bool> RemoveIfNoDocks;

	AircraftPutData() :
		PosOffset { }
		, RemoveIfNoDocks { }
		, ForceOffset { }
	{ }

	~AircraftPutData() = default;

	void Read(INI_EX& parser, const char* pSection)
	{
		PosOffset.Read(parser, pSection, "NoHelipadPutOffset");
		ForceOffset.Read(parser, pSection, "ForcePutOffset");
		RemoveIfNoDocks.Read(parser, pSection, "RemoveIfNoDocks");
	}

	bool Load(PhobosStreamReader& Stm, bool RegisterForChange)
	{ Debug::Log("Loading Element From AircraftPutData ! \n"); return Serialize(Stm); }

	bool Save(PhobosStreamWriter& Stm)
	{ return Serialize(Stm); }

private:
	template <typename T>
	bool Serialize(T& Stm)
	{
		return Stm
			.Process(PosOffset)
			.Process(RemoveIfNoDocks)
			.Process(ForceOffset)
			.Success()
			;
	}
};