#include "Gameloop.h"

GameTracker gameTrackerX;

void GAMELOOP_AllocStaticMemory()
{
}

void GAMELOOP_InitGameTracker()
{

}

void GAMELOOP_SystemInit()
{
	GAMELOOP_AllocStaticMemory();

	GAMELOOP_InitGameTracker();
}