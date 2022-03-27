#include "LevitateLocomotor.h"

/**
*  Retrieves pointers to the supported interfaces on an object.
*
*  @author: CCHyper
*/
IFACEMETHODIMP TestLocomotionClass::QueryInterface(REFIID riid, LPVOID *ppvObj)
{
	Debug::Log(__FUNCTION__" Called !  \n");
	return LocomotionClass::QueryInterface(riid, ppvObj);
}


/**
 *  Increments the reference count for an interface pointer to a COM object.
 *
 *  @author: CCHyper
 */
IFACEMETHODIMP_(ULONG) TestLocomotionClass::AddRef()
{
	Debug::Log(__FUNCTION__" Called !  \n");
	return LocomotionClass::AddRef();
}


/**
 *  Decrements the reference count for an interface on a COM object.
 *
 *  @author: CCHyper
 */
IFACEMETHODIMP_(ULONG) TestLocomotionClass::Release()
{
	Debug::Log(__FUNCTION__" Called !  \n");
	return LocomotionClass::Release();
}


/**
 *  Determines whether an object has changed since it was last saved to its stream.
 *
 *  @author: CCHyper
 */
IFACEMETHODIMP_(LONG) TestLocomotionClass::IsDirty()
{
	Debug::Log(__FUNCTION__" Called !  \n");
	return LocomotionClass::IsDirty();
}


/**
 *  Retrieves the class identifier (CLSID) of the object.
 *
 *  @author: CCHyper
 */
IFACEMETHODIMP TestLocomotionClass::GetClassID(CLSID *pClassID)
{
	Debug::Log(__FUNCTION__" Called !  \n");
	if (pClassID == nullptr) {
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
IFACEMETHODIMP TestLocomotionClass::Load(IStream *pStm)
{
	Debug::Log(__FUNCTION__" Called !  \n");
	HRESULT hr = LocomotionClass::Load(pStm);
	if (SUCCEEDED(hr)) {
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
IFACEMETHODIMP TestLocomotionClass::Save(IStream *pStm, BOOL fClearDirty)
{
	Debug::Log(__FUNCTION__" Called !  \n");
	HRESULT hr = LocomotionClass::Save(pStm, fClearDirty);
	if (SUCCEEDED(hr)) {
		// Insert any data to be saved here.
	}

	return hr;
}


/**
 *  Retrieves the size of the stream needed to save the object.
 *
 *  @author: CCHyper
 */
IFACEMETHODIMP_(LONG) TestLocomotionClass::GetSizeMax(ULARGE_INTEGER *pcbSize)
{
	Debug::Log(__FUNCTION__" Called !  \n");
	if (pcbSize == nullptr) {
		return E_POINTER;
	}

	LocomotionClass::GetSizeMax(pcbSize);

	return S_OK;
}


/**
 *  Links object to locomotor.
 *
 *  @author: CCHyper
 */
IFACEMETHODIMP TestLocomotionClass::Link_To_Object(void *object)
{
	Debug::Log(__FUNCTION__" Called !  \n");
	HRESULT hr = LocomotionClass::Link_To_Object(object);

	if (SUCCEEDED(hr)) {
		Debug::Log("TestLocomotionClass - Sucessfully linked to \"%s\"\n", Owner->get_ID());
	}

	return hr;
}

/**
 *  Sees if object is moving.
 *
 *  @author: CCHyper
 */
IFACEMETHODIMP_(bool) TestLocomotionClass::Is_Moving()
{
	Debug::Log(__FUNCTION__" Called !  \n");
	return RefCount != 0;
}

/**
 *  Fetches destination CoordStruct.
 *
 *  @author: CCHyper
 */
IFACEMETHODIMP_(CoordStruct*) TestLocomotionClass::Destination(CoordStruct* pcoord)
{
	Debug::Log(__FUNCTION__" Called !  \n");
	return pcoord;
}

/**
 *  Fetches immediate (next cell) destination CoordStruct.
 *
 *  @author: CCHyper
 */
IFACEMETHODIMP_(CoordStruct*) TestLocomotionClass::Head_To_Coord(CoordStruct* pcoord)
{
	Debug::Log(__FUNCTION__" Called !  \n");
	LinkedTo->GetCoords(pcoord);
	return pcoord;
}

/**
 *  Determine if specific cell can be entered.
 *
 *  @author: CCHyper
 */
IFACEMETHODIMP_(Move) TestLocomotionClass::Can_Enter_Cell(CellStruct cell)
{
	Debug::Log(__FUNCTION__" Called !  \n");
	/**
	 *  Query the linked object to determine if the cell can be entered.
	 */
	return LinkedTo->IsCellOccupied(Map[cell], -1, -1, nullptr, false);
}

/**
 *  Should object cast a shadow?
 *
 *  @author: CCHyper
 */
IFACEMETHODIMP_(bool) TestLocomotionClass::Is_To_Have_Shadow()
{
	Debug::Log(__FUNCTION__" Called !  \n");
	return LocomotionClass::Is_To_Have_Shadow();
}

/**
 *  Fetch voxel draw matrix.
 *
 *  @author: CCHyper
 */
IFACEMETHODIMP_(Matrix3D) TestLocomotionClass::Draw_Matrix(int *key)
{
	Debug::Log(__FUNCTION__" Called !  \n");
	Matrix3D nBuff;
	LocomotionClass::Draw_Matrix(&nBuff, key);
	return nBuff;
}

/**
 *  Fetch shadow draw matrix.
 *
 *  @author: CCHyper
 */
IFACEMETHODIMP_(Matrix3D) TestLocomotionClass::Shadow_Matrix(int *key)
{
	Debug::Log(__FUNCTION__" Called !  \n");
	Matrix3D nBuff;
	LocomotionClass::Shadow_Matrix(&nBuff, key);
	return nBuff;
}

/**
 *  Draw point center location.
 *
 *  @author: CCHyper
 */
IFACEMETHODIMP_(Point2D) TestLocomotionClass::Draw_Point()
{
	Debug::Log(__FUNCTION__" Called !  \n");
	Point2D nBuffer;
	LocomotionClass::Draw_Point(&nBuffer);
	return nBuffer;
}

/**
 *  Shadow draw point center location.
 *
 *  @author: CCHyper
 */
IFACEMETHODIMP_(Point2D) TestLocomotionClass::Shadow_Point()
{
	Debug::Log(__FUNCTION__" Called !  \n");
	Point2D nBuffer;
	LocomotionClass::Shadow_Point(&nBuffer);
	return nBuffer;
}

/**
 *  Visual character for drawing.
 *
 *  @author: CCHyper
 */
IFACEMETHODIMP_(VisualType) TestLocomotionClass::Visual_Character(bool flag)
{
	Debug::Log(__FUNCTION__" Called !  \n");
	return VisualType::Normal;
}

/**
 *  Z adjust control value.
 *
 *  @author: CCHyper
 */
IFACEMETHODIMP_(int) TestLocomotionClass::Z_Adjust()
{
	Debug::Log(__FUNCTION__" Called !  \n");
	return 0;
}

/**
 *  Z gradient control value.
 *
 *  @author: CCHyper
 */
IFACEMETHODIMP_(ZGradient) TestLocomotionClass::Z_Gradient()
{
	Debug::Log(__FUNCTION__" Called !  \n");
	return ZGradient::Deg90;
}

void TestLocomotionClass::ProcessHovering()
{
	auto nHeight = 0;
	auto nCoord = LinkedTo->GetCoords();

	if (LinkedTo->PathDirections[0] != -1)
	{
		auto nZ = Map.GetCellFloorHeight(nCoord);
		auto nAdjent = LinkedTo->PathDirections[0] & 7;
		auto nAdjentCoord = Unsorted::AdjacentCoord[nAdjent];
		auto nDestCoord = nCoord + CoordStruct{ nAdjentCoord.X,nAdjentCoord.Y,0 };
		if (Map.GetCellFloorHeight(nDestCoord) > nZ)
		{
			nHeight -= RulesGlobal->HoverHeight;
		}
		else
		{
			nHeight = LinkedTo->GetHeight();
		}
	}

	auto nDampen = nHeight + __Gravity_HoverDampen;
	auto pTechno = abstract_cast<TechnoClass*>(LinkedTo);
	auto pTechWhat = (int)LinkedTo->WhatAmI();
	double dMult = pTechno ? 1.1 :1.0;
	auto nFrame = Unsorted::CurrentFrame + 2 * pTechWhat;
	auto nHoverBob = dMult * RulesGlobal->HoverBob* 900.0;
	auto nVal2 = (nFrame % Game::F2I(nHoverBob)) * 6.283185307179586 / nHoverBob;
	auto nVal3 = Math::sin(nVal2);
	int nDampenResult =	nVal3 + nVal3 + nDampen;

	if (nDampenResult < 0)
	{
		__Gravity_HoverDampen = 0.0;
		nDampenResult = 0;
	}

	auto bIsOnMap = LinkedTo->IsOnMap;
	LinkedTo->IsOnMap = false;
	LinkedTo->SetHeight(nDampenResult);
	LinkedTo->IsOnMap = bIsOnMap;
	
	if (nHeight < RulesGlobal->HoverHeight)
	{
		if (Is_Powered())
		{
			auto nDoubleHeight = RulesGlobal->HoverHeight * 2;
			auto nHeight1 = nDoubleHeight - nHeight;
			auto nVal4 = nHeight1 / RulesGlobal->HoverHeight * RulesGlobal->Gravity + __Gravity_HoverDampen;
			__Gravity_HoverDampen = nVal4;
		}

		if (nHeight < (RulesGlobal->HoverHeight / 4))
		{
			__Gravity_HoverDampen = (RulesGlobal->Gravity / 3) + __Gravity_HoverDampen;
		}
	}

	auto v17 = __Gravity_HoverDampen - RulesGlobal->Gravity;
	__Gravity_HoverDampen = v17;
	__Gravity_HoverDampen = v17 * RulesGlobal->HoverDampen;
}

IFACEMETHODIMP_(bool) TestLocomotionClass::Process()
{
	Debug::Log(__FUNCTION__" Called !  \n");

	switch (RefCount)
	{
	case 0u:
		break;
	case 1u:
		break;
	case 2u:
		break;
	case 3u:
		break;
	case 4u:
		break;
	case 5u:
		break;
	case 6u:
		break;
	default:
		break;
	}

	if (Is_Moving())
	{
		if (!(Unsorted::CurrentFrame % 10))
		{
			if (!LinkedTo->IsOnBridge() && LinkedTo->GetCell()->LandType == LandType::Water)
			{
				if (auto pAnimType = RulesGlobal->Wake)
				{
					auto nCoord = LinkedTo->GetCoords();
					GameCreate<AnimClass>(pAnimType, nCoord);
				}
			}
		}
	}

	ProcessHovering();
	return IsDirty();
}


/**
 *  Instruct to move to location specified.
 *
 *  @author: CCHyper
 */
IFACEMETHODIMP_(void) TestLocomotionClass::Move_To(CoordStruct to)
{
	Debug::Log(__FUNCTION__" Called !  \n");
}


/**
 *  Stop moving at first opportunity.
 *
 *  @author: CCHyper
 */
IFACEMETHODIMP_(void) TestLocomotionClass::Stop_Moving()
{
	Debug::Log(__FUNCTION__" Called !  \n");
}


/**
 *  Try to face direction specified.
 *
 *  @author: CCHyper
 */
IFACEMETHODIMP_(void) TestLocomotionClass::Do_Turn(DirStruct coord)
{
	Debug::Log(__FUNCTION__" Called !  \n");
	LinkedTo->PrimaryFacing.set(coord);
}


/**
 *  Object is appearing in the world.
 *
 *  @author: CCHyper
 */
IFACEMETHODIMP_(void) TestLocomotionClass::Unlimbo()
{
	Debug::Log(__FUNCTION__" Called !  \n");
	/**
	 *  Set the objects ramp for redraw.
	 */
	Force_New_Slope(LinkedTo->GetCell()->RedrawCountMAYBE);
}


/**
 *  Special tilting AI function.
 *
 *  @author: CCHyper
 */
IFACEMETHODIMP_(void) TestLocomotionClass::Tilt_Pitch_AI()
{
	Debug::Log(__FUNCTION__" Called !  \n");
}


/**
 *  Locomotor becomes powered.
 *
 *  @author: CCHyper
 */
IFACEMETHODIMP_(bool) TestLocomotionClass::Power_On()
{
	Debug::Log(__FUNCTION__" Called !  \n");
	return LocomotionClass::Power_On();
}


/**
 *  Locomotor becomes powered.
 *
 *  @author: CCHyper
 */
IFACEMETHODIMP_(bool) TestLocomotionClass::Power_Off()
{
	Debug::Log(__FUNCTION__" Called !  \n");
	return LocomotionClass::Power_Off();
}


/**
 *  Is locomotor powered?
 *
 *  @author: CCHyper
 */
IFACEMETHODIMP_(bool) TestLocomotionClass::Is_Powered()
{
	Debug::Log(__FUNCTION__" Called !  \n");
	return LocomotionClass::Is_Powered();
}


/**
 *  Is locomotor sensitive to ion storms?
 *
 *  @author: CCHyper
 */
IFACEMETHODIMP_(bool) TestLocomotionClass::Is_Ion_Sensitive()
{
	Debug::Log(__FUNCTION__" Called !  \n");
	return false;
}


/**
 *  Push object in direction specified.
 *
 *  @author: CCHyper
 */
IFACEMETHODIMP_(bool) TestLocomotionClass::Push(DirStruct dir)
{
	Debug::Log(__FUNCTION__" Called !  \n");
	return false;
}


/**
 *  Shove object (with spin) in direction specified.
 *
 *  @author: CCHyper
 */
IFACEMETHODIMP_(bool) TestLocomotionClass::Shove(DirStruct dir)
{
	Debug::Log(__FUNCTION__" Called !  \n");
	return false;
}


/**
 *  Force drive track -- special case only.
 *
 *  @author: CCHyper
 */
IFACEMETHODIMP_(void) TestLocomotionClass::Force_Track(int track, CoordStruct coord)
{
	Debug::Log(__FUNCTION__" Called !  \n");
}


/**
 *  What display layer is it located in.
 *
 *  @author: CCHyper
 */
IFACEMETHODIMP_(Layer) TestLocomotionClass::In_Which_Layer()
{
	Debug::Log(__FUNCTION__" Called !  \n");
	return Layer::Ground;
}


/**
 *  Force object to destination (no processing).
 *
 *  @author: CCHyper
 */
IFACEMETHODIMP_(void) TestLocomotionClass::Force_Immediate_Destination(CoordStruct coord)
{
	Debug::Log(__FUNCTION__" Called !  \n");
}


/**
 *  Force a voxel unit to a given slope. Used in cratering.
 *
 *  @author: CCHyper
 */
IFACEMETHODIMP_(void) TestLocomotionClass::Force_New_Slope(int ramp)
{
	Debug::Log(__FUNCTION__" Called !  \n");
}


/**
 *  Is it actually moving across the ground this very second?
 *
 *  @author: CCHyper
 */
IFACEMETHODIMP_(bool) TestLocomotionClass::Is_Moving_Now()
{
	return RefCount !=0;
}


/**
 *  Actual current speed of object expressed as leptons per game frame.
 *
 *  @author: CCHyper
 */
IFACEMETHODIMP_(int) TestLocomotionClass::Apparent_Speed()
{
	Debug::Log(__FUNCTION__" Called !  \n");
	return LinkedTo->GetCurrentSpeed();
}


/**
 *  Special drawing feedback code (locomotor specific meaning).
 *
 *  @author: CCHyper
 */
IFACEMETHODIMP_(int) TestLocomotionClass::Drawing_Code()
{
	Debug::Log(__FUNCTION__" Called !  \n");
	return 0;
}


/**
 *  Queries if any locomotor specific state prevents the object from firing.
 *
 *  @author: CCHyper
 */
IFACEMETHODIMP_(FireError) TestLocomotionClass::Can_Fire()
{
	Debug::Log(__FUNCTION__" Called !  \n");
	return FireError::OK;
}


/**
 *  Queries the general state of the locomotor.
 *
 *  @author: CCHyper
 */
IFACEMETHODIMP_(int) TestLocomotionClass::Get_Status()
{
	Debug::Log(__FUNCTION__" Called !  \n");
	return 0;
}


/**
 *  Forces a hunter seeker droid to find a target.
 *
 *  @author: CCHyper
 */
IFACEMETHODIMP_(void) TestLocomotionClass::Acquire_Hunter_Seeker_Target()
{
	Debug::Log(__FUNCTION__" Called !  \n");
}


/**
 *  Is this object surfacing?
 *
 *  @author: CCHyper
 */
IFACEMETHODIMP_(bool) TestLocomotionClass::Is_Surfacing()
{
	Debug::Log(__FUNCTION__" Called !  \n");
	return false;
}


/**
 *  Lifts all occupation bits associated with the object off the map.
 *
 *  @author: CCHyper
 */
IFACEMETHODIMP_(void) TestLocomotionClass::Mark_All_Occupation_Bits(int mark)
{
	Debug::Log(__FUNCTION__" Called !  \n");
	auto headto = Head_To_Coord(nullptr);
	if (mark != 0) {
		LinkedTo->MarkAllOccupationBits(*headto);
	}
	else {
		LinkedTo->MarkAllOccupationBits(*headto);
	}
}


/**
 *  Is this object in the process of moving into this coord.
 *
 *  @author: CCHyper
 */
IFACEMETHODIMP_(bool) TestLocomotionClass::Is_Moving_Here(CoordStruct to)
{
	Debug::Log(__FUNCTION__" Called !  \n");
	auto nBuff = CellClass::Coord2Cell(*Head_To_Coord(nullptr));
	CoordStruct headto_cell{ nBuff.X , nBuff.Y ,0 };
	return nBuff.X == headto_cell.X && nBuff.Y == headto_cell.Y && std::abs(headto_cell.Z - to.Z) <= Unsorted::CellHeight;
}


/**
 *  Will this object jump tracks?
 *
 *  @author: CCHyper
 */
IFACEMETHODIMP_(bool) TestLocomotionClass::Will_Jump_Tracks()
{
	Debug::Log(__FUNCTION__" Called !  \n");
	return false;
}


/**
 *  Infantry moving query function.
 *
 *  @author: CCHyper
 */
IFACEMETHODIMP_(bool) TestLocomotionClass::Is_Really_Moving_Now()
{
	Debug::Log(__FUNCTION__" Called !  \n");
	return Is_Moving();
}


/**
 *  Falsifies the IsReallyMoving flag in WalkLocomotionClass.
 *
 *  @author: CCHyper
 */
IFACEMETHODIMP_(void) TestLocomotionClass::Stop_Movement_Animation()
{
	Debug::Log(__FUNCTION__" Called !  \n");
}


/**
 *  Locks the locomotor from being deleted.
 *
 *  @author: CCHyper
 */
IFACEMETHODIMP_(void) TestLocomotionClass::Lock()
{
	Debug::Log(__FUNCTION__" Called !  \n");
}


/**
 *  Unlocks the locomotor from being deleted.
 *
 *  @author: CCHyper
 */
IFACEMETHODIMP_(void) TestLocomotionClass::Unlock()
{
	Debug::Log(__FUNCTION__" Called !  \n");
}


/**
 *  Queries internal variables.
 *
 *  @author: CCHyper
 */
IFACEMETHODIMP_(int) TestLocomotionClass::Get_Track_Number()
{
	Debug::Log(__FUNCTION__" Called !  \n");
	return -1;
}


/**
 *  Queries internal variables.
 *
 *  @author: CCHyper
 */
IFACEMETHODIMP_(int) TestLocomotionClass::Get_Track_Index()
{
	Debug::Log(__FUNCTION__" Called !  \n");
	return -1;
}


/**
 *  Queries internal variables.
 *
 *  @author: CCHyper
 */
IFACEMETHODIMP_(int) TestLocomotionClass::Get_Speed_Accum()
{
	Debug::Log(__FUNCTION__" Called !  \n");
	return -1;
}

/**
 *  Clear Coords.
 *
 *  @author: Otamaa
 */
IFACEMETHODIMP_(void) TestLocomotionClass::Clear_Coords()
{
	Debug::Log(__FUNCTION__" Called !  \n");
}