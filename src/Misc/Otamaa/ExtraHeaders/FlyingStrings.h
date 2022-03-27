/*FlyingStrings.h
Useable to get out messages from units
Used to output Bounty messages
By AlexB and Joshy
*/

#pragma once
#include <vector>
#include <CoordStruct.h>
#include <ColorScheme.h>
#include <RectangleStruct.h>

class FlyingStrings
{
private:

	struct Item
	{
		CoordStruct coords;
		int created;
		wchar_t text[0x20];
		ColorScheme* color;

	};

	static const int Duration = 75;
	static std::vector<Item> data;

	static bool DrawAllowed(CoordStruct &nCoords);
	static void DrawIt(CoordStruct nCoords, Point2D &nPoint, Point2D &nRetPoint, RectangleStruct &nDrawRect, const wchar_t *pText, ColorScheme *pColor);

public:
	static void Add(const wchar_t *text, CoordStruct coords, ColorScheme *color);
	static void UpdateAll();
};
