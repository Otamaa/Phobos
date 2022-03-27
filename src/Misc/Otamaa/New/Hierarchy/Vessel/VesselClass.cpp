#include "VesselClass.h"

/**
 *  Retrieves the class identifier (CLSID) of the object.
 *
 *  @author: CCHyper
 */
IFACEMETHODIMP VesselClass::GetClassID(CLSID* pClassID)
{
	if (pClassID == nullptr)
	{
		return E_POINTER;
	}

	*pClassID = __uuidof(this);

	return S_OK;
}


/**
 *  Initializes an object from the stream where it was saved previously.
 *
 *  @author: CCHyper
 *
 *  @param      pStm           An IStream pointer to the stream from which the object should be loaded.
 */
IFACEMETHODIMP VesselClass::Load(IStream* pStm)
{
	HRESULT hr = FootClass::Load(pStm);
	if (SUCCEEDED(hr))
	{
		// Insert any data to be loaded here.
	}

	return hr;
}


/**
 *  Saves an object to the specified stream.
 *
 *  @author: CCHyper
 *
 *  @param      pStm           An IStream pointer to the stream into which the object should be saved.
 *
 *  @param      fClearDirty    Indicates whether to clear the dirty flag after the save is complete.
 */
IFACEMETHODIMP VesselClass::Save(IStream* pStm, BOOL fClearDirty)
{
	HRESULT hr = FootClass::Save(pStm, fClearDirty);
	if (SUCCEEDED(hr))
	{
		// Insert any data to be saved here.
	}

	return hr;
}


/***********************************************************************************************
 * VesselClass::VesselClass -- Constructor for vessel class objects.                           *
 *                                                                                             *
 *    This is the normal constructor for vessel class objects. It will set up a vessel that    *
 *    is valid excepting that it won't be placed on the map.                                   *
 *                                                                                             *
 * INPUT:   classid  -- The type of vessel this will be.                                       *
 *                                                                                             *
 *          house    -- The owner of this vessel.                                              *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/14/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
VesselClass::VesselClass(const VesselTypeClass* classof, HouseClass* house) :
	FootClass(house),
	Class(classof)
{
	Create_ID();

	SecondaryFacing.Set(PrimaryFacing.Current());

	/**
	 *  Create an instance of the locomotor and link it to this object.
	 */
	if (Class)
	{
		ILocomotionPtr loco(Class->Locomotor);
		if (Locomotion != loco)
		{
			Locomotion = loco;
		}
		Locomotion->Link_To_Object(this);
	}

	if (Class)
	{
		PrimaryFacing.Set_ROT(Class->Rotation);
		SecondaryFacing.Set_ROT(Class->Rotation);

		/**
		 *  The ammo member is actually part of the techno class, but must be
		 *  initialized manually here because this is where we first have access
		 *  to the class pointer.
		 */
		Ammo = Class->MaxAmmo;

		/**
		 *  The techno class cloakabilty flag is set according to the type
		 *  class cloakability flag.
		 */
		IsCloakable = Class->IsCloakable;

		Strength = Class->MaxStrength;
	}

	if (House)
	{
		House->Tracking_Add(this);
	}

	Init();

	Vessels.Add(this);
	//    index_80E8C8.Add(Fetch_ID());
}


VesselClass::VesselClass(const NoInitClass& noinit) :
	FootClass(noinit)
{
}


/***********************************************************************************************
 * VesselClass::~VesselClass -- Destructor for vessel objects.                                 *
 *                                                                                             *
 *    The destructor will destroy the vessel and ensure that it is properly removed from the   *
 *    game engine.                                                                             *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/14/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
VesselClass::~VesselClass()
{
	if (GameActive && Class)
	{

		if (House->Can_Build(Class) == -1)
		{
			House->IsRecalcNeeded = true;
		}

		if (Team)
		{
			Team->Remove(this);
			Team = nullptr;
		}

		House->Tracking_Remove(this);

		while (Cargo.Is_Something_Attached())
		{
			delete Cargo.Detach_Object();
		}

		Limbo();
	}

	Detach_This_From_All(this);

	Vessels.Delete(this);
	//    index_80E8C8.Remove(Fetch_ID());


	IsActive = false;
}


void VesselClass::Detach(TARGET target, bool all)
{
	FootClass::Detach(target, all);
}


RTTIType VesselClass::Kind_Of() const
{
	//return RTTI_VESSEL;
	return RTTI_UNIT;       // TEMP!
}


int VesselClass::Size_Of(bool firestorm) const
{
	return sizeof(VesselClass);
}


void VesselClass::Compute_CRC(WWCRCEngine& crc) const
{
	FootClass::Compute_CRC(crc);
}


void VesselClass::AI()
{
	ASSERT(IsActive);

	FootClass::AI();
	if (!IsActive)
	{
		return;
	}
}


/***********************************************************************************************
 * VesselClass::Class_Of -- Fetches a reference to the vessel's class data.                    *
 *                                                                                             *
 *    This routine will return with a reference to the static class data for this vessel.      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a reference to the class data structure associated with this vessel.  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   03/14/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
ObjectTypeClass* const VesselClass::Class_Of() const
{
	ASSERT(IsActive);

	return (ObjectTypeClass* const)Class;
}


/***********************************************************************************************
 * VesselClass::Read_INI -- Read the vessel data from the INI database.                        *
 *                                                                                             *
 *    This will read and create all vessels specified in the INI database. This routine is     *
 *    called when the scenario starts.                                                         *
 *                                                                                             *
 * INPUT:   ini   -- Reference to the INI database to read the vessel data from.               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Vessels will be created and placed on the map by this function.                 *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void VesselClass::Read_INI(CCINIClass& ini)
{
#if 0
	VesselClass* vessel;        // Working vessel pointer.
	HousesType inhouse;         // Vessel house.
	VesselType classid;         // Vessel class.
	char buf[128];

	int len = ini.Entry_Count(INI_Name());
	for (int index = 0; index < len; index++)
	{
		const char* entry = ini.Get_Entry(INI_Name(), index);

		ini.Get_String(INI_Name(), entry, nullptr, buf, sizeof(buf));
		inhouse = HouseTypeClass::From_Name(std::strtok(buf, ","));
		if (inhouse != HOUSE_NONE)
		{
			classid = VesselTypeClass::From_Name(std::strtok(nullptr, ","));

			if (classid != VESSEL_NONE)
			{

				if (HouseClass::As_Pointer(inhouse) != nullptr)
				{
					vessel = new VesselClass(classid, inhouse);
					if (vessel != nullptr)
					{

						/**
						 *  Read the raw data.
						 */
						int strength = std::atoi(std::strtok(nullptr, ",\r\n"));
						Cell cell = std::atoi(std::strtok(nullptr, ",\r\n"));
						Coordinate coord = Cell_Coord(cell);
						DirType dir = (DirType)std::atoi(std::strtok(nullptr, ",\r\n"));
						MissionType mission = MissionClass::Mission_From_Name(std::strtok(nullptr, ",\n\r"));

						vessel->Trigger = nullptr;
						TriggerTypeClass* tp = TriggerTypeClass::From_Name(std::strtok(nullptr, ",\r\n"));
						if (tp != nullptr)
						{
							TriggerClass* tt = TriggerClass::Find_Or_Make(tp);
							if (tt != nullptr)
							{
								tt->AttachCount++;
								vessel->Trigger = tt;
							}
						}

						if (vessel->Unlimbo(coord, dir))
						{
							vessel->Strength = (int)vessel->Class->MaxStrength * fixed(strength, 256);
							if (vessel->Strength > vessel->Class->MaxStrength - 3) vessel->Strength = vessel->Class->MaxStrength;
							if (Session.Type == GAME_NORMAL || vessel->House->IsHuman)
							{
								vessel->Assign_Mission(mission);
								vessel->Commence();
							}
							else
							{
								vessel->Enter_Idle_Mode();
							}

						}
						else
						{

							/**
							 *  If the vessel could not be unlimbo'ed, then this
							 *  is a catastrophic error condition. Delete the vessel.
							 */
							delete vessel;
						}
					}
				}
			}
		}
	}
#endif
}


/***********************************************************************************************
 * VesselClass::Write_INI -- Write all vessel scenario data to the INI database.               *
 *                                                                                             *
 *    This routine is used to add the vessel data (needed for scenario start) to the INI       *
 *    database specified. If there was any preexisting vessel data in the database, it will    *
 *    be cleared                                                                               *
 *                                                                                             *
 * INPUT:   ini   -- Reference to the ini database to store the vessel data into.              *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/09/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
void VesselClass::Write_INI(CCINIClass& ini)
{
	/**
	 *  First, clear out all existing vessel data from the ini file.
	 */
	ini.Clear(INI_Name());

#if 0
	/**
	 *  Write the vessel data out.
	 */
	for (int index = 0; index < Vessels.Count(); ++index)
	{
		VesselClass* vessel = Vessels[index];
		if (vessel != nullptr && !vessel->IsInLimbo && vessel->IsActive)
		{
			char uname[10];
			char buf[128];

			std::snprintf(uname, sizeof(uname), "%d", index);
			std::snprintf(buf, sizeof(buf), "%s,%s,%d,%u,%d,%s,%s",
				vessel->House->Class->IniName,
				vessel->Class->IniName,
				vessel->Health_Ratio() * 256,
				Coord_Cell(vessel->Coord),
				vessel->PrimaryFacing.Current(),
				MissionClass::Mission_Name(vessel->Mission),
				vessel->Trigger ? vessel->Trigger->Class->IniName : "None"
			);
			ini.Put_String(INI_Name(), uname, buf);
		}
	}
#endif
}