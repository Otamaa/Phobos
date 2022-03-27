#include "FlyingStrings.h"

#include <MapClass.h>
#include <Phobos.CRT.h>
#include <TacticalClass.h>
#include <ColorScheme.h>
#include <RectangleStruct.h>

std::vector<FlyingStrings::Item> FlyingStrings::data;

bool FlyingStrings::DrawAllowed(CoordStruct &nCoords)
{
	if (!nCoords)
		return false;

	if (auto const pCell = MapClass::Instance->TryGetCellAt(nCoords))
	{
		if (pCell->IsFogged() || pCell->IsShrouded())
			return false;

		return true;
	}

	return false;
}

void FlyingStrings::DrawIt(CoordStruct nCoords, Point2D &nPoint, Point2D &nRetPoint, RectangleStruct &nDrawRect, const wchar_t *pText, ColorScheme *pColor)
{
	if (DrawAllowed(nCoords))
	{
		Fancy_Text_Print_Wide(
			&nRetPoint,
			pText,
			DSurface::Temp,
			&nDrawRect,
			&nPoint,
			pColor,
			0u,
			TextPrintType::LightShadow | TextPrintType::Center | TextPrintType::BrightColor
		);
	}
}

void FlyingStrings::Add(const wchar_t *text, CoordStruct coords, ColorScheme *color)
{
	Item item;
	item.coords = coords;
	item.created = Unsorted::CurrentFrame;
	item.color = color;
	PhobosCRT::wstrCopy(item.text, text, 0x20);

	data.push_back(item);
}

void FlyingStrings::UpdateAll()
{
	if (data.empty())
		return;

	for (auto const &DataItems : data)
	{
		Point2D point;
		Point2D retPoint;
		TacticalClass::Instance->CoordsToClient(DataItems.coords, &point);
		auto DrawingRect = DSurface::ViewBounds.get();

		if (Unsorted::CurrentFrame > DataItems.created + Duration - 70)
		{
			point.Y -= (Unsorted::CurrentFrame - DataItems.created);
			DrawIt(DataItems.coords, point, retPoint, DrawingRect, DataItems.text, DataItems.color);
		}
		else
		{
			DrawIt(DataItems.coords, point, retPoint, DrawingRect, DataItems.text, DataItems.color);
		}
	}

	auto const it = std::remove_if(data.begin(), data.end(), [](Item nItem) { return Unsorted::CurrentFrame > nItem.created + Duration || Unsorted::CurrentFrame < nItem.created; });

	if (it != data.end())
		data.erase(it);
}
