#include "PassengersFunctional.h"

#include <Ext/Techno/Body.h>
#include <Ext/TechnoType/Body.h>

void PassengersFunctional::AI(TechnoClass* pThis)
{
	if (auto const pTranporter = pThis->Transporter)
	{
		if (pTranporter->GetTechnoType()->OpenTopped)
		{
			if (auto const pTransportExt = TechnoTypeExt::ExtMap.Find(pTranporter->GetTechnoType()))
			{
				auto& nData = pTransportExt->AnotherData;

				if (!nData.MyPassangersData.PassiveAcquire)
				{
					auto nMission = pTranporter->GetCurrentMission();
					if (nMission != Mission::Attack)
						pThis->Override_Mission(Mission::Sleep, nullptr,nullptr);
				}

				if (nData.MyPassangersData.ForceFire)
					pThis->Override_Mission(pTranporter->CurrentMission, pTranporter->Target);
			}
		}
	}
}

void PassengersFunctional::CanFire(TechnoClass* pThis, bool& cease)
{
	if (auto const pTranporter = pThis->Transporter)
	{
		if (pTranporter->GetTechnoType()->OpenTopped)
		{
			if (auto const pTransportExt = TechnoTypeExt::ExtMap.Find(pTranporter->GetTechnoType()))
			{
				auto& nData = pTransportExt->AnotherData;

				switch (pTranporter->GetCurrentMission())
				{
				case Mission::Attack:
					cease = !nData.MyPassangersData.SameFire;
					break;
				case Mission::Move:
				case Mission::AttackMove:
					cease = !nData.MyPassangersData.MobileFire;
					break;
				}
			}
		}
	}
}