#pragma once

#include <Windows.h>
#include <d3d9.h>

#include "PCInternalResource.h"
#include "PCShaderBinary.h"

namespace cdc
{
	class PCVertexShader : public PCInternalResource
	{
	private:
		PCShaderBinary m_binary;
		unsigned int m_refCount;

	public:
		IDirect3DVertexShader9* m_pD3DVertexShader;

		PCVertexShader(unsigned int* pFunction, bool copyFunction);

		bool OnCreateDevice();
		void OnDestroyDevice();
	};
}