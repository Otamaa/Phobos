#include "Body.h"

#include <Misc/Otamaa/New/Type/ArmorTypeClass.h>
#include <experimental/map>

void Otamaa::WHExt::ExtData::Read(INI_EX& exINI, const char* pID)
{
	auto pSection = pID;

	this->NotHuman_DeathAnim.Read(exINI, pSection, "NotHuman.DeahAnim");

	this->SquidSplash.Read(exINI, pSection, "Squid.SplashAnims");
	this->TemporalExpiredAnim.Read(exINI, pSection, "Temporal.ExpiredAnim");
	this->TemporalDetachDamageFactor.Read(exINI, pSection, "Temporal.ExpiredDamageFactor");
	this->TemporalExpiredApplyDamage.Read(exINI, pSection, "Temporal.ExpiredApplyDamage");
	this->DebrisAnimTypes.Read(exINI, pSection, "DebrisAnims");
	this->Flammability.Read(exINI, pSection, "FlameChance");

	this->LaunchSW.Read(exINI, pSection, "LauchSW", true);

	if (SuperWeaponTypeClass::Array->IsAllocated)
		this->LaunchWhat.Read(exINI, pSection, "LauchSW.Type", true);

	this->LaunchWaitcharge.Read(exINI, pSection, "LauchSW.WaitForCharge");
	this->LaunchResetCharge.Read(exINI, pSection, "LuachSW.ResetCharge");

	this->Parasite_DisableRocking.Read(exINI, pSection, "Parasite.DisableRocking");
	this->Parasite_GrappleAnimIndex.Read(exINI, pSection, "Parasite.GrappleAnim");

	auto ReadHitAnim = [this, &exINI, pSection](int nSize, const char* pBaseKey, bool bAllocate = true)
	{
		char tempBuffer[2048];
		for (int i = 0; i < nSize; ++i)
		{
			Nullable<AnimTypeClass*> pAnimReaded;
			if (auto pArmor = ArmorTypeClass::FindFromIndex(i))
			{
				_snprintf_s(tempBuffer, _TRUNCATE, "%s.%s", pBaseKey, pArmor->Name.data());
				pAnimReaded.Read(exINI, pSection, tempBuffer, bAllocate);
			}
			ArmorHitAnim[i] = pAnimReaded.Get(nullptr);
		}

		//remove invalid items!
		if (!ArmorHitAnim.empty())
		{
			//remove invalid items to keep memory clean !
			for (auto const& nData : ArmorHitAnim)
			{
				if (!nData.second)
					ArmorHitAnim.erase(nData.first);
			}
		}
	};

	ReadHitAnim(ArmorTypeClass::Array.size(), "HitAnim");
	this->IsNukeWarhead.Read(exINI, pSection, "IsNukeWarhead");
	this->Eraser.Read(exINI, pSection, "Eraser");
	this->Eraser_Anim.Read(exINI, pSection, "Eraser.Anim");

}