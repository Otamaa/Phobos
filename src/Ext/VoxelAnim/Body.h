#pragma once

#include <VoxelAnimClass.h>

#include <Utilities/Container.h>
#include <Utilities/Constructs.h>
#include <Utilities/Template.h>
#include <Utilities/TemplateDef.h>
#include <Utilities/Debug.h>

#include <Helpers/Macro.h>

#include <New/Entity/LaserTrailClass.h>
#include <Misc/Otamaa/Ext/VoxelAnim/Body.h>

class VoxelAnimExt
{
public:
	using base_type = VoxelAnimClass;

	class ExtData final : public Extension<VoxelAnimClass>
	{
	public:

		ValueableVector<std::unique_ptr<LaserTrailClass>> LaserTrails;

		Otamaa::VExt::ExtData AnotherData;
		ExtData(VoxelAnimClass* OwnerObject) : Extension<VoxelAnimClass>(OwnerObject)
			, LaserTrails()
			, AnotherData()
		{ }

		virtual ~ExtData() override
		{
			//TrailsManager::CleanUp(this->OwnerObject());
		}
		virtual size_t Size() const { return sizeof(*this); };
		virtual void InvalidatePointer(void *ptr, bool bRemoved) override {}
		virtual void LoadFromStream(PhobosStreamReader& Stm)override;
		virtual void SaveToStream(PhobosStreamWriter& Stm)override;
		virtual void Initialize() override;

	private:
		template <typename T>
		void Serialize(T& Stm);
	};

	class ExtContainer final : public Container<VoxelAnimExt>
	{
	public:
		ExtContainer();
		~ExtContainer();
		virtual void InvalidatePointer(void* ptr, bool bRemoved) override;
	};

	static ExtContainer ExtMap;
	static void InitializeLaserTrails(VoxelAnimClass* pThis);
	static bool LoadGlobals(PhobosStreamReader& Stm);
	static bool SaveGlobals(PhobosStreamWriter& Stm);
};
