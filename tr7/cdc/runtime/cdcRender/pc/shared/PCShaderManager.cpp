#include "PCShaderManager.h"
#include "PCShaderBinary.h"

cdc::PCShaderManager::PCShaderManager() : m_pixelShaderList(), m_vertexShaderList()
{
	m_numPixelShaders = 0;
	m_numVertexShaders = 0;
	m_peakPixelShaders = 0;
	m_peakVertexShaders = 0;
}

template <class T>
T* cdc::PCShaderManager::CreateShader(unsigned int* pData, bool copyFunction)
{
	//cdc::PCShaderBinary binary(pData, false);

	auto shader = new T(pData, copyFunction);

	return shader;
}

cdc::PCVertexShader* cdc::PCShaderManager::CreateVertexShader(unsigned int* pFunction, bool copyData)
{
	return CreateShader<PCVertexShader>(pFunction, copyData);
}