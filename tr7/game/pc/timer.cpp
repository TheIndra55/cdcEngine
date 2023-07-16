#include "timer.h"

#include <Windows.h>

static unsigned __int64 lasttime;
static unsigned __int64 timeus;
static unsigned __int64 curtime;

int TIMER_TimeScale = 4096;
int TIMER_HoldTime = 0;

void TIMER_Init()
{
	lasttime = WIN_TimeUS();
}

unsigned __int64 WIN_TimeUS()
{
	LARGE_INTEGER time;
	LARGE_INTEGER frequency;

	QueryPerformanceCounter(&time);
	QueryPerformanceFrequency(&frequency);

	timeus = (unsigned __int64)(244 * TIMER_TimeScale * time.QuadPart) / frequency.QuadPart;

	return timeus;
}

unsigned __int64 TIMER_GetTimeUS()
{
	// TODO
	return 0;
}

unsigned __int64 TIMER_GetTimeMS()
{
	return TIMER_GetTimeUS() / 1000;
}

unsigned __int64 TIMER_TimeDiff(unsigned __int64 prev)
{
	return TIMER_GetTimeUS() - prev;
}