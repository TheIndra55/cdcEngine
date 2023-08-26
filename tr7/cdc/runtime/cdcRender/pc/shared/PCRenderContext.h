#pragma once

#include <Windows.h>
#include <d3d9.h>

#include "PCInternalResource.h"

namespace cdc
{
	class PCRenderContext : public PCInternalResource
	{
	private:
		IDirect3DSwapChain9* m_pD3DSwapChain;
		HWND m_hwnd;

	public:
		PCRenderContext(HWND hwnd, unsigned int width, unsigned int height);

		void Present(RECT* pSourceRect, RECT* pDestRect, HWND hOverrideWND);

		bool OnCreateDevice();
		void OnDestroyDevice();
	};
}