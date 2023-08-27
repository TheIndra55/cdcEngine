#include "PCStaticPool.h"
#include "PCDeviceManager.h"

cdc::PCStaticPool::PCStaticPool(int VBufferSize, int IBufferSize, bool is32BitsIndex) : PCInternalResource(),
	m_vertexBuffers(), m_indexBuffers()
{
	m_VBufferSize = VBufferSize;
	m_IBufferSize = IBufferSize;
	m_is32BitsIndex = is32BitsIndex;

	OnConstruct();
}

cdc::PCStaticPool::Allocation<IDirect3DVertexBuffer9>* cdc::PCStaticPool::AllocVertices(Allocation<IDirect3DVertexBuffer9>* result, unsigned int stride, unsigned int numVertices)
{
	auto device = cdc::PCDeviceManager::s_pInstance;
	auto size = numVertices * stride;

	m_vertexBuffers.resize(m_vertexBuffers.size() + 1);

	if (FAILED(device->m_pD3DDevice->CreateVertexBuffer(m_VBufferSize, 0, 0, D3DPOOL_MANAGED, &m_vertexBuffers.back(), NULL)) )
	{
		device->OnCreateResourceFailed();
	}

	result->m_offset = 0;
	result->m_size = size;
	result->m_buffer = m_vertexBuffers.back();

	return result;
}

cdc::PCStaticPool::Allocation<IDirect3DIndexBuffer9>* cdc::PCStaticPool::AllocIndices(Allocation<IDirect3DIndexBuffer9>* result, unsigned int numIndices)
{
	auto device = cdc::PCDeviceManager::s_pInstance;

	auto size = numIndices * (m_is32BitsIndex ? 4 : 2);
	auto format = m_is32BitsIndex ? D3DFMT_INDEX32 : D3DFMT_INDEX16;

	m_indexBuffers.resize(m_indexBuffers.size() + 1);

	if (FAILED(device->m_pD3DDevice->CreateIndexBuffer(m_IBufferSize, 0, format, D3DPOOL_MANAGED, &m_indexBuffers.back(), NULL)))
	{
		device->OnCreateResourceFailed();
	}

	result->m_offset = 0;
	result->m_size = size;
	result->m_buffer = m_indexBuffers.back();

	return result;
}

bool cdc::PCStaticPool::OnCreateDevice()
{
	return true;
}

void cdc::PCStaticPool::OnDestroyDevice()
{
	for (int i = 0; i < m_vertexBuffers.size(); i++)
	{
		m_vertexBuffers[i]->Release();
	}

	for (int i = 0; i < m_indexBuffers.size(); i++)
	{
		m_vertexBuffers[i]->Release();
	}
}