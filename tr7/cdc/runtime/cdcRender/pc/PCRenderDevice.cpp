#include "PCRenderDevice.h"
#include "PCRenderModel.h"
#include "PCShaderLib.h"
#include "PCTexture.h"

#include "cdc/runtime/cdcRender/pc/shared/PCDeviceManager.h"

cdc::RenderDevice* cdc::RenderDevice::s_pcInstance = nullptr;

cdc::PCRenderDevice::PCRenderDevice(void* hwnd, unsigned int width, unsigned int height, bool effectsEnabled) : PCInternalResource()
{
	m_isInScene = false;
	m_isInFrame = false;
	m_isFrameFailed = false;

	m_pDefaultContext = cdc::PCDeviceManager::s_pInstance->CreateRenderContext((HWND)hwnd, width, height);
	m_pStaticPool = new PCStaticPool(0x7FFF80, 0x400000, false);

	OnConstruct();
}

bool cdc::PCRenderDevice::BeginFrame()
{
	m_isFrameFailed = true;
	m_isInFrame = true;

	auto stateManager = cdc::PCDeviceManager::s_pInstance->m_pStateManager;

	if (FAILED(stateManager->m_pD3DDevice->BeginScene()))
	{
		return false;
	}

	stateManager->m_bInScene = true;

	m_isFrameFailed = false;
	m_isInFrame = true;

	return true;
}

bool cdc::PCRenderDevice::EndFrame()
{
	if (m_isFrameFailed)
	{
		m_isInFrame = false;
		m_isFrameFailed = false;
	}
	else
	{
		m_isInFrame = false;

		auto stateManager = cdc::PCDeviceManager::s_pInstance->m_pStateManager;
		stateManager->m_pD3DDevice->EndScene();
		stateManager->m_bInScene = false;

		m_pDefaultContext->Present(NULL, NULL, NULL);
	}

	return true;
}

bool cdc::PCRenderDevice::BeginScene(RenderViewport* pViewport)
{
	return true;
}

bool cdc::PCRenderDevice::EndScene()
{
	return true;
}

cdc::RenderMesh* cdc::PCRenderDevice::CreateRenderModel(void* renderModelData, TextureMap** pcTextures, IShaderLib** shaderLibs, RenderMesh* pcRefData)
{
	return new PCRenderModel((PCModelData*)renderModelData, pcTextures, shaderLibs, pcRefData, this);
}

cdc::RenderModelInstance* cdc::PCRenderDevice::CreateRenderModelInstance(RenderMesh* renderMesh)
{
	return new PCRenderModelInstance((cdc::PCRenderModel*)renderMesh, this);
}

void cdc::PCRenderDevice::SetFullScreenAlpha(float alpha)
{
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

	if (deviceManager->m_status == PCDeviceManager::kStatusNotInitialized || true)
	{
		RenderDevice::s_pcInstance = new PCRenderDevice(hwnd, width, height, effectsEnabled);
	}
	else
	{
		PCDeviceManager::Destroy();
	}
}