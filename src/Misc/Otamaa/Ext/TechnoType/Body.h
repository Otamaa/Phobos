#include <Utilities/TemplateDef.h>

#include <Misc/Otamaa/New/Type/HoverTypeClass.h>
#include <Misc/Otamaa/Misc/DynamicPatcher/Techno/CrawlFLHData.h>
//#include <Misc/Otamaa/Misc/DynamicPatcher/Trails/TrailsManager.h>
//#include <Misc/Otamaa/Misc/DynamicPatcher/Techno/FireSW/FireSW.h>
//#include <Misc/Otamaa/Misc/DynamicPatcher/Techno/SpawnSupport/SpawnSupportData.h>
//#include <Misc/Otamaa/Misc/DynamicPatcher/Techno/Passengers/Passengers.h>
//#include <Misc/Otamaa/Misc/DynamicPatcher/Techno/JumjetFaceTarget/JJFacingData.h>
#include <Misc/Otamaa/Misc/DynamicPatcher/Techno/GiftBox/GiftBoxData.h>
//#include <Misc/Otamaa/Misc/DynamicPatcher/Techno/AircraftDive/AircraftDiveData.h>
//#include <Misc/Otamaa/Misc/DynamicPatcher/Techno/AircraftPut/AircraftPutData.h>

#include <vector>

class AnimTypeClass;
class ParticleSystemTypeClass;
class WeaponTypeClass;
namespace Otamaa
{
	struct TTyExt
	{
		struct ExtDataB
		{
			Valueable<bool> DontShake;

			Valueable<int> CrateType; //unused

			NullableIdx<VocClass>DiskLaserChargeUp;
			Nullable<AnimTypeClass*>DrainAnimationType;

			NullableIdx<VoxClass>SpyplaneCameraSound;

			Valueable<bool> Is_Cow;

			Nullable<int> ParadropRadius;
			Nullable<int> ParadropOverflRadius;
			Valueable<bool> Paradrop_DropPassangers;
			Nullable<BYTE> Paradrop_MaxAttempt;

			Valueable<double> CrashSpinLevelRate;
			Valueable<double> CrashSpinVerticalRate;

			Nullable<float> TalkBubbleTime;

			Valueable<bool> IsCustomMissile;
			Valueable<RocketStruct> CustomMissileData;
			Valueable<bool> CustomMissileEliteRaise;
			Valueable<bool> CustomMissileRaise;

			//Health data
			Nullable<ShapeHandlerEnumerator*> HealthBarSHP;
			Valueable<Point2D> HealthBarSHP_PointOffset;
			Nullable<ShapeHandlerEnumerator*> HealthBarSHP_Selected;
			Valueable<int> HealthBarSHPBracketOffset;
			Valueable<CoordStruct> HealthBarSHP_HealthFrame;
			CustomPalette HealthBarSHP_Palette;
			Valueable<bool> HealthbarRemap;

			//pip
			Nullable<ShapeHandlerEnumerator*> PipShapes01;
			Nullable<ShapeHandlerEnumerator*> PipShapes02;
			Nullable<ShapeHandlerEnumerator*> PipGarrison;
			Valueable<int> PipGarrison_FrameIndex;
			CustomPalette PipGarrison_Palette;

			Nullable<ShapeHandlerEnumerator*> HealthNumber_SHP;
			Valueable<bool> HealthNumber_Show;
			Valueable<bool> HealthNumber_Percent;
			Nullable<Point2D> Healnumber_Offset;
			Nullable<Point2D> Healnumber_Decrement;

			ValueableIdx<VoxClass> ParasiteExit_Sound;

			Nullable <int> AttackingAircraftSightRange;

			Valueable<bool> FacingRotation_Disable;
			Valueable<bool> FacingRotation_DisalbeOnEMP;
			Valueable<bool> FacingRotation_DisalbeOnDeactivated;

			Nullable<int> JumpJet_HeightOffset;

			NullableVector<int> Overload_Count;
			NullableVector<int> Overload_Damage;
			NullableVector<int> Overload_Frames;
			NullableIdx<VocClass> Overload_DeathSound;
			Nullable<ParticleSystemTypeClass*> Overload_ParticleSys;
			Nullable<int> Overload_ParticleSysCount;

			Valueable<AnimTypeClass*> Landing_Anim;
			Valueable<AnimTypeClass*> Landing_AnimOnWater;

			std::vector<CoordStruct> HitCoordOffset;
			Valueable<bool> HitCoordOffset_Random;

			Valueable<bool> Draw_MindControlLink;

			ValueableIdx<HoverTypeClass> HoverLocoIdx;
			Valueable<WeaponTypeClass*> CrashWeapon;
			Valueable<bool> Disable_C4WarheadExp;
			CrawlingFLHData CrawlFLHData;
			Valueable<bool> Cloak_WhenMove;

			Nullable<ShapeHandlerEnumerator*> GClock_Shape;
			Nullable<int> GClock_Transculency;
			CustomPalette GClock_Palette;

			ValueableVector<TechnoTypeClass*> CanBeAutoTargetedBy;

			Valueable<bool> ROF_Random;
			Nullable<Point2D> Rof_RandomMinMax;

			Valueable<bool> TankDisguiseAsTank;
			ValueableVector<TechnoTypeClass*> DisguiseDisAllowed;
			//TrailsReader Trails;
			//FireSuperWeaponData SWFireData;
			Valueable <bool> VirtualUnit;

			//SpawnSupportFLHData MySpawnSupportFLH;
			//SpawnSupportData MySpawnSupportDatas;
			//PassengersData MyPassangersData;
			//JJFacingData  MyJJData;
			GiftBoxData MyGiftBoxData;
			//AircraftDiveData MyDiveData;
			//AircraftPutData MuPutData;

			ExtDataB() :

				FacingRotation_Disable { false }
				, FacingRotation_DisalbeOnEMP { false }
				, FacingRotation_DisalbeOnDeactivated { false }

				, HealthNumber_Show { false }
				, HealthNumber_Percent { false }
				, Healnumber_Offset {}

				, Draw_MindControlLink { true }

				, HealthBarSHP {}
				, HealthBarSHP_Selected {}
				, HealthBarSHPBracketOffset { 0 }
				, HealthBarSHP_HealthFrame { { 18,16,17 } }
				, HealthBarSHP_Palette {}
				, HealthBarSHP_PointOffset { { 0,0 } }
				, HealthbarRemap { false }
				, CustomMissileEliteRaise { true }
				, CustomMissileRaise { true }
				, IsCustomMissile { false }
				, CustomMissileData {}

				, SpyplaneCameraSound {}

				, Is_Cow { false }
				, DontShake { true }

				, CrateType { 0 } //not used

				, DiskLaserChargeUp {}
				, DrainAnimationType {}
				, JumpJet_HeightOffset {}

				, ParadropRadius {}
				, ParadropOverflRadius {}
				, Paradrop_DropPassangers { true }
				, Paradrop_MaxAttempt { }

				, CrashSpinLevelRate { 1.0 }
				, CrashSpinVerticalRate { 1.0 }

				, TalkBubbleTime {}

				, PipShapes01 {}
				, PipShapes02 {}
				, PipGarrison {}
				, PipGarrison_FrameIndex { 0 }
				, PipGarrison_Palette {}

				, HealthNumber_SHP {}
				, Healnumber_Decrement {}
				, ParasiteExit_Sound {}

				, AttackingAircraftSightRange {}

				, Overload_Count {}
				, Overload_Damage {}
				, Overload_Frames {}
				, Overload_DeathSound {}
				, Overload_ParticleSys {}
				, Overload_ParticleSysCount {}

				, Landing_Anim {}
				, Landing_AnimOnWater {}

				, HitCoordOffset { }
				, HitCoordOffset_Random { true }

				, HoverLocoIdx { 0 }

				, CrashWeapon { nullptr }

				, Disable_C4WarheadExp { false }

				, CrawlFLHData { }

				, Cloak_WhenMove { false }

				, GClock_Shape { }
				, GClock_Transculency { }
				, GClock_Palette { }

				, CanBeAutoTargetedBy { }

				, ROF_Random { true }
				, Rof_RandomMinMax { }
				, TankDisguiseAsTank { false }
				, DisguiseDisAllowed { }
				//, Trails { }
				//, SWFireData { }
				//, VirtualUnit { false }

				//, MySpawnSupportFLH { }
				//, MySpawnSupportDatas { }
				//, MyPassangersData { }
				//, MyJJData { }
				, MyGiftBoxData { }
				//, MyDiveData { }
				//, MuPutData { }
			{ }

			void Read_Rules(TechnoTypeClass* pThis, INI_EX& exINI, const char* pID);
			void Read_Art(TechnoTypeClass* pThis, INI_EX& exINI, const char* pID);

			void CleanUp() { }

			bool Load(PhobosStreamReader& Stm, bool RegisterForChange)
			{ Debug::Log("Loading Elements From TTyExt::ExtData ! \n"); return Serialize(Stm); }

			bool Save(PhobosStreamWriter& Stm)
			{ return Serialize(Stm); }

			void Init(TechnoTypeClass* pThis);

		private:
			template <typename T>
			bool Serialize(T& Stm)
			{
				return Stm
					.Process(FacingRotation_Disable)
					.Process(FacingRotation_DisalbeOnEMP)
					.Process(FacingRotation_DisalbeOnDeactivated)

					.Process(HealthNumber_Show)
					.Process(HealthNumber_Percent)
					.Process(Healnumber_Offset)

					.Process(Draw_MindControlLink)


					.Process(HealthBarSHP)
					.Process(HealthBarSHP_Selected)
					.Process(HealthBarSHPBracketOffset)
					.Process(HealthBarSHP_HealthFrame)
					.Process(HealthBarSHP_Palette)
					.Process(HealthBarSHP_PointOffset)
					.Process(HealthbarRemap)
					.Process(CustomMissileEliteRaise)
					.Process(CustomMissileRaise)
					.Process(IsCustomMissile)
					.Process(CustomMissileData)

					.Process(SpyplaneCameraSound)

					.Process(Is_Cow)
					.Process(DontShake)

					.Process(CrateType)

					.Process(DiskLaserChargeUp)
					.Process(DrainAnimationType)
					.Process(JumpJet_HeightOffset)

					.Process(ParadropRadius)
					.Process(ParadropOverflRadius)
					.Process(Paradrop_DropPassangers)

					.Process(CrashSpinLevelRate)
					.Process(CrashSpinVerticalRate)

					.Process(TalkBubbleTime)

					.Process(PipShapes01)
					.Process(PipShapes02)
					.Process(PipGarrison)
					.Process(PipGarrison_FrameIndex)
					.Process(PipGarrison_Palette)

					.Process(HealthNumber_SHP)
					.Process(Healnumber_Decrement)
					.Process(ParasiteExit_Sound)

					.Process(AttackingAircraftSightRange)

					.Process(Overload_Count)
					.Process(Overload_Damage)
					.Process(Overload_Frames)
					.Process(Overload_DeathSound)
					.Process(Overload_ParticleSys)
					.Process(Overload_ParticleSysCount)

					.Process(Landing_Anim)
					.Process(Landing_AnimOnWater)

					.Process(HitCoordOffset)
					.Process(HitCoordOffset_Random)

					.Process(HoverLocoIdx)

					.Process(CrashWeapon)

					.Process(Disable_C4WarheadExp)

					.Process(CrawlFLHData)

					.Process(Cloak_WhenMove)

					.Process(GClock_Shape)
					.Process(GClock_Transculency)
					.Process(GClock_Palette)

					.Process(CanBeAutoTargetedBy)

					.Process(ROF_Random)
					.Process(Rof_RandomMinMax)

					.Process(TankDisguiseAsTank)
					.Process(DisguiseDisAllowed)
					//.Process(Trails)
					//.Process(SWFireData)
					.Process(VirtualUnit)

					//.Process(MySpawnSupportFLH)
					//.Process(MySpawnSupportDatas)
					//.Process(MyPassangersData)

					//.Process(MyJJData)
					.Process(MyGiftBoxData)
					//.Process(MyDiveData)
					//.Process(MuPutData)
					.Success()
					;
			}
		};
	};
};