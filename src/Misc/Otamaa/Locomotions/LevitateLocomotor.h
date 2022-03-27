#pragma once

#include <LocomotionClass.h>
#include <Utilities/Debug.h>
#include <YRPPGlobal.h>


// Auto generated !
//{EA976E75-4C99-438c-91E6-EC6B6C7A7AF6}
// EBE80B85-EED2-4DEF-92CA-BC0C99AF4A1E
class DECLSPEC_UUID("EA976E75-4C99-438c-91E6-EC6B6C7A7AF6")
	TestLocomotionClass : public LocomotionClass
{
public:
	/**
	 *  IUnknown
	 */
	virtual HRESULT __stdcall QueryInterface(REFIID riid, LPVOID * ppvObj);
	virtual ULONG __stdcall AddRef();
	virtual ULONG __stdcall Release();

	/**
	 *  IPersist
	 */
	virtual HRESULT __stdcall GetClassID(CLSID * pClassID);

	/**
	 *  IPersistStream
	 */
	virtual LONG __stdcall IsDirty();
	virtual HRESULT __stdcall Load(IStream * pStm);
	virtual HRESULT __stdcall Save(IStream * pStm, BOOL fClearDirty);
	virtual LONG __stdcall GetSizeMax(ULARGE_INTEGER * pcbSize);

	/**
	 *  ILocomotion
	 */
	virtual HRESULT __stdcall Link_To_Object(void* object);
	virtual bool __stdcall Is_Moving();
	virtual CoordStruct* __stdcall Destination(CoordStruct * pcoord);
	virtual CoordStruct* __stdcall Head_To_Coord(CoordStruct * pcoord);
	virtual Move __stdcall Can_Enter_Cell(CellStruct CellStruct);
	virtual bool __stdcall Is_To_Have_Shadow();
	virtual Matrix3D __stdcall Draw_Matrix(int* key);
	virtual Matrix3D __stdcall Shadow_Matrix(int* key);
	virtual Point2D __stdcall Draw_Point();
	virtual Point2D __stdcall Shadow_Point();
	virtual VisualType __stdcall Visual_Character(bool flag);
	virtual int __stdcall Z_Adjust();
	virtual ZGradient __stdcall Z_Gradient();
	virtual bool __stdcall Process();
	virtual void __stdcall Move_To(CoordStruct to);
	virtual void __stdcall Stop_Moving();
	virtual void __stdcall Do_Turn(DirStruct coord);
	virtual void __stdcall Unlimbo();
	virtual void __stdcall Tilt_Pitch_AI();
	virtual bool __stdcall Power_On();
	virtual bool __stdcall Power_Off();
	virtual bool __stdcall Is_Powered();
	virtual bool __stdcall Is_Ion_Sensitive();
	virtual bool __stdcall Push(DirStruct dir);
	virtual bool __stdcall Shove(DirStruct dir);
	virtual void __stdcall Force_Track(int track, CoordStruct coord);
	virtual Layer __stdcall In_Which_Layer();
	virtual void __stdcall Force_Immediate_Destination(CoordStruct coord);
	virtual void __stdcall Force_New_Slope(int ramp);
	virtual bool __stdcall Is_Moving_Now();
	virtual int __stdcall Apparent_Speed();
	virtual int __stdcall Drawing_Code();
	virtual FireError __stdcall Can_Fire();
	virtual int __stdcall Get_Status();
	virtual void __stdcall Acquire_Hunter_Seeker_Target();
	virtual bool __stdcall Is_Surfacing();
	virtual void __stdcall Mark_All_Occupation_Bits(int mark);
	virtual bool __stdcall Is_Moving_Here(CoordStruct to);
	virtual bool __stdcall Will_Jump_Tracks();
	virtual bool __stdcall Is_Really_Moving_Now();
	virtual void __stdcall Stop_Movement_Animation();
	virtual void __stdcall Clear_Coords();
	virtual void __stdcall Lock();
	virtual void __stdcall Unlock();
	virtual int __stdcall Get_Track_Number();
	virtual int __stdcall Get_Track_Index();
	virtual int __stdcall Get_Speed_Accum();

	//
	void ProcessHovering();
public:
	virtual int Size() override { return sizeof(*this); }

public:
	TestLocomotionClass() :
		LocomotionClass {},
		State { 0 },
		field_18 { 0.0 },
		field_20 { 0.0 },
		field_28 { 0.0 },
		field_30 { 0.0 },
		field_38 { 0.0 },
		field_40 { 0 },
		field_44 { 0 },
		field_48 { 0.0 },
		__Gravity_HoverDampen { 0.0 },
		field_58 { 0.0 }

	{ };
	~TestLocomotionClass() { };

	int State; // State?
	double field_18; // CurrentVelocity?
	double field_20; // DeltaX?
	double field_28; // DeltaY?
	double field_30; // AccelerationDurationCosinus?
	double field_38; // AccelerationDurationNegSinus?
	int field_40; // AccelerationDuration?
	int field_44; // BlocksCounter?
	double field_48; // CurrentSpeed?
	double __Gravity_HoverDampen; // Dampen? 50
	double field_58;

};