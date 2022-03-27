#pragma once

#include <TiberiumClass.h>
#include <CCINIClass.h>
#include <RulesClass.h>

#include <Utilities/Container.h>
#include <Utilities/Constructs.h>
#include <Utilities/Template.h>
#include <Utilities/TemplateDef.h>
#include <Utilities/Debug.h>
#include <Helpers/Macro.h>

class AnimTypeClass;
class TiberiumExt
{
public:
	using base_type = TiberiumClass;

	class ExtData final : public Extension<TiberiumClass>
	{
	public:

		Nullable<AnimTypeClass*> OreTwinkle;
		Nullable<int> OreTwinkleChance;
		Nullable<int> Ore_TintLevel;
		Nullable<ColorStruct> MinimapColor;

		ExtData(TiberiumClass* OwnerObject) : Extension<TiberiumClass>(OwnerObject)
			, OreTwinkle()
			, OreTwinkleChance()
			, Ore_TintLevel()
			, MinimapColor {}
		{ }

		virtual ~ExtData() = default;
		virtual size_t Size() const { return sizeof(*this); }
		virtual void LoadFromINIFile(CCINIClass* pINI) override;
		virtual void InvalidatePointer(void *ptr, bool bRemoved) override {}
		virtual void LoadFromStream(PhobosStreamReader& Stm) override;
		virtual void SaveToStream(PhobosStreamWriter& Stm) override;

		AnimTypeClass* GetTwinkleAnim() const
		{
			return this->OreTwinkle.Get(RulesClass::Instance->OreTwinkle);
		}

		int GetTwinkleChance() const
		{
			return this->OreTwinkleChance.Get(RulesClass::Instance->OreTwinkleChance);
		}

	private:
		template <typename T>
		void Serialize(T& Stm);
	};

	class ExtContainer final : public Container<TiberiumExt>
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
