#include "Body.h"

template<> const DWORD Extension<ObjectTypeClass>::Canary = 0x11111111;
ObjectTypeExt::ExtContainer ObjectTypeExt::ExtMap;

void ObjectTypeExt::ExtData::Initialize()
{ }

void ObjectTypeExt::ExtData::LoadFromINIFile(CCINIClass* const pINI)
{
	auto pThis = this->OwnerObject();
	const char* pSection = pThis->ID;
	const char* pArtSection = pThis->ImageFile;

	if (!pINI->GetSection(pSection) || !pINI->GetSection(pArtSection))
		return;

	INI_EX exINI(pINI);
	INI_EX exArtINI(CCINIClass::INI_Art);

}

template <typename T>
void ObjectTypeExt::ExtData::Serialize(T& Stm)
{ 
	Stm

		;
}

void ObjectTypeExt::ExtData::LoadFromStream(PhobosStreamReader& Stm)
{
	Extension<ObjectTypeClass>::LoadFromStream(Stm);
	this->Serialize(Stm);
}

void ObjectTypeExt::ExtData::SaveToStream(PhobosStreamWriter& Stm)
{
	Extension<ObjectTypeClass>::SaveToStream(Stm);
	this->Serialize(Stm);
}

void ObjectTypeExt::ExtContainer::InvalidatePointer(void* ptr, bool bRemoved) { }

bool ObjectTypeExt::LoadGlobals(PhobosStreamReader& Stm)
{
	return Stm
		.Success();
}

bool ObjectTypeExt::SaveGlobals(PhobosStreamWriter& Stm)
{
	return Stm
		.Success();
}

ObjectTypeExt::ExtContainer::ExtContainer() : Container("ObjectTypeClass") { }
ObjectTypeExt::ExtContainer::~ExtContainer() = default;


DEFINE_HOOK(0x5F7249, ObjectTypeClass_CTOR ,0x8)
{
	GET(ObjectTypeClass*, pItem, EBP);
	//Debug::Log(__FUNCTION__" Called ![%x][%s] \n", pItem, pItem->get_ID());
	ObjectTypeExt::ExtMap.FindOrAllocate(pItem);

	return 0;
}

DEFINE_HOOK(0x5F732D, ObjectTypeClass_CTOR_noInt, 0x8)
{
	GET(ObjectTypeClass*, pItem, ESI);
	//Debug::Log(__FUNCTION__" Called ![%x][%s] \n", pItem, pItem->get_ID());
	ObjectTypeExt::ExtMap.FindOrAllocate(pItem);

	return 0;
}

DEFINE_HOOK(0x5F753E, ObjectTypeClass_DTOR, 0x9)
{
	GET(ObjectTypeClass*, pItem, ESI);
	//Debug::Log(__FUNCTION__" Called ![%x][%s] \n", pItem, pItem->get_ID());
	ObjectTypeExt::ExtMap.Remove(pItem);

	return 0;
}
//save
DEFINE_HOOK_AGAIN(0x5F9950, ObjectTypeClass_SaveLoad_Prefix, 0x8)
//load
DEFINE_HOOK(0x5F9720, ObjectTypeClass_SaveLoad_Prefix, 0x6)
{
	GET_STACK(ObjectTypeClass*, pItem, 0x4);
	GET_STACK(IStream*, pStm, 0x8);
	if (Phobos::Config::MoreDetailSLDebugLog)
	Debug::Log(__FUNCTION__" Called !\n");
	ObjectTypeExt::ExtMap.PrepareStream(pItem, pStm);

	return 0;
}

DEFINE_HOOK(0x5F993E, ObjectTypeClass_Load_Suffix, 0xA)
{
	ObjectTypeExt::ExtMap.LoadStatic();
	if (Phobos::Config::MoreDetailSLDebugLog)
	Debug::Log(__FUNCTION__" Called ! \n");
	return 0;
}

DEFINE_HOOK(0x5F996A, ObjectTypeClass_Save_Suffix, 0x5)
{
	ObjectTypeExt::ExtMap.SaveStatic();
	if (Phobos::Config::MoreDetailSLDebugLog)
	Debug::Log(__FUNCTION__" Called ! \n");
	return 0;
}

// all graphic name loaded , load our stuffs after
DEFINE_HOOK(0x5F9652, ObjectTypeClass_LoadFromINI, 0xC)
{
	GET(ObjectTypeClass*, pItem, EBX);
	GET_STACK(CCINIClass*, pINI, STACK_OFFS(0x1AC, -0x4));
	//Debug::Log(__FUNCTION__" Called ![%x][%s] \n", pItem, pItem->get_ID());

	ObjectTypeExt::ExtMap.LoadFromINI(pItem, pINI);

	return 0;
}