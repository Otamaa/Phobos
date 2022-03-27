#pragma once
#include "Body.h"

class IceContainer
{
public:

	//	static void InitOneTime();
	static IsometricTileType GetIceTile();
	static void SetCellFlag
	(
		IsometricTileType nComp1,
		IsometricTileType nComp2,
		IsometricTileType nComp3,
		IsometricTileType nComp4,
		IsometricTileType nComp5,
		IsometricTileType nComp6
	);

	//member
	static IsometricTileType IceSet[3];
};