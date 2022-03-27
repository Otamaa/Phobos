#include "Body.h"
//#include <Misc/Otamaa/Misc/DynamicPatcher/Trails/TrailType.h>

void Otamaa::RExt::ExtData::Read_LoadBeforeTypeData(INI_EX& exINI)
{
	//Debug::Log("Reading [TrailType] \n");
	//TrailType::LoadFromINIList(&CCINIClass::INI_Art.get(), true);

	NukeWarheadName.Read(exINI.GetINI(), "SpecialWeapons", "NukeWarhead");
	AI_AutoSellHealthRatio.Read(exINI, "General", "AI.AutoSellHealthRatio");
	Building_PlacementPreview.Read(exINI, "AudioVisual", "ShowBuildingPlacementPreview");
	PlacementGrid_TranslucentLevel.Read(exINI, "AudioVisual", "BuildingPlacementGrid.TranslucentLevel");
	DisablePathfindFailureLog.Read(exINI, "General", "DisablePathfindFailureLog");
	//MyPutData.Read(exINI, "General");
}

void Otamaa::RExt::ExtData::Read_LoadAfterTypeData(INI_EX& exINI)
{
	VeinholeParticle.Read(exINI, "AudioVisual", "VeinholeSpawnParticleType");
}