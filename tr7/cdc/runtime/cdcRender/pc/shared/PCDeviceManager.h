#pragma once

#include <Windows.h>
#include <d3d9.h>

#include "PCStateManager.h"

#include "cdc/runtime/cdcSys/Array.h"

namespace cdc
{
	class PCDeviceManager
	{
	public:
		enum Status
		{
			kStatusOk,
			kStatusNotInitialized = 0xFFFFFFFF,
			kStatusDeviceLost = 0xFFFFFFFE,
			kStatusInvalidSettings = 0xFFFFFFFD,
			kStatusCreateResourceFailed = 0xFFFFFFFC,
			kStatusCreateDeviceFailed = 0xFFFFFFFB
		};

		struct AdapterInfo
		{
			unsigned int ordinal;
			char name[512];
			D3DADAPTER_IDENTIFIER9 d3dAdapterId;
		};

		class Settings
		{
		};

		PCInternalResource* m_pFirstResource;
		PCInternalResource* m_pLastResource;

		IDirect3D9* m_pD3D;
		IDirect3DDevice9* m_pD3DDevice;

		bool m_bIsRecreatingResources;

		int m_refCount;

		Status m_status;

		HWND m_hFocusWindow;

	private:

		PCStateManager* m_pStateManager;

		cdc::Array<cdc::PCDeviceManager::AdapterInfo> m_adapters;

	public:
		PCDeviceManager(HINSTANCE pD3DLib, IDirect3D9* pD3D);

		bool Init(HWND hFocusWindow, Settings* settings);

		void ReleaseDevice(Status status);
		bool CreateDevice(Settings* settings);

		bool CreateAttachedResources();

		void AddDeviceResource(PCInternalResource* resource);
		void PostConstructorInit();
		void EnumAdaptersAndModes(bool force16Bit);
		bool GetAdapterRect(unsigned int adapterId, RECT* rect);

		static PCDeviceManager* s_pInstance;
		static PCDeviceManager* Create();
	};
}