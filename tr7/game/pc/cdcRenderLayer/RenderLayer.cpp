#include "RenderLayer.h"

#include "game/pc/gamewindow.h"

#include "cdc/runtime/cdcRender/pc/PCRenderDevice.h"

static cdc::PCRenderDevice* s_cdcDev;

cdc::PCRenderDevice* cdcRenderLayer::GetPCRD()
{
	return s_cdcDev;
}

void cdcRenderLayer::DRAW_PrepareObjectForDraw(Object* object)
{
} 

void cdcRenderLayer::SetupViewport()
{
}

void cdcRenderLayer::RenderReflectionBuffer()
{
}

void cdcRenderLayer::BeginFrame()
{
}

void cdcRenderLayer::EndFrame()
{
	s_cdcDev->SetFullScreenAlpha(0.f);
	s_cdcDev->EndFrame();
}

void cdcRenderLayer::D3D_BeginScene()
{
}

void cdcRenderLayer::D3D_EndScene()
{
}

bool cdcRenderLayer::Create(HWND hWnd)
{
	cdc::PCRenderDeviceCreate(hWnd, gGameWindowStatus.screenWidth, gGameWindowStatus.screenHeight, true);
	s_cdcDev = (cdc::PCRenderDevice*)cdc::RenderDevice::s_pcInstance;

	return true;
}