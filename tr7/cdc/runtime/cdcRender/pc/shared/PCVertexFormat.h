#pragma once

#include <Windows.h>
#include <d3d9.h>

#include "PCInternalResource.h"

namespace cdc
{
	class PCVertexFormat : public PCInternalResource
	{
	public:
		D3DVERTEXELEMENT9* m_pVertexElements;
		IDirect3DVertexDeclaration9* m_pD3DVertexDeclaration;

		unsigned __int16 m_stride;
		PCVertexFormat* m_pNextFormat;

	public:
		PCVertexFormat(D3DVERTEXELEMENT9* pVertexElements);

		bool OnCreateDevice();
		void OnDestroyDevice();

		static PCVertexFormat* s_pFirstFormat;
		static PCVertexFormat* Create(D3DVERTEXELEMENT9* pVertexElements);
	};
}