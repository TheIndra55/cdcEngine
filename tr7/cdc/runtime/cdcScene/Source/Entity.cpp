#include "Scene.h"

cdc::SceneEntity::SceneEntity(Scene* pScene)
{
	m_pScene = pScene;
	m_entityIndex = -1;
	m_pDrawable = nullptr;
}

void cdc::SceneEntity::SetMatrix(cdc::Matrix* matrix)
{
}

void cdc::SceneEntity::SetDrawable(IDrawable* drawable)
{
	m_pDrawable = drawable;
}

cdc::IDrawable* cdc::SceneEntity::GetDrawable()
{
	return m_pDrawable;
}

void cdc::SceneEntity::Update()
{
}