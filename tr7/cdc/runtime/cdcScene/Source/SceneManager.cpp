#include "SceneManager.h"

cdc::SceneManager* cdc::SceneManager::s_instance = nullptr;

cdc::Scene* cdc::SceneManager::CreateScene(RenderDevice* pRenderDevice, SceneId id)
{
	auto scene = new Scene(pRenderDevice);

	return scene;
}

void cdc::SceneManager::Create()
{
	s_instance = new SceneManager();
}