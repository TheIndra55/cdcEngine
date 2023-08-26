#include "PCStateManager.h"
#include "PCDeviceManager.h"

cdc::PCStateManager::PCStateManager() : PCInternalResource()
{
	m_pD3DDevice = nullptr;
	m_bInScene = false;

	OnConstruct();
}

bool cdc::PCStateManager::OnCreateDevice()
{
	m_pD3DDevice = cdc::PCDeviceManager::s_pInstance->m_pD3DDevice;

	if (m_pD3DDevice)
	{
		m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		m_pD3DDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
		m_pD3DDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
		m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, D3DFOG_LINEAR);
	}
	else
	{
		OnCreateResourceFailed();
		return false;
	}

	return true;
}

void cdc::PCStateManager::OnDestroyDevice()
{
	m_pD3DDevice = nullptr;
}