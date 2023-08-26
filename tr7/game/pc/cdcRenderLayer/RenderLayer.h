#pragma once

#include <Windows.h>

#include "game/Stream/stream.h"

#include "cdc/runtime/cdcRender/pc/PCRenderDevice.h"

class cdcRenderLayer
{
public:
	static cdc::PCRenderDevice* GetPCRD(); 

	static void DRAW_PrepareObjectForDraw(Object* object);

	static void SetupViewport();
	static void RenderReflectionBuffer();

	static void BeginFrame();
	static void EndFrame();

	static void D3D_BeginScene();
	static void D3D_EndScene();

	static bool Create(HWND hWnd);
};