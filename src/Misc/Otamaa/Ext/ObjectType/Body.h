#pragma once
#include <ObjectTypeClass.h>

#include <Helpers/Macro.h>
#include <Utilities/Container.h>
#include <Utilities/TemplateDef.h>
class ObjectTypeExt
{
public:
	using base_type = ObjectTypeClass;
	class ExtData final : public Extension<ObjectTypeClass>
	{
	public:


		ExtData(ObjectTypeClass* OwnerObject) : Extension<ObjectTypeClass>(OwnerObject)

		{ }

		virtual ~ExtData() = default;
		virtual void LoadFromINIFile(CCINIClass* pINI) override;
		virtual void Initialize() override;
		virtual size_t Size() const { return sizeof(*this); }
		virtual void InvalidatePointer(void* ptr, bool bRemoved) override { }

		virtual void LoadFromStream(PhobosStreamReader& Stm) override;
		virtual void SaveToStream(PhobosStreamWriter& Stm) override;
		virtual void ClearExtensionDataAction() override { }

	private:
		template <typename T>
		void Serialize(T& Stm);
	};

	class ExtContainer final : public Container<ObjectTypeExt>
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