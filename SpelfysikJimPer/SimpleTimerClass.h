#ifndef _SIMPLE_TIMER_CLASS_H_
#define _SIMPLE_TIMER_CLASS_H_

#include <Windows.h>

class SimpleTimerClass
{
public:
	SimpleTimerClass(){
		ZeroMemory(this, sizeof(SimpleTimerClass));
		QueryPerformanceFrequency(&frequencyCount);
		countsPerSecond = double(frequencyCount.QuadPart);
		QueryPerformanceCounter(&frequencyCount);
		counterStart = frequencyCount.QuadPart;
	}
	~SimpleTimerClass(){}
	LARGE_INTEGER frequencyCount;
	LARGE_INTEGER currentTime;
	double countsPerSecond;
	double curTime;
	double deltaTime;
	__int64 counterStart;
	__int64 frameCount;
	int fps;
	void update();
};

#endif
