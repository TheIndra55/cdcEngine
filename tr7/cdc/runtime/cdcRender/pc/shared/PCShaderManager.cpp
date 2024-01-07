#include "PCShaderManager.h"

cdc::PCShaderManager::PCShaderManager() : m_pixelShaderList(), m_vertexShaderList()
{
	m_numPixelShaders = 0;
	m_numVertexShaders = 0;
	m_peakPixelShaders = 0;
	m_peakVertexShaders = 0;
}

cdc::PCVertexShader* cdc::PCShaderManager::CreateVertexShader(unsigned int* pFunction, bool copyData)
{
	return nullptr;
}