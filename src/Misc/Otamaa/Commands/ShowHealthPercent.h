#pragma once

#include "Body.h"

#include <Ext/Rules/Body.h>

class ShowHealthPercentCommandClass : public PhobosCommandClass
{
public:

	// CommandClass
	virtual const char* GetName() const override
	{
		return "Show Health Percent";
	}

	virtual const wchar_t* GetUIName() const override
	{
		return GeneralUtils::LoadStringUnlessMissing("TXT_SHOWHEALTHPERCENT", L"Show Health Percent");
	}

	virtual const wchar_t* GetUICategory() const override
	{
		return GeneralUtils::LoadStringUnlessMissing("TXT_INTERFACE", L"Interface");
	}

	virtual const wchar_t* GetUIDescription() const override
	{
		return GeneralUtils::LoadStringUnlessMissing("TXT_SHOWHEALTHPERCENT_DESC", L"Show health percent of all technos if available.");
	}

	virtual void Execute(WWKey eInput) const override
	{
		Phobos::Config::ShowHealthPercentEnabled = !Phobos::Config::ShowHealthPercentEnabled;

		if (CCINIClass* pINI = GameCreate<CCINIClass>())
		{
			auto pFlag = (Make_Pointer<const char>(0x827DC8));
			if (auto cfg = GameCreate<CCFileClass>(pFlag))
			{
				if (cfg->Exists())
				{
					//Debug::Log_Masselist("Key Saved [%s] ! ", Phobos::Config::ShowHealthPercentEnabled ? "True" : "False");
					pINI->WriteString("CommandClass", "ShowHealthPercentEnabled", Phobos::Config::ShowHealthPercentEnabled ? "True" : "False");
				}

				GameDelete(cfg);
				cfg = nullptr;
			}

			GameDelete(pINI);
			pINI = nullptr;
		}
	}
};
