#pragma once

#include <Utilities/TemplateDef.h>
#include <Utilities/Constructs.h>

class AnimTypeClass;
class SuperWeaponTypeClass;
namespace Otamaa
{
	struct WHExt
	{
		struct ExtData
		{
			PhobosMap<int,AnimTypeClass*> ArmorHitAnim;

			//
			NullableVector<AnimTypeClass*> DebrisAnimTypes;

			NullableVector<AnimTypeClass*> SquidSplash;

			Valueable<AnimTypeClass*> TemporalExpiredAnim;
			Valueable<bool> TemporalExpiredApplyDamage;
			Valueable<double> TemporalDetachDamageFactor;

			Valueable<bool> Parasite_DisableRocking;
			NullableIdx<AnimTypeClass> Parasite_GrappleAnimIndex;
			//
			Nullable<int> Flammability;

			Valueable<bool> LaunchSW;
			Nullable<SuperWeaponTypeClass*> LaunchWhat;
			Valueable<bool> LaunchWaitcharge;
			Valueable<bool> LaunchResetCharge;
			//
			Valueable<bool> IsNukeWarhead;
			//
			Nullable<AnimTypeClass*> NotHuman_DeathAnim;

			Valueable<bool> Eraser;
			Valueable<AnimTypeClass*> Eraser_Anim;

			ExtData() :
				NotHuman_DeathAnim { }

				, LaunchSW { false }
				, LaunchWhat {}
				, LaunchWaitcharge { false }
				, LaunchResetCharge { false }

				, SquidSplash {}

				, TemporalExpiredAnim { nullptr }
				, TemporalExpiredApplyDamage { false }
				, TemporalDetachDamageFactor { 1.0 }

				, DebrisAnimTypes {}
				, Flammability {}

				, Parasite_DisableRocking {}
				, Parasite_GrappleAnimIndex {}

				, ArmorHitAnim { }
				, IsNukeWarhead { false }

				, Eraser { false }
				, Eraser_Anim { nullptr }
			{ }


			void Read(INI_EX& exINI, const char* pID);

			bool Load(PhobosStreamReader& Stm, bool RegisterForChange)
			{ Debug::Log("Loading Elements From WHExt::ExtData ! \n"); return Serialize(Stm); }

			bool Save(PhobosStreamWriter& Stm)
			{ return Serialize(Stm); }

		private:
			template <typename T>
			bool Serialize(T& Stm)
			{
				return Stm
					.Process(NotHuman_DeathAnim)

					.Process(LaunchSW)
					.Process(LaunchWhat)
					.Process(LaunchWaitcharge)
					.Process(LaunchResetCharge)

					.Process(SquidSplash)

					.Process(TemporalExpiredAnim)
					.Process(TemporalExpiredApplyDamage)
					.Process(TemporalDetachDamageFactor)

					.Process(DebrisAnimTypes)
					.Process(Flammability)

					.Process(Parasite_DisableRocking)
					.Process(Parasite_GrappleAnimIndex)

					.Process(ArmorHitAnim)
					.Process(IsNukeWarhead)

					.Process(Eraser)
					.Process(Eraser_Anim)
					.Success()
					;
			}
		};
	};
};