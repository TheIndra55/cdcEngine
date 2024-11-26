#include <Windows.h>
#include <stdbool.h>

#include "game/Save/MSSave.h"
#include "game/local/localstr.h"
#include "game/Main.h"
#include "D3D/rnd_d3d.h"
#include "gamewindow.h"
#include "timer.h"
#include "setup.h"

#include "cdc/runtime/cdcFile/DebugFileSystem.h"

HANDLE hMainThread;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow)
{
	_strlwr(lpCmdLine);

	DuplicateHandle(GetCurrentProcess(), GetCurrentThread(), GetCurrentProcess(), &hMainThread, 0, FALSE, DUPLICATE_SAME_ACCESS);
	SetThreadAffinityMask(GetCurrentThread(), 1);

	bool firstTime = !GAMEWINDOW_ReadSettings();
	GAMEWINDOW_ReadIngameSettings();

	int forceOptions = strstr(lpCmdLine, "-setup") != 0;
	if (strstr(lpCmdLine, "-config") != 0)
	{
		forceOptions = 2;
	}

	cdc::DebugFileSystem::Create();

	MSSave::Create();

	InitFS();
	TIMER_Init();
	InitArchive();
	SetupBuildDir("PC-W");

	if (!localstr_verifyexistence())
	{
		SetupBuildDir("PCENGLISH");
	}

	localstr_set_language(language_default);

	SETUP_LocalizeSetupDialog();

	if (!D3D_PreInit())
	{
		auto text = localstr_get(18);
		auto caption = localstr_get(17);

		MessageBoxA(NULL, text, caption, MB_OK);

		return 1;
	}

	if (!D3D_HasISSE)
	{
		auto text = localstr_get(44);
		auto caption = localstr_get(17);

		MessageBoxA(NULL, text, caption, MB_OK);

		return 1;
	}

	if (!D3D_HasMMX)
	{
		auto text = localstr_get(28);
		auto caption = localstr_get(17);

		MessageBoxA(NULL, text, caption, MB_OK);

		return 1;
	}

	GAMEWINDOW_Init();
	D3D_Init();

	MainG2();

	return 0;
}