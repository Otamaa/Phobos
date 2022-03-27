#pragma once

#include <CoordStruct.h>
#include <Utilities/TemplateDef.h>

class WeaponTypeClass;

struct ExtraFireData
{
	ValueableVector<CoordStruct> WeaponFLHData;
	ValueableVector<CoordStruct> EliteWeaponFLHData;
	ValueableVector<DynamicVectorClass<WeaponTypeClass*>> WeaponData;
	ValueableVector<DynamicVectorClass<WeaponTypeClass*>> EliteWeaponData;

	ExtraFireData() :
		WeaponFLHData { }
		, EliteWeaponFLHData { }
		, WeaponData { }
		, EliteWeaponData { }
	{ }

	~ExtraFireData() = default;

	//confuse ? , yeah me too :kekw:
	void Read(TechnoTypeClass* pTech, INI_EX& parserRules, INI_EX& parserArt, const char* pSection_rules, const char* pSection_Art);

	bool Load(PhobosStreamReader& Stm, bool RegisterForChange)
	{ Debug::Log("Loading Element From ExtraFireData ! \n"); return Serialize(Stm); }

	bool Save(PhobosStreamWriter& Stm)
	{ return Serialize(Stm); }

private:
	template <typename T>
	bool Serialize(T& Stm)
	{
		return Stm
			.Process(WeaponFLHData)
			.Process(EliteWeaponFLHData)
			.Process(WeaponData)
			.Process(EliteWeaponData)
			.Success()
			;
	}

};