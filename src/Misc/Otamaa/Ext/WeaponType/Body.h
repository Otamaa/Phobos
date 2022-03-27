#pragma once

#include <Utilities/TemplateDef.h>
//#include <Misc/Otamaa/Misc/DynamicPatcher/Bullet/Proximity/ProximityRangeData.h>

class AnimTypeClass;
namespace Otamaa
{
	struct WPExt
	{
		struct ExtData
		{

			Valueable<int>Xhi;
			Valueable<int>Xlo;
			Valueable<int>Yhi;
			Valueable<int>Ylo;
			Valueable<bool> ShakeLocal;

			Valueable<Leptons> ProjectileRange;

			ValueableVector<AnimTypeClass*> OccupantAnims;
			Valueable<bool> OccupantAnim_UseMultiple;
			Valueable<int> VeinHoleDamage;
			Valueable<bool> VeinHoleCaller;

			Valueable<bool>Range_IgnoreVertical;
			Valueable<float> RockerPitch;
			//ProximityRangeData ProximityRangeDatas;

			ExtData() :
				  Xhi { 0 }
				, Xlo { 0 }
				, Yhi { 0 }
				, Ylo { 0 }
				, ProjectileRange { Leptons(100000) }
				, OccupantAnims {}
				, OccupantAnim_UseMultiple { false }
				, VeinHoleDamage { 0 }
				, VeinHoleCaller { false }
				, Range_IgnoreVertical { false }
				, RockerPitch { 0.0f }
				//, ProximityRangeDatas { }
			{}

			void Read(INI_EX& exINI, const char* pID)
			{
				auto pSection = pID;

				this->Ylo.Read(exINI, pSection, "ShakeYlo");
				this->Yhi.Read(exINI, pSection, "ShakeYhi");
				this->Xhi.Read(exINI, pSection, "ShakeXhi");
				this->Xlo.Read(exINI, pSection, "ShakeXlo");
				this->ShakeLocal.Read(exINI, pSection, "Shake.Local");

				this->ProjectileRange.Read(exINI, pSection, "ProjectileRange");

				this->OccupantAnims.Read(exINI, pSection, "OccupantAnims");
				this->OccupantAnim_UseMultiple.Read(exINI, pSection, "OccupantAnim.UseAnimsInstead");
				this->VeinHoleDamage.Read(exINI, pSection, "ToVeinholeDamage");
				this->VeinHoleCaller.Read(exINI, pSection, "ToVeinhole");

				this->Range_IgnoreVertical.Read(exINI, pSection, "Range.IgnoreVertical");
				this->RockerPitch.Read(exINI, pSection, "RockerPitch");

				if (this->RockerPitch > 0.0f)
				{
					this->RockerPitch = 1.0f;
					this->RockerPitch = this->RockerPitch.Get() * (Math::PI / 2);
				}
				//this->ProximityRangeDatas.Read(exINI, pID, true);
			}

			bool Load(PhobosStreamReader& Stm, bool RegisterForChange)
			{ Debug::Log("Loading Elements From WPExt::ExtData ! \n"); return Serialize(Stm); }

			bool Save(PhobosStreamWriter& Stm)
			{ return Serialize(Stm); }

		private:
			template <typename T>
			bool Serialize(T& Stm)
			{
				return Stm
					.Process(this->VeinHoleCaller)
					.Process(this->VeinHoleDamage)
					.Process(this->Ylo)
					.Process(this->Xlo)
					.Process(this->Xhi)
					.Process(this->Yhi)
					.Process(this->ProjectileRange)
					.Process(this->OccupantAnims)
					.Process(this->OccupantAnim_UseMultiple)
					.Process(this->ShakeLocal)
					.Process(this->Range_IgnoreVertical)
					.Process(this->RockerPitch)
					//.Process(this->ProximityRangeDatas)
					.Success()
					;
			}
		};
	};
};