#pragma once

#include <TechnoClass.h>
#include <Utilities/Constructs.h>

class ConvertManager
{
	TechnoClass* Owner;
public:
	//we store this as string then look up the type
	PhobosFixedString<0x18> CurrentTypeName;
	PhobosFixedString<0x18> PrevTypeName;

	ConvertManager():
	  Owner(nullptr)
	, CurrentTypeName()
	, PrevTypeName()
	{ }

	explicit ConvertManager(TechnoClass* pOwner)
		: Owner(pOwner), PrevTypeName()
	{  CurrentTypeName = pOwner->get_ID(); }

	bool CheckConverted()
	{
		if (_strcmpi(CurrentTypeName.c_str(), Owner->get_ID()))
		{
			PrevTypeName = CurrentTypeName;
			CurrentTypeName = Owner->get_ID();
			return true;
		}

		return false;
	}

	bool Load(PhobosStreamReader& Stm, bool RegisterForChange)
	{
		return Stm
			.Process(Owner)
			.Process(CurrentTypeName)
			.Process(PrevTypeName)
			.Success();
	}

	bool Save(PhobosStreamWriter& Stm) const
	{
		return Stm
			.Process(Owner)
			.Process(CurrentTypeName)
			.Process(PrevTypeName)
			.Success();
	}

};