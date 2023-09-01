#include "PCVertexShader.h"
#include "PCDeviceManager.h"

cdc::PCVertexShader::PCVertexShader(unsigned int* pFunction, bool copyFunction) : PCInternalResource(), m_binary(pFunction, copyFunction)
{
	m_pD3DVertexShader = nullptr;
	m_refCount = 1;

	OnConstruct();
}

bool cdc::PCVertexShader::OnCreateDevice()
{
	auto device = cdc::PCDeviceManager::s_pInstance->m_pD3DDevice;

	if (FAILED(device->CreateVertexShader((DWORD*)m_binary.m_pFunction, &m_pD3DVertexShader)))
	{
		OnCreateResourceFailed();
		return false;
	}

	return true;
}

void cdc::PCVertexShader::OnDestroyDevice()
{
}