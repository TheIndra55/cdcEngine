#pragma once

#include <Windows.h>
#include <d3d9.h>

#include "PCInternalResource.h"
#include "PCShaderBinary.h"

namespace cdc
{
	class PCPixelShader : public PCInternalResource
	{
	private:
		PCShaderBinary m_binary;
		unsigned int m_refCount;

	public:
		IDirect3DPixelShader9* m_pD3DPixelShader;

		PCPixelShader(unsigned int* pFunction, bool copyFunction);

		bool OnCreateDevice();
		void OnDestroyDevice();
	};
}