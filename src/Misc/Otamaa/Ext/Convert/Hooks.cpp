#include "Body.h"


DEFINE_HOOK(0x52BA78, _YR_GameInit_Pre, 0x5)
{
	Phobos::PhobosTlsThread_1 = TlsAlloc();
	Phobos::PhobosTlsThread_2 = TlsAlloc();

	if (Phobos::PhobosTlsThread_1 == TLS_OUT_OF_INDEXES)
	{
		Debug::Log(__FUNCTION__" TLS1 Out of IDX! \n");
	}
	else
	{
		Debug::Log(__FUNCTION__" TLS1 Allocated Successfully! \n");
	}

	if (Phobos::PhobosTlsThread_2 == TLS_OUT_OF_INDEXES)
	{
		Debug::Log(__FUNCTION__" TLS2 Out of IDX! \n");
	}
	else
	{
		Debug::Log(__FUNCTION__" TLS2 Allocated Successfully! \n");
	}


	return 0;
}

DEFINE_HOOK_AGAIN(0x437990, DSurface_BlitWithRLE_Off, 0x7)
DEFINE_HOOK_AGAIN(0x437B33, DSurface_BlitWithRLE_Off, 0x7)
DEFINE_HOOK_AGAIN(0x437D47, DSurface_BlitWithRLE_Off, 0x7)
DEFINE_HOOK(0x43746A, DSurface_BlitWithRLE_Off, 0x7)
{
	//Debug::Log(__FUNCTION__" Clearing TLS ! \n");
	TlsSetValue(Phobos::PhobosTlsThread_1, 0);
	TlsSetValue(Phobos::PhobosTlsThread_2, 0);
	return 0;
}

DEFINE_HOOK(0x437E45, DSurface_BlitWithRLE_Add, 0x5)
{
	//Debug::Log(__FUNCTION__" Executed ! \n");
	GET(DWORD, pEBP, EBP);

	if (TlsGetValue(Phobos::PhobosTlsThread_2))
	{
		if (pEBP)
		{
			if (auto nTen = *reinterpret_cast<DWORD*>(pEBP + 0x10))
			{
				if (auto nEightTeen = *reinterpret_cast<DWORD*>(pEBP + 0x18))
				{
					*reinterpret_cast<DWORD*>(pEBP + 0x18) = nEightTeen + nTen;
				}
			}
		}
	}

	return 0;
}

DEFINE_HOOK(0x4376BB , DSurface_BlitWithPlain_AdjustHeight2, 0x6)
{
	GET(DWORD, nECX, ECX);
	Debug::Log(__FUNCTION__" Executed ! \n");

	if (auto nData = (DWORD)TlsGetValue(Phobos::PhobosTlsThread_2))
	{
		if (auto nEightTeen = *reinterpret_cast<DWORD*>(nData + 0x18))
		{
			if (auto nTen = *reinterpret_cast<DWORD*>(nData + 0x10))
			{
				*reinterpret_cast<DWORD*>(nData + 0x10) = nTen + nECX * nEightTeen;
			}
		}
	}

	return 0;
}

DEFINE_HOOK_AGAIN(0x4378FC , DSurface_BlitWithPlain_Add, 0x6)
DEFINE_HOOK(0x4377E5, DSurface_BlitWithPlain_Add, 0x8)
{
	Debug::Log(__FUNCTION__" Executed ! \n");

	if (auto nData = (DWORD)(TlsGetValue(Phobos::PhobosTlsThread_2)))
	{
		if (auto nEightTeen = *reinterpret_cast<DWORD*>(nData + 0x6))
		{
			if (auto nTen = *reinterpret_cast<DWORD*>(nData + 0x4))
			{
				*reinterpret_cast<DWORD*>(nData + 0x4) = nTen + nEightTeen;
			}
		}
	}

	return 0;

}

DEFINE_HOOK(0x437D51, DSurface_BlitWIthRLE_AdjustHeight, 0x6)
{
	Debug::Log(__FUNCTION__" Executed ! \n");

	GET_STACK(DWORD, n90, 0x90);
	GET_STACK(int, n28, 0x28);

	auto nVAl = Math::min(n28,0);
	if (TlsGetValue(Phobos::PhobosTlsThread_2))
	{
		if (n90)
		{
			if (char * nAVal = reinterpret_cast<char*>(n90 + 0x14))
			{
				if (auto nBVal = *reinterpret_cast<DWORD*>(n90 + 0x18))
				{
					*reinterpret_cast<DWORD*>(n90 + 0x10) = nAVal[nBVal * nVAl];
				}
			}
		}
	}

	return 0;
}
