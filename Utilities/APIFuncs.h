#pragma once

#define VK_00 0x30
#define VK_01 0x31
#define VK_02 0x32
#define VK_03 0x33
#define VK_04 0x34
#define VK_05 0x35
#define VK_06 0x36
#define VK_07 0x37
#define VK_08 0x38
#define VK_09 0x39

class APIFuncs
{
public:
	static RECT DefineRect(int x, int y, int width, int height);
	static RECT DefineRectCentered(int x, int y, int width, int height);

	static void DrawLine(HDC hdc, int x1, int y1, int x2, int y2);
	static void DrawLine(HDC hdc, RECT r);
	static void DrawRect(HDC hdc, int x, int y, int width, int height);
	static void DrawRect(HDC hdc, RECT r);
	static void DrawCircle(HDC hdc, int x, int y, int width, int height);
	static void DrawCircle(HDC hdc, RECT r);

	static bool CheckMouseMoverOnRect(RECT rect, POINT pt);
};