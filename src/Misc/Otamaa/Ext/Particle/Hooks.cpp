#include "Body.h"

// Init in constructor because yes - Morton
//DEFINE_HOOK(0x62BB13, ParticleClass_CTOR_SetLaserTrail, 0x5)
//{
//	GET(ParticleClass*, pThis, ESI);
//	ParticleExt::InitializeLaserTrails(pThis);
//	return 0;
//}

DEFINE_HOOK(0x62CE86, ParticleClass_AI, 0x7) //this is the end, here's the beginning: 0x62CE49 0x6
{
	GET(ParticleClass*, pThis, ESI);
	auto pParticleExt = ParticleExt::ExtMap.Find(pThis);

	if (!pParticleExt)
		return 0;

	// TODO: Check this - Morton
	// LaserTrails update routine is in BulletClass::AI hook because BulletClass::Draw
	// doesn't run when the object is off-screen which leads to visual bugs - Kerbiter
	if (pParticleExt->LaserTrails.size())
	{
		CoordStruct location = pThis->GetCoords();
		CoordStruct drawnCoords = location;
		for (auto const& trail : pParticleExt->LaserTrails)
		{
			// Left this here for now - Morton
			// We insert initial position so the first frame of trail doesn't get skipped - Kerbiter
			// TODO move hack to BulletClass creation
			if (!trail->LastLocation.isset())
				trail->LastLocation = location;

			trail->Update(drawnCoords);
			trail->Visible = pThis->IsOnMyView();
		}

	}

	return 0;
}