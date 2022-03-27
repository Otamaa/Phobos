#pragma once

#include "Trails.h"
#include <IsometricTileTypeClass.h>
#include <memory>

class ObjectClass;
struct TrailData
{
	int CurrentType;
	ValueableVector<LandType> OnLand;
	ValueableVector<TileType> OnTileTypes;
	CoordStruct FLHs;
	bool Onturrents;

	// { trail.Get(),land,nTiles,flh.Get(),isOnTurret.Get(false) };

	TrailData(int cur, ValueableVector<LandType> nOnLand, ValueableVector<TileType> nOnTileTypes, CoordStruct flh, bool nTur) :
		CurrentType { cur }
		, OnLand { nOnLand }
		, OnTileTypes { nOnTileTypes }
		, FLHs { flh }
		, Onturrents { nTur }
	{ }

	TrailData() :
		CurrentType { }
		, OnLand {  }
		, OnTileTypes {  }
		, FLHs {  }
		, Onturrents {  }
	{ }

	bool Load(PhobosStreamReader& Stm, bool RegisterForChange)
	{ Debug::Log("Loading Element From TrailData ! \n");  return Serialize(Stm); }

	bool Save(PhobosStreamWriter& Stm)
	{ return Serialize(Stm); }

private:
	template <typename T>
	bool Serialize(T& Stm)
	{
		return Stm
			.Process(CurrentType)
			.Process(OnLand)
			.Process(OnTileTypes)
			.Process(FLHs)
			.Process(Onturrents)
			.Success()
			;
	}
};

//managing the vector
class TrailsManager
{
public:

	static void Construct(ObjectClass* pOwner, bool IsConverted = false);
	static void AI(ObjectClass* pOwner);
	static void Hide(ObjectClass* pOwner);
	static void CleanUp(ObjectClass* pOwner);

};

//reading
struct TrailsReader
{
	ValueableVector<TrailData> CurrentData;

	explicit TrailsReader() { };

	void Read(INI_EX& nParser, const char* pSection, bool IsTechno = false)
	{
		char tempBuffer[32];

		if (TrailType::Array.empty())
			return;
		size_t nTotal = 0;
		for (size_t i = 0; ; ++i)
		{
			NullableIdx <TrailType> trail;
			_snprintf_s(tempBuffer, sizeof(tempBuffer), "Trail%d.Type", i);
			trail.Read(nParser, pSection, tempBuffer);

			if (!trail.isset() || trail == -1)
				break;

			Valueable<CoordStruct> flh;
			_snprintf_s(tempBuffer, sizeof(tempBuffer), "Trail%d.FLH", i);
			flh.Read(nParser, pSection, tempBuffer);

			Nullable<bool> isOnTurret;
			if (IsTechno)
			{
				_snprintf_s(tempBuffer, sizeof(tempBuffer), "Trail%d.IsOnTurret", i);
				isOnTurret.Read(nParser, pSection, tempBuffer);
			}

			ValueableVector<LandType> land;
			_snprintf_s(tempBuffer, sizeof(tempBuffer), "Trail%d.OnLands", i);
			land.Read(nParser, pSection, tempBuffer);

			ValueableVector<TileType> nTiles;
			_snprintf_s(tempBuffer, sizeof(tempBuffer), "Trail%d.OnTiles", i);
			nTiles.Read(nParser, pSection, tempBuffer);

			CurrentData.push_back({ trail.Get(),land,nTiles,flh.Get(),isOnTurret.Get(false) });
			++nTotal;
		}

		if (nTotal > 0)
			CurrentData.resize(nTotal);
		else
			CurrentData.clear();
	}

	bool Load(PhobosStreamReader& Stm, bool RegisterForChange)
	{ Debug::Log("Loading Element From TrailManager ! \n"); return Serialize(Stm); }

	bool Save(PhobosStreamWriter& Stm)
	{ return Serialize(Stm); }

private:
	template <typename T>
	bool Serialize(T& Stm)
	{
		return Stm
			.Process(CurrentData)
			.Success()
			;
	}

};