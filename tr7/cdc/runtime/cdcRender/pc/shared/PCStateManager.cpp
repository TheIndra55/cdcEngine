#include "PCStateManager.h"
#include "PCDeviceManager.h"

cdc::PCStateManager::PCStateManager() : PCInternalResource(), m_renderStates{}
{
	m_pD3DDevice = nullptr;
	m_bInScene = false;
	m_pIndexBuffer = nullptr;
	m_pVertexBuffer = nullptr;
	m_pVertexDeclaration = nullptr;
	m_vertexStride = 0;
	m_pPixelShader = nullptr;
	m_pVertexShader = nullptr;

	OnConstruct();
}

void cdc::PCStateManager::SetRenderState(D3DRENDERSTATETYPE state, unsigned int value)
{
	if (m_renderStates[state] != value)
	{
		m_pD3DDevice->SetRenderState(state, value);
		m_renderStates[state] = value;
	}
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

void cdc::PCStateManager::SetPixelShader(PCPixelShader* pPixelShader)
{
	if (pPixelShader != m_pPixelShader)
	{
		if (pPixelShader)
		{
			m_pD3DDevice->SetPixelShader(pPixelShader->m_pD3DPixelShader);
			m_pPixelShader = pPixelShader;
		}
		else
		{
			m_pD3DDevice->SetPixelShader(NULL);
			m_pPixelShader = nullptr;
		}
	}
}

void cdc::PCStateManager::SetVertexShader(PCVertexShader* pVertexShader)
{
	if (pVertexShader != m_pVertexShader)
	{
		if (pVertexShader)
		{
			m_pD3DDevice->SetVertexShader(pVertexShader->m_pD3DVertexShader);
			pVertexShader = pVertexShader;
		}
		else
		{
			m_pD3DDevice->SetVertexShader(NULL);
			pVertexShader = nullptr;
		}
	}
}

void cdc::PCStateManager::SetPixelConstants(unsigned int index, const float* data, unsigned int count)
{
	m_pD3DDevice->SetPixelShaderConstantF(index, data, count);
}

void cdc::PCStateManager::SetVertexConstants(unsigned __int16 index, const float* data, unsigned __int16 count)
{
	m_pD3DDevice->SetPixelShaderConstantF(index, data, count);
}

void cdc::PCStateManager::SetVertexConstantMatrix4x4(unsigned __int16 index, Matrix* m)
{
	float data[16];

	data[0]  = m->col0.x;
	data[1]  = m->col1.x;
	data[2]  = m->col2.x;
	data[3]  = m->col3.x;
	data[4]  = m->col0.y;
	data[5]  = m->col1.y;
	data[6]  = m->col2.y;
	data[7]  = m->col3.y;
	data[8]  = m->col0.z;
	data[9]  = m->col1.z;
	data[10] = m->col2.z;
	data[11] = m->col3.z;
	data[12] = m->col0.w;
	data[13] = m->col1.w;
	data[14] = m->col2.w;
	data[15] = m->col3.w;

	SetVertexConstants(index, data, 4);
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