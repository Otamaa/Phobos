#include "ExtraFirefunctional.h"

#include <TechnoClass.h>
#include <Ext/TechnoType/Body.h>
#include <Ext/WeaponType/Body.h>
#include <Ext/Techno/Body.h>
#include <Misc/DynamicPatcher/Helpers/Helpers.h>
#include <Misc/DynamicPatcher/CustomWeapon/CustomWeapon.h>

void ExtraFirefunctional::GetWeapon(TechnoClass* pThis, AbstractClass* pTarget, int nWeaponIdx)
{
	// wtf is this , really

	auto const pType = pThis->GetTechnoType();
	auto const pExt = TechnoExt::ExtMap.Find(pThis);
	auto const pTypeExt = TechnoTypeExt::ExtMap.Find(pType);
	auto const nExtraFireData = pTypeExt->ExtraFireDatas;

	// it has no weapon anyway
	if (nExtraFireData.WeaponData.empty() || (size_t)(nWeaponIdx+1) > nExtraFireData.WeaponData.size())
		return;

	Debug::Log("Executing ExtraFire for [%s] ! \n", pType->get_ID());

	ValueableVector<WeaponTypeClass*>  pSelectedWeapon;
	ValueableVector<CoordStruct> nSelectedFLH;
	ValueableVector<CoordStruct> nSelectedFLH_Elite;

	nSelectedFLH = nExtraFireData.WeaponFLHData;
	nSelectedFLH_Elite = nExtraFireData.EliteWeaponFLHData;

	if (auto pTransporter = pThis->Transporter)
	{
		auto const pTrasTypeExt = TechnoTypeExt::ExtMap.Find(pTransporter->GetTechnoType());
		auto const pTransnExtraFireData = pTrasTypeExt->ExtraFireDatas;
		nSelectedFLH = pTransnExtraFireData.WeaponFLHData;
		nSelectedFLH_Elite = pTransnExtraFireData.EliteWeaponFLHData;
	}

	const auto GetRof = [](TechnoClass* pTech)
	{
		bool rofAbility = false;
		if (pTech->Veterancy.IsElite())
			rofAbility = pTech->GetTechnoType()->VeteranAbilities.ROF || pTech->GetTechnoType()->EliteAbilities.ROF;
		else if (pTech->Veterancy.IsVeteran())
			rofAbility = pTech->GetTechnoType()->VeteranAbilities.ROF;

		return !rofAbility ? 1.0 :
			RulesClass::Instance->VeteranROF * ((!pTech->Owner || !pTech->Owner->Type) ?
				1.0 : pTech->Owner->Type->ROFMult);
	};

	CoordStruct nFLH = CoordStruct::Empty;

	if (!pThis->Veterancy.IsElite())
	{
		if (!nSelectedFLH.empty())
		{
			if ((size_t)nWeaponIdx < nSelectedFLH.size())
				nFLH = nSelectedFLH.at(nWeaponIdx);
		}

		pSelectedWeapon = nExtraFireData.WeaponData.at(nWeaponIdx);
	}
	else
	{
		if (!nSelectedFLH_Elite.empty())
		{
			if ((size_t)nWeaponIdx < nSelectedFLH.size())
				nFLH = nSelectedFLH_Elite.at(nWeaponIdx);
		}


		pSelectedWeapon = nExtraFireData.EliteWeaponData.at(nWeaponIdx);
	}

	pExt->ExtraWeapons = pSelectedWeapon;

	if (!pExt->ExtraWeapons.empty())
	{
		auto ROF = GetRof(pThis);
		CoordStruct flh = pThis->GetWeapon(nWeaponIdx)->FLH;

		if (nFLH != CoordStruct::Empty)
			nFLH = flh;

		if (pExt->ExtraWeaponsCurIDx >= pExt->ExtraWeapons.size())
			pExt->ExtraWeaponsCurIDx = 0;

		for (size_t k = 0 + pExt->ExtraWeaponsCurIDx; k < pExt->ExtraWeapons.size(); ++k)
		{
			if (auto pWeapon = pExt->ExtraWeapons.at(k))
			{
				if (auto const pWeaponTypeExt = WeaponTypeExt::ExtMap.Find(pWeapon))
				{
					auto const fireData = pWeaponTypeExt->WAttachFireData;

					if (fireData.UseROF)
					{
						int rof = (int)(pWeapon->ROF * ROF);
						auto& nTimer = pExt->ExtraFireROFTimer;

						if (nTimer.Expired())
						{
							pExt->WeaponManager.FireCustomWeapon(pThis, pThis, pTarget, pWeapon, flh, CoordStruct::Empty, ROF);
							pExt->ExtraWeaponsCurIDx = k;
							nTimer.Start(rof);
							break;
						}
					}
					else
					{
						pExt->WeaponManager.FireCustomWeapon(pThis, pThis, pTarget, pWeapon, flh, CoordStruct::Empty, ROF);
					}
				}
			}
		}
	}
}