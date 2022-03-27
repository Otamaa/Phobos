#pragma once
#include <TechnoClass.h>
#include <AnimClass.h>

#include <Helpers/Macro.h>
#include <Utilities/Container.h>
#include <Utilities/TemplateDef.h>

#include <New/Entity/ShieldClass.h>
#include <New/Entity/LaserTrailClass.h>

#include <Misc/Otamaa/Ext/Techno/Body.h>

class BulletClass;

class TechnoExt
{
public:
	using base_type = TechnoClass;

	class ExtData final : public Extension<TechnoClass>
	{
	public:
		Valueable<BulletClass*> InterceptedBullet;
		std::unique_ptr<ShieldClass> Shield;
		ValueableVector<std::unique_ptr<LaserTrailClass>> LaserTrails;
		Valueable<bool> ReceiveDamage;
		Valueable<bool> LastKillWasTeamTarget;
		TimerStruct	PassengerDeletionTimer;
		Valueable<int> PassengerDeletionCountDown;
		Valueable<ShieldTypeClass*> CurrentShieldType;
		Valueable<int> LastWarpDistance;
		int Death_Countdown;
		Valueable<AnimTypeClass*> MindControlRingAnimType;

		Otamaa::TTExt::ExtData AnotherData;
		bool DelayKill;
		ExtData(TechnoClass* OwnerObject) : Extension<TechnoClass>(OwnerObject)
			, InterceptedBullet { nullptr }
			, Shield {}
			, LaserTrails {}
			, ReceiveDamage { false }
			, LastKillWasTeamTarget { false }
			, PassengerDeletionTimer {}
			, PassengerDeletionCountDown { -1 }
			, CurrentShieldType { nullptr }
			, LastWarpDistance {}
			, Death_Countdown(-1)
			, MindControlRingAnimType { nullptr }
			, AnotherData { }
			, DelayKill { false  }
		{ }

		virtual ~ExtData() override
		{
			//TrailsManager::CleanUp(this->OwnerObject());
		}

		virtual void InvalidatePointer(void* ptr, bool bRemoved) override
		{
			if(this->GetShield())
				this->GetShield()->InvalidatePointer(ptr);
		}

		ShieldClass* GetShield() const
		{
			return this->Shield.get();
		}

		virtual size_t Size() const { return sizeof(*this); }
		virtual void LoadFromStream(PhobosStreamReader& Stm) override;
		virtual void SaveToStream(PhobosStreamWriter& Stm) override;
		virtual void InitializeConstants() override;

	private:
		template <typename T>
		void Serialize(T& Stm);
	};

	class ExtContainer final : public Container<TechnoExt>
	{
	public:
		ExtContainer();
		~ExtContainer();

		virtual void InvalidatePointer(void* ptr, bool bRemoved) override;
	};

	static ExtContainer ExtMap;

	static bool LoadGlobals(PhobosStreamReader& Stm);
	static bool SaveGlobals(PhobosStreamWriter& Stm);

	static bool IsActive(TechnoClass* pThis);
	static bool IsReallyAlive(TechnoClass* const pThis);
	static int GetSizeLeft(FootClass* const pThis);
	static void Stop(TechnoClass* pThis, Mission eMission = Mission::Guard);
	static bool IsHarvesting(TechnoClass* pThis);
	static bool HasAvailableDock(TechnoClass* pThis);

	static void InitializeItems(TechnoClass* pThis);
	static void InitializeLaserTrail(TechnoClass* pThis, bool bIsconverted);

	static CoordStruct GetFLHAbsoluteCoords(TechnoClass* pThis, CoordStruct flh, bool turretFLH = false , CoordStruct Overrider = CoordStruct::Empty);
	static CoordStruct GetBurstFLH(TechnoClass* pThis, int weaponIndex, bool& FLHFound);

	static void FireWeaponAtSelf(TechnoClass* pThis, WeaponTypeClass* pWeaponType);

	static void TransferMindControlOnDeploy(TechnoClass* pTechnoFrom, TechnoClass* pTechnoTo);

	static void ApplyMindControlRangeLimit(TechnoClass* pThis);
	static void ApplyInterceptor(TechnoClass* pThis);
	static void ApplyPowered_KillSpawns(TechnoClass* pThis);
	static void ApplySpawn_LimitRange(TechnoClass* pThis);
	static void CheckDeathConditions(TechnoClass* pThis);
	static void ObjectKilledBy(TechnoClass* pThis, TechnoClass* pKiller);
	static void EatPassengers(TechnoClass* pThis);
	static void UpdateSharedAmmo(TechnoClass* pThis);
	static void UpdateMindControlAnim(TechnoClass* pThis);
	static bool CanFireNoAmmoWeapon(TechnoClass* pThis, int weaponIndex);
	static void DrawSelfHealPips(TechnoClass* pThis, Point2D* pLocation, RectangleStruct* pBounds);
	static void ApplyGainedSelfHeal(TechnoClass* pThis);
};
