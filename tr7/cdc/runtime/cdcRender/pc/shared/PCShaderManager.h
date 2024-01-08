#pragma once

#include "PCPixelShader.h"
#include "PCVertexShader.h"

#include "cdc/runtime/cdcSys/SList.h"

namespace cdc
{
	class PCShaderManager
	{
	private:
		unsigned int m_numPixelShaders;
		unsigned int m_numVertexShaders;
		unsigned int m_peakPixelShaders;
		unsigned int m_peakVertexShaders;

		cdc::SList<cdc::PCPixelShader*> m_pixelShaderList;
		cdc::SList<cdc::PCVertexShader*> m_vertexShaderList;

	public:
		PCShaderManager();

		template <class T>
		T* CreateShader(unsigned int* pData, bool copyFunction);

		PCVertexShader* CreateVertexShader(unsigned int* pFunction, bool copyData);
	};
}