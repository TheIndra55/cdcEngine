#include "PCShaderTable.h"
#include "PCDeviceManager.h"

cdc::PCShaderTable::PCShaderTable(void* pShaderTableData)
{
	m_pOffsetTable = 0;
	m_tableSize = 0;
	m_pBasePtr = (char*)pShaderTableData;

	m_tableSize = (int)pShaderTableData >> 2;
	m_pOffsetTable = (unsigned int*)((char*)pShaderTableData + 8);
}

unsigned int cdc::PCShaderTable::GetSize()
{
	return m_tableSize;
}

unsigned int* cdc::PCShaderTable::GetShaderFunction(unsigned int index)
{
	return (unsigned int*)&m_pBasePtr[m_pOffsetTable[index]];
}

bool cdc::PCShaderTable::ShaderExists(unsigned int index)
{
	return m_pOffsetTable[index] != -1;
}

cdc::PCVertexShaderTable::PCVertexShaderTable(void* pShaderTableData, bool copyData, bool neverPrecreate) : PCShaderTable(pShaderTableData)
{
	m_pVertexShaders = nullptr;
	m_copyData = copyData;

	auto tableSize = GetSize();
	m_pVertexShaders = new cdc::PCVertexShader*[tableSize];
	memset(m_pVertexShaders, 0, tableSize * sizeof(cdc::PCVertexShader*));

	if (!neverPrecreate && cdc::PCDeviceManager::s_pInstance->GetSettings()->dontDeferShaderCreation)
	{
		for (unsigned int i = 0; i < tableSize; i++)
		{
			if (ShaderExists(i))
			{
				GetVertexShader(i);
			}
		}
	}
}

void cdc::PCVertexShaderTable::PreWarmShader(unsigned int index)
{
}

cdc::PCVertexShader* cdc::PCVertexShaderTable::GetVertexShader(unsigned int index)
{
	if (m_pVertexShaders[index])
	{
		return m_pVertexShaders[index];
	}

	auto shaderManager = cdc::PCDeviceManager::s_pInstance->m_pShaderManager;
	m_pVertexShaders[index] = shaderManager->CreateVertexShader(GetShaderFunction(index), m_copyData);

	return m_pVertexShaders[index];
}