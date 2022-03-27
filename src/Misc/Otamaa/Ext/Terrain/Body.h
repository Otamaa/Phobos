#pragma once
#include <TerrainClass.h>

#include <Helpers/Macro.h>
#include <Utilities/Container.h>
#include <Utilities/TemplateDef.h>
#include <Utilities/Debug.h>

#include <LightSourceClass.h>
#include <CellClass.h>

class TerrainExt
{
public:
	using base_type = TerrainClass;

	class ExtData final : public Extension<TerrainClass>
	{
	public:

		UniqueGamePtr<LightSourceClass> LighSource;
		UniqueGamePtr<AnimClass> AttachedAnim;

	//	unsigned int CurrentShape;
	//	bool AlreadyUpdated;

		ExtData(TerrainClass* OwnerObject) : Extension<TerrainClass>(OwnerObject)
			, LighSource()
		//	, CurrentShape(0)
		//	, AlreadyUpdated(false)
			, AttachedAnim { }
		{ }

		virtual ~ExtData() = default;
		virtual size_t Size() const { return sizeof(*this); };
		virtual void InvalidatePointer(void *ptr, bool bRemoved) override
		{
			if (LighSource.get() && (void*)LighSource.get() == ptr)
				LighSource.release();

			if (AttachedAnim.get() && (void*)AttachedAnim.get() == ptr)
				AttachedAnim.release();
		}

		virtual void LoadFromStream(PhobosStreamReader& Stm)override;
		virtual void SaveToStream(PhobosStreamWriter& Stm)override;
		virtual void InitializeConstants() override;
		void InitializeLightSource();
		void InitializeAnim();
		void ClearLightSource();
		void ClearAnim();

	private:
		template <typename T>
		void Serialize(T& Stm);

	};

	class ExtContainer final : public Container<TerrainExt>
	{
	public:
		ExtContainer();
		~ExtContainer();
		virtual void InvalidatePointer(void* ptr, bool bRemoved) override;
	};

	static ExtContainer ExtMap;

	static void Unlimbo(TerrainClass* pThis);
	static void CleanUp(TerrainClass* pThis);

	static const void DeallocateItemsAndRemove(TerrainClass* pThis);

	static bool LoadGlobals(PhobosStreamReader& Stm);
	static bool SaveGlobals(PhobosStreamWriter& Stm);

};