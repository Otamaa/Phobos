#pragma once

#include "CaptureObjects.h"
#include "ShowHealthPercent.h"

namespace Otamaa
{
	void AddCommand()
	{
		Make(ShowHealthPercentCommandClass);
		Make(CaptureObjectsCommandClass);
	}
}