#include "PCRenderDevice.h"

#include "cdc/runtime/cdcRender/pc/shared/PCDeviceManager.h"

cdc::RenderDevice* cdc::RenderDevice::s_pcInstance = nullptr;

cdc::PCRenderDevice::PCRenderDevice(void* hwnd, unsigned int width, unsigned int height, bool effectsEnabled) : PCInternalResource()
{
	OnConstruct();
}

void cdc::PCRenderDevice::ResolveDeferredReleases()
{
}

bool cdc::PCRenderDevice::OnCreateDevice()
{
	return true;
}

void cdc::PCRenderDevice::OnDestroyDevice()
{
}

void cdc::PCRenderDeviceCreate(void* hwnd, unsigned int width, unsigned int height, bool effectsEnabled)
{
	auto deviceManager = PCDeviceManager::Create();

	if (deviceManager->m_status == PCDeviceManager::kStatusNotInitialized)
	{
		RenderDevice::s_pcInstance = new PCRenderDevice(hwnd, width, height, effectsEnabled);
	}
	else
	{
		PCDeviceManager::Destroy();
	}
}