#include "Body.h"

#if 0
GET_REGISTER_STATIC(BlitTransXlatAlphaZRead *, this_ptr, ebp);
GET_REGISTER_STATIC(unsigned, src_pixel, eax);
GET_REGISTER_STATIC(short *, alpha_buff, ebp);
GET_REGISTER_STATIC(short *, alpha_ptr, esi);

static unsigned tint_color;
static unsigned pixel;

tint_color = DSurface::RGBA_To_Pixel(63, 0, 0);

static short *buff;
static short alpha_pixel;
buff = this_ptr->Buffer;
alpha_pixel = alpha_buff[*alpha_ptr];
pixel = buff[src_pixel | alpha_pixel];

if (tint_color) {
	pixel |= tint_color;
}

_asm { mov ecx, pixel }

JMP_REG(ecx, 0x00469AD9);
#endif

DEFINE_HOOK(0x4DAF85, FootClass_Draw_A_VXL_TintAdd, 0x5)
{
	//GET(FootClass*, pThis, EBX);
	//GET(int*, colorptr, ESP); //RGB565

	auto pColor = ColorStruct{ 128,255,255 };

	static constexpr reference<int, 0x8205D0u> const RGBMode{};

	Debug::Log("%d mode \n", RGBMode.get());

	int Res = 0;

	if (RGBMode <= 2)
	{
		if (RGBMode > 1)
		{
			Res = pColor.B | (32 * (pColor.G | (pColor.R << 6)));
		}
		Res |= pColor.B | (32 * ((pColor.R << 6) | pColor.G & 0xFFFE));
	}
	Res = Res | (pColor.G >> 1) | (32 * (pColor.B | (32 * pColor.R)));

	R->ESP<int*>(&Res);

	return 0;
}

DEFINE_HOOK(0x73C15F, UnitClass_DrawVXL_Tint, 0x7)
{
	GET(UnitClass *, Item, EBP);
	GET(int, nCurTint, ESI);

	if (Item->Berzerk || Item->IronCurtainTimer.GetTimeLeft() > 0)
		return 0;

	auto pColor = ColorStruct{ 128,255,255 };

	static constexpr reference<int, 0x8205D0u> const RGBMode{};

	Debug::Log("%d mode \n", RGBMode.get());

	int Res = 0;

	if (RGBMode <= 2)
	{
		if (RGBMode > 1)
		{
			Res = pColor.B | (32 * (pColor.G | (pColor.R << 6)));
		}
		Res |= pColor.B | (32 * ((pColor.R << 6) | pColor.G & 0xFFFE));
	}
	Res = Res | (pColor.G >> 1) | (32 * (pColor.B | (32 * pColor.R)));


	int nTint = nCurTint + Res;
	R->ESI(nTint);
	return 0;
}

DEFINE_HOOK(0x43D52D, BuildingClass_Draw_Tint, 0x5)
{
	GET(BuildingClass *, Item, ESI);
	GET(int, nCurTint, EDI);

	if (Item->Berzerk || Item->IronCurtainTimer.GetTimeLeft() > 0)
		return 0;

	auto pColor = ColorStruct{ 35,255,255 };

	static constexpr reference<int, 0x8205D0u> const RGBMode{};

	Debug::Log("%d mode \n", RGBMode.get());

	int Res = 0;

	if (RGBMode <= 2)
	{
		if (RGBMode > 1)
		{
			Res = pColor.B | (32 * (pColor.G | (pColor.R << 6)));
		}
		Res |= pColor.B | (32 * ((pColor.R << 6) | pColor.G & 0xFFFE));
	}
	Res = Res | (pColor.G >> 1) | (32 * (pColor.B | (32 * pColor.R)));


	int nTint = nCurTint + Res;

	R->EDI(nTint);
	return 0;
}

DEFINE_HOOK(0x5190C5, InfantryClass_Draw_Tint, 0x5)
{
	GET(InfantryClass *, Item, EBP);
	GET_STACK(int, nCurTint, 0x18);

	if (Item->Berzerk || Item->IronCurtainTimer.GetTimeLeft() > 0)
		return 0;

	auto pColor = ColorStruct{ 210,255,255 };

	static constexpr reference<int, 0x8205D0u> const RGBMode{};

	Debug::Log("%d mode \n", RGBMode.get());

	int Res = 0;

	if (RGBMode <= 2)
	{
		if (RGBMode > 1)
		{
			Res = pColor.B | (32 * (pColor.G | (pColor.R << 6)));
		}
		Res |= pColor.B | (32 * ((pColor.R << 6) | pColor.G & 0xFFFE));
	}
	Res = Res | (pColor.G >> 1) | (32 * (pColor.B | (32 * pColor.R)));


	int nTint = nCurTint + Res;
	R->Stack(0x18, nTint);

	return 0;
}

DEFINE_HOOK(0x518FCE, InfantryClass_Draw_Berzerk, 0x6)
{
	GET(InfantryClass*, pThis, EBP);

	if (pThis->Berzerk)
		return 0x518FD4;

	auto pColor = ColorStruct{ 35,255,255 };

	if (pColor == ColorStruct{ 0,0,0 })
		return 0x519082;

	R->BX(pColor.G);
	R->Stack(0x12, pColor.R);
	R->Stack(0x13, pColor.B);

	return 0x519008;
}

DEFINE_HOOK(0x73C083, UnitClass_DrawAsVXL_Berzerk, 0x6)
{
	GET(UnitClass*, pThis, EBP);
	if (pThis->Berzerk)
		return 0x0;

	auto pColor = ColorStruct{ 0,255,255 };

	if (pColor == ColorStruct{ 0,0,0 })
		return 0x73C141;

	R->Stack(0x13, pColor.R);
	R->Stack(0x12, pColor.G);
	R->Stack(0x11, pColor.B);
	return 0x73C0CA;
}