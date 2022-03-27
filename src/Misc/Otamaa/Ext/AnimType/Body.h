#pragma once

#include <Utilities/TemplateDef.h>

class SuperWeaponTypeClass;
class AnimTypeClass;
namespace Otamaa
{
	struct ATExt
	{
		struct ExtData
		{
			Valueable<bool> LaunchSW;
			Nullable<SuperWeaponTypeClass*> LaunchWhat;
			Valueable<bool> LaunchWaitcharge;
			Valueable<bool> LaunchResetCharge;

			Valueable<double> ParticleRangeMin;
			Valueable<double> ParticleRangeMax;
			Nullable<int> ParticleChance;

			NullableVector <AnimTypeClass*> SplashList;
			Nullable<int> SplashIndex;
			Valueable<bool> SplashIndexRandom;

			Nullable<AnimTypeClass*> WakeAnim;
			Valueable<bool> ExplodeOnWater;

			ValueableVector<AnimTypeClass*> SpawnsMultiple;
			Valueable<bool> SpawnsMultiple_Random;
			DynamicVectorClass<int> SpawnsMultiple_amouts;

			ExtData() :
				 LaunchSW { false }
				, LaunchWhat {}
				, LaunchWaitcharge { false }
				, LaunchResetCharge { false }

				, ParticleRangeMin { 0.0 }
				, ParticleRangeMax { 0.0 }
				, ParticleChance {}

				, SplashList {}
				, WakeAnim {}
				, ExplodeOnWater { false }

				, SplashIndex {}
				, SplashIndexRandom { false }

				, SpawnsMultiple {}
				, SpawnsMultiple_amouts {}
				, SpawnsMultiple_Random { false }

			{ }


			void Read(INI_EX& exINI, const char* pID);

			void CleanUp()
			{
				SpawnsMultiple_amouts.Clear();
			}

			bool Load(PhobosStreamReader& Stm, bool RegisterForChange)
			{ Debug::Log("Loading Elements From ATExt::ExtData ! \n"); return Serialize(Stm); }

			bool Save(PhobosStreamWriter& Stm)
			{ return Serialize(Stm); }

		private:
			template <typename T>
			bool Serialize(T& Stm)
			{
				return Stm
					.Process(LaunchSW)
					.Process(LaunchWhat)
					.Process(LaunchWaitcharge)
					.Process(LaunchResetCharge)

					.Process(ParticleRangeMin)
					.Process(ParticleRangeMax)
					.Process(ParticleChance)

					.Process(SplashList)
					.Process(WakeAnim)
					.Process(ExplodeOnWater)

					.Process(SplashIndex)
					.Process(SplashIndexRandom)

					.Process(SpawnsMultiple)
					.Process(SpawnsMultiple_Random)
					.Process(SpawnsMultiple_amouts)

					.Success()
					;
			}
		};
	};
};