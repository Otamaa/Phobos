#pragma once

#include <Utilities/TemplateDef.h>

class SpawnSupportFLHData
{
public:

	Valueable<CoordStruct> SpawnSupportFLH;
	Valueable<CoordStruct> EliteSpawnSupportFLH;

	Valueable<CoordStruct> SpawnHitFLH;
	Valueable<CoordStruct> EliteSpawnHitFLH;

	SpawnSupportFLHData()
		: SpawnSupportFLH { }
		, EliteSpawnSupportFLH { }
		, SpawnHitFLH { }
		, EliteSpawnHitFLH { }
	{ }

	void Read(INI_EX& nParser, const char* pSection)
	{
		SpawnSupportFLH.Read(nParser, pSection, "SupportWeaponFLH");
		EliteSpawnSupportFLH.Read(nParser, pSection, "EliteSupportWeaponFLH");

		if (EliteSpawnSupportFLH.Get() == CoordStruct::Empty)
			EliteSpawnSupportFLH = SpawnSupportFLH;

		SpawnHitFLH.Read(nParser, pSection, "SupportWeaponHitFLH");
		EliteSpawnHitFLH.Read(nParser, pSection, "EliteSupportWeaponHitFLH");

		if (EliteSpawnHitFLH.Get() == CoordStruct::Empty)
			EliteSpawnHitFLH = SpawnHitFLH;
	}

	bool Load(PhobosStreamReader& Stm, bool RegisterForChange)
	{ Debug::Log("Loading Element From SpawnSupportFLHData ! \n");  return Serialize(Stm); }

	bool Save(PhobosStreamWriter& Stm)
	{ return Serialize(Stm); }

private:
	template <typename T>
	bool Serialize(T& Stm)
	{
		return Stm
			.Process(SpawnSupportFLH)
			.Process(EliteSpawnSupportFLH)
			.Process(SpawnHitFLH)
			.Process(EliteSpawnHitFLH)
			.Success()
			;
	}
};

class SpawnSupportData
{
public:

	Valueable<bool> Enable;
	Valueable<WeaponTypeClass*> SupportWeapon;
	Valueable<WeaponTypeClass*> EliteSupportWeapon;
	Valueable<bool> SwitchFLH;
	Valueable<bool> Always;

	Valueable<bool> FireOnce;
	Valueable<int> Delay;

	SpawnSupportData() :
		Enable { false }
		, SupportWeapon { nullptr }
		, EliteSupportWeapon { nullptr }
		, SwitchFLH { false }
		, Always { false }

		, FireOnce { false }
		, Delay { 0 }
	{ }

	void Read(INI_EX& nParser, const char* pSection)
	{
		Enable.Read(nParser, pSection, "SupportSpawns");

		if(Enable)
		{
			SupportWeapon.Read(nParser, pSection, "SupportSpawns.Weapon", true);
			EliteSupportWeapon.Read(nParser, pSection, "SupportSpawns.EliteWeapon", true);

			if (!EliteSupportWeapon)
				SupportWeapon = EliteSupportWeapon;

			SwitchFLH.Read(nParser, pSection, "SupportSpawns.SwitchFLH");
			Always.Read(nParser, pSection, "SupportSpawns.AlwaysFire");
		}

		FireOnce.Read(nParser, pSection, "SpawnFireOnce");
		Delay.Read(nParser, pSection, "SpawnFireOnceDelay");
	}

	bool Load(PhobosStreamReader& Stm, bool RegisterForChange)
	{ Debug::Log("Loading Element From SpawnSupportData ! \n");  return Serialize(Stm); }

	bool Save(PhobosStreamWriter& Stm)
	{ return Serialize(Stm); }

private:
	template <typename T>
	bool Serialize(T& Stm)
	{
		return Stm
			.Process(Enable)
			.Process(SupportWeapon)
			.Process(EliteSupportWeapon)
			.Process(SwitchFLH)
			.Process(Always)
			.Process(FireOnce)
			.Process(Delay)
			.Success()
			;
	}
};