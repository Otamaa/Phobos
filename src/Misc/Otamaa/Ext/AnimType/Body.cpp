#include "Body.h"

#include <SuperWeaponTypeClass.h>

void Otamaa::ATExt::ExtData::Read(INI_EX& exINI, const char* pID)
{

	this->LaunchSW.Read(exINI, pID, "LauchSW");
	this->LaunchWhat.Read(exINI, pID, "LauchSW.Type", true);
	this->LaunchWaitcharge.Read(exINI, pID, "LauchSW.WaitForCharge");
	this->LaunchResetCharge.Read(exINI, pID, "LuachSW.ResetCharge");

	this->ParticleRangeMin.Read(exINI, pID, "SpawnsParticle.RangeMinimum");
	this->ParticleRangeMax.Read(exINI, pID, "SpawnsParticle.RangeMaximum");
	this->ParticleChance.Read(exINI, pID, "SpawnsParticle.Chance");

	this->SplashList.Read(exINI, pID, "SplashList");
	this->SplashIndex.Read(exINI, pID, "Splash.Index");
	this->SplashIndexRandom.Read(exINI, pID, "Splash.IsRandom");

	this->WakeAnim.Read(exINI, pID, "WakeAnim");
	this->ExplodeOnWater.Read(exINI, pID, "ExplodeOnWater");

	//set allocate to true to shut off debug warning
	this->SpawnsMultiple.Read(exINI, pID, "SpawnsMultiple", true);
	this->SpawnsMultiple_Random.Read(exINI, pID, "SpawnsMultiple.Random");

	if (!this->SpawnsMultiple.empty())
	{
		auto const nBaseSize = (int)this->SpawnsMultiple.size();
		this->SpawnsMultiple_amouts.Clear();
		this->SpawnsMultiple_amouts.Reserve(nBaseSize);
		this->SpawnsMultiple_amouts.Count = nBaseSize;
		auto const pKey = "SpawnsMultiple.Amount";

		for (auto& nSpawnMult : this->SpawnsMultiple_amouts)
			nSpawnMult = 1;

		if (exINI.ReadString(pID, pKey))
		{
			int nCount = 0;
			char* context = nullptr;
			for (char* cur = strtok_s(exINI.value(), Phobos::readDelims, &context); cur; cur = strtok_s(nullptr, Phobos::readDelims, &context))
			{
				if (Phobos::Config::MoreDetailSLDebugLog)
					Debug::Log("Parsing %d Size of [%s]%s=%s idx[%d] \n", nBaseSize, pID, pKey, cur, nCount);

				int buffer;
				if (Parser<int>::TryParse(cur, &buffer))
					this->SpawnsMultiple_amouts[nCount] = buffer;

				if (++nCount >= nBaseSize)
					break;
			}
		}
	}
}