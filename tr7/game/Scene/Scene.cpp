#include "Scene.h"

#include "cdc/runtime/cdcScene/Source/SceneManager.h"

bool SceneLayer::s_enabled = false;

cdc::IScene* SceneLayer::s_pGlobalScene = nullptr;
cdc::SceneId SceneLayer::s_globalSceneId = { nullptr, 1 };

void SceneLayer::Init()
{
	cdc::SceneManager::Create();

	s_pGlobalScene = cdc::SceneManager::s_instance->CreateScene(cdc::RenderDevice::s_pcInstance, s_globalSceneId);
}

void SceneLayer::Update()
{
}

void SceneLayer::Render(cdc::ISceneCell* pStartCell)
{
	s_pGlobalScene->Render(nullptr);
}

void SceneLayer::SetEnabled(bool enabled)
{
	s_enabled = enabled;
}