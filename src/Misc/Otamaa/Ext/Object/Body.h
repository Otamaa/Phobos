#pragma once
#include <ObjectClass.h>

#include <Utilities/Container.h>
#include <Utilities/Constructs.h>
#include <Utilities/Template.h>
#include <Helpers/Macro.h>

#include <Utilities/Debug.h>

class ObjectExt
{
public:
	using base_type = ObjectClass;

	class ExtData final : public Extension<ObjectClass>
	{
	public:
		// Nothing yet

		ExtData(ObjectClass* OwnerObject) : Extension<ObjectClass>(OwnerObject)
			// Nothing yet
		{ }

		virtual ~ExtData() = default;
		virtual size_t Size() const { return sizeof(*this); }
		virtual void InvalidatePointer(void* ptr, bool bRemoved) override {}

	};

	class ExtContainer final : public Container<ObjectExt> 
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
