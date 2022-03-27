#pragma once

#include <Utilities/TemplateDef.h>
//#include <Misc/Otamaa/Misc/DynamicPatcher/Trails/TrailsManager.h>
//#include <Misc/Otamaa/Misc/DynamicPatcher/Bullet/Straight/StraightBulletData.h>
//#include <Misc/Otamaa/Misc/DynamicPatcher/Bullet/Proximity/ProximityRangeData.h>
//#include <Misc/Otamaa/Misc/DynamicPatcher/Bullet/Proximity/ProximityData.h>
//#include <Misc/Otamaa/Misc/DynamicPatcher/Bullet/MissileBullet/MissileBulletData.h>

class AnimTypeClass;
namespace Otamaa
{
	struct BlTExt
	{
		struct ExtData
		{
			Nullable<AnimTypeClass*> Parachute;
			Nullable<double> MissileROTVar;
			Nullable<int> MissileSafetyAltitude;

			//TrailsReader Trails;
			//StraightBulletData StraightBulletDatas;
			//ProximityData BulletProximityData;
			//MissileBulletData MissileData;
			ExtData() :
				 Parachute { }
				, MissileROTVar { }
				, MissileSafetyAltitude { }
				//, Trails { }
				//, StraightBulletDatas { }
				//, BulletProximityData { }
				//, MissileData { }

			{ }

			void Read_Rules(INI_EX& exINI, const char* pID);
			void Read_Art(INI_EX& exINI, const char* pID);

			void CleanUp() { }

			bool Load(PhobosStreamReader& Stm, bool RegisterForChange)
			{ Debug::Log("Loading Elements From BlTExt::ExtData ! \n"); return Serialize(Stm); }

			bool Save(PhobosStreamWriter& Stm)
			{ return Serialize(Stm); }

		private:
			template <typename T>
			bool Serialize(T& Stm)
			{
				return Stm
					.Process(Parachute)
					.Process(MissileROTVar)
					.Process(MissileSafetyAltitude)
					//.Process(Trails)
					//.Process(StraightBulletDatas)
					//.Process(BulletProximityData)
					//.Process(MissileData)
					.Success()
					;
			}
		};
	};
};