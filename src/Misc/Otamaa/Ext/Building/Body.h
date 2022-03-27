#pragma once

#include <Utilities/TemplateDef.h>

class AnimClass;
namespace Otamaa
{
	struct BExt
	{
		struct ExtData
		{
			ValueableVector<AnimClass*> DamageFireAnims;
			TimerStruct AutoSellTimer;

			ExtData() :
				DamageFireAnims { }
				, AutoSellTimer { }
			{ }

			static void Init(BuildingClass* pThis);
			void CleanUp() { }

			bool Load(PhobosStreamReader& Stm, bool RegisterForChange)
			{ Debug::Log("Loading Elements From BExt::ExtData ! \n"); return Serialize(Stm); }

			bool Save(PhobosStreamWriter& Stm)
			{ return Serialize(Stm); }

		private:
			template <typename T>
			bool Serialize(T& Stm)
			{
				return Stm
					.Process(DamageFireAnims)
					.Process(AutoSellTimer)
					.Success()
					;
			}
		};
	};
};