#pragma once

void TIMER_Init();

unsigned __int64 WIN_TimeUS();
unsigned __int64 TIMER_GetTimeUS();
unsigned __int64 TIMER_GetTimeMS();
unsigned __int64 TIMER_TimeDiff(unsigned __int64 prev);