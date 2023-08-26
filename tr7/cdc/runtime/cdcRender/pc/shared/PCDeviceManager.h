#pragma once

#include <Windows.h>
#include <d3d9.h>

#include "PCStateManager.h"
#include "PCRenderContext.h"

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

			_D3DFORMAT displayFormat;
			_D3DFORMAT backBufferFormat;

			unsigned int maxMultiSampleQuality;
		};

		class Settings
		{
		public:
			unsigned int adapterId;

			unsigned int fullscreenModeId;
			bool fullscreen;

			bool enableVSync;
			bool enableFSAA;

			Settings();
		};

		PCInternalResource* m_pFirstResource;
		PCInternalResource* m_pLastResource;

		IDirect3D9* m_pD3D;
		IDirect3DDevice9* m_pD3DDevice;
		D3DCAPS9 m_d3dCaps;

		bool m_bIsRecreatingResources;
		Settings m_settings;

		bool m_isPixelShader20;
		bool m_isPixelShader30;

		D3DPRESENT_PARAMETERS m_d3dPresentParams;
		D3DDEVTYPE m_d3dDevType;
		unsigned int m_d3dBehaviorFlags;

		cdc::PCRenderContext* m_pCurrentContext;

		int m_refCount;

		Status m_status;

		HWND m_hFocusWindow;

		PCStateManager* m_pStateManager;

	private:

		cdc::Array<cdc::PCDeviceManager::AdapterInfo> m_adapters;

	public:
		PCDeviceManager(HINSTANCE pD3DLib, IDirect3D9* pD3D);

		bool Init(HWND hFocusWindow, Settings* settings);

		void ReleaseDevice(Status status);
		bool CreateDevice(Settings* settings);
		PCRenderContext* CreateRenderContext(HWND hwnd, unsigned int width, unsigned int height);

		bool CreateAttachedResources();
		void DestroyAttachedResources();

		void OnCreateResourceFailed();

		void AddDeviceResource(PCInternalResource* resource);
		void PostConstructorInit();
		void EnumAdaptersAndModes(bool force16Bit);
		void SelectAdapterDisplayFormat(AdapterInfo* adapterInfo, bool force16Bit);
		void SelectAdapterMultiSampleType(AdapterInfo* adapterInfo);
		void SettingsChanged();
		bool GetAdapterRect(unsigned int adapterId, RECT* rect);
		bool ValidateSettings(Settings* settings);

		static PCDeviceManager* s_pInstance;
		static PCDeviceManager* Create();
		static void Destroy();
	};
}