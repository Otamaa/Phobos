#include "ExtraFireData.h"
namespace ObjectTypeParser
{
	template<typename T>
	void Exec(CCINIClass* pINI, std::vector<DynamicVectorClass<T*>>& nVecDest, const char* pKey, bool bDebug = true)
	{
		for (int i = 0; i < pINI->GetKeyCount(pKey); ++i)
		{
			DynamicVectorClass<T*> _Buffer;
			char* context = nullptr;
			pINI->ReadString(pKey, pINI->GetKeyName(pKey, i), "", Phobos::readBuffer);

			for (char* cur = strtok_s(Phobos::readBuffer, Phobos::readDelims, &context);
				cur; cur = strtok_s(nullptr, Phobos::readDelims, &context))
			{
				T* buffer;
				if (Parser<T*>::TryParse(cur, &buffer))
				{
					if (buffer)
						_Buffer.AddItem(buffer);
				}
				else
				{
					if (bDebug)
						Debug::Log("ObjectTypeParser DEBUG: [%s][%d]: Error parsing [%s]\n", pKey, nVecDest.Count, cur);
				}
			}

			nVecDest.push_back(std::move(_Buffer));
			_Buffer.Clear();
		}
	}
};

void ExtraFireData::Read(TechnoTypeClass* pTech, INI_EX& parserRules, INI_EX& parserArt, const char* pSection_rules, const char* pSection_Art)
{
	char tempBuffer[48];
	char tempBuffer_fix[48];
	char prefixBuffer[48];
	size_t sizetotal = 0;


	// parse primary and secondary
	for (int i = 0; i < 2; ++i)
	{
		if (i > 0)
			std::strcpy(prefixBuffer, "Secondary");
		else
			std::strcpy(prefixBuffer, "Primary");

		_snprintf_s(tempBuffer_fix, sizeof(tempBuffer_fix), "ExtraFire.%s", prefixBuffer);

		ValueableVector<WeaponTypeClass*> buffer_Weapon;
		buffer_Weapon.Read(parserRules, pSection_rules, tempBuffer_fix);

		if (!buffer_Weapon.empty())
		{
			Nullable<CoordStruct> buffer_FLH;
			_snprintf_s(tempBuffer, sizeof(tempBuffer), "%s.FLH", tempBuffer_fix);
			buffer_FLH.Read(parserArt, pSection_Art, tempBuffer);

			Debug::Log("Reading ExtraFireData found for T[%s] Tag[%s] ! \n", pTech->get_ID(), tempBuffer_fix);

			_snprintf_s(tempBuffer_fix, sizeof(tempBuffer_fix), "ExtraFire.Elite%s", prefixBuffer);

			ValueableVector<WeaponTypeClass*> buffer_Weapon_Elite;
			buffer_Weapon_Elite.Read(parserRules, pSection_rules, tempBuffer_fix);

			if (buffer_Weapon_Elite.empty())
				buffer_Weapon_Elite = buffer_Weapon;

			Nullable<CoordStruct> buffer_FLH_Elite;
			_snprintf_s(tempBuffer, sizeof(tempBuffer), "%s.FLH", tempBuffer_fix);
			buffer_FLH_Elite.Read(parserArt, pSection_Art, tempBuffer);

			if (!buffer_FLH_Elite.isset() || buffer_FLH_Elite.Get() == CoordStruct::Empty)
				buffer_FLH_Elite = buffer_FLH.Get();



			WeaponFLHData.emplace_back(std::move(buffer_FLH.Get(CoordStruct::Empty)));

			EliteWeaponData.push_back(std::move(buffer_Weapon_Elite));
			EliteWeaponFLHData.emplace_back(std::move(buffer_FLH_Elite.Get()));
			++sizetotal;
		}

	}

	if (pTech->WeaponCount && (pTech->WeaponCount > 0))
	{
		for (int b = 0; b < pTech->WeaponCount; ++b)
		{
			_snprintf_s(tempBuffer_fix, sizeof(tempBuffer_fix), "ExtraFire.Weapon%d", b + 1);

			ValueableVector<WeaponTypeClass*> buffer_Weapon;
			buffer_Weapon.Read(parserRules, pSection_rules, tempBuffer_fix);

			if (!buffer_Weapon.empty())
			{
				Nullable<CoordStruct> buffer_FLH;
				_snprintf_s(tempBuffer, sizeof(tempBuffer), "%s.FLH", tempBuffer_fix);
				buffer_FLH.Read(parserArt, pSection_Art, tempBuffer);

				Debug::Log("Reading ExtraFireData found for T[%s] Tag[%s] ! \n", pTech->get_ID(), tempBuffer_fix);

				_snprintf_s(tempBuffer_fix, sizeof(tempBuffer_fix), "ExtraFire.EliteWeapon%d", b + 1);

				ValueableVector<WeaponTypeClass*> buffer_Weapon_Elite;
				buffer_Weapon_Elite.Read(parserRules, pSection_rules, tempBuffer_fix);

				if (buffer_Weapon_Elite.empty())
					buffer_Weapon_Elite = buffer_Weapon;

				Nullable<CoordStruct> buffer_FLH_Elite;
				_snprintf_s(tempBuffer, sizeof(tempBuffer), "%s.FLH", tempBuffer_fix);
				buffer_FLH_Elite.Read(parserArt, pSection_Art, tempBuffer);

				if (!buffer_FLH_Elite.isset() || buffer_FLH_Elite.Get() == CoordStruct::Empty)
					buffer_FLH_Elite = buffer_FLH.Get();

				WeaponData.emplace_back(std::move(buffer_Weapon));
				WeaponFLHData.emplace_back(std::move(buffer_FLH.Get(CoordStruct::Empty)));
				EliteWeaponData.push_back(std::move(buffer_Weapon_Elite));
				EliteWeaponFLHData.emplace_back(std::move(buffer_FLH_Elite.Get()));
				++sizetotal;
			}
		}

		if (sizetotal > 0)
		{
			WeaponFLHData.resize(sizetotal);
			EliteWeaponFLHData.resize(sizetotal);
			WeaponData.resize(sizetotal);
			EliteWeaponData.resize(sizetotal);
		}
		else
		{
			WeaponFLHData.clear();
			EliteWeaponFLHData.clear();
			WeaponData.clear();
			EliteWeaponData.clear();
		}
	}
}