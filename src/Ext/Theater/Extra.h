#pragma once

#include <ASMMacros.h>
#include <BasicStructures.h>

class Extra
{
public:
	static void __cdecl _makepath(char* arg1, const char* arg2, const char* arg3, const char* arg4, const char* arg5)
	{
		JMP_STD(0x7C9FF0);
	}

	static char *__cdecl strupr(char* pInput)
	{
		JMP_THIS(0x7DCFC4);
	}

	static ColorStruct Adjust_Brightness(ColorStruct& color, float adjust)
	{
		ColorStruct nRet;

		constexpr auto nClamp = [](float nInput)
		{
			if (nInput > 255.0 || nInput >= 0.0)
			{
				if (nInput > 255.0)
					nInput = 255.0;
			}
			else
				nInput = 0.0;

			return (char)nInput;
		};

		nRet.R = nClamp(color.R * adjust);
		nRet.G = nClamp(color.G * adjust);
		nRet.B = nClamp(color.B * adjust);

		return nRet;
	}
};


