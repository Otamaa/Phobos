#include "Body.h"

#include <Ext/TechnoType/Body.h>
#include <Ext/Techno/Body.h>
#include <Ext/Bullet/Body.h>
#include <Ext/BulletType/Body.h>

PhobosMap<LineTrailExt*, CoordStruct> LineTrailExt::LineTrailMap;

bool LineTrailExt::_DeallocateLineTrail(FootClass* pTech)
{
	if (auto const pTechExt = TechnoExt::ExtMap.Find(pTech))
	{
		if (!pTechExt->TechnoLineTrail.empty())
		{
			for (auto& pItems : pTechExt->TechnoLineTrail)
			{
				if (pItems)
				{
					GameDelete(pItems);
					return pTechExt->TechnoLineTrail.Remove(pItems);
				}
			}
		}
	}

	return false;
}

bool LineTrailExt::_DeallocateLineTrail(BulletClass* pBullet)
{
	if (auto const pBulletExt = BulletExt::ExtMap.Find(pBullet))
	{
		if (!pBulletExt->BulletTrails.empty())
		{
			for (auto& pItems : pBulletExt->BulletTrails)
			{
				if (pItems)
				{
					GameDelete(pItems);
					return pBulletExt->BulletTrails.Remove(pItems);
				}
			}
		}
	}

	return false;
}

void LineTrailExt::_ClearPhobosMap()
{
	Debug::Log("Clearing LineTrail Map ! \n");
	LineTrailMap.clear();
}

bool LineTrailExt::_DeallocateLineTrail(ObjectClass* pObject)
{
	if (auto pFoot = abstract_cast<FootClass*>(pObject))
		return LineTrailExt::_DeallocateLineTrail(pFoot);

	if (auto pBullet = abstract_cast<BulletClass*>(pObject))
		return LineTrailExt::_DeallocateLineTrail(pBullet);

	return false;
}

void LineTrailExt::_DetachLineTrail(ObjectClass* pThis)
{
	LineTrailExt::_DeallocateLineTrail(pThis);

	//also handle vanilla LinTrail if its there
	if (pThis->LineTrailer)
		GameDelete(pThis->LineTrailer);

}

//Override Color 0x5F51A3
ColorStruct LineTrailExt::_OverrideColor(ColorStruct& nColorInput)
{
	auto nColorOverrider = RulesClass::Instance->LineTrailColorOverride;

	ColorStruct nColorBuffer { 0,0,0 };

	nColorBuffer.R = nColorOverrider.R ? nColorOverrider.R : nColorInput.R;
	nColorBuffer.G = nColorOverrider.G ? nColorOverrider.G : nColorInput.G;
	nColorBuffer.B = nColorOverrider.B ? nColorOverrider.B : nColorInput.B;

	return nColorBuffer;
}

bool LineTrailExt::_CreateLineTrail(DynamicVectorClass<LineTrail*>& nVec, ObjectClass* pOwner, ColorStruct nColor, int nDecrement, CoordStruct nFLH)
{
	bool created = false;

	if (auto pLineTrail = static_cast<LineTrailExt*>(GameCreate<LineTrail>()))
	{
		created = pLineTrail;
		LineTrailExt::LineTrailMap.insert(pLineTrail, nFLH);
		pLineTrail->Color = LineTrailExt::_OverrideColor(nColor);
		pLineTrail->SetDecrement(nDecrement);
		pLineTrail->Owner = pOwner;
		nVec.AddUnique(pLineTrail);
	}

	return created;
}

bool LineTrailExt::_ConstructLineTrail(ObjectClass* pThis)
{
	if (auto pFoot = abstract_cast<FootClass*>(pThis))
	{
		auto const pTechExt = TechnoExt::ExtMap.Find(pFoot);
		auto const pType = pFoot->Type();
		auto const pTypeExt = TechnoTypeExt::ExtMap.Find(pType);

		if (pTechExt && pTypeExt)
		{
			if (!pTypeExt->LineTrailData.empty())
			{
				for (auto const& pTraildata : pTypeExt->LineTrailData)
				{
					LineTrailExt::_CreateLineTrail(
						pTechExt->TechnoLineTrail,
						pFoot, pTraildata.LineTrailColor,
						pTraildata.LineTrailColorDecrement,
						pTraildata.LineTrailFLH);
				}

				return true;
			}
		}
	}

	if (auto pBullet = abstract_cast<BulletClass*>(pThis))
	{
		auto const pBulletExt = BulletExt::ExtMap.Find(pBullet);
		auto const pType = pBullet->Type;
		auto const pTypeExt = BulletTypeExt::ExtMap.Find(pType);

		if (pBulletExt && pTypeExt)
		{
			if (!pTypeExt->LineTrailData.empty())
			{
				for (auto const& pTraildata : pTypeExt->LineTrailData)
				{
					LineTrailExt::_CreateLineTrail(
						pBulletExt->BulletTrails,
						pBullet, pTraildata.LineTrailColor,
						pTraildata.LineTrailColorDecrement,
						pTraildata.LineTrailFLH);
				}

				return true;
			}
		}
	}

	return false;
}

void LineTrailExt::_AnnounceInvalidPtr(DynamicVectorClass<LineTrail*>& nVec, void* ptr)
{
	if (!nVec.empty())
	{
		for (auto const& pItems : nVec)
		{
			if (pItems == ptr)
				nVec.Remove(pItems);
		}
	}
}
/*
void LineTrailExt::LoadFromStream(PhobosStreamReader& Stm)
{
	Stm
		.Process(Color)
		.Process(Owner)
		.Process(Decrement)
		.Process(ActiveSlot)
		.Process(Trails)
		;
}

void LineTrailExt::SaveToStream(PhobosStreamWriter& Stm)
{
	Stm
		.Process(Color)
		.Process(Owner)
		.Process(Decrement)
		.Process(ActiveSlot)
		.Process(Trails)
		;
}*/

bool LineTrailExt::LoadGlobals(PhobosStreamReader& Stm)
{
	return Stm
		.Success();
}

bool LineTrailExt::SaveGlobals(PhobosStreamWriter& Stm)
{
	return Stm
		.Success();
}

/*
DEFINE_HOOK(0x5F515D, ObjectClass_UnLimbo_ConstructLineTrail, 0x6)
{
	enum { ConstructVanilla = 0x5F5163, EndFunc = 0x5F5210 };
	GET(ObjectClass* const, pThis, ESI);

	//auto pFoot = abstract_cast<FootClass*>(pThis);
	//auto pBullet = abstract_cast<BulletClass*>(pThis);
	auto pTechno = abstract_cast<TechnoClass*>(pThis);

	if (pTechno)
		if (auto pCell = Map.TryGetCellAt(pTechno->Location))
			pCell->ActivateVeins();

	if (R->CL())
		return LineTrailExt::_ConstructLineTrail(pThis) ? EndFunc : ConstructVanilla;

	return EndFunc;
}

DEFINE_HOOK(0x556B39, LineTrail_DTOR, 0x9)
{
	GET(LineTrail*, pThis, ECX);
	GET(ObjectClass*, pObject, EAX);

	if (LineTrailExt::LineTrailMap.contains(static_cast<LineTrailExt*>(pThis)))
		LineTrailExt::LineTrailMap.erase(static_cast<LineTrailExt*>(pThis));

	pObject->LineTrailer = nullptr;
	pThis->Owner = nullptr;

	return 0x556B42;
}

DEFINE_HOOK(0x5F5284, ObjectClass_Detach_RemoveLineTrail, 0x8)
{
	GET(ObjectClass* const, pThis, ESI);

	LineTrailExt::_DeallocateLineTrail(pThis);

	return 0x5F529D;
}

DEFINE_HOOK(0x5F3D93, ObjectClass_Destroy_LineTrail, 0x8)
{
	GET(ObjectClass*, pThis, ESI);

	LineTrailExt::_DeallocateLineTrail(pThis);

	return 0x5F3DAC;
}

DEFINE_HOOK(0x556ADA, LineTrailClass_Destroy_Check, 0x9)
{
	GET(LineTrail*, pThis, ECX);
	GET(ObjectClass*, pObject, EAX);

	if (LineTrailExt::LineTrailMap.contains(static_cast<LineTrailExt*>(pThis)))
		LineTrailExt::LineTrailMap.erase(static_cast<LineTrailExt*>(pThis));

	pObject->LineTrailer = nullptr;
	pThis->Owner = nullptr;

	return 0x556AE3;
}

DEFINE_HOOK(0x556B7F, LineTrail_556B70_DrawCoords, 0x8)
{
	GET(LineTrail*, pThis, ECX);
	GET(CoordStruct const*, pCoord, EAX);

	auto nCoord = *pCoord;
	auto nOutput = LineTrailExt::LineTrailMap.get_or_default(static_cast<LineTrailExt*>(pThis), CoordStruct::Empty);
	nCoord += TechnoExt::GetFLHAbsoluteCoords(pThis->Owner,nOutput,false);

	R->EDX(nCoord.X);
	R->ESI(nCoord.Y);
	R->EDI(nCoord.Z);

	return 0x556B87;
}

//DEFINE_LJMP(0x5F5EED, 0x5F5EF7);

// Bugfix , line trail gone after game load !
// it will redrawn because game not saving the draw code stuffs
DEFINE_HOOK(0x5F3EDE, ObjectClass_AI_lineTrail, 0x6)
{
	GET(ObjectClass*, pThis, ESI);

	auto pType = pThis->GetType();
	if (pThis->WhatAmI() == AbstractType::Unit ||
		pThis->WhatAmI() == AbstractType::Infantry ||
		pThis->WhatAmI() == AbstractType::Anim ||
		pThis->WhatAmI() == AbstractType::Particle ||
		pThis->WhatAmI() == AbstractType::ParticleSystem ||
		pThis->WhatAmI() == AbstractType::Bullet ||
		pThis->WhatAmI() == AbstractType::Aircraft
		)
	{
		if (!pThis->LineTrailer && pType->UseLineTrail)
		{
			if (LineTrailExt::_ConstructLineTrail(pThis))
				return 0;
			else
			{
				if (auto pLineTrail = static_cast<LineTrailExt*>(GameCreate<LineTrail>()))
				{
					LineTrailExt::LineTrailMap.insert(pLineTrail, CoordStruct::Empty);
					pLineTrail->Color = LineTrailExt::_OverrideColor(pType->LineTrailColor);
					pLineTrail->SetDecrement(pType->LineTrailColorDecrement);
					pLineTrail->Owner = pThis;
					pThis->LineTrailer = pLineTrail;
				}
			}
		}
	}
	return 0x0;
}*/
/*
DEFINE_HOOK(0x5F3D4A, ObjectClass_DTOR_LineTrail, 0x7)
{
	GET(ObjectClass*, pThis, ESI);
//	GET(LineTrail*, pLine, ECX);
	Debug::Log(__FUNCTION__" Called! \n");
	LineTrailExt::DeallocateLineTrail(pThis);

	return 0;
}
*/