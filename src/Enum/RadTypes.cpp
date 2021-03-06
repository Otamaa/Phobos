#include "RadTypes.h"

#include "../Utilities/TemplateDef.h"

#include <WarheadTypeClass.h>
#include <string.h>

Enumerable<RadType>::container_t Enumerable<RadType>::Array;

const char * Enumerable<RadType>::GetMainSection()
{
	return "RadiationTypes";
}

RadType::RadType(const char* const pTitle)
	: Enumerable<RadType>(pTitle),
	Level_Delay(),
	Light_Delay(),
	RadSiteColor(),
	Level_Max(),
	Level_Factor(),
	Light_Factor(),
	Tint_Factor(),
	RadWarhead(),
	Duration_Multiple(),
	Application_Delay(),
	BuildingApplicationDelay()
{ }

RadType::~RadType() = default;

void RadType::AddDefaults()
{
	FindOrAllocate("Radiation");
}

void RadType::LoadListSection(CCINIClass *pINI)
{
	const char *section = GetMainSection();
	int len = pINI->GetKeyCount(section);
	for (int i = 0; i < len; ++i) {
		const char *key = pINI->GetKeyName(section, i);
		if (pINI->ReadString(section, key, "", Phobos::readBuffer)) {
			FindOrAllocate(Phobos::readBuffer);
		}
	}
	for (size_t i = 0; i < Array.size(); ++i) {
		Array[i]->LoadFromINI(pINI);
	}
}

void RadType::LoadFromINI(CCINIClass *pINI)
{
	const char *section = this->Name;

	INI_EX exINI(pINI);

	this->RadWarhead.Read(exINI, section, "RadSiteWarhead");
	this->RadSiteColor.Read(exINI, section, "RadColor");
	this->Duration_Multiple.Read(exINI, section, "RadDurationMultiple");
	this->Application_Delay.Read(exINI, section, "RadApplicationDelay");
	this->BuildingApplicationDelay.Read(exINI, section, "RadApplicationDelayOnBuilding");
	this->Level_Max.Read(exINI, section, "RadLevelMax");
	this->Level_Delay.Read(exINI, section, "RadLevelDelay");
	this->Light_Delay.Read(exINI, section, "RadLightDelay");
	this->Level_Factor.Read(exINI, section, "RadLevelFactor");
	this->Light_Factor.Read(exINI, section, "RadLightFactor");
	this->Tint_Factor.Read(exINI, section, "RadTintFactor");

	
	Debug::Log("Reading RadType sections , section Name = %s\n", this->Name);
	Debug::Log("RadType Index = %d\n", RadType::FindIndex(this->Name));
	Debug::Log("RadWarhead = %s\n", this->RadWarhead->ID);
	Debug::Log("RadSiteColor = %d,%d,%d\n", this->RadSiteColor.Get().R, this->RadSiteColor.Get().G, this->RadSiteColor.Get().B);
	Debug::Log("Duration_Multiple = %d\n", this->Duration_Multiple);
	Debug::Log("Level_Max = %d\n", this->Level_Max);
	Debug::Log("Level_Delay = %d\n", this->Level_Delay);
	Debug::Log("Light_Delay = %d\n", this->Light_Delay);


}