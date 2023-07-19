#include "PCDeviceManager.h"

typedef IDirect3D9*(WINAPI* LPDIRECT3DCCREATE9)(UINT);

cdc::PCDeviceManager* cdc::PCDeviceManager::s_pInstance;

cdc::PCDeviceManager::PCDeviceManager(HINSTANCE pD3DLib, IDirect3D9* pD3D)
	: m_refCount(0), m_adapters(), m_bIsRecreatingResources(false), m_status(kStatusNotInitialized)
{
	m_pD3D = pD3D;

	EnumAdaptersAndModes(false);
}

void cdc::PCDeviceManager::AddDeviceResource(PCInternalResource* resource)
{
}

void cdc::PCDeviceManager::PostConstructorInit()
{
	m_pStateManager = new PCStateManager();
}

void cdc::PCDeviceManager::EnumAdaptersAndModes(bool force16Bit)
{
	m_adapters.clear();

	auto adapterCount = m_pD3D->GetAdapterCount();

	for (unsigned int i = 0; i < adapterCount; i++)
	{
	}
}

cdc::PCDeviceManager* cdc::PCDeviceManager::Create()
{
	if (!s_pInstance)
	{
		auto d3dLib = LoadLibraryA("d3d9.dll");

		if (!d3dLib)
		{
			return nullptr;
		}

		auto Direct3DCreate9 = (LPDIRECT3DCCREATE9)GetProcAddress(d3dLib, "Direct3DCreate9");

		IDirect3D9* d3d9;
		if (!Direct3DCreate9 || (d3d9 = Direct3DCreate9(D3D_SDK_VERSION)) != nullptr)
		{
			FreeLibrary(d3dLib);

			return nullptr;
		}

		if (d3d9->GetAdapterCount() == 0)
		{
			return nullptr;
		}

		s_pInstance = new PCDeviceManager(d3dLib, d3d9);

		if (s_pInstance->m_adapters.size() == 0)
		{
			delete s_pInstance;

			return nullptr;
		}

		s_pInstance->PostConstructorInit();
	}

	s_pInstance->m_refCount++;

	return s_pInstance;
}