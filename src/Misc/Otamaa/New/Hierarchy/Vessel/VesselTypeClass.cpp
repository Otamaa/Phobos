#include "VesselTypeClass.h"

#include <PhobosGlobals.h>
#include <YRPPGlobal.h>

IFACEMETHODIMP VesselTypeClass::Load(IStream* pStm)
{
	HRESULT hr = TechnoTypeClass::Load(pStm);
	if (SUCCEEDED(hr))
	{
		// Insert any data to be loaded here.
	}

	return hr;
}

IFACEMETHODIMP VesselTypeClass::Save(IStream* pStm, BOOL fClearDirty)
{
	HRESULT hr = TechnoTypeClass::Save(pStm, fClearDirty);
	if (SUCCEEDED(hr))
	{
		// Insert any data to be saved here.
	}

	return hr;
}

VesselTypeClass::VesselTypeClass(const char* ini_name) :
	TechnoTypeClass(ini_name, SpeedType::Float)
{
	Create_ID();

	VesselTypes.AddItem(this);
}

VesselTypeClass::~VesselTypeClass()
{
	Detach_This_From_All(this);

	VesselTypes.Remove(this);
}

AbstractType VesselTypeClass::What_Am_I() const
{
	return AbstractType(RTTI_VESSELTYPE);
}

int VesselTypeClass::Size() const
{
	return sizeof(VesselTypeClass);
}

void VesselTypeClass::CalculateChecksum(WWCRCEngine& crc) const
{
	TechnoTypeClass::CalculateChecksum(crc);
}

int VesselTypeClass::GetArrayIndex() const
{
	TechnoTypeClass::GetArrayIndex();
}

int VesselTypeClass::Fetch_ID() const
{
	return int(Type);
}

bool VesselTypeClass::LoadFromINI(CCINIClass* ini)
{
	return !TechnoTypeClass::LoadFromINI(ini);
}

CoordStruct* VesselTypeClass::vt_entry_6C(CoordStruct* pRet, CoordStruct* pSrc) const
{
	auto nBuffs= CoordStruct::Empty;
	return &nBuffs;
}

void VesselTypeClass::PixelDimensions(Point3D* pDest) const
{
	auto nBuffs = Point3D::Empty;
	pDest = &nBuffs;
}

void VesselTypeClass::Dimension2(CoordStruct* pDest)
{
	auto nBuffs = CoordStruct::Empty;
	pDest = &nBuffs;
}

bool VesselTypeClass::SpawnAtMapCoords(CellStruct* pMapCoords, HouseClass* pOwner)
{
	auto pVessel = GameCreate<VesselClass>(this, pOwner);
	if (pVessel)
	{
		return pVessel->Unlimbo(CellClass::Cell2Coord(*pMapCoords), ScenarioGlobal->Random(7));
	}
	GameDelete(pVessel);
	return false;
}

ObjectClass* const VesselTypeClass::CreateObject(HouseClass* house)
{
	return static_cast<ObjectClass*>(GameCreate<VesselClass>(this, house));
}

const Point3D VesselTypeClass::TurretAdjust(Direction dir, Point3D& pos) const
{
	Point3D newpos = Point3D::Empty;

	return newpos;
}

const VesselTypeClass& VesselTypeClass::As_Reference(VesselType type)
{
//	if(type != VESSEL_NONE && type < VesselTypes.Count);
		return *VesselTypes[type];

}

const VesselTypeClass* VesselTypeClass::As_Pointer(VesselType type)
{
	//TSPP_ASSERT(type != VESSEL_NONE && type < VesselTypes.Count());
	return type != VESSEL_NONE && type < VesselTypes.Count ? VesselTypes[type] : nullptr;
}

const VesselTypeClass& VesselTypeClass::As_Reference(const char* name)
{
	return As_Reference(From_Name(name));
}


const VesselTypeClass* VesselTypeClass::As_Pointer(const char* name)
{
	return As_Pointer(From_Name(name));
}

VesselType VesselTypeClass::From_Name(const char* name)
{
	
	if (!_strcmpi(name, "<none>") || !_strcmpi(name, "none"))
	{
		return VESSEL_NONE;
	}

	if (name != nullptr)
	{
		for (VesselType index = VESSEL_FIRST; index < VesselTypes.Count; ++index)
		{
			if (!_strcmpi(As_Reference(index).Name(), name))
			{
				return index;
			}
		}
	}

	return VESSEL_NONE;
}

const char* VesselTypeClass::Name_From(VesselType type)
{
	return (type != VESSEL_NONE && type < VesselTypes.Count ? As_Reference(type).Name() : "<none>");
}

bool RulesClassFake::Ships(CCINIClass& ini)
{
	static const char* const SHIPTYPES = "ShipTypes";

	char buf[128];
	const VesselTypeClass* vesseltype;

	int counter = ini.Entry_Count(SHIPTYPES);
	for (int index = 0; index < counter; ++index)
	{
		const char* entry = ini.Get_Entry(SHIPTYPES, index);

		/**
		 *  Get a weapon entry.
		 */
		if (ini.Get_String(SHIPTYPES, entry, buf, sizeof(buf)))
		{

			/**
			 *  Find or create a vessel of the name specified.
			 */
			vesseltype = VesselTypeClass::Find_Or_Make(buf);
			if (vesseltype)
			{
#ifndef NDEBUG
				DEV_DEBUG_INFO("Rules: Found VesselType \"%s\".\n", buf);
#endif
			}
			else
			{
				DEV_DEBUG_WARNING("Rules: Error processing VesselType \"%s\"!\n", buf);
			}
		}

	}

	return counter > 0;
}

/**
 *  Fetch all the object characteristic values.
 *
 *  @author: CCHyper
 */
void RulesClassExtension::Objects(CCINIClass& ini)
{
	ASSERT(ThisPtr != nullptr);
	//EXT_DEBUG_TRACE("RulesClassExtension::Objects - 0x%08X\n", (uintptr_t)(ThisPtr));

	/**
	 *  Fetch the game object values from the rules file.
	 */
	for (int index = 0; index < VesselTypes.Count(); ++index)
	{
		VesselTypes[index]->Read_INI(ini);
	}
}

#endif

for (VesselType index = VESSEL_FIRST; index < VesselTypes.Count(); ++index)
{
	VesselTypeClass const& vessel_type = VesselTypeClass::As_Reference(index);
	if (vessel_type.Get_Ownable() /*&& unit_type.Level != -1*/)
	{
		VesselClass* vessel = (VesselClass*)vessel_type.Create_One_Of(PlayerPtr);
		if (vessel)
		{

			attempt = origin;

			while (attempt.Y < map_cell_bottom)
			{
				if (Try_Unlimbo(vessel, attempt))
				{
					DEBUG_INFO("VesselType %s spawned at %d,%d.\n", vessel_type.Name(), attempt.X, attempt.Y);
					break;
				}
			}
		}
	}
}

#if 0