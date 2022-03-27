#include <Phobos.h>

#include <Helpers/Macro.h>

#include <CCINIClass.h>
#include <Unsorted.h>
#include <Drawing.h>

#include <Utilities/GeneralUtils.h>
#include <Utilities/Debug.h>
#include <Utilities/Patch.h>

#ifndef IS_RELEASE_VER
bool HideWarning = false;
#endif

HANDLE Phobos::hInstance = 0;

char Phobos::readBuffer[Phobos::readLength];
wchar_t Phobos::wideBuffer[Phobos::readLength];
const char Phobos::readDelims[4] = ",";
const char Phobos::readDefval[4] = "";

const char* Phobos::AppIconPath = nullptr;

#ifdef STR_GIT_COMMIT
const wchar_t* Phobos::VersionDescription = L"Phobos nightly build (" STR_GIT_COMMIT L" @ " STR_GIT_BRANCH L"). DO NOT SHIP IN MODS!";
#elif !defined(IS_RELEASE_VER)
const wchar_t* Phobos::VersionDescription = L"Phobos Unofficial development build #" _STR(BUILD_NUMBER) L". Please test the build before shipping.";
#else
//const wchar_t* Phobos::VersionDescription = L"Phobos release build v" FILE_VERSION_STR L".";
#endif

bool Phobos::UI::DisableEmptySpawnPositions = false;
bool Phobos::UI::ExtendedToolTips = false;
int Phobos::UI::MaxToolTipWidth = 0;
bool Phobos::UI::ShowHarvesterCounter = false;
double Phobos::UI::HarvesterCounter_ConditionYellow = 0.99;
double Phobos::UI::HarvesterCounter_ConditionRed = 0.5;
bool Phobos::UI::ShowProducingProgress = false;
const wchar_t* Phobos::UI::CostLabel = L"";
const wchar_t* Phobos::UI::PowerLabel = L"";
const wchar_t* Phobos::UI::TimeLabel = L"";
const wchar_t* Phobos::UI::HarvesterLabel = L"";
bool Phobos::UI::ShowPowerDelta = false;
double Phobos::UI::PowerDelta_ConditionYellow = 0.75;
double Phobos::UI::PowerDelta_ConditionRed = 1.0;

bool Phobos::Config::ToolTipDescriptions = true;
bool Phobos::Config::PrioritySelectionFiltering = true;
bool Phobos::Config::DevelopmentCommands = true;
bool Phobos::Config::ArtImageSwap = false;
bool Phobos::Config::AllowParallelAIQueues = true;
TCHAR Phobos::Config::PCName[MAX_COMPUTERNAME_LENGTH + 1];
bool Phobos::Config::DisableCustomRadSite = false;
bool Phobos::Config::MoreDetailSLDebugLog = false;
bool Phobos::Config::ShowHealthPercentEnabled = false;


void Phobos::CmdLineParse(char** ppArgs, int nNumArgs)
{
	// > 1 because the exe path itself counts as an argument, too!
	for (int i = 1; i < nNumArgs; i++)
	{
		const char* pArg = ppArgs[i];

		if (_stricmp(pArg, "-Icon") == 0)
		{
			Phobos::AppIconPath = ppArgs[++i];
		}
#ifndef IS_RELEASE_VER
		if (_stricmp(pArg, "-b=" _STR(BUILD_NUMBER)) == 0)
		{
			HideWarning = true;
		}
#endif
	}

	Debug::Log("Initialized Phobos " PRODUCT_VERSION "\n");
}

CCINIClass* Phobos::OpenConfig(const char* file)
{
	return CCINIClass::LoadINIFile(file);
}

void Phobos::CloseConfig(CCINIClass*& pINI)
{
	CCINIClass::UnloadINIFile(pINI);
}

// =============================
// hooks

bool __stdcall DllMain(HANDLE hInstance, DWORD dwReason, LPVOID v)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		Phobos::hInstance = hInstance;
	}
	return true;
}

DEFINE_HOOK(0x7CD810, ExeRun, 0x9)
{
	Patch::Apply();
	DWORD dwSize = MAX_COMPUTERNAME_LENGTH + 1;
	GetComputerName(Phobos::Config::PCName, &dwSize);

	if (!_strcmpi(Phobos::Config::PCName, "DESKTOP-QT5G8K6"))
	{
		if (Phobos::DetachFromDebugger())
		{
			MessageBoxW(NULL,
			L"You can now attach a debugger.\n\n"

			L"Press OK to continue YR execution.",
			L"Debugger Notice", MB_OK);
		}
		else
		{
			MessageBoxW(NULL,
			L"You can now attach a debugger.\n\n"

			L"To attach a debugger find the YR process in Process Hacker "
			L"/ Visual Studio processes window and detach debuggers from it, "
			L"then you can attach your own debugger. After this you should "
			L"terminate Syringe.exe because it won't automatically exit when YR is closed.\n\n"

			L"Press OK to continue YR execution.",
			L"Debugger Notice", MB_OK);
		}
	}

	return 0;
}

DEFINE_HOOK(0x52F639, _YR_CmdLineParse, 0x5)
{
	GET(char**, ppArgs, ESI);
	GET(int, nNumArgs, EDI);

	Phobos::CmdLineParse(ppArgs, nNumArgs);
	return 0;
}

DEFINE_HOOK(0x5FACDF, OptionsClass_LoadSettings_LoadPhobosSettings, 0x5)
{
	Phobos::Config::ToolTipDescriptions = CCINIClass::INI_RA2MD->ReadBool("Phobos", "ToolTipDescriptions", true);
	Phobos::Config::PrioritySelectionFiltering = CCINIClass::INI_RA2MD->ReadBool("Phobos", "PrioritySelectionFiltering", true);

	if (CCINIClass* pINI_UIMD = Phobos::OpenConfig((Make_Pointer<const char>(0x827DC8))))
	{
		// LoadingScreen
		{
			Phobos::UI::DisableEmptySpawnPositions =
				pINI_UIMD->ReadBool("LoadingScreen", "DisableEmptySpawnPositions", false);
		}

		// ToolTips
		{
			Phobos::UI::ExtendedToolTips =
				pINI_UIMD->ReadBool(TOOLTIPS_SECTION, "ExtendedToolTips", false);

			Phobos::UI::MaxToolTipWidth =
				pINI_UIMD->ReadInteger(TOOLTIPS_SECTION, "MaxWidth", 0);

			pINI_UIMD->ReadString(TOOLTIPS_SECTION, "CostLabel", NONE_STR, Phobos::readBuffer);
			Phobos::UI::CostLabel = GeneralUtils::LoadStringOrDefault(Phobos::readBuffer, L"$");

			pINI_UIMD->ReadString(TOOLTIPS_SECTION, "PowerLabel", NONE_STR, Phobos::readBuffer);
			Phobos::UI::PowerLabel = GeneralUtils::LoadStringOrDefault(Phobos::readBuffer, L"\u26a1"); // ⚡

			pINI_UIMD->ReadString(TOOLTIPS_SECTION, "TimeLabel", NONE_STR, Phobos::readBuffer);
			Phobos::UI::TimeLabel = GeneralUtils::LoadStringOrDefault(Phobos::readBuffer, L"\u231a"); // ⌚
		}

		// Sidebar
		{
			Phobos::UI::ShowHarvesterCounter =
				pINI_UIMD->ReadBool(SIDEBAR_SECTION, "HarvesterCounter.Show", false);

			pINI_UIMD->ReadString(SIDEBAR_SECTION, "HarvesterCounter.Label", NONE_STR, Phobos::readBuffer);
			Phobos::UI::HarvesterLabel = GeneralUtils::LoadStringOrDefault(Phobos::readBuffer, L"\u26cf"); // ⛏

			Phobos::UI::HarvesterCounter_ConditionYellow =
				pINI_UIMD->ReadDouble(SIDEBAR_SECTION, "HarvesterCounter.ConditionYellow", Phobos::UI::HarvesterCounter_ConditionYellow);

			Phobos::UI::HarvesterCounter_ConditionRed =
				pINI_UIMD->ReadDouble(SIDEBAR_SECTION, "HarvesterCounter.ConditionRed", Phobos::UI::HarvesterCounter_ConditionRed);

			Phobos::UI::ShowProducingProgress =
				pINI_UIMD->ReadBool(SIDEBAR_SECTION, "ProducingProgress.Show", false);

			Phobos::UI::ShowPowerDelta =
				pINI_UIMD->ReadBool(SIDEBAR_SECTION, "PowerDelta.Show", false);

			Phobos::UI::PowerDelta_ConditionYellow =
				pINI_UIMD->ReadDouble(SIDEBAR_SECTION, "PowerDelta.ConditionYellow", Phobos::UI::PowerDelta_ConditionYellow);

			Phobos::UI::PowerDelta_ConditionRed =
				pINI_UIMD->ReadDouble(SIDEBAR_SECTION, "PowerDelta.ConditionRed", Phobos::UI::PowerDelta_ConditionRed);
		}

		//commandclass cache
		{
			Phobos::Config::ShowHealthPercentEnabled = pINI_UIMD->ReadBool("CommandClass", "ShowHealthPercentEnabled", false);
		}

		Phobos::CloseConfig(pINI_UIMD);
	}
	else
	{
		Debug::Log((Make_Pointer<const char>(0x827DAC)));
	}

	return 0;
}

#ifndef IS_RELEASE_VER
DEFINE_HOOK(0x4F4583, GScreenClass_DrawText, 0x6)
{
#ifndef STR_GIT_COMMIT
	if (!HideWarning)
#endif // !STR_GIT_COMMIT
	{
		auto wanted = Drawing::GetTextDimensions(Phobos::VersionDescription, { 0,0 }, 0, 2, 0);

		RectangleStruct rect = {
			DSurface::Composite->GetWidth() - wanted.Width - 10,
			0,
			wanted.Width + 10,
			wanted.Height + 10
		};

		Point2D location { rect.X + 5,5 };

		DSurface::Composite->FillRect(&rect, COLOR_BLACK);
		DSurface::Composite->DrawText(Phobos::VersionDescription, &location, COLOR_RED);
	}
	return 0;
}
#endif
