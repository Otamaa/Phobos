#pragma once

#include <Phobos.CRT.h>
#include "Savegame.h"
#include "Constructs.h"

#include <algorithm>
#include <memory>
#include <vector>

#include <ArrayClasses.h>
#include <CCINIClass.h>

template <typename T> class Enumerable
{
	typedef std::vector<std::unique_ptr<T>> container_t;

public:
	static container_t Array;

	static int FindIndex(const char* Title)
	{
		auto result = std::find_if(Array.begin(), Array.end(), [Title](std::unique_ptr<T>& Item)
			{
				return _strcmpi(Item->Name, Title) == 0;
			});

		if (result == Array.end())
			return -1;

		return std::distance(Array.begin(), result);
	}

	static int FindIndexFromType(T* pType)
	{
		return (pType) ? FindIndex(pType->Name.data()):-1;
	}

	static T* Find(const char* Title)
	{
		int result = FindIndex(Title);
		result = result > (int)Array.size() ? (Array.empty() ? -1:0) : result;
		return (result < 0) ? nullptr : Array[static_cast<size_t>(result)].get();
	}

	static T* Find_AtArr(const char* Title)
	{
		auto result = FindIndex(Title);
		return Array.at(result).get();
	}

	static T* FindFromIndex_AtArr(int Idx)
	{
		return Array.at(Idx).get();
	}

	static T* FindFromIndex(int Idx)
	{
		Idx = Idx > (int)Array.size() ? (Array.empty() ? -1:0) : Idx;
		return ((Idx < 0)) ? nullptr : Array[static_cast<size_t>(Idx)].get();
	}

	static T* FindOrAllocate(const char* Title)
	{
		if (T* find = Find(Title))
			return find;

		Array.push_back(std::make_unique<T>(Title));

		return Array.back().get();
	}

	static void Clear()
	{
		Debug::Log("%s Clearing Array Count [%d] ! \n", typeid(T).name(), Array.size());
		Array.clear();
	}

	static void LoadFromINIList(CCINIClass* pINI, bool bDebug = false)
	{
		if (!pINI)
			return;

		const char* section = GetMainSection();

		if (!pINI->GetSection(section))
			return;

		for (int i = 0; i < pINI->GetKeyCount(section); ++i)
		{
			if (pINI->ReadString(section, pINI->GetKeyName(section, i), "", Phobos::readBuffer))
			{
				if (auto const pItem = FindOrAllocate(Phobos::readBuffer))
				{
					if (bDebug)
						Debug::Log("%s Reading[%d] %s \"%s\".\n", typeid(T).name(), i, section, Phobos::readBuffer);

					pItem->LoadFromINI(pINI);
				}
				else
				{
					if (bDebug)
						Debug::Log("%s Error Creating[%d] %s \"%s\"!\n", typeid(T).name(), i, section, Phobos::readBuffer);
				}
			}
		}
	}

	static bool LoadGlobals(PhobosStreamReader& Stm)
	{
		Clear();

		size_t Count = 0;
		if (!Stm.Load(Count))
			return false;


		for (size_t i = 0; i < Count; ++i)
		{
			void* oldPtr = nullptr;
			decltype(Name) name;

			if (!Stm.Load(oldPtr) || !Stm.Load(name))
				return false;

			//Debug::Log("Loading %s Global Save For [%s] , Idx[i] \n", TypeName<T>::Get(), name, i);

			auto newPtr = FindOrAllocate(name);
			PhobosSwizzle::Instance.RegisterChange(oldPtr, newPtr);

			newPtr->LoadFromStream(Stm);
		}

		return true;
	}

	static bool SaveGlobals(PhobosStreamWriter& Stm)
	{
		Stm.Save(Array.size());

		//int nIdx = 0;
		for (const auto& item : Array)
		{
			// write old pointer and name, then delegate
			//Debug::Log("Saving %s Global Save For [%s] , Idx[i] \n", TypeName<T>::Get(), item->Name, ++nIdx);
			Stm.Save(item.get());
			Stm.Save(item->Name);
			item->SaveToStream(Stm);
		}

		return true;
	}

	static const char* GetMainSection();

	Enumerable(const char* Title)
	{
		this->Name = Title;
	}

	virtual ~Enumerable() = default;

	virtual void LoadFromINI(CCINIClass* pINI) { }

	virtual void LoadFromStream(PhobosStreamReader& Stm) = 0;

	virtual void SaveToStream(PhobosStreamWriter& Stm) = 0;

	PhobosFixedString<32> Name;
};