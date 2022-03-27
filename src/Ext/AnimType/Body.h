#pragma once

#include <AnimTypeClass.h>

#include <Utilities/Container.h>
#include <Utilities/Enum.h>
#include <Utilities/Constructs.h>
#include <Utilities/Template.h>

#include <Misc/Otamaa/Ext/AnimType/Body.h>

class AnimTypeExt
{
public:
	using base_type = AnimTypeClass;

	class ExtData final : public Extension<AnimTypeClass>
	{
	public:
		CustomPalette Palette;
		Valueable<UnitTypeClass*> CreateUnit;
		Valueable<unsigned short> CreateUnit_Facing;
		Valueable<bool> CreateUnit_InheritDeathFacings;
		Valueable<bool> CreateUnit_InheritTurretFacings;
		Valueable<bool> CreateUnit_RemapAnim;
		Valueable<bool> CreateUnit_RandomFacing;
		Valueable<Mission> CreateUnit_Mission;
		Valueable<OwnerHouseKind> CreateUnit_Owner;
		Valueable<bool> CreateUnit_ConsiderPathfinding;
		Valueable<int> XDrawOffset;
		Valueable<int> HideIfNoOre_Threshold;
		Nullable<bool> Layer_UseObjectLayer;
		Valueable<bool> UseCenterCoordsIfAttached;

		Otamaa::ATExt::ExtData AnotherData;

		Valueable<int> DamageDelay;
		ExtData(AnimTypeClass* OwnerObject) : Extension<AnimTypeClass>(OwnerObject)
			, Palette { CustomPalette::PaletteMode::Temperate }
			, CreateUnit_Facing { 0 }
			, CreateUnit_RandomFacing { true }
			, CreateUnit_InheritDeathFacings { false }
			, CreateUnit_InheritTurretFacings { false }
			, CreateUnit_RemapAnim { false }
			, CreateUnit_Mission { Mission::Guard }
			, CreateUnit_Owner { OwnerHouseKind::Victim }
			, CreateUnit_ConsiderPathfinding { false }
			, XDrawOffset { 0 }
			, HideIfNoOre_Threshold { 0 }
			, Layer_UseObjectLayer {}
			, UseCenterCoordsIfAttached { false }
			, AnotherData { }
			, DamageDelay { 0 }
		{ }

		virtual ~ExtData() = default;

		virtual void LoadFromINIFile(CCINIClass* pINI) override;
		virtual size_t Size() const { return sizeof(*this); }
		virtual void InvalidatePointer(void* ptr, bool bRemoved) override { }

		virtual void InitializeConstants() override { }
		virtual void LoadFromStream(PhobosStreamReader& Stm) override;
		virtual void SaveToStream(PhobosStreamWriter& Stm) override;

	private:
		template <typename T>
		void Serialize(T& Stm);
	};

	class ExtContainer final : public Container<AnimTypeExt>
	{
	public:
		ExtContainer();
		~ExtContainer();
		virtual void InvalidatePointer(void* ptr, bool bRemoved) override;
	};

	static ExtContainer ExtMap;

	static const void ProcessDestroyAnims(UnitClass* pThis, TechnoClass* pKiller = nullptr);
	static bool LoadGlobals(PhobosStreamReader& Stm);
	static bool SaveGlobals(PhobosStreamWriter& Stm);
};
