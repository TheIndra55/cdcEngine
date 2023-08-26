#include <intrin.h>

#include "rnd_d3d.h"
#include "d3dtextr.h"

#include "game/pc/gamewindow.h"

#include "cdc/runtime/cdcRender/pc/shared/PCDeviceManager.h"

bool D3D_HasISSE;
bool D3D_HasMMX;

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

	D3DTEX_Init();

	s_pDeviceCallback = new DeviceCallback();

	return true;
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