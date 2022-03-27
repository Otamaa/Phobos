#include "AttachEffectClass.h"
#include "AttachEffectManagerClass.h"

int AttachedAffectClass::CalculateInitialDelay(AttachEffectTypeClass* pType)
{
	if (pType->InitialRandomDelay && pType->InitialMinDelay < pType->InitialMaxDelay)
		return ScenarioGlobal->Random(pType->InitialMinDelay, pType->InitialMaxDelay);
	else
		return  pType->InitialDelay;
}

int AttachedAffectClass::CalculatelDelay(AttachEffectTypeClass* pType)
{
	if (pType->RandomDelay && pType->MinDelay < pType->MaxDelay)
		return ScenarioGlobal->Random(pType->MinDelay, pType->MaxDelay);
	else
		return  pType->Delay;
}

void AttachedAffectClass::CreateAnim(ObjectClass* pVictim)
{
	auto const pType = this->Type;
	if (!pType || !pVictim)
		return;

	if (auto pTechnoVictim = generic_cast<TechnoClass*>(pVictim))
	{
		auto const state = pTechnoVictim->CloakState;
		if ((state == CloakState::Cloaked || state == CloakState::Cloaking) ||
			(pTechnoVictim->TemporalTargetingMe && pType->TemporalHidesAnim))
			return;
	}

	//hide anim when on this states
	if (pVictim->InLimbo || pVictim->Health <= 0 || !pVictim->IsAlive)
		return;

	if (AnimTypeClass* const pAnimTypeHere = pType->AnimType.Get())
	{
		this->Animation.reset(GameCreate<AnimClass>(pAnimTypeHere, pVictim->Location));

		if (AnimClass* const pAnim = this->Animation)
		{
			pAnim->SetOwnerObject(pVictim);
			pAnim->RemainingIterations = 0xFFu;

			if (this->Invoker)
				pAnim->Owner = this->Invoker->GetOwningHouse();
		}
	}
}

void AttachedAffectClass::AI(ObjectClass* pVictim, bool Reset, bool DontDelete)
{
	if (!Type || !pVictim)
	{
		Delete = true;
		return;
	}

	if (!Type->AffectTypes.empty())
	{
		for (auto const& pAllowName : Type->AffectTypes)
		{
			if (_strcmpi(pAllowName.c_str(), pVictim->GetType()->get_ID()))
			{
				Delete = true;
				return;
			}

		}
	}

	if (pVictim->Health <= 0 || !pVictim->IsAlive)
	{
		Delete = true;
		return;
	}

	if (!Type->LimboUpdateAe && pVictim->InLimbo)
		return;

	if (!Type->PenetratesIronCurtain && pVictim->IsIronCurtained())
	{
		Delete = true;
		return;
	}

	if (auto pTechnoVictim = generic_cast<TechnoClass*>(pVictim))
	{
		if (!Type->TemporalUpdateAE && pTechnoVictim->TemporalTargetingMe)
			return;

		if (pTechnoVictim->CloakState == CloakState::Cloaked ||
			pTechnoVictim->CloakState == CloakState::Cloaking
			)
		{
			if (Type->ForceDecloak && pTechnoVictim->CloakState != CloakState::Uncloaked)
				pTechnoVictim->Uncloak(true);

			if (!RecreateAnims)
			{
				KillAnim();
				RecreateAnims = true;
			}
		}
	}

	if (RecreateAnims)
	{
		CreateAnim(pVictim);
		RecreateAnims = false;
	}

	if (AnimClass* const pAnim = this->Animation)
	{
		if (ComulativeReapllied)
		{
			if (Invoker)
				pAnim->Owner = Invoker->GetOwningHouse();

			ComulativeReapllied = false;
		}
	}


	if (Delay > 0)
	{
		--Delay;
	}
	else
	{
		if (Disabled)
		{
			Disabled = false;
			CreateAnim(pVictim);
			Debug::Log_Masselist("AE [%s] delay depleted AffectApplied !", WhatType(), Delay);
			AttachEffectManagerClass::RecalcStat(pVictim);
		}

		if (Type->Duration.Get() == -1)
		{
			UnlimitedDuration = true;
			RealDuration = 1;
		}
			
		if(!UnlimitedDuration && RealDuration > 0)
			--RealDuration;

		if (auto pVictimTech = generic_cast<TechnoClass*>(pVictim))
			if (pVictimTech->Deactivated)
				RealDuration = 0;

		if (!RealDuration)
		{
			auto nDelay = CalculatelDelay(Type);
			if (nDelay > 0)
			{ 
				Disabled = true;
				Delay = nDelay;
			}
			else
			{
				Delete = true;
			}

			KillAnim();
			AttachEffectManagerClass::RecalcStat(pVictim);
		}
	}
}