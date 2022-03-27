#pragma once
#include <WarheadTypeClass.h>

#include <Helpers/Macro.h>
#include <Utilities/Container.h>
#include <Utilities/TemplateDef.h>
#include <New/Type/ShieldTypeClass.h>

#include <Misc/Otamaa/Ext/WarheadType/Body.h>

class WarheadTypeExt
{
public:
	using base_type = WarheadTypeClass;

	class ExtData final : public Extension<WarheadTypeClass>
	{
	public:

		Valueable<bool> SpySat;
		Valueable<bool> BigGap;
		Valueable<int> TransactMoney;
		ValueableVector<AnimTypeClass*> SplashList;
		Valueable<bool> SplashList_PickRandom;
		Valueable<bool> RemoveDisguise;
		Valueable<bool> RemoveMindControl;
		Valueable<bool> AnimList_PickRandom;
		Valueable<bool> DecloakDamagedTargets;

		Valueable<double> Crit_Chance;
		Valueable<bool> Crit_ApplyChancePerTarget;
		Valueable<int> Crit_ExtraDamage;
		Nullable<WarheadTypeClass*> Crit_Warhead;
		Valueable<AffectedTarget> Crit_Affects;
		ValueableVector<AnimTypeClass*> Crit_AnimList;
		Nullable<bool> Crit_AnimList_PickRandom;
		Valueable<bool> Crit_AnimOnAffectedTargets;
		Valueable<double> Crit_AffectBelowPercent;

		Nullable<AnimTypeClass*> MindControl_Anim;

		// Ares tags
		// http://ares-developers.github.io/Ares-docs/new/warheads/general.html
		Valueable<bool> AffectsEnemies;
		Nullable<bool> AffectsOwner;

		Valueable<bool> Shield_Penetrate;
		Valueable<bool> Shield_Break;
		Nullable<AnimTypeClass*> Shield_BreakAnim;
		Nullable<AnimTypeClass*> Shield_HitAnim;
		Nullable<WeaponTypeClass*> Shield_BreakWeapon;

		double RandomBuffer;
		bool HasCrit;

		Valueable<int> NotHuman_DeathSequence;

		Nullable<double> Shield_AbsorbPercent;
		Nullable<double> Shield_PassPercent;

		Valueable<int> Shield_Respawn_Duration;
		Valueable<double> Shield_Respawn_Amount;
		Valueable<int> Shield_Respawn_Rate;
		Valueable<bool> Shield_Respawn_ResetTimer;
		Valueable<int> Shield_SelfHealing_Duration;
		Nullable<double> Shield_SelfHealing_Amount;
		Valueable<int> Shield_SelfHealing_Rate;
		Valueable<bool> Shield_SelfHealing_ResetTimer;

		ValueableVector<ShieldTypeClass*> Shield_AttachTypes;
		ValueableVector<ShieldTypeClass*> Shield_RemoveTypes;
		Valueable<bool> Shield_ReplaceOnly;
		Valueable<bool> Shield_ReplaceNonRespawning;
		Valueable<bool> Shield_InheritStateOnReplace;
		ValueableVector<ShieldTypeClass*> Shield_AffectTypes;

		Valueable<int> Shield_MinimumReplaceDelay;

		Valueable<bool> Transact;
		Valueable<int> Transact_Experience_Value;
		Valueable<int> Transact_Experience_Source_Flat;
		Valueable<double> Transact_Experience_Source_Percent;
		Valueable<bool> Transact_Experience_Source_Percent_CalcFromTarget;
		Valueable<int> Transact_Experience_Target_Flat;
		Valueable<double> Transact_Experience_Target_Percent;
		Valueable<bool> Transact_Experience_Target_Percent_CalcFromSource;
		Valueable<bool> Transact_SpreadAmongTargets;

		Valueable<bool> ShakeIsLocal;

		Otamaa::WHExt::ExtData  AnotherData;


	private:
		Valueable<double> Shield_Respawn_Rate_InMinutes;
		Valueable<double> Shield_SelfHealing_Rate_InMinutes;

	public:
		ExtData(WarheadTypeClass* OwnerObject) : Extension<WarheadTypeClass>(OwnerObject)
			, SpySat { false }
			, BigGap { false }
			, TransactMoney { 0 }
			, SplashList {}
			, SplashList_PickRandom { false }
			, RemoveDisguise { false }
			, RemoveMindControl { false }
			, AnimList_PickRandom { false }
			, DecloakDamagedTargets { true }

			, Crit_Chance { 0.0 }
			, Crit_ApplyChancePerTarget { false }
			, Crit_ExtraDamage { 0 }
			, Crit_Warhead {}
			, Crit_Affects { AffectedTarget::All }
			, Crit_AnimList {}
			, Crit_AnimList_PickRandom {}
			, Crit_AnimOnAffectedTargets { false }
			, Crit_AffectBelowPercent { 1.0 }
			, RandomBuffer { 0.0 }
			, HasCrit { false }

			, MindControl_Anim {}

			, AffectsEnemies { true }
			, AffectsOwner {}

			, Shield_Penetrate { false }
			, Shield_Break { false }
			, Shield_BreakAnim {}
			, Shield_HitAnim {}
			, Shield_BreakWeapon {}
			, Shield_AbsorbPercent {}
			, Shield_PassPercent {}
			, Shield_MinimumReplaceDelay { 0 }
			, Shield_Respawn_Duration { 0 }
			, Shield_Respawn_Amount { 0.0 }
			, Shield_Respawn_Rate { -1 }
			, Shield_Respawn_Rate_InMinutes { -1.0 }
			, Shield_Respawn_ResetTimer { false }
			, Shield_SelfHealing_Duration { 0 }
			, Shield_SelfHealing_Amount { }
			, Shield_SelfHealing_Rate { -1 }
			, Shield_SelfHealing_Rate_InMinutes { -1.0 }
			, Shield_SelfHealing_ResetTimer { false }
			, Shield_AttachTypes {}
			, Shield_RemoveTypes {}
			, Shield_ReplaceOnly { false }
			, Shield_ReplaceNonRespawning { false }
			, Shield_InheritStateOnReplace { false }
			, Shield_AffectTypes {}

			, NotHuman_DeathSequence { -1 }


			, Transact { false }
			, Transact_Experience_Value { 1 }
			, Transact_Experience_Source_Flat { 0 }
			, Transact_Experience_Source_Percent { 0.0 }
			, Transact_Experience_Source_Percent_CalcFromTarget { false }
			, Transact_Experience_Target_Flat { 0 }
			, Transact_Experience_Target_Percent { 0.0 }
			, Transact_Experience_Target_Percent_CalcFromSource { false }
			, Transact_SpreadAmongTargets { false }

			, ShakeIsLocal { false }

			, AnotherData { }
		{ }

	private:
		void DetonateOnOneUnit(HouseClass* pHouse, TechnoClass* pTarget, TechnoClass* pOwner = nullptr);
		void DetonateOnCell(HouseClass* pHouse, CellClass* pTarget, TechnoClass* pOwner = nullptr);

		void ApplyRemoveDisguiseToInf(HouseClass* pHouse, TechnoClass* pTarget);
		void ApplyRemoveMindControl(HouseClass* pHouse, TechnoClass* pTarget);
		void ApplyCrit(HouseClass* pHouse, AbstractClass* pTarget, TechnoClass* Owner);
		void ApplyShieldModifiers(TechnoClass* pTarget);

		void DetonateOnAllUnits(HouseClass* pHouse, const CoordStruct coords, const float cellSpread, TechnoClass* pOwner);
		void TransactOnOneUnit(TechnoClass* pTarget, TechnoClass* pOwner, int targets);
		void TransactOnAllUnits(HouseClass* pHouse, const CoordStruct coords, const float cellSpread, TechnoClass* pOwner);
		int TransactGetValue(TechnoClass* pTarget, TechnoClass* pOwner, int flat, double percent, boolean calcFromTarget, int targetValue, int ownerValue);
		std::vector<std::vector<int>> TransactGetSourceAndTarget(TechnoClass* pTarget, TechnoTypeClass* pTargetType, TechnoClass* pOwner, TechnoTypeClass* pOwnerType, int targets);
		int TransactOneValue(TechnoClass* pTechno, TechnoTypeClass* pTechnoType, int transactValue, TransactValueType valueType);

	public:
		void Detonate(TechnoClass* pOwner, HouseClass* pHouse, BulletClass* pBullet, CoordStruct coords);
		bool CanTargetHouse(HouseClass* pHouse, TechnoClass* pTechno);

		virtual ~ExtData() = default;
		virtual void LoadFromINIFile(CCINIClass* pINI) override;
		virtual void InvalidatePointer(void* ptr, bool bRemoved) override { }
		virtual void LoadFromStream(PhobosStreamReader& Stm) override;
		virtual void Initialize() override;
		virtual void SaveToStream(PhobosStreamWriter& Stm) override;

		virtual size_t Size() const { return sizeof(*this); }
	private:
		template <typename T>
		void Serialize(T& Stm);
	};

	class ExtContainer final : public Container<WarheadTypeExt>
	{
	public:
		ExtContainer();
		~ExtContainer();

		virtual void InvalidatePointer(void* ptr, bool bRemoved) override;
	};

	static ExtContainer ExtMap;
	static WarheadTypeClass* Temporal_WH;
	static bool LoadGlobals(PhobosStreamReader& Stm);
	static bool SaveGlobals(PhobosStreamWriter& Stm);

	static void DetonateAt(WarheadTypeClass* pThis, ObjectClass* pTarget, TechnoClass* pOwner, int damage);
	static void DetonateAt(WarheadTypeClass* pThis, const CoordStruct& coords, TechnoClass* pOwner, int damage);

};
