#include "AnimTrailTypeClass.h"
#include <Utilities/TemplateDef.h>

Enumerable<AnimTrailTypeClass>::container_t Enumerable<AnimTrailTypeClass>::Array;

const char* Enumerable<AnimTrailTypeClass>::GetMainSection()
{
	return "AnimTrailTypes";
}

void AnimTrailTypeClass::LoadFromINI(CCINIClass* pINI)
{
	const char* section = this->Name;

	if (CCINIClass::IsBlank(section) || !pINI->GetSection(section) )
		return;

	INI_EX exINI(pINI);

	this->Anim_While.Read(exINI, section, "Anim.While",true);
	this->Anim_Start.Read(exINI, section, "Anim.Start", true);
	this->Anim_Stop.Read(exINI, section, "Anim.Stop", true);
	this->IgnoreVertical.Read(exINI, section, "IgnoreVertical");
	this->Distance.Read(exINI, section, "Distance");
	this->InitialDelay.Read(exINI, section, "InitialDelay");
}

template <typename T>
void AnimTrailTypeClass::Serialize(T& Stm)
{
	Stm
		.Process(this->Anim_While)
		.Process(this->Anim_Start)
		.Process(this->Anim_Stop)
		.Process(this->Distance)
		.Process(this->InitialDelay)
		.Process(this->IgnoreVertical)
		;
}

void AnimTrailTypeClass::LoadFromStream(PhobosStreamReader& Stm)
{
	this->Serialize(Stm);
}

void AnimTrailTypeClass::SaveToStream(PhobosStreamWriter& Stm)
{
	this->Serialize(Stm);
}