#pragma once

#include <CoordStruct.h>
#include <Utilities/Savegame.h>
#include <Utilities/Constructs.h>
#include "AttachEffectTypeClass.h"
#include <ObjectClass.h>

class WarheadTypeClass;
class TemporalClass;
class HouseClass;
class TechnoClass;
class AttachedAffectClass
{
	struct UninitAnim
	{
		void operator() (AnimClass* const pAnim) const
		{
			pAnim->TimeToDie = true;
			pAnim->UnInit();
		}
	};

public:

	static int CalculateInitialDelay(AttachEffectTypeClass* pType);
	static int CalculatelDelay(AttachEffectTypeClass* pType);

	AttachedAffectClass() = default;

	bool Delete { false };
	bool RecreateAnims { false };
	Handle<AnimClass*, UninitAnim> Animation { nullptr };
	int RealDuration { 0 };
	bool UnlimitedDuration { false };
	int Delay { 0 };
	int InitialDelay { 0 };
	bool ComulativeReapllied { false };
	bool InitialDelayApplied { false };
	ObjectClass* Invoker { nullptr };
	AttachEffectTypeClass* Type { nullptr };
	bool AffectDisabled { false };
	bool Disabled { false };

	AttachedAffectClass(AttachEffectTypeClass* attachEffectType, ObjectClass* invoker) :
		Type(attachEffectType)
		, Invoker(invoker)
		, Delay(0)
		, InitialDelay(CalculateInitialDelay(attachEffectType))
		, RealDuration(attachEffectType->Duration.Get())
	{ }

	virtual ~AttachedAffectClass() { KillAnim(); };

	virtual const char* WhatType() { return Type->Name.c_str(); }

	void CreateAnim(ObjectClass* pVictim);
	void AI(ObjectClass* pVictim, bool Reset , bool DontDelete);
	void KillAnim()
	{
		this->Animation.clear();
	}

	void InvalidatePointer(void* ptr)
	{
		AnnounceInvalidPointer(Invoker, ptr);

		if (Animation.get() && Animation.get() == ptr)
			Animation.clear();
	}

	bool Load(PhobosStreamReader& Stm, bool RegisterForChange)
	{
		return Stm
			.Process(this->Type)
			.Process(this->Animation)
			.Process(this->RealDuration)
			.Process(this->Invoker)
			.Process(this->RecreateAnims)

			.Process(this->Delete)
			.Process(this->InitialDelay)
			.Process(this->Delay)
			.Process(this->ComulativeReapllied)
			.Process(this->InitialDelayApplied)
			.Process(this->AffectDisabled)
			.Success();
	}

	bool Save(PhobosStreamWriter& Stm) const
	{
		return Stm
			.Process(this->Type)
			.Process(this->Animation)
			.Process(this->RealDuration)
			.Process(this->Invoker)
			.Process(this->RecreateAnims)

			.Process(this->Delete)
			.Process(this->InitialDelay)
			.Process(this->Delay)
			.Process(this->ComulativeReapllied)
			.Process(this->InitialDelayApplied)
			.Process(this->AffectDisabled)
			.Success();
	}

};