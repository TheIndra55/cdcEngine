#pragma once

#include "PCShaderLib.h"
#include "PCTexture.h"

#include "shared/PCInternalResource.h"
#include "shared/PCRenderContext.h"
#include "shared/PCStaticPool.h"

namespace cdc
{
	class RenderMesh;
	class RenderModelInstance;

	class RenderViewport
	{
	};

	class RenderDevice
	{
	public:
		virtual bool BeginFrame() = 0;
		virtual bool EndFrame() = 0;
		virtual bool BeginScene(RenderViewport* pViewport) = 0;
		virtual bool EndScene() = 0;
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
		PCStaticPool* m_pStaticPool;

		PCRenderDevice(void* hwnd, unsigned int width, unsigned int height, bool effectsEnabled);

		bool BeginFrame();
		bool EndFrame();

		bool BeginScene(RenderViewport* pViewport);
		bool EndScene();

		RenderMesh* CreateRenderModel(void* renderModelData, TextureMap** pcTextures, IShaderLib** shaderLibs, RenderMesh* pcRefData);
		RenderModelInstance* CreateRenderModelInstance(RenderMesh* renderMesh);

		void SetFullScreenAlpha(float alpha);

		void ResolveDeferredReleases();

		bool OnCreateDevice();
		void OnDestroyDevice();
	};

	void PCRenderDeviceCreate(void* hwnd, unsigned int width, unsigned int height, bool effectsEnabled);
}