#pragma once
#include <BulletClass.h>

#include <Helpers/Macro.h>
#include <Utilities/Container.h>
#include <Utilities/TemplateDef.h>

#include <New/Entity/LaserTrailClass.h>
#include <Misc/Otamaa/Ext/Bullet/Body.h>

class BulletExt
{
public:
	using base_type = BulletClass;

	class ExtData final : public Extension<BulletClass>
	{
	public:
		Valueable<bool> Intercepted;
		Valueable<bool> ShouldIntercept;
		ValueableVector<std::unique_ptr<LaserTrailClass>> LaserTrails;

		Otamaa::BlExt::ExtData  AnotherData;
		ExtData(BulletClass* OwnerObject) : Extension<BulletClass>(OwnerObject)
			, Intercepted { false }
			, ShouldIntercept { false }
			, LaserTrails {}
			, AnotherData { }
		{ }

		virtual ~ExtData() override
		{
			//TrailsManager::CleanUp(this->OwnerObject());
		}
		virtual size_t Size() const { return sizeof(*this); }
		virtual void InvalidatePointer(void* ptr, bool bRemoved) override {}

		virtual void LoadFromStream(PhobosStreamReader& Stm) override;
		virtual void SaveToStream(PhobosStreamWriter& Stm) override;
		virtual void InitializeConstants() override;
		void ApplyRadiationToCell(CellStruct Cell, int Spread, int RadLevel);

	private:
		template <typename T>
		void Serialize(T& Stm);
	};

	class ExtContainer final : public Container<BulletExt> {
	public:
		ExtContainer();
		~ExtContainer();
		virtual void InvalidatePointer(void* ptr, bool bRemoved) override;
	};

	static void InitializeLaserTrails(BulletClass* pThis);
	static void UpdateOwner(BulletClass* pThis);
	static ExtContainer ExtMap;
	static bool LoadGlobals(PhobosStreamReader& Stm);
	static bool SaveGlobals(PhobosStreamWriter& Stm);
};
