#include "input.h"

#include "game/pc/D3D/inpsku.h"

bool g_doneInit = false;
bool g_disabled = false;

static unsigned int s_saveStatusStackDepth = 0;

void INPUT_Process()
{
}

void INPUT_ResetStatus()
{
}

void INPUT_Init()
{
	g_doneInit = true;
	s_saveStatusStackDepth = 0;

	InputDevice::Create();
}