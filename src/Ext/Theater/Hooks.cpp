#include <New/Type/TheaterTypeClass.h>
#include "Extra.h"

#define CURRENT_THEATER (*ScenarioClass::Instance).Theater

class CCINIClassFake final : public CCINIClass
{
public:

	TheaterType _Get_TheaterType(const char* section, const char* entry, const TheaterType defvalue)
	{
		char buffer[2048];

		if (CCINIClass::ReadString(section, entry, "", buffer, sizeof(buffer)))
		{ return TheaterTypeClass::From_Name(buffer); }

		return defvalue;
	}

	bool _Put_TheaterType(const char* section, const char* entry, TheaterType value)
	{
		return CCINIClass::WriteString(section, entry, TheaterTypeClass::GetIdentifier(value));
	}
};

namespace Log
{
	bool bEnableLog = false;

	void Exec(const char* pFunct, TheaterType nTheater)
	{
		if (bEnableLog)
		{
			Debug::Log("%s Executed! for [%d = %s] \n", pFunct, nTheater, TheaterTypeClass::GetIdentifier(nTheater));
		}
	}

	void Exec(const char* pFunct, int nIdx, const char* pTheaterName)
	{
		if (bEnableLog)
			Debug::Log("%s Executed! for [%d = %s] \n", pFunct, nIdx, pTheaterName);
	}
}

DEFINE_HOOK(0x48DBE0, TheaterTypeClass_FindIndex, 0x0)
{
	GET(char*, nTheaterName, ECX);

	auto nIdx = TheaterTypeClass::FindIndex(nTheaterName);

	Log::Exec(__FUNCTION__, nIdx, nTheaterName);

	R->EAX<int>(nIdx);
	return 0x48DC12;
}

#pragma region IsoTileTypeHooks

DEFINE_HOOK(0x5452F2, IsometricTileTypeClass_TheaterType_Slope, 0x6)
{
	Log::Exec(__FUNCTION__, CURRENT_THEATER);
	R->EAX(TheaterTypeClass::GetExtension(CURRENT_THEATER));
	return 0x5452F8;
}

//Isometric pal = ISO+Extension.pal
DEFINE_HOOK(0x54547F, IsometricTileTypeClass_TheaterType_ISOPal, 0x6)
{
	Log::Exec(__FUNCTION__, CURRENT_THEATER);
	R->EAX(TheaterTypeClass::GetExtension(CURRENT_THEATER));
	return 0x545485;
}

//Ini  file = ControlFilename+MD.ini
DEFINE_HOOK(0x5454F0, IsometricTileTypeClass_TheaterType_IniFile, 0x6)
{
	Log::Exec(__FUNCTION__, CURRENT_THEATER);
	R->EDX(TheaterTypeClass::GetControlFileName(CURRENT_THEATER));
	return 0x5454F6;
}

DEFINE_HOOK(0x546662, IsometricTileTypeClass_TheaterType_makepath, 0x6)
{
	Log::Exec(__FUNCTION__, CURRENT_THEATER);
	R->EAX(TheaterTypeClass::GetExtension(CURRENT_THEATER));
	return 0x546668;
}

DEFINE_HOOK(0x546753, IsometricTileTypeClass_TheaterType_MMx, 0x6)
{
	Log::Exec(__FUNCTION__, CURRENT_THEATER);
	R->EAX(TheaterTypeClass::GetMMExtension(CURRENT_THEATER));
	return 0x546759;
}

/*  Possible Ares conflict
	DEFINE_HOOK(5F9070, ObjectTypeClass_Load2DArt, 0) // not really , ugh
	DEFINE_HOOK(5F96B0, ObjectTypeClass_TheaterSpecificID, 6) //hackable
*/

// dont really know how to do these actually
// but these works well , so , whatever
namespace _ReplaceMakePath
{
	void __cdecl Exec(char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5 //this were real theater input
	)
	{
		Extra::_makepath(arg1, arg2, arg3, arg4, TheaterTypeClass::GetExtension(CURRENT_THEATER));
	}

	void __cdecl Exec2(char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
	{
		char nBuffer[256];
		std::sprintf(nBuffer, ".%s", TheaterTypeClass::GetCharExtension(CURRENT_THEATER));
		Extra::_makepath(arg1, arg2, arg3, arg4, nBuffer);
	}
}

// Why the fuck this trying to load direcly from name like that ? 
// WWP ?
DEFINE_POINTER_CALL(0x546843, _ReplaceMakePath::Exec2);
DEFINE_POINTER_CALL(0x54692B, _ReplaceMakePath::Exec2);
#pragma endregion

#pragma region ScenarioClass_InitTheater

//How mix loaded ? 
// controlfilename.mix
// ArtFileName.mix
// Extension.mix
// PaletteFileName+md.mix
// controlfilename+md.mix

DEFINE_HOOK(0x5349E3, ScenarioClass_InitTheater_TheaterType_ControlFileName, 0x6)
{
	Log::Exec(__FUNCTION__, CURRENT_THEATER);
	GET(TheaterType, nType, EDI);
	R->EBX(TheaterTypeClass::GetControlFileName(nType));
	return 0x5349E9;
}

//PaletteFileName.pal
DEFINE_HOOK(0x534BEE, ScenarioClass_InitTheater_Palette_Filename, 0x5)
{
	Log::Exec(__FUNCTION__, CURRENT_THEATER);
	R->EBX(TheaterTypeClass::GetPaletteFileName(CURRENT_THEATER));
	return 0;
}

// UNIT+Extension.pal
DEFINE_HOOK(0x534CB1, ScenarioClass_IniTheater_Palette_Unit, 0x6)
{
	Log::Exec(__FUNCTION__, CURRENT_THEATER);
	R->EDX(TheaterTypeClass::GetExtension(CURRENT_THEATER));
	return 0x0;
}

DEFINE_HOOK(0x5349F2, ScenarioClass_InitTheater_TheaterType_ArtMix, 0x6)
{
	Log::Exec(__FUNCTION__, CURRENT_THEATER);
	GET(TheaterType, nType, EDI);
	R->ECX(TheaterTypeClass::GetArtFileName(nType));
	return 0x5349F8;
}

DEFINE_HOOK(0x534A05, ScenarioClass_InitTheater_TheaterType_Control, 0x6)
{
	Log::Exec(__FUNCTION__, CURRENT_THEATER);
	GET(TheaterType, nType, EDI);
	R->EAX(TheaterTypeClass::GetControlFileName(nType));
	return 0x534A0B;
}

DEFINE_HOOK(0x534A1C, ScenarioClass_initTheater_TheaterType_MdMix, 0x6)
{
	Log::Exec(__FUNCTION__, CURRENT_THEATER);
	GET(TheaterType, nType, EDI);
	R->ECX(TheaterTypeClass::GetPaletteFileName(nType));
	return 0x534A22;
}

DEFINE_HOOK(0x534A3C, ScenarioClass_initTheater_TheaterType_DebugLog, 0x6)
{
	Log::Exec(__FUNCTION__, CURRENT_THEATER);
	GET(TheaterType, nType, EDI);
	R->ECX(TheaterTypeClass::GetIdentifier(nType));
	return 0x534A42;
}

DEFINE_HOOK(0x534A9D, ScenarioClass_initTheater_TheaterType_ArticCheck, 0x6)
{
	Log::Exec(__FUNCTION__, CURRENT_THEATER);
	return TheaterTypeClass::GetIsArtic(CURRENT_THEATER) ? 0x534AA6 : 0x534AD6;
}

#pragma endregion

//AnimType
DEFINE_POINTER_CALL(0x4279CC, _ReplaceMakePath::Exec);
DEFINE_POINTER_CALL(0x427B07, _ReplaceMakePath::Exec);
DEFINE_POINTER_CALL(0x428910, _ReplaceMakePath::Exec);
DEFINE_POINTER_CALL(0x428CCC, _ReplaceMakePath::Exec);

//BuildingType 
DEFINE_POINTER_CALL(0x45EA08, _ReplaceMakePath::Exec);
DEFINE_POINTER_CALL(0x45EA6B, _ReplaceMakePath::Exec);

//OverlayType
DEFINE_POINTER_CALL(0x5FE67E, _ReplaceMakePath::Exec);
DEFINE_POINTER_CALL(0x5FEBA1, _ReplaceMakePath::Exec);
DEFINE_POINTER_CALL(0x5FEE4F, _ReplaceMakePath::Exec);

//SmudgeTypes
DEFINE_POINTER_CALL(0x6B54DC, _ReplaceMakePath::Exec);
DEFINE_POINTER_CALL(0x6B57B4, _ReplaceMakePath::Exec);

//TerrainType 
DEFINE_POINTER_CALL(0x71DCEF, _ReplaceMakePath::Exec);

//objectType
DEFINE_POINTER_CALL(0x5F9169, _ReplaceMakePath::Exec);
//5F96F8 //ares
//5F91D7 //ares

DEFINE_HOOK(0x483DE5, CellClass_CheckPassability_Artict, 0x6)
{
	Log::Exec(__FUNCTION__, CURRENT_THEATER);
	GET(ScenarioClass*, pScen, ECX);
	return TheaterTypeClass::GetIsArtic(pScen->Theater) ? 0x483E0C : 0x483DF5;
}

DEFINE_HOOK(0x6DAE3E, TacticalClass_DrawWaypoints_SelectColor, 0x8)
{
	Log::Exec(__FUNCTION__, CURRENT_THEATER);
	GET(ScenarioClass*, pScen, ECX);
	R->EAX(TheaterTypeClass::GetIsArtic(pScen->Theater) ? 12 : 14);
	return 0;
}

DEFINE_HOOK(0x71C076, TerrainClass_ClearOccupyBit_Theater, 0x7)
{
	Log::Exec(__FUNCTION__, CURRENT_THEATER);
	GET(ScenarioClass*, pScen, EAX);
	return TheaterTypeClass::GetIsArtic(pScen->Theater) ? 0x71C08D : 0x71C07F;
}

DEFINE_HOOK(0x71C076, TerrainClass_SetOccupyBit_Theater, 0x7)
{
	Log::Exec(__FUNCTION__, CURRENT_THEATER);
	GET(ScenarioClass*, pScen, EAX);
	return TheaterTypeClass::GetIsArtic(pScen->Theater) ? 0x71C12D : 0x71C11F;
}

namespace _fetchColor
{
	ColorStruct __fastcall Exec(ColorStruct& nReturn, ColorStruct& nColor, float nval)
	{
		//First argument is unused !
		Log::Exec(__FUNCTION__, CURRENT_THEATER);
		return Extra::Adjust_Brightness(nColor, TheaterTypeClass::GetLowRadarBrightness(CURRENT_THEATER));
	}
}

DEFINE_POINTER_CALL(0x47C324, _fetchColor::Exec);

// these crashing a lot when ares trying to map capture
// i suespect CCINIClass / INIClass align is broken 
// i checked it with static assert , dunno how to fix it 
// thus causing this to crash with 0x0000001 EIP
// or maybe just my bad code , idk 
namespace _fetchCCINI
{
	bool __fastcall Exec(CCINIClassFake* pThis, const char* section, const char* entry, TheaterType value)
	{
		return pThis->_Put_TheaterType(section, entry, value);
	}
}

DEFINE_POINTER_CALL(0x4AD81B, _fetchCCINI::Exec);

DEFINE_HOOK(0x5997C0, RMGClass_TheaterType_initRandomMap, 0x6)
{
	Log::Exec(__FUNCTION__, CURRENT_THEATER);
	GET(TheaterType, nIndex, EAX);
	R->ECX(TheaterTypeClass::GetIdentifier(nIndex));
	return 0x5997C6;
}

DEFINE_HOOK(0x6275B7, TheaterTypeClass_ProcessOtherPalettes_screwthese, 0x6)
{ return 0x627680; }

#include <MixFileClass.h>
DEFINE_HOOK(0x627699, TheaterTypeClass_ProcessOtherPalettes_Process, 0x6)
{
	GET_STACK(char*, pOriginalName, STACK_OFFS(0x424, -0x4));

	char nBuffer[256];
	std::sprintf(nBuffer, "%s%s.PAL", pOriginalName, TheaterTypeClass::GetCharExtension(CURRENT_THEATER));
	Extra::strupr(nBuffer);// Uppercase all !

	// original stuffs were stored here 
	//LEA_STACK(char*, pNameProcessed, STACK_OFFS(0x424, 0x400));
	Debug::Log(__FUNCTION__" Exec OutResult [%s] \n", nBuffer);
	R->EAX(MixFileClass::Retrieve(nBuffer, false));
	return 0x6276A4;
}

// Picked from Ares custom theater branch
DEFINE_HOOK(0x74D45A, TheaterTypeClass_ProcessVeinhole, 0x0)
{
	Log::Exec(__FUNCTION__, CURRENT_THEATER);
	GET(TheaterType, index, ECX);
	R->EAX(TheaterTypeClass::GetCharExtension(index));
	R->ECX<DWORD>(R->ESP());
	return 0x74D468;
}
#undef CURRENT_THEATER