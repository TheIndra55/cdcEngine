#pragma once

#include <Windows.h>
#include <d3d9.h>

#include "PCInternalResource.h"

#include <cdc/runtime/cdcSys/Array.h>

namespace cdc
{
	class PCStaticPool : public PCInternalResource
	{
	public:
		template <typename T>
		struct Allocation
		{
			unsigned int m_offset;
			unsigned int m_size;
			T* m_buffer;
		};

	private:
		int m_VBufferSize;
		int m_IBufferSize;
		bool m_is32BitsIndex;

		cdc::Array<IDirect3DVertexBuffer9*> m_vertexBuffers;
		cdc::Array<IDirect3DIndexBuffer9*> m_indexBuffers;

	public:
		PCStaticPool(int VBufferSize, int IBufferSize, bool is32BitsIndex);

		// I don't think this is the original function prototype, but removing the reference means returning stack-allocated memory
		Allocation<IDirect3DVertexBuffer9>* AllocVertices(Allocation<IDirect3DVertexBuffer9>* result, unsigned int stride, unsigned int numVertices);
		Allocation<IDirect3DIndexBuffer9>* AllocIndices(Allocation<IDirect3DIndexBuffer9>* result, unsigned int numIndices);

		bool OnCreateDevice();
		void OnDestroyDevice();
	};
}