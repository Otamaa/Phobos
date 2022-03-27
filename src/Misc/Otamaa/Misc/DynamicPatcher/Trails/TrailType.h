#pragma once

#include <Utilities/Enumerable.h>
#include <Utilities/Template.h>
#include <Utilities/TemplateDef.h>
#include <Utilities/GeneralUtils.h>
#include <Utilities/Enum.h>
#include "../Helpers/EffectHelpers.h"

enum TrailMode : int
{
	NONE = 0,
	LASER = 1,
	ELECTIRIC = 2,
	BEAM = 3,
	PARTICLE = 4,
	ANIM = 5
};

class TrailType final : public Enumerable<TrailType>
{
public:

	TrailMode Mode;
	Valueable<int> Distance;
	Valueable<bool> IgnoreVertical;
	Valueable<int> InitialDelay;
	Valueable<bool> HideWhenCloak;

	//Mode : Anim
	struct AnimTrailType
	{
		AnimTrailType() :
			StartDrivingAnim { nullptr }
			, WhileDrivingAnim { nullptr }
			, StopDrivingAnim { nullptr }
		{ };

		Valueable<AnimTypeClass*> StartDrivingAnim;
		Valueable<AnimTypeClass*> WhileDrivingAnim;
		Valueable<AnimTypeClass*> StopDrivingAnim;

	} AnimTrailType;

	//Mode : Beam
	BeamType BeamTrailType;

	//Mode : Ebolt
	BoltType BoltTrailType;

	//Mode : Laser
	LaserType LaserTrailType;

	//Mode : Particle
	struct ParticleTrailType
	{
		ParticleTrailType()
			:ParticleSystem { nullptr }
		{ };

		Valueable<ParticleSystemTypeClass*> ParticleSystem;

	}ParticleTrailType ;

	TrailType(const char* const pTitle) : Enumerable<TrailType>(pTitle)
		, Mode { TrailMode::LASER }
		, Distance { 64 }
		, IgnoreVertical { false }
		, InitialDelay { 0 }

		//
		, AnimTrailType { }
		, BeamTrailType { RadBeamType::Eruption }
		, BoltTrailType { }
		, LaserTrailType { true }
		, ParticleTrailType { }
		, HideWhenCloak { true }

	{ }

	virtual ~TrailType() override = default;
	virtual void LoadFromINI(CCINIClass* pINI) override;
	virtual void LoadFromStream(PhobosStreamReader& Stm) override;
	virtual void SaveToStream(PhobosStreamWriter& Stm) override;

private:
	template <typename T>
	void Serialize(T& Stm)
	{
		Stm
			.Process(Mode)
			.Process(Distance)
			.Process(IgnoreVertical)
			.Process(InitialDelay)
			.Process(AnimTrailType)
			.Process(BeamTrailType)
			.Process(BoltTrailType)
			.Process(LaserTrailType)
			.Process(ParticleTrailType)
			.Process(HideWhenCloak)
			;

	}
};