#include "Body.h"

void Otamaa::BlTExt::ExtData::Read_Rules(INI_EX& exINI, const char* pID)
{
	this->MissileROTVar.Read(exINI, pID, "MissileROTVar");
	this->MissileSafetyAltitude.Read(exINI, pID, "MissileSafetyAltitude");
	//this->StraightBulletDatas.Read(exINI, pID);
	//this->BulletProximityData.Read(exINI, pID, true);
	//this->MissileData.Read(exINI, pID);
}

void Otamaa::BlTExt::ExtData::Read_Art(INI_EX& exINI, const char* pID)
{
	this->Parachute.Read(exINI, pID, "Parachute");
	//this->Trails.Read(exINI, pID);
}