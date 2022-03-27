#pragma once
#include <BulletTypeClass.h>

#include <Helpers/Macro.h>
#include <Utilities/Container.h>
#include <Utilities/TemplateDef.h>

#include <New/Type/LaserTrailTypeClass.h>
#include <Misc/Otamaa/Ext/BulletType/Body.h>

class BulletTypeExt
{
public:
	using base_type = BulletTypeClass;

	class ExtData final : public Extension<BulletTypeClass>
	{
	public:
		Valueable<bool> Interceptable;
		ValueableIdxVector<LaserTrailTypeClass> LaserTrail_Types;
		Nullable<double> Gravity;
		Valueable<bool> Gravity_HeightFix;
		Valueable<bool> Shrapnel_AffectsGround;
		Valueable<bool> Shrapnel_AffectsBuildings;

		Otamaa::BlTExt::ExtData AnotherData;
		ExtData(BulletTypeClass* OwnerObject) : Extension<BulletTypeClass>(OwnerObject)
			, Interceptable { false }
			, LaserTrail_Types {}
			, Gravity {}
			, Gravity_HeightFix { false }
			, Shrapnel_AffectsGround { false }
			, Shrapnel_AffectsBuildings { false }
			, AnotherData { }
		{ }

		virtual ~ExtData() = default;
		virtual size_t Size() const { return sizeof(*this); }
		virtual void LoadFromINIFile(CCINIClass* pINI) override;
		virtual void InitializeConstants() override { }

		virtual void InvalidatePointer(void* ptr, bool bRemoved) override { }

		virtual void LoadFromStream(PhobosStreamReader& Stm) override;
		virtual void SaveToStream(PhobosStreamWriter& Stm) override;

	private:
		template <typename T>
		void Serialize(T& Stm);
	};

	class ExtContainer final : public Container<BulletTypeExt> {
	public:
		ExtContainer();
		~ExtContainer();
		virtual void InvalidatePointer(void* ptr, bool bRemoved) override;
	};

	static ExtContainer ExtMap;
	static bool LoadGlobals(PhobosStreamReader& Stm);
	static bool SaveGlobals(PhobosStreamWriter& Stm);

	static double GetAdjustedGravity(BulletTypeClass* pType);
	static BulletTypeClass* GetDefaultBulletType();
};
