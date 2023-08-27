#pragma once

#include "Scene.h"

namespace cdc
{
	class SceneId
	{
	public:
		const char* m_name;
		int m_id;
	};

	class SceneManager
	{
	public:
		Scene* CreateScene(RenderDevice* pRenderDevice, SceneId id);

		static void Create();
		static SceneManager* s_instance;
	};
}