#pragma once

#include <Utilities/TemplateDef.h>

class SpawnSupport
{
public:

	int supportFLHMult;
	TimerStruct supportFireROF;
	TimerStruct spawnFireOnceDelay;
	bool spawnFireFlag;

	SpawnSupport()
		: supportFLHMult { 1 }
		, supportFireROF { }
		, spawnFireOnceDelay { }
		, spawnFireFlag { false }
	{ }

	bool Load(PhobosStreamReader& Stm, bool RegisterForChange)
	{ Debug::Log("Loading Element From SpawnSupport ! \n");  return Serialize(Stm); }

	bool Save(PhobosStreamWriter& Stm)
	{ return Serialize(Stm); }

private:
	template <typename T>
	bool Serialize(T& Stm)
	{
		return Stm
			.Process(supportFLHMult)
			.Process(supportFireROF)
			.Process(spawnFireOnceDelay)
			.Process(spawnFireFlag)
			.Success()
			;
	}
};
