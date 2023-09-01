#include "PCPixelShader.h"
#include "PCDeviceManager.h"

cdc::PCPixelShader::PCPixelShader(unsigned int* pFunction, bool copyFunction) : PCInternalResource(), m_binary(pFunction, copyFunction)
{
	m_pD3DPixelShader = nullptr;
	m_refCount = 1;

	OnConstruct();
}

bool cdc::PCPixelShader::OnCreateDevice()
{
	auto device = cdc::PCDeviceManager::s_pInstance->m_pD3DDevice;

	if (FAILED(device->CreatePixelShader((DWORD*)m_binary.m_pFunction, &m_pD3DPixelShader)))
	{
		OnCreateResourceFailed();
		return false;
	}

	return true;
}

void cdc::PCPixelShader::OnDestroyDevice()
{
}