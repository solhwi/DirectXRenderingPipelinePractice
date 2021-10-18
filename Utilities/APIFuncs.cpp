#include "stdafx.h"
#include "APIFuncs.h"

RECT APIFuncs::DefineRect(int x, int y, int width, int height)
{
	RECT rect = { x,y,x + width, y + height };
	return rect;
}

RECT APIFuncs::DefineRectCentered(int x, int y, int width, int height)
{
	RECT rect = { x - (width / 2), y - (height / 2), x + (width / 2), y + (height / 2) };
	return rect;
}

void APIFuncs::DrawLine(HDC hdc, int x1, int y1, int x2, int y2)
{
	MoveToEx(hdc, x1, y1, nullptr);
	LineTo(hdc, x2, y2);
}

void APIFuncs::DrawLine(HDC hdc, RECT r)
{
	MoveToEx(hdc, r.left, r.top, nullptr);
	LineTo(hdc, r.right, r.bottom);
}

void APIFuncs::DrawRect(HDC hdc, int x, int y, int width, int height)
{
	Rectangle(hdc, x, y, x + width, y + height);
}

void APIFuncs::DrawRect(HDC hdc, RECT r)
{
	Rectangle(hdc, r.left, r.top, r.right, r.bottom);
}

void APIFuncs::DrawCircle(HDC hdc, int x, int y, int width, int height)
{
	Ellipse(hdc, x, y, width, height);
}

void APIFuncs::DrawCircle(HDC hdc, RECT r)
{
	Ellipse(hdc, r.left, r.top, r.right, r.bottom);
}

bool APIFuncs::CheckMouseMoverOnRect(RECT rect, POINT pt)
{
	bool check = false;
	check |= pt.x > rect.left;
	check |= pt.x < rect.right;
	check |= pt.y > rect.top;
	check |= pt.y < rect.bottom;

	return check;
}
