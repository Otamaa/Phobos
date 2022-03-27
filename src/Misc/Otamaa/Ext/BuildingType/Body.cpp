#include "Body.h"

void Otamaa::BTExt::ExtData::Read_Rules(INI_EX& exINI, const char* pID)
{
	auto pSection = pID;

	auto GetGarrisonAnim = [&exINI, pID](
	PhobosMap<int,AnimTypeClass*>& nVec, const char* pBaseFlag, bool bAllocate = true, bool bParseDebug = false)
	{
		auto nHouseCount = HouseTypeClass::Array()->Count;
		char tempBuffer[2048];
		nVec.clear();

		for (int i = 0; i < nHouseCount; ++i)
		{
			Nullable<AnimTypeClass*> nBuffer;
			_snprintf_s(tempBuffer, sizeof(tempBuffer), "%s%d", pBaseFlag, i);

			if (bParseDebug)
				Debug::Log("GetGarrisonAnim for [%s]=%s idx[%d] \n", pID, tempBuffer, i);

			nBuffer.Read(exINI, pID, tempBuffer, bAllocate);

			if (!nBuffer.isset())
				continue;

			nVec[i] = nBuffer.Get(nullptr);
		}

		if (!nVec.empty())
		{
			//remove invalid items to keep memory clean !
			for (auto const& nData : nVec)
			{
				if (!nData.second)
					nVec.erase(nData.first);
			}
		}
	};

	GetGarrisonAnim(this->GarrisonAnim_idle, "GarrisonAnim.IdleForCountry", Phobos::Config::MoreDetailSLDebugLog);
	GetGarrisonAnim(this->GarrisonAnim_ActiveOne, "GarrisonAnim.ActiveOneForCountry", Phobos::Config::MoreDetailSLDebugLog);
	GetGarrisonAnim(this->GarrisonAnim_ActiveTwo, "GarrisonAnim.ActiveTwoForCountry", Phobos::Config::MoreDetailSLDebugLog);
	GetGarrisonAnim(this->GarrisonAnim_ActiveThree, "GarrisonAnim.ActiveThreeForCountry", Phobos::Config::MoreDetailSLDebugLog);
	GetGarrisonAnim(this->GarrisonAnim_ActiveFour, "GarrisonAnim.ActiveFourForCountry", Phobos::Config::MoreDetailSLDebugLog);

	this->AIBuildInsteadPerDiff.Read(exINI, pID, "AIBuildInstead");

	this->PackupSound_PlayGlobal.Read(exINI, pID, "PackupSoundPlayGlobal");

	this->DamageFireTypes.Read(exINI, pID, "DamageFireTypes");

	this->RepairRate.Read(exINI, pID, "RepairRate");
	this->RepairStep.Read(exINI, pID, "RepairStep");

	this->DisableDamageSound.Read(exINI, pID, "DisableDamagedSound");
	this->PlayerReturnFire.Read(exINI, pID, "PlayerReturnFire");

	this->BuildingOccupyDamageMult.Read(exINI, pID, "OccupyDamageMultiplier");
	this->BuildingOccupyROFMult.Read(exINI, pID, "OccupyROFMultiplier");

	this->BuildingBunkerDamageMult.Read(exINI, pID, "BunkerDamageMultiplier");
	this->BuildingBunkerROFMult.Read(exINI, pID, "BunkerROFMultMultiplier");

	this->BunkerWallsUpSound.Read(exINI, pID, "BunkerWallsUpSound");
	this->BunkerWallsDownSound.Read(exINI, pID, "BunkerWallsDownSound");

	this->PipShapes01Palette.Read(exINI.GetINI(), pID, "PipShapes.Palette");
	this->PipShapes01Remap.Read(exINI, pID, "PipShapes.Remap");

	this->IsJuggernaut.Read(exINI, pID, "IsJuggernaut");

	this->PlacementPreview_Show.Read(exINI, pSection, "PlacementPreview.Show");
	this->PlacementPreview_Shape.Read(exINI, pSection, "PlacementPreview.Shape");
	this->PlacementPreview_ShapeFrame.Read(exINI, pSection, "PlacementPreview.ShapeFrame");
	this->PlacementPreview_Offset.Read(exINI, pSection, "PlacementPreview.Offset");
	this->PlacementPreview_Remap.Read(exINI, pSection, "PlacementPreview.Remap");
	this->PlacementPreview_Palette.Read(exINI.GetINI(), pSection, "PlacementPreview.Palette");
	this->PlacementPreview_Transculency.Read(exINI, pSection, "PlacementPreview.TransculentLevel");

	this->TurretAnim_LowPower.Read(exINI, pSection, "TurretAnim.LowPower");
	this->TurretAnim_DamagedLowPower.Read(exINI, pSection, "TurretAnim.DamagedLowPower");

	this->Power_DegradeWithHealth.Read(exINI, pSection, "Power.DegradeWithHealth");
	this->AutoSellTime.Read(exINI, pSection, "AutoSell.Time");
	this->SolidHeight.Read(exINI, pSection, "SolidHeight");
}

void Otamaa::BTExt::ExtData::Read_Art(INI_EX& exINI, const char* pID)
{
	this->OnFireIndex.Read(exINI, pID, "OnFireIndex"); //not used ?

	this->HealthOnfire.Read(exINI, pID, "OnFire.Health");

	//Disabled : stability reason !
	this->DamageFire_Offs.Clear();

	char tempFire_OffsBuffer[32];
	for (int i = 0;; ++i)
	{
		Nullable<Point2D> nFire_offs;
		_snprintf_s(tempFire_OffsBuffer, sizeof(tempFire_OffsBuffer), "DamageFireOffset%d", i);
		nFire_offs.Read(exINI, pID, tempFire_OffsBuffer);

		if (!nFire_offs.isset() || nFire_offs.Get() == Point2D::Empty)
			break;

		this->DamageFire_Offs.AddItem(nFire_offs.Get());
	}

	//if (Phobos::Config::MoreDetailSLDebugLog)
	//	if (this->DamageFire_Offs.Count > 0)
	//		Debug::Log("DamageFireOffset for [%s] count [%d] \n", pID, this->DamageFire_Offs.Count);

	this->BuildUp_UseNormalLIght.Read(exINI, pID, "Buildup.UseNormalLight");
}