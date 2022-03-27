#pragma once

#include <Utilities/TemplateDef.h>
#include <Misc/Otamaa/New/Type/ShapeHandlerEnumerator.h>

class AnimTypeClass;
namespace Otamaa
{
	struct HealthOnFireData
	{
		bool RedOnFire;
		bool GreenOnFire;
		bool YellowOnFire;

		HealthOnFireData() :
			RedOnFire(true), GreenOnFire(false), YellowOnFire(true)
		{
		}

		virtual bool Read(INI_EX& parser, const char* pSection, const char* pKey)
		{
			if (!pSection)
				return false;

			auto value = this;
			if (parser.ReadString(pSection, pKey))
			{
				auto const buffer = parser.value();
				char* context = nullptr;
				if (auto const nRedOnFire = strtok_s(buffer, Phobos::readDelims, &context))
				{
					Parser<bool>::Parse(nRedOnFire, &value->RedOnFire);
				}
				if (auto const nGreenOnFire = strtok_s(nullptr, Phobos::readDelims, &context))
				{
					Parser<bool>::Parse(nGreenOnFire, &value->GreenOnFire);
				}
				if (auto const nYellowOnFire = strtok_s(nullptr, Phobos::readDelims, &context))
				{
					Parser<bool>::Parse(nYellowOnFire, &value->YellowOnFire);
				}

				return true;
			}

			return false;
		}

		bool Load(PhobosStreamReader& Stm, bool RegisterForChange)
		{ Debug::Log("Loading Elements From HealthOnFireData ! \n"); return Serialize(Stm); }

		bool Save(PhobosStreamWriter& Stm)
		{ return Serialize(Stm); }

	private:
		template <typename T>
		bool Serialize(T& Stm)
		{
			return Stm
				.Process(RedOnFire)
				.Process(GreenOnFire)
				.Process(YellowOnFire)
				.Success()
				;
		}
	};

	struct BTExt
	{
		struct ExtData
		{
			NullableVector<AnimTypeClass*> DamageFireTypes;
			NullableVector<AnimTypeClass*> OnFireTypes;
			NullableVector<int> OnFireIndex;
			HealthOnFireData HealthOnfire;
			DynamicVectorClass<Point2D> DamageFire_Offs;

			Nullable<double> RepairRate;
			Nullable<int> RepairStep;

			Nullable<bool> PlayerReturnFire;

			Valueable<bool> PackupSound_PlayGlobal;
			Valueable<bool> DisableDamageSound;

			Nullable<float> BuildingOccupyDamageMult;
			Nullable<float> BuildingOccupyROFMult;

			Nullable<float> BuildingBunkerDamageMult;
			Nullable<float> BuildingBunkerROFMult;


			NullableIdx<VocClass> BunkerWallsUpSound;
			NullableIdx<VocClass> BunkerWallsDownSound;

			ValueableIdxVector<BuildingTypeClass> AIBuildInsteadPerDiff;

			PhobosMap<int, AnimTypeClass*> GarrisonAnim_idle;
			PhobosMap<int, AnimTypeClass*> GarrisonAnim_ActiveOne;
			PhobosMap<int, AnimTypeClass*> GarrisonAnim_ActiveTwo;
			PhobosMap<int, AnimTypeClass*> GarrisonAnim_ActiveThree;
			PhobosMap<int, AnimTypeClass*> GarrisonAnim_ActiveFour;

			CustomPalette PipShapes01Palette;
			Valueable<bool> PipShapes01Remap;

			Nullable<bool> PlacementPreview_Show;
			Nullable<ShapeHandlerEnumerator*> PlacementPreview_Shape;
			Valueable<int> PlacementPreview_ShapeFrame;
			Nullable<Point2D> PlacementPreview_Offset;
			Valueable<bool> PlacementPreview_Remap;
			CustomPalette PlacementPreview_Palette;
			Valueable<int> PlacementPreview_Transculency;

			Nullable<AnimTypeClass*> TurretAnim_LowPower;
			Nullable<AnimTypeClass*> TurretAnim_DamagedLowPower;

			Valueable<bool> BuildUp_UseNormalLIght;

			Valueable<bool> Power_DegradeWithHealth;

			Nullable<float> AutoSellTime;

			Valueable<bool> IsJuggernaut;
			Nullable<short> SolidHeight;

			ExtData() :
				TurretAnim_LowPower { }
				, TurretAnim_DamagedLowPower { }
				, PlacementPreview_Remap { true }
				, PlacementPreview_Palette {}
				, PlacementPreview_Offset {}
				, PlacementPreview_Show {}
				, PlacementPreview_Shape {}
				, PlacementPreview_ShapeFrame { 0 }
				, PlacementPreview_Transculency { 0 }
				, PipShapes01Remap { false }
				, PipShapes01Palette { CustomPalette::PaletteMode::Temperate }

				, DamageFireTypes {}
				, RepairRate {}
				, RepairStep {}


				, OnFireIndex {}
				, PlayerReturnFire {}

				, PackupSound_PlayGlobal { false }
				, DisableDamageSound { false }

				, BuildingOccupyDamageMult {}
				, BuildingOccupyROFMult {}

				, BuildingBunkerDamageMult {}
				, BuildingBunkerROFMult {}

				, BunkerWallsUpSound { }
				, BunkerWallsDownSound { }

				, AIBuildInsteadPerDiff {}
				, OnFireTypes {}
				, HealthOnfire {}
				, GarrisonAnim_idle {}
				, GarrisonAnim_ActiveOne {}
				, GarrisonAnim_ActiveTwo {}
				, GarrisonAnim_ActiveThree {}
				, GarrisonAnim_ActiveFour {}
				, IsJuggernaut {}
				, DamageFire_Offs {}
				, BuildUp_UseNormalLIght { false }
				, Power_DegradeWithHealth { true }
				, AutoSellTime { }
				, SolidHeight { }

			{ }

			void Read_Rules(INI_EX& exINI, const char* pID);
			void Read_Art(INI_EX& exINI, const char* pID);

			void CleanUp()
			{
				GarrisonAnim_idle.clear();
				GarrisonAnim_ActiveOne.clear();
				GarrisonAnim_ActiveTwo.clear();
				GarrisonAnim_ActiveThree.clear();
				GarrisonAnim_ActiveFour.clear();
				DamageFire_Offs.Clear();
			}

			bool Load(PhobosStreamReader& Stm, bool RegisterForChange)
			{ Debug::Log("Loading Elements From BTExt::ExtData ! \n"); return Serialize(Stm); }

			bool Save(PhobosStreamWriter& Stm)
			{ return Serialize(Stm); }

		private:
			template <typename T>
			bool Serialize(T& Stm)
			{
				return Stm
					.Process(TurretAnim_LowPower)
					.Process(TurretAnim_DamagedLowPower)
					.Process(PlacementPreview_Remap)
					.Process(PlacementPreview_Palette)
					.Process(PlacementPreview_Offset)
					.Process(PlacementPreview_Show)
					.Process(PlacementPreview_Shape)
					.Process(PlacementPreview_ShapeFrame)
					.Process(PlacementPreview_Transculency)
					.Process(PipShapes01Remap)
					.Process(PipShapes01Palette)

					.Process(DamageFireTypes)
					.Process(RepairRate)
					.Process(RepairStep)

					.Process(OnFireIndex)
					.Process(PlayerReturnFire)

					.Process(PackupSound_PlayGlobal)
					.Process(DisableDamageSound)
					.Process(BuildingOccupyDamageMult)
					.Process(BuildingOccupyROFMult)
					.Process(BuildingBunkerDamageMult)
					.Process(BuildingBunkerROFMult)

					.Process(BunkerWallsUpSound)
					.Process(BunkerWallsDownSound)

					.Process(AIBuildInsteadPerDiff)
					.Process(OnFireTypes)
					.Process(HealthOnfire)
					.Process(GarrisonAnim_idle)
					.Process(GarrisonAnim_ActiveOne)
					.Process(GarrisonAnim_ActiveTwo)
					.Process(GarrisonAnim_ActiveThree)
					.Process(GarrisonAnim_ActiveFour)
					.Process(IsJuggernaut)
					.Process(DamageFire_Offs)
					.Process(BuildUp_UseNormalLIght)
					.Process(Power_DegradeWithHealth)
					.Process(AutoSellTime)
					.Process(SolidHeight)
					.Success()
					;
			}
		};

	};
};