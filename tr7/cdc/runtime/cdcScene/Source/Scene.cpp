#include "Scene.h"

cdc::Scene::Scene(RenderDevice* pRenderDevice) : m_entities()
{
	m_pRenderDevice = pRenderDevice;
}

void cdc::Scene::Release()
{
}

void cdc::Scene::Render(RenderViewport* viewport)
{
}

cdc::ISceneEntity* cdc::Scene::CreateEntity()
{
	auto entity = new SceneEntity(this);

	m_entities.push_back(entity);

	return entity;
}