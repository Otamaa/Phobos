#pragma once

#include <Utilities/TemplateDef.h>
//#include <Misc/Otamaa/Misc/DynamicPatcher/Trails/TrailsManager.h>

namespace Otamaa
{
	struct VExt
	{
		struct ExtData
		{
		//	std::vector<std::unique_ptr<UniversalTrail>> Trails;

			ExtData() //:
			//	Trails { }
			{ }

			void CleanUp() { }

			bool Load(PhobosStreamReader& Stm, bool RegisterForChange)
			{ Debug::Log("Loading Elements From VExt::ExtData ! \n"); return Serialize(Stm); }

			bool Save(PhobosStreamWriter& Stm)
			{ return Serialize(Stm); }

		private:
			template <typename T>
			bool Serialize(T& Stm)
			{
				return Stm
				//	.Process(Trails)
					.Success()
					;
			}
		};
	};
}