#pragma once
#include <SuperWeaponTypeClass.h>

#include <Helpers/Macro.h>
#include <Utilities/Container.h>
#include <Utilities/TemplateDef.h>

#include <Misc/Otamaa/Ext/SWType/Body.h>

class SWTypeExt
{
public:
	using base_type = SuperWeaponTypeClass;

	class ExtData final : public Extension<SuperWeaponTypeClass>
	{
	public:

		Valueable<int> Money_Amount;
		Valueable<CSFText> UIDescription;
		Valueable<int> CameoPriority;
		ValueableVector<TechnoTypeClass*> LimboDelivery_Types;
		ValueableVector<int> LimboDelivery_IDs;
		ValueableVector<float> LimboDelivery_RollChances;
		Valueable<AffectedHouse> LimboKill_Affected;
		ValueableVector<int> LimboKill_IDs;
		Valueable<double> RandomBuffer;

		ValueableVector<ValueableVector<int>> LimboDelivery_RandomWeightsData;

		Otamaa::SWTExt::ExtData AnotherData;
		ExtData(SuperWeaponTypeClass* OwnerObject) : Extension<SuperWeaponTypeClass>(OwnerObject)
			, Money_Amount { 0 }
			, UIDescription {}
			, CameoPriority { 0 }
			, LimboDelivery_Types {}
			, LimboDelivery_IDs {}
			, LimboDelivery_RollChances {}
			, LimboDelivery_RandomWeightsData {}
			, LimboKill_Affected { AffectedHouse::Owner }
			, LimboKill_IDs {}
			, RandomBuffer { 0.0 }
			, AnotherData { }
		{ }


		void FireSuperWeapon(SuperClass* pSW, HouseClass* pHouse, CoordStruct coords);

		virtual void LoadFromINIFile(CCINIClass* pINI) override;
		virtual ~ExtData() = default;
		virtual size_t Size() const { return sizeof(*this); }
		virtual void InvalidatePointer(void* ptr, bool bRemoved) override { }

		virtual void LoadFromStream(PhobosStreamReader& Stm) override;

		virtual void SaveToStream(PhobosStreamWriter& Stm) override;
	private:
		void ApplyLimboDelivery(HouseClass* pHouse);
		void ApplyLimboKill(HouseClass* pHouse);

		template <typename T>
		void Serialize(T& Stm);
	};

	class ExtContainer final : public Container<SWTypeExt>
	{
	public:
		ExtContainer();
		~ExtContainer();
	};

	static ExtContainer ExtMap;
	static bool LoadGlobals(PhobosStreamReader& Stm);
	static bool SaveGlobals(PhobosStreamWriter& Stm);
};