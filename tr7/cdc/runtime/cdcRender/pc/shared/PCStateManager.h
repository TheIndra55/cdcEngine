#pragma once

#include <Windows.h>
#include <d3d9.h>

#include "PCInternalResource.h"

namespace cdc
{
	class PCStateManager : public PCInternalResource
	{
	public:
		IDirect3DDevice9* m_pD3DDevice;
		bool m_bInScene;

		PCStateManager();

		bool OnCreateDevice();
		void OnDestroyDevice();
	};
}