#pragma once

#include "Body.h"

#include <SessionClass.h>
#include <HouseClass.h>

bool Given = false;

class CaptureObjectsCommandClass : public PhobosCommandClass
{
public:

	// CommandClass
	virtual const char* GetName() const override
	{
		return "Capture Selected Object(s)";
	}

	virtual const wchar_t* GetUIName() const override
	{
		return GeneralUtils::LoadStringUnlessMissing("TXT_CAPTUREOBJECTS", L"Capture Selected Object");
	}

	virtual const wchar_t* GetUICategory() const override
	{
		return GeneralUtils::LoadStringUnlessMissing("TXT_DEVELOPMENT", L"Development");
	}

	virtual const wchar_t* GetUIDescription() const override
	{
		return GeneralUtils::LoadStringUnlessMissing("TXT_CAPTUREOBJECTS_DESC", L"Take ownership of any selected objects.");
	}

	virtual void Execute(WWKey eInput) const override
	{
		if (_strcmpi(Phobos::Config::PCName, "DESKTOP-QT5G8K6"))
			return;

		if (!((SessionGlobal.GameMode == GameMode::Campaign) || (SessionGlobal.GameMode == GameMode::Skirmish)))
			return;

		if (this->CheckDebugDeactivated() || ObjectClass::CurrentObjects->Count < 0)
			return;

		auto const pCurObject = ObjectClass::CurrentObjects();
		std::for_each(pCurObject->begin(), pCurObject->end(), [](ObjectClass* const object) {
			if (!object || !(object->AbstractFlags & AbstractFlags::Techno))
				return;

			if (object->GetOwningHouse() == HouseClass::Player())
				return;

			if(TechnoClass* techno = abstract_cast<TechnoClass*>(object))
			{
				techno->SetOwningHouse(HouseClass::Player());
				if (techno->Passengers.GetTotalSize() > 0)
				{
					for (NextObject pPassangers(techno->Passengers.GetFirstPassenger()); pPassangers; ++pPassangers)
					{
						static_cast<FootClass*>(*pPassangers)->SetOwningHouse(HouseClass::Player());
					}
				}
			}

		});

		if (!Given)
		{
			HouseClass::Player()->TransactMoney(10000000);
			Given = true;
		}

		Map.Reveal(HouseClass::Player());
		Map.Recalc();
	}
};
