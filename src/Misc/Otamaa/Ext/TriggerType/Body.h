#pragma once
#include<TriggerTypeClass.h>

#include <Utilities/Container.h>
#include <Utilities/Constructs.h>
#include <Utilities/Template.h>
#include <Utilities/TemplateDef.h>
#include <Utilities/Debug.h>
#include <Helpers/Macro.h>


class TriggerTypeExt
{
public:
	using base_type = TriggerTypeClass;

	class ExtData final : public Extension<TriggerTypeClass>
	{
	public:

		//int HouseIdx;
		ExtData(TriggerTypeClass* OwnerObject) : Extension<TriggerTypeClass>(OwnerObject)
		//	, HouseIdx(-1)
		{ }

		virtual ~ExtData() = default;
		virtual void InvalidatePointer(void *ptr, bool bRemoved) override {}
		virtual void LoadFromStream(PhobosStreamReader& Stm)override;
		virtual void SaveToStream(PhobosStreamWriter& Stm)override;
		virtual size_t Size() const { return sizeof(*this); }
	private:
		template <typename T>
		void Serialize(T& Stm);
	};

	class ExtContainer final : public Container<TriggerTypeExt>
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
