#pragma once

#include <Utilities/TemplateDef.h>
//#include <Misc/Otamaa/Misc/DynamicPatcher/Trails/TrailsManager.h>
//#include <Misc/Otamaa/Misc/DynamicPatcher/Bullet/Straight/StraightBullet.h>
//#include <Misc/Otamaa/Misc/DynamicPatcher/Bullet/Proximity/Proximity.h>
//#include <Misc/Otamaa/Misc/DynamicPatcher/Bullet/Proximity/ProximityRange.h>

namespace Otamaa
{
	struct BlExt
	{
		struct ExtData
		{

			//std::vector<std::unique_ptr<UniversalTrail>> Trails;
			//UniqueGamePtr<StraightBullet> StraightBulletD;
			//UniqueGamePtr<Proximity> BulletProximity;
			//UniqueGamePtr<ProximityRange> BulletProximityRange;
			HouseClass* OwnerHouse;

			ExtData() :
				//Trails { }
				//, StraightBulletD { }
				//, BulletProximity { }
				//, BulletProximityRange { }
				//,
				OwnerHouse { nullptr }
			{ }

			void CleanUp() { }

			bool Load(PhobosStreamReader& Stm, bool RegisterForChange)
			{ Debug::Log("Loading Elements From BlExt::ExtData ! \n"); return Serialize(Stm); }

			bool Save(PhobosStreamWriter& Stm)
			{ return Serialize(Stm); }

		private:
			template <typename T>
			bool Serialize(T& Stm)
			{
				return Stm
					//.Process(Trails)
					//.Process(StraightBulletD)
					//.Process(BulletProximity)
					//.Process(BulletProximityRange)
					.Process(OwnerHouse)
					.Success()
					;
			}
		};
	};
}