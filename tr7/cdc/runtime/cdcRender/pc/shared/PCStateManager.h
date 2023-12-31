#pragma once

#include <Windows.h>
#include <d3d9.h>

#include "PCInternalResource.h"
#include "PCIndexBuffer.h"
#include "PCVertexBuffer.h"
#include "PCPixelShader.h"
#include "PCVertexShader.h"

#include "cdc/runtime/cdcMath/Matrix.h"

namespace cdc
{
	class PCStateManager : public PCInternalResource
	{
	public:
		IDirect3DDevice9* m_pD3DDevice;
		bool m_bInScene;

		IDirect3DIndexBuffer9* m_pIndexBuffer;
		IDirect3DVertexBuffer9* m_pVertexBuffer;
		IDirect3DVertexDeclaration9* m_pVertexDeclaration;
		unsigned __int16 m_vertexStride;

		PCPixelShader* m_pPixelShader;
		PCVertexShader* m_pVertexShader;

		unsigned int m_renderStates[210];

		PCStateManager();

		void SetRenderState(D3DRENDERSTATETYPE state, unsigned int value);
		void SetIndexBuffer(PCIndexBuffer* pNewIndexBuffer);
		void SetVertexBuffer(PCVertexBuffer* pVertexBuffer, IDirect3DVertexDeclaration9* pOverrideDecl);

		void SetPixelShader(PCPixelShader* pPixelShader);
		void SetVertexShader(PCVertexShader* pVertexShader);

		void SetPixelConstants(unsigned int index, const float* data, unsigned int count);
		void SetVertexConstants(unsigned __int16 index, const float* data, unsigned __int16 count);
		void SetVertexConstantMatrix4x4(unsigned __int16 index, Matrix* m);

		bool OnCreateDevice();
		void OnDestroyDevice();
	};
}