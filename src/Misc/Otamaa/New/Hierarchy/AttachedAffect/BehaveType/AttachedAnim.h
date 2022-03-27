#pragma once

#include "Base.h"

class AttachedAnimClass : public AttachEffectBehaviour
{
public:

	AttachedAnimClass(AnimTypeClass* pAttach, AttachEffectTypeClass* pType , ObjectClass* Invoker) :
		AttachEffectBehaviour(pType , Invoker)
		, AnimType(pAttach)
	{
		Init();
	}

	virtual ~AttachedAnimClass() override
	{
		KillAnim();
	};

	virtual BehaveKind WhatImI() override { return BehaveKind::Anim; }
	virtual void Init() override { }

	virtual void OnUpdate(ObjectClass* pVictim) override
	{
		if (!Attached)
		{
			if (AnimType)
			{
				if (auto pAnim = GameCreate<AnimClass>(AnimType, pVictim->Location))
				{
					if(Invoker)
					pAnim->Owner = Invoker->GetOwningHouse();
				}
			}
		}
		else
		{
			if (auto pAnim = Attached)
			{
				if (!pAnim->IsPlaying)
						KillAnim();
			}
		}
	}

	virtual void OnRemove(ObjectClass* pVictim) override
	{
		KillAnim();
	}

	void KillAnim()
	{
		if (auto pAnim = Attached)
		{
			pAnim->RemainingIterations = 0;
			pAnim->TimeToDie = true;
			pAnim->UnInit();
		}
	}		
	
	virtual void LoadFromStream(PhobosStreamReader& Stm) override
	{
		Stm
			.Process(Invoker)
			.Process(Type)
			.Process(AnimType)
			.Process(Attached)
			;
	}

	virtual void SaveToStream(PhobosStreamWriter& Stm) override
	{
		Stm
			.Process(Invoker)
			.Process(Type)
			.Process(AnimType)
			.Process(Attached)
			;
	}

	AnimTypeClass* AnimType;
	AnimClass* Attached;
};