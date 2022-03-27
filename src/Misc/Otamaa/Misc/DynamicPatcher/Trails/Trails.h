#pragma once
#include <CoordStruct.h>
#include <GeneralStructures.h>
#include <CellClass.h>
#include "TrailType.h"

#include <Misc/Otamaa/Misc/DynamicPatcher/Techno/DriveData/DriveData.h>
#include "../Helpers/EffectHelpers.h"

class TrailsManager;
class UniversalTrail
{
public:

	Nullable<CoordStruct> LastLocation;
	bool canDraw;
	int initialDelay;
	TimerStruct DelayTimer;
	bool forceDraw;
	DrivingState drivingState;
	TrailType* Type;
	CoordStruct FLH;
	bool IsOnTurret;
	std::vector<LandType> OnLandTypes;
	std::vector<TileType> OnTileTypes;

	UniversalTrail(TrailType* type ,CoordStruct flh, bool onturret) :
		Type { type }
		, LastLocation { CoordStruct::Empty }
		, DelayTimer { }
		, FLH { flh }
		, IsOnTurret { onturret }
		, initialDelay { type->InitialDelay > 0 ? type->InitialDelay:0 }
		, canDraw { !(type->InitialDelay > 0) }
		, forceDraw { false }
		, drivingState { DrivingState::Moving }
	{ }

	UniversalTrail() :
		Type {}
		, LastLocation {}
		, DelayTimer {}
		, FLH {}
		, IsOnTurret {}
		, canDraw {}
		, initialDelay {}
		, forceDraw {}
		, drivingState {}
	{ }

	~UniversalTrail() = default;

	void ClearLastLocation()
	{
		LastLocation.Reset();
	}

	void SetDrivingState(DrivingState state)
	{
		drivingState = state;
		if (state == DrivingState::Stop)
		{
			forceDraw = true;
		}
	}

	void RealDrawTrail(CoordStruct sourcePos, CoordStruct targetPos, HouseClass* pHouse)
	{
		switch (Type->Mode)
		{
		case TrailMode::LASER:

			ColorStruct houseColor = { 0,0,0 };
			if (pHouse && Type->LaserTrailType.IsHouseColor)
				houseColor = pHouse->LaserColor;

			EffectHelpers::DrawLine(sourcePos, targetPos, Type->LaserTrailType, houseColor);
			break;
		case TrailMode::ELECTIRIC:
			EffectHelpers::DrawBolt(sourcePos, targetPos, Type->BoltTrailType);
			break;
		case TrailMode::BEAM:
			EffectHelpers::DrawBeam(sourcePos, targetPos, Type->BeamTrailType);
			break;
		case TrailMode::PARTICLE:
			EffectHelpers::DrawParticle(pHouse,sourcePos, targetPos, Type->ParticleTrailType.ParticleSystem.Get());
			break;
		case TrailMode::ANIM:
			DrawAnimTrail(sourcePos, pHouse);
			break;
		}
	}

	void DrawAnimTrail(CoordStruct sourcePos, HouseClass* pHouse)
	{
		auto animType = Type->AnimTrailType.WhileDrivingAnim;

		switch (drivingState)
		{
		case DrivingState::Start:
			animType = Type->AnimTrailType.StartDrivingAnim;
			break;
		case DrivingState::Stop:
			animType = Type->AnimTrailType.StopDrivingAnim;
			break;
		}

		if (animType)
			if (auto pAnim = GameCreate<AnimClass>(animType, sourcePos))
				pAnim->Owner = pHouse;
	}

	void DrawTrail(HouseClass* pHouse, CoordStruct sourcePos, CoordStruct createOffset = CoordStruct::Empty)
	{
		if (sourcePos != CoordStruct::Empty)
		{
			if (LastLocation.Get(CoordStruct::Empty) != CoordStruct::Empty)
			{
				CoordStruct targetPos = LastLocation;
				int distance = Type->Distance;

				if (sourcePos.DistanceFromI(targetPos) > distance || forceDraw)
				{
					if ((CanDraw() && CheckVertical(sourcePos, targetPos)) || forceDraw)
					{
						forceDraw = false;
						if (IsOnLand(sourcePos))
						{
							RealDrawTrail(sourcePos, targetPos, pHouse);
						}
						drivingState = DrivingState::Moving;
					}
					LastLocation = sourcePos;
				}
			}
			else
			{
				LastLocation = sourcePos - createOffset;
			}
		}
	}

private:

	bool CanDraw()
	{
		if (!canDraw)
		{
			if (initialDelay > 0)
			{
				DelayTimer.Start(initialDelay);
				initialDelay = 0;
			}
			canDraw = DelayTimer.Expired();
		}
		return canDraw;
	}

	bool CheckVertical(CoordStruct sourcePos, CoordStruct targetPos)
	{
		return (
			Type->IgnoreVertical ?
			(abs(sourcePos.X - targetPos.X) > 32 || abs(sourcePos.Y - targetPos.Y) > 32) : true);
	}

	bool IsOnLand(CoordStruct sourcePos)
	{
		if (!OnLandTypes.empty() && OnLandTypes.size() > 0)
		{
			if (auto pCell = MapClass::Instance->TryGetCellAt(sourcePos))
			{
				LandType landType = pCell->LandType;
				auto nITer = make_iterator(OnLandTypes);
				if (nITer.contains(landType))
				{
					if (!OnTileTypes.empty() && OnTileTypes.size() > 0)
					{
						bool Allow = true;
						for (auto const& nTile : OnTileTypes)
						{
							Allow |= (pCell)->TileIs(nTile);
						}

						return Allow;
					}

					return true;
				}
			}
		}
		return true;
	}
public:
	bool Load(PhobosStreamReader& Stm, bool RegisterForChange)
	{ Debug::Log("Loading Element From UniversalTrail ! \n"); return Serialize(Stm); }

	bool Save(PhobosStreamWriter& Stm)
	{ return Serialize(Stm); }

private:
	template <typename T>
	bool Serialize(T& Stm)
	{
		return Stm
		.Process(LastLocation)
		.Process(canDraw)
		.Process(initialDelay)
		.Process(DelayTimer)
		.Process(forceDraw)
		.Process(drivingState)
		.Process(Type)
		.Process(FLH)
		.Process(IsOnTurret)
		.Process(OnLandTypes)
		.Process(OnTileTypes)
		.Success()
			;
	}
};