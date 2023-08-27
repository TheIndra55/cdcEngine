#pragma once

#include "PCIndexBuffer.h"
#include "PCInternalResource.h"
#include "PCStaticPool.h"

namespace cdc
{
	class PCStaticIndexBuffer : public PCIndexBuffer, public PCInternalResource
	{
	private:
		PCStaticPool* m_pStaticPool;
		PCStaticPool::Allocation<IDirect3DIndexBuffer9> m_allocation;

		unsigned int m_numIndices;
		unsigned int m_startIndex;
		unsigned __int16* m_pIndexData;

	public:
		PCStaticIndexBuffer(PCStaticPool* pStaticPool);

		void Create(unsigned __int16* pIndexData, unsigned int numIndices);

		IDirect3DIndexBuffer9* GetD3DIndexBuffer();
		unsigned int GetNumIndices();

		bool OnCreateDevice();
		void OnDestroyDevice();
	};
}