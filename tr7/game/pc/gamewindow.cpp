#include "gamewindow.h"

HWND gInputHwnd;
HWND gGameHwnd;
HWND gForegroundHwnd;

cdc::PCDeviceManager::Settings settings;

bool GAMEWINDOW_Poll()
{
	MSG msg;
	while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return true;
}

bool GAMEWINDOW_ReadSettings()
{
	return true;
}

void GAMEWINDOW_ReadIngameSettings()
{
}

LRESULT CALLBACK RegularWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return 0;
}

void GAMEWINDOW_Init()
{
	WNDCLASS WndClass = {};

	WndClass.lpfnWndProc = RegularWndProc;
	WndClass.hInstance = GetModuleHandleA(NULL);
	WndClass.lpszClassName = "TOMBRAIDER";
	WndClass.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);

	RegisterClass(&WndClass);

	if (settings.fullscreen)
	{
		RECT rect;
		cdc::PCDeviceManager::s_pInstance->GetAdapterRect(0 /* adapterId */, &rect);

		gInputHwnd = CreateWindowEx(
			WS_EX_TOPMOST,
			"TOMBRAIDER",
			"Tomb Raider: Legend",
			WS_POPUP | WS_SYSMENU,
			rect.left,
			rect.top,
			rect.right - rect.left,
			rect.bottom - rect.top,
			NULL,
			NULL,
			WndClass.hInstance,
			NULL);

		gGameHwnd = gInputHwnd;
		gForegroundHwnd = gInputHwnd;

		ShowWindow(gInputHwnd, SW_MAXIMIZE);
	}
	else
	{
		RECT rect;
		rect.left = 0;
		rect.top = 0;
		rect.right = 640;
		rect.bottom = 480;

		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

		gInputHwnd = CreateWindowEx(
			0,
			"TOMBRAIDER",
			"Tomb Raider: Legend",
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			rect.right - rect.left,
			rect.bottom - rect.top,
			NULL,
			NULL,
			WndClass.hInstance,
			NULL);

		gGameHwnd = gInputHwnd;
		gForegroundHwnd = gInputHwnd;

		ShowWindow(gInputHwnd, SW_SHOW);
	}
}