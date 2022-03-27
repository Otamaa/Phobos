#pragma once

#include <Utilities/TemplateDef.h>

namespace Otamaa
{
	struct SWTExt
	{
		struct ExtData
		{
			Nullable<ShapeHandlerEnumerator*> GClock_Shape;
			Nullable<int> GClock_Transculency;
			CustomPalette GClock_Palette;
			Valueable<bool> ChargeTimer;
			Valueable<bool> ChargeTimer_Backwards;

			ExtData() :
				GClock_Shape { }
				, GClock_Transculency { }
				, GClock_Palette { }
				, ChargeTimer { false }
				, ChargeTimer_Backwards { false }
			{}

			void Read(INI_EX& exINI, const char* pID)
			{
				this->GClock_Shape.Read(exINI, pID, "GClock.Shape");
				this->GClock_Transculency.Read(exINI, pID, "GClock.Transculency");
				this->GClock_Palette.Read(exINI.GetINI(), pID, "GClock.Palette");

				// no , still broken
				this->ChargeTimer.Read(exINI, pID, "Timer.ChargeMode");
				this->ChargeTimer_Backwards.Read(exINI, pID, "Timer.ChargeModeBackwards");
			}

			bool Load(PhobosStreamReader& Stm, bool RegisterForChange)
			{ Debug::Log("Loading Elements From SWTExt::ExtData ! \n"); return Serialize(Stm); }

			bool Save(PhobosStreamWriter& Stm)
			{ return Serialize(Stm); }

		private:
			template <typename T>
			bool Serialize(T& Stm)
			{
				return Stm
					.Process(GClock_Shape)
					.Process(GClock_Transculency)
					.Process(GClock_Palette)
					.Process(ChargeTimer)
					.Process(ChargeTimer_Backwards)
					.Success()
					;
			}
		};
	};
};