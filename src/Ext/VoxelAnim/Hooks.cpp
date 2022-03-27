#include "Body.h"

#include <Ext/VoxelAnimType/Body.h>

DEFINE_HOOK(0x74A70E, VoxelAnimClass_AI_Additional, 0xC)
{
	GET(VoxelAnimClass* const, pThis, EBX);

	auto const pThisExt = VoxelAnimExt::ExtMap.Find(pThis);

	if (!pThisExt->LaserTrails.empty())
	{
		CoordStruct location = pThis->GetCoords();
		CoordStruct drawnCoords = location;

		for (auto const& trail : pThisExt->LaserTrails)
		{
			if (!trail->LastLocation.isset())
				trail->LastLocation = location;

			trail->Visible = pThis->IsVisible;
			trail->Update(drawnCoords);
		}
	}

	//TrailsManager::AI(pThis);
	return 0;
}