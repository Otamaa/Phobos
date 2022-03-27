#pragma once

#include <Utilities/TemplateDef.h>
//#include <Misc/Otamaa/Misc/DynamicPatcher/Techno/AircraftPut/AircraftPutDataRules.h>

class ParticleTypeClass;
namespace Otamaa
{
	struct RExt
	{
		struct ExtData
		{
			NullableIdx<ParticleTypeClass> VeinholeParticle;
			PhobosFixedString<0x19> NukeWarheadName;
			Valueable<bool> Building_PlacementPreview;
			NullableVector<float> AI_AutoSellHealthRatio;

			Valueable<bool> DisablePathfindFailureLog;
			Valueable<int> PlacementGrid_TranslucentLevel;
			//AircraftPutDataRules MyPutData;

			ExtData() :

				 VeinholeParticle { }
				, NukeWarheadName { }
				, Building_PlacementPreview { }
				, AI_AutoSellHealthRatio { }
				, DisablePathfindFailureLog { false }
				, PlacementGrid_TranslucentLevel { 0 }
				//, MyPutData { }
			{}

			void Read_LoadBeforeTypeData(INI_EX& exINI);
			void Read_LoadAfterTypeData(INI_EX& exINI);


			bool Load(PhobosStreamReader& Stm, bool RegisterForChange)
			{ Debug::Log("Loading Elements From RExt::ExtData ! \n"); return Serialize(Stm); }

			bool Save(PhobosStreamWriter& Stm)
			{ return Serialize(Stm); }

		private:
			template <typename T>
			bool Serialize(T& Stm)
			{
				return Stm
					.Process(VeinholeParticle)
					.Process(NukeWarheadName)
					.Process(Building_PlacementPreview)
					.Process(AI_AutoSellHealthRatio)
					.Process(DisablePathfindFailureLog)
					.Process(PlacementGrid_TranslucentLevel)
					//.Process(MyPutData)
					.Success()
					;
			}
		};

	};

};