#pragma once

#include <CCINIClass.h>
#include <HouseTypeClass.h>

#include <Helpers/Macro.h>
#include <Utilities/Container.h>
#include <Utilities/TemplateDef.h>

class AircraftTypeClass;
class HouseTypeExt
{
public:
	using base_type = HouseTypeClass;

	class ExtData final : public Extension<HouseTypeClass>
	{
	public:

		ExtData(HouseTypeClass* OwnerObject) : Extension<HouseTypeClass>(OwnerObject)
		{ }

		virtual ~ExtData() = default;
		virtual size_t Size() const { return sizeof(*this); }
		virtual void LoadFromINIFile(CCINIClass* pINI) override;
		virtual void LoadFromRulesFile(CCINIClass *pINI) override;
		virtual void InitializeConstants() override;
		virtual void Initialize() override;
		virtual void InvalidatePointer(void *ptr, bool bRemoved) override {	}

		virtual void LoadFromStream(PhobosStreamReader &Stm) override;
		virtual void SaveToStream(PhobosStreamWriter &Stm) override;


	private:
		template <typename T>
		void Serialize(T& Stm);
	};

	class ExtContainer final : public Container<HouseTypeExt>
	{
	public:
		ExtContainer();
		~ExtContainer();
		virtual void InvalidatePointer(void* ptr, bool bRemoved) override;
	};

	static bool LoadGlobals(PhobosStreamReader& Stm);
	static bool SaveGlobals(PhobosStreamWriter& Stm);

	static ExtContainer ExtMap;
};