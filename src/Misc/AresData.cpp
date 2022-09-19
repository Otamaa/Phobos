#include "AresData.h"

#include <ASMMacros.h>
#include <Phobos.h>

#include <filesystem>
#include <tlhelp32.h>

class TechnoClass;
class TechnoTypeClass;

constexpr const wchar_t* ARES_DLL = L"Ares.dll";
constexpr const char* ARES_DLL_S = "Ares.dll";

DWORD AresData::HandleConvert::CallableAddress = 0x0;
DWORD AresData::HandleConvert::FunctionOffset = 0x44130;
uintptr_t AresData::AresBaseAddress = 0x0;
HMODULE AresData::AresDllHmodule = nullptr;

std::filesystem::path get_module_path(HMODULE module)
{
	WCHAR buf[4096];
	return GetModuleFileNameW(module, buf, ARRAYSIZE(buf)) ? buf : std::filesystem::path();
}

uintptr_t GetModuleBaseAddress(const char* modName)
{
	HANDLE hCurrentProcess = GetCurrentProcess();
	uintptr_t modBaseAddr = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, GetProcessId(hCurrentProcess));
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 modEntry { };
		modEntry.dwSize = sizeof(modEntry);
		if (Module32First(hSnap, &modEntry))
		{
			do
			{
				if (!_strcmpi(modEntry.szModule, modName))
				{
					modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
					break;
				}
			}
			while (Module32Next(hSnap, &modEntry));
		}
	}
	CloseHandle(hSnap);
	return modBaseAddr;
}

void AresData::Init()
{
	AresData::AresBaseAddress = GetModuleBaseAddress(ARES_DLL_S);

	if (GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_PIN, ARES_DLL, &AresDllHmodule))
	{
		HandleConvert::CallableAddress = AresData::AresBaseAddress + HandleConvert::FunctionOffset;
	}
}

void __stdcall AresData::HandleConvert::Exec(TechnoClass* pTechno, TechnoTypeClass* pConvertTo)
{
	JMP_STD(HandleConvert::CallableAddress);
}
