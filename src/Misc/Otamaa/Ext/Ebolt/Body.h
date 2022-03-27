#pragma once

#include <EBolt.h>

#include <Helpers/Macro.h>
#include <Utilities/Container.h>
#include <Utilities/TemplateDef.h>

class EBoltExt
{
public:
	using base_type = EBolt;

	class ExtData final : public Extension<base_type>
	{
	public:

		ColorStruct Color_1;
		bool Disable_1;
		ColorStruct Color_2;
		bool Disable_2;
		ColorStruct Color_3;
		bool Disable_3;

		ExtData(base_type* OwnerObject) : Extension<base_type>(OwnerObject)
			, Color_1 { ColorStruct::Empty }
			, Disable_1 { false }
			, Color_2 { ColorStruct::Empty }
			, Disable_2 { false }
			, Color_3 { ColorStruct::Empty }
			, Disable_3 { false }
		{ }

		virtual ~ExtData() = default;

		virtual size_t Size() const
		{
			return sizeof(*this);
		}

		virtual void InvalidatePointer(void* ptr, bool bRemoved)
		{ }

		virtual void LoadFromStream(PhobosStreamReader& Stm) override;
		virtual void SaveToStream(PhobosStreamWriter& Stm) override;
		virtual void InitializeConstants() override;

		void CleanUp() { }

	private:
		template <typename T>
		void Serialize(T& Stm);
	};

	class ExtContainer final : public Container<EBoltExt>
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