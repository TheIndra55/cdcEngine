#pragma once

#include "shared/PCInternalResource.h"
#include "shared/PCRenderContext.h"

namespace cdc
{
	class RenderDevice
	{
	public:
		virtual bool BeginFrame() = 0;
		virtual bool EndFrame() = 0;
		virtual void SetFullScreenAlpha(float alpha) = 0;

		static RenderDevice* s_pcInstance;
	};

	class PCRenderDevice : public RenderDevice, public PCInternalResource
	{
	private:
		bool m_isInScene;
		bool m_isInFrame;
		bool m_isFrameFailed;

		PCRenderContext* m_pDefaultContext;

	public:
		PCRenderDevice(void* hwnd, unsigned int width, unsigned int height, bool effectsEnabled);

		bool BeginFrame();
		bool EndFrame();
		void SetFullScreenAlpha(float alpha);

		void ResolveDeferredReleases();

		bool OnCreateDevice();
		void OnDestroyDevice();
	};

	void PCRenderDeviceCreate(void* hwnd, unsigned int width, unsigned int height, bool effectsEnabled);
}