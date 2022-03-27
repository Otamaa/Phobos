#pragma once

#include "Vessel.Def.h"

 /**
  *  This specifies the constant attribute data associated with naval vessels.
  */
class DECLSPEC_UUID(CLSID_VESSEL_TYPE)
	VesselTypeClass : public TechnoTypeClass
{
public:

	//IUnknown
	virtual HRESULT __stdcall QueryInterface(REFIID iid, void** ppvObject) R0;
	virtual ULONG __stdcall AddRef() R0;
	virtual ULONG __stdcall Release() R0;

	//IPersist
	IFACEMETHOD(GetClassID)(CLSID * pClassID);

	//IPersistStream
	IFACEMETHOD(IsDirty)() R0;
	IFACEMETHOD(Load)(IStream * pStm);
	IFACEMETHOD(Save)(IStream * pStm, BOOL fClearDirty);
	IFACEMETHOD(GetSizeMax)(ULARGE_INTEGER * pcbSize);

	//IRTTITypeInfo
	virtual AbstractType __stdcall What_Am_I() const override;
	virtual int __stdcall Fetch_ID() const override;

	//INoticeSink
	virtual bool __stdcall INoticeSink_Unknown(DWORD dwUnknown) R0;

	//INoticeSource
	virtual void __stdcall INoticeSource_Unknown() RX;

	//Destructor
	virtual ~VesselTypeClass();

	virtual int Size() const override;
	virtual void CalculateChecksum(WWCRCEngine& checksum) const override;
	virtual int GetArrayIndex() const override;

	//AbstractTypeClass
	virtual bool LoadFromINI(CCINIClass* ini) override;

	//ObjectTypeClass
	virtual CoordStruct* vt_entry_6C(CoordStruct * pRet, CoordStruct * pSrc) const override;
	virtual void PixelDimensions(Point3D * pDest) const override;
	virtual void Dimension2(CoordStruct * pDest) override;
	virtual bool SpawnAtMapCoords(CellStruct * pMapCoords, HouseClass * pOwner = nullptr) override;
	virtual ObjectClass* CreateObject(HouseClass * pOwner = nullptr) override;

	//TechnoTypeClass

	const Point3D TurretAdjust(Direction dir, Point3D &pos) const;

	static const VesselTypeClass& As_Reference(VesselType type);
	static const VesselTypeClass* As_Pointer(VesselType type);
	static const VesselTypeClass& As_Reference(const char* name);
	static const VesselTypeClass* As_Pointer(const char* name);
	static VesselType From_Name(const char* name);
	static const char* Name_From(VesselType type);
	static const VesselTypeClass* Find_Or_Make(const char* name);

	static void One_Time();

	VesselTypeClass(const char* ini_name = nullptr);
protected:
	explicit __forceinline VesselTypeClass(noinit_t) noexcept
		: TechnoTypeClass(noinit_t())
	{}
public:
	/**
	 *  This value represents the unit class. It can serve as a unique
	 *  identification number for this unit class.
	 */
	VesselType Type;
};