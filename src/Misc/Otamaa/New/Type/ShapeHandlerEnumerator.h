#pragma once

#include <Utilities/Enumerable.h>
#include <Utilities/Template.h>
#include <Utilities/GeneralUtils.h>

class ShapeHandlerEnumerator final : public Enumerable<ShapeHandlerEnumerator>
{
public :
	SHPStruct* LoadedShape;

	ShapeHandlerEnumerator(const char* const pTitle) :
		Enumerable<ShapeHandlerEnumerator>(pTitle)
		, LoadedShape(nullptr)
	{ FetchSHP(); }

	virtual ~ShapeHandlerEnumerator() override
	{
		if (LoadedShape)
		{
			GameDelete(LoadedShape);
			LoadedShape = nullptr;
		}
	};

	virtual void LoadFromStream(PhobosStreamReader& Stm)
	{
		Stm.Process(LoadedShape);
	}

	virtual void SaveToStream(PhobosStreamWriter& Stm)
	{
		Stm.Process(LoadedShape);
	}

	virtual void LoadFromINI(CCINIClass* pINI) override { };
	virtual void FetchSHP();

	static void Clear() // do nothing :p
	{ }

	static void Cleanup()
	{
		Array.clear();
	}

	SHPStruct* GetOrDefaultShape(SHPStruct* pDefault = nullptr) const
	{return (LoadedShape) ? LoadedShape : pDefault; }

	SHPStruct* operator ()()
	{ return LoadedShape; }
};