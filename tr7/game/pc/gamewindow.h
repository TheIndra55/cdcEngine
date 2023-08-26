#pragma once

#include <Windows.h>

#include "cdc/runtime/cdcRender/pc/shared/PCDeviceManager.h"

extern HWND gInputHwnd;
extern HWND gGameHwnd;
extern HWND gForegroundHwnd;

extern cdc::PCDeviceManager::Settings settings;

bool GAMEWINDOW_Poll();
bool GAMEWINDOW_ReadSettings();
void GAMEWINDOW_ReadIngameSettings();
void GAMEWINDOW_Init();