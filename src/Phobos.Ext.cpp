#include <Phobos.h>
#include <Phobos.Ext.h>

#include <Ext/Aircraft/Body.h>
#include <Ext/AnimType/Body.h>
#include <Ext/Anim/Body.h>
#include <Ext/Building/Body.h>
#include <Ext/BuildingType/Body.h>
#include <Ext/Bullet/Body.h>
#include <Ext/BulletType/Body.h>
#include <Ext/House/Body.h>
#include <Ext/RadSite/Body.h>
#include <Ext/Rules/Body.h>
#include <Ext/Scenario/Body.h>
#include <Ext/Script/Body.h>
#include <Ext/Side/Body.h>
#include <Ext/SWType/Body.h>
#include <Ext/Tiberium/Body.h>
#include <Ext/TAction/Body.h>
#include <Ext/Team/Body.h>
#include <Ext/Techno/Body.h>
#include <Ext/TechnoType/Body.h>
#include <Ext/TerrainType/Body.h>
#include <Ext/VoxelAnim/Body.h>
#include <Ext/VoxelAnimType/Body.h>
#include <Ext/WarheadType/Body.h>
#include <Ext/WeaponType/Body.h>

#include <New/Type/RadTypeClass.h>
#include <New/Type/LaserTrailTypeClass.h>
#include <Misc/Otamaa/AddMassActions.h>

#include <utility>


// Add more class names as you like
auto MassActions = MassAction <

	HouseTypeExt,
	TiberiumExt,
	BombExt,
	CaptureExt,
	ScriptTypeExt,
	TerrainExt,
	HoverTypeClass,
	ShapeHandlerEnumerator,
	ArmorTypeClass,
	// Ext classes
	AircraftExt,
	AnimTypeExt,
	AnimExt,
	BuildingExt,
	BuildingTypeExt,
	BulletExt,
	BulletTypeExt,
	HouseExt,
	RadSiteExt,
	RulesExt,
	ScenarioExt,
	ScriptExt,
	SideExt,
	SWTypeExt,
	TActionExt,
	TeamExt,
	TechnoExt,
	TechnoTypeExt,
	TerrainTypeExt,
	VoxelAnimExt,
	VoxelAnimTypeExt,
	WarheadTypeExt,
	WeaponTypeExt,
	// New classes
	ShieldTypeClass,
	LaserTrailTypeClass,
	RadTypeClass
	//,TrailType
	// other classes
> ();

DEFINE_HOOK(0x7258D0, AnnounceInvalidPointer, 0x6)
{
	GET(AbstractClass* const, pInvalid, ECX);
	GET(bool const, removed, EDX);

	Phobos::PointerGotInvalid(pInvalid, removed);

	return 0;
}

DEFINE_HOOK(0x685659, Scenario_ClearClasses, 0xa)
{
	Phobos::Clear();
	return 0;
}

void Phobos::Clear()
{
	MassActions.Clear();
}

void Phobos::PointerGotInvalid(AbstractClass* const pInvalid, bool const removed)
{
	MassActions.InvalidPointer(pInvalid, removed);
}

HRESULT Phobos::SaveGameData(IStream* pStm)
{
	Debug::Log("Saving global Phobos data\n");

	if (!MassActions.Save(pStm))
		return E_FAIL;

	Debug::Log("Finished saving the game\n");

	return S_OK;
}

void Phobos::LoadGameData(IStream* pStm)
{
	Debug::Log("Loading global Phobos data\n");

	if (!MassActions.Load(pStm))
		Debug::Log("Error loading the game\n");
	else
		Debug::Log("Finished loading the game\n");
}

#ifdef DEBUG

#pragma warning (disable : 4091)
#pragma warning (disable : 4245)

#include <Dbghelp.h>
#include <tlhelp32.h>
#include <winternl.h>

bool Phobos::DetachFromDebugger()
{
	auto GetDebuggerProcessId = [](DWORD dwSelfProcessId) -> DWORD
	{
		DWORD dwParentProcessId = -1;
		HANDLE hSnapshot = CreateToolhelp32Snapshot(2, 0);
		PROCESSENTRY32 pe32;
		pe32.dwSize = sizeof(PROCESSENTRY32);
		Process32First(hSnapshot, &pe32);
		do
		{
			if (pe32.th32ProcessID == dwSelfProcessId)
			{
				dwParentProcessId = pe32.th32ParentProcessID;
				break;
			}
		}
		while (Process32Next(hSnapshot, &pe32));
		CloseHandle(hSnapshot);
		return dwParentProcessId;
	};

	HMODULE hModule = LoadLibrary("ntdll.dll");
	if (hModule != NULL)
	{
		auto const NtRemoveProcessDebug =
			(NTSTATUS(__stdcall*)(HANDLE, HANDLE))GetProcAddress(hModule, "NtRemoveProcessDebug");
		auto const NtSetInformationDebugObject =
			(NTSTATUS(__stdcall*)(HANDLE, ULONG, PVOID, ULONG, PULONG))GetProcAddress(hModule, "NtSetInformationDebugObject");
		auto const NtQueryInformationProcess =
			(NTSTATUS(__stdcall*)(HANDLE, ULONG, PVOID, ULONG, PULONG))GetProcAddress(hModule, "NtQueryInformationProcess");
		auto const NtClose =
			(NTSTATUS(__stdcall*)(HANDLE))GetProcAddress(hModule, "NtClose");

		HANDLE hDebug;
		HANDLE hCurrentProcess = GetCurrentProcess();
		NTSTATUS status = NtQueryInformationProcess(hCurrentProcess, 30, &hDebug, sizeof(HANDLE), 0);
		if (0 <= status)
		{
			ULONG killProcessOnExit = FALSE;
			status = NtSetInformationDebugObject(
				hDebug,
				1,
				&killProcessOnExit,
				sizeof(ULONG),
				NULL
			);
			if (0 <= status)
			{
				const auto pid = GetDebuggerProcessId(GetProcessId(hCurrentProcess));
				status = NtRemoveProcessDebug(hCurrentProcess, hDebug);
				if (0 <= status)
				{
					//sprintf_s(Phobos::readBuffer, "taskkill /F /PID %d", pid);
					CRT::sprintf(Phobos::readBuffer, "taskkill /F /PID %d", pid);
					WinExec(Phobos::readBuffer, SW_HIDE);
					return true;
				}
			}
			NtClose(hDebug);
		}
		FreeLibrary(hModule);
	}

	return false;
}
#endif