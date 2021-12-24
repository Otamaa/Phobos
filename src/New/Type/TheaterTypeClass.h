#pragma once

#include <Utilities/Debug.h>
#include <Utilities/Macro.h>
#include <ScenarioClass.h>

#include <CCINIClass.h>
#include <Theater.h>
#include <string>

#include <Phobos.CRT.h>

class TheaterTypeClass
{
private:

	TheaterTypeClass() :

		Identifier("<none>"),
		UIName("Name:<none>"),
		ControlFileName("X"),
		ArtFileName("X"),
		PaletteFileName("X"),
		Extension("X"),
		MMExtension("X"),
		Letter("X"),
		IsArctic(false),
		LowRadarBrightness1(1.0f),
		LowRadarBrightness2(1.0f),
		HighRadarBrightness1(1.0f),
		HighRadarBrightness2(1.0f),
		unknown_int_68(0),
		unknown_int_6C(0)
	{}

public:

	TheaterTypeClass(const char* pName);
	TheaterTypeClass
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
	);

	~TheaterTypeClass();

	virtual void LoadFromINI(CCINIClass* pINI);

	static TheaterType From_Name(const char* pName);
	static const TheaterTypeClass& As_Reference(TheaterType nType);
	static const TheaterTypeClass* As_Pointer(TheaterType nType);
	static const TheaterTypeClass& As_Reference(const char* pName);
	static const TheaterTypeClass* As_Pointer(const char* pName);
	static const char* GetIdentifier(TheaterType type);
	static const char* GetUIName(TheaterType type);
	static const char* GetControlFileName(TheaterType type);
	static const char* GetArtFileName(TheaterType type);
	static const char* GetPaletteFileName(TheaterType type);
	static const char* GetExtension(TheaterType type);
	static char* GetCharExtension(TheaterType type);
	static const char* GetMMExtension(TheaterType type);
	static const char* GetLetter(TheaterType type);
	static bool GetIsArtic(TheaterType type);
	static bool GetAllowMapGen(TheaterType type);
	static float GetLowRadarBrightness(TheaterType type, bool bSecond = false);
	static float GetHighRadarBrightness(TheaterType type, bool bSecond = false);
	static int GetUnknownVal(TheaterType type, bool bSecond = false);
	static void LoadConfiguration();
	static void InitOriginalTheater();
	static void CleanUp();
	static int FindIndex(const char* pName);
	static bool LoadFromINIList(CCINIClass* pINI, bool bDebug = false);
	static TheaterTypeClass* FindOrAllocate(const char* pName);


private:

	char Identifier[16]; //1
	char UIName[32];//2
	char ControlFileName[10];//3 
	char ArtFileName[10];//4 , datamix
	char PaletteFileName[10];//5 , expansionmix
	char Extension[4];//6, suffix
	char MMExtension[4];//7
	char Letter[2]; //Letter , 8
	bool IsArctic;
	bool IsAllowedInMapGenerator;

	float LowRadarBrightness1;
	float LowRadarBrightness2; //absolute

	float HighRadarBrightness1; //absolute
	float HighRadarBrightness2; //absolute

	int	unknown_int_68; //absolute
	int	unknown_int_6C; //absolute
};

extern DynamicVectorClass<TheaterTypeClass*> TheaterTypesArray;