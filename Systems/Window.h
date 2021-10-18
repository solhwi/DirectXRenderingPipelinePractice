#pragma once

struct WinDesc
{
	wstring AppName;
	HINSTANCE instance;
	HWND handle;
	UINT width;
	UINT height;
};

class Window
{
public:
	Window(WinDesc desc);
	~Window();

	WPARAM Run();

private:
	static LRESULT CALLBACK WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);

	static WinDesc desc;
	static class Program* program;
};