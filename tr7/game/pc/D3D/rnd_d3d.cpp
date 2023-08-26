#include <intrin.h>

#include "rnd_d3d.h"
#include "d3dtextr.h"
#include "d3ddraw.h"

#include "game/pc/gamewindow.h"
#include "game/local/localstr.h"
#include "game/Scene/Scene.h"
#include "game/pc/cdcRenderLayer/RenderLayer.h"

#include "cdc/runtime/cdcRender/pc/shared/PCDeviceManager.h"

bool D3D_HasISSE;
bool D3D_HasMMX;

bool D3D_InNextGenMode;
bool D3D_InScene;

static DeviceCallback* s_pDeviceCallback;

bool D3D_PreInit()
{
	int regs[4];
	__cpuid(regs, 1);

	D3D_HasISSE = (regs[3] & 1 << 25) != 0;
	D3D_HasMMX = (regs[3] & 1 << 23) != 0;

	return cdc::PCDeviceManager::Create() != nullptr;
}

bool D3D_Init()
{
	if (!cdc::PCDeviceManager::s_pInstance->Init(gGameHwnd, &settings))
	{
		return false;
	}

	cdcRenderLayer::Create(gGameHwnd);
	D3DTEX_Init();

	s_pDeviceCallback = new DeviceCallback();

	return true;
}

void D3D_BeginScene(bool enableReflection)
{
	cdcRenderLayer::BeginFrame();

	if (D3D_InNextGenMode)
	{
		cdcRenderLayer::SetupViewport();

		if (enableReflection)
		{
			cdcRenderLayer::RenderReflectionBuffer();
		}

		cdcRenderLayer::D3D_BeginScene();
		D3D_InScene = true;
	}
	else
	{
		// TODO
	}
}

void D3D_EndScene()
{
	if (D3D_InScene)
	{
		D3D_InScene = false;

		if (D3D_InNextGenMode)
		{
			cdcRenderLayer::D3D_EndScene();
		}
		else
		{
			// TODO
		}

		cdcRenderLayer::EndFrame();

		if (!D3D_InNextGenMode && cdc::PCDeviceManager::s_pInstance->m_status == cdc::PCDeviceManager::kStatusOk)
		{
			// TODO
		}
	}
}

DeviceCallback::DeviceCallback() : PCInternalResource()
{
	OnConstruct();
}

bool DeviceCallback::OnCreateDevice()
{
	if (!m_bInitialized)
	{
		m_bInitialized = true;

		auto inNextGen = cdc::PCDeviceManager::s_pInstance->m_isPixelShader20;
		D3D_InNextGenMode = inNextGen;

		if (!inNextGen)
		{
			// TODO
		}

		localstr_set_gfx_gen(inNextGen);
		SceneLayer::SetEnabled(inNextGen);

		auto renderDevice = cdcRenderLayer::GetPCRD();
		renderDevice->ResolveDeferredReleases();

		D3DDRAW_Init();
	}

	return true;
}

void DeviceCallback::OnDestroyDevice()
{
	if (m_bInitialized)
	{
		m_bInitialized = false;

		InvalidateRect(gGameHwnd, NULL, TRUE);
	}
}