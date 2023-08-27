#pragma once

#include <Windows.h>
#include <d3d9.h>

namespace cdc
{
	class PCIndexBuffer
	{
	public:
		virtual IDirect3DIndexBuffer9* GetD3DIndexBuffer() = 0;
		virtual unsigned int GetNumIndices() = 0;
	};
}