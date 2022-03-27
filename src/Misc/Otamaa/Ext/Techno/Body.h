#pragma once

#include <Utilities/TemplateDef.h>
//#include <Misc/Otamaa/Misc/DynamicPatcher/Techno/DriveData/DriveData.h>
//#include <Misc/Otamaa/Misc/DynamicPatcher/Trails/TrailsManager.h>
//#include <Misc/Otamaa/Misc/DynamicPatcher/Techno/SpawnSupport/SpawnSupport.h>
//#include <Misc/Otamaa/Misc/DynamicPatcher/Techno/JumjetFaceTarget/JJFacingToTarget.h>
#include <Misc/Otamaa/Misc/DynamicPatcher/Techno/GiftBox/GiftBox.h>
//#include <Misc/Otamaa/Misc/DynamicPatcher/Techno/AircraftDive/AircraftDive.h>

namespace Otamaa
{
	struct TTExt
	{
		struct ExtData
		{
			bool IsDtorEd;
//			std::vector<std::unique_ptr<UniversalTrail>> Trails;
//			DriveData MyDriveData;
//			SpawnSupport MySpawnSuport;
//			JJFacingToTarget MyJJData;
			UniqueGamePtr<GiftBox> MyGiftBox;
//			UniqueGamePtr<AircraftDive> MyDiveData;
//			bool aircraftPutOffsetFlag;
//			bool aircraftPutOffset;

			ExtData() :
				 IsDtorEd { false }
//				, Trails { }
//				, MyDriveData { }
//				, MySpawnSuport { }
//				, MyJJData { }
				, MyGiftBox { }
//				, MyDiveData { }
//				, aircraftPutOffsetFlag { false }
//				, aircraftPutOffset { false }
			{ }

			void CleanUp() { IsDtorEd = true; }
			static bool AI(TechnoClass* pThis);

			bool Load(PhobosStreamReader& Stm, bool RegisterForChange)
			{ Debug::Log("Loading Elements From TTExt::ExtData ! \n"); return Serialize(Stm); }

			bool Save(PhobosStreamWriter& Stm)
			{ return Serialize(Stm); }

		private:
			template <typename T>
			bool Serialize(T& Stm)
			{
				return Stm
//					.Process(Trails)
//					.Process(MyDriveData)
//					.Process(MySpawnSuport)
//					.Process(MyJJData)
					.Process(MyGiftBox)
//					.Process(MyDiveData)
//					.Process(aircraftPutOffsetFlag)
//					.Process(aircraftPutOffset)
					.Process(IsDtorEd)
					.Success()
					;
			}
		};
	};
}