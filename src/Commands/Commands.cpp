#include "Commands.h"

#include "ObjectInfo.h"
#include "NextIdleHarvester.h"
#include "QuickSave.h"

#include <Misc/Otamaa/Commands/AddCommands.h>

DEFINE_HOOK(0x533066, CommandClassCallback_Register, 0x6)
{
	Make(ObjectInfoCommandClass);
	Make(NextIdleHarvesterCommandClass);
	Make(QuickSaveCommandClass);
	Otamaa::AddCommand();

	return 0;
}