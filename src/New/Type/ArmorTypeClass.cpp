#include "ArmorTypeClass.h"
#include <Utilities/TemplateDef.h>

Enumerable<ArmorTypeClass>::container_t Enumerable<ArmorTypeClass>::Array;

const char* Enumerable<ArmorTypeClass>::GetMainSection()
{
	return "ArmorTypes";
}

void ArmorTypeClass::AddDefaults()
{
	FindOrAllocate("none");
	FindOrAllocate("flak");
	FindOrAllocate("plate");
	FindOrAllocate("light");
	FindOrAllocate("medium");
	FindOrAllocate("heavy");
	FindOrAllocate("wood");
	FindOrAllocate("steel");
	FindOrAllocate("concrete");
	FindOrAllocate("special_1");
	FindOrAllocate("special_2");
}

void ArmorTypeClass::LoadFromINI(CCINIClass* pINI)
{
	const char* pName = this->Name.data();

	//Dont need to load these for default !
	if (
		_strcmpi(pName, "none") ||
		_strcmpi(pName, "flak") ||
		_strcmpi(pName, "plate") ||
		_strcmpi(pName, "light") ||
		_strcmpi(pName, "medium") ||
		_strcmpi(pName, "heavy") ||
		_strcmpi(pName, "wood") ||
		_strcmpi(pName, "steel") ||
		_strcmpi(pName, "concrete") ||
		_strcmpi(pName, "special_1") ||
		_strcmpi(pName, "special_2")
		)
	{
		INI_EX exINI(pINI);

		char buffer[0x40];
		pINI->ReadString(Enumerable<ArmorTypeClass>::GetMainSection(), pName, "", buffer);

		if (buffer[0] != '\0' && std::strlen(buffer))
			this->DefaultTo = ArmorTypeClass::FindIndex(buffer);
	}

	//Debug::Log("Trying to parse Versesdata for [%s] DefaultIndex [%d]\n", pName, this->DefaultTo);
}

void ArmorTypeClass::LoadFromINIList_(CCINIClass* pINI, bool bDebug)
{
	ArmorTypeClass::AddDefaults();

	const char* pSection = GetMainSection();
	for (int i = 0; i < pINI->GetKeyCount(pSection); ++i)
	{
		const char* pKey = pINI->GetKeyName(pSection, i);
		if (auto pAlloc = FindOrAllocate(pKey))
		{
			pAlloc->LoadFromINI(pINI);
		}

		if (bDebug)
			Debug::Log("Allocating ArmorType with Name[%s] at [%d] \n", pKey, i);
	}

	if (bDebug)
		Debug::Log("ArmorType Array count currently [%d]\n", ArmorTypeClass::Array.size());
}

template <typename T>
void ArmorTypeClass::Serialize(T& Stm)
{
	Stm
		.Process(this->DefaultTo)
		;
}

void ArmorTypeClass::LoadFromStream(PhobosStreamReader& Stm)
{
	this->Serialize(Stm);
}

void ArmorTypeClass::SaveToStream(PhobosStreamWriter& Stm)
{
	this->Serialize(Stm);
}
