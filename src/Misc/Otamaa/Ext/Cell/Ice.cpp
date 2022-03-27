#include "Ice.h"
#include <MapClass.h>

IsometricTileType IceContainer::GetIceTile()
{
	auto nRandom = Random2Global(2);
	if (!nRandom)
		return IceContainer::IceSet[0];

	if(!(nRandom == 1))
		return IceContainer::IceSet[1];

	return IceContainer::IceSet[2];
}

void IceContainer::SetCellFlag
(
	    IsometricTileType nComp1,
		IsometricTileType nComp2,
		IsometricTileType nComp3,
		IsometricTileType nComp4,
		IsometricTileType nComp5,
		IsometricTileType nComp6
)
{
	for (auto i = Map.CellIteratorNext(); i; i = Map.CellIteratorNext())
	{
		auto nCurCellIso = i->IsoTileTypeIndex;
		if (i->IsIceGrowthAllowed &&nCurCellIso != nComp1 && nCurCellIso != nComp2 &&
			(nCurCellIso >= nComp3 && nCurCellIso < nComp4 || nCurCellIso >= nComp5 && nCurCellIso < nComp6))
		{
			i->Flags |= 0x40000u;
		}
	}
}


//TS LogicClass 
/*
  if ( MapClass_Ice_Growth_AI(&MapClass::Instance) ) //0520DB0
	sub_52C610(&MapClass::Instance);

	 MapClass_Cracked_Ice_AI(&MapClass::Instance); //520F40
*/