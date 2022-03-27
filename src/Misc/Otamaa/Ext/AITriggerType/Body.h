#pragma once
#include <AITriggerTypeClass.h>

#include <Helpers/Macro.h>
#include <Utilities/Container.h>
#include <Utilities/TemplateDef.h>

class AITriggerTypeExt
{
public:
	using base_type = AITriggerTypeClass;

	class ExtData final : public Extension<AITriggerTypeClass>
	{
	public:

		ExtData(AITriggerTypeClass* OwnerObject) : Extension<AITriggerTypeClass>(OwnerObject)
		{ }

		virtual ~ExtData() = default;
		virtual size_t Size() const { return sizeof(*this); }
		virtual void InvalidatePointer(void *ptr, bool bRemoved) override {}
		virtual void LoadFromStream(PhobosStreamReader& Stm)override;
		virtual void SaveToStream(PhobosStreamWriter& Stm)override;

	private:
		template <typename T>
		void Serialize(T& Stm);
	};

	class ExtContainer final : public Container<AITriggerTypeExt>
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