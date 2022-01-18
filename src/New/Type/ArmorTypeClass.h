#pragma once
#include <Utilities/Enumerable.h>
#include <Utilities/Template.h>
#include <Utilities/GeneralUtils.h>

class ArmorTypeClass final : public Enumerable<ArmorTypeClass>
{
public:

	int DefaultTo;
	ArmorTypeClass(const char* const pTitle) : Enumerable<ArmorTypeClass>(pTitle)
		, DefaultTo{ -1 }
	{ }

	virtual ~ArmorTypeClass() override = default;
	static void AddDefaults();

	virtual void LoadFromINI(CCINIClass* pINI) override;
	virtual void LoadFromStream(PhobosStreamReader& Stm);
	virtual void SaveToStream(PhobosStreamWriter& Stm);
	static void LoadFromINIList_(CCINIClass* pINI, bool bDebug = false);

private:
	template <typename T>
	void Serialize(T& Stm);
};