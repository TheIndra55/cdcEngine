#pragma once

#include "cdc/runtime/cdcRender/pc/PCRenderDevice.h"

#include "Entity.h"

namespace cdc
{
	class IScene
	{
	public:
		virtual void Release() = 0;
		virtual void Render(RenderViewport* viewport) = 0;
		virtual ISceneEntity* CreateEntity() = 0;
	};

	class Scene : public IScene
	{
	private:
		RenderDevice* m_pRenderDevice;

		cdc::Array<cdc::SceneEntity*> m_entities;

	public:
		Scene(RenderDevice* pRenderDevice);

		void Release();
		void Render(RenderViewport* viewport);
		ISceneEntity* CreateEntity();
	};
}