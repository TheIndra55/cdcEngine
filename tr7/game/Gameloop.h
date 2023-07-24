#pragma once

struct GameTracker
{
};

extern GameTracker gameTrackerX;

void GAMELOOP_AllocStaticMemory();
void GAMELOOP_InitGameTracker();
void GAMELOOP_SystemInit();