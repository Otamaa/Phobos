#pragma once
#include <StringTable.h>
#include <CCINIClass.h>
#include <CellSpread.h>
#include <StopWatch.h>

#include <Helpers/Iterators.h>
#include <Helpers/Enumerators.h>

#include <Utilities/Debug.h>
#include <Utilities/Constructs.h>

#include <string.h>
#include <iterator>
#include <vector>
#include <YRMath.h>

#define MIN(x) std::numeric_limits<x>::min()
#define MAX(x) std::numeric_limits<x>::max()

class GeneralUtils
{
public:
	static bool IsValidString(const char* str);
	static void IntValidCheck(int* source, const char* section, const char* tag, int defaultValue, int min = MIN(int), int max = MAX(int));
	static void DoubleValidCheck(double* source, const char* section, const char* tag, double defaultValue, double min = MIN(double), double max = MAX(double));
	static const wchar_t* LoadStringOrDefault(const char* key, const wchar_t* defaultValue);
	static const wchar_t* LoadStringUnlessMissing(const char* key, const wchar_t* defaultValue);
	static std::vector<CellStruct> AdjacentCellsInRange(unsigned int range);
	static const int GetRangedRandomOrSingleValue(Point2D range);
	static const double GetWarheadVersusArmor(WarheadTypeClass* pWH, Armor ArmorType);

	static int ChooseOneWeighted(const double dice, const std::vector<int>* weights);
	static const int GetAnimIndexFromFacing(FootClass* pFoot, int nVectorSize);

	static const int ScaleF2I(float value, int scale)
	{
		value = std::clamp(value, 0.0f, 1.0f);
		return Game::F2I(value * scale);
	}


	//Point2Dir
	static const DirStruct Desired_Facing(int x1, int y1, int x2, int y2)
	{
		DirStruct dir;
		unsigned short value = (short)Game::F2I((Math::atan2((double)(y2 - y1), (double)(x2 - x1)) - Math::deg2rad(-(360.0 / (USHRT_MAX - 1)))));
		dir.value(value);
		return dir;
	}

	static const DirStruct Desired_Facing(const Point2D &point1, const Point2D &point2)
	{
		return Desired_Facing(point1.X, point1.Y, point2.X, point2.Y);
	}

	static const DirStruct Coord2DirSTruct(CoordStruct Loc1, CoordStruct Loc2)
	{
		auto angle = Math::atan2((double)(Loc2.X - Loc1.X), (double)(Loc2.Y - Loc1.Y));
		auto theta = angle * (180 / Math::Pi);

		return DirStruct(theta);
	}

	static Leptons PixelToLeptons(int pixel)
	{ return Leptons((((pixel * 256) + (48 / 2) - ((pixel < 0) ? (48 - 1) : 0)) / 48)); }

	static Leptons DistanceToLeptons(int distance)
	{ return Leptons(distance * 256); }
};

struct StopwatchLogger
{
	Stopwatch watch;
	const char* func;
	const char* name;

	StopwatchLogger(const char* pFunc = nullptr, const char* pName = nullptr) : func(pFunc), name(pName) {}

	~StopwatchLogger()
	{
		Debug::Log("STOPWATCH %s (%s): %lld\n", func, name, watch.get_nano().QuadPart);
	}
};
