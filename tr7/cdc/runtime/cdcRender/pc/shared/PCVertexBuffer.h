#pragma once

#include <Windows.h>
#include <d3d9.h>

namespace cdc
{
	class PCVertexBuffer
	{
	public:
		virtual unsigned int GetBaseVertexIndex() = 0;
		virtual IDirect3DVertexBuffer9* GetD3DVertexBuffer() = 0;
		virtual IDirect3DVertexDeclaration9* GetD3DVertexDeclaration() = 0;
		virtual unsigned int GetNumVertices() = 0;
		virtual unsigned __int16 GetStride() = 0;
	};
}