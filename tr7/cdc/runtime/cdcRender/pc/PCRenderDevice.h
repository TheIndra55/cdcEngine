#pragma once

#include "cdc/runtime/cdcRender/pc/shared/PCInternalResource.h"

namespace cdc
{
	class RenderDevice
	{
	public:
		static RenderDevice* s_pcInstance;
	};

	class PCRenderDevice : public RenderDevice, public PCInternalResource
	{
	public:
		PCRenderDevice(void* hwnd, unsigned int width, unsigned int height, bool effectsEnabled);

		void ResolveDeferredReleases();

		bool OnCreateDevice();
		void OnDestroyDevice();
	};

	void PCRenderDeviceCreate(void* hwnd, unsigned int width, unsigned int height, bool effectsEnabled);
}