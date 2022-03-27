#pragma once

#include <Utilities/TemplateDef.h>
//#include <Misc/Otamaa/Misc/DynamicPatcher/Trails/TrailsManager.h>

class AnimTypeClass;
class WeaponTypeClass;
namespace Otamaa
{
	struct VTExt
	{
		struct ExtData
		{
			NullableVector <AnimTypeClass*> SplashList;//
			Nullable<AnimTypeClass*> WakeAnim; //
			Valueable<bool> ExplodeOnWater;
			Valueable<WeaponTypeClass*> Weapon;
			//TrailsReader Trails;

			ExtData() :
				SplashList { }
				, WakeAnim { }
				, ExplodeOnWater { false }
				, Weapon(nullptr)
				//, Trails { }
			{ }

			void Read_Rules(INI_EX& exINI, const char* pID)
			{
				this->SplashList.Read(exINI, pID, "SplashList");
				this->WakeAnim.Read(exINI, pID, "WakeAnim");
				this->ExplodeOnWater.Read(exINI, pID, "ExplodeOnWater");
				this->Weapon.Read(exINI, pID, "Weapon", true);
				//this->Trails.Read(exINI, pID);
			}

			bool Load(PhobosStreamReader& Stm, bool RegisterForChange)
			{ Debug::Log("Loading Elements From VTExt::ExtData ! \n"); return Serialize(Stm); }

			bool Save(PhobosStreamWriter& Stm)
			{ return Serialize(Stm); }

		private:
			template <typename T>
			bool Serialize(T& Stm)
			{
				return Stm
					.Process(SplashList)
					.Process(WakeAnim)
					.Process(ExplodeOnWater)
					.Process(Weapon)
					//.Process(Trails)
					.Success()
					;
			}
		};
	};
};