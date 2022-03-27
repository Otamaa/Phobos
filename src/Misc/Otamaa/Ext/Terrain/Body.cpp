#include "Body.h"
#include <Ext/TerrainType/Body.h>

template<> const DWORD Extension<TerrainClass>::Canary = 0xE1E2E3E4;
TerrainExt::ExtContainer TerrainExt::ExtMap;

void TerrainExt::ExtData::InitializeConstants() { }
/*Called when it needed to be*/
void TerrainExt::ExtData::InitializeLightSource()
{
	auto const TypeExt = TerrainTypeExt::ExtMap.Find(this->OwnerObject()->Type);

	if (!TypeExt || !TypeExt->AnotherData.LightIntensity.isset())
		return;

	if (!this->LighSource.get())
	{
		auto const nVisibility = TypeExt->AnotherData.LightVisibility.Get();

		if (!nVisibility)
			return;

		auto const Tint = TypeExt->AnotherData.GetLightTint();
		auto const Coords = this->OwnerObject()->GetCoords();

		if (auto light = GameCreate<LightSourceClass>(Coords, nVisibility, TypeExt->AnotherData.GetLightIntensity(), Tint))
		{
			this->LighSource.reset(light);
			light->Activate();
		}
	}
}

void TerrainExt::ExtData::InitializeAnim()
{
	auto const TypeExt = TerrainTypeExt::ExtMap.Find(this->OwnerObject()->Type);

	if (!TypeExt || TypeExt->AnotherData.AttachedAnim.empty())
		return;


	if (!AttachedAnim.get())
	{
		AnimTypeClass* pAnimType = nullptr;
		if (TypeExt->AnotherData.AttachedAnim.size() == 1)
			pAnimType = TypeExt->AnotherData.AttachedAnim.at(0);
		else
			pAnimType = TypeExt->AnotherData.AttachedAnim.at(ScenarioGlobal->Random(0, TypeExt->AnotherData.AttachedAnim.size() - 1));

		if (pAnimType)
		{
			auto const Coords = this->OwnerObject()->GetCoords();

			if (auto pAnim = GameCreate<AnimClass>(pAnimType, Coords))
			{
				pAnim->SetOwnerObject(this->OwnerObject());
				AttachedAnim.reset(pAnim);
			}
		}
	}
}

void TerrainExt::ExtData::ClearAnim()
{
	if (auto const pAnim = AttachedAnim.get())
	{
		pAnim->RemainingIterations = 0;
		pAnim->UnInit();
		AttachedAnim.release();
	}
}

//called when it Dtor ed , for more optimal
void TerrainExt::ExtData::ClearLightSource()
{
	if (auto const pLight = this->LighSource.get())
	{
		pLight->Deactivate();
		this->LighSource.release();
	}
}

//helper function(s)
/**/
void TerrainExt::Unlimbo(TerrainClass* pThis)
{
	if (!pThis)
		return;

	if (auto const TerrainExt = TerrainExt::ExtMap.Find(pThis))
	{
		TerrainExt->InitializeLightSource();
		TerrainExt->InitializeAnim();
	}

}

void TerrainExt::CleanUp(TerrainClass* pThis)
{
	if (!pThis)
		return;

	if (auto const TerrainExt = TerrainExt::ExtMap.Find(pThis))
	{
		TerrainExt->ClearLightSource();
		TerrainExt->ClearAnim();
	}
}

const void TerrainExt::DeallocateItemsAndRemove(TerrainClass* pThis)
{
	if (!pThis)
		return;

	if (auto const pExt = TerrainExt::ExtMap.Find(pThis))
	{
		pExt->ClearLightSource();
		pExt->ClearAnim();

		TerrainExt::ExtMap.Remove(pThis);
	}
}

// =============================
// load / save
template <typename T>
void TerrainExt::ExtData::Serialize(T& Stm)
{
	Stm
		.Process(this->LighSource)
		.Process(this->AttachedAnim)
		//.Process(this->AlreadyUpdated)
		;
}

void TerrainExt::ExtData::LoadFromStream(PhobosStreamReader& Stm)
{
	Extension<TerrainClass>::LoadFromStream(Stm);
	this->Serialize(Stm);
}

void TerrainExt::ExtData::SaveToStream(PhobosStreamWriter& Stm)
{
	Extension<TerrainClass>::SaveToStream(Stm);
	this->Serialize(Stm);
}

void TerrainExt::ExtContainer::InvalidatePointer(void* ptr, bool bRemoved) { }

bool TerrainExt::LoadGlobals(PhobosStreamReader& Stm)
{
	return Stm
		.Success();
}

bool TerrainExt::SaveGlobals(PhobosStreamWriter& Stm)
{
	return Stm
		.Success();
}

// =============================
// container

TerrainExt::ExtContainer::ExtContainer() : Container("TerrainClass") { }
TerrainExt::ExtContainer::~ExtContainer() = default;

// container hooks
DEFINE_HOOK_AGAIN(0x71BBF8, TerrainClass_CTOR, 0xD)
DEFINE_HOOK(0x71BE6D, TerrainClass_CTOR, 0xC)
{
	GET(TerrainClass*, pItem, ESI);

	TerrainExt::ExtMap.FindOrAllocate(pItem);
	return 0;
}

DEFINE_HOOK(0x71B7C9, TerrainClass_DTOR, 0xD)
{
	GET(TerrainClass*, pItem, ESI);

	TerrainExt::DeallocateItemsAndRemove(pItem);

	return 0;
}

DEFINE_HOOK_AGAIN(0x71CDA0, TerrainClass_SaveLoad_Prefix, 0x8)
DEFINE_HOOK(0x71CF30, TerrainClass_SaveLoad_Prefix, 0x8)
{
	GET_STACK(TerrainClass*, pItem, 0x4);
	GET_STACK(IStream*, pStm, 0x8);

	TerrainExt::ExtMap.PrepareStream(pItem, pStm);

	return 0;
}

DEFINE_HOOK(0x71CEAC, TerrainClass_Load_Suffix, 0x9)
{
	TerrainExt::ExtMap.LoadStatic();
	return 0;
}

DEFINE_HOOK(0x71CF44, TerrainClass_Save_Suffix, 0x5)
{
	TerrainExt::ExtMap.SaveStatic();
	return 0;
}