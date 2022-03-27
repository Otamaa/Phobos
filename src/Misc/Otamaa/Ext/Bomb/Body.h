#pragma once
#include <BombClass.h>

#include <Helpers/Macro.h>
#include <Utilities/Container.h>
#include <Utilities/TemplateDef.h>

class BombExt
{
public:
	using base_type = BombClass;

	class ExtData final : public Extension<BombClass>
	{
	public:

		ExtData(BombClass* OwnerObject) : Extension<BombClass>(OwnerObject)

		{ }

		virtual ~ExtData() = default;
		virtual size_t Size() const { return sizeof(*this); }
		virtual void InvalidatePointer(void* ptr, bool bRemoved) { }

		virtual void LoadFromStream(PhobosStreamReader& Stm) override;
		virtual void SaveToStream(PhobosStreamWriter& Stm) override;

		virtual void InitializeConstants() override { }
	};

	class ExtContainer final : public Container<BombExt>
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