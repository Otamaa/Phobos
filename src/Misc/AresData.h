#pragma once
#include <Windows.h>

class TechnoClass;
class TechnoTypeClass;
struct AresData
{
	static HMODULE AresDllHmodule;
	static uintptr_t AresBaseAddress;


	// Add new function here
	// use Function name As struct name
	struct HandleConvert
	{
		static DWORD FunctionOffset;
		static DWORD CallableAddress;

		static void __stdcall Exec(TechnoClass* pTechno, TechnoTypeClass* pConvertTo);
	};


	static void Init();
	static void UnInit()
	{
		FreeLibrary(AresDllHmodule);
	}
};
