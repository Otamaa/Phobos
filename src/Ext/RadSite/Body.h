#pragma once

#include <CCINIClass.h>
#include <RadSiteClass.h>
#include <WarheadTypeClass.h>

#include "../_Container.hpp"
#include "../../Enum/RadTypes.h"
#include "../../Phobos.h"
#include "../../Utilities/TemplateDef.h"
#include "../WeaponType/Body.h"

class RadSiteExt {
public:
	using base_type = RadSiteClass;

	class ExtData final : public Extension<RadSiteClass> 
	{
	public:
		Valueable<RadType*> Type;
		Nullable<unsigned> TypeIndex;
		Nullable<HouseClass*> Owner;

		ExtData(RadSiteClass* OwnerObject) : Extension<RadSiteClass>(OwnerObject),
			Type(nullptr),
			TypeIndex(0),
			Owner(nullptr)
			{ };

		virtual ~ExtData() { }

		virtual size_t Size() const { return sizeof(*this); };

		virtual void InvalidatePointer(void *ptr, bool bRemoved) { }
		virtual void LoadFromStream(IStream* Stm);
		virtual void SaveToStream(IStream* Stm);

	};
	static DynamicVectorClass<RadSiteExt::ExtData*> RadSiteInstance;

	static void CreateInstance(CellStruct location, int spread, int amount, WeaponTypeExt::ExtData *pWeaponExt , HouseClass* pOwner);
	static void RadSiteAdd(RadSiteClass* pRad, int lvmax, int amount);
	static void SetRadLevel(RadSiteClass* pRad, RadType* Type, int amount);
	static double GetRadLevelAt(RadSiteClass* pThis, CellStruct const& cell);
	//static void RadSiteClass_Radiate(RadSiteClass* pThis);
	

	class ExtContainer final : public Container<RadSiteExt> {
	public:
		ExtContainer();
		~ExtContainer();
	};
	static ExtContainer ExtMap;

};