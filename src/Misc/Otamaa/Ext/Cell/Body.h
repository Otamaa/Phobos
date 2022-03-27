#pragma once
#include <CellClass.h>

#include <Helpers/Macro.h>
#include <Utilities/Container.h>
#include <Utilities/TemplateDef.h>

//#include <Misc/FogOfWar/FogOfWar.h>
class CellExt
{
public:
	using base_type = CellClass;

	class ExtData final : public Extension<CellClass>
	{
	public:

		int NewPowerups;
		//std::vector<FoggedObject*> FoggedObjects;

	//	int Smudge_DecalsIdx; // this can be vector if you desire , but for testing purposes no!

		ExtData(CellClass* OwnerObject) : Extension<CellClass>(OwnerObject)
			, NewPowerups {-1}
		//	, Smudge_DecalsIdx(-1)
			//, FoggedObjects()
        { };

		virtual ~ExtData() = default;
		virtual size_t Size() const
		{
			return sizeof(*this);
		}

		virtual void InvalidatePointer(void* ptr, bool bRemoved) { }

		virtual void LoadFromStream(PhobosStreamReader& Stm) override;
		virtual void SaveToStream(PhobosStreamWriter& Stm) override;

		virtual void InitializeConstants() override { }

    };

	class ExtContainer final : public Container<CellExt>
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
/*
#include <Ext/_NewExt/Extension.h>

class CellExt
{
public:
	using base_type = CellClass;

	class ExtData final : public TExtension<base_type>
	{
	public:

		int NewPowerups;

		ExtData(base_type* OwnerObject) : TExtension<base_type>(OwnerObject)
			, NewPowerups { -1 }
		{ }

		virtual ~ExtData() = default;

		virtual size_t GetSize() const override final
		{ return sizeof(*this); }

		virtual HRESULT Load(IStream * pStm, AbstractClass * pOwner) override;
		virtual HRESULT Save(IStream * pStm) override;

		void InvalidatePointer(void* ptr, bool bRemoved) { }

		static CellExt::ExtData* GetExtData(base_type* pThis)
		{
			return reinterpret_cast<CellExt::ExtData*>
				(ExtensionWrapper::GetWrapper(pThis)->ExtensionObject);
		}

		void Init() { }
		void CleanUp() { }

	};
};*/