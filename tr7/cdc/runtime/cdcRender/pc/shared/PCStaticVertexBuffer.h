#pragma once

#include "PCVertexBuffer.h"
#include "PCInternalResource.h"
#include "PCStaticPool.h"
#include "PCVertexFormat.h"

namespace cdc
{
	class PCStaticVertexBuffer : public PCVertexBuffer, public PCInternalResource
	{
	private:
		PCStaticPool* m_pStaticPool;
		PCStaticPool::Allocation<IDirect3DVertexBuffer9> m_allocation;

		unsigned int m_numVertices;
		unsigned int m_stride;
		unsigned int m_baseVertIndex;
		void* m_pVertexData;
		PCVertexFormat* m_pVertexFormat;
		bool m_ownsData;

	public:
		PCStaticVertexBuffer(PCStaticPool* pStaticPool);

		void Create(void* pVertexData, D3DVERTEXELEMENT9* pVertexElements, unsigned int stride, unsigned int numVertices);

		unsigned int GetBaseVertexIndex();
		IDirect3DVertexBuffer9* GetD3DVertexBuffer();
		IDirect3DVertexDeclaration9* GetD3DVertexDeclaration();
		unsigned int GetNumVertices();
		unsigned __int16 GetStride();

		bool OnCreateDevice();
		void OnDestroyDevice();
	};
}