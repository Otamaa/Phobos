#include "TrailType.h"

Enumerable<TrailType>::container_t Enumerable<TrailType>::Array;

const char* Enumerable<TrailType>::GetMainSection()
{
	return "TrailTypes";
}

void TrailType::LoadFromINI(CCINIClass* pINI)
{
	const char* pSection = this->Name;

	INI_EX exINI(pINI);

	pINI->ReadString(pSection, "Mode","LASER", Phobos::readBuffer, sizeof(Phobos::readBuffer));

	if (!_strcmpi(Phobos::readBuffer, "NONE"))
		Mode = TrailMode::NONE;
	else if (!_strcmpi(Phobos::readBuffer, "LASER"))
		Mode = TrailMode::LASER;
	else if (!_strcmpi(Phobos::readBuffer, "ELECTIRIC"))
		Mode = TrailMode::ELECTIRIC;
	else if (!_strcmpi(Phobos::readBuffer, "BEAM"))
		Mode = TrailMode::BEAM;
	else if (!_strcmpi(Phobos::readBuffer, "PARTICLE"))
		Mode = TrailMode::PARTICLE;
	else if (!_strcmpi(Phobos::readBuffer, "ANIM"))
		Mode = TrailMode::ANIM;

	switch (Mode)
	{
	case NONE:
		return;
		break;
	case LASER: //working
	{

		LaserTrailType.Duration.Read(exINI, pSection, "Laser.Duration");
		LaserTrailType.IsHouseColor.Read(exINI, pSection, "Laser.IsHouseColor");
		LaserTrailType.IsSupported.Read(exINI, pSection, "Laser.IsSupported");
		LaserTrailType.Fade.Read(exINI, pSection, "Laser.Fade");
		LaserTrailType.InnerColor.Read(exINI, pSection, "Laser.InnerColor");
		LaserTrailType.OuterColor.Read(exINI, pSection, "Laser.OuterColor");
		LaserTrailType.OuterSpread.Read(exINI, pSection, "Laser.OuterSpread");
		LaserTrailType.Thickness.Read(exINI, pSection, "Laser.Thickness");

	}
		break;
	case ELECTIRIC: //working
	{
			BoltTrailType.IsAlternateColor.Read(exINI, pSection, "Bolt.IsAlternateColor");
			BoltTrailType.Color1.Read(exINI, pSection, "Bolt.Color1");
			BoltTrailType.Color2.Read(exINI, pSection, "Bolt.Color2");
			BoltTrailType.Color3.Read(exINI, pSection, "Bolt.Color3");
			BoltTrailType.ParticleSystem.Read(exINI, pSection, "Bolt.ParticleSystem",true);
			BoltTrailType.ParticleSystem_coordFlip.Read(exINI, pSection, "Bolt.ParticleSytemCoordFlip");
	}
		break;
	case BEAM://working
	{
		BeamTrailType.BeamColor.Read(exINI, pSection, "Beam.Color");
		BeamTrailType.Period.Read(exINI, pSection, "Beam.Period");
		BeamTrailType.Amplitude.Read(exINI, pSection, "Beam.Amplitude");
	}
		break;
	case PARTICLE: //notworking
	{
		ParticleTrailType.ParticleSystem.Read(exINI, pSection, "ParticleSystem",true);
	}
		break;
	case ANIM: //working
	{
		AnimTrailType.StartDrivingAnim.Read(exINI, pSection, "Anim.Start", true);
		AnimTrailType.WhileDrivingAnim.Read(exINI, pSection, "Anim.While", true);
		AnimTrailType.StopDrivingAnim.Read(exINI, pSection, "Anim.Stop", true);
	}
		break;
	}

	Distance.Read(exINI, pSection, "Distance");
	IgnoreVertical.Read(exINI, pSection, "IgnoreVertical");
	InitialDelay.Read(exINI, pSection, "InitialDelay");
	HideWhenCloak.Read(exINI, pSection, "HideWhenCloak");
}

void TrailType::LoadFromStream(PhobosStreamReader& Stm) { Debug::Log("Loading Element From TrailType ! \n"); this->Serialize(Stm); }
void TrailType::SaveToStream(PhobosStreamWriter& Stm) { this->Serialize(Stm); }