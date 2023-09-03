#include "PCStaticIndexBuffer.h"

cdc::PCStaticIndexBuffer::PCStaticIndexBuffer(PCStaticPool* pStaticPool) : PCInternalResource(), m_allocation()
{
	m_pStaticPool = pStaticPool;
	m_numIndices = 0;
	m_startIndex = 0;
	m_pIndexData = 0;
}

void cdc::PCStaticIndexBuffer::Create(unsigned __int16* pIndexData, unsigned int numIndices)
{
	m_pIndexData = pIndexData;
	m_numIndices = numIndices;
	m_startIndex = 0;

	OnConstruct();
}

IDirect3DIndexBuffer9* cdc::PCStaticIndexBuffer::GetD3DIndexBuffer()
{
	return m_allocation.m_buffer;
}

unsigned int cdc::PCStaticIndexBuffer::GetNumIndices()
{
	return m_numIndices;
}

bool cdc::PCStaticIndexBuffer::OnCreateDevice()
{
	m_pStaticPool->AllocIndices(&m_allocation, m_numIndices);

	if (m_pStaticPool->m_is32BitsIndex)
	{
		 // TODO
	}
	else
	{
		auto buffer = m_allocation.m_buffer;
		void* pIndexData;

		buffer->Lock(m_allocation.m_offset, m_allocation.m_size, &pIndexData, 0);
		memcpy(pIndexData, m_pIndexData, m_numIndices * 2);
		buffer->Unlock();
	}

	return true;
}

void cdc::PCStaticIndexBuffer::OnDestroyDevice()
{
}