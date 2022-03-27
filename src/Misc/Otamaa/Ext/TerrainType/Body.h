#pragma once

#include <Utilities/TemplateDef.h>

namespace Otamaa
{
	struct TeTExt
	{
		struct ExtData
		{
			Nullable<int> LightVisibility;
			Nullable<double> LightIntensity;
			Nullable<double> LightRedTint;
			Nullable<double> LightGreenTint;
			Nullable<double> LightBlueTint;

			ValueableVector<AnimTypeClass*> AttachedAnim;

			ExtData() :
				LightVisibility { 0 }
				, LightIntensity { }
				, LightRedTint { }
				, LightGreenTint { }
				, LightBlueTint { }
				, AttachedAnim { }
			{ }

			void Read_Rules(INI_EX& exINI, const char* pID)
			{
				this->LightVisibility.Read(exINI, pID, "LightVisibility");
				this->LightIntensity.Read(exINI, pID, "LightIntensity");
				this->LightRedTint.Read(exINI, pID, "LightRedTint");
				this->LightGreenTint.Read(exINI, pID, "LightGreenTint");
				this->LightBlueTint.Read(exINI, pID, "LightBlueTint");

				this->AttachedAnim.Read(exINI, pID, "AttachedAnims");
			}

			int GetLightIntensity() const
			{
				auto const Intense = this->LightIntensity.Get(0.0);
				return (int)(Intense * 1000.0);
			}

			TintStruct GetLightTint() const
			{
				auto ToInt = [](double nInput)
				{ return Math::clamp(((int)(nInput * 1000.0)), -2000, 2000); };

				return TintStruct
				{
					ToInt(this->LightRedTint.Get(1.0)),
					ToInt(this->LightGreenTint.Get(1.0)),
					ToInt(this->LightBlueTint.Get(1.0))
				};
			}

			bool Load(PhobosStreamReader& Stm, bool RegisterForChange)
			{ Debug::Log("Loading Elements From TeTExt::ExtData ! \n"); return Serialize(Stm); }

			bool Save(PhobosStreamWriter& Stm)
			{ return Serialize(Stm); }

		private:
			template <typename T>
			bool Serialize(T& Stm)
			{
				return Stm

					.Process(LightVisibility)
					.Process(LightIntensity)
					.Process(LightRedTint)
					.Process(LightGreenTint)
					.Process(LightBlueTint)
					.Process(AttachedAnim)
					.Success()
					;
			}
		};

	};
};