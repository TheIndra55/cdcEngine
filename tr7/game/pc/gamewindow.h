#pragma once

#include <Windows.h>

extern HWND gInputHwnd;
extern HWND gGameHwnd;
extern HWND gForegroundHwnd;

bool GAMEWINDOW_ReadSettings();
void GAMEWINDOW_ReadIngameSettings();
void GAMEWINDOW_Init();