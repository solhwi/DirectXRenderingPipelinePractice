#pragma once

class Time : public Singleton<Time>
{
public:
	friend class Singleton<Time>;

	static bool Stopped() { return isTimerStopped; }
	static float Delta() { return isTimerStopped ? 0.0f : timeElapsed; }

	void Update();
	void Print();

	void Start();
	void Stop();

	float FPS() const { return framePerSecond; }
	float Running() const { return runningTime; }

private:
	Time();

	static bool isTimerStopped;
	static float timeElapsed;

	INT64 ticksPerSecond = 0;
	INT64 currentTime = 0;
	INT64 lastTime = 0;
	INT64 lastFPSUpdate = 0;
	INT64 fpsUpdateInterval = 0;

	UINT frameCount = 0;
	float runningTime = 0.0f;
	float framePerSecond = 0.0f;

	RECT fpsRect = { 0 };
	wstring fpsStr = { 0 };
};