#include "PCRenderContext.h"
#include "PCDeviceManager.h"

cdc::PCRenderContext::PCRenderContext(HWND hwnd, unsigned int width, unsigned int height) : PCInternalResource()
{
	m_pD3DSwapChain = nullptr;
	m_hwnd = hwnd;

	OnConstruct();
}

void cdc::PCRenderContext::Present(RECT* pSourceRect, RECT* pDestRect, HWND hOverrideWND)
{
	if (m_pD3DSwapChain)
	{
		m_pD3DSwapChain->Present(pSourceRect, pDestRect, hOverrideWND, NULL, 0);
	}
}

bool cdc::PCRenderContext::OnCreateDevice()
{
	auto deviceManager = cdc::PCDeviceManager::s_pInstance;
	auto device = deviceManager->m_pD3DDevice;

	if (deviceManager->m_settings.fullscreen)
	{
		// TODO
	}
	else
	{
		D3DPRESENT_PARAMETERS d3dpp;
		memcpy(&d3dpp, &deviceManager->m_d3dPresentParams, sizeof(d3dpp));

		d3dpp.hDeviceWindow = m_hwnd;
		d3dpp.EnableAutoDepthStencil = FALSE;
		d3dpp.AutoDepthStencilFormat = D3DFMT_UNKNOWN;
		d3dpp.Windowed = TRUE;

		if (SUCCEEDED(device->CreateAdditionalSwapChain(&d3dpp, &m_pD3DSwapChain)))
		{
			return true;
		}
	}

	OnCreateResourceFailed();
	return false;
}

void cdc::PCRenderContext::OnDestroyDevice()
{
	if (m_pD3DSwapChain)
	{
		m_pD3DSwapChain->Release();
		m_pD3DSwapChain = nullptr;
	}
}