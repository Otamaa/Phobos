#pragma once
#include <LineTrail.h>

#include <Utilities/Macro.h>
#include <Utilities/Container.h>
#include <Utilities/TemplateDef.h>


struct LineTrailData
{
public:

	CoordStruct LineTrailFLH;
	ColorStruct LineTrailColor;
	int LineTrailColorDecrement;

	bool operator==(const LineTrailData& that) const { return std::memcmp(this, &that, sizeof(LineTrailData)) == 0; }
	bool operator!=(const LineTrailData& that) const { return std::memcmp(this, &that, sizeof(LineTrailData)) != 0; }
	operator bool() const { return LineTrailColor != ColorStruct::Empty;}

	static const void Read(std::vector<LineTrailData>& nVec, INI_EX &parser, const char* pSection)
	{
		if (!pSection)
			return;

		char tempBuffer[32];

		for (size_t i = 0; ; ++i)
		{
			Nullable<ColorStruct> tempColor;
			_snprintf_s(tempBuffer, sizeof(tempBuffer), "LineTrail%dColor", i);
			tempColor.Read(parser, pSection, tempBuffer);

			if (!tempColor.isset() || tempColor.Get() == ColorStruct::Empty)
				break;

			Valueable<CoordStruct> tempflh;
			_snprintf_s(tempBuffer, sizeof(tempBuffer), "LineTrail%dFLH", i);
			tempflh.Read(parser, pSection, tempBuffer);

			Valueable<int> tempDecrement;
			_snprintf_s(tempBuffer, sizeof(tempBuffer), "LineTrail%dColorDecrement", i);
			tempDecrement.Read(parser, pSection, tempBuffer);

			nVec.push_back({ tempflh, tempColor,tempDecrement });
		}
	}

};

class NOVTABLE LineTrailExt final :public LineTrail
{
public:

	static bool _DeallocateLineTrail(FootClass* pTech);
	static bool _DeallocateLineTrail(BulletClass* pBullet);
	static bool _DeallocateLineTrail(ObjectClass* pObject);
	static void _DetachLineTrail(ObjectClass* pThis);
	static bool _CreateLineTrail(DynamicVectorClass<LineTrail*>& nVec, ObjectClass* pOwner, ColorStruct nColor, int nDecrement, CoordStruct nFLH = CoordStruct::Empty);
	static bool _ConstructLineTrail(ObjectClass* pThis);
	static void _ClearPhobosMap();
	static void _AnnounceInvalidPtr(DynamicVectorClass<LineTrail*>& nVec, void* ptr);
	static ColorStruct _OverrideColor(ColorStruct& nColorInput);
	static PhobosMap<LineTrailExt*, CoordStruct> LineTrailMap;
	static bool LoadGlobals(PhobosStreamReader& Stm);
	static bool SaveGlobals(PhobosStreamWriter& Stm);

	//void LoadFromStream(PhobosStreamReader& Stm);
	//void SaveToStream(PhobosStreamWriter& Stm);

};