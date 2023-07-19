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
		};

		IDirect3D9* m_pD3D;

		bool m_bIsRecreatingResources;

		int m_refCount;

		Status m_status;

	private:

		PCStateManager* m_pStateManager;

		cdc::Array<cdc::PCDeviceManager::AdapterInfo> m_adapters;

	public:
		PCDeviceManager(HINSTANCE pD3DLib, IDirect3D9* pD3D);

		void AddDeviceResource(PCInternalResource* resource);
		void PostConstructorInit();
		void EnumAdaptersAndModes(bool force16Bit);

		static PCDeviceManager* s_pInstance;
		static PCDeviceManager* Create();
	};
}