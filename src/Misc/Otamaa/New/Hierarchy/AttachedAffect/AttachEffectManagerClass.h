#pragma once

#include <Utilities/Constructs.h>
#include <Utilities/TemplateDef.h>
#include "AttachEffectClass.h"

#include <BulletClass.h>

//type on each object 
class AttachEffectDataClass
{
public:
	std::vector<AttachEffectTypeClass*> AttachEffectTypes;
	int CabinLength;
};

//for handling location and direction
class LocationMarkClass
{
public:
	CoordStruct Location;
	DirStruct Direction;

	LocationMarkClass(CoordStruct location, DirStruct direction) :
		Location(location), Direction(direction)
	{
	}
};


//managing all current AE
class AttachEffectManagerClass
{
public:
	std::vector<AttachedAffectClass*> AttachedAffects;
	PhobosMap<AttachEffectTypeClass*, bool> AppliedOnce;
	ObjectClass* Owner;

public:
	AttachEffectManagerClass() :
		AttachedAffects(),
		Owner(nullptr)
	{
	}

	explicit AttachEffectManagerClass(TechnoClass* pOwner) :
		AttachedAffects(),
		Owner(pOwner)
	{
	}

	explicit AttachEffectManagerClass(BulletClass* pOwner) :
		AttachedAffects(),
		Owner(pOwner)
	{
	}

	size_t Count()
	{ return AttachedAffects.size(); }

	virtual void Init()
	{
		AttachedAffects.clear();
	}

	void AI();
	void OnTemporaled();
	void OnRemove();
	void Attach(AttachedAffectClass* pBehave);
	static void RecalcStat(ObjectClass* pOwner);

	virtual void LoadFromStream(PhobosStreamReader& Stm)
	{
		Stm
			.Process(AttachedAffects)
			.Process(AppliedOnce)
			.Process(Owner)
			;
	}

	virtual void SaveToStream(PhobosStreamWriter& Stm)
	{
		Stm
			.Process(AttachedAffects)
			.Process(AppliedOnce)
			.Process(Owner)
			;
	}
};