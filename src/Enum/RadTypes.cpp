#include "RadTypes.h"

// for Debug::Log
#include <WarheadTypeClass.h>

void RadType::Read(CCINIClass* const pINI, const char* pSection, const char* pKey)
{
	INI_EX exINI(pINI);

	if(this->ID.Read(pINI, pSection, "RadType")){

		const char* section = this->ID;

		this->RadWarhead.Read(exINI, section, "RadSiteWarhead");
		this->RadSiteColor.Read(exINI, section, "RadColor");
		this->DurationMultiple.Read(exINI, section, "RadDurationMultiple");
		this->ApplicationDelay.Read(exINI, section, "RadApplicationDelay");
		this->BuildingApplicationDelay.Read(exINI, section, "RadApplicationDelayOnBuilding");
		this->LevelMax.Read(exINI, section, "RadLevelMax");
		this->LevelDelay.Read(exINI, section, "RadLevelDelay");
		this->LightDelay.Read(exINI, section, "RadLightDelay");
		this->LevelFactor.Read(exINI, section, "RadLevelFactor");
		this->LightFactor.Read(exINI, section, "RadLightFactor");
		this->TintFactor.Read(exINI, section, "RadTintFactor");

		RadType::DebugLog("Read");
	}
}

void RadType::Load(IStream* Stm) {
	PhobosStreamReader::Process(Stm, this->ID);
	if (strlen(ID)) {
		this->DurationMultiple.Load(Stm);
		this->ApplicationDelay.Load(Stm);
		this->LevelFactor.Load(Stm);
		this->LevelMax.Load(Stm);
		this->LevelDelay.Load(Stm);
		this->LightDelay.Load(Stm);
		this->BuildingApplicationDelay.Load(Stm);

		char warheadID[0x18];
		PhobosStreamReader::Process(Stm, warheadID);
		RadWarhead = WarheadTypeClass::Find(warheadID);

		this->RadSiteColor.Load(Stm);
		this->LightFactor.Load(Stm);
		this->TintFactor.Load(Stm);

		RadType::DebugLog("Load");
	}
}

void RadType::Save(IStream* Stm) {
	PhobosStreamWriter::Process(Stm, this->ID);
	if (strlen(ID)) {
		this->DurationMultiple.Save(Stm);
		this->ApplicationDelay.Save(Stm);
		this->LevelFactor.Save(Stm);
		this->LevelMax.Save(Stm);
		this->LevelDelay.Save(Stm);
		this->LightDelay.Save(Stm);
		this->BuildingApplicationDelay.Save(Stm);

		PhobosStreamWriter::Process(Stm, this->RadWarhead->ID);

		this->RadSiteColor.Save(Stm);
		this->LightFactor.Save(Stm);
		this->TintFactor.Save(Stm);

		RadType::DebugLog("Save");
	}
}

void RadType::DebugLog(const char* str) {
	Debug::Log("%s RadType\n", str);

	Debug::Log("\tID = %s\n", (char*)ID);

	Debug::Log("\tDuration_Multiple = %d\n", DurationMultiple);
	Debug::Log("\tApplicationDelay = %d\n", ApplicationDelay);

	Debug::Log("\tLevelFactor = %f\n", LevelFactor);

	Debug::Log("\tLevelMax = %d\n", LevelMax);
	Debug::Log("\tLevelDelay = %d\n", LevelDelay);
	Debug::Log("\tLightDelay = %d\n", LightDelay);
	Debug::Log("\tBuildingApplicationDelay = %d\n", BuildingApplicationDelay);

	Debug::Log("\tRadWarhead = %s\n", this->RadWarhead ? this->RadWarhead->ID : NONE_STR);
	Debug::Log("\tRadSiteColor = %d,%d,%d\n", this->RadSiteColor.Get().R, this->RadSiteColor.Get().G, this->RadSiteColor.Get().B);
	
	Debug::Log("\tLightFactor = %f\n", LightFactor);
	Debug::Log("\tTintFactor = %f\n", TintFactor);
}
