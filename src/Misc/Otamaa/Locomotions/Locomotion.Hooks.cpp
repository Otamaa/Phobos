#include <Locomotions/TClassFactory.h>

#include <Utilities/Macro.h>
#include <Utilities/Debug.h>
#include <Utilities/TemplateDef.h>
#include <Utilities/GeneralUtils.h>

#include <Locomotions/LevitateLocomotor.h>

DEFINE_HOOK(0x6BC4AE, WinMain_CoRegisterClassObjects_Locomotions, 0x7)
{

	Debug::Log("Registering new com objects...\n");

	Debug::Log("1.TestLocomotionClass\n");
	{ 
		REGISTER_CLASS(TestLocomotionClass);
	}
	
	Debug::Log("  ...OK!\n");

	return 0;
}