#include "PCStaticVertexBuffer.h"

cdc::PCStaticVertexBuffer::PCStaticVertexBuffer(PCStaticPool* pStaticPool) : PCInternalResource(), m_allocation()
{
	m_pStaticPool = pStaticPool;
	m_numVertices = 0;
	m_stride = 0;
	m_baseVertIndex = 0;
	m_pVertexData = nullptr;
	m_ownsData = false;
}

void cdc::PCStaticVertexBuffer::Create(void* pVertexData, D3DVERTEXELEMENT9* pVertexElements, unsigned int stride, unsigned int numVertices)
{
	m_pVertexFormat = PCVertexFormat::Create(pVertexElements);
	m_numVertices = numVertices;
	m_stride = stride;
	m_pVertexData = pVertexData;
	m_ownsData = false;

	OnConstruct();
}

unsigned int cdc::PCStaticVertexBuffer::GetBaseVertexIndex()
{
	return m_baseVertIndex;
}

IDirect3DVertexBuffer9* cdc::PCStaticVertexBuffer::GetD3DVertexBuffer()
{
	return m_allocation.m_buffer;
}

IDirect3DVertexDeclaration9* cdc::PCStaticVertexBuffer::GetD3DVertexDeclaration()
{
	return m_pVertexFormat->m_pD3DVertexDeclaration;
}

unsigned int cdc::PCStaticVertexBuffer::GetNumVertices()
{
	return m_numVertices;
}

unsigned __int16 cdc::PCStaticVertexBuffer::GetStride()
{
	return m_stride;
}

bool cdc::PCStaticVertexBuffer::OnCreateDevice()
{
	m_pStaticPool->AllocVertices(&m_allocation, m_stride, m_numVertices);

	auto buffer = m_allocation.m_buffer;
	void* pVertexData;

	buffer->Lock(m_allocation.m_offset, m_allocation.m_size, &pVertexData, 0);
	memcpy(pVertexData, m_pVertexData, m_numVertices * m_stride);
	buffer->Unlock();

	return true;
}

void cdc::PCStaticVertexBuffer::OnDestroyDevice()
{
}