#include "TheaterTypeClass.h"

#include <Phobos.h>

DynamicVectorClass<TheaterTypeClass*> TheaterTypesArray;

// Otamaa 22/12/2021 0:12AM
// Most of the code here is Copied from Vinivera @CCHyper custom theater codes ! 
// Adapted to YRPP and my style of code mostly
// Using call pointer  replace is way faster for me , since i dont know how to completely replace the array pointing code around those areas
// Todo : Save/Load , RMG , more hooks ? , sorting ares stuffs !

namespace TypeLog
{
	bool bEnableLog = false;
}

TheaterTypeClass::TheaterTypeClass(const char* pName)
	:
	Identifier("<none>")
	, UIName("Name:<none>")
	, ControlFileName("X")
	, ArtFileName("X")
	, PaletteFileName("X")
	, Extension("X")
	, MMExtension("X")
	, Letter("X")
	, IsArctic(false)
	, IsAllowedInMapGenerator(false)
	, LowRadarBrightness1(1.0f)
	, LowRadarBrightness2(1.0f)
	, HighRadarBrightness1(1.0f)
	, HighRadarBrightness2(1.0f)
	, unknown_int_68(0)
	, unknown_int_6C(0)
{
	if (pName[0] != '\0' && std::strlen(pName))
	{
		std::strncpy(Identifier, pName, sizeof(Identifier));
		Identifier[sizeof(Identifier) - 1] = '\0';
	}

	TheaterTypesArray.AddItem(this);
}

TheaterTypeClass::TheaterTypeClass
(
	const char* pName,
	const char* pUIName,
	const char* pControlFileName,
	const char* pArtFileName,
	const char* pPaletteFileName,
	const char* pExtension,
	const char* pMMExtension,
	const char* nLetter,
	bool bIsArtic,
	bool bMapgenAllow,
	float low_brightness_1,
	float low_brightness_2,
	float high_brightness_1,
	float high_brightness_2,
	int nunknown_int_68,
	int  nunknown_int_6C
) :

	Identifier("<none>")
	, UIName("Name:<none>")
	, ControlFileName("X")
	, ArtFileName("X")
	, PaletteFileName("X")
	, Extension("X")
	, MMExtension("X")
	, Letter("X")
	, IsArctic(bIsArtic)
	, IsAllowedInMapGenerator(bMapgenAllow)
	, LowRadarBrightness1(low_brightness_1)
	, LowRadarBrightness2(low_brightness_2)
	, HighRadarBrightness1(high_brightness_1)
	, HighRadarBrightness2(high_brightness_2)
	, unknown_int_68(nunknown_int_68)
	, unknown_int_6C(nunknown_int_6C)
{
	if (pName[0] != '\0' && std::strlen(pName))
	{
		std::strncpy(Identifier, pName, sizeof(Identifier));
		Identifier[sizeof(Identifier) - 1] = '\0';
	}

	if (pUIName[0] != '\0' && std::strlen(pUIName))
	{
		std::strncpy(UIName, pUIName, sizeof(UIName));
		UIName[sizeof(UIName) - 1] = '\0';
	}

	if (pControlFileName[0] != '\0' && std::strlen(pControlFileName))
	{
		std::strncpy(ControlFileName, pControlFileName, sizeof(ControlFileName));
		ControlFileName[sizeof(ControlFileName) - 1] = '\0';
	}

	if (pArtFileName[0] != '\0' && std::strlen(pArtFileName))
	{
		std::strncpy(ArtFileName, pArtFileName, sizeof(ArtFileName));
		ArtFileName[sizeof(ArtFileName) - 1] = '\0';
	}

	if (pPaletteFileName[0] != '\0' && std::strlen(pPaletteFileName))
	{
		std::strncpy(PaletteFileName, pPaletteFileName, sizeof(PaletteFileName));
		PaletteFileName[sizeof(PaletteFileName) - 1] = '\0';
	}

	if (pExtension[0] != '\0' && std::strlen(pExtension))
	{
		std::strncpy(Extension, pExtension, sizeof(Extension));
		Extension[sizeof(Extension) - 1] = '\0';
	}

	if (pMMExtension[0] != '\0' && std::strlen(pMMExtension))
	{
		std::strncpy(MMExtension, pMMExtension, sizeof(MMExtension));
		MMExtension[sizeof(MMExtension) - 1] = '\0';
	}

	if (Letter[0] != '\0' && std::strlen(Letter))
	{
		std::strncpy(Letter, nLetter, sizeof(Letter));
		Letter[sizeof(Letter) - 1] = '\0';
	}

	TheaterTypesArray.AddItem(this);
}

TheaterTypeClass::~TheaterTypeClass() { TheaterTypesArray.Remove(this); };

const TheaterTypeClass& TheaterTypeClass::As_Reference(TheaterType nType)
{
	static const TheaterTypeClass _x;
	return nType == TheaterType::None || (int)nType > TheaterTypesArray.Count ? _x : *TheaterTypesArray[(int)nType];
}

const TheaterTypeClass* TheaterTypeClass::As_Pointer(TheaterType nType)
{
	return nType != TheaterType::None && (size_t)nType < TheaterTypesArray.Count ? TheaterTypesArray[(int)nType] : nullptr;
}

const TheaterTypeClass& TheaterTypeClass::As_Reference(const char* pName)
{
	return As_Reference(From_Name(pName));
}

const TheaterTypeClass* TheaterTypeClass::As_Pointer(const char* pName)
{
	return As_Pointer(From_Name(pName));
}

const char* TheaterTypeClass::GetIdentifier(TheaterType type)
{
	return (type != TheaterType::None && (size_t)type < TheaterTypesArray.Count ? As_Reference(type).Identifier : "<none>");
}

const char* TheaterTypeClass::GetUIName(TheaterType type)
{
	return (type != TheaterType::None && (size_t)type < TheaterTypesArray.Count ? As_Reference(type).UIName : "Name:<none>");
}

const char* TheaterTypeClass::GetControlFileName(TheaterType type)
{
	return As_Reference(type).ControlFileName;
}

const char* TheaterTypeClass::GetArtFileName(TheaterType type) { return As_Reference(type).ArtFileName; }
const char* TheaterTypeClass::GetPaletteFileName(TheaterType type) { return As_Reference(type).PaletteFileName; }
const char* TheaterTypeClass::GetExtension(TheaterType type) { return As_Reference(type).Extension; }
char* TheaterTypeClass::GetCharExtension(TheaterType type) { return _strdup(As_Reference(type).Extension); }
const char* TheaterTypeClass::GetMMExtension(TheaterType type) { return As_Reference(type).MMExtension; }
const char* TheaterTypeClass::GetLetter(TheaterType type) { return As_Reference(type).Letter; }

bool TheaterTypeClass::GetIsArtic(TheaterType type) { return As_Reference(type).IsArctic; }
bool TheaterTypeClass::GetAllowMapGen(TheaterType type) { return As_Reference(type).IsAllowedInMapGenerator; }

float TheaterTypeClass::GetLowRadarBrightness(TheaterType type, bool bSecond)
{
	 return (!bSecond ? As_Reference(type).LowRadarBrightness1 : As_Reference(type).LowRadarBrightness2); 
 }

float TheaterTypeClass::GetHighRadarBrightness(TheaterType type, bool bSecond)
{ 
	return (!bSecond ? As_Reference(type).HighRadarBrightness1 : As_Reference(type).HighRadarBrightness2);
}

int TheaterTypeClass::GetUnknownVal(TheaterType type, bool bSecond)
{
	return (!bSecond ? As_Reference(type).unknown_int_68 : As_Reference(type).unknown_int_6C);
}

TheaterType TheaterTypeClass::From_Name(const char* pName)
{
	if (!_stricmp(pName, "<none>") || !_stricmp(pName, "none"))
		return TheaterType::None;

	if (pName != nullptr)
	{
		for (auto index = (size_t)TheaterType::Temperate; index < TheaterTypesArray.Count; ++index)
		{
			if (!_stricmp(As_Reference(TheaterType(index)).Identifier, pName))
			{
				return (TheaterType)index;
			}
		}
	}

	return TheaterType::None;
}

void TheaterTypeClass::CleanUp()
{
	for (auto const& pTheater : TheaterTypesArray)
	{
		if (pTheater)
		{
			GameDelete(pTheater);
			TheaterTypesArray.Remove(pTheater);
		}
	}

	TheaterTypesArray.Clear();
}

int TheaterTypeClass::FindIndex(const char* pName)
{
	if (TheaterTypesArray.Count <= 0)
		TheaterTypeClass::LoadConfiguration();

	for (int i = 0; i < TheaterTypesArray.Count; i++)
	{
		if (_strcmpi(TheaterTypesArray[i]->Identifier, pName) == 0)
		{
			return i;
		}
	}

	return -1;
}

TheaterTypeClass* TheaterTypeClass::FindOrAllocate(const char* pName)
{
	if (!_stricmp(pName, "<none>") || !_stricmp(pName, "none"))
		return nullptr;

	for (int index = (int)TheaterType::Temperate; index < TheaterTypesArray.Count; ++index)
	{
		if (!_stricmp(TheaterTypesArray[index]->Identifier, pName))
		{ return TheaterTypesArray[index]; }
	}

	return GameCreate<TheaterTypeClass>(pName);
}

void TheaterTypeClass::LoadFromINI(CCINIClass* pINI)
{
	const char* pSection = this->Identifier;

	if (!pINI->GetSection(pSection))
		return;

	pINI->ReadString(pSection, "UIName", UIName, UIName, sizeof(UIName));
	pINI->ReadString(pSection, "ControlFileName", ControlFileName, ControlFileName, sizeof(ControlFileName));
	pINI->ReadString(pSection, "ArtFileName", ArtFileName, ArtFileName, sizeof(ArtFileName));
	pINI->ReadString(pSection, "PaletteFileName", PaletteFileName, PaletteFileName, sizeof(PaletteFileName));
	pINI->ReadString(pSection, "Extension", Extension, Extension, sizeof(Extension));
	pINI->ReadString(pSection, "MMExtension", MMExtension, MMExtension, sizeof(MMExtension));
	pINI->ReadString(pSection, "ImageLetter", Letter, Letter, sizeof(Letter));

	IsArctic = pINI->ReadBool(pSection, "IsArctic", IsArctic);
	IsAllowedInMapGenerator = pINI->ReadBool(pSection, "IsAllowedInMapGenerator", IsAllowedInMapGenerator);

	LowRadarBrightness1 = (float)pINI->ReadDouble(pSection, "LowRadarBrightness", LowRadarBrightness1);
	LowRadarBrightness2 = (float)pINI->ReadDouble(pSection, "LowRadarBrightness2", LowRadarBrightness1);
	HighRadarBrightness1 = (float)pINI->ReadDouble(pSection, "HighRadarBrightness", HighRadarBrightness1);
	HighRadarBrightness2 = (float)pINI->ReadDouble(pSection, "HighRadarBrightness2", HighRadarBrightness1);

	unknown_int_68 = pINI->ReadInteger(pSection, "unknown_int_68", unknown_int_68);
	unknown_int_6C = pINI->ReadInteger(pSection, "unknown_int_6C", unknown_int_6C);

}

void TheaterTypeClass::InitOriginalTheater()
{
	TheaterTypeClass* theater = nullptr;

	auto nPrintDebugLine = [](TheaterTypeClass* pTheater, const char* pTheaterName)
	{
		if (TypeLog::bEnableLog)
			Debug::Log("%s to Load %s Theater!  \n", (pTheater ? "Success" : "Failed"), pTheaterName);
	};

#pragma region Temperate
	theater = GameCreate<TheaterTypeClass>(
		"TEMPERATE", //1
		"Name:Temperate",//2
		"TEMPERAT",//3
		"ISOTEMP",//4
		"ISOTEM",//5
		"TEM",//6
		"MMT",//7
		"T",//8
		false,
		true,
		1.0f,
		1.6f,
		0.0f,
		1.875f,
		-1,
		0);

	nPrintDebugLine(theater, "Temperate");
#pragma endregion

#pragma region Snow
	theater = GameCreate<TheaterTypeClass>(
		"SNOW",
		"Name:Snow",
		"SNOW",
		"ISOSNOW",
		"ISOSNO",
		"SNO",
		"MMS",
		"A",
		true,
		true,
		0.8f,
		1.1f,
		0.0f,
		1.8125f,
		-1,
		0);

	nPrintDebugLine(theater, "Snow");
#pragma endregion

#pragma region Urban

	theater = GameCreate<TheaterTypeClass>(
		"URBAN",
		"Name:Urban",
		"URBAN",
		"ISOURB",
		"ISOURB",
		"URB",
		"MMU",
		"U",
		false,
		true,
		1.0f,
		1.6f,
		0.0f,
		1.875f,
		0,
		0);

	nPrintDebugLine(theater, "Urban");
#pragma endregion

#pragma region Desert

	theater = GameCreate<TheaterTypeClass>(
		"DESERT",
		"Name:Desert",
		"DESERT",
		"ISODES",
		"ISODES",
		"DES",
		"MMD",
		"D",
		false,
		true,
		1.0f,
		1.6f,
		0.0f,
		1.875f,
		-1,
		0);

	nPrintDebugLine(theater, "Desert");
#pragma endregion

#pragma region NewUrban

	theater = GameCreate<TheaterTypeClass>(
		"NEWURBAN",
		"Name:New Urban",
		"URBANN",
		"ISOUBN",
		"ISOUBN",
		"UBN",
		"MMT",
		"N",
		false,
		true,
		1.0f,
		1.6f,
		0.0f,
		1.875f,
		-1,
		0);

	nPrintDebugLine(theater, "New Urban");
#pragma endregion

#pragma region Lunar

	theater = GameCreate<TheaterTypeClass>(
		"LUNAR",
		"Name:Lunar",
		"LUNAR",
		"ISOLUN",
		"ISOLUN",
		"LUN",
		"MML",
		"L",
		false,
		true,
		1.0f,
		1.6f,
		0.0f,
		1.875f,
		-1,
		0);

	nPrintDebugLine(theater, "Lunar");
#pragma endregion

}

bool TheaterTypeClass::LoadFromINIList(CCINIClass* pINI, bool bDebug)
{
	const char* section = "TheaterTypes";

	if (!pINI->GetSection(section))
		return false;

	for (int i = 0; i < pINI->GetKeyCount(section); ++i)
	{
		if (pINI->ReadString(section, pINI->GetKeyName(section, i), "", Phobos::readBuffer))
		{
			bool bFailed = false;
			if (auto pItem = FindOrAllocate(Phobos::readBuffer))
				pItem->LoadFromINI(pINI);
			else
				bFailed = true;

			if (bDebug)
				Debug::Log("%s Reading %s \"%s\".\n", bFailed ? "Error" : "Success", section, Phobos::readBuffer);
		}
	}

	return true;
}

void TheaterTypeClass::LoadConfiguration()
{
	TheaterTypeClass::InitOriginalTheater();

	auto nFileName = "THEATERS.INI";

	if (CCINIClass* pINI = GameCreate<CCINIClass>())
	{
		if (auto pTheaterIni = GameCreate<CCFileClass>(nFileName))
		{
			bool bFound = false;

			if (pTheaterIni->Exists())
			{
				bFound = true;
				pINI->ReadCCFile(pTheaterIni);
				TheaterTypeClass::LoadFromINIList(pINI, TypeLog::bEnableLog);
			}

			if (TypeLog::bEnableLog)
			{
				if(!bFound)
					Debug::Log("%s not found!\n", nFileName);
			}

			GameDelete(pTheaterIni);
		}
	}

	if (TypeLog::bEnableLog)
	{
		for (auto const& nTheater : TheaterTypesArray)
		{
			Debug::Log(
				"Found Theater [%s] with name [%s] control [%s] Art [%s] Palette [%s] Extension [%s,%s] Letter [%s] Radar [%lf] \n"
				, nTheater->Identifier, nTheater->UIName, nTheater->ControlFileName, nTheater->ArtFileName, nTheater->PaletteFileName,
				nTheater->Extension, nTheater->MMExtension, nTheater->Letter, nTheater->LowRadarBrightness1);
		}
	}
}