#pragma once
#include <Windows.h>

class TechnoClass;
class TechnoTypeClass;
struct AresData
{
	static  HMODULE AresDllHmodule;
	static uintptr_t AresBaseAddress;

	struct HandleConvert
	{
		static DWORD Offset;
		static DWORD CallableAddress;

		static void __stdcall Exec(TechnoClass* pTechno, TechnoTypeClass* pConvertTo);
	};


	static void Init();
	static void UnInit()
	{
		FreeLibrary(AresDllHmodule);
	}
};
