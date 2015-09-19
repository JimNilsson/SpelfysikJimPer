#include "SimpleTimerClass.h"

void SimpleTimerClass::update()
{
	frameCount++;
	QueryPerformanceCounter(&currentTime);
	double lastTime = curTime;
	curTime = double(currentTime.QuadPart - counterStart) / countsPerSecond;
	deltaTime = curTime - lastTime;
	fps = static_cast<int>(1.0f / deltaTime);
}