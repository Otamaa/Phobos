#pragma once
#include <InfantryClass.h>

#include <Helpers/Macro.h>
#include <Utilities/Container.h>
#include <Utilities/TemplateDef.h>
#include <Utilities/Debug.h>

#include <BuildingClass.h>
#include <CellClass.h>

class InfantryExt
{
public:
	using base_type = InfantryClass;

	class ExtData final : public Extension<InfantryClass>
	{
	public:

		ExtData(InfantryClass* OwnerObject) : Extension<InfantryClass>(OwnerObject)
		{ }

		virtual ~ExtData() = default;
		virtual void InvalidatePointer(void *ptr, bool bRemoved) override {}
		virtual size_t Size() const { return sizeof(*this); }
		virtual void LoadFromStream(PhobosStreamReader& Stm) override;
		virtual void SaveToStream(PhobosStreamWriter& Stm) override;
		virtual void InitializeConstants() override;
	private:
		template <typename T>
		void Serialize(T& Stm);
	};

	class ExtContainer final : public Container<InfantryExt>
    {
	public:
		ExtContainer();
		~ExtContainer();
		virtual void InvalidatePointer(void* ptr, bool bRemoved) override;
	};

	static ExtContainer ExtMap;
	static bool IsOccupant(InfantryClass* thisTrooper); //!< Determines whether this InfantryClass is currently an occupant inside a BuildingClass.

	static bool LoadGlobals(PhobosStreamReader& Stm);
	static bool SaveGlobals(PhobosStreamWriter& Stm);
};