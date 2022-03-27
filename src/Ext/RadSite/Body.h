#pragma once

#include <RadSiteClass.h>

#include <Helpers/Macro.h>
#include <Utilities/Container.h>
#include <Utilities/TemplateDef.h>

#include <Ext/WeaponType/Body.h>
#include <Phobos.h>

class RadTypeClass;

class RadSiteExt
{
public:

	using base_type = RadSiteClass;

	class ExtData final : public Extension<RadSiteClass>
	{
	public:
		Valueable<RadTypeClass*> Type;
		HouseClass* RadHouse;
		RepeatableTimerStruct ApplycationDelay_I;
		RepeatableTimerStruct ApplycationDelay_B;
		ExtData(RadSiteClass* OwnerObject) : Extension<RadSiteClass>(OwnerObject)
			, RadHouse { }
			, Type { }
			, ApplycationDelay_I { }
			, ApplycationDelay_B { }
		{ }

		virtual ~ExtData() = default;
		virtual size_t Size() const
		{
			return sizeof(*this);
		}

		virtual void InvalidatePointer(void* ptr, bool bRemoved)
		{
			AnnounceInvalidPointer(RadHouse, ptr);
		}

		virtual void LoadFromStream(PhobosStreamReader& Stm) override;
		virtual void SaveToStream(PhobosStreamWriter& Stm) override;
		virtual void InitializeConstants() override;

		void UpdateTimer();

	private:
		template <typename T>
		void Serialize(T& Stm);
	};

	static DynamicVectorClass<RadSiteExt::ExtData*> Array;
	static void CreateInstance(CellStruct location, int spread, int amount, WeaponTypeExt::ExtData* pWeaponExt, HouseClass* const pOwner);
	static void CreateLight(RadSiteClass* pThis);
	static void Add(RadSiteClass* pThis,int amount,HouseClass* pNewOwner);
	static void SetRadLevel(RadSiteClass* pThis,int amount);
	static double GetRadLevelAt(RadSiteClass* pThis,CellStruct const& cell);

	class ExtContainer final : public Container<RadSiteExt>
	{
	public:
		ExtContainer();
		~ExtContainer();
		virtual void InvalidatePointer(void* ptr, bool bRemoved) override;
	};

	static ExtContainer ExtMap;

	static bool LoadGlobals(PhobosStreamReader& Stm);
	static bool SaveGlobals(PhobosStreamWriter& Stm);
};