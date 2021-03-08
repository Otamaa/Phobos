#pragma once

#include "../Utilities/TemplateDef.h"

#include <RulesClass.h>
#include <CCINIClass.h>

class RadType
{
public:
	void Read(CCINIClass* const pINI, const char* section, const char* pKey);
	void RadType::Load(IStream* Stm);
	void RadType::Save(IStream* Stm);
	void RadType::DebugLog(const char* str);

	PhobosFixedString<0x20> ID;
	Valueable<int> DurationMultiple;
	Valueable<int> ApplicationDelay;
	Valueable<double> LevelFactor;
	Valueable<int> LevelMax;
	Valueable<int> LevelDelay;
	Valueable<int> LightDelay;
	Valueable<int> BuildingApplicationDelay;
	Valueable<WarheadTypeClass*> RadWarhead;
	Valueable<ColorStruct> RadSiteColor;
	Valueable<double> LightFactor;
	Valueable<double> TintFactor;

	RadType() :
		DurationMultiple(0),
		ApplicationDelay(0),
		LevelFactor(0),
		LevelMax(0),
		LevelDelay(0),
		LightDelay(0),
		BuildingApplicationDelay(false),
		RadWarhead(nullptr),
		RadSiteColor(),
		LightFactor(0),
		TintFactor(0)
	{
		ID = "";
	}
};
