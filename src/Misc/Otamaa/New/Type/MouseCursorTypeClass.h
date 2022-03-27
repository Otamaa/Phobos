#pragma once

#include <Utilities/Enumerable.h>
#include <Utilities/Template.h>
#include <Utilities/GeneralUtils.h>

class MouseCursorTypeClass final : public Enumerable<MouseCursorTypeClass>
{
public:

	Valueable<MouseCursor> Cursor;
	MouseCursorTypeClass(const char* const pTitle) : Enumerable<MouseCursorTypeClass>(pTitle)
		, Cursor { }
	{ }

	MouseCursorTypeClass(const char* const pTitle, MouseCursor nDefault) : 
		Enumerable<MouseCursorTypeClass>(pTitle)
		, Cursor { nDefault }
	{ }

	virtual ~MouseCursorTypeClass() override = default;
	static void AddDefaults();

	virtual void LoadFromINI(CCINIClass* pINI) override;
	virtual void LoadFromStream(PhobosStreamReader& Stm);
	virtual void SaveToStream(PhobosStreamWriter& Stm);
	static void LoadFromINIList_New(CCINIClass* pINI, bool bDebug = false);

	static MouseCursorTypeClass* FindOrAllocate_(const char* Title, MouseCursor nDefault)
	{
		if (MouseCursorTypeClass* find = Find(Title))
			return find;

		Array.push_back(std::make_unique<MouseCursorTypeClass>(Title, nDefault));

		return Array.back().get();
	}

	class Actions
	{
	public:
		// actions for custom sw
		static const Action SuperWeaponAllowed = static_cast<Action>(0x7F);
		static const Action SuperWeaponDisallowed = static_cast<Action>(0x7E);

		static MouseCursorType TypeIndex;
		static MouseCursor* TempCursor;
	};

	static PhobosMap<Action, int> ActionCursor_Map;
	static PhobosMap<Action, bool> ActionCursorAllowShrouded_Map;

	static void Insert(int nCursorIndex, Action nAct, bool bAllowShrouded)
	{
		ActionCursor_Map[nAct] = nCursorIndex;
		ActionCursorAllowShrouded_Map[nAct] = bAllowShrouded;
	}

	static void Clear() // do nothing :p 
	{ }

	static void Cleanup()
	{
		Array.clear();
	}

private:
	template <typename T>
	void Serialize(T& Stm);
};