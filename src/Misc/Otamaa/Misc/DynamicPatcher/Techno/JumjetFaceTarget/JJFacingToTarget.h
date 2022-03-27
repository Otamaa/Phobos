#pragma once
#include <GeneralStructures.h>

class JJFacingToTarget
{
public:

	bool NeedToTurn;
	DirStruct ToDir;
	int Facing;

	JJFacingToTarget()
		: NeedToTurn { false }, ToDir { } , Facing{ 8 }
	{ }

	~JJFacingToTarget() = default;

	void TurnTo(DirStruct toDir, int facing)
	{
		NeedToTurn = true;
		ToDir = toDir;
		Facing = facing;
	}

	void Turning()
	{
		NeedToTurn = false;
	}

	void Cancel()
	{
		NeedToTurn = false;
	}

	bool Load(PhobosStreamReader& Stm, bool RegisterForChange)
	{ Debug::Log("Loading Elements From JJFacingToTarget ! \n"); return Serialize(Stm); }

	bool Save(PhobosStreamWriter& Stm)
	{ return Serialize(Stm); }

private:
	template <typename T>
	bool Serialize(T& Stm)
	{
		return Stm
			.Process(NeedToTurn)
			.Process(ToDir)
			.Process(Facing)
			.Success()
			;
	}
};