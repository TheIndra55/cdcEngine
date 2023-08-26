#pragma once

#include <Windows.h>

#include "cdc/runtime/cdcRender/pc/shared/PCDeviceManager.h"

struct GameWindowStatus
{
	unsigned int screenWidth;
	unsigned int screenHeight;
};

extern HWND gInputHwnd;
extern HWND gGameHwnd;
extern HWND gForegroundHwnd;

extern cdc::PCDeviceManager::Settings settings;
extern GameWindowStatus gGameWindowStatus;

bool GAMEWINDOW_Poll();
bool GAMEWINDOW_ReadSettings();
void GAMEWINDOW_ReadIngameSettings();
void GAMEWINDOW_Init();