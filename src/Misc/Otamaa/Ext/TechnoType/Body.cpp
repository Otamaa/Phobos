#include "Body.h"

void Otamaa::TTyExt::ExtDataB::Read_Rules(TechnoTypeClass* pThis ,INI_EX& exINI, const char* pID)
{
	auto pSection = pID;
	auto pINI = exINI.GetINI();

	this->DontShake.Read(exINI, pSection, "DontShakeScreen");
	this->CrateType.Read(exINI, pSection, "CrateType");

	this->HoverLocoIdx.Read(exINI, pSection, "HoverType");

	this->DiskLaserChargeUp.Read(exINI, pSection, "DiskLaserChargeUp");
	this->DrainAnimationType.Read(exINI, pSection, "DrainAnimationType");

	this->TalkBubbleTime.Read(exINI, pSection, "TalkBubbleTime");

	if (auto pAircraftType = specific_cast<AircraftTypeClass*>(pThis))
	{
		this->SpyplaneCameraSound.Read(exINI, pSection, "SpyplaneCameraSound");
		this->ParadropRadius.Read(exINI, pSection, "Paradrop.ApproachRadius");
		this->ParadropOverflRadius.Read(exINI, pSection, "Paradrop.OverflyRadius");
		this->Paradrop_DropPassangers.Read(exINI, pSection, "Paradrop.DropPassangers");
		this->Paradrop_MaxAttempt.Read(exINI, pSection, "Paradrop.MaxApproachAttempt");

		this->IsCustomMissile.Read(exINI, pSection, "Missile.Custom");
		this->CustomMissileData.Read(exINI, pSection, "Missile");
		this->CustomMissileData.GetEx()->Type = pAircraftType;
		this->CustomMissileEliteRaise.Read(exINI, pSection, "Missile.EliteRaiseBeforeLaunching");
		this->CustomMissileRaise.Read(exINI, pSection, "Missile.RaiseBeforeLaunching");
		this->AttackingAircraftSightRange.Read(exINI, pSection, "AttackingAircraftSightRange");
		//this->MyDiveData.Read(exINI, pSection);
		//this->MuPutData.Read(exINI, pSection);
	}

	//pipshape
	this->HealthBarSHP.Read(exINI, pSection, "HealthBarSHP");

	//pipbar
	this->HealthBarSHP_Selected.Read(exINI, pSection, "HealthBarSHP.Selected");
	this->HealthBarSHPBracketOffset.Read(exINI, pSection, "HealthBarSHP.BracketOffset");
	this->HealthBarSHP_HealthFrame.Read(exINI, pSection, "HealthBarSHP.HealthFrame");
	this->HealthBarSHP_Palette.Read(pINI, pSection, "HealthBarSHP.Palette");
	this->HealthBarSHP_PointOffset.Read(exINI, pSection, "HealthBarSHP.Point2DOffset");
	this->HealthbarRemap.Read(exINI, pSection, "HealthBarSHP.Remap");

	this->PipShapes02.Read(exINI, pSection, "PipShapes.Foot");
	this->PipGarrison.Read(exINI, pSection, "PipShapes.Garrison");
	this->PipGarrison_FrameIndex.Read(exINI, pSection, "PipShapes.GarrisonFrameIndex");
	this->PipGarrison_Palette.Read(pINI, pSection, "PipShapes.GarrisonPalette");
	this->PipShapes01.Read(exINI, pSection, "PipShapes.Building");

	this->Is_Cow.Read(exINI, pSection, "IsCow");

	this->HealthNumber_SHP.Read(exINI, pSection, "HealthNumber.Shape");
	this->HealthNumber_Show.Read(exINI, pSection, "HealthNumber.Show");
	this->HealthNumber_Percent.Read(exINI, pSection, "HealthNumber.Percent");
	this->Healnumber_Offset.Read(exINI, pSection, "HealthNumber.Offset");
	this->Healnumber_Decrement.Read(exINI, pSection, "HealthNumber.Decrement");

	this->ParasiteExit_Sound.Read(exINI, pSection, "Parasite.ExitSound");

	this->Overload_Count.Read(exINI, pSection, "Overload.Count");
	this->Overload_Damage.Read(exINI, pSection, "Overload.Damage");
	this->Overload_Frames.Read(exINI, pSection, "Overload.Frames");
	this->Overload_DeathSound.Read(exINI, pSection, "Overload.DeathSound");
	this->Overload_ParticleSys.Read(exINI, pSection, "Overload.ParticleSys");
	this->Overload_ParticleSysCount.Read(exINI, pSection, "Overload.ParticleSysCount");

	this->Landing_Anim.Read(exINI, pSection, "Landing.Anim");
	this->Landing_AnimOnWater.Read(exINI, pSection, "Landing.AnimOnWater");

	this->FacingRotation_Disable.Read(exINI, pSection, "FacingRotation.Disabled");
	this->FacingRotation_DisalbeOnEMP.Read(exINI, pSection, "FacingRotation.DisabledOnEMP");
	this->FacingRotation_DisalbeOnDeactivated.Read(exINI, pSection, "FacingRotation.DisabledOnDeactivated");

	this->JumpJet_HeightOffset.Read(exINI, pSection, "Jumpjet.HeightOffset");
	this->Draw_MindControlLink.Read(exINI, pSection, "MindControll.DrawLink");

	this->CrashWeapon.Read(exINI, pSection, "Crash.Weapon", true);
	this->Disable_C4WarheadExp.Read(exINI, pSection, "Crash.DisableC4WarheadExplosion");

	this->Cloak_WhenMove.Read(exINI, pSection, "Cloak.OnlyWhenMoving");
	this->GClock_Shape.Read(exINI, pSection, "GClock.Shape");
	this->GClock_Transculency.Read(exINI, pSection, "GClock.Transculency");
	this->GClock_Palette.Read(pINI, pSection, "GClock.Palette");

	this->CanBeAutoTargetedBy.Read(exINI, pSection, "CanBeAutoTargetedBy");
	this->ROF_Random.Read(exINI, pSection, "ROF.AddRandom");
	this->Rof_RandomMinMax.Read(exINI, pSection, "ROF.RandomMinMax");

	this->TankDisguiseAsTank.Read(exINI, pSection, "TankDisguiseAsTank");
	this->DisguiseDisAllowed.Read(exINI, pSection, "Disguise.DisAllowed");

	this->CrashSpinLevelRate.Read(exINI, pSection, "CrashSpin.LevelRate");
	this->CrashSpinVerticalRate.Read(exINI, pSection, "CrashSpin.VerticalRate");

	//this->SWFireData.Read(exINI, pSection);
	this->VirtualUnit.Read(exINI, pSection, "VirtualUnit");

	//this->MySpawnSupportDatas.Read(exINI, pSection);
	//this->MyPassangersData.Read(exINI, pSection);
	this->MyGiftBoxData.Read(exINI, pSection);
	//this->MyJJData.Read(exINI, pSection);
}

void Otamaa::TTyExt::ExtDataB::Read_Art(TechnoTypeClass* pThis, INI_EX& exINI, const char* pID)
{
	char HitCoord_tempBuffer[32];
	for (size_t i = 0; ; ++i)
	{
		Nullable<CoordStruct> nHitBuff;
		_snprintf_s(HitCoord_tempBuffer, sizeof(HitCoord_tempBuffer), "HitCoordOffset%d", i);
		nHitBuff.Read(exINI, pID, HitCoord_tempBuffer);

		if (!nHitBuff.isset() || nHitBuff.Get() == CoordStruct::Empty)
			break;

		this->HitCoordOffset.emplace_back(nHitBuff.Get());
	}

	this->HitCoordOffset_Random.Read(exINI, pID, "HitCoordOffset.Random");
	this->CrawlFLHData.Read(exINI, pID);
	//this->Trails.Read(exINI, pID,true);
	//this->MySpawnSupportFLH.Read(exINI, pID);
}

void Otamaa::TTyExt::ExtDataB::Init(TechnoTypeClass* pThis)
{
	if (!pThis)
		return;

	Is_Cow = !strcmp(pThis->ID, "COW");
}