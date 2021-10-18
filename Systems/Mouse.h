#pragma once

#define MAX_INPUT_MOUSE 8

class Mouse : public Singleton<Mouse>
{
public:
	friend class Singleton<Mouse>;
	
	void SetHandle(HWND handle) { this->handle = handle; }

	void Update();

	LRESULT InputProc(UINT message, WPARAM wParam, LPARAM lParam);

	void Print();

	Vector3 GetPosition() { return position; }
	Vector3 GetMoveValue() { return wheelMoveValue; }

	bool Down(DWORD button) { return buttonMap[button] == BUTTON_INPUT_STATUS_DOWN; }
	bool Up(DWORD button) { return buttonMap[button] == BUTTON_INPUT_STATUS_UP; }
	bool Press(DWORD button) { return buttonMap[button] == BUTTON_INPUT_STATUS_PRESS; }

private:
	Mouse();

	HWND handle;
	Vector3 position;

	byte buttonStatus[MAX_INPUT_MOUSE];
	byte buttonOldStatus[MAX_INPUT_MOUSE];
	byte buttonMap[MAX_INPUT_MOUSE];

	Vector3 wheelStatus;
	Vector3 wheelOldStatus;
	Vector3 wheelMoveValue;

	DWORD timeDblClk;
	DWORD startDblClk[MAX_INPUT_MOUSE];
	int buttonCount[MAX_INPUT_MOUSE];

	enum
	{
		MOUSE_ROTATION_NONE = 0,
		MOUSE_ROTATION_LEFT,
		MOUSE_ROTATION_RIGHT
	};

	enum
	{
		BUTTON_INPUT_STATUS_NONE = 0,
		BUTTON_INPUT_STATUS_DOWN,
		BUTTON_INPUT_STATUS_UP,
		BUTTON_INPUT_STATUS_PRESS,
		BUTTON_INPUT_STATUS_DBLCLK
	};

	RECT mouseRect;
	wstring mouseStr;
};