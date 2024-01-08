#include "PCDynamicIndexBuffer.h"

cdc::PCDynamicIndexBuffer::PCDynamicIndexBuffer(IDirect3DIndexBuffer9* pD3DIndexBuffer, void* pIndexData, unsigned int baseIndex, unsigned int numIndices)
{
	m_pD3DIndexBuffer = pD3DIndexBuffer;
	m_pIndexData = pIndexData;
	m_baseIndex = baseIndex;
	m_numIndices = numIndices;
}

unsigned int cdc::PCDynamicIndexBuffer::GetBaseIndex()
{
	return m_baseIndex;
}

unsigned __int16* cdc::PCDynamicIndexBuffer::Lock()
{
	return (unsigned __int16*)m_pIndexData;
}

IDirect3DIndexBuffer9* cdc::PCDynamicIndexBuffer::GetD3DIndexBuffer()
{
	return m_pD3DIndexBuffer;
}

unsigned int cdc::PCDynamicIndexBuffer::GetNumIndices()
{
	return m_numIndices;
}