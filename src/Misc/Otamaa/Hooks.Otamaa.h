#pragma once

#include <TechnoClass.h>
#include <FootClass.h>

#include <FileFormats/VXL.h>
#include <Utilities/Macro.h>
#include <Utilities/Debug.h>
#include <Surface.h>

#include "Ext/CaptureManager/Body.h"
#include "New/Type/ArmorTypeClass.h"
//#include "New/Type/MouseCursorTypeClass.h"
#include <Utilities/EnumFunctions.h>
//#include <Misc/Otamaa/Delegates.h>
#include <Ext/RadSite/Body.h>

#include <AnimClass.h>
#include <BuildingClass.h>
#include <TechnoClass.h>
#include <FootClass.h>
#include <UnitClass.h>
#include <ScenarioClass.h>
#include <VoxelAnimClass.h>
#include <BulletClass.h>
#include <TunnelLocomotionClass.h>
#include <JumpjetLocomotionClass.h>
#include <VeinholeMonsterClass.h>
#include <HoverLocomotionClass.h>
#include <SmudgeTypeClass.h>
#include <TerrainTypeClass.h>
#include <SlaveManagerClass.h>

#include <Ext/House/Body.h>
#include <Ext/Building/Body.h>
#include <Ext/BulletType/Body.h>
#include <Ext/Anim/Body.h>
#include <Ext/AnimType/Body.h>
#include <Ext/Bullet/Body.h>
#include <Ext/Rules/Body.h>
#include <Ext/Techno/Body.h>
#include <Ext/TechnoType/Body.h>
#include <Ext/BuildingType/Body.h>
#include <Ext/WeaponType/Body.h>
#include <Ext/WarheadType/Body.h>
#include <Ext/SWType/Body.h>
#include <Ext/Scenario/Body.h>
#include <Utilities/Macro.h>
#include <Utilities/Debug.h>
#include <Utilities/TemplateDef.h>
#include <Utilities/GeneralUtils.h>

#include <BitFont.h>
#include <cassert>
#include <mutex>
#include <thread>
#include <chrono>
#include <iostream>
#include <string>
#include <string_view>
#include <functional>

struct VoxelShadowDrawStruct //SomeVoxelCache
{
	VoxLib* LibPtr;
	int HeaderIndex;
	DWORD InfoIndex;
	Vector3D<float> v0;
	Vector3D<float> v1;
	Vector3D<float> v2;
	Vector3D<float> v3;
	Surface* SurfacePtr;
	Point2D ShadowPoint;
};

static_assert(sizeof(VoxelShadowDrawStruct) == 0x48);//

//extern PhobosMap<int, int> NonMarbleArr;

class AnimClassCopy final : public AnimClass
{
public:

	AnimClass* _AnimClass_CTOR(AnimTypeClass* pType, CoordStruct* pCoord, int nLoopDelay,
		int nLoopCount, DWORD nflags, int nForceZAdjust, bool nReverse)
	{
		JMP_THIS(0x421EA0);
	}
};

class VoxelAnimClassCopy final : public VoxelAnimClass
{
public:
	VoxelAnimClass* VoxelAnimClass_CTOR(VoxelAnimTypeClass* pVoxelAnimType, CoordStruct* pLocation, HouseClass* pOwnerHouse)
	{ JMP_THIS(0x7493B0); }
};

class BounceClassCopy final : public BounceClass
{
public:
	void BounceClassInit_Init(CoordStruct* pCoord, double Els, double Grav, double maxVel, Vector3D<float>* pVel, double aVel)
	{ JMP_THIS(0x4397E0); }
};

/* need to be inside the function/inline , otherwise the result is not right !
		auto const end = 0x100 / 4;
		DWORD lea_nData;
		_asm {mov lea_nData, esp};
		auto const* const mem = reinterpret_cast<DWORD*>(static_cast<DWORD>(lea_nData + static_cast<DWORD>(0x0)));

		for (auto i = 0u; i < end; ++i)
		{
			Debug::LogUnflushed("esp+%04X = %08X\n", i * 4, mem[i]);
		}
		Debug::Log("Phobos::Dumping[ProjectileMotion_Exec] Done.\n"); // flushes

*/

template<class T>
class Queue
{
public:

	Queue(const std::initializer_list<T>& i) :elem { i } { }
	Queue() = default;

	std::size_t size() const {
		return elem.size();
	}

	bool empty() const {
		return elem.empty();
	}

	void enqueue(T&&) {
		elem.push_back(t);
	}

	void dequeue() {
		elem.erase(elem.begin());
	}

	T peek() const {
		return elem.front();
	}

	T& operator[](std::size_t i) {
		return elem[i];
	}

	const T& operator[](std::size_t i) const {
		return elem[i];
	}

	typename std::vector <T>::iterator begin() {
		return elem.begin();
	}

	typename std::vector <T>::iterator end()	{
		return elem.end();
	}

private:
	std::vector<T> elem;
};