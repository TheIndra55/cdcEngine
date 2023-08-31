#include "PCStateManager.h"
#include "PCDeviceManager.h"

cdc::PCStateManager::PCStateManager() : PCInternalResource()
{
	m_pD3DDevice = nullptr;
	m_bInScene = false;
	m_pIndexBuffer = nullptr;
	m_pVertexBuffer = nullptr;
	m_pVertexDeclaration = nullptr;
	m_vertexStride = 0;

	OnConstruct();
}

void cdc::PCStateManager::SetIndexBuffer(PCIndexBuffer* pNewIndexBuffer)
{
	IDirect3DIndexBuffer9* newBuffer = nullptr;

	if (pNewIndexBuffer)
	{
		newBuffer = pNewIndexBuffer->GetD3DIndexBuffer();
	}

	if (newBuffer != m_pIndexBuffer)
	{
		m_pD3DDevice->SetIndices(newBuffer);
		m_pIndexBuffer = newBuffer;
	}
}

void cdc::PCStateManager::SetVertexBuffer(PCVertexBuffer* pVertexBuffer, IDirect3DVertexDeclaration9* pOverrideDecl)
{
	IDirect3DVertexBuffer9* newBuffer = nullptr;
	IDirect3DVertexDeclaration9* newDecl = nullptr;
	unsigned __int16 newStride = 0;

	if (pVertexBuffer)
	{
		newBuffer = pVertexBuffer->GetD3DVertexBuffer();
		newDecl = pVertexBuffer->GetD3DVertexDeclaration();
		newStride = pVertexBuffer->GetStride();
	}

	if (pOverrideDecl)
	{
		newDecl = pOverrideDecl;
	}

	if (newDecl != m_pVertexDeclaration)
	{
		m_pVertexDeclaration = newDecl;

		if (newDecl)
		{
			m_pD3DDevice->SetVertexDeclaration(newDecl);
		}
	}

	if (newBuffer != m_pVertexBuffer || newStride != m_vertexStride)
	{
		m_pD3DDevice->SetStreamSource(NULL, newBuffer, 0, newStride);

		m_pVertexBuffer = newBuffer;
		m_vertexStride = newStride;
	}
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