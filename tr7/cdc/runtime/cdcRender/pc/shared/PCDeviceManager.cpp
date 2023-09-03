#include "PCDeviceManager.h"

typedef IDirect3D9*(WINAPI* LPDIRECT3DCCREATE9)(UINT);

cdc::PCDeviceManager* cdc::PCDeviceManager::s_pInstance;

cdc::PCDeviceManager::PCDeviceManager(HINSTANCE pD3DLib, IDirect3D9* pD3D)
	: m_refCount(0), m_adapters(), m_bIsRecreatingResources(false), m_status(kStatusNotInitialized),
	  m_pFirstResource(nullptr), m_pLastResource(nullptr), m_pD3DDevice(nullptr), m_hFocusWindow(0), m_settings(),
	  m_d3dPresentParams(), m_d3dDevType(D3DDEVTYPE_HAL), m_d3dBehaviorFlags(0), m_d3dCaps(),
	  m_isPixelShader20(false), m_isPixelShader30(false), m_pCurrentContext(nullptr)
{
	m_pD3D = pD3D;

	EnumAdaptersAndModes(false);
}

bool cdc::PCDeviceManager::Init(HWND hFocusWindow, Settings* settings)
{
	m_hFocusWindow = hFocusWindow;

	if (!ValidateSettings(settings))
	{
		return false;
	}

	if (memcmp(&m_settings, settings, sizeof(Settings)) == 0 && m_pD3DDevice)
	{
		SettingsChanged();
		DestroyAttachedResources();
		CreateAttachedResources();

		if (m_status == kStatusOk)
		{
			return true;
		}

		ReleaseDevice(kStatusNotInitialized);
		CreateDevice(settings);

		if (m_status == kStatusOk)
		{
			return true;
		}
	}
	else
	{
		if (m_pD3DDevice)
		{
			ReleaseDevice(kStatusNotInitialized);
		}

		if (CreateDevice(settings))
		{
			return true;
		}
	}

	return false;
}

void cdc::PCDeviceManager::Destroy()
{
}

void cdc::PCDeviceManager::ReleaseDevice(Status status)
{
	m_status = status;

	for (auto resource = m_pFirstResource; resource != nullptr; resource = resource->m_pNext)
	{
		resource->OnDestroyDevice();
	}

	if (m_pD3DDevice)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = nullptr;
	}
}

bool cdc::PCDeviceManager::CreateDevice(Settings* settings)
{
	if (!m_pD3D)
	{
		m_status = kStatusCreateDeviceFailed;
		return false;
	}

	m_status = kStatusNotInitialized;

	if (settings->adapterId >= m_adapters.size())
	{
		m_status = kStatusInvalidSettings;
		return false;
	}

	auto adapter = m_adapters.at(settings->adapterId);

	memset(&m_d3dPresentParams, 0, sizeof(D3DPRESENT_PARAMETERS));
	m_d3dPresentParams.hDeviceWindow = NULL;
	m_d3dPresentParams.Flags = 0;
	m_d3dPresentParams.PresentationInterval = settings->enableVSync ? D3DPRESENT_INTERVAL_DEFAULT : D3DPRESENT_INTERVAL_IMMEDIATE;
	m_d3dPresentParams.MultiSampleType = settings->enableFSAA ? D3DMULTISAMPLE_NONE : D3DMULTISAMPLE_NONMASKABLE;
	m_d3dPresentParams.MultiSampleQuality = settings->enableFSAA ? adapter.maxMultiSampleQuality : 0;
	m_d3dPresentParams.SwapEffect = D3DSWAPEFFECT_DISCARD;

	if (settings->fullscreen)
	{
		// TODO
	}
	else
	{
		m_d3dPresentParams.Windowed = TRUE;
		m_d3dPresentParams.BackBufferWidth = 2;
		m_d3dPresentParams.BackBufferHeight = 2;
		m_d3dPresentParams.BackBufferFormat = adapter.backBufferFormat;
		m_d3dPresentParams.BackBufferCount = 1;
		m_d3dPresentParams.EnableAutoDepthStencil = FALSE;
		m_d3dPresentParams.AutoDepthStencilFormat = D3DFMT_UNKNOWN;
	}

	if (FAILED(m_pD3D->GetDeviceCaps(adapter.ordinal, m_d3dDevType, &m_d3dCaps)))
	{
		return false;
	}

	m_d3dBehaviorFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING;

	if (FAILED(m_pD3D->CreateDevice(adapter.ordinal, m_d3dDevType, m_hFocusWindow, m_d3dBehaviorFlags, &m_d3dPresentParams, &m_pD3DDevice)))
	{
		m_status = kStatusCreateResourceFailed;
		return false;
	}

	SettingsChanged();

	if (m_status != kStatusNotInitialized)
	{
		return false;
	}

	m_status = kStatusOk;

	CreateAttachedResources();
}

cdc::PCRenderContext* cdc::PCDeviceManager::CreateRenderContext(HWND hwnd, unsigned int width, unsigned int height)
{
	return new PCRenderContext(hwnd, width, height);
}

bool cdc::PCDeviceManager::CreateAttachedResources()
{
	m_bIsRecreatingResources = true;

	for (auto resource = m_pFirstResource; resource != nullptr; resource = resource->m_pNext)
	{
		if (!resource->OnCreateDevice())
		{
			m_bIsRecreatingResources = false;
			return false;
		}
	}

	m_bIsRecreatingResources = false;
	return true;
}

void cdc::PCDeviceManager::DestroyAttachedResources()
{
	for (auto resource = m_pFirstResource; resource != nullptr; resource = resource->m_pNext)
	{
		resource->OnDestroyDevice();
	}
}

void cdc::PCDeviceManager::OnCreateResourceFailed()
{
	ReleaseDevice(kStatusCreateResourceFailed);
}

void cdc::PCDeviceManager::AddDeviceResource(PCInternalResource* resource)
{
	if (!m_pFirstResource)
	{
		resource->m_pPrev = nullptr;
		m_pFirstResource = resource;
	}
	else
	{
		resource->m_pPrev = m_pLastResource;
		m_pLastResource->m_pNext = resource;
	}

	m_pLastResource = resource;
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
		AdapterInfo adapterInfo = {};

		m_pD3D->GetAdapterIdentifier(i, 0, &adapterInfo.d3dAdapterId);

		adapterInfo.ordinal = i;

		SelectAdapterDisplayFormat(&adapterInfo, force16Bit);
		SelectAdapterMultiSampleType(&adapterInfo);

		if (adapterCount > 1)
		{
			sprintf_s(adapterInfo.name, "%s (%d)", adapterInfo.d3dAdapterId.Description, i + 1);
		}
		else
		{
			sprintf_s(adapterInfo.name, "%s", adapterInfo.d3dAdapterId.Description);
		}

		m_adapters.push_back(adapterInfo);
	}
}

void cdc::PCDeviceManager::SelectAdapterDisplayFormat(AdapterInfo* adapterInfo, bool force16Bit)
{
	if (m_pD3D)
	{
		if (SUCCEEDED(m_pD3D->CheckDeviceFormat(adapterInfo->ordinal, m_d3dDevType, D3DFMT_X8R8G8B8, D3DUSAGE_RENDERTARGET, D3DRTYPE_SURFACE, D3DFMT_A8R8G8B8)) || force16Bit)
		{
			adapterInfo->displayFormat = D3DFMT_X8R8G8B8;
			adapterInfo->backBufferFormat = D3DFMT_A8R8G8B8;

			return;
		}

		if (SUCCEEDED(m_pD3D->CheckDeviceFormat(adapterInfo->ordinal, m_d3dDevType, D3DFMT_R5G6B5, D3DUSAGE_RENDERTARGET, D3DRTYPE_SURFACE, D3DFMT_R5G6B5)))
		{
			adapterInfo->displayFormat = D3DFMT_R5G6B5;
			adapterInfo->backBufferFormat = D3DFMT_R5G6B5;

			return;
		}
	}

	adapterInfo->displayFormat = D3DFMT_UNKNOWN;
	adapterInfo->backBufferFormat = D3DFMT_UNKNOWN;
}

void cdc::PCDeviceManager::SelectAdapterMultiSampleType(AdapterInfo* adapterInfo)
{
	// TODO
	adapterInfo->maxMultiSampleQuality = 3;
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
		if (!Direct3DCreate9 || (d3d9 = Direct3DCreate9(D3D_SDK_VERSION)) == nullptr)
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

void cdc::PCDeviceManager::SettingsChanged()
{
	m_isPixelShader30 = true;
	m_isPixelShader20 = true;
}

bool cdc::PCDeviceManager::GetAdapterRect(unsigned int adapterId, RECT* rect)
{
	return false;
}

bool cdc::PCDeviceManager::ValidateSettings(Settings* settings)
{
	return true;
}

cdc::PCDeviceManager::Settings::Settings()
{
	adapterId = 0;
	fullscreenModeId = 0;
	fullscreen = false;
	enableVSync = true;
	enableFSAA = false;
}