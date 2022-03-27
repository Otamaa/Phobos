
#include <Utilities/Macro.h>

static void __fastcall UnitTypeClass_OneTIme()
{ JMP_STD(0x7473E0); }

static void __fastcall OneTime_Init_Intercept()
{
	UnitTypeClass_OneTIme();
	//VesselTypeClass_OneTime();
}

DEFINE_POINTER_CALL(0x53173A, &OneTime_Init_Intercept);