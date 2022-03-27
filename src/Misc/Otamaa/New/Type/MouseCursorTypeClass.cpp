#include "MouseCursorTypeClass.h"
#include <Utilities/TemplateDef.h>

Enumerable<MouseCursorTypeClass>::container_t Enumerable<MouseCursorTypeClass>::Array;

const char* Enumerable<MouseCursorTypeClass>::GetMainSection()
{
	return "MouseCursors";
}

PhobosMap<Action, int> MouseCursorTypeClass::ActionCursor_Map;
PhobosMap<Action, bool> MouseCursorTypeClass::ActionCursorAllowShrouded_Map;

//init this somewhere more early than rules , because game also need to access the cursor !
void MouseCursorTypeClass::AddDefaults()
{
	FindOrAllocate_("Default", MouseCursor::DefaultCursors()[0]);

	MouseCursor::GetDefaultCursor(MouseCursorType::ParaDrop).Interval = 4;

	// issue #214: also animate the chronosphere cursor
	MouseCursor::GetDefaultCursor(MouseCursorType::Chronosphere).Interval = 4;

	// issue #1380: the iron curtain cursor
	MouseCursor::GetDefaultCursor(MouseCursorType::IronCurtain).Interval = 4;

	// animate the engineer damage cursor
	MouseCursor::GetDefaultCursor(MouseCursorType::Detonate).Interval = 4;

#pragma region MoveCursor
	FindOrAllocate_("MoveN", MouseCursor::DefaultCursors()[1]);
	FindOrAllocate_("MoveNE", MouseCursor::DefaultCursors()[2]);
	FindOrAllocate_("MoveE", MouseCursor::DefaultCursors()[3]);
	FindOrAllocate_("MoveSE", MouseCursor::DefaultCursors()[4]);
	FindOrAllocate_("MoveS", MouseCursor::DefaultCursors()[5]);
	FindOrAllocate_("MoveSW", MouseCursor::DefaultCursors()[6]);
	FindOrAllocate_("MoveW", MouseCursor::DefaultCursors()[7]);
	FindOrAllocate_("MoveNW", MouseCursor::DefaultCursors()[8]);
#pragma endregion

#pragma region NoMoveCursor
	FindOrAllocate_("NoMoveN", MouseCursor::DefaultCursors()[9]);
	FindOrAllocate_("NoMoveNE", MouseCursor::DefaultCursors()[10]);
	FindOrAllocate_("NoMoveE", MouseCursor::DefaultCursors()[11]);
	FindOrAllocate_("NoMoveSE", MouseCursor::DefaultCursors()[12]);
	FindOrAllocate_("NoMoveS", MouseCursor::DefaultCursors()[13]);
	FindOrAllocate_("NoMoveSW", MouseCursor::DefaultCursors()[14]);
	FindOrAllocate_("NoMoveW", MouseCursor::DefaultCursors()[15]);
	FindOrAllocate_("NoMoveNW", MouseCursor::DefaultCursors()[16]);
#pragma endregion

	FindOrAllocate_("Select", MouseCursor::DefaultCursors()[17]);

	FindOrAllocate_("Move", MouseCursor::DefaultCursors()[18]);
	FindOrAllocate_("NoMove", MouseCursor::DefaultCursors()[19]);

	FindOrAllocate_("Attack", MouseCursor::DefaultCursors()[20]);
	FindOrAllocate_("AttackOutOfRange", MouseCursor::DefaultCursors()[21]);

	FindOrAllocate_("AreaGuard", MouseCursor::DefaultCursors()[22]);

	FindOrAllocate_("Tote", MouseCursor::DefaultCursors()[23]);
	FindOrAllocate_("NoTote", MouseCursor::DefaultCursors()[24]);

	FindOrAllocate_("Enter", MouseCursor::DefaultCursors()[25]);
	FindOrAllocate_("NoEnter", MouseCursor::DefaultCursors()[26]);

	FindOrAllocate_("Deploy", MouseCursor::DefaultCursors()[27]);
	FindOrAllocate_("NoDeploy", MouseCursor::DefaultCursors()[28]);
	FindOrAllocate_("Undeploy", MouseCursor::DefaultCursors()[29]);

	FindOrAllocate_("Sell", MouseCursor::DefaultCursors()[30]);
	FindOrAllocate_("SellUnit", MouseCursor::DefaultCursors()[31]);
	FindOrAllocate_("NoSell", MouseCursor::DefaultCursors()[32]);

	FindOrAllocate_("Repair", MouseCursor::DefaultCursors()[33]);
	FindOrAllocate_("EngineerRepair", MouseCursor::DefaultCursors()[34]);
	FindOrAllocate_("NoRepair", MouseCursor::DefaultCursors()[35]);

	FindOrAllocate_("Waypoint", MouseCursor::DefaultCursors()[36]);
	FindOrAllocate_("Disguise", MouseCursor::DefaultCursors()[37]);

	FindOrAllocate_("IvanBomb", MouseCursor::DefaultCursors()[38]);
	FindOrAllocate_("MindControl", MouseCursor::DefaultCursors()[39]);
	FindOrAllocate_("RemoveSquid", MouseCursor::DefaultCursors()[40]);

	FindOrAllocate_("Crush", MouseCursor::DefaultCursors()[41]);

	FindOrAllocate_("SpyTech", MouseCursor::DefaultCursors()[42]);
	FindOrAllocate_("SpyPower", MouseCursor::DefaultCursors()[43]);

	FindOrAllocate_("Cursor_2C", MouseCursor::DefaultCursors()[44]);

	FindOrAllocate_("GIDeploy", MouseCursor::DefaultCursors()[45]);
	FindOrAllocate_("Ripple", MouseCursor::DefaultCursors()[46]);

	FindOrAllocate_("ParaDrop", MouseCursor::DefaultCursors()[47]);

	FindOrAllocate_("Rallypoint", MouseCursor::DefaultCursors()[48]);
	FindOrAllocate_("CloseWaypoint", MouseCursor::DefaultCursors()[49]);

	FindOrAllocate_("LightningStorm", MouseCursor::DefaultCursors()[50]);
	FindOrAllocate_("Detonate", MouseCursor::DefaultCursors()[51]);
	FindOrAllocate_("Demolish", MouseCursor::DefaultCursors()[52]);
	FindOrAllocate_("Nuke", MouseCursor::DefaultCursors()[53]);

	FindOrAllocate_("BlueCursor", MouseCursor::DefaultCursors()[54]);
	FindOrAllocate_("Power", MouseCursor::DefaultCursors()[55]);
	FindOrAllocate_("NoBlueCursor", MouseCursor::DefaultCursors()[56]);

	FindOrAllocate_("IronCurtain", MouseCursor::DefaultCursors()[57]);
	FindOrAllocate_("Chronosphere", MouseCursor::DefaultCursors()[58]);
	FindOrAllocate_("Disarm", MouseCursor::DefaultCursors()[59]);
	FindOrAllocate_("Disallowed", MouseCursor::DefaultCursors()[60]);

	FindOrAllocate_("Scroll", MouseCursor::DefaultCursors()[61]);
	FindOrAllocate_("ScrollESW", MouseCursor::DefaultCursors()[62]);
	FindOrAllocate_("ScrollSW", MouseCursor::DefaultCursors()[63]);
	FindOrAllocate_("ScrollNSW", MouseCursor::DefaultCursors()[64]);
	FindOrAllocate_("ScrollNW", MouseCursor::DefaultCursors()[65]);
	FindOrAllocate_("ScrollNEW", MouseCursor::DefaultCursors()[66]);
	FindOrAllocate_("ScrollNE", MouseCursor::DefaultCursors()[67]);
	FindOrAllocate_("ScrollNES", MouseCursor::DefaultCursors()[68]);
	FindOrAllocate_("ScrollES", MouseCursor::DefaultCursors()[69]);

	FindOrAllocate_("Protect2", MouseCursor::DefaultCursors()[70]);
	FindOrAllocate_("AttackOutOfRange2", MouseCursor::DefaultCursors()[71]);
	FindOrAllocate_("Deployinh", MouseCursor::DefaultCursors()[72]);
	FindOrAllocate_("InfantryAbsorb", MouseCursor::DefaultCursors()[73]);

	FindOrAllocate_("NoMindControl", MouseCursor::DefaultCursors()[74]);
	FindOrAllocate_("NoMindControlAlly", MouseCursor::DefaultCursors()[75]);

	FindOrAllocate_("SmallGuard", MouseCursor::DefaultCursors()[76]);
	FindOrAllocate_("SmallAttack", MouseCursor::DefaultCursors()[77]);

	FindOrAllocate_("Beacon", MouseCursor::DefaultCursors()[78]);

	FindOrAllocate_("ForceShield", MouseCursor::DefaultCursors()[79]);
	FindOrAllocate_("NoForceShield", MouseCursor::DefaultCursors()[80]);

	FindOrAllocate_("GeneticMutator", MouseCursor::DefaultCursors()[81]);
	FindOrAllocate_("AirStrike", MouseCursor::DefaultCursors()[82]);
	FindOrAllocate_("PsychicDominator", MouseCursor::DefaultCursors()[83]);
	FindOrAllocate_("PsychicReveal", MouseCursor::DefaultCursors()[84]);
	FindOrAllocate_("SpyPlane", MouseCursor::DefaultCursors()[85]);

	Debug::Log("MouseCursorType added [%d] , default !\n", Array.size());
}

void MouseCursorTypeClass::LoadFromINI(CCINIClass* pINI)
{
	const char* pName = this->Name.data();

	//Dont need to load these for default !
	if (_strcmpi(pName, "Default"))
	{
		INI_EX exINI(pINI);
		this->Cursor.Read(exINI, Enumerable<MouseCursorTypeClass>::GetMainSection(), pName, false);
	}
}

void MouseCursorTypeClass::LoadFromINIList_New(CCINIClass* pINI, bool bDebug)
{
	const char* pSection = GetMainSection();
	for (int i = 0; i < pINI->GetKeyCount(pSection); ++i)
	{
		const char* pKey = pINI->GetKeyName(pSection, i);
		if (auto pAlloc = FindOrAllocate(pKey))
		{
			pAlloc->LoadFromINI(pINI);
		}

		if (bDebug)
			Debug::Log("Allocating CursorType with Name[%s] at [%d] \n", pKey, i);
	}

	if (bDebug)
	{
		//Debug::Log("CursorType Array count currently [%d]\n", Array.size());

		for (size_t i = 0; i < Array.size(); ++i)
		{
			auto nArr = Array[i].get();
			auto nCur = nArr->Cursor.Get();
			Debug::Log("Cursor [%d=%s] <%d,%d,%d,%d,%d,%d,%d> \n", i,
				nArr->Name.data(),
				nCur.Frame,
				nCur.Count,
				nCur.Interval,
				nCur.MiniFrame,
				nCur.MiniCount,
				nCur.HotX,
				nCur.HotY
			);
		}
	}

}

template <typename T>
void MouseCursorTypeClass::Serialize(T& Stm)
{
	Stm
		.Process(this->Cursor)
		;
}

void MouseCursorTypeClass::LoadFromStream(PhobosStreamReader& Stm)
{
	this->Serialize(Stm);
}

void MouseCursorTypeClass::SaveToStream(PhobosStreamWriter& Stm)
{
	this->Serialize(Stm);
}

//save them to global S/L
MouseCursor* MouseCursorTypeClass::Actions::TempCursor = nullptr;
MouseCursorType MouseCursorTypeClass::Actions::TypeIndex = MouseCursorType::Default;