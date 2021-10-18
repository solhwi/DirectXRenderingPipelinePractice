#include "stdafx.h"
#include "Time.h"

bool Time::isTimerStopped = true;
float Time::timeElapsed = 0.0f;

void Time::Update()
{
	if (isTimerStopped) return;

	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
	timeElapsed = static_cast<float>(currentTime - lastTime) / static_cast<float>(ticksPerSecond);
	runningTime += timeElapsed;

	frameCount++;

	if (currentTime - lastFPSUpdate >= fpsUpdateInterval)
	{
		float tempCurrentTime = static_cast<float>(currentTime) / static_cast<float>(ticksPerSecond);
		float tempLastTime = static_cast<float>(lastFPSUpdate) / static_cast<float>(ticksPerSecond);
		framePerSecond = static_cast<float>(frameCount) / (tempCurrentTime - tempLastTime);

		lastFPSUpdate = currentTime;
		frameCount = 0;
	}

	lastTime = currentTime;
}

void Time::Print()
{
	if (instance == nullptr) return;

	fpsStr = L"FPS : " + to_wstring(FPS());
	//DrawText(hdc, fpsStr.c_str(), -1, &fpsRect, DT_CENTER | DT_VCENTER);
}

void Time::Start()
{
	if (!isTimerStopped)
		assert(false);

	QueryPerformanceCounter((LARGE_INTEGER*)&lastTime);
	isTimerStopped = false;
}

void Time::Stop()
{
	if (isTimerStopped)
		assert(false);

	INT64 stopTime = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&stopTime);
	runningTime += static_cast<float>(stopTime - lastTime) / static_cast<float>(ticksPerSecond);
	isTimerStopped = true;
}

Time::Time()
{
	QueryPerformanceFrequency((LARGE_INTEGER*)&ticksPerSecond);
	fpsUpdateInterval = ticksPerSecond >> 1;

	fpsRect = { WinMaxWidth - 300, 0 , WinMaxWidth, 100 };
	fpsStr = L"";
}
