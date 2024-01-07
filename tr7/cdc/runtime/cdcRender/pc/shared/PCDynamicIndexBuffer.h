#pragma once

#include "PCIndexBuffer.h"

namespace cdc
{
	class PCDynamicIndexBuffer : public PCIndexBuffer
	{
	private:
		IDirect3DIndexBuffer9* m_pD3DIndexBuffer;

		void* m_pIndexData;
		unsigned int m_baseIndex;
		unsigned int m_numIndices;

	public:
		PCDynamicIndexBuffer(IDirect3DIndexBuffer9* pD3DIndexBuffer, void* pIndexData, unsigned int baseIndex, unsigned int numIndices);

		unsigned int GetBaseIndex();

		IDirect3DIndexBuffer9* GetD3DIndexBuffer();
		unsigned int GetNumIndices();
	};
}