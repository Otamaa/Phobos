#pragma once

#include <GeneralStructures.h>
#include <LaserDrawClass.h>
#include <HouseClass.h>

#include <New/Type/LaserTrailTypeClass.h>

#include <vector>

class LaserTrailClass
{
public:
	LaserTrailTypeClass* Type;
	bool Visible;
	CoordStruct FLH;
	bool IsOnTurret;
	ColorStruct CurrentColor;
	Nullable<CoordStruct> LastLocation;
	TimerStruct InitialDelayTimer;
	bool CanDraw;
	int InitialDelay;

	LaserTrailClass(LaserTrailTypeClass* pTrailType, ColorStruct nHouseColor,
		CoordStruct flh = { 0, 0, 0 }, bool isOnTurret = false) :
		Type(pTrailType)
		, Visible(true)
		, FLH(flh)
		, IsOnTurret(isOnTurret)
		, CurrentColor((pTrailType->IsHouseColor.Get() && (nHouseColor != ColorStruct::Empty)) ? nHouseColor : pTrailType->Color)
		, LastLocation()
		, CanDraw(false)
		, InitialDelay(pTrailType->InitialDelay.Get())
		, InitialDelayTimer()
	{ }

	LaserTrailClass() :
		Type(),
		Visible(),
		FLH(),
		IsOnTurret(),
		CurrentColor(),
		LastLocation()
		, CanDraw()
		, InitialDelay()
		, InitialDelayTimer()
	{ }

	bool Update(CoordStruct location);
	void FixZLoc(bool forWho);

	bool Load(PhobosStreamReader& stm, bool registerForChange);
	bool Save(PhobosStreamWriter& stm) const;

private:
	template <typename T>
	bool Serialize(T& stm);

	bool AllowDraw(CoordStruct location)
	{
		return this->Visible && (this->Type->IgnoreVertical ?
		  (abs(location.X - this->LastLocation.Get().X) > 16 || abs(location.Y - this->LastLocation.Get().Y) > 16) : true) && IsInitialDelayFinish();
	}

	bool IsInitialDelayFinish()
	{
		if (!CanDraw)
		{
			if (InitialDelay > 0)
			{
				InitialDelayTimer.Start(InitialDelay);
				InitialDelay = 0;
			}

			CanDraw = InitialDelayTimer.Expired();
		}

		return CanDraw;
	}

};

template <>
struct Savegame::ObjectFactory<LaserTrailClass>
{
	std::unique_ptr<LaserTrailClass> operator() (PhobosStreamReader& Stm) const
	{
		return std::make_unique<LaserTrailClass>();
	}
};