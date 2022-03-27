#include "AttachEffectManagerClass.h"

void AttachEffectManagerClass::AI()
{
	if (!Owner)
		return;

	if (!AttachedAffects.empty())
	{
		auto IsExpired = [this](AttachedAffectClass* pAffect)
		{ return pAffect && pAffect->Delete; };


		auto it = std::remove_if(AttachedAffects.begin(), AttachedAffects.end(), IsExpired);

		if (it != AttachedAffects.end())
		{
			if ((*it)->Type->Next)
			{
				auto itAeType = AttachEffectTypeClass::Find((*it)->Type->Next.data());
				if (auto ItIs = GameCreate<AttachedAffectClass>(itAeType, (*it)->Invoker))
					Attach(ItIs);
			}

			GameDelete((*it));
			AttachedAffects.erase(it, AttachedAffects.end());
			AttachEffectManagerClass::RecalcStat(Owner);
		}

		for (auto const& pItem : AttachedAffects)
		{
			if (pItem)
			{
				if (pItem->InitialDelay > 0)
				{
					if (!pItem->InitialDelayApplied)
						--pItem->InitialDelay;
				}
				else
				{
					pItem->InitialDelayApplied = true;
					pItem->AI(Owner, false , false);
				}
			}
		}
	}
}

void AttachEffectManagerClass::OnTemporaled()
{
	for (auto const& pItem : AttachedAffects)
	{
		if (pItem)
		{
			pItem->KillAnim();
			pItem->RecreateAnims = true;
		}
	}

	AI();
}

void AttachEffectManagerClass::OnRemove()
{
	for (auto const& pItem : AttachedAffects)
	{
		if (pItem)
		{
			pItem->KillAnim();
			pItem->RecreateAnims = true;
		}
	}

	auto const it = std::remove_if(
	AttachedAffects.begin(), AttachedAffects.end(),
	[](AttachedAffectClass* Item)
	{
		return Item && Item->Type->DiscardOnEntry.Get();
	});

	if (it != AttachedAffects.end())
	{
		GameDelete((*it));
		AttachedAffects.erase(it, AttachedAffects.end());
		AttachEffectManagerClass::RecalcStat(Owner);
	}
}

void AttachEffectManagerClass::Attach(AttachedAffectClass* pBehave)
{
	if (!pBehave || !pBehave->Type || !pBehave->Invoker)
		return;

	auto pAEType = pBehave->Type;

	if (pAEType->AppliedOnlyOnce)
		AppliedOnce[pAEType] = true;

	if (AppliedOnce.contains(pAEType) && AppliedOnce[pAEType])
		return;

	//cumulative = yes , check type , stack them
	//cumulative = no , renew duration
	if (pAEType->Cumulative == CumulativeMode::NO)
	{
		auto bSameType = [&pAEType, &pBehave](AttachedAffectClass* pBe)
		{
			return
				!_strcmpi(pBe->Type->Name.data(), pAEType->Name.data()) &&
				pBe->Invoker == pBehave->Invoker;
		};

		auto const it = std::find_if(AttachedAffects.begin(), AttachedAffects.end(), bSameType);
		if (it != AttachedAffects.end())
		{
			auto& Item = (*it);
			if (pAEType->Duration != -1)
			{
				Item->RealDuration = Math::clamp(Item->RealDuration + pAEType->Duration.Get(), 0, pAEType->Duration.Get());
			}
			Item->Invoker = pBehave->Invoker;
			Item->ComulativeReapllied = true;

			if (pAEType->AnimResetOnReapply)
			{ Item->CreateAnim(Owner); }

			//because it is not comulative , we delete previousely created 
			//just reapply the duration
			GameDelete(pBehave);
			return;
		}
	}

	AttachedAffects.emplace_back(pBehave);
	auto& Attaching = AttachedAffects.back();
	AttachEffectManagerClass::RecalcStat(Owner);
	Attaching->CreateAnim(Owner);
}
#include <Ext/Abstract/Body.h>

void AttachEffectManagerClass::RecalcStat(ObjectClass* pOwner)
{
	bool bCloak = false;
	double FirePowerMult = 1.0;
	double ArmorMult = 1.0;
	double SpeedMult = 1.0;

	if (auto pAbsExt = AbstractExt::ExtMap.Find(pOwner))
	{
		if (auto pAEMan = pAbsExt->AEManager.get())
		{
			for (const auto& Item : pAEMan->AttachedAffects)
			{
				if (!Item->AffectDisabled)
				{
					auto const pType = Item->Type;
					FirePowerMult *= pType->FirepowerMultiplier;
					SpeedMult *= pType->SpeedMultiplier;
					ArmorMult *= pType->ArmorMultiplier;
					bCloak |= pType->Cloakable;
				}
			}
		}
	}

	if (auto pTechno = abstract_cast<TechnoClass*>(pOwner))
	{
		pTechno->FirepowerMultiplier *= FirePowerMult;
		pTechno->Cloakable |= bCloak;
		pTechno->ArmorMultiplier *= ArmorMult;

		if (auto const pFoot = abstract_cast<FootClass*>(pTechno))
		{
			 pFoot->SpeedMultiplier *= SpeedMult;
		}
	}

	if (auto pBullet = abstract_cast<BulletClass*>(pOwner))
	{
		pBullet->DamageMultiplier *= FirePowerMult;
		pBullet->Speed *= SpeedMult;
	}
}